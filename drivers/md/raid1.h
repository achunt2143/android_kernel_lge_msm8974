<<<<<<< HEAD
#ifndef _RAID1_H
#define _RAID1_H

struct mirror_info {
	struct md_rdev	*rdev;
	sector_t	head_position;
=======
/* SPDX-License-Identifier: GPL-2.0 */
#ifndef _RAID1_H
#define _RAID1_H

/*
 * each barrier unit size is 64MB fow now
 * note: it must be larger than RESYNC_DEPTH
 */
#define BARRIER_UNIT_SECTOR_BITS	17
#define BARRIER_UNIT_SECTOR_SIZE	(1<<17)
/*
 * In struct r1conf, the following members are related to I/O barrier
 * buckets,
 *	atomic_t	*nr_pending;
 *	atomic_t	*nr_waiting;
 *	atomic_t	*nr_queued;
 *	atomic_t	*barrier;
 * Each of them points to array of atomic_t variables, each array is
 * designed to have BARRIER_BUCKETS_NR elements and occupy a single
 * memory page. The data width of atomic_t variables is 4 bytes, equal
 * to 1<<(ilog2(sizeof(atomic_t))), BARRIER_BUCKETS_NR_BITS is defined
 * as (PAGE_SHIFT - ilog2(sizeof(int))) to make sure an array of
 * atomic_t variables with BARRIER_BUCKETS_NR elements just exactly
 * occupies a single memory page.
 */
#define BARRIER_BUCKETS_NR_BITS		(PAGE_SHIFT - ilog2(sizeof(atomic_t)))
#define BARRIER_BUCKETS_NR		(1<<BARRIER_BUCKETS_NR_BITS)

/* Note: raid1_info.rdev can be set to NULL asynchronously by raid1_remove_disk.
 * There are three safe ways to access raid1_info.rdev.
 * 1/ when holding mddev->reconfig_mutex
 * 2/ when resync/recovery is known to be happening - i.e. in code that is
 *    called as part of performing resync/recovery.
 * 3/ while holding rcu_read_lock(), use rcu_dereference to get the pointer
 *    and if it is non-NULL, increment rdev->nr_pending before dropping the
 *    RCU lock.
 * When .rdev is set to NULL, the nr_pending count checked again and if it has
 * been incremented, the pointer is put back in .rdev.
 */

struct raid1_info {
	struct md_rdev	*rdev;
	sector_t	head_position;

	/* When choose the best device for a read (read_balance())
	 * we try to keep sequential reads one the same device
	 */
	sector_t	next_seq_sect;
	sector_t	seq_start;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
};

/*
 * memory pools need a pointer to the mddev, so they can force an unplug
 * when memory is tight, and a count of the number of drives that the
 * pool was allocated for, so they know how much to allocate and free.
 * mddev->raid_disks cannot be used, as it can change while a pool is active
 * These two datums are stored in a kmalloced struct.
 * The 'raid_disks' here is twice the raid_disks in r1conf.
 * This allows space for each 'real' device can have a replacement in the
 * second half of the array.
 */

struct pool_info {
	struct mddev *mddev;
	int	raid_disks;
};

struct r1conf {
	struct mddev		*mddev;
<<<<<<< HEAD
	struct mirror_info	*mirrors;	/* twice 'raid_disks' to
						 * allow for replacements.
						 */
	int			raid_disks;

	/* When choose the best device for a read (read_balance())
	 * we try to keep sequential reads one the same device
	 * using 'last_used' and 'next_seq_sect'
	 */
	int			last_used;
	sector_t		next_seq_sect;
	/* During resync, read_balancing is only allowed on the part
	 * of the array that has been resynced.  'next_resync' tells us
	 * where that is.
	 */
	sector_t		next_resync;
=======
	struct raid1_info	*mirrors;	/* twice 'raid_disks' to
						 * allow for replacements.
						 */
	int			raid_disks;
	int			nonrot_disks;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	spinlock_t		device_lock;

	/* list of 'struct r1bio' that need to be processed by raid1d,
	 * whether to retry a read, writeout a resync or recovery
	 * block, or anything else.
	 */
	struct list_head	retry_list;
	/* A separate list of r1bio which just need raid_end_bio_io called.
	 * This mustn't happen for writes which had any errors if the superblock
	 * needs to be written.
	 */
	struct list_head	bio_end_io_list;

	/* queue pending writes to be submitted on unplug */
	struct bio_list		pending_bio_list;
<<<<<<< HEAD
	int			pending_count;
=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	/* for use when syncing mirrors:
	 * We don't allow both normal IO and resync/recovery IO at
	 * the same time - resync/recovery can only happen when there
	 * is no other IO.  So when either is active, the other has to wait.
	 * See more details description in raid1.c near raise_barrier().
	 */
	wait_queue_head_t	wait_barrier;
	spinlock_t		resync_lock;
<<<<<<< HEAD
	int			nr_pending;
	int			nr_waiting;
	int			nr_queued;
	int			barrier;
=======
	atomic_t		nr_sync_pending;
	atomic_t		*nr_pending;
	atomic_t		*nr_waiting;
	atomic_t		*nr_queued;
	atomic_t		*barrier;
	int			array_frozen;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	/* Set to 1 if a full sync is needed, (fresh device added).
	 * Cleared when a sync completes.
	 */
	int			fullsync;

	/* When the same as mddev->recovery_disabled we don't allow
	 * recovery to be attempted as we expect a read error.
	 */
	int			recovery_disabled;

<<<<<<< HEAD

=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	/* poolinfo contains information about the content of the
	 * mempools - it changes when the array grows or shrinks
	 */
	struct pool_info	*poolinfo;
<<<<<<< HEAD
	mempool_t		*r1bio_pool;
	mempool_t		*r1buf_pool;
=======
	mempool_t		r1bio_pool;
	mempool_t		r1buf_pool;

	struct bio_set		bio_split;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	/* temporary buffer to synchronous IO when attempting to repair
	 * a read error.
	 */
	struct page		*tmppage;

<<<<<<< HEAD

	/* When taking over an array from a different personality, we store
	 * the new thread here until we fully activate the array.
	 */
	struct md_thread	*thread;
=======
	/* When taking over an array from a different personality, we store
	 * the new thread here until we fully activate the array.
	 */
	struct md_thread __rcu	*thread;

	/* Keep track of cluster resync window to send to other
	 * nodes.
	 */
	sector_t		cluster_sync_low;
	sector_t		cluster_sync_high;

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
};

/*
 * this is our 'private' RAID1 bio.
 *
 * it contains information about what kind of IO operations were started
 * for this RAID1 operation, and about their status:
 */

struct r1bio {
	atomic_t		remaining; /* 'have we finished' count,
					    * used from IRQ handlers
					    */
	atomic_t		behind_remaining; /* number of write-behind ios remaining
						 * in this BehindIO request
						 */
	sector_t		sector;
	int			sectors;
	unsigned long		state;
	struct mddev		*mddev;
	/*
	 * original bio going to /dev/mdx
	 */
	struct bio		*master_bio;
	/*
	 * if the IO is in READ direction, then this is where we read
	 */
	int			read_disk;

	struct list_head	retry_list;
<<<<<<< HEAD
	/* Next two are only valid when R1BIO_BehindIO is set */
	struct bio_vec		*behind_bvecs;
	int			behind_page_count;
=======

	/*
	 * When R1BIO_BehindIO is set, we store pages for write behind
	 * in behind_master_bio.
	 */
	struct bio		*behind_master_bio;

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	/*
	 * if the IO is in WRITE direction, then multiple bios are used.
	 * We choose the number when they are allocated.
	 */
<<<<<<< HEAD
	struct bio		*bios[0];
	/* DO NOT PUT ANY NEW FIELDS HERE - bios array is contiguously alloced*/
};

/* when we get a read error on a read-only array, we redirect to another
 * device without failing the first device, or trying to over-write to
 * correct the read error.  To keep track of bad blocks on a per-bio
 * level, we store IO_BLOCKED in the appropriate 'bios' pointer
 */
#define IO_BLOCKED ((struct bio *)1)
/* When we successfully write to a known bad-block, we need to remove the
 * bad-block marking which must be done from process context.  So we record
 * the success by setting bios[n] to IO_MADE_GOOD
 */
#define IO_MADE_GOOD ((struct bio *)2)

#define BIO_SPECIAL(bio) ((unsigned long)bio <= 2)

/* bits for r1bio.state */
#define	R1BIO_Uptodate	0
#define	R1BIO_IsSync	1
#define	R1BIO_Degraded	2
#define	R1BIO_BehindIO	3
/* Set ReadError on bios that experience a readerror so that
 * raid1d knows what to do with them.
 */
#define R1BIO_ReadError 4
=======
	struct bio		*bios[];
	/* DO NOT PUT ANY NEW FIELDS HERE - bios array is contiguously alloced*/
};

/* bits for r1bio.state */
enum r1bio_state {
	R1BIO_Uptodate,
	R1BIO_IsSync,
	R1BIO_Degraded,
	R1BIO_BehindIO,
/* Set ReadError on bios that experience a readerror so that
 * raid1d knows what to do with them.
 */
	R1BIO_ReadError,
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
/* For write-behind requests, we call bi_end_io when
 * the last non-write-behind device completes, providing
 * any write was successful.  Otherwise we call when
 * any write-behind write succeeds, otherwise we call
 * with failure when last write completes (and all failed).
 * Record that bi_end_io was called with this flag...
 */
<<<<<<< HEAD
#define	R1BIO_Returned 6
/* If a write for this request means we can clear some
 * known-bad-block records, we set this flag
 */
#define	R1BIO_MadeGood 7
#define	R1BIO_WriteError 8

extern int md_raid1_congested(struct mddev *mddev, int bits);

=======
	R1BIO_Returned,
/* If a write for this request means we can clear some
 * known-bad-block records, we set this flag
 */
	R1BIO_MadeGood,
	R1BIO_WriteError,
	R1BIO_FailFast,
};

static inline int sector_to_idx(sector_t sector)
{
	return hash_long(sector >> BARRIER_UNIT_SECTOR_BITS,
			 BARRIER_BUCKETS_NR_BITS);
}
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#endif
