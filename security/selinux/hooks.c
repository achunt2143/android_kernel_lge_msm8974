<<<<<<< HEAD
/*
 *  NSA Security-Enhanced Linux (SELinux) security module
 *
 *  This file contains the SELinux hook function implementations.
 *
 *  Authors:  Stephen Smalley, <sds@epoch.ncsc.mil>
=======
// SPDX-License-Identifier: GPL-2.0-only
/*
 *  Security-Enhanced Linux (SELinux) security module
 *
 *  This file contains the SELinux hook function implementations.
 *
 *  Authors:  Stephen Smalley, <stephen.smalley.work@gmail.com>
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 *	      Chris Vance, <cvance@nai.com>
 *	      Wayne Salamon, <wsalamon@nai.com>
 *	      James Morris <jmorris@redhat.com>
 *
 *  Copyright (C) 2001,2002 Networks Associates Technology, Inc.
 *  Copyright (C) 2003-2008 Red Hat, Inc., James Morris <jmorris@redhat.com>
 *					   Eric Paris <eparis@redhat.com>
 *  Copyright (C) 2004-2005 Trusted Computer Solutions, Inc.
 *			    <dgoeddel@trustedcs.com>
 *  Copyright (C) 2006, 2007, 2009 Hewlett-Packard Development Company, L.P.
 *	Paul Moore <paul@paul-moore.com>
 *  Copyright (C) 2007 Hitachi Software Engineering Co., Ltd.
 *		       Yuichi Nakamura <ynakam@hitachisoft.jp>
<<<<<<< HEAD
 *
 *	This program is free software; you can redistribute it and/or modify
 *	it under the terms of the GNU General Public License version 2,
 *	as published by the Free Software Foundation.
=======
 *  Copyright (C) 2016 Mellanox Technologies
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 */

#include <linux/init.h>
#include <linux/kd.h>
#include <linux/kernel.h>
<<<<<<< HEAD
#include <linux/tracehook.h>
#include <linux/errno.h>
#include <linux/sched.h>
#include <linux/security.h>
=======
#include <linux/kernel_read_file.h>
#include <linux/errno.h>
#include <linux/sched/signal.h>
#include <linux/sched/task.h>
#include <linux/lsm_hooks.h>
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#include <linux/xattr.h>
#include <linux/capability.h>
#include <linux/unistd.h>
#include <linux/mm.h>
#include <linux/mman.h>
#include <linux/slab.h>
#include <linux/pagemap.h>
#include <linux/proc_fs.h>
#include <linux/swap.h>
#include <linux/spinlock.h>
#include <linux/syscalls.h>
#include <linux/dcache.h>
#include <linux/file.h>
#include <linux/fdtable.h>
#include <linux/namei.h>
#include <linux/mount.h>
<<<<<<< HEAD
=======
#include <linux/fs_context.h>
#include <linux/fs_parser.h>
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#include <linux/netfilter_ipv4.h>
#include <linux/netfilter_ipv6.h>
#include <linux/tty.h>
#include <net/icmp.h>
#include <net/ip.h>		/* for local_port_range[] */
#include <net/tcp.h>		/* struct or_callable used in sock_rcv_skb */
#include <net/inet_connection_sock.h>
#include <net/net_namespace.h>
#include <net/netlabel.h>
#include <linux/uaccess.h>
#include <asm/ioctls.h>
#include <linux/atomic.h>
#include <linux/bitops.h>
#include <linux/interrupt.h>
#include <linux/netdevice.h>	/* for network interface checks */
<<<<<<< HEAD
#include <linux/netlink.h>
#include <linux/tcp.h>
#include <linux/udp.h>
#include <linux/dccp.h>
=======
#include <net/netlink.h>
#include <linux/tcp.h>
#include <linux/udp.h>
#include <linux/dccp.h>
#include <linux/sctp.h>
#include <net/sctp/structs.h>
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#include <linux/quota.h>
#include <linux/un.h>		/* for Unix socket types */
#include <net/af_unix.h>	/* for Unix socket types */
#include <linux/parser.h>
#include <linux/nfs_mount.h>
#include <net/ipv6.h>
#include <linux/hugetlb.h>
#include <linux/personality.h>
#include <linux/audit.h>
#include <linux/string.h>
<<<<<<< HEAD
#include <linux/selinux.h>
=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#include <linux/mutex.h>
#include <linux/posix-timers.h>
#include <linux/syslog.h>
#include <linux/user_namespace.h>
#include <linux/export.h>
#include <linux/msg.h>
#include <linux/shm.h>
<<<<<<< HEAD
#include <linux/pft.h>
=======
#include <uapi/linux/shm.h>
#include <linux/bpf.h>
#include <linux/kernfs.h>
#include <linux/stringhash.h>	/* for hashlen_string() */
#include <uapi/linux/mount.h>
#include <linux/fsnotify.h>
#include <linux/fanotify.h>
#include <linux/io_uring/cmd.h>
#include <uapi/linux/lsm.h>
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

#include "avc.h"
#include "objsec.h"
#include "netif.h"
#include "netnode.h"
#include "netport.h"
<<<<<<< HEAD
=======
#include "ibpkey.h"
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#include "xfrm.h"
#include "netlabel.h"
#include "audit.h"
#include "avc_ss.h"

<<<<<<< HEAD
#define NUM_SEL_MNT_OPTS 5

extern struct security_operations *security_ops;
=======
#define SELINUX_INODE_INIT_XATTRS 1

struct selinux_state selinux_state;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

/* SECMARK reference count */
static atomic_t selinux_secmark_refcount = ATOMIC_INIT(0);

#ifdef CONFIG_SECURITY_SELINUX_DEVELOP
<<<<<<< HEAD
int selinux_enforcing;
=======
static int selinux_enforcing_boot __initdata;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

static int __init enforcing_setup(char *str)
{
	unsigned long enforcing;
<<<<<<< HEAD
	if (!strict_strtoul(str, 0, &enforcing))
		selinux_enforcing = enforcing ? 1 : 0;
	return 1;
}
__setup("enforcing=", enforcing_setup);
#endif

#ifdef CONFIG_SECURITY_SELINUX_BOOTPARAM
int selinux_enabled = CONFIG_SECURITY_SELINUX_BOOTPARAM_VALUE;

static int __init selinux_enabled_setup(char *str)
{
	unsigned long enabled;
	if (!strict_strtoul(str, 0, &enabled))
		selinux_enabled = enabled ? 1 : 0;
	return 1;
}
__setup("selinux=", selinux_enabled_setup);
#else
int selinux_enabled = 1;
#endif

static struct kmem_cache *sel_inode_cache;
=======
	if (!kstrtoul(str, 0, &enforcing))
		selinux_enforcing_boot = enforcing ? 1 : 0;
	return 1;
}
__setup("enforcing=", enforcing_setup);
#else
#define selinux_enforcing_boot 1
#endif

int selinux_enabled_boot __initdata = 1;
#ifdef CONFIG_SECURITY_SELINUX_BOOTPARAM
static int __init selinux_enabled_setup(char *str)
{
	unsigned long enabled;
	if (!kstrtoul(str, 0, &enabled))
		selinux_enabled_boot = enabled ? 1 : 0;
	return 1;
}
__setup("selinux=", selinux_enabled_setup);
#endif

static int __init checkreqprot_setup(char *str)
{
	unsigned long checkreqprot;

	if (!kstrtoul(str, 0, &checkreqprot)) {
		if (checkreqprot)
			pr_err("SELinux: checkreqprot set to 1 via kernel parameter.  This is no longer supported.\n");
	}
	return 1;
}
__setup("checkreqprot=", checkreqprot_setup);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

/**
 * selinux_secmark_enabled - Check to see if SECMARK is currently enabled
 *
 * Description:
 * This function checks the SECMARK reference counter to see if any SECMARK
 * targets are currently configured, if the reference counter is greater than
 * zero SECMARK is considered to be enabled.  Returns true (1) if SECMARK is
<<<<<<< HEAD
 * enabled, false (0) if SECMARK is disabled.
=======
 * enabled, false (0) if SECMARK is disabled.  If the always_check_network
 * policy capability is enabled, SECMARK is always considered enabled.
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 *
 */
static int selinux_secmark_enabled(void)
{
<<<<<<< HEAD
	return (atomic_read(&selinux_secmark_refcount) > 0);
=======
	return (selinux_policycap_alwaysnetwork() ||
		atomic_read(&selinux_secmark_refcount));
}

/**
 * selinux_peerlbl_enabled - Check to see if peer labeling is currently enabled
 *
 * Description:
 * This function checks if NetLabel or labeled IPSEC is enabled.  Returns true
 * (1) if any are enabled or false (0) if neither are enabled.  If the
 * always_check_network policy capability is enabled, peer labeling
 * is always considered enabled.
 *
 */
static int selinux_peerlbl_enabled(void)
{
	return (selinux_policycap_alwaysnetwork() ||
		netlbl_enabled() || selinux_xfrm_enabled());
}

static int selinux_netcache_avc_callback(u32 event)
{
	if (event == AVC_CALLBACK_RESET) {
		sel_netif_flush();
		sel_netnode_flush();
		sel_netport_flush();
		synchronize_net();
	}
	return 0;
}

static int selinux_lsm_notifier_avc_callback(u32 event)
{
	if (event == AVC_CALLBACK_RESET) {
		sel_ib_pkey_flush();
		call_blocking_lsm_notifier(LSM_POLICY_CHANGE, NULL);
	}

	return 0;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

/*
 * initialise the security for the init task
 */
static void cred_init_security(void)
{
<<<<<<< HEAD
	struct cred *cred = (struct cred *) current->real_cred;
	struct task_security_struct *tsec;

	tsec = kzalloc(sizeof(struct task_security_struct), GFP_KERNEL);
	if (!tsec)
		panic("SELinux:  Failed to initialize initial task.\n");

	tsec->osid = tsec->sid = SECINITSID_KERNEL;
	cred->security = tsec;
=======
	struct task_security_struct *tsec;

	tsec = selinux_cred(unrcu_pointer(current->real_cred));
	tsec->osid = tsec->sid = SECINITSID_KERNEL;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

/*
 * get the security ID of a set of credentials
 */
static inline u32 cred_sid(const struct cred *cred)
{
	const struct task_security_struct *tsec;

<<<<<<< HEAD
	tsec = cred->security;
	return tsec->sid;
}

/*
 * get the objective security ID of a task
 */
static inline u32 task_sid(const struct task_struct *task)
=======
	tsec = selinux_cred(cred);
	return tsec->sid;
}

static void __ad_net_init(struct common_audit_data *ad,
			  struct lsm_network_audit *net,
			  int ifindex, struct sock *sk, u16 family)
{
	ad->type = LSM_AUDIT_DATA_NET;
	ad->u.net = net;
	net->netif = ifindex;
	net->sk = sk;
	net->family = family;
}

static void ad_net_init_from_sk(struct common_audit_data *ad,
				struct lsm_network_audit *net,
				struct sock *sk)
{
	__ad_net_init(ad, net, 0, sk, 0);
}

static void ad_net_init_from_iif(struct common_audit_data *ad,
				 struct lsm_network_audit *net,
				 int ifindex, u16 family)
{
	__ad_net_init(ad, net, ifindex, NULL, family);
}

/*
 * get the objective security ID of a task
 */
static inline u32 task_sid_obj(const struct task_struct *task)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
{
	u32 sid;

	rcu_read_lock();
	sid = cred_sid(__task_cred(task));
	rcu_read_unlock();
	return sid;
}

<<<<<<< HEAD
/*
 * get the subjective security ID of the current task
 */
static inline u32 current_sid(void)
{
	const struct task_security_struct *tsec = current_security();

	return tsec->sid;
}

/* Allocate and free functions for each kind of security blob. */

static int inode_alloc_security(struct inode *inode)
{
	struct inode_security_struct *isec;
	u32 sid = current_sid();

	isec = kmem_cache_zalloc(sel_inode_cache, GFP_NOFS);
	if (!isec)
		return -ENOMEM;

	mutex_init(&isec->lock);
	INIT_LIST_HEAD(&isec->list);
	isec->inode = inode;
	isec->sid = SECINITSID_UNLABELED;
	isec->sclass = SECCLASS_FILE;
	isec->task_sid = sid;
	inode->i_security = isec;

	return 0;
}

static void inode_free_rcu(struct rcu_head *head)
{
	struct inode_security_struct *isec;

	isec = container_of(head, struct inode_security_struct, rcu);
	kmem_cache_free(sel_inode_cache, isec);
=======
static int inode_doinit_with_dentry(struct inode *inode, struct dentry *opt_dentry);

/*
 * Try reloading inode security labels that have been marked as invalid.  The
 * @may_sleep parameter indicates when sleeping and thus reloading labels is
 * allowed; when set to false, returns -ECHILD when the label is
 * invalid.  The @dentry parameter should be set to a dentry of the inode.
 */
static int __inode_security_revalidate(struct inode *inode,
				       struct dentry *dentry,
				       bool may_sleep)
{
	struct inode_security_struct *isec = selinux_inode(inode);

	might_sleep_if(may_sleep);

	if (selinux_initialized() &&
	    isec->initialized != LABEL_INITIALIZED) {
		if (!may_sleep)
			return -ECHILD;

		/*
		 * Try reloading the inode security label.  This will fail if
		 * @opt_dentry is NULL and no dentry for this inode can be
		 * found; in that case, continue using the old label.
		 */
		inode_doinit_with_dentry(inode, dentry);
	}
	return 0;
}

static struct inode_security_struct *inode_security_novalidate(struct inode *inode)
{
	return selinux_inode(inode);
}

static struct inode_security_struct *inode_security_rcu(struct inode *inode, bool rcu)
{
	int error;

	error = __inode_security_revalidate(inode, NULL, !rcu);
	if (error)
		return ERR_PTR(error);
	return selinux_inode(inode);
}

/*
 * Get the security label of an inode.
 */
static struct inode_security_struct *inode_security(struct inode *inode)
{
	__inode_security_revalidate(inode, NULL, true);
	return selinux_inode(inode);
}

static struct inode_security_struct *backing_inode_security_novalidate(struct dentry *dentry)
{
	struct inode *inode = d_backing_inode(dentry);

	return selinux_inode(inode);
}

/*
 * Get the security label of a dentry's backing inode.
 */
static struct inode_security_struct *backing_inode_security(struct dentry *dentry)
{
	struct inode *inode = d_backing_inode(dentry);

	__inode_security_revalidate(inode, dentry, true);
	return selinux_inode(inode);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

static void inode_free_security(struct inode *inode)
{
<<<<<<< HEAD
	struct inode_security_struct *isec = inode->i_security;
	struct superblock_security_struct *sbsec = inode->i_sb->s_security;

	spin_lock(&sbsec->isec_lock);
	if (!list_empty(&isec->list))
		list_del_init(&isec->list);
	spin_unlock(&sbsec->isec_lock);

	/*
	 * The inode may still be referenced in a path walk and
	 * a call to selinux_inode_permission() can be made
	 * after inode_free_security() is called. Ideally, the VFS
	 * wouldn't do this, but fixing that is a much harder
	 * job. For now, simply free the i_security via RCU, and
	 * leave the current inode->i_security pointer intact.
	 * The inode will be freed after the RCU grace period too.
	 */
	call_rcu(&isec->rcu, inode_free_rcu);
}

static int file_alloc_security(struct file *file)
{
	struct file_security_struct *fsec;
	u32 sid = current_sid();

	fsec = kzalloc(sizeof(struct file_security_struct), GFP_KERNEL);
	if (!fsec)
		return -ENOMEM;

	fsec->sid = sid;
	fsec->fown_sid = sid;
	file->f_security = fsec;

	return 0;
}

static void file_free_security(struct file *file)
{
	struct file_security_struct *fsec = file->f_security;
	file->f_security = NULL;
	kfree(fsec);
}

static int superblock_alloc_security(struct super_block *sb)
{
	struct superblock_security_struct *sbsec;

	sbsec = kzalloc(sizeof(struct superblock_security_struct), GFP_KERNEL);
	if (!sbsec)
		return -ENOMEM;

	mutex_init(&sbsec->lock);
	INIT_LIST_HEAD(&sbsec->isec_head);
	spin_lock_init(&sbsec->isec_lock);
	sbsec->sb = sb;
	sbsec->sid = SECINITSID_UNLABELED;
	sbsec->def_sid = SECINITSID_FILE;
	sbsec->mntpoint_sid = SECINITSID_UNLABELED;
	sb->s_security = sbsec;

	return 0;
}

static void superblock_free_security(struct super_block *sb)
{
	struct superblock_security_struct *sbsec = sb->s_security;
	sb->s_security = NULL;
	kfree(sbsec);
}

/* The file system's label must be initialized prior to use. */

static const char *labeling_behaviors[6] = {
	"uses xattr",
	"uses transition SIDs",
	"uses task SIDs",
	"uses genfs_contexts",
	"not configured for labeling",
	"uses mountpoint labeling",
};

static int inode_doinit_with_dentry(struct inode *inode, struct dentry *opt_dentry);

static inline int inode_doinit(struct inode *inode)
{
	return inode_doinit_with_dentry(inode, NULL);
=======
	struct inode_security_struct *isec = selinux_inode(inode);
	struct superblock_security_struct *sbsec;

	if (!isec)
		return;
	sbsec = selinux_superblock(inode->i_sb);
	/*
	 * As not all inode security structures are in a list, we check for
	 * empty list outside of the lock to make sure that we won't waste
	 * time taking a lock doing nothing.
	 *
	 * The list_del_init() function can be safely called more than once.
	 * It should not be possible for this function to be called with
	 * concurrent list_add(), but for better safety against future changes
	 * in the code, we use list_empty_careful() here.
	 */
	if (!list_empty_careful(&isec->list)) {
		spin_lock(&sbsec->isec_lock);
		list_del_init(&isec->list);
		spin_unlock(&sbsec->isec_lock);
	}
}

struct selinux_mnt_opts {
	u32 fscontext_sid;
	u32 context_sid;
	u32 rootcontext_sid;
	u32 defcontext_sid;
};

static void selinux_free_mnt_opts(void *mnt_opts)
{
	kfree(mnt_opts);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

enum {
	Opt_error = -1,
<<<<<<< HEAD
	Opt_context = 1,
	Opt_fscontext = 2,
	Opt_defcontext = 3,
	Opt_rootcontext = 4,
	Opt_labelsupport = 5,
};

static const match_table_t tokens = {
	{Opt_context, CONTEXT_STR "%s"},
	{Opt_fscontext, FSCONTEXT_STR "%s"},
	{Opt_defcontext, DEFCONTEXT_STR "%s"},
	{Opt_rootcontext, ROOTCONTEXT_STR "%s"},
	{Opt_labelsupport, LABELSUPP_STR},
	{Opt_error, NULL},
};
=======
	Opt_context = 0,
	Opt_defcontext = 1,
	Opt_fscontext = 2,
	Opt_rootcontext = 3,
	Opt_seclabel = 4,
};

#define A(s, has_arg) {#s, sizeof(#s) - 1, Opt_##s, has_arg}
static const struct {
	const char *name;
	int len;
	int opt;
	bool has_arg;
} tokens[] = {
	A(context, true),
	A(fscontext, true),
	A(defcontext, true),
	A(rootcontext, true),
	A(seclabel, false),
};
#undef A

static int match_opt_prefix(char *s, int l, char **arg)
{
	int i;

	for (i = 0; i < ARRAY_SIZE(tokens); i++) {
		size_t len = tokens[i].len;
		if (len > l || memcmp(s, tokens[i].name, len))
			continue;
		if (tokens[i].has_arg) {
			if (len == l || s[len] != '=')
				continue;
			*arg = s + len + 1;
		} else if (len != l)
			continue;
		return tokens[i].opt;
	}
	return Opt_error;
}
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

#define SEL_MOUNT_FAIL_MSG "SELinux:  duplicate or incompatible mount options\n"

static int may_context_mount_sb_relabel(u32 sid,
			struct superblock_security_struct *sbsec,
			const struct cred *cred)
{
<<<<<<< HEAD
	const struct task_security_struct *tsec = cred->security;
=======
	const struct task_security_struct *tsec = selinux_cred(cred);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	int rc;

	rc = avc_has_perm(tsec->sid, sbsec->sid, SECCLASS_FILESYSTEM,
			  FILESYSTEM__RELABELFROM, NULL);
	if (rc)
		return rc;

	rc = avc_has_perm(tsec->sid, sid, SECCLASS_FILESYSTEM,
			  FILESYSTEM__RELABELTO, NULL);
	return rc;
}

static int may_context_mount_inode_relabel(u32 sid,
			struct superblock_security_struct *sbsec,
			const struct cred *cred)
{
<<<<<<< HEAD
	const struct task_security_struct *tsec = cred->security;
=======
	const struct task_security_struct *tsec = selinux_cred(cred);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	int rc;
	rc = avc_has_perm(tsec->sid, sbsec->sid, SECCLASS_FILESYSTEM,
			  FILESYSTEM__RELABELFROM, NULL);
	if (rc)
		return rc;

	rc = avc_has_perm(sid, sbsec->sid, SECCLASS_FILESYSTEM,
			  FILESYSTEM__ASSOCIATE, NULL);
	return rc;
}

<<<<<<< HEAD
static int sb_finish_set_opts(struct super_block *sb)
{
	struct superblock_security_struct *sbsec = sb->s_security;
	struct dentry *root = sb->s_root;
	struct inode *root_inode = root->d_inode;
	int rc = 0;

	if (sbsec->behavior == SECURITY_FS_USE_XATTR) {
		/* Make sure that the xattr handler exists and that no
		   error other than -ENODATA is returned by getxattr on
		   the root directory.  -ENODATA is ok, as this may be
		   the first boot of the SELinux kernel before we have
		   assigned xattr values to the filesystem. */
		if (!root_inode->i_op->getxattr) {
			printk(KERN_WARNING "SELinux: (dev %s, type %s) has no "
			       "xattr support\n", sb->s_id, sb->s_type->name);
			rc = -EOPNOTSUPP;
			goto out;
		}
		rc = root_inode->i_op->getxattr(root, XATTR_NAME_SELINUX, NULL, 0);
		if (rc < 0 && rc != -ENODATA) {
			if (rc == -EOPNOTSUPP)
				printk(KERN_WARNING "SELinux: (dev %s, type "
				       "%s) has no security xattr handler\n",
				       sb->s_id, sb->s_type->name);
			else
				printk(KERN_WARNING "SELinux: (dev %s, type "
				       "%s) getxattr errno %d\n", sb->s_id,
				       sb->s_type->name, -rc);
			goto out;
		}
	}

	sbsec->flags |= (SE_SBINITIALIZED | SE_SBLABELSUPP);

	if (sbsec->behavior > ARRAY_SIZE(labeling_behaviors))
		printk(KERN_ERR "SELinux: initialized (dev %s, type %s), unknown behavior\n",
		       sb->s_id, sb->s_type->name);
	else
		printk(KERN_DEBUG "SELinux: initialized (dev %s, type %s), %s\n",
		       sb->s_id, sb->s_type->name,
		       labeling_behaviors[sbsec->behavior-1]);

	if (sbsec->behavior == SECURITY_FS_USE_GENFS ||
	    sbsec->behavior == SECURITY_FS_USE_MNTPOINT ||
	    sbsec->behavior == SECURITY_FS_USE_NONE ||
	    sbsec->behavior > ARRAY_SIZE(labeling_behaviors))
		sbsec->flags &= ~SE_SBLABELSUPP;

	/* Special handling. Is genfs but also has in-core setxattr handler*/
	if (!strcmp(sb->s_type->name, "sysfs") ||
	    !strcmp(sb->s_type->name, "pstore") ||
	    !strcmp(sb->s_type->name, "debugfs") ||
	    !strcmp(sb->s_type->name, "rootfs"))
		sbsec->flags |= SE_SBLABELSUPP;
=======
static int selinux_is_genfs_special_handling(struct super_block *sb)
{
	/* Special handling. Genfs but also in-core setxattr handler */
	return	!strcmp(sb->s_type->name, "sysfs") ||
		!strcmp(sb->s_type->name, "pstore") ||
		!strcmp(sb->s_type->name, "debugfs") ||
		!strcmp(sb->s_type->name, "tracefs") ||
		!strcmp(sb->s_type->name, "rootfs") ||
		(selinux_policycap_cgroupseclabel() &&
		 (!strcmp(sb->s_type->name, "cgroup") ||
		  !strcmp(sb->s_type->name, "cgroup2")));
}

static int selinux_is_sblabel_mnt(struct super_block *sb)
{
	struct superblock_security_struct *sbsec = selinux_superblock(sb);

	/*
	 * IMPORTANT: Double-check logic in this function when adding a new
	 * SECURITY_FS_USE_* definition!
	 */
	BUILD_BUG_ON(SECURITY_FS_USE_MAX != 7);

	switch (sbsec->behavior) {
	case SECURITY_FS_USE_XATTR:
	case SECURITY_FS_USE_TRANS:
	case SECURITY_FS_USE_TASK:
	case SECURITY_FS_USE_NATIVE:
		return 1;

	case SECURITY_FS_USE_GENFS:
		return selinux_is_genfs_special_handling(sb);

	/* Never allow relabeling on context mounts */
	case SECURITY_FS_USE_MNTPOINT:
	case SECURITY_FS_USE_NONE:
	default:
		return 0;
	}
}

static int sb_check_xattr_support(struct super_block *sb)
{
	struct superblock_security_struct *sbsec = selinux_superblock(sb);
	struct dentry *root = sb->s_root;
	struct inode *root_inode = d_backing_inode(root);
	u32 sid;
	int rc;

	/*
	 * Make sure that the xattr handler exists and that no
	 * error other than -ENODATA is returned by getxattr on
	 * the root directory.  -ENODATA is ok, as this may be
	 * the first boot of the SELinux kernel before we have
	 * assigned xattr values to the filesystem.
	 */
	if (!(root_inode->i_opflags & IOP_XATTR)) {
		pr_warn("SELinux: (dev %s, type %s) has no xattr support\n",
			sb->s_id, sb->s_type->name);
		goto fallback;
	}

	rc = __vfs_getxattr(root, root_inode, XATTR_NAME_SELINUX, NULL, 0);
	if (rc < 0 && rc != -ENODATA) {
		if (rc == -EOPNOTSUPP) {
			pr_warn("SELinux: (dev %s, type %s) has no security xattr handler\n",
				sb->s_id, sb->s_type->name);
			goto fallback;
		} else {
			pr_warn("SELinux: (dev %s, type %s) getxattr errno %d\n",
				sb->s_id, sb->s_type->name, -rc);
			return rc;
		}
	}
	return 0;

fallback:
	/* No xattr support - try to fallback to genfs if possible. */
	rc = security_genfs_sid(sb->s_type->name, "/",
				SECCLASS_DIR, &sid);
	if (rc)
		return -EOPNOTSUPP;

	pr_warn("SELinux: (dev %s, type %s) falling back to genfs\n",
		sb->s_id, sb->s_type->name);
	sbsec->behavior = SECURITY_FS_USE_GENFS;
	sbsec->sid = sid;
	return 0;
}

static int sb_finish_set_opts(struct super_block *sb)
{
	struct superblock_security_struct *sbsec = selinux_superblock(sb);
	struct dentry *root = sb->s_root;
	struct inode *root_inode = d_backing_inode(root);
	int rc = 0;

	if (sbsec->behavior == SECURITY_FS_USE_XATTR) {
		rc = sb_check_xattr_support(sb);
		if (rc)
			return rc;
	}

	sbsec->flags |= SE_SBINITIALIZED;

	/*
	 * Explicitly set or clear SBLABEL_MNT.  It's not sufficient to simply
	 * leave the flag untouched because sb_clone_mnt_opts might be handing
	 * us a superblock that needs the flag to be cleared.
	 */
	if (selinux_is_sblabel_mnt(sb))
		sbsec->flags |= SBLABEL_MNT;
	else
		sbsec->flags &= ~SBLABEL_MNT;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	/* Initialize the root inode. */
	rc = inode_doinit_with_dentry(root_inode, root);

	/* Initialize any other inodes associated with the superblock, e.g.
	   inodes created prior to initial policy load or inodes created
	   during get_sb by a pseudo filesystem that directly
	   populates itself. */
	spin_lock(&sbsec->isec_lock);
<<<<<<< HEAD
next_inode:
	if (!list_empty(&sbsec->isec_head)) {
		struct inode_security_struct *isec =
				list_entry(sbsec->isec_head.next,
=======
	while (!list_empty(&sbsec->isec_head)) {
		struct inode_security_struct *isec =
				list_first_entry(&sbsec->isec_head,
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
					   struct inode_security_struct, list);
		struct inode *inode = isec->inode;
		list_del_init(&isec->list);
		spin_unlock(&sbsec->isec_lock);
		inode = igrab(inode);
		if (inode) {
			if (!IS_PRIVATE(inode))
<<<<<<< HEAD
				inode_doinit(inode);
			iput(inode);
		}
		spin_lock(&sbsec->isec_lock);
		goto next_inode;
	}
	spin_unlock(&sbsec->isec_lock);
out:
	return rc;
}

/*
 * This function should allow an FS to ask what it's mount security
 * options were so it can use those later for submounts, displaying
 * mount options, or whatever.
 */
static int selinux_get_mnt_opts(const struct super_block *sb,
				struct security_mnt_opts *opts)
{
	int rc = 0, i;
	struct superblock_security_struct *sbsec = sb->s_security;
	char *context = NULL;
	u32 len;
	char tmp;

	security_init_mnt_opts(opts);

	if (!(sbsec->flags & SE_SBINITIALIZED))
		return -EINVAL;

	if (!ss_initialized)
		return -EINVAL;

	tmp = sbsec->flags & SE_MNTMASK;
	/* count the number of mount options for this sb */
	for (i = 0; i < 8; i++) {
		if (tmp & 0x01)
			opts->num_mnt_opts++;
		tmp >>= 1;
	}
	/* Check if the Label support flag is set */
	if (sbsec->flags & SE_SBLABELSUPP)
		opts->num_mnt_opts++;

	opts->mnt_opts = kcalloc(opts->num_mnt_opts, sizeof(char *), GFP_ATOMIC);
	if (!opts->mnt_opts) {
		rc = -ENOMEM;
		goto out_free;
	}

	opts->mnt_opts_flags = kcalloc(opts->num_mnt_opts, sizeof(int), GFP_ATOMIC);
	if (!opts->mnt_opts_flags) {
		rc = -ENOMEM;
		goto out_free;
	}

	i = 0;
	if (sbsec->flags & FSCONTEXT_MNT) {
		rc = security_sid_to_context(sbsec->sid, &context, &len);
		if (rc)
			goto out_free;
		opts->mnt_opts[i] = context;
		opts->mnt_opts_flags[i++] = FSCONTEXT_MNT;
	}
	if (sbsec->flags & CONTEXT_MNT) {
		rc = security_sid_to_context(sbsec->mntpoint_sid, &context, &len);
		if (rc)
			goto out_free;
		opts->mnt_opts[i] = context;
		opts->mnt_opts_flags[i++] = CONTEXT_MNT;
	}
	if (sbsec->flags & DEFCONTEXT_MNT) {
		rc = security_sid_to_context(sbsec->def_sid, &context, &len);
		if (rc)
			goto out_free;
		opts->mnt_opts[i] = context;
		opts->mnt_opts_flags[i++] = DEFCONTEXT_MNT;
	}
	if (sbsec->flags & ROOTCONTEXT_MNT) {
		struct inode *root = sbsec->sb->s_root->d_inode;
		struct inode_security_struct *isec = root->i_security;

		rc = security_sid_to_context(isec->sid, &context, &len);
		if (rc)
			goto out_free;
		opts->mnt_opts[i] = context;
		opts->mnt_opts_flags[i++] = ROOTCONTEXT_MNT;
	}
	if (sbsec->flags & SE_SBLABELSUPP) {
		opts->mnt_opts[i] = NULL;
		opts->mnt_opts_flags[i++] = SE_SBLABELSUPP;
	}

	BUG_ON(i != opts->num_mnt_opts);

	return 0;

out_free:
	security_free_mnt_opts(opts);
=======
				inode_doinit_with_dentry(inode, NULL);
			iput(inode);
		}
		spin_lock(&sbsec->isec_lock);
	}
	spin_unlock(&sbsec->isec_lock);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	return rc;
}

static int bad_option(struct superblock_security_struct *sbsec, char flag,
		      u32 old_sid, u32 new_sid)
{
	char mnt_flags = sbsec->flags & SE_MNTMASK;

	/* check if the old mount command had the same options */
	if (sbsec->flags & SE_SBINITIALIZED)
		if (!(sbsec->flags & flag) ||
		    (old_sid != new_sid))
			return 1;

	/* check if we were passed the same options twice,
	 * aka someone passed context=a,context=b
	 */
	if (!(sbsec->flags & SE_SBINITIALIZED))
		if (mnt_flags & flag)
			return 1;
	return 0;
}

/*
 * Allow filesystems with binary mount data to explicitly set mount point
 * labeling information.
 */
static int selinux_set_mnt_opts(struct super_block *sb,
<<<<<<< HEAD
				struct security_mnt_opts *opts)
{
	const struct cred *cred = current_cred();
	int rc = 0, i;
	struct superblock_security_struct *sbsec = sb->s_security;
	const char *name = sb->s_type->name;
	struct inode *inode = sbsec->sb->s_root->d_inode;
	struct inode_security_struct *root_isec = inode->i_security;
	u32 fscontext_sid = 0, context_sid = 0, rootcontext_sid = 0;
	u32 defcontext_sid = 0;
	char **mount_options = opts->mnt_opts;
	int *flags = opts->mnt_opts_flags;
	int num_opts = opts->num_mnt_opts;

	mutex_lock(&sbsec->lock);

	if (!ss_initialized) {
		if (!num_opts) {
			/* Defer initialization until selinux_complete_init,
			   after the initial policy is loaded and the security
			   server is ready to handle calls. */
			goto out;
		}
		rc = -EINVAL;
		printk(KERN_WARNING "SELinux: Unable to set superblock options "
=======
				void *mnt_opts,
				unsigned long kern_flags,
				unsigned long *set_kern_flags)
{
	const struct cred *cred = current_cred();
	struct superblock_security_struct *sbsec = selinux_superblock(sb);
	struct dentry *root = sb->s_root;
	struct selinux_mnt_opts *opts = mnt_opts;
	struct inode_security_struct *root_isec;
	u32 fscontext_sid = 0, context_sid = 0, rootcontext_sid = 0;
	u32 defcontext_sid = 0;
	int rc = 0;

	/*
	 * Specifying internal flags without providing a place to
	 * place the results is not allowed
	 */
	if (kern_flags && !set_kern_flags)
		return -EINVAL;

	mutex_lock(&sbsec->lock);

	if (!selinux_initialized()) {
		if (!opts) {
			/* Defer initialization until selinux_complete_init,
			   after the initial policy is loaded and the security
			   server is ready to handle calls. */
			if (kern_flags & SECURITY_LSM_NATIVE_LABELS) {
				sbsec->flags |= SE_SBNATIVE;
				*set_kern_flags |= SECURITY_LSM_NATIVE_LABELS;
			}
			goto out;
		}
		rc = -EINVAL;
		pr_warn("SELinux: Unable to set superblock options "
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
			"before the security server is initialized\n");
		goto out;
	}

	/*
	 * Binary mount data FS will come through this function twice.  Once
	 * from an explicit call and once from the generic calls from the vfs.
	 * Since the generic VFS calls will not contain any security mount data
	 * we need to skip the double mount verification.
	 *
	 * This does open a hole in which we will not notice if the first
<<<<<<< HEAD
	 * mount using this sb set explict options and a second mount using
=======
	 * mount using this sb set explicit options and a second mount using
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	 * this sb does not set any security options.  (The first options
	 * will be used for both mounts)
	 */
	if ((sbsec->flags & SE_SBINITIALIZED) && (sb->s_type->fs_flags & FS_BINARY_MOUNTDATA)
<<<<<<< HEAD
	    && (num_opts == 0))
		goto out;

=======
	    && !opts)
		goto out;

	root_isec = backing_inode_security_novalidate(root);

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	/*
	 * parse the mount options, check if they are valid sids.
	 * also check if someone is trying to mount the same sb more
	 * than once with different security options.
	 */
<<<<<<< HEAD
	for (i = 0; i < num_opts; i++) {
		u32 sid;

		if (flags[i] == SE_SBLABELSUPP)
			continue;
		rc = security_context_to_sid(mount_options[i],
					     strlen(mount_options[i]), &sid);
		if (rc) {
			printk(KERN_WARNING "SELinux: security_context_to_sid"
			       "(%s) failed for (dev %s, type %s) errno=%d\n",
			       mount_options[i], sb->s_id, name, rc);
			goto out;
		}
		switch (flags[i]) {
		case FSCONTEXT_MNT:
			fscontext_sid = sid;

			if (bad_option(sbsec, FSCONTEXT_MNT, sbsec->sid,
					fscontext_sid))
				goto out_double_mount;

			sbsec->flags |= FSCONTEXT_MNT;
			break;
		case CONTEXT_MNT:
			context_sid = sid;

			if (bad_option(sbsec, CONTEXT_MNT, sbsec->mntpoint_sid,
					context_sid))
				goto out_double_mount;

			sbsec->flags |= CONTEXT_MNT;
			break;
		case ROOTCONTEXT_MNT:
			rootcontext_sid = sid;

			if (bad_option(sbsec, ROOTCONTEXT_MNT, root_isec->sid,
					rootcontext_sid))
				goto out_double_mount;

			sbsec->flags |= ROOTCONTEXT_MNT;

			break;
		case DEFCONTEXT_MNT:
			defcontext_sid = sid;

			if (bad_option(sbsec, DEFCONTEXT_MNT, sbsec->def_sid,
					defcontext_sid))
				goto out_double_mount;

			sbsec->flags |= DEFCONTEXT_MNT;

			break;
		default:
			rc = -EINVAL;
			goto out;
=======
	if (opts) {
		if (opts->fscontext_sid) {
			fscontext_sid = opts->fscontext_sid;
			if (bad_option(sbsec, FSCONTEXT_MNT, sbsec->sid,
					fscontext_sid))
				goto out_double_mount;
			sbsec->flags |= FSCONTEXT_MNT;
		}
		if (opts->context_sid) {
			context_sid = opts->context_sid;
			if (bad_option(sbsec, CONTEXT_MNT, sbsec->mntpoint_sid,
					context_sid))
				goto out_double_mount;
			sbsec->flags |= CONTEXT_MNT;
		}
		if (opts->rootcontext_sid) {
			rootcontext_sid = opts->rootcontext_sid;
			if (bad_option(sbsec, ROOTCONTEXT_MNT, root_isec->sid,
					rootcontext_sid))
				goto out_double_mount;
			sbsec->flags |= ROOTCONTEXT_MNT;
		}
		if (opts->defcontext_sid) {
			defcontext_sid = opts->defcontext_sid;
			if (bad_option(sbsec, DEFCONTEXT_MNT, sbsec->def_sid,
					defcontext_sid))
				goto out_double_mount;
			sbsec->flags |= DEFCONTEXT_MNT;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		}
	}

	if (sbsec->flags & SE_SBINITIALIZED) {
		/* previously mounted with options, but not on this attempt? */
<<<<<<< HEAD
		if ((sbsec->flags & SE_MNTMASK) && !num_opts)
=======
		if ((sbsec->flags & SE_MNTMASK) && !opts)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
			goto out_double_mount;
		rc = 0;
		goto out;
	}

	if (strcmp(sb->s_type->name, "proc") == 0)
		sbsec->flags |= SE_SBPROC | SE_SBGENFS;

	if (!strcmp(sb->s_type->name, "debugfs") ||
<<<<<<< HEAD
	    !strcmp(sb->s_type->name, "sysfs") ||
	    !strcmp(sb->s_type->name, "pstore"))
		sbsec->flags |= SE_SBGENFS;

	/* Determine the labeling behavior to use for this filesystem type. */
	rc = security_fs_use((sbsec->flags & SE_SBPROC) ? "proc" : sb->s_type->name, &sbsec->behavior, &sbsec->sid);
	if (rc) {
		printk(KERN_WARNING "%s: security_fs_use(%s) returned %d\n",
		       __func__, sb->s_type->name, rc);
		goto out;
=======
	    !strcmp(sb->s_type->name, "tracefs") ||
	    !strcmp(sb->s_type->name, "binder") ||
	    !strcmp(sb->s_type->name, "bpf") ||
	    !strcmp(sb->s_type->name, "pstore") ||
	    !strcmp(sb->s_type->name, "securityfs"))
		sbsec->flags |= SE_SBGENFS;

	if (!strcmp(sb->s_type->name, "sysfs") ||
	    !strcmp(sb->s_type->name, "cgroup") ||
	    !strcmp(sb->s_type->name, "cgroup2"))
		sbsec->flags |= SE_SBGENFS | SE_SBGENFS_XATTR;

	if (!sbsec->behavior) {
		/*
		 * Determine the labeling behavior to use for this
		 * filesystem type.
		 */
		rc = security_fs_use(sb);
		if (rc) {
			pr_warn("%s: security_fs_use(%s) returned %d\n",
					__func__, sb->s_type->name, rc);
			goto out;
		}
	}

	/*
	 * If this is a user namespace mount and the filesystem type is not
	 * explicitly whitelisted, then no contexts are allowed on the command
	 * line and security labels must be ignored.
	 */
	if (sb->s_user_ns != &init_user_ns &&
	    strcmp(sb->s_type->name, "tmpfs") &&
	    strcmp(sb->s_type->name, "ramfs") &&
	    strcmp(sb->s_type->name, "devpts") &&
	    strcmp(sb->s_type->name, "overlay")) {
		if (context_sid || fscontext_sid || rootcontext_sid ||
		    defcontext_sid) {
			rc = -EACCES;
			goto out;
		}
		if (sbsec->behavior == SECURITY_FS_USE_XATTR) {
			sbsec->behavior = SECURITY_FS_USE_MNTPOINT;
			rc = security_transition_sid(current_sid(),
						     current_sid(),
						     SECCLASS_FILE, NULL,
						     &sbsec->mntpoint_sid);
			if (rc)
				goto out;
		}
		goto out_set_opts;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	}

	/* sets the context of the superblock for the fs being mounted. */
	if (fscontext_sid) {
		rc = may_context_mount_sb_relabel(fscontext_sid, sbsec, cred);
		if (rc)
			goto out;

		sbsec->sid = fscontext_sid;
	}

	/*
	 * Switch to using mount point labeling behavior.
	 * sets the label used on all file below the mountpoint, and will set
	 * the superblock context if not already set.
	 */
<<<<<<< HEAD
=======
	if (sbsec->flags & SE_SBNATIVE) {
		/*
		 * This means we are initializing a superblock that has been
		 * mounted before the SELinux was initialized and the
		 * filesystem requested native labeling. We had already
		 * returned SECURITY_LSM_NATIVE_LABELS in *set_kern_flags
		 * in the original mount attempt, so now we just need to set
		 * the SECURITY_FS_USE_NATIVE behavior.
		 */
		sbsec->behavior = SECURITY_FS_USE_NATIVE;
	} else if (kern_flags & SECURITY_LSM_NATIVE_LABELS && !context_sid) {
		sbsec->behavior = SECURITY_FS_USE_NATIVE;
		*set_kern_flags |= SECURITY_LSM_NATIVE_LABELS;
	}

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	if (context_sid) {
		if (!fscontext_sid) {
			rc = may_context_mount_sb_relabel(context_sid, sbsec,
							  cred);
			if (rc)
				goto out;
			sbsec->sid = context_sid;
		} else {
			rc = may_context_mount_inode_relabel(context_sid, sbsec,
							     cred);
			if (rc)
				goto out;
		}
		if (!rootcontext_sid)
			rootcontext_sid = context_sid;

		sbsec->mntpoint_sid = context_sid;
		sbsec->behavior = SECURITY_FS_USE_MNTPOINT;
	}

	if (rootcontext_sid) {
		rc = may_context_mount_inode_relabel(rootcontext_sid, sbsec,
						     cred);
		if (rc)
			goto out;

		root_isec->sid = rootcontext_sid;
<<<<<<< HEAD
		root_isec->initialized = 1;
	}

	if (defcontext_sid) {
		if (sbsec->behavior != SECURITY_FS_USE_XATTR) {
			rc = -EINVAL;
			printk(KERN_WARNING "SELinux: defcontext option is "
=======
		root_isec->initialized = LABEL_INITIALIZED;
	}

	if (defcontext_sid) {
		if (sbsec->behavior != SECURITY_FS_USE_XATTR &&
			sbsec->behavior != SECURITY_FS_USE_NATIVE) {
			rc = -EINVAL;
			pr_warn("SELinux: defcontext option is "
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
			       "invalid for this filesystem type\n");
			goto out;
		}

		if (defcontext_sid != sbsec->def_sid) {
			rc = may_context_mount_inode_relabel(defcontext_sid,
							     sbsec, cred);
			if (rc)
				goto out;
		}

		sbsec->def_sid = defcontext_sid;
	}

<<<<<<< HEAD
=======
out_set_opts:
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	rc = sb_finish_set_opts(sb);
out:
	mutex_unlock(&sbsec->lock);
	return rc;
out_double_mount:
	rc = -EINVAL;
<<<<<<< HEAD
	printk(KERN_WARNING "SELinux: mount invalid.  Same superblock, different "
	       "security settings for (dev %s, type %s)\n", sb->s_id, name);
	goto out;
}

static void selinux_sb_clone_mnt_opts(const struct super_block *oldsb,
					struct super_block *newsb)
{
	const struct superblock_security_struct *oldsbsec = oldsb->s_security;
	struct superblock_security_struct *newsbsec = newsb->s_security;
=======
	pr_warn("SELinux: mount invalid.  Same superblock, different "
	       "security settings for (dev %s, type %s)\n", sb->s_id,
	       sb->s_type->name);
	goto out;
}

static int selinux_cmp_sb_context(const struct super_block *oldsb,
				    const struct super_block *newsb)
{
	struct superblock_security_struct *old = selinux_superblock(oldsb);
	struct superblock_security_struct *new = selinux_superblock(newsb);
	char oldflags = old->flags & SE_MNTMASK;
	char newflags = new->flags & SE_MNTMASK;

	if (oldflags != newflags)
		goto mismatch;
	if ((oldflags & FSCONTEXT_MNT) && old->sid != new->sid)
		goto mismatch;
	if ((oldflags & CONTEXT_MNT) && old->mntpoint_sid != new->mntpoint_sid)
		goto mismatch;
	if ((oldflags & DEFCONTEXT_MNT) && old->def_sid != new->def_sid)
		goto mismatch;
	if (oldflags & ROOTCONTEXT_MNT) {
		struct inode_security_struct *oldroot = backing_inode_security(oldsb->s_root);
		struct inode_security_struct *newroot = backing_inode_security(newsb->s_root);
		if (oldroot->sid != newroot->sid)
			goto mismatch;
	}
	return 0;
mismatch:
	pr_warn("SELinux: mount invalid.  Same superblock, "
			    "different security settings for (dev %s, "
			    "type %s)\n", newsb->s_id, newsb->s_type->name);
	return -EBUSY;
}

static int selinux_sb_clone_mnt_opts(const struct super_block *oldsb,
					struct super_block *newsb,
					unsigned long kern_flags,
					unsigned long *set_kern_flags)
{
	int rc = 0;
	const struct superblock_security_struct *oldsbsec =
						selinux_superblock(oldsb);
	struct superblock_security_struct *newsbsec = selinux_superblock(newsb);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	int set_fscontext =	(oldsbsec->flags & FSCONTEXT_MNT);
	int set_context =	(oldsbsec->flags & CONTEXT_MNT);
	int set_rootcontext =	(oldsbsec->flags & ROOTCONTEXT_MNT);

	/*
<<<<<<< HEAD
	 * if the parent was able to be mounted it clearly had no special lsm
	 * mount options.  thus we can safely deal with this superblock later
	 */
	if (!ss_initialized)
		return;
=======
	 * Specifying internal flags without providing a place to
	 * place the results is not allowed.
	 */
	if (kern_flags && !set_kern_flags)
		return -EINVAL;

	mutex_lock(&newsbsec->lock);

	/*
	 * if the parent was able to be mounted it clearly had no special lsm
	 * mount options.  thus we can safely deal with this superblock later
	 */
	if (!selinux_initialized()) {
		if (kern_flags & SECURITY_LSM_NATIVE_LABELS) {
			newsbsec->flags |= SE_SBNATIVE;
			*set_kern_flags |= SECURITY_LSM_NATIVE_LABELS;
		}
		goto out;
	}
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	/* how can we clone if the old one wasn't set up?? */
	BUG_ON(!(oldsbsec->flags & SE_SBINITIALIZED));

<<<<<<< HEAD
	/* if fs is reusing a sb, just let its options stand... */
	if (newsbsec->flags & SE_SBINITIALIZED)
		return;

	mutex_lock(&newsbsec->lock);
=======
	/* if fs is reusing a sb, make sure that the contexts match */
	if (newsbsec->flags & SE_SBINITIALIZED) {
		mutex_unlock(&newsbsec->lock);
		if ((kern_flags & SECURITY_LSM_NATIVE_LABELS) && !set_context)
			*set_kern_flags |= SECURITY_LSM_NATIVE_LABELS;
		return selinux_cmp_sb_context(oldsb, newsb);
	}
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	newsbsec->flags = oldsbsec->flags;

	newsbsec->sid = oldsbsec->sid;
	newsbsec->def_sid = oldsbsec->def_sid;
	newsbsec->behavior = oldsbsec->behavior;

<<<<<<< HEAD
=======
	if (newsbsec->behavior == SECURITY_FS_USE_NATIVE &&
		!(kern_flags & SECURITY_LSM_NATIVE_LABELS) && !set_context) {
		rc = security_fs_use(newsb);
		if (rc)
			goto out;
	}

	if (kern_flags & SECURITY_LSM_NATIVE_LABELS && !set_context) {
		newsbsec->behavior = SECURITY_FS_USE_NATIVE;
		*set_kern_flags |= SECURITY_LSM_NATIVE_LABELS;
	}

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	if (set_context) {
		u32 sid = oldsbsec->mntpoint_sid;

		if (!set_fscontext)
			newsbsec->sid = sid;
		if (!set_rootcontext) {
<<<<<<< HEAD
			struct inode *newinode = newsb->s_root->d_inode;
			struct inode_security_struct *newisec = newinode->i_security;
=======
			struct inode_security_struct *newisec = backing_inode_security(newsb->s_root);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
			newisec->sid = sid;
		}
		newsbsec->mntpoint_sid = sid;
	}
	if (set_rootcontext) {
<<<<<<< HEAD
		const struct inode *oldinode = oldsb->s_root->d_inode;
		const struct inode_security_struct *oldisec = oldinode->i_security;
		struct inode *newinode = newsb->s_root->d_inode;
		struct inode_security_struct *newisec = newinode->i_security;
=======
		const struct inode_security_struct *oldisec = backing_inode_security(oldsb->s_root);
		struct inode_security_struct *newisec = backing_inode_security(newsb->s_root);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

		newisec->sid = oldisec->sid;
	}

	sb_finish_set_opts(newsb);
<<<<<<< HEAD
	mutex_unlock(&newsbsec->lock);
}

static int selinux_parse_opts_str(char *options,
				  struct security_mnt_opts *opts)
{
	char *p;
	char *context = NULL, *defcontext = NULL;
	char *fscontext = NULL, *rootcontext = NULL;
	int rc, num_mnt_opts = 0;

	opts->num_mnt_opts = 0;

	/* Standard string-based options. */
	while ((p = strsep(&options, "|")) != NULL) {
		int token;
		substring_t args[MAX_OPT_ARGS];

		if (!*p)
			continue;

		token = match_token(p, tokens, args);

		switch (token) {
		case Opt_context:
			if (context || defcontext) {
				rc = -EINVAL;
				printk(KERN_WARNING SEL_MOUNT_FAIL_MSG);
				goto out_err;
			}
			context = match_strdup(&args[0]);
			if (!context) {
				rc = -ENOMEM;
				goto out_err;
			}
			break;

		case Opt_fscontext:
			if (fscontext) {
				rc = -EINVAL;
				printk(KERN_WARNING SEL_MOUNT_FAIL_MSG);
				goto out_err;
			}
			fscontext = match_strdup(&args[0]);
			if (!fscontext) {
				rc = -ENOMEM;
				goto out_err;
			}
			break;

		case Opt_rootcontext:
			if (rootcontext) {
				rc = -EINVAL;
				printk(KERN_WARNING SEL_MOUNT_FAIL_MSG);
				goto out_err;
			}
			rootcontext = match_strdup(&args[0]);
			if (!rootcontext) {
				rc = -ENOMEM;
				goto out_err;
			}
			break;

		case Opt_defcontext:
			if (context || defcontext) {
				rc = -EINVAL;
				printk(KERN_WARNING SEL_MOUNT_FAIL_MSG);
				goto out_err;
			}
			defcontext = match_strdup(&args[0]);
			if (!defcontext) {
				rc = -ENOMEM;
				goto out_err;
			}
			break;
		case Opt_labelsupport:
			break;
		default:
			rc = -EINVAL;
			printk(KERN_WARNING "SELinux:  unknown mount option\n");
			goto out_err;

		}
	}

	rc = -ENOMEM;
	opts->mnt_opts = kcalloc(NUM_SEL_MNT_OPTS, sizeof(char *), GFP_ATOMIC);
	if (!opts->mnt_opts)
		goto out_err;

	opts->mnt_opts_flags = kcalloc(NUM_SEL_MNT_OPTS, sizeof(int), GFP_ATOMIC);
	if (!opts->mnt_opts_flags) {
		kfree(opts->mnt_opts);
		goto out_err;
	}

	if (fscontext) {
		opts->mnt_opts[num_mnt_opts] = fscontext;
		opts->mnt_opts_flags[num_mnt_opts++] = FSCONTEXT_MNT;
	}
	if (context) {
		opts->mnt_opts[num_mnt_opts] = context;
		opts->mnt_opts_flags[num_mnt_opts++] = CONTEXT_MNT;
	}
	if (rootcontext) {
		opts->mnt_opts[num_mnt_opts] = rootcontext;
		opts->mnt_opts_flags[num_mnt_opts++] = ROOTCONTEXT_MNT;
	}
	if (defcontext) {
		opts->mnt_opts[num_mnt_opts] = defcontext;
		opts->mnt_opts_flags[num_mnt_opts++] = DEFCONTEXT_MNT;
	}

	opts->num_mnt_opts = num_mnt_opts;
	return 0;

out_err:
	kfree(context);
	kfree(defcontext);
	kfree(fscontext);
	kfree(rootcontext);
	return rc;
}
/*
 * string mount options parsing and call set the sbsec
 */
static int superblock_doinit(struct super_block *sb, void *data)
{
	int rc = 0;
	char *options = data;
	struct security_mnt_opts opts;

	security_init_mnt_opts(&opts);

	if (!data)
		goto out;

	BUG_ON(sb->s_type->fs_flags & FS_BINARY_MOUNTDATA);

	rc = selinux_parse_opts_str(options, &opts);
	if (rc)
		goto out_err;

out:
	rc = selinux_set_mnt_opts(sb, &opts);

out_err:
	security_free_mnt_opts(&opts);
	return rc;
}

static void selinux_write_opts(struct seq_file *m,
			       struct security_mnt_opts *opts)
{
	int i;
	char *prefix;

	for (i = 0; i < opts->num_mnt_opts; i++) {
		char *has_comma;

		if (opts->mnt_opts[i])
			has_comma = strchr(opts->mnt_opts[i], ',');
		else
			has_comma = NULL;

		switch (opts->mnt_opts_flags[i]) {
		case CONTEXT_MNT:
			prefix = CONTEXT_STR;
			break;
		case FSCONTEXT_MNT:
			prefix = FSCONTEXT_STR;
			break;
		case ROOTCONTEXT_MNT:
			prefix = ROOTCONTEXT_STR;
			break;
		case DEFCONTEXT_MNT:
			prefix = DEFCONTEXT_STR;
			break;
		case SE_SBLABELSUPP:
			seq_putc(m, ',');
			seq_puts(m, LABELSUPP_STR);
			continue;
		default:
			BUG();
			return;
		};
		/* we need a comma before each option */
		seq_putc(m, ',');
		seq_puts(m, prefix);
		if (has_comma)
			seq_putc(m, '\"');
		seq_escape(m, opts->mnt_opts[i], "\"\n\\");
		if (has_comma)
			seq_putc(m, '\"');
	}
=======
out:
	mutex_unlock(&newsbsec->lock);
	return rc;
}

/*
 * NOTE: the caller is responsible for freeing the memory even if on error.
 */
static int selinux_add_opt(int token, const char *s, void **mnt_opts)
{
	struct selinux_mnt_opts *opts = *mnt_opts;
	u32 *dst_sid;
	int rc;

	if (token == Opt_seclabel)
		/* eaten and completely ignored */
		return 0;
	if (!s)
		return -EINVAL;

	if (!selinux_initialized()) {
		pr_warn("SELinux: Unable to set superblock options before the security server is initialized\n");
		return -EINVAL;
	}

	if (!opts) {
		opts = kzalloc(sizeof(*opts), GFP_KERNEL);
		if (!opts)
			return -ENOMEM;
		*mnt_opts = opts;
	}

	switch (token) {
	case Opt_context:
		if (opts->context_sid || opts->defcontext_sid)
			goto err;
		dst_sid = &opts->context_sid;
		break;
	case Opt_fscontext:
		if (opts->fscontext_sid)
			goto err;
		dst_sid = &opts->fscontext_sid;
		break;
	case Opt_rootcontext:
		if (opts->rootcontext_sid)
			goto err;
		dst_sid = &opts->rootcontext_sid;
		break;
	case Opt_defcontext:
		if (opts->context_sid || opts->defcontext_sid)
			goto err;
		dst_sid = &opts->defcontext_sid;
		break;
	default:
		WARN_ON(1);
		return -EINVAL;
	}
	rc = security_context_str_to_sid(s, dst_sid, GFP_KERNEL);
	if (rc)
		pr_warn("SELinux: security_context_str_to_sid (%s) failed with errno=%d\n",
			s, rc);
	return rc;

err:
	pr_warn(SEL_MOUNT_FAIL_MSG);
	return -EINVAL;
}

static int show_sid(struct seq_file *m, u32 sid)
{
	char *context = NULL;
	u32 len;
	int rc;

	rc = security_sid_to_context(sid, &context, &len);
	if (!rc) {
		bool has_comma = strchr(context, ',');

		seq_putc(m, '=');
		if (has_comma)
			seq_putc(m, '\"');
		seq_escape(m, context, "\"\n\\");
		if (has_comma)
			seq_putc(m, '\"');
	}
	kfree(context);
	return rc;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

static int selinux_sb_show_options(struct seq_file *m, struct super_block *sb)
{
<<<<<<< HEAD
	struct security_mnt_opts opts;
	int rc;

	rc = selinux_get_mnt_opts(sb, &opts);
	if (rc) {
		/* before policy load we may get EINVAL, don't show anything */
		if (rc == -EINVAL)
			rc = 0;
		return rc;
	}

	selinux_write_opts(m, &opts);

	security_free_mnt_opts(&opts);

	return rc;
=======
	struct superblock_security_struct *sbsec = selinux_superblock(sb);
	int rc;

	if (!(sbsec->flags & SE_SBINITIALIZED))
		return 0;

	if (!selinux_initialized())
		return 0;

	if (sbsec->flags & FSCONTEXT_MNT) {
		seq_putc(m, ',');
		seq_puts(m, FSCONTEXT_STR);
		rc = show_sid(m, sbsec->sid);
		if (rc)
			return rc;
	}
	if (sbsec->flags & CONTEXT_MNT) {
		seq_putc(m, ',');
		seq_puts(m, CONTEXT_STR);
		rc = show_sid(m, sbsec->mntpoint_sid);
		if (rc)
			return rc;
	}
	if (sbsec->flags & DEFCONTEXT_MNT) {
		seq_putc(m, ',');
		seq_puts(m, DEFCONTEXT_STR);
		rc = show_sid(m, sbsec->def_sid);
		if (rc)
			return rc;
	}
	if (sbsec->flags & ROOTCONTEXT_MNT) {
		struct dentry *root = sb->s_root;
		struct inode_security_struct *isec = backing_inode_security(root);
		seq_putc(m, ',');
		seq_puts(m, ROOTCONTEXT_STR);
		rc = show_sid(m, isec->sid);
		if (rc)
			return rc;
	}
	if (sbsec->flags & SBLABEL_MNT) {
		seq_putc(m, ',');
		seq_puts(m, SECLABEL_STR);
	}
	return 0;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

static inline u16 inode_mode_to_security_class(umode_t mode)
{
	switch (mode & S_IFMT) {
	case S_IFSOCK:
		return SECCLASS_SOCK_FILE;
	case S_IFLNK:
		return SECCLASS_LNK_FILE;
	case S_IFREG:
		return SECCLASS_FILE;
	case S_IFBLK:
		return SECCLASS_BLK_FILE;
	case S_IFDIR:
		return SECCLASS_DIR;
	case S_IFCHR:
		return SECCLASS_CHR_FILE;
	case S_IFIFO:
		return SECCLASS_FIFO_FILE;

	}

	return SECCLASS_FILE;
}

static inline int default_protocol_stream(int protocol)
{
<<<<<<< HEAD
	return (protocol == IPPROTO_IP || protocol == IPPROTO_TCP);
=======
	return (protocol == IPPROTO_IP || protocol == IPPROTO_TCP ||
		protocol == IPPROTO_MPTCP);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

static inline int default_protocol_dgram(int protocol)
{
	return (protocol == IPPROTO_IP || protocol == IPPROTO_UDP);
}

static inline u16 socket_type_to_security_class(int family, int type, int protocol)
{
<<<<<<< HEAD
=======
	bool extsockclass = selinux_policycap_extsockclass();

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	switch (family) {
	case PF_UNIX:
		switch (type) {
		case SOCK_STREAM:
		case SOCK_SEQPACKET:
			return SECCLASS_UNIX_STREAM_SOCKET;
		case SOCK_DGRAM:
<<<<<<< HEAD
=======
		case SOCK_RAW:
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
			return SECCLASS_UNIX_DGRAM_SOCKET;
		}
		break;
	case PF_INET:
	case PF_INET6:
		switch (type) {
		case SOCK_STREAM:
<<<<<<< HEAD
			if (default_protocol_stream(protocol))
				return SECCLASS_TCP_SOCKET;
=======
		case SOCK_SEQPACKET:
			if (default_protocol_stream(protocol))
				return SECCLASS_TCP_SOCKET;
			else if (extsockclass && protocol == IPPROTO_SCTP)
				return SECCLASS_SCTP_SOCKET;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
			else
				return SECCLASS_RAWIP_SOCKET;
		case SOCK_DGRAM:
			if (default_protocol_dgram(protocol))
				return SECCLASS_UDP_SOCKET;
<<<<<<< HEAD
=======
			else if (extsockclass && (protocol == IPPROTO_ICMP ||
						  protocol == IPPROTO_ICMPV6))
				return SECCLASS_ICMP_SOCKET;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
			else
				return SECCLASS_RAWIP_SOCKET;
		case SOCK_DCCP:
			return SECCLASS_DCCP_SOCKET;
		default:
			return SECCLASS_RAWIP_SOCKET;
		}
		break;
	case PF_NETLINK:
		switch (protocol) {
		case NETLINK_ROUTE:
			return SECCLASS_NETLINK_ROUTE_SOCKET;
		case NETLINK_SOCK_DIAG:
			return SECCLASS_NETLINK_TCPDIAG_SOCKET;
		case NETLINK_NFLOG:
			return SECCLASS_NETLINK_NFLOG_SOCKET;
		case NETLINK_XFRM:
			return SECCLASS_NETLINK_XFRM_SOCKET;
		case NETLINK_SELINUX:
			return SECCLASS_NETLINK_SELINUX_SOCKET;
		case NETLINK_ISCSI:
			return SECCLASS_NETLINK_ISCSI_SOCKET;
		case NETLINK_AUDIT:
			return SECCLASS_NETLINK_AUDIT_SOCKET;
		case NETLINK_FIB_LOOKUP:
			return SECCLASS_NETLINK_FIB_LOOKUP_SOCKET;
		case NETLINK_CONNECTOR:
			return SECCLASS_NETLINK_CONNECTOR_SOCKET;
		case NETLINK_NETFILTER:
			return SECCLASS_NETLINK_NETFILTER_SOCKET;
		case NETLINK_DNRTMSG:
			return SECCLASS_NETLINK_DNRT_SOCKET;
		case NETLINK_KOBJECT_UEVENT:
			return SECCLASS_NETLINK_KOBJECT_UEVENT_SOCKET;
		case NETLINK_GENERIC:
			return SECCLASS_NETLINK_GENERIC_SOCKET;
		case NETLINK_SCSITRANSPORT:
			return SECCLASS_NETLINK_SCSITRANSPORT_SOCKET;
		case NETLINK_RDMA:
			return SECCLASS_NETLINK_RDMA_SOCKET;
		case NETLINK_CRYPTO:
			return SECCLASS_NETLINK_CRYPTO_SOCKET;
		default:
			return SECCLASS_NETLINK_SOCKET;
		}
	case PF_PACKET:
		return SECCLASS_PACKET_SOCKET;
	case PF_KEY:
		return SECCLASS_KEY_SOCKET;
	case PF_APPLETALK:
		return SECCLASS_APPLETALK_SOCKET;
	}

<<<<<<< HEAD
=======
	if (extsockclass) {
		switch (family) {
		case PF_AX25:
			return SECCLASS_AX25_SOCKET;
		case PF_IPX:
			return SECCLASS_IPX_SOCKET;
		case PF_NETROM:
			return SECCLASS_NETROM_SOCKET;
		case PF_ATMPVC:
			return SECCLASS_ATMPVC_SOCKET;
		case PF_X25:
			return SECCLASS_X25_SOCKET;
		case PF_ROSE:
			return SECCLASS_ROSE_SOCKET;
		case PF_DECnet:
			return SECCLASS_DECNET_SOCKET;
		case PF_ATMSVC:
			return SECCLASS_ATMSVC_SOCKET;
		case PF_RDS:
			return SECCLASS_RDS_SOCKET;
		case PF_IRDA:
			return SECCLASS_IRDA_SOCKET;
		case PF_PPPOX:
			return SECCLASS_PPPOX_SOCKET;
		case PF_LLC:
			return SECCLASS_LLC_SOCKET;
		case PF_CAN:
			return SECCLASS_CAN_SOCKET;
		case PF_TIPC:
			return SECCLASS_TIPC_SOCKET;
		case PF_BLUETOOTH:
			return SECCLASS_BLUETOOTH_SOCKET;
		case PF_IUCV:
			return SECCLASS_IUCV_SOCKET;
		case PF_RXRPC:
			return SECCLASS_RXRPC_SOCKET;
		case PF_ISDN:
			return SECCLASS_ISDN_SOCKET;
		case PF_PHONET:
			return SECCLASS_PHONET_SOCKET;
		case PF_IEEE802154:
			return SECCLASS_IEEE802154_SOCKET;
		case PF_CAIF:
			return SECCLASS_CAIF_SOCKET;
		case PF_ALG:
			return SECCLASS_ALG_SOCKET;
		case PF_NFC:
			return SECCLASS_NFC_SOCKET;
		case PF_VSOCK:
			return SECCLASS_VSOCK_SOCKET;
		case PF_KCM:
			return SECCLASS_KCM_SOCKET;
		case PF_QIPCRTR:
			return SECCLASS_QIPCRTR_SOCKET;
		case PF_SMC:
			return SECCLASS_SMC_SOCKET;
		case PF_XDP:
			return SECCLASS_XDP_SOCKET;
		case PF_MCTP:
			return SECCLASS_MCTP_SOCKET;
#if PF_MAX > 46
#error New address family defined, please update this function.
#endif
		}
	}

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	return SECCLASS_SOCKET;
}

static int selinux_genfs_get_sid(struct dentry *dentry,
				 u16 tclass,
				 u16 flags,
				 u32 *sid)
{
	int rc;
<<<<<<< HEAD
	struct super_block *sb = dentry->d_inode->i_sb;
=======
	struct super_block *sb = dentry->d_sb;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	char *buffer, *path;

	buffer = (char *)__get_free_page(GFP_KERNEL);
	if (!buffer)
		return -ENOMEM;

	path = dentry_path_raw(dentry, buffer, PAGE_SIZE);
	if (IS_ERR(path))
		rc = PTR_ERR(path);
	else {
		if (flags & SE_SBPROC) {
			/* each process gets a /proc/PID/ entry. Strip off the
			 * PID part to get a valid selinux labeling.
			 * e.g. /proc/1/net/rpc/nfs -> /net/rpc/nfs */
			while (path[1] >= '0' && path[1] <= '9') {
				path[1] = '/';
				path++;
			}
		}
<<<<<<< HEAD
		rc = security_genfs_sid(sb->s_type->name, path, tclass, sid);
=======
		rc = security_genfs_sid(sb->s_type->name,
					path, tclass, sid);
		if (rc == -ENOENT) {
			/* No match in policy, mark as unlabeled. */
			*sid = SECINITSID_UNLABELED;
			rc = 0;
		}
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	}
	free_page((unsigned long)buffer);
	return rc;
}

<<<<<<< HEAD
=======
static int inode_doinit_use_xattr(struct inode *inode, struct dentry *dentry,
				  u32 def_sid, u32 *sid)
{
#define INITCONTEXTLEN 255
	char *context;
	unsigned int len;
	int rc;

	len = INITCONTEXTLEN;
	context = kmalloc(len + 1, GFP_NOFS);
	if (!context)
		return -ENOMEM;

	context[len] = '\0';
	rc = __vfs_getxattr(dentry, inode, XATTR_NAME_SELINUX, context, len);
	if (rc == -ERANGE) {
		kfree(context);

		/* Need a larger buffer.  Query for the right size. */
		rc = __vfs_getxattr(dentry, inode, XATTR_NAME_SELINUX, NULL, 0);
		if (rc < 0)
			return rc;

		len = rc;
		context = kmalloc(len + 1, GFP_NOFS);
		if (!context)
			return -ENOMEM;

		context[len] = '\0';
		rc = __vfs_getxattr(dentry, inode, XATTR_NAME_SELINUX,
				    context, len);
	}
	if (rc < 0) {
		kfree(context);
		if (rc != -ENODATA) {
			pr_warn("SELinux: %s:  getxattr returned %d for dev=%s ino=%ld\n",
				__func__, -rc, inode->i_sb->s_id, inode->i_ino);
			return rc;
		}
		*sid = def_sid;
		return 0;
	}

	rc = security_context_to_sid_default(context, rc, sid,
					     def_sid, GFP_NOFS);
	if (rc) {
		char *dev = inode->i_sb->s_id;
		unsigned long ino = inode->i_ino;

		if (rc == -EINVAL) {
			pr_notice_ratelimited("SELinux: inode=%lu on dev=%s was found to have an invalid context=%s.  This indicates you may need to relabel the inode or the filesystem in question.\n",
					      ino, dev, context);
		} else {
			pr_warn("SELinux: %s:  context_to_sid(%s) returned %d for dev=%s ino=%ld\n",
				__func__, context, -rc, dev, ino);
		}
	}
	kfree(context);
	return 0;
}

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
/* The inode's security attributes must be initialized before first use. */
static int inode_doinit_with_dentry(struct inode *inode, struct dentry *opt_dentry)
{
	struct superblock_security_struct *sbsec = NULL;
<<<<<<< HEAD
	struct inode_security_struct *isec = inode->i_security;
	u32 sid;
	struct dentry *dentry;
#define INITCONTEXTLEN 255
	char *context = NULL;
	unsigned len = 0;
	int rc = 0;

	if (isec->initialized)
		goto out;

	mutex_lock(&isec->lock);
	if (isec->initialized)
		goto out_unlock;

	sbsec = inode->i_sb->s_security;
=======
	struct inode_security_struct *isec = selinux_inode(inode);
	u32 task_sid, sid = 0;
	u16 sclass;
	struct dentry *dentry;
	int rc = 0;

	if (isec->initialized == LABEL_INITIALIZED)
		return 0;

	spin_lock(&isec->lock);
	if (isec->initialized == LABEL_INITIALIZED)
		goto out_unlock;

	if (isec->sclass == SECCLASS_FILE)
		isec->sclass = inode_mode_to_security_class(inode->i_mode);

	sbsec = selinux_superblock(inode->i_sb);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	if (!(sbsec->flags & SE_SBINITIALIZED)) {
		/* Defer initialization until selinux_complete_init,
		   after the initial policy is loaded and the security
		   server is ready to handle calls. */
		spin_lock(&sbsec->isec_lock);
		if (list_empty(&isec->list))
			list_add(&isec->list, &sbsec->isec_head);
		spin_unlock(&sbsec->isec_lock);
		goto out_unlock;
	}

<<<<<<< HEAD
	switch (sbsec->behavior) {
	case SECURITY_FS_USE_XATTR:
		if (!inode->i_op->getxattr) {
			isec->sid = sbsec->def_sid;
			break;
		}

=======
	sclass = isec->sclass;
	task_sid = isec->task_sid;
	sid = isec->sid;
	isec->initialized = LABEL_PENDING;
	spin_unlock(&isec->lock);

	switch (sbsec->behavior) {
	/*
	 * In case of SECURITY_FS_USE_NATIVE we need to re-fetch the labels
	 * via xattr when called from delayed_superblock_init().
	 */
	case SECURITY_FS_USE_NATIVE:
	case SECURITY_FS_USE_XATTR:
		if (!(inode->i_opflags & IOP_XATTR)) {
			sid = sbsec->def_sid;
			break;
		}
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		/* Need a dentry, since the xattr API requires one.
		   Life would be simpler if we could just pass the inode. */
		if (opt_dentry) {
			/* Called from d_instantiate or d_splice_alias. */
			dentry = dget(opt_dentry);
		} else {
<<<<<<< HEAD
			/* Called from selinux_complete_init, try to find a dentry. */
			dentry = d_find_alias(inode);
=======
			/*
			 * Called from selinux_complete_init, try to find a dentry.
			 * Some filesystems really want a connected one, so try
			 * that first.  We could split SECURITY_FS_USE_XATTR in
			 * two, depending upon that...
			 */
			dentry = d_find_alias(inode);
			if (!dentry)
				dentry = d_find_any_alias(inode);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		}
		if (!dentry) {
			/*
			 * this is can be hit on boot when a file is accessed
			 * before the policy is loaded.  When we load policy we
			 * may find inodes that have no dentry on the
			 * sbsec->isec_head list.  No reason to complain as these
			 * will get fixed up the next time we go through
			 * inode_doinit with a dentry, before these inodes could
			 * be used again by userspace.
			 */
<<<<<<< HEAD
			goto out_unlock;
		}

		len = INITCONTEXTLEN;
		context = kmalloc(len+1, GFP_NOFS);
		if (!context) {
			rc = -ENOMEM;
			dput(dentry);
			goto out_unlock;
		}
		context[len] = '\0';
		rc = inode->i_op->getxattr(dentry, XATTR_NAME_SELINUX,
					   context, len);
		if (rc == -ERANGE) {
			kfree(context);

			/* Need a larger buffer.  Query for the right size. */
			rc = inode->i_op->getxattr(dentry, XATTR_NAME_SELINUX,
						   NULL, 0);
			if (rc < 0) {
				dput(dentry);
				goto out_unlock;
			}
			len = rc;
			context = kmalloc(len+1, GFP_NOFS);
			if (!context) {
				rc = -ENOMEM;
				dput(dentry);
				goto out_unlock;
			}
			context[len] = '\0';
			rc = inode->i_op->getxattr(dentry,
						   XATTR_NAME_SELINUX,
						   context, len);
		}
		dput(dentry);
		if (rc < 0) {
			if (rc != -ENODATA) {
				printk(KERN_WARNING "SELinux: %s:  getxattr returned "
				       "%d for dev=%s ino=%ld\n", __func__,
				       -rc, inode->i_sb->s_id, inode->i_ino);
				kfree(context);
				goto out_unlock;
			}
			/* Map ENODATA to the default file SID */
			sid = sbsec->def_sid;
			rc = 0;
		} else {
			rc = security_context_to_sid_default(context, rc, &sid,
							     sbsec->def_sid,
							     GFP_NOFS);
			if (rc) {
				char *dev = inode->i_sb->s_id;
				unsigned long ino = inode->i_ino;

				if (rc == -EINVAL) {
					if (printk_ratelimit())
						printk(KERN_NOTICE "SELinux: inode=%lu on dev=%s was found to have an invalid "
							"context=%s.  This indicates you may need to relabel the inode or the "
							"filesystem in question.\n", ino, dev, context);
				} else {
					printk(KERN_WARNING "SELinux: %s:  context_to_sid(%s) "
					       "returned %d for dev=%s ino=%ld\n",
					       __func__, context, -rc, dev, ino);
				}
				kfree(context);
				/* Leave with the unlabeled SID */
				rc = 0;
				break;
			}
		}
		kfree(context);
		isec->sid = sid;
		break;
	case SECURITY_FS_USE_TASK:
		isec->sid = isec->task_sid;
		break;
	case SECURITY_FS_USE_TRANS:
		/* Default to the fs SID. */
		isec->sid = sbsec->sid;

		/* Try to obtain a transition SID. */
		isec->sclass = inode_mode_to_security_class(inode->i_mode);
		rc = security_transition_sid(isec->task_sid, sbsec->sid,
					     isec->sclass, NULL, &sid);
		if (rc)
			goto out_unlock;
		isec->sid = sid;
		break;
	case SECURITY_FS_USE_MNTPOINT:
		isec->sid = sbsec->mntpoint_sid;
		break;
	default:
		/* Default to the fs superblock SID. */
		isec->sid = sbsec->sid;

		if ((sbsec->flags & SE_SBGENFS) && !S_ISLNK(inode->i_mode)) {
			/* We must have a dentry to determine the label on
			 * procfs inodes */
			if (opt_dentry)
				/* Called from d_instantiate or
				 * d_splice_alias. */
				dentry = dget(opt_dentry);
			else
				/* Called from selinux_complete_init, try to
				 * find a dentry. */
				dentry = d_find_alias(inode);
=======
			goto out_invalid;
		}

		rc = inode_doinit_use_xattr(inode, dentry, sbsec->def_sid,
					    &sid);
		dput(dentry);
		if (rc)
			goto out;
		break;
	case SECURITY_FS_USE_TASK:
		sid = task_sid;
		break;
	case SECURITY_FS_USE_TRANS:
		/* Default to the fs SID. */
		sid = sbsec->sid;

		/* Try to obtain a transition SID. */
		rc = security_transition_sid(task_sid, sid,
					     sclass, NULL, &sid);
		if (rc)
			goto out;
		break;
	case SECURITY_FS_USE_MNTPOINT:
		sid = sbsec->mntpoint_sid;
		break;
	default:
		/* Default to the fs superblock SID. */
		sid = sbsec->sid;

		if ((sbsec->flags & SE_SBGENFS) &&
		     (!S_ISLNK(inode->i_mode) ||
		      selinux_policycap_genfs_seclabel_symlinks())) {
			/* We must have a dentry to determine the label on
			 * procfs inodes */
			if (opt_dentry) {
				/* Called from d_instantiate or
				 * d_splice_alias. */
				dentry = dget(opt_dentry);
			} else {
				/* Called from selinux_complete_init, try to
				 * find a dentry.  Some filesystems really want
				 * a connected one, so try that first.
				 */
				dentry = d_find_alias(inode);
				if (!dentry)
					dentry = d_find_any_alias(inode);
			}
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
			/*
			 * This can be hit on boot when a file is accessed
			 * before the policy is loaded.  When we load policy we
			 * may find inodes that have no dentry on the
			 * sbsec->isec_head list.  No reason to complain as
			 * these will get fixed up the next time we go through
			 * inode_doinit() with a dentry, before these inodes
			 * could be used again by userspace.
			 */
			if (!dentry)
<<<<<<< HEAD
				goto out_unlock;
			isec->sclass = inode_mode_to_security_class(inode->i_mode);
			rc = selinux_genfs_get_sid(dentry, isec->sclass,
						   sbsec->flags, &sid);
			dput(dentry);
			if (rc)
				goto out_unlock;
			isec->sid = sid;
=======
				goto out_invalid;
			rc = selinux_genfs_get_sid(dentry, sclass,
						   sbsec->flags, &sid);
			if (rc) {
				dput(dentry);
				goto out;
			}

			if ((sbsec->flags & SE_SBGENFS_XATTR) &&
			    (inode->i_opflags & IOP_XATTR)) {
				rc = inode_doinit_use_xattr(inode, dentry,
							    sid, &sid);
				if (rc) {
					dput(dentry);
					goto out;
				}
			}
			dput(dentry);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		}
		break;
	}

<<<<<<< HEAD
	isec->initialized = 1;

out_unlock:
	mutex_unlock(&isec->lock);
out:
	if (isec->sclass == SECCLASS_FILE)
		isec->sclass = inode_mode_to_security_class(inode->i_mode);
	return rc;
=======
out:
	spin_lock(&isec->lock);
	if (isec->initialized == LABEL_PENDING) {
		if (rc) {
			isec->initialized = LABEL_INVALID;
			goto out_unlock;
		}
		isec->initialized = LABEL_INITIALIZED;
		isec->sid = sid;
	}

out_unlock:
	spin_unlock(&isec->lock);
	return rc;

out_invalid:
	spin_lock(&isec->lock);
	if (isec->initialized == LABEL_PENDING) {
		isec->initialized = LABEL_INVALID;
		isec->sid = sid;
	}
	spin_unlock(&isec->lock);
	return 0;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

/* Convert a Linux signal to an access vector. */
static inline u32 signal_to_av(int sig)
{
	u32 perm = 0;

	switch (sig) {
	case SIGCHLD:
		/* Commonly granted from child to parent. */
		perm = PROCESS__SIGCHLD;
		break;
	case SIGKILL:
		/* Cannot be caught or ignored */
		perm = PROCESS__SIGKILL;
		break;
	case SIGSTOP:
		/* Cannot be caught or ignored */
		perm = PROCESS__SIGSTOP;
		break;
	default:
		/* All other signals. */
		perm = PROCESS__SIGNAL;
		break;
	}

	return perm;
}

<<<<<<< HEAD
/*
 * Check permission between a pair of credentials
 * fork check, ptrace check, etc.
 */
static int cred_has_perm(const struct cred *actor,
			 const struct cred *target,
			 u32 perms)
{
	u32 asid = cred_sid(actor), tsid = cred_sid(target);

	return avc_has_perm(asid, tsid, SECCLASS_PROCESS, perms, NULL);
}

/*
 * Check permission between a pair of tasks, e.g. signal checks,
 * fork check, ptrace check, etc.
 * tsk1 is the actor and tsk2 is the target
 * - this uses the default subjective creds of tsk1
 */
static int task_has_perm(const struct task_struct *tsk1,
			 const struct task_struct *tsk2,
			 u32 perms)
{
	const struct task_security_struct *__tsec1, *__tsec2;
	u32 sid1, sid2;

	rcu_read_lock();
	__tsec1 = __task_cred(tsk1)->security;	sid1 = __tsec1->sid;
	__tsec2 = __task_cred(tsk2)->security;	sid2 = __tsec2->sid;
	rcu_read_unlock();
	return avc_has_perm(sid1, sid2, SECCLASS_PROCESS, perms, NULL);
}

/*
 * Check permission between current and another task, e.g. signal checks,
 * fork check, ptrace check, etc.
 * current is the actor and tsk2 is the target
 * - this uses current's subjective creds
 */
static int current_has_perm(const struct task_struct *tsk,
			    u32 perms)
{
	u32 sid, tsid;

	sid = current_sid();
	tsid = task_sid(tsk);
	return avc_has_perm(sid, tsid, SECCLASS_PROCESS, perms, NULL);
}

=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#if CAP_LAST_CAP > 63
#error Fix SELinux to handle capabilities > 63.
#endif

/* Check whether a task is allowed to use a capability. */
static int cred_has_capability(const struct cred *cred,
<<<<<<< HEAD
			       int cap, int audit)
{
	struct common_audit_data ad;
	struct selinux_audit_data sad = {0,};
=======
			       int cap, unsigned int opts, bool initns)
{
	struct common_audit_data ad;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	struct av_decision avd;
	u16 sclass;
	u32 sid = cred_sid(cred);
	u32 av = CAP_TO_MASK(cap);
	int rc;

<<<<<<< HEAD
	COMMON_AUDIT_DATA_INIT(&ad, CAP);
	ad.selinux_audit_data = &sad;
	ad.tsk = current;
=======
	ad.type = LSM_AUDIT_DATA_CAP;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	ad.u.cap = cap;

	switch (CAP_TO_INDEX(cap)) {
	case 0:
<<<<<<< HEAD
		sclass = SECCLASS_CAPABILITY;
		break;
	case 1:
		sclass = SECCLASS_CAPABILITY2;
		break;
	default:
		printk(KERN_ERR
		       "SELinux:  out of range capability %d\n", cap);
=======
		sclass = initns ? SECCLASS_CAPABILITY : SECCLASS_CAP_USERNS;
		break;
	case 1:
		sclass = initns ? SECCLASS_CAPABILITY2 : SECCLASS_CAP2_USERNS;
		break;
	default:
		pr_err("SELinux:  out of range capability %d\n", cap);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		BUG();
		return -EINVAL;
	}

	rc = avc_has_perm_noaudit(sid, sid, sclass, av, 0, &avd);
<<<<<<< HEAD
	if (audit == SECURITY_CAP_AUDIT) {
		int rc2 = avc_audit(sid, sid, sclass, av, &avd, rc, &ad, 0);
=======
	if (!(opts & CAP_OPT_NOAUDIT)) {
		int rc2 = avc_audit(sid, sid, sclass, av, &avd, rc, &ad);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		if (rc2)
			return rc2;
	}
	return rc;
}

<<<<<<< HEAD
/* Check whether a task is allowed to use a system operation. */
static int task_has_system(struct task_struct *tsk,
			   u32 perms)
{
	u32 sid = task_sid(tsk);

	return avc_has_perm(sid, SECINITSID_KERNEL,
			    SECCLASS_SYSTEM, perms, NULL);
}

=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
/* Check whether a task has a particular permission to an inode.
   The 'adp' parameter is optional and allows other audit
   data to be passed (e.g. the dentry). */
static int inode_has_perm(const struct cred *cred,
			  struct inode *inode,
			  u32 perms,
<<<<<<< HEAD
			  struct common_audit_data *adp,
			  unsigned flags)
=======
			  struct common_audit_data *adp)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
{
	struct inode_security_struct *isec;
	u32 sid;

<<<<<<< HEAD
	validate_creds(cred);

=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	if (unlikely(IS_PRIVATE(inode)))
		return 0;

	sid = cred_sid(cred);
<<<<<<< HEAD
	isec = inode->i_security;

	return avc_has_perm_flags(sid, isec->sid, isec->sclass, perms, adp, flags);
}

static int inode_has_perm_noadp(const struct cred *cred,
				struct inode *inode,
				u32 perms,
				unsigned flags)
{
	struct common_audit_data ad;
	struct selinux_audit_data sad = {0,};

	COMMON_AUDIT_DATA_INIT(&ad, INODE);
	ad.u.inode = inode;
	ad.selinux_audit_data = &sad;
	return inode_has_perm(cred, inode, perms, &ad, flags);
=======
	isec = selinux_inode(inode);

	return avc_has_perm(sid, isec->sid, isec->sclass, perms, adp);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

/* Same as inode_has_perm, but pass explicit audit data containing
   the dentry to help the auditing code to more easily generate the
   pathname if needed. */
static inline int dentry_has_perm(const struct cred *cred,
				  struct dentry *dentry,
				  u32 av)
{
<<<<<<< HEAD
	struct inode *inode = dentry->d_inode;
	struct common_audit_data ad;
	struct selinux_audit_data sad = {0,};

	COMMON_AUDIT_DATA_INIT(&ad, DENTRY);
	ad.u.dentry = dentry;
	ad.selinux_audit_data = &sad;
	return inode_has_perm(cred, inode, av, &ad, 0);
=======
	struct inode *inode = d_backing_inode(dentry);
	struct common_audit_data ad;

	ad.type = LSM_AUDIT_DATA_DENTRY;
	ad.u.dentry = dentry;
	__inode_security_revalidate(inode, dentry, true);
	return inode_has_perm(cred, inode, av, &ad);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

/* Same as inode_has_perm, but pass explicit audit data containing
   the path to help the auditing code to more easily generate the
   pathname if needed. */
static inline int path_has_perm(const struct cred *cred,
<<<<<<< HEAD
				struct path *path,
				u32 av)
{
	struct inode *inode = path->dentry->d_inode;
	struct common_audit_data ad;
	struct selinux_audit_data sad = {0,};

	COMMON_AUDIT_DATA_INIT(&ad, PATH);
	ad.u.path = *path;
	ad.selinux_audit_data = &sad;
	return inode_has_perm(cred, inode, av, &ad, 0);
}

=======
				const struct path *path,
				u32 av)
{
	struct inode *inode = d_backing_inode(path->dentry);
	struct common_audit_data ad;

	ad.type = LSM_AUDIT_DATA_PATH;
	ad.u.path = *path;
	__inode_security_revalidate(inode, path->dentry, true);
	return inode_has_perm(cred, inode, av, &ad);
}

/* Same as path_has_perm, but uses the inode from the file struct. */
static inline int file_path_has_perm(const struct cred *cred,
				     struct file *file,
				     u32 av)
{
	struct common_audit_data ad;

	ad.type = LSM_AUDIT_DATA_FILE;
	ad.u.file = file;
	return inode_has_perm(cred, file_inode(file), av, &ad);
}

#ifdef CONFIG_BPF_SYSCALL
static int bpf_fd_pass(const struct file *file, u32 sid);
#endif

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
/* Check whether a task can use an open file descriptor to
   access an inode in a given way.  Check access to the
   descriptor itself, and then use dentry_has_perm to
   check a particular permission to the file.
   Access to the descriptor is implicitly granted if it
   has the same SID as the process.  If av is zero, then
   access to the file is not checked, e.g. for cases
   where only the descriptor is affected like seek. */
static int file_has_perm(const struct cred *cred,
			 struct file *file,
			 u32 av)
{
<<<<<<< HEAD
	struct file_security_struct *fsec = file->f_security;
	struct inode *inode = file->f_path.dentry->d_inode;
	struct common_audit_data ad;
	struct selinux_audit_data sad = {0,};
	u32 sid = cred_sid(cred);
	int rc;

	COMMON_AUDIT_DATA_INIT(&ad, PATH);
	ad.u.path = file->f_path;
	ad.selinux_audit_data = &sad;
=======
	struct file_security_struct *fsec = selinux_file(file);
	struct inode *inode = file_inode(file);
	struct common_audit_data ad;
	u32 sid = cred_sid(cred);
	int rc;

	ad.type = LSM_AUDIT_DATA_FILE;
	ad.u.file = file;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	if (sid != fsec->sid) {
		rc = avc_has_perm(sid, fsec->sid,
				  SECCLASS_FD,
				  FD__USE,
				  &ad);
		if (rc)
			goto out;
	}

<<<<<<< HEAD
	/* av is zero if only checking access to the descriptor. */
	rc = 0;
	if (av)
		rc = inode_has_perm(cred, inode, av, &ad, 0);
=======
#ifdef CONFIG_BPF_SYSCALL
	rc = bpf_fd_pass(file, cred_sid(cred));
	if (rc)
		return rc;
#endif

	/* av is zero if only checking access to the descriptor. */
	rc = 0;
	if (av)
		rc = inode_has_perm(cred, inode, av, &ad);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

out:
	return rc;
}

<<<<<<< HEAD
=======
/*
 * Determine the label for an inode that might be unioned.
 */
static int
selinux_determine_inode_label(const struct task_security_struct *tsec,
				 struct inode *dir,
				 const struct qstr *name, u16 tclass,
				 u32 *_new_isid)
{
	const struct superblock_security_struct *sbsec =
						selinux_superblock(dir->i_sb);

	if ((sbsec->flags & SE_SBINITIALIZED) &&
	    (sbsec->behavior == SECURITY_FS_USE_MNTPOINT)) {
		*_new_isid = sbsec->mntpoint_sid;
	} else if ((sbsec->flags & SBLABEL_MNT) &&
		   tsec->create_sid) {
		*_new_isid = tsec->create_sid;
	} else {
		const struct inode_security_struct *dsec = inode_security(dir);
		return security_transition_sid(tsec->sid,
					       dsec->sid, tclass,
					       name, _new_isid);
	}

	return 0;
}

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
/* Check whether a task can create a file. */
static int may_create(struct inode *dir,
		      struct dentry *dentry,
		      u16 tclass)
{
<<<<<<< HEAD
	const struct task_security_struct *tsec = current_security();
=======
	const struct task_security_struct *tsec = selinux_cred(current_cred());
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	struct inode_security_struct *dsec;
	struct superblock_security_struct *sbsec;
	u32 sid, newsid;
	struct common_audit_data ad;
<<<<<<< HEAD
	struct selinux_audit_data sad = {0,};
	int rc;

	dsec = dir->i_security;
	sbsec = dir->i_sb->s_security;

	sid = tsec->sid;
	newsid = tsec->create_sid;

	COMMON_AUDIT_DATA_INIT(&ad, DENTRY);
	ad.u.dentry = dentry;
	ad.selinux_audit_data = &sad;
=======
	int rc;

	dsec = inode_security(dir);
	sbsec = selinux_superblock(dir->i_sb);

	sid = tsec->sid;

	ad.type = LSM_AUDIT_DATA_DENTRY;
	ad.u.dentry = dentry;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	rc = avc_has_perm(sid, dsec->sid, SECCLASS_DIR,
			  DIR__ADD_NAME | DIR__SEARCH,
			  &ad);
	if (rc)
		return rc;

<<<<<<< HEAD
	if (!newsid || !(sbsec->flags & SE_SBLABELSUPP)) {
		rc = security_transition_sid(sid, dsec->sid, tclass,
					     &dentry->d_name, &newsid);
		if (rc)
			return rc;
	}
=======
	rc = selinux_determine_inode_label(tsec, dir, &dentry->d_name, tclass,
					   &newsid);
	if (rc)
		return rc;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	rc = avc_has_perm(sid, newsid, tclass, FILE__CREATE, &ad);
	if (rc)
		return rc;

<<<<<<< HEAD
	rc = avc_has_perm(newsid, sbsec->sid,
			  SECCLASS_FILESYSTEM,
			  FILESYSTEM__ASSOCIATE, &ad);
	if (rc)
		return rc;

	rc = pft_inode_mknod(dir, dentry, 0, 0);

	return rc;
}

/* Check whether a task can create a key. */
static int may_create_key(u32 ksid,
			  struct task_struct *ctx)
{
	u32 sid = task_sid(ctx);

	return avc_has_perm(sid, ksid, SECCLASS_KEY, KEY__CREATE, NULL);
=======
	return avc_has_perm(newsid, sbsec->sid,
			    SECCLASS_FILESYSTEM,
			    FILESYSTEM__ASSOCIATE, &ad);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

#define MAY_LINK	0
#define MAY_UNLINK	1
#define MAY_RMDIR	2

/* Check whether a task can link, unlink, or rmdir a file/directory. */
static int may_link(struct inode *dir,
		    struct dentry *dentry,
		    int kind)

{
	struct inode_security_struct *dsec, *isec;
	struct common_audit_data ad;
<<<<<<< HEAD
	struct selinux_audit_data sad = {0,};
=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	u32 sid = current_sid();
	u32 av;
	int rc;

<<<<<<< HEAD
	dsec = dir->i_security;
	isec = dentry->d_inode->i_security;

	COMMON_AUDIT_DATA_INIT(&ad, DENTRY);
	ad.u.dentry = dentry;
	ad.selinux_audit_data = &sad;
=======
	dsec = inode_security(dir);
	isec = backing_inode_security(dentry);

	ad.type = LSM_AUDIT_DATA_DENTRY;
	ad.u.dentry = dentry;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	av = DIR__SEARCH;
	av |= (kind ? DIR__REMOVE_NAME : DIR__ADD_NAME);
	rc = avc_has_perm(sid, dsec->sid, SECCLASS_DIR, av, &ad);
	if (rc)
		return rc;

	switch (kind) {
	case MAY_LINK:
		av = FILE__LINK;
		break;
	case MAY_UNLINK:
		av = FILE__UNLINK;
		break;
	case MAY_RMDIR:
		av = DIR__RMDIR;
		break;
	default:
<<<<<<< HEAD
		printk(KERN_WARNING "SELinux: %s:  unrecognized kind %d\n",
=======
		pr_warn("SELinux: %s:  unrecognized kind %d\n",
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
			__func__, kind);
		return 0;
	}

	rc = avc_has_perm(sid, isec->sid, isec->sclass, av, &ad);
<<<<<<< HEAD
	if (rc)
		return rc;

	if (kind == MAY_UNLINK)
		rc = pft_inode_unlink(dir, dentry);

=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	return rc;
}

static inline int may_rename(struct inode *old_dir,
			     struct dentry *old_dentry,
			     struct inode *new_dir,
			     struct dentry *new_dentry)
{
	struct inode_security_struct *old_dsec, *new_dsec, *old_isec, *new_isec;
	struct common_audit_data ad;
<<<<<<< HEAD
	struct selinux_audit_data sad = {0,};
=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	u32 sid = current_sid();
	u32 av;
	int old_is_dir, new_is_dir;
	int rc;

<<<<<<< HEAD
	old_dsec = old_dir->i_security;
	old_isec = old_dentry->d_inode->i_security;
	old_is_dir = S_ISDIR(old_dentry->d_inode->i_mode);
	new_dsec = new_dir->i_security;

	COMMON_AUDIT_DATA_INIT(&ad, DENTRY);
	ad.selinux_audit_data = &sad;
=======
	old_dsec = inode_security(old_dir);
	old_isec = backing_inode_security(old_dentry);
	old_is_dir = d_is_dir(old_dentry);
	new_dsec = inode_security(new_dir);

	ad.type = LSM_AUDIT_DATA_DENTRY;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	ad.u.dentry = old_dentry;
	rc = avc_has_perm(sid, old_dsec->sid, SECCLASS_DIR,
			  DIR__REMOVE_NAME | DIR__SEARCH, &ad);
	if (rc)
		return rc;
	rc = avc_has_perm(sid, old_isec->sid,
			  old_isec->sclass, FILE__RENAME, &ad);
	if (rc)
		return rc;
	if (old_is_dir && new_dir != old_dir) {
		rc = avc_has_perm(sid, old_isec->sid,
				  old_isec->sclass, DIR__REPARENT, &ad);
		if (rc)
			return rc;
	}

	ad.u.dentry = new_dentry;
	av = DIR__ADD_NAME | DIR__SEARCH;
<<<<<<< HEAD
	if (new_dentry->d_inode)
=======
	if (d_is_positive(new_dentry))
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		av |= DIR__REMOVE_NAME;
	rc = avc_has_perm(sid, new_dsec->sid, SECCLASS_DIR, av, &ad);
	if (rc)
		return rc;
<<<<<<< HEAD
	if (new_dentry->d_inode) {
		new_isec = new_dentry->d_inode->i_security;
		new_is_dir = S_ISDIR(new_dentry->d_inode->i_mode);
=======
	if (d_is_positive(new_dentry)) {
		new_isec = backing_inode_security(new_dentry);
		new_is_dir = d_is_dir(new_dentry);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		rc = avc_has_perm(sid, new_isec->sid,
				  new_isec->sclass,
				  (new_is_dir ? DIR__RMDIR : FILE__UNLINK), &ad);
		if (rc)
			return rc;
	}

	return 0;
}

/* Check whether a task can perform a filesystem operation. */
static int superblock_has_perm(const struct cred *cred,
<<<<<<< HEAD
			       struct super_block *sb,
=======
			       const struct super_block *sb,
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
			       u32 perms,
			       struct common_audit_data *ad)
{
	struct superblock_security_struct *sbsec;
	u32 sid = cred_sid(cred);

<<<<<<< HEAD
	sbsec = sb->s_security;
=======
	sbsec = selinux_superblock(sb);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	return avc_has_perm(sid, sbsec->sid, SECCLASS_FILESYSTEM, perms, ad);
}

/* Convert a Linux mode and permission mask to an access vector. */
static inline u32 file_mask_to_av(int mode, int mask)
{
	u32 av = 0;

	if (!S_ISDIR(mode)) {
		if (mask & MAY_EXEC)
			av |= FILE__EXECUTE;
		if (mask & MAY_READ)
			av |= FILE__READ;

		if (mask & MAY_APPEND)
			av |= FILE__APPEND;
		else if (mask & MAY_WRITE)
			av |= FILE__WRITE;

	} else {
		if (mask & MAY_EXEC)
			av |= DIR__SEARCH;
		if (mask & MAY_WRITE)
			av |= DIR__WRITE;
		if (mask & MAY_READ)
			av |= DIR__READ;
	}

	return av;
}

/* Convert a Linux file to an access vector. */
<<<<<<< HEAD
static inline u32 file_to_av(struct file *file)
=======
static inline u32 file_to_av(const struct file *file)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
{
	u32 av = 0;

	if (file->f_mode & FMODE_READ)
		av |= FILE__READ;
	if (file->f_mode & FMODE_WRITE) {
		if (file->f_flags & O_APPEND)
			av |= FILE__APPEND;
		else
			av |= FILE__WRITE;
	}
	if (!av) {
		/*
		 * Special file opened with flags 3 for ioctl-only use.
		 */
		av = FILE__IOCTL;
	}

	return av;
}

/*
<<<<<<< HEAD
 * Convert a file to an access vector and include the correct open
=======
 * Convert a file to an access vector and include the correct
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 * open permission.
 */
static inline u32 open_file_to_av(struct file *file)
{
	u32 av = file_to_av(file);
<<<<<<< HEAD

	if (selinux_policycap_openperm)
=======
	struct inode *inode = file_inode(file);

	if (selinux_policycap_openperm() &&
	    inode->i_sb->s_magic != SOCKFS_MAGIC)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		av |= FILE__OPEN;

	return av;
}

/* Hook functions begin here. */

<<<<<<< HEAD
static int selinux_binder_set_context_mgr(struct task_struct *mgr)
{
	u32 mysid = current_sid();
	u32 mgrsid = task_sid(mgr);

	return avc_has_perm(mysid, mgrsid, SECCLASS_BINDER, BINDER__SET_CONTEXT_MGR, NULL);
}

static int selinux_binder_transaction(struct task_struct *from, struct task_struct *to)
{
	u32 mysid = current_sid();
	u32 fromsid = task_sid(from);
	u32 tosid = task_sid(to);
	int rc;

	if (mysid != fromsid) {
		rc = avc_has_perm(mysid, fromsid, SECCLASS_BINDER, BINDER__IMPERSONATE, NULL);
=======
static int selinux_binder_set_context_mgr(const struct cred *mgr)
{
	return avc_has_perm(current_sid(), cred_sid(mgr), SECCLASS_BINDER,
			    BINDER__SET_CONTEXT_MGR, NULL);
}

static int selinux_binder_transaction(const struct cred *from,
				      const struct cred *to)
{
	u32 mysid = current_sid();
	u32 fromsid = cred_sid(from);
	u32 tosid = cred_sid(to);
	int rc;

	if (mysid != fromsid) {
		rc = avc_has_perm(mysid, fromsid, SECCLASS_BINDER,
				  BINDER__IMPERSONATE, NULL);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		if (rc)
			return rc;
	}

<<<<<<< HEAD
	return avc_has_perm(fromsid, tosid, SECCLASS_BINDER, BINDER__CALL, NULL);
}

static int selinux_binder_transfer_binder(struct task_struct *from, struct task_struct *to)
{
	u32 fromsid = task_sid(from);
	u32 tosid = task_sid(to);
	return avc_has_perm(fromsid, tosid, SECCLASS_BINDER, BINDER__TRANSFER, NULL);
}

static int selinux_binder_transfer_file(struct task_struct *from, struct task_struct *to, struct file *file)
{
	u32 sid = task_sid(to);
	struct file_security_struct *fsec = file->f_security;
	struct inode *inode = file->f_path.dentry->d_inode;
	struct inode_security_struct *isec = inode->i_security;
	struct common_audit_data ad;
	struct selinux_audit_data sad = {0,};
	int rc;

	COMMON_AUDIT_DATA_INIT(&ad, PATH);
	ad.u.path = file->f_path;
	ad.selinux_audit_data = &sad;
=======
	return avc_has_perm(fromsid, tosid,
			    SECCLASS_BINDER, BINDER__CALL, NULL);
}

static int selinux_binder_transfer_binder(const struct cred *from,
					  const struct cred *to)
{
	return avc_has_perm(cred_sid(from), cred_sid(to),
			    SECCLASS_BINDER, BINDER__TRANSFER,
			    NULL);
}

static int selinux_binder_transfer_file(const struct cred *from,
					const struct cred *to,
					const struct file *file)
{
	u32 sid = cred_sid(to);
	struct file_security_struct *fsec = selinux_file(file);
	struct dentry *dentry = file->f_path.dentry;
	struct inode_security_struct *isec;
	struct common_audit_data ad;
	int rc;

	ad.type = LSM_AUDIT_DATA_PATH;
	ad.u.path = file->f_path;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	if (sid != fsec->sid) {
		rc = avc_has_perm(sid, fsec->sid,
				  SECCLASS_FD,
				  FD__USE,
				  &ad);
		if (rc)
			return rc;
	}

<<<<<<< HEAD
	if (unlikely(IS_PRIVATE(inode)))
		return 0;

=======
#ifdef CONFIG_BPF_SYSCALL
	rc = bpf_fd_pass(file, sid);
	if (rc)
		return rc;
#endif

	if (unlikely(IS_PRIVATE(d_backing_inode(dentry))))
		return 0;

	isec = backing_inode_security(dentry);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	return avc_has_perm(sid, isec->sid, isec->sclass, file_to_av(file),
			    &ad);
}

static int selinux_ptrace_access_check(struct task_struct *child,
<<<<<<< HEAD
				     unsigned int mode)
{
	int rc;

	rc = cap_ptrace_access_check(child, mode);
	if (rc)
		return rc;

	if (mode & PTRACE_MODE_READ) {
		u32 sid = current_sid();
		u32 csid = task_sid(child);
		return avc_has_perm(sid, csid, SECCLASS_FILE, FILE__READ, NULL);
	}

	return current_has_perm(child, PROCESS__PTRACE);
=======
				       unsigned int mode)
{
	u32 sid = current_sid();
	u32 csid = task_sid_obj(child);

	if (mode & PTRACE_MODE_READ)
		return avc_has_perm(sid, csid, SECCLASS_FILE, FILE__READ,
				NULL);

	return avc_has_perm(sid, csid, SECCLASS_PROCESS, PROCESS__PTRACE,
			NULL);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

static int selinux_ptrace_traceme(struct task_struct *parent)
{
<<<<<<< HEAD
	int rc;

	rc = cap_ptrace_traceme(parent);
	if (rc)
		return rc;

	return task_has_perm(parent, current, PROCESS__PTRACE);
}

static int selinux_capget(struct task_struct *target, kernel_cap_t *effective,
			  kernel_cap_t *inheritable, kernel_cap_t *permitted)
{
	int error;

	error = current_has_perm(target, PROCESS__GETCAP);
	if (error)
		return error;

	return cap_capget(target, effective, inheritable, permitted);
=======
	return avc_has_perm(task_sid_obj(parent), task_sid_obj(current),
			    SECCLASS_PROCESS, PROCESS__PTRACE, NULL);
}

static int selinux_capget(const struct task_struct *target, kernel_cap_t *effective,
			  kernel_cap_t *inheritable, kernel_cap_t *permitted)
{
	return avc_has_perm(current_sid(), task_sid_obj(target),
			SECCLASS_PROCESS, PROCESS__GETCAP, NULL);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

static int selinux_capset(struct cred *new, const struct cred *old,
			  const kernel_cap_t *effective,
			  const kernel_cap_t *inheritable,
			  const kernel_cap_t *permitted)
{
<<<<<<< HEAD
	int error;

	error = cap_capset(new, old,
				      effective, inheritable, permitted);
	if (error)
		return error;

	return cred_has_perm(old, new, PROCESS__SETCAP);
=======
	return avc_has_perm(cred_sid(old), cred_sid(new), SECCLASS_PROCESS,
			    PROCESS__SETCAP, NULL);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

/*
 * (This comment used to live with the selinux_task_setuid hook,
 * which was removed).
 *
 * Since setuid only affects the current process, and since the SELinux
 * controls are not based on the Linux identity attributes, SELinux does not
 * need to control this operation.  However, SELinux does control the use of
 * the CAP_SETUID and CAP_SETGID capabilities using the capable hook.
 */

static int selinux_capable(const struct cred *cred, struct user_namespace *ns,
<<<<<<< HEAD
			   int cap, int audit)
{
	int rc;

	rc = cap_capable(cred, ns, cap, audit);
	if (rc)
		return rc;

	return cred_has_capability(cred, cap, audit);
}

static int selinux_quotactl(int cmds, int type, int id, struct super_block *sb)
=======
			   int cap, unsigned int opts)
{
	return cred_has_capability(cred, cap, opts, ns == &init_user_ns);
}

static int selinux_quotactl(int cmds, int type, int id, const struct super_block *sb)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
{
	const struct cred *cred = current_cred();
	int rc = 0;

	if (!sb)
		return 0;

	switch (cmds) {
	case Q_SYNC:
	case Q_QUOTAON:
	case Q_QUOTAOFF:
	case Q_SETINFO:
	case Q_SETQUOTA:
<<<<<<< HEAD
=======
	case Q_XQUOTAOFF:
	case Q_XQUOTAON:
	case Q_XSETQLIM:
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		rc = superblock_has_perm(cred, sb, FILESYSTEM__QUOTAMOD, NULL);
		break;
	case Q_GETFMT:
	case Q_GETINFO:
	case Q_GETQUOTA:
<<<<<<< HEAD
=======
	case Q_XGETQUOTA:
	case Q_XGETQSTAT:
	case Q_XGETQSTATV:
	case Q_XGETNEXTQUOTA:
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		rc = superblock_has_perm(cred, sb, FILESYSTEM__QUOTAGET, NULL);
		break;
	default:
		rc = 0;  /* let the kernel handle invalid cmds */
		break;
	}
	return rc;
}

static int selinux_quota_on(struct dentry *dentry)
{
	const struct cred *cred = current_cred();

	return dentry_has_perm(cred, dentry, FILE__QUOTAON);
}

static int selinux_syslog(int type)
{
<<<<<<< HEAD
	int rc;

	switch (type) {
	case SYSLOG_ACTION_READ_ALL:	/* Read last kernel messages */
	case SYSLOG_ACTION_SIZE_BUFFER:	/* Return size of the log buffer */
		rc = task_has_system(current, SYSTEM__SYSLOG_READ);
		break;
=======
	switch (type) {
	case SYSLOG_ACTION_READ_ALL:	/* Read last kernel messages */
	case SYSLOG_ACTION_SIZE_BUFFER:	/* Return size of the log buffer */
		return avc_has_perm(current_sid(), SECINITSID_KERNEL,
				    SECCLASS_SYSTEM, SYSTEM__SYSLOG_READ, NULL);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	case SYSLOG_ACTION_CONSOLE_OFF:	/* Disable logging to console */
	case SYSLOG_ACTION_CONSOLE_ON:	/* Enable logging to console */
	/* Set level of messages printed to console */
	case SYSLOG_ACTION_CONSOLE_LEVEL:
<<<<<<< HEAD
		rc = task_has_system(current, SYSTEM__SYSLOG_CONSOLE);
		break;
	case SYSLOG_ACTION_CLOSE:	/* Close log */
	case SYSLOG_ACTION_OPEN:	/* Open log */
	case SYSLOG_ACTION_READ:	/* Read from log */
	case SYSLOG_ACTION_READ_CLEAR:	/* Read/clear last kernel messages */
	case SYSLOG_ACTION_CLEAR:	/* Clear ring buffer */
	default:
		rc = task_has_system(current, SYSTEM__SYSLOG_MOD);
		break;
	}
	return rc;
=======
		return avc_has_perm(current_sid(), SECINITSID_KERNEL,
				    SECCLASS_SYSTEM, SYSTEM__SYSLOG_CONSOLE,
				    NULL);
	}
	/* All other syslog types */
	return avc_has_perm(current_sid(), SECINITSID_KERNEL,
			    SECCLASS_SYSTEM, SYSTEM__SYSLOG_MOD, NULL);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

/*
 * Check that a process has enough memory to allocate a new virtual
 * mapping. 0 means there is enough memory for the allocation to
 * succeed and -ENOMEM implies there is not.
 *
 * Do not audit the selinux permission check, as this is applied to all
 * processes that allocate mappings.
 */
static int selinux_vm_enough_memory(struct mm_struct *mm, long pages)
{
	int rc, cap_sys_admin = 0;

<<<<<<< HEAD
	rc = selinux_capable(current_cred(), &init_user_ns, CAP_SYS_ADMIN,
			     SECURITY_CAP_NOAUDIT);
	if (rc == 0)
		cap_sys_admin = 1;

	return __vm_enough_memory(mm, pages, cap_sys_admin);
=======
	rc = cred_has_capability(current_cred(), CAP_SYS_ADMIN,
				 CAP_OPT_NOAUDIT, true);
	if (rc == 0)
		cap_sys_admin = 1;

	return cap_sys_admin;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

/* binprm security operations */

<<<<<<< HEAD
static int selinux_bprm_set_creds(struct linux_binprm *bprm)
=======
static u32 ptrace_parent_sid(void)
{
	u32 sid = 0;
	struct task_struct *tracer;

	rcu_read_lock();
	tracer = ptrace_parent(current);
	if (tracer)
		sid = task_sid_obj(tracer);
	rcu_read_unlock();

	return sid;
}

static int check_nnp_nosuid(const struct linux_binprm *bprm,
			    const struct task_security_struct *old_tsec,
			    const struct task_security_struct *new_tsec)
{
	int nnp = (bprm->unsafe & LSM_UNSAFE_NO_NEW_PRIVS);
	int nosuid = !mnt_may_suid(bprm->file->f_path.mnt);
	int rc;
	u32 av;

	if (!nnp && !nosuid)
		return 0; /* neither NNP nor nosuid */

	if (new_tsec->sid == old_tsec->sid)
		return 0; /* No change in credentials */

	/*
	 * If the policy enables the nnp_nosuid_transition policy capability,
	 * then we permit transitions under NNP or nosuid if the
	 * policy allows the corresponding permission between
	 * the old and new contexts.
	 */
	if (selinux_policycap_nnp_nosuid_transition()) {
		av = 0;
		if (nnp)
			av |= PROCESS2__NNP_TRANSITION;
		if (nosuid)
			av |= PROCESS2__NOSUID_TRANSITION;
		rc = avc_has_perm(old_tsec->sid, new_tsec->sid,
				  SECCLASS_PROCESS2, av, NULL);
		if (!rc)
			return 0;
	}

	/*
	 * We also permit NNP or nosuid transitions to bounded SIDs,
	 * i.e. SIDs that are guaranteed to only be allowed a subset
	 * of the permissions of the current SID.
	 */
	rc = security_bounded_transition(old_tsec->sid,
					 new_tsec->sid);
	if (!rc)
		return 0;

	/*
	 * On failure, preserve the errno values for NNP vs nosuid.
	 * NNP:  Operation not permitted for caller.
	 * nosuid:  Permission denied to file.
	 */
	if (nnp)
		return -EPERM;
	return -EACCES;
}

static int selinux_bprm_creds_for_exec(struct linux_binprm *bprm)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
{
	const struct task_security_struct *old_tsec;
	struct task_security_struct *new_tsec;
	struct inode_security_struct *isec;
	struct common_audit_data ad;
<<<<<<< HEAD
	struct selinux_audit_data sad = {0,};
	struct inode *inode = bprm->file->f_path.dentry->d_inode;
	int rc;

	rc = cap_bprm_set_creds(bprm);
	if (rc)
		return rc;

	/* SELinux context only depends on initial program or script and not
	 * the script interpreter */
	if (bprm->cred_prepared)
		return 0;

	old_tsec = current_security();
	new_tsec = bprm->cred->security;
	isec = inode->i_security;
=======
	struct inode *inode = file_inode(bprm->file);
	int rc;

	/* SELinux context only depends on initial program or script and not
	 * the script interpreter */

	old_tsec = selinux_cred(current_cred());
	new_tsec = selinux_cred(bprm->cred);
	isec = inode_security(inode);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	/* Default to the current task SID. */
	new_tsec->sid = old_tsec->sid;
	new_tsec->osid = old_tsec->sid;

	/* Reset fs, key, and sock SIDs on execve. */
	new_tsec->create_sid = 0;
	new_tsec->keycreate_sid = 0;
	new_tsec->sockcreate_sid = 0;

<<<<<<< HEAD
=======
	/*
	 * Before policy is loaded, label any task outside kernel space
	 * as SECINITSID_INIT, so that any userspace tasks surviving from
	 * early boot end up with a label different from SECINITSID_KERNEL
	 * (if the policy chooses to set SECINITSID_INIT != SECINITSID_KERNEL).
	 */
	if (!selinux_initialized()) {
		new_tsec->sid = SECINITSID_INIT;
		/* also clear the exec_sid just in case */
		new_tsec->exec_sid = 0;
		return 0;
	}

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	if (old_tsec->exec_sid) {
		new_tsec->sid = old_tsec->exec_sid;
		/* Reset exec SID on execve. */
		new_tsec->exec_sid = 0;

<<<<<<< HEAD
		/*
		 * Minimize confusion: if no_new_privs and a transition is
		 * explicitly requested, then fail the exec.
		 */
		if (bprm->unsafe & LSM_UNSAFE_NO_NEW_PRIVS)
			return -EPERM;
	} else {
		/* Check for a default transition on this program. */
		rc = security_transition_sid(old_tsec->sid, isec->sid,
					     SECCLASS_PROCESS, NULL,
					     &new_tsec->sid);
		if (rc)
			return rc;
	}

	COMMON_AUDIT_DATA_INIT(&ad, PATH);
	ad.selinux_audit_data = &sad;
	ad.u.path = bprm->file->f_path;

	if ((bprm->file->f_path.mnt->mnt_flags & MNT_NOSUID) ||
	    (bprm->unsafe & LSM_UNSAFE_NO_NEW_PRIVS))
		new_tsec->sid = old_tsec->sid;
=======
		/* Fail on NNP or nosuid if not an allowed transition. */
		rc = check_nnp_nosuid(bprm, old_tsec, new_tsec);
		if (rc)
			return rc;
	} else {
		/* Check for a default transition on this program. */
		rc = security_transition_sid(old_tsec->sid,
					     isec->sid, SECCLASS_PROCESS, NULL,
					     &new_tsec->sid);
		if (rc)
			return rc;

		/*
		 * Fallback to old SID on NNP or nosuid if not an allowed
		 * transition.
		 */
		rc = check_nnp_nosuid(bprm, old_tsec, new_tsec);
		if (rc)
			new_tsec->sid = old_tsec->sid;
	}

	ad.type = LSM_AUDIT_DATA_FILE;
	ad.u.file = bprm->file;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	if (new_tsec->sid == old_tsec->sid) {
		rc = avc_has_perm(old_tsec->sid, isec->sid,
				  SECCLASS_FILE, FILE__EXECUTE_NO_TRANS, &ad);
		if (rc)
			return rc;
	} else {
		/* Check permissions for the transition. */
		rc = avc_has_perm(old_tsec->sid, new_tsec->sid,
				  SECCLASS_PROCESS, PROCESS__TRANSITION, &ad);
		if (rc)
			return rc;

		rc = avc_has_perm(new_tsec->sid, isec->sid,
				  SECCLASS_FILE, FILE__ENTRYPOINT, &ad);
		if (rc)
			return rc;

		/* Check for shared state */
		if (bprm->unsafe & LSM_UNSAFE_SHARE) {
			rc = avc_has_perm(old_tsec->sid, new_tsec->sid,
					  SECCLASS_PROCESS, PROCESS__SHARE,
					  NULL);
			if (rc)
				return -EPERM;
		}

		/* Make sure that anyone attempting to ptrace over a task that
		 * changes its SID has the appropriate permit */
<<<<<<< HEAD
		if (bprm->unsafe &
		    (LSM_UNSAFE_PTRACE | LSM_UNSAFE_PTRACE_CAP)) {
			struct task_struct *tracer;
			struct task_security_struct *sec;
			u32 ptsid = 0;

			rcu_read_lock();
			tracer = ptrace_parent(current);
			if (likely(tracer != NULL)) {
				sec = __task_cred(tracer)->security;
				ptsid = sec->sid;
			}
			rcu_read_unlock();

=======
		if (bprm->unsafe & LSM_UNSAFE_PTRACE) {
			u32 ptsid = ptrace_parent_sid();
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
			if (ptsid != 0) {
				rc = avc_has_perm(ptsid, new_tsec->sid,
						  SECCLASS_PROCESS,
						  PROCESS__PTRACE, NULL);
				if (rc)
					return -EPERM;
			}
		}

		/* Clear any possibly unsafe personality bits on exec: */
		bprm->per_clear |= PER_CLEAR_ON_SETID;
<<<<<<< HEAD
=======

		/* Enable secure mode for SIDs transitions unless
		   the noatsecure permission is granted between
		   the two SIDs, i.e. ahp returns 0. */
		rc = avc_has_perm(old_tsec->sid, new_tsec->sid,
				  SECCLASS_PROCESS, PROCESS__NOATSECURE,
				  NULL);
		bprm->secureexec |= !!rc;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	}

	return 0;
}

<<<<<<< HEAD
static int selinux_bprm_secureexec(struct linux_binprm *bprm)
{
	const struct task_security_struct *tsec = current_security();
	u32 sid, osid;
	int atsecure = 0;

	sid = tsec->sid;
	osid = tsec->osid;

	if (osid != sid) {
		/* Enable secure mode for SIDs transitions unless
		   the noatsecure permission is granted between
		   the two SIDs, i.e. ahp returns 0. */
		atsecure = avc_has_perm(osid, sid,
					SECCLASS_PROCESS,
					PROCESS__NOATSECURE, NULL);
	}

	return (atsecure || cap_bprm_secureexec(bprm));
=======
static int match_file(const void *p, struct file *file, unsigned fd)
{
	return file_has_perm(p, file, file_to_av(file)) ? fd + 1 : 0;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

/* Derived from fs/exec.c:flush_old_files. */
static inline void flush_unauthorized_files(const struct cred *cred,
					    struct files_struct *files)
{
<<<<<<< HEAD
	struct common_audit_data ad;
	struct selinux_audit_data sad = {0,};
	struct file *file, *devnull = NULL;
	struct tty_struct *tty;
	struct fdtable *fdt;
	long j = -1;
	int drop_tty = 0;

	tty = get_current_tty();
	if (tty) {
		spin_lock(&tty_files_lock);
		if (!list_empty(&tty->tty_files)) {
			struct tty_file_private *file_priv;
			struct inode *inode;

			/* Revalidate access to controlling tty.
			   Use inode_has_perm on the tty inode directly rather
			   than using file_has_perm, as this particular open
			   file may belong to another process and we are only
			   interested in the inode-based check here. */
			file_priv = list_first_entry(&tty->tty_files,
						struct tty_file_private, list);
			file = file_priv->file;
			inode = file->f_path.dentry->d_inode;
			if (inode_has_perm_noadp(cred, inode,
					   FILE__READ | FILE__WRITE, 0)) {
				drop_tty = 1;
			}
		}
		spin_unlock(&tty_files_lock);
=======
	struct file *file, *devnull = NULL;
	struct tty_struct *tty;
	int drop_tty = 0;
	unsigned n;

	tty = get_current_tty();
	if (tty) {
		spin_lock(&tty->files_lock);
		if (!list_empty(&tty->tty_files)) {
			struct tty_file_private *file_priv;

			/* Revalidate access to controlling tty.
			   Use file_path_has_perm on the tty path directly
			   rather than using file_has_perm, as this particular
			   open file may belong to another process and we are
			   only interested in the inode-based check here. */
			file_priv = list_first_entry(&tty->tty_files,
						struct tty_file_private, list);
			file = file_priv->file;
			if (file_path_has_perm(cred, file, FILE__READ | FILE__WRITE))
				drop_tty = 1;
		}
		spin_unlock(&tty->files_lock);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		tty_kref_put(tty);
	}
	/* Reset controlling tty. */
	if (drop_tty)
		no_tty();

	/* Revalidate access to inherited open files. */
<<<<<<< HEAD

	COMMON_AUDIT_DATA_INIT(&ad, INODE);
	ad.selinux_audit_data = &sad;

	spin_lock(&files->file_lock);
	for (;;) {
		unsigned long set, i;
		int fd;

		j++;
		i = j * BITS_PER_LONG;
		fdt = files_fdtable(files);
		if (i >= fdt->max_fds)
			break;
		set = fdt->open_fds[j];
		if (!set)
			continue;
		spin_unlock(&files->file_lock);
		for ( ; set ; i++, set >>= 1) {
			if (set & 1) {
				file = fget(i);
				if (!file)
					continue;
				if (file_has_perm(cred,
						  file,
						  file_to_av(file))) {
					sys_close(i);
					fd = get_unused_fd();
					if (fd != i) {
						if (fd >= 0)
							put_unused_fd(fd);
						fput(file);
						continue;
					}
					if (devnull) {
						get_file(devnull);
					} else {
						devnull = dentry_open(
							dget(selinux_null),
							mntget(selinuxfs_mount),
							O_RDWR, cred);
						if (IS_ERR(devnull)) {
							devnull = NULL;
							put_unused_fd(fd);
							fput(file);
							continue;
						}
					}
					fd_install(fd, devnull);
				}
				fput(file);
			}
		}
		spin_lock(&files->file_lock);

	}
	spin_unlock(&files->file_lock);
=======
	n = iterate_fd(files, 0, match_file, cred);
	if (!n) /* none found? */
		return;

	devnull = dentry_open(&selinux_null, O_RDWR, cred);
	if (IS_ERR(devnull))
		devnull = NULL;
	/* replace all the matching ones with this */
	do {
		replace_fd(n - 1, devnull, 0);
	} while ((n = iterate_fd(files, n, match_file, cred)) != 0);
	if (devnull)
		fput(devnull);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

/*
 * Prepare a process for imminent new credential changes due to exec
 */
<<<<<<< HEAD
static void selinux_bprm_committing_creds(struct linux_binprm *bprm)
=======
static void selinux_bprm_committing_creds(const struct linux_binprm *bprm)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
{
	struct task_security_struct *new_tsec;
	struct rlimit *rlim, *initrlim;
	int rc, i;

<<<<<<< HEAD
	new_tsec = bprm->cred->security;
=======
	new_tsec = selinux_cred(bprm->cred);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	if (new_tsec->sid == new_tsec->osid)
		return;

	/* Close files for which the new task SID is not authorized. */
	flush_unauthorized_files(bprm->cred, current->files);

	/* Always clear parent death signal on SID transitions. */
	current->pdeath_signal = 0;

	/* Check whether the new SID can inherit resource limits from the old
	 * SID.  If not, reset all soft limits to the lower of the current
	 * task's hard limit and the init task's soft limit.
	 *
	 * Note that the setting of hard limits (even to lower them) can be
	 * controlled by the setrlimit check.  The inclusion of the init task's
	 * soft limit into the computation is to avoid resetting soft limits
	 * higher than the default soft limit for cases where the default is
	 * lower than the hard limit, e.g. RLIMIT_CORE or RLIMIT_STACK.
	 */
	rc = avc_has_perm(new_tsec->osid, new_tsec->sid, SECCLASS_PROCESS,
			  PROCESS__RLIMITINH, NULL);
	if (rc) {
		/* protect against do_prlimit() */
		task_lock(current);
		for (i = 0; i < RLIM_NLIMITS; i++) {
			rlim = current->signal->rlim + i;
			initrlim = init_task.signal->rlim + i;
			rlim->rlim_cur = min(rlim->rlim_max, initrlim->rlim_cur);
		}
		task_unlock(current);
<<<<<<< HEAD
		update_rlimit_cpu(current, rlimit(RLIMIT_CPU));
=======
		if (IS_ENABLED(CONFIG_POSIX_TIMERS))
			update_rlimit_cpu(current, rlimit(RLIMIT_CPU));
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	}
}

/*
 * Clean up the process immediately after the installation of new credentials
 * due to exec
 */
<<<<<<< HEAD
static void selinux_bprm_committed_creds(struct linux_binprm *bprm)
{
	const struct task_security_struct *tsec = current_security();
	struct itimerval itimer;
	u32 osid, sid;
	int rc, i;
=======
static void selinux_bprm_committed_creds(const struct linux_binprm *bprm)
{
	const struct task_security_struct *tsec = selinux_cred(current_cred());
	u32 osid, sid;
	int rc;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	osid = tsec->osid;
	sid = tsec->sid;

	if (sid == osid)
		return;

	/* Check whether the new SID can inherit signal state from the old SID.
	 * If not, clear itimers to avoid subsequent signal generation and
	 * flush and unblock signals.
	 *
	 * This must occur _after_ the task SID has been updated so that any
	 * kill done after the flush will be checked against the new SID.
	 */
	rc = avc_has_perm(osid, sid, SECCLASS_PROCESS, PROCESS__SIGINH, NULL);
	if (rc) {
<<<<<<< HEAD
		memset(&itimer, 0, sizeof itimer);
		for (i = 0; i < 3; i++)
			do_setitimer(i, &itimer, NULL);
		spin_lock_irq(&current->sighand->siglock);
		if (!(current->signal->flags & SIGNAL_GROUP_EXIT)) {
			__flush_signals(current);
			flush_signal_handlers(current, 1);
			sigemptyset(&current->blocked);
		}
		spin_unlock_irq(&current->sighand->siglock);
=======
		clear_itimer();

		spin_lock_irq(&unrcu_pointer(current->sighand)->siglock);
		if (!fatal_signal_pending(current)) {
			flush_sigqueue(&current->pending);
			flush_sigqueue(&current->signal->shared_pending);
			flush_signal_handlers(current, 1);
			sigemptyset(&current->blocked);
			recalc_sigpending();
		}
		spin_unlock_irq(&unrcu_pointer(current->sighand)->siglock);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	}

	/* Wake up the parent if it is waiting so that it can recheck
	 * wait permission to the new task SID. */
	read_lock(&tasklist_lock);
<<<<<<< HEAD
	__wake_up_parent(current, current->real_parent);
=======
	__wake_up_parent(current, unrcu_pointer(current->real_parent));
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	read_unlock(&tasklist_lock);
}

/* superblock security operations */

static int selinux_sb_alloc_security(struct super_block *sb)
{
<<<<<<< HEAD
	return superblock_alloc_security(sb);
}

static void selinux_sb_free_security(struct super_block *sb)
{
	superblock_free_security(sb);
}

static inline int match_prefix(char *prefix, int plen, char *option, int olen)
{
	if (plen > olen)
		return 0;

	return !memcmp(prefix, option, plen);
}

static inline int selinux_option(char *option, int len)
{
	return (match_prefix(CONTEXT_STR, sizeof(CONTEXT_STR)-1, option, len) ||
		match_prefix(FSCONTEXT_STR, sizeof(FSCONTEXT_STR)-1, option, len) ||
		match_prefix(DEFCONTEXT_STR, sizeof(DEFCONTEXT_STR)-1, option, len) ||
		match_prefix(ROOTCONTEXT_STR, sizeof(ROOTCONTEXT_STR)-1, option, len) ||
		match_prefix(LABELSUPP_STR, sizeof(LABELSUPP_STR)-1, option, len));
}

static inline void take_option(char **to, char *from, int *first, int len)
{
	if (!*first) {
		**to = ',';
		*to += 1;
	} else
		*first = 0;
	memcpy(*to, from, len);
	*to += len;
}

static inline void take_selinux_option(char **to, char *from, int *first,
				       int len)
{
	int current_size = 0;

	if (!*first) {
		**to = '|';
		*to += 1;
	} else
		*first = 0;

	while (current_size < len) {
		if (*from != '"') {
			**to = *from;
			*to += 1;
		}
		from += 1;
		current_size += 1;
	}
}

static int selinux_sb_copy_data(char *orig, char *copy)
{
	int fnosec, fsec, rc = 0;
	char *in_save, *in_curr, *in_end;
	char *sec_curr, *nosec_save, *nosec;
	int open_quote = 0;

	in_curr = orig;
	sec_curr = copy;

	nosec = (char *)get_zeroed_page(GFP_KERNEL);
	if (!nosec) {
		rc = -ENOMEM;
		goto out;
	}

	nosec_save = nosec;
	fnosec = fsec = 1;
	in_save = in_end = orig;

	do {
		if (*in_end == '"')
			open_quote = !open_quote;
		if ((*in_end == ',' && open_quote == 0) ||
				*in_end == '\0') {
			int len = in_end - in_curr;

			if (selinux_option(in_curr, len))
				take_selinux_option(&sec_curr, in_curr, &fsec, len);
			else
				take_option(&nosec, in_curr, &fnosec, len);

			in_curr = in_end + 1;
		}
	} while (*in_end++);

	strcpy(in_save, nosec_save);
	free_page((unsigned long)nosec_save);
out:
	return rc;
}

static int selinux_sb_remount(struct super_block *sb, void *data)
{
	int rc, i, *flags;
	struct security_mnt_opts opts;
	char *secdata, **mount_options;
	struct superblock_security_struct *sbsec = sb->s_security;
=======
	struct superblock_security_struct *sbsec = selinux_superblock(sb);

	mutex_init(&sbsec->lock);
	INIT_LIST_HEAD(&sbsec->isec_head);
	spin_lock_init(&sbsec->isec_lock);
	sbsec->sid = SECINITSID_UNLABELED;
	sbsec->def_sid = SECINITSID_FILE;
	sbsec->mntpoint_sid = SECINITSID_UNLABELED;

	return 0;
}

static inline int opt_len(const char *s)
{
	bool open_quote = false;
	int len;
	char c;

	for (len = 0; (c = s[len]) != '\0'; len++) {
		if (c == '"')
			open_quote = !open_quote;
		if (c == ',' && !open_quote)
			break;
	}
	return len;
}

static int selinux_sb_eat_lsm_opts(char *options, void **mnt_opts)
{
	char *from = options;
	char *to = options;
	bool first = true;
	int rc;

	while (1) {
		int len = opt_len(from);
		int token;
		char *arg = NULL;

		token = match_opt_prefix(from, len, &arg);

		if (token != Opt_error) {
			char *p, *q;

			/* strip quotes */
			if (arg) {
				for (p = q = arg; p < from + len; p++) {
					char c = *p;
					if (c != '"')
						*q++ = c;
				}
				arg = kmemdup_nul(arg, q - arg, GFP_KERNEL);
				if (!arg) {
					rc = -ENOMEM;
					goto free_opt;
				}
			}
			rc = selinux_add_opt(token, arg, mnt_opts);
			kfree(arg);
			arg = NULL;
			if (unlikely(rc)) {
				goto free_opt;
			}
		} else {
			if (!first) {	// copy with preceding comma
				from--;
				len++;
			}
			if (to != from)
				memmove(to, from, len);
			to += len;
			first = false;
		}
		if (!from[len])
			break;
		from += len + 1;
	}
	*to = '\0';
	return 0;

free_opt:
	if (*mnt_opts) {
		selinux_free_mnt_opts(*mnt_opts);
		*mnt_opts = NULL;
	}
	return rc;
}

static int selinux_sb_mnt_opts_compat(struct super_block *sb, void *mnt_opts)
{
	struct selinux_mnt_opts *opts = mnt_opts;
	struct superblock_security_struct *sbsec = selinux_superblock(sb);

	/*
	 * Superblock not initialized (i.e. no options) - reject if any
	 * options specified, otherwise accept.
	 */
	if (!(sbsec->flags & SE_SBINITIALIZED))
		return opts ? 1 : 0;

	/*
	 * Superblock initialized and no options specified - reject if
	 * superblock has any options set, otherwise accept.
	 */
	if (!opts)
		return (sbsec->flags & SE_MNTMASK) ? 1 : 0;

	if (opts->fscontext_sid) {
		if (bad_option(sbsec, FSCONTEXT_MNT, sbsec->sid,
			       opts->fscontext_sid))
			return 1;
	}
	if (opts->context_sid) {
		if (bad_option(sbsec, CONTEXT_MNT, sbsec->mntpoint_sid,
			       opts->context_sid))
			return 1;
	}
	if (opts->rootcontext_sid) {
		struct inode_security_struct *root_isec;

		root_isec = backing_inode_security(sb->s_root);
		if (bad_option(sbsec, ROOTCONTEXT_MNT, root_isec->sid,
			       opts->rootcontext_sid))
			return 1;
	}
	if (opts->defcontext_sid) {
		if (bad_option(sbsec, DEFCONTEXT_MNT, sbsec->def_sid,
			       opts->defcontext_sid))
			return 1;
	}
	return 0;
}

static int selinux_sb_remount(struct super_block *sb, void *mnt_opts)
{
	struct selinux_mnt_opts *opts = mnt_opts;
	struct superblock_security_struct *sbsec = selinux_superblock(sb);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	if (!(sbsec->flags & SE_SBINITIALIZED))
		return 0;

<<<<<<< HEAD
	if (!data)
		return 0;

	if (sb->s_type->fs_flags & FS_BINARY_MOUNTDATA)
		return 0;

	security_init_mnt_opts(&opts);
	secdata = alloc_secdata();
	if (!secdata)
		return -ENOMEM;
	rc = selinux_sb_copy_data(data, secdata);
	if (rc)
		goto out_free_secdata;

	rc = selinux_parse_opts_str(secdata, &opts);
	if (rc)
		goto out_free_secdata;

	mount_options = opts.mnt_opts;
	flags = opts.mnt_opts_flags;

	for (i = 0; i < opts.num_mnt_opts; i++) {
		u32 sid;
		size_t len;

		if (flags[i] == SE_SBLABELSUPP)
			continue;
		len = strlen(mount_options[i]);
		rc = security_context_to_sid(mount_options[i], len, &sid);
		if (rc) {
			printk(KERN_WARNING "SELinux: security_context_to_sid"
			       "(%s) failed for (dev %s, type %s) errno=%d\n",
			       mount_options[i], sb->s_id, sb->s_type->name, rc);
			goto out_free_opts;
		}
		rc = -EINVAL;
		switch (flags[i]) {
		case FSCONTEXT_MNT:
			if (bad_option(sbsec, FSCONTEXT_MNT, sbsec->sid, sid))
				goto out_bad_option;
			break;
		case CONTEXT_MNT:
			if (bad_option(sbsec, CONTEXT_MNT, sbsec->mntpoint_sid, sid))
				goto out_bad_option;
			break;
		case ROOTCONTEXT_MNT: {
			struct inode_security_struct *root_isec;
			root_isec = sb->s_root->d_inode->i_security;

			if (bad_option(sbsec, ROOTCONTEXT_MNT, root_isec->sid, sid))
				goto out_bad_option;
			break;
		}
		case DEFCONTEXT_MNT:
			if (bad_option(sbsec, DEFCONTEXT_MNT, sbsec->def_sid, sid))
				goto out_bad_option;
			break;
		default:
			goto out_free_opts;
		}
	}

	rc = 0;
out_free_opts:
	security_free_mnt_opts(&opts);
out_free_secdata:
	free_secdata(secdata);
	return rc;
out_bad_option:
	printk(KERN_WARNING "SELinux: unable to change security options "
	       "during remount (dev %s, type=%s)\n", sb->s_id,
	       sb->s_type->name);
	goto out_free_opts;
}

static int selinux_sb_kern_mount(struct super_block *sb, int flags, void *data)
{
	const struct cred *cred = current_cred();
	struct common_audit_data ad;
	struct selinux_audit_data sad = {0,};
	int rc;

	rc = superblock_doinit(sb, data);
	if (rc)
		return rc;

	/* Allow all mounts performed by the kernel */
	if (flags & MS_KERNMOUNT)
		return 0;

	COMMON_AUDIT_DATA_INIT(&ad, DENTRY);
	ad.selinux_audit_data = &sad;
=======
	if (!opts)
		return 0;

	if (opts->fscontext_sid) {
		if (bad_option(sbsec, FSCONTEXT_MNT, sbsec->sid,
			       opts->fscontext_sid))
			goto out_bad_option;
	}
	if (opts->context_sid) {
		if (bad_option(sbsec, CONTEXT_MNT, sbsec->mntpoint_sid,
			       opts->context_sid))
			goto out_bad_option;
	}
	if (opts->rootcontext_sid) {
		struct inode_security_struct *root_isec;
		root_isec = backing_inode_security(sb->s_root);
		if (bad_option(sbsec, ROOTCONTEXT_MNT, root_isec->sid,
			       opts->rootcontext_sid))
			goto out_bad_option;
	}
	if (opts->defcontext_sid) {
		if (bad_option(sbsec, DEFCONTEXT_MNT, sbsec->def_sid,
			       opts->defcontext_sid))
			goto out_bad_option;
	}
	return 0;

out_bad_option:
	pr_warn("SELinux: unable to change security options "
	       "during remount (dev %s, type=%s)\n", sb->s_id,
	       sb->s_type->name);
	return -EINVAL;
}

static int selinux_sb_kern_mount(const struct super_block *sb)
{
	const struct cred *cred = current_cred();
	struct common_audit_data ad;

	ad.type = LSM_AUDIT_DATA_DENTRY;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	ad.u.dentry = sb->s_root;
	return superblock_has_perm(cred, sb, FILESYSTEM__MOUNT, &ad);
}

static int selinux_sb_statfs(struct dentry *dentry)
{
	const struct cred *cred = current_cred();
	struct common_audit_data ad;
<<<<<<< HEAD
	struct selinux_audit_data sad = {0,};

	COMMON_AUDIT_DATA_INIT(&ad, DENTRY);
	ad.selinux_audit_data = &sad;
=======

	ad.type = LSM_AUDIT_DATA_DENTRY;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	ad.u.dentry = dentry->d_sb->s_root;
	return superblock_has_perm(cred, dentry->d_sb, FILESYSTEM__GETATTR, &ad);
}

static int selinux_mount(const char *dev_name,
<<<<<<< HEAD
			 struct path *path,
=======
			 const struct path *path,
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
			 const char *type,
			 unsigned long flags,
			 void *data)
{
	const struct cred *cred = current_cred();

	if (flags & MS_REMOUNT)
		return superblock_has_perm(cred, path->dentry->d_sb,
					   FILESYSTEM__REMOUNT, NULL);
	else
		return path_has_perm(cred, path, FILE__MOUNTON);
}

<<<<<<< HEAD
=======
static int selinux_move_mount(const struct path *from_path,
			      const struct path *to_path)
{
	const struct cred *cred = current_cred();

	return path_has_perm(cred, to_path, FILE__MOUNTON);
}

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
static int selinux_umount(struct vfsmount *mnt, int flags)
{
	const struct cred *cred = current_cred();

	return superblock_has_perm(cred, mnt->mnt_sb,
				   FILESYSTEM__UNMOUNT, NULL);
}

<<<<<<< HEAD
=======
static int selinux_fs_context_submount(struct fs_context *fc,
				   struct super_block *reference)
{
	const struct superblock_security_struct *sbsec = selinux_superblock(reference);
	struct selinux_mnt_opts *opts;

	/*
	 * Ensure that fc->security remains NULL when no options are set
	 * as expected by selinux_set_mnt_opts().
	 */
	if (!(sbsec->flags & (FSCONTEXT_MNT|CONTEXT_MNT|DEFCONTEXT_MNT)))
		return 0;

	opts = kzalloc(sizeof(*opts), GFP_KERNEL);
	if (!opts)
		return -ENOMEM;

	if (sbsec->flags & FSCONTEXT_MNT)
		opts->fscontext_sid = sbsec->sid;
	if (sbsec->flags & CONTEXT_MNT)
		opts->context_sid = sbsec->mntpoint_sid;
	if (sbsec->flags & DEFCONTEXT_MNT)
		opts->defcontext_sid = sbsec->def_sid;
	fc->security = opts;
	return 0;
}

static int selinux_fs_context_dup(struct fs_context *fc,
				  struct fs_context *src_fc)
{
	const struct selinux_mnt_opts *src = src_fc->security;

	if (!src)
		return 0;

	fc->security = kmemdup(src, sizeof(*src), GFP_KERNEL);
	return fc->security ? 0 : -ENOMEM;
}

static const struct fs_parameter_spec selinux_fs_parameters[] = {
	fsparam_string(CONTEXT_STR,	Opt_context),
	fsparam_string(DEFCONTEXT_STR,	Opt_defcontext),
	fsparam_string(FSCONTEXT_STR,	Opt_fscontext),
	fsparam_string(ROOTCONTEXT_STR,	Opt_rootcontext),
	fsparam_flag  (SECLABEL_STR,	Opt_seclabel),
	{}
};

static int selinux_fs_context_parse_param(struct fs_context *fc,
					  struct fs_parameter *param)
{
	struct fs_parse_result result;
	int opt;

	opt = fs_parse(fc, selinux_fs_parameters, param, &result);
	if (opt < 0)
		return opt;

	return selinux_add_opt(opt, param->string, &fc->security);
}

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
/* inode security operations */

static int selinux_inode_alloc_security(struct inode *inode)
{
<<<<<<< HEAD
	return inode_alloc_security(inode);
=======
	struct inode_security_struct *isec = selinux_inode(inode);
	u32 sid = current_sid();

	spin_lock_init(&isec->lock);
	INIT_LIST_HEAD(&isec->list);
	isec->inode = inode;
	isec->sid = SECINITSID_UNLABELED;
	isec->sclass = SECCLASS_FILE;
	isec->task_sid = sid;
	isec->initialized = LABEL_INVALID;

	return 0;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

static void selinux_inode_free_security(struct inode *inode)
{
	inode_free_security(inode);
}

<<<<<<< HEAD
static int selinux_inode_init_security(struct inode *inode, struct inode *dir,
				       const struct qstr *qstr, char **name,
				       void **value, size_t *len)
{
	const struct task_security_struct *tsec = current_security();
	struct inode_security_struct *dsec;
	struct superblock_security_struct *sbsec;
	u32 sid, newsid, clen;
	int rc;
	char *namep = NULL, *context;

	dsec = dir->i_security;
	sbsec = dir->i_sb->s_security;

	sid = tsec->sid;
	newsid = tsec->create_sid;

	if ((sbsec->flags & SE_SBINITIALIZED) &&
	    (sbsec->behavior == SECURITY_FS_USE_MNTPOINT))
		newsid = sbsec->mntpoint_sid;
	else if (!newsid || !(sbsec->flags & SE_SBLABELSUPP)) {
		rc = security_transition_sid(sid, dsec->sid,
					     inode_mode_to_security_class(inode->i_mode),
					     qstr, &newsid);
		if (rc) {
			printk(KERN_WARNING "%s:  "
			       "security_transition_sid failed, rc=%d (dev=%s "
			       "ino=%ld)\n",
			       __func__,
			       -rc, inode->i_sb->s_id, inode->i_ino);
			return rc;
		}
	}

	/* Possibly defer initialization to selinux_complete_init. */
	if (sbsec->flags & SE_SBINITIALIZED) {
		struct inode_security_struct *isec = inode->i_security;
		isec->sclass = inode_mode_to_security_class(inode->i_mode);
		isec->sid = newsid;
		isec->initialized = 1;
	}

	if (!ss_initialized || !(sbsec->flags & SE_SBLABELSUPP))
		return -EOPNOTSUPP;

	if (name) {
		namep = kstrdup(XATTR_SELINUX_SUFFIX, GFP_NOFS);
		if (!namep)
			return -ENOMEM;
		*name = namep;
	}

	if (value && len) {
		rc = security_sid_to_context_force(newsid, &context, &clen);
		if (rc) {
			kfree(namep);
			return rc;
		}
		*value = context;
		*len = clen;
=======
static int selinux_dentry_init_security(struct dentry *dentry, int mode,
					const struct qstr *name,
					const char **xattr_name, void **ctx,
					u32 *ctxlen)
{
	u32 newsid;
	int rc;

	rc = selinux_determine_inode_label(selinux_cred(current_cred()),
					   d_inode(dentry->d_parent), name,
					   inode_mode_to_security_class(mode),
					   &newsid);
	if (rc)
		return rc;

	if (xattr_name)
		*xattr_name = XATTR_NAME_SELINUX;

	return security_sid_to_context(newsid, (char **)ctx,
				       ctxlen);
}

static int selinux_dentry_create_files_as(struct dentry *dentry, int mode,
					  struct qstr *name,
					  const struct cred *old,
					  struct cred *new)
{
	u32 newsid;
	int rc;
	struct task_security_struct *tsec;

	rc = selinux_determine_inode_label(selinux_cred(old),
					   d_inode(dentry->d_parent), name,
					   inode_mode_to_security_class(mode),
					   &newsid);
	if (rc)
		return rc;

	tsec = selinux_cred(new);
	tsec->create_sid = newsid;
	return 0;
}

static int selinux_inode_init_security(struct inode *inode, struct inode *dir,
				       const struct qstr *qstr,
				       struct xattr *xattrs, int *xattr_count)
{
	const struct task_security_struct *tsec = selinux_cred(current_cred());
	struct superblock_security_struct *sbsec;
	struct xattr *xattr = lsm_get_xattr_slot(xattrs, xattr_count);
	u32 newsid, clen;
	u16 newsclass;
	int rc;
	char *context;

	sbsec = selinux_superblock(dir->i_sb);

	newsid = tsec->create_sid;
	newsclass = inode_mode_to_security_class(inode->i_mode);
	rc = selinux_determine_inode_label(tsec, dir, qstr, newsclass, &newsid);
	if (rc)
		return rc;

	/* Possibly defer initialization to selinux_complete_init. */
	if (sbsec->flags & SE_SBINITIALIZED) {
		struct inode_security_struct *isec = selinux_inode(inode);
		isec->sclass = newsclass;
		isec->sid = newsid;
		isec->initialized = LABEL_INITIALIZED;
	}

	if (!selinux_initialized() ||
	    !(sbsec->flags & SBLABEL_MNT))
		return -EOPNOTSUPP;

	if (xattr) {
		rc = security_sid_to_context_force(newsid,
						   &context, &clen);
		if (rc)
			return rc;
		xattr->value = context;
		xattr->value_len = clen;
		xattr->name = XATTR_SELINUX_SUFFIX;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	}

	return 0;
}

<<<<<<< HEAD
static int selinux_inode_create(struct inode *dir, struct dentry *dentry, umode_t mode)
{
	int ret;

	ret = pft_inode_create(dir, dentry, mode);
	if (ret < 0)
		return ret;

	return may_create(dir, dentry, SECCLASS_FILE);
}

static int selinux_inode_post_create(struct inode *dir, struct dentry *dentry,
				     umode_t mode)
{
	int ret;

	ret = pft_inode_post_create(dir, dentry, mode);

	return ret;
=======
static int selinux_inode_init_security_anon(struct inode *inode,
					    const struct qstr *name,
					    const struct inode *context_inode)
{
	const struct task_security_struct *tsec = selinux_cred(current_cred());
	struct common_audit_data ad;
	struct inode_security_struct *isec;
	int rc;

	if (unlikely(!selinux_initialized()))
		return 0;

	isec = selinux_inode(inode);

	/*
	 * We only get here once per ephemeral inode.  The inode has
	 * been initialized via inode_alloc_security but is otherwise
	 * untouched.
	 */

	if (context_inode) {
		struct inode_security_struct *context_isec =
			selinux_inode(context_inode);
		if (context_isec->initialized != LABEL_INITIALIZED) {
			pr_err("SELinux:  context_inode is not initialized\n");
			return -EACCES;
		}

		isec->sclass = context_isec->sclass;
		isec->sid = context_isec->sid;
	} else {
		isec->sclass = SECCLASS_ANON_INODE;
		rc = security_transition_sid(
			tsec->sid, tsec->sid,
			isec->sclass, name, &isec->sid);
		if (rc)
			return rc;
	}

	isec->initialized = LABEL_INITIALIZED;
	/*
	 * Now that we've initialized security, check whether we're
	 * allowed to actually create this type of anonymous inode.
	 */

	ad.type = LSM_AUDIT_DATA_ANONINODE;
	ad.u.anonclass = name ? (const char *)name->name : "?";

	return avc_has_perm(tsec->sid,
			    isec->sid,
			    isec->sclass,
			    FILE__CREATE,
			    &ad);
}

static int selinux_inode_create(struct inode *dir, struct dentry *dentry, umode_t mode)
{
	return may_create(dir, dentry, SECCLASS_FILE);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

static int selinux_inode_link(struct dentry *old_dentry, struct inode *dir, struct dentry *new_dentry)
{
	return may_link(dir, old_dentry, MAY_LINK);
}

static int selinux_inode_unlink(struct inode *dir, struct dentry *dentry)
{
	return may_link(dir, dentry, MAY_UNLINK);
}

static int selinux_inode_symlink(struct inode *dir, struct dentry *dentry, const char *name)
{
	return may_create(dir, dentry, SECCLASS_LNK_FILE);
}

static int selinux_inode_mkdir(struct inode *dir, struct dentry *dentry, umode_t mask)
{
	return may_create(dir, dentry, SECCLASS_DIR);
}

static int selinux_inode_rmdir(struct inode *dir, struct dentry *dentry)
{
	return may_link(dir, dentry, MAY_RMDIR);
}

static int selinux_inode_mknod(struct inode *dir, struct dentry *dentry, umode_t mode, dev_t dev)
{
	return may_create(dir, dentry, inode_mode_to_security_class(mode));
}

static int selinux_inode_rename(struct inode *old_inode, struct dentry *old_dentry,
				struct inode *new_inode, struct dentry *new_dentry)
{
<<<<<<< HEAD
	int rc;

	rc = pft_inode_rename(old_inode, old_dentry, new_inode, new_dentry);
	if (rc)
		return rc;

=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	return may_rename(old_inode, old_dentry, new_inode, new_dentry);
}

static int selinux_inode_readlink(struct dentry *dentry)
{
	const struct cred *cred = current_cred();

	return dentry_has_perm(cred, dentry, FILE__READ);
}

<<<<<<< HEAD
static int selinux_inode_follow_link(struct dentry *dentry, struct nameidata *nameidata)
{
	const struct cred *cred = current_cred();

	return dentry_has_perm(cred, dentry, FILE__READ);
=======
static int selinux_inode_follow_link(struct dentry *dentry, struct inode *inode,
				     bool rcu)
{
	const struct cred *cred = current_cred();
	struct common_audit_data ad;
	struct inode_security_struct *isec;
	u32 sid;

	ad.type = LSM_AUDIT_DATA_DENTRY;
	ad.u.dentry = dentry;
	sid = cred_sid(cred);
	isec = inode_security_rcu(inode, rcu);
	if (IS_ERR(isec))
		return PTR_ERR(isec);

	return avc_has_perm(sid, isec->sid, isec->sclass, FILE__READ, &ad);
}

static noinline int audit_inode_permission(struct inode *inode,
					   u32 perms, u32 audited, u32 denied,
					   int result)
{
	struct common_audit_data ad;
	struct inode_security_struct *isec = selinux_inode(inode);

	ad.type = LSM_AUDIT_DATA_INODE;
	ad.u.inode = inode;

	return slow_avc_audit(current_sid(), isec->sid, isec->sclass, perms,
			    audited, denied, result, &ad);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

static int selinux_inode_permission(struct inode *inode, int mask)
{
	const struct cred *cred = current_cred();
<<<<<<< HEAD
	struct common_audit_data ad;
	struct selinux_audit_data sad = {0,};
	u32 perms;
	bool from_access;
	unsigned flags = mask & MAY_NOT_BLOCK;
=======
	u32 perms;
	bool from_access;
	bool no_block = mask & MAY_NOT_BLOCK;
	struct inode_security_struct *isec;
	u32 sid;
	struct av_decision avd;
	int rc, rc2;
	u32 audited, denied;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	from_access = mask & MAY_ACCESS;
	mask &= (MAY_READ|MAY_WRITE|MAY_EXEC|MAY_APPEND);

	/* No permission to check.  Existence test. */
	if (!mask)
		return 0;

<<<<<<< HEAD
	COMMON_AUDIT_DATA_INIT(&ad, INODE);
	ad.selinux_audit_data = &sad;
	ad.u.inode = inode;

	if (from_access)
		ad.selinux_audit_data->auditdeny |= FILE__AUDIT_ACCESS;

	perms = file_mask_to_av(inode->i_mode, mask);

	return inode_has_perm(cred, inode, perms, &ad, flags);
}

static int selinux_inode_setattr(struct dentry *dentry, struct iattr *iattr)
{
	const struct cred *cred = current_cred();
	unsigned int ia_valid = iattr->ia_valid;
=======
	if (unlikely(IS_PRIVATE(inode)))
		return 0;

	perms = file_mask_to_av(inode->i_mode, mask);

	sid = cred_sid(cred);
	isec = inode_security_rcu(inode, no_block);
	if (IS_ERR(isec))
		return PTR_ERR(isec);

	rc = avc_has_perm_noaudit(sid, isec->sid, isec->sclass, perms, 0,
				  &avd);
	audited = avc_audit_required(perms, &avd, rc,
				     from_access ? FILE__AUDIT_ACCESS : 0,
				     &denied);
	if (likely(!audited))
		return rc;

	rc2 = audit_inode_permission(inode, perms, audited, denied, rc);
	if (rc2)
		return rc2;
	return rc;
}

static int selinux_inode_setattr(struct mnt_idmap *idmap, struct dentry *dentry,
				 struct iattr *iattr)
{
	const struct cred *cred = current_cred();
	struct inode *inode = d_backing_inode(dentry);
	unsigned int ia_valid = iattr->ia_valid;
	__u32 av = FILE__WRITE;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	/* ATTR_FORCE is just used for ATTR_KILL_S[UG]ID. */
	if (ia_valid & ATTR_FORCE) {
		ia_valid &= ~(ATTR_KILL_SUID | ATTR_KILL_SGID | ATTR_MODE |
			      ATTR_FORCE);
		if (!ia_valid)
			return 0;
	}

	if (ia_valid & (ATTR_MODE | ATTR_UID | ATTR_GID |
			ATTR_ATIME_SET | ATTR_MTIME_SET | ATTR_TIMES_SET))
		return dentry_has_perm(cred, dentry, FILE__SETATTR);

<<<<<<< HEAD
	return dentry_has_perm(cred, dentry, FILE__WRITE);
}

static int selinux_inode_getattr(struct vfsmount *mnt, struct dentry *dentry)
{
	const struct cred *cred = current_cred();
	struct path path;

	path.dentry = dentry;
	path.mnt = mnt;

	return path_has_perm(cred, &path, FILE__GETATTR);
}

static int selinux_inode_setotherxattr(struct dentry *dentry, const char *name)
{
	const struct cred *cred = current_cred();

	if (pft_inode_set_xattr(dentry, name) < 0)
		return -EACCES;


	if (!strncmp(name, XATTR_SECURITY_PREFIX,
		     sizeof XATTR_SECURITY_PREFIX - 1)) {
		if (!strcmp(name, XATTR_NAME_CAPS)) {
			if (!capable(CAP_SETFCAP))
				return -EPERM;
		} else if (!capable(CAP_SYS_ADMIN)) {
			/* A different attribute in the security namespace.
			   Restrict to administrator. */
			return -EPERM;
		}
	}

	/* Not an attribute we recognize, so just check the
	   ordinary setattr permission. */
	return dentry_has_perm(cred, dentry, FILE__SETATTR);
}

static int selinux_inode_setxattr(struct dentry *dentry, const char *name,
				  const void *value, size_t size, int flags)
{
	struct inode *inode = dentry->d_inode;
	struct inode_security_struct *isec = inode->i_security;
	struct superblock_security_struct *sbsec;
	struct common_audit_data ad;
	struct selinux_audit_data sad = {0,};
	u32 newsid, sid = current_sid();
	int rc = 0;

	if (strcmp(name, XATTR_NAME_SELINUX))
		return selinux_inode_setotherxattr(dentry, name);

	sbsec = inode->i_sb->s_security;
	if (!(sbsec->flags & SE_SBLABELSUPP))
		return -EOPNOTSUPP;

	if (!inode_owner_or_capable(inode))
		return -EPERM;

	COMMON_AUDIT_DATA_INIT(&ad, DENTRY);
	ad.selinux_audit_data = &sad;
	ad.u.dentry = dentry;

=======
	if (selinux_policycap_openperm() &&
	    inode->i_sb->s_magic != SOCKFS_MAGIC &&
	    (ia_valid & ATTR_SIZE) &&
	    !(ia_valid & ATTR_FILE))
		av |= FILE__OPEN;

	return dentry_has_perm(cred, dentry, av);
}

static int selinux_inode_getattr(const struct path *path)
{
	return path_has_perm(current_cred(), path, FILE__GETATTR);
}

static bool has_cap_mac_admin(bool audit)
{
	const struct cred *cred = current_cred();
	unsigned int opts = audit ? CAP_OPT_NONE : CAP_OPT_NOAUDIT;

	if (cap_capable(cred, &init_user_ns, CAP_MAC_ADMIN, opts))
		return false;
	if (cred_has_capability(cred, CAP_MAC_ADMIN, opts, true))
		return false;
	return true;
}

static int selinux_inode_setxattr(struct mnt_idmap *idmap,
				  struct dentry *dentry, const char *name,
				  const void *value, size_t size, int flags)
{
	struct inode *inode = d_backing_inode(dentry);
	struct inode_security_struct *isec;
	struct superblock_security_struct *sbsec;
	struct common_audit_data ad;
	u32 newsid, sid = current_sid();
	int rc = 0;

	if (strcmp(name, XATTR_NAME_SELINUX)) {
		rc = cap_inode_setxattr(dentry, name, value, size, flags);
		if (rc)
			return rc;

		/* Not an attribute we recognize, so just check the
		   ordinary setattr permission. */
		return dentry_has_perm(current_cred(), dentry, FILE__SETATTR);
	}

	if (!selinux_initialized())
		return (inode_owner_or_capable(idmap, inode) ? 0 : -EPERM);

	sbsec = selinux_superblock(inode->i_sb);
	if (!(sbsec->flags & SBLABEL_MNT))
		return -EOPNOTSUPP;

	if (!inode_owner_or_capable(idmap, inode))
		return -EPERM;

	ad.type = LSM_AUDIT_DATA_DENTRY;
	ad.u.dentry = dentry;

	isec = backing_inode_security(dentry);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	rc = avc_has_perm(sid, isec->sid, isec->sclass,
			  FILE__RELABELFROM, &ad);
	if (rc)
		return rc;

<<<<<<< HEAD
	rc = security_context_to_sid(value, size, &newsid);
	if (rc == -EINVAL) {
		if (!capable(CAP_MAC_ADMIN))
			return rc;
		rc = security_context_to_sid_force(value, size, &newsid);
=======
	rc = security_context_to_sid(value, size, &newsid,
				     GFP_KERNEL);
	if (rc == -EINVAL) {
		if (!has_cap_mac_admin(true)) {
			struct audit_buffer *ab;
			size_t audit_size;

			/* We strip a nul only if it is at the end, otherwise the
			 * context contains a nul and we should audit that */
			if (value) {
				const char *str = value;

				if (str[size - 1] == '\0')
					audit_size = size - 1;
				else
					audit_size = size;
			} else {
				audit_size = 0;
			}
			ab = audit_log_start(audit_context(),
					     GFP_ATOMIC, AUDIT_SELINUX_ERR);
			if (!ab)
				return rc;
			audit_log_format(ab, "op=setxattr invalid_context=");
			audit_log_n_untrustedstring(ab, value, audit_size);
			audit_log_end(ab);

			return rc;
		}
		rc = security_context_to_sid_force(value,
						   size, &newsid);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	}
	if (rc)
		return rc;

	rc = avc_has_perm(sid, newsid, isec->sclass,
			  FILE__RELABELTO, &ad);
	if (rc)
		return rc;

<<<<<<< HEAD
	rc = security_validate_transition(isec->sid, newsid, sid,
					  isec->sclass);
=======
	rc = security_validate_transition(isec->sid, newsid,
					  sid, isec->sclass);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	if (rc)
		return rc;

	return avc_has_perm(newsid,
			    sbsec->sid,
			    SECCLASS_FILESYSTEM,
			    FILESYSTEM__ASSOCIATE,
			    &ad);
}

<<<<<<< HEAD
=======
static int selinux_inode_set_acl(struct mnt_idmap *idmap,
				 struct dentry *dentry, const char *acl_name,
				 struct posix_acl *kacl)
{
	return dentry_has_perm(current_cred(), dentry, FILE__SETATTR);
}

static int selinux_inode_get_acl(struct mnt_idmap *idmap,
				 struct dentry *dentry, const char *acl_name)
{
	return dentry_has_perm(current_cred(), dentry, FILE__GETATTR);
}

static int selinux_inode_remove_acl(struct mnt_idmap *idmap,
				    struct dentry *dentry, const char *acl_name)
{
	return dentry_has_perm(current_cred(), dentry, FILE__SETATTR);
}

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
static void selinux_inode_post_setxattr(struct dentry *dentry, const char *name,
					const void *value, size_t size,
					int flags)
{
<<<<<<< HEAD
	struct inode *inode = dentry->d_inode;
	struct inode_security_struct *isec = inode->i_security;
=======
	struct inode *inode = d_backing_inode(dentry);
	struct inode_security_struct *isec;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	u32 newsid;
	int rc;

	if (strcmp(name, XATTR_NAME_SELINUX)) {
		/* Not an attribute we recognize, so nothing to do. */
		return;
	}

<<<<<<< HEAD
	rc = security_context_to_sid_force(value, size, &newsid);
	if (rc) {
		printk(KERN_ERR "SELinux:  unable to map context to SID"
=======
	if (!selinux_initialized()) {
		/* If we haven't even been initialized, then we can't validate
		 * against a policy, so leave the label as invalid. It may
		 * resolve to a valid label on the next revalidation try if
		 * we've since initialized.
		 */
		return;
	}

	rc = security_context_to_sid_force(value, size,
					   &newsid);
	if (rc) {
		pr_err("SELinux:  unable to map context to SID"
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		       "for (%s, %lu), rc=%d\n",
		       inode->i_sb->s_id, inode->i_ino, -rc);
		return;
	}

<<<<<<< HEAD
	isec->sid = newsid;
	return;
=======
	isec = backing_inode_security(dentry);
	spin_lock(&isec->lock);
	isec->sclass = inode_mode_to_security_class(inode->i_mode);
	isec->sid = newsid;
	isec->initialized = LABEL_INITIALIZED;
	spin_unlock(&isec->lock);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

static int selinux_inode_getxattr(struct dentry *dentry, const char *name)
{
	const struct cred *cred = current_cred();

	return dentry_has_perm(cred, dentry, FILE__GETATTR);
}

static int selinux_inode_listxattr(struct dentry *dentry)
{
	const struct cred *cred = current_cred();

	return dentry_has_perm(cred, dentry, FILE__GETATTR);
}

<<<<<<< HEAD
static int selinux_inode_removexattr(struct dentry *dentry, const char *name)
{
	if (strcmp(name, XATTR_NAME_SELINUX))
		return selinux_inode_setotherxattr(dentry, name);
=======
static int selinux_inode_removexattr(struct mnt_idmap *idmap,
				     struct dentry *dentry, const char *name)
{
	if (strcmp(name, XATTR_NAME_SELINUX)) {
		int rc = cap_inode_removexattr(idmap, dentry, name);
		if (rc)
			return rc;

		/* Not an attribute we recognize, so just check the
		   ordinary setattr permission. */
		return dentry_has_perm(current_cred(), dentry, FILE__SETATTR);
	}

	if (!selinux_initialized())
		return 0;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	/* No one is allowed to remove a SELinux security label.
	   You can change the label, but all data must be labeled. */
	return -EACCES;
}

<<<<<<< HEAD
=======
static int selinux_path_notify(const struct path *path, u64 mask,
						unsigned int obj_type)
{
	int ret;
	u32 perm;

	struct common_audit_data ad;

	ad.type = LSM_AUDIT_DATA_PATH;
	ad.u.path = *path;

	/*
	 * Set permission needed based on the type of mark being set.
	 * Performs an additional check for sb watches.
	 */
	switch (obj_type) {
	case FSNOTIFY_OBJ_TYPE_VFSMOUNT:
		perm = FILE__WATCH_MOUNT;
		break;
	case FSNOTIFY_OBJ_TYPE_SB:
		perm = FILE__WATCH_SB;
		ret = superblock_has_perm(current_cred(), path->dentry->d_sb,
						FILESYSTEM__WATCH, &ad);
		if (ret)
			return ret;
		break;
	case FSNOTIFY_OBJ_TYPE_INODE:
		perm = FILE__WATCH;
		break;
	default:
		return -EINVAL;
	}

	/* blocking watches require the file:watch_with_perm permission */
	if (mask & (ALL_FSNOTIFY_PERM_EVENTS))
		perm |= FILE__WATCH_WITH_PERM;

	/* watches on read-like events need the file:watch_reads permission */
	if (mask & (FS_ACCESS | FS_ACCESS_PERM | FS_CLOSE_NOWRITE))
		perm |= FILE__WATCH_READS;

	return path_has_perm(current_cred(), path, perm);
}

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
/*
 * Copy the inode security context value to the user.
 *
 * Permission check is handled by selinux_inode_getxattr hook.
 */
<<<<<<< HEAD
static int selinux_inode_getsecurity(const struct inode *inode, const char *name, void **buffer, bool alloc)
=======
static int selinux_inode_getsecurity(struct mnt_idmap *idmap,
				     struct inode *inode, const char *name,
				     void **buffer, bool alloc)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
{
	u32 size;
	int error;
	char *context = NULL;
<<<<<<< HEAD
	struct inode_security_struct *isec = inode->i_security;

	if (strcmp(name, XATTR_SELINUX_SUFFIX))
=======
	struct inode_security_struct *isec;

	/*
	 * If we're not initialized yet, then we can't validate contexts, so
	 * just let vfs_getxattr fall back to using the on-disk xattr.
	 */
	if (!selinux_initialized() ||
	    strcmp(name, XATTR_SELINUX_SUFFIX))
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		return -EOPNOTSUPP;

	/*
	 * If the caller has CAP_MAC_ADMIN, then get the raw context
	 * value even if it is not defined by current policy; otherwise,
	 * use the in-core value under current policy.
	 * Use the non-auditing forms of the permission checks since
	 * getxattr may be called by unprivileged processes commonly
	 * and lack of permission just means that we fall back to the
	 * in-core context value, not a denial.
	 */
<<<<<<< HEAD
	error = selinux_capable(current_cred(), &init_user_ns, CAP_MAC_ADMIN,
				SECURITY_CAP_NOAUDIT);
	if (!error)
		error = security_sid_to_context_force(isec->sid, &context,
						      &size);
	else
		error = security_sid_to_context(isec->sid, &context, &size);
=======
	isec = inode_security(inode);
	if (has_cap_mac_admin(false))
		error = security_sid_to_context_force(isec->sid, &context,
						      &size);
	else
		error = security_sid_to_context(isec->sid,
						&context, &size);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	if (error)
		return error;
	error = size;
	if (alloc) {
		*buffer = context;
		goto out_nofree;
	}
	kfree(context);
out_nofree:
	return error;
}

static int selinux_inode_setsecurity(struct inode *inode, const char *name,
				     const void *value, size_t size, int flags)
{
<<<<<<< HEAD
	struct inode_security_struct *isec = inode->i_security;
=======
	struct inode_security_struct *isec = inode_security_novalidate(inode);
	struct superblock_security_struct *sbsec;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	u32 newsid;
	int rc;

	if (strcmp(name, XATTR_SELINUX_SUFFIX))
		return -EOPNOTSUPP;

<<<<<<< HEAD
	if (!value || !size)
		return -EACCES;

	rc = security_context_to_sid((void *)value, size, &newsid);
	if (rc)
		return rc;

	isec->sid = newsid;
	isec->initialized = 1;
=======
	sbsec = selinux_superblock(inode->i_sb);
	if (!(sbsec->flags & SBLABEL_MNT))
		return -EOPNOTSUPP;

	if (!value || !size)
		return -EACCES;

	rc = security_context_to_sid(value, size, &newsid,
				     GFP_KERNEL);
	if (rc)
		return rc;

	spin_lock(&isec->lock);
	isec->sclass = inode_mode_to_security_class(inode->i_mode);
	isec->sid = newsid;
	isec->initialized = LABEL_INITIALIZED;
	spin_unlock(&isec->lock);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	return 0;
}

static int selinux_inode_listsecurity(struct inode *inode, char *buffer, size_t buffer_size)
{
	const int len = sizeof(XATTR_NAME_SELINUX);
<<<<<<< HEAD
=======

	if (!selinux_initialized())
		return 0;

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	if (buffer && len <= buffer_size)
		memcpy(buffer, XATTR_NAME_SELINUX, len);
	return len;
}

<<<<<<< HEAD
static void selinux_inode_getsecid(const struct inode *inode, u32 *secid)
{
	struct inode_security_struct *isec = inode->i_security;
	*secid = isec->sid;
}

=======
static void selinux_inode_getsecid(struct inode *inode, u32 *secid)
{
	struct inode_security_struct *isec = inode_security_novalidate(inode);
	*secid = isec->sid;
}

static int selinux_inode_copy_up(struct dentry *src, struct cred **new)
{
	u32 sid;
	struct task_security_struct *tsec;
	struct cred *new_creds = *new;

	if (new_creds == NULL) {
		new_creds = prepare_creds();
		if (!new_creds)
			return -ENOMEM;
	}

	tsec = selinux_cred(new_creds);
	/* Get label from overlay inode and set it in create_sid */
	selinux_inode_getsecid(d_inode(src), &sid);
	tsec->create_sid = sid;
	*new = new_creds;
	return 0;
}

static int selinux_inode_copy_up_xattr(const char *name)
{
	/* The copy_up hook above sets the initial context on an inode, but we
	 * don't then want to overwrite it by blindly copying all the lower
	 * xattrs up.  Instead, filter out SELinux-related xattrs following
	 * policy load.
	 */
	if (selinux_initialized() && strcmp(name, XATTR_NAME_SELINUX) == 0)
		return 1; /* Discard */
	/*
	 * Any other attribute apart from SELINUX is not claimed, supported
	 * by selinux.
	 */
	return -EOPNOTSUPP;
}

/* kernfs node operations */

static int selinux_kernfs_init_security(struct kernfs_node *kn_dir,
					struct kernfs_node *kn)
{
	const struct task_security_struct *tsec = selinux_cred(current_cred());
	u32 parent_sid, newsid, clen;
	int rc;
	char *context;

	rc = kernfs_xattr_get(kn_dir, XATTR_NAME_SELINUX, NULL, 0);
	if (rc == -ENODATA)
		return 0;
	else if (rc < 0)
		return rc;

	clen = (u32)rc;
	context = kmalloc(clen, GFP_KERNEL);
	if (!context)
		return -ENOMEM;

	rc = kernfs_xattr_get(kn_dir, XATTR_NAME_SELINUX, context, clen);
	if (rc < 0) {
		kfree(context);
		return rc;
	}

	rc = security_context_to_sid(context, clen, &parent_sid,
				     GFP_KERNEL);
	kfree(context);
	if (rc)
		return rc;

	if (tsec->create_sid) {
		newsid = tsec->create_sid;
	} else {
		u16 secclass = inode_mode_to_security_class(kn->mode);
		struct qstr q;

		q.name = kn->name;
		q.hash_len = hashlen_string(kn_dir, kn->name);

		rc = security_transition_sid(tsec->sid,
					     parent_sid, secclass, &q,
					     &newsid);
		if (rc)
			return rc;
	}

	rc = security_sid_to_context_force(newsid,
					   &context, &clen);
	if (rc)
		return rc;

	rc = kernfs_xattr_set(kn, XATTR_NAME_SELINUX, context, clen,
			      XATTR_CREATE);
	kfree(context);
	return rc;
}


>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
/* file security operations */

static int selinux_revalidate_file_permission(struct file *file, int mask)
{
	const struct cred *cred = current_cred();
<<<<<<< HEAD
	struct inode *inode = file->f_path.dentry->d_inode;
=======
	struct inode *inode = file_inode(file);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	/* file_mask_to_av won't add FILE__WRITE if MAY_APPEND is set */
	if ((file->f_flags & O_APPEND) && (mask & MAY_WRITE))
		mask |= MAY_APPEND;

	return file_has_perm(cred, file,
			     file_mask_to_av(inode->i_mode, mask));
}

static int selinux_file_permission(struct file *file, int mask)
{
<<<<<<< HEAD
	struct inode *inode = file->f_path.dentry->d_inode;
	struct file_security_struct *fsec = file->f_security;
	struct inode_security_struct *isec = inode->i_security;
	u32 sid = current_sid();
	int ret;
=======
	struct inode *inode = file_inode(file);
	struct file_security_struct *fsec = selinux_file(file);
	struct inode_security_struct *isec;
	u32 sid = current_sid();
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	if (!mask)
		/* No permission to check.  Existence test. */
		return 0;

<<<<<<< HEAD
	ret = pft_file_permission(file, mask);
	if (ret < 0)
		return ret;

	if (sid == fsec->sid && fsec->isid == isec->sid &&
	    fsec->pseqno == avc_policy_seqno())
		/* No change since dentry_open check. */
=======
	isec = inode_security(inode);
	if (sid == fsec->sid && fsec->isid == isec->sid &&
	    fsec->pseqno == avc_policy_seqno())
		/* No change since file_open check. */
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		return 0;

	return selinux_revalidate_file_permission(file, mask);
}

static int selinux_file_alloc_security(struct file *file)
{
<<<<<<< HEAD
	return file_alloc_security(file);
}

static void selinux_file_free_security(struct file *file)
{
	file_free_security(file);
=======
	struct file_security_struct *fsec = selinux_file(file);
	u32 sid = current_sid();

	fsec->sid = sid;
	fsec->fown_sid = sid;

	return 0;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

/*
 * Check whether a task has the ioctl permission and cmd
 * operation to an inode.
 */
<<<<<<< HEAD
int ioctl_has_perm(const struct cred *cred, struct file *file,
		u32 requested, u16 cmd)
{
	struct common_audit_data ad;
	struct file_security_struct *fsec = file->f_security;
	struct inode *inode = file->f_path.dentry->d_inode;
	struct inode_security_struct *isec = inode->i_security;
	struct lsm_ioctlop_audit ioctl;
	u32 ssid = cred_sid(cred);
	struct selinux_audit_data sad = {0,};
=======
static int ioctl_has_perm(const struct cred *cred, struct file *file,
		u32 requested, u16 cmd)
{
	struct common_audit_data ad;
	struct file_security_struct *fsec = selinux_file(file);
	struct inode *inode = file_inode(file);
	struct inode_security_struct *isec;
	struct lsm_ioctlop_audit ioctl;
	u32 ssid = cred_sid(cred);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	int rc;
	u8 driver = cmd >> 8;
	u8 xperm = cmd & 0xff;

<<<<<<< HEAD
	COMMON_AUDIT_DATA_INIT(&ad, IOCTL_OP);
	ad.u.op = &ioctl;
	ad.u.op->cmd = cmd;
	ad.selinux_audit_data = &sad;
=======
	ad.type = LSM_AUDIT_DATA_IOCTL_OP;
	ad.u.op = &ioctl;
	ad.u.op->cmd = cmd;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	ad.u.op->path = file->f_path;

	if (ssid != fsec->sid) {
		rc = avc_has_perm(ssid, fsec->sid,
				SECCLASS_FD,
				FD__USE,
				&ad);
		if (rc)
			goto out;
	}

	if (unlikely(IS_PRIVATE(inode)))
		return 0;

<<<<<<< HEAD
	rc = avc_has_extended_perms(ssid, isec->sid, isec->sclass,
			requested, driver, xperm, &ad);
=======
	isec = inode_security(inode);
	rc = avc_has_extended_perms(ssid, isec->sid, isec->sclass,
				    requested, driver, xperm, &ad);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
out:
	return rc;
}

static int selinux_file_ioctl(struct file *file, unsigned int cmd,
			      unsigned long arg)
{
	const struct cred *cred = current_cred();
	int error = 0;

	switch (cmd) {
	case FIONREAD:
<<<<<<< HEAD
	/* fall through */
	case FIBMAP:
	/* fall through */
	case FIGETBSZ:
	/* fall through */
	case FS_IOC_GETFLAGS:
	/* fall through */
=======
	case FIBMAP:
	case FIGETBSZ:
	case FS_IOC_GETFLAGS:
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	case FS_IOC_GETVERSION:
		error = file_has_perm(cred, file, FILE__GETATTR);
		break;

	case FS_IOC_SETFLAGS:
<<<<<<< HEAD
	/* fall through */
=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	case FS_IOC_SETVERSION:
		error = file_has_perm(cred, file, FILE__SETATTR);
		break;

	/* sys_ioctl() checks */
	case FIONBIO:
<<<<<<< HEAD
	/* fall through */
=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	case FIOASYNC:
		error = file_has_perm(cred, file, 0);
		break;

	case KDSKBENT:
	case KDSKBSENT:
		error = cred_has_capability(cred, CAP_SYS_TTY_CONFIG,
<<<<<<< HEAD
					    SECURITY_CAP_AUDIT);
=======
					    CAP_OPT_NONE, true);
		break;

	case FIOCLEX:
	case FIONCLEX:
		if (!selinux_policycap_ioctl_skip_cloexec())
			error = ioctl_has_perm(cred, file, FILE__IOCTL, (u16) cmd);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		break;

	/* default case assumes that the command will go
	 * to the file's ioctl() function.
	 */
	default:
		error = ioctl_has_perm(cred, file, FILE__IOCTL, (u16) cmd);
	}
	return error;
}

<<<<<<< HEAD
static int default_noexec;
=======
static int selinux_file_ioctl_compat(struct file *file, unsigned int cmd,
			      unsigned long arg)
{
	/*
	 * If we are in a 64-bit kernel running 32-bit userspace, we need to
	 * make sure we don't compare 32-bit flags to 64-bit flags.
	 */
	switch (cmd) {
	case FS_IOC32_GETFLAGS:
		cmd = FS_IOC_GETFLAGS;
		break;
	case FS_IOC32_SETFLAGS:
		cmd = FS_IOC_SETFLAGS;
		break;
	case FS_IOC32_GETVERSION:
		cmd = FS_IOC_GETVERSION;
		break;
	case FS_IOC32_SETVERSION:
		cmd = FS_IOC_SETVERSION;
		break;
	default:
		break;
	}

	return selinux_file_ioctl(file, cmd, arg);
}

static int default_noexec __ro_after_init;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

static int file_map_prot_check(struct file *file, unsigned long prot, int shared)
{
	const struct cred *cred = current_cred();
<<<<<<< HEAD
	int rc = 0;

	if (default_noexec &&
	    (prot & PROT_EXEC) && (!file || (!shared && (prot & PROT_WRITE)))) {
=======
	u32 sid = cred_sid(cred);
	int rc = 0;

	if (default_noexec &&
	    (prot & PROT_EXEC) && (!file || IS_PRIVATE(file_inode(file)) ||
				   (!shared && (prot & PROT_WRITE)))) {
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		/*
		 * We are making executable an anonymous mapping or a
		 * private file mapping that will also be writable.
		 * This has an additional check.
		 */
<<<<<<< HEAD
		rc = cred_has_perm(cred, cred, PROCESS__EXECMEM);
=======
		rc = avc_has_perm(sid, sid, SECCLASS_PROCESS,
				  PROCESS__EXECMEM, NULL);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		if (rc)
			goto error;
	}

	if (file) {
		/* read access is always possible with a mapping */
		u32 av = FILE__READ;

		/* write access only matters if the mapping is shared */
		if (shared && (prot & PROT_WRITE))
			av |= FILE__WRITE;

		if (prot & PROT_EXEC)
			av |= FILE__EXECUTE;

		return file_has_perm(cred, file, av);
	}

error:
	return rc;
}

<<<<<<< HEAD
static int selinux_file_mmap(struct file *file, unsigned long reqprot,
			     unsigned long prot, unsigned long flags,
			     unsigned long addr, unsigned long addr_only)
{
	int rc = 0;
	u32 sid = current_sid();

	/*
	 * notice that we are intentionally putting the SELinux check before
	 * the secondary cap_file_mmap check.  This is such a likely attempt
	 * at bad behaviour/exploit that we always want to get the AVC, even
	 * if DAC would have also denied the operation.
	 */
	if (addr < CONFIG_LSM_MMAP_MIN_ADDR) {
		rc = avc_has_perm(sid, sid, SECCLASS_MEMPROTECT,
				  MEMPROTECT__MMAP_ZERO, NULL);
=======
static int selinux_mmap_addr(unsigned long addr)
{
	int rc = 0;

	if (addr < CONFIG_LSM_MMAP_MIN_ADDR) {
		u32 sid = current_sid();
		rc = avc_has_perm(sid, sid, SECCLASS_MEMPROTECT,
				  MEMPROTECT__MMAP_ZERO, NULL);
	}

	return rc;
}

static int selinux_mmap_file(struct file *file,
			     unsigned long reqprot __always_unused,
			     unsigned long prot, unsigned long flags)
{
	struct common_audit_data ad;
	int rc;

	if (file) {
		ad.type = LSM_AUDIT_DATA_FILE;
		ad.u.file = file;
		rc = inode_has_perm(current_cred(), file_inode(file),
				    FILE__MAP, &ad);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		if (rc)
			return rc;
	}

<<<<<<< HEAD
	/* do DAC check on address space usage */
	rc = cap_file_mmap(file, reqprot, prot, flags, addr, addr_only);
	if (rc || addr_only)
		return rc;

	if (selinux_checkreqprot)
		prot = reqprot;

=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	return file_map_prot_check(file, prot,
				   (flags & MAP_TYPE) == MAP_SHARED);
}

static int selinux_file_mprotect(struct vm_area_struct *vma,
<<<<<<< HEAD
				 unsigned long reqprot,
				 unsigned long prot)
{
	const struct cred *cred = current_cred();

	if (selinux_checkreqprot)
		prot = reqprot;
=======
				 unsigned long reqprot __always_unused,
				 unsigned long prot)
{
	const struct cred *cred = current_cred();
	u32 sid = cred_sid(cred);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	if (default_noexec &&
	    (prot & PROT_EXEC) && !(vma->vm_flags & VM_EXEC)) {
		int rc = 0;
<<<<<<< HEAD
		if (vma->vm_start >= vma->vm_mm->start_brk &&
		    vma->vm_end <= vma->vm_mm->brk) {
			rc = cred_has_perm(cred, cred, PROCESS__EXECHEAP);
		} else if (!vma->vm_file &&
			   vma->vm_start <= vma->vm_mm->start_stack &&
			   vma->vm_end >= vma->vm_mm->start_stack) {
			rc = current_has_perm(current, PROCESS__EXECSTACK);
=======
		if (vma_is_initial_heap(vma)) {
			rc = avc_has_perm(sid, sid, SECCLASS_PROCESS,
					  PROCESS__EXECHEAP, NULL);
		} else if (!vma->vm_file && (vma_is_initial_stack(vma) ||
			    vma_is_stack_for_current(vma))) {
			rc = avc_has_perm(sid, sid, SECCLASS_PROCESS,
					  PROCESS__EXECSTACK, NULL);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		} else if (vma->vm_file && vma->anon_vma) {
			/*
			 * We are making executable a file mapping that has
			 * had some COW done. Since pages might have been
			 * written, check ability to execute the possibly
			 * modified content.  This typically should only
			 * occur for text relocations.
			 */
			rc = file_has_perm(cred, vma->vm_file, FILE__EXECMOD);
		}
		if (rc)
			return rc;
	}

	return file_map_prot_check(vma->vm_file, prot, vma->vm_flags&VM_SHARED);
}

static int selinux_file_lock(struct file *file, unsigned int cmd)
{
	const struct cred *cred = current_cred();

	return file_has_perm(cred, file, FILE__LOCK);
}

static int selinux_file_fcntl(struct file *file, unsigned int cmd,
			      unsigned long arg)
{
	const struct cred *cred = current_cred();
	int err = 0;

	switch (cmd) {
	case F_SETFL:
<<<<<<< HEAD
		if (!file->f_path.dentry || !file->f_path.dentry->d_inode) {
			err = -EINVAL;
			break;
		}

=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		if ((file->f_flags & O_APPEND) && !(arg & O_APPEND)) {
			err = file_has_perm(cred, file, FILE__WRITE);
			break;
		}
<<<<<<< HEAD
		/* fall through */
=======
		fallthrough;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	case F_SETOWN:
	case F_SETSIG:
	case F_GETFL:
	case F_GETOWN:
	case F_GETSIG:
<<<<<<< HEAD
=======
	case F_GETOWNER_UIDS:
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		/* Just check FD__USE permission */
		err = file_has_perm(cred, file, 0);
		break;
	case F_GETLK:
	case F_SETLK:
	case F_SETLKW:
	case F_OFD_GETLK:
	case F_OFD_SETLK:
	case F_OFD_SETLKW:
#if BITS_PER_LONG == 32
	case F_GETLK64:
	case F_SETLK64:
	case F_SETLKW64:
#endif
<<<<<<< HEAD
		if (!file->f_path.dentry || !file->f_path.dentry->d_inode) {
			err = -EINVAL;
			break;
		}
=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		err = file_has_perm(cred, file, FILE__LOCK);
		break;
	}

	return err;
}

<<<<<<< HEAD
static int selinux_file_set_fowner(struct file *file)
{
	struct file_security_struct *fsec;

	fsec = file->f_security;
	fsec->fown_sid = current_sid();

	return 0;
=======
static void selinux_file_set_fowner(struct file *file)
{
	struct file_security_struct *fsec;

	fsec = selinux_file(file);
	fsec->fown_sid = current_sid();
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

static int selinux_file_send_sigiotask(struct task_struct *tsk,
				       struct fown_struct *fown, int signum)
{
	struct file *file;
<<<<<<< HEAD
	u32 sid = task_sid(tsk);
=======
	u32 sid = task_sid_obj(tsk);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	u32 perm;
	struct file_security_struct *fsec;

	/* struct fown_struct is never outside the context of a struct file */
	file = container_of(fown, struct file, f_owner);

<<<<<<< HEAD
	fsec = file->f_security;
=======
	fsec = selinux_file(file);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	if (!signum)
		perm = signal_to_av(SIGIO); /* as per send_sigio_to_task */
	else
		perm = signal_to_av(signum);

	return avc_has_perm(fsec->fown_sid, sid,
			    SECCLASS_PROCESS, perm, NULL);
}

static int selinux_file_receive(struct file *file)
{
	const struct cred *cred = current_cred();

	return file_has_perm(cred, file, file_to_av(file));
}

<<<<<<< HEAD
static int selinux_dentry_open(struct file *file, const struct cred *cred)
{
	struct file_security_struct *fsec;
	struct inode *inode;
	struct inode_security_struct *isec;
	int ret;

	ret = pft_file_open(file, cred);
	if (ret < 0)
		return ret;

	inode = file->f_path.dentry->d_inode;
	fsec = file->f_security;
	isec = inode->i_security;
=======
static int selinux_file_open(struct file *file)
{
	struct file_security_struct *fsec;
	struct inode_security_struct *isec;

	fsec = selinux_file(file);
	isec = inode_security(file_inode(file));
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	/*
	 * Save inode label and policy sequence number
	 * at open-time so that selinux_file_permission
	 * can determine whether revalidation is necessary.
	 * Task label is already saved in the file security
	 * struct as its SID.
	 */
	fsec->isid = isec->sid;
	fsec->pseqno = avc_policy_seqno();
	/*
	 * Since the inode label or policy seqno may have changed
	 * between the selinux_inode_permission check and the saving
	 * of state above, recheck that access is still permitted.
	 * Otherwise, access might never be revalidated against the
	 * new inode label or new policy.
	 * This check is not redundant - do not remove.
	 */
<<<<<<< HEAD
	return inode_has_perm_noadp(cred, inode, open_file_to_av(file), 0);
}

static int selinux_file_close(struct file *file)
{
	return pft_file_close(file);
}

static bool selinux_allow_merge_bio(struct bio *bio1, struct bio *bio2)
{
	return pft_allow_merge_bio(bio1, bio2);
=======
	return file_path_has_perm(file->f_cred, file, open_file_to_av(file));
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

/* task security operations */

<<<<<<< HEAD
static int selinux_task_create(unsigned long clone_flags)
{
	return current_has_perm(current, PROCESS__FORK);
}

/*
 * allocate the SELinux part of blank credentials
 */
static int selinux_cred_alloc_blank(struct cred *cred, gfp_t gfp)
{
	struct task_security_struct *tsec;

	tsec = kzalloc(sizeof(struct task_security_struct), gfp);
	if (!tsec)
		return -ENOMEM;

	cred->security = tsec;
	return 0;
}

/*
 * detach and free the LSM part of a set of credentials
 */
static void selinux_cred_free(struct cred *cred)
{
	struct task_security_struct *tsec = cred->security;

	/*
	 * cred->security == NULL if security_cred_alloc_blank() or
	 * security_prepare_creds() returned an error.
	 */
	BUG_ON(cred->security && (unsigned long) cred->security < PAGE_SIZE);
	cred->security = (void *) 0x7UL;
	kfree(tsec);
=======
static int selinux_task_alloc(struct task_struct *task,
			      unsigned long clone_flags)
{
	u32 sid = current_sid();

	return avc_has_perm(sid, sid, SECCLASS_PROCESS, PROCESS__FORK, NULL);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

/*
 * prepare a new set of credentials for modification
 */
static int selinux_cred_prepare(struct cred *new, const struct cred *old,
				gfp_t gfp)
{
<<<<<<< HEAD
	const struct task_security_struct *old_tsec;
	struct task_security_struct *tsec;

	old_tsec = old->security;

	tsec = kmemdup(old_tsec, sizeof(struct task_security_struct), gfp);
	if (!tsec)
		return -ENOMEM;

	new->security = tsec;
=======
	const struct task_security_struct *old_tsec = selinux_cred(old);
	struct task_security_struct *tsec = selinux_cred(new);

	*tsec = *old_tsec;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	return 0;
}

/*
 * transfer the SELinux data to a blank set of creds
 */
static void selinux_cred_transfer(struct cred *new, const struct cred *old)
{
<<<<<<< HEAD
	const struct task_security_struct *old_tsec = old->security;
	struct task_security_struct *tsec = new->security;
=======
	const struct task_security_struct *old_tsec = selinux_cred(old);
	struct task_security_struct *tsec = selinux_cred(new);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	*tsec = *old_tsec;
}

<<<<<<< HEAD
=======
static void selinux_cred_getsecid(const struct cred *c, u32 *secid)
{
	*secid = cred_sid(c);
}

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
/*
 * set the security data for a kernel service
 * - all the creation contexts are set to unlabelled
 */
static int selinux_kernel_act_as(struct cred *new, u32 secid)
{
<<<<<<< HEAD
	struct task_security_struct *tsec = new->security;
=======
	struct task_security_struct *tsec = selinux_cred(new);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	u32 sid = current_sid();
	int ret;

	ret = avc_has_perm(sid, secid,
			   SECCLASS_KERNEL_SERVICE,
			   KERNEL_SERVICE__USE_AS_OVERRIDE,
			   NULL);
	if (ret == 0) {
		tsec->sid = secid;
		tsec->create_sid = 0;
		tsec->keycreate_sid = 0;
		tsec->sockcreate_sid = 0;
	}
	return ret;
}

/*
 * set the file creation context in a security record to the same as the
 * objective context of the specified inode
 */
static int selinux_kernel_create_files_as(struct cred *new, struct inode *inode)
{
<<<<<<< HEAD
	struct inode_security_struct *isec = inode->i_security;
	struct task_security_struct *tsec = new->security;
=======
	struct inode_security_struct *isec = inode_security(inode);
	struct task_security_struct *tsec = selinux_cred(new);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	u32 sid = current_sid();
	int ret;

	ret = avc_has_perm(sid, isec->sid,
			   SECCLASS_KERNEL_SERVICE,
			   KERNEL_SERVICE__CREATE_FILES_AS,
			   NULL);

	if (ret == 0)
		tsec->create_sid = isec->sid;
	return ret;
}

static int selinux_kernel_module_request(char *kmod_name)
{
<<<<<<< HEAD
	u32 sid;
	struct common_audit_data ad;
	struct selinux_audit_data sad = {0,};

	sid = task_sid(current);

	COMMON_AUDIT_DATA_INIT(&ad, KMOD);
	ad.selinux_audit_data = &sad;
	ad.u.kmod_name = kmod_name;

	return avc_has_perm(sid, SECINITSID_KERNEL, SECCLASS_SYSTEM,
			    SYSTEM__MODULE_REQUEST, &ad);
}

static int selinux_task_setpgid(struct task_struct *p, pid_t pgid)
{
	return current_has_perm(p, PROCESS__SETPGID);
=======
	struct common_audit_data ad;

	ad.type = LSM_AUDIT_DATA_KMOD;
	ad.u.kmod_name = kmod_name;

	return avc_has_perm(current_sid(), SECINITSID_KERNEL, SECCLASS_SYSTEM,
			    SYSTEM__MODULE_REQUEST, &ad);
}

static int selinux_kernel_module_from_file(struct file *file)
{
	struct common_audit_data ad;
	struct inode_security_struct *isec;
	struct file_security_struct *fsec;
	u32 sid = current_sid();
	int rc;

	/* init_module */
	if (file == NULL)
		return avc_has_perm(sid, sid, SECCLASS_SYSTEM,
					SYSTEM__MODULE_LOAD, NULL);

	/* finit_module */

	ad.type = LSM_AUDIT_DATA_FILE;
	ad.u.file = file;

	fsec = selinux_file(file);
	if (sid != fsec->sid) {
		rc = avc_has_perm(sid, fsec->sid, SECCLASS_FD, FD__USE, &ad);
		if (rc)
			return rc;
	}

	isec = inode_security(file_inode(file));
	return avc_has_perm(sid, isec->sid, SECCLASS_SYSTEM,
				SYSTEM__MODULE_LOAD, &ad);
}

static int selinux_kernel_read_file(struct file *file,
				    enum kernel_read_file_id id,
				    bool contents)
{
	int rc = 0;

	switch (id) {
	case READING_MODULE:
		rc = selinux_kernel_module_from_file(contents ? file : NULL);
		break;
	default:
		break;
	}

	return rc;
}

static int selinux_kernel_load_data(enum kernel_load_data_id id, bool contents)
{
	int rc = 0;

	switch (id) {
	case LOADING_MODULE:
		rc = selinux_kernel_module_from_file(NULL);
		break;
	default:
		break;
	}

	return rc;
}

static int selinux_task_setpgid(struct task_struct *p, pid_t pgid)
{
	return avc_has_perm(current_sid(), task_sid_obj(p), SECCLASS_PROCESS,
			    PROCESS__SETPGID, NULL);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

static int selinux_task_getpgid(struct task_struct *p)
{
<<<<<<< HEAD
	return current_has_perm(p, PROCESS__GETPGID);
=======
	return avc_has_perm(current_sid(), task_sid_obj(p), SECCLASS_PROCESS,
			    PROCESS__GETPGID, NULL);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

static int selinux_task_getsid(struct task_struct *p)
{
<<<<<<< HEAD
	return current_has_perm(p, PROCESS__GETSESSION);
}

static void selinux_task_getsecid(struct task_struct *p, u32 *secid)
{
	*secid = task_sid(p);
=======
	return avc_has_perm(current_sid(), task_sid_obj(p), SECCLASS_PROCESS,
			    PROCESS__GETSESSION, NULL);
}

static void selinux_current_getsecid_subj(u32 *secid)
{
	*secid = current_sid();
}

static void selinux_task_getsecid_obj(struct task_struct *p, u32 *secid)
{
	*secid = task_sid_obj(p);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

static int selinux_task_setnice(struct task_struct *p, int nice)
{
<<<<<<< HEAD
	int rc;

	rc = cap_task_setnice(p, nice);
	if (rc)
		return rc;

	return current_has_perm(p, PROCESS__SETSCHED);
=======
	return avc_has_perm(current_sid(), task_sid_obj(p), SECCLASS_PROCESS,
			    PROCESS__SETSCHED, NULL);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

static int selinux_task_setioprio(struct task_struct *p, int ioprio)
{
<<<<<<< HEAD
	int rc;

	rc = cap_task_setioprio(p, ioprio);
	if (rc)
		return rc;

	return current_has_perm(p, PROCESS__SETSCHED);
=======
	return avc_has_perm(current_sid(), task_sid_obj(p), SECCLASS_PROCESS,
			    PROCESS__SETSCHED, NULL);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

static int selinux_task_getioprio(struct task_struct *p)
{
<<<<<<< HEAD
	return current_has_perm(p, PROCESS__GETSCHED);
=======
	return avc_has_perm(current_sid(), task_sid_obj(p), SECCLASS_PROCESS,
			    PROCESS__GETSCHED, NULL);
}

static int selinux_task_prlimit(const struct cred *cred, const struct cred *tcred,
				unsigned int flags)
{
	u32 av = 0;

	if (!flags)
		return 0;
	if (flags & LSM_PRLIMIT_WRITE)
		av |= PROCESS__SETRLIMIT;
	if (flags & LSM_PRLIMIT_READ)
		av |= PROCESS__GETRLIMIT;
	return avc_has_perm(cred_sid(cred), cred_sid(tcred),
			    SECCLASS_PROCESS, av, NULL);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

static int selinux_task_setrlimit(struct task_struct *p, unsigned int resource,
		struct rlimit *new_rlim)
{
	struct rlimit *old_rlim = p->signal->rlim + resource;

	/* Control the ability to change the hard limit (whether
	   lowering or raising it), so that the hard limit can
	   later be used as a safe reset point for the soft limit
	   upon context transitions.  See selinux_bprm_committing_creds. */
	if (old_rlim->rlim_max != new_rlim->rlim_max)
<<<<<<< HEAD
		return current_has_perm(p, PROCESS__SETRLIMIT);
=======
		return avc_has_perm(current_sid(), task_sid_obj(p),
				    SECCLASS_PROCESS, PROCESS__SETRLIMIT, NULL);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	return 0;
}

static int selinux_task_setscheduler(struct task_struct *p)
{
<<<<<<< HEAD
	int rc;

	rc = cap_task_setscheduler(p);
	if (rc)
		return rc;

	return current_has_perm(p, PROCESS__SETSCHED);
=======
	return avc_has_perm(current_sid(), task_sid_obj(p), SECCLASS_PROCESS,
			    PROCESS__SETSCHED, NULL);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

static int selinux_task_getscheduler(struct task_struct *p)
{
<<<<<<< HEAD
	return current_has_perm(p, PROCESS__GETSCHED);
=======
	return avc_has_perm(current_sid(), task_sid_obj(p), SECCLASS_PROCESS,
			    PROCESS__GETSCHED, NULL);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

static int selinux_task_movememory(struct task_struct *p)
{
<<<<<<< HEAD
	return current_has_perm(p, PROCESS__SETSCHED);
}

static int selinux_task_kill(struct task_struct *p, struct siginfo *info,
				int sig, u32 secid)
{
	u32 perm;
	int rc;
=======
	return avc_has_perm(current_sid(), task_sid_obj(p), SECCLASS_PROCESS,
			    PROCESS__SETSCHED, NULL);
}

static int selinux_task_kill(struct task_struct *p, struct kernel_siginfo *info,
				int sig, const struct cred *cred)
{
	u32 secid;
	u32 perm;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	if (!sig)
		perm = PROCESS__SIGNULL; /* null signal; existence test */
	else
		perm = signal_to_av(sig);
<<<<<<< HEAD
	if (secid)
		rc = avc_has_perm(secid, task_sid(p),
				  SECCLASS_PROCESS, perm, NULL);
	else
		rc = current_has_perm(p, perm);
	return rc;
}

static int selinux_task_wait(struct task_struct *p)
{
	return task_has_perm(p, current, PROCESS__SIGCHLD);
=======
	if (!cred)
		secid = current_sid();
	else
		secid = cred_sid(cred);
	return avc_has_perm(secid, task_sid_obj(p), SECCLASS_PROCESS, perm, NULL);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

static void selinux_task_to_inode(struct task_struct *p,
				  struct inode *inode)
{
<<<<<<< HEAD
	struct inode_security_struct *isec = inode->i_security;
	u32 sid = task_sid(p);

	isec->sid = sid;
	isec->initialized = 1;
=======
	struct inode_security_struct *isec = selinux_inode(inode);
	u32 sid = task_sid_obj(p);

	spin_lock(&isec->lock);
	isec->sclass = inode_mode_to_security_class(inode->i_mode);
	isec->sid = sid;
	isec->initialized = LABEL_INITIALIZED;
	spin_unlock(&isec->lock);
}

static int selinux_userns_create(const struct cred *cred)
{
	u32 sid = current_sid();

	return avc_has_perm(sid, sid, SECCLASS_USER_NAMESPACE,
			USER_NAMESPACE__CREATE, NULL);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

/* Returns error only if unable to parse addresses */
static int selinux_parse_skb_ipv4(struct sk_buff *skb,
			struct common_audit_data *ad, u8 *proto)
{
	int offset, ihlen, ret = -EINVAL;
	struct iphdr _iph, *ih;

	offset = skb_network_offset(skb);
	ih = skb_header_pointer(skb, offset, sizeof(_iph), &_iph);
	if (ih == NULL)
		goto out;

	ihlen = ih->ihl * 4;
	if (ihlen < sizeof(_iph))
		goto out;

	ad->u.net->v4info.saddr = ih->saddr;
	ad->u.net->v4info.daddr = ih->daddr;
	ret = 0;

	if (proto)
		*proto = ih->protocol;

	switch (ih->protocol) {
	case IPPROTO_TCP: {
		struct tcphdr _tcph, *th;

		if (ntohs(ih->frag_off) & IP_OFFSET)
			break;

		offset += ihlen;
		th = skb_header_pointer(skb, offset, sizeof(_tcph), &_tcph);
		if (th == NULL)
			break;

		ad->u.net->sport = th->source;
		ad->u.net->dport = th->dest;
		break;
	}

	case IPPROTO_UDP: {
		struct udphdr _udph, *uh;

		if (ntohs(ih->frag_off) & IP_OFFSET)
			break;

		offset += ihlen;
		uh = skb_header_pointer(skb, offset, sizeof(_udph), &_udph);
		if (uh == NULL)
			break;

		ad->u.net->sport = uh->source;
		ad->u.net->dport = uh->dest;
		break;
	}

	case IPPROTO_DCCP: {
		struct dccp_hdr _dccph, *dh;

		if (ntohs(ih->frag_off) & IP_OFFSET)
			break;

		offset += ihlen;
		dh = skb_header_pointer(skb, offset, sizeof(_dccph), &_dccph);
		if (dh == NULL)
			break;

		ad->u.net->sport = dh->dccph_sport;
		ad->u.net->dport = dh->dccph_dport;
		break;
	}

<<<<<<< HEAD
=======
#if IS_ENABLED(CONFIG_IP_SCTP)
	case IPPROTO_SCTP: {
		struct sctphdr _sctph, *sh;

		if (ntohs(ih->frag_off) & IP_OFFSET)
			break;

		offset += ihlen;
		sh = skb_header_pointer(skb, offset, sizeof(_sctph), &_sctph);
		if (sh == NULL)
			break;

		ad->u.net->sport = sh->source;
		ad->u.net->dport = sh->dest;
		break;
	}
#endif
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	default:
		break;
	}
out:
	return ret;
}

<<<<<<< HEAD
#if defined(CONFIG_IPV6) || defined(CONFIG_IPV6_MODULE)
=======
#if IS_ENABLED(CONFIG_IPV6)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

/* Returns error only if unable to parse addresses */
static int selinux_parse_skb_ipv6(struct sk_buff *skb,
			struct common_audit_data *ad, u8 *proto)
{
	u8 nexthdr;
	int ret = -EINVAL, offset;
	struct ipv6hdr _ipv6h, *ip6;
	__be16 frag_off;

	offset = skb_network_offset(skb);
	ip6 = skb_header_pointer(skb, offset, sizeof(_ipv6h), &_ipv6h);
	if (ip6 == NULL)
		goto out;

	ad->u.net->v6info.saddr = ip6->saddr;
	ad->u.net->v6info.daddr = ip6->daddr;
	ret = 0;

	nexthdr = ip6->nexthdr;
	offset += sizeof(_ipv6h);
	offset = ipv6_skip_exthdr(skb, offset, &nexthdr, &frag_off);
	if (offset < 0)
		goto out;

	if (proto)
		*proto = nexthdr;

	switch (nexthdr) {
	case IPPROTO_TCP: {
		struct tcphdr _tcph, *th;

		th = skb_header_pointer(skb, offset, sizeof(_tcph), &_tcph);
		if (th == NULL)
			break;

		ad->u.net->sport = th->source;
		ad->u.net->dport = th->dest;
		break;
	}

	case IPPROTO_UDP: {
		struct udphdr _udph, *uh;

		uh = skb_header_pointer(skb, offset, sizeof(_udph), &_udph);
		if (uh == NULL)
			break;

		ad->u.net->sport = uh->source;
		ad->u.net->dport = uh->dest;
		break;
	}

	case IPPROTO_DCCP: {
		struct dccp_hdr _dccph, *dh;

		dh = skb_header_pointer(skb, offset, sizeof(_dccph), &_dccph);
		if (dh == NULL)
			break;

		ad->u.net->sport = dh->dccph_sport;
		ad->u.net->dport = dh->dccph_dport;
		break;
	}

<<<<<<< HEAD
=======
#if IS_ENABLED(CONFIG_IP_SCTP)
	case IPPROTO_SCTP: {
		struct sctphdr _sctph, *sh;

		sh = skb_header_pointer(skb, offset, sizeof(_sctph), &_sctph);
		if (sh == NULL)
			break;

		ad->u.net->sport = sh->source;
		ad->u.net->dport = sh->dest;
		break;
	}
#endif
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	/* includes fragments */
	default:
		break;
	}
out:
	return ret;
}

#endif /* IPV6 */

static int selinux_parse_skb(struct sk_buff *skb, struct common_audit_data *ad,
			     char **_addrp, int src, u8 *proto)
{
	char *addrp;
	int ret;

	switch (ad->u.net->family) {
	case PF_INET:
		ret = selinux_parse_skb_ipv4(skb, ad, proto);
		if (ret)
			goto parse_error;
		addrp = (char *)(src ? &ad->u.net->v4info.saddr :
				       &ad->u.net->v4info.daddr);
		goto okay;

<<<<<<< HEAD
#if defined(CONFIG_IPV6) || defined(CONFIG_IPV6_MODULE)
=======
#if IS_ENABLED(CONFIG_IPV6)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	case PF_INET6:
		ret = selinux_parse_skb_ipv6(skb, ad, proto);
		if (ret)
			goto parse_error;
		addrp = (char *)(src ? &ad->u.net->v6info.saddr :
				       &ad->u.net->v6info.daddr);
		goto okay;
#endif	/* IPV6 */
	default:
		addrp = NULL;
		goto okay;
	}

parse_error:
<<<<<<< HEAD
	printk(KERN_WARNING
=======
	pr_warn(
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	       "SELinux: failure in selinux_parse_skb(),"
	       " unable to parse packet\n");
	return ret;

okay:
	if (_addrp)
		*_addrp = addrp;
	return 0;
}

/**
 * selinux_skb_peerlbl_sid - Determine the peer label of a packet
 * @skb: the packet
 * @family: protocol family
 * @sid: the packet's peer label SID
 *
 * Description:
 * Check the various different forms of network peer labeling and determine
 * the peer label/SID for the packet; most of the magic actually occurs in
 * the security server function security_net_peersid_cmp().  The function
 * returns zero if the value in @sid is valid (although it may be SECSID_NULL)
 * or -EACCES if @sid is invalid due to inconsistencies with the different
 * peer labels.
 *
 */
static int selinux_skb_peerlbl_sid(struct sk_buff *skb, u16 family, u32 *sid)
{
	int err;
	u32 xfrm_sid;
	u32 nlbl_sid;
	u32 nlbl_type;

<<<<<<< HEAD
	selinux_xfrm_skb_sid(skb, &xfrm_sid);
	selinux_netlbl_skbuff_getsid(skb, family, &nlbl_type, &nlbl_sid);

	err = security_net_peersid_resolve(nlbl_sid, nlbl_type, xfrm_sid, sid);
	if (unlikely(err)) {
		printk(KERN_WARNING
=======
	err = selinux_xfrm_skb_sid(skb, &xfrm_sid);
	if (unlikely(err))
		return -EACCES;
	err = selinux_netlbl_skbuff_getsid(skb, family, &nlbl_type, &nlbl_sid);
	if (unlikely(err))
		return -EACCES;

	err = security_net_peersid_resolve(nlbl_sid,
					   nlbl_type, xfrm_sid, sid);
	if (unlikely(err)) {
		pr_warn(
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		       "SELinux: failure in selinux_skb_peerlbl_sid(),"
		       " unable to determine packet's peer label\n");
		return -EACCES;
	}

	return 0;
}

/**
 * selinux_conn_sid - Determine the child socket label for a connection
 * @sk_sid: the parent socket's SID
 * @skb_sid: the packet's SID
 * @conn_sid: the resulting connection SID
 *
 * If @skb_sid is valid then the user:role:type information from @sk_sid is
 * combined with the MLS information from @skb_sid in order to create
<<<<<<< HEAD
 * @conn_sid.  If @skb_sid is not valid then then @conn_sid is simply a copy
=======
 * @conn_sid.  If @skb_sid is not valid then @conn_sid is simply a copy
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 * of @sk_sid.  Returns zero on success, negative values on failure.
 *
 */
static int selinux_conn_sid(u32 sk_sid, u32 skb_sid, u32 *conn_sid)
{
	int err = 0;

	if (skb_sid != SECSID_NULL)
<<<<<<< HEAD
		err = security_sid_mls_copy(sk_sid, skb_sid, conn_sid);
=======
		err = security_sid_mls_copy(sk_sid, skb_sid,
					    conn_sid);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	else
		*conn_sid = sk_sid;

	return err;
}

/* socket security operations */

static int socket_sockcreate_sid(const struct task_security_struct *tsec,
				 u16 secclass, u32 *socksid)
{
	if (tsec->sockcreate_sid > SECSID_NULL) {
		*socksid = tsec->sockcreate_sid;
		return 0;
	}

<<<<<<< HEAD
	return security_transition_sid(tsec->sid, tsec->sid, secclass, NULL,
				       socksid);
}

static int sock_has_perm(struct task_struct *task, struct sock *sk, u32 perms)
{
	struct sk_security_struct *sksec = sk->sk_security;
	struct common_audit_data ad;
	struct selinux_audit_data sad = {0,};
	struct lsm_network_audit net = {0,};
	u32 tsid = task_sid(task);

	if (unlikely(!sksec)) {
		pr_warn("SELinux: sksec is NULL, socket is already freed\n");
		return -EINVAL;
	}
=======
	return security_transition_sid(tsec->sid, tsec->sid,
				       secclass, NULL, socksid);
}

static int sock_has_perm(struct sock *sk, u32 perms)
{
	struct sk_security_struct *sksec = sk->sk_security;
	struct common_audit_data ad;
	struct lsm_network_audit net;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	if (sksec->sid == SECINITSID_KERNEL)
		return 0;

<<<<<<< HEAD
	COMMON_AUDIT_DATA_INIT(&ad, NET);
	ad.selinux_audit_data = &sad;
	ad.u.net = &net;
	ad.u.net->sk = sk;

	return avc_has_perm(tsid, sksec->sid, sksec->sclass, perms, &ad);
=======
	/*
	 * Before POLICYDB_CAP_USERSPACE_INITIAL_CONTEXT, sockets that
	 * inherited the kernel context from early boot used to be skipped
	 * here, so preserve that behavior unless the capability is set.
	 *
	 * By setting the capability the policy signals that it is ready
	 * for this quirk to be fixed. Note that sockets created by a kernel
	 * thread or a usermode helper executed without a transition will
	 * still be skipped in this check regardless of the policycap
	 * setting.
	 */
	if (!selinux_policycap_userspace_initial_context() &&
	    sksec->sid == SECINITSID_INIT)
		return 0;

	ad_net_init_from_sk(&ad, &net, sk);

	return avc_has_perm(current_sid(), sksec->sid, sksec->sclass, perms,
			    &ad);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

static int selinux_socket_create(int family, int type,
				 int protocol, int kern)
{
<<<<<<< HEAD
	const struct task_security_struct *tsec = current_security();
=======
	const struct task_security_struct *tsec = selinux_cred(current_cred());
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	u32 newsid;
	u16 secclass;
	int rc;

	if (kern)
		return 0;

	secclass = socket_type_to_security_class(family, type, protocol);
	rc = socket_sockcreate_sid(tsec, secclass, &newsid);
	if (rc)
		return rc;

	return avc_has_perm(tsec->sid, newsid, secclass, SOCKET__CREATE, NULL);
}

static int selinux_socket_post_create(struct socket *sock, int family,
				      int type, int protocol, int kern)
{
<<<<<<< HEAD
	const struct task_security_struct *tsec = current_security();
	struct inode_security_struct *isec = SOCK_INODE(sock)->i_security;
	struct sk_security_struct *sksec;
	int err = 0;

	isec->sclass = socket_type_to_security_class(family, type, protocol);

	if (kern)
		isec->sid = SECINITSID_KERNEL;
	else {
		err = socket_sockcreate_sid(tsec, isec->sclass, &(isec->sid));
=======
	const struct task_security_struct *tsec = selinux_cred(current_cred());
	struct inode_security_struct *isec = inode_security_novalidate(SOCK_INODE(sock));
	struct sk_security_struct *sksec;
	u16 sclass = socket_type_to_security_class(family, type, protocol);
	u32 sid = SECINITSID_KERNEL;
	int err = 0;

	if (!kern) {
		err = socket_sockcreate_sid(tsec, sclass, &sid);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		if (err)
			return err;
	}

<<<<<<< HEAD
	isec->initialized = 1;

	if (sock->sk) {
		sksec = sock->sk->sk_security;
		sksec->sid = isec->sid;
		sksec->sclass = isec->sclass;
=======
	isec->sclass = sclass;
	isec->sid = sid;
	isec->initialized = LABEL_INITIALIZED;

	if (sock->sk) {
		sksec = sock->sk->sk_security;
		sksec->sclass = sclass;
		sksec->sid = sid;
		/* Allows detection of the first association on this socket */
		if (sksec->sclass == SECCLASS_SCTP_SOCKET)
			sksec->sctp_assoc_state = SCTP_ASSOC_UNSET;

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		err = selinux_netlbl_socket_post_create(sock->sk, family);
	}

	return err;
}

<<<<<<< HEAD
=======
static int selinux_socket_socketpair(struct socket *socka,
				     struct socket *sockb)
{
	struct sk_security_struct *sksec_a = socka->sk->sk_security;
	struct sk_security_struct *sksec_b = sockb->sk->sk_security;

	sksec_a->peer_sid = sksec_b->sid;
	sksec_b->peer_sid = sksec_a->sid;

	return 0;
}

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
/* Range of port numbers used to automatically bind.
   Need to determine whether we should perform a name_bind
   permission check between the socket and the port number. */

static int selinux_socket_bind(struct socket *sock, struct sockaddr *address, int addrlen)
{
	struct sock *sk = sock->sk;
<<<<<<< HEAD
	u16 family;
	int err;

	err = sock_has_perm(current, sk, SOCKET__BIND);
	if (err)
		goto out;

	/*
	 * If PF_INET or PF_INET6, check name_bind permission for the port.
	 * Multiple address binding for SCTP is not supported yet: we just
	 * check the first address now.
	 */
	family = sk->sk_family;
	if (family == PF_INET || family == PF_INET6) {
		char *addrp;
		struct sk_security_struct *sksec = sk->sk_security;
		struct common_audit_data ad;
		struct selinux_audit_data sad = {0,};
		struct lsm_network_audit net = {0,};
		struct sockaddr_in *addr4 = NULL;
		struct sockaddr_in6 *addr6 = NULL;
		unsigned short snum;
		u32 sid, node_perm;

		if (family == PF_INET) {
			addr4 = (struct sockaddr_in *)address;
			snum = ntohs(addr4->sin_port);
			addrp = (char *)&addr4->sin_addr.s_addr;
		} else {
			addr6 = (struct sockaddr_in6 *)address;
			snum = ntohs(addr6->sin6_port);
			addrp = (char *)&addr6->sin6_addr.s6_addr;
		}

		if (snum) {
			int low, high;

			inet_get_local_port_range(&low, &high);

			if (snum < max(PROT_SOCK, low) || snum > high) {
=======
	struct sk_security_struct *sksec = sk->sk_security;
	u16 family;
	int err;

	err = sock_has_perm(sk, SOCKET__BIND);
	if (err)
		goto out;

	/* If PF_INET or PF_INET6, check name_bind permission for the port. */
	family = sk->sk_family;
	if (family == PF_INET || family == PF_INET6) {
		char *addrp;
		struct common_audit_data ad;
		struct lsm_network_audit net = {0,};
		struct sockaddr_in *addr4 = NULL;
		struct sockaddr_in6 *addr6 = NULL;
		u16 family_sa;
		unsigned short snum;
		u32 sid, node_perm;

		/*
		 * sctp_bindx(3) calls via selinux_sctp_bind_connect()
		 * that validates multiple binding addresses. Because of this
		 * need to check address->sa_family as it is possible to have
		 * sk->sk_family = PF_INET6 with addr->sa_family = AF_INET.
		 */
		if (addrlen < offsetofend(struct sockaddr, sa_family))
			return -EINVAL;
		family_sa = address->sa_family;
		switch (family_sa) {
		case AF_UNSPEC:
		case AF_INET:
			if (addrlen < sizeof(struct sockaddr_in))
				return -EINVAL;
			addr4 = (struct sockaddr_in *)address;
			if (family_sa == AF_UNSPEC) {
				if (family == PF_INET6) {
					/* Length check from inet6_bind_sk() */
					if (addrlen < SIN6_LEN_RFC2133)
						return -EINVAL;
					/* Family check from __inet6_bind() */
					goto err_af;
				}
				/* see __inet_bind(), we only want to allow
				 * AF_UNSPEC if the address is INADDR_ANY
				 */
				if (addr4->sin_addr.s_addr != htonl(INADDR_ANY))
					goto err_af;
				family_sa = AF_INET;
			}
			snum = ntohs(addr4->sin_port);
			addrp = (char *)&addr4->sin_addr.s_addr;
			break;
		case AF_INET6:
			if (addrlen < SIN6_LEN_RFC2133)
				return -EINVAL;
			addr6 = (struct sockaddr_in6 *)address;
			snum = ntohs(addr6->sin6_port);
			addrp = (char *)&addr6->sin6_addr.s6_addr;
			break;
		default:
			goto err_af;
		}

		ad.type = LSM_AUDIT_DATA_NET;
		ad.u.net = &net;
		ad.u.net->sport = htons(snum);
		ad.u.net->family = family_sa;

		if (snum) {
			int low, high;

			inet_get_local_port_range(sock_net(sk), &low, &high);

			if (inet_port_requires_bind_service(sock_net(sk), snum) ||
			    snum < low || snum > high) {
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
				err = sel_netport_sid(sk->sk_protocol,
						      snum, &sid);
				if (err)
					goto out;
<<<<<<< HEAD
				COMMON_AUDIT_DATA_INIT(&ad, NET);
				ad.selinux_audit_data = &sad;
				ad.u.net = &net;
				ad.u.net->sport = htons(snum);
				ad.u.net->family = family;
=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
				err = avc_has_perm(sksec->sid, sid,
						   sksec->sclass,
						   SOCKET__NAME_BIND, &ad);
				if (err)
					goto out;
			}
		}

		switch (sksec->sclass) {
		case SECCLASS_TCP_SOCKET:
			node_perm = TCP_SOCKET__NODE_BIND;
			break;

		case SECCLASS_UDP_SOCKET:
			node_perm = UDP_SOCKET__NODE_BIND;
			break;

		case SECCLASS_DCCP_SOCKET:
			node_perm = DCCP_SOCKET__NODE_BIND;
			break;

<<<<<<< HEAD
=======
		case SECCLASS_SCTP_SOCKET:
			node_perm = SCTP_SOCKET__NODE_BIND;
			break;

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		default:
			node_perm = RAWIP_SOCKET__NODE_BIND;
			break;
		}

<<<<<<< HEAD
		err = sel_netnode_sid(addrp, family, &sid);
		if (err)
			goto out;

		COMMON_AUDIT_DATA_INIT(&ad, NET);
		ad.selinux_audit_data = &sad;
		ad.u.net = &net;
		ad.u.net->sport = htons(snum);
		ad.u.net->family = family;

		if (family == PF_INET)
=======
		err = sel_netnode_sid(addrp, family_sa, &sid);
		if (err)
			goto out;

		if (family_sa == AF_INET)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
			ad.u.net->v4info.saddr = addr4->sin_addr.s_addr;
		else
			ad.u.net->v6info.saddr = addr6->sin6_addr;

		err = avc_has_perm(sksec->sid, sid,
				   sksec->sclass, node_perm, &ad);
		if (err)
			goto out;
	}
out:
	return err;
<<<<<<< HEAD
}

static int selinux_socket_connect(struct socket *sock, struct sockaddr *address, int addrlen)
=======
err_af:
	/* Note that SCTP services expect -EINVAL, others -EAFNOSUPPORT. */
	if (sksec->sclass == SECCLASS_SCTP_SOCKET)
		return -EINVAL;
	return -EAFNOSUPPORT;
}

/* This supports connect(2) and SCTP connect services such as sctp_connectx(3)
 * and sctp_sendmsg(3) as described in Documentation/security/SCTP.rst
 */
static int selinux_socket_connect_helper(struct socket *sock,
					 struct sockaddr *address, int addrlen)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
{
	struct sock *sk = sock->sk;
	struct sk_security_struct *sksec = sk->sk_security;
	int err;

<<<<<<< HEAD
	err = sock_has_perm(current, sk, SOCKET__CONNECT);
	if (err)
		return err;

	/*
	 * If a TCP or DCCP socket, check name_connect permission for the port.
	 */
	if (sksec->sclass == SECCLASS_TCP_SOCKET ||
	    sksec->sclass == SECCLASS_DCCP_SOCKET) {
		struct common_audit_data ad;
		struct selinux_audit_data sad = {0,};
=======
	err = sock_has_perm(sk, SOCKET__CONNECT);
	if (err)
		return err;
	if (addrlen < offsetofend(struct sockaddr, sa_family))
		return -EINVAL;

	/* connect(AF_UNSPEC) has special handling, as it is a documented
	 * way to disconnect the socket
	 */
	if (address->sa_family == AF_UNSPEC)
		return 0;

	/*
	 * If a TCP, DCCP or SCTP socket, check name_connect permission
	 * for the port.
	 */
	if (sksec->sclass == SECCLASS_TCP_SOCKET ||
	    sksec->sclass == SECCLASS_DCCP_SOCKET ||
	    sksec->sclass == SECCLASS_SCTP_SOCKET) {
		struct common_audit_data ad;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		struct lsm_network_audit net = {0,};
		struct sockaddr_in *addr4 = NULL;
		struct sockaddr_in6 *addr6 = NULL;
		unsigned short snum;
		u32 sid, perm;

<<<<<<< HEAD
		if (sk->sk_family == PF_INET) {
=======
		/* sctp_connectx(3) calls via selinux_sctp_bind_connect()
		 * that validates multiple connect addresses. Because of this
		 * need to check address->sa_family as it is possible to have
		 * sk->sk_family = PF_INET6 with addr->sa_family = AF_INET.
		 */
		switch (address->sa_family) {
		case AF_INET:
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
			addr4 = (struct sockaddr_in *)address;
			if (addrlen < sizeof(struct sockaddr_in))
				return -EINVAL;
			snum = ntohs(addr4->sin_port);
<<<<<<< HEAD
		} else {
=======
			break;
		case AF_INET6:
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
			addr6 = (struct sockaddr_in6 *)address;
			if (addrlen < SIN6_LEN_RFC2133)
				return -EINVAL;
			snum = ntohs(addr6->sin6_port);
<<<<<<< HEAD
=======
			break;
		default:
			/* Note that SCTP services expect -EINVAL, whereas
			 * others expect -EAFNOSUPPORT.
			 */
			if (sksec->sclass == SECCLASS_SCTP_SOCKET)
				return -EINVAL;
			else
				return -EAFNOSUPPORT;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		}

		err = sel_netport_sid(sk->sk_protocol, snum, &sid);
		if (err)
<<<<<<< HEAD
			goto out;

		perm = (sksec->sclass == SECCLASS_TCP_SOCKET) ?
		       TCP_SOCKET__NAME_CONNECT : DCCP_SOCKET__NAME_CONNECT;

		COMMON_AUDIT_DATA_INIT(&ad, NET);
		ad.selinux_audit_data = &sad;
		ad.u.net = &net;
		ad.u.net->dport = htons(snum);
		ad.u.net->family = sk->sk_family;
		err = avc_has_perm(sksec->sid, sid, sksec->sclass, perm, &ad);
		if (err)
			goto out;
	}

	err = selinux_netlbl_socket_connect(sk, address);

out:
	return err;
=======
			return err;

		switch (sksec->sclass) {
		case SECCLASS_TCP_SOCKET:
			perm = TCP_SOCKET__NAME_CONNECT;
			break;
		case SECCLASS_DCCP_SOCKET:
			perm = DCCP_SOCKET__NAME_CONNECT;
			break;
		case SECCLASS_SCTP_SOCKET:
			perm = SCTP_SOCKET__NAME_CONNECT;
			break;
		}

		ad.type = LSM_AUDIT_DATA_NET;
		ad.u.net = &net;
		ad.u.net->dport = htons(snum);
		ad.u.net->family = address->sa_family;
		err = avc_has_perm(sksec->sid, sid, sksec->sclass, perm, &ad);
		if (err)
			return err;
	}

	return 0;
}

/* Supports connect(2), see comments in selinux_socket_connect_helper() */
static int selinux_socket_connect(struct socket *sock,
				  struct sockaddr *address, int addrlen)
{
	int err;
	struct sock *sk = sock->sk;

	err = selinux_socket_connect_helper(sock, address, addrlen);
	if (err)
		return err;

	return selinux_netlbl_socket_connect(sk, address);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

static int selinux_socket_listen(struct socket *sock, int backlog)
{
<<<<<<< HEAD
	return sock_has_perm(current, sock->sk, SOCKET__LISTEN);
=======
	return sock_has_perm(sock->sk, SOCKET__LISTEN);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

static int selinux_socket_accept(struct socket *sock, struct socket *newsock)
{
	int err;
	struct inode_security_struct *isec;
	struct inode_security_struct *newisec;
<<<<<<< HEAD

	err = sock_has_perm(current, sock->sk, SOCKET__ACCEPT);
	if (err)
		return err;

	newisec = SOCK_INODE(newsock)->i_security;

	isec = SOCK_INODE(sock)->i_security;
	newisec->sclass = isec->sclass;
	newisec->sid = isec->sid;
	newisec->initialized = 1;
=======
	u16 sclass;
	u32 sid;

	err = sock_has_perm(sock->sk, SOCKET__ACCEPT);
	if (err)
		return err;

	isec = inode_security_novalidate(SOCK_INODE(sock));
	spin_lock(&isec->lock);
	sclass = isec->sclass;
	sid = isec->sid;
	spin_unlock(&isec->lock);

	newisec = inode_security_novalidate(SOCK_INODE(newsock));
	newisec->sclass = sclass;
	newisec->sid = sid;
	newisec->initialized = LABEL_INITIALIZED;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	return 0;
}

static int selinux_socket_sendmsg(struct socket *sock, struct msghdr *msg,
				  int size)
{
<<<<<<< HEAD
	return sock_has_perm(current, sock->sk, SOCKET__WRITE);
=======
	return sock_has_perm(sock->sk, SOCKET__WRITE);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

static int selinux_socket_recvmsg(struct socket *sock, struct msghdr *msg,
				  int size, int flags)
{
<<<<<<< HEAD
	return sock_has_perm(current, sock->sk, SOCKET__READ);
=======
	return sock_has_perm(sock->sk, SOCKET__READ);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

static int selinux_socket_getsockname(struct socket *sock)
{
<<<<<<< HEAD
	return sock_has_perm(current, sock->sk, SOCKET__GETATTR);
=======
	return sock_has_perm(sock->sk, SOCKET__GETATTR);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

static int selinux_socket_getpeername(struct socket *sock)
{
<<<<<<< HEAD
	return sock_has_perm(current, sock->sk, SOCKET__GETATTR);
=======
	return sock_has_perm(sock->sk, SOCKET__GETATTR);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

static int selinux_socket_setsockopt(struct socket *sock, int level, int optname)
{
	int err;

<<<<<<< HEAD
	err = sock_has_perm(current, sock->sk, SOCKET__SETOPT);
=======
	err = sock_has_perm(sock->sk, SOCKET__SETOPT);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	if (err)
		return err;

	return selinux_netlbl_socket_setsockopt(sock, level, optname);
}

static int selinux_socket_getsockopt(struct socket *sock, int level,
				     int optname)
{
<<<<<<< HEAD
	return sock_has_perm(current, sock->sk, SOCKET__GETOPT);
=======
	return sock_has_perm(sock->sk, SOCKET__GETOPT);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

static int selinux_socket_shutdown(struct socket *sock, int how)
{
<<<<<<< HEAD
	return sock_has_perm(current, sock->sk, SOCKET__SHUTDOWN);
=======
	return sock_has_perm(sock->sk, SOCKET__SHUTDOWN);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

static int selinux_socket_unix_stream_connect(struct sock *sock,
					      struct sock *other,
					      struct sock *newsk)
{
	struct sk_security_struct *sksec_sock = sock->sk_security;
	struct sk_security_struct *sksec_other = other->sk_security;
	struct sk_security_struct *sksec_new = newsk->sk_security;
	struct common_audit_data ad;
<<<<<<< HEAD
	struct selinux_audit_data sad = {0,};
	struct lsm_network_audit net = {0,};
	int err;

	COMMON_AUDIT_DATA_INIT(&ad, NET);
	ad.selinux_audit_data = &sad;
	ad.u.net = &net;
	ad.u.net->sk = other;
=======
	struct lsm_network_audit net;
	int err;

	ad_net_init_from_sk(&ad, &net, other);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	err = avc_has_perm(sksec_sock->sid, sksec_other->sid,
			   sksec_other->sclass,
			   UNIX_STREAM_SOCKET__CONNECTTO, &ad);
	if (err)
		return err;

	/* server child socket */
	sksec_new->peer_sid = sksec_sock->sid;
<<<<<<< HEAD
	err = security_sid_mls_copy(sksec_other->sid, sksec_sock->sid,
				    &sksec_new->sid);
=======
	err = security_sid_mls_copy(sksec_other->sid,
				    sksec_sock->sid, &sksec_new->sid);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	if (err)
		return err;

	/* connecting socket */
	sksec_sock->peer_sid = sksec_new->sid;

	return 0;
}

static int selinux_socket_unix_may_send(struct socket *sock,
					struct socket *other)
{
	struct sk_security_struct *ssec = sock->sk->sk_security;
	struct sk_security_struct *osec = other->sk->sk_security;
	struct common_audit_data ad;
<<<<<<< HEAD
	struct selinux_audit_data sad = {0,};
	struct lsm_network_audit net = {0,};

	COMMON_AUDIT_DATA_INIT(&ad, NET);
	ad.selinux_audit_data = &sad;
	ad.u.net = &net;
	ad.u.net->sk = other->sk;
=======
	struct lsm_network_audit net;

	ad_net_init_from_sk(&ad, &net, other->sk);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	return avc_has_perm(ssec->sid, osec->sid, osec->sclass, SOCKET__SENDTO,
			    &ad);
}

<<<<<<< HEAD
static int selinux_inet_sys_rcv_skb(int ifindex, char *addrp, u16 family,
				    u32 peer_sid,
=======
static int selinux_inet_sys_rcv_skb(struct net *ns, int ifindex,
				    char *addrp, u16 family, u32 peer_sid,
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
				    struct common_audit_data *ad)
{
	int err;
	u32 if_sid;
	u32 node_sid;

<<<<<<< HEAD
	err = sel_netif_sid(ifindex, &if_sid);
=======
	err = sel_netif_sid(ns, ifindex, &if_sid);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	if (err)
		return err;
	err = avc_has_perm(peer_sid, if_sid,
			   SECCLASS_NETIF, NETIF__INGRESS, ad);
	if (err)
		return err;

	err = sel_netnode_sid(addrp, family, &node_sid);
	if (err)
		return err;
	return avc_has_perm(peer_sid, node_sid,
			    SECCLASS_NODE, NODE__RECVFROM, ad);
}

static int selinux_sock_rcv_skb_compat(struct sock *sk, struct sk_buff *skb,
				       u16 family)
{
	int err = 0;
	struct sk_security_struct *sksec = sk->sk_security;
	u32 sk_sid = sksec->sid;
	struct common_audit_data ad;
<<<<<<< HEAD
	struct selinux_audit_data sad = {0,};
	struct lsm_network_audit net = {0,};
	char *addrp;

	COMMON_AUDIT_DATA_INIT(&ad, NET);
	ad.selinux_audit_data = &sad;
	ad.u.net = &net;
	ad.u.net->netif = skb->skb_iif;
	ad.u.net->family = family;
=======
	struct lsm_network_audit net;
	char *addrp;

	ad_net_init_from_iif(&ad, &net, skb->skb_iif, family);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	err = selinux_parse_skb(skb, &ad, &addrp, 1, NULL);
	if (err)
		return err;

	if (selinux_secmark_enabled()) {
		err = avc_has_perm(sk_sid, skb->secmark, SECCLASS_PACKET,
				   PACKET__RECV, &ad);
		if (err)
			return err;
	}

	err = selinux_netlbl_sock_rcv_skb(sksec, skb, family, &ad);
	if (err)
		return err;
	err = selinux_xfrm_sock_rcv_skb(sksec->sid, skb, &ad);

	return err;
}

static int selinux_socket_sock_rcv_skb(struct sock *sk, struct sk_buff *skb)
{
<<<<<<< HEAD
	int err;
=======
	int err, peerlbl_active, secmark_active;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	struct sk_security_struct *sksec = sk->sk_security;
	u16 family = sk->sk_family;
	u32 sk_sid = sksec->sid;
	struct common_audit_data ad;
<<<<<<< HEAD
	struct selinux_audit_data sad = {0,};
	struct lsm_network_audit net = {0,};
	char *addrp;
	u8 secmark_active;
	u8 peerlbl_active;
=======
	struct lsm_network_audit net;
	char *addrp;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	if (family != PF_INET && family != PF_INET6)
		return 0;

	/* Handle mapped IPv4 packets arriving via IPv6 sockets */
	if (family == PF_INET6 && skb->protocol == htons(ETH_P_IP))
		family = PF_INET;

	/* If any sort of compatibility mode is enabled then handoff processing
	 * to the selinux_sock_rcv_skb_compat() function to deal with the
	 * special handling.  We do this in an attempt to keep this function
	 * as fast and as clean as possible. */
<<<<<<< HEAD
	if (!selinux_policycap_netpeer)
		return selinux_sock_rcv_skb_compat(sk, skb, family);

	secmark_active = selinux_secmark_enabled();
	peerlbl_active = netlbl_enabled() || selinux_xfrm_enabled();
	if (!secmark_active && !peerlbl_active)
		return 0;

	COMMON_AUDIT_DATA_INIT(&ad, NET);
	ad.selinux_audit_data = &sad;
	ad.u.net = &net;
	ad.u.net->netif = skb->skb_iif;
	ad.u.net->family = family;
=======
	if (!selinux_policycap_netpeer())
		return selinux_sock_rcv_skb_compat(sk, skb, family);

	secmark_active = selinux_secmark_enabled();
	peerlbl_active = selinux_peerlbl_enabled();
	if (!secmark_active && !peerlbl_active)
		return 0;

	ad_net_init_from_iif(&ad, &net, skb->skb_iif, family);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	err = selinux_parse_skb(skb, &ad, &addrp, 1, NULL);
	if (err)
		return err;

	if (peerlbl_active) {
		u32 peer_sid;

		err = selinux_skb_peerlbl_sid(skb, family, &peer_sid);
		if (err)
			return err;
<<<<<<< HEAD
		err = selinux_inet_sys_rcv_skb(skb->skb_iif, addrp, family,
					       peer_sid, &ad);
		if (err) {
			selinux_netlbl_err(skb, err, 0);
=======
		err = selinux_inet_sys_rcv_skb(sock_net(sk), skb->skb_iif,
					       addrp, family, peer_sid, &ad);
		if (err) {
			selinux_netlbl_err(skb, family, err, 0);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
			return err;
		}
		err = avc_has_perm(sk_sid, peer_sid, SECCLASS_PEER,
				   PEER__RECV, &ad);
		if (err) {
<<<<<<< HEAD
			selinux_netlbl_err(skb, err, 0);
=======
			selinux_netlbl_err(skb, family, err, 0);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
			return err;
		}
	}

	if (secmark_active) {
		err = avc_has_perm(sk_sid, skb->secmark, SECCLASS_PACKET,
				   PACKET__RECV, &ad);
		if (err)
			return err;
	}

	return err;
}

<<<<<<< HEAD
static int selinux_socket_getpeersec_stream(struct socket *sock, char __user *optval,
					    int __user *optlen, unsigned len)
{
	int err = 0;
	char *scontext;
=======
static int selinux_socket_getpeersec_stream(struct socket *sock,
					    sockptr_t optval, sockptr_t optlen,
					    unsigned int len)
{
	int err = 0;
	char *scontext = NULL;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	u32 scontext_len;
	struct sk_security_struct *sksec = sock->sk->sk_security;
	u32 peer_sid = SECSID_NULL;

	if (sksec->sclass == SECCLASS_UNIX_STREAM_SOCKET ||
<<<<<<< HEAD
	    sksec->sclass == SECCLASS_TCP_SOCKET)
=======
	    sksec->sclass == SECCLASS_TCP_SOCKET ||
	    sksec->sclass == SECCLASS_SCTP_SOCKET)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		peer_sid = sksec->peer_sid;
	if (peer_sid == SECSID_NULL)
		return -ENOPROTOOPT;

<<<<<<< HEAD
	err = security_sid_to_context(peer_sid, &scontext, &scontext_len);
	if (err)
		return err;

=======
	err = security_sid_to_context(peer_sid, &scontext,
				      &scontext_len);
	if (err)
		return err;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	if (scontext_len > len) {
		err = -ERANGE;
		goto out_len;
	}

<<<<<<< HEAD
	if (copy_to_user(optval, scontext, scontext_len))
		err = -EFAULT;

out_len:
	if (put_user(scontext_len, optlen))
=======
	if (copy_to_sockptr(optval, scontext, scontext_len))
		err = -EFAULT;
out_len:
	if (copy_to_sockptr(optlen, &scontext_len, sizeof(scontext_len)))
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		err = -EFAULT;
	kfree(scontext);
	return err;
}

<<<<<<< HEAD
static int selinux_socket_getpeersec_dgram(struct socket *sock, struct sk_buff *skb, u32 *secid)
=======
static int selinux_socket_getpeersec_dgram(struct socket *sock,
					   struct sk_buff *skb, u32 *secid)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
{
	u32 peer_secid = SECSID_NULL;
	u16 family;

	if (skb && skb->protocol == htons(ETH_P_IP))
		family = PF_INET;
	else if (skb && skb->protocol == htons(ETH_P_IPV6))
		family = PF_INET6;
	else if (sock)
		family = sock->sk->sk_family;
<<<<<<< HEAD
	else
		goto out;

	if (sock && family == PF_UNIX)
		selinux_inode_getsecid(SOCK_INODE(sock), &peer_secid);
	else if (skb)
		selinux_skb_peerlbl_sid(skb, family, &peer_secid);

out:
	*secid = peer_secid;
	if (peer_secid == SECSID_NULL)
		return -EINVAL;
=======
	else {
		*secid = SECSID_NULL;
		return -EINVAL;
	}

	if (sock && family == PF_UNIX) {
		struct inode_security_struct *isec;
		isec = inode_security_novalidate(SOCK_INODE(sock));
		peer_secid = isec->sid;
	} else if (skb)
		selinux_skb_peerlbl_sid(skb, family, &peer_secid);

	*secid = peer_secid;
	if (peer_secid == SECSID_NULL)
		return -ENOPROTOOPT;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	return 0;
}

static int selinux_sk_alloc_security(struct sock *sk, int family, gfp_t priority)
{
	struct sk_security_struct *sksec;

	sksec = kzalloc(sizeof(*sksec), priority);
	if (!sksec)
		return -ENOMEM;

	sksec->peer_sid = SECINITSID_UNLABELED;
	sksec->sid = SECINITSID_UNLABELED;
<<<<<<< HEAD
=======
	sksec->sclass = SECCLASS_SOCKET;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	selinux_netlbl_sk_security_reset(sksec);
	sk->sk_security = sksec;

	return 0;
}

static void selinux_sk_free_security(struct sock *sk)
{
	struct sk_security_struct *sksec = sk->sk_security;

	sk->sk_security = NULL;
	selinux_netlbl_sk_security_free(sksec);
	kfree(sksec);
}

static void selinux_sk_clone_security(const struct sock *sk, struct sock *newsk)
{
	struct sk_security_struct *sksec = sk->sk_security;
	struct sk_security_struct *newsksec = newsk->sk_security;

	newsksec->sid = sksec->sid;
	newsksec->peer_sid = sksec->peer_sid;
	newsksec->sclass = sksec->sclass;

	selinux_netlbl_sk_security_reset(newsksec);
}

<<<<<<< HEAD
static void selinux_sk_getsecid(struct sock *sk, u32 *secid)
=======
static void selinux_sk_getsecid(const struct sock *sk, u32 *secid)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
{
	if (!sk)
		*secid = SECINITSID_ANY_SOCKET;
	else {
<<<<<<< HEAD
		struct sk_security_struct *sksec = sk->sk_security;
=======
		const struct sk_security_struct *sksec = sk->sk_security;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

		*secid = sksec->sid;
	}
}

static void selinux_sock_graft(struct sock *sk, struct socket *parent)
{
<<<<<<< HEAD
	struct inode_security_struct *isec = SOCK_INODE(parent)->i_security;
=======
	struct inode_security_struct *isec =
		inode_security_novalidate(SOCK_INODE(parent));
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	struct sk_security_struct *sksec = sk->sk_security;

	if (sk->sk_family == PF_INET || sk->sk_family == PF_INET6 ||
	    sk->sk_family == PF_UNIX)
		isec->sid = sksec->sid;
	sksec->sclass = isec->sclass;
}

<<<<<<< HEAD
static int selinux_inet_conn_request(struct sock *sk, struct sk_buff *skb,
				     struct request_sock *req)
{
	struct sk_security_struct *sksec = sk->sk_security;
	int err;
	u16 family = sk->sk_family;
	u32 connsid;
	u32 peersid;
=======
/*
 * Determines peer_secid for the asoc and updates socket's peer label
 * if it's the first association on the socket.
 */
static int selinux_sctp_process_new_assoc(struct sctp_association *asoc,
					  struct sk_buff *skb)
{
	struct sock *sk = asoc->base.sk;
	u16 family = sk->sk_family;
	struct sk_security_struct *sksec = sk->sk_security;
	struct common_audit_data ad;
	struct lsm_network_audit net;
	int err;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	/* handle mapped IPv4 packets arriving via IPv6 sockets */
	if (family == PF_INET6 && skb->protocol == htons(ETH_P_IP))
		family = PF_INET;

<<<<<<< HEAD
=======
	if (selinux_peerlbl_enabled()) {
		asoc->peer_secid = SECSID_NULL;

		/* This will return peer_sid = SECSID_NULL if there are
		 * no peer labels, see security_net_peersid_resolve().
		 */
		err = selinux_skb_peerlbl_sid(skb, family, &asoc->peer_secid);
		if (err)
			return err;

		if (asoc->peer_secid == SECSID_NULL)
			asoc->peer_secid = SECINITSID_UNLABELED;
	} else {
		asoc->peer_secid = SECINITSID_UNLABELED;
	}

	if (sksec->sctp_assoc_state == SCTP_ASSOC_UNSET) {
		sksec->sctp_assoc_state = SCTP_ASSOC_SET;

		/* Here as first association on socket. As the peer SID
		 * was allowed by peer recv (and the netif/node checks),
		 * then it is approved by policy and used as the primary
		 * peer SID for getpeercon(3).
		 */
		sksec->peer_sid = asoc->peer_secid;
	} else if (sksec->peer_sid != asoc->peer_secid) {
		/* Other association peer SIDs are checked to enforce
		 * consistency among the peer SIDs.
		 */
		ad_net_init_from_sk(&ad, &net, asoc->base.sk);
		err = avc_has_perm(sksec->peer_sid, asoc->peer_secid,
				   sksec->sclass, SCTP_SOCKET__ASSOCIATION,
				   &ad);
		if (err)
			return err;
	}
	return 0;
}

/* Called whenever SCTP receives an INIT or COOKIE ECHO chunk. This
 * happens on an incoming connect(2), sctp_connectx(3) or
 * sctp_sendmsg(3) (with no association already present).
 */
static int selinux_sctp_assoc_request(struct sctp_association *asoc,
				      struct sk_buff *skb)
{
	struct sk_security_struct *sksec = asoc->base.sk->sk_security;
	u32 conn_sid;
	int err;

	if (!selinux_policycap_extsockclass())
		return 0;

	err = selinux_sctp_process_new_assoc(asoc, skb);
	if (err)
		return err;

	/* Compute the MLS component for the connection and store
	 * the information in asoc. This will be used by SCTP TCP type
	 * sockets and peeled off connections as they cause a new
	 * socket to be generated. selinux_sctp_sk_clone() will then
	 * plug this into the new socket.
	 */
	err = selinux_conn_sid(sksec->sid, asoc->peer_secid, &conn_sid);
	if (err)
		return err;

	asoc->secid = conn_sid;

	/* Set any NetLabel labels including CIPSO/CALIPSO options. */
	return selinux_netlbl_sctp_assoc_request(asoc, skb);
}

/* Called when SCTP receives a COOKIE ACK chunk as the final
 * response to an association request (initited by us).
 */
static int selinux_sctp_assoc_established(struct sctp_association *asoc,
					  struct sk_buff *skb)
{
	struct sk_security_struct *sksec = asoc->base.sk->sk_security;

	if (!selinux_policycap_extsockclass())
		return 0;

	/* Inherit secid from the parent socket - this will be picked up
	 * by selinux_sctp_sk_clone() if the association gets peeled off
	 * into a new socket.
	 */
	asoc->secid = sksec->sid;

	return selinux_sctp_process_new_assoc(asoc, skb);
}

/* Check if sctp IPv4/IPv6 addresses are valid for binding or connecting
 * based on their @optname.
 */
static int selinux_sctp_bind_connect(struct sock *sk, int optname,
				     struct sockaddr *address,
				     int addrlen)
{
	int len, err = 0, walk_size = 0;
	void *addr_buf;
	struct sockaddr *addr;
	struct socket *sock;

	if (!selinux_policycap_extsockclass())
		return 0;

	/* Process one or more addresses that may be IPv4 or IPv6 */
	sock = sk->sk_socket;
	addr_buf = address;

	while (walk_size < addrlen) {
		if (walk_size + sizeof(sa_family_t) > addrlen)
			return -EINVAL;

		addr = addr_buf;
		switch (addr->sa_family) {
		case AF_UNSPEC:
		case AF_INET:
			len = sizeof(struct sockaddr_in);
			break;
		case AF_INET6:
			len = sizeof(struct sockaddr_in6);
			break;
		default:
			return -EINVAL;
		}

		if (walk_size + len > addrlen)
			return -EINVAL;

		err = -EINVAL;
		switch (optname) {
		/* Bind checks */
		case SCTP_PRIMARY_ADDR:
		case SCTP_SET_PEER_PRIMARY_ADDR:
		case SCTP_SOCKOPT_BINDX_ADD:
			err = selinux_socket_bind(sock, addr, len);
			break;
		/* Connect checks */
		case SCTP_SOCKOPT_CONNECTX:
		case SCTP_PARAM_SET_PRIMARY:
		case SCTP_PARAM_ADD_IP:
		case SCTP_SENDMSG_CONNECT:
			err = selinux_socket_connect_helper(sock, addr, len);
			if (err)
				return err;

			/* As selinux_sctp_bind_connect() is called by the
			 * SCTP protocol layer, the socket is already locked,
			 * therefore selinux_netlbl_socket_connect_locked()
			 * is called here. The situations handled are:
			 * sctp_connectx(3), sctp_sendmsg(3), sendmsg(2),
			 * whenever a new IP address is added or when a new
			 * primary address is selected.
			 * Note that an SCTP connect(2) call happens before
			 * the SCTP protocol layer and is handled via
			 * selinux_socket_connect().
			 */
			err = selinux_netlbl_socket_connect_locked(sk, addr);
			break;
		}

		if (err)
			return err;

		addr_buf += len;
		walk_size += len;
	}

	return 0;
}

/* Called whenever a new socket is created by accept(2) or sctp_peeloff(3). */
static void selinux_sctp_sk_clone(struct sctp_association *asoc, struct sock *sk,
				  struct sock *newsk)
{
	struct sk_security_struct *sksec = sk->sk_security;
	struct sk_security_struct *newsksec = newsk->sk_security;

	/* If policy does not support SECCLASS_SCTP_SOCKET then call
	 * the non-sctp clone version.
	 */
	if (!selinux_policycap_extsockclass())
		return selinux_sk_clone_security(sk, newsk);

	newsksec->sid = asoc->secid;
	newsksec->peer_sid = asoc->peer_secid;
	newsksec->sclass = sksec->sclass;
	selinux_netlbl_sctp_sk_clone(sk, newsk);
}

static int selinux_mptcp_add_subflow(struct sock *sk, struct sock *ssk)
{
	struct sk_security_struct *ssksec = ssk->sk_security;
	struct sk_security_struct *sksec = sk->sk_security;

	ssksec->sclass = sksec->sclass;
	ssksec->sid = sksec->sid;

	/* replace the existing subflow label deleting the existing one
	 * and re-recreating a new label using the updated context
	 */
	selinux_netlbl_sk_security_free(ssksec);
	return selinux_netlbl_socket_post_create(ssk, ssk->sk_family);
}

static int selinux_inet_conn_request(const struct sock *sk, struct sk_buff *skb,
				     struct request_sock *req)
{
	struct sk_security_struct *sksec = sk->sk_security;
	int err;
	u16 family = req->rsk_ops->family;
	u32 connsid;
	u32 peersid;

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	err = selinux_skb_peerlbl_sid(skb, family, &peersid);
	if (err)
		return err;
	err = selinux_conn_sid(sksec->sid, peersid, &connsid);
	if (err)
		return err;
	req->secid = connsid;
	req->peer_secid = peersid;

	return selinux_netlbl_inet_conn_request(req, family);
}

static void selinux_inet_csk_clone(struct sock *newsk,
				   const struct request_sock *req)
{
	struct sk_security_struct *newsksec = newsk->sk_security;

	newsksec->sid = req->secid;
	newsksec->peer_sid = req->peer_secid;
	/* NOTE: Ideally, we should also get the isec->sid for the
	   new socket in sync, but we don't have the isec available yet.
	   So we will wait until sock_graft to do it, by which
	   time it will have been created and available. */

	/* We don't need to take any sort of lock here as we are the only
	 * thread with access to newsksec */
	selinux_netlbl_inet_csk_clone(newsk, req->rsk_ops->family);
}

static void selinux_inet_conn_established(struct sock *sk, struct sk_buff *skb)
{
	u16 family = sk->sk_family;
	struct sk_security_struct *sksec = sk->sk_security;

	/* handle mapped IPv4 packets arriving via IPv6 sockets */
	if (family == PF_INET6 && skb->protocol == htons(ETH_P_IP))
		family = PF_INET;

	selinux_skb_peerlbl_sid(skb, family, &sksec->peer_sid);
}

static int selinux_secmark_relabel_packet(u32 sid)
{
<<<<<<< HEAD
	const struct task_security_struct *__tsec;
	u32 tsid;

	__tsec = current_security();
	tsid = __tsec->sid;

	return avc_has_perm(tsid, sid, SECCLASS_PACKET, PACKET__RELABELTO, NULL);
=======
	const struct task_security_struct *tsec;
	u32 tsid;

	tsec = selinux_cred(current_cred());
	tsid = tsec->sid;

	return avc_has_perm(tsid, sid, SECCLASS_PACKET, PACKET__RELABELTO,
			    NULL);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

static void selinux_secmark_refcount_inc(void)
{
	atomic_inc(&selinux_secmark_refcount);
}

static void selinux_secmark_refcount_dec(void)
{
	atomic_dec(&selinux_secmark_refcount);
}

static void selinux_req_classify_flow(const struct request_sock *req,
<<<<<<< HEAD
				      struct flowi *fl)
{
	fl->flowi_secid = req->secid;
=======
				      struct flowi_common *flic)
{
	flic->flowic_secid = req->secid;
}

static int selinux_tun_dev_alloc_security(void **security)
{
	struct tun_security_struct *tunsec;

	tunsec = kzalloc(sizeof(*tunsec), GFP_KERNEL);
	if (!tunsec)
		return -ENOMEM;
	tunsec->sid = current_sid();

	*security = tunsec;
	return 0;
}

static void selinux_tun_dev_free_security(void *security)
{
	kfree(security);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

static int selinux_tun_dev_create(void)
{
	u32 sid = current_sid();

	/* we aren't taking into account the "sockcreate" SID since the socket
	 * that is being created here is not a socket in the traditional sense,
	 * instead it is a private sock, accessible only to the kernel, and
	 * representing a wide range of network traffic spanning multiple
	 * connections unlike traditional sockets - check the TUN driver to
	 * get a better understanding of why this socket is special */

	return avc_has_perm(sid, sid, SECCLASS_TUN_SOCKET, TUN_SOCKET__CREATE,
			    NULL);
}

<<<<<<< HEAD
static void selinux_tun_dev_post_create(struct sock *sk)
{
=======
static int selinux_tun_dev_attach_queue(void *security)
{
	struct tun_security_struct *tunsec = security;

	return avc_has_perm(current_sid(), tunsec->sid, SECCLASS_TUN_SOCKET,
			    TUN_SOCKET__ATTACH_QUEUE, NULL);
}

static int selinux_tun_dev_attach(struct sock *sk, void *security)
{
	struct tun_security_struct *tunsec = security;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	struct sk_security_struct *sksec = sk->sk_security;

	/* we don't currently perform any NetLabel based labeling here and it
	 * isn't clear that we would want to do so anyway; while we could apply
	 * labeling without the support of the TUN user the resulting labeled
	 * traffic from the other end of the connection would almost certainly
	 * cause confusion to the TUN user that had no idea network labeling
	 * protocols were being used */

<<<<<<< HEAD
	/* see the comments in selinux_tun_dev_create() about why we don't use
	 * the sockcreate SID here */

	sksec->sid = current_sid();
	sksec->sclass = SECCLASS_TUN_SOCKET;
}

static int selinux_tun_dev_attach(struct sock *sk)
{
	struct sk_security_struct *sksec = sk->sk_security;
	u32 sid = current_sid();
	int err;

	err = avc_has_perm(sid, sksec->sid, SECCLASS_TUN_SOCKET,
=======
	sksec->sid = tunsec->sid;
	sksec->sclass = SECCLASS_TUN_SOCKET;

	return 0;
}

static int selinux_tun_dev_open(void *security)
{
	struct tun_security_struct *tunsec = security;
	u32 sid = current_sid();
	int err;

	err = avc_has_perm(sid, tunsec->sid, SECCLASS_TUN_SOCKET,
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
			   TUN_SOCKET__RELABELFROM, NULL);
	if (err)
		return err;
	err = avc_has_perm(sid, sid, SECCLASS_TUN_SOCKET,
			   TUN_SOCKET__RELABELTO, NULL);
	if (err)
		return err;
<<<<<<< HEAD

	sksec->sid = sid;
=======
	tunsec->sid = sid;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	return 0;
}

<<<<<<< HEAD
static int selinux_nlmsg_perm(struct sock *sk, struct sk_buff *skb)
{
	int err = 0;
	u32 perm;
	struct nlmsghdr *nlh;
	struct sk_security_struct *sksec = sk->sk_security;

	if (skb->len < NLMSG_SPACE(0)) {
		err = -EINVAL;
		goto out;
	}
	nlh = nlmsg_hdr(skb);

	err = selinux_nlmsg_lookup(sksec->sclass, nlh->nlmsg_type, &perm);
	if (err) {
		if (err == -EINVAL) {
			audit_log(current->audit_context, GFP_KERNEL, AUDIT_SELINUX_ERR,
				  "SELinux:  unrecognized netlink message"
				  " type=%hu for sclass=%hu\n",
				  nlh->nlmsg_type, sksec->sclass);
			if (!selinux_enforcing || security_get_allow_unknown())
				err = 0;
		}

		/* Ignore */
		if (err == -ENOENT)
			err = 0;
		goto out;
	}

	err = sock_has_perm(current, sk, perm);
out:
	return err;
}

#ifdef CONFIG_NETFILTER

static unsigned int selinux_ip_forward(struct sk_buff *skb, int ifindex,
				       u16 family)
{
	int err;
	char *addrp;
	u32 peer_sid;
	struct common_audit_data ad;
	struct selinux_audit_data sad = {0,};
	struct lsm_network_audit net = {0,};
	u8 secmark_active;
	u8 netlbl_active;
	u8 peerlbl_active;

	if (!selinux_policycap_netpeer)
		return NF_ACCEPT;

	secmark_active = selinux_secmark_enabled();
	netlbl_active = netlbl_enabled();
	peerlbl_active = netlbl_active || selinux_xfrm_enabled();
	if (!secmark_active && !peerlbl_active)
		return NF_ACCEPT;

	if (selinux_skb_peerlbl_sid(skb, family, &peer_sid) != 0)
		return NF_DROP;

	COMMON_AUDIT_DATA_INIT(&ad, NET);
	ad.selinux_audit_data = &sad;
	ad.u.net = &net;
	ad.u.net->netif = ifindex;
	ad.u.net->family = family;
=======
#ifdef CONFIG_NETFILTER

static unsigned int selinux_ip_forward(void *priv, struct sk_buff *skb,
				       const struct nf_hook_state *state)
{
	int ifindex;
	u16 family;
	char *addrp;
	u32 peer_sid;
	struct common_audit_data ad;
	struct lsm_network_audit net;
	int secmark_active, peerlbl_active;

	if (!selinux_policycap_netpeer())
		return NF_ACCEPT;

	secmark_active = selinux_secmark_enabled();
	peerlbl_active = selinux_peerlbl_enabled();
	if (!secmark_active && !peerlbl_active)
		return NF_ACCEPT;

	family = state->pf;
	if (selinux_skb_peerlbl_sid(skb, family, &peer_sid) != 0)
		return NF_DROP;

	ifindex = state->in->ifindex;
	ad_net_init_from_iif(&ad, &net, ifindex, family);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	if (selinux_parse_skb(skb, &ad, &addrp, 1, NULL) != 0)
		return NF_DROP;

	if (peerlbl_active) {
<<<<<<< HEAD
		err = selinux_inet_sys_rcv_skb(ifindex, addrp, family,
					       peer_sid, &ad);
		if (err) {
			selinux_netlbl_err(skb, err, 1);
=======
		int err;

		err = selinux_inet_sys_rcv_skb(state->net, ifindex,
					       addrp, family, peer_sid, &ad);
		if (err) {
			selinux_netlbl_err(skb, family, err, 1);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
			return NF_DROP;
		}
	}

	if (secmark_active)
		if (avc_has_perm(peer_sid, skb->secmark,
				 SECCLASS_PACKET, PACKET__FORWARD_IN, &ad))
			return NF_DROP;

<<<<<<< HEAD
	if (netlbl_active)
=======
	if (netlbl_enabled())
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		/* we do this in the FORWARD path and not the POST_ROUTING
		 * path because we want to make sure we apply the necessary
		 * labeling before IPsec is applied so we can leverage AH
		 * protection */
		if (selinux_netlbl_skbuff_setsid(skb, family, peer_sid) != 0)
			return NF_DROP;

	return NF_ACCEPT;
}

<<<<<<< HEAD
static unsigned int selinux_ipv4_forward(unsigned int hooknum,
					 struct sk_buff *skb,
					 const struct net_device *in,
					 const struct net_device *out,
					 int (*okfn)(struct sk_buff *))
{
	return selinux_ip_forward(skb, in->ifindex, PF_INET);
}

#if defined(CONFIG_IPV6) || defined(CONFIG_IPV6_MODULE)
static unsigned int selinux_ipv6_forward(unsigned int hooknum,
					 struct sk_buff *skb,
					 const struct net_device *in,
					 const struct net_device *out,
					 int (*okfn)(struct sk_buff *))
{
	return selinux_ip_forward(skb, in->ifindex, PF_INET6);
}
#endif	/* IPV6 */

static unsigned int selinux_ip_output(struct sk_buff *skb,
				      u16 family)
=======
static unsigned int selinux_ip_output(void *priv, struct sk_buff *skb,
				      const struct nf_hook_state *state)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
{
	struct sock *sk;
	u32 sid;

	if (!netlbl_enabled())
		return NF_ACCEPT;

	/* we do this in the LOCAL_OUT path and not the POST_ROUTING path
	 * because we want to make sure we apply the necessary labeling
	 * before IPsec is applied so we can leverage AH protection */
	sk = skb->sk;
	if (sk) {
		struct sk_security_struct *sksec;

<<<<<<< HEAD
		if (sk->sk_state == TCP_LISTEN)
=======
		if (sk_listener(sk))
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
			/* if the socket is the listening state then this
			 * packet is a SYN-ACK packet which means it needs to
			 * be labeled based on the connection/request_sock and
			 * not the parent socket.  unfortunately, we can't
			 * lookup the request_sock yet as it isn't queued on
			 * the parent socket until after the SYN-ACK is sent.
			 * the "solution" is to simply pass the packet as-is
			 * as any IP option based labeling should be copied
			 * from the initial connection request (in the IP
			 * layer).  it is far from ideal, but until we get a
			 * security label in the packet itself this is the
			 * best we can do. */
			return NF_ACCEPT;

		/* standard practice, label using the parent socket */
		sksec = sk->sk_security;
		sid = sksec->sid;
	} else
		sid = SECINITSID_KERNEL;
<<<<<<< HEAD
	if (selinux_netlbl_skbuff_setsid(skb, family, sid) != 0)
=======
	if (selinux_netlbl_skbuff_setsid(skb, state->pf, sid) != 0)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		return NF_DROP;

	return NF_ACCEPT;
}

<<<<<<< HEAD
static unsigned int selinux_ipv4_output(unsigned int hooknum,
					struct sk_buff *skb,
					const struct net_device *in,
					const struct net_device *out,
					int (*okfn)(struct sk_buff *))
{
	return selinux_ip_output(skb, PF_INET);
}

static unsigned int selinux_ip_postroute_compat(struct sk_buff *skb,
						int ifindex,
						u16 family)
{
	struct sock *sk = skb->sk;
	struct sk_security_struct *sksec;
	struct common_audit_data ad;
	struct selinux_audit_data sad = {0,};
	struct lsm_network_audit net = {0,};
	char *addrp;
	u8 proto;

=======

static unsigned int selinux_ip_postroute_compat(struct sk_buff *skb,
					const struct nf_hook_state *state)
{
	struct sock *sk;
	struct sk_security_struct *sksec;
	struct common_audit_data ad;
	struct lsm_network_audit net;
	u8 proto = 0;

	sk = skb_to_full_sk(skb);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	if (sk == NULL)
		return NF_ACCEPT;
	sksec = sk->sk_security;

<<<<<<< HEAD
	COMMON_AUDIT_DATA_INIT(&ad, NET);
	ad.selinux_audit_data = &sad;
	ad.u.net = &net;
	ad.u.net->netif = ifindex;
	ad.u.net->family = family;
	if (selinux_parse_skb(skb, &ad, &addrp, 0, &proto))
=======
	ad_net_init_from_iif(&ad, &net, state->out->ifindex, state->pf);
	if (selinux_parse_skb(skb, &ad, NULL, 0, &proto))
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		return NF_DROP;

	if (selinux_secmark_enabled())
		if (avc_has_perm(sksec->sid, skb->secmark,
				 SECCLASS_PACKET, PACKET__SEND, &ad))
			return NF_DROP_ERR(-ECONNREFUSED);

	if (selinux_xfrm_postroute_last(sksec->sid, skb, &ad, proto))
		return NF_DROP_ERR(-ECONNREFUSED);

	return NF_ACCEPT;
}

<<<<<<< HEAD
static unsigned int selinux_ip_postroute(struct sk_buff *skb, int ifindex,
					 u16 family)
{
	u32 secmark_perm;
	u32 peer_sid;
	struct sock *sk;
	struct common_audit_data ad;
	struct selinux_audit_data sad = {0,};
	struct lsm_network_audit net = {0,};
	char *addrp;
	u8 secmark_active;
	u8 peerlbl_active;
=======
static unsigned int selinux_ip_postroute(void *priv,
					 struct sk_buff *skb,
					 const struct nf_hook_state *state)
{
	u16 family;
	u32 secmark_perm;
	u32 peer_sid;
	int ifindex;
	struct sock *sk;
	struct common_audit_data ad;
	struct lsm_network_audit net;
	char *addrp;
	int secmark_active, peerlbl_active;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	/* If any sort of compatibility mode is enabled then handoff processing
	 * to the selinux_ip_postroute_compat() function to deal with the
	 * special handling.  We do this in an attempt to keep this function
	 * as fast and as clean as possible. */
<<<<<<< HEAD
	if (!selinux_policycap_netpeer)
		return selinux_ip_postroute_compat(skb, ifindex, family);

	secmark_active = selinux_secmark_enabled();
	peerlbl_active = netlbl_enabled() || selinux_xfrm_enabled();
	if (!secmark_active && !peerlbl_active)
		return NF_ACCEPT;

	sk = skb->sk;
=======
	if (!selinux_policycap_netpeer())
		return selinux_ip_postroute_compat(skb, state);

	secmark_active = selinux_secmark_enabled();
	peerlbl_active = selinux_peerlbl_enabled();
	if (!secmark_active && !peerlbl_active)
		return NF_ACCEPT;

	sk = skb_to_full_sk(skb);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

#ifdef CONFIG_XFRM
	/* If skb->dst->xfrm is non-NULL then the packet is undergoing an IPsec
	 * packet transformation so allow the packet to pass without any checks
	 * since we'll have another chance to perform access control checks
	 * when the packet is on it's final way out.
	 * NOTE: there appear to be some IPv6 multicast cases where skb->dst
	 *       is NULL, in this case go ahead and apply access control.
<<<<<<< HEAD
	 *       is NULL, in this case go ahead and apply access control.
=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	 * NOTE: if this is a local socket (skb->sk != NULL) that is in the
	 *       TCP listening state we cannot wait until the XFRM processing
	 *       is done as we will miss out on the SA label if we do;
	 *       unfortunately, this means more work, but it is only once per
	 *       connection. */
	if (skb_dst(skb) != NULL && skb_dst(skb)->xfrm != NULL &&
<<<<<<< HEAD
	    !(sk != NULL && sk->sk_state == TCP_LISTEN))
		return NF_ACCEPT;
#endif

=======
	    !(sk && sk_listener(sk)))
		return NF_ACCEPT;
#endif

	family = state->pf;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	if (sk == NULL) {
		/* Without an associated socket the packet is either coming
		 * from the kernel or it is being forwarded; check the packet
		 * to determine which and if the packet is being forwarded
		 * query the packet directly to determine the security label. */
		if (skb->skb_iif) {
			secmark_perm = PACKET__FORWARD_OUT;
			if (selinux_skb_peerlbl_sid(skb, family, &peer_sid))
				return NF_DROP;
		} else {
			secmark_perm = PACKET__SEND;
			peer_sid = SECINITSID_KERNEL;
		}
<<<<<<< HEAD
	} else if (sk->sk_state == TCP_LISTEN) {
=======
	} else if (sk_listener(sk)) {
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		/* Locally generated packet but the associated socket is in the
		 * listening state which means this is a SYN-ACK packet.  In
		 * this particular case the correct security label is assigned
		 * to the connection/request_sock but unfortunately we can't
		 * query the request_sock as it isn't queued on the parent
		 * socket until after the SYN-ACK packet is sent; the only
		 * viable choice is to regenerate the label like we do in
		 * selinux_inet_conn_request().  See also selinux_ip_output()
		 * for similar problems. */
		u32 skb_sid;
<<<<<<< HEAD
		struct sk_security_struct *sksec = sk->sk_security;
=======
		struct sk_security_struct *sksec;

		sksec = sk->sk_security;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		if (selinux_skb_peerlbl_sid(skb, family, &skb_sid))
			return NF_DROP;
		/* At this point, if the returned skb peerlbl is SECSID_NULL
		 * and the packet has been through at least one XFRM
		 * transformation then we must be dealing with the "final"
		 * form of labeled IPsec packet; since we've already applied
		 * all of our access controls on this packet we can safely
		 * pass the packet. */
		if (skb_sid == SECSID_NULL) {
			switch (family) {
			case PF_INET:
				if (IPCB(skb)->flags & IPSKB_XFRM_TRANSFORMED)
					return NF_ACCEPT;
				break;
			case PF_INET6:
				if (IP6CB(skb)->flags & IP6SKB_XFRM_TRANSFORMED)
					return NF_ACCEPT;
<<<<<<< HEAD
=======
				break;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
			default:
				return NF_DROP_ERR(-ECONNREFUSED);
			}
		}
		if (selinux_conn_sid(sksec->sid, skb_sid, &peer_sid))
			return NF_DROP;
		secmark_perm = PACKET__SEND;
	} else {
		/* Locally generated packet, fetch the security label from the
		 * associated socket. */
		struct sk_security_struct *sksec = sk->sk_security;
		peer_sid = sksec->sid;
		secmark_perm = PACKET__SEND;
	}

<<<<<<< HEAD
	COMMON_AUDIT_DATA_INIT(&ad, NET);
	ad.selinux_audit_data = &sad;
	ad.u.net = &net;
	ad.u.net->netif = ifindex;
	ad.u.net->family = family;
=======
	ifindex = state->out->ifindex;
	ad_net_init_from_iif(&ad, &net, ifindex, family);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	if (selinux_parse_skb(skb, &ad, &addrp, 0, NULL))
		return NF_DROP;

	if (secmark_active)
		if (avc_has_perm(peer_sid, skb->secmark,
				 SECCLASS_PACKET, secmark_perm, &ad))
			return NF_DROP_ERR(-ECONNREFUSED);

	if (peerlbl_active) {
		u32 if_sid;
		u32 node_sid;

<<<<<<< HEAD
		if (sel_netif_sid(ifindex, &if_sid))
=======
		if (sel_netif_sid(state->net, ifindex, &if_sid))
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
			return NF_DROP;
		if (avc_has_perm(peer_sid, if_sid,
				 SECCLASS_NETIF, NETIF__EGRESS, &ad))
			return NF_DROP_ERR(-ECONNREFUSED);

		if (sel_netnode_sid(addrp, family, &node_sid))
			return NF_DROP;
		if (avc_has_perm(peer_sid, node_sid,
				 SECCLASS_NODE, NODE__SENDTO, &ad))
			return NF_DROP_ERR(-ECONNREFUSED);
	}

	return NF_ACCEPT;
}
<<<<<<< HEAD

static unsigned int selinux_ipv4_postroute(unsigned int hooknum,
					   struct sk_buff *skb,
					   const struct net_device *in,
					   const struct net_device *out,
					   int (*okfn)(struct sk_buff *))
{
	return selinux_ip_postroute(skb, out->ifindex, PF_INET);
}

#if defined(CONFIG_IPV6) || defined(CONFIG_IPV6_MODULE)
static unsigned int selinux_ipv6_postroute(unsigned int hooknum,
					   struct sk_buff *skb,
					   const struct net_device *in,
					   const struct net_device *out,
					   int (*okfn)(struct sk_buff *))
{
	return selinux_ip_postroute(skb, out->ifindex, PF_INET6);
}
#endif	/* IPV6 */

=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#endif	/* CONFIG_NETFILTER */

static int selinux_netlink_send(struct sock *sk, struct sk_buff *skb)
{
<<<<<<< HEAD
	int err;

	err = cap_netlink_send(sk, skb);
	if (err)
		return err;

	return selinux_nlmsg_perm(sk, skb);
}

static int ipc_alloc_security(struct task_struct *task,
			      struct kern_ipc_perm *perm,
			      u16 sclass)
{
	struct ipc_security_struct *isec;
	u32 sid;

	isec = kzalloc(sizeof(struct ipc_security_struct), GFP_KERNEL);
	if (!isec)
		return -ENOMEM;

	sid = task_sid(task);
	isec->sclass = sclass;
	isec->sid = sid;
	perm->security = isec;

	return 0;
}

static void ipc_free_security(struct kern_ipc_perm *perm)
{
	struct ipc_security_struct *isec = perm->security;
	perm->security = NULL;
	kfree(isec);
}

static int msg_msg_alloc_security(struct msg_msg *msg)
{
	struct msg_security_struct *msec;

	msec = kzalloc(sizeof(struct msg_security_struct), GFP_KERNEL);
	if (!msec)
		return -ENOMEM;

	msec->sid = SECINITSID_UNLABELED;
	msg->security = msec;

	return 0;
}

static void msg_msg_free_security(struct msg_msg *msg)
{
	struct msg_security_struct *msec = msg->security;

	msg->security = NULL;
	kfree(msec);
=======
	int rc = 0;
	unsigned int msg_len;
	unsigned int data_len = skb->len;
	unsigned char *data = skb->data;
	struct nlmsghdr *nlh;
	struct sk_security_struct *sksec = sk->sk_security;
	u16 sclass = sksec->sclass;
	u32 perm;

	while (data_len >= nlmsg_total_size(0)) {
		nlh = (struct nlmsghdr *)data;

		/* NOTE: the nlmsg_len field isn't reliably set by some netlink
		 *       users which means we can't reject skb's with bogus
		 *       length fields; our solution is to follow what
		 *       netlink_rcv_skb() does and simply skip processing at
		 *       messages with length fields that are clearly junk
		 */
		if (nlh->nlmsg_len < NLMSG_HDRLEN || nlh->nlmsg_len > data_len)
			return 0;

		rc = selinux_nlmsg_lookup(sclass, nlh->nlmsg_type, &perm);
		if (rc == 0) {
			rc = sock_has_perm(sk, perm);
			if (rc)
				return rc;
		} else if (rc == -EINVAL) {
			/* -EINVAL is a missing msg/perm mapping */
			pr_warn_ratelimited("SELinux: unrecognized netlink"
				" message: protocol=%hu nlmsg_type=%hu sclass=%s"
				" pid=%d comm=%s\n",
				sk->sk_protocol, nlh->nlmsg_type,
				secclass_map[sclass - 1].name,
				task_pid_nr(current), current->comm);
			if (enforcing_enabled() &&
			    !security_get_allow_unknown())
				return rc;
			rc = 0;
		} else if (rc == -ENOENT) {
			/* -ENOENT is a missing socket/class mapping, ignore */
			rc = 0;
		} else {
			return rc;
		}

		/* move to the next message after applying netlink padding */
		msg_len = NLMSG_ALIGN(nlh->nlmsg_len);
		if (msg_len >= data_len)
			return 0;
		data_len -= msg_len;
		data += msg_len;
	}

	return rc;
}

static void ipc_init_security(struct ipc_security_struct *isec, u16 sclass)
{
	isec->sclass = sclass;
	isec->sid = current_sid();
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

static int ipc_has_perm(struct kern_ipc_perm *ipc_perms,
			u32 perms)
{
	struct ipc_security_struct *isec;
	struct common_audit_data ad;
<<<<<<< HEAD
	struct selinux_audit_data sad = {0,};
	u32 sid = current_sid();

	isec = ipc_perms->security;

	COMMON_AUDIT_DATA_INIT(&ad, IPC);
	ad.selinux_audit_data = &sad;
=======
	u32 sid = current_sid();

	isec = selinux_ipc(ipc_perms);

	ad.type = LSM_AUDIT_DATA_IPC;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	ad.u.ipc_id = ipc_perms->key;

	return avc_has_perm(sid, isec->sid, isec->sclass, perms, &ad);
}

static int selinux_msg_msg_alloc_security(struct msg_msg *msg)
{
<<<<<<< HEAD
	return msg_msg_alloc_security(msg);
}

static void selinux_msg_msg_free_security(struct msg_msg *msg)
{
	msg_msg_free_security(msg);
}

/* message queue security operations */
static int selinux_msg_queue_alloc_security(struct msg_queue *msq)
{
	struct ipc_security_struct *isec;
	struct common_audit_data ad;
	struct selinux_audit_data sad = {0,};
	u32 sid = current_sid();
	int rc;

	rc = ipc_alloc_security(current, &msq->q_perm, SECCLASS_MSGQ);
	if (rc)
		return rc;

	isec = msq->q_perm.security;

	COMMON_AUDIT_DATA_INIT(&ad, IPC);
	ad.selinux_audit_data = &sad;
	ad.u.ipc_id = msq->q_perm.key;

	rc = avc_has_perm(sid, isec->sid, SECCLASS_MSGQ,
			  MSGQ__CREATE, &ad);
	if (rc) {
		ipc_free_security(&msq->q_perm);
		return rc;
	}
	return 0;
}

static void selinux_msg_queue_free_security(struct msg_queue *msq)
{
	ipc_free_security(&msq->q_perm);
}

static int selinux_msg_queue_associate(struct msg_queue *msq, int msqflg)
{
	struct ipc_security_struct *isec;
	struct common_audit_data ad;
	struct selinux_audit_data sad = {0,};
	u32 sid = current_sid();

	isec = msq->q_perm.security;

	COMMON_AUDIT_DATA_INIT(&ad, IPC);
	ad.selinux_audit_data = &sad;
	ad.u.ipc_id = msq->q_perm.key;
=======
	struct msg_security_struct *msec;

	msec = selinux_msg_msg(msg);
	msec->sid = SECINITSID_UNLABELED;

	return 0;
}

/* message queue security operations */
static int selinux_msg_queue_alloc_security(struct kern_ipc_perm *msq)
{
	struct ipc_security_struct *isec;
	struct common_audit_data ad;
	u32 sid = current_sid();

	isec = selinux_ipc(msq);
	ipc_init_security(isec, SECCLASS_MSGQ);

	ad.type = LSM_AUDIT_DATA_IPC;
	ad.u.ipc_id = msq->key;

	return avc_has_perm(sid, isec->sid, SECCLASS_MSGQ,
			    MSGQ__CREATE, &ad);
}

static int selinux_msg_queue_associate(struct kern_ipc_perm *msq, int msqflg)
{
	struct ipc_security_struct *isec;
	struct common_audit_data ad;
	u32 sid = current_sid();

	isec = selinux_ipc(msq);

	ad.type = LSM_AUDIT_DATA_IPC;
	ad.u.ipc_id = msq->key;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	return avc_has_perm(sid, isec->sid, SECCLASS_MSGQ,
			    MSGQ__ASSOCIATE, &ad);
}

<<<<<<< HEAD
static int selinux_msg_queue_msgctl(struct msg_queue *msq, int cmd)
{
	int err;
	int perms;
=======
static int selinux_msg_queue_msgctl(struct kern_ipc_perm *msq, int cmd)
{
	u32 perms;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	switch (cmd) {
	case IPC_INFO:
	case MSG_INFO:
		/* No specific object, just general system-wide information. */
<<<<<<< HEAD
		return task_has_system(current, SYSTEM__IPC_INFO);
	case IPC_STAT:
	case MSG_STAT:
=======
		return avc_has_perm(current_sid(), SECINITSID_KERNEL,
				    SECCLASS_SYSTEM, SYSTEM__IPC_INFO, NULL);
	case IPC_STAT:
	case MSG_STAT:
	case MSG_STAT_ANY:
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		perms = MSGQ__GETATTR | MSGQ__ASSOCIATE;
		break;
	case IPC_SET:
		perms = MSGQ__SETATTR;
		break;
	case IPC_RMID:
		perms = MSGQ__DESTROY;
		break;
	default:
		return 0;
	}

<<<<<<< HEAD
	err = ipc_has_perm(&msq->q_perm, perms);
	return err;
}

static int selinux_msg_queue_msgsnd(struct msg_queue *msq, struct msg_msg *msg, int msqflg)
=======
	return ipc_has_perm(msq, perms);
}

static int selinux_msg_queue_msgsnd(struct kern_ipc_perm *msq, struct msg_msg *msg, int msqflg)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
{
	struct ipc_security_struct *isec;
	struct msg_security_struct *msec;
	struct common_audit_data ad;
<<<<<<< HEAD
	struct selinux_audit_data sad = {0,};
	u32 sid = current_sid();
	int rc;

	isec = msq->q_perm.security;
	msec = msg->security;
=======
	u32 sid = current_sid();
	int rc;

	isec = selinux_ipc(msq);
	msec = selinux_msg_msg(msg);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	/*
	 * First time through, need to assign label to the message
	 */
	if (msec->sid == SECINITSID_UNLABELED) {
		/*
		 * Compute new sid based on current process and
		 * message queue this message will be stored in
		 */
<<<<<<< HEAD
		rc = security_transition_sid(sid, isec->sid, SECCLASS_MSG,
					     NULL, &msec->sid);
=======
		rc = security_transition_sid(sid, isec->sid,
					     SECCLASS_MSG, NULL, &msec->sid);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		if (rc)
			return rc;
	}

<<<<<<< HEAD
	COMMON_AUDIT_DATA_INIT(&ad, IPC);
	ad.selinux_audit_data = &sad;
	ad.u.ipc_id = msq->q_perm.key;
=======
	ad.type = LSM_AUDIT_DATA_IPC;
	ad.u.ipc_id = msq->key;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	/* Can this process write to the queue? */
	rc = avc_has_perm(sid, isec->sid, SECCLASS_MSGQ,
			  MSGQ__WRITE, &ad);
	if (!rc)
		/* Can this process send the message */
		rc = avc_has_perm(sid, msec->sid, SECCLASS_MSG,
				  MSG__SEND, &ad);
	if (!rc)
		/* Can the message be put in the queue? */
		rc = avc_has_perm(msec->sid, isec->sid, SECCLASS_MSGQ,
				  MSGQ__ENQUEUE, &ad);

	return rc;
}

<<<<<<< HEAD
static int selinux_msg_queue_msgrcv(struct msg_queue *msq, struct msg_msg *msg,
=======
static int selinux_msg_queue_msgrcv(struct kern_ipc_perm *msq, struct msg_msg *msg,
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
				    struct task_struct *target,
				    long type, int mode)
{
	struct ipc_security_struct *isec;
	struct msg_security_struct *msec;
	struct common_audit_data ad;
<<<<<<< HEAD
	struct selinux_audit_data sad = {0,};
	u32 sid = task_sid(target);
	int rc;

	isec = msq->q_perm.security;
	msec = msg->security;

	COMMON_AUDIT_DATA_INIT(&ad, IPC);
	ad.selinux_audit_data = &sad;
	ad.u.ipc_id = msq->q_perm.key;
=======
	u32 sid = task_sid_obj(target);
	int rc;

	isec = selinux_ipc(msq);
	msec = selinux_msg_msg(msg);

	ad.type = LSM_AUDIT_DATA_IPC;
	ad.u.ipc_id = msq->key;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	rc = avc_has_perm(sid, isec->sid,
			  SECCLASS_MSGQ, MSGQ__READ, &ad);
	if (!rc)
		rc = avc_has_perm(sid, msec->sid,
				  SECCLASS_MSG, MSG__RECEIVE, &ad);
	return rc;
}

/* Shared Memory security operations */
<<<<<<< HEAD
static int selinux_shm_alloc_security(struct shmid_kernel *shp)
{
	struct ipc_security_struct *isec;
	struct common_audit_data ad;
	struct selinux_audit_data sad = {0,};
	u32 sid = current_sid();
	int rc;

	rc = ipc_alloc_security(current, &shp->shm_perm, SECCLASS_SHM);
	if (rc)
		return rc;

	isec = shp->shm_perm.security;

	COMMON_AUDIT_DATA_INIT(&ad, IPC);
	ad.selinux_audit_data = &sad;
	ad.u.ipc_id = shp->shm_perm.key;

	rc = avc_has_perm(sid, isec->sid, SECCLASS_SHM,
			  SHM__CREATE, &ad);
	if (rc) {
		ipc_free_security(&shp->shm_perm);
		return rc;
	}
	return 0;
}

static void selinux_shm_free_security(struct shmid_kernel *shp)
{
	ipc_free_security(&shp->shm_perm);
}

static int selinux_shm_associate(struct shmid_kernel *shp, int shmflg)
{
	struct ipc_security_struct *isec;
	struct common_audit_data ad;
	struct selinux_audit_data sad = {0,};
	u32 sid = current_sid();

	isec = shp->shm_perm.security;

	COMMON_AUDIT_DATA_INIT(&ad, IPC);
	ad.selinux_audit_data = &sad;
	ad.u.ipc_id = shp->shm_perm.key;
=======
static int selinux_shm_alloc_security(struct kern_ipc_perm *shp)
{
	struct ipc_security_struct *isec;
	struct common_audit_data ad;
	u32 sid = current_sid();

	isec = selinux_ipc(shp);
	ipc_init_security(isec, SECCLASS_SHM);

	ad.type = LSM_AUDIT_DATA_IPC;
	ad.u.ipc_id = shp->key;

	return avc_has_perm(sid, isec->sid, SECCLASS_SHM,
			    SHM__CREATE, &ad);
}

static int selinux_shm_associate(struct kern_ipc_perm *shp, int shmflg)
{
	struct ipc_security_struct *isec;
	struct common_audit_data ad;
	u32 sid = current_sid();

	isec = selinux_ipc(shp);

	ad.type = LSM_AUDIT_DATA_IPC;
	ad.u.ipc_id = shp->key;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	return avc_has_perm(sid, isec->sid, SECCLASS_SHM,
			    SHM__ASSOCIATE, &ad);
}

/* Note, at this point, shp is locked down */
<<<<<<< HEAD
static int selinux_shm_shmctl(struct shmid_kernel *shp, int cmd)
{
	int perms;
	int err;
=======
static int selinux_shm_shmctl(struct kern_ipc_perm *shp, int cmd)
{
	u32 perms;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	switch (cmd) {
	case IPC_INFO:
	case SHM_INFO:
		/* No specific object, just general system-wide information. */
<<<<<<< HEAD
		return task_has_system(current, SYSTEM__IPC_INFO);
	case IPC_STAT:
	case SHM_STAT:
=======
		return avc_has_perm(current_sid(), SECINITSID_KERNEL,
				    SECCLASS_SYSTEM, SYSTEM__IPC_INFO, NULL);
	case IPC_STAT:
	case SHM_STAT:
	case SHM_STAT_ANY:
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		perms = SHM__GETATTR | SHM__ASSOCIATE;
		break;
	case IPC_SET:
		perms = SHM__SETATTR;
		break;
	case SHM_LOCK:
	case SHM_UNLOCK:
		perms = SHM__LOCK;
		break;
	case IPC_RMID:
		perms = SHM__DESTROY;
		break;
	default:
		return 0;
	}

<<<<<<< HEAD
	err = ipc_has_perm(&shp->shm_perm, perms);
	return err;
}

static int selinux_shm_shmat(struct shmid_kernel *shp,
=======
	return ipc_has_perm(shp, perms);
}

static int selinux_shm_shmat(struct kern_ipc_perm *shp,
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
			     char __user *shmaddr, int shmflg)
{
	u32 perms;

	if (shmflg & SHM_RDONLY)
		perms = SHM__READ;
	else
		perms = SHM__READ | SHM__WRITE;

<<<<<<< HEAD
	return ipc_has_perm(&shp->shm_perm, perms);
}

/* Semaphore security operations */
static int selinux_sem_alloc_security(struct sem_array *sma)
{
	struct ipc_security_struct *isec;
	struct common_audit_data ad;
	struct selinux_audit_data sad = {0,};
	u32 sid = current_sid();
	int rc;

	rc = ipc_alloc_security(current, &sma->sem_perm, SECCLASS_SEM);
	if (rc)
		return rc;

	isec = sma->sem_perm.security;

	COMMON_AUDIT_DATA_INIT(&ad, IPC);
	ad.selinux_audit_data = &sad;
	ad.u.ipc_id = sma->sem_perm.key;

	rc = avc_has_perm(sid, isec->sid, SECCLASS_SEM,
			  SEM__CREATE, &ad);
	if (rc) {
		ipc_free_security(&sma->sem_perm);
		return rc;
	}
	return 0;
}

static void selinux_sem_free_security(struct sem_array *sma)
{
	ipc_free_security(&sma->sem_perm);
}

static int selinux_sem_associate(struct sem_array *sma, int semflg)
{
	struct ipc_security_struct *isec;
	struct common_audit_data ad;
	struct selinux_audit_data sad = {0,};
	u32 sid = current_sid();

	isec = sma->sem_perm.security;

	COMMON_AUDIT_DATA_INIT(&ad, IPC);
	ad.selinux_audit_data = &sad;
	ad.u.ipc_id = sma->sem_perm.key;
=======
	return ipc_has_perm(shp, perms);
}

/* Semaphore security operations */
static int selinux_sem_alloc_security(struct kern_ipc_perm *sma)
{
	struct ipc_security_struct *isec;
	struct common_audit_data ad;
	u32 sid = current_sid();

	isec = selinux_ipc(sma);
	ipc_init_security(isec, SECCLASS_SEM);

	ad.type = LSM_AUDIT_DATA_IPC;
	ad.u.ipc_id = sma->key;

	return avc_has_perm(sid, isec->sid, SECCLASS_SEM,
			    SEM__CREATE, &ad);
}

static int selinux_sem_associate(struct kern_ipc_perm *sma, int semflg)
{
	struct ipc_security_struct *isec;
	struct common_audit_data ad;
	u32 sid = current_sid();

	isec = selinux_ipc(sma);

	ad.type = LSM_AUDIT_DATA_IPC;
	ad.u.ipc_id = sma->key;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	return avc_has_perm(sid, isec->sid, SECCLASS_SEM,
			    SEM__ASSOCIATE, &ad);
}

/* Note, at this point, sma is locked down */
<<<<<<< HEAD
static int selinux_sem_semctl(struct sem_array *sma, int cmd)
=======
static int selinux_sem_semctl(struct kern_ipc_perm *sma, int cmd)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
{
	int err;
	u32 perms;

	switch (cmd) {
	case IPC_INFO:
	case SEM_INFO:
		/* No specific object, just general system-wide information. */
<<<<<<< HEAD
		return task_has_system(current, SYSTEM__IPC_INFO);
=======
		return avc_has_perm(current_sid(), SECINITSID_KERNEL,
				    SECCLASS_SYSTEM, SYSTEM__IPC_INFO, NULL);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	case GETPID:
	case GETNCNT:
	case GETZCNT:
		perms = SEM__GETATTR;
		break;
	case GETVAL:
	case GETALL:
		perms = SEM__READ;
		break;
	case SETVAL:
	case SETALL:
		perms = SEM__WRITE;
		break;
	case IPC_RMID:
		perms = SEM__DESTROY;
		break;
	case IPC_SET:
		perms = SEM__SETATTR;
		break;
	case IPC_STAT:
	case SEM_STAT:
<<<<<<< HEAD
=======
	case SEM_STAT_ANY:
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		perms = SEM__GETATTR | SEM__ASSOCIATE;
		break;
	default:
		return 0;
	}

<<<<<<< HEAD
	err = ipc_has_perm(&sma->sem_perm, perms);
	return err;
}

static int selinux_sem_semop(struct sem_array *sma,
=======
	err = ipc_has_perm(sma, perms);
	return err;
}

static int selinux_sem_semop(struct kern_ipc_perm *sma,
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
			     struct sembuf *sops, unsigned nsops, int alter)
{
	u32 perms;

	if (alter)
		perms = SEM__READ | SEM__WRITE;
	else
		perms = SEM__READ;

<<<<<<< HEAD
	return ipc_has_perm(&sma->sem_perm, perms);
=======
	return ipc_has_perm(sma, perms);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

static int selinux_ipc_permission(struct kern_ipc_perm *ipcp, short flag)
{
	u32 av = 0;

	av = 0;
	if (flag & S_IRUGO)
		av |= IPC__UNIX_READ;
	if (flag & S_IWUGO)
		av |= IPC__UNIX_WRITE;

	if (av == 0)
		return 0;

	return ipc_has_perm(ipcp, av);
}

static void selinux_ipc_getsecid(struct kern_ipc_perm *ipcp, u32 *secid)
{
<<<<<<< HEAD
	struct ipc_security_struct *isec = ipcp->security;
=======
	struct ipc_security_struct *isec = selinux_ipc(ipcp);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	*secid = isec->sid;
}

static void selinux_d_instantiate(struct dentry *dentry, struct inode *inode)
{
	if (inode)
		inode_doinit_with_dentry(inode, dentry);
}

<<<<<<< HEAD
static int selinux_getprocattr(struct task_struct *p,
			       char *name, char **value)
=======
static int selinux_lsm_getattr(unsigned int attr, struct task_struct *p,
			       char **value)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
{
	const struct task_security_struct *__tsec;
	u32 sid;
	int error;
	unsigned len;

<<<<<<< HEAD
	if (current != p) {
		error = current_has_perm(p, PROCESS__GETATTR);
		if (error)
			return error;
	}

	rcu_read_lock();
	__tsec = __task_cred(p)->security;

	if (!strcmp(name, "current"))
		sid = __tsec->sid;
	else if (!strcmp(name, "prev"))
		sid = __tsec->osid;
	else if (!strcmp(name, "exec"))
		sid = __tsec->exec_sid;
	else if (!strcmp(name, "fscreate"))
		sid = __tsec->create_sid;
	else if (!strcmp(name, "keycreate"))
		sid = __tsec->keycreate_sid;
	else if (!strcmp(name, "sockcreate"))
		sid = __tsec->sockcreate_sid;
	else
		goto invalid;
=======
	rcu_read_lock();
	__tsec = selinux_cred(__task_cred(p));

	if (current != p) {
		error = avc_has_perm(current_sid(), __tsec->sid,
				     SECCLASS_PROCESS, PROCESS__GETATTR, NULL);
		if (error)
			goto bad;
	}

	switch (attr) {
	case LSM_ATTR_CURRENT:
		sid = __tsec->sid;
		break;
	case LSM_ATTR_PREV:
		sid = __tsec->osid;
		break;
	case LSM_ATTR_EXEC:
		sid = __tsec->exec_sid;
		break;
	case LSM_ATTR_FSCREATE:
		sid = __tsec->create_sid;
		break;
	case LSM_ATTR_KEYCREATE:
		sid = __tsec->keycreate_sid;
		break;
	case LSM_ATTR_SOCKCREATE:
		sid = __tsec->sockcreate_sid;
		break;
	default:
		error = -EOPNOTSUPP;
		goto bad;
	}
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	rcu_read_unlock();

	if (!sid)
		return 0;

	error = security_sid_to_context(sid, value, &len);
	if (error)
		return error;
	return len;

<<<<<<< HEAD
invalid:
	rcu_read_unlock();
	return -EINVAL;
}

static int selinux_setprocattr(struct task_struct *p,
			       char *name, void *value, size_t size)
{
	struct task_security_struct *tsec;
	struct task_struct *tracer;
	struct cred *new;
	u32 sid = 0, ptsid;
	int error;
	char *str = value;

	if (current != p) {
		/* SELinux only allows a process to change its own
		   security attributes. */
		return -EACCES;
	}

	/*
	 * Basic control over ability to set these attributes at all.
	 * current == p, but we'll pass them separately in case the
	 * above restriction is ever removed.
	 */
	if (!strcmp(name, "exec"))
		error = current_has_perm(p, PROCESS__SETEXEC);
	else if (!strcmp(name, "fscreate"))
		error = current_has_perm(p, PROCESS__SETFSCREATE);
	else if (!strcmp(name, "keycreate"))
		error = current_has_perm(p, PROCESS__SETKEYCREATE);
	else if (!strcmp(name, "sockcreate"))
		error = current_has_perm(p, PROCESS__SETSOCKCREATE);
	else if (!strcmp(name, "current"))
		error = current_has_perm(p, PROCESS__SETCURRENT);
	else
		error = -EINVAL;
=======
bad:
	rcu_read_unlock();
	return error;
}

static int selinux_lsm_setattr(u64 attr, void *value, size_t size)
{
	struct task_security_struct *tsec;
	struct cred *new;
	u32 mysid = current_sid(), sid = 0, ptsid;
	int error;
	char *str = value;

	/*
	 * Basic control over ability to set these attributes at all.
	 */
	switch (attr) {
	case LSM_ATTR_EXEC:
		error = avc_has_perm(mysid, mysid, SECCLASS_PROCESS,
				     PROCESS__SETEXEC, NULL);
		break;
	case LSM_ATTR_FSCREATE:
		error = avc_has_perm(mysid, mysid, SECCLASS_PROCESS,
				     PROCESS__SETFSCREATE, NULL);
		break;
	case LSM_ATTR_KEYCREATE:
		error = avc_has_perm(mysid, mysid, SECCLASS_PROCESS,
				     PROCESS__SETKEYCREATE, NULL);
		break;
	case LSM_ATTR_SOCKCREATE:
		error = avc_has_perm(mysid, mysid, SECCLASS_PROCESS,
				     PROCESS__SETSOCKCREATE, NULL);
		break;
	case LSM_ATTR_CURRENT:
		error = avc_has_perm(mysid, mysid, SECCLASS_PROCESS,
				     PROCESS__SETCURRENT, NULL);
		break;
	default:
		error = -EOPNOTSUPP;
		break;
	}
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	if (error)
		return error;

	/* Obtain a SID for the context, if one was specified. */
	if (size && str[0] && str[0] != '\n') {
		if (str[size-1] == '\n') {
			str[size-1] = 0;
			size--;
		}
<<<<<<< HEAD
		error = security_context_to_sid(value, size, &sid);
		if (error == -EINVAL && !strcmp(name, "fscreate")) {
			if (!capable(CAP_MAC_ADMIN))
				return error;
			error = security_context_to_sid_force(value, size,
							      &sid);
=======
		error = security_context_to_sid(value, size,
						&sid, GFP_KERNEL);
		if (error == -EINVAL && attr == LSM_ATTR_FSCREATE) {
			if (!has_cap_mac_admin(true)) {
				struct audit_buffer *ab;
				size_t audit_size;

				/* We strip a nul only if it is at the end,
				 * otherwise the context contains a nul and
				 * we should audit that */
				if (str[size - 1] == '\0')
					audit_size = size - 1;
				else
					audit_size = size;
				ab = audit_log_start(audit_context(),
						     GFP_ATOMIC,
						     AUDIT_SELINUX_ERR);
				if (!ab)
					return error;
				audit_log_format(ab, "op=fscreate invalid_context=");
				audit_log_n_untrustedstring(ab, value,
							    audit_size);
				audit_log_end(ab);

				return error;
			}
			error = security_context_to_sid_force(value, size,
							&sid);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		}
		if (error)
			return error;
	}

	new = prepare_creds();
	if (!new)
		return -ENOMEM;

	/* Permission checking based on the specified context is
	   performed during the actual operation (execve,
	   open/mkdir/...), when we know the full context of the
<<<<<<< HEAD
	   operation.  See selinux_bprm_set_creds for the execve
	   checks and may_create for the file creation checks. The
	   operation will then fail if the context is not permitted. */
	tsec = new->security;
	if (!strcmp(name, "exec")) {
		tsec->exec_sid = sid;
	} else if (!strcmp(name, "fscreate")) {
		tsec->create_sid = sid;
	} else if (!strcmp(name, "keycreate")) {
		error = may_create_key(sid, p);
		if (error)
			goto abort_change;
		tsec->keycreate_sid = sid;
	} else if (!strcmp(name, "sockcreate")) {
		tsec->sockcreate_sid = sid;
	} else if (!strcmp(name, "current")) {
=======
	   operation.  See selinux_bprm_creds_for_exec for the execve
	   checks and may_create for the file creation checks. The
	   operation will then fail if the context is not permitted. */
	tsec = selinux_cred(new);
	if (attr == LSM_ATTR_EXEC) {
		tsec->exec_sid = sid;
	} else if (attr == LSM_ATTR_FSCREATE) {
		tsec->create_sid = sid;
	} else if (attr == LSM_ATTR_KEYCREATE) {
		if (sid) {
			error = avc_has_perm(mysid, sid,
					     SECCLASS_KEY, KEY__CREATE, NULL);
			if (error)
				goto abort_change;
		}
		tsec->keycreate_sid = sid;
	} else if (attr == LSM_ATTR_SOCKCREATE) {
		tsec->sockcreate_sid = sid;
	} else if (attr == LSM_ATTR_CURRENT) {
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		error = -EINVAL;
		if (sid == 0)
			goto abort_change;

<<<<<<< HEAD
		/* Only allow single threaded processes to change context */
		error = -EPERM;
=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		if (!current_is_single_threaded()) {
			error = security_bounded_transition(tsec->sid, sid);
			if (error)
				goto abort_change;
		}

		/* Check permissions for the transition. */
		error = avc_has_perm(tsec->sid, sid, SECCLASS_PROCESS,
				     PROCESS__DYNTRANSITION, NULL);
		if (error)
			goto abort_change;

		/* Check for ptracing, and update the task SID if ok.
		   Otherwise, leave SID unchanged and fail. */
<<<<<<< HEAD
		ptsid = 0;
		rcu_read_lock();
		tracer = ptrace_parent(p);
		if (tracer)
			ptsid = task_sid(tracer);
		rcu_read_unlock();

		if (tracer) {
=======
		ptsid = ptrace_parent_sid();
		if (ptsid != 0) {
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
			error = avc_has_perm(ptsid, sid, SECCLASS_PROCESS,
					     PROCESS__PTRACE, NULL);
			if (error)
				goto abort_change;
		}

		tsec->sid = sid;
	} else {
		error = -EINVAL;
		goto abort_change;
	}

	commit_creds(new);
	return size;

abort_change:
	abort_creds(new);
	return error;
}

<<<<<<< HEAD
static int selinux_secid_to_secctx(u32 secid, char **secdata, u32 *seclen)
{
	return security_sid_to_context(secid, secdata, seclen);
=======
/**
 * selinux_getselfattr - Get SELinux current task attributes
 * @attr: the requested attribute
 * @ctx: buffer to receive the result
 * @size: buffer size (input), buffer size used (output)
 * @flags: unused
 *
 * Fill the passed user space @ctx with the details of the requested
 * attribute.
 *
 * Returns the number of attributes on success, an error code otherwise.
 * There will only ever be one attribute.
 */
static int selinux_getselfattr(unsigned int attr, struct lsm_ctx __user *ctx,
			       u32 *size, u32 flags)
{
	int rc;
	char *val = NULL;
	int val_len;

	val_len = selinux_lsm_getattr(attr, current, &val);
	if (val_len < 0)
		return val_len;
	rc = lsm_fill_user_ctx(ctx, size, val, val_len, LSM_ID_SELINUX, 0);
	kfree(val);
	return (!rc ? 1 : rc);
}

static int selinux_setselfattr(unsigned int attr, struct lsm_ctx *ctx,
			       u32 size, u32 flags)
{
	int rc;

	rc = selinux_lsm_setattr(attr, ctx->ctx, ctx->ctx_len);
	if (rc > 0)
		return 0;
	return rc;
}

static int selinux_getprocattr(struct task_struct *p,
			       const char *name, char **value)
{
	unsigned int attr = lsm_name_to_attr(name);
	int rc;

	if (attr) {
		rc = selinux_lsm_getattr(attr, p, value);
		if (rc != -EOPNOTSUPP)
			return rc;
	}

	return -EINVAL;
}

static int selinux_setprocattr(const char *name, void *value, size_t size)
{
	int attr = lsm_name_to_attr(name);

	if (attr)
		return selinux_lsm_setattr(attr, value, size);
	return -EINVAL;
}

static int selinux_ismaclabel(const char *name)
{
	return (strcmp(name, XATTR_SELINUX_SUFFIX) == 0);
}

static int selinux_secid_to_secctx(u32 secid, char **secdata, u32 *seclen)
{
	return security_sid_to_context(secid,
				       secdata, seclen);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

static int selinux_secctx_to_secid(const char *secdata, u32 seclen, u32 *secid)
{
<<<<<<< HEAD
	return security_context_to_sid(secdata, seclen, secid);
=======
	return security_context_to_sid(secdata, seclen,
				       secid, GFP_KERNEL);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

static void selinux_release_secctx(char *secdata, u32 seclen)
{
	kfree(secdata);
}

<<<<<<< HEAD
=======
static void selinux_inode_invalidate_secctx(struct inode *inode)
{
	struct inode_security_struct *isec = selinux_inode(inode);

	spin_lock(&isec->lock);
	isec->initialized = LABEL_INVALID;
	spin_unlock(&isec->lock);
}

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
/*
 *	called with inode->i_mutex locked
 */
static int selinux_inode_notifysecctx(struct inode *inode, void *ctx, u32 ctxlen)
{
<<<<<<< HEAD
	return selinux_inode_setsecurity(inode, XATTR_SELINUX_SUFFIX, ctx, ctxlen, 0);
=======
	int rc = selinux_inode_setsecurity(inode, XATTR_SELINUX_SUFFIX,
					   ctx, ctxlen, 0);
	/* Do not return error when suppressing label (SBLABEL_MNT not set). */
	return rc == -EOPNOTSUPP ? 0 : rc;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

/*
 *	called with inode->i_mutex locked
 */
static int selinux_inode_setsecctx(struct dentry *dentry, void *ctx, u32 ctxlen)
{
<<<<<<< HEAD
	return __vfs_setxattr_noperm(dentry, XATTR_NAME_SELINUX, ctx, ctxlen, 0);
=======
	return __vfs_setxattr_noperm(&nop_mnt_idmap, dentry, XATTR_NAME_SELINUX,
				     ctx, ctxlen, 0);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

static int selinux_inode_getsecctx(struct inode *inode, void **ctx, u32 *ctxlen)
{
	int len = 0;
<<<<<<< HEAD
	len = selinux_inode_getsecurity(inode, XATTR_SELINUX_SUFFIX,
						ctx, true);
=======
	len = selinux_inode_getsecurity(&nop_mnt_idmap, inode,
					XATTR_SELINUX_SUFFIX, ctx, true);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	if (len < 0)
		return len;
	*ctxlen = len;
	return 0;
}
#ifdef CONFIG_KEYS

static int selinux_key_alloc(struct key *k, const struct cred *cred,
			     unsigned long flags)
{
	const struct task_security_struct *tsec;
	struct key_security_struct *ksec;

	ksec = kzalloc(sizeof(struct key_security_struct), GFP_KERNEL);
	if (!ksec)
		return -ENOMEM;

<<<<<<< HEAD
	tsec = cred->security;
=======
	tsec = selinux_cred(cred);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	if (tsec->keycreate_sid)
		ksec->sid = tsec->keycreate_sid;
	else
		ksec->sid = tsec->sid;

	k->security = ksec;
	return 0;
}

static void selinux_key_free(struct key *k)
{
	struct key_security_struct *ksec = k->security;

	k->security = NULL;
	kfree(ksec);
}

static int selinux_key_permission(key_ref_t key_ref,
				  const struct cred *cred,
<<<<<<< HEAD
				  key_perm_t perm)
{
	struct key *key;
	struct key_security_struct *ksec;
	u32 sid;

	/* if no specific permissions are requested, we skip the
	   permission check. No serious, additional covert channels
	   appear to be created. */
	if (perm == 0)
		return 0;

	sid = cred_sid(cred);

=======
				  enum key_need_perm need_perm)
{
	struct key *key;
	struct key_security_struct *ksec;
	u32 perm, sid;

	switch (need_perm) {
	case KEY_NEED_VIEW:
		perm = KEY__VIEW;
		break;
	case KEY_NEED_READ:
		perm = KEY__READ;
		break;
	case KEY_NEED_WRITE:
		perm = KEY__WRITE;
		break;
	case KEY_NEED_SEARCH:
		perm = KEY__SEARCH;
		break;
	case KEY_NEED_LINK:
		perm = KEY__LINK;
		break;
	case KEY_NEED_SETATTR:
		perm = KEY__SETATTR;
		break;
	case KEY_NEED_UNLINK:
	case KEY_SYSADMIN_OVERRIDE:
	case KEY_AUTHTOKEN_OVERRIDE:
	case KEY_DEFER_PERM_CHECK:
		return 0;
	default:
		WARN_ON(1);
		return -EPERM;

	}

	sid = cred_sid(cred);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	key = key_ref_to_ptr(key_ref);
	ksec = key->security;

	return avc_has_perm(sid, ksec->sid, SECCLASS_KEY, perm, NULL);
}

static int selinux_key_getsecurity(struct key *key, char **_buffer)
{
	struct key_security_struct *ksec = key->security;
	char *context = NULL;
	unsigned len;
	int rc;

<<<<<<< HEAD
	rc = security_sid_to_context(ksec->sid, &context, &len);
=======
	rc = security_sid_to_context(ksec->sid,
				     &context, &len);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	if (!rc)
		rc = len;
	*_buffer = context;
	return rc;
}

<<<<<<< HEAD
#endif

static struct security_operations selinux_ops = {
	.name =				"selinux",

	.binder_set_context_mgr =	selinux_binder_set_context_mgr,
	.binder_transaction =		selinux_binder_transaction,
	.binder_transfer_binder =	selinux_binder_transfer_binder,
	.binder_transfer_file =		selinux_binder_transfer_file,

	.ptrace_access_check =		selinux_ptrace_access_check,
	.ptrace_traceme =		selinux_ptrace_traceme,
	.capget =			selinux_capget,
	.capset =			selinux_capset,
	.capable =			selinux_capable,
	.quotactl =			selinux_quotactl,
	.quota_on =			selinux_quota_on,
	.syslog =			selinux_syslog,
	.vm_enough_memory =		selinux_vm_enough_memory,

	.netlink_send =			selinux_netlink_send,

	.bprm_set_creds =		selinux_bprm_set_creds,
	.bprm_committing_creds =	selinux_bprm_committing_creds,
	.bprm_committed_creds =		selinux_bprm_committed_creds,
	.bprm_secureexec =		selinux_bprm_secureexec,

	.sb_alloc_security =		selinux_sb_alloc_security,
	.sb_free_security =		selinux_sb_free_security,
	.sb_copy_data =			selinux_sb_copy_data,
	.sb_remount =			selinux_sb_remount,
	.sb_kern_mount =		selinux_sb_kern_mount,
	.sb_show_options =		selinux_sb_show_options,
	.sb_statfs =			selinux_sb_statfs,
	.sb_mount =			selinux_mount,
	.sb_umount =			selinux_umount,
	.sb_set_mnt_opts =		selinux_set_mnt_opts,
	.sb_clone_mnt_opts =		selinux_sb_clone_mnt_opts,
	.sb_parse_opts_str = 		selinux_parse_opts_str,


	.inode_alloc_security =		selinux_inode_alloc_security,
	.inode_free_security =		selinux_inode_free_security,
	.inode_init_security =		selinux_inode_init_security,
	.inode_create =			selinux_inode_create,
	.inode_post_create =		selinux_inode_post_create,
	.inode_link =			selinux_inode_link,
	.inode_unlink =			selinux_inode_unlink,
	.inode_symlink =		selinux_inode_symlink,
	.inode_mkdir =			selinux_inode_mkdir,
	.inode_rmdir =			selinux_inode_rmdir,
	.inode_mknod =			selinux_inode_mknod,
	.inode_rename =			selinux_inode_rename,
	.inode_readlink =		selinux_inode_readlink,
	.inode_follow_link =		selinux_inode_follow_link,
	.inode_permission =		selinux_inode_permission,
	.inode_setattr =		selinux_inode_setattr,
	.inode_getattr =		selinux_inode_getattr,
	.inode_setxattr =		selinux_inode_setxattr,
	.inode_post_setxattr =		selinux_inode_post_setxattr,
	.inode_getxattr =		selinux_inode_getxattr,
	.inode_listxattr =		selinux_inode_listxattr,
	.inode_removexattr =		selinux_inode_removexattr,
	.inode_getsecurity =		selinux_inode_getsecurity,
	.inode_setsecurity =		selinux_inode_setsecurity,
	.inode_listsecurity =		selinux_inode_listsecurity,
	.inode_getsecid =		selinux_inode_getsecid,

	.file_permission =		selinux_file_permission,
	.file_alloc_security =		selinux_file_alloc_security,
	.file_free_security =		selinux_file_free_security,
	.file_ioctl =			selinux_file_ioctl,
	.file_mmap =			selinux_file_mmap,
	.file_mprotect =		selinux_file_mprotect,
	.file_lock =			selinux_file_lock,
	.file_fcntl =			selinux_file_fcntl,
	.file_set_fowner =		selinux_file_set_fowner,
	.file_send_sigiotask =		selinux_file_send_sigiotask,
	.file_receive =			selinux_file_receive,

	.dentry_open =			selinux_dentry_open,
	.file_close =			selinux_file_close,
	.allow_merge_bio =		selinux_allow_merge_bio,

	.task_create =			selinux_task_create,
	.cred_alloc_blank =		selinux_cred_alloc_blank,
	.cred_free =			selinux_cred_free,
	.cred_prepare =			selinux_cred_prepare,
	.cred_transfer =		selinux_cred_transfer,
	.kernel_act_as =		selinux_kernel_act_as,
	.kernel_create_files_as =	selinux_kernel_create_files_as,
	.kernel_module_request =	selinux_kernel_module_request,
	.task_setpgid =			selinux_task_setpgid,
	.task_getpgid =			selinux_task_getpgid,
	.task_getsid =			selinux_task_getsid,
	.task_getsecid =		selinux_task_getsecid,
	.task_setnice =			selinux_task_setnice,
	.task_setioprio =		selinux_task_setioprio,
	.task_getioprio =		selinux_task_getioprio,
	.task_setrlimit =		selinux_task_setrlimit,
	.task_setscheduler =		selinux_task_setscheduler,
	.task_getscheduler =		selinux_task_getscheduler,
	.task_movememory =		selinux_task_movememory,
	.task_kill =			selinux_task_kill,
	.task_wait =			selinux_task_wait,
	.task_to_inode =		selinux_task_to_inode,

	.ipc_permission =		selinux_ipc_permission,
	.ipc_getsecid =			selinux_ipc_getsecid,

	.msg_msg_alloc_security =	selinux_msg_msg_alloc_security,
	.msg_msg_free_security =	selinux_msg_msg_free_security,

	.msg_queue_alloc_security =	selinux_msg_queue_alloc_security,
	.msg_queue_free_security =	selinux_msg_queue_free_security,
	.msg_queue_associate =		selinux_msg_queue_associate,
	.msg_queue_msgctl =		selinux_msg_queue_msgctl,
	.msg_queue_msgsnd =		selinux_msg_queue_msgsnd,
	.msg_queue_msgrcv =		selinux_msg_queue_msgrcv,

	.shm_alloc_security =		selinux_shm_alloc_security,
	.shm_free_security =		selinux_shm_free_security,
	.shm_associate =		selinux_shm_associate,
	.shm_shmctl =			selinux_shm_shmctl,
	.shm_shmat =			selinux_shm_shmat,

	.sem_alloc_security =		selinux_sem_alloc_security,
	.sem_free_security =		selinux_sem_free_security,
	.sem_associate =		selinux_sem_associate,
	.sem_semctl =			selinux_sem_semctl,
	.sem_semop =			selinux_sem_semop,

	.d_instantiate =		selinux_d_instantiate,

	.getprocattr =			selinux_getprocattr,
	.setprocattr =			selinux_setprocattr,

	.secid_to_secctx =		selinux_secid_to_secctx,
	.secctx_to_secid =		selinux_secctx_to_secid,
	.release_secctx =		selinux_release_secctx,
	.inode_notifysecctx =		selinux_inode_notifysecctx,
	.inode_setsecctx =		selinux_inode_setsecctx,
	.inode_getsecctx =		selinux_inode_getsecctx,

	.unix_stream_connect =		selinux_socket_unix_stream_connect,
	.unix_may_send =		selinux_socket_unix_may_send,

	.socket_create =		selinux_socket_create,
	.socket_post_create =		selinux_socket_post_create,
	.socket_bind =			selinux_socket_bind,
	.socket_connect =		selinux_socket_connect,
	.socket_listen =		selinux_socket_listen,
	.socket_accept =		selinux_socket_accept,
	.socket_sendmsg =		selinux_socket_sendmsg,
	.socket_recvmsg =		selinux_socket_recvmsg,
	.socket_getsockname =		selinux_socket_getsockname,
	.socket_getpeername =		selinux_socket_getpeername,
	.socket_getsockopt =		selinux_socket_getsockopt,
	.socket_setsockopt =		selinux_socket_setsockopt,
	.socket_shutdown =		selinux_socket_shutdown,
	.socket_sock_rcv_skb =		selinux_socket_sock_rcv_skb,
	.socket_getpeersec_stream =	selinux_socket_getpeersec_stream,
	.socket_getpeersec_dgram =	selinux_socket_getpeersec_dgram,
	.sk_alloc_security =		selinux_sk_alloc_security,
	.sk_free_security =		selinux_sk_free_security,
	.sk_clone_security =		selinux_sk_clone_security,
	.sk_getsecid =			selinux_sk_getsecid,
	.sock_graft =			selinux_sock_graft,
	.inet_conn_request =		selinux_inet_conn_request,
	.inet_csk_clone =		selinux_inet_csk_clone,
	.inet_conn_established =	selinux_inet_conn_established,
	.secmark_relabel_packet =	selinux_secmark_relabel_packet,
	.secmark_refcount_inc =		selinux_secmark_refcount_inc,
	.secmark_refcount_dec =		selinux_secmark_refcount_dec,
	.req_classify_flow =		selinux_req_classify_flow,
	.tun_dev_create =		selinux_tun_dev_create,
	.tun_dev_post_create = 		selinux_tun_dev_post_create,
	.tun_dev_attach =		selinux_tun_dev_attach,

#ifdef CONFIG_SECURITY_NETWORK_XFRM
	.xfrm_policy_alloc_security =	selinux_xfrm_policy_alloc,
	.xfrm_policy_clone_security =	selinux_xfrm_policy_clone,
	.xfrm_policy_free_security =	selinux_xfrm_policy_free,
	.xfrm_policy_delete_security =	selinux_xfrm_policy_delete,
	.xfrm_state_alloc_security =	selinux_xfrm_state_alloc,
	.xfrm_state_free_security =	selinux_xfrm_state_free,
	.xfrm_state_delete_security =	selinux_xfrm_state_delete,
	.xfrm_policy_lookup =		selinux_xfrm_policy_lookup,
	.xfrm_state_pol_flow_match =	selinux_xfrm_state_pol_flow_match,
	.xfrm_decode_session =		selinux_xfrm_decode_session,
#endif

#ifdef CONFIG_KEYS
	.key_alloc =			selinux_key_alloc,
	.key_free =			selinux_key_free,
	.key_permission =		selinux_key_permission,
	.key_getsecurity =		selinux_key_getsecurity,
#endif

#ifdef CONFIG_AUDIT
	.audit_rule_init =		selinux_audit_rule_init,
	.audit_rule_known =		selinux_audit_rule_known,
	.audit_rule_match =		selinux_audit_rule_match,
	.audit_rule_free =		selinux_audit_rule_free,
=======
#ifdef CONFIG_KEY_NOTIFICATIONS
static int selinux_watch_key(struct key *key)
{
	struct key_security_struct *ksec = key->security;
	u32 sid = current_sid();

	return avc_has_perm(sid, ksec->sid, SECCLASS_KEY, KEY__VIEW, NULL);
}
#endif
#endif

#ifdef CONFIG_SECURITY_INFINIBAND
static int selinux_ib_pkey_access(void *ib_sec, u64 subnet_prefix, u16 pkey_val)
{
	struct common_audit_data ad;
	int err;
	u32 sid = 0;
	struct ib_security_struct *sec = ib_sec;
	struct lsm_ibpkey_audit ibpkey;

	err = sel_ib_pkey_sid(subnet_prefix, pkey_val, &sid);
	if (err)
		return err;

	ad.type = LSM_AUDIT_DATA_IBPKEY;
	ibpkey.subnet_prefix = subnet_prefix;
	ibpkey.pkey = pkey_val;
	ad.u.ibpkey = &ibpkey;
	return avc_has_perm(sec->sid, sid,
			    SECCLASS_INFINIBAND_PKEY,
			    INFINIBAND_PKEY__ACCESS, &ad);
}

static int selinux_ib_endport_manage_subnet(void *ib_sec, const char *dev_name,
					    u8 port_num)
{
	struct common_audit_data ad;
	int err;
	u32 sid = 0;
	struct ib_security_struct *sec = ib_sec;
	struct lsm_ibendport_audit ibendport;

	err = security_ib_endport_sid(dev_name, port_num,
				      &sid);

	if (err)
		return err;

	ad.type = LSM_AUDIT_DATA_IBENDPORT;
	ibendport.dev_name = dev_name;
	ibendport.port = port_num;
	ad.u.ibendport = &ibendport;
	return avc_has_perm(sec->sid, sid,
			    SECCLASS_INFINIBAND_ENDPORT,
			    INFINIBAND_ENDPORT__MANAGE_SUBNET, &ad);
}

static int selinux_ib_alloc_security(void **ib_sec)
{
	struct ib_security_struct *sec;

	sec = kzalloc(sizeof(*sec), GFP_KERNEL);
	if (!sec)
		return -ENOMEM;
	sec->sid = current_sid();

	*ib_sec = sec;
	return 0;
}

static void selinux_ib_free_security(void *ib_sec)
{
	kfree(ib_sec);
}
#endif

#ifdef CONFIG_BPF_SYSCALL
static int selinux_bpf(int cmd, union bpf_attr *attr,
				     unsigned int size)
{
	u32 sid = current_sid();
	int ret;

	switch (cmd) {
	case BPF_MAP_CREATE:
		ret = avc_has_perm(sid, sid, SECCLASS_BPF, BPF__MAP_CREATE,
				   NULL);
		break;
	case BPF_PROG_LOAD:
		ret = avc_has_perm(sid, sid, SECCLASS_BPF, BPF__PROG_LOAD,
				   NULL);
		break;
	default:
		ret = 0;
		break;
	}

	return ret;
}

static u32 bpf_map_fmode_to_av(fmode_t fmode)
{
	u32 av = 0;

	if (fmode & FMODE_READ)
		av |= BPF__MAP_READ;
	if (fmode & FMODE_WRITE)
		av |= BPF__MAP_WRITE;
	return av;
}

/* This function will check the file pass through unix socket or binder to see
 * if it is a bpf related object. And apply corresponding checks on the bpf
 * object based on the type. The bpf maps and programs, not like other files and
 * socket, are using a shared anonymous inode inside the kernel as their inode.
 * So checking that inode cannot identify if the process have privilege to
 * access the bpf object and that's why we have to add this additional check in
 * selinux_file_receive and selinux_binder_transfer_files.
 */
static int bpf_fd_pass(const struct file *file, u32 sid)
{
	struct bpf_security_struct *bpfsec;
	struct bpf_prog *prog;
	struct bpf_map *map;
	int ret;

	if (file->f_op == &bpf_map_fops) {
		map = file->private_data;
		bpfsec = map->security;
		ret = avc_has_perm(sid, bpfsec->sid, SECCLASS_BPF,
				   bpf_map_fmode_to_av(file->f_mode), NULL);
		if (ret)
			return ret;
	} else if (file->f_op == &bpf_prog_fops) {
		prog = file->private_data;
		bpfsec = prog->aux->security;
		ret = avc_has_perm(sid, bpfsec->sid, SECCLASS_BPF,
				   BPF__PROG_RUN, NULL);
		if (ret)
			return ret;
	}
	return 0;
}

static int selinux_bpf_map(struct bpf_map *map, fmode_t fmode)
{
	u32 sid = current_sid();
	struct bpf_security_struct *bpfsec;

	bpfsec = map->security;
	return avc_has_perm(sid, bpfsec->sid, SECCLASS_BPF,
			    bpf_map_fmode_to_av(fmode), NULL);
}

static int selinux_bpf_prog(struct bpf_prog *prog)
{
	u32 sid = current_sid();
	struct bpf_security_struct *bpfsec;

	bpfsec = prog->aux->security;
	return avc_has_perm(sid, bpfsec->sid, SECCLASS_BPF,
			    BPF__PROG_RUN, NULL);
}

static int selinux_bpf_map_create(struct bpf_map *map, union bpf_attr *attr,
				  struct bpf_token *token)
{
	struct bpf_security_struct *bpfsec;

	bpfsec = kzalloc(sizeof(*bpfsec), GFP_KERNEL);
	if (!bpfsec)
		return -ENOMEM;

	bpfsec->sid = current_sid();
	map->security = bpfsec;

	return 0;
}

static void selinux_bpf_map_free(struct bpf_map *map)
{
	struct bpf_security_struct *bpfsec = map->security;

	map->security = NULL;
	kfree(bpfsec);
}

static int selinux_bpf_prog_load(struct bpf_prog *prog, union bpf_attr *attr,
				 struct bpf_token *token)
{
	struct bpf_security_struct *bpfsec;

	bpfsec = kzalloc(sizeof(*bpfsec), GFP_KERNEL);
	if (!bpfsec)
		return -ENOMEM;

	bpfsec->sid = current_sid();
	prog->aux->security = bpfsec;

	return 0;
}

static void selinux_bpf_prog_free(struct bpf_prog *prog)
{
	struct bpf_security_struct *bpfsec = prog->aux->security;

	prog->aux->security = NULL;
	kfree(bpfsec);
}

static int selinux_bpf_token_create(struct bpf_token *token, union bpf_attr *attr,
				    struct path *path)
{
	struct bpf_security_struct *bpfsec;

	bpfsec = kzalloc(sizeof(*bpfsec), GFP_KERNEL);
	if (!bpfsec)
		return -ENOMEM;

	bpfsec->sid = current_sid();
	token->security = bpfsec;

	return 0;
}

static void selinux_bpf_token_free(struct bpf_token *token)
{
	struct bpf_security_struct *bpfsec = token->security;

	token->security = NULL;
	kfree(bpfsec);
}
#endif

struct lsm_blob_sizes selinux_blob_sizes __ro_after_init = {
	.lbs_cred = sizeof(struct task_security_struct),
	.lbs_file = sizeof(struct file_security_struct),
	.lbs_inode = sizeof(struct inode_security_struct),
	.lbs_ipc = sizeof(struct ipc_security_struct),
	.lbs_msg_msg = sizeof(struct msg_security_struct),
	.lbs_superblock = sizeof(struct superblock_security_struct),
	.lbs_xattr_count = SELINUX_INODE_INIT_XATTRS,
};

#ifdef CONFIG_PERF_EVENTS
static int selinux_perf_event_open(struct perf_event_attr *attr, int type)
{
	u32 requested, sid = current_sid();

	if (type == PERF_SECURITY_OPEN)
		requested = PERF_EVENT__OPEN;
	else if (type == PERF_SECURITY_CPU)
		requested = PERF_EVENT__CPU;
	else if (type == PERF_SECURITY_KERNEL)
		requested = PERF_EVENT__KERNEL;
	else if (type == PERF_SECURITY_TRACEPOINT)
		requested = PERF_EVENT__TRACEPOINT;
	else
		return -EINVAL;

	return avc_has_perm(sid, sid, SECCLASS_PERF_EVENT,
			    requested, NULL);
}

static int selinux_perf_event_alloc(struct perf_event *event)
{
	struct perf_event_security_struct *perfsec;

	perfsec = kzalloc(sizeof(*perfsec), GFP_KERNEL);
	if (!perfsec)
		return -ENOMEM;

	perfsec->sid = current_sid();
	event->security = perfsec;

	return 0;
}

static void selinux_perf_event_free(struct perf_event *event)
{
	struct perf_event_security_struct *perfsec = event->security;

	event->security = NULL;
	kfree(perfsec);
}

static int selinux_perf_event_read(struct perf_event *event)
{
	struct perf_event_security_struct *perfsec = event->security;
	u32 sid = current_sid();

	return avc_has_perm(sid, perfsec->sid,
			    SECCLASS_PERF_EVENT, PERF_EVENT__READ, NULL);
}

static int selinux_perf_event_write(struct perf_event *event)
{
	struct perf_event_security_struct *perfsec = event->security;
	u32 sid = current_sid();

	return avc_has_perm(sid, perfsec->sid,
			    SECCLASS_PERF_EVENT, PERF_EVENT__WRITE, NULL);
}
#endif

#ifdef CONFIG_IO_URING
/**
 * selinux_uring_override_creds - check the requested cred override
 * @new: the target creds
 *
 * Check to see if the current task is allowed to override it's credentials
 * to service an io_uring operation.
 */
static int selinux_uring_override_creds(const struct cred *new)
{
	return avc_has_perm(current_sid(), cred_sid(new),
			    SECCLASS_IO_URING, IO_URING__OVERRIDE_CREDS, NULL);
}

/**
 * selinux_uring_sqpoll - check if a io_uring polling thread can be created
 *
 * Check to see if the current task is allowed to create a new io_uring
 * kernel polling thread.
 */
static int selinux_uring_sqpoll(void)
{
	u32 sid = current_sid();

	return avc_has_perm(sid, sid,
			    SECCLASS_IO_URING, IO_URING__SQPOLL, NULL);
}

/**
 * selinux_uring_cmd - check if IORING_OP_URING_CMD is allowed
 * @ioucmd: the io_uring command structure
 *
 * Check to see if the current domain is allowed to execute an
 * IORING_OP_URING_CMD against the device/file specified in @ioucmd.
 *
 */
static int selinux_uring_cmd(struct io_uring_cmd *ioucmd)
{
	struct file *file = ioucmd->file;
	struct inode *inode = file_inode(file);
	struct inode_security_struct *isec = selinux_inode(inode);
	struct common_audit_data ad;

	ad.type = LSM_AUDIT_DATA_FILE;
	ad.u.file = file;

	return avc_has_perm(current_sid(), isec->sid,
			    SECCLASS_IO_URING, IO_URING__CMD, &ad);
}
#endif /* CONFIG_IO_URING */

static const struct lsm_id selinux_lsmid = {
	.name = "selinux",
	.id = LSM_ID_SELINUX,
};

/*
 * IMPORTANT NOTE: When adding new hooks, please be careful to keep this order:
 * 1. any hooks that don't belong to (2.) or (3.) below,
 * 2. hooks that both access structures allocated by other hooks, and allocate
 *    structures that can be later accessed by other hooks (mostly "cloning"
 *    hooks),
 * 3. hooks that only allocate structures that can be later accessed by other
 *    hooks ("allocating" hooks).
 *
 * Please follow block comment delimiters in the list to keep this order.
 */
static struct security_hook_list selinux_hooks[] __ro_after_init = {
	LSM_HOOK_INIT(binder_set_context_mgr, selinux_binder_set_context_mgr),
	LSM_HOOK_INIT(binder_transaction, selinux_binder_transaction),
	LSM_HOOK_INIT(binder_transfer_binder, selinux_binder_transfer_binder),
	LSM_HOOK_INIT(binder_transfer_file, selinux_binder_transfer_file),

	LSM_HOOK_INIT(ptrace_access_check, selinux_ptrace_access_check),
	LSM_HOOK_INIT(ptrace_traceme, selinux_ptrace_traceme),
	LSM_HOOK_INIT(capget, selinux_capget),
	LSM_HOOK_INIT(capset, selinux_capset),
	LSM_HOOK_INIT(capable, selinux_capable),
	LSM_HOOK_INIT(quotactl, selinux_quotactl),
	LSM_HOOK_INIT(quota_on, selinux_quota_on),
	LSM_HOOK_INIT(syslog, selinux_syslog),
	LSM_HOOK_INIT(vm_enough_memory, selinux_vm_enough_memory),

	LSM_HOOK_INIT(netlink_send, selinux_netlink_send),

	LSM_HOOK_INIT(bprm_creds_for_exec, selinux_bprm_creds_for_exec),
	LSM_HOOK_INIT(bprm_committing_creds, selinux_bprm_committing_creds),
	LSM_HOOK_INIT(bprm_committed_creds, selinux_bprm_committed_creds),

	LSM_HOOK_INIT(sb_free_mnt_opts, selinux_free_mnt_opts),
	LSM_HOOK_INIT(sb_mnt_opts_compat, selinux_sb_mnt_opts_compat),
	LSM_HOOK_INIT(sb_remount, selinux_sb_remount),
	LSM_HOOK_INIT(sb_kern_mount, selinux_sb_kern_mount),
	LSM_HOOK_INIT(sb_show_options, selinux_sb_show_options),
	LSM_HOOK_INIT(sb_statfs, selinux_sb_statfs),
	LSM_HOOK_INIT(sb_mount, selinux_mount),
	LSM_HOOK_INIT(sb_umount, selinux_umount),
	LSM_HOOK_INIT(sb_set_mnt_opts, selinux_set_mnt_opts),
	LSM_HOOK_INIT(sb_clone_mnt_opts, selinux_sb_clone_mnt_opts),

	LSM_HOOK_INIT(move_mount, selinux_move_mount),

	LSM_HOOK_INIT(dentry_init_security, selinux_dentry_init_security),
	LSM_HOOK_INIT(dentry_create_files_as, selinux_dentry_create_files_as),

	LSM_HOOK_INIT(inode_free_security, selinux_inode_free_security),
	LSM_HOOK_INIT(inode_init_security, selinux_inode_init_security),
	LSM_HOOK_INIT(inode_init_security_anon, selinux_inode_init_security_anon),
	LSM_HOOK_INIT(inode_create, selinux_inode_create),
	LSM_HOOK_INIT(inode_link, selinux_inode_link),
	LSM_HOOK_INIT(inode_unlink, selinux_inode_unlink),
	LSM_HOOK_INIT(inode_symlink, selinux_inode_symlink),
	LSM_HOOK_INIT(inode_mkdir, selinux_inode_mkdir),
	LSM_HOOK_INIT(inode_rmdir, selinux_inode_rmdir),
	LSM_HOOK_INIT(inode_mknod, selinux_inode_mknod),
	LSM_HOOK_INIT(inode_rename, selinux_inode_rename),
	LSM_HOOK_INIT(inode_readlink, selinux_inode_readlink),
	LSM_HOOK_INIT(inode_follow_link, selinux_inode_follow_link),
	LSM_HOOK_INIT(inode_permission, selinux_inode_permission),
	LSM_HOOK_INIT(inode_setattr, selinux_inode_setattr),
	LSM_HOOK_INIT(inode_getattr, selinux_inode_getattr),
	LSM_HOOK_INIT(inode_setxattr, selinux_inode_setxattr),
	LSM_HOOK_INIT(inode_post_setxattr, selinux_inode_post_setxattr),
	LSM_HOOK_INIT(inode_getxattr, selinux_inode_getxattr),
	LSM_HOOK_INIT(inode_listxattr, selinux_inode_listxattr),
	LSM_HOOK_INIT(inode_removexattr, selinux_inode_removexattr),
	LSM_HOOK_INIT(inode_set_acl, selinux_inode_set_acl),
	LSM_HOOK_INIT(inode_get_acl, selinux_inode_get_acl),
	LSM_HOOK_INIT(inode_remove_acl, selinux_inode_remove_acl),
	LSM_HOOK_INIT(inode_getsecurity, selinux_inode_getsecurity),
	LSM_HOOK_INIT(inode_setsecurity, selinux_inode_setsecurity),
	LSM_HOOK_INIT(inode_listsecurity, selinux_inode_listsecurity),
	LSM_HOOK_INIT(inode_getsecid, selinux_inode_getsecid),
	LSM_HOOK_INIT(inode_copy_up, selinux_inode_copy_up),
	LSM_HOOK_INIT(inode_copy_up_xattr, selinux_inode_copy_up_xattr),
	LSM_HOOK_INIT(path_notify, selinux_path_notify),

	LSM_HOOK_INIT(kernfs_init_security, selinux_kernfs_init_security),

	LSM_HOOK_INIT(file_permission, selinux_file_permission),
	LSM_HOOK_INIT(file_alloc_security, selinux_file_alloc_security),
	LSM_HOOK_INIT(file_ioctl, selinux_file_ioctl),
	LSM_HOOK_INIT(file_ioctl_compat, selinux_file_ioctl_compat),
	LSM_HOOK_INIT(mmap_file, selinux_mmap_file),
	LSM_HOOK_INIT(mmap_addr, selinux_mmap_addr),
	LSM_HOOK_INIT(file_mprotect, selinux_file_mprotect),
	LSM_HOOK_INIT(file_lock, selinux_file_lock),
	LSM_HOOK_INIT(file_fcntl, selinux_file_fcntl),
	LSM_HOOK_INIT(file_set_fowner, selinux_file_set_fowner),
	LSM_HOOK_INIT(file_send_sigiotask, selinux_file_send_sigiotask),
	LSM_HOOK_INIT(file_receive, selinux_file_receive),

	LSM_HOOK_INIT(file_open, selinux_file_open),

	LSM_HOOK_INIT(task_alloc, selinux_task_alloc),
	LSM_HOOK_INIT(cred_prepare, selinux_cred_prepare),
	LSM_HOOK_INIT(cred_transfer, selinux_cred_transfer),
	LSM_HOOK_INIT(cred_getsecid, selinux_cred_getsecid),
	LSM_HOOK_INIT(kernel_act_as, selinux_kernel_act_as),
	LSM_HOOK_INIT(kernel_create_files_as, selinux_kernel_create_files_as),
	LSM_HOOK_INIT(kernel_module_request, selinux_kernel_module_request),
	LSM_HOOK_INIT(kernel_load_data, selinux_kernel_load_data),
	LSM_HOOK_INIT(kernel_read_file, selinux_kernel_read_file),
	LSM_HOOK_INIT(task_setpgid, selinux_task_setpgid),
	LSM_HOOK_INIT(task_getpgid, selinux_task_getpgid),
	LSM_HOOK_INIT(task_getsid, selinux_task_getsid),
	LSM_HOOK_INIT(current_getsecid_subj, selinux_current_getsecid_subj),
	LSM_HOOK_INIT(task_getsecid_obj, selinux_task_getsecid_obj),
	LSM_HOOK_INIT(task_setnice, selinux_task_setnice),
	LSM_HOOK_INIT(task_setioprio, selinux_task_setioprio),
	LSM_HOOK_INIT(task_getioprio, selinux_task_getioprio),
	LSM_HOOK_INIT(task_prlimit, selinux_task_prlimit),
	LSM_HOOK_INIT(task_setrlimit, selinux_task_setrlimit),
	LSM_HOOK_INIT(task_setscheduler, selinux_task_setscheduler),
	LSM_HOOK_INIT(task_getscheduler, selinux_task_getscheduler),
	LSM_HOOK_INIT(task_movememory, selinux_task_movememory),
	LSM_HOOK_INIT(task_kill, selinux_task_kill),
	LSM_HOOK_INIT(task_to_inode, selinux_task_to_inode),
	LSM_HOOK_INIT(userns_create, selinux_userns_create),

	LSM_HOOK_INIT(ipc_permission, selinux_ipc_permission),
	LSM_HOOK_INIT(ipc_getsecid, selinux_ipc_getsecid),

	LSM_HOOK_INIT(msg_queue_associate, selinux_msg_queue_associate),
	LSM_HOOK_INIT(msg_queue_msgctl, selinux_msg_queue_msgctl),
	LSM_HOOK_INIT(msg_queue_msgsnd, selinux_msg_queue_msgsnd),
	LSM_HOOK_INIT(msg_queue_msgrcv, selinux_msg_queue_msgrcv),

	LSM_HOOK_INIT(shm_associate, selinux_shm_associate),
	LSM_HOOK_INIT(shm_shmctl, selinux_shm_shmctl),
	LSM_HOOK_INIT(shm_shmat, selinux_shm_shmat),

	LSM_HOOK_INIT(sem_associate, selinux_sem_associate),
	LSM_HOOK_INIT(sem_semctl, selinux_sem_semctl),
	LSM_HOOK_INIT(sem_semop, selinux_sem_semop),

	LSM_HOOK_INIT(d_instantiate, selinux_d_instantiate),

	LSM_HOOK_INIT(getselfattr, selinux_getselfattr),
	LSM_HOOK_INIT(setselfattr, selinux_setselfattr),
	LSM_HOOK_INIT(getprocattr, selinux_getprocattr),
	LSM_HOOK_INIT(setprocattr, selinux_setprocattr),

	LSM_HOOK_INIT(ismaclabel, selinux_ismaclabel),
	LSM_HOOK_INIT(secctx_to_secid, selinux_secctx_to_secid),
	LSM_HOOK_INIT(release_secctx, selinux_release_secctx),
	LSM_HOOK_INIT(inode_invalidate_secctx, selinux_inode_invalidate_secctx),
	LSM_HOOK_INIT(inode_notifysecctx, selinux_inode_notifysecctx),
	LSM_HOOK_INIT(inode_setsecctx, selinux_inode_setsecctx),

	LSM_HOOK_INIT(unix_stream_connect, selinux_socket_unix_stream_connect),
	LSM_HOOK_INIT(unix_may_send, selinux_socket_unix_may_send),

	LSM_HOOK_INIT(socket_create, selinux_socket_create),
	LSM_HOOK_INIT(socket_post_create, selinux_socket_post_create),
	LSM_HOOK_INIT(socket_socketpair, selinux_socket_socketpair),
	LSM_HOOK_INIT(socket_bind, selinux_socket_bind),
	LSM_HOOK_INIT(socket_connect, selinux_socket_connect),
	LSM_HOOK_INIT(socket_listen, selinux_socket_listen),
	LSM_HOOK_INIT(socket_accept, selinux_socket_accept),
	LSM_HOOK_INIT(socket_sendmsg, selinux_socket_sendmsg),
	LSM_HOOK_INIT(socket_recvmsg, selinux_socket_recvmsg),
	LSM_HOOK_INIT(socket_getsockname, selinux_socket_getsockname),
	LSM_HOOK_INIT(socket_getpeername, selinux_socket_getpeername),
	LSM_HOOK_INIT(socket_getsockopt, selinux_socket_getsockopt),
	LSM_HOOK_INIT(socket_setsockopt, selinux_socket_setsockopt),
	LSM_HOOK_INIT(socket_shutdown, selinux_socket_shutdown),
	LSM_HOOK_INIT(socket_sock_rcv_skb, selinux_socket_sock_rcv_skb),
	LSM_HOOK_INIT(socket_getpeersec_stream,
			selinux_socket_getpeersec_stream),
	LSM_HOOK_INIT(socket_getpeersec_dgram, selinux_socket_getpeersec_dgram),
	LSM_HOOK_INIT(sk_free_security, selinux_sk_free_security),
	LSM_HOOK_INIT(sk_clone_security, selinux_sk_clone_security),
	LSM_HOOK_INIT(sk_getsecid, selinux_sk_getsecid),
	LSM_HOOK_INIT(sock_graft, selinux_sock_graft),
	LSM_HOOK_INIT(sctp_assoc_request, selinux_sctp_assoc_request),
	LSM_HOOK_INIT(sctp_sk_clone, selinux_sctp_sk_clone),
	LSM_HOOK_INIT(sctp_bind_connect, selinux_sctp_bind_connect),
	LSM_HOOK_INIT(sctp_assoc_established, selinux_sctp_assoc_established),
	LSM_HOOK_INIT(mptcp_add_subflow, selinux_mptcp_add_subflow),
	LSM_HOOK_INIT(inet_conn_request, selinux_inet_conn_request),
	LSM_HOOK_INIT(inet_csk_clone, selinux_inet_csk_clone),
	LSM_HOOK_INIT(inet_conn_established, selinux_inet_conn_established),
	LSM_HOOK_INIT(secmark_relabel_packet, selinux_secmark_relabel_packet),
	LSM_HOOK_INIT(secmark_refcount_inc, selinux_secmark_refcount_inc),
	LSM_HOOK_INIT(secmark_refcount_dec, selinux_secmark_refcount_dec),
	LSM_HOOK_INIT(req_classify_flow, selinux_req_classify_flow),
	LSM_HOOK_INIT(tun_dev_free_security, selinux_tun_dev_free_security),
	LSM_HOOK_INIT(tun_dev_create, selinux_tun_dev_create),
	LSM_HOOK_INIT(tun_dev_attach_queue, selinux_tun_dev_attach_queue),
	LSM_HOOK_INIT(tun_dev_attach, selinux_tun_dev_attach),
	LSM_HOOK_INIT(tun_dev_open, selinux_tun_dev_open),
#ifdef CONFIG_SECURITY_INFINIBAND
	LSM_HOOK_INIT(ib_pkey_access, selinux_ib_pkey_access),
	LSM_HOOK_INIT(ib_endport_manage_subnet,
		      selinux_ib_endport_manage_subnet),
	LSM_HOOK_INIT(ib_free_security, selinux_ib_free_security),
#endif
#ifdef CONFIG_SECURITY_NETWORK_XFRM
	LSM_HOOK_INIT(xfrm_policy_free_security, selinux_xfrm_policy_free),
	LSM_HOOK_INIT(xfrm_policy_delete_security, selinux_xfrm_policy_delete),
	LSM_HOOK_INIT(xfrm_state_free_security, selinux_xfrm_state_free),
	LSM_HOOK_INIT(xfrm_state_delete_security, selinux_xfrm_state_delete),
	LSM_HOOK_INIT(xfrm_policy_lookup, selinux_xfrm_policy_lookup),
	LSM_HOOK_INIT(xfrm_state_pol_flow_match,
			selinux_xfrm_state_pol_flow_match),
	LSM_HOOK_INIT(xfrm_decode_session, selinux_xfrm_decode_session),
#endif

#ifdef CONFIG_KEYS
	LSM_HOOK_INIT(key_free, selinux_key_free),
	LSM_HOOK_INIT(key_permission, selinux_key_permission),
	LSM_HOOK_INIT(key_getsecurity, selinux_key_getsecurity),
#ifdef CONFIG_KEY_NOTIFICATIONS
	LSM_HOOK_INIT(watch_key, selinux_watch_key),
#endif
#endif

#ifdef CONFIG_AUDIT
	LSM_HOOK_INIT(audit_rule_known, selinux_audit_rule_known),
	LSM_HOOK_INIT(audit_rule_match, selinux_audit_rule_match),
	LSM_HOOK_INIT(audit_rule_free, selinux_audit_rule_free),
#endif

#ifdef CONFIG_BPF_SYSCALL
	LSM_HOOK_INIT(bpf, selinux_bpf),
	LSM_HOOK_INIT(bpf_map, selinux_bpf_map),
	LSM_HOOK_INIT(bpf_prog, selinux_bpf_prog),
	LSM_HOOK_INIT(bpf_map_free, selinux_bpf_map_free),
	LSM_HOOK_INIT(bpf_prog_free, selinux_bpf_prog_free),
	LSM_HOOK_INIT(bpf_token_free, selinux_bpf_token_free),
#endif

#ifdef CONFIG_PERF_EVENTS
	LSM_HOOK_INIT(perf_event_open, selinux_perf_event_open),
	LSM_HOOK_INIT(perf_event_free, selinux_perf_event_free),
	LSM_HOOK_INIT(perf_event_read, selinux_perf_event_read),
	LSM_HOOK_INIT(perf_event_write, selinux_perf_event_write),
#endif

#ifdef CONFIG_IO_URING
	LSM_HOOK_INIT(uring_override_creds, selinux_uring_override_creds),
	LSM_HOOK_INIT(uring_sqpoll, selinux_uring_sqpoll),
	LSM_HOOK_INIT(uring_cmd, selinux_uring_cmd),
#endif

	/*
	 * PUT "CLONING" (ACCESSING + ALLOCATING) HOOKS HERE
	 */
	LSM_HOOK_INIT(fs_context_submount, selinux_fs_context_submount),
	LSM_HOOK_INIT(fs_context_dup, selinux_fs_context_dup),
	LSM_HOOK_INIT(fs_context_parse_param, selinux_fs_context_parse_param),
	LSM_HOOK_INIT(sb_eat_lsm_opts, selinux_sb_eat_lsm_opts),
#ifdef CONFIG_SECURITY_NETWORK_XFRM
	LSM_HOOK_INIT(xfrm_policy_clone_security, selinux_xfrm_policy_clone),
#endif

	/*
	 * PUT "ALLOCATING" HOOKS HERE
	 */
	LSM_HOOK_INIT(msg_msg_alloc_security, selinux_msg_msg_alloc_security),
	LSM_HOOK_INIT(msg_queue_alloc_security,
		      selinux_msg_queue_alloc_security),
	LSM_HOOK_INIT(shm_alloc_security, selinux_shm_alloc_security),
	LSM_HOOK_INIT(sb_alloc_security, selinux_sb_alloc_security),
	LSM_HOOK_INIT(inode_alloc_security, selinux_inode_alloc_security),
	LSM_HOOK_INIT(sem_alloc_security, selinux_sem_alloc_security),
	LSM_HOOK_INIT(secid_to_secctx, selinux_secid_to_secctx),
	LSM_HOOK_INIT(inode_getsecctx, selinux_inode_getsecctx),
	LSM_HOOK_INIT(sk_alloc_security, selinux_sk_alloc_security),
	LSM_HOOK_INIT(tun_dev_alloc_security, selinux_tun_dev_alloc_security),
#ifdef CONFIG_SECURITY_INFINIBAND
	LSM_HOOK_INIT(ib_alloc_security, selinux_ib_alloc_security),
#endif
#ifdef CONFIG_SECURITY_NETWORK_XFRM
	LSM_HOOK_INIT(xfrm_policy_alloc_security, selinux_xfrm_policy_alloc),
	LSM_HOOK_INIT(xfrm_state_alloc, selinux_xfrm_state_alloc),
	LSM_HOOK_INIT(xfrm_state_alloc_acquire,
		      selinux_xfrm_state_alloc_acquire),
#endif
#ifdef CONFIG_KEYS
	LSM_HOOK_INIT(key_alloc, selinux_key_alloc),
#endif
#ifdef CONFIG_AUDIT
	LSM_HOOK_INIT(audit_rule_init, selinux_audit_rule_init),
#endif
#ifdef CONFIG_BPF_SYSCALL
	LSM_HOOK_INIT(bpf_map_create, selinux_bpf_map_create),
	LSM_HOOK_INIT(bpf_prog_load, selinux_bpf_prog_load),
	LSM_HOOK_INIT(bpf_token_create, selinux_bpf_token_create),
#endif
#ifdef CONFIG_PERF_EVENTS
	LSM_HOOK_INIT(perf_event_alloc, selinux_perf_event_alloc),
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#endif
};

static __init int selinux_init(void)
{
<<<<<<< HEAD
	if (!security_module_enable(&selinux_ops)) {
		selinux_enabled = 0;
		return 0;
	}

	if (!selinux_enabled) {
		printk(KERN_INFO "SELinux:  Disabled at boot.\n");
		return 0;
	}

	printk(KERN_INFO "SELinux:  Initializing.\n");
=======
	pr_info("SELinux:  Initializing.\n");

	memset(&selinux_state, 0, sizeof(selinux_state));
	enforcing_set(selinux_enforcing_boot);
	selinux_avc_init();
	mutex_init(&selinux_state.status_lock);
	mutex_init(&selinux_state.policy_mutex);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	/* Set the security state for the initial task. */
	cred_init_security();

	default_noexec = !(VM_DATA_DEFAULT_FLAGS & VM_EXEC);
<<<<<<< HEAD

	sel_inode_cache = kmem_cache_create("selinux_inode_security",
					    sizeof(struct inode_security_struct),
					    0, SLAB_PANIC, NULL);
	avc_init();

	if (register_security(&selinux_ops))
		panic("SELinux: Unable to register with kernel.\n");

	if (selinux_enforcing)
		printk(KERN_DEBUG "SELinux:  Starting in enforcing mode\n");
	else
		printk(KERN_DEBUG "SELinux:  Starting in permissive mode\n");
=======
	if (!default_noexec)
		pr_notice("SELinux:  virtual memory is executable by default\n");

	avc_init();

	avtab_cache_init();

	ebitmap_cache_init();

	hashtab_cache_init();

	security_add_hooks(selinux_hooks, ARRAY_SIZE(selinux_hooks),
			   &selinux_lsmid);

	if (avc_add_callback(selinux_netcache_avc_callback, AVC_CALLBACK_RESET))
		panic("SELinux: Unable to register AVC netcache callback\n");

	if (avc_add_callback(selinux_lsm_notifier_avc_callback, AVC_CALLBACK_RESET))
		panic("SELinux: Unable to register AVC LSM notifier callback\n");

	if (selinux_enforcing_boot)
		pr_debug("SELinux:  Starting in enforcing mode\n");
	else
		pr_debug("SELinux:  Starting in permissive mode\n");

	fs_validate_description("selinux", selinux_fs_parameters);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	return 0;
}

static void delayed_superblock_init(struct super_block *sb, void *unused)
{
<<<<<<< HEAD
	superblock_doinit(sb, NULL);
=======
	selinux_set_mnt_opts(sb, NULL, 0, NULL);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

void selinux_complete_init(void)
{
<<<<<<< HEAD
	printk(KERN_DEBUG "SELinux:  Completing initialization.\n");

	/* Set up any superblocks initialized prior to the policy load. */
	printk(KERN_DEBUG "SELinux:  Setting up existing superblocks.\n");
=======
	pr_debug("SELinux:  Completing initialization.\n");

	/* Set up any superblocks initialized prior to the policy load. */
	pr_debug("SELinux:  Setting up existing superblocks.\n");
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	iterate_supers(delayed_superblock_init, NULL);
}

/* SELinux requires early initialization in order to label
   all processes and objects when they are created. */
<<<<<<< HEAD
security_initcall(selinux_init);

#if defined(CONFIG_NETFILTER)

static struct nf_hook_ops selinux_ipv4_ops[] = {
	{
		.hook =		selinux_ipv4_postroute,
		.owner =	THIS_MODULE,
		.pf =		PF_INET,
=======
DEFINE_LSM(selinux) = {
	.name = "selinux",
	.flags = LSM_FLAG_LEGACY_MAJOR | LSM_FLAG_EXCLUSIVE,
	.enabled = &selinux_enabled_boot,
	.blobs = &selinux_blob_sizes,
	.init = selinux_init,
};

#if defined(CONFIG_NETFILTER)
static const struct nf_hook_ops selinux_nf_ops[] = {
	{
		.hook =		selinux_ip_postroute,
		.pf =		NFPROTO_IPV4,
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		.hooknum =	NF_INET_POST_ROUTING,
		.priority =	NF_IP_PRI_SELINUX_LAST,
	},
	{
<<<<<<< HEAD
		.hook =		selinux_ipv4_forward,
		.owner =	THIS_MODULE,
		.pf =		PF_INET,
=======
		.hook =		selinux_ip_forward,
		.pf =		NFPROTO_IPV4,
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		.hooknum =	NF_INET_FORWARD,
		.priority =	NF_IP_PRI_SELINUX_FIRST,
	},
	{
<<<<<<< HEAD
		.hook =		selinux_ipv4_output,
		.owner =	THIS_MODULE,
		.pf =		PF_INET,
		.hooknum =	NF_INET_LOCAL_OUT,
		.priority =	NF_IP_PRI_SELINUX_FIRST,
	}
};

#if defined(CONFIG_IPV6) || defined(CONFIG_IPV6_MODULE)

static struct nf_hook_ops selinux_ipv6_ops[] = {
	{
		.hook =		selinux_ipv6_postroute,
		.owner =	THIS_MODULE,
		.pf =		PF_INET6,
=======
		.hook =		selinux_ip_output,
		.pf =		NFPROTO_IPV4,
		.hooknum =	NF_INET_LOCAL_OUT,
		.priority =	NF_IP_PRI_SELINUX_FIRST,
	},
#if IS_ENABLED(CONFIG_IPV6)
	{
		.hook =		selinux_ip_postroute,
		.pf =		NFPROTO_IPV6,
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		.hooknum =	NF_INET_POST_ROUTING,
		.priority =	NF_IP6_PRI_SELINUX_LAST,
	},
	{
<<<<<<< HEAD
		.hook =		selinux_ipv6_forward,
		.owner =	THIS_MODULE,
		.pf =		PF_INET6,
		.hooknum =	NF_INET_FORWARD,
		.priority =	NF_IP6_PRI_SELINUX_FIRST,
	}
};

#endif	/* IPV6 */

static int __init selinux_nf_ip_init(void)
{
	int err = 0;

	if (!selinux_enabled)
		goto out;

	printk(KERN_DEBUG "SELinux:  Registering netfilter hooks\n");

	err = nf_register_hooks(selinux_ipv4_ops, ARRAY_SIZE(selinux_ipv4_ops));
	if (err)
		panic("SELinux: nf_register_hooks for IPv4: error %d\n", err);

#if defined(CONFIG_IPV6) || defined(CONFIG_IPV6_MODULE)
	err = nf_register_hooks(selinux_ipv6_ops, ARRAY_SIZE(selinux_ipv6_ops));
	if (err)
		panic("SELinux: nf_register_hooks for IPv6: error %d\n", err);
#endif	/* IPV6 */

out:
	return err;
}

__initcall(selinux_nf_ip_init);

#ifdef CONFIG_SECURITY_SELINUX_DISABLE
static void selinux_nf_ip_exit(void)
{
	printk(KERN_DEBUG "SELinux:  Unregistering netfilter hooks\n");

	nf_unregister_hooks(selinux_ipv4_ops, ARRAY_SIZE(selinux_ipv4_ops));
#if defined(CONFIG_IPV6) || defined(CONFIG_IPV6_MODULE)
	nf_unregister_hooks(selinux_ipv6_ops, ARRAY_SIZE(selinux_ipv6_ops));
#endif	/* IPV6 */
}
#endif

#else /* CONFIG_NETFILTER */

#ifdef CONFIG_SECURITY_SELINUX_DISABLE
#define selinux_nf_ip_exit()
#endif

#endif /* CONFIG_NETFILTER */

#ifdef CONFIG_SECURITY_SELINUX_DISABLE
static int selinux_disabled;

int selinux_disable(void)
{
	if (ss_initialized) {
		/* Not permitted after initial policy load. */
		return -EINVAL;
	}

	if (selinux_disabled) {
		/* Only do this once. */
		return -EINVAL;
	}

	printk(KERN_INFO "SELinux:  Disabled at runtime.\n");

	selinux_disabled = 1;
	selinux_enabled = 0;

	reset_security_ops();

	/* Try to destroy the avc node cache */
	avc_disable();

	/* Unregister netfilter hooks. */
	selinux_nf_ip_exit();

	/* Unregister selinuxfs. */
	exit_sel_fs();

	return 0;
}
#endif
=======
		.hook =		selinux_ip_forward,
		.pf =		NFPROTO_IPV6,
		.hooknum =	NF_INET_FORWARD,
		.priority =	NF_IP6_PRI_SELINUX_FIRST,
	},
	{
		.hook =		selinux_ip_output,
		.pf =		NFPROTO_IPV6,
		.hooknum =	NF_INET_LOCAL_OUT,
		.priority =	NF_IP6_PRI_SELINUX_FIRST,
	},
#endif	/* IPV6 */
};

static int __net_init selinux_nf_register(struct net *net)
{
	return nf_register_net_hooks(net, selinux_nf_ops,
				     ARRAY_SIZE(selinux_nf_ops));
}

static void __net_exit selinux_nf_unregister(struct net *net)
{
	nf_unregister_net_hooks(net, selinux_nf_ops,
				ARRAY_SIZE(selinux_nf_ops));
}

static struct pernet_operations selinux_net_ops = {
	.init = selinux_nf_register,
	.exit = selinux_nf_unregister,
};

static int __init selinux_nf_ip_init(void)
{
	int err;

	if (!selinux_enabled_boot)
		return 0;

	pr_debug("SELinux:  Registering netfilter hooks\n");

	err = register_pernet_subsys(&selinux_net_ops);
	if (err)
		panic("SELinux: register_pernet_subsys: error %d\n", err);

	return 0;
}
__initcall(selinux_nf_ip_init);
#endif /* CONFIG_NETFILTER */
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
