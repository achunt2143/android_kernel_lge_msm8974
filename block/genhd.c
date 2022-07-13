<<<<<<< HEAD
/*
 *  gendisk handling
 */

#include <linux/module.h>
#include <linux/fs.h>
#include <linux/genhd.h>
#include <linux/kdev_t.h>
#include <linux/kernel.h>
#include <linux/blkdev.h>
=======
// SPDX-License-Identifier: GPL-2.0
/*
 *  gendisk handling
 *
 * Portions Copyright (C) 2020 Christoph Hellwig
 */

#include <linux/module.h>
#include <linux/ctype.h>
#include <linux/fs.h>
#include <linux/kdev_t.h>
#include <linux/kernel.h>
#include <linux/blkdev.h>
#include <linux/backing-dev.h>
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#include <linux/init.h>
#include <linux/spinlock.h>
#include <linux/proc_fs.h>
#include <linux/seq_file.h>
#include <linux/slab.h>
#include <linux/kmod.h>
<<<<<<< HEAD
#include <linux/kobj_map.h>
#include <linux/mutex.h>
#include <linux/idr.h>
#include <linux/log2.h>

#include "blk.h"

static DEFINE_MUTEX(block_class_lock);
struct kobject *block_depr;

/* for extended dynamic devt allocation, currently only one major is used */
#define NR_EXT_DEVT		(1 << MINORBITS)

/* For extended devt allocation.  ext_devt_lock prevents look up
 * results from going away underneath its user.
 */
static DEFINE_SPINLOCK(ext_devt_lock);
static DEFINE_IDR(ext_devt_idr);

static struct device_type disk_type;

static void disk_alloc_events(struct gendisk *disk);
static void disk_add_events(struct gendisk *disk);
static void disk_del_events(struct gendisk *disk);
static void disk_release_events(struct gendisk *disk);

/**
 * disk_get_part - get partition
 * @disk: disk to look partition from
 * @partno: partition number
 *
 * Look for partition @partno from @disk.  If found, increment
 * reference count and return it.
 *
 * CONTEXT:
 * Don't care.
 *
 * RETURNS:
 * Pointer to the found partition on success, NULL if not found.
 */
struct hd_struct *disk_get_part(struct gendisk *disk, int partno)
{
	struct hd_struct *part = NULL;
	struct disk_part_tbl *ptbl;

	if (unlikely(partno < 0))
		return NULL;

	rcu_read_lock();

	ptbl = rcu_dereference(disk->part_tbl);
	if (likely(partno < ptbl->len)) {
		part = rcu_dereference(ptbl->part[partno]);
		if (part)
			get_device(part_to_dev(part));
	}

	rcu_read_unlock();

	return part;
}
EXPORT_SYMBOL_GPL(disk_get_part);

/**
 * disk_part_iter_init - initialize partition iterator
 * @piter: iterator to initialize
 * @disk: disk to iterate over
 * @flags: DISK_PITER_* flags
 *
 * Initialize @piter so that it iterates over partitions of @disk.
 *
 * CONTEXT:
 * Don't care.
 */
void disk_part_iter_init(struct disk_part_iter *piter, struct gendisk *disk,
			  unsigned int flags)
{
	struct disk_part_tbl *ptbl;

	rcu_read_lock();
	ptbl = rcu_dereference(disk->part_tbl);

	piter->disk = disk;
	piter->part = NULL;

	if (flags & DISK_PITER_REVERSE)
		piter->idx = ptbl->len - 1;
	else if (flags & (DISK_PITER_INCL_PART0 | DISK_PITER_INCL_EMPTY_PART0))
		piter->idx = 0;
	else
		piter->idx = 1;

	piter->flags = flags;

	rcu_read_unlock();
}
EXPORT_SYMBOL_GPL(disk_part_iter_init);

/**
 * disk_part_iter_next - proceed iterator to the next partition and return it
 * @piter: iterator of interest
 *
 * Proceed @piter to the next partition and return it.
 *
 * CONTEXT:
 * Don't care.
 */
struct hd_struct *disk_part_iter_next(struct disk_part_iter *piter)
{
	struct disk_part_tbl *ptbl;
	int inc, end;

	/* put the last partition */
	disk_put_part(piter->part);
	piter->part = NULL;

	/* get part_tbl */
	rcu_read_lock();
	ptbl = rcu_dereference(piter->disk->part_tbl);

	/* determine iteration parameters */
	if (piter->flags & DISK_PITER_REVERSE) {
		inc = -1;
		if (piter->flags & (DISK_PITER_INCL_PART0 |
				    DISK_PITER_INCL_EMPTY_PART0))
			end = -1;
		else
			end = 0;
	} else {
		inc = 1;
		end = ptbl->len;
	}

	/* iterate to the next partition */
	for (; piter->idx != end; piter->idx += inc) {
		struct hd_struct *part;

		part = rcu_dereference(ptbl->part[piter->idx]);
		if (!part)
			continue;
		if (!part->nr_sects &&
		    !(piter->flags & DISK_PITER_INCL_EMPTY) &&
		    !(piter->flags & DISK_PITER_INCL_EMPTY_PART0 &&
		      piter->idx == 0))
			continue;

		get_device(part_to_dev(part));
		piter->part = part;
		piter->idx += inc;
		break;
	}

	rcu_read_unlock();

	return piter->part;
}
EXPORT_SYMBOL_GPL(disk_part_iter_next);

/**
 * disk_part_iter_exit - finish up partition iteration
 * @piter: iter of interest
 *
 * Called when iteration is over.  Cleans up @piter.
 *
 * CONTEXT:
 * Don't care.
 */
void disk_part_iter_exit(struct disk_part_iter *piter)
{
	disk_put_part(piter->part);
	piter->part = NULL;
}
EXPORT_SYMBOL_GPL(disk_part_iter_exit);

static inline int sector_in_part(struct hd_struct *part, sector_t sector)
{
	return part->start_sect <= sector &&
		sector < part->start_sect + part->nr_sects;
}

/**
 * disk_map_sector_rcu - map sector to partition
 * @disk: gendisk of interest
 * @sector: sector to map
 *
 * Find out which partition @sector maps to on @disk.  This is
 * primarily used for stats accounting.
 *
 * CONTEXT:
 * RCU read locked.  The returned partition pointer is valid only
 * while preemption is disabled.
 *
 * RETURNS:
 * Found partition on success, part0 is returned if no partition matches
 */
struct hd_struct *disk_map_sector_rcu(struct gendisk *disk, sector_t sector)
{
	struct disk_part_tbl *ptbl;
	struct hd_struct *part;
	int i;

	ptbl = rcu_dereference(disk->part_tbl);

	part = rcu_dereference(ptbl->last_lookup);
	if (part && sector_in_part(part, sector))
		return part;

	for (i = 1; i < ptbl->len; i++) {
		part = rcu_dereference(ptbl->part[i]);

		if (part && sector_in_part(part, sector)) {
			rcu_assign_pointer(ptbl->last_lookup, part);
			return part;
		}
	}
	return &disk->part0;
}
EXPORT_SYMBOL_GPL(disk_map_sector_rcu);
=======
#include <linux/major.h>
#include <linux/mutex.h>
#include <linux/idr.h>
#include <linux/log2.h>
#include <linux/pm_runtime.h>
#include <linux/badblocks.h>
#include <linux/part_stat.h>
#include <linux/blktrace_api.h>

#include "blk-throttle.h"
#include "blk.h"
#include "blk-mq-sched.h"
#include "blk-rq-qos.h"
#include "blk-cgroup.h"

static struct kobject *block_depr;

/*
 * Unique, monotonically increasing sequential number associated with block
 * devices instances (i.e. incremented each time a device is attached).
 * Associating uevents with block devices in userspace is difficult and racy:
 * the uevent netlink socket is lossy, and on slow and overloaded systems has
 * a very high latency.
 * Block devices do not have exclusive owners in userspace, any process can set
 * one up (e.g. loop devices). Moreover, device names can be reused (e.g. loop0
 * can be reused again and again).
 * A userspace process setting up a block device and watching for its events
 * cannot thus reliably tell whether an event relates to the device it just set
 * up or another earlier instance with the same name.
 * This sequential number allows userspace processes to solve this problem, and
 * uniquely associate an uevent to the lifetime to a device.
 */
static atomic64_t diskseq;

/* for extended dynamic devt allocation, currently only one major is used */
#define NR_EXT_DEVT		(1 << MINORBITS)
static DEFINE_IDA(ext_devt_ida);

void set_capacity(struct gendisk *disk, sector_t sectors)
{
	bdev_set_nr_sectors(disk->part0, sectors);
}
EXPORT_SYMBOL(set_capacity);

/*
 * Set disk capacity and notify if the size is not currently zero and will not
 * be set to zero.  Returns true if a uevent was sent, otherwise false.
 */
bool set_capacity_and_notify(struct gendisk *disk, sector_t size)
{
	sector_t capacity = get_capacity(disk);
	char *envp[] = { "RESIZE=1", NULL };

	set_capacity(disk, size);

	/*
	 * Only print a message and send a uevent if the gendisk is user visible
	 * and alive.  This avoids spamming the log and udev when setting the
	 * initial capacity during probing.
	 */
	if (size == capacity ||
	    !disk_live(disk) ||
	    (disk->flags & GENHD_FL_HIDDEN))
		return false;

	pr_info("%s: detected capacity change from %lld to %lld\n",
		disk->disk_name, capacity, size);

	/*
	 * Historically we did not send a uevent for changes to/from an empty
	 * device.
	 */
	if (!capacity || !size)
		return false;
	kobject_uevent_env(&disk_to_dev(disk)->kobj, KOBJ_CHANGE, envp);
	return true;
}
EXPORT_SYMBOL_GPL(set_capacity_and_notify);

static void part_stat_read_all(struct block_device *part,
		struct disk_stats *stat)
{
	int cpu;

	memset(stat, 0, sizeof(struct disk_stats));
	for_each_possible_cpu(cpu) {
		struct disk_stats *ptr = per_cpu_ptr(part->bd_stats, cpu);
		int group;

		for (group = 0; group < NR_STAT_GROUPS; group++) {
			stat->nsecs[group] += ptr->nsecs[group];
			stat->sectors[group] += ptr->sectors[group];
			stat->ios[group] += ptr->ios[group];
			stat->merges[group] += ptr->merges[group];
		}

		stat->io_ticks += ptr->io_ticks;
	}
}

static unsigned int part_in_flight(struct block_device *part)
{
	unsigned int inflight = 0;
	int cpu;

	for_each_possible_cpu(cpu) {
		inflight += part_stat_local_read_cpu(part, in_flight[0], cpu) +
			    part_stat_local_read_cpu(part, in_flight[1], cpu);
	}
	if ((int)inflight < 0)
		inflight = 0;

	return inflight;
}

static void part_in_flight_rw(struct block_device *part,
		unsigned int inflight[2])
{
	int cpu;

	inflight[0] = 0;
	inflight[1] = 0;
	for_each_possible_cpu(cpu) {
		inflight[0] += part_stat_local_read_cpu(part, in_flight[0], cpu);
		inflight[1] += part_stat_local_read_cpu(part, in_flight[1], cpu);
	}
	if ((int)inflight[0] < 0)
		inflight[0] = 0;
	if ((int)inflight[1] < 0)
		inflight[1] = 0;
}
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

/*
 * Can be deleted altogether. Later.
 *
 */
<<<<<<< HEAD
=======
#define BLKDEV_MAJOR_HASH_SIZE 255
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
static struct blk_major_name {
	struct blk_major_name *next;
	int major;
	char name[16];
<<<<<<< HEAD
} *major_names[BLKDEV_MAJOR_HASH_SIZE];
=======
#ifdef CONFIG_BLOCK_LEGACY_AUTOLOAD
	void (*probe)(dev_t devt);
#endif
} *major_names[BLKDEV_MAJOR_HASH_SIZE];
static DEFINE_MUTEX(major_names_lock);
static DEFINE_SPINLOCK(major_names_spinlock);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

/* index in the above - for now: assume no multimajor ranges */
static inline int major_to_index(unsigned major)
{
	return major % BLKDEV_MAJOR_HASH_SIZE;
}

#ifdef CONFIG_PROC_FS
void blkdev_show(struct seq_file *seqf, off_t offset)
{
	struct blk_major_name *dp;

<<<<<<< HEAD
	if (offset < BLKDEV_MAJOR_HASH_SIZE) {
		mutex_lock(&block_class_lock);
		for (dp = major_names[offset]; dp; dp = dp->next)
			seq_printf(seqf, "%3d %s\n", dp->major, dp->name);
		mutex_unlock(&block_class_lock);
	}
=======
	spin_lock(&major_names_spinlock);
	for (dp = major_names[major_to_index(offset)]; dp; dp = dp->next)
		if (dp->major == offset)
			seq_printf(seqf, "%3d %s\n", dp->major, dp->name);
	spin_unlock(&major_names_spinlock);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}
#endif /* CONFIG_PROC_FS */

/**
<<<<<<< HEAD
 * register_blkdev - register a new block device
 *
 * @major: the requested major device number [1..255]. If @major=0, try to
 *         allocate any unused major number.
 * @name: the name of the new block device as a zero terminated string
 *
 * The @name must be unique within the system.
 *
 * The return value depends on the @major input parameter.
 *  - if a major device number was requested in range [1..255] then the
 *    function returns zero on success, or a negative error code
 *  - if any unused major number was requested with @major=0 parameter
 *    then the return value is the allocated major number in range
 *    [1..255] or a negative error code otherwise
 */
int register_blkdev(unsigned int major, const char *name)
=======
 * __register_blkdev - register a new block device
 *
 * @major: the requested major device number [1..BLKDEV_MAJOR_MAX-1]. If
 *         @major = 0, try to allocate any unused major number.
 * @name: the name of the new block device as a zero terminated string
 * @probe: pre-devtmpfs / pre-udev callback used to create disks when their
 *	   pre-created device node is accessed. When a probe call uses
 *	   add_disk() and it fails the driver must cleanup resources. This
 *	   interface may soon be removed.
 *
 * The @name must be unique within the system.
 *
 * The return value depends on the @major input parameter:
 *
 *  - if a major device number was requested in range [1..BLKDEV_MAJOR_MAX-1]
 *    then the function returns zero on success, or a negative error code
 *  - if any unused major number was requested with @major = 0 parameter
 *    then the return value is the allocated major number in range
 *    [1..BLKDEV_MAJOR_MAX-1] or a negative error code otherwise
 *
 * See Documentation/admin-guide/devices.txt for the list of allocated
 * major numbers.
 *
 * Use register_blkdev instead for any new code.
 */
int __register_blkdev(unsigned int major, const char *name,
		void (*probe)(dev_t devt))
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
{
	struct blk_major_name **n, *p;
	int index, ret = 0;

<<<<<<< HEAD
	mutex_lock(&block_class_lock);
=======
	mutex_lock(&major_names_lock);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	/* temporary */
	if (major == 0) {
		for (index = ARRAY_SIZE(major_names)-1; index > 0; index--) {
			if (major_names[index] == NULL)
				break;
		}

		if (index == 0) {
<<<<<<< HEAD
			printk("register_blkdev: failed to get major for %s\n",
			       name);
=======
			printk("%s: failed to get major for %s\n",
			       __func__, name);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
			ret = -EBUSY;
			goto out;
		}
		major = index;
		ret = major;
	}

<<<<<<< HEAD
=======
	if (major >= BLKDEV_MAJOR_MAX) {
		pr_err("%s: major requested (%u) is greater than the maximum (%u) for %s\n",
		       __func__, major, BLKDEV_MAJOR_MAX-1, name);

		ret = -EINVAL;
		goto out;
	}

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	p = kmalloc(sizeof(struct blk_major_name), GFP_KERNEL);
	if (p == NULL) {
		ret = -ENOMEM;
		goto out;
	}

	p->major = major;
<<<<<<< HEAD
	strlcpy(p->name, name, sizeof(p->name));
	p->next = NULL;
	index = major_to_index(major);

=======
#ifdef CONFIG_BLOCK_LEGACY_AUTOLOAD
	p->probe = probe;
#endif
	strscpy(p->name, name, sizeof(p->name));
	p->next = NULL;
	index = major_to_index(major);

	spin_lock(&major_names_spinlock);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	for (n = &major_names[index]; *n; n = &(*n)->next) {
		if ((*n)->major == major)
			break;
	}
	if (!*n)
		*n = p;
	else
		ret = -EBUSY;
<<<<<<< HEAD

	if (ret < 0) {
		printk("register_blkdev: cannot get major %d for %s\n",
=======
	spin_unlock(&major_names_spinlock);

	if (ret < 0) {
		printk("register_blkdev: cannot get major %u for %s\n",
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		       major, name);
		kfree(p);
	}
out:
<<<<<<< HEAD
	mutex_unlock(&block_class_lock);
	return ret;
}

EXPORT_SYMBOL(register_blkdev);
=======
	mutex_unlock(&major_names_lock);
	return ret;
}
EXPORT_SYMBOL(__register_blkdev);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

void unregister_blkdev(unsigned int major, const char *name)
{
	struct blk_major_name **n;
	struct blk_major_name *p = NULL;
	int index = major_to_index(major);

<<<<<<< HEAD
	mutex_lock(&block_class_lock);
=======
	mutex_lock(&major_names_lock);
	spin_lock(&major_names_spinlock);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	for (n = &major_names[index]; *n; n = &(*n)->next)
		if ((*n)->major == major)
			break;
	if (!*n || strcmp((*n)->name, name)) {
		WARN_ON(1);
	} else {
		p = *n;
		*n = p->next;
	}
<<<<<<< HEAD
	mutex_unlock(&block_class_lock);
=======
	spin_unlock(&major_names_spinlock);
	mutex_unlock(&major_names_lock);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	kfree(p);
}

EXPORT_SYMBOL(unregister_blkdev);

<<<<<<< HEAD
static struct kobj_map *bdev_map;

/**
 * blk_mangle_minor - scatter minor numbers apart
 * @minor: minor number to mangle
 *
 * Scatter consecutively allocated @minor number apart if MANGLE_DEVT
 * is enabled.  Mangling twice gives the original value.
 *
 * RETURNS:
 * Mangled value.
 *
 * CONTEXT:
 * Don't care.
 */
static int blk_mangle_minor(int minor)
{
#ifdef CONFIG_DEBUG_BLOCK_EXT_DEVT
	int i;

	for (i = 0; i < MINORBITS / 2; i++) {
		int low = minor & (1 << i);
		int high = minor & (1 << (MINORBITS - 1 - i));
		int distance = MINORBITS - 1 - 2 * i;

		minor ^= low | high;	/* clear both bits */
		low <<= distance;	/* swap the positions */
		high >>= distance;
		minor |= low | high;	/* and set */
	}
#endif
	return minor;
}

/**
 * blk_alloc_devt - allocate a dev_t for a partition
 * @part: partition to allocate dev_t for
 * @devt: out parameter for resulting dev_t
 *
 * Allocate a dev_t for block device.
 *
 * RETURNS:
 * 0 on success, allocated dev_t is returned in *@devt.  -errno on
 * failure.
 *
 * CONTEXT:
 * Might sleep.
 */
int blk_alloc_devt(struct hd_struct *part, dev_t *devt)
{
	struct gendisk *disk = part_to_disk(part);
	int idx, rc;

	/* in consecutive minor range? */
	if (part->partno < disk->minors) {
		*devt = MKDEV(disk->major, disk->first_minor + part->partno);
		return 0;
	}

	/* allocate ext devt */
	do {
		if (!idr_pre_get(&ext_devt_idr, GFP_KERNEL))
			return -ENOMEM;
		spin_lock_bh(&ext_devt_lock);
		rc = idr_get_new(&ext_devt_idr, part, &idx);
		if (!rc && idx >= NR_EXT_DEVT) {
			idr_remove(&ext_devt_idr, idx);
			rc = -EBUSY;
		}
		spin_unlock_bh(&ext_devt_lock);
	} while (rc == -EAGAIN);

	if (rc)
		return rc;

	*devt = MKDEV(BLOCK_EXT_MAJOR, blk_mangle_minor(idx));
	return 0;
}

/**
 * blk_free_devt - free a dev_t
 * @devt: dev_t to free
 *
 * Free @devt which was allocated using blk_alloc_devt().
 *
 * CONTEXT:
 * Might sleep.
 */
void blk_free_devt(dev_t devt)
{
	if (devt == MKDEV(0, 0))
		return;

	if (MAJOR(devt) == BLOCK_EXT_MAJOR) {
		spin_lock_bh(&ext_devt_lock);
		idr_remove(&ext_devt_idr, blk_mangle_minor(MINOR(devt)));
		spin_unlock_bh(&ext_devt_lock);
	}
}

static char *bdevt_str(dev_t devt, char *buf)
{
	if (MAJOR(devt) <= 0xff && MINOR(devt) <= 0xff) {
		char tbuf[BDEVT_SIZE];
		snprintf(tbuf, BDEVT_SIZE, "%02x%02x", MAJOR(devt), MINOR(devt));
		snprintf(buf, BDEVT_SIZE, "%-9s", tbuf);
	} else
		snprintf(buf, BDEVT_SIZE, "%03x:%05x", MAJOR(devt), MINOR(devt));

	return buf;
}

/*
 * Register device numbers dev..(dev+range-1)
 * range must be nonzero
 * The hash chain is sorted on range, so that subranges can override.
 */
void blk_register_region(dev_t devt, unsigned long range, struct module *module,
			 struct kobject *(*probe)(dev_t, int *, void *),
			 int (*lock)(dev_t, void *), void *data)
{
	kobj_map(bdev_map, devt, range, module, probe, lock, data);
}

EXPORT_SYMBOL(blk_register_region);

void blk_unregister_region(dev_t devt, unsigned long range)
{
	kobj_unmap(bdev_map, devt, range);
}

EXPORT_SYMBOL(blk_unregister_region);

static struct kobject *exact_match(dev_t devt, int *partno, void *data)
{
	struct gendisk *p = data;

	return &disk_to_dev(p)->kobj;
}

static int exact_lock(dev_t devt, void *data)
{
	struct gendisk *p = data;

	if (!get_disk(p))
		return -1;
	return 0;
}

static void register_disk(struct gendisk *disk)
{
	struct device *ddev = disk_to_dev(disk);
	struct block_device *bdev;
	struct disk_part_iter piter;
	struct hd_struct *part;
	int err;

	ddev->parent = disk->driverfs_dev;

	dev_set_name(ddev, "%s", disk->disk_name);
=======
int blk_alloc_ext_minor(void)
{
	int idx;

	idx = ida_alloc_range(&ext_devt_ida, 0, NR_EXT_DEVT - 1, GFP_KERNEL);
	if (idx == -ENOSPC)
		return -EBUSY;
	return idx;
}

void blk_free_ext_minor(unsigned int minor)
{
	ida_free(&ext_devt_ida, minor);
}

void disk_uevent(struct gendisk *disk, enum kobject_action action)
{
	struct block_device *part;
	unsigned long idx;

	rcu_read_lock();
	xa_for_each(&disk->part_tbl, idx, part) {
		if (bdev_is_partition(part) && !bdev_nr_sectors(part))
			continue;
		if (!kobject_get_unless_zero(&part->bd_device.kobj))
			continue;

		rcu_read_unlock();
		kobject_uevent(bdev_kobj(part), action);
		put_device(&part->bd_device);
		rcu_read_lock();
	}
	rcu_read_unlock();
}
EXPORT_SYMBOL_GPL(disk_uevent);

int disk_scan_partitions(struct gendisk *disk, blk_mode_t mode)
{
	struct file *file;
	int ret = 0;

	if (disk->flags & (GENHD_FL_NO_PART | GENHD_FL_HIDDEN))
		return -EINVAL;
	if (test_bit(GD_SUPPRESS_PART_SCAN, &disk->state))
		return -EINVAL;
	if (disk->open_partitions)
		return -EBUSY;

	/*
	 * If the device is opened exclusively by current thread already, it's
	 * safe to scan partitons, otherwise, use bd_prepare_to_claim() to
	 * synchronize with other exclusive openers and other partition
	 * scanners.
	 */
	if (!(mode & BLK_OPEN_EXCL)) {
		ret = bd_prepare_to_claim(disk->part0, disk_scan_partitions,
					  NULL);
		if (ret)
			return ret;
	}

	set_bit(GD_NEED_PART_SCAN, &disk->state);
	file = bdev_file_open_by_dev(disk_devt(disk), mode & ~BLK_OPEN_EXCL,
				     NULL, NULL);
	if (IS_ERR(file))
		ret = PTR_ERR(file);
	else
		fput(file);

	/*
	 * If blkdev_get_by_dev() failed early, GD_NEED_PART_SCAN is still set,
	 * and this will cause that re-assemble partitioned raid device will
	 * creat partition for underlying disk.
	 */
	clear_bit(GD_NEED_PART_SCAN, &disk->state);
	if (!(mode & BLK_OPEN_EXCL))
		bd_abort_claiming(disk->part0, disk_scan_partitions);
	return ret;
}

/**
 * device_add_disk - add disk information to kernel list
 * @parent: parent device for the disk
 * @disk: per-device partitioning information
 * @groups: Additional per-device sysfs groups
 *
 * This function registers the partitioning information in @disk
 * with the kernel.
 */
int __must_check device_add_disk(struct device *parent, struct gendisk *disk,
				 const struct attribute_group **groups)

{
	struct device *ddev = disk_to_dev(disk);
	int ret;

	/* Only makes sense for bio-based to set ->poll_bio */
	if (queue_is_mq(disk->queue) && disk->fops->poll_bio)
		return -EINVAL;

	/*
	 * The disk queue should now be all set with enough information about
	 * the device for the elevator code to pick an adequate default
	 * elevator if one is needed, that is, for devices requesting queue
	 * registration.
	 */
	elevator_init_mq(disk->queue);

	/* Mark bdev as having a submit_bio, if needed */
	disk->part0->bd_has_submit_bio = disk->fops->submit_bio != NULL;

	/*
	 * If the driver provides an explicit major number it also must provide
	 * the number of minors numbers supported, and those will be used to
	 * setup the gendisk.
	 * Otherwise just allocate the device numbers for both the whole device
	 * and all partitions from the extended dev_t space.
	 */
	ret = -EINVAL;
	if (disk->major) {
		if (WARN_ON(!disk->minors))
			goto out_exit_elevator;

		if (disk->minors > DISK_MAX_PARTS) {
			pr_err("block: can't allocate more than %d partitions\n",
				DISK_MAX_PARTS);
			disk->minors = DISK_MAX_PARTS;
		}
		if (disk->first_minor > MINORMASK ||
		    disk->minors > MINORMASK + 1 ||
		    disk->first_minor + disk->minors > MINORMASK + 1)
			goto out_exit_elevator;
	} else {
		if (WARN_ON(disk->minors))
			goto out_exit_elevator;

		ret = blk_alloc_ext_minor();
		if (ret < 0)
			goto out_exit_elevator;
		disk->major = BLOCK_EXT_MAJOR;
		disk->first_minor = ret;
	}
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	/* delay uevents, until we scanned partition table */
	dev_set_uevent_suppress(ddev, 1);

<<<<<<< HEAD
	if (device_add(ddev))
		return;
	if (!sysfs_deprecated) {
		err = sysfs_create_link(block_depr, &ddev->kobj,
					kobject_name(&ddev->kobj));
		if (err) {
			device_del(ddev);
			return;
		}
	}
	disk->part0.holder_dir = kobject_create_and_add("holders", &ddev->kobj);
	disk->slave_dir = kobject_create_and_add("slaves", &ddev->kobj);

	/* No minors to use for partitions */
	if (!disk_part_scan_enabled(disk))
		goto exit;

	/* No such device (e.g., media were just removed) */
	if (!get_capacity(disk))
		goto exit;

	bdev = bdget_disk(disk, 0);
	if (!bdev)
		goto exit;

	bdev->bd_invalidated = 1;
	err = blkdev_get(bdev, FMODE_READ, NULL);
	if (err < 0)
		goto exit;
	blkdev_put(bdev, FMODE_READ);

exit:
	/* announce disk after possible partitions are created */
	dev_set_uevent_suppress(ddev, 0);
	kobject_uevent(&ddev->kobj, KOBJ_ADD);

	/* announce possible partitions */
	disk_part_iter_init(&piter, disk, 0);
	while ((part = disk_part_iter_next(&piter)))
		kobject_uevent(&part_to_dev(part)->kobj, KOBJ_ADD);
	disk_part_iter_exit(&piter);
}

/**
 * add_disk - add partitioning information to kernel list
 * @disk: per-device partitioning information
 *
 * This function registers the partitioning information in @disk
 * with the kernel.
 *
 * FIXME: error handling
 */
void add_disk(struct gendisk *disk)
{
	struct backing_dev_info *bdi;
	dev_t devt;
	int retval;

	/* minors == 0 indicates to use ext devt from part0 and should
	 * be accompanied with EXT_DEVT flag.  Make sure all
	 * parameters make sense.
	 */
	WARN_ON(disk->minors && !(disk->major || disk->first_minor));
	WARN_ON(!disk->minors && !(disk->flags & GENHD_FL_EXT_DEVT));

	disk->flags |= GENHD_FL_UP;

	retval = blk_alloc_devt(&disk->part0, &devt);
	if (retval) {
		WARN_ON(1);
		return;
	}
	disk_to_dev(disk)->devt = devt;

	/* ->major and ->first_minor aren't supposed to be
	 * dereferenced from here on, but set them just in case.
	 */
	disk->major = MAJOR(devt);
	disk->first_minor = MINOR(devt);

	disk_alloc_events(disk);

	/* Register BDI before referencing it from bdev */
	bdi = &disk->queue->backing_dev_info;
	bdi_register_dev(bdi, disk_devt(disk));

	blk_register_region(disk_devt(disk), disk->minors, NULL,
			    exact_match, exact_lock, disk);
	register_disk(disk);
	blk_register_queue(disk);

	/*
	 * Take an extra ref on queue which will be put on disk_release()
	 * so that it sticks around as long as @disk is there.
	 */
	WARN_ON_ONCE(!blk_get_queue(disk->queue));

	retval = sysfs_create_link(&disk_to_dev(disk)->kobj, &bdi->dev->kobj,
				   "bdi");
	WARN_ON(retval);

	disk_add_events(disk);
}
EXPORT_SYMBOL(add_disk);

void del_gendisk(struct gendisk *disk)
{
	struct disk_part_iter piter;
	struct hd_struct *part;

	disk_del_events(disk);

	/* invalidate stuff */
	disk_part_iter_init(&piter, disk,
			     DISK_PITER_INCL_EMPTY | DISK_PITER_REVERSE);
	while ((part = disk_part_iter_next(&piter))) {
		invalidate_partition(disk, part->partno);
		delete_partition(disk, part->partno);
	}
	disk_part_iter_exit(&piter);

	invalidate_partition(disk, 0);
	set_capacity(disk, 0);
	disk->flags &= ~GENHD_FL_UP;

	sysfs_remove_link(&disk_to_dev(disk)->kobj, "bdi");
	bdi_unregister(&disk->queue->backing_dev_info);
	blk_unregister_queue(disk);
	blk_unregister_region(disk_devt(disk), disk->minors);

	part_stat_set_all(&disk->part0, 0);
	disk->part0.stamp = 0;

	kobject_put(disk->part0.holder_dir);
	kobject_put(disk->slave_dir);
	disk->driverfs_dev = NULL;
	if (!sysfs_deprecated)
		sysfs_remove_link(block_depr, dev_name(disk_to_dev(disk)));
	device_del(disk_to_dev(disk));
=======
	ddev->parent = parent;
	ddev->groups = groups;
	dev_set_name(ddev, "%s", disk->disk_name);
	if (!(disk->flags & GENHD_FL_HIDDEN))
		ddev->devt = MKDEV(disk->major, disk->first_minor);
	ret = device_add(ddev);
	if (ret)
		goto out_free_ext_minor;

	ret = disk_alloc_events(disk);
	if (ret)
		goto out_device_del;

	ret = sysfs_create_link(block_depr, &ddev->kobj,
				kobject_name(&ddev->kobj));
	if (ret)
		goto out_device_del;

	/*
	 * avoid probable deadlock caused by allocating memory with
	 * GFP_KERNEL in runtime_resume callback of its all ancestor
	 * devices
	 */
	pm_runtime_set_memalloc_noio(ddev, true);

	disk->part0->bd_holder_dir =
		kobject_create_and_add("holders", &ddev->kobj);
	if (!disk->part0->bd_holder_dir) {
		ret = -ENOMEM;
		goto out_del_block_link;
	}
	disk->slave_dir = kobject_create_and_add("slaves", &ddev->kobj);
	if (!disk->slave_dir) {
		ret = -ENOMEM;
		goto out_put_holder_dir;
	}

	ret = blk_register_queue(disk);
	if (ret)
		goto out_put_slave_dir;

	if (!(disk->flags & GENHD_FL_HIDDEN)) {
		ret = bdi_register(disk->bdi, "%u:%u",
				   disk->major, disk->first_minor);
		if (ret)
			goto out_unregister_queue;
		bdi_set_owner(disk->bdi, ddev);
		ret = sysfs_create_link(&ddev->kobj,
					&disk->bdi->dev->kobj, "bdi");
		if (ret)
			goto out_unregister_bdi;

		/* Make sure the first partition scan will be proceed */
		if (get_capacity(disk) && !(disk->flags & GENHD_FL_NO_PART) &&
		    !test_bit(GD_SUPPRESS_PART_SCAN, &disk->state))
			set_bit(GD_NEED_PART_SCAN, &disk->state);

		bdev_add(disk->part0, ddev->devt);
		if (get_capacity(disk))
			disk_scan_partitions(disk, BLK_OPEN_READ);

		/*
		 * Announce the disk and partitions after all partitions are
		 * created. (for hidden disks uevents remain suppressed forever)
		 */
		dev_set_uevent_suppress(ddev, 0);
		disk_uevent(disk, KOBJ_ADD);
	} else {
		/*
		 * Even if the block_device for a hidden gendisk is not
		 * registered, it needs to have a valid bd_dev so that the
		 * freeing of the dynamic major works.
		 */
		disk->part0->bd_dev = MKDEV(disk->major, disk->first_minor);
	}

	disk_update_readahead(disk);
	disk_add_events(disk);
	set_bit(GD_ADDED, &disk->state);
	return 0;

out_unregister_bdi:
	if (!(disk->flags & GENHD_FL_HIDDEN))
		bdi_unregister(disk->bdi);
out_unregister_queue:
	blk_unregister_queue(disk);
	rq_qos_exit(disk->queue);
out_put_slave_dir:
	kobject_put(disk->slave_dir);
	disk->slave_dir = NULL;
out_put_holder_dir:
	kobject_put(disk->part0->bd_holder_dir);
out_del_block_link:
	sysfs_remove_link(block_depr, dev_name(ddev));
	pm_runtime_set_memalloc_noio(ddev, false);
out_device_del:
	device_del(ddev);
out_free_ext_minor:
	if (disk->major == BLOCK_EXT_MAJOR)
		blk_free_ext_minor(disk->first_minor);
out_exit_elevator:
	if (disk->queue->elevator)
		elevator_exit(disk->queue);
	return ret;
}
EXPORT_SYMBOL(device_add_disk);

static void blk_report_disk_dead(struct gendisk *disk, bool surprise)
{
	struct block_device *bdev;
	unsigned long idx;

	/*
	 * On surprise disk removal, bdev_mark_dead() may call into file
	 * systems below. Make it clear that we're expecting to not hold
	 * disk->open_mutex.
	 */
	lockdep_assert_not_held(&disk->open_mutex);

	rcu_read_lock();
	xa_for_each(&disk->part_tbl, idx, bdev) {
		if (!kobject_get_unless_zero(&bdev->bd_device.kobj))
			continue;
		rcu_read_unlock();

		bdev_mark_dead(bdev, surprise);

		put_device(&bdev->bd_device);
		rcu_read_lock();
	}
	rcu_read_unlock();
}

static void __blk_mark_disk_dead(struct gendisk *disk)
{
	/*
	 * Fail any new I/O.
	 */
	if (test_and_set_bit(GD_DEAD, &disk->state))
		return;

	if (test_bit(GD_OWNS_QUEUE, &disk->state))
		blk_queue_flag_set(QUEUE_FLAG_DYING, disk->queue);

	/*
	 * Stop buffered writers from dirtying pages that can't be written out.
	 */
	set_capacity(disk, 0);

	/*
	 * Prevent new I/O from crossing bio_queue_enter().
	 */
	blk_queue_start_drain(disk->queue);
}

/**
 * blk_mark_disk_dead - mark a disk as dead
 * @disk: disk to mark as dead
 *
 * Mark as disk as dead (e.g. surprise removed) and don't accept any new I/O
 * to this disk.
 */
void blk_mark_disk_dead(struct gendisk *disk)
{
	__blk_mark_disk_dead(disk);
	blk_report_disk_dead(disk, true);
}
EXPORT_SYMBOL_GPL(blk_mark_disk_dead);

/**
 * del_gendisk - remove the gendisk
 * @disk: the struct gendisk to remove
 *
 * Removes the gendisk and all its associated resources. This deletes the
 * partitions associated with the gendisk, and unregisters the associated
 * request_queue.
 *
 * This is the counter to the respective __device_add_disk() call.
 *
 * The final removal of the struct gendisk happens when its refcount reaches 0
 * with put_disk(), which should be called after del_gendisk(), if
 * __device_add_disk() was used.
 *
 * Drivers exist which depend on the release of the gendisk to be synchronous,
 * it should not be deferred.
 *
 * Context: can sleep
 */
void del_gendisk(struct gendisk *disk)
{
	struct request_queue *q = disk->queue;
	struct block_device *part;
	unsigned long idx;

	might_sleep();

	if (WARN_ON_ONCE(!disk_live(disk) && !(disk->flags & GENHD_FL_HIDDEN)))
		return;

	disk_del_events(disk);

	/*
	 * Prevent new openers by unlinked the bdev inode.
	 */
	mutex_lock(&disk->open_mutex);
	xa_for_each(&disk->part_tbl, idx, part)
		remove_inode_hash(part->bd_inode);
	mutex_unlock(&disk->open_mutex);

	/*
	 * Tell the file system to write back all dirty data and shut down if
	 * it hasn't been notified earlier.
	 */
	if (!test_bit(GD_DEAD, &disk->state))
		blk_report_disk_dead(disk, false);
	__blk_mark_disk_dead(disk);

	/*
	 * Drop all partitions now that the disk is marked dead.
	 */
	mutex_lock(&disk->open_mutex);
	xa_for_each_start(&disk->part_tbl, idx, part, 1)
		drop_partition(part);
	mutex_unlock(&disk->open_mutex);

	if (!(disk->flags & GENHD_FL_HIDDEN)) {
		sysfs_remove_link(&disk_to_dev(disk)->kobj, "bdi");

		/*
		 * Unregister bdi before releasing device numbers (as they can
		 * get reused and we'd get clashes in sysfs).
		 */
		bdi_unregister(disk->bdi);
	}

	blk_unregister_queue(disk);

	kobject_put(disk->part0->bd_holder_dir);
	kobject_put(disk->slave_dir);
	disk->slave_dir = NULL;

	part_stat_set_all(disk->part0, 0);
	disk->part0->bd_stamp = 0;
	sysfs_remove_link(block_depr, dev_name(disk_to_dev(disk)));
	pm_runtime_set_memalloc_noio(disk_to_dev(disk), false);
	device_del(disk_to_dev(disk));

	blk_mq_freeze_queue_wait(q);

	blk_throtl_cancel_bios(disk);

	blk_sync_queue(q);
	blk_flush_integrity();

	if (queue_is_mq(q))
		blk_mq_cancel_work_sync(q);

	blk_mq_quiesce_queue(q);
	if (q->elevator) {
		mutex_lock(&q->sysfs_lock);
		elevator_exit(q);
		mutex_unlock(&q->sysfs_lock);
	}
	rq_qos_exit(q);
	blk_mq_unquiesce_queue(q);

	/*
	 * If the disk does not own the queue, allow using passthrough requests
	 * again.  Else leave the queue frozen to fail all I/O.
	 */
	if (!test_bit(GD_OWNS_QUEUE, &disk->state)) {
		blk_queue_flag_clear(QUEUE_FLAG_INIT_DONE, q);
		__blk_mq_unfreeze_queue(q, true);
	} else {
		if (queue_is_mq(q))
			blk_mq_exit_queue(q);
	}
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}
EXPORT_SYMBOL(del_gendisk);

/**
<<<<<<< HEAD
 * get_gendisk - get partitioning information for a given device
 * @devt: device to get partitioning information for
 * @partno: returned partition index
 *
 * This function gets the structure containing partitioning
 * information for the given device @devt.
 */
struct gendisk *get_gendisk(dev_t devt, int *partno)
{
	struct gendisk *disk = NULL;

	if (MAJOR(devt) != BLOCK_EXT_MAJOR) {
		struct kobject *kobj;

		kobj = kobj_lookup(bdev_map, devt, partno);
		if (kobj)
			disk = dev_to_disk(kobj_to_dev(kobj));
	} else {
		struct hd_struct *part;

		spin_lock_bh(&ext_devt_lock);
		part = idr_find(&ext_devt_idr, blk_mangle_minor(MINOR(devt)));
		if (part && get_disk(part_to_disk(part))) {
			*partno = part->partno;
			disk = part_to_disk(part);
		}
		spin_unlock_bh(&ext_devt_lock);
	}

	return disk;
}
EXPORT_SYMBOL(get_gendisk);

/**
 * bdget_disk - do bdget() by gendisk and partition number
 * @disk: gendisk of interest
 * @partno: partition number
 *
 * Find partition @partno from @disk, do bdget() on it.
 *
 * CONTEXT:
 * Don't care.
 *
 * RETURNS:
 * Resulting block_device on success, NULL on failure.
 */
struct block_device *bdget_disk(struct gendisk *disk, int partno)
{
	struct hd_struct *part;
	struct block_device *bdev = NULL;

	part = disk_get_part(disk, partno);
	if (part)
		bdev = bdget(part_devt(part));
	disk_put_part(part);

	return bdev;
}
EXPORT_SYMBOL(bdget_disk);

/*
 * print a full list of all partitions - intended for places where the root
 * filesystem can't be mounted and thus to give the victim some idea of what
 * went wrong
 */
void __init printk_all_partitions(void)
{
	struct class_dev_iter iter;
	struct device *dev;

	class_dev_iter_init(&iter, &block_class, NULL, &disk_type);
	while ((dev = class_dev_iter_next(&iter))) {
		struct gendisk *disk = dev_to_disk(dev);
		struct disk_part_iter piter;
		struct hd_struct *part;
		char name_buf[BDEVNAME_SIZE];
		char devt_buf[BDEVT_SIZE];
		char uuid_buf[PARTITION_META_INFO_UUIDLTH * 2 + 5];

		/*
		 * Don't show empty devices or things that have been
		 * suppressed
		 */
		if (get_capacity(disk) == 0 ||
		    (disk->flags & GENHD_FL_SUPPRESS_PARTITION_INFO))
			continue;

		/*
		 * Note, unlike /proc/partitions, I am showing the
		 * numbers in hex - the same format as the root=
		 * option takes.
		 */
		disk_part_iter_init(&piter, disk, DISK_PITER_INCL_PART0);
		while ((part = disk_part_iter_next(&piter))) {
			bool is_part0 = part == &disk->part0;

			uuid_buf[0] = '\0';
			if (part->info)
				snprintf(uuid_buf, sizeof(uuid_buf), "%pU",
					 part->info->uuid);

			printk("%s%s %10llu %s %s", is_part0 ? "" : "  ",
			       bdevt_str(part_devt(part), devt_buf),
			       (unsigned long long)part->nr_sects >> 1,
			       disk_name(disk, part->partno, name_buf),
			       uuid_buf);
			if (is_part0) {
				if (disk->driverfs_dev != NULL &&
				    disk->driverfs_dev->driver != NULL)
					printk(" driver: %s\n",
					      disk->driverfs_dev->driver->name);
				else
					printk(" (driver?)\n");
			} else
				printk("\n");
		}
		disk_part_iter_exit(&piter);
	}
	class_dev_iter_exit(&iter);
}
=======
 * invalidate_disk - invalidate the disk
 * @disk: the struct gendisk to invalidate
 *
 * A helper to invalidates the disk. It will clean the disk's associated
 * buffer/page caches and reset its internal states so that the disk
 * can be reused by the drivers.
 *
 * Context: can sleep
 */
void invalidate_disk(struct gendisk *disk)
{
	struct block_device *bdev = disk->part0;

	invalidate_bdev(bdev);
	bdev->bd_inode->i_mapping->wb_err = 0;
	set_capacity(disk, 0);
}
EXPORT_SYMBOL(invalidate_disk);

/* sysfs access to bad-blocks list. */
static ssize_t disk_badblocks_show(struct device *dev,
					struct device_attribute *attr,
					char *page)
{
	struct gendisk *disk = dev_to_disk(dev);

	if (!disk->bb)
		return sprintf(page, "\n");

	return badblocks_show(disk->bb, page, 0);
}

static ssize_t disk_badblocks_store(struct device *dev,
					struct device_attribute *attr,
					const char *page, size_t len)
{
	struct gendisk *disk = dev_to_disk(dev);

	if (!disk->bb)
		return -ENXIO;

	return badblocks_store(disk->bb, page, len, 0);
}

#ifdef CONFIG_BLOCK_LEGACY_AUTOLOAD
void blk_request_module(dev_t devt)
{
	unsigned int major = MAJOR(devt);
	struct blk_major_name **n;

	mutex_lock(&major_names_lock);
	for (n = &major_names[major_to_index(major)]; *n; n = &(*n)->next) {
		if ((*n)->major == major && (*n)->probe) {
			(*n)->probe(devt);
			mutex_unlock(&major_names_lock);
			return;
		}
	}
	mutex_unlock(&major_names_lock);

	if (request_module("block-major-%d-%d", MAJOR(devt), MINOR(devt)) > 0)
		/* Make old-style 2.4 aliases work */
		request_module("block-major-%d", MAJOR(devt));
}
#endif /* CONFIG_BLOCK_LEGACY_AUTOLOAD */
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

#ifdef CONFIG_PROC_FS
/* iterator */
static void *disk_seqf_start(struct seq_file *seqf, loff_t *pos)
{
	loff_t skip = *pos;
	struct class_dev_iter *iter;
	struct device *dev;

	iter = kmalloc(sizeof(*iter), GFP_KERNEL);
	if (!iter)
		return ERR_PTR(-ENOMEM);

	seqf->private = iter;
	class_dev_iter_init(iter, &block_class, NULL, &disk_type);
	do {
		dev = class_dev_iter_next(iter);
		if (!dev)
			return NULL;
	} while (skip--);

	return dev_to_disk(dev);
}

static void *disk_seqf_next(struct seq_file *seqf, void *v, loff_t *pos)
{
	struct device *dev;

	(*pos)++;
	dev = class_dev_iter_next(seqf->private);
	if (dev)
		return dev_to_disk(dev);

	return NULL;
}

static void disk_seqf_stop(struct seq_file *seqf, void *v)
{
	struct class_dev_iter *iter = seqf->private;

	/* stop is called even after start failed :-( */
	if (iter) {
		class_dev_iter_exit(iter);
		kfree(iter);
		seqf->private = NULL;
	}
}

static void *show_partition_start(struct seq_file *seqf, loff_t *pos)
{
<<<<<<< HEAD
	static void *p;
=======
	void *p;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	p = disk_seqf_start(seqf, pos);
	if (!IS_ERR_OR_NULL(p) && !*pos)
		seq_puts(seqf, "major minor  #blocks  name\n\n");
	return p;
}

static int show_partition(struct seq_file *seqf, void *v)
{
	struct gendisk *sgp = v;
<<<<<<< HEAD
	struct disk_part_iter piter;
	struct hd_struct *part;
	char buf[BDEVNAME_SIZE];

	/* Don't show non-partitionable removeable devices or empty devices */
	if (!get_capacity(sgp) || (!disk_max_parts(sgp) &&
				   (sgp->flags & GENHD_FL_REMOVABLE)))
		return 0;
	if (sgp->flags & GENHD_FL_SUPPRESS_PARTITION_INFO)
		return 0;

	/* show the full disk and all non-0 size partitions of it */
	disk_part_iter_init(&piter, sgp, DISK_PITER_INCL_PART0);
	while ((part = disk_part_iter_next(&piter)))
		seq_printf(seqf, "%4d  %7d %10llu %s\n",
			   MAJOR(part_devt(part)), MINOR(part_devt(part)),
			   (unsigned long long)part->nr_sects >> 1,
			   disk_name(sgp, part->partno, buf));
	disk_part_iter_exit(&piter);

=======
	struct block_device *part;
	unsigned long idx;

	if (!get_capacity(sgp) || (sgp->flags & GENHD_FL_HIDDEN))
		return 0;

	rcu_read_lock();
	xa_for_each(&sgp->part_tbl, idx, part) {
		if (!bdev_nr_sectors(part))
			continue;
		seq_printf(seqf, "%4d  %7d %10llu %pg\n",
			   MAJOR(part->bd_dev), MINOR(part->bd_dev),
			   bdev_nr_sectors(part) >> 1, part);
	}
	rcu_read_unlock();
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	return 0;
}

static const struct seq_operations partitions_op = {
	.start	= show_partition_start,
	.next	= disk_seqf_next,
	.stop	= disk_seqf_stop,
	.show	= show_partition
};
<<<<<<< HEAD

static int partitions_open(struct inode *inode, struct file *file)
{
	return seq_open(file, &partitions_op);
}

static const struct file_operations proc_partitions_operations = {
	.open		= partitions_open,
	.read		= seq_read,
	.llseek		= seq_lseek,
	.release	= seq_release,
};
#endif


static struct kobject *base_probe(dev_t devt, int *partno, void *data)
{
	if (request_module("block-major-%d-%d", MAJOR(devt), MINOR(devt)) > 0)
		/* Make old-style 2.4 aliases work */
		request_module("block-major-%d", MAJOR(devt));
	return NULL;
}

=======
#endif

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
static int __init genhd_device_init(void)
{
	int error;

<<<<<<< HEAD
	block_class.dev_kobj = sysfs_dev_block_kobj;
	error = class_register(&block_class);
	if (unlikely(error))
		return error;
	bdev_map = kobj_map_init(base_probe, &block_class_lock);
=======
	error = class_register(&block_class);
	if (unlikely(error))
		return error;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	blk_dev_init();

	register_blkdev(BLOCK_EXT_MAJOR, "blkext");

	/* create top-level block dir */
<<<<<<< HEAD
	if (!sysfs_deprecated)
		block_depr = kobject_create_and_add("block", NULL);
=======
	block_depr = kobject_create_and_add("block", NULL);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	return 0;
}

subsys_initcall(genhd_device_init);

static ssize_t disk_range_show(struct device *dev,
			       struct device_attribute *attr, char *buf)
{
	struct gendisk *disk = dev_to_disk(dev);

	return sprintf(buf, "%d\n", disk->minors);
}

static ssize_t disk_ext_range_show(struct device *dev,
				   struct device_attribute *attr, char *buf)
{
	struct gendisk *disk = dev_to_disk(dev);

<<<<<<< HEAD
	return sprintf(buf, "%d\n", disk_max_parts(disk));
=======
	return sprintf(buf, "%d\n",
		(disk->flags & GENHD_FL_NO_PART) ? 1 : DISK_MAX_PARTS);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

static ssize_t disk_removable_show(struct device *dev,
				   struct device_attribute *attr, char *buf)
{
	struct gendisk *disk = dev_to_disk(dev);

	return sprintf(buf, "%d\n",
		       (disk->flags & GENHD_FL_REMOVABLE ? 1 : 0));
}

<<<<<<< HEAD
=======
static ssize_t disk_hidden_show(struct device *dev,
				   struct device_attribute *attr, char *buf)
{
	struct gendisk *disk = dev_to_disk(dev);

	return sprintf(buf, "%d\n",
		       (disk->flags & GENHD_FL_HIDDEN ? 1 : 0));
}

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
static ssize_t disk_ro_show(struct device *dev,
				   struct device_attribute *attr, char *buf)
{
	struct gendisk *disk = dev_to_disk(dev);

	return sprintf(buf, "%d\n", get_disk_ro(disk) ? 1 : 0);
}

<<<<<<< HEAD
static ssize_t disk_capability_show(struct device *dev,
				    struct device_attribute *attr, char *buf)
{
	struct gendisk *disk = dev_to_disk(dev);

	return sprintf(buf, "%x\n", disk->flags);
=======
ssize_t part_size_show(struct device *dev,
		       struct device_attribute *attr, char *buf)
{
	return sprintf(buf, "%llu\n", bdev_nr_sectors(dev_to_bdev(dev)));
}

ssize_t part_stat_show(struct device *dev,
		       struct device_attribute *attr, char *buf)
{
	struct block_device *bdev = dev_to_bdev(dev);
	struct request_queue *q = bdev_get_queue(bdev);
	struct disk_stats stat;
	unsigned int inflight;

	if (queue_is_mq(q))
		inflight = blk_mq_in_flight(q, bdev);
	else
		inflight = part_in_flight(bdev);

	if (inflight) {
		part_stat_lock();
		update_io_ticks(bdev, jiffies, true);
		part_stat_unlock();
	}
	part_stat_read_all(bdev, &stat);
	return sprintf(buf,
		"%8lu %8lu %8llu %8u "
		"%8lu %8lu %8llu %8u "
		"%8u %8u %8u "
		"%8lu %8lu %8llu %8u "
		"%8lu %8u"
		"\n",
		stat.ios[STAT_READ],
		stat.merges[STAT_READ],
		(unsigned long long)stat.sectors[STAT_READ],
		(unsigned int)div_u64(stat.nsecs[STAT_READ], NSEC_PER_MSEC),
		stat.ios[STAT_WRITE],
		stat.merges[STAT_WRITE],
		(unsigned long long)stat.sectors[STAT_WRITE],
		(unsigned int)div_u64(stat.nsecs[STAT_WRITE], NSEC_PER_MSEC),
		inflight,
		jiffies_to_msecs(stat.io_ticks),
		(unsigned int)div_u64(stat.nsecs[STAT_READ] +
				      stat.nsecs[STAT_WRITE] +
				      stat.nsecs[STAT_DISCARD] +
				      stat.nsecs[STAT_FLUSH],
						NSEC_PER_MSEC),
		stat.ios[STAT_DISCARD],
		stat.merges[STAT_DISCARD],
		(unsigned long long)stat.sectors[STAT_DISCARD],
		(unsigned int)div_u64(stat.nsecs[STAT_DISCARD], NSEC_PER_MSEC),
		stat.ios[STAT_FLUSH],
		(unsigned int)div_u64(stat.nsecs[STAT_FLUSH], NSEC_PER_MSEC));
}

ssize_t part_inflight_show(struct device *dev, struct device_attribute *attr,
			   char *buf)
{
	struct block_device *bdev = dev_to_bdev(dev);
	struct request_queue *q = bdev_get_queue(bdev);
	unsigned int inflight[2];

	if (queue_is_mq(q))
		blk_mq_in_flight_rw(q, bdev, inflight);
	else
		part_in_flight_rw(bdev, inflight);

	return sprintf(buf, "%8u %8u\n", inflight[0], inflight[1]);
}

static ssize_t disk_capability_show(struct device *dev,
				    struct device_attribute *attr, char *buf)
{
	dev_warn_once(dev, "the capability attribute has been deprecated.\n");
	return sprintf(buf, "0\n");
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

static ssize_t disk_alignment_offset_show(struct device *dev,
					  struct device_attribute *attr,
					  char *buf)
{
	struct gendisk *disk = dev_to_disk(dev);

<<<<<<< HEAD
	return sprintf(buf, "%d\n", queue_alignment_offset(disk->queue));
=======
	return sprintf(buf, "%d\n", bdev_alignment_offset(disk->part0));
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

static ssize_t disk_discard_alignment_show(struct device *dev,
					   struct device_attribute *attr,
					   char *buf)
{
	struct gendisk *disk = dev_to_disk(dev);

<<<<<<< HEAD
	return sprintf(buf, "%d\n", queue_discard_alignment(disk->queue));
}

static DEVICE_ATTR(range, S_IRUGO, disk_range_show, NULL);
static DEVICE_ATTR(ext_range, S_IRUGO, disk_ext_range_show, NULL);
static DEVICE_ATTR(removable, S_IRUGO, disk_removable_show, NULL);
static DEVICE_ATTR(ro, S_IRUGO, disk_ro_show, NULL);
static DEVICE_ATTR(size, S_IRUGO, part_size_show, NULL);
static DEVICE_ATTR(alignment_offset, S_IRUGO, disk_alignment_offset_show, NULL);
static DEVICE_ATTR(discard_alignment, S_IRUGO, disk_discard_alignment_show,
		   NULL);
static DEVICE_ATTR(capability, S_IRUGO, disk_capability_show, NULL);
static DEVICE_ATTR(stat, S_IRUGO, part_stat_show, NULL);
static DEVICE_ATTR(inflight, S_IRUGO, part_inflight_show, NULL);
#ifdef CONFIG_FAIL_MAKE_REQUEST
static struct device_attribute dev_attr_fail =
	__ATTR(make-it-fail, S_IRUGO|S_IWUSR, part_fail_show, part_fail_store);
#endif
#ifdef CONFIG_FAIL_IO_TIMEOUT
static struct device_attribute dev_attr_fail_timeout =
	__ATTR(io-timeout-fail,  S_IRUGO|S_IWUSR, part_timeout_show,
		part_timeout_store);
=======
	return sprintf(buf, "%d\n", bdev_alignment_offset(disk->part0));
}

static ssize_t diskseq_show(struct device *dev,
			    struct device_attribute *attr, char *buf)
{
	struct gendisk *disk = dev_to_disk(dev);

	return sprintf(buf, "%llu\n", disk->diskseq);
}

static DEVICE_ATTR(range, 0444, disk_range_show, NULL);
static DEVICE_ATTR(ext_range, 0444, disk_ext_range_show, NULL);
static DEVICE_ATTR(removable, 0444, disk_removable_show, NULL);
static DEVICE_ATTR(hidden, 0444, disk_hidden_show, NULL);
static DEVICE_ATTR(ro, 0444, disk_ro_show, NULL);
static DEVICE_ATTR(size, 0444, part_size_show, NULL);
static DEVICE_ATTR(alignment_offset, 0444, disk_alignment_offset_show, NULL);
static DEVICE_ATTR(discard_alignment, 0444, disk_discard_alignment_show, NULL);
static DEVICE_ATTR(capability, 0444, disk_capability_show, NULL);
static DEVICE_ATTR(stat, 0444, part_stat_show, NULL);
static DEVICE_ATTR(inflight, 0444, part_inflight_show, NULL);
static DEVICE_ATTR(badblocks, 0644, disk_badblocks_show, disk_badblocks_store);
static DEVICE_ATTR(diskseq, 0444, diskseq_show, NULL);

#ifdef CONFIG_FAIL_MAKE_REQUEST
ssize_t part_fail_show(struct device *dev,
		       struct device_attribute *attr, char *buf)
{
	return sprintf(buf, "%d\n", dev_to_bdev(dev)->bd_make_it_fail);
}

ssize_t part_fail_store(struct device *dev,
			struct device_attribute *attr,
			const char *buf, size_t count)
{
	int i;

	if (count > 0 && sscanf(buf, "%d", &i) > 0)
		dev_to_bdev(dev)->bd_make_it_fail = i;

	return count;
}

static struct device_attribute dev_attr_fail =
	__ATTR(make-it-fail, 0644, part_fail_show, part_fail_store);
#endif /* CONFIG_FAIL_MAKE_REQUEST */

#ifdef CONFIG_FAIL_IO_TIMEOUT
static struct device_attribute dev_attr_fail_timeout =
	__ATTR(io-timeout-fail, 0644, part_timeout_show, part_timeout_store);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#endif

static struct attribute *disk_attrs[] = {
	&dev_attr_range.attr,
	&dev_attr_ext_range.attr,
	&dev_attr_removable.attr,
<<<<<<< HEAD
=======
	&dev_attr_hidden.attr,
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	&dev_attr_ro.attr,
	&dev_attr_size.attr,
	&dev_attr_alignment_offset.attr,
	&dev_attr_discard_alignment.attr,
	&dev_attr_capability.attr,
	&dev_attr_stat.attr,
	&dev_attr_inflight.attr,
<<<<<<< HEAD
=======
	&dev_attr_badblocks.attr,
	&dev_attr_events.attr,
	&dev_attr_events_async.attr,
	&dev_attr_events_poll_msecs.attr,
	&dev_attr_diskseq.attr,
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#ifdef CONFIG_FAIL_MAKE_REQUEST
	&dev_attr_fail.attr,
#endif
#ifdef CONFIG_FAIL_IO_TIMEOUT
	&dev_attr_fail_timeout.attr,
#endif
	NULL
};

<<<<<<< HEAD
static struct attribute_group disk_attr_group = {
	.attrs = disk_attrs,
=======
static umode_t disk_visible(struct kobject *kobj, struct attribute *a, int n)
{
	struct device *dev = container_of(kobj, typeof(*dev), kobj);
	struct gendisk *disk = dev_to_disk(dev);

	if (a == &dev_attr_badblocks.attr && !disk->bb)
		return 0;
	return a->mode;
}

static struct attribute_group disk_attr_group = {
	.attrs = disk_attrs,
	.is_visible = disk_visible,
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
};

static const struct attribute_group *disk_attr_groups[] = {
	&disk_attr_group,
<<<<<<< HEAD
=======
#ifdef CONFIG_BLK_DEV_IO_TRACE
	&blk_trace_attr_group,
#endif
#ifdef CONFIG_BLK_DEV_INTEGRITY
	&blk_integrity_attr_group,
#endif
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	NULL
};

/**
<<<<<<< HEAD
 * disk_replace_part_tbl - replace disk->part_tbl in RCU-safe way
 * @disk: disk to replace part_tbl for
 * @new_ptbl: new part_tbl to install
 *
 * Replace disk->part_tbl with @new_ptbl in RCU-safe way.  The
 * original ptbl is freed using RCU callback.
 *
 * LOCKING:
 * Matching bd_mutx locked.
 */
static void disk_replace_part_tbl(struct gendisk *disk,
				  struct disk_part_tbl *new_ptbl)
{
	struct disk_part_tbl *old_ptbl = disk->part_tbl;

	rcu_assign_pointer(disk->part_tbl, new_ptbl);

	if (old_ptbl) {
		rcu_assign_pointer(old_ptbl->last_lookup, NULL);
		kfree_rcu(old_ptbl, rcu_head);
	}
}

/**
 * disk_expand_part_tbl - expand disk->part_tbl
 * @disk: disk to expand part_tbl for
 * @partno: expand such that this partno can fit in
 *
 * Expand disk->part_tbl such that @partno can fit in.  disk->part_tbl
 * uses RCU to allow unlocked dereferencing for stats and other stuff.
 *
 * LOCKING:
 * Matching bd_mutex locked, might sleep.
 *
 * RETURNS:
 * 0 on success, -errno on failure.
 */
int disk_expand_part_tbl(struct gendisk *disk, int partno)
{
	struct disk_part_tbl *old_ptbl = disk->part_tbl;
	struct disk_part_tbl *new_ptbl;
	int len = old_ptbl ? old_ptbl->len : 0;
	int i, target;
	size_t size;

	/*
	 * check for int overflow, since we can get here from blkpg_ioctl()
	 * with a user passed 'partno'.
	 */
	target = partno + 1;
	if (target < 0)
		return -EINVAL;

	/* disk_max_parts() is zero during initialization, ignore if so */
	if (disk_max_parts(disk) && target > disk_max_parts(disk))
		return -EINVAL;

	if (target <= len)
		return 0;

	size = sizeof(*new_ptbl) + target * sizeof(new_ptbl->part[0]);
	new_ptbl = kzalloc_node(size, GFP_KERNEL, disk->node_id);
	if (!new_ptbl)
		return -ENOMEM;

	new_ptbl->len = target;

	for (i = 0; i < len; i++)
		rcu_assign_pointer(new_ptbl->part[i], old_ptbl->part[i]);

	disk_replace_part_tbl(disk, new_ptbl);
	return 0;
}

=======
 * disk_release - releases all allocated resources of the gendisk
 * @dev: the device representing this disk
 *
 * This function releases all allocated resources of the gendisk.
 *
 * Drivers which used __device_add_disk() have a gendisk with a request_queue
 * assigned. Since the request_queue sits on top of the gendisk for these
 * drivers we also call blk_put_queue() for them, and we expect the
 * request_queue refcount to reach 0 at this point, and so the request_queue
 * will also be freed prior to the disk.
 *
 * Context: can sleep
 */
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
static void disk_release(struct device *dev)
{
	struct gendisk *disk = dev_to_disk(dev);

<<<<<<< HEAD
	blk_free_devt(dev->devt);
	disk_release_events(disk);
	kfree(disk->random);
	disk_replace_part_tbl(disk, NULL);
	free_part_stats(&disk->part0);
	free_part_info(&disk->part0);
	if (disk->queue)
		blk_put_queue(disk->queue);
	kfree(disk);
}

static int disk_uevent(struct device *dev, struct kobj_uevent_env *env)
{
	struct gendisk *disk = dev_to_disk(dev);
	struct disk_part_iter piter;
	struct hd_struct *part;
	int cnt = 0;

	disk_part_iter_init(&piter, disk, 0);
	while((part = disk_part_iter_next(&piter)))
		cnt++;
	disk_part_iter_exit(&piter);
	add_uevent_var(env, "NPARTS=%u", cnt);
	return 0;
}

struct class block_class = {
	.name		= "block",
};

static char *block_devnode(struct device *dev, umode_t *mode)
{
	struct gendisk *disk = dev_to_disk(dev);

	if (disk->devnode)
		return disk->devnode(disk, mode);
	return NULL;
}

static struct device_type disk_type = {
=======
	might_sleep();
	WARN_ON_ONCE(disk_live(disk));

	blk_trace_remove(disk->queue);

	/*
	 * To undo the all initialization from blk_mq_init_allocated_queue in
	 * case of a probe failure where add_disk is never called we have to
	 * call blk_mq_exit_queue here. We can't do this for the more common
	 * teardown case (yet) as the tagset can be gone by the time the disk
	 * is released once it was added.
	 */
	if (queue_is_mq(disk->queue) &&
	    test_bit(GD_OWNS_QUEUE, &disk->state) &&
	    !test_bit(GD_ADDED, &disk->state))
		blk_mq_exit_queue(disk->queue);

	blkcg_exit_disk(disk);

	bioset_exit(&disk->bio_split);

	disk_release_events(disk);
	kfree(disk->random);
	disk_free_zone_bitmaps(disk);
	xa_destroy(&disk->part_tbl);

	disk->queue->disk = NULL;
	blk_put_queue(disk->queue);

	if (test_bit(GD_ADDED, &disk->state) && disk->fops->free_disk)
		disk->fops->free_disk(disk);

	iput(disk->part0->bd_inode);	/* frees the disk */
}

static int block_uevent(const struct device *dev, struct kobj_uevent_env *env)
{
	const struct gendisk *disk = dev_to_disk(dev);

	return add_uevent_var(env, "DISKSEQ=%llu", disk->diskseq);
}

const struct class block_class = {
	.name		= "block",
	.dev_uevent	= block_uevent,
};

static char *block_devnode(const struct device *dev, umode_t *mode,
			   kuid_t *uid, kgid_t *gid)
{
	struct gendisk *disk = dev_to_disk(dev);

	if (disk->fops->devnode)
		return disk->fops->devnode(disk, mode);
	return NULL;
}

const struct device_type disk_type = {
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	.name		= "disk",
	.groups		= disk_attr_groups,
	.release	= disk_release,
	.devnode	= block_devnode,
<<<<<<< HEAD
	.uevent		= disk_uevent,
=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
};

#ifdef CONFIG_PROC_FS
/*
 * aggregate disk stat collector.  Uses the same stats that the sysfs
 * entries do, above, but makes them available through one seq_file.
 *
 * The output looks suspiciously like /proc/partitions with a bunch of
 * extra fields.
 */
static int diskstats_show(struct seq_file *seqf, void *v)
{
	struct gendisk *gp = v;
<<<<<<< HEAD
	struct disk_part_iter piter;
	struct hd_struct *hd;
	char buf[BDEVNAME_SIZE];
	int cpu;
=======
	struct block_device *hd;
	unsigned int inflight;
	struct disk_stats stat;
	unsigned long idx;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	/*
	if (&disk_to_dev(gp)->kobj.entry == block_class.devices.next)
		seq_puts(seqf,	"major minor name"
				"     rio rmerge rsect ruse wio wmerge "
				"wsect wuse running use aveq"
				"\n\n");
	*/

<<<<<<< HEAD
	disk_part_iter_init(&piter, gp, DISK_PITER_INCL_EMPTY_PART0);
	while ((hd = disk_part_iter_next(&piter))) {
		cpu = part_stat_lock();
		part_round_stats(cpu, hd);
		part_stat_unlock();
		seq_printf(seqf, "%4d %7d %s %lu %lu %lu "
			   "%u %lu %lu %lu %u %u %u %u\n",
			   MAJOR(part_devt(hd)), MINOR(part_devt(hd)),
			   disk_name(gp, hd->partno, buf),
			   part_stat_read(hd, ios[READ]),
			   part_stat_read(hd, merges[READ]),
			   part_stat_read(hd, sectors[READ]),
			   jiffies_to_msecs(part_stat_read(hd, ticks[READ])),
			   part_stat_read(hd, ios[WRITE]),
			   part_stat_read(hd, merges[WRITE]),
			   part_stat_read(hd, sectors[WRITE]),
			   jiffies_to_msecs(part_stat_read(hd, ticks[WRITE])),
			   part_in_flight(hd),
			   jiffies_to_msecs(part_stat_read(hd, io_ticks)),
			   jiffies_to_msecs(part_stat_read(hd, time_in_queue))
			);
	}
	disk_part_iter_exit(&piter);
=======
	rcu_read_lock();
	xa_for_each(&gp->part_tbl, idx, hd) {
		if (bdev_is_partition(hd) && !bdev_nr_sectors(hd))
			continue;
		if (queue_is_mq(gp->queue))
			inflight = blk_mq_in_flight(gp->queue, hd);
		else
			inflight = part_in_flight(hd);

		if (inflight) {
			part_stat_lock();
			update_io_ticks(hd, jiffies, true);
			part_stat_unlock();
		}
		part_stat_read_all(hd, &stat);
		seq_printf(seqf, "%4d %7d %pg "
			   "%lu %lu %lu %u "
			   "%lu %lu %lu %u "
			   "%u %u %u "
			   "%lu %lu %lu %u "
			   "%lu %u"
			   "\n",
			   MAJOR(hd->bd_dev), MINOR(hd->bd_dev), hd,
			   stat.ios[STAT_READ],
			   stat.merges[STAT_READ],
			   stat.sectors[STAT_READ],
			   (unsigned int)div_u64(stat.nsecs[STAT_READ],
							NSEC_PER_MSEC),
			   stat.ios[STAT_WRITE],
			   stat.merges[STAT_WRITE],
			   stat.sectors[STAT_WRITE],
			   (unsigned int)div_u64(stat.nsecs[STAT_WRITE],
							NSEC_PER_MSEC),
			   inflight,
			   jiffies_to_msecs(stat.io_ticks),
			   (unsigned int)div_u64(stat.nsecs[STAT_READ] +
						 stat.nsecs[STAT_WRITE] +
						 stat.nsecs[STAT_DISCARD] +
						 stat.nsecs[STAT_FLUSH],
							NSEC_PER_MSEC),
			   stat.ios[STAT_DISCARD],
			   stat.merges[STAT_DISCARD],
			   stat.sectors[STAT_DISCARD],
			   (unsigned int)div_u64(stat.nsecs[STAT_DISCARD],
						 NSEC_PER_MSEC),
			   stat.ios[STAT_FLUSH],
			   (unsigned int)div_u64(stat.nsecs[STAT_FLUSH],
						 NSEC_PER_MSEC)
			);
	}
	rcu_read_unlock();
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	return 0;
}

static const struct seq_operations diskstats_op = {
	.start	= disk_seqf_start,
	.next	= disk_seqf_next,
	.stop	= disk_seqf_stop,
	.show	= diskstats_show
};

<<<<<<< HEAD
static int diskstats_open(struct inode *inode, struct file *file)
{
	return seq_open(file, &diskstats_op);
}

static const struct file_operations proc_diskstats_operations = {
	.open		= diskstats_open,
	.read		= seq_read,
	.llseek		= seq_lseek,
	.release	= seq_release,
};

static int __init proc_genhd_init(void)
{
	proc_create("diskstats", 0, NULL, &proc_diskstats_operations);
	proc_create("partitions", 0, NULL, &proc_partitions_operations);
=======
static int __init proc_genhd_init(void)
{
	proc_create_seq("diskstats", 0, NULL, &diskstats_op);
	proc_create_seq("partitions", 0, NULL, &partitions_op);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	return 0;
}
module_init(proc_genhd_init);
#endif /* CONFIG_PROC_FS */

<<<<<<< HEAD
dev_t blk_lookup_devt(const char *name, int partno)
{
	dev_t devt = MKDEV(0, 0);
	struct class_dev_iter iter;
	struct device *dev;

	class_dev_iter_init(&iter, &block_class, NULL, &disk_type);
	while ((dev = class_dev_iter_next(&iter))) {
		struct gendisk *disk = dev_to_disk(dev);
		struct hd_struct *part;

		if (strcmp(dev_name(dev), name))
			continue;

		if (partno < disk->minors) {
			/* We need to return the right devno, even
			 * if the partition doesn't exist yet.
			 */
			devt = MKDEV(MAJOR(dev->devt),
				     MINOR(dev->devt) + partno);
			break;
		}
		part = disk_get_part(disk, partno);
		if (part) {
			devt = part_devt(part);
			disk_put_part(part);
			break;
		}
		disk_put_part(part);
	}
	class_dev_iter_exit(&iter);
	return devt;
}
EXPORT_SYMBOL(blk_lookup_devt);

struct gendisk *alloc_disk(int minors)
{
	return alloc_disk_node(minors, -1);
}
EXPORT_SYMBOL(alloc_disk);

struct gendisk *alloc_disk_node(int minors, int node_id)
{
	struct gendisk *disk;

	disk = kmalloc_node(sizeof(struct gendisk),
				GFP_KERNEL | __GFP_ZERO, node_id);
	if (disk) {
		if (!init_part_stats(&disk->part0)) {
			kfree(disk);
			return NULL;
		}
		disk->node_id = node_id;
		if (disk_expand_part_tbl(disk, 0)) {
			free_part_stats(&disk->part0);
			kfree(disk);
			return NULL;
		}
		disk->part_tbl->part[0] = &disk->part0;

		hd_ref_init(&disk->part0);

		disk->minors = minors;
		rand_initialize_disk(disk);
		disk_to_dev(disk)->class = &block_class;
		disk_to_dev(disk)->type = &disk_type;
		device_initialize(disk_to_dev(disk));
	}
	return disk;
}
EXPORT_SYMBOL(alloc_disk_node);

struct kobject *get_disk(struct gendisk *disk)
{
	struct module *owner;
	struct kobject *kobj;

	if (!disk->fops)
		return NULL;
	owner = disk->fops->owner;
	if (owner && !try_module_get(owner))
		return NULL;
	kobj = kobject_get(&disk_to_dev(disk)->kobj);
	if (kobj == NULL) {
		module_put(owner);
		return NULL;
	}
	return kobj;

}

EXPORT_SYMBOL(get_disk);

void put_disk(struct gendisk *disk)
{
	if (disk)
		kobject_put(&disk_to_dev(disk)->kobj);
}

=======
dev_t part_devt(struct gendisk *disk, u8 partno)
{
	struct block_device *part;
	dev_t devt = 0;

	rcu_read_lock();
	part = xa_load(&disk->part_tbl, partno);
	if (part)
		devt = part->bd_dev;
	rcu_read_unlock();

	return devt;
}

struct gendisk *__alloc_disk_node(struct request_queue *q, int node_id,
		struct lock_class_key *lkclass)
{
	struct gendisk *disk;

	disk = kzalloc_node(sizeof(struct gendisk), GFP_KERNEL, node_id);
	if (!disk)
		return NULL;

	if (bioset_init(&disk->bio_split, BIO_POOL_SIZE, 0, 0))
		goto out_free_disk;

	disk->bdi = bdi_alloc(node_id);
	if (!disk->bdi)
		goto out_free_bioset;

	/* bdev_alloc() might need the queue, set before the first call */
	disk->queue = q;

	disk->part0 = bdev_alloc(disk, 0);
	if (!disk->part0)
		goto out_free_bdi;

	disk->node_id = node_id;
	mutex_init(&disk->open_mutex);
	xa_init(&disk->part_tbl);
	if (xa_insert(&disk->part_tbl, 0, disk->part0, GFP_KERNEL))
		goto out_destroy_part_tbl;

	if (blkcg_init_disk(disk))
		goto out_erase_part0;

	rand_initialize_disk(disk);
	disk_to_dev(disk)->class = &block_class;
	disk_to_dev(disk)->type = &disk_type;
	device_initialize(disk_to_dev(disk));
	inc_diskseq(disk);
	q->disk = disk;
	lockdep_init_map(&disk->lockdep_map, "(bio completion)", lkclass, 0);
#ifdef CONFIG_BLOCK_HOLDER_DEPRECATED
	INIT_LIST_HEAD(&disk->slave_bdevs);
#endif
	return disk;

out_erase_part0:
	xa_erase(&disk->part_tbl, 0);
out_destroy_part_tbl:
	xa_destroy(&disk->part_tbl);
	disk->part0->bd_disk = NULL;
	iput(disk->part0->bd_inode);
out_free_bdi:
	bdi_put(disk->bdi);
out_free_bioset:
	bioset_exit(&disk->bio_split);
out_free_disk:
	kfree(disk);
	return NULL;
}

struct gendisk *__blk_alloc_disk(struct queue_limits *lim, int node,
		struct lock_class_key *lkclass)
{
	struct queue_limits default_lim = { };
	struct request_queue *q;
	struct gendisk *disk;

	q = blk_alloc_queue(lim ? lim : &default_lim, node);
	if (IS_ERR(q))
		return ERR_CAST(q);

	disk = __alloc_disk_node(q, node, lkclass);
	if (!disk) {
		blk_put_queue(q);
		return ERR_PTR(-ENOMEM);
	}
	set_bit(GD_OWNS_QUEUE, &disk->state);
	return disk;
}
EXPORT_SYMBOL(__blk_alloc_disk);

/**
 * put_disk - decrements the gendisk refcount
 * @disk: the struct gendisk to decrement the refcount for
 *
 * This decrements the refcount for the struct gendisk. When this reaches 0
 * we'll have disk_release() called.
 *
 * Note: for blk-mq disk put_disk must be called before freeing the tag_set
 * when handling probe errors (that is before add_disk() is called).
 *
 * Context: Any context, but the last reference must not be dropped from
 *          atomic context.
 */
void put_disk(struct gendisk *disk)
{
	if (disk)
		put_device(disk_to_dev(disk));
}
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
EXPORT_SYMBOL(put_disk);

static void set_disk_ro_uevent(struct gendisk *gd, int ro)
{
	char event[] = "DISK_RO=1";
	char *envp[] = { event, NULL };

	if (!ro)
		event[8] = '0';
	kobject_uevent_env(&disk_to_dev(gd)->kobj, KOBJ_CHANGE, envp);
}

<<<<<<< HEAD
void set_device_ro(struct block_device *bdev, int flag)
{
	bdev->bd_part->policy = flag;
}

EXPORT_SYMBOL(set_device_ro);

void set_disk_ro(struct gendisk *disk, int flag)
{
	struct disk_part_iter piter;
	struct hd_struct *part;

	if (disk->part0.policy != flag) {
		set_disk_ro_uevent(disk, flag);
		disk->part0.policy = flag;
	}

	disk_part_iter_init(&piter, disk, DISK_PITER_INCL_EMPTY);
	while ((part = disk_part_iter_next(&piter)))
		part->policy = flag;
	disk_part_iter_exit(&piter);
}

EXPORT_SYMBOL(set_disk_ro);

int bdev_read_only(struct block_device *bdev)
{
	if (!bdev)
		return 0;
	return bdev->bd_part->policy;
}

EXPORT_SYMBOL(bdev_read_only);

int invalidate_partition(struct gendisk *disk, int partno)
{
	int res = 0;
	struct block_device *bdev = bdget_disk(disk, partno);
	if (bdev) {
		fsync_bdev(bdev);
		res = __invalidate_device(bdev, true);
		bdput(bdev);
	}
	return res;
}

EXPORT_SYMBOL(invalidate_partition);

/*
 * Disk events - monitor disk events like media change and eject request.
 */
struct disk_events {
	struct list_head	node;		/* all disk_event's */
	struct gendisk		*disk;		/* the associated disk */
	spinlock_t		lock;

	struct mutex		block_mutex;	/* protects blocking */
	int			block;		/* event blocking depth */
	unsigned int		pending;	/* events already sent out */
	unsigned int		clearing;	/* events being cleared */

	long			poll_msecs;	/* interval, -1 for default */
	struct delayed_work	dwork;
};

static const char *disk_events_strs[] = {
	[ilog2(DISK_EVENT_MEDIA_CHANGE)]	= "media_change",
	[ilog2(DISK_EVENT_EJECT_REQUEST)]	= "eject_request",
};

static char *disk_uevents[] = {
	[ilog2(DISK_EVENT_MEDIA_CHANGE)]	= "DISK_MEDIA_CHANGE=1",
	[ilog2(DISK_EVENT_EJECT_REQUEST)]	= "DISK_EJECT_REQUEST=1",
};

/* list of all disk_events */
static DEFINE_MUTEX(disk_events_mutex);
static LIST_HEAD(disk_events);

/* disable in-kernel polling by default */
static unsigned long disk_events_dfl_poll_msecs	= 0;

static unsigned long disk_events_poll_jiffies(struct gendisk *disk)
{
	struct disk_events *ev = disk->ev;
	long intv_msecs = 0;

	/*
	 * If device-specific poll interval is set, always use it.  If
	 * the default is being used, poll iff there are events which
	 * can't be monitored asynchronously.
	 */
	if (ev->poll_msecs >= 0)
		intv_msecs = ev->poll_msecs;
	else if (disk->events & ~disk->async_events)
		intv_msecs = disk_events_dfl_poll_msecs;

	return msecs_to_jiffies(intv_msecs);
}

/**
 * disk_block_events - block and flush disk event checking
 * @disk: disk to block events for
 *
 * On return from this function, it is guaranteed that event checking
 * isn't in progress and won't happen until unblocked by
 * disk_unblock_events().  Events blocking is counted and the actual
 * unblocking happens after the matching number of unblocks are done.
 *
 * Note that this intentionally does not block event checking from
 * disk_clear_events().
 *
 * CONTEXT:
 * Might sleep.
 */
void disk_block_events(struct gendisk *disk)
{
	struct disk_events *ev = disk->ev;
	unsigned long flags;
	bool cancel;

	if (!ev)
		return;

	/*
	 * Outer mutex ensures that the first blocker completes canceling
	 * the event work before further blockers are allowed to finish.
	 */
	mutex_lock(&ev->block_mutex);

	spin_lock_irqsave(&ev->lock, flags);
	cancel = !ev->block++;
	spin_unlock_irqrestore(&ev->lock, flags);

	if (cancel)
		cancel_delayed_work_sync(&disk->ev->dwork);

	mutex_unlock(&ev->block_mutex);
}

static void __disk_unblock_events(struct gendisk *disk, bool check_now)
{
	struct disk_events *ev = disk->ev;
	unsigned long intv;
	unsigned long flags;

	spin_lock_irqsave(&ev->lock, flags);

	if (WARN_ON_ONCE(ev->block <= 0))
		goto out_unlock;

	if (--ev->block)
		goto out_unlock;

	/*
	 * Not exactly a latency critical operation, set poll timer
	 * slack to 25% and kick event check.
	 */
	intv = disk_events_poll_jiffies(disk);
	set_timer_slack(&ev->dwork.timer, intv / 4);
	if (check_now)
		queue_delayed_work(system_nrt_freezable_wq, &ev->dwork, 0);
	else if (intv)
		queue_delayed_work(system_nrt_freezable_wq, &ev->dwork, intv);
out_unlock:
	spin_unlock_irqrestore(&ev->lock, flags);
}

/**
 * disk_unblock_events - unblock disk event checking
 * @disk: disk to unblock events for
 *
 * Undo disk_block_events().  When the block count reaches zero, it
 * starts events polling if configured.
 *
 * CONTEXT:
 * Don't care.  Safe to call from irq context.
 */
void disk_unblock_events(struct gendisk *disk)
{
	if (disk->ev)
		__disk_unblock_events(disk, false);
}

/**
 * disk_flush_events - schedule immediate event checking and flushing
 * @disk: disk to check and flush events for
 * @mask: events to flush
 *
 * Schedule immediate event checking on @disk if not blocked.  Events in
 * @mask are scheduled to be cleared from the driver.  Note that this
 * doesn't clear the events from @disk->ev.
 *
 * CONTEXT:
 * If @mask is non-zero must be called with bdev->bd_mutex held.
 */
void disk_flush_events(struct gendisk *disk, unsigned int mask)
{
	struct disk_events *ev = disk->ev;

	if (!ev)
		return;

	spin_lock_irq(&ev->lock);
	ev->clearing |= mask;
	if (!ev->block) {
		cancel_delayed_work(&ev->dwork);
		queue_delayed_work(system_nrt_freezable_wq, &ev->dwork, 0);
	}
	spin_unlock_irq(&ev->lock);
}

/**
 * disk_clear_events - synchronously check, clear and return pending events
 * @disk: disk to fetch and clear events from
 * @mask: mask of events to be fetched and clearted
 *
 * Disk events are synchronously checked and pending events in @mask
 * are cleared and returned.  This ignores the block count.
 *
 * CONTEXT:
 * Might sleep.
 */
unsigned int disk_clear_events(struct gendisk *disk, unsigned int mask)
{
	const struct block_device_operations *bdops = disk->fops;
	struct disk_events *ev = disk->ev;
	unsigned int pending;

	if (!ev) {
		/* for drivers still using the old ->media_changed method */
		if ((mask & DISK_EVENT_MEDIA_CHANGE) &&
		    bdops->media_changed && bdops->media_changed(disk))
			return DISK_EVENT_MEDIA_CHANGE;
		return 0;
	}

	/* tell the workfn about the events being cleared */
	spin_lock_irq(&ev->lock);
	ev->clearing |= mask;
	spin_unlock_irq(&ev->lock);

	/* uncondtionally schedule event check and wait for it to finish */
	disk_block_events(disk);
	queue_delayed_work(system_nrt_freezable_wq, &ev->dwork, 0);
	flush_delayed_work(&ev->dwork);
	__disk_unblock_events(disk, false);

	/* then, fetch and clear pending events */
	spin_lock_irq(&ev->lock);
	WARN_ON_ONCE(ev->clearing & mask);	/* cleared by workfn */
	pending = ev->pending & mask;
	ev->pending &= ~mask;
	spin_unlock_irq(&ev->lock);

	return pending;
}

static void disk_events_workfn(struct work_struct *work)
{
	struct delayed_work *dwork = to_delayed_work(work);
	struct disk_events *ev = container_of(dwork, struct disk_events, dwork);
	struct gendisk *disk = ev->disk;
	char *envp[ARRAY_SIZE(disk_uevents) + 1] = { };
	unsigned int clearing = ev->clearing;
	unsigned int events;
	unsigned long intv;
	int nr_events = 0, i;

	/* check events */
	events = disk->fops->check_events(disk, clearing);

	/* accumulate pending events and schedule next poll if necessary */
	spin_lock_irq(&ev->lock);

	events &= ~ev->pending;
	ev->pending |= events;
	ev->clearing &= ~clearing;

	intv = disk_events_poll_jiffies(disk);
	if (!ev->block && intv)
		queue_delayed_work(system_nrt_freezable_wq, &ev->dwork, intv);

	spin_unlock_irq(&ev->lock);

	/*
	 * Tell userland about new events.  Only the events listed in
	 * @disk->events are reported.  Unlisted events are processed the
	 * same internally but never get reported to userland.
	 */
	for (i = 0; i < ARRAY_SIZE(disk_uevents); i++)
		if (events & disk->events & (1 << i))
			envp[nr_events++] = disk_uevents[i];

	if (nr_events)
		kobject_uevent_env(&disk_to_dev(disk)->kobj, KOBJ_CHANGE, envp);
}

/*
 * A disk events enabled device has the following sysfs nodes under
 * its /sys/block/X/ directory.
 *
 * events		: list of all supported events
 * events_async		: list of events which can be detected w/o polling
 * events_poll_msecs	: polling interval, 0: disable, -1: system default
 */
static ssize_t __disk_events_show(unsigned int events, char *buf)
{
	const char *delim = "";
	ssize_t pos = 0;
	int i;

	for (i = 0; i < ARRAY_SIZE(disk_events_strs); i++)
		if (events & (1 << i)) {
			pos += sprintf(buf + pos, "%s%s",
				       delim, disk_events_strs[i]);
			delim = " ";
		}
	if (pos)
		pos += sprintf(buf + pos, "\n");
	return pos;
}

static ssize_t disk_events_show(struct device *dev,
				struct device_attribute *attr, char *buf)
{
	struct gendisk *disk = dev_to_disk(dev);

	return __disk_events_show(disk->events, buf);
}

static ssize_t disk_events_async_show(struct device *dev,
				      struct device_attribute *attr, char *buf)
{
	struct gendisk *disk = dev_to_disk(dev);

	return __disk_events_show(disk->async_events, buf);
}

static ssize_t disk_events_poll_msecs_show(struct device *dev,
					   struct device_attribute *attr,
					   char *buf)
{
	struct gendisk *disk = dev_to_disk(dev);

	return sprintf(buf, "%ld\n", disk->ev->poll_msecs);
}

static ssize_t disk_events_poll_msecs_store(struct device *dev,
					    struct device_attribute *attr,
					    const char *buf, size_t count)
{
	struct gendisk *disk = dev_to_disk(dev);
	long intv;

	if (!count || !sscanf(buf, "%ld", &intv))
		return -EINVAL;

	if (intv < 0 && intv != -1)
		return -EINVAL;

	disk_block_events(disk);
	disk->ev->poll_msecs = intv;
	__disk_unblock_events(disk, true);

	return count;
}

static const DEVICE_ATTR(events, S_IRUGO, disk_events_show, NULL);
static const DEVICE_ATTR(events_async, S_IRUGO, disk_events_async_show, NULL);
static const DEVICE_ATTR(events_poll_msecs, S_IRUGO|S_IWUSR,
			 disk_events_poll_msecs_show,
			 disk_events_poll_msecs_store);

static const struct attribute *disk_events_attrs[] = {
	&dev_attr_events.attr,
	&dev_attr_events_async.attr,
	&dev_attr_events_poll_msecs.attr,
	NULL,
};

/*
 * The default polling interval can be specified by the kernel
 * parameter block.events_dfl_poll_msecs which defaults to 0
 * (disable).  This can also be modified runtime by writing to
 * /sys/module/block/events_dfl_poll_msecs.
 */
static int disk_events_set_dfl_poll_msecs(const char *val,
					  const struct kernel_param *kp)
{
	struct disk_events *ev;
	int ret;

	ret = param_set_ulong(val, kp);
	if (ret < 0)
		return ret;

	mutex_lock(&disk_events_mutex);

	list_for_each_entry(ev, &disk_events, node)
		disk_flush_events(ev->disk, 0);

	mutex_unlock(&disk_events_mutex);

	return 0;
}

static const struct kernel_param_ops disk_events_dfl_poll_msecs_param_ops = {
	.set	= disk_events_set_dfl_poll_msecs,
	.get	= param_get_ulong,
};

#undef MODULE_PARAM_PREFIX
#define MODULE_PARAM_PREFIX	"block."

module_param_cb(events_dfl_poll_msecs, &disk_events_dfl_poll_msecs_param_ops,
		&disk_events_dfl_poll_msecs, 0644);

/*
 * disk_{alloc|add|del|release}_events - initialize and destroy disk_events.
 */
static void disk_alloc_events(struct gendisk *disk)
{
	struct disk_events *ev;

	if (!disk->fops->check_events)
		return;

	ev = kzalloc(sizeof(*ev), GFP_KERNEL);
	if (!ev) {
		pr_warn("%s: failed to initialize events\n", disk->disk_name);
		return;
	}

	INIT_LIST_HEAD(&ev->node);
	ev->disk = disk;
	spin_lock_init(&ev->lock);
	mutex_init(&ev->block_mutex);
	ev->block = 1;
	ev->poll_msecs = -1;
	INIT_DELAYED_WORK(&ev->dwork, disk_events_workfn);

	disk->ev = ev;
}

static void disk_add_events(struct gendisk *disk)
{
	if (!disk->ev)
		return;

	/* FIXME: error handling */
	if (sysfs_create_files(&disk_to_dev(disk)->kobj, disk_events_attrs) < 0)
		pr_warn("%s: failed to create sysfs files for events\n",
			disk->disk_name);

	mutex_lock(&disk_events_mutex);
	list_add_tail(&disk->ev->node, &disk_events);
	mutex_unlock(&disk_events_mutex);

	/*
	 * Block count is initialized to 1 and the following initial
	 * unblock kicks it into action.
	 */
	__disk_unblock_events(disk, true);
}

static void disk_del_events(struct gendisk *disk)
{
	if (!disk->ev)
		return;

	disk_block_events(disk);

	mutex_lock(&disk_events_mutex);
	list_del_init(&disk->ev->node);
	mutex_unlock(&disk_events_mutex);

	sysfs_remove_files(&disk_to_dev(disk)->kobj, disk_events_attrs);
}

static void disk_release_events(struct gendisk *disk)
{
	/* the block count should be 1 from disk_del_events() */
	WARN_ON_ONCE(disk->ev && disk->ev->block != 1);
	kfree(disk->ev);
=======
/**
 * set_disk_ro - set a gendisk read-only
 * @disk:	gendisk to operate on
 * @read_only:	%true to set the disk read-only, %false set the disk read/write
 *
 * This function is used to indicate whether a given disk device should have its
 * read-only flag set. set_disk_ro() is typically used by device drivers to
 * indicate whether the underlying physical device is write-protected.
 */
void set_disk_ro(struct gendisk *disk, bool read_only)
{
	if (read_only) {
		if (test_and_set_bit(GD_READ_ONLY, &disk->state))
			return;
	} else {
		if (!test_and_clear_bit(GD_READ_ONLY, &disk->state))
			return;
	}
	set_disk_ro_uevent(disk, read_only);
}
EXPORT_SYMBOL(set_disk_ro);

void inc_diskseq(struct gendisk *disk)
{
	disk->diskseq = atomic64_inc_return(&diskseq);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}
