<<<<<<< HEAD
=======
// SPDX-License-Identifier: GPL-2.0-only
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
/*
 * Ram backed block device driver.
 *
 * Copyright (C) 2007 Nick Piggin
 * Copyright (C) 2007 Novell Inc.
 *
 * Parts derived from drivers/block/rd.c, and drivers/block/loop.c, copyright
 * of their respective owners.
 */

#include <linux/init.h>
<<<<<<< HEAD
=======
#include <linux/initrd.h>
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#include <linux/module.h>
#include <linux/moduleparam.h>
#include <linux/major.h>
#include <linux/blkdev.h>
#include <linux/bio.h>
#include <linux/highmem.h>
#include <linux/mutex.h>
<<<<<<< HEAD
#include <linux/radix-tree.h>
#include <linux/fs.h>
#include <linux/slab.h>

#include <asm/uaccess.h>

#define SECTOR_SHIFT		9
#define PAGE_SECTORS_SHIFT	(PAGE_SHIFT - SECTOR_SHIFT)
#define PAGE_SECTORS		(1 << PAGE_SECTORS_SHIFT)

/*
 * Each block ramdisk device has a radix_tree brd_pages of pages that stores
=======
#include <linux/pagemap.h>
#include <linux/xarray.h>
#include <linux/fs.h>
#include <linux/slab.h>
#include <linux/backing-dev.h>
#include <linux/debugfs.h>

#include <linux/uaccess.h>

/*
 * Each block ramdisk device has a xarray brd_pages of pages that stores
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 * the pages containing the block device's contents. A brd page's ->index is
 * its offset in PAGE_SIZE units. This is similar to, but in no way connected
 * with, the kernel's pagecache or buffer cache (which sit above our block
 * device).
 */
struct brd_device {
<<<<<<< HEAD
	int		brd_number;

	struct request_queue	*brd_queue;
=======
	int			brd_number;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	struct gendisk		*brd_disk;
	struct list_head	brd_list;

	/*
<<<<<<< HEAD
	 * Backing store of pages and lock to protect it. This is the contents
	 * of the block device.
	 */
	spinlock_t		brd_lock;
	struct radix_tree_root	brd_pages;
=======
	 * Backing store of pages. This is the contents of the block device.
	 */
	struct xarray	        brd_pages;
	u64			brd_nr_pages;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
};

/*
 * Look up and return a brd's page for a given sector.
 */
<<<<<<< HEAD
static DEFINE_MUTEX(brd_mutex);
=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
static struct page *brd_lookup_page(struct brd_device *brd, sector_t sector)
{
	pgoff_t idx;
	struct page *page;

<<<<<<< HEAD
	/*
	 * The page lifetime is protected by the fact that we have opened the
	 * device node -- brd pages will never be deleted under us, so we
	 * don't need any further locking or refcounting.
	 *
	 * This is strictly true for the radix-tree nodes as well (ie. we
	 * don't actually need the rcu_read_lock()), however that is not a
	 * documented feature of the radix-tree API so it is better to be
	 * safe here (we don't have total exclusion from radix tree updates
	 * here, only deletes).
	 */
	rcu_read_lock();
	idx = sector >> PAGE_SECTORS_SHIFT; /* sector to page index */
	page = radix_tree_lookup(&brd->brd_pages, idx);
	rcu_read_unlock();
=======
	idx = sector >> PAGE_SECTORS_SHIFT; /* sector to page index */
	page = xa_load(&brd->brd_pages, idx);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	BUG_ON(page && page->index != idx);

	return page;
}

/*
<<<<<<< HEAD
 * Look up and return a brd's page for a given sector.
 * If one does not exist, allocate an empty page, and insert that. Then
 * return it.
 */
static struct page *brd_insert_page(struct brd_device *brd, sector_t sector)
{
	pgoff_t idx;
	struct page *page;
	gfp_t gfp_flags;

	page = brd_lookup_page(brd, sector);
	if (page)
		return page;

	/*
	 * Must use NOIO because we don't want to recurse back into the
	 * block or filesystem layers from page reclaim.
	 *
	 * Cannot support XIP and highmem, because our ->direct_access
	 * routine for XIP must return memory that is always addressable.
	 * If XIP was reworked to use pfns and kmap throughout, this
	 * restriction might be able to be lifted.
	 */
	gfp_flags = GFP_NOIO | __GFP_ZERO;
#ifndef CONFIG_BLK_DEV_XIP
	gfp_flags |= __GFP_HIGHMEM;
#endif
	page = alloc_page(gfp_flags);
	if (!page)
		return NULL;

	if (radix_tree_preload(GFP_NOIO)) {
		__free_page(page);
		return NULL;
	}

	spin_lock(&brd->brd_lock);
	idx = sector >> PAGE_SECTORS_SHIFT;
	page->index = idx;
	if (radix_tree_insert(&brd->brd_pages, idx, page)) {
		__free_page(page);
		page = radix_tree_lookup(&brd->brd_pages, idx);
		BUG_ON(!page);
		BUG_ON(page->index != idx);
	}
	spin_unlock(&brd->brd_lock);

	radix_tree_preload_end();

	return page;
}

static void brd_free_page(struct brd_device *brd, sector_t sector)
{
	struct page *page;
	pgoff_t idx;

	spin_lock(&brd->brd_lock);
	idx = sector >> PAGE_SECTORS_SHIFT;
	page = radix_tree_delete(&brd->brd_pages, idx);
	spin_unlock(&brd->brd_lock);
	if (page)
		__free_page(page);
}

static void brd_zero_page(struct brd_device *brd, sector_t sector)
{
	struct page *page;

	page = brd_lookup_page(brd, sector);
	if (page)
		clear_highpage(page);
}

/*
 * Free all backing store pages and radix tree. This must only be called when
 * there are no other users of the device.
 */
#define FREE_BATCH 16
static void brd_free_pages(struct brd_device *brd)
{
	unsigned long pos = 0;
	struct page *pages[FREE_BATCH];
	int nr_pages;

	do {
		int i;

		nr_pages = radix_tree_gang_lookup(&brd->brd_pages,
				(void **)pages, pos, FREE_BATCH);

		for (i = 0; i < nr_pages; i++) {
			void *ret;

			BUG_ON(pages[i]->index < pos);
			pos = pages[i]->index;
			ret = radix_tree_delete(&brd->brd_pages, pos);
			BUG_ON(!ret || ret != pages[i]);
			__free_page(pages[i]);
		}

		pos++;

		/*
		 * This assumes radix_tree_gang_lookup always returns as
		 * many pages as possible. If the radix-tree code changes,
		 * so will this have to.
		 */
	} while (nr_pages == FREE_BATCH);
=======
 * Insert a new page for a given sector, if one does not already exist.
 */
static int brd_insert_page(struct brd_device *brd, sector_t sector, gfp_t gfp)
{
	pgoff_t idx;
	struct page *page, *cur;
	int ret = 0;

	page = brd_lookup_page(brd, sector);
	if (page)
		return 0;

	page = alloc_page(gfp | __GFP_ZERO | __GFP_HIGHMEM);
	if (!page)
		return -ENOMEM;

	xa_lock(&brd->brd_pages);

	idx = sector >> PAGE_SECTORS_SHIFT;
	page->index = idx;

	cur = __xa_cmpxchg(&brd->brd_pages, idx, NULL, page, gfp);

	if (unlikely(cur)) {
		__free_page(page);
		ret = xa_err(cur);
		if (!ret && (cur->index != idx))
			ret = -EIO;
	} else {
		brd->brd_nr_pages++;
	}

	xa_unlock(&brd->brd_pages);

	return ret;
}

/*
 * Free all backing store pages and xarray. This must only be called when
 * there are no other users of the device.
 */
static void brd_free_pages(struct brd_device *brd)
{
	struct page *page;
	pgoff_t idx;

	xa_for_each(&brd->brd_pages, idx, page) {
		__free_page(page);
		cond_resched();
	}

	xa_destroy(&brd->brd_pages);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

/*
 * copy_to_brd_setup must be called before copy_to_brd. It may sleep.
 */
<<<<<<< HEAD
static int copy_to_brd_setup(struct brd_device *brd, sector_t sector, size_t n)
{
	unsigned int offset = (sector & (PAGE_SECTORS-1)) << SECTOR_SHIFT;
	size_t copy;

	copy = min_t(size_t, n, PAGE_SIZE - offset);
	if (!brd_insert_page(brd, sector))
		return -ENOMEM;
	if (copy < n) {
		sector += copy >> SECTOR_SHIFT;
		if (!brd_insert_page(brd, sector))
			return -ENOMEM;
	}
	return 0;
}

static void discard_from_brd(struct brd_device *brd,
			sector_t sector, size_t n)
{
	while (n >= PAGE_SIZE) {
		/*
		 * Don't want to actually discard pages here because
		 * re-allocating the pages can result in writeback
		 * deadlocks under heavy load.
		 */
		if (0)
			brd_free_page(brd, sector);
		else
			brd_zero_page(brd, sector);
		sector += PAGE_SIZE >> SECTOR_SHIFT;
		n -= PAGE_SIZE;
	}
=======
static int copy_to_brd_setup(struct brd_device *brd, sector_t sector, size_t n,
			     gfp_t gfp)
{
	unsigned int offset = (sector & (PAGE_SECTORS-1)) << SECTOR_SHIFT;
	size_t copy;
	int ret;

	copy = min_t(size_t, n, PAGE_SIZE - offset);
	ret = brd_insert_page(brd, sector, gfp);
	if (ret)
		return ret;
	if (copy < n) {
		sector += copy >> SECTOR_SHIFT;
		ret = brd_insert_page(brd, sector, gfp);
	}
	return ret;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

/*
 * Copy n bytes from src to the brd starting at sector. Does not sleep.
 */
static void copy_to_brd(struct brd_device *brd, const void *src,
			sector_t sector, size_t n)
{
	struct page *page;
	void *dst;
	unsigned int offset = (sector & (PAGE_SECTORS-1)) << SECTOR_SHIFT;
	size_t copy;

	copy = min_t(size_t, n, PAGE_SIZE - offset);
	page = brd_lookup_page(brd, sector);
	BUG_ON(!page);

	dst = kmap_atomic(page);
	memcpy(dst + offset, src, copy);
	kunmap_atomic(dst);

	if (copy < n) {
		src += copy;
		sector += copy >> SECTOR_SHIFT;
		copy = n - copy;
		page = brd_lookup_page(brd, sector);
		BUG_ON(!page);

		dst = kmap_atomic(page);
		memcpy(dst, src, copy);
		kunmap_atomic(dst);
	}
}

/*
 * Copy n bytes to dst from the brd starting at sector. Does not sleep.
 */
static void copy_from_brd(void *dst, struct brd_device *brd,
			sector_t sector, size_t n)
{
	struct page *page;
	void *src;
	unsigned int offset = (sector & (PAGE_SECTORS-1)) << SECTOR_SHIFT;
	size_t copy;

	copy = min_t(size_t, n, PAGE_SIZE - offset);
	page = brd_lookup_page(brd, sector);
	if (page) {
		src = kmap_atomic(page);
		memcpy(dst, src + offset, copy);
		kunmap_atomic(src);
	} else
		memset(dst, 0, copy);

	if (copy < n) {
		dst += copy;
		sector += copy >> SECTOR_SHIFT;
		copy = n - copy;
		page = brd_lookup_page(brd, sector);
		if (page) {
			src = kmap_atomic(page);
			memcpy(dst, src, copy);
			kunmap_atomic(src);
		} else
			memset(dst, 0, copy);
	}
}

/*
 * Process a single bvec of a bio.
 */
static int brd_do_bvec(struct brd_device *brd, struct page *page,
<<<<<<< HEAD
			unsigned int len, unsigned int off, int rw,
=======
			unsigned int len, unsigned int off, blk_opf_t opf,
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
			sector_t sector)
{
	void *mem;
	int err = 0;

<<<<<<< HEAD
	if (rw != READ) {
		err = copy_to_brd_setup(brd, sector, len);
=======
	if (op_is_write(opf)) {
		/*
		 * Must use NOIO because we don't want to recurse back into the
		 * block or filesystem layers from page reclaim.
		 */
		gfp_t gfp = opf & REQ_NOWAIT ? GFP_NOWAIT : GFP_NOIO;

		err = copy_to_brd_setup(brd, sector, len, gfp);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		if (err)
			goto out;
	}

	mem = kmap_atomic(page);
<<<<<<< HEAD
	if (rw == READ) {
=======
	if (!op_is_write(opf)) {
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		copy_from_brd(mem + off, brd, sector, len);
		flush_dcache_page(page);
	} else {
		flush_dcache_page(page);
		copy_to_brd(brd, mem + off, sector, len);
	}
	kunmap_atomic(mem);

out:
	return err;
}

<<<<<<< HEAD
static void brd_make_request(struct request_queue *q, struct bio *bio)
{
	struct block_device *bdev = bio->bi_bdev;
	struct brd_device *brd = bdev->bd_disk->private_data;
	int rw;
	struct bio_vec *bvec;
	sector_t sector;
	int i;
	int err = -EIO;

	sector = bio->bi_sector;
	if (sector + (bio->bi_size >> SECTOR_SHIFT) >
						get_capacity(bdev->bd_disk))
		goto out;

	if (unlikely(bio->bi_rw & REQ_DISCARD)) {
		err = 0;
		discard_from_brd(brd, sector, bio->bi_size);
		goto out;
	}

	rw = bio_rw(bio);
	if (rw == READA)
		rw = READ;

	bio_for_each_segment(bvec, bio, i) {
		unsigned int len = bvec->bv_len;
		err = brd_do_bvec(brd, bvec->bv_page, len,
					bvec->bv_offset, rw, sector);
		if (err)
			break;
		sector += len >> SECTOR_SHIFT;
	}

out:
	bio_endio(bio, err);
}

#ifdef CONFIG_BLK_DEV_XIP
static int brd_direct_access(struct block_device *bdev, sector_t sector,
			void **kaddr, unsigned long *pfn)
{
	struct brd_device *brd = bdev->bd_disk->private_data;
	struct page *page;

	if (!brd)
		return -ENODEV;
	if (sector & (PAGE_SECTORS-1))
		return -EINVAL;
	if (sector + PAGE_SECTORS > get_capacity(bdev->bd_disk))
		return -ERANGE;
	page = brd_insert_page(brd, sector);
	if (!page)
		return -ENOMEM;
	*kaddr = page_address(page);
	*pfn = page_to_pfn(page);

	return 0;
}
#endif

static int brd_ioctl(struct block_device *bdev, fmode_t mode,
			unsigned int cmd, unsigned long arg)
{
	int error;
	struct brd_device *brd = bdev->bd_disk->private_data;

	if (cmd != BLKFLSBUF)
		return -ENOTTY;

	/*
	 * ram device BLKFLSBUF has special semantics, we want to actually
	 * release and destroy the ramdisk data.
	 */
	mutex_lock(&brd_mutex);
	mutex_lock(&bdev->bd_mutex);
	error = -EBUSY;
	if (bdev->bd_openers <= 1) {
		/*
		 * Kill the cache first, so it isn't written back to the
		 * device.
		 *
		 * Another thread might instantiate more buffercache here,
		 * but there is not much we can do to close that race.
		 */
		kill_bdev(bdev);
		brd_free_pages(brd);
		error = 0;
	}
	mutex_unlock(&bdev->bd_mutex);
	mutex_unlock(&brd_mutex);

	return error;
=======
static void brd_submit_bio(struct bio *bio)
{
	struct brd_device *brd = bio->bi_bdev->bd_disk->private_data;
	sector_t sector = bio->bi_iter.bi_sector;
	struct bio_vec bvec;
	struct bvec_iter iter;

	bio_for_each_segment(bvec, bio, iter) {
		unsigned int len = bvec.bv_len;
		int err;

		/* Don't support un-aligned buffer */
		WARN_ON_ONCE((bvec.bv_offset & (SECTOR_SIZE - 1)) ||
				(len & (SECTOR_SIZE - 1)));

		err = brd_do_bvec(brd, bvec.bv_page, len, bvec.bv_offset,
				  bio->bi_opf, sector);
		if (err) {
			if (err == -ENOMEM && bio->bi_opf & REQ_NOWAIT) {
				bio_wouldblock_error(bio);
				return;
			}
			bio_io_error(bio);
			return;
		}
		sector += len >> SECTOR_SHIFT;
	}

	bio_endio(bio);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

static const struct block_device_operations brd_fops = {
	.owner =		THIS_MODULE,
<<<<<<< HEAD
	.ioctl =		brd_ioctl,
#ifdef CONFIG_BLK_DEV_XIP
	.direct_access =	brd_direct_access,
#endif
=======
	.submit_bio =		brd_submit_bio,
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
};

/*
 * And now the modules code and kernel interface.
 */
<<<<<<< HEAD
static int rd_nr;
int rd_size = CONFIG_BLK_DEV_RAM_SIZE;
static int max_part;
static int part_shift;
module_param(rd_nr, int, S_IRUGO);
MODULE_PARM_DESC(rd_nr, "Maximum number of brd devices");
module_param(rd_size, int, S_IRUGO);
MODULE_PARM_DESC(rd_size, "Size of each RAM disk in kbytes.");
module_param(max_part, int, S_IRUGO);
MODULE_PARM_DESC(max_part, "Maximum number of partitions per RAM disk");
=======
static int rd_nr = CONFIG_BLK_DEV_RAM_COUNT;
module_param(rd_nr, int, 0444);
MODULE_PARM_DESC(rd_nr, "Maximum number of brd devices");

unsigned long rd_size = CONFIG_BLK_DEV_RAM_SIZE;
module_param(rd_size, ulong, 0444);
MODULE_PARM_DESC(rd_size, "Size of each RAM disk in kbytes.");

static int max_part = 1;
module_param(max_part, int, 0444);
MODULE_PARM_DESC(max_part, "Num Minors to reserve between devices");

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
MODULE_LICENSE("GPL");
MODULE_ALIAS_BLOCKDEV_MAJOR(RAMDISK_MAJOR);
MODULE_ALIAS("rd");

#ifndef MODULE
/* Legacy boot options - nonmodular */
static int __init ramdisk_size(char *str)
{
	rd_size = simple_strtol(str, NULL, 0);
	return 1;
}
__setup("ramdisk_size=", ramdisk_size);
#endif

/*
 * The device scheme is derived from loop.c. Keep them in synch where possible
 * (should share code eventually).
 */
static LIST_HEAD(brd_devices);
<<<<<<< HEAD
static DEFINE_MUTEX(brd_devices_mutex);

static struct brd_device *brd_alloc(int i)
{
	struct brd_device *brd;
	struct gendisk *disk;

	brd = kzalloc(sizeof(*brd), GFP_KERNEL);
	if (!brd)
		goto out;
	brd->brd_number		= i;
	spin_lock_init(&brd->brd_lock);
	INIT_RADIX_TREE(&brd->brd_pages, GFP_ATOMIC);

	brd->brd_queue = blk_alloc_queue(GFP_KERNEL);
	if (!brd->brd_queue)
		goto out_free_dev;
	blk_queue_make_request(brd->brd_queue, brd_make_request);
	blk_queue_max_hw_sectors(brd->brd_queue, 1024);
	blk_queue_bounce_limit(brd->brd_queue, BLK_BOUNCE_ANY);

	brd->brd_queue->limits.discard_granularity = PAGE_SIZE;
	brd->brd_queue->limits.max_discard_sectors = UINT_MAX;
	brd->brd_queue->limits.discard_zeroes_data = 1;
	queue_flag_set_unlocked(QUEUE_FLAG_DISCARD, brd->brd_queue);

	disk = brd->brd_disk = alloc_disk(1 << part_shift);
	if (!disk)
		goto out_free_queue;
	disk->major		= RAMDISK_MAJOR;
	disk->first_minor	= i << part_shift;
	disk->fops		= &brd_fops;
	disk->private_data	= brd;
	disk->queue		= brd->brd_queue;
	disk->flags |= GENHD_FL_SUPPRESS_PARTITION_INFO;
	sprintf(disk->disk_name, "ram%d", i);
	set_capacity(disk, rd_size * 2);

	return brd;

out_free_queue:
	blk_cleanup_queue(brd->brd_queue);
out_free_dev:
	kfree(brd);
out:
	return NULL;
}

static void brd_free(struct brd_device *brd)
{
	put_disk(brd->brd_disk);
	blk_cleanup_queue(brd->brd_queue);
	brd_free_pages(brd);
	kfree(brd);
}

static struct brd_device *brd_init_one(int i)
{
	struct brd_device *brd;

	list_for_each_entry(brd, &brd_devices, brd_list) {
		if (brd->brd_number == i)
			goto out;
	}

	brd = brd_alloc(i);
	if (brd) {
		add_disk(brd->brd_disk);
		list_add_tail(&brd->brd_list, &brd_devices);
	}
out:
	return brd;
}

static void brd_del_one(struct brd_device *brd)
{
	list_del(&brd->brd_list);
	del_gendisk(brd->brd_disk);
	brd_free(brd);
}

static struct kobject *brd_probe(dev_t dev, int *part, void *data)
{
	struct brd_device *brd;
	struct kobject *kobj;

	mutex_lock(&brd_devices_mutex);
	brd = brd_init_one(MINOR(dev) >> part_shift);
	kobj = brd ? get_disk(brd->brd_disk) : NULL;
	mutex_unlock(&brd_devices_mutex);

	*part = 0;
	return kobj;
=======
static struct dentry *brd_debugfs_dir;

static int brd_alloc(int i)
{
	struct brd_device *brd;
	struct gendisk *disk;
	char buf[DISK_NAME_LEN];
	int err = -ENOMEM;
	struct queue_limits lim = {
		/*
		 * This is so fdisk will align partitions on 4k, because of
		 * direct_access API needing 4k alignment, returning a PFN
		 * (This is only a problem on very small devices <= 4M,
		 *  otherwise fdisk will align on 1M. Regardless this call
		 *  is harmless)
		 */
		.physical_block_size	= PAGE_SIZE,
	};

	list_for_each_entry(brd, &brd_devices, brd_list)
		if (brd->brd_number == i)
			return -EEXIST;
	brd = kzalloc(sizeof(*brd), GFP_KERNEL);
	if (!brd)
		return -ENOMEM;
	brd->brd_number		= i;
	list_add_tail(&brd->brd_list, &brd_devices);

	xa_init(&brd->brd_pages);

	snprintf(buf, DISK_NAME_LEN, "ram%d", i);
	if (!IS_ERR_OR_NULL(brd_debugfs_dir))
		debugfs_create_u64(buf, 0444, brd_debugfs_dir,
				&brd->brd_nr_pages);

	disk = brd->brd_disk = blk_alloc_disk(&lim, NUMA_NO_NODE);
	if (IS_ERR(disk)) {
		err = PTR_ERR(disk);
		goto out_free_dev;
	}
	disk->major		= RAMDISK_MAJOR;
	disk->first_minor	= i * max_part;
	disk->minors		= max_part;
	disk->fops		= &brd_fops;
	disk->private_data	= brd;
	strscpy(disk->disk_name, buf, DISK_NAME_LEN);
	set_capacity(disk, rd_size * 2);
	
	/* Tell the block layer that this is not a rotational device */
	blk_queue_flag_set(QUEUE_FLAG_NONROT, disk->queue);
	blk_queue_flag_set(QUEUE_FLAG_SYNCHRONOUS, disk->queue);
	blk_queue_flag_set(QUEUE_FLAG_NOWAIT, disk->queue);
	err = add_disk(disk);
	if (err)
		goto out_cleanup_disk;

	return 0;

out_cleanup_disk:
	put_disk(disk);
out_free_dev:
	list_del(&brd->brd_list);
	kfree(brd);
	return err;
}

static void brd_probe(dev_t dev)
{
	brd_alloc(MINOR(dev) / max_part);
}

static void brd_cleanup(void)
{
	struct brd_device *brd, *next;

	debugfs_remove_recursive(brd_debugfs_dir);

	list_for_each_entry_safe(brd, next, &brd_devices, brd_list) {
		del_gendisk(brd->brd_disk);
		put_disk(brd->brd_disk);
		brd_free_pages(brd);
		list_del(&brd->brd_list);
		kfree(brd);
	}
}

static inline void brd_check_and_reset_par(void)
{
	if (unlikely(!max_part))
		max_part = 1;

	/*
	 * make sure 'max_part' can be divided exactly by (1U << MINORBITS),
	 * otherwise, it is possiable to get same dev_t when adding partitions.
	 */
	if ((1U << MINORBITS) % max_part != 0)
		max_part = 1UL << fls(max_part);

	if (max_part > DISK_MAX_PARTS) {
		pr_info("brd: max_part can't be larger than %d, reset max_part = %d.\n",
			DISK_MAX_PARTS, DISK_MAX_PARTS);
		max_part = DISK_MAX_PARTS;
	}
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

static int __init brd_init(void)
{
<<<<<<< HEAD
	int i, nr;
	unsigned long range;
	struct brd_device *brd, *next;
=======
	int err, i;

	brd_check_and_reset_par();

	brd_debugfs_dir = debugfs_create_dir("ramdisk_pages", NULL);

	for (i = 0; i < rd_nr; i++) {
		err = brd_alloc(i);
		if (err)
			goto out_free;
	}
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	/*
	 * brd module now has a feature to instantiate underlying device
	 * structure on-demand, provided that there is an access dev node.
<<<<<<< HEAD
	 * However, this will not work well with user space tool that doesn't
	 * know about such "feature".  In order to not break any existing
	 * tool, we do the following:
	 *
	 * (1) if rd_nr is specified, create that many upfront, and this
	 *     also becomes a hard limit.
	 * (2) if rd_nr is not specified, create CONFIG_BLK_DEV_RAM_COUNT
	 *     (default 16) rd device on module load, user can further
	 *     extend brd device by create dev node themselves and have
	 *     kernel automatically instantiate actual device on-demand.
	 */

	part_shift = 0;
	if (max_part > 0) {
		part_shift = fls(max_part);

		/*
		 * Adjust max_part according to part_shift as it is exported
		 * to user space so that user can decide correct minor number
		 * if [s]he want to create more devices.
		 *
		 * Note that -1 is required because partition 0 is reserved
		 * for the whole disk.
		 */
		max_part = (1UL << part_shift) - 1;
	}

	if ((1UL << part_shift) > DISK_MAX_PARTS)
		return -EINVAL;

	if (rd_nr > 1UL << (MINORBITS - part_shift))
		return -EINVAL;

	if (rd_nr) {
		nr = rd_nr;
		range = rd_nr << part_shift;
	} else {
		nr = CONFIG_BLK_DEV_RAM_COUNT;
		range = 1UL << MINORBITS;
	}

	if (register_blkdev(RAMDISK_MAJOR, "ramdisk"))
		return -EIO;

	for (i = 0; i < nr; i++) {
		brd = brd_alloc(i);
		if (!brd)
			goto out_free;
		list_add_tail(&brd->brd_list, &brd_devices);
	}

	/* point of no return */

	list_for_each_entry(brd, &brd_devices, brd_list)
		add_disk(brd->brd_disk);

	blk_register_region(MKDEV(RAMDISK_MAJOR, 0), range,
				  THIS_MODULE, brd_probe, NULL, NULL);

	printk(KERN_INFO "brd: module loaded\n");
	return 0;

out_free:
	list_for_each_entry_safe(brd, next, &brd_devices, brd_list) {
		list_del(&brd->brd_list);
		brd_free(brd);
	}
	unregister_blkdev(RAMDISK_MAJOR, "ramdisk");

	return -ENOMEM;
=======
	 *
	 * (1) if rd_nr is specified, create that many upfront. else
	 *     it defaults to CONFIG_BLK_DEV_RAM_COUNT
	 * (2) User can further extend brd devices by create dev node themselves
	 *     and have kernel automatically instantiate actual device
	 *     on-demand. Example:
	 *		mknod /path/devnod_name b 1 X	# 1 is the rd major
	 *		fdisk -l /path/devnod_name
	 *	If (X / max_part) was not already created it will be created
	 *	dynamically.
	 */

	if (__register_blkdev(RAMDISK_MAJOR, "ramdisk", brd_probe)) {
		err = -EIO;
		goto out_free;
	}

	pr_info("brd: module loaded\n");
	return 0;

out_free:
	brd_cleanup();

	pr_info("brd: module NOT loaded !!!\n");
	return err;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

static void __exit brd_exit(void)
{
<<<<<<< HEAD
	unsigned long range;
	struct brd_device *brd, *next;

	range = rd_nr ? rd_nr << part_shift : 1UL << MINORBITS;

	list_for_each_entry_safe(brd, next, &brd_devices, brd_list)
		brd_del_one(brd);

	blk_unregister_region(MKDEV(RAMDISK_MAJOR, 0), range);
	unregister_blkdev(RAMDISK_MAJOR, "ramdisk");
=======

	unregister_blkdev(RAMDISK_MAJOR, "ramdisk");
	brd_cleanup();

	pr_info("brd: module unloaded\n");
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

module_init(brd_init);
module_exit(brd_exit);

