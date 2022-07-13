<<<<<<< HEAD
=======
// SPDX-License-Identifier: GPL-2.0-only
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
/* Updated: Karl MacMillan <kmacmillan@tresys.com>
 *
 *	Added conditional policy language extensions
 *
 *  Updated: Hewlett-Packard <paul@paul-moore.com>
 *
 *	Added support for the policy capability bitmap
 *
 * Copyright (C) 2007 Hewlett-Packard Development Company, L.P.
 * Copyright (C) 2003 - 2004 Tresys Technology, LLC
 * Copyright (C) 2004 Red Hat, Inc., James Morris <jmorris@redhat.com>
<<<<<<< HEAD
 *	This program is free software; you can redistribute it and/or modify
 *	it under the terms of the GNU General Public License as published by
 *	the Free Software Foundation, version 2.
=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 */

#include <linux/kernel.h>
#include <linux/pagemap.h>
#include <linux/slab.h>
#include <linux/vmalloc.h>
#include <linux/fs.h>
<<<<<<< HEAD
#include <linux/mutex.h>
=======
#include <linux/fs_context.h>
#include <linux/mount.h>
#include <linux/mutex.h>
#include <linux/namei.h>
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#include <linux/init.h>
#include <linux/string.h>
#include <linux/security.h>
#include <linux/major.h>
#include <linux/seq_file.h>
#include <linux/percpu.h>
#include <linux/audit.h>
#include <linux/uaccess.h>
#include <linux/kobject.h>
#include <linux/ctype.h>

/* selinuxfs pseudo filesystem for exporting the security policy API.
   Based on the proc code and the fs/nfsd/nfsctl.c code. */

#include "flask.h"
#include "avc.h"
#include "avc_ss.h"
#include "security.h"
#include "objsec.h"
#include "conditional.h"
<<<<<<< HEAD

/* Policy capability filenames */
static char *policycap_names[] = {
	"network_peer_controls",
	"open_perms"
};

unsigned int selinux_checkreqprot = CONFIG_SECURITY_SELINUX_CHECKREQPROT_VALUE;

static int __init checkreqprot_setup(char *str)
{
	unsigned long checkreqprot;
	if (!strict_strtoul(str, 0, &checkreqprot))
		selinux_checkreqprot = checkreqprot ? 1 : 0;
	return 1;
}
__setup("checkreqprot=", checkreqprot_setup);

static DEFINE_MUTEX(sel_mutex);

/* global data for booleans */
static struct dentry *bool_dir;
static int bool_num;
static char **bool_pending_names;
static int *bool_pending_values;

/* global data for classes */
static struct dentry *class_dir;
static unsigned long last_class_ino;

static char policy_opened;

/* global data for policy capabilities */
static struct dentry *policycap_dir;

/* Check whether a task is allowed to use a security operation. */
static int task_has_security(struct task_struct *tsk,
			     u32 perms)
{
	const struct task_security_struct *tsec;
	u32 sid = 0;

	rcu_read_lock();
	tsec = __task_cred(tsk)->security;
	if (tsec)
		sid = tsec->sid;
	rcu_read_unlock();
	if (!tsec)
		return -EACCES;

	return avc_has_perm(sid, SECINITSID_SECURITY,
			    SECCLASS_SECURITY, perms, NULL);
}
=======
#include "ima.h"
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

enum sel_inos {
	SEL_ROOT_INO = 2,
	SEL_LOAD,	/* load policy */
	SEL_ENFORCE,	/* get or set enforcing status */
	SEL_CONTEXT,	/* validate context */
	SEL_ACCESS,	/* compute access decision */
	SEL_CREATE,	/* compute create labeling decision */
	SEL_RELABEL,	/* compute relabeling decision */
	SEL_USER,	/* compute reachable user contexts */
	SEL_POLICYVERS,	/* return policy version for this kernel */
	SEL_COMMIT_BOOLS, /* commit new boolean values */
	SEL_MLS,	/* return if MLS policy is enabled */
	SEL_DISABLE,	/* disable SELinux until next reboot */
	SEL_MEMBER,	/* compute polyinstantiation membership decision */
	SEL_CHECKREQPROT, /* check requested protection, not kernel-applied one */
	SEL_COMPAT_NET,	/* whether to use old compat network packet controls */
	SEL_REJECT_UNKNOWN, /* export unknown reject handling to userspace */
	SEL_DENY_UNKNOWN, /* export unknown deny handling to userspace */
	SEL_STATUS,	/* export current status using mmap() */
	SEL_POLICY,	/* allow userspace to read the in kernel policy */
<<<<<<< HEAD
	SEL_INO_NEXT,	/* The next inode number to use */
};

static unsigned long sel_last_ino = SEL_INO_NEXT - 1;
=======
	SEL_VALIDATE_TRANS, /* compute validatetrans decision */
	SEL_INO_NEXT,	/* The next inode number to use */
};

struct selinux_fs_info {
	struct dentry *bool_dir;
	unsigned int bool_num;
	char **bool_pending_names;
	int *bool_pending_values;
	struct dentry *class_dir;
	unsigned long last_class_ino;
	bool policy_opened;
	struct dentry *policycap_dir;
	unsigned long last_ino;
	struct super_block *sb;
};

static int selinux_fs_info_create(struct super_block *sb)
{
	struct selinux_fs_info *fsi;

	fsi = kzalloc(sizeof(*fsi), GFP_KERNEL);
	if (!fsi)
		return -ENOMEM;

	fsi->last_ino = SEL_INO_NEXT - 1;
	fsi->sb = sb;
	sb->s_fs_info = fsi;
	return 0;
}

static void selinux_fs_info_free(struct super_block *sb)
{
	struct selinux_fs_info *fsi = sb->s_fs_info;
	unsigned int i;

	if (fsi) {
		for (i = 0; i < fsi->bool_num; i++)
			kfree(fsi->bool_pending_names[i]);
		kfree(fsi->bool_pending_names);
		kfree(fsi->bool_pending_values);
	}
	kfree(sb->s_fs_info);
	sb->s_fs_info = NULL;
}
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

#define SEL_INITCON_INO_OFFSET		0x01000000
#define SEL_BOOL_INO_OFFSET		0x02000000
#define SEL_CLASS_INO_OFFSET		0x04000000
#define SEL_POLICYCAP_INO_OFFSET	0x08000000
#define SEL_INO_MASK			0x00ffffff

<<<<<<< HEAD
=======
#define BOOL_DIR_NAME "booleans"
#define CLASS_DIR_NAME "class"
#define POLICYCAP_DIR_NAME "policy_capabilities"

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#define TMPBUFLEN	12
static ssize_t sel_read_enforce(struct file *filp, char __user *buf,
				size_t count, loff_t *ppos)
{
	char tmpbuf[TMPBUFLEN];
	ssize_t length;

<<<<<<< HEAD
	length = scnprintf(tmpbuf, TMPBUFLEN, "%d", selinux_enforcing);
=======
	length = scnprintf(tmpbuf, TMPBUFLEN, "%d",
			   enforcing_enabled());
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	return simple_read_from_buffer(buf, count, ppos, tmpbuf, length);
}

#ifdef CONFIG_SECURITY_SELINUX_DEVELOP
static ssize_t sel_write_enforce(struct file *file, const char __user *buf,
				 size_t count, loff_t *ppos)

{
	char *page = NULL;
	ssize_t length;
<<<<<<< HEAD
	int new_value;

	length = -ENOMEM;
	if (count >= PAGE_SIZE)
		goto out;

	/* No partial writes. */
	length = -EINVAL;
	if (*ppos != 0)
		goto out;

	length = -ENOMEM;
	page = (char *)get_zeroed_page(GFP_KERNEL);
	if (!page)
		goto out;

	length = -EFAULT;
	if (copy_from_user(page, buf, count))
		goto out;

	length = -EINVAL;
	if (sscanf(page, "%d", &new_value) != 1)
		goto out;

	if (new_value != selinux_enforcing) {
		length = task_has_security(current, SECURITY__SETENFORCE);
		if (length)
			goto out;
		audit_log(current->audit_context, GFP_KERNEL, AUDIT_MAC_STATUS,
			"enforcing=%d old_enforcing=%d auid=%u ses=%u",
			new_value, selinux_enforcing,
			audit_get_loginuid(current),
			audit_get_sessionid(current));
		selinux_enforcing = new_value;
		if (selinux_enforcing)
			avc_ss_reset(0);
		selnl_notify_setenforce(selinux_enforcing);
		selinux_status_update_setenforce(selinux_enforcing);
	}
	length = count;
out:
	free_page((unsigned long) page);
=======
	int scan_value;
	bool old_value, new_value;

	if (count >= PAGE_SIZE)
		return -ENOMEM;

	/* No partial writes. */
	if (*ppos != 0)
		return -EINVAL;

	page = memdup_user_nul(buf, count);
	if (IS_ERR(page))
		return PTR_ERR(page);

	length = -EINVAL;
	if (sscanf(page, "%d", &scan_value) != 1)
		goto out;

	new_value = !!scan_value;

	old_value = enforcing_enabled();
	if (new_value != old_value) {
		length = avc_has_perm(current_sid(), SECINITSID_SECURITY,
				      SECCLASS_SECURITY, SECURITY__SETENFORCE,
				      NULL);
		if (length)
			goto out;
		audit_log(audit_context(), GFP_KERNEL, AUDIT_MAC_STATUS,
			"enforcing=%d old_enforcing=%d auid=%u ses=%u"
			" enabled=1 old-enabled=1 lsm=selinux res=1",
			new_value, old_value,
			from_kuid(&init_user_ns, audit_get_loginuid(current)),
			audit_get_sessionid(current));
		enforcing_set(new_value);
		if (new_value)
			avc_ss_reset(0);
		selnl_notify_setenforce(new_value);
		selinux_status_update_setenforce(new_value);
		if (!new_value)
			call_blocking_lsm_notifier(LSM_POLICY_CHANGE, NULL);

		selinux_ima_measure_state();
	}
	length = count;
out:
	kfree(page);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	return length;
}
#else
#define sel_write_enforce NULL
#endif

static const struct file_operations sel_enforce_ops = {
	.read		= sel_read_enforce,
	.write		= sel_write_enforce,
	.llseek		= generic_file_llseek,
};

static ssize_t sel_read_handle_unknown(struct file *filp, char __user *buf,
					size_t count, loff_t *ppos)
{
	char tmpbuf[TMPBUFLEN];
	ssize_t length;
<<<<<<< HEAD
	ino_t ino = filp->f_path.dentry->d_inode->i_ino;
	int handle_unknown = (ino == SEL_REJECT_UNKNOWN) ?
		security_get_reject_unknown() : !security_get_allow_unknown();
=======
	ino_t ino = file_inode(filp)->i_ino;
	int handle_unknown = (ino == SEL_REJECT_UNKNOWN) ?
		security_get_reject_unknown() :
		!security_get_allow_unknown();
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	length = scnprintf(tmpbuf, TMPBUFLEN, "%d", handle_unknown);
	return simple_read_from_buffer(buf, count, ppos, tmpbuf, length);
}

static const struct file_operations sel_handle_unknown_ops = {
	.read		= sel_read_handle_unknown,
	.llseek		= generic_file_llseek,
};

static int sel_open_handle_status(struct inode *inode, struct file *filp)
{
	struct page    *status = selinux_kernel_status_page();

	if (!status)
		return -ENOMEM;

	filp->private_data = status;

	return 0;
}

static ssize_t sel_read_handle_status(struct file *filp, char __user *buf,
				      size_t count, loff_t *ppos)
{
	struct page    *status = filp->private_data;

	BUG_ON(!status);

	return simple_read_from_buffer(buf, count, ppos,
				       page_address(status),
				       sizeof(struct selinux_kernel_status));
}

static int sel_mmap_handle_status(struct file *filp,
				  struct vm_area_struct *vma)
{
	struct page    *status = filp->private_data;
	unsigned long	size = vma->vm_end - vma->vm_start;

	BUG_ON(!status);

	/* only allows one page from the head */
	if (vma->vm_pgoff > 0 || size != PAGE_SIZE)
		return -EIO;
	/* disallow writable mapping */
	if (vma->vm_flags & VM_WRITE)
		return -EPERM;
	/* disallow mprotect() turns it into writable */
<<<<<<< HEAD
	vma->vm_flags &= ~VM_MAYWRITE;
=======
	vm_flags_clear(vma, VM_MAYWRITE);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	return remap_pfn_range(vma, vma->vm_start,
			       page_to_pfn(status),
			       size, vma->vm_page_prot);
}

static const struct file_operations sel_handle_status_ops = {
	.open		= sel_open_handle_status,
	.read		= sel_read_handle_status,
	.mmap		= sel_mmap_handle_status,
	.llseek		= generic_file_llseek,
};

<<<<<<< HEAD
#ifdef CONFIG_SECURITY_SELINUX_DISABLE
=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
static ssize_t sel_write_disable(struct file *file, const char __user *buf,
				 size_t count, loff_t *ppos)

{
<<<<<<< HEAD
	char *page = NULL;
	ssize_t length;
	int new_value;

	length = -ENOMEM;
	if (count >= PAGE_SIZE)
		goto out;

	/* No partial writes. */
	length = -EINVAL;
	if (*ppos != 0)
		goto out;

	length = -ENOMEM;
	page = (char *)get_zeroed_page(GFP_KERNEL);
	if (!page)
		goto out;

	length = -EFAULT;
	if (copy_from_user(page, buf, count))
		goto out;

	length = -EINVAL;
	if (sscanf(page, "%d", &new_value) != 1)
		goto out;

	if (new_value) {
		length = selinux_disable();
		if (length)
			goto out;
		audit_log(current->audit_context, GFP_KERNEL, AUDIT_MAC_STATUS,
			"selinux=0 auid=%u ses=%u",
			audit_get_loginuid(current),
			audit_get_sessionid(current));
	}

	length = count;
out:
	free_page((unsigned long) page);
	return length;
}
#else
#define sel_write_disable NULL
#endif
=======
	char *page;
	ssize_t length;
	int new_value;

	if (count >= PAGE_SIZE)
		return -ENOMEM;

	/* No partial writes. */
	if (*ppos != 0)
		return -EINVAL;

	page = memdup_user_nul(buf, count);
	if (IS_ERR(page))
		return PTR_ERR(page);

	if (sscanf(page, "%d", &new_value) != 1) {
		length = -EINVAL;
		goto out;
	}
	length = count;

	if (new_value) {
		pr_err("SELinux: https://github.com/SELinuxProject/selinux-kernel/wiki/DEPRECATE-runtime-disable\n");
		pr_err("SELinux: Runtime disable is not supported, use selinux=0 on the kernel cmdline.\n");
	}

out:
	kfree(page);
	return length;
}
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

static const struct file_operations sel_disable_ops = {
	.write		= sel_write_disable,
	.llseek		= generic_file_llseek,
};

static ssize_t sel_read_policyvers(struct file *filp, char __user *buf,
				   size_t count, loff_t *ppos)
{
	char tmpbuf[TMPBUFLEN];
	ssize_t length;

	length = scnprintf(tmpbuf, TMPBUFLEN, "%u", POLICYDB_VERSION_MAX);
	return simple_read_from_buffer(buf, count, ppos, tmpbuf, length);
}

static const struct file_operations sel_policyvers_ops = {
	.read		= sel_read_policyvers,
	.llseek		= generic_file_llseek,
};

/* declaration for sel_write_load */
<<<<<<< HEAD
static int sel_make_bools(void);
static int sel_make_classes(void);
static int sel_make_policycap(void);
=======
static int sel_make_bools(struct selinux_policy *newpolicy, struct dentry *bool_dir,
			  unsigned int *bool_num, char ***bool_pending_names,
			  int **bool_pending_values);
static int sel_make_classes(struct selinux_policy *newpolicy,
			    struct dentry *class_dir,
			    unsigned long *last_class_ino);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

/* declaration for sel_make_class_dirs */
static struct dentry *sel_make_dir(struct dentry *dir, const char *name,
			unsigned long *ino);

<<<<<<< HEAD
=======
/* declaration for sel_make_policy_nodes */
static struct dentry *sel_make_swapover_dir(struct super_block *sb,
						unsigned long *ino);

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
static ssize_t sel_read_mls(struct file *filp, char __user *buf,
				size_t count, loff_t *ppos)
{
	char tmpbuf[TMPBUFLEN];
	ssize_t length;

	length = scnprintf(tmpbuf, TMPBUFLEN, "%d",
			   security_mls_enabled());
	return simple_read_from_buffer(buf, count, ppos, tmpbuf, length);
}

static const struct file_operations sel_mls_ops = {
	.read		= sel_read_mls,
	.llseek		= generic_file_llseek,
};

struct policy_load_memory {
	size_t len;
	void *data;
};

static int sel_open_policy(struct inode *inode, struct file *filp)
{
<<<<<<< HEAD
=======
	struct selinux_fs_info *fsi = inode->i_sb->s_fs_info;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	struct policy_load_memory *plm = NULL;
	int rc;

	BUG_ON(filp->private_data);

<<<<<<< HEAD
	mutex_lock(&sel_mutex);

	rc = task_has_security(current, SECURITY__READ_POLICY);
=======
	mutex_lock(&selinux_state.policy_mutex);

	rc = avc_has_perm(current_sid(), SECINITSID_SECURITY,
			  SECCLASS_SECURITY, SECURITY__READ_POLICY, NULL);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	if (rc)
		goto err;

	rc = -EBUSY;
<<<<<<< HEAD
	if (policy_opened)
=======
	if (fsi->policy_opened)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		goto err;

	rc = -ENOMEM;
	plm = kzalloc(sizeof(*plm), GFP_KERNEL);
	if (!plm)
		goto err;

<<<<<<< HEAD
	if (i_size_read(inode) != security_policydb_len()) {
		mutex_lock(&inode->i_mutex);
		i_size_write(inode, security_policydb_len());
		mutex_unlock(&inode->i_mutex);
	}

=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	rc = security_read_policy(&plm->data, &plm->len);
	if (rc)
		goto err;

<<<<<<< HEAD
	policy_opened = 1;

	filp->private_data = plm;

	mutex_unlock(&sel_mutex);

	return 0;
err:
	mutex_unlock(&sel_mutex);
=======
	if ((size_t)i_size_read(inode) != plm->len) {
		inode_lock(inode);
		i_size_write(inode, plm->len);
		inode_unlock(inode);
	}

	fsi->policy_opened = 1;

	filp->private_data = plm;

	mutex_unlock(&selinux_state.policy_mutex);

	return 0;
err:
	mutex_unlock(&selinux_state.policy_mutex);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	if (plm)
		vfree(plm->data);
	kfree(plm);
	return rc;
}

static int sel_release_policy(struct inode *inode, struct file *filp)
{
<<<<<<< HEAD
=======
	struct selinux_fs_info *fsi = inode->i_sb->s_fs_info;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	struct policy_load_memory *plm = filp->private_data;

	BUG_ON(!plm);

<<<<<<< HEAD
	policy_opened = 0;
=======
	fsi->policy_opened = 0;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	vfree(plm->data);
	kfree(plm);

	return 0;
}

static ssize_t sel_read_policy(struct file *filp, char __user *buf,
			       size_t count, loff_t *ppos)
{
	struct policy_load_memory *plm = filp->private_data;
	int ret;

<<<<<<< HEAD
	mutex_lock(&sel_mutex);

	ret = task_has_security(current, SECURITY__READ_POLICY);
	if (ret)
		goto out;

	ret = simple_read_from_buffer(buf, count, ppos, plm->data, plm->len);
out:
	mutex_unlock(&sel_mutex);
	return ret;
}

static int sel_mmap_policy_fault(struct vm_area_struct *vma,
				 struct vm_fault *vmf)
{
	struct policy_load_memory *plm = vma->vm_file->private_data;
=======
	ret = avc_has_perm(current_sid(), SECINITSID_SECURITY,
			  SECCLASS_SECURITY, SECURITY__READ_POLICY, NULL);
	if (ret)
		return ret;

	return simple_read_from_buffer(buf, count, ppos, plm->data, plm->len);
}

static vm_fault_t sel_mmap_policy_fault(struct vm_fault *vmf)
{
	struct policy_load_memory *plm = vmf->vma->vm_file->private_data;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	unsigned long offset;
	struct page *page;

	if (vmf->flags & (FAULT_FLAG_MKWRITE | FAULT_FLAG_WRITE))
		return VM_FAULT_SIGBUS;

	offset = vmf->pgoff << PAGE_SHIFT;
	if (offset >= roundup(plm->len, PAGE_SIZE))
		return VM_FAULT_SIGBUS;

	page = vmalloc_to_page(plm->data + offset);
	get_page(page);

	vmf->page = page;

	return 0;
}

<<<<<<< HEAD
static struct vm_operations_struct sel_mmap_policy_ops = {
=======
static const struct vm_operations_struct sel_mmap_policy_ops = {
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	.fault = sel_mmap_policy_fault,
	.page_mkwrite = sel_mmap_policy_fault,
};

static int sel_mmap_policy(struct file *filp, struct vm_area_struct *vma)
{
	if (vma->vm_flags & VM_SHARED) {
		/* do not allow mprotect to make mapping writable */
<<<<<<< HEAD
		vma->vm_flags &= ~VM_MAYWRITE;
=======
		vm_flags_clear(vma, VM_MAYWRITE);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

		if (vma->vm_flags & VM_WRITE)
			return -EACCES;
	}

<<<<<<< HEAD
	vma->vm_flags |= VM_RESERVED;
=======
	vm_flags_set(vma, VM_DONTEXPAND | VM_DONTDUMP);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	vma->vm_ops = &sel_mmap_policy_ops;

	return 0;
}

static const struct file_operations sel_policy_ops = {
	.open		= sel_open_policy,
	.read		= sel_read_policy,
	.mmap		= sel_mmap_policy,
	.release	= sel_release_policy,
<<<<<<< HEAD
};

=======
	.llseek		= generic_file_llseek,
};

static void sel_remove_old_bool_data(unsigned int bool_num, char **bool_names,
				     int *bool_values)
{
	u32 i;

	/* bool_dir cleanup */
	for (i = 0; i < bool_num; i++)
		kfree(bool_names[i]);
	kfree(bool_names);
	kfree(bool_values);
}

static int sel_make_policy_nodes(struct selinux_fs_info *fsi,
				struct selinux_policy *newpolicy)
{
	int ret = 0;
	struct dentry *tmp_parent, *tmp_bool_dir, *tmp_class_dir;
	unsigned int bool_num = 0;
	char **bool_names = NULL;
	int *bool_values = NULL;
	unsigned long tmp_ino = fsi->last_ino; /* Don't increment last_ino in this function */

	tmp_parent = sel_make_swapover_dir(fsi->sb, &tmp_ino);
	if (IS_ERR(tmp_parent))
		return PTR_ERR(tmp_parent);

	tmp_ino = fsi->bool_dir->d_inode->i_ino - 1; /* sel_make_dir will increment and set */
	tmp_bool_dir = sel_make_dir(tmp_parent, BOOL_DIR_NAME, &tmp_ino);
	if (IS_ERR(tmp_bool_dir)) {
		ret = PTR_ERR(tmp_bool_dir);
		goto out;
	}

	tmp_ino = fsi->class_dir->d_inode->i_ino - 1; /* sel_make_dir will increment and set */
	tmp_class_dir = sel_make_dir(tmp_parent, CLASS_DIR_NAME, &tmp_ino);
	if (IS_ERR(tmp_class_dir)) {
		ret = PTR_ERR(tmp_class_dir);
		goto out;
	}

	ret = sel_make_bools(newpolicy, tmp_bool_dir, &bool_num,
			     &bool_names, &bool_values);
	if (ret)
		goto out;

	ret = sel_make_classes(newpolicy, tmp_class_dir,
			       &fsi->last_class_ino);
	if (ret)
		goto out;

	lock_rename(tmp_parent, fsi->sb->s_root);

	/* booleans */
	d_exchange(tmp_bool_dir, fsi->bool_dir);

	swap(fsi->bool_num, bool_num);
	swap(fsi->bool_pending_names, bool_names);
	swap(fsi->bool_pending_values, bool_values);

	fsi->bool_dir = tmp_bool_dir;

	/* classes */
	d_exchange(tmp_class_dir, fsi->class_dir);
	fsi->class_dir = tmp_class_dir;

	unlock_rename(tmp_parent, fsi->sb->s_root);

out:
	sel_remove_old_bool_data(bool_num, bool_names, bool_values);
	/* Since the other temporary dirs are children of tmp_parent
	 * this will handle all the cleanup in the case of a failure before
	 * the swapover
	 */
	simple_recursive_removal(tmp_parent, NULL);

	return ret;
}

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
static ssize_t sel_write_load(struct file *file, const char __user *buf,
			      size_t count, loff_t *ppos)

{
<<<<<<< HEAD
	ssize_t length;
	void *data = NULL;

	mutex_lock(&sel_mutex);

	length = task_has_security(current, SECURITY__LOAD_POLICY);
=======
	struct selinux_fs_info *fsi = file_inode(file)->i_sb->s_fs_info;
	struct selinux_load_state load_state;
	ssize_t length;
	void *data = NULL;

	mutex_lock(&selinux_state.policy_mutex);

	length = avc_has_perm(current_sid(), SECINITSID_SECURITY,
			      SECCLASS_SECURITY, SECURITY__LOAD_POLICY, NULL);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	if (length)
		goto out;

	/* No partial writes. */
	length = -EINVAL;
	if (*ppos != 0)
		goto out;

<<<<<<< HEAD
	length = -EFBIG;
	if (count > 64 * 1024 * 1024)
		goto out;

=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	length = -ENOMEM;
	data = vmalloc(count);
	if (!data)
		goto out;

	length = -EFAULT;
	if (copy_from_user(data, buf, count) != 0)
		goto out;

<<<<<<< HEAD
	length = security_load_policy(data, count);
	if (length)
		goto out;

	length = sel_make_bools();
	if (length)
		goto out1;

	length = sel_make_classes();
	if (length)
		goto out1;

	length = sel_make_policycap();
	if (length)
		goto out1;

	length = count;

out1:
	audit_log(current->audit_context, GFP_KERNEL, AUDIT_MAC_POLICY_LOAD,
		"policy loaded auid=%u ses=%u",
		audit_get_loginuid(current),
		audit_get_sessionid(current));
out:
	mutex_unlock(&sel_mutex);
=======
	length = security_load_policy(data, count, &load_state);
	if (length) {
		pr_warn_ratelimited("SELinux: failed to load policy\n");
		goto out;
	}

	length = sel_make_policy_nodes(fsi, load_state.policy);
	if (length) {
		pr_warn_ratelimited("SELinux: failed to initialize selinuxfs\n");
		selinux_policy_cancel(&load_state);
		goto out;
	}

	selinux_policy_commit(&load_state);

	length = count;

	audit_log(audit_context(), GFP_KERNEL, AUDIT_MAC_POLICY_LOAD,
		"auid=%u ses=%u lsm=selinux res=1",
		from_kuid(&init_user_ns, audit_get_loginuid(current)),
		audit_get_sessionid(current));
out:
	mutex_unlock(&selinux_state.policy_mutex);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	vfree(data);
	return length;
}

static const struct file_operations sel_load_ops = {
	.write		= sel_write_load,
	.llseek		= generic_file_llseek,
};

static ssize_t sel_write_context(struct file *file, char *buf, size_t size)
{
	char *canon = NULL;
	u32 sid, len;
	ssize_t length;

<<<<<<< HEAD
	length = task_has_security(current, SECURITY__CHECK_CONTEXT);
	if (length)
		goto out;

	length = security_context_to_sid(buf, size, &sid);
=======
	length = avc_has_perm(current_sid(), SECINITSID_SECURITY,
			      SECCLASS_SECURITY, SECURITY__CHECK_CONTEXT, NULL);
	if (length)
		goto out;

	length = security_context_to_sid(buf, size, &sid, GFP_KERNEL);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	if (length)
		goto out;

	length = security_sid_to_context(sid, &canon, &len);
	if (length)
		goto out;

	length = -ERANGE;
	if (len > SIMPLE_TRANSACTION_LIMIT) {
<<<<<<< HEAD
		printk(KERN_ERR "SELinux: %s:  context size (%u) exceeds "
=======
		pr_err("SELinux: %s:  context size (%u) exceeds "
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
			"payload max\n", __func__, len);
		goto out;
	}

	memcpy(buf, canon, len);
	length = len;
out:
	kfree(canon);
	return length;
}

static ssize_t sel_read_checkreqprot(struct file *filp, char __user *buf,
				     size_t count, loff_t *ppos)
{
	char tmpbuf[TMPBUFLEN];
	ssize_t length;

<<<<<<< HEAD
	length = scnprintf(tmpbuf, TMPBUFLEN, "%u", selinux_checkreqprot);
=======
	length = scnprintf(tmpbuf, TMPBUFLEN, "%u",
			   checkreqprot_get());
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	return simple_read_from_buffer(buf, count, ppos, tmpbuf, length);
}

static ssize_t sel_write_checkreqprot(struct file *file, const char __user *buf,
				      size_t count, loff_t *ppos)
{
<<<<<<< HEAD
	char *page = NULL;
	ssize_t length;
	unsigned int new_value;

	length = task_has_security(current, SECURITY__SETCHECKREQPROT);
	if (length)
		goto out;

	length = -ENOMEM;
	if (count >= PAGE_SIZE)
		goto out;

	/* No partial writes. */
	length = -EINVAL;
	if (*ppos != 0)
		goto out;

	length = -ENOMEM;
	page = (char *)get_zeroed_page(GFP_KERNEL);
	if (!page)
		goto out;

	length = -EFAULT;
	if (copy_from_user(page, buf, count))
		goto out;

	length = -EINVAL;
	if (sscanf(page, "%u", &new_value) != 1)
		goto out;

	selinux_checkreqprot = new_value ? 1 : 0;
	length = count;
out:
	free_page((unsigned long) page);
=======
	char *page;
	ssize_t length;
	unsigned int new_value;

	length = avc_has_perm(current_sid(), SECINITSID_SECURITY,
			      SECCLASS_SECURITY, SECURITY__SETCHECKREQPROT,
			      NULL);
	if (length)
		return length;

	if (count >= PAGE_SIZE)
		return -ENOMEM;

	/* No partial writes. */
	if (*ppos != 0)
		return -EINVAL;

	page = memdup_user_nul(buf, count);
	if (IS_ERR(page))
		return PTR_ERR(page);

	if (sscanf(page, "%u", &new_value) != 1) {
		length = -EINVAL;
		goto out;
	}
	length = count;

	if (new_value) {
		char comm[sizeof(current->comm)];

		memcpy(comm, current->comm, sizeof(comm));
		pr_err("SELinux: %s (%d) set checkreqprot to 1. This is no longer supported.\n",
		       comm, current->pid);
	}

	selinux_ima_measure_state();

out:
	kfree(page);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	return length;
}
static const struct file_operations sel_checkreqprot_ops = {
	.read		= sel_read_checkreqprot,
	.write		= sel_write_checkreqprot,
	.llseek		= generic_file_llseek,
};

<<<<<<< HEAD
=======
static ssize_t sel_write_validatetrans(struct file *file,
					const char __user *buf,
					size_t count, loff_t *ppos)
{
	char *oldcon = NULL, *newcon = NULL, *taskcon = NULL;
	char *req = NULL;
	u32 osid, nsid, tsid;
	u16 tclass;
	int rc;

	rc = avc_has_perm(current_sid(), SECINITSID_SECURITY,
			  SECCLASS_SECURITY, SECURITY__VALIDATE_TRANS, NULL);
	if (rc)
		goto out;

	rc = -ENOMEM;
	if (count >= PAGE_SIZE)
		goto out;

	/* No partial writes. */
	rc = -EINVAL;
	if (*ppos != 0)
		goto out;

	req = memdup_user_nul(buf, count);
	if (IS_ERR(req)) {
		rc = PTR_ERR(req);
		req = NULL;
		goto out;
	}

	rc = -ENOMEM;
	oldcon = kzalloc(count + 1, GFP_KERNEL);
	if (!oldcon)
		goto out;

	newcon = kzalloc(count + 1, GFP_KERNEL);
	if (!newcon)
		goto out;

	taskcon = kzalloc(count + 1, GFP_KERNEL);
	if (!taskcon)
		goto out;

	rc = -EINVAL;
	if (sscanf(req, "%s %s %hu %s", oldcon, newcon, &tclass, taskcon) != 4)
		goto out;

	rc = security_context_str_to_sid(oldcon, &osid, GFP_KERNEL);
	if (rc)
		goto out;

	rc = security_context_str_to_sid(newcon, &nsid, GFP_KERNEL);
	if (rc)
		goto out;

	rc = security_context_str_to_sid(taskcon, &tsid, GFP_KERNEL);
	if (rc)
		goto out;

	rc = security_validate_transition_user(osid, nsid, tsid, tclass);
	if (!rc)
		rc = count;
out:
	kfree(req);
	kfree(oldcon);
	kfree(newcon);
	kfree(taskcon);
	return rc;
}

static const struct file_operations sel_transition_ops = {
	.write		= sel_write_validatetrans,
	.llseek		= generic_file_llseek,
};

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
/*
 * Remaining nodes use transaction based IO methods like nfsd/nfsctl.c
 */
static ssize_t sel_write_access(struct file *file, char *buf, size_t size);
static ssize_t sel_write_create(struct file *file, char *buf, size_t size);
static ssize_t sel_write_relabel(struct file *file, char *buf, size_t size);
static ssize_t sel_write_user(struct file *file, char *buf, size_t size);
static ssize_t sel_write_member(struct file *file, char *buf, size_t size);

<<<<<<< HEAD
static ssize_t (*write_op[])(struct file *, char *, size_t) = {
=======
static ssize_t (*const write_op[])(struct file *, char *, size_t) = {
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	[SEL_ACCESS] = sel_write_access,
	[SEL_CREATE] = sel_write_create,
	[SEL_RELABEL] = sel_write_relabel,
	[SEL_USER] = sel_write_user,
	[SEL_MEMBER] = sel_write_member,
	[SEL_CONTEXT] = sel_write_context,
};

static ssize_t selinux_transaction_write(struct file *file, const char __user *buf, size_t size, loff_t *pos)
{
<<<<<<< HEAD
	ino_t ino = file->f_path.dentry->d_inode->i_ino;
=======
	ino_t ino = file_inode(file)->i_ino;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	char *data;
	ssize_t rv;

	if (ino >= ARRAY_SIZE(write_op) || !write_op[ino])
		return -EINVAL;

	data = simple_transaction_get(file, buf, size);
	if (IS_ERR(data))
		return PTR_ERR(data);

	rv = write_op[ino](file, data, size);
	if (rv > 0) {
		simple_transaction_set(file, rv);
		rv = size;
	}
	return rv;
}

static const struct file_operations transaction_ops = {
	.write		= selinux_transaction_write,
	.read		= simple_transaction_read,
	.release	= simple_transaction_release,
	.llseek		= generic_file_llseek,
};

/*
 * payload - write methods
 * If the method has a response, the response should be put in buf,
 * and the length returned.  Otherwise return 0 or and -error.
 */

static ssize_t sel_write_access(struct file *file, char *buf, size_t size)
{
	char *scon = NULL, *tcon = NULL;
	u32 ssid, tsid;
	u16 tclass;
	struct av_decision avd;
	ssize_t length;

<<<<<<< HEAD
	length = task_has_security(current, SECURITY__COMPUTE_AV);
=======
	length = avc_has_perm(current_sid(), SECINITSID_SECURITY,
			      SECCLASS_SECURITY, SECURITY__COMPUTE_AV, NULL);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	if (length)
		goto out;

	length = -ENOMEM;
	scon = kzalloc(size + 1, GFP_KERNEL);
	if (!scon)
		goto out;

	length = -ENOMEM;
	tcon = kzalloc(size + 1, GFP_KERNEL);
	if (!tcon)
		goto out;

	length = -EINVAL;
	if (sscanf(buf, "%s %s %hu", scon, tcon, &tclass) != 3)
		goto out;

<<<<<<< HEAD
	length = security_context_to_sid(scon, strlen(scon) + 1, &ssid);
	if (length)
		goto out;

	length = security_context_to_sid(tcon, strlen(tcon) + 1, &tsid);
=======
	length = security_context_str_to_sid(scon, &ssid, GFP_KERNEL);
	if (length)
		goto out;

	length = security_context_str_to_sid(tcon, &tsid, GFP_KERNEL);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	if (length)
		goto out;

	security_compute_av_user(ssid, tsid, tclass, &avd);

	length = scnprintf(buf, SIMPLE_TRANSACTION_LIMIT,
			  "%x %x %x %x %u %x",
			  avd.allowed, 0xffffffff,
			  avd.auditallow, avd.auditdeny,
			  avd.seqno, avd.flags);
out:
	kfree(tcon);
	kfree(scon);
	return length;
}

static ssize_t sel_write_create(struct file *file, char *buf, size_t size)
{
	char *scon = NULL, *tcon = NULL;
	char *namebuf = NULL, *objname = NULL;
	u32 ssid, tsid, newsid;
	u16 tclass;
	ssize_t length;
	char *newcon = NULL;
	u32 len;
	int nargs;

<<<<<<< HEAD
	length = task_has_security(current, SECURITY__COMPUTE_CREATE);
=======
	length = avc_has_perm(current_sid(), SECINITSID_SECURITY,
			      SECCLASS_SECURITY, SECURITY__COMPUTE_CREATE,
			      NULL);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	if (length)
		goto out;

	length = -ENOMEM;
	scon = kzalloc(size + 1, GFP_KERNEL);
	if (!scon)
		goto out;

	length = -ENOMEM;
	tcon = kzalloc(size + 1, GFP_KERNEL);
	if (!tcon)
		goto out;

	length = -ENOMEM;
	namebuf = kzalloc(size + 1, GFP_KERNEL);
	if (!namebuf)
		goto out;

	length = -EINVAL;
	nargs = sscanf(buf, "%s %s %hu %s", scon, tcon, &tclass, namebuf);
	if (nargs < 3 || nargs > 4)
		goto out;
	if (nargs == 4) {
		/*
		 * If and when the name of new object to be queried contains
		 * either whitespace or multibyte characters, they shall be
		 * encoded based on the percentage-encoding rule.
		 * If not encoded, the sscanf logic picks up only left-half
<<<<<<< HEAD
		 * of the supplied name; splitted by a whitespace unexpectedly.
=======
		 * of the supplied name; split by a whitespace unexpectedly.
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		 */
		char   *r, *w;
		int     c1, c2;

		r = w = namebuf;
		do {
			c1 = *r++;
			if (c1 == '+')
				c1 = ' ';
			else if (c1 == '%') {
				c1 = hex_to_bin(*r++);
				if (c1 < 0)
					goto out;
				c2 = hex_to_bin(*r++);
				if (c2 < 0)
					goto out;
				c1 = (c1 << 4) | c2;
			}
			*w++ = c1;
		} while (c1 != '\0');

		objname = namebuf;
	}

<<<<<<< HEAD
	length = security_context_to_sid(scon, strlen(scon) + 1, &ssid);
	if (length)
		goto out;

	length = security_context_to_sid(tcon, strlen(tcon) + 1, &tsid);
=======
	length = security_context_str_to_sid(scon, &ssid, GFP_KERNEL);
	if (length)
		goto out;

	length = security_context_str_to_sid(tcon, &tsid, GFP_KERNEL);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	if (length)
		goto out;

	length = security_transition_sid_user(ssid, tsid, tclass,
					      objname, &newsid);
	if (length)
		goto out;

	length = security_sid_to_context(newsid, &newcon, &len);
	if (length)
		goto out;

	length = -ERANGE;
	if (len > SIMPLE_TRANSACTION_LIMIT) {
<<<<<<< HEAD
		printk(KERN_ERR "SELinux: %s:  context size (%u) exceeds "
=======
		pr_err("SELinux: %s:  context size (%u) exceeds "
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
			"payload max\n", __func__, len);
		goto out;
	}

	memcpy(buf, newcon, len);
	length = len;
out:
	kfree(newcon);
	kfree(namebuf);
	kfree(tcon);
	kfree(scon);
	return length;
}

static ssize_t sel_write_relabel(struct file *file, char *buf, size_t size)
{
	char *scon = NULL, *tcon = NULL;
	u32 ssid, tsid, newsid;
	u16 tclass;
	ssize_t length;
	char *newcon = NULL;
	u32 len;

<<<<<<< HEAD
	length = task_has_security(current, SECURITY__COMPUTE_RELABEL);
=======
	length = avc_has_perm(current_sid(), SECINITSID_SECURITY,
			      SECCLASS_SECURITY, SECURITY__COMPUTE_RELABEL,
			      NULL);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	if (length)
		goto out;

	length = -ENOMEM;
	scon = kzalloc(size + 1, GFP_KERNEL);
	if (!scon)
		goto out;

	length = -ENOMEM;
	tcon = kzalloc(size + 1, GFP_KERNEL);
	if (!tcon)
		goto out;

	length = -EINVAL;
	if (sscanf(buf, "%s %s %hu", scon, tcon, &tclass) != 3)
		goto out;

<<<<<<< HEAD
	length = security_context_to_sid(scon, strlen(scon) + 1, &ssid);
	if (length)
		goto out;

	length = security_context_to_sid(tcon, strlen(tcon) + 1, &tsid);
=======
	length = security_context_str_to_sid(scon, &ssid, GFP_KERNEL);
	if (length)
		goto out;

	length = security_context_str_to_sid(tcon, &tsid, GFP_KERNEL);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	if (length)
		goto out;

	length = security_change_sid(ssid, tsid, tclass, &newsid);
	if (length)
		goto out;

	length = security_sid_to_context(newsid, &newcon, &len);
	if (length)
		goto out;

	length = -ERANGE;
	if (len > SIMPLE_TRANSACTION_LIMIT)
		goto out;

	memcpy(buf, newcon, len);
	length = len;
out:
	kfree(newcon);
	kfree(tcon);
	kfree(scon);
	return length;
}

static ssize_t sel_write_user(struct file *file, char *buf, size_t size)
{
	char *con = NULL, *user = NULL, *ptr;
	u32 sid, *sids = NULL;
	ssize_t length;
	char *newcon;
<<<<<<< HEAD
	int i, rc;
	u32 len, nsids;

	length = task_has_security(current, SECURITY__COMPUTE_USER);
=======
	int rc;
	u32 i, len, nsids;

	length = avc_has_perm(current_sid(), SECINITSID_SECURITY,
			      SECCLASS_SECURITY, SECURITY__COMPUTE_USER,
			      NULL);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	if (length)
		goto out;

	length = -ENOMEM;
	con = kzalloc(size + 1, GFP_KERNEL);
	if (!con)
		goto out;

	length = -ENOMEM;
	user = kzalloc(size + 1, GFP_KERNEL);
	if (!user)
		goto out;

	length = -EINVAL;
	if (sscanf(buf, "%s %s", con, user) != 2)
		goto out;

<<<<<<< HEAD
	length = security_context_to_sid(con, strlen(con) + 1, &sid);
=======
	length = security_context_str_to_sid(con, &sid, GFP_KERNEL);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	if (length)
		goto out;

	length = security_get_user_sids(sid, user, &sids, &nsids);
	if (length)
		goto out;

	length = sprintf(buf, "%u", nsids) + 1;
	ptr = buf + length;
	for (i = 0; i < nsids; i++) {
		rc = security_sid_to_context(sids[i], &newcon, &len);
		if (rc) {
			length = rc;
			goto out;
		}
		if ((length + len) >= SIMPLE_TRANSACTION_LIMIT) {
			kfree(newcon);
			length = -ERANGE;
			goto out;
		}
		memcpy(ptr, newcon, len);
		kfree(newcon);
		ptr += len;
		length += len;
	}
out:
	kfree(sids);
	kfree(user);
	kfree(con);
	return length;
}

static ssize_t sel_write_member(struct file *file, char *buf, size_t size)
{
	char *scon = NULL, *tcon = NULL;
	u32 ssid, tsid, newsid;
	u16 tclass;
	ssize_t length;
	char *newcon = NULL;
	u32 len;

<<<<<<< HEAD
	length = task_has_security(current, SECURITY__COMPUTE_MEMBER);
=======
	length = avc_has_perm(current_sid(), SECINITSID_SECURITY,
			      SECCLASS_SECURITY, SECURITY__COMPUTE_MEMBER,
			      NULL);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	if (length)
		goto out;

	length = -ENOMEM;
	scon = kzalloc(size + 1, GFP_KERNEL);
	if (!scon)
		goto out;

	length = -ENOMEM;
	tcon = kzalloc(size + 1, GFP_KERNEL);
	if (!tcon)
		goto out;

	length = -EINVAL;
	if (sscanf(buf, "%s %s %hu", scon, tcon, &tclass) != 3)
		goto out;

<<<<<<< HEAD
	length = security_context_to_sid(scon, strlen(scon) + 1, &ssid);
	if (length)
		goto out;

	length = security_context_to_sid(tcon, strlen(tcon) + 1, &tsid);
=======
	length = security_context_str_to_sid(scon, &ssid, GFP_KERNEL);
	if (length)
		goto out;

	length = security_context_str_to_sid(tcon, &tsid, GFP_KERNEL);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	if (length)
		goto out;

	length = security_member_sid(ssid, tsid, tclass, &newsid);
	if (length)
		goto out;

	length = security_sid_to_context(newsid, &newcon, &len);
	if (length)
		goto out;

	length = -ERANGE;
	if (len > SIMPLE_TRANSACTION_LIMIT) {
<<<<<<< HEAD
		printk(KERN_ERR "SELinux: %s:  context size (%u) exceeds "
=======
		pr_err("SELinux: %s:  context size (%u) exceeds "
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
			"payload max\n", __func__, len);
		goto out;
	}

	memcpy(buf, newcon, len);
	length = len;
out:
	kfree(newcon);
	kfree(tcon);
	kfree(scon);
	return length;
}

<<<<<<< HEAD
static struct inode *sel_make_inode(struct super_block *sb, int mode)
=======
static struct inode *sel_make_inode(struct super_block *sb, umode_t mode)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
{
	struct inode *ret = new_inode(sb);

	if (ret) {
		ret->i_mode = mode;
<<<<<<< HEAD
		ret->i_atime = ret->i_mtime = ret->i_ctime = CURRENT_TIME;
=======
		simple_inode_init_ts(ret);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	}
	return ret;
}

static ssize_t sel_read_bool(struct file *filep, char __user *buf,
			     size_t count, loff_t *ppos)
{
<<<<<<< HEAD
=======
	struct selinux_fs_info *fsi = file_inode(filep)->i_sb->s_fs_info;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	char *page = NULL;
	ssize_t length;
	ssize_t ret;
	int cur_enforcing;
<<<<<<< HEAD
	struct inode *inode = filep->f_path.dentry->d_inode;
	unsigned index = inode->i_ino & SEL_INO_MASK;
	const char *name = filep->f_path.dentry->d_name.name;

	mutex_lock(&sel_mutex);

	ret = -EINVAL;
	if (index >= bool_num || strcmp(name, bool_pending_names[index]))
		goto out;
=======
	unsigned index = file_inode(filep)->i_ino & SEL_INO_MASK;
	const char *name = filep->f_path.dentry->d_name.name;

	mutex_lock(&selinux_state.policy_mutex);

	ret = -EINVAL;
	if (index >= fsi->bool_num || strcmp(name,
					     fsi->bool_pending_names[index]))
		goto out_unlock;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	ret = -ENOMEM;
	page = (char *)get_zeroed_page(GFP_KERNEL);
	if (!page)
<<<<<<< HEAD
		goto out;
=======
		goto out_unlock;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	cur_enforcing = security_get_bool_value(index);
	if (cur_enforcing < 0) {
		ret = cur_enforcing;
<<<<<<< HEAD
		goto out;
	}
	length = scnprintf(page, PAGE_SIZE, "%d %d", cur_enforcing,
			  bool_pending_values[index]);
	ret = simple_read_from_buffer(buf, count, ppos, page, length);
out:
	mutex_unlock(&sel_mutex);
	free_page((unsigned long)page);
	return ret;
=======
		goto out_unlock;
	}
	length = scnprintf(page, PAGE_SIZE, "%d %d", cur_enforcing,
			  fsi->bool_pending_values[index]);
	mutex_unlock(&selinux_state.policy_mutex);
	ret = simple_read_from_buffer(buf, count, ppos, page, length);
out_free:
	free_page((unsigned long)page);
	return ret;

out_unlock:
	mutex_unlock(&selinux_state.policy_mutex);
	goto out_free;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

static ssize_t sel_write_bool(struct file *filep, const char __user *buf,
			      size_t count, loff_t *ppos)
{
<<<<<<< HEAD
	char *page = NULL;
	ssize_t length;
	int new_value;
	struct inode *inode = filep->f_path.dentry->d_inode;
	unsigned index = inode->i_ino & SEL_INO_MASK;
	const char *name = filep->f_path.dentry->d_name.name;

	mutex_lock(&sel_mutex);

	length = task_has_security(current, SECURITY__SETBOOL);
=======
	struct selinux_fs_info *fsi = file_inode(filep)->i_sb->s_fs_info;
	char *page = NULL;
	ssize_t length;
	int new_value;
	unsigned index = file_inode(filep)->i_ino & SEL_INO_MASK;
	const char *name = filep->f_path.dentry->d_name.name;

	if (count >= PAGE_SIZE)
		return -ENOMEM;

	/* No partial writes. */
	if (*ppos != 0)
		return -EINVAL;

	page = memdup_user_nul(buf, count);
	if (IS_ERR(page))
		return PTR_ERR(page);

	mutex_lock(&selinux_state.policy_mutex);

	length = avc_has_perm(current_sid(), SECINITSID_SECURITY,
			      SECCLASS_SECURITY, SECURITY__SETBOOL,
			      NULL);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	if (length)
		goto out;

	length = -EINVAL;
<<<<<<< HEAD
	if (index >= bool_num || strcmp(name, bool_pending_names[index]))
		goto out;

	length = -ENOMEM;
	if (count >= PAGE_SIZE)
		goto out;

	/* No partial writes. */
	length = -EINVAL;
	if (*ppos != 0)
		goto out;

	length = -ENOMEM;
	page = (char *)get_zeroed_page(GFP_KERNEL);
	if (!page)
		goto out;

	length = -EFAULT;
	if (copy_from_user(page, buf, count))
=======
	if (index >= fsi->bool_num || strcmp(name,
					     fsi->bool_pending_names[index]))
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		goto out;

	length = -EINVAL;
	if (sscanf(page, "%d", &new_value) != 1)
		goto out;

	if (new_value)
		new_value = 1;

<<<<<<< HEAD
	bool_pending_values[index] = new_value;
	length = count;

out:
	mutex_unlock(&sel_mutex);
	free_page((unsigned long) page);
=======
	fsi->bool_pending_values[index] = new_value;
	length = count;

out:
	mutex_unlock(&selinux_state.policy_mutex);
	kfree(page);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	return length;
}

static const struct file_operations sel_bool_ops = {
	.read		= sel_read_bool,
	.write		= sel_write_bool,
	.llseek		= generic_file_llseek,
};

static ssize_t sel_commit_bools_write(struct file *filep,
				      const char __user *buf,
				      size_t count, loff_t *ppos)
{
<<<<<<< HEAD
=======
	struct selinux_fs_info *fsi = file_inode(filep)->i_sb->s_fs_info;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	char *page = NULL;
	ssize_t length;
	int new_value;

<<<<<<< HEAD
	mutex_lock(&sel_mutex);

	length = task_has_security(current, SECURITY__SETBOOL);
	if (length)
		goto out;

	length = -ENOMEM;
	if (count >= PAGE_SIZE)
		goto out;

	/* No partial writes. */
	length = -EINVAL;
	if (*ppos != 0)
		goto out;

	length = -ENOMEM;
	page = (char *)get_zeroed_page(GFP_KERNEL);
	if (!page)
		goto out;

	length = -EFAULT;
	if (copy_from_user(page, buf, count))
=======
	if (count >= PAGE_SIZE)
		return -ENOMEM;

	/* No partial writes. */
	if (*ppos != 0)
		return -EINVAL;

	page = memdup_user_nul(buf, count);
	if (IS_ERR(page))
		return PTR_ERR(page);

	mutex_lock(&selinux_state.policy_mutex);

	length = avc_has_perm(current_sid(), SECINITSID_SECURITY,
			      SECCLASS_SECURITY, SECURITY__SETBOOL,
			      NULL);
	if (length)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		goto out;

	length = -EINVAL;
	if (sscanf(page, "%d", &new_value) != 1)
		goto out;

	length = 0;
<<<<<<< HEAD
	if (new_value && bool_pending_values)
		length = security_set_bools(bool_num, bool_pending_values);
=======
	if (new_value && fsi->bool_pending_values)
		length = security_set_bools(fsi->bool_num,
					    fsi->bool_pending_values);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	if (!length)
		length = count;

out:
<<<<<<< HEAD
	mutex_unlock(&sel_mutex);
	free_page((unsigned long) page);
=======
	mutex_unlock(&selinux_state.policy_mutex);
	kfree(page);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	return length;
}

static const struct file_operations sel_commit_bools_ops = {
	.write		= sel_commit_bools_write,
	.llseek		= generic_file_llseek,
};

<<<<<<< HEAD
static void sel_remove_entries(struct dentry *de)
{
	struct list_head *node;

	spin_lock(&de->d_lock);
	node = de->d_subdirs.next;
	while (node != &de->d_subdirs) {
		struct dentry *d = list_entry(node, struct dentry, d_child);

		spin_lock_nested(&d->d_lock, DENTRY_D_LOCK_NESTED);
		list_del_init(node);

		if (d->d_inode) {
			dget_dlock(d);
			spin_unlock(&de->d_lock);
			spin_unlock(&d->d_lock);
			d_delete(d);
			simple_unlink(de->d_inode, d);
			dput(d);
			spin_lock(&de->d_lock);
		} else
			spin_unlock(&d->d_lock);
		node = de->d_subdirs.next;
	}

	spin_unlock(&de->d_lock);
}

#define BOOL_DIR_NAME "booleans"

static int sel_make_bools(void)
{
	int i, ret;
	ssize_t len;
	struct dentry *dentry = NULL;
	struct dentry *dir = bool_dir;
	struct inode *inode = NULL;
	struct inode_security_struct *isec;
	char **names = NULL, *page;
	int num;
	int *values = NULL;
	u32 sid;

	/* remove any existing files */
	for (i = 0; i < bool_num; i++)
		kfree(bool_pending_names[i]);
	kfree(bool_pending_names);
	kfree(bool_pending_values);
	bool_num = 0;
	bool_pending_names = NULL;
	bool_pending_values = NULL;

	sel_remove_entries(dir);

	ret = -ENOMEM;
	page = (char *)get_zeroed_page(GFP_KERNEL);
	if (!page)
		goto out;

	ret = security_get_bools(&num, &names, &values);
	if (ret)
		goto out;

	for (i = 0; i < num; i++) {
		ret = -ENOMEM;
		dentry = d_alloc_name(dir, names[i]);
		if (!dentry)
			goto out;

		ret = -ENOMEM;
		inode = sel_make_inode(dir->d_sb, S_IFREG | S_IRUGO | S_IWUSR);
		if (!inode)
			goto out;

		ret = -EINVAL;
		len = snprintf(page, PAGE_SIZE, "/%s/%s", BOOL_DIR_NAME, names[i]);
		if (len < 0)
			goto out;

		ret = -ENAMETOOLONG;
		if (len >= PAGE_SIZE)
			goto out;

		isec = (struct inode_security_struct *)inode->i_security;
		ret = security_genfs_sid("selinuxfs", page, SECCLASS_FILE, &sid);
		if (ret)
			goto out;

		isec->sid = sid;
		isec->initialized = 1;
=======
static int sel_make_bools(struct selinux_policy *newpolicy, struct dentry *bool_dir,
			  unsigned int *bool_num, char ***bool_pending_names,
			  int **bool_pending_values)
{
	int ret;
	char **names, *page;
	u32 i, num;

	page = (char *)get_zeroed_page(GFP_KERNEL);
	if (!page)
		return -ENOMEM;

	ret = security_get_bools(newpolicy, &num, &names, bool_pending_values);
	if (ret)
		goto out;

	*bool_num = num;
	*bool_pending_names = names;

	for (i = 0; i < num; i++) {
		struct dentry *dentry;
		struct inode *inode;
		struct inode_security_struct *isec;
		ssize_t len;
		u32 sid;

		len = snprintf(page, PAGE_SIZE, "/%s/%s", BOOL_DIR_NAME, names[i]);
		if (len >= PAGE_SIZE) {
			ret = -ENAMETOOLONG;
			break;
		}
		dentry = d_alloc_name(bool_dir, names[i]);
		if (!dentry) {
			ret = -ENOMEM;
			break;
		}

		inode = sel_make_inode(bool_dir->d_sb, S_IFREG | S_IRUGO | S_IWUSR);
		if (!inode) {
			dput(dentry);
			ret = -ENOMEM;
			break;
		}

		isec = selinux_inode(inode);
		ret = selinux_policy_genfs_sid(newpolicy, "selinuxfs", page,
					 SECCLASS_FILE, &sid);
		if (ret) {
			pr_warn_ratelimited("SELinux: no sid found, defaulting to security isid for %s\n",
					   page);
			sid = SECINITSID_SECURITY;
		}

		isec->sid = sid;
		isec->initialized = LABEL_INITIALIZED;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		inode->i_fop = &sel_bool_ops;
		inode->i_ino = i|SEL_BOOL_INO_OFFSET;
		d_add(dentry, inode);
	}
<<<<<<< HEAD
	bool_num = num;
	bool_pending_names = names;
	bool_pending_values = values;

	free_page((unsigned long)page);
	return 0;
out:
	free_page((unsigned long)page);

	if (names) {
		for (i = 0; i < num; i++)
			kfree(names[i]);
		kfree(names);
	}
	kfree(values);
	sel_remove_entries(dir);

	return ret;
}

#define NULL_FILE_NAME "null"

struct dentry *selinux_null;

=======
out:
	free_page((unsigned long)page);
	return ret;
}

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
static ssize_t sel_read_avc_cache_threshold(struct file *filp, char __user *buf,
					    size_t count, loff_t *ppos)
{
	char tmpbuf[TMPBUFLEN];
	ssize_t length;

<<<<<<< HEAD
	length = scnprintf(tmpbuf, TMPBUFLEN, "%u", avc_cache_threshold);
=======
	length = scnprintf(tmpbuf, TMPBUFLEN, "%u",
			   avc_get_cache_threshold());
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	return simple_read_from_buffer(buf, count, ppos, tmpbuf, length);
}

static ssize_t sel_write_avc_cache_threshold(struct file *file,
					     const char __user *buf,
					     size_t count, loff_t *ppos)

{
<<<<<<< HEAD
	char *page = NULL;
	ssize_t ret;
	int new_value;

	ret = task_has_security(current, SECURITY__SETSECPARAM);
	if (ret)
		goto out;

	ret = -ENOMEM;
	if (count >= PAGE_SIZE)
		goto out;

	/* No partial writes. */
	ret = -EINVAL;
	if (*ppos != 0)
		goto out;

	ret = -ENOMEM;
	page = (char *)get_zeroed_page(GFP_KERNEL);
	if (!page)
		goto out;

	ret = -EFAULT;
	if (copy_from_user(page, buf, count))
		goto out;
=======
	char *page;
	ssize_t ret;
	unsigned int new_value;

	ret = avc_has_perm(current_sid(), SECINITSID_SECURITY,
			   SECCLASS_SECURITY, SECURITY__SETSECPARAM,
			   NULL);
	if (ret)
		return ret;

	if (count >= PAGE_SIZE)
		return -ENOMEM;

	/* No partial writes. */
	if (*ppos != 0)
		return -EINVAL;

	page = memdup_user_nul(buf, count);
	if (IS_ERR(page))
		return PTR_ERR(page);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	ret = -EINVAL;
	if (sscanf(page, "%u", &new_value) != 1)
		goto out;

<<<<<<< HEAD
	avc_cache_threshold = new_value;

	ret = count;
out:
	free_page((unsigned long)page);
=======
	avc_set_cache_threshold(new_value);

	ret = count;
out:
	kfree(page);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	return ret;
}

static ssize_t sel_read_avc_hash_stats(struct file *filp, char __user *buf,
				       size_t count, loff_t *ppos)
{
	char *page;
	ssize_t length;

	page = (char *)__get_free_page(GFP_KERNEL);
	if (!page)
		return -ENOMEM;

	length = avc_get_hash_stats(page);
	if (length >= 0)
		length = simple_read_from_buffer(buf, count, ppos, page, length);
	free_page((unsigned long)page);

	return length;
}

<<<<<<< HEAD
=======
static ssize_t sel_read_sidtab_hash_stats(struct file *filp, char __user *buf,
					size_t count, loff_t *ppos)
{
	char *page;
	ssize_t length;

	page = (char *)__get_free_page(GFP_KERNEL);
	if (!page)
		return -ENOMEM;

	length = security_sidtab_hash_stats(page);
	if (length >= 0)
		length = simple_read_from_buffer(buf, count, ppos, page,
						length);
	free_page((unsigned long)page);

	return length;
}

static const struct file_operations sel_sidtab_hash_stats_ops = {
	.read		= sel_read_sidtab_hash_stats,
	.llseek		= generic_file_llseek,
};

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
static const struct file_operations sel_avc_cache_threshold_ops = {
	.read		= sel_read_avc_cache_threshold,
	.write		= sel_write_avc_cache_threshold,
	.llseek		= generic_file_llseek,
};

static const struct file_operations sel_avc_hash_stats_ops = {
	.read		= sel_read_avc_hash_stats,
	.llseek		= generic_file_llseek,
};

#ifdef CONFIG_SECURITY_SELINUX_AVC_STATS
static struct avc_cache_stats *sel_avc_get_stat_idx(loff_t *idx)
{
	int cpu;

	for (cpu = *idx; cpu < nr_cpu_ids; ++cpu) {
		if (!cpu_possible(cpu))
			continue;
		*idx = cpu + 1;
		return &per_cpu(avc_cache_stats, cpu);
	}
<<<<<<< HEAD
=======
	(*idx)++;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	return NULL;
}

static void *sel_avc_stats_seq_start(struct seq_file *seq, loff_t *pos)
{
	loff_t n = *pos - 1;

	if (*pos == 0)
		return SEQ_START_TOKEN;

	return sel_avc_get_stat_idx(&n);
}

static void *sel_avc_stats_seq_next(struct seq_file *seq, void *v, loff_t *pos)
{
	return sel_avc_get_stat_idx(pos);
}

static int sel_avc_stats_seq_show(struct seq_file *seq, void *v)
{
	struct avc_cache_stats *st = v;

<<<<<<< HEAD
	if (v == SEQ_START_TOKEN)
		seq_printf(seq, "lookups hits misses allocations reclaims "
			   "frees\n");
	else {
=======
	if (v == SEQ_START_TOKEN) {
		seq_puts(seq,
			 "lookups hits misses allocations reclaims frees\n");
	} else {
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		unsigned int lookups = st->lookups;
		unsigned int misses = st->misses;
		unsigned int hits = lookups - misses;
		seq_printf(seq, "%u %u %u %u %u %u\n", lookups,
			   hits, misses, st->allocations,
			   st->reclaims, st->frees);
	}
	return 0;
}

static void sel_avc_stats_seq_stop(struct seq_file *seq, void *v)
{ }

static const struct seq_operations sel_avc_cache_stats_seq_ops = {
	.start		= sel_avc_stats_seq_start,
	.next		= sel_avc_stats_seq_next,
	.show		= sel_avc_stats_seq_show,
	.stop		= sel_avc_stats_seq_stop,
};

static int sel_open_avc_cache_stats(struct inode *inode, struct file *file)
{
	return seq_open(file, &sel_avc_cache_stats_seq_ops);
}

static const struct file_operations sel_avc_cache_stats_ops = {
	.open		= sel_open_avc_cache_stats,
	.read		= seq_read,
	.llseek		= seq_lseek,
	.release	= seq_release,
};
#endif

static int sel_make_avc_files(struct dentry *dir)
{
<<<<<<< HEAD
	int i;
	static struct tree_descr files[] = {
=======
	struct super_block *sb = dir->d_sb;
	struct selinux_fs_info *fsi = sb->s_fs_info;
	unsigned int i;
	static const struct tree_descr files[] = {
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		{ "cache_threshold",
		  &sel_avc_cache_threshold_ops, S_IRUGO|S_IWUSR },
		{ "hash_stats", &sel_avc_hash_stats_ops, S_IRUGO },
#ifdef CONFIG_SECURITY_SELINUX_AVC_STATS
		{ "cache_stats", &sel_avc_cache_stats_ops, S_IRUGO },
#endif
	};

	for (i = 0; i < ARRAY_SIZE(files); i++) {
		struct inode *inode;
		struct dentry *dentry;

		dentry = d_alloc_name(dir, files[i].name);
		if (!dentry)
			return -ENOMEM;

		inode = sel_make_inode(dir->d_sb, S_IFREG|files[i].mode);
<<<<<<< HEAD
		if (!inode)
			return -ENOMEM;

		inode->i_fop = files[i].ops;
		inode->i_ino = ++sel_last_ino;
=======
		if (!inode) {
			dput(dentry);
			return -ENOMEM;
		}

		inode->i_fop = files[i].ops;
		inode->i_ino = ++fsi->last_ino;
		d_add(dentry, inode);
	}

	return 0;
}

static int sel_make_ss_files(struct dentry *dir)
{
	struct super_block *sb = dir->d_sb;
	struct selinux_fs_info *fsi = sb->s_fs_info;
	unsigned int i;
	static const struct tree_descr files[] = {
		{ "sidtab_hash_stats", &sel_sidtab_hash_stats_ops, S_IRUGO },
	};

	for (i = 0; i < ARRAY_SIZE(files); i++) {
		struct inode *inode;
		struct dentry *dentry;

		dentry = d_alloc_name(dir, files[i].name);
		if (!dentry)
			return -ENOMEM;

		inode = sel_make_inode(dir->d_sb, S_IFREG|files[i].mode);
		if (!inode) {
			dput(dentry);
			return -ENOMEM;
		}

		inode->i_fop = files[i].ops;
		inode->i_ino = ++fsi->last_ino;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		d_add(dentry, inode);
	}

	return 0;
}

static ssize_t sel_read_initcon(struct file *file, char __user *buf,
				size_t count, loff_t *ppos)
{
<<<<<<< HEAD
	struct inode *inode;
=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	char *con;
	u32 sid, len;
	ssize_t ret;

<<<<<<< HEAD
	inode = file->f_path.dentry->d_inode;
	sid = inode->i_ino&SEL_INO_MASK;
=======
	sid = file_inode(file)->i_ino&SEL_INO_MASK;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	ret = security_sid_to_context(sid, &con, &len);
	if (ret)
		return ret;

	ret = simple_read_from_buffer(buf, count, ppos, con, len);
	kfree(con);
	return ret;
}

static const struct file_operations sel_initcon_ops = {
	.read		= sel_read_initcon,
	.llseek		= generic_file_llseek,
};

static int sel_make_initcon_files(struct dentry *dir)
{
<<<<<<< HEAD
	int i;
=======
	unsigned int i;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	for (i = 1; i <= SECINITSID_NUM; i++) {
		struct inode *inode;
		struct dentry *dentry;
<<<<<<< HEAD
		dentry = d_alloc_name(dir, security_get_initial_sid_context(i));
=======
		const char *s = security_get_initial_sid_context(i);

		if (!s)
			continue;
		dentry = d_alloc_name(dir, s);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		if (!dentry)
			return -ENOMEM;

		inode = sel_make_inode(dir->d_sb, S_IFREG|S_IRUGO);
<<<<<<< HEAD
		if (!inode)
			return -ENOMEM;
=======
		if (!inode) {
			dput(dentry);
			return -ENOMEM;
		}
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

		inode->i_fop = &sel_initcon_ops;
		inode->i_ino = i|SEL_INITCON_INO_OFFSET;
		d_add(dentry, inode);
	}

	return 0;
}

<<<<<<< HEAD
static inline unsigned int sel_div(unsigned long a, unsigned long b)
{
	return a / b - (a % b < 0);
}

=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
static inline unsigned long sel_class_to_ino(u16 class)
{
	return (class * (SEL_VEC_MAX + 1)) | SEL_CLASS_INO_OFFSET;
}

static inline u16 sel_ino_to_class(unsigned long ino)
{
<<<<<<< HEAD
	return sel_div(ino & SEL_INO_MASK, SEL_VEC_MAX + 1);
=======
	return (ino & SEL_INO_MASK) / (SEL_VEC_MAX + 1);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

static inline unsigned long sel_perm_to_ino(u16 class, u32 perm)
{
	return (class * (SEL_VEC_MAX + 1) + perm) | SEL_CLASS_INO_OFFSET;
}

static inline u32 sel_ino_to_perm(unsigned long ino)
{
	return (ino & SEL_INO_MASK) % (SEL_VEC_MAX + 1);
}

static ssize_t sel_read_class(struct file *file, char __user *buf,
				size_t count, loff_t *ppos)
{
<<<<<<< HEAD
	ssize_t rc, len;
	char *page;
	unsigned long ino = file->f_path.dentry->d_inode->i_ino;

	page = (char *)__get_free_page(GFP_KERNEL);
	if (!page)
		return -ENOMEM;

	len = snprintf(page, PAGE_SIZE, "%d", sel_ino_to_class(ino));
	rc = simple_read_from_buffer(buf, count, ppos, page, len);
	free_page((unsigned long)page);

	return rc;
=======
	unsigned long ino = file_inode(file)->i_ino;
	char res[TMPBUFLEN];
	ssize_t len = scnprintf(res, sizeof(res), "%d", sel_ino_to_class(ino));
	return simple_read_from_buffer(buf, count, ppos, res, len);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

static const struct file_operations sel_class_ops = {
	.read		= sel_read_class,
	.llseek		= generic_file_llseek,
};

static ssize_t sel_read_perm(struct file *file, char __user *buf,
				size_t count, loff_t *ppos)
{
<<<<<<< HEAD
	ssize_t rc, len;
	char *page;
	unsigned long ino = file->f_path.dentry->d_inode->i_ino;

	page = (char *)__get_free_page(GFP_KERNEL);
	if (!page)
		return -ENOMEM;

	len = snprintf(page, PAGE_SIZE, "%d", sel_ino_to_perm(ino));
	rc = simple_read_from_buffer(buf, count, ppos, page, len);
	free_page((unsigned long)page);

	return rc;
=======
	unsigned long ino = file_inode(file)->i_ino;
	char res[TMPBUFLEN];
	ssize_t len = scnprintf(res, sizeof(res), "%d", sel_ino_to_perm(ino));
	return simple_read_from_buffer(buf, count, ppos, res, len);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

static const struct file_operations sel_perm_ops = {
	.read		= sel_read_perm,
	.llseek		= generic_file_llseek,
};

static ssize_t sel_read_policycap(struct file *file, char __user *buf,
				  size_t count, loff_t *ppos)
{
	int value;
	char tmpbuf[TMPBUFLEN];
	ssize_t length;
<<<<<<< HEAD
	unsigned long i_ino = file->f_path.dentry->d_inode->i_ino;
=======
	unsigned long i_ino = file_inode(file)->i_ino;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	value = security_policycap_supported(i_ino & SEL_INO_MASK);
	length = scnprintf(tmpbuf, TMPBUFLEN, "%d", value);

	return simple_read_from_buffer(buf, count, ppos, tmpbuf, length);
}

static const struct file_operations sel_policycap_ops = {
	.read		= sel_read_policycap,
	.llseek		= generic_file_llseek,
};

<<<<<<< HEAD
static int sel_make_perm_files(char *objclass, int classvalue,
				struct dentry *dir)
{
	int i, rc, nperms;
	char **perms;

	rc = security_get_permissions(objclass, &perms, &nperms);
=======
static int sel_make_perm_files(struct selinux_policy *newpolicy,
			char *objclass, int classvalue,
			struct dentry *dir)
{
	u32 i, nperms;
	int rc;
	char **perms;

	rc = security_get_permissions(newpolicy, objclass, &perms, &nperms);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	if (rc)
		return rc;

	for (i = 0; i < nperms; i++) {
		struct inode *inode;
		struct dentry *dentry;

		rc = -ENOMEM;
		dentry = d_alloc_name(dir, perms[i]);
		if (!dentry)
			goto out;

		rc = -ENOMEM;
		inode = sel_make_inode(dir->d_sb, S_IFREG|S_IRUGO);
<<<<<<< HEAD
		if (!inode)
			goto out;
=======
		if (!inode) {
			dput(dentry);
			goto out;
		}
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

		inode->i_fop = &sel_perm_ops;
		/* i+1 since perm values are 1-indexed */
		inode->i_ino = sel_perm_to_ino(classvalue, i + 1);
		d_add(dentry, inode);
	}
	rc = 0;
out:
	for (i = 0; i < nperms; i++)
		kfree(perms[i]);
	kfree(perms);
	return rc;
}

<<<<<<< HEAD
static int sel_make_class_dir_entries(char *classname, int index,
					struct dentry *dir)
{
	struct dentry *dentry = NULL;
	struct inode *inode = NULL;
	int rc;
=======
static int sel_make_class_dir_entries(struct selinux_policy *newpolicy,
				char *classname, int index,
				struct dentry *dir)
{
	struct super_block *sb = dir->d_sb;
	struct selinux_fs_info *fsi = sb->s_fs_info;
	struct dentry *dentry = NULL;
	struct inode *inode = NULL;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	dentry = d_alloc_name(dir, "index");
	if (!dentry)
		return -ENOMEM;

	inode = sel_make_inode(dir->d_sb, S_IFREG|S_IRUGO);
<<<<<<< HEAD
	if (!inode)
		return -ENOMEM;
=======
	if (!inode) {
		dput(dentry);
		return -ENOMEM;
	}
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	inode->i_fop = &sel_class_ops;
	inode->i_ino = sel_class_to_ino(index);
	d_add(dentry, inode);

<<<<<<< HEAD
	dentry = sel_make_dir(dir, "perms", &last_class_ino);
	if (IS_ERR(dentry))
		return PTR_ERR(dentry);

	rc = sel_make_perm_files(classname, index, dentry);

	return rc;
}

static void sel_remove_classes(void)
{
	struct list_head *class_node;

	list_for_each(class_node, &class_dir->d_subdirs) {
		struct dentry *class_subdir = list_entry(class_node,
					struct dentry, d_child);
		struct list_head *class_subdir_node;

		list_for_each(class_subdir_node, &class_subdir->d_subdirs) {
			struct dentry *d = list_entry(class_subdir_node,
						struct dentry, d_child);

			if (d->d_inode)
				if (d->d_inode->i_mode & S_IFDIR)
					sel_remove_entries(d);
		}

		sel_remove_entries(class_subdir);
	}

	sel_remove_entries(class_dir);
}

static int sel_make_classes(void)
{
	int rc, nclasses, i;
	char **classes;

	/* delete any existing entries */
	sel_remove_classes();

	rc = security_get_classes(&classes, &nclasses);
=======
	dentry = sel_make_dir(dir, "perms", &fsi->last_class_ino);
	if (IS_ERR(dentry))
		return PTR_ERR(dentry);

	return sel_make_perm_files(newpolicy, classname, index, dentry);
}

static int sel_make_classes(struct selinux_policy *newpolicy,
			    struct dentry *class_dir,
			    unsigned long *last_class_ino)
{
	u32 i, nclasses;
	int rc;
	char **classes;

	rc = security_get_classes(newpolicy, &classes, &nclasses);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	if (rc)
		return rc;

	/* +2 since classes are 1-indexed */
<<<<<<< HEAD
	last_class_ino = sel_class_to_ino(nclasses + 2);
=======
	*last_class_ino = sel_class_to_ino(nclasses + 2);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	for (i = 0; i < nclasses; i++) {
		struct dentry *class_name_dir;

		class_name_dir = sel_make_dir(class_dir, classes[i],
<<<<<<< HEAD
				&last_class_ino);
=======
					      last_class_ino);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		if (IS_ERR(class_name_dir)) {
			rc = PTR_ERR(class_name_dir);
			goto out;
		}

		/* i+1 since class values are 1-indexed */
<<<<<<< HEAD
		rc = sel_make_class_dir_entries(classes[i], i + 1,
=======
		rc = sel_make_class_dir_entries(newpolicy, classes[i], i + 1,
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
				class_name_dir);
		if (rc)
			goto out;
	}
	rc = 0;
out:
	for (i = 0; i < nclasses; i++)
		kfree(classes[i]);
	kfree(classes);
	return rc;
}

<<<<<<< HEAD
static int sel_make_policycap(void)
=======
static int sel_make_policycap(struct selinux_fs_info *fsi)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
{
	unsigned int iter;
	struct dentry *dentry = NULL;
	struct inode *inode = NULL;

<<<<<<< HEAD
	sel_remove_entries(policycap_dir);

	for (iter = 0; iter <= POLICYDB_CAPABILITY_MAX; iter++) {
		if (iter < ARRAY_SIZE(policycap_names))
			dentry = d_alloc_name(policycap_dir,
					      policycap_names[iter]);
		else
			dentry = d_alloc_name(policycap_dir, "unknown");
=======
	for (iter = 0; iter <= POLICYDB_CAP_MAX; iter++) {
		if (iter < ARRAY_SIZE(selinux_policycap_names))
			dentry = d_alloc_name(fsi->policycap_dir,
					      selinux_policycap_names[iter]);
		else
			dentry = d_alloc_name(fsi->policycap_dir, "unknown");
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

		if (dentry == NULL)
			return -ENOMEM;

<<<<<<< HEAD
		inode = sel_make_inode(policycap_dir->d_sb, S_IFREG | S_IRUGO);
		if (inode == NULL)
			return -ENOMEM;
=======
		inode = sel_make_inode(fsi->sb, S_IFREG | 0444);
		if (inode == NULL) {
			dput(dentry);
			return -ENOMEM;
		}
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

		inode->i_fop = &sel_policycap_ops;
		inode->i_ino = iter | SEL_POLICYCAP_INO_OFFSET;
		d_add(dentry, inode);
	}

	return 0;
}

static struct dentry *sel_make_dir(struct dentry *dir, const char *name,
			unsigned long *ino)
{
	struct dentry *dentry = d_alloc_name(dir, name);
	struct inode *inode;

	if (!dentry)
		return ERR_PTR(-ENOMEM);

	inode = sel_make_inode(dir->d_sb, S_IFDIR | S_IRUGO | S_IXUGO);
	if (!inode) {
		dput(dentry);
		return ERR_PTR(-ENOMEM);
	}

	inode->i_op = &simple_dir_inode_operations;
	inode->i_fop = &simple_dir_operations;
	inode->i_ino = ++(*ino);
	/* directory inodes start off with i_nlink == 2 (for "." entry) */
	inc_nlink(inode);
	d_add(dentry, inode);
	/* bump link count on parent directory, too */
<<<<<<< HEAD
	inc_nlink(dir->d_inode);
=======
	inc_nlink(d_inode(dir));
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	return dentry;
}

<<<<<<< HEAD
static int sel_fill_super(struct super_block *sb, void *data, int silent)
{
=======
static int reject_all(struct mnt_idmap *idmap, struct inode *inode, int mask)
{
	return -EPERM;	// no access for anyone, root or no root.
}

static const struct inode_operations swapover_dir_inode_operations = {
	.lookup		= simple_lookup,
	.permission	= reject_all,
};

static struct dentry *sel_make_swapover_dir(struct super_block *sb,
						unsigned long *ino)
{
	struct dentry *dentry = d_alloc_name(sb->s_root, ".swapover");
	struct inode *inode;

	if (!dentry)
		return ERR_PTR(-ENOMEM);

	inode = sel_make_inode(sb, S_IFDIR);
	if (!inode) {
		dput(dentry);
		return ERR_PTR(-ENOMEM);
	}

	inode->i_op = &swapover_dir_inode_operations;
	inode->i_ino = ++(*ino);
	/* directory inodes start off with i_nlink == 2 (for "." entry) */
	inc_nlink(inode);
	inode_lock(sb->s_root->d_inode);
	d_add(dentry, inode);
	inc_nlink(sb->s_root->d_inode);
	inode_unlock(sb->s_root->d_inode);
	return dentry;
}

#define NULL_FILE_NAME "null"

static int sel_fill_super(struct super_block *sb, struct fs_context *fc)
{
	struct selinux_fs_info *fsi;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	int ret;
	struct dentry *dentry;
	struct inode *inode;
	struct inode_security_struct *isec;

<<<<<<< HEAD
	static struct tree_descr selinux_files[] = {
=======
	static const struct tree_descr selinux_files[] = {
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		[SEL_LOAD] = {"load", &sel_load_ops, S_IRUSR|S_IWUSR},
		[SEL_ENFORCE] = {"enforce", &sel_enforce_ops, S_IRUGO|S_IWUSR},
		[SEL_CONTEXT] = {"context", &transaction_ops, S_IRUGO|S_IWUGO},
		[SEL_ACCESS] = {"access", &transaction_ops, S_IRUGO|S_IWUGO},
		[SEL_CREATE] = {"create", &transaction_ops, S_IRUGO|S_IWUGO},
		[SEL_RELABEL] = {"relabel", &transaction_ops, S_IRUGO|S_IWUGO},
		[SEL_USER] = {"user", &transaction_ops, S_IRUGO|S_IWUGO},
		[SEL_POLICYVERS] = {"policyvers", &sel_policyvers_ops, S_IRUGO},
		[SEL_COMMIT_BOOLS] = {"commit_pending_bools", &sel_commit_bools_ops, S_IWUSR},
		[SEL_MLS] = {"mls", &sel_mls_ops, S_IRUGO},
		[SEL_DISABLE] = {"disable", &sel_disable_ops, S_IWUSR},
		[SEL_MEMBER] = {"member", &transaction_ops, S_IRUGO|S_IWUGO},
		[SEL_CHECKREQPROT] = {"checkreqprot", &sel_checkreqprot_ops, S_IRUGO|S_IWUSR},
		[SEL_REJECT_UNKNOWN] = {"reject_unknown", &sel_handle_unknown_ops, S_IRUGO},
		[SEL_DENY_UNKNOWN] = {"deny_unknown", &sel_handle_unknown_ops, S_IRUGO},
		[SEL_STATUS] = {"status", &sel_handle_status_ops, S_IRUGO},
<<<<<<< HEAD
		[SEL_POLICY] = {"policy", &sel_policy_ops, S_IRUSR},
		/* last one */ {""}
	};
=======
		[SEL_POLICY] = {"policy", &sel_policy_ops, S_IRUGO},
		[SEL_VALIDATE_TRANS] = {"validatetrans", &sel_transition_ops,
					S_IWUGO},
		/* last one */ {""}
	};

	ret = selinux_fs_info_create(sb);
	if (ret)
		goto err;

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	ret = simple_fill_super(sb, SELINUX_MAGIC, selinux_files);
	if (ret)
		goto err;

<<<<<<< HEAD
	bool_dir = sel_make_dir(sb->s_root, BOOL_DIR_NAME, &sel_last_ino);
	if (IS_ERR(bool_dir)) {
		ret = PTR_ERR(bool_dir);
		bool_dir = NULL;
=======
	fsi = sb->s_fs_info;
	fsi->bool_dir = sel_make_dir(sb->s_root, BOOL_DIR_NAME, &fsi->last_ino);
	if (IS_ERR(fsi->bool_dir)) {
		ret = PTR_ERR(fsi->bool_dir);
		fsi->bool_dir = NULL;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		goto err;
	}

	ret = -ENOMEM;
	dentry = d_alloc_name(sb->s_root, NULL_FILE_NAME);
	if (!dentry)
		goto err;

	ret = -ENOMEM;
	inode = sel_make_inode(sb, S_IFCHR | S_IRUGO | S_IWUGO);
<<<<<<< HEAD
	if (!inode)
		goto err;

	inode->i_ino = ++sel_last_ino;
	isec = (struct inode_security_struct *)inode->i_security;
	isec->sid = SECINITSID_DEVNULL;
	isec->sclass = SECCLASS_CHR_FILE;
	isec->initialized = 1;

	init_special_inode(inode, S_IFCHR | S_IRUGO | S_IWUGO, MKDEV(MEM_MAJOR, 3));
	d_add(dentry, inode);
	selinux_null = dentry;

	dentry = sel_make_dir(sb->s_root, "avc", &sel_last_ino);
=======
	if (!inode) {
		dput(dentry);
		goto err;
	}

	inode->i_ino = ++fsi->last_ino;
	isec = selinux_inode(inode);
	isec->sid = SECINITSID_DEVNULL;
	isec->sclass = SECCLASS_CHR_FILE;
	isec->initialized = LABEL_INITIALIZED;

	init_special_inode(inode, S_IFCHR | S_IRUGO | S_IWUGO, MKDEV(MEM_MAJOR, 3));
	d_add(dentry, inode);

	dentry = sel_make_dir(sb->s_root, "avc", &fsi->last_ino);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	if (IS_ERR(dentry)) {
		ret = PTR_ERR(dentry);
		goto err;
	}

	ret = sel_make_avc_files(dentry);
	if (ret)
		goto err;

<<<<<<< HEAD
	dentry = sel_make_dir(sb->s_root, "initial_contexts", &sel_last_ino);
=======
	dentry = sel_make_dir(sb->s_root, "ss", &fsi->last_ino);
	if (IS_ERR(dentry)) {
		ret = PTR_ERR(dentry);
		goto err;
	}

	ret = sel_make_ss_files(dentry);
	if (ret)
		goto err;

	dentry = sel_make_dir(sb->s_root, "initial_contexts", &fsi->last_ino);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	if (IS_ERR(dentry)) {
		ret = PTR_ERR(dentry);
		goto err;
	}

	ret = sel_make_initcon_files(dentry);
	if (ret)
		goto err;

<<<<<<< HEAD
	class_dir = sel_make_dir(sb->s_root, "class", &sel_last_ino);
	if (IS_ERR(class_dir)) {
		ret = PTR_ERR(class_dir);
		class_dir = NULL;
		goto err;
	}

	policycap_dir = sel_make_dir(sb->s_root, "policy_capabilities", &sel_last_ino);
	if (IS_ERR(policycap_dir)) {
		ret = PTR_ERR(policycap_dir);
		policycap_dir = NULL;
		goto err;
	}
	return 0;
err:
	printk(KERN_ERR "SELinux: %s:  failed while creating inodes\n",
		__func__);
	return ret;
}

static struct dentry *sel_mount(struct file_system_type *fs_type,
		      int flags, const char *dev_name, void *data)
{
	return mount_single(fs_type, flags, data, sel_fill_super);
=======
	fsi->class_dir = sel_make_dir(sb->s_root, CLASS_DIR_NAME, &fsi->last_ino);
	if (IS_ERR(fsi->class_dir)) {
		ret = PTR_ERR(fsi->class_dir);
		fsi->class_dir = NULL;
		goto err;
	}

	fsi->policycap_dir = sel_make_dir(sb->s_root, POLICYCAP_DIR_NAME,
					  &fsi->last_ino);
	if (IS_ERR(fsi->policycap_dir)) {
		ret = PTR_ERR(fsi->policycap_dir);
		fsi->policycap_dir = NULL;
		goto err;
	}

	ret = sel_make_policycap(fsi);
	if (ret) {
		pr_err("SELinux: failed to load policy capabilities\n");
		goto err;
	}

	return 0;
err:
	pr_err("SELinux: %s:  failed while creating inodes\n",
		__func__);

	selinux_fs_info_free(sb);

	return ret;
}

static int sel_get_tree(struct fs_context *fc)
{
	return get_tree_single(fc, sel_fill_super);
}

static const struct fs_context_operations sel_context_ops = {
	.get_tree	= sel_get_tree,
};

static int sel_init_fs_context(struct fs_context *fc)
{
	fc->ops = &sel_context_ops;
	return 0;
}

static void sel_kill_sb(struct super_block *sb)
{
	selinux_fs_info_free(sb);
	kill_litter_super(sb);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

static struct file_system_type sel_fs_type = {
	.name		= "selinuxfs",
<<<<<<< HEAD
	.mount		= sel_mount,
	.kill_sb	= kill_litter_super,
};

struct vfsmount *selinuxfs_mount;
static struct kobject *selinuxfs_kobj;

static int __init init_sel_fs(void)
{
	int err;

	if (!selinux_enabled)
		return 0;

	selinuxfs_kobj = kobject_create_and_add("selinux", fs_kobj);
	if (!selinuxfs_kobj)
		return -ENOMEM;

	err = register_filesystem(&sel_fs_type);
	if (err) {
		kobject_put(selinuxfs_kobj);
		return err;
	}

	selinuxfs_mount = kern_mount(&sel_fs_type);
	if (IS_ERR(selinuxfs_mount)) {
		printk(KERN_ERR "selinuxfs:  could not mount!\n");
		err = PTR_ERR(selinuxfs_mount);
		selinuxfs_mount = NULL;
=======
	.init_fs_context = sel_init_fs_context,
	.kill_sb	= sel_kill_sb,
};

struct path selinux_null __ro_after_init;

static int __init init_sel_fs(void)
{
	struct qstr null_name = QSTR_INIT(NULL_FILE_NAME,
					  sizeof(NULL_FILE_NAME)-1);
	int err;

	if (!selinux_enabled_boot)
		return 0;

	err = sysfs_create_mount_point(fs_kobj, "selinux");
	if (err)
		return err;

	err = register_filesystem(&sel_fs_type);
	if (err) {
		sysfs_remove_mount_point(fs_kobj, "selinux");
		return err;
	}

	selinux_null.mnt = kern_mount(&sel_fs_type);
	if (IS_ERR(selinux_null.mnt)) {
		pr_err("selinuxfs:  could not mount!\n");
		err = PTR_ERR(selinux_null.mnt);
		selinux_null.mnt = NULL;
		return err;
	}

	selinux_null.dentry = d_hash_and_lookup(selinux_null.mnt->mnt_root,
						&null_name);
	if (IS_ERR(selinux_null.dentry)) {
		pr_err("selinuxfs:  could not lookup null!\n");
		err = PTR_ERR(selinux_null.dentry);
		selinux_null.dentry = NULL;
		return err;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	}

	return err;
}

__initcall(init_sel_fs);
<<<<<<< HEAD

#ifdef CONFIG_SECURITY_SELINUX_DISABLE
void exit_sel_fs(void)
{
	kobject_put(selinuxfs_kobj);
	kern_unmount(selinuxfs_mount);
	unregister_filesystem(&sel_fs_type);
}
#endif
=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
