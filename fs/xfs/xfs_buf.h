<<<<<<< HEAD
/*
 * Copyright (c) 2000-2005 Silicon Graphics, Inc.
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
 * Copyright (c) 2000-2005 Silicon Graphics, Inc.
 * All Rights Reserved.
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 */
#ifndef __XFS_BUF_H__
#define __XFS_BUF_H__

#include <linux/list.h>
#include <linux/types.h>
#include <linux/spinlock.h>
#include <linux/mm.h>
#include <linux/fs.h>
<<<<<<< HEAD
#include <linux/buffer_head.h>
#include <linux/uio.h>
=======
#include <linux/dax.h>
#include <linux/uio.h>
#include <linux/list_lru.h>

extern struct kmem_cache *xfs_buf_cache;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

/*
 *	Base types
 */
<<<<<<< HEAD

#define XFS_BUF_DADDR_NULL	((xfs_daddr_t) (-1LL))

#define xfs_buf_ctob(pp)	((pp) * PAGE_CACHE_SIZE)
#define xfs_buf_btoc(dd)	(((dd) + PAGE_CACHE_SIZE-1) >> PAGE_CACHE_SHIFT)
#define xfs_buf_btoct(dd)	((dd) >> PAGE_CACHE_SHIFT)
#define xfs_buf_poff(aa)	((aa) & ~PAGE_CACHE_MASK)

typedef enum {
	XBRW_READ = 1,			/* transfer into target memory */
	XBRW_WRITE = 2,			/* transfer from target memory */
	XBRW_ZERO = 3,			/* Zero target memory */
} xfs_buf_rw_t;

#define XBF_READ	(1 << 0) /* buffer intended for reading from device */
#define XBF_WRITE	(1 << 1) /* buffer intended for writing to device */
#define XBF_READ_AHEAD	(1 << 2) /* asynchronous read-ahead */
#define XBF_MAPPED	(1 << 3) /* buffer mapped (b_addr valid) */
#define XBF_ASYNC	(1 << 4) /* initiator will not wait for completion */
#define XBF_DONE	(1 << 5) /* all pages in the buffer uptodate */
#define XBF_DELWRI	(1 << 6) /* buffer has dirty pages */
#define XBF_STALE	(1 << 7) /* buffer has been staled, do not find it */

/* I/O hints for the BIO layer */
#define XBF_SYNCIO	(1 << 10)/* treat this buffer as synchronous I/O */
#define XBF_FUA		(1 << 11)/* force cache write through mode */
#define XBF_FLUSH	(1 << 12)/* flush the disk cache before a write */

/* flags used only as arguments to access routines */
#define XBF_LOCK	(1 << 15)/* lock requested */
#define XBF_TRYLOCK	(1 << 16)/* lock requested, but do not wait */
#define XBF_DONT_BLOCK	(1 << 17)/* do not block in current thread */

/* flags used only internally */
#define _XBF_PAGES	(1 << 20)/* backed by refcounted pages */
#define _XBF_KMEM	(1 << 21)/* backed by heap memory */
#define _XBF_DELWRI_Q	(1 << 22)/* buffer on delwri queue */
=======
struct xfs_buf;

#define XFS_BUF_DADDR_NULL	((xfs_daddr_t) (-1LL))

#define XBF_READ	 (1u << 0) /* buffer intended for reading from device */
#define XBF_WRITE	 (1u << 1) /* buffer intended for writing to device */
#define XBF_READ_AHEAD	 (1u << 2) /* asynchronous read-ahead */
#define XBF_NO_IOACCT	 (1u << 3) /* bypass I/O accounting (non-LRU bufs) */
#define XBF_ASYNC	 (1u << 4) /* initiator will not wait for completion */
#define XBF_DONE	 (1u << 5) /* all pages in the buffer uptodate */
#define XBF_STALE	 (1u << 6) /* buffer has been staled, do not find it */
#define XBF_WRITE_FAIL	 (1u << 7) /* async writes have failed on this buffer */

/* buffer type flags for write callbacks */
#define _XBF_INODES	 (1u << 16)/* inode buffer */
#define _XBF_DQUOTS	 (1u << 17)/* dquot buffer */
#define _XBF_LOGRECOVERY (1u << 18)/* log recovery buffer */

/* flags used only internally */
#define _XBF_PAGES	 (1u << 20)/* backed by refcounted pages */
#define _XBF_KMEM	 (1u << 21)/* backed by heap memory */
#define _XBF_DELWRI_Q	 (1u << 22)/* buffer on a delwri queue */

/* flags used only as arguments to access routines */
/*
 * Online fsck is scanning the buffer cache for live buffers.  Do not warn
 * about length mismatches during lookups and do not return stale buffers.
 */
#define XBF_LIVESCAN	 (1u << 28)
#define XBF_INCORE	 (1u << 29)/* lookup only, return if found in cache */
#define XBF_TRYLOCK	 (1u << 30)/* lock requested, but do not wait */
#define XBF_UNMAPPED	 (1u << 31)/* do not map the buffer */

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

typedef unsigned int xfs_buf_flags_t;

#define XFS_BUF_FLAGS \
	{ XBF_READ,		"READ" }, \
	{ XBF_WRITE,		"WRITE" }, \
	{ XBF_READ_AHEAD,	"READ_AHEAD" }, \
<<<<<<< HEAD
	{ XBF_MAPPED,		"MAPPED" }, \
	{ XBF_ASYNC,		"ASYNC" }, \
	{ XBF_DONE,		"DONE" }, \
	{ XBF_DELWRI,		"DELWRI" }, \
	{ XBF_STALE,		"STALE" }, \
	{ XBF_SYNCIO,		"SYNCIO" }, \
	{ XBF_FUA,		"FUA" }, \
	{ XBF_FLUSH,		"FLUSH" }, \
	{ XBF_LOCK,		"LOCK" },  	/* should never be set */\
	{ XBF_TRYLOCK,		"TRYLOCK" }, 	/* ditto */\
	{ XBF_DONT_BLOCK,	"DONT_BLOCK" },	/* ditto */\
	{ _XBF_PAGES,		"PAGES" }, \
	{ _XBF_KMEM,		"KMEM" }, \
	{ _XBF_DELWRI_Q,	"DELWRI_Q" }

typedef enum {
	XBT_FORCE_FLUSH = 0,
} xfs_buftarg_flags_t;

typedef struct xfs_buftarg {
	dev_t			bt_dev;
	struct block_device	*bt_bdev;
	struct backing_dev_info	*bt_bdi;
	struct xfs_mount	*bt_mount;
	unsigned int		bt_bsize;
	unsigned int		bt_sshift;
	size_t			bt_smask;

	/* per device delwri queue */
	struct task_struct	*bt_task;
	struct list_head	bt_delwri_queue;
	spinlock_t		bt_delwri_lock;
	unsigned long		bt_flags;

	/* LRU control structures */
	struct shrinker		bt_shrinker;
	struct list_head	bt_lru;
	spinlock_t		bt_lru_lock;
	unsigned int		bt_lru_nr;
} xfs_buftarg_t;

struct xfs_buf;
typedef void (*xfs_buf_iodone_t)(struct xfs_buf *);

#define XB_PAGES	2

typedef struct xfs_buf {
=======
	{ XBF_NO_IOACCT,	"NO_IOACCT" }, \
	{ XBF_ASYNC,		"ASYNC" }, \
	{ XBF_DONE,		"DONE" }, \
	{ XBF_STALE,		"STALE" }, \
	{ XBF_WRITE_FAIL,	"WRITE_FAIL" }, \
	{ _XBF_INODES,		"INODES" }, \
	{ _XBF_DQUOTS,		"DQUOTS" }, \
	{ _XBF_LOGRECOVERY,	"LOG_RECOVERY" }, \
	{ _XBF_PAGES,		"PAGES" }, \
	{ _XBF_KMEM,		"KMEM" }, \
	{ _XBF_DELWRI_Q,	"DELWRI_Q" }, \
	/* The following interface flags should never be set */ \
	{ XBF_LIVESCAN,		"LIVESCAN" }, \
	{ XBF_INCORE,		"INCORE" }, \
	{ XBF_TRYLOCK,		"TRYLOCK" }, \
	{ XBF_UNMAPPED,		"UNMAPPED" }

/*
 * Internal state flags.
 */
#define XFS_BSTATE_DISPOSE	 (1 << 0)	/* buffer being discarded */
#define XFS_BSTATE_IN_FLIGHT	 (1 << 1)	/* I/O in flight */

struct xfs_buf_cache {
	spinlock_t		bc_lock;
	struct rhashtable	bc_hash;
};

int xfs_buf_cache_init(struct xfs_buf_cache *bch);
void xfs_buf_cache_destroy(struct xfs_buf_cache *bch);

/*
 * The xfs_buftarg contains 2 notions of "sector size" -
 *
 * 1) The metadata sector size, which is the minimum unit and
 *    alignment of IO which will be performed by metadata operations.
 * 2) The device logical sector size
 *
 * The first is specified at mkfs time, and is stored on-disk in the
 * superblock's sb_sectsize.
 *
 * The latter is derived from the underlying device, and controls direct IO
 * alignment constraints.
 */
struct xfs_buftarg {
	dev_t			bt_dev;
	struct file		*bt_bdev_file;
	struct block_device	*bt_bdev;
	struct dax_device	*bt_daxdev;
	struct file		*bt_file;
	u64			bt_dax_part_off;
	struct xfs_mount	*bt_mount;
	unsigned int		bt_meta_sectorsize;
	size_t			bt_meta_sectormask;
	size_t			bt_logical_sectorsize;
	size_t			bt_logical_sectormask;

	/* LRU control structures */
	struct shrinker		*bt_shrinker;
	struct list_lru		bt_lru;

	struct percpu_counter	bt_io_count;
	struct ratelimit_state	bt_ioerror_rl;

	/* built-in cache, if we're not using the perag one */
	struct xfs_buf_cache	bt_cache[];
};

#define XB_PAGES	2

struct xfs_buf_map {
	xfs_daddr_t		bm_bn;	/* block number for I/O */
	int			bm_len;	/* size of I/O */
	unsigned int		bm_flags;
};

/*
 * Online fsck is scanning the buffer cache for live buffers.  Do not warn
 * about length mismatches during lookups and do not return stale buffers.
 */
#define XBM_LIVESCAN		(1U << 0)

#define DEFINE_SINGLE_BUF_MAP(map, blkno, numblk) \
	struct xfs_buf_map (map) = { .bm_bn = (blkno), .bm_len = (numblk) };

struct xfs_buf_ops {
	char *name;
	union {
		__be32 magic[2];	/* v4 and v5 on disk magic values */
		__be16 magic16[2];	/* v4 and v5 on disk magic values */
	};
	void (*verify_read)(struct xfs_buf *);
	void (*verify_write)(struct xfs_buf *);
	xfs_failaddr_t (*verify_struct)(struct xfs_buf *bp);
};

struct xfs_buf {
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	/*
	 * first cacheline holds all the fields needed for an uncontended cache
	 * hit to be fully processed. The semaphore straddles the cacheline
	 * boundary, but the counter and lock sits on the first cacheline,
	 * which is the only bit that is touched if we hit the semaphore
	 * fast-path on locking.
	 */
<<<<<<< HEAD
	struct rb_node		b_rbnode;	/* rbtree node */
	xfs_off_t		b_file_offset;	/* offset in file */
	size_t			b_buffer_length;/* size of buffer in bytes */
=======
	struct rhash_head	b_rhash_head;	/* pag buffer hash node */

	xfs_daddr_t		b_rhash_key;	/* buffer cache index */
	int			b_length;	/* size of buffer in BBs */
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	atomic_t		b_hold;		/* reference count */
	atomic_t		b_lru_ref;	/* lru reclaim ref count */
	xfs_buf_flags_t		b_flags;	/* status flags */
	struct semaphore	b_sema;		/* semaphore for lockables */

<<<<<<< HEAD
	struct list_head	b_lru;		/* lru list */
	wait_queue_head_t	b_waiters;	/* unpin waiters */
	struct list_head	b_list;
	struct xfs_perag	*b_pag;		/* contains rbtree root */
	xfs_buftarg_t		*b_target;	/* buffer target (device) */
	xfs_daddr_t		b_bn;		/* block number for I/O */
	size_t			b_count_desired;/* desired transfer size */
	void			*b_addr;	/* virtual address of buffer */
	struct work_struct	b_iodone_work;
	xfs_buf_iodone_t	b_iodone;	/* I/O completion function */
	struct completion	b_iowait;	/* queue for I/O waiters */
	void			*b_fspriv;
	struct xfs_trans	*b_transp;
	struct page		**b_pages;	/* array of page pointers */
	struct page		*b_page_array[XB_PAGES]; /* inline pages */
	unsigned long		b_queuetime;	/* time buffer was queued */
	atomic_t		b_pin_count;	/* pin count */
	atomic_t		b_io_remaining;	/* #outstanding I/O requests */
	unsigned int		b_page_count;	/* size of page array */
	unsigned int		b_offset;	/* page offset in first page */
	unsigned short		b_error;	/* error code on I/O */
#ifdef XFS_BUF_LOCK_TRACKING
	int			b_last_holder;
#endif
} xfs_buf_t;


/* Finding and Reading Buffers */
extern xfs_buf_t *_xfs_buf_find(xfs_buftarg_t *, xfs_off_t, size_t,
				xfs_buf_flags_t, xfs_buf_t *);
#define xfs_incore(buftarg,blkno,len,lockit) \
	_xfs_buf_find(buftarg, blkno ,len, lockit, NULL)

extern xfs_buf_t *xfs_buf_get(xfs_buftarg_t *, xfs_off_t, size_t,
				xfs_buf_flags_t);
extern xfs_buf_t *xfs_buf_read(xfs_buftarg_t *, xfs_off_t, size_t,
				xfs_buf_flags_t);

struct xfs_buf *xfs_buf_alloc(struct xfs_buftarg *, xfs_off_t, size_t,
			      xfs_buf_flags_t);
extern void xfs_buf_set_empty(struct xfs_buf *bp, size_t len);
extern xfs_buf_t *xfs_buf_get_uncached(struct xfs_buftarg *, size_t, int);
extern int xfs_buf_associate_memory(xfs_buf_t *, void *, size_t);
extern void xfs_buf_hold(xfs_buf_t *);
extern void xfs_buf_readahead(xfs_buftarg_t *, xfs_off_t, size_t);
struct xfs_buf *xfs_buf_read_uncached(struct xfs_mount *mp,
				struct xfs_buftarg *target,
				xfs_daddr_t daddr, size_t length, int flags);

/* Releasing Buffers */
extern void xfs_buf_free(xfs_buf_t *);
extern void xfs_buf_rele(xfs_buf_t *);

/* Locking and Unlocking Buffers */
extern int xfs_buf_trylock(xfs_buf_t *);
extern void xfs_buf_lock(xfs_buf_t *);
extern void xfs_buf_unlock(xfs_buf_t *);
#define xfs_buf_islocked(bp) \
	((bp)->b_sema.count <= 0)

/* Buffer Read and Write Routines */
extern int xfs_bwrite(struct xfs_buf *bp);

extern void xfsbdstrat(struct xfs_mount *, struct xfs_buf *);
extern int xfs_bdstrat_cb(struct xfs_buf *);

extern void xfs_buf_ioend(xfs_buf_t *,	int);
extern void xfs_buf_ioerror(xfs_buf_t *, int);
extern void xfs_buf_ioerror_alert(struct xfs_buf *, const char *func);
extern int xfs_buf_iorequest(xfs_buf_t *);
extern int xfs_buf_iowait(xfs_buf_t *);
extern void xfs_buf_iomove(xfs_buf_t *, size_t, size_t, void *,
				xfs_buf_rw_t);
#define xfs_buf_zero(bp, off, len) \
	    xfs_buf_iomove((bp), (off), (len), NULL, XBRW_ZERO)

static inline int xfs_buf_geterror(xfs_buf_t *bp)
{
	return bp ? bp->b_error : ENOMEM;
}

/* Buffer Utility Routines */
extern xfs_caddr_t xfs_buf_offset(xfs_buf_t *, size_t);

/* Delayed Write Buffer Routines */
extern void xfs_buf_delwri_queue(struct xfs_buf *);
extern void xfs_buf_delwri_dequeue(struct xfs_buf *);
extern void xfs_buf_delwri_promote(struct xfs_buf *);

/* Buffer Daemon Setup Routines */
extern int xfs_buf_init(void);
extern void xfs_buf_terminate(void);

#define XFS_BUF_ZEROFLAGS(bp) \
	((bp)->b_flags &= ~(XBF_READ|XBF_WRITE|XBF_ASYNC|XBF_DELWRI| \
			    XBF_SYNCIO|XBF_FUA|XBF_FLUSH))

void xfs_buf_stale(struct xfs_buf *bp);
#define XFS_BUF_UNSTALE(bp)	((bp)->b_flags &= ~XBF_STALE)
#define XFS_BUF_ISSTALE(bp)	((bp)->b_flags & XBF_STALE)

#define XFS_BUF_ISDELAYWRITE(bp)	((bp)->b_flags & XBF_DELWRI)

#define XFS_BUF_DONE(bp)	((bp)->b_flags |= XBF_DONE)
#define XFS_BUF_UNDONE(bp)	((bp)->b_flags &= ~XBF_DONE)
#define XFS_BUF_ISDONE(bp)	((bp)->b_flags & XBF_DONE)

#define XFS_BUF_ASYNC(bp)	((bp)->b_flags |= XBF_ASYNC)
#define XFS_BUF_UNASYNC(bp)	((bp)->b_flags &= ~XBF_ASYNC)
#define XFS_BUF_ISASYNC(bp)	((bp)->b_flags & XBF_ASYNC)

#define XFS_BUF_READ(bp)	((bp)->b_flags |= XBF_READ)
#define XFS_BUF_UNREAD(bp)	((bp)->b_flags &= ~XBF_READ)
#define XFS_BUF_ISREAD(bp)	((bp)->b_flags & XBF_READ)

#define XFS_BUF_WRITE(bp)	((bp)->b_flags |= XBF_WRITE)
#define XFS_BUF_UNWRITE(bp)	((bp)->b_flags &= ~XBF_WRITE)
#define XFS_BUF_ISWRITE(bp)	((bp)->b_flags & XBF_WRITE)

#define XFS_BUF_ADDR(bp)		((bp)->b_bn)
#define XFS_BUF_SET_ADDR(bp, bno)	((bp)->b_bn = (xfs_daddr_t)(bno))
#define XFS_BUF_OFFSET(bp)		((bp)->b_file_offset)
#define XFS_BUF_SET_OFFSET(bp, off)	((bp)->b_file_offset = (off))
#define XFS_BUF_COUNT(bp)		((bp)->b_count_desired)
#define XFS_BUF_SET_COUNT(bp, cnt)	((bp)->b_count_desired = (cnt))
#define XFS_BUF_SIZE(bp)		((bp)->b_buffer_length)
#define XFS_BUF_SET_SIZE(bp, cnt)	((bp)->b_buffer_length = (cnt))

static inline void xfs_buf_set_ref(struct xfs_buf *bp, int lru_ref)
{
	atomic_set(&bp->b_lru_ref, lru_ref);
=======
	/*
	 * concurrent access to b_lru and b_lru_flags are protected by
	 * bt_lru_lock and not by b_sema
	 */
	struct list_head	b_lru;		/* lru list */
	spinlock_t		b_lock;		/* internal state lock */
	unsigned int		b_state;	/* internal state flags */
	int			b_io_error;	/* internal IO error state */
	wait_queue_head_t	b_waiters;	/* unpin waiters */
	struct list_head	b_list;
	struct xfs_perag	*b_pag;		/* contains rbtree root */
	struct xfs_mount	*b_mount;
	struct xfs_buftarg	*b_target;	/* buffer target (device) */
	void			*b_addr;	/* virtual address of buffer */
	struct work_struct	b_ioend_work;
	struct completion	b_iowait;	/* queue for I/O waiters */
	struct xfs_buf_log_item	*b_log_item;
	struct list_head	b_li_list;	/* Log items list head */
	struct xfs_trans	*b_transp;
	struct page		**b_pages;	/* array of page pointers */
	struct page		*b_page_array[XB_PAGES]; /* inline pages */
	struct xfs_buf_map	*b_maps;	/* compound buffer map */
	struct xfs_buf_map	__b_map;	/* inline compound buffer map */
	int			b_map_count;
	atomic_t		b_pin_count;	/* pin count */
	atomic_t		b_io_remaining;	/* #outstanding I/O requests */
	unsigned int		b_page_count;	/* size of page array */
	unsigned int		b_offset;	/* page offset of b_addr,
						   only for _XBF_KMEM buffers */
	int			b_error;	/* error code on I/O */

	/*
	 * async write failure retry count. Initialised to zero on the first
	 * failure, then when it exceeds the maximum configured without a
	 * success the write is considered to be failed permanently and the
	 * iodone handler will take appropriate action.
	 *
	 * For retry timeouts, we record the jiffie of the first failure. This
	 * means that we can change the retry timeout for buffers already under
	 * I/O and thus avoid getting stuck in a retry loop with a long timeout.
	 *
	 * last_error is used to ensure that we are getting repeated errors, not
	 * different errors. e.g. a block device might change ENOSPC to EIO when
	 * a failure timeout occurs, so we want to re-initialise the error
	 * retry behaviour appropriately when that happens.
	 */
	int			b_retries;
	unsigned long		b_first_retry_time; /* in jiffies */
	int			b_last_error;

	const struct xfs_buf_ops	*b_ops;
	struct rcu_head		b_rcu;
};

/* Finding and Reading Buffers */
int xfs_buf_get_map(struct xfs_buftarg *target, struct xfs_buf_map *map,
		int nmaps, xfs_buf_flags_t flags, struct xfs_buf **bpp);
int xfs_buf_read_map(struct xfs_buftarg *target, struct xfs_buf_map *map,
		int nmaps, xfs_buf_flags_t flags, struct xfs_buf **bpp,
		const struct xfs_buf_ops *ops, xfs_failaddr_t fa);
void xfs_buf_readahead_map(struct xfs_buftarg *target,
			       struct xfs_buf_map *map, int nmaps,
			       const struct xfs_buf_ops *ops);

static inline int
xfs_buf_incore(
	struct xfs_buftarg	*target,
	xfs_daddr_t		blkno,
	size_t			numblks,
	xfs_buf_flags_t		flags,
	struct xfs_buf		**bpp)
{
	DEFINE_SINGLE_BUF_MAP(map, blkno, numblks);

	return xfs_buf_get_map(target, &map, 1, XBF_INCORE | flags, bpp);
}

static inline int
xfs_buf_get(
	struct xfs_buftarg	*target,
	xfs_daddr_t		blkno,
	size_t			numblks,
	struct xfs_buf		**bpp)
{
	DEFINE_SINGLE_BUF_MAP(map, blkno, numblks);

	return xfs_buf_get_map(target, &map, 1, 0, bpp);
}

static inline int
xfs_buf_read(
	struct xfs_buftarg	*target,
	xfs_daddr_t		blkno,
	size_t			numblks,
	xfs_buf_flags_t		flags,
	struct xfs_buf		**bpp,
	const struct xfs_buf_ops *ops)
{
	DEFINE_SINGLE_BUF_MAP(map, blkno, numblks);

	return xfs_buf_read_map(target, &map, 1, flags, bpp, ops,
			__builtin_return_address(0));
}

static inline void
xfs_buf_readahead(
	struct xfs_buftarg	*target,
	xfs_daddr_t		blkno,
	size_t			numblks,
	const struct xfs_buf_ops *ops)
{
	DEFINE_SINGLE_BUF_MAP(map, blkno, numblks);
	return xfs_buf_readahead_map(target, &map, 1, ops);
}

int xfs_buf_get_uncached(struct xfs_buftarg *target, size_t numblks,
		xfs_buf_flags_t flags, struct xfs_buf **bpp);
int xfs_buf_read_uncached(struct xfs_buftarg *target, xfs_daddr_t daddr,
		size_t numblks, xfs_buf_flags_t flags, struct xfs_buf **bpp,
		const struct xfs_buf_ops *ops);
int _xfs_buf_read(struct xfs_buf *bp, xfs_buf_flags_t flags);
void xfs_buf_hold(struct xfs_buf *bp);

/* Releasing Buffers */
extern void xfs_buf_rele(struct xfs_buf *);

/* Locking and Unlocking Buffers */
extern int xfs_buf_trylock(struct xfs_buf *);
extern void xfs_buf_lock(struct xfs_buf *);
extern void xfs_buf_unlock(struct xfs_buf *);
#define xfs_buf_islocked(bp) \
	((bp)->b_sema.count <= 0)

static inline void xfs_buf_relse(struct xfs_buf *bp)
{
	xfs_buf_unlock(bp);
	xfs_buf_rele(bp);
}

/* Buffer Read and Write Routines */
extern int xfs_bwrite(struct xfs_buf *bp);

extern void __xfs_buf_ioerror(struct xfs_buf *bp, int error,
		xfs_failaddr_t failaddr);
#define xfs_buf_ioerror(bp, err) __xfs_buf_ioerror((bp), (err), __this_address)
extern void xfs_buf_ioerror_alert(struct xfs_buf *bp, xfs_failaddr_t fa);
void xfs_buf_ioend_fail(struct xfs_buf *);
void xfs_buf_zero(struct xfs_buf *bp, size_t boff, size_t bsize);
void __xfs_buf_mark_corrupt(struct xfs_buf *bp, xfs_failaddr_t fa);
#define xfs_buf_mark_corrupt(bp) __xfs_buf_mark_corrupt((bp), __this_address)

/* Buffer Utility Routines */
extern void *xfs_buf_offset(struct xfs_buf *, size_t);
extern void xfs_buf_stale(struct xfs_buf *bp);

/* Delayed Write Buffer Routines */
extern void xfs_buf_delwri_cancel(struct list_head *);
extern bool xfs_buf_delwri_queue(struct xfs_buf *, struct list_head *);
void xfs_buf_delwri_queue_here(struct xfs_buf *bp, struct list_head *bl);
extern int xfs_buf_delwri_submit(struct list_head *);
extern int xfs_buf_delwri_submit_nowait(struct list_head *);
extern int xfs_buf_delwri_pushbuf(struct xfs_buf *, struct list_head *);

static inline xfs_daddr_t xfs_buf_daddr(struct xfs_buf *bp)
{
	return bp->b_maps[0].bm_bn;
}

void xfs_buf_set_ref(struct xfs_buf *bp, int lru_ref);

/*
 * If the buffer is already on the LRU, do nothing. Otherwise set the buffer
 * up with a reference count of 0 so it will be tossed from the cache when
 * released.
 */
static inline void xfs_buf_oneshot(struct xfs_buf *bp)
{
	if (!list_empty(&bp->b_lru) || atomic_read(&bp->b_lru_ref) > 1)
		return;
	atomic_set(&bp->b_lru_ref, 0);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

static inline int xfs_buf_ispinned(struct xfs_buf *bp)
{
	return atomic_read(&bp->b_pin_count);
}

<<<<<<< HEAD
static inline void xfs_buf_relse(xfs_buf_t *bp)
{
	xfs_buf_unlock(bp);
	xfs_buf_rele(bp);
=======
static inline int
xfs_buf_verify_cksum(struct xfs_buf *bp, unsigned long cksum_offset)
{
	return xfs_verify_cksum(bp->b_addr, BBTOB(bp->b_length),
				cksum_offset);
}

static inline void
xfs_buf_update_cksum(struct xfs_buf *bp, unsigned long cksum_offset)
{
	xfs_update_cksum(bp->b_addr, BBTOB(bp->b_length),
			 cksum_offset);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

/*
 *	Handling of buftargs.
 */
<<<<<<< HEAD
extern xfs_buftarg_t *xfs_alloc_buftarg(struct xfs_mount *,
			struct block_device *, int, const char *);
extern void xfs_free_buftarg(struct xfs_mount *, struct xfs_buftarg *);
extern void xfs_wait_buftarg(xfs_buftarg_t *);
extern int xfs_setsize_buftarg(xfs_buftarg_t *, unsigned int, unsigned int);
extern int xfs_flush_buftarg(xfs_buftarg_t *, int);
=======
struct xfs_buftarg *xfs_alloc_buftarg(struct xfs_mount *mp,
		struct file *bdev_file);
extern void xfs_free_buftarg(struct xfs_buftarg *);
extern void xfs_buftarg_wait(struct xfs_buftarg *);
extern void xfs_buftarg_drain(struct xfs_buftarg *);
extern int xfs_setsize_buftarg(struct xfs_buftarg *, unsigned int);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

#define xfs_getsize_buftarg(buftarg)	block_size((buftarg)->bt_bdev)
#define xfs_readonly_buftarg(buftarg)	bdev_read_only((buftarg)->bt_bdev)

<<<<<<< HEAD
=======
int xfs_buf_reverify(struct xfs_buf *bp, const struct xfs_buf_ops *ops);
bool xfs_verify_magic(struct xfs_buf *bp, __be32 dmagic);
bool xfs_verify_magic16(struct xfs_buf *bp, __be16 dmagic);

/* for xfs_buf_mem.c only: */
int xfs_init_buftarg(struct xfs_buftarg *btp, size_t logical_sectorsize,
		const char *descr);
void xfs_destroy_buftarg(struct xfs_buftarg *btp);

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#endif	/* __XFS_BUF_H__ */
