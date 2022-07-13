<<<<<<< HEAD
=======
// SPDX-License-Identifier: GPL-2.0-only
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
/*
 * Copyright (C) 2001, 2002 Sistina Software (UK) Limited.
 * Copyright (C) 2004-2008 Red Hat, Inc. All rights reserved.
 *
 * This file is released under the GPL.
 */

<<<<<<< HEAD
#include "dm.h"
#include "dm-uevent.h"
=======
#include "dm-core.h"
#include "dm-rq.h"
#include "dm-uevent.h"
#include "dm-ima.h"
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

#include <linux/init.h>
#include <linux/module.h>
#include <linux/mutex.h>
<<<<<<< HEAD
#include <linux/moduleparam.h>
#include <linux/blkpg.h>
#include <linux/bio.h>
#include <linux/mempool.h>
#include <linux/slab.h>
#include <linux/idr.h>
#include <linux/hdreg.h>
#include <linux/delay.h>

#include <trace/events/block.h>

#define DM_MSG_PREFIX "core"

#ifdef CONFIG_PRINTK
/*
 * ratelimit state to be used in DMXXX_LIMIT().
 */
DEFINE_RATELIMIT_STATE(dm_ratelimit_state,
		       DEFAULT_RATELIMIT_INTERVAL,
		       DEFAULT_RATELIMIT_BURST);
EXPORT_SYMBOL(dm_ratelimit_state);
#endif

=======
#include <linux/sched/mm.h>
#include <linux/sched/signal.h>
#include <linux/blkpg.h>
#include <linux/bio.h>
#include <linux/mempool.h>
#include <linux/dax.h>
#include <linux/slab.h>
#include <linux/idr.h>
#include <linux/uio.h>
#include <linux/hdreg.h>
#include <linux/delay.h>
#include <linux/wait.h>
#include <linux/pr.h>
#include <linux/refcount.h>
#include <linux/part_stat.h>
#include <linux/blk-crypto.h>
#include <linux/blk-crypto-profile.h>

#define DM_MSG_PREFIX "core"

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
/*
 * Cookies are numeric values sent with CHANGE and REMOVE
 * uevents while resuming, removing or renaming the device.
 */
#define DM_COOKIE_ENV_VAR_NAME "DM_COOKIE"
#define DM_COOKIE_LENGTH 24

<<<<<<< HEAD
static const char *_name = DM_NAME;

static unsigned int major = 0;
static unsigned int _major = 0;
=======
/*
 * For REQ_POLLED fs bio, this flag is set if we link mapped underlying
 * dm_io into one list, and reuse bio->bi_private as the list head. Before
 * ending this fs bio, we will recover its ->bi_private.
 */
#define REQ_DM_POLL_LIST	REQ_DRV

static const char *_name = DM_NAME;

static unsigned int major;
static unsigned int _major;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

static DEFINE_IDR(_minor_idr);

static DEFINE_SPINLOCK(_minor_lock);
<<<<<<< HEAD
/*
 * For bio-based dm.
 * One of these is allocated per bio.
 */
struct dm_io {
	struct mapped_device *md;
	int error;
	atomic_t io_count;
	struct bio *bio;
	unsigned long start_time;
	spinlock_t endio_lock;
};

/*
 * For bio-based dm.
 * One of these is allocated per target within a bio.  Hopefully
 * this will be simplified out one day.
 */
struct dm_target_io {
	struct dm_io *io;
	struct dm_target *ti;
	union map_info info;
};

/*
 * For request-based dm.
 * One of these is allocated per request.
 */
struct dm_rq_target_io {
	struct mapped_device *md;
	struct dm_target *ti;
	struct request *orig, clone;
	int error;
	union map_info info;
};

/*
 * For request-based dm.
 * One of these is allocated per bio.
 */
struct dm_rq_clone_bio_info {
	struct bio *orig;
	struct dm_rq_target_io *tio;
};

union map_info *dm_get_mapinfo(struct bio *bio)
{
	if (bio && bio->bi_private)
		return &((struct dm_target_io *)bio->bi_private)->info;
	return NULL;
}

union map_info *dm_get_rq_mapinfo(struct request *rq)
{
	if (rq && rq->end_io_data)
		return &((struct dm_rq_target_io *)rq->end_io_data)->info;
	return NULL;
}
EXPORT_SYMBOL_GPL(dm_get_rq_mapinfo);

#define MINOR_ALLOCED ((void *)-1)

/*
 * Bits for the md->flags field.
 */
#define DMF_BLOCK_IO_FOR_SUSPEND 0
#define DMF_SUSPENDED 1
#define DMF_FROZEN 2
#define DMF_FREEING 3
#define DMF_DELETING 4
#define DMF_NOFLUSH_SUSPENDING 5
#define DMF_MERGE_IS_OPTIONAL 6

/*
 * Work processed by per-device workqueue.
 */
struct mapped_device {
	struct rw_semaphore io_lock;
	struct mutex suspend_lock;
	rwlock_t map_lock;
	atomic_t holders;
	atomic_t open_count;

	unsigned long flags;

	struct request_queue *queue;
	unsigned type;
	/* Protect queue and type against concurrent access. */
	struct mutex type_lock;

	struct target_type *immutable_target_type;

	struct gendisk *disk;
	char name[16];

	void *interface_ptr;

	/*
	 * A list of ios that arrived while we were suspended.
	 */
	atomic_t pending[2];
	wait_queue_head_t wait;
	struct work_struct work;
	struct bio_list deferred;
	spinlock_t deferred_lock;

	/*
	 * Processing queue (flush)
	 */
	struct workqueue_struct *wq;

	/*
	 * The current mapping.
	 */
	struct dm_table *map;

	/*
	 * io objects are allocated from here.
	 */
	mempool_t *io_pool;
	mempool_t *tio_pool;

	struct bio_set *bs;

	/*
	 * Event handling.
	 */
	atomic_t event_nr;
	wait_queue_head_t eventq;
	atomic_t uevent_seq;
	struct list_head uevent_list;
	spinlock_t uevent_lock; /* Protect access to uevent_list */

	/*
	 * freeze/thaw support require holding onto a super block
	 */
	struct super_block *frozen_sb;
	struct block_device *bdev;

	/* forced geometry settings */
	struct hd_geometry geometry;

	/* kobject and completion */
	struct dm_kobject_holder kobj_holder;

	/* zero-length flush that will be cloned and submitted to targets */
	struct bio flush_bio;
};

/*
 * For mempools pre-allocation at the table loading time.
 */
struct dm_md_mempools {
	mempool_t *io_pool;
	mempool_t *tio_pool;
	struct bio_set *bs;
};

#define MIN_IOS 256
static struct kmem_cache *_io_cache;
static struct kmem_cache *_tio_cache;
static struct kmem_cache *_rq_tio_cache;
static struct kmem_cache *_rq_bio_info_cache;

static int __init local_init(void)
{
	int r = -ENOMEM;

	/* allocate a slab for the dm_ios */
	_io_cache = KMEM_CACHE(dm_io, 0);
	if (!_io_cache)
		return r;

	/* allocate a slab for the target ios */
	_tio_cache = KMEM_CACHE(dm_target_io, 0);
	if (!_tio_cache)
		goto out_free_io_cache;

	_rq_tio_cache = KMEM_CACHE(dm_rq_target_io, 0);
	if (!_rq_tio_cache)
		goto out_free_tio_cache;

	_rq_bio_info_cache = KMEM_CACHE(dm_rq_clone_bio_info, 0);
	if (!_rq_bio_info_cache)
		goto out_free_rq_tio_cache;

	r = dm_uevent_init();
	if (r)
		goto out_free_rq_bio_info_cache;
=======

static void do_deferred_remove(struct work_struct *w);

static DECLARE_WORK(deferred_remove_work, do_deferred_remove);

static struct workqueue_struct *deferred_remove_workqueue;

atomic_t dm_global_event_nr = ATOMIC_INIT(0);
DECLARE_WAIT_QUEUE_HEAD(dm_global_eventq);

void dm_issue_global_event(void)
{
	atomic_inc(&dm_global_event_nr);
	wake_up(&dm_global_eventq);
}

DEFINE_STATIC_KEY_FALSE(stats_enabled);
DEFINE_STATIC_KEY_FALSE(swap_bios_enabled);
DEFINE_STATIC_KEY_FALSE(zoned_enabled);

/*
 * One of these is allocated (on-stack) per original bio.
 */
struct clone_info {
	struct dm_table *map;
	struct bio *bio;
	struct dm_io *io;
	sector_t sector;
	unsigned int sector_count;
	bool is_abnormal_io:1;
	bool submit_as_polled:1;
};

static inline struct dm_target_io *clone_to_tio(struct bio *clone)
{
	return container_of(clone, struct dm_target_io, clone);
}

void *dm_per_bio_data(struct bio *bio, size_t data_size)
{
	if (!dm_tio_flagged(clone_to_tio(bio), DM_TIO_INSIDE_DM_IO))
		return (char *)bio - DM_TARGET_IO_BIO_OFFSET - data_size;
	return (char *)bio - DM_IO_BIO_OFFSET - data_size;
}
EXPORT_SYMBOL_GPL(dm_per_bio_data);

struct bio *dm_bio_from_per_bio_data(void *data, size_t data_size)
{
	struct dm_io *io = (struct dm_io *)((char *)data + data_size);

	if (io->magic == DM_IO_MAGIC)
		return (struct bio *)((char *)io + DM_IO_BIO_OFFSET);
	BUG_ON(io->magic != DM_TIO_MAGIC);
	return (struct bio *)((char *)io + DM_TARGET_IO_BIO_OFFSET);
}
EXPORT_SYMBOL_GPL(dm_bio_from_per_bio_data);

unsigned int dm_bio_get_target_bio_nr(const struct bio *bio)
{
	return container_of(bio, struct dm_target_io, clone)->target_bio_nr;
}
EXPORT_SYMBOL_GPL(dm_bio_get_target_bio_nr);

#define MINOR_ALLOCED ((void *)-1)

#define DM_NUMA_NODE NUMA_NO_NODE
static int dm_numa_node = DM_NUMA_NODE;

#define DEFAULT_SWAP_BIOS	(8 * 1048576 / PAGE_SIZE)
static int swap_bios = DEFAULT_SWAP_BIOS;
static int get_swap_bios(void)
{
	int latch = READ_ONCE(swap_bios);

	if (unlikely(latch <= 0))
		latch = DEFAULT_SWAP_BIOS;
	return latch;
}

struct table_device {
	struct list_head list;
	refcount_t count;
	struct dm_dev dm_dev;
};

/*
 * Bio-based DM's mempools' reserved IOs set by the user.
 */
#define RESERVED_BIO_BASED_IOS		16
static unsigned int reserved_bio_based_ios = RESERVED_BIO_BASED_IOS;

static int __dm_get_module_param_int(int *module_param, int min, int max)
{
	int param = READ_ONCE(*module_param);
	int modified_param = 0;
	bool modified = true;

	if (param < min)
		modified_param = min;
	else if (param > max)
		modified_param = max;
	else
		modified = false;

	if (modified) {
		(void)cmpxchg(module_param, param, modified_param);
		param = modified_param;
	}

	return param;
}

unsigned int __dm_get_module_param(unsigned int *module_param, unsigned int def, unsigned int max)
{
	unsigned int param = READ_ONCE(*module_param);
	unsigned int modified_param = 0;

	if (!param)
		modified_param = def;
	else if (param > max)
		modified_param = max;

	if (modified_param) {
		(void)cmpxchg(module_param, param, modified_param);
		param = modified_param;
	}

	return param;
}

unsigned int dm_get_reserved_bio_based_ios(void)
{
	return __dm_get_module_param(&reserved_bio_based_ios,
				     RESERVED_BIO_BASED_IOS, DM_RESERVED_MAX_IOS);
}
EXPORT_SYMBOL_GPL(dm_get_reserved_bio_based_ios);

static unsigned int dm_get_numa_node(void)
{
	return __dm_get_module_param_int(&dm_numa_node,
					 DM_NUMA_NODE, num_online_nodes() - 1);
}

static int __init local_init(void)
{
	int r;

	r = dm_uevent_init();
	if (r)
		return r;

	deferred_remove_workqueue = alloc_ordered_workqueue("kdmremove", 0);
	if (!deferred_remove_workqueue) {
		r = -ENOMEM;
		goto out_uevent_exit;
	}
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	_major = major;
	r = register_blkdev(_major, _name);
	if (r < 0)
<<<<<<< HEAD
		goto out_uevent_exit;
=======
		goto out_free_workqueue;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	if (!_major)
		_major = r;

	return 0;

<<<<<<< HEAD
out_uevent_exit:
	dm_uevent_exit();
out_free_rq_bio_info_cache:
	kmem_cache_destroy(_rq_bio_info_cache);
out_free_rq_tio_cache:
	kmem_cache_destroy(_rq_tio_cache);
out_free_tio_cache:
	kmem_cache_destroy(_tio_cache);
out_free_io_cache:
	kmem_cache_destroy(_io_cache);
=======
out_free_workqueue:
	destroy_workqueue(deferred_remove_workqueue);
out_uevent_exit:
	dm_uevent_exit();
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	return r;
}

static void local_exit(void)
{
<<<<<<< HEAD
	kmem_cache_destroy(_rq_bio_info_cache);
	kmem_cache_destroy(_rq_tio_cache);
	kmem_cache_destroy(_tio_cache);
	kmem_cache_destroy(_io_cache);
=======
	destroy_workqueue(deferred_remove_workqueue);

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	unregister_blkdev(_major, _name);
	dm_uevent_exit();

	_major = 0;

	DMINFO("cleaned up");
}

static int (*_inits[])(void) __initdata = {
	local_init,
	dm_target_init,
	dm_linear_init,
	dm_stripe_init,
	dm_io_init,
	dm_kcopyd_init,
	dm_interface_init,
<<<<<<< HEAD
=======
	dm_statistics_init,
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
};

static void (*_exits[])(void) = {
	local_exit,
	dm_target_exit,
	dm_linear_exit,
	dm_stripe_exit,
	dm_io_exit,
	dm_kcopyd_exit,
	dm_interface_exit,
<<<<<<< HEAD
=======
	dm_statistics_exit,
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
};

static int __init dm_init(void)
{
	const int count = ARRAY_SIZE(_inits);
<<<<<<< HEAD

	int r, i;

=======
	int r, i;

#if (IS_ENABLED(CONFIG_IMA) && !IS_ENABLED(CONFIG_IMA_DISABLE_HTABLE))
	DMWARN("CONFIG_IMA_DISABLE_HTABLE is disabled."
	       " Duplicate IMA measurements will not be recorded in the IMA log.");
#endif

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	for (i = 0; i < count; i++) {
		r = _inits[i]();
		if (r)
			goto bad;
	}

	return 0;
<<<<<<< HEAD

      bad:
=======
bad:
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	while (i--)
		_exits[i]();

	return r;
}

static void __exit dm_exit(void)
{
	int i = ARRAY_SIZE(_exits);

	while (i--)
		_exits[i]();

	/*
	 * Should be empty by this point.
	 */
<<<<<<< HEAD
	idr_remove_all(&_minor_idr);
=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	idr_destroy(&_minor_idr);
}

/*
 * Block device functions
 */
int dm_deleting_md(struct mapped_device *md)
{
	return test_bit(DMF_DELETING, &md->flags);
}

<<<<<<< HEAD
static int dm_blk_open(struct block_device *bdev, fmode_t mode)
=======
static int dm_blk_open(struct gendisk *disk, blk_mode_t mode)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
{
	struct mapped_device *md;

	spin_lock(&_minor_lock);

<<<<<<< HEAD
	md = bdev->bd_disk->private_data;
=======
	md = disk->private_data;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	if (!md)
		goto out;

	if (test_bit(DMF_FREEING, &md->flags) ||
	    dm_deleting_md(md)) {
		md = NULL;
		goto out;
	}

	dm_get(md);
	atomic_inc(&md->open_count);
<<<<<<< HEAD

=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
out:
	spin_unlock(&_minor_lock);

	return md ? 0 : -ENXIO;
}

<<<<<<< HEAD
static int dm_blk_close(struct gendisk *disk, fmode_t mode)
{
	struct mapped_device *md = disk->private_data;

	spin_lock(&_minor_lock);

	atomic_dec(&md->open_count);
	dm_put(md);

	spin_unlock(&_minor_lock);

	return 0;
=======
static void dm_blk_close(struct gendisk *disk)
{
	struct mapped_device *md;

	spin_lock(&_minor_lock);

	md = disk->private_data;
	if (WARN_ON(!md))
		goto out;

	if (atomic_dec_and_test(&md->open_count) &&
	    (test_bit(DMF_DEFERRED_REMOVE, &md->flags)))
		queue_work(deferred_remove_workqueue, &deferred_remove_work);

	dm_put(md);
out:
	spin_unlock(&_minor_lock);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

int dm_open_count(struct mapped_device *md)
{
	return atomic_read(&md->open_count);
}

/*
 * Guarantees nothing is using the device before it's deleted.
 */
<<<<<<< HEAD
int dm_lock_for_deletion(struct mapped_device *md)
=======
int dm_lock_for_deletion(struct mapped_device *md, bool mark_deferred, bool only_deferred)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
{
	int r = 0;

	spin_lock(&_minor_lock);

<<<<<<< HEAD
	if (dm_open_count(md))
		r = -EBUSY;
=======
	if (dm_open_count(md)) {
		r = -EBUSY;
		if (mark_deferred)
			set_bit(DMF_DEFERRED_REMOVE, &md->flags);
	} else if (only_deferred && !test_bit(DMF_DEFERRED_REMOVE, &md->flags))
		r = -EEXIST;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	else
		set_bit(DMF_DELETING, &md->flags);

	spin_unlock(&_minor_lock);

	return r;
}

<<<<<<< HEAD
=======
int dm_cancel_deferred_remove(struct mapped_device *md)
{
	int r = 0;

	spin_lock(&_minor_lock);

	if (test_bit(DMF_DELETING, &md->flags))
		r = -EBUSY;
	else
		clear_bit(DMF_DEFERRED_REMOVE, &md->flags);

	spin_unlock(&_minor_lock);

	return r;
}

static void do_deferred_remove(struct work_struct *w)
{
	dm_deferred_remove();
}

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
static int dm_blk_getgeo(struct block_device *bdev, struct hd_geometry *geo)
{
	struct mapped_device *md = bdev->bd_disk->private_data;

	return dm_get_geometry(md, geo);
}

<<<<<<< HEAD
static int dm_blk_ioctl(struct block_device *bdev, fmode_t mode,
			unsigned int cmd, unsigned long arg)
{
	struct mapped_device *md = bdev->bd_disk->private_data;
	struct dm_table *map = dm_get_live_table(md);
	struct dm_target *tgt;
	int r = -ENOTTY;

	if (!map || !dm_table_get_size(map))
		goto out;

	/* We only support devices that have a single target */
	if (dm_table_get_num_targets(map) != 1)
		goto out;

	tgt = dm_table_get_target(map, 0);

	if (dm_suspended_md(md)) {
		r = -EAGAIN;
		goto out;
	}

	if (tgt->type->ioctl)
		r = tgt->type->ioctl(tgt, cmd, arg);

out:
	dm_table_put(map);

	return r;
}

static struct dm_io *alloc_io(struct mapped_device *md)
{
	return mempool_alloc(md->io_pool, GFP_NOIO);
}

static void free_io(struct mapped_device *md, struct dm_io *io)
{
	mempool_free(io, md->io_pool);
}

static void free_tio(struct mapped_device *md, struct dm_target_io *tio)
{
	mempool_free(tio, md->tio_pool);
}

static struct dm_rq_target_io *alloc_rq_tio(struct mapped_device *md,
					    gfp_t gfp_mask)
{
	return mempool_alloc(md->tio_pool, gfp_mask);
}

static void free_rq_tio(struct dm_rq_target_io *tio)
{
	mempool_free(tio, tio->md->tio_pool);
}

static struct dm_rq_clone_bio_info *alloc_bio_info(struct mapped_device *md)
{
	return mempool_alloc(md->io_pool, GFP_ATOMIC);
}

static void free_bio_info(struct dm_rq_clone_bio_info *info)
{
	mempool_free(info, info->tio->md->io_pool);
}

static int md_in_flight(struct mapped_device *md)
{
	return atomic_read(&md->pending[READ]) +
	       atomic_read(&md->pending[WRITE]);
}

static void start_io_acct(struct dm_io *io)
{
	struct mapped_device *md = io->md;
	int cpu;
	int rw = bio_data_dir(io->bio);

	io->start_time = jiffies;

	cpu = part_stat_lock();
	part_round_stats(cpu, &dm_disk(md)->part0);
	part_stat_unlock();
	atomic_set(&dm_disk(md)->part0.in_flight[rw],
		atomic_inc_return(&md->pending[rw]));
}

static void end_io_acct(struct dm_io *io)
{
	struct mapped_device *md = io->md;
	struct bio *bio = io->bio;
	unsigned long duration = jiffies - io->start_time;
	int pending, cpu;
	int rw = bio_data_dir(bio);

	cpu = part_stat_lock();
	part_round_stats(cpu, &dm_disk(md)->part0);
	part_stat_add(cpu, &dm_disk(md)->part0, ticks[rw], duration);
	part_stat_unlock();

	/*
	 * After this is decremented the bio must not be touched if it is
	 * a flush.
	 */
	pending = atomic_dec_return(&md->pending[rw]);
	atomic_set(&dm_disk(md)->part0.in_flight[rw], pending);
	pending += atomic_read(&md->pending[rw^0x1]);

	/* nudge anyone waiting on suspend queue */
	if (!pending)
		wake_up(&md->wait);
=======
static int dm_prepare_ioctl(struct mapped_device *md, int *srcu_idx,
			    struct block_device **bdev)
{
	struct dm_target *ti;
	struct dm_table *map;
	int r;

retry:
	r = -ENOTTY;
	map = dm_get_live_table(md, srcu_idx);
	if (!map || !dm_table_get_size(map))
		return r;

	/* We only support devices that have a single target */
	if (map->num_targets != 1)
		return r;

	ti = dm_table_get_target(map, 0);
	if (!ti->type->prepare_ioctl)
		return r;

	if (dm_suspended_md(md))
		return -EAGAIN;

	r = ti->type->prepare_ioctl(ti, bdev);
	if (r == -ENOTCONN && !fatal_signal_pending(current)) {
		dm_put_live_table(md, *srcu_idx);
		fsleep(10000);
		goto retry;
	}

	return r;
}

static void dm_unprepare_ioctl(struct mapped_device *md, int srcu_idx)
{
	dm_put_live_table(md, srcu_idx);
}

static int dm_blk_ioctl(struct block_device *bdev, blk_mode_t mode,
			unsigned int cmd, unsigned long arg)
{
	struct mapped_device *md = bdev->bd_disk->private_data;
	int r, srcu_idx;

	r = dm_prepare_ioctl(md, &srcu_idx, &bdev);
	if (r < 0)
		goto out;

	if (r > 0) {
		/*
		 * Target determined this ioctl is being issued against a
		 * subset of the parent bdev; require extra privileges.
		 */
		if (!capable(CAP_SYS_RAWIO)) {
			DMDEBUG_LIMIT(
	"%s: sending ioctl %x to DM device without required privilege.",
				current->comm, cmd);
			r = -ENOIOCTLCMD;
			goto out;
		}
	}

	if (!bdev->bd_disk->fops->ioctl)
		r = -ENOTTY;
	else
		r = bdev->bd_disk->fops->ioctl(bdev, mode, cmd, arg);
out:
	dm_unprepare_ioctl(md, srcu_idx);
	return r;
}

u64 dm_start_time_ns_from_clone(struct bio *bio)
{
	return jiffies_to_nsecs(clone_to_tio(bio)->io->start_time);
}
EXPORT_SYMBOL_GPL(dm_start_time_ns_from_clone);

static inline bool bio_is_flush_with_data(struct bio *bio)
{
	return ((bio->bi_opf & REQ_PREFLUSH) && bio->bi_iter.bi_size);
}

static inline unsigned int dm_io_sectors(struct dm_io *io, struct bio *bio)
{
	/*
	 * If REQ_PREFLUSH set, don't account payload, it will be
	 * submitted (and accounted) after this flush completes.
	 */
	if (bio_is_flush_with_data(bio))
		return 0;
	if (unlikely(dm_io_flagged(io, DM_IO_WAS_SPLIT)))
		return io->sectors;
	return bio_sectors(bio);
}

static void dm_io_acct(struct dm_io *io, bool end)
{
	struct bio *bio = io->orig_bio;

	if (dm_io_flagged(io, DM_IO_BLK_STAT)) {
		if (!end)
			bdev_start_io_acct(bio->bi_bdev, bio_op(bio),
					   io->start_time);
		else
			bdev_end_io_acct(bio->bi_bdev, bio_op(bio),
					 dm_io_sectors(io, bio),
					 io->start_time);
	}

	if (static_branch_unlikely(&stats_enabled) &&
	    unlikely(dm_stats_used(&io->md->stats))) {
		sector_t sector;

		if (unlikely(dm_io_flagged(io, DM_IO_WAS_SPLIT)))
			sector = bio_end_sector(bio) - io->sector_offset;
		else
			sector = bio->bi_iter.bi_sector;

		dm_stats_account_io(&io->md->stats, bio_data_dir(bio),
				    sector, dm_io_sectors(io, bio),
				    end, io->start_time, &io->stats_aux);
	}
}

static void __dm_start_io_acct(struct dm_io *io)
{
	dm_io_acct(io, false);
}

static void dm_start_io_acct(struct dm_io *io, struct bio *clone)
{
	/*
	 * Ensure IO accounting is only ever started once.
	 */
	if (dm_io_flagged(io, DM_IO_ACCOUNTED))
		return;

	/* Expect no possibility for race unless DM_TIO_IS_DUPLICATE_BIO. */
	if (!clone || likely(dm_tio_is_normal(clone_to_tio(clone)))) {
		dm_io_set_flag(io, DM_IO_ACCOUNTED);
	} else {
		unsigned long flags;
		/* Can afford locking given DM_TIO_IS_DUPLICATE_BIO */
		spin_lock_irqsave(&io->lock, flags);
		if (dm_io_flagged(io, DM_IO_ACCOUNTED)) {
			spin_unlock_irqrestore(&io->lock, flags);
			return;
		}
		dm_io_set_flag(io, DM_IO_ACCOUNTED);
		spin_unlock_irqrestore(&io->lock, flags);
	}

	__dm_start_io_acct(io);
}

static void dm_end_io_acct(struct dm_io *io)
{
	dm_io_acct(io, true);
}

static struct dm_io *alloc_io(struct mapped_device *md, struct bio *bio, gfp_t gfp_mask)
{
	struct dm_io *io;
	struct dm_target_io *tio;
	struct bio *clone;

	clone = bio_alloc_clone(NULL, bio, gfp_mask, &md->mempools->io_bs);
	if (unlikely(!clone))
		return NULL;
	tio = clone_to_tio(clone);
	tio->flags = 0;
	dm_tio_set_flag(tio, DM_TIO_INSIDE_DM_IO);
	tio->io = NULL;

	io = container_of(tio, struct dm_io, tio);
	io->magic = DM_IO_MAGIC;
	io->status = BLK_STS_OK;

	/* one ref is for submission, the other is for completion */
	atomic_set(&io->io_count, 2);
	this_cpu_inc(*md->pending_io);
	io->orig_bio = bio;
	io->md = md;
	spin_lock_init(&io->lock);
	io->start_time = jiffies;
	io->flags = 0;
	if (blk_queue_io_stat(md->queue))
		dm_io_set_flag(io, DM_IO_BLK_STAT);

	if (static_branch_unlikely(&stats_enabled) &&
	    unlikely(dm_stats_used(&md->stats)))
		dm_stats_record_start(&md->stats, &io->stats_aux);

	return io;
}

static void free_io(struct dm_io *io)
{
	bio_put(&io->tio.clone);
}

static struct bio *alloc_tio(struct clone_info *ci, struct dm_target *ti,
			     unsigned int target_bio_nr, unsigned int *len, gfp_t gfp_mask)
{
	struct mapped_device *md = ci->io->md;
	struct dm_target_io *tio;
	struct bio *clone;

	if (!ci->io->tio.io) {
		/* the dm_target_io embedded in ci->io is available */
		tio = &ci->io->tio;
		/* alloc_io() already initialized embedded clone */
		clone = &tio->clone;
	} else {
		clone = bio_alloc_clone(NULL, ci->bio, gfp_mask,
					&md->mempools->bs);
		if (!clone)
			return NULL;

		/* REQ_DM_POLL_LIST shouldn't be inherited */
		clone->bi_opf &= ~REQ_DM_POLL_LIST;

		tio = clone_to_tio(clone);
		tio->flags = 0; /* also clears DM_TIO_INSIDE_DM_IO */
	}

	tio->magic = DM_TIO_MAGIC;
	tio->io = ci->io;
	tio->ti = ti;
	tio->target_bio_nr = target_bio_nr;
	tio->len_ptr = len;
	tio->old_sector = 0;

	/* Set default bdev, but target must bio_set_dev() before issuing IO */
	clone->bi_bdev = md->disk->part0;
	if (unlikely(ti->needs_bio_set_dev))
		bio_set_dev(clone, md->disk->part0);

	if (len) {
		clone->bi_iter.bi_size = to_bytes(*len);
		if (bio_integrity(clone))
			bio_integrity_trim(clone);
	}

	return clone;
}

static void free_tio(struct bio *clone)
{
	if (dm_tio_flagged(clone_to_tio(clone), DM_TIO_INSIDE_DM_IO))
		return;
	bio_put(clone);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

/*
 * Add the bio to the list of deferred io.
 */
static void queue_io(struct mapped_device *md, struct bio *bio)
{
	unsigned long flags;

	spin_lock_irqsave(&md->deferred_lock, flags);
	bio_list_add(&md->deferred, bio);
	spin_unlock_irqrestore(&md->deferred_lock, flags);
	queue_work(md->wq, &md->work);
}

/*
 * Everyone (including functions in this file), should use this
 * function to access the md->map field, and make sure they call
<<<<<<< HEAD
 * dm_table_put() when finished.
 */
struct dm_table *dm_get_live_table(struct mapped_device *md)
{
	struct dm_table *t;
	unsigned long flags;

	read_lock_irqsave(&md->map_lock, flags);
	t = md->map;
	if (t)
		dm_table_get(t);
	read_unlock_irqrestore(&md->map_lock, flags);

	return t;
=======
 * dm_put_live_table() when finished.
 */
struct dm_table *dm_get_live_table(struct mapped_device *md,
				   int *srcu_idx) __acquires(md->io_barrier)
{
	*srcu_idx = srcu_read_lock(&md->io_barrier);

	return srcu_dereference(md->map, &md->io_barrier);
}

void dm_put_live_table(struct mapped_device *md,
		       int srcu_idx) __releases(md->io_barrier)
{
	srcu_read_unlock(&md->io_barrier, srcu_idx);
}

void dm_sync_table(struct mapped_device *md)
{
	synchronize_srcu(&md->io_barrier);
	synchronize_rcu_expedited();
}

/*
 * A fast alternative to dm_get_live_table/dm_put_live_table.
 * The caller must not block between these two functions.
 */
static struct dm_table *dm_get_live_table_fast(struct mapped_device *md) __acquires(RCU)
{
	rcu_read_lock();
	return rcu_dereference(md->map);
}

static void dm_put_live_table_fast(struct mapped_device *md) __releases(RCU)
{
	rcu_read_unlock();
}

static char *_dm_claim_ptr = "I belong to device-mapper";

/*
 * Open a table device so we can use it as a map destination.
 */
static struct table_device *open_table_device(struct mapped_device *md,
		dev_t dev, blk_mode_t mode)
{
	struct table_device *td;
	struct file *bdev_file;
	struct block_device *bdev;
	u64 part_off;
	int r;

	td = kmalloc_node(sizeof(*td), GFP_KERNEL, md->numa_node_id);
	if (!td)
		return ERR_PTR(-ENOMEM);
	refcount_set(&td->count, 1);

	bdev_file = bdev_file_open_by_dev(dev, mode, _dm_claim_ptr, NULL);
	if (IS_ERR(bdev_file)) {
		r = PTR_ERR(bdev_file);
		goto out_free_td;
	}

	bdev = file_bdev(bdev_file);

	/*
	 * We can be called before the dm disk is added.  In that case we can't
	 * register the holder relation here.  It will be done once add_disk was
	 * called.
	 */
	if (md->disk->slave_dir) {
		r = bd_link_disk_holder(bdev, md->disk);
		if (r)
			goto out_blkdev_put;
	}

	td->dm_dev.mode = mode;
	td->dm_dev.bdev = bdev;
	td->dm_dev.bdev_file = bdev_file;
	td->dm_dev.dax_dev = fs_dax_get_by_bdev(bdev, &part_off,
						NULL, NULL);
	format_dev_t(td->dm_dev.name, dev);
	list_add(&td->list, &md->table_devices);
	return td;

out_blkdev_put:
	__fput_sync(bdev_file);
out_free_td:
	kfree(td);
	return ERR_PTR(r);
}

/*
 * Close a table device that we've been using.
 */
static void close_table_device(struct table_device *td, struct mapped_device *md)
{
	if (md->disk->slave_dir)
		bd_unlink_disk_holder(td->dm_dev.bdev, md->disk);

	/* Leverage async fput() if DMF_DEFERRED_REMOVE set */
	if (unlikely(test_bit(DMF_DEFERRED_REMOVE, &md->flags)))
		fput(td->dm_dev.bdev_file);
	else
		__fput_sync(td->dm_dev.bdev_file);

	put_dax(td->dm_dev.dax_dev);
	list_del(&td->list);
	kfree(td);
}

static struct table_device *find_table_device(struct list_head *l, dev_t dev,
					      blk_mode_t mode)
{
	struct table_device *td;

	list_for_each_entry(td, l, list)
		if (td->dm_dev.bdev->bd_dev == dev && td->dm_dev.mode == mode)
			return td;

	return NULL;
}

int dm_get_table_device(struct mapped_device *md, dev_t dev, blk_mode_t mode,
			struct dm_dev **result)
{
	struct table_device *td;

	mutex_lock(&md->table_devices_lock);
	td = find_table_device(&md->table_devices, dev, mode);
	if (!td) {
		td = open_table_device(md, dev, mode);
		if (IS_ERR(td)) {
			mutex_unlock(&md->table_devices_lock);
			return PTR_ERR(td);
		}
	} else {
		refcount_inc(&td->count);
	}
	mutex_unlock(&md->table_devices_lock);

	*result = &td->dm_dev;
	return 0;
}

void dm_put_table_device(struct mapped_device *md, struct dm_dev *d)
{
	struct table_device *td = container_of(d, struct table_device, dm_dev);

	mutex_lock(&md->table_devices_lock);
	if (refcount_dec_and_test(&td->count))
		close_table_device(td, md);
	mutex_unlock(&md->table_devices_lock);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

/*
 * Get the geometry associated with a dm device
 */
int dm_get_geometry(struct mapped_device *md, struct hd_geometry *geo)
{
	*geo = md->geometry;

	return 0;
}

/*
 * Set the geometry of a device.
 */
int dm_set_geometry(struct mapped_device *md, struct hd_geometry *geo)
{
	sector_t sz = (sector_t)geo->cylinders * geo->heads * geo->sectors;

	if (geo->start > sz) {
<<<<<<< HEAD
		DMWARN("Start sector is beyond the geometry limits.");
=======
		DMERR("Start sector is beyond the geometry limits.");
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		return -EINVAL;
	}

	md->geometry = *geo;

	return 0;
}

<<<<<<< HEAD
/*-----------------------------------------------------------------
 * CRUD START:
 *   A more elegant soln is in the works that uses the queue
 *   merge fn, unfortunately there are a couple of changes to
 *   the block layer that I want to make for this.  So in the
 *   interests of getting something for people to use I give
 *   you this clearly demarcated crap.
 *---------------------------------------------------------------*/

=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
static int __noflush_suspending(struct mapped_device *md)
{
	return test_bit(DMF_NOFLUSH_SUSPENDING, &md->flags);
}

<<<<<<< HEAD
=======
static void dm_requeue_add_io(struct dm_io *io, bool first_stage)
{
	struct mapped_device *md = io->md;

	if (first_stage) {
		struct dm_io *next = md->requeue_list;

		md->requeue_list = io;
		io->next = next;
	} else {
		bio_list_add_head(&md->deferred, io->orig_bio);
	}
}

static void dm_kick_requeue(struct mapped_device *md, bool first_stage)
{
	if (first_stage)
		queue_work(md->wq, &md->requeue_work);
	else
		queue_work(md->wq, &md->work);
}

/*
 * Return true if the dm_io's original bio is requeued.
 * io->status is updated with error if requeue disallowed.
 */
static bool dm_handle_requeue(struct dm_io *io, bool first_stage)
{
	struct bio *bio = io->orig_bio;
	bool handle_requeue = (io->status == BLK_STS_DM_REQUEUE);
	bool handle_polled_eagain = ((io->status == BLK_STS_AGAIN) &&
				     (bio->bi_opf & REQ_POLLED));
	struct mapped_device *md = io->md;
	bool requeued = false;

	if (handle_requeue || handle_polled_eagain) {
		unsigned long flags;

		if (bio->bi_opf & REQ_POLLED) {
			/*
			 * Upper layer won't help us poll split bio
			 * (io->orig_bio may only reflect a subset of the
			 * pre-split original) so clear REQ_POLLED.
			 */
			bio_clear_polled(bio);
		}

		/*
		 * Target requested pushing back the I/O or
		 * polled IO hit BLK_STS_AGAIN.
		 */
		spin_lock_irqsave(&md->deferred_lock, flags);
		if ((__noflush_suspending(md) &&
		     !WARN_ON_ONCE(dm_is_zone_write(md, bio))) ||
		    handle_polled_eagain || first_stage) {
			dm_requeue_add_io(io, first_stage);
			requeued = true;
		} else {
			/*
			 * noflush suspend was interrupted or this is
			 * a write to a zoned target.
			 */
			io->status = BLK_STS_IOERR;
		}
		spin_unlock_irqrestore(&md->deferred_lock, flags);
	}

	if (requeued)
		dm_kick_requeue(md, first_stage);

	return requeued;
}

static void __dm_io_complete(struct dm_io *io, bool first_stage)
{
	struct bio *bio = io->orig_bio;
	struct mapped_device *md = io->md;
	blk_status_t io_error;
	bool requeued;

	requeued = dm_handle_requeue(io, first_stage);
	if (requeued && first_stage)
		return;

	io_error = io->status;
	if (dm_io_flagged(io, DM_IO_ACCOUNTED))
		dm_end_io_acct(io);
	else if (!io_error) {
		/*
		 * Must handle target that DM_MAPIO_SUBMITTED only to
		 * then bio_endio() rather than dm_submit_bio_remap()
		 */
		__dm_start_io_acct(io);
		dm_end_io_acct(io);
	}
	free_io(io);
	smp_wmb();
	this_cpu_dec(*md->pending_io);

	/* nudge anyone waiting on suspend queue */
	if (unlikely(wq_has_sleeper(&md->wait)))
		wake_up(&md->wait);

	/* Return early if the original bio was requeued */
	if (requeued)
		return;

	if (bio_is_flush_with_data(bio)) {
		/*
		 * Preflush done for flush with data, reissue
		 * without REQ_PREFLUSH.
		 */
		bio->bi_opf &= ~REQ_PREFLUSH;
		queue_io(md, bio);
	} else {
		/* done with normal IO or empty flush */
		if (io_error)
			bio->bi_status = io_error;
		bio_endio(bio);
	}
}

static void dm_wq_requeue_work(struct work_struct *work)
{
	struct mapped_device *md = container_of(work, struct mapped_device,
						requeue_work);
	unsigned long flags;
	struct dm_io *io;

	/* reuse deferred lock to simplify dm_handle_requeue */
	spin_lock_irqsave(&md->deferred_lock, flags);
	io = md->requeue_list;
	md->requeue_list = NULL;
	spin_unlock_irqrestore(&md->deferred_lock, flags);

	while (io) {
		struct dm_io *next = io->next;

		dm_io_rewind(io, &md->disk->bio_split);

		io->next = NULL;
		__dm_io_complete(io, false);
		io = next;
		cond_resched();
	}
}

/*
 * Two staged requeue:
 *
 * 1) io->orig_bio points to the real original bio, and the part mapped to
 *    this io must be requeued, instead of other parts of the original bio.
 *
 * 2) io->orig_bio points to new cloned bio which matches the requeued dm_io.
 */
static void dm_io_complete(struct dm_io *io)
{
	bool first_requeue;

	/*
	 * Only dm_io that has been split needs two stage requeue, otherwise
	 * we may run into long bio clone chain during suspend and OOM could
	 * be triggered.
	 *
	 * Also flush data dm_io won't be marked as DM_IO_WAS_SPLIT, so they
	 * also aren't handled via the first stage requeue.
	 */
	if (dm_io_flagged(io, DM_IO_WAS_SPLIT))
		first_requeue = true;
	else
		first_requeue = false;

	__dm_io_complete(io, first_requeue);
}

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
/*
 * Decrements the number of outstanding ios that a bio has been
 * cloned into, completing the original io if necc.
 */
<<<<<<< HEAD
static void dec_pending(struct dm_io *io, int error)
{
	unsigned long flags;
	int io_error;
	struct bio *bio;
	struct mapped_device *md = io->md;

	/* Push-back supersedes any I/O errors */
	if (unlikely(error)) {
		spin_lock_irqsave(&io->endio_lock, flags);
		if (!(io->error > 0 && __noflush_suspending(md)))
			io->error = error;
		spin_unlock_irqrestore(&io->endio_lock, flags);
	}

	if (atomic_dec_and_test(&io->io_count)) {
		if (io->error == DM_ENDIO_REQUEUE) {
			/*
			 * Target requested pushing back the I/O.
			 */
			spin_lock_irqsave(&md->deferred_lock, flags);
			if (__noflush_suspending(md))
				bio_list_add_head(&md->deferred, io->bio);
			else
				/* noflush suspend was interrupted. */
				io->error = -EIO;
			spin_unlock_irqrestore(&md->deferred_lock, flags);
		}

		io_error = io->error;
		bio = io->bio;
		end_io_acct(io);
		free_io(md, io);

		if (io_error == DM_ENDIO_REQUEUE)
			return;

		if ((bio->bi_rw & REQ_FLUSH) && bio->bi_size) {
			/*
			 * Preflush done for flush with data, reissue
			 * without REQ_FLUSH.
			 */
			bio->bi_rw &= ~REQ_FLUSH;
			queue_io(md, bio);
		} else {
			/* done with normal IO or empty flush */
			trace_block_bio_complete(md->queue, bio, io_error);
			bio_endio(bio, io_error);
		}
	}
}

static void clone_endio(struct bio *bio, int error)
{
	int r = 0;
	struct dm_target_io *tio = bio->bi_private;
	struct dm_io *io = tio->io;
	struct mapped_device *md = tio->io->md;
	dm_endio_fn endio = tio->ti->type->end_io;

	if (!bio_flagged(bio, BIO_UPTODATE) && !error)
		error = -EIO;

	if (endio) {
		r = endio(tio->ti, bio, error, &tio->info);
		if (r < 0 || r == DM_ENDIO_REQUEUE)
			/*
			 * error and requeue request are handled
			 * in dec_pending().
			 */
			error = r;
		else if (r == DM_ENDIO_INCOMPLETE)
			/* The target will handle the io */
			return;
		else if (r) {
			DMWARN("unimplemented target endio return value: %d", r);
=======
static inline void __dm_io_dec_pending(struct dm_io *io)
{
	if (atomic_dec_and_test(&io->io_count))
		dm_io_complete(io);
}

static void dm_io_set_error(struct dm_io *io, blk_status_t error)
{
	unsigned long flags;

	/* Push-back supersedes any I/O errors */
	spin_lock_irqsave(&io->lock, flags);
	if (!(io->status == BLK_STS_DM_REQUEUE &&
	      __noflush_suspending(io->md))) {
		io->status = error;
	}
	spin_unlock_irqrestore(&io->lock, flags);
}

static void dm_io_dec_pending(struct dm_io *io, blk_status_t error)
{
	if (unlikely(error))
		dm_io_set_error(io, error);

	__dm_io_dec_pending(io);
}

/*
 * The queue_limits are only valid as long as you have a reference
 * count on 'md'. But _not_ imposing verification to avoid atomic_read(),
 */
static inline struct queue_limits *dm_get_queue_limits(struct mapped_device *md)
{
	return &md->queue->limits;
}

void disable_discard(struct mapped_device *md)
{
	struct queue_limits *limits = dm_get_queue_limits(md);

	/* device doesn't really support DISCARD, disable it */
	limits->max_discard_sectors = 0;
}

void disable_write_zeroes(struct mapped_device *md)
{
	struct queue_limits *limits = dm_get_queue_limits(md);

	/* device doesn't really support WRITE ZEROES, disable it */
	limits->max_write_zeroes_sectors = 0;
}

static bool swap_bios_limit(struct dm_target *ti, struct bio *bio)
{
	return unlikely((bio->bi_opf & REQ_SWAP) != 0) && unlikely(ti->limit_swap_bios);
}

static void clone_endio(struct bio *bio)
{
	blk_status_t error = bio->bi_status;
	struct dm_target_io *tio = clone_to_tio(bio);
	struct dm_target *ti = tio->ti;
	dm_endio_fn endio = ti->type->end_io;
	struct dm_io *io = tio->io;
	struct mapped_device *md = io->md;

	if (unlikely(error == BLK_STS_TARGET)) {
		if (bio_op(bio) == REQ_OP_DISCARD &&
		    !bdev_max_discard_sectors(bio->bi_bdev))
			disable_discard(md);
		else if (bio_op(bio) == REQ_OP_WRITE_ZEROES &&
			 !bdev_write_zeroes_sectors(bio->bi_bdev))
			disable_write_zeroes(md);
	}

	if (static_branch_unlikely(&zoned_enabled) &&
	    unlikely(bdev_is_zoned(bio->bi_bdev)))
		dm_zone_endio(io, bio);

	if (endio) {
		int r = endio(ti, bio, &error);

		switch (r) {
		case DM_ENDIO_REQUEUE:
			if (static_branch_unlikely(&zoned_enabled)) {
				/*
				 * Requeuing writes to a sequential zone of a zoned
				 * target will break the sequential write pattern:
				 * fail such IO.
				 */
				if (WARN_ON_ONCE(dm_is_zone_write(md, bio)))
					error = BLK_STS_IOERR;
				else
					error = BLK_STS_DM_REQUEUE;
			} else
				error = BLK_STS_DM_REQUEUE;
			fallthrough;
		case DM_ENDIO_DONE:
			break;
		case DM_ENDIO_INCOMPLETE:
			/* The target will handle the io */
			return;
		default:
			DMCRIT("unimplemented target endio return value: %d", r);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
			BUG();
		}
	}

<<<<<<< HEAD
	/*
	 * Store md for cleanup instead of tio which is about to get freed.
	 */
	bio->bi_private = md->bs;

	free_tio(md, tio);
	bio_put(bio);
	dec_pending(io, error);
}

/*
 * Partial completion handling for request-based dm
 */
static void end_clone_bio(struct bio *clone, int error)
{
	struct dm_rq_clone_bio_info *info = clone->bi_private;
	struct dm_rq_target_io *tio = info->tio;
	struct bio *bio = info->orig;
	unsigned int nr_bytes = info->orig->bi_size;

	bio_put(clone);

	if (tio->error)
		/*
		 * An error has already been detected on the request.
		 * Once error occurred, just let clone->end_io() handle
		 * the remainder.
		 */
		return;
	else if (error) {
		/*
		 * Don't notice the error to the upper layer yet.
		 * The error handling decision is made by the target driver,
		 * when the request is completed.
		 */
		tio->error = error;
		return;
	}

	/*
	 * I/O for the bio successfully completed.
	 * Notice the data completion to the upper layer.
	 */

	/*
	 * bios are processed from the head of the list.
	 * So the completing bio should always be rq->bio.
	 * If it's not, something wrong is happening.
	 */
	if (tio->orig->bio != bio)
		DMERR("bio completion is going in the middle of the request");

	/*
	 * Update the original request.
	 * Do not use blk_end_request() here, because it may complete
	 * the original request before the clone, and break the ordering.
	 */
	blk_update_request(tio->orig, 0, nr_bytes);
}

/*
 * Don't touch any member of the md after calling this function because
 * the md may be freed in dm_put() at the end of this function.
 * Or do dm_get() before calling this function and dm_put() later.
 */
static void rq_completed(struct mapped_device *md, int rw, int run_queue)
{
	atomic_dec(&md->pending[rw]);

	/* nudge anyone waiting on suspend queue */
	if (!md_in_flight(md))
		wake_up(&md->wait);

	/*
	 * Run this off this callpath, as drivers could invoke end_io while
	 * inside their request_fn (and holding the queue lock). Calling
	 * back into ->request_fn() could deadlock attempting to grab the
	 * queue lock again.
	 */
	if (run_queue)
		blk_run_queue_async(md->queue);

	/*
	 * dm_put() must be at the end of this function. See the comment above
	 */
	dm_put(md);
}

static void free_rq_clone(struct request *clone)
{
	struct dm_rq_target_io *tio = clone->end_io_data;

	blk_rq_unprep_clone(clone);
	free_rq_tio(tio);
}

/*
 * Complete the clone and the original request.
 * Must be called without queue lock.
 */
void dm_end_request(struct request *clone, int error)
{
	int rw = rq_data_dir(clone);
	struct dm_rq_target_io *tio = clone->end_io_data;
	struct mapped_device *md = tio->md;
	struct request *rq = tio->orig;

	if (rq->cmd_type == REQ_TYPE_BLOCK_PC) {
		rq->errors = clone->errors;
		rq->resid_len = clone->resid_len;

		if (rq->sense)
			/*
			 * We are using the sense buffer of the original
			 * request.
			 * So setting the length of the sense data is enough.
			 */
			rq->sense_len = clone->sense_len;
	}

	free_rq_clone(clone);
	blk_end_request_all(rq, error);
	rq_completed(md, rw, true);
}

static void dm_unprep_request(struct request *rq)
{
	struct request *clone = rq->special;

	rq->special = NULL;
	rq->cmd_flags &= ~REQ_DONTPREP;

	free_rq_clone(clone);
}

/*
 * Requeue the original request of a clone.
 */
void dm_requeue_unmapped_request(struct request *clone)
{
	int rw = rq_data_dir(clone);
	struct dm_rq_target_io *tio = clone->end_io_data;
	struct mapped_device *md = tio->md;
	struct request *rq = tio->orig;
	struct request_queue *q = rq->q;
	unsigned long flags;

	dm_unprep_request(rq);

	spin_lock_irqsave(q->queue_lock, flags);
	blk_requeue_request(q, rq);
	spin_unlock_irqrestore(q->queue_lock, flags);

	rq_completed(md, rw, 0);
}
EXPORT_SYMBOL_GPL(dm_requeue_unmapped_request);

static void __stop_queue(struct request_queue *q)
{
	blk_stop_queue(q);
}

static void stop_queue(struct request_queue *q)
{
	unsigned long flags;

	spin_lock_irqsave(q->queue_lock, flags);
	__stop_queue(q);
	spin_unlock_irqrestore(q->queue_lock, flags);
}

static void __start_queue(struct request_queue *q)
{
	if (blk_queue_stopped(q))
		blk_start_queue(q);
}

static void start_queue(struct request_queue *q)
{
	unsigned long flags;

	spin_lock_irqsave(q->queue_lock, flags);
	__start_queue(q);
	spin_unlock_irqrestore(q->queue_lock, flags);
}

static void dm_done(struct request *clone, int error, bool mapped)
{
	int r = error;
	struct dm_rq_target_io *tio = clone->end_io_data;
	dm_request_endio_fn rq_end_io = NULL;

	if (tio->ti) {
		rq_end_io = tio->ti->type->rq_end_io;

		if (mapped && rq_end_io)
			r = rq_end_io(tio->ti, clone, error, &tio->info);
	}

	if (r <= 0)
		/* The target wants to complete the I/O */
		dm_end_request(clone, r);
	else if (r == DM_ENDIO_INCOMPLETE)
		/* The target will handle the I/O */
		return;
	else if (r == DM_ENDIO_REQUEUE)
		/* The target wants to requeue the I/O */
		dm_requeue_unmapped_request(clone);
	else {
		DMWARN("unimplemented target endio return value: %d", r);
		BUG();
	}
}

/*
 * Request completion handler for request-based dm
 */
static void dm_softirq_done(struct request *rq)
{
	bool mapped = true;
	struct request *clone = rq->completion_data;
	struct dm_rq_target_io *tio = clone->end_io_data;

	if (rq->cmd_flags & REQ_FAILED)
		mapped = false;

	dm_done(clone, tio->error, mapped);
}

/*
 * Complete the clone and the original request with the error status
 * through softirq context.
 */
static void dm_complete_request(struct request *clone, int error)
{
	struct dm_rq_target_io *tio = clone->end_io_data;
	struct request *rq = tio->orig;

	tio->error = error;
	rq->completion_data = clone;
	blk_complete_request(rq);
}

/*
 * Complete the not-mapped clone and the original request with the error status
 * through softirq context.
 * Target's rq_end_io() function isn't called.
 * This may be used when the target's map_rq() function fails.
 */
void dm_kill_unmapped_request(struct request *clone, int error)
{
	struct dm_rq_target_io *tio = clone->end_io_data;
	struct request *rq = tio->orig;

	rq->cmd_flags |= REQ_FAILED;
	dm_complete_request(clone, error);
}
EXPORT_SYMBOL_GPL(dm_kill_unmapped_request);

/*
 * Called with the queue lock held
 */
static void end_clone_request(struct request *clone, int error)
{
	/*
	 * For just cleaning up the information of the queue in which
	 * the clone was dispatched.
	 * The clone is *NOT* freed actually here because it is alloced from
	 * dm own mempool and REQ_ALLOCED isn't set in clone->cmd_flags.
	 */
	__blk_put_request(clone->q, clone);

	/*
	 * Actual request completion is done in a softirq context which doesn't
	 * hold the queue lock.  Otherwise, deadlock could occur because:
	 *     - another request may be submitted by the upper level driver
	 *       of the stacking during the completion
	 *     - the submission which requires queue lock may be done
	 *       against this queue
	 */
	dm_complete_request(clone, error);
=======
	if (static_branch_unlikely(&swap_bios_enabled) &&
	    unlikely(swap_bios_limit(ti, bio)))
		up(&md->swap_bios_semaphore);

	free_tio(bio);
	dm_io_dec_pending(io, error);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

/*
 * Return maximum size of I/O possible at the supplied sector up to the current
 * target boundary.
 */
<<<<<<< HEAD
static sector_t max_io_len_target_boundary(sector_t sector, struct dm_target *ti)
{
	sector_t target_offset = dm_target_offset(ti, sector);

	return ti->len - target_offset;
}

static sector_t max_io_len(sector_t sector, struct dm_target *ti)
{
	sector_t len = max_io_len_target_boundary(sector, ti);

	/*
	 * Does the target need to split even further ?
	 */
	if (ti->split_io) {
		sector_t boundary;
		sector_t offset = dm_target_offset(ti, sector);
		boundary = ((offset + ti->split_io) & ~(ti->split_io - 1))
			   - offset;
		if (len > boundary)
			len = boundary;
	}

	return len;
}

static void __map_bio(struct dm_target *ti, struct bio *clone,
		      struct dm_target_io *tio)
{
	int r;
	sector_t sector;
	struct mapped_device *md;

	clone->bi_end_io = clone_endio;
	clone->bi_private = tio;

	/*
	 * Map the clone.  If r == 0 we don't need to do
	 * anything, the target has assumed ownership of
	 * this io.
	 */
	atomic_inc(&tio->io->io_count);
	sector = clone->bi_sector;
	r = ti->type->map(ti, clone, &tio->info);
	if (r == DM_MAPIO_REMAPPED) {
		/* the bio has been remapped so dispatch it */

		trace_block_bio_remap(bdev_get_queue(clone->bi_bdev), clone,
				      tio->io->bio->bi_bdev->bd_dev, sector);

		generic_make_request(clone);
	} else if (r < 0 || r == DM_MAPIO_REQUEUE) {
		/* error the io and bail out, or requeue it if needed */
		md = tio->io->md;
		dec_pending(tio->io, r);
		/*
		 * Store bio_set for cleanup.
		 */
		clone->bi_end_io = NULL;
		clone->bi_private = md->bs;
		bio_put(clone);
		free_tio(md, tio);
	} else if (r) {
		DMWARN("unimplemented target map return value: %d", r);
=======
static inline sector_t max_io_len_target_boundary(struct dm_target *ti,
						  sector_t target_offset)
{
	return ti->len - target_offset;
}

static sector_t __max_io_len(struct dm_target *ti, sector_t sector,
			     unsigned int max_granularity,
			     unsigned int max_sectors)
{
	sector_t target_offset = dm_target_offset(ti, sector);
	sector_t len = max_io_len_target_boundary(ti, target_offset);

	/*
	 * Does the target need to split IO even further?
	 * - varied (per target) IO splitting is a tenet of DM; this
	 *   explains why stacked chunk_sectors based splitting via
	 *   bio_split_to_limits() isn't possible here.
	 */
	if (!max_granularity)
		return len;
	return min_t(sector_t, len,
		min(max_sectors ? : queue_max_sectors(ti->table->md->queue),
		    blk_chunk_sectors_left(target_offset, max_granularity)));
}

static inline sector_t max_io_len(struct dm_target *ti, sector_t sector)
{
	return __max_io_len(ti, sector, ti->max_io_len, 0);
}

int dm_set_target_max_io_len(struct dm_target *ti, sector_t len)
{
	if (len > UINT_MAX) {
		DMERR("Specified maximum size of target IO (%llu) exceeds limit (%u)",
		      (unsigned long long)len, UINT_MAX);
		ti->error = "Maximum size of target IO is too large";
		return -EINVAL;
	}

	ti->max_io_len = (uint32_t) len;

	return 0;
}
EXPORT_SYMBOL_GPL(dm_set_target_max_io_len);

static struct dm_target *dm_dax_get_live_target(struct mapped_device *md,
						sector_t sector, int *srcu_idx)
	__acquires(md->io_barrier)
{
	struct dm_table *map;
	struct dm_target *ti;

	map = dm_get_live_table(md, srcu_idx);
	if (!map)
		return NULL;

	ti = dm_table_find_target(map, sector);
	if (!ti)
		return NULL;

	return ti;
}

static long dm_dax_direct_access(struct dax_device *dax_dev, pgoff_t pgoff,
		long nr_pages, enum dax_access_mode mode, void **kaddr,
		pfn_t *pfn)
{
	struct mapped_device *md = dax_get_private(dax_dev);
	sector_t sector = pgoff * PAGE_SECTORS;
	struct dm_target *ti;
	long len, ret = -EIO;
	int srcu_idx;

	ti = dm_dax_get_live_target(md, sector, &srcu_idx);

	if (!ti)
		goto out;
	if (!ti->type->direct_access)
		goto out;
	len = max_io_len(ti, sector) / PAGE_SECTORS;
	if (len < 1)
		goto out;
	nr_pages = min(len, nr_pages);
	ret = ti->type->direct_access(ti, pgoff, nr_pages, mode, kaddr, pfn);

 out:
	dm_put_live_table(md, srcu_idx);

	return ret;
}

static int dm_dax_zero_page_range(struct dax_device *dax_dev, pgoff_t pgoff,
				  size_t nr_pages)
{
	struct mapped_device *md = dax_get_private(dax_dev);
	sector_t sector = pgoff * PAGE_SECTORS;
	struct dm_target *ti;
	int ret = -EIO;
	int srcu_idx;

	ti = dm_dax_get_live_target(md, sector, &srcu_idx);

	if (!ti)
		goto out;
	if (WARN_ON(!ti->type->dax_zero_page_range)) {
		/*
		 * ->zero_page_range() is mandatory dax operation. If we are
		 *  here, something is wrong.
		 */
		goto out;
	}
	ret = ti->type->dax_zero_page_range(ti, pgoff, nr_pages);
 out:
	dm_put_live_table(md, srcu_idx);

	return ret;
}

static size_t dm_dax_recovery_write(struct dax_device *dax_dev, pgoff_t pgoff,
		void *addr, size_t bytes, struct iov_iter *i)
{
	struct mapped_device *md = dax_get_private(dax_dev);
	sector_t sector = pgoff * PAGE_SECTORS;
	struct dm_target *ti;
	int srcu_idx;
	long ret = 0;

	ti = dm_dax_get_live_target(md, sector, &srcu_idx);
	if (!ti || !ti->type->dax_recovery_write)
		goto out;

	ret = ti->type->dax_recovery_write(ti, pgoff, addr, bytes, i);
out:
	dm_put_live_table(md, srcu_idx);
	return ret;
}

/*
 * A target may call dm_accept_partial_bio only from the map routine.  It is
 * allowed for all bio types except REQ_PREFLUSH, REQ_OP_ZONE_* zone management
 * operations, REQ_OP_ZONE_APPEND (zone append writes) and any bio serviced by
 * __send_duplicate_bios().
 *
 * dm_accept_partial_bio informs the dm that the target only wants to process
 * additional n_sectors sectors of the bio and the rest of the data should be
 * sent in a next bio.
 *
 * A diagram that explains the arithmetics:
 * +--------------------+---------------+-------+
 * |         1          |       2       |   3   |
 * +--------------------+---------------+-------+
 *
 * <-------------- *tio->len_ptr --------------->
 *                      <----- bio_sectors ----->
 *                      <-- n_sectors -->
 *
 * Region 1 was already iterated over with bio_advance or similar function.
 *	(it may be empty if the target doesn't use bio_advance)
 * Region 2 is the remaining bio size that the target wants to process.
 *	(it may be empty if region 1 is non-empty, although there is no reason
 *	 to make it empty)
 * The target requires that region 3 is to be sent in the next bio.
 *
 * If the target wants to receive multiple copies of the bio (via num_*bios, etc),
 * the partially processed part (the sum of regions 1+2) must be the same for all
 * copies of the bio.
 */
void dm_accept_partial_bio(struct bio *bio, unsigned int n_sectors)
{
	struct dm_target_io *tio = clone_to_tio(bio);
	struct dm_io *io = tio->io;
	unsigned int bio_sectors = bio_sectors(bio);

	BUG_ON(dm_tio_flagged(tio, DM_TIO_IS_DUPLICATE_BIO));
	BUG_ON(op_is_zone_mgmt(bio_op(bio)));
	BUG_ON(bio_op(bio) == REQ_OP_ZONE_APPEND);
	BUG_ON(bio_sectors > *tio->len_ptr);
	BUG_ON(n_sectors > bio_sectors);

	*tio->len_ptr -= bio_sectors - n_sectors;
	bio->bi_iter.bi_size = n_sectors << SECTOR_SHIFT;

	/*
	 * __split_and_process_bio() may have already saved mapped part
	 * for accounting but it is being reduced so update accordingly.
	 */
	dm_io_set_flag(io, DM_IO_WAS_SPLIT);
	io->sectors = n_sectors;
	io->sector_offset = bio_sectors(io->orig_bio);
}
EXPORT_SYMBOL_GPL(dm_accept_partial_bio);

/*
 * @clone: clone bio that DM core passed to target's .map function
 * @tgt_clone: clone of @clone bio that target needs submitted
 *
 * Targets should use this interface to submit bios they take
 * ownership of when returning DM_MAPIO_SUBMITTED.
 *
 * Target should also enable ti->accounts_remapped_io
 */
void dm_submit_bio_remap(struct bio *clone, struct bio *tgt_clone)
{
	struct dm_target_io *tio = clone_to_tio(clone);
	struct dm_io *io = tio->io;

	/* establish bio that will get submitted */
	if (!tgt_clone)
		tgt_clone = clone;

	/*
	 * Account io->origin_bio to DM dev on behalf of target
	 * that took ownership of IO with DM_MAPIO_SUBMITTED.
	 */
	dm_start_io_acct(io, clone);

	trace_block_bio_remap(tgt_clone, disk_devt(io->md->disk),
			      tio->old_sector);
	submit_bio_noacct(tgt_clone);
}
EXPORT_SYMBOL_GPL(dm_submit_bio_remap);

static noinline void __set_swap_bios_limit(struct mapped_device *md, int latch)
{
	mutex_lock(&md->swap_bios_lock);
	while (latch < md->swap_bios) {
		cond_resched();
		down(&md->swap_bios_semaphore);
		md->swap_bios--;
	}
	while (latch > md->swap_bios) {
		cond_resched();
		up(&md->swap_bios_semaphore);
		md->swap_bios++;
	}
	mutex_unlock(&md->swap_bios_lock);
}

static void __map_bio(struct bio *clone)
{
	struct dm_target_io *tio = clone_to_tio(clone);
	struct dm_target *ti = tio->ti;
	struct dm_io *io = tio->io;
	struct mapped_device *md = io->md;
	int r;

	clone->bi_end_io = clone_endio;

	/*
	 * Map the clone.
	 */
	tio->old_sector = clone->bi_iter.bi_sector;

	if (static_branch_unlikely(&swap_bios_enabled) &&
	    unlikely(swap_bios_limit(ti, clone))) {
		int latch = get_swap_bios();

		if (unlikely(latch != md->swap_bios))
			__set_swap_bios_limit(md, latch);
		down(&md->swap_bios_semaphore);
	}

	if (static_branch_unlikely(&zoned_enabled)) {
		/*
		 * Check if the IO needs a special mapping due to zone append
		 * emulation on zoned target. In this case, dm_zone_map_bio()
		 * calls the target map operation.
		 */
		if (unlikely(dm_emulate_zone_append(md)))
			r = dm_zone_map_bio(tio);
		else
			goto do_map;
	} else {
do_map:
		if (likely(ti->type->map == linear_map))
			r = linear_map(ti, clone);
		else if (ti->type->map == stripe_map)
			r = stripe_map(ti, clone);
		else
			r = ti->type->map(ti, clone);
	}

	switch (r) {
	case DM_MAPIO_SUBMITTED:
		/* target has assumed ownership of this io */
		if (!ti->accounts_remapped_io)
			dm_start_io_acct(io, clone);
		break;
	case DM_MAPIO_REMAPPED:
		dm_submit_bio_remap(clone, NULL);
		break;
	case DM_MAPIO_KILL:
	case DM_MAPIO_REQUEUE:
		if (static_branch_unlikely(&swap_bios_enabled) &&
		    unlikely(swap_bios_limit(ti, clone)))
			up(&md->swap_bios_semaphore);
		free_tio(clone);
		if (r == DM_MAPIO_KILL)
			dm_io_dec_pending(io, BLK_STS_IOERR);
		else
			dm_io_dec_pending(io, BLK_STS_DM_REQUEUE);
		break;
	default:
		DMCRIT("unimplemented target map return value: %d", r);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		BUG();
	}
}

<<<<<<< HEAD
struct clone_info {
	struct mapped_device *md;
	struct dm_table *map;
	struct bio *bio;
	struct dm_io *io;
	sector_t sector;
	sector_t sector_count;
	unsigned short idx;
};

static void dm_bio_destructor(struct bio *bio)
{
	struct bio_set *bs = bio->bi_private;

	bio_free(bio, bs);
}

/*
 * Creates a little bio that just does part of a bvec.
 */
static struct bio *split_bvec(struct bio *bio, sector_t sector,
			      unsigned short idx, unsigned int offset,
			      unsigned int len, struct bio_set *bs)
{
	struct bio *clone;
	struct bio_vec *bv = bio->bi_io_vec + idx;

	clone = bio_alloc_bioset(GFP_NOIO, 1, bs);
	clone->bi_destructor = dm_bio_destructor;
	*clone->bi_io_vec = *bv;

	clone->bi_sector = sector;
	clone->bi_bdev = bio->bi_bdev;
	clone->bi_rw = bio->bi_rw;
	clone->bi_vcnt = 1;
	clone->bi_size = to_bytes(len);
	clone->bi_io_vec->bv_offset = offset;
	clone->bi_io_vec->bv_len = clone->bi_size;
	clone->bi_flags |= 1 << BIO_CLONED;

	if (bio_integrity(bio)) {
		bio_integrity_clone(clone, bio, GFP_NOIO, bs);
		bio_integrity_trim(clone,
				   bio_sector_offset(bio, idx, offset), len);
	}

	return clone;
}

/*
 * Creates a bio that consists of range of complete bvecs.
 */
static struct bio *clone_bio(struct bio *bio, sector_t sector,
			     unsigned short idx, unsigned short bv_count,
			     unsigned int len, struct bio_set *bs)
{
	struct bio *clone;

	clone = bio_alloc_bioset(GFP_NOIO, bio->bi_max_vecs, bs);
	__bio_clone(clone, bio);
	clone->bi_destructor = dm_bio_destructor;
	clone->bi_sector = sector;
	clone->bi_idx = idx;
	clone->bi_vcnt = idx + bv_count;
	clone->bi_size = to_bytes(len);
	clone->bi_flags &= ~(1 << BIO_SEG_VALID);

	if (bio_integrity(bio)) {
		bio_integrity_clone(clone, bio, GFP_NOIO, bs);

		if (idx != bio->bi_idx || clone->bi_size < bio->bi_size)
			bio_integrity_trim(clone,
					   bio_sector_offset(bio, idx, 0), len);
	}

	return clone;
}

static struct dm_target_io *alloc_tio(struct clone_info *ci,
				      struct dm_target *ti)
{
	struct dm_target_io *tio = mempool_alloc(ci->md->tio_pool, GFP_NOIO);

	tio->io = ci->io;
	tio->ti = ti;
	memset(&tio->info, 0, sizeof(tio->info));

	return tio;
}

static void __issue_target_request(struct clone_info *ci, struct dm_target *ti,
				   unsigned request_nr, sector_t len)
{
	struct dm_target_io *tio = alloc_tio(ci, ti);
	struct bio *clone;

	tio->info.target_request_nr = request_nr;

	/*
	 * Discard requests require the bio's inline iovecs be initialized.
	 * ci->bio->bi_max_vecs is BIO_INLINE_VECS anyway, for both flush
	 * and discard, so no need for concern about wasted bvec allocations.
	 */
	clone = bio_alloc_bioset(GFP_NOIO, ci->bio->bi_max_vecs, ci->md->bs);
	__bio_clone(clone, ci->bio);
	clone->bi_destructor = dm_bio_destructor;
	if (len) {
		clone->bi_sector = ci->sector;
		clone->bi_size = to_bytes(len);
	}

	__map_bio(ti, clone, tio);
}

static void __issue_target_requests(struct clone_info *ci, struct dm_target *ti,
				    unsigned num_requests, sector_t len)
{
	unsigned request_nr;

	for (request_nr = 0; request_nr < num_requests; request_nr++)
		__issue_target_request(ci, ti, request_nr, len);
}

static int __clone_and_map_empty_flush(struct clone_info *ci)
{
	unsigned target_nr = 0;
	struct dm_target *ti;

	BUG_ON(bio_has_data(ci->bio));
	while ((ti = dm_table_get_target(ci->map, target_nr++)))
		__issue_target_requests(ci, ti, ti->num_flush_requests, 0);

	return 0;
}

/*
 * Perform all io with a single clone.
 */
static void __clone_and_map_simple(struct clone_info *ci, struct dm_target *ti)
{
	struct bio *clone, *bio = ci->bio;
	struct dm_target_io *tio;

	tio = alloc_tio(ci, ti);
	clone = clone_bio(bio, ci->sector, ci->idx,
			  bio->bi_vcnt - ci->idx, ci->sector_count,
			  ci->md->bs);
	__map_bio(ti, clone, tio);
	ci->sector_count = 0;
}

static int __clone_and_map_discard(struct clone_info *ci)
{
	struct dm_target *ti;
	sector_t len;

	do {
		ti = dm_table_find_target(ci->map, ci->sector);
		if (!dm_target_is_valid(ti))
			return -EIO;

		/*
		 * Even though the device advertised discard support,
		 * that does not mean every target supports it, and
		 * reconfiguration might also have changed that since the
		 * check was performed.
		 */
		if (!ti->num_discard_requests)
			return -EOPNOTSUPP;

		len = min(ci->sector_count, max_io_len_target_boundary(ci->sector, ti));

		__issue_target_requests(ci, ti, ti->num_discard_requests, len);

		ci->sector += len;
	} while (ci->sector_count -= len);

	return 0;
}

static int __clone_and_map(struct clone_info *ci)
{
	struct bio *clone, *bio = ci->bio;
	struct dm_target *ti;
	sector_t len = 0, max;
	struct dm_target_io *tio;

	if (unlikely(bio->bi_rw & REQ_DISCARD))
		return __clone_and_map_discard(ci);

	ti = dm_table_find_target(ci->map, ci->sector);
	if (!dm_target_is_valid(ti))
		return -EIO;

	max = max_io_len(ci->sector, ti);

	if (ci->sector_count <= max) {
		/*
		 * Optimise for the simple case where we can do all of
		 * the remaining io with a single clone.
		 */
		__clone_and_map_simple(ci, ti);

	} else if (to_sector(bio->bi_io_vec[ci->idx].bv_len) <= max) {
		/*
		 * There are some bvecs that don't span targets.
		 * Do as many of these as possible.
		 */
		int i;
		sector_t remaining = max;
		sector_t bv_len;

		for (i = ci->idx; remaining && (i < bio->bi_vcnt); i++) {
			bv_len = to_sector(bio->bi_io_vec[i].bv_len);

			if (bv_len > remaining)
				break;

			remaining -= bv_len;
			len += bv_len;
		}

		tio = alloc_tio(ci, ti);
		clone = clone_bio(bio, ci->sector, ci->idx, i - ci->idx, len,
				  ci->md->bs);
		__map_bio(ti, clone, tio);

		ci->sector += len;
		ci->sector_count -= len;
		ci->idx = i;

	} else {
		/*
		 * Handle a bvec that must be split between two or more targets.
		 */
		struct bio_vec *bv = bio->bi_io_vec + ci->idx;
		sector_t remaining = to_sector(bv->bv_len);
		unsigned int offset = 0;

		do {
			if (offset) {
				ti = dm_table_find_target(ci->map, ci->sector);
				if (!dm_target_is_valid(ti))
					return -EIO;

				max = max_io_len(ci->sector, ti);
			}

			len = min(remaining, max);

			tio = alloc_tio(ci, ti);
			clone = split_bvec(bio, ci->sector, ci->idx,
					   bv->bv_offset + offset, len,
					   ci->md->bs);

			__map_bio(ti, clone, tio);

			ci->sector += len;
			ci->sector_count -= len;
			offset += to_bytes(len);
		} while (remaining -= len);

		ci->idx++;
	}

	return 0;
}

/*
 * Split the bio into several clones and submit it to targets.
 */
static void __split_and_process_bio(struct mapped_device *md, struct bio *bio)
{
	struct clone_info ci;
	int error = 0;

	ci.map = dm_get_live_table(md);
	if (unlikely(!ci.map)) {
		bio_io_error(bio);
		return;
	}

	ci.md = md;
	ci.io = alloc_io(md);
	ci.io->error = 0;
	atomic_set(&ci.io->io_count, 1);
	ci.io->bio = bio;
	ci.io->md = md;
	spin_lock_init(&ci.io->endio_lock);
	ci.sector = bio->bi_sector;
	ci.idx = bio->bi_idx;

	start_io_acct(ci.io);
	if (bio->bi_rw & REQ_FLUSH) {
		ci.bio = &ci.md->flush_bio;
		ci.sector_count = 0;
		error = __clone_and_map_empty_flush(&ci);
		/* dec_pending submits any data associated with flush */
	} else {
		ci.bio = bio;
		ci.sector_count = bio_sectors(bio);
		while (ci.sector_count && !error)
			error = __clone_and_map(&ci);
	}

	/* drop the extra reference count */
	dec_pending(ci.io, error);
	dm_table_put(ci.map);
}
/*-----------------------------------------------------------------
 * CRUD END
 *---------------------------------------------------------------*/

static int dm_merge_bvec(struct request_queue *q,
			 struct bvec_merge_data *bvm,
			 struct bio_vec *biovec)
{
	struct mapped_device *md = q->queuedata;
	struct dm_table *map = dm_get_live_table(md);
	struct dm_target *ti;
	sector_t max_sectors;
	int max_size = 0;

	if (unlikely(!map))
		goto out;

	ti = dm_table_find_target(map, bvm->bi_sector);
	if (!dm_target_is_valid(ti))
		goto out_table;

	/*
	 * Find maximum amount of I/O that won't need splitting
	 */
	max_sectors = min(max_io_len(bvm->bi_sector, ti),
			  (sector_t) BIO_MAX_SECTORS);
	max_size = (max_sectors << SECTOR_SHIFT) - bvm->bi_size;
	if (max_size < 0)
		max_size = 0;

	/*
	 * merge_bvec_fn() returns number of bytes
	 * it can accept at this offset
	 * max is precomputed maximal io size
	 */
	if (max_size && ti->type->merge)
		max_size = ti->type->merge(ti, bvm, biovec, max_size);
	/*
	 * If the target doesn't support merge method and some of the devices
	 * provided their merge_bvec method (we know this by looking at
	 * queue_max_hw_sectors), then we can't allow bios with multiple vector
	 * entries.  So always set max_size to 0, and the code below allows
	 * just one page.
	 */
	else if (queue_max_hw_sectors(q) <= PAGE_SIZE >> 9)

		max_size = 0;

out_table:
	dm_table_put(map);

out:
	/*
	 * Always allow an entire first page
	 */
	if (max_size <= biovec->bv_len && !(bvm->bi_size >> SECTOR_SHIFT))
		max_size = biovec->bv_len;

	return max_size;
}

/*
 * The request function that just remaps the bio built up by
 * dm_merge_bvec.
 */
static void _dm_request(struct request_queue *q, struct bio *bio)
{
	int rw = bio_data_dir(bio);
	struct mapped_device *md = q->queuedata;
	int cpu;

	down_read(&md->io_lock);

	cpu = part_stat_lock();
	part_stat_inc(cpu, &dm_disk(md)->part0, ios[rw]);
	part_stat_add(cpu, &dm_disk(md)->part0, sectors[rw], bio_sectors(bio));
	part_stat_unlock();

	/* if we're suspended, we have to queue this io for later */
	if (unlikely(test_bit(DMF_BLOCK_IO_FOR_SUSPEND, &md->flags))) {
		up_read(&md->io_lock);

		if (bio_rw(bio) != READA)
			queue_io(md, bio);
		else
			bio_io_error(bio);
		return;
	}

	__split_and_process_bio(md, bio);
	up_read(&md->io_lock);
	return;
}

static int dm_request_based(struct mapped_device *md)
{
	return blk_queue_stackable(md->queue);
}

static void dm_request(struct request_queue *q, struct bio *bio)
{
	struct mapped_device *md = q->queuedata;

	if (dm_request_based(md))
		blk_queue_bio(q, bio);
	else
		_dm_request(q, bio);
}

void dm_dispatch_request(struct request *rq)
{
	int r;

	if (blk_queue_io_stat(rq->q))
		rq->cmd_flags |= REQ_IO_STAT;

	rq->start_time = jiffies;
	r = blk_insert_cloned_request(rq->q, rq);
	if (r)
		dm_complete_request(rq, r);
}
EXPORT_SYMBOL_GPL(dm_dispatch_request);

static void dm_rq_bio_destructor(struct bio *bio)
{
	struct dm_rq_clone_bio_info *info = bio->bi_private;
	struct mapped_device *md = info->tio->md;

	free_bio_info(info);
	bio_free(bio, md->bs);
}

static int dm_rq_bio_constructor(struct bio *bio, struct bio *bio_orig,
				 void *data)
{
	struct dm_rq_target_io *tio = data;
	struct mapped_device *md = tio->md;
	struct dm_rq_clone_bio_info *info = alloc_bio_info(md);

	if (!info)
		return -ENOMEM;

	info->orig = bio_orig;
	info->tio = tio;
	bio->bi_end_io = end_clone_bio;
	bio->bi_private = info;
	bio->bi_destructor = dm_rq_bio_destructor;

	return 0;
}

static int setup_clone(struct request *clone, struct request *rq,
		       struct dm_rq_target_io *tio)
{
	int r;

	r = blk_rq_prep_clone(clone, rq, tio->md->bs, GFP_ATOMIC,
			      dm_rq_bio_constructor, tio);
	if (r)
		return r;

	clone->cmd = rq->cmd;
	clone->cmd_len = rq->cmd_len;
	clone->sense = rq->sense;
	clone->buffer = rq->buffer;
	clone->end_io = end_clone_request;
	clone->end_io_data = tio;

	return 0;
}

static struct request *clone_rq(struct request *rq, struct mapped_device *md,
				gfp_t gfp_mask)
{
	struct request *clone;
	struct dm_rq_target_io *tio;

	tio = alloc_rq_tio(md, gfp_mask);
	if (!tio)
		return NULL;

	tio->md = md;
	tio->ti = NULL;
	tio->orig = rq;
	tio->error = 0;
	memset(&tio->info, 0, sizeof(tio->info));

	clone = &tio->clone;
	if (setup_clone(clone, rq, tio)) {
		/* -ENOMEM */
		free_rq_tio(tio);
		return NULL;
	}

	return clone;
}

/*
 * Called with the queue lock held.
 */
static int dm_prep_fn(struct request_queue *q, struct request *rq)
{
	struct mapped_device *md = q->queuedata;
	struct request *clone;

	if (unlikely(rq->special)) {
		DMWARN("Already has something in rq->special.");
		return BLKPREP_KILL;
	}

	clone = clone_rq(rq, md, GFP_ATOMIC);
	if (!clone)
		return BLKPREP_DEFER;

	rq->special = clone;
	rq->cmd_flags |= REQ_DONTPREP;

	return BLKPREP_OK;
}

/*
 * Returns:
 * 0  : the request has been processed (not requeued)
 * !0 : the request has been requeued
 */
static int map_request(struct dm_target *ti, struct request *clone,
		       struct mapped_device *md)
{
	int r, requeued = 0;
	struct dm_rq_target_io *tio = clone->end_io_data;

	tio->ti = ti;
	r = ti->type->map_rq(ti, clone, &tio->info);
	switch (r) {
	case DM_MAPIO_SUBMITTED:
		/* The target has taken the I/O to submit by itself later */
		break;
	case DM_MAPIO_REMAPPED:
		/* The target has remapped the I/O so dispatch it */
		trace_block_rq_remap(clone->q, clone, disk_devt(dm_disk(md)),
				     blk_rq_pos(tio->orig));
		dm_dispatch_request(clone);
		break;
	case DM_MAPIO_REQUEUE:
		/* The target wants to requeue the I/O */
		dm_requeue_unmapped_request(clone);
		requeued = 1;
		break;
	default:
		if (r > 0) {
			DMWARN("unimplemented target map return value: %d", r);
			BUG();
		}

		/* The target wants to complete the I/O */
		dm_kill_unmapped_request(clone, r);
		break;
	}

	return requeued;
}

static struct request *dm_start_request(struct mapped_device *md, struct request *orig)
{
	struct request *clone;

	blk_start_request(orig);
	clone = orig->special;
	atomic_inc(&md->pending[rq_data_dir(clone)]);

	/*
	 * Hold the md reference here for the in-flight I/O.
	 * We can't rely on the reference count by device opener,
	 * because the device may be closed during the request completion
	 * when all bios are completed.
	 * See the comment in rq_completed() too.
	 */
	dm_get(md);

	return clone;
}

/*
 * q->request_fn for request-based dm.
 * Called with the queue lock held.
 */
static void dm_request_fn(struct request_queue *q)
{
	struct mapped_device *md = q->queuedata;
	struct dm_table *map = dm_get_live_table(md);
	struct dm_target *ti;
	struct request *rq, *clone;
	sector_t pos;

	/*
	 * For suspend, check blk_queue_stopped() and increment
	 * ->pending within a single queue_lock not to increment the
	 * number of in-flight I/Os after the queue is stopped in
	 * dm_suspend().
	 */
	while (!blk_queue_stopped(q)) {
		rq = blk_peek_request(q);
		if (!rq)
			goto delay_and_out;

		/* always use block 0 to find the target for flushes for now */
		pos = 0;
		if (!(rq->cmd_flags & REQ_FLUSH))
			pos = blk_rq_pos(rq);

		ti = dm_table_find_target(map, pos);
		if (!dm_target_is_valid(ti)) {
			/*
			 * Must perform setup, that dm_done() requires,
			 * before calling dm_kill_unmapped_request
			 */
			DMERR_LIMIT("request attempted access beyond the end of device");
			clone = dm_start_request(md, rq);
			dm_kill_unmapped_request(clone, -EIO);
			continue;
		}

		if (ti->type->busy && ti->type->busy(ti))
			goto delay_and_out;

		clone = dm_start_request(md, rq);

		spin_unlock(q->queue_lock);
		if (map_request(ti, clone, md))
			goto requeued;

		BUG_ON(!irqs_disabled());
		spin_lock(q->queue_lock);
	}

	goto out;

requeued:
	BUG_ON(!irqs_disabled());
	spin_lock(q->queue_lock);

delay_and_out:
	blk_delay_queue(q, HZ / 10);
out:
	dm_table_put(map);
}

int dm_underlying_device_busy(struct request_queue *q)
{
	return blk_lld_busy(q);
}
EXPORT_SYMBOL_GPL(dm_underlying_device_busy);

static int dm_lld_busy(struct request_queue *q)
{
	int r;
	struct mapped_device *md = q->queuedata;
	struct dm_table *map = dm_get_live_table(md);

	if (!map || test_bit(DMF_BLOCK_IO_FOR_SUSPEND, &md->flags))
		r = 1;
	else
		r = dm_table_any_busy_target(map);

	dm_table_put(map);

	return r;
}

static int dm_any_congested(void *congested_data, int bdi_bits)
{
	int r = bdi_bits;
	struct mapped_device *md = congested_data;
	struct dm_table *map;

	if (!test_bit(DMF_BLOCK_IO_FOR_SUSPEND, &md->flags)) {
		map = dm_get_live_table(md);
		if (map) {
			/*
			 * Request-based dm cares about only own queue for
			 * the query about congestion status of request_queue
			 */
			if (dm_request_based(md))
				r = md->queue->backing_dev_info.state &
				    bdi_bits;
			else
				r = dm_table_any_congested(map, bdi_bits);

			dm_table_put(map);
		}
	}

	return r;
}

/*-----------------------------------------------------------------
 * An IDR is used to keep track of allocated minor numbers.
 *---------------------------------------------------------------*/
=======
static void setup_split_accounting(struct clone_info *ci, unsigned int len)
{
	struct dm_io *io = ci->io;

	if (ci->sector_count > len) {
		/*
		 * Split needed, save the mapped part for accounting.
		 * NOTE: dm_accept_partial_bio() will update accordingly.
		 */
		dm_io_set_flag(io, DM_IO_WAS_SPLIT);
		io->sectors = len;
		io->sector_offset = bio_sectors(ci->bio);
	}
}

static void alloc_multiple_bios(struct bio_list *blist, struct clone_info *ci,
				struct dm_target *ti, unsigned int num_bios,
				unsigned *len, gfp_t gfp_flag)
{
	struct bio *bio;
	int try = (gfp_flag & GFP_NOWAIT) ? 0 : 1;

	for (; try < 2; try++) {
		int bio_nr;

		if (try && num_bios > 1)
			mutex_lock(&ci->io->md->table_devices_lock);
		for (bio_nr = 0; bio_nr < num_bios; bio_nr++) {
			bio = alloc_tio(ci, ti, bio_nr, len,
					try ? GFP_NOIO : GFP_NOWAIT);
			if (!bio)
				break;

			bio_list_add(blist, bio);
		}
		if (try && num_bios > 1)
			mutex_unlock(&ci->io->md->table_devices_lock);
		if (bio_nr == num_bios)
			return;

		while ((bio = bio_list_pop(blist)))
			free_tio(bio);
	}
}

static unsigned int __send_duplicate_bios(struct clone_info *ci, struct dm_target *ti,
					  unsigned int num_bios, unsigned int *len,
					  gfp_t gfp_flag)
{
	struct bio_list blist = BIO_EMPTY_LIST;
	struct bio *clone;
	unsigned int ret = 0;

	if (WARN_ON_ONCE(num_bios == 0)) /* num_bios = 0 is a bug in caller */
		return 0;

	/* dm_accept_partial_bio() is not supported with shared tio->len_ptr */
	if (len)
		setup_split_accounting(ci, *len);

	/*
	 * Using alloc_multiple_bios(), even if num_bios is 1, to consistently
	 * support allocating using GFP_NOWAIT with GFP_NOIO fallback.
	 */
	alloc_multiple_bios(&blist, ci, ti, num_bios, len, gfp_flag);
	while ((clone = bio_list_pop(&blist))) {
		if (num_bios > 1)
			dm_tio_set_flag(clone_to_tio(clone), DM_TIO_IS_DUPLICATE_BIO);
		__map_bio(clone);
		ret += 1;
	}

	return ret;
}

static void __send_empty_flush(struct clone_info *ci)
{
	struct dm_table *t = ci->map;
	struct bio flush_bio;

	/*
	 * Use an on-stack bio for this, it's safe since we don't
	 * need to reference it after submit. It's just used as
	 * the basis for the clone(s).
	 */
	bio_init(&flush_bio, ci->io->md->disk->part0, NULL, 0,
		 REQ_OP_WRITE | REQ_PREFLUSH | REQ_SYNC);

	ci->bio = &flush_bio;
	ci->sector_count = 0;
	ci->io->tio.clone.bi_iter.bi_size = 0;

	for (unsigned int i = 0; i < t->num_targets; i++) {
		unsigned int bios;
		struct dm_target *ti = dm_table_get_target(t, i);

		if (unlikely(ti->num_flush_bios == 0))
			continue;

		atomic_add(ti->num_flush_bios, &ci->io->io_count);
		bios = __send_duplicate_bios(ci, ti, ti->num_flush_bios,
					     NULL, GFP_NOWAIT);
		atomic_sub(ti->num_flush_bios - bios, &ci->io->io_count);
	}

	/*
	 * alloc_io() takes one extra reference for submission, so the
	 * reference won't reach 0 without the following subtraction
	 */
	atomic_sub(1, &ci->io->io_count);

	bio_uninit(ci->bio);
}

static void __send_abnormal_io(struct clone_info *ci, struct dm_target *ti,
			       unsigned int num_bios, unsigned int max_granularity,
			       unsigned int max_sectors)
{
	unsigned int len, bios;

	len = min_t(sector_t, ci->sector_count,
		    __max_io_len(ti, ci->sector, max_granularity, max_sectors));

	atomic_add(num_bios, &ci->io->io_count);
	bios = __send_duplicate_bios(ci, ti, num_bios, &len, GFP_NOIO);
	/*
	 * alloc_io() takes one extra reference for submission, so the
	 * reference won't reach 0 without the following (+1) subtraction
	 */
	atomic_sub(num_bios - bios + 1, &ci->io->io_count);

	ci->sector += len;
	ci->sector_count -= len;
}

static bool is_abnormal_io(struct bio *bio)
{
	enum req_op op = bio_op(bio);

	if (op != REQ_OP_READ && op != REQ_OP_WRITE && op != REQ_OP_FLUSH) {
		switch (op) {
		case REQ_OP_DISCARD:
		case REQ_OP_SECURE_ERASE:
		case REQ_OP_WRITE_ZEROES:
			return true;
		default:
			break;
		}
	}

	return false;
}

static blk_status_t __process_abnormal_io(struct clone_info *ci,
					  struct dm_target *ti)
{
	unsigned int num_bios = 0;
	unsigned int max_granularity = 0;
	unsigned int max_sectors = 0;
	struct queue_limits *limits = dm_get_queue_limits(ti->table->md);

	switch (bio_op(ci->bio)) {
	case REQ_OP_DISCARD:
		num_bios = ti->num_discard_bios;
		max_sectors = limits->max_discard_sectors;
		if (ti->max_discard_granularity)
			max_granularity = max_sectors;
		break;
	case REQ_OP_SECURE_ERASE:
		num_bios = ti->num_secure_erase_bios;
		max_sectors = limits->max_secure_erase_sectors;
		if (ti->max_secure_erase_granularity)
			max_granularity = max_sectors;
		break;
	case REQ_OP_WRITE_ZEROES:
		num_bios = ti->num_write_zeroes_bios;
		max_sectors = limits->max_write_zeroes_sectors;
		if (ti->max_write_zeroes_granularity)
			max_granularity = max_sectors;
		break;
	default:
		break;
	}

	/*
	 * Even though the device advertised support for this type of
	 * request, that does not mean every target supports it, and
	 * reconfiguration might also have changed that since the
	 * check was performed.
	 */
	if (unlikely(!num_bios))
		return BLK_STS_NOTSUPP;

	__send_abnormal_io(ci, ti, num_bios, max_granularity, max_sectors);

	return BLK_STS_OK;
}

/*
 * Reuse ->bi_private as dm_io list head for storing all dm_io instances
 * associated with this bio, and this bio's bi_private needs to be
 * stored in dm_io->data before the reuse.
 *
 * bio->bi_private is owned by fs or upper layer, so block layer won't
 * touch it after splitting. Meantime it won't be changed by anyone after
 * bio is submitted. So this reuse is safe.
 */
static inline struct dm_io **dm_poll_list_head(struct bio *bio)
{
	return (struct dm_io **)&bio->bi_private;
}

static void dm_queue_poll_io(struct bio *bio, struct dm_io *io)
{
	struct dm_io **head = dm_poll_list_head(bio);

	if (!(bio->bi_opf & REQ_DM_POLL_LIST)) {
		bio->bi_opf |= REQ_DM_POLL_LIST;
		/*
		 * Save .bi_private into dm_io, so that we can reuse
		 * .bi_private as dm_io list head for storing dm_io list
		 */
		io->data = bio->bi_private;

		/* tell block layer to poll for completion */
		bio->bi_cookie = ~BLK_QC_T_NONE;

		io->next = NULL;
	} else {
		/*
		 * bio recursed due to split, reuse original poll list,
		 * and save bio->bi_private too.
		 */
		io->data = (*head)->data;
		io->next = *head;
	}

	*head = io;
}

/*
 * Select the correct strategy for processing a non-flush bio.
 */
static blk_status_t __split_and_process_bio(struct clone_info *ci)
{
	struct bio *clone;
	struct dm_target *ti;
	unsigned int len;

	ti = dm_table_find_target(ci->map, ci->sector);
	if (unlikely(!ti))
		return BLK_STS_IOERR;

	if (unlikely(ci->is_abnormal_io))
		return __process_abnormal_io(ci, ti);

	/*
	 * Only support bio polling for normal IO, and the target io is
	 * exactly inside the dm_io instance (verified in dm_poll_dm_io)
	 */
	ci->submit_as_polled = !!(ci->bio->bi_opf & REQ_POLLED);

	len = min_t(sector_t, max_io_len(ti, ci->sector), ci->sector_count);
	setup_split_accounting(ci, len);

	if (unlikely(ci->bio->bi_opf & REQ_NOWAIT)) {
		if (unlikely(!dm_target_supports_nowait(ti->type)))
			return BLK_STS_NOTSUPP;

		clone = alloc_tio(ci, ti, 0, &len, GFP_NOWAIT);
		if (unlikely(!clone))
			return BLK_STS_AGAIN;
	} else {
		clone = alloc_tio(ci, ti, 0, &len, GFP_NOIO);
	}
	__map_bio(clone);

	ci->sector += len;
	ci->sector_count -= len;

	return BLK_STS_OK;
}

static void init_clone_info(struct clone_info *ci, struct dm_io *io,
			    struct dm_table *map, struct bio *bio, bool is_abnormal)
{
	ci->map = map;
	ci->io = io;
	ci->bio = bio;
	ci->is_abnormal_io = is_abnormal;
	ci->submit_as_polled = false;
	ci->sector = bio->bi_iter.bi_sector;
	ci->sector_count = bio_sectors(bio);

	/* Shouldn't happen but sector_count was being set to 0 so... */
	if (static_branch_unlikely(&zoned_enabled) &&
	    WARN_ON_ONCE(op_is_zone_mgmt(bio_op(bio)) && ci->sector_count))
		ci->sector_count = 0;
}

/*
 * Entry point to split a bio into clones and submit them to the targets.
 */
static void dm_split_and_process_bio(struct mapped_device *md,
				     struct dm_table *map, struct bio *bio)
{
	struct clone_info ci;
	struct dm_io *io;
	blk_status_t error = BLK_STS_OK;
	bool is_abnormal;

	is_abnormal = is_abnormal_io(bio);
	if (unlikely(is_abnormal)) {
		/*
		 * Use bio_split_to_limits() for abnormal IO (e.g. discard, etc)
		 * otherwise associated queue_limits won't be imposed.
		 */
		bio = bio_split_to_limits(bio);
		if (!bio)
			return;
	}

	/* Only support nowait for normal IO */
	if (unlikely(bio->bi_opf & REQ_NOWAIT) && !is_abnormal) {
		io = alloc_io(md, bio, GFP_NOWAIT);
		if (unlikely(!io)) {
			/* Unable to do anything without dm_io. */
			bio_wouldblock_error(bio);
			return;
		}
	} else {
		io = alloc_io(md, bio, GFP_NOIO);
	}
	init_clone_info(&ci, io, map, bio, is_abnormal);

	if (bio->bi_opf & REQ_PREFLUSH) {
		__send_empty_flush(&ci);
		/* dm_io_complete submits any data associated with flush */
		goto out;
	}

	error = __split_and_process_bio(&ci);
	if (error || !ci.sector_count)
		goto out;
	/*
	 * Remainder must be passed to submit_bio_noacct() so it gets handled
	 * *after* bios already submitted have been completely processed.
	 */
	bio_trim(bio, io->sectors, ci.sector_count);
	trace_block_split(bio, bio->bi_iter.bi_sector);
	bio_inc_remaining(bio);
	submit_bio_noacct(bio);
out:
	/*
	 * Drop the extra reference count for non-POLLED bio, and hold one
	 * reference for POLLED bio, which will be released in dm_poll_bio
	 *
	 * Add every dm_io instance into the dm_io list head which is stored
	 * in bio->bi_private, so that dm_poll_bio can poll them all.
	 */
	if (error || !ci.submit_as_polled) {
		/*
		 * In case of submission failure, the extra reference for
		 * submitting io isn't consumed yet
		 */
		if (error)
			atomic_dec(&io->io_count);
		dm_io_dec_pending(io, error);
	} else
		dm_queue_poll_io(bio, io);
}

static void dm_submit_bio(struct bio *bio)
{
	struct mapped_device *md = bio->bi_bdev->bd_disk->private_data;
	int srcu_idx;
	struct dm_table *map;

	map = dm_get_live_table(md, &srcu_idx);

	/* If suspended, or map not yet available, queue this IO for later */
	if (unlikely(test_bit(DMF_BLOCK_IO_FOR_SUSPEND, &md->flags)) ||
	    unlikely(!map)) {
		if (bio->bi_opf & REQ_NOWAIT)
			bio_wouldblock_error(bio);
		else if (bio->bi_opf & REQ_RAHEAD)
			bio_io_error(bio);
		else
			queue_io(md, bio);
		goto out;
	}

	dm_split_and_process_bio(md, map, bio);
out:
	dm_put_live_table(md, srcu_idx);
}

static bool dm_poll_dm_io(struct dm_io *io, struct io_comp_batch *iob,
			  unsigned int flags)
{
	WARN_ON_ONCE(!dm_tio_is_normal(&io->tio));

	/* don't poll if the mapped io is done */
	if (atomic_read(&io->io_count) > 1)
		bio_poll(&io->tio.clone, iob, flags);

	/* bio_poll holds the last reference */
	return atomic_read(&io->io_count) == 1;
}

static int dm_poll_bio(struct bio *bio, struct io_comp_batch *iob,
		       unsigned int flags)
{
	struct dm_io **head = dm_poll_list_head(bio);
	struct dm_io *list = *head;
	struct dm_io *tmp = NULL;
	struct dm_io *curr, *next;

	/* Only poll normal bio which was marked as REQ_DM_POLL_LIST */
	if (!(bio->bi_opf & REQ_DM_POLL_LIST))
		return 0;

	WARN_ON_ONCE(!list);

	/*
	 * Restore .bi_private before possibly completing dm_io.
	 *
	 * bio_poll() is only possible once @bio has been completely
	 * submitted via submit_bio_noacct()'s depth-first submission.
	 * So there is no dm_queue_poll_io() race associated with
	 * clearing REQ_DM_POLL_LIST here.
	 */
	bio->bi_opf &= ~REQ_DM_POLL_LIST;
	bio->bi_private = list->data;

	for (curr = list, next = curr->next; curr; curr = next, next =
			curr ? curr->next : NULL) {
		if (dm_poll_dm_io(curr, iob, flags)) {
			/*
			 * clone_endio() has already occurred, so no
			 * error handling is needed here.
			 */
			__dm_io_dec_pending(curr);
		} else {
			curr->next = tmp;
			tmp = curr;
		}
	}

	/* Not done? */
	if (tmp) {
		bio->bi_opf |= REQ_DM_POLL_LIST;
		/* Reset bio->bi_private to dm_io list head */
		*head = tmp;
		return 0;
	}
	return 1;
}

/*
 *---------------------------------------------------------------
 * An IDR is used to keep track of allocated minor numbers.
 *---------------------------------------------------------------
 */
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
static void free_minor(int minor)
{
	spin_lock(&_minor_lock);
	idr_remove(&_minor_idr, minor);
	spin_unlock(&_minor_lock);
}

/*
 * See if the device with a specific minor # is free.
 */
static int specific_minor(int minor)
{
<<<<<<< HEAD
	int r, m;
=======
	int r;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	if (minor >= (1 << MINORBITS))
		return -EINVAL;

<<<<<<< HEAD
	r = idr_pre_get(&_minor_idr, GFP_KERNEL);
	if (!r)
		return -ENOMEM;

	spin_lock(&_minor_lock);

	if (idr_find(&_minor_idr, minor)) {
		r = -EBUSY;
		goto out;
	}

	r = idr_get_new_above(&_minor_idr, MINOR_ALLOCED, minor, &m);
	if (r)
		goto out;

	if (m != minor) {
		idr_remove(&_minor_idr, m);
		r = -EBUSY;
		goto out;
	}

out:
	spin_unlock(&_minor_lock);
	return r;
=======
	idr_preload(GFP_KERNEL);
	spin_lock(&_minor_lock);

	r = idr_alloc(&_minor_idr, MINOR_ALLOCED, minor, minor + 1, GFP_NOWAIT);

	spin_unlock(&_minor_lock);
	idr_preload_end();
	if (r < 0)
		return r == -ENOSPC ? -EBUSY : r;
	return 0;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

static int next_free_minor(int *minor)
{
<<<<<<< HEAD
	int r, m;

	r = idr_pre_get(&_minor_idr, GFP_KERNEL);
	if (!r)
		return -ENOMEM;

	spin_lock(&_minor_lock);

	r = idr_get_new(&_minor_idr, MINOR_ALLOCED, &m);
	if (r)
		goto out;

	if (m >= (1 << MINORBITS)) {
		idr_remove(&_minor_idr, m);
		r = -ENOSPC;
		goto out;
	}

	*minor = m;

out:
	spin_unlock(&_minor_lock);
	return r;
}

static const struct block_device_operations dm_blk_dops;

static void dm_wq_work(struct work_struct *work);

static void dm_init_md_queue(struct mapped_device *md)
{
	/*
	 * Request-based dm devices cannot be stacked on top of bio-based dm
	 * devices.  The type of this dm device has not been decided yet.
	 * The type is decided at the first table loading time.
	 * To prevent problematic device stacking, clear the queue flag
	 * for request stacking support until then.
	 *
	 * This queue is new, so no concurrency on the queue_flags.
	 */
	queue_flag_clear_unlocked(QUEUE_FLAG_STACKABLE, md->queue);

	md->queue->queuedata = md;
	md->queue->backing_dev_info.congested_fn = dm_any_congested;
	md->queue->backing_dev_info.congested_data = md;
	blk_queue_make_request(md->queue, dm_request);
	blk_queue_bounce_limit(md->queue, BLK_BOUNCE_ANY);
	blk_queue_merge_bvec(md->queue, dm_merge_bvec);
=======
	int r;

	idr_preload(GFP_KERNEL);
	spin_lock(&_minor_lock);

	r = idr_alloc(&_minor_idr, MINOR_ALLOCED, 0, 1 << MINORBITS, GFP_NOWAIT);

	spin_unlock(&_minor_lock);
	idr_preload_end();
	if (r < 0)
		return r;
	*minor = r;
	return 0;
}

static const struct block_device_operations dm_blk_dops;
static const struct block_device_operations dm_rq_blk_dops;
static const struct dax_operations dm_dax_ops;

static void dm_wq_work(struct work_struct *work);

#ifdef CONFIG_BLK_INLINE_ENCRYPTION
static void dm_queue_destroy_crypto_profile(struct request_queue *q)
{
	dm_destroy_crypto_profile(q->crypto_profile);
}

#else /* CONFIG_BLK_INLINE_ENCRYPTION */

static inline void dm_queue_destroy_crypto_profile(struct request_queue *q)
{
}
#endif /* !CONFIG_BLK_INLINE_ENCRYPTION */

static void cleanup_mapped_device(struct mapped_device *md)
{
	if (md->wq)
		destroy_workqueue(md->wq);
	dm_free_md_mempools(md->mempools);

	if (md->dax_dev) {
		dax_remove_host(md->disk);
		kill_dax(md->dax_dev);
		put_dax(md->dax_dev);
		md->dax_dev = NULL;
	}

	dm_cleanup_zoned_dev(md);
	if (md->disk) {
		spin_lock(&_minor_lock);
		md->disk->private_data = NULL;
		spin_unlock(&_minor_lock);
		if (dm_get_md_type(md) != DM_TYPE_NONE) {
			struct table_device *td;

			dm_sysfs_exit(md);
			list_for_each_entry(td, &md->table_devices, list) {
				bd_unlink_disk_holder(td->dm_dev.bdev,
						      md->disk);
			}

			/*
			 * Hold lock to make sure del_gendisk() won't concurrent
			 * with open/close_table_device().
			 */
			mutex_lock(&md->table_devices_lock);
			del_gendisk(md->disk);
			mutex_unlock(&md->table_devices_lock);
		}
		dm_queue_destroy_crypto_profile(md->queue);
		put_disk(md->disk);
	}

	if (md->pending_io) {
		free_percpu(md->pending_io);
		md->pending_io = NULL;
	}

	cleanup_srcu_struct(&md->io_barrier);

	mutex_destroy(&md->suspend_lock);
	mutex_destroy(&md->type_lock);
	mutex_destroy(&md->table_devices_lock);
	mutex_destroy(&md->swap_bios_lock);

	dm_mq_cleanup_mapped_device(md);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

/*
 * Allocate and initialise a blank device with a given minor.
 */
static struct mapped_device *alloc_dev(int minor)
{
<<<<<<< HEAD
	int r;
	struct mapped_device *md = kzalloc(sizeof(*md), GFP_KERNEL);
	void *old_md;

	if (!md) {
		DMWARN("unable to allocate device, out of memory.");
=======
	int r, numa_node_id = dm_get_numa_node();
	struct dax_device *dax_dev;
	struct mapped_device *md;
	void *old_md;

	md = kvzalloc_node(sizeof(*md), GFP_KERNEL, numa_node_id);
	if (!md) {
		DMERR("unable to allocate device, out of memory.");
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		return NULL;
	}

	if (!try_module_get(THIS_MODULE))
		goto bad_module_get;

	/* get a minor number for the dev */
	if (minor == DM_ANY_MINOR)
		r = next_free_minor(&minor);
	else
		r = specific_minor(minor);
	if (r < 0)
		goto bad_minor;

<<<<<<< HEAD
	md->type = DM_TYPE_NONE;
	init_rwsem(&md->io_lock);
	mutex_init(&md->suspend_lock);
	mutex_init(&md->type_lock);
	spin_lock_init(&md->deferred_lock);
	rwlock_init(&md->map_lock);
=======
	r = init_srcu_struct(&md->io_barrier);
	if (r < 0)
		goto bad_io_barrier;

	md->numa_node_id = numa_node_id;
	md->init_tio_pdu = false;
	md->type = DM_TYPE_NONE;
	mutex_init(&md->suspend_lock);
	mutex_init(&md->type_lock);
	mutex_init(&md->table_devices_lock);
	spin_lock_init(&md->deferred_lock);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	atomic_set(&md->holders, 1);
	atomic_set(&md->open_count, 0);
	atomic_set(&md->event_nr, 0);
	atomic_set(&md->uevent_seq, 0);
	INIT_LIST_HEAD(&md->uevent_list);
<<<<<<< HEAD
	spin_lock_init(&md->uevent_lock);

	md->queue = blk_alloc_queue(GFP_KERNEL);
	if (!md->queue)
		goto bad_queue;

	dm_init_md_queue(md);

	md->disk = alloc_disk(1);
	if (!md->disk)
		goto bad_disk;

	atomic_set(&md->pending[0], 0);
	atomic_set(&md->pending[1], 0);
	init_waitqueue_head(&md->wait);
	INIT_WORK(&md->work, dm_wq_work);
	init_waitqueue_head(&md->eventq);
	init_completion(&md->kobj_holder.completion);

	md->disk->major = _major;
	md->disk->first_minor = minor;
	md->disk->fops = &dm_blk_dops;
	md->disk->queue = md->queue;
	md->disk->private_data = md;
	sprintf(md->disk->disk_name, "dm-%d", minor);
	add_disk(md->disk);
	format_dev_t(md->name, MKDEV(_major, minor));

	md->wq = alloc_workqueue("kdmflush",
				 WQ_NON_REENTRANT | WQ_MEM_RECLAIM, 0);
	if (!md->wq)
		goto bad_thread;

	md->bdev = bdget_disk(md->disk, 0);
	if (!md->bdev)
		goto bad_bdev;

	bio_init(&md->flush_bio);
	md->flush_bio.bi_bdev = md->bdev;
	md->flush_bio.bi_rw = WRITE_FLUSH;
=======
	INIT_LIST_HEAD(&md->table_devices);
	spin_lock_init(&md->uevent_lock);

	/*
	 * default to bio-based until DM table is loaded and md->type
	 * established. If request-based table is loaded: blk-mq will
	 * override accordingly.
	 */
	md->disk = blk_alloc_disk(NULL, md->numa_node_id);
	if (IS_ERR(md->disk))
		goto bad;
	md->queue = md->disk->queue;

	init_waitqueue_head(&md->wait);
	INIT_WORK(&md->work, dm_wq_work);
	INIT_WORK(&md->requeue_work, dm_wq_requeue_work);
	init_waitqueue_head(&md->eventq);
	init_completion(&md->kobj_holder.completion);

	md->requeue_list = NULL;
	md->swap_bios = get_swap_bios();
	sema_init(&md->swap_bios_semaphore, md->swap_bios);
	mutex_init(&md->swap_bios_lock);

	md->disk->major = _major;
	md->disk->first_minor = minor;
	md->disk->minors = 1;
	md->disk->flags |= GENHD_FL_NO_PART;
	md->disk->fops = &dm_blk_dops;
	md->disk->private_data = md;
	sprintf(md->disk->disk_name, "dm-%d", minor);

	dax_dev = alloc_dax(md, &dm_dax_ops);
	if (IS_ERR(dax_dev)) {
		if (PTR_ERR(dax_dev) != -EOPNOTSUPP)
			goto bad;
	} else {
		set_dax_nocache(dax_dev);
		set_dax_nomc(dax_dev);
		md->dax_dev = dax_dev;
		if (dax_add_host(dax_dev, md->disk))
			goto bad;
	}

	format_dev_t(md->name, MKDEV(_major, minor));

	md->wq = alloc_workqueue("kdmflush/%s", WQ_MEM_RECLAIM, 0, md->name);
	if (!md->wq)
		goto bad;

	md->pending_io = alloc_percpu(unsigned long);
	if (!md->pending_io)
		goto bad;

	r = dm_stats_init(&md->stats);
	if (r < 0)
		goto bad;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	/* Populate the mapping, nobody knows we exist yet */
	spin_lock(&_minor_lock);
	old_md = idr_replace(&_minor_idr, md, minor);
	spin_unlock(&_minor_lock);

	BUG_ON(old_md != MINOR_ALLOCED);

	return md;

<<<<<<< HEAD
bad_bdev:
	destroy_workqueue(md->wq);
bad_thread:
	del_gendisk(md->disk);
	put_disk(md->disk);
bad_disk:
	blk_cleanup_queue(md->queue);
bad_queue:
=======
bad:
	cleanup_mapped_device(md);
bad_io_barrier:
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	free_minor(minor);
bad_minor:
	module_put(THIS_MODULE);
bad_module_get:
<<<<<<< HEAD
	kfree(md);
=======
	kvfree(md);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	return NULL;
}

static void unlock_fs(struct mapped_device *md);

static void free_dev(struct mapped_device *md)
{
	int minor = MINOR(disk_devt(md->disk));

	unlock_fs(md);
<<<<<<< HEAD
	bdput(md->bdev);
	destroy_workqueue(md->wq);
	if (md->tio_pool)
		mempool_destroy(md->tio_pool);
	if (md->io_pool)
		mempool_destroy(md->io_pool);
	if (md->bs)
		bioset_free(md->bs);
	blk_integrity_unregister(md->disk);
	del_gendisk(md->disk);
	free_minor(minor);

	spin_lock(&_minor_lock);
	md->disk->private_data = NULL;
	spin_unlock(&_minor_lock);

	put_disk(md->disk);
	blk_cleanup_queue(md->queue);
	module_put(THIS_MODULE);
	kfree(md);
}

static void __bind_mempools(struct mapped_device *md, struct dm_table *t)
{
	struct dm_md_mempools *p;

	if (md->io_pool && md->tio_pool && md->bs)
		/* the md already has necessary mempools */
		goto out;

	p = dm_table_get_md_mempools(t);
	BUG_ON(!p || md->io_pool || md->tio_pool || md->bs);

	md->io_pool = p->io_pool;
	p->io_pool = NULL;
	md->tio_pool = p->tio_pool;
	p->tio_pool = NULL;
	md->bs = p->bs;
	p->bs = NULL;

out:
	/* mempool bind completed, now no need any mempools in the table */
	dm_table_free_md_mempools(t);
=======

	cleanup_mapped_device(md);

	WARN_ON_ONCE(!list_empty(&md->table_devices));
	dm_stats_cleanup(&md->stats);
	free_minor(minor);

	module_put(THIS_MODULE);
	kvfree(md);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

/*
 * Bind a table to the device.
 */
static void event_callback(void *context)
{
	unsigned long flags;
	LIST_HEAD(uevents);
<<<<<<< HEAD
	struct mapped_device *md = (struct mapped_device *) context;
=======
	struct mapped_device *md = context;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	spin_lock_irqsave(&md->uevent_lock, flags);
	list_splice_init(&md->uevent_list, &uevents);
	spin_unlock_irqrestore(&md->uevent_lock, flags);

	dm_send_uevents(&uevents, &disk_to_dev(md->disk)->kobj);

	atomic_inc(&md->event_nr);
	wake_up(&md->eventq);
<<<<<<< HEAD
}

/*
 * Protected by md->suspend_lock obtained by dm_swap_table().
 */
static void __set_size(struct mapped_device *md, sector_t size)
{
	set_capacity(md->disk, size);

	i_size_write(md->bdev->bd_inode, (loff_t)size << SECTOR_SHIFT);
}

/*
 * Return 1 if the queue has a compulsory merge_bvec_fn function.
 *
 * If this function returns 0, then the device is either a non-dm
 * device without a merge_bvec_fn, or it is a dm device that is
 * able to split any bios it receives that are too big.
 */
int dm_queue_merge_is_compulsory(struct request_queue *q)
{
	struct mapped_device *dev_md;

	if (!q->merge_bvec_fn)
		return 0;

	if (q->make_request_fn == dm_request) {
		dev_md = q->queuedata;
		if (test_bit(DMF_MERGE_IS_OPTIONAL, &dev_md->flags))
			return 0;
	}

	return 1;
}

static int dm_device_merge_is_compulsory(struct dm_target *ti,
					 struct dm_dev *dev, sector_t start,
					 sector_t len, void *data)
{
	struct block_device *bdev = dev->bdev;
	struct request_queue *q = bdev_get_queue(bdev);

	return dm_queue_merge_is_compulsory(q);
}

/*
 * Return 1 if it is acceptable to ignore merge_bvec_fn based
 * on the properties of the underlying devices.
 */
static int dm_table_merge_is_optional(struct dm_table *table)
{
	unsigned i = 0;
	struct dm_target *ti;

	while (i < dm_table_get_num_targets(table)) {
		ti = dm_table_get_target(table, i++);

		if (ti->type->iterate_devices &&
		    ti->type->iterate_devices(ti, dm_device_merge_is_compulsory, NULL))
			return 0;
	}

	return 1;
=======
	dm_issue_global_event();
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

/*
 * Returns old map, which caller must destroy.
 */
static struct dm_table *__bind(struct mapped_device *md, struct dm_table *t,
			       struct queue_limits *limits)
{
	struct dm_table *old_map;
<<<<<<< HEAD
	struct request_queue *q = md->queue;
	sector_t size;
	unsigned long flags;
	int merge_is_optional;
=======
	sector_t size;
	int ret;

	lockdep_assert_held(&md->suspend_lock);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	size = dm_table_get_size(t);

	/*
	 * Wipe any geometry if the size of the table changed.
	 */
<<<<<<< HEAD
	if (size != get_capacity(md->disk))
		memset(&md->geometry, 0, sizeof(md->geometry));

	__set_size(md, size);

	dm_table_event_callback(t, event_callback, md);

	/*
	 * The queue hasn't been stopped yet, if the old table type wasn't
	 * for request-based during suspension.  So stop it to prevent
	 * I/O mapping before resume.
	 * This must be done before setting the queue restrictions,
	 * because request-based dm may be run just after the setting.
	 */
	if (dm_table_request_based(t) && !blk_queue_stopped(q))
		stop_queue(q);

	__bind_mempools(md, t);

	merge_is_optional = dm_table_merge_is_optional(t);

	write_lock_irqsave(&md->map_lock, flags);
	old_map = md->map;
	md->map = t;
	md->immutable_target_type = dm_table_get_immutable_target_type(t);

	dm_table_set_restrictions(t, q, limits);
	if (merge_is_optional)
		set_bit(DMF_MERGE_IS_OPTIONAL, &md->flags);
	else
		clear_bit(DMF_MERGE_IS_OPTIONAL, &md->flags);
	write_unlock_irqrestore(&md->map_lock, flags);

=======
	if (size != dm_get_size(md))
		memset(&md->geometry, 0, sizeof(md->geometry));

	set_capacity(md->disk, size);

	dm_table_event_callback(t, event_callback, md);

	if (dm_table_request_based(t)) {
		/*
		 * Leverage the fact that request-based DM targets are
		 * immutable singletons - used to optimize dm_mq_queue_rq.
		 */
		md->immutable_target = dm_table_get_immutable_target(t);

		/*
		 * There is no need to reload with request-based dm because the
		 * size of front_pad doesn't change.
		 *
		 * Note for future: If you are to reload bioset, prep-ed
		 * requests in the queue may refer to bio from the old bioset,
		 * so you must walk through the queue to unprep.
		 */
		if (!md->mempools) {
			md->mempools = t->mempools;
			t->mempools = NULL;
		}
	} else {
		/*
		 * The md may already have mempools that need changing.
		 * If so, reload bioset because front_pad may have changed
		 * because a different table was loaded.
		 */
		dm_free_md_mempools(md->mempools);
		md->mempools = t->mempools;
		t->mempools = NULL;
	}

	ret = dm_table_set_restrictions(t, md->queue, limits);
	if (ret) {
		old_map = ERR_PTR(ret);
		goto out;
	}

	old_map = rcu_dereference_protected(md->map, lockdep_is_held(&md->suspend_lock));
	rcu_assign_pointer(md->map, (void *)t);
	md->immutable_target_type = dm_table_get_immutable_target_type(t);

	if (old_map)
		dm_sync_table(md);
out:
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	return old_map;
}

/*
 * Returns unbound table for the caller to free.
 */
static struct dm_table *__unbind(struct mapped_device *md)
{
<<<<<<< HEAD
	struct dm_table *map = md->map;
	unsigned long flags;
=======
	struct dm_table *map = rcu_dereference_protected(md->map, 1);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	if (!map)
		return NULL;

	dm_table_event_callback(map, NULL, NULL);
<<<<<<< HEAD
	write_lock_irqsave(&md->map_lock, flags);
	md->map = NULL;
	write_unlock_irqrestore(&md->map_lock, flags);
=======
	RCU_INIT_POINTER(md->map, NULL);
	dm_sync_table(md);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	return map;
}

/*
 * Constructor for a new device.
 */
int dm_create(int minor, struct mapped_device **result)
{
	struct mapped_device *md;

	md = alloc_dev(minor);
	if (!md)
		return -ENXIO;

<<<<<<< HEAD
	dm_sysfs_init(md);
=======
	dm_ima_reset_data(md);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	*result = md;
	return 0;
}

/*
 * Functions to manage md->type.
 * All are required to hold md->type_lock.
 */
void dm_lock_md_type(struct mapped_device *md)
{
	mutex_lock(&md->type_lock);
}

void dm_unlock_md_type(struct mapped_device *md)
{
	mutex_unlock(&md->type_lock);
}

<<<<<<< HEAD
void dm_set_md_type(struct mapped_device *md, unsigned type)
{
	md->type = type;
}

unsigned dm_get_md_type(struct mapped_device *md)
=======
void dm_set_md_type(struct mapped_device *md, enum dm_queue_mode type)
{
	BUG_ON(!mutex_is_locked(&md->type_lock));
	md->type = type;
}

enum dm_queue_mode dm_get_md_type(struct mapped_device *md)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
{
	return md->type;
}

struct target_type *dm_get_immutable_target_type(struct mapped_device *md)
{
	return md->immutable_target_type;
}

/*
<<<<<<< HEAD
 * Fully initialize a request-based queue (->elevator, ->request_fn, etc).
 */
static int dm_init_request_based_queue(struct mapped_device *md)
{
	struct request_queue *q = NULL;

	if (md->queue->elevator)
		return 1;

	/* Fully initialize the queue */
	q = blk_init_allocated_queue(md->queue, dm_request_fn, NULL);
	if (!q)
		return 0;

	md->queue = q;
	dm_init_md_queue(md);
	blk_queue_softirq_done(md->queue, dm_softirq_done);
	blk_queue_prep_rq(md->queue, dm_prep_fn);
	blk_queue_lld_busy(md->queue, dm_lld_busy);

	elv_register_queue(md->queue);

	return 1;
}

/*
 * Setup the DM device's queue based on md's type
 */
int dm_setup_md_queue(struct mapped_device *md)
{
	if ((dm_get_md_type(md) == DM_TYPE_REQUEST_BASED) &&
	    !dm_init_request_based_queue(md)) {
		DMWARN("Cannot initialize queue for request-based mapped device");
		return -EINVAL;
	}

	return 0;
=======
 * Setup the DM device's queue based on md's type
 */
int dm_setup_md_queue(struct mapped_device *md, struct dm_table *t)
{
	enum dm_queue_mode type = dm_table_get_type(t);
	struct queue_limits limits;
	struct table_device *td;
	int r;

	switch (type) {
	case DM_TYPE_REQUEST_BASED:
		md->disk->fops = &dm_rq_blk_dops;
		r = dm_mq_init_request_queue(md, t);
		if (r) {
			DMERR("Cannot initialize queue for request-based dm mapped device");
			return r;
		}
		break;
	case DM_TYPE_BIO_BASED:
	case DM_TYPE_DAX_BIO_BASED:
		blk_queue_flag_set(QUEUE_FLAG_IO_STAT, md->queue);
		break;
	case DM_TYPE_NONE:
		WARN_ON_ONCE(true);
		break;
	}

	r = dm_calculate_queue_limits(t, &limits);
	if (r) {
		DMERR("Cannot calculate initial queue limits");
		return r;
	}
	r = dm_table_set_restrictions(t, md->queue, &limits);
	if (r)
		return r;

	/*
	 * Hold lock to make sure add_disk() and del_gendisk() won't concurrent
	 * with open_table_device() and close_table_device().
	 */
	mutex_lock(&md->table_devices_lock);
	r = add_disk(md->disk);
	mutex_unlock(&md->table_devices_lock);
	if (r)
		return r;

	/*
	 * Register the holder relationship for devices added before the disk
	 * was live.
	 */
	list_for_each_entry(td, &md->table_devices, list) {
		r = bd_link_disk_holder(td->dm_dev.bdev, md->disk);
		if (r)
			goto out_undo_holders;
	}

	r = dm_sysfs_init(md);
	if (r)
		goto out_undo_holders;

	md->type = type;
	return 0;

out_undo_holders:
	list_for_each_entry_continue_reverse(td, &md->table_devices, list)
		bd_unlink_disk_holder(td->dm_dev.bdev, md->disk);
	mutex_lock(&md->table_devices_lock);
	del_gendisk(md->disk);
	mutex_unlock(&md->table_devices_lock);
	return r;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

struct mapped_device *dm_get_md(dev_t dev)
{
	struct mapped_device *md;
<<<<<<< HEAD
	unsigned minor = MINOR(dev);
=======
	unsigned int minor = MINOR(dev);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	if (MAJOR(dev) != _major || minor >= (1 << MINORBITS))
		return NULL;

	spin_lock(&_minor_lock);

	md = idr_find(&_minor_idr, minor);
<<<<<<< HEAD
	if (md) {
		if ((md == MINOR_ALLOCED ||
		     (MINOR(disk_devt(dm_disk(md))) != minor) ||
		     dm_deleting_md(md) ||
		     test_bit(DMF_FREEING, &md->flags))) {
			md = NULL;
			goto out;
		}
		dm_get(md);
	}

=======
	if (!md || md == MINOR_ALLOCED || (MINOR(disk_devt(dm_disk(md))) != minor) ||
	    test_bit(DMF_FREEING, &md->flags) || dm_deleting_md(md)) {
		md = NULL;
		goto out;
	}
	dm_get(md);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
out:
	spin_unlock(&_minor_lock);

	return md;
}
EXPORT_SYMBOL_GPL(dm_get_md);

void *dm_get_mdptr(struct mapped_device *md)
{
	return md->interface_ptr;
}

void dm_set_mdptr(struct mapped_device *md, void *ptr)
{
	md->interface_ptr = ptr;
}

void dm_get(struct mapped_device *md)
{
	atomic_inc(&md->holders);
	BUG_ON(test_bit(DMF_FREEING, &md->flags));
}

<<<<<<< HEAD
=======
int dm_hold(struct mapped_device *md)
{
	spin_lock(&_minor_lock);
	if (test_bit(DMF_FREEING, &md->flags)) {
		spin_unlock(&_minor_lock);
		return -EBUSY;
	}
	dm_get(md);
	spin_unlock(&_minor_lock);
	return 0;
}
EXPORT_SYMBOL_GPL(dm_hold);

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
const char *dm_device_name(struct mapped_device *md)
{
	return md->name;
}
EXPORT_SYMBOL_GPL(dm_device_name);

static void __dm_destroy(struct mapped_device *md, bool wait)
{
	struct dm_table *map;
<<<<<<< HEAD
=======
	int srcu_idx;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	might_sleep();

	spin_lock(&_minor_lock);
<<<<<<< HEAD
	map = dm_get_live_table(md);
=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	idr_replace(&_minor_idr, MINOR_ALLOCED, MINOR(disk_devt(dm_disk(md))));
	set_bit(DMF_FREEING, &md->flags);
	spin_unlock(&_minor_lock);

<<<<<<< HEAD
=======
	blk_mark_disk_dead(md->disk);

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	/*
	 * Take suspend_lock so that presuspend and postsuspend methods
	 * do not race with internal suspend.
	 */
	mutex_lock(&md->suspend_lock);
<<<<<<< HEAD
	if (!dm_suspended_md(md)) {
		dm_table_presuspend_targets(map);
		dm_table_postsuspend_targets(map);
	}
=======
	map = dm_get_live_table(md, &srcu_idx);
	if (!dm_suspended_md(md)) {
		dm_table_presuspend_targets(map);
		set_bit(DMF_SUSPENDED, &md->flags);
		set_bit(DMF_POST_SUSPENDING, &md->flags);
		dm_table_postsuspend_targets(map);
	}
	/* dm_put_live_table must be before fsleep, otherwise deadlock is possible */
	dm_put_live_table(md, srcu_idx);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	mutex_unlock(&md->suspend_lock);

	/*
	 * Rare, but there may be I/O requests still going to complete,
	 * for example.  Wait for all references to disappear.
	 * No one should increment the reference count of the mapped_device,
	 * after the mapped_device state becomes DMF_FREEING.
	 */
	if (wait)
		while (atomic_read(&md->holders))
<<<<<<< HEAD
			msleep(1);
=======
			fsleep(1000);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	else if (atomic_read(&md->holders))
		DMWARN("%s: Forcibly removing mapped_device still in use! (%d users)",
		       dm_device_name(md), atomic_read(&md->holders));

<<<<<<< HEAD
	dm_sysfs_exit(md);
	dm_table_put(map);
=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	dm_table_destroy(__unbind(md));
	free_dev(md);
}

void dm_destroy(struct mapped_device *md)
{
	__dm_destroy(md, true);
}

void dm_destroy_immediate(struct mapped_device *md)
{
	__dm_destroy(md, false);
}

void dm_put(struct mapped_device *md)
{
	atomic_dec(&md->holders);
}
EXPORT_SYMBOL_GPL(dm_put);

<<<<<<< HEAD
static int dm_wait_for_completion(struct mapped_device *md, int interruptible)
{
	int r = 0;
	DECLARE_WAITQUEUE(wait, current);

	add_wait_queue(&md->wait, &wait);

	while (1) {
		set_current_state(interruptible);

		if (!md_in_flight(md))
			break;

		if (interruptible == TASK_INTERRUPTIBLE &&
		    signal_pending(current)) {
=======
static bool dm_in_flight_bios(struct mapped_device *md)
{
	int cpu;
	unsigned long sum = 0;

	for_each_possible_cpu(cpu)
		sum += *per_cpu_ptr(md->pending_io, cpu);

	return sum != 0;
}

static int dm_wait_for_bios_completion(struct mapped_device *md, unsigned int task_state)
{
	int r = 0;
	DEFINE_WAIT(wait);

	while (true) {
		prepare_to_wait(&md->wait, &wait, task_state);

		if (!dm_in_flight_bios(md))
			break;

		if (signal_pending_state(task_state, current)) {
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
			r = -EINTR;
			break;
		}

		io_schedule();
	}
<<<<<<< HEAD
	set_current_state(TASK_RUNNING);

	remove_wait_queue(&md->wait, &wait);
=======
	finish_wait(&md->wait, &wait);

	smp_rmb();

	return r;
}

static int dm_wait_for_completion(struct mapped_device *md, unsigned int task_state)
{
	int r = 0;

	if (!queue_is_mq(md->queue))
		return dm_wait_for_bios_completion(md, task_state);

	while (true) {
		if (!blk_mq_queue_inflight(md->queue))
			break;

		if (signal_pending_state(task_state, current)) {
			r = -EINTR;
			break;
		}

		fsleep(5000);
	}
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	return r;
}

/*
 * Process the deferred bios
 */
static void dm_wq_work(struct work_struct *work)
{
<<<<<<< HEAD
	struct mapped_device *md = container_of(work, struct mapped_device,
						work);
	struct bio *c;

	down_read(&md->io_lock);

	while (!test_bit(DMF_BLOCK_IO_FOR_SUSPEND, &md->flags)) {
		spin_lock_irq(&md->deferred_lock);
		c = bio_list_pop(&md->deferred);
		spin_unlock_irq(&md->deferred_lock);

		if (!c)
			break;

		up_read(&md->io_lock);

		if (dm_request_based(md))
			generic_make_request(c);
		else
			__split_and_process_bio(md, c);

		down_read(&md->io_lock);
	}

	up_read(&md->io_lock);
=======
	struct mapped_device *md = container_of(work, struct mapped_device, work);
	struct bio *bio;

	while (!test_bit(DMF_BLOCK_IO_FOR_SUSPEND, &md->flags)) {
		spin_lock_irq(&md->deferred_lock);
		bio = bio_list_pop(&md->deferred);
		spin_unlock_irq(&md->deferred_lock);

		if (!bio)
			break;

		submit_bio_noacct(bio);
		cond_resched();
	}
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

static void dm_queue_flush(struct mapped_device *md)
{
	clear_bit(DMF_BLOCK_IO_FOR_SUSPEND, &md->flags);
<<<<<<< HEAD
	smp_mb__after_clear_bit();
=======
	smp_mb__after_atomic();
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	queue_work(md->wq, &md->work);
}

/*
 * Swap in a new table, returning the old one for the caller to destroy.
 */
struct dm_table *dm_swap_table(struct mapped_device *md, struct dm_table *table)
{
<<<<<<< HEAD
	struct dm_table *map = ERR_PTR(-EINVAL);
=======
	struct dm_table *live_map = NULL, *map = ERR_PTR(-EINVAL);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	struct queue_limits limits;
	int r;

	mutex_lock(&md->suspend_lock);

	/* device must be suspended */
	if (!dm_suspended_md(md))
		goto out;

<<<<<<< HEAD
	r = dm_calculate_queue_limits(table, &limits);
	if (r) {
		map = ERR_PTR(r);
		goto out;
	}

	map = __bind(md, table, &limits);
=======
	/*
	 * If the new table has no data devices, retain the existing limits.
	 * This helps multipath with queue_if_no_path if all paths disappear,
	 * then new I/O is queued based on these limits, and then some paths
	 * reappear.
	 */
	if (dm_table_has_no_data_devices(table)) {
		live_map = dm_get_live_table_fast(md);
		if (live_map)
			limits = md->queue->limits;
		dm_put_live_table_fast(md);
	}

	if (!live_map) {
		r = dm_calculate_queue_limits(table, &limits);
		if (r) {
			map = ERR_PTR(r);
			goto out;
		}
	}

	map = __bind(md, table, &limits);
	dm_issue_global_event();
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

out:
	mutex_unlock(&md->suspend_lock);
	return map;
}

/*
 * Functions to lock and unlock any filesystem running on the
 * device.
 */
static int lock_fs(struct mapped_device *md)
{
	int r;

<<<<<<< HEAD
	WARN_ON(md->frozen_sb);

	md->frozen_sb = freeze_bdev(md->bdev);
	if (IS_ERR(md->frozen_sb)) {
		r = PTR_ERR(md->frozen_sb);
		md->frozen_sb = NULL;
		return r;
	}

	set_bit(DMF_FROZEN, &md->flags);

	return 0;
=======
	WARN_ON(test_bit(DMF_FROZEN, &md->flags));

	r = bdev_freeze(md->disk->part0);
	if (!r)
		set_bit(DMF_FROZEN, &md->flags);
	return r;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

static void unlock_fs(struct mapped_device *md)
{
	if (!test_bit(DMF_FROZEN, &md->flags))
		return;
<<<<<<< HEAD

	thaw_bdev(md->bdev, md->frozen_sb);
	md->frozen_sb = NULL;
=======
	bdev_thaw(md->disk->part0);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	clear_bit(DMF_FROZEN, &md->flags);
}

/*
<<<<<<< HEAD
=======
 * @suspend_flags: DM_SUSPEND_LOCKFS_FLAG and/or DM_SUSPEND_NOFLUSH_FLAG
 * @task_state: e.g. TASK_INTERRUPTIBLE or TASK_UNINTERRUPTIBLE
 * @dmf_suspended_flag: DMF_SUSPENDED or DMF_SUSPENDED_INTERNALLY
 *
 * If __dm_suspend returns 0, the device is completely quiescent
 * now. There is no request-processing activity. All new requests
 * are being added to md->deferred list.
 */
static int __dm_suspend(struct mapped_device *md, struct dm_table *map,
			unsigned int suspend_flags, unsigned int task_state,
			int dmf_suspended_flag)
{
	bool do_lockfs = suspend_flags & DM_SUSPEND_LOCKFS_FLAG;
	bool noflush = suspend_flags & DM_SUSPEND_NOFLUSH_FLAG;
	int r;

	lockdep_assert_held(&md->suspend_lock);

	/*
	 * DMF_NOFLUSH_SUSPENDING must be set before presuspend.
	 * This flag is cleared before dm_suspend returns.
	 */
	if (noflush)
		set_bit(DMF_NOFLUSH_SUSPENDING, &md->flags);
	else
		DMDEBUG("%s: suspending with flush", dm_device_name(md));

	/*
	 * This gets reverted if there's an error later and the targets
	 * provide the .presuspend_undo hook.
	 */
	dm_table_presuspend_targets(map);

	/*
	 * Flush I/O to the device.
	 * Any I/O submitted after lock_fs() may not be flushed.
	 * noflush takes precedence over do_lockfs.
	 * (lock_fs() flushes I/Os and waits for them to complete.)
	 */
	if (!noflush && do_lockfs) {
		r = lock_fs(md);
		if (r) {
			dm_table_presuspend_undo_targets(map);
			return r;
		}
	}

	/*
	 * Here we must make sure that no processes are submitting requests
	 * to target drivers i.e. no one may be executing
	 * dm_split_and_process_bio from dm_submit_bio.
	 *
	 * To get all processes out of dm_split_and_process_bio in dm_submit_bio,
	 * we take the write lock. To prevent any process from reentering
	 * dm_split_and_process_bio from dm_submit_bio and quiesce the thread
	 * (dm_wq_work), we set DMF_BLOCK_IO_FOR_SUSPEND and call
	 * flush_workqueue(md->wq).
	 */
	set_bit(DMF_BLOCK_IO_FOR_SUSPEND, &md->flags);
	if (map)
		synchronize_srcu(&md->io_barrier);

	/*
	 * Stop md->queue before flushing md->wq in case request-based
	 * dm defers requests to md->wq from md->queue.
	 */
	if (dm_request_based(md))
		dm_stop_queue(md->queue);

	flush_workqueue(md->wq);

	/*
	 * At this point no more requests are entering target request routines.
	 * We call dm_wait_for_completion to wait for all existing requests
	 * to finish.
	 */
	r = dm_wait_for_completion(md, task_state);
	if (!r)
		set_bit(dmf_suspended_flag, &md->flags);

	if (noflush)
		clear_bit(DMF_NOFLUSH_SUSPENDING, &md->flags);
	if (map)
		synchronize_srcu(&md->io_barrier);

	/* were we interrupted ? */
	if (r < 0) {
		dm_queue_flush(md);

		if (dm_request_based(md))
			dm_start_queue(md->queue);

		unlock_fs(md);
		dm_table_presuspend_undo_targets(map);
		/* pushback list is already flushed, so skip flush */
	}

	return r;
}

/*
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 * We need to be able to change a mapping table under a mounted
 * filesystem.  For example we might want to move some data in
 * the background.  Before the table can be swapped with
 * dm_bind_table, dm_suspend must be called to flush any in
 * flight bios and ensure that any further io gets deferred.
 */
/*
 * Suspend mechanism in request-based dm.
 *
 * 1. Flush all I/Os by lock_fs() if needed.
 * 2. Stop dispatching any I/O by stopping the request_queue.
 * 3. Wait for all in-flight I/Os to be completed or requeued.
 *
 * To abort suspend, start the request_queue.
 */
<<<<<<< HEAD
int dm_suspend(struct mapped_device *md, unsigned suspend_flags)
{
	struct dm_table *map = NULL;
	int r = 0;
	int do_lockfs = suspend_flags & DM_SUSPEND_LOCKFS_FLAG ? 1 : 0;
	int noflush = suspend_flags & DM_SUSPEND_NOFLUSH_FLAG ? 1 : 0;

	mutex_lock(&md->suspend_lock);
=======
int dm_suspend(struct mapped_device *md, unsigned int suspend_flags)
{
	struct dm_table *map = NULL;
	int r = 0;

retry:
	mutex_lock_nested(&md->suspend_lock, SINGLE_DEPTH_NESTING);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	if (dm_suspended_md(md)) {
		r = -EINVAL;
		goto out_unlock;
	}

<<<<<<< HEAD
	map = dm_get_live_table(md);

	/*
	 * DMF_NOFLUSH_SUSPENDING must be set before presuspend.
	 * This flag is cleared before dm_suspend returns.
	 */
	if (noflush)
		set_bit(DMF_NOFLUSH_SUSPENDING, &md->flags);

	/* This does not get reverted if there's an error later. */
	dm_table_presuspend_targets(map);

	/*
	 * Flush I/O to the device.
	 * Any I/O submitted after lock_fs() may not be flushed.
	 * noflush takes precedence over do_lockfs.
	 * (lock_fs() flushes I/Os and waits for them to complete.)
	 */
	if (!noflush && do_lockfs) {
		r = lock_fs(md);
		if (r)
			goto out;
	}

	/*
	 * Here we must make sure that no processes are submitting requests
	 * to target drivers i.e. no one may be executing
	 * __split_and_process_bio. This is called from dm_request and
	 * dm_wq_work.
	 *
	 * To get all processes out of __split_and_process_bio in dm_request,
	 * we take the write lock. To prevent any process from reentering
	 * __split_and_process_bio from dm_request and quiesce the thread
	 * (dm_wq_work), we set BMF_BLOCK_IO_FOR_SUSPEND and call
	 * flush_workqueue(md->wq).
	 */
	down_write(&md->io_lock);
	set_bit(DMF_BLOCK_IO_FOR_SUSPEND, &md->flags);
	up_write(&md->io_lock);

	/*
	 * Stop md->queue before flushing md->wq in case request-based
	 * dm defers requests to md->wq from md->queue.
	 */
	if (dm_request_based(md))
		stop_queue(md->queue);

	flush_workqueue(md->wq);

	/*
	 * At this point no more requests are entering target request routines.
	 * We call dm_wait_for_completion to wait for all existing requests
	 * to finish.
	 */
	r = dm_wait_for_completion(md, TASK_INTERRUPTIBLE);

	down_write(&md->io_lock);
	if (noflush)
		clear_bit(DMF_NOFLUSH_SUSPENDING, &md->flags);
	up_write(&md->io_lock);

	/* were we interrupted ? */
	if (r < 0) {
		dm_queue_flush(md);

		if (dm_request_based(md))
			start_queue(md->queue);

		unlock_fs(md);
		goto out; /* pushback list is already flushed, so skip flush */
	}

	/*
	 * If dm_wait_for_completion returned 0, the device is completely
	 * quiescent now. There is no request-processing activity. All new
	 * requests are being added to md->deferred list.
	 */

	set_bit(DMF_SUSPENDED, &md->flags);

	dm_table_postsuspend_targets(map);

out:
	dm_table_put(map);
=======
	if (dm_suspended_internally_md(md)) {
		/* already internally suspended, wait for internal resume */
		mutex_unlock(&md->suspend_lock);
		r = wait_on_bit(&md->flags, DMF_SUSPENDED_INTERNALLY, TASK_INTERRUPTIBLE);
		if (r)
			return r;
		goto retry;
	}

	map = rcu_dereference_protected(md->map, lockdep_is_held(&md->suspend_lock));
	if (!map) {
		/* avoid deadlock with fs/namespace.c:do_mount() */
		suspend_flags &= ~DM_SUSPEND_LOCKFS_FLAG;
	}

	r = __dm_suspend(md, map, suspend_flags, TASK_INTERRUPTIBLE, DMF_SUSPENDED);
	if (r)
		goto out_unlock;

	set_bit(DMF_POST_SUSPENDING, &md->flags);
	dm_table_postsuspend_targets(map);
	clear_bit(DMF_POST_SUSPENDING, &md->flags);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

out_unlock:
	mutex_unlock(&md->suspend_lock);
	return r;
}

<<<<<<< HEAD
int dm_resume(struct mapped_device *md)
{
	int r = -EINVAL;
	struct dm_table *map = NULL;

	mutex_lock(&md->suspend_lock);
	if (!dm_suspended_md(md))
		goto out;

	map = dm_get_live_table(md);
	if (!map || !dm_table_get_size(map))
		goto out;

	r = dm_table_resume_targets(map);
	if (r)
		goto out;
=======
static int __dm_resume(struct mapped_device *md, struct dm_table *map)
{
	if (map) {
		int r = dm_table_resume_targets(map);

		if (r)
			return r;
	}
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	dm_queue_flush(md);

	/*
	 * Flushing deferred I/Os must be done after targets are resumed
	 * so that mapping of targets can work correctly.
	 * Request-based dm is queueing the deferred I/Os in its request_queue.
	 */
	if (dm_request_based(md))
<<<<<<< HEAD
		start_queue(md->queue);

	unlock_fs(md);

	clear_bit(DMF_SUSPENDED, &md->flags);

	r = 0;
out:
	dm_table_put(map);
=======
		dm_start_queue(md->queue);

	unlock_fs(md);

	return 0;
}

int dm_resume(struct mapped_device *md)
{
	int r;
	struct dm_table *map = NULL;

retry:
	r = -EINVAL;
	mutex_lock_nested(&md->suspend_lock, SINGLE_DEPTH_NESTING);

	if (!dm_suspended_md(md))
		goto out;

	if (dm_suspended_internally_md(md)) {
		/* already internally suspended, wait for internal resume */
		mutex_unlock(&md->suspend_lock);
		r = wait_on_bit(&md->flags, DMF_SUSPENDED_INTERNALLY, TASK_INTERRUPTIBLE);
		if (r)
			return r;
		goto retry;
	}

	map = rcu_dereference_protected(md->map, lockdep_is_held(&md->suspend_lock));
	if (!map || !dm_table_get_size(map))
		goto out;

	r = __dm_resume(md, map);
	if (r)
		goto out;

	clear_bit(DMF_SUSPENDED, &md->flags);
out:
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	mutex_unlock(&md->suspend_lock);

	return r;
}

<<<<<<< HEAD
/*-----------------------------------------------------------------
 * Event notification.
 *---------------------------------------------------------------*/
int dm_kobject_uevent(struct mapped_device *md, enum kobject_action action,
		       unsigned cookie)
{
	char udev_cookie[DM_COOKIE_LENGTH];
	char *envp[] = { udev_cookie, NULL };

	if (!cookie)
		return kobject_uevent(&disk_to_dev(md->disk)->kobj, action);
	else {
		snprintf(udev_cookie, DM_COOKIE_LENGTH, "%s=%u",
			 DM_COOKIE_ENV_VAR_NAME, cookie);
		return kobject_uevent_env(&disk_to_dev(md->disk)->kobj,
					  action, envp);
	}
=======
/*
 * Internal suspend/resume works like userspace-driven suspend. It waits
 * until all bios finish and prevents issuing new bios to the target drivers.
 * It may be used only from the kernel.
 */

static void __dm_internal_suspend(struct mapped_device *md, unsigned int suspend_flags)
{
	struct dm_table *map = NULL;

	lockdep_assert_held(&md->suspend_lock);

	if (md->internal_suspend_count++)
		return; /* nested internal suspend */

	if (dm_suspended_md(md)) {
		set_bit(DMF_SUSPENDED_INTERNALLY, &md->flags);
		return; /* nest suspend */
	}

	map = rcu_dereference_protected(md->map, lockdep_is_held(&md->suspend_lock));

	/*
	 * Using TASK_UNINTERRUPTIBLE because only NOFLUSH internal suspend is
	 * supported.  Properly supporting a TASK_INTERRUPTIBLE internal suspend
	 * would require changing .presuspend to return an error -- avoid this
	 * until there is a need for more elaborate variants of internal suspend.
	 */
	(void) __dm_suspend(md, map, suspend_flags, TASK_UNINTERRUPTIBLE,
			    DMF_SUSPENDED_INTERNALLY);

	set_bit(DMF_POST_SUSPENDING, &md->flags);
	dm_table_postsuspend_targets(map);
	clear_bit(DMF_POST_SUSPENDING, &md->flags);
}

static void __dm_internal_resume(struct mapped_device *md)
{
	int r;
	struct dm_table *map;

	BUG_ON(!md->internal_suspend_count);

	if (--md->internal_suspend_count)
		return; /* resume from nested internal suspend */

	if (dm_suspended_md(md))
		goto done; /* resume from nested suspend */

	map = rcu_dereference_protected(md->map, lockdep_is_held(&md->suspend_lock));
	r = __dm_resume(md, map);
	if (r) {
		/*
		 * If a preresume method of some target failed, we are in a
		 * tricky situation. We can't return an error to the caller. We
		 * can't fake success because then the "resume" and
		 * "postsuspend" methods would not be paired correctly, and it
		 * would break various targets, for example it would cause list
		 * corruption in the "origin" target.
		 *
		 * So, we fake normal suspend here, to make sure that the
		 * "resume" and "postsuspend" methods will be paired correctly.
		 */
		DMERR("Preresume method failed: %d", r);
		set_bit(DMF_SUSPENDED, &md->flags);
	}
done:
	clear_bit(DMF_SUSPENDED_INTERNALLY, &md->flags);
	smp_mb__after_atomic();
	wake_up_bit(&md->flags, DMF_SUSPENDED_INTERNALLY);
}

void dm_internal_suspend_noflush(struct mapped_device *md)
{
	mutex_lock(&md->suspend_lock);
	__dm_internal_suspend(md, DM_SUSPEND_NOFLUSH_FLAG);
	mutex_unlock(&md->suspend_lock);
}
EXPORT_SYMBOL_GPL(dm_internal_suspend_noflush);

void dm_internal_resume(struct mapped_device *md)
{
	mutex_lock(&md->suspend_lock);
	__dm_internal_resume(md);
	mutex_unlock(&md->suspend_lock);
}
EXPORT_SYMBOL_GPL(dm_internal_resume);

/*
 * Fast variants of internal suspend/resume hold md->suspend_lock,
 * which prevents interaction with userspace-driven suspend.
 */

void dm_internal_suspend_fast(struct mapped_device *md)
{
	mutex_lock(&md->suspend_lock);
	if (dm_suspended_md(md) || dm_suspended_internally_md(md))
		return;

	set_bit(DMF_BLOCK_IO_FOR_SUSPEND, &md->flags);
	synchronize_srcu(&md->io_barrier);
	flush_workqueue(md->wq);
	dm_wait_for_completion(md, TASK_UNINTERRUPTIBLE);
}
EXPORT_SYMBOL_GPL(dm_internal_suspend_fast);

void dm_internal_resume_fast(struct mapped_device *md)
{
	if (dm_suspended_md(md) || dm_suspended_internally_md(md))
		goto done;

	dm_queue_flush(md);

done:
	mutex_unlock(&md->suspend_lock);
}
EXPORT_SYMBOL_GPL(dm_internal_resume_fast);

/*
 *---------------------------------------------------------------
 * Event notification.
 *---------------------------------------------------------------
 */
int dm_kobject_uevent(struct mapped_device *md, enum kobject_action action,
		      unsigned int cookie, bool need_resize_uevent)
{
	int r;
	unsigned int noio_flag;
	char udev_cookie[DM_COOKIE_LENGTH];
	char *envp[3] = { NULL, NULL, NULL };
	char **envpp = envp;
	if (cookie) {
		snprintf(udev_cookie, DM_COOKIE_LENGTH, "%s=%u",
			 DM_COOKIE_ENV_VAR_NAME, cookie);
		*envpp++ = udev_cookie;
	}
	if (need_resize_uevent) {
		*envpp++ = "RESIZE=1";
	}

	noio_flag = memalloc_noio_save();

	r = kobject_uevent_env(&disk_to_dev(md->disk)->kobj, action, envp);

	memalloc_noio_restore(noio_flag);

	return r;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

uint32_t dm_next_uevent_seq(struct mapped_device *md)
{
	return atomic_add_return(1, &md->uevent_seq);
}

uint32_t dm_get_event_nr(struct mapped_device *md)
{
	return atomic_read(&md->event_nr);
}

int dm_wait_event(struct mapped_device *md, int event_nr)
{
	return wait_event_interruptible(md->eventq,
			(event_nr != atomic_read(&md->event_nr)));
}

void dm_uevent_add(struct mapped_device *md, struct list_head *elist)
{
	unsigned long flags;

	spin_lock_irqsave(&md->uevent_lock, flags);
	list_add(elist, &md->uevent_list);
	spin_unlock_irqrestore(&md->uevent_lock, flags);
}

/*
 * The gendisk is only valid as long as you have a reference
 * count on 'md'.
 */
struct gendisk *dm_disk(struct mapped_device *md)
{
	return md->disk;
}
<<<<<<< HEAD
=======
EXPORT_SYMBOL_GPL(dm_disk);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

struct kobject *dm_kobject(struct mapped_device *md)
{
	return &md->kobj_holder.kobj;
}

struct mapped_device *dm_get_from_kobject(struct kobject *kobj)
{
	struct mapped_device *md;

	md = container_of(kobj, struct mapped_device, kobj_holder.kobj);

<<<<<<< HEAD
	if (test_bit(DMF_FREEING, &md->flags) ||
	    dm_deleting_md(md))
		return NULL;

	dm_get(md);
=======
	spin_lock(&_minor_lock);
	if (test_bit(DMF_FREEING, &md->flags) || dm_deleting_md(md)) {
		md = NULL;
		goto out;
	}
	dm_get(md);
out:
	spin_unlock(&_minor_lock);

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	return md;
}

int dm_suspended_md(struct mapped_device *md)
{
	return test_bit(DMF_SUSPENDED, &md->flags);
}

<<<<<<< HEAD
int dm_suspended(struct dm_target *ti)
{
	return dm_suspended_md(dm_table_get_md(ti->table));
}
EXPORT_SYMBOL_GPL(dm_suspended);

int dm_noflush_suspending(struct dm_target *ti)
{
	return __noflush_suspending(dm_table_get_md(ti->table));
}
EXPORT_SYMBOL_GPL(dm_noflush_suspending);

struct dm_md_mempools *dm_alloc_md_mempools(unsigned type, unsigned integrity)
{
	struct dm_md_mempools *pools = kmalloc(sizeof(*pools), GFP_KERNEL);
	unsigned int pool_size = (type == DM_TYPE_BIO_BASED) ? 16 : MIN_IOS;

	if (!pools)
		return NULL;

	pools->io_pool = (type == DM_TYPE_BIO_BASED) ?
			 mempool_create_slab_pool(MIN_IOS, _io_cache) :
			 mempool_create_slab_pool(MIN_IOS, _rq_bio_info_cache);
	if (!pools->io_pool)
		goto free_pools_and_out;

	pools->tio_pool = (type == DM_TYPE_BIO_BASED) ?
			  mempool_create_slab_pool(MIN_IOS, _tio_cache) :
			  mempool_create_slab_pool(MIN_IOS, _rq_tio_cache);
	if (!pools->tio_pool)
		goto free_io_pool_and_out;

	pools->bs = bioset_create(pool_size, 0);
	if (!pools->bs)
		goto free_tio_pool_and_out;

	if (integrity && bioset_integrity_create(pools->bs, pool_size))
		goto free_bioset_and_out;

	return pools;

free_bioset_and_out:
	bioset_free(pools->bs);

free_tio_pool_and_out:
	mempool_destroy(pools->tio_pool);

free_io_pool_and_out:
	mempool_destroy(pools->io_pool);

free_pools_and_out:
	kfree(pools);

	return NULL;
}

=======
static int dm_post_suspending_md(struct mapped_device *md)
{
	return test_bit(DMF_POST_SUSPENDING, &md->flags);
}

int dm_suspended_internally_md(struct mapped_device *md)
{
	return test_bit(DMF_SUSPENDED_INTERNALLY, &md->flags);
}

int dm_test_deferred_remove_flag(struct mapped_device *md)
{
	return test_bit(DMF_DEFERRED_REMOVE, &md->flags);
}

int dm_suspended(struct dm_target *ti)
{
	return dm_suspended_md(ti->table->md);
}
EXPORT_SYMBOL_GPL(dm_suspended);

int dm_post_suspending(struct dm_target *ti)
{
	return dm_post_suspending_md(ti->table->md);
}
EXPORT_SYMBOL_GPL(dm_post_suspending);

int dm_noflush_suspending(struct dm_target *ti)
{
	return __noflush_suspending(ti->table->md);
}
EXPORT_SYMBOL_GPL(dm_noflush_suspending);

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
void dm_free_md_mempools(struct dm_md_mempools *pools)
{
	if (!pools)
		return;

<<<<<<< HEAD
	if (pools->io_pool)
		mempool_destroy(pools->io_pool);

	if (pools->tio_pool)
		mempool_destroy(pools->tio_pool);

	if (pools->bs)
		bioset_free(pools->bs);
=======
	bioset_exit(&pools->bs);
	bioset_exit(&pools->io_bs);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	kfree(pools);
}

<<<<<<< HEAD
static const struct block_device_operations dm_blk_dops = {
=======
struct dm_pr {
	u64	old_key;
	u64	new_key;
	u32	flags;
	bool	abort;
	bool	fail_early;
	int	ret;
	enum pr_type type;
	struct pr_keys *read_keys;
	struct pr_held_reservation *rsv;
};

static int dm_call_pr(struct block_device *bdev, iterate_devices_callout_fn fn,
		      struct dm_pr *pr)
{
	struct mapped_device *md = bdev->bd_disk->private_data;
	struct dm_table *table;
	struct dm_target *ti;
	int ret = -ENOTTY, srcu_idx;

	table = dm_get_live_table(md, &srcu_idx);
	if (!table || !dm_table_get_size(table))
		goto out;

	/* We only support devices that have a single target */
	if (table->num_targets != 1)
		goto out;
	ti = dm_table_get_target(table, 0);

	if (dm_suspended_md(md)) {
		ret = -EAGAIN;
		goto out;
	}

	ret = -EINVAL;
	if (!ti->type->iterate_devices)
		goto out;

	ti->type->iterate_devices(ti, fn, pr);
	ret = 0;
out:
	dm_put_live_table(md, srcu_idx);
	return ret;
}

/*
 * For register / unregister we need to manually call out to every path.
 */
static int __dm_pr_register(struct dm_target *ti, struct dm_dev *dev,
			    sector_t start, sector_t len, void *data)
{
	struct dm_pr *pr = data;
	const struct pr_ops *ops = dev->bdev->bd_disk->fops->pr_ops;
	int ret;

	if (!ops || !ops->pr_register) {
		pr->ret = -EOPNOTSUPP;
		return -1;
	}

	ret = ops->pr_register(dev->bdev, pr->old_key, pr->new_key, pr->flags);
	if (!ret)
		return 0;

	if (!pr->ret)
		pr->ret = ret;

	if (pr->fail_early)
		return -1;

	return 0;
}

static int dm_pr_register(struct block_device *bdev, u64 old_key, u64 new_key,
			  u32 flags)
{
	struct dm_pr pr = {
		.old_key	= old_key,
		.new_key	= new_key,
		.flags		= flags,
		.fail_early	= true,
		.ret		= 0,
	};
	int ret;

	ret = dm_call_pr(bdev, __dm_pr_register, &pr);
	if (ret) {
		/* Didn't even get to register a path */
		return ret;
	}

	if (!pr.ret)
		return 0;
	ret = pr.ret;

	if (!new_key)
		return ret;

	/* unregister all paths if we failed to register any path */
	pr.old_key = new_key;
	pr.new_key = 0;
	pr.flags = 0;
	pr.fail_early = false;
	(void) dm_call_pr(bdev, __dm_pr_register, &pr);
	return ret;
}


static int __dm_pr_reserve(struct dm_target *ti, struct dm_dev *dev,
			   sector_t start, sector_t len, void *data)
{
	struct dm_pr *pr = data;
	const struct pr_ops *ops = dev->bdev->bd_disk->fops->pr_ops;

	if (!ops || !ops->pr_reserve) {
		pr->ret = -EOPNOTSUPP;
		return -1;
	}

	pr->ret = ops->pr_reserve(dev->bdev, pr->old_key, pr->type, pr->flags);
	if (!pr->ret)
		return -1;

	return 0;
}

static int dm_pr_reserve(struct block_device *bdev, u64 key, enum pr_type type,
			 u32 flags)
{
	struct dm_pr pr = {
		.old_key	= key,
		.flags		= flags,
		.type		= type,
		.fail_early	= false,
		.ret		= 0,
	};
	int ret;

	ret = dm_call_pr(bdev, __dm_pr_reserve, &pr);
	if (ret)
		return ret;

	return pr.ret;
}

/*
 * If there is a non-All Registrants type of reservation, the release must be
 * sent down the holding path. For the cases where there is no reservation or
 * the path is not the holder the device will also return success, so we must
 * try each path to make sure we got the correct path.
 */
static int __dm_pr_release(struct dm_target *ti, struct dm_dev *dev,
			   sector_t start, sector_t len, void *data)
{
	struct dm_pr *pr = data;
	const struct pr_ops *ops = dev->bdev->bd_disk->fops->pr_ops;

	if (!ops || !ops->pr_release) {
		pr->ret = -EOPNOTSUPP;
		return -1;
	}

	pr->ret = ops->pr_release(dev->bdev, pr->old_key, pr->type);
	if (pr->ret)
		return -1;

	return 0;
}

static int dm_pr_release(struct block_device *bdev, u64 key, enum pr_type type)
{
	struct dm_pr pr = {
		.old_key	= key,
		.type		= type,
		.fail_early	= false,
	};
	int ret;

	ret = dm_call_pr(bdev, __dm_pr_release, &pr);
	if (ret)
		return ret;

	return pr.ret;
}

static int __dm_pr_preempt(struct dm_target *ti, struct dm_dev *dev,
			   sector_t start, sector_t len, void *data)
{
	struct dm_pr *pr = data;
	const struct pr_ops *ops = dev->bdev->bd_disk->fops->pr_ops;

	if (!ops || !ops->pr_preempt) {
		pr->ret = -EOPNOTSUPP;
		return -1;
	}

	pr->ret = ops->pr_preempt(dev->bdev, pr->old_key, pr->new_key, pr->type,
				  pr->abort);
	if (!pr->ret)
		return -1;

	return 0;
}

static int dm_pr_preempt(struct block_device *bdev, u64 old_key, u64 new_key,
			 enum pr_type type, bool abort)
{
	struct dm_pr pr = {
		.new_key	= new_key,
		.old_key	= old_key,
		.type		= type,
		.fail_early	= false,
	};
	int ret;

	ret = dm_call_pr(bdev, __dm_pr_preempt, &pr);
	if (ret)
		return ret;

	return pr.ret;
}

static int dm_pr_clear(struct block_device *bdev, u64 key)
{
	struct mapped_device *md = bdev->bd_disk->private_data;
	const struct pr_ops *ops;
	int r, srcu_idx;

	r = dm_prepare_ioctl(md, &srcu_idx, &bdev);
	if (r < 0)
		goto out;

	ops = bdev->bd_disk->fops->pr_ops;
	if (ops && ops->pr_clear)
		r = ops->pr_clear(bdev, key);
	else
		r = -EOPNOTSUPP;
out:
	dm_unprepare_ioctl(md, srcu_idx);
	return r;
}

static int __dm_pr_read_keys(struct dm_target *ti, struct dm_dev *dev,
			     sector_t start, sector_t len, void *data)
{
	struct dm_pr *pr = data;
	const struct pr_ops *ops = dev->bdev->bd_disk->fops->pr_ops;

	if (!ops || !ops->pr_read_keys) {
		pr->ret = -EOPNOTSUPP;
		return -1;
	}

	pr->ret = ops->pr_read_keys(dev->bdev, pr->read_keys);
	if (!pr->ret)
		return -1;

	return 0;
}

static int dm_pr_read_keys(struct block_device *bdev, struct pr_keys *keys)
{
	struct dm_pr pr = {
		.read_keys = keys,
	};
	int ret;

	ret = dm_call_pr(bdev, __dm_pr_read_keys, &pr);
	if (ret)
		return ret;

	return pr.ret;
}

static int __dm_pr_read_reservation(struct dm_target *ti, struct dm_dev *dev,
				    sector_t start, sector_t len, void *data)
{
	struct dm_pr *pr = data;
	const struct pr_ops *ops = dev->bdev->bd_disk->fops->pr_ops;

	if (!ops || !ops->pr_read_reservation) {
		pr->ret = -EOPNOTSUPP;
		return -1;
	}

	pr->ret = ops->pr_read_reservation(dev->bdev, pr->rsv);
	if (!pr->ret)
		return -1;

	return 0;
}

static int dm_pr_read_reservation(struct block_device *bdev,
				  struct pr_held_reservation *rsv)
{
	struct dm_pr pr = {
		.rsv = rsv,
	};
	int ret;

	ret = dm_call_pr(bdev, __dm_pr_read_reservation, &pr);
	if (ret)
		return ret;

	return pr.ret;
}

static const struct pr_ops dm_pr_ops = {
	.pr_register	= dm_pr_register,
	.pr_reserve	= dm_pr_reserve,
	.pr_release	= dm_pr_release,
	.pr_preempt	= dm_pr_preempt,
	.pr_clear	= dm_pr_clear,
	.pr_read_keys	= dm_pr_read_keys,
	.pr_read_reservation = dm_pr_read_reservation,
};

static const struct block_device_operations dm_blk_dops = {
	.submit_bio = dm_submit_bio,
	.poll_bio = dm_poll_bio,
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	.open = dm_blk_open,
	.release = dm_blk_close,
	.ioctl = dm_blk_ioctl,
	.getgeo = dm_blk_getgeo,
<<<<<<< HEAD
	.owner = THIS_MODULE
};

EXPORT_SYMBOL(dm_get_mapinfo);
=======
	.report_zones = dm_blk_report_zones,
	.pr_ops = &dm_pr_ops,
	.owner = THIS_MODULE
};

static const struct block_device_operations dm_rq_blk_dops = {
	.open = dm_blk_open,
	.release = dm_blk_close,
	.ioctl = dm_blk_ioctl,
	.getgeo = dm_blk_getgeo,
	.pr_ops = &dm_pr_ops,
	.owner = THIS_MODULE
};

static const struct dax_operations dm_dax_ops = {
	.direct_access = dm_dax_direct_access,
	.zero_page_range = dm_dax_zero_page_range,
	.recovery_write = dm_dax_recovery_write,
};
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

/*
 * module hooks
 */
module_init(dm_init);
module_exit(dm_exit);

module_param(major, uint, 0);
MODULE_PARM_DESC(major, "The major number of the device mapper");
<<<<<<< HEAD
MODULE_DESCRIPTION(DM_NAME " driver");
MODULE_AUTHOR("Joe Thornber <dm-devel@redhat.com>");
=======

module_param(reserved_bio_based_ios, uint, 0644);
MODULE_PARM_DESC(reserved_bio_based_ios, "Reserved IOs in bio-based mempools");

module_param(dm_numa_node, int, 0644);
MODULE_PARM_DESC(dm_numa_node, "NUMA node for DM device memory allocations");

module_param(swap_bios, int, 0644);
MODULE_PARM_DESC(swap_bios, "Maximum allowed inflight swap IOs");

MODULE_DESCRIPTION(DM_NAME " driver");
MODULE_AUTHOR("Joe Thornber <dm-devel@lists.linux.dev>");
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
MODULE_LICENSE("GPL");
