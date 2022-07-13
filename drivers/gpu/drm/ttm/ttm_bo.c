<<<<<<< HEAD
=======
/* SPDX-License-Identifier: GPL-2.0 OR MIT */
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
/**************************************************************************
 *
 * Copyright (c) 2006-2009 VMware, Inc., Palo Alto, CA., USA
 * All Rights Reserved.
 *
 * Permission is hereby granted, free of charge, to any person obtaining a
 * copy of this software and associated documentation files (the
 * "Software"), to deal in the Software without restriction, including
 * without limitation the rights to use, copy, modify, merge, publish,
 * distribute, sub license, and/or sell copies of the Software, and to
 * permit persons to whom the Software is furnished to do so, subject to
 * the following conditions:
 *
 * The above copyright notice and this permission notice (including the
 * next paragraph) shall be included in all copies or substantial portions
 * of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NON-INFRINGEMENT. IN NO EVENT SHALL
 * THE COPYRIGHT HOLDERS, AUTHORS AND/OR ITS SUPPLIERS BE LIABLE FOR ANY CLAIM,
 * DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR
 * OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE
 * USE OR OTHER DEALINGS IN THE SOFTWARE.
 *
 **************************************************************************/
/*
 * Authors: Thomas Hellstrom <thellstrom-at-vmware-dot-com>
 */

#define pr_fmt(fmt) "[TTM] " fmt

<<<<<<< HEAD
#include "ttm/ttm_module.h"
#include "ttm/ttm_bo_driver.h"
#include "ttm/ttm_placement.h"
=======
#include <drm/ttm/ttm_bo.h>
#include <drm/ttm/ttm_placement.h>
#include <drm/ttm/ttm_tt.h>

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#include <linux/jiffies.h>
#include <linux/slab.h>
#include <linux/sched.h>
#include <linux/mm.h>
#include <linux/file.h>
#include <linux/module.h>
#include <linux/atomic.h>
<<<<<<< HEAD

#define TTM_ASSERT_LOCKED(param)
#define TTM_DEBUG(fmt, arg...)
#define TTM_BO_HASH_ORDER 13

static int ttm_bo_setup_vm(struct ttm_buffer_object *bo);
static int ttm_bo_swapout(struct ttm_mem_shrink *shrink);
static void ttm_bo_global_kobj_release(struct kobject *kobj);

static struct attribute ttm_bo_count = {
	.name = "bo_count",
	.mode = S_IRUGO
};

static inline int ttm_mem_type_from_flags(uint32_t flags, uint32_t *mem_type)
{
	int i;

	for (i = 0; i <= TTM_PL_PRIV5; i++)
		if (flags & (1 << i)) {
			*mem_type = i;
			return 0;
		}
	return -EINVAL;
}

static void ttm_mem_type_debug(struct ttm_bo_device *bdev, int mem_type)
{
	struct ttm_mem_type_manager *man = &bdev->man[mem_type];

	pr_err("    has_type: %d\n", man->has_type);
	pr_err("    use_type: %d\n", man->use_type);
	pr_err("    flags: 0x%08X\n", man->flags);
	pr_err("    gpu_offset: 0x%08lX\n", man->gpu_offset);
	pr_err("    size: %llu\n", man->size);
	pr_err("    available_caching: 0x%08X\n", man->available_caching);
	pr_err("    default_caching: 0x%08X\n", man->default_caching);
	if (mem_type != TTM_PL_SYSTEM)
		(*man->func->debug)(man, TTM_PFX);
}
=======
#include <linux/dma-resv.h>

#include "ttm_module.h"
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

static void ttm_bo_mem_space_debug(struct ttm_buffer_object *bo,
					struct ttm_placement *placement)
{
<<<<<<< HEAD
	int i, ret, mem_type;

	pr_err("No space for %p (%lu pages, %luK, %luM)\n",
	       bo, bo->mem.num_pages, bo->mem.size >> 10,
	       bo->mem.size >> 20);
	for (i = 0; i < placement->num_placement; i++) {
		ret = ttm_mem_type_from_flags(placement->placement[i],
						&mem_type);
		if (ret)
			return;
		pr_err("  placement[%d]=0x%08X (%d)\n",
		       i, placement->placement[i], mem_type);
		ttm_mem_type_debug(bo->bdev, mem_type);
	}
}

static ssize_t ttm_bo_global_show(struct kobject *kobj,
				  struct attribute *attr,
				  char *buffer)
{
	struct ttm_bo_global *glob =
		container_of(kobj, struct ttm_bo_global, kobj);

	return snprintf(buffer, PAGE_SIZE, "%lu\n",
			(unsigned long) atomic_read(&glob->bo_count));
}

static struct attribute *ttm_bo_global_attrs[] = {
	&ttm_bo_count,
	NULL
};

static const struct sysfs_ops ttm_bo_global_ops = {
	.show = &ttm_bo_global_show
};

static struct kobj_type ttm_bo_glob_kobj_type  = {
	.release = &ttm_bo_global_kobj_release,
	.sysfs_ops = &ttm_bo_global_ops,
	.default_attrs = ttm_bo_global_attrs
};


static inline uint32_t ttm_bo_type_flags(unsigned type)
{
	return 1 << (type);
}

static void ttm_bo_release_list(struct kref *list_kref)
{
	struct ttm_buffer_object *bo =
	    container_of(list_kref, struct ttm_buffer_object, list_kref);
	struct ttm_bo_device *bdev = bo->bdev;
	size_t acc_size = bo->acc_size;

	BUG_ON(atomic_read(&bo->list_kref.refcount));
	BUG_ON(atomic_read(&bo->kref.refcount));
	BUG_ON(atomic_read(&bo->cpu_writers));
	BUG_ON(bo->sync_obj != NULL);
	BUG_ON(bo->mem.mm_node != NULL);
	BUG_ON(!list_empty(&bo->lru));
	BUG_ON(!list_empty(&bo->ddestroy));

	if (bo->ttm)
		ttm_tt_destroy(bo->ttm);
	atomic_dec(&bo->glob->bo_count);
	if (bo->destroy)
		bo->destroy(bo);
	else {
		kfree(bo);
	}
	ttm_mem_global_free(bdev->glob->mem_glob, acc_size);
}

int ttm_bo_wait_unreserved(struct ttm_buffer_object *bo, bool interruptible)
{
	if (interruptible) {
		return wait_event_interruptible(bo->event_queue,
					       atomic_read(&bo->reserved) == 0);
	} else {
		wait_event(bo->event_queue, atomic_read(&bo->reserved) == 0);
		return 0;
	}
}
EXPORT_SYMBOL(ttm_bo_wait_unreserved);

void ttm_bo_add_to_lru(struct ttm_buffer_object *bo)
{
	struct ttm_bo_device *bdev = bo->bdev;
	struct ttm_mem_type_manager *man;

	BUG_ON(!atomic_read(&bo->reserved));

	if (!(bo->mem.placement & TTM_PL_FLAG_NO_EVICT)) {

		BUG_ON(!list_empty(&bo->lru));

		man = &bdev->man[bo->mem.mem_type];
		list_add_tail(&bo->lru, &man->lru);
		kref_get(&bo->list_kref);

		if (bo->ttm != NULL) {
			list_add_tail(&bo->swap, &bo->glob->swap_lru);
			kref_get(&bo->list_kref);
		}
	}
}

int ttm_bo_del_from_lru(struct ttm_buffer_object *bo)
{
	int put_count = 0;

	if (!list_empty(&bo->swap)) {
		list_del_init(&bo->swap);
		++put_count;
	}
	if (!list_empty(&bo->lru)) {
		list_del_init(&bo->lru);
		++put_count;
	}

	/*
	 * TODO: Add a driver hook to delete from
	 * driver-specific LRU's here.
	 */

	return put_count;
}

int ttm_bo_reserve_locked(struct ttm_buffer_object *bo,
			  bool interruptible,
			  bool no_wait, bool use_sequence, uint32_t sequence)
{
	struct ttm_bo_global *glob = bo->glob;
	int ret;

	while (unlikely(atomic_cmpxchg(&bo->reserved, 0, 1) != 0)) {
		/**
		 * Deadlock avoidance for multi-bo reserving.
		 */
		if (use_sequence && bo->seq_valid) {
			/**
			 * We've already reserved this one.
			 */
			if (unlikely(sequence == bo->val_seq))
				return -EDEADLK;
			/**
			 * Already reserved by a thread that will not back
			 * off for us. We need to back off.
			 */
			if (unlikely(sequence - bo->val_seq < (1 << 31)))
				return -EAGAIN;
		}

		if (no_wait)
			return -EBUSY;

		spin_unlock(&glob->lru_lock);
		ret = ttm_bo_wait_unreserved(bo, interruptible);
		spin_lock(&glob->lru_lock);

		if (unlikely(ret))
			return ret;
	}

	if (use_sequence) {
		/**
		 * Wake up waiters that may need to recheck for deadlock,
		 * if we decreased the sequence number.
		 */
		if (unlikely((bo->val_seq - sequence < (1 << 31))
			     || !bo->seq_valid))
			wake_up_all(&bo->event_queue);

		bo->val_seq = sequence;
		bo->seq_valid = true;
	} else {
		bo->seq_valid = false;
	}

	return 0;
}
EXPORT_SYMBOL(ttm_bo_reserve);

static void ttm_bo_ref_bug(struct kref *list_kref)
{
	BUG();
}

void ttm_bo_list_ref_sub(struct ttm_buffer_object *bo, int count,
			 bool never_free)
{
	kref_sub(&bo->list_kref, count,
		 (never_free) ? ttm_bo_ref_bug : ttm_bo_release_list);
}

int ttm_bo_reserve(struct ttm_buffer_object *bo,
		   bool interruptible,
		   bool no_wait, bool use_sequence, uint32_t sequence)
{
	struct ttm_bo_global *glob = bo->glob;
	int put_count = 0;
	int ret;

	spin_lock(&glob->lru_lock);
	ret = ttm_bo_reserve_locked(bo, interruptible, no_wait, use_sequence,
				    sequence);
	if (likely(ret == 0))
		put_count = ttm_bo_del_from_lru(bo);
	spin_unlock(&glob->lru_lock);

	ttm_bo_list_ref_sub(bo, put_count, true);

	return ret;
}

void ttm_bo_unreserve_locked(struct ttm_buffer_object *bo)
{
	ttm_bo_add_to_lru(bo);
	atomic_set(&bo->reserved, 0);
	wake_up_all(&bo->event_queue);
}

void ttm_bo_unreserve(struct ttm_buffer_object *bo)
{
	struct ttm_bo_global *glob = bo->glob;

	spin_lock(&glob->lru_lock);
	ttm_bo_unreserve_locked(bo);
	spin_unlock(&glob->lru_lock);
}
EXPORT_SYMBOL(ttm_bo_unreserve);

/*
 * Call bo->mutex locked.
 */
static int ttm_bo_add_ttm(struct ttm_buffer_object *bo, bool zero_alloc)
{
	struct ttm_bo_device *bdev = bo->bdev;
	struct ttm_bo_global *glob = bo->glob;
	int ret = 0;
	uint32_t page_flags = 0;

	TTM_ASSERT_LOCKED(&bo->mutex);
	bo->ttm = NULL;

	if (bdev->need_dma32)
		page_flags |= TTM_PAGE_FLAG_DMA32;

	switch (bo->type) {
	case ttm_bo_type_device:
		if (zero_alloc)
			page_flags |= TTM_PAGE_FLAG_ZERO_ALLOC;
	case ttm_bo_type_kernel:
		bo->ttm = bdev->driver->ttm_tt_create(bdev, bo->num_pages << PAGE_SHIFT,
						      page_flags, glob->dummy_read_page);
		if (unlikely(bo->ttm == NULL))
			ret = -ENOMEM;
		break;
	default:
		pr_err("Illegal buffer object type\n");
		ret = -EINVAL;
		break;
	}

	return ret;
}

static int ttm_bo_handle_move_mem(struct ttm_buffer_object *bo,
				  struct ttm_mem_reg *mem,
				  bool evict, bool interruptible,
				  bool no_wait_reserve, bool no_wait_gpu)
{
	struct ttm_bo_device *bdev = bo->bdev;
	bool old_is_pci = ttm_mem_reg_is_pci(bdev, &bo->mem);
	bool new_is_pci = ttm_mem_reg_is_pci(bdev, mem);
	struct ttm_mem_type_manager *old_man = &bdev->man[bo->mem.mem_type];
	struct ttm_mem_type_manager *new_man = &bdev->man[mem->mem_type];
	int ret = 0;

	if (old_is_pci || new_is_pci ||
	    ((mem->placement & bo->mem.placement & TTM_PL_MASK_CACHING) == 0)) {
		ret = ttm_mem_io_lock(old_man, true);
		if (unlikely(ret != 0))
			goto out_err;
		ttm_bo_unmap_virtual_locked(bo);
		ttm_mem_io_unlock(old_man);
	}
=======
	struct drm_printer p = drm_dbg_printer(NULL, DRM_UT_CORE, TTM_PFX);
	struct ttm_resource_manager *man;
	int i, mem_type;

	for (i = 0; i < placement->num_placement; i++) {
		mem_type = placement->placement[i].mem_type;
		drm_printf(&p, "  placement[%d]=0x%08X (%d)\n",
			   i, placement->placement[i].flags, mem_type);
		man = ttm_manager_type(bo->bdev, mem_type);
		ttm_resource_manager_debug(man, &p);
	}
}

/**
 * ttm_bo_move_to_lru_tail
 *
 * @bo: The buffer object.
 *
 * Move this BO to the tail of all lru lists used to lookup and reserve an
 * object. This function must be called with struct ttm_global::lru_lock
 * held, and is used to make a BO less likely to be considered for eviction.
 */
void ttm_bo_move_to_lru_tail(struct ttm_buffer_object *bo)
{
	dma_resv_assert_held(bo->base.resv);

	if (bo->resource)
		ttm_resource_move_to_lru_tail(bo->resource);
}
EXPORT_SYMBOL(ttm_bo_move_to_lru_tail);

/**
 * ttm_bo_set_bulk_move - update BOs bulk move object
 *
 * @bo: The buffer object.
 * @bulk: bulk move structure
 *
 * Update the BOs bulk move object, making sure that resources are added/removed
 * as well. A bulk move allows to move many resource on the LRU at once,
 * resulting in much less overhead of maintaining the LRU.
 * The only requirement is that the resources stay together on the LRU and are
 * never separated. This is enforces by setting the bulk_move structure on a BO.
 * ttm_lru_bulk_move_tail() should be used to move all resources to the tail of
 * their LRU list.
 */
void ttm_bo_set_bulk_move(struct ttm_buffer_object *bo,
			  struct ttm_lru_bulk_move *bulk)
{
	dma_resv_assert_held(bo->base.resv);

	if (bo->bulk_move == bulk)
		return;

	spin_lock(&bo->bdev->lru_lock);
	if (bo->resource)
		ttm_resource_del_bulk_move(bo->resource, bo);
	bo->bulk_move = bulk;
	if (bo->resource)
		ttm_resource_add_bulk_move(bo->resource, bo);
	spin_unlock(&bo->bdev->lru_lock);
}
EXPORT_SYMBOL(ttm_bo_set_bulk_move);

static int ttm_bo_handle_move_mem(struct ttm_buffer_object *bo,
				  struct ttm_resource *mem, bool evict,
				  struct ttm_operation_ctx *ctx,
				  struct ttm_place *hop)
{
	struct ttm_device *bdev = bo->bdev;
	bool old_use_tt, new_use_tt;
	int ret;

	old_use_tt = !bo->resource || ttm_manager_type(bdev, bo->resource->mem_type)->use_tt;
	new_use_tt = ttm_manager_type(bdev, mem->mem_type)->use_tt;

	ttm_bo_unmap_virtual(bo);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	/*
	 * Create and bind a ttm if required.
	 */

<<<<<<< HEAD
	if (!(new_man->flags & TTM_MEMTYPE_FLAG_FIXED)) {
		if (bo->ttm == NULL) {
			bool zero = !(old_man->flags & TTM_MEMTYPE_FLAG_FIXED);
			ret = ttm_bo_add_ttm(bo, zero);
			if (ret)
				goto out_err;
		}

		ret = ttm_tt_set_placement_caching(bo->ttm, mem->placement);
=======
	if (new_use_tt) {
		/* Zero init the new TTM structure if the old location should
		 * have used one as well.
		 */
		ret = ttm_tt_create(bo, old_use_tt);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		if (ret)
			goto out_err;

		if (mem->mem_type != TTM_PL_SYSTEM) {
<<<<<<< HEAD
			ret = ttm_tt_bind(bo->ttm, mem);
			if (ret)
				goto out_err;
		}

		if (bo->mem.mem_type == TTM_PL_SYSTEM) {
			if (bdev->driver->move_notify)
				bdev->driver->move_notify(bo, mem);
			bo->mem = *mem;
			mem->mm_node = NULL;
			goto moved;
		}
	}

	if (bdev->driver->move_notify)
		bdev->driver->move_notify(bo, mem);

	if (!(old_man->flags & TTM_MEMTYPE_FLAG_FIXED) &&
	    !(new_man->flags & TTM_MEMTYPE_FLAG_FIXED))
		ret = ttm_bo_move_ttm(bo, evict, no_wait_reserve, no_wait_gpu, mem);
	else if (bdev->driver->move)
		ret = bdev->driver->move(bo, evict, interruptible,
					 no_wait_reserve, no_wait_gpu, mem);
	else
		ret = ttm_bo_move_memcpy(bo, evict, no_wait_reserve, no_wait_gpu, mem);

	if (ret) {
		if (bdev->driver->move_notify) {
			struct ttm_mem_reg tmp_mem = *mem;
			*mem = bo->mem;
			bo->mem = tmp_mem;
			bdev->driver->move_notify(bo, mem);
			bo->mem = *mem;
		}

		goto out_err;
	}

moved:
	if (bo->evicted) {
		if (bdev->driver->invalidate_caches) {
			ret = bdev->driver->invalidate_caches(bdev, bo->mem.placement);
			if (ret)
				pr_err("Can not flush read caches\n");
		}
		bo->evicted = false;
	}

	if (bo->mem.mm_node) {
		bo->offset = (bo->mem.start << PAGE_SHIFT) +
		    bdev->man[bo->mem.mem_type].gpu_offset;
		bo->cur_placement = bo->mem.placement;
	} else
		bo->offset = 0;

	return 0;

out_err:
	new_man = &bdev->man[bo->mem.mem_type];
	if ((new_man->flags & TTM_MEMTYPE_FLAG_FIXED) && bo->ttm) {
		ttm_tt_unbind(bo->ttm);
		ttm_tt_destroy(bo->ttm);
		bo->ttm = NULL;
	}
=======
			ret = ttm_tt_populate(bo->bdev, bo->ttm, ctx);
			if (ret)
				goto out_err;
		}
	}

	ret = dma_resv_reserve_fences(bo->base.resv, 1);
	if (ret)
		goto out_err;

	ret = bdev->funcs->move(bo, evict, ctx, mem, hop);
	if (ret) {
		if (ret == -EMULTIHOP)
			return ret;
		goto out_err;
	}

	ctx->bytes_moved += bo->base.size;
	return 0;

out_err:
	if (!old_use_tt)
		ttm_bo_tt_destroy(bo);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	return ret;
}

<<<<<<< HEAD
/**
=======
/*
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 * Call bo::reserved.
 * Will release GPU memory type usage on destruction.
 * This is the place to put in driver specific hooks to release
 * driver private resources.
 * Will release the bo::reserved lock.
 */

static void ttm_bo_cleanup_memtype_use(struct ttm_buffer_object *bo)
{
<<<<<<< HEAD
	if (bo->bdev->driver->move_notify)
		bo->bdev->driver->move_notify(bo, NULL);

	if (bo->ttm) {
		ttm_tt_unbind(bo->ttm);
		ttm_tt_destroy(bo->ttm);
		bo->ttm = NULL;
	}
	ttm_bo_mem_put(bo, &bo->mem);

	atomic_set(&bo->reserved, 0);

	/*
	 * Make processes trying to reserve really pick it up.
	 */
	smp_mb__after_atomic_dec();
	wake_up_all(&bo->event_queue);
}

static void ttm_bo_cleanup_refs_or_queue(struct ttm_buffer_object *bo)
{
	struct ttm_bo_device *bdev = bo->bdev;
	struct ttm_bo_global *glob = bo->glob;
	struct ttm_bo_driver *driver;
	void *sync_obj = NULL;
	void *sync_obj_arg;
	int put_count;
	int ret;

	spin_lock(&bdev->fence_lock);
	(void) ttm_bo_wait(bo, false, false, true);
	if (!bo->sync_obj) {

		spin_lock(&glob->lru_lock);

		/**
		 * Lock inversion between bo:reserve and bdev::fence_lock here,
		 * but that's OK, since we're only trylocking.
		 */

		ret = ttm_bo_reserve_locked(bo, false, true, false, 0);

		if (unlikely(ret == -EBUSY))
			goto queue;

		spin_unlock(&bdev->fence_lock);
		put_count = ttm_bo_del_from_lru(bo);

		spin_unlock(&glob->lru_lock);
		ttm_bo_cleanup_memtype_use(bo);

		ttm_bo_list_ref_sub(bo, put_count, true);

		return;
	} else {
		spin_lock(&glob->lru_lock);
	}
queue:
	driver = bdev->driver;
	if (bo->sync_obj)
		sync_obj = driver->sync_obj_ref(bo->sync_obj);
	sync_obj_arg = bo->sync_obj_arg;

	kref_get(&bo->list_kref);
	list_add_tail(&bo->ddestroy, &bdev->ddestroy);
	spin_unlock(&glob->lru_lock);
	spin_unlock(&bdev->fence_lock);

	if (sync_obj) {
		driver->sync_obj_flush(sync_obj, sync_obj_arg);
		driver->sync_obj_unref(&sync_obj);
	}
	schedule_delayed_work(&bdev->wq,
			      ((HZ / 100) < 1) ? 1 : HZ / 100);
}

/**
 * function ttm_bo_cleanup_refs
 * If bo idle, remove from delayed- and lru lists, and unref.
 * If not idle, do nothing.
 *
 * @interruptible         Any sleeps should occur interruptibly.
 * @no_wait_reserve       Never wait for reserve. Return -EBUSY instead.
 * @no_wait_gpu           Never wait for gpu. Return -EBUSY instead.
 */

static int ttm_bo_cleanup_refs(struct ttm_buffer_object *bo,
			       bool interruptible,
			       bool no_wait_reserve,
			       bool no_wait_gpu)
{
	struct ttm_bo_device *bdev = bo->bdev;
	struct ttm_bo_global *glob = bo->glob;
	int put_count;
	int ret = 0;

retry:
	spin_lock(&bdev->fence_lock);
	ret = ttm_bo_wait(bo, false, interruptible, no_wait_gpu);
	spin_unlock(&bdev->fence_lock);

	if (unlikely(ret != 0))
		return ret;

	spin_lock(&glob->lru_lock);

	if (unlikely(list_empty(&bo->ddestroy))) {
		spin_unlock(&glob->lru_lock);
		return 0;
	}

	ret = ttm_bo_reserve_locked(bo, interruptible,
				    no_wait_reserve, false, 0);

	if (unlikely(ret != 0)) {
		spin_unlock(&glob->lru_lock);
		return ret;
	}

	/**
	 * We can re-check for sync object without taking
	 * the bo::lock since setting the sync object requires
	 * also bo::reserved. A busy object at this point may
	 * be caused by another thread recently starting an accelerated
	 * eviction.
	 */

	if (unlikely(bo->sync_obj)) {
		atomic_set(&bo->reserved, 0);
		wake_up_all(&bo->event_queue);
		spin_unlock(&glob->lru_lock);
		goto retry;
	}

	put_count = ttm_bo_del_from_lru(bo);
	list_del_init(&bo->ddestroy);
	++put_count;

	spin_unlock(&glob->lru_lock);
	ttm_bo_cleanup_memtype_use(bo);

	ttm_bo_list_ref_sub(bo, put_count, true);
=======
	if (bo->bdev->funcs->delete_mem_notify)
		bo->bdev->funcs->delete_mem_notify(bo);

	ttm_bo_tt_destroy(bo);
	ttm_resource_free(bo, &bo->resource);
}

static int ttm_bo_individualize_resv(struct ttm_buffer_object *bo)
{
	int r;

	if (bo->base.resv == &bo->base._resv)
		return 0;

	BUG_ON(!dma_resv_trylock(&bo->base._resv));

	r = dma_resv_copy_fences(&bo->base._resv, bo->base.resv);
	dma_resv_unlock(&bo->base._resv);
	if (r)
		return r;

	if (bo->type != ttm_bo_type_sg) {
		/* This works because the BO is about to be destroyed and nobody
		 * reference it any more. The only tricky case is the trylock on
		 * the resv object while holding the lru_lock.
		 */
		spin_lock(&bo->bdev->lru_lock);
		bo->base.resv = &bo->base._resv;
		spin_unlock(&bo->bdev->lru_lock);
	}

	return r;
}

static void ttm_bo_flush_all_fences(struct ttm_buffer_object *bo)
{
	struct dma_resv *resv = &bo->base._resv;
	struct dma_resv_iter cursor;
	struct dma_fence *fence;

	dma_resv_iter_begin(&cursor, resv, DMA_RESV_USAGE_BOOKKEEP);
	dma_resv_for_each_fence_unlocked(&cursor, fence) {
		if (!fence->ops->signaled)
			dma_fence_enable_sw_signaling(fence);
	}
	dma_resv_iter_end(&cursor);
}

/**
 * ttm_bo_cleanup_refs
 * If bo idle, remove from lru lists, and unref.
 * If not idle, block if possible.
 *
 * Must be called with lru_lock and reservation held, this function
 * will drop the lru lock and optionally the reservation lock before returning.
 *
 * @bo:                    The buffer object to clean-up
 * @interruptible:         Any sleeps should occur interruptibly.
 * @no_wait_gpu:           Never wait for gpu. Return -EBUSY instead.
 * @unlock_resv:           Unlock the reservation lock as well.
 */

static int ttm_bo_cleanup_refs(struct ttm_buffer_object *bo,
			       bool interruptible, bool no_wait_gpu,
			       bool unlock_resv)
{
	struct dma_resv *resv = &bo->base._resv;
	int ret;

	if (dma_resv_test_signaled(resv, DMA_RESV_USAGE_BOOKKEEP))
		ret = 0;
	else
		ret = -EBUSY;

	if (ret && !no_wait_gpu) {
		long lret;

		if (unlock_resv)
			dma_resv_unlock(bo->base.resv);
		spin_unlock(&bo->bdev->lru_lock);

		lret = dma_resv_wait_timeout(resv, DMA_RESV_USAGE_BOOKKEEP,
					     interruptible,
					     30 * HZ);

		if (lret < 0)
			return lret;
		else if (lret == 0)
			return -EBUSY;

		spin_lock(&bo->bdev->lru_lock);
		if (unlock_resv && !dma_resv_trylock(bo->base.resv)) {
			/*
			 * We raced, and lost, someone else holds the reservation now,
			 * and is probably busy in ttm_bo_cleanup_memtype_use.
			 *
			 * Even if it's not the case, because we finished waiting any
			 * delayed destruction would succeed, so just return success
			 * here.
			 */
			spin_unlock(&bo->bdev->lru_lock);
			return 0;
		}
		ret = 0;
	}

	if (ret) {
		if (unlock_resv)
			dma_resv_unlock(bo->base.resv);
		spin_unlock(&bo->bdev->lru_lock);
		return ret;
	}

	spin_unlock(&bo->bdev->lru_lock);
	ttm_bo_cleanup_memtype_use(bo);

	if (unlock_resv)
		dma_resv_unlock(bo->base.resv);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	return 0;
}

<<<<<<< HEAD
/**
 * Traverse the delayed list, and call ttm_bo_cleanup_refs on all
 * encountered buffers.
 */

static int ttm_bo_delayed_delete(struct ttm_bo_device *bdev, bool remove_all)
{
	struct ttm_bo_global *glob = bdev->glob;
	struct ttm_buffer_object *entry = NULL;
	int ret = 0;

	spin_lock(&glob->lru_lock);
	if (list_empty(&bdev->ddestroy))
		goto out_unlock;

	entry = list_first_entry(&bdev->ddestroy,
		struct ttm_buffer_object, ddestroy);
	kref_get(&entry->list_kref);

	for (;;) {
		struct ttm_buffer_object *nentry = NULL;

		if (entry->ddestroy.next != &bdev->ddestroy) {
			nentry = list_first_entry(&entry->ddestroy,
				struct ttm_buffer_object, ddestroy);
			kref_get(&nentry->list_kref);
		}

		spin_unlock(&glob->lru_lock);
		ret = ttm_bo_cleanup_refs(entry, false, !remove_all,
					  !remove_all);
		kref_put(&entry->list_kref, ttm_bo_release_list);
		entry = nentry;

		if (ret || !entry)
			goto out;

		spin_lock(&glob->lru_lock);
		if (list_empty(&entry->ddestroy))
			break;
	}

out_unlock:
	spin_unlock(&glob->lru_lock);
out:
	if (entry)
		kref_put(&entry->list_kref, ttm_bo_release_list);
	return ret;
}

static void ttm_bo_delayed_workqueue(struct work_struct *work)
{
	struct ttm_bo_device *bdev =
	    container_of(work, struct ttm_bo_device, wq.work);

	if (ttm_bo_delayed_delete(bdev, false)) {
		schedule_delayed_work(&bdev->wq,
				      ((HZ / 100) < 1) ? 1 : HZ / 100);
	}
=======
/*
 * Block for the dma_resv object to become idle, lock the buffer and clean up
 * the resource and tt object.
 */
static void ttm_bo_delayed_delete(struct work_struct *work)
{
	struct ttm_buffer_object *bo;

	bo = container_of(work, typeof(*bo), delayed_delete);

	dma_resv_wait_timeout(bo->base.resv, DMA_RESV_USAGE_BOOKKEEP, false,
			      MAX_SCHEDULE_TIMEOUT);
	dma_resv_lock(bo->base.resv, NULL);
	ttm_bo_cleanup_memtype_use(bo);
	dma_resv_unlock(bo->base.resv);
	ttm_bo_put(bo);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

static void ttm_bo_release(struct kref *kref)
{
	struct ttm_buffer_object *bo =
	    container_of(kref, struct ttm_buffer_object, kref);
<<<<<<< HEAD
	struct ttm_bo_device *bdev = bo->bdev;
	struct ttm_mem_type_manager *man = &bdev->man[bo->mem.mem_type];

	if (likely(bo->vm_node != NULL)) {
		rb_erase(&bo->vm_rb, &bdev->addr_space_rb);
		drm_mm_put_block(bo->vm_node);
		bo->vm_node = NULL;
	}
	write_unlock(&bdev->vm_lock);
	ttm_mem_io_lock(man, false);
	ttm_mem_io_free_vm(bo);
	ttm_mem_io_unlock(man);
	ttm_bo_cleanup_refs_or_queue(bo);
	kref_put(&bo->list_kref, ttm_bo_release_list);
	write_lock(&bdev->vm_lock);
}

void ttm_bo_unref(struct ttm_buffer_object **p_bo)
{
	struct ttm_buffer_object *bo = *p_bo;
	struct ttm_bo_device *bdev = bo->bdev;

	*p_bo = NULL;
	write_lock(&bdev->vm_lock);
	kref_put(&bo->kref, ttm_bo_release);
	write_unlock(&bdev->vm_lock);
}
EXPORT_SYMBOL(ttm_bo_unref);

int ttm_bo_lock_delayed_workqueue(struct ttm_bo_device *bdev)
{
	return cancel_delayed_work_sync(&bdev->wq);
}
EXPORT_SYMBOL(ttm_bo_lock_delayed_workqueue);

void ttm_bo_unlock_delayed_workqueue(struct ttm_bo_device *bdev, int resched)
{
	if (resched)
		schedule_delayed_work(&bdev->wq,
				      ((HZ / 100) < 1) ? 1 : HZ / 100);
}
EXPORT_SYMBOL(ttm_bo_unlock_delayed_workqueue);

static int ttm_bo_evict(struct ttm_buffer_object *bo, bool interruptible,
			bool no_wait_reserve, bool no_wait_gpu)
{
	struct ttm_bo_device *bdev = bo->bdev;
	struct ttm_mem_reg evict_mem;
	struct ttm_placement placement;
	int ret = 0;

	spin_lock(&bdev->fence_lock);
	ret = ttm_bo_wait(bo, false, interruptible, no_wait_gpu);
	spin_unlock(&bdev->fence_lock);

	if (unlikely(ret != 0)) {
		if (ret != -ERESTARTSYS) {
			pr_err("Failed to expire sync object before buffer eviction\n");
		}
		goto out;
	}

	BUG_ON(!atomic_read(&bo->reserved));

	evict_mem = bo->mem;
	evict_mem.mm_node = NULL;
	evict_mem.bus.io_reserved_vm = false;
	evict_mem.bus.io_reserved_count = 0;

	placement.fpfn = 0;
	placement.lpfn = 0;
	placement.num_placement = 0;
	placement.num_busy_placement = 0;
	bdev->driver->evict_flags(bo, &placement);
	ret = ttm_bo_mem_space(bo, &placement, &evict_mem, interruptible,
				no_wait_reserve, no_wait_gpu);
=======
	struct ttm_device *bdev = bo->bdev;
	int ret;

	WARN_ON_ONCE(bo->pin_count);
	WARN_ON_ONCE(bo->bulk_move);

	if (!bo->deleted) {
		ret = ttm_bo_individualize_resv(bo);
		if (ret) {
			/* Last resort, if we fail to allocate memory for the
			 * fences block for the BO to become idle
			 */
			dma_resv_wait_timeout(bo->base.resv,
					      DMA_RESV_USAGE_BOOKKEEP, false,
					      30 * HZ);
		}

		if (bo->bdev->funcs->release_notify)
			bo->bdev->funcs->release_notify(bo);

		drm_vma_offset_remove(bdev->vma_manager, &bo->base.vma_node);
		ttm_mem_io_free(bdev, bo->resource);

		if (!dma_resv_test_signaled(bo->base.resv,
					    DMA_RESV_USAGE_BOOKKEEP) ||
		    (want_init_on_free() && (bo->ttm != NULL)) ||
		    !dma_resv_trylock(bo->base.resv)) {
			/* The BO is not idle, resurrect it for delayed destroy */
			ttm_bo_flush_all_fences(bo);
			bo->deleted = true;

			spin_lock(&bo->bdev->lru_lock);

			/*
			 * Make pinned bos immediately available to
			 * shrinkers, now that they are queued for
			 * destruction.
			 *
			 * FIXME: QXL is triggering this. Can be removed when the
			 * driver is fixed.
			 */
			if (bo->pin_count) {
				bo->pin_count = 0;
				ttm_resource_move_to_lru_tail(bo->resource);
			}

			kref_init(&bo->kref);
			spin_unlock(&bo->bdev->lru_lock);

			INIT_WORK(&bo->delayed_delete, ttm_bo_delayed_delete);

			/* Schedule the worker on the closest NUMA node. This
			 * improves performance since system memory might be
			 * cleared on free and that is best done on a CPU core
			 * close to it.
			 */
			queue_work_node(bdev->pool.nid, bdev->wq, &bo->delayed_delete);
			return;
		}

		ttm_bo_cleanup_memtype_use(bo);
		dma_resv_unlock(bo->base.resv);
	}

	atomic_dec(&ttm_glob.bo_count);
	bo->destroy(bo);
}

/**
 * ttm_bo_put
 *
 * @bo: The buffer object.
 *
 * Unreference a buffer object.
 */
void ttm_bo_put(struct ttm_buffer_object *bo)
{
	kref_put(&bo->kref, ttm_bo_release);
}
EXPORT_SYMBOL(ttm_bo_put);

static int ttm_bo_bounce_temp_buffer(struct ttm_buffer_object *bo,
				     struct ttm_resource **mem,
				     struct ttm_operation_ctx *ctx,
				     struct ttm_place *hop)
{
	struct ttm_placement hop_placement;
	struct ttm_resource *hop_mem;
	int ret;

	hop_placement.num_placement = 1;
	hop_placement.placement = hop;

	/* find space in the bounce domain */
	ret = ttm_bo_mem_space(bo, &hop_placement, &hop_mem, ctx);
	if (ret)
		return ret;
	/* move to the bounce domain */
	ret = ttm_bo_handle_move_mem(bo, hop_mem, false, ctx, NULL);
	if (ret) {
		ttm_resource_free(bo, &hop_mem);
		return ret;
	}
	return 0;
}

static int ttm_bo_evict(struct ttm_buffer_object *bo,
			struct ttm_operation_ctx *ctx)
{
	struct ttm_device *bdev = bo->bdev;
	struct ttm_resource *evict_mem;
	struct ttm_placement placement;
	struct ttm_place hop;
	int ret = 0;

	memset(&hop, 0, sizeof(hop));

	dma_resv_assert_held(bo->base.resv);

	placement.num_placement = 0;
	bdev->funcs->evict_flags(bo, &placement);

	if (!placement.num_placement) {
		ret = ttm_bo_wait_ctx(bo, ctx);
		if (ret)
			return ret;

		/*
		 * Since we've already synced, this frees backing store
		 * immediately.
		 */
		return ttm_bo_pipeline_gutting(bo);
	}

	ret = ttm_bo_mem_space(bo, &placement, &evict_mem, ctx);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	if (ret) {
		if (ret != -ERESTARTSYS) {
			pr_err("Failed to find memory space for buffer 0x%p eviction\n",
			       bo);
			ttm_bo_mem_space_debug(bo, &placement);
		}
		goto out;
	}

<<<<<<< HEAD
	ret = ttm_bo_handle_move_mem(bo, &evict_mem, true, interruptible,
				     no_wait_reserve, no_wait_gpu);
	if (ret) {
		if (ret != -ERESTARTSYS)
			pr_err("Buffer eviction failed\n");
		ttm_bo_mem_put(bo, &evict_mem);
		goto out;
	}
	bo->evicted = true;
=======
	do {
		ret = ttm_bo_handle_move_mem(bo, evict_mem, true, ctx, &hop);
		if (ret != -EMULTIHOP)
			break;

		ret = ttm_bo_bounce_temp_buffer(bo, &evict_mem, ctx, &hop);
	} while (!ret);

	if (ret) {
		ttm_resource_free(bo, &evict_mem);
		if (ret != -ERESTARTSYS && ret != -EINTR)
			pr_err("Buffer eviction failed\n");
	}
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
out:
	return ret;
}

<<<<<<< HEAD
static int ttm_mem_evict_first(struct ttm_bo_device *bdev,
				uint32_t mem_type,
				bool interruptible, bool no_wait_reserve,
				bool no_wait_gpu)
{
	struct ttm_bo_global *glob = bdev->glob;
	struct ttm_mem_type_manager *man = &bdev->man[mem_type];
	struct ttm_buffer_object *bo;
	int ret, put_count = 0;

retry:
	spin_lock(&glob->lru_lock);
	if (list_empty(&man->lru)) {
		spin_unlock(&glob->lru_lock);
		return -EBUSY;
	}

	bo = list_first_entry(&man->lru, struct ttm_buffer_object, lru);
	kref_get(&bo->list_kref);

	if (!list_empty(&bo->ddestroy)) {
		spin_unlock(&glob->lru_lock);
		ret = ttm_bo_cleanup_refs(bo, interruptible,
					  no_wait_reserve, no_wait_gpu);
		kref_put(&bo->list_kref, ttm_bo_release_list);

		if (likely(ret == 0 || ret == -ERESTARTSYS))
			return ret;

		goto retry;
	}

	ret = ttm_bo_reserve_locked(bo, false, no_wait_reserve, false, 0);

	if (unlikely(ret == -EBUSY)) {
		spin_unlock(&glob->lru_lock);
		if (likely(!no_wait_gpu))
			ret = ttm_bo_wait_unreserved(bo, interruptible);

		kref_put(&bo->list_kref, ttm_bo_release_list);

		/**
		 * We *need* to retry after releasing the lru lock.
		 */

		if (unlikely(ret != 0))
			return ret;
		goto retry;
	}

	put_count = ttm_bo_del_from_lru(bo);
	spin_unlock(&glob->lru_lock);

	BUG_ON(ret != 0);

	ttm_bo_list_ref_sub(bo, put_count, true);

	ret = ttm_bo_evict(bo, interruptible, no_wait_reserve, no_wait_gpu);
	ttm_bo_unreserve(bo);

	kref_put(&bo->list_kref, ttm_bo_release_list);
	return ret;
}

void ttm_bo_mem_put(struct ttm_buffer_object *bo, struct ttm_mem_reg *mem)
{
	struct ttm_mem_type_manager *man = &bo->bdev->man[mem->mem_type];

	if (mem->mm_node)
		(*man->func->put_node)(man, mem);
}
EXPORT_SYMBOL(ttm_bo_mem_put);

/**
=======
/**
 * ttm_bo_eviction_valuable
 *
 * @bo: The buffer object to evict
 * @place: the placement we need to make room for
 *
 * Check if it is valuable to evict the BO to make room for the given placement.
 */
bool ttm_bo_eviction_valuable(struct ttm_buffer_object *bo,
			      const struct ttm_place *place)
{
	struct ttm_resource *res = bo->resource;
	struct ttm_device *bdev = bo->bdev;

	dma_resv_assert_held(bo->base.resv);
	if (bo->resource->mem_type == TTM_PL_SYSTEM)
		return true;

	/* Don't evict this BO if it's outside of the
	 * requested placement range
	 */
	return ttm_resource_intersects(bdev, res, place, bo->base.size);
}
EXPORT_SYMBOL(ttm_bo_eviction_valuable);

/*
 * Check the target bo is allowable to be evicted or swapout, including cases:
 *
 * a. if share same reservation object with ctx->resv, have assumption
 * reservation objects should already be locked, so not lock again and
 * return true directly when either the opreation allow_reserved_eviction
 * or the target bo already is in delayed free list;
 *
 * b. Otherwise, trylock it.
 */
static bool ttm_bo_evict_swapout_allowable(struct ttm_buffer_object *bo,
					   struct ttm_operation_ctx *ctx,
					   const struct ttm_place *place,
					   bool *locked, bool *busy)
{
	bool ret = false;

	if (bo->pin_count) {
		*locked = false;
		if (busy)
			*busy = false;
		return false;
	}

	if (bo->base.resv == ctx->resv) {
		dma_resv_assert_held(bo->base.resv);
		if (ctx->allow_res_evict)
			ret = true;
		*locked = false;
		if (busy)
			*busy = false;
	} else {
		ret = dma_resv_trylock(bo->base.resv);
		*locked = ret;
		if (busy)
			*busy = !ret;
	}

	if (ret && place && (bo->resource->mem_type != place->mem_type ||
		!bo->bdev->funcs->eviction_valuable(bo, place))) {
		ret = false;
		if (*locked) {
			dma_resv_unlock(bo->base.resv);
			*locked = false;
		}
	}

	return ret;
}

/**
 * ttm_mem_evict_wait_busy - wait for a busy BO to become available
 *
 * @busy_bo: BO which couldn't be locked with trylock
 * @ctx: operation context
 * @ticket: acquire ticket
 *
 * Try to lock a busy buffer object to avoid failing eviction.
 */
static int ttm_mem_evict_wait_busy(struct ttm_buffer_object *busy_bo,
				   struct ttm_operation_ctx *ctx,
				   struct ww_acquire_ctx *ticket)
{
	int r;

	if (!busy_bo || !ticket)
		return -EBUSY;

	if (ctx->interruptible)
		r = dma_resv_lock_interruptible(busy_bo->base.resv,
							  ticket);
	else
		r = dma_resv_lock(busy_bo->base.resv, ticket);

	/*
	 * TODO: It would be better to keep the BO locked until allocation is at
	 * least tried one more time, but that would mean a much larger rework
	 * of TTM.
	 */
	if (!r)
		dma_resv_unlock(busy_bo->base.resv);

	return r == -EDEADLK ? -EBUSY : r;
}

int ttm_mem_evict_first(struct ttm_device *bdev,
			struct ttm_resource_manager *man,
			const struct ttm_place *place,
			struct ttm_operation_ctx *ctx,
			struct ww_acquire_ctx *ticket)
{
	struct ttm_buffer_object *bo = NULL, *busy_bo = NULL;
	struct ttm_resource_cursor cursor;
	struct ttm_resource *res;
	bool locked = false;
	int ret;

	spin_lock(&bdev->lru_lock);
	ttm_resource_manager_for_each_res(man, &cursor, res) {
		bool busy;

		if (!ttm_bo_evict_swapout_allowable(res->bo, ctx, place,
						    &locked, &busy)) {
			if (busy && !busy_bo && ticket !=
			    dma_resv_locking_ctx(res->bo->base.resv))
				busy_bo = res->bo;
			continue;
		}

		if (ttm_bo_get_unless_zero(res->bo)) {
			bo = res->bo;
			break;
		}
		if (locked)
			dma_resv_unlock(res->bo->base.resv);
	}

	if (!bo) {
		if (busy_bo && !ttm_bo_get_unless_zero(busy_bo))
			busy_bo = NULL;
		spin_unlock(&bdev->lru_lock);
		ret = ttm_mem_evict_wait_busy(busy_bo, ctx, ticket);
		if (busy_bo)
			ttm_bo_put(busy_bo);
		return ret;
	}

	if (bo->deleted) {
		ret = ttm_bo_cleanup_refs(bo, ctx->interruptible,
					  ctx->no_wait_gpu, locked);
		ttm_bo_put(bo);
		return ret;
	}

	spin_unlock(&bdev->lru_lock);

	ret = ttm_bo_evict(bo, ctx);
	if (locked)
		ttm_bo_unreserve(bo);
	else
		ttm_bo_move_to_lru_tail_unlocked(bo);

	ttm_bo_put(bo);
	return ret;
}

/**
 * ttm_bo_pin - Pin the buffer object.
 * @bo: The buffer object to pin
 *
 * Make sure the buffer is not evicted any more during memory pressure.
 * @bo must be unpinned again by calling ttm_bo_unpin().
 */
void ttm_bo_pin(struct ttm_buffer_object *bo)
{
	dma_resv_assert_held(bo->base.resv);
	WARN_ON_ONCE(!kref_read(&bo->kref));
	spin_lock(&bo->bdev->lru_lock);
	if (bo->resource)
		ttm_resource_del_bulk_move(bo->resource, bo);
	++bo->pin_count;
	spin_unlock(&bo->bdev->lru_lock);
}
EXPORT_SYMBOL(ttm_bo_pin);

/**
 * ttm_bo_unpin - Unpin the buffer object.
 * @bo: The buffer object to unpin
 *
 * Allows the buffer object to be evicted again during memory pressure.
 */
void ttm_bo_unpin(struct ttm_buffer_object *bo)
{
	dma_resv_assert_held(bo->base.resv);
	WARN_ON_ONCE(!kref_read(&bo->kref));
	if (WARN_ON_ONCE(!bo->pin_count))
		return;

	spin_lock(&bo->bdev->lru_lock);
	--bo->pin_count;
	if (bo->resource)
		ttm_resource_add_bulk_move(bo->resource, bo);
	spin_unlock(&bo->bdev->lru_lock);
}
EXPORT_SYMBOL(ttm_bo_unpin);

/*
 * Add the last move fence to the BO as kernel dependency and reserve a new
 * fence slot.
 */
static int ttm_bo_add_move_fence(struct ttm_buffer_object *bo,
				 struct ttm_resource_manager *man,
				 struct ttm_resource *mem,
				 bool no_wait_gpu)
{
	struct dma_fence *fence;
	int ret;

	spin_lock(&man->move_lock);
	fence = dma_fence_get(man->move);
	spin_unlock(&man->move_lock);

	if (!fence)
		return 0;

	if (no_wait_gpu) {
		ret = dma_fence_is_signaled(fence) ? 0 : -EBUSY;
		dma_fence_put(fence);
		return ret;
	}

	dma_resv_add_fence(bo->base.resv, fence, DMA_RESV_USAGE_KERNEL);

	ret = dma_resv_reserve_fences(bo->base.resv, 1);
	dma_fence_put(fence);
	return ret;
}

/*
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 * Repeatedly evict memory from the LRU for @mem_type until we create enough
 * space, or we've evicted everything and there isn't enough space.
 */
static int ttm_bo_mem_force_space(struct ttm_buffer_object *bo,
<<<<<<< HEAD
					uint32_t mem_type,
					struct ttm_placement *placement,
					struct ttm_mem_reg *mem,
					bool interruptible,
					bool no_wait_reserve,
					bool no_wait_gpu)
{
	struct ttm_bo_device *bdev = bo->bdev;
	struct ttm_mem_type_manager *man = &bdev->man[mem_type];
	int ret;

	do {
		ret = (*man->func->get_node)(man, bo, placement, mem);
		if (unlikely(ret != 0))
			return ret;
		if (mem->mm_node)
			break;
		ret = ttm_mem_evict_first(bdev, mem_type, interruptible,
						no_wait_reserve, no_wait_gpu);
		if (unlikely(ret != 0))
			return ret;
	} while (1);
	if (mem->mm_node == NULL)
		return -ENOMEM;
	mem->mem_type = mem_type;
	return 0;
}

static uint32_t ttm_bo_select_caching(struct ttm_mem_type_manager *man,
				      uint32_t cur_placement,
				      uint32_t proposed_placement)
{
	uint32_t caching = proposed_placement & TTM_PL_MASK_CACHING;
	uint32_t result = proposed_placement & ~TTM_PL_MASK_CACHING;

	/**
	 * Keep current caching if possible.
	 */

	if ((cur_placement & caching) != 0)
		result |= (cur_placement & caching);
	else if ((man->default_caching & caching) != 0)
		result |= man->default_caching;
	else if ((TTM_PL_FLAG_CACHED & caching) != 0)
		result |= TTM_PL_FLAG_CACHED;
	else if ((TTM_PL_FLAG_WC & caching) != 0)
		result |= TTM_PL_FLAG_WC;
	else if ((TTM_PL_FLAG_UNCACHED & caching) != 0)
		result |= TTM_PL_FLAG_UNCACHED;

	return result;
}

static bool ttm_bo_mt_compatible(struct ttm_mem_type_manager *man,
				 uint32_t mem_type,
				 uint32_t proposed_placement,
				 uint32_t *masked_placement)
{
	uint32_t cur_flags = ttm_bo_type_flags(mem_type);

	if ((cur_flags & proposed_placement & TTM_PL_MASK_MEM) == 0)
		return false;

	if ((proposed_placement & man->available_caching) == 0)
		return false;

	cur_flags |= (proposed_placement & man->available_caching);

	*masked_placement = cur_flags;
	return true;
}

/**
 * Creates space for memory region @mem according to its type.
 *
 * This function first searches for free space in compatible memory types in
 * the priority order defined by the driver.  If free space isn't found, then
 * ttm_bo_mem_force_space is attempted in priority order to evict and find
 * space.
 */
int ttm_bo_mem_space(struct ttm_buffer_object *bo,
			struct ttm_placement *placement,
			struct ttm_mem_reg *mem,
			bool interruptible, bool no_wait_reserve,
			bool no_wait_gpu)
{
	struct ttm_bo_device *bdev = bo->bdev;
	struct ttm_mem_type_manager *man;
	uint32_t mem_type = TTM_PL_SYSTEM;
	uint32_t cur_flags = 0;
	bool type_found = false;
	bool type_ok = false;
	bool has_erestartsys = false;
	int i, ret;

	mem->mm_node = NULL;
	for (i = 0; i < placement->num_placement; ++i) {
		ret = ttm_mem_type_from_flags(placement->placement[i],
						&mem_type);
		if (ret)
			return ret;
		man = &bdev->man[mem_type];

		type_ok = ttm_bo_mt_compatible(man,
						mem_type,
						placement->placement[i],
						&cur_flags);

		if (!type_ok)
			continue;

		cur_flags = ttm_bo_select_caching(man, bo->mem.placement,
						  cur_flags);
		/*
		 * Use the access and other non-mapping-related flag bits from
		 * the memory placement flags to the current flags
		 */
		ttm_flag_masked(&cur_flags, placement->placement[i],
				~TTM_PL_MASK_MEMTYPE);

		if (mem_type == TTM_PL_SYSTEM)
			break;

		if (man->has_type && man->use_type) {
			type_found = true;
			ret = (*man->func->get_node)(man, bo, placement, mem);
			if (unlikely(ret))
				return ret;
		}
		if (mem->mm_node)
			break;
	}

	if ((type_ok && (mem_type == TTM_PL_SYSTEM)) || mem->mm_node) {
		mem->mem_type = mem_type;
		mem->placement = cur_flags;
		return 0;
	}

	if (!type_found)
		return -EINVAL;

	for (i = 0; i < placement->num_busy_placement; ++i) {
		ret = ttm_mem_type_from_flags(placement->busy_placement[i],
						&mem_type);
		if (ret)
			return ret;
		man = &bdev->man[mem_type];
		if (!man->has_type)
			continue;
		if (!ttm_bo_mt_compatible(man,
						mem_type,
						placement->busy_placement[i],
						&cur_flags))
			continue;

		cur_flags = ttm_bo_select_caching(man, bo->mem.placement,
						  cur_flags);
		/*
		 * Use the access and other non-mapping-related flag bits from
		 * the memory placement flags to the current flags
		 */
		ttm_flag_masked(&cur_flags, placement->busy_placement[i],
				~TTM_PL_MASK_MEMTYPE);


		if (mem_type == TTM_PL_SYSTEM) {
			mem->mem_type = mem_type;
			mem->placement = cur_flags;
			mem->mm_node = NULL;
			return 0;
		}

		ret = ttm_bo_mem_force_space(bo, mem_type, placement, mem,
						interruptible, no_wait_reserve, no_wait_gpu);
		if (ret == 0 && mem->mm_node) {
			mem->placement = cur_flags;
			return 0;
		}
		if (ret == -ERESTARTSYS)
			has_erestartsys = true;
	}
	ret = (has_erestartsys) ? -ERESTARTSYS : -ENOMEM;
=======
				  const struct ttm_place *place,
				  struct ttm_resource **mem,
				  struct ttm_operation_ctx *ctx)
{
	struct ttm_device *bdev = bo->bdev;
	struct ttm_resource_manager *man;
	struct ww_acquire_ctx *ticket;
	int ret;

	man = ttm_manager_type(bdev, place->mem_type);
	ticket = dma_resv_locking_ctx(bo->base.resv);
	do {
		ret = ttm_resource_alloc(bo, place, mem);
		if (likely(!ret))
			break;
		if (unlikely(ret != -ENOSPC))
			return ret;
		ret = ttm_mem_evict_first(bdev, man, place, ctx,
					  ticket);
		if (unlikely(ret != 0))
			return ret;
	} while (1);

	return ttm_bo_add_move_fence(bo, man, *mem, ctx->no_wait_gpu);
}

/**
 * ttm_bo_mem_space
 *
 * @bo: Pointer to a struct ttm_buffer_object. the data of which
 * we want to allocate space for.
 * @placement: Proposed new placement for the buffer object.
 * @mem: A struct ttm_resource.
 * @ctx: if and how to sleep, lock buffers and alloc memory
 *
 * Allocate memory space for the buffer object pointed to by @bo, using
 * the placement flags in @placement, potentially evicting other idle buffer objects.
 * This function may sleep while waiting for space to become available.
 * Returns:
 * -EBUSY: No space available (only if no_wait == 1).
 * -ENOSPC: Could not allocate space for the buffer object, either due to
 * fragmentation or concurrent allocators.
 * -ERESTARTSYS: An interruptible sleep was interrupted by a signal.
 */
int ttm_bo_mem_space(struct ttm_buffer_object *bo,
			struct ttm_placement *placement,
			struct ttm_resource **mem,
			struct ttm_operation_ctx *ctx)
{
	struct ttm_device *bdev = bo->bdev;
	bool type_found = false;
	int i, ret;

	ret = dma_resv_reserve_fences(bo->base.resv, 1);
	if (unlikely(ret))
		return ret;

	for (i = 0; i < placement->num_placement; ++i) {
		const struct ttm_place *place = &placement->placement[i];
		struct ttm_resource_manager *man;

		if (place->flags & TTM_PL_FLAG_FALLBACK)
			continue;

		man = ttm_manager_type(bdev, place->mem_type);
		if (!man || !ttm_resource_manager_used(man))
			continue;

		type_found = true;
		ret = ttm_resource_alloc(bo, place, mem);
		if (ret == -ENOSPC)
			continue;
		if (unlikely(ret))
			goto error;

		ret = ttm_bo_add_move_fence(bo, man, *mem, ctx->no_wait_gpu);
		if (unlikely(ret)) {
			ttm_resource_free(bo, mem);
			if (ret == -EBUSY)
				continue;

			goto error;
		}
		return 0;
	}

	for (i = 0; i < placement->num_placement; ++i) {
		const struct ttm_place *place = &placement->placement[i];
		struct ttm_resource_manager *man;

		if (place->flags & TTM_PL_FLAG_DESIRED)
			continue;

		man = ttm_manager_type(bdev, place->mem_type);
		if (!man || !ttm_resource_manager_used(man))
			continue;

		type_found = true;
		ret = ttm_bo_mem_force_space(bo, place, mem, ctx);
		if (likely(!ret))
			return 0;

		if (ret && ret != -EBUSY)
			goto error;
	}

	ret = -ENOSPC;
	if (!type_found) {
		pr_err(TTM_PFX "No compatible memory type found\n");
		ret = -EINVAL;
	}

error:
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	return ret;
}
EXPORT_SYMBOL(ttm_bo_mem_space);

<<<<<<< HEAD
int ttm_bo_wait_cpu(struct ttm_buffer_object *bo, bool no_wait)
{
	if ((atomic_read(&bo->cpu_writers) > 0) && no_wait)
		return -EBUSY;

	return wait_event_interruptible(bo->event_queue,
					atomic_read(&bo->cpu_writers) == 0);
}
EXPORT_SYMBOL(ttm_bo_wait_cpu);

int ttm_bo_move_buffer(struct ttm_buffer_object *bo,
			struct ttm_placement *placement,
			bool interruptible, bool no_wait_reserve,
			bool no_wait_gpu)
{
	int ret = 0;
	struct ttm_mem_reg mem;
	struct ttm_bo_device *bdev = bo->bdev;

	BUG_ON(!atomic_read(&bo->reserved));

	/*
	 * FIXME: It's possible to pipeline buffer moves.
	 * Have the driver move function wait for idle when necessary,
	 * instead of doing it here.
	 */
	spin_lock(&bdev->fence_lock);
	ret = ttm_bo_wait(bo, false, interruptible, no_wait_gpu);
	spin_unlock(&bdev->fence_lock);
	if (ret)
		return ret;
	mem.num_pages = bo->num_pages;
	mem.size = mem.num_pages << PAGE_SHIFT;
	mem.page_alignment = bo->mem.page_alignment;
	mem.bus.io_reserved_vm = false;
	mem.bus.io_reserved_count = 0;
	/*
	 * Determine where to move the buffer.
	 */
	ret = ttm_bo_mem_space(bo, placement, &mem, interruptible, no_wait_reserve, no_wait_gpu);
	if (ret)
		goto out_unlock;
	ret = ttm_bo_handle_move_mem(bo, &mem, false, interruptible, no_wait_reserve, no_wait_gpu);
out_unlock:
	if (ret && mem.mm_node)
		ttm_bo_mem_put(bo, &mem);
	return ret;
}

static bool ttm_bo_mem_compat(struct ttm_placement *placement,
			      struct ttm_mem_reg *mem,
			      uint32_t *new_flags)
{
	int i;

	if (mem->mm_node && placement->lpfn != 0 &&
	    (mem->start < placement->fpfn ||
	     mem->start + mem->num_pages > placement->lpfn))
		return false;

	for (i = 0; i < placement->num_placement; i++) {
		*new_flags = placement->placement[i];
		if ((*new_flags & mem->placement & TTM_PL_MASK_CACHING) &&
		    (*new_flags & mem->placement & TTM_PL_MASK_MEM))
			return true;
	}

	for (i = 0; i < placement->num_busy_placement; i++) {
		*new_flags = placement->busy_placement[i];
		if ((*new_flags & mem->placement & TTM_PL_MASK_CACHING) &&
		    (*new_flags & mem->placement & TTM_PL_MASK_MEM))
			return true;
	}

	return false;
}

int ttm_bo_validate(struct ttm_buffer_object *bo,
			struct ttm_placement *placement,
			bool interruptible, bool no_wait_reserve,
			bool no_wait_gpu)
{
	int ret;
	uint32_t new_flags;

	BUG_ON(!atomic_read(&bo->reserved));
	/* Check that range is valid */
	if (placement->lpfn || placement->fpfn)
		if (placement->fpfn > placement->lpfn ||
			(placement->lpfn - placement->fpfn) < bo->num_pages)
			return -EINVAL;
	/*
	 * Check whether we need to move buffer.
	 */
	if (!ttm_bo_mem_compat(placement, &bo->mem, &new_flags)) {
		ret = ttm_bo_move_buffer(bo, placement, interruptible, no_wait_reserve, no_wait_gpu);
		if (ret)
			return ret;
	} else {
		/*
		 * Use the access and other non-mapping-related flag bits from
		 * the compatible memory placement flags to the active flags
		 */
		ttm_flag_masked(&bo->mem.placement, new_flags,
				~TTM_PL_MASK_MEMTYPE);
	}
	/*
	 * We might need to add a TTM.
	 */
	if (bo->mem.mem_type == TTM_PL_SYSTEM && bo->ttm == NULL) {
		ret = ttm_bo_add_ttm(bo, true);
=======
static int ttm_bo_move_buffer(struct ttm_buffer_object *bo,
			      struct ttm_placement *placement,
			      struct ttm_operation_ctx *ctx)
{
	struct ttm_resource *mem;
	struct ttm_place hop;
	int ret;

	dma_resv_assert_held(bo->base.resv);

	/*
	 * Determine where to move the buffer.
	 *
	 * If driver determines move is going to need
	 * an extra step then it will return -EMULTIHOP
	 * and the buffer will be moved to the temporary
	 * stop and the driver will be called to make
	 * the second hop.
	 */
	ret = ttm_bo_mem_space(bo, placement, &mem, ctx);
	if (ret)
		return ret;
bounce:
	ret = ttm_bo_handle_move_mem(bo, mem, false, ctx, &hop);
	if (ret == -EMULTIHOP) {
		ret = ttm_bo_bounce_temp_buffer(bo, &mem, ctx, &hop);
		if (ret)
			goto out;
		/* try and move to final place now. */
		goto bounce;
	}
out:
	if (ret)
		ttm_resource_free(bo, &mem);
	return ret;
}

/**
 * ttm_bo_validate
 *
 * @bo: The buffer object.
 * @placement: Proposed placement for the buffer object.
 * @ctx: validation parameters.
 *
 * Changes placement and caching policy of the buffer object
 * according proposed placement.
 * Returns
 * -EINVAL on invalid proposed placement.
 * -ENOMEM on out-of-memory condition.
 * -EBUSY if no_wait is true and buffer busy.
 * -ERESTARTSYS if interrupted by a signal.
 */
int ttm_bo_validate(struct ttm_buffer_object *bo,
		    struct ttm_placement *placement,
		    struct ttm_operation_ctx *ctx)
{
	int ret;

	dma_resv_assert_held(bo->base.resv);

	/*
	 * Remove the backing store if no placement is given.
	 */
	if (!placement->num_placement)
		return ttm_bo_pipeline_gutting(bo);

	/* Check whether we need to move buffer. */
	if (bo->resource && ttm_resource_compatible(bo->resource, placement))
		return 0;

	/* Moving of pinned BOs is forbidden */
	if (bo->pin_count)
		return -EINVAL;

	ret = ttm_bo_move_buffer(bo, placement, ctx);
	/* For backward compatibility with userspace */
	if (ret == -ENOSPC)
		return -ENOMEM;
	if (ret)
		return ret;

	/*
	 * We might need to add a TTM.
	 */
	if (!bo->resource || bo->resource->mem_type == TTM_PL_SYSTEM) {
		ret = ttm_tt_create(bo, true);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		if (ret)
			return ret;
	}
	return 0;
}
EXPORT_SYMBOL(ttm_bo_validate);

<<<<<<< HEAD
int ttm_bo_check_placement(struct ttm_buffer_object *bo,
				struct ttm_placement *placement)
{
	BUG_ON((placement->fpfn || placement->lpfn) &&
	       (bo->mem.num_pages > (placement->lpfn - placement->fpfn)));

	return 0;
}

int ttm_bo_init(struct ttm_bo_device *bdev,
		struct ttm_buffer_object *bo,
		unsigned long size,
		enum ttm_bo_type type,
		struct ttm_placement *placement,
		uint32_t page_alignment,
		unsigned long buffer_start,
		bool interruptible,
		struct file *persistent_swap_storage,
		size_t acc_size,
		void (*destroy) (struct ttm_buffer_object *))
{
	int ret = 0;
	unsigned long num_pages;
	struct ttm_mem_global *mem_glob = bdev->glob->mem_glob;

	ret = ttm_mem_global_alloc(mem_glob, acc_size, false, false);
	if (ret) {
		pr_err("Out of kernel memory\n");
		if (destroy)
			(*destroy)(bo);
		else
			kfree(bo);
		return -ENOMEM;
	}

	size += buffer_start & ~PAGE_MASK;
	num_pages = (size + PAGE_SIZE - 1) >> PAGE_SHIFT;
	if (num_pages == 0) {
		pr_err("Illegal buffer object size\n");
		if (destroy)
			(*destroy)(bo);
		else
			kfree(bo);
		ttm_mem_global_free(mem_glob, acc_size);
		return -EINVAL;
	}
	bo->destroy = destroy;

	kref_init(&bo->kref);
	kref_init(&bo->list_kref);
	atomic_set(&bo->cpu_writers, 0);
	atomic_set(&bo->reserved, 1);
	init_waitqueue_head(&bo->event_queue);
	INIT_LIST_HEAD(&bo->lru);
	INIT_LIST_HEAD(&bo->ddestroy);
	INIT_LIST_HEAD(&bo->swap);
	INIT_LIST_HEAD(&bo->io_reserve_lru);
	bo->bdev = bdev;
	bo->glob = bdev->glob;
	bo->type = type;
	bo->num_pages = num_pages;
	bo->mem.size = num_pages << PAGE_SHIFT;
	bo->mem.mem_type = TTM_PL_SYSTEM;
	bo->mem.num_pages = bo->num_pages;
	bo->mem.mm_node = NULL;
	bo->mem.page_alignment = page_alignment;
	bo->mem.bus.io_reserved_vm = false;
	bo->mem.bus.io_reserved_count = 0;
	bo->buffer_start = buffer_start & PAGE_MASK;
	bo->priv_flags = 0;
	bo->mem.placement = (TTM_PL_FLAG_SYSTEM | TTM_PL_FLAG_CACHED);
	bo->seq_valid = false;
	bo->persistent_swap_storage = persistent_swap_storage;
	bo->acc_size = acc_size;
	atomic_inc(&bo->glob->bo_count);

	ret = ttm_bo_check_placement(bo, placement);
	if (unlikely(ret != 0))
		goto out_err;
=======
/**
 * ttm_bo_init_reserved
 *
 * @bdev: Pointer to a ttm_device struct.
 * @bo: Pointer to a ttm_buffer_object to be initialized.
 * @type: Requested type of buffer object.
 * @placement: Initial placement for buffer object.
 * @alignment: Data alignment in pages.
 * @ctx: TTM operation context for memory allocation.
 * @sg: Scatter-gather table.
 * @resv: Pointer to a dma_resv, or NULL to let ttm allocate one.
 * @destroy: Destroy function. Use NULL for kfree().
 *
 * This function initializes a pre-allocated struct ttm_buffer_object.
 * As this object may be part of a larger structure, this function,
 * together with the @destroy function, enables driver-specific objects
 * derived from a ttm_buffer_object.
 *
 * On successful return, the caller owns an object kref to @bo. The kref and
 * list_kref are usually set to 1, but note that in some situations, other
 * tasks may already be holding references to @bo as well.
 * Furthermore, if resv == NULL, the buffer's reservation lock will be held,
 * and it is the caller's responsibility to call ttm_bo_unreserve.
 *
 * If a failure occurs, the function will call the @destroy function. Thus,
 * after a failure, dereferencing @bo is illegal and will likely cause memory
 * corruption.
 *
 * Returns
 * -ENOMEM: Out of memory.
 * -EINVAL: Invalid placement flags.
 * -ERESTARTSYS: Interrupted by signal while sleeping waiting for resources.
 */
int ttm_bo_init_reserved(struct ttm_device *bdev, struct ttm_buffer_object *bo,
			 enum ttm_bo_type type, struct ttm_placement *placement,
			 uint32_t alignment, struct ttm_operation_ctx *ctx,
			 struct sg_table *sg, struct dma_resv *resv,
			 void (*destroy) (struct ttm_buffer_object *))
{
	int ret;

	kref_init(&bo->kref);
	bo->bdev = bdev;
	bo->type = type;
	bo->page_alignment = alignment;
	bo->destroy = destroy;
	bo->pin_count = 0;
	bo->sg = sg;
	bo->bulk_move = NULL;
	if (resv)
		bo->base.resv = resv;
	else
		bo->base.resv = &bo->base._resv;
	atomic_inc(&ttm_glob.bo_count);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	/*
	 * For ttm_bo_type_device buffers, allocate
	 * address space from the device.
	 */
<<<<<<< HEAD
	if (bo->type == ttm_bo_type_device) {
		ret = ttm_bo_setup_vm(bo);
		if (ret)
			goto out_err;
	}

	ret = ttm_bo_validate(bo, placement, interruptible, false, false);
	if (ret)
		goto out_err;

	ttm_bo_unreserve(bo);
	return 0;

out_err:
	ttm_bo_unreserve(bo);
	ttm_bo_unref(&bo);

	return ret;
}
EXPORT_SYMBOL(ttm_bo_init);

size_t ttm_bo_acc_size(struct ttm_bo_device *bdev,
		       unsigned long bo_size,
		       unsigned struct_size)
{
	unsigned npages = (PAGE_ALIGN(bo_size)) >> PAGE_SHIFT;
	size_t size = 0;

	size += ttm_round_pot(struct_size);
	size += PAGE_ALIGN(npages * sizeof(void *));
	size += ttm_round_pot(sizeof(struct ttm_tt));
	return size;
}
EXPORT_SYMBOL(ttm_bo_acc_size);

size_t ttm_bo_dma_acc_size(struct ttm_bo_device *bdev,
			   unsigned long bo_size,
			   unsigned struct_size)
{
	unsigned npages = (PAGE_ALIGN(bo_size)) >> PAGE_SHIFT;
	size_t size = 0;

	size += ttm_round_pot(struct_size);
	size += PAGE_ALIGN(npages * sizeof(void *));
	size += PAGE_ALIGN(npages * sizeof(dma_addr_t));
	size += ttm_round_pot(sizeof(struct ttm_dma_tt));
	return size;
}
EXPORT_SYMBOL(ttm_bo_dma_acc_size);

int ttm_bo_create(struct ttm_bo_device *bdev,
			unsigned long size,
			enum ttm_bo_type type,
			struct ttm_placement *placement,
			uint32_t page_alignment,
			unsigned long buffer_start,
			bool interruptible,
			struct file *persistent_swap_storage,
			struct ttm_buffer_object **p_bo)
{
	struct ttm_buffer_object *bo;
	size_t acc_size;
	int ret;

	bo = kzalloc(sizeof(*bo), GFP_KERNEL);
	if (unlikely(bo == NULL))
		return -ENOMEM;

	acc_size = ttm_bo_acc_size(bdev, size, sizeof(struct ttm_buffer_object));
	ret = ttm_bo_init(bdev, bo, size, type, placement, page_alignment,
				buffer_start, interruptible,
				persistent_swap_storage, acc_size, NULL);
	if (likely(ret == 0))
		*p_bo = bo;

	return ret;
}
EXPORT_SYMBOL(ttm_bo_create);

static int ttm_bo_force_list_clean(struct ttm_bo_device *bdev,
					unsigned mem_type, bool allow_errors)
{
	struct ttm_mem_type_manager *man = &bdev->man[mem_type];
	struct ttm_bo_global *glob = bdev->glob;
	int ret;

	/*
	 * Can't use standard list traversal since we're unlocking.
	 */

	spin_lock(&glob->lru_lock);
	while (!list_empty(&man->lru)) {
		spin_unlock(&glob->lru_lock);
		ret = ttm_mem_evict_first(bdev, mem_type, false, false, false);
		if (ret) {
			if (allow_errors) {
				return ret;
			} else {
				pr_err("Cleanup eviction failed\n");
			}
		}
		spin_lock(&glob->lru_lock);
	}
	spin_unlock(&glob->lru_lock);
	return 0;
}

int ttm_bo_clean_mm(struct ttm_bo_device *bdev, unsigned mem_type)
{
	struct ttm_mem_type_manager *man;
	int ret = -EINVAL;

	if (mem_type >= TTM_NUM_MEM_TYPES) {
		pr_err("Illegal memory type %d\n", mem_type);
		return ret;
	}
	man = &bdev->man[mem_type];

	if (!man->has_type) {
		pr_err("Trying to take down uninitialized memory manager type %u\n",
		       mem_type);
		return ret;
	}

	man->use_type = false;
	man->has_type = false;

	ret = 0;
	if (mem_type > 0) {
		ttm_bo_force_list_clean(bdev, mem_type, false);

		ret = (*man->func->takedown)(man);
	}

	return ret;
}
EXPORT_SYMBOL(ttm_bo_clean_mm);

int ttm_bo_evict_mm(struct ttm_bo_device *bdev, unsigned mem_type)
{
	struct ttm_mem_type_manager *man = &bdev->man[mem_type];

	if (mem_type == 0 || mem_type >= TTM_NUM_MEM_TYPES) {
		pr_err("Illegal memory manager memory type %u\n", mem_type);
		return -EINVAL;
	}

	if (!man->has_type) {
		pr_err("Memory type %u has not been initialized\n", mem_type);
		return 0;
	}

	return ttm_bo_force_list_clean(bdev, mem_type, true);
}
EXPORT_SYMBOL(ttm_bo_evict_mm);

int ttm_bo_init_mm(struct ttm_bo_device *bdev, unsigned type,
			unsigned long p_size)
{
	int ret = -EINVAL;
	struct ttm_mem_type_manager *man;

	BUG_ON(type >= TTM_NUM_MEM_TYPES);
	man = &bdev->man[type];
	BUG_ON(man->has_type);
	man->io_reserve_fastpath = true;
	man->use_io_reserve_lru = false;
	mutex_init(&man->io_reserve_mutex);
	INIT_LIST_HEAD(&man->io_reserve_lru);

	ret = bdev->driver->init_mem_type(bdev, type, man);
	if (ret)
		return ret;
	man->bdev = bdev;

	ret = 0;
	if (type != TTM_PL_SYSTEM) {
		ret = (*man->func->init)(man, p_size);
		if (ret)
			return ret;
	}
	man->has_type = true;
	man->use_type = true;
	man->size = p_size;

	INIT_LIST_HEAD(&man->lru);

	return 0;
}
EXPORT_SYMBOL(ttm_bo_init_mm);

static void ttm_bo_global_kobj_release(struct kobject *kobj)
{
	struct ttm_bo_global *glob =
		container_of(kobj, struct ttm_bo_global, kobj);

	ttm_mem_unregister_shrink(glob->mem_glob, &glob->shrink);
	__free_page(glob->dummy_read_page);
	kfree(glob);
}

void ttm_bo_global_release(struct drm_global_reference *ref)
{
	struct ttm_bo_global *glob = ref->object;

	kobject_del(&glob->kobj);
	kobject_put(&glob->kobj);
}
EXPORT_SYMBOL(ttm_bo_global_release);

int ttm_bo_global_init(struct drm_global_reference *ref)
{
	struct ttm_bo_global_ref *bo_ref =
		container_of(ref, struct ttm_bo_global_ref, ref);
	struct ttm_bo_global *glob = ref->object;
	int ret;

	mutex_init(&glob->device_list_mutex);
	spin_lock_init(&glob->lru_lock);
	glob->mem_glob = bo_ref->mem_glob;
	glob->dummy_read_page = alloc_page(__GFP_ZERO | GFP_DMA32);

	if (unlikely(glob->dummy_read_page == NULL)) {
		ret = -ENOMEM;
		goto out_no_drp;
	}

	INIT_LIST_HEAD(&glob->swap_lru);
	INIT_LIST_HEAD(&glob->device_list);

	ttm_mem_init_shrink(&glob->shrink, ttm_bo_swapout);
	ret = ttm_mem_register_shrink(glob->mem_glob, &glob->shrink);
	if (unlikely(ret != 0)) {
		pr_err("Could not register buffer object swapout\n");
		goto out_no_shrink;
	}

	atomic_set(&glob->bo_count, 0);

	ret = kobject_init_and_add(
		&glob->kobj, &ttm_bo_glob_kobj_type, ttm_get_kobj(), "buffer_objects");
	if (unlikely(ret != 0))
		kobject_put(&glob->kobj);
	return ret;
out_no_shrink:
	__free_page(glob->dummy_read_page);
out_no_drp:
	kfree(glob);
	return ret;
}
EXPORT_SYMBOL(ttm_bo_global_init);


int ttm_bo_device_release(struct ttm_bo_device *bdev)
{
	int ret = 0;
	unsigned i = TTM_NUM_MEM_TYPES;
	struct ttm_mem_type_manager *man;
	struct ttm_bo_global *glob = bdev->glob;

	while (i--) {
		man = &bdev->man[i];
		if (man->has_type) {
			man->use_type = false;
			if ((i != TTM_PL_SYSTEM) && ttm_bo_clean_mm(bdev, i)) {
				ret = -EBUSY;
				pr_err("DRM memory manager type %d is not clean\n",
				       i);
			}
			man->has_type = false;
		}
	}

	mutex_lock(&glob->device_list_mutex);
	list_del(&bdev->device_list);
	mutex_unlock(&glob->device_list_mutex);

	cancel_delayed_work_sync(&bdev->wq);

	while (ttm_bo_delayed_delete(bdev, true))
		;

	spin_lock(&glob->lru_lock);
	if (list_empty(&bdev->ddestroy))
		TTM_DEBUG("Delayed destroy list was clean\n");

	if (list_empty(&bdev->man[0].lru))
		TTM_DEBUG("Swap list was clean\n");
	spin_unlock(&glob->lru_lock);

	BUG_ON(!drm_mm_clean(&bdev->addr_space_mm));
	write_lock(&bdev->vm_lock);
	drm_mm_takedown(&bdev->addr_space_mm);
	write_unlock(&bdev->vm_lock);

	return ret;
}
EXPORT_SYMBOL(ttm_bo_device_release);

int ttm_bo_device_init(struct ttm_bo_device *bdev,
		       struct ttm_bo_global *glob,
		       struct ttm_bo_driver *driver,
		       uint64_t file_page_offset,
		       bool need_dma32)
{
	int ret = -EINVAL;

	rwlock_init(&bdev->vm_lock);
	bdev->driver = driver;

	memset(bdev->man, 0, sizeof(bdev->man));

	/*
	 * Initialize the system memory buffer type.
	 * Other types need to be driver / IOCTL initialized.
	 */
	ret = ttm_bo_init_mm(bdev, TTM_PL_SYSTEM, 0);
	if (unlikely(ret != 0))
		goto out_no_sys;

	bdev->addr_space_rb = RB_ROOT;
	ret = drm_mm_init(&bdev->addr_space_mm, file_page_offset, 0x10000000);
	if (unlikely(ret != 0))
		goto out_no_addr_mm;

	INIT_DELAYED_WORK(&bdev->wq, ttm_bo_delayed_workqueue);
	bdev->nice_mode = true;
	INIT_LIST_HEAD(&bdev->ddestroy);
	bdev->dev_mapping = NULL;
	bdev->glob = glob;
	bdev->need_dma32 = need_dma32;
	bdev->val_seq = 0;
	spin_lock_init(&bdev->fence_lock);
	mutex_lock(&glob->device_list_mutex);
	list_add_tail(&bdev->device_list, &glob->device_list);
	mutex_unlock(&glob->device_list_mutex);

	return 0;
out_no_addr_mm:
	ttm_bo_clean_mm(bdev, 0);
out_no_sys:
	return ret;
}
EXPORT_SYMBOL(ttm_bo_device_init);
=======
	if (bo->type == ttm_bo_type_device || bo->type == ttm_bo_type_sg) {
		ret = drm_vma_offset_add(bdev->vma_manager, &bo->base.vma_node,
					 PFN_UP(bo->base.size));
		if (ret)
			goto err_put;
	}

	/* passed reservation objects should already be locked,
	 * since otherwise lockdep will be angered in radeon.
	 */
	if (!resv)
		WARN_ON(!dma_resv_trylock(bo->base.resv));
	else
		dma_resv_assert_held(resv);

	ret = ttm_bo_validate(bo, placement, ctx);
	if (unlikely(ret))
		goto err_unlock;

	return 0;

err_unlock:
	if (!resv)
		dma_resv_unlock(bo->base.resv);

err_put:
	ttm_bo_put(bo);
	return ret;
}
EXPORT_SYMBOL(ttm_bo_init_reserved);

/**
 * ttm_bo_init_validate
 *
 * @bdev: Pointer to a ttm_device struct.
 * @bo: Pointer to a ttm_buffer_object to be initialized.
 * @type: Requested type of buffer object.
 * @placement: Initial placement for buffer object.
 * @alignment: Data alignment in pages.
 * @interruptible: If needing to sleep to wait for GPU resources,
 * sleep interruptible.
 * pinned in physical memory. If this behaviour is not desired, this member
 * holds a pointer to a persistent shmem object. Typically, this would
 * point to the shmem object backing a GEM object if TTM is used to back a
 * GEM user interface.
 * @sg: Scatter-gather table.
 * @resv: Pointer to a dma_resv, or NULL to let ttm allocate one.
 * @destroy: Destroy function. Use NULL for kfree().
 *
 * This function initializes a pre-allocated struct ttm_buffer_object.
 * As this object may be part of a larger structure, this function,
 * together with the @destroy function,
 * enables driver-specific objects derived from a ttm_buffer_object.
 *
 * On successful return, the caller owns an object kref to @bo. The kref and
 * list_kref are usually set to 1, but note that in some situations, other
 * tasks may already be holding references to @bo as well.
 *
 * If a failure occurs, the function will call the @destroy function, Thus,
 * after a failure, dereferencing @bo is illegal and will likely cause memory
 * corruption.
 *
 * Returns
 * -ENOMEM: Out of memory.
 * -EINVAL: Invalid placement flags.
 * -ERESTARTSYS: Interrupted by signal while sleeping waiting for resources.
 */
int ttm_bo_init_validate(struct ttm_device *bdev, struct ttm_buffer_object *bo,
			 enum ttm_bo_type type, struct ttm_placement *placement,
			 uint32_t alignment, bool interruptible,
			 struct sg_table *sg, struct dma_resv *resv,
			 void (*destroy) (struct ttm_buffer_object *))
{
	struct ttm_operation_ctx ctx = { interruptible, false };
	int ret;

	ret = ttm_bo_init_reserved(bdev, bo, type, placement, alignment, &ctx,
				   sg, resv, destroy);
	if (ret)
		return ret;

	if (!resv)
		ttm_bo_unreserve(bo);

	return 0;
}
EXPORT_SYMBOL(ttm_bo_init_validate);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

/*
 * buffer object vm functions.
 */

<<<<<<< HEAD
bool ttm_mem_reg_is_pci(struct ttm_bo_device *bdev, struct ttm_mem_reg *mem)
{
	struct ttm_mem_type_manager *man = &bdev->man[mem->mem_type];

	if (!(man->flags & TTM_MEMTYPE_FLAG_FIXED)) {
		if (mem->mem_type == TTM_PL_SYSTEM)
			return false;

		if (man->flags & TTM_MEMTYPE_FLAG_CMA)
			return false;

		if (mem->placement & TTM_PL_FLAG_CACHED)
			return false;
	}
	return true;
}

void ttm_bo_unmap_virtual_locked(struct ttm_buffer_object *bo)
{
	struct ttm_bo_device *bdev = bo->bdev;
	loff_t offset = (loff_t) bo->addr_space_offset;
	loff_t holelen = ((loff_t) bo->mem.num_pages) << PAGE_SHIFT;

	if (!bdev->dev_mapping)
		return;
	unmap_mapping_range(bdev->dev_mapping, offset, holelen, 1);
	ttm_mem_io_free_vm(bo);
}

void ttm_bo_unmap_virtual(struct ttm_buffer_object *bo)
{
	struct ttm_bo_device *bdev = bo->bdev;
	struct ttm_mem_type_manager *man = &bdev->man[bo->mem.mem_type];

	ttm_mem_io_lock(man, false);
	ttm_bo_unmap_virtual_locked(bo);
	ttm_mem_io_unlock(man);
}


EXPORT_SYMBOL(ttm_bo_unmap_virtual);

static void ttm_bo_vm_insert_rb(struct ttm_buffer_object *bo)
{
	struct ttm_bo_device *bdev = bo->bdev;
	struct rb_node **cur = &bdev->addr_space_rb.rb_node;
	struct rb_node *parent = NULL;
	struct ttm_buffer_object *cur_bo;
	unsigned long offset = bo->vm_node->start;
	unsigned long cur_offset;

	while (*cur) {
		parent = *cur;
		cur_bo = rb_entry(parent, struct ttm_buffer_object, vm_rb);
		cur_offset = cur_bo->vm_node->start;
		if (offset < cur_offset)
			cur = &parent->rb_left;
		else if (offset > cur_offset)
			cur = &parent->rb_right;
		else
			BUG();
	}

	rb_link_node(&bo->vm_rb, parent, cur);
	rb_insert_color(&bo->vm_rb, &bdev->addr_space_rb);
}

/**
 * ttm_bo_setup_vm:
 *
 * @bo: the buffer to allocate address space for
 *
 * Allocate address space in the drm device so that applications
 * can mmap the buffer and access the contents. This only
 * applies to ttm_bo_type_device objects as others are not
 * placed in the drm device address space.
 */

static int ttm_bo_setup_vm(struct ttm_buffer_object *bo)
{
	struct ttm_bo_device *bdev = bo->bdev;
	int ret;

retry_pre_get:
	ret = drm_mm_pre_get(&bdev->addr_space_mm);
	if (unlikely(ret != 0))
		return ret;

	write_lock(&bdev->vm_lock);
	bo->vm_node = drm_mm_search_free(&bdev->addr_space_mm,
					 bo->mem.num_pages, 0, 0);

	if (unlikely(bo->vm_node == NULL)) {
		ret = -ENOMEM;
		goto out_unlock;
	}

	bo->vm_node = drm_mm_get_block_atomic(bo->vm_node,
					      bo->mem.num_pages, 0);

	if (unlikely(bo->vm_node == NULL)) {
		write_unlock(&bdev->vm_lock);
		goto retry_pre_get;
	}

	ttm_bo_vm_insert_rb(bo);
	write_unlock(&bdev->vm_lock);
	bo->addr_space_offset = ((uint64_t) bo->vm_node->start) << PAGE_SHIFT;

	return 0;
out_unlock:
	write_unlock(&bdev->vm_lock);
	return ret;
}

int ttm_bo_wait(struct ttm_buffer_object *bo,
		bool lazy, bool interruptible, bool no_wait)
{
	struct ttm_bo_driver *driver = bo->bdev->driver;
	struct ttm_bo_device *bdev = bo->bdev;
	void *sync_obj;
	void *sync_obj_arg;
	int ret = 0;

	if (likely(bo->sync_obj == NULL))
		return 0;

	while (bo->sync_obj) {

		if (driver->sync_obj_signaled(bo->sync_obj, bo->sync_obj_arg)) {
			void *tmp_obj = bo->sync_obj;
			bo->sync_obj = NULL;
			clear_bit(TTM_BO_PRIV_FLAG_MOVING, &bo->priv_flags);
			spin_unlock(&bdev->fence_lock);
			driver->sync_obj_unref(&tmp_obj);
			spin_lock(&bdev->fence_lock);
			continue;
		}

		if (no_wait)
			return -EBUSY;

		sync_obj = driver->sync_obj_ref(bo->sync_obj);
		sync_obj_arg = bo->sync_obj_arg;
		spin_unlock(&bdev->fence_lock);
		ret = driver->sync_obj_wait(sync_obj, sync_obj_arg,
					    lazy, interruptible);
		if (unlikely(ret != 0)) {
			driver->sync_obj_unref(&sync_obj);
			spin_lock(&bdev->fence_lock);
			return ret;
		}
		spin_lock(&bdev->fence_lock);
		if (likely(bo->sync_obj == sync_obj &&
			   bo->sync_obj_arg == sync_obj_arg)) {
			void *tmp_obj = bo->sync_obj;
			bo->sync_obj = NULL;
			clear_bit(TTM_BO_PRIV_FLAG_MOVING,
				  &bo->priv_flags);
			spin_unlock(&bdev->fence_lock);
			driver->sync_obj_unref(&sync_obj);
			driver->sync_obj_unref(&tmp_obj);
			spin_lock(&bdev->fence_lock);
		} else {
			spin_unlock(&bdev->fence_lock);
			driver->sync_obj_unref(&sync_obj);
			spin_lock(&bdev->fence_lock);
		}
	}
	return 0;
}
EXPORT_SYMBOL(ttm_bo_wait);

int ttm_bo_synccpu_write_grab(struct ttm_buffer_object *bo, bool no_wait)
{
	struct ttm_bo_device *bdev = bo->bdev;
	int ret = 0;

	/*
	 * Using ttm_bo_reserve makes sure the lru lists are updated.
	 */

	ret = ttm_bo_reserve(bo, true, no_wait, false, 0);
	if (unlikely(ret != 0))
		return ret;
	spin_lock(&bdev->fence_lock);
	ret = ttm_bo_wait(bo, false, true, no_wait);
	spin_unlock(&bdev->fence_lock);
	if (likely(ret == 0))
		atomic_inc(&bo->cpu_writers);
	ttm_bo_unreserve(bo);
	return ret;
}
EXPORT_SYMBOL(ttm_bo_synccpu_write_grab);

void ttm_bo_synccpu_write_release(struct ttm_buffer_object *bo)
{
	if (atomic_dec_and_test(&bo->cpu_writers))
		wake_up_all(&bo->event_queue);
}
EXPORT_SYMBOL(ttm_bo_synccpu_write_release);

/**
 * A buffer object shrink method that tries to swap out the first
 * buffer object on the bo_global::swap_lru list.
 */

static int ttm_bo_swapout(struct ttm_mem_shrink *shrink)
{
	struct ttm_bo_global *glob =
	    container_of(shrink, struct ttm_bo_global, shrink);
	struct ttm_buffer_object *bo;
	int ret = -EBUSY;
	int put_count;
	uint32_t swap_placement = (TTM_PL_FLAG_CACHED | TTM_PL_FLAG_SYSTEM);

	spin_lock(&glob->lru_lock);
	while (ret == -EBUSY) {
		if (unlikely(list_empty(&glob->swap_lru))) {
			spin_unlock(&glob->lru_lock);
			return -EBUSY;
		}

		bo = list_first_entry(&glob->swap_lru,
				      struct ttm_buffer_object, swap);
		kref_get(&bo->list_kref);

		if (!list_empty(&bo->ddestroy)) {
			spin_unlock(&glob->lru_lock);
			(void) ttm_bo_cleanup_refs(bo, false, false, false);
			kref_put(&bo->list_kref, ttm_bo_release_list);
			spin_lock(&glob->lru_lock);
			continue;
		}

		/**
		 * Reserve buffer. Since we unlock while sleeping, we need
		 * to re-check that nobody removed us from the swap-list while
		 * we slept.
		 */

		ret = ttm_bo_reserve_locked(bo, false, true, false, 0);
		if (unlikely(ret == -EBUSY)) {
			spin_unlock(&glob->lru_lock);
			ttm_bo_wait_unreserved(bo, false);
			kref_put(&bo->list_kref, ttm_bo_release_list);
			spin_lock(&glob->lru_lock);
		}
	}

	BUG_ON(ret != 0);
	put_count = ttm_bo_del_from_lru(bo);
	spin_unlock(&glob->lru_lock);

	ttm_bo_list_ref_sub(bo, put_count, true);

	/**
	 * Wait for GPU, then move to system cached.
	 */

	spin_lock(&bo->bdev->fence_lock);
	ret = ttm_bo_wait(bo, false, false, false);
	spin_unlock(&bo->bdev->fence_lock);

	if (unlikely(ret != 0))
		goto out;

	if ((bo->mem.placement & swap_placement) != swap_placement) {
		struct ttm_mem_reg evict_mem;

		evict_mem = bo->mem;
		evict_mem.mm_node = NULL;
		evict_mem.placement = TTM_PL_FLAG_SYSTEM | TTM_PL_FLAG_CACHED;
		evict_mem.mem_type = TTM_PL_SYSTEM;

		ret = ttm_bo_handle_move_mem(bo, &evict_mem, true,
					     false, false, false);
		if (unlikely(ret != 0))
			goto out;
	}

	ttm_bo_unmap_virtual(bo);

	/**
	 * Swap out. Buffer will be swapped in again as soon as
	 * anyone tries to access a ttm page.
	 */

	if (bo->bdev->driver->swap_notify)
		bo->bdev->driver->swap_notify(bo);

	ret = ttm_tt_swapout(bo->ttm, bo->persistent_swap_storage);
out:

	/**
	 *
	 * Unreserve without putting on LRU to avoid swapping out an
	 * already swapped buffer.
	 */

	atomic_set(&bo->reserved, 0);
	wake_up_all(&bo->event_queue);
	kref_put(&bo->list_kref, ttm_bo_release_list);
	return ret;
}

void ttm_bo_swapout_all(struct ttm_bo_device *bdev)
{
	while (ttm_bo_swapout(&bdev->glob->shrink) == 0)
		;
}
EXPORT_SYMBOL(ttm_bo_swapout_all);
=======
/**
 * ttm_bo_unmap_virtual
 *
 * @bo: tear down the virtual mappings for this BO
 */
void ttm_bo_unmap_virtual(struct ttm_buffer_object *bo)
{
	struct ttm_device *bdev = bo->bdev;

	drm_vma_node_unmap(&bo->base.vma_node, bdev->dev_mapping);
	ttm_mem_io_free(bdev, bo->resource);
}
EXPORT_SYMBOL(ttm_bo_unmap_virtual);

/**
 * ttm_bo_wait_ctx - wait for buffer idle.
 *
 * @bo:  The buffer object.
 * @ctx: defines how to wait
 *
 * Waits for the buffer to be idle. Used timeout depends on the context.
 * Returns -EBUSY if wait timed outt, -ERESTARTSYS if interrupted by a signal or
 * zero on success.
 */
int ttm_bo_wait_ctx(struct ttm_buffer_object *bo, struct ttm_operation_ctx *ctx)
{
	long ret;

	if (ctx->no_wait_gpu) {
		if (dma_resv_test_signaled(bo->base.resv,
					   DMA_RESV_USAGE_BOOKKEEP))
			return 0;
		else
			return -EBUSY;
	}

	ret = dma_resv_wait_timeout(bo->base.resv, DMA_RESV_USAGE_BOOKKEEP,
				    ctx->interruptible, 15 * HZ);
	if (unlikely(ret < 0))
		return ret;
	if (unlikely(ret == 0))
		return -EBUSY;
	return 0;
}
EXPORT_SYMBOL(ttm_bo_wait_ctx);

int ttm_bo_swapout(struct ttm_buffer_object *bo, struct ttm_operation_ctx *ctx,
		   gfp_t gfp_flags)
{
	struct ttm_place place;
	bool locked;
	long ret;

	/*
	 * While the bo may already reside in SYSTEM placement, set
	 * SYSTEM as new placement to cover also the move further below.
	 * The driver may use the fact that we're moving from SYSTEM
	 * as an indication that we're about to swap out.
	 */
	memset(&place, 0, sizeof(place));
	place.mem_type = bo->resource->mem_type;
	if (!ttm_bo_evict_swapout_allowable(bo, ctx, &place, &locked, NULL))
		return -EBUSY;

	if (!bo->ttm || !ttm_tt_is_populated(bo->ttm) ||
	    bo->ttm->page_flags & TTM_TT_FLAG_EXTERNAL ||
	    bo->ttm->page_flags & TTM_TT_FLAG_SWAPPED ||
	    !ttm_bo_get_unless_zero(bo)) {
		if (locked)
			dma_resv_unlock(bo->base.resv);
		return -EBUSY;
	}

	if (bo->deleted) {
		ret = ttm_bo_cleanup_refs(bo, false, false, locked);
		ttm_bo_put(bo);
		return ret == -EBUSY ? -ENOSPC : ret;
	}

	/* TODO: Cleanup the locking */
	spin_unlock(&bo->bdev->lru_lock);

	/*
	 * Move to system cached
	 */
	if (bo->resource->mem_type != TTM_PL_SYSTEM) {
		struct ttm_resource *evict_mem;
		struct ttm_place hop;

		memset(&hop, 0, sizeof(hop));
		place.mem_type = TTM_PL_SYSTEM;
		ret = ttm_resource_alloc(bo, &place, &evict_mem);
		if (unlikely(ret))
			goto out;

		ret = ttm_bo_handle_move_mem(bo, evict_mem, true, ctx, &hop);
		if (unlikely(ret != 0)) {
			WARN(ret == -EMULTIHOP, "Unexpected multihop in swaput - likely driver bug.\n");
			ttm_resource_free(bo, &evict_mem);
			goto out;
		}
	}

	/*
	 * Make sure BO is idle.
	 */
	ret = ttm_bo_wait_ctx(bo, ctx);
	if (unlikely(ret != 0))
		goto out;

	ttm_bo_unmap_virtual(bo);

	/*
	 * Swap out. Buffer will be swapped in again as soon as
	 * anyone tries to access a ttm page.
	 */
	if (bo->bdev->funcs->swap_notify)
		bo->bdev->funcs->swap_notify(bo);

	if (ttm_tt_is_populated(bo->ttm))
		ret = ttm_tt_swapout(bo->bdev, bo->ttm, gfp_flags);
out:

	/*
	 * Unreserve without putting on LRU to avoid swapping out an
	 * already swapped buffer.
	 */
	if (locked)
		dma_resv_unlock(bo->base.resv);
	ttm_bo_put(bo);
	return ret == -EBUSY ? -ENOSPC : ret;
}

void ttm_bo_tt_destroy(struct ttm_buffer_object *bo)
{
	if (bo->ttm == NULL)
		return;

	ttm_tt_unpopulate(bo->bdev, bo->ttm);
	ttm_tt_destroy(bo->bdev, bo->ttm);
	bo->ttm = NULL;
}
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
