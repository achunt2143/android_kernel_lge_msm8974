<<<<<<< HEAD
=======
// SPDX-License-Identifier: GPL-2.0-only
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
/*
 * Copyright (C) 2001-2003 Sistina Software (UK) Limited.
 *
 * This file is released under the GPL.
 */

#include "dm.h"
#include <linux/module.h>
#include <linux/init.h>
#include <linux/blkdev.h>
#include <linux/bio.h>
<<<<<<< HEAD
=======
#include <linux/dax.h>
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#include <linux/slab.h>
#include <linux/device-mapper.h>

#define DM_MSG_PREFIX "linear"

/*
 * Linear: maps a linear range of a device.
 */
struct linear_c {
	struct dm_dev *dev;
	sector_t start;
};

/*
 * Construct a linear mapping: <dev_path> <offset>
 */
static int linear_ctr(struct dm_target *ti, unsigned int argc, char **argv)
{
	struct linear_c *lc;
	unsigned long long tmp;
	char dummy;
<<<<<<< HEAD
=======
	int ret;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	if (argc != 2) {
		ti->error = "Invalid argument count";
		return -EINVAL;
	}

	lc = kmalloc(sizeof(*lc), GFP_KERNEL);
	if (lc == NULL) {
<<<<<<< HEAD
		ti->error = "dm-linear: Cannot allocate linear context";
		return -ENOMEM;
	}

	if (sscanf(argv[1], "%llu%c", &tmp, &dummy) != 1) {
		ti->error = "dm-linear: Invalid device sector";
=======
		ti->error = "Cannot allocate linear context";
		return -ENOMEM;
	}

	ret = -EINVAL;
	if (sscanf(argv[1], "%llu%c", &tmp, &dummy) != 1 || tmp != (sector_t)tmp) {
		ti->error = "Invalid device sector";
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		goto bad;
	}
	lc->start = tmp;

<<<<<<< HEAD
	if (dm_get_device(ti, argv[0], dm_table_get_mode(ti->table), &lc->dev)) {
		ti->error = "dm-linear: Device lookup failed";
		goto bad;
	}

	ti->num_flush_requests = 1;
	ti->num_discard_requests = 1;
	ti->private = lc;
	return 0;

      bad:
	kfree(lc);
	return -EINVAL;
=======
	ret = dm_get_device(ti, argv[0], dm_table_get_mode(ti->table), &lc->dev);
	if (ret) {
		ti->error = "Device lookup failed";
		goto bad;
	}

	ti->num_flush_bios = 1;
	ti->num_discard_bios = 1;
	ti->num_secure_erase_bios = 1;
	ti->num_write_zeroes_bios = 1;
	ti->private = lc;
	return 0;

bad:
	kfree(lc);
	return ret;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

static void linear_dtr(struct dm_target *ti)
{
<<<<<<< HEAD
	struct linear_c *lc = (struct linear_c *) ti->private;
=======
	struct linear_c *lc = ti->private;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	dm_put_device(ti, lc->dev);
	kfree(lc);
}

static sector_t linear_map_sector(struct dm_target *ti, sector_t bi_sector)
{
	struct linear_c *lc = ti->private;

	return lc->start + dm_target_offset(ti, bi_sector);
}

<<<<<<< HEAD
static void linear_map_bio(struct dm_target *ti, struct bio *bio)
{
	struct linear_c *lc = ti->private;

	bio->bi_bdev = lc->dev->bdev;
	if (bio_sectors(bio))
		bio->bi_sector = linear_map_sector(ti, bio->bi_sector);
}

static int linear_map(struct dm_target *ti, struct bio *bio,
		      union map_info *map_context)
{
	linear_map_bio(ti, bio);
=======
int linear_map(struct dm_target *ti, struct bio *bio)
{
	struct linear_c *lc = ti->private;

	bio_set_dev(bio, lc->dev->bdev);
	bio->bi_iter.bi_sector = linear_map_sector(ti, bio->bi_iter.bi_sector);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	return DM_MAPIO_REMAPPED;
}

static void linear_status(struct dm_target *ti, status_type_t type,
<<<<<<< HEAD
			  char *result, unsigned int maxlen)
{
	struct linear_c *lc = (struct linear_c *) ti->private;
=======
			  unsigned int status_flags, char *result, unsigned int maxlen)
{
	struct linear_c *lc = ti->private;
	size_t sz = 0;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	switch (type) {
	case STATUSTYPE_INFO:
		result[0] = '\0';
		break;

	case STATUSTYPE_TABLE:
<<<<<<< HEAD
		snprintf(result, maxlen, "%s %llu", lc->dev->name,
				(unsigned long long)lc->start);
=======
		DMEMIT("%s %llu", lc->dev->name, (unsigned long long)lc->start);
		break;

	case STATUSTYPE_IMA:
		DMEMIT_TARGET_NAME_VERSION(ti->type);
		DMEMIT(",device_name=%s,start=%llu;", lc->dev->name,
		       (unsigned long long)lc->start);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		break;
	}
}

<<<<<<< HEAD
static int linear_ioctl(struct dm_target *ti, unsigned int cmd,
			unsigned long arg)
{
	struct linear_c *lc = (struct linear_c *) ti->private;
	struct dm_dev *dev = lc->dev;
	int r = 0;
=======
static int linear_prepare_ioctl(struct dm_target *ti, struct block_device **bdev)
{
	struct linear_c *lc = ti->private;
	struct dm_dev *dev = lc->dev;

	*bdev = dev->bdev;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	/*
	 * Only pass ioctls through if the device sizes match exactly.
	 */
<<<<<<< HEAD
	if (lc->start ||
	    ti->len != i_size_read(dev->bdev->bd_inode) >> SECTOR_SHIFT)
		r = scsi_verify_blk_ioctl(NULL, cmd);

	return r ? : __blkdev_driver_ioctl(dev->bdev, dev->mode, cmd, arg);
}

static int linear_merge(struct dm_target *ti, struct bvec_merge_data *bvm,
			struct bio_vec *biovec, int max_size)
{
	struct linear_c *lc = ti->private;
	struct request_queue *q = bdev_get_queue(lc->dev->bdev);

	if (!q->merge_bvec_fn)
		return max_size;

	bvm->bi_bdev = lc->dev->bdev;
	bvm->bi_sector = linear_map_sector(ti, bvm->bi_sector);

	return min(max_size, q->merge_bvec_fn(q, bvm, biovec));
}
=======
	if (lc->start || ti->len != bdev_nr_sectors(dev->bdev))
		return 1;
	return 0;
}

#ifdef CONFIG_BLK_DEV_ZONED
static int linear_report_zones(struct dm_target *ti,
		struct dm_report_zones_args *args, unsigned int nr_zones)
{
	struct linear_c *lc = ti->private;

	return dm_report_zones(lc->dev->bdev, lc->start,
			       linear_map_sector(ti, args->next_sector),
			       args, nr_zones);
}
#else
#define linear_report_zones NULL
#endif
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

static int linear_iterate_devices(struct dm_target *ti,
				  iterate_devices_callout_fn fn, void *data)
{
	struct linear_c *lc = ti->private;

	return fn(ti, lc->dev, lc->start, ti->len, data);
}

<<<<<<< HEAD
static struct target_type linear_target = {
	.name   = "linear",
	.version = {1, 1, 0},
=======
#if IS_ENABLED(CONFIG_FS_DAX)
static struct dax_device *linear_dax_pgoff(struct dm_target *ti, pgoff_t *pgoff)
{
	struct linear_c *lc = ti->private;
	sector_t sector = linear_map_sector(ti, *pgoff << PAGE_SECTORS_SHIFT);

	*pgoff = (get_start_sect(lc->dev->bdev) + sector) >> PAGE_SECTORS_SHIFT;
	return lc->dev->dax_dev;
}

static long linear_dax_direct_access(struct dm_target *ti, pgoff_t pgoff,
		long nr_pages, enum dax_access_mode mode, void **kaddr,
		pfn_t *pfn)
{
	struct dax_device *dax_dev = linear_dax_pgoff(ti, &pgoff);

	return dax_direct_access(dax_dev, pgoff, nr_pages, mode, kaddr, pfn);
}

static int linear_dax_zero_page_range(struct dm_target *ti, pgoff_t pgoff,
				      size_t nr_pages)
{
	struct dax_device *dax_dev = linear_dax_pgoff(ti, &pgoff);

	return dax_zero_page_range(dax_dev, pgoff, nr_pages);
}

static size_t linear_dax_recovery_write(struct dm_target *ti, pgoff_t pgoff,
		void *addr, size_t bytes, struct iov_iter *i)
{
	struct dax_device *dax_dev = linear_dax_pgoff(ti, &pgoff);

	return dax_recovery_write(dax_dev, pgoff, addr, bytes, i);
}

#else
#define linear_dax_direct_access NULL
#define linear_dax_zero_page_range NULL
#define linear_dax_recovery_write NULL
#endif

static struct target_type linear_target = {
	.name   = "linear",
	.version = {1, 4, 0},
	.features = DM_TARGET_PASSES_INTEGRITY | DM_TARGET_NOWAIT |
		    DM_TARGET_ZONED_HM | DM_TARGET_PASSES_CRYPTO,
	.report_zones = linear_report_zones,
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	.module = THIS_MODULE,
	.ctr    = linear_ctr,
	.dtr    = linear_dtr,
	.map    = linear_map,
	.status = linear_status,
<<<<<<< HEAD
	.ioctl  = linear_ioctl,
	.merge  = linear_merge,
	.iterate_devices = linear_iterate_devices,
=======
	.prepare_ioctl = linear_prepare_ioctl,
	.iterate_devices = linear_iterate_devices,
	.direct_access = linear_dax_direct_access,
	.dax_zero_page_range = linear_dax_zero_page_range,
	.dax_recovery_write = linear_dax_recovery_write,
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
};

int __init dm_linear_init(void)
{
	int r = dm_register_target(&linear_target);

	if (r < 0)
		DMERR("register failed %d", r);

	return r;
}

void dm_linear_exit(void)
{
	dm_unregister_target(&linear_target);
}
