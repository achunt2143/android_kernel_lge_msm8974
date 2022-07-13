<<<<<<< HEAD
/*
 * Copyright (c) 2000,2005 Silicon Graphics, Inc.
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
 * Copyright (c) 2000,2005 Silicon Graphics, Inc.
 * All Rights Reserved.
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 */
#ifndef	__XFS_INODE_ITEM_H__
#define	__XFS_INODE_ITEM_H__

<<<<<<< HEAD
/*
 * This is the structure used to lay out an inode log item in the
 * log.  The size of the inline data/extents/b-tree root to be logged
 * (if any) is indicated in the ilf_dsize field.  Changes to this structure
 * must be added on to the end.
 */
typedef struct xfs_inode_log_format {
	__uint16_t		ilf_type;	/* inode log item type */
	__uint16_t		ilf_size;	/* size of this item */
	__uint32_t		ilf_fields;	/* flags for fields logged */
	__uint16_t		ilf_asize;	/* size of attr d/ext/root */
	__uint16_t		ilf_dsize;	/* size of data/ext/root */
	__uint64_t		ilf_ino;	/* inode number */
	union {
		__uint32_t	ilfu_rdev;	/* rdev value for dev inode*/
		uuid_t		ilfu_uuid;	/* mount point value */
	} ilf_u;
	__int64_t		ilf_blkno;	/* blkno of inode buffer */
	__int32_t		ilf_len;	/* len of inode buffer */
	__int32_t		ilf_boffset;	/* off of inode in buffer */
} xfs_inode_log_format_t;

typedef struct xfs_inode_log_format_32 {
	__uint16_t		ilf_type;	/* inode log item type */
	__uint16_t		ilf_size;	/* size of this item */
	__uint32_t		ilf_fields;	/* flags for fields logged */
	__uint16_t		ilf_asize;	/* size of attr d/ext/root */
	__uint16_t		ilf_dsize;	/* size of data/ext/root */
	__uint64_t		ilf_ino;	/* inode number */
	union {
		__uint32_t	ilfu_rdev;	/* rdev value for dev inode*/
		uuid_t		ilfu_uuid;	/* mount point value */
	} ilf_u;
	__int64_t		ilf_blkno;	/* blkno of inode buffer */
	__int32_t		ilf_len;	/* len of inode buffer */
	__int32_t		ilf_boffset;	/* off of inode in buffer */
} __attribute__((packed)) xfs_inode_log_format_32_t;

typedef struct xfs_inode_log_format_64 {
	__uint16_t		ilf_type;	/* inode log item type */
	__uint16_t		ilf_size;	/* size of this item */
	__uint32_t		ilf_fields;	/* flags for fields logged */
	__uint16_t		ilf_asize;	/* size of attr d/ext/root */
	__uint16_t		ilf_dsize;	/* size of data/ext/root */
	__uint32_t		ilf_pad;	/* pad for 64 bit boundary */
	__uint64_t		ilf_ino;	/* inode number */
	union {
		__uint32_t	ilfu_rdev;	/* rdev value for dev inode*/
		uuid_t		ilfu_uuid;	/* mount point value */
	} ilf_u;
	__int64_t		ilf_blkno;	/* blkno of inode buffer */
	__int32_t		ilf_len;	/* len of inode buffer */
	__int32_t		ilf_boffset;	/* off of inode in buffer */
} xfs_inode_log_format_64_t;

/*
 * Flags for xfs_trans_log_inode flags field.
 */
#define	XFS_ILOG_CORE	0x001	/* log standard inode fields */
#define	XFS_ILOG_DDATA	0x002	/* log i_df.if_data */
#define	XFS_ILOG_DEXT	0x004	/* log i_df.if_extents */
#define	XFS_ILOG_DBROOT	0x008	/* log i_df.i_broot */
#define	XFS_ILOG_DEV	0x010	/* log the dev field */
#define	XFS_ILOG_UUID	0x020	/* log the uuid field */
#define	XFS_ILOG_ADATA	0x040	/* log i_af.if_data */
#define	XFS_ILOG_AEXT	0x080	/* log i_af.if_extents */
#define	XFS_ILOG_ABROOT	0x100	/* log i_af.i_broot */


/*
 * The timestamps are dirty, but not necessarily anything else in the inode
 * core.  Unlike the other fields above this one must never make it to disk
 * in the ilf_fields of the inode_log_format, but is purely store in-memory in
 * ili_fields in the inode_log_item.
 */
#define XFS_ILOG_TIMESTAMP	0x4000

#define	XFS_ILOG_NONCORE	(XFS_ILOG_DDATA | XFS_ILOG_DEXT | \
				 XFS_ILOG_DBROOT | XFS_ILOG_DEV | \
				 XFS_ILOG_UUID | XFS_ILOG_ADATA | \
				 XFS_ILOG_AEXT | XFS_ILOG_ABROOT)

#define	XFS_ILOG_DFORK		(XFS_ILOG_DDATA | XFS_ILOG_DEXT | \
				 XFS_ILOG_DBROOT)

#define	XFS_ILOG_AFORK		(XFS_ILOG_ADATA | XFS_ILOG_AEXT | \
				 XFS_ILOG_ABROOT)

#define	XFS_ILOG_ALL		(XFS_ILOG_CORE | XFS_ILOG_DDATA | \
				 XFS_ILOG_DEXT | XFS_ILOG_DBROOT | \
				 XFS_ILOG_DEV | XFS_ILOG_UUID | \
				 XFS_ILOG_ADATA | XFS_ILOG_AEXT | \
				 XFS_ILOG_ABROOT | XFS_ILOG_TIMESTAMP)

static inline int xfs_ilog_fbroot(int w)
{
	return (w == XFS_DATA_FORK ? XFS_ILOG_DBROOT : XFS_ILOG_ABROOT);
}

static inline int xfs_ilog_fext(int w)
{
	return (w == XFS_DATA_FORK ? XFS_ILOG_DEXT : XFS_ILOG_AEXT);
}

static inline int xfs_ilog_fdata(int w)
{
	return (w == XFS_DATA_FORK ? XFS_ILOG_DDATA : XFS_ILOG_ADATA);
}

#ifdef __KERNEL__
=======
/* kernel only definitions */
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

struct xfs_buf;
struct xfs_bmbt_rec;
struct xfs_inode;
struct xfs_mount;

<<<<<<< HEAD

typedef struct xfs_inode_log_item {
	xfs_log_item_t		ili_item;	   /* common portion */
	struct xfs_inode	*ili_inode;	   /* inode ptr */
	xfs_lsn_t		ili_flush_lsn;	   /* lsn at last flush */
	xfs_lsn_t		ili_last_lsn;	   /* lsn at last transaction */
	unsigned short		ili_lock_flags;	   /* lock flags */
	unsigned short		ili_logged;	   /* flushed logged data */
	unsigned int		ili_last_fields;   /* fields when flushed */
	unsigned int		ili_fields;	   /* fields to be logged */
	struct xfs_bmbt_rec	*ili_extents_buf;  /* array of logged
						      data exts */
	struct xfs_bmbt_rec	*ili_aextents_buf; /* array of logged
						      attr exts */
#ifdef XFS_TRANS_DEBUG
	int			ili_root_size;
	char			*ili_orig_root;
#endif
	xfs_inode_log_format_t	ili_format;	   /* logged structure */
} xfs_inode_log_item_t;


static inline int xfs_inode_clean(xfs_inode_t *ip)
=======
struct xfs_inode_log_item {
	struct xfs_log_item	ili_item;	   /* common portion */
	struct xfs_inode	*ili_inode;	   /* inode ptr */
	unsigned short		ili_lock_flags;	   /* inode lock flags */
	unsigned int		ili_dirty_flags;   /* dirty in current tx */
	/*
	 * The ili_lock protects the interactions between the dirty state and
	 * the flush state of the inode log item. This allows us to do atomic
	 * modifications of multiple state fields without having to hold a
	 * specific inode lock to serialise them.
	 *
	 * We need atomic changes between inode dirtying, inode flushing and
	 * inode completion, but these all hold different combinations of
	 * ILOCK and IFLUSHING and hence we need some other method of
	 * serialising updates to the flush state.
	 */
	spinlock_t		ili_lock;	   /* flush state lock */
	unsigned int		ili_last_fields;   /* fields when flushed */
	unsigned int		ili_fields;	   /* fields to be logged */
	unsigned int		ili_fsync_fields;  /* logged since last fsync */
	xfs_lsn_t		ili_flush_lsn;	   /* lsn at last flush */
	xfs_csn_t		ili_commit_seq;	   /* last transaction commit */
};

static inline int xfs_inode_clean(struct xfs_inode *ip)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
{
	return !ip->i_itemp || !(ip->i_itemp->ili_fields & XFS_ILOG_ALL);
}

extern void xfs_inode_item_init(struct xfs_inode *, struct xfs_mount *);
extern void xfs_inode_item_destroy(struct xfs_inode *);
<<<<<<< HEAD
extern void xfs_iflush_done(struct xfs_buf *, struct xfs_log_item *);
extern void xfs_istale_done(struct xfs_buf *, struct xfs_log_item *);
extern void xfs_iflush_abort(struct xfs_inode *);
extern int xfs_inode_item_format_convert(xfs_log_iovec_t *,
					 xfs_inode_log_format_t *);

#endif	/* __KERNEL__ */
=======
extern void xfs_iflush_abort(struct xfs_inode *);
extern void xfs_iflush_shutdown_abort(struct xfs_inode *);
extern int xfs_inode_item_format_convert(xfs_log_iovec_t *,
					 struct xfs_inode_log_format *);

extern struct kmem_cache	*xfs_ili_cache;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

#endif	/* __XFS_INODE_ITEM_H__ */
