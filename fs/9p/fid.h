<<<<<<< HEAD
=======
/* SPDX-License-Identifier: GPL-2.0-only */
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
/*
 * V9FS FID Management
 *
 *  Copyright (C) 2005 by Eric Van Hensbergen <ericvh@gmail.com>
<<<<<<< HEAD
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License version 2
 *  as published by the Free Software Foundation.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to:
 *  Free Software Foundation
 *  51 Franklin Street, Fifth Floor
 *  Boston, MA  02111-1301  USA
 *
=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 */
#ifndef FS_9P_FID_H
#define FS_9P_FID_H
#include <linux/list.h>
<<<<<<< HEAD

/**
 * struct v9fs_dentry - 9p private data stored in dentry d_fsdata
 * @lock: protects the fidlist
 * @fidlist: list of FIDs currently associated with this dentry
 *
 * This structure defines the 9p private data associated with
 * a particular dentry.  In particular, this private data is used
 * to lookup which 9P FID handle should be used for a particular VFS
 * operation.  FID handles are associated with dentries instead of
 * inodes in order to more closely map functionality to the Plan 9
 * expected behavior for FID reclaimation and tracking.
 *
 * See Also: Mapping FIDs to Linux VFS model in
 * Design and Implementation of the Linux 9P File System documentation
 */
struct v9fs_dentry {
	spinlock_t lock; /* protect fidlist */
	struct list_head fidlist;
};

struct p9_fid *v9fs_fid_lookup(struct dentry *dentry);
struct p9_fid *v9fs_fid_clone(struct dentry *dentry);
int v9fs_fid_add(struct dentry *dentry, struct p9_fid *fid);
struct p9_fid *v9fs_writeback_fid(struct dentry *dentry);
=======
#include "v9fs.h"

struct p9_fid *v9fs_fid_find_inode(struct inode *inode, bool want_writeable,
	kuid_t uid, bool any);
struct p9_fid *v9fs_fid_lookup(struct dentry *dentry);
static inline struct p9_fid *v9fs_parent_fid(struct dentry *dentry)
{
	return v9fs_fid_lookup(dentry->d_parent);
}
void v9fs_fid_add(struct dentry *dentry, struct p9_fid **fid);
void v9fs_open_fid_add(struct inode *inode, struct p9_fid **fid);
static inline struct p9_fid *clone_fid(struct p9_fid *fid)
{
	return IS_ERR(fid) ? fid :  p9_client_walk(fid, 0, NULL, 1);
}
static inline struct p9_fid *v9fs_fid_clone(struct dentry *dentry)
{
	struct p9_fid *fid, *nfid;

	fid = v9fs_fid_lookup(dentry);
	if (!fid || IS_ERR(fid))
		return fid;

	nfid = clone_fid(fid);
	p9_fid_put(fid);
	return nfid;
}
/**
 * v9fs_fid_addmodes - add cache flags to fid mode (for client use only)
 * @fid: fid to augment
 * @s_flags: session info mount flags
 * @s_cache: session info cache flags
 * @f_flags: unix open flags
 *
 * make sure mode reflects flags of underlying mounts
 * also qid.version == 0 reflects a synthetic or legacy file system
 * NOTE: these are set after open so only reflect 9p client not
 * underlying file system on server.
 */
static inline void v9fs_fid_add_modes(struct p9_fid *fid, unsigned int s_flags,
	unsigned int s_cache, unsigned int f_flags)
{
	if ((!s_cache) ||
	   ((fid->qid.version == 0) && !(s_flags & V9FS_IGNORE_QV)) ||
	   (s_flags & V9FS_DIRECT_IO) || (f_flags & O_DIRECT)) {
		fid->mode |= P9L_DIRECT; /* no read or write cache */
	} else if ((!(s_cache & CACHE_WRITEBACK)) ||
				(f_flags & O_DSYNC) || (s_flags & V9FS_SYNC)) {
		fid->mode |= P9L_NOWRITECACHE;
	}
}
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#endif
