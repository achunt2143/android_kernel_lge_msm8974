<<<<<<< HEAD
/*
 * 	connector.c
 *
 * 2004+ Copyright (c) Evgeniy Polyakov <zbr@ioremap.net>
 * All rights reserved.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */

=======
// SPDX-License-Identifier: GPL-2.0-or-later
/*
 *	connector.c
 *
 * 2004+ Copyright (c) Evgeniy Polyakov <zbr@ioremap.net>
 * All rights reserved.
 */

#include <linux/compiler.h>
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/list.h>
#include <linux/skbuff.h>
<<<<<<< HEAD
#include <linux/netlink.h>
=======
#include <net/netlink.h>
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#include <linux/moduleparam.h>
#include <linux/connector.h>
#include <linux/slab.h>
#include <linux/mutex.h>
#include <linux/proc_fs.h>
#include <linux/spinlock.h>

#include <net/sock.h>

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Evgeniy Polyakov <zbr@ioremap.net>");
MODULE_DESCRIPTION("Generic userspace <-> kernelspace connector.");
MODULE_ALIAS_NET_PF_PROTO(PF_NETLINK, NETLINK_CONNECTOR);

static struct cn_dev cdev;

static int cn_already_initialized;

/*
<<<<<<< HEAD
=======
 * Sends mult (multiple) cn_msg at a time.
 *
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 * msg->seq and msg->ack are used to determine message genealogy.
 * When someone sends message it puts there locally unique sequence
 * and random acknowledge numbers.  Sequence number may be copied into
 * nlmsghdr->nlmsg_seq too.
 *
 * Sequence number is incremented with each message to be sent.
 *
<<<<<<< HEAD
 * If we expect reply to our message then the sequence number in
=======
 * If we expect a reply to our message then the sequence number in
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 * received message MUST be the same as in original message, and
 * acknowledge number MUST be the same + 1.
 *
 * If we receive a message and its sequence number is not equal to the
 * one we are expecting then it is a new message.
 *
 * If we receive a message and its sequence number is the same as one
 * we are expecting but it's acknowledgement number is not equal to
 * the acknowledgement number in the original message + 1, then it is
 * a new message.
 *
<<<<<<< HEAD
 */
int cn_netlink_send(struct cn_msg *msg, u32 __group, gfp_t gfp_mask)
=======
 * If msg->len != len, then additional cn_msg messages are expected following
 * the first msg.
 *
 * The message is sent to, the portid if given, the group if given, both if
 * both, or if both are zero then the group is looked up and sent there.
 */
int cn_netlink_send_mult(struct cn_msg *msg, u16 len, u32 portid, u32 __group,
			 gfp_t gfp_mask, netlink_filter_fn filter,
			 void *filter_data)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
{
	struct cn_callback_entry *__cbq;
	unsigned int size;
	struct sk_buff *skb;
	struct nlmsghdr *nlh;
	struct cn_msg *data;
	struct cn_dev *dev = &cdev;
	u32 group = 0;
	int found = 0;

<<<<<<< HEAD
	if (!__group) {
=======
	if (portid || __group) {
		group = __group;
	} else {
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		spin_lock_bh(&dev->cbdev->queue_lock);
		list_for_each_entry(__cbq, &dev->cbdev->queue_list,
				    callback_entry) {
			if (cn_cb_equal(&__cbq->id.id, &msg->id)) {
				found = 1;
				group = __cbq->group;
				break;
			}
		}
		spin_unlock_bh(&dev->cbdev->queue_lock);

		if (!found)
			return -ENODEV;
<<<<<<< HEAD
	} else {
		group = __group;
	}

	if (!netlink_has_listeners(dev->nls, group))
		return -ESRCH;

	size = NLMSG_SPACE(sizeof(*msg) + msg->len);

	skb = alloc_skb(size, gfp_mask);
	if (!skb)
		return -ENOMEM;

	nlh = NLMSG_PUT(skb, 0, msg->seq, NLMSG_DONE, size - sizeof(*nlh));

	data = NLMSG_DATA(nlh);

	memcpy(data, msg, sizeof(*data) + msg->len);

	NETLINK_CB(skb).dst_group = group;

	return netlink_broadcast(dev->nls, skb, 0, group, gfp_mask);

nlmsg_failure:
	kfree_skb(skb);
	return -EINVAL;
=======
	}

	if (!portid && !netlink_has_listeners(dev->nls, group))
		return -ESRCH;

	size = sizeof(*msg) + len;

	skb = nlmsg_new(size, gfp_mask);
	if (!skb)
		return -ENOMEM;

	nlh = nlmsg_put(skb, 0, msg->seq, NLMSG_DONE, size, 0);
	if (!nlh) {
		kfree_skb(skb);
		return -EMSGSIZE;
	}

	data = nlmsg_data(nlh);

	memcpy(data, msg, size);

	NETLINK_CB(skb).dst_group = group;

	if (group)
		return netlink_broadcast_filtered(dev->nls, skb, portid, group,
						  gfp_mask, filter,
						  (void *)filter_data);
	return netlink_unicast(dev->nls, skb, portid,
			!gfpflags_allow_blocking(gfp_mask));
}
EXPORT_SYMBOL_GPL(cn_netlink_send_mult);

/* same as cn_netlink_send_mult except msg->len is used for len */
int cn_netlink_send(struct cn_msg *msg, u32 portid, u32 __group,
	gfp_t gfp_mask)
{
	return cn_netlink_send_mult(msg, msg->len, portid, __group, gfp_mask,
				    NULL, NULL);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}
EXPORT_SYMBOL_GPL(cn_netlink_send);

/*
 * Callback helper - queues work and setup destructor for given data.
 */
static int cn_call_callback(struct sk_buff *skb)
{
<<<<<<< HEAD
	struct cn_callback_entry *i, *cbq = NULL;
	struct cn_dev *dev = &cdev;
	struct cn_msg *msg = NLMSG_DATA(nlmsg_hdr(skb));
	struct netlink_skb_parms *nsp = &NETLINK_CB(skb);
	int err = -ENODEV;

	spin_lock_bh(&dev->cbdev->queue_lock);
	list_for_each_entry(i, &dev->cbdev->queue_list, callback_entry) {
		if (cn_cb_equal(&i->id.id, &msg->id)) {
			atomic_inc(&i->refcnt);
=======
	struct nlmsghdr *nlh;
	struct cn_callback_entry *i, *cbq = NULL;
	struct cn_dev *dev = &cdev;
	struct cn_msg *msg = nlmsg_data(nlmsg_hdr(skb));
	struct netlink_skb_parms *nsp = &NETLINK_CB(skb);
	int err = -ENODEV;

	/* verify msg->len is within skb */
	nlh = nlmsg_hdr(skb);
	if (nlh->nlmsg_len < NLMSG_HDRLEN + sizeof(struct cn_msg) + msg->len)
		return -EINVAL;

	spin_lock_bh(&dev->cbdev->queue_lock);
	list_for_each_entry(i, &dev->cbdev->queue_list, callback_entry) {
		if (cn_cb_equal(&i->id.id, &msg->id)) {
			refcount_inc(&i->refcnt);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
			cbq = i;
			break;
		}
	}
	spin_unlock_bh(&dev->cbdev->queue_lock);

	if (cbq != NULL) {
<<<<<<< HEAD
		err = 0;
=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		cbq->callback(msg, nsp);
		kfree_skb(skb);
		cn_queue_release_callback(cbq);
		err = 0;
	}

	return err;
}

/*
<<<<<<< HEAD
=======
 * Allow non-root access for NETLINK_CONNECTOR family having CN_IDX_PROC
 * multicast group.
 */
static int cn_bind(struct net *net, int group)
{
	unsigned long groups = (unsigned long) group;

	if (ns_capable(net->user_ns, CAP_NET_ADMIN))
		return 0;

	if (test_bit(CN_IDX_PROC - 1, &groups))
		return 0;

	return -EPERM;
}

static void cn_release(struct sock *sk, unsigned long *groups)
{
	if (groups && test_bit(CN_IDX_PROC - 1, groups)) {
		kfree(sk->sk_user_data);
		sk->sk_user_data = NULL;
	}
}

/*
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 * Main netlink receiving function.
 *
 * It checks skb, netlink header and msg sizes, and calls callback helper.
 */
<<<<<<< HEAD
static void cn_rx_skb(struct sk_buff *__skb)
{
	struct nlmsghdr *nlh;
	struct sk_buff *skb;
	int len, err;

	skb = skb_get(__skb);

	if (skb->len >= NLMSG_SPACE(0)) {
=======
static void cn_rx_skb(struct sk_buff *skb)
{
	struct nlmsghdr *nlh;
	int len, err;

	if (skb->len >= NLMSG_HDRLEN) {
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		nlh = nlmsg_hdr(skb);
		len = nlmsg_len(nlh);

		if (len < (int)sizeof(struct cn_msg) ||
		    skb->len < nlh->nlmsg_len ||
<<<<<<< HEAD
		    len > CONNECTOR_MAX_MSG_SIZE) {
			kfree_skb(skb);
			return;
		}

		err = cn_call_callback(skb);
=======
		    len > CONNECTOR_MAX_MSG_SIZE)
			return;

		err = cn_call_callback(skb_get(skb));
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		if (err < 0)
			kfree_skb(skb);
	}
}

/*
 * Callback add routing - adds callback with given ID and name.
 * If there is registered callback with the same ID it will not be added.
 *
 * May sleep.
 */
<<<<<<< HEAD
int cn_add_callback(struct cb_id *id, const char *name,
		    void (*callback)(struct cn_msg *, struct netlink_skb_parms *))
{
	int err;
=======
int cn_add_callback(const struct cb_id *id, const char *name,
		    void (*callback)(struct cn_msg *,
				     struct netlink_skb_parms *))
{
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	struct cn_dev *dev = &cdev;

	if (!cn_already_initialized)
		return -EAGAIN;

<<<<<<< HEAD
	err = cn_queue_add_callback(dev->cbdev, name, id, callback);
	if (err)
		return err;

	return 0;
=======
	return cn_queue_add_callback(dev->cbdev, name, id, callback);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}
EXPORT_SYMBOL_GPL(cn_add_callback);

/*
 * Callback remove routing - removes callback
 * with given ID.
 * If there is no registered callback with given
 * ID nothing happens.
 *
 * May sleep while waiting for reference counter to become zero.
 */
<<<<<<< HEAD
void cn_del_callback(struct cb_id *id)
=======
void cn_del_callback(const struct cb_id *id)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
{
	struct cn_dev *dev = &cdev;

	cn_queue_del_callback(dev->cbdev, id);
}
EXPORT_SYMBOL_GPL(cn_del_callback);

<<<<<<< HEAD
static int cn_proc_show(struct seq_file *m, void *v)
=======
static int __maybe_unused cn_proc_show(struct seq_file *m, void *v)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
{
	struct cn_queue_dev *dev = cdev.cbdev;
	struct cn_callback_entry *cbq;

	seq_printf(m, "Name            ID\n");

	spin_lock_bh(&dev->queue_lock);

	list_for_each_entry(cbq, &dev->queue_list, callback_entry) {
		seq_printf(m, "%-15s %u:%u\n",
			   cbq->id.name,
			   cbq->id.id.idx,
			   cbq->id.id.val);
	}

	spin_unlock_bh(&dev->queue_lock);

	return 0;
}

<<<<<<< HEAD
static int cn_proc_open(struct inode *inode, struct file *file)
{
	return single_open(file, cn_proc_show, NULL);
}

static const struct file_operations cn_file_ops = {
	.owner   = THIS_MODULE,
	.open    = cn_proc_open,
	.read    = seq_read,
	.llseek  = seq_lseek,
	.release = single_release
};

static int __devinit cn_init(void)
{
	struct cn_dev *dev = &cdev;

	dev->input = cn_rx_skb;

	dev->nls = netlink_kernel_create(&init_net, NETLINK_CONNECTOR,
					 CN_NETLINK_USERS + 0xf,
					 dev->input, NULL, THIS_MODULE);
=======
static int cn_init(void)
{
	struct cn_dev *dev = &cdev;
	struct netlink_kernel_cfg cfg = {
		.groups	= CN_NETLINK_USERS + 0xf,
		.input	= cn_rx_skb,
		.flags  = NL_CFG_F_NONROOT_RECV,
		.bind   = cn_bind,
		.release = cn_release,
	};

	dev->nls = netlink_kernel_create(&init_net, NETLINK_CONNECTOR, &cfg);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	if (!dev->nls)
		return -EIO;

	dev->cbdev = cn_queue_alloc_dev("cqueue", dev->nls);
	if (!dev->cbdev) {
		netlink_kernel_release(dev->nls);
		return -EINVAL;
	}

	cn_already_initialized = 1;

<<<<<<< HEAD
	proc_net_fops_create(&init_net, "connector", S_IRUGO, &cn_file_ops);
=======
	proc_create_single("connector", S_IRUGO, init_net.proc_net, cn_proc_show);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	return 0;
}

<<<<<<< HEAD
static void __devexit cn_fini(void)
=======
static void cn_fini(void)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
{
	struct cn_dev *dev = &cdev;

	cn_already_initialized = 0;

<<<<<<< HEAD
	proc_net_remove(&init_net, "connector");
=======
	remove_proc_entry("connector", init_net.proc_net);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	cn_queue_free_dev(dev->cbdev);
	netlink_kernel_release(dev->nls);
}

subsys_initcall(cn_init);
module_exit(cn_fini);
