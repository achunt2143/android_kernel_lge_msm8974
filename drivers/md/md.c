<<<<<<< HEAD
/*
   md.c : Multiple Devices driver for Linux
	  Copyright (C) 1998, 1999, 2000 Ingo Molnar
=======
// SPDX-License-Identifier: GPL-2.0-or-later
/*
   md.c : Multiple Devices driver for Linux
     Copyright (C) 1998, 1999, 2000 Ingo Molnar
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

     completely rewritten, based on the MD driver code from Marc Zyngier

   Changes:

   - RAID-1/RAID-5 extensions by Miguel de Icaza, Gadi Oxman, Ingo Molnar
   - RAID-6 extensions by H. Peter Anvin <hpa@zytor.com>
   - boot support for linear and striped mode by Harald Hoyer <HarryH@Royal.Net>
   - kerneld support by Boris Tobotras <boris@xtalk.msk.su>
   - kmod support by: Cyrus Durgin
   - RAID0 bugfixes: Mark Anthony Lisher <markal@iname.com>
   - Devfs support by Richard Gooch <rgooch@atnf.csiro.au>

   - lots of fixes and improvements to the RAID1/RAID5 and generic
     RAID code (such as request based resynchronization):

     Neil Brown <neilb@cse.unsw.edu.au>.

   - persistent bitmap code
     Copyright (C) 2003-2004, Paul Clements, SteelEye Technology, Inc.

<<<<<<< HEAD
   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 2, or (at your option)
   any later version.

   You should have received a copy of the GNU General Public License
   (for example /usr/src/linux/COPYING); if not, write to the Free
   Software Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
*/

#include <linux/kthread.h>
#include <linux/blkdev.h>
=======

   Errors, Warnings, etc.
   Please use:
     pr_crit() for error conditions that risk data loss
     pr_err() for error conditions that are unexpected, like an IO error
         or internal inconsistency
     pr_warn() for error conditions that could have been predicated, like
         adding a device to an array when it has incompatible metadata
     pr_info() for every interesting, very rare events, like an array starting
         or stopping, or resync starting or stopping
     pr_debug() for everything else.

*/

#include <linux/sched/mm.h>
#include <linux/sched/signal.h>
#include <linux/kthread.h>
#include <linux/blkdev.h>
#include <linux/blk-integrity.h>
#include <linux/badblocks.h>
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#include <linux/sysctl.h>
#include <linux/seq_file.h>
#include <linux/fs.h>
#include <linux/poll.h>
#include <linux/ctype.h>
#include <linux/string.h>
#include <linux/hdreg.h>
#include <linux/proc_fs.h>
#include <linux/random.h>
<<<<<<< HEAD
=======
#include <linux/major.h>
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#include <linux/module.h>
#include <linux/reboot.h>
#include <linux/file.h>
#include <linux/compat.h>
#include <linux/delay.h>
#include <linux/raid/md_p.h>
#include <linux/raid/md_u.h>
<<<<<<< HEAD
#include <linux/slab.h>
#include "md.h"
#include "bitmap.h"

#ifndef MODULE
static void autostart_arrays(int part);
#endif

/* pers_list is a list of registered personalities protected
 * by pers_lock.
 * pers_lock does extra service to protect accesses to
 * mddev->thread when the mutex cannot be held.
 */
static LIST_HEAD(pers_list);
static DEFINE_SPINLOCK(pers_lock);

static void md_print_devices(void);

static DECLARE_WAIT_QUEUE_HEAD(resync_wait);
static struct workqueue_struct *md_wq;
static struct workqueue_struct *md_misc_wq;

#define MD_BUG(x...) { printk("md: bug in file %s, line %d\n", __FILE__, __LINE__); md_print_devices(); }
=======
#include <linux/raid/detect.h>
#include <linux/slab.h>
#include <linux/percpu-refcount.h>
#include <linux/part_stat.h>

#include "md.h"
#include "md-bitmap.h"
#include "md-cluster.h"

/* pers_list is a list of registered personalities protected by pers_lock. */
static LIST_HEAD(pers_list);
static DEFINE_SPINLOCK(pers_lock);

static const struct kobj_type md_ktype;

struct md_cluster_operations *md_cluster_ops;
EXPORT_SYMBOL(md_cluster_ops);
static struct module *md_cluster_mod;

static DECLARE_WAIT_QUEUE_HEAD(resync_wait);
static struct workqueue_struct *md_wq;

/*
 * This workqueue is used for sync_work to register new sync_thread, and for
 * del_work to remove rdev, and for event_work that is only set by dm-raid.
 *
 * Noted that sync_work will grab reconfig_mutex, hence never flush this
 * workqueue whith reconfig_mutex grabbed.
 */
static struct workqueue_struct *md_misc_wq;
struct workqueue_struct *md_bitmap_wq;

static int remove_and_add_spares(struct mddev *mddev,
				 struct md_rdev *this);
static void mddev_detach(struct mddev *mddev);
static void export_rdev(struct md_rdev *rdev, struct mddev *mddev);
static void md_wakeup_thread_directly(struct md_thread __rcu *thread);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

/*
 * Default number of read corrections we'll attempt on an rdev
 * before ejecting it from the array. We divide the read error
 * count by 2 for every hour elapsed between read errors.
 */
#define MD_DEFAULT_MAX_CORRECTED_READ_ERRORS 20
<<<<<<< HEAD
=======
/* Default safemode delay: 200 msec */
#define DEFAULT_SAFEMODE_DELAY ((200 * HZ)/1000 +1)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
/*
 * Current RAID-1,4,5 parallel reconstruction 'guaranteed speed limit'
 * is 1000 KB/sec, so the extra system load does not show up that much.
 * Increase it if you want to have more _guaranteed_ speed. Note that
 * the RAID driver will use the maximum available bandwidth if the IO
 * subsystem is idle. There is also an 'absolute maximum' reconstruction
 * speed limit - in case reconstruction slows down your system despite
 * idle IO detection.
 *
 * you can change it via /proc/sys/dev/raid/speed_limit_min and _max.
 * or /sys/block/mdX/md/sync_speed_{min,max}
 */

static int sysctl_speed_limit_min = 1000;
static int sysctl_speed_limit_max = 200000;
static inline int speed_min(struct mddev *mddev)
{
	return mddev->sync_speed_min ?
		mddev->sync_speed_min : sysctl_speed_limit_min;
}

static inline int speed_max(struct mddev *mddev)
{
	return mddev->sync_speed_max ?
		mddev->sync_speed_max : sysctl_speed_limit_max;
}

<<<<<<< HEAD
static struct ctl_table_header *raid_table_header;

static ctl_table raid_table[] = {
=======
static void rdev_uninit_serial(struct md_rdev *rdev)
{
	if (!test_and_clear_bit(CollisionCheck, &rdev->flags))
		return;

	kvfree(rdev->serial);
	rdev->serial = NULL;
}

static void rdevs_uninit_serial(struct mddev *mddev)
{
	struct md_rdev *rdev;

	rdev_for_each(rdev, mddev)
		rdev_uninit_serial(rdev);
}

static int rdev_init_serial(struct md_rdev *rdev)
{
	/* serial_nums equals with BARRIER_BUCKETS_NR */
	int i, serial_nums = 1 << ((PAGE_SHIFT - ilog2(sizeof(atomic_t))));
	struct serial_in_rdev *serial = NULL;

	if (test_bit(CollisionCheck, &rdev->flags))
		return 0;

	serial = kvmalloc(sizeof(struct serial_in_rdev) * serial_nums,
			  GFP_KERNEL);
	if (!serial)
		return -ENOMEM;

	for (i = 0; i < serial_nums; i++) {
		struct serial_in_rdev *serial_tmp = &serial[i];

		spin_lock_init(&serial_tmp->serial_lock);
		serial_tmp->serial_rb = RB_ROOT_CACHED;
		init_waitqueue_head(&serial_tmp->serial_io_wait);
	}

	rdev->serial = serial;
	set_bit(CollisionCheck, &rdev->flags);

	return 0;
}

static int rdevs_init_serial(struct mddev *mddev)
{
	struct md_rdev *rdev;
	int ret = 0;

	rdev_for_each(rdev, mddev) {
		ret = rdev_init_serial(rdev);
		if (ret)
			break;
	}

	/* Free all resources if pool is not existed */
	if (ret && !mddev->serial_info_pool)
		rdevs_uninit_serial(mddev);

	return ret;
}

/*
 * rdev needs to enable serial stuffs if it meets the conditions:
 * 1. it is multi-queue device flaged with writemostly.
 * 2. the write-behind mode is enabled.
 */
static int rdev_need_serial(struct md_rdev *rdev)
{
	return (rdev && rdev->mddev->bitmap_info.max_write_behind > 0 &&
		rdev->bdev->bd_disk->queue->nr_hw_queues != 1 &&
		test_bit(WriteMostly, &rdev->flags));
}

/*
 * Init resource for rdev(s), then create serial_info_pool if:
 * 1. rdev is the first device which return true from rdev_enable_serial.
 * 2. rdev is NULL, means we want to enable serialization for all rdevs.
 */
void mddev_create_serial_pool(struct mddev *mddev, struct md_rdev *rdev)
{
	int ret = 0;

	if (rdev && !rdev_need_serial(rdev) &&
	    !test_bit(CollisionCheck, &rdev->flags))
		return;

	if (!rdev)
		ret = rdevs_init_serial(mddev);
	else
		ret = rdev_init_serial(rdev);
	if (ret)
		return;

	if (mddev->serial_info_pool == NULL) {
		/*
		 * already in memalloc noio context by
		 * mddev_suspend()
		 */
		mddev->serial_info_pool =
			mempool_create_kmalloc_pool(NR_SERIAL_INFOS,
						sizeof(struct serial_info));
		if (!mddev->serial_info_pool) {
			rdevs_uninit_serial(mddev);
			pr_err("can't alloc memory pool for serialization\n");
		}
	}
}

/*
 * Free resource from rdev(s), and destroy serial_info_pool under conditions:
 * 1. rdev is the last device flaged with CollisionCheck.
 * 2. when bitmap is destroyed while policy is not enabled.
 * 3. for disable policy, the pool is destroyed only when no rdev needs it.
 */
void mddev_destroy_serial_pool(struct mddev *mddev, struct md_rdev *rdev)
{
	if (rdev && !test_bit(CollisionCheck, &rdev->flags))
		return;

	if (mddev->serial_info_pool) {
		struct md_rdev *temp;
		int num = 0; /* used to track if other rdevs need the pool */

		rdev_for_each(temp, mddev) {
			if (!rdev) {
				if (!mddev->serialize_policy ||
				    !rdev_need_serial(temp))
					rdev_uninit_serial(temp);
				else
					num++;
			} else if (temp != rdev &&
				   test_bit(CollisionCheck, &temp->flags))
				num++;
		}

		if (rdev)
			rdev_uninit_serial(rdev);

		if (num)
			pr_info("The mempool could be used by other devices\n");
		else {
			mempool_destroy(mddev->serial_info_pool);
			mddev->serial_info_pool = NULL;
		}
	}
}

static struct ctl_table_header *raid_table_header;

static struct ctl_table raid_table[] = {
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	{
		.procname	= "speed_limit_min",
		.data		= &sysctl_speed_limit_min,
		.maxlen		= sizeof(int),
		.mode		= S_IRUGO|S_IWUSR,
		.proc_handler	= proc_dointvec,
	},
	{
		.procname	= "speed_limit_max",
		.data		= &sysctl_speed_limit_max,
		.maxlen		= sizeof(int),
		.mode		= S_IRUGO|S_IWUSR,
		.proc_handler	= proc_dointvec,
	},
<<<<<<< HEAD
	{ }
};

static ctl_table raid_dir_table[] = {
	{
		.procname	= "raid",
		.maxlen		= 0,
		.mode		= S_IRUGO|S_IXUGO,
		.child		= raid_table,
	},
	{ }
};

static ctl_table raid_root_table[] = {
	{
		.procname	= "dev",
		.maxlen		= 0,
		.mode		= 0555,
		.child		= raid_dir_table,
	},
	{  }
};

static const struct block_device_operations md_fops;

static int start_readonly;

/* bio_clone_mddev
 * like bio_clone, but with a local bio set
 */

static void mddev_bio_destructor(struct bio *bio)
{
	struct mddev *mddev, **mddevp;

	mddevp = (void*)bio;
	mddev = mddevp[-1];

	bio_free(bio, mddev->bio_set);
}

struct bio *bio_alloc_mddev(gfp_t gfp_mask, int nr_iovecs,
			    struct mddev *mddev)
{
	struct bio *b;
	struct mddev **mddevp;

	if (!mddev || !mddev->bio_set)
		return bio_alloc(gfp_mask, nr_iovecs);

	b = bio_alloc_bioset(gfp_mask, nr_iovecs,
			     mddev->bio_set);
	if (!b)
		return NULL;
	mddevp = (void*)b;
	mddevp[-1] = mddev;
	b->bi_destructor = mddev_bio_destructor;
	return b;
}
EXPORT_SYMBOL_GPL(bio_alloc_mddev);

struct bio *bio_clone_mddev(struct bio *bio, gfp_t gfp_mask,
			    struct mddev *mddev)
{
	struct bio *b;
	struct mddev **mddevp;

	if (!mddev || !mddev->bio_set)
		return bio_clone(bio, gfp_mask);

	b = bio_alloc_bioset(gfp_mask, bio->bi_max_vecs,
			     mddev->bio_set);
	if (!b)
		return NULL;
	mddevp = (void*)b;
	mddevp[-1] = mddev;
	b->bi_destructor = mddev_bio_destructor;
	__bio_clone(b, bio);
	if (bio_integrity(bio)) {
		int ret;

		ret = bio_integrity_clone(b, bio, gfp_mask, mddev->bio_set);

		if (ret < 0) {
			bio_put(b);
			return NULL;
		}
	}

	return b;
}
EXPORT_SYMBOL_GPL(bio_clone_mddev);

void md_trim_bio(struct bio *bio, int offset, int size)
{
	/* 'bio' is a cloned bio which we need to trim to match
	 * the given offset and size.
	 * This requires adjusting bi_sector, bi_size, and bi_io_vec
	 */
	int i;
	struct bio_vec *bvec;
	int sofar = 0;

	size <<= 9;
	if (offset == 0 && size == bio->bi_size)
		return;

	bio->bi_sector += offset;
	bio->bi_size = size;
	offset <<= 9;
	clear_bit(BIO_SEG_VALID, &bio->bi_flags);

	while (bio->bi_idx < bio->bi_vcnt &&
	       bio->bi_io_vec[bio->bi_idx].bv_len <= offset) {
		/* remove this whole bio_vec */
		offset -= bio->bi_io_vec[bio->bi_idx].bv_len;
		bio->bi_idx++;
	}
	if (bio->bi_idx < bio->bi_vcnt) {
		bio->bi_io_vec[bio->bi_idx].bv_offset += offset;
		bio->bi_io_vec[bio->bi_idx].bv_len -= offset;
	}
	/* avoid any complications with bi_idx being non-zero*/
	if (bio->bi_idx) {
		memmove(bio->bi_io_vec, bio->bi_io_vec+bio->bi_idx,
			(bio->bi_vcnt - bio->bi_idx) * sizeof(struct bio_vec));
		bio->bi_vcnt -= bio->bi_idx;
		bio->bi_idx = 0;
	}
	/* Make sure vcnt and last bv are not too big */
	bio_for_each_segment(bvec, bio, i) {
		if (sofar + bvec->bv_len > size)
			bvec->bv_len = size - sofar;
		if (bvec->bv_len == 0) {
			bio->bi_vcnt = i;
			break;
		}
		sofar += bvec->bv_len;
	}
}
EXPORT_SYMBOL_GPL(md_trim_bio);
=======
};

static int start_readonly;

/*
 * The original mechanism for creating an md device is to create
 * a device node in /dev and to open it.  This causes races with device-close.
 * The preferred method is to write to the "new_array" module parameter.
 * This can avoid races.
 * Setting create_on_open to false disables the original mechanism
 * so all the races disappear.
 */
static bool create_on_open = true;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

/*
 * We have a system wide 'event count' that is incremented
 * on any 'interesting' event, and readers of /proc/mdstat
 * can use 'poll' or 'select' to find out when the event
 * count increases.
 *
 * Events are:
 *  start array, stop array, error, add device, remove device,
 *  start build, activate spare
 */
static DECLARE_WAIT_QUEUE_HEAD(md_event_waiters);
static atomic_t md_event_count;
<<<<<<< HEAD
void md_new_event(struct mddev *mddev)
=======
void md_new_event(void)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
{
	atomic_inc(&md_event_count);
	wake_up(&md_event_waiters);
}
EXPORT_SYMBOL_GPL(md_new_event);

<<<<<<< HEAD
/* Alternate version that can be called from interrupts
 * when calling sysfs_notify isn't needed.
 */
static void md_new_event_inintr(struct mddev *mddev)
{
	atomic_inc(&md_event_count);
	wake_up(&md_event_waiters);
}

=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
/*
 * Enables to iterate over all existing md arrays
 * all_mddevs_lock protects this list.
 */
static LIST_HEAD(all_mddevs);
static DEFINE_SPINLOCK(all_mddevs_lock);

<<<<<<< HEAD

/*
 * iterates through all used mddevs in the system.
 * We take care to grab the all_mddevs_lock whenever navigating
 * the list, and to always hold a refcount when unlocked.
 * Any code which breaks out of this loop while own
 * a reference to the current mddev and must mddev_put it.
 */
#define for_each_mddev(_mddev,_tmp)					\
									\
	for (({ spin_lock(&all_mddevs_lock); 				\
		_tmp = all_mddevs.next;					\
		_mddev = NULL;});					\
	     ({ if (_tmp != &all_mddevs)				\
			mddev_get(list_entry(_tmp, struct mddev, all_mddevs));\
		spin_unlock(&all_mddevs_lock);				\
		if (_mddev) mddev_put(_mddev);				\
		_mddev = list_entry(_tmp, struct mddev, all_mddevs);	\
		_tmp != &all_mddevs;});					\
	     ({ spin_lock(&all_mddevs_lock);				\
		_tmp = _tmp->next;})					\
		)


=======
static bool is_md_suspended(struct mddev *mddev)
{
	return percpu_ref_is_dying(&mddev->active_io);
}
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
/* Rather than calling directly into the personality make_request function,
 * IO requests come here first so that we can check if the device is
 * being suspended pending a reconfiguration.
 * We hold a refcount over the call to ->make_request.  By the time that
 * call has finished, the bio has been linked into some internal structure
 * and so is visible to ->quiesce(), so we don't need the refcount any more.
 */
<<<<<<< HEAD
static void md_make_request(struct request_queue *q, struct bio *bio)
{
	const int rw = bio_data_dir(bio);
	struct mddev *mddev = q->queuedata;
	int cpu;
	unsigned int sectors;

	if (mddev == NULL || mddev->pers == NULL
	    || !mddev->ready) {
		bio_io_error(bio);
		return;
	}
	if (mddev->ro == 1 && unlikely(rw == WRITE)) {
		bio_endio(bio, bio_sectors(bio) == 0 ? 0 : -EROFS);
		return;
	}
	smp_rmb(); /* Ensure implications of  'active' are visible */
	rcu_read_lock();
	if (mddev->suspended) {
		DEFINE_WAIT(__wait);
		for (;;) {
			prepare_to_wait(&mddev->sb_wait, &__wait,
					TASK_UNINTERRUPTIBLE);
			if (!mddev->suspended)
				break;
			rcu_read_unlock();
			schedule();
			rcu_read_lock();
		}
		finish_wait(&mddev->sb_wait, &__wait);
	}
	atomic_inc(&mddev->active_io);
	rcu_read_unlock();

	/*
	 * save the sectors now since our bio can
	 * go away inside make_request
	 */
	sectors = bio_sectors(bio);
	mddev->pers->make_request(mddev, bio);

	cpu = part_stat_lock();
	part_stat_inc(cpu, &mddev->gendisk->part0, ios[rw]);
	part_stat_add(cpu, &mddev->gendisk->part0, sectors[rw], sectors);
	part_stat_unlock();

	if (atomic_dec_and_test(&mddev->active_io) && mddev->suspended)
		wake_up(&mddev->sb_wait);
}

/* mddev_suspend makes sure no new requests are submitted
 * to the device, and that any requests that have been submitted
 * are completely handled.
 * Once ->stop is called and completes, the module will be completely
 * unused.
 */
void mddev_suspend(struct mddev *mddev)
{
	BUG_ON(mddev->suspended);
	mddev->suspended = 1;
	synchronize_rcu();
	wait_event(mddev->sb_wait, atomic_read(&mddev->active_io) == 0);
	mddev->pers->quiesce(mddev, 1);

	del_timer_sync(&mddev->safemode_timer);
}
EXPORT_SYMBOL_GPL(mddev_suspend);

void mddev_resume(struct mddev *mddev)
{
	mddev->suspended = 0;
	wake_up(&mddev->sb_wait);
	mddev->pers->quiesce(mddev, 0);

	md_wakeup_thread(mddev->thread);
	md_wakeup_thread(mddev->sync_thread); /* possibly kick off a reshape */
}
EXPORT_SYMBOL_GPL(mddev_resume);

int mddev_congested(struct mddev *mddev, int bits)
{
	return mddev->suspended;
}
EXPORT_SYMBOL(mddev_congested);
=======
static bool is_suspended(struct mddev *mddev, struct bio *bio)
{
	if (is_md_suspended(mddev))
		return true;
	if (bio_data_dir(bio) != WRITE)
		return false;
	if (READ_ONCE(mddev->suspend_lo) >= READ_ONCE(mddev->suspend_hi))
		return false;
	if (bio->bi_iter.bi_sector >= READ_ONCE(mddev->suspend_hi))
		return false;
	if (bio_end_sector(bio) < READ_ONCE(mddev->suspend_lo))
		return false;
	return true;
}

bool md_handle_request(struct mddev *mddev, struct bio *bio)
{
check_suspended:
	if (is_suspended(mddev, bio)) {
		DEFINE_WAIT(__wait);
		/* Bail out if REQ_NOWAIT is set for the bio */
		if (bio->bi_opf & REQ_NOWAIT) {
			bio_wouldblock_error(bio);
			return true;
		}
		for (;;) {
			prepare_to_wait(&mddev->sb_wait, &__wait,
					TASK_UNINTERRUPTIBLE);
			if (!is_suspended(mddev, bio))
				break;
			schedule();
		}
		finish_wait(&mddev->sb_wait, &__wait);
	}
	if (!percpu_ref_tryget_live(&mddev->active_io))
		goto check_suspended;

	if (!mddev->pers->make_request(mddev, bio)) {
		percpu_ref_put(&mddev->active_io);
		if (!mddev->gendisk && mddev->pers->prepare_suspend)
			return false;
		goto check_suspended;
	}

	percpu_ref_put(&mddev->active_io);
	return true;
}
EXPORT_SYMBOL(md_handle_request);

static void md_submit_bio(struct bio *bio)
{
	const int rw = bio_data_dir(bio);
	struct mddev *mddev = bio->bi_bdev->bd_disk->private_data;

	if (mddev == NULL || mddev->pers == NULL) {
		bio_io_error(bio);
		return;
	}

	if (unlikely(test_bit(MD_BROKEN, &mddev->flags)) && (rw == WRITE)) {
		bio_io_error(bio);
		return;
	}

	bio = bio_split_to_limits(bio);
	if (!bio)
		return;

	if (mddev->ro == MD_RDONLY && unlikely(rw == WRITE)) {
		if (bio_sectors(bio) != 0)
			bio->bi_status = BLK_STS_IOERR;
		bio_endio(bio);
		return;
	}

	/* bio could be mergeable after passing to underlayer */
	bio->bi_opf &= ~REQ_NOMERGE;

	md_handle_request(mddev, bio);
}

/*
 * Make sure no new requests are submitted to the device, and any requests that
 * have been submitted are completely handled.
 */
int mddev_suspend(struct mddev *mddev, bool interruptible)
{
	int err = 0;

	/*
	 * hold reconfig_mutex to wait for normal io will deadlock, because
	 * other context can't update super_block, and normal io can rely on
	 * updating super_block.
	 */
	lockdep_assert_not_held(&mddev->reconfig_mutex);

	if (interruptible)
		err = mutex_lock_interruptible(&mddev->suspend_mutex);
	else
		mutex_lock(&mddev->suspend_mutex);
	if (err)
		return err;

	if (mddev->suspended) {
		WRITE_ONCE(mddev->suspended, mddev->suspended + 1);
		mutex_unlock(&mddev->suspend_mutex);
		return 0;
	}

	percpu_ref_kill(&mddev->active_io);
	if (interruptible)
		err = wait_event_interruptible(mddev->sb_wait,
				percpu_ref_is_zero(&mddev->active_io));
	else
		wait_event(mddev->sb_wait,
				percpu_ref_is_zero(&mddev->active_io));
	if (err) {
		percpu_ref_resurrect(&mddev->active_io);
		mutex_unlock(&mddev->suspend_mutex);
		return err;
	}

	/*
	 * For raid456, io might be waiting for reshape to make progress,
	 * allow new reshape to start while waiting for io to be done to
	 * prevent deadlock.
	 */
	WRITE_ONCE(mddev->suspended, mddev->suspended + 1);

	del_timer_sync(&mddev->safemode_timer);
	/* restrict memory reclaim I/O during raid array is suspend */
	mddev->noio_flag = memalloc_noio_save();

	mutex_unlock(&mddev->suspend_mutex);
	return 0;
}
EXPORT_SYMBOL_GPL(mddev_suspend);

static void __mddev_resume(struct mddev *mddev, bool recovery_needed)
{
	lockdep_assert_not_held(&mddev->reconfig_mutex);

	mutex_lock(&mddev->suspend_mutex);
	WRITE_ONCE(mddev->suspended, mddev->suspended - 1);
	if (mddev->suspended) {
		mutex_unlock(&mddev->suspend_mutex);
		return;
	}

	/* entred the memalloc scope from mddev_suspend() */
	memalloc_noio_restore(mddev->noio_flag);

	percpu_ref_resurrect(&mddev->active_io);
	wake_up(&mddev->sb_wait);

	if (recovery_needed)
		set_bit(MD_RECOVERY_NEEDED, &mddev->recovery);
	md_wakeup_thread(mddev->thread);
	md_wakeup_thread(mddev->sync_thread); /* possibly kick off a reshape */

	mutex_unlock(&mddev->suspend_mutex);
}

void mddev_resume(struct mddev *mddev)
{
	return __mddev_resume(mddev, true);
}
EXPORT_SYMBOL_GPL(mddev_resume);

/* sync bdev before setting device to readonly or stopping raid*/
static int mddev_set_closing_and_sync_blockdev(struct mddev *mddev, int opener_num)
{
	mutex_lock(&mddev->open_mutex);
	if (mddev->pers && atomic_read(&mddev->openers) > opener_num) {
		mutex_unlock(&mddev->open_mutex);
		return -EBUSY;
	}
	if (test_and_set_bit(MD_CLOSING, &mddev->flags)) {
		mutex_unlock(&mddev->open_mutex);
		return -EBUSY;
	}
	mutex_unlock(&mddev->open_mutex);

	sync_blockdev(mddev->gendisk->part0);
	return 0;
}
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

/*
 * Generic flush handling for md
 */

<<<<<<< HEAD
static void md_end_flush(struct bio *bio, int err)
=======
static void md_end_flush(struct bio *bio)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
{
	struct md_rdev *rdev = bio->bi_private;
	struct mddev *mddev = rdev->mddev;

<<<<<<< HEAD
	rdev_dec_pending(rdev, mddev);

	if (atomic_dec_and_test(&mddev->flush_pending)) {
		/* The pre-request flush has finished */
		queue_work(md_wq, &mddev->flush_work);
	}
	bio_put(bio);
=======
	bio_put(bio);

	rdev_dec_pending(rdev, mddev);

	if (atomic_dec_and_test(&mddev->flush_pending)) {
		/* The pair is percpu_ref_get() from md_flush_request() */
		percpu_ref_put(&mddev->active_io);

		/* The pre-request flush has finished */
		queue_work(md_wq, &mddev->flush_work);
	}
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

static void md_submit_flush_data(struct work_struct *ws);

static void submit_flushes(struct work_struct *ws)
{
	struct mddev *mddev = container_of(ws, struct mddev, flush_work);
	struct md_rdev *rdev;

<<<<<<< HEAD
=======
	mddev->start_flush = ktime_get_boottime();
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	INIT_WORK(&mddev->flush_work, md_submit_flush_data);
	atomic_set(&mddev->flush_pending, 1);
	rcu_read_lock();
	rdev_for_each_rcu(rdev, mddev)
		if (rdev->raid_disk >= 0 &&
		    !test_bit(Faulty, &rdev->flags)) {
<<<<<<< HEAD
			/* Take two references, one is dropped
			 * when request finishes, one after
			 * we reclaim rcu_read_lock
			 */
			struct bio *bi;
			atomic_inc(&rdev->nr_pending);
			atomic_inc(&rdev->nr_pending);
			rcu_read_unlock();
			bi = bio_alloc_mddev(GFP_NOIO, 0, mddev);
			bi->bi_end_io = md_end_flush;
			bi->bi_private = rdev;
			bi->bi_bdev = rdev->bdev;
			atomic_inc(&mddev->flush_pending);
			submit_bio(WRITE_FLUSH, bi);
			rcu_read_lock();
			rdev_dec_pending(rdev, mddev);
		}
	rcu_read_unlock();
	if (atomic_dec_and_test(&mddev->flush_pending))
		queue_work(md_wq, &mddev->flush_work);
=======
			struct bio *bi;

			atomic_inc(&rdev->nr_pending);
			rcu_read_unlock();
			bi = bio_alloc_bioset(rdev->bdev, 0,
					      REQ_OP_WRITE | REQ_PREFLUSH,
					      GFP_NOIO, &mddev->bio_set);
			bi->bi_end_io = md_end_flush;
			bi->bi_private = rdev;
			atomic_inc(&mddev->flush_pending);
			submit_bio(bi);
			rcu_read_lock();
		}
	rcu_read_unlock();
	if (atomic_dec_and_test(&mddev->flush_pending)) {
		/* The pair is percpu_ref_get() from md_flush_request() */
		percpu_ref_put(&mddev->active_io);

		queue_work(md_wq, &mddev->flush_work);
	}
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

static void md_submit_flush_data(struct work_struct *ws)
{
	struct mddev *mddev = container_of(ws, struct mddev, flush_work);
	struct bio *bio = mddev->flush_bio;

<<<<<<< HEAD
	if (bio->bi_size == 0)
		/* an empty barrier - all done */
		bio_endio(bio, 0);
	else {
		bio->bi_rw &= ~REQ_FLUSH;
		mddev->pers->make_request(mddev, bio);
	}

	mddev->flush_bio = NULL;
	wake_up(&mddev->sb_wait);
}

void md_flush_request(struct mddev *mddev, struct bio *bio)
{
	spin_lock_irq(&mddev->write_lock);
	wait_event_lock_irq(mddev->sb_wait,
			    !mddev->flush_bio,
			    mddev->write_lock, /*nothing*/);
	mddev->flush_bio = bio;
	spin_unlock_irq(&mddev->write_lock);

	INIT_WORK(&mddev->flush_work, submit_flushes);
	queue_work(md_wq, &mddev->flush_work);
}
EXPORT_SYMBOL(md_flush_request);

/* Support for plugging.
 * This mirrors the plugging support in request_queue, but does not
 * require having a whole queue or request structures.
 * We allocate an md_plug_cb for each md device and each thread it gets
 * plugged on.  This links tot the private plug_handle structure in the
 * personality data where we keep a count of the number of outstanding
 * plugs so other code can see if a plug is active.
 */
struct md_plug_cb {
	struct blk_plug_cb cb;
	struct mddev *mddev;
};

static void plugger_unplug(struct blk_plug_cb *cb)
{
	struct md_plug_cb *mdcb = container_of(cb, struct md_plug_cb, cb);
	if (atomic_dec_and_test(&mdcb->mddev->plug_cnt))
		md_wakeup_thread(mdcb->mddev->thread);
	kfree(mdcb);
}

/* Check that an unplug wakeup will come shortly.
 * If not, wakeup the md thread immediately
 */
int mddev_check_plugged(struct mddev *mddev)
{
	struct blk_plug *plug = current->plug;
	struct md_plug_cb *mdcb;

	if (!plug)
		return 0;

	list_for_each_entry(mdcb, &plug->cb_list, cb.list) {
		if (mdcb->cb.callback == plugger_unplug &&
		    mdcb->mddev == mddev) {
			/* Already on the list, move to top */
			if (mdcb != list_first_entry(&plug->cb_list,
						    struct md_plug_cb,
						    cb.list))
				list_move(&mdcb->cb.list, &plug->cb_list);
			return 1;
		}
	}
	/* Not currently on the callback list */
	mdcb = kmalloc(sizeof(*mdcb), GFP_ATOMIC);
	if (!mdcb)
		return 0;

	mdcb->mddev = mddev;
	mdcb->cb.callback = plugger_unplug;
	atomic_inc(&mddev->plug_cnt);
	list_add(&mdcb->cb.list, &plug->cb_list);
	return 1;
}
EXPORT_SYMBOL_GPL(mddev_check_plugged);

static inline struct mddev *mddev_get(struct mddev *mddev)
{
=======
	/*
	 * must reset flush_bio before calling into md_handle_request to avoid a
	 * deadlock, because other bios passed md_handle_request suspend check
	 * could wait for this and below md_handle_request could wait for those
	 * bios because of suspend check
	 */
	spin_lock_irq(&mddev->lock);
	mddev->prev_flush_start = mddev->start_flush;
	mddev->flush_bio = NULL;
	spin_unlock_irq(&mddev->lock);
	wake_up(&mddev->sb_wait);

	if (bio->bi_iter.bi_size == 0) {
		/* an empty barrier - all done */
		bio_endio(bio);
	} else {
		bio->bi_opf &= ~REQ_PREFLUSH;
		md_handle_request(mddev, bio);
	}
}

/*
 * Manages consolidation of flushes and submitting any flushes needed for
 * a bio with REQ_PREFLUSH.  Returns true if the bio is finished or is
 * being finished in another context.  Returns false if the flushing is
 * complete but still needs the I/O portion of the bio to be processed.
 */
bool md_flush_request(struct mddev *mddev, struct bio *bio)
{
	ktime_t req_start = ktime_get_boottime();
	spin_lock_irq(&mddev->lock);
	/* flush requests wait until ongoing flush completes,
	 * hence coalescing all the pending requests.
	 */
	wait_event_lock_irq(mddev->sb_wait,
			    !mddev->flush_bio ||
			    ktime_before(req_start, mddev->prev_flush_start),
			    mddev->lock);
	/* new request after previous flush is completed */
	if (ktime_after(req_start, mddev->prev_flush_start)) {
		WARN_ON(mddev->flush_bio);
		/*
		 * Grab a reference to make sure mddev_suspend() will wait for
		 * this flush to be done.
		 *
		 * md_flush_reqeust() is called under md_handle_request() and
		 * 'active_io' is already grabbed, hence percpu_ref_is_zero()
		 * won't pass, percpu_ref_tryget_live() can't be used because
		 * percpu_ref_kill() can be called by mddev_suspend()
		 * concurrently.
		 */
		WARN_ON(percpu_ref_is_zero(&mddev->active_io));
		percpu_ref_get(&mddev->active_io);
		mddev->flush_bio = bio;
		bio = NULL;
	}
	spin_unlock_irq(&mddev->lock);

	if (!bio) {
		INIT_WORK(&mddev->flush_work, submit_flushes);
		queue_work(md_wq, &mddev->flush_work);
	} else {
		/* flush was performed for some other bio while we waited. */
		if (bio->bi_iter.bi_size == 0)
			/* an empty barrier - all done */
			bio_endio(bio);
		else {
			bio->bi_opf &= ~REQ_PREFLUSH;
			return false;
		}
	}
	return true;
}
EXPORT_SYMBOL(md_flush_request);

static inline struct mddev *mddev_get(struct mddev *mddev)
{
	lockdep_assert_held(&all_mddevs_lock);

	if (test_bit(MD_DELETED, &mddev->flags))
		return NULL;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	atomic_inc(&mddev->active);
	return mddev;
}

static void mddev_delayed_delete(struct work_struct *ws);

<<<<<<< HEAD
static void mddev_put(struct mddev *mddev)
{
	struct bio_set *bs = NULL;

	if (!atomic_dec_and_lock(&mddev->active, &all_mddevs_lock))
		return;
	if (!mddev->raid_disks && list_empty(&mddev->disks) &&
	    mddev->ctime == 0 && !mddev->hold_active) {
		/* Array is not configured at all, and not held active,
		 * so destroy it */
		list_del_init(&mddev->all_mddevs);
		bs = mddev->bio_set;
		mddev->bio_set = NULL;
		if (mddev->gendisk) {
			/* We did a probe so need to clean up.  Call
			 * queue_work inside the spinlock so that
			 * flush_workqueue() after mddev_find will
			 * succeed in waiting for the work to be done.
			 */
			INIT_WORK(&mddev->del_work, mddev_delayed_delete);
			queue_work(md_misc_wq, &mddev->del_work);
		} else
			kfree(mddev);
	}
	spin_unlock(&all_mddevs_lock);
	if (bs)
		bioset_free(bs);
}

void mddev_init(struct mddev *mddev)
{
	mutex_init(&mddev->open_mutex);
	mutex_init(&mddev->reconfig_mutex);
	mutex_init(&mddev->bitmap_info.mutex);
	INIT_LIST_HEAD(&mddev->disks);
	INIT_LIST_HEAD(&mddev->all_mddevs);
	init_timer(&mddev->safemode_timer);
	atomic_set(&mddev->active, 1);
	atomic_set(&mddev->openers, 0);
	atomic_set(&mddev->active_io, 0);
	atomic_set(&mddev->plug_cnt, 0);
	spin_lock_init(&mddev->write_lock);
=======
static void __mddev_put(struct mddev *mddev)
{
	if (mddev->raid_disks || !list_empty(&mddev->disks) ||
	    mddev->ctime || mddev->hold_active)
		return;

	/* Array is not configured at all, and not held active, so destroy it */
	set_bit(MD_DELETED, &mddev->flags);

	/*
	 * Call queue_work inside the spinlock so that flush_workqueue() after
	 * mddev_find will succeed in waiting for the work to be done.
	 */
	queue_work(md_misc_wq, &mddev->del_work);
}

void mddev_put(struct mddev *mddev)
{
	if (!atomic_dec_and_lock(&mddev->active, &all_mddevs_lock))
		return;

	__mddev_put(mddev);
	spin_unlock(&all_mddevs_lock);
}

static void md_safemode_timeout(struct timer_list *t);
static void md_start_sync(struct work_struct *ws);

static void active_io_release(struct percpu_ref *ref)
{
	struct mddev *mddev = container_of(ref, struct mddev, active_io);

	wake_up(&mddev->sb_wait);
}

static void no_op(struct percpu_ref *r) {}

int mddev_init(struct mddev *mddev)
{

	if (percpu_ref_init(&mddev->active_io, active_io_release,
			    PERCPU_REF_ALLOW_REINIT, GFP_KERNEL))
		return -ENOMEM;

	if (percpu_ref_init(&mddev->writes_pending, no_op,
			    PERCPU_REF_ALLOW_REINIT, GFP_KERNEL)) {
		percpu_ref_exit(&mddev->active_io);
		return -ENOMEM;
	}

	/* We want to start with the refcount at zero */
	percpu_ref_put(&mddev->writes_pending);

	mutex_init(&mddev->open_mutex);
	mutex_init(&mddev->reconfig_mutex);
	mutex_init(&mddev->sync_mutex);
	mutex_init(&mddev->suspend_mutex);
	mutex_init(&mddev->bitmap_info.mutex);
	INIT_LIST_HEAD(&mddev->disks);
	INIT_LIST_HEAD(&mddev->all_mddevs);
	INIT_LIST_HEAD(&mddev->deleting);
	timer_setup(&mddev->safemode_timer, md_safemode_timeout, 0);
	atomic_set(&mddev->active, 1);
	atomic_set(&mddev->openers, 0);
	atomic_set(&mddev->sync_seq, 0);
	spin_lock_init(&mddev->lock);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	atomic_set(&mddev->flush_pending, 0);
	init_waitqueue_head(&mddev->sb_wait);
	init_waitqueue_head(&mddev->recovery_wait);
	mddev->reshape_position = MaxSector;
<<<<<<< HEAD
	mddev->resync_min = 0;
	mddev->resync_max = MaxSector;
	mddev->level = LEVEL_NONE;
}
EXPORT_SYMBOL_GPL(mddev_init);

static struct mddev * mddev_find(dev_t unit)
{
	struct mddev *mddev, *new = NULL;

	if (unit && MAJOR(unit) != MD_MAJOR)
		unit &= ~((1<<MdpMinorShift)-1);

 retry:
	spin_lock(&all_mddevs_lock);

	if (unit) {
		list_for_each_entry(mddev, &all_mddevs, all_mddevs)
			if (mddev->unit == unit) {
				mddev_get(mddev);
				spin_unlock(&all_mddevs_lock);
				kfree(new);
				return mddev;
			}

		if (new) {
			list_add(&new->all_mddevs, &all_mddevs);
			spin_unlock(&all_mddevs_lock);
			new->hold_active = UNTIL_IOCTL;
			return new;
		}
	} else if (new) {
		/* find an unused unit number */
		static int next_minor = 512;
		int start = next_minor;
		int is_free = 0;
		int dev = 0;
		while (!is_free) {
			dev = MKDEV(MD_MAJOR, next_minor);
			next_minor++;
			if (next_minor > MINORMASK)
				next_minor = 0;
			if (next_minor == start) {
				/* Oh dear, all in use. */
				spin_unlock(&all_mddevs_lock);
				kfree(new);
				return NULL;
			}
				
			is_free = 1;
			list_for_each_entry(mddev, &all_mddevs, all_mddevs)
				if (mddev->unit == dev) {
					is_free = 0;
					break;
				}
		}
		new->unit = dev;
		new->md_minor = MINOR(dev);
		new->hold_active = UNTIL_STOP;
		list_add(&new->all_mddevs, &all_mddevs);
		spin_unlock(&all_mddevs_lock);
		return new;
	}
	spin_unlock(&all_mddevs_lock);

	new = kzalloc(sizeof(*new), GFP_KERNEL);
	if (!new)
		return NULL;

	new->unit = unit;
	if (MAJOR(unit) == MD_MAJOR)
		new->md_minor = MINOR(unit);
	else
		new->md_minor = MINOR(unit) >> MdpMinorShift;

	mddev_init(new);

	goto retry;
}

static inline int mddev_lock(struct mddev * mddev)
{
	return mutex_lock_interruptible(&mddev->reconfig_mutex);
}

static inline int mddev_is_locked(struct mddev *mddev)
{
	return mutex_is_locked(&mddev->reconfig_mutex);
}

static inline int mddev_trylock(struct mddev * mddev)
{
	return mutex_trylock(&mddev->reconfig_mutex);
}

static struct attribute_group md_redundancy_group;

static void mddev_unlock(struct mddev * mddev)
{
=======
	mddev->reshape_backwards = 0;
	mddev->last_sync_action = "none";
	mddev->resync_min = 0;
	mddev->resync_max = MaxSector;
	mddev->level = LEVEL_NONE;

	INIT_WORK(&mddev->sync_work, md_start_sync);
	INIT_WORK(&mddev->del_work, mddev_delayed_delete);

	return 0;
}
EXPORT_SYMBOL_GPL(mddev_init);

void mddev_destroy(struct mddev *mddev)
{
	percpu_ref_exit(&mddev->active_io);
	percpu_ref_exit(&mddev->writes_pending);
}
EXPORT_SYMBOL_GPL(mddev_destroy);

static struct mddev *mddev_find_locked(dev_t unit)
{
	struct mddev *mddev;

	list_for_each_entry(mddev, &all_mddevs, all_mddevs)
		if (mddev->unit == unit)
			return mddev;

	return NULL;
}

/* find an unused unit number */
static dev_t mddev_alloc_unit(void)
{
	static int next_minor = 512;
	int start = next_minor;
	bool is_free = 0;
	dev_t dev = 0;

	while (!is_free) {
		dev = MKDEV(MD_MAJOR, next_minor);
		next_minor++;
		if (next_minor > MINORMASK)
			next_minor = 0;
		if (next_minor == start)
			return 0;		/* Oh dear, all in use. */
		is_free = !mddev_find_locked(dev);
	}

	return dev;
}

static struct mddev *mddev_alloc(dev_t unit)
{
	struct mddev *new;
	int error;

	if (unit && MAJOR(unit) != MD_MAJOR)
		unit &= ~((1 << MdpMinorShift) - 1);

	new = kzalloc(sizeof(*new), GFP_KERNEL);
	if (!new)
		return ERR_PTR(-ENOMEM);

	error = mddev_init(new);
	if (error)
		goto out_free_new;

	spin_lock(&all_mddevs_lock);
	if (unit) {
		error = -EEXIST;
		if (mddev_find_locked(unit))
			goto out_destroy_new;
		new->unit = unit;
		if (MAJOR(unit) == MD_MAJOR)
			new->md_minor = MINOR(unit);
		else
			new->md_minor = MINOR(unit) >> MdpMinorShift;
		new->hold_active = UNTIL_IOCTL;
	} else {
		error = -ENODEV;
		new->unit = mddev_alloc_unit();
		if (!new->unit)
			goto out_destroy_new;
		new->md_minor = MINOR(new->unit);
		new->hold_active = UNTIL_STOP;
	}

	list_add(&new->all_mddevs, &all_mddevs);
	spin_unlock(&all_mddevs_lock);
	return new;

out_destroy_new:
	spin_unlock(&all_mddevs_lock);
	mddev_destroy(new);
out_free_new:
	kfree(new);
	return ERR_PTR(error);
}

static void mddev_free(struct mddev *mddev)
{
	spin_lock(&all_mddevs_lock);
	list_del(&mddev->all_mddevs);
	spin_unlock(&all_mddevs_lock);

	mddev_destroy(mddev);
	kfree(mddev);
}

static const struct attribute_group md_redundancy_group;

void mddev_unlock(struct mddev *mddev)
{
	struct md_rdev *rdev;
	struct md_rdev *tmp;
	LIST_HEAD(delete);

	if (!list_empty(&mddev->deleting))
		list_splice_init(&mddev->deleting, &delete);

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	if (mddev->to_remove) {
		/* These cannot be removed under reconfig_mutex as
		 * an access to the files will try to take reconfig_mutex
		 * while holding the file unremovable, which leads to
		 * a deadlock.
		 * So hold set sysfs_active while the remove in happeing,
		 * and anything else which might set ->to_remove or my
		 * otherwise change the sysfs namespace will fail with
		 * -EBUSY if sysfs_active is still set.
		 * We set sysfs_active under reconfig_mutex and elsewhere
		 * test it under the same mutex to ensure its correct value
		 * is seen.
		 */
<<<<<<< HEAD
		struct attribute_group *to_remove = mddev->to_remove;
=======
		const struct attribute_group *to_remove = mddev->to_remove;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		mddev->to_remove = NULL;
		mddev->sysfs_active = 1;
		mutex_unlock(&mddev->reconfig_mutex);

		if (mddev->kobj.sd) {
			if (to_remove != &md_redundancy_group)
				sysfs_remove_group(&mddev->kobj, to_remove);
			if (mddev->pers == NULL ||
			    mddev->pers->sync_request == NULL) {
				sysfs_remove_group(&mddev->kobj, &md_redundancy_group);
				if (mddev->sysfs_action)
					sysfs_put(mddev->sysfs_action);
<<<<<<< HEAD
				mddev->sysfs_action = NULL;
=======
				if (mddev->sysfs_completed)
					sysfs_put(mddev->sysfs_completed);
				if (mddev->sysfs_degraded)
					sysfs_put(mddev->sysfs_degraded);
				mddev->sysfs_action = NULL;
				mddev->sysfs_completed = NULL;
				mddev->sysfs_degraded = NULL;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
			}
		}
		mddev->sysfs_active = 0;
	} else
		mutex_unlock(&mddev->reconfig_mutex);

<<<<<<< HEAD
	/* As we've dropped the mutex we need a spinlock to
	 * make sure the thread doesn't disappear
	 */
	spin_lock(&pers_lock);
	md_wakeup_thread(mddev->thread);
	spin_unlock(&pers_lock);
}

static struct md_rdev * find_rdev_nr(struct mddev *mddev, int nr)
{
	struct md_rdev *rdev;

	rdev_for_each(rdev, mddev)
=======
	md_wakeup_thread(mddev->thread);
	wake_up(&mddev->sb_wait);

	list_for_each_entry_safe(rdev, tmp, &delete, same_set) {
		list_del_init(&rdev->same_set);
		kobject_del(&rdev->kobj);
		export_rdev(rdev, mddev);
	}
}
EXPORT_SYMBOL_GPL(mddev_unlock);

struct md_rdev *md_find_rdev_nr_rcu(struct mddev *mddev, int nr)
{
	struct md_rdev *rdev;

	rdev_for_each_rcu(rdev, mddev)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		if (rdev->desc_nr == nr)
			return rdev;

	return NULL;
}
<<<<<<< HEAD

static struct md_rdev * find_rdev(struct mddev * mddev, dev_t dev)
=======
EXPORT_SYMBOL_GPL(md_find_rdev_nr_rcu);

static struct md_rdev *find_rdev(struct mddev *mddev, dev_t dev)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
{
	struct md_rdev *rdev;

	rdev_for_each(rdev, mddev)
		if (rdev->bdev->bd_dev == dev)
			return rdev;

	return NULL;
}

<<<<<<< HEAD
=======
struct md_rdev *md_find_rdev_rcu(struct mddev *mddev, dev_t dev)
{
	struct md_rdev *rdev;

	rdev_for_each_rcu(rdev, mddev)
		if (rdev->bdev->bd_dev == dev)
			return rdev;

	return NULL;
}
EXPORT_SYMBOL_GPL(md_find_rdev_rcu);

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
static struct md_personality *find_pers(int level, char *clevel)
{
	struct md_personality *pers;
	list_for_each_entry(pers, &pers_list, list) {
		if (level != LEVEL_NONE && pers->level == level)
			return pers;
		if (strcmp(pers->name, clevel)==0)
			return pers;
	}
	return NULL;
}

/* return the offset of the super block in 512byte sectors */
static inline sector_t calc_dev_sboffset(struct md_rdev *rdev)
{
<<<<<<< HEAD
	sector_t num_sectors = i_size_read(rdev->bdev->bd_inode) / 512;
	return MD_NEW_SIZE_SECTORS(num_sectors);
}

static int alloc_disk_sb(struct md_rdev * rdev)
{
	if (rdev->sb_page)
		MD_BUG();

	rdev->sb_page = alloc_page(GFP_KERNEL);
	if (!rdev->sb_page) {
		printk(KERN_ALERT "md: out of memory.\n");
		return -ENOMEM;
	}

	return 0;
}

static void free_disk_sb(struct md_rdev * rdev)
=======
	return MD_NEW_SIZE_SECTORS(bdev_nr_sectors(rdev->bdev));
}

static int alloc_disk_sb(struct md_rdev *rdev)
{
	rdev->sb_page = alloc_page(GFP_KERNEL);
	if (!rdev->sb_page)
		return -ENOMEM;
	return 0;
}

void md_rdev_clear(struct md_rdev *rdev)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
{
	if (rdev->sb_page) {
		put_page(rdev->sb_page);
		rdev->sb_loaded = 0;
		rdev->sb_page = NULL;
		rdev->sb_start = 0;
		rdev->sectors = 0;
	}
	if (rdev->bb_page) {
		put_page(rdev->bb_page);
		rdev->bb_page = NULL;
	}
<<<<<<< HEAD
}


static void super_written(struct bio *bio, int error)
=======
	badblocks_exit(&rdev->badblocks);
}
EXPORT_SYMBOL_GPL(md_rdev_clear);

static void super_written(struct bio *bio)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
{
	struct md_rdev *rdev = bio->bi_private;
	struct mddev *mddev = rdev->mddev;

<<<<<<< HEAD
	if (error || !test_bit(BIO_UPTODATE, &bio->bi_flags)) {
		printk("md: super_written gets error=%d, uptodate=%d\n",
		       error, test_bit(BIO_UPTODATE, &bio->bi_flags));
		WARN_ON(test_bit(BIO_UPTODATE, &bio->bi_flags));
		md_error(mddev, rdev);
	}

	if (atomic_dec_and_test(&mddev->pending_writes))
		wake_up(&mddev->sb_wait);
	bio_put(bio);
=======
	if (bio->bi_status) {
		pr_err("md: %s gets error=%d\n", __func__,
		       blk_status_to_errno(bio->bi_status));
		md_error(mddev, rdev);
		if (!test_bit(Faulty, &rdev->flags)
		    && (bio->bi_opf & MD_FAILFAST)) {
			set_bit(MD_SB_NEED_REWRITE, &mddev->sb_flags);
			set_bit(LastDev, &rdev->flags);
		}
	} else
		clear_bit(LastDev, &rdev->flags);

	bio_put(bio);

	rdev_dec_pending(rdev, mddev);

	if (atomic_dec_and_test(&mddev->pending_writes))
		wake_up(&mddev->sb_wait);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

void md_super_write(struct mddev *mddev, struct md_rdev *rdev,
		   sector_t sector, int size, struct page *page)
{
	/* write first size bytes of page to sector of rdev
	 * Increment mddev->pending_writes before returning
	 * and decrement it on completion, waking up sb_wait
	 * if zero is reached.
	 * If an error occurred, call md_error
	 */
<<<<<<< HEAD
	struct bio *bio = bio_alloc_mddev(GFP_NOIO, 1, mddev);

	bio->bi_bdev = rdev->meta_bdev ? rdev->meta_bdev : rdev->bdev;
	bio->bi_sector = sector;
	bio_add_page(bio, page, size, 0);
	bio->bi_private = rdev;
	bio->bi_end_io = super_written;

	atomic_inc(&mddev->pending_writes);
	submit_bio(WRITE_FLUSH_FUA, bio);
}

void md_super_wait(struct mddev *mddev)
{
	/* wait for all superblock writes that were scheduled to complete */
	DEFINE_WAIT(wq);
	for(;;) {
		prepare_to_wait(&mddev->sb_wait, &wq, TASK_UNINTERRUPTIBLE);
		if (atomic_read(&mddev->pending_writes)==0)
			break;
		schedule();
	}
	finish_wait(&mddev->sb_wait, &wq);
}

static void bi_complete(struct bio *bio, int error)
{
	complete((struct completion*)bio->bi_private);
}

int sync_page_io(struct md_rdev *rdev, sector_t sector, int size,
		 struct page *page, int rw, bool metadata_op)
{
	struct bio *bio = bio_alloc_mddev(GFP_NOIO, 1, rdev->mddev);
	struct completion event;
	int ret;

	rw |= REQ_SYNC;

	bio->bi_bdev = (metadata_op && rdev->meta_bdev) ?
		rdev->meta_bdev : rdev->bdev;
	if (metadata_op)
		bio->bi_sector = sector + rdev->sb_start;
	else
		bio->bi_sector = sector + rdev->data_offset;
	bio_add_page(bio, page, size, 0);
	init_completion(&event);
	bio->bi_private = &event;
	bio->bi_end_io = bi_complete;
	submit_bio(rw, bio);
	wait_for_completion(&event);

	ret = test_bit(BIO_UPTODATE, &bio->bi_flags);
	bio_put(bio);
	return ret;
}
EXPORT_SYMBOL_GPL(sync_page_io);

static int read_disk_sb(struct md_rdev * rdev, int size)
{
	char b[BDEVNAME_SIZE];
	if (!rdev->sb_page) {
		MD_BUG();
		return -EINVAL;
	}
	if (rdev->sb_loaded)
		return 0;


	if (!sync_page_io(rdev, 0, size, rdev->sb_page, READ, true))
=======
	struct bio *bio;

	if (!page)
		return;

	if (test_bit(Faulty, &rdev->flags))
		return;

	bio = bio_alloc_bioset(rdev->meta_bdev ? rdev->meta_bdev : rdev->bdev,
			      1,
			      REQ_OP_WRITE | REQ_SYNC | REQ_IDLE | REQ_META
				  | REQ_PREFLUSH | REQ_FUA,
			      GFP_NOIO, &mddev->sync_set);

	atomic_inc(&rdev->nr_pending);

	bio->bi_iter.bi_sector = sector;
	__bio_add_page(bio, page, size, 0);
	bio->bi_private = rdev;
	bio->bi_end_io = super_written;

	if (test_bit(MD_FAILFAST_SUPPORTED, &mddev->flags) &&
	    test_bit(FailFast, &rdev->flags) &&
	    !test_bit(LastDev, &rdev->flags))
		bio->bi_opf |= MD_FAILFAST;

	atomic_inc(&mddev->pending_writes);
	submit_bio(bio);
}

int md_super_wait(struct mddev *mddev)
{
	/* wait for all superblock writes that were scheduled to complete */
	wait_event(mddev->sb_wait, atomic_read(&mddev->pending_writes)==0);
	if (test_and_clear_bit(MD_SB_NEED_REWRITE, &mddev->sb_flags))
		return -EAGAIN;
	return 0;
}

int sync_page_io(struct md_rdev *rdev, sector_t sector, int size,
		 struct page *page, blk_opf_t opf, bool metadata_op)
{
	struct bio bio;
	struct bio_vec bvec;

	if (metadata_op && rdev->meta_bdev)
		bio_init(&bio, rdev->meta_bdev, &bvec, 1, opf);
	else
		bio_init(&bio, rdev->bdev, &bvec, 1, opf);

	if (metadata_op)
		bio.bi_iter.bi_sector = sector + rdev->sb_start;
	else if (rdev->mddev->reshape_position != MaxSector &&
		 (rdev->mddev->reshape_backwards ==
		  (sector >= rdev->mddev->reshape_position)))
		bio.bi_iter.bi_sector = sector + rdev->new_data_offset;
	else
		bio.bi_iter.bi_sector = sector + rdev->data_offset;
	__bio_add_page(&bio, page, size, 0);

	submit_bio_wait(&bio);

	return !bio.bi_status;
}
EXPORT_SYMBOL_GPL(sync_page_io);

static int read_disk_sb(struct md_rdev *rdev, int size)
{
	if (rdev->sb_loaded)
		return 0;

	if (!sync_page_io(rdev, 0, size, rdev->sb_page, REQ_OP_READ, true))
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		goto fail;
	rdev->sb_loaded = 1;
	return 0;

fail:
<<<<<<< HEAD
	printk(KERN_WARNING "md: disabled device %s, could not read superblock.\n",
		bdevname(rdev->bdev,b));
	return -EINVAL;
}

static int uuid_equal(mdp_super_t *sb1, mdp_super_t *sb2)
{
	return 	sb1->set_uuid0 == sb2->set_uuid0 &&
=======
	pr_err("md: disabled device %pg, could not read superblock.\n",
	       rdev->bdev);
	return -EINVAL;
}

static int md_uuid_equal(mdp_super_t *sb1, mdp_super_t *sb2)
{
	return	sb1->set_uuid0 == sb2->set_uuid0 &&
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		sb1->set_uuid1 == sb2->set_uuid1 &&
		sb1->set_uuid2 == sb2->set_uuid2 &&
		sb1->set_uuid3 == sb2->set_uuid3;
}

<<<<<<< HEAD
static int sb_equal(mdp_super_t *sb1, mdp_super_t *sb2)
=======
static int md_sb_equal(mdp_super_t *sb1, mdp_super_t *sb2)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
{
	int ret;
	mdp_super_t *tmp1, *tmp2;

	tmp1 = kmalloc(sizeof(*tmp1),GFP_KERNEL);
	tmp2 = kmalloc(sizeof(*tmp2),GFP_KERNEL);

	if (!tmp1 || !tmp2) {
		ret = 0;
<<<<<<< HEAD
		printk(KERN_INFO "md.c sb_equal(): failed to allocate memory!\n");
=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		goto abort;
	}

	*tmp1 = *sb1;
	*tmp2 = *sb2;

	/*
	 * nr_disks is not constant
	 */
	tmp1->nr_disks = 0;
	tmp2->nr_disks = 0;

	ret = (memcmp(tmp1, tmp2, MD_SB_GENERIC_CONSTANT_WORDS * 4) == 0);
abort:
	kfree(tmp1);
	kfree(tmp2);
	return ret;
}

<<<<<<< HEAD

=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
static u32 md_csum_fold(u32 csum)
{
	csum = (csum & 0xffff) + (csum >> 16);
	return (csum & 0xffff) + (csum >> 16);
}

<<<<<<< HEAD
static unsigned int calc_sb_csum(mdp_super_t * sb)
=======
static unsigned int calc_sb_csum(mdp_super_t *sb)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
{
	u64 newcsum = 0;
	u32 *sb32 = (u32*)sb;
	int i;
	unsigned int disk_csum, csum;

	disk_csum = sb->sb_csum;
	sb->sb_csum = 0;

	for (i = 0; i < MD_SB_BYTES/4 ; i++)
		newcsum += sb32[i];
	csum = (newcsum & 0xffffffff) + (newcsum>>32);

<<<<<<< HEAD

=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#ifdef CONFIG_ALPHA
	/* This used to use csum_partial, which was wrong for several
	 * reasons including that different results are returned on
	 * different architectures.  It isn't critical that we get exactly
	 * the same return value as before (we always csum_fold before
	 * testing, and that removes any differences).  However as we
	 * know that csum_partial always returned a 16bit value on
	 * alphas, do a fold to maximise conformity to previous behaviour.
	 */
	sb->sb_csum = md_csum_fold(disk_csum);
#else
	sb->sb_csum = disk_csum;
#endif
	return csum;
}

<<<<<<< HEAD

=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
/*
 * Handle superblock details.
 * We want to be able to handle multiple superblock formats
 * so we have a common interface to them all, and an array of
 * different handlers.
 * We rely on user-space to write the initial superblock, and support
 * reading and updating of superblocks.
 * Interface methods are:
 *   int load_super(struct md_rdev *dev, struct md_rdev *refdev, int minor_version)
 *      loads and validates a superblock on dev.
 *      if refdev != NULL, compare superblocks on both devices
 *    Return:
 *      0 - dev has a superblock that is compatible with refdev
 *      1 - dev has a superblock that is compatible and newer than refdev
 *          so dev should be used as the refdev in future
 *     -EINVAL superblock incompatible or invalid
 *     -othererror e.g. -EIO
 *
 *   int validate_super(struct mddev *mddev, struct md_rdev *dev)
 *      Verify that dev is acceptable into mddev.
 *       The first time, mddev->raid_disks will be 0, and data from
 *       dev should be merged in.  Subsequent calls check that dev
 *       is new enough.  Return 0 or -EINVAL
 *
 *   void sync_super(struct mddev *mddev, struct md_rdev *dev)
 *     Update the superblock for rdev with data in mddev
 *     This does not write to disc.
 *
 */

struct super_type  {
	char		    *name;
	struct module	    *owner;
<<<<<<< HEAD
	int		    (*load_super)(struct md_rdev *rdev, struct md_rdev *refdev,
					  int minor_version);
	int		    (*validate_super)(struct mddev *mddev, struct md_rdev *rdev);
	void		    (*sync_super)(struct mddev *mddev, struct md_rdev *rdev);
	unsigned long long  (*rdev_size_change)(struct md_rdev *rdev,
						sector_t num_sectors);
=======
	int		    (*load_super)(struct md_rdev *rdev,
					  struct md_rdev *refdev,
					  int minor_version);
	int		    (*validate_super)(struct mddev *mddev,
					      struct md_rdev *freshest,
					      struct md_rdev *rdev);
	void		    (*sync_super)(struct mddev *mddev,
					  struct md_rdev *rdev);
	unsigned long long  (*rdev_size_change)(struct md_rdev *rdev,
						sector_t num_sectors);
	int		    (*allow_new_offset)(struct md_rdev *rdev,
						unsigned long long new_offset);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
};

/*
 * Check that the given mddev has no bitmap.
 *
 * This function is called from the run method of all personalities that do not
 * support bitmaps. It prints an error message and returns non-zero if mddev
 * has a bitmap. Otherwise, it returns 0.
 *
 */
int md_check_no_bitmap(struct mddev *mddev)
{
	if (!mddev->bitmap_info.file && !mddev->bitmap_info.offset)
		return 0;
<<<<<<< HEAD
	printk(KERN_ERR "%s: bitmaps are not supported for %s\n",
=======
	pr_warn("%s: bitmaps are not supported for %s\n",
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		mdname(mddev), mddev->pers->name);
	return 1;
}
EXPORT_SYMBOL(md_check_no_bitmap);

/*
<<<<<<< HEAD
 * load_super for 0.90.0 
 */
static int super_90_load(struct md_rdev *rdev, struct md_rdev *refdev, int minor_version)
{
	char b[BDEVNAME_SIZE], b2[BDEVNAME_SIZE];
	mdp_super_t *sb;
	int ret;
=======
 * load_super for 0.90.0
 */
static int super_90_load(struct md_rdev *rdev, struct md_rdev *refdev, int minor_version)
{
	mdp_super_t *sb;
	int ret;
	bool spare_disk = true;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	/*
	 * Calculate the position of the superblock (512byte sectors),
	 * it's at the end of the disk.
	 *
	 * It also happens to be a multiple of 4Kb.
	 */
	rdev->sb_start = calc_dev_sboffset(rdev);

	ret = read_disk_sb(rdev, MD_SB_BYTES);
<<<<<<< HEAD
	if (ret) return ret;

	ret = -EINVAL;

	bdevname(rdev->bdev, b);
	sb = page_address(rdev->sb_page);

	if (sb->md_magic != MD_SB_MAGIC) {
		printk(KERN_ERR "md: invalid raid superblock magic on %s\n",
		       b);
=======
	if (ret)
		return ret;

	ret = -EINVAL;

	sb = page_address(rdev->sb_page);

	if (sb->md_magic != MD_SB_MAGIC) {
		pr_warn("md: invalid raid superblock magic on %pg\n",
			rdev->bdev);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		goto abort;
	}

	if (sb->major_version != 0 ||
	    sb->minor_version < 90 ||
	    sb->minor_version > 91) {
<<<<<<< HEAD
		printk(KERN_WARNING "Bad version number %d.%d on %s\n",
			sb->major_version, sb->minor_version,
			b);
=======
		pr_warn("Bad version number %d.%d on %pg\n",
			sb->major_version, sb->minor_version, rdev->bdev);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		goto abort;
	}

	if (sb->raid_disks <= 0)
		goto abort;

	if (md_csum_fold(calc_sb_csum(sb)) != md_csum_fold(sb->sb_csum)) {
<<<<<<< HEAD
		printk(KERN_WARNING "md: invalid superblock checksum on %s\n",
			b);
=======
		pr_warn("md: invalid superblock checksum on %pg\n", rdev->bdev);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		goto abort;
	}

	rdev->preferred_minor = sb->md_minor;
	rdev->data_offset = 0;
<<<<<<< HEAD
	rdev->sb_size = MD_SB_BYTES;
	rdev->badblocks.shift = -1;

	if (sb->level == LEVEL_MULTIPATH)
		rdev->desc_nr = -1;
	else
		rdev->desc_nr = sb->this_disk.number;

	if (!refdev) {
		ret = 1;
	} else {
		__u64 ev1, ev2;
		mdp_super_t *refsb = page_address(refdev->sb_page);
		if (!uuid_equal(refsb, sb)) {
			printk(KERN_WARNING "md: %s has different UUID to %s\n",
				b, bdevname(refdev->bdev,b2));
			goto abort;
		}
		if (!sb_equal(refsb, sb)) {
			printk(KERN_WARNING "md: %s has same UUID"
			       " but different superblock to %s\n",
			       b, bdevname(refdev->bdev, b2));
=======
	rdev->new_data_offset = 0;
	rdev->sb_size = MD_SB_BYTES;
	rdev->badblocks.shift = -1;

	rdev->desc_nr = sb->this_disk.number;

	/* not spare disk */
	if (rdev->desc_nr >= 0 && rdev->desc_nr < MD_SB_DISKS &&
	    sb->disks[rdev->desc_nr].state & ((1<<MD_DISK_SYNC) | (1 << MD_DISK_ACTIVE)))
		spare_disk = false;

	if (!refdev) {
		if (!spare_disk)
			ret = 1;
		else
			ret = 0;
	} else {
		__u64 ev1, ev2;
		mdp_super_t *refsb = page_address(refdev->sb_page);
		if (!md_uuid_equal(refsb, sb)) {
			pr_warn("md: %pg has different UUID to %pg\n",
				rdev->bdev, refdev->bdev);
			goto abort;
		}
		if (!md_sb_equal(refsb, sb)) {
			pr_warn("md: %pg has same UUID but different superblock to %pg\n",
				rdev->bdev, refdev->bdev);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
			goto abort;
		}
		ev1 = md_event(sb);
		ev2 = md_event(refsb);
<<<<<<< HEAD
		if (ev1 > ev2)
			ret = 1;
		else 
=======

		if (!spare_disk && ev1 > ev2)
			ret = 1;
		else
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
			ret = 0;
	}
	rdev->sectors = rdev->sb_start;
	/* Limit to 4TB as metadata cannot record more than that.
	 * (not needed for Linear and RAID0 as metadata doesn't
	 * record this size)
	 */
<<<<<<< HEAD
	if (rdev->sectors >= (2ULL << 32) && sb->level >= 1)
		rdev->sectors = (2ULL << 32) - 2;
=======
	if ((u64)rdev->sectors >= (2ULL << 32) && sb->level >= 1)
		rdev->sectors = (sector_t)(2ULL << 32) - 2;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	if (rdev->sectors < ((sector_t)sb->size) * 2 && sb->level >= 1)
		/* "this cannot possibly happen" ... */
		ret = -EINVAL;

 abort:
	return ret;
}

/*
 * validate_super for 0.90.0
<<<<<<< HEAD
 */
static int super_90_validate(struct mddev *mddev, struct md_rdev *rdev)
=======
 * note: we are not using "freshest" for 0.9 superblock
 */
static int super_90_validate(struct mddev *mddev, struct md_rdev *freshest, struct md_rdev *rdev)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
{
	mdp_disk_t *desc;
	mdp_super_t *sb = page_address(rdev->sb_page);
	__u64 ev1 = md_event(sb);

	rdev->raid_disk = -1;
	clear_bit(Faulty, &rdev->flags);
	clear_bit(In_sync, &rdev->flags);
<<<<<<< HEAD
=======
	clear_bit(Bitmap_sync, &rdev->flags);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	clear_bit(WriteMostly, &rdev->flags);

	if (mddev->raid_disks == 0) {
		mddev->major_version = 0;
		mddev->minor_version = sb->minor_version;
		mddev->patch_version = sb->patch_version;
		mddev->external = 0;
		mddev->chunk_sectors = sb->chunk_size >> 9;
		mddev->ctime = sb->ctime;
		mddev->utime = sb->utime;
		mddev->level = sb->level;
		mddev->clevel[0] = 0;
		mddev->layout = sb->layout;
		mddev->raid_disks = sb->raid_disks;
		mddev->dev_sectors = ((sector_t)sb->size) * 2;
		mddev->events = ev1;
		mddev->bitmap_info.offset = 0;
<<<<<<< HEAD
		mddev->bitmap_info.default_offset = MD_SB_BYTES >> 9;
=======
		mddev->bitmap_info.space = 0;
		/* bitmap can use 60 K after the 4K superblocks */
		mddev->bitmap_info.default_offset = MD_SB_BYTES >> 9;
		mddev->bitmap_info.default_space = 64*2 - (MD_SB_BYTES >> 9);
		mddev->reshape_backwards = 0;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

		if (mddev->minor_version >= 91) {
			mddev->reshape_position = sb->reshape_position;
			mddev->delta_disks = sb->delta_disks;
			mddev->new_level = sb->new_level;
			mddev->new_layout = sb->new_layout;
			mddev->new_chunk_sectors = sb->new_chunk >> 9;
<<<<<<< HEAD
=======
			if (mddev->delta_disks < 0)
				mddev->reshape_backwards = 1;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		} else {
			mddev->reshape_position = MaxSector;
			mddev->delta_disks = 0;
			mddev->new_level = mddev->level;
			mddev->new_layout = mddev->layout;
			mddev->new_chunk_sectors = mddev->chunk_sectors;
		}
<<<<<<< HEAD
=======
		if (mddev->level == 0)
			mddev->layout = -1;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

		if (sb->state & (1<<MD_SB_CLEAN))
			mddev->recovery_cp = MaxSector;
		else {
<<<<<<< HEAD
			if (sb->events_hi == sb->cp_events_hi && 
=======
			if (sb->events_hi == sb->cp_events_hi &&
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
				sb->events_lo == sb->cp_events_lo) {
				mddev->recovery_cp = sb->recovery_cp;
			} else
				mddev->recovery_cp = 0;
		}

		memcpy(mddev->uuid+0, &sb->set_uuid0, 4);
		memcpy(mddev->uuid+4, &sb->set_uuid1, 4);
		memcpy(mddev->uuid+8, &sb->set_uuid2, 4);
		memcpy(mddev->uuid+12,&sb->set_uuid3, 4);

		mddev->max_disks = MD_SB_DISKS;

		if (sb->state & (1<<MD_SB_BITMAP_PRESENT) &&
<<<<<<< HEAD
		    mddev->bitmap_info.file == NULL)
			mddev->bitmap_info.offset =
				mddev->bitmap_info.default_offset;
=======
		    mddev->bitmap_info.file == NULL) {
			mddev->bitmap_info.offset =
				mddev->bitmap_info.default_offset;
			mddev->bitmap_info.space =
				mddev->bitmap_info.default_space;
		}
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	} else if (mddev->pers == NULL) {
		/* Insist on good event counter while assembling, except
		 * for spares (which don't need an event count) */
		++ev1;
		if (sb->disks[rdev->desc_nr].state & (
			    (1<<MD_DISK_SYNC) | (1 << MD_DISK_ACTIVE)))
<<<<<<< HEAD
			if (ev1 < mddev->events) 
=======
			if (ev1 < mddev->events)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
				return -EINVAL;
	} else if (mddev->bitmap) {
		/* if adding to array with a bitmap, then we can accept an
		 * older device ... but not too old.
		 */
		if (ev1 < mddev->bitmap->events_cleared)
			return 0;
<<<<<<< HEAD
=======
		if (ev1 < mddev->events)
			set_bit(Bitmap_sync, &rdev->flags);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	} else {
		if (ev1 < mddev->events)
			/* just a hot-add of a new device, leave raid_disk at -1 */
			return 0;
	}

<<<<<<< HEAD
	if (mddev->level != LEVEL_MULTIPATH) {
		desc = sb->disks + rdev->desc_nr;

		if (desc->state & (1<<MD_DISK_FAULTY))
			set_bit(Faulty, &rdev->flags);
		else if (desc->state & (1<<MD_DISK_SYNC) /* &&
			    desc->raid_disk < mddev->raid_disks */) {
			set_bit(In_sync, &rdev->flags);
			rdev->raid_disk = desc->raid_disk;
		} else if (desc->state & (1<<MD_DISK_ACTIVE)) {
			/* active but not in sync implies recovery up to
			 * reshape position.  We don't know exactly where
			 * that is, so set to zero for now */
			if (mddev->minor_version >= 91) {
				rdev->recovery_offset = 0;
				rdev->raid_disk = desc->raid_disk;
			}
		}
		if (desc->state & (1<<MD_DISK_WRITEMOSTLY))
			set_bit(WriteMostly, &rdev->flags);
	} else /* MULTIPATH are always insync */
		set_bit(In_sync, &rdev->flags);
=======
	desc = sb->disks + rdev->desc_nr;

	if (desc->state & (1<<MD_DISK_FAULTY))
		set_bit(Faulty, &rdev->flags);
	else if (desc->state & (1<<MD_DISK_SYNC)) {
		set_bit(In_sync, &rdev->flags);
		rdev->raid_disk = desc->raid_disk;
		rdev->saved_raid_disk = desc->raid_disk;
	} else if (desc->state & (1<<MD_DISK_ACTIVE)) {
		/* active but not in sync implies recovery up to
		 * reshape position.  We don't know exactly where
		 * that is, so set to zero for now
		 */
		if (mddev->minor_version >= 91) {
			rdev->recovery_offset = 0;
			rdev->raid_disk = desc->raid_disk;
		}
	}
	if (desc->state & (1<<MD_DISK_WRITEMOSTLY))
		set_bit(WriteMostly, &rdev->flags);
	if (desc->state & (1<<MD_DISK_FAILFAST))
		set_bit(FailFast, &rdev->flags);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	return 0;
}

/*
 * sync_super for 0.90.0
 */
static void super_90_sync(struct mddev *mddev, struct md_rdev *rdev)
{
	mdp_super_t *sb;
	struct md_rdev *rdev2;
	int next_spare = mddev->raid_disks;

<<<<<<< HEAD

=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	/* make rdev->sb match mddev data..
	 *
	 * 1/ zero out disks
	 * 2/ Add info for each disk, keeping track of highest desc_nr (next_spare);
	 * 3/ any empty disks < next_spare become removed
	 *
	 * disks[0] gets initialised to REMOVED because
	 * we cannot be sure from other fields if it has
	 * been initialised or not.
	 */
	int i;
	int active=0, working=0,failed=0,spare=0,nr_disks=0;

	rdev->sb_size = MD_SB_BYTES;

	sb = page_address(rdev->sb_page);

	memset(sb, 0, sizeof(*sb));

	sb->md_magic = MD_SB_MAGIC;
	sb->major_version = mddev->major_version;
	sb->patch_version = mddev->patch_version;
	sb->gvalid_words  = 0; /* ignored */
	memcpy(&sb->set_uuid0, mddev->uuid+0, 4);
	memcpy(&sb->set_uuid1, mddev->uuid+4, 4);
	memcpy(&sb->set_uuid2, mddev->uuid+8, 4);
	memcpy(&sb->set_uuid3, mddev->uuid+12,4);

<<<<<<< HEAD
	sb->ctime = mddev->ctime;
=======
	sb->ctime = clamp_t(time64_t, mddev->ctime, 0, U32_MAX);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	sb->level = mddev->level;
	sb->size = mddev->dev_sectors / 2;
	sb->raid_disks = mddev->raid_disks;
	sb->md_minor = mddev->md_minor;
	sb->not_persistent = 0;
<<<<<<< HEAD
	sb->utime = mddev->utime;
=======
	sb->utime = clamp_t(time64_t, mddev->utime, 0, U32_MAX);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	sb->state = 0;
	sb->events_hi = (mddev->events>>32);
	sb->events_lo = (u32)mddev->events;

	if (mddev->reshape_position == MaxSector)
		sb->minor_version = 90;
	else {
		sb->minor_version = 91;
		sb->reshape_position = mddev->reshape_position;
		sb->new_level = mddev->new_level;
		sb->delta_disks = mddev->delta_disks;
		sb->new_layout = mddev->new_layout;
		sb->new_chunk = mddev->new_chunk_sectors << 9;
	}
	mddev->minor_version = sb->minor_version;
	if (mddev->in_sync)
	{
		sb->recovery_cp = mddev->recovery_cp;
		sb->cp_events_hi = (mddev->events>>32);
		sb->cp_events_lo = (u32)mddev->events;
		if (mddev->recovery_cp == MaxSector)
			sb->state = (1<< MD_SB_CLEAN);
	} else
		sb->recovery_cp = 0;

	sb->layout = mddev->layout;
	sb->chunk_size = mddev->chunk_sectors << 9;

	if (mddev->bitmap && mddev->bitmap_info.file == NULL)
		sb->state |= (1<<MD_SB_BITMAP_PRESENT);

	sb->disks[0].state = (1<<MD_DISK_REMOVED);
	rdev_for_each(rdev2, mddev) {
		mdp_disk_t *d;
		int desc_nr;
		int is_active = test_bit(In_sync, &rdev2->flags);

		if (rdev2->raid_disk >= 0 &&
		    sb->minor_version >= 91)
			/* we have nowhere to store the recovery_offset,
			 * but if it is not below the reshape_position,
			 * we can piggy-back on that.
			 */
			is_active = 1;
		if (rdev2->raid_disk < 0 ||
		    test_bit(Faulty, &rdev2->flags))
			is_active = 0;
		if (is_active)
			desc_nr = rdev2->raid_disk;
		else
			desc_nr = next_spare++;
		rdev2->desc_nr = desc_nr;
		d = &sb->disks[rdev2->desc_nr];
		nr_disks++;
		d->number = rdev2->desc_nr;
		d->major = MAJOR(rdev2->bdev->bd_dev);
		d->minor = MINOR(rdev2->bdev->bd_dev);
		if (is_active)
			d->raid_disk = rdev2->raid_disk;
		else
			d->raid_disk = rdev2->desc_nr; /* compatibility */
		if (test_bit(Faulty, &rdev2->flags))
			d->state = (1<<MD_DISK_FAULTY);
		else if (is_active) {
			d->state = (1<<MD_DISK_ACTIVE);
			if (test_bit(In_sync, &rdev2->flags))
				d->state |= (1<<MD_DISK_SYNC);
			active++;
			working++;
		} else {
			d->state = 0;
			spare++;
			working++;
		}
		if (test_bit(WriteMostly, &rdev2->flags))
			d->state |= (1<<MD_DISK_WRITEMOSTLY);
<<<<<<< HEAD
=======
		if (test_bit(FailFast, &rdev2->flags))
			d->state |= (1<<MD_DISK_FAILFAST);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	}
	/* now set the "removed" and "faulty" bits on any missing devices */
	for (i=0 ; i < mddev->raid_disks ; i++) {
		mdp_disk_t *d = &sb->disks[i];
		if (d->state == 0 && d->number == 0) {
			d->number = i;
			d->raid_disk = i;
			d->state = (1<<MD_DISK_REMOVED);
			d->state |= (1<<MD_DISK_FAULTY);
			failed++;
		}
	}
	sb->nr_disks = nr_disks;
	sb->active_disks = active;
	sb->working_disks = working;
	sb->failed_disks = failed;
	sb->spare_disks = spare;

	sb->this_disk = sb->disks[rdev->desc_nr];
	sb->sb_csum = calc_sb_csum(sb);
}

/*
 * rdev_size_change for 0.90.0
 */
static unsigned long long
super_90_rdev_size_change(struct md_rdev *rdev, sector_t num_sectors)
{
	if (num_sectors && num_sectors < rdev->mddev->dev_sectors)
		return 0; /* component must fit device */
	if (rdev->mddev->bitmap_info.offset)
		return 0; /* can't move bitmap */
	rdev->sb_start = calc_dev_sboffset(rdev);
	if (!num_sectors || num_sectors > rdev->sb_start)
		num_sectors = rdev->sb_start;
	/* Limit to 4TB as metadata cannot record more than that.
	 * 4TB == 2^32 KB, or 2*2^32 sectors.
	 */
<<<<<<< HEAD
	if (num_sectors >= (2ULL << 32) && rdev->mddev->level >= 1)
		num_sectors = (2ULL << 32) - 2;
	md_super_write(rdev->mddev, rdev, rdev->sb_start, rdev->sb_size,
		       rdev->sb_page);
	md_super_wait(rdev->mddev);
	return num_sectors;
}

=======
	if ((u64)num_sectors >= (2ULL << 32) && rdev->mddev->level >= 1)
		num_sectors = (sector_t)(2ULL << 32) - 2;
	do {
		md_super_write(rdev->mddev, rdev, rdev->sb_start, rdev->sb_size,
		       rdev->sb_page);
	} while (md_super_wait(rdev->mddev) < 0);
	return num_sectors;
}

static int
super_90_allow_new_offset(struct md_rdev *rdev, unsigned long long new_offset)
{
	/* non-zero offset changes not possible with v0.90 */
	return new_offset == 0;
}
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

/*
 * version 1 superblock
 */

<<<<<<< HEAD
static __le32 calc_sb_1_csum(struct mdp_superblock_1 * sb)
=======
static __le32 calc_sb_1_csum(struct mdp_superblock_1 *sb)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
{
	__le32 disk_csum;
	u32 csum;
	unsigned long long newcsum;
	int size = 256 + le32_to_cpu(sb->max_dev)*2;
	__le32 *isuper = (__le32*)sb;
<<<<<<< HEAD
	int i;
=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	disk_csum = sb->sb_csum;
	sb->sb_csum = 0;
	newcsum = 0;
<<<<<<< HEAD
	for (i=0; size>=4; size -= 4 )
=======
	for (; size >= 4; size -= 4)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		newcsum += le32_to_cpu(*isuper++);

	if (size == 2)
		newcsum += le16_to_cpu(*(__le16*) isuper);

	csum = (newcsum & 0xffffffff) + (newcsum >> 32);
	sb->sb_csum = disk_csum;
	return cpu_to_le32(csum);
}

<<<<<<< HEAD
static int md_set_badblocks(struct badblocks *bb, sector_t s, int sectors,
			    int acknowledged);
=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
static int super_1_load(struct md_rdev *rdev, struct md_rdev *refdev, int minor_version)
{
	struct mdp_superblock_1 *sb;
	int ret;
	sector_t sb_start;
<<<<<<< HEAD
	char b[BDEVNAME_SIZE], b2[BDEVNAME_SIZE];
	int bmask;
=======
	sector_t sectors;
	int bmask;
	bool spare_disk = true;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	/*
	 * Calculate the position of the superblock in 512byte sectors.
	 * It is always aligned to a 4K boundary and
	 * depeding on minor_version, it can be:
	 * 0: At least 8K, but less than 12K, from end of device
	 * 1: At start of device
	 * 2: 4K from start of device.
	 */
	switch(minor_version) {
	case 0:
<<<<<<< HEAD
		sb_start = i_size_read(rdev->bdev->bd_inode) >> 9;
		sb_start -= 8*2;
=======
		sb_start = bdev_nr_sectors(rdev->bdev) - 8 * 2;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		sb_start &= ~(sector_t)(4*2-1);
		break;
	case 1:
		sb_start = 0;
		break;
	case 2:
		sb_start = 8;
		break;
	default:
		return -EINVAL;
	}
	rdev->sb_start = sb_start;

	/* superblock is rarely larger than 1K, but it can be larger,
	 * and it is safe to read 4k, so we do that
	 */
	ret = read_disk_sb(rdev, 4096);
	if (ret) return ret;

<<<<<<< HEAD

=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	sb = page_address(rdev->sb_page);

	if (sb->magic != cpu_to_le32(MD_SB_MAGIC) ||
	    sb->major_version != cpu_to_le32(1) ||
	    le32_to_cpu(sb->max_dev) > (4096-256)/2 ||
	    le64_to_cpu(sb->super_offset) != rdev->sb_start ||
	    (le32_to_cpu(sb->feature_map) & ~MD_FEATURE_ALL) != 0)
		return -EINVAL;

	if (calc_sb_1_csum(sb) != sb->sb_csum) {
<<<<<<< HEAD
		printk("md: invalid superblock checksum on %s\n",
			bdevname(rdev->bdev,b));
		return -EINVAL;
	}
	if (le64_to_cpu(sb->data_size) < 10) {
		printk("md: data_size too small on %s\n",
		       bdevname(rdev->bdev,b));
		return -EINVAL;
	}

	rdev->preferred_minor = 0xffff;
	rdev->data_offset = le64_to_cpu(sb->data_offset);
=======
		pr_warn("md: invalid superblock checksum on %pg\n",
			rdev->bdev);
		return -EINVAL;
	}
	if (le64_to_cpu(sb->data_size) < 10) {
		pr_warn("md: data_size too small on %pg\n",
			rdev->bdev);
		return -EINVAL;
	}
	if (sb->pad0 ||
	    sb->pad3[0] ||
	    memcmp(sb->pad3, sb->pad3+1, sizeof(sb->pad3) - sizeof(sb->pad3[1])))
		/* Some padding is non-zero, might be a new feature */
		return -EINVAL;

	rdev->preferred_minor = 0xffff;
	rdev->data_offset = le64_to_cpu(sb->data_offset);
	rdev->new_data_offset = rdev->data_offset;
	if ((le32_to_cpu(sb->feature_map) & MD_FEATURE_RESHAPE_ACTIVE) &&
	    (le32_to_cpu(sb->feature_map) & MD_FEATURE_NEW_OFFSET))
		rdev->new_data_offset += (s32)le32_to_cpu(sb->new_offset);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	atomic_set(&rdev->corrected_errors, le32_to_cpu(sb->cnt_corrected_read));

	rdev->sb_size = le32_to_cpu(sb->max_dev) * 2 + 256;
	bmask = queue_logical_block_size(rdev->bdev->bd_disk->queue)-1;
	if (rdev->sb_size & bmask)
		rdev->sb_size = (rdev->sb_size | bmask) + 1;

	if (minor_version
	    && rdev->data_offset < sb_start + (rdev->sb_size/512))
		return -EINVAL;
<<<<<<< HEAD

	if (sb->level == cpu_to_le32(LEVEL_MULTIPATH))
		rdev->desc_nr = -1;
	else
		rdev->desc_nr = le32_to_cpu(sb->dev_number);
=======
	if (minor_version
	    && rdev->new_data_offset < sb_start + (rdev->sb_size/512))
		return -EINVAL;

	rdev->desc_nr = le32_to_cpu(sb->dev_number);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	if (!rdev->bb_page) {
		rdev->bb_page = alloc_page(GFP_KERNEL);
		if (!rdev->bb_page)
			return -ENOMEM;
	}
	if ((le32_to_cpu(sb->feature_map) & MD_FEATURE_BAD_BLOCKS) &&
	    rdev->badblocks.count == 0) {
		/* need to load the bad block list.
		 * Currently we limit it to one page.
		 */
		s32 offset;
		sector_t bb_sector;
<<<<<<< HEAD
		u64 *bbp;
=======
		__le64 *bbp;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		int i;
		int sectors = le16_to_cpu(sb->bblog_size);
		if (sectors > (PAGE_SIZE / 512))
			return -EINVAL;
		offset = le32_to_cpu(sb->bblog_offset);
		if (offset == 0)
			return -EINVAL;
		bb_sector = (long long)offset;
		if (!sync_page_io(rdev, bb_sector, sectors << 9,
<<<<<<< HEAD
				  rdev->bb_page, READ, true))
			return -EIO;
		bbp = (u64 *)page_address(rdev->bb_page);
=======
				  rdev->bb_page, REQ_OP_READ, true))
			return -EIO;
		bbp = (__le64 *)page_address(rdev->bb_page);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		rdev->badblocks.shift = sb->bblog_shift;
		for (i = 0 ; i < (sectors << (9-3)) ; i++, bbp++) {
			u64 bb = le64_to_cpu(*bbp);
			int count = bb & (0x3ff);
			u64 sector = bb >> 10;
			sector <<= sb->bblog_shift;
			count <<= sb->bblog_shift;
			if (bb + 1 == 0)
				break;
<<<<<<< HEAD
			if (md_set_badblocks(&rdev->badblocks,
					     sector, count, 1) == 0)
=======
			if (badblocks_set(&rdev->badblocks, sector, count, 1))
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
				return -EINVAL;
		}
	} else if (sb->bblog_offset != 0)
		rdev->badblocks.shift = 0;

<<<<<<< HEAD
	if (!refdev) {
		ret = 1;
=======
	if ((le32_to_cpu(sb->feature_map) &
	    (MD_FEATURE_PPL | MD_FEATURE_MULTIPLE_PPLS))) {
		rdev->ppl.offset = (__s16)le16_to_cpu(sb->ppl.offset);
		rdev->ppl.size = le16_to_cpu(sb->ppl.size);
		rdev->ppl.sector = rdev->sb_start + rdev->ppl.offset;
	}

	if ((le32_to_cpu(sb->feature_map) & MD_FEATURE_RAID0_LAYOUT) &&
	    sb->level != 0)
		return -EINVAL;

	/* not spare disk */
	if (rdev->desc_nr >= 0 && rdev->desc_nr < le32_to_cpu(sb->max_dev) &&
	    (le16_to_cpu(sb->dev_roles[rdev->desc_nr]) < MD_DISK_ROLE_MAX ||
	     le16_to_cpu(sb->dev_roles[rdev->desc_nr]) == MD_DISK_ROLE_JOURNAL))
		spare_disk = false;

	if (!refdev) {
		if (!spare_disk)
			ret = 1;
		else
			ret = 0;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	} else {
		__u64 ev1, ev2;
		struct mdp_superblock_1 *refsb = page_address(refdev->sb_page);

		if (memcmp(sb->set_uuid, refsb->set_uuid, 16) != 0 ||
		    sb->level != refsb->level ||
		    sb->layout != refsb->layout ||
		    sb->chunksize != refsb->chunksize) {
<<<<<<< HEAD
			printk(KERN_WARNING "md: %s has strangely different"
				" superblock to %s\n",
				bdevname(rdev->bdev,b),
				bdevname(refdev->bdev,b2));
=======
			pr_warn("md: %pg has strangely different superblock to %pg\n",
				rdev->bdev,
				refdev->bdev);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
			return -EINVAL;
		}
		ev1 = le64_to_cpu(sb->events);
		ev2 = le64_to_cpu(refsb->events);

<<<<<<< HEAD
		if (ev1 > ev2)
=======
		if (!spare_disk && ev1 > ev2)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
			ret = 1;
		else
			ret = 0;
	}
	if (minor_version)
<<<<<<< HEAD
		rdev->sectors = (i_size_read(rdev->bdev->bd_inode) >> 9) -
			le64_to_cpu(sb->data_offset);
	else
		rdev->sectors = rdev->sb_start;
	if (rdev->sectors < le64_to_cpu(sb->data_size))
		return -EINVAL;
	rdev->sectors = le64_to_cpu(sb->data_size);
	if (le64_to_cpu(sb->size) > rdev->sectors)
		return -EINVAL;
	return ret;
}

static int super_1_validate(struct mddev *mddev, struct md_rdev *rdev)
{
	struct mdp_superblock_1 *sb = page_address(rdev->sb_page);
	__u64 ev1 = le64_to_cpu(sb->events);
=======
		sectors = bdev_nr_sectors(rdev->bdev) - rdev->data_offset;
	else
		sectors = rdev->sb_start;
	if (sectors < le64_to_cpu(sb->data_size))
		return -EINVAL;
	rdev->sectors = le64_to_cpu(sb->data_size);
	return ret;
}

static int super_1_validate(struct mddev *mddev, struct md_rdev *freshest, struct md_rdev *rdev)
{
	struct mdp_superblock_1 *sb = page_address(rdev->sb_page);
	__u64 ev1 = le64_to_cpu(sb->events);
	int role;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	rdev->raid_disk = -1;
	clear_bit(Faulty, &rdev->flags);
	clear_bit(In_sync, &rdev->flags);
<<<<<<< HEAD
=======
	clear_bit(Bitmap_sync, &rdev->flags);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	clear_bit(WriteMostly, &rdev->flags);

	if (mddev->raid_disks == 0) {
		mddev->major_version = 1;
		mddev->patch_version = 0;
		mddev->external = 0;
		mddev->chunk_sectors = le32_to_cpu(sb->chunksize);
<<<<<<< HEAD
		mddev->ctime = le64_to_cpu(sb->ctime) & ((1ULL << 32)-1);
		mddev->utime = le64_to_cpu(sb->utime) & ((1ULL << 32)-1);
=======
		mddev->ctime = le64_to_cpu(sb->ctime);
		mddev->utime = le64_to_cpu(sb->utime);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		mddev->level = le32_to_cpu(sb->level);
		mddev->clevel[0] = 0;
		mddev->layout = le32_to_cpu(sb->layout);
		mddev->raid_disks = le32_to_cpu(sb->raid_disks);
		mddev->dev_sectors = le64_to_cpu(sb->size);
		mddev->events = ev1;
		mddev->bitmap_info.offset = 0;
<<<<<<< HEAD
		mddev->bitmap_info.default_offset = 1024 >> 9;
		
=======
		mddev->bitmap_info.space = 0;
		/* Default location for bitmap is 1K after superblock
		 * using 3K - total of 4K
		 */
		mddev->bitmap_info.default_offset = 1024 >> 9;
		mddev->bitmap_info.default_space = (4096-1024) >> 9;
		mddev->reshape_backwards = 0;

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		mddev->recovery_cp = le64_to_cpu(sb->resync_offset);
		memcpy(mddev->uuid, sb->set_uuid, 16);

		mddev->max_disks =  (4096-256)/2;

		if ((le32_to_cpu(sb->feature_map) & MD_FEATURE_BITMAP_OFFSET) &&
<<<<<<< HEAD
		    mddev->bitmap_info.file == NULL )
			mddev->bitmap_info.offset =
				(__s32)le32_to_cpu(sb->bitmap_offset);
=======
		    mddev->bitmap_info.file == NULL) {
			mddev->bitmap_info.offset =
				(__s32)le32_to_cpu(sb->bitmap_offset);
			/* Metadata doesn't record how much space is available.
			 * For 1.0, we assume we can use up to the superblock
			 * if before, else to 4K beyond superblock.
			 * For others, assume no change is possible.
			 */
			if (mddev->minor_version > 0)
				mddev->bitmap_info.space = 0;
			else if (mddev->bitmap_info.offset > 0)
				mddev->bitmap_info.space =
					8 - mddev->bitmap_info.offset;
			else
				mddev->bitmap_info.space =
					-mddev->bitmap_info.offset;
		}
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

		if ((le32_to_cpu(sb->feature_map) & MD_FEATURE_RESHAPE_ACTIVE)) {
			mddev->reshape_position = le64_to_cpu(sb->reshape_position);
			mddev->delta_disks = le32_to_cpu(sb->delta_disks);
			mddev->new_level = le32_to_cpu(sb->new_level);
			mddev->new_layout = le32_to_cpu(sb->new_layout);
			mddev->new_chunk_sectors = le32_to_cpu(sb->new_chunk);
<<<<<<< HEAD
=======
			if (mddev->delta_disks < 0 ||
			    (mddev->delta_disks == 0 &&
			     (le32_to_cpu(sb->feature_map)
			      & MD_FEATURE_RESHAPE_BACKWARDS)))
				mddev->reshape_backwards = 1;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		} else {
			mddev->reshape_position = MaxSector;
			mddev->delta_disks = 0;
			mddev->new_level = mddev->level;
			mddev->new_layout = mddev->layout;
			mddev->new_chunk_sectors = mddev->chunk_sectors;
		}

<<<<<<< HEAD
	} else if (mddev->pers == NULL) {
		/* Insist of good event counter while assembling, except for
		 * spares (which don't need an event count) */
		++ev1;
		if (rdev->desc_nr >= 0 &&
		    rdev->desc_nr < le32_to_cpu(sb->max_dev) &&
		    le16_to_cpu(sb->dev_roles[rdev->desc_nr]) < 0xfffe)
			if (ev1 < mddev->events)
=======
		if (mddev->level == 0 &&
		    !(le32_to_cpu(sb->feature_map) & MD_FEATURE_RAID0_LAYOUT))
			mddev->layout = -1;

		if (le32_to_cpu(sb->feature_map) & MD_FEATURE_JOURNAL)
			set_bit(MD_HAS_JOURNAL, &mddev->flags);

		if (le32_to_cpu(sb->feature_map) &
		    (MD_FEATURE_PPL | MD_FEATURE_MULTIPLE_PPLS)) {
			if (le32_to_cpu(sb->feature_map) &
			    (MD_FEATURE_BITMAP_OFFSET | MD_FEATURE_JOURNAL))
				return -EINVAL;
			if ((le32_to_cpu(sb->feature_map) & MD_FEATURE_PPL) &&
			    (le32_to_cpu(sb->feature_map) &
					    MD_FEATURE_MULTIPLE_PPLS))
				return -EINVAL;
			set_bit(MD_HAS_PPL, &mddev->flags);
		}
	} else if (mddev->pers == NULL) {
		/* Insist of good event counter while assembling, except for
		 * spares (which don't need an event count).
		 * Similar to mdadm, we allow event counter difference of 1
		 * from the freshest device.
		 */
		if (rdev->desc_nr >= 0 &&
		    rdev->desc_nr < le32_to_cpu(sb->max_dev) &&
		    (le16_to_cpu(sb->dev_roles[rdev->desc_nr]) < MD_DISK_ROLE_MAX ||
		     le16_to_cpu(sb->dev_roles[rdev->desc_nr]) == MD_DISK_ROLE_JOURNAL))
			if (ev1 + 1 < mddev->events)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
				return -EINVAL;
	} else if (mddev->bitmap) {
		/* If adding to array with a bitmap, then we can accept an
		 * older device, but not too old.
		 */
		if (ev1 < mddev->bitmap->events_cleared)
			return 0;
<<<<<<< HEAD
=======
		if (ev1 < mddev->events)
			set_bit(Bitmap_sync, &rdev->flags);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	} else {
		if (ev1 < mddev->events)
			/* just a hot-add of a new device, leave raid_disk at -1 */
			return 0;
	}
<<<<<<< HEAD
	if (mddev->level != LEVEL_MULTIPATH) {
		int role;
		if (rdev->desc_nr < 0 ||
		    rdev->desc_nr >= le32_to_cpu(sb->max_dev)) {
			role = 0xffff;
			rdev->desc_nr = -1;
		} else
			role = le16_to_cpu(sb->dev_roles[rdev->desc_nr]);
		switch(role) {
		case 0xffff: /* spare */
			break;
		case 0xfffe: /* faulty */
			set_bit(Faulty, &rdev->flags);
			break;
		default:
			if ((le32_to_cpu(sb->feature_map) &
			     MD_FEATURE_RECOVERY_OFFSET))
				rdev->recovery_offset = le64_to_cpu(sb->recovery_offset);
			else
				set_bit(In_sync, &rdev->flags);
			rdev->raid_disk = role;
			break;
		}
		if (sb->devflags & WriteMostly1)
			set_bit(WriteMostly, &rdev->flags);
		if (le32_to_cpu(sb->feature_map) & MD_FEATURE_REPLACEMENT)
			set_bit(Replacement, &rdev->flags);
	} else /* MULTIPATH are always insync */
		set_bit(In_sync, &rdev->flags);
=======

	if (rdev->desc_nr < 0 ||
	    rdev->desc_nr >= le32_to_cpu(sb->max_dev)) {
		role = MD_DISK_ROLE_SPARE;
		rdev->desc_nr = -1;
	} else if (mddev->pers == NULL && freshest && ev1 < mddev->events) {
		/*
		 * If we are assembling, and our event counter is smaller than the
		 * highest event counter, we cannot trust our superblock about the role.
		 * It could happen that our rdev was marked as Faulty, and all other
		 * superblocks were updated with +1 event counter.
		 * Then, before the next superblock update, which typically happens when
		 * remove_and_add_spares() removes the device from the array, there was
		 * a crash or reboot.
		 * If we allow current rdev without consulting the freshest superblock,
		 * we could cause data corruption.
		 * Note that in this case our event counter is smaller by 1 than the
		 * highest, otherwise, this rdev would not be allowed into array;
		 * both kernel and mdadm allow event counter difference of 1.
		 */
		struct mdp_superblock_1 *freshest_sb = page_address(freshest->sb_page);
		u32 freshest_max_dev = le32_to_cpu(freshest_sb->max_dev);

		if (rdev->desc_nr >= freshest_max_dev) {
			/* this is unexpected, better not proceed */
			pr_warn("md: %s: rdev[%pg]: desc_nr(%d) >= freshest(%pg)->sb->max_dev(%u)\n",
				mdname(mddev), rdev->bdev, rdev->desc_nr,
				freshest->bdev, freshest_max_dev);
			return -EUCLEAN;
		}

		role = le16_to_cpu(freshest_sb->dev_roles[rdev->desc_nr]);
		pr_debug("md: %s: rdev[%pg]: role=%d(0x%x) according to freshest %pg\n",
			 mdname(mddev), rdev->bdev, role, role, freshest->bdev);
	} else {
		role = le16_to_cpu(sb->dev_roles[rdev->desc_nr]);
	}
	switch (role) {
	case MD_DISK_ROLE_SPARE: /* spare */
		break;
	case MD_DISK_ROLE_FAULTY: /* faulty */
		set_bit(Faulty, &rdev->flags);
		break;
	case MD_DISK_ROLE_JOURNAL: /* journal device */
		if (!(le32_to_cpu(sb->feature_map) & MD_FEATURE_JOURNAL)) {
			/* journal device without journal feature */
			pr_warn("md: journal device provided without journal feature, ignoring the device\n");
			return -EINVAL;
		}
		set_bit(Journal, &rdev->flags);
		rdev->journal_tail = le64_to_cpu(sb->journal_tail);
		rdev->raid_disk = 0;
		break;
	default:
		rdev->saved_raid_disk = role;
		if ((le32_to_cpu(sb->feature_map) &
		     MD_FEATURE_RECOVERY_OFFSET)) {
			rdev->recovery_offset = le64_to_cpu(sb->recovery_offset);
			if (!(le32_to_cpu(sb->feature_map) &
			      MD_FEATURE_RECOVERY_BITMAP))
				rdev->saved_raid_disk = -1;
		} else {
			/*
			 * If the array is FROZEN, then the device can't
			 * be in_sync with rest of array.
			 */
			if (!test_bit(MD_RECOVERY_FROZEN,
				      &mddev->recovery))
				set_bit(In_sync, &rdev->flags);
		}
		rdev->raid_disk = role;
		break;
	}
	if (sb->devflags & WriteMostly1)
		set_bit(WriteMostly, &rdev->flags);
	if (sb->devflags & FailFast1)
		set_bit(FailFast, &rdev->flags);
	if (le32_to_cpu(sb->feature_map) & MD_FEATURE_REPLACEMENT)
		set_bit(Replacement, &rdev->flags);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	return 0;
}

static void super_1_sync(struct mddev *mddev, struct md_rdev *rdev)
{
	struct mdp_superblock_1 *sb;
	struct md_rdev *rdev2;
	int max_dev, i;
	/* make rdev->sb match mddev and rdev data. */

	sb = page_address(rdev->sb_page);

	sb->feature_map = 0;
	sb->pad0 = 0;
	sb->recovery_offset = cpu_to_le64(0);
<<<<<<< HEAD
	memset(sb->pad1, 0, sizeof(sb->pad1));
=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	memset(sb->pad3, 0, sizeof(sb->pad3));

	sb->utime = cpu_to_le64((__u64)mddev->utime);
	sb->events = cpu_to_le64(mddev->events);
	if (mddev->in_sync)
		sb->resync_offset = cpu_to_le64(mddev->recovery_cp);
<<<<<<< HEAD
=======
	else if (test_bit(MD_JOURNAL_CLEAN, &mddev->flags))
		sb->resync_offset = cpu_to_le64(MaxSector);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	else
		sb->resync_offset = cpu_to_le64(0);

	sb->cnt_corrected_read = cpu_to_le32(atomic_read(&rdev->corrected_errors));

	sb->raid_disks = cpu_to_le32(mddev->raid_disks);
	sb->size = cpu_to_le64(mddev->dev_sectors);
	sb->chunksize = cpu_to_le32(mddev->chunk_sectors);
	sb->level = cpu_to_le32(mddev->level);
	sb->layout = cpu_to_le32(mddev->layout);
<<<<<<< HEAD
=======
	if (test_bit(FailFast, &rdev->flags))
		sb->devflags |= FailFast1;
	else
		sb->devflags &= ~FailFast1;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	if (test_bit(WriteMostly, &rdev->flags))
		sb->devflags |= WriteMostly1;
	else
		sb->devflags &= ~WriteMostly1;
<<<<<<< HEAD
=======
	sb->data_offset = cpu_to_le64(rdev->data_offset);
	sb->data_size = cpu_to_le64(rdev->sectors);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	if (mddev->bitmap && mddev->bitmap_info.file == NULL) {
		sb->bitmap_offset = cpu_to_le32((__u32)mddev->bitmap_info.offset);
		sb->feature_map = cpu_to_le32(MD_FEATURE_BITMAP_OFFSET);
	}

<<<<<<< HEAD
	if (rdev->raid_disk >= 0 &&
=======
	if (rdev->raid_disk >= 0 && !test_bit(Journal, &rdev->flags) &&
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	    !test_bit(In_sync, &rdev->flags)) {
		sb->feature_map |=
			cpu_to_le32(MD_FEATURE_RECOVERY_OFFSET);
		sb->recovery_offset =
			cpu_to_le64(rdev->recovery_offset);
<<<<<<< HEAD
	}
=======
		if (rdev->saved_raid_disk >= 0 && mddev->bitmap)
			sb->feature_map |=
				cpu_to_le32(MD_FEATURE_RECOVERY_BITMAP);
	}
	/* Note: recovery_offset and journal_tail share space  */
	if (test_bit(Journal, &rdev->flags))
		sb->journal_tail = cpu_to_le64(rdev->journal_tail);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	if (test_bit(Replacement, &rdev->flags))
		sb->feature_map |=
			cpu_to_le32(MD_FEATURE_REPLACEMENT);

	if (mddev->reshape_position != MaxSector) {
		sb->feature_map |= cpu_to_le32(MD_FEATURE_RESHAPE_ACTIVE);
		sb->reshape_position = cpu_to_le64(mddev->reshape_position);
		sb->new_layout = cpu_to_le32(mddev->new_layout);
		sb->delta_disks = cpu_to_le32(mddev->delta_disks);
		sb->new_level = cpu_to_le32(mddev->new_level);
		sb->new_chunk = cpu_to_le32(mddev->new_chunk_sectors);
<<<<<<< HEAD
	}

=======
		if (mddev->delta_disks == 0 &&
		    mddev->reshape_backwards)
			sb->feature_map
				|= cpu_to_le32(MD_FEATURE_RESHAPE_BACKWARDS);
		if (rdev->new_data_offset != rdev->data_offset) {
			sb->feature_map
				|= cpu_to_le32(MD_FEATURE_NEW_OFFSET);
			sb->new_offset = cpu_to_le32((__u32)(rdev->new_data_offset
							     - rdev->data_offset));
		}
	}

	if (mddev_is_clustered(mddev))
		sb->feature_map |= cpu_to_le32(MD_FEATURE_CLUSTERED);

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	if (rdev->badblocks.count == 0)
		/* Nothing to do for bad blocks*/ ;
	else if (sb->bblog_offset == 0)
		/* Cannot record bad blocks on this device */
		md_error(mddev, rdev);
	else {
		struct badblocks *bb = &rdev->badblocks;
<<<<<<< HEAD
		u64 *bbp = (u64 *)page_address(rdev->bb_page);
=======
		__le64 *bbp = (__le64 *)page_address(rdev->bb_page);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		u64 *p = bb->page;
		sb->feature_map |= cpu_to_le32(MD_FEATURE_BAD_BLOCKS);
		if (bb->changed) {
			unsigned seq;

retry:
			seq = read_seqbegin(&bb->lock);

			memset(bbp, 0xff, PAGE_SIZE);

			for (i = 0 ; i < bb->count ; i++) {
				u64 internal_bb = p[i];
				u64 store_bb = ((BB_OFFSET(internal_bb) << 10)
						| BB_LEN(internal_bb));
				bbp[i] = cpu_to_le64(store_bb);
			}
			bb->changed = 0;
			if (read_seqretry(&bb->lock, seq))
				goto retry;

			bb->sector = (rdev->sb_start +
				      (int)le32_to_cpu(sb->bblog_offset));
			bb->size = le16_to_cpu(sb->bblog_size);
		}
	}

	max_dev = 0;
	rdev_for_each(rdev2, mddev)
		if (rdev2->desc_nr+1 > max_dev)
			max_dev = rdev2->desc_nr+1;

	if (max_dev > le32_to_cpu(sb->max_dev)) {
		int bmask;
		sb->max_dev = cpu_to_le32(max_dev);
		rdev->sb_size = max_dev * 2 + 256;
		bmask = queue_logical_block_size(rdev->bdev->bd_disk->queue)-1;
		if (rdev->sb_size & bmask)
			rdev->sb_size = (rdev->sb_size | bmask) + 1;
	} else
		max_dev = le32_to_cpu(sb->max_dev);

	for (i=0; i<max_dev;i++)
<<<<<<< HEAD
		sb->dev_roles[i] = cpu_to_le16(0xfffe);
	
	rdev_for_each(rdev2, mddev) {
		i = rdev2->desc_nr;
		if (test_bit(Faulty, &rdev2->flags))
			sb->dev_roles[i] = cpu_to_le16(0xfffe);
		else if (test_bit(In_sync, &rdev2->flags))
			sb->dev_roles[i] = cpu_to_le16(rdev2->raid_disk);
		else if (rdev2->raid_disk >= 0)
			sb->dev_roles[i] = cpu_to_le16(rdev2->raid_disk);
		else
			sb->dev_roles[i] = cpu_to_le16(0xffff);
=======
		sb->dev_roles[i] = cpu_to_le16(MD_DISK_ROLE_SPARE);

	if (test_bit(MD_HAS_JOURNAL, &mddev->flags))
		sb->feature_map |= cpu_to_le32(MD_FEATURE_JOURNAL);

	if (test_bit(MD_HAS_PPL, &mddev->flags)) {
		if (test_bit(MD_HAS_MULTIPLE_PPLS, &mddev->flags))
			sb->feature_map |=
			    cpu_to_le32(MD_FEATURE_MULTIPLE_PPLS);
		else
			sb->feature_map |= cpu_to_le32(MD_FEATURE_PPL);
		sb->ppl.offset = cpu_to_le16(rdev->ppl.offset);
		sb->ppl.size = cpu_to_le16(rdev->ppl.size);
	}

	rdev_for_each(rdev2, mddev) {
		i = rdev2->desc_nr;
		if (test_bit(Faulty, &rdev2->flags))
			sb->dev_roles[i] = cpu_to_le16(MD_DISK_ROLE_FAULTY);
		else if (test_bit(In_sync, &rdev2->flags))
			sb->dev_roles[i] = cpu_to_le16(rdev2->raid_disk);
		else if (test_bit(Journal, &rdev2->flags))
			sb->dev_roles[i] = cpu_to_le16(MD_DISK_ROLE_JOURNAL);
		else if (rdev2->raid_disk >= 0)
			sb->dev_roles[i] = cpu_to_le16(rdev2->raid_disk);
		else
			sb->dev_roles[i] = cpu_to_le16(MD_DISK_ROLE_SPARE);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	}

	sb->sb_csum = calc_sb_1_csum(sb);
}

<<<<<<< HEAD
=======
static sector_t super_1_choose_bm_space(sector_t dev_size)
{
	sector_t bm_space;

	/* if the device is bigger than 8Gig, save 64k for bitmap
	 * usage, if bigger than 200Gig, save 128k
	 */
	if (dev_size < 64*2)
		bm_space = 0;
	else if (dev_size - 64*2 >= 200*1024*1024*2)
		bm_space = 128*2;
	else if (dev_size - 4*2 > 8*1024*1024*2)
		bm_space = 64*2;
	else
		bm_space = 4*2;
	return bm_space;
}

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
static unsigned long long
super_1_rdev_size_change(struct md_rdev *rdev, sector_t num_sectors)
{
	struct mdp_superblock_1 *sb;
	sector_t max_sectors;
	if (num_sectors && num_sectors < rdev->mddev->dev_sectors)
		return 0; /* component must fit device */
<<<<<<< HEAD
	if (rdev->sb_start < rdev->data_offset) {
		/* minor versions 1 and 2; superblock before data */
		max_sectors = i_size_read(rdev->bdev->bd_inode) >> 9;
		max_sectors -= rdev->data_offset;
=======
	if (rdev->data_offset != rdev->new_data_offset)
		return 0; /* too confusing */
	if (rdev->sb_start < rdev->data_offset) {
		/* minor versions 1 and 2; superblock before data */
		max_sectors = bdev_nr_sectors(rdev->bdev) - rdev->data_offset;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		if (!num_sectors || num_sectors > max_sectors)
			num_sectors = max_sectors;
	} else if (rdev->mddev->bitmap_info.offset) {
		/* minor version 0 with bitmap we can't move */
		return 0;
	} else {
		/* minor version 0; superblock after data */
<<<<<<< HEAD
		sector_t sb_start;
		sb_start = (i_size_read(rdev->bdev->bd_inode) >> 9) - 8*2;
		sb_start &= ~(sector_t)(4*2 - 1);
		max_sectors = rdev->sectors + sb_start - rdev->sb_start;
=======
		sector_t sb_start, bm_space;
		sector_t dev_size = bdev_nr_sectors(rdev->bdev);

		/* 8K is for superblock */
		sb_start = dev_size - 8*2;
		sb_start &= ~(sector_t)(4*2 - 1);

		bm_space = super_1_choose_bm_space(dev_size);

		/* Space that can be used to store date needs to decrease
		 * superblock bitmap space and bad block space(4K)
		 */
		max_sectors = sb_start - bm_space - 4*2;

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		if (!num_sectors || num_sectors > max_sectors)
			num_sectors = max_sectors;
		rdev->sb_start = sb_start;
	}
	sb = page_address(rdev->sb_page);
	sb->data_size = cpu_to_le64(num_sectors);
<<<<<<< HEAD
	sb->super_offset = rdev->sb_start;
	sb->sb_csum = calc_sb_1_csum(sb);
	md_super_write(rdev->mddev, rdev, rdev->sb_start, rdev->sb_size,
		       rdev->sb_page);
	md_super_wait(rdev->mddev);
	return num_sectors;
=======
	sb->super_offset = cpu_to_le64(rdev->sb_start);
	sb->sb_csum = calc_sb_1_csum(sb);
	do {
		md_super_write(rdev->mddev, rdev, rdev->sb_start, rdev->sb_size,
			       rdev->sb_page);
	} while (md_super_wait(rdev->mddev) < 0);
	return num_sectors;

}

static int
super_1_allow_new_offset(struct md_rdev *rdev,
			 unsigned long long new_offset)
{
	/* All necessary checks on new >= old have been done */
	struct bitmap *bitmap;
	if (new_offset >= rdev->data_offset)
		return 1;

	/* with 1.0 metadata, there is no metadata to tread on
	 * so we can always move back */
	if (rdev->mddev->minor_version == 0)
		return 1;

	/* otherwise we must be sure not to step on
	 * any metadata, so stay:
	 * 36K beyond start of superblock
	 * beyond end of badblocks
	 * beyond write-intent bitmap
	 */
	if (rdev->sb_start + (32+4)*2 > new_offset)
		return 0;
	bitmap = rdev->mddev->bitmap;
	if (bitmap && !rdev->mddev->bitmap_info.file &&
	    rdev->sb_start + rdev->mddev->bitmap_info.offset +
	    bitmap->storage.file_pages * (PAGE_SIZE>>9) > new_offset)
		return 0;
	if (rdev->badblocks.sector + rdev->badblocks.size > new_offset)
		return 0;

	return 1;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

static struct super_type super_types[] = {
	[0] = {
		.name	= "0.90.0",
		.owner	= THIS_MODULE,
		.load_super	    = super_90_load,
		.validate_super	    = super_90_validate,
		.sync_super	    = super_90_sync,
		.rdev_size_change   = super_90_rdev_size_change,
<<<<<<< HEAD
=======
		.allow_new_offset   = super_90_allow_new_offset,
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	},
	[1] = {
		.name	= "md-1",
		.owner	= THIS_MODULE,
		.load_super	    = super_1_load,
		.validate_super	    = super_1_validate,
		.sync_super	    = super_1_sync,
		.rdev_size_change   = super_1_rdev_size_change,
<<<<<<< HEAD
=======
		.allow_new_offset   = super_1_allow_new_offset,
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	},
};

static void sync_super(struct mddev *mddev, struct md_rdev *rdev)
{
	if (mddev->sync_super) {
		mddev->sync_super(mddev, rdev);
		return;
	}

	BUG_ON(mddev->major_version >= ARRAY_SIZE(super_types));

	super_types[mddev->major_version].sync_super(mddev, rdev);
}

static int match_mddev_units(struct mddev *mddev1, struct mddev *mddev2)
{
	struct md_rdev *rdev, *rdev2;

	rcu_read_lock();
<<<<<<< HEAD
	rdev_for_each_rcu(rdev, mddev1)
		rdev_for_each_rcu(rdev2, mddev2)
			if (rdev->bdev->bd_contains ==
			    rdev2->bdev->bd_contains) {
				rcu_read_unlock();
				return 1;
			}
=======
	rdev_for_each_rcu(rdev, mddev1) {
		if (test_bit(Faulty, &rdev->flags) ||
		    test_bit(Journal, &rdev->flags) ||
		    rdev->raid_disk == -1)
			continue;
		rdev_for_each_rcu(rdev2, mddev2) {
			if (test_bit(Faulty, &rdev2->flags) ||
			    test_bit(Journal, &rdev2->flags) ||
			    rdev2->raid_disk == -1)
				continue;
			if (rdev->bdev->bd_disk == rdev2->bdev->bd_disk) {
				rcu_read_unlock();
				return 1;
			}
		}
	}
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	rcu_read_unlock();
	return 0;
}

static LIST_HEAD(pending_raid_disks);

/*
 * Try to register data integrity profile for an mddev
 *
 * This is called when an array is started and after a disk has been kicked
 * from the array. It only succeeds if all working and active component devices
 * are integrity capable with matching profiles.
 */
int md_integrity_register(struct mddev *mddev)
{
	struct md_rdev *rdev, *reference = NULL;

	if (list_empty(&mddev->disks))
		return 0; /* nothing to do */
<<<<<<< HEAD
	if (!mddev->gendisk || blk_get_integrity(mddev->gendisk))
=======
	if (mddev_is_dm(mddev) || blk_get_integrity(mddev->gendisk))
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		return 0; /* shouldn't register, or already is */
	rdev_for_each(rdev, mddev) {
		/* skip spares and non-functional disks */
		if (test_bit(Faulty, &rdev->flags))
			continue;
		if (rdev->raid_disk < 0)
			continue;
		if (!reference) {
			/* Use the first rdev as the reference */
			reference = rdev;
			continue;
		}
		/* does this rdev's profile match the reference profile? */
		if (blk_integrity_compare(reference->bdev->bd_disk,
				rdev->bdev->bd_disk) < 0)
			return -EINVAL;
	}
	if (!reference || !bdev_get_integrity(reference->bdev))
		return 0;
	/*
	 * All component devices are integrity capable and have matching
	 * profiles, register the common profile for the md device.
	 */
<<<<<<< HEAD
	if (blk_integrity_register(mddev->gendisk,
			bdev_get_integrity(reference->bdev)) != 0) {
		printk(KERN_ERR "md: failed to register integrity for %s\n",
			mdname(mddev));
		return -EINVAL;
	}
	printk(KERN_NOTICE "md: data integrity enabled on %s\n", mdname(mddev));
	if (bioset_integrity_create(mddev->bio_set, BIO_POOL_SIZE)) {
		printk(KERN_ERR "md: failed to create integrity pool for %s\n",
=======
	blk_integrity_register(mddev->gendisk,
			       bdev_get_integrity(reference->bdev));

	pr_debug("md: data integrity enabled on %s\n", mdname(mddev));
	if (bioset_integrity_create(&mddev->bio_set, BIO_POOL_SIZE) ||
	    (mddev->level != 1 && mddev->level != 10 &&
	     bioset_integrity_create(&mddev->io_clone_set, BIO_POOL_SIZE))) {
		/*
		 * No need to handle the failure of bioset_integrity_create,
		 * because the function is called by md_run() -> pers->run(),
		 * md_run calls bioset_exit -> bioset_integrity_free in case
		 * of failure case.
		 */
		pr_err("md: failed to create integrity pool for %s\n",
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		       mdname(mddev));
		return -EINVAL;
	}
	return 0;
}
EXPORT_SYMBOL(md_integrity_register);

<<<<<<< HEAD
/* Disable data integrity if non-capable/non-matching disk is being added */
void md_integrity_add_rdev(struct md_rdev *rdev, struct mddev *mddev)
{
	struct blk_integrity *bi_rdev = bdev_get_integrity(rdev->bdev);
	struct blk_integrity *bi_mddev = blk_get_integrity(mddev->gendisk);

	if (!bi_mddev) /* nothing to do */
		return;
	if (rdev->raid_disk < 0) /* skip spares */
		return;
	if (bi_rdev && blk_integrity_compare(mddev->gendisk,
					     rdev->bdev->bd_disk) >= 0)
		return;
	printk(KERN_NOTICE "disabling data integrity on %s\n", mdname(mddev));
	blk_integrity_unregister(mddev->gendisk);
}
EXPORT_SYMBOL(md_integrity_add_rdev);

static int bind_rdev_to_array(struct md_rdev * rdev, struct mddev * mddev)
{
	char b[BDEVNAME_SIZE];
	struct kobject *ko;
	char *s;
	int err;

	if (rdev->mddev) {
		MD_BUG();
		return -EINVAL;
	}

=======
/*
 * Attempt to add an rdev, but only if it is consistent with the current
 * integrity profile
 */
int md_integrity_add_rdev(struct md_rdev *rdev, struct mddev *mddev)
{
	struct blk_integrity *bi_mddev;

	if (mddev_is_dm(mddev))
		return 0;

	bi_mddev = blk_get_integrity(mddev->gendisk);

	if (!bi_mddev) /* nothing to do */
		return 0;

	if (blk_integrity_compare(mddev->gendisk, rdev->bdev->bd_disk) != 0) {
		pr_err("%s: incompatible integrity profile for %pg\n",
		       mdname(mddev), rdev->bdev);
		return -ENXIO;
	}

	return 0;
}
EXPORT_SYMBOL(md_integrity_add_rdev);

static bool rdev_read_only(struct md_rdev *rdev)
{
	return bdev_read_only(rdev->bdev) ||
		(rdev->meta_bdev && bdev_read_only(rdev->meta_bdev));
}

static int bind_rdev_to_array(struct md_rdev *rdev, struct mddev *mddev)
{
	char b[BDEVNAME_SIZE];
	int err;

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	/* prevent duplicates */
	if (find_rdev(mddev, rdev->bdev->bd_dev))
		return -EEXIST;

<<<<<<< HEAD
	/* make sure rdev->sectors exceeds mddev->dev_sectors */
	if (rdev->sectors && (mddev->dev_sectors == 0 ||
			rdev->sectors < mddev->dev_sectors)) {
=======
	if (rdev_read_only(rdev) && mddev->pers)
		return -EROFS;

	/* make sure rdev->sectors exceeds mddev->dev_sectors */
	if (!test_bit(Journal, &rdev->flags) &&
	    rdev->sectors &&
	    (mddev->dev_sectors == 0 || rdev->sectors < mddev->dev_sectors)) {
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		if (mddev->pers) {
			/* Cannot change size, so fail
			 * If mddev->level <= 0, then we don't care
			 * about aligning sizes (e.g. linear)
			 */
			if (mddev->level > 0)
				return -ENOSPC;
		} else
			mddev->dev_sectors = rdev->sectors;
	}

	/* Verify rdev->desc_nr is unique.
	 * If it is -1, assign a free number, else
	 * check number is not in use
	 */
<<<<<<< HEAD
	if (rdev->desc_nr < 0) {
		int choice = 0;
		if (mddev->pers) choice = mddev->raid_disks;
		while (find_rdev_nr(mddev, choice))
			choice++;
		rdev->desc_nr = choice;
	} else {
		if (find_rdev_nr(mddev, rdev->desc_nr))
			return -EBUSY;
	}
	if (mddev->max_disks && rdev->desc_nr >= mddev->max_disks) {
		printk(KERN_WARNING "md: %s: array is limited to %d devices\n",
		       mdname(mddev), mddev->max_disks);
		return -EBUSY;
	}
	bdevname(rdev->bdev,b);
	while ( (s=strchr(b, '/')) != NULL)
		*s = '!';

	rdev->mddev = mddev;
	printk(KERN_INFO "md: bind<%s>\n", b);
=======
	rcu_read_lock();
	if (rdev->desc_nr < 0) {
		int choice = 0;
		if (mddev->pers)
			choice = mddev->raid_disks;
		while (md_find_rdev_nr_rcu(mddev, choice))
			choice++;
		rdev->desc_nr = choice;
	} else {
		if (md_find_rdev_nr_rcu(mddev, rdev->desc_nr)) {
			rcu_read_unlock();
			return -EBUSY;
		}
	}
	rcu_read_unlock();
	if (!test_bit(Journal, &rdev->flags) &&
	    mddev->max_disks && rdev->desc_nr >= mddev->max_disks) {
		pr_warn("md: %s: array is limited to %d devices\n",
			mdname(mddev), mddev->max_disks);
		return -EBUSY;
	}
	snprintf(b, sizeof(b), "%pg", rdev->bdev);
	strreplace(b, '/', '!');

	rdev->mddev = mddev;
	pr_debug("md: bind<%s>\n", b);

	if (mddev->raid_disks)
		mddev_create_serial_pool(mddev, rdev);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	if ((err = kobject_add(&rdev->kobj, &mddev->kobj, "dev-%s", b)))
		goto fail;

<<<<<<< HEAD
	ko = &part_to_dev(rdev->bdev->bd_part)->kobj;
	if (sysfs_create_link(&rdev->kobj, ko, "block"))
		/* failure here is OK */;
	rdev->sysfs_state = sysfs_get_dirent_safe(rdev->kobj.sd, "state");
=======
	/* failure here is OK */
	err = sysfs_create_link(&rdev->kobj, bdev_kobj(rdev->bdev), "block");
	rdev->sysfs_state = sysfs_get_dirent_safe(rdev->kobj.sd, "state");
	rdev->sysfs_unack_badblocks =
		sysfs_get_dirent_safe(rdev->kobj.sd, "unacknowledged_bad_blocks");
	rdev->sysfs_badblocks =
		sysfs_get_dirent_safe(rdev->kobj.sd, "bad_blocks");
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	list_add_rcu(&rdev->same_set, &mddev->disks);
	bd_link_disk_holder(rdev->bdev, mddev->gendisk);

	/* May as well allow recovery to be retried once */
	mddev->recovery_disabled++;

	return 0;

 fail:
<<<<<<< HEAD
	printk(KERN_WARNING "md: failed to register dev-%s for %s\n",
	       b, mdname(mddev));
	return err;
}

static void md_delayed_delete(struct work_struct *ws)
{
	struct md_rdev *rdev = container_of(ws, struct md_rdev, del_work);
	kobject_del(&rdev->kobj);
	kobject_put(&rdev->kobj);
}

static void unbind_rdev_from_array(struct md_rdev * rdev)
{
	char b[BDEVNAME_SIZE];
	if (!rdev->mddev) {
		MD_BUG();
		return;
	}
	bd_unlink_disk_holder(rdev->bdev, rdev->mddev->gendisk);
	list_del_rcu(&rdev->same_set);
	printk(KERN_INFO "md: unbind<%s>\n", bdevname(rdev->bdev,b));
	rdev->mddev = NULL;
	sysfs_remove_link(&rdev->kobj, "block");
	sysfs_put(rdev->sysfs_state);
	rdev->sysfs_state = NULL;
	kfree(rdev->badblocks.page);
	rdev->badblocks.count = 0;
	rdev->badblocks.page = NULL;
	/* We need to delay this, otherwise we can deadlock when
	 * writing to 'remove' to "dev/state".  We also need
	 * to delay it due to rcu usage.
	 */
	synchronize_rcu();
	INIT_WORK(&rdev->del_work, md_delayed_delete);
	kobject_get(&rdev->kobj);
	queue_work(md_misc_wq, &rdev->del_work);
}

/*
 * prevent the device from being mounted, repartitioned or
 * otherwise reused by a RAID array (or any other kernel
 * subsystem), by bd_claiming the device.
 */
static int lock_rdev(struct md_rdev *rdev, dev_t dev, int shared)
{
	int err = 0;
	struct block_device *bdev;
	char b[BDEVNAME_SIZE];

	bdev = blkdev_get_by_dev(dev, FMODE_READ|FMODE_WRITE|FMODE_EXCL,
				 shared ? (struct md_rdev *)lock_rdev : rdev);
	if (IS_ERR(bdev)) {
		printk(KERN_ERR "md: could not open %s.\n",
			__bdevname(dev, b));
		return PTR_ERR(bdev);
	}
	rdev->bdev = bdev;
	return err;
}

static void unlock_rdev(struct md_rdev *rdev)
{
	struct block_device *bdev = rdev->bdev;
	rdev->bdev = NULL;
	if (!bdev)
		MD_BUG();
	blkdev_put(bdev, FMODE_READ|FMODE_WRITE|FMODE_EXCL);
}

void md_autodetect_dev(dev_t dev);

static void export_rdev(struct md_rdev * rdev)
{
	char b[BDEVNAME_SIZE];
	printk(KERN_INFO "md: export_rdev(%s)\n",
		bdevname(rdev->bdev,b));
	if (rdev->mddev)
		MD_BUG();
	free_disk_sb(rdev);
=======
	pr_warn("md: failed to register dev-%s for %s\n",
		b, mdname(mddev));
	mddev_destroy_serial_pool(mddev, rdev);
	return err;
}

void md_autodetect_dev(dev_t dev);

/* just for claiming the bdev */
static struct md_rdev claim_rdev;

static void export_rdev(struct md_rdev *rdev, struct mddev *mddev)
{
	pr_debug("md: export_rdev(%pg)\n", rdev->bdev);
	md_rdev_clear(rdev);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#ifndef MODULE
	if (test_bit(AutoDetected, &rdev->flags))
		md_autodetect_dev(rdev->bdev->bd_dev);
#endif
<<<<<<< HEAD
	unlock_rdev(rdev);
	kobject_put(&rdev->kobj);
}

static void kick_rdev_from_array(struct md_rdev * rdev)
{
	unbind_rdev_from_array(rdev);
	export_rdev(rdev);
=======
	fput(rdev->bdev_file);
	rdev->bdev = NULL;
	kobject_put(&rdev->kobj);
}

static void md_kick_rdev_from_array(struct md_rdev *rdev)
{
	struct mddev *mddev = rdev->mddev;

	bd_unlink_disk_holder(rdev->bdev, rdev->mddev->gendisk);
	list_del_rcu(&rdev->same_set);
	pr_debug("md: unbind<%pg>\n", rdev->bdev);
	mddev_destroy_serial_pool(rdev->mddev, rdev);
	WRITE_ONCE(rdev->mddev, NULL);
	sysfs_remove_link(&rdev->kobj, "block");
	sysfs_put(rdev->sysfs_state);
	sysfs_put(rdev->sysfs_unack_badblocks);
	sysfs_put(rdev->sysfs_badblocks);
	rdev->sysfs_state = NULL;
	rdev->sysfs_unack_badblocks = NULL;
	rdev->sysfs_badblocks = NULL;
	rdev->badblocks.count = 0;

	synchronize_rcu();

	/*
	 * kobject_del() will wait for all in progress writers to be done, where
	 * reconfig_mutex is held, hence it can't be called under
	 * reconfig_mutex and it's delayed to mddev_unlock().
	 */
	list_add(&rdev->same_set, &mddev->deleting);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

static void export_array(struct mddev *mddev)
{
<<<<<<< HEAD
	struct md_rdev *rdev, *tmp;

	rdev_for_each_safe(rdev, tmp, mddev) {
		if (!rdev->mddev) {
			MD_BUG();
			continue;
		}
		kick_rdev_from_array(rdev);
	}
	if (!list_empty(&mddev->disks))
		MD_BUG();
=======
	struct md_rdev *rdev;

	while (!list_empty(&mddev->disks)) {
		rdev = list_first_entry(&mddev->disks, struct md_rdev,
					same_set);
		md_kick_rdev_from_array(rdev);
	}
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	mddev->raid_disks = 0;
	mddev->major_version = 0;
}

<<<<<<< HEAD
static void print_desc(mdp_disk_t *desc)
{
	printk(" DISK<N:%d,(%d,%d),R:%d,S:%d>\n", desc->number,
		desc->major,desc->minor,desc->raid_disk,desc->state);
}

static void print_sb_90(mdp_super_t *sb)
{
	int i;

	printk(KERN_INFO 
		"md:  SB: (V:%d.%d.%d) ID:<%08x.%08x.%08x.%08x> CT:%08x\n",
		sb->major_version, sb->minor_version, sb->patch_version,
		sb->set_uuid0, sb->set_uuid1, sb->set_uuid2, sb->set_uuid3,
		sb->ctime);
	printk(KERN_INFO "md:     L%d S%08d ND:%d RD:%d md%d LO:%d CS:%d\n",
		sb->level, sb->size, sb->nr_disks, sb->raid_disks,
		sb->md_minor, sb->layout, sb->chunk_size);
	printk(KERN_INFO "md:     UT:%08x ST:%d AD:%d WD:%d"
		" FD:%d SD:%d CSUM:%08x E:%08lx\n",
		sb->utime, sb->state, sb->active_disks, sb->working_disks,
		sb->failed_disks, sb->spare_disks,
		sb->sb_csum, (unsigned long)sb->events_lo);

	printk(KERN_INFO);
	for (i = 0; i < MD_SB_DISKS; i++) {
		mdp_disk_t *desc;

		desc = sb->disks + i;
		if (desc->number || desc->major || desc->minor ||
		    desc->raid_disk || (desc->state && (desc->state != 4))) {
			printk("     D %2d: ", i);
			print_desc(desc);
		}
	}
	printk(KERN_INFO "md:     THIS: ");
	print_desc(&sb->this_disk);
}

static void print_sb_1(struct mdp_superblock_1 *sb)
{
	__u8 *uuid;

	uuid = sb->set_uuid;
	printk(KERN_INFO
	       "md:  SB: (V:%u) (F:0x%08x) Array-ID:<%pU>\n"
	       "md:    Name: \"%s\" CT:%llu\n",
		le32_to_cpu(sb->major_version),
		le32_to_cpu(sb->feature_map),
		uuid,
		sb->set_name,
		(unsigned long long)le64_to_cpu(sb->ctime)
		       & MD_SUPERBLOCK_1_TIME_SEC_MASK);

	uuid = sb->device_uuid;
	printk(KERN_INFO
	       "md:       L%u SZ%llu RD:%u LO:%u CS:%u DO:%llu DS:%llu SO:%llu"
			" RO:%llu\n"
	       "md:     Dev:%08x UUID: %pU\n"
	       "md:       (F:0x%08x) UT:%llu Events:%llu ResyncOffset:%llu CSUM:0x%08x\n"
	       "md:         (MaxDev:%u) \n",
		le32_to_cpu(sb->level),
		(unsigned long long)le64_to_cpu(sb->size),
		le32_to_cpu(sb->raid_disks),
		le32_to_cpu(sb->layout),
		le32_to_cpu(sb->chunksize),
		(unsigned long long)le64_to_cpu(sb->data_offset),
		(unsigned long long)le64_to_cpu(sb->data_size),
		(unsigned long long)le64_to_cpu(sb->super_offset),
		(unsigned long long)le64_to_cpu(sb->recovery_offset),
		le32_to_cpu(sb->dev_number),
		uuid,
		sb->devflags,
		(unsigned long long)le64_to_cpu(sb->utime) & MD_SUPERBLOCK_1_TIME_SEC_MASK,
		(unsigned long long)le64_to_cpu(sb->events),
		(unsigned long long)le64_to_cpu(sb->resync_offset),
		le32_to_cpu(sb->sb_csum),
		le32_to_cpu(sb->max_dev)
		);
}

static void print_rdev(struct md_rdev *rdev, int major_version)
{
	char b[BDEVNAME_SIZE];
	printk(KERN_INFO "md: rdev %s, Sect:%08llu F:%d S:%d DN:%u\n",
		bdevname(rdev->bdev, b), (unsigned long long)rdev->sectors,
	        test_bit(Faulty, &rdev->flags), test_bit(In_sync, &rdev->flags),
	        rdev->desc_nr);
	if (rdev->sb_loaded) {
		printk(KERN_INFO "md: rdev superblock (MJ:%d):\n", major_version);
		switch (major_version) {
		case 0:
			print_sb_90(page_address(rdev->sb_page));
			break;
		case 1:
			print_sb_1(page_address(rdev->sb_page));
			break;
		}
	} else
		printk(KERN_INFO "md: no rdev superblock!\n");
}

static void md_print_devices(void)
{
	struct list_head *tmp;
	struct md_rdev *rdev;
	struct mddev *mddev;
	char b[BDEVNAME_SIZE];

	printk("\n");
	printk("md:	**********************************\n");
	printk("md:	* <COMPLETE RAID STATE PRINTOUT> *\n");
	printk("md:	**********************************\n");
	for_each_mddev(mddev, tmp) {

		if (mddev->bitmap)
			bitmap_print_sb(mddev->bitmap);
		else
			printk("%s: ", mdname(mddev));
		rdev_for_each(rdev, mddev)
			printk("<%s>", bdevname(rdev->bdev,b));
		printk("\n");

		rdev_for_each(rdev, mddev)
			print_rdev(rdev, mddev->major_version);
	}
	printk("md:	**********************************\n");
	printk("\n");
}


static void sync_sbs(struct mddev * mddev, int nospares)
=======
static bool set_in_sync(struct mddev *mddev)
{
	lockdep_assert_held(&mddev->lock);
	if (!mddev->in_sync) {
		mddev->sync_checkers++;
		spin_unlock(&mddev->lock);
		percpu_ref_switch_to_atomic_sync(&mddev->writes_pending);
		spin_lock(&mddev->lock);
		if (!mddev->in_sync &&
		    percpu_ref_is_zero(&mddev->writes_pending)) {
			mddev->in_sync = 1;
			/*
			 * Ensure ->in_sync is visible before we clear
			 * ->sync_checkers.
			 */
			smp_mb();
			set_bit(MD_SB_CHANGE_CLEAN, &mddev->sb_flags);
			sysfs_notify_dirent_safe(mddev->sysfs_state);
		}
		if (--mddev->sync_checkers == 0)
			percpu_ref_switch_to_percpu(&mddev->writes_pending);
	}
	if (mddev->safemode == 1)
		mddev->safemode = 0;
	return mddev->in_sync;
}

static void sync_sbs(struct mddev *mddev, int nospares)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
{
	/* Update each superblock (in-memory image), but
	 * if we are allowed to, skip spares which already
	 * have the right event counter, or have one earlier
	 * (which would mean they aren't being marked as dirty
	 * with the rest of the array)
	 */
	struct md_rdev *rdev;
	rdev_for_each(rdev, mddev) {
		if (rdev->sb_events == mddev->events ||
		    (nospares &&
		     rdev->raid_disk < 0 &&
		     rdev->sb_events+1 == mddev->events)) {
			/* Don't update this superblock */
			rdev->sb_loaded = 2;
		} else {
			sync_super(mddev, rdev);
			rdev->sb_loaded = 1;
		}
	}
}

<<<<<<< HEAD
static void md_update_sb(struct mddev * mddev, int force_change)
=======
static bool does_sb_need_changing(struct mddev *mddev)
{
	struct md_rdev *rdev = NULL, *iter;
	struct mdp_superblock_1 *sb;
	int role;

	/* Find a good rdev */
	rdev_for_each(iter, mddev)
		if ((iter->raid_disk >= 0) && !test_bit(Faulty, &iter->flags)) {
			rdev = iter;
			break;
		}

	/* No good device found. */
	if (!rdev)
		return false;

	sb = page_address(rdev->sb_page);
	/* Check if a device has become faulty or a spare become active */
	rdev_for_each(rdev, mddev) {
		role = le16_to_cpu(sb->dev_roles[rdev->desc_nr]);
		/* Device activated? */
		if (role == MD_DISK_ROLE_SPARE && rdev->raid_disk >= 0 &&
		    !test_bit(Faulty, &rdev->flags))
			return true;
		/* Device turned faulty? */
		if (test_bit(Faulty, &rdev->flags) && (role < MD_DISK_ROLE_MAX))
			return true;
	}

	/* Check if any mddev parameters have changed */
	if ((mddev->dev_sectors != le64_to_cpu(sb->size)) ||
	    (mddev->reshape_position != le64_to_cpu(sb->reshape_position)) ||
	    (mddev->layout != le32_to_cpu(sb->layout)) ||
	    (mddev->raid_disks != le32_to_cpu(sb->raid_disks)) ||
	    (mddev->chunk_sectors != le32_to_cpu(sb->chunksize)))
		return true;

	return false;
}

void md_update_sb(struct mddev *mddev, int force_change)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
{
	struct md_rdev *rdev;
	int sync_req;
	int nospares = 0;
	int any_badblocks_changed = 0;
<<<<<<< HEAD

repeat:
	/* First make sure individual recovery_offsets are correct */
	rdev_for_each(rdev, mddev) {
		if (rdev->raid_disk >= 0 &&
		    mddev->delta_disks >= 0 &&
=======
	int ret = -1;

	if (!md_is_rdwr(mddev)) {
		if (force_change)
			set_bit(MD_SB_CHANGE_DEVS, &mddev->sb_flags);
		return;
	}

repeat:
	if (mddev_is_clustered(mddev)) {
		if (test_and_clear_bit(MD_SB_CHANGE_DEVS, &mddev->sb_flags))
			force_change = 1;
		if (test_and_clear_bit(MD_SB_CHANGE_CLEAN, &mddev->sb_flags))
			nospares = 1;
		ret = md_cluster_ops->metadata_update_start(mddev);
		/* Has someone else has updated the sb */
		if (!does_sb_need_changing(mddev)) {
			if (ret == 0)
				md_cluster_ops->metadata_update_cancel(mddev);
			bit_clear_unless(&mddev->sb_flags, BIT(MD_SB_CHANGE_PENDING),
							 BIT(MD_SB_CHANGE_DEVS) |
							 BIT(MD_SB_CHANGE_CLEAN));
			return;
		}
	}

	/*
	 * First make sure individual recovery_offsets are correct
	 * curr_resync_completed can only be used during recovery.
	 * During reshape/resync it might use array-addresses rather
	 * that device addresses.
	 */
	rdev_for_each(rdev, mddev) {
		if (rdev->raid_disk >= 0 &&
		    mddev->delta_disks >= 0 &&
		    test_bit(MD_RECOVERY_RUNNING, &mddev->recovery) &&
		    test_bit(MD_RECOVERY_RECOVER, &mddev->recovery) &&
		    !test_bit(MD_RECOVERY_RESHAPE, &mddev->recovery) &&
		    !test_bit(Journal, &rdev->flags) &&
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		    !test_bit(In_sync, &rdev->flags) &&
		    mddev->curr_resync_completed > rdev->recovery_offset)
				rdev->recovery_offset = mddev->curr_resync_completed;

<<<<<<< HEAD
	}	
	if (!mddev->persistent) {
		clear_bit(MD_CHANGE_CLEAN, &mddev->flags);
		clear_bit(MD_CHANGE_DEVS, &mddev->flags);
		if (!mddev->external) {
			clear_bit(MD_CHANGE_PENDING, &mddev->flags);
			rdev_for_each(rdev, mddev) {
				if (rdev->badblocks.changed) {
					rdev->badblocks.changed = 0;
					md_ack_all_badblocks(&rdev->badblocks);
=======
	}
	if (!mddev->persistent) {
		clear_bit(MD_SB_CHANGE_CLEAN, &mddev->sb_flags);
		clear_bit(MD_SB_CHANGE_DEVS, &mddev->sb_flags);
		if (!mddev->external) {
			clear_bit(MD_SB_CHANGE_PENDING, &mddev->sb_flags);
			rdev_for_each(rdev, mddev) {
				if (rdev->badblocks.changed) {
					rdev->badblocks.changed = 0;
					ack_all_badblocks(&rdev->badblocks);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
					md_error(mddev, rdev);
				}
				clear_bit(Blocked, &rdev->flags);
				clear_bit(BlockedBadBlocks, &rdev->flags);
				wake_up(&rdev->blocked_wait);
			}
		}
		wake_up(&mddev->sb_wait);
		return;
	}

<<<<<<< HEAD
	spin_lock_irq(&mddev->write_lock);

	mddev->utime = get_seconds();

	if (test_and_clear_bit(MD_CHANGE_DEVS, &mddev->flags))
		force_change = 1;
	if (test_and_clear_bit(MD_CHANGE_CLEAN, &mddev->flags))
=======
	spin_lock(&mddev->lock);

	mddev->utime = ktime_get_real_seconds();

	if (test_and_clear_bit(MD_SB_CHANGE_DEVS, &mddev->sb_flags))
		force_change = 1;
	if (test_and_clear_bit(MD_SB_CHANGE_CLEAN, &mddev->sb_flags))
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		/* just a clean<-> dirty transition, possibly leave spares alone,
		 * though if events isn't the right even/odd, we will have to do
		 * spares after all
		 */
		nospares = 1;
	if (force_change)
		nospares = 0;
	if (mddev->degraded)
		/* If the array is degraded, then skipping spares is both
		 * dangerous and fairly pointless.
		 * Dangerous because a device that was removed from the array
		 * might have a event_count that still looks up-to-date,
		 * so it can be re-added without a resync.
		 * Pointless because if there are any spares to skip,
		 * then a recovery will happen and soon that array won't
		 * be degraded any more and the spare can go back to sleep then.
		 */
		nospares = 0;

	sync_req = mddev->in_sync;

	/* If this is just a dirty<->clean transition, and the array is clean
	 * and 'events' is odd, we can roll back to the previous clean state */
	if (nospares
	    && (mddev->in_sync && mddev->recovery_cp == MaxSector)
	    && mddev->can_decrease_events
	    && mddev->events != 1) {
		mddev->events--;
		mddev->can_decrease_events = 0;
	} else {
		/* otherwise we have to go forward and ... */
		mddev->events ++;
		mddev->can_decrease_events = nospares;
	}

<<<<<<< HEAD
	if (!mddev->events) {
		/*
		 * oops, this 64-bit counter should never wrap.
		 * Either we are in around ~1 trillion A.C., assuming
		 * 1 reboot per second, or we have a bug:
		 */
		MD_BUG();
		mddev->events --;
	}
=======
	/*
	 * This 64-bit counter should never wrap.
	 * Either we are in around ~1 trillion A.C., assuming
	 * 1 reboot per second, or we have a bug...
	 */
	WARN_ON(mddev->events == 0);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	rdev_for_each(rdev, mddev) {
		if (rdev->badblocks.changed)
			any_badblocks_changed++;
		if (test_bit(Faulty, &rdev->flags))
			set_bit(FaultRecorded, &rdev->flags);
	}

	sync_sbs(mddev, nospares);
<<<<<<< HEAD
	spin_unlock_irq(&mddev->write_lock);
=======
	spin_unlock(&mddev->lock);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	pr_debug("md: updating %s RAID superblock on device (in sync %d)\n",
		 mdname(mddev), mddev->in_sync);

<<<<<<< HEAD
	bitmap_update_sb(mddev->bitmap);
	rdev_for_each(rdev, mddev) {
		char b[BDEVNAME_SIZE];

		if (rdev->sb_loaded != 1)
			continue; /* no noise on spare devices */

		if (!test_bit(Faulty, &rdev->flags) &&
		    rdev->saved_raid_disk == -1) {
			md_super_write(mddev,rdev,
				       rdev->sb_start, rdev->sb_size,
				       rdev->sb_page);
			pr_debug("md: (write) %s's sb offset: %llu\n",
				 bdevname(rdev->bdev, b),
=======
	mddev_add_trace_msg(mddev, "md md_update_sb");
rewrite:
	md_bitmap_update_sb(mddev->bitmap);
	rdev_for_each(rdev, mddev) {
		if (rdev->sb_loaded != 1)
			continue; /* no noise on spare devices */

		if (!test_bit(Faulty, &rdev->flags)) {
			md_super_write(mddev,rdev,
				       rdev->sb_start, rdev->sb_size,
				       rdev->sb_page);
			pr_debug("md: (write) %pg's sb offset: %llu\n",
				 rdev->bdev,
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
				 (unsigned long long)rdev->sb_start);
			rdev->sb_events = mddev->events;
			if (rdev->badblocks.size) {
				md_super_write(mddev, rdev,
					       rdev->badblocks.sector,
					       rdev->badblocks.size << 9,
					       rdev->bb_page);
				rdev->badblocks.size = 0;
			}

<<<<<<< HEAD
		} else if (test_bit(Faulty, &rdev->flags))
			pr_debug("md: %s (skipping faulty)\n",
				 bdevname(rdev->bdev, b));
		else
			pr_debug("(skipping incremental s/r ");

		if (mddev->level == LEVEL_MULTIPATH)
			/* only need to write one superblock... */
			break;
	}
	md_super_wait(mddev);
	/* if there was a failure, MD_CHANGE_DEVS was set, and we re-write super */

	spin_lock_irq(&mddev->write_lock);
	if (mddev->in_sync != sync_req ||
	    test_bit(MD_CHANGE_DEVS, &mddev->flags)) {
		/* have to write it out again */
		spin_unlock_irq(&mddev->write_lock);
		goto repeat;
	}
	clear_bit(MD_CHANGE_PENDING, &mddev->flags);
	spin_unlock_irq(&mddev->write_lock);
	wake_up(&mddev->sb_wait);
	if (test_bit(MD_RECOVERY_RUNNING, &mddev->recovery))
		sysfs_notify(&mddev->kobj, NULL, "sync_completed");
=======
		} else
			pr_debug("md: %pg (skipping faulty)\n",
				 rdev->bdev);
	}
	if (md_super_wait(mddev) < 0)
		goto rewrite;
	/* if there was a failure, MD_SB_CHANGE_DEVS was set, and we re-write super */

	if (mddev_is_clustered(mddev) && ret == 0)
		md_cluster_ops->metadata_update_finish(mddev);

	if (mddev->in_sync != sync_req ||
	    !bit_clear_unless(&mddev->sb_flags, BIT(MD_SB_CHANGE_PENDING),
			       BIT(MD_SB_CHANGE_DEVS) | BIT(MD_SB_CHANGE_CLEAN)))
		/* have to write it out again */
		goto repeat;
	wake_up(&mddev->sb_wait);
	if (test_bit(MD_RECOVERY_RUNNING, &mddev->recovery))
		sysfs_notify_dirent_safe(mddev->sysfs_completed);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	rdev_for_each(rdev, mddev) {
		if (test_and_clear_bit(FaultRecorded, &rdev->flags))
			clear_bit(Blocked, &rdev->flags);

		if (any_badblocks_changed)
<<<<<<< HEAD
			md_ack_all_badblocks(&rdev->badblocks);
=======
			ack_all_badblocks(&rdev->badblocks);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		clear_bit(BlockedBadBlocks, &rdev->flags);
		wake_up(&rdev->blocked_wait);
	}
}
<<<<<<< HEAD
=======
EXPORT_SYMBOL(md_update_sb);

static int add_bound_rdev(struct md_rdev *rdev)
{
	struct mddev *mddev = rdev->mddev;
	int err = 0;
	bool add_journal = test_bit(Journal, &rdev->flags);

	if (!mddev->pers->hot_remove_disk || add_journal) {
		/* If there is hot_add_disk but no hot_remove_disk
		 * then added disks for geometry changes,
		 * and should be added immediately.
		 */
		super_types[mddev->major_version].
			validate_super(mddev, NULL/*freshest*/, rdev);
		err = mddev->pers->hot_add_disk(mddev, rdev);
		if (err) {
			md_kick_rdev_from_array(rdev);
			return err;
		}
	}
	sysfs_notify_dirent_safe(rdev->sysfs_state);

	set_bit(MD_SB_CHANGE_DEVS, &mddev->sb_flags);
	if (mddev->degraded)
		set_bit(MD_RECOVERY_RECOVER, &mddev->recovery);
	set_bit(MD_RECOVERY_NEEDED, &mddev->recovery);
	md_new_event();
	return 0;
}
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

/* words written to sysfs files may, or may not, be \n terminated.
 * We want to accept with case. For this we use cmd_match.
 */
static int cmd_match(const char *cmd, const char *str)
{
	/* See if cmd, written into a sysfs file, matches
	 * str.  They must either be the same, or cmd can
	 * have a trailing newline
	 */
	while (*cmd && *str && *cmd == *str) {
		cmd++;
		str++;
	}
	if (*cmd == '\n')
		cmd++;
	if (*str || *cmd)
		return 0;
	return 1;
}

struct rdev_sysfs_entry {
	struct attribute attr;
	ssize_t (*show)(struct md_rdev *, char *);
	ssize_t (*store)(struct md_rdev *, const char *, size_t);
};

static ssize_t
state_show(struct md_rdev *rdev, char *page)
{
<<<<<<< HEAD
	char *sep = "";
	size_t len = 0;

	if (test_bit(Faulty, &rdev->flags) ||
	    rdev->badblocks.unacked_exist) {
		len+= sprintf(page+len, "%sfaulty",sep);
		sep = ",";
	}
	if (test_bit(In_sync, &rdev->flags)) {
		len += sprintf(page+len, "%sin_sync",sep);
		sep = ",";
	}
	if (test_bit(WriteMostly, &rdev->flags)) {
		len += sprintf(page+len, "%swrite_mostly",sep);
		sep = ",";
	}
	if (test_bit(Blocked, &rdev->flags) ||
	    (rdev->badblocks.unacked_exist
	     && !test_bit(Faulty, &rdev->flags))) {
		len += sprintf(page+len, "%sblocked", sep);
		sep = ",";
	}
	if (!test_bit(Faulty, &rdev->flags) &&
	    !test_bit(In_sync, &rdev->flags)) {
		len += sprintf(page+len, "%sspare", sep);
		sep = ",";
	}
	if (test_bit(WriteErrorSeen, &rdev->flags)) {
		len += sprintf(page+len, "%swrite_error", sep);
		sep = ",";
	}
	if (test_bit(WantReplacement, &rdev->flags)) {
		len += sprintf(page+len, "%swant_replacement", sep);
		sep = ",";
	}
	if (test_bit(Replacement, &rdev->flags)) {
		len += sprintf(page+len, "%sreplacement", sep);
		sep = ",";
	}
=======
	char *sep = ",";
	size_t len = 0;
	unsigned long flags = READ_ONCE(rdev->flags);

	if (test_bit(Faulty, &flags) ||
	    (!test_bit(ExternalBbl, &flags) &&
	    rdev->badblocks.unacked_exist))
		len += sprintf(page+len, "faulty%s", sep);
	if (test_bit(In_sync, &flags))
		len += sprintf(page+len, "in_sync%s", sep);
	if (test_bit(Journal, &flags))
		len += sprintf(page+len, "journal%s", sep);
	if (test_bit(WriteMostly, &flags))
		len += sprintf(page+len, "write_mostly%s", sep);
	if (test_bit(Blocked, &flags) ||
	    (rdev->badblocks.unacked_exist
	     && !test_bit(Faulty, &flags)))
		len += sprintf(page+len, "blocked%s", sep);
	if (!test_bit(Faulty, &flags) &&
	    !test_bit(Journal, &flags) &&
	    !test_bit(In_sync, &flags))
		len += sprintf(page+len, "spare%s", sep);
	if (test_bit(WriteErrorSeen, &flags))
		len += sprintf(page+len, "write_error%s", sep);
	if (test_bit(WantReplacement, &flags))
		len += sprintf(page+len, "want_replacement%s", sep);
	if (test_bit(Replacement, &flags))
		len += sprintf(page+len, "replacement%s", sep);
	if (test_bit(ExternalBbl, &flags))
		len += sprintf(page+len, "external_bbl%s", sep);
	if (test_bit(FailFast, &flags))
		len += sprintf(page+len, "failfast%s", sep);

	if (len)
		len -= strlen(sep);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	return len+sprintf(page+len, "\n");
}

static ssize_t
state_store(struct md_rdev *rdev, const char *buf, size_t len)
{
	/* can write
	 *  faulty  - simulates an error
	 *  remove  - disconnects the device
	 *  writemostly - sets write_mostly
	 *  -writemostly - clears write_mostly
	 *  blocked - sets the Blocked flags
	 *  -blocked - clears the Blocked and possibly simulates an error
	 *  insync - sets Insync providing device isn't active
<<<<<<< HEAD
	 *  write_error - sets WriteErrorSeen
	 *  -write_error - clears WriteErrorSeen
	 */
	int err = -EINVAL;
	if (cmd_match(buf, "faulty") && rdev->mddev->pers) {
		md_error(rdev->mddev, rdev);
		if (test_bit(Faulty, &rdev->flags))
			err = 0;
		else
			err = -EBUSY;
	} else if (cmd_match(buf, "remove")) {
		if (rdev->raid_disk >= 0)
			err = -EBUSY;
		else {
			struct mddev *mddev = rdev->mddev;
			kick_rdev_from_array(rdev);
			if (mddev->pers)
				md_update_sb(mddev, 1);
			md_new_event(mddev);
			err = 0;
		}
	} else if (cmd_match(buf, "writemostly")) {
		set_bit(WriteMostly, &rdev->flags);
		err = 0;
	} else if (cmd_match(buf, "-writemostly")) {
		clear_bit(WriteMostly, &rdev->flags);
=======
	 *  -insync - clear Insync for a device with a slot assigned,
	 *            so that it gets rebuilt based on bitmap
	 *  write_error - sets WriteErrorSeen
	 *  -write_error - clears WriteErrorSeen
	 *  {,-}failfast - set/clear FailFast
	 */

	struct mddev *mddev = rdev->mddev;
	int err = -EINVAL;
	bool need_update_sb = false;

	if (cmd_match(buf, "faulty") && rdev->mddev->pers) {
		md_error(rdev->mddev, rdev);

		if (test_bit(MD_BROKEN, &rdev->mddev->flags))
			err = -EBUSY;
		else
			err = 0;
	} else if (cmd_match(buf, "remove")) {
		if (rdev->mddev->pers) {
			clear_bit(Blocked, &rdev->flags);
			remove_and_add_spares(rdev->mddev, rdev);
		}
		if (rdev->raid_disk >= 0)
			err = -EBUSY;
		else {
			err = 0;
			if (mddev_is_clustered(mddev))
				err = md_cluster_ops->remove_disk(mddev, rdev);

			if (err == 0) {
				md_kick_rdev_from_array(rdev);
				if (mddev->pers)
					set_bit(MD_SB_CHANGE_DEVS, &mddev->sb_flags);
				md_new_event();
			}
		}
	} else if (cmd_match(buf, "writemostly")) {
		set_bit(WriteMostly, &rdev->flags);
		mddev_create_serial_pool(rdev->mddev, rdev);
		need_update_sb = true;
		err = 0;
	} else if (cmd_match(buf, "-writemostly")) {
		mddev_destroy_serial_pool(rdev->mddev, rdev);
		clear_bit(WriteMostly, &rdev->flags);
		need_update_sb = true;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		err = 0;
	} else if (cmd_match(buf, "blocked")) {
		set_bit(Blocked, &rdev->flags);
		err = 0;
	} else if (cmd_match(buf, "-blocked")) {
		if (!test_bit(Faulty, &rdev->flags) &&
<<<<<<< HEAD
=======
		    !test_bit(ExternalBbl, &rdev->flags) &&
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		    rdev->badblocks.unacked_exist) {
			/* metadata handler doesn't understand badblocks,
			 * so we need to fail the device
			 */
			md_error(rdev->mddev, rdev);
		}
		clear_bit(Blocked, &rdev->flags);
		clear_bit(BlockedBadBlocks, &rdev->flags);
		wake_up(&rdev->blocked_wait);
		set_bit(MD_RECOVERY_NEEDED, &rdev->mddev->recovery);
<<<<<<< HEAD
		md_wakeup_thread(rdev->mddev->thread);
=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

		err = 0;
	} else if (cmd_match(buf, "insync") && rdev->raid_disk == -1) {
		set_bit(In_sync, &rdev->flags);
		err = 0;
<<<<<<< HEAD
=======
	} else if (cmd_match(buf, "failfast")) {
		set_bit(FailFast, &rdev->flags);
		need_update_sb = true;
		err = 0;
	} else if (cmd_match(buf, "-failfast")) {
		clear_bit(FailFast, &rdev->flags);
		need_update_sb = true;
		err = 0;
	} else if (cmd_match(buf, "-insync") && rdev->raid_disk >= 0 &&
		   !test_bit(Journal, &rdev->flags)) {
		if (rdev->mddev->pers == NULL) {
			clear_bit(In_sync, &rdev->flags);
			rdev->saved_raid_disk = rdev->raid_disk;
			rdev->raid_disk = -1;
			err = 0;
		}
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	} else if (cmd_match(buf, "write_error")) {
		set_bit(WriteErrorSeen, &rdev->flags);
		err = 0;
	} else if (cmd_match(buf, "-write_error")) {
		clear_bit(WriteErrorSeen, &rdev->flags);
		err = 0;
	} else if (cmd_match(buf, "want_replacement")) {
		/* Any non-spare device that is not a replacement can
		 * become want_replacement at any time, but we then need to
		 * check if recovery is needed.
		 */
		if (rdev->raid_disk >= 0 &&
<<<<<<< HEAD
		    !test_bit(Replacement, &rdev->flags))
			set_bit(WantReplacement, &rdev->flags);
		set_bit(MD_RECOVERY_NEEDED, &rdev->mddev->recovery);
		md_wakeup_thread(rdev->mddev->thread);
=======
		    !test_bit(Journal, &rdev->flags) &&
		    !test_bit(Replacement, &rdev->flags))
			set_bit(WantReplacement, &rdev->flags);
		set_bit(MD_RECOVERY_NEEDED, &rdev->mddev->recovery);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		err = 0;
	} else if (cmd_match(buf, "-want_replacement")) {
		/* Clearing 'want_replacement' is always allowed.
		 * Once replacements starts it is too late though.
		 */
		err = 0;
		clear_bit(WantReplacement, &rdev->flags);
	} else if (cmd_match(buf, "replacement")) {
		/* Can only set a device as a replacement when array has not
		 * yet been started.  Once running, replacement is automatic
		 * from spares, or by assigning 'slot'.
		 */
		if (rdev->mddev->pers)
			err = -EBUSY;
		else {
			set_bit(Replacement, &rdev->flags);
			err = 0;
		}
	} else if (cmd_match(buf, "-replacement")) {
		/* Similarly, can only clear Replacement before start */
		if (rdev->mddev->pers)
			err = -EBUSY;
		else {
			clear_bit(Replacement, &rdev->flags);
			err = 0;
		}
<<<<<<< HEAD
	}
=======
	} else if (cmd_match(buf, "re-add")) {
		if (!rdev->mddev->pers)
			err = -EINVAL;
		else if (test_bit(Faulty, &rdev->flags) && (rdev->raid_disk == -1) &&
				rdev->saved_raid_disk >= 0) {
			/* clear_bit is performed _after_ all the devices
			 * have their local Faulty bit cleared. If any writes
			 * happen in the meantime in the local node, they
			 * will land in the local bitmap, which will be synced
			 * by this node eventually
			 */
			if (!mddev_is_clustered(rdev->mddev) ||
			    (err = md_cluster_ops->gather_bitmaps(rdev)) == 0) {
				clear_bit(Faulty, &rdev->flags);
				err = add_bound_rdev(rdev);
			}
		} else
			err = -EBUSY;
	} else if (cmd_match(buf, "external_bbl") && (rdev->mddev->external)) {
		set_bit(ExternalBbl, &rdev->flags);
		rdev->badblocks.shift = 0;
		err = 0;
	} else if (cmd_match(buf, "-external_bbl") && (rdev->mddev->external)) {
		clear_bit(ExternalBbl, &rdev->flags);
		err = 0;
	}
	if (need_update_sb)
		md_update_sb(mddev, 1);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	if (!err)
		sysfs_notify_dirent_safe(rdev->sysfs_state);
	return err ? err : len;
}
static struct rdev_sysfs_entry rdev_state =
<<<<<<< HEAD
__ATTR(state, S_IRUGO|S_IWUSR, state_show, state_store);
=======
__ATTR_PREALLOC(state, S_IRUGO|S_IWUSR, state_show, state_store);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

static ssize_t
errors_show(struct md_rdev *rdev, char *page)
{
	return sprintf(page, "%d\n", atomic_read(&rdev->corrected_errors));
}

static ssize_t
errors_store(struct md_rdev *rdev, const char *buf, size_t len)
{
<<<<<<< HEAD
	char *e;
	unsigned long n = simple_strtoul(buf, &e, 10);
	if (*buf && (*e == 0 || *e == '\n')) {
		atomic_set(&rdev->corrected_errors, n);
		return len;
	}
	return -EINVAL;
=======
	unsigned int n;
	int rv;

	rv = kstrtouint(buf, 10, &n);
	if (rv < 0)
		return rv;
	atomic_set(&rdev->corrected_errors, n);
	return len;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}
static struct rdev_sysfs_entry rdev_errors =
__ATTR(errors, S_IRUGO|S_IWUSR, errors_show, errors_store);

static ssize_t
slot_show(struct md_rdev *rdev, char *page)
{
<<<<<<< HEAD
	if (rdev->raid_disk < 0)
=======
	if (test_bit(Journal, &rdev->flags))
		return sprintf(page, "journal\n");
	else if (rdev->raid_disk < 0)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		return sprintf(page, "none\n");
	else
		return sprintf(page, "%d\n", rdev->raid_disk);
}

static ssize_t
slot_store(struct md_rdev *rdev, const char *buf, size_t len)
{
<<<<<<< HEAD
	char *e;
	int err;
	int slot = simple_strtoul(buf, &e, 10);
	if (strncmp(buf, "none", 4)==0)
		slot = -1;
	else if (e==buf || (*e && *e!= '\n'))
		return -EINVAL;
=======
	int slot;
	int err;

	if (test_bit(Journal, &rdev->flags))
		return -EBUSY;
	if (strncmp(buf, "none", 4)==0)
		slot = -1;
	else {
		err = kstrtouint(buf, 10, (unsigned int *)&slot);
		if (err < 0)
			return err;
		if (slot < 0)
			/* overflow */
			return -ENOSPC;
	}
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	if (rdev->mddev->pers && slot == -1) {
		/* Setting 'slot' on an active array requires also
		 * updating the 'rd%d' link, and communicating
		 * with the personality with ->hot_*_disk.
		 * For now we only support removing
		 * failed/spare devices.  This normally happens automatically,
		 * but not when the metadata is externally managed.
		 */
		if (rdev->raid_disk == -1)
			return -EEXIST;
		/* personality does all needed checks */
		if (rdev->mddev->pers->hot_remove_disk == NULL)
			return -EINVAL;
<<<<<<< HEAD
		err = rdev->mddev->pers->
			hot_remove_disk(rdev->mddev, rdev);
		if (err)
			return err;
		sysfs_unlink_rdev(rdev->mddev, rdev);
		rdev->raid_disk = -1;
		set_bit(MD_RECOVERY_NEEDED, &rdev->mddev->recovery);
		md_wakeup_thread(rdev->mddev->thread);
=======
		clear_bit(Blocked, &rdev->flags);
		remove_and_add_spares(rdev->mddev, rdev);
		if (rdev->raid_disk >= 0)
			return -EBUSY;
		set_bit(MD_RECOVERY_NEEDED, &rdev->mddev->recovery);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	} else if (rdev->mddev->pers) {
		/* Activating a spare .. or possibly reactivating
		 * if we ever get bitmaps working here.
		 */
<<<<<<< HEAD
=======
		int err;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

		if (rdev->raid_disk != -1)
			return -EBUSY;

		if (test_bit(MD_RECOVERY_RUNNING, &rdev->mddev->recovery))
			return -EBUSY;

		if (rdev->mddev->pers->hot_add_disk == NULL)
			return -EINVAL;

		if (slot >= rdev->mddev->raid_disks &&
		    slot >= rdev->mddev->raid_disks + rdev->mddev->delta_disks)
			return -ENOSPC;

		rdev->raid_disk = slot;
		if (test_bit(In_sync, &rdev->flags))
			rdev->saved_raid_disk = slot;
		else
			rdev->saved_raid_disk = -1;
		clear_bit(In_sync, &rdev->flags);
<<<<<<< HEAD
		err = rdev->mddev->pers->
			hot_add_disk(rdev->mddev, rdev);
=======
		clear_bit(Bitmap_sync, &rdev->flags);
		err = rdev->mddev->pers->hot_add_disk(rdev->mddev, rdev);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		if (err) {
			rdev->raid_disk = -1;
			return err;
		} else
			sysfs_notify_dirent_safe(rdev->sysfs_state);
<<<<<<< HEAD
		if (sysfs_link_rdev(rdev->mddev, rdev))
			/* failure here is OK */;
=======
		/* failure here is OK */;
		sysfs_link_rdev(rdev->mddev, rdev);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		/* don't wakeup anyone, leave that to userspace. */
	} else {
		if (slot >= rdev->mddev->raid_disks &&
		    slot >= rdev->mddev->raid_disks + rdev->mddev->delta_disks)
			return -ENOSPC;
		rdev->raid_disk = slot;
		/* assume it is working */
		clear_bit(Faulty, &rdev->flags);
		clear_bit(WriteMostly, &rdev->flags);
		set_bit(In_sync, &rdev->flags);
		sysfs_notify_dirent_safe(rdev->sysfs_state);
	}
	return len;
}

<<<<<<< HEAD

=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
static struct rdev_sysfs_entry rdev_slot =
__ATTR(slot, S_IRUGO|S_IWUSR, slot_show, slot_store);

static ssize_t
offset_show(struct md_rdev *rdev, char *page)
{
	return sprintf(page, "%llu\n", (unsigned long long)rdev->data_offset);
}

static ssize_t
offset_store(struct md_rdev *rdev, const char *buf, size_t len)
{
<<<<<<< HEAD
	char *e;
	unsigned long long offset = simple_strtoull(buf, &e, 10);
	if (e==buf || (*e && *e != '\n'))
=======
	unsigned long long offset;
	if (kstrtoull(buf, 10, &offset) < 0)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		return -EINVAL;
	if (rdev->mddev->pers && rdev->raid_disk >= 0)
		return -EBUSY;
	if (rdev->sectors && rdev->mddev->external)
		/* Must set offset before size, so overlap checks
		 * can be sane */
		return -EBUSY;
	rdev->data_offset = offset;
<<<<<<< HEAD
=======
	rdev->new_data_offset = offset;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	return len;
}

static struct rdev_sysfs_entry rdev_offset =
__ATTR(offset, S_IRUGO|S_IWUSR, offset_show, offset_store);

<<<<<<< HEAD
=======
static ssize_t new_offset_show(struct md_rdev *rdev, char *page)
{
	return sprintf(page, "%llu\n",
		       (unsigned long long)rdev->new_data_offset);
}

static ssize_t new_offset_store(struct md_rdev *rdev,
				const char *buf, size_t len)
{
	unsigned long long new_offset;
	struct mddev *mddev = rdev->mddev;

	if (kstrtoull(buf, 10, &new_offset) < 0)
		return -EINVAL;

	if (test_bit(MD_RECOVERY_RUNNING, &mddev->recovery))
		return -EBUSY;
	if (new_offset == rdev->data_offset)
		/* reset is always permitted */
		;
	else if (new_offset > rdev->data_offset) {
		/* must not push array size beyond rdev_sectors */
		if (new_offset - rdev->data_offset
		    + mddev->dev_sectors > rdev->sectors)
				return -E2BIG;
	}
	/* Metadata worries about other space details. */

	/* decreasing the offset is inconsistent with a backwards
	 * reshape.
	 */
	if (new_offset < rdev->data_offset &&
	    mddev->reshape_backwards)
		return -EINVAL;
	/* Increasing offset is inconsistent with forwards
	 * reshape.  reshape_direction should be set to
	 * 'backwards' first.
	 */
	if (new_offset > rdev->data_offset &&
	    !mddev->reshape_backwards)
		return -EINVAL;

	if (mddev->pers && mddev->persistent &&
	    !super_types[mddev->major_version]
	    .allow_new_offset(rdev, new_offset))
		return -E2BIG;
	rdev->new_data_offset = new_offset;
	if (new_offset > rdev->data_offset)
		mddev->reshape_backwards = 1;
	else if (new_offset < rdev->data_offset)
		mddev->reshape_backwards = 0;

	return len;
}
static struct rdev_sysfs_entry rdev_new_offset =
__ATTR(new_offset, S_IRUGO|S_IWUSR, new_offset_show, new_offset_store);

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
static ssize_t
rdev_size_show(struct md_rdev *rdev, char *page)
{
	return sprintf(page, "%llu\n", (unsigned long long)rdev->sectors / 2);
}

<<<<<<< HEAD
static int overlaps(sector_t s1, sector_t l1, sector_t s2, sector_t l2)
{
	/* check if two start/length pairs overlap */
	if (s1+l1 <= s2)
		return 0;
	if (s2+l2 <= s1)
		return 0;
	return 1;
=======
static int md_rdevs_overlap(struct md_rdev *a, struct md_rdev *b)
{
	/* check if two start/length pairs overlap */
	if (a->data_offset + a->sectors <= b->data_offset)
		return false;
	if (b->data_offset + b->sectors <= a->data_offset)
		return false;
	return true;
}

static bool md_rdev_overlaps(struct md_rdev *rdev)
{
	struct mddev *mddev;
	struct md_rdev *rdev2;

	spin_lock(&all_mddevs_lock);
	list_for_each_entry(mddev, &all_mddevs, all_mddevs) {
		if (test_bit(MD_DELETED, &mddev->flags))
			continue;
		rdev_for_each(rdev2, mddev) {
			if (rdev != rdev2 && rdev->bdev == rdev2->bdev &&
			    md_rdevs_overlap(rdev, rdev2)) {
				spin_unlock(&all_mddevs_lock);
				return true;
			}
		}
	}
	spin_unlock(&all_mddevs_lock);
	return false;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

static int strict_blocks_to_sectors(const char *buf, sector_t *sectors)
{
	unsigned long long blocks;
	sector_t new;

<<<<<<< HEAD
	if (strict_strtoull(buf, 10, &blocks) < 0)
=======
	if (kstrtoull(buf, 10, &blocks) < 0)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		return -EINVAL;

	if (blocks & 1ULL << (8 * sizeof(blocks) - 1))
		return -EINVAL; /* sector conversion overflow */

	new = blocks * 2;
	if (new != blocks * 2)
		return -EINVAL; /* unsigned long long to sector_t overflow */

	*sectors = new;
	return 0;
}

static ssize_t
rdev_size_store(struct md_rdev *rdev, const char *buf, size_t len)
{
	struct mddev *my_mddev = rdev->mddev;
	sector_t oldsectors = rdev->sectors;
	sector_t sectors;

<<<<<<< HEAD
	if (strict_blocks_to_sectors(buf, &sectors) < 0)
		return -EINVAL;
=======
	if (test_bit(Journal, &rdev->flags))
		return -EBUSY;
	if (strict_blocks_to_sectors(buf, &sectors) < 0)
		return -EINVAL;
	if (rdev->data_offset != rdev->new_data_offset)
		return -EINVAL; /* too confusing */
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	if (my_mddev->pers && rdev->raid_disk >= 0) {
		if (my_mddev->persistent) {
			sectors = super_types[my_mddev->major_version].
				rdev_size_change(rdev, sectors);
			if (!sectors)
				return -EBUSY;
		} else if (!sectors)
<<<<<<< HEAD
			sectors = (i_size_read(rdev->bdev->bd_inode) >> 9) -
=======
			sectors = bdev_nr_sectors(rdev->bdev) -
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
				rdev->data_offset;
		if (!my_mddev->pers->resize)
			/* Cannot change size for RAID0 or Linear etc */
			return -EINVAL;
	}
	if (sectors < my_mddev->dev_sectors)
		return -EINVAL; /* component must fit device */

	rdev->sectors = sectors;
<<<<<<< HEAD
	if (sectors > oldsectors && my_mddev->external) {
		/* need to check that all other rdevs with the same ->bdev
		 * do not overlap.  We need to unlock the mddev to avoid
		 * a deadlock.  We have already changed rdev->sectors, and if
		 * we have to change it back, we will have the lock again.
		 */
		struct mddev *mddev;
		int overlap = 0;
		struct list_head *tmp;

		mddev_unlock(my_mddev);
		for_each_mddev(mddev, tmp) {
			struct md_rdev *rdev2;

			mddev_lock(mddev);
			rdev_for_each(rdev2, mddev)
				if (rdev->bdev == rdev2->bdev &&
				    rdev != rdev2 &&
				    overlaps(rdev->data_offset, rdev->sectors,
					     rdev2->data_offset,
					     rdev2->sectors)) {
					overlap = 1;
					break;
				}
			mddev_unlock(mddev);
			if (overlap) {
				mddev_put(mddev);
				break;
			}
		}
		mddev_lock(my_mddev);
		if (overlap) {
			/* Someone else could have slipped in a size
			 * change here, but doing so is just silly.
			 * We put oldsectors back because we *know* it is
			 * safe, and trust userspace not to race with
			 * itself
			 */
			rdev->sectors = oldsectors;
			return -EBUSY;
		}
=======

	/*
	 * Check that all other rdevs with the same bdev do not overlap.  This
	 * check does not provide a hard guarantee, it just helps avoid
	 * dangerous mistakes.
	 */
	if (sectors > oldsectors && my_mddev->external &&
	    md_rdev_overlaps(rdev)) {
		/*
		 * Someone else could have slipped in a size change here, but
		 * doing so is just silly.  We put oldsectors back because we
		 * know it is safe, and trust userspace not to race with itself.
		 */
		rdev->sectors = oldsectors;
		return -EBUSY;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	}
	return len;
}

static struct rdev_sysfs_entry rdev_size =
__ATTR(size, S_IRUGO|S_IWUSR, rdev_size_show, rdev_size_store);

<<<<<<< HEAD

=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
static ssize_t recovery_start_show(struct md_rdev *rdev, char *page)
{
	unsigned long long recovery_start = rdev->recovery_offset;

	if (test_bit(In_sync, &rdev->flags) ||
	    recovery_start == MaxSector)
		return sprintf(page, "none\n");

	return sprintf(page, "%llu\n", recovery_start);
}

static ssize_t recovery_start_store(struct md_rdev *rdev, const char *buf, size_t len)
{
	unsigned long long recovery_start;

	if (cmd_match(buf, "none"))
		recovery_start = MaxSector;
<<<<<<< HEAD
	else if (strict_strtoull(buf, 10, &recovery_start))
=======
	else if (kstrtoull(buf, 10, &recovery_start))
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		return -EINVAL;

	if (rdev->mddev->pers &&
	    rdev->raid_disk >= 0)
		return -EBUSY;

	rdev->recovery_offset = recovery_start;
	if (recovery_start == MaxSector)
		set_bit(In_sync, &rdev->flags);
	else
		clear_bit(In_sync, &rdev->flags);
	return len;
}

static struct rdev_sysfs_entry rdev_recovery_start =
__ATTR(recovery_start, S_IRUGO|S_IWUSR, recovery_start_show, recovery_start_store);

<<<<<<< HEAD

static ssize_t
badblocks_show(struct badblocks *bb, char *page, int unack);
static ssize_t
badblocks_store(struct badblocks *bb, const char *page, size_t len, int unack);

=======
/* sysfs access to bad-blocks list.
 * We present two files.
 * 'bad-blocks' lists sector numbers and lengths of ranges that
 *    are recorded as bad.  The list is truncated to fit within
 *    the one-page limit of sysfs.
 *    Writing "sector length" to this file adds an acknowledged
 *    bad block list.
 * 'unacknowledged-bad-blocks' lists bad blocks that have not yet
 *    been acknowledged.  Writing to this file adds bad blocks
 *    without acknowledging them.  This is largely for testing.
 */
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
static ssize_t bb_show(struct md_rdev *rdev, char *page)
{
	return badblocks_show(&rdev->badblocks, page, 0);
}
static ssize_t bb_store(struct md_rdev *rdev, const char *page, size_t len)
{
	int rv = badblocks_store(&rdev->badblocks, page, len, 0);
	/* Maybe that ack was all we needed */
	if (test_and_clear_bit(BlockedBadBlocks, &rdev->flags))
		wake_up(&rdev->blocked_wait);
	return rv;
}
static struct rdev_sysfs_entry rdev_bad_blocks =
__ATTR(bad_blocks, S_IRUGO|S_IWUSR, bb_show, bb_store);

<<<<<<< HEAD

=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
static ssize_t ubb_show(struct md_rdev *rdev, char *page)
{
	return badblocks_show(&rdev->badblocks, page, 1);
}
static ssize_t ubb_store(struct md_rdev *rdev, const char *page, size_t len)
{
	return badblocks_store(&rdev->badblocks, page, len, 1);
}
static struct rdev_sysfs_entry rdev_unack_bad_blocks =
__ATTR(unacknowledged_bad_blocks, S_IRUGO|S_IWUSR, ubb_show, ubb_store);

<<<<<<< HEAD
=======
static ssize_t
ppl_sector_show(struct md_rdev *rdev, char *page)
{
	return sprintf(page, "%llu\n", (unsigned long long)rdev->ppl.sector);
}

static ssize_t
ppl_sector_store(struct md_rdev *rdev, const char *buf, size_t len)
{
	unsigned long long sector;

	if (kstrtoull(buf, 10, &sector) < 0)
		return -EINVAL;
	if (sector != (sector_t)sector)
		return -EINVAL;

	if (rdev->mddev->pers && test_bit(MD_HAS_PPL, &rdev->mddev->flags) &&
	    rdev->raid_disk >= 0)
		return -EBUSY;

	if (rdev->mddev->persistent) {
		if (rdev->mddev->major_version == 0)
			return -EINVAL;
		if ((sector > rdev->sb_start &&
		     sector - rdev->sb_start > S16_MAX) ||
		    (sector < rdev->sb_start &&
		     rdev->sb_start - sector > -S16_MIN))
			return -EINVAL;
		rdev->ppl.offset = sector - rdev->sb_start;
	} else if (!rdev->mddev->external) {
		return -EBUSY;
	}
	rdev->ppl.sector = sector;
	return len;
}

static struct rdev_sysfs_entry rdev_ppl_sector =
__ATTR(ppl_sector, S_IRUGO|S_IWUSR, ppl_sector_show, ppl_sector_store);

static ssize_t
ppl_size_show(struct md_rdev *rdev, char *page)
{
	return sprintf(page, "%u\n", rdev->ppl.size);
}

static ssize_t
ppl_size_store(struct md_rdev *rdev, const char *buf, size_t len)
{
	unsigned int size;

	if (kstrtouint(buf, 10, &size) < 0)
		return -EINVAL;

	if (rdev->mddev->pers && test_bit(MD_HAS_PPL, &rdev->mddev->flags) &&
	    rdev->raid_disk >= 0)
		return -EBUSY;

	if (rdev->mddev->persistent) {
		if (rdev->mddev->major_version == 0)
			return -EINVAL;
		if (size > U16_MAX)
			return -EINVAL;
	} else if (!rdev->mddev->external) {
		return -EBUSY;
	}
	rdev->ppl.size = size;
	return len;
}

static struct rdev_sysfs_entry rdev_ppl_size =
__ATTR(ppl_size, S_IRUGO|S_IWUSR, ppl_size_show, ppl_size_store);

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
static struct attribute *rdev_default_attrs[] = {
	&rdev_state.attr,
	&rdev_errors.attr,
	&rdev_slot.attr,
	&rdev_offset.attr,
<<<<<<< HEAD
=======
	&rdev_new_offset.attr,
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	&rdev_size.attr,
	&rdev_recovery_start.attr,
	&rdev_bad_blocks.attr,
	&rdev_unack_bad_blocks.attr,
<<<<<<< HEAD
	NULL,
};
=======
	&rdev_ppl_sector.attr,
	&rdev_ppl_size.attr,
	NULL,
};
ATTRIBUTE_GROUPS(rdev_default);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
static ssize_t
rdev_attr_show(struct kobject *kobj, struct attribute *attr, char *page)
{
	struct rdev_sysfs_entry *entry = container_of(attr, struct rdev_sysfs_entry, attr);
	struct md_rdev *rdev = container_of(kobj, struct md_rdev, kobj);
<<<<<<< HEAD
	struct mddev *mddev = rdev->mddev;
	ssize_t rv;

	if (!entry->show)
		return -EIO;

	rv = mddev ? mddev_lock(mddev) : -EBUSY;
	if (!rv) {
		if (rdev->mddev == NULL)
			rv = -EBUSY;
		else
			rv = entry->show(rdev, page);
		mddev_unlock(mddev);
	}
	return rv;
=======

	if (!entry->show)
		return -EIO;
	if (!rdev->mddev)
		return -ENODEV;
	return entry->show(rdev, page);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

static ssize_t
rdev_attr_store(struct kobject *kobj, struct attribute *attr,
	      const char *page, size_t length)
{
	struct rdev_sysfs_entry *entry = container_of(attr, struct rdev_sysfs_entry, attr);
	struct md_rdev *rdev = container_of(kobj, struct md_rdev, kobj);
<<<<<<< HEAD
	ssize_t rv;
	struct mddev *mddev = rdev->mddev;
=======
	struct kernfs_node *kn = NULL;
	bool suspend = false;
	ssize_t rv;
	struct mddev *mddev = READ_ONCE(rdev->mddev);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	if (!entry->store)
		return -EIO;
	if (!capable(CAP_SYS_ADMIN))
		return -EACCES;
<<<<<<< HEAD
	rv = mddev ? mddev_lock(mddev): -EBUSY;
	if (!rv) {
		if (rdev->mddev == NULL)
			rv = -EBUSY;
		else
			rv = entry->store(rdev, page, length);
		mddev_unlock(mddev);
	}
=======
	if (!mddev)
		return -ENODEV;

	if (entry->store == state_store) {
		if (cmd_match(page, "remove"))
			kn = sysfs_break_active_protection(kobj, attr);
		if (cmd_match(page, "remove") || cmd_match(page, "re-add") ||
		    cmd_match(page, "writemostly") ||
		    cmd_match(page, "-writemostly"))
			suspend = true;
	}

	rv = suspend ? mddev_suspend_and_lock(mddev) : mddev_lock(mddev);
	if (!rv) {
		if (rdev->mddev == NULL)
			rv = -ENODEV;
		else
			rv = entry->store(rdev, page, length);
		suspend ? mddev_unlock_and_resume(mddev) : mddev_unlock(mddev);
	}

	if (kn)
		sysfs_unbreak_active_protection(kn);

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	return rv;
}

static void rdev_free(struct kobject *ko)
{
	struct md_rdev *rdev = container_of(ko, struct md_rdev, kobj);
	kfree(rdev);
}
static const struct sysfs_ops rdev_sysfs_ops = {
	.show		= rdev_attr_show,
	.store		= rdev_attr_store,
};
<<<<<<< HEAD
static struct kobj_type rdev_ktype = {
	.release	= rdev_free,
	.sysfs_ops	= &rdev_sysfs_ops,
	.default_attrs	= rdev_default_attrs,
=======
static const struct kobj_type rdev_ktype = {
	.release	= rdev_free,
	.sysfs_ops	= &rdev_sysfs_ops,
	.default_groups	= rdev_default_groups,
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
};

int md_rdev_init(struct md_rdev *rdev)
{
	rdev->desc_nr = -1;
	rdev->saved_raid_disk = -1;
	rdev->raid_disk = -1;
	rdev->flags = 0;
	rdev->data_offset = 0;
<<<<<<< HEAD
	rdev->sb_events = 0;
	rdev->last_read_error.tv_sec  = 0;
	rdev->last_read_error.tv_nsec = 0;
=======
	rdev->new_data_offset = 0;
	rdev->sb_events = 0;
	rdev->last_read_error = 0;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	rdev->sb_loaded = 0;
	rdev->bb_page = NULL;
	atomic_set(&rdev->nr_pending, 0);
	atomic_set(&rdev->read_errors, 0);
	atomic_set(&rdev->corrected_errors, 0);

	INIT_LIST_HEAD(&rdev->same_set);
	init_waitqueue_head(&rdev->blocked_wait);

	/* Add space to store bad block list.
	 * This reserves the space even on arrays where it cannot
	 * be used - I wonder if that matters
	 */
<<<<<<< HEAD
	rdev->badblocks.count = 0;
	rdev->badblocks.shift = -1; /* disabled until explicitly enabled */
	rdev->badblocks.page = kmalloc(PAGE_SIZE, GFP_KERNEL);
	seqlock_init(&rdev->badblocks.lock);
	if (rdev->badblocks.page == NULL)
		return -ENOMEM;

	return 0;
}
EXPORT_SYMBOL_GPL(md_rdev_init);
=======
	return badblocks_init(&rdev->badblocks, 0);
}
EXPORT_SYMBOL_GPL(md_rdev_init);

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
/*
 * Import a device. If 'super_format' >= 0, then sanity check the superblock
 *
 * mark the device faulty if:
 *
 *   - the device is nonexistent (zero size)
 *   - the device has no valid superblock
 *
 * a faulty rdev _never_ has rdev->sb set.
 */
static struct md_rdev *md_import_device(dev_t newdev, int super_format, int super_minor)
{
<<<<<<< HEAD
	char b[BDEVNAME_SIZE];
	int err;
	struct md_rdev *rdev;
	sector_t size;

	rdev = kzalloc(sizeof(*rdev), GFP_KERNEL);
	if (!rdev) {
		printk(KERN_ERR "md: could not alloc mem for new device!\n");
		return ERR_PTR(-ENOMEM);
	}

	err = md_rdev_init(rdev);
	if (err)
		goto abort_free;
	err = alloc_disk_sb(rdev);
	if (err)
		goto abort_free;

	err = lock_rdev(rdev, newdev, super_format == -2);
	if (err)
		goto abort_free;

	kobject_init(&rdev->kobj, &rdev_ktype);

	size = i_size_read(rdev->bdev->bd_inode) >> BLOCK_SIZE_BITS;
	if (!size) {
		printk(KERN_WARNING 
			"md: %s has zero or unknown size, marking faulty!\n",
			bdevname(rdev->bdev,b));
		err = -EINVAL;
		goto abort_free;
=======
	struct md_rdev *rdev;
	sector_t size;
	int err;

	rdev = kzalloc(sizeof(*rdev), GFP_KERNEL);
	if (!rdev)
		return ERR_PTR(-ENOMEM);

	err = md_rdev_init(rdev);
	if (err)
		goto out_free_rdev;
	err = alloc_disk_sb(rdev);
	if (err)
		goto out_clear_rdev;

	rdev->bdev_file = bdev_file_open_by_dev(newdev,
			BLK_OPEN_READ | BLK_OPEN_WRITE,
			super_format == -2 ? &claim_rdev : rdev, NULL);
	if (IS_ERR(rdev->bdev_file)) {
		pr_warn("md: could not open device unknown-block(%u,%u).\n",
			MAJOR(newdev), MINOR(newdev));
		err = PTR_ERR(rdev->bdev_file);
		goto out_clear_rdev;
	}
	rdev->bdev = file_bdev(rdev->bdev_file);

	kobject_init(&rdev->kobj, &rdev_ktype);

	size = bdev_nr_bytes(rdev->bdev) >> BLOCK_SIZE_BITS;
	if (!size) {
		pr_warn("md: %pg has zero or unknown size, marking faulty!\n",
			rdev->bdev);
		err = -EINVAL;
		goto out_blkdev_put;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	}

	if (super_format >= 0) {
		err = super_types[super_format].
			load_super(rdev, NULL, super_minor);
		if (err == -EINVAL) {
<<<<<<< HEAD
			printk(KERN_WARNING
				"md: %s does not have a valid v%d.%d "
			       "superblock, not importing!\n",
				bdevname(rdev->bdev,b),
			       super_format, super_minor);
			goto abort_free;
		}
		if (err < 0) {
			printk(KERN_WARNING 
				"md: could not read %s's sb, not importing!\n",
				bdevname(rdev->bdev,b));
			goto abort_free;
=======
			pr_warn("md: %pg does not have a valid v%d.%d superblock, not importing!\n",
				rdev->bdev,
				super_format, super_minor);
			goto out_blkdev_put;
		}
		if (err < 0) {
			pr_warn("md: could not read %pg's sb, not importing!\n",
				rdev->bdev);
			goto out_blkdev_put;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		}
	}

	return rdev;

<<<<<<< HEAD
abort_free:
	if (rdev->bdev)
		unlock_rdev(rdev);
	free_disk_sb(rdev);
	kfree(rdev->badblocks.page);
=======
out_blkdev_put:
	fput(rdev->bdev_file);
out_clear_rdev:
	md_rdev_clear(rdev);
out_free_rdev:
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	kfree(rdev);
	return ERR_PTR(err);
}

/*
 * Check a full RAID array for plausibility
 */

<<<<<<< HEAD

static void analyze_sbs(struct mddev * mddev)
{
	int i;
	struct md_rdev *rdev, *freshest, *tmp;
	char b[BDEVNAME_SIZE];
=======
static int analyze_sbs(struct mddev *mddev)
{
	int i;
	struct md_rdev *rdev, *freshest, *tmp;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	freshest = NULL;
	rdev_for_each_safe(rdev, tmp, mddev)
		switch (super_types[mddev->major_version].
			load_super(rdev, freshest, mddev->minor_version)) {
		case 1:
			freshest = rdev;
			break;
		case 0:
			break;
		default:
<<<<<<< HEAD
			printk( KERN_ERR \
				"md: fatal superblock inconsistency in %s"
				" -- removing from array\n", 
				bdevname(rdev->bdev,b));
			kick_rdev_from_array(rdev);
		}


	super_types[mddev->major_version].
		validate_super(mddev, freshest);
=======
			pr_warn("md: fatal superblock inconsistency in %pg -- removing from array\n",
				rdev->bdev);
			md_kick_rdev_from_array(rdev);
		}

	/* Cannot find a valid fresh disk */
	if (!freshest) {
		pr_warn("md: cannot find a valid disk\n");
		return -EINVAL;
	}

	super_types[mddev->major_version].
		validate_super(mddev, NULL/*freshest*/, freshest);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	i = 0;
	rdev_for_each_safe(rdev, tmp, mddev) {
		if (mddev->max_disks &&
		    (rdev->desc_nr >= mddev->max_disks ||
		     i > mddev->max_disks)) {
<<<<<<< HEAD
			printk(KERN_WARNING
			       "md: %s: %s: only %d devices permitted\n",
			       mdname(mddev), bdevname(rdev->bdev, b),
			       mddev->max_disks);
			kick_rdev_from_array(rdev);
			continue;
		}
		if (rdev != freshest)
			if (super_types[mddev->major_version].
			    validate_super(mddev, rdev)) {
				printk(KERN_WARNING "md: kicking non-fresh %s"
					" from array!\n",
					bdevname(rdev->bdev,b));
				kick_rdev_from_array(rdev);
				continue;
			}
		if (mddev->level == LEVEL_MULTIPATH) {
			rdev->desc_nr = i++;
			rdev->raid_disk = rdev->desc_nr;
			set_bit(In_sync, &rdev->flags);
		} else if (rdev->raid_disk >= (mddev->raid_disks - min(0, mddev->delta_disks))) {
=======
			pr_warn("md: %s: %pg: only %d devices permitted\n",
				mdname(mddev), rdev->bdev,
				mddev->max_disks);
			md_kick_rdev_from_array(rdev);
			continue;
		}
		if (rdev != freshest) {
			if (super_types[mddev->major_version].
			    validate_super(mddev, freshest, rdev)) {
				pr_warn("md: kicking non-fresh %pg from array!\n",
					rdev->bdev);
				md_kick_rdev_from_array(rdev);
				continue;
			}
		}
		if (rdev->raid_disk >= (mddev->raid_disks - min(0, mddev->delta_disks)) &&
		    !test_bit(Journal, &rdev->flags)) {
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
			rdev->raid_disk = -1;
			clear_bit(In_sync, &rdev->flags);
		}
	}
<<<<<<< HEAD
=======

	return 0;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

/* Read a fixed-point number.
 * Numbers in sysfs attributes should be in "standard" units where
 * possible, so time should be in seconds.
<<<<<<< HEAD
 * However we internally use a a much smaller unit such as 
=======
 * However we internally use a a much smaller unit such as
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 * milliseconds or jiffies.
 * This function takes a decimal number with a possible fractional
 * component, and produces an integer which is the result of
 * multiplying that number by 10^'scale'.
 * all without any floating-point arithmetic.
 */
int strict_strtoul_scaled(const char *cp, unsigned long *res, int scale)
{
	unsigned long result = 0;
	long decimals = -1;
	while (isdigit(*cp) || (*cp == '.' && decimals < 0)) {
		if (*cp == '.')
			decimals = 0;
		else if (decimals < scale) {
			unsigned int value;
			value = *cp - '0';
			result = result * 10 + value;
			if (decimals >= 0)
				decimals++;
		}
		cp++;
	}
	if (*cp == '\n')
		cp++;
	if (*cp)
		return -EINVAL;
	if (decimals < 0)
		decimals = 0;
<<<<<<< HEAD
	while (decimals < scale) {
		result *= 10;
		decimals ++;
	}
	*res = result;
	return 0;
}


static void md_safemode_timeout(unsigned long data);

static ssize_t
safe_delay_show(struct mddev *mddev, char *page)
{
	int msec = (mddev->safemode_delay*1000)/HZ;
	return sprintf(page, "%d.%03d\n", msec/1000, msec%1000);
=======
	*res = result * int_pow(10, scale - decimals);
	return 0;
}

static ssize_t
safe_delay_show(struct mddev *mddev, char *page)
{
	unsigned int msec = ((unsigned long)mddev->safemode_delay*1000)/HZ;

	return sprintf(page, "%u.%03u\n", msec/1000, msec%1000);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}
static ssize_t
safe_delay_store(struct mddev *mddev, const char *cbuf, size_t len)
{
	unsigned long msec;

<<<<<<< HEAD
	if (strict_strtoul_scaled(cbuf, &msec, 3) < 0)
=======
	if (mddev_is_clustered(mddev)) {
		pr_warn("md: Safemode is disabled for clustered mode\n");
		return -EINVAL;
	}

	if (strict_strtoul_scaled(cbuf, &msec, 3) < 0 || msec > UINT_MAX / HZ)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		return -EINVAL;
	if (msec == 0)
		mddev->safemode_delay = 0;
	else {
		unsigned long old_delay = mddev->safemode_delay;
<<<<<<< HEAD
		mddev->safemode_delay = (msec*HZ)/1000;
		if (mddev->safemode_delay == 0)
			mddev->safemode_delay = 1;
		if (mddev->safemode_delay < old_delay)
			md_safemode_timeout((unsigned long)mddev);
=======
		unsigned long new_delay = (msec*HZ)/1000;

		if (new_delay == 0)
			new_delay = 1;
		mddev->safemode_delay = new_delay;
		if (new_delay < old_delay || old_delay == 0)
			mod_timer(&mddev->safemode_timer, jiffies+1);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	}
	return len;
}
static struct md_sysfs_entry md_safe_delay =
__ATTR(safe_mode_delay, S_IRUGO|S_IWUSR,safe_delay_show, safe_delay_store);

static ssize_t
level_show(struct mddev *mddev, char *page)
{
<<<<<<< HEAD
	struct md_personality *p = mddev->pers;
	if (p)
		return sprintf(page, "%s\n", p->name);
	else if (mddev->clevel[0])
		return sprintf(page, "%s\n", mddev->clevel);
	else if (mddev->level != LEVEL_NONE)
		return sprintf(page, "%d\n", mddev->level);
	else
		return 0;
=======
	struct md_personality *p;
	int ret;
	spin_lock(&mddev->lock);
	p = mddev->pers;
	if (p)
		ret = sprintf(page, "%s\n", p->name);
	else if (mddev->clevel[0])
		ret = sprintf(page, "%s\n", mddev->clevel);
	else if (mddev->level != LEVEL_NONE)
		ret = sprintf(page, "%d\n", mddev->level);
	else
		ret = 0;
	spin_unlock(&mddev->lock);
	return ret;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

static ssize_t
level_store(struct mddev *mddev, const char *buf, size_t len)
{
	char clevel[16];
<<<<<<< HEAD
	ssize_t rv = len;
	struct md_personality *pers;
	long level;
	void *priv;
	struct md_rdev *rdev;

	if (mddev->pers == NULL) {
		if (len == 0)
			return 0;
		if (len >= sizeof(mddev->clevel))
			return -ENOSPC;
		strncpy(mddev->clevel, buf, len);
		if (mddev->clevel[len-1] == '\n')
			len--;
		mddev->clevel[len] = 0;
		mddev->level = LEVEL_NONE;
		return rv;
	}
=======
	ssize_t rv;
	size_t slen = len;
	struct md_personality *pers, *oldpers;
	long level;
	void *priv, *oldpriv;
	struct md_rdev *rdev;

	if (slen == 0 || slen >= sizeof(clevel))
		return -EINVAL;

	rv = mddev_suspend_and_lock(mddev);
	if (rv)
		return rv;

	if (mddev->pers == NULL) {
		memcpy(mddev->clevel, buf, slen);
		if (mddev->clevel[slen-1] == '\n')
			slen--;
		mddev->clevel[slen] = 0;
		mddev->level = LEVEL_NONE;
		rv = len;
		goto out_unlock;
	}
	rv = -EROFS;
	if (!md_is_rdwr(mddev))
		goto out_unlock;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	/* request to change the personality.  Need to ensure:
	 *  - array is not engaged in resync/recovery/reshape
	 *  - old personality can be suspended
	 *  - new personality will access other array.
	 */

<<<<<<< HEAD
	if (mddev->sync_thread ||
	    mddev->reshape_position != MaxSector ||
	    mddev->sysfs_active)
		return -EBUSY;

	if (!mddev->pers->quiesce) {
		printk(KERN_WARNING "md: %s: %s does not support online personality change\n",
		       mdname(mddev), mddev->pers->name);
		return -EINVAL;
	}

	/* Now find the new personality */
	if (len == 0 || len >= sizeof(clevel))
		return -EINVAL;
	strncpy(clevel, buf, len);
	if (clevel[len-1] == '\n')
		len--;
	clevel[len] = 0;
	if (strict_strtol(clevel, 10, &level))
=======
	rv = -EBUSY;
	if (test_bit(MD_RECOVERY_RUNNING, &mddev->recovery) ||
	    mddev->reshape_position != MaxSector ||
	    mddev->sysfs_active)
		goto out_unlock;

	rv = -EINVAL;
	if (!mddev->pers->quiesce) {
		pr_warn("md: %s: %s does not support online personality change\n",
			mdname(mddev), mddev->pers->name);
		goto out_unlock;
	}

	/* Now find the new personality */
	memcpy(clevel, buf, slen);
	if (clevel[slen-1] == '\n')
		slen--;
	clevel[slen] = 0;
	if (kstrtol(clevel, 10, &level))
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		level = LEVEL_NONE;

	if (request_module("md-%s", clevel) != 0)
		request_module("md-level-%s", clevel);
	spin_lock(&pers_lock);
	pers = find_pers(level, clevel);
	if (!pers || !try_module_get(pers->owner)) {
		spin_unlock(&pers_lock);
<<<<<<< HEAD
		printk(KERN_WARNING "md: personality %s not loaded\n", clevel);
		return -EINVAL;
=======
		pr_warn("md: personality %s not loaded\n", clevel);
		rv = -EINVAL;
		goto out_unlock;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	}
	spin_unlock(&pers_lock);

	if (pers == mddev->pers) {
		/* Nothing to do! */
		module_put(pers->owner);
<<<<<<< HEAD
		return rv;
	}
	if (!pers->takeover) {
		module_put(pers->owner);
		printk(KERN_WARNING "md: %s: %s does not support personality takeover\n",
		       mdname(mddev), clevel);
		return -EINVAL;
=======
		rv = len;
		goto out_unlock;
	}
	if (!pers->takeover) {
		module_put(pers->owner);
		pr_warn("md: %s: %s does not support personality takeover\n",
			mdname(mddev), clevel);
		rv = -EINVAL;
		goto out_unlock;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	}

	rdev_for_each(rdev, mddev)
		rdev->new_raid_disk = rdev->raid_disk;

	/* ->takeover must set new_* and/or delta_disks
	 * if it succeeds, and may set them when it fails.
	 */
	priv = pers->takeover(mddev);
	if (IS_ERR(priv)) {
		mddev->new_level = mddev->level;
		mddev->new_layout = mddev->layout;
		mddev->new_chunk_sectors = mddev->chunk_sectors;
		mddev->raid_disks -= mddev->delta_disks;
		mddev->delta_disks = 0;
<<<<<<< HEAD
		module_put(pers->owner);
		printk(KERN_WARNING "md: %s: %s would not accept array\n",
		       mdname(mddev), clevel);
		return PTR_ERR(priv);
	}

	/* Looks like we have a winner */
	mddev_suspend(mddev);
	mddev->pers->stop(mddev);
	
	if (mddev->pers->sync_request == NULL &&
	    pers->sync_request != NULL) {
		/* need to add the md_redundancy_group */
		if (sysfs_create_group(&mddev->kobj, &md_redundancy_group))
			printk(KERN_WARNING
			       "md: cannot register extra attributes for %s\n",
			       mdname(mddev));
		mddev->sysfs_action = sysfs_get_dirent(mddev->kobj.sd, NULL, "sync_action");
	}		
	if (mddev->pers->sync_request != NULL &&
	    pers->sync_request == NULL) {
		/* need to remove the md_redundancy_group */
		if (mddev->to_remove == NULL)
			mddev->to_remove = &md_redundancy_group;
	}

	if (mddev->pers->sync_request == NULL &&
=======
		mddev->reshape_backwards = 0;
		module_put(pers->owner);
		pr_warn("md: %s: %s would not accept array\n",
			mdname(mddev), clevel);
		rv = PTR_ERR(priv);
		goto out_unlock;
	}

	/* Looks like we have a winner */
	mddev_detach(mddev);

	spin_lock(&mddev->lock);
	oldpers = mddev->pers;
	oldpriv = mddev->private;
	mddev->pers = pers;
	mddev->private = priv;
	strscpy(mddev->clevel, pers->name, sizeof(mddev->clevel));
	mddev->level = mddev->new_level;
	mddev->layout = mddev->new_layout;
	mddev->chunk_sectors = mddev->new_chunk_sectors;
	mddev->delta_disks = 0;
	mddev->reshape_backwards = 0;
	mddev->degraded = 0;
	spin_unlock(&mddev->lock);

	if (oldpers->sync_request == NULL &&
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	    mddev->external) {
		/* We are converting from a no-redundancy array
		 * to a redundancy array and metadata is managed
		 * externally so we need to be sure that writes
		 * won't block due to a need to transition
		 *      clean->dirty
		 * until external management is started.
		 */
		mddev->in_sync = 0;
		mddev->safemode_delay = 0;
		mddev->safemode = 0;
	}

<<<<<<< HEAD
=======
	oldpers->free(mddev, oldpriv);

	if (oldpers->sync_request == NULL &&
	    pers->sync_request != NULL) {
		/* need to add the md_redundancy_group */
		if (sysfs_create_group(&mddev->kobj, &md_redundancy_group))
			pr_warn("md: cannot register extra attributes for %s\n",
				mdname(mddev));
		mddev->sysfs_action = sysfs_get_dirent(mddev->kobj.sd, "sync_action");
		mddev->sysfs_completed = sysfs_get_dirent_safe(mddev->kobj.sd, "sync_completed");
		mddev->sysfs_degraded = sysfs_get_dirent_safe(mddev->kobj.sd, "degraded");
	}
	if (oldpers->sync_request != NULL &&
	    pers->sync_request == NULL) {
		/* need to remove the md_redundancy_group */
		if (mddev->to_remove == NULL)
			mddev->to_remove = &md_redundancy_group;
	}

	module_put(oldpers->owner);

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	rdev_for_each(rdev, mddev) {
		if (rdev->raid_disk < 0)
			continue;
		if (rdev->new_raid_disk >= mddev->raid_disks)
			rdev->new_raid_disk = -1;
		if (rdev->new_raid_disk == rdev->raid_disk)
			continue;
		sysfs_unlink_rdev(mddev, rdev);
	}
	rdev_for_each(rdev, mddev) {
		if (rdev->raid_disk < 0)
			continue;
		if (rdev->new_raid_disk == rdev->raid_disk)
			continue;
		rdev->raid_disk = rdev->new_raid_disk;
		if (rdev->raid_disk < 0)
			clear_bit(In_sync, &rdev->flags);
		else {
			if (sysfs_link_rdev(mddev, rdev))
<<<<<<< HEAD
				printk(KERN_WARNING "md: cannot register rd%d"
				       " for %s after level change\n",
				       rdev->raid_disk, mdname(mddev));
		}
	}

	module_put(mddev->pers->owner);
	mddev->pers = pers;
	mddev->private = priv;
	strlcpy(mddev->clevel, pers->name, sizeof(mddev->clevel));
	mddev->level = mddev->new_level;
	mddev->layout = mddev->new_layout;
	mddev->chunk_sectors = mddev->new_chunk_sectors;
	mddev->delta_disks = 0;
	mddev->degraded = 0;
	if (mddev->pers->sync_request == NULL) {
=======
				pr_warn("md: cannot register rd%d for %s after level change\n",
					rdev->raid_disk, mdname(mddev));
		}
	}

	if (pers->sync_request == NULL) {
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		/* this is now an array without redundancy, so
		 * it must always be in_sync
		 */
		mddev->in_sync = 1;
		del_timer_sync(&mddev->safemode_timer);
	}
<<<<<<< HEAD
	blk_set_stacking_limits(&mddev->queue->limits);
	pers->run(mddev);
	mddev_resume(mddev);
	set_bit(MD_CHANGE_DEVS, &mddev->flags);
	set_bit(MD_RECOVERY_NEEDED, &mddev->recovery);
	md_wakeup_thread(mddev->thread);
	sysfs_notify(&mddev->kobj, NULL, "level");
	md_new_event(mddev);
=======
	pers->run(mddev);
	set_bit(MD_SB_CHANGE_DEVS, &mddev->sb_flags);
	if (!mddev->thread)
		md_update_sb(mddev, 1);
	sysfs_notify_dirent_safe(mddev->sysfs_level);
	md_new_event();
	rv = len;
out_unlock:
	mddev_unlock_and_resume(mddev);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	return rv;
}

static struct md_sysfs_entry md_level =
__ATTR(level, S_IRUGO|S_IWUSR, level_show, level_store);

<<<<<<< HEAD

=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
static ssize_t
layout_show(struct mddev *mddev, char *page)
{
	/* just a number, not meaningful for all levels */
	if (mddev->reshape_position != MaxSector &&
	    mddev->layout != mddev->new_layout)
		return sprintf(page, "%d (%d)\n",
			       mddev->new_layout, mddev->layout);
	return sprintf(page, "%d\n", mddev->layout);
}

static ssize_t
layout_store(struct mddev *mddev, const char *buf, size_t len)
{
<<<<<<< HEAD
	char *e;
	unsigned long n = simple_strtoul(buf, &e, 10);

	if (!*buf || (*e && *e != '\n'))
		return -EINVAL;

	if (mddev->pers) {
		int err;
		if (mddev->pers->check_reshape == NULL)
			return -EBUSY;
		mddev->new_layout = n;
		err = mddev->pers->check_reshape(mddev);
		if (err) {
			mddev->new_layout = mddev->layout;
			return err;
=======
	unsigned int n;
	int err;

	err = kstrtouint(buf, 10, &n);
	if (err < 0)
		return err;
	err = mddev_lock(mddev);
	if (err)
		return err;

	if (mddev->pers) {
		if (mddev->pers->check_reshape == NULL)
			err = -EBUSY;
		else if (!md_is_rdwr(mddev))
			err = -EROFS;
		else {
			mddev->new_layout = n;
			err = mddev->pers->check_reshape(mddev);
			if (err)
				mddev->new_layout = mddev->layout;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		}
	} else {
		mddev->new_layout = n;
		if (mddev->reshape_position == MaxSector)
			mddev->layout = n;
	}
<<<<<<< HEAD
	return len;
=======
	mddev_unlock(mddev);
	return err ?: len;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}
static struct md_sysfs_entry md_layout =
__ATTR(layout, S_IRUGO|S_IWUSR, layout_show, layout_store);

<<<<<<< HEAD

=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
static ssize_t
raid_disks_show(struct mddev *mddev, char *page)
{
	if (mddev->raid_disks == 0)
		return 0;
	if (mddev->reshape_position != MaxSector &&
	    mddev->delta_disks != 0)
		return sprintf(page, "%d (%d)\n", mddev->raid_disks,
			       mddev->raid_disks - mddev->delta_disks);
	return sprintf(page, "%d\n", mddev->raid_disks);
}

static int update_raid_disks(struct mddev *mddev, int raid_disks);

static ssize_t
raid_disks_store(struct mddev *mddev, const char *buf, size_t len)
{
<<<<<<< HEAD
	char *e;
	int rv = 0;
	unsigned long n = simple_strtoul(buf, &e, 10);

	if (!*buf || (*e && *e != '\n'))
		return -EINVAL;

	if (mddev->pers)
		rv = update_raid_disks(mddev, n);
	else if (mddev->reshape_position != MaxSector) {
		int olddisks = mddev->raid_disks - mddev->delta_disks;
		mddev->delta_disks = n - olddisks;
		mddev->raid_disks = n;
	} else
		mddev->raid_disks = n;
	return rv ? rv : len;
=======
	unsigned int n;
	int err;

	err = kstrtouint(buf, 10, &n);
	if (err < 0)
		return err;

	err = mddev_lock(mddev);
	if (err)
		return err;
	if (mddev->pers)
		err = update_raid_disks(mddev, n);
	else if (mddev->reshape_position != MaxSector) {
		struct md_rdev *rdev;
		int olddisks = mddev->raid_disks - mddev->delta_disks;

		err = -EINVAL;
		rdev_for_each(rdev, mddev) {
			if (olddisks < n &&
			    rdev->data_offset < rdev->new_data_offset)
				goto out_unlock;
			if (olddisks > n &&
			    rdev->data_offset > rdev->new_data_offset)
				goto out_unlock;
		}
		err = 0;
		mddev->delta_disks = n - olddisks;
		mddev->raid_disks = n;
		mddev->reshape_backwards = (mddev->delta_disks < 0);
	} else
		mddev->raid_disks = n;
out_unlock:
	mddev_unlock(mddev);
	return err ? err : len;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}
static struct md_sysfs_entry md_raid_disks =
__ATTR(raid_disks, S_IRUGO|S_IWUSR, raid_disks_show, raid_disks_store);

static ssize_t
<<<<<<< HEAD
=======
uuid_show(struct mddev *mddev, char *page)
{
	return sprintf(page, "%pU\n", mddev->uuid);
}
static struct md_sysfs_entry md_uuid =
__ATTR(uuid, S_IRUGO, uuid_show, NULL);

static ssize_t
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
chunk_size_show(struct mddev *mddev, char *page)
{
	if (mddev->reshape_position != MaxSector &&
	    mddev->chunk_sectors != mddev->new_chunk_sectors)
		return sprintf(page, "%d (%d)\n",
			       mddev->new_chunk_sectors << 9,
			       mddev->chunk_sectors << 9);
	return sprintf(page, "%d\n", mddev->chunk_sectors << 9);
}

static ssize_t
chunk_size_store(struct mddev *mddev, const char *buf, size_t len)
{
<<<<<<< HEAD
	char *e;
	unsigned long n = simple_strtoul(buf, &e, 10);

	if (!*buf || (*e && *e != '\n'))
		return -EINVAL;

	if (mddev->pers) {
		int err;
		if (mddev->pers->check_reshape == NULL)
			return -EBUSY;
		mddev->new_chunk_sectors = n >> 9;
		err = mddev->pers->check_reshape(mddev);
		if (err) {
			mddev->new_chunk_sectors = mddev->chunk_sectors;
			return err;
=======
	unsigned long n;
	int err;

	err = kstrtoul(buf, 10, &n);
	if (err < 0)
		return err;

	err = mddev_lock(mddev);
	if (err)
		return err;
	if (mddev->pers) {
		if (mddev->pers->check_reshape == NULL)
			err = -EBUSY;
		else if (!md_is_rdwr(mddev))
			err = -EROFS;
		else {
			mddev->new_chunk_sectors = n >> 9;
			err = mddev->pers->check_reshape(mddev);
			if (err)
				mddev->new_chunk_sectors = mddev->chunk_sectors;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		}
	} else {
		mddev->new_chunk_sectors = n >> 9;
		if (mddev->reshape_position == MaxSector)
			mddev->chunk_sectors = n >> 9;
	}
<<<<<<< HEAD
	return len;
=======
	mddev_unlock(mddev);
	return err ?: len;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}
static struct md_sysfs_entry md_chunk_size =
__ATTR(chunk_size, S_IRUGO|S_IWUSR, chunk_size_show, chunk_size_store);

static ssize_t
resync_start_show(struct mddev *mddev, char *page)
{
	if (mddev->recovery_cp == MaxSector)
		return sprintf(page, "none\n");
	return sprintf(page, "%llu\n", (unsigned long long)mddev->recovery_cp);
}

static ssize_t
resync_start_store(struct mddev *mddev, const char *buf, size_t len)
{
<<<<<<< HEAD
	char *e;
	unsigned long long n = simple_strtoull(buf, &e, 10);

	if (mddev->pers && !test_bit(MD_RECOVERY_FROZEN, &mddev->recovery))
		return -EBUSY;
	if (cmd_match(buf, "none"))
		n = MaxSector;
	else if (!*buf || (*e && *e != '\n'))
		return -EINVAL;

	mddev->recovery_cp = n;
	return len;
}
static struct md_sysfs_entry md_resync_start =
__ATTR(resync_start, S_IRUGO|S_IWUSR, resync_start_show, resync_start_store);
=======
	unsigned long long n;
	int err;

	if (cmd_match(buf, "none"))
		n = MaxSector;
	else {
		err = kstrtoull(buf, 10, &n);
		if (err < 0)
			return err;
		if (n != (sector_t)n)
			return -EINVAL;
	}

	err = mddev_lock(mddev);
	if (err)
		return err;
	if (mddev->pers && !test_bit(MD_RECOVERY_FROZEN, &mddev->recovery))
		err = -EBUSY;

	if (!err) {
		mddev->recovery_cp = n;
		if (mddev->pers)
			set_bit(MD_SB_CHANGE_CLEAN, &mddev->sb_flags);
	}
	mddev_unlock(mddev);
	return err ?: len;
}
static struct md_sysfs_entry md_resync_start =
__ATTR_PREALLOC(resync_start, S_IRUGO|S_IWUSR,
		resync_start_show, resync_start_store);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

/*
 * The array state can be:
 *
 * clear
 *     No devices, no size, no level
 *     Equivalent to STOP_ARRAY ioctl
 * inactive
 *     May have some settings, but array is not active
 *        all IO results in error
 *     When written, doesn't tear down array, but just stops it
 * suspended (not supported yet)
 *     All IO requests will block. The array can be reconfigured.
 *     Writing this, if accepted, will block until array is quiescent
 * readonly
 *     no resync can happen.  no superblocks get written.
 *     write requests fail
 * read-auto
 *     like readonly, but behaves like 'clean' on a write request.
 *
 * clean - no pending writes, but otherwise active.
 *     When written to inactive array, starts without resync
 *     If a write request arrives then
 *       if metadata is known, mark 'dirty' and switch to 'active'.
 *       if not known, block and switch to write-pending
 *     If written to an active array that has pending writes, then fails.
 * active
 *     fully active: IO and resync can be happening.
 *     When written to inactive array, starts with resync
 *
 * write-pending
 *     clean, but writes are blocked waiting for 'active' to be written.
 *
 * active-idle
 *     like active, but no writes have been seen for a while (100msec).
 *
<<<<<<< HEAD
 */
enum array_state { clear, inactive, suspended, readonly, read_auto, clean, active,
		   write_pending, active_idle, bad_word};
static char *array_states[] = {
	"clear", "inactive", "suspended", "readonly", "read-auto", "clean", "active",
	"write-pending", "active-idle", NULL };
=======
 * broken
*     Array is failed. It's useful because mounted-arrays aren't stopped
*     when array is failed, so this state will at least alert the user that
*     something is wrong.
 */
enum array_state { clear, inactive, suspended, readonly, read_auto, clean, active,
		   write_pending, active_idle, broken, bad_word};
static char *array_states[] = {
	"clear", "inactive", "suspended", "readonly", "read-auto", "clean", "active",
	"write-pending", "active-idle", "broken", NULL };
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

static int match_word(const char *word, char **list)
{
	int n;
	for (n=0; list[n]; n++)
		if (cmd_match(word, list[n]))
			break;
	return n;
}

static ssize_t
array_state_show(struct mddev *mddev, char *page)
{
	enum array_state st = inactive;

<<<<<<< HEAD
	if (mddev->pers)
		switch(mddev->ro) {
		case 1:
			st = readonly;
			break;
		case 2:
			st = read_auto;
			break;
		case 0:
			if (mddev->in_sync)
				st = clean;
			else if (test_bit(MD_CHANGE_PENDING, &mddev->flags))
				st = write_pending;
=======
	if (mddev->pers && !test_bit(MD_NOT_READY, &mddev->flags)) {
		switch(mddev->ro) {
		case MD_RDONLY:
			st = readonly;
			break;
		case MD_AUTO_READ:
			st = read_auto;
			break;
		case MD_RDWR:
			spin_lock(&mddev->lock);
			if (test_bit(MD_SB_CHANGE_PENDING, &mddev->sb_flags))
				st = write_pending;
			else if (mddev->in_sync)
				st = clean;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
			else if (mddev->safemode)
				st = active_idle;
			else
				st = active;
<<<<<<< HEAD
		}
	else {
=======
			spin_unlock(&mddev->lock);
		}

		if (test_bit(MD_BROKEN, &mddev->flags) && st == clean)
			st = broken;
	} else {
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		if (list_empty(&mddev->disks) &&
		    mddev->raid_disks == 0 &&
		    mddev->dev_sectors == 0)
			st = clear;
		else
			st = inactive;
	}
	return sprintf(page, "%s\n", array_states[st]);
}

<<<<<<< HEAD
static int do_md_stop(struct mddev * mddev, int ro, struct block_device *bdev);
static int md_set_readonly(struct mddev * mddev, struct block_device *bdev);
static int do_md_run(struct mddev * mddev);
=======
static int do_md_stop(struct mddev *mddev, int ro);
static int md_set_readonly(struct mddev *mddev);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
static int restart_array(struct mddev *mddev);

static ssize_t
array_state_store(struct mddev *mddev, const char *buf, size_t len)
{
<<<<<<< HEAD
	int err = -EINVAL;
	enum array_state st = match_word(buf, array_states);
	switch(st) {
	case bad_word:
		break;
	case clear:
		/* stopping an active array */
		if (atomic_read(&mddev->openers) > 0)
			return -EBUSY;
		err = do_md_stop(mddev, 0, NULL);
		break;
	case inactive:
		/* stopping an active array */
		if (mddev->pers) {
			if (atomic_read(&mddev->openers) > 0)
				return -EBUSY;
			err = do_md_stop(mddev, 2, NULL);
		} else
			err = 0; /* already inactive */
		break;
	case suspended:
		break; /* not supported yet */
	case readonly:
		if (mddev->pers)
			err = md_set_readonly(mddev, NULL);
		else {
			mddev->ro = 1;
=======
	int err = 0;
	enum array_state st = match_word(buf, array_states);

	/* No lock dependent actions */
	switch (st) {
	case suspended:		/* not supported yet */
	case write_pending:	/* cannot be set */
	case active_idle:	/* cannot be set */
	case broken:		/* cannot be set */
	case bad_word:
		return -EINVAL;
	case clear:
	case readonly:
	case inactive:
	case read_auto:
		if (!mddev->pers || !md_is_rdwr(mddev))
			break;
		/* write sysfs will not open mddev and opener should be 0 */
		err = mddev_set_closing_and_sync_blockdev(mddev, 0);
		if (err)
			return err;
		break;
	default:
		break;
	}

	if (mddev->pers && (st == active || st == clean) &&
	    mddev->ro != MD_RDONLY) {
		/* don't take reconfig_mutex when toggling between
		 * clean and active
		 */
		spin_lock(&mddev->lock);
		if (st == active) {
			restart_array(mddev);
			clear_bit(MD_SB_CHANGE_PENDING, &mddev->sb_flags);
			md_wakeup_thread(mddev->thread);
			wake_up(&mddev->sb_wait);
		} else /* st == clean */ {
			restart_array(mddev);
			if (!set_in_sync(mddev))
				err = -EBUSY;
		}
		if (!err)
			sysfs_notify_dirent_safe(mddev->sysfs_state);
		spin_unlock(&mddev->lock);
		return err ?: len;
	}
	err = mddev_lock(mddev);
	if (err)
		return err;

	switch (st) {
	case inactive:
		/* stop an active array, return 0 otherwise */
		if (mddev->pers)
			err = do_md_stop(mddev, 2);
		break;
	case clear:
		err = do_md_stop(mddev, 0);
		break;
	case readonly:
		if (mddev->pers)
			err = md_set_readonly(mddev);
		else {
			mddev->ro = MD_RDONLY;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
			set_disk_ro(mddev->gendisk, 1);
			err = do_md_run(mddev);
		}
		break;
	case read_auto:
		if (mddev->pers) {
<<<<<<< HEAD
			if (mddev->ro == 0)
				err = md_set_readonly(mddev, NULL);
			else if (mddev->ro == 1)
				err = restart_array(mddev);
			if (err == 0) {
				mddev->ro = 2;
				set_disk_ro(mddev->gendisk, 0);
			}
		} else {
			mddev->ro = 2;
=======
			if (md_is_rdwr(mddev))
				err = md_set_readonly(mddev);
			else if (mddev->ro == MD_RDONLY)
				err = restart_array(mddev);
			if (err == 0) {
				mddev->ro = MD_AUTO_READ;
				set_disk_ro(mddev->gendisk, 0);
			}
		} else {
			mddev->ro = MD_AUTO_READ;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
			err = do_md_run(mddev);
		}
		break;
	case clean:
		if (mddev->pers) {
<<<<<<< HEAD
			restart_array(mddev);
			spin_lock_irq(&mddev->write_lock);
			if (atomic_read(&mddev->writes_pending) == 0) {
				if (mddev->in_sync == 0) {
					mddev->in_sync = 1;
					if (mddev->safemode == 1)
						mddev->safemode = 0;
					set_bit(MD_CHANGE_CLEAN, &mddev->flags);
				}
				err = 0;
			} else
				err = -EBUSY;
			spin_unlock_irq(&mddev->write_lock);
=======
			err = restart_array(mddev);
			if (err)
				break;
			spin_lock(&mddev->lock);
			if (!set_in_sync(mddev))
				err = -EBUSY;
			spin_unlock(&mddev->lock);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		} else
			err = -EINVAL;
		break;
	case active:
		if (mddev->pers) {
<<<<<<< HEAD
			restart_array(mddev);
			clear_bit(MD_CHANGE_PENDING, &mddev->flags);
			wake_up(&mddev->sb_wait);
			err = 0;
		} else {
			mddev->ro = 0;
=======
			err = restart_array(mddev);
			if (err)
				break;
			clear_bit(MD_SB_CHANGE_PENDING, &mddev->sb_flags);
			wake_up(&mddev->sb_wait);
			err = 0;
		} else {
			mddev->ro = MD_RDWR;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
			set_disk_ro(mddev->gendisk, 0);
			err = do_md_run(mddev);
		}
		break;
<<<<<<< HEAD
	case write_pending:
	case active_idle:
		/* these cannot be set */
		break;
	}
	if (err)
		return err;
	else {
		if (mddev->hold_active == UNTIL_IOCTL)
			mddev->hold_active = 0;
		sysfs_notify_dirent_safe(mddev->sysfs_state);
		return len;
	}
}
static struct md_sysfs_entry md_array_state =
__ATTR(array_state, S_IRUGO|S_IWUSR, array_state_show, array_state_store);
=======
	default:
		err = -EINVAL;
		break;
	}

	if (!err) {
		if (mddev->hold_active == UNTIL_IOCTL)
			mddev->hold_active = 0;
		sysfs_notify_dirent_safe(mddev->sysfs_state);
	}
	mddev_unlock(mddev);

	if (st == readonly || st == read_auto || st == inactive ||
	    (err && st == clear))
		clear_bit(MD_CLOSING, &mddev->flags);

	return err ?: len;
}
static struct md_sysfs_entry md_array_state =
__ATTR_PREALLOC(array_state, S_IRUGO|S_IWUSR, array_state_show, array_state_store);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

static ssize_t
max_corrected_read_errors_show(struct mddev *mddev, char *page) {
	return sprintf(page, "%d\n",
		       atomic_read(&mddev->max_corr_read_errors));
}

static ssize_t
max_corrected_read_errors_store(struct mddev *mddev, const char *buf, size_t len)
{
<<<<<<< HEAD
	char *e;
	unsigned long n = simple_strtoul(buf, &e, 10);

	if (*buf && (*e == 0 || *e == '\n')) {
		atomic_set(&mddev->max_corr_read_errors, n);
		return len;
	}
	return -EINVAL;
=======
	unsigned int n;
	int rv;

	rv = kstrtouint(buf, 10, &n);
	if (rv < 0)
		return rv;
	if (n > INT_MAX)
		return -EINVAL;
	atomic_set(&mddev->max_corr_read_errors, n);
	return len;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

static struct md_sysfs_entry max_corr_read_errors =
__ATTR(max_read_errors, S_IRUGO|S_IWUSR, max_corrected_read_errors_show,
	max_corrected_read_errors_store);

static ssize_t
null_show(struct mddev *mddev, char *page)
{
	return -EINVAL;
}

static ssize_t
new_dev_store(struct mddev *mddev, const char *buf, size_t len)
{
	/* buf must be %d:%d\n? giving major and minor numbers */
	/* The new device is added to the array.
	 * If the array has a persistent superblock, we read the
	 * superblock to initialise info and check validity.
	 * Otherwise, only checking done is that in bind_rdev_to_array,
	 * which mainly checks size.
	 */
	char *e;
	int major = simple_strtoul(buf, &e, 10);
	int minor;
	dev_t dev;
	struct md_rdev *rdev;
	int err;

	if (!*buf || *e != ':' || !e[1] || e[1] == '\n')
		return -EINVAL;
	minor = simple_strtoul(e+1, &e, 10);
	if (*e && *e != '\n')
		return -EINVAL;
	dev = MKDEV(major, minor);
	if (major != MAJOR(dev) ||
	    minor != MINOR(dev))
		return -EOVERFLOW;

<<<<<<< HEAD

=======
	err = mddev_suspend_and_lock(mddev);
	if (err)
		return err;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	if (mddev->persistent) {
		rdev = md_import_device(dev, mddev->major_version,
					mddev->minor_version);
		if (!IS_ERR(rdev) && !list_empty(&mddev->disks)) {
			struct md_rdev *rdev0
				= list_entry(mddev->disks.next,
					     struct md_rdev, same_set);
			err = super_types[mddev->major_version]
				.load_super(rdev, rdev0, mddev->minor_version);
			if (err < 0)
				goto out;
		}
	} else if (mddev->external)
		rdev = md_import_device(dev, -2, -1);
	else
		rdev = md_import_device(dev, -1, -1);

<<<<<<< HEAD
	if (IS_ERR(rdev))
		return PTR_ERR(rdev);
	err = bind_rdev_to_array(rdev, mddev);
 out:
	if (err)
		export_rdev(rdev);
=======
	if (IS_ERR(rdev)) {
		mddev_unlock_and_resume(mddev);
		return PTR_ERR(rdev);
	}
	err = bind_rdev_to_array(rdev, mddev);
 out:
	if (err)
		export_rdev(rdev, mddev);
	mddev_unlock_and_resume(mddev);
	if (!err)
		md_new_event();
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	return err ? err : len;
}

static struct md_sysfs_entry md_new_device =
__ATTR(new_dev, S_IWUSR, null_show, new_dev_store);

static ssize_t
bitmap_store(struct mddev *mddev, const char *buf, size_t len)
{
	char *end;
	unsigned long chunk, end_chunk;
<<<<<<< HEAD

=======
	int err;

	err = mddev_lock(mddev);
	if (err)
		return err;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	if (!mddev->bitmap)
		goto out;
	/* buf should be <chunk> <chunk> ... or <chunk>-<chunk> ... (range) */
	while (*buf) {
		chunk = end_chunk = simple_strtoul(buf, &end, 0);
		if (buf == end) break;
		if (*end == '-') { /* range */
			buf = end + 1;
			end_chunk = simple_strtoul(buf, &end, 0);
			if (buf == end) break;
		}
		if (*end && !isspace(*end)) break;
<<<<<<< HEAD
		bitmap_dirty_bits(mddev->bitmap, chunk, end_chunk);
		buf = skip_spaces(end);
	}
	bitmap_unplug(mddev->bitmap); /* flush the bits to disk */
out:
=======
		md_bitmap_dirty_bits(mddev->bitmap, chunk, end_chunk);
		buf = skip_spaces(end);
	}
	md_bitmap_unplug(mddev->bitmap); /* flush the bits to disk */
out:
	mddev_unlock(mddev);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	return len;
}

static struct md_sysfs_entry md_bitmap =
__ATTR(bitmap_set_bits, S_IWUSR, null_show, bitmap_store);

static ssize_t
size_show(struct mddev *mddev, char *page)
{
	return sprintf(page, "%llu\n",
		(unsigned long long)mddev->dev_sectors / 2);
}

static int update_size(struct mddev *mddev, sector_t num_sectors);

static ssize_t
size_store(struct mddev *mddev, const char *buf, size_t len)
{
	/* If array is inactive, we can reduce the component size, but
	 * not increase it (except from 0).
	 * If array is active, we can try an on-line resize
	 */
	sector_t sectors;
	int err = strict_blocks_to_sectors(buf, &sectors);

	if (err < 0)
		return err;
<<<<<<< HEAD
	if (mddev->pers) {
		err = update_size(mddev, sectors);
		md_update_sb(mddev, 1);
=======
	err = mddev_lock(mddev);
	if (err)
		return err;
	if (mddev->pers) {
		err = update_size(mddev, sectors);
		if (err == 0)
			md_update_sb(mddev, 1);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	} else {
		if (mddev->dev_sectors == 0 ||
		    mddev->dev_sectors > sectors)
			mddev->dev_sectors = sectors;
		else
			err = -ENOSPC;
	}
<<<<<<< HEAD
=======
	mddev_unlock(mddev);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	return err ? err : len;
}

static struct md_sysfs_entry md_size =
__ATTR(component_size, S_IRUGO|S_IWUSR, size_show, size_store);

<<<<<<< HEAD

/* Metdata version.
=======
/* Metadata version.
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 * This is one of
 *   'none' for arrays with no metadata (good luck...)
 *   'external' for arrays with externally managed metadata,
 * or N.M for internally known formats
 */
static ssize_t
metadata_show(struct mddev *mddev, char *page)
{
	if (mddev->persistent)
		return sprintf(page, "%d.%d\n",
			       mddev->major_version, mddev->minor_version);
	else if (mddev->external)
		return sprintf(page, "external:%s\n", mddev->metadata_type);
	else
		return sprintf(page, "none\n");
}

static ssize_t
metadata_store(struct mddev *mddev, const char *buf, size_t len)
{
	int major, minor;
	char *e;
<<<<<<< HEAD
=======
	int err;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	/* Changing the details of 'external' metadata is
	 * always permitted.  Otherwise there must be
	 * no devices attached to the array.
	 */
<<<<<<< HEAD
	if (mddev->external && strncmp(buf, "external:", 9) == 0)
		;
	else if (!list_empty(&mddev->disks))
		return -EBUSY;

=======

	err = mddev_lock(mddev);
	if (err)
		return err;
	err = -EBUSY;
	if (mddev->external && strncmp(buf, "external:", 9) == 0)
		;
	else if (!list_empty(&mddev->disks))
		goto out_unlock;

	err = 0;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	if (cmd_match(buf, "none")) {
		mddev->persistent = 0;
		mddev->external = 0;
		mddev->major_version = 0;
		mddev->minor_version = 90;
<<<<<<< HEAD
		return len;
=======
		goto out_unlock;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	}
	if (strncmp(buf, "external:", 9) == 0) {
		size_t namelen = len-9;
		if (namelen >= sizeof(mddev->metadata_type))
			namelen = sizeof(mddev->metadata_type)-1;
<<<<<<< HEAD
		strncpy(mddev->metadata_type, buf+9, namelen);
=======
		memcpy(mddev->metadata_type, buf+9, namelen);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		mddev->metadata_type[namelen] = 0;
		if (namelen && mddev->metadata_type[namelen-1] == '\n')
			mddev->metadata_type[--namelen] = 0;
		mddev->persistent = 0;
		mddev->external = 1;
		mddev->major_version = 0;
		mddev->minor_version = 90;
<<<<<<< HEAD
		return len;
	}
	major = simple_strtoul(buf, &e, 10);
	if (e==buf || *e != '.')
		return -EINVAL;
	buf = e+1;
	minor = simple_strtoul(buf, &e, 10);
	if (e==buf || (*e && *e != '\n') )
		return -EINVAL;
	if (major >= ARRAY_SIZE(super_types) || super_types[major].name == NULL)
		return -ENOENT;
=======
		goto out_unlock;
	}
	major = simple_strtoul(buf, &e, 10);
	err = -EINVAL;
	if (e==buf || *e != '.')
		goto out_unlock;
	buf = e+1;
	minor = simple_strtoul(buf, &e, 10);
	if (e==buf || (*e && *e != '\n') )
		goto out_unlock;
	err = -ENOENT;
	if (major >= ARRAY_SIZE(super_types) || super_types[major].name == NULL)
		goto out_unlock;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	mddev->major_version = major;
	mddev->minor_version = minor;
	mddev->persistent = 1;
	mddev->external = 0;
<<<<<<< HEAD
	return len;
}

static struct md_sysfs_entry md_metadata =
__ATTR(metadata_version, S_IRUGO|S_IWUSR, metadata_show, metadata_store);
=======
	err = 0;
out_unlock:
	mddev_unlock(mddev);
	return err ?: len;
}

static struct md_sysfs_entry md_metadata =
__ATTR_PREALLOC(metadata_version, S_IRUGO|S_IWUSR, metadata_show, metadata_store);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

static ssize_t
action_show(struct mddev *mddev, char *page)
{
	char *type = "idle";
<<<<<<< HEAD
	if (test_bit(MD_RECOVERY_FROZEN, &mddev->recovery))
		type = "frozen";
	else if (test_bit(MD_RECOVERY_RUNNING, &mddev->recovery) ||
	    (!mddev->ro && test_bit(MD_RECOVERY_NEEDED, &mddev->recovery))) {
		if (test_bit(MD_RECOVERY_RESHAPE, &mddev->recovery))
			type = "reshape";
		else if (test_bit(MD_RECOVERY_SYNC, &mddev->recovery)) {
			if (!test_bit(MD_RECOVERY_REQUESTED, &mddev->recovery))
				type = "resync";
			else if (test_bit(MD_RECOVERY_CHECK, &mddev->recovery))
				type = "check";
			else
				type = "repair";
		} else if (test_bit(MD_RECOVERY_RECOVER, &mddev->recovery))
			type = "recover";
=======
	unsigned long recovery = mddev->recovery;
	if (test_bit(MD_RECOVERY_FROZEN, &recovery))
		type = "frozen";
	else if (test_bit(MD_RECOVERY_RUNNING, &recovery) ||
	    (md_is_rdwr(mddev) && test_bit(MD_RECOVERY_NEEDED, &recovery))) {
		if (test_bit(MD_RECOVERY_RESHAPE, &recovery))
			type = "reshape";
		else if (test_bit(MD_RECOVERY_SYNC, &recovery)) {
			if (!test_bit(MD_RECOVERY_REQUESTED, &recovery))
				type = "resync";
			else if (test_bit(MD_RECOVERY_CHECK, &recovery))
				type = "check";
			else
				type = "repair";
		} else if (test_bit(MD_RECOVERY_RECOVER, &recovery))
			type = "recover";
		else if (mddev->reshape_position != MaxSector)
			type = "reshape";
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	}
	return sprintf(page, "%s\n", type);
}

<<<<<<< HEAD
static void reap_sync_thread(struct mddev *mddev);
=======
/**
 * stop_sync_thread() - wait for sync_thread to stop if it's running.
 * @mddev:	the array.
 * @locked:	if set, reconfig_mutex will still be held after this function
 *		return; if not set, reconfig_mutex will be released after this
 *		function return.
 * @check_seq:	if set, only wait for curent running sync_thread to stop, noted
 *		that new sync_thread can still start.
 */
static void stop_sync_thread(struct mddev *mddev, bool locked, bool check_seq)
{
	int sync_seq;

	if (check_seq)
		sync_seq = atomic_read(&mddev->sync_seq);

	if (!test_bit(MD_RECOVERY_RUNNING, &mddev->recovery)) {
		if (!locked)
			mddev_unlock(mddev);
		return;
	}

	mddev_unlock(mddev);

	set_bit(MD_RECOVERY_INTR, &mddev->recovery);
	/*
	 * Thread might be blocked waiting for metadata update which will now
	 * never happen
	 */
	md_wakeup_thread_directly(mddev->sync_thread);
	if (work_pending(&mddev->sync_work))
		flush_work(&mddev->sync_work);

	wait_event(resync_wait,
		   !test_bit(MD_RECOVERY_RUNNING, &mddev->recovery) ||
		   (check_seq && sync_seq != atomic_read(&mddev->sync_seq)));

	if (locked)
		mddev_lock_nointr(mddev);
}

void md_idle_sync_thread(struct mddev *mddev)
{
	lockdep_assert_held(&mddev->reconfig_mutex);

	clear_bit(MD_RECOVERY_FROZEN, &mddev->recovery);
	stop_sync_thread(mddev, true, true);
}
EXPORT_SYMBOL_GPL(md_idle_sync_thread);

void md_frozen_sync_thread(struct mddev *mddev)
{
	lockdep_assert_held(&mddev->reconfig_mutex);

	set_bit(MD_RECOVERY_FROZEN, &mddev->recovery);
	stop_sync_thread(mddev, true, false);
}
EXPORT_SYMBOL_GPL(md_frozen_sync_thread);

void md_unfrozen_sync_thread(struct mddev *mddev)
{
	lockdep_assert_held(&mddev->reconfig_mutex);

	clear_bit(MD_RECOVERY_FROZEN, &mddev->recovery);
	set_bit(MD_RECOVERY_NEEDED, &mddev->recovery);
	md_wakeup_thread(mddev->thread);
	sysfs_notify_dirent_safe(mddev->sysfs_action);
}
EXPORT_SYMBOL_GPL(md_unfrozen_sync_thread);

static void idle_sync_thread(struct mddev *mddev)
{
	mutex_lock(&mddev->sync_mutex);
	clear_bit(MD_RECOVERY_FROZEN, &mddev->recovery);

	if (mddev_lock(mddev)) {
		mutex_unlock(&mddev->sync_mutex);
		return;
	}

	stop_sync_thread(mddev, false, true);
	mutex_unlock(&mddev->sync_mutex);
}

static void frozen_sync_thread(struct mddev *mddev)
{
	mutex_lock(&mddev->sync_mutex);
	set_bit(MD_RECOVERY_FROZEN, &mddev->recovery);

	if (mddev_lock(mddev)) {
		mutex_unlock(&mddev->sync_mutex);
		return;
	}

	stop_sync_thread(mddev, false, false);
	mutex_unlock(&mddev->sync_mutex);
}
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

static ssize_t
action_store(struct mddev *mddev, const char *page, size_t len)
{
	if (!mddev->pers || !mddev->pers->sync_request)
		return -EINVAL;

<<<<<<< HEAD
	if (cmd_match(page, "frozen"))
		set_bit(MD_RECOVERY_FROZEN, &mddev->recovery);
	else
		clear_bit(MD_RECOVERY_FROZEN, &mddev->recovery);

	if (cmd_match(page, "idle") || cmd_match(page, "frozen")) {
		if (mddev->sync_thread) {
			set_bit(MD_RECOVERY_INTR, &mddev->recovery);
			reap_sync_thread(mddev);
		}
	} else if (test_bit(MD_RECOVERY_RUNNING, &mddev->recovery) ||
		   test_bit(MD_RECOVERY_NEEDED, &mddev->recovery))
		return -EBUSY;
	else if (cmd_match(page, "resync"))
		set_bit(MD_RECOVERY_NEEDED, &mddev->recovery);
	else if (cmd_match(page, "recover")) {
		set_bit(MD_RECOVERY_RECOVER, &mddev->recovery);
		set_bit(MD_RECOVERY_NEEDED, &mddev->recovery);
=======

	if (cmd_match(page, "idle"))
		idle_sync_thread(mddev);
	else if (cmd_match(page, "frozen"))
		frozen_sync_thread(mddev);
	else if (test_bit(MD_RECOVERY_RUNNING, &mddev->recovery))
		return -EBUSY;
	else if (cmd_match(page, "resync"))
		clear_bit(MD_RECOVERY_FROZEN, &mddev->recovery);
	else if (cmd_match(page, "recover")) {
		clear_bit(MD_RECOVERY_FROZEN, &mddev->recovery);
		set_bit(MD_RECOVERY_RECOVER, &mddev->recovery);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	} else if (cmd_match(page, "reshape")) {
		int err;
		if (mddev->pers->start_reshape == NULL)
			return -EINVAL;
<<<<<<< HEAD
		err = mddev->pers->start_reshape(mddev);
		if (err)
			return err;
		sysfs_notify(&mddev->kobj, NULL, "degraded");
=======
		err = mddev_lock(mddev);
		if (!err) {
			if (test_bit(MD_RECOVERY_RUNNING, &mddev->recovery)) {
				err =  -EBUSY;
			} else if (mddev->reshape_position == MaxSector ||
				   mddev->pers->check_reshape == NULL ||
				   mddev->pers->check_reshape(mddev)) {
				clear_bit(MD_RECOVERY_FROZEN, &mddev->recovery);
				err = mddev->pers->start_reshape(mddev);
			} else {
				/*
				 * If reshape is still in progress, and
				 * md_check_recovery() can continue to reshape,
				 * don't restart reshape because data can be
				 * corrupted for raid456.
				 */
				clear_bit(MD_RECOVERY_FROZEN, &mddev->recovery);
			}
			mddev_unlock(mddev);
		}
		if (err)
			return err;
		sysfs_notify_dirent_safe(mddev->sysfs_degraded);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	} else {
		if (cmd_match(page, "check"))
			set_bit(MD_RECOVERY_CHECK, &mddev->recovery);
		else if (!cmd_match(page, "repair"))
			return -EINVAL;
<<<<<<< HEAD
		set_bit(MD_RECOVERY_REQUESTED, &mddev->recovery);
		set_bit(MD_RECOVERY_SYNC, &mddev->recovery);
	}
=======
		clear_bit(MD_RECOVERY_FROZEN, &mddev->recovery);
		set_bit(MD_RECOVERY_REQUESTED, &mddev->recovery);
		set_bit(MD_RECOVERY_SYNC, &mddev->recovery);
	}
	if (mddev->ro == MD_AUTO_READ) {
		/* A write to sync_action is enough to justify
		 * canceling read-auto mode
		 */
		flush_work(&mddev->sync_work);
		mddev->ro = MD_RDWR;
		md_wakeup_thread(mddev->sync_thread);
	}
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	set_bit(MD_RECOVERY_NEEDED, &mddev->recovery);
	md_wakeup_thread(mddev->thread);
	sysfs_notify_dirent_safe(mddev->sysfs_action);
	return len;
}

<<<<<<< HEAD
=======
static struct md_sysfs_entry md_scan_mode =
__ATTR_PREALLOC(sync_action, S_IRUGO|S_IWUSR, action_show, action_store);

static ssize_t
last_sync_action_show(struct mddev *mddev, char *page)
{
	return sprintf(page, "%s\n", mddev->last_sync_action);
}

static struct md_sysfs_entry md_last_scan_mode = __ATTR_RO(last_sync_action);

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
static ssize_t
mismatch_cnt_show(struct mddev *mddev, char *page)
{
	return sprintf(page, "%llu\n",
<<<<<<< HEAD
		       (unsigned long long) mddev->resync_mismatches);
}

static struct md_sysfs_entry md_scan_mode =
__ATTR(sync_action, S_IRUGO|S_IWUSR, action_show, action_store);


=======
		       (unsigned long long)
		       atomic64_read(&mddev->resync_mismatches));
}

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
static struct md_sysfs_entry md_mismatches = __ATTR_RO(mismatch_cnt);

static ssize_t
sync_min_show(struct mddev *mddev, char *page)
{
	return sprintf(page, "%d (%s)\n", speed_min(mddev),
		       mddev->sync_speed_min ? "local": "system");
}

static ssize_t
sync_min_store(struct mddev *mddev, const char *buf, size_t len)
{
<<<<<<< HEAD
	int min;
	char *e;
	if (strncmp(buf, "system", 6)==0) {
		mddev->sync_speed_min = 0;
		return len;
	}
	min = simple_strtoul(buf, &e, 10);
	if (buf == e || (*e && *e != '\n') || min <= 0)
		return -EINVAL;
=======
	unsigned int min;
	int rv;

	if (strncmp(buf, "system", 6)==0) {
		min = 0;
	} else {
		rv = kstrtouint(buf, 10, &min);
		if (rv < 0)
			return rv;
		if (min == 0)
			return -EINVAL;
	}
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	mddev->sync_speed_min = min;
	return len;
}

static struct md_sysfs_entry md_sync_min =
__ATTR(sync_speed_min, S_IRUGO|S_IWUSR, sync_min_show, sync_min_store);

static ssize_t
sync_max_show(struct mddev *mddev, char *page)
{
	return sprintf(page, "%d (%s)\n", speed_max(mddev),
		       mddev->sync_speed_max ? "local": "system");
}

static ssize_t
sync_max_store(struct mddev *mddev, const char *buf, size_t len)
{
<<<<<<< HEAD
	int max;
	char *e;
	if (strncmp(buf, "system", 6)==0) {
		mddev->sync_speed_max = 0;
		return len;
	}
	max = simple_strtoul(buf, &e, 10);
	if (buf == e || (*e && *e != '\n') || max <= 0)
		return -EINVAL;
=======
	unsigned int max;
	int rv;

	if (strncmp(buf, "system", 6)==0) {
		max = 0;
	} else {
		rv = kstrtouint(buf, 10, &max);
		if (rv < 0)
			return rv;
		if (max == 0)
			return -EINVAL;
	}
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	mddev->sync_speed_max = max;
	return len;
}

static struct md_sysfs_entry md_sync_max =
__ATTR(sync_speed_max, S_IRUGO|S_IWUSR, sync_max_show, sync_max_store);

static ssize_t
degraded_show(struct mddev *mddev, char *page)
{
	return sprintf(page, "%d\n", mddev->degraded);
}
static struct md_sysfs_entry md_degraded = __ATTR_RO(degraded);

static ssize_t
sync_force_parallel_show(struct mddev *mddev, char *page)
{
	return sprintf(page, "%d\n", mddev->parallel_resync);
}

static ssize_t
sync_force_parallel_store(struct mddev *mddev, const char *buf, size_t len)
{
	long n;

<<<<<<< HEAD
	if (strict_strtol(buf, 10, &n))
=======
	if (kstrtol(buf, 10, &n))
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		return -EINVAL;

	if (n != 0 && n != 1)
		return -EINVAL;

	mddev->parallel_resync = n;

	if (mddev->sync_thread)
		wake_up(&resync_wait);

	return len;
}

/* force parallel resync, even with shared block devices */
static struct md_sysfs_entry md_sync_force_parallel =
__ATTR(sync_force_parallel, S_IRUGO|S_IWUSR,
       sync_force_parallel_show, sync_force_parallel_store);

static ssize_t
sync_speed_show(struct mddev *mddev, char *page)
{
	unsigned long resync, dt, db;
<<<<<<< HEAD
	if (mddev->curr_resync == 0)
=======
	if (mddev->curr_resync == MD_RESYNC_NONE)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		return sprintf(page, "none\n");
	resync = mddev->curr_mark_cnt - atomic_read(&mddev->recovery_active);
	dt = (jiffies - mddev->resync_mark) / HZ;
	if (!dt) dt++;
	db = resync - mddev->resync_mark_cnt;
	return sprintf(page, "%lu\n", db/dt/2); /* K/sec */
}

static struct md_sysfs_entry md_sync_speed = __ATTR_RO(sync_speed);

static ssize_t
sync_completed_show(struct mddev *mddev, char *page)
{
	unsigned long long max_sectors, resync;

	if (!test_bit(MD_RECOVERY_RUNNING, &mddev->recovery))
		return sprintf(page, "none\n");

<<<<<<< HEAD
	if (test_bit(MD_RECOVERY_SYNC, &mddev->recovery))
=======
	if (mddev->curr_resync == MD_RESYNC_YIELDED ||
	    mddev->curr_resync == MD_RESYNC_DELAYED)
		return sprintf(page, "delayed\n");

	if (test_bit(MD_RECOVERY_SYNC, &mddev->recovery) ||
	    test_bit(MD_RECOVERY_RESHAPE, &mddev->recovery))
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		max_sectors = mddev->resync_max_sectors;
	else
		max_sectors = mddev->dev_sectors;

	resync = mddev->curr_resync_completed;
	return sprintf(page, "%llu / %llu\n", resync, max_sectors);
}

<<<<<<< HEAD
static struct md_sysfs_entry md_sync_completed = __ATTR_RO(sync_completed);
=======
static struct md_sysfs_entry md_sync_completed =
	__ATTR_PREALLOC(sync_completed, S_IRUGO, sync_completed_show, NULL);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

static ssize_t
min_sync_show(struct mddev *mddev, char *page)
{
	return sprintf(page, "%llu\n",
		       (unsigned long long)mddev->resync_min);
}
static ssize_t
min_sync_store(struct mddev *mddev, const char *buf, size_t len)
{
	unsigned long long min;
<<<<<<< HEAD
	if (strict_strtoull(buf, 10, &min))
		return -EINVAL;
	if (min > mddev->resync_max)
		return -EINVAL;
	if (test_bit(MD_RECOVERY_RUNNING, &mddev->recovery))
		return -EBUSY;

	/* Must be a multiple of chunk_size */
	if (mddev->chunk_sectors) {
		sector_t temp = min;
		if (sector_div(temp, mddev->chunk_sectors))
			return -EINVAL;
	}
	mddev->resync_min = min;

	return len;
=======
	int err;

	if (kstrtoull(buf, 10, &min))
		return -EINVAL;

	spin_lock(&mddev->lock);
	err = -EINVAL;
	if (min > mddev->resync_max)
		goto out_unlock;

	err = -EBUSY;
	if (test_bit(MD_RECOVERY_RUNNING, &mddev->recovery))
		goto out_unlock;

	/* Round down to multiple of 4K for safety */
	mddev->resync_min = round_down(min, 8);
	err = 0;

out_unlock:
	spin_unlock(&mddev->lock);
	return err ?: len;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

static struct md_sysfs_entry md_min_sync =
__ATTR(sync_min, S_IRUGO|S_IWUSR, min_sync_show, min_sync_store);

static ssize_t
max_sync_show(struct mddev *mddev, char *page)
{
	if (mddev->resync_max == MaxSector)
		return sprintf(page, "max\n");
	else
		return sprintf(page, "%llu\n",
			       (unsigned long long)mddev->resync_max);
}
static ssize_t
max_sync_store(struct mddev *mddev, const char *buf, size_t len)
{
<<<<<<< HEAD
=======
	int err;
	spin_lock(&mddev->lock);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	if (strncmp(buf, "max", 3) == 0)
		mddev->resync_max = MaxSector;
	else {
		unsigned long long max;
<<<<<<< HEAD
		if (strict_strtoull(buf, 10, &max))
			return -EINVAL;
		if (max < mddev->resync_min)
			return -EINVAL;
		if (max < mddev->resync_max &&
		    mddev->ro == 0 &&
		    test_bit(MD_RECOVERY_RUNNING, &mddev->recovery))
			return -EBUSY;

		/* Must be a multiple of chunk_size */
		if (mddev->chunk_sectors) {
			sector_t temp = max;
			if (sector_div(temp, mddev->chunk_sectors))
				return -EINVAL;
=======
		int chunk;

		err = -EINVAL;
		if (kstrtoull(buf, 10, &max))
			goto out_unlock;
		if (max < mddev->resync_min)
			goto out_unlock;

		err = -EBUSY;
		if (max < mddev->resync_max && md_is_rdwr(mddev) &&
		    test_bit(MD_RECOVERY_RUNNING, &mddev->recovery))
			goto out_unlock;

		/* Must be a multiple of chunk_size */
		chunk = mddev->chunk_sectors;
		if (chunk) {
			sector_t temp = max;

			err = -EINVAL;
			if (sector_div(temp, chunk))
				goto out_unlock;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		}
		mddev->resync_max = max;
	}
	wake_up(&mddev->recovery_wait);
<<<<<<< HEAD
	return len;
=======
	err = 0;
out_unlock:
	spin_unlock(&mddev->lock);
	return err ?: len;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

static struct md_sysfs_entry md_max_sync =
__ATTR(sync_max, S_IRUGO|S_IWUSR, max_sync_show, max_sync_store);

static ssize_t
suspend_lo_show(struct mddev *mddev, char *page)
{
<<<<<<< HEAD
	return sprintf(page, "%llu\n", (unsigned long long)mddev->suspend_lo);
=======
	return sprintf(page, "%llu\n",
		       (unsigned long long)READ_ONCE(mddev->suspend_lo));
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

static ssize_t
suspend_lo_store(struct mddev *mddev, const char *buf, size_t len)
{
<<<<<<< HEAD
	char *e;
	unsigned long long new = simple_strtoull(buf, &e, 10);
	unsigned long long old = mddev->suspend_lo;

	if (mddev->pers == NULL || 
	    mddev->pers->quiesce == NULL)
		return -EINVAL;
	if (buf == e || (*e && *e != '\n'))
		return -EINVAL;

	mddev->suspend_lo = new;
	if (new >= old)
		/* Shrinking suspended region */
		mddev->pers->quiesce(mddev, 2);
	else {
		/* Expanding suspended region - need to wait */
		mddev->pers->quiesce(mddev, 1);
		mddev->pers->quiesce(mddev, 0);
	}
=======
	unsigned long long new;
	int err;

	err = kstrtoull(buf, 10, &new);
	if (err < 0)
		return err;
	if (new != (sector_t)new)
		return -EINVAL;

	err = mddev_suspend(mddev, true);
	if (err)
		return err;

	WRITE_ONCE(mddev->suspend_lo, new);
	mddev_resume(mddev);

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	return len;
}
static struct md_sysfs_entry md_suspend_lo =
__ATTR(suspend_lo, S_IRUGO|S_IWUSR, suspend_lo_show, suspend_lo_store);

<<<<<<< HEAD

static ssize_t
suspend_hi_show(struct mddev *mddev, char *page)
{
	return sprintf(page, "%llu\n", (unsigned long long)mddev->suspend_hi);
=======
static ssize_t
suspend_hi_show(struct mddev *mddev, char *page)
{
	return sprintf(page, "%llu\n",
		       (unsigned long long)READ_ONCE(mddev->suspend_hi));
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

static ssize_t
suspend_hi_store(struct mddev *mddev, const char *buf, size_t len)
{
<<<<<<< HEAD
	char *e;
	unsigned long long new = simple_strtoull(buf, &e, 10);
	unsigned long long old = mddev->suspend_hi;

	if (mddev->pers == NULL ||
	    mddev->pers->quiesce == NULL)
		return -EINVAL;
	if (buf == e || (*e && *e != '\n'))
		return -EINVAL;

	mddev->suspend_hi = new;
	if (new <= old)
		/* Shrinking suspended region */
		mddev->pers->quiesce(mddev, 2);
	else {
		/* Expanding suspended region - need to wait */
		mddev->pers->quiesce(mddev, 1);
		mddev->pers->quiesce(mddev, 0);
	}
=======
	unsigned long long new;
	int err;

	err = kstrtoull(buf, 10, &new);
	if (err < 0)
		return err;
	if (new != (sector_t)new)
		return -EINVAL;

	err = mddev_suspend(mddev, true);
	if (err)
		return err;

	WRITE_ONCE(mddev->suspend_hi, new);
	mddev_resume(mddev);

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	return len;
}
static struct md_sysfs_entry md_suspend_hi =
__ATTR(suspend_hi, S_IRUGO|S_IWUSR, suspend_hi_show, suspend_hi_store);

static ssize_t
reshape_position_show(struct mddev *mddev, char *page)
{
	if (mddev->reshape_position != MaxSector)
		return sprintf(page, "%llu\n",
			       (unsigned long long)mddev->reshape_position);
	strcpy(page, "none\n");
	return 5;
}

static ssize_t
reshape_position_store(struct mddev *mddev, const char *buf, size_t len)
{
<<<<<<< HEAD
	char *e;
	unsigned long long new = simple_strtoull(buf, &e, 10);
	if (mddev->pers)
		return -EBUSY;
	if (buf == e || (*e && *e != '\n'))
		return -EINVAL;
	mddev->reshape_position = new;
	mddev->delta_disks = 0;
	mddev->new_level = mddev->level;
	mddev->new_layout = mddev->layout;
	mddev->new_chunk_sectors = mddev->chunk_sectors;
	return len;
=======
	struct md_rdev *rdev;
	unsigned long long new;
	int err;

	err = kstrtoull(buf, 10, &new);
	if (err < 0)
		return err;
	if (new != (sector_t)new)
		return -EINVAL;
	err = mddev_lock(mddev);
	if (err)
		return err;
	err = -EBUSY;
	if (mddev->pers)
		goto unlock;
	mddev->reshape_position = new;
	mddev->delta_disks = 0;
	mddev->reshape_backwards = 0;
	mddev->new_level = mddev->level;
	mddev->new_layout = mddev->layout;
	mddev->new_chunk_sectors = mddev->chunk_sectors;
	rdev_for_each(rdev, mddev)
		rdev->new_data_offset = rdev->data_offset;
	err = 0;
unlock:
	mddev_unlock(mddev);
	return err ?: len;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

static struct md_sysfs_entry md_reshape_position =
__ATTR(reshape_position, S_IRUGO|S_IWUSR, reshape_position_show,
       reshape_position_store);

static ssize_t
<<<<<<< HEAD
=======
reshape_direction_show(struct mddev *mddev, char *page)
{
	return sprintf(page, "%s\n",
		       mddev->reshape_backwards ? "backwards" : "forwards");
}

static ssize_t
reshape_direction_store(struct mddev *mddev, const char *buf, size_t len)
{
	int backwards = 0;
	int err;

	if (cmd_match(buf, "forwards"))
		backwards = 0;
	else if (cmd_match(buf, "backwards"))
		backwards = 1;
	else
		return -EINVAL;
	if (mddev->reshape_backwards == backwards)
		return len;

	err = mddev_lock(mddev);
	if (err)
		return err;
	/* check if we are allowed to change */
	if (mddev->delta_disks)
		err = -EBUSY;
	else if (mddev->persistent &&
	    mddev->major_version == 0)
		err =  -EINVAL;
	else
		mddev->reshape_backwards = backwards;
	mddev_unlock(mddev);
	return err ?: len;
}

static struct md_sysfs_entry md_reshape_direction =
__ATTR(reshape_direction, S_IRUGO|S_IWUSR, reshape_direction_show,
       reshape_direction_store);

static ssize_t
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
array_size_show(struct mddev *mddev, char *page)
{
	if (mddev->external_size)
		return sprintf(page, "%llu\n",
			       (unsigned long long)mddev->array_sectors/2);
	else
		return sprintf(page, "default\n");
}

static ssize_t
array_size_store(struct mddev *mddev, const char *buf, size_t len)
{
	sector_t sectors;
<<<<<<< HEAD
=======
	int err;

	err = mddev_lock(mddev);
	if (err)
		return err;

	/* cluster raid doesn't support change array_sectors */
	if (mddev_is_clustered(mddev)) {
		mddev_unlock(mddev);
		return -EINVAL;
	}
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	if (strncmp(buf, "default", 7) == 0) {
		if (mddev->pers)
			sectors = mddev->pers->size(mddev, 0, 0);
		else
			sectors = mddev->array_sectors;

		mddev->external_size = 0;
	} else {
		if (strict_blocks_to_sectors(buf, &sectors) < 0)
<<<<<<< HEAD
			return -EINVAL;
		if (mddev->pers && mddev->pers->size(mddev, 0, 0) < sectors)
			return -E2BIG;

		mddev->external_size = 1;
	}

	mddev->array_sectors = sectors;
	if (mddev->pers) {
		set_capacity(mddev->gendisk, mddev->array_sectors);
		revalidate_disk(mddev->gendisk);
	}
	return len;
=======
			err = -EINVAL;
		else if (mddev->pers && mddev->pers->size(mddev, 0, 0) < sectors)
			err = -E2BIG;
		else
			mddev->external_size = 1;
	}

	if (!err) {
		mddev->array_sectors = sectors;
		if (mddev->pers)
			set_capacity_and_notify(mddev->gendisk,
						mddev->array_sectors);
	}
	mddev_unlock(mddev);
	return err ?: len;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

static struct md_sysfs_entry md_array_size =
__ATTR(array_size, S_IRUGO|S_IWUSR, array_size_show,
       array_size_store);

<<<<<<< HEAD
=======
static ssize_t
consistency_policy_show(struct mddev *mddev, char *page)
{
	int ret;

	if (test_bit(MD_HAS_JOURNAL, &mddev->flags)) {
		ret = sprintf(page, "journal\n");
	} else if (test_bit(MD_HAS_PPL, &mddev->flags)) {
		ret = sprintf(page, "ppl\n");
	} else if (mddev->bitmap) {
		ret = sprintf(page, "bitmap\n");
	} else if (mddev->pers) {
		if (mddev->pers->sync_request)
			ret = sprintf(page, "resync\n");
		else
			ret = sprintf(page, "none\n");
	} else {
		ret = sprintf(page, "unknown\n");
	}

	return ret;
}

static ssize_t
consistency_policy_store(struct mddev *mddev, const char *buf, size_t len)
{
	int err = 0;

	if (mddev->pers) {
		if (mddev->pers->change_consistency_policy)
			err = mddev->pers->change_consistency_policy(mddev, buf);
		else
			err = -EBUSY;
	} else if (mddev->external && strncmp(buf, "ppl", 3) == 0) {
		set_bit(MD_HAS_PPL, &mddev->flags);
	} else {
		err = -EINVAL;
	}

	return err ? err : len;
}

static struct md_sysfs_entry md_consistency_policy =
__ATTR(consistency_policy, S_IRUGO | S_IWUSR, consistency_policy_show,
       consistency_policy_store);

static ssize_t fail_last_dev_show(struct mddev *mddev, char *page)
{
	return sprintf(page, "%d\n", mddev->fail_last_dev);
}

/*
 * Setting fail_last_dev to true to allow last device to be forcibly removed
 * from RAID1/RAID10.
 */
static ssize_t
fail_last_dev_store(struct mddev *mddev, const char *buf, size_t len)
{
	int ret;
	bool value;

	ret = kstrtobool(buf, &value);
	if (ret)
		return ret;

	if (value != mddev->fail_last_dev)
		mddev->fail_last_dev = value;

	return len;
}
static struct md_sysfs_entry md_fail_last_dev =
__ATTR(fail_last_dev, S_IRUGO | S_IWUSR, fail_last_dev_show,
       fail_last_dev_store);

static ssize_t serialize_policy_show(struct mddev *mddev, char *page)
{
	if (mddev->pers == NULL || (mddev->pers->level != 1))
		return sprintf(page, "n/a\n");
	else
		return sprintf(page, "%d\n", mddev->serialize_policy);
}

/*
 * Setting serialize_policy to true to enforce write IO is not reordered
 * for raid1.
 */
static ssize_t
serialize_policy_store(struct mddev *mddev, const char *buf, size_t len)
{
	int err;
	bool value;

	err = kstrtobool(buf, &value);
	if (err)
		return err;

	if (value == mddev->serialize_policy)
		return len;

	err = mddev_suspend_and_lock(mddev);
	if (err)
		return err;
	if (mddev->pers == NULL || (mddev->pers->level != 1)) {
		pr_err("md: serialize_policy is only effective for raid1\n");
		err = -EINVAL;
		goto unlock;
	}

	if (value)
		mddev_create_serial_pool(mddev, NULL);
	else
		mddev_destroy_serial_pool(mddev, NULL);
	mddev->serialize_policy = value;
unlock:
	mddev_unlock_and_resume(mddev);
	return err ?: len;
}

static struct md_sysfs_entry md_serialize_policy =
__ATTR(serialize_policy, S_IRUGO | S_IWUSR, serialize_policy_show,
       serialize_policy_store);


>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
static struct attribute *md_default_attrs[] = {
	&md_level.attr,
	&md_layout.attr,
	&md_raid_disks.attr,
<<<<<<< HEAD
=======
	&md_uuid.attr,
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	&md_chunk_size.attr,
	&md_size.attr,
	&md_resync_start.attr,
	&md_metadata.attr,
	&md_new_device.attr,
	&md_safe_delay.attr,
	&md_array_state.attr,
	&md_reshape_position.attr,
<<<<<<< HEAD
	&md_array_size.attr,
	&max_corr_read_errors.attr,
	NULL,
};

static struct attribute *md_redundancy_attrs[] = {
	&md_scan_mode.attr,
=======
	&md_reshape_direction.attr,
	&md_array_size.attr,
	&max_corr_read_errors.attr,
	&md_consistency_policy.attr,
	&md_fail_last_dev.attr,
	&md_serialize_policy.attr,
	NULL,
};

static const struct attribute_group md_default_group = {
	.attrs = md_default_attrs,
};

static struct attribute *md_redundancy_attrs[] = {
	&md_scan_mode.attr,
	&md_last_scan_mode.attr,
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	&md_mismatches.attr,
	&md_sync_min.attr,
	&md_sync_max.attr,
	&md_sync_speed.attr,
	&md_sync_force_parallel.attr,
	&md_sync_completed.attr,
	&md_min_sync.attr,
	&md_max_sync.attr,
	&md_suspend_lo.attr,
	&md_suspend_hi.attr,
	&md_bitmap.attr,
	&md_degraded.attr,
	NULL,
};
<<<<<<< HEAD
static struct attribute_group md_redundancy_group = {
=======
static const struct attribute_group md_redundancy_group = {
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	.name = NULL,
	.attrs = md_redundancy_attrs,
};

<<<<<<< HEAD
=======
static const struct attribute_group *md_attr_groups[] = {
	&md_default_group,
	&md_bitmap_group,
	NULL,
};
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

static ssize_t
md_attr_show(struct kobject *kobj, struct attribute *attr, char *page)
{
	struct md_sysfs_entry *entry = container_of(attr, struct md_sysfs_entry, attr);
	struct mddev *mddev = container_of(kobj, struct mddev, kobj);
	ssize_t rv;

	if (!entry->show)
		return -EIO;
	spin_lock(&all_mddevs_lock);
<<<<<<< HEAD
	if (list_empty(&mddev->all_mddevs)) {
		spin_unlock(&all_mddevs_lock);
		return -EBUSY;
	}
	mddev_get(mddev);
	spin_unlock(&all_mddevs_lock);

	rv = mddev_lock(mddev);
	if (!rv) {
		rv = entry->show(mddev, page);
		mddev_unlock(mddev);
	}
=======
	if (!mddev_get(mddev)) {
		spin_unlock(&all_mddevs_lock);
		return -EBUSY;
	}
	spin_unlock(&all_mddevs_lock);

	rv = entry->show(mddev, page);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	mddev_put(mddev);
	return rv;
}

static ssize_t
md_attr_store(struct kobject *kobj, struct attribute *attr,
	      const char *page, size_t length)
{
	struct md_sysfs_entry *entry = container_of(attr, struct md_sysfs_entry, attr);
	struct mddev *mddev = container_of(kobj, struct mddev, kobj);
	ssize_t rv;

	if (!entry->store)
		return -EIO;
	if (!capable(CAP_SYS_ADMIN))
		return -EACCES;
	spin_lock(&all_mddevs_lock);
<<<<<<< HEAD
	if (list_empty(&mddev->all_mddevs)) {
		spin_unlock(&all_mddevs_lock);
		return -EBUSY;
	}
	mddev_get(mddev);
	spin_unlock(&all_mddevs_lock);
	rv = mddev_lock(mddev);
	if (!rv) {
		rv = entry->store(mddev, page, length);
		mddev_unlock(mddev);
	}
=======
	if (!mddev_get(mddev)) {
		spin_unlock(&all_mddevs_lock);
		return -EBUSY;
	}
	spin_unlock(&all_mddevs_lock);
	rv = entry->store(mddev, page, length);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	mddev_put(mddev);
	return rv;
}

<<<<<<< HEAD
static void md_free(struct kobject *ko)
=======
static void md_kobj_release(struct kobject *ko)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
{
	struct mddev *mddev = container_of(ko, struct mddev, kobj);

	if (mddev->sysfs_state)
		sysfs_put(mddev->sysfs_state);
<<<<<<< HEAD

	if (mddev->gendisk) {
		del_gendisk(mddev->gendisk);
		put_disk(mddev->gendisk);
	}
	if (mddev->queue)
		blk_cleanup_queue(mddev->queue);

	kfree(mddev);
=======
	if (mddev->sysfs_level)
		sysfs_put(mddev->sysfs_level);

	del_gendisk(mddev->gendisk);
	put_disk(mddev->gendisk);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

static const struct sysfs_ops md_sysfs_ops = {
	.show	= md_attr_show,
	.store	= md_attr_store,
};
<<<<<<< HEAD
static struct kobj_type md_ktype = {
	.release	= md_free,
	.sysfs_ops	= &md_sysfs_ops,
	.default_attrs	= md_default_attrs,
=======
static const struct kobj_type md_ktype = {
	.release	= md_kobj_release,
	.sysfs_ops	= &md_sysfs_ops,
	.default_groups	= md_attr_groups,
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
};

int mdp_major = 0;

<<<<<<< HEAD
=======
/* stack the limit for all rdevs into lim */
void mddev_stack_rdev_limits(struct mddev *mddev, struct queue_limits *lim)
{
	struct md_rdev *rdev;

	rdev_for_each(rdev, mddev) {
		queue_limits_stack_bdev(lim, rdev->bdev, rdev->data_offset,
					mddev->gendisk->disk_name);
	}
}
EXPORT_SYMBOL_GPL(mddev_stack_rdev_limits);

/* apply the extra stacking limits from a new rdev into mddev */
int mddev_stack_new_rdev(struct mddev *mddev, struct md_rdev *rdev)
{
	struct queue_limits lim;

	if (mddev_is_dm(mddev))
		return 0;

	lim = queue_limits_start_update(mddev->gendisk->queue);
	queue_limits_stack_bdev(&lim, rdev->bdev, rdev->data_offset,
				mddev->gendisk->disk_name);
	return queue_limits_commit_update(mddev->gendisk->queue, &lim);
}
EXPORT_SYMBOL_GPL(mddev_stack_new_rdev);

/* update the optimal I/O size after a reshape */
void mddev_update_io_opt(struct mddev *mddev, unsigned int nr_stripes)
{
	struct queue_limits lim;

	if (mddev_is_dm(mddev))
		return;

	/* don't bother updating io_opt if we can't suspend the array */
	if (mddev_suspend(mddev, false) < 0)
		return;
	lim = queue_limits_start_update(mddev->gendisk->queue);
	lim.io_opt = lim.io_min * nr_stripes;
	queue_limits_commit_update(mddev->gendisk->queue, &lim);
	mddev_resume(mddev);
}
EXPORT_SYMBOL_GPL(mddev_update_io_opt);

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
static void mddev_delayed_delete(struct work_struct *ws)
{
	struct mddev *mddev = container_of(ws, struct mddev, del_work);

<<<<<<< HEAD
	sysfs_remove_group(&mddev->kobj, &md_bitmap_group);
	kobject_del(&mddev->kobj);
	kobject_put(&mddev->kobj);
}

static int md_alloc(dev_t dev, char *name)
{
	static DEFINE_MUTEX(disks_mutex);
	struct mddev *mddev = mddev_find(dev);
=======
	kobject_put(&mddev->kobj);
}

struct mddev *md_alloc(dev_t dev, char *name)
{
	/*
	 * If dev is zero, name is the name of a device to allocate with
	 * an arbitrary minor number.  It will be "md_???"
	 * If dev is non-zero it must be a device number with a MAJOR of
	 * MD_MAJOR or mdp_major.  In this case, if "name" is NULL, then
	 * the device is being created by opening a node in /dev.
	 * If "name" is not NULL, the device is being created by
	 * writing to /sys/module/md_mod/parameters/new_array.
	 */
	static DEFINE_MUTEX(disks_mutex);
	struct mddev *mddev;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	struct gendisk *disk;
	int partitioned;
	int shift;
	int unit;
<<<<<<< HEAD
	int error;

	if (!mddev)
		return -ENODEV;
=======
	int error ;

	/*
	 * Wait for any previous instance of this device to be completely
	 * removed (mddev_delayed_delete).
	 */
	flush_workqueue(md_misc_wq);

	mutex_lock(&disks_mutex);
	mddev = mddev_alloc(dev);
	if (IS_ERR(mddev)) {
		error = PTR_ERR(mddev);
		goto out_unlock;
	}
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	partitioned = (MAJOR(mddev->unit) != MD_MAJOR);
	shift = partitioned ? MdpMinorShift : 0;
	unit = MINOR(mddev->unit) >> shift;

<<<<<<< HEAD
	/* wait for any previous instance of this device to be
	 * completely removed (mddev_delayed_delete).
	 */
	flush_workqueue(md_misc_wq);

	mutex_lock(&disks_mutex);
	error = -EEXIST;
	if (mddev->gendisk)
		goto abort;

	if (name) {
=======
	if (name && !dev) {
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		/* Need to ensure that 'name' is not a duplicate.
		 */
		struct mddev *mddev2;
		spin_lock(&all_mddevs_lock);

		list_for_each_entry(mddev2, &all_mddevs, all_mddevs)
			if (mddev2->gendisk &&
			    strcmp(mddev2->gendisk->disk_name, name) == 0) {
				spin_unlock(&all_mddevs_lock);
<<<<<<< HEAD
				goto abort;
			}
		spin_unlock(&all_mddevs_lock);
	}

	error = -ENOMEM;
	mddev->queue = blk_alloc_queue(GFP_KERNEL);
	if (!mddev->queue)
		goto abort;
	mddev->queue->queuedata = mddev;

	blk_queue_make_request(mddev->queue, md_make_request);
	blk_set_stacking_limits(&mddev->queue->limits);

	disk = alloc_disk(1 << shift);
	if (!disk) {
		blk_cleanup_queue(mddev->queue);
		mddev->queue = NULL;
		goto abort;
	}
	disk->major = MAJOR(mddev->unit);
	disk->first_minor = unit << shift;
=======
				error = -EEXIST;
				goto out_free_mddev;
			}
		spin_unlock(&all_mddevs_lock);
	}
	if (name && dev)
		/*
		 * Creating /dev/mdNNN via "newarray", so adjust hold_active.
		 */
		mddev->hold_active = UNTIL_STOP;

	disk = blk_alloc_disk(NULL, NUMA_NO_NODE);
	if (IS_ERR(disk)) {
		error = PTR_ERR(disk);
		goto out_free_mddev;
	}

	disk->major = MAJOR(mddev->unit);
	disk->first_minor = unit << shift;
	disk->minors = 1 << shift;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	if (name)
		strcpy(disk->disk_name, name);
	else if (partitioned)
		sprintf(disk->disk_name, "md_d%d", unit);
	else
		sprintf(disk->disk_name, "md%d", unit);
	disk->fops = &md_fops;
	disk->private_data = mddev;
<<<<<<< HEAD
	disk->queue = mddev->queue;
	blk_queue_flush(mddev->queue, REQ_FLUSH | REQ_FUA);
	/* Allow extended partitions.  This makes the
	 * 'mdp' device redundant, but we can't really
	 * remove it now.
	 */
	disk->flags |= GENHD_FL_EXT_DEVT;
	mddev->gendisk = disk;
	/* As soon as we call add_disk(), another thread could get
	 * through to md_open, so make sure it doesn't get too far
	 */
	mutex_lock(&mddev->open_mutex);
	add_disk(disk);

	error = kobject_init_and_add(&mddev->kobj, &md_ktype,
				     &disk_to_dev(disk)->kobj, "%s", "md");
	if (error) {
		/* This isn't possible, but as kobject_init_and_add is marked
		 * __must_check, we must do something with the result
		 */
		printk(KERN_WARNING "md: cannot register %s/md - name in use\n",
		       disk->disk_name);
		error = 0;
	}
	if (mddev->kobj.sd &&
	    sysfs_create_group(&mddev->kobj, &md_bitmap_group))
		printk(KERN_DEBUG "pointless warning\n");
	mutex_unlock(&mddev->open_mutex);
 abort:
	mutex_unlock(&disks_mutex);
	if (!error && mddev->kobj.sd) {
		kobject_uevent(&mddev->kobj, KOBJ_ADD);
		mddev->sysfs_state = sysfs_get_dirent_safe(mddev->kobj.sd, "array_state");
	}
	mddev_put(mddev);
	return error;
}

static struct kobject *md_probe(dev_t dev, int *part, void *data)
{
	md_alloc(dev, NULL);
	return NULL;
}

static int add_named_array(const char *val, struct kernel_param *kp)
{
	/* val must be "md_*" where * is not all digits.
	 * We allocate an array with a large free minor number, and
	 * set the name to val.  val must not already be an active name.
	 */
	int len = strlen(val);
	char buf[DISK_NAME_LEN];
=======

	blk_queue_write_cache(disk->queue, true, true);
	disk->events |= DISK_EVENT_MEDIA_CHANGE;
	mddev->gendisk = disk;
	error = add_disk(disk);
	if (error)
		goto out_put_disk;

	kobject_init(&mddev->kobj, &md_ktype);
	error = kobject_add(&mddev->kobj, &disk_to_dev(disk)->kobj, "%s", "md");
	if (error) {
		/*
		 * The disk is already live at this point.  Clear the hold flag
		 * and let mddev_put take care of the deletion, as it isn't any
		 * different from a normal close on last release now.
		 */
		mddev->hold_active = 0;
		mutex_unlock(&disks_mutex);
		mddev_put(mddev);
		return ERR_PTR(error);
	}

	kobject_uevent(&mddev->kobj, KOBJ_ADD);
	mddev->sysfs_state = sysfs_get_dirent_safe(mddev->kobj.sd, "array_state");
	mddev->sysfs_level = sysfs_get_dirent_safe(mddev->kobj.sd, "level");
	mutex_unlock(&disks_mutex);
	return mddev;

out_put_disk:
	put_disk(disk);
out_free_mddev:
	mddev_free(mddev);
out_unlock:
	mutex_unlock(&disks_mutex);
	return ERR_PTR(error);
}

static int md_alloc_and_put(dev_t dev, char *name)
{
	struct mddev *mddev = md_alloc(dev, name);

	if (IS_ERR(mddev))
		return PTR_ERR(mddev);
	mddev_put(mddev);
	return 0;
}

static void md_probe(dev_t dev)
{
	if (MAJOR(dev) == MD_MAJOR && MINOR(dev) >= 512)
		return;
	if (create_on_open)
		md_alloc_and_put(dev, NULL);
}

static int add_named_array(const char *val, const struct kernel_param *kp)
{
	/*
	 * val must be "md_*" or "mdNNN".
	 * For "md_*" we allocate an array with a large free minor number, and
	 * set the name to val.  val must not already be an active name.
	 * For "mdNNN" we allocate an array with the minor number NNN
	 * which must not already be in use.
	 */
	int len = strlen(val);
	char buf[DISK_NAME_LEN];
	unsigned long devnum;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	while (len && val[len-1] == '\n')
		len--;
	if (len >= DISK_NAME_LEN)
		return -E2BIG;
<<<<<<< HEAD
	strlcpy(buf, val, len+1);
	if (strncmp(buf, "md_", 3) != 0)
		return -EINVAL;
	return md_alloc(0, buf);
}

static void md_safemode_timeout(unsigned long data)
{
	struct mddev *mddev = (struct mddev *) data;

	if (!atomic_read(&mddev->writes_pending)) {
		mddev->safemode = 1;
		if (mddev->external)
			sysfs_notify_dirent_safe(mddev->sysfs_state);
	}
=======
	strscpy(buf, val, len+1);
	if (strncmp(buf, "md_", 3) == 0)
		return md_alloc_and_put(0, buf);
	if (strncmp(buf, "md", 2) == 0 &&
	    isdigit(buf[2]) &&
	    kstrtoul(buf+2, 10, &devnum) == 0 &&
	    devnum <= MINORMASK)
		return md_alloc_and_put(MKDEV(MD_MAJOR, devnum), NULL);

	return -EINVAL;
}

static void md_safemode_timeout(struct timer_list *t)
{
	struct mddev *mddev = from_timer(mddev, t, safemode_timer);

	mddev->safemode = 1;
	if (mddev->external)
		sysfs_notify_dirent_safe(mddev->sysfs_state);

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	md_wakeup_thread(mddev->thread);
}

static int start_dirty_degraded;

int md_run(struct mddev *mddev)
{
	int err;
	struct md_rdev *rdev;
	struct md_personality *pers;
<<<<<<< HEAD
=======
	bool nowait = true;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	if (list_empty(&mddev->disks))
		/* cannot run an array with no devices.. */
		return -EINVAL;

	if (mddev->pers)
		return -EBUSY;
	/* Cannot run until previous stop completes properly */
	if (mddev->sysfs_active)
		return -EBUSY;

	/*
	 * Analyze all RAID superblock(s)
	 */
	if (!mddev->raid_disks) {
		if (!mddev->persistent)
			return -EINVAL;
<<<<<<< HEAD
		analyze_sbs(mddev);
=======
		err = analyze_sbs(mddev);
		if (err)
			return -EINVAL;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	}

	if (mddev->level != LEVEL_NONE)
		request_module("md-level-%d", mddev->level);
	else if (mddev->clevel[0])
		request_module("md-%s", mddev->clevel);

	/*
	 * Drop all container device buffers, from now on
	 * the only valid external interface is through the md
	 * device.
	 */
<<<<<<< HEAD
=======
	mddev->has_superblocks = false;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	rdev_for_each(rdev, mddev) {
		if (test_bit(Faulty, &rdev->flags))
			continue;
		sync_blockdev(rdev->bdev);
		invalidate_bdev(rdev->bdev);
<<<<<<< HEAD
=======
		if (mddev->ro != MD_RDONLY && rdev_read_only(rdev)) {
			mddev->ro = MD_RDONLY;
			if (!mddev_is_dm(mddev))
				set_disk_ro(mddev->gendisk, 1);
		}

		if (rdev->sb_page)
			mddev->has_superblocks = true;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

		/* perform some consistency tests on the device.
		 * We don't want the data to overlap the metadata,
		 * Internal Bitmap issues have been handled elsewhere.
		 */
		if (rdev->meta_bdev) {
			/* Nothing to check */;
		} else if (rdev->data_offset < rdev->sb_start) {
			if (mddev->dev_sectors &&
			    rdev->data_offset + mddev->dev_sectors
			    > rdev->sb_start) {
<<<<<<< HEAD
				printk("md: %s: data overlaps metadata\n",
				       mdname(mddev));
=======
				pr_warn("md: %s: data overlaps metadata\n",
					mdname(mddev));
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
				return -EINVAL;
			}
		} else {
			if (rdev->sb_start + rdev->sb_size/512
			    > rdev->data_offset) {
<<<<<<< HEAD
				printk("md: %s: metadata overlaps data\n",
				       mdname(mddev));
=======
				pr_warn("md: %s: metadata overlaps data\n",
					mdname(mddev));
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
				return -EINVAL;
			}
		}
		sysfs_notify_dirent_safe(rdev->sysfs_state);
<<<<<<< HEAD
	}

	if (mddev->bio_set == NULL)
		mddev->bio_set = bioset_create(BIO_POOL_SIZE,
					       sizeof(struct mddev *));
=======
		nowait = nowait && bdev_nowait(rdev->bdev);
	}

	if (!bioset_initialized(&mddev->bio_set)) {
		err = bioset_init(&mddev->bio_set, BIO_POOL_SIZE, 0, BIOSET_NEED_BVECS);
		if (err)
			return err;
	}
	if (!bioset_initialized(&mddev->sync_set)) {
		err = bioset_init(&mddev->sync_set, BIO_POOL_SIZE, 0, BIOSET_NEED_BVECS);
		if (err)
			goto exit_bio_set;
	}

	if (!bioset_initialized(&mddev->io_clone_set)) {
		err = bioset_init(&mddev->io_clone_set, BIO_POOL_SIZE,
				  offsetof(struct md_io_clone, bio_clone), 0);
		if (err)
			goto exit_sync_set;
	}
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	spin_lock(&pers_lock);
	pers = find_pers(mddev->level, mddev->clevel);
	if (!pers || !try_module_get(pers->owner)) {
		spin_unlock(&pers_lock);
		if (mddev->level != LEVEL_NONE)
<<<<<<< HEAD
			printk(KERN_WARNING "md: personality for level %d is not loaded!\n",
			       mddev->level);
		else
			printk(KERN_WARNING "md: personality for level %s is not loaded!\n",
			       mddev->clevel);
		return -EINVAL;
	}
	mddev->pers = pers;
=======
			pr_warn("md: personality for level %d is not loaded!\n",
				mddev->level);
		else
			pr_warn("md: personality for level %s is not loaded!\n",
				mddev->clevel);
		err = -EINVAL;
		goto abort;
	}
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	spin_unlock(&pers_lock);
	if (mddev->level != pers->level) {
		mddev->level = pers->level;
		mddev->new_level = pers->level;
	}
<<<<<<< HEAD
	strlcpy(mddev->clevel, pers->name, sizeof(mddev->clevel));
=======
	strscpy(mddev->clevel, pers->name, sizeof(mddev->clevel));
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	if (mddev->reshape_position != MaxSector &&
	    pers->start_reshape == NULL) {
		/* This personality cannot handle reshaping... */
<<<<<<< HEAD
		mddev->pers = NULL;
		module_put(pers->owner);
		return -EINVAL;
=======
		module_put(pers->owner);
		err = -EINVAL;
		goto abort;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	}

	if (pers->sync_request) {
		/* Warn if this is a potentially silly
		 * configuration.
		 */
<<<<<<< HEAD
		char b[BDEVNAME_SIZE], b2[BDEVNAME_SIZE];
=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		struct md_rdev *rdev2;
		int warned = 0;

		rdev_for_each(rdev, mddev)
			rdev_for_each(rdev2, mddev) {
				if (rdev < rdev2 &&
<<<<<<< HEAD
				    rdev->bdev->bd_contains ==
				    rdev2->bdev->bd_contains) {
					printk(KERN_WARNING
					       "%s: WARNING: %s appears to be"
					       " on the same physical disk as"
					       " %s.\n",
					       mdname(mddev),
					       bdevname(rdev->bdev,b),
					       bdevname(rdev2->bdev,b2));
=======
				    rdev->bdev->bd_disk ==
				    rdev2->bdev->bd_disk) {
					pr_warn("%s: WARNING: %pg appears to be on the same physical disk as %pg.\n",
						mdname(mddev),
						rdev->bdev,
						rdev2->bdev);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
					warned = 1;
				}
			}

		if (warned)
<<<<<<< HEAD
			printk(KERN_WARNING
			       "True protection against single-disk"
			       " failure might be compromised.\n");
	}

	mddev->recovery = 0;
=======
			pr_warn("True protection against single-disk failure might be compromised.\n");
	}

	/* dm-raid expect sync_thread to be frozen until resume */
	if (mddev->gendisk)
		mddev->recovery = 0;

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	/* may be over-ridden by personality */
	mddev->resync_max_sectors = mddev->dev_sectors;

	mddev->ok_start_degraded = start_dirty_degraded;

<<<<<<< HEAD
	if (start_readonly && mddev->ro == 0)
		mddev->ro = 2; /* read-only, but switch on first write */

	err = mddev->pers->run(mddev);
	if (err)
		printk(KERN_ERR "md: pers->run() failed ...\n");
	else if (mddev->pers->size(mddev, 0, 0) < mddev->array_sectors) {
		WARN_ONCE(!mddev->external_size, "%s: default size too small,"
			  " but 'external_size' not in effect?\n", __func__);
		printk(KERN_ERR
		       "md: invalid array_size %llu > default size %llu\n",
		       (unsigned long long)mddev->array_sectors / 2,
		       (unsigned long long)mddev->pers->size(mddev, 0, 0) / 2);
		err = -EINVAL;
		mddev->pers->stop(mddev);
	}
	if (err == 0 && mddev->pers->sync_request) {
		err = bitmap_create(mddev);
		if (err) {
			printk(KERN_ERR "%s: failed to create bitmap (%d)\n",
			       mdname(mddev), err);
			mddev->pers->stop(mddev);
		}
	}
	if (err) {
		module_put(mddev->pers->owner);
		mddev->pers = NULL;
		bitmap_destroy(mddev);
		return err;
	}
	if (mddev->pers->sync_request) {
		if (mddev->kobj.sd &&
		    sysfs_create_group(&mddev->kobj, &md_redundancy_group))
			printk(KERN_WARNING
			       "md: cannot register extra attributes for %s\n",
			       mdname(mddev));
		mddev->sysfs_action = sysfs_get_dirent_safe(mddev->kobj.sd, "sync_action");
	} else if (mddev->ro == 2) /* auto-readonly not meaningful */
		mddev->ro = 0;

 	atomic_set(&mddev->writes_pending,0);
	atomic_set(&mddev->max_corr_read_errors,
		   MD_DEFAULT_MAX_CORRECTED_READ_ERRORS);
	mddev->safemode = 0;
	mddev->safemode_timer.function = md_safemode_timeout;
	mddev->safemode_timer.data = (unsigned long) mddev;
	mddev->safemode_delay = (200 * HZ)/1000 +1; /* 200 msec delay */
	mddev->in_sync = 1;
	smp_wmb();
	mddev->ready = 1;
	rdev_for_each(rdev, mddev)
		if (rdev->raid_disk >= 0)
			if (sysfs_link_rdev(mddev, rdev))
				/* failure here is OK */;
	
	set_bit(MD_RECOVERY_NEEDED, &mddev->recovery);
	
	if (mddev->flags)
		md_update_sb(mddev, 0);

	md_new_event(mddev);
	sysfs_notify_dirent_safe(mddev->sysfs_state);
	sysfs_notify_dirent_safe(mddev->sysfs_action);
	sysfs_notify(&mddev->kobj, NULL, "degraded");
	return 0;
}
EXPORT_SYMBOL_GPL(md_run);

static int do_md_run(struct mddev *mddev)
{
	int err;

	err = md_run(mddev);
	if (err)
		goto out;
	err = bitmap_load(mddev);
	if (err) {
		bitmap_destroy(mddev);
		goto out;
	}

	md_wakeup_thread(mddev->thread);
	md_wakeup_thread(mddev->sync_thread); /* possibly kick off a reshape */

	set_capacity(mddev->gendisk, mddev->array_sectors);
	revalidate_disk(mddev->gendisk);
	mddev->changed = 1;
	kobject_uevent(&disk_to_dev(mddev->gendisk)->kobj, KOBJ_CHANGE);
out:
	return err;
}

static int restart_array(struct mddev *mddev)
{
	struct gendisk *disk = mddev->gendisk;
=======
	if (start_readonly && md_is_rdwr(mddev))
		mddev->ro = MD_AUTO_READ; /* read-only, but switch on first write */

	err = pers->run(mddev);
	if (err)
		pr_warn("md: pers->run() failed ...\n");
	else if (pers->size(mddev, 0, 0) < mddev->array_sectors) {
		WARN_ONCE(!mddev->external_size,
			  "%s: default size too small, but 'external_size' not in effect?\n",
			  __func__);
		pr_warn("md: invalid array_size %llu > default size %llu\n",
			(unsigned long long)mddev->array_sectors / 2,
			(unsigned long long)pers->size(mddev, 0, 0) / 2);
		err = -EINVAL;
	}
	if (err == 0 && pers->sync_request &&
	    (mddev->bitmap_info.file || mddev->bitmap_info.offset)) {
		struct bitmap *bitmap;

		bitmap = md_bitmap_create(mddev, -1);
		if (IS_ERR(bitmap)) {
			err = PTR_ERR(bitmap);
			pr_warn("%s: failed to create bitmap (%d)\n",
				mdname(mddev), err);
		} else
			mddev->bitmap = bitmap;

	}
	if (err)
		goto bitmap_abort;

	if (mddev->bitmap_info.max_write_behind > 0) {
		bool create_pool = false;

		rdev_for_each(rdev, mddev) {
			if (test_bit(WriteMostly, &rdev->flags) &&
			    rdev_init_serial(rdev))
				create_pool = true;
		}
		if (create_pool && mddev->serial_info_pool == NULL) {
			mddev->serial_info_pool =
				mempool_create_kmalloc_pool(NR_SERIAL_INFOS,
						    sizeof(struct serial_info));
			if (!mddev->serial_info_pool) {
				err = -ENOMEM;
				goto bitmap_abort;
			}
		}
	}

	if (!mddev_is_dm(mddev)) {
		struct request_queue *q = mddev->gendisk->queue;
		bool nonrot = true;

		rdev_for_each(rdev, mddev) {
			if (rdev->raid_disk >= 0 && !bdev_nonrot(rdev->bdev)) {
				nonrot = false;
				break;
			}
		}
		if (mddev->degraded)
			nonrot = false;
		if (nonrot)
			blk_queue_flag_set(QUEUE_FLAG_NONROT, q);
		else
			blk_queue_flag_clear(QUEUE_FLAG_NONROT, q);
		blk_queue_flag_set(QUEUE_FLAG_IO_STAT, q);

		/* Set the NOWAIT flags if all underlying devices support it */
		if (nowait)
			blk_queue_flag_set(QUEUE_FLAG_NOWAIT, q);
	}
	if (pers->sync_request) {
		if (mddev->kobj.sd &&
		    sysfs_create_group(&mddev->kobj, &md_redundancy_group))
			pr_warn("md: cannot register extra attributes for %s\n",
				mdname(mddev));
		mddev->sysfs_action = sysfs_get_dirent_safe(mddev->kobj.sd, "sync_action");
		mddev->sysfs_completed = sysfs_get_dirent_safe(mddev->kobj.sd, "sync_completed");
		mddev->sysfs_degraded = sysfs_get_dirent_safe(mddev->kobj.sd, "degraded");
	} else if (mddev->ro == MD_AUTO_READ)
		mddev->ro = MD_RDWR;

	atomic_set(&mddev->max_corr_read_errors,
		   MD_DEFAULT_MAX_CORRECTED_READ_ERRORS);
	mddev->safemode = 0;
	if (mddev_is_clustered(mddev))
		mddev->safemode_delay = 0;
	else
		mddev->safemode_delay = DEFAULT_SAFEMODE_DELAY;
	mddev->in_sync = 1;
	smp_wmb();
	spin_lock(&mddev->lock);
	mddev->pers = pers;
	spin_unlock(&mddev->lock);
	rdev_for_each(rdev, mddev)
		if (rdev->raid_disk >= 0)
			sysfs_link_rdev(mddev, rdev); /* failure here is OK */

	if (mddev->degraded && md_is_rdwr(mddev))
		/* This ensures that recovering status is reported immediately
		 * via sysfs - until a lack of spares is confirmed.
		 */
		set_bit(MD_RECOVERY_RECOVER, &mddev->recovery);
	set_bit(MD_RECOVERY_NEEDED, &mddev->recovery);

	if (mddev->sb_flags)
		md_update_sb(mddev, 0);

	md_new_event();
	return 0;

bitmap_abort:
	mddev_detach(mddev);
	if (mddev->private)
		pers->free(mddev, mddev->private);
	mddev->private = NULL;
	module_put(pers->owner);
	md_bitmap_destroy(mddev);
abort:
	bioset_exit(&mddev->io_clone_set);
exit_sync_set:
	bioset_exit(&mddev->sync_set);
exit_bio_set:
	bioset_exit(&mddev->bio_set);
	return err;
}
EXPORT_SYMBOL_GPL(md_run);

int do_md_run(struct mddev *mddev)
{
	int err;

	set_bit(MD_NOT_READY, &mddev->flags);
	err = md_run(mddev);
	if (err)
		goto out;
	err = md_bitmap_load(mddev);
	if (err) {
		md_bitmap_destroy(mddev);
		goto out;
	}

	if (mddev_is_clustered(mddev))
		md_allow_write(mddev);

	/* run start up tasks that require md_thread */
	md_start(mddev);

	md_wakeup_thread(mddev->sync_thread); /* possibly kick off a reshape */

	set_capacity_and_notify(mddev->gendisk, mddev->array_sectors);
	clear_bit(MD_NOT_READY, &mddev->flags);
	mddev->changed = 1;
	kobject_uevent(&disk_to_dev(mddev->gendisk)->kobj, KOBJ_CHANGE);
	sysfs_notify_dirent_safe(mddev->sysfs_state);
	sysfs_notify_dirent_safe(mddev->sysfs_action);
	sysfs_notify_dirent_safe(mddev->sysfs_degraded);
out:
	clear_bit(MD_NOT_READY, &mddev->flags);
	return err;
}

int md_start(struct mddev *mddev)
{
	int ret = 0;

	if (mddev->pers->start) {
		set_bit(MD_RECOVERY_WAIT, &mddev->recovery);
		ret = mddev->pers->start(mddev);
		clear_bit(MD_RECOVERY_WAIT, &mddev->recovery);
		md_wakeup_thread(mddev->sync_thread);
	}
	return ret;
}
EXPORT_SYMBOL_GPL(md_start);

static int restart_array(struct mddev *mddev)
{
	struct gendisk *disk = mddev->gendisk;
	struct md_rdev *rdev;
	bool has_journal = false;
	bool has_readonly = false;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	/* Complain if it has no devices */
	if (list_empty(&mddev->disks))
		return -ENXIO;
	if (!mddev->pers)
		return -EINVAL;
<<<<<<< HEAD
	if (!mddev->ro)
		return -EBUSY;
	mddev->safemode = 0;
	mddev->ro = 0;
	set_disk_ro(disk, 0);
	printk(KERN_INFO "md: %s switched to read-write mode.\n",
		mdname(mddev));
	/* Kick recovery or resync if necessary */
	set_bit(MD_RECOVERY_NEEDED, &mddev->recovery);
	md_wakeup_thread(mddev->thread);
=======
	if (md_is_rdwr(mddev))
		return -EBUSY;

	rcu_read_lock();
	rdev_for_each_rcu(rdev, mddev) {
		if (test_bit(Journal, &rdev->flags) &&
		    !test_bit(Faulty, &rdev->flags))
			has_journal = true;
		if (rdev_read_only(rdev))
			has_readonly = true;
	}
	rcu_read_unlock();
	if (test_bit(MD_HAS_JOURNAL, &mddev->flags) && !has_journal)
		/* Don't restart rw with journal missing/faulty */
			return -EINVAL;
	if (has_readonly)
		return -EROFS;

	mddev->safemode = 0;
	mddev->ro = MD_RDWR;
	set_disk_ro(disk, 0);
	pr_debug("md: %s switched to read-write mode.\n", mdname(mddev));
	/* Kick recovery or resync if necessary */
	set_bit(MD_RECOVERY_NEEDED, &mddev->recovery);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	md_wakeup_thread(mddev->sync_thread);
	sysfs_notify_dirent_safe(mddev->sysfs_state);
	return 0;
}

<<<<<<< HEAD
/* similar to deny_write_access, but accounts for our holding a reference
 * to the file ourselves */
static int deny_bitmap_write_access(struct file * file)
{
	struct inode *inode = file->f_mapping->host;

	spin_lock(&inode->i_lock);
	if (atomic_read(&inode->i_writecount) > 1) {
		spin_unlock(&inode->i_lock);
		return -ETXTBSY;
	}
	atomic_set(&inode->i_writecount, -1);
	spin_unlock(&inode->i_lock);

	return 0;
}

void restore_bitmap_write_access(struct file *file)
{
	struct inode *inode = file->f_mapping->host;

	spin_lock(&inode->i_lock);
	atomic_set(&inode->i_writecount, 1);
	spin_unlock(&inode->i_lock);
}

=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
static void md_clean(struct mddev *mddev)
{
	mddev->array_sectors = 0;
	mddev->external_size = 0;
	mddev->dev_sectors = 0;
	mddev->raid_disks = 0;
	mddev->recovery_cp = 0;
	mddev->resync_min = 0;
	mddev->resync_max = MaxSector;
	mddev->reshape_position = MaxSector;
<<<<<<< HEAD
	mddev->external = 0;
	mddev->persistent = 0;
	mddev->level = LEVEL_NONE;
	mddev->clevel[0] = 0;
	mddev->flags = 0;
	mddev->ro = 0;
=======
	/* we still need mddev->external in export_rdev, do not clear it yet */
	mddev->persistent = 0;
	mddev->level = LEVEL_NONE;
	mddev->clevel[0] = 0;
	/*
	 * Don't clear MD_CLOSING, or mddev can be opened again.
	 * 'hold_active != 0' means mddev is still in the creation
	 * process and will be used later.
	 */
	if (mddev->hold_active)
		mddev->flags = 0;
	else
		mddev->flags &= BIT_ULL_MASK(MD_CLOSING);
	mddev->sb_flags = 0;
	mddev->ro = MD_RDWR;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	mddev->metadata_type[0] = 0;
	mddev->chunk_sectors = 0;
	mddev->ctime = mddev->utime = 0;
	mddev->layout = 0;
	mddev->max_disks = 0;
	mddev->events = 0;
	mddev->can_decrease_events = 0;
	mddev->delta_disks = 0;
<<<<<<< HEAD
	mddev->new_level = LEVEL_NONE;
	mddev->new_layout = 0;
	mddev->new_chunk_sectors = 0;
	mddev->curr_resync = 0;
	mddev->resync_mismatches = 0;
=======
	mddev->reshape_backwards = 0;
	mddev->new_level = LEVEL_NONE;
	mddev->new_layout = 0;
	mddev->new_chunk_sectors = 0;
	mddev->curr_resync = MD_RESYNC_NONE;
	atomic64_set(&mddev->resync_mismatches, 0);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	mddev->suspend_lo = mddev->suspend_hi = 0;
	mddev->sync_speed_min = mddev->sync_speed_max = 0;
	mddev->recovery = 0;
	mddev->in_sync = 0;
	mddev->changed = 0;
	mddev->degraded = 0;
	mddev->safemode = 0;
<<<<<<< HEAD
	mddev->merge_check_needed = 0;
	mddev->bitmap_info.offset = 0;
	mddev->bitmap_info.default_offset = 0;
	mddev->bitmap_info.chunksize = 0;
	mddev->bitmap_info.daemon_sleep = 0;
	mddev->bitmap_info.max_write_behind = 0;
=======
	mddev->private = NULL;
	mddev->cluster_info = NULL;
	mddev->bitmap_info.offset = 0;
	mddev->bitmap_info.default_offset = 0;
	mddev->bitmap_info.default_space = 0;
	mddev->bitmap_info.chunksize = 0;
	mddev->bitmap_info.daemon_sleep = 0;
	mddev->bitmap_info.max_write_behind = 0;
	mddev->bitmap_info.nodes = 0;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

static void __md_stop_writes(struct mddev *mddev)
{
<<<<<<< HEAD
	if (mddev->sync_thread) {
		set_bit(MD_RECOVERY_FROZEN, &mddev->recovery);
		set_bit(MD_RECOVERY_INTR, &mddev->recovery);
		reap_sync_thread(mddev);
	}

	del_timer_sync(&mddev->safemode_timer);

	bitmap_flush(mddev);
	md_super_wait(mddev);

	if (!mddev->in_sync || mddev->flags) {
		/* mark array as shutdown cleanly */
		mddev->in_sync = 1;
		md_update_sb(mddev, 1);
	}
=======
	del_timer_sync(&mddev->safemode_timer);

	if (mddev->pers && mddev->pers->quiesce) {
		mddev->pers->quiesce(mddev, 1);
		mddev->pers->quiesce(mddev, 0);
	}
	md_bitmap_flush(mddev);

	if (md_is_rdwr(mddev) &&
	    ((!mddev->in_sync && !mddev_is_clustered(mddev)) ||
	     mddev->sb_flags)) {
		/* mark array as shutdown cleanly */
		if (!mddev_is_clustered(mddev))
			mddev->in_sync = 1;
		md_update_sb(mddev, 1);
	}
	/* disable policy to guarantee rdevs free resources for serialization */
	mddev->serialize_policy = 0;
	mddev_destroy_serial_pool(mddev, NULL);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

void md_stop_writes(struct mddev *mddev)
{
<<<<<<< HEAD
	mddev_lock(mddev);
=======
	mddev_lock_nointr(mddev);
	set_bit(MD_RECOVERY_FROZEN, &mddev->recovery);
	stop_sync_thread(mddev, true, false);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	__md_stop_writes(mddev);
	mddev_unlock(mddev);
}
EXPORT_SYMBOL_GPL(md_stop_writes);

<<<<<<< HEAD
static void __md_stop(struct mddev *mddev)
{
	mddev->ready = 0;
	mddev->pers->stop(mddev);
	if (mddev->pers->sync_request && mddev->to_remove == NULL)
		mddev->to_remove = &md_redundancy_group;
	module_put(mddev->pers->owner);
	/* Ensure ->event_work is done */
	flush_workqueue(md_misc_wq);
	mddev->pers = NULL;
	clear_bit(MD_RECOVERY_FROZEN, &mddev->recovery);
=======
static void mddev_detach(struct mddev *mddev)
{
	md_bitmap_wait_behind_writes(mddev);
	if (mddev->pers && mddev->pers->quiesce && !is_md_suspended(mddev)) {
		mddev->pers->quiesce(mddev, 1);
		mddev->pers->quiesce(mddev, 0);
	}
	md_unregister_thread(mddev, &mddev->thread);

	/* the unplug fn references 'conf' */
	if (!mddev_is_dm(mddev))
		blk_sync_queue(mddev->gendisk->queue);
}

static void __md_stop(struct mddev *mddev)
{
	struct md_personality *pers = mddev->pers;
	md_bitmap_destroy(mddev);
	mddev_detach(mddev);
	spin_lock(&mddev->lock);
	mddev->pers = NULL;
	spin_unlock(&mddev->lock);
	if (mddev->private)
		pers->free(mddev, mddev->private);
	mddev->private = NULL;
	if (pers->sync_request && mddev->to_remove == NULL)
		mddev->to_remove = &md_redundancy_group;
	module_put(pers->owner);
	clear_bit(MD_RECOVERY_FROZEN, &mddev->recovery);

	bioset_exit(&mddev->bio_set);
	bioset_exit(&mddev->sync_set);
	bioset_exit(&mddev->io_clone_set);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

void md_stop(struct mddev *mddev)
{
<<<<<<< HEAD
	/* stop the array and free an attached data structures.
	 * This is called from dm-raid
	 */
	__md_stop(mddev);
	bitmap_destroy(mddev);
	if (mddev->bio_set)
		bioset_free(mddev->bio_set);
=======
	lockdep_assert_held(&mddev->reconfig_mutex);

	/* stop the array and free an attached data structures.
	 * This is called from dm-raid
	 */
	__md_stop_writes(mddev);
	__md_stop(mddev);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

EXPORT_SYMBOL_GPL(md_stop);

<<<<<<< HEAD
static int md_set_readonly(struct mddev *mddev, struct block_device *bdev)
{
	int err = 0;
	mutex_lock(&mddev->open_mutex);
	if (atomic_read(&mddev->openers) > !!bdev) {
		printk("md: %s still in use.\n",mdname(mddev));
		err = -EBUSY;
		goto out;
	}
	if (bdev)
		sync_blockdev(bdev);
	if (mddev->pers) {
		__md_stop_writes(mddev);

		err  = -ENXIO;
		if (mddev->ro==1)
			goto out;
		mddev->ro = 1;
		set_disk_ro(mddev->gendisk, 1);
		clear_bit(MD_RECOVERY_FROZEN, &mddev->recovery);
		sysfs_notify_dirent_safe(mddev->sysfs_state);
		err = 0;	
	}
out:
	mutex_unlock(&mddev->open_mutex);
=======
/* ensure 'mddev->pers' exist before calling md_set_readonly() */
static int md_set_readonly(struct mddev *mddev)
{
	int err = 0;
	int did_freeze = 0;

	if (mddev->external && test_bit(MD_SB_CHANGE_PENDING, &mddev->sb_flags))
		return -EBUSY;

	if (!test_bit(MD_RECOVERY_FROZEN, &mddev->recovery)) {
		did_freeze = 1;
		set_bit(MD_RECOVERY_FROZEN, &mddev->recovery);
	}

	stop_sync_thread(mddev, false, false);
	wait_event(mddev->sb_wait,
		   !test_bit(MD_SB_CHANGE_PENDING, &mddev->sb_flags));
	mddev_lock_nointr(mddev);

	if (test_bit(MD_RECOVERY_RUNNING, &mddev->recovery)) {
		pr_warn("md: %s still in use.\n",mdname(mddev));
		err = -EBUSY;
		goto out;
	}

	__md_stop_writes(mddev);

	if (mddev->ro == MD_RDONLY) {
		err  = -ENXIO;
		goto out;
	}

	mddev->ro = MD_RDONLY;
	set_disk_ro(mddev->gendisk, 1);

out:
	if (!err || did_freeze) {
		clear_bit(MD_RECOVERY_FROZEN, &mddev->recovery);
		set_bit(MD_RECOVERY_NEEDED, &mddev->recovery);
		sysfs_notify_dirent_safe(mddev->sysfs_state);
	}

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	return err;
}

/* mode:
 *   0 - completely stop and dis-assemble array
 *   2 - stop but do not disassemble array
 */
<<<<<<< HEAD
static int do_md_stop(struct mddev * mddev, int mode,
		      struct block_device *bdev)
{
	struct gendisk *disk = mddev->gendisk;
	struct md_rdev *rdev;

	mutex_lock(&mddev->open_mutex);
	if (atomic_read(&mddev->openers) > !!bdev ||
	    mddev->sysfs_active) {
		printk("md: %s still in use.\n",mdname(mddev));
		mutex_unlock(&mddev->open_mutex);
		return -EBUSY;
	}
	if (bdev)
		/* It is possible IO was issued on some other
		 * open file which was closed before we took ->open_mutex.
		 * As that was not the last close __blkdev_put will not
		 * have called sync_blockdev, so we must.
		 */
		sync_blockdev(bdev);

	if (mddev->pers) {
		if (mddev->ro)
=======
static int do_md_stop(struct mddev *mddev, int mode)
{
	struct gendisk *disk = mddev->gendisk;
	struct md_rdev *rdev;
	int did_freeze = 0;

	if (!test_bit(MD_RECOVERY_FROZEN, &mddev->recovery)) {
		did_freeze = 1;
		set_bit(MD_RECOVERY_FROZEN, &mddev->recovery);
	}

	stop_sync_thread(mddev, true, false);

	if (mddev->sysfs_active ||
	    test_bit(MD_RECOVERY_RUNNING, &mddev->recovery)) {
		pr_warn("md: %s still in use.\n",mdname(mddev));
		if (did_freeze) {
			clear_bit(MD_RECOVERY_FROZEN, &mddev->recovery);
			set_bit(MD_RECOVERY_NEEDED, &mddev->recovery);
		}
		return -EBUSY;
	}
	if (mddev->pers) {
		if (!md_is_rdwr(mddev))
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
			set_disk_ro(disk, 0);

		__md_stop_writes(mddev);
		__md_stop(mddev);
<<<<<<< HEAD
		mddev->queue->merge_bvec_fn = NULL;
		mddev->queue->backing_dev_info.congested_fn = NULL;
=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

		/* tell userspace to handle 'inactive' */
		sysfs_notify_dirent_safe(mddev->sysfs_state);

		rdev_for_each(rdev, mddev)
			if (rdev->raid_disk >= 0)
				sysfs_unlink_rdev(mddev, rdev);

<<<<<<< HEAD
		set_capacity(disk, 0);
		mutex_unlock(&mddev->open_mutex);
		mddev->changed = 1;
		revalidate_disk(disk);

		if (mddev->ro)
			mddev->ro = 0;
	} else
		mutex_unlock(&mddev->open_mutex);
=======
		set_capacity_and_notify(disk, 0);
		mddev->changed = 1;

		if (!md_is_rdwr(mddev))
			mddev->ro = MD_RDWR;
	}
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	/*
	 * Free resources if final stop
	 */
	if (mode == 0) {
<<<<<<< HEAD
		printk(KERN_INFO "md: %s stopped.\n", mdname(mddev));

		bitmap_destroy(mddev);
		if (mddev->bitmap_info.file) {
			restore_bitmap_write_access(mddev->bitmap_info.file);
			fput(mddev->bitmap_info.file);
			mddev->bitmap_info.file = NULL;
=======
		pr_info("md: %s stopped.\n", mdname(mddev));

		if (mddev->bitmap_info.file) {
			struct file *f = mddev->bitmap_info.file;
			spin_lock(&mddev->lock);
			mddev->bitmap_info.file = NULL;
			spin_unlock(&mddev->lock);
			fput(f);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		}
		mddev->bitmap_info.offset = 0;

		export_array(mddev);

		md_clean(mddev);
<<<<<<< HEAD
		kobject_uevent(&disk_to_dev(mddev->gendisk)->kobj, KOBJ_CHANGE);
		if (mddev->hold_active == UNTIL_STOP)
			mddev->hold_active = 0;
	}
	blk_integrity_unregister(disk);
	md_new_event(mddev);
=======
		if (mddev->hold_active == UNTIL_STOP)
			mddev->hold_active = 0;
	}
	md_new_event();
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	sysfs_notify_dirent_safe(mddev->sysfs_state);
	return 0;
}

#ifndef MODULE
static void autorun_array(struct mddev *mddev)
{
	struct md_rdev *rdev;
	int err;

	if (list_empty(&mddev->disks))
		return;

<<<<<<< HEAD
	printk(KERN_INFO "md: running: ");

	rdev_for_each(rdev, mddev) {
		char b[BDEVNAME_SIZE];
		printk("<%s>", bdevname(rdev->bdev,b));
	}
	printk("\n");

	err = do_md_run(mddev);
	if (err) {
		printk(KERN_WARNING "md: do_md_run() returned %d\n", err);
		do_md_stop(mddev, 0, NULL);
=======
	pr_info("md: running: ");

	rdev_for_each(rdev, mddev) {
		pr_cont("<%pg>", rdev->bdev);
	}
	pr_cont("\n");

	err = do_md_run(mddev);
	if (err) {
		pr_warn("md: do_md_run() returned %d\n", err);
		do_md_stop(mddev, 0);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	}
}

/*
 * lets try to run arrays based on all disks that have arrived
 * until now. (those are in pending_raid_disks)
 *
 * the method: pick the first pending disk, collect all disks with
 * the same UUID, remove all from the pending list and put them into
 * the 'same_array' list. Then order this list based on superblock
 * update time (freshest comes first), kick out 'old' disks and
 * compare superblocks. If everything's fine then run it.
 *
 * If "unit" is allocated, then bump its reference count
 */
static void autorun_devices(int part)
{
	struct md_rdev *rdev0, *rdev, *tmp;
	struct mddev *mddev;
<<<<<<< HEAD
	char b[BDEVNAME_SIZE];

	printk(KERN_INFO "md: autorun ...\n");
=======

	pr_info("md: autorun ...\n");
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	while (!list_empty(&pending_raid_disks)) {
		int unit;
		dev_t dev;
		LIST_HEAD(candidates);
		rdev0 = list_entry(pending_raid_disks.next,
					 struct md_rdev, same_set);

<<<<<<< HEAD
		printk(KERN_INFO "md: considering %s ...\n",
			bdevname(rdev0->bdev,b));
		INIT_LIST_HEAD(&candidates);
		rdev_for_each_list(rdev, tmp, &pending_raid_disks)
			if (super_90_load(rdev, rdev0, 0) >= 0) {
				printk(KERN_INFO "md:  adding %s ...\n",
					bdevname(rdev->bdev,b));
=======
		pr_debug("md: considering %pg ...\n", rdev0->bdev);
		INIT_LIST_HEAD(&candidates);
		rdev_for_each_list(rdev, tmp, &pending_raid_disks)
			if (super_90_load(rdev, rdev0, 0) >= 0) {
				pr_debug("md:  adding %pg ...\n",
					 rdev->bdev);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
				list_move(&rdev->same_set, &candidates);
			}
		/*
		 * now we have a set of devices, with all of them having
		 * mostly sane superblocks. It's time to allocate the
		 * mddev.
		 */
		if (part) {
			dev = MKDEV(mdp_major,
				    rdev0->preferred_minor << MdpMinorShift);
			unit = MINOR(dev) >> MdpMinorShift;
		} else {
			dev = MKDEV(MD_MAJOR, rdev0->preferred_minor);
			unit = MINOR(dev);
		}
		if (rdev0->preferred_minor != unit) {
<<<<<<< HEAD
			printk(KERN_INFO "md: unit number in %s is bad: %d\n",
			       bdevname(rdev0->bdev, b), rdev0->preferred_minor);
			break;
		}

		md_probe(dev, NULL, NULL);
		mddev = mddev_find(dev);
		if (!mddev || !mddev->gendisk) {
			if (mddev)
				mddev_put(mddev);
			printk(KERN_ERR
				"md: cannot allocate memory for md drive.\n");
			break;
		}
		if (mddev_lock(mddev)) 
			printk(KERN_WARNING "md: %s locked, cannot run\n",
			       mdname(mddev));
		else if (mddev->raid_disks || mddev->major_version
			 || !list_empty(&mddev->disks)) {
			printk(KERN_WARNING 
				"md: %s already running, cannot run %s\n",
				mdname(mddev), bdevname(rdev0->bdev,b));
			mddev_unlock(mddev);
		} else {
			printk(KERN_INFO "md: created %s\n", mdname(mddev));
=======
			pr_warn("md: unit number in %pg is bad: %d\n",
				rdev0->bdev, rdev0->preferred_minor);
			break;
		}

		mddev = md_alloc(dev, NULL);
		if (IS_ERR(mddev))
			break;

		if (mddev_suspend_and_lock(mddev))
			pr_warn("md: %s locked, cannot run\n", mdname(mddev));
		else if (mddev->raid_disks || mddev->major_version
			 || !list_empty(&mddev->disks)) {
			pr_warn("md: %s already running, cannot run %pg\n",
				mdname(mddev), rdev0->bdev);
			mddev_unlock_and_resume(mddev);
		} else {
			pr_debug("md: created %s\n", mdname(mddev));
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
			mddev->persistent = 1;
			rdev_for_each_list(rdev, tmp, &candidates) {
				list_del_init(&rdev->same_set);
				if (bind_rdev_to_array(rdev, mddev))
<<<<<<< HEAD
					export_rdev(rdev);
			}
			autorun_array(mddev);
			mddev_unlock(mddev);
=======
					export_rdev(rdev, mddev);
			}
			autorun_array(mddev);
			mddev_unlock_and_resume(mddev);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		}
		/* on success, candidates will be empty, on error
		 * it won't...
		 */
		rdev_for_each_list(rdev, tmp, &candidates) {
			list_del_init(&rdev->same_set);
<<<<<<< HEAD
			export_rdev(rdev);
		}
		mddev_put(mddev);
	}
	printk(KERN_INFO "md: ... autorun DONE.\n");
}
#endif /* !MODULE */

static int get_version(void __user * arg)
=======
			export_rdev(rdev, mddev);
		}
		mddev_put(mddev);
	}
	pr_info("md: ... autorun DONE.\n");
}
#endif /* !MODULE */

static int get_version(void __user *arg)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
{
	mdu_version_t ver;

	ver.major = MD_MAJOR_VERSION;
	ver.minor = MD_MINOR_VERSION;
	ver.patchlevel = MD_PATCHLEVEL_VERSION;

	if (copy_to_user(arg, &ver, sizeof(ver)))
		return -EFAULT;

	return 0;
}

<<<<<<< HEAD
static int get_array_info(struct mddev * mddev, void __user * arg)
=======
static int get_array_info(struct mddev *mddev, void __user *arg)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
{
	mdu_array_info_t info;
	int nr,working,insync,failed,spare;
	struct md_rdev *rdev;

<<<<<<< HEAD
	nr=working=insync=failed=spare=0;
	rdev_for_each(rdev, mddev) {
=======
	nr = working = insync = failed = spare = 0;
	rcu_read_lock();
	rdev_for_each_rcu(rdev, mddev) {
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		nr++;
		if (test_bit(Faulty, &rdev->flags))
			failed++;
		else {
			working++;
			if (test_bit(In_sync, &rdev->flags))
<<<<<<< HEAD
				insync++;	
=======
				insync++;
			else if (test_bit(Journal, &rdev->flags))
				/* TODO: add journal count to md_u.h */
				;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
			else
				spare++;
		}
	}
<<<<<<< HEAD
=======
	rcu_read_unlock();
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	info.major_version = mddev->major_version;
	info.minor_version = mddev->minor_version;
	info.patch_version = MD_PATCHLEVEL_VERSION;
<<<<<<< HEAD
	info.ctime         = mddev->ctime;
=======
	info.ctime         = clamp_t(time64_t, mddev->ctime, 0, U32_MAX);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	info.level         = mddev->level;
	info.size          = mddev->dev_sectors / 2;
	if (info.size != mddev->dev_sectors / 2) /* overflow */
		info.size = -1;
	info.nr_disks      = nr;
	info.raid_disks    = mddev->raid_disks;
	info.md_minor      = mddev->md_minor;
	info.not_persistent= !mddev->persistent;

<<<<<<< HEAD
	info.utime         = mddev->utime;
=======
	info.utime         = clamp_t(time64_t, mddev->utime, 0, U32_MAX);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	info.state         = 0;
	if (mddev->in_sync)
		info.state = (1<<MD_SB_CLEAN);
	if (mddev->bitmap && mddev->bitmap_info.offset)
<<<<<<< HEAD
		info.state = (1<<MD_SB_BITMAP_PRESENT);
=======
		info.state |= (1<<MD_SB_BITMAP_PRESENT);
	if (mddev_is_clustered(mddev))
		info.state |= (1<<MD_SB_CLUSTERED);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	info.active_disks  = insync;
	info.working_disks = working;
	info.failed_disks  = failed;
	info.spare_disks   = spare;

	info.layout        = mddev->layout;
	info.chunk_size    = mddev->chunk_sectors << 9;

	if (copy_to_user(arg, &info, sizeof(info)))
		return -EFAULT;

	return 0;
}

<<<<<<< HEAD
static int get_bitmap_file(struct mddev * mddev, void __user * arg)
{
	mdu_bitmap_file_t *file = NULL; /* too big for stack allocation */
	char *ptr, *buf = NULL;
	int err = -ENOMEM;

	if (md_allow_write(mddev))
		file = kzalloc(sizeof(*file), GFP_NOIO);
	else
		file = kzalloc(sizeof(*file), GFP_KERNEL);

	if (!file)
		goto out;

	/* bitmap disabled, zero the first byte and copy out */
	if (!mddev->bitmap || !mddev->bitmap->file) {
		file->pathname[0] = '\0';
		goto copy_out;
	}

	buf = kmalloc(sizeof(file->pathname), GFP_KERNEL);
	if (!buf)
		goto out;

	ptr = d_path(&mddev->bitmap->file->f_path, buf, sizeof(file->pathname));
	if (IS_ERR(ptr))
		goto out;

	strcpy(file->pathname, ptr);

copy_out:
	err = 0;
	if (copy_to_user(arg, file, sizeof(*file)))
		err = -EFAULT;
out:
	kfree(buf);
=======
static int get_bitmap_file(struct mddev *mddev, void __user * arg)
{
	mdu_bitmap_file_t *file = NULL; /* too big for stack allocation */
	char *ptr;
	int err;

	file = kzalloc(sizeof(*file), GFP_NOIO);
	if (!file)
		return -ENOMEM;

	err = 0;
	spin_lock(&mddev->lock);
	/* bitmap enabled */
	if (mddev->bitmap_info.file) {
		ptr = file_path(mddev->bitmap_info.file, file->pathname,
				sizeof(file->pathname));
		if (IS_ERR(ptr))
			err = PTR_ERR(ptr);
		else
			memmove(file->pathname, ptr,
				sizeof(file->pathname)-(ptr-file->pathname));
	}
	spin_unlock(&mddev->lock);

	if (err == 0 &&
	    copy_to_user(arg, file, sizeof(*file)))
		err = -EFAULT;

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	kfree(file);
	return err;
}

<<<<<<< HEAD
static int get_disk_info(struct mddev * mddev, void __user * arg)
=======
static int get_disk_info(struct mddev *mddev, void __user * arg)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
{
	mdu_disk_info_t info;
	struct md_rdev *rdev;

	if (copy_from_user(&info, arg, sizeof(info)))
		return -EFAULT;

<<<<<<< HEAD
	rdev = find_rdev_nr(mddev, info.number);
=======
	rcu_read_lock();
	rdev = md_find_rdev_nr_rcu(mddev, info.number);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	if (rdev) {
		info.major = MAJOR(rdev->bdev->bd_dev);
		info.minor = MINOR(rdev->bdev->bd_dev);
		info.raid_disk = rdev->raid_disk;
		info.state = 0;
		if (test_bit(Faulty, &rdev->flags))
			info.state |= (1<<MD_DISK_FAULTY);
		else if (test_bit(In_sync, &rdev->flags)) {
			info.state |= (1<<MD_DISK_ACTIVE);
			info.state |= (1<<MD_DISK_SYNC);
		}
<<<<<<< HEAD
		if (test_bit(WriteMostly, &rdev->flags))
			info.state |= (1<<MD_DISK_WRITEMOSTLY);
=======
		if (test_bit(Journal, &rdev->flags))
			info.state |= (1<<MD_DISK_JOURNAL);
		if (test_bit(WriteMostly, &rdev->flags))
			info.state |= (1<<MD_DISK_WRITEMOSTLY);
		if (test_bit(FailFast, &rdev->flags))
			info.state |= (1<<MD_DISK_FAILFAST);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	} else {
		info.major = info.minor = 0;
		info.raid_disk = -1;
		info.state = (1<<MD_DISK_REMOVED);
	}
<<<<<<< HEAD
=======
	rcu_read_unlock();
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	if (copy_to_user(arg, &info, sizeof(info)))
		return -EFAULT;

	return 0;
}

<<<<<<< HEAD
static int add_new_disk(struct mddev * mddev, mdu_disk_info_t *info)
{
	char b[BDEVNAME_SIZE], b2[BDEVNAME_SIZE];
	struct md_rdev *rdev;
	dev_t dev = MKDEV(info->major,info->minor);

=======
int md_add_new_disk(struct mddev *mddev, struct mdu_disk_info_s *info)
{
	struct md_rdev *rdev;
	dev_t dev = MKDEV(info->major,info->minor);

	if (mddev_is_clustered(mddev) &&
		!(info->state & ((1 << MD_DISK_CLUSTER_ADD) | (1 << MD_DISK_CANDIDATE)))) {
		pr_warn("%s: Cannot add to clustered mddev.\n",
			mdname(mddev));
		return -EINVAL;
	}

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	if (info->major != MAJOR(dev) || info->minor != MINOR(dev))
		return -EOVERFLOW;

	if (!mddev->raid_disks) {
		int err;
		/* expecting a device which has a superblock */
		rdev = md_import_device(dev, mddev->major_version, mddev->minor_version);
		if (IS_ERR(rdev)) {
<<<<<<< HEAD
			printk(KERN_WARNING 
				"md: md_import_device returned %ld\n",
=======
			pr_warn("md: md_import_device returned %ld\n",
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
				PTR_ERR(rdev));
			return PTR_ERR(rdev);
		}
		if (!list_empty(&mddev->disks)) {
			struct md_rdev *rdev0
				= list_entry(mddev->disks.next,
					     struct md_rdev, same_set);
			err = super_types[mddev->major_version]
				.load_super(rdev, rdev0, mddev->minor_version);
			if (err < 0) {
<<<<<<< HEAD
				printk(KERN_WARNING 
					"md: %s has different UUID to %s\n",
					bdevname(rdev->bdev,b), 
					bdevname(rdev0->bdev,b2));
				export_rdev(rdev);
=======
				pr_warn("md: %pg has different UUID to %pg\n",
					rdev->bdev,
					rdev0->bdev);
				export_rdev(rdev, mddev);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
				return -EINVAL;
			}
		}
		err = bind_rdev_to_array(rdev, mddev);
		if (err)
<<<<<<< HEAD
			export_rdev(rdev);
=======
			export_rdev(rdev, mddev);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		return err;
	}

	/*
<<<<<<< HEAD
	 * add_new_disk can be used once the array is assembled
=======
	 * md_add_new_disk can be used once the array is assembled
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	 * to add "hot spares".  They must already have a superblock
	 * written
	 */
	if (mddev->pers) {
		int err;
		if (!mddev->pers->hot_add_disk) {
<<<<<<< HEAD
			printk(KERN_WARNING 
				"%s: personality does not support diskops!\n",
			       mdname(mddev));
=======
			pr_warn("%s: personality does not support diskops!\n",
				mdname(mddev));
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
			return -EINVAL;
		}
		if (mddev->persistent)
			rdev = md_import_device(dev, mddev->major_version,
						mddev->minor_version);
		else
			rdev = md_import_device(dev, -1, -1);
		if (IS_ERR(rdev)) {
<<<<<<< HEAD
			printk(KERN_WARNING 
				"md: md_import_device returned %ld\n",
=======
			pr_warn("md: md_import_device returned %ld\n",
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
				PTR_ERR(rdev));
			return PTR_ERR(rdev);
		}
		/* set saved_raid_disk if appropriate */
		if (!mddev->persistent) {
			if (info->state & (1<<MD_DISK_SYNC)  &&
			    info->raid_disk < mddev->raid_disks) {
				rdev->raid_disk = info->raid_disk;
<<<<<<< HEAD
				set_bit(In_sync, &rdev->flags);
			} else
				rdev->raid_disk = -1;
		} else
			super_types[mddev->major_version].
				validate_super(mddev, rdev);
		if ((info->state & (1<<MD_DISK_SYNC)) &&
		    (!test_bit(In_sync, &rdev->flags) ||
		     rdev->raid_disk != info->raid_disk)) {
			/* This was a hot-add request, but events doesn't
			 * match, so reject it.
			 */
			export_rdev(rdev);
			return -EINVAL;
		}

		if (test_bit(In_sync, &rdev->flags))
			rdev->saved_raid_disk = rdev->raid_disk;
		else
			rdev->saved_raid_disk = -1;

=======
				clear_bit(Bitmap_sync, &rdev->flags);
			} else
				rdev->raid_disk = -1;
			rdev->saved_raid_disk = rdev->raid_disk;
		} else
			super_types[mddev->major_version].
				validate_super(mddev, NULL/*freshest*/, rdev);
		if ((info->state & (1<<MD_DISK_SYNC)) &&
		     rdev->raid_disk != info->raid_disk) {
			/* This was a hot-add request, but events doesn't
			 * match, so reject it.
			 */
			export_rdev(rdev, mddev);
			return -EINVAL;
		}

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		clear_bit(In_sync, &rdev->flags); /* just to be sure */
		if (info->state & (1<<MD_DISK_WRITEMOSTLY))
			set_bit(WriteMostly, &rdev->flags);
		else
			clear_bit(WriteMostly, &rdev->flags);
<<<<<<< HEAD

		rdev->raid_disk = -1;
		err = bind_rdev_to_array(rdev, mddev);
		if (!err && !mddev->pers->hot_remove_disk) {
			/* If there is hot_add_disk but no hot_remove_disk
			 * then added disks for geometry changes,
			 * and should be added immediately.
			 */
			super_types[mddev->major_version].
				validate_super(mddev, rdev);
			err = mddev->pers->hot_add_disk(mddev, rdev);
			if (err)
				unbind_rdev_from_array(rdev);
		}
		if (err)
			export_rdev(rdev);
		else
			sysfs_notify_dirent_safe(rdev->sysfs_state);

		md_update_sb(mddev, 1);
		if (mddev->degraded)
			set_bit(MD_RECOVERY_RECOVER, &mddev->recovery);
		set_bit(MD_RECOVERY_NEEDED, &mddev->recovery);
		if (!err)
			md_new_event(mddev);
		md_wakeup_thread(mddev->thread);
		return err;
	}

	/* otherwise, add_new_disk is only allowed
	 * for major_version==0 superblocks
	 */
	if (mddev->major_version != 0) {
		printk(KERN_WARNING "%s: ADD_NEW_DISK not supported\n",
		       mdname(mddev));
=======
		if (info->state & (1<<MD_DISK_FAILFAST))
			set_bit(FailFast, &rdev->flags);
		else
			clear_bit(FailFast, &rdev->flags);

		if (info->state & (1<<MD_DISK_JOURNAL)) {
			struct md_rdev *rdev2;
			bool has_journal = false;

			/* make sure no existing journal disk */
			rdev_for_each(rdev2, mddev) {
				if (test_bit(Journal, &rdev2->flags)) {
					has_journal = true;
					break;
				}
			}
			if (has_journal || mddev->bitmap) {
				export_rdev(rdev, mddev);
				return -EBUSY;
			}
			set_bit(Journal, &rdev->flags);
		}
		/*
		 * check whether the device shows up in other nodes
		 */
		if (mddev_is_clustered(mddev)) {
			if (info->state & (1 << MD_DISK_CANDIDATE))
				set_bit(Candidate, &rdev->flags);
			else if (info->state & (1 << MD_DISK_CLUSTER_ADD)) {
				/* --add initiated by this node */
				err = md_cluster_ops->add_new_disk(mddev, rdev);
				if (err) {
					export_rdev(rdev, mddev);
					return err;
				}
			}
		}

		rdev->raid_disk = -1;
		err = bind_rdev_to_array(rdev, mddev);

		if (err)
			export_rdev(rdev, mddev);

		if (mddev_is_clustered(mddev)) {
			if (info->state & (1 << MD_DISK_CANDIDATE)) {
				if (!err) {
					err = md_cluster_ops->new_disk_ack(mddev,
						err == 0);
					if (err)
						md_kick_rdev_from_array(rdev);
				}
			} else {
				if (err)
					md_cluster_ops->add_new_disk_cancel(mddev);
				else
					err = add_bound_rdev(rdev);
			}

		} else if (!err)
			err = add_bound_rdev(rdev);

		return err;
	}

	/* otherwise, md_add_new_disk is only allowed
	 * for major_version==0 superblocks
	 */
	if (mddev->major_version != 0) {
		pr_warn("%s: ADD_NEW_DISK not supported\n", mdname(mddev));
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		return -EINVAL;
	}

	if (!(info->state & (1<<MD_DISK_FAULTY))) {
		int err;
		rdev = md_import_device(dev, -1, 0);
		if (IS_ERR(rdev)) {
<<<<<<< HEAD
			printk(KERN_WARNING 
				"md: error, md_import_device() returned %ld\n",
=======
			pr_warn("md: error, md_import_device() returned %ld\n",
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
				PTR_ERR(rdev));
			return PTR_ERR(rdev);
		}
		rdev->desc_nr = info->number;
		if (info->raid_disk < mddev->raid_disks)
			rdev->raid_disk = info->raid_disk;
		else
			rdev->raid_disk = -1;

		if (rdev->raid_disk < mddev->raid_disks)
			if (info->state & (1<<MD_DISK_SYNC))
				set_bit(In_sync, &rdev->flags);

		if (info->state & (1<<MD_DISK_WRITEMOSTLY))
			set_bit(WriteMostly, &rdev->flags);
<<<<<<< HEAD

		if (!mddev->persistent) {
			printk(KERN_INFO "md: nonpersistent superblock ...\n");
			rdev->sb_start = i_size_read(rdev->bdev->bd_inode) / 512;
=======
		if (info->state & (1<<MD_DISK_FAILFAST))
			set_bit(FailFast, &rdev->flags);

		if (!mddev->persistent) {
			pr_debug("md: nonpersistent superblock ...\n");
			rdev->sb_start = bdev_nr_sectors(rdev->bdev);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		} else
			rdev->sb_start = calc_dev_sboffset(rdev);
		rdev->sectors = rdev->sb_start;

		err = bind_rdev_to_array(rdev, mddev);
		if (err) {
<<<<<<< HEAD
			export_rdev(rdev);
=======
			export_rdev(rdev, mddev);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
			return err;
		}
	}

	return 0;
}

<<<<<<< HEAD
static int hot_remove_disk(struct mddev * mddev, dev_t dev)
{
	char b[BDEVNAME_SIZE];
	struct md_rdev *rdev;

=======
static int hot_remove_disk(struct mddev *mddev, dev_t dev)
{
	struct md_rdev *rdev;

	if (!mddev->pers)
		return -ENODEV;

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	rdev = find_rdev(mddev, dev);
	if (!rdev)
		return -ENXIO;

<<<<<<< HEAD
	if (rdev->raid_disk >= 0)
		goto busy;

	kick_rdev_from_array(rdev);
	md_update_sb(mddev, 1);
	md_new_event(mddev);

	return 0;
busy:
	printk(KERN_WARNING "md: cannot remove active disk %s from %s ...\n",
		bdevname(rdev->bdev,b), mdname(mddev));
	return -EBUSY;
}

static int hot_add_disk(struct mddev * mddev, dev_t dev)
{
	char b[BDEVNAME_SIZE];
=======
	if (rdev->raid_disk < 0)
		goto kick_rdev;

	clear_bit(Blocked, &rdev->flags);
	remove_and_add_spares(mddev, rdev);

	if (rdev->raid_disk >= 0)
		goto busy;

kick_rdev:
	if (mddev_is_clustered(mddev)) {
		if (md_cluster_ops->remove_disk(mddev, rdev))
			goto busy;
	}

	md_kick_rdev_from_array(rdev);
	set_bit(MD_SB_CHANGE_DEVS, &mddev->sb_flags);
	if (!mddev->thread)
		md_update_sb(mddev, 1);
	md_new_event();

	return 0;
busy:
	pr_debug("md: cannot remove active disk %pg from %s ...\n",
		 rdev->bdev, mdname(mddev));
	return -EBUSY;
}

static int hot_add_disk(struct mddev *mddev, dev_t dev)
{
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	int err;
	struct md_rdev *rdev;

	if (!mddev->pers)
		return -ENODEV;

	if (mddev->major_version != 0) {
<<<<<<< HEAD
		printk(KERN_WARNING "%s: HOT_ADD may only be used with"
			" version-0 superblocks.\n",
=======
		pr_warn("%s: HOT_ADD may only be used with version-0 superblocks.\n",
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
			mdname(mddev));
		return -EINVAL;
	}
	if (!mddev->pers->hot_add_disk) {
<<<<<<< HEAD
		printk(KERN_WARNING 
			"%s: personality does not support diskops!\n",
=======
		pr_warn("%s: personality does not support diskops!\n",
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
			mdname(mddev));
		return -EINVAL;
	}

	rdev = md_import_device(dev, -1, 0);
	if (IS_ERR(rdev)) {
<<<<<<< HEAD
		printk(KERN_WARNING 
			"md: error, md_import_device() returned %ld\n",
=======
		pr_warn("md: error, md_import_device() returned %ld\n",
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
			PTR_ERR(rdev));
		return -EINVAL;
	}

	if (mddev->persistent)
		rdev->sb_start = calc_dev_sboffset(rdev);
	else
<<<<<<< HEAD
		rdev->sb_start = i_size_read(rdev->bdev->bd_inode) / 512;
=======
		rdev->sb_start = bdev_nr_sectors(rdev->bdev);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	rdev->sectors = rdev->sb_start;

	if (test_bit(Faulty, &rdev->flags)) {
<<<<<<< HEAD
		printk(KERN_WARNING 
			"md: can not hot-add faulty %s disk to %s!\n",
			bdevname(rdev->bdev,b), mdname(mddev));
		err = -EINVAL;
		goto abort_export;
	}
=======
		pr_warn("md: can not hot-add faulty %pg disk to %s!\n",
			rdev->bdev, mdname(mddev));
		err = -EINVAL;
		goto abort_export;
	}

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	clear_bit(In_sync, &rdev->flags);
	rdev->desc_nr = -1;
	rdev->saved_raid_disk = -1;
	err = bind_rdev_to_array(rdev, mddev);
	if (err)
		goto abort_export;

	/*
	 * The rest should better be atomic, we can have disk failures
	 * noticed in interrupt contexts ...
	 */

	rdev->raid_disk = -1;

<<<<<<< HEAD
	md_update_sb(mddev, 1);

=======
	set_bit(MD_SB_CHANGE_DEVS, &mddev->sb_flags);
	if (!mddev->thread)
		md_update_sb(mddev, 1);
	/*
	 * If the new disk does not support REQ_NOWAIT,
	 * disable on the whole MD.
	 */
	if (!bdev_nowait(rdev->bdev)) {
		pr_info("%s: Disabling nowait because %pg does not support nowait\n",
			mdname(mddev), rdev->bdev);
		blk_queue_flag_clear(QUEUE_FLAG_NOWAIT, mddev->gendisk->queue);
	}
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	/*
	 * Kick recovery, maybe this spare has to be added to the
	 * array immediately.
	 */
	set_bit(MD_RECOVERY_NEEDED, &mddev->recovery);
<<<<<<< HEAD
	md_wakeup_thread(mddev->thread);
	md_new_event(mddev);
	return 0;

abort_export:
	export_rdev(rdev);
=======
	md_new_event();
	return 0;

abort_export:
	export_rdev(rdev, mddev);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	return err;
}

static int set_bitmap_file(struct mddev *mddev, int fd)
{
<<<<<<< HEAD
	int err;

	if (mddev->pers) {
		if (!mddev->pers->quiesce)
=======
	int err = 0;

	if (mddev->pers) {
		if (!mddev->pers->quiesce || !mddev->thread)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
			return -EBUSY;
		if (mddev->recovery || mddev->sync_thread)
			return -EBUSY;
		/* we should be able to change the bitmap.. */
	}

<<<<<<< HEAD

	if (fd >= 0) {
		if (mddev->bitmap)
			return -EEXIST; /* cannot add when bitmap is present */
		mddev->bitmap_info.file = fget(fd);

		if (mddev->bitmap_info.file == NULL) {
			printk(KERN_ERR "%s: error: failed to get bitmap file\n",
			       mdname(mddev));
			return -EBADF;
		}

		err = deny_bitmap_write_access(mddev->bitmap_info.file);
		if (err) {
			printk(KERN_ERR "%s: error: bitmap file is already in use\n",
			       mdname(mddev));
			fput(mddev->bitmap_info.file);
			mddev->bitmap_info.file = NULL;
			return err;
		}
=======
	if (fd >= 0) {
		struct inode *inode;
		struct file *f;

		if (mddev->bitmap || mddev->bitmap_info.file)
			return -EEXIST; /* cannot add when bitmap is present */

		if (!IS_ENABLED(CONFIG_MD_BITMAP_FILE)) {
			pr_warn("%s: bitmap files not supported by this kernel\n",
				mdname(mddev));
			return -EINVAL;
		}
		pr_warn("%s: using deprecated bitmap file support\n",
			mdname(mddev));

		f = fget(fd);

		if (f == NULL) {
			pr_warn("%s: error: failed to get bitmap file\n",
				mdname(mddev));
			return -EBADF;
		}

		inode = f->f_mapping->host;
		if (!S_ISREG(inode->i_mode)) {
			pr_warn("%s: error: bitmap file must be a regular file\n",
				mdname(mddev));
			err = -EBADF;
		} else if (!(f->f_mode & FMODE_WRITE)) {
			pr_warn("%s: error: bitmap file must open for write\n",
				mdname(mddev));
			err = -EBADF;
		} else if (atomic_read(&inode->i_writecount) != 1) {
			pr_warn("%s: error: bitmap file is already in use\n",
				mdname(mddev));
			err = -EBUSY;
		}
		if (err) {
			fput(f);
			return err;
		}
		mddev->bitmap_info.file = f;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		mddev->bitmap_info.offset = 0; /* file overrides offset */
	} else if (mddev->bitmap == NULL)
		return -ENOENT; /* cannot remove what isn't there */
	err = 0;
	if (mddev->pers) {
<<<<<<< HEAD
		mddev->pers->quiesce(mddev, 1);
		if (fd >= 0) {
			err = bitmap_create(mddev);
			if (!err)
				err = bitmap_load(mddev);
		}
		if (fd < 0 || err) {
			bitmap_destroy(mddev);
			fd = -1; /* make sure to put the file */
		}
		mddev->pers->quiesce(mddev, 0);
	}
	if (fd < 0) {
		if (mddev->bitmap_info.file) {
			restore_bitmap_write_access(mddev->bitmap_info.file);
			fput(mddev->bitmap_info.file);
		}
		mddev->bitmap_info.file = NULL;
=======
		if (fd >= 0) {
			struct bitmap *bitmap;

			bitmap = md_bitmap_create(mddev, -1);
			if (!IS_ERR(bitmap)) {
				mddev->bitmap = bitmap;
				err = md_bitmap_load(mddev);
			} else
				err = PTR_ERR(bitmap);
			if (err) {
				md_bitmap_destroy(mddev);
				fd = -1;
			}
		} else if (fd < 0) {
			md_bitmap_destroy(mddev);
		}
	}
	if (fd < 0) {
		struct file *f = mddev->bitmap_info.file;
		if (f) {
			spin_lock(&mddev->lock);
			mddev->bitmap_info.file = NULL;
			spin_unlock(&mddev->lock);
			fput(f);
		}
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	}

	return err;
}

/*
<<<<<<< HEAD
 * set_array_info is used two different ways
=======
 * md_set_array_info is used two different ways
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 * The original usage is when creating a new array.
 * In this usage, raid_disks is > 0 and it together with
 *  level, size, not_persistent,layout,chunksize determine the
 *  shape of the array.
 *  This will always create an array with a type-0.90.0 superblock.
 * The newer usage is when assembling an array.
 *  In this case raid_disks will be 0, and the major_version field is
 *  use to determine which style super-blocks are to be found on the devices.
 *  The minor and patch _version numbers are also kept incase the
 *  super_block handler wishes to interpret them.
 */
<<<<<<< HEAD
static int set_array_info(struct mddev * mddev, mdu_array_info_t *info)
{

=======
int md_set_array_info(struct mddev *mddev, struct mdu_array_info_s *info)
{
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	if (info->raid_disks == 0) {
		/* just setting version number for superblock loading */
		if (info->major_version < 0 ||
		    info->major_version >= ARRAY_SIZE(super_types) ||
		    super_types[info->major_version].name == NULL) {
			/* maybe try to auto-load a module? */
<<<<<<< HEAD
			printk(KERN_INFO 
				"md: superblock version %d not known\n",
=======
			pr_warn("md: superblock version %d not known\n",
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
				info->major_version);
			return -EINVAL;
		}
		mddev->major_version = info->major_version;
		mddev->minor_version = info->minor_version;
		mddev->patch_version = info->patch_version;
		mddev->persistent = !info->not_persistent;
		/* ensure mddev_put doesn't delete this now that there
		 * is some minimal configuration.
		 */
<<<<<<< HEAD
		mddev->ctime         = get_seconds();
=======
		mddev->ctime         = ktime_get_real_seconds();
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		return 0;
	}
	mddev->major_version = MD_MAJOR_VERSION;
	mddev->minor_version = MD_MINOR_VERSION;
	mddev->patch_version = MD_PATCHLEVEL_VERSION;
<<<<<<< HEAD
	mddev->ctime         = get_seconds();
=======
	mddev->ctime         = ktime_get_real_seconds();
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	mddev->level         = info->level;
	mddev->clevel[0]     = 0;
	mddev->dev_sectors   = 2 * (sector_t)info->size;
	mddev->raid_disks    = info->raid_disks;
	/* don't set md_minor, it is determined by which /dev/md* was
	 * openned
	 */
	if (info->state & (1<<MD_SB_CLEAN))
		mddev->recovery_cp = MaxSector;
	else
		mddev->recovery_cp = 0;
	mddev->persistent    = ! info->not_persistent;
	mddev->external	     = 0;

	mddev->layout        = info->layout;
<<<<<<< HEAD
	mddev->chunk_sectors = info->chunk_size >> 9;

	mddev->max_disks     = MD_SB_DISKS;

	if (mddev->persistent)
		mddev->flags         = 0;
	set_bit(MD_CHANGE_DEVS, &mddev->flags);

	mddev->bitmap_info.default_offset = MD_SB_BYTES >> 9;
=======
	if (mddev->level == 0)
		/* Cannot trust RAID0 layout info here */
		mddev->layout = -1;
	mddev->chunk_sectors = info->chunk_size >> 9;

	if (mddev->persistent) {
		mddev->max_disks = MD_SB_DISKS;
		mddev->flags = 0;
		mddev->sb_flags = 0;
	}
	set_bit(MD_SB_CHANGE_DEVS, &mddev->sb_flags);

	mddev->bitmap_info.default_offset = MD_SB_BYTES >> 9;
	mddev->bitmap_info.default_space = 64*2 - (MD_SB_BYTES >> 9);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	mddev->bitmap_info.offset = 0;

	mddev->reshape_position = MaxSector;

	/*
	 * Generate a 128 bit UUID
	 */
	get_random_bytes(mddev->uuid, 16);

	mddev->new_level = mddev->level;
	mddev->new_chunk_sectors = mddev->chunk_sectors;
	mddev->new_layout = mddev->layout;
	mddev->delta_disks = 0;
<<<<<<< HEAD
=======
	mddev->reshape_backwards = 0;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	return 0;
}

void md_set_array_sectors(struct mddev *mddev, sector_t array_sectors)
{
<<<<<<< HEAD
	WARN(!mddev_is_locked(mddev), "%s: unlocked mddev!\n", __func__);
=======
	lockdep_assert_held(&mddev->reconfig_mutex);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	if (mddev->external_size)
		return;

	mddev->array_sectors = array_sectors;
}
EXPORT_SYMBOL(md_set_array_sectors);

static int update_size(struct mddev *mddev, sector_t num_sectors)
{
	struct md_rdev *rdev;
	int rv;
	int fit = (num_sectors == 0);
<<<<<<< HEAD
=======
	sector_t old_dev_sectors = mddev->dev_sectors;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	if (mddev->pers->resize == NULL)
		return -EINVAL;
	/* The "num_sectors" is the number of sectors of each device that
	 * is used.  This can only make sense for arrays with redundancy.
	 * linear and raid0 always use whatever space is available. We can only
	 * consider changing this number if no resync or reconstruction is
	 * happening, and if the new size is acceptable. It must fit before the
	 * sb_start or, if that is <data_offset, it must fit before the size
	 * of each device.  If num_sectors is zero, we find the largest size
	 * that fits.
	 */
<<<<<<< HEAD
	if (mddev->sync_thread)
		return -EBUSY;
	if (mddev->bitmap)
		/* Sorry, cannot grow a bitmap yet, just remove it,
		 * grow, and re-add.
		 */
		return -EBUSY;
=======
	if (test_bit(MD_RECOVERY_RUNNING, &mddev->recovery))
		return -EBUSY;
	if (!md_is_rdwr(mddev))
		return -EROFS;

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	rdev_for_each(rdev, mddev) {
		sector_t avail = rdev->sectors;

		if (fit && (num_sectors == 0 || num_sectors > avail))
			num_sectors = avail;
		if (avail < num_sectors)
			return -ENOSPC;
	}
	rv = mddev->pers->resize(mddev, num_sectors);
<<<<<<< HEAD
	if (!rv)
		revalidate_disk(mddev->gendisk);
=======
	if (!rv) {
		if (mddev_is_clustered(mddev))
			md_cluster_ops->update_size(mddev, old_dev_sectors);
		else if (!mddev_is_dm(mddev))
			set_capacity_and_notify(mddev->gendisk,
						mddev->array_sectors);
	}
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	return rv;
}

static int update_raid_disks(struct mddev *mddev, int raid_disks)
{
	int rv;
<<<<<<< HEAD
	/* change the number of raid disks */
	if (mddev->pers->check_reshape == NULL)
		return -EINVAL;
	if (raid_disks <= 0 ||
	    (mddev->max_disks && raid_disks >= mddev->max_disks))
		return -EINVAL;
	if (mddev->sync_thread || mddev->reshape_position != MaxSector)
		return -EBUSY;
	mddev->delta_disks = raid_disks - mddev->raid_disks;

	rv = mddev->pers->check_reshape(mddev);
	if (rv < 0)
		mddev->delta_disks = 0;
	return rv;
}


=======
	struct md_rdev *rdev;
	/* change the number of raid disks */
	if (mddev->pers->check_reshape == NULL)
		return -EINVAL;
	if (!md_is_rdwr(mddev))
		return -EROFS;
	if (raid_disks <= 0 ||
	    (mddev->max_disks && raid_disks >= mddev->max_disks))
		return -EINVAL;
	if (test_bit(MD_RECOVERY_RUNNING, &mddev->recovery) ||
	    test_bit(MD_RESYNCING_REMOTE, &mddev->recovery) ||
	    mddev->reshape_position != MaxSector)
		return -EBUSY;

	rdev_for_each(rdev, mddev) {
		if (mddev->raid_disks < raid_disks &&
		    rdev->data_offset < rdev->new_data_offset)
			return -EINVAL;
		if (mddev->raid_disks > raid_disks &&
		    rdev->data_offset > rdev->new_data_offset)
			return -EINVAL;
	}

	mddev->delta_disks = raid_disks - mddev->raid_disks;
	if (mddev->delta_disks < 0)
		mddev->reshape_backwards = 1;
	else if (mddev->delta_disks > 0)
		mddev->reshape_backwards = 0;

	rv = mddev->pers->check_reshape(mddev);
	if (rv < 0) {
		mddev->delta_disks = 0;
		mddev->reshape_backwards = 0;
	}
	return rv;
}

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
/*
 * update_array_info is used to change the configuration of an
 * on-line array.
 * The version, ctime,level,size,raid_disks,not_persistent, layout,chunk_size
 * fields in the info are checked against the array.
 * Any differences that cannot be handled will cause an error.
 * Normally, only one change can be managed at a time.
 */
static int update_array_info(struct mddev *mddev, mdu_array_info_t *info)
{
	int rv = 0;
	int cnt = 0;
	int state = 0;

	/* calculate expected state,ignoring low bits */
	if (mddev->bitmap && mddev->bitmap_info.offset)
		state |= (1 << MD_SB_BITMAP_PRESENT);

	if (mddev->major_version != info->major_version ||
	    mddev->minor_version != info->minor_version ||
/*	    mddev->patch_version != info->patch_version || */
	    mddev->ctime         != info->ctime         ||
	    mddev->level         != info->level         ||
/*	    mddev->layout        != info->layout        || */
<<<<<<< HEAD
	    !mddev->persistent	 != info->not_persistent||
=======
	    mddev->persistent	 != !info->not_persistent ||
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	    mddev->chunk_sectors != info->chunk_size >> 9 ||
	    /* ignore bottom 8 bits of state, and allow SB_BITMAP_PRESENT to change */
	    ((state^info->state) & 0xfffffe00)
		)
		return -EINVAL;
	/* Check there is only one change */
	if (info->size >= 0 && mddev->dev_sectors / 2 != info->size)
		cnt++;
	if (mddev->raid_disks != info->raid_disks)
		cnt++;
	if (mddev->layout != info->layout)
		cnt++;
	if ((state ^ info->state) & (1<<MD_SB_BITMAP_PRESENT))
		cnt++;
	if (cnt == 0)
		return 0;
	if (cnt > 1)
		return -EINVAL;

	if (mddev->layout != info->layout) {
		/* Change layout
		 * we don't need to do anything at the md level, the
		 * personality will take care of it all.
		 */
		if (mddev->pers->check_reshape == NULL)
			return -EINVAL;
		else {
			mddev->new_layout = info->layout;
			rv = mddev->pers->check_reshape(mddev);
			if (rv)
				mddev->new_layout = mddev->layout;
			return rv;
		}
	}
	if (info->size >= 0 && mddev->dev_sectors / 2 != info->size)
		rv = update_size(mddev, (sector_t)info->size * 2);

	if (mddev->raid_disks    != info->raid_disks)
		rv = update_raid_disks(mddev, info->raid_disks);

	if ((state ^ info->state) & (1<<MD_SB_BITMAP_PRESENT)) {
<<<<<<< HEAD
		if (mddev->pers->quiesce == NULL)
			return -EINVAL;
		if (mddev->recovery || mddev->sync_thread)
			return -EBUSY;
		if (info->state & (1<<MD_SB_BITMAP_PRESENT)) {
			/* add the bitmap */
			if (mddev->bitmap)
				return -EEXIST;
			if (mddev->bitmap_info.default_offset == 0)
				return -EINVAL;
			mddev->bitmap_info.offset =
				mddev->bitmap_info.default_offset;
			mddev->pers->quiesce(mddev, 1);
			rv = bitmap_create(mddev);
			if (!rv)
				rv = bitmap_load(mddev);
			if (rv)
				bitmap_destroy(mddev);
			mddev->pers->quiesce(mddev, 0);
		} else {
			/* remove the bitmap */
			if (!mddev->bitmap)
				return -ENOENT;
			if (mddev->bitmap->file)
				return -EINVAL;
			mddev->pers->quiesce(mddev, 1);
			bitmap_destroy(mddev);
			mddev->pers->quiesce(mddev, 0);
=======
		if (mddev->pers->quiesce == NULL || mddev->thread == NULL) {
			rv = -EINVAL;
			goto err;
		}
		if (mddev->recovery || mddev->sync_thread) {
			rv = -EBUSY;
			goto err;
		}
		if (info->state & (1<<MD_SB_BITMAP_PRESENT)) {
			struct bitmap *bitmap;
			/* add the bitmap */
			if (mddev->bitmap) {
				rv = -EEXIST;
				goto err;
			}
			if (mddev->bitmap_info.default_offset == 0) {
				rv = -EINVAL;
				goto err;
			}
			mddev->bitmap_info.offset =
				mddev->bitmap_info.default_offset;
			mddev->bitmap_info.space =
				mddev->bitmap_info.default_space;
			bitmap = md_bitmap_create(mddev, -1);
			if (!IS_ERR(bitmap)) {
				mddev->bitmap = bitmap;
				rv = md_bitmap_load(mddev);
			} else
				rv = PTR_ERR(bitmap);
			if (rv)
				md_bitmap_destroy(mddev);
		} else {
			/* remove the bitmap */
			if (!mddev->bitmap) {
				rv = -ENOENT;
				goto err;
			}
			if (mddev->bitmap->storage.file) {
				rv = -EINVAL;
				goto err;
			}
			if (mddev->bitmap_info.nodes) {
				/* hold PW on all the bitmap lock */
				if (md_cluster_ops->lock_all_bitmaps(mddev) <= 0) {
					pr_warn("md: can't change bitmap to none since the array is in use by more than one node\n");
					rv = -EPERM;
					md_cluster_ops->unlock_all_bitmaps(mddev);
					goto err;
				}

				mddev->bitmap_info.nodes = 0;
				md_cluster_ops->leave(mddev);
				module_put(md_cluster_mod);
				mddev->safemode_delay = DEFAULT_SAFEMODE_DELAY;
			}
			md_bitmap_destroy(mddev);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
			mddev->bitmap_info.offset = 0;
		}
	}
	md_update_sb(mddev, 1);
	return rv;
<<<<<<< HEAD
=======
err:
	return rv;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

static int set_disk_faulty(struct mddev *mddev, dev_t dev)
{
	struct md_rdev *rdev;
<<<<<<< HEAD
=======
	int err = 0;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	if (mddev->pers == NULL)
		return -ENODEV;

<<<<<<< HEAD
	rdev = find_rdev(mddev, dev);
	if (!rdev)
		return -ENODEV;

	md_error(mddev, rdev);
	if (!test_bit(Faulty, &rdev->flags))
		return -EBUSY;
	return 0;
=======
	rcu_read_lock();
	rdev = md_find_rdev_rcu(mddev, dev);
	if (!rdev)
		err =  -ENODEV;
	else {
		md_error(mddev, rdev);
		if (test_bit(MD_BROKEN, &mddev->flags))
			err = -EBUSY;
	}
	rcu_read_unlock();
	return err;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

/*
 * We have a problem here : there is no easy way to give a CHS
 * virtual geometry. We currently pretend that we have a 2 heads
 * 4 sectors (with a BIG number of cylinders...). This drives
 * dosfs just mad... ;-)
 */
static int md_getgeo(struct block_device *bdev, struct hd_geometry *geo)
{
	struct mddev *mddev = bdev->bd_disk->private_data;

	geo->heads = 2;
	geo->sectors = 4;
	geo->cylinders = mddev->array_sectors / 8;
	return 0;
}

<<<<<<< HEAD
static int md_ioctl(struct block_device *bdev, fmode_t mode,
=======
static inline int md_ioctl_valid(unsigned int cmd)
{
	switch (cmd) {
	case GET_ARRAY_INFO:
	case GET_DISK_INFO:
	case RAID_VERSION:
		return 0;
	case ADD_NEW_DISK:
	case GET_BITMAP_FILE:
	case HOT_ADD_DISK:
	case HOT_REMOVE_DISK:
	case RESTART_ARRAY_RW:
	case RUN_ARRAY:
	case SET_ARRAY_INFO:
	case SET_BITMAP_FILE:
	case SET_DISK_FAULTY:
	case STOP_ARRAY:
	case STOP_ARRAY_RO:
	case CLUSTERED_DISK_NACK:
		if (!capable(CAP_SYS_ADMIN))
			return -EACCES;
		return 0;
	default:
		return -ENOTTY;
	}
}

static bool md_ioctl_need_suspend(unsigned int cmd)
{
	switch (cmd) {
	case ADD_NEW_DISK:
	case HOT_ADD_DISK:
	case HOT_REMOVE_DISK:
	case SET_BITMAP_FILE:
	case SET_ARRAY_INFO:
		return true;
	default:
		return false;
	}
}

static int __md_set_array_info(struct mddev *mddev, void __user *argp)
{
	mdu_array_info_t info;
	int err;

	if (!argp)
		memset(&info, 0, sizeof(info));
	else if (copy_from_user(&info, argp, sizeof(info)))
		return -EFAULT;

	if (mddev->pers) {
		err = update_array_info(mddev, &info);
		if (err)
			pr_warn("md: couldn't update array info. %d\n", err);
		return err;
	}

	if (!list_empty(&mddev->disks)) {
		pr_warn("md: array %s already has disks!\n", mdname(mddev));
		return -EBUSY;
	}

	if (mddev->raid_disks) {
		pr_warn("md: array %s already initialised!\n", mdname(mddev));
		return -EBUSY;
	}

	err = md_set_array_info(mddev, &info);
	if (err)
		pr_warn("md: couldn't set array info. %d\n", err);

	return err;
}

static int md_ioctl(struct block_device *bdev, blk_mode_t mode,
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
			unsigned int cmd, unsigned long arg)
{
	int err = 0;
	void __user *argp = (void __user *)arg;
	struct mddev *mddev = NULL;
<<<<<<< HEAD
	int ro;

	switch (cmd) {
	case RAID_VERSION:
	case GET_ARRAY_INFO:
	case GET_DISK_INFO:
		break;
	default:
		if (!capable(CAP_SYS_ADMIN))
			return -EACCES;
	}
=======

	err = md_ioctl_valid(cmd);
	if (err)
		return err;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	/*
	 * Commands dealing with the RAID driver but not any
	 * particular array:
	 */
<<<<<<< HEAD
	switch (cmd)
	{
		case RAID_VERSION:
			err = get_version(argp);
			goto done;

		case PRINT_RAID_DEBUG:
			err = 0;
			md_print_devices();
			goto done;

#ifndef MODULE
		case RAID_AUTORUN:
			err = 0;
			autostart_arrays(arg);
			goto done;
#endif
		default:;
	}
=======
	if (cmd == RAID_VERSION)
		return get_version(argp);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	/*
	 * Commands creating/starting a new array:
	 */

	mddev = bdev->bd_disk->private_data;

<<<<<<< HEAD
	if (!mddev) {
		BUG();
		goto abort;
	}

	err = mddev_lock(mddev);
	if (err) {
		printk(KERN_INFO 
			"md: ioctl lock interrupted, reason %d, cmd %d\n",
			err, cmd);
		goto abort;
	}

	switch (cmd)
	{
		case SET_ARRAY_INFO:
			{
				mdu_array_info_t info;
				if (!arg)
					memset(&info, 0, sizeof(info));
				else if (copy_from_user(&info, argp, sizeof(info))) {
					err = -EFAULT;
					goto abort_unlock;
				}
				if (mddev->pers) {
					err = update_array_info(mddev, &info);
					if (err) {
						printk(KERN_WARNING "md: couldn't update"
						       " array info. %d\n", err);
						goto abort_unlock;
					}
					goto done_unlock;
				}
				if (!list_empty(&mddev->disks)) {
					printk(KERN_WARNING
					       "md: array %s already has disks!\n",
					       mdname(mddev));
					err = -EBUSY;
					goto abort_unlock;
				}
				if (mddev->raid_disks) {
					printk(KERN_WARNING
					       "md: array %s already initialised!\n",
					       mdname(mddev));
					err = -EBUSY;
					goto abort_unlock;
				}
				err = set_array_info(mddev, &info);
				if (err) {
					printk(KERN_WARNING "md: couldn't set"
					       " array info. %d\n", err);
					goto abort_unlock;
				}
			}
			goto done_unlock;

		default:;
=======
	/* Some actions do not requires the mutex */
	switch (cmd) {
	case GET_ARRAY_INFO:
		if (!mddev->raid_disks && !mddev->external)
			return -ENODEV;
		return get_array_info(mddev, argp);

	case GET_DISK_INFO:
		if (!mddev->raid_disks && !mddev->external)
			return -ENODEV;
		return get_disk_info(mddev, argp);

	case SET_DISK_FAULTY:
		return set_disk_faulty(mddev, new_decode_dev(arg));

	case GET_BITMAP_FILE:
		return get_bitmap_file(mddev, argp);
	}

	if (cmd == HOT_REMOVE_DISK)
		/* need to ensure recovery thread has run */
		wait_event_interruptible_timeout(mddev->sb_wait,
						 !test_bit(MD_RECOVERY_NEEDED,
							   &mddev->recovery),
						 msecs_to_jiffies(5000));
	if (cmd == STOP_ARRAY || cmd == STOP_ARRAY_RO) {
		/* Need to flush page cache, and ensure no-one else opens
		 * and writes
		 */
		err = mddev_set_closing_and_sync_blockdev(mddev, 1);
		if (err)
			return err;
	}

	if (!md_is_rdwr(mddev))
		flush_work(&mddev->sync_work);

	err = md_ioctl_need_suspend(cmd) ? mddev_suspend_and_lock(mddev) :
					   mddev_lock(mddev);
	if (err) {
		pr_debug("md: ioctl lock interrupted, reason %d, cmd %d\n",
			 err, cmd);
		goto out;
	}

	if (cmd == SET_ARRAY_INFO) {
		err = __md_set_array_info(mddev, argp);
		goto unlock;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	}

	/*
	 * Commands querying/configuring an existing array:
	 */
	/* if we are not initialised yet, only ADD_NEW_DISK, STOP_ARRAY,
	 * RUN_ARRAY, and GET_ and SET_BITMAP_FILE are allowed */
	if ((!mddev->raid_disks && !mddev->external)
	    && cmd != ADD_NEW_DISK && cmd != STOP_ARRAY
	    && cmd != RUN_ARRAY && cmd != SET_BITMAP_FILE
	    && cmd != GET_BITMAP_FILE) {
		err = -ENODEV;
<<<<<<< HEAD
		goto abort_unlock;
=======
		goto unlock;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	}

	/*
	 * Commands even a read-only array can execute:
	 */
<<<<<<< HEAD
	switch (cmd)
	{
		case GET_ARRAY_INFO:
			err = get_array_info(mddev, argp);
			goto done_unlock;

		case GET_BITMAP_FILE:
			err = get_bitmap_file(mddev, argp);
			goto done_unlock;

		case GET_DISK_INFO:
			err = get_disk_info(mddev, argp);
			goto done_unlock;

		case RESTART_ARRAY_RW:
			err = restart_array(mddev);
			goto done_unlock;

		case STOP_ARRAY:
			err = do_md_stop(mddev, 0, bdev);
			goto done_unlock;

		case STOP_ARRAY_RO:
			err = md_set_readonly(mddev, bdev);
			goto done_unlock;

		case BLKROSET:
			if (get_user(ro, (int __user *)(arg))) {
				err = -EFAULT;
				goto done_unlock;
			}
			err = -EINVAL;

			/* if the bdev is going readonly the value of mddev->ro
			 * does not matter, no writes are coming
			 */
			if (ro)
				goto done_unlock;

			/* are we are already prepared for writes? */
			if (mddev->ro != 1)
				goto done_unlock;

			/* transitioning to readauto need only happen for
			 * arrays that call md_write_start
			 */
			if (mddev->pers) {
				err = restart_array(mddev);
				if (err == 0) {
					mddev->ro = 2;
					set_disk_ro(mddev->gendisk, 0);
				}
			}
			goto done_unlock;
=======
	switch (cmd) {
	case RESTART_ARRAY_RW:
		err = restart_array(mddev);
		goto unlock;

	case STOP_ARRAY:
		err = do_md_stop(mddev, 0);
		goto unlock;

	case STOP_ARRAY_RO:
		if (mddev->pers)
			err = md_set_readonly(mddev);
		goto unlock;

	case HOT_REMOVE_DISK:
		err = hot_remove_disk(mddev, new_decode_dev(arg));
		goto unlock;

	case ADD_NEW_DISK:
		/* We can support ADD_NEW_DISK on read-only arrays
		 * only if we are re-adding a preexisting device.
		 * So require mddev->pers and MD_DISK_SYNC.
		 */
		if (mddev->pers) {
			mdu_disk_info_t info;
			if (copy_from_user(&info, argp, sizeof(info)))
				err = -EFAULT;
			else if (!(info.state & (1<<MD_DISK_SYNC)))
				/* Need to clear read-only for this */
				break;
			else
				err = md_add_new_disk(mddev, &info);
			goto unlock;
		}
		break;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	}

	/*
	 * The remaining ioctls are changing the state of the
	 * superblock, so we do not allow them on read-only arrays.
<<<<<<< HEAD
	 * However non-MD ioctls (e.g. get-size) will still come through
	 * here and hit the 'default' below, so only disallow
	 * 'md' ioctls, and switch to rw mode if started auto-readonly.
	 */
	if (_IOC_TYPE(cmd) == MD_MAJOR && mddev->ro && mddev->pers) {
		if (mddev->ro == 2) {
			mddev->ro = 0;
			sysfs_notify_dirent_safe(mddev->sysfs_state);
			set_bit(MD_RECOVERY_NEEDED, &mddev->recovery);
			md_wakeup_thread(mddev->thread);
		} else {
			err = -EROFS;
			goto abort_unlock;
		}
	}

	switch (cmd)
	{
		case ADD_NEW_DISK:
		{
			mdu_disk_info_t info;
			if (copy_from_user(&info, argp, sizeof(info)))
				err = -EFAULT;
			else
				err = add_new_disk(mddev, &info);
			goto done_unlock;
		}

		case HOT_REMOVE_DISK:
			err = hot_remove_disk(mddev, new_decode_dev(arg));
			goto done_unlock;

		case HOT_ADD_DISK:
			err = hot_add_disk(mddev, new_decode_dev(arg));
			goto done_unlock;

		case SET_DISK_FAULTY:
			err = set_disk_faulty(mddev, new_decode_dev(arg));
			goto done_unlock;

		case RUN_ARRAY:
			err = do_md_run(mddev);
			goto done_unlock;

		case SET_BITMAP_FILE:
			err = set_bitmap_file(mddev, (int)arg);
			goto done_unlock;

		default:
			err = -EINVAL;
			goto abort_unlock;
	}

done_unlock:
abort_unlock:
	if (mddev->hold_active == UNTIL_IOCTL &&
	    err != -EINVAL)
		mddev->hold_active = 0;
	mddev_unlock(mddev);

	return err;
done:
	if (err)
		MD_BUG();
abort:
	return err;
}
#ifdef CONFIG_COMPAT
static int md_compat_ioctl(struct block_device *bdev, fmode_t mode,
=======
	 */
	if (!md_is_rdwr(mddev) && mddev->pers) {
		if (mddev->ro != MD_AUTO_READ) {
			err = -EROFS;
			goto unlock;
		}
		mddev->ro = MD_RDWR;
		sysfs_notify_dirent_safe(mddev->sysfs_state);
		set_bit(MD_RECOVERY_NEEDED, &mddev->recovery);
		/* mddev_unlock will wake thread */
		/* If a device failed while we were read-only, we
		 * need to make sure the metadata is updated now.
		 */
		if (test_bit(MD_SB_CHANGE_DEVS, &mddev->sb_flags)) {
			mddev_unlock(mddev);
			wait_event(mddev->sb_wait,
				   !test_bit(MD_SB_CHANGE_DEVS, &mddev->sb_flags) &&
				   !test_bit(MD_SB_CHANGE_PENDING, &mddev->sb_flags));
			mddev_lock_nointr(mddev);
		}
	}

	switch (cmd) {
	case ADD_NEW_DISK:
	{
		mdu_disk_info_t info;
		if (copy_from_user(&info, argp, sizeof(info)))
			err = -EFAULT;
		else
			err = md_add_new_disk(mddev, &info);
		goto unlock;
	}

	case CLUSTERED_DISK_NACK:
		if (mddev_is_clustered(mddev))
			md_cluster_ops->new_disk_ack(mddev, false);
		else
			err = -EINVAL;
		goto unlock;

	case HOT_ADD_DISK:
		err = hot_add_disk(mddev, new_decode_dev(arg));
		goto unlock;

	case RUN_ARRAY:
		err = do_md_run(mddev);
		goto unlock;

	case SET_BITMAP_FILE:
		err = set_bitmap_file(mddev, (int)arg);
		goto unlock;

	default:
		err = -EINVAL;
		goto unlock;
	}

unlock:
	if (mddev->hold_active == UNTIL_IOCTL &&
	    err != -EINVAL)
		mddev->hold_active = 0;

	md_ioctl_need_suspend(cmd) ? mddev_unlock_and_resume(mddev) :
				     mddev_unlock(mddev);

out:
	if (cmd == STOP_ARRAY_RO || (err && cmd == STOP_ARRAY))
		clear_bit(MD_CLOSING, &mddev->flags);
	return err;
}
#ifdef CONFIG_COMPAT
static int md_compat_ioctl(struct block_device *bdev, blk_mode_t mode,
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		    unsigned int cmd, unsigned long arg)
{
	switch (cmd) {
	case HOT_REMOVE_DISK:
	case HOT_ADD_DISK:
	case SET_DISK_FAULTY:
	case SET_BITMAP_FILE:
		/* These take in integer arg, do not convert */
		break;
	default:
		arg = (unsigned long)compat_ptr(arg);
		break;
	}

	return md_ioctl(bdev, mode, cmd, arg);
}
#endif /* CONFIG_COMPAT */

<<<<<<< HEAD
static int md_open(struct block_device *bdev, fmode_t mode)
{
	/*
	 * Succeed if we can lock the mddev, which confirms that
	 * it isn't being stopped right now.
	 */
	struct mddev *mddev = mddev_find(bdev->bd_dev);
	int err;

	if (mddev->gendisk != bdev->bd_disk) {
		/* we are racing with mddev_put which is discarding this
		 * bd_disk.
		 */
		mddev_put(mddev);
		/* Wait until bdev->bd_disk is definitely gone */
		flush_workqueue(md_misc_wq);
		/* Then retry the open from the top */
		return -ERESTARTSYS;
	}
	BUG_ON(mddev != bdev->bd_disk->private_data);

	if ((err = mutex_lock_interruptible(&mddev->open_mutex)))
		goto out;

	err = 0;
	atomic_inc(&mddev->openers);
	mutex_unlock(&mddev->open_mutex);

	check_disk_change(bdev);
 out:
	return err;
}

static int md_release(struct gendisk *disk, fmode_t mode)
{
 	struct mddev *mddev = disk->private_data;
=======
static int md_set_read_only(struct block_device *bdev, bool ro)
{
	struct mddev *mddev = bdev->bd_disk->private_data;
	int err;

	err = mddev_lock(mddev);
	if (err)
		return err;

	if (!mddev->raid_disks && !mddev->external) {
		err = -ENODEV;
		goto out_unlock;
	}

	/*
	 * Transitioning to read-auto need only happen for arrays that call
	 * md_write_start and which are not ready for writes yet.
	 */
	if (!ro && mddev->ro == MD_RDONLY && mddev->pers) {
		err = restart_array(mddev);
		if (err)
			goto out_unlock;
		mddev->ro = MD_AUTO_READ;
	}

out_unlock:
	mddev_unlock(mddev);
	return err;
}

static int md_open(struct gendisk *disk, blk_mode_t mode)
{
	struct mddev *mddev;
	int err;

	spin_lock(&all_mddevs_lock);
	mddev = mddev_get(disk->private_data);
	spin_unlock(&all_mddevs_lock);
	if (!mddev)
		return -ENODEV;

	err = mutex_lock_interruptible(&mddev->open_mutex);
	if (err)
		goto out;

	err = -ENODEV;
	if (test_bit(MD_CLOSING, &mddev->flags))
		goto out_unlock;

	atomic_inc(&mddev->openers);
	mutex_unlock(&mddev->open_mutex);

	disk_check_media_change(disk);
	return 0;

out_unlock:
	mutex_unlock(&mddev->open_mutex);
out:
	mddev_put(mddev);
	return err;
}

static void md_release(struct gendisk *disk)
{
	struct mddev *mddev = disk->private_data;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	BUG_ON(!mddev);
	atomic_dec(&mddev->openers);
	mddev_put(mddev);
<<<<<<< HEAD

	return 0;
}

static int md_media_changed(struct gendisk *disk)
{
	struct mddev *mddev = disk->private_data;

	return mddev->changed;
}

static int md_revalidate(struct gendisk *disk)
{
	struct mddev *mddev = disk->private_data;

	mddev->changed = 0;
	return 0;
}
static const struct block_device_operations md_fops =
{
	.owner		= THIS_MODULE,
=======
}

static unsigned int md_check_events(struct gendisk *disk, unsigned int clearing)
{
	struct mddev *mddev = disk->private_data;
	unsigned int ret = 0;

	if (mddev->changed)
		ret = DISK_EVENT_MEDIA_CHANGE;
	mddev->changed = 0;
	return ret;
}

static void md_free_disk(struct gendisk *disk)
{
	struct mddev *mddev = disk->private_data;

	mddev_free(mddev);
}

const struct block_device_operations md_fops =
{
	.owner		= THIS_MODULE,
	.submit_bio	= md_submit_bio,
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	.open		= md_open,
	.release	= md_release,
	.ioctl		= md_ioctl,
#ifdef CONFIG_COMPAT
	.compat_ioctl	= md_compat_ioctl,
#endif
	.getgeo		= md_getgeo,
<<<<<<< HEAD
	.media_changed  = md_media_changed,
	.revalidate_disk= md_revalidate,
};

static int md_thread(void * arg)
=======
	.check_events	= md_check_events,
	.set_read_only	= md_set_read_only,
	.free_disk	= md_free_disk,
};

static int md_thread(void *arg)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
{
	struct md_thread *thread = arg;

	/*
	 * md_thread is a 'system-thread', it's priority should be very
	 * high. We avoid resource deadlocks individually in each
	 * raid personality. (RAID5 does preallocation) We also use RR and
	 * the very same RT priority as kswapd, thus we will never get
	 * into a priority inversion deadlock.
	 *
	 * we definitely have to have equal or higher priority than
	 * bdflush, otherwise bdflush will deadlock if there are too
	 * many dirty RAID5 blocks.
	 */

	allow_signal(SIGKILL);
	while (!kthread_should_stop()) {

		/* We need to wait INTERRUPTIBLE so that
		 * we don't add to the load-average.
		 * That means we need to be sure no signals are
		 * pending
		 */
		if (signal_pending(current))
			flush_signals(current);

		wait_event_interruptible_timeout
			(thread->wqueue,
			 test_bit(THREAD_WAKEUP, &thread->flags)
<<<<<<< HEAD
			 || kthread_should_stop(),
			 thread->timeout);

		clear_bit(THREAD_WAKEUP, &thread->flags);
		if (!kthread_should_stop())
			thread->run(thread->mddev);
=======
			 || kthread_should_stop() || kthread_should_park(),
			 thread->timeout);

		clear_bit(THREAD_WAKEUP, &thread->flags);
		if (kthread_should_park())
			kthread_parkme();
		if (!kthread_should_stop())
			thread->run(thread);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	}

	return 0;
}

<<<<<<< HEAD
void md_wakeup_thread(struct md_thread *thread)
{
	if (thread) {
		pr_debug("md: waking up MD thread %s.\n", thread->tsk->comm);
		set_bit(THREAD_WAKEUP, &thread->flags);
		wake_up(&thread->wqueue);
	}
}

struct md_thread *md_register_thread(void (*run) (struct mddev *), struct mddev *mddev,
				 const char *name)
=======
static void md_wakeup_thread_directly(struct md_thread __rcu *thread)
{
	struct md_thread *t;

	rcu_read_lock();
	t = rcu_dereference(thread);
	if (t)
		wake_up_process(t->tsk);
	rcu_read_unlock();
}

void md_wakeup_thread(struct md_thread __rcu *thread)
{
	struct md_thread *t;

	rcu_read_lock();
	t = rcu_dereference(thread);
	if (t) {
		pr_debug("md: waking up MD thread %s.\n", t->tsk->comm);
		set_bit(THREAD_WAKEUP, &t->flags);
		wake_up(&t->wqueue);
	}
	rcu_read_unlock();
}
EXPORT_SYMBOL(md_wakeup_thread);

struct md_thread *md_register_thread(void (*run) (struct md_thread *),
		struct mddev *mddev, const char *name)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
{
	struct md_thread *thread;

	thread = kzalloc(sizeof(struct md_thread), GFP_KERNEL);
	if (!thread)
		return NULL;

	init_waitqueue_head(&thread->wqueue);

	thread->run = run;
	thread->mddev = mddev;
	thread->timeout = MAX_SCHEDULE_TIMEOUT;
	thread->tsk = kthread_run(md_thread, thread,
				  "%s_%s",
				  mdname(thread->mddev),
<<<<<<< HEAD
				  name ?: mddev->pers->name);
=======
				  name);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	if (IS_ERR(thread->tsk)) {
		kfree(thread);
		return NULL;
	}
	return thread;
}
<<<<<<< HEAD

void md_unregister_thread(struct md_thread **threadp)
{
	struct md_thread *thread = *threadp;
	if (!thread)
		return;
	pr_debug("interrupting MD-thread pid %d\n", task_pid_nr(thread->tsk));
	/* Locking ensures that mddev_unlock does not wake_up a
	 * non-existent thread
	 */
	spin_lock(&pers_lock);
	*threadp = NULL;
	spin_unlock(&pers_lock);

	kthread_stop(thread->tsk);
	kfree(thread);
}

void md_error(struct mddev *mddev, struct md_rdev *rdev)
{
	if (!mddev) {
		MD_BUG();
		return;
	}

=======
EXPORT_SYMBOL(md_register_thread);

void md_unregister_thread(struct mddev *mddev, struct md_thread __rcu **threadp)
{
	struct md_thread *thread = rcu_dereference_protected(*threadp,
					lockdep_is_held(&mddev->reconfig_mutex));

	if (!thread)
		return;

	rcu_assign_pointer(*threadp, NULL);
	synchronize_rcu();

	pr_debug("interrupting MD-thread pid %d\n", task_pid_nr(thread->tsk));
	kthread_stop(thread->tsk);
	kfree(thread);
}
EXPORT_SYMBOL(md_unregister_thread);

void md_error(struct mddev *mddev, struct md_rdev *rdev)
{
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	if (!rdev || test_bit(Faulty, &rdev->flags))
		return;

	if (!mddev->pers || !mddev->pers->error_handler)
		return;
<<<<<<< HEAD
	mddev->pers->error_handler(mddev,rdev);
	if (mddev->degraded)
		set_bit(MD_RECOVERY_RECOVER, &mddev->recovery);
	sysfs_notify_dirent_safe(rdev->sysfs_state);
	set_bit(MD_RECOVERY_INTR, &mddev->recovery);
	set_bit(MD_RECOVERY_NEEDED, &mddev->recovery);
	md_wakeup_thread(mddev->thread);
	if (mddev->event_work.func)
		queue_work(md_misc_wq, &mddev->event_work);
	md_new_event_inintr(mddev);
}
=======
	mddev->pers->error_handler(mddev, rdev);

	if (mddev->pers->level == 0)
		return;

	if (mddev->degraded && !test_bit(MD_BROKEN, &mddev->flags))
		set_bit(MD_RECOVERY_RECOVER, &mddev->recovery);
	sysfs_notify_dirent_safe(rdev->sysfs_state);
	set_bit(MD_RECOVERY_INTR, &mddev->recovery);
	if (!test_bit(MD_BROKEN, &mddev->flags)) {
		set_bit(MD_RECOVERY_NEEDED, &mddev->recovery);
		md_wakeup_thread(mddev->thread);
	}
	if (mddev->event_work.func)
		queue_work(md_misc_wq, &mddev->event_work);
	md_new_event();
}
EXPORT_SYMBOL(md_error);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

/* seq_file implementation /proc/mdstat */

static void status_unused(struct seq_file *seq)
{
	int i = 0;
	struct md_rdev *rdev;

	seq_printf(seq, "unused devices: ");

	list_for_each_entry(rdev, &pending_raid_disks, same_set) {
<<<<<<< HEAD
		char b[BDEVNAME_SIZE];
		i++;
		seq_printf(seq, "%s ",
			      bdevname(rdev->bdev,b));
=======
		i++;
		seq_printf(seq, "%pg ", rdev->bdev);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	}
	if (!i)
		seq_printf(seq, "<none>");

	seq_printf(seq, "\n");
}

<<<<<<< HEAD

static void status_resync(struct seq_file *seq, struct mddev * mddev)
{
	sector_t max_sectors, resync, res;
	unsigned long dt, db;
	sector_t rt;
	int scale;
	unsigned int per_milli;

	resync = mddev->curr_resync - atomic_read(&mddev->recovery_active);

	if (test_bit(MD_RECOVERY_SYNC, &mddev->recovery))
=======
static void status_personalities(struct seq_file *seq)
{
	struct md_personality *pers;

	seq_puts(seq, "Personalities : ");
	spin_lock(&pers_lock);
	list_for_each_entry(pers, &pers_list, list)
		seq_printf(seq, "[%s] ", pers->name);

	spin_unlock(&pers_lock);
	seq_puts(seq, "\n");
}

static int status_resync(struct seq_file *seq, struct mddev *mddev)
{
	sector_t max_sectors, resync, res;
	unsigned long dt, db = 0;
	sector_t rt, curr_mark_cnt, resync_mark_cnt;
	int scale, recovery_active;
	unsigned int per_milli;

	if (test_bit(MD_RECOVERY_SYNC, &mddev->recovery) ||
	    test_bit(MD_RECOVERY_RESHAPE, &mddev->recovery))
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		max_sectors = mddev->resync_max_sectors;
	else
		max_sectors = mddev->dev_sectors;

<<<<<<< HEAD
	/*
	 * Should not happen.
	 */
	if (!max_sectors) {
		MD_BUG();
		return;
	}
=======
	resync = mddev->curr_resync;
	if (resync < MD_RESYNC_ACTIVE) {
		if (test_bit(MD_RECOVERY_DONE, &mddev->recovery))
			/* Still cleaning up */
			resync = max_sectors;
	} else if (resync > max_sectors) {
		resync = max_sectors;
	} else {
		res = atomic_read(&mddev->recovery_active);
		/*
		 * Resync has started, but the subtraction has overflowed or
		 * yielded one of the special values. Force it to active to
		 * ensure the status reports an active resync.
		 */
		if (resync < res || resync - res < MD_RESYNC_ACTIVE)
			resync = MD_RESYNC_ACTIVE;
		else
			resync -= res;
	}

	if (resync == MD_RESYNC_NONE) {
		if (test_bit(MD_RESYNCING_REMOTE, &mddev->recovery)) {
			struct md_rdev *rdev;

			rdev_for_each(rdev, mddev)
				if (rdev->raid_disk >= 0 &&
				    !test_bit(Faulty, &rdev->flags) &&
				    rdev->recovery_offset != MaxSector &&
				    rdev->recovery_offset) {
					seq_printf(seq, "\trecover=REMOTE");
					return 1;
				}
			if (mddev->reshape_position != MaxSector)
				seq_printf(seq, "\treshape=REMOTE");
			else
				seq_printf(seq, "\tresync=REMOTE");
			return 1;
		}
		if (mddev->recovery_cp < MaxSector) {
			seq_printf(seq, "\tresync=PENDING");
			return 1;
		}
		return 0;
	}
	if (resync < MD_RESYNC_ACTIVE) {
		seq_printf(seq, "\tresync=DELAYED");
		return 1;
	}

	WARN_ON(max_sectors == 0);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	/* Pick 'scale' such that (resync>>scale)*1000 will fit
	 * in a sector_t, and (max_sectors>>scale) will fit in a
	 * u32, as those are the requirements for sector_div.
	 * Thus 'scale' must be at least 10
	 */
	scale = 10;
	if (sizeof(sector_t) > sizeof(unsigned long)) {
		while ( max_sectors/2 > (1ULL<<(scale+32)))
			scale++;
	}
	res = (resync>>scale)*1000;
	sector_div(res, (u32)((max_sectors>>scale)+1));

	per_milli = res;
	{
		int i, x = per_milli/50, y = 20-x;
		seq_printf(seq, "[");
		for (i = 0; i < x; i++)
			seq_printf(seq, "=");
		seq_printf(seq, ">");
		for (i = 0; i < y; i++)
			seq_printf(seq, ".");
		seq_printf(seq, "] ");
	}
	seq_printf(seq, " %s =%3u.%u%% (%llu/%llu)",
		   (test_bit(MD_RECOVERY_RESHAPE, &mddev->recovery)?
		    "reshape" :
		    (test_bit(MD_RECOVERY_CHECK, &mddev->recovery)?
		     "check" :
		     (test_bit(MD_RECOVERY_SYNC, &mddev->recovery) ?
		      "resync" : "recovery"))),
		   per_milli/10, per_milli % 10,
		   (unsigned long long) resync/2,
		   (unsigned long long) max_sectors/2);

	/*
	 * dt: time from mark until now
	 * db: blocks written from mark until now
	 * rt: remaining time
	 *
<<<<<<< HEAD
	 * rt is a sector_t, so could be 32bit or 64bit.
	 * So we divide before multiply in case it is 32bit and close
	 * to the limit.
	 * We scale the divisor (db) by 32 to avoid losing precision
	 * near the end of resync when the number of remaining sectors
	 * is close to 'db'.
	 * We then divide rt by 32 after multiplying by db to compensate.
	 * The '+1' avoids division by zero if db is very small.
	 */
	dt = ((jiffies - mddev->resync_mark) / HZ);
	if (!dt) dt++;
	db = (mddev->curr_mark_cnt - atomic_read(&mddev->recovery_active))
		- mddev->resync_mark_cnt;

	rt = max_sectors - resync;    /* number of remaining sectors */
	sector_div(rt, db/32+1);
=======
	 * rt is a sector_t, which is always 64bit now. We are keeping
	 * the original algorithm, but it is not really necessary.
	 *
	 * Original algorithm:
	 *   So we divide before multiply in case it is 32bit and close
	 *   to the limit.
	 *   We scale the divisor (db) by 32 to avoid losing precision
	 *   near the end of resync when the number of remaining sectors
	 *   is close to 'db'.
	 *   We then divide rt by 32 after multiplying by db to compensate.
	 *   The '+1' avoids division by zero if db is very small.
	 */
	dt = ((jiffies - mddev->resync_mark) / HZ);
	if (!dt) dt++;

	curr_mark_cnt = mddev->curr_mark_cnt;
	recovery_active = atomic_read(&mddev->recovery_active);
	resync_mark_cnt = mddev->resync_mark_cnt;

	if (curr_mark_cnt >= (recovery_active + resync_mark_cnt))
		db = curr_mark_cnt - (recovery_active + resync_mark_cnt);

	rt = max_sectors - resync;    /* number of remaining sectors */
	rt = div64_u64(rt, db/32+1);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	rt *= dt;
	rt >>= 5;

	seq_printf(seq, " finish=%lu.%lumin", (unsigned long)rt / 60,
		   ((unsigned long)rt % 60)/6);

	seq_printf(seq, " speed=%ldK/sec", db/2/dt);
<<<<<<< HEAD
}

static void *md_seq_start(struct seq_file *seq, loff_t *pos)
{
	struct list_head *tmp;
	loff_t l = *pos;
	struct mddev *mddev;

	if (l >= 0x10000)
		return NULL;
	if (!l--)
		/* header */
		return (void*)1;

	spin_lock(&all_mddevs_lock);
	list_for_each(tmp,&all_mddevs)
		if (!l--) {
			mddev = list_entry(tmp, struct mddev, all_mddevs);
			mddev_get(mddev);
			spin_unlock(&all_mddevs_lock);
			return mddev;
		}
	spin_unlock(&all_mddevs_lock);
	if (!l--)
		return (void*)2;/* tail */
	return NULL;
=======
	return 1;
}

static void *md_seq_start(struct seq_file *seq, loff_t *pos)
	__acquires(&all_mddevs_lock)
{
	seq->poll_event = atomic_read(&md_event_count);
	spin_lock(&all_mddevs_lock);

	return seq_list_start_head(&all_mddevs, *pos);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

static void *md_seq_next(struct seq_file *seq, void *v, loff_t *pos)
{
<<<<<<< HEAD
	struct list_head *tmp;
	struct mddev *next_mddev, *mddev = v;
	
	++*pos;
	if (v == (void*)2)
		return NULL;

	spin_lock(&all_mddevs_lock);
	if (v == (void*)1)
		tmp = all_mddevs.next;
	else
		tmp = mddev->all_mddevs.next;
	if (tmp != &all_mddevs)
		next_mddev = mddev_get(list_entry(tmp,struct mddev,all_mddevs));
	else {
		next_mddev = (void*)2;
		*pos = 0x10000;
	}		
	spin_unlock(&all_mddevs_lock);

	if (v != (void*)1)
		mddev_put(mddev);
	return next_mddev;

}

static void md_seq_stop(struct seq_file *seq, void *v)
{
	struct mddev *mddev = v;

	if (mddev && v != (void*)1 && v != (void*)2)
		mddev_put(mddev);
=======
	return seq_list_next(v, &all_mddevs, pos);
}

static void md_seq_stop(struct seq_file *seq, void *v)
	__releases(&all_mddevs_lock)
{
	spin_unlock(&all_mddevs_lock);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

static int md_seq_show(struct seq_file *seq, void *v)
{
<<<<<<< HEAD
	struct mddev *mddev = v;
	sector_t sectors;
	struct md_rdev *rdev;

	if (v == (void*)1) {
		struct md_personality *pers;
		seq_printf(seq, "Personalities : ");
		spin_lock(&pers_lock);
		list_for_each_entry(pers, &pers_list, list)
			seq_printf(seq, "[%s] ", pers->name);

		spin_unlock(&pers_lock);
		seq_printf(seq, "\n");
		seq->poll_event = atomic_read(&md_event_count);
		return 0;
	}
	if (v == (void*)2) {
		status_unused(seq);
		return 0;
	}

	if (mddev_lock(mddev) < 0)
		return -EINTR;

=======
	struct mddev *mddev;
	sector_t sectors;
	struct md_rdev *rdev;

	if (v == &all_mddevs) {
		status_personalities(seq);
		if (list_empty(&all_mddevs))
			status_unused(seq);
		return 0;
	}

	mddev = list_entry(v, struct mddev, all_mddevs);
	if (!mddev_get(mddev))
		return 0;

	spin_unlock(&all_mddevs_lock);
	spin_lock(&mddev->lock);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	if (mddev->pers || mddev->raid_disks || !list_empty(&mddev->disks)) {
		seq_printf(seq, "%s : %sactive", mdname(mddev),
						mddev->pers ? "" : "in");
		if (mddev->pers) {
<<<<<<< HEAD
			if (mddev->ro==1)
				seq_printf(seq, " (read-only)");
			if (mddev->ro==2)
=======
			if (mddev->ro == MD_RDONLY)
				seq_printf(seq, " (read-only)");
			if (mddev->ro == MD_AUTO_READ)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
				seq_printf(seq, " (auto-read-only)");
			seq_printf(seq, " %s", mddev->pers->name);
		}

		sectors = 0;
<<<<<<< HEAD
		rdev_for_each(rdev, mddev) {
			char b[BDEVNAME_SIZE];
			seq_printf(seq, " %s[%d]",
				bdevname(rdev->bdev,b), rdev->desc_nr);
			if (test_bit(WriteMostly, &rdev->flags))
				seq_printf(seq, "(W)");
=======
		rcu_read_lock();
		rdev_for_each_rcu(rdev, mddev) {
			seq_printf(seq, " %pg[%d]", rdev->bdev, rdev->desc_nr);

			if (test_bit(WriteMostly, &rdev->flags))
				seq_printf(seq, "(W)");
			if (test_bit(Journal, &rdev->flags))
				seq_printf(seq, "(J)");
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
			if (test_bit(Faulty, &rdev->flags)) {
				seq_printf(seq, "(F)");
				continue;
			}
			if (rdev->raid_disk < 0)
				seq_printf(seq, "(S)"); /* spare */
			if (test_bit(Replacement, &rdev->flags))
				seq_printf(seq, "(R)");
			sectors += rdev->sectors;
		}
<<<<<<< HEAD
=======
		rcu_read_unlock();
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

		if (!list_empty(&mddev->disks)) {
			if (mddev->pers)
				seq_printf(seq, "\n      %llu blocks",
					   (unsigned long long)
					   mddev->array_sectors / 2);
			else
				seq_printf(seq, "\n      %llu blocks",
					   (unsigned long long)sectors / 2);
		}
		if (mddev->persistent) {
			if (mddev->major_version != 0 ||
			    mddev->minor_version != 90) {
				seq_printf(seq," super %d.%d",
					   mddev->major_version,
					   mddev->minor_version);
			}
		} else if (mddev->external)
			seq_printf(seq, " super external:%s",
				   mddev->metadata_type);
		else
			seq_printf(seq, " super non-persistent");

		if (mddev->pers) {
			mddev->pers->status(seq, mddev);
<<<<<<< HEAD
	 		seq_printf(seq, "\n      ");
			if (mddev->pers->sync_request) {
				if (mddev->curr_resync > 2) {
					status_resync(seq, mddev);
					seq_printf(seq, "\n      ");
				} else if (mddev->curr_resync == 1 || mddev->curr_resync == 2)
					seq_printf(seq, "\tresync=DELAYED\n      ");
				else if (mddev->recovery_cp < MaxSector)
					seq_printf(seq, "\tresync=PENDING\n      ");
=======
			seq_printf(seq, "\n      ");
			if (mddev->pers->sync_request) {
				if (status_resync(seq, mddev))
					seq_printf(seq, "\n      ");
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
			}
		} else
			seq_printf(seq, "\n       ");

<<<<<<< HEAD
		bitmap_status(seq, mddev->bitmap);

		seq_printf(seq, "\n");
	}
	mddev_unlock(mddev);
	
=======
		md_bitmap_status(seq, mddev->bitmap);

		seq_printf(seq, "\n");
	}
	spin_unlock(&mddev->lock);
	spin_lock(&all_mddevs_lock);

	if (mddev == list_last_entry(&all_mddevs, struct mddev, all_mddevs))
		status_unused(seq);

	if (atomic_dec_and_test(&mddev->active))
		__mddev_put(mddev);

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	return 0;
}

static const struct seq_operations md_seq_ops = {
	.start  = md_seq_start,
	.next   = md_seq_next,
	.stop   = md_seq_stop,
	.show   = md_seq_show,
};

static int md_seq_open(struct inode *inode, struct file *file)
{
	struct seq_file *seq;
	int error;

	error = seq_open(file, &md_seq_ops);
	if (error)
		return error;

	seq = file->private_data;
	seq->poll_event = atomic_read(&md_event_count);
	return error;
}

<<<<<<< HEAD
static unsigned int mdstat_poll(struct file *filp, poll_table *wait)
{
	struct seq_file *seq = filp->private_data;
	int mask;

	poll_wait(filp, &md_event_waiters, wait);

	/* always allow read */
	mask = POLLIN | POLLRDNORM;

	if (seq->poll_event != atomic_read(&md_event_count))
		mask |= POLLERR | POLLPRI;
	return mask;
}

static const struct file_operations md_seq_fops = {
	.owner		= THIS_MODULE,
	.open           = md_seq_open,
	.read           = seq_read,
	.llseek         = seq_lseek,
	.release	= seq_release_private,
	.poll		= mdstat_poll,
=======
static int md_unloading;
static __poll_t mdstat_poll(struct file *filp, poll_table *wait)
{
	struct seq_file *seq = filp->private_data;
	__poll_t mask;

	if (md_unloading)
		return EPOLLIN|EPOLLRDNORM|EPOLLERR|EPOLLPRI;
	poll_wait(filp, &md_event_waiters, wait);

	/* always allow read */
	mask = EPOLLIN | EPOLLRDNORM;

	if (seq->poll_event != atomic_read(&md_event_count))
		mask |= EPOLLERR | EPOLLPRI;
	return mask;
}

static const struct proc_ops mdstat_proc_ops = {
	.proc_open	= md_seq_open,
	.proc_read	= seq_read,
	.proc_lseek	= seq_lseek,
	.proc_release	= seq_release,
	.proc_poll	= mdstat_poll,
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
};

int register_md_personality(struct md_personality *p)
{
<<<<<<< HEAD
	spin_lock(&pers_lock);
	list_add_tail(&p->list, &pers_list);
	printk(KERN_INFO "md: %s personality registered for level %d\n", p->name, p->level);
	spin_unlock(&pers_lock);
	return 0;
}

int unregister_md_personality(struct md_personality *p)
{
	printk(KERN_INFO "md: %s personality unregistered\n", p->name);
=======
	pr_debug("md: %s personality registered for level %d\n",
		 p->name, p->level);
	spin_lock(&pers_lock);
	list_add_tail(&p->list, &pers_list);
	spin_unlock(&pers_lock);
	return 0;
}
EXPORT_SYMBOL(register_md_personality);

int unregister_md_personality(struct md_personality *p)
{
	pr_debug("md: %s personality unregistered\n", p->name);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	spin_lock(&pers_lock);
	list_del_init(&p->list);
	spin_unlock(&pers_lock);
	return 0;
}
<<<<<<< HEAD

static int is_mddev_idle(struct mddev *mddev, int init)
{
	struct md_rdev * rdev;
=======
EXPORT_SYMBOL(unregister_md_personality);

int register_md_cluster_operations(struct md_cluster_operations *ops,
				   struct module *module)
{
	int ret = 0;
	spin_lock(&pers_lock);
	if (md_cluster_ops != NULL)
		ret = -EALREADY;
	else {
		md_cluster_ops = ops;
		md_cluster_mod = module;
	}
	spin_unlock(&pers_lock);
	return ret;
}
EXPORT_SYMBOL(register_md_cluster_operations);

int unregister_md_cluster_operations(void)
{
	spin_lock(&pers_lock);
	md_cluster_ops = NULL;
	spin_unlock(&pers_lock);
	return 0;
}
EXPORT_SYMBOL(unregister_md_cluster_operations);

int md_setup_cluster(struct mddev *mddev, int nodes)
{
	int ret;
	if (!md_cluster_ops)
		request_module("md-cluster");
	spin_lock(&pers_lock);
	/* ensure module won't be unloaded */
	if (!md_cluster_ops || !try_module_get(md_cluster_mod)) {
		pr_warn("can't find md-cluster module or get its reference.\n");
		spin_unlock(&pers_lock);
		return -ENOENT;
	}
	spin_unlock(&pers_lock);

	ret = md_cluster_ops->join(mddev, nodes);
	if (!ret)
		mddev->safemode_delay = 0;
	return ret;
}

void md_cluster_stop(struct mddev *mddev)
{
	if (!md_cluster_ops)
		return;
	md_cluster_ops->leave(mddev);
	module_put(md_cluster_mod);
}

static int is_mddev_idle(struct mddev *mddev, int init)
{
	struct md_rdev *rdev;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	int idle;
	int curr_events;

	idle = 1;
	rcu_read_lock();
	rdev_for_each_rcu(rdev, mddev) {
<<<<<<< HEAD
		struct gendisk *disk = rdev->bdev->bd_contains->bd_disk;
		curr_events = (int)part_stat_read(&disk->part0, sectors[0]) +
			      (int)part_stat_read(&disk->part0, sectors[1]) -
=======
		struct gendisk *disk = rdev->bdev->bd_disk;
		curr_events = (int)part_stat_read_accum(disk->part0, sectors) -
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
			      atomic_read(&disk->sync_io);
		/* sync IO will cause sync_io to increase before the disk_stats
		 * as sync_io is counted when a request starts, and
		 * disk_stats is counted when it completes.
		 * So resync activity will cause curr_events to be smaller than
		 * when there was no such activity.
		 * non-sync IO will cause disk_stat to increase without
		 * increasing sync_io so curr_events will (eventually)
		 * be larger than it was before.  Once it becomes
		 * substantially larger, the test below will cause
		 * the array to appear non-idle, and resync will slow
		 * down.
		 * If there is a lot of outstanding resync activity when
		 * we set last_event to curr_events, then all that activity
		 * completing might cause the array to appear non-idle
		 * and resync will be slowed down even though there might
		 * not have been non-resync activity.  This will only
		 * happen once though.  'last_events' will soon reflect
		 * the state where there is little or no outstanding
		 * resync requests, and further resync activity will
		 * always make curr_events less than last_events.
		 *
		 */
		if (init || curr_events - rdev->last_events > 64) {
			rdev->last_events = curr_events;
			idle = 0;
		}
	}
	rcu_read_unlock();
	return idle;
}

void md_done_sync(struct mddev *mddev, int blocks, int ok)
{
	/* another "blocks" (512byte) blocks have been synced */
	atomic_sub(blocks, &mddev->recovery_active);
	wake_up(&mddev->recovery_wait);
	if (!ok) {
		set_bit(MD_RECOVERY_INTR, &mddev->recovery);
<<<<<<< HEAD
=======
		set_bit(MD_RECOVERY_ERROR, &mddev->recovery);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		md_wakeup_thread(mddev->thread);
		// stop recovery, signal do_sync ....
	}
}
<<<<<<< HEAD

=======
EXPORT_SYMBOL(md_done_sync);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

/* md_write_start(mddev, bi)
 * If we need to update some array metadata (e.g. 'active' flag
 * in superblock) before writing, schedule a superblock update
 * and wait for it to complete.
<<<<<<< HEAD
 */
void md_write_start(struct mddev *mddev, struct bio *bi)
{
	int did_change = 0;
	if (bio_data_dir(bi) != WRITE)
		return;

	BUG_ON(mddev->ro == 1);
	if (mddev->ro == 2) {
		/* need to switch to read/write */
		mddev->ro = 0;
=======
 * A return value of 'false' means that the write wasn't recorded
 * and cannot proceed as the array is being suspend.
 */
bool md_write_start(struct mddev *mddev, struct bio *bi)
{
	int did_change = 0;

	if (bio_data_dir(bi) != WRITE)
		return true;

	BUG_ON(mddev->ro == MD_RDONLY);
	if (mddev->ro == MD_AUTO_READ) {
		/* need to switch to read/write */
		flush_work(&mddev->sync_work);
		mddev->ro = MD_RDWR;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		set_bit(MD_RECOVERY_NEEDED, &mddev->recovery);
		md_wakeup_thread(mddev->thread);
		md_wakeup_thread(mddev->sync_thread);
		did_change = 1;
	}
<<<<<<< HEAD
	atomic_inc(&mddev->writes_pending);
	if (mddev->safemode == 1)
		mddev->safemode = 0;
	if (mddev->in_sync) {
		spin_lock_irq(&mddev->write_lock);
		if (mddev->in_sync) {
			mddev->in_sync = 0;
			set_bit(MD_CHANGE_CLEAN, &mddev->flags);
			set_bit(MD_CHANGE_PENDING, &mddev->flags);
			md_wakeup_thread(mddev->thread);
			did_change = 1;
		}
		spin_unlock_irq(&mddev->write_lock);
	}
	if (did_change)
		sysfs_notify_dirent_safe(mddev->sysfs_state);
	wait_event(mddev->sb_wait,
		   !test_bit(MD_CHANGE_PENDING, &mddev->flags));
}

void md_write_end(struct mddev *mddev)
{
	if (atomic_dec_and_test(&mddev->writes_pending)) {
		if (mddev->safemode == 2)
			md_wakeup_thread(mddev->thread);
		else if (mddev->safemode_delay)
			mod_timer(&mddev->safemode_timer, jiffies + mddev->safemode_delay);
	}
}

=======
	rcu_read_lock();
	percpu_ref_get(&mddev->writes_pending);
	smp_mb(); /* Match smp_mb in set_in_sync() */
	if (mddev->safemode == 1)
		mddev->safemode = 0;
	/* sync_checkers is always 0 when writes_pending is in per-cpu mode */
	if (mddev->in_sync || mddev->sync_checkers) {
		spin_lock(&mddev->lock);
		if (mddev->in_sync) {
			mddev->in_sync = 0;
			set_bit(MD_SB_CHANGE_CLEAN, &mddev->sb_flags);
			set_bit(MD_SB_CHANGE_PENDING, &mddev->sb_flags);
			md_wakeup_thread(mddev->thread);
			did_change = 1;
		}
		spin_unlock(&mddev->lock);
	}
	rcu_read_unlock();
	if (did_change)
		sysfs_notify_dirent_safe(mddev->sysfs_state);
	if (!mddev->has_superblocks)
		return true;
	wait_event(mddev->sb_wait,
		   !test_bit(MD_SB_CHANGE_PENDING, &mddev->sb_flags) ||
		   is_md_suspended(mddev));
	if (test_bit(MD_SB_CHANGE_PENDING, &mddev->sb_flags)) {
		percpu_ref_put(&mddev->writes_pending);
		return false;
	}
	return true;
}
EXPORT_SYMBOL(md_write_start);

/* md_write_inc can only be called when md_write_start() has
 * already been called at least once of the current request.
 * It increments the counter and is useful when a single request
 * is split into several parts.  Each part causes an increment and
 * so needs a matching md_write_end().
 * Unlike md_write_start(), it is safe to call md_write_inc() inside
 * a spinlocked region.
 */
void md_write_inc(struct mddev *mddev, struct bio *bi)
{
	if (bio_data_dir(bi) != WRITE)
		return;
	WARN_ON_ONCE(mddev->in_sync || !md_is_rdwr(mddev));
	percpu_ref_get(&mddev->writes_pending);
}
EXPORT_SYMBOL(md_write_inc);

void md_write_end(struct mddev *mddev)
{
	percpu_ref_put(&mddev->writes_pending);

	if (mddev->safemode == 2)
		md_wakeup_thread(mddev->thread);
	else if (mddev->safemode_delay)
		/* The roundup() ensures this only performs locking once
		 * every ->safemode_delay jiffies
		 */
		mod_timer(&mddev->safemode_timer,
			  roundup(jiffies, mddev->safemode_delay) +
			  mddev->safemode_delay);
}

EXPORT_SYMBOL(md_write_end);

/* This is used by raid0 and raid10 */
void md_submit_discard_bio(struct mddev *mddev, struct md_rdev *rdev,
			struct bio *bio, sector_t start, sector_t size)
{
	struct bio *discard_bio = NULL;

	if (__blkdev_issue_discard(rdev->bdev, start, size, GFP_NOIO,
			&discard_bio) || !discard_bio)
		return;

	bio_chain(discard_bio, bio);
	bio_clone_blkg_association(discard_bio, bio);
	mddev_trace_remap(mddev, discard_bio, bio->bi_iter.bi_sector);
	submit_bio_noacct(discard_bio);
}
EXPORT_SYMBOL_GPL(md_submit_discard_bio);

static void md_end_clone_io(struct bio *bio)
{
	struct md_io_clone *md_io_clone = bio->bi_private;
	struct bio *orig_bio = md_io_clone->orig_bio;
	struct mddev *mddev = md_io_clone->mddev;

	if (bio->bi_status && !orig_bio->bi_status)
		orig_bio->bi_status = bio->bi_status;

	if (md_io_clone->start_time)
		bio_end_io_acct(orig_bio, md_io_clone->start_time);

	bio_put(bio);
	bio_endio(orig_bio);
	percpu_ref_put(&mddev->active_io);
}

static void md_clone_bio(struct mddev *mddev, struct bio **bio)
{
	struct block_device *bdev = (*bio)->bi_bdev;
	struct md_io_clone *md_io_clone;
	struct bio *clone =
		bio_alloc_clone(bdev, *bio, GFP_NOIO, &mddev->io_clone_set);

	md_io_clone = container_of(clone, struct md_io_clone, bio_clone);
	md_io_clone->orig_bio = *bio;
	md_io_clone->mddev = mddev;
	if (blk_queue_io_stat(bdev->bd_disk->queue))
		md_io_clone->start_time = bio_start_io_acct(*bio);

	clone->bi_end_io = md_end_clone_io;
	clone->bi_private = md_io_clone;
	*bio = clone;
}

void md_account_bio(struct mddev *mddev, struct bio **bio)
{
	percpu_ref_get(&mddev->active_io);
	md_clone_bio(mddev, bio);
}
EXPORT_SYMBOL_GPL(md_account_bio);

void md_free_cloned_bio(struct bio *bio)
{
	struct md_io_clone *md_io_clone = bio->bi_private;
	struct bio *orig_bio = md_io_clone->orig_bio;
	struct mddev *mddev = md_io_clone->mddev;

	if (bio->bi_status && !orig_bio->bi_status)
		orig_bio->bi_status = bio->bi_status;

	if (md_io_clone->start_time)
		bio_end_io_acct(orig_bio, md_io_clone->start_time);

	bio_put(bio);
	percpu_ref_put(&mddev->active_io);
}
EXPORT_SYMBOL_GPL(md_free_cloned_bio);

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
/* md_allow_write(mddev)
 * Calling this ensures that the array is marked 'active' so that writes
 * may proceed without blocking.  It is important to call this before
 * attempting a GFP_KERNEL allocation while holding the mddev lock.
 * Must be called with mddev_lock held.
<<<<<<< HEAD
 *
 * In the ->external case MD_CHANGE_CLEAN can not be cleared until mddev->lock
 * is dropped, so return -EAGAIN after notifying userspace.
 */
int md_allow_write(struct mddev *mddev)
{
	if (!mddev->pers)
		return 0;
	if (mddev->ro)
		return 0;
	if (!mddev->pers->sync_request)
		return 0;

	spin_lock_irq(&mddev->write_lock);
	if (mddev->in_sync) {
		mddev->in_sync = 0;
		set_bit(MD_CHANGE_CLEAN, &mddev->flags);
		set_bit(MD_CHANGE_PENDING, &mddev->flags);
		if (mddev->safemode_delay &&
		    mddev->safemode == 0)
			mddev->safemode = 1;
		spin_unlock_irq(&mddev->write_lock);
		md_update_sb(mddev, 0);
		sysfs_notify_dirent_safe(mddev->sysfs_state);
	} else
		spin_unlock_irq(&mddev->write_lock);

	if (test_bit(MD_CHANGE_PENDING, &mddev->flags))
		return -EAGAIN;
	else
		return 0;
=======
 */
void md_allow_write(struct mddev *mddev)
{
	if (!mddev->pers)
		return;
	if (!md_is_rdwr(mddev))
		return;
	if (!mddev->pers->sync_request)
		return;

	spin_lock(&mddev->lock);
	if (mddev->in_sync) {
		mddev->in_sync = 0;
		set_bit(MD_SB_CHANGE_CLEAN, &mddev->sb_flags);
		set_bit(MD_SB_CHANGE_PENDING, &mddev->sb_flags);
		if (mddev->safemode_delay &&
		    mddev->safemode == 0)
			mddev->safemode = 1;
		spin_unlock(&mddev->lock);
		md_update_sb(mddev, 0);
		sysfs_notify_dirent_safe(mddev->sysfs_state);
		/* wait for the dirty state to be recorded in the metadata */
		wait_event(mddev->sb_wait,
			   !test_bit(MD_SB_CHANGE_PENDING, &mddev->sb_flags));
	} else
		spin_unlock(&mddev->lock);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}
EXPORT_SYMBOL_GPL(md_allow_write);

#define SYNC_MARKS	10
#define	SYNC_MARK_STEP	(3*HZ)
<<<<<<< HEAD
void md_do_sync(struct mddev *mddev)
{
	struct mddev *mddev2;
	unsigned int currspeed = 0,
		 window;
	sector_t max_sectors,j, io_sectors;
	unsigned long mark[SYNC_MARKS];
	sector_t mark_cnt[SYNC_MARKS];
	int last_mark,m;
	struct list_head *tmp;
	sector_t last_check;
	int skipped = 0;
	struct md_rdev *rdev;
	char *desc;
=======
#define UPDATE_FREQUENCY (5*60*HZ)
void md_do_sync(struct md_thread *thread)
{
	struct mddev *mddev = thread->mddev;
	struct mddev *mddev2;
	unsigned int currspeed = 0, window;
	sector_t max_sectors,j, io_sectors, recovery_done;
	unsigned long mark[SYNC_MARKS];
	unsigned long update_time;
	sector_t mark_cnt[SYNC_MARKS];
	int last_mark,m;
	sector_t last_check;
	int skipped = 0;
	struct md_rdev *rdev;
	char *desc, *action = NULL;
	struct blk_plug plug;
	int ret;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	/* just incase thread restarts... */
	if (test_bit(MD_RECOVERY_DONE, &mddev->recovery))
		return;
<<<<<<< HEAD
	if (mddev->ro) {/* never try to sync a read-only array */
		set_bit(MD_RECOVERY_INTR, &mddev->recovery);
		return;
	}

	if (test_bit(MD_RECOVERY_SYNC, &mddev->recovery)) {
		if (test_bit(MD_RECOVERY_CHECK, &mddev->recovery))
			desc = "data-check";
		else if (test_bit(MD_RECOVERY_REQUESTED, &mddev->recovery))
			desc = "requested-resync";
		else
=======

	if (test_bit(MD_RECOVERY_INTR, &mddev->recovery))
		goto skip;

	if (test_bit(MD_RECOVERY_WAIT, &mddev->recovery) ||
	    !md_is_rdwr(mddev)) {/* never try to sync a read-only array */
		set_bit(MD_RECOVERY_INTR, &mddev->recovery);
		goto skip;
	}

	if (mddev_is_clustered(mddev)) {
		ret = md_cluster_ops->resync_start(mddev);
		if (ret)
			goto skip;

		set_bit(MD_CLUSTER_RESYNC_LOCKED, &mddev->flags);
		if (!(test_bit(MD_RECOVERY_SYNC, &mddev->recovery) ||
			test_bit(MD_RECOVERY_RESHAPE, &mddev->recovery) ||
			test_bit(MD_RECOVERY_RECOVER, &mddev->recovery))
		     && ((unsigned long long)mddev->curr_resync_completed
			 < (unsigned long long)mddev->resync_max_sectors))
			goto skip;
	}

	if (test_bit(MD_RECOVERY_SYNC, &mddev->recovery)) {
		if (test_bit(MD_RECOVERY_CHECK, &mddev->recovery)) {
			desc = "data-check";
			action = "check";
		} else if (test_bit(MD_RECOVERY_REQUESTED, &mddev->recovery)) {
			desc = "requested-resync";
			action = "repair";
		} else
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
			desc = "resync";
	} else if (test_bit(MD_RECOVERY_RESHAPE, &mddev->recovery))
		desc = "reshape";
	else
		desc = "recovery";

<<<<<<< HEAD
	/* we overload curr_resync somewhat here.
	 * 0 == not engaged in resync at all
	 * 2 == checking that there is no conflict with another sync
	 * 1 == like 2, but have yielded to allow conflicting resync to
	 *		commense
	 * other == active in resync - this many blocks
	 *
=======
	mddev->last_sync_action = action ?: desc;

	/*
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	 * Before starting a resync we must have set curr_resync to
	 * 2, and then checked that every "conflicting" array has curr_resync
	 * less than ours.  When we find one that is the same or higher
	 * we wait on resync_wait.  To avoid deadlock, we reduce curr_resync
	 * to 1 if we choose to yield (based arbitrarily on address of mddev structure).
	 * This will mean we have to start checking from the beginning again.
	 *
	 */

	do {
<<<<<<< HEAD
		mddev->curr_resync = 2;

	try_again:
		if (kthread_should_stop())
			set_bit(MD_RECOVERY_INTR, &mddev->recovery);

		if (test_bit(MD_RECOVERY_INTR, &mddev->recovery))
			goto skip;
		for_each_mddev(mddev2, tmp) {
=======
		int mddev2_minor = -1;
		mddev->curr_resync = MD_RESYNC_DELAYED;

	try_again:
		if (test_bit(MD_RECOVERY_INTR, &mddev->recovery))
			goto skip;
		spin_lock(&all_mddevs_lock);
		list_for_each_entry(mddev2, &all_mddevs, all_mddevs) {
			if (test_bit(MD_DELETED, &mddev2->flags))
				continue;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
			if (mddev2 == mddev)
				continue;
			if (!mddev->parallel_resync
			&&  mddev2->curr_resync
			&&  match_mddev_units(mddev, mddev2)) {
				DEFINE_WAIT(wq);
<<<<<<< HEAD
				if (mddev < mddev2 && mddev->curr_resync == 2) {
					/* arbitrarily yield */
					mddev->curr_resync = 1;
					wake_up(&resync_wait);
				}
				if (mddev > mddev2 && mddev->curr_resync == 1)
=======
				if (mddev < mddev2 &&
				    mddev->curr_resync == MD_RESYNC_DELAYED) {
					/* arbitrarily yield */
					mddev->curr_resync = MD_RESYNC_YIELDED;
					wake_up(&resync_wait);
				}
				if (mddev > mddev2 &&
				    mddev->curr_resync == MD_RESYNC_YIELDED)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
					/* no need to wait here, we can wait the next
					 * time 'round when curr_resync == 2
					 */
					continue;
				/* We need to wait 'interruptible' so as not to
				 * contribute to the load average, and not to
				 * be caught by 'softlockup'
				 */
				prepare_to_wait(&resync_wait, &wq, TASK_INTERRUPTIBLE);
<<<<<<< HEAD
				if (!kthread_should_stop() &&
				    mddev2->curr_resync >= mddev->curr_resync) {
					printk(KERN_INFO "md: delaying %s of %s"
					       " until %s has finished (they"
					       " share one or more physical units)\n",
					       desc, mdname(mddev), mdname(mddev2));
					mddev_put(mddev2);
=======
				if (!test_bit(MD_RECOVERY_INTR, &mddev->recovery) &&
				    mddev2->curr_resync >= mddev->curr_resync) {
					if (mddev2_minor != mddev2->md_minor) {
						mddev2_minor = mddev2->md_minor;
						pr_info("md: delaying %s of %s until %s has finished (they share one or more physical units)\n",
							desc, mdname(mddev),
							mdname(mddev2));
					}
					spin_unlock(&all_mddevs_lock);

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
					if (signal_pending(current))
						flush_signals(current);
					schedule();
					finish_wait(&resync_wait, &wq);
					goto try_again;
				}
				finish_wait(&resync_wait, &wq);
			}
		}
<<<<<<< HEAD
	} while (mddev->curr_resync < 2);
=======
		spin_unlock(&all_mddevs_lock);
	} while (mddev->curr_resync < MD_RESYNC_DELAYED);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	j = 0;
	if (test_bit(MD_RECOVERY_SYNC, &mddev->recovery)) {
		/* resync follows the size requested by the personality,
		 * which defaults to physical size, but can be virtual size
		 */
		max_sectors = mddev->resync_max_sectors;
<<<<<<< HEAD
		mddev->resync_mismatches = 0;
=======
		atomic64_set(&mddev->resync_mismatches, 0);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		/* we don't use the checkpoint if there's a bitmap */
		if (test_bit(MD_RECOVERY_REQUESTED, &mddev->recovery))
			j = mddev->resync_min;
		else if (!mddev->bitmap)
			j = mddev->recovery_cp;

<<<<<<< HEAD
	} else if (test_bit(MD_RECOVERY_RESHAPE, &mddev->recovery))
		max_sectors = mddev->dev_sectors;
	else {
=======
	} else if (test_bit(MD_RECOVERY_RESHAPE, &mddev->recovery)) {
		max_sectors = mddev->resync_max_sectors;
		/*
		 * If the original node aborts reshaping then we continue the
		 * reshaping, so set j again to avoid restart reshape from the
		 * first beginning
		 */
		if (mddev_is_clustered(mddev) &&
		    mddev->reshape_position != MaxSector)
			j = mddev->reshape_position;
	} else {
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		/* recovery follows the physical size of devices */
		max_sectors = mddev->dev_sectors;
		j = MaxSector;
		rcu_read_lock();
		rdev_for_each_rcu(rdev, mddev)
			if (rdev->raid_disk >= 0 &&
<<<<<<< HEAD
=======
			    !test_bit(Journal, &rdev->flags) &&
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
			    !test_bit(Faulty, &rdev->flags) &&
			    !test_bit(In_sync, &rdev->flags) &&
			    rdev->recovery_offset < j)
				j = rdev->recovery_offset;
		rcu_read_unlock();

		/* If there is a bitmap, we need to make sure all
		 * writes that started before we added a spare
		 * complete before we start doing a recovery.
		 * Otherwise the write might complete and (via
		 * bitmap_endwrite) set a bit in the bitmap after the
		 * recovery has checked that bit and skipped that
		 * region.
		 */
		if (mddev->bitmap) {
			mddev->pers->quiesce(mddev, 1);
			mddev->pers->quiesce(mddev, 0);
		}
	}

<<<<<<< HEAD
	printk(KERN_INFO "md: %s of RAID array %s\n", desc, mdname(mddev));
	printk(KERN_INFO "md: minimum _guaranteed_  speed:"
		" %d KB/sec/disk.\n", speed_min(mddev));
	printk(KERN_INFO "md: using maximum available idle IO bandwidth "
	       "(but not more than %d KB/sec) for %s.\n",
	       speed_max(mddev), desc);
=======
	pr_info("md: %s of RAID array %s\n", desc, mdname(mddev));
	pr_debug("md: minimum _guaranteed_  speed: %d KB/sec/disk.\n", speed_min(mddev));
	pr_debug("md: using maximum available idle IO bandwidth (but not more than %d KB/sec) for %s.\n",
		 speed_max(mddev), desc);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	is_mddev_idle(mddev, 1); /* this initializes IO event counters */

	io_sectors = 0;
	for (m = 0; m < SYNC_MARKS; m++) {
		mark[m] = jiffies;
		mark_cnt[m] = io_sectors;
	}
	last_mark = 0;
	mddev->resync_mark = mark[last_mark];
	mddev->resync_mark_cnt = mark_cnt[last_mark];

	/*
	 * Tune reconstruction:
	 */
<<<<<<< HEAD
	window = 32*(PAGE_SIZE/512);
	printk(KERN_INFO "md: using %dk window, over a total of %lluk.\n",
		window/2, (unsigned long long)max_sectors/2);
=======
	window = 32 * (PAGE_SIZE / 512);
	pr_debug("md: using %dk window, over a total of %lluk.\n",
		 window/2, (unsigned long long)max_sectors/2);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	atomic_set(&mddev->recovery_active, 0);
	last_check = 0;

<<<<<<< HEAD
	if (j>2) {
		printk(KERN_INFO 
		       "md: resuming %s of %s from checkpoint.\n",
		       desc, mdname(mddev));
		mddev->curr_resync = j;
	}
	mddev->curr_resync_completed = j;

=======
	if (j >= MD_RESYNC_ACTIVE) {
		pr_debug("md: resuming %s of %s from checkpoint.\n",
			 desc, mdname(mddev));
		mddev->curr_resync = j;
	} else
		mddev->curr_resync = MD_RESYNC_ACTIVE; /* no longer delayed */
	mddev->curr_resync_completed = j;
	sysfs_notify_dirent_safe(mddev->sysfs_completed);
	md_new_event();
	update_time = jiffies;

	blk_start_plug(&plug);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	while (j < max_sectors) {
		sector_t sectors;

		skipped = 0;

		if (!test_bit(MD_RECOVERY_RESHAPE, &mddev->recovery) &&
		    ((mddev->curr_resync > mddev->curr_resync_completed &&
		      (mddev->curr_resync - mddev->curr_resync_completed)
		      > (max_sectors >> 4)) ||
<<<<<<< HEAD
		     (j - mddev->curr_resync_completed)*2
		     >= mddev->resync_max - mddev->curr_resync_completed
=======
		     time_after_eq(jiffies, update_time + UPDATE_FREQUENCY) ||
		     (j - mddev->curr_resync_completed)*2
		     >= mddev->resync_max - mddev->curr_resync_completed ||
		     mddev->curr_resync_completed > mddev->resync_max
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
			    )) {
			/* time to update curr_resync_completed */
			wait_event(mddev->recovery_wait,
				   atomic_read(&mddev->recovery_active) == 0);
			mddev->curr_resync_completed = j;
<<<<<<< HEAD
			set_bit(MD_CHANGE_CLEAN, &mddev->flags);
			sysfs_notify(&mddev->kobj, NULL, "sync_completed");
		}

		while (j >= mddev->resync_max && !kthread_should_stop()) {
=======
			if (test_bit(MD_RECOVERY_SYNC, &mddev->recovery) &&
			    j > mddev->recovery_cp)
				mddev->recovery_cp = j;
			update_time = jiffies;
			set_bit(MD_SB_CHANGE_CLEAN, &mddev->sb_flags);
			sysfs_notify_dirent_safe(mddev->sysfs_completed);
		}

		while (j >= mddev->resync_max &&
		       !test_bit(MD_RECOVERY_INTR, &mddev->recovery)) {
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
			/* As this condition is controlled by user-space,
			 * we can block indefinitely, so use '_interruptible'
			 * to avoid triggering warnings.
			 */
			flush_signals(current); /* just in case */
			wait_event_interruptible(mddev->recovery_wait,
						 mddev->resync_max > j
<<<<<<< HEAD
						 || kthread_should_stop());
		}

		if (kthread_should_stop())
			goto interrupted;

		sectors = mddev->pers->sync_request(mddev, j, &skipped,
						  currspeed < speed_min(mddev));
		if (sectors == 0) {
			set_bit(MD_RECOVERY_INTR, &mddev->recovery);
			goto out;
=======
						 || test_bit(MD_RECOVERY_INTR,
							     &mddev->recovery));
		}

		if (test_bit(MD_RECOVERY_INTR, &mddev->recovery))
			break;

		sectors = mddev->pers->sync_request(mddev, j, &skipped);
		if (sectors == 0) {
			set_bit(MD_RECOVERY_INTR, &mddev->recovery);
			break;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		}

		if (!skipped) { /* actual IO requested */
			io_sectors += sectors;
			atomic_add(sectors, &mddev->recovery_active);
		}

		if (test_bit(MD_RECOVERY_INTR, &mddev->recovery))
			break;

		j += sectors;
<<<<<<< HEAD
		if (j>1) mddev->curr_resync = j;
=======
		if (j > max_sectors)
			/* when skipping, extra large numbers can be returned. */
			j = max_sectors;
		if (j >= MD_RESYNC_ACTIVE)
			mddev->curr_resync = j;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		mddev->curr_mark_cnt = io_sectors;
		if (last_check == 0)
			/* this is the earliest that rebuild will be
			 * visible in /proc/mdstat
			 */
<<<<<<< HEAD
			md_new_event(mddev);
=======
			md_new_event();
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

		if (last_check + window > io_sectors || j == max_sectors)
			continue;

		last_check = io_sectors;
	repeat:
		if (time_after_eq(jiffies, mark[last_mark] + SYNC_MARK_STEP )) {
			/* step marks */
			int next = (last_mark+1) % SYNC_MARKS;

			mddev->resync_mark = mark[next];
			mddev->resync_mark_cnt = mark_cnt[next];
			mark[next] = jiffies;
			mark_cnt[next] = io_sectors - atomic_read(&mddev->recovery_active);
			last_mark = next;
		}

<<<<<<< HEAD

		if (kthread_should_stop())
			goto interrupted;

=======
		if (test_bit(MD_RECOVERY_INTR, &mddev->recovery))
			break;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

		/*
		 * this loop exits only if either when we are slower than
		 * the 'hard' speed limit, or the system was IO-idle for
		 * a jiffy.
		 * the system might be non-idle CPU-wise, but we only care
		 * about not overloading the IO subsystem. (things like an
		 * e2fsck being done on the RAID array should execute fast)
		 */
		cond_resched();

<<<<<<< HEAD
		currspeed = ((unsigned long)(io_sectors-mddev->resync_mark_cnt))/2
			/((jiffies-mddev->resync_mark)/HZ +1) +1;

		if (currspeed > speed_min(mddev)) {
			if ((currspeed > speed_max(mddev)) ||
					!is_mddev_idle(mddev, 0)) {
				msleep(500);
				goto repeat;
			}
		}
	}
	printk(KERN_INFO "md: %s: %s done.\n",mdname(mddev), desc);
	/*
	 * this also signals 'finished resyncing' to md_stop
	 */
 out:
	wait_event(mddev->recovery_wait, !atomic_read(&mddev->recovery_active));

	/* tell personality that we are finished */
	mddev->pers->sync_request(mddev, max_sectors, &skipped, 1);

	if (!test_bit(MD_RECOVERY_CHECK, &mddev->recovery) &&
	    mddev->curr_resync > 2) {
		if (test_bit(MD_RECOVERY_SYNC, &mddev->recovery)) {
			if (test_bit(MD_RECOVERY_INTR, &mddev->recovery)) {
				if (mddev->curr_resync >= mddev->recovery_cp) {
					printk(KERN_INFO
					       "md: checkpointing %s of %s.\n",
					       desc, mdname(mddev));
					mddev->recovery_cp =
						mddev->curr_resync_completed;
=======
		recovery_done = io_sectors - atomic_read(&mddev->recovery_active);
		currspeed = ((unsigned long)(recovery_done - mddev->resync_mark_cnt))/2
			/((jiffies-mddev->resync_mark)/HZ +1) +1;

		if (currspeed > speed_min(mddev)) {
			if (currspeed > speed_max(mddev)) {
				msleep(500);
				goto repeat;
			}
			if (!is_mddev_idle(mddev, 0)) {
				/*
				 * Give other IO more of a chance.
				 * The faster the devices, the less we wait.
				 */
				wait_event(mddev->recovery_wait,
					   !atomic_read(&mddev->recovery_active));
			}
		}
	}
	pr_info("md: %s: %s %s.\n",mdname(mddev), desc,
		test_bit(MD_RECOVERY_INTR, &mddev->recovery)
		? "interrupted" : "done");
	/*
	 * this also signals 'finished resyncing' to md_stop
	 */
	blk_finish_plug(&plug);
	wait_event(mddev->recovery_wait, !atomic_read(&mddev->recovery_active));

	if (!test_bit(MD_RECOVERY_RESHAPE, &mddev->recovery) &&
	    !test_bit(MD_RECOVERY_INTR, &mddev->recovery) &&
	    mddev->curr_resync >= MD_RESYNC_ACTIVE) {
		mddev->curr_resync_completed = mddev->curr_resync;
		sysfs_notify_dirent_safe(mddev->sysfs_completed);
	}
	mddev->pers->sync_request(mddev, max_sectors, &skipped);

	if (!test_bit(MD_RECOVERY_CHECK, &mddev->recovery) &&
	    mddev->curr_resync > MD_RESYNC_ACTIVE) {
		if (test_bit(MD_RECOVERY_SYNC, &mddev->recovery)) {
			if (test_bit(MD_RECOVERY_INTR, &mddev->recovery)) {
				if (mddev->curr_resync >= mddev->recovery_cp) {
					pr_debug("md: checkpointing %s of %s.\n",
						 desc, mdname(mddev));
					if (test_bit(MD_RECOVERY_ERROR,
						&mddev->recovery))
						mddev->recovery_cp =
							mddev->curr_resync_completed;
					else
						mddev->recovery_cp =
							mddev->curr_resync;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
				}
			} else
				mddev->recovery_cp = MaxSector;
		} else {
			if (!test_bit(MD_RECOVERY_INTR, &mddev->recovery))
				mddev->curr_resync = MaxSector;
<<<<<<< HEAD
			rcu_read_lock();
			rdev_for_each_rcu(rdev, mddev)
				if (rdev->raid_disk >= 0 &&
				    mddev->delta_disks >= 0 &&
				    !test_bit(Faulty, &rdev->flags) &&
				    !test_bit(In_sync, &rdev->flags) &&
				    rdev->recovery_offset < mddev->curr_resync)
					rdev->recovery_offset = mddev->curr_resync;
			rcu_read_unlock();
		}
	}
 skip:
	set_bit(MD_CHANGE_DEVS, &mddev->flags);

=======
			if (!test_bit(MD_RECOVERY_RESHAPE, &mddev->recovery) &&
			    test_bit(MD_RECOVERY_RECOVER, &mddev->recovery)) {
				rcu_read_lock();
				rdev_for_each_rcu(rdev, mddev)
					if (rdev->raid_disk >= 0 &&
					    mddev->delta_disks >= 0 &&
					    !test_bit(Journal, &rdev->flags) &&
					    !test_bit(Faulty, &rdev->flags) &&
					    !test_bit(In_sync, &rdev->flags) &&
					    rdev->recovery_offset < mddev->curr_resync)
						rdev->recovery_offset = mddev->curr_resync;
				rcu_read_unlock();
			}
		}
	}
 skip:
	/* set CHANGE_PENDING here since maybe another update is needed,
	 * so other nodes are informed. It should be harmless for normal
	 * raid */
	set_mask_bits(&mddev->sb_flags, 0,
		      BIT(MD_SB_CHANGE_PENDING) | BIT(MD_SB_CHANGE_DEVS));

	if (test_bit(MD_RECOVERY_RESHAPE, &mddev->recovery) &&
			!test_bit(MD_RECOVERY_INTR, &mddev->recovery) &&
			mddev->delta_disks > 0 &&
			mddev->pers->finish_reshape &&
			mddev->pers->size &&
			!mddev_is_dm(mddev)) {
		mddev_lock_nointr(mddev);
		md_set_array_sectors(mddev, mddev->pers->size(mddev, 0, 0));
		mddev_unlock(mddev);
		if (!mddev_is_clustered(mddev))
			set_capacity_and_notify(mddev->gendisk,
						mddev->array_sectors);
	}

	spin_lock(&mddev->lock);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	if (!test_bit(MD_RECOVERY_INTR, &mddev->recovery)) {
		/* We completed so min/max setting can be forgotten if used. */
		if (test_bit(MD_RECOVERY_REQUESTED, &mddev->recovery))
			mddev->resync_min = 0;
		mddev->resync_max = MaxSector;
	} else if (test_bit(MD_RECOVERY_REQUESTED, &mddev->recovery))
		mddev->resync_min = mddev->curr_resync_completed;
<<<<<<< HEAD
	mddev->curr_resync = 0;
	wake_up(&resync_wait);
	set_bit(MD_RECOVERY_DONE, &mddev->recovery);
	md_wakeup_thread(mddev->thread);
	return;

 interrupted:
	/*
	 * got a signal, exit.
	 */
	printk(KERN_INFO
	       "md: md_do_sync() got signal ... exiting\n");
	set_bit(MD_RECOVERY_INTR, &mddev->recovery);
	goto out;

}
EXPORT_SYMBOL_GPL(md_do_sync);

static int remove_and_add_spares(struct mddev *mddev)
=======
	set_bit(MD_RECOVERY_DONE, &mddev->recovery);
	mddev->curr_resync = MD_RESYNC_NONE;
	spin_unlock(&mddev->lock);

	wake_up(&resync_wait);
	md_wakeup_thread(mddev->thread);
	return;
}
EXPORT_SYMBOL_GPL(md_do_sync);

static bool rdev_removeable(struct md_rdev *rdev)
{
	/* rdev is not used. */
	if (rdev->raid_disk < 0)
		return false;

	/* There are still inflight io, don't remove this rdev. */
	if (atomic_read(&rdev->nr_pending))
		return false;

	/*
	 * An error occurred but has not yet been acknowledged by the metadata
	 * handler, don't remove this rdev.
	 */
	if (test_bit(Blocked, &rdev->flags))
		return false;

	/* Fautly rdev is not used, it's safe to remove it. */
	if (test_bit(Faulty, &rdev->flags))
		return true;

	/* Journal disk can only be removed if it's faulty. */
	if (test_bit(Journal, &rdev->flags))
		return false;

	/*
	 * 'In_sync' is cleared while 'raid_disk' is valid, which means
	 * replacement has just become active from pers->spare_active(), and
	 * then pers->hot_remove_disk() will replace this rdev with replacement.
	 */
	if (!test_bit(In_sync, &rdev->flags))
		return true;

	return false;
}

static bool rdev_is_spare(struct md_rdev *rdev)
{
	return !test_bit(Candidate, &rdev->flags) && rdev->raid_disk >= 0 &&
	       !test_bit(In_sync, &rdev->flags) &&
	       !test_bit(Journal, &rdev->flags) &&
	       !test_bit(Faulty, &rdev->flags);
}

static bool rdev_addable(struct md_rdev *rdev)
{
	/* rdev is already used, don't add it again. */
	if (test_bit(Candidate, &rdev->flags) || rdev->raid_disk >= 0 ||
	    test_bit(Faulty, &rdev->flags))
		return false;

	/* Allow to add journal disk. */
	if (test_bit(Journal, &rdev->flags))
		return true;

	/* Allow to add if array is read-write. */
	if (md_is_rdwr(rdev->mddev))
		return true;

	/*
	 * For read-only array, only allow to readd a rdev. And if bitmap is
	 * used, don't allow to readd a rdev that is too old.
	 */
	if (rdev->saved_raid_disk >= 0 && !test_bit(Bitmap_sync, &rdev->flags))
		return true;

	return false;
}

static bool md_spares_need_change(struct mddev *mddev)
{
	struct md_rdev *rdev;

	rcu_read_lock();
	rdev_for_each_rcu(rdev, mddev) {
		if (rdev_removeable(rdev) || rdev_addable(rdev)) {
			rcu_read_unlock();
			return true;
		}
	}
	rcu_read_unlock();
	return false;
}

static int remove_and_add_spares(struct mddev *mddev,
				 struct md_rdev *this)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
{
	struct md_rdev *rdev;
	int spares = 0;
	int removed = 0;

<<<<<<< HEAD
	mddev->curr_resync_completed = 0;

	rdev_for_each(rdev, mddev)
		if (rdev->raid_disk >= 0 &&
		    !test_bit(Blocked, &rdev->flags) &&
		    (test_bit(Faulty, &rdev->flags) ||
		     ! test_bit(In_sync, &rdev->flags)) &&
		    atomic_read(&rdev->nr_pending)==0) {
			if (mddev->pers->hot_remove_disk(
				    mddev, rdev) == 0) {
				sysfs_unlink_rdev(mddev, rdev);
				rdev->raid_disk = -1;
				removed++;
			}
		}
	if (removed)
		sysfs_notify(&mddev->kobj, NULL,
			     "degraded");


	rdev_for_each(rdev, mddev) {
		if (rdev->raid_disk >= 0 &&
		    !test_bit(In_sync, &rdev->flags) &&
		    !test_bit(Faulty, &rdev->flags))
			spares++;
		if (rdev->raid_disk < 0
		    && !test_bit(Faulty, &rdev->flags)) {
			rdev->recovery_offset = 0;
			if (mddev->pers->
			    hot_add_disk(mddev, rdev) == 0) {
				if (sysfs_link_rdev(mddev, rdev))
					/* failure here is OK */;
				spares++;
				md_new_event(mddev);
				set_bit(MD_CHANGE_DEVS, &mddev->flags);
			}
		}
	}
	if (removed)
		set_bit(MD_CHANGE_DEVS, &mddev->flags);
	return spares;
}

static void reap_sync_thread(struct mddev *mddev)
{
	struct md_rdev *rdev;

	/* resync has finished, collect result */
	md_unregister_thread(&mddev->sync_thread);
	if (!test_bit(MD_RECOVERY_INTR, &mddev->recovery) &&
	    !test_bit(MD_RECOVERY_REQUESTED, &mddev->recovery)) {
		/* success...*/
		/* activate any spares */
		if (mddev->pers->spare_active(mddev)) {
			sysfs_notify(&mddev->kobj, NULL,
				     "degraded");
			set_bit(MD_CHANGE_DEVS, &mddev->flags);
		}
	}
	if (test_bit(MD_RECOVERY_RESHAPE, &mddev->recovery) &&
	    mddev->pers->finish_reshape)
		mddev->pers->finish_reshape(mddev);

	/* If array is no-longer degraded, then any saved_raid_disk
	 * information must be scrapped.  Also if any device is now
	 * In_sync we must scrape the saved_raid_disk for that device
	 * do the superblock for an incrementally recovered device
	 * written out.
	 */
	rdev_for_each(rdev, mddev)
		if (!mddev->degraded ||
		    test_bit(In_sync, &rdev->flags))
			rdev->saved_raid_disk = -1;

	md_update_sb(mddev, 1);
	clear_bit(MD_RECOVERY_RUNNING, &mddev->recovery);
=======
	if (this && test_bit(MD_RECOVERY_RUNNING, &mddev->recovery))
		/* Mustn't remove devices when resync thread is running */
		return 0;

	rdev_for_each(rdev, mddev) {
		if ((this == NULL || rdev == this) && rdev_removeable(rdev) &&
		    !mddev->pers->hot_remove_disk(mddev, rdev)) {
			sysfs_unlink_rdev(mddev, rdev);
			rdev->saved_raid_disk = rdev->raid_disk;
			rdev->raid_disk = -1;
			removed++;
		}
	}

	if (removed && mddev->kobj.sd)
		sysfs_notify_dirent_safe(mddev->sysfs_degraded);

	if (this && removed)
		goto no_add;

	rdev_for_each(rdev, mddev) {
		if (this && this != rdev)
			continue;
		if (rdev_is_spare(rdev))
			spares++;
		if (!rdev_addable(rdev))
			continue;
		if (!test_bit(Journal, &rdev->flags))
			rdev->recovery_offset = 0;
		if (mddev->pers->hot_add_disk(mddev, rdev) == 0) {
			/* failure here is OK */
			sysfs_link_rdev(mddev, rdev);
			if (!test_bit(Journal, &rdev->flags))
				spares++;
			md_new_event();
			set_bit(MD_SB_CHANGE_DEVS, &mddev->sb_flags);
		}
	}
no_add:
	if (removed)
		set_bit(MD_SB_CHANGE_DEVS, &mddev->sb_flags);
	return spares;
}

static bool md_choose_sync_action(struct mddev *mddev, int *spares)
{
	/* Check if reshape is in progress first. */
	if (mddev->reshape_position != MaxSector) {
		if (mddev->pers->check_reshape == NULL ||
		    mddev->pers->check_reshape(mddev) != 0)
			return false;

		set_bit(MD_RECOVERY_RESHAPE, &mddev->recovery);
		clear_bit(MD_RECOVERY_RECOVER, &mddev->recovery);
		return true;
	}

	/*
	 * Remove any failed drives, then add spares if possible. Spares are
	 * also removed and re-added, to allow the personality to fail the
	 * re-add.
	 */
	*spares = remove_and_add_spares(mddev, NULL);
	if (*spares) {
		clear_bit(MD_RECOVERY_SYNC, &mddev->recovery);
		clear_bit(MD_RECOVERY_CHECK, &mddev->recovery);
		clear_bit(MD_RECOVERY_REQUESTED, &mddev->recovery);

		/* Start new recovery. */
		set_bit(MD_RECOVERY_RECOVER, &mddev->recovery);
		return true;
	}

	/* Check if recovery is in progress. */
	if (mddev->recovery_cp < MaxSector) {
		set_bit(MD_RECOVERY_SYNC, &mddev->recovery);
		clear_bit(MD_RECOVERY_RECOVER, &mddev->recovery);
		return true;
	}

	/* Delay to choose resync/check/repair in md_do_sync(). */
	if (test_bit(MD_RECOVERY_SYNC, &mddev->recovery))
		return true;

	/* Nothing to be done */
	return false;
}

static void md_start_sync(struct work_struct *ws)
{
	struct mddev *mddev = container_of(ws, struct mddev, sync_work);
	int spares = 0;
	bool suspend = false;
	char *name;

	/*
	 * If reshape is still in progress, spares won't be added or removed
	 * from conf until reshape is done.
	 */
	if (mddev->reshape_position == MaxSector &&
	    md_spares_need_change(mddev)) {
		suspend = true;
		mddev_suspend(mddev, false);
	}

	mddev_lock_nointr(mddev);
	if (!md_is_rdwr(mddev)) {
		/*
		 * On a read-only array we can:
		 * - remove failed devices
		 * - add already-in_sync devices if the array itself is in-sync.
		 * As we only add devices that are already in-sync, we can
		 * activate the spares immediately.
		 */
		remove_and_add_spares(mddev, NULL);
		goto not_running;
	}

	if (!md_choose_sync_action(mddev, &spares))
		goto not_running;

	if (!mddev->pers->sync_request)
		goto not_running;

	/*
	 * We are adding a device or devices to an array which has the bitmap
	 * stored on all devices. So make sure all bitmap pages get written.
	 */
	if (spares)
		md_bitmap_write_all(mddev->bitmap);

	name = test_bit(MD_RECOVERY_RESHAPE, &mddev->recovery) ?
			"reshape" : "resync";
	rcu_assign_pointer(mddev->sync_thread,
			   md_register_thread(md_do_sync, mddev, name));
	if (!mddev->sync_thread) {
		pr_warn("%s: could not start resync thread...\n",
			mdname(mddev));
		/* leave the spares where they are, it shouldn't hurt */
		goto not_running;
	}

	mddev_unlock(mddev);
	/*
	 * md_start_sync was triggered by MD_RECOVERY_NEEDED, so we should
	 * not set it again. Otherwise, we may cause issue like this one:
	 *     https://bugzilla.kernel.org/show_bug.cgi?id=218200
	 * Therefore, use __mddev_resume(mddev, false).
	 */
	if (suspend)
		__mddev_resume(mddev, false);
	md_wakeup_thread(mddev->sync_thread);
	sysfs_notify_dirent_safe(mddev->sysfs_action);
	md_new_event();
	return;

not_running:
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	clear_bit(MD_RECOVERY_SYNC, &mddev->recovery);
	clear_bit(MD_RECOVERY_RESHAPE, &mddev->recovery);
	clear_bit(MD_RECOVERY_REQUESTED, &mddev->recovery);
	clear_bit(MD_RECOVERY_CHECK, &mddev->recovery);
<<<<<<< HEAD
	/* flag recovery needed just to double check */
	set_bit(MD_RECOVERY_NEEDED, &mddev->recovery);
	sysfs_notify_dirent_safe(mddev->sysfs_action);
	md_new_event(mddev);
	if (mddev->event_work.func)
		queue_work(md_misc_wq, &mddev->event_work);
=======
	clear_bit(MD_RECOVERY_RUNNING, &mddev->recovery);
	mddev_unlock(mddev);
	/*
	 * md_start_sync was triggered by MD_RECOVERY_NEEDED, so we should
	 * not set it again. Otherwise, we may cause issue like this one:
	 *     https://bugzilla.kernel.org/show_bug.cgi?id=218200
	 * Therefore, use __mddev_resume(mddev, false).
	 */
	if (suspend)
		__mddev_resume(mddev, false);

	wake_up(&resync_wait);
	if (test_and_clear_bit(MD_RECOVERY_RECOVER, &mddev->recovery) &&
	    mddev->sysfs_action)
		sysfs_notify_dirent_safe(mddev->sysfs_action);
}

static void unregister_sync_thread(struct mddev *mddev)
{
	if (!test_bit(MD_RECOVERY_DONE, &mddev->recovery)) {
		/* resync/recovery still happening */
		clear_bit(MD_RECOVERY_NEEDED, &mddev->recovery);
		return;
	}

	if (WARN_ON_ONCE(!mddev->sync_thread))
		return;

	md_reap_sync_thread(mddev);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

/*
 * This routine is regularly called by all per-raid-array threads to
 * deal with generic issues like resync and super-block update.
 * Raid personalities that don't have a thread (linear/raid0) do not
 * need this as they never do any recovery or update the superblock.
 *
 * It does not do any resync itself, but rather "forks" off other threads
 * to do that as needed.
 * When it is determined that resync is needed, we set MD_RECOVERY_RUNNING in
 * "->recovery" and create a thread at ->sync_thread.
 * When the thread finishes it sets MD_RECOVERY_DONE
 * and wakeups up this thread which will reap the thread and finish up.
 * This thread also removes any faulty devices (with nr_pending == 0).
 *
 * The overall approach is:
 *  1/ if the superblock needs updating, update it.
 *  2/ If a recovery thread is running, don't do anything else.
 *  3/ If recovery has finished, clean up, possibly marking spares active.
 *  4/ If there are any faulty devices, remove them.
 *  5/ If array is degraded, try to add spares devices
 *  6/ If array has spares or is not in-sync, start a resync thread.
 */
void md_check_recovery(struct mddev *mddev)
{
<<<<<<< HEAD
	if (mddev->suspended)
		return;

	if (mddev->bitmap)
		bitmap_daemon_work(mddev);

	if (signal_pending(current)) {
		if (mddev->pers->sync_request && !mddev->external) {
			printk(KERN_INFO "md: %s in immediate safe mode\n",
			       mdname(mddev));
=======
	if (mddev->bitmap)
		md_bitmap_daemon_work(mddev);

	if (signal_pending(current)) {
		if (mddev->pers->sync_request && !mddev->external) {
			pr_debug("md: %s in immediate safe mode\n",
				 mdname(mddev));
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
			mddev->safemode = 2;
		}
		flush_signals(current);
	}

<<<<<<< HEAD
	if (mddev->ro && !test_bit(MD_RECOVERY_NEEDED, &mddev->recovery))
		return;
	if ( ! (
		(mddev->flags & ~ (1<<MD_CHANGE_PENDING)) ||
		test_bit(MD_RECOVERY_NEEDED, &mddev->recovery) ||
		test_bit(MD_RECOVERY_DONE, &mddev->recovery) ||
		(mddev->external == 0 && mddev->safemode == 1) ||
		(mddev->safemode == 2 && ! atomic_read(&mddev->writes_pending)
=======
	if (!md_is_rdwr(mddev) &&
	    !test_bit(MD_RECOVERY_NEEDED, &mddev->recovery) &&
	    !test_bit(MD_RECOVERY_DONE, &mddev->recovery))
		return;
	if ( ! (
		(mddev->sb_flags & ~ (1<<MD_SB_CHANGE_PENDING)) ||
		test_bit(MD_RECOVERY_NEEDED, &mddev->recovery) ||
		test_bit(MD_RECOVERY_DONE, &mddev->recovery) ||
		(mddev->external == 0 && mddev->safemode == 1) ||
		(mddev->safemode == 2
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		 && !mddev->in_sync && mddev->recovery_cp == MaxSector)
		))
		return;

	if (mddev_trylock(mddev)) {
<<<<<<< HEAD
		int spares = 0;

		if (mddev->ro) {
			/* Only thing we do on a ro array is remove
			 * failed devices.
			 */
			struct md_rdev *rdev;
			rdev_for_each(rdev, mddev)
				if (rdev->raid_disk >= 0 &&
				    !test_bit(Blocked, &rdev->flags) &&
				    test_bit(Faulty, &rdev->flags) &&
				    atomic_read(&rdev->nr_pending)==0) {
					if (mddev->pers->hot_remove_disk(
						    mddev, rdev) == 0) {
						sysfs_unlink_rdev(mddev, rdev);
						rdev->raid_disk = -1;
					}
				}
			clear_bit(MD_RECOVERY_NEEDED, &mddev->recovery);
			goto unlock;
		}

		if (!mddev->external) {
			int did_change = 0;
			spin_lock_irq(&mddev->write_lock);
			if (mddev->safemode &&
			    !atomic_read(&mddev->writes_pending) &&
			    !mddev->in_sync &&
			    mddev->recovery_cp == MaxSector) {
				mddev->in_sync = 1;
				did_change = 1;
				set_bit(MD_CHANGE_CLEAN, &mddev->flags);
			}
			if (mddev->safemode == 1)
				mddev->safemode = 0;
			spin_unlock_irq(&mddev->write_lock);
			if (did_change)
				sysfs_notify_dirent_safe(mddev->sysfs_state);
		}

		if (mddev->flags)
			md_update_sb(mddev, 0);

		if (test_bit(MD_RECOVERY_RUNNING, &mddev->recovery) &&
		    !test_bit(MD_RECOVERY_DONE, &mddev->recovery)) {
			/* resync/recovery still happening */
			clear_bit(MD_RECOVERY_NEEDED, &mddev->recovery);
			goto unlock;
		}
		if (mddev->sync_thread) {
			reap_sync_thread(mddev);
			goto unlock;
		}
		/* Set RUNNING before clearing NEEDED to avoid
		 * any transients in the value of "sync_action".
		 */
		set_bit(MD_RECOVERY_RUNNING, &mddev->recovery);
=======
		bool try_set_sync = mddev->safemode != 0;

		if (!mddev->external && mddev->safemode == 1)
			mddev->safemode = 0;

		if (!md_is_rdwr(mddev)) {
			struct md_rdev *rdev;

			if (test_bit(MD_RECOVERY_RUNNING, &mddev->recovery)) {
				unregister_sync_thread(mddev);
				goto unlock;
			}

			if (!mddev->external && mddev->in_sync)
				/*
				 * 'Blocked' flag not needed as failed devices
				 * will be recorded if array switched to read/write.
				 * Leaving it set will prevent the device
				 * from being removed.
				 */
				rdev_for_each(rdev, mddev)
					clear_bit(Blocked, &rdev->flags);

			/*
			 * There is no thread, but we need to call
			 * ->spare_active and clear saved_raid_disk
			 */
			set_bit(MD_RECOVERY_INTR, &mddev->recovery);
			md_reap_sync_thread(mddev);

			/*
			 * Let md_start_sync() to remove and add rdevs to the
			 * array.
			 */
			if (md_spares_need_change(mddev)) {
				set_bit(MD_RECOVERY_RUNNING, &mddev->recovery);
				queue_work(md_misc_wq, &mddev->sync_work);
			}

			clear_bit(MD_RECOVERY_RECOVER, &mddev->recovery);
			clear_bit(MD_RECOVERY_NEEDED, &mddev->recovery);
			clear_bit(MD_SB_CHANGE_PENDING, &mddev->sb_flags);

			goto unlock;
		}

		if (mddev_is_clustered(mddev)) {
			struct md_rdev *rdev, *tmp;
			/* kick the device if another node issued a
			 * remove disk.
			 */
			rdev_for_each_safe(rdev, tmp, mddev) {
				if (test_and_clear_bit(ClusterRemove, &rdev->flags) &&
						rdev->raid_disk < 0)
					md_kick_rdev_from_array(rdev);
			}
		}

		if (try_set_sync && !mddev->external && !mddev->in_sync) {
			spin_lock(&mddev->lock);
			set_in_sync(mddev);
			spin_unlock(&mddev->lock);
		}

		if (mddev->sb_flags)
			md_update_sb(mddev, 0);

		/*
		 * Never start a new sync thread if MD_RECOVERY_RUNNING is
		 * still set.
		 */
		if (test_bit(MD_RECOVERY_RUNNING, &mddev->recovery)) {
			unregister_sync_thread(mddev);
			goto unlock;
		}

		/* Set RUNNING before clearing NEEDED to avoid
		 * any transients in the value of "sync_action".
		 */
		mddev->curr_resync_completed = 0;
		spin_lock(&mddev->lock);
		set_bit(MD_RECOVERY_RUNNING, &mddev->recovery);
		spin_unlock(&mddev->lock);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		/* Clear some bits that don't mean anything, but
		 * might be left set
		 */
		clear_bit(MD_RECOVERY_INTR, &mddev->recovery);
		clear_bit(MD_RECOVERY_DONE, &mddev->recovery);

<<<<<<< HEAD
		if (!test_and_clear_bit(MD_RECOVERY_NEEDED, &mddev->recovery) ||
		    test_bit(MD_RECOVERY_FROZEN, &mddev->recovery))
			goto unlock;
		/* no recovery is running.
		 * remove any failed drives, then
		 * add spares if possible.
		 * Spare are also removed and re-added, to allow
		 * the personality to fail the re-add.
		 */

		if (mddev->reshape_position != MaxSector) {
			if (mddev->pers->check_reshape == NULL ||
			    mddev->pers->check_reshape(mddev) != 0)
				/* Cannot proceed */
				goto unlock;
			set_bit(MD_RECOVERY_RESHAPE, &mddev->recovery);
			clear_bit(MD_RECOVERY_RECOVER, &mddev->recovery);
		} else if ((spares = remove_and_add_spares(mddev))) {
			clear_bit(MD_RECOVERY_SYNC, &mddev->recovery);
			clear_bit(MD_RECOVERY_CHECK, &mddev->recovery);
			clear_bit(MD_RECOVERY_REQUESTED, &mddev->recovery);
			set_bit(MD_RECOVERY_RECOVER, &mddev->recovery);
		} else if (mddev->recovery_cp < MaxSector) {
			set_bit(MD_RECOVERY_SYNC, &mddev->recovery);
			clear_bit(MD_RECOVERY_RECOVER, &mddev->recovery);
		} else if (!test_bit(MD_RECOVERY_SYNC, &mddev->recovery))
			/* nothing to be done ... */
			goto unlock;

		if (mddev->pers->sync_request) {
			if (spares && mddev->bitmap && ! mddev->bitmap->file) {
				/* We are adding a device or devices to an array
				 * which has the bitmap stored on all devices.
				 * So make sure all bitmap pages get written
				 */
				bitmap_write_all(mddev->bitmap);
			}
			mddev->sync_thread = md_register_thread(md_do_sync,
								mddev,
								"resync");
			if (!mddev->sync_thread) {
				printk(KERN_ERR "%s: could not start resync"
					" thread...\n", 
					mdname(mddev));
				/* leave the spares where they are, it shouldn't hurt */
				clear_bit(MD_RECOVERY_RUNNING, &mddev->recovery);
				clear_bit(MD_RECOVERY_SYNC, &mddev->recovery);
				clear_bit(MD_RECOVERY_RESHAPE, &mddev->recovery);
				clear_bit(MD_RECOVERY_REQUESTED, &mddev->recovery);
				clear_bit(MD_RECOVERY_CHECK, &mddev->recovery);
			} else
				md_wakeup_thread(mddev->sync_thread);
			sysfs_notify_dirent_safe(mddev->sysfs_action);
			md_new_event(mddev);
		}
	unlock:
		if (!mddev->sync_thread) {
			clear_bit(MD_RECOVERY_RUNNING, &mddev->recovery);
			if (test_and_clear_bit(MD_RECOVERY_RECOVER,
					       &mddev->recovery))
				if (mddev->sysfs_action)
					sysfs_notify_dirent_safe(mddev->sysfs_action);
		}
		mddev_unlock(mddev);
	}
}
=======
		if (test_and_clear_bit(MD_RECOVERY_NEEDED, &mddev->recovery) &&
		    !test_bit(MD_RECOVERY_FROZEN, &mddev->recovery)) {
			queue_work(md_misc_wq, &mddev->sync_work);
		} else {
			clear_bit(MD_RECOVERY_RUNNING, &mddev->recovery);
			wake_up(&resync_wait);
		}

	unlock:
		wake_up(&mddev->sb_wait);
		mddev_unlock(mddev);
	}
}
EXPORT_SYMBOL(md_check_recovery);

void md_reap_sync_thread(struct mddev *mddev)
{
	struct md_rdev *rdev;
	sector_t old_dev_sectors = mddev->dev_sectors;
	bool is_reshaped = false;

	/* resync has finished, collect result */
	md_unregister_thread(mddev, &mddev->sync_thread);
	atomic_inc(&mddev->sync_seq);

	if (!test_bit(MD_RECOVERY_INTR, &mddev->recovery) &&
	    !test_bit(MD_RECOVERY_REQUESTED, &mddev->recovery) &&
	    mddev->degraded != mddev->raid_disks) {
		/* success...*/
		/* activate any spares */
		if (mddev->pers->spare_active(mddev)) {
			sysfs_notify_dirent_safe(mddev->sysfs_degraded);
			set_bit(MD_SB_CHANGE_DEVS, &mddev->sb_flags);
		}
	}
	if (test_bit(MD_RECOVERY_RESHAPE, &mddev->recovery) &&
	    mddev->pers->finish_reshape) {
		mddev->pers->finish_reshape(mddev);
		if (mddev_is_clustered(mddev))
			is_reshaped = true;
	}

	/* If array is no-longer degraded, then any saved_raid_disk
	 * information must be scrapped.
	 */
	if (!mddev->degraded)
		rdev_for_each(rdev, mddev)
			rdev->saved_raid_disk = -1;

	md_update_sb(mddev, 1);
	/* MD_SB_CHANGE_PENDING should be cleared by md_update_sb, so we can
	 * call resync_finish here if MD_CLUSTER_RESYNC_LOCKED is set by
	 * clustered raid */
	if (test_and_clear_bit(MD_CLUSTER_RESYNC_LOCKED, &mddev->flags))
		md_cluster_ops->resync_finish(mddev);
	clear_bit(MD_RECOVERY_RUNNING, &mddev->recovery);
	clear_bit(MD_RECOVERY_DONE, &mddev->recovery);
	clear_bit(MD_RECOVERY_SYNC, &mddev->recovery);
	clear_bit(MD_RECOVERY_RESHAPE, &mddev->recovery);
	clear_bit(MD_RECOVERY_REQUESTED, &mddev->recovery);
	clear_bit(MD_RECOVERY_CHECK, &mddev->recovery);
	/*
	 * We call md_cluster_ops->update_size here because sync_size could
	 * be changed by md_update_sb, and MD_RECOVERY_RESHAPE is cleared,
	 * so it is time to update size across cluster.
	 */
	if (mddev_is_clustered(mddev) && is_reshaped
				      && !test_bit(MD_CLOSING, &mddev->flags))
		md_cluster_ops->update_size(mddev, old_dev_sectors);
	/* flag recovery needed just to double check */
	set_bit(MD_RECOVERY_NEEDED, &mddev->recovery);
	sysfs_notify_dirent_safe(mddev->sysfs_completed);
	sysfs_notify_dirent_safe(mddev->sysfs_action);
	md_new_event();
	if (mddev->event_work.func)
		queue_work(md_misc_wq, &mddev->event_work);
	wake_up(&resync_wait);
}
EXPORT_SYMBOL(md_reap_sync_thread);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

void md_wait_for_blocked_rdev(struct md_rdev *rdev, struct mddev *mddev)
{
	sysfs_notify_dirent_safe(rdev->sysfs_state);
	wait_event_timeout(rdev->blocked_wait,
			   !test_bit(Blocked, &rdev->flags) &&
			   !test_bit(BlockedBadBlocks, &rdev->flags),
			   msecs_to_jiffies(5000));
	rdev_dec_pending(rdev, mddev);
}
EXPORT_SYMBOL(md_wait_for_blocked_rdev);

<<<<<<< HEAD

/* Bad block management.
 * We can record which blocks on each device are 'bad' and so just
 * fail those blocks, or that stripe, rather than the whole device.
 * Entries in the bad-block table are 64bits wide.  This comprises:
 * Length of bad-range, in sectors: 0-511 for lengths 1-512
 * Start of bad-range, sector offset, 54 bits (allows 8 exbibytes)
 *  A 'shift' can be set so that larger blocks are tracked and
 *  consequently larger devices can be covered.
 * 'Acknowledged' flag - 1 bit. - the most significant bit.
 *
 * Locking of the bad-block table uses a seqlock so md_is_badblock
 * might need to retry if it is very unlucky.
 * We will sometimes want to check for bad blocks in a bi_end_io function,
 * so we use the write_seqlock_irq variant.
 *
 * When looking for a bad block we specify a range and want to
 * know if any block in the range is bad.  So we binary-search
 * to the last range that starts at-or-before the given endpoint,
 * (or "before the sector after the target range")
 * then see if it ends after the given start.
 * We return
 *  0 if there are no known bad blocks in the range
 *  1 if there are known bad block which are all acknowledged
 * -1 if there are bad blocks which have not yet been acknowledged in metadata.
 * plus the start/length of the first bad section we overlap.
 */
int md_is_badblock(struct badblocks *bb, sector_t s, int sectors,
		   sector_t *first_bad, int *bad_sectors)
{
	int hi;
	int lo;
	u64 *p = bb->page;
	int rv;
	sector_t target = s + sectors;
	unsigned seq;

	if (bb->shift > 0) {
		/* round the start down, and the end up */
		s >>= bb->shift;
		target += (1<<bb->shift) - 1;
		target >>= bb->shift;
		sectors = target - s;
	}
	/* 'target' is now the first block after the bad range */

retry:
	seq = read_seqbegin(&bb->lock);
	lo = 0;
	rv = 0;
	hi = bb->count;

	/* Binary search between lo and hi for 'target'
	 * i.e. for the last range that starts before 'target'
	 */
	/* INVARIANT: ranges before 'lo' and at-or-after 'hi'
	 * are known not to be the last range before target.
	 * VARIANT: hi-lo is the number of possible
	 * ranges, and decreases until it reaches 1
	 */
	while (hi - lo > 1) {
		int mid = (lo + hi) / 2;
		sector_t a = BB_OFFSET(p[mid]);
		if (a < target)
			/* This could still be the one, earlier ranges
			 * could not. */
			lo = mid;
		else
			/* This and later ranges are definitely out. */
			hi = mid;
	}
	/* 'lo' might be the last that started before target, but 'hi' isn't */
	if (hi > lo) {
		/* need to check all range that end after 's' to see if
		 * any are unacknowledged.
		 */
		while (lo >= 0 &&
		       BB_OFFSET(p[lo]) + BB_LEN(p[lo]) > s) {
			if (BB_OFFSET(p[lo]) < target) {
				/* starts before the end, and finishes after
				 * the start, so they must overlap
				 */
				if (rv != -1 && BB_ACK(p[lo]))
					rv = 1;
				else
					rv = -1;
				*first_bad = BB_OFFSET(p[lo]);
				*bad_sectors = BB_LEN(p[lo]);
			}
			lo--;
		}
	}

	if (read_seqretry(&bb->lock, seq))
		goto retry;

	return rv;
}
EXPORT_SYMBOL_GPL(md_is_badblock);

/*
 * Add a range of bad blocks to the table.
 * This might extend the table, or might contract it
 * if two adjacent ranges can be merged.
 * We binary-search to find the 'insertion' point, then
 * decide how best to handle it.
 */
static int md_set_badblocks(struct badblocks *bb, sector_t s, int sectors,
			    int acknowledged)
{
	u64 *p;
	int lo, hi;
	int rv = 1;

	if (bb->shift < 0)
		/* badblocks are disabled */
		return 0;

	if (bb->shift) {
		/* round the start down, and the end up */
		sector_t next = s + sectors;
		s >>= bb->shift;
		next += (1<<bb->shift) - 1;
		next >>= bb->shift;
		sectors = next - s;
	}

	write_seqlock_irq(&bb->lock);

	p = bb->page;
	lo = 0;
	hi = bb->count;
	/* Find the last range that starts at-or-before 's' */
	while (hi - lo > 1) {
		int mid = (lo + hi) / 2;
		sector_t a = BB_OFFSET(p[mid]);
		if (a <= s)
			lo = mid;
		else
			hi = mid;
	}
	if (hi > lo && BB_OFFSET(p[lo]) > s)
		hi = lo;

	if (hi > lo) {
		/* we found a range that might merge with the start
		 * of our new range
		 */
		sector_t a = BB_OFFSET(p[lo]);
		sector_t e = a + BB_LEN(p[lo]);
		int ack = BB_ACK(p[lo]);
		if (e >= s) {
			/* Yes, we can merge with a previous range */
			if (s == a && s + sectors >= e)
				/* new range covers old */
				ack = acknowledged;
			else
				ack = ack && acknowledged;

			if (e < s + sectors)
				e = s + sectors;
			if (e - a <= BB_MAX_LEN) {
				p[lo] = BB_MAKE(a, e-a, ack);
				s = e;
			} else {
				/* does not all fit in one range,
				 * make p[lo] maximal
				 */
				if (BB_LEN(p[lo]) != BB_MAX_LEN)
					p[lo] = BB_MAKE(a, BB_MAX_LEN, ack);
				s = a + BB_MAX_LEN;
			}
			sectors = e - s;
		}
	}
	if (sectors && hi < bb->count) {
		/* 'hi' points to the first range that starts after 's'.
		 * Maybe we can merge with the start of that range */
		sector_t a = BB_OFFSET(p[hi]);
		sector_t e = a + BB_LEN(p[hi]);
		int ack = BB_ACK(p[hi]);
		if (a <= s + sectors) {
			/* merging is possible */
			if (e <= s + sectors) {
				/* full overlap */
				e = s + sectors;
				ack = acknowledged;
			} else
				ack = ack && acknowledged;

			a = s;
			if (e - a <= BB_MAX_LEN) {
				p[hi] = BB_MAKE(a, e-a, ack);
				s = e;
			} else {
				p[hi] = BB_MAKE(a, BB_MAX_LEN, ack);
				s = a + BB_MAX_LEN;
			}
			sectors = e - s;
			lo = hi;
			hi++;
		}
	}
	if (sectors == 0 && hi < bb->count) {
		/* we might be able to combine lo and hi */
		/* Note: 's' is at the end of 'lo' */
		sector_t a = BB_OFFSET(p[hi]);
		int lolen = BB_LEN(p[lo]);
		int hilen = BB_LEN(p[hi]);
		int newlen = lolen + hilen - (s - a);
		if (s >= a && newlen < BB_MAX_LEN) {
			/* yes, we can combine them */
			int ack = BB_ACK(p[lo]) && BB_ACK(p[hi]);
			p[lo] = BB_MAKE(BB_OFFSET(p[lo]), newlen, ack);
			memmove(p + hi, p + hi + 1,
				(bb->count - hi - 1) * 8);
			bb->count--;
		}
	}
	while (sectors) {
		/* didn't merge (it all).
		 * Need to add a range just before 'hi' */
		if (bb->count >= MD_MAX_BADBLOCKS) {
			/* No room for more */
			rv = 0;
			break;
		} else {
			int this_sectors = sectors;
			memmove(p + hi + 1, p + hi,
				(bb->count - hi) * 8);
			bb->count++;

			if (this_sectors > BB_MAX_LEN)
				this_sectors = BB_MAX_LEN;
			p[hi] = BB_MAKE(s, this_sectors, acknowledged);
			sectors -= this_sectors;
			s += this_sectors;
		}
	}

	bb->changed = 1;
	if (!acknowledged)
		bb->unacked_exist = 1;
	write_sequnlock_irq(&bb->lock);

	return rv;
}

int rdev_set_badblocks(struct md_rdev *rdev, sector_t s, int sectors,
		       int acknowledged)
{
	int rv = md_set_badblocks(&rdev->badblocks,
				  s + rdev->data_offset, sectors, acknowledged);
	if (rv) {
		/* Make sure they get written out promptly */
		sysfs_notify_dirent_safe(rdev->sysfs_state);
		set_bit(MD_CHANGE_CLEAN, &rdev->mddev->flags);
		set_bit(MD_CHANGE_PENDING, &rdev->mddev->flags);
		md_wakeup_thread(rdev->mddev->thread);
	}
	return rv;
}
EXPORT_SYMBOL_GPL(rdev_set_badblocks);

/*
 * Remove a range of bad blocks from the table.
 * This may involve extending the table if we spilt a region,
 * but it must not fail.  So if the table becomes full, we just
 * drop the remove request.
 */
static int md_clear_badblocks(struct badblocks *bb, sector_t s, int sectors)
{
	u64 *p;
	int lo, hi;
	sector_t target = s + sectors;
	int rv = 0;

	if (bb->shift > 0) {
		/* When clearing we round the start up and the end down.
		 * This should not matter as the shift should align with
		 * the block size and no rounding should ever be needed.
		 * However it is better the think a block is bad when it
		 * isn't than to think a block is not bad when it is.
		 */
		s += (1<<bb->shift) - 1;
		s >>= bb->shift;
		target >>= bb->shift;
		sectors = target - s;
	}

	write_seqlock_irq(&bb->lock);

	p = bb->page;
	lo = 0;
	hi = bb->count;
	/* Find the last range that starts before 'target' */
	while (hi - lo > 1) {
		int mid = (lo + hi) / 2;
		sector_t a = BB_OFFSET(p[mid]);
		if (a < target)
			lo = mid;
		else
			hi = mid;
	}
	if (hi > lo) {
		/* p[lo] is the last range that could overlap the
		 * current range.  Earlier ranges could also overlap,
		 * but only this one can overlap the end of the range.
		 */
		if (BB_OFFSET(p[lo]) + BB_LEN(p[lo]) > target) {
			/* Partial overlap, leave the tail of this range */
			int ack = BB_ACK(p[lo]);
			sector_t a = BB_OFFSET(p[lo]);
			sector_t end = a + BB_LEN(p[lo]);

			if (a < s) {
				/* we need to split this range */
				if (bb->count >= MD_MAX_BADBLOCKS) {
					rv = 0;
					goto out;
				}
				memmove(p+lo+1, p+lo, (bb->count - lo) * 8);
				bb->count++;
				p[lo] = BB_MAKE(a, s-a, ack);
				lo++;
			}
			p[lo] = BB_MAKE(target, end - target, ack);
			/* there is no longer an overlap */
			hi = lo;
			lo--;
		}
		while (lo >= 0 &&
		       BB_OFFSET(p[lo]) + BB_LEN(p[lo]) > s) {
			/* This range does overlap */
			if (BB_OFFSET(p[lo]) < s) {
				/* Keep the early parts of this range. */
				int ack = BB_ACK(p[lo]);
				sector_t start = BB_OFFSET(p[lo]);
				p[lo] = BB_MAKE(start, s - start, ack);
				/* now low doesn't overlap, so.. */
				break;
			}
			lo--;
		}
		/* 'lo' is strictly before, 'hi' is strictly after,
		 * anything between needs to be discarded
		 */
		if (hi - lo > 1) {
			memmove(p+lo+1, p+hi, (bb->count - hi) * 8);
			bb->count -= (hi - lo - 1);
		}
	}

	bb->changed = 1;
out:
	write_sequnlock_irq(&bb->lock);
	return rv;
}

int rdev_clear_badblocks(struct md_rdev *rdev, sector_t s, int sectors)
{
	return md_clear_badblocks(&rdev->badblocks,
				  s + rdev->data_offset,
				  sectors);
}
EXPORT_SYMBOL_GPL(rdev_clear_badblocks);

/*
 * Acknowledge all bad blocks in a list.
 * This only succeeds if ->changed is clear.  It is used by
 * in-kernel metadata updates
 */
void md_ack_all_badblocks(struct badblocks *bb)
{
	if (bb->page == NULL || bb->changed)
		/* no point even trying */
		return;
	write_seqlock_irq(&bb->lock);

	if (bb->changed == 0 && bb->unacked_exist) {
		u64 *p = bb->page;
		int i;
		for (i = 0; i < bb->count ; i++) {
			if (!BB_ACK(p[i])) {
				sector_t start = BB_OFFSET(p[i]);
				int len = BB_LEN(p[i]);
				p[i] = BB_MAKE(start, len, 1);
			}
		}
		bb->unacked_exist = 0;
	}
	write_sequnlock_irq(&bb->lock);
}
EXPORT_SYMBOL_GPL(md_ack_all_badblocks);

/* sysfs access to bad-blocks list.
 * We present two files.
 * 'bad-blocks' lists sector numbers and lengths of ranges that
 *    are recorded as bad.  The list is truncated to fit within
 *    the one-page limit of sysfs.
 *    Writing "sector length" to this file adds an acknowledged
 *    bad block list.
 * 'unacknowledged-bad-blocks' lists bad blocks that have not yet
 *    been acknowledged.  Writing to this file adds bad blocks
 *    without acknowledging them.  This is largely for testing.
 */

static ssize_t
badblocks_show(struct badblocks *bb, char *page, int unack)
{
	size_t len;
	int i;
	u64 *p = bb->page;
	unsigned seq;

	if (bb->shift < 0)
		return 0;

retry:
	seq = read_seqbegin(&bb->lock);

	len = 0;
	i = 0;

	while (len < PAGE_SIZE && i < bb->count) {
		sector_t s = BB_OFFSET(p[i]);
		unsigned int length = BB_LEN(p[i]);
		int ack = BB_ACK(p[i]);
		i++;

		if (unack && ack)
			continue;

		len += snprintf(page+len, PAGE_SIZE-len, "%llu %u\n",
				(unsigned long long)s << bb->shift,
				length << bb->shift);
	}
	if (unack && len == 0)
		bb->unacked_exist = 0;

	if (read_seqretry(&bb->lock, seq))
		goto retry;

	return len;
}

#define DO_DEBUG 1

static ssize_t
badblocks_store(struct badblocks *bb, const char *page, size_t len, int unack)
{
	unsigned long long sector;
	int length;
	char newline;
#ifdef DO_DEBUG
	/* Allow clearing via sysfs *only* for testing/debugging.
	 * Normally only a successful write may clear a badblock
	 */
	int clear = 0;
	if (page[0] == '-') {
		clear = 1;
		page++;
	}
#endif /* DO_DEBUG */

	switch (sscanf(page, "%llu %d%c", &sector, &length, &newline)) {
	case 3:
		if (newline != '\n')
			return -EINVAL;
	case 2:
		if (length <= 0)
			return -EINVAL;
		break;
	default:
		return -EINVAL;
	}

#ifdef DO_DEBUG
	if (clear) {
		md_clear_badblocks(bb, sector, length);
		return len;
	}
#endif /* DO_DEBUG */
	if (md_set_badblocks(bb, sector, length, !unack))
		return len;
	else
		return -ENOSPC;
}

static int md_notify_reboot(struct notifier_block *this,
			    unsigned long code, void *x)
{
	struct list_head *tmp;
	struct mddev *mddev;
	int need_delay = 0;

	for_each_mddev(mddev, tmp) {
=======
void md_finish_reshape(struct mddev *mddev)
{
	/* called be personality module when reshape completes. */
	struct md_rdev *rdev;

	rdev_for_each(rdev, mddev) {
		if (rdev->data_offset > rdev->new_data_offset)
			rdev->sectors += rdev->data_offset - rdev->new_data_offset;
		else
			rdev->sectors -= rdev->new_data_offset - rdev->data_offset;
		rdev->data_offset = rdev->new_data_offset;
	}
}
EXPORT_SYMBOL(md_finish_reshape);

/* Bad block management */

/* Returns 1 on success, 0 on failure */
int rdev_set_badblocks(struct md_rdev *rdev, sector_t s, int sectors,
		       int is_new)
{
	struct mddev *mddev = rdev->mddev;
	int rv;
	if (is_new)
		s += rdev->new_data_offset;
	else
		s += rdev->data_offset;
	rv = badblocks_set(&rdev->badblocks, s, sectors, 0);
	if (rv == 0) {
		/* Make sure they get written out promptly */
		if (test_bit(ExternalBbl, &rdev->flags))
			sysfs_notify_dirent_safe(rdev->sysfs_unack_badblocks);
		sysfs_notify_dirent_safe(rdev->sysfs_state);
		set_mask_bits(&mddev->sb_flags, 0,
			      BIT(MD_SB_CHANGE_CLEAN) | BIT(MD_SB_CHANGE_PENDING));
		md_wakeup_thread(rdev->mddev->thread);
		return 1;
	} else
		return 0;
}
EXPORT_SYMBOL_GPL(rdev_set_badblocks);

int rdev_clear_badblocks(struct md_rdev *rdev, sector_t s, int sectors,
			 int is_new)
{
	int rv;
	if (is_new)
		s += rdev->new_data_offset;
	else
		s += rdev->data_offset;
	rv = badblocks_clear(&rdev->badblocks, s, sectors);
	if ((rv == 0) && test_bit(ExternalBbl, &rdev->flags))
		sysfs_notify_dirent_safe(rdev->sysfs_badblocks);
	return rv;
}
EXPORT_SYMBOL_GPL(rdev_clear_badblocks);

static int md_notify_reboot(struct notifier_block *this,
			    unsigned long code, void *x)
{
	struct mddev *mddev, *n;
	int need_delay = 0;

	spin_lock(&all_mddevs_lock);
	list_for_each_entry_safe(mddev, n, &all_mddevs, all_mddevs) {
		if (!mddev_get(mddev))
			continue;
		spin_unlock(&all_mddevs_lock);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		if (mddev_trylock(mddev)) {
			if (mddev->pers)
				__md_stop_writes(mddev);
			if (mddev->persistent)
				mddev->safemode = 2;
			mddev_unlock(mddev);
		}
		need_delay = 1;
<<<<<<< HEAD
	}
=======
		mddev_put(mddev);
		spin_lock(&all_mddevs_lock);
	}
	spin_unlock(&all_mddevs_lock);

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	/*
	 * certain more exotic SCSI devices are known to be
	 * volatile wrt too early system reboots. While the
	 * right place to handle this issue is the given
	 * driver, we do want to have a safe RAID driver ...
	 */
	if (need_delay)
<<<<<<< HEAD
		mdelay(1000*1);
=======
		msleep(1000);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	return NOTIFY_DONE;
}

static struct notifier_block md_notifier = {
	.notifier_call	= md_notify_reboot,
	.next		= NULL,
	.priority	= INT_MAX, /* before any real devices */
};

static void md_geninit(void)
{
	pr_debug("md: sizeof(mdp_super_t) = %d\n", (int)sizeof(mdp_super_t));

<<<<<<< HEAD
	proc_create("mdstat", S_IRUGO, NULL, &md_seq_fops);
=======
	proc_create("mdstat", S_IRUGO, NULL, &mdstat_proc_ops);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

static int __init md_init(void)
{
	int ret = -ENOMEM;

	md_wq = alloc_workqueue("md", WQ_MEM_RECLAIM, 0);
	if (!md_wq)
		goto err_wq;

	md_misc_wq = alloc_workqueue("md_misc", 0, 0);
	if (!md_misc_wq)
		goto err_misc_wq;

<<<<<<< HEAD
	if ((ret = register_blkdev(MD_MAJOR, "md")) < 0)
		goto err_md;

	if ((ret = register_blkdev(0, "mdp")) < 0)
		goto err_mdp;
	mdp_major = ret;

	blk_register_region(MKDEV(MD_MAJOR, 0), 1UL<<MINORBITS, THIS_MODULE,
			    md_probe, NULL, NULL);
	blk_register_region(MKDEV(mdp_major, 0), 1UL<<MINORBITS, THIS_MODULE,
			    md_probe, NULL, NULL);

	register_reboot_notifier(&md_notifier);
	raid_table_header = register_sysctl_table(raid_root_table);
=======
	md_bitmap_wq = alloc_workqueue("md_bitmap", WQ_MEM_RECLAIM | WQ_UNBOUND,
				       0);
	if (!md_bitmap_wq)
		goto err_bitmap_wq;

	ret = __register_blkdev(MD_MAJOR, "md", md_probe);
	if (ret < 0)
		goto err_md;

	ret = __register_blkdev(0, "mdp", md_probe);
	if (ret < 0)
		goto err_mdp;
	mdp_major = ret;

	register_reboot_notifier(&md_notifier);
	raid_table_header = register_sysctl("dev/raid", raid_table);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	md_geninit();
	return 0;

err_mdp:
	unregister_blkdev(MD_MAJOR, "md");
err_md:
<<<<<<< HEAD
=======
	destroy_workqueue(md_bitmap_wq);
err_bitmap_wq:
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	destroy_workqueue(md_misc_wq);
err_misc_wq:
	destroy_workqueue(md_wq);
err_wq:
	return ret;
}

<<<<<<< HEAD
=======
static void check_sb_changes(struct mddev *mddev, struct md_rdev *rdev)
{
	struct mdp_superblock_1 *sb = page_address(rdev->sb_page);
	struct md_rdev *rdev2, *tmp;
	int role, ret;

	/*
	 * If size is changed in another node then we need to
	 * do resize as well.
	 */
	if (mddev->dev_sectors != le64_to_cpu(sb->size)) {
		ret = mddev->pers->resize(mddev, le64_to_cpu(sb->size));
		if (ret)
			pr_info("md-cluster: resize failed\n");
		else
			md_bitmap_update_sb(mddev->bitmap);
	}

	/* Check for change of roles in the active devices */
	rdev_for_each_safe(rdev2, tmp, mddev) {
		if (test_bit(Faulty, &rdev2->flags))
			continue;

		/* Check if the roles changed */
		role = le16_to_cpu(sb->dev_roles[rdev2->desc_nr]);

		if (test_bit(Candidate, &rdev2->flags)) {
			if (role == MD_DISK_ROLE_FAULTY) {
				pr_info("md: Removing Candidate device %pg because add failed\n",
					rdev2->bdev);
				md_kick_rdev_from_array(rdev2);
				continue;
			}
			else
				clear_bit(Candidate, &rdev2->flags);
		}

		if (role != rdev2->raid_disk) {
			/*
			 * got activated except reshape is happening.
			 */
			if (rdev2->raid_disk == -1 && role != MD_DISK_ROLE_SPARE &&
			    !(le32_to_cpu(sb->feature_map) &
			      MD_FEATURE_RESHAPE_ACTIVE)) {
				rdev2->saved_raid_disk = role;
				ret = remove_and_add_spares(mddev, rdev2);
				pr_info("Activated spare: %pg\n",
					rdev2->bdev);
				/* wakeup mddev->thread here, so array could
				 * perform resync with the new activated disk */
				set_bit(MD_RECOVERY_NEEDED, &mddev->recovery);
				md_wakeup_thread(mddev->thread);
			}
			/* device faulty
			 * We just want to do the minimum to mark the disk
			 * as faulty. The recovery is performed by the
			 * one who initiated the error.
			 */
			if (role == MD_DISK_ROLE_FAULTY ||
			    role == MD_DISK_ROLE_JOURNAL) {
				md_error(mddev, rdev2);
				clear_bit(Blocked, &rdev2->flags);
			}
		}
	}

	if (mddev->raid_disks != le32_to_cpu(sb->raid_disks)) {
		ret = update_raid_disks(mddev, le32_to_cpu(sb->raid_disks));
		if (ret)
			pr_warn("md: updating array disks failed. %d\n", ret);
	}

	/*
	 * Since mddev->delta_disks has already updated in update_raid_disks,
	 * so it is time to check reshape.
	 */
	if (test_bit(MD_RESYNCING_REMOTE, &mddev->recovery) &&
	    (le32_to_cpu(sb->feature_map) & MD_FEATURE_RESHAPE_ACTIVE)) {
		/*
		 * reshape is happening in the remote node, we need to
		 * update reshape_position and call start_reshape.
		 */
		mddev->reshape_position = le64_to_cpu(sb->reshape_position);
		if (mddev->pers->update_reshape_pos)
			mddev->pers->update_reshape_pos(mddev);
		if (mddev->pers->start_reshape)
			mddev->pers->start_reshape(mddev);
	} else if (test_bit(MD_RESYNCING_REMOTE, &mddev->recovery) &&
		   mddev->reshape_position != MaxSector &&
		   !(le32_to_cpu(sb->feature_map) & MD_FEATURE_RESHAPE_ACTIVE)) {
		/* reshape is just done in another node. */
		mddev->reshape_position = MaxSector;
		if (mddev->pers->update_reshape_pos)
			mddev->pers->update_reshape_pos(mddev);
	}

	/* Finally set the event to be up to date */
	mddev->events = le64_to_cpu(sb->events);
}

static int read_rdev(struct mddev *mddev, struct md_rdev *rdev)
{
	int err;
	struct page *swapout = rdev->sb_page;
	struct mdp_superblock_1 *sb;

	/* Store the sb page of the rdev in the swapout temporary
	 * variable in case we err in the future
	 */
	rdev->sb_page = NULL;
	err = alloc_disk_sb(rdev);
	if (err == 0) {
		ClearPageUptodate(rdev->sb_page);
		rdev->sb_loaded = 0;
		err = super_types[mddev->major_version].
			load_super(rdev, NULL, mddev->minor_version);
	}
	if (err < 0) {
		pr_warn("%s: %d Could not reload rdev(%d) err: %d. Restoring old values\n",
				__func__, __LINE__, rdev->desc_nr, err);
		if (rdev->sb_page)
			put_page(rdev->sb_page);
		rdev->sb_page = swapout;
		rdev->sb_loaded = 1;
		return err;
	}

	sb = page_address(rdev->sb_page);
	/* Read the offset unconditionally, even if MD_FEATURE_RECOVERY_OFFSET
	 * is not set
	 */

	if ((le32_to_cpu(sb->feature_map) & MD_FEATURE_RECOVERY_OFFSET))
		rdev->recovery_offset = le64_to_cpu(sb->recovery_offset);

	/* The other node finished recovery, call spare_active to set
	 * device In_sync and mddev->degraded
	 */
	if (rdev->recovery_offset == MaxSector &&
	    !test_bit(In_sync, &rdev->flags) &&
	    mddev->pers->spare_active(mddev))
		sysfs_notify_dirent_safe(mddev->sysfs_degraded);

	put_page(swapout);
	return 0;
}

void md_reload_sb(struct mddev *mddev, int nr)
{
	struct md_rdev *rdev = NULL, *iter;
	int err;

	/* Find the rdev */
	rdev_for_each_rcu(iter, mddev) {
		if (iter->desc_nr == nr) {
			rdev = iter;
			break;
		}
	}

	if (!rdev) {
		pr_warn("%s: %d Could not find rdev with nr %d\n", __func__, __LINE__, nr);
		return;
	}

	err = read_rdev(mddev, rdev);
	if (err < 0)
		return;

	check_sb_changes(mddev, rdev);

	/* Read all rdev's to update recovery_offset */
	rdev_for_each_rcu(rdev, mddev) {
		if (!test_bit(Faulty, &rdev->flags))
			read_rdev(mddev, rdev);
	}
}
EXPORT_SYMBOL(md_reload_sb);

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#ifndef MODULE

/*
 * Searches all registered partitions for autorun RAID arrays
 * at boot time.
 */

<<<<<<< HEAD
=======
static DEFINE_MUTEX(detected_devices_mutex);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
static LIST_HEAD(all_detected_devices);
struct detected_devices_node {
	struct list_head list;
	dev_t dev;
};

void md_autodetect_dev(dev_t dev)
{
	struct detected_devices_node *node_detected_dev;

	node_detected_dev = kzalloc(sizeof(*node_detected_dev), GFP_KERNEL);
	if (node_detected_dev) {
		node_detected_dev->dev = dev;
<<<<<<< HEAD
		list_add_tail(&node_detected_dev->list, &all_detected_devices);
	} else {
		printk(KERN_CRIT "md: md_autodetect_dev: kzalloc failed"
			", skipping dev(%d,%d)\n", MAJOR(dev), MINOR(dev));
	}
}


static void autostart_arrays(int part)
=======
		mutex_lock(&detected_devices_mutex);
		list_add_tail(&node_detected_dev->list, &all_detected_devices);
		mutex_unlock(&detected_devices_mutex);
	}
}

void md_autostart_arrays(int part)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
{
	struct md_rdev *rdev;
	struct detected_devices_node *node_detected_dev;
	dev_t dev;
	int i_scanned, i_passed;

	i_scanned = 0;
	i_passed = 0;

<<<<<<< HEAD
	printk(KERN_INFO "md: Autodetecting RAID arrays.\n");

=======
	pr_info("md: Autodetecting RAID arrays.\n");

	mutex_lock(&detected_devices_mutex);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	while (!list_empty(&all_detected_devices) && i_scanned < INT_MAX) {
		i_scanned++;
		node_detected_dev = list_entry(all_detected_devices.next,
					struct detected_devices_node, list);
		list_del(&node_detected_dev->list);
		dev = node_detected_dev->dev;
		kfree(node_detected_dev);
<<<<<<< HEAD
		rdev = md_import_device(dev,0, 90);
		if (IS_ERR(rdev))
			continue;

		if (test_bit(Faulty, &rdev->flags)) {
			MD_BUG();
			continue;
		}
=======
		mutex_unlock(&detected_devices_mutex);
		rdev = md_import_device(dev,0, 90);
		mutex_lock(&detected_devices_mutex);
		if (IS_ERR(rdev))
			continue;

		if (test_bit(Faulty, &rdev->flags))
			continue;

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		set_bit(AutoDetected, &rdev->flags);
		list_add(&rdev->same_set, &pending_raid_disks);
		i_passed++;
	}
<<<<<<< HEAD

	printk(KERN_INFO "md: Scanned %d and added %d devices.\n",
						i_scanned, i_passed);
=======
	mutex_unlock(&detected_devices_mutex);

	pr_debug("md: Scanned %d and added %d devices.\n", i_scanned, i_passed);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	autorun_devices(part);
}

#endif /* !MODULE */

static __exit void md_exit(void)
{
<<<<<<< HEAD
	struct mddev *mddev;
	struct list_head *tmp;

	blk_unregister_region(MKDEV(MD_MAJOR,0), 1U << MINORBITS);
	blk_unregister_region(MKDEV(mdp_major,0), 1U << MINORBITS);
=======
	struct mddev *mddev, *n;
	int delay = 1;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	unregister_blkdev(MD_MAJOR,"md");
	unregister_blkdev(mdp_major, "mdp");
	unregister_reboot_notifier(&md_notifier);
	unregister_sysctl_table(raid_table_header);
<<<<<<< HEAD
	remove_proc_entry("mdstat", NULL);
	for_each_mddev(mddev, tmp) {
		export_array(mddev);
		mddev->hold_active = 0;
	}
	destroy_workqueue(md_misc_wq);
=======

	/* We cannot unload the modules while some process is
	 * waiting for us in select() or poll() - wake them up
	 */
	md_unloading = 1;
	while (waitqueue_active(&md_event_waiters)) {
		/* not safe to leave yet */
		wake_up(&md_event_waiters);
		msleep(delay);
		delay += delay;
	}
	remove_proc_entry("mdstat", NULL);

	spin_lock(&all_mddevs_lock);
	list_for_each_entry_safe(mddev, n, &all_mddevs, all_mddevs) {
		if (!mddev_get(mddev))
			continue;
		spin_unlock(&all_mddevs_lock);
		export_array(mddev);
		mddev->ctime = 0;
		mddev->hold_active = 0;
		/*
		 * As the mddev is now fully clear, mddev_put will schedule
		 * the mddev for destruction by a workqueue, and the
		 * destroy_workqueue() below will wait for that to complete.
		 */
		mddev_put(mddev);
		spin_lock(&all_mddevs_lock);
	}
	spin_unlock(&all_mddevs_lock);

	destroy_workqueue(md_misc_wq);
	destroy_workqueue(md_bitmap_wq);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	destroy_workqueue(md_wq);
}

subsys_initcall(md_init);
module_exit(md_exit)

<<<<<<< HEAD
static int get_ro(char *buffer, struct kernel_param *kp)
{
	return sprintf(buffer, "%d", start_readonly);
}
static int set_ro(const char *val, struct kernel_param *kp)
{
	char *e;
	int num = simple_strtoul(val, &e, 10);
	if (*val && (*e == '\0' || *e == '\n')) {
		start_readonly = num;
		return 0;
	}
	return -EINVAL;
=======
static int get_ro(char *buffer, const struct kernel_param *kp)
{
	return sprintf(buffer, "%d\n", start_readonly);
}
static int set_ro(const char *val, const struct kernel_param *kp)
{
	return kstrtouint(val, 10, (unsigned int *)&start_readonly);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

module_param_call(start_ro, set_ro, get_ro, NULL, S_IRUSR|S_IWUSR);
module_param(start_dirty_degraded, int, S_IRUGO|S_IWUSR);
<<<<<<< HEAD

module_param_call(new_array, add_named_array, NULL, NULL, S_IWUSR);

EXPORT_SYMBOL(register_md_personality);
EXPORT_SYMBOL(unregister_md_personality);
EXPORT_SYMBOL(md_error);
EXPORT_SYMBOL(md_done_sync);
EXPORT_SYMBOL(md_write_start);
EXPORT_SYMBOL(md_write_end);
EXPORT_SYMBOL(md_register_thread);
EXPORT_SYMBOL(md_unregister_thread);
EXPORT_SYMBOL(md_wakeup_thread);
EXPORT_SYMBOL(md_check_recovery);
=======
module_param_call(new_array, add_named_array, NULL, NULL, S_IWUSR);
module_param(create_on_open, bool, S_IRUSR|S_IWUSR);

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("MD RAID framework");
MODULE_ALIAS("md");
MODULE_ALIAS_BLOCKDEV_MAJOR(MD_MAJOR);
