<<<<<<< HEAD
/*
 * Copyright IBM Corporation, 2010
 * Author Aneesh Kumar K.V <aneesh.kumar@linux.vnet.ibm.com>
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of version 2.1 of the GNU Lesser General Public License
 * as published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it would be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 *
=======
/* SPDX-License-Identifier: LGPL-2.1 */
/*
 * Copyright IBM Corporation, 2010
 * Author Aneesh Kumar K.V <aneesh.kumar@linux.vnet.ibm.com>
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 */
#ifndef FS_9P_ACL_H
#define FS_9P_ACL_H

#ifdef CONFIG_9P_FS_POSIX_ACL
<<<<<<< HEAD
extern int v9fs_get_acl(struct inode *, struct p9_fid *);
extern struct posix_acl *v9fs_iop_get_acl(struct inode *inode, int type);
extern int v9fs_acl_chmod(struct dentry *);
extern int v9fs_set_create_acl(struct dentry *,
			       struct posix_acl **, struct posix_acl **);
extern int v9fs_acl_mode(struct inode *dir, umode_t *modep,
			 struct posix_acl **dpacl, struct posix_acl **pacl);
#else
#define v9fs_iop_get_acl NULL
=======
int v9fs_get_acl(struct inode *inode, struct p9_fid *fid);
struct posix_acl *v9fs_iop_get_inode_acl(struct inode *inode, int type,
				   bool rcu);
struct posix_acl *v9fs_iop_get_acl(struct mnt_idmap *idmap,
					  struct dentry *dentry, int type);
int v9fs_iop_set_acl(struct mnt_idmap *idmap, struct dentry *dentry,
		     struct posix_acl *acl, int type);
int v9fs_acl_chmod(struct inode *inode, struct p9_fid *fid);
int v9fs_set_create_acl(struct inode *inode, struct p9_fid *fid,
			struct posix_acl *dacl, struct posix_acl *acl);
int v9fs_acl_mode(struct inode *dir, umode_t *modep,
		  struct posix_acl **dpacl, struct posix_acl **pacl);
void v9fs_put_acl(struct posix_acl *dacl, struct posix_acl *acl);
#else
#define v9fs_iop_get_inode_acl	NULL
#define v9fs_iop_get_acl NULL
#define v9fs_iop_set_acl NULL
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
static inline int v9fs_get_acl(struct inode *inode, struct p9_fid *fid)
{
	return 0;
}
<<<<<<< HEAD
static inline int v9fs_acl_chmod(struct dentry *dentry)
{
	return 0;
}
static inline int v9fs_set_create_acl(struct dentry *dentry,
				      struct posix_acl **dpacl,
				      struct posix_acl **pacl)
{
	return 0;
}
=======
static inline int v9fs_acl_chmod(struct inode *inode, struct p9_fid *fid)
{
	return 0;
}
static inline int v9fs_set_create_acl(struct inode *inode,
				      struct p9_fid *fid,
				      struct posix_acl *dacl,
				      struct posix_acl *acl)
{
	return 0;
}
static inline void v9fs_put_acl(struct posix_acl *dacl,
				struct posix_acl *acl)
{
}
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
static inline int v9fs_acl_mode(struct inode *dir, umode_t *modep,
				struct posix_acl **dpacl,
				struct posix_acl **pacl)
{
	return 0;
}

#endif
#endif /* FS_9P_XATTR_H */
