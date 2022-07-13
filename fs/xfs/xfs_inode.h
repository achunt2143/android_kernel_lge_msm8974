<<<<<<< HEAD
/*
 * Copyright (c) 2000-2003,2005 Silicon Graphics, Inc.
 * All Rights Reserved.
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it would be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write the Free Software Foundation,
 * Inc.,  51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
=======
// SPDX-License-Identifier: GPL-2.0
/*
 * Copyright (c) 2000-2003,2005 Silicon Graphics, Inc.
 * All Rights Reserved.
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 */
#ifndef	__XFS_INODE_H__
#define	__XFS_INODE_H__

<<<<<<< HEAD
struct posix_acl;
struct xfs_dinode;
struct xfs_inode;

/*
 * Fork identifiers.
 */
#define	XFS_DATA_FORK	0
#define	XFS_ATTR_FORK	1

/*
 * The following xfs_ext_irec_t struct introduces a second (top) level
 * to the in-core extent allocation scheme. These structs are allocated
 * in a contiguous block, creating an indirection array where each entry
 * (irec) contains a pointer to a buffer of in-core extent records which
 * it manages. Each extent buffer is 4k in size, since 4k is the system
 * page size on Linux i386 and systems with larger page sizes don't seem
 * to gain much, if anything, by using their native page size as the
 * extent buffer size. Also, using 4k extent buffers everywhere provides
 * a consistent interface for CXFS across different platforms.
 *
 * There is currently no limit on the number of irec's (extent lists)
 * allowed, so heavily fragmented files may require an indirection array
 * which spans multiple system pages of memory. The number of extents
 * which would require this amount of contiguous memory is very large
 * and should not cause problems in the foreseeable future. However,
 * if the memory needed for the contiguous array ever becomes a problem,
 * it is possible that a third level of indirection may be required.
 */
typedef struct xfs_ext_irec {
	xfs_bmbt_rec_host_t *er_extbuf;	/* block of extent records */
	xfs_extnum_t	er_extoff;	/* extent offset in file */
	xfs_extnum_t	er_extcount;	/* number of extents in page/block */
} xfs_ext_irec_t;

/*
 * File incore extent information, present for each of data & attr forks.
 */
#define	XFS_IEXT_BUFSZ		4096
#define	XFS_LINEAR_EXTS		(XFS_IEXT_BUFSZ / (uint)sizeof(xfs_bmbt_rec_t))
#define	XFS_INLINE_EXTS		2
#define	XFS_INLINE_DATA		32
typedef struct xfs_ifork {
	int			if_bytes;	/* bytes in if_u1 */
	int			if_real_bytes;	/* bytes allocated in if_u1 */
	struct xfs_btree_block	*if_broot;	/* file's incore btree root */
	short			if_broot_bytes;	/* bytes allocated for root */
	unsigned char		if_flags;	/* per-fork flags */
	union {
		xfs_bmbt_rec_host_t *if_extents;/* linear map file exts */
		xfs_ext_irec_t	*if_ext_irec;	/* irec map file exts */
		char		*if_data;	/* inline file data */
	} if_u1;
	union {
		xfs_bmbt_rec_host_t if_inline_ext[XFS_INLINE_EXTS];
						/* very small file extents */
		char		if_inline_data[XFS_INLINE_DATA];
						/* very small file data */
		xfs_dev_t	if_rdev;	/* dev number if special */
		uuid_t		if_uuid;	/* mount point value */
	} if_u2;
} xfs_ifork_t;

/*
 * Inode location information.  Stored in the inode and passed to
 * xfs_imap_to_bp() to get a buffer and dinode for a given inode.
 */
struct xfs_imap {
	xfs_daddr_t	im_blkno;	/* starting BB of inode chunk */
	ushort		im_len;		/* length in BBs of inode chunk */
	ushort		im_boffset;	/* inode offset in block in bytes */
};

/*
 * This is the xfs in-core inode structure.
 * Most of the on-disk inode is embedded in the i_d field.
 *
 * The extent pointers/inline file space, however, are managed
 * separately.  The memory for this information is pointed to by
 * the if_u1 unions depending on the type of the data.
 * This is used to linearize the array of extents for fast in-core
 * access.  This is used until the file's number of extents
 * surpasses XFS_MAX_INCORE_EXTENTS, at which point all extent pointers
 * are accessed through the buffer cache.
 *
 * Other state kept in the in-core inode is used for identification,
 * locking, transactional updating, etc of the inode.
 *
 * Generally, we do not want to hold the i_rlock while holding the
 * i_ilock. Hierarchy is i_iolock followed by i_rlock.
 *
 * xfs_iptr_t contains all the inode fields up to and including the
 * i_mnext and i_mprev fields, it is used as a marker in the inode
 * chain off the mount structure by xfs_sync calls.
 */

typedef struct xfs_ictimestamp {
	__int32_t	t_sec;		/* timestamp seconds */
	__int32_t	t_nsec;		/* timestamp nanoseconds */
} xfs_ictimestamp_t;

/*
 * NOTE:  This structure must be kept identical to struct xfs_dinode
 * 	  in xfs_dinode.h except for the endianness annotations.
 */
typedef struct xfs_icdinode {
	__uint16_t	di_magic;	/* inode magic # = XFS_DINODE_MAGIC */
	__uint16_t	di_mode;	/* mode and type of file */
	__int8_t	di_version;	/* inode version */
	__int8_t	di_format;	/* format of di_c data */
	__uint16_t	di_onlink;	/* old number of links to file */
	__uint32_t	di_uid;		/* owner's user id */
	__uint32_t	di_gid;		/* owner's group id */
	__uint32_t	di_nlink;	/* number of links to file */
	__uint16_t	di_projid_lo;	/* lower part of owner's project id */
	__uint16_t	di_projid_hi;	/* higher part of owner's project id */
	__uint8_t	di_pad[6];	/* unused, zeroed space */
	__uint16_t	di_flushiter;	/* incremented on flush */
	xfs_ictimestamp_t di_atime;	/* time last accessed */
	xfs_ictimestamp_t di_mtime;	/* time last modified */
	xfs_ictimestamp_t di_ctime;	/* time created/inode modified */
	xfs_fsize_t	di_size;	/* number of bytes in file */
	xfs_drfsbno_t	di_nblocks;	/* # of direct & btree blocks used */
	xfs_extlen_t	di_extsize;	/* basic/minimum extent size for file */
	xfs_extnum_t	di_nextents;	/* number of extents in data fork */
	xfs_aextnum_t	di_anextents;	/* number of extents in attribute fork*/
	__uint8_t	di_forkoff;	/* attr fork offs, <<3 for 64b align */
	__int8_t	di_aformat;	/* format of attr fork's data */
	__uint32_t	di_dmevmask;	/* DMIG event mask */
	__uint16_t	di_dmstate;	/* DMIG state info */
	__uint16_t	di_flags;	/* random flags, XFS_DIFLAG_... */
	__uint32_t	di_gen;		/* generation number */
} xfs_icdinode_t;

/*
 * Flags for xfs_ichgtime().
 */
#define	XFS_ICHGTIME_MOD	0x1	/* data fork modification timestamp */
#define	XFS_ICHGTIME_CHG	0x2	/* inode field change timestamp */

/*
 * Per-fork incore inode flags.
 */
#define	XFS_IFINLINE	0x01	/* Inline data is read in */
#define	XFS_IFEXTENTS	0x02	/* All extent pointers are read in */
#define	XFS_IFBROOT	0x04	/* i_broot points to the bmap b-tree root */
#define	XFS_IFEXTIREC	0x08	/* Indirection array of extent blocks */

/*
 * Fork handling.
 */

#define XFS_IFORK_Q(ip)			((ip)->i_d.di_forkoff != 0)
#define XFS_IFORK_BOFF(ip)		((int)((ip)->i_d.di_forkoff << 3))

#define XFS_IFORK_PTR(ip,w)		\
	((w) == XFS_DATA_FORK ? \
		&(ip)->i_df : \
		(ip)->i_afp)
#define XFS_IFORK_DSIZE(ip) \
	(XFS_IFORK_Q(ip) ? \
		XFS_IFORK_BOFF(ip) : \
		XFS_LITINO((ip)->i_mount))
#define XFS_IFORK_ASIZE(ip) \
	(XFS_IFORK_Q(ip) ? \
		XFS_LITINO((ip)->i_mount) - XFS_IFORK_BOFF(ip) : \
		0)
#define XFS_IFORK_SIZE(ip,w) \
	((w) == XFS_DATA_FORK ? \
		XFS_IFORK_DSIZE(ip) : \
		XFS_IFORK_ASIZE(ip))
#define XFS_IFORK_FORMAT(ip,w) \
	((w) == XFS_DATA_FORK ? \
		(ip)->i_d.di_format : \
		(ip)->i_d.di_aformat)
#define XFS_IFORK_FMT_SET(ip,w,n) \
	((w) == XFS_DATA_FORK ? \
		((ip)->i_d.di_format = (n)) : \
		((ip)->i_d.di_aformat = (n)))
#define XFS_IFORK_NEXTENTS(ip,w) \
	((w) == XFS_DATA_FORK ? \
		(ip)->i_d.di_nextents : \
		(ip)->i_d.di_anextents)
#define XFS_IFORK_NEXT_SET(ip,w,n) \
	((w) == XFS_DATA_FORK ? \
		((ip)->i_d.di_nextents = (n)) : \
		((ip)->i_d.di_anextents = (n)))
#define XFS_IFORK_MAXEXT(ip, w) \
	(XFS_IFORK_SIZE(ip, w) / sizeof(xfs_bmbt_rec_t))


#ifdef __KERNEL__

struct xfs_buf;
struct xfs_bmap_free;
=======
#include "xfs_inode_buf.h"
#include "xfs_inode_fork.h"

/*
 * Kernel only inode definitions
 */
struct xfs_dinode;
struct xfs_inode;
struct xfs_buf;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
struct xfs_bmbt_irec;
struct xfs_inode_log_item;
struct xfs_mount;
struct xfs_trans;
struct xfs_dquot;

typedef struct xfs_inode {
	/* Inode linking and identification information. */
	struct xfs_mount	*i_mount;	/* fs mount struct ptr */
	struct xfs_dquot	*i_udquot;	/* user dquot */
	struct xfs_dquot	*i_gdquot;	/* group dquot */
<<<<<<< HEAD
=======
	struct xfs_dquot	*i_pdquot;	/* project dquot */
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	/* Inode location stuff */
	xfs_ino_t		i_ino;		/* inode number (agno/agino)*/
	struct xfs_imap		i_imap;		/* location for xfs_imap() */

	/* Extent information. */
<<<<<<< HEAD
	xfs_ifork_t		*i_afp;		/* attribute fork pointer */
	xfs_ifork_t		i_df;		/* data fork */

	/* Transaction and locking information. */
	struct xfs_inode_log_item *i_itemp;	/* logging information */
	mrlock_t		i_lock;		/* inode lock */
	mrlock_t		i_iolock;	/* inode IO lock */
	atomic_t		i_pincount;	/* inode pin count */
	spinlock_t		i_flags_lock;	/* inode i_flags lock */
	/* Miscellaneous state. */
	unsigned long		i_flags;	/* see defined flags below */
	unsigned int		i_delayed_blks;	/* count of delay alloc blks */

	xfs_icdinode_t		i_d;		/* most of ondisk inode */

	/* VFS inode */
	struct inode		i_vnode;	/* embedded VFS inode */
} xfs_inode_t;

=======
	struct xfs_ifork	*i_cowfp;	/* copy on write extents */
	struct xfs_ifork	i_df;		/* data fork */
	struct xfs_ifork	i_af;		/* attribute fork */

	/* Transaction and locking information. */
	struct xfs_inode_log_item *i_itemp;	/* logging information */
	struct rw_semaphore	i_lock;		/* inode lock */
	atomic_t		i_pincount;	/* inode pin count */
	struct llist_node	i_gclist;	/* deferred inactivation list */

	/*
	 * Bitsets of inode metadata that have been checked and/or are sick.
	 * Callers must hold i_flags_lock before accessing this field.
	 */
	uint16_t		i_checked;
	uint16_t		i_sick;

	spinlock_t		i_flags_lock;	/* inode i_flags lock */
	/* Miscellaneous state. */
	unsigned long		i_flags;	/* see defined flags below */
	uint64_t		i_delayed_blks;	/* count of delay alloc blks */
	xfs_fsize_t		i_disk_size;	/* number of bytes in file */
	xfs_rfsblock_t		i_nblocks;	/* # of direct & btree blocks */
	prid_t			i_projid;	/* owner's project id */
	xfs_extlen_t		i_extsize;	/* basic/minimum extent size */
	/* cowextsize is only used for v3 inodes, flushiter for v1/2 */
	union {
		xfs_extlen_t	i_cowextsize;	/* basic cow extent size */
		uint16_t	i_flushiter;	/* incremented on flush */
	};
	uint8_t			i_forkoff;	/* attr fork offset >> 3 */
	uint16_t		i_diflags;	/* XFS_DIFLAG_... */
	uint64_t		i_diflags2;	/* XFS_DIFLAG2_... */
	struct timespec64	i_crtime;	/* time created */

	/*
	 * Unlinked list pointers.  These point to the next and previous inodes
	 * in the AGI unlinked bucket list, respectively.  These fields can
	 * only be updated with the AGI locked.
	 *
	 * i_next_unlinked caches di_next_unlinked.
	 */
	xfs_agino_t		i_next_unlinked;

	/*
	 * If the inode is not on an unlinked list, this field is zero.  If the
	 * inode is the first element in an unlinked list, this field is
	 * NULLAGINO.  Otherwise, i_prev_unlinked points to the previous inode
	 * in the unlinked list.
	 */
	xfs_agino_t		i_prev_unlinked;

	/* VFS inode */
	struct inode		i_vnode;	/* embedded VFS inode */

	/* pending io completions */
	spinlock_t		i_ioend_lock;
	struct work_struct	i_ioend_work;
	struct list_head	i_ioend_list;
} xfs_inode_t;

static inline bool xfs_inode_on_unlinked_list(const struct xfs_inode *ip)
{
	return ip->i_prev_unlinked != 0;
}

static inline bool xfs_inode_has_attr_fork(struct xfs_inode *ip)
{
	return ip->i_forkoff > 0;
}

static inline struct xfs_ifork *
xfs_ifork_ptr(
	struct xfs_inode	*ip,
	int			whichfork)
{
	switch (whichfork) {
	case XFS_DATA_FORK:
		return &ip->i_df;
	case XFS_ATTR_FORK:
		if (!xfs_inode_has_attr_fork(ip))
			return NULL;
		return &ip->i_af;
	case XFS_COW_FORK:
		return ip->i_cowfp;
	default:
		ASSERT(0);
		return NULL;
	}
}

static inline unsigned int xfs_inode_fork_boff(struct xfs_inode *ip)
{
	return ip->i_forkoff << 3;
}

static inline unsigned int xfs_inode_data_fork_size(struct xfs_inode *ip)
{
	if (xfs_inode_has_attr_fork(ip))
		return xfs_inode_fork_boff(ip);

	return XFS_LITINO(ip->i_mount);
}

static inline unsigned int xfs_inode_attr_fork_size(struct xfs_inode *ip)
{
	if (xfs_inode_has_attr_fork(ip))
		return XFS_LITINO(ip->i_mount) - xfs_inode_fork_boff(ip);
	return 0;
}

static inline unsigned int
xfs_inode_fork_size(
	struct xfs_inode	*ip,
	int			whichfork)
{
	switch (whichfork) {
	case XFS_DATA_FORK:
		return xfs_inode_data_fork_size(ip);
	case XFS_ATTR_FORK:
		return xfs_inode_attr_fork_size(ip);
	default:
		return 0;
	}
}

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
/* Convert from vfs inode to xfs inode */
static inline struct xfs_inode *XFS_I(struct inode *inode)
{
	return container_of(inode, struct xfs_inode, i_vnode);
}

/* convert from xfs inode to vfs inode */
static inline struct inode *VFS_I(struct xfs_inode *ip)
{
	return &ip->i_vnode;
}

<<<<<<< HEAD
=======
/* convert from const xfs inode to const vfs inode */
static inline const struct inode *VFS_IC(const struct xfs_inode *ip)
{
	return &ip->i_vnode;
}

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
/*
 * For regular files we only update the on-disk filesize when actually
 * writing data back to disk.  Until then only the copy in the VFS inode
 * is uptodate.
 */
static inline xfs_fsize_t XFS_ISIZE(struct xfs_inode *ip)
{
<<<<<<< HEAD
	if (S_ISREG(ip->i_d.di_mode))
		return i_size_read(VFS_I(ip));
	return ip->i_d.di_size;
=======
	if (S_ISREG(VFS_I(ip)->i_mode))
		return i_size_read(VFS_I(ip));
	return ip->i_disk_size;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

/*
 * If this I/O goes past the on-disk inode size update it unless it would
 * be past the current in-core inode size.
 */
static inline xfs_fsize_t
xfs_new_eof(struct xfs_inode *ip, xfs_fsize_t new_size)
{
	xfs_fsize_t i_size = i_size_read(VFS_I(ip));

<<<<<<< HEAD
	if (new_size > i_size)
		new_size = i_size;
	return new_size > ip->i_d.di_size ? new_size : 0;
=======
	if (new_size > i_size || new_size < 0)
		new_size = i_size;
	return new_size > ip->i_disk_size ? new_size : 0;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

/*
 * i_flags helper functions
 */
static inline void
__xfs_iflags_set(xfs_inode_t *ip, unsigned short flags)
{
	ip->i_flags |= flags;
}

static inline void
xfs_iflags_set(xfs_inode_t *ip, unsigned short flags)
{
	spin_lock(&ip->i_flags_lock);
	__xfs_iflags_set(ip, flags);
	spin_unlock(&ip->i_flags_lock);
}

static inline void
xfs_iflags_clear(xfs_inode_t *ip, unsigned short flags)
{
	spin_lock(&ip->i_flags_lock);
	ip->i_flags &= ~flags;
	spin_unlock(&ip->i_flags_lock);
}

static inline int
__xfs_iflags_test(xfs_inode_t *ip, unsigned short flags)
{
	return (ip->i_flags & flags);
}

static inline int
xfs_iflags_test(xfs_inode_t *ip, unsigned short flags)
{
	int ret;
	spin_lock(&ip->i_flags_lock);
	ret = __xfs_iflags_test(ip, flags);
	spin_unlock(&ip->i_flags_lock);
	return ret;
}

static inline int
xfs_iflags_test_and_clear(xfs_inode_t *ip, unsigned short flags)
{
	int ret;

	spin_lock(&ip->i_flags_lock);
	ret = ip->i_flags & flags;
	if (ret)
		ip->i_flags &= ~flags;
	spin_unlock(&ip->i_flags_lock);
	return ret;
}

static inline int
xfs_iflags_test_and_set(xfs_inode_t *ip, unsigned short flags)
{
	int ret;

	spin_lock(&ip->i_flags_lock);
	ret = ip->i_flags & flags;
	if (!ret)
		ip->i_flags |= flags;
	spin_unlock(&ip->i_flags_lock);
	return ret;
}

<<<<<<< HEAD
/*
 * Project quota id helpers (previously projid was 16bit only
 * and using two 16bit values to hold new 32bit projid was chosen
 * to retain compatibility with "old" filesystems).
 */
static inline prid_t
xfs_get_projid(struct xfs_inode *ip)
{
	return (prid_t)ip->i_d.di_projid_hi << 16 | ip->i_d.di_projid_lo;
}

static inline void
xfs_set_projid(struct xfs_inode *ip,
		prid_t projid)
{
	ip->i_d.di_projid_hi = (__uint16_t) (projid >> 16);
	ip->i_d.di_projid_lo = (__uint16_t) (projid & 0xffff);
}

=======
static inline prid_t
xfs_get_initial_prid(struct xfs_inode *dp)
{
	if (dp->i_diflags & XFS_DIFLAG_PROJINHERIT)
		return dp->i_projid;

	return XFS_PROJID_DEFAULT;
}

static inline bool xfs_is_reflink_inode(struct xfs_inode *ip)
{
	return ip->i_diflags2 & XFS_DIFLAG2_REFLINK;
}

static inline bool xfs_is_metadata_inode(struct xfs_inode *ip)
{
	struct xfs_mount	*mp = ip->i_mount;

	return ip == mp->m_rbmip || ip == mp->m_rsumip ||
		xfs_is_quota_inode(&mp->m_sb, ip->i_ino);
}

/*
 * Check if an inode has any data in the COW fork.  This might be often false
 * even for inodes with the reflink flag when there is no pending COW operation.
 */
static inline bool xfs_inode_has_cow_data(struct xfs_inode *ip)
{
	return ip->i_cowfp && ip->i_cowfp->if_bytes;
}

static inline bool xfs_inode_has_bigtime(struct xfs_inode *ip)
{
	return ip->i_diflags2 & XFS_DIFLAG2_BIGTIME;
}

static inline bool xfs_inode_has_large_extent_counts(struct xfs_inode *ip)
{
	return ip->i_diflags2 & XFS_DIFLAG2_NREXT64;
}

/*
 * Return the buftarg used for data allocations on a given inode.
 */
#define xfs_inode_buftarg(ip) \
	(XFS_IS_REALTIME_INODE(ip) ? \
		(ip)->i_mount->m_rtdev_targp : (ip)->i_mount->m_ddev_targp)

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
/*
 * In-core inode flags.
 */
#define XFS_IRECLAIM		(1 << 0) /* started reclaiming this inode */
#define XFS_ISTALE		(1 << 1) /* inode has been staled */
#define XFS_IRECLAIMABLE	(1 << 2) /* inode can be reclaimed */
#define XFS_INEW		(1 << 3) /* inode has just been allocated */
<<<<<<< HEAD
#define XFS_IFILESTREAM		(1 << 4) /* inode is in a filestream dir. */
#define XFS_ITRUNCATED		(1 << 5) /* truncated down so flush-on-close */
#define XFS_IDIRTY_RELEASE	(1 << 6) /* dirty release already seen */
#define __XFS_IFLOCK_BIT	7	 /* inode is being flushed right now */
#define XFS_IFLOCK		(1 << __XFS_IFLOCK_BIT)
#define __XFS_IPINNED_BIT	8	 /* wakeup key for zero pin count */
#define XFS_IPINNED		(1 << __XFS_IPINNED_BIT)
#define XFS_IDONTCACHE		(1 << 9) /* don't cache the inode long term */
=======
#define XFS_IPRESERVE_DM_FIELDS	(1 << 4) /* has legacy DMAPI fields set */
#define XFS_ITRUNCATED		(1 << 5) /* truncated down so flush-on-close */
#define XFS_IDIRTY_RELEASE	(1 << 6) /* dirty release already seen */
#define XFS_IFLUSHING		(1 << 7) /* inode is being flushed */
#define __XFS_IPINNED_BIT	8	 /* wakeup key for zero pin count */
#define XFS_IPINNED		(1 << __XFS_IPINNED_BIT)
#define XFS_IEOFBLOCKS		(1 << 9) /* has the preallocblocks tag set */
#define XFS_NEED_INACTIVE	(1 << 10) /* see XFS_INACTIVATING below */
/*
 * If this unlinked inode is in the middle of recovery, don't let drop_inode
 * truncate and free the inode.  This can happen if we iget the inode during
 * log recovery to replay a bmap operation on the inode.
 */
#define XFS_IRECOVERY		(1 << 11)
#define XFS_ICOWBLOCKS		(1 << 12)/* has the cowblocks tag set */

/*
 * If we need to update on-disk metadata before this IRECLAIMABLE inode can be
 * freed, then NEED_INACTIVE will be set.  Once we start the updates, the
 * INACTIVATING bit will be set to keep iget away from this inode.  After the
 * inactivation completes, both flags will be cleared and the inode is a
 * plain old IRECLAIMABLE inode.
 */
#define XFS_INACTIVATING	(1 << 13)

/* Quotacheck is running but inode has not been added to quota counts. */
#define XFS_IQUOTAUNCHECKED	(1 << 14)

/*
 * Remap in progress. Callers that wish to update file data while
 * holding a shared IOLOCK or MMAPLOCK must drop the lock and retake
 * the lock in exclusive mode. Relocking the file will block until
 * IREMAPPING is cleared.
 */
#define XFS_IREMAPPING		(1U << 15)

/* All inode state flags related to inode reclaim. */
#define XFS_ALL_IRECLAIM_FLAGS	(XFS_IRECLAIMABLE | \
				 XFS_IRECLAIM | \
				 XFS_NEED_INACTIVE | \
				 XFS_INACTIVATING)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

/*
 * Per-lifetime flags need to be reset when re-using a reclaimable inode during
 * inode lookup. This prevents unintended behaviour on the new inode from
 * ocurring.
 */
#define XFS_IRECLAIM_RESET_FLAGS	\
	(XFS_IRECLAIMABLE | XFS_IRECLAIM | \
<<<<<<< HEAD
	 XFS_IDIRTY_RELEASE | XFS_ITRUNCATED | \
	 XFS_IFILESTREAM);

/*
 * Synchronize processes attempting to flush the in-core inode back to disk.
 */

extern void __xfs_iflock(struct xfs_inode *ip);

static inline int xfs_iflock_nowait(struct xfs_inode *ip)
{
	return !xfs_iflags_test_and_set(ip, XFS_IFLOCK);
}

static inline void xfs_iflock(struct xfs_inode *ip)
{
	if (!xfs_iflock_nowait(ip))
		__xfs_iflock(ip);
}

static inline void xfs_ifunlock(struct xfs_inode *ip)
{
	xfs_iflags_clear(ip, XFS_IFLOCK);
	wake_up_bit(&ip->i_flags, __XFS_IFLOCK_BIT);
}

static inline int xfs_isiflocked(struct xfs_inode *ip)
{
	return xfs_iflags_test(ip, XFS_IFLOCK);
}
=======
	 XFS_IDIRTY_RELEASE | XFS_ITRUNCATED | XFS_NEED_INACTIVE | \
	 XFS_INACTIVATING | XFS_IQUOTAUNCHECKED)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

/*
 * Flags for inode locking.
 * Bit ranges:	1<<1  - 1<<16-1 -- iolock/ilock modes (bitfield)
 *		1<<16 - 1<<32-1 -- lockdep annotation (integers)
 */
<<<<<<< HEAD
#define	XFS_IOLOCK_EXCL		(1<<0)
#define	XFS_IOLOCK_SHARED	(1<<1)
#define	XFS_ILOCK_EXCL		(1<<2)
#define	XFS_ILOCK_SHARED	(1<<3)

#define XFS_LOCK_MASK		(XFS_IOLOCK_EXCL | XFS_IOLOCK_SHARED \
				| XFS_ILOCK_EXCL | XFS_ILOCK_SHARED)
=======
#define	XFS_IOLOCK_EXCL		(1u << 0)
#define	XFS_IOLOCK_SHARED	(1u << 1)
#define	XFS_ILOCK_EXCL		(1u << 2)
#define	XFS_ILOCK_SHARED	(1u << 3)
#define	XFS_MMAPLOCK_EXCL	(1u << 4)
#define	XFS_MMAPLOCK_SHARED	(1u << 5)

#define XFS_LOCK_MASK		(XFS_IOLOCK_EXCL | XFS_IOLOCK_SHARED \
				| XFS_ILOCK_EXCL | XFS_ILOCK_SHARED \
				| XFS_MMAPLOCK_EXCL | XFS_MMAPLOCK_SHARED)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

#define XFS_LOCK_FLAGS \
	{ XFS_IOLOCK_EXCL,	"IOLOCK_EXCL" }, \
	{ XFS_IOLOCK_SHARED,	"IOLOCK_SHARED" }, \
	{ XFS_ILOCK_EXCL,	"ILOCK_EXCL" }, \
<<<<<<< HEAD
	{ XFS_ILOCK_SHARED,	"ILOCK_SHARED" }
=======
	{ XFS_ILOCK_SHARED,	"ILOCK_SHARED" }, \
	{ XFS_MMAPLOCK_EXCL,	"MMAPLOCK_EXCL" }, \
	{ XFS_MMAPLOCK_SHARED,	"MMAPLOCK_SHARED" }
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)


/*
 * Flags for lockdep annotations.
 *
 * XFS_LOCK_PARENT - for directory operations that require locking a
<<<<<<< HEAD
 * parent directory inode and a child entry inode.  The parent gets locked
 * with this flag so it gets a lockdep subclass of 1 and the child entry
 * lock will have a lockdep subclass of 0.
=======
 * parent directory inode and a child entry inode. IOLOCK requires nesting,
 * MMAPLOCK does not support this class, ILOCK requires a single subclass
 * to differentiate parent from child.
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 *
 * XFS_LOCK_RTBITMAP/XFS_LOCK_RTSUM - the realtime device bitmap and summary
 * inodes do not participate in the normal lock order, and thus have their
 * own subclasses.
 *
 * XFS_LOCK_INUMORDER - for locking several inodes at the some time
 * with xfs_lock_inodes().  This flag is used as the starting subclass
 * and each subsequent lock acquired will increment the subclass by one.
<<<<<<< HEAD
 * So the first lock acquired will have a lockdep subclass of 4, the
 * second lock will have a lockdep subclass of 5, and so on. It is
 * the responsibility of the class builder to shift this to the correct
 * portion of the lock_mode lockdep mask.
 */
#define XFS_LOCK_PARENT		1
#define XFS_LOCK_RTBITMAP	2
#define XFS_LOCK_RTSUM		3
#define XFS_LOCK_INUMORDER	4

#define XFS_IOLOCK_SHIFT	16
#define	XFS_IOLOCK_PARENT	(XFS_LOCK_PARENT << XFS_IOLOCK_SHIFT)

#define XFS_ILOCK_SHIFT		24
#define	XFS_ILOCK_PARENT	(XFS_LOCK_PARENT << XFS_ILOCK_SHIFT)
#define	XFS_ILOCK_RTBITMAP	(XFS_LOCK_RTBITMAP << XFS_ILOCK_SHIFT)
#define	XFS_ILOCK_RTSUM		(XFS_LOCK_RTSUM << XFS_ILOCK_SHIFT)

#define XFS_IOLOCK_DEP_MASK	0x00ff0000
#define XFS_ILOCK_DEP_MASK	0xff000000
#define XFS_LOCK_DEP_MASK	(XFS_IOLOCK_DEP_MASK | XFS_ILOCK_DEP_MASK)

#define XFS_IOLOCK_DEP(flags)	(((flags) & XFS_IOLOCK_DEP_MASK) >> XFS_IOLOCK_SHIFT)
#define XFS_ILOCK_DEP(flags)	(((flags) & XFS_ILOCK_DEP_MASK) >> XFS_ILOCK_SHIFT)

extern struct lock_class_key xfs_iolock_reclaimable;
=======
 * However, MAX_LOCKDEP_SUBCLASSES == 8, which means we are greatly
 * limited to the subclasses we can represent via nesting. We need at least
 * 5 inodes nest depth for the ILOCK through rename, and we also have to support
 * XFS_ILOCK_PARENT, which gives 6 subclasses. Then we have XFS_ILOCK_RTBITMAP
 * and XFS_ILOCK_RTSUM, which are another 2 unique subclasses, so that's all
 * 8 subclasses supported by lockdep.
 *
 * This also means we have to number the sub-classes in the lowest bits of
 * the mask we keep, and we have to ensure we never exceed 3 bits of lockdep
 * mask and we can't use bit-masking to build the subclasses. What a mess.
 *
 * Bit layout:
 *
 * Bit		Lock Region
 * 16-19	XFS_IOLOCK_SHIFT dependencies
 * 20-23	XFS_MMAPLOCK_SHIFT dependencies
 * 24-31	XFS_ILOCK_SHIFT dependencies
 *
 * IOLOCK values
 *
 * 0-3		subclass value
 * 4-7		unused
 *
 * MMAPLOCK values
 *
 * 0-3		subclass value
 * 4-7		unused
 *
 * ILOCK values
 * 0-4		subclass values
 * 5		PARENT subclass (not nestable)
 * 6		RTBITMAP subclass (not nestable)
 * 7		RTSUM subclass (not nestable)
 * 
 */
#define XFS_IOLOCK_SHIFT		16
#define XFS_IOLOCK_MAX_SUBCLASS		3
#define XFS_IOLOCK_DEP_MASK		0x000f0000u

#define XFS_MMAPLOCK_SHIFT		20
#define XFS_MMAPLOCK_NUMORDER		0
#define XFS_MMAPLOCK_MAX_SUBCLASS	3
#define XFS_MMAPLOCK_DEP_MASK		0x00f00000u

#define XFS_ILOCK_SHIFT			24
#define XFS_ILOCK_PARENT_VAL		5u
#define XFS_ILOCK_MAX_SUBCLASS		(XFS_ILOCK_PARENT_VAL - 1)
#define XFS_ILOCK_RTBITMAP_VAL		6u
#define XFS_ILOCK_RTSUM_VAL		7u
#define XFS_ILOCK_DEP_MASK		0xff000000u
#define	XFS_ILOCK_PARENT		(XFS_ILOCK_PARENT_VAL << XFS_ILOCK_SHIFT)
#define	XFS_ILOCK_RTBITMAP		(XFS_ILOCK_RTBITMAP_VAL << XFS_ILOCK_SHIFT)
#define	XFS_ILOCK_RTSUM			(XFS_ILOCK_RTSUM_VAL << XFS_ILOCK_SHIFT)

#define XFS_LOCK_SUBCLASS_MASK	(XFS_IOLOCK_DEP_MASK | \
				 XFS_MMAPLOCK_DEP_MASK | \
				 XFS_ILOCK_DEP_MASK)

#define XFS_IOLOCK_DEP(flags)	(((flags) & XFS_IOLOCK_DEP_MASK) \
					>> XFS_IOLOCK_SHIFT)
#define XFS_MMAPLOCK_DEP(flags)	(((flags) & XFS_MMAPLOCK_DEP_MASK) \
					>> XFS_MMAPLOCK_SHIFT)
#define XFS_ILOCK_DEP(flags)	(((flags) & XFS_ILOCK_DEP_MASK) \
					>> XFS_ILOCK_SHIFT)

/*
 * Layouts are broken in the BREAK_WRITE case to ensure that
 * layout-holders do not collide with local writes. Additionally,
 * layouts are broken in the BREAK_UNMAP case to make sure the
 * layout-holder has a consistent view of the file's extent map. While
 * BREAK_WRITE breaks can be satisfied by recalling FL_LAYOUT leases,
 * BREAK_UNMAP breaks additionally require waiting for busy dax-pages to
 * go idle.
 */
enum layout_break_reason {
        BREAK_WRITE,
        BREAK_UNMAP,
};
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

/*
 * For multiple groups support: if S_ISGID bit is set in the parent
 * directory, group of new file is set to that of the parent, and
 * new subdirectory gets S_ISGID bit from parent.
 */
#define XFS_INHERIT_GID(pip)	\
<<<<<<< HEAD
	(((pip)->i_mount->m_flags & XFS_MOUNT_GRPID) || \
	 ((pip)->i_d.di_mode & S_ISGID))

/*
 * xfs_iget.c prototypes.
 */
int		xfs_iget(struct xfs_mount *, struct xfs_trans *, xfs_ino_t,
			 uint, uint, xfs_inode_t **);
=======
	(xfs_has_grpid((pip)->i_mount) || (VFS_I(pip)->i_mode & S_ISGID))

int		xfs_release(struct xfs_inode *ip);
int		xfs_inactive(struct xfs_inode *ip);
int		xfs_lookup(struct xfs_inode *dp, const struct xfs_name *name,
			   struct xfs_inode **ipp, struct xfs_name *ci_name);
int		xfs_create(struct mnt_idmap *idmap,
			   struct xfs_inode *dp, struct xfs_name *name,
			   umode_t mode, dev_t rdev, bool need_xattr,
			   struct xfs_inode **ipp);
int		xfs_create_tmpfile(struct mnt_idmap *idmap,
			   struct xfs_inode *dp, umode_t mode,
			   struct xfs_inode **ipp);
int		xfs_remove(struct xfs_inode *dp, struct xfs_name *name,
			   struct xfs_inode *ip);
int		xfs_link(struct xfs_inode *tdp, struct xfs_inode *sip,
			 struct xfs_name *target_name);
int		xfs_rename(struct mnt_idmap *idmap,
			   struct xfs_inode *src_dp, struct xfs_name *src_name,
			   struct xfs_inode *src_ip, struct xfs_inode *target_dp,
			   struct xfs_name *target_name,
			   struct xfs_inode *target_ip, unsigned int flags);

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
void		xfs_ilock(xfs_inode_t *, uint);
int		xfs_ilock_nowait(xfs_inode_t *, uint);
void		xfs_iunlock(xfs_inode_t *, uint);
void		xfs_ilock_demote(xfs_inode_t *, uint);
<<<<<<< HEAD
int		xfs_isilocked(xfs_inode_t *, uint);
uint		xfs_ilock_map_shared(xfs_inode_t *);
void		xfs_iunlock_map_shared(xfs_inode_t *, uint);
void		xfs_inode_free(struct xfs_inode *ip);

/*
 * xfs_inode.c prototypes.
 */
int		xfs_ialloc(struct xfs_trans *, xfs_inode_t *, umode_t,
			   xfs_nlink_t, xfs_dev_t, prid_t, int,
			   struct xfs_buf **, boolean_t *, xfs_inode_t **);

uint		xfs_ip2xflags(struct xfs_inode *);
uint		xfs_dic2xflags(struct xfs_dinode *);
int		xfs_ifree(struct xfs_trans *, xfs_inode_t *,
			   struct xfs_bmap_free *);
int		xfs_itruncate_extents(struct xfs_trans **, struct xfs_inode *,
				      int, xfs_fsize_t);
int		xfs_iunlink(struct xfs_trans *, xfs_inode_t *);

void		xfs_iext_realloc(xfs_inode_t *, int, int);
void		xfs_iunpin_wait(xfs_inode_t *);
int		xfs_iflush(xfs_inode_t *, uint);
void		xfs_promote_inode(struct xfs_inode *);
void		xfs_lock_inodes(xfs_inode_t **, int, uint);
void		xfs_lock_two_inodes(xfs_inode_t *, xfs_inode_t *, uint);

#define IHOLD(ip) \
do { \
	ASSERT(atomic_read(&VFS_I(ip)->i_count) > 0) ; \
	ihold(VFS_I(ip)); \
	trace_xfs_ihold(ip, _THIS_IP_); \
} while (0)

#define IRELE(ip) \
do { \
	trace_xfs_irele(ip, _THIS_IP_); \
	iput(VFS_I(ip)); \
} while (0)

#endif /* __KERNEL__ */

/*
 * Flags for xfs_iget()
 */
#define XFS_IGET_CREATE		0x1
#define XFS_IGET_UNTRUSTED	0x2
#define XFS_IGET_DONTCACHE	0x4

int		xfs_inotobp(struct xfs_mount *, struct xfs_trans *,
			    xfs_ino_t, struct xfs_dinode **,
			    struct xfs_buf **, int *, uint);
int		xfs_itobp(struct xfs_mount *, struct xfs_trans *,
			  struct xfs_inode *, struct xfs_dinode **,
			  struct xfs_buf **, uint);
int		xfs_iread(struct xfs_mount *, struct xfs_trans *,
			  struct xfs_inode *, uint);
void		xfs_dinode_to_disk(struct xfs_dinode *,
				   struct xfs_icdinode *);
void		xfs_idestroy_fork(struct xfs_inode *, int);
void		xfs_idata_realloc(struct xfs_inode *, int, int);
void		xfs_iroot_realloc(struct xfs_inode *, int, int);
int		xfs_iread_extents(struct xfs_trans *, struct xfs_inode *, int);
int		xfs_iextents_copy(struct xfs_inode *, xfs_bmbt_rec_t *, int);

xfs_bmbt_rec_host_t *xfs_iext_get_ext(xfs_ifork_t *, xfs_extnum_t);
void		xfs_iext_insert(xfs_inode_t *, xfs_extnum_t, xfs_extnum_t,
				xfs_bmbt_irec_t *, int);
void		xfs_iext_add(xfs_ifork_t *, xfs_extnum_t, int);
void		xfs_iext_add_indirect_multi(xfs_ifork_t *, int, xfs_extnum_t, int);
void		xfs_iext_remove(xfs_inode_t *, xfs_extnum_t, int, int);
void		xfs_iext_remove_inline(xfs_ifork_t *, xfs_extnum_t, int);
void		xfs_iext_remove_direct(xfs_ifork_t *, xfs_extnum_t, int);
void		xfs_iext_remove_indirect(xfs_ifork_t *, xfs_extnum_t, int);
void		xfs_iext_realloc_direct(xfs_ifork_t *, int);
void		xfs_iext_direct_to_inline(xfs_ifork_t *, xfs_extnum_t);
void		xfs_iext_inline_to_direct(xfs_ifork_t *, int);
void		xfs_iext_destroy(xfs_ifork_t *);
xfs_bmbt_rec_host_t *xfs_iext_bno_to_ext(xfs_ifork_t *, xfs_fileoff_t, int *);
xfs_ext_irec_t	*xfs_iext_bno_to_irec(xfs_ifork_t *, xfs_fileoff_t, int *);
xfs_ext_irec_t	*xfs_iext_idx_to_irec(xfs_ifork_t *, xfs_extnum_t *, int *, int);
void		xfs_iext_irec_init(xfs_ifork_t *);
xfs_ext_irec_t *xfs_iext_irec_new(xfs_ifork_t *, int);
void		xfs_iext_irec_remove(xfs_ifork_t *, int);
void		xfs_iext_irec_compact(xfs_ifork_t *);
void		xfs_iext_irec_compact_pages(xfs_ifork_t *);
void		xfs_iext_irec_compact_full(xfs_ifork_t *);
void		xfs_iext_irec_update_extoffs(xfs_ifork_t *, int, int);

#define xfs_ipincount(ip)	((unsigned int) atomic_read(&ip->i_pincount))

#if defined(DEBUG)
void		xfs_inobp_check(struct xfs_mount *, struct xfs_buf *);
#else
#define	xfs_inobp_check(mp, bp)
#endif /* DEBUG */

extern struct kmem_zone	*xfs_ifork_zone;
extern struct kmem_zone	*xfs_inode_zone;
extern struct kmem_zone	*xfs_ili_zone;
=======
void		xfs_assert_ilocked(struct xfs_inode *, uint);
uint		xfs_ilock_data_map_shared(struct xfs_inode *);
uint		xfs_ilock_attr_map_shared(struct xfs_inode *);

uint		xfs_ip2xflags(struct xfs_inode *);
int		xfs_ifree(struct xfs_trans *, struct xfs_inode *);
int		xfs_itruncate_extents_flags(struct xfs_trans **,
				struct xfs_inode *, int, xfs_fsize_t, int);
void		xfs_iext_realloc(xfs_inode_t *, int, int);

int		xfs_log_force_inode(struct xfs_inode *ip);
void		xfs_iunpin_wait(xfs_inode_t *);
#define xfs_ipincount(ip)	((unsigned int) atomic_read(&ip->i_pincount))

int		xfs_iflush_cluster(struct xfs_buf *);
void		xfs_lock_two_inodes(struct xfs_inode *ip0, uint ip0_mode,
				struct xfs_inode *ip1, uint ip1_mode);

xfs_extlen_t	xfs_get_extsz_hint(struct xfs_inode *ip);
xfs_extlen_t	xfs_get_cowextsz_hint(struct xfs_inode *ip);

int xfs_init_new_inode(struct mnt_idmap *idmap, struct xfs_trans *tp,
		struct xfs_inode *pip, xfs_ino_t ino, umode_t mode,
		xfs_nlink_t nlink, dev_t rdev, prid_t prid, bool init_xattrs,
		struct xfs_inode **ipp);

static inline int
xfs_itruncate_extents(
	struct xfs_trans	**tpp,
	struct xfs_inode	*ip,
	int			whichfork,
	xfs_fsize_t		new_size)
{
	return xfs_itruncate_extents_flags(tpp, ip, whichfork, new_size, 0);
}

/* from xfs_file.c */
int	xfs_break_dax_layouts(struct inode *inode, bool *retry);
int	xfs_break_layouts(struct inode *inode, uint *iolock,
		enum layout_break_reason reason);

/* from xfs_iops.c */
extern void xfs_setup_inode(struct xfs_inode *ip);
extern void xfs_setup_iops(struct xfs_inode *ip);
extern void xfs_diflags_to_iflags(struct xfs_inode *ip, bool init);

static inline void xfs_update_stable_writes(struct xfs_inode *ip)
{
	if (bdev_stable_writes(xfs_inode_buftarg(ip)->bt_bdev))
		mapping_set_stable_writes(VFS_I(ip)->i_mapping);
	else
		mapping_clear_stable_writes(VFS_I(ip)->i_mapping);
}

/*
 * When setting up a newly allocated inode, we need to call
 * xfs_finish_inode_setup() once the inode is fully instantiated at
 * the VFS level to prevent the rest of the world seeing the inode
 * before we've completed instantiation. Otherwise we can do it
 * the moment the inode lookup is complete.
 */
static inline void xfs_finish_inode_setup(struct xfs_inode *ip)
{
	xfs_iflags_clear(ip, XFS_INEW);
	barrier();
	unlock_new_inode(VFS_I(ip));
}

static inline void xfs_setup_existing_inode(struct xfs_inode *ip)
{
	xfs_setup_inode(ip);
	xfs_setup_iops(ip);
	xfs_finish_inode_setup(ip);
}

void xfs_irele(struct xfs_inode *ip);

extern struct kmem_cache	*xfs_inode_cache;

/* The default CoW extent size hint. */
#define XFS_DEFAULT_COWEXTSZ_HINT 32

bool xfs_inode_needs_inactive(struct xfs_inode *ip);

void xfs_end_io(struct work_struct *work);

int xfs_ilock2_io_mmap(struct xfs_inode *ip1, struct xfs_inode *ip2);
void xfs_iunlock2_io_mmap(struct xfs_inode *ip1, struct xfs_inode *ip2);
void xfs_iunlock2_remapping(struct xfs_inode *ip1, struct xfs_inode *ip2);

static inline bool
xfs_inode_unlinked_incomplete(
	struct xfs_inode	*ip)
{
	return VFS_I(ip)->i_nlink == 0 && !xfs_inode_on_unlinked_list(ip);
}
int xfs_inode_reload_unlinked_bucket(struct xfs_trans *tp, struct xfs_inode *ip);
int xfs_inode_reload_unlinked(struct xfs_inode *ip);

bool xfs_ifork_zapped(const struct xfs_inode *ip, int whichfork);
void xfs_inode_count_blocks(struct xfs_trans *tp, struct xfs_inode *ip,
		xfs_filblks_t *dblocks, xfs_filblks_t *rblocks);

struct xfs_dir_update_params {
	const struct xfs_inode	*dp;
	const struct xfs_inode	*ip;
	const struct xfs_name	*name;
	int			delta;
};

#ifdef CONFIG_XFS_LIVE_HOOKS
void xfs_dir_update_hook(struct xfs_inode *dp, struct xfs_inode *ip,
		int delta, const struct xfs_name *name);

struct xfs_dir_hook {
	struct xfs_hook		dirent_hook;
};

void xfs_dir_hook_disable(void);
void xfs_dir_hook_enable(void);

int xfs_dir_hook_add(struct xfs_mount *mp, struct xfs_dir_hook *hook);
void xfs_dir_hook_del(struct xfs_mount *mp, struct xfs_dir_hook *hook);
void xfs_dir_hook_setup(struct xfs_dir_hook *hook, notifier_fn_t mod_fn);
#else
# define xfs_dir_update_hook(dp, ip, delta, name)	((void)0)
#endif /* CONFIG_XFS_LIVE_HOOKS */
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

#endif	/* __XFS_INODE_H__ */
