<<<<<<< HEAD
/*
 * Copyright (C) 2011 STRATO.  All rights reserved.
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public
 * License v2 as published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * General Public License for more details.
 *
 * You should have received a copy of the GNU General Public
 * License along with this program; if not, write to the
 * Free Software Foundation, Inc., 59 Temple Place - Suite 330,
 * Boston, MA 021110-1307, USA.
=======
// SPDX-License-Identifier: GPL-2.0
/*
 * Copyright (C) 2011, 2012 STRATO.  All rights reserved.
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 */

#include <linux/blkdev.h>
#include <linux/ratelimit.h>
<<<<<<< HEAD
#include "ctree.h"
=======
#include <linux/sched/mm.h>
#include <crypto/hash.h>
#include "ctree.h"
#include "discard.h"
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#include "volumes.h"
#include "disk-io.h"
#include "ordered-data.h"
#include "transaction.h"
#include "backref.h"
#include "extent_io.h"
<<<<<<< HEAD
#include "check-integrity.h"
=======
#include "dev-replace.h"
#include "raid56.h"
#include "block-group.h"
#include "zoned.h"
#include "fs.h"
#include "accessors.h"
#include "file-item.h"
#include "scrub.h"
#include "raid-stripe-tree.h"
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

/*
 * This is only the first step towards a full-features scrub. It reads all
 * extent and super block and verifies the checksums. In case a bad checksum
 * is found or the extent cannot be read, good data will be written back if
 * any can be found.
 *
 * Future enhancements:
 *  - In case an unrepairable extent is encountered, track which files are
 *    affected and report them
 *  - track and record media errors, throw out bad devices
 *  - add a mode to also read unallocated space
 */

<<<<<<< HEAD
struct scrub_block;
struct scrub_dev;

#define SCRUB_PAGES_PER_BIO	16	/* 64k per bio */
#define SCRUB_BIOS_PER_DEV	16	/* 1 MB per device in flight */
#define SCRUB_MAX_PAGES_PER_BLOCK	16	/* 64k per node/leaf/sector */

struct scrub_page {
	struct scrub_block	*sblock;
	struct page		*page;
	struct block_device	*bdev;
	u64			flags;  /* extent flags */
	u64			generation;
	u64			logical;
	u64			physical;
	struct {
		unsigned int	mirror_num:8;
		unsigned int	have_csum:1;
		unsigned int	io_error:1;
	};
	u8			csum[BTRFS_CSUM_SIZE];
};

struct scrub_bio {
	int			index;
	struct scrub_dev	*sdev;
	struct bio		*bio;
	int			err;
	u64			logical;
	u64			physical;
	struct scrub_page	*pagev[SCRUB_PAGES_PER_BIO];
	int			page_count;
	int			next_free;
	struct btrfs_work	work;
};

struct scrub_block {
	struct scrub_page	pagev[SCRUB_MAX_PAGES_PER_BLOCK];
	int			page_count;
	atomic_t		outstanding_pages;
	atomic_t		ref_count; /* free mem on transition to zero */
	struct scrub_dev	*sdev;
	struct {
		unsigned int	header_error:1;
		unsigned int	checksum_error:1;
		unsigned int	no_io_error_seen:1;
	};
};

struct scrub_dev {
	struct scrub_bio	*bios[SCRUB_BIOS_PER_DEV];
	struct btrfs_device	*dev;
	int			first_free;
	int			curr;
	atomic_t		in_flight;
	atomic_t		fixup_cnt;
	spinlock_t		list_lock;
	wait_queue_head_t	list_wait;
	u16			csum_size;
	struct list_head	csum_list;
	atomic_t		cancel_req;
	int			readonly;
	int			pages_per_bio; /* <= SCRUB_PAGES_PER_BIO */
	u32			sectorsize;
	u32			nodesize;
	u32			leafsize;
=======
struct scrub_ctx;

/*
 * The following value only influences the performance.
 *
 * This determines how many stripes would be submitted in one go,
 * which is 512KiB (BTRFS_STRIPE_LEN * SCRUB_STRIPES_PER_GROUP).
 */
#define SCRUB_STRIPES_PER_GROUP		8

/*
 * How many groups we have for each sctx.
 *
 * This would be 8M per device, the same value as the old scrub in-flight bios
 * size limit.
 */
#define SCRUB_GROUPS_PER_SCTX		16

#define SCRUB_TOTAL_STRIPES		(SCRUB_GROUPS_PER_SCTX * SCRUB_STRIPES_PER_GROUP)

/*
 * The following value times PAGE_SIZE needs to be large enough to match the
 * largest node/leaf/sector size that shall be supported.
 */
#define SCRUB_MAX_SECTORS_PER_BLOCK	(BTRFS_MAX_METADATA_BLOCKSIZE / SZ_4K)

/* Represent one sector and its needed info to verify the content. */
struct scrub_sector_verification {
	bool is_metadata;

	union {
		/*
		 * Csum pointer for data csum verification.  Should point to a
		 * sector csum inside scrub_stripe::csums.
		 *
		 * NULL if this data sector has no csum.
		 */
		u8 *csum;

		/*
		 * Extra info for metadata verification.  All sectors inside a
		 * tree block share the same generation.
		 */
		u64 generation;
	};
};

enum scrub_stripe_flags {
	/* Set when @mirror_num, @dev, @physical and @logical are set. */
	SCRUB_STRIPE_FLAG_INITIALIZED,

	/* Set when the read-repair is finished. */
	SCRUB_STRIPE_FLAG_REPAIR_DONE,

	/*
	 * Set for data stripes if it's triggered from P/Q stripe.
	 * During such scrub, we should not report errors in data stripes, nor
	 * update the accounting.
	 */
	SCRUB_STRIPE_FLAG_NO_REPORT,
};

#define SCRUB_STRIPE_PAGES		(BTRFS_STRIPE_LEN / PAGE_SIZE)

/*
 * Represent one contiguous range with a length of BTRFS_STRIPE_LEN.
 */
struct scrub_stripe {
	struct scrub_ctx *sctx;
	struct btrfs_block_group *bg;

	struct page *pages[SCRUB_STRIPE_PAGES];
	struct scrub_sector_verification *sectors;

	struct btrfs_device *dev;
	u64 logical;
	u64 physical;

	u16 mirror_num;

	/* Should be BTRFS_STRIPE_LEN / sectorsize. */
	u16 nr_sectors;

	/*
	 * How many data/meta extents are in this stripe.  Only for scrub status
	 * reporting purposes.
	 */
	u16 nr_data_extents;
	u16 nr_meta_extents;

	atomic_t pending_io;
	wait_queue_head_t io_wait;
	wait_queue_head_t repair_wait;

	/*
	 * Indicate the states of the stripe.  Bits are defined in
	 * scrub_stripe_flags enum.
	 */
	unsigned long state;

	/* Indicate which sectors are covered by extent items. */
	unsigned long extent_sector_bitmap;

	/*
	 * The errors hit during the initial read of the stripe.
	 *
	 * Would be utilized for error reporting and repair.
	 *
	 * The remaining init_nr_* records the number of errors hit, only used
	 * by error reporting.
	 */
	unsigned long init_error_bitmap;
	unsigned int init_nr_io_errors;
	unsigned int init_nr_csum_errors;
	unsigned int init_nr_meta_errors;

	/*
	 * The following error bitmaps are all for the current status.
	 * Every time we submit a new read, these bitmaps may be updated.
	 *
	 * error_bitmap = io_error_bitmap | csum_error_bitmap | meta_error_bitmap;
	 *
	 * IO and csum errors can happen for both metadata and data.
	 */
	unsigned long error_bitmap;
	unsigned long io_error_bitmap;
	unsigned long csum_error_bitmap;
	unsigned long meta_error_bitmap;

	/* For writeback (repair or replace) error reporting. */
	unsigned long write_error_bitmap;

	/* Writeback can be concurrent, thus we need to protect the bitmap. */
	spinlock_t write_error_lock;

	/*
	 * Checksum for the whole stripe if this stripe is inside a data block
	 * group.
	 */
	u8 *csums;

	struct work_struct work;
};

struct scrub_ctx {
	struct scrub_stripe	stripes[SCRUB_TOTAL_STRIPES];
	struct scrub_stripe	*raid56_data_stripes;
	struct btrfs_fs_info	*fs_info;
	struct btrfs_path	extent_path;
	struct btrfs_path	csum_path;
	int			first_free;
	int			cur_stripe;
	atomic_t		cancel_req;
	int			readonly;

	/* State of IO submission throttling affecting the associated device */
	ktime_t			throttle_deadline;
	u64			throttle_sent;

	int			is_dev_replace;
	u64			write_pointer;

	struct mutex            wr_lock;
	struct btrfs_device     *wr_tgtdev;

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	/*
	 * statistics
	 */
	struct btrfs_scrub_progress stat;
	spinlock_t		stat_lock;
<<<<<<< HEAD
};

struct scrub_fixup_nodatasum {
	struct scrub_dev	*sdev;
	u64			logical;
	struct btrfs_root	*root;
	struct btrfs_work	work;
	int			mirror_num;
=======

	/*
	 * Use a ref counter to avoid use-after-free issues. Scrub workers
	 * decrement bios_in_flight and workers_pending and then do a wakeup
	 * on the list_wait wait queue. We must ensure the main scrub task
	 * doesn't free the scrub context before or while the workers are
	 * doing the wakeup() call.
	 */
	refcount_t              refs;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
};

struct scrub_warning {
	struct btrfs_path	*path;
	u64			extent_item_size;
<<<<<<< HEAD
	char			*scratch_buf;
	char			*msg_buf;
	const char		*errstr;
	sector_t		sector;
	u64			logical;
	struct btrfs_device	*dev;
	int			msg_bufsize;
	int			scratch_bufsize;
};


static int scrub_handle_errored_block(struct scrub_block *sblock_to_check);
static int scrub_setup_recheck_block(struct scrub_dev *sdev,
				     struct btrfs_mapping_tree *map_tree,
				     u64 length, u64 logical,
				     struct scrub_block *sblock);
static int scrub_recheck_block(struct btrfs_fs_info *fs_info,
			       struct scrub_block *sblock, int is_metadata,
			       int have_csum, u8 *csum, u64 generation,
			       u16 csum_size);
static void scrub_recheck_block_checksum(struct btrfs_fs_info *fs_info,
					 struct scrub_block *sblock,
					 int is_metadata, int have_csum,
					 const u8 *csum, u64 generation,
					 u16 csum_size);
static void scrub_complete_bio_end_io(struct bio *bio, int err);
static int scrub_repair_block_from_good_copy(struct scrub_block *sblock_bad,
					     struct scrub_block *sblock_good,
					     int force_write);
static int scrub_repair_page_from_good_copy(struct scrub_block *sblock_bad,
					    struct scrub_block *sblock_good,
					    int page_num, int force_write);
static int scrub_checksum_data(struct scrub_block *sblock);
static int scrub_checksum_tree_block(struct scrub_block *sblock);
static int scrub_checksum_super(struct scrub_block *sblock);
static void scrub_block_get(struct scrub_block *sblock);
static void scrub_block_put(struct scrub_block *sblock);
static int scrub_add_page_to_bio(struct scrub_dev *sdev,
				 struct scrub_page *spage);
static int scrub_pages(struct scrub_dev *sdev, u64 logical, u64 len,
		       u64 physical, u64 flags, u64 gen, int mirror_num,
		       u8 *csum, int force);
static void scrub_bio_end_io(struct bio *bio, int err);
static void scrub_bio_end_io_worker(struct btrfs_work *work);
static void scrub_block_complete(struct scrub_block *sblock);


static void scrub_free_csums(struct scrub_dev *sdev)
{
	while (!list_empty(&sdev->csum_list)) {
		struct btrfs_ordered_sum *sum;
		sum = list_first_entry(&sdev->csum_list,
				       struct btrfs_ordered_sum, list);
		list_del(&sum->list);
		kfree(sum);
	}
}

static noinline_for_stack void scrub_free_dev(struct scrub_dev *sdev)
{
	int i;

	if (!sdev)
		return;

	/* this can happen when scrub is cancelled */
	if (sdev->curr != -1) {
		struct scrub_bio *sbio = sdev->bios[sdev->curr];

		for (i = 0; i < sbio->page_count; i++) {
			BUG_ON(!sbio->pagev[i]);
			BUG_ON(!sbio->pagev[i]->page);
			scrub_block_put(sbio->pagev[i]->sblock);
		}
		bio_put(sbio->bio);
	}

	for (i = 0; i < SCRUB_BIOS_PER_DEV; ++i) {
		struct scrub_bio *sbio = sdev->bios[i];

		if (!sbio)
			break;
		kfree(sbio);
	}

	scrub_free_csums(sdev);
	kfree(sdev);
}

static noinline_for_stack
struct scrub_dev *scrub_setup_dev(struct btrfs_device *dev)
{
	struct scrub_dev *sdev;
	int		i;
	struct btrfs_fs_info *fs_info = dev->dev_root->fs_info;
	int pages_per_bio;

	pages_per_bio = min_t(int, SCRUB_PAGES_PER_BIO,
			      bio_get_nr_vecs(dev->bdev));
	sdev = kzalloc(sizeof(*sdev), GFP_NOFS);
	if (!sdev)
		goto nomem;
	sdev->dev = dev;
	sdev->pages_per_bio = pages_per_bio;
	sdev->curr = -1;
	for (i = 0; i < SCRUB_BIOS_PER_DEV; ++i) {
		struct scrub_bio *sbio;

		sbio = kzalloc(sizeof(*sbio), GFP_NOFS);
		if (!sbio)
			goto nomem;
		sdev->bios[i] = sbio;

		sbio->index = i;
		sbio->sdev = sdev;
		sbio->page_count = 0;
		sbio->work.func = scrub_bio_end_io_worker;

		if (i != SCRUB_BIOS_PER_DEV-1)
			sdev->bios[i]->next_free = i + 1;
		else
			sdev->bios[i]->next_free = -1;
	}
	sdev->first_free = 0;
	sdev->nodesize = dev->dev_root->nodesize;
	sdev->leafsize = dev->dev_root->leafsize;
	sdev->sectorsize = dev->dev_root->sectorsize;
	atomic_set(&sdev->in_flight, 0);
	atomic_set(&sdev->fixup_cnt, 0);
	atomic_set(&sdev->cancel_req, 0);
	sdev->csum_size = btrfs_super_csum_size(fs_info->super_copy);
	INIT_LIST_HEAD(&sdev->csum_list);

	spin_lock_init(&sdev->list_lock);
	spin_lock_init(&sdev->stat_lock);
	init_waitqueue_head(&sdev->list_wait);
	return sdev;

nomem:
	scrub_free_dev(sdev);
	return ERR_PTR(-ENOMEM);
}

static int scrub_print_warning_inode(u64 inum, u64 offset, u64 root, void *ctx)
{
	u64 isize;
	u32 nlink;
	int ret;
	int i;
	struct extent_buffer *eb;
	struct btrfs_inode_item *inode_item;
	struct scrub_warning *swarn = ctx;
	struct btrfs_fs_info *fs_info = swarn->dev->dev_root->fs_info;
	struct inode_fs_paths *ipath = NULL;
	struct btrfs_root *local_root;
	struct btrfs_key root_key;

	root_key.objectid = root;
	root_key.type = BTRFS_ROOT_ITEM_KEY;
	root_key.offset = (u64)-1;
	local_root = btrfs_read_fs_root_no_name(fs_info, &root_key);
=======
	const char		*errstr;
	u64			physical;
	u64			logical;
	struct btrfs_device	*dev;
};

static void release_scrub_stripe(struct scrub_stripe *stripe)
{
	if (!stripe)
		return;

	for (int i = 0; i < SCRUB_STRIPE_PAGES; i++) {
		if (stripe->pages[i])
			__free_page(stripe->pages[i]);
		stripe->pages[i] = NULL;
	}
	kfree(stripe->sectors);
	kfree(stripe->csums);
	stripe->sectors = NULL;
	stripe->csums = NULL;
	stripe->sctx = NULL;
	stripe->state = 0;
}

static int init_scrub_stripe(struct btrfs_fs_info *fs_info,
			     struct scrub_stripe *stripe)
{
	int ret;

	memset(stripe, 0, sizeof(*stripe));

	stripe->nr_sectors = BTRFS_STRIPE_LEN >> fs_info->sectorsize_bits;
	stripe->state = 0;

	init_waitqueue_head(&stripe->io_wait);
	init_waitqueue_head(&stripe->repair_wait);
	atomic_set(&stripe->pending_io, 0);
	spin_lock_init(&stripe->write_error_lock);

	ret = btrfs_alloc_page_array(SCRUB_STRIPE_PAGES, stripe->pages, 0);
	if (ret < 0)
		goto error;

	stripe->sectors = kcalloc(stripe->nr_sectors,
				  sizeof(struct scrub_sector_verification),
				  GFP_KERNEL);
	if (!stripe->sectors)
		goto error;

	stripe->csums = kcalloc(BTRFS_STRIPE_LEN >> fs_info->sectorsize_bits,
				fs_info->csum_size, GFP_KERNEL);
	if (!stripe->csums)
		goto error;
	return 0;
error:
	release_scrub_stripe(stripe);
	return -ENOMEM;
}

static void wait_scrub_stripe_io(struct scrub_stripe *stripe)
{
	wait_event(stripe->io_wait, atomic_read(&stripe->pending_io) == 0);
}

static void scrub_put_ctx(struct scrub_ctx *sctx);

static void __scrub_blocked_if_needed(struct btrfs_fs_info *fs_info)
{
	while (atomic_read(&fs_info->scrub_pause_req)) {
		mutex_unlock(&fs_info->scrub_lock);
		wait_event(fs_info->scrub_pause_wait,
		   atomic_read(&fs_info->scrub_pause_req) == 0);
		mutex_lock(&fs_info->scrub_lock);
	}
}

static void scrub_pause_on(struct btrfs_fs_info *fs_info)
{
	atomic_inc(&fs_info->scrubs_paused);
	wake_up(&fs_info->scrub_pause_wait);
}

static void scrub_pause_off(struct btrfs_fs_info *fs_info)
{
	mutex_lock(&fs_info->scrub_lock);
	__scrub_blocked_if_needed(fs_info);
	atomic_dec(&fs_info->scrubs_paused);
	mutex_unlock(&fs_info->scrub_lock);

	wake_up(&fs_info->scrub_pause_wait);
}

static void scrub_blocked_if_needed(struct btrfs_fs_info *fs_info)
{
	scrub_pause_on(fs_info);
	scrub_pause_off(fs_info);
}

static noinline_for_stack void scrub_free_ctx(struct scrub_ctx *sctx)
{
	int i;

	if (!sctx)
		return;

	for (i = 0; i < SCRUB_TOTAL_STRIPES; i++)
		release_scrub_stripe(&sctx->stripes[i]);

	kvfree(sctx);
}

static void scrub_put_ctx(struct scrub_ctx *sctx)
{
	if (refcount_dec_and_test(&sctx->refs))
		scrub_free_ctx(sctx);
}

static noinline_for_stack struct scrub_ctx *scrub_setup_ctx(
		struct btrfs_fs_info *fs_info, int is_dev_replace)
{
	struct scrub_ctx *sctx;
	int		i;

	/* Since sctx has inline 128 stripes, it can go beyond 64K easily.  Use
	 * kvzalloc().
	 */
	sctx = kvzalloc(sizeof(*sctx), GFP_KERNEL);
	if (!sctx)
		goto nomem;
	refcount_set(&sctx->refs, 1);
	sctx->is_dev_replace = is_dev_replace;
	sctx->fs_info = fs_info;
	sctx->extent_path.search_commit_root = 1;
	sctx->extent_path.skip_locking = 1;
	sctx->csum_path.search_commit_root = 1;
	sctx->csum_path.skip_locking = 1;
	for (i = 0; i < SCRUB_TOTAL_STRIPES; i++) {
		int ret;

		ret = init_scrub_stripe(fs_info, &sctx->stripes[i]);
		if (ret < 0)
			goto nomem;
		sctx->stripes[i].sctx = sctx;
	}
	sctx->first_free = 0;
	atomic_set(&sctx->cancel_req, 0);

	spin_lock_init(&sctx->stat_lock);
	sctx->throttle_deadline = 0;

	mutex_init(&sctx->wr_lock);
	if (is_dev_replace) {
		WARN_ON(!fs_info->dev_replace.tgtdev);
		sctx->wr_tgtdev = fs_info->dev_replace.tgtdev;
	}

	return sctx;

nomem:
	scrub_free_ctx(sctx);
	return ERR_PTR(-ENOMEM);
}

static int scrub_print_warning_inode(u64 inum, u64 offset, u64 num_bytes,
				     u64 root, void *warn_ctx)
{
	u32 nlink;
	int ret;
	int i;
	unsigned nofs_flag;
	struct extent_buffer *eb;
	struct btrfs_inode_item *inode_item;
	struct scrub_warning *swarn = warn_ctx;
	struct btrfs_fs_info *fs_info = swarn->dev->fs_info;
	struct inode_fs_paths *ipath = NULL;
	struct btrfs_root *local_root;
	struct btrfs_key key;

	local_root = btrfs_get_fs_root(fs_info, root, true);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	if (IS_ERR(local_root)) {
		ret = PTR_ERR(local_root);
		goto err;
	}

<<<<<<< HEAD
	ret = inode_item_info(inum, 0, local_root, swarn->path);
	if (ret) {
=======
	/*
	 * this makes the path point to (inum INODE_ITEM ioff)
	 */
	key.objectid = inum;
	key.type = BTRFS_INODE_ITEM_KEY;
	key.offset = 0;

	ret = btrfs_search_slot(NULL, local_root, &key, swarn->path, 0, 0);
	if (ret) {
		btrfs_put_root(local_root);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		btrfs_release_path(swarn->path);
		goto err;
	}

	eb = swarn->path->nodes[0];
	inode_item = btrfs_item_ptr(eb, swarn->path->slots[0],
					struct btrfs_inode_item);
<<<<<<< HEAD
	isize = btrfs_inode_size(eb, inode_item);
	nlink = btrfs_inode_nlink(eb, inode_item);
	btrfs_release_path(swarn->path);

	ipath = init_ipath(4096, local_root, swarn->path);
	if (IS_ERR(ipath)) {
=======
	nlink = btrfs_inode_nlink(eb, inode_item);
	btrfs_release_path(swarn->path);

	/*
	 * init_path might indirectly call vmalloc, or use GFP_KERNEL. Scrub
	 * uses GFP_NOFS in this context, so we keep it consistent but it does
	 * not seem to be strictly necessary.
	 */
	nofs_flag = memalloc_nofs_save();
	ipath = init_ipath(4096, local_root, swarn->path);
	memalloc_nofs_restore(nofs_flag);
	if (IS_ERR(ipath)) {
		btrfs_put_root(local_root);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		ret = PTR_ERR(ipath);
		ipath = NULL;
		goto err;
	}
	ret = paths_from_inode(inum, ipath);

	if (ret < 0)
		goto err;

	/*
	 * we deliberately ignore the bit ipath might have been too small to
	 * hold all of the paths here
	 */
	for (i = 0; i < ipath->fspath->elem_cnt; ++i)
<<<<<<< HEAD
		printk(KERN_WARNING "btrfs: %s at logical %llu on dev "
			"%s, sector %llu, root %llu, inode %llu, offset %llu, "
			"length %llu, links %u (path: %s)\n", swarn->errstr,
			swarn->logical, swarn->dev->name,
			(unsigned long long)swarn->sector, root, inum, offset,
			min(isize - offset, (u64)PAGE_SIZE), nlink,
			(char *)(unsigned long)ipath->fspath->val[i]);

=======
		btrfs_warn_in_rcu(fs_info,
"%s at logical %llu on dev %s, physical %llu, root %llu, inode %llu, offset %llu, length %u, links %u (path: %s)",
				  swarn->errstr, swarn->logical,
				  btrfs_dev_name(swarn->dev),
				  swarn->physical,
				  root, inum, offset,
				  fs_info->sectorsize, nlink,
				  (char *)(unsigned long)ipath->fspath->val[i]);

	btrfs_put_root(local_root);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	free_ipath(ipath);
	return 0;

err:
<<<<<<< HEAD
	printk(KERN_WARNING "btrfs: %s at logical %llu on dev "
		"%s, sector %llu, root %llu, inode %llu, offset %llu: path "
		"resolving failed with ret=%d\n", swarn->errstr,
		swarn->logical, swarn->dev->name,
		(unsigned long long)swarn->sector, root, inum, offset, ret);
=======
	btrfs_warn_in_rcu(fs_info,
			  "%s at logical %llu on dev %s, physical %llu, root %llu, inode %llu, offset %llu: path resolving failed with ret=%d",
			  swarn->errstr, swarn->logical,
			  btrfs_dev_name(swarn->dev),
			  swarn->physical,
			  root, inum, offset, ret);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	free_ipath(ipath);
	return 0;
}

<<<<<<< HEAD
static void scrub_print_warning(const char *errstr, struct scrub_block *sblock)
{
	struct btrfs_device *dev = sblock->sdev->dev;
	struct btrfs_fs_info *fs_info = dev->dev_root->fs_info;
=======
static void scrub_print_common_warning(const char *errstr, struct btrfs_device *dev,
				       bool is_super, u64 logical, u64 physical)
{
	struct btrfs_fs_info *fs_info = dev->fs_info;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	struct btrfs_path *path;
	struct btrfs_key found_key;
	struct extent_buffer *eb;
	struct btrfs_extent_item *ei;
	struct scrub_warning swarn;
<<<<<<< HEAD
	u32 item_size;
	int ret;
	u64 ref_root;
	u8 ref_level;
	unsigned long ptr = 0;
	const int bufsize = 4096;
	u64 extent_item_pos;

	path = btrfs_alloc_path();

	swarn.scratch_buf = kmalloc(bufsize, GFP_NOFS);
	swarn.msg_buf = kmalloc(bufsize, GFP_NOFS);
	BUG_ON(sblock->page_count < 1);
	swarn.sector = (sblock->pagev[0].physical) >> 9;
	swarn.logical = sblock->pagev[0].logical;
	swarn.errstr = errstr;
	swarn.dev = dev;
	swarn.msg_bufsize = bufsize;
	swarn.scratch_bufsize = bufsize;

	if (!path || !swarn.scratch_buf || !swarn.msg_buf)
		goto out;

	ret = extent_from_logical(fs_info, swarn.logical, path, &found_key);
	if (ret < 0)
		goto out;

	extent_item_pos = swarn.logical - found_key.objectid;
=======
	u64 flags = 0;
	u32 item_size;
	int ret;

	/* Super block error, no need to search extent tree. */
	if (is_super) {
		btrfs_warn_in_rcu(fs_info, "%s on device %s, physical %llu",
				  errstr, btrfs_dev_name(dev), physical);
		return;
	}
	path = btrfs_alloc_path();
	if (!path)
		return;

	swarn.physical = physical;
	swarn.logical = logical;
	swarn.errstr = errstr;
	swarn.dev = NULL;

	ret = extent_from_logical(fs_info, swarn.logical, path, &found_key,
				  &flags);
	if (ret < 0)
		goto out;

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	swarn.extent_item_size = found_key.offset;

	eb = path->nodes[0];
	ei = btrfs_item_ptr(eb, path->slots[0], struct btrfs_extent_item);
<<<<<<< HEAD
	item_size = btrfs_item_size_nr(eb, path->slots[0]);

	if (ret & BTRFS_EXTENT_FLAG_TREE_BLOCK) {
		do {
			ret = tree_backref_for_extent(&ptr, eb, ei, item_size,
							&ref_root, &ref_level);
			printk(KERN_WARNING
				"btrfs: %s at logical %llu on dev %s, "
				"sector %llu: metadata %s (level %d) in tree "
				"%llu\n", errstr, swarn.logical, dev->name,
				(unsigned long long)swarn.sector,
				ref_level ? "node" : "leaf",
				ret < 0 ? -1 : ref_level,
				ret < 0 ? -1 : ref_root);
		} while (ret != 1);
		btrfs_release_path(path);
	} else {
		btrfs_release_path(path);
		swarn.path = path;
		iterate_extent_inodes(fs_info, found_key.objectid,
					extent_item_pos, 1,
					scrub_print_warning_inode, &swarn);
	}

out:
	btrfs_free_path(path);
	kfree(swarn.scratch_buf);
	kfree(swarn.msg_buf);
}

static int scrub_fixup_readpage(u64 inum, u64 offset, u64 root, void *ctx)
{
	struct page *page = NULL;
	unsigned long index;
	struct scrub_fixup_nodatasum *fixup = ctx;
	int ret;
	int corrected = 0;
	struct btrfs_key key;
	struct inode *inode = NULL;
	u64 end = offset + PAGE_SIZE - 1;
	struct btrfs_root *local_root;

	key.objectid = root;
	key.type = BTRFS_ROOT_ITEM_KEY;
	key.offset = (u64)-1;
	local_root = btrfs_read_fs_root_no_name(fixup->root->fs_info, &key);
	if (IS_ERR(local_root))
		return PTR_ERR(local_root);

	key.type = BTRFS_INODE_ITEM_KEY;
	key.objectid = inum;
	key.offset = 0;
	inode = btrfs_iget(fixup->root->fs_info->sb, &key, local_root, NULL);
	if (IS_ERR(inode))
		return PTR_ERR(inode);

	index = offset >> PAGE_CACHE_SHIFT;

	page = find_or_create_page(inode->i_mapping, index, GFP_NOFS);
	if (!page) {
		ret = -ENOMEM;
		goto out;
	}

	if (PageUptodate(page)) {
		struct btrfs_mapping_tree *map_tree;
		if (PageDirty(page)) {
			/*
			 * we need to write the data to the defect sector. the
			 * data that was in that sector is not in memory,
			 * because the page was modified. we must not write the
			 * modified page to that sector.
			 *
			 * TODO: what could be done here: wait for the delalloc
			 *       runner to write out that page (might involve
			 *       COW) and see whether the sector is still
			 *       referenced afterwards.
			 *
			 * For the meantime, we'll treat this error
			 * incorrectable, although there is a chance that a
			 * later scrub will find the bad sector again and that
			 * there's no dirty page in memory, then.
			 */
			ret = -EIO;
			goto out;
		}
		map_tree = &BTRFS_I(inode)->root->fs_info->mapping_tree;
		ret = repair_io_failure(map_tree, offset, PAGE_SIZE,
					fixup->logical, page,
					fixup->mirror_num);
		unlock_page(page);
		corrected = !ret;
	} else {
		/*
		 * we need to get good data first. the general readpage path
		 * will call repair_io_failure for us, we just have to make
		 * sure we read the bad mirror.
		 */
		ret = set_extent_bits(&BTRFS_I(inode)->io_tree, offset, end,
					EXTENT_DAMAGED, GFP_NOFS);
		if (ret) {
			/* set_extent_bits should give proper error */
			WARN_ON(ret > 0);
			if (ret > 0)
				ret = -EFAULT;
			goto out;
		}

		ret = extent_read_full_page(&BTRFS_I(inode)->io_tree, page,
						btrfs_get_extent,
						fixup->mirror_num);
		wait_on_page_locked(page);

		corrected = !test_range_bit(&BTRFS_I(inode)->io_tree, offset,
						end, EXTENT_DAMAGED, 0, NULL);
		if (!corrected)
			clear_extent_bits(&BTRFS_I(inode)->io_tree, offset, end,
						EXTENT_DAMAGED, GFP_NOFS);
	}

out:
	if (page)
		put_page(page);
	if (inode)
		iput(inode);

	if (ret < 0)
		return ret;

	if (ret == 0 && corrected) {
		/*
		 * we only need to call readpage for one of the inodes belonging
		 * to this extent. so make iterate_extent_inodes stop
		 */
		return 1;
	}

	return -EIO;
}

static void scrub_fixup_nodatasum(struct btrfs_work *work)
{
	int ret;
	struct scrub_fixup_nodatasum *fixup;
	struct scrub_dev *sdev;
	struct btrfs_trans_handle *trans = NULL;
	struct btrfs_fs_info *fs_info;
	struct btrfs_path *path;
	int uncorrectable = 0;

	fixup = container_of(work, struct scrub_fixup_nodatasum, work);
	sdev = fixup->sdev;
	fs_info = fixup->root->fs_info;

	path = btrfs_alloc_path();
	if (!path) {
		spin_lock(&sdev->stat_lock);
		++sdev->stat.malloc_errors;
		spin_unlock(&sdev->stat_lock);
		uncorrectable = 1;
		goto out;
	}

	trans = btrfs_join_transaction(fixup->root);
	if (IS_ERR(trans)) {
		uncorrectable = 1;
		goto out;
	}

	/*
	 * the idea is to trigger a regular read through the standard path. we
	 * read a page from the (failed) logical address by specifying the
	 * corresponding copynum of the failed sector. thus, that readpage is
	 * expected to fail.
	 * that is the point where on-the-fly error correction will kick in
	 * (once it's finished) and rewrite the failed sector if a good copy
	 * can be found.
	 */
	ret = iterate_inodes_from_logical(fixup->logical, fixup->root->fs_info,
						path, scrub_fixup_readpage,
						fixup);
	if (ret < 0) {
		uncorrectable = 1;
		goto out;
	}
	WARN_ON(ret != 1);

	spin_lock(&sdev->stat_lock);
	++sdev->stat.corrected_errors;
	spin_unlock(&sdev->stat_lock);

out:
	if (trans && !IS_ERR(trans))
		btrfs_end_transaction(trans, fixup->root);
	if (uncorrectable) {
		spin_lock(&sdev->stat_lock);
		++sdev->stat.uncorrectable_errors;
		spin_unlock(&sdev->stat_lock);
		printk_ratelimited(KERN_ERR
			"btrfs: unable to fixup (nodatasum) error at logical %llu on dev %s\n",
			(unsigned long long)fixup->logical, sdev->dev->name);
	}

	btrfs_free_path(path);
	kfree(fixup);

	/* see caller why we're pretending to be paused in the scrub counters */
	mutex_lock(&fs_info->scrub_lock);
	atomic_dec(&fs_info->scrubs_running);
	atomic_dec(&fs_info->scrubs_paused);
	mutex_unlock(&fs_info->scrub_lock);
	atomic_dec(&sdev->fixup_cnt);
	wake_up(&fs_info->scrub_pause_wait);
	wake_up(&sdev->list_wait);
}

/*
 * scrub_handle_errored_block gets called when either verification of the
 * pages failed or the bio failed to read, e.g. with EIO. In the latter
 * case, this function handles all pages in the bio, even though only one
 * may be bad.
 * The goal of this function is to repair the errored block by using the
 * contents of one of the mirrors.
 */
static int scrub_handle_errored_block(struct scrub_block *sblock_to_check)
{
	struct scrub_dev *sdev = sblock_to_check->sdev;
	struct btrfs_fs_info *fs_info;
	u64 length;
	u64 logical;
	u64 generation;
	unsigned int failed_mirror_index;
	unsigned int is_metadata;
	unsigned int have_csum;
	u8 *csum;
	struct scrub_block *sblocks_for_recheck; /* holds one for each mirror */
	struct scrub_block *sblock_bad;
	int ret;
	int mirror_index;
	int page_num;
	int success;
	static DEFINE_RATELIMIT_STATE(_rs, DEFAULT_RATELIMIT_INTERVAL,
				      DEFAULT_RATELIMIT_BURST);

	BUG_ON(sblock_to_check->page_count < 1);
	fs_info = sdev->dev->dev_root->fs_info;
	length = sblock_to_check->page_count * PAGE_SIZE;
	logical = sblock_to_check->pagev[0].logical;
	generation = sblock_to_check->pagev[0].generation;
	BUG_ON(sblock_to_check->pagev[0].mirror_num < 1);
	failed_mirror_index = sblock_to_check->pagev[0].mirror_num - 1;
	is_metadata = !(sblock_to_check->pagev[0].flags &
			BTRFS_EXTENT_FLAG_DATA);
	have_csum = sblock_to_check->pagev[0].have_csum;
	csum = sblock_to_check->pagev[0].csum;

	/*
	 * read all mirrors one after the other. This includes to
	 * re-read the extent or metadata block that failed (that was
	 * the cause that this fixup code is called) another time,
	 * page by page this time in order to know which pages
	 * caused I/O errors and which ones are good (for all mirrors).
	 * It is the goal to handle the situation when more than one
	 * mirror contains I/O errors, but the errors do not
	 * overlap, i.e. the data can be repaired by selecting the
	 * pages from those mirrors without I/O error on the
	 * particular pages. One example (with blocks >= 2 * PAGE_SIZE)
	 * would be that mirror #1 has an I/O error on the first page,
	 * the second page is good, and mirror #2 has an I/O error on
	 * the second page, but the first page is good.
	 * Then the first page of the first mirror can be repaired by
	 * taking the first page of the second mirror, and the
	 * second page of the second mirror can be repaired by
	 * copying the contents of the 2nd page of the 1st mirror.
	 * One more note: if the pages of one mirror contain I/O
	 * errors, the checksum cannot be verified. In order to get
	 * the best data for repairing, the first attempt is to find
	 * a mirror without I/O errors and with a validated checksum.
	 * Only if this is not possible, the pages are picked from
	 * mirrors with I/O errors without considering the checksum.
	 * If the latter is the case, at the end, the checksum of the
	 * repaired area is verified in order to correctly maintain
	 * the statistics.
	 */

	sblocks_for_recheck = kzalloc(BTRFS_MAX_MIRRORS *
				     sizeof(*sblocks_for_recheck),
				     GFP_NOFS);
	if (!sblocks_for_recheck) {
		spin_lock(&sdev->stat_lock);
		sdev->stat.malloc_errors++;
		sdev->stat.read_errors++;
		sdev->stat.uncorrectable_errors++;
		spin_unlock(&sdev->stat_lock);
		goto out;
	}

	/* setup the context, map the logical blocks and alloc the pages */
	ret = scrub_setup_recheck_block(sdev, &fs_info->mapping_tree, length,
					logical, sblocks_for_recheck);
	if (ret) {
		spin_lock(&sdev->stat_lock);
		sdev->stat.read_errors++;
		sdev->stat.uncorrectable_errors++;
		spin_unlock(&sdev->stat_lock);
		goto out;
	}
	BUG_ON(failed_mirror_index >= BTRFS_MAX_MIRRORS);
	sblock_bad = sblocks_for_recheck + failed_mirror_index;

	/* build and submit the bios for the failed mirror, check checksums */
	ret = scrub_recheck_block(fs_info, sblock_bad, is_metadata, have_csum,
				  csum, generation, sdev->csum_size);
	if (ret) {
		spin_lock(&sdev->stat_lock);
		sdev->stat.read_errors++;
		sdev->stat.uncorrectable_errors++;
		spin_unlock(&sdev->stat_lock);
		goto out;
	}

	if (!sblock_bad->header_error && !sblock_bad->checksum_error &&
	    sblock_bad->no_io_error_seen) {
		/*
		 * the error disappeared after reading page by page, or
		 * the area was part of a huge bio and other parts of the
		 * bio caused I/O errors, or the block layer merged several
		 * read requests into one and the error is caused by a
		 * different bio (usually one of the two latter cases is
		 * the cause)
		 */
		spin_lock(&sdev->stat_lock);
		sdev->stat.unverified_errors++;
		spin_unlock(&sdev->stat_lock);

		goto out;
	}

	if (!sblock_bad->no_io_error_seen) {
		spin_lock(&sdev->stat_lock);
		sdev->stat.read_errors++;
		spin_unlock(&sdev->stat_lock);
		if (__ratelimit(&_rs))
			scrub_print_warning("i/o error", sblock_to_check);
	} else if (sblock_bad->checksum_error) {
		spin_lock(&sdev->stat_lock);
		sdev->stat.csum_errors++;
		spin_unlock(&sdev->stat_lock);
		if (__ratelimit(&_rs))
			scrub_print_warning("checksum error", sblock_to_check);
	} else if (sblock_bad->header_error) {
		spin_lock(&sdev->stat_lock);
		sdev->stat.verify_errors++;
		spin_unlock(&sdev->stat_lock);
		if (__ratelimit(&_rs))
			scrub_print_warning("checksum/header error",
					    sblock_to_check);
	}

	if (sdev->readonly)
		goto did_not_correct_error;

	if (!is_metadata && !have_csum) {
		struct scrub_fixup_nodatasum *fixup_nodatasum;

		/*
		 * !is_metadata and !have_csum, this means that the data
		 * might not be COW'ed, that it might be modified
		 * concurrently. The general strategy to work on the
		 * commit root does not help in the case when COW is not
		 * used.
		 */
		fixup_nodatasum = kzalloc(sizeof(*fixup_nodatasum), GFP_NOFS);
		if (!fixup_nodatasum)
			goto did_not_correct_error;
		fixup_nodatasum->sdev = sdev;
		fixup_nodatasum->logical = logical;
		fixup_nodatasum->root = fs_info->extent_root;
		fixup_nodatasum->mirror_num = failed_mirror_index + 1;
		/*
		 * increment scrubs_running to prevent cancel requests from
		 * completing as long as a fixup worker is running. we must also
		 * increment scrubs_paused to prevent deadlocking on pause
		 * requests used for transactions commits (as the worker uses a
		 * transaction context). it is safe to regard the fixup worker
		 * as paused for all matters practical. effectively, we only
		 * avoid cancellation requests from completing.
		 */
		mutex_lock(&fs_info->scrub_lock);
		atomic_inc(&fs_info->scrubs_running);
		atomic_inc(&fs_info->scrubs_paused);
		mutex_unlock(&fs_info->scrub_lock);
		atomic_inc(&sdev->fixup_cnt);
		fixup_nodatasum->work.func = scrub_fixup_nodatasum;
		btrfs_queue_worker(&fs_info->scrub_workers,
				   &fixup_nodatasum->work);
		goto out;
	}

	/*
	 * now build and submit the bios for the other mirrors, check
	 * checksums
	 */
	for (mirror_index = 0;
	     mirror_index < BTRFS_MAX_MIRRORS &&
	     sblocks_for_recheck[mirror_index].page_count > 0;
	     mirror_index++) {
		if (mirror_index == failed_mirror_index)
			continue;

		/* build and submit the bios, check checksums */
		ret = scrub_recheck_block(fs_info,
					  sblocks_for_recheck + mirror_index,
					  is_metadata, have_csum, csum,
					  generation, sdev->csum_size);
		if (ret)
			goto did_not_correct_error;
	}

	/*
	 * first try to pick the mirror which is completely without I/O
	 * errors and also does not have a checksum error.
	 * If one is found, and if a checksum is present, the full block
	 * that is known to contain an error is rewritten. Afterwards
	 * the block is known to be corrected.
	 * If a mirror is found which is completely correct, and no
	 * checksum is present, only those pages are rewritten that had
	 * an I/O error in the block to be repaired, since it cannot be
	 * determined, which copy of the other pages is better (and it
	 * could happen otherwise that a correct page would be
	 * overwritten by a bad one).
	 */
	for (mirror_index = 0;
	     mirror_index < BTRFS_MAX_MIRRORS &&
	     sblocks_for_recheck[mirror_index].page_count > 0;
	     mirror_index++) {
		struct scrub_block *sblock_other = sblocks_for_recheck +
						   mirror_index;

		if (!sblock_other->header_error &&
		    !sblock_other->checksum_error &&
		    sblock_other->no_io_error_seen) {
			int force_write = is_metadata || have_csum;

			ret = scrub_repair_block_from_good_copy(sblock_bad,
								sblock_other,
								force_write);
			if (0 == ret)
				goto corrected_error;
		}
	}

	/*
	 * in case of I/O errors in the area that is supposed to be
	 * repaired, continue by picking good copies of those pages.
	 * Select the good pages from mirrors to rewrite bad pages from
	 * the area to fix. Afterwards verify the checksum of the block
	 * that is supposed to be repaired. This verification step is
	 * only done for the purpose of statistic counting and for the
	 * final scrub report, whether errors remain.
	 * A perfect algorithm could make use of the checksum and try
	 * all possible combinations of pages from the different mirrors
	 * until the checksum verification succeeds. For example, when
	 * the 2nd page of mirror #1 faces I/O errors, and the 2nd page
	 * of mirror #2 is readable but the final checksum test fails,
	 * then the 2nd page of mirror #3 could be tried, whether now
	 * the final checksum succeedes. But this would be a rare
	 * exception and is therefore not implemented. At least it is
	 * avoided that the good copy is overwritten.
	 * A more useful improvement would be to pick the sectors
	 * without I/O error based on sector sizes (512 bytes on legacy
	 * disks) instead of on PAGE_SIZE. Then maybe 512 byte of one
	 * mirror could be repaired by taking 512 byte of a different
	 * mirror, even if other 512 byte sectors in the same PAGE_SIZE
	 * area are unreadable.
	 */

	/* can only fix I/O errors from here on */
	if (sblock_bad->no_io_error_seen)
		goto did_not_correct_error;

	success = 1;
	for (page_num = 0; page_num < sblock_bad->page_count; page_num++) {
		struct scrub_page *page_bad = sblock_bad->pagev + page_num;

		if (!page_bad->io_error)
			continue;

		for (mirror_index = 0;
		     mirror_index < BTRFS_MAX_MIRRORS &&
		     sblocks_for_recheck[mirror_index].page_count > 0;
		     mirror_index++) {
			struct scrub_block *sblock_other = sblocks_for_recheck +
							   mirror_index;
			struct scrub_page *page_other = sblock_other->pagev +
							page_num;

			if (!page_other->io_error) {
				ret = scrub_repair_page_from_good_copy(
					sblock_bad, sblock_other, page_num, 0);
				if (0 == ret) {
					page_bad->io_error = 0;
					break; /* succeeded for this page */
				}
			}
		}

		if (page_bad->io_error) {
			/* did not find a mirror to copy the page from */
			success = 0;
		}
	}

	if (success) {
		if (is_metadata || have_csum) {
			/*
			 * need to verify the checksum now that all
			 * sectors on disk are repaired (the write
			 * request for data to be repaired is on its way).
			 * Just be lazy and use scrub_recheck_block()
			 * which re-reads the data before the checksum
			 * is verified, but most likely the data comes out
			 * of the page cache.
			 */
			ret = scrub_recheck_block(fs_info, sblock_bad,
						  is_metadata, have_csum, csum,
						  generation, sdev->csum_size);
			if (!ret && !sblock_bad->header_error &&
			    !sblock_bad->checksum_error &&
			    sblock_bad->no_io_error_seen)
				goto corrected_error;
			else
				goto did_not_correct_error;
		} else {
corrected_error:
			spin_lock(&sdev->stat_lock);
			sdev->stat.corrected_errors++;
			spin_unlock(&sdev->stat_lock);
			printk_ratelimited(KERN_ERR
				"btrfs: fixed up error at logical %llu on dev %s\n",
				(unsigned long long)logical, sdev->dev->name);
		}
	} else {
did_not_correct_error:
		spin_lock(&sdev->stat_lock);
		sdev->stat.uncorrectable_errors++;
		spin_unlock(&sdev->stat_lock);
		printk_ratelimited(KERN_ERR
			"btrfs: unable to fixup (regular) error at logical %llu on dev %s\n",
			(unsigned long long)logical, sdev->dev->name);
	}

out:
	if (sblocks_for_recheck) {
		for (mirror_index = 0; mirror_index < BTRFS_MAX_MIRRORS;
		     mirror_index++) {
			struct scrub_block *sblock = sblocks_for_recheck +
						     mirror_index;
			int page_index;

			for (page_index = 0; page_index < SCRUB_PAGES_PER_BIO;
			     page_index++)
				if (sblock->pagev[page_index].page)
					__free_page(
						sblock->pagev[page_index].page);
		}
		kfree(sblocks_for_recheck);
	}

	return 0;
}

static int scrub_setup_recheck_block(struct scrub_dev *sdev,
				     struct btrfs_mapping_tree *map_tree,
				     u64 length, u64 logical,
				     struct scrub_block *sblocks_for_recheck)
{
	int page_index;
	int mirror_index;
	int ret;

	/*
	 * note: the three members sdev, ref_count and outstanding_pages
	 * are not used (and not set) in the blocks that are used for
	 * the recheck procedure
	 */

	page_index = 0;
	while (length > 0) {
		u64 sublen = min_t(u64, length, PAGE_SIZE);
		u64 mapped_length = sublen;
		struct btrfs_bio *bbio = NULL;

		/*
		 * with a length of PAGE_SIZE, each returned stripe
		 * represents one mirror
		 */
		ret = btrfs_map_block(map_tree, WRITE, logical, &mapped_length,
				      &bbio, 0);
		if (ret || !bbio || mapped_length < sublen) {
			kfree(bbio);
			return -EIO;
		}

		BUG_ON(page_index >= SCRUB_PAGES_PER_BIO);
		for (mirror_index = 0; mirror_index < (int)bbio->num_stripes;
		     mirror_index++) {
			struct scrub_block *sblock;
			struct scrub_page *page;

			if (mirror_index >= BTRFS_MAX_MIRRORS)
				continue;

			sblock = sblocks_for_recheck + mirror_index;
			page = sblock->pagev + page_index;
			page->logical = logical;
			page->physical = bbio->stripes[mirror_index].physical;
			/* for missing devices, bdev is NULL */
			page->bdev = bbio->stripes[mirror_index].dev->bdev;
			page->mirror_num = mirror_index + 1;
			page->page = alloc_page(GFP_NOFS);
			if (!page->page) {
				spin_lock(&sdev->stat_lock);
				sdev->stat.malloc_errors++;
				spin_unlock(&sdev->stat_lock);
				return -ENOMEM;
			}
			sblock->page_count++;
		}
		kfree(bbio);
		length -= sublen;
		logical += sublen;
		page_index++;
	}

	return 0;
}

/*
 * this function will check the on disk data for checksum errors, header
 * errors and read I/O errors. If any I/O errors happen, the exact pages
 * which are errored are marked as being bad. The goal is to enable scrub
 * to take those pages that are not errored from all the mirrors so that
 * the pages that are errored in the just handled mirror can be repaired.
 */
static int scrub_recheck_block(struct btrfs_fs_info *fs_info,
			       struct scrub_block *sblock, int is_metadata,
			       int have_csum, u8 *csum, u64 generation,
			       u16 csum_size)
{
	int page_num;

	sblock->no_io_error_seen = 1;
	sblock->header_error = 0;
	sblock->checksum_error = 0;

	for (page_num = 0; page_num < sblock->page_count; page_num++) {
		struct bio *bio;
		int ret;
		struct scrub_page *page = sblock->pagev + page_num;
		DECLARE_COMPLETION_ONSTACK(complete);

		if (page->bdev == NULL) {
			page->io_error = 1;
			sblock->no_io_error_seen = 0;
			continue;
		}

		BUG_ON(!page->page);
		bio = bio_alloc(GFP_NOFS, 1);
		if (!bio)
			return -EIO;
		bio->bi_bdev = page->bdev;
		bio->bi_sector = page->physical >> 9;
		bio->bi_end_io = scrub_complete_bio_end_io;
		bio->bi_private = &complete;

		ret = bio_add_page(bio, page->page, PAGE_SIZE, 0);
		if (PAGE_SIZE != ret) {
			bio_put(bio);
			return -EIO;
		}
		btrfsic_submit_bio(READ, bio);

		/* this will also unplug the queue */
		wait_for_completion(&complete);

		page->io_error = !test_bit(BIO_UPTODATE, &bio->bi_flags);
		if (!test_bit(BIO_UPTODATE, &bio->bi_flags))
			sblock->no_io_error_seen = 0;
		bio_put(bio);
	}

	if (sblock->no_io_error_seen)
		scrub_recheck_block_checksum(fs_info, sblock, is_metadata,
					     have_csum, csum, generation,
					     csum_size);

	return 0;
}

static void scrub_recheck_block_checksum(struct btrfs_fs_info *fs_info,
					 struct scrub_block *sblock,
					 int is_metadata, int have_csum,
					 const u8 *csum, u64 generation,
					 u16 csum_size)
{
	int page_num;
	u8 calculated_csum[BTRFS_CSUM_SIZE];
	u32 crc = ~(u32)0;
	struct btrfs_root *root = fs_info->extent_root;
	void *mapped_buffer;

	BUG_ON(!sblock->pagev[0].page);
	if (is_metadata) {
		struct btrfs_header *h;

		mapped_buffer = kmap_atomic(sblock->pagev[0].page);
		h = (struct btrfs_header *)mapped_buffer;

		if (sblock->pagev[0].logical != le64_to_cpu(h->bytenr) ||
		    generation != le64_to_cpu(h->generation) ||
		    memcmp(h->fsid, fs_info->fsid, BTRFS_UUID_SIZE) ||
		    memcmp(h->chunk_tree_uuid, fs_info->chunk_tree_uuid,
			   BTRFS_UUID_SIZE))
			sblock->header_error = 1;
		csum = h->csum;
	} else {
		if (!have_csum)
			return;

		mapped_buffer = kmap_atomic(sblock->pagev[0].page);
	}

	for (page_num = 0;;) {
		if (page_num == 0 && is_metadata)
			crc = btrfs_csum_data(root,
				((u8 *)mapped_buffer) + BTRFS_CSUM_SIZE,
				crc, PAGE_SIZE - BTRFS_CSUM_SIZE);
		else
			crc = btrfs_csum_data(root, mapped_buffer, crc,
					      PAGE_SIZE);

		kunmap_atomic(mapped_buffer);
		page_num++;
		if (page_num >= sblock->page_count)
			break;
		BUG_ON(!sblock->pagev[page_num].page);

		mapped_buffer = kmap_atomic(sblock->pagev[page_num].page);
	}

	btrfs_csum_final(crc, calculated_csum);
	if (memcmp(calculated_csum, csum, csum_size))
		sblock->checksum_error = 1;
}

static void scrub_complete_bio_end_io(struct bio *bio, int err)
{
	complete((struct completion *)bio->bi_private);
}

static int scrub_repair_block_from_good_copy(struct scrub_block *sblock_bad,
					     struct scrub_block *sblock_good,
					     int force_write)
{
	int page_num;
	int ret = 0;

	for (page_num = 0; page_num < sblock_bad->page_count; page_num++) {
		int ret_sub;

		ret_sub = scrub_repair_page_from_good_copy(sblock_bad,
							   sblock_good,
							   page_num,
							   force_write);
		if (ret_sub)
			ret = ret_sub;
	}

	return ret;
}

static int scrub_repair_page_from_good_copy(struct scrub_block *sblock_bad,
					    struct scrub_block *sblock_good,
					    int page_num, int force_write)
{
	struct scrub_page *page_bad = sblock_bad->pagev + page_num;
	struct scrub_page *page_good = sblock_good->pagev + page_num;

	BUG_ON(sblock_bad->pagev[page_num].page == NULL);
	BUG_ON(sblock_good->pagev[page_num].page == NULL);
	if (force_write || sblock_bad->header_error ||
	    sblock_bad->checksum_error || page_bad->io_error) {
		struct bio *bio;
		int ret;
		DECLARE_COMPLETION_ONSTACK(complete);

		bio = bio_alloc(GFP_NOFS, 1);
		if (!bio)
			return -EIO;
		bio->bi_bdev = page_bad->bdev;
		bio->bi_sector = page_bad->physical >> 9;
		bio->bi_end_io = scrub_complete_bio_end_io;
		bio->bi_private = &complete;

		ret = bio_add_page(bio, page_good->page, PAGE_SIZE, 0);
		if (PAGE_SIZE != ret) {
			bio_put(bio);
			return -EIO;
		}
		btrfsic_submit_bio(WRITE, bio);

		/* this will also unplug the queue */
		wait_for_completion(&complete);
		bio_put(bio);
	}

	return 0;
}

static void scrub_checksum(struct scrub_block *sblock)
{
	u64 flags;
	int ret;

	BUG_ON(sblock->page_count < 1);
	flags = sblock->pagev[0].flags;
	ret = 0;
	if (flags & BTRFS_EXTENT_FLAG_DATA)
		ret = scrub_checksum_data(sblock);
	else if (flags & BTRFS_EXTENT_FLAG_TREE_BLOCK)
		ret = scrub_checksum_tree_block(sblock);
	else if (flags & BTRFS_EXTENT_FLAG_SUPER)
		(void)scrub_checksum_super(sblock);
	else
		WARN_ON(1);
	if (ret)
		scrub_handle_errored_block(sblock);
}

static int scrub_checksum_data(struct scrub_block *sblock)
{
	struct scrub_dev *sdev = sblock->sdev;
	u8 csum[BTRFS_CSUM_SIZE];
	u8 *on_disk_csum;
	struct page *page;
	void *buffer;
	u32 crc = ~(u32)0;
	int fail = 0;
	struct btrfs_root *root = sdev->dev->dev_root;
	u64 len;
	int index;

	BUG_ON(sblock->page_count < 1);
	if (!sblock->pagev[0].have_csum)
		return 0;

	on_disk_csum = sblock->pagev[0].csum;
	page = sblock->pagev[0].page;
	buffer = kmap_atomic(page);

	len = sdev->sectorsize;
	index = 0;
	for (;;) {
		u64 l = min_t(u64, len, PAGE_SIZE);

		crc = btrfs_csum_data(root, buffer, crc, l);
		kunmap_atomic(buffer);
		len -= l;
		if (len == 0)
			break;
		index++;
		BUG_ON(index >= sblock->page_count);
		BUG_ON(!sblock->pagev[index].page);
		page = sblock->pagev[index].page;
		buffer = kmap_atomic(page);
	}

	btrfs_csum_final(crc, csum);
	if (memcmp(csum, on_disk_csum, sdev->csum_size))
		fail = 1;

	return fail;
}

static int scrub_checksum_tree_block(struct scrub_block *sblock)
{
	struct scrub_dev *sdev = sblock->sdev;
	struct btrfs_header *h;
	struct btrfs_root *root = sdev->dev->dev_root;
	struct btrfs_fs_info *fs_info = root->fs_info;
	u8 calculated_csum[BTRFS_CSUM_SIZE];
	u8 on_disk_csum[BTRFS_CSUM_SIZE];
	struct page *page;
	void *mapped_buffer;
	u64 mapped_size;
	void *p;
	u32 crc = ~(u32)0;
	int fail = 0;
	int crc_fail = 0;
	u64 len;
	int index;

	BUG_ON(sblock->page_count < 1);
	page = sblock->pagev[0].page;
	mapped_buffer = kmap_atomic(page);
	h = (struct btrfs_header *)mapped_buffer;
	memcpy(on_disk_csum, h->csum, sdev->csum_size);

	/*
	 * we don't use the getter functions here, as we
	 * a) don't have an extent buffer and
	 * b) the page is already kmapped
	 */

	if (sblock->pagev[0].logical != le64_to_cpu(h->bytenr))
		++fail;

	if (sblock->pagev[0].generation != le64_to_cpu(h->generation))
		++fail;

	if (memcmp(h->fsid, fs_info->fsid, BTRFS_UUID_SIZE))
		++fail;

	if (memcmp(h->chunk_tree_uuid, fs_info->chunk_tree_uuid,
		   BTRFS_UUID_SIZE))
		++fail;

	BUG_ON(sdev->nodesize != sdev->leafsize);
	len = sdev->nodesize - BTRFS_CSUM_SIZE;
	mapped_size = PAGE_SIZE - BTRFS_CSUM_SIZE;
	p = ((u8 *)mapped_buffer) + BTRFS_CSUM_SIZE;
	index = 0;
	for (;;) {
		u64 l = min_t(u64, len, mapped_size);

		crc = btrfs_csum_data(root, p, crc, l);
		kunmap_atomic(mapped_buffer);
		len -= l;
		if (len == 0)
			break;
		index++;
		BUG_ON(index >= sblock->page_count);
		BUG_ON(!sblock->pagev[index].page);
		page = sblock->pagev[index].page;
		mapped_buffer = kmap_atomic(page);
		mapped_size = PAGE_SIZE;
		p = mapped_buffer;
	}

	btrfs_csum_final(crc, calculated_csum);
	if (memcmp(calculated_csum, on_disk_csum, sdev->csum_size))
		++crc_fail;

	return fail || crc_fail;
}

static int scrub_checksum_super(struct scrub_block *sblock)
{
	struct btrfs_super_block *s;
	struct scrub_dev *sdev = sblock->sdev;
	struct btrfs_root *root = sdev->dev->dev_root;
	struct btrfs_fs_info *fs_info = root->fs_info;
	u8 calculated_csum[BTRFS_CSUM_SIZE];
	u8 on_disk_csum[BTRFS_CSUM_SIZE];
	struct page *page;
	void *mapped_buffer;
	u64 mapped_size;
	void *p;
	u32 crc = ~(u32)0;
	int fail = 0;
	u64 len;
	int index;

	BUG_ON(sblock->page_count < 1);
	page = sblock->pagev[0].page;
	mapped_buffer = kmap_atomic(page);
	s = (struct btrfs_super_block *)mapped_buffer;
	memcpy(on_disk_csum, s->csum, sdev->csum_size);

	if (sblock->pagev[0].logical != le64_to_cpu(s->bytenr))
		++fail;

	if (sblock->pagev[0].generation != le64_to_cpu(s->generation))
		++fail;

	if (memcmp(s->fsid, fs_info->fsid, BTRFS_UUID_SIZE))
		++fail;

	len = BTRFS_SUPER_INFO_SIZE - BTRFS_CSUM_SIZE;
	mapped_size = PAGE_SIZE - BTRFS_CSUM_SIZE;
	p = ((u8 *)mapped_buffer) + BTRFS_CSUM_SIZE;
	index = 0;
	for (;;) {
		u64 l = min_t(u64, len, mapped_size);

		crc = btrfs_csum_data(root, p, crc, l);
		kunmap_atomic(mapped_buffer);
		len -= l;
		if (len == 0)
			break;
		index++;
		BUG_ON(index >= sblock->page_count);
		BUG_ON(!sblock->pagev[index].page);
		page = sblock->pagev[index].page;
		mapped_buffer = kmap_atomic(page);
		mapped_size = PAGE_SIZE;
		p = mapped_buffer;
	}

	btrfs_csum_final(crc, calculated_csum);
	if (memcmp(calculated_csum, on_disk_csum, sdev->csum_size))
		++fail;

	if (fail) {
		/*
		 * if we find an error in a super block, we just report it.
		 * They will get written with the next transaction commit
		 * anyway
		 */
		spin_lock(&sdev->stat_lock);
		++sdev->stat.super_errors;
		spin_unlock(&sdev->stat_lock);
	}

	return fail;
}

static void scrub_block_get(struct scrub_block *sblock)
{
	atomic_inc(&sblock->ref_count);
}

static void scrub_block_put(struct scrub_block *sblock)
{
	if (atomic_dec_and_test(&sblock->ref_count)) {
		int i;

		for (i = 0; i < sblock->page_count; i++)
			if (sblock->pagev[i].page)
				__free_page(sblock->pagev[i].page);
		kfree(sblock);
	}
}

static void scrub_submit(struct scrub_dev *sdev)
{
	struct scrub_bio *sbio;

	if (sdev->curr == -1)
		return;

	sbio = sdev->bios[sdev->curr];
	sdev->curr = -1;
	atomic_inc(&sdev->in_flight);

	btrfsic_submit_bio(READ, sbio->bio);
}

static int scrub_add_page_to_bio(struct scrub_dev *sdev,
				 struct scrub_page *spage)
{
	struct scrub_block *sblock = spage->sblock;
	struct scrub_bio *sbio;
	int ret;

again:
	/*
	 * grab a fresh bio or wait for one to become available
	 */
	while (sdev->curr == -1) {
		spin_lock(&sdev->list_lock);
		sdev->curr = sdev->first_free;
		if (sdev->curr != -1) {
			sdev->first_free = sdev->bios[sdev->curr]->next_free;
			sdev->bios[sdev->curr]->next_free = -1;
			sdev->bios[sdev->curr]->page_count = 0;
			spin_unlock(&sdev->list_lock);
		} else {
			spin_unlock(&sdev->list_lock);
			wait_event(sdev->list_wait, sdev->first_free != -1);
		}
	}
	sbio = sdev->bios[sdev->curr];
	if (sbio->page_count == 0) {
		struct bio *bio;

		sbio->physical = spage->physical;
		sbio->logical = spage->logical;
		bio = sbio->bio;
		if (!bio) {
			bio = bio_alloc(GFP_NOFS, sdev->pages_per_bio);
			if (!bio)
				return -ENOMEM;
			sbio->bio = bio;
		}

		bio->bi_private = sbio;
		bio->bi_end_io = scrub_bio_end_io;
		bio->bi_bdev = sdev->dev->bdev;
		bio->bi_sector = spage->physical >> 9;
		sbio->err = 0;
	} else if (sbio->physical + sbio->page_count * PAGE_SIZE !=
		   spage->physical ||
		   sbio->logical + sbio->page_count * PAGE_SIZE !=
		   spage->logical) {
		scrub_submit(sdev);
		goto again;
	}

	sbio->pagev[sbio->page_count] = spage;
	ret = bio_add_page(sbio->bio, spage->page, PAGE_SIZE, 0);
	if (ret != PAGE_SIZE) {
		if (sbio->page_count < 1) {
			bio_put(sbio->bio);
			sbio->bio = NULL;
			return -EIO;
		}
		scrub_submit(sdev);
		goto again;
	}

	scrub_block_get(sblock); /* one for the added page */
	atomic_inc(&sblock->outstanding_pages);
	sbio->page_count++;
	if (sbio->page_count == sdev->pages_per_bio)
		scrub_submit(sdev);

	return 0;
}

static int scrub_pages(struct scrub_dev *sdev, u64 logical, u64 len,
		       u64 physical, u64 flags, u64 gen, int mirror_num,
		       u8 *csum, int force)
{
	struct scrub_block *sblock;
	int index;

	sblock = kzalloc(sizeof(*sblock), GFP_NOFS);
	if (!sblock) {
		spin_lock(&sdev->stat_lock);
		sdev->stat.malloc_errors++;
		spin_unlock(&sdev->stat_lock);
		return -ENOMEM;
	}

	/* one ref inside this function, plus one for each page later on */
	atomic_set(&sblock->ref_count, 1);
	sblock->sdev = sdev;
	sblock->no_io_error_seen = 1;

	for (index = 0; len > 0; index++) {
		struct scrub_page *spage = sblock->pagev + index;
		u64 l = min_t(u64, len, PAGE_SIZE);

		BUG_ON(index >= SCRUB_MAX_PAGES_PER_BLOCK);
		spage->page = alloc_page(GFP_NOFS);
		if (!spage->page) {
			spin_lock(&sdev->stat_lock);
			sdev->stat.malloc_errors++;
			spin_unlock(&sdev->stat_lock);
			while (index > 0) {
				index--;
				__free_page(sblock->pagev[index].page);
			}
			kfree(sblock);
			return -ENOMEM;
		}
		spage->sblock = sblock;
		spage->bdev = sdev->dev->bdev;
		spage->flags = flags;
		spage->generation = gen;
		spage->logical = logical;
		spage->physical = physical;
		spage->mirror_num = mirror_num;
		if (csum) {
			spage->have_csum = 1;
			memcpy(spage->csum, csum, sdev->csum_size);
		} else {
			spage->have_csum = 0;
		}
		sblock->page_count++;
		len -= l;
		logical += l;
		physical += l;
	}

	BUG_ON(sblock->page_count == 0);
	for (index = 0; index < sblock->page_count; index++) {
		struct scrub_page *spage = sblock->pagev + index;
		int ret;

		ret = scrub_add_page_to_bio(sdev, spage);
		if (ret) {
			scrub_block_put(sblock);
			return ret;
		}
	}

	if (force)
		scrub_submit(sdev);

	/* last one frees, either here or in bio completion for last page */
	scrub_block_put(sblock);
	return 0;
}

static void scrub_bio_end_io(struct bio *bio, int err)
{
	struct scrub_bio *sbio = bio->bi_private;
	struct scrub_dev *sdev = sbio->sdev;
	struct btrfs_fs_info *fs_info = sdev->dev->dev_root->fs_info;

	sbio->err = err;
	sbio->bio = bio;

	btrfs_queue_worker(&fs_info->scrub_workers, &sbio->work);
}

static void scrub_bio_end_io_worker(struct btrfs_work *work)
{
	struct scrub_bio *sbio = container_of(work, struct scrub_bio, work);
	struct scrub_dev *sdev = sbio->sdev;
	int i;

	BUG_ON(sbio->page_count > SCRUB_PAGES_PER_BIO);
	if (sbio->err) {
		for (i = 0; i < sbio->page_count; i++) {
			struct scrub_page *spage = sbio->pagev[i];

			spage->io_error = 1;
			spage->sblock->no_io_error_seen = 0;
		}
	}

	/* now complete the scrub_block items that have all pages completed */
	for (i = 0; i < sbio->page_count; i++) {
		struct scrub_page *spage = sbio->pagev[i];
		struct scrub_block *sblock = spage->sblock;

		if (atomic_dec_and_test(&sblock->outstanding_pages))
			scrub_block_complete(sblock);
		scrub_block_put(sblock);
	}

	if (sbio->err) {
		/* what is this good for??? */
		sbio->bio->bi_flags &= ~(BIO_POOL_MASK - 1);
		sbio->bio->bi_flags |= 1 << BIO_UPTODATE;
		sbio->bio->bi_phys_segments = 0;
		sbio->bio->bi_idx = 0;

		for (i = 0; i < sbio->page_count; i++) {
			struct bio_vec *bi;
			bi = &sbio->bio->bi_io_vec[i];
			bi->bv_offset = 0;
			bi->bv_len = PAGE_SIZE;
		}
	}

	bio_put(sbio->bio);
	sbio->bio = NULL;
	spin_lock(&sdev->list_lock);
	sbio->next_free = sdev->first_free;
	sdev->first_free = sbio->index;
	spin_unlock(&sdev->list_lock);
	atomic_dec(&sdev->in_flight);
	wake_up(&sdev->list_wait);
}

static void scrub_block_complete(struct scrub_block *sblock)
{
	if (!sblock->no_io_error_seen)
		scrub_handle_errored_block(sblock);
	else
		scrub_checksum(sblock);
}

static int scrub_find_csum(struct scrub_dev *sdev, u64 logical, u64 len,
			   u8 *csum)
{
	struct btrfs_ordered_sum *sum = NULL;
	int ret = 0;
	unsigned long i;
	unsigned long num_sectors;

	while (!list_empty(&sdev->csum_list)) {
		sum = list_first_entry(&sdev->csum_list,
				       struct btrfs_ordered_sum, list);
		if (sum->bytenr > logical)
			return 0;
		if (sum->bytenr + sum->len > logical)
			break;

		++sdev->stat.csum_discards;
		list_del(&sum->list);
		kfree(sum);
		sum = NULL;
	}
	if (!sum)
		return 0;

	num_sectors = sum->len / sdev->sectorsize;
	for (i = 0; i < num_sectors; ++i) {
		if (sum->sums[i].bytenr == logical) {
			memcpy(csum, &sum->sums[i].sum, sdev->csum_size);
			ret = 1;
			break;
		}
	}
	if (ret && i == num_sectors - 1) {
		list_del(&sum->list);
		kfree(sum);
	}
	return ret;
}

/* scrub extent tries to collect up to 64 kB for each bio */
static int scrub_extent(struct scrub_dev *sdev, u64 logical, u64 len,
			u64 physical, u64 flags, u64 gen, int mirror_num)
{
	int ret;
	u8 csum[BTRFS_CSUM_SIZE];
	u32 blocksize;

	if (flags & BTRFS_EXTENT_FLAG_DATA) {
		blocksize = sdev->sectorsize;
		spin_lock(&sdev->stat_lock);
		sdev->stat.data_extents_scrubbed++;
		sdev->stat.data_bytes_scrubbed += len;
		spin_unlock(&sdev->stat_lock);
	} else if (flags & BTRFS_EXTENT_FLAG_TREE_BLOCK) {
		BUG_ON(sdev->nodesize != sdev->leafsize);
		blocksize = sdev->nodesize;
		spin_lock(&sdev->stat_lock);
		sdev->stat.tree_extents_scrubbed++;
		sdev->stat.tree_bytes_scrubbed += len;
		spin_unlock(&sdev->stat_lock);
	} else {
		blocksize = sdev->sectorsize;
		BUG_ON(1);
	}

	while (len) {
		u64 l = min_t(u64, len, blocksize);
		int have_csum = 0;

		if (flags & BTRFS_EXTENT_FLAG_DATA) {
			/* push csums to sbio */
			have_csum = scrub_find_csum(sdev, logical, l, csum);
			if (have_csum == 0)
				++sdev->stat.no_csum;
		}
		ret = scrub_pages(sdev, logical, l, physical, flags, gen,
				  mirror_num, have_csum ? csum : NULL, 0);
		if (ret)
			return ret;
		len -= l;
		logical += l;
		physical += l;
	}
	return 0;
}

static noinline_for_stack int scrub_stripe(struct scrub_dev *sdev,
	struct map_lookup *map, int num, u64 base, u64 length)
{
	struct btrfs_path *path;
	struct btrfs_fs_info *fs_info = sdev->dev->dev_root->fs_info;
	struct btrfs_root *root = fs_info->extent_root;
	struct btrfs_root *csum_root = fs_info->csum_root;
	struct btrfs_extent_item *extent;
	struct blk_plug plug;
	u64 flags;
	int ret;
	int slot;
	int i;
	u64 nstripes;
	struct extent_buffer *l;
	struct btrfs_key key;
	u64 physical;
	u64 logical;
	u64 generation;
	int mirror_num;
	struct reada_control *reada1;
	struct reada_control *reada2;
	struct btrfs_key key_start;
	struct btrfs_key key_end;

	u64 increment = map->stripe_len;
	u64 offset;

	nstripes = length;
	offset = 0;
	do_div(nstripes, map->stripe_len);
	if (map->type & BTRFS_BLOCK_GROUP_RAID0) {
		offset = map->stripe_len * num;
		increment = map->stripe_len * map->num_stripes;
		mirror_num = 1;
	} else if (map->type & BTRFS_BLOCK_GROUP_RAID10) {
		int factor = map->num_stripes / map->sub_stripes;
		offset = map->stripe_len * (num / map->sub_stripes);
		increment = map->stripe_len * factor;
		mirror_num = num % map->sub_stripes + 1;
	} else if (map->type & BTRFS_BLOCK_GROUP_RAID1) {
		increment = map->stripe_len;
		mirror_num = num % map->num_stripes + 1;
	} else if (map->type & BTRFS_BLOCK_GROUP_DUP) {
		increment = map->stripe_len;
		mirror_num = num % map->num_stripes + 1;
	} else {
		increment = map->stripe_len;
		mirror_num = 1;
	}

	path = btrfs_alloc_path();
	if (!path)
		return -ENOMEM;

	/*
	 * work on commit root. The related disk blocks are static as
	 * long as COW is applied. This means, it is save to rewrite
	 * them to repair disk errors without any race conditions
	 */
	path->search_commit_root = 1;
	path->skip_locking = 1;

	/*
	 * trigger the readahead for extent tree csum tree and wait for
	 * completion. During readahead, the scrub is officially paused
	 * to not hold off transaction commits
	 */
	logical = base + offset;

	wait_event(sdev->list_wait,
		   atomic_read(&sdev->in_flight) == 0);
	atomic_inc(&fs_info->scrubs_paused);
	wake_up(&fs_info->scrub_pause_wait);

	/* FIXME it might be better to start readahead at commit root */
	key_start.objectid = logical;
	key_start.type = BTRFS_EXTENT_ITEM_KEY;
	key_start.offset = (u64)0;
	key_end.objectid = base + offset + nstripes * increment;
	key_end.type = BTRFS_EXTENT_ITEM_KEY;
	key_end.offset = (u64)0;
	reada1 = btrfs_reada_add(root, &key_start, &key_end);

	key_start.objectid = BTRFS_EXTENT_CSUM_OBJECTID;
	key_start.type = BTRFS_EXTENT_CSUM_KEY;
	key_start.offset = logical;
	key_end.objectid = BTRFS_EXTENT_CSUM_OBJECTID;
	key_end.type = BTRFS_EXTENT_CSUM_KEY;
	key_end.offset = base + offset + nstripes * increment;
	reada2 = btrfs_reada_add(csum_root, &key_start, &key_end);

	if (!IS_ERR(reada1))
		btrfs_reada_wait(reada1);
	if (!IS_ERR(reada2))
		btrfs_reada_wait(reada2);

	mutex_lock(&fs_info->scrub_lock);
	while (atomic_read(&fs_info->scrub_pause_req)) {
		mutex_unlock(&fs_info->scrub_lock);
		wait_event(fs_info->scrub_pause_wait,
		   atomic_read(&fs_info->scrub_pause_req) == 0);
		mutex_lock(&fs_info->scrub_lock);
	}
	atomic_dec(&fs_info->scrubs_paused);
	mutex_unlock(&fs_info->scrub_lock);
	wake_up(&fs_info->scrub_pause_wait);

	/*
	 * collect all data csums for the stripe to avoid seeking during
	 * the scrub. This might currently (crc32) end up to be about 1MB
	 */
	blk_start_plug(&plug);

	/*
	 * now find all extents for each stripe and scrub them
	 */
	logical = base + offset;
	physical = map->stripes[num].physical;
	ret = 0;
	for (i = 0; i < nstripes; ++i) {
		/*
		 * canceled?
		 */
		if (atomic_read(&fs_info->scrub_cancel_req) ||
		    atomic_read(&sdev->cancel_req)) {
			ret = -ECANCELED;
			goto out;
		}
		/*
		 * check to see if we have to pause
		 */
		if (atomic_read(&fs_info->scrub_pause_req)) {
			/* push queued extents */
			scrub_submit(sdev);
			wait_event(sdev->list_wait,
				   atomic_read(&sdev->in_flight) == 0);
			atomic_inc(&fs_info->scrubs_paused);
			wake_up(&fs_info->scrub_pause_wait);
			mutex_lock(&fs_info->scrub_lock);
			while (atomic_read(&fs_info->scrub_pause_req)) {
				mutex_unlock(&fs_info->scrub_lock);
				wait_event(fs_info->scrub_pause_wait,
				   atomic_read(&fs_info->scrub_pause_req) == 0);
				mutex_lock(&fs_info->scrub_lock);
			}
			atomic_dec(&fs_info->scrubs_paused);
			mutex_unlock(&fs_info->scrub_lock);
			wake_up(&fs_info->scrub_pause_wait);
		}

		ret = btrfs_lookup_csums_range(csum_root, logical,
					       logical + map->stripe_len - 1,
					       &sdev->csum_list, 1);
		if (ret)
			goto out;

		key.objectid = logical;
		key.type = BTRFS_EXTENT_ITEM_KEY;
		key.offset = (u64)0;

		ret = btrfs_search_slot(NULL, root, &key, path, 0, 0);
		if (ret < 0)
			goto out;
		if (ret > 0) {
			ret = btrfs_previous_item(root, path, 0,
						  BTRFS_EXTENT_ITEM_KEY);
			if (ret < 0)
				goto out;
			if (ret > 0) {
				/* there's no smaller item, so stick with the
				 * larger one */
				btrfs_release_path(path);
				ret = btrfs_search_slot(NULL, root, &key,
							path, 0, 0);
				if (ret < 0)
					goto out;
			}
		}

		while (1) {
			l = path->nodes[0];
			slot = path->slots[0];
			if (slot >= btrfs_header_nritems(l)) {
				ret = btrfs_next_leaf(root, path);
				if (ret == 0)
					continue;
				if (ret < 0)
					goto out;

				break;
			}
			btrfs_item_key_to_cpu(l, &key, slot);

			if (key.objectid + key.offset <= logical)
				goto next;

			if (key.objectid >= logical + map->stripe_len)
				break;

			if (btrfs_key_type(&key) != BTRFS_EXTENT_ITEM_KEY)
				goto next;

			extent = btrfs_item_ptr(l, slot,
						struct btrfs_extent_item);
			flags = btrfs_extent_flags(l, extent);
			generation = btrfs_extent_generation(l, extent);

			if (key.objectid < logical &&
			    (flags & BTRFS_EXTENT_FLAG_TREE_BLOCK)) {
				printk(KERN_ERR
				       "btrfs scrub: tree block %llu spanning "
				       "stripes, ignored. logical=%llu\n",
				       (unsigned long long)key.objectid,
				       (unsigned long long)logical);
				goto next;
			}

			/*
			 * trim extent to this stripe
			 */
			if (key.objectid < logical) {
				key.offset -= logical - key.objectid;
				key.objectid = logical;
			}
			if (key.objectid + key.offset >
			    logical + map->stripe_len) {
				key.offset = logical + map->stripe_len -
					     key.objectid;
			}

			ret = scrub_extent(sdev, key.objectid, key.offset,
					   key.objectid - logical + physical,
					   flags, generation, mirror_num);
			if (ret)
				goto out;

next:
			path->slots[0]++;
		}
		btrfs_release_path(path);
		logical += increment;
		physical += map->stripe_len;
		spin_lock(&sdev->stat_lock);
		sdev->stat.last_physical = physical;
		spin_unlock(&sdev->stat_lock);
	}
	/* push queued extents */
	scrub_submit(sdev);

out:
	blk_finish_plug(&plug);
	btrfs_free_path(path);
	return ret < 0 ? ret : 0;
}

static noinline_for_stack int scrub_chunk(struct scrub_dev *sdev,
	u64 chunk_tree, u64 chunk_objectid, u64 chunk_offset, u64 length,
	u64 dev_offset)
{
	struct btrfs_mapping_tree *map_tree =
		&sdev->dev->dev_root->fs_info->mapping_tree;
	struct map_lookup *map;
	struct extent_map *em;
	int i;
	int ret = -EINVAL;

	read_lock(&map_tree->map_tree.lock);
	em = lookup_extent_mapping(&map_tree->map_tree, chunk_offset, 1);
	read_unlock(&map_tree->map_tree.lock);

	if (!em)
		return -EINVAL;

	map = (struct map_lookup *)em->bdev;
	if (em->start != chunk_offset)
		goto out;

	if (em->len < length)
		goto out;

	for (i = 0; i < map->num_stripes; ++i) {
		if (map->stripes[i].dev == sdev->dev &&
		    map->stripes[i].physical == dev_offset) {
			ret = scrub_stripe(sdev, map, i, chunk_offset, length);
=======
	item_size = btrfs_item_size(eb, path->slots[0]);

	if (flags & BTRFS_EXTENT_FLAG_TREE_BLOCK) {
		unsigned long ptr = 0;
		u8 ref_level;
		u64 ref_root;

		while (true) {
			ret = tree_backref_for_extent(&ptr, eb, &found_key, ei,
						      item_size, &ref_root,
						      &ref_level);
			if (ret < 0) {
				btrfs_warn(fs_info,
				"failed to resolve tree backref for logical %llu: %d",
						  swarn.logical, ret);
				break;
			}
			if (ret > 0)
				break;
			btrfs_warn_in_rcu(fs_info,
"%s at logical %llu on dev %s, physical %llu: metadata %s (level %d) in tree %llu",
				errstr, swarn.logical, btrfs_dev_name(dev),
				swarn.physical, (ref_level ? "node" : "leaf"),
				ref_level, ref_root);
		}
		btrfs_release_path(path);
	} else {
		struct btrfs_backref_walk_ctx ctx = { 0 };

		btrfs_release_path(path);

		ctx.bytenr = found_key.objectid;
		ctx.extent_item_pos = swarn.logical - found_key.objectid;
		ctx.fs_info = fs_info;

		swarn.path = path;
		swarn.dev = dev;

		iterate_extent_inodes(&ctx, true, scrub_print_warning_inode, &swarn);
	}

out:
	btrfs_free_path(path);
}

static int fill_writer_pointer_gap(struct scrub_ctx *sctx, u64 physical)
{
	int ret = 0;
	u64 length;

	if (!btrfs_is_zoned(sctx->fs_info))
		return 0;

	if (!btrfs_dev_is_sequential(sctx->wr_tgtdev, physical))
		return 0;

	if (sctx->write_pointer < physical) {
		length = physical - sctx->write_pointer;

		ret = btrfs_zoned_issue_zeroout(sctx->wr_tgtdev,
						sctx->write_pointer, length);
		if (!ret)
			sctx->write_pointer = physical;
	}
	return ret;
}

static struct page *scrub_stripe_get_page(struct scrub_stripe *stripe, int sector_nr)
{
	struct btrfs_fs_info *fs_info = stripe->bg->fs_info;
	int page_index = (sector_nr << fs_info->sectorsize_bits) >> PAGE_SHIFT;

	return stripe->pages[page_index];
}

static unsigned int scrub_stripe_get_page_offset(struct scrub_stripe *stripe,
						 int sector_nr)
{
	struct btrfs_fs_info *fs_info = stripe->bg->fs_info;

	return offset_in_page(sector_nr << fs_info->sectorsize_bits);
}

static void scrub_verify_one_metadata(struct scrub_stripe *stripe, int sector_nr)
{
	struct btrfs_fs_info *fs_info = stripe->bg->fs_info;
	const u32 sectors_per_tree = fs_info->nodesize >> fs_info->sectorsize_bits;
	const u64 logical = stripe->logical + (sector_nr << fs_info->sectorsize_bits);
	const struct page *first_page = scrub_stripe_get_page(stripe, sector_nr);
	const unsigned int first_off = scrub_stripe_get_page_offset(stripe, sector_nr);
	SHASH_DESC_ON_STACK(shash, fs_info->csum_shash);
	u8 on_disk_csum[BTRFS_CSUM_SIZE];
	u8 calculated_csum[BTRFS_CSUM_SIZE];
	struct btrfs_header *header;

	/*
	 * Here we don't have a good way to attach the pages (and subpages)
	 * to a dummy extent buffer, thus we have to directly grab the members
	 * from pages.
	 */
	header = (struct btrfs_header *)(page_address(first_page) + first_off);
	memcpy(on_disk_csum, header->csum, fs_info->csum_size);

	if (logical != btrfs_stack_header_bytenr(header)) {
		bitmap_set(&stripe->csum_error_bitmap, sector_nr, sectors_per_tree);
		bitmap_set(&stripe->error_bitmap, sector_nr, sectors_per_tree);
		btrfs_warn_rl(fs_info,
		"tree block %llu mirror %u has bad bytenr, has %llu want %llu",
			      logical, stripe->mirror_num,
			      btrfs_stack_header_bytenr(header), logical);
		return;
	}
	if (memcmp(header->fsid, fs_info->fs_devices->metadata_uuid,
		   BTRFS_FSID_SIZE) != 0) {
		bitmap_set(&stripe->meta_error_bitmap, sector_nr, sectors_per_tree);
		bitmap_set(&stripe->error_bitmap, sector_nr, sectors_per_tree);
		btrfs_warn_rl(fs_info,
		"tree block %llu mirror %u has bad fsid, has %pU want %pU",
			      logical, stripe->mirror_num,
			      header->fsid, fs_info->fs_devices->fsid);
		return;
	}
	if (memcmp(header->chunk_tree_uuid, fs_info->chunk_tree_uuid,
		   BTRFS_UUID_SIZE) != 0) {
		bitmap_set(&stripe->meta_error_bitmap, sector_nr, sectors_per_tree);
		bitmap_set(&stripe->error_bitmap, sector_nr, sectors_per_tree);
		btrfs_warn_rl(fs_info,
		"tree block %llu mirror %u has bad chunk tree uuid, has %pU want %pU",
			      logical, stripe->mirror_num,
			      header->chunk_tree_uuid, fs_info->chunk_tree_uuid);
		return;
	}

	/* Now check tree block csum. */
	shash->tfm = fs_info->csum_shash;
	crypto_shash_init(shash);
	crypto_shash_update(shash, page_address(first_page) + first_off +
			    BTRFS_CSUM_SIZE, fs_info->sectorsize - BTRFS_CSUM_SIZE);

	for (int i = sector_nr + 1; i < sector_nr + sectors_per_tree; i++) {
		struct page *page = scrub_stripe_get_page(stripe, i);
		unsigned int page_off = scrub_stripe_get_page_offset(stripe, i);

		crypto_shash_update(shash, page_address(page) + page_off,
				    fs_info->sectorsize);
	}

	crypto_shash_final(shash, calculated_csum);
	if (memcmp(calculated_csum, on_disk_csum, fs_info->csum_size) != 0) {
		bitmap_set(&stripe->meta_error_bitmap, sector_nr, sectors_per_tree);
		bitmap_set(&stripe->error_bitmap, sector_nr, sectors_per_tree);
		btrfs_warn_rl(fs_info,
		"tree block %llu mirror %u has bad csum, has " CSUM_FMT " want " CSUM_FMT,
			      logical, stripe->mirror_num,
			      CSUM_FMT_VALUE(fs_info->csum_size, on_disk_csum),
			      CSUM_FMT_VALUE(fs_info->csum_size, calculated_csum));
		return;
	}
	if (stripe->sectors[sector_nr].generation !=
	    btrfs_stack_header_generation(header)) {
		bitmap_set(&stripe->meta_error_bitmap, sector_nr, sectors_per_tree);
		bitmap_set(&stripe->error_bitmap, sector_nr, sectors_per_tree);
		btrfs_warn_rl(fs_info,
		"tree block %llu mirror %u has bad generation, has %llu want %llu",
			      logical, stripe->mirror_num,
			      btrfs_stack_header_generation(header),
			      stripe->sectors[sector_nr].generation);
		return;
	}
	bitmap_clear(&stripe->error_bitmap, sector_nr, sectors_per_tree);
	bitmap_clear(&stripe->csum_error_bitmap, sector_nr, sectors_per_tree);
	bitmap_clear(&stripe->meta_error_bitmap, sector_nr, sectors_per_tree);
}

static void scrub_verify_one_sector(struct scrub_stripe *stripe, int sector_nr)
{
	struct btrfs_fs_info *fs_info = stripe->bg->fs_info;
	struct scrub_sector_verification *sector = &stripe->sectors[sector_nr];
	const u32 sectors_per_tree = fs_info->nodesize >> fs_info->sectorsize_bits;
	struct page *page = scrub_stripe_get_page(stripe, sector_nr);
	unsigned int pgoff = scrub_stripe_get_page_offset(stripe, sector_nr);
	u8 csum_buf[BTRFS_CSUM_SIZE];
	int ret;

	ASSERT(sector_nr >= 0 && sector_nr < stripe->nr_sectors);

	/* Sector not utilized, skip it. */
	if (!test_bit(sector_nr, &stripe->extent_sector_bitmap))
		return;

	/* IO error, no need to check. */
	if (test_bit(sector_nr, &stripe->io_error_bitmap))
		return;

	/* Metadata, verify the full tree block. */
	if (sector->is_metadata) {
		/*
		 * Check if the tree block crosses the stripe boundary.  If
		 * crossed the boundary, we cannot verify it but only give a
		 * warning.
		 *
		 * This can only happen on a very old filesystem where chunks
		 * are not ensured to be stripe aligned.
		 */
		if (unlikely(sector_nr + sectors_per_tree > stripe->nr_sectors)) {
			btrfs_warn_rl(fs_info,
			"tree block at %llu crosses stripe boundary %llu",
				      stripe->logical +
				      (sector_nr << fs_info->sectorsize_bits),
				      stripe->logical);
			return;
		}
		scrub_verify_one_metadata(stripe, sector_nr);
		return;
	}

	/*
	 * Data is easier, we just verify the data csum (if we have it).  For
	 * cases without csum, we have no other choice but to trust it.
	 */
	if (!sector->csum) {
		clear_bit(sector_nr, &stripe->error_bitmap);
		return;
	}

	ret = btrfs_check_sector_csum(fs_info, page, pgoff, csum_buf, sector->csum);
	if (ret < 0) {
		set_bit(sector_nr, &stripe->csum_error_bitmap);
		set_bit(sector_nr, &stripe->error_bitmap);
	} else {
		clear_bit(sector_nr, &stripe->csum_error_bitmap);
		clear_bit(sector_nr, &stripe->error_bitmap);
	}
}

/* Verify specified sectors of a stripe. */
static void scrub_verify_one_stripe(struct scrub_stripe *stripe, unsigned long bitmap)
{
	struct btrfs_fs_info *fs_info = stripe->bg->fs_info;
	const u32 sectors_per_tree = fs_info->nodesize >> fs_info->sectorsize_bits;
	int sector_nr;

	for_each_set_bit(sector_nr, &bitmap, stripe->nr_sectors) {
		scrub_verify_one_sector(stripe, sector_nr);
		if (stripe->sectors[sector_nr].is_metadata)
			sector_nr += sectors_per_tree - 1;
	}
}

static int calc_sector_number(struct scrub_stripe *stripe, struct bio_vec *first_bvec)
{
	int i;

	for (i = 0; i < stripe->nr_sectors; i++) {
		if (scrub_stripe_get_page(stripe, i) == first_bvec->bv_page &&
		    scrub_stripe_get_page_offset(stripe, i) == first_bvec->bv_offset)
			break;
	}
	ASSERT(i < stripe->nr_sectors);
	return i;
}

/*
 * Repair read is different to the regular read:
 *
 * - Only reads the failed sectors
 * - May have extra blocksize limits
 */
static void scrub_repair_read_endio(struct btrfs_bio *bbio)
{
	struct scrub_stripe *stripe = bbio->private;
	struct btrfs_fs_info *fs_info = stripe->bg->fs_info;
	struct bio_vec *bvec;
	int sector_nr = calc_sector_number(stripe, bio_first_bvec_all(&bbio->bio));
	u32 bio_size = 0;
	int i;

	ASSERT(sector_nr < stripe->nr_sectors);

	bio_for_each_bvec_all(bvec, &bbio->bio, i)
		bio_size += bvec->bv_len;

	if (bbio->bio.bi_status) {
		bitmap_set(&stripe->io_error_bitmap, sector_nr,
			   bio_size >> fs_info->sectorsize_bits);
		bitmap_set(&stripe->error_bitmap, sector_nr,
			   bio_size >> fs_info->sectorsize_bits);
	} else {
		bitmap_clear(&stripe->io_error_bitmap, sector_nr,
			     bio_size >> fs_info->sectorsize_bits);
	}
	bio_put(&bbio->bio);
	if (atomic_dec_and_test(&stripe->pending_io))
		wake_up(&stripe->io_wait);
}

static int calc_next_mirror(int mirror, int num_copies)
{
	ASSERT(mirror <= num_copies);
	return (mirror + 1 > num_copies) ? 1 : mirror + 1;
}

static void scrub_stripe_submit_repair_read(struct scrub_stripe *stripe,
					    int mirror, int blocksize, bool wait)
{
	struct btrfs_fs_info *fs_info = stripe->bg->fs_info;
	struct btrfs_bio *bbio = NULL;
	const unsigned long old_error_bitmap = stripe->error_bitmap;
	int i;

	ASSERT(stripe->mirror_num >= 1);
	ASSERT(atomic_read(&stripe->pending_io) == 0);

	for_each_set_bit(i, &old_error_bitmap, stripe->nr_sectors) {
		struct page *page;
		int pgoff;
		int ret;

		page = scrub_stripe_get_page(stripe, i);
		pgoff = scrub_stripe_get_page_offset(stripe, i);

		/* The current sector cannot be merged, submit the bio. */
		if (bbio && ((i > 0 && !test_bit(i - 1, &stripe->error_bitmap)) ||
			     bbio->bio.bi_iter.bi_size >= blocksize)) {
			ASSERT(bbio->bio.bi_iter.bi_size);
			atomic_inc(&stripe->pending_io);
			btrfs_submit_bio(bbio, mirror);
			if (wait)
				wait_scrub_stripe_io(stripe);
			bbio = NULL;
		}

		if (!bbio) {
			bbio = btrfs_bio_alloc(stripe->nr_sectors, REQ_OP_READ,
				fs_info, scrub_repair_read_endio, stripe);
			bbio->bio.bi_iter.bi_sector = (stripe->logical +
				(i << fs_info->sectorsize_bits)) >> SECTOR_SHIFT;
		}

		ret = bio_add_page(&bbio->bio, page, fs_info->sectorsize, pgoff);
		ASSERT(ret == fs_info->sectorsize);
	}
	if (bbio) {
		ASSERT(bbio->bio.bi_iter.bi_size);
		atomic_inc(&stripe->pending_io);
		btrfs_submit_bio(bbio, mirror);
		if (wait)
			wait_scrub_stripe_io(stripe);
	}
}

static void scrub_stripe_report_errors(struct scrub_ctx *sctx,
				       struct scrub_stripe *stripe)
{
	static DEFINE_RATELIMIT_STATE(rs, DEFAULT_RATELIMIT_INTERVAL,
				      DEFAULT_RATELIMIT_BURST);
	struct btrfs_fs_info *fs_info = sctx->fs_info;
	struct btrfs_device *dev = NULL;
	u64 physical = 0;
	int nr_data_sectors = 0;
	int nr_meta_sectors = 0;
	int nr_nodatacsum_sectors = 0;
	int nr_repaired_sectors = 0;
	int sector_nr;

	if (test_bit(SCRUB_STRIPE_FLAG_NO_REPORT, &stripe->state))
		return;

	/*
	 * Init needed infos for error reporting.
	 *
	 * Although our scrub_stripe infrastructure is mostly based on btrfs_submit_bio()
	 * thus no need for dev/physical, error reporting still needs dev and physical.
	 */
	if (!bitmap_empty(&stripe->init_error_bitmap, stripe->nr_sectors)) {
		u64 mapped_len = fs_info->sectorsize;
		struct btrfs_io_context *bioc = NULL;
		int stripe_index = stripe->mirror_num - 1;
		int ret;

		/* For scrub, our mirror_num should always start at 1. */
		ASSERT(stripe->mirror_num >= 1);
		ret = btrfs_map_block(fs_info, BTRFS_MAP_GET_READ_MIRRORS,
				      stripe->logical, &mapped_len, &bioc,
				      NULL, NULL);
		/*
		 * If we failed, dev will be NULL, and later detailed reports
		 * will just be skipped.
		 */
		if (ret < 0)
			goto skip;
		physical = bioc->stripes[stripe_index].physical;
		dev = bioc->stripes[stripe_index].dev;
		btrfs_put_bioc(bioc);
	}

skip:
	for_each_set_bit(sector_nr, &stripe->extent_sector_bitmap, stripe->nr_sectors) {
		bool repaired = false;

		if (stripe->sectors[sector_nr].is_metadata) {
			nr_meta_sectors++;
		} else {
			nr_data_sectors++;
			if (!stripe->sectors[sector_nr].csum)
				nr_nodatacsum_sectors++;
		}

		if (test_bit(sector_nr, &stripe->init_error_bitmap) &&
		    !test_bit(sector_nr, &stripe->error_bitmap)) {
			nr_repaired_sectors++;
			repaired = true;
		}

		/* Good sector from the beginning, nothing need to be done. */
		if (!test_bit(sector_nr, &stripe->init_error_bitmap))
			continue;

		/*
		 * Report error for the corrupted sectors.  If repaired, just
		 * output the message of repaired message.
		 */
		if (repaired) {
			if (dev) {
				btrfs_err_rl_in_rcu(fs_info,
			"fixed up error at logical %llu on dev %s physical %llu",
					    stripe->logical, btrfs_dev_name(dev),
					    physical);
			} else {
				btrfs_err_rl_in_rcu(fs_info,
			"fixed up error at logical %llu on mirror %u",
					    stripe->logical, stripe->mirror_num);
			}
			continue;
		}

		/* The remaining are all for unrepaired. */
		if (dev) {
			btrfs_err_rl_in_rcu(fs_info,
	"unable to fixup (regular) error at logical %llu on dev %s physical %llu",
					    stripe->logical, btrfs_dev_name(dev),
					    physical);
		} else {
			btrfs_err_rl_in_rcu(fs_info,
	"unable to fixup (regular) error at logical %llu on mirror %u",
					    stripe->logical, stripe->mirror_num);
		}

		if (test_bit(sector_nr, &stripe->io_error_bitmap))
			if (__ratelimit(&rs) && dev)
				scrub_print_common_warning("i/o error", dev, false,
						     stripe->logical, physical);
		if (test_bit(sector_nr, &stripe->csum_error_bitmap))
			if (__ratelimit(&rs) && dev)
				scrub_print_common_warning("checksum error", dev, false,
						     stripe->logical, physical);
		if (test_bit(sector_nr, &stripe->meta_error_bitmap))
			if (__ratelimit(&rs) && dev)
				scrub_print_common_warning("header error", dev, false,
						     stripe->logical, physical);
	}

	spin_lock(&sctx->stat_lock);
	sctx->stat.data_extents_scrubbed += stripe->nr_data_extents;
	sctx->stat.tree_extents_scrubbed += stripe->nr_meta_extents;
	sctx->stat.data_bytes_scrubbed += nr_data_sectors << fs_info->sectorsize_bits;
	sctx->stat.tree_bytes_scrubbed += nr_meta_sectors << fs_info->sectorsize_bits;
	sctx->stat.no_csum += nr_nodatacsum_sectors;
	sctx->stat.read_errors += stripe->init_nr_io_errors;
	sctx->stat.csum_errors += stripe->init_nr_csum_errors;
	sctx->stat.verify_errors += stripe->init_nr_meta_errors;
	sctx->stat.uncorrectable_errors +=
		bitmap_weight(&stripe->error_bitmap, stripe->nr_sectors);
	sctx->stat.corrected_errors += nr_repaired_sectors;
	spin_unlock(&sctx->stat_lock);
}

static void scrub_write_sectors(struct scrub_ctx *sctx, struct scrub_stripe *stripe,
				unsigned long write_bitmap, bool dev_replace);

/*
 * The main entrance for all read related scrub work, including:
 *
 * - Wait for the initial read to finish
 * - Verify and locate any bad sectors
 * - Go through the remaining mirrors and try to read as large blocksize as
 *   possible
 * - Go through all mirrors (including the failed mirror) sector-by-sector
 * - Submit writeback for repaired sectors
 *
 * Writeback for dev-replace does not happen here, it needs extra
 * synchronization for zoned devices.
 */
static void scrub_stripe_read_repair_worker(struct work_struct *work)
{
	struct scrub_stripe *stripe = container_of(work, struct scrub_stripe, work);
	struct scrub_ctx *sctx = stripe->sctx;
	struct btrfs_fs_info *fs_info = sctx->fs_info;
	int num_copies = btrfs_num_copies(fs_info, stripe->bg->start,
					  stripe->bg->length);
	unsigned long repaired;
	int mirror;
	int i;

	ASSERT(stripe->mirror_num > 0);

	wait_scrub_stripe_io(stripe);
	scrub_verify_one_stripe(stripe, stripe->extent_sector_bitmap);
	/* Save the initial failed bitmap for later repair and report usage. */
	stripe->init_error_bitmap = stripe->error_bitmap;
	stripe->init_nr_io_errors = bitmap_weight(&stripe->io_error_bitmap,
						  stripe->nr_sectors);
	stripe->init_nr_csum_errors = bitmap_weight(&stripe->csum_error_bitmap,
						    stripe->nr_sectors);
	stripe->init_nr_meta_errors = bitmap_weight(&stripe->meta_error_bitmap,
						    stripe->nr_sectors);

	if (bitmap_empty(&stripe->init_error_bitmap, stripe->nr_sectors))
		goto out;

	/*
	 * Try all remaining mirrors.
	 *
	 * Here we still try to read as large block as possible, as this is
	 * faster and we have extra safety nets to rely on.
	 */
	for (mirror = calc_next_mirror(stripe->mirror_num, num_copies);
	     mirror != stripe->mirror_num;
	     mirror = calc_next_mirror(mirror, num_copies)) {
		const unsigned long old_error_bitmap = stripe->error_bitmap;

		scrub_stripe_submit_repair_read(stripe, mirror,
						BTRFS_STRIPE_LEN, false);
		wait_scrub_stripe_io(stripe);
		scrub_verify_one_stripe(stripe, old_error_bitmap);
		if (bitmap_empty(&stripe->error_bitmap, stripe->nr_sectors))
			goto out;
	}

	/*
	 * Last safety net, try re-checking all mirrors, including the failed
	 * one, sector-by-sector.
	 *
	 * As if one sector failed the drive's internal csum, the whole read
	 * containing the offending sector would be marked as error.
	 * Thus here we do sector-by-sector read.
	 *
	 * This can be slow, thus we only try it as the last resort.
	 */

	for (i = 0, mirror = stripe->mirror_num;
	     i < num_copies;
	     i++, mirror = calc_next_mirror(mirror, num_copies)) {
		const unsigned long old_error_bitmap = stripe->error_bitmap;

		scrub_stripe_submit_repair_read(stripe, mirror,
						fs_info->sectorsize, true);
		wait_scrub_stripe_io(stripe);
		scrub_verify_one_stripe(stripe, old_error_bitmap);
		if (bitmap_empty(&stripe->error_bitmap, stripe->nr_sectors))
			goto out;
	}
out:
	/*
	 * Submit the repaired sectors.  For zoned case, we cannot do repair
	 * in-place, but queue the bg to be relocated.
	 */
	bitmap_andnot(&repaired, &stripe->init_error_bitmap, &stripe->error_bitmap,
		      stripe->nr_sectors);
	if (!sctx->readonly && !bitmap_empty(&repaired, stripe->nr_sectors)) {
		if (btrfs_is_zoned(fs_info)) {
			btrfs_repair_one_zone(fs_info, sctx->stripes[0].bg->start);
		} else {
			scrub_write_sectors(sctx, stripe, repaired, false);
			wait_scrub_stripe_io(stripe);
		}
	}

	scrub_stripe_report_errors(sctx, stripe);
	set_bit(SCRUB_STRIPE_FLAG_REPAIR_DONE, &stripe->state);
	wake_up(&stripe->repair_wait);
}

static void scrub_read_endio(struct btrfs_bio *bbio)
{
	struct scrub_stripe *stripe = bbio->private;
	struct bio_vec *bvec;
	int sector_nr = calc_sector_number(stripe, bio_first_bvec_all(&bbio->bio));
	int num_sectors;
	u32 bio_size = 0;
	int i;

	ASSERT(sector_nr < stripe->nr_sectors);
	bio_for_each_bvec_all(bvec, &bbio->bio, i)
		bio_size += bvec->bv_len;
	num_sectors = bio_size >> stripe->bg->fs_info->sectorsize_bits;

	if (bbio->bio.bi_status) {
		bitmap_set(&stripe->io_error_bitmap, sector_nr, num_sectors);
		bitmap_set(&stripe->error_bitmap, sector_nr, num_sectors);
	} else {
		bitmap_clear(&stripe->io_error_bitmap, sector_nr, num_sectors);
	}
	bio_put(&bbio->bio);
	if (atomic_dec_and_test(&stripe->pending_io)) {
		wake_up(&stripe->io_wait);
		INIT_WORK(&stripe->work, scrub_stripe_read_repair_worker);
		queue_work(stripe->bg->fs_info->scrub_workers, &stripe->work);
	}
}

static void scrub_write_endio(struct btrfs_bio *bbio)
{
	struct scrub_stripe *stripe = bbio->private;
	struct btrfs_fs_info *fs_info = stripe->bg->fs_info;
	struct bio_vec *bvec;
	int sector_nr = calc_sector_number(stripe, bio_first_bvec_all(&bbio->bio));
	u32 bio_size = 0;
	int i;

	bio_for_each_bvec_all(bvec, &bbio->bio, i)
		bio_size += bvec->bv_len;

	if (bbio->bio.bi_status) {
		unsigned long flags;

		spin_lock_irqsave(&stripe->write_error_lock, flags);
		bitmap_set(&stripe->write_error_bitmap, sector_nr,
			   bio_size >> fs_info->sectorsize_bits);
		spin_unlock_irqrestore(&stripe->write_error_lock, flags);
	}
	bio_put(&bbio->bio);

	if (atomic_dec_and_test(&stripe->pending_io))
		wake_up(&stripe->io_wait);
}

static void scrub_submit_write_bio(struct scrub_ctx *sctx,
				   struct scrub_stripe *stripe,
				   struct btrfs_bio *bbio, bool dev_replace)
{
	struct btrfs_fs_info *fs_info = sctx->fs_info;
	u32 bio_len = bbio->bio.bi_iter.bi_size;
	u32 bio_off = (bbio->bio.bi_iter.bi_sector << SECTOR_SHIFT) -
		      stripe->logical;

	fill_writer_pointer_gap(sctx, stripe->physical + bio_off);
	atomic_inc(&stripe->pending_io);
	btrfs_submit_repair_write(bbio, stripe->mirror_num, dev_replace);
	if (!btrfs_is_zoned(fs_info))
		return;
	/*
	 * For zoned writeback, queue depth must be 1, thus we must wait for
	 * the write to finish before the next write.
	 */
	wait_scrub_stripe_io(stripe);

	/*
	 * And also need to update the write pointer if write finished
	 * successfully.
	 */
	if (!test_bit(bio_off >> fs_info->sectorsize_bits,
		      &stripe->write_error_bitmap))
		sctx->write_pointer += bio_len;
}

/*
 * Submit the write bio(s) for the sectors specified by @write_bitmap.
 *
 * Here we utilize btrfs_submit_repair_write(), which has some extra benefits:
 *
 * - Only needs logical bytenr and mirror_num
 *   Just like the scrub read path
 *
 * - Would only result in writes to the specified mirror
 *   Unlike the regular writeback path, which would write back to all stripes
 *
 * - Handle dev-replace and read-repair writeback differently
 */
static void scrub_write_sectors(struct scrub_ctx *sctx, struct scrub_stripe *stripe,
				unsigned long write_bitmap, bool dev_replace)
{
	struct btrfs_fs_info *fs_info = stripe->bg->fs_info;
	struct btrfs_bio *bbio = NULL;
	int sector_nr;

	for_each_set_bit(sector_nr, &write_bitmap, stripe->nr_sectors) {
		struct page *page = scrub_stripe_get_page(stripe, sector_nr);
		unsigned int pgoff = scrub_stripe_get_page_offset(stripe, sector_nr);
		int ret;

		/* We should only writeback sectors covered by an extent. */
		ASSERT(test_bit(sector_nr, &stripe->extent_sector_bitmap));

		/* Cannot merge with previous sector, submit the current one. */
		if (bbio && sector_nr && !test_bit(sector_nr - 1, &write_bitmap)) {
			scrub_submit_write_bio(sctx, stripe, bbio, dev_replace);
			bbio = NULL;
		}
		if (!bbio) {
			bbio = btrfs_bio_alloc(stripe->nr_sectors, REQ_OP_WRITE,
					       fs_info, scrub_write_endio, stripe);
			bbio->bio.bi_iter.bi_sector = (stripe->logical +
				(sector_nr << fs_info->sectorsize_bits)) >>
				SECTOR_SHIFT;
		}
		ret = bio_add_page(&bbio->bio, page, fs_info->sectorsize, pgoff);
		ASSERT(ret == fs_info->sectorsize);
	}
	if (bbio)
		scrub_submit_write_bio(sctx, stripe, bbio, dev_replace);
}

/*
 * Throttling of IO submission, bandwidth-limit based, the timeslice is 1
 * second.  Limit can be set via /sys/fs/UUID/devinfo/devid/scrub_speed_max.
 */
static void scrub_throttle_dev_io(struct scrub_ctx *sctx, struct btrfs_device *device,
				  unsigned int bio_size)
{
	const int time_slice = 1000;
	s64 delta;
	ktime_t now;
	u32 div;
	u64 bwlimit;

	bwlimit = READ_ONCE(device->scrub_speed_max);
	if (bwlimit == 0)
		return;

	/*
	 * Slice is divided into intervals when the IO is submitted, adjust by
	 * bwlimit and maximum of 64 intervals.
	 */
	div = max_t(u32, 1, (u32)(bwlimit / (16 * 1024 * 1024)));
	div = min_t(u32, 64, div);

	/* Start new epoch, set deadline */
	now = ktime_get();
	if (sctx->throttle_deadline == 0) {
		sctx->throttle_deadline = ktime_add_ms(now, time_slice / div);
		sctx->throttle_sent = 0;
	}

	/* Still in the time to send? */
	if (ktime_before(now, sctx->throttle_deadline)) {
		/* If current bio is within the limit, send it */
		sctx->throttle_sent += bio_size;
		if (sctx->throttle_sent <= div_u64(bwlimit, div))
			return;

		/* We're over the limit, sleep until the rest of the slice */
		delta = ktime_ms_delta(sctx->throttle_deadline, now);
	} else {
		/* New request after deadline, start new epoch */
		delta = 0;
	}

	if (delta) {
		long timeout;

		timeout = div_u64(delta * HZ, 1000);
		schedule_timeout_interruptible(timeout);
	}

	/* Next call will start the deadline period */
	sctx->throttle_deadline = 0;
}

/*
 * Given a physical address, this will calculate it's
 * logical offset. if this is a parity stripe, it will return
 * the most left data stripe's logical offset.
 *
 * return 0 if it is a data stripe, 1 means parity stripe.
 */
static int get_raid56_logic_offset(u64 physical, int num,
				   struct btrfs_chunk_map *map, u64 *offset,
				   u64 *stripe_start)
{
	int i;
	int j = 0;
	u64 last_offset;
	const int data_stripes = nr_data_stripes(map);

	last_offset = (physical - map->stripes[num].physical) * data_stripes;
	if (stripe_start)
		*stripe_start = last_offset;

	*offset = last_offset;
	for (i = 0; i < data_stripes; i++) {
		u32 stripe_nr;
		u32 stripe_index;
		u32 rot;

		*offset = last_offset + btrfs_stripe_nr_to_offset(i);

		stripe_nr = (u32)(*offset >> BTRFS_STRIPE_LEN_SHIFT) / data_stripes;

		/* Work out the disk rotation on this stripe-set */
		rot = stripe_nr % map->num_stripes;
		/* calculate which stripe this data locates */
		rot += i;
		stripe_index = rot % map->num_stripes;
		if (stripe_index == num)
			return 0;
		if (stripe_index < num)
			j++;
	}
	*offset = last_offset + btrfs_stripe_nr_to_offset(j);
	return 1;
}

/*
 * Return 0 if the extent item range covers any byte of the range.
 * Return <0 if the extent item is before @search_start.
 * Return >0 if the extent item is after @start_start + @search_len.
 */
static int compare_extent_item_range(struct btrfs_path *path,
				     u64 search_start, u64 search_len)
{
	struct btrfs_fs_info *fs_info = path->nodes[0]->fs_info;
	u64 len;
	struct btrfs_key key;

	btrfs_item_key_to_cpu(path->nodes[0], &key, path->slots[0]);
	ASSERT(key.type == BTRFS_EXTENT_ITEM_KEY ||
	       key.type == BTRFS_METADATA_ITEM_KEY);
	if (key.type == BTRFS_METADATA_ITEM_KEY)
		len = fs_info->nodesize;
	else
		len = key.offset;

	if (key.objectid + len <= search_start)
		return -1;
	if (key.objectid >= search_start + search_len)
		return 1;
	return 0;
}

/*
 * Locate one extent item which covers any byte in range
 * [@search_start, @search_start + @search_length)
 *
 * If the path is not initialized, we will initialize the search by doing
 * a btrfs_search_slot().
 * If the path is already initialized, we will use the path as the initial
 * slot, to avoid duplicated btrfs_search_slot() calls.
 *
 * NOTE: If an extent item starts before @search_start, we will still
 * return the extent item. This is for data extent crossing stripe boundary.
 *
 * Return 0 if we found such extent item, and @path will point to the extent item.
 * Return >0 if no such extent item can be found, and @path will be released.
 * Return <0 if hit fatal error, and @path will be released.
 */
static int find_first_extent_item(struct btrfs_root *extent_root,
				  struct btrfs_path *path,
				  u64 search_start, u64 search_len)
{
	struct btrfs_fs_info *fs_info = extent_root->fs_info;
	struct btrfs_key key;
	int ret;

	/* Continue using the existing path */
	if (path->nodes[0])
		goto search_forward;

	if (btrfs_fs_incompat(fs_info, SKINNY_METADATA))
		key.type = BTRFS_METADATA_ITEM_KEY;
	else
		key.type = BTRFS_EXTENT_ITEM_KEY;
	key.objectid = search_start;
	key.offset = (u64)-1;

	ret = btrfs_search_slot(NULL, extent_root, &key, path, 0, 0);
	if (ret < 0)
		return ret;
	if (ret == 0) {
		/*
		 * Key with offset -1 found, there would have to exist an extent
		 * item with such offset, but this is out of the valid range.
		 */
		btrfs_release_path(path);
		return -EUCLEAN;
	}

	/*
	 * Here we intentionally pass 0 as @min_objectid, as there could be
	 * an extent item starting before @search_start.
	 */
	ret = btrfs_previous_extent_item(extent_root, path, 0);
	if (ret < 0)
		return ret;
	/*
	 * No matter whether we have found an extent item, the next loop will
	 * properly do every check on the key.
	 */
search_forward:
	while (true) {
		btrfs_item_key_to_cpu(path->nodes[0], &key, path->slots[0]);
		if (key.objectid >= search_start + search_len)
			break;
		if (key.type != BTRFS_METADATA_ITEM_KEY &&
		    key.type != BTRFS_EXTENT_ITEM_KEY)
			goto next;

		ret = compare_extent_item_range(path, search_start, search_len);
		if (ret == 0)
			return ret;
		if (ret > 0)
			break;
next:
		ret = btrfs_next_item(extent_root, path);
		if (ret) {
			/* Either no more items or a fatal error. */
			btrfs_release_path(path);
			return ret;
		}
	}
	btrfs_release_path(path);
	return 1;
}

static void get_extent_info(struct btrfs_path *path, u64 *extent_start_ret,
			    u64 *size_ret, u64 *flags_ret, u64 *generation_ret)
{
	struct btrfs_key key;
	struct btrfs_extent_item *ei;

	btrfs_item_key_to_cpu(path->nodes[0], &key, path->slots[0]);
	ASSERT(key.type == BTRFS_METADATA_ITEM_KEY ||
	       key.type == BTRFS_EXTENT_ITEM_KEY);
	*extent_start_ret = key.objectid;
	if (key.type == BTRFS_METADATA_ITEM_KEY)
		*size_ret = path->nodes[0]->fs_info->nodesize;
	else
		*size_ret = key.offset;
	ei = btrfs_item_ptr(path->nodes[0], path->slots[0], struct btrfs_extent_item);
	*flags_ret = btrfs_extent_flags(path->nodes[0], ei);
	*generation_ret = btrfs_extent_generation(path->nodes[0], ei);
}

static int sync_write_pointer_for_zoned(struct scrub_ctx *sctx, u64 logical,
					u64 physical, u64 physical_end)
{
	struct btrfs_fs_info *fs_info = sctx->fs_info;
	int ret = 0;

	if (!btrfs_is_zoned(fs_info))
		return 0;

	mutex_lock(&sctx->wr_lock);
	if (sctx->write_pointer < physical_end) {
		ret = btrfs_sync_zone_write_pointer(sctx->wr_tgtdev, logical,
						    physical,
						    sctx->write_pointer);
		if (ret)
			btrfs_err(fs_info,
				  "zoned: failed to recover write pointer");
	}
	mutex_unlock(&sctx->wr_lock);
	btrfs_dev_clear_zone_empty(sctx->wr_tgtdev, physical);

	return ret;
}

static void fill_one_extent_info(struct btrfs_fs_info *fs_info,
				 struct scrub_stripe *stripe,
				 u64 extent_start, u64 extent_len,
				 u64 extent_flags, u64 extent_gen)
{
	for (u64 cur_logical = max(stripe->logical, extent_start);
	     cur_logical < min(stripe->logical + BTRFS_STRIPE_LEN,
			       extent_start + extent_len);
	     cur_logical += fs_info->sectorsize) {
		const int nr_sector = (cur_logical - stripe->logical) >>
				      fs_info->sectorsize_bits;
		struct scrub_sector_verification *sector =
						&stripe->sectors[nr_sector];

		set_bit(nr_sector, &stripe->extent_sector_bitmap);
		if (extent_flags & BTRFS_EXTENT_FLAG_TREE_BLOCK) {
			sector->is_metadata = true;
			sector->generation = extent_gen;
		}
	}
}

static void scrub_stripe_reset_bitmaps(struct scrub_stripe *stripe)
{
	stripe->extent_sector_bitmap = 0;
	stripe->init_error_bitmap = 0;
	stripe->init_nr_io_errors = 0;
	stripe->init_nr_csum_errors = 0;
	stripe->init_nr_meta_errors = 0;
	stripe->error_bitmap = 0;
	stripe->io_error_bitmap = 0;
	stripe->csum_error_bitmap = 0;
	stripe->meta_error_bitmap = 0;
}

/*
 * Locate one stripe which has at least one extent in its range.
 *
 * Return 0 if found such stripe, and store its info into @stripe.
 * Return >0 if there is no such stripe in the specified range.
 * Return <0 for error.
 */
static int scrub_find_fill_first_stripe(struct btrfs_block_group *bg,
					struct btrfs_path *extent_path,
					struct btrfs_path *csum_path,
					struct btrfs_device *dev, u64 physical,
					int mirror_num, u64 logical_start,
					u32 logical_len,
					struct scrub_stripe *stripe)
{
	struct btrfs_fs_info *fs_info = bg->fs_info;
	struct btrfs_root *extent_root = btrfs_extent_root(fs_info, bg->start);
	struct btrfs_root *csum_root = btrfs_csum_root(fs_info, bg->start);
	const u64 logical_end = logical_start + logical_len;
	u64 cur_logical = logical_start;
	u64 stripe_end;
	u64 extent_start;
	u64 extent_len;
	u64 extent_flags;
	u64 extent_gen;
	int ret;

	memset(stripe->sectors, 0, sizeof(struct scrub_sector_verification) *
				   stripe->nr_sectors);
	scrub_stripe_reset_bitmaps(stripe);

	/* The range must be inside the bg. */
	ASSERT(logical_start >= bg->start && logical_end <= bg->start + bg->length);

	ret = find_first_extent_item(extent_root, extent_path, logical_start,
				     logical_len);
	/* Either error or not found. */
	if (ret)
		goto out;
	get_extent_info(extent_path, &extent_start, &extent_len, &extent_flags,
			&extent_gen);
	if (extent_flags & BTRFS_EXTENT_FLAG_TREE_BLOCK)
		stripe->nr_meta_extents++;
	if (extent_flags & BTRFS_EXTENT_FLAG_DATA)
		stripe->nr_data_extents++;
	cur_logical = max(extent_start, cur_logical);

	/*
	 * Round down to stripe boundary.
	 *
	 * The extra calculation against bg->start is to handle block groups
	 * whose logical bytenr is not BTRFS_STRIPE_LEN aligned.
	 */
	stripe->logical = round_down(cur_logical - bg->start, BTRFS_STRIPE_LEN) +
			  bg->start;
	stripe->physical = physical + stripe->logical - logical_start;
	stripe->dev = dev;
	stripe->bg = bg;
	stripe->mirror_num = mirror_num;
	stripe_end = stripe->logical + BTRFS_STRIPE_LEN - 1;

	/* Fill the first extent info into stripe->sectors[] array. */
	fill_one_extent_info(fs_info, stripe, extent_start, extent_len,
			     extent_flags, extent_gen);
	cur_logical = extent_start + extent_len;

	/* Fill the extent info for the remaining sectors. */
	while (cur_logical <= stripe_end) {
		ret = find_first_extent_item(extent_root, extent_path, cur_logical,
					     stripe_end - cur_logical + 1);
		if (ret < 0)
			goto out;
		if (ret > 0) {
			ret = 0;
			break;
		}
		get_extent_info(extent_path, &extent_start, &extent_len,
				&extent_flags, &extent_gen);
		if (extent_flags & BTRFS_EXTENT_FLAG_TREE_BLOCK)
			stripe->nr_meta_extents++;
		if (extent_flags & BTRFS_EXTENT_FLAG_DATA)
			stripe->nr_data_extents++;
		fill_one_extent_info(fs_info, stripe, extent_start, extent_len,
				     extent_flags, extent_gen);
		cur_logical = extent_start + extent_len;
	}

	/* Now fill the data csum. */
	if (bg->flags & BTRFS_BLOCK_GROUP_DATA) {
		int sector_nr;
		unsigned long csum_bitmap = 0;

		/* Csum space should have already been allocated. */
		ASSERT(stripe->csums);

		/*
		 * Our csum bitmap should be large enough, as BTRFS_STRIPE_LEN
		 * should contain at most 16 sectors.
		 */
		ASSERT(BITS_PER_LONG >= BTRFS_STRIPE_LEN >> fs_info->sectorsize_bits);

		ret = btrfs_lookup_csums_bitmap(csum_root, csum_path,
						stripe->logical, stripe_end,
						stripe->csums, &csum_bitmap);
		if (ret < 0)
			goto out;
		if (ret > 0)
			ret = 0;

		for_each_set_bit(sector_nr, &csum_bitmap, stripe->nr_sectors) {
			stripe->sectors[sector_nr].csum = stripe->csums +
				sector_nr * fs_info->csum_size;
		}
	}
	set_bit(SCRUB_STRIPE_FLAG_INITIALIZED, &stripe->state);
out:
	return ret;
}

static void scrub_reset_stripe(struct scrub_stripe *stripe)
{
	scrub_stripe_reset_bitmaps(stripe);

	stripe->nr_meta_extents = 0;
	stripe->nr_data_extents = 0;
	stripe->state = 0;

	for (int i = 0; i < stripe->nr_sectors; i++) {
		stripe->sectors[i].is_metadata = false;
		stripe->sectors[i].csum = NULL;
		stripe->sectors[i].generation = 0;
	}
}

static void scrub_submit_extent_sector_read(struct scrub_ctx *sctx,
					    struct scrub_stripe *stripe)
{
	struct btrfs_fs_info *fs_info = stripe->bg->fs_info;
	struct btrfs_bio *bbio = NULL;
	unsigned int nr_sectors = min(BTRFS_STRIPE_LEN, stripe->bg->start +
				      stripe->bg->length - stripe->logical) >>
				  fs_info->sectorsize_bits;
	u64 stripe_len = BTRFS_STRIPE_LEN;
	int mirror = stripe->mirror_num;
	int i;

	atomic_inc(&stripe->pending_io);

	for_each_set_bit(i, &stripe->extent_sector_bitmap, stripe->nr_sectors) {
		struct page *page = scrub_stripe_get_page(stripe, i);
		unsigned int pgoff = scrub_stripe_get_page_offset(stripe, i);

		/* We're beyond the chunk boundary, no need to read anymore. */
		if (i >= nr_sectors)
			break;

		/* The current sector cannot be merged, submit the bio. */
		if (bbio &&
		    ((i > 0 &&
		      !test_bit(i - 1, &stripe->extent_sector_bitmap)) ||
		     bbio->bio.bi_iter.bi_size >= stripe_len)) {
			ASSERT(bbio->bio.bi_iter.bi_size);
			atomic_inc(&stripe->pending_io);
			btrfs_submit_bio(bbio, mirror);
			bbio = NULL;
		}

		if (!bbio) {
			struct btrfs_io_stripe io_stripe = {};
			struct btrfs_io_context *bioc = NULL;
			const u64 logical = stripe->logical +
					    (i << fs_info->sectorsize_bits);
			int err;

			bbio = btrfs_bio_alloc(stripe->nr_sectors, REQ_OP_READ,
					       fs_info, scrub_read_endio, stripe);
			bbio->bio.bi_iter.bi_sector = logical >> SECTOR_SHIFT;

			io_stripe.is_scrub = true;
			err = btrfs_map_block(fs_info, BTRFS_MAP_READ, logical,
					      &stripe_len, &bioc, &io_stripe,
					      &mirror);
			btrfs_put_bioc(bioc);
			if (err) {
				btrfs_bio_end_io(bbio,
						 errno_to_blk_status(err));
				return;
			}
		}

		__bio_add_page(&bbio->bio, page, fs_info->sectorsize, pgoff);
	}

	if (bbio) {
		ASSERT(bbio->bio.bi_iter.bi_size);
		atomic_inc(&stripe->pending_io);
		btrfs_submit_bio(bbio, mirror);
	}

	if (atomic_dec_and_test(&stripe->pending_io)) {
		wake_up(&stripe->io_wait);
		INIT_WORK(&stripe->work, scrub_stripe_read_repair_worker);
		queue_work(stripe->bg->fs_info->scrub_workers, &stripe->work);
	}
}

static void scrub_submit_initial_read(struct scrub_ctx *sctx,
				      struct scrub_stripe *stripe)
{
	struct btrfs_fs_info *fs_info = sctx->fs_info;
	struct btrfs_bio *bbio;
	unsigned int nr_sectors = min(BTRFS_STRIPE_LEN, stripe->bg->start +
				      stripe->bg->length - stripe->logical) >>
				  fs_info->sectorsize_bits;
	int mirror = stripe->mirror_num;

	ASSERT(stripe->bg);
	ASSERT(stripe->mirror_num > 0);
	ASSERT(test_bit(SCRUB_STRIPE_FLAG_INITIALIZED, &stripe->state));

	if (btrfs_need_stripe_tree_update(fs_info, stripe->bg->flags)) {
		scrub_submit_extent_sector_read(sctx, stripe);
		return;
	}

	bbio = btrfs_bio_alloc(SCRUB_STRIPE_PAGES, REQ_OP_READ, fs_info,
			       scrub_read_endio, stripe);

	bbio->bio.bi_iter.bi_sector = stripe->logical >> SECTOR_SHIFT;
	/* Read the whole range inside the chunk boundary. */
	for (unsigned int cur = 0; cur < nr_sectors; cur++) {
		struct page *page = scrub_stripe_get_page(stripe, cur);
		unsigned int pgoff = scrub_stripe_get_page_offset(stripe, cur);
		int ret;

		ret = bio_add_page(&bbio->bio, page, fs_info->sectorsize, pgoff);
		/* We should have allocated enough bio vectors. */
		ASSERT(ret == fs_info->sectorsize);
	}
	atomic_inc(&stripe->pending_io);

	/*
	 * For dev-replace, either user asks to avoid the source dev, or
	 * the device is missing, we try the next mirror instead.
	 */
	if (sctx->is_dev_replace &&
	    (fs_info->dev_replace.cont_reading_from_srcdev_mode ==
	     BTRFS_DEV_REPLACE_ITEM_CONT_READING_FROM_SRCDEV_MODE_AVOID ||
	     !stripe->dev->bdev)) {
		int num_copies = btrfs_num_copies(fs_info, stripe->bg->start,
						  stripe->bg->length);

		mirror = calc_next_mirror(mirror, num_copies);
	}
	btrfs_submit_bio(bbio, mirror);
}

static bool stripe_has_metadata_error(struct scrub_stripe *stripe)
{
	int i;

	for_each_set_bit(i, &stripe->error_bitmap, stripe->nr_sectors) {
		if (stripe->sectors[i].is_metadata) {
			struct btrfs_fs_info *fs_info = stripe->bg->fs_info;

			btrfs_err(fs_info,
			"stripe %llu has unrepaired metadata sector at %llu",
				  stripe->logical,
				  stripe->logical + (i << fs_info->sectorsize_bits));
			return true;
		}
	}
	return false;
}

static void submit_initial_group_read(struct scrub_ctx *sctx,
				      unsigned int first_slot,
				      unsigned int nr_stripes)
{
	struct blk_plug plug;

	ASSERT(first_slot < SCRUB_TOTAL_STRIPES);
	ASSERT(first_slot + nr_stripes <= SCRUB_TOTAL_STRIPES);

	scrub_throttle_dev_io(sctx, sctx->stripes[0].dev,
			      btrfs_stripe_nr_to_offset(nr_stripes));
	blk_start_plug(&plug);
	for (int i = 0; i < nr_stripes; i++) {
		struct scrub_stripe *stripe = &sctx->stripes[first_slot + i];

		/* Those stripes should be initialized. */
		ASSERT(test_bit(SCRUB_STRIPE_FLAG_INITIALIZED, &stripe->state));
		scrub_submit_initial_read(sctx, stripe);
	}
	blk_finish_plug(&plug);
}

static int flush_scrub_stripes(struct scrub_ctx *sctx)
{
	struct btrfs_fs_info *fs_info = sctx->fs_info;
	struct scrub_stripe *stripe;
	const int nr_stripes = sctx->cur_stripe;
	int ret = 0;

	if (!nr_stripes)
		return 0;

	ASSERT(test_bit(SCRUB_STRIPE_FLAG_INITIALIZED, &sctx->stripes[0].state));

	/* Submit the stripes which are populated but not submitted. */
	if (nr_stripes % SCRUB_STRIPES_PER_GROUP) {
		const int first_slot = round_down(nr_stripes, SCRUB_STRIPES_PER_GROUP);

		submit_initial_group_read(sctx, first_slot, nr_stripes - first_slot);
	}

	for (int i = 0; i < nr_stripes; i++) {
		stripe = &sctx->stripes[i];

		wait_event(stripe->repair_wait,
			   test_bit(SCRUB_STRIPE_FLAG_REPAIR_DONE, &stripe->state));
	}

	/* Submit for dev-replace. */
	if (sctx->is_dev_replace) {
		/*
		 * For dev-replace, if we know there is something wrong with
		 * metadata, we should immediately abort.
		 */
		for (int i = 0; i < nr_stripes; i++) {
			if (stripe_has_metadata_error(&sctx->stripes[i])) {
				ret = -EIO;
				goto out;
			}
		}
		for (int i = 0; i < nr_stripes; i++) {
			unsigned long good;

			stripe = &sctx->stripes[i];

			ASSERT(stripe->dev == fs_info->dev_replace.srcdev);

			bitmap_andnot(&good, &stripe->extent_sector_bitmap,
				      &stripe->error_bitmap, stripe->nr_sectors);
			scrub_write_sectors(sctx, stripe, good, true);
		}
	}

	/* Wait for the above writebacks to finish. */
	for (int i = 0; i < nr_stripes; i++) {
		stripe = &sctx->stripes[i];

		wait_scrub_stripe_io(stripe);
		scrub_reset_stripe(stripe);
	}
out:
	sctx->cur_stripe = 0;
	return ret;
}

static void raid56_scrub_wait_endio(struct bio *bio)
{
	complete(bio->bi_private);
}

static int queue_scrub_stripe(struct scrub_ctx *sctx, struct btrfs_block_group *bg,
			      struct btrfs_device *dev, int mirror_num,
			      u64 logical, u32 length, u64 physical,
			      u64 *found_logical_ret)
{
	struct scrub_stripe *stripe;
	int ret;

	/*
	 * There should always be one slot left, as caller filling the last
	 * slot should flush them all.
	 */
	ASSERT(sctx->cur_stripe < SCRUB_TOTAL_STRIPES);

	/* @found_logical_ret must be specified. */
	ASSERT(found_logical_ret);

	stripe = &sctx->stripes[sctx->cur_stripe];
	scrub_reset_stripe(stripe);
	ret = scrub_find_fill_first_stripe(bg, &sctx->extent_path,
					   &sctx->csum_path, dev, physical,
					   mirror_num, logical, length, stripe);
	/* Either >0 as no more extents or <0 for error. */
	if (ret)
		return ret;
	*found_logical_ret = stripe->logical;
	sctx->cur_stripe++;

	/* We filled one group, submit it. */
	if (sctx->cur_stripe % SCRUB_STRIPES_PER_GROUP == 0) {
		const int first_slot = sctx->cur_stripe - SCRUB_STRIPES_PER_GROUP;

		submit_initial_group_read(sctx, first_slot, SCRUB_STRIPES_PER_GROUP);
	}

	/* Last slot used, flush them all. */
	if (sctx->cur_stripe == SCRUB_TOTAL_STRIPES)
		return flush_scrub_stripes(sctx);
	return 0;
}

static int scrub_raid56_parity_stripe(struct scrub_ctx *sctx,
				      struct btrfs_device *scrub_dev,
				      struct btrfs_block_group *bg,
				      struct btrfs_chunk_map *map,
				      u64 full_stripe_start)
{
	DECLARE_COMPLETION_ONSTACK(io_done);
	struct btrfs_fs_info *fs_info = sctx->fs_info;
	struct btrfs_raid_bio *rbio;
	struct btrfs_io_context *bioc = NULL;
	struct btrfs_path extent_path = { 0 };
	struct btrfs_path csum_path = { 0 };
	struct bio *bio;
	struct scrub_stripe *stripe;
	bool all_empty = true;
	const int data_stripes = nr_data_stripes(map);
	unsigned long extent_bitmap = 0;
	u64 length = btrfs_stripe_nr_to_offset(data_stripes);
	int ret;

	ASSERT(sctx->raid56_data_stripes);

	/*
	 * For data stripe search, we cannot re-use the same extent/csum paths,
	 * as the data stripe bytenr may be smaller than previous extent.  Thus
	 * we have to use our own extent/csum paths.
	 */
	extent_path.search_commit_root = 1;
	extent_path.skip_locking = 1;
	csum_path.search_commit_root = 1;
	csum_path.skip_locking = 1;

	for (int i = 0; i < data_stripes; i++) {
		int stripe_index;
		int rot;
		u64 physical;

		stripe = &sctx->raid56_data_stripes[i];
		rot = div_u64(full_stripe_start - bg->start,
			      data_stripes) >> BTRFS_STRIPE_LEN_SHIFT;
		stripe_index = (i + rot) % map->num_stripes;
		physical = map->stripes[stripe_index].physical +
			   btrfs_stripe_nr_to_offset(rot);

		scrub_reset_stripe(stripe);
		set_bit(SCRUB_STRIPE_FLAG_NO_REPORT, &stripe->state);
		ret = scrub_find_fill_first_stripe(bg, &extent_path, &csum_path,
				map->stripes[stripe_index].dev, physical, 1,
				full_stripe_start + btrfs_stripe_nr_to_offset(i),
				BTRFS_STRIPE_LEN, stripe);
		if (ret < 0)
			goto out;
		/*
		 * No extent in this data stripe, need to manually mark them
		 * initialized to make later read submission happy.
		 */
		if (ret > 0) {
			stripe->logical = full_stripe_start +
					  btrfs_stripe_nr_to_offset(i);
			stripe->dev = map->stripes[stripe_index].dev;
			stripe->mirror_num = 1;
			set_bit(SCRUB_STRIPE_FLAG_INITIALIZED, &stripe->state);
		}
	}

	/* Check if all data stripes are empty. */
	for (int i = 0; i < data_stripes; i++) {
		stripe = &sctx->raid56_data_stripes[i];
		if (!bitmap_empty(&stripe->extent_sector_bitmap, stripe->nr_sectors)) {
			all_empty = false;
			break;
		}
	}
	if (all_empty) {
		ret = 0;
		goto out;
	}

	for (int i = 0; i < data_stripes; i++) {
		stripe = &sctx->raid56_data_stripes[i];
		scrub_submit_initial_read(sctx, stripe);
	}
	for (int i = 0; i < data_stripes; i++) {
		stripe = &sctx->raid56_data_stripes[i];

		wait_event(stripe->repair_wait,
			   test_bit(SCRUB_STRIPE_FLAG_REPAIR_DONE, &stripe->state));
	}
	/* For now, no zoned support for RAID56. */
	ASSERT(!btrfs_is_zoned(sctx->fs_info));

	/*
	 * Now all data stripes are properly verified. Check if we have any
	 * unrepaired, if so abort immediately or we could further corrupt the
	 * P/Q stripes.
	 *
	 * During the loop, also populate extent_bitmap.
	 */
	for (int i = 0; i < data_stripes; i++) {
		unsigned long error;

		stripe = &sctx->raid56_data_stripes[i];

		/*
		 * We should only check the errors where there is an extent.
		 * As we may hit an empty data stripe while it's missing.
		 */
		bitmap_and(&error, &stripe->error_bitmap,
			   &stripe->extent_sector_bitmap, stripe->nr_sectors);
		if (!bitmap_empty(&error, stripe->nr_sectors)) {
			btrfs_err(fs_info,
"unrepaired sectors detected, full stripe %llu data stripe %u errors %*pbl",
				  full_stripe_start, i, stripe->nr_sectors,
				  &error);
			ret = -EIO;
			goto out;
		}
		bitmap_or(&extent_bitmap, &extent_bitmap,
			  &stripe->extent_sector_bitmap, stripe->nr_sectors);
	}

	/* Now we can check and regenerate the P/Q stripe. */
	bio = bio_alloc(NULL, 1, REQ_OP_READ, GFP_NOFS);
	bio->bi_iter.bi_sector = full_stripe_start >> SECTOR_SHIFT;
	bio->bi_private = &io_done;
	bio->bi_end_io = raid56_scrub_wait_endio;

	btrfs_bio_counter_inc_blocked(fs_info);
	ret = btrfs_map_block(fs_info, BTRFS_MAP_WRITE, full_stripe_start,
			      &length, &bioc, NULL, NULL);
	if (ret < 0) {
		btrfs_put_bioc(bioc);
		btrfs_bio_counter_dec(fs_info);
		goto out;
	}
	rbio = raid56_parity_alloc_scrub_rbio(bio, bioc, scrub_dev, &extent_bitmap,
				BTRFS_STRIPE_LEN >> fs_info->sectorsize_bits);
	btrfs_put_bioc(bioc);
	if (!rbio) {
		ret = -ENOMEM;
		btrfs_bio_counter_dec(fs_info);
		goto out;
	}
	/* Use the recovered stripes as cache to avoid read them from disk again. */
	for (int i = 0; i < data_stripes; i++) {
		stripe = &sctx->raid56_data_stripes[i];

		raid56_parity_cache_data_pages(rbio, stripe->pages,
				full_stripe_start + (i << BTRFS_STRIPE_LEN_SHIFT));
	}
	raid56_parity_submit_scrub_rbio(rbio);
	wait_for_completion_io(&io_done);
	ret = blk_status_to_errno(bio->bi_status);
	bio_put(bio);
	btrfs_bio_counter_dec(fs_info);

	btrfs_release_path(&extent_path);
	btrfs_release_path(&csum_path);
out:
	return ret;
}

/*
 * Scrub one range which can only has simple mirror based profile.
 * (Including all range in SINGLE/DUP/RAID1/RAID1C*, and each stripe in
 *  RAID0/RAID10).
 *
 * Since we may need to handle a subset of block group, we need @logical_start
 * and @logical_length parameter.
 */
static int scrub_simple_mirror(struct scrub_ctx *sctx,
			       struct btrfs_block_group *bg,
			       struct btrfs_chunk_map *map,
			       u64 logical_start, u64 logical_length,
			       struct btrfs_device *device,
			       u64 physical, int mirror_num)
{
	struct btrfs_fs_info *fs_info = sctx->fs_info;
	const u64 logical_end = logical_start + logical_length;
	u64 cur_logical = logical_start;
	int ret;

	/* The range must be inside the bg */
	ASSERT(logical_start >= bg->start && logical_end <= bg->start + bg->length);

	/* Go through each extent items inside the logical range */
	while (cur_logical < logical_end) {
		u64 found_logical = U64_MAX;
		u64 cur_physical = physical + cur_logical - logical_start;

		/* Canceled? */
		if (atomic_read(&fs_info->scrub_cancel_req) ||
		    atomic_read(&sctx->cancel_req)) {
			ret = -ECANCELED;
			break;
		}
		/* Paused? */
		if (atomic_read(&fs_info->scrub_pause_req)) {
			/* Push queued extents */
			scrub_blocked_if_needed(fs_info);
		}
		/* Block group removed? */
		spin_lock(&bg->lock);
		if (test_bit(BLOCK_GROUP_FLAG_REMOVED, &bg->runtime_flags)) {
			spin_unlock(&bg->lock);
			ret = 0;
			break;
		}
		spin_unlock(&bg->lock);

		ret = queue_scrub_stripe(sctx, bg, device, mirror_num,
					 cur_logical, logical_end - cur_logical,
					 cur_physical, &found_logical);
		if (ret > 0) {
			/* No more extent, just update the accounting */
			sctx->stat.last_physical = physical + logical_length;
			ret = 0;
			break;
		}
		if (ret < 0)
			break;

		/* queue_scrub_stripe() returned 0, @found_logical must be updated. */
		ASSERT(found_logical != U64_MAX);
		cur_logical = found_logical + BTRFS_STRIPE_LEN;

		/* Don't hold CPU for too long time */
		cond_resched();
	}
	return ret;
}

/* Calculate the full stripe length for simple stripe based profiles */
static u64 simple_stripe_full_stripe_len(const struct btrfs_chunk_map *map)
{
	ASSERT(map->type & (BTRFS_BLOCK_GROUP_RAID0 |
			    BTRFS_BLOCK_GROUP_RAID10));

	return btrfs_stripe_nr_to_offset(map->num_stripes / map->sub_stripes);
}

/* Get the logical bytenr for the stripe */
static u64 simple_stripe_get_logical(struct btrfs_chunk_map *map,
				     struct btrfs_block_group *bg,
				     int stripe_index)
{
	ASSERT(map->type & (BTRFS_BLOCK_GROUP_RAID0 |
			    BTRFS_BLOCK_GROUP_RAID10));
	ASSERT(stripe_index < map->num_stripes);

	/*
	 * (stripe_index / sub_stripes) gives how many data stripes we need to
	 * skip.
	 */
	return btrfs_stripe_nr_to_offset(stripe_index / map->sub_stripes) +
	       bg->start;
}

/* Get the mirror number for the stripe */
static int simple_stripe_mirror_num(struct btrfs_chunk_map *map, int stripe_index)
{
	ASSERT(map->type & (BTRFS_BLOCK_GROUP_RAID0 |
			    BTRFS_BLOCK_GROUP_RAID10));
	ASSERT(stripe_index < map->num_stripes);

	/* For RAID0, it's fixed to 1, for RAID10 it's 0,1,0,1... */
	return stripe_index % map->sub_stripes + 1;
}

static int scrub_simple_stripe(struct scrub_ctx *sctx,
			       struct btrfs_block_group *bg,
			       struct btrfs_chunk_map *map,
			       struct btrfs_device *device,
			       int stripe_index)
{
	const u64 logical_increment = simple_stripe_full_stripe_len(map);
	const u64 orig_logical = simple_stripe_get_logical(map, bg, stripe_index);
	const u64 orig_physical = map->stripes[stripe_index].physical;
	const int mirror_num = simple_stripe_mirror_num(map, stripe_index);
	u64 cur_logical = orig_logical;
	u64 cur_physical = orig_physical;
	int ret = 0;

	while (cur_logical < bg->start + bg->length) {
		/*
		 * Inside each stripe, RAID0 is just SINGLE, and RAID10 is
		 * just RAID1, so we can reuse scrub_simple_mirror() to scrub
		 * this stripe.
		 */
		ret = scrub_simple_mirror(sctx, bg, map, cur_logical,
					  BTRFS_STRIPE_LEN, device, cur_physical,
					  mirror_num);
		if (ret)
			return ret;
		/* Skip to next stripe which belongs to the target device */
		cur_logical += logical_increment;
		/* For physical offset, we just go to next stripe */
		cur_physical += BTRFS_STRIPE_LEN;
	}
	return ret;
}

static noinline_for_stack int scrub_stripe(struct scrub_ctx *sctx,
					   struct btrfs_block_group *bg,
					   struct btrfs_chunk_map *map,
					   struct btrfs_device *scrub_dev,
					   int stripe_index)
{
	struct btrfs_fs_info *fs_info = sctx->fs_info;
	const u64 profile = map->type & BTRFS_BLOCK_GROUP_PROFILE_MASK;
	const u64 chunk_logical = bg->start;
	int ret;
	int ret2;
	u64 physical = map->stripes[stripe_index].physical;
	const u64 dev_stripe_len = btrfs_calc_stripe_length(map);
	const u64 physical_end = physical + dev_stripe_len;
	u64 logical;
	u64 logic_end;
	/* The logical increment after finishing one stripe */
	u64 increment;
	/* Offset inside the chunk */
	u64 offset;
	u64 stripe_logical;
	int stop_loop = 0;

	/* Extent_path should be released by now. */
	ASSERT(sctx->extent_path.nodes[0] == NULL);

	scrub_blocked_if_needed(fs_info);

	if (sctx->is_dev_replace &&
	    btrfs_dev_is_sequential(sctx->wr_tgtdev, physical)) {
		mutex_lock(&sctx->wr_lock);
		sctx->write_pointer = physical;
		mutex_unlock(&sctx->wr_lock);
	}

	/* Prepare the extra data stripes used by RAID56. */
	if (profile & BTRFS_BLOCK_GROUP_RAID56_MASK) {
		ASSERT(sctx->raid56_data_stripes == NULL);

		sctx->raid56_data_stripes = kcalloc(nr_data_stripes(map),
						    sizeof(struct scrub_stripe),
						    GFP_KERNEL);
		if (!sctx->raid56_data_stripes) {
			ret = -ENOMEM;
			goto out;
		}
		for (int i = 0; i < nr_data_stripes(map); i++) {
			ret = init_scrub_stripe(fs_info,
						&sctx->raid56_data_stripes[i]);
			if (ret < 0)
				goto out;
			sctx->raid56_data_stripes[i].bg = bg;
			sctx->raid56_data_stripes[i].sctx = sctx;
		}
	}
	/*
	 * There used to be a big double loop to handle all profiles using the
	 * same routine, which grows larger and more gross over time.
	 *
	 * So here we handle each profile differently, so simpler profiles
	 * have simpler scrubbing function.
	 */
	if (!(profile & (BTRFS_BLOCK_GROUP_RAID0 | BTRFS_BLOCK_GROUP_RAID10 |
			 BTRFS_BLOCK_GROUP_RAID56_MASK))) {
		/*
		 * Above check rules out all complex profile, the remaining
		 * profiles are SINGLE|DUP|RAID1|RAID1C*, which is simple
		 * mirrored duplication without stripe.
		 *
		 * Only @physical and @mirror_num needs to calculated using
		 * @stripe_index.
		 */
		ret = scrub_simple_mirror(sctx, bg, map, bg->start, bg->length,
				scrub_dev, map->stripes[stripe_index].physical,
				stripe_index + 1);
		offset = 0;
		goto out;
	}
	if (profile & (BTRFS_BLOCK_GROUP_RAID0 | BTRFS_BLOCK_GROUP_RAID10)) {
		ret = scrub_simple_stripe(sctx, bg, map, scrub_dev, stripe_index);
		offset = btrfs_stripe_nr_to_offset(stripe_index / map->sub_stripes);
		goto out;
	}

	/* Only RAID56 goes through the old code */
	ASSERT(map->type & BTRFS_BLOCK_GROUP_RAID56_MASK);
	ret = 0;

	/* Calculate the logical end of the stripe */
	get_raid56_logic_offset(physical_end, stripe_index,
				map, &logic_end, NULL);
	logic_end += chunk_logical;

	/* Initialize @offset in case we need to go to out: label */
	get_raid56_logic_offset(physical, stripe_index, map, &offset, NULL);
	increment = btrfs_stripe_nr_to_offset(nr_data_stripes(map));

	/*
	 * Due to the rotation, for RAID56 it's better to iterate each stripe
	 * using their physical offset.
	 */
	while (physical < physical_end) {
		ret = get_raid56_logic_offset(physical, stripe_index, map,
					      &logical, &stripe_logical);
		logical += chunk_logical;
		if (ret) {
			/* it is parity strip */
			stripe_logical += chunk_logical;
			ret = scrub_raid56_parity_stripe(sctx, scrub_dev, bg,
							 map, stripe_logical);
			if (ret)
				goto out;
			goto next;
		}

		/*
		 * Now we're at a data stripe, scrub each extents in the range.
		 *
		 * At this stage, if we ignore the repair part, inside each data
		 * stripe it is no different than SINGLE profile.
		 * We can reuse scrub_simple_mirror() here, as the repair part
		 * is still based on @mirror_num.
		 */
		ret = scrub_simple_mirror(sctx, bg, map, logical, BTRFS_STRIPE_LEN,
					  scrub_dev, physical, 1);
		if (ret < 0)
			goto out;
next:
		logical += increment;
		physical += BTRFS_STRIPE_LEN;
		spin_lock(&sctx->stat_lock);
		if (stop_loop)
			sctx->stat.last_physical =
				map->stripes[stripe_index].physical + dev_stripe_len;
		else
			sctx->stat.last_physical = physical;
		spin_unlock(&sctx->stat_lock);
		if (stop_loop)
			break;
	}
out:
	ret2 = flush_scrub_stripes(sctx);
	if (!ret)
		ret = ret2;
	btrfs_release_path(&sctx->extent_path);
	btrfs_release_path(&sctx->csum_path);

	if (sctx->raid56_data_stripes) {
		for (int i = 0; i < nr_data_stripes(map); i++)
			release_scrub_stripe(&sctx->raid56_data_stripes[i]);
		kfree(sctx->raid56_data_stripes);
		sctx->raid56_data_stripes = NULL;
	}

	if (sctx->is_dev_replace && ret >= 0) {
		int ret2;

		ret2 = sync_write_pointer_for_zoned(sctx,
				chunk_logical + offset,
				map->stripes[stripe_index].physical,
				physical_end);
		if (ret2)
			ret = ret2;
	}

	return ret < 0 ? ret : 0;
}

static noinline_for_stack int scrub_chunk(struct scrub_ctx *sctx,
					  struct btrfs_block_group *bg,
					  struct btrfs_device *scrub_dev,
					  u64 dev_offset,
					  u64 dev_extent_len)
{
	struct btrfs_fs_info *fs_info = sctx->fs_info;
	struct btrfs_chunk_map *map;
	int i;
	int ret = 0;

	map = btrfs_find_chunk_map(fs_info, bg->start, bg->length);
	if (!map) {
		/*
		 * Might have been an unused block group deleted by the cleaner
		 * kthread or relocation.
		 */
		spin_lock(&bg->lock);
		if (!test_bit(BLOCK_GROUP_FLAG_REMOVED, &bg->runtime_flags))
			ret = -EINVAL;
		spin_unlock(&bg->lock);

		return ret;
	}
	if (map->start != bg->start)
		goto out;
	if (map->chunk_len < dev_extent_len)
		goto out;

	for (i = 0; i < map->num_stripes; ++i) {
		if (map->stripes[i].dev->bdev == scrub_dev->bdev &&
		    map->stripes[i].physical == dev_offset) {
			ret = scrub_stripe(sctx, bg, map, scrub_dev, i);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
			if (ret)
				goto out;
		}
	}
out:
<<<<<<< HEAD
	free_extent_map(em);
=======
	btrfs_free_chunk_map(map);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	return ret;
}

<<<<<<< HEAD
static noinline_for_stack
int scrub_enumerate_chunks(struct scrub_dev *sdev, u64 start, u64 end)
{
	struct btrfs_dev_extent *dev_extent = NULL;
	struct btrfs_path *path;
	struct btrfs_root *root = sdev->dev->dev_root;
	struct btrfs_fs_info *fs_info = root->fs_info;
	u64 length;
	u64 chunk_tree;
	u64 chunk_objectid;
	u64 chunk_offset;
	int ret;
=======
static int finish_extent_writes_for_zoned(struct btrfs_root *root,
					  struct btrfs_block_group *cache)
{
	struct btrfs_fs_info *fs_info = cache->fs_info;
	struct btrfs_trans_handle *trans;

	if (!btrfs_is_zoned(fs_info))
		return 0;

	btrfs_wait_block_group_reservations(cache);
	btrfs_wait_nocow_writers(cache);
	btrfs_wait_ordered_roots(fs_info, U64_MAX, cache->start, cache->length);

	trans = btrfs_join_transaction(root);
	if (IS_ERR(trans))
		return PTR_ERR(trans);
	return btrfs_commit_transaction(trans);
}

static noinline_for_stack
int scrub_enumerate_chunks(struct scrub_ctx *sctx,
			   struct btrfs_device *scrub_dev, u64 start, u64 end)
{
	struct btrfs_dev_extent *dev_extent = NULL;
	struct btrfs_path *path;
	struct btrfs_fs_info *fs_info = sctx->fs_info;
	struct btrfs_root *root = fs_info->dev_root;
	u64 chunk_offset;
	int ret = 0;
	int ro_set;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	int slot;
	struct extent_buffer *l;
	struct btrfs_key key;
	struct btrfs_key found_key;
<<<<<<< HEAD
	struct btrfs_block_group_cache *cache;
=======
	struct btrfs_block_group *cache;
	struct btrfs_dev_replace *dev_replace = &fs_info->dev_replace;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	path = btrfs_alloc_path();
	if (!path)
		return -ENOMEM;

<<<<<<< HEAD
	path->reada = 2;
	path->search_commit_root = 1;
	path->skip_locking = 1;

	key.objectid = sdev->dev->devid;
	key.offset = 0ull;
	key.type = BTRFS_DEV_EXTENT_KEY;


	while (1) {
=======
	path->reada = READA_FORWARD;
	path->search_commit_root = 1;
	path->skip_locking = 1;

	key.objectid = scrub_dev->devid;
	key.offset = 0ull;
	key.type = BTRFS_DEV_EXTENT_KEY;

	while (1) {
		u64 dev_extent_len;

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		ret = btrfs_search_slot(NULL, root, &key, path, 0, 0);
		if (ret < 0)
			break;
		if (ret > 0) {
			if (path->slots[0] >=
			    btrfs_header_nritems(path->nodes[0])) {
				ret = btrfs_next_leaf(root, path);
<<<<<<< HEAD
				if (ret)
					break;
=======
				if (ret < 0)
					break;
				if (ret > 0) {
					ret = 0;
					break;
				}
			} else {
				ret = 0;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
			}
		}

		l = path->nodes[0];
		slot = path->slots[0];

		btrfs_item_key_to_cpu(l, &found_key, slot);

<<<<<<< HEAD
		if (found_key.objectid != sdev->dev->devid)
			break;

		if (btrfs_key_type(&found_key) != BTRFS_DEV_EXTENT_KEY)
=======
		if (found_key.objectid != scrub_dev->devid)
			break;

		if (found_key.type != BTRFS_DEV_EXTENT_KEY)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
			break;

		if (found_key.offset >= end)
			break;

		if (found_key.offset < key.offset)
			break;

		dev_extent = btrfs_item_ptr(l, slot, struct btrfs_dev_extent);
<<<<<<< HEAD
		length = btrfs_dev_extent_length(l, dev_extent);

		if (found_key.offset + length <= start) {
			key.offset = found_key.offset + length;
			btrfs_release_path(path);
			continue;
		}

		chunk_tree = btrfs_dev_extent_chunk_tree(l, dev_extent);
		chunk_objectid = btrfs_dev_extent_chunk_objectid(l, dev_extent);
=======
		dev_extent_len = btrfs_dev_extent_length(l, dev_extent);

		if (found_key.offset + dev_extent_len <= start)
			goto skip;

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		chunk_offset = btrfs_dev_extent_chunk_offset(l, dev_extent);

		/*
		 * get a reference on the corresponding block group to prevent
		 * the chunk from going away while we scrub it
		 */
		cache = btrfs_lookup_block_group(fs_info, chunk_offset);
<<<<<<< HEAD
		if (!cache) {
			ret = -ENOENT;
			break;
		}
		ret = scrub_chunk(sdev, chunk_tree, chunk_objectid,
				  chunk_offset, length, found_key.offset);
		btrfs_put_block_group(cache);
		if (ret)
			break;

		key.offset = found_key.offset + length;
=======

		/* some chunks are removed but not committed to disk yet,
		 * continue scrubbing */
		if (!cache)
			goto skip;

		ASSERT(cache->start <= chunk_offset);
		/*
		 * We are using the commit root to search for device extents, so
		 * that means we could have found a device extent item from a
		 * block group that was deleted in the current transaction. The
		 * logical start offset of the deleted block group, stored at
		 * @chunk_offset, might be part of the logical address range of
		 * a new block group (which uses different physical extents).
		 * In this case btrfs_lookup_block_group() has returned the new
		 * block group, and its start address is less than @chunk_offset.
		 *
		 * We skip such new block groups, because it's pointless to
		 * process them, as we won't find their extents because we search
		 * for them using the commit root of the extent tree. For a device
		 * replace it's also fine to skip it, we won't miss copying them
		 * to the target device because we have the write duplication
		 * setup through the regular write path (by btrfs_map_block()),
		 * and we have committed a transaction when we started the device
		 * replace, right after setting up the device replace state.
		 */
		if (cache->start < chunk_offset) {
			btrfs_put_block_group(cache);
			goto skip;
		}

		if (sctx->is_dev_replace && btrfs_is_zoned(fs_info)) {
			if (!test_bit(BLOCK_GROUP_FLAG_TO_COPY, &cache->runtime_flags)) {
				btrfs_put_block_group(cache);
				goto skip;
			}
		}

		/*
		 * Make sure that while we are scrubbing the corresponding block
		 * group doesn't get its logical address and its device extents
		 * reused for another block group, which can possibly be of a
		 * different type and different profile. We do this to prevent
		 * false error detections and crashes due to bogus attempts to
		 * repair extents.
		 */
		spin_lock(&cache->lock);
		if (test_bit(BLOCK_GROUP_FLAG_REMOVED, &cache->runtime_flags)) {
			spin_unlock(&cache->lock);
			btrfs_put_block_group(cache);
			goto skip;
		}
		btrfs_freeze_block_group(cache);
		spin_unlock(&cache->lock);

		/*
		 * we need call btrfs_inc_block_group_ro() with scrubs_paused,
		 * to avoid deadlock caused by:
		 * btrfs_inc_block_group_ro()
		 * -> btrfs_wait_for_commit()
		 * -> btrfs_commit_transaction()
		 * -> btrfs_scrub_pause()
		 */
		scrub_pause_on(fs_info);

		/*
		 * Don't do chunk preallocation for scrub.
		 *
		 * This is especially important for SYSTEM bgs, or we can hit
		 * -EFBIG from btrfs_finish_chunk_alloc() like:
		 * 1. The only SYSTEM bg is marked RO.
		 *    Since SYSTEM bg is small, that's pretty common.
		 * 2. New SYSTEM bg will be allocated
		 *    Due to regular version will allocate new chunk.
		 * 3. New SYSTEM bg is empty and will get cleaned up
		 *    Before cleanup really happens, it's marked RO again.
		 * 4. Empty SYSTEM bg get scrubbed
		 *    We go back to 2.
		 *
		 * This can easily boost the amount of SYSTEM chunks if cleaner
		 * thread can't be triggered fast enough, and use up all space
		 * of btrfs_super_block::sys_chunk_array
		 *
		 * While for dev replace, we need to try our best to mark block
		 * group RO, to prevent race between:
		 * - Write duplication
		 *   Contains latest data
		 * - Scrub copy
		 *   Contains data from commit tree
		 *
		 * If target block group is not marked RO, nocow writes can
		 * be overwritten by scrub copy, causing data corruption.
		 * So for dev-replace, it's not allowed to continue if a block
		 * group is not RO.
		 */
		ret = btrfs_inc_block_group_ro(cache, sctx->is_dev_replace);
		if (!ret && sctx->is_dev_replace) {
			ret = finish_extent_writes_for_zoned(root, cache);
			if (ret) {
				btrfs_dec_block_group_ro(cache);
				scrub_pause_off(fs_info);
				btrfs_put_block_group(cache);
				break;
			}
		}

		if (ret == 0) {
			ro_set = 1;
		} else if (ret == -ENOSPC && !sctx->is_dev_replace &&
			   !(cache->flags & BTRFS_BLOCK_GROUP_RAID56_MASK)) {
			/*
			 * btrfs_inc_block_group_ro return -ENOSPC when it
			 * failed in creating new chunk for metadata.
			 * It is not a problem for scrub, because
			 * metadata are always cowed, and our scrub paused
			 * commit_transactions.
			 *
			 * For RAID56 chunks, we have to mark them read-only
			 * for scrub, as later we would use our own cache
			 * out of RAID56 realm.
			 * Thus we want the RAID56 bg to be marked RO to
			 * prevent RMW from screwing up out cache.
			 */
			ro_set = 0;
		} else if (ret == -ETXTBSY) {
			btrfs_warn(fs_info,
		   "skipping scrub of block group %llu due to active swapfile",
				   cache->start);
			scrub_pause_off(fs_info);
			ret = 0;
			goto skip_unfreeze;
		} else {
			btrfs_warn(fs_info,
				   "failed setting block group ro: %d", ret);
			btrfs_unfreeze_block_group(cache);
			btrfs_put_block_group(cache);
			scrub_pause_off(fs_info);
			break;
		}

		/*
		 * Now the target block is marked RO, wait for nocow writes to
		 * finish before dev-replace.
		 * COW is fine, as COW never overwrites extents in commit tree.
		 */
		if (sctx->is_dev_replace) {
			btrfs_wait_nocow_writers(cache);
			btrfs_wait_ordered_roots(fs_info, U64_MAX, cache->start,
					cache->length);
		}

		scrub_pause_off(fs_info);
		down_write(&dev_replace->rwsem);
		dev_replace->cursor_right = found_key.offset + dev_extent_len;
		dev_replace->cursor_left = found_key.offset;
		dev_replace->item_needs_writeback = 1;
		up_write(&dev_replace->rwsem);

		ret = scrub_chunk(sctx, cache, scrub_dev, found_key.offset,
				  dev_extent_len);
		if (sctx->is_dev_replace &&
		    !btrfs_finish_block_group_to_copy(dev_replace->srcdev,
						      cache, found_key.offset))
			ro_set = 0;

		down_write(&dev_replace->rwsem);
		dev_replace->cursor_left = dev_replace->cursor_right;
		dev_replace->item_needs_writeback = 1;
		up_write(&dev_replace->rwsem);

		if (ro_set)
			btrfs_dec_block_group_ro(cache);

		/*
		 * We might have prevented the cleaner kthread from deleting
		 * this block group if it was already unused because we raced
		 * and set it to RO mode first. So add it back to the unused
		 * list, otherwise it might not ever be deleted unless a manual
		 * balance is triggered or it becomes used and unused again.
		 */
		spin_lock(&cache->lock);
		if (!test_bit(BLOCK_GROUP_FLAG_REMOVED, &cache->runtime_flags) &&
		    !cache->ro && cache->reserved == 0 && cache->used == 0) {
			spin_unlock(&cache->lock);
			if (btrfs_test_opt(fs_info, DISCARD_ASYNC))
				btrfs_discard_queue_work(&fs_info->discard_ctl,
							 cache);
			else
				btrfs_mark_bg_unused(cache);
		} else {
			spin_unlock(&cache->lock);
		}
skip_unfreeze:
		btrfs_unfreeze_block_group(cache);
		btrfs_put_block_group(cache);
		if (ret)
			break;
		if (sctx->is_dev_replace &&
		    atomic64_read(&dev_replace->num_write_errors) > 0) {
			ret = -EIO;
			break;
		}
		if (sctx->stat.malloc_errors > 0) {
			ret = -ENOMEM;
			break;
		}
skip:
		key.offset = found_key.offset + dev_extent_len;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		btrfs_release_path(path);
	}

	btrfs_free_path(path);

<<<<<<< HEAD
	/*
	 * ret can still be 1 from search_slot or next_leaf,
	 * that's not an error
	 */
	return ret < 0 ? ret : 0;
}

static noinline_for_stack int scrub_supers(struct scrub_dev *sdev)
=======
	return ret;
}

static int scrub_one_super(struct scrub_ctx *sctx, struct btrfs_device *dev,
			   struct page *page, u64 physical, u64 generation)
{
	struct btrfs_fs_info *fs_info = sctx->fs_info;
	struct bio_vec bvec;
	struct bio bio;
	struct btrfs_super_block *sb = page_address(page);
	int ret;

	bio_init(&bio, dev->bdev, &bvec, 1, REQ_OP_READ);
	bio.bi_iter.bi_sector = physical >> SECTOR_SHIFT;
	__bio_add_page(&bio, page, BTRFS_SUPER_INFO_SIZE, 0);
	ret = submit_bio_wait(&bio);
	bio_uninit(&bio);

	if (ret < 0)
		return ret;
	ret = btrfs_check_super_csum(fs_info, sb);
	if (ret != 0) {
		btrfs_err_rl(fs_info,
			"super block at physical %llu devid %llu has bad csum",
			physical, dev->devid);
		return -EIO;
	}
	if (btrfs_super_generation(sb) != generation) {
		btrfs_err_rl(fs_info,
"super block at physical %llu devid %llu has bad generation %llu expect %llu",
			     physical, dev->devid,
			     btrfs_super_generation(sb), generation);
		return -EUCLEAN;
	}

	return btrfs_validate_super(fs_info, sb, -1);
}

static noinline_for_stack int scrub_supers(struct scrub_ctx *sctx,
					   struct btrfs_device *scrub_dev)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
{
	int	i;
	u64	bytenr;
	u64	gen;
<<<<<<< HEAD
	int	ret;
	struct btrfs_device *device = sdev->dev;
	struct btrfs_root *root = device->dev_root;

	if (root->fs_info->fs_state & BTRFS_SUPER_FLAG_ERROR)
		return -EIO;

	gen = root->fs_info->last_trans_committed;

	for (i = 0; i < BTRFS_SUPER_MIRROR_MAX; i++) {
		bytenr = btrfs_sb_offset(i);
		if (bytenr + BTRFS_SUPER_INFO_SIZE > device->total_bytes)
			break;

		ret = scrub_pages(sdev, bytenr, BTRFS_SUPER_INFO_SIZE, bytenr,
				     BTRFS_EXTENT_FLAG_SUPER, gen, i, NULL, 1);
		if (ret)
			return ret;
	}
	wait_event(sdev->list_wait, atomic_read(&sdev->in_flight) == 0);

	return 0;
}

/*
 * get a reference count on fs_info->scrub_workers. start worker if necessary
 */
static noinline_for_stack int scrub_workers_get(struct btrfs_root *root)
{
	struct btrfs_fs_info *fs_info = root->fs_info;
	int ret = 0;

	mutex_lock(&fs_info->scrub_lock);
	if (fs_info->scrub_workers_refcnt == 0) {
		btrfs_init_workers(&fs_info->scrub_workers, "scrub",
			   fs_info->thread_pool_size, &fs_info->generic_worker);
		fs_info->scrub_workers.idle_thresh = 4;
		ret = btrfs_start_workers(&fs_info->scrub_workers);
		if (ret)
			goto out;
	}
	++fs_info->scrub_workers_refcnt;
out:
	mutex_unlock(&fs_info->scrub_lock);

	return ret;
}

static noinline_for_stack void scrub_workers_put(struct btrfs_root *root)
{
	struct btrfs_fs_info *fs_info = root->fs_info;

	mutex_lock(&fs_info->scrub_lock);
	if (--fs_info->scrub_workers_refcnt == 0)
		btrfs_stop_workers(&fs_info->scrub_workers);
	WARN_ON(fs_info->scrub_workers_refcnt < 0);
	mutex_unlock(&fs_info->scrub_lock);
}


int btrfs_scrub_dev(struct btrfs_root *root, u64 devid, u64 start, u64 end,
		    struct btrfs_scrub_progress *progress, int readonly)
{
	struct scrub_dev *sdev;
	struct btrfs_fs_info *fs_info = root->fs_info;
	int ret;
	struct btrfs_device *dev;

	if (btrfs_fs_closing(root->fs_info))
		return -EINVAL;

	/*
	 * check some assumptions
	 */
	if (root->nodesize != root->leafsize) {
		printk(KERN_ERR
		       "btrfs_scrub: size assumption nodesize == leafsize (%d == %d) fails\n",
		       root->nodesize, root->leafsize);
		return -EINVAL;
	}

	if (root->nodesize > BTRFS_STRIPE_LEN) {
		/*
		 * in this case scrub is unable to calculate the checksum
		 * the way scrub is implemented. Do not handle this
		 * situation at all because it won't ever happen.
		 */
		printk(KERN_ERR
		       "btrfs_scrub: size assumption nodesize <= BTRFS_STRIPE_LEN (%d <= %d) fails\n",
		       root->nodesize, BTRFS_STRIPE_LEN);
		return -EINVAL;
	}

	if (root->sectorsize != PAGE_SIZE) {
		/* not supported for data w/o checksums */
		printk(KERN_ERR
		       "btrfs_scrub: size assumption sectorsize != PAGE_SIZE (%d != %lld) fails\n",
		       root->sectorsize, (unsigned long long)PAGE_SIZE);
		return -EINVAL;
	}

	ret = scrub_workers_get(root);
	if (ret)
		return ret;

	mutex_lock(&root->fs_info->fs_devices->device_list_mutex);
	dev = btrfs_find_device(root, devid, NULL, NULL);
	if (!dev || dev->missing) {
		mutex_unlock(&root->fs_info->fs_devices->device_list_mutex);
		scrub_workers_put(root);
		return -ENODEV;
	}
	mutex_lock(&fs_info->scrub_lock);

	if (!dev->in_fs_metadata) {
		mutex_unlock(&fs_info->scrub_lock);
		mutex_unlock(&root->fs_info->fs_devices->device_list_mutex);
		scrub_workers_put(root);
		return -ENODEV;
	}

	if (dev->scrub_device) {
		mutex_unlock(&fs_info->scrub_lock);
		mutex_unlock(&root->fs_info->fs_devices->device_list_mutex);
		scrub_workers_put(root);
		return -EINPROGRESS;
	}
	sdev = scrub_setup_dev(dev);
	if (IS_ERR(sdev)) {
		mutex_unlock(&fs_info->scrub_lock);
		mutex_unlock(&root->fs_info->fs_devices->device_list_mutex);
		scrub_workers_put(root);
		return PTR_ERR(sdev);
	}
	sdev->readonly = readonly;
	dev->scrub_device = sdev;

	atomic_inc(&fs_info->scrubs_running);
	mutex_unlock(&fs_info->scrub_lock);
	mutex_unlock(&root->fs_info->fs_devices->device_list_mutex);

	down_read(&fs_info->scrub_super_lock);
	ret = scrub_supers(sdev);
	up_read(&fs_info->scrub_super_lock);

	if (!ret)
		ret = scrub_enumerate_chunks(sdev, start, end);

	wait_event(sdev->list_wait, atomic_read(&sdev->in_flight) == 0);
	atomic_dec(&fs_info->scrubs_running);
	wake_up(&fs_info->scrub_pause_wait);

	wait_event(sdev->list_wait, atomic_read(&sdev->fixup_cnt) == 0);

	if (progress)
		memcpy(progress, &sdev->stat, sizeof(*progress));

	mutex_lock(&fs_info->scrub_lock);
	dev->scrub_device = NULL;
	mutex_unlock(&fs_info->scrub_lock);

	scrub_free_dev(sdev);
	scrub_workers_put(root);
=======
	int ret = 0;
	struct page *page;
	struct btrfs_fs_info *fs_info = sctx->fs_info;

	if (BTRFS_FS_ERROR(fs_info))
		return -EROFS;

	page = alloc_page(GFP_KERNEL);
	if (!page) {
		spin_lock(&sctx->stat_lock);
		sctx->stat.malloc_errors++;
		spin_unlock(&sctx->stat_lock);
		return -ENOMEM;
	}

	/* Seed devices of a new filesystem has their own generation. */
	if (scrub_dev->fs_devices != fs_info->fs_devices)
		gen = scrub_dev->generation;
	else
		gen = btrfs_get_last_trans_committed(fs_info);

	for (i = 0; i < BTRFS_SUPER_MIRROR_MAX; i++) {
		ret = btrfs_sb_log_location(scrub_dev, i, 0, &bytenr);
		if (ret == -ENOENT)
			break;

		if (ret) {
			spin_lock(&sctx->stat_lock);
			sctx->stat.super_errors++;
			spin_unlock(&sctx->stat_lock);
			continue;
		}

		if (bytenr + BTRFS_SUPER_INFO_SIZE >
		    scrub_dev->commit_total_bytes)
			break;
		if (!btrfs_check_super_location(scrub_dev, bytenr))
			continue;

		ret = scrub_one_super(sctx, scrub_dev, page, bytenr, gen);
		if (ret) {
			spin_lock(&sctx->stat_lock);
			sctx->stat.super_errors++;
			spin_unlock(&sctx->stat_lock);
		}
	}
	__free_page(page);
	return 0;
}

static void scrub_workers_put(struct btrfs_fs_info *fs_info)
{
	if (refcount_dec_and_mutex_lock(&fs_info->scrub_workers_refcnt,
					&fs_info->scrub_lock)) {
		struct workqueue_struct *scrub_workers = fs_info->scrub_workers;

		fs_info->scrub_workers = NULL;
		mutex_unlock(&fs_info->scrub_lock);

		if (scrub_workers)
			destroy_workqueue(scrub_workers);
	}
}

/*
 * get a reference count on fs_info->scrub_workers. start worker if necessary
 */
static noinline_for_stack int scrub_workers_get(struct btrfs_fs_info *fs_info)
{
	struct workqueue_struct *scrub_workers = NULL;
	unsigned int flags = WQ_FREEZABLE | WQ_UNBOUND;
	int max_active = fs_info->thread_pool_size;
	int ret = -ENOMEM;

	if (refcount_inc_not_zero(&fs_info->scrub_workers_refcnt))
		return 0;

	scrub_workers = alloc_workqueue("btrfs-scrub", flags, max_active);
	if (!scrub_workers)
		return -ENOMEM;

	mutex_lock(&fs_info->scrub_lock);
	if (refcount_read(&fs_info->scrub_workers_refcnt) == 0) {
		ASSERT(fs_info->scrub_workers == NULL);
		fs_info->scrub_workers = scrub_workers;
		refcount_set(&fs_info->scrub_workers_refcnt, 1);
		mutex_unlock(&fs_info->scrub_lock);
		return 0;
	}
	/* Other thread raced in and created the workers for us */
	refcount_inc(&fs_info->scrub_workers_refcnt);
	mutex_unlock(&fs_info->scrub_lock);

	ret = 0;

	destroy_workqueue(scrub_workers);
	return ret;
}

int btrfs_scrub_dev(struct btrfs_fs_info *fs_info, u64 devid, u64 start,
		    u64 end, struct btrfs_scrub_progress *progress,
		    int readonly, int is_dev_replace)
{
	struct btrfs_dev_lookup_args args = { .devid = devid };
	struct scrub_ctx *sctx;
	int ret;
	struct btrfs_device *dev;
	unsigned int nofs_flag;
	bool need_commit = false;

	if (btrfs_fs_closing(fs_info))
		return -EAGAIN;

	/* At mount time we have ensured nodesize is in the range of [4K, 64K]. */
	ASSERT(fs_info->nodesize <= BTRFS_STRIPE_LEN);

	/*
	 * SCRUB_MAX_SECTORS_PER_BLOCK is calculated using the largest possible
	 * value (max nodesize / min sectorsize), thus nodesize should always
	 * be fine.
	 */
	ASSERT(fs_info->nodesize <=
	       SCRUB_MAX_SECTORS_PER_BLOCK << fs_info->sectorsize_bits);

	/* Allocate outside of device_list_mutex */
	sctx = scrub_setup_ctx(fs_info, is_dev_replace);
	if (IS_ERR(sctx))
		return PTR_ERR(sctx);

	ret = scrub_workers_get(fs_info);
	if (ret)
		goto out_free_ctx;

	mutex_lock(&fs_info->fs_devices->device_list_mutex);
	dev = btrfs_find_device(fs_info->fs_devices, &args);
	if (!dev || (test_bit(BTRFS_DEV_STATE_MISSING, &dev->dev_state) &&
		     !is_dev_replace)) {
		mutex_unlock(&fs_info->fs_devices->device_list_mutex);
		ret = -ENODEV;
		goto out;
	}

	if (!is_dev_replace && !readonly &&
	    !test_bit(BTRFS_DEV_STATE_WRITEABLE, &dev->dev_state)) {
		mutex_unlock(&fs_info->fs_devices->device_list_mutex);
		btrfs_err_in_rcu(fs_info,
			"scrub on devid %llu: filesystem on %s is not writable",
				 devid, btrfs_dev_name(dev));
		ret = -EROFS;
		goto out;
	}

	mutex_lock(&fs_info->scrub_lock);
	if (!test_bit(BTRFS_DEV_STATE_IN_FS_METADATA, &dev->dev_state) ||
	    test_bit(BTRFS_DEV_STATE_REPLACE_TGT, &dev->dev_state)) {
		mutex_unlock(&fs_info->scrub_lock);
		mutex_unlock(&fs_info->fs_devices->device_list_mutex);
		ret = -EIO;
		goto out;
	}

	down_read(&fs_info->dev_replace.rwsem);
	if (dev->scrub_ctx ||
	    (!is_dev_replace &&
	     btrfs_dev_replace_is_ongoing(&fs_info->dev_replace))) {
		up_read(&fs_info->dev_replace.rwsem);
		mutex_unlock(&fs_info->scrub_lock);
		mutex_unlock(&fs_info->fs_devices->device_list_mutex);
		ret = -EINPROGRESS;
		goto out;
	}
	up_read(&fs_info->dev_replace.rwsem);

	sctx->readonly = readonly;
	dev->scrub_ctx = sctx;
	mutex_unlock(&fs_info->fs_devices->device_list_mutex);

	/*
	 * checking @scrub_pause_req here, we can avoid
	 * race between committing transaction and scrubbing.
	 */
	__scrub_blocked_if_needed(fs_info);
	atomic_inc(&fs_info->scrubs_running);
	mutex_unlock(&fs_info->scrub_lock);

	/*
	 * In order to avoid deadlock with reclaim when there is a transaction
	 * trying to pause scrub, make sure we use GFP_NOFS for all the
	 * allocations done at btrfs_scrub_sectors() and scrub_sectors_for_parity()
	 * invoked by our callees. The pausing request is done when the
	 * transaction commit starts, and it blocks the transaction until scrub
	 * is paused (done at specific points at scrub_stripe() or right above
	 * before incrementing fs_info->scrubs_running).
	 */
	nofs_flag = memalloc_nofs_save();
	if (!is_dev_replace) {
		u64 old_super_errors;

		spin_lock(&sctx->stat_lock);
		old_super_errors = sctx->stat.super_errors;
		spin_unlock(&sctx->stat_lock);

		btrfs_info(fs_info, "scrub: started on devid %llu", devid);
		/*
		 * by holding device list mutex, we can
		 * kick off writing super in log tree sync.
		 */
		mutex_lock(&fs_info->fs_devices->device_list_mutex);
		ret = scrub_supers(sctx, dev);
		mutex_unlock(&fs_info->fs_devices->device_list_mutex);

		spin_lock(&sctx->stat_lock);
		/*
		 * Super block errors found, but we can not commit transaction
		 * at current context, since btrfs_commit_transaction() needs
		 * to pause the current running scrub (hold by ourselves).
		 */
		if (sctx->stat.super_errors > old_super_errors && !sctx->readonly)
			need_commit = true;
		spin_unlock(&sctx->stat_lock);
	}

	if (!ret)
		ret = scrub_enumerate_chunks(sctx, dev, start, end);
	memalloc_nofs_restore(nofs_flag);

	atomic_dec(&fs_info->scrubs_running);
	wake_up(&fs_info->scrub_pause_wait);

	if (progress)
		memcpy(progress, &sctx->stat, sizeof(*progress));

	if (!is_dev_replace)
		btrfs_info(fs_info, "scrub: %s on devid %llu with status: %d",
			ret ? "not finished" : "finished", devid, ret);

	mutex_lock(&fs_info->scrub_lock);
	dev->scrub_ctx = NULL;
	mutex_unlock(&fs_info->scrub_lock);

	scrub_workers_put(fs_info);
	scrub_put_ctx(sctx);

	/*
	 * We found some super block errors before, now try to force a
	 * transaction commit, as scrub has finished.
	 */
	if (need_commit) {
		struct btrfs_trans_handle *trans;

		trans = btrfs_start_transaction(fs_info->tree_root, 0);
		if (IS_ERR(trans)) {
			ret = PTR_ERR(trans);
			btrfs_err(fs_info,
	"scrub: failed to start transaction to fix super block errors: %d", ret);
			return ret;
		}
		ret = btrfs_commit_transaction(trans);
		if (ret < 0)
			btrfs_err(fs_info,
	"scrub: failed to commit transaction to fix super block errors: %d", ret);
	}
	return ret;
out:
	scrub_workers_put(fs_info);
out_free_ctx:
	scrub_free_ctx(sctx);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	return ret;
}

<<<<<<< HEAD
void btrfs_scrub_pause(struct btrfs_root *root)
{
	struct btrfs_fs_info *fs_info = root->fs_info;

=======
void btrfs_scrub_pause(struct btrfs_fs_info *fs_info)
{
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	mutex_lock(&fs_info->scrub_lock);
	atomic_inc(&fs_info->scrub_pause_req);
	while (atomic_read(&fs_info->scrubs_paused) !=
	       atomic_read(&fs_info->scrubs_running)) {
		mutex_unlock(&fs_info->scrub_lock);
		wait_event(fs_info->scrub_pause_wait,
			   atomic_read(&fs_info->scrubs_paused) ==
			   atomic_read(&fs_info->scrubs_running));
		mutex_lock(&fs_info->scrub_lock);
	}
	mutex_unlock(&fs_info->scrub_lock);
}

<<<<<<< HEAD
void btrfs_scrub_continue(struct btrfs_root *root)
{
	struct btrfs_fs_info *fs_info = root->fs_info;

=======
void btrfs_scrub_continue(struct btrfs_fs_info *fs_info)
{
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	atomic_dec(&fs_info->scrub_pause_req);
	wake_up(&fs_info->scrub_pause_wait);
}

<<<<<<< HEAD
void btrfs_scrub_pause_super(struct btrfs_root *root)
{
	down_write(&root->fs_info->scrub_super_lock);
}

void btrfs_scrub_continue_super(struct btrfs_root *root)
{
	up_write(&root->fs_info->scrub_super_lock);
}

int __btrfs_scrub_cancel(struct btrfs_fs_info *fs_info)
{

=======
int btrfs_scrub_cancel(struct btrfs_fs_info *fs_info)
{
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	mutex_lock(&fs_info->scrub_lock);
	if (!atomic_read(&fs_info->scrubs_running)) {
		mutex_unlock(&fs_info->scrub_lock);
		return -ENOTCONN;
	}

	atomic_inc(&fs_info->scrub_cancel_req);
	while (atomic_read(&fs_info->scrubs_running)) {
		mutex_unlock(&fs_info->scrub_lock);
		wait_event(fs_info->scrub_pause_wait,
			   atomic_read(&fs_info->scrubs_running) == 0);
		mutex_lock(&fs_info->scrub_lock);
	}
	atomic_dec(&fs_info->scrub_cancel_req);
	mutex_unlock(&fs_info->scrub_lock);

	return 0;
}

<<<<<<< HEAD
int btrfs_scrub_cancel(struct btrfs_root *root)
{
	return __btrfs_scrub_cancel(root->fs_info);
}

int btrfs_scrub_cancel_dev(struct btrfs_root *root, struct btrfs_device *dev)
{
	struct btrfs_fs_info *fs_info = root->fs_info;
	struct scrub_dev *sdev;

	mutex_lock(&fs_info->scrub_lock);
	sdev = dev->scrub_device;
	if (!sdev) {
		mutex_unlock(&fs_info->scrub_lock);
		return -ENOTCONN;
	}
	atomic_inc(&sdev->cancel_req);
	while (dev->scrub_device) {
		mutex_unlock(&fs_info->scrub_lock);
		wait_event(fs_info->scrub_pause_wait,
			   dev->scrub_device == NULL);
=======
int btrfs_scrub_cancel_dev(struct btrfs_device *dev)
{
	struct btrfs_fs_info *fs_info = dev->fs_info;
	struct scrub_ctx *sctx;

	mutex_lock(&fs_info->scrub_lock);
	sctx = dev->scrub_ctx;
	if (!sctx) {
		mutex_unlock(&fs_info->scrub_lock);
		return -ENOTCONN;
	}
	atomic_inc(&sctx->cancel_req);
	while (dev->scrub_ctx) {
		mutex_unlock(&fs_info->scrub_lock);
		wait_event(fs_info->scrub_pause_wait,
			   dev->scrub_ctx == NULL);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		mutex_lock(&fs_info->scrub_lock);
	}
	mutex_unlock(&fs_info->scrub_lock);

	return 0;
}

<<<<<<< HEAD
int btrfs_scrub_cancel_devid(struct btrfs_root *root, u64 devid)
{
	struct btrfs_fs_info *fs_info = root->fs_info;
	struct btrfs_device *dev;
	int ret;

	/*
	 * we have to hold the device_list_mutex here so the device
	 * does not go away in cancel_dev. FIXME: find a better solution
	 */
	mutex_lock(&fs_info->fs_devices->device_list_mutex);
	dev = btrfs_find_device(root, devid, NULL, NULL);
	if (!dev) {
		mutex_unlock(&fs_info->fs_devices->device_list_mutex);
		return -ENODEV;
	}
	ret = btrfs_scrub_cancel_dev(root, dev);
	mutex_unlock(&fs_info->fs_devices->device_list_mutex);

	return ret;
}

int btrfs_scrub_progress(struct btrfs_root *root, u64 devid,
			 struct btrfs_scrub_progress *progress)
{
	struct btrfs_device *dev;
	struct scrub_dev *sdev = NULL;

	mutex_lock(&root->fs_info->fs_devices->device_list_mutex);
	dev = btrfs_find_device(root, devid, NULL, NULL);
	if (dev)
		sdev = dev->scrub_device;
	if (sdev)
		memcpy(progress, &sdev->stat, sizeof(*progress));
	mutex_unlock(&root->fs_info->fs_devices->device_list_mutex);

	return dev ? (sdev ? 0 : -ENOTCONN) : -ENODEV;
=======
int btrfs_scrub_progress(struct btrfs_fs_info *fs_info, u64 devid,
			 struct btrfs_scrub_progress *progress)
{
	struct btrfs_dev_lookup_args args = { .devid = devid };
	struct btrfs_device *dev;
	struct scrub_ctx *sctx = NULL;

	mutex_lock(&fs_info->fs_devices->device_list_mutex);
	dev = btrfs_find_device(fs_info->fs_devices, &args);
	if (dev)
		sctx = dev->scrub_ctx;
	if (sctx)
		memcpy(progress, &sctx->stat, sizeof(*progress));
	mutex_unlock(&fs_info->fs_devices->device_list_mutex);

	return dev ? (sctx ? 0 : -ENOTCONN) : -ENODEV;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}
