<<<<<<< HEAD
/*
 * Copyright (C) 2007 Red Hat.  All rights reserved.
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public
 * License v2 as published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * General Public License for more details.
 *
 * You should have received a copy of the GNU General Public
 * License along with this program; if not, write to the
 * Free Software Foundation, Inc., 59 Temple Place - Suite 330,
 * Boston, MA 021110-1307, USA.
=======
// SPDX-License-Identifier: GPL-2.0
/*
 * Copyright (C) 2007 Red Hat.  All rights reserved.
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 */

#include <linux/fs.h>
#include <linux/string.h>
#include <linux/xattr.h>
#include <linux/posix_acl_xattr.h>
#include <linux/posix_acl.h>
#include <linux/sched.h>
<<<<<<< HEAD
#include <linux/slab.h>

#include "ctree.h"
#include "btrfs_inode.h"
#include "xattr.h"

struct posix_acl *btrfs_get_acl(struct inode *inode, int type)
=======
#include <linux/sched/mm.h>
#include <linux/slab.h>
#include "ctree.h"
#include "xattr.h"
#include "acl.h"

struct posix_acl *btrfs_get_acl(struct inode *inode, int type, bool rcu)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
{
	int size;
	const char *name;
	char *value = NULL;
	struct posix_acl *acl;

<<<<<<< HEAD
	if (!IS_POSIXACL(inode))
		return NULL;

	acl = get_cached_acl(inode, type);
	if (acl != ACL_NOT_CACHED)
		return acl;

	switch (type) {
	case ACL_TYPE_ACCESS:
		name = POSIX_ACL_XATTR_ACCESS;
		break;
	case ACL_TYPE_DEFAULT:
		name = POSIX_ACL_XATTR_DEFAULT;
		break;
	default:
		BUG();
	}

	size = __btrfs_getxattr(inode, name, "", 0);
	if (size > 0) {
		value = kzalloc(size, GFP_NOFS);
		if (!value)
			return ERR_PTR(-ENOMEM);
		size = __btrfs_getxattr(inode, name, value, size);
	}
	if (size > 0) {
		acl = posix_acl_from_xattr(value, size);
	} else if (size == -ENOENT || size == -ENODATA || size == 0) {
		/* FIXME, who returns -ENOENT?  I think nobody */
		acl = NULL;
	} else {
		acl = ERR_PTR(-EIO);
	}
	kfree(value);

	if (!IS_ERR(acl))
		set_cached_acl(inode, type, acl);

	return acl;
}

static int btrfs_xattr_acl_get(struct dentry *dentry, const char *name,
		void *value, size_t size, int type)
{
	struct posix_acl *acl;
	int ret = 0;

	if (!IS_POSIXACL(dentry->d_inode))
		return -EOPNOTSUPP;

	acl = btrfs_get_acl(dentry->d_inode, type);

	if (IS_ERR(acl))
		return PTR_ERR(acl);
	if (acl == NULL)
		return -ENODATA;
	ret = posix_acl_to_xattr(acl, value, size);
	posix_acl_release(acl);

	return ret;
}

/*
 * Needs to be called with fs_mutex held
 */
static int btrfs_set_acl(struct btrfs_trans_handle *trans,
			 struct inode *inode, struct posix_acl *acl, int type)
=======
	if (rcu)
		return ERR_PTR(-ECHILD);

	switch (type) {
	case ACL_TYPE_ACCESS:
		name = XATTR_NAME_POSIX_ACL_ACCESS;
		break;
	case ACL_TYPE_DEFAULT:
		name = XATTR_NAME_POSIX_ACL_DEFAULT;
		break;
	default:
		return ERR_PTR(-EINVAL);
	}

	size = btrfs_getxattr(inode, name, NULL, 0);
	if (size > 0) {
		value = kzalloc(size, GFP_KERNEL);
		if (!value)
			return ERR_PTR(-ENOMEM);
		size = btrfs_getxattr(inode, name, value, size);
	}
	if (size > 0)
		acl = posix_acl_from_xattr(&init_user_ns, value, size);
	else if (size == -ENODATA || size == 0)
		acl = NULL;
	else
		acl = ERR_PTR(size);
	kfree(value);

	return acl;
}

int __btrfs_set_acl(struct btrfs_trans_handle *trans, struct inode *inode,
		    struct posix_acl *acl, int type)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
{
	int ret, size = 0;
	const char *name;
	char *value = NULL;

<<<<<<< HEAD
	if (acl) {
		ret = posix_acl_valid(acl);
		if (ret < 0)
			return ret;
		ret = 0;
	}

	switch (type) {
	case ACL_TYPE_ACCESS:
		name = POSIX_ACL_XATTR_ACCESS;
		if (acl) {
			ret = posix_acl_update_mode(inode, &inode->i_mode, &acl);
			if (ret)
				return ret;
		}
		ret = 0;
=======
	switch (type) {
	case ACL_TYPE_ACCESS:
		name = XATTR_NAME_POSIX_ACL_ACCESS;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		break;
	case ACL_TYPE_DEFAULT:
		if (!S_ISDIR(inode->i_mode))
			return acl ? -EINVAL : 0;
<<<<<<< HEAD
		name = POSIX_ACL_XATTR_DEFAULT;
=======
		name = XATTR_NAME_POSIX_ACL_DEFAULT;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		break;
	default:
		return -EINVAL;
	}

	if (acl) {
<<<<<<< HEAD
		size = posix_acl_xattr_size(acl->a_count);
		value = kmalloc(size, GFP_NOFS);
=======
		unsigned int nofs_flag;

		size = posix_acl_xattr_size(acl->a_count);
		/*
		 * We're holding a transaction handle, so use a NOFS memory
		 * allocation context to avoid deadlock if reclaim happens.
		 */
		nofs_flag = memalloc_nofs_save();
		value = kmalloc(size, GFP_KERNEL);
		memalloc_nofs_restore(nofs_flag);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		if (!value) {
			ret = -ENOMEM;
			goto out;
		}

<<<<<<< HEAD
		ret = posix_acl_to_xattr(acl, value, size);
=======
		ret = posix_acl_to_xattr(&init_user_ns, acl, value, size);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		if (ret < 0)
			goto out;
	}

<<<<<<< HEAD
	ret = __btrfs_setxattr(trans, inode, name, value, size, 0);
=======
	if (trans)
		ret = btrfs_setxattr(trans, inode, name, value, size, 0);
	else
		ret = btrfs_setxattr_trans(inode, name, value, size, 0);

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
out:
	kfree(value);

	if (!ret)
		set_cached_acl(inode, type, acl);

	return ret;
}

<<<<<<< HEAD
static int btrfs_xattr_acl_set(struct dentry *dentry, const char *name,
		const void *value, size_t size, int flags, int type)
{
	int ret;
	struct posix_acl *acl = NULL;

	if (!inode_owner_or_capable(dentry->d_inode))
		return -EPERM;

	if (!IS_POSIXACL(dentry->d_inode))
		return -EOPNOTSUPP;

	if (value) {
		acl = posix_acl_from_xattr(value, size);
		if (IS_ERR(acl))
			return PTR_ERR(acl);

		if (acl) {
			ret = posix_acl_valid(acl);
			if (ret)
				goto out;
		}
	}

	ret = btrfs_set_acl(NULL, dentry->d_inode, acl, type);
out:
	posix_acl_release(acl);

	return ret;
}

/*
 * btrfs_init_acl is already generally called under fs_mutex, so the locking
 * stuff has been fixed to work with that.  If the locking stuff changes, we
 * need to re-evaluate the acl locking stuff.
 */
int btrfs_init_acl(struct btrfs_trans_handle *trans,
		   struct inode *inode, struct inode *dir)
{
	struct posix_acl *acl = NULL;
	int ret = 0;

	/* this happens with subvols */
	if (!dir)
		return 0;

	if (!S_ISLNK(inode->i_mode)) {
		if (IS_POSIXACL(dir)) {
			acl = btrfs_get_acl(dir, ACL_TYPE_DEFAULT);
			if (IS_ERR(acl))
				return PTR_ERR(acl);
		}

		if (!acl)
			inode->i_mode &= ~current_umask();
	}

	if (IS_POSIXACL(dir) && acl) {
		if (S_ISDIR(inode->i_mode)) {
			ret = btrfs_set_acl(trans, inode, acl,
					    ACL_TYPE_DEFAULT);
			if (ret)
				goto failed;
		}
		ret = posix_acl_create(&acl, GFP_NOFS, &inode->i_mode);
		if (ret < 0)
			return ret;

		if (ret > 0) {
			/* we need an acl */
			ret = btrfs_set_acl(trans, inode, acl, ACL_TYPE_ACCESS);
		}
	}
failed:
	posix_acl_release(acl);

	return ret;
}

int btrfs_acl_chmod(struct inode *inode)
{
	struct posix_acl *acl;
	int ret = 0;

	if (S_ISLNK(inode->i_mode))
		return -EOPNOTSUPP;

	if (!IS_POSIXACL(inode))
		return 0;

	acl = btrfs_get_acl(inode, ACL_TYPE_ACCESS);
	if (IS_ERR_OR_NULL(acl))
		return PTR_ERR(acl);

	ret = posix_acl_chmod(&acl, GFP_KERNEL, inode->i_mode);
	if (ret)
		return ret;
	ret = btrfs_set_acl(NULL, inode, acl, ACL_TYPE_ACCESS);
	posix_acl_release(acl);
	return ret;
}

const struct xattr_handler btrfs_xattr_acl_default_handler = {
	.prefix = POSIX_ACL_XATTR_DEFAULT,
	.flags	= ACL_TYPE_DEFAULT,
	.get	= btrfs_xattr_acl_get,
	.set	= btrfs_xattr_acl_set,
};

const struct xattr_handler btrfs_xattr_acl_access_handler = {
	.prefix = POSIX_ACL_XATTR_ACCESS,
	.flags	= ACL_TYPE_ACCESS,
	.get	= btrfs_xattr_acl_get,
	.set	= btrfs_xattr_acl_set,
};
=======
int btrfs_set_acl(struct mnt_idmap *idmap, struct dentry *dentry,
		  struct posix_acl *acl, int type)
{
	int ret;
	struct inode *inode = d_inode(dentry);
	umode_t old_mode = inode->i_mode;

	if (type == ACL_TYPE_ACCESS && acl) {
		ret = posix_acl_update_mode(idmap, inode,
					    &inode->i_mode, &acl);
		if (ret)
			return ret;
	}
	ret = __btrfs_set_acl(NULL, inode, acl, type);
	if (ret)
		inode->i_mode = old_mode;
	return ret;
}
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
