<<<<<<< HEAD
/*
 * Copyright (C) Sistina Software, Inc.  1997-2003 All rights reserved.
 * Copyright (C) 2004-2006 Red Hat, Inc.  All rights reserved.
 *
 * This copyrighted material is made available to anyone wishing to use,
 * modify, copy, or redistribute it subject to the terms and conditions
 * of the GNU General Public License version 2.
=======
/* SPDX-License-Identifier: GPL-2.0-only */
/*
 * Copyright (C) Sistina Software, Inc.  1997-2003 All rights reserved.
 * Copyright (C) 2004-2006 Red Hat, Inc.  All rights reserved.
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 */

#ifndef __ACL_DOT_H__
#define __ACL_DOT_H__

#include "incore.h"

<<<<<<< HEAD
#define GFS2_POSIX_ACL_ACCESS		"posix_acl_access"
#define GFS2_POSIX_ACL_DEFAULT		"posix_acl_default"
#define GFS2_ACL_MAX_ENTRIES		25

extern struct posix_acl *gfs2_get_acl(struct inode *inode, int type);
extern int gfs2_acl_create(struct gfs2_inode *dip, struct inode *inode);
extern int gfs2_acl_chmod(struct gfs2_inode *ip, struct iattr *attr);
extern const struct xattr_handler gfs2_xattr_system_handler;
=======
#define GFS2_ACL_MAX_ENTRIES(sdp) ((300 << (sdp)->sd_sb.sb_bsize_shift) >> 12)

struct posix_acl *gfs2_get_acl(struct inode *inode, int type, bool rcu);
int __gfs2_set_acl(struct inode *inode, struct posix_acl *acl, int type);
int gfs2_set_acl(struct mnt_idmap *idmap, struct dentry *dentry,
		 struct posix_acl *acl, int type);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

#endif /* __ACL_DOT_H__ */
