<<<<<<< HEAD
=======
// SPDX-License-Identifier: GPL-2.0-only
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
/*
 * Copyright (C) 2003 Sistina Software Limited.
 * Copyright (C) 2005-2008 Red Hat, Inc. All rights reserved.
 *
 * This file is released under the GPL.
 */

#include "dm-bio-record.h"

#include <linux/init.h>
#include <linux/mempool.h>
#include <linux/module.h>
#include <linux/pagemap.h>
#include <linux/slab.h>
#include <linux/workqueue.h>
#include <linux/device-mapper.h>
#include <linux/dm-io.h>
#include <linux/dm-dirty-log.h>
#include <linux/dm-kcopyd.h>
#include <linux/dm-region-hash.h>

<<<<<<< HEAD
=======
static struct workqueue_struct *dm_raid1_wq;

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#define DM_MSG_PREFIX "raid1"

#define MAX_RECOVERY 1	/* Maximum number of regions recovered in parallel. */

<<<<<<< HEAD
#define DM_RAID1_HANDLE_ERRORS 0x01
#define errors_handled(p)	((p)->features & DM_RAID1_HANDLE_ERRORS)

static DECLARE_WAIT_QUEUE_HEAD(_kmirrord_recovery_stopped);

/*-----------------------------------------------------------------
 * Mirror set structures.
 *---------------------------------------------------------------*/
=======
#define MAX_NR_MIRRORS	(DM_KCOPYD_MAX_REGIONS + 1)

#define DM_RAID1_HANDLE_ERRORS	0x01
#define DM_RAID1_KEEP_LOG	0x02
#define errors_handled(p)	((p)->features & DM_RAID1_HANDLE_ERRORS)
#define keep_log(p)		((p)->features & DM_RAID1_KEEP_LOG)

static DECLARE_WAIT_QUEUE_HEAD(_kmirrord_recovery_stopped);

/*
 *---------------------------------------------------------------
 * Mirror set structures.
 *---------------------------------------------------------------
 */
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
enum dm_raid1_error {
	DM_RAID1_WRITE_ERROR,
	DM_RAID1_FLUSH_ERROR,
	DM_RAID1_SYNC_ERROR,
	DM_RAID1_READ_ERROR
};

struct mirror {
	struct mirror_set *ms;
	atomic_t error_count;
	unsigned long error_type;
	struct dm_dev *dev;
	sector_t offset;
};

struct mirror_set {
	struct dm_target *ti;
	struct list_head list;

	uint64_t features;

	spinlock_t lock;	/* protects the lists */
	struct bio_list reads;
	struct bio_list writes;
	struct bio_list failures;
	struct bio_list holds;	/* bios are waiting until suspend */

	struct dm_region_hash *rh;
	struct dm_kcopyd_client *kcopyd_client;
	struct dm_io_client *io_client;
<<<<<<< HEAD
	mempool_t *read_record_pool;
=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	/* recovery */
	region_t nr_regions;
	int in_sync;
	int log_failure;
	int leg_failure;
	atomic_t suspend;

	atomic_t default_mirror;	/* Default mirror */

	struct workqueue_struct *kmirrord_wq;
	struct work_struct kmirrord_work;
	struct timer_list timer;
	unsigned long timer_pending;

	struct work_struct trigger_event;

<<<<<<< HEAD
	unsigned nr_mirrors;
	struct mirror mirror[0];
};

=======
	unsigned int nr_mirrors;
	struct mirror mirror[];
};

DECLARE_DM_KCOPYD_THROTTLE_WITH_MODULE_PARM(raid1_resync_throttle,
		"A percentage of time allocated for raid resynchronization");

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
static void wakeup_mirrord(void *context)
{
	struct mirror_set *ms = context;

	queue_work(ms->kmirrord_wq, &ms->kmirrord_work);
}

<<<<<<< HEAD
static void delayed_wake_fn(unsigned long data)
{
	struct mirror_set *ms = (struct mirror_set *) data;
=======
static void delayed_wake_fn(struct timer_list *t)
{
	struct mirror_set *ms = from_timer(ms, t, timer);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	clear_bit(0, &ms->timer_pending);
	wakeup_mirrord(ms);
}

static void delayed_wake(struct mirror_set *ms)
{
	if (test_and_set_bit(0, &ms->timer_pending))
		return;

	ms->timer.expires = jiffies + HZ / 5;
<<<<<<< HEAD
	ms->timer.data = (unsigned long) ms;
	ms->timer.function = delayed_wake_fn;
=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	add_timer(&ms->timer);
}

static void wakeup_all_recovery_waiters(void *context)
{
	wake_up_all(&_kmirrord_recovery_stopped);
}

static void queue_bio(struct mirror_set *ms, struct bio *bio, int rw)
{
	unsigned long flags;
	int should_wake = 0;
	struct bio_list *bl;

	bl = (rw == WRITE) ? &ms->writes : &ms->reads;
	spin_lock_irqsave(&ms->lock, flags);
	should_wake = !(bl->head);
	bio_list_add(bl, bio);
	spin_unlock_irqrestore(&ms->lock, flags);

	if (should_wake)
		wakeup_mirrord(ms);
}

static void dispatch_bios(void *context, struct bio_list *bio_list)
{
	struct mirror_set *ms = context;
	struct bio *bio;

	while ((bio = bio_list_pop(bio_list)))
		queue_bio(ms, bio, WRITE);
}

<<<<<<< HEAD
#define MIN_READ_RECORDS 20
struct dm_raid1_read_record {
	struct mirror *m;
	struct dm_bio_details details;
};

static struct kmem_cache *_dm_raid1_read_record_cache;

=======
struct dm_raid1_bio_record {
	struct mirror *m;
	/* if details->bi_bdev == NULL, details were not saved */
	struct dm_bio_details details;
	region_t write_region;
};

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
/*
 * Every mirror should look like this one.
 */
#define DEFAULT_MIRROR 0

/*
 * This is yucky.  We squirrel the mirror struct away inside
 * bi_next for read/write buffers.  This is safe since the bh
 * doesn't get submitted to the lower levels of block layer.
 */
static struct mirror *bio_get_m(struct bio *bio)
{
	return (struct mirror *) bio->bi_next;
}

static void bio_set_m(struct bio *bio, struct mirror *m)
{
	bio->bi_next = (struct bio *) m;
}

static struct mirror *get_default_mirror(struct mirror_set *ms)
{
	return &ms->mirror[atomic_read(&ms->default_mirror)];
}

static void set_default_mirror(struct mirror *m)
{
	struct mirror_set *ms = m->ms;
	struct mirror *m0 = &(ms->mirror[0]);

	atomic_set(&ms->default_mirror, m - m0);
}

static struct mirror *get_valid_mirror(struct mirror_set *ms)
{
	struct mirror *m;

	for (m = ms->mirror; m < ms->mirror + ms->nr_mirrors; m++)
		if (!atomic_read(&m->error_count))
			return m;

	return NULL;
}

/* fail_mirror
 * @m: mirror device to fail
 * @error_type: one of the enum's, DM_RAID1_*_ERROR
 *
 * If errors are being handled, record the type of
 * error encountered for this device.  If this type
 * of error has already been recorded, we can return;
 * otherwise, we must signal userspace by triggering
 * an event.  Additionally, if the device is the
 * primary device, we must choose a new primary, but
 * only if the mirror is in-sync.
 *
 * This function must not block.
 */
static void fail_mirror(struct mirror *m, enum dm_raid1_error error_type)
{
	struct mirror_set *ms = m->ms;
	struct mirror *new;

	ms->leg_failure = 1;

	/*
	 * error_count is used for nothing more than a
	 * simple way to tell if a device has encountered
	 * errors.
	 */
	atomic_inc(&m->error_count);

	if (test_and_set_bit(error_type, &m->error_type))
		return;

	if (!errors_handled(ms))
		return;

	if (m != get_default_mirror(ms))
		goto out;

<<<<<<< HEAD
	if (!ms->in_sync) {
=======
	if (!ms->in_sync && !keep_log(ms)) {
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		/*
		 * Better to issue requests to same failing device
		 * than to risk returning corrupt data.
		 */
<<<<<<< HEAD
		DMERR("Primary mirror (%s) failed while out-of-sync: "
		      "Reads may fail.", m->dev->name);
=======
		DMERR("Primary mirror (%s) failed while out-of-sync: Reads may fail.",
		      m->dev->name);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		goto out;
	}

	new = get_valid_mirror(ms);
	if (new)
		set_default_mirror(new);
	else
		DMWARN("All sides of mirror have failed.");

out:
<<<<<<< HEAD
	schedule_work(&ms->trigger_event);
=======
	queue_work(dm_raid1_wq, &ms->trigger_event);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

static int mirror_flush(struct dm_target *ti)
{
	struct mirror_set *ms = ti->private;
	unsigned long error_bits;

	unsigned int i;
<<<<<<< HEAD
	struct dm_io_region io[ms->nr_mirrors];
	struct mirror *m;
	struct dm_io_request io_req = {
		.bi_rw = WRITE_FLUSH,
=======
	struct dm_io_region io[MAX_NR_MIRRORS];
	struct mirror *m;
	struct dm_io_request io_req = {
		.bi_opf = REQ_OP_WRITE | REQ_PREFLUSH | REQ_SYNC,
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		.mem.type = DM_IO_KMEM,
		.mem.ptr.addr = NULL,
		.client = ms->io_client,
	};

	for (i = 0, m = ms->mirror; i < ms->nr_mirrors; i++, m++) {
		io[i].bdev = m->dev->bdev;
		io[i].sector = 0;
		io[i].count = 0;
	}

	error_bits = -1;
<<<<<<< HEAD
	dm_io(&io_req, ms->nr_mirrors, io, &error_bits);
=======
	dm_io(&io_req, ms->nr_mirrors, io, &error_bits, IOPRIO_DEFAULT);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	if (unlikely(error_bits != 0)) {
		for (i = 0; i < ms->nr_mirrors; i++)
			if (test_bit(i, &error_bits))
				fail_mirror(ms->mirror + i,
					    DM_RAID1_FLUSH_ERROR);
		return -EIO;
	}

	return 0;
}

<<<<<<< HEAD
/*-----------------------------------------------------------------
=======
/*
 *---------------------------------------------------------------
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 * Recovery.
 *
 * When a mirror is first activated we may find that some regions
 * are in the no-sync state.  We have to recover these by
 * recopying from the default mirror to all the others.
<<<<<<< HEAD
 *---------------------------------------------------------------*/
=======
 *---------------------------------------------------------------
 */
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
static void recovery_complete(int read_err, unsigned long write_err,
			      void *context)
{
	struct dm_region *reg = context;
	struct mirror_set *ms = dm_rh_region_context(reg);
	int m, bit = 0;

	if (read_err) {
		/* Read error means the failure of default mirror. */
		DMERR_LIMIT("Unable to read primary mirror during recovery");
		fail_mirror(get_default_mirror(ms), DM_RAID1_SYNC_ERROR);
	}

	if (write_err) {
		DMERR_LIMIT("Write error during recovery (error = 0x%lx)",
			    write_err);
		/*
		 * Bits correspond to devices (excluding default mirror).
		 * The default mirror cannot change during recovery.
		 */
		for (m = 0; m < ms->nr_mirrors; m++) {
			if (&ms->mirror[m] == get_default_mirror(ms))
				continue;
			if (test_bit(bit, &write_err))
				fail_mirror(ms->mirror + m,
					    DM_RAID1_SYNC_ERROR);
			bit++;
		}
	}

	dm_rh_recovery_end(reg, !(read_err || write_err));
}

<<<<<<< HEAD
static int recover(struct mirror_set *ms, struct dm_region *reg)
{
	int r;
	unsigned i;
=======
static void recover(struct mirror_set *ms, struct dm_region *reg)
{
	unsigned int i;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	struct dm_io_region from, to[DM_KCOPYD_MAX_REGIONS], *dest;
	struct mirror *m;
	unsigned long flags = 0;
	region_t key = dm_rh_get_region_key(reg);
	sector_t region_size = dm_rh_get_region_size(ms->rh);

	/* fill in the source */
	m = get_default_mirror(ms);
	from.bdev = m->dev->bdev;
	from.sector = m->offset + dm_rh_region_to_sector(ms->rh, key);
	if (key == (ms->nr_regions - 1)) {
		/*
		 * The final region may be smaller than
		 * region_size.
		 */
		from.count = ms->ti->len & (region_size - 1);
		if (!from.count)
			from.count = region_size;
	} else
		from.count = region_size;

	/* fill in the destinations */
	for (i = 0, dest = to; i < ms->nr_mirrors; i++) {
		if (&ms->mirror[i] == get_default_mirror(ms))
			continue;

		m = ms->mirror + i;
		dest->bdev = m->dev->bdev;
		dest->sector = m->offset + dm_rh_region_to_sector(ms->rh, key);
		dest->count = from.count;
		dest++;
	}

	/* hand to kcopyd */
	if (!errors_handled(ms))
<<<<<<< HEAD
		set_bit(DM_KCOPYD_IGNORE_ERROR, &flags);

	r = dm_kcopyd_copy(ms->kcopyd_client, &from, ms->nr_mirrors - 1, to,
			   flags, recovery_complete, reg);

	return r;
=======
		flags |= BIT(DM_KCOPYD_IGNORE_ERROR);

	dm_kcopyd_copy(ms->kcopyd_client, &from, ms->nr_mirrors - 1, to,
		       flags, recovery_complete, reg);
}

static void reset_ms_flags(struct mirror_set *ms)
{
	unsigned int m;

	ms->leg_failure = 0;
	for (m = 0; m < ms->nr_mirrors; m++) {
		atomic_set(&(ms->mirror[m].error_count), 0);
		ms->mirror[m].error_type = 0;
	}
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

static void do_recovery(struct mirror_set *ms)
{
	struct dm_region *reg;
	struct dm_dirty_log *log = dm_rh_dirty_log(ms->rh);
<<<<<<< HEAD
	int r;
=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	/*
	 * Start quiescing some regions.
	 */
	dm_rh_recovery_prepare(ms->rh);

	/*
	 * Copy any already quiesced regions.
	 */
<<<<<<< HEAD
	while ((reg = dm_rh_recovery_start(ms->rh))) {
		r = recover(ms, reg);
		if (r)
			dm_rh_recovery_end(reg, 0);
	}
=======
	while ((reg = dm_rh_recovery_start(ms->rh)))
		recover(ms, reg);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	/*
	 * Update the in sync flag.
	 */
	if (!ms->in_sync &&
	    (log->type->get_sync_count(log) == ms->nr_regions)) {
		/* the sync is complete */
		dm_table_event(ms->ti->table);
		ms->in_sync = 1;
<<<<<<< HEAD
	}
}

/*-----------------------------------------------------------------
 * Reads
 *---------------------------------------------------------------*/
=======
		reset_ms_flags(ms);
	}
}

/*
 *---------------------------------------------------------------
 * Reads
 *---------------------------------------------------------------
 */
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
static struct mirror *choose_mirror(struct mirror_set *ms, sector_t sector)
{
	struct mirror *m = get_default_mirror(ms);

	do {
		if (likely(!atomic_read(&m->error_count)))
			return m;

		if (m-- == ms->mirror)
			m += ms->nr_mirrors;
	} while (m != get_default_mirror(ms));

	return NULL;
}

static int default_ok(struct mirror *m)
{
	struct mirror *default_mirror = get_default_mirror(m->ms);

	return !atomic_read(&default_mirror->error_count);
}

static int mirror_available(struct mirror_set *ms, struct bio *bio)
{
	struct dm_dirty_log *log = dm_rh_dirty_log(ms->rh);
	region_t region = dm_rh_bio_to_region(ms->rh, bio);

	if (log->type->in_sync(log, region, 0))
<<<<<<< HEAD
		return choose_mirror(ms,  bio->bi_sector) ? 1 : 0;
=======
		return choose_mirror(ms,  bio->bi_iter.bi_sector) ? 1 : 0;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	return 0;
}

/*
 * remap a buffer to a particular mirror.
 */
static sector_t map_sector(struct mirror *m, struct bio *bio)
{
<<<<<<< HEAD
	if (unlikely(!bio->bi_size))
		return 0;
	return m->offset + dm_target_offset(m->ms->ti, bio->bi_sector);
=======
	if (unlikely(!bio->bi_iter.bi_size))
		return 0;
	return m->offset + dm_target_offset(m->ms->ti, bio->bi_iter.bi_sector);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

static void map_bio(struct mirror *m, struct bio *bio)
{
<<<<<<< HEAD
	bio->bi_bdev = m->dev->bdev;
	bio->bi_sector = map_sector(m, bio);
=======
	bio_set_dev(bio, m->dev->bdev);
	bio->bi_iter.bi_sector = map_sector(m, bio);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

static void map_region(struct dm_io_region *io, struct mirror *m,
		       struct bio *bio)
{
	io->bdev = m->dev->bdev;
	io->sector = map_sector(m, bio);
<<<<<<< HEAD
	io->count = bio->bi_size >> 9;
=======
	io->count = bio_sectors(bio);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

static void hold_bio(struct mirror_set *ms, struct bio *bio)
{
	/*
	 * Lock is required to avoid race condition during suspend
	 * process.
	 */
	spin_lock_irq(&ms->lock);

	if (atomic_read(&ms->suspend)) {
		spin_unlock_irq(&ms->lock);

		/*
		 * If device is suspended, complete the bio.
		 */
		if (dm_noflush_suspending(ms->ti))
<<<<<<< HEAD
			bio_endio(bio, DM_ENDIO_REQUEUE);
		else
			bio_endio(bio, -EIO);
=======
			bio->bi_status = BLK_STS_DM_REQUEUE;
		else
			bio->bi_status = BLK_STS_IOERR;

		bio_endio(bio);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		return;
	}

	/*
	 * Hold bio until the suspend is complete.
	 */
	bio_list_add(&ms->holds, bio);
	spin_unlock_irq(&ms->lock);
}

<<<<<<< HEAD
/*-----------------------------------------------------------------
 * Reads
 *---------------------------------------------------------------*/
=======
/*
 *---------------------------------------------------------------
 * Reads
 *---------------------------------------------------------------
 */
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
static void read_callback(unsigned long error, void *context)
{
	struct bio *bio = context;
	struct mirror *m;

	m = bio_get_m(bio);
	bio_set_m(bio, NULL);

	if (likely(!error)) {
<<<<<<< HEAD
		bio_endio(bio, 0);
=======
		bio_endio(bio);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		return;
	}

	fail_mirror(m, DM_RAID1_READ_ERROR);

	if (likely(default_ok(m)) || mirror_available(m->ms, bio)) {
<<<<<<< HEAD
		DMWARN_LIMIT("Read failure on mirror device %s.  "
			     "Trying alternative device.",
			     m->dev->name);
		queue_bio(m->ms, bio, bio_rw(bio));
=======
		DMWARN_LIMIT("Read failure on mirror device %s. Trying alternative device.",
			     m->dev->name);
		queue_bio(m->ms, bio, bio_data_dir(bio));
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		return;
	}

	DMERR_LIMIT("Read failure on mirror device %s.  Failing I/O.",
		    m->dev->name);
<<<<<<< HEAD
	bio_endio(bio, -EIO);
=======
	bio_io_error(bio);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

/* Asynchronous read. */
static void read_async_bio(struct mirror *m, struct bio *bio)
{
	struct dm_io_region io;
	struct dm_io_request io_req = {
<<<<<<< HEAD
		.bi_rw = READ,
		.mem.type = DM_IO_BVEC,
		.mem.ptr.bvec = bio->bi_io_vec + bio->bi_idx,
=======
		.bi_opf = REQ_OP_READ,
		.mem.type = DM_IO_BIO,
		.mem.ptr.bio = bio,
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		.notify.fn = read_callback,
		.notify.context = bio,
		.client = m->ms->io_client,
	};

	map_region(&io, m, bio);
	bio_set_m(bio, m);
<<<<<<< HEAD
	BUG_ON(dm_io(&io_req, 1, &io, NULL));
=======
	BUG_ON(dm_io(&io_req, 1, &io, NULL, IOPRIO_DEFAULT));
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

static inline int region_in_sync(struct mirror_set *ms, region_t region,
				 int may_block)
{
	int state = dm_rh_get_state(ms->rh, region, may_block);
	return state == DM_RH_CLEAN || state == DM_RH_DIRTY;
}

static void do_reads(struct mirror_set *ms, struct bio_list *reads)
{
	region_t region;
	struct bio *bio;
	struct mirror *m;

	while ((bio = bio_list_pop(reads))) {
		region = dm_rh_bio_to_region(ms->rh, bio);
		m = get_default_mirror(ms);

		/*
		 * We can only read balance if the region is in sync.
		 */
		if (likely(region_in_sync(ms, region, 1)))
<<<<<<< HEAD
			m = choose_mirror(ms, bio->bi_sector);
=======
			m = choose_mirror(ms, bio->bi_iter.bi_sector);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		else if (m && atomic_read(&m->error_count))
			m = NULL;

		if (likely(m))
			read_async_bio(m, bio);
		else
<<<<<<< HEAD
			bio_endio(bio, -EIO);
	}
}

/*-----------------------------------------------------------------
=======
			bio_io_error(bio);
	}
}

/*
 *---------------------------------------------------------------------
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 * Writes.
 *
 * We do different things with the write io depending on the
 * state of the region that it's in:
 *
<<<<<<< HEAD
 * SYNC: 	increment pending, use kcopyd to write to *all* mirrors
 * RECOVERING:	delay the io until recovery completes
 * NOSYNC:	increment pending, just write to the default mirror
 *---------------------------------------------------------------*/


static void write_callback(unsigned long error, void *context)
{
	unsigned i, ret = 0;
	struct bio *bio = (struct bio *) context;
=======
 * SYNC:	increment pending, use kcopyd to write to *all* mirrors
 * RECOVERING:	delay the io until recovery completes
 * NOSYNC:	increment pending, just write to the default mirror
 *---------------------------------------------------------------------
 */
static void write_callback(unsigned long error, void *context)
{
	unsigned int i;
	struct bio *bio = context;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	struct mirror_set *ms;
	int should_wake = 0;
	unsigned long flags;

	ms = bio_get_m(bio)->ms;
	bio_set_m(bio, NULL);

	/*
	 * NOTE: We don't decrement the pending count here,
	 * instead it is done by the targets endio function.
	 * This way we handle both writes to SYNC and NOSYNC
	 * regions with the same code.
	 */
	if (likely(!error)) {
<<<<<<< HEAD
		bio_endio(bio, ret);
=======
		bio_endio(bio);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		return;
	}

	/*
	 * If the bio is discard, return an error, but do not
	 * degrade the array.
	 */
<<<<<<< HEAD
	if (bio->bi_rw & REQ_DISCARD) {
		bio_endio(bio, -EOPNOTSUPP);
=======
	if (bio_op(bio) == REQ_OP_DISCARD) {
		bio->bi_status = BLK_STS_NOTSUPP;
		bio_endio(bio);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		return;
	}

	for (i = 0; i < ms->nr_mirrors; i++)
		if (test_bit(i, &error))
			fail_mirror(ms->mirror + i, DM_RAID1_WRITE_ERROR);

	/*
	 * Need to raise event.  Since raising
	 * events can block, we need to do it in
	 * the main thread.
	 */
	spin_lock_irqsave(&ms->lock, flags);
	if (!ms->failures.head)
		should_wake = 1;
	bio_list_add(&ms->failures, bio);
	spin_unlock_irqrestore(&ms->lock, flags);
	if (should_wake)
		wakeup_mirrord(ms);
}

static void do_write(struct mirror_set *ms, struct bio *bio)
{
	unsigned int i;
<<<<<<< HEAD
	struct dm_io_region io[ms->nr_mirrors], *dest = io;
	struct mirror *m;
	struct dm_io_request io_req = {
		.bi_rw = WRITE | (bio->bi_rw & WRITE_FLUSH_FUA),
		.mem.type = DM_IO_BVEC,
		.mem.ptr.bvec = bio->bi_io_vec + bio->bi_idx,
=======
	struct dm_io_region io[MAX_NR_MIRRORS], *dest = io;
	struct mirror *m;
	blk_opf_t op_flags = bio->bi_opf & (REQ_FUA | REQ_PREFLUSH);
	struct dm_io_request io_req = {
		.bi_opf = REQ_OP_WRITE | op_flags,
		.mem.type = DM_IO_BIO,
		.mem.ptr.bio = bio,
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		.notify.fn = write_callback,
		.notify.context = bio,
		.client = ms->io_client,
	};

<<<<<<< HEAD
	if (bio->bi_rw & REQ_DISCARD) {
		io_req.bi_rw |= REQ_DISCARD;
=======
	if (bio_op(bio) == REQ_OP_DISCARD) {
		io_req.bi_opf = REQ_OP_DISCARD | op_flags;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		io_req.mem.type = DM_IO_KMEM;
		io_req.mem.ptr.addr = NULL;
	}

	for (i = 0, m = ms->mirror; i < ms->nr_mirrors; i++, m++)
		map_region(dest++, m, bio);

	/*
	 * Use default mirror because we only need it to retrieve the reference
	 * to the mirror set in write_callback().
	 */
	bio_set_m(bio, get_default_mirror(ms));

<<<<<<< HEAD
	BUG_ON(dm_io(&io_req, ms->nr_mirrors, io, NULL));
=======
	BUG_ON(dm_io(&io_req, ms->nr_mirrors, io, NULL, IOPRIO_DEFAULT));
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

static void do_writes(struct mirror_set *ms, struct bio_list *writes)
{
	int state;
	struct bio *bio;
	struct bio_list sync, nosync, recover, *this_list = NULL;
	struct bio_list requeue;
	struct dm_dirty_log *log = dm_rh_dirty_log(ms->rh);
	region_t region;

	if (!writes->head)
		return;

	/*
	 * Classify each write.
	 */
	bio_list_init(&sync);
	bio_list_init(&nosync);
	bio_list_init(&recover);
	bio_list_init(&requeue);

	while ((bio = bio_list_pop(writes))) {
<<<<<<< HEAD
		if ((bio->bi_rw & REQ_FLUSH) ||
		    (bio->bi_rw & REQ_DISCARD)) {
=======
		if ((bio->bi_opf & REQ_PREFLUSH) ||
		    (bio_op(bio) == REQ_OP_DISCARD)) {
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
			bio_list_add(&sync, bio);
			continue;
		}

		region = dm_rh_bio_to_region(ms->rh, bio);

		if (log->type->is_remote_recovering &&
		    log->type->is_remote_recovering(log, region)) {
			bio_list_add(&requeue, bio);
			continue;
		}

		state = dm_rh_get_state(ms->rh, region, 1);
		switch (state) {
		case DM_RH_CLEAN:
		case DM_RH_DIRTY:
			this_list = &sync;
			break;

		case DM_RH_NOSYNC:
			this_list = &nosync;
			break;

		case DM_RH_RECOVERING:
			this_list = &recover;
			break;
		}

		bio_list_add(this_list, bio);
	}

	/*
	 * Add bios that are delayed due to remote recovery
	 * back on to the write queue
	 */
	if (unlikely(requeue.head)) {
		spin_lock_irq(&ms->lock);
		bio_list_merge(&ms->writes, &requeue);
		spin_unlock_irq(&ms->lock);
		delayed_wake(ms);
	}

	/*
	 * Increment the pending counts for any regions that will
	 * be written to (writes to recover regions are going to
	 * be delayed).
	 */
	dm_rh_inc_pending(ms->rh, &sync);
	dm_rh_inc_pending(ms->rh, &nosync);

	/*
	 * If the flush fails on a previous call and succeeds here,
	 * we must not reset the log_failure variable.  We need
	 * userspace interaction to do that.
	 */
	ms->log_failure = dm_rh_flush(ms->rh) ? 1 : ms->log_failure;

	/*
	 * Dispatch io.
	 */
	if (unlikely(ms->log_failure) && errors_handled(ms)) {
		spin_lock_irq(&ms->lock);
		bio_list_merge(&ms->failures, &sync);
		spin_unlock_irq(&ms->lock);
		wakeup_mirrord(ms);
	} else
		while ((bio = bio_list_pop(&sync)))
			do_write(ms, bio);

	while ((bio = bio_list_pop(&recover)))
		dm_rh_delay(ms->rh, bio);

	while ((bio = bio_list_pop(&nosync))) {
<<<<<<< HEAD
		if (unlikely(ms->leg_failure) && errors_handled(ms)) {
=======
		if (unlikely(ms->leg_failure) && errors_handled(ms) && !keep_log(ms)) {
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
			spin_lock_irq(&ms->lock);
			bio_list_add(&ms->failures, bio);
			spin_unlock_irq(&ms->lock);
			wakeup_mirrord(ms);
		} else {
			map_bio(get_default_mirror(ms), bio);
<<<<<<< HEAD
			generic_make_request(bio);
=======
			submit_bio_noacct(bio);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		}
	}
}

static void do_failures(struct mirror_set *ms, struct bio_list *failures)
{
	struct bio *bio;

	if (likely(!failures->head))
		return;

	/*
	 * If the log has failed, unattempted writes are being
	 * put on the holds list.  We can't issue those writes
	 * until a log has been marked, so we must store them.
	 *
	 * If a 'noflush' suspend is in progress, we can requeue
	 * the I/O's to the core.  This give userspace a chance
	 * to reconfigure the mirror, at which point the core
	 * will reissue the writes.  If the 'noflush' flag is
	 * not set, we have no choice but to return errors.
	 *
	 * Some writes on the failures list may have been
	 * submitted before the log failure and represent a
	 * failure to write to one of the devices.  It is ok
	 * for us to treat them the same and requeue them
	 * as well.
	 */
	while ((bio = bio_list_pop(failures))) {
		if (!ms->log_failure) {
			ms->in_sync = 0;
			dm_rh_mark_nosync(ms->rh, bio);
		}

		/*
		 * If all the legs are dead, fail the I/O.
<<<<<<< HEAD
		 * If we have been told to handle errors, hold the bio
		 * and wait for userspace to deal with the problem.
=======
		 * If the device has failed and keep_log is enabled,
		 * fail the I/O.
		 *
		 * If we have been told to handle errors, and keep_log
		 * isn't enabled, hold the bio and wait for userspace to
		 * deal with the problem.
		 *
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		 * Otherwise pretend that the I/O succeeded. (This would
		 * be wrong if the failed leg returned after reboot and
		 * got replicated back to the good legs.)
		 */
<<<<<<< HEAD
		if (!get_valid_mirror(ms))
			bio_endio(bio, -EIO);
		else if (errors_handled(ms))
			hold_bio(ms, bio);
		else
			bio_endio(bio, 0);
=======
		if (unlikely(!get_valid_mirror(ms) || (keep_log(ms) && ms->log_failure)))
			bio_io_error(bio);
		else if (errors_handled(ms) && !keep_log(ms))
			hold_bio(ms, bio);
		else
			bio_endio(bio);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	}
}

static void trigger_event(struct work_struct *work)
{
	struct mirror_set *ms =
		container_of(work, struct mirror_set, trigger_event);

	dm_table_event(ms->ti->table);
}

<<<<<<< HEAD
/*-----------------------------------------------------------------
 * kmirrord
 *---------------------------------------------------------------*/
=======
/*
 *---------------------------------------------------------------
 * kmirrord
 *---------------------------------------------------------------
 */
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
static void do_mirror(struct work_struct *work)
{
	struct mirror_set *ms = container_of(work, struct mirror_set,
					     kmirrord_work);
	struct bio_list reads, writes, failures;
	unsigned long flags;

	spin_lock_irqsave(&ms->lock, flags);
	reads = ms->reads;
	writes = ms->writes;
	failures = ms->failures;
	bio_list_init(&ms->reads);
	bio_list_init(&ms->writes);
	bio_list_init(&ms->failures);
	spin_unlock_irqrestore(&ms->lock, flags);

	dm_rh_update_states(ms->rh, errors_handled(ms));
	do_recovery(ms);
	do_reads(ms, &reads);
	do_writes(ms, &writes);
	do_failures(ms, &failures);
}

<<<<<<< HEAD
/*-----------------------------------------------------------------
 * Target functions
 *---------------------------------------------------------------*/
=======
/*
 *---------------------------------------------------------------
 * Target functions
 *---------------------------------------------------------------
 */
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
static struct mirror_set *alloc_context(unsigned int nr_mirrors,
					uint32_t region_size,
					struct dm_target *ti,
					struct dm_dirty_log *dl)
{
<<<<<<< HEAD
	size_t len;
	struct mirror_set *ms = NULL;

	len = sizeof(*ms) + (sizeof(ms->mirror[0]) * nr_mirrors);

	ms = kzalloc(len, GFP_KERNEL);
=======
	struct mirror_set *ms =
		kzalloc(struct_size(ms, mirror, nr_mirrors), GFP_KERNEL);

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	if (!ms) {
		ti->error = "Cannot allocate mirror context";
		return NULL;
	}

	spin_lock_init(&ms->lock);
	bio_list_init(&ms->reads);
	bio_list_init(&ms->writes);
	bio_list_init(&ms->failures);
	bio_list_init(&ms->holds);

	ms->ti = ti;
	ms->nr_mirrors = nr_mirrors;
	ms->nr_regions = dm_sector_div_up(ti->len, region_size);
	ms->in_sync = 0;
	ms->log_failure = 0;
	ms->leg_failure = 0;
	atomic_set(&ms->suspend, 0);
	atomic_set(&ms->default_mirror, DEFAULT_MIRROR);

<<<<<<< HEAD
	ms->read_record_pool = mempool_create_slab_pool(MIN_READ_RECORDS,
						_dm_raid1_read_record_cache);

	if (!ms->read_record_pool) {
		ti->error = "Error creating mirror read_record_pool";
		kfree(ms);
		return NULL;
	}

	ms->io_client = dm_io_client_create();
	if (IS_ERR(ms->io_client)) {
		ti->error = "Error creating dm_io client";
		mempool_destroy(ms->read_record_pool);
		kfree(ms);
 		return NULL;
=======
	ms->io_client = dm_io_client_create();
	if (IS_ERR(ms->io_client)) {
		ti->error = "Error creating dm_io client";
		kfree(ms);
		return NULL;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	}

	ms->rh = dm_region_hash_create(ms, dispatch_bios, wakeup_mirrord,
				       wakeup_all_recovery_waiters,
				       ms->ti->begin, MAX_RECOVERY,
				       dl, region_size, ms->nr_regions);
	if (IS_ERR(ms->rh)) {
		ti->error = "Error creating dirty region hash";
		dm_io_client_destroy(ms->io_client);
<<<<<<< HEAD
		mempool_destroy(ms->read_record_pool);
=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		kfree(ms);
		return NULL;
	}

	return ms;
}

static void free_context(struct mirror_set *ms, struct dm_target *ti,
			 unsigned int m)
{
	while (m--)
		dm_put_device(ti, ms->mirror[m].dev);

	dm_io_client_destroy(ms->io_client);
	dm_region_hash_destroy(ms->rh);
<<<<<<< HEAD
	mempool_destroy(ms->read_record_pool);
=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	kfree(ms);
}

static int get_mirror(struct mirror_set *ms, struct dm_target *ti,
		      unsigned int mirror, char **argv)
{
	unsigned long long offset;
	char dummy;
<<<<<<< HEAD

	if (sscanf(argv[1], "%llu%c", &offset, &dummy) != 1) {
=======
	int ret;

	if (sscanf(argv[1], "%llu%c", &offset, &dummy) != 1 ||
	    offset != (sector_t)offset) {
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		ti->error = "Invalid offset";
		return -EINVAL;
	}

<<<<<<< HEAD
	if (dm_get_device(ti, argv[0], dm_table_get_mode(ti->table),
			  &ms->mirror[mirror].dev)) {
		ti->error = "Device lookup failure";
		return -ENXIO;
=======
	ret = dm_get_device(ti, argv[0], dm_table_get_mode(ti->table),
			    &ms->mirror[mirror].dev);
	if (ret) {
		ti->error = "Device lookup failure";
		return ret;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	}

	ms->mirror[mirror].ms = ms;
	atomic_set(&(ms->mirror[mirror].error_count), 0);
	ms->mirror[mirror].error_type = 0;
	ms->mirror[mirror].offset = offset;

	return 0;
}

/*
 * Create dirty log: log_type #log_params <log_params>
 */
static struct dm_dirty_log *create_dirty_log(struct dm_target *ti,
<<<<<<< HEAD
					     unsigned argc, char **argv,
					     unsigned *args_used)
{
	unsigned param_count;
=======
					     unsigned int argc, char **argv,
					     unsigned int *args_used)
{
	unsigned int param_count;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	struct dm_dirty_log *dl;
	char dummy;

	if (argc < 2) {
		ti->error = "Insufficient mirror log arguments";
		return NULL;
	}

	if (sscanf(argv[1], "%u%c", &param_count, &dummy) != 1) {
		ti->error = "Invalid mirror log argument count";
		return NULL;
	}

	*args_used = 2 + param_count;

	if (argc < *args_used) {
		ti->error = "Insufficient mirror log arguments";
		return NULL;
	}

	dl = dm_dirty_log_create(argv[0], ti, mirror_flush, param_count,
				 argv + 2);
	if (!dl) {
		ti->error = "Error creating mirror dirty log";
		return NULL;
	}

	return dl;
}

<<<<<<< HEAD
static int parse_features(struct mirror_set *ms, unsigned argc, char **argv,
			  unsigned *args_used)
{
	unsigned num_features;
	struct dm_target *ti = ms->ti;
	char dummy;
=======
static int parse_features(struct mirror_set *ms, unsigned int argc, char **argv,
			  unsigned int *args_used)
{
	unsigned int num_features;
	struct dm_target *ti = ms->ti;
	char dummy;
	int i;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	*args_used = 0;

	if (!argc)
		return 0;

	if (sscanf(argv[0], "%u%c", &num_features, &dummy) != 1) {
		ti->error = "Invalid number of features";
		return -EINVAL;
	}

	argc--;
	argv++;
	(*args_used)++;

	if (num_features > argc) {
		ti->error = "Not enough arguments to support feature count";
		return -EINVAL;
	}

<<<<<<< HEAD
	if (!strcmp("handle_errors", argv[0]))
		ms->features |= DM_RAID1_HANDLE_ERRORS;
	else {
		ti->error = "Unrecognised feature requested";
		return -EINVAL;
	}

	(*args_used)++;

=======
	for (i = 0; i < num_features; i++) {
		if (!strcmp("handle_errors", argv[0]))
			ms->features |= DM_RAID1_HANDLE_ERRORS;
		else if (!strcmp("keep_log", argv[0]))
			ms->features |= DM_RAID1_KEEP_LOG;
		else {
			ti->error = "Unrecognised feature requested";
			return -EINVAL;
		}

		argc--;
		argv++;
		(*args_used)++;
	}
	if (!errors_handled(ms) && keep_log(ms)) {
		ti->error = "keep_log feature requires the handle_errors feature";
		return -EINVAL;
	}

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	return 0;
}

/*
 * Construct a mirror mapping:
 *
 * log_type #log_params <log_params>
 * #mirrors [mirror_path offset]{2,}
 * [#features <features>]
 *
 * log_type is "core" or "disk"
 * #log_params is between 1 and 3
 *
<<<<<<< HEAD
 * If present, features must be "handle_errors".
=======
 * If present, supported features are "handle_errors" and "keep_log".
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 */
static int mirror_ctr(struct dm_target *ti, unsigned int argc, char **argv)
{
	int r;
	unsigned int nr_mirrors, m, args_used;
	struct mirror_set *ms;
	struct dm_dirty_log *dl;
	char dummy;

	dl = create_dirty_log(ti, argc, argv, &args_used);
	if (!dl)
		return -EINVAL;

	argv += args_used;
	argc -= args_used;

	if (!argc || sscanf(argv[0], "%u%c", &nr_mirrors, &dummy) != 1 ||
<<<<<<< HEAD
	    nr_mirrors < 2 || nr_mirrors > DM_KCOPYD_MAX_REGIONS + 1) {
=======
	    nr_mirrors < 2 || nr_mirrors > MAX_NR_MIRRORS) {
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		ti->error = "Invalid number of mirrors";
		dm_dirty_log_destroy(dl);
		return -EINVAL;
	}

	argv++, argc--;

	if (argc < nr_mirrors * 2) {
		ti->error = "Too few mirror arguments";
		dm_dirty_log_destroy(dl);
		return -EINVAL;
	}

	ms = alloc_context(nr_mirrors, dl->type->get_region_size(dl), ti, dl);
	if (!ms) {
		dm_dirty_log_destroy(dl);
		return -ENOMEM;
	}

	/* Get the mirror parameter sets */
	for (m = 0; m < nr_mirrors; m++) {
		r = get_mirror(ms, ti, m, argv);
		if (r) {
			free_context(ms, ti, m);
			return r;
		}
		argv += 2;
		argc -= 2;
	}

	ti->private = ms;
<<<<<<< HEAD
	ti->split_io = dm_rh_get_region_size(ms->rh);
	ti->num_flush_requests = 1;
	ti->num_discard_requests = 1;
	ti->discard_zeroes_data_unsupported = 1;

	ms->kmirrord_wq = alloc_workqueue("kmirrord",
					  WQ_NON_REENTRANT | WQ_MEM_RECLAIM, 0);
=======

	r = dm_set_target_max_io_len(ti, dm_rh_get_region_size(ms->rh));
	if (r)
		goto err_free_context;

	ti->num_flush_bios = 1;
	ti->num_discard_bios = 1;
	ti->per_io_data_size = sizeof(struct dm_raid1_bio_record);

	ms->kmirrord_wq = alloc_workqueue("kmirrord", WQ_MEM_RECLAIM, 0);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	if (!ms->kmirrord_wq) {
		DMERR("couldn't start kmirrord");
		r = -ENOMEM;
		goto err_free_context;
	}
	INIT_WORK(&ms->kmirrord_work, do_mirror);
<<<<<<< HEAD
	init_timer(&ms->timer);
=======
	timer_setup(&ms->timer, delayed_wake_fn, 0);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	ms->timer_pending = 0;
	INIT_WORK(&ms->trigger_event, trigger_event);

	r = parse_features(ms, argc, argv, &args_used);
	if (r)
		goto err_destroy_wq;

	argv += args_used;
	argc -= args_used;

	/*
	 * Any read-balancing addition depends on the
	 * DM_RAID1_HANDLE_ERRORS flag being present.
	 * This is because the decision to balance depends
	 * on the sync state of a region.  If the above
	 * flag is not present, we ignore errors; and
	 * the sync state may be inaccurate.
	 */

	if (argc) {
		ti->error = "Too many mirror arguments";
		r = -EINVAL;
		goto err_destroy_wq;
	}

<<<<<<< HEAD
	ms->kcopyd_client = dm_kcopyd_client_create();
=======
	ms->kcopyd_client = dm_kcopyd_client_create(&dm_kcopyd_throttle);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	if (IS_ERR(ms->kcopyd_client)) {
		r = PTR_ERR(ms->kcopyd_client);
		goto err_destroy_wq;
	}

	wakeup_mirrord(ms);
	return 0;

err_destroy_wq:
	destroy_workqueue(ms->kmirrord_wq);
err_free_context:
	free_context(ms, ti, ms->nr_mirrors);
	return r;
}

static void mirror_dtr(struct dm_target *ti)
{
<<<<<<< HEAD
	struct mirror_set *ms = (struct mirror_set *) ti->private;

	del_timer_sync(&ms->timer);
	flush_workqueue(ms->kmirrord_wq);
	flush_work_sync(&ms->trigger_event);
=======
	struct mirror_set *ms = ti->private;

	del_timer_sync(&ms->timer);
	flush_workqueue(ms->kmirrord_wq);
	flush_work(&ms->trigger_event);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	dm_kcopyd_client_destroy(ms->kcopyd_client);
	destroy_workqueue(ms->kmirrord_wq);
	free_context(ms, ti, ms->nr_mirrors);
}

/*
 * Mirror mapping function
 */
<<<<<<< HEAD
static int mirror_map(struct dm_target *ti, struct bio *bio,
		      union map_info *map_context)
{
	int r, rw = bio_rw(bio);
	struct mirror *m;
	struct mirror_set *ms = ti->private;
	struct dm_raid1_read_record *read_record = NULL;
	struct dm_dirty_log *log = dm_rh_dirty_log(ms->rh);

	if (rw == WRITE) {
		/* Save region for mirror_end_io() handler */
		map_context->ll = dm_rh_bio_to_region(ms->rh, bio);
=======
static int mirror_map(struct dm_target *ti, struct bio *bio)
{
	int r, rw = bio_data_dir(bio);
	struct mirror *m;
	struct mirror_set *ms = ti->private;
	struct dm_dirty_log *log = dm_rh_dirty_log(ms->rh);
	struct dm_raid1_bio_record *bio_record =
	  dm_per_bio_data(bio, sizeof(struct dm_raid1_bio_record));

	bio_record->details.bi_bdev = NULL;

	if (rw == WRITE) {
		/* Save region for mirror_end_io() handler */
		bio_record->write_region = dm_rh_bio_to_region(ms->rh, bio);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		queue_bio(ms, bio, rw);
		return DM_MAPIO_SUBMITTED;
	}

	r = log->type->in_sync(log, dm_rh_bio_to_region(ms->rh, bio), 0);
	if (r < 0 && r != -EWOULDBLOCK)
<<<<<<< HEAD
		return r;
=======
		return DM_MAPIO_KILL;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	/*
	 * If region is not in-sync queue the bio.
	 */
	if (!r || (r == -EWOULDBLOCK)) {
<<<<<<< HEAD
		if (rw == READA)
			return -EWOULDBLOCK;
=======
		if (bio->bi_opf & REQ_RAHEAD)
			return DM_MAPIO_KILL;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

		queue_bio(ms, bio, rw);
		return DM_MAPIO_SUBMITTED;
	}

	/*
	 * The region is in-sync and we can perform reads directly.
	 * Store enough information so we can retry if it fails.
	 */
<<<<<<< HEAD
	m = choose_mirror(ms, bio->bi_sector);
	if (unlikely(!m))
		return -EIO;

	read_record = mempool_alloc(ms->read_record_pool, GFP_NOIO);
	if (likely(read_record)) {
		dm_bio_record(&read_record->details, bio);
		map_context->ptr = read_record;
		read_record->m = m;
	}
=======
	m = choose_mirror(ms, bio->bi_iter.bi_sector);
	if (unlikely(!m))
		return DM_MAPIO_KILL;

	dm_bio_record(&bio_record->details, bio);
	bio_record->m = m;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	map_bio(m, bio);

	return DM_MAPIO_REMAPPED;
}

static int mirror_end_io(struct dm_target *ti, struct bio *bio,
<<<<<<< HEAD
			 int error, union map_info *map_context)
{
	int rw = bio_rw(bio);
	struct mirror_set *ms = (struct mirror_set *) ti->private;
	struct mirror *m = NULL;
	struct dm_bio_details *bd = NULL;
	struct dm_raid1_read_record *read_record = map_context->ptr;
=======
		blk_status_t *error)
{
	int rw = bio_data_dir(bio);
	struct mirror_set *ms = ti->private;
	struct mirror *m = NULL;
	struct dm_bio_details *bd = NULL;
	struct dm_raid1_bio_record *bio_record =
	  dm_per_bio_data(bio, sizeof(struct dm_raid1_bio_record));
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	/*
	 * We need to dec pending if this was a write.
	 */
	if (rw == WRITE) {
<<<<<<< HEAD
		if (!(bio->bi_rw & (REQ_FLUSH | REQ_DISCARD)))
			dm_rh_dec(ms->rh, map_context->ll);
		return error;
	}

	if (error == -EOPNOTSUPP)
		goto out;

	if ((error == -EWOULDBLOCK) && (bio->bi_rw & REQ_RAHEAD))
		goto out;

	if (unlikely(error)) {
		if (!read_record) {
=======
		if (!(bio->bi_opf & REQ_PREFLUSH) &&
		    bio_op(bio) != REQ_OP_DISCARD)
			dm_rh_dec(ms->rh, bio_record->write_region);
		return DM_ENDIO_DONE;
	}

	if (*error == BLK_STS_NOTSUPP)
		goto out;

	if (bio->bi_opf & REQ_RAHEAD)
		goto out;

	if (unlikely(*error)) {
		if (!bio_record->details.bi_bdev) {
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
			/*
			 * There wasn't enough memory to record necessary
			 * information for a retry or there was no other
			 * mirror in-sync.
			 */
			DMERR_LIMIT("Mirror read failed.");
<<<<<<< HEAD
			return -EIO;
		}

		m = read_record->m;
=======
			return DM_ENDIO_DONE;
		}

		m = bio_record->m;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

		DMERR("Mirror read failed from %s. Trying alternative device.",
		      m->dev->name);

		fail_mirror(m, DM_RAID1_READ_ERROR);

		/*
		 * A failed read is requeued for another attempt using an intact
		 * mirror.
		 */
		if (default_ok(m) || mirror_available(ms, bio)) {
<<<<<<< HEAD
			bd = &read_record->details;

			dm_bio_restore(bd, bio);
			mempool_free(read_record, ms->read_record_pool);
			map_context->ptr = NULL;
			queue_bio(ms, bio, rw);
			return 1;
=======
			bd = &bio_record->details;

			dm_bio_restore(bd, bio);
			bio_record->details.bi_bdev = NULL;
			bio->bi_status = 0;

			queue_bio(ms, bio, rw);
			return DM_ENDIO_INCOMPLETE;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		}
		DMERR("All replicated volumes dead, failing I/O");
	}

out:
<<<<<<< HEAD
	if (read_record) {
		mempool_free(read_record, ms->read_record_pool);
		map_context->ptr = NULL;
	}

	return error;
=======
	bio_record->details.bi_bdev = NULL;

	return DM_ENDIO_DONE;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

static void mirror_presuspend(struct dm_target *ti)
{
<<<<<<< HEAD
	struct mirror_set *ms = (struct mirror_set *) ti->private;
=======
	struct mirror_set *ms = ti->private;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	struct dm_dirty_log *log = dm_rh_dirty_log(ms->rh);

	struct bio_list holds;
	struct bio *bio;

	atomic_set(&ms->suspend, 1);

	/*
	 * Process bios in the hold list to start recovery waiting
	 * for bios in the hold list. After the process, no bio has
	 * a chance to be added in the hold list because ms->suspend
	 * is set.
	 */
	spin_lock_irq(&ms->lock);
	holds = ms->holds;
	bio_list_init(&ms->holds);
	spin_unlock_irq(&ms->lock);

	while ((bio = bio_list_pop(&holds)))
		hold_bio(ms, bio);

	/*
	 * We must finish up all the work that we've
	 * generated (i.e. recovery work).
	 */
	dm_rh_stop_recovery(ms->rh);

	wait_event(_kmirrord_recovery_stopped,
		   !dm_rh_recovery_in_flight(ms->rh));

	if (log->type->presuspend && log->type->presuspend(log))
		/* FIXME: need better error handling */
		DMWARN("log presuspend failed");

	/*
	 * Now that recovery is complete/stopped and the
	 * delayed bios are queued, we need to wait for
	 * the worker thread to complete.  This way,
	 * we know that all of our I/O has been pushed.
	 */
	flush_workqueue(ms->kmirrord_wq);
}

static void mirror_postsuspend(struct dm_target *ti)
{
	struct mirror_set *ms = ti->private;
	struct dm_dirty_log *log = dm_rh_dirty_log(ms->rh);

	if (log->type->postsuspend && log->type->postsuspend(log))
		/* FIXME: need better error handling */
		DMWARN("log postsuspend failed");
}

static void mirror_resume(struct dm_target *ti)
{
	struct mirror_set *ms = ti->private;
	struct dm_dirty_log *log = dm_rh_dirty_log(ms->rh);

	atomic_set(&ms->suspend, 0);
	if (log->type->resume && log->type->resume(log))
		/* FIXME: need better error handling */
		DMWARN("log resume failed");
	dm_rh_start_recovery(ms->rh);
}

/*
 * device_status_char
 * @m: mirror device/leg we want the status of
 *
 * We return one character representing the most severe error
 * we have encountered.
 *    A => Alive - No failures
 *    D => Dead - A write failure occurred leaving mirror out-of-sync
 *    S => Sync - A sychronization failure occurred, mirror out-of-sync
 *    R => Read - A read failure occurred, mirror data unaffected
 *
 * Returns: <char>
 */
static char device_status_char(struct mirror *m)
{
	if (!atomic_read(&(m->error_count)))
		return 'A';

	return (test_bit(DM_RAID1_FLUSH_ERROR, &(m->error_type))) ? 'F' :
		(test_bit(DM_RAID1_WRITE_ERROR, &(m->error_type))) ? 'D' :
		(test_bit(DM_RAID1_SYNC_ERROR, &(m->error_type))) ? 'S' :
		(test_bit(DM_RAID1_READ_ERROR, &(m->error_type))) ? 'R' : 'U';
}


static void mirror_status(struct dm_target *ti, status_type_t type,
<<<<<<< HEAD
			  char *result, unsigned int maxlen)
{
	unsigned int m, sz = 0;
	struct mirror_set *ms = (struct mirror_set *) ti->private;
	struct dm_dirty_log *log = dm_rh_dirty_log(ms->rh);
	char buffer[ms->nr_mirrors + 1];
=======
			  unsigned int status_flags, char *result, unsigned int maxlen)
{
	unsigned int m, sz = 0;
	int num_feature_args = 0;
	struct mirror_set *ms = ti->private;
	struct dm_dirty_log *log = dm_rh_dirty_log(ms->rh);
	char buffer[MAX_NR_MIRRORS + 1];
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	switch (type) {
	case STATUSTYPE_INFO:
		DMEMIT("%d ", ms->nr_mirrors);
		for (m = 0; m < ms->nr_mirrors; m++) {
			DMEMIT("%s ", ms->mirror[m].dev->name);
			buffer[m] = device_status_char(&(ms->mirror[m]));
		}
		buffer[m] = '\0';

		DMEMIT("%llu/%llu 1 %s ",
		      (unsigned long long)log->type->get_sync_count(log),
		      (unsigned long long)ms->nr_regions, buffer);

		sz += log->type->status(log, type, result+sz, maxlen-sz);

		break;

	case STATUSTYPE_TABLE:
		sz = log->type->status(log, type, result, maxlen);

		DMEMIT("%d", ms->nr_mirrors);
		for (m = 0; m < ms->nr_mirrors; m++)
			DMEMIT(" %s %llu", ms->mirror[m].dev->name,
			       (unsigned long long)ms->mirror[m].offset);

<<<<<<< HEAD
		if (ms->features & DM_RAID1_HANDLE_ERRORS)
			DMEMIT(" 1 handle_errors");
=======
		num_feature_args += !!errors_handled(ms);
		num_feature_args += !!keep_log(ms);
		if (num_feature_args) {
			DMEMIT(" %d", num_feature_args);
			if (errors_handled(ms))
				DMEMIT(" handle_errors");
			if (keep_log(ms))
				DMEMIT(" keep_log");
		}

		break;

	case STATUSTYPE_IMA:
		DMEMIT_TARGET_NAME_VERSION(ti->type);
		DMEMIT(",nr_mirrors=%d", ms->nr_mirrors);
		for (m = 0; m < ms->nr_mirrors; m++) {
			DMEMIT(",mirror_device_%d=%s", m, ms->mirror[m].dev->name);
			DMEMIT(",mirror_device_%d_status=%c",
			       m, device_status_char(&(ms->mirror[m])));
		}

		DMEMIT(",handle_errors=%c", errors_handled(ms) ? 'y' : 'n');
		DMEMIT(",keep_log=%c", keep_log(ms) ? 'y' : 'n');

		DMEMIT(",log_type_status=");
		sz += log->type->status(log, type, result+sz, maxlen-sz);
		DMEMIT(";");
		break;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	}
}

static int mirror_iterate_devices(struct dm_target *ti,
				  iterate_devices_callout_fn fn, void *data)
{
	struct mirror_set *ms = ti->private;
	int ret = 0;
<<<<<<< HEAD
	unsigned i;
=======
	unsigned int i;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	for (i = 0; !ret && i < ms->nr_mirrors; i++)
		ret = fn(ti, ms->mirror[i].dev,
			 ms->mirror[i].offset, ti->len, data);

	return ret;
}

static struct target_type mirror_target = {
	.name	 = "mirror",
<<<<<<< HEAD
	.version = {1, 12, 1},
=======
	.version = {1, 14, 0},
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	.module	 = THIS_MODULE,
	.ctr	 = mirror_ctr,
	.dtr	 = mirror_dtr,
	.map	 = mirror_map,
	.end_io	 = mirror_end_io,
	.presuspend = mirror_presuspend,
	.postsuspend = mirror_postsuspend,
	.resume	 = mirror_resume,
	.status	 = mirror_status,
	.iterate_devices = mirror_iterate_devices,
};

static int __init dm_mirror_init(void)
{
	int r;

<<<<<<< HEAD
	_dm_raid1_read_record_cache = KMEM_CACHE(dm_raid1_read_record, 0);
	if (!_dm_raid1_read_record_cache) {
		DMERR("Can't allocate dm_raid1_read_record cache");
		r = -ENOMEM;
		goto bad_cache;
=======
	dm_raid1_wq = alloc_workqueue("dm_raid1_wq", 0, 0);
	if (!dm_raid1_wq) {
		DMERR("Failed to alloc workqueue");
		return -ENOMEM;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	}

	r = dm_register_target(&mirror_target);
	if (r < 0) {
<<<<<<< HEAD
		DMERR("Failed to register mirror target");
		goto bad_target;
	}

	return 0;

bad_target:
	kmem_cache_destroy(_dm_raid1_read_record_cache);
bad_cache:
	return r;
=======
		destroy_workqueue(dm_raid1_wq);
		return r;
	}

	return 0;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

static void __exit dm_mirror_exit(void)
{
<<<<<<< HEAD
	dm_unregister_target(&mirror_target);
	kmem_cache_destroy(_dm_raid1_read_record_cache);
=======
	destroy_workqueue(dm_raid1_wq);
	dm_unregister_target(&mirror_target);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

/* Module hooks */
module_init(dm_mirror_init);
module_exit(dm_mirror_exit);

MODULE_DESCRIPTION(DM_NAME " mirror target");
MODULE_AUTHOR("Joe Thornber");
MODULE_LICENSE("GPL");
