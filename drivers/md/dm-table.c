<<<<<<< HEAD
=======
// SPDX-License-Identifier: GPL-2.0-only
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
/*
 * Copyright (C) 2001 Sistina Software (UK) Limited.
 * Copyright (C) 2004-2008 Red Hat, Inc. All rights reserved.
 *
 * This file is released under the GPL.
 */

<<<<<<< HEAD
#include "dm.h"
=======
#include "dm-core.h"
#include "dm-rq.h"
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

#include <linux/module.h>
#include <linux/vmalloc.h>
#include <linux/blkdev.h>
<<<<<<< HEAD
=======
#include <linux/blk-integrity.h>
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#include <linux/namei.h>
#include <linux/ctype.h>
#include <linux/string.h>
#include <linux/slab.h>
#include <linux/interrupt.h>
#include <linux/mutex.h>
#include <linux/delay.h>
#include <linux/atomic.h>
<<<<<<< HEAD

#define DM_MSG_PREFIX "table"

#define MAX_DEPTH 16
=======
#include <linux/blk-mq.h>
#include <linux/mount.h>
#include <linux/dax.h>

#define DM_MSG_PREFIX "table"

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#define NODE_SIZE L1_CACHE_BYTES
#define KEYS_PER_NODE (NODE_SIZE / sizeof(sector_t))
#define CHILDREN_PER_NODE (KEYS_PER_NODE + 1)

/*
<<<<<<< HEAD
 * The table has always exactly one reference from either mapped_device->map
 * or hash_cell->new_map. This reference is not counted in table->holders.
 * A pair of dm_create_table/dm_destroy_table functions is used for table
 * creation/destruction.
 *
 * Temporary references from the other code increase table->holders. A pair
 * of dm_table_get/dm_table_put functions is used to manipulate it.
 *
 * When the table is about to be destroyed, we wait for table->holders to
 * drop to zero.
 */

struct dm_table {
	struct mapped_device *md;
	atomic_t holders;
	unsigned type;

	/* btree table */
	unsigned int depth;
	unsigned int counts[MAX_DEPTH];	/* in nodes */
	sector_t *index[MAX_DEPTH];

	unsigned int num_targets;
	unsigned int num_allocated;
	sector_t *highs;
	struct dm_target *targets;

	struct target_type *immutable_target_type;
	unsigned integrity_supported:1;
	unsigned singleton:1;

	/*
	 * Indicates the rw permissions for the new logical
	 * device.  This should be a combination of FMODE_READ
	 * and FMODE_WRITE.
	 */
	fmode_t mode;

	/* a list of devices used by this table */
	struct list_head devices;

	/* events get handed up using this callback */
	void (*event_fn)(void *);
	void *event_context;

	struct dm_md_mempools *mempools;

	struct list_head target_callbacks;
};

/*
=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 * Similar to ceiling(log_size(n))
 */
static unsigned int int_log(unsigned int n, unsigned int base)
{
	int result = 0;

	while (n > 1) {
		n = dm_div_up(n, base);
		result++;
	}

	return result;
}

/*
 * Calculate the index of the child node of the n'th node k'th key.
 */
static inline unsigned int get_child(unsigned int n, unsigned int k)
{
	return (n * CHILDREN_PER_NODE) + k;
}

/*
 * Return the n'th node of level l from table t.
 */
static inline sector_t *get_node(struct dm_table *t,
				 unsigned int l, unsigned int n)
{
	return t->index[l] + (n * KEYS_PER_NODE);
}

/*
 * Return the highest key that you could lookup from the n'th
 * node on level l of the btree.
 */
static sector_t high(struct dm_table *t, unsigned int l, unsigned int n)
{
	for (; l < t->depth - 1; l++)
		n = get_child(n, CHILDREN_PER_NODE - 1);

	if (n >= t->counts[l])
<<<<<<< HEAD
		return (sector_t) - 1;
=======
		return (sector_t) -1;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	return get_node(t, l, n)[KEYS_PER_NODE - 1];
}

/*
 * Fills in a level of the btree based on the highs of the level
 * below it.
 */
static int setup_btree_index(unsigned int l, struct dm_table *t)
{
	unsigned int n, k;
	sector_t *node;

	for (n = 0U; n < t->counts[l]; n++) {
		node = get_node(t, l, n);

		for (k = 0U; k < KEYS_PER_NODE; k++)
			node[k] = high(t, l + 1, get_child(n, k));
	}

	return 0;
}

<<<<<<< HEAD
void *dm_vcalloc(unsigned long nmemb, unsigned long elem_size)
{
	unsigned long size;
	void *addr;

	/*
	 * Check that we're not going to overflow.
	 */
	if (nmemb > (ULONG_MAX / elem_size))
		return NULL;

	size = nmemb * elem_size;
	addr = vzalloc(size);

	return addr;
}
EXPORT_SYMBOL(dm_vcalloc);

=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
/*
 * highs, and targets are managed as dynamic arrays during a
 * table load.
 */
static int alloc_targets(struct dm_table *t, unsigned int num)
{
	sector_t *n_highs;
	struct dm_target *n_targets;
<<<<<<< HEAD
	int n = t->num_targets;

	/*
	 * Allocate both the target array and offset array at once.
	 * Append an empty entry to catch sectors beyond the end of
	 * the device.
	 */
	n_highs = (sector_t *) dm_vcalloc(num + 1, sizeof(struct dm_target) +
					  sizeof(sector_t));
=======

	/*
	 * Allocate both the target array and offset array at once.
	 */
	n_highs = kvcalloc(num, sizeof(struct dm_target) + sizeof(sector_t),
			   GFP_KERNEL);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	if (!n_highs)
		return -ENOMEM;

	n_targets = (struct dm_target *) (n_highs + num);

<<<<<<< HEAD
	if (n) {
		memcpy(n_highs, t->highs, sizeof(*n_highs) * n);
		memcpy(n_targets, t->targets, sizeof(*n_targets) * n);
	}

	memset(n_highs + n, -1, sizeof(*n_highs) * (num - n));
	vfree(t->highs);
=======
	memset(n_highs, -1, sizeof(*n_highs) * num);
	kvfree(t->highs);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	t->num_allocated = num;
	t->highs = n_highs;
	t->targets = n_targets;

	return 0;
}

<<<<<<< HEAD
int dm_table_create(struct dm_table **result, fmode_t mode,
		    unsigned num_targets, struct mapped_device *md)
{
	struct dm_table *t = kzalloc(sizeof(*t), GFP_KERNEL);
=======
int dm_table_create(struct dm_table **result, blk_mode_t mode,
		    unsigned int num_targets, struct mapped_device *md)
{
	struct dm_table *t;

	if (num_targets > DM_MAX_TARGETS)
		return -EOVERFLOW;

	t = kzalloc(sizeof(*t), GFP_KERNEL);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	if (!t)
		return -ENOMEM;

	INIT_LIST_HEAD(&t->devices);
<<<<<<< HEAD
	INIT_LIST_HEAD(&t->target_callbacks);
	atomic_set(&t->holders, 0);
=======
	init_rwsem(&t->devices_lock);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	if (!num_targets)
		num_targets = KEYS_PER_NODE;

	num_targets = dm_round_up(num_targets, KEYS_PER_NODE);

	if (!num_targets) {
		kfree(t);
<<<<<<< HEAD
		return -ENOMEM;
=======
		return -EOVERFLOW;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	}

	if (alloc_targets(t, num_targets)) {
		kfree(t);
<<<<<<< HEAD
		t = NULL;
		return -ENOMEM;
	}

=======
		return -ENOMEM;
	}

	t->type = DM_TYPE_NONE;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	t->mode = mode;
	t->md = md;
	*result = t;
	return 0;
}

<<<<<<< HEAD
static void free_devices(struct list_head *devices)
=======
static void free_devices(struct list_head *devices, struct mapped_device *md)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
{
	struct list_head *tmp, *next;

	list_for_each_safe(tmp, next, devices) {
		struct dm_dev_internal *dd =
		    list_entry(tmp, struct dm_dev_internal, list);
<<<<<<< HEAD
		DMWARN("dm_table_destroy: dm_put_device call missing for %s",
		       dd->dm_dev.name);
=======
		DMWARN("%s: dm_table_destroy: dm_put_device call missing for %s",
		       dm_device_name(md), dd->dm_dev->name);
		dm_put_table_device(md, dd->dm_dev);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		kfree(dd);
	}
}

<<<<<<< HEAD
void dm_table_destroy(struct dm_table *t)
{
	unsigned int i;

	if (!t)
		return;

	while (atomic_read(&t->holders))
		msleep(1);
	smp_mb();

	/* free the indexes */
	if (t->depth >= 2)
		vfree(t->index[t->depth - 2]);

	/* free the targets */
	for (i = 0; i < t->num_targets; i++) {
		struct dm_target *tgt = t->targets + i;

		if (tgt->type->dtr)
			tgt->type->dtr(tgt);

		dm_put_target_type(tgt->type);
	}

	vfree(t->highs);

	/* free the device list */
	free_devices(&t->devices);

	dm_free_md_mempools(t->mempools);

	kfree(t);
}

void dm_table_get(struct dm_table *t)
{
	atomic_inc(&t->holders);
}
EXPORT_SYMBOL(dm_table_get);

void dm_table_put(struct dm_table *t)
{
	if (!t)
		return;

	smp_mb__before_atomic_dec();
	atomic_dec(&t->holders);
}
EXPORT_SYMBOL(dm_table_put);

/*
 * Checks to see if we need to extend highs or targets.
 */
static inline int check_space(struct dm_table *t)
{
	if (t->num_targets >= t->num_allocated)
		return alloc_targets(t, t->num_allocated * 2);

	return 0;
}

=======
static void dm_table_destroy_crypto_profile(struct dm_table *t);

void dm_table_destroy(struct dm_table *t)
{
	if (!t)
		return;

	/* free the indexes */
	if (t->depth >= 2)
		kvfree(t->index[t->depth - 2]);

	/* free the targets */
	for (unsigned int i = 0; i < t->num_targets; i++) {
		struct dm_target *ti = dm_table_get_target(t, i);

		if (ti->type->dtr)
			ti->type->dtr(ti);

		dm_put_target_type(ti->type);
	}

	kvfree(t->highs);

	/* free the device list */
	free_devices(&t->devices, t->md);

	dm_free_md_mempools(t->mempools);

	dm_table_destroy_crypto_profile(t);

	kfree(t);
}

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
/*
 * See if we've already got a device in the list.
 */
static struct dm_dev_internal *find_device(struct list_head *l, dev_t dev)
{
	struct dm_dev_internal *dd;

<<<<<<< HEAD
	list_for_each_entry (dd, l, list)
		if (dd->dm_dev.bdev->bd_dev == dev)
=======
	list_for_each_entry(dd, l, list)
		if (dd->dm_dev->bdev->bd_dev == dev)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
			return dd;

	return NULL;
}

/*
<<<<<<< HEAD
 * Open a device so we can use it as a map destination.
 */
static int open_dev(struct dm_dev_internal *d, dev_t dev,
		    struct mapped_device *md)
{
	static char *_claim_ptr = "I belong to device-mapper";
	struct block_device *bdev;

	int r;

	BUG_ON(d->dm_dev.bdev);

	bdev = blkdev_get_by_dev(dev, d->dm_dev.mode | FMODE_EXCL, _claim_ptr);
	if (IS_ERR(bdev))
		return PTR_ERR(bdev);

	r = bd_link_disk_holder(bdev, dm_disk(md));
	if (r) {
		blkdev_put(bdev, d->dm_dev.mode | FMODE_EXCL);
		return r;
	}

	d->dm_dev.bdev = bdev;
	return 0;
}

/*
 * Close a device that we've been using.
 */
static void close_dev(struct dm_dev_internal *d, struct mapped_device *md)
{
	if (!d->dm_dev.bdev)
		return;

	bd_unlink_disk_holder(d->dm_dev.bdev, dm_disk(md));
	blkdev_put(d->dm_dev.bdev, d->dm_dev.mode | FMODE_EXCL);
	d->dm_dev.bdev = NULL;
}

/*
=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 * If possible, this checks an area of a destination device is invalid.
 */
static int device_area_is_invalid(struct dm_target *ti, struct dm_dev *dev,
				  sector_t start, sector_t len, void *data)
{
<<<<<<< HEAD
	struct request_queue *q;
	struct queue_limits *limits = data;
	struct block_device *bdev = dev->bdev;
	sector_t dev_size =
		i_size_read(bdev->bd_inode) >> SECTOR_SHIFT;
	unsigned short logical_block_size_sectors =
		limits->logical_block_size >> SECTOR_SHIFT;
	char b[BDEVNAME_SIZE];

	/*
	 * Some devices exist without request functions,
	 * such as loop devices not yet bound to backing files.
	 * Forbid the use of such devices.
	 */
	q = bdev_get_queue(bdev);
	if (!q || !q->make_request_fn) {
		DMWARN("%s: %s is not yet initialised: "
		       "start=%llu, len=%llu, dev_size=%llu",
		       dm_device_name(ti->table->md), bdevname(bdev, b),
		       (unsigned long long)start,
		       (unsigned long long)len,
		       (unsigned long long)dev_size);
		return 1;
	}
=======
	struct queue_limits *limits = data;
	struct block_device *bdev = dev->bdev;
	sector_t dev_size = bdev_nr_sectors(bdev);
	unsigned short logical_block_size_sectors =
		limits->logical_block_size >> SECTOR_SHIFT;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	if (!dev_size)
		return 0;

	if ((start >= dev_size) || (start + len > dev_size)) {
<<<<<<< HEAD
		DMWARN("%s: %s too small for target: "
		       "start=%llu, len=%llu, dev_size=%llu",
		       dm_device_name(ti->table->md), bdevname(bdev, b),
		       (unsigned long long)start,
		       (unsigned long long)len,
		       (unsigned long long)dev_size);
		return 1;
	}

=======
		DMERR("%s: %pg too small for target: start=%llu, len=%llu, dev_size=%llu",
		      dm_device_name(ti->table->md), bdev,
		      (unsigned long long)start,
		      (unsigned long long)len,
		      (unsigned long long)dev_size);
		return 1;
	}

	/*
	 * If the target is mapped to zoned block device(s), check
	 * that the zones are not partially mapped.
	 */
	if (bdev_is_zoned(bdev)) {
		unsigned int zone_sectors = bdev_zone_sectors(bdev);

		if (start & (zone_sectors - 1)) {
			DMERR("%s: start=%llu not aligned to h/w zone size %u of %pg",
			      dm_device_name(ti->table->md),
			      (unsigned long long)start,
			      zone_sectors, bdev);
			return 1;
		}

		/*
		 * Note: The last zone of a zoned block device may be smaller
		 * than other zones. So for a target mapping the end of a
		 * zoned block device with such a zone, len would not be zone
		 * aligned. We do not allow such last smaller zone to be part
		 * of the mapping here to ensure that mappings with multiple
		 * devices do not end up with a smaller zone in the middle of
		 * the sector range.
		 */
		if (len & (zone_sectors - 1)) {
			DMERR("%s: len=%llu not aligned to h/w zone size %u of %pg",
			      dm_device_name(ti->table->md),
			      (unsigned long long)len,
			      zone_sectors, bdev);
			return 1;
		}
	}

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	if (logical_block_size_sectors <= 1)
		return 0;

	if (start & (logical_block_size_sectors - 1)) {
<<<<<<< HEAD
		DMWARN("%s: start=%llu not aligned to h/w "
		       "logical block size %u of %s",
		       dm_device_name(ti->table->md),
		       (unsigned long long)start,
		       limits->logical_block_size, bdevname(bdev, b));
=======
		DMERR("%s: start=%llu not aligned to h/w logical block size %u of %pg",
		      dm_device_name(ti->table->md),
		      (unsigned long long)start,
		      limits->logical_block_size, bdev);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		return 1;
	}

	if (len & (logical_block_size_sectors - 1)) {
<<<<<<< HEAD
		DMWARN("%s: len=%llu not aligned to h/w "
		       "logical block size %u of %s",
		       dm_device_name(ti->table->md),
		       (unsigned long long)len,
		       limits->logical_block_size, bdevname(bdev, b));
=======
		DMERR("%s: len=%llu not aligned to h/w logical block size %u of %pg",
		      dm_device_name(ti->table->md),
		      (unsigned long long)len,
		      limits->logical_block_size, bdev);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		return 1;
	}

	return 0;
}

/*
 * This upgrades the mode on an already open dm_dev, being
 * careful to leave things as they were if we fail to reopen the
 * device and not to touch the existing bdev field in case
<<<<<<< HEAD
 * it is accessed concurrently inside dm_table_any_congested().
 */
static int upgrade_mode(struct dm_dev_internal *dd, fmode_t new_mode,
			struct mapped_device *md)
{
	int r;
	struct dm_dev_internal dd_new, dd_old;

	dd_new = dd_old = *dd;

	dd_new.dm_dev.mode |= new_mode;
	dd_new.dm_dev.bdev = NULL;

	r = open_dev(&dd_new, dd->dm_dev.bdev->bd_dev, md);
	if (r)
		return r;

	dd->dm_dev.mode |= new_mode;
	close_dev(&dd_old, md);
=======
 * it is accessed concurrently.
 */
static int upgrade_mode(struct dm_dev_internal *dd, blk_mode_t new_mode,
			struct mapped_device *md)
{
	int r;
	struct dm_dev *old_dev, *new_dev;

	old_dev = dd->dm_dev;

	r = dm_get_table_device(md, dd->dm_dev->bdev->bd_dev,
				dd->dm_dev->mode | new_mode, &new_dev);
	if (r)
		return r;

	dd->dm_dev = new_dev;
	dm_put_table_device(md, old_dev);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	return 0;
}

/*
 * Add a device to the list, or just increment the usage count if
 * it's already present.
<<<<<<< HEAD
 */
int dm_get_device(struct dm_target *ti, const char *path, fmode_t mode,
		  struct dm_dev **result)
{
	int r;
	dev_t uninitialized_var(dev);
	struct dm_dev_internal *dd;
	unsigned int major, minor;
	struct dm_table *t = ti->table;
	char dummy;
=======
 *
 * Note: the __ref annotation is because this function can call the __init
 * marked early_lookup_bdev when called during early boot code from dm-init.c.
 */
int __ref dm_get_device(struct dm_target *ti, const char *path, blk_mode_t mode,
		  struct dm_dev **result)
{
	int r;
	dev_t dev;
	unsigned int major, minor;
	char dummy;
	struct dm_dev_internal *dd;
	struct dm_table *t = ti->table;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	BUG_ON(!t);

	if (sscanf(path, "%u:%u%c", &major, &minor, &dummy) == 2) {
		/* Extract the major/minor numbers */
		dev = MKDEV(major, minor);
		if (MAJOR(dev) != major || MINOR(dev) != minor)
			return -EOVERFLOW;
	} else {
<<<<<<< HEAD
		/* convert the path to a device */
		struct block_device *bdev = lookup_bdev(path);

		if (IS_ERR(bdev))
			return PTR_ERR(bdev);
		dev = bdev->bd_dev;
		bdput(bdev);
	}
=======
		r = lookup_bdev(path, &dev);
#ifndef MODULE
		if (r && system_state < SYSTEM_RUNNING)
			r = early_lookup_bdev(path, &dev);
#endif
		if (r)
			return r;
	}
	if (dev == disk_devt(t->md->disk))
		return -EINVAL;

	down_write(&t->devices_lock);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	dd = find_device(&t->devices, dev);
	if (!dd) {
		dd = kmalloc(sizeof(*dd), GFP_KERNEL);
<<<<<<< HEAD
		if (!dd)
			return -ENOMEM;

		dd->dm_dev.mode = mode;
		dd->dm_dev.bdev = NULL;

		if ((r = open_dev(dd, dev, t->md))) {
			kfree(dd);
			return r;
		}

		format_dev_t(dd->dm_dev.name, dev);

		atomic_set(&dd->count, 0);
		list_add(&dd->list, &t->devices);

	} else if (dd->dm_dev.mode != (mode | dd->dm_dev.mode)) {
		r = upgrade_mode(dd, mode, t->md);
		if (r)
			return r;
	}
	atomic_inc(&dd->count);

	*result = &dd->dm_dev;
	return 0;
}
EXPORT_SYMBOL(dm_get_device);

int dm_set_device_limits(struct dm_target *ti, struct dm_dev *dev,
			 sector_t start, sector_t len, void *data)
=======
		if (!dd) {
			r = -ENOMEM;
			goto unlock_ret_r;
		}

		r = dm_get_table_device(t->md, dev, mode, &dd->dm_dev);
		if (r) {
			kfree(dd);
			goto unlock_ret_r;
		}

		refcount_set(&dd->count, 1);
		list_add(&dd->list, &t->devices);
		goto out;

	} else if (dd->dm_dev->mode != (mode | dd->dm_dev->mode)) {
		r = upgrade_mode(dd, mode, t->md);
		if (r)
			goto unlock_ret_r;
	}
	refcount_inc(&dd->count);
out:
	up_write(&t->devices_lock);
	*result = dd->dm_dev;
	return 0;

unlock_ret_r:
	up_write(&t->devices_lock);
	return r;
}
EXPORT_SYMBOL(dm_get_device);

static int dm_set_device_limits(struct dm_target *ti, struct dm_dev *dev,
				sector_t start, sector_t len, void *data)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
{
	struct queue_limits *limits = data;
	struct block_device *bdev = dev->bdev;
	struct request_queue *q = bdev_get_queue(bdev);
<<<<<<< HEAD
	char b[BDEVNAME_SIZE];

	if (unlikely(!q)) {
		DMWARN("%s: Cannot set limits for nonexistent device %s",
		       dm_device_name(ti->table->md), bdevname(bdev, b));
		return 0;
	}

	if (bdev_stack_limits(limits, bdev, start) < 0)
		DMWARN("%s: adding target device %s caused an alignment inconsistency: "
		       "physical_block_size=%u, logical_block_size=%u, "
		       "alignment_offset=%u, start=%llu",
		       dm_device_name(ti->table->md), bdevname(bdev, b),
=======

	if (unlikely(!q)) {
		DMWARN("%s: Cannot set limits for nonexistent device %pg",
		       dm_device_name(ti->table->md), bdev);
		return 0;
	}

	if (blk_stack_limits(limits, &q->limits,
			get_start_sect(bdev) + start) < 0)
		DMWARN("%s: adding target device %pg caused an alignment inconsistency: "
		       "physical_block_size=%u, logical_block_size=%u, "
		       "alignment_offset=%u, start=%llu",
		       dm_device_name(ti->table->md), bdev,
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		       q->limits.physical_block_size,
		       q->limits.logical_block_size,
		       q->limits.alignment_offset,
		       (unsigned long long) start << SECTOR_SHIFT);
<<<<<<< HEAD

	/*
	 * Check if merge fn is supported.
	 * If not we'll force DM to use PAGE_SIZE or
	 * smaller I/O, just to be safe.
	 */
	if (dm_queue_merge_is_compulsory(q) && !ti->type->merge)
		blk_limits_max_hw_sectors(limits,
					  (unsigned int) (PAGE_SIZE >> 9));
	return 0;
}
EXPORT_SYMBOL_GPL(dm_set_device_limits);
=======
	return 0;
}
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

/*
 * Decrement a device's use count and remove it if necessary.
 */
void dm_put_device(struct dm_target *ti, struct dm_dev *d)
{
<<<<<<< HEAD
	struct dm_dev_internal *dd = container_of(d, struct dm_dev_internal,
						  dm_dev);

	if (atomic_dec_and_test(&dd->count)) {
		close_dev(dd, ti->table->md);
		list_del(&dd->list);
		kfree(dd);
	}
=======
	int found = 0;
	struct dm_table *t = ti->table;
	struct list_head *devices = &t->devices;
	struct dm_dev_internal *dd;

	down_write(&t->devices_lock);

	list_for_each_entry(dd, devices, list) {
		if (dd->dm_dev == d) {
			found = 1;
			break;
		}
	}
	if (!found) {
		DMERR("%s: device %s not in table devices list",
		      dm_device_name(t->md), d->name);
		goto unlock_ret;
	}
	if (refcount_dec_and_test(&dd->count)) {
		dm_put_table_device(t->md, d);
		list_del(&dd->list);
		kfree(dd);
	}

unlock_ret:
	up_write(&t->devices_lock);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}
EXPORT_SYMBOL(dm_put_device);

/*
 * Checks to see if the target joins onto the end of the table.
 */
<<<<<<< HEAD
static int adjoin(struct dm_table *table, struct dm_target *ti)
{
	struct dm_target *prev;

	if (!table->num_targets)
		return !ti->begin;

	prev = &table->targets[table->num_targets - 1];
=======
static int adjoin(struct dm_table *t, struct dm_target *ti)
{
	struct dm_target *prev;

	if (!t->num_targets)
		return !ti->begin;

	prev = &t->targets[t->num_targets - 1];
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	return (ti->begin == (prev->begin + prev->len));
}

/*
 * Used to dynamically allocate the arg array.
 *
 * We do first allocation with GFP_NOIO because dm-mpath and dm-thin must
 * process messages even if some device is suspended. These messages have a
 * small fixed number of arguments.
 *
 * On the other hand, dm-switch needs to process bulk data using messages and
 * excessive use of GFP_NOIO could cause trouble.
 */
<<<<<<< HEAD
static char **realloc_argv(unsigned *array_size, char **old_argv)
{
	char **argv;
	unsigned new_size;
	gfp_t gfp;

	if (*array_size) {
		new_size = *array_size * 2;
=======
static char **realloc_argv(unsigned int *size, char **old_argv)
{
	char **argv;
	unsigned int new_size;
	gfp_t gfp;

	if (*size) {
		new_size = *size * 2;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		gfp = GFP_KERNEL;
	} else {
		new_size = 8;
		gfp = GFP_NOIO;
	}
<<<<<<< HEAD
	argv = kmalloc(new_size * sizeof(*argv), gfp);
	if (argv) {
		memcpy(argv, old_argv, *array_size * sizeof(*argv));
		*array_size = new_size;
=======
	argv = kmalloc_array(new_size, sizeof(*argv), gfp);
	if (argv && old_argv) {
		memcpy(argv, old_argv, *size * sizeof(*argv));
		*size = new_size;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	}

	kfree(old_argv);
	return argv;
}

/*
 * Destructively splits up the argument list to pass to ctr.
 */
int dm_split_args(int *argc, char ***argvp, char *input)
{
	char *start, *end = input, *out, **argv = NULL;
<<<<<<< HEAD
	unsigned array_size = 0;
=======
	unsigned int array_size = 0;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	*argc = 0;

	if (!input) {
		*argvp = NULL;
		return 0;
	}

	argv = realloc_argv(&array_size, argv);
	if (!argv)
		return -ENOMEM;

	while (1) {
		/* Skip whitespace */
		start = skip_spaces(end);

		if (!*start)
			break;	/* success, we hit the end */

		/* 'out' is used to remove any back-quotes */
		end = out = start;
		while (*end) {
			/* Everything apart from '\0' can be quoted */
			if (*end == '\\' && *(end + 1)) {
				*out++ = *(end + 1);
				end += 2;
				continue;
			}

			if (isspace(*end))
				break;	/* end of token */

			*out++ = *end++;
		}

		/* have we already filled the array ? */
		if ((*argc + 1) > array_size) {
			argv = realloc_argv(&array_size, argv);
			if (!argv)
				return -ENOMEM;
		}

		/* we know this is whitespace */
		if (*end)
			end++;

		/* terminate the string and put it in the array */
		*out = '\0';
		argv[*argc] = start;
		(*argc)++;
	}

	*argvp = argv;
	return 0;
}

/*
 * Impose necessary and sufficient conditions on a devices's table such
 * that any incoming bio which respects its logical_block_size can be
 * processed successfully.  If it falls across the boundary between
 * two or more targets, the size of each piece it gets split into must
 * be compatible with the logical_block_size of the target processing it.
 */
<<<<<<< HEAD
static int validate_hardware_logical_block_alignment(struct dm_table *table,
						 struct queue_limits *limits)
=======
static int validate_hardware_logical_block_alignment(struct dm_table *t,
						     struct queue_limits *limits)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
{
	/*
	 * This function uses arithmetic modulo the logical_block_size
	 * (in units of 512-byte sectors).
	 */
	unsigned short device_logical_block_size_sects =
		limits->logical_block_size >> SECTOR_SHIFT;

	/*
	 * Offset of the start of the next table entry, mod logical_block_size.
	 */
	unsigned short next_target_start = 0;

	/*
	 * Given an aligned bio that extends beyond the end of a
	 * target, how many sectors must the next target handle?
	 */
	unsigned short remaining = 0;

<<<<<<< HEAD
	struct dm_target *uninitialized_var(ti);
	struct queue_limits ti_limits;
	unsigned i = 0;
=======
	struct dm_target *ti;
	struct queue_limits ti_limits;
	unsigned int i;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	/*
	 * Check each entry in the table in turn.
	 */
<<<<<<< HEAD
	while (i < dm_table_get_num_targets(table)) {
		ti = dm_table_get_target(table, i++);
=======
	for (i = 0; i < t->num_targets; i++) {
		ti = dm_table_get_target(t, i);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

		blk_set_stacking_limits(&ti_limits);

		/* combine all target devices' limits */
		if (ti->type->iterate_devices)
			ti->type->iterate_devices(ti, dm_set_device_limits,
						  &ti_limits);

		/*
		 * If the remaining sectors fall entirely within this
		 * table entry are they compatible with its logical_block_size?
		 */
		if (remaining < ti->len &&
		    remaining & ((ti_limits.logical_block_size >>
				  SECTOR_SHIFT) - 1))
			break;	/* Error */

		next_target_start =
		    (unsigned short) ((next_target_start + ti->len) &
				      (device_logical_block_size_sects - 1));
		remaining = next_target_start ?
		    device_logical_block_size_sects - next_target_start : 0;
	}

	if (remaining) {
<<<<<<< HEAD
		DMWARN("%s: table line %u (start sect %llu len %llu) "
		       "not aligned to h/w logical block size %u",
		       dm_device_name(table->md), i,
		       (unsigned long long) ti->begin,
		       (unsigned long long) ti->len,
		       limits->logical_block_size);
=======
		DMERR("%s: table line %u (start sect %llu len %llu) "
		      "not aligned to h/w logical block size %u",
		      dm_device_name(t->md), i,
		      (unsigned long long) ti->begin,
		      (unsigned long long) ti->len,
		      limits->logical_block_size);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		return -EINVAL;
	}

	return 0;
}

int dm_table_add_target(struct dm_table *t, const char *type,
			sector_t start, sector_t len, char *params)
{
	int r = -EINVAL, argc;
	char **argv;
<<<<<<< HEAD
	struct dm_target *tgt;
=======
	struct dm_target *ti;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	if (t->singleton) {
		DMERR("%s: target type %s must appear alone in table",
		      dm_device_name(t->md), t->targets->type->name);
		return -EINVAL;
	}

<<<<<<< HEAD
	if ((r = check_space(t)))
		return r;

	tgt = t->targets + t->num_targets;
	memset(tgt, 0, sizeof(*tgt));
=======
	BUG_ON(t->num_targets >= t->num_allocated);

	ti = t->targets + t->num_targets;
	memset(ti, 0, sizeof(*ti));
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	if (!len) {
		DMERR("%s: zero-length target", dm_device_name(t->md));
		return -EINVAL;
	}

<<<<<<< HEAD
	tgt->type = dm_get_target_type(type);
	if (!tgt->type) {
		DMERR("%s: %s: unknown target type", dm_device_name(t->md),
		      type);
		return -EINVAL;
	}

	if (dm_target_needs_singleton(tgt->type)) {
		if (t->num_targets) {
			DMERR("%s: target type %s must appear alone in table",
			      dm_device_name(t->md), type);
			return -EINVAL;
		}
		t->singleton = 1;
	}

	if (dm_target_always_writeable(tgt->type) && !(t->mode & FMODE_WRITE)) {
		DMERR("%s: target type %s may not be included in read-only tables",
		      dm_device_name(t->md), type);
		return -EINVAL;
	}

	if (t->immutable_target_type) {
		if (t->immutable_target_type != tgt->type) {
			DMERR("%s: immutable target type %s cannot be mixed with other target types",
			      dm_device_name(t->md), t->immutable_target_type->name);
			return -EINVAL;
		}
	} else if (dm_target_is_immutable(tgt->type)) {
		if (t->num_targets) {
			DMERR("%s: immutable target type %s cannot be mixed with other target types",
			      dm_device_name(t->md), tgt->type->name);
			return -EINVAL;
		}
		t->immutable_target_type = tgt->type;
	}

	tgt->table = t;
	tgt->begin = start;
	tgt->len = len;
	tgt->error = "Unknown error";
=======
	ti->type = dm_get_target_type(type);
	if (!ti->type) {
		DMERR("%s: %s: unknown target type", dm_device_name(t->md), type);
		return -EINVAL;
	}

	if (dm_target_needs_singleton(ti->type)) {
		if (t->num_targets) {
			ti->error = "singleton target type must appear alone in table";
			goto bad;
		}
		t->singleton = true;
	}

	if (dm_target_always_writeable(ti->type) &&
	    !(t->mode & BLK_OPEN_WRITE)) {
		ti->error = "target type may not be included in a read-only table";
		goto bad;
	}

	if (t->immutable_target_type) {
		if (t->immutable_target_type != ti->type) {
			ti->error = "immutable target type cannot be mixed with other target types";
			goto bad;
		}
	} else if (dm_target_is_immutable(ti->type)) {
		if (t->num_targets) {
			ti->error = "immutable target type cannot be mixed with other target types";
			goto bad;
		}
		t->immutable_target_type = ti->type;
	}

	if (dm_target_has_integrity(ti->type))
		t->integrity_added = 1;

	ti->table = t;
	ti->begin = start;
	ti->len = len;
	ti->error = "Unknown error";
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	/*
	 * Does this target adjoin the previous one ?
	 */
<<<<<<< HEAD
	if (!adjoin(t, tgt)) {
		tgt->error = "Gap in table";
		r = -EINVAL;
=======
	if (!adjoin(t, ti)) {
		ti->error = "Gap in table";
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		goto bad;
	}

	r = dm_split_args(&argc, &argv, params);
	if (r) {
<<<<<<< HEAD
		tgt->error = "couldn't split parameters (insufficient memory)";
		goto bad;
	}

	r = tgt->type->ctr(tgt, argc, argv);
=======
		ti->error = "couldn't split parameters";
		goto bad;
	}

	r = ti->type->ctr(ti, argc, argv);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	kfree(argv);
	if (r)
		goto bad;

<<<<<<< HEAD
	t->highs[t->num_targets++] = tgt->begin + tgt->len - 1;

	if (!tgt->num_discard_requests && tgt->discards_supported)
		DMWARN("%s: %s: ignoring discards_supported because num_discard_requests is zero.",
		       dm_device_name(t->md), type);

	return 0;

 bad:
	DMERR("%s: %s: %s", dm_device_name(t->md), type, tgt->error);
	dm_put_target_type(tgt->type);
=======
	t->highs[t->num_targets++] = ti->begin + ti->len - 1;

	if (!ti->num_discard_bios && ti->discards_supported)
		DMWARN("%s: %s: ignoring discards_supported because num_discard_bios is zero.",
		       dm_device_name(t->md), type);

	if (ti->limit_swap_bios && !static_key_enabled(&swap_bios_enabled.key))
		static_branch_enable(&swap_bios_enabled);

	return 0;

 bad:
	DMERR("%s: %s: %s (%pe)", dm_device_name(t->md), type, ti->error, ERR_PTR(r));
	dm_put_target_type(ti->type);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	return r;
}

/*
 * Target argument parsing helpers.
 */
<<<<<<< HEAD
static int validate_next_arg(struct dm_arg *arg, struct dm_arg_set *arg_set,
			     unsigned *value, char **error, unsigned grouped)
=======
static int validate_next_arg(const struct dm_arg *arg, struct dm_arg_set *arg_set,
			     unsigned int *value, char **error, unsigned int grouped)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
{
	const char *arg_str = dm_shift_arg(arg_set);
	char dummy;

	if (!arg_str ||
	    (sscanf(arg_str, "%u%c", value, &dummy) != 1) ||
	    (*value < arg->min) ||
	    (*value > arg->max) ||
	    (grouped && arg_set->argc < *value)) {
		*error = arg->error;
		return -EINVAL;
	}

	return 0;
}

<<<<<<< HEAD
int dm_read_arg(struct dm_arg *arg, struct dm_arg_set *arg_set,
		unsigned *value, char **error)
=======
int dm_read_arg(const struct dm_arg *arg, struct dm_arg_set *arg_set,
		unsigned int *value, char **error)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
{
	return validate_next_arg(arg, arg_set, value, error, 0);
}
EXPORT_SYMBOL(dm_read_arg);

<<<<<<< HEAD
int dm_read_arg_group(struct dm_arg *arg, struct dm_arg_set *arg_set,
		      unsigned *value, char **error)
=======
int dm_read_arg_group(const struct dm_arg *arg, struct dm_arg_set *arg_set,
		      unsigned int *value, char **error)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
{
	return validate_next_arg(arg, arg_set, value, error, 1);
}
EXPORT_SYMBOL(dm_read_arg_group);

const char *dm_shift_arg(struct dm_arg_set *as)
{
	char *r;

	if (as->argc) {
		as->argc--;
		r = *as->argv;
		as->argv++;
		return r;
	}

	return NULL;
}
EXPORT_SYMBOL(dm_shift_arg);

<<<<<<< HEAD
void dm_consume_args(struct dm_arg_set *as, unsigned num_args)
=======
void dm_consume_args(struct dm_arg_set *as, unsigned int num_args)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
{
	BUG_ON(as->argc < num_args);
	as->argc -= num_args;
	as->argv += num_args;
}
EXPORT_SYMBOL(dm_consume_args);

<<<<<<< HEAD
static int dm_table_set_type(struct dm_table *t)
{
	unsigned i;
	unsigned bio_based = 0, request_based = 0;
	struct dm_target *tgt;
	struct dm_dev_internal *dd;
	struct list_head *devices;

	for (i = 0; i < t->num_targets; i++) {
		tgt = t->targets + i;
		if (dm_target_request_based(tgt))
=======
static bool __table_type_bio_based(enum dm_queue_mode table_type)
{
	return (table_type == DM_TYPE_BIO_BASED ||
		table_type == DM_TYPE_DAX_BIO_BASED);
}

static bool __table_type_request_based(enum dm_queue_mode table_type)
{
	return table_type == DM_TYPE_REQUEST_BASED;
}

void dm_table_set_type(struct dm_table *t, enum dm_queue_mode type)
{
	t->type = type;
}
EXPORT_SYMBOL_GPL(dm_table_set_type);

/* validate the dax capability of the target device span */
static int device_not_dax_capable(struct dm_target *ti, struct dm_dev *dev,
			sector_t start, sector_t len, void *data)
{
	if (dev->dax_dev)
		return false;

	DMDEBUG("%pg: error: dax unsupported by block device", dev->bdev);
	return true;
}

/* Check devices support synchronous DAX */
static int device_not_dax_synchronous_capable(struct dm_target *ti, struct dm_dev *dev,
					      sector_t start, sector_t len, void *data)
{
	return !dev->dax_dev || !dax_synchronous(dev->dax_dev);
}

static bool dm_table_supports_dax(struct dm_table *t,
				  iterate_devices_callout_fn iterate_fn)
{
	/* Ensure that all targets support DAX. */
	for (unsigned int i = 0; i < t->num_targets; i++) {
		struct dm_target *ti = dm_table_get_target(t, i);

		if (!ti->type->direct_access)
			return false;

		if (dm_target_is_wildcard(ti->type) ||
		    !ti->type->iterate_devices ||
		    ti->type->iterate_devices(ti, iterate_fn, NULL))
			return false;
	}

	return true;
}

static int device_is_rq_stackable(struct dm_target *ti, struct dm_dev *dev,
				  sector_t start, sector_t len, void *data)
{
	struct block_device *bdev = dev->bdev;
	struct request_queue *q = bdev_get_queue(bdev);

	/* request-based cannot stack on partitions! */
	if (bdev_is_partition(bdev))
		return false;

	return queue_is_mq(q);
}

static int dm_table_determine_type(struct dm_table *t)
{
	unsigned int bio_based = 0, request_based = 0, hybrid = 0;
	struct dm_target *ti;
	struct list_head *devices = dm_table_get_devices(t);
	enum dm_queue_mode live_md_type = dm_get_md_type(t->md);

	if (t->type != DM_TYPE_NONE) {
		/* target already set the table's type */
		if (t->type == DM_TYPE_BIO_BASED) {
			/* possibly upgrade to a variant of bio-based */
			goto verify_bio_based;
		}
		BUG_ON(t->type == DM_TYPE_DAX_BIO_BASED);
		goto verify_rq_based;
	}

	for (unsigned int i = 0; i < t->num_targets; i++) {
		ti = dm_table_get_target(t, i);
		if (dm_target_hybrid(ti))
			hybrid = 1;
		else if (dm_target_request_based(ti))
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
			request_based = 1;
		else
			bio_based = 1;

		if (bio_based && request_based) {
<<<<<<< HEAD
			DMWARN("Inconsistent table: different target types"
			       " can't be mixed up");
=======
			DMERR("Inconsistent table: different target types can't be mixed up");
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
			return -EINVAL;
		}
	}

<<<<<<< HEAD
	if (bio_based) {
		/* We must use this table as bio-based */
		t->type = DM_TYPE_BIO_BASED;
=======
	if (hybrid && !bio_based && !request_based) {
		/*
		 * The targets can work either way.
		 * Determine the type from the live device.
		 * Default to bio-based if device is new.
		 */
		if (__table_type_request_based(live_md_type))
			request_based = 1;
		else
			bio_based = 1;
	}

	if (bio_based) {
verify_bio_based:
		/* We must use this table as bio-based */
		t->type = DM_TYPE_BIO_BASED;
		if (dm_table_supports_dax(t, device_not_dax_capable) ||
		    (list_empty(devices) && live_md_type == DM_TYPE_DAX_BIO_BASED)) {
			t->type = DM_TYPE_DAX_BIO_BASED;
		}
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		return 0;
	}

	BUG_ON(!request_based); /* No targets in this table */

<<<<<<< HEAD
	/* Non-request-stackable devices can't be used for request-based dm */
	devices = dm_table_get_devices(t);
	list_for_each_entry(dd, devices, list) {
		if (!blk_queue_stackable(bdev_get_queue(dd->dm_dev.bdev))) {
			DMWARN("table load rejected: including"
			       " non-request-stackable devices");
			return -EINVAL;
		}
	}

=======
	t->type = DM_TYPE_REQUEST_BASED;

verify_rq_based:
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	/*
	 * Request-based dm supports only tables that have a single target now.
	 * To support multiple targets, request splitting support is needed,
	 * and that needs lots of changes in the block-layer.
	 * (e.g. request completion process for partial completion.)
	 */
	if (t->num_targets > 1) {
<<<<<<< HEAD
		DMWARN("Request-based dm doesn't support multiple targets yet");
		return -EINVAL;
	}

	t->type = DM_TYPE_REQUEST_BASED;
=======
		DMERR("request-based DM doesn't support multiple targets");
		return -EINVAL;
	}

	if (list_empty(devices)) {
		int srcu_idx;
		struct dm_table *live_table = dm_get_live_table(t->md, &srcu_idx);

		/* inherit live table's type */
		if (live_table)
			t->type = live_table->type;
		dm_put_live_table(t->md, srcu_idx);
		return 0;
	}

	ti = dm_table_get_immutable_target(t);
	if (!ti) {
		DMERR("table load rejected: immutable target is required");
		return -EINVAL;
	} else if (ti->max_io_len) {
		DMERR("table load rejected: immutable target that splits IO is not supported");
		return -EINVAL;
	}

	/* Non-request-stackable devices can't be used for request-based dm */
	if (!ti->type->iterate_devices ||
	    !ti->type->iterate_devices(ti, device_is_rq_stackable, NULL)) {
		DMERR("table load rejected: including non-request-stackable devices");
		return -EINVAL;
	}
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	return 0;
}

<<<<<<< HEAD
unsigned dm_table_get_type(struct dm_table *t)
=======
enum dm_queue_mode dm_table_get_type(struct dm_table *t)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
{
	return t->type;
}

struct target_type *dm_table_get_immutable_target_type(struct dm_table *t)
{
	return t->immutable_target_type;
}

<<<<<<< HEAD
bool dm_table_request_based(struct dm_table *t)
{
	return dm_table_get_type(t) == DM_TYPE_REQUEST_BASED;
}

int dm_table_alloc_md_mempools(struct dm_table *t)
{
	unsigned type = dm_table_get_type(t);

	if (unlikely(type == DM_TYPE_NONE)) {
		DMWARN("no table type is set, can't allocate mempools");
		return -EINVAL;
	}

	t->mempools = dm_alloc_md_mempools(type, t->integrity_supported);
	if (!t->mempools)
		return -ENOMEM;

	return 0;
}

void dm_table_free_md_mempools(struct dm_table *t)
{
	dm_free_md_mempools(t->mempools);
	t->mempools = NULL;
}

struct dm_md_mempools *dm_table_get_md_mempools(struct dm_table *t)
{
	return t->mempools;
=======
struct dm_target *dm_table_get_immutable_target(struct dm_table *t)
{
	/* Immutable target is implicitly a singleton */
	if (t->num_targets > 1 ||
	    !dm_target_is_immutable(t->targets[0].type))
		return NULL;

	return t->targets;
}

struct dm_target *dm_table_get_wildcard_target(struct dm_table *t)
{
	for (unsigned int i = 0; i < t->num_targets; i++) {
		struct dm_target *ti = dm_table_get_target(t, i);

		if (dm_target_is_wildcard(ti->type))
			return ti;
	}

	return NULL;
}

bool dm_table_bio_based(struct dm_table *t)
{
	return __table_type_bio_based(dm_table_get_type(t));
}

bool dm_table_request_based(struct dm_table *t)
{
	return __table_type_request_based(dm_table_get_type(t));
}

static bool dm_table_supports_poll(struct dm_table *t);

static int dm_table_alloc_md_mempools(struct dm_table *t, struct mapped_device *md)
{
	enum dm_queue_mode type = dm_table_get_type(t);
	unsigned int per_io_data_size = 0, front_pad, io_front_pad;
	unsigned int min_pool_size = 0, pool_size;
	struct dm_md_mempools *pools;

	if (unlikely(type == DM_TYPE_NONE)) {
		DMERR("no table type is set, can't allocate mempools");
		return -EINVAL;
	}

	pools = kzalloc_node(sizeof(*pools), GFP_KERNEL, md->numa_node_id);
	if (!pools)
		return -ENOMEM;

	if (type == DM_TYPE_REQUEST_BASED) {
		pool_size = dm_get_reserved_rq_based_ios();
		front_pad = offsetof(struct dm_rq_clone_bio_info, clone);
		goto init_bs;
	}

	for (unsigned int i = 0; i < t->num_targets; i++) {
		struct dm_target *ti = dm_table_get_target(t, i);

		per_io_data_size = max(per_io_data_size, ti->per_io_data_size);
		min_pool_size = max(min_pool_size, ti->num_flush_bios);
	}
	pool_size = max(dm_get_reserved_bio_based_ios(), min_pool_size);
	front_pad = roundup(per_io_data_size,
		__alignof__(struct dm_target_io)) + DM_TARGET_IO_BIO_OFFSET;

	io_front_pad = roundup(per_io_data_size,
		__alignof__(struct dm_io)) + DM_IO_BIO_OFFSET;
	if (bioset_init(&pools->io_bs, pool_size, io_front_pad,
			dm_table_supports_poll(t) ? BIOSET_PERCPU_CACHE : 0))
		goto out_free_pools;
	if (t->integrity_supported &&
	    bioset_integrity_create(&pools->io_bs, pool_size))
		goto out_free_pools;
init_bs:
	if (bioset_init(&pools->bs, pool_size, front_pad, 0))
		goto out_free_pools;
	if (t->integrity_supported &&
	    bioset_integrity_create(&pools->bs, pool_size))
		goto out_free_pools;

	t->mempools = pools;
	return 0;

out_free_pools:
	dm_free_md_mempools(pools);
	return -ENOMEM;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

static int setup_indexes(struct dm_table *t)
{
	int i;
	unsigned int total = 0;
	sector_t *indexes;

	/* allocate the space for *all* the indexes */
	for (i = t->depth - 2; i >= 0; i--) {
		t->counts[i] = dm_div_up(t->counts[i + 1], CHILDREN_PER_NODE);
		total += t->counts[i];
	}

<<<<<<< HEAD
	indexes = (sector_t *) dm_vcalloc(total, (unsigned long) NODE_SIZE);
=======
	indexes = kvcalloc(total, NODE_SIZE, GFP_KERNEL);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	if (!indexes)
		return -ENOMEM;

	/* set up internal nodes, bottom-up */
	for (i = t->depth - 2; i >= 0; i--) {
		t->index[i] = indexes;
		indexes += (KEYS_PER_NODE * t->counts[i]);
		setup_btree_index(i, t);
	}

	return 0;
}

/*
 * Builds the btree to index the map.
 */
static int dm_table_build_index(struct dm_table *t)
{
	int r = 0;
	unsigned int leaf_nodes;

	/* how many indexes will the btree have ? */
	leaf_nodes = dm_div_up(t->num_targets, KEYS_PER_NODE);
	t->depth = 1 + int_log(leaf_nodes, CHILDREN_PER_NODE);

	/* leaf layer has already been set up */
	t->counts[t->depth - 1] = leaf_nodes;
	t->index[t->depth - 1] = t->highs;

	if (t->depth >= 2)
		r = setup_indexes(t);

	return r;
}

<<<<<<< HEAD
/*
 * Get a disk whose integrity profile reflects the table's profile.
 * If %match_all is true, all devices' profiles must match.
 * If %match_all is false, all devices must at least have an
 * allocated integrity profile; but uninitialized is ok.
 * Returns NULL if integrity support was inconsistent or unavailable.
 */
static struct gendisk * dm_table_get_integrity_disk(struct dm_table *t,
						    bool match_all)
=======
static bool integrity_profile_exists(struct gendisk *disk)
{
	return !!blk_get_integrity(disk);
}

/*
 * Get a disk whose integrity profile reflects the table's profile.
 * Returns NULL if integrity support was inconsistent or unavailable.
 */
static struct gendisk *dm_table_get_integrity_disk(struct dm_table *t)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
{
	struct list_head *devices = dm_table_get_devices(t);
	struct dm_dev_internal *dd = NULL;
	struct gendisk *prev_disk = NULL, *template_disk = NULL;

<<<<<<< HEAD
	list_for_each_entry(dd, devices, list) {
		template_disk = dd->dm_dev.bdev->bd_disk;
		if (!blk_get_integrity(template_disk))
			goto no_integrity;
		if (!match_all && !blk_integrity_is_initialized(template_disk))
			continue; /* skip uninitialized profiles */
=======
	for (unsigned int i = 0; i < t->num_targets; i++) {
		struct dm_target *ti = dm_table_get_target(t, i);

		if (!dm_target_passes_integrity(ti->type))
			goto no_integrity;
	}

	list_for_each_entry(dd, devices, list) {
		template_disk = dd->dm_dev->bdev->bd_disk;
		if (!integrity_profile_exists(template_disk))
			goto no_integrity;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		else if (prev_disk &&
			 blk_integrity_compare(prev_disk, template_disk) < 0)
			goto no_integrity;
		prev_disk = template_disk;
	}

	return template_disk;

no_integrity:
	if (prev_disk)
		DMWARN("%s: integrity not set: %s and %s profile mismatch",
		       dm_device_name(t->md),
		       prev_disk->disk_name,
		       template_disk->disk_name);
	return NULL;
}

/*
<<<<<<< HEAD
 * Register the mapped device for blk_integrity support if
 * the underlying devices have an integrity profile.  But all devices
 * may not have matching profiles (checking all devices isn't reliable
 * during table load because this table may use other DM device(s) which
 * must be resumed before they will have an initialized integity profile).
 * Stacked DM devices force a 2 stage integrity profile validation:
 * 1 - during load, validate all initialized integrity profiles match
 * 2 - during resume, validate all integrity profiles match
 */
static int dm_table_prealloc_integrity(struct dm_table *t, struct mapped_device *md)
{
	struct gendisk *template_disk = NULL;

	template_disk = dm_table_get_integrity_disk(t, false);
	if (!template_disk)
		return 0;

	if (!blk_integrity_is_initialized(dm_disk(md))) {
		t->integrity_supported = 1;
		return blk_integrity_register(dm_disk(md), NULL);
	}

	/*
	 * If DM device already has an initalized integrity
	 * profile the new profile should not conflict.
	 */
	if (blk_integrity_is_initialized(template_disk) &&
	    blk_integrity_compare(dm_disk(md), template_disk) < 0) {
		DMWARN("%s: conflict with existing integrity profile: "
		       "%s profile mismatch",
		       dm_device_name(t->md),
		       template_disk->disk_name);
		return 1;
	}

	/* Preserve existing initialized integrity profile */
	t->integrity_supported = 1;
	return 0;
}

=======
 * Register the mapped device for blk_integrity support if the
 * underlying devices have an integrity profile.  But all devices may
 * not have matching profiles (checking all devices isn't reliable
 * during table load because this table may use other DM device(s) which
 * must be resumed before they will have an initialized integity
 * profile).  Consequently, stacked DM devices force a 2 stage integrity
 * profile validation: First pass during table load, final pass during
 * resume.
 */
static int dm_table_register_integrity(struct dm_table *t)
{
	struct mapped_device *md = t->md;
	struct gendisk *template_disk = NULL;

	/* If target handles integrity itself do not register it here. */
	if (t->integrity_added)
		return 0;

	template_disk = dm_table_get_integrity_disk(t);
	if (!template_disk)
		return 0;

	if (!integrity_profile_exists(dm_disk(md))) {
		t->integrity_supported = true;
		/*
		 * Register integrity profile during table load; we can do
		 * this because the final profile must match during resume.
		 */
		blk_integrity_register(dm_disk(md),
				       blk_get_integrity(template_disk));
		return 0;
	}

	/*
	 * If DM device already has an initialized integrity
	 * profile the new profile should not conflict.
	 */
	if (blk_integrity_compare(dm_disk(md), template_disk) < 0) {
		DMERR("%s: conflict with existing integrity profile: %s profile mismatch",
		      dm_device_name(t->md),
		      template_disk->disk_name);
		return 1;
	}

	/* Preserve existing integrity profile */
	t->integrity_supported = true;
	return 0;
}

#ifdef CONFIG_BLK_INLINE_ENCRYPTION

struct dm_crypto_profile {
	struct blk_crypto_profile profile;
	struct mapped_device *md;
};

static int dm_keyslot_evict_callback(struct dm_target *ti, struct dm_dev *dev,
				     sector_t start, sector_t len, void *data)
{
	const struct blk_crypto_key *key = data;

	blk_crypto_evict_key(dev->bdev, key);
	return 0;
}

/*
 * When an inline encryption key is evicted from a device-mapper device, evict
 * it from all the underlying devices.
 */
static int dm_keyslot_evict(struct blk_crypto_profile *profile,
			    const struct blk_crypto_key *key, unsigned int slot)
{
	struct mapped_device *md =
		container_of(profile, struct dm_crypto_profile, profile)->md;
	struct dm_table *t;
	int srcu_idx;

	t = dm_get_live_table(md, &srcu_idx);
	if (!t)
		return 0;

	for (unsigned int i = 0; i < t->num_targets; i++) {
		struct dm_target *ti = dm_table_get_target(t, i);

		if (!ti->type->iterate_devices)
			continue;
		ti->type->iterate_devices(ti, dm_keyslot_evict_callback,
					  (void *)key);
	}

	dm_put_live_table(md, srcu_idx);
	return 0;
}

static int
device_intersect_crypto_capabilities(struct dm_target *ti, struct dm_dev *dev,
				     sector_t start, sector_t len, void *data)
{
	struct blk_crypto_profile *parent = data;
	struct blk_crypto_profile *child =
		bdev_get_queue(dev->bdev)->crypto_profile;

	blk_crypto_intersect_capabilities(parent, child);
	return 0;
}

void dm_destroy_crypto_profile(struct blk_crypto_profile *profile)
{
	struct dm_crypto_profile *dmcp = container_of(profile,
						      struct dm_crypto_profile,
						      profile);

	if (!profile)
		return;

	blk_crypto_profile_destroy(profile);
	kfree(dmcp);
}

static void dm_table_destroy_crypto_profile(struct dm_table *t)
{
	dm_destroy_crypto_profile(t->crypto_profile);
	t->crypto_profile = NULL;
}

/*
 * Constructs and initializes t->crypto_profile with a crypto profile that
 * represents the common set of crypto capabilities of the devices described by
 * the dm_table.  However, if the constructed crypto profile doesn't support all
 * crypto capabilities that are supported by the current mapped_device, it
 * returns an error instead, since we don't support removing crypto capabilities
 * on table changes.  Finally, if the constructed crypto profile is "empty" (has
 * no crypto capabilities at all), it just sets t->crypto_profile to NULL.
 */
static int dm_table_construct_crypto_profile(struct dm_table *t)
{
	struct dm_crypto_profile *dmcp;
	struct blk_crypto_profile *profile;
	unsigned int i;
	bool empty_profile = true;

	dmcp = kmalloc(sizeof(*dmcp), GFP_KERNEL);
	if (!dmcp)
		return -ENOMEM;
	dmcp->md = t->md;

	profile = &dmcp->profile;
	blk_crypto_profile_init(profile, 0);
	profile->ll_ops.keyslot_evict = dm_keyslot_evict;
	profile->max_dun_bytes_supported = UINT_MAX;
	memset(profile->modes_supported, 0xFF,
	       sizeof(profile->modes_supported));

	for (i = 0; i < t->num_targets; i++) {
		struct dm_target *ti = dm_table_get_target(t, i);

		if (!dm_target_passes_crypto(ti->type)) {
			blk_crypto_intersect_capabilities(profile, NULL);
			break;
		}
		if (!ti->type->iterate_devices)
			continue;
		ti->type->iterate_devices(ti,
					  device_intersect_crypto_capabilities,
					  profile);
	}

	if (t->md->queue &&
	    !blk_crypto_has_capabilities(profile,
					 t->md->queue->crypto_profile)) {
		DMERR("Inline encryption capabilities of new DM table were more restrictive than the old table's. This is not supported!");
		dm_destroy_crypto_profile(profile);
		return -EINVAL;
	}

	/*
	 * If the new profile doesn't actually support any crypto capabilities,
	 * we may as well represent it with a NULL profile.
	 */
	for (i = 0; i < ARRAY_SIZE(profile->modes_supported); i++) {
		if (profile->modes_supported[i]) {
			empty_profile = false;
			break;
		}
	}

	if (empty_profile) {
		dm_destroy_crypto_profile(profile);
		profile = NULL;
	}

	/*
	 * t->crypto_profile is only set temporarily while the table is being
	 * set up, and it gets set to NULL after the profile has been
	 * transferred to the request_queue.
	 */
	t->crypto_profile = profile;

	return 0;
}

static void dm_update_crypto_profile(struct request_queue *q,
				     struct dm_table *t)
{
	if (!t->crypto_profile)
		return;

	/* Make the crypto profile less restrictive. */
	if (!q->crypto_profile) {
		blk_crypto_register(t->crypto_profile, q);
	} else {
		blk_crypto_update_capabilities(q->crypto_profile,
					       t->crypto_profile);
		dm_destroy_crypto_profile(t->crypto_profile);
	}
	t->crypto_profile = NULL;
}

#else /* CONFIG_BLK_INLINE_ENCRYPTION */

static int dm_table_construct_crypto_profile(struct dm_table *t)
{
	return 0;
}

void dm_destroy_crypto_profile(struct blk_crypto_profile *profile)
{
}

static void dm_table_destroy_crypto_profile(struct dm_table *t)
{
}

static void dm_update_crypto_profile(struct request_queue *q,
				     struct dm_table *t)
{
}

#endif /* !CONFIG_BLK_INLINE_ENCRYPTION */

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
/*
 * Prepares the table for use by building the indices,
 * setting the type, and allocating mempools.
 */
int dm_table_complete(struct dm_table *t)
{
	int r;

<<<<<<< HEAD
	r = dm_table_set_type(t);
	if (r) {
		DMERR("unable to set table type");
=======
	r = dm_table_determine_type(t);
	if (r) {
		DMERR("unable to determine table type");
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		return r;
	}

	r = dm_table_build_index(t);
	if (r) {
		DMERR("unable to build btrees");
		return r;
	}

<<<<<<< HEAD
	r = dm_table_prealloc_integrity(t, t->md);
=======
	r = dm_table_register_integrity(t);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	if (r) {
		DMERR("could not register integrity profile.");
		return r;
	}

<<<<<<< HEAD
	r = dm_table_alloc_md_mempools(t);
=======
	r = dm_table_construct_crypto_profile(t);
	if (r) {
		DMERR("could not construct crypto profile.");
		return r;
	}

	r = dm_table_alloc_md_mempools(t, t->md);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	if (r)
		DMERR("unable to allocate mempools");

	return r;
}

static DEFINE_MUTEX(_event_lock);
void dm_table_event_callback(struct dm_table *t,
			     void (*fn)(void *), void *context)
{
	mutex_lock(&_event_lock);
	t->event_fn = fn;
	t->event_context = context;
	mutex_unlock(&_event_lock);
}

void dm_table_event(struct dm_table *t)
{
<<<<<<< HEAD
	/*
	 * You can no longer call dm_table_event() from interrupt
	 * context, use a bottom half instead.
	 */
	BUG_ON(in_interrupt());

=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	mutex_lock(&_event_lock);
	if (t->event_fn)
		t->event_fn(t->event_context);
	mutex_unlock(&_event_lock);
}
EXPORT_SYMBOL(dm_table_event);

<<<<<<< HEAD
sector_t dm_table_get_size(struct dm_table *t)
=======
inline sector_t dm_table_get_size(struct dm_table *t)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
{
	return t->num_targets ? (t->highs[t->num_targets - 1] + 1) : 0;
}
EXPORT_SYMBOL(dm_table_get_size);

<<<<<<< HEAD
struct dm_target *dm_table_get_target(struct dm_table *t, unsigned int index)
{
	if (index >= t->num_targets)
		return NULL;

	return t->targets + index;
}

/*
 * Search the btree for the correct target.
 *
 * Caller should check returned pointer with dm_target_is_valid()
=======
/*
 * Search the btree for the correct target.
 *
 * Caller should check returned pointer for NULL
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 * to trap I/O beyond end of device.
 */
struct dm_target *dm_table_find_target(struct dm_table *t, sector_t sector)
{
	unsigned int l, n = 0, k = 0;
	sector_t *node;

<<<<<<< HEAD
=======
	if (unlikely(sector >= dm_table_get_size(t)))
		return NULL;

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	for (l = 0; l < t->depth; l++) {
		n = get_child(n, k);
		node = get_node(t, l, n);

		for (k = 0; k < KEYS_PER_NODE; k++)
			if (node[k] >= sector)
				break;
	}

	return &t->targets[(KEYS_PER_NODE * n) + k];
}

<<<<<<< HEAD
/*
 * Establish the new table's queue_limits and validate them.
 */
int dm_calculate_queue_limits(struct dm_table *table,
			      struct queue_limits *limits)
{
	struct dm_target *uninitialized_var(ti);
	struct queue_limits ti_limits;
	unsigned i = 0;

	blk_set_stacking_limits(limits);

	while (i < dm_table_get_num_targets(table)) {
		blk_set_stacking_limits(&ti_limits);

		ti = dm_table_get_target(table, i++);

		if (!ti->type->iterate_devices)
			goto combine_limits;
=======
static int device_not_poll_capable(struct dm_target *ti, struct dm_dev *dev,
				   sector_t start, sector_t len, void *data)
{
	struct request_queue *q = bdev_get_queue(dev->bdev);

	return !test_bit(QUEUE_FLAG_POLL, &q->queue_flags);
}

/*
 * type->iterate_devices() should be called when the sanity check needs to
 * iterate and check all underlying data devices. iterate_devices() will
 * iterate all underlying data devices until it encounters a non-zero return
 * code, returned by whether the input iterate_devices_callout_fn, or
 * iterate_devices() itself internally.
 *
 * For some target type (e.g. dm-stripe), one call of iterate_devices() may
 * iterate multiple underlying devices internally, in which case a non-zero
 * return code returned by iterate_devices_callout_fn will stop the iteration
 * in advance.
 *
 * Cases requiring _any_ underlying device supporting some kind of attribute,
 * should use the iteration structure like dm_table_any_dev_attr(), or call
 * it directly. @func should handle semantics of positive examples, e.g.
 * capable of something.
 *
 * Cases requiring _all_ underlying devices supporting some kind of attribute,
 * should use the iteration structure like dm_table_supports_nowait() or
 * dm_table_supports_discards(). Or introduce dm_table_all_devs_attr() that
 * uses an @anti_func that handle semantics of counter examples, e.g. not
 * capable of something. So: return !dm_table_any_dev_attr(t, anti_func, data);
 */
static bool dm_table_any_dev_attr(struct dm_table *t,
				  iterate_devices_callout_fn func, void *data)
{
	for (unsigned int i = 0; i < t->num_targets; i++) {
		struct dm_target *ti = dm_table_get_target(t, i);

		if (ti->type->iterate_devices &&
		    ti->type->iterate_devices(ti, func, data))
			return true;
	}

	return false;
}

static int count_device(struct dm_target *ti, struct dm_dev *dev,
			sector_t start, sector_t len, void *data)
{
	unsigned int *num_devices = data;

	(*num_devices)++;

	return 0;
}

static bool dm_table_supports_poll(struct dm_table *t)
{
	for (unsigned int i = 0; i < t->num_targets; i++) {
		struct dm_target *ti = dm_table_get_target(t, i);

		if (!ti->type->iterate_devices ||
		    ti->type->iterate_devices(ti, device_not_poll_capable, NULL))
			return false;
	}

	return true;
}

/*
 * Check whether a table has no data devices attached using each
 * target's iterate_devices method.
 * Returns false if the result is unknown because a target doesn't
 * support iterate_devices.
 */
bool dm_table_has_no_data_devices(struct dm_table *t)
{
	for (unsigned int i = 0; i < t->num_targets; i++) {
		struct dm_target *ti = dm_table_get_target(t, i);
		unsigned int num_devices = 0;

		if (!ti->type->iterate_devices)
			return false;

		ti->type->iterate_devices(ti, count_device, &num_devices);
		if (num_devices)
			return false;
	}

	return true;
}

static int device_not_zoned(struct dm_target *ti, struct dm_dev *dev,
			    sector_t start, sector_t len, void *data)
{
	bool *zoned = data;

	return bdev_is_zoned(dev->bdev) != *zoned;
}

static int device_is_zoned_model(struct dm_target *ti, struct dm_dev *dev,
				 sector_t start, sector_t len, void *data)
{
	return bdev_is_zoned(dev->bdev);
}

/*
 * Check the device zoned model based on the target feature flag. If the target
 * has the DM_TARGET_ZONED_HM feature flag set, host-managed zoned devices are
 * also accepted but all devices must have the same zoned model. If the target
 * has the DM_TARGET_MIXED_ZONED_MODEL feature set, the devices can have any
 * zoned model with all zoned devices having the same zone size.
 */
static bool dm_table_supports_zoned(struct dm_table *t, bool zoned)
{
	for (unsigned int i = 0; i < t->num_targets; i++) {
		struct dm_target *ti = dm_table_get_target(t, i);

		/*
		 * For the wildcard target (dm-error), if we do not have a
		 * backing device, we must always return false. If we have a
		 * backing device, the result must depend on checking zoned
		 * model, like for any other target. So for this, check directly
		 * if the target backing device is zoned as we get "false" when
		 * dm-error was set without a backing device.
		 */
		if (dm_target_is_wildcard(ti->type) &&
		    !ti->type->iterate_devices(ti, device_is_zoned_model, NULL))
			return false;

		if (dm_target_supports_zoned_hm(ti->type)) {
			if (!ti->type->iterate_devices ||
			    ti->type->iterate_devices(ti, device_not_zoned,
						      &zoned))
				return false;
		} else if (!dm_target_supports_mixed_zoned_model(ti->type)) {
			if (zoned)
				return false;
		}
	}

	return true;
}

static int device_not_matches_zone_sectors(struct dm_target *ti, struct dm_dev *dev,
					   sector_t start, sector_t len, void *data)
{
	unsigned int *zone_sectors = data;

	if (!bdev_is_zoned(dev->bdev))
		return 0;
	return bdev_zone_sectors(dev->bdev) != *zone_sectors;
}

/*
 * Check consistency of zoned model and zone sectors across all targets. For
 * zone sectors, if the destination device is a zoned block device, it shall
 * have the specified zone_sectors.
 */
static int validate_hardware_zoned(struct dm_table *t, bool zoned,
				   unsigned int zone_sectors)
{
	if (!zoned)
		return 0;

	if (!dm_table_supports_zoned(t, zoned)) {
		DMERR("%s: zoned model is not consistent across all devices",
		      dm_device_name(t->md));
		return -EINVAL;
	}

	/* Check zone size validity and compatibility */
	if (!zone_sectors || !is_power_of_2(zone_sectors))
		return -EINVAL;

	if (dm_table_any_dev_attr(t, device_not_matches_zone_sectors, &zone_sectors)) {
		DMERR("%s: zone sectors is not consistent across all zoned devices",
		      dm_device_name(t->md));
		return -EINVAL;
	}

	return 0;
}

/*
 * Establish the new table's queue_limits and validate them.
 */
int dm_calculate_queue_limits(struct dm_table *t,
			      struct queue_limits *limits)
{
	struct queue_limits ti_limits;
	unsigned int zone_sectors = 0;
	bool zoned = false;

	blk_set_stacking_limits(limits);

	for (unsigned int i = 0; i < t->num_targets; i++) {
		struct dm_target *ti = dm_table_get_target(t, i);

		blk_set_stacking_limits(&ti_limits);

		if (!ti->type->iterate_devices) {
			/* Set I/O hints portion of queue limits */
			if (ti->type->io_hints)
				ti->type->io_hints(ti, &ti_limits);
			goto combine_limits;
		}
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

		/*
		 * Combine queue limits of all the devices this target uses.
		 */
		ti->type->iterate_devices(ti, dm_set_device_limits,
					  &ti_limits);

<<<<<<< HEAD
=======
		if (!zoned && ti_limits.zoned) {
			/*
			 * After stacking all limits, validate all devices
			 * in table support this zoned model and zone sectors.
			 */
			zoned = ti_limits.zoned;
			zone_sectors = ti_limits.chunk_sectors;
		}

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		/* Set I/O hints portion of queue limits */
		if (ti->type->io_hints)
			ti->type->io_hints(ti, &ti_limits);

		/*
		 * Check each device area is consistent with the target's
		 * overall queue limits.
		 */
		if (ti->type->iterate_devices(ti, device_area_is_invalid,
					      &ti_limits))
			return -EINVAL;

combine_limits:
		/*
		 * Merge this target's queue limits into the overall limits
		 * for the table.
		 */
		if (blk_stack_limits(limits, &ti_limits, 0) < 0)
<<<<<<< HEAD
			DMWARN("%s: adding target device "
			       "(start sect %llu len %llu) "
			       "caused an alignment inconsistency",
			       dm_device_name(table->md),
=======
			DMWARN("%s: adding target device (start sect %llu len %llu) "
			       "caused an alignment inconsistency",
			       dm_device_name(t->md),
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
			       (unsigned long long) ti->begin,
			       (unsigned long long) ti->len);
	}

<<<<<<< HEAD
	return validate_hardware_logical_block_alignment(table, limits);
}

/*
 * Set the integrity profile for this device if all devices used have
 * matching profiles.  We're quite deep in the resume path but still
 * don't know if all devices (particularly DM devices this device
 * may be stacked on) have matching profiles.  Even if the profiles
 * don't match we have no way to fail (to resume) at this point.
 */
static void dm_table_set_integrity(struct dm_table *t)
{
	struct gendisk *template_disk = NULL;

	if (!blk_get_integrity(dm_disk(t->md)))
		return;

	template_disk = dm_table_get_integrity_disk(t, true);
	if (template_disk)
		blk_integrity_register(dm_disk(t->md),
				       blk_get_integrity(template_disk));
	else if (blk_integrity_is_initialized(dm_disk(t->md)))
		DMWARN("%s: device no longer has a valid integrity profile",
		       dm_device_name(t->md));
	else
		DMWARN("%s: unable to establish an integrity profile",
		       dm_device_name(t->md));
=======
	/*
	 * Verify that the zoned model and zone sectors, as determined before
	 * any .io_hints override, are the same across all devices in the table.
	 * - this is especially relevant if .io_hints is emulating a disk-managed
	 *   zoned model on host-managed zoned block devices.
	 * BUT...
	 */
	if (limits->zoned) {
		/*
		 * ...IF the above limits stacking determined a zoned model
		 * validate that all of the table's devices conform to it.
		 */
		zoned = limits->zoned;
		zone_sectors = limits->chunk_sectors;
	}
	if (validate_hardware_zoned(t, zoned, zone_sectors))
		return -EINVAL;

	return validate_hardware_logical_block_alignment(t, limits);
}

/*
 * Verify that all devices have an integrity profile that matches the
 * DM device's registered integrity profile.  If the profiles don't
 * match then unregister the DM device's integrity profile.
 */
static void dm_table_verify_integrity(struct dm_table *t)
{
	struct gendisk *template_disk = NULL;

	if (t->integrity_added)
		return;

	if (t->integrity_supported) {
		/*
		 * Verify that the original integrity profile
		 * matches all the devices in this table.
		 */
		template_disk = dm_table_get_integrity_disk(t);
		if (template_disk &&
		    blk_integrity_compare(dm_disk(t->md), template_disk) >= 0)
			return;
	}

	if (integrity_profile_exists(dm_disk(t->md))) {
		DMWARN("%s: unable to establish an integrity profile",
		       dm_device_name(t->md));
		blk_integrity_unregister(dm_disk(t->md));
	}
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

static int device_flush_capable(struct dm_target *ti, struct dm_dev *dev,
				sector_t start, sector_t len, void *data)
{
<<<<<<< HEAD
	unsigned flush = (*(unsigned *)data);
	struct request_queue *q = bdev_get_queue(dev->bdev);

	return q && (q->flush_flags & flush);
}

static bool dm_table_supports_flush(struct dm_table *t, unsigned flush)
{
	struct dm_target *ti;
	unsigned i = 0;

=======
	unsigned long flush = (unsigned long) data;
	struct request_queue *q = bdev_get_queue(dev->bdev);

	return (q->queue_flags & flush);
}

static bool dm_table_supports_flush(struct dm_table *t, unsigned long flush)
{
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	/*
	 * Require at least one underlying device to support flushes.
	 * t->devices includes internal dm devices such as mirror logs
	 * so we need to use iterate_devices here, which targets
	 * supporting flushes must provide.
	 */
<<<<<<< HEAD
	while (i < dm_table_get_num_targets(t)) {
		ti = dm_table_get_target(t, i++);

		if (!ti->num_flush_requests)
			continue;

		if (ti->type->iterate_devices &&
		    ti->type->iterate_devices(ti, device_flush_capable, &flush))
			return 1;
	}

	return 0;
}

static bool dm_table_discard_zeroes_data(struct dm_table *t)
{
	struct dm_target *ti;
	unsigned i = 0;

	/* Ensure that all targets supports discard_zeroes_data. */
	while (i < dm_table_get_num_targets(t)) {
		ti = dm_table_get_target(t, i++);

		if (ti->discard_zeroes_data_unsupported)
			return 0;
	}

	return 1;
}

static int device_is_nonrot(struct dm_target *ti, struct dm_dev *dev,
			    sector_t start, sector_t len, void *data)
{
	struct request_queue *q = bdev_get_queue(dev->bdev);

	return q && blk_queue_nonrot(q);
=======
	for (unsigned int i = 0; i < t->num_targets; i++) {
		struct dm_target *ti = dm_table_get_target(t, i);

		if (!ti->num_flush_bios)
			continue;

		if (ti->flush_supported)
			return true;

		if (ti->type->iterate_devices &&
		    ti->type->iterate_devices(ti, device_flush_capable, (void *) flush))
			return true;
	}

	return false;
}

static int device_dax_write_cache_enabled(struct dm_target *ti,
					  struct dm_dev *dev, sector_t start,
					  sector_t len, void *data)
{
	struct dax_device *dax_dev = dev->dax_dev;

	if (!dax_dev)
		return false;

	if (dax_write_cache_enabled(dax_dev))
		return true;
	return false;
}

static int device_is_rotational(struct dm_target *ti, struct dm_dev *dev,
				sector_t start, sector_t len, void *data)
{
	return !bdev_nonrot(dev->bdev);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

static int device_is_not_random(struct dm_target *ti, struct dm_dev *dev,
			     sector_t start, sector_t len, void *data)
{
	struct request_queue *q = bdev_get_queue(dev->bdev);

<<<<<<< HEAD
	return q && !blk_queue_add_random(q);
}

static bool dm_table_all_devices_attribute(struct dm_table *t,
					   iterate_devices_callout_fn func)
{
	struct dm_target *ti;
	unsigned i = 0;

	while (i < dm_table_get_num_targets(t)) {
		ti = dm_table_get_target(t, i++);

		if (!ti->type->iterate_devices ||
		    !ti->type->iterate_devices(ti, func, NULL))
			return 0;
	}

	return 1;
}

void dm_table_set_restrictions(struct dm_table *t, struct request_queue *q,
			       struct queue_limits *limits)
{
	unsigned flush = 0;
=======
	return !blk_queue_add_random(q);
}

static int device_not_write_zeroes_capable(struct dm_target *ti, struct dm_dev *dev,
					   sector_t start, sector_t len, void *data)
{
	struct request_queue *q = bdev_get_queue(dev->bdev);

	return !q->limits.max_write_zeroes_sectors;
}

static bool dm_table_supports_write_zeroes(struct dm_table *t)
{
	for (unsigned int i = 0; i < t->num_targets; i++) {
		struct dm_target *ti = dm_table_get_target(t, i);

		if (!ti->num_write_zeroes_bios)
			return false;

		if (!ti->type->iterate_devices ||
		    ti->type->iterate_devices(ti, device_not_write_zeroes_capable, NULL))
			return false;
	}

	return true;
}

static int device_not_nowait_capable(struct dm_target *ti, struct dm_dev *dev,
				     sector_t start, sector_t len, void *data)
{
	return !bdev_nowait(dev->bdev);
}

static bool dm_table_supports_nowait(struct dm_table *t)
{
	for (unsigned int i = 0; i < t->num_targets; i++) {
		struct dm_target *ti = dm_table_get_target(t, i);

		if (!dm_target_supports_nowait(ti->type))
			return false;

		if (!ti->type->iterate_devices ||
		    ti->type->iterate_devices(ti, device_not_nowait_capable, NULL))
			return false;
	}

	return true;
}

static int device_not_discard_capable(struct dm_target *ti, struct dm_dev *dev,
				      sector_t start, sector_t len, void *data)
{
	return !bdev_max_discard_sectors(dev->bdev);
}

static bool dm_table_supports_discards(struct dm_table *t)
{
	for (unsigned int i = 0; i < t->num_targets; i++) {
		struct dm_target *ti = dm_table_get_target(t, i);

		if (!ti->num_discard_bios)
			return false;

		/*
		 * Either the target provides discard support (as implied by setting
		 * 'discards_supported') or it relies on _all_ data devices having
		 * discard support.
		 */
		if (!ti->discards_supported &&
		    (!ti->type->iterate_devices ||
		     ti->type->iterate_devices(ti, device_not_discard_capable, NULL)))
			return false;
	}

	return true;
}

static int device_not_secure_erase_capable(struct dm_target *ti,
					   struct dm_dev *dev, sector_t start,
					   sector_t len, void *data)
{
	return !bdev_max_secure_erase_sectors(dev->bdev);
}

static bool dm_table_supports_secure_erase(struct dm_table *t)
{
	for (unsigned int i = 0; i < t->num_targets; i++) {
		struct dm_target *ti = dm_table_get_target(t, i);

		if (!ti->num_secure_erase_bios)
			return false;

		if (!ti->type->iterate_devices ||
		    ti->type->iterate_devices(ti, device_not_secure_erase_capable, NULL))
			return false;
	}

	return true;
}

static int device_requires_stable_pages(struct dm_target *ti,
					struct dm_dev *dev, sector_t start,
					sector_t len, void *data)
{
	return bdev_stable_writes(dev->bdev);
}

int dm_table_set_restrictions(struct dm_table *t, struct request_queue *q,
			      struct queue_limits *limits)
{
	bool wc = false, fua = false;
	int r;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	/*
	 * Copy table's limits to the DM device's request_queue
	 */
	q->limits = *limits;

<<<<<<< HEAD
	if (!dm_table_supports_discards(t))
		queue_flag_clear_unlocked(QUEUE_FLAG_DISCARD, q);
	else
		queue_flag_set_unlocked(QUEUE_FLAG_DISCARD, q);

	if (dm_table_supports_flush(t, REQ_FLUSH)) {
		flush |= REQ_FLUSH;
		if (dm_table_supports_flush(t, REQ_FUA))
			flush |= REQ_FUA;
	}
	blk_queue_flush(q, flush);

	if (!dm_table_discard_zeroes_data(t))
		q->limits.discard_zeroes_data = 0;

	/* Ensure that all underlying devices are non-rotational. */
	if (dm_table_all_devices_attribute(t, device_is_nonrot))
		queue_flag_set_unlocked(QUEUE_FLAG_NONROT, q);
	else
		queue_flag_clear_unlocked(QUEUE_FLAG_NONROT, q);

	dm_table_set_integrity(t);
=======
	if (dm_table_supports_nowait(t))
		blk_queue_flag_set(QUEUE_FLAG_NOWAIT, q);
	else
		blk_queue_flag_clear(QUEUE_FLAG_NOWAIT, q);

	if (!dm_table_supports_discards(t)) {
		q->limits.max_discard_sectors = 0;
		q->limits.max_hw_discard_sectors = 0;
		q->limits.discard_granularity = 0;
		q->limits.discard_alignment = 0;
		q->limits.discard_misaligned = 0;
	}

	if (!dm_table_supports_secure_erase(t))
		q->limits.max_secure_erase_sectors = 0;

	if (dm_table_supports_flush(t, (1UL << QUEUE_FLAG_WC))) {
		wc = true;
		if (dm_table_supports_flush(t, (1UL << QUEUE_FLAG_FUA)))
			fua = true;
	}
	blk_queue_write_cache(q, wc, fua);

	if (dm_table_supports_dax(t, device_not_dax_capable)) {
		blk_queue_flag_set(QUEUE_FLAG_DAX, q);
		if (dm_table_supports_dax(t, device_not_dax_synchronous_capable))
			set_dax_synchronous(t->md->dax_dev);
	} else
		blk_queue_flag_clear(QUEUE_FLAG_DAX, q);

	if (dm_table_any_dev_attr(t, device_dax_write_cache_enabled, NULL))
		dax_write_cache(t->md->dax_dev, true);

	/* Ensure that all underlying devices are non-rotational. */
	if (dm_table_any_dev_attr(t, device_is_rotational, NULL))
		blk_queue_flag_clear(QUEUE_FLAG_NONROT, q);
	else
		blk_queue_flag_set(QUEUE_FLAG_NONROT, q);

	if (!dm_table_supports_write_zeroes(t))
		q->limits.max_write_zeroes_sectors = 0;

	dm_table_verify_integrity(t);

	/*
	 * Some devices don't use blk_integrity but still want stable pages
	 * because they do their own checksumming.
	 * If any underlying device requires stable pages, a table must require
	 * them as well.  Only targets that support iterate_devices are considered:
	 * don't want error, zero, etc to require stable pages.
	 */
	if (dm_table_any_dev_attr(t, device_requires_stable_pages, NULL))
		blk_queue_flag_set(QUEUE_FLAG_STABLE_WRITES, q);
	else
		blk_queue_flag_clear(QUEUE_FLAG_STABLE_WRITES, q);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	/*
	 * Determine whether or not this queue's I/O timings contribute
	 * to the entropy pool, Only request-based targets use this.
	 * Clear QUEUE_FLAG_ADD_RANDOM if any underlying device does not
	 * have it set.
	 */
<<<<<<< HEAD
	if (blk_queue_add_random(q) && dm_table_all_devices_attribute(t, device_is_not_random))
		queue_flag_clear_unlocked(QUEUE_FLAG_ADD_RANDOM, q);

	/*
	 * QUEUE_FLAG_STACKABLE must be set after all queue settings are
	 * visible to other CPUs because, once the flag is set, incoming bios
	 * are processed by request-based dm, which refers to the queue
	 * settings.
	 * Until the flag set, bios are passed to bio-based dm and queued to
	 * md->deferred where queue settings are not needed yet.
	 * Those bios are passed to request-based dm at the resume time.
	 */
	smp_mb();
	if (dm_table_request_based(t))
		queue_flag_set_unlocked(QUEUE_FLAG_STACKABLE, q);
}

unsigned int dm_table_get_num_targets(struct dm_table *t)
{
	return t->num_targets;
=======
	if (blk_queue_add_random(q) &&
	    dm_table_any_dev_attr(t, device_is_not_random, NULL))
		blk_queue_flag_clear(QUEUE_FLAG_ADD_RANDOM, q);

	/*
	 * For a zoned target, setup the zones related queue attributes
	 * and resources necessary for zone append emulation if necessary.
	 */
	if (blk_queue_is_zoned(q)) {
		r = dm_set_zones_restrictions(t, q);
		if (r)
			return r;
		if (!static_key_enabled(&zoned_enabled.key))
			static_branch_enable(&zoned_enabled);
	}

	dm_update_crypto_profile(q, t);
	disk_update_readahead(t->md->disk);

	/*
	 * Check for request-based device is left to
	 * dm_mq_init_request_queue()->blk_mq_init_allocated_queue().
	 *
	 * For bio-based device, only set QUEUE_FLAG_POLL when all
	 * underlying devices supporting polling.
	 */
	if (__table_type_bio_based(t->type)) {
		if (dm_table_supports_poll(t))
			blk_queue_flag_set(QUEUE_FLAG_POLL, q);
		else
			blk_queue_flag_clear(QUEUE_FLAG_POLL, q);
	}

	return 0;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

struct list_head *dm_table_get_devices(struct dm_table *t)
{
	return &t->devices;
}

<<<<<<< HEAD
fmode_t dm_table_get_mode(struct dm_table *t)
=======
blk_mode_t dm_table_get_mode(struct dm_table *t)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
{
	return t->mode;
}
EXPORT_SYMBOL(dm_table_get_mode);

<<<<<<< HEAD
static void suspend_targets(struct dm_table *t, unsigned postsuspend)
{
	int i = t->num_targets;
	struct dm_target *ti = t->targets;

	while (i--) {
		if (postsuspend) {
			if (ti->type->postsuspend)
				ti->type->postsuspend(ti);
		} else if (ti->type->presuspend)
			ti->type->presuspend(ti);

		ti++;
=======
enum suspend_mode {
	PRESUSPEND,
	PRESUSPEND_UNDO,
	POSTSUSPEND,
};

static void suspend_targets(struct dm_table *t, enum suspend_mode mode)
{
	lockdep_assert_held(&t->md->suspend_lock);

	for (unsigned int i = 0; i < t->num_targets; i++) {
		struct dm_target *ti = dm_table_get_target(t, i);

		switch (mode) {
		case PRESUSPEND:
			if (ti->type->presuspend)
				ti->type->presuspend(ti);
			break;
		case PRESUSPEND_UNDO:
			if (ti->type->presuspend_undo)
				ti->type->presuspend_undo(ti);
			break;
		case POSTSUSPEND:
			if (ti->type->postsuspend)
				ti->type->postsuspend(ti);
			break;
		}
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	}
}

void dm_table_presuspend_targets(struct dm_table *t)
{
	if (!t)
		return;

<<<<<<< HEAD
	suspend_targets(t, 0);
=======
	suspend_targets(t, PRESUSPEND);
}

void dm_table_presuspend_undo_targets(struct dm_table *t)
{
	if (!t)
		return;

	suspend_targets(t, PRESUSPEND_UNDO);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

void dm_table_postsuspend_targets(struct dm_table *t)
{
	if (!t)
		return;

<<<<<<< HEAD
	suspend_targets(t, 1);
=======
	suspend_targets(t, POSTSUSPEND);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

int dm_table_resume_targets(struct dm_table *t)
{
<<<<<<< HEAD
	int i, r = 0;

	for (i = 0; i < t->num_targets; i++) {
		struct dm_target *ti = t->targets + i;
=======
	unsigned int i;
	int r = 0;

	lockdep_assert_held(&t->md->suspend_lock);

	for (i = 0; i < t->num_targets; i++) {
		struct dm_target *ti = dm_table_get_target(t, i);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

		if (!ti->type->preresume)
			continue;

		r = ti->type->preresume(ti);
<<<<<<< HEAD
		if (r)
			return r;
	}

	for (i = 0; i < t->num_targets; i++) {
		struct dm_target *ti = t->targets + i;
=======
		if (r) {
			DMERR("%s: %s: preresume failed, error = %d",
			      dm_device_name(t->md), ti->type->name, r);
			return r;
		}
	}

	for (i = 0; i < t->num_targets; i++) {
		struct dm_target *ti = dm_table_get_target(t, i);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

		if (ti->type->resume)
			ti->type->resume(ti);
	}

	return 0;
}

<<<<<<< HEAD
void dm_table_add_target_callbacks(struct dm_table *t, struct dm_target_callbacks *cb)
{
	list_add(&cb->list, &t->target_callbacks);
}
EXPORT_SYMBOL_GPL(dm_table_add_target_callbacks);

int dm_table_any_congested(struct dm_table *t, int bdi_bits)
{
	struct dm_dev_internal *dd;
	struct list_head *devices = dm_table_get_devices(t);
	struct dm_target_callbacks *cb;
	int r = 0;

	list_for_each_entry(dd, devices, list) {
		struct request_queue *q = bdev_get_queue(dd->dm_dev.bdev);
		char b[BDEVNAME_SIZE];

		if (likely(q))
			r |= bdi_congested(&q->backing_dev_info, bdi_bits);
		else
			DMWARN_LIMIT("%s: any_congested: nonexistent device %s",
				     dm_device_name(t->md),
				     bdevname(dd->dm_dev.bdev, b));
	}

	list_for_each_entry(cb, &t->target_callbacks, list)
		if (cb->congested_fn)
			r |= cb->congested_fn(cb, bdi_bits);

	return r;
}

int dm_table_any_busy_target(struct dm_table *t)
{
	unsigned i;
	struct dm_target *ti;

	for (i = 0; i < t->num_targets; i++) {
		ti = t->targets + i;
		if (ti->type->busy && ti->type->busy(ti))
			return 1;
	}

	return 0;
}

=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
struct mapped_device *dm_table_get_md(struct dm_table *t)
{
	return t->md;
}
EXPORT_SYMBOL(dm_table_get_md);

<<<<<<< HEAD
static int device_discard_capable(struct dm_target *ti, struct dm_dev *dev,
				  sector_t start, sector_t len, void *data)
{
	struct request_queue *q = bdev_get_queue(dev->bdev);

	return q && blk_queue_discard(q);
}

bool dm_table_supports_discards(struct dm_table *t)
{
	struct dm_target *ti;
	unsigned i = 0;

	/*
	 * Unless any target used by the table set discards_supported,
	 * require at least one underlying device to support discards.
	 * t->devices includes internal dm devices such as mirror logs
	 * so we need to use iterate_devices here, which targets
	 * supporting discard selectively must provide.
	 */
	while (i < dm_table_get_num_targets(t)) {
		ti = dm_table_get_target(t, i++);

		if (!ti->num_discard_requests)
			continue;

		if (ti->discards_supported)
			return 1;

		if (ti->type->iterate_devices &&
		    ti->type->iterate_devices(ti, device_discard_capable, NULL))
			return 1;
	}

	return 0;
}
=======
const char *dm_table_device_name(struct dm_table *t)
{
	return dm_device_name(t->md);
}
EXPORT_SYMBOL_GPL(dm_table_device_name);

void dm_table_run_md_queue_async(struct dm_table *t)
{
	if (!dm_table_request_based(t))
		return;

	if (t->md->queue)
		blk_mq_run_hw_queues(t->md->queue, true);
}
EXPORT_SYMBOL(dm_table_run_md_queue_async);

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
