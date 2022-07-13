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
#include <ttm/ttm_module.h>
#include <ttm/ttm_bo_driver.h>
#include <ttm/ttm_placement.h>
#include <linux/mm.h>
#include <linux/rbtree.h>
#include <linux/module.h>
#include <linux/uaccess.h>

#define TTM_BO_VM_NUM_PREFAULT 16

static struct ttm_buffer_object *ttm_bo_vm_lookup_rb(struct ttm_bo_device *bdev,
						     unsigned long page_start,
						     unsigned long num_pages)
{
	struct rb_node *cur = bdev->addr_space_rb.rb_node;
	unsigned long cur_offset;
	struct ttm_buffer_object *bo;
	struct ttm_buffer_object *best_bo = NULL;

	while (likely(cur != NULL)) {
		bo = rb_entry(cur, struct ttm_buffer_object, vm_rb);
		cur_offset = bo->vm_node->start;
		if (page_start >= cur_offset) {
			cur = cur->rb_right;
			best_bo = bo;
			if (page_start == cur_offset)
				break;
		} else
			cur = cur->rb_left;
	}

	if (unlikely(best_bo == NULL))
		return NULL;

	if (unlikely((best_bo->vm_node->start + best_bo->num_pages) <
		     (page_start + num_pages)))
		return NULL;

	return best_bo;
}

static int ttm_bo_vm_fault(struct vm_area_struct *vma, struct vm_fault *vmf)
{
	struct ttm_buffer_object *bo = (struct ttm_buffer_object *)
	    vma->vm_private_data;
	struct ttm_bo_device *bdev = bo->bdev;
=======
#include <drm/ttm/ttm_bo.h>
#include <drm/ttm/ttm_placement.h>
#include <drm/ttm/ttm_tt.h>

#include <drm/drm_drv.h>
#include <drm/drm_managed.h>

static vm_fault_t ttm_bo_vm_fault_idle(struct ttm_buffer_object *bo,
				struct vm_fault *vmf)
{
	long err = 0;

	/*
	 * Quick non-stalling check for idle.
	 */
	if (dma_resv_test_signaled(bo->base.resv, DMA_RESV_USAGE_KERNEL))
		return 0;

	/*
	 * If possible, avoid waiting for GPU with mmap_lock
	 * held.  We only do this if the fault allows retry and this
	 * is the first attempt.
	 */
	if (fault_flag_allow_retry_first(vmf->flags)) {
		if (vmf->flags & FAULT_FLAG_RETRY_NOWAIT)
			return VM_FAULT_RETRY;

		ttm_bo_get(bo);
		mmap_read_unlock(vmf->vma->vm_mm);
		(void)dma_resv_wait_timeout(bo->base.resv,
					    DMA_RESV_USAGE_KERNEL, true,
					    MAX_SCHEDULE_TIMEOUT);
		dma_resv_unlock(bo->base.resv);
		ttm_bo_put(bo);
		return VM_FAULT_RETRY;
	}

	/*
	 * Ordinary wait.
	 */
	err = dma_resv_wait_timeout(bo->base.resv, DMA_RESV_USAGE_KERNEL, true,
				    MAX_SCHEDULE_TIMEOUT);
	if (unlikely(err < 0)) {
		return (err != -ERESTARTSYS) ? VM_FAULT_SIGBUS :
			VM_FAULT_NOPAGE;
	}

	return 0;
}

static unsigned long ttm_bo_io_mem_pfn(struct ttm_buffer_object *bo,
				       unsigned long page_offset)
{
	struct ttm_device *bdev = bo->bdev;

	if (bdev->funcs->io_mem_pfn)
		return bdev->funcs->io_mem_pfn(bo, page_offset);

	return (bo->resource->bus.offset >> PAGE_SHIFT) + page_offset;
}

/**
 * ttm_bo_vm_reserve - Reserve a buffer object in a retryable vm callback
 * @bo: The buffer object
 * @vmf: The fault structure handed to the callback
 *
 * vm callbacks like fault() and *_mkwrite() allow for the mmap_lock to be dropped
 * during long waits, and after the wait the callback will be restarted. This
 * is to allow other threads using the same virtual memory space concurrent
 * access to map(), unmap() completely unrelated buffer objects. TTM buffer
 * object reservations sometimes wait for GPU and should therefore be
 * considered long waits. This function reserves the buffer object interruptibly
 * taking this into account. Starvation is avoided by the vm system not
 * allowing too many repeated restarts.
 * This function is intended to be used in customized fault() and _mkwrite()
 * handlers.
 *
 * Return:
 *    0 on success and the bo was reserved.
 *    VM_FAULT_RETRY if blocking wait.
 *    VM_FAULT_NOPAGE if blocking wait and retrying was not allowed.
 */
vm_fault_t ttm_bo_vm_reserve(struct ttm_buffer_object *bo,
			     struct vm_fault *vmf)
{
	/*
	 * Work around locking order reversal in fault / nopfn
	 * between mmap_lock and bo_reserve: Perform a trylock operation
	 * for reserve, and if it fails, retry the fault after waiting
	 * for the buffer to become unreserved.
	 */
	if (unlikely(!dma_resv_trylock(bo->base.resv))) {
		/*
		 * If the fault allows retry and this is the first
		 * fault attempt, we try to release the mmap_lock
		 * before waiting
		 */
		if (fault_flag_allow_retry_first(vmf->flags)) {
			if (!(vmf->flags & FAULT_FLAG_RETRY_NOWAIT)) {
				ttm_bo_get(bo);
				mmap_read_unlock(vmf->vma->vm_mm);
				if (!dma_resv_lock_interruptible(bo->base.resv,
								 NULL))
					dma_resv_unlock(bo->base.resv);
				ttm_bo_put(bo);
			}

			return VM_FAULT_RETRY;
		}

		if (dma_resv_lock_interruptible(bo->base.resv, NULL))
			return VM_FAULT_NOPAGE;
	}

	/*
	 * Refuse to fault imported pages. This should be handled
	 * (if at all) by redirecting mmap to the exporter.
	 */
	if (bo->ttm && (bo->ttm->page_flags & TTM_TT_FLAG_EXTERNAL)) {
		if (!(bo->ttm->page_flags & TTM_TT_FLAG_EXTERNAL_MAPPABLE)) {
			dma_resv_unlock(bo->base.resv);
			return VM_FAULT_SIGBUS;
		}
	}

	return 0;
}
EXPORT_SYMBOL(ttm_bo_vm_reserve);

/**
 * ttm_bo_vm_fault_reserved - TTM fault helper
 * @vmf: The struct vm_fault given as argument to the fault callback
 * @prot: The page protection to be used for this memory area.
 * @num_prefault: Maximum number of prefault pages. The caller may want to
 * specify this based on madvice settings and the size of the GPU object
 * backed by the memory.
 *
 * This function inserts one or more page table entries pointing to the
 * memory backing the buffer object, and then returns a return code
 * instructing the caller to retry the page access.
 *
 * Return:
 *   VM_FAULT_NOPAGE on success or pending signal
 *   VM_FAULT_SIGBUS on unspecified error
 *   VM_FAULT_OOM on out-of-memory
 *   VM_FAULT_RETRY if retryable wait
 */
vm_fault_t ttm_bo_vm_fault_reserved(struct vm_fault *vmf,
				    pgprot_t prot,
				    pgoff_t num_prefault)
{
	struct vm_area_struct *vma = vmf->vma;
	struct ttm_buffer_object *bo = vma->vm_private_data;
	struct ttm_device *bdev = bo->bdev;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	unsigned long page_offset;
	unsigned long page_last;
	unsigned long pfn;
	struct ttm_tt *ttm = NULL;
	struct page *page;
<<<<<<< HEAD
	int ret;
	int i;
	unsigned long address = (unsigned long)vmf->virtual_address;
	int retval = VM_FAULT_NOPAGE;
	struct ttm_mem_type_manager *man =
		&bdev->man[bo->mem.mem_type];

	/*
	 * Work around locking order reversal in fault / nopfn
	 * between mmap_sem and bo_reserve: Perform a trylock operation
	 * for reserve, and if it fails, retry the fault after scheduling.
	 */

	ret = ttm_bo_reserve(bo, true, true, false, 0);
	if (unlikely(ret != 0)) {
		if (ret == -EBUSY)
			set_need_resched();
		return VM_FAULT_NOPAGE;
	}

	if (bdev->driver->fault_reserve_notify) {
		ret = bdev->driver->fault_reserve_notify(bo);
		switch (ret) {
		case 0:
			break;
		case -EBUSY:
			set_need_resched();
		case -ERESTARTSYS:
			retval = VM_FAULT_NOPAGE;
			goto out_unlock;
		default:
			retval = VM_FAULT_SIGBUS;
			goto out_unlock;
		}
	}
=======
	int err;
	pgoff_t i;
	vm_fault_t ret = VM_FAULT_NOPAGE;
	unsigned long address = vmf->address;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	/*
	 * Wait for buffer data in transit, due to a pipelined
	 * move.
	 */
<<<<<<< HEAD

	spin_lock(&bdev->fence_lock);
	if (test_bit(TTM_BO_PRIV_FLAG_MOVING, &bo->priv_flags)) {
		ret = ttm_bo_wait(bo, false, true, false);
		spin_unlock(&bdev->fence_lock);
		if (unlikely(ret != 0)) {
			retval = (ret != -ERESTARTSYS) ?
			    VM_FAULT_SIGBUS : VM_FAULT_NOPAGE;
			goto out_unlock;
		}
	} else
		spin_unlock(&bdev->fence_lock);

	ret = ttm_mem_io_lock(man, true);
	if (unlikely(ret != 0)) {
		retval = VM_FAULT_NOPAGE;
		goto out_unlock;
	}
	ret = ttm_mem_io_reserve_vm(bo);
	if (unlikely(ret != 0)) {
		retval = VM_FAULT_SIGBUS;
		goto out_io_unlock;
	}

	page_offset = ((address - vma->vm_start) >> PAGE_SHIFT) +
	    bo->vm_node->start - vma->vm_pgoff;
	page_last = ((vma->vm_end - vma->vm_start) >> PAGE_SHIFT) +
	    bo->vm_node->start - vma->vm_pgoff;

	if (unlikely(page_offset >= bo->num_pages)) {
		retval = VM_FAULT_SIGBUS;
		goto out_io_unlock;
	}

	/*
	 * Strictly, we're not allowed to modify vma->vm_page_prot here,
	 * since the mmap_sem is only held in read mode. However, we
	 * modify only the caching bits of vma->vm_page_prot and
	 * consider those bits protected by
	 * the bo->mutex, as we should be the only writers.
	 * There shouldn't really be any readers of these bits except
	 * within vm_insert_mixed()? fork?
	 *
	 * TODO: Add a list of vmas to the bo, and change the
	 * vma->vm_page_prot when the object changes caching policy, with
	 * the correct locks held.
	 */
	if (bo->mem.bus.is_iomem) {
		vma->vm_page_prot = ttm_io_prot(bo->mem.placement,
						vma->vm_page_prot);
	} else {
		ttm = bo->ttm;
		vma->vm_page_prot = (bo->mem.placement & TTM_PL_FLAG_CACHED) ?
		    vm_get_page_prot(vma->vm_flags) :
		    ttm_io_prot(bo->mem.placement, vma->vm_page_prot);

		/* Allocate all page at once, most common usage */
		if (ttm->bdev->driver->ttm_tt_populate(ttm)) {
			retval = VM_FAULT_OOM;
			goto out_io_unlock;
		}
=======
	ret = ttm_bo_vm_fault_idle(bo, vmf);
	if (unlikely(ret != 0))
		return ret;

	err = ttm_mem_io_reserve(bdev, bo->resource);
	if (unlikely(err != 0))
		return VM_FAULT_SIGBUS;

	page_offset = ((address - vma->vm_start) >> PAGE_SHIFT) +
		vma->vm_pgoff - drm_vma_node_start(&bo->base.vma_node);
	page_last = vma_pages(vma) + vma->vm_pgoff -
		drm_vma_node_start(&bo->base.vma_node);

	if (unlikely(page_offset >= PFN_UP(bo->base.size)))
		return VM_FAULT_SIGBUS;

	prot = ttm_io_prot(bo, bo->resource, prot);
	if (!bo->resource->bus.is_iomem) {
		struct ttm_operation_ctx ctx = {
			.interruptible = true,
			.no_wait_gpu = false,
			.force_alloc = true
		};

		ttm = bo->ttm;
		err = ttm_tt_populate(bdev, bo->ttm, &ctx);
		if (err) {
			if (err == -EINTR || err == -ERESTARTSYS ||
			    err == -EAGAIN)
				return VM_FAULT_NOPAGE;

			pr_debug("TTM fault hit %pe.\n", ERR_PTR(err));
			return VM_FAULT_SIGBUS;
		}
	} else {
		/* Iomem should not be marked encrypted */
		prot = pgprot_decrypted(prot);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	}

	/*
	 * Speculatively prefault a number of pages. Only error on
	 * first page.
	 */
<<<<<<< HEAD
	for (i = 0; i < TTM_BO_VM_NUM_PREFAULT; ++i) {
		if (bo->mem.bus.is_iomem)
			pfn = ((bo->mem.bus.base + bo->mem.bus.offset) >> PAGE_SHIFT) + page_offset;
		else {
			page = ttm->pages[page_offset];
			if (unlikely(!page && i == 0)) {
				retval = VM_FAULT_OOM;
				goto out_io_unlock;
=======
	for (i = 0; i < num_prefault; ++i) {
		if (bo->resource->bus.is_iomem) {
			pfn = ttm_bo_io_mem_pfn(bo, page_offset);
		} else {
			page = ttm->pages[page_offset];
			if (unlikely(!page && i == 0)) {
				return VM_FAULT_OOM;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
			} else if (unlikely(!page)) {
				break;
			}
			pfn = page_to_pfn(page);
		}

<<<<<<< HEAD
		ret = vm_insert_mixed(vma, address, pfn);
		/*
		 * Somebody beat us to this PTE or prefaulting to
		 * an already populated PTE, or prefaulting error.
		 */

		if (unlikely((ret == -EBUSY) || (ret != 0 && i > 0)))
			break;
		else if (unlikely(ret != 0)) {
			retval =
			    (ret == -ENOMEM) ? VM_FAULT_OOM : VM_FAULT_SIGBUS;
			goto out_io_unlock;
=======
		/*
		 * Note that the value of @prot at this point may differ from
		 * the value of @vma->vm_page_prot in the caching- and
		 * encryption bits. This is because the exact location of the
		 * data may not be known at mmap() time and may also change
		 * at arbitrary times while the data is mmap'ed.
		 * See vmf_insert_pfn_prot() for a discussion.
		 */
		ret = vmf_insert_pfn_prot(vma, address, pfn, prot);

		/* Never error on prefaulted PTEs */
		if (unlikely((ret & VM_FAULT_ERROR))) {
			if (i == 0)
				return VM_FAULT_NOPAGE;
			else
				break;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		}

		address += PAGE_SIZE;
		if (unlikely(++page_offset >= page_last))
			break;
	}
<<<<<<< HEAD
out_io_unlock:
	ttm_mem_io_unlock(man);
out_unlock:
	ttm_bo_unreserve(bo);
	return retval;
}

static void ttm_bo_vm_open(struct vm_area_struct *vma)
{
	struct ttm_buffer_object *bo =
	    (struct ttm_buffer_object *)vma->vm_private_data;

	(void)ttm_bo_reference(bo);
}

static void ttm_bo_vm_close(struct vm_area_struct *vma)
{
	struct ttm_buffer_object *bo = (struct ttm_buffer_object *)vma->vm_private_data;

	ttm_bo_unref(&bo);
	vma->vm_private_data = NULL;
}
=======
	return ret;
}
EXPORT_SYMBOL(ttm_bo_vm_fault_reserved);

static void ttm_bo_release_dummy_page(struct drm_device *dev, void *res)
{
	struct page *dummy_page = (struct page *)res;

	__free_page(dummy_page);
}

vm_fault_t ttm_bo_vm_dummy_page(struct vm_fault *vmf, pgprot_t prot)
{
	struct vm_area_struct *vma = vmf->vma;
	struct ttm_buffer_object *bo = vma->vm_private_data;
	struct drm_device *ddev = bo->base.dev;
	vm_fault_t ret = VM_FAULT_NOPAGE;
	unsigned long address;
	unsigned long pfn;
	struct page *page;

	/* Allocate new dummy page to map all the VA range in this VMA to it*/
	page = alloc_page(GFP_KERNEL | __GFP_ZERO);
	if (!page)
		return VM_FAULT_OOM;

	/* Set the page to be freed using drmm release action */
	if (drmm_add_action_or_reset(ddev, ttm_bo_release_dummy_page, page))
		return VM_FAULT_OOM;

	pfn = page_to_pfn(page);

	/* Prefault the entire VMA range right away to avoid further faults */
	for (address = vma->vm_start; address < vma->vm_end;
	     address += PAGE_SIZE)
		ret = vmf_insert_pfn_prot(vma, address, pfn, prot);

	return ret;
}
EXPORT_SYMBOL(ttm_bo_vm_dummy_page);

vm_fault_t ttm_bo_vm_fault(struct vm_fault *vmf)
{
	struct vm_area_struct *vma = vmf->vma;
	pgprot_t prot;
	struct ttm_buffer_object *bo = vma->vm_private_data;
	struct drm_device *ddev = bo->base.dev;
	vm_fault_t ret;
	int idx;

	ret = ttm_bo_vm_reserve(bo, vmf);
	if (ret)
		return ret;

	prot = vma->vm_page_prot;
	if (drm_dev_enter(ddev, &idx)) {
		ret = ttm_bo_vm_fault_reserved(vmf, prot, TTM_BO_VM_NUM_PREFAULT);
		drm_dev_exit(idx);
	} else {
		ret = ttm_bo_vm_dummy_page(vmf, prot);
	}
	if (ret == VM_FAULT_RETRY && !(vmf->flags & FAULT_FLAG_RETRY_NOWAIT))
		return ret;

	dma_resv_unlock(bo->base.resv);

	return ret;
}
EXPORT_SYMBOL(ttm_bo_vm_fault);

void ttm_bo_vm_open(struct vm_area_struct *vma)
{
	struct ttm_buffer_object *bo = vma->vm_private_data;

	WARN_ON(bo->bdev->dev_mapping != vma->vm_file->f_mapping);

	ttm_bo_get(bo);
}
EXPORT_SYMBOL(ttm_bo_vm_open);

void ttm_bo_vm_close(struct vm_area_struct *vma)
{
	struct ttm_buffer_object *bo = vma->vm_private_data;

	ttm_bo_put(bo);
	vma->vm_private_data = NULL;
}
EXPORT_SYMBOL(ttm_bo_vm_close);

static int ttm_bo_vm_access_kmap(struct ttm_buffer_object *bo,
				 unsigned long offset,
				 uint8_t *buf, int len, int write)
{
	unsigned long page = offset >> PAGE_SHIFT;
	unsigned long bytes_left = len;
	int ret;

	/* Copy a page at a time, that way no extra virtual address
	 * mapping is needed
	 */
	offset -= page << PAGE_SHIFT;
	do {
		unsigned long bytes = min(bytes_left, PAGE_SIZE - offset);
		struct ttm_bo_kmap_obj map;
		void *ptr;
		bool is_iomem;

		ret = ttm_bo_kmap(bo, page, 1, &map);
		if (ret)
			return ret;

		ptr = (uint8_t *)ttm_kmap_obj_virtual(&map, &is_iomem) + offset;
		WARN_ON_ONCE(is_iomem);
		if (write)
			memcpy(ptr, buf, bytes);
		else
			memcpy(buf, ptr, bytes);
		ttm_bo_kunmap(&map);

		page++;
		buf += bytes;
		bytes_left -= bytes;
		offset = 0;
	} while (bytes_left);

	return len;
}

int ttm_bo_vm_access(struct vm_area_struct *vma, unsigned long addr,
		     void *buf, int len, int write)
{
	struct ttm_buffer_object *bo = vma->vm_private_data;
	unsigned long offset = (addr) - vma->vm_start +
		((vma->vm_pgoff - drm_vma_node_start(&bo->base.vma_node))
		 << PAGE_SHIFT);
	int ret;

	if (len < 1 || (offset + len) > bo->base.size)
		return -EIO;

	ret = ttm_bo_reserve(bo, true, false, NULL);
	if (ret)
		return ret;

	switch (bo->resource->mem_type) {
	case TTM_PL_SYSTEM:
		fallthrough;
	case TTM_PL_TT:
		ret = ttm_bo_vm_access_kmap(bo, offset, buf, len, write);
		break;
	default:
		if (bo->bdev->funcs->access_memory)
			ret = bo->bdev->funcs->access_memory(
				bo, offset, buf, len, write);
		else
			ret = -EIO;
	}

	ttm_bo_unreserve(bo);

	return ret;
}
EXPORT_SYMBOL(ttm_bo_vm_access);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

static const struct vm_operations_struct ttm_bo_vm_ops = {
	.fault = ttm_bo_vm_fault,
	.open = ttm_bo_vm_open,
<<<<<<< HEAD
	.close = ttm_bo_vm_close
};

int ttm_bo_mmap(struct file *filp, struct vm_area_struct *vma,
		struct ttm_bo_device *bdev)
{
	struct ttm_bo_driver *driver;
	struct ttm_buffer_object *bo;
	int ret;

	read_lock(&bdev->vm_lock);
	bo = ttm_bo_vm_lookup_rb(bdev, vma->vm_pgoff,
				 (vma->vm_end - vma->vm_start) >> PAGE_SHIFT);
	if (likely(bo != NULL))
		ttm_bo_reference(bo);
	read_unlock(&bdev->vm_lock);

	if (unlikely(bo == NULL)) {
		pr_err("Could not find buffer object to map\n");
		return -EINVAL;
	}

	driver = bo->bdev->driver;
	if (unlikely(!driver->verify_access)) {
		ret = -EPERM;
		goto out_unref;
	}
	ret = driver->verify_access(bo, filp);
	if (unlikely(ret != 0))
		goto out_unref;

	vma->vm_ops = &ttm_bo_vm_ops;
=======
	.close = ttm_bo_vm_close,
	.access = ttm_bo_vm_access,
};

/**
 * ttm_bo_mmap_obj - mmap memory backed by a ttm buffer object.
 *
 * @vma:       vma as input from the fbdev mmap method.
 * @bo:        The bo backing the address space.
 *
 * Maps a buffer object.
 */
int ttm_bo_mmap_obj(struct vm_area_struct *vma, struct ttm_buffer_object *bo)
{
	/* Enforce no COW since would have really strange behavior with it. */
	if (is_cow_mapping(vma->vm_flags))
		return -EINVAL;

	ttm_bo_get(bo);

	/*
	 * Drivers may want to override the vm_ops field. Otherwise we
	 * use TTM's default callbacks.
	 */
	if (!vma->vm_ops)
		vma->vm_ops = &ttm_bo_vm_ops;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	/*
	 * Note: We're transferring the bo reference to
	 * vma->vm_private_data here.
	 */

	vma->vm_private_data = bo;
<<<<<<< HEAD
	vma->vm_flags |= VM_RESERVED | VM_IO | VM_MIXEDMAP | VM_DONTEXPAND;
	return 0;
out_unref:
	ttm_bo_unref(&bo);
	return ret;
}
EXPORT_SYMBOL(ttm_bo_mmap);

int ttm_fbdev_mmap(struct vm_area_struct *vma, struct ttm_buffer_object *bo)
{
	if (vma->vm_pgoff != 0)
		return -EACCES;

	vma->vm_ops = &ttm_bo_vm_ops;
	vma->vm_private_data = ttm_bo_reference(bo);
	vma->vm_flags |= VM_RESERVED | VM_IO | VM_MIXEDMAP | VM_DONTEXPAND;
	return 0;
}
EXPORT_SYMBOL(ttm_fbdev_mmap);


ssize_t ttm_bo_io(struct ttm_bo_device *bdev, struct file *filp,
		  const char __user *wbuf, char __user *rbuf, size_t count,
		  loff_t *f_pos, bool write)
{
	struct ttm_buffer_object *bo;
	struct ttm_bo_driver *driver;
	struct ttm_bo_kmap_obj map;
	unsigned long dev_offset = (*f_pos >> PAGE_SHIFT);
	unsigned long kmap_offset;
	unsigned long kmap_end;
	unsigned long kmap_num;
	size_t io_size;
	unsigned int page_offset;
	char *virtual;
	int ret;
	bool no_wait = false;
	bool dummy;

	read_lock(&bdev->vm_lock);
	bo = ttm_bo_vm_lookup_rb(bdev, dev_offset, 1);
	if (likely(bo != NULL))
		ttm_bo_reference(bo);
	read_unlock(&bdev->vm_lock);

	if (unlikely(bo == NULL))
		return -EFAULT;

	driver = bo->bdev->driver;
	if (unlikely(!driver->verify_access)) {
		ret = -EPERM;
		goto out_unref;
	}

	ret = driver->verify_access(bo, filp);
	if (unlikely(ret != 0))
		goto out_unref;

	kmap_offset = dev_offset - bo->vm_node->start;
	if (unlikely(kmap_offset >= bo->num_pages)) {
		ret = -EFBIG;
		goto out_unref;
	}

	page_offset = *f_pos & ~PAGE_MASK;
	io_size = bo->num_pages - kmap_offset;
	io_size = (io_size << PAGE_SHIFT) - page_offset;
	if (count < io_size)
		io_size = count;

	kmap_end = (*f_pos + count - 1) >> PAGE_SHIFT;
	kmap_num = kmap_end - kmap_offset + 1;

	ret = ttm_bo_reserve(bo, true, no_wait, false, 0);

	switch (ret) {
	case 0:
		break;
	case -EBUSY:
		ret = -EAGAIN;
		goto out_unref;
	default:
		goto out_unref;
	}

	ret = ttm_bo_kmap(bo, kmap_offset, kmap_num, &map);
	if (unlikely(ret != 0)) {
		ttm_bo_unreserve(bo);
		goto out_unref;
	}

	virtual = ttm_kmap_obj_virtual(&map, &dummy);
	virtual += page_offset;

	if (write)
		ret = copy_from_user(virtual, wbuf, io_size);
	else
		ret = copy_to_user(rbuf, virtual, io_size);

	ttm_bo_kunmap(&map);
	ttm_bo_unreserve(bo);
	ttm_bo_unref(&bo);

	if (unlikely(ret != 0))
		return -EFBIG;

	*f_pos += io_size;

	return io_size;
out_unref:
	ttm_bo_unref(&bo);
	return ret;
}

ssize_t ttm_bo_fbdev_io(struct ttm_buffer_object *bo, const char __user *wbuf,
			char __user *rbuf, size_t count, loff_t *f_pos,
			bool write)
{
	struct ttm_bo_kmap_obj map;
	unsigned long kmap_offset;
	unsigned long kmap_end;
	unsigned long kmap_num;
	size_t io_size;
	unsigned int page_offset;
	char *virtual;
	int ret;
	bool no_wait = false;
	bool dummy;

	kmap_offset = (*f_pos >> PAGE_SHIFT);
	if (unlikely(kmap_offset >= bo->num_pages))
		return -EFBIG;

	page_offset = *f_pos & ~PAGE_MASK;
	io_size = bo->num_pages - kmap_offset;
	io_size = (io_size << PAGE_SHIFT) - page_offset;
	if (count < io_size)
		io_size = count;

	kmap_end = (*f_pos + count - 1) >> PAGE_SHIFT;
	kmap_num = kmap_end - kmap_offset + 1;

	ret = ttm_bo_reserve(bo, true, no_wait, false, 0);

	switch (ret) {
	case 0:
		break;
	case -EBUSY:
		return -EAGAIN;
	default:
		return ret;
	}

	ret = ttm_bo_kmap(bo, kmap_offset, kmap_num, &map);
	if (unlikely(ret != 0)) {
		ttm_bo_unreserve(bo);
		return ret;
	}

	virtual = ttm_kmap_obj_virtual(&map, &dummy);
	virtual += page_offset;

	if (write)
		ret = copy_from_user(virtual, wbuf, io_size);
	else
		ret = copy_to_user(rbuf, virtual, io_size);

	ttm_bo_kunmap(&map);
	ttm_bo_unreserve(bo);
	ttm_bo_unref(&bo);

	if (unlikely(ret != 0))
		return ret;

	*f_pos += io_size;

	return io_size;
}
=======

	vm_flags_set(vma, VM_PFNMAP | VM_IO | VM_DONTEXPAND | VM_DONTDUMP);
	return 0;
}
EXPORT_SYMBOL(ttm_bo_mmap_obj);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
