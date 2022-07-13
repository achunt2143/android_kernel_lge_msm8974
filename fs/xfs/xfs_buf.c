<<<<<<< HEAD
/*
 * Copyright (c) 2000-2006 Silicon Graphics, Inc.
 * All Rights Reserved.
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it would be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write the Free Software Foundation,
 * Inc.,  51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
 */
#include "xfs.h"
#include <linux/stddef.h>
#include <linux/errno.h>
#include <linux/gfp.h>
#include <linux/pagemap.h>
#include <linux/init.h>
#include <linux/vmalloc.h>
#include <linux/bio.h>
#include <linux/sysctl.h>
#include <linux/proc_fs.h>
#include <linux/workqueue.h>
#include <linux/percpu.h>
#include <linux/blkdev.h>
#include <linux/hash.h>
#include <linux/kthread.h>
#include <linux/migrate.h>
#include <linux/backing-dev.h>
#include <linux/freezer.h>

#include "xfs_sb.h"
#include "xfs_inum.h"
#include "xfs_log.h"
#include "xfs_ag.h"
#include "xfs_mount.h"
#include "xfs_trace.h"

static kmem_zone_t *xfs_buf_zone;
STATIC int xfsbufd(void *);

static struct workqueue_struct *xfslogd_workqueue;

#ifdef XFS_BUF_LOCK_TRACKING
# define XB_SET_OWNER(bp)	((bp)->b_last_holder = current->pid)
# define XB_CLEAR_OWNER(bp)	((bp)->b_last_holder = -1)
# define XB_GET_OWNER(bp)	((bp)->b_last_holder)
#else
# define XB_SET_OWNER(bp)	do { } while (0)
# define XB_CLEAR_OWNER(bp)	do { } while (0)
# define XB_GET_OWNER(bp)	do { } while (0)
#endif

#define xb_to_gfp(flags) \
	((((flags) & XBF_READ_AHEAD) ? __GFP_NORETRY : \
	  ((flags) & XBF_DONT_BLOCK) ? GFP_NOFS : GFP_KERNEL) | __GFP_NOWARN)

#define xb_to_km(flags) \
	 (((flags) & XBF_DONT_BLOCK) ? KM_NOFS : KM_SLEEP)

=======
// SPDX-License-Identifier: GPL-2.0
/*
 * Copyright (c) 2000-2006 Silicon Graphics, Inc.
 * All Rights Reserved.
 */
#include "xfs.h"
#include <linux/backing-dev.h>
#include <linux/dax.h>

#include "xfs_shared.h"
#include "xfs_format.h"
#include "xfs_log_format.h"
#include "xfs_trans_resv.h"
#include "xfs_mount.h"
#include "xfs_trace.h"
#include "xfs_log.h"
#include "xfs_log_recover.h"
#include "xfs_log_priv.h"
#include "xfs_trans.h"
#include "xfs_buf_item.h"
#include "xfs_errortag.h"
#include "xfs_error.h"
#include "xfs_ag.h"
#include "xfs_buf_mem.h"

struct kmem_cache *xfs_buf_cache;

/*
 * Locking orders
 *
 * xfs_buf_ioacct_inc:
 * xfs_buf_ioacct_dec:
 *	b_sema (caller holds)
 *	  b_lock
 *
 * xfs_buf_stale:
 *	b_sema (caller holds)
 *	  b_lock
 *	    lru_lock
 *
 * xfs_buf_rele:
 *	b_lock
 *	  pag_buf_lock
 *	    lru_lock
 *
 * xfs_buftarg_drain_rele
 *	lru_lock
 *	  b_lock (trylock due to inversion)
 *
 * xfs_buftarg_isolate
 *	lru_lock
 *	  b_lock (trylock due to inversion)
 */

static int __xfs_buf_submit(struct xfs_buf *bp, bool wait);

static inline int
xfs_buf_submit(
	struct xfs_buf		*bp)
{
	return __xfs_buf_submit(bp, !(bp->b_flags & XBF_ASYNC));
}

static inline bool xfs_buf_is_uncached(struct xfs_buf *bp)
{
	return bp->b_rhash_key == XFS_BUF_DADDR_NULL;
}
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

static inline int
xfs_buf_is_vmapped(
	struct xfs_buf	*bp)
{
	/*
	 * Return true if the buffer is vmapped.
	 *
<<<<<<< HEAD
	 * The XBF_MAPPED flag is set if the buffer should be mapped, but the
	 * code is clever enough to know it doesn't have to map a single page,
	 * so the check has to be both for XBF_MAPPED and bp->b_page_count > 1.
	 */
	return (bp->b_flags & XBF_MAPPED) && bp->b_page_count > 1;
=======
	 * b_addr is null if the buffer is not mapped, but the code is clever
	 * enough to know it doesn't have to map a single page, so the check has
	 * to be both for b_addr and bp->b_page_count > 1.
	 */
	return bp->b_addr && bp->b_page_count > 1;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

static inline int
xfs_buf_vmap_len(
	struct xfs_buf	*bp)
{
<<<<<<< HEAD
	return (bp->b_page_count * PAGE_SIZE) - bp->b_offset;
}

/*
 * xfs_buf_lru_add - add a buffer to the LRU.
 *
 * The LRU takes a new reference to the buffer so that it will only be freed
 * once the shrinker takes the buffer off the LRU.
 */
STATIC void
xfs_buf_lru_add(
	struct xfs_buf	*bp)
{
	struct xfs_buftarg *btp = bp->b_target;

	spin_lock(&btp->bt_lru_lock);
	if (list_empty(&bp->b_lru)) {
		atomic_inc(&bp->b_hold);
		list_add_tail(&bp->b_lru, &btp->bt_lru);
		btp->bt_lru_nr++;
	}
	spin_unlock(&btp->bt_lru_lock);
}

/*
 * xfs_buf_lru_del - remove a buffer from the LRU
 *
 * The unlocked check is safe here because it only occurs when there are not
 * b_lru_ref counts left on the inode under the pag->pag_buf_lock. it is there
 * to optimise the shrinker removing the buffer from the LRU and calling
 * xfs_buf_free(). i.e. it removes an unnecessary round trip on the
 * bt_lru_lock.
 */
STATIC void
xfs_buf_lru_del(
	struct xfs_buf	*bp)
{
	struct xfs_buftarg *btp = bp->b_target;

	if (list_empty(&bp->b_lru))
		return;

	spin_lock(&btp->bt_lru_lock);
	if (!list_empty(&bp->b_lru)) {
		list_del_init(&bp->b_lru);
		btp->bt_lru_nr--;
	}
	spin_unlock(&btp->bt_lru_lock);
=======
	return (bp->b_page_count * PAGE_SIZE);
}

/*
 * Bump the I/O in flight count on the buftarg if we haven't yet done so for
 * this buffer. The count is incremented once per buffer (per hold cycle)
 * because the corresponding decrement is deferred to buffer release. Buffers
 * can undergo I/O multiple times in a hold-release cycle and per buffer I/O
 * tracking adds unnecessary overhead. This is used for sychronization purposes
 * with unmount (see xfs_buftarg_drain()), so all we really need is a count of
 * in-flight buffers.
 *
 * Buffers that are never released (e.g., superblock, iclog buffers) must set
 * the XBF_NO_IOACCT flag before I/O submission. Otherwise, the buftarg count
 * never reaches zero and unmount hangs indefinitely.
 */
static inline void
xfs_buf_ioacct_inc(
	struct xfs_buf	*bp)
{
	if (bp->b_flags & XBF_NO_IOACCT)
		return;

	ASSERT(bp->b_flags & XBF_ASYNC);
	spin_lock(&bp->b_lock);
	if (!(bp->b_state & XFS_BSTATE_IN_FLIGHT)) {
		bp->b_state |= XFS_BSTATE_IN_FLIGHT;
		percpu_counter_inc(&bp->b_target->bt_io_count);
	}
	spin_unlock(&bp->b_lock);
}

/*
 * Clear the in-flight state on a buffer about to be released to the LRU or
 * freed and unaccount from the buftarg.
 */
static inline void
__xfs_buf_ioacct_dec(
	struct xfs_buf	*bp)
{
	lockdep_assert_held(&bp->b_lock);

	if (bp->b_state & XFS_BSTATE_IN_FLIGHT) {
		bp->b_state &= ~XFS_BSTATE_IN_FLIGHT;
		percpu_counter_dec(&bp->b_target->bt_io_count);
	}
}

static inline void
xfs_buf_ioacct_dec(
	struct xfs_buf	*bp)
{
	spin_lock(&bp->b_lock);
	__xfs_buf_ioacct_dec(bp);
	spin_unlock(&bp->b_lock);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

/*
 * When we mark a buffer stale, we remove the buffer from the LRU and clear the
 * b_lru_ref count so that the buffer is freed immediately when the buffer
 * reference count falls to zero. If the buffer is already on the LRU, we need
 * to remove the reference that LRU holds on the buffer.
 *
 * This prevents build-up of stale buffers on the LRU.
 */
void
xfs_buf_stale(
	struct xfs_buf	*bp)
{
<<<<<<< HEAD
	bp->b_flags |= XBF_STALE;
	xfs_buf_delwri_dequeue(bp);
	atomic_set(&(bp)->b_lru_ref, 0);
	if (!list_empty(&bp->b_lru)) {
		struct xfs_buftarg *btp = bp->b_target;

		spin_lock(&btp->bt_lru_lock);
		if (!list_empty(&bp->b_lru)) {
			list_del_init(&bp->b_lru);
			btp->bt_lru_nr--;
			atomic_dec(&bp->b_hold);
		}
		spin_unlock(&btp->bt_lru_lock);
	}
	ASSERT(atomic_read(&bp->b_hold) >= 1);
}

struct xfs_buf *
xfs_buf_alloc(
	struct xfs_buftarg	*target,
	xfs_off_t		range_base,
	size_t			range_length,
	xfs_buf_flags_t		flags)
{
	struct xfs_buf		*bp;

	bp = kmem_zone_alloc(xfs_buf_zone, xb_to_km(flags));
	if (unlikely(!bp))
		return NULL;

	/*
	 * We don't want certain flags to appear in b_flags.
	 */
	flags &= ~(XBF_LOCK|XBF_MAPPED|XBF_DONT_BLOCK|XBF_READ_AHEAD);

	memset(bp, 0, sizeof(xfs_buf_t));
	atomic_set(&bp->b_hold, 1);
	atomic_set(&bp->b_lru_ref, 1);
	init_completion(&bp->b_iowait);
	INIT_LIST_HEAD(&bp->b_lru);
	INIT_LIST_HEAD(&bp->b_list);
	RB_CLEAR_NODE(&bp->b_rbnode);
	sema_init(&bp->b_sema, 0); /* held, no waiters */
	XB_SET_OWNER(bp);
	bp->b_target = target;
	bp->b_file_offset = range_base;
	/*
	 * Set buffer_length and count_desired to the same value initially.
	 * I/O routines should use count_desired, which will be the same in
	 * most cases but may be reset (e.g. XFS recovery).
	 */
	bp->b_buffer_length = bp->b_count_desired = range_length;
	bp->b_flags = flags;
	bp->b_bn = XFS_BUF_DADDR_NULL;
	atomic_set(&bp->b_pin_count, 0);
	init_waitqueue_head(&bp->b_waiters);

	XFS_STATS_INC(xb_create);
	trace_xfs_buf_init(bp, _RET_IP_);

	return bp;
}

/*
 *	Allocate a page array capable of holding a specified number
 *	of pages, and point the page buf at it.
 */
STATIC int
_xfs_buf_get_pages(
	xfs_buf_t		*bp,
	int			page_count,
	xfs_buf_flags_t		flags)
{
	/* Make sure that we have a page list */
	if (bp->b_pages == NULL) {
		bp->b_offset = xfs_buf_poff(bp->b_file_offset);
		bp->b_page_count = page_count;
		if (page_count <= XB_PAGES) {
			bp->b_pages = bp->b_page_array;
		} else {
			bp->b_pages = kmem_alloc(sizeof(struct page *) *
					page_count, xb_to_km(flags));
			if (bp->b_pages == NULL)
				return -ENOMEM;
		}
		memset(bp->b_pages, 0, sizeof(struct page *) * page_count);
	}
=======
	ASSERT(xfs_buf_islocked(bp));

	bp->b_flags |= XBF_STALE;

	/*
	 * Clear the delwri status so that a delwri queue walker will not
	 * flush this buffer to disk now that it is stale. The delwri queue has
	 * a reference to the buffer, so this is safe to do.
	 */
	bp->b_flags &= ~_XBF_DELWRI_Q;

	/*
	 * Once the buffer is marked stale and unlocked, a subsequent lookup
	 * could reset b_flags. There is no guarantee that the buffer is
	 * unaccounted (released to LRU) before that occurs. Drop in-flight
	 * status now to preserve accounting consistency.
	 */
	spin_lock(&bp->b_lock);
	__xfs_buf_ioacct_dec(bp);

	atomic_set(&bp->b_lru_ref, 0);
	if (!(bp->b_state & XFS_BSTATE_DISPOSE) &&
	    (list_lru_del_obj(&bp->b_target->bt_lru, &bp->b_lru)))
		atomic_dec(&bp->b_hold);

	ASSERT(atomic_read(&bp->b_hold) >= 1);
	spin_unlock(&bp->b_lock);
}

static int
xfs_buf_get_maps(
	struct xfs_buf		*bp,
	int			map_count)
{
	ASSERT(bp->b_maps == NULL);
	bp->b_map_count = map_count;

	if (map_count == 1) {
		bp->b_maps = &bp->__b_map;
		return 0;
	}

	bp->b_maps = kzalloc(map_count * sizeof(struct xfs_buf_map),
			GFP_KERNEL | __GFP_NOLOCKDEP | __GFP_NOFAIL);
	if (!bp->b_maps)
		return -ENOMEM;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	return 0;
}

/*
 *	Frees b_pages if it was allocated.
 */
<<<<<<< HEAD
STATIC void
_xfs_buf_free_pages(
	xfs_buf_t	*bp)
{
	if (bp->b_pages != bp->b_page_array) {
		kmem_free(bp->b_pages);
		bp->b_pages = NULL;
	}
}

/*
 *	Releases the specified buffer.
 *
 * 	The modification state of any associated pages is left unchanged.
 * 	The buffer most not be on any hash - use xfs_buf_rele instead for
 * 	hashed and refcounted buffers
 */
void
xfs_buf_free(
	xfs_buf_t		*bp)
=======
static void
xfs_buf_free_maps(
	struct xfs_buf	*bp)
{
	if (bp->b_maps != &bp->__b_map) {
		kfree(bp->b_maps);
		bp->b_maps = NULL;
	}
}

static int
_xfs_buf_alloc(
	struct xfs_buftarg	*target,
	struct xfs_buf_map	*map,
	int			nmaps,
	xfs_buf_flags_t		flags,
	struct xfs_buf		**bpp)
{
	struct xfs_buf		*bp;
	int			error;
	int			i;

	*bpp = NULL;
	bp = kmem_cache_zalloc(xfs_buf_cache,
			GFP_KERNEL | __GFP_NOLOCKDEP | __GFP_NOFAIL);

	/*
	 * We don't want certain flags to appear in b_flags unless they are
	 * specifically set by later operations on the buffer.
	 */
	flags &= ~(XBF_UNMAPPED | XBF_TRYLOCK | XBF_ASYNC | XBF_READ_AHEAD);

	atomic_set(&bp->b_hold, 1);
	atomic_set(&bp->b_lru_ref, 1);
	init_completion(&bp->b_iowait);
	INIT_LIST_HEAD(&bp->b_lru);
	INIT_LIST_HEAD(&bp->b_list);
	INIT_LIST_HEAD(&bp->b_li_list);
	sema_init(&bp->b_sema, 0); /* held, no waiters */
	spin_lock_init(&bp->b_lock);
	bp->b_target = target;
	bp->b_mount = target->bt_mount;
	bp->b_flags = flags;

	/*
	 * Set length and io_length to the same value initially.
	 * I/O routines should use io_length, which will be the same in
	 * most cases but may be reset (e.g. XFS recovery).
	 */
	error = xfs_buf_get_maps(bp, nmaps);
	if (error)  {
		kmem_cache_free(xfs_buf_cache, bp);
		return error;
	}

	bp->b_rhash_key = map[0].bm_bn;
	bp->b_length = 0;
	for (i = 0; i < nmaps; i++) {
		bp->b_maps[i].bm_bn = map[i].bm_bn;
		bp->b_maps[i].bm_len = map[i].bm_len;
		bp->b_length += map[i].bm_len;
	}

	atomic_set(&bp->b_pin_count, 0);
	init_waitqueue_head(&bp->b_waiters);

	XFS_STATS_INC(bp->b_mount, xb_create);
	trace_xfs_buf_init(bp, _RET_IP_);

	*bpp = bp;
	return 0;
}

static void
xfs_buf_free_pages(
	struct xfs_buf	*bp)
{
	uint		i;

	ASSERT(bp->b_flags & _XBF_PAGES);

	if (xfs_buf_is_vmapped(bp))
		vm_unmap_ram(bp->b_addr, bp->b_page_count);

	for (i = 0; i < bp->b_page_count; i++) {
		if (bp->b_pages[i])
			__free_page(bp->b_pages[i]);
	}
	mm_account_reclaimed_pages(bp->b_page_count);

	if (bp->b_pages != bp->b_page_array)
		kfree(bp->b_pages);
	bp->b_pages = NULL;
	bp->b_flags &= ~_XBF_PAGES;
}

static void
xfs_buf_free_callback(
	struct callback_head	*cb)
{
	struct xfs_buf		*bp = container_of(cb, struct xfs_buf, b_rcu);

	xfs_buf_free_maps(bp);
	kmem_cache_free(xfs_buf_cache, bp);
}

static void
xfs_buf_free(
	struct xfs_buf		*bp)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
{
	trace_xfs_buf_free(bp, _RET_IP_);

	ASSERT(list_empty(&bp->b_lru));

<<<<<<< HEAD
	if (bp->b_flags & _XBF_PAGES) {
		uint		i;

		if (xfs_buf_is_vmapped(bp))
			vm_unmap_ram(bp->b_addr - bp->b_offset,
					bp->b_page_count);

		for (i = 0; i < bp->b_page_count; i++) {
			struct page	*page = bp->b_pages[i];

			__free_page(page);
		}
	} else if (bp->b_flags & _XBF_KMEM)
		kmem_free(bp->b_addr);
	_xfs_buf_free_pages(bp);
	kmem_zone_free(xfs_buf_zone, bp);
}

/*
 * Allocates all the pages for buffer in question and builds it's page list.
 */
STATIC int
xfs_buf_allocate_memory(
	xfs_buf_t		*bp,
	uint			flags)
{
	size_t			size = bp->b_count_desired;
	size_t			nbytes, offset;
	gfp_t			gfp_mask = xb_to_gfp(flags);
	unsigned short		page_count, i;
	xfs_off_t		end;
	int			error;

	/*
	 * for buffers that are contained within a single page, just allocate
	 * the memory from the heap - there's no need for the complexity of
	 * page arrays to keep allocation down to order 0.
	 */
	if (bp->b_buffer_length < PAGE_SIZE) {
		bp->b_addr = kmem_alloc(bp->b_buffer_length, xb_to_km(flags));
		if (!bp->b_addr) {
			/* low memory - use alloc_page loop instead */
			goto use_alloc_page;
		}

		if (((unsigned long)(bp->b_addr + bp->b_buffer_length - 1) &
								PAGE_MASK) !=
		    ((unsigned long)bp->b_addr & PAGE_MASK)) {
			/* b_addr spans two pages - use alloc_page instead */
			kmem_free(bp->b_addr);
			bp->b_addr = NULL;
			goto use_alloc_page;
		}
		bp->b_offset = offset_in_page(bp->b_addr);
		bp->b_pages = bp->b_page_array;
		bp->b_pages[0] = virt_to_page(bp->b_addr);
		bp->b_page_count = 1;
		bp->b_flags |= XBF_MAPPED | _XBF_KMEM;
		return 0;
	}

use_alloc_page:
	end = bp->b_file_offset + bp->b_buffer_length;
	page_count = xfs_buf_btoc(end) - xfs_buf_btoct(bp->b_file_offset);
	error = _xfs_buf_get_pages(bp, page_count, flags);
	if (unlikely(error))
		return error;

	offset = bp->b_offset;
	bp->b_flags |= _XBF_PAGES;

	for (i = 0; i < bp->b_page_count; i++) {
		struct page	*page;
		uint		retries = 0;
retry:
		page = alloc_page(gfp_mask);
		if (unlikely(page == NULL)) {
			if (flags & XBF_READ_AHEAD) {
				bp->b_page_count = i;
				error = ENOMEM;
				goto out_free_pages;
			}

			/*
			 * This could deadlock.
			 *
			 * But until all the XFS lowlevel code is revamped to
			 * handle buffer allocation failures we can't do much.
			 */
			if (!(++retries % 100))
				xfs_err(NULL,
		"possible memory allocation deadlock in %s (mode:0x%x)",
					__func__, gfp_mask);

			XFS_STATS_INC(xb_page_retries);
			congestion_wait(BLK_RW_ASYNC, HZ/50);
			goto retry;
		}

		XFS_STATS_INC(xb_page_found);

		nbytes = min_t(size_t, size, PAGE_SIZE - offset);
		size -= nbytes;
		bp->b_pages[i] = page;
		offset = 0;
	}
	return 0;

out_free_pages:
	for (i = 0; i < bp->b_page_count; i++)
		__free_page(bp->b_pages[i]);
	return error;
=======
	if (xfs_buftarg_is_mem(bp->b_target))
		xmbuf_unmap_page(bp);
	else if (bp->b_flags & _XBF_PAGES)
		xfs_buf_free_pages(bp);
	else if (bp->b_flags & _XBF_KMEM)
		kfree(bp->b_addr);

	call_rcu(&bp->b_rcu, xfs_buf_free_callback);
}

static int
xfs_buf_alloc_kmem(
	struct xfs_buf	*bp,
	xfs_buf_flags_t	flags)
{
	gfp_t		gfp_mask = GFP_KERNEL | __GFP_NOLOCKDEP | __GFP_NOFAIL;
	size_t		size = BBTOB(bp->b_length);

	/* Assure zeroed buffer for non-read cases. */
	if (!(flags & XBF_READ))
		gfp_mask |= __GFP_ZERO;

	bp->b_addr = kmalloc(size, gfp_mask);
	if (!bp->b_addr)
		return -ENOMEM;

	if (((unsigned long)(bp->b_addr + size - 1) & PAGE_MASK) !=
	    ((unsigned long)bp->b_addr & PAGE_MASK)) {
		/* b_addr spans two pages - use alloc_page instead */
		kfree(bp->b_addr);
		bp->b_addr = NULL;
		return -ENOMEM;
	}
	bp->b_offset = offset_in_page(bp->b_addr);
	bp->b_pages = bp->b_page_array;
	bp->b_pages[0] = kmem_to_page(bp->b_addr);
	bp->b_page_count = 1;
	bp->b_flags |= _XBF_KMEM;
	return 0;
}

static int
xfs_buf_alloc_pages(
	struct xfs_buf	*bp,
	xfs_buf_flags_t	flags)
{
	gfp_t		gfp_mask = GFP_KERNEL | __GFP_NOLOCKDEP | __GFP_NOWARN;
	long		filled = 0;

	if (flags & XBF_READ_AHEAD)
		gfp_mask |= __GFP_NORETRY;

	/* Make sure that we have a page list */
	bp->b_page_count = DIV_ROUND_UP(BBTOB(bp->b_length), PAGE_SIZE);
	if (bp->b_page_count <= XB_PAGES) {
		bp->b_pages = bp->b_page_array;
	} else {
		bp->b_pages = kzalloc(sizeof(struct page *) * bp->b_page_count,
					gfp_mask);
		if (!bp->b_pages)
			return -ENOMEM;
	}
	bp->b_flags |= _XBF_PAGES;

	/* Assure zeroed buffer for non-read cases. */
	if (!(flags & XBF_READ))
		gfp_mask |= __GFP_ZERO;

	/*
	 * Bulk filling of pages can take multiple calls. Not filling the entire
	 * array is not an allocation failure, so don't back off if we get at
	 * least one extra page.
	 */
	for (;;) {
		long	last = filled;

		filled = alloc_pages_bulk_array(gfp_mask, bp->b_page_count,
						bp->b_pages);
		if (filled == bp->b_page_count) {
			XFS_STATS_INC(bp->b_mount, xb_page_found);
			break;
		}

		if (filled != last)
			continue;

		if (flags & XBF_READ_AHEAD) {
			xfs_buf_free_pages(bp);
			return -ENOMEM;
		}

		XFS_STATS_INC(bp->b_mount, xb_page_retries);
		memalloc_retry_wait(gfp_mask);
	}
	return 0;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

/*
 *	Map buffer into kernel address-space if necessary.
 */
STATIC int
_xfs_buf_map_pages(
<<<<<<< HEAD
	xfs_buf_t		*bp,
	uint			flags)
=======
	struct xfs_buf		*bp,
	xfs_buf_flags_t		flags)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
{
	ASSERT(bp->b_flags & _XBF_PAGES);
	if (bp->b_page_count == 1) {
		/* A single page buffer is always mappable */
<<<<<<< HEAD
		bp->b_addr = page_address(bp->b_pages[0]) + bp->b_offset;
		bp->b_flags |= XBF_MAPPED;
	} else if (flags & XBF_MAPPED) {
		int retried = 0;

		do {
			bp->b_addr = vm_map_ram(bp->b_pages, bp->b_page_count,
						-1, PAGE_KERNEL);
=======
		bp->b_addr = page_address(bp->b_pages[0]);
	} else if (flags & XBF_UNMAPPED) {
		bp->b_addr = NULL;
	} else {
		int retried = 0;
		unsigned nofs_flag;

		/*
		 * vm_map_ram() will allocate auxiliary structures (e.g.
		 * pagetables) with GFP_KERNEL, yet we often under a scoped nofs
		 * context here. Mixing GFP_KERNEL with GFP_NOFS allocations
		 * from the same call site that can be run from both above and
		 * below memory reclaim causes lockdep false positives. Hence we
		 * always need to force this allocation to nofs context because
		 * we can't pass __GFP_NOLOCKDEP down to auxillary structures to
		 * prevent false positive lockdep reports.
		 *
		 * XXX(dgc): I think dquot reclaim is the only place we can get
		 * to this function from memory reclaim context now. If we fix
		 * that like we've fixed inode reclaim to avoid writeback from
		 * reclaim, this nofs wrapping can go away.
		 */
		nofs_flag = memalloc_nofs_save();
		do {
			bp->b_addr = vm_map_ram(bp->b_pages, bp->b_page_count,
						-1);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
			if (bp->b_addr)
				break;
			vm_unmap_aliases();
		} while (retried++ <= 1);
<<<<<<< HEAD

		if (!bp->b_addr)
			return -ENOMEM;
		bp->b_addr += bp->b_offset;
		bp->b_flags |= XBF_MAPPED;
=======
		memalloc_nofs_restore(nofs_flag);

		if (!bp->b_addr)
			return -ENOMEM;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	}

	return 0;
}

/*
 *	Finding and Reading Buffers
 */
<<<<<<< HEAD

/*
 *	Look up, and creates if absent, a lockable buffer for
 *	a given range of an inode.  The buffer is returned
 *	locked.	No I/O is implied by this call.
 */
xfs_buf_t *
_xfs_buf_find(
	xfs_buftarg_t		*btp,	/* block device target		*/
	xfs_off_t		ioff,	/* starting offset of range	*/
	size_t			isize,	/* length of range		*/
	xfs_buf_flags_t		flags,
	xfs_buf_t		*new_bp)
{
	xfs_off_t		range_base;
	size_t			range_length;
	struct xfs_perag	*pag;
	struct rb_node		**rbp;
	struct rb_node		*parent;
	xfs_buf_t		*bp;

	range_base = (ioff << BBSHIFT);
	range_length = (isize << BBSHIFT);

	/* Check for IOs smaller than the sector size / not sector aligned */
	ASSERT(!(range_length < (1 << btp->bt_sshift)));
	ASSERT(!(range_base & (xfs_off_t)btp->bt_smask));

	/* get tree root */
	pag = xfs_perag_get(btp->bt_mount,
				xfs_daddr_to_agno(btp->bt_mount, ioff));

	/* walk tree */
	spin_lock(&pag->pag_buf_lock);
	rbp = &pag->pag_buf_tree.rb_node;
	parent = NULL;
	bp = NULL;
	while (*rbp) {
		parent = *rbp;
		bp = rb_entry(parent, struct xfs_buf, b_rbnode);

		if (range_base < bp->b_file_offset)
			rbp = &(*rbp)->rb_left;
		else if (range_base > bp->b_file_offset)
			rbp = &(*rbp)->rb_right;
		else {
			/*
			 * found a block offset match. If the range doesn't
			 * match, the only way this is allowed is if the buffer
			 * in the cache is stale and the transaction that made
			 * it stale has not yet committed. i.e. we are
			 * reallocating a busy extent. Skip this buffer and
			 * continue searching to the right for an exact match.
			 */
			if (bp->b_buffer_length != range_length) {
				ASSERT(bp->b_flags & XBF_STALE);
				rbp = &(*rbp)->rb_right;
				continue;
			}
			atomic_inc(&bp->b_hold);
			goto found;
		}
	}

	/* No match found */
	if (new_bp) {
		rb_link_node(&new_bp->b_rbnode, parent, rbp);
		rb_insert_color(&new_bp->b_rbnode, &pag->pag_buf_tree);
		/* the buffer keeps the perag reference until it is freed */
		new_bp->b_pag = pag;
		spin_unlock(&pag->pag_buf_lock);
	} else {
		XFS_STATS_INC(xb_miss_locked);
		spin_unlock(&pag->pag_buf_lock);
		xfs_perag_put(pag);
	}
	return new_bp;

found:
	spin_unlock(&pag->pag_buf_lock);
	xfs_perag_put(pag);

	if (!xfs_buf_trylock(bp)) {
		if (flags & XBF_TRYLOCK) {
			xfs_buf_rele(bp);
			XFS_STATS_INC(xb_busy_locked);
			return NULL;
		}
		xfs_buf_lock(bp);
		XFS_STATS_INC(xb_get_locked_waited);
=======
static int
_xfs_buf_obj_cmp(
	struct rhashtable_compare_arg	*arg,
	const void			*obj)
{
	const struct xfs_buf_map	*map = arg->key;
	const struct xfs_buf		*bp = obj;

	/*
	 * The key hashing in the lookup path depends on the key being the
	 * first element of the compare_arg, make sure to assert this.
	 */
	BUILD_BUG_ON(offsetof(struct xfs_buf_map, bm_bn) != 0);

	if (bp->b_rhash_key != map->bm_bn)
		return 1;

	if (unlikely(bp->b_length != map->bm_len)) {
		/*
		 * found a block number match. If the range doesn't
		 * match, the only way this is allowed is if the buffer
		 * in the cache is stale and the transaction that made
		 * it stale has not yet committed. i.e. we are
		 * reallocating a busy extent. Skip this buffer and
		 * continue searching for an exact match.
		 */
		if (!(map->bm_flags & XBM_LIVESCAN))
			ASSERT(bp->b_flags & XBF_STALE);
		return 1;
	}
	return 0;
}

static const struct rhashtable_params xfs_buf_hash_params = {
	.min_size		= 32,	/* empty AGs have minimal footprint */
	.nelem_hint		= 16,
	.key_len		= sizeof(xfs_daddr_t),
	.key_offset		= offsetof(struct xfs_buf, b_rhash_key),
	.head_offset		= offsetof(struct xfs_buf, b_rhash_head),
	.automatic_shrinking	= true,
	.obj_cmpfn		= _xfs_buf_obj_cmp,
};

int
xfs_buf_cache_init(
	struct xfs_buf_cache	*bch)
{
	spin_lock_init(&bch->bc_lock);
	return rhashtable_init(&bch->bc_hash, &xfs_buf_hash_params);
}

void
xfs_buf_cache_destroy(
	struct xfs_buf_cache	*bch)
{
	rhashtable_destroy(&bch->bc_hash);
}

static int
xfs_buf_map_verify(
	struct xfs_buftarg	*btp,
	struct xfs_buf_map	*map)
{
	xfs_daddr_t		eofs;

	/* Check for IOs smaller than the sector size / not sector aligned */
	ASSERT(!(BBTOB(map->bm_len) < btp->bt_meta_sectorsize));
	ASSERT(!(BBTOB(map->bm_bn) & (xfs_off_t)btp->bt_meta_sectormask));

	/*
	 * Corrupted block numbers can get through to here, unfortunately, so we
	 * have to check that the buffer falls within the filesystem bounds.
	 */
	eofs = XFS_FSB_TO_BB(btp->bt_mount, btp->bt_mount->m_sb.sb_dblocks);
	if (map->bm_bn < 0 || map->bm_bn >= eofs) {
		xfs_alert(btp->bt_mount,
			  "%s: daddr 0x%llx out of range, EOFS 0x%llx",
			  __func__, map->bm_bn, eofs);
		WARN_ON(1);
		return -EFSCORRUPTED;
	}
	return 0;
}

static int
xfs_buf_find_lock(
	struct xfs_buf          *bp,
	xfs_buf_flags_t		flags)
{
	if (flags & XBF_TRYLOCK) {
		if (!xfs_buf_trylock(bp)) {
			XFS_STATS_INC(bp->b_mount, xb_busy_locked);
			return -EAGAIN;
		}
	} else {
		xfs_buf_lock(bp);
		XFS_STATS_INC(bp->b_mount, xb_get_locked_waited);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	}

	/*
	 * if the buffer is stale, clear all the external state associated with
	 * it. We need to keep flags such as how we allocated the buffer memory
	 * intact here.
	 */
	if (bp->b_flags & XBF_STALE) {
<<<<<<< HEAD
		ASSERT((bp->b_flags & _XBF_DELWRI_Q) == 0);
		bp->b_flags &= XBF_MAPPED | _XBF_KMEM | _XBF_PAGES;
	}

	trace_xfs_buf_find(bp, flags, _RET_IP_);
	XFS_STATS_INC(xb_get_locked);
	return bp;
=======
		if (flags & XBF_LIVESCAN) {
			xfs_buf_unlock(bp);
			return -ENOENT;
		}
		ASSERT((bp->b_flags & _XBF_DELWRI_Q) == 0);
		bp->b_flags &= _XBF_KMEM | _XBF_PAGES;
		bp->b_ops = NULL;
	}
	return 0;
}

static inline int
xfs_buf_lookup(
	struct xfs_buf_cache	*bch,
	struct xfs_buf_map	*map,
	xfs_buf_flags_t		flags,
	struct xfs_buf		**bpp)
{
	struct xfs_buf          *bp;
	int			error;

	rcu_read_lock();
	bp = rhashtable_lookup(&bch->bc_hash, map, xfs_buf_hash_params);
	if (!bp || !atomic_inc_not_zero(&bp->b_hold)) {
		rcu_read_unlock();
		return -ENOENT;
	}
	rcu_read_unlock();

	error = xfs_buf_find_lock(bp, flags);
	if (error) {
		xfs_buf_rele(bp);
		return error;
	}

	trace_xfs_buf_find(bp, flags, _RET_IP_);
	*bpp = bp;
	return 0;
}

/*
 * Insert the new_bp into the hash table. This consumes the perag reference
 * taken for the lookup regardless of the result of the insert.
 */
static int
xfs_buf_find_insert(
	struct xfs_buftarg	*btp,
	struct xfs_buf_cache	*bch,
	struct xfs_perag	*pag,
	struct xfs_buf_map	*cmap,
	struct xfs_buf_map	*map,
	int			nmaps,
	xfs_buf_flags_t		flags,
	struct xfs_buf		**bpp)
{
	struct xfs_buf		*new_bp;
	struct xfs_buf		*bp;
	int			error;

	error = _xfs_buf_alloc(btp, map, nmaps, flags, &new_bp);
	if (error)
		goto out_drop_pag;

	if (xfs_buftarg_is_mem(new_bp->b_target)) {
		error = xmbuf_map_page(new_bp);
	} else if (BBTOB(new_bp->b_length) >= PAGE_SIZE ||
		   xfs_buf_alloc_kmem(new_bp, flags) < 0) {
		/*
		 * For buffers that fit entirely within a single page, first
		 * attempt to allocate the memory from the heap to minimise
		 * memory usage. If we can't get heap memory for these small
		 * buffers, we fall back to using the page allocator.
		 */
		error = xfs_buf_alloc_pages(new_bp, flags);
	}
	if (error)
		goto out_free_buf;

	spin_lock(&bch->bc_lock);
	bp = rhashtable_lookup_get_insert_fast(&bch->bc_hash,
			&new_bp->b_rhash_head, xfs_buf_hash_params);
	if (IS_ERR(bp)) {
		error = PTR_ERR(bp);
		spin_unlock(&bch->bc_lock);
		goto out_free_buf;
	}
	if (bp) {
		/* found an existing buffer */
		atomic_inc(&bp->b_hold);
		spin_unlock(&bch->bc_lock);
		error = xfs_buf_find_lock(bp, flags);
		if (error)
			xfs_buf_rele(bp);
		else
			*bpp = bp;
		goto out_free_buf;
	}

	/* The new buffer keeps the perag reference until it is freed. */
	new_bp->b_pag = pag;
	spin_unlock(&bch->bc_lock);
	*bpp = new_bp;
	return 0;

out_free_buf:
	xfs_buf_free(new_bp);
out_drop_pag:
	if (pag)
		xfs_perag_put(pag);
	return error;
}

static inline struct xfs_perag *
xfs_buftarg_get_pag(
	struct xfs_buftarg		*btp,
	const struct xfs_buf_map	*map)
{
	struct xfs_mount		*mp = btp->bt_mount;

	if (xfs_buftarg_is_mem(btp))
		return NULL;
	return xfs_perag_get(mp, xfs_daddr_to_agno(mp, map->bm_bn));
}

static inline struct xfs_buf_cache *
xfs_buftarg_buf_cache(
	struct xfs_buftarg		*btp,
	struct xfs_perag		*pag)
{
	if (pag)
		return &pag->pag_bcache;
	return btp->bt_cache;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

/*
 * Assembles a buffer covering the specified range. The code is optimised for
 * cache hits, as metadata intensive workloads will see 3 orders of magnitude
 * more hits than misses.
 */
<<<<<<< HEAD
struct xfs_buf *
xfs_buf_get(
	xfs_buftarg_t		*target,/* target for buffer		*/
	xfs_off_t		ioff,	/* starting offset of range	*/
	size_t			isize,	/* length of range		*/
	xfs_buf_flags_t		flags)
{
	struct xfs_buf		*bp;
	struct xfs_buf		*new_bp;
	int			error = 0;

	bp = _xfs_buf_find(target, ioff, isize, flags, NULL);
	if (likely(bp))
		goto found;

	new_bp = xfs_buf_alloc(target, ioff << BBSHIFT, isize << BBSHIFT,
			       flags);
	if (unlikely(!new_bp))
		return NULL;

	bp = _xfs_buf_find(target, ioff, isize, flags, new_bp);
	if (!bp) {
		kmem_zone_free(xfs_buf_zone, new_bp);
		return NULL;
	}

	if (bp == new_bp) {
		error = xfs_buf_allocate_memory(bp, flags);
		if (error)
			goto no_buffer;
	} else
		kmem_zone_free(xfs_buf_zone, new_bp);

	/*
	 * Now we have a workable buffer, fill in the block number so
	 * that we can do IO on it.
	 */
	bp->b_bn = ioff;
	bp->b_count_desired = bp->b_buffer_length;

found:
	if (!(bp->b_flags & XBF_MAPPED)) {
		error = _xfs_buf_map_pages(bp, flags);
		if (unlikely(error)) {
			xfs_warn(target->bt_mount,
				"%s: failed to map pages\n", __func__);
			goto no_buffer;
		}
	}

	XFS_STATS_INC(xb_get);
	trace_xfs_buf_get(bp, flags, _RET_IP_);
	return bp;

no_buffer:
	if (flags & (XBF_LOCK | XBF_TRYLOCK))
		xfs_buf_unlock(bp);
	xfs_buf_rele(bp);
	return NULL;
}

STATIC int
_xfs_buf_read(
	xfs_buf_t		*bp,
	xfs_buf_flags_t		flags)
{
	int			status;

	ASSERT(!(flags & (XBF_DELWRI|XBF_WRITE)));
	ASSERT(bp->b_bn != XFS_BUF_DADDR_NULL);

	bp->b_flags &= ~(XBF_WRITE | XBF_ASYNC | XBF_DELWRI | XBF_READ_AHEAD);
	bp->b_flags |= flags & (XBF_READ | XBF_ASYNC | XBF_READ_AHEAD);

	status = xfs_buf_iorequest(bp);
	if (status || bp->b_error || (flags & XBF_ASYNC))
		return status;
	return xfs_buf_iowait(bp);
}

xfs_buf_t *
xfs_buf_read(
	xfs_buftarg_t		*target,
	xfs_off_t		ioff,
	size_t			isize,
	xfs_buf_flags_t		flags)
{
	xfs_buf_t		*bp;

	flags |= XBF_READ;

	bp = xfs_buf_get(target, ioff, isize, flags);
	if (bp) {
		trace_xfs_buf_read(bp, flags, _RET_IP_);

		if (!XFS_BUF_ISDONE(bp)) {
			XFS_STATS_INC(xb_get_read);
			_xfs_buf_read(bp, flags);
		} else if (flags & XBF_ASYNC) {
			/*
			 * Read ahead call which is already satisfied,
			 * drop the buffer
			 */
			goto no_buffer;
		} else {
			/* We do not want read in the flags */
			bp->b_flags &= ~XBF_READ;
		}
	}

	return bp;

 no_buffer:
	if (flags & (XBF_LOCK | XBF_TRYLOCK))
		xfs_buf_unlock(bp);
	xfs_buf_rele(bp);
	return NULL;
=======
int
xfs_buf_get_map(
	struct xfs_buftarg	*btp,
	struct xfs_buf_map	*map,
	int			nmaps,
	xfs_buf_flags_t		flags,
	struct xfs_buf		**bpp)
{
	struct xfs_buf_cache	*bch;
	struct xfs_perag	*pag;
	struct xfs_buf		*bp = NULL;
	struct xfs_buf_map	cmap = { .bm_bn = map[0].bm_bn };
	int			error;
	int			i;

	if (flags & XBF_LIVESCAN)
		cmap.bm_flags |= XBM_LIVESCAN;
	for (i = 0; i < nmaps; i++)
		cmap.bm_len += map[i].bm_len;

	error = xfs_buf_map_verify(btp, &cmap);
	if (error)
		return error;

	pag = xfs_buftarg_get_pag(btp, &cmap);
	bch = xfs_buftarg_buf_cache(btp, pag);

	error = xfs_buf_lookup(bch, &cmap, flags, &bp);
	if (error && error != -ENOENT)
		goto out_put_perag;

	/* cache hits always outnumber misses by at least 10:1 */
	if (unlikely(!bp)) {
		XFS_STATS_INC(btp->bt_mount, xb_miss_locked);

		if (flags & XBF_INCORE)
			goto out_put_perag;

		/* xfs_buf_find_insert() consumes the perag reference. */
		error = xfs_buf_find_insert(btp, bch, pag, &cmap, map, nmaps,
				flags, &bp);
		if (error)
			return error;
	} else {
		XFS_STATS_INC(btp->bt_mount, xb_get_locked);
		if (pag)
			xfs_perag_put(pag);
	}

	/* We do not hold a perag reference anymore. */
	if (!bp->b_addr) {
		error = _xfs_buf_map_pages(bp, flags);
		if (unlikely(error)) {
			xfs_warn_ratelimited(btp->bt_mount,
				"%s: failed to map %u pages", __func__,
				bp->b_page_count);
			xfs_buf_relse(bp);
			return error;
		}
	}

	/*
	 * Clear b_error if this is a lookup from a caller that doesn't expect
	 * valid data to be found in the buffer.
	 */
	if (!(flags & XBF_READ))
		xfs_buf_ioerror(bp, 0);

	XFS_STATS_INC(btp->bt_mount, xb_get);
	trace_xfs_buf_get(bp, flags, _RET_IP_);
	*bpp = bp;
	return 0;

out_put_perag:
	if (pag)
		xfs_perag_put(pag);
	return error;
}

int
_xfs_buf_read(
	struct xfs_buf		*bp,
	xfs_buf_flags_t		flags)
{
	ASSERT(!(flags & XBF_WRITE));
	ASSERT(bp->b_maps[0].bm_bn != XFS_BUF_DADDR_NULL);

	bp->b_flags &= ~(XBF_WRITE | XBF_ASYNC | XBF_READ_AHEAD | XBF_DONE);
	bp->b_flags |= flags & (XBF_READ | XBF_ASYNC | XBF_READ_AHEAD);

	return xfs_buf_submit(bp);
}

/*
 * Reverify a buffer found in cache without an attached ->b_ops.
 *
 * If the caller passed an ops structure and the buffer doesn't have ops
 * assigned, set the ops and use it to verify the contents. If verification
 * fails, clear XBF_DONE. We assume the buffer has no recorded errors and is
 * already in XBF_DONE state on entry.
 *
 * Under normal operations, every in-core buffer is verified on read I/O
 * completion. There are two scenarios that can lead to in-core buffers without
 * an assigned ->b_ops. The first is during log recovery of buffers on a V4
 * filesystem, though these buffers are purged at the end of recovery. The
 * other is online repair, which intentionally reads with a NULL buffer ops to
 * run several verifiers across an in-core buffer in order to establish buffer
 * type.  If repair can't establish that, the buffer will be left in memory
 * with NULL buffer ops.
 */
int
xfs_buf_reverify(
	struct xfs_buf		*bp,
	const struct xfs_buf_ops *ops)
{
	ASSERT(bp->b_flags & XBF_DONE);
	ASSERT(bp->b_error == 0);

	if (!ops || bp->b_ops)
		return 0;

	bp->b_ops = ops;
	bp->b_ops->verify_read(bp);
	if (bp->b_error)
		bp->b_flags &= ~XBF_DONE;
	return bp->b_error;
}

int
xfs_buf_read_map(
	struct xfs_buftarg	*target,
	struct xfs_buf_map	*map,
	int			nmaps,
	xfs_buf_flags_t		flags,
	struct xfs_buf		**bpp,
	const struct xfs_buf_ops *ops,
	xfs_failaddr_t		fa)
{
	struct xfs_buf		*bp;
	int			error;

	flags |= XBF_READ;
	*bpp = NULL;

	error = xfs_buf_get_map(target, map, nmaps, flags, &bp);
	if (error)
		return error;

	trace_xfs_buf_read(bp, flags, _RET_IP_);

	if (!(bp->b_flags & XBF_DONE)) {
		/* Initiate the buffer read and wait. */
		XFS_STATS_INC(target->bt_mount, xb_get_read);
		bp->b_ops = ops;
		error = _xfs_buf_read(bp, flags);

		/* Readahead iodone already dropped the buffer, so exit. */
		if (flags & XBF_ASYNC)
			return 0;
	} else {
		/* Buffer already read; all we need to do is check it. */
		error = xfs_buf_reverify(bp, ops);

		/* Readahead already finished; drop the buffer and exit. */
		if (flags & XBF_ASYNC) {
			xfs_buf_relse(bp);
			return 0;
		}

		/* We do not want read in the flags */
		bp->b_flags &= ~XBF_READ;
		ASSERT(bp->b_ops != NULL || ops == NULL);
	}

	/*
	 * If we've had a read error, then the contents of the buffer are
	 * invalid and should not be used. To ensure that a followup read tries
	 * to pull the buffer from disk again, we clear the XBF_DONE flag and
	 * mark the buffer stale. This ensures that anyone who has a current
	 * reference to the buffer will interpret it's contents correctly and
	 * future cache lookups will also treat it as an empty, uninitialised
	 * buffer.
	 */
	if (error) {
		/*
		 * Check against log shutdown for error reporting because
		 * metadata writeback may require a read first and we need to
		 * report errors in metadata writeback until the log is shut
		 * down. High level transaction read functions already check
		 * against mount shutdown, anyway, so we only need to be
		 * concerned about low level IO interactions here.
		 */
		if (!xlog_is_shutdown(target->bt_mount->m_log))
			xfs_buf_ioerror_alert(bp, fa);

		bp->b_flags &= ~XBF_DONE;
		xfs_buf_stale(bp);
		xfs_buf_relse(bp);

		/* bad CRC means corrupted metadata */
		if (error == -EFSBADCRC)
			error = -EFSCORRUPTED;
		return error;
	}

	*bpp = bp;
	return 0;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

/*
 *	If we are not low on memory then do the readahead in a deadlock
 *	safe manner.
 */
void
<<<<<<< HEAD
xfs_buf_readahead(
	xfs_buftarg_t		*target,
	xfs_off_t		ioff,
	size_t			isize)
{
	if (bdi_read_congested(target->bt_bdi))
		return;

	xfs_buf_read(target, ioff, isize,
		     XBF_TRYLOCK|XBF_ASYNC|XBF_READ_AHEAD|XBF_DONT_BLOCK);
=======
xfs_buf_readahead_map(
	struct xfs_buftarg	*target,
	struct xfs_buf_map	*map,
	int			nmaps,
	const struct xfs_buf_ops *ops)
{
	struct xfs_buf		*bp;

	/*
	 * Currently we don't have a good means or justification for performing
	 * xmbuf_map_page asynchronously, so we don't do readahead.
	 */
	if (xfs_buftarg_is_mem(target))
		return;

	xfs_buf_read_map(target, map, nmaps,
		     XBF_TRYLOCK | XBF_ASYNC | XBF_READ_AHEAD, &bp, ops,
		     __this_address);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

/*
 * Read an uncached buffer from disk. Allocates and returns a locked
<<<<<<< HEAD
 * buffer containing the disk contents or nothing.
 */
struct xfs_buf *
xfs_buf_read_uncached(
	struct xfs_mount	*mp,
	struct xfs_buftarg	*target,
	xfs_daddr_t		daddr,
	size_t			length,
	int			flags)
{
	xfs_buf_t		*bp;
	int			error;

	bp = xfs_buf_get_uncached(target, length, flags);
	if (!bp)
		return NULL;

	/* set up the buffer for a read IO */
	XFS_BUF_SET_ADDR(bp, daddr);
	XFS_BUF_READ(bp);

	xfsbdstrat(mp, bp);
	error = xfs_buf_iowait(bp);
	if (error || bp->b_error) {
		xfs_buf_relse(bp);
		return NULL;
	}
	return bp;
}

/*
 * Return a buffer allocated as an empty buffer and associated to external
 * memory via xfs_buf_associate_memory() back to it's empty state.
 */
void
xfs_buf_set_empty(
	struct xfs_buf		*bp,
	size_t			len)
{
	if (bp->b_pages)
		_xfs_buf_free_pages(bp);

	bp->b_pages = NULL;
	bp->b_page_count = 0;
	bp->b_addr = NULL;
	bp->b_file_offset = 0;
	bp->b_buffer_length = bp->b_count_desired = len;
	bp->b_bn = XFS_BUF_DADDR_NULL;
	bp->b_flags &= ~XBF_MAPPED;
}

static inline struct page *
mem_to_page(
	void			*addr)
{
	if ((!is_vmalloc_addr(addr))) {
		return virt_to_page(addr);
	} else {
		return vmalloc_to_page(addr);
	}
}

int
xfs_buf_associate_memory(
	xfs_buf_t		*bp,
	void			*mem,
	size_t			len)
{
	int			rval;
	int			i = 0;
	unsigned long		pageaddr;
	unsigned long		offset;
	size_t			buflen;
	int			page_count;

	pageaddr = (unsigned long)mem & PAGE_MASK;
	offset = (unsigned long)mem - pageaddr;
	buflen = PAGE_ALIGN(len + offset);
	page_count = buflen >> PAGE_SHIFT;

	/* Free any previous set of page pointers */
	if (bp->b_pages)
		_xfs_buf_free_pages(bp);

	bp->b_pages = NULL;
	bp->b_addr = mem;

	rval = _xfs_buf_get_pages(bp, page_count, XBF_DONT_BLOCK);
	if (rval)
		return rval;

	bp->b_offset = offset;

	for (i = 0; i < bp->b_page_count; i++) {
		bp->b_pages[i] = mem_to_page((void *)pageaddr);
		pageaddr += PAGE_SIZE;
	}

	bp->b_count_desired = len;
	bp->b_buffer_length = buflen;
	bp->b_flags |= XBF_MAPPED;

	return 0;
}

xfs_buf_t *
xfs_buf_get_uncached(
	struct xfs_buftarg	*target,
	size_t			len,
	int			flags)
{
	unsigned long		page_count = PAGE_ALIGN(len) >> PAGE_SHIFT;
	int			error, i;
	xfs_buf_t		*bp;

	bp = xfs_buf_alloc(target, 0, len, 0);
	if (unlikely(bp == NULL))
		goto fail;

	error = _xfs_buf_get_pages(bp, page_count, 0);
	if (error)
		goto fail_free_buf;

	for (i = 0; i < page_count; i++) {
		bp->b_pages[i] = alloc_page(xb_to_gfp(flags));
		if (!bp->b_pages[i])
			goto fail_free_mem;
	}
	bp->b_flags |= _XBF_PAGES;

	error = _xfs_buf_map_pages(bp, XBF_MAPPED);
	if (unlikely(error)) {
		xfs_warn(target->bt_mount,
			"%s: failed to map pages\n", __func__);
		goto fail_free_mem;
	}

	trace_xfs_buf_get_uncached(bp, _RET_IP_);
	return bp;

 fail_free_mem:
	while (--i >= 0)
		__free_page(bp->b_pages[i]);
	_xfs_buf_free_pages(bp);
 fail_free_buf:
	kmem_zone_free(xfs_buf_zone, bp);
 fail:
	return NULL;
=======
 * buffer containing the disk contents or nothing. Uncached buffers always have
 * a cache index of XFS_BUF_DADDR_NULL so we can easily determine if the buffer
 * is cached or uncached during fault diagnosis.
 */
int
xfs_buf_read_uncached(
	struct xfs_buftarg	*target,
	xfs_daddr_t		daddr,
	size_t			numblks,
	xfs_buf_flags_t		flags,
	struct xfs_buf		**bpp,
	const struct xfs_buf_ops *ops)
{
	struct xfs_buf		*bp;
	int			error;

	*bpp = NULL;

	error = xfs_buf_get_uncached(target, numblks, flags, &bp);
	if (error)
		return error;

	/* set up the buffer for a read IO */
	ASSERT(bp->b_map_count == 1);
	bp->b_rhash_key = XFS_BUF_DADDR_NULL;
	bp->b_maps[0].bm_bn = daddr;
	bp->b_flags |= XBF_READ;
	bp->b_ops = ops;

	xfs_buf_submit(bp);
	if (bp->b_error) {
		error = bp->b_error;
		xfs_buf_relse(bp);
		return error;
	}

	*bpp = bp;
	return 0;
}

int
xfs_buf_get_uncached(
	struct xfs_buftarg	*target,
	size_t			numblks,
	xfs_buf_flags_t		flags,
	struct xfs_buf		**bpp)
{
	int			error;
	struct xfs_buf		*bp;
	DEFINE_SINGLE_BUF_MAP(map, XFS_BUF_DADDR_NULL, numblks);

	*bpp = NULL;

	/* flags might contain irrelevant bits, pass only what we care about */
	error = _xfs_buf_alloc(target, &map, 1, flags & XBF_NO_IOACCT, &bp);
	if (error)
		return error;

	if (xfs_buftarg_is_mem(bp->b_target))
		error = xmbuf_map_page(bp);
	else
		error = xfs_buf_alloc_pages(bp, flags);
	if (error)
		goto fail_free_buf;

	error = _xfs_buf_map_pages(bp, 0);
	if (unlikely(error)) {
		xfs_warn(target->bt_mount,
			"%s: failed to map pages", __func__);
		goto fail_free_buf;
	}

	trace_xfs_buf_get_uncached(bp, _RET_IP_);
	*bpp = bp;
	return 0;

fail_free_buf:
	xfs_buf_free(bp);
	return error;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

/*
 *	Increment reference count on buffer, to hold the buffer concurrently
 *	with another thread which may release (free) the buffer asynchronously.
 *	Must hold the buffer already to call this function.
 */
void
xfs_buf_hold(
<<<<<<< HEAD
	xfs_buf_t		*bp)
=======
	struct xfs_buf		*bp)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
{
	trace_xfs_buf_hold(bp, _RET_IP_);
	atomic_inc(&bp->b_hold);
}

<<<<<<< HEAD
/*
 *	Releases a hold on the specified buffer.  If the
 *	the hold count is 1, calls xfs_buf_free.
 */
void
xfs_buf_rele(
	xfs_buf_t		*bp)
{
	struct xfs_perag	*pag = bp->b_pag;

	trace_xfs_buf_rele(bp, _RET_IP_);

	if (!pag) {
		ASSERT(list_empty(&bp->b_lru));
		ASSERT(RB_EMPTY_NODE(&bp->b_rbnode));
		if (atomic_dec_and_test(&bp->b_hold))
			xfs_buf_free(bp);
		return;
	}

	ASSERT(!RB_EMPTY_NODE(&bp->b_rbnode));

	ASSERT(atomic_read(&bp->b_hold) > 0);
	if (atomic_dec_and_lock(&bp->b_hold, &pag->pag_buf_lock)) {
		if (!(bp->b_flags & XBF_STALE) &&
			   atomic_read(&bp->b_lru_ref)) {
			xfs_buf_lru_add(bp);
			spin_unlock(&pag->pag_buf_lock);
		} else {
			xfs_buf_lru_del(bp);
			ASSERT(!(bp->b_flags & (XBF_DELWRI|_XBF_DELWRI_Q)));
			rb_erase(&bp->b_rbnode, &pag->pag_buf_tree);
			spin_unlock(&pag->pag_buf_lock);
			xfs_perag_put(pag);
			xfs_buf_free(bp);
		}
	}
}

=======
static void
xfs_buf_rele_uncached(
	struct xfs_buf		*bp)
{
	ASSERT(list_empty(&bp->b_lru));
	if (atomic_dec_and_test(&bp->b_hold)) {
		xfs_buf_ioacct_dec(bp);
		xfs_buf_free(bp);
	}
}

static void
xfs_buf_rele_cached(
	struct xfs_buf		*bp)
{
	struct xfs_buftarg	*btp = bp->b_target;
	struct xfs_perag	*pag = bp->b_pag;
	struct xfs_buf_cache	*bch = xfs_buftarg_buf_cache(btp, pag);
	bool			release;
	bool			freebuf = false;

	trace_xfs_buf_rele(bp, _RET_IP_);

	ASSERT(atomic_read(&bp->b_hold) > 0);

	/*
	 * We grab the b_lock here first to serialise racing xfs_buf_rele()
	 * calls. The pag_buf_lock being taken on the last reference only
	 * serialises against racing lookups in xfs_buf_find(). IOWs, the second
	 * to last reference we drop here is not serialised against the last
	 * reference until we take bp->b_lock. Hence if we don't grab b_lock
	 * first, the last "release" reference can win the race to the lock and
	 * free the buffer before the second-to-last reference is processed,
	 * leading to a use-after-free scenario.
	 */
	spin_lock(&bp->b_lock);
	release = atomic_dec_and_lock(&bp->b_hold, &bch->bc_lock);
	if (!release) {
		/*
		 * Drop the in-flight state if the buffer is already on the LRU
		 * and it holds the only reference. This is racy because we
		 * haven't acquired the pag lock, but the use of _XBF_IN_FLIGHT
		 * ensures the decrement occurs only once per-buf.
		 */
		if ((atomic_read(&bp->b_hold) == 1) && !list_empty(&bp->b_lru))
			__xfs_buf_ioacct_dec(bp);
		goto out_unlock;
	}

	/* the last reference has been dropped ... */
	__xfs_buf_ioacct_dec(bp);
	if (!(bp->b_flags & XBF_STALE) && atomic_read(&bp->b_lru_ref)) {
		/*
		 * If the buffer is added to the LRU take a new reference to the
		 * buffer for the LRU and clear the (now stale) dispose list
		 * state flag
		 */
		if (list_lru_add_obj(&btp->bt_lru, &bp->b_lru)) {
			bp->b_state &= ~XFS_BSTATE_DISPOSE;
			atomic_inc(&bp->b_hold);
		}
		spin_unlock(&bch->bc_lock);
	} else {
		/*
		 * most of the time buffers will already be removed from the
		 * LRU, so optimise that case by checking for the
		 * XFS_BSTATE_DISPOSE flag indicating the last list the buffer
		 * was on was the disposal list
		 */
		if (!(bp->b_state & XFS_BSTATE_DISPOSE)) {
			list_lru_del_obj(&btp->bt_lru, &bp->b_lru);
		} else {
			ASSERT(list_empty(&bp->b_lru));
		}

		ASSERT(!(bp->b_flags & _XBF_DELWRI_Q));
		rhashtable_remove_fast(&bch->bc_hash, &bp->b_rhash_head,
				xfs_buf_hash_params);
		spin_unlock(&bch->bc_lock);
		if (pag)
			xfs_perag_put(pag);
		freebuf = true;
	}

out_unlock:
	spin_unlock(&bp->b_lock);

	if (freebuf)
		xfs_buf_free(bp);
}

/*
 * Release a hold on the specified buffer.
 */
void
xfs_buf_rele(
	struct xfs_buf		*bp)
{
	trace_xfs_buf_rele(bp, _RET_IP_);
	if (xfs_buf_is_uncached(bp))
		xfs_buf_rele_uncached(bp);
	else
		xfs_buf_rele_cached(bp);
}
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

/*
 *	Lock a buffer object, if it is not already locked.
 *
 *	If we come across a stale, pinned, locked buffer, we know that we are
 *	being asked to lock a buffer that has been reallocated. Because it is
 *	pinned, we know that the log has not been pushed to disk and hence it
 *	will still be locked.  Rather than continuing to have trylock attempts
 *	fail until someone else pushes the log, push it ourselves before
 *	returning.  This means that the xfsaild will not get stuck trying
 *	to push on stale inode buffers.
 */
int
xfs_buf_trylock(
	struct xfs_buf		*bp)
{
	int			locked;

	locked = down_trylock(&bp->b_sema) == 0;
	if (locked)
<<<<<<< HEAD
		XB_SET_OWNER(bp);
	else if (atomic_read(&bp->b_pin_count) && (bp->b_flags & XBF_STALE))
		xfs_log_force(bp->b_target->bt_mount, 0);

	trace_xfs_buf_trylock(bp, _RET_IP_);
=======
		trace_xfs_buf_trylock(bp, _RET_IP_);
	else
		trace_xfs_buf_trylock_fail(bp, _RET_IP_);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	return locked;
}

/*
 *	Lock a buffer object.
 *
 *	If we come across a stale, pinned, locked buffer, we know that we
 *	are being asked to lock a buffer that has been reallocated. Because
 *	it is pinned, we know that the log has not been pushed to disk and
 *	hence it will still be locked. Rather than sleeping until someone
 *	else pushes the log, push it ourselves before trying to get the lock.
 */
void
xfs_buf_lock(
	struct xfs_buf		*bp)
{
	trace_xfs_buf_lock(bp, _RET_IP_);

	if (atomic_read(&bp->b_pin_count) && (bp->b_flags & XBF_STALE))
<<<<<<< HEAD
		xfs_log_force(bp->b_target->bt_mount, 0);
	down(&bp->b_sema);
	XB_SET_OWNER(bp);
=======
		xfs_log_force(bp->b_mount, 0);
	down(&bp->b_sema);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	trace_xfs_buf_lock_done(bp, _RET_IP_);
}

<<<<<<< HEAD
/*
 *	Releases the lock on the buffer object.
 *	If the buffer is marked delwri but is not queued, do so before we
 *	unlock the buffer as we need to set flags correctly.  We also need to
 *	take a reference for the delwri queue because the unlocker is going to
 *	drop their's and they don't know we just queued it.
 */
=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
void
xfs_buf_unlock(
	struct xfs_buf		*bp)
{
<<<<<<< HEAD
	XB_CLEAR_OWNER(bp);
	up(&bp->b_sema);

=======
	ASSERT(xfs_buf_islocked(bp));

	up(&bp->b_sema);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	trace_xfs_buf_unlock(bp, _RET_IP_);
}

STATIC void
xfs_buf_wait_unpin(
<<<<<<< HEAD
	xfs_buf_t		*bp)
=======
	struct xfs_buf		*bp)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
{
	DECLARE_WAITQUEUE	(wait, current);

	if (atomic_read(&bp->b_pin_count) == 0)
		return;

	add_wait_queue(&bp->b_waiters, &wait);
	for (;;) {
		set_current_state(TASK_UNINTERRUPTIBLE);
		if (atomic_read(&bp->b_pin_count) == 0)
			break;
		io_schedule();
	}
	remove_wait_queue(&bp->b_waiters, &wait);
	set_current_state(TASK_RUNNING);
}

<<<<<<< HEAD
/*
 *	Buffer Utility Routines
 */

STATIC void
xfs_buf_iodone_work(
	struct work_struct	*work)
{
	xfs_buf_t		*bp =
		container_of(work, xfs_buf_t, b_iodone_work);

	if (bp->b_iodone)
		(*(bp->b_iodone))(bp);
	else if (bp->b_flags & XBF_ASYNC)
		xfs_buf_relse(bp);
}

void
xfs_buf_ioend(
	xfs_buf_t		*bp,
	int			schedule)
{
	trace_xfs_buf_iodone(bp, _RET_IP_);

	bp->b_flags &= ~(XBF_READ | XBF_WRITE | XBF_READ_AHEAD);
	if (bp->b_error == 0)
		bp->b_flags |= XBF_DONE;

	if ((bp->b_iodone) || (bp->b_flags & XBF_ASYNC)) {
		if (schedule) {
			INIT_WORK(&bp->b_iodone_work, xfs_buf_iodone_work);
			queue_work(xfslogd_workqueue, &bp->b_iodone_work);
		} else {
			xfs_buf_iodone_work(&bp->b_iodone_work);
		}
	} else {
		complete(&bp->b_iowait);
	}
}

void
xfs_buf_ioerror(
	xfs_buf_t		*bp,
	int			error)
{
	ASSERT(error >= 0 && error <= 0xffff);
	bp->b_error = (unsigned short)error;
	trace_xfs_buf_ioerror(bp, error, _RET_IP_);
=======
static void
xfs_buf_ioerror_alert_ratelimited(
	struct xfs_buf		*bp)
{
	static unsigned long	lasttime;
	static struct xfs_buftarg *lasttarg;

	if (bp->b_target != lasttarg ||
	    time_after(jiffies, (lasttime + 5*HZ))) {
		lasttime = jiffies;
		xfs_buf_ioerror_alert(bp, __this_address);
	}
	lasttarg = bp->b_target;
}

/*
 * Account for this latest trip around the retry handler, and decide if
 * we've failed enough times to constitute a permanent failure.
 */
static bool
xfs_buf_ioerror_permanent(
	struct xfs_buf		*bp,
	struct xfs_error_cfg	*cfg)
{
	struct xfs_mount	*mp = bp->b_mount;

	if (cfg->max_retries != XFS_ERR_RETRY_FOREVER &&
	    ++bp->b_retries > cfg->max_retries)
		return true;
	if (cfg->retry_timeout != XFS_ERR_RETRY_FOREVER &&
	    time_after(jiffies, cfg->retry_timeout + bp->b_first_retry_time))
		return true;

	/* At unmount we may treat errors differently */
	if (xfs_is_unmounting(mp) && mp->m_fail_unmount)
		return true;

	return false;
}

/*
 * On a sync write or shutdown we just want to stale the buffer and let the
 * caller handle the error in bp->b_error appropriately.
 *
 * If the write was asynchronous then no one will be looking for the error.  If
 * this is the first failure of this type, clear the error state and write the
 * buffer out again. This means we always retry an async write failure at least
 * once, but we also need to set the buffer up to behave correctly now for
 * repeated failures.
 *
 * If we get repeated async write failures, then we take action according to the
 * error configuration we have been set up to use.
 *
 * Returns true if this function took care of error handling and the caller must
 * not touch the buffer again.  Return false if the caller should proceed with
 * normal I/O completion handling.
 */
static bool
xfs_buf_ioend_handle_error(
	struct xfs_buf		*bp)
{
	struct xfs_mount	*mp = bp->b_mount;
	struct xfs_error_cfg	*cfg;

	/*
	 * If we've already shutdown the journal because of I/O errors, there's
	 * no point in giving this a retry.
	 */
	if (xlog_is_shutdown(mp->m_log))
		goto out_stale;

	xfs_buf_ioerror_alert_ratelimited(bp);

	/*
	 * We're not going to bother about retrying this during recovery.
	 * One strike!
	 */
	if (bp->b_flags & _XBF_LOGRECOVERY) {
		xfs_force_shutdown(mp, SHUTDOWN_META_IO_ERROR);
		return false;
	}

	/*
	 * Synchronous writes will have callers process the error.
	 */
	if (!(bp->b_flags & XBF_ASYNC))
		goto out_stale;

	trace_xfs_buf_iodone_async(bp, _RET_IP_);

	cfg = xfs_error_get_cfg(mp, XFS_ERR_METADATA, bp->b_error);
	if (bp->b_last_error != bp->b_error ||
	    !(bp->b_flags & (XBF_STALE | XBF_WRITE_FAIL))) {
		bp->b_last_error = bp->b_error;
		if (cfg->retry_timeout != XFS_ERR_RETRY_FOREVER &&
		    !bp->b_first_retry_time)
			bp->b_first_retry_time = jiffies;
		goto resubmit;
	}

	/*
	 * Permanent error - we need to trigger a shutdown if we haven't already
	 * to indicate that inconsistency will result from this action.
	 */
	if (xfs_buf_ioerror_permanent(bp, cfg)) {
		xfs_force_shutdown(mp, SHUTDOWN_META_IO_ERROR);
		goto out_stale;
	}

	/* Still considered a transient error. Caller will schedule retries. */
	if (bp->b_flags & _XBF_INODES)
		xfs_buf_inode_io_fail(bp);
	else if (bp->b_flags & _XBF_DQUOTS)
		xfs_buf_dquot_io_fail(bp);
	else
		ASSERT(list_empty(&bp->b_li_list));
	xfs_buf_ioerror(bp, 0);
	xfs_buf_relse(bp);
	return true;

resubmit:
	xfs_buf_ioerror(bp, 0);
	bp->b_flags |= (XBF_DONE | XBF_WRITE_FAIL);
	xfs_buf_submit(bp);
	return true;
out_stale:
	xfs_buf_stale(bp);
	bp->b_flags |= XBF_DONE;
	bp->b_flags &= ~XBF_WRITE;
	trace_xfs_buf_error_relse(bp, _RET_IP_);
	return false;
}

static void
xfs_buf_ioend(
	struct xfs_buf	*bp)
{
	trace_xfs_buf_iodone(bp, _RET_IP_);

	/*
	 * Pull in IO completion errors now. We are guaranteed to be running
	 * single threaded, so we don't need the lock to read b_io_error.
	 */
	if (!bp->b_error && bp->b_io_error)
		xfs_buf_ioerror(bp, bp->b_io_error);

	if (bp->b_flags & XBF_READ) {
		if (!bp->b_error && bp->b_ops)
			bp->b_ops->verify_read(bp);
		if (!bp->b_error)
			bp->b_flags |= XBF_DONE;
	} else {
		if (!bp->b_error) {
			bp->b_flags &= ~XBF_WRITE_FAIL;
			bp->b_flags |= XBF_DONE;
		}

		if (unlikely(bp->b_error) && xfs_buf_ioend_handle_error(bp))
			return;

		/* clear the retry state */
		bp->b_last_error = 0;
		bp->b_retries = 0;
		bp->b_first_retry_time = 0;

		/*
		 * Note that for things like remote attribute buffers, there may
		 * not be a buffer log item here, so processing the buffer log
		 * item must remain optional.
		 */
		if (bp->b_log_item)
			xfs_buf_item_done(bp);

		if (bp->b_flags & _XBF_INODES)
			xfs_buf_inode_iodone(bp);
		else if (bp->b_flags & _XBF_DQUOTS)
			xfs_buf_dquot_iodone(bp);

	}

	bp->b_flags &= ~(XBF_READ | XBF_WRITE | XBF_READ_AHEAD |
			 _XBF_LOGRECOVERY);

	if (bp->b_flags & XBF_ASYNC)
		xfs_buf_relse(bp);
	else
		complete(&bp->b_iowait);
}

static void
xfs_buf_ioend_work(
	struct work_struct	*work)
{
	struct xfs_buf		*bp =
		container_of(work, struct xfs_buf, b_ioend_work);

	xfs_buf_ioend(bp);
}

static void
xfs_buf_ioend_async(
	struct xfs_buf	*bp)
{
	INIT_WORK(&bp->b_ioend_work, xfs_buf_ioend_work);
	queue_work(bp->b_mount->m_buf_workqueue, &bp->b_ioend_work);
}

void
__xfs_buf_ioerror(
	struct xfs_buf		*bp,
	int			error,
	xfs_failaddr_t		failaddr)
{
	ASSERT(error <= 0 && error >= -1000);
	bp->b_error = error;
	trace_xfs_buf_ioerror(bp, error, failaddr);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

void
xfs_buf_ioerror_alert(
	struct xfs_buf		*bp,
<<<<<<< HEAD
	const char		*func)
{
	xfs_alert(bp->b_target->bt_mount,
"metadata I/O error: block 0x%llx (\"%s\") error %d buf count %zd",
		(__uint64_t)XFS_BUF_ADDR(bp), func,
		bp->b_error, XFS_BUF_COUNT(bp));
=======
	xfs_failaddr_t		func)
{
	xfs_buf_alert_ratelimited(bp, "XFS: metadata IO error",
		"metadata I/O error in \"%pS\" at daddr 0x%llx len %d error %d",
				  func, (uint64_t)xfs_buf_daddr(bp),
				  bp->b_length, -bp->b_error);
}

/*
 * To simulate an I/O failure, the buffer must be locked and held with at least
 * three references. The LRU reference is dropped by the stale call. The buf
 * item reference is dropped via ioend processing. The third reference is owned
 * by the caller and is dropped on I/O completion if the buffer is XBF_ASYNC.
 */
void
xfs_buf_ioend_fail(
	struct xfs_buf	*bp)
{
	bp->b_flags &= ~XBF_DONE;
	xfs_buf_stale(bp);
	xfs_buf_ioerror(bp, -EIO);
	xfs_buf_ioend(bp);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

int
xfs_bwrite(
	struct xfs_buf		*bp)
{
	int			error;

<<<<<<< HEAD
	bp->b_flags |= XBF_WRITE;
	bp->b_flags &= ~(XBF_ASYNC | XBF_READ);

	xfs_buf_delwri_dequeue(bp);
	xfs_bdstrat_cb(bp);

	error = xfs_buf_iowait(bp);
	if (error) {
		xfs_force_shutdown(bp->b_target->bt_mount,
				   SHUTDOWN_META_IO_ERROR);
	}
	return error;
}

/*
 * Called when we want to stop a buffer from getting written or read.
 * We attach the EIO error, muck with its flags, and call xfs_buf_ioend
 * so that the proper iodone callbacks get called.
 */
STATIC int
xfs_bioerror(
	xfs_buf_t *bp)
{
#ifdef XFSERRORDEBUG
	ASSERT(XFS_BUF_ISREAD(bp) || bp->b_iodone);
#endif

	/*
	 * No need to wait until the buffer is unpinned, we aren't flushing it.
	 */
	xfs_buf_ioerror(bp, EIO);

	/*
	 * We're calling xfs_buf_ioend, so delete XBF_DONE flag.
	 */
	XFS_BUF_UNREAD(bp);
	XFS_BUF_UNDONE(bp);
	xfs_buf_stale(bp);

	xfs_buf_ioend(bp, 0);

	return EIO;
}

/*
 * Same as xfs_bioerror, except that we are releasing the buffer
 * here ourselves, and avoiding the xfs_buf_ioend call.
 * This is meant for userdata errors; metadata bufs come with
 * iodone functions attached, so that we can track down errors.
 */
STATIC int
xfs_bioerror_relse(
	struct xfs_buf	*bp)
{
	int64_t		fl = bp->b_flags;
	/*
	 * No need to wait until the buffer is unpinned.
	 * We aren't flushing it.
	 *
	 * chunkhold expects B_DONE to be set, whether
	 * we actually finish the I/O or not. We don't want to
	 * change that interface.
	 */
	XFS_BUF_UNREAD(bp);
	XFS_BUF_DONE(bp);
	xfs_buf_stale(bp);
	bp->b_iodone = NULL;
	if (!(fl & XBF_ASYNC)) {
		/*
		 * Mark b_error and B_ERROR _both_.
		 * Lot's of chunkcache code assumes that.
		 * There's no reason to mark error for
		 * ASYNC buffers.
		 */
		xfs_buf_ioerror(bp, EIO);
		complete(&bp->b_iowait);
	} else {
		xfs_buf_relse(bp);
	}

	return EIO;
}


/*
 * All xfs metadata buffers except log state machine buffers
 * get this attached as their b_bdstrat callback function.
 * This is so that we can catch a buffer
 * after prematurely unpinning it to forcibly shutdown the filesystem.
 */
int
xfs_bdstrat_cb(
	struct xfs_buf	*bp)
{
	if (XFS_FORCED_SHUTDOWN(bp->b_target->bt_mount)) {
		trace_xfs_bdstrat_shut(bp, _RET_IP_);
		/*
		 * Metadata write that didn't get logged but
		 * written delayed anyway. These aren't associated
		 * with a transaction, and can be ignored.
		 */
		if (!bp->b_iodone && !XFS_BUF_ISREAD(bp))
			return xfs_bioerror_relse(bp);
		else
			return xfs_bioerror(bp);
	}

	xfs_buf_iorequest(bp);
	return 0;
}

/*
 * Wrapper around bdstrat so that we can stop data from going to disk in case
 * we are shutting down the filesystem.  Typically user data goes thru this
 * path; one of the exceptions is the superblock.
 */
void
xfsbdstrat(
	struct xfs_mount	*mp,
	struct xfs_buf		*bp)
{
	if (XFS_FORCED_SHUTDOWN(mp)) {
		trace_xfs_bdstrat_shut(bp, _RET_IP_);
		xfs_bioerror_relse(bp);
		return;
	}

	xfs_buf_iorequest(bp);
}

STATIC void
_xfs_buf_ioend(
	xfs_buf_t		*bp,
	int			schedule)
{
	if (atomic_dec_and_test(&bp->b_io_remaining) == 1)
		xfs_buf_ioend(bp, schedule);
}

STATIC void
xfs_buf_bio_end_io(
	struct bio		*bio,
	int			error)
{
	xfs_buf_t		*bp = (xfs_buf_t *)bio->bi_private;
=======
	ASSERT(xfs_buf_islocked(bp));

	bp->b_flags |= XBF_WRITE;
	bp->b_flags &= ~(XBF_ASYNC | XBF_READ | _XBF_DELWRI_Q |
			 XBF_DONE);

	error = xfs_buf_submit(bp);
	if (error)
		xfs_force_shutdown(bp->b_mount, SHUTDOWN_META_IO_ERROR);
	return error;
}

static void
xfs_buf_bio_end_io(
	struct bio		*bio)
{
	struct xfs_buf		*bp = (struct xfs_buf *)bio->bi_private;

	if (!bio->bi_status &&
	    (bp->b_flags & XBF_WRITE) && (bp->b_flags & XBF_ASYNC) &&
	    XFS_TEST_ERROR(false, bp->b_mount, XFS_ERRTAG_BUF_IOERROR))
		bio->bi_status = BLK_STS_IOERR;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	/*
	 * don't overwrite existing errors - otherwise we can lose errors on
	 * buffers that require multiple bios to complete.
	 */
<<<<<<< HEAD
	if (!bp->b_error)
		xfs_buf_ioerror(bp, -error);
=======
	if (bio->bi_status) {
		int error = blk_status_to_errno(bio->bi_status);

		cmpxchg(&bp->b_io_error, 0, error);
	}
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	if (!bp->b_error && xfs_buf_is_vmapped(bp) && (bp->b_flags & XBF_READ))
		invalidate_kernel_vmap_range(bp->b_addr, xfs_buf_vmap_len(bp));

<<<<<<< HEAD
	_xfs_buf_ioend(bp, 1);
	bio_put(bio);
}

STATIC void
_xfs_buf_ioapply(
	xfs_buf_t		*bp)
{
	int			rw, map_i, total_nr_pages, nr_pages;
	struct bio		*bio;
	int			offset = bp->b_offset;
	int			size = bp->b_count_desired;
	sector_t		sector = bp->b_bn;

	total_nr_pages = bp->b_page_count;
	map_i = 0;

	if (bp->b_flags & XBF_WRITE) {
		if (bp->b_flags & XBF_SYNCIO)
			rw = WRITE_SYNC;
		else
			rw = WRITE;
		if (bp->b_flags & XBF_FUA)
			rw |= REQ_FUA;
		if (bp->b_flags & XBF_FLUSH)
			rw |= REQ_FLUSH;
	} else if (bp->b_flags & XBF_READ_AHEAD) {
		rw = READA;
	} else {
		rw = READ;
	}

	/* we only use the buffer cache for meta-data */
	rw |= REQ_META;

next_chunk:
	atomic_inc(&bp->b_io_remaining);
	nr_pages = BIO_MAX_SECTORS >> (PAGE_SHIFT - BBSHIFT);
	if (nr_pages > total_nr_pages)
		nr_pages = total_nr_pages;

	bio = bio_alloc(GFP_NOIO, nr_pages);
	bio->bi_bdev = bp->b_target->bt_bdev;
	bio->bi_sector = sector;
	bio->bi_end_io = xfs_buf_bio_end_io;
	bio->bi_private = bp;


	for (; size && nr_pages; nr_pages--, map_i++) {
=======
	if (atomic_dec_and_test(&bp->b_io_remaining) == 1)
		xfs_buf_ioend_async(bp);
	bio_put(bio);
}

static void
xfs_buf_ioapply_map(
	struct xfs_buf	*bp,
	int		map,
	int		*buf_offset,
	int		*count,
	blk_opf_t	op)
{
	int		page_index;
	unsigned int	total_nr_pages = bp->b_page_count;
	int		nr_pages;
	struct bio	*bio;
	sector_t	sector =  bp->b_maps[map].bm_bn;
	int		size;
	int		offset;

	/* skip the pages in the buffer before the start offset */
	page_index = 0;
	offset = *buf_offset;
	while (offset >= PAGE_SIZE) {
		page_index++;
		offset -= PAGE_SIZE;
	}

	/*
	 * Limit the IO size to the length of the current vector, and update the
	 * remaining IO count for the next time around.
	 */
	size = min_t(int, BBTOB(bp->b_maps[map].bm_len), *count);
	*count -= size;
	*buf_offset += size;

next_chunk:
	atomic_inc(&bp->b_io_remaining);
	nr_pages = bio_max_segs(total_nr_pages);

	bio = bio_alloc(bp->b_target->bt_bdev, nr_pages, op, GFP_NOIO);
	bio->bi_iter.bi_sector = sector;
	bio->bi_end_io = xfs_buf_bio_end_io;
	bio->bi_private = bp;

	for (; size && nr_pages; nr_pages--, page_index++) {
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		int	rbytes, nbytes = PAGE_SIZE - offset;

		if (nbytes > size)
			nbytes = size;

<<<<<<< HEAD
		rbytes = bio_add_page(bio, bp->b_pages[map_i], nbytes, offset);
=======
		rbytes = bio_add_page(bio, bp->b_pages[page_index], nbytes,
				      offset);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		if (rbytes < nbytes)
			break;

		offset = 0;
<<<<<<< HEAD
		sector += nbytes >> BBSHIFT;
=======
		sector += BTOBB(nbytes);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		size -= nbytes;
		total_nr_pages--;
	}

<<<<<<< HEAD
	if (likely(bio->bi_size)) {
=======
	if (likely(bio->bi_iter.bi_size)) {
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		if (xfs_buf_is_vmapped(bp)) {
			flush_kernel_vmap_range(bp->b_addr,
						xfs_buf_vmap_len(bp));
		}
<<<<<<< HEAD
		submit_bio(rw, bio);
=======
		submit_bio(bio);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		if (size)
			goto next_chunk;
	} else {
		/*
		 * This is guaranteed not to be the last io reference count
<<<<<<< HEAD
		 * because the caller (xfs_buf_iorequest) holds a count itself.
		 */
		atomic_dec(&bp->b_io_remaining);
		xfs_buf_ioerror(bp, EIO);
		bio_put(bio);
	}
}

int
xfs_buf_iorequest(
	xfs_buf_t		*bp)
{
	trace_xfs_buf_iorequest(bp, _RET_IP_);

	ASSERT(!(bp->b_flags & XBF_DELWRI));

	if (bp->b_flags & XBF_WRITE)
		xfs_buf_wait_unpin(bp);
	xfs_buf_hold(bp);

	/* Set the count to 1 initially, this will stop an I/O
	 * completion callout which happens before we have started
	 * all the I/O from calling xfs_buf_ioend too early.
	 */
	atomic_set(&bp->b_io_remaining, 1);
	_xfs_buf_ioapply(bp);
	_xfs_buf_ioend(bp, 0);

	xfs_buf_rele(bp);
	return 0;
}

/*
 *	Waits for I/O to complete on the buffer supplied.
 *	It returns immediately if no I/O is pending.
 *	It returns the I/O error code, if any, or 0 if there was no error.
 */
int
xfs_buf_iowait(
	xfs_buf_t		*bp)
{
	trace_xfs_buf_iowait(bp, _RET_IP_);

	wait_for_completion(&bp->b_iowait);

	trace_xfs_buf_iowait_done(bp, _RET_IP_);
	return bp->b_error;
}

xfs_caddr_t
xfs_buf_offset(
	xfs_buf_t		*bp,
=======
		 * because the caller (xfs_buf_submit) holds a count itself.
		 */
		atomic_dec(&bp->b_io_remaining);
		xfs_buf_ioerror(bp, -EIO);
		bio_put(bio);
	}

}

STATIC void
_xfs_buf_ioapply(
	struct xfs_buf	*bp)
{
	struct blk_plug	plug;
	blk_opf_t	op;
	int		offset;
	int		size;
	int		i;

	/*
	 * Make sure we capture only current IO errors rather than stale errors
	 * left over from previous use of the buffer (e.g. failed readahead).
	 */
	bp->b_error = 0;

	if (bp->b_flags & XBF_WRITE) {
		op = REQ_OP_WRITE;

		/*
		 * Run the write verifier callback function if it exists. If
		 * this function fails it will mark the buffer with an error and
		 * the IO should not be dispatched.
		 */
		if (bp->b_ops) {
			bp->b_ops->verify_write(bp);
			if (bp->b_error) {
				xfs_force_shutdown(bp->b_mount,
						   SHUTDOWN_CORRUPT_INCORE);
				return;
			}
		} else if (bp->b_rhash_key != XFS_BUF_DADDR_NULL) {
			struct xfs_mount *mp = bp->b_mount;

			/*
			 * non-crc filesystems don't attach verifiers during
			 * log recovery, so don't warn for such filesystems.
			 */
			if (xfs_has_crc(mp)) {
				xfs_warn(mp,
					"%s: no buf ops on daddr 0x%llx len %d",
					__func__, xfs_buf_daddr(bp),
					bp->b_length);
				xfs_hex_dump(bp->b_addr,
						XFS_CORRUPTION_DUMP_LEN);
				dump_stack();
			}
		}
	} else {
		op = REQ_OP_READ;
		if (bp->b_flags & XBF_READ_AHEAD)
			op |= REQ_RAHEAD;
	}

	/* we only use the buffer cache for meta-data */
	op |= REQ_META;

	/* in-memory targets are directly mapped, no IO required. */
	if (xfs_buftarg_is_mem(bp->b_target)) {
		xfs_buf_ioend(bp);
		return;
	}

	/*
	 * Walk all the vectors issuing IO on them. Set up the initial offset
	 * into the buffer and the desired IO size before we start -
	 * _xfs_buf_ioapply_vec() will modify them appropriately for each
	 * subsequent call.
	 */
	offset = bp->b_offset;
	size = BBTOB(bp->b_length);
	blk_start_plug(&plug);
	for (i = 0; i < bp->b_map_count; i++) {
		xfs_buf_ioapply_map(bp, i, &offset, &size, op);
		if (bp->b_error)
			break;
		if (size <= 0)
			break;	/* all done */
	}
	blk_finish_plug(&plug);
}

/*
 * Wait for I/O completion of a sync buffer and return the I/O error code.
 */
static int
xfs_buf_iowait(
	struct xfs_buf	*bp)
{
	ASSERT(!(bp->b_flags & XBF_ASYNC));

	trace_xfs_buf_iowait(bp, _RET_IP_);
	wait_for_completion(&bp->b_iowait);
	trace_xfs_buf_iowait_done(bp, _RET_IP_);

	return bp->b_error;
}

/*
 * Buffer I/O submission path, read or write. Asynchronous submission transfers
 * the buffer lock ownership and the current reference to the IO. It is not
 * safe to reference the buffer after a call to this function unless the caller
 * holds an additional reference itself.
 */
static int
__xfs_buf_submit(
	struct xfs_buf	*bp,
	bool		wait)
{
	int		error = 0;

	trace_xfs_buf_submit(bp, _RET_IP_);

	ASSERT(!(bp->b_flags & _XBF_DELWRI_Q));

	/*
	 * On log shutdown we stale and complete the buffer immediately. We can
	 * be called to read the superblock before the log has been set up, so
	 * be careful checking the log state.
	 *
	 * Checking the mount shutdown state here can result in the log tail
	 * moving inappropriately on disk as the log may not yet be shut down.
	 * i.e. failing this buffer on mount shutdown can remove it from the AIL
	 * and move the tail of the log forwards without having written this
	 * buffer to disk. This corrupts the log tail state in memory, and
	 * because the log may not be shut down yet, it can then be propagated
	 * to disk before the log is shutdown. Hence we check log shutdown
	 * state here rather than mount state to avoid corrupting the log tail
	 * on shutdown.
	 */
	if (bp->b_mount->m_log &&
	    xlog_is_shutdown(bp->b_mount->m_log)) {
		xfs_buf_ioend_fail(bp);
		return -EIO;
	}

	/*
	 * Grab a reference so the buffer does not go away underneath us. For
	 * async buffers, I/O completion drops the callers reference, which
	 * could occur before submission returns.
	 */
	xfs_buf_hold(bp);

	if (bp->b_flags & XBF_WRITE)
		xfs_buf_wait_unpin(bp);

	/* clear the internal error state to avoid spurious errors */
	bp->b_io_error = 0;

	/*
	 * Set the count to 1 initially, this will stop an I/O completion
	 * callout which happens before we have started all the I/O from calling
	 * xfs_buf_ioend too early.
	 */
	atomic_set(&bp->b_io_remaining, 1);
	if (bp->b_flags & XBF_ASYNC)
		xfs_buf_ioacct_inc(bp);
	_xfs_buf_ioapply(bp);

	/*
	 * If _xfs_buf_ioapply failed, we can get back here with only the IO
	 * reference we took above. If we drop it to zero, run completion so
	 * that we don't return to the caller with completion still pending.
	 */
	if (atomic_dec_and_test(&bp->b_io_remaining) == 1) {
		if (bp->b_error || !(bp->b_flags & XBF_ASYNC))
			xfs_buf_ioend(bp);
		else
			xfs_buf_ioend_async(bp);
	}

	if (wait)
		error = xfs_buf_iowait(bp);

	/*
	 * Release the hold that keeps the buffer referenced for the entire
	 * I/O. Note that if the buffer is async, it is not safe to reference
	 * after this release.
	 */
	xfs_buf_rele(bp);
	return error;
}

void *
xfs_buf_offset(
	struct xfs_buf		*bp,
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	size_t			offset)
{
	struct page		*page;

<<<<<<< HEAD
	if (bp->b_flags & XBF_MAPPED)
		return bp->b_addr + offset;

	offset += bp->b_offset;
	page = bp->b_pages[offset >> PAGE_SHIFT];
	return (xfs_caddr_t)page_address(page) + (offset & (PAGE_SIZE-1));
}

/*
 *	Move data into or out of a buffer.
 */
void
xfs_buf_iomove(
	xfs_buf_t		*bp,	/* buffer to process		*/
	size_t			boff,	/* starting buffer offset	*/
	size_t			bsize,	/* length to copy		*/
	void			*data,	/* data address			*/
	xfs_buf_rw_t		mode)	/* read/write/zero flag		*/
{
	size_t			bend, cpoff, csize;
	struct page		*page;

	bend = boff + bsize;
	while (boff < bend) {
		page = bp->b_pages[xfs_buf_btoct(boff + bp->b_offset)];
		cpoff = xfs_buf_poff(boff + bp->b_offset);
		csize = min_t(size_t,
			      PAGE_SIZE-cpoff, bp->b_count_desired-boff);

		ASSERT(((csize + cpoff) <= PAGE_SIZE));

		switch (mode) {
		case XBRW_ZERO:
			memset(page_address(page) + cpoff, 0, csize);
			break;
		case XBRW_READ:
			memcpy(data, page_address(page) + cpoff, csize);
			break;
		case XBRW_WRITE:
			memcpy(page_address(page) + cpoff, data, csize);
		}

		boff += csize;
		data += csize;
=======
	if (bp->b_addr)
		return bp->b_addr + offset;

	page = bp->b_pages[offset >> PAGE_SHIFT];
	return page_address(page) + (offset & (PAGE_SIZE-1));
}

void
xfs_buf_zero(
	struct xfs_buf		*bp,
	size_t			boff,
	size_t			bsize)
{
	size_t			bend;

	bend = boff + bsize;
	while (boff < bend) {
		struct page	*page;
		int		page_index, page_offset, csize;

		page_index = (boff + bp->b_offset) >> PAGE_SHIFT;
		page_offset = (boff + bp->b_offset) & ~PAGE_MASK;
		page = bp->b_pages[page_index];
		csize = min_t(size_t, PAGE_SIZE - page_offset,
				      BBTOB(bp->b_length) - boff);

		ASSERT((csize + page_offset) <= PAGE_SIZE);

		memset(page_address(page) + page_offset, 0, csize);

		boff += csize;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	}
}

/*
<<<<<<< HEAD
=======
 * Log a message about and stale a buffer that a caller has decided is corrupt.
 *
 * This function should be called for the kinds of metadata corruption that
 * cannot be detect from a verifier, such as incorrect inter-block relationship
 * data.  Do /not/ call this function from a verifier function.
 *
 * The buffer must be XBF_DONE prior to the call.  Afterwards, the buffer will
 * be marked stale, but b_error will not be set.  The caller is responsible for
 * releasing the buffer or fixing it.
 */
void
__xfs_buf_mark_corrupt(
	struct xfs_buf		*bp,
	xfs_failaddr_t		fa)
{
	ASSERT(bp->b_flags & XBF_DONE);

	xfs_buf_corruption_error(bp, fa);
	xfs_buf_stale(bp);
}

/*
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 *	Handling of buffer targets (buftargs).
 */

/*
 * Wait for any bufs with callbacks that have been submitted but have not yet
 * returned. These buffers will have an elevated hold count, so wait on those
 * while freeing all the buffers only held by the LRU.
 */
<<<<<<< HEAD
void
xfs_wait_buftarg(
	struct xfs_buftarg	*btp)
{
	struct xfs_buf		*bp;

restart:
	spin_lock(&btp->bt_lru_lock);
	while (!list_empty(&btp->bt_lru)) {
		bp = list_first_entry(&btp->bt_lru, struct xfs_buf, b_lru);
		if (atomic_read(&bp->b_hold) > 1) {
			spin_unlock(&btp->bt_lru_lock);
			delay(100);
			goto restart;
		}
		/*
		 * clear the LRU reference count so the buffer doesn't get
		 * ignored in xfs_buf_rele().
		 */
		atomic_set(&bp->b_lru_ref, 0);
		spin_unlock(&btp->bt_lru_lock);
		xfs_buf_rele(bp);
		spin_lock(&btp->bt_lru_lock);
	}
	spin_unlock(&btp->bt_lru_lock);
}

int
xfs_buftarg_shrink(
	struct shrinker		*shrink,
	struct shrink_control	*sc)
{
	struct xfs_buftarg	*btp = container_of(shrink,
					struct xfs_buftarg, bt_shrinker);
	struct xfs_buf		*bp;
	int nr_to_scan = sc->nr_to_scan;
	LIST_HEAD(dispose);

	if (!nr_to_scan)
		return btp->bt_lru_nr;

	spin_lock(&btp->bt_lru_lock);
	while (!list_empty(&btp->bt_lru)) {
		if (nr_to_scan-- <= 0)
			break;

		bp = list_first_entry(&btp->bt_lru, struct xfs_buf, b_lru);

		/*
		 * Decrement the b_lru_ref count unless the value is already
		 * zero. If the value is already zero, we need to reclaim the
		 * buffer, otherwise it gets another trip through the LRU.
		 */
		if (!atomic_add_unless(&bp->b_lru_ref, -1, 0)) {
			list_move_tail(&bp->b_lru, &btp->bt_lru);
			continue;
		}

		/*
		 * remove the buffer from the LRU now to avoid needing another
		 * lock round trip inside xfs_buf_rele().
		 */
		list_move(&bp->b_lru, &dispose);
		btp->bt_lru_nr--;
	}
	spin_unlock(&btp->bt_lru_lock);

	while (!list_empty(&dispose)) {
=======
static enum lru_status
xfs_buftarg_drain_rele(
	struct list_head	*item,
	struct list_lru_one	*lru,
	spinlock_t		*lru_lock,
	void			*arg)

{
	struct xfs_buf		*bp = container_of(item, struct xfs_buf, b_lru);
	struct list_head	*dispose = arg;

	if (atomic_read(&bp->b_hold) > 1) {
		/* need to wait, so skip it this pass */
		trace_xfs_buf_drain_buftarg(bp, _RET_IP_);
		return LRU_SKIP;
	}
	if (!spin_trylock(&bp->b_lock))
		return LRU_SKIP;

	/*
	 * clear the LRU reference count so the buffer doesn't get
	 * ignored in xfs_buf_rele().
	 */
	atomic_set(&bp->b_lru_ref, 0);
	bp->b_state |= XFS_BSTATE_DISPOSE;
	list_lru_isolate_move(lru, item, dispose);
	spin_unlock(&bp->b_lock);
	return LRU_REMOVED;
}

/*
 * Wait for outstanding I/O on the buftarg to complete.
 */
void
xfs_buftarg_wait(
	struct xfs_buftarg	*btp)
{
	/*
	 * First wait on the buftarg I/O count for all in-flight buffers to be
	 * released. This is critical as new buffers do not make the LRU until
	 * they are released.
	 *
	 * Next, flush the buffer workqueue to ensure all completion processing
	 * has finished. Just waiting on buffer locks is not sufficient for
	 * async IO as the reference count held over IO is not released until
	 * after the buffer lock is dropped. Hence we need to ensure here that
	 * all reference counts have been dropped before we start walking the
	 * LRU list.
	 */
	while (percpu_counter_sum(&btp->bt_io_count))
		delay(100);
	flush_workqueue(btp->bt_mount->m_buf_workqueue);
}

void
xfs_buftarg_drain(
	struct xfs_buftarg	*btp)
{
	LIST_HEAD(dispose);
	int			loop = 0;
	bool			write_fail = false;

	xfs_buftarg_wait(btp);

	/* loop until there is nothing left on the lru list. */
	while (list_lru_count(&btp->bt_lru)) {
		list_lru_walk(&btp->bt_lru, xfs_buftarg_drain_rele,
			      &dispose, LONG_MAX);

		while (!list_empty(&dispose)) {
			struct xfs_buf *bp;
			bp = list_first_entry(&dispose, struct xfs_buf, b_lru);
			list_del_init(&bp->b_lru);
			if (bp->b_flags & XBF_WRITE_FAIL) {
				write_fail = true;
				xfs_buf_alert_ratelimited(bp,
					"XFS: Corruption Alert",
"Corruption Alert: Buffer at daddr 0x%llx had permanent write failures!",
					(long long)xfs_buf_daddr(bp));
			}
			xfs_buf_rele(bp);
		}
		if (loop++ != 0)
			delay(100);
	}

	/*
	 * If one or more failed buffers were freed, that means dirty metadata
	 * was thrown away. This should only ever happen after I/O completion
	 * handling has elevated I/O error(s) to permanent failures and shuts
	 * down the journal.
	 */
	if (write_fail) {
		ASSERT(xlog_is_shutdown(btp->bt_mount->m_log));
		xfs_alert(btp->bt_mount,
	      "Please run xfs_repair to determine the extent of the problem.");
	}
}

static enum lru_status
xfs_buftarg_isolate(
	struct list_head	*item,
	struct list_lru_one	*lru,
	spinlock_t		*lru_lock,
	void			*arg)
{
	struct xfs_buf		*bp = container_of(item, struct xfs_buf, b_lru);
	struct list_head	*dispose = arg;

	/*
	 * we are inverting the lru lock/bp->b_lock here, so use a trylock.
	 * If we fail to get the lock, just skip it.
	 */
	if (!spin_trylock(&bp->b_lock))
		return LRU_SKIP;
	/*
	 * Decrement the b_lru_ref count unless the value is already
	 * zero. If the value is already zero, we need to reclaim the
	 * buffer, otherwise it gets another trip through the LRU.
	 */
	if (atomic_add_unless(&bp->b_lru_ref, -1, 0)) {
		spin_unlock(&bp->b_lock);
		return LRU_ROTATE;
	}

	bp->b_state |= XFS_BSTATE_DISPOSE;
	list_lru_isolate_move(lru, item, dispose);
	spin_unlock(&bp->b_lock);
	return LRU_REMOVED;
}

static unsigned long
xfs_buftarg_shrink_scan(
	struct shrinker		*shrink,
	struct shrink_control	*sc)
{
	struct xfs_buftarg	*btp = shrink->private_data;
	LIST_HEAD(dispose);
	unsigned long		freed;

	freed = list_lru_shrink_walk(&btp->bt_lru, sc,
				     xfs_buftarg_isolate, &dispose);

	while (!list_empty(&dispose)) {
		struct xfs_buf *bp;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		bp = list_first_entry(&dispose, struct xfs_buf, b_lru);
		list_del_init(&bp->b_lru);
		xfs_buf_rele(bp);
	}

<<<<<<< HEAD
	return btp->bt_lru_nr;
=======
	return freed;
}

static unsigned long
xfs_buftarg_shrink_count(
	struct shrinker		*shrink,
	struct shrink_control	*sc)
{
	struct xfs_buftarg	*btp = shrink->private_data;
	return list_lru_shrink_count(&btp->bt_lru, sc);
}

void
xfs_destroy_buftarg(
	struct xfs_buftarg	*btp)
{
	shrinker_free(btp->bt_shrinker);
	ASSERT(percpu_counter_sum(&btp->bt_io_count) == 0);
	percpu_counter_destroy(&btp->bt_io_count);
	list_lru_destroy(&btp->bt_lru);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

void
xfs_free_buftarg(
<<<<<<< HEAD
	struct xfs_mount	*mp,
	struct xfs_buftarg	*btp)
{
	unregister_shrinker(&btp->bt_shrinker);

	xfs_flush_buftarg(btp, 1);
	if (mp->m_flags & XFS_MOUNT_BARRIER)
		xfs_blkdev_issue_flush(btp);

	kthread_stop(btp->bt_task);
	kmem_free(btp);
}

STATIC int
xfs_setsize_buftarg_flags(
	xfs_buftarg_t		*btp,
	unsigned int		blocksize,
	unsigned int		sectorsize,
	int			verbose)
{
	btp->bt_bsize = blocksize;
	btp->bt_sshift = ffs(sectorsize) - 1;
	btp->bt_smask = sectorsize - 1;

	if (set_blocksize(btp->bt_bdev, sectorsize)) {
		char name[BDEVNAME_SIZE];

		bdevname(btp->bt_bdev, name);

		xfs_warn(btp->bt_mount,
			"Cannot set_blocksize to %u on device %s\n",
			sectorsize, name);
		return EINVAL;
	}

	return 0;
}

/*
 *	When allocating the initial buffer target we have not yet
 *	read in the superblock, so don't know what sized sectors
 *	are being used is at this early stage.  Play safe.
 */
STATIC int
xfs_setsize_buftarg_early(
	xfs_buftarg_t		*btp,
	struct block_device	*bdev)
{
	return xfs_setsize_buftarg_flags(btp,
			PAGE_SIZE, bdev_logical_block_size(bdev), 0);
=======
	struct xfs_buftarg	*btp)
{
	xfs_destroy_buftarg(btp);
	fs_put_dax(btp->bt_daxdev, btp->bt_mount);
	/* the main block device is closed by kill_block_super */
	if (btp->bt_bdev != btp->bt_mount->m_super->s_bdev)
		bdev_fput(btp->bt_bdev_file);
	kfree(btp);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

int
xfs_setsize_buftarg(
<<<<<<< HEAD
	xfs_buftarg_t		*btp,
	unsigned int		blocksize,
	unsigned int		sectorsize)
{
	return xfs_setsize_buftarg_flags(btp, blocksize, sectorsize, 1);
}

STATIC int
xfs_alloc_delwri_queue(
	xfs_buftarg_t		*btp,
	const char		*fsname)
{
	INIT_LIST_HEAD(&btp->bt_delwri_queue);
	spin_lock_init(&btp->bt_delwri_lock);
	btp->bt_flags = 0;
	btp->bt_task = kthread_run(xfsbufd, btp, "xfsbufd/%s", fsname);
	if (IS_ERR(btp->bt_task))
		return PTR_ERR(btp->bt_task);
	return 0;
}

xfs_buftarg_t *
xfs_alloc_buftarg(
	struct xfs_mount	*mp,
	struct block_device	*bdev,
	int			external,
	const char		*fsname)
{
	xfs_buftarg_t		*btp;

	btp = kmem_zalloc(sizeof(*btp), KM_SLEEP);

	btp->bt_mount = mp;
	btp->bt_dev =  bdev->bd_dev;
	btp->bt_bdev = bdev;
	btp->bt_bdi = blk_get_backing_dev_info(bdev);
	if (!btp->bt_bdi)
		goto error;

	INIT_LIST_HEAD(&btp->bt_lru);
	spin_lock_init(&btp->bt_lru_lock);
	if (xfs_setsize_buftarg_early(btp, bdev))
		goto error;
	if (xfs_alloc_delwri_queue(btp, fsname))
		goto error;
	btp->bt_shrinker.shrink = xfs_buftarg_shrink;
	btp->bt_shrinker.seeks = DEFAULT_SEEKS;
	register_shrinker(&btp->bt_shrinker);
	return btp;

error:
	kmem_free(btp);
	return NULL;
}


/*
 *	Delayed write buffer handling
 */
void
xfs_buf_delwri_queue(
	xfs_buf_t		*bp)
{
	struct xfs_buftarg	*btp = bp->b_target;

	trace_xfs_buf_delwri_queue(bp, _RET_IP_);

	ASSERT(!(bp->b_flags & XBF_READ));

	spin_lock(&btp->bt_delwri_lock);
	if (!list_empty(&bp->b_list)) {
		/* if already in the queue, move it to the tail */
		ASSERT(bp->b_flags & _XBF_DELWRI_Q);
		list_move_tail(&bp->b_list, &btp->bt_delwri_queue);
	} else {
		/* start xfsbufd as it is about to have something to do */
		if (list_empty(&btp->bt_delwri_queue))
			wake_up_process(bp->b_target->bt_task);

		atomic_inc(&bp->b_hold);
		bp->b_flags |= XBF_DELWRI | _XBF_DELWRI_Q | XBF_ASYNC;
		list_add_tail(&bp->b_list, &btp->bt_delwri_queue);
	}
	bp->b_queuetime = jiffies;
	spin_unlock(&btp->bt_delwri_lock);
}

void
xfs_buf_delwri_dequeue(
	xfs_buf_t		*bp)
{
	int			dequeued = 0;

	spin_lock(&bp->b_target->bt_delwri_lock);
	if ((bp->b_flags & XBF_DELWRI) && !list_empty(&bp->b_list)) {
		ASSERT(bp->b_flags & _XBF_DELWRI_Q);
		list_del_init(&bp->b_list);
		dequeued = 1;
	}
	bp->b_flags &= ~(XBF_DELWRI|_XBF_DELWRI_Q);
	spin_unlock(&bp->b_target->bt_delwri_lock);

	if (dequeued)
		xfs_buf_rele(bp);

	trace_xfs_buf_delwri_dequeue(bp, _RET_IP_);
}

/*
 * If a delwri buffer needs to be pushed before it has aged out, then promote
 * it to the head of the delwri queue so that it will be flushed on the next
 * xfsbufd run. We do this by resetting the queuetime of the buffer to be older
 * than the age currently needed to flush the buffer. Hence the next time the
 * xfsbufd sees it is guaranteed to be considered old enough to flush.
 */
void
xfs_buf_delwri_promote(
	struct xfs_buf	*bp)
{
	struct xfs_buftarg *btp = bp->b_target;
	long		age = xfs_buf_age_centisecs * msecs_to_jiffies(10) + 1;

	ASSERT(bp->b_flags & XBF_DELWRI);
	ASSERT(bp->b_flags & _XBF_DELWRI_Q);

	/*
	 * Check the buffer age before locking the delayed write queue as we
	 * don't need to promote buffers that are already past the flush age.
	 */
	if (bp->b_queuetime < jiffies - age)
		return;
	bp->b_queuetime = jiffies - age;
	spin_lock(&btp->bt_delwri_lock);
	list_move(&bp->b_list, &btp->bt_delwri_queue);
	spin_unlock(&btp->bt_delwri_lock);
}

/*
 * Move as many buffers as specified to the supplied list
 * idicating if we skipped any buffers to prevent deadlocks.
 */
STATIC int
xfs_buf_delwri_split(
	xfs_buftarg_t	*target,
	struct list_head *list,
	unsigned long	age)
{
	xfs_buf_t	*bp, *n;
	int		skipped = 0;
	int		force;

	force = test_and_clear_bit(XBT_FORCE_FLUSH, &target->bt_flags);
	INIT_LIST_HEAD(list);
	spin_lock(&target->bt_delwri_lock);
	list_for_each_entry_safe(bp, n, &target->bt_delwri_queue, b_list) {
		ASSERT(bp->b_flags & XBF_DELWRI);

		if (!xfs_buf_ispinned(bp) && xfs_buf_trylock(bp)) {
			if (!force &&
			    time_before(jiffies, bp->b_queuetime + age)) {
				xfs_buf_unlock(bp);
				break;
			}

			bp->b_flags &= ~(XBF_DELWRI | _XBF_DELWRI_Q);
			bp->b_flags |= XBF_WRITE;
			list_move_tail(&bp->b_list, list);
			trace_xfs_buf_delwri_split(bp, _RET_IP_);
		} else
			skipped++;
	}

	spin_unlock(&target->bt_delwri_lock);
	return skipped;
=======
	struct xfs_buftarg	*btp,
	unsigned int		sectorsize)
{
	/* Set up metadata sector size info */
	btp->bt_meta_sectorsize = sectorsize;
	btp->bt_meta_sectormask = sectorsize - 1;

	if (set_blocksize(btp->bt_bdev, sectorsize)) {
		xfs_warn(btp->bt_mount,
			"Cannot set_blocksize to %u on device %pg",
			sectorsize, btp->bt_bdev);
		return -EINVAL;
	}

	return 0;
}

int
xfs_init_buftarg(
	struct xfs_buftarg		*btp,
	size_t				logical_sectorsize,
	const char			*descr)
{
	/* Set up device logical sector size mask */
	btp->bt_logical_sectorsize = logical_sectorsize;
	btp->bt_logical_sectormask = logical_sectorsize - 1;

	/*
	 * Buffer IO error rate limiting. Limit it to no more than 10 messages
	 * per 30 seconds so as to not spam logs too much on repeated errors.
	 */
	ratelimit_state_init(&btp->bt_ioerror_rl, 30 * HZ,
			     DEFAULT_RATELIMIT_BURST);

	if (list_lru_init(&btp->bt_lru))
		return -ENOMEM;
	if (percpu_counter_init(&btp->bt_io_count, 0, GFP_KERNEL))
		goto out_destroy_lru;

	btp->bt_shrinker =
		shrinker_alloc(SHRINKER_NUMA_AWARE, "xfs-buf:%s", descr);
	if (!btp->bt_shrinker)
		goto out_destroy_io_count;
	btp->bt_shrinker->count_objects = xfs_buftarg_shrink_count;
	btp->bt_shrinker->scan_objects = xfs_buftarg_shrink_scan;
	btp->bt_shrinker->private_data = btp;
	shrinker_register(btp->bt_shrinker);
	return 0;

out_destroy_io_count:
	percpu_counter_destroy(&btp->bt_io_count);
out_destroy_lru:
	list_lru_destroy(&btp->bt_lru);
	return -ENOMEM;
}

struct xfs_buftarg *
xfs_alloc_buftarg(
	struct xfs_mount	*mp,
	struct file		*bdev_file)
{
	struct xfs_buftarg	*btp;
	const struct dax_holder_operations *ops = NULL;

#if defined(CONFIG_FS_DAX) && defined(CONFIG_MEMORY_FAILURE)
	ops = &xfs_dax_holder_operations;
#endif
	btp = kzalloc(sizeof(*btp), GFP_KERNEL | __GFP_NOFAIL);

	btp->bt_mount = mp;
	btp->bt_bdev_file = bdev_file;
	btp->bt_bdev = file_bdev(bdev_file);
	btp->bt_dev = btp->bt_bdev->bd_dev;
	btp->bt_daxdev = fs_dax_get_by_bdev(btp->bt_bdev, &btp->bt_dax_part_off,
					    mp, ops);

	/*
	 * When allocating the buftargs we have not yet read the super block and
	 * thus don't know the file system sector size yet.
	 */
	if (xfs_setsize_buftarg(btp, bdev_logical_block_size(btp->bt_bdev)))
		goto error_free;
	if (xfs_init_buftarg(btp, bdev_logical_block_size(btp->bt_bdev),
			mp->m_super->s_id))
		goto error_free;

	return btp;

error_free:
	kfree(btp);
	return NULL;
}

static inline void
xfs_buf_list_del(
	struct xfs_buf		*bp)
{
	list_del_init(&bp->b_list);
	wake_up_var(&bp->b_list);
}

/*
 * Cancel a delayed write list.
 *
 * Remove each buffer from the list, clear the delwri queue flag and drop the
 * associated buffer reference.
 */
void
xfs_buf_delwri_cancel(
	struct list_head	*list)
{
	struct xfs_buf		*bp;

	while (!list_empty(list)) {
		bp = list_first_entry(list, struct xfs_buf, b_list);

		xfs_buf_lock(bp);
		bp->b_flags &= ~_XBF_DELWRI_Q;
		xfs_buf_list_del(bp);
		xfs_buf_relse(bp);
	}
}

/*
 * Add a buffer to the delayed write list.
 *
 * This queues a buffer for writeout if it hasn't already been.  Note that
 * neither this routine nor the buffer list submission functions perform
 * any internal synchronization.  It is expected that the lists are thread-local
 * to the callers.
 *
 * Returns true if we queued up the buffer, or false if it already had
 * been on the buffer list.
 */
bool
xfs_buf_delwri_queue(
	struct xfs_buf		*bp,
	struct list_head	*list)
{
	ASSERT(xfs_buf_islocked(bp));
	ASSERT(!(bp->b_flags & XBF_READ));

	/*
	 * If the buffer is already marked delwri it already is queued up
	 * by someone else for imediate writeout.  Just ignore it in that
	 * case.
	 */
	if (bp->b_flags & _XBF_DELWRI_Q) {
		trace_xfs_buf_delwri_queued(bp, _RET_IP_);
		return false;
	}

	trace_xfs_buf_delwri_queue(bp, _RET_IP_);

	/*
	 * If a buffer gets written out synchronously or marked stale while it
	 * is on a delwri list we lazily remove it. To do this, the other party
	 * clears the  _XBF_DELWRI_Q flag but otherwise leaves the buffer alone.
	 * It remains referenced and on the list.  In a rare corner case it
	 * might get readded to a delwri list after the synchronous writeout, in
	 * which case we need just need to re-add the flag here.
	 */
	bp->b_flags |= _XBF_DELWRI_Q;
	if (list_empty(&bp->b_list)) {
		atomic_inc(&bp->b_hold);
		list_add_tail(&bp->b_list, list);
	}

	return true;
}

/*
 * Queue a buffer to this delwri list as part of a data integrity operation.
 * If the buffer is on any other delwri list, we'll wait for that to clear
 * so that the caller can submit the buffer for IO and wait for the result.
 * Callers must ensure the buffer is not already on the list.
 */
void
xfs_buf_delwri_queue_here(
	struct xfs_buf		*bp,
	struct list_head	*buffer_list)
{
	/*
	 * We need this buffer to end up on the /caller's/ delwri list, not any
	 * old list.  This can happen if the buffer is marked stale (which
	 * clears DELWRI_Q) after the AIL queues the buffer to its list but
	 * before the AIL has a chance to submit the list.
	 */
	while (!list_empty(&bp->b_list)) {
		xfs_buf_unlock(bp);
		wait_var_event(&bp->b_list, list_empty(&bp->b_list));
		xfs_buf_lock(bp);
	}

	ASSERT(!(bp->b_flags & _XBF_DELWRI_Q));

	xfs_buf_delwri_queue(bp, buffer_list);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

/*
 * Compare function is more complex than it needs to be because
 * the return value is only 32 bits and we are doing comparisons
 * on 64 bit values
 */
static int
xfs_buf_cmp(
<<<<<<< HEAD
	void		*priv,
	struct list_head *a,
	struct list_head *b)
=======
	void			*priv,
	const struct list_head	*a,
	const struct list_head	*b)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
{
	struct xfs_buf	*ap = container_of(a, struct xfs_buf, b_list);
	struct xfs_buf	*bp = container_of(b, struct xfs_buf, b_list);
	xfs_daddr_t		diff;

<<<<<<< HEAD
	diff = ap->b_bn - bp->b_bn;
=======
	diff = ap->b_maps[0].bm_bn - bp->b_maps[0].bm_bn;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	if (diff < 0)
		return -1;
	if (diff > 0)
		return 1;
	return 0;
}

<<<<<<< HEAD
STATIC int
xfsbufd(
	void		*data)
{
	xfs_buftarg_t   *target = (xfs_buftarg_t *)data;

	current->flags |= PF_MEMALLOC;

	set_freezable();

	do {
		long	age = xfs_buf_age_centisecs * msecs_to_jiffies(10);
		long	tout = xfs_buf_timer_centisecs * msecs_to_jiffies(10);
		struct list_head tmp;
		struct blk_plug plug;

		if (unlikely(freezing(current)))
			try_to_freeze();

		/* sleep for a long time if there is nothing to do. */
		if (list_empty(&target->bt_delwri_queue))
			tout = MAX_SCHEDULE_TIMEOUT;
		schedule_timeout_interruptible(tout);

		xfs_buf_delwri_split(target, &tmp, age);
		list_sort(NULL, &tmp, xfs_buf_cmp);

		blk_start_plug(&plug);
		while (!list_empty(&tmp)) {
			struct xfs_buf *bp;
			bp = list_first_entry(&tmp, struct xfs_buf, b_list);
			list_del_init(&bp->b_list);
			xfs_bdstrat_cb(bp);
		}
		blk_finish_plug(&plug);
	} while (!kthread_should_stop());

	return 0;
}

/*
 *	Go through all incore buffers, and release buffers if they belong to
 *	the given device. This is used in filesystem error handling to
 *	preserve the consistency of its metadata.
 */
int
xfs_flush_buftarg(
	xfs_buftarg_t	*target,
	int		wait)
{
	xfs_buf_t	*bp;
	int		pincount = 0;
	LIST_HEAD(tmp_list);
	LIST_HEAD(wait_list);
	struct blk_plug plug;

	flush_workqueue(xfslogd_workqueue);

	set_bit(XBT_FORCE_FLUSH, &target->bt_flags);
	pincount = xfs_buf_delwri_split(target, &tmp_list, 0);

	/*
	 * Dropped the delayed write list lock, now walk the temporary list.
	 * All I/O is issued async and then if we need to wait for completion
	 * we do that after issuing all the IO.
	 */
	list_sort(NULL, &tmp_list, xfs_buf_cmp);

	blk_start_plug(&plug);
	while (!list_empty(&tmp_list)) {
		bp = list_first_entry(&tmp_list, struct xfs_buf, b_list);
		ASSERT(target == bp->b_target);
		list_del_init(&bp->b_list);
		if (wait) {
			bp->b_flags &= ~XBF_ASYNC;
			list_add(&bp->b_list, &wait_list);
		}
		xfs_bdstrat_cb(bp);
	}
	blk_finish_plug(&plug);

	if (wait) {
		/* Wait for IO to complete. */
		while (!list_empty(&wait_list)) {
			bp = list_first_entry(&wait_list, struct xfs_buf, b_list);

			list_del_init(&bp->b_list);
			xfs_buf_iowait(bp);
			xfs_buf_relse(bp);
		}
	}

	return pincount;
}

int __init
xfs_buf_init(void)
{
	xfs_buf_zone = kmem_zone_init_flags(sizeof(xfs_buf_t), "xfs_buf",
						KM_ZONE_HWALIGN, NULL);
	if (!xfs_buf_zone)
		goto out;

	xfslogd_workqueue = alloc_workqueue("xfslogd",
					WQ_MEM_RECLAIM | WQ_HIGHPRI, 1);
	if (!xfslogd_workqueue)
		goto out_free_buf_zone;

	return 0;

 out_free_buf_zone:
	kmem_zone_destroy(xfs_buf_zone);
 out:
	return -ENOMEM;
}

void
xfs_buf_terminate(void)
{
	destroy_workqueue(xfslogd_workqueue);
	kmem_zone_destroy(xfs_buf_zone);
=======
/*
 * Submit buffers for write. If wait_list is specified, the buffers are
 * submitted using sync I/O and placed on the wait list such that the caller can
 * iowait each buffer. Otherwise async I/O is used and the buffers are released
 * at I/O completion time. In either case, buffers remain locked until I/O
 * completes and the buffer is released from the queue.
 */
static int
xfs_buf_delwri_submit_buffers(
	struct list_head	*buffer_list,
	struct list_head	*wait_list)
{
	struct xfs_buf		*bp, *n;
	int			pinned = 0;
	struct blk_plug		plug;

	list_sort(NULL, buffer_list, xfs_buf_cmp);

	blk_start_plug(&plug);
	list_for_each_entry_safe(bp, n, buffer_list, b_list) {
		if (!wait_list) {
			if (!xfs_buf_trylock(bp))
				continue;
			if (xfs_buf_ispinned(bp)) {
				xfs_buf_unlock(bp);
				pinned++;
				continue;
			}
		} else {
			xfs_buf_lock(bp);
		}

		/*
		 * Someone else might have written the buffer synchronously or
		 * marked it stale in the meantime.  In that case only the
		 * _XBF_DELWRI_Q flag got cleared, and we have to drop the
		 * reference and remove it from the list here.
		 */
		if (!(bp->b_flags & _XBF_DELWRI_Q)) {
			xfs_buf_list_del(bp);
			xfs_buf_relse(bp);
			continue;
		}

		trace_xfs_buf_delwri_split(bp, _RET_IP_);

		/*
		 * If we have a wait list, each buffer (and associated delwri
		 * queue reference) transfers to it and is submitted
		 * synchronously. Otherwise, drop the buffer from the delwri
		 * queue and submit async.
		 */
		bp->b_flags &= ~_XBF_DELWRI_Q;
		bp->b_flags |= XBF_WRITE;
		if (wait_list) {
			bp->b_flags &= ~XBF_ASYNC;
			list_move_tail(&bp->b_list, wait_list);
		} else {
			bp->b_flags |= XBF_ASYNC;
			xfs_buf_list_del(bp);
		}
		__xfs_buf_submit(bp, false);
	}
	blk_finish_plug(&plug);

	return pinned;
}

/*
 * Write out a buffer list asynchronously.
 *
 * This will take the @buffer_list, write all non-locked and non-pinned buffers
 * out and not wait for I/O completion on any of the buffers.  This interface
 * is only safely useable for callers that can track I/O completion by higher
 * level means, e.g. AIL pushing as the @buffer_list is consumed in this
 * function.
 *
 * Note: this function will skip buffers it would block on, and in doing so
 * leaves them on @buffer_list so they can be retried on a later pass. As such,
 * it is up to the caller to ensure that the buffer list is fully submitted or
 * cancelled appropriately when they are finished with the list. Failure to
 * cancel or resubmit the list until it is empty will result in leaked buffers
 * at unmount time.
 */
int
xfs_buf_delwri_submit_nowait(
	struct list_head	*buffer_list)
{
	return xfs_buf_delwri_submit_buffers(buffer_list, NULL);
}

/*
 * Write out a buffer list synchronously.
 *
 * This will take the @buffer_list, write all buffers out and wait for I/O
 * completion on all of the buffers. @buffer_list is consumed by the function,
 * so callers must have some other way of tracking buffers if they require such
 * functionality.
 */
int
xfs_buf_delwri_submit(
	struct list_head	*buffer_list)
{
	LIST_HEAD		(wait_list);
	int			error = 0, error2;
	struct xfs_buf		*bp;

	xfs_buf_delwri_submit_buffers(buffer_list, &wait_list);

	/* Wait for IO to complete. */
	while (!list_empty(&wait_list)) {
		bp = list_first_entry(&wait_list, struct xfs_buf, b_list);

		xfs_buf_list_del(bp);

		/*
		 * Wait on the locked buffer, check for errors and unlock and
		 * release the delwri queue reference.
		 */
		error2 = xfs_buf_iowait(bp);
		xfs_buf_relse(bp);
		if (!error)
			error = error2;
	}

	return error;
}

/*
 * Push a single buffer on a delwri queue.
 *
 * The purpose of this function is to submit a single buffer of a delwri queue
 * and return with the buffer still on the original queue. The waiting delwri
 * buffer submission infrastructure guarantees transfer of the delwri queue
 * buffer reference to a temporary wait list. We reuse this infrastructure to
 * transfer the buffer back to the original queue.
 *
 * Note the buffer transitions from the queued state, to the submitted and wait
 * listed state and back to the queued state during this call. The buffer
 * locking and queue management logic between _delwri_pushbuf() and
 * _delwri_queue() guarantee that the buffer cannot be queued to another list
 * before returning.
 */
int
xfs_buf_delwri_pushbuf(
	struct xfs_buf		*bp,
	struct list_head	*buffer_list)
{
	LIST_HEAD		(submit_list);
	int			error;

	ASSERT(bp->b_flags & _XBF_DELWRI_Q);

	trace_xfs_buf_delwri_pushbuf(bp, _RET_IP_);

	/*
	 * Isolate the buffer to a new local list so we can submit it for I/O
	 * independently from the rest of the original list.
	 */
	xfs_buf_lock(bp);
	list_move(&bp->b_list, &submit_list);
	xfs_buf_unlock(bp);

	/*
	 * Delwri submission clears the DELWRI_Q buffer flag and returns with
	 * the buffer on the wait list with the original reference. Rather than
	 * bounce the buffer from a local wait list back to the original list
	 * after I/O completion, reuse the original list as the wait list.
	 */
	xfs_buf_delwri_submit_buffers(&submit_list, buffer_list);

	/*
	 * The buffer is now locked, under I/O and wait listed on the original
	 * delwri queue. Wait for I/O completion, restore the DELWRI_Q flag and
	 * return with the buffer unlocked and on the original queue.
	 */
	error = xfs_buf_iowait(bp);
	bp->b_flags |= _XBF_DELWRI_Q;
	xfs_buf_unlock(bp);

	return error;
}

void xfs_buf_set_ref(struct xfs_buf *bp, int lru_ref)
{
	/*
	 * Set the lru reference count to 0 based on the error injection tag.
	 * This allows userspace to disrupt buffer caching for debug/testing
	 * purposes.
	 */
	if (XFS_TEST_ERROR(false, bp->b_mount, XFS_ERRTAG_BUF_LRU_REF))
		lru_ref = 0;

	atomic_set(&bp->b_lru_ref, lru_ref);
}

/*
 * Verify an on-disk magic value against the magic value specified in the
 * verifier structure. The verifier magic is in disk byte order so the caller is
 * expected to pass the value directly from disk.
 */
bool
xfs_verify_magic(
	struct xfs_buf		*bp,
	__be32			dmagic)
{
	struct xfs_mount	*mp = bp->b_mount;
	int			idx;

	idx = xfs_has_crc(mp);
	if (WARN_ON(!bp->b_ops || !bp->b_ops->magic[idx]))
		return false;
	return dmagic == bp->b_ops->magic[idx];
}
/*
 * Verify an on-disk magic value against the magic value specified in the
 * verifier structure. The verifier magic is in disk byte order so the caller is
 * expected to pass the value directly from disk.
 */
bool
xfs_verify_magic16(
	struct xfs_buf		*bp,
	__be16			dmagic)
{
	struct xfs_mount	*mp = bp->b_mount;
	int			idx;

	idx = xfs_has_crc(mp);
	if (WARN_ON(!bp->b_ops || !bp->b_ops->magic16[idx]))
		return false;
	return dmagic == bp->b_ops->magic16[idx];
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}
