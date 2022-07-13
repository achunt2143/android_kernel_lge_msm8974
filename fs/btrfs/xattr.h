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
 */

#ifndef __XATTR__
#define __XATTR__

#include <linux/xattr.h>

extern const struct xattr_handler btrfs_xattr_acl_access_handler;
extern const struct xattr_handler btrfs_xattr_acl_default_handler;
extern const struct xattr_handler *btrfs_xattr_handlers[];

extern ssize_t __btrfs_getxattr(struct inode *inode, const char *name,
		void *buffer, size_t size);
extern int __btrfs_setxattr(struct btrfs_trans_handle *trans,
			    struct inode *inode, const char *name,
			    const void *value, size_t size, int flags);
extern ssize_t btrfs_getxattr(struct dentry *dentry, const char *name,
		void *buffer, size_t size);
extern int btrfs_setxattr(struct dentry *dentry, const char *name,
		const void *value, size_t size, int flags);
extern int btrfs_removexattr(struct dentry *dentry, const char *name);

extern int btrfs_xattr_security_init(struct btrfs_trans_handle *trans,
				     struct inode *inode, struct inode *dir,
				     const struct qstr *qstr);

#endif /* __XATTR__ */
=======
/* SPDX-License-Identifier: GPL-2.0 */
/*
 * Copyright (C) 2007 Red Hat.  All rights reserved.
 */

#ifndef BTRFS_XATTR_H
#define BTRFS_XATTR_H

struct dentry;
struct inode;
struct qstr;
struct xattr_handler;
struct btrfs_trans_handle;

extern const struct xattr_handler * const btrfs_xattr_handlers[];

int btrfs_getxattr(struct inode *inode, const char *name,
		void *buffer, size_t size);
int btrfs_setxattr(struct btrfs_trans_handle *trans, struct inode *inode,
		   const char *name, const void *value, size_t size, int flags);
int btrfs_setxattr_trans(struct inode *inode, const char *name,
			 const void *value, size_t size, int flags);
ssize_t btrfs_listxattr(struct dentry *dentry, char *buffer, size_t size);

int btrfs_xattr_security_init(struct btrfs_trans_handle *trans,
				     struct inode *inode, struct inode *dir,
				     const struct qstr *qstr);

#endif
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
