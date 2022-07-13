<<<<<<< HEAD
=======
// SPDX-License-Identifier: GPL-2.0-or-later
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
/******************************************************************************
 * vlanproc.c	VLAN Module. /proc filesystem interface.
 *
 *		This module is completely hardware-independent and provides
 *		access to the router using Linux /proc filesystem.
 *
 * Author:	Ben Greear, <greearb@candelatech.com> coppied from wanproc.c
 *               by: Gene Kozin	<genek@compuserve.com>
 *
 * Copyright:	(c) 1998 Ben Greear
 *
<<<<<<< HEAD
 *		This program is free software; you can redistribute it and/or
 *		modify it under the terms of the GNU General Public License
 *		as published by the Free Software Foundation; either version
 *		2 of the License, or (at your option) any later version.
=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 * ============================================================================
 * Jan 20, 1998        Ben Greear     Initial Version
 *****************************************************************************/

#define pr_fmt(fmt) KBUILD_MODNAME ": " fmt

#include <linux/module.h>
#include <linux/errno.h>
#include <linux/kernel.h>
#include <linux/string.h>
#include <linux/proc_fs.h>
#include <linux/seq_file.h>
#include <linux/fs.h>
#include <linux/netdevice.h>
#include <linux/if_vlan.h>
#include <net/net_namespace.h>
#include <net/netns/generic.h>
#include "vlanproc.h"
#include "vlan.h"

/****** Function Prototypes *************************************************/

/* Methods for preparing data for reading proc entries */
static int vlan_seq_show(struct seq_file *seq, void *v);
static void *vlan_seq_start(struct seq_file *seq, loff_t *pos);
static void *vlan_seq_next(struct seq_file *seq, void *v, loff_t *pos);
static void vlan_seq_stop(struct seq_file *seq, void *);
static int vlandev_seq_show(struct seq_file *seq, void *v);

/*
 *	Global Data
 */


/*
 *	Names of the proc directory entries
 */

static const char name_root[]	 = "vlan";
static const char name_conf[]	 = "config";

/*
 *	Structures for interfacing with the /proc filesystem.
 *	VLAN creates its own directory /proc/net/vlan with the following
 *	entries:
 *	config		device status/configuration
 *	<device>	entry for each  device
 */

/*
 *	Generic /proc/net/vlan/<file> file and inode operations
 */

static const struct seq_operations vlan_seq_ops = {
	.start = vlan_seq_start,
	.next = vlan_seq_next,
	.stop = vlan_seq_stop,
	.show = vlan_seq_show,
};

<<<<<<< HEAD
static int vlan_seq_open(struct inode *inode, struct file *file)
{
	return seq_open_net(inode, file, &vlan_seq_ops,
			sizeof(struct seq_net_private));
}

static const struct file_operations vlan_fops = {
	.owner	 = THIS_MODULE,
	.open    = vlan_seq_open,
	.read    = seq_read,
	.llseek  = seq_lseek,
	.release = seq_release_net,
};

/*
 *	/proc/net/vlan/<device> file and inode operations
 */

static int vlandev_seq_open(struct inode *inode, struct file *file)
{
	return single_open(file, vlandev_seq_show, PDE(inode)->data);
}

static const struct file_operations vlandev_fops = {
	.owner = THIS_MODULE,
	.open    = vlandev_seq_open,
	.read    = seq_read,
	.llseek  = seq_lseek,
	.release = single_release,
};

/*
 * Proc filesystem derectory entries.
=======
/*
 * Proc filesystem directory entries.
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 */

/* Strings */
static const char *const vlan_name_type_str[VLAN_NAME_TYPE_HIGHEST] = {
    [VLAN_NAME_TYPE_RAW_PLUS_VID]        = "VLAN_NAME_TYPE_RAW_PLUS_VID",
    [VLAN_NAME_TYPE_PLUS_VID_NO_PAD]	 = "VLAN_NAME_TYPE_PLUS_VID_NO_PAD",
    [VLAN_NAME_TYPE_RAW_PLUS_VID_NO_PAD] = "VLAN_NAME_TYPE_RAW_PLUS_VID_NO_PAD",
    [VLAN_NAME_TYPE_PLUS_VID]		 = "VLAN_NAME_TYPE_PLUS_VID",
};
/*
 *	Interface functions
 */

/*
 *	Clean up /proc/net/vlan entries
 */

void vlan_proc_cleanup(struct net *net)
{
	struct vlan_net *vn = net_generic(net, vlan_net_id);

	if (vn->proc_vlan_conf)
		remove_proc_entry(name_conf, vn->proc_vlan_dir);

	if (vn->proc_vlan_dir)
<<<<<<< HEAD
		proc_net_remove(net, name_root);
=======
		remove_proc_entry(name_root, net->proc_net);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	/* Dynamically added entries should be cleaned up as their vlan_device
	 * is removed, so we should not have to take care of it here...
	 */
}

/*
 *	Create /proc/net/vlan entries
 */

int __net_init vlan_proc_init(struct net *net)
{
	struct vlan_net *vn = net_generic(net, vlan_net_id);

	vn->proc_vlan_dir = proc_net_mkdir(net, name_root, net->proc_net);
	if (!vn->proc_vlan_dir)
		goto err;

<<<<<<< HEAD
	vn->proc_vlan_conf = proc_create(name_conf, S_IFREG|S_IRUSR|S_IWUSR,
				     vn->proc_vlan_dir, &vlan_fops);
=======
	vn->proc_vlan_conf = proc_create_net(name_conf, S_IFREG | 0600,
			vn->proc_vlan_dir, &vlan_seq_ops,
			sizeof(struct seq_net_private));
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	if (!vn->proc_vlan_conf)
		goto err;
	return 0;

err:
	pr_err("can't create entry in proc filesystem!\n");
	vlan_proc_cleanup(net);
	return -ENOBUFS;
}

/*
 *	Add directory entry for VLAN device.
 */

int vlan_proc_add_dev(struct net_device *vlandev)
{
	struct vlan_dev_priv *vlan = vlan_dev_priv(vlandev);
	struct vlan_net *vn = net_generic(dev_net(vlandev), vlan_net_id);

<<<<<<< HEAD
	vlan->dent =
		proc_create_data(vlandev->name, S_IFREG|S_IRUSR|S_IWUSR,
				 vn->proc_vlan_dir, &vlandev_fops, vlandev);
=======
	if (!strcmp(vlandev->name, name_conf))
		return -EINVAL;
	vlan->dent = proc_create_single_data(vlandev->name, S_IFREG | 0600,
			vn->proc_vlan_dir, vlandev_seq_show, vlandev);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	if (!vlan->dent)
		return -ENOBUFS;
	return 0;
}

/*
 *	Delete directory entry for VLAN device.
 */
<<<<<<< HEAD
int vlan_proc_rem_dev(struct net_device *vlandev)
{
	struct vlan_net *vn = net_generic(dev_net(vlandev), vlan_net_id);

	/** NOTE:  This will consume the memory pointed to by dent, it seems. */
	if (vlan_dev_priv(vlandev)->dent) {
		remove_proc_entry(vlan_dev_priv(vlandev)->dent->name,
				  vn->proc_vlan_dir);
		vlan_dev_priv(vlandev)->dent = NULL;
	}
	return 0;
=======
void vlan_proc_rem_dev(struct net_device *vlandev)
{
	/** NOTE:  This will consume the memory pointed to by dent, it seems. */
	proc_remove(vlan_dev_priv(vlandev)->dent);
	vlan_dev_priv(vlandev)->dent = NULL;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

/****** Proc filesystem entry points ****************************************/

/*
 * The following few functions build the content of /proc/net/vlan/config
 */

<<<<<<< HEAD
/* start read of /proc/net/vlan/config */
static void *vlan_seq_start(struct seq_file *seq, loff_t *pos)
	__acquires(rcu)
{
	struct net_device *dev;
	struct net *net = seq_file_net(seq);
	loff_t i = 1;

=======
static void *vlan_seq_from_index(struct seq_file *seq, loff_t *pos)
{
	unsigned long ifindex = *pos;
	struct net_device *dev;

	for_each_netdev_dump(seq_file_net(seq), dev, ifindex) {
		if (!is_vlan_dev(dev))
			continue;
		*pos = dev->ifindex;
		return dev;
	}
	return NULL;
}

static void *vlan_seq_start(struct seq_file *seq, loff_t *pos)
	__acquires(rcu)
{
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	rcu_read_lock();
	if (*pos == 0)
		return SEQ_START_TOKEN;

<<<<<<< HEAD
	for_each_netdev_rcu(net, dev) {
		if (!is_vlan_dev(dev))
			continue;

		if (i++ == *pos)
			return dev;
	}

	return  NULL;
=======
	return vlan_seq_from_index(seq, pos);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

static void *vlan_seq_next(struct seq_file *seq, void *v, loff_t *pos)
{
<<<<<<< HEAD
	struct net_device *dev;
	struct net *net = seq_file_net(seq);

	++*pos;

	dev = v;
	if (v == SEQ_START_TOKEN)
		dev = net_device_entry(&net->dev_base_head);

	for_each_netdev_continue_rcu(net, dev) {
		if (!is_vlan_dev(dev))
			continue;

		return dev;
	}

	return NULL;
=======
	++*pos;
	return vlan_seq_from_index(seq, pos);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

static void vlan_seq_stop(struct seq_file *seq, void *v)
	__releases(rcu)
{
	rcu_read_unlock();
}

static int vlan_seq_show(struct seq_file *seq, void *v)
{
	struct net *net = seq_file_net(seq);
	struct vlan_net *vn = net_generic(net, vlan_net_id);

	if (v == SEQ_START_TOKEN) {
		const char *nmtype = NULL;

		seq_puts(seq, "VLAN Dev name	 | VLAN ID\n");

		if (vn->name_type < ARRAY_SIZE(vlan_name_type_str))
		    nmtype =  vlan_name_type_str[vn->name_type];

		seq_printf(seq, "Name-Type: %s\n",
			   nmtype ? nmtype :  "UNKNOWN");
	} else {
		const struct net_device *vlandev = v;
		const struct vlan_dev_priv *vlan = vlan_dev_priv(vlandev);

		seq_printf(seq, "%-15s| %d  | %s\n",  vlandev->name,
			   vlan->vlan_id,    vlan->real_dev->name);
	}
	return 0;
}

static int vlandev_seq_show(struct seq_file *seq, void *offset)
{
	struct net_device *vlandev = (struct net_device *) seq->private;
	const struct vlan_dev_priv *vlan = vlan_dev_priv(vlandev);
	struct rtnl_link_stats64 temp;
	const struct rtnl_link_stats64 *stats;
	static const char fmt64[] = "%30s %12llu\n";
	int i;

	if (!is_vlan_dev(vlandev))
		return 0;

	stats = dev_get_stats(vlandev, &temp);
	seq_printf(seq,
<<<<<<< HEAD
		   "%s  VID: %d	 REORDER_HDR: %i  dev->priv_flags: %hx\n",
=======
		   "%s  VID: %d	 REORDER_HDR: %i  dev->priv_flags: %llx\n",
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		   vlandev->name, vlan->vlan_id,
		   (int)(vlan->flags & 1), vlandev->priv_flags);

	seq_printf(seq, fmt64, "total frames received", stats->rx_packets);
	seq_printf(seq, fmt64, "total bytes received", stats->rx_bytes);
	seq_printf(seq, fmt64, "Broadcast/Multicast Rcvd", stats->multicast);
	seq_puts(seq, "\n");
	seq_printf(seq, fmt64, "total frames transmitted", stats->tx_packets);
	seq_printf(seq, fmt64, "total bytes transmitted", stats->tx_bytes);
	seq_printf(seq, "Device: %s", vlan->real_dev->name);
	/* now show all PRIORITY mappings relating to this VLAN */
	seq_printf(seq, "\nINGRESS priority mappings: "
			"0:%u  1:%u  2:%u  3:%u  4:%u  5:%u  6:%u 7:%u\n",
		   vlan->ingress_priority_map[0],
		   vlan->ingress_priority_map[1],
		   vlan->ingress_priority_map[2],
		   vlan->ingress_priority_map[3],
		   vlan->ingress_priority_map[4],
		   vlan->ingress_priority_map[5],
		   vlan->ingress_priority_map[6],
		   vlan->ingress_priority_map[7]);

	seq_printf(seq, " EGRESS priority mappings: ");
	for (i = 0; i < 16; i++) {
		const struct vlan_priority_tci_mapping *mp
			= vlan->egress_priority_map[i];
		while (mp) {
<<<<<<< HEAD
			seq_printf(seq, "%u:%hu ",
=======
			seq_printf(seq, "%u:%d ",
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
				   mp->priority, ((mp->vlan_qos >> 13) & 0x7));
			mp = mp->next;
		}
	}
	seq_puts(seq, "\n");

	return 0;
}
