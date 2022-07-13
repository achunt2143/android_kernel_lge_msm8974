<<<<<<< HEAD
=======
// SPDX-License-Identifier: GPL-2.0
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
/*
 * Functions related to setting various queue properties from drivers
 */
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/init.h>
#include <linux/bio.h>
#include <linux/blkdev.h>
<<<<<<< HEAD
#include <linux/bootmem.h>	/* for max_pfn/max_low_pfn */
=======
#include <linux/pagemap.h>
#include <linux/backing-dev-defs.h>
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#include <linux/gcd.h>
#include <linux/lcm.h>
#include <linux/jiffies.h>
#include <linux/gfp.h>
<<<<<<< HEAD

#include "blk.h"

unsigned long blk_max_low_pfn;
EXPORT_SYMBOL(blk_max_low_pfn);

unsigned long blk_max_pfn;

/**
 * blk_queue_prep_rq - set a prepare_request function for queue
 * @q:		queue
 * @pfn:	prepare_request function
 *
 * It's possible for a queue to register a prepare_request callback which
 * is invoked before the request is handed to the request_fn. The goal of
 * the function is to prepare a request for I/O, it can be used to build a
 * cdb from the request data for instance.
 *
 */
void blk_queue_prep_rq(struct request_queue *q, prep_rq_fn *pfn)
{
	q->prep_rq_fn = pfn;
}
EXPORT_SYMBOL(blk_queue_prep_rq);

/**
 * blk_queue_unprep_rq - set an unprepare_request function for queue
 * @q:		queue
 * @ufn:	unprepare_request function
 *
 * It's possible for a queue to register an unprepare_request callback
 * which is invoked before the request is finally completed. The goal
 * of the function is to deallocate any data that was allocated in the
 * prepare_request callback.
 *
 */
void blk_queue_unprep_rq(struct request_queue *q, unprep_rq_fn *ufn)
{
	q->unprep_rq_fn = ufn;
}
EXPORT_SYMBOL(blk_queue_unprep_rq);

/**
 * blk_queue_merge_bvec - set a merge_bvec function for queue
 * @q:		queue
 * @mbfn:	merge_bvec_fn
 *
 * Usually queues have static limitations on the max sectors or segments that
 * we can put in a request. Stacking drivers may have some settings that
 * are dynamic, and thus we have to query the queue whether it is ok to
 * add a new bio_vec to a bio at a given offset or not. If the block device
 * has such limitations, it needs to register a merge_bvec_fn to control
 * the size of bio's sent to it. Note that a block device *must* allow a
 * single page to be added to an empty bio. The block device driver may want
 * to use the bio_split() function to deal with these bio's. By default
 * no merge_bvec_fn is defined for a queue, and only the fixed limits are
 * honored.
 */
void blk_queue_merge_bvec(struct request_queue *q, merge_bvec_fn *mbfn)
{
	q->merge_bvec_fn = mbfn;
}
EXPORT_SYMBOL(blk_queue_merge_bvec);

void blk_queue_softirq_done(struct request_queue *q, softirq_done_fn *fn)
{
	q->softirq_done_fn = fn;
}
EXPORT_SYMBOL(blk_queue_softirq_done);
=======
#include <linux/dma-mapping.h>

#include "blk.h"
#include "blk-rq-qos.h"
#include "blk-wbt.h"
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

void blk_queue_rq_timeout(struct request_queue *q, unsigned int timeout)
{
	q->rq_timeout = timeout;
}
EXPORT_SYMBOL_GPL(blk_queue_rq_timeout);

<<<<<<< HEAD
void blk_queue_rq_timed_out(struct request_queue *q, rq_timed_out_fn *fn)
{
	q->rq_timed_out_fn = fn;
}
EXPORT_SYMBOL_GPL(blk_queue_rq_timed_out);

void blk_queue_lld_busy(struct request_queue *q, lld_busy_fn *fn)
{
	q->lld_busy_fn = fn;
}
EXPORT_SYMBOL_GPL(blk_queue_lld_busy);

/**
 * blk_urgent_request() - Set an urgent_request handler function for queue
 * @q:		queue
 * @fn:		handler for urgent requests
 *
 */
void blk_urgent_request(struct request_queue *q, request_fn_proc *fn)
{
	q->urgent_request_fn = fn;
}
EXPORT_SYMBOL(blk_urgent_request);

/**
 * blk_set_default_limits - reset limits to default values
 * @lim:  the queue_limits structure to reset
 *
 * Description:
 *   Returns a queue_limit struct to its default state.
 */
void blk_set_default_limits(struct queue_limits *lim)
{
	lim->max_segments = BLK_MAX_SEGMENTS;
	lim->max_integrity_segments = 0;
	lim->seg_boundary_mask = BLK_SEG_BOUNDARY_MASK;
	lim->max_segment_size = BLK_MAX_SEGMENT_SIZE;
	lim->max_sectors = lim->max_hw_sectors = BLK_SAFE_MAX_SECTORS;
	lim->max_discard_sectors = 0;
	lim->discard_granularity = 0;
	lim->discard_alignment = 0;
	lim->discard_misaligned = 0;
	lim->discard_zeroes_data = 0;
	lim->logical_block_size = lim->physical_block_size = lim->io_min = 512;
	lim->bounce_pfn = (unsigned long)(BLK_BOUNCE_ANY >> PAGE_SHIFT);
	lim->alignment_offset = 0;
	lim->io_opt = 0;
	lim->misaligned = 0;
	lim->cluster = 1;
}
EXPORT_SYMBOL(blk_set_default_limits);

=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
/**
 * blk_set_stacking_limits - set default limits for stacking devices
 * @lim:  the queue_limits structure to reset
 *
<<<<<<< HEAD
 * Description:
 *   Returns a queue_limit struct to its default state. Should be used
 *   by stacking drivers like DM that have no internal limits.
 */
void blk_set_stacking_limits(struct queue_limits *lim)
{
	blk_set_default_limits(lim);

	/* Inherit limits from component devices */
	lim->discard_zeroes_data = 1;
	lim->max_segments = USHRT_MAX;
	lim->max_hw_sectors = UINT_MAX;
	lim->max_segment_size = UINT_MAX;

	lim->max_sectors = BLK_DEF_MAX_SECTORS;
}
EXPORT_SYMBOL(blk_set_stacking_limits);

/**
 * blk_queue_make_request - define an alternate make_request function for a device
 * @q:  the request queue for the device to be affected
 * @mfn: the alternate make_request function
 *
 * Description:
 *    The normal way for &struct bios to be passed to a device
 *    driver is for them to be collected into requests on a request
 *    queue, and then to allow the device driver to select requests
 *    off that queue when it is ready.  This works well for many block
 *    devices. However some block devices (typically virtual devices
 *    such as md or lvm) do not benefit from the processing on the
 *    request queue, and are served best by having the requests passed
 *    directly to them.  This can be achieved by providing a function
 *    to blk_queue_make_request().
 *
 * Caveat:
 *    The driver that does this *must* be able to deal appropriately
 *    with buffers in "highmemory". This can be accomplished by either calling
 *    __bio_kmap_atomic() to get a temporary kernel mapping, or by calling
 *    blk_queue_bounce() to create a buffer in normal memory.
 **/
void blk_queue_make_request(struct request_queue *q, make_request_fn *mfn)
{
	/*
	 * set defaults
	 */
	q->nr_requests = BLKDEV_MAX_RQ;

	q->make_request_fn = mfn;
	blk_queue_dma_alignment(q, 511);
	blk_queue_congestion_threshold(q);
	q->nr_batching = BLK_BATCH_REQ;

	blk_set_default_limits(&q->limits);

	/*
	 * by default assume old behaviour and bounce for any highmem page
	 */
	blk_queue_bounce_limit(q, BLK_BOUNCE_HIGH);
}
EXPORT_SYMBOL(blk_queue_make_request);
=======
 * Prepare queue limits for applying limits from underlying devices using
 * blk_stack_limits().
 */
void blk_set_stacking_limits(struct queue_limits *lim)
{
	memset(lim, 0, sizeof(*lim));
	lim->logical_block_size = SECTOR_SIZE;
	lim->physical_block_size = SECTOR_SIZE;
	lim->io_min = SECTOR_SIZE;
	lim->discard_granularity = SECTOR_SIZE;
	lim->dma_alignment = SECTOR_SIZE - 1;
	lim->seg_boundary_mask = BLK_SEG_BOUNDARY_MASK;

	/* Inherit limits from component devices */
	lim->max_segments = USHRT_MAX;
	lim->max_discard_segments = USHRT_MAX;
	lim->max_hw_sectors = UINT_MAX;
	lim->max_segment_size = UINT_MAX;
	lim->max_sectors = UINT_MAX;
	lim->max_dev_sectors = UINT_MAX;
	lim->max_write_zeroes_sectors = UINT_MAX;
	lim->max_zone_append_sectors = UINT_MAX;
	lim->max_user_discard_sectors = UINT_MAX;
}
EXPORT_SYMBOL(blk_set_stacking_limits);

static void blk_apply_bdi_limits(struct backing_dev_info *bdi,
		struct queue_limits *lim)
{
	/*
	 * For read-ahead of large files to be effective, we need to read ahead
	 * at least twice the optimal I/O size.
	 */
	bdi->ra_pages = max(lim->io_opt * 2 / PAGE_SIZE, VM_READAHEAD_PAGES);
	bdi->io_pages = lim->max_sectors >> PAGE_SECTORS_SHIFT;
}

static int blk_validate_zoned_limits(struct queue_limits *lim)
{
	if (!lim->zoned) {
		if (WARN_ON_ONCE(lim->max_open_zones) ||
		    WARN_ON_ONCE(lim->max_active_zones) ||
		    WARN_ON_ONCE(lim->zone_write_granularity) ||
		    WARN_ON_ONCE(lim->max_zone_append_sectors))
			return -EINVAL;
		return 0;
	}

	if (WARN_ON_ONCE(!IS_ENABLED(CONFIG_BLK_DEV_ZONED)))
		return -EINVAL;

	if (lim->zone_write_granularity < lim->logical_block_size)
		lim->zone_write_granularity = lim->logical_block_size;

	if (lim->max_zone_append_sectors) {
		/*
		 * The Zone Append size is limited by the maximum I/O size
		 * and the zone size given that it can't span zones.
		 */
		lim->max_zone_append_sectors =
			min3(lim->max_hw_sectors,
			     lim->max_zone_append_sectors,
			     lim->chunk_sectors);
	}

	return 0;
}

/*
 * Check that the limits in lim are valid, initialize defaults for unset
 * values, and cap values based on others where needed.
 */
static int blk_validate_limits(struct queue_limits *lim)
{
	unsigned int max_hw_sectors;

	/*
	 * Unless otherwise specified, default to 512 byte logical blocks and a
	 * physical block size equal to the logical block size.
	 */
	if (!lim->logical_block_size)
		lim->logical_block_size = SECTOR_SIZE;
	if (lim->physical_block_size < lim->logical_block_size)
		lim->physical_block_size = lim->logical_block_size;

	/*
	 * The minimum I/O size defaults to the physical block size unless
	 * explicitly overridden.
	 */
	if (lim->io_min < lim->physical_block_size)
		lim->io_min = lim->physical_block_size;

	/*
	 * max_hw_sectors has a somewhat weird default for historical reason,
	 * but driver really should set their own instead of relying on this
	 * value.
	 *
	 * The block layer relies on the fact that every driver can
	 * handle at lest a page worth of data per I/O, and needs the value
	 * aligned to the logical block size.
	 */
	if (!lim->max_hw_sectors)
		lim->max_hw_sectors = BLK_SAFE_MAX_SECTORS;
	if (WARN_ON_ONCE(lim->max_hw_sectors < PAGE_SECTORS))
		return -EINVAL;
	lim->max_hw_sectors = round_down(lim->max_hw_sectors,
			lim->logical_block_size >> SECTOR_SHIFT);

	/*
	 * The actual max_sectors value is a complex beast and also takes the
	 * max_dev_sectors value (set by SCSI ULPs) and a user configurable
	 * value into account.  The ->max_sectors value is always calculated
	 * from these, so directly setting it won't have any effect.
	 */
	max_hw_sectors = min_not_zero(lim->max_hw_sectors,
				lim->max_dev_sectors);
	if (lim->max_user_sectors) {
		if (lim->max_user_sectors < PAGE_SIZE / SECTOR_SIZE)
			return -EINVAL;
		lim->max_sectors = min(max_hw_sectors, lim->max_user_sectors);
	} else {
		lim->max_sectors = min(max_hw_sectors, BLK_DEF_MAX_SECTORS_CAP);
	}
	lim->max_sectors = round_down(lim->max_sectors,
			lim->logical_block_size >> SECTOR_SHIFT);

	/*
	 * Random default for the maximum number of segments.  Driver should not
	 * rely on this and set their own.
	 */
	if (!lim->max_segments)
		lim->max_segments = BLK_MAX_SEGMENTS;

	lim->max_discard_sectors =
		min(lim->max_hw_discard_sectors, lim->max_user_discard_sectors);

	if (!lim->max_discard_segments)
		lim->max_discard_segments = 1;

	if (lim->discard_granularity < lim->physical_block_size)
		lim->discard_granularity = lim->physical_block_size;

	/*
	 * By default there is no limit on the segment boundary alignment,
	 * but if there is one it can't be smaller than the page size as
	 * that would break all the normal I/O patterns.
	 */
	if (!lim->seg_boundary_mask)
		lim->seg_boundary_mask = BLK_SEG_BOUNDARY_MASK;
	if (WARN_ON_ONCE(lim->seg_boundary_mask < PAGE_SIZE - 1))
		return -EINVAL;

	/*
	 * Stacking device may have both virtual boundary and max segment
	 * size limit, so allow this setting now, and long-term the two
	 * might need to move out of stacking limits since we have immutable
	 * bvec and lower layer bio splitting is supposed to handle the two
	 * correctly.
	 */
	if (lim->virt_boundary_mask) {
		if (!lim->max_segment_size)
			lim->max_segment_size = UINT_MAX;
	} else {
		/*
		 * The maximum segment size has an odd historic 64k default that
		 * drivers probably should override.  Just like the I/O size we
		 * require drivers to at least handle a full page per segment.
		 */
		if (!lim->max_segment_size)
			lim->max_segment_size = BLK_MAX_SEGMENT_SIZE;
		if (WARN_ON_ONCE(lim->max_segment_size < PAGE_SIZE))
			return -EINVAL;
	}

	/*
	 * We require drivers to at least do logical block aligned I/O, but
	 * historically could not check for that due to the separate calls
	 * to set the limits.  Once the transition is finished the check
	 * below should be narrowed down to check the logical block size.
	 */
	if (!lim->dma_alignment)
		lim->dma_alignment = SECTOR_SIZE - 1;
	if (WARN_ON_ONCE(lim->dma_alignment > PAGE_SIZE))
		return -EINVAL;

	if (lim->alignment_offset) {
		lim->alignment_offset &= (lim->physical_block_size - 1);
		lim->misaligned = 0;
	}

	return blk_validate_zoned_limits(lim);
}

/*
 * Set the default limits for a newly allocated queue.  @lim contains the
 * initial limits set by the driver, which could be no limit in which case
 * all fields are cleared to zero.
 */
int blk_set_default_limits(struct queue_limits *lim)
{
	/*
	 * Most defaults are set by capping the bounds in blk_validate_limits,
	 * but max_user_discard_sectors is special and needs an explicit
	 * initialization to the max value here.
	 */
	lim->max_user_discard_sectors = UINT_MAX;
	return blk_validate_limits(lim);
}

/**
 * queue_limits_commit_update - commit an atomic update of queue limits
 * @q:		queue to update
 * @lim:	limits to apply
 *
 * Apply the limits in @lim that were obtained from queue_limits_start_update()
 * and updated by the caller to @q.
 *
 * Returns 0 if successful, else a negative error code.
 */
int queue_limits_commit_update(struct request_queue *q,
		struct queue_limits *lim)
	__releases(q->limits_lock)
{
	int error = blk_validate_limits(lim);

	if (!error) {
		q->limits = *lim;
		if (q->disk)
			blk_apply_bdi_limits(q->disk->bdi, lim);
	}
	mutex_unlock(&q->limits_lock);
	return error;
}
EXPORT_SYMBOL_GPL(queue_limits_commit_update);

/**
 * queue_limits_set - apply queue limits to queue
 * @q:		queue to update
 * @lim:	limits to apply
 *
 * Apply the limits in @lim that were freshly initialized to @q.
 * To update existing limits use queue_limits_start_update() and
 * queue_limits_commit_update() instead.
 *
 * Returns 0 if successful, else a negative error code.
 */
int queue_limits_set(struct request_queue *q, struct queue_limits *lim)
{
	mutex_lock(&q->limits_lock);
	return queue_limits_commit_update(q, lim);
}
EXPORT_SYMBOL_GPL(queue_limits_set);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

/**
 * blk_queue_bounce_limit - set bounce buffer limit for queue
 * @q: the request queue for the device
<<<<<<< HEAD
 * @dma_mask: the maximum address the device can handle
 *
 * Description:
 *    Different hardware can have different requirements as to what pages
 *    it can do I/O directly to. A low level driver can call
 *    blk_queue_bounce_limit to have lower memory pages allocated as bounce
 *    buffers for doing I/O to pages residing above @dma_mask.
 **/
void blk_queue_bounce_limit(struct request_queue *q, u64 dma_mask)
{
	unsigned long b_pfn = dma_mask >> PAGE_SHIFT;
	int dma = 0;

	q->bounce_gfp = GFP_NOIO;
#if BITS_PER_LONG == 64
	/*
	 * Assume anything <= 4GB can be handled by IOMMU.  Actually
	 * some IOMMUs can handle everything, but I don't know of a
	 * way to test this here.
	 */
	if (b_pfn < (min_t(u64, 0xffffffffUL, BLK_BOUNCE_HIGH) >> PAGE_SHIFT))
		dma = 1;
	q->limits.bounce_pfn = max(max_low_pfn, b_pfn);
#else
	if (b_pfn < blk_max_low_pfn)
		dma = 1;
	q->limits.bounce_pfn = b_pfn;
#endif
	if (dma) {
		init_emergency_isa_pool();
		q->bounce_gfp = GFP_NOIO | GFP_DMA;
		q->limits.bounce_pfn = b_pfn;
	}
=======
 * @bounce: bounce limit to enforce
 *
 * Description:
 *    Force bouncing for ISA DMA ranges or highmem.
 *
 *    DEPRECATED, don't use in new code.
 **/
void blk_queue_bounce_limit(struct request_queue *q, enum blk_bounce bounce)
{
	q->limits.bounce = bounce;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}
EXPORT_SYMBOL(blk_queue_bounce_limit);

/**
<<<<<<< HEAD
 * blk_limits_max_hw_sectors - set hard and soft limit of max sectors for request
 * @limits: the queue limits
 * @max_hw_sectors:  max hardware sectors in the usual 512b unit
 *
 * Description:
 *    Enables a low level driver to set a hard upper limit,
 *    max_hw_sectors, on the size of requests.  max_hw_sectors is set by
 *    the device driver based upon the combined capabilities of I/O
 *    controller and storage device.
 *
 *    max_sectors is a soft limit imposed by the block layer for
 *    filesystem type requests.  This value can be overridden on a
 *    per-device basis in /sys/block/<device>/queue/max_sectors_kb.
 *    The soft limit can not exceed max_hw_sectors.
 **/
void blk_limits_max_hw_sectors(struct queue_limits *limits, unsigned int max_hw_sectors)
{
	if ((max_hw_sectors << 9) < PAGE_CACHE_SIZE) {
		max_hw_sectors = 1 << (PAGE_CACHE_SHIFT - 9);
		printk(KERN_INFO "%s: set to minimum %d\n",
		       __func__, max_hw_sectors);
	}

	limits->max_hw_sectors = max_hw_sectors;
	limits->max_sectors = min_t(unsigned int, max_hw_sectors,
				    BLK_DEF_MAX_SECTORS);
}
EXPORT_SYMBOL(blk_limits_max_hw_sectors);

/**
=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 * blk_queue_max_hw_sectors - set max sectors for a request for this queue
 * @q:  the request queue for the device
 * @max_hw_sectors:  max hardware sectors in the usual 512b unit
 *
 * Description:
<<<<<<< HEAD
 *    See description for blk_limits_max_hw_sectors().
 **/
void blk_queue_max_hw_sectors(struct request_queue *q, unsigned int max_hw_sectors)
{
	blk_limits_max_hw_sectors(&q->limits, max_hw_sectors);
=======
 *    Enables a low level driver to set a hard upper limit,
 *    max_hw_sectors, on the size of requests.  max_hw_sectors is set by
 *    the device driver based upon the capabilities of the I/O
 *    controller.
 *
 *    max_dev_sectors is a hard limit imposed by the storage device for
 *    READ/WRITE requests. It is set by the disk driver.
 *
 *    max_sectors is a soft limit imposed by the block layer for
 *    filesystem type requests.  This value can be overridden on a
 *    per-device basis in /sys/block/<device>/queue/max_sectors_kb.
 *    The soft limit can not exceed max_hw_sectors.
 **/
void blk_queue_max_hw_sectors(struct request_queue *q, unsigned int max_hw_sectors)
{
	struct queue_limits *limits = &q->limits;
	unsigned int max_sectors;

	if ((max_hw_sectors << 9) < PAGE_SIZE) {
		max_hw_sectors = 1 << (PAGE_SHIFT - 9);
		pr_info("%s: set to minimum %u\n", __func__, max_hw_sectors);
	}

	max_hw_sectors = round_down(max_hw_sectors,
				    limits->logical_block_size >> SECTOR_SHIFT);
	limits->max_hw_sectors = max_hw_sectors;

	max_sectors = min_not_zero(max_hw_sectors, limits->max_dev_sectors);

	if (limits->max_user_sectors)
		max_sectors = min(max_sectors, limits->max_user_sectors);
	else
		max_sectors = min(max_sectors, BLK_DEF_MAX_SECTORS_CAP);

	max_sectors = round_down(max_sectors,
				 limits->logical_block_size >> SECTOR_SHIFT);
	limits->max_sectors = max_sectors;

	if (!q->disk)
		return;
	q->disk->bdi->io_pages = max_sectors >> (PAGE_SHIFT - 9);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}
EXPORT_SYMBOL(blk_queue_max_hw_sectors);

/**
<<<<<<< HEAD
=======
 * blk_queue_chunk_sectors - set size of the chunk for this queue
 * @q:  the request queue for the device
 * @chunk_sectors:  chunk sectors in the usual 512b unit
 *
 * Description:
 *    If a driver doesn't want IOs to cross a given chunk size, it can set
 *    this limit and prevent merging across chunks. Note that the block layer
 *    must accept a page worth of data at any offset. So if the crossing of
 *    chunks is a hard limitation in the driver, it must still be prepared
 *    to split single page bios.
 **/
void blk_queue_chunk_sectors(struct request_queue *q, unsigned int chunk_sectors)
{
	q->limits.chunk_sectors = chunk_sectors;
}
EXPORT_SYMBOL(blk_queue_chunk_sectors);

/**
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 * blk_queue_max_discard_sectors - set max sectors for a single discard
 * @q:  the request queue for the device
 * @max_discard_sectors: maximum number of sectors to discard
 **/
void blk_queue_max_discard_sectors(struct request_queue *q,
		unsigned int max_discard_sectors)
{
<<<<<<< HEAD
	q->limits.max_discard_sectors = max_discard_sectors;
=======
	struct queue_limits *lim = &q->limits;

	lim->max_hw_discard_sectors = max_discard_sectors;
	lim->max_discard_sectors =
		min(max_discard_sectors, lim->max_user_discard_sectors);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}
EXPORT_SYMBOL(blk_queue_max_discard_sectors);

/**
<<<<<<< HEAD
=======
 * blk_queue_max_secure_erase_sectors - set max sectors for a secure erase
 * @q:  the request queue for the device
 * @max_sectors: maximum number of sectors to secure_erase
 **/
void blk_queue_max_secure_erase_sectors(struct request_queue *q,
		unsigned int max_sectors)
{
	q->limits.max_secure_erase_sectors = max_sectors;
}
EXPORT_SYMBOL(blk_queue_max_secure_erase_sectors);

/**
 * blk_queue_max_write_zeroes_sectors - set max sectors for a single
 *                                      write zeroes
 * @q:  the request queue for the device
 * @max_write_zeroes_sectors: maximum number of sectors to write per command
 **/
void blk_queue_max_write_zeroes_sectors(struct request_queue *q,
		unsigned int max_write_zeroes_sectors)
{
	q->limits.max_write_zeroes_sectors = max_write_zeroes_sectors;
}
EXPORT_SYMBOL(blk_queue_max_write_zeroes_sectors);

/**
 * blk_queue_max_zone_append_sectors - set max sectors for a single zone append
 * @q:  the request queue for the device
 * @max_zone_append_sectors: maximum number of sectors to write per command
 **/
void blk_queue_max_zone_append_sectors(struct request_queue *q,
		unsigned int max_zone_append_sectors)
{
	unsigned int max_sectors;

	if (WARN_ON(!blk_queue_is_zoned(q)))
		return;

	max_sectors = min(q->limits.max_hw_sectors, max_zone_append_sectors);
	max_sectors = min(q->limits.chunk_sectors, max_sectors);

	/*
	 * Signal eventual driver bugs resulting in the max_zone_append sectors limit
	 * being 0 due to a 0 argument, the chunk_sectors limit (zone size) not set,
	 * or the max_hw_sectors limit not set.
	 */
	WARN_ON(!max_sectors);

	q->limits.max_zone_append_sectors = max_sectors;
}
EXPORT_SYMBOL_GPL(blk_queue_max_zone_append_sectors);

/**
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 * blk_queue_max_segments - set max hw segments for a request for this queue
 * @q:  the request queue for the device
 * @max_segments:  max number of segments
 *
 * Description:
 *    Enables a low level driver to set an upper limit on the number of
 *    hw data segments in a request.
 **/
void blk_queue_max_segments(struct request_queue *q, unsigned short max_segments)
{
	if (!max_segments) {
		max_segments = 1;
<<<<<<< HEAD
		printk(KERN_INFO "%s: set to minimum %d\n",
		       __func__, max_segments);
=======
		pr_info("%s: set to minimum %u\n", __func__, max_segments);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	}

	q->limits.max_segments = max_segments;
}
EXPORT_SYMBOL(blk_queue_max_segments);

/**
<<<<<<< HEAD
=======
 * blk_queue_max_discard_segments - set max segments for discard requests
 * @q:  the request queue for the device
 * @max_segments:  max number of segments
 *
 * Description:
 *    Enables a low level driver to set an upper limit on the number of
 *    segments in a discard request.
 **/
void blk_queue_max_discard_segments(struct request_queue *q,
		unsigned short max_segments)
{
	q->limits.max_discard_segments = max_segments;
}
EXPORT_SYMBOL_GPL(blk_queue_max_discard_segments);

/**
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 * blk_queue_max_segment_size - set max segment size for blk_rq_map_sg
 * @q:  the request queue for the device
 * @max_size:  max size of segment in bytes
 *
 * Description:
 *    Enables a low level driver to set an upper limit on the size of a
 *    coalesced segment
 **/
void blk_queue_max_segment_size(struct request_queue *q, unsigned int max_size)
{
<<<<<<< HEAD
	if (max_size < PAGE_CACHE_SIZE) {
		max_size = PAGE_CACHE_SIZE;
		printk(KERN_INFO "%s: set to minimum %d\n",
		       __func__, max_size);
	}

=======
	if (max_size < PAGE_SIZE) {
		max_size = PAGE_SIZE;
		pr_info("%s: set to minimum %u\n", __func__, max_size);
	}

	/* see blk_queue_virt_boundary() for the explanation */
	WARN_ON_ONCE(q->limits.virt_boundary_mask);

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	q->limits.max_segment_size = max_size;
}
EXPORT_SYMBOL(blk_queue_max_segment_size);

/**
 * blk_queue_logical_block_size - set logical block size for the queue
 * @q:  the request queue for the device
 * @size:  the logical block size, in bytes
 *
 * Description:
 *   This should be set to the lowest possible block size that the
 *   storage device can address.  The default of 512 covers most
 *   hardware.
 **/
<<<<<<< HEAD
void blk_queue_logical_block_size(struct request_queue *q, unsigned short size)
{
	q->limits.logical_block_size = size;

	if (q->limits.physical_block_size < size)
		q->limits.physical_block_size = size;

	if (q->limits.io_min < q->limits.physical_block_size)
		q->limits.io_min = q->limits.physical_block_size;
=======
void blk_queue_logical_block_size(struct request_queue *q, unsigned int size)
{
	struct queue_limits *limits = &q->limits;

	limits->logical_block_size = size;

	if (limits->discard_granularity < limits->logical_block_size)
		limits->discard_granularity = limits->logical_block_size;

	if (limits->physical_block_size < size)
		limits->physical_block_size = size;

	if (limits->io_min < limits->physical_block_size)
		limits->io_min = limits->physical_block_size;

	limits->max_hw_sectors =
		round_down(limits->max_hw_sectors, size >> SECTOR_SHIFT);
	limits->max_sectors =
		round_down(limits->max_sectors, size >> SECTOR_SHIFT);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}
EXPORT_SYMBOL(blk_queue_logical_block_size);

/**
 * blk_queue_physical_block_size - set physical block size for the queue
 * @q:  the request queue for the device
 * @size:  the physical block size, in bytes
 *
 * Description:
 *   This should be set to the lowest possible sector size that the
 *   hardware can operate on without reverting to read-modify-write
 *   operations.
 */
void blk_queue_physical_block_size(struct request_queue *q, unsigned int size)
{
	q->limits.physical_block_size = size;

	if (q->limits.physical_block_size < q->limits.logical_block_size)
		q->limits.physical_block_size = q->limits.logical_block_size;

<<<<<<< HEAD
=======
	if (q->limits.discard_granularity < q->limits.physical_block_size)
		q->limits.discard_granularity = q->limits.physical_block_size;

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	if (q->limits.io_min < q->limits.physical_block_size)
		q->limits.io_min = q->limits.physical_block_size;
}
EXPORT_SYMBOL(blk_queue_physical_block_size);

/**
<<<<<<< HEAD
=======
 * blk_queue_zone_write_granularity - set zone write granularity for the queue
 * @q:  the request queue for the zoned device
 * @size:  the zone write granularity size, in bytes
 *
 * Description:
 *   This should be set to the lowest possible size allowing to write in
 *   sequential zones of a zoned block device.
 */
void blk_queue_zone_write_granularity(struct request_queue *q,
				      unsigned int size)
{
	if (WARN_ON_ONCE(!blk_queue_is_zoned(q)))
		return;

	q->limits.zone_write_granularity = size;

	if (q->limits.zone_write_granularity < q->limits.logical_block_size)
		q->limits.zone_write_granularity = q->limits.logical_block_size;
}
EXPORT_SYMBOL_GPL(blk_queue_zone_write_granularity);

/**
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 * blk_queue_alignment_offset - set physical block alignment offset
 * @q:	the request queue for the device
 * @offset: alignment offset in bytes
 *
 * Description:
 *   Some devices are naturally misaligned to compensate for things like
 *   the legacy DOS partition table 63-sector offset.  Low-level drivers
 *   should call this function for devices whose first sector is not
 *   naturally aligned.
 */
void blk_queue_alignment_offset(struct request_queue *q, unsigned int offset)
{
	q->limits.alignment_offset =
		offset & (q->limits.physical_block_size - 1);
	q->limits.misaligned = 0;
}
EXPORT_SYMBOL(blk_queue_alignment_offset);

<<<<<<< HEAD
=======
void disk_update_readahead(struct gendisk *disk)
{
	blk_apply_bdi_limits(disk->bdi, &disk->queue->limits);
}
EXPORT_SYMBOL_GPL(disk_update_readahead);

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
/**
 * blk_limits_io_min - set minimum request size for a device
 * @limits: the queue limits
 * @min:  smallest I/O size in bytes
 *
 * Description:
 *   Some devices have an internal block size bigger than the reported
 *   hardware sector size.  This function can be used to signal the
 *   smallest I/O the device can perform without incurring a performance
 *   penalty.
 */
void blk_limits_io_min(struct queue_limits *limits, unsigned int min)
{
	limits->io_min = min;

	if (limits->io_min < limits->logical_block_size)
		limits->io_min = limits->logical_block_size;

	if (limits->io_min < limits->physical_block_size)
		limits->io_min = limits->physical_block_size;
}
EXPORT_SYMBOL(blk_limits_io_min);

/**
 * blk_queue_io_min - set minimum request size for the queue
 * @q:	the request queue for the device
 * @min:  smallest I/O size in bytes
 *
 * Description:
 *   Storage devices may report a granularity or preferred minimum I/O
 *   size which is the smallest request the device can perform without
 *   incurring a performance penalty.  For disk drives this is often the
 *   physical block size.  For RAID arrays it is often the stripe chunk
 *   size.  A properly aligned multiple of minimum_io_size is the
 *   preferred request size for workloads where a high number of I/O
 *   operations is desired.
 */
void blk_queue_io_min(struct request_queue *q, unsigned int min)
{
	blk_limits_io_min(&q->limits, min);
}
EXPORT_SYMBOL(blk_queue_io_min);

/**
 * blk_limits_io_opt - set optimal request size for a device
 * @limits: the queue limits
 * @opt:  smallest I/O size in bytes
 *
 * Description:
 *   Storage devices may report an optimal I/O size, which is the
 *   device's preferred unit for sustained I/O.  This is rarely reported
 *   for disk drives.  For RAID arrays it is usually the stripe width or
 *   the internal track size.  A properly aligned multiple of
 *   optimal_io_size is the preferred request size for workloads where
 *   sustained throughput is desired.
 */
void blk_limits_io_opt(struct queue_limits *limits, unsigned int opt)
{
	limits->io_opt = opt;
}
EXPORT_SYMBOL(blk_limits_io_opt);

/**
 * blk_queue_io_opt - set optimal request size for the queue
 * @q:	the request queue for the device
 * @opt:  optimal request size in bytes
 *
 * Description:
 *   Storage devices may report an optimal I/O size, which is the
 *   device's preferred unit for sustained I/O.  This is rarely reported
 *   for disk drives.  For RAID arrays it is usually the stripe width or
 *   the internal track size.  A properly aligned multiple of
 *   optimal_io_size is the preferred request size for workloads where
 *   sustained throughput is desired.
 */
void blk_queue_io_opt(struct request_queue *q, unsigned int opt)
{
	blk_limits_io_opt(&q->limits, opt);
<<<<<<< HEAD
}
EXPORT_SYMBOL(blk_queue_io_opt);

/**
 * blk_queue_stack_limits - inherit underlying queue limits for stacked drivers
 * @t:	the stacking driver (top)
 * @b:  the underlying device (bottom)
 **/
void blk_queue_stack_limits(struct request_queue *t, struct request_queue *b)
{
	blk_stack_limits(&t->limits, &b->limits, 0);
}
EXPORT_SYMBOL(blk_queue_stack_limits);
=======
	if (!q->disk)
		return;
	q->disk->bdi->ra_pages =
		max(queue_io_opt(q) * 2 / PAGE_SIZE, VM_READAHEAD_PAGES);
}
EXPORT_SYMBOL(blk_queue_io_opt);

static int queue_limit_alignment_offset(const struct queue_limits *lim,
		sector_t sector)
{
	unsigned int granularity = max(lim->physical_block_size, lim->io_min);
	unsigned int alignment = sector_div(sector, granularity >> SECTOR_SHIFT)
		<< SECTOR_SHIFT;

	return (granularity + lim->alignment_offset - alignment) % granularity;
}

static unsigned int queue_limit_discard_alignment(
		const struct queue_limits *lim, sector_t sector)
{
	unsigned int alignment, granularity, offset;

	if (!lim->max_discard_sectors)
		return 0;

	/* Why are these in bytes, not sectors? */
	alignment = lim->discard_alignment >> SECTOR_SHIFT;
	granularity = lim->discard_granularity >> SECTOR_SHIFT;
	if (!granularity)
		return 0;

	/* Offset of the partition start in 'granularity' sectors */
	offset = sector_div(sector, granularity);

	/* And why do we do this modulus *again* in blkdev_issue_discard()? */
	offset = (granularity + alignment - offset) % granularity;

	/* Turn it back into bytes, gaah */
	return offset << SECTOR_SHIFT;
}

static unsigned int blk_round_down_sectors(unsigned int sectors, unsigned int lbs)
{
	sectors = round_down(sectors, lbs >> SECTOR_SHIFT);
	if (sectors < PAGE_SIZE >> SECTOR_SHIFT)
		sectors = PAGE_SIZE >> SECTOR_SHIFT;
	return sectors;
}
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

/**
 * blk_stack_limits - adjust queue_limits for stacked devices
 * @t:	the stacking driver limits (top device)
 * @b:  the underlying queue limits (bottom, component device)
 * @start:  first data sector within component device
 *
 * Description:
 *    This function is used by stacking drivers like MD and DM to ensure
 *    that all component devices have compatible block sizes and
 *    alignments.  The stacking driver must provide a queue_limits
 *    struct (top) and then iteratively call the stacking function for
 *    all component (bottom) devices.  The stacking function will
 *    attempt to combine the values and ensure proper alignment.
 *
 *    Returns 0 if the top and bottom queue_limits are compatible.  The
 *    top device's block sizes and alignment offsets may be adjusted to
 *    ensure alignment with the bottom device. If no compatible sizes
 *    and alignments exist, -1 is returned and the resulting top
 *    queue_limits will have the misaligned flag set to indicate that
 *    the alignment_offset is undefined.
 */
int blk_stack_limits(struct queue_limits *t, struct queue_limits *b,
		     sector_t start)
{
	unsigned int top, bottom, alignment, ret = 0;

	t->max_sectors = min_not_zero(t->max_sectors, b->max_sectors);
	t->max_hw_sectors = min_not_zero(t->max_hw_sectors, b->max_hw_sectors);
<<<<<<< HEAD
	t->bounce_pfn = min_not_zero(t->bounce_pfn, b->bounce_pfn);

	t->seg_boundary_mask = min_not_zero(t->seg_boundary_mask,
					    b->seg_boundary_mask);

	t->max_segments = min_not_zero(t->max_segments, b->max_segments);
=======
	t->max_dev_sectors = min_not_zero(t->max_dev_sectors, b->max_dev_sectors);
	t->max_write_zeroes_sectors = min(t->max_write_zeroes_sectors,
					b->max_write_zeroes_sectors);
	t->max_zone_append_sectors = min(t->max_zone_append_sectors,
					b->max_zone_append_sectors);
	t->bounce = max(t->bounce, b->bounce);

	t->seg_boundary_mask = min_not_zero(t->seg_boundary_mask,
					    b->seg_boundary_mask);
	t->virt_boundary_mask = min_not_zero(t->virt_boundary_mask,
					    b->virt_boundary_mask);

	t->max_segments = min_not_zero(t->max_segments, b->max_segments);
	t->max_discard_segments = min_not_zero(t->max_discard_segments,
					       b->max_discard_segments);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	t->max_integrity_segments = min_not_zero(t->max_integrity_segments,
						 b->max_integrity_segments);

	t->max_segment_size = min_not_zero(t->max_segment_size,
					   b->max_segment_size);

	t->misaligned |= b->misaligned;

	alignment = queue_limit_alignment_offset(b, start);

	/* Bottom device has different alignment.  Check that it is
	 * compatible with the current top alignment.
	 */
	if (t->alignment_offset != alignment) {

		top = max(t->physical_block_size, t->io_min)
			+ t->alignment_offset;
		bottom = max(b->physical_block_size, b->io_min) + alignment;

		/* Verify that top and bottom intervals line up */
		if (max(top, bottom) % min(top, bottom)) {
			t->misaligned = 1;
			ret = -1;
		}
	}

	t->logical_block_size = max(t->logical_block_size,
				    b->logical_block_size);

	t->physical_block_size = max(t->physical_block_size,
				     b->physical_block_size);

	t->io_min = max(t->io_min, b->io_min);
<<<<<<< HEAD
	t->io_opt = lcm(t->io_opt, b->io_opt);

	t->cluster &= b->cluster;
	t->discard_zeroes_data &= b->discard_zeroes_data;
=======
	t->io_opt = lcm_not_zero(t->io_opt, b->io_opt);
	t->dma_alignment = max(t->dma_alignment, b->dma_alignment);

	/* Set non-power-of-2 compatible chunk_sectors boundary */
	if (b->chunk_sectors)
		t->chunk_sectors = gcd(t->chunk_sectors, b->chunk_sectors);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	/* Physical block size a multiple of the logical block size? */
	if (t->physical_block_size & (t->logical_block_size - 1)) {
		t->physical_block_size = t->logical_block_size;
		t->misaligned = 1;
		ret = -1;
	}

	/* Minimum I/O a multiple of the physical block size? */
	if (t->io_min & (t->physical_block_size - 1)) {
		t->io_min = t->physical_block_size;
		t->misaligned = 1;
		ret = -1;
	}

	/* Optimal I/O a multiple of the physical block size? */
	if (t->io_opt & (t->physical_block_size - 1)) {
		t->io_opt = 0;
		t->misaligned = 1;
		ret = -1;
	}

<<<<<<< HEAD
	/* Find lowest common alignment_offset */
	t->alignment_offset = lcm(t->alignment_offset, alignment)
=======
	/* chunk_sectors a multiple of the physical block size? */
	if ((t->chunk_sectors << 9) & (t->physical_block_size - 1)) {
		t->chunk_sectors = 0;
		t->misaligned = 1;
		ret = -1;
	}

	t->raid_partial_stripes_expensive =
		max(t->raid_partial_stripes_expensive,
		    b->raid_partial_stripes_expensive);

	/* Find lowest common alignment_offset */
	t->alignment_offset = lcm_not_zero(t->alignment_offset, alignment)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		% max(t->physical_block_size, t->io_min);

	/* Verify that new alignment_offset is on a logical block boundary */
	if (t->alignment_offset & (t->logical_block_size - 1)) {
		t->misaligned = 1;
		ret = -1;
	}

<<<<<<< HEAD
=======
	t->max_sectors = blk_round_down_sectors(t->max_sectors, t->logical_block_size);
	t->max_hw_sectors = blk_round_down_sectors(t->max_hw_sectors, t->logical_block_size);
	t->max_dev_sectors = blk_round_down_sectors(t->max_dev_sectors, t->logical_block_size);

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	/* Discard alignment and granularity */
	if (b->discard_granularity) {
		alignment = queue_limit_discard_alignment(b, start);

		if (t->discard_granularity != 0 &&
		    t->discard_alignment != alignment) {
			top = t->discard_granularity + t->discard_alignment;
			bottom = b->discard_granularity + alignment;

			/* Verify that top and bottom intervals line up */
<<<<<<< HEAD
			if (max(top, bottom) & (min(top, bottom) - 1))
=======
			if ((max(top, bottom) % min(top, bottom)) != 0)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
				t->discard_misaligned = 1;
		}

		t->max_discard_sectors = min_not_zero(t->max_discard_sectors,
						      b->max_discard_sectors);
<<<<<<< HEAD
		t->discard_granularity = max(t->discard_granularity,
					     b->discard_granularity);
		t->discard_alignment = lcm(t->discard_alignment, alignment) &
			(t->discard_granularity - 1);
	}

=======
		t->max_hw_discard_sectors = min_not_zero(t->max_hw_discard_sectors,
							 b->max_hw_discard_sectors);
		t->discard_granularity = max(t->discard_granularity,
					     b->discard_granularity);
		t->discard_alignment = lcm_not_zero(t->discard_alignment, alignment) %
			t->discard_granularity;
	}
	t->max_secure_erase_sectors = min_not_zero(t->max_secure_erase_sectors,
						   b->max_secure_erase_sectors);
	t->zone_write_granularity = max(t->zone_write_granularity,
					b->zone_write_granularity);
	t->zoned = max(t->zoned, b->zoned);
	if (!t->zoned) {
		t->zone_write_granularity = 0;
		t->max_zone_append_sectors = 0;
	}
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	return ret;
}
EXPORT_SYMBOL(blk_stack_limits);

/**
<<<<<<< HEAD
 * bdev_stack_limits - adjust queue limits for stacked drivers
 * @t:	the stacking driver limits (top device)
 * @bdev:  the component block_device (bottom)
 * @start:  first data sector within component device
 *
 * Description:
 *    Merges queue limits for a top device and a block_device.  Returns
 *    0 if alignment didn't change.  Returns -1 if adding the bottom
 *    device caused misalignment.
 */
int bdev_stack_limits(struct queue_limits *t, struct block_device *bdev,
		      sector_t start)
{
	struct request_queue *bq = bdev_get_queue(bdev);

	start += get_start_sect(bdev);

	return blk_stack_limits(t, &bq->limits, start);
}
EXPORT_SYMBOL(bdev_stack_limits);

/**
 * disk_stack_limits - adjust queue limits for stacked drivers
 * @disk:  MD/DM gendisk (top)
 * @bdev:  the underlying block device (bottom)
 * @offset:  offset to beginning of data within component device
 *
 * Description:
 *    Merges the limits for a top level gendisk and a bottom level
 *    block_device.
 */
void disk_stack_limits(struct gendisk *disk, struct block_device *bdev,
		       sector_t offset)
{
	struct request_queue *t = disk->queue;

	if (bdev_stack_limits(&t->limits, bdev, offset >> 9) < 0) {
		char top[BDEVNAME_SIZE], bottom[BDEVNAME_SIZE];

		disk_name(disk, 0, top);
		bdevname(bdev, bottom);

		printk(KERN_NOTICE "%s: Warning: Device %s is misaligned\n",
		       top, bottom);
	}
}
EXPORT_SYMBOL(disk_stack_limits);

/**
 * blk_queue_dma_pad - set pad mask
 * @q:     the request queue for the device
 * @mask:  pad mask
 *
 * Set dma pad mask.
 *
 * Appending pad buffer to a request modifies the last entry of a
 * scatter list such that it includes the pad buffer.
 **/
void blk_queue_dma_pad(struct request_queue *q, unsigned int mask)
{
	q->dma_pad_mask = mask;
}
EXPORT_SYMBOL(blk_queue_dma_pad);
=======
 * queue_limits_stack_bdev - adjust queue_limits for stacked devices
 * @t:	the stacking driver limits (top device)
 * @bdev:  the underlying block device (bottom)
 * @offset:  offset to beginning of data within component device
 * @pfx: prefix to use for warnings logged
 *
 * Description:
 *    This function is used by stacking drivers like MD and DM to ensure
 *    that all component devices have compatible block sizes and
 *    alignments.  The stacking driver must provide a queue_limits
 *    struct (top) and then iteratively call the stacking function for
 *    all component (bottom) devices.  The stacking function will
 *    attempt to combine the values and ensure proper alignment.
 */
void queue_limits_stack_bdev(struct queue_limits *t, struct block_device *bdev,
		sector_t offset, const char *pfx)
{
	if (blk_stack_limits(t, &bdev_get_queue(bdev)->limits,
			get_start_sect(bdev) + offset))
		pr_notice("%s: Warning: Device %pg is misaligned\n",
			pfx, bdev);
}
EXPORT_SYMBOL_GPL(queue_limits_stack_bdev);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

/**
 * blk_queue_update_dma_pad - update pad mask
 * @q:     the request queue for the device
 * @mask:  pad mask
 *
 * Update dma pad mask.
 *
 * Appending pad buffer to a request modifies the last entry of a
 * scatter list such that it includes the pad buffer.
 **/
void blk_queue_update_dma_pad(struct request_queue *q, unsigned int mask)
{
	if (mask > q->dma_pad_mask)
		q->dma_pad_mask = mask;
}
EXPORT_SYMBOL(blk_queue_update_dma_pad);

/**
<<<<<<< HEAD
 * blk_queue_dma_drain - Set up a drain buffer for excess dma.
 * @q:  the request queue for the device
 * @dma_drain_needed: fn which returns non-zero if drain is necessary
 * @buf:	physically contiguous buffer
 * @size:	size of the buffer in bytes
 *
 * Some devices have excess DMA problems and can't simply discard (or
 * zero fill) the unwanted piece of the transfer.  They have to have a
 * real area of memory to transfer it into.  The use case for this is
 * ATAPI devices in DMA mode.  If the packet command causes a transfer
 * bigger than the transfer size some HBAs will lock up if there
 * aren't DMA elements to contain the excess transfer.  What this API
 * does is adjust the queue so that the buf is always appended
 * silently to the scatterlist.
 *
 * Note: This routine adjusts max_hw_segments to make room for appending
 * the drain buffer.  If you call blk_queue_max_segments() after calling
 * this routine, you must set the limit to one fewer than your device
 * can support otherwise there won't be room for the drain buffer.
 */
int blk_queue_dma_drain(struct request_queue *q,
			       dma_drain_needed_fn *dma_drain_needed,
			       void *buf, unsigned int size)
{
	if (queue_max_segments(q) < 2)
		return -EINVAL;
	/* make room for appending the drain */
	blk_queue_max_segments(q, queue_max_segments(q) - 1);
	q->dma_drain_needed = dma_drain_needed;
	q->dma_drain_buffer = buf;
	q->dma_drain_size = size;

	return 0;
}
EXPORT_SYMBOL_GPL(blk_queue_dma_drain);

/**
=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 * blk_queue_segment_boundary - set boundary rules for segment merging
 * @q:  the request queue for the device
 * @mask:  the memory boundary mask
 **/
void blk_queue_segment_boundary(struct request_queue *q, unsigned long mask)
{
<<<<<<< HEAD
	if (mask < PAGE_CACHE_SIZE - 1) {
		mask = PAGE_CACHE_SIZE - 1;
		printk(KERN_INFO "%s: set to minimum %lx\n",
		       __func__, mask);
=======
	if (mask < PAGE_SIZE - 1) {
		mask = PAGE_SIZE - 1;
		pr_info("%s: set to minimum %lx\n", __func__, mask);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	}

	q->limits.seg_boundary_mask = mask;
}
EXPORT_SYMBOL(blk_queue_segment_boundary);

/**
<<<<<<< HEAD
=======
 * blk_queue_virt_boundary - set boundary rules for bio merging
 * @q:  the request queue for the device
 * @mask:  the memory boundary mask
 **/
void blk_queue_virt_boundary(struct request_queue *q, unsigned long mask)
{
	q->limits.virt_boundary_mask = mask;

	/*
	 * Devices that require a virtual boundary do not support scatter/gather
	 * I/O natively, but instead require a descriptor list entry for each
	 * page (which might not be idential to the Linux PAGE_SIZE).  Because
	 * of that they are not limited by our notion of "segment size".
	 */
	if (mask)
		q->limits.max_segment_size = UINT_MAX;
}
EXPORT_SYMBOL(blk_queue_virt_boundary);

/**
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 * blk_queue_dma_alignment - set dma length and memory alignment
 * @q:     the request queue for the device
 * @mask:  alignment mask
 *
 * description:
 *    set required memory and length alignment for direct dma transactions.
 *    this is used when building direct io requests for the queue.
 *
 **/
void blk_queue_dma_alignment(struct request_queue *q, int mask)
{
<<<<<<< HEAD
	q->dma_alignment = mask;
=======
	q->limits.dma_alignment = mask;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}
EXPORT_SYMBOL(blk_queue_dma_alignment);

/**
 * blk_queue_update_dma_alignment - update dma length and memory alignment
 * @q:     the request queue for the device
 * @mask:  alignment mask
 *
 * description:
 *    update required memory and length alignment for direct dma transactions.
 *    If the requested alignment is larger than the current alignment, then
 *    the current queue alignment is updated to the new value, otherwise it
 *    is left alone.  The design of this is to allow multiple objects
 *    (driver, device, transport etc) to set their respective
 *    alignments without having them interfere.
 *
 **/
void blk_queue_update_dma_alignment(struct request_queue *q, int mask)
{
	BUG_ON(mask > PAGE_SIZE);

<<<<<<< HEAD
	if (mask > q->dma_alignment)
		q->dma_alignment = mask;
=======
	if (mask > q->limits.dma_alignment)
		q->limits.dma_alignment = mask;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}
EXPORT_SYMBOL(blk_queue_update_dma_alignment);

/**
<<<<<<< HEAD
 * blk_queue_flush - configure queue's cache flush capability
 * @q:		the request queue for the device
 * @flush:	0, REQ_FLUSH or REQ_FLUSH | REQ_FUA
 *
 * Tell block layer cache flush capability of @q.  If it supports
 * flushing, REQ_FLUSH should be set.  If it supports bypassing
 * write cache for individual writes, REQ_FUA should be set.
 */
void blk_queue_flush(struct request_queue *q, unsigned int flush)
{
	WARN_ON_ONCE(flush & ~(REQ_FLUSH | REQ_FUA));

	if (WARN_ON_ONCE(!(flush & REQ_FLUSH) && (flush & REQ_FUA)))
		flush &= ~REQ_FUA;

	q->flush_flags = flush & (REQ_FLUSH | REQ_FUA);
}
EXPORT_SYMBOL_GPL(blk_queue_flush);

void blk_queue_flush_queueable(struct request_queue *q, bool queueable)
{
	q->flush_not_queueable = !queueable;
}
EXPORT_SYMBOL_GPL(blk_queue_flush_queueable);

static int __init blk_settings_init(void)
{
	blk_max_low_pfn = max_low_pfn - 1;
	blk_max_pfn = max_pfn - 1;
	return 0;
}
subsys_initcall(blk_settings_init);
=======
 * blk_set_queue_depth - tell the block layer about the device queue depth
 * @q:		the request queue for the device
 * @depth:		queue depth
 *
 */
void blk_set_queue_depth(struct request_queue *q, unsigned int depth)
{
	q->queue_depth = depth;
	rq_qos_queue_depth_changed(q);
}
EXPORT_SYMBOL(blk_set_queue_depth);

/**
 * blk_queue_write_cache - configure queue's write cache
 * @q:		the request queue for the device
 * @wc:		write back cache on or off
 * @fua:	device supports FUA writes, if true
 *
 * Tell the block layer about the write cache of @q.
 */
void blk_queue_write_cache(struct request_queue *q, bool wc, bool fua)
{
	if (wc) {
		blk_queue_flag_set(QUEUE_FLAG_HW_WC, q);
		blk_queue_flag_set(QUEUE_FLAG_WC, q);
	} else {
		blk_queue_flag_clear(QUEUE_FLAG_HW_WC, q);
		blk_queue_flag_clear(QUEUE_FLAG_WC, q);
	}
	if (fua)
		blk_queue_flag_set(QUEUE_FLAG_FUA, q);
	else
		blk_queue_flag_clear(QUEUE_FLAG_FUA, q);
}
EXPORT_SYMBOL_GPL(blk_queue_write_cache);

/**
 * blk_queue_required_elevator_features - Set a queue required elevator features
 * @q:		the request queue for the target device
 * @features:	Required elevator features OR'ed together
 *
 * Tell the block layer that for the device controlled through @q, only the
 * only elevators that can be used are those that implement at least the set of
 * features specified by @features.
 */
void blk_queue_required_elevator_features(struct request_queue *q,
					  unsigned int features)
{
	q->required_elevator_features = features;
}
EXPORT_SYMBOL_GPL(blk_queue_required_elevator_features);

/**
 * blk_queue_can_use_dma_map_merging - configure queue for merging segments.
 * @q:		the request queue for the device
 * @dev:	the device pointer for dma
 *
 * Tell the block layer about merging the segments by dma map of @q.
 */
bool blk_queue_can_use_dma_map_merging(struct request_queue *q,
				       struct device *dev)
{
	unsigned long boundary = dma_get_merge_boundary(dev);

	if (!boundary)
		return false;

	/* No need to update max_segment_size. see blk_queue_virt_boundary() */
	blk_queue_virt_boundary(q, boundary);

	return true;
}
EXPORT_SYMBOL_GPL(blk_queue_can_use_dma_map_merging);

/**
 * disk_set_zoned - inidicate a zoned device
 * @disk:	gendisk to configure
 */
void disk_set_zoned(struct gendisk *disk)
{
	struct request_queue *q = disk->queue;

	WARN_ON_ONCE(!IS_ENABLED(CONFIG_BLK_DEV_ZONED));

	/*
	 * Set the zone write granularity to the device logical block
	 * size by default. The driver can change this value if needed.
	 */
	q->limits.zoned = true;
	blk_queue_zone_write_granularity(q, queue_logical_block_size(q));
}
EXPORT_SYMBOL_GPL(disk_set_zoned);

int bdev_alignment_offset(struct block_device *bdev)
{
	struct request_queue *q = bdev_get_queue(bdev);

	if (q->limits.misaligned)
		return -1;
	if (bdev_is_partition(bdev))
		return queue_limit_alignment_offset(&q->limits,
				bdev->bd_start_sect);
	return q->limits.alignment_offset;
}
EXPORT_SYMBOL_GPL(bdev_alignment_offset);

unsigned int bdev_discard_alignment(struct block_device *bdev)
{
	struct request_queue *q = bdev_get_queue(bdev);

	if (bdev_is_partition(bdev))
		return queue_limit_discard_alignment(&q->limits,
				bdev->bd_start_sect);
	return q->limits.discard_alignment;
}
EXPORT_SYMBOL_GPL(bdev_discard_alignment);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
