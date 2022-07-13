<<<<<<< HEAD
/*
 * File...........: linux/drivers/s390/block/dasd_genhd.c
=======
// SPDX-License-Identifier: GPL-2.0
/*
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 * Author(s)......: Holger Smolinski <Holger.Smolinski@de.ibm.com>
 *		    Horst Hummel <Horst.Hummel@de.ibm.com>
 *		    Carsten Otte <Cotte@de.ibm.com>
 *		    Martin Schwidefsky <schwidefsky@de.ibm.com>
 * Bugreports.to..: <Linux390@de.ibm.com>
<<<<<<< HEAD
 * (C) IBM Corporation, IBM Deutschland Entwicklung GmbH, 1999-2001
=======
 * Copyright IBM Corp. 1999, 2001
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 *
 * gendisk related functions for the dasd driver.
 *
 */

<<<<<<< HEAD
#define KMSG_COMPONENT "dasd"

#include <linux/interrupt.h>
#include <linux/fs.h>
#include <linux/blkpg.h>

#include <asm/uaccess.h>

/* This is ugly... */
#define PRINTK_HEADER "dasd_gendisk:"

#include "dasd_int.h"

=======
#include <linux/interrupt.h>
#include <linux/major.h>
#include <linux/fs.h>
#include <linux/blkpg.h>

#include <linux/uaccess.h>

#include "dasd_int.h"

static unsigned int queue_depth = 32;
static unsigned int nr_hw_queues = 4;

module_param(queue_depth, uint, 0444);
MODULE_PARM_DESC(queue_depth, "Default queue depth for new DASD devices");

module_param(nr_hw_queues, uint, 0444);
MODULE_PARM_DESC(nr_hw_queues, "Default number of hardware queues for new DASD devices");

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
/*
 * Allocate and register gendisk structure for device.
 */
int dasd_gendisk_alloc(struct dasd_block *block)
{
<<<<<<< HEAD
	struct gendisk *gdp;
	struct dasd_device *base;
	int len;
=======
	struct queue_limits lim = {
		/*
		 * With page sized segments, each segment can be translated into
		 * one idaw/tidaw.
		 */
		.max_segment_size = PAGE_SIZE,
		.seg_boundary_mask = PAGE_SIZE - 1,
		.dma_alignment = PAGE_SIZE - 1,
		.max_segments = USHRT_MAX,
	};
	struct gendisk *gdp;
	struct dasd_device *base;
	int len, rc;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	/* Make sure the minor for this device exists. */
	base = block->base;
	if (base->devindex >= DASD_PER_MAJOR)
		return -EBUSY;

<<<<<<< HEAD
	gdp = alloc_disk(1 << DASD_PARTN_BITS);
	if (!gdp)
		return -ENOMEM;
=======
	block->tag_set.ops = &dasd_mq_ops;
	block->tag_set.cmd_size = sizeof(struct dasd_ccw_req);
	block->tag_set.nr_hw_queues = nr_hw_queues;
	block->tag_set.queue_depth = queue_depth;
	block->tag_set.flags = BLK_MQ_F_SHOULD_MERGE;
	block->tag_set.numa_node = NUMA_NO_NODE;
	rc = blk_mq_alloc_tag_set(&block->tag_set);
	if (rc)
		return rc;

	gdp = blk_mq_alloc_disk(&block->tag_set, &lim, block);
	if (IS_ERR(gdp)) {
		blk_mq_free_tag_set(&block->tag_set);
		return PTR_ERR(gdp);
	}
	blk_queue_flag_set(QUEUE_FLAG_NONROT, gdp->queue);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	/* Initialize gendisk structure. */
	gdp->major = DASD_MAJOR;
	gdp->first_minor = base->devindex << DASD_PARTN_BITS;
<<<<<<< HEAD
	gdp->fops = &dasd_device_operations;
	gdp->driverfs_dev = &base->cdev->dev;
=======
	gdp->minors = 1 << DASD_PARTN_BITS;
	gdp->fops = &dasd_device_operations;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	/*
	 * Set device name.
	 *   dasda - dasdz : 26 devices
	 *   dasdaa - dasdzz : 676 devices, added up = 702
	 *   dasdaaa - dasdzzz : 17576 devices, added up = 18278
	 *   dasdaaaa - dasdzzzz : 456976 devices, added up = 475252
	 */
	len = sprintf(gdp->disk_name, "dasd");
	if (base->devindex > 25) {
		if (base->devindex > 701) {
			if (base->devindex > 18277)
			        len += sprintf(gdp->disk_name + len, "%c",
					       'a'+(((base->devindex-18278)
						     /17576)%26));
			len += sprintf(gdp->disk_name + len, "%c",
				       'a'+(((base->devindex-702)/676)%26));
		}
		len += sprintf(gdp->disk_name + len, "%c",
			       'a'+(((base->devindex-26)/26)%26));
	}
	len += sprintf(gdp->disk_name + len, "%c", 'a'+(base->devindex%26));

	if (base->features & DASD_FEATURE_READONLY ||
	    test_bit(DASD_FLAG_DEVICE_RO, &base->flags))
		set_disk_ro(gdp, 1);
	dasd_add_link_to_gendisk(gdp, base);
<<<<<<< HEAD
	gdp->queue = block->request_queue;
	block->gdp = gdp;
	set_capacity(block->gdp, 0);
	add_disk(block->gdp);
=======
	block->gdp = gdp;
	set_capacity(block->gdp, 0);

	rc = device_add_disk(&base->cdev->dev, block->gdp, NULL);
	if (rc) {
		dasd_gendisk_free(block);
		return rc;
	}

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	return 0;
}

/*
 * Unregister and free gendisk structure for device.
 */
void dasd_gendisk_free(struct dasd_block *block)
{
	if (block->gdp) {
		del_gendisk(block->gdp);
<<<<<<< HEAD
		block->gdp->queue = NULL;
		block->gdp->private_data = NULL;
		put_disk(block->gdp);
		block->gdp = NULL;
=======
		block->gdp->private_data = NULL;
		put_disk(block->gdp);
		block->gdp = NULL;
		blk_mq_free_tag_set(&block->tag_set);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	}
}

/*
 * Trigger a partition detection.
 */
int dasd_scan_partitions(struct dasd_block *block)
{
<<<<<<< HEAD
	struct block_device *bdev;

	bdev = bdget_disk(block->gdp, 0);
	if (!bdev || blkdev_get(bdev, FMODE_READ, NULL) < 0)
		return -ENODEV;
	/*
	 * See fs/partition/check.c:register_disk,rescan_partitions
	 * Can't call rescan_partitions directly. Use ioctl.
	 */
	ioctl_by_bdev(bdev, BLKRRPART, 0);
	/*
	 * Since the matching blkdev_put call to the blkdev_get in
	 * this function is not called before dasd_destroy_partitions
	 * the offline open_count limit needs to be increased from
	 * 0 to 1. This is done by setting device->bdev (see
	 * dasd_generic_set_offline). As long as the partition
	 * detection is running no offline should be allowed. That
	 * is why the assignment to device->bdev is done AFTER
	 * the BLKRRPART ioctl.
	 */
	block->bdev = bdev;
=======
	struct file *bdev_file;
	int rc;

	bdev_file = bdev_file_open_by_dev(disk_devt(block->gdp), BLK_OPEN_READ,
				       NULL, NULL);
	if (IS_ERR(bdev_file)) {
		DBF_DEV_EVENT(DBF_ERR, block->base,
			      "scan partitions error, blkdev_get returned %ld",
			      PTR_ERR(bdev_file));
		return -ENODEV;
	}

	mutex_lock(&block->gdp->open_mutex);
	rc = bdev_disk_changed(block->gdp, false);
	mutex_unlock(&block->gdp->open_mutex);
	if (rc)
		DBF_DEV_EVENT(DBF_ERR, block->base,
				"scan partitions error, rc %d", rc);

	/*
	 * Since the matching fput() call to the
	 * bdev_file_open_by_path() in this function is not called before
	 * dasd_destroy_partitions the offline open_count limit needs to be
	 * increased from 0 to 1. This is done by setting device->bdev_file
	 * (see dasd_generic_set_offline). As long as the partition detection
	 * is running no offline should be allowed. That is why the assignment
	 * to block->bdev_file is done AFTER the BLKRRPART ioctl.
	 */
	block->bdev_file = bdev_file;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	return 0;
}

/*
 * Remove all inodes in the system for a device, delete the
 * partitions and make device unusable by setting its size to zero.
 */
void dasd_destroy_partitions(struct dasd_block *block)
{
<<<<<<< HEAD
	/* The two structs have 168/176 byte on 31/64 bit. */
	struct blkpg_partition bpart;
	struct blkpg_ioctl_arg barg;
	struct block_device *bdev;

	/*
	 * Get the bdev pointer from the device structure and clear
	 * device->bdev to lower the offline open_count limit again.
	 */
	bdev = block->bdev;
	block->bdev = NULL;

	/*
	 * See fs/partition/check.c:delete_partition
	 * Can't call delete_partitions directly. Use ioctl.
	 * The ioctl also does locking and invalidation.
	 */
	memset(&bpart, 0, sizeof(struct blkpg_partition));
	memset(&barg, 0, sizeof(struct blkpg_ioctl_arg));
	barg.data = (void __force __user *) &bpart;
	barg.op = BLKPG_DEL_PARTITION;
	for (bpart.pno = block->gdp->minors - 1; bpart.pno > 0; bpart.pno--)
		ioctl_by_bdev(bdev, BLKPG, (unsigned long) &barg);

	invalidate_partition(block->gdp, 0);
	/* Matching blkdev_put to the blkdev_get in dasd_scan_partitions. */
	blkdev_put(bdev, FMODE_READ);
	set_capacity(block->gdp, 0);
=======
	struct file *bdev_file;

	/*
	 * Get the bdev_file pointer from the device structure and clear
	 * device->bdev_file to lower the offline open_count limit again.
	 */
	bdev_file = block->bdev_file;
	block->bdev_file = NULL;

	mutex_lock(&file_bdev(bdev_file)->bd_disk->open_mutex);
	bdev_disk_changed(file_bdev(bdev_file)->bd_disk, true);
	mutex_unlock(&file_bdev(bdev_file)->bd_disk->open_mutex);

	/* Matching blkdev_put to the blkdev_get in dasd_scan_partitions. */
	fput(bdev_file);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

int dasd_gendisk_init(void)
{
	int rc;

	/* Register to static dasd major 94 */
	rc = register_blkdev(DASD_MAJOR, "dasd");
	if (rc != 0) {
<<<<<<< HEAD
		pr_warning("Registering the device driver with major number "
			   "%d failed\n", DASD_MAJOR);
=======
		pr_warn("Registering the device driver with major number %d failed\n",
			DASD_MAJOR);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		return rc;
	}
	return 0;
}

void dasd_gendisk_exit(void)
{
	unregister_blkdev(DASD_MAJOR, "dasd");
}
