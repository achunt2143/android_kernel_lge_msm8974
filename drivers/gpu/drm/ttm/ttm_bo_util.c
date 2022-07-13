<<<<<<< HEAD
=======
/* SPDX-License-Identifier: GPL-2.0 OR MIT */
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
/**************************************************************************
 *
 * Copyright (c) 2007-2009 VMware, Inc., Palo Alto, CA., USA
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

<<<<<<< HEAD
#include "ttm/ttm_bo_driver.h"
#include "ttm/ttm_placement.h"
#include <linux/io.h>
#include <linux/highmem.h>
#include <linux/wait.h>
#include <linux/slab.h>
#include <linux/vmalloc.h>
#include <linux/module.h>

void ttm_bo_free_old_node(struct ttm_buffer_object *bo)
{
	ttm_bo_mem_put(bo, &bo->mem);
}

int ttm_bo_move_ttm(struct ttm_buffer_object *bo,
		    bool evict, bool no_wait_reserve,
		    bool no_wait_gpu, struct ttm_mem_reg *new_mem)
{
	struct ttm_tt *ttm = bo->ttm;
	struct ttm_mem_reg *old_mem = &bo->mem;
	int ret;

	if (old_mem->mem_type != TTM_PL_SYSTEM) {
		ttm_tt_unbind(ttm);
		ttm_bo_free_old_node(bo);
		ttm_flag_masked(&old_mem->placement, TTM_PL_FLAG_SYSTEM,
				TTM_PL_MASK_MEM);
		old_mem->mem_type = TTM_PL_SYSTEM;
	}

	ret = ttm_tt_set_placement_caching(ttm, new_mem->placement);
	if (unlikely(ret != 0))
		return ret;

	if (new_mem->mem_type != TTM_PL_SYSTEM) {
		ret = ttm_tt_bind(ttm, new_mem);
		if (unlikely(ret != 0))
			return ret;
	}

	*old_mem = *new_mem;
	new_mem->mm_node = NULL;

	return 0;
}
EXPORT_SYMBOL(ttm_bo_move_ttm);

int ttm_mem_io_lock(struct ttm_mem_type_manager *man, bool interruptible)
{
	if (likely(man->io_reserve_fastpath))
		return 0;

	if (interruptible)
		return mutex_lock_interruptible(&man->io_reserve_mutex);

	mutex_lock(&man->io_reserve_mutex);
	return 0;
}

void ttm_mem_io_unlock(struct ttm_mem_type_manager *man)
{
	if (likely(man->io_reserve_fastpath))
		return;

	mutex_unlock(&man->io_reserve_mutex);
}

static int ttm_mem_io_evict(struct ttm_mem_type_manager *man)
{
	struct ttm_buffer_object *bo;

	if (!man->use_io_reserve_lru || list_empty(&man->io_reserve_lru))
		return -EAGAIN;

	bo = list_first_entry(&man->io_reserve_lru,
			      struct ttm_buffer_object,
			      io_reserve_lru);
	list_del_init(&bo->io_reserve_lru);
	ttm_bo_unmap_virtual_locked(bo);

	return 0;
}

static int ttm_mem_io_reserve(struct ttm_bo_device *bdev,
			      struct ttm_mem_reg *mem)
{
	struct ttm_mem_type_manager *man = &bdev->man[mem->mem_type];
	int ret = 0;

	if (!bdev->driver->io_mem_reserve)
		return 0;
	if (likely(man->io_reserve_fastpath))
		return bdev->driver->io_mem_reserve(bdev, mem);

	if (bdev->driver->io_mem_reserve &&
	    mem->bus.io_reserved_count++ == 0) {
retry:
		ret = bdev->driver->io_mem_reserve(bdev, mem);
		if (ret == -EAGAIN) {
			ret = ttm_mem_io_evict(man);
			if (ret == 0)
				goto retry;
		}
	}
	return ret;
}

static void ttm_mem_io_free(struct ttm_bo_device *bdev,
			    struct ttm_mem_reg *mem)
{
	struct ttm_mem_type_manager *man = &bdev->man[mem->mem_type];

	if (likely(man->io_reserve_fastpath))
		return;

	if (bdev->driver->io_mem_reserve &&
	    --mem->bus.io_reserved_count == 0 &&
	    bdev->driver->io_mem_free)
		bdev->driver->io_mem_free(bdev, mem);

}

int ttm_mem_io_reserve_vm(struct ttm_buffer_object *bo)
{
	struct ttm_mem_reg *mem = &bo->mem;
	int ret;

	if (!mem->bus.io_reserved_vm) {
		struct ttm_mem_type_manager *man =
			&bo->bdev->man[mem->mem_type];

		ret = ttm_mem_io_reserve(bo->bdev, mem);
		if (unlikely(ret != 0))
			return ret;
		mem->bus.io_reserved_vm = true;
		if (man->use_io_reserve_lru)
			list_add_tail(&bo->io_reserve_lru,
				      &man->io_reserve_lru);
	}
	return 0;
}

void ttm_mem_io_free_vm(struct ttm_buffer_object *bo)
{
	struct ttm_mem_reg *mem = &bo->mem;

	if (mem->bus.io_reserved_vm) {
		mem->bus.io_reserved_vm = false;
		list_del_init(&bo->io_reserve_lru);
		ttm_mem_io_free(bo->bdev, mem);
	}
}

int ttm_mem_reg_ioremap(struct ttm_bo_device *bdev, struct ttm_mem_reg *mem,
			void **virtual)
{
	struct ttm_mem_type_manager *man = &bdev->man[mem->mem_type];
	int ret;
	void *addr;

	*virtual = NULL;
	(void) ttm_mem_io_lock(man, false);
	ret = ttm_mem_io_reserve(bdev, mem);
	ttm_mem_io_unlock(man);
	if (ret || !mem->bus.is_iomem)
		return ret;

	if (mem->bus.addr) {
		addr = mem->bus.addr;
	} else {
		if (mem->placement & TTM_PL_FLAG_WC)
			addr = ioremap_wc(mem->bus.base + mem->bus.offset, mem->bus.size);
		else
			addr = ioremap_nocache(mem->bus.base + mem->bus.offset, mem->bus.size);
		if (!addr) {
			(void) ttm_mem_io_lock(man, false);
			ttm_mem_io_free(bdev, mem);
			ttm_mem_io_unlock(man);
			return -ENOMEM;
		}
	}
	*virtual = addr;
	return 0;
}

void ttm_mem_reg_iounmap(struct ttm_bo_device *bdev, struct ttm_mem_reg *mem,
			 void *virtual)
{
	struct ttm_mem_type_manager *man;

	man = &bdev->man[mem->mem_type];

	if (virtual && mem->bus.addr == NULL)
		iounmap(virtual);
	(void) ttm_mem_io_lock(man, false);
	ttm_mem_io_free(bdev, mem);
	ttm_mem_io_unlock(man);
}

static int ttm_copy_io_page(void *dst, void *src, unsigned long page)
{
	uint32_t *dstP =
	    (uint32_t *) ((unsigned long)dst + (page << PAGE_SHIFT));
	uint32_t *srcP =
	    (uint32_t *) ((unsigned long)src + (page << PAGE_SHIFT));

	int i;
	for (i = 0; i < PAGE_SIZE / sizeof(uint32_t); ++i)
		iowrite32(ioread32(srcP++), dstP++);
	return 0;
}

static int ttm_copy_io_ttm_page(struct ttm_tt *ttm, void *src,
				unsigned long page,
				pgprot_t prot)
{
	struct page *d = ttm->pages[page];
	void *dst;

	if (!d)
		return -ENOMEM;

	src = (void *)((unsigned long)src + (page << PAGE_SHIFT));

#ifdef CONFIG_X86
	dst = kmap_atomic_prot(d, prot);
#else
	if (pgprot_val(prot) != pgprot_val(PAGE_KERNEL))
		dst = vmap(&d, 1, 0, prot);
	else
		dst = kmap(d);
#endif
	if (!dst)
		return -ENOMEM;

	memcpy_fromio(dst, src, PAGE_SIZE);

#ifdef CONFIG_X86
	kunmap_atomic(dst);
#else
	if (pgprot_val(prot) != pgprot_val(PAGE_KERNEL))
		vunmap(dst);
	else
		kunmap(d);
#endif

	return 0;
}

static int ttm_copy_ttm_io_page(struct ttm_tt *ttm, void *dst,
				unsigned long page,
				pgprot_t prot)
{
	struct page *s = ttm->pages[page];
	void *src;

	if (!s)
		return -ENOMEM;

	dst = (void *)((unsigned long)dst + (page << PAGE_SHIFT));
#ifdef CONFIG_X86
	src = kmap_atomic_prot(s, prot);
#else
	if (pgprot_val(prot) != pgprot_val(PAGE_KERNEL))
		src = vmap(&s, 1, 0, prot);
	else
		src = kmap(s);
#endif
	if (!src)
		return -ENOMEM;

	memcpy_toio(dst, src, PAGE_SIZE);

#ifdef CONFIG_X86
	kunmap_atomic(src);
#else
	if (pgprot_val(prot) != pgprot_val(PAGE_KERNEL))
		vunmap(src);
	else
		kunmap(s);
#endif

	return 0;
}

int ttm_bo_move_memcpy(struct ttm_buffer_object *bo,
		       bool evict, bool no_wait_reserve, bool no_wait_gpu,
		       struct ttm_mem_reg *new_mem)
{
	struct ttm_bo_device *bdev = bo->bdev;
	struct ttm_mem_type_manager *man = &bdev->man[new_mem->mem_type];
	struct ttm_tt *ttm = bo->ttm;
	struct ttm_mem_reg *old_mem = &bo->mem;
	struct ttm_mem_reg old_copy = *old_mem;
	void *old_iomap;
	void *new_iomap;
	int ret;
	unsigned long i;
	unsigned long page;
	unsigned long add = 0;
	int dir;

	ret = ttm_mem_reg_ioremap(bdev, old_mem, &old_iomap);
	if (ret)
		return ret;
	ret = ttm_mem_reg_ioremap(bdev, new_mem, &new_iomap);
	if (ret)
		goto out;

	if (old_iomap == NULL && new_iomap == NULL)
		goto out2;
	if (old_iomap == NULL && ttm == NULL)
		goto out2;

	/* TTM might be null for moves within the same region.
	 */
	if (ttm && ttm->state == tt_unpopulated) {
		ret = ttm->bdev->driver->ttm_tt_populate(ttm);
		if (ret)
			goto out1;
	}

	add = 0;
	dir = 1;

	if ((old_mem->mem_type == new_mem->mem_type) &&
	    (new_mem->start < old_mem->start + old_mem->size)) {
		dir = -1;
		add = new_mem->num_pages - 1;
	}

	for (i = 0; i < new_mem->num_pages; ++i) {
		page = i * dir + add;
		if (old_iomap == NULL) {
			pgprot_t prot = ttm_io_prot(old_mem->placement,
						    PAGE_KERNEL);
			ret = ttm_copy_ttm_io_page(ttm, new_iomap, page,
						   prot);
		} else if (new_iomap == NULL) {
			pgprot_t prot = ttm_io_prot(new_mem->placement,
						    PAGE_KERNEL);
			ret = ttm_copy_io_ttm_page(ttm, old_iomap, page,
						   prot);
		} else
			ret = ttm_copy_io_page(new_iomap, old_iomap, page);
		if (ret)
			goto out1;
	}
	mb();
out2:
	old_copy = *old_mem;
	*old_mem = *new_mem;
	new_mem->mm_node = NULL;

	if ((man->flags & TTM_MEMTYPE_FLAG_FIXED) && (ttm != NULL)) {
		ttm_tt_unbind(ttm);
		ttm_tt_destroy(ttm);
		bo->ttm = NULL;
	}

out1:
	ttm_mem_reg_iounmap(bdev, old_mem, new_iomap);
out:
	ttm_mem_reg_iounmap(bdev, &old_copy, old_iomap);
	ttm_bo_mem_put(bo, &old_copy);
=======
#include <linux/vmalloc.h>

#include <drm/ttm/ttm_bo.h>
#include <drm/ttm/ttm_placement.h>
#include <drm/ttm/ttm_tt.h>

#include <drm/drm_cache.h>

struct ttm_transfer_obj {
	struct ttm_buffer_object base;
	struct ttm_buffer_object *bo;
};

int ttm_mem_io_reserve(struct ttm_device *bdev,
		       struct ttm_resource *mem)
{
	if (mem->bus.offset || mem->bus.addr)
		return 0;

	mem->bus.is_iomem = false;
	if (!bdev->funcs->io_mem_reserve)
		return 0;

	return bdev->funcs->io_mem_reserve(bdev, mem);
}

void ttm_mem_io_free(struct ttm_device *bdev,
		     struct ttm_resource *mem)
{
	if (!mem)
		return;

	if (!mem->bus.offset && !mem->bus.addr)
		return;

	if (bdev->funcs->io_mem_free)
		bdev->funcs->io_mem_free(bdev, mem);

	mem->bus.offset = 0;
	mem->bus.addr = NULL;
}

/**
 * ttm_move_memcpy - Helper to perform a memcpy ttm move operation.
 * @clear: Whether to clear rather than copy.
 * @num_pages: Number of pages of the operation.
 * @dst_iter: A struct ttm_kmap_iter representing the destination resource.
 * @src_iter: A struct ttm_kmap_iter representing the source resource.
 *
 * This function is intended to be able to move out async under a
 * dma-fence if desired.
 */
void ttm_move_memcpy(bool clear,
		     u32 num_pages,
		     struct ttm_kmap_iter *dst_iter,
		     struct ttm_kmap_iter *src_iter)
{
	const struct ttm_kmap_iter_ops *dst_ops = dst_iter->ops;
	const struct ttm_kmap_iter_ops *src_ops = src_iter->ops;
	struct iosys_map src_map, dst_map;
	pgoff_t i;

	/* Single TTM move. NOP */
	if (dst_ops->maps_tt && src_ops->maps_tt)
		return;

	/* Don't move nonexistent data. Clear destination instead. */
	if (clear) {
		for (i = 0; i < num_pages; ++i) {
			dst_ops->map_local(dst_iter, &dst_map, i);
			if (dst_map.is_iomem)
				memset_io(dst_map.vaddr_iomem, 0, PAGE_SIZE);
			else
				memset(dst_map.vaddr, 0, PAGE_SIZE);
			if (dst_ops->unmap_local)
				dst_ops->unmap_local(dst_iter, &dst_map);
		}
		return;
	}

	for (i = 0; i < num_pages; ++i) {
		dst_ops->map_local(dst_iter, &dst_map, i);
		src_ops->map_local(src_iter, &src_map, i);

		drm_memcpy_from_wc(&dst_map, &src_map, PAGE_SIZE);

		if (src_ops->unmap_local)
			src_ops->unmap_local(src_iter, &src_map);
		if (dst_ops->unmap_local)
			dst_ops->unmap_local(dst_iter, &dst_map);
	}
}
EXPORT_SYMBOL(ttm_move_memcpy);

/**
 * ttm_bo_move_memcpy
 *
 * @bo: A pointer to a struct ttm_buffer_object.
 * @ctx: operation context
 * @dst_mem: struct ttm_resource indicating where to move.
 *
 * Fallback move function for a mappable buffer object in mappable memory.
 * The function will, if successful,
 * free any old aperture space, and set (@new_mem)->mm_node to NULL,
 * and update the (@bo)->mem placement flags. If unsuccessful, the old
 * data remains untouched, and it's up to the caller to free the
 * memory space indicated by @new_mem.
 * Returns:
 * !0: Failure.
 */
int ttm_bo_move_memcpy(struct ttm_buffer_object *bo,
		       struct ttm_operation_ctx *ctx,
		       struct ttm_resource *dst_mem)
{
	struct ttm_device *bdev = bo->bdev;
	struct ttm_resource_manager *dst_man =
		ttm_manager_type(bo->bdev, dst_mem->mem_type);
	struct ttm_tt *ttm = bo->ttm;
	struct ttm_resource *src_mem = bo->resource;
	struct ttm_resource_manager *src_man;
	union {
		struct ttm_kmap_iter_tt tt;
		struct ttm_kmap_iter_linear_io io;
	} _dst_iter, _src_iter;
	struct ttm_kmap_iter *dst_iter, *src_iter;
	bool clear;
	int ret = 0;

	if (WARN_ON(!src_mem))
		return -EINVAL;

	src_man = ttm_manager_type(bdev, src_mem->mem_type);
	if (ttm && ((ttm->page_flags & TTM_TT_FLAG_SWAPPED) ||
		    dst_man->use_tt)) {
		ret = ttm_tt_populate(bdev, ttm, ctx);
		if (ret)
			return ret;
	}

	dst_iter = ttm_kmap_iter_linear_io_init(&_dst_iter.io, bdev, dst_mem);
	if (PTR_ERR(dst_iter) == -EINVAL && dst_man->use_tt)
		dst_iter = ttm_kmap_iter_tt_init(&_dst_iter.tt, bo->ttm);
	if (IS_ERR(dst_iter))
		return PTR_ERR(dst_iter);

	src_iter = ttm_kmap_iter_linear_io_init(&_src_iter.io, bdev, src_mem);
	if (PTR_ERR(src_iter) == -EINVAL && src_man->use_tt)
		src_iter = ttm_kmap_iter_tt_init(&_src_iter.tt, bo->ttm);
	if (IS_ERR(src_iter)) {
		ret = PTR_ERR(src_iter);
		goto out_src_iter;
	}

	clear = src_iter->ops->maps_tt && (!ttm || !ttm_tt_is_populated(ttm));
	if (!(clear && ttm && !(ttm->page_flags & TTM_TT_FLAG_ZERO_ALLOC)))
		ttm_move_memcpy(clear, PFN_UP(dst_mem->size), dst_iter, src_iter);

	if (!src_iter->ops->maps_tt)
		ttm_kmap_iter_linear_io_fini(&_src_iter.io, bdev, src_mem);
	ttm_bo_move_sync_cleanup(bo, dst_mem);

out_src_iter:
	if (!dst_iter->ops->maps_tt)
		ttm_kmap_iter_linear_io_fini(&_dst_iter.io, bdev, dst_mem);

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	return ret;
}
EXPORT_SYMBOL(ttm_bo_move_memcpy);

static void ttm_transfered_destroy(struct ttm_buffer_object *bo)
{
<<<<<<< HEAD
	kfree(bo);
=======
	struct ttm_transfer_obj *fbo;

	fbo = container_of(bo, struct ttm_transfer_obj, base);
	dma_resv_fini(&fbo->base.base._resv);
	ttm_bo_put(fbo->bo);
	kfree(fbo);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

/**
 * ttm_buffer_object_transfer
 *
 * @bo: A pointer to a struct ttm_buffer_object.
 * @new_obj: A pointer to a pointer to a newly created ttm_buffer_object,
 * holding the data of @bo with the old placement.
 *
 * This is a utility function that may be called after an accelerated move
 * has been scheduled. A new buffer object is created as a placeholder for
 * the old data while it's being copied. When that buffer object is idle,
 * it can be destroyed, releasing the space of the old placement.
 * Returns:
 * !0: Failure.
 */

static int ttm_buffer_object_transfer(struct ttm_buffer_object *bo,
				      struct ttm_buffer_object **new_obj)
{
<<<<<<< HEAD
	struct ttm_buffer_object *fbo;
	struct ttm_bo_device *bdev = bo->bdev;
	struct ttm_bo_driver *driver = bdev->driver;

	fbo = kzalloc(sizeof(*fbo), GFP_KERNEL);
	if (!fbo)
		return -ENOMEM;

	*fbo = *bo;
=======
	struct ttm_transfer_obj *fbo;
	int ret;

	fbo = kmalloc(sizeof(*fbo), GFP_KERNEL);
	if (!fbo)
		return -ENOMEM;

	fbo->base = *bo;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	/**
	 * Fix up members that we shouldn't copy directly:
	 * TODO: Explicit member copy would probably be better here.
	 */

<<<<<<< HEAD
	init_waitqueue_head(&fbo->event_queue);
	INIT_LIST_HEAD(&fbo->ddestroy);
	INIT_LIST_HEAD(&fbo->lru);
	INIT_LIST_HEAD(&fbo->swap);
	INIT_LIST_HEAD(&fbo->io_reserve_lru);
	fbo->vm_node = NULL;
	atomic_set(&fbo->cpu_writers, 0);

	fbo->sync_obj = driver->sync_obj_ref(bo->sync_obj);
	kref_init(&fbo->list_kref);
	kref_init(&fbo->kref);
	fbo->destroy = &ttm_transfered_destroy;
	fbo->acc_size = 0;

	*new_obj = fbo;
	return 0;
}

pgprot_t ttm_io_prot(uint32_t caching_flags, pgprot_t tmp)
{
#if defined(__i386__) || defined(__x86_64__)
	if (caching_flags & TTM_PL_FLAG_WC)
		tmp = pgprot_writecombine(tmp);
	else if (boot_cpu_data.x86 > 3)
		tmp = pgprot_noncached(tmp);

#elif defined(__powerpc__)
	if (!(caching_flags & TTM_PL_FLAG_CACHED)) {
		pgprot_val(tmp) |= _PAGE_NO_CACHE;
		if (caching_flags & TTM_PL_FLAG_UNCACHED)
			pgprot_val(tmp) |= _PAGE_GUARDED;
	}
#endif
#if defined(__ia64__)
	if (caching_flags & TTM_PL_FLAG_WC)
		tmp = pgprot_writecombine(tmp);
	else
		tmp = pgprot_noncached(tmp);
#endif
#if defined(__sparc__)
	if (!(caching_flags & TTM_PL_FLAG_CACHED))
		tmp = pgprot_noncached(tmp);
#endif
	return tmp;
=======
	atomic_inc(&ttm_glob.bo_count);
	drm_vma_node_reset(&fbo->base.base.vma_node);

	kref_init(&fbo->base.kref);
	fbo->base.destroy = &ttm_transfered_destroy;
	fbo->base.pin_count = 0;
	if (bo->type != ttm_bo_type_sg)
		fbo->base.base.resv = &fbo->base.base._resv;

	dma_resv_init(&fbo->base.base._resv);
	fbo->base.base.dev = NULL;
	ret = dma_resv_trylock(&fbo->base.base._resv);
	WARN_ON(!ret);

	if (fbo->base.resource) {
		ttm_resource_set_bo(fbo->base.resource, &fbo->base);
		bo->resource = NULL;
		ttm_bo_set_bulk_move(&fbo->base, NULL);
	} else {
		fbo->base.bulk_move = NULL;
	}

	ret = dma_resv_reserve_fences(&fbo->base.base._resv, 1);
	if (ret) {
		kfree(fbo);
		return ret;
	}

	ttm_bo_get(bo);
	fbo->bo = bo;

	ttm_bo_move_to_lru_tail_unlocked(&fbo->base);

	*new_obj = &fbo->base;
	return 0;
}

/**
 * ttm_io_prot
 *
 * @bo: ttm buffer object
 * @res: ttm resource object
 * @tmp: Page protection flag for a normal, cached mapping.
 *
 * Utility function that returns the pgprot_t that should be used for
 * setting up a PTE with the caching model indicated by @c_state.
 */
pgprot_t ttm_io_prot(struct ttm_buffer_object *bo, struct ttm_resource *res,
		     pgprot_t tmp)
{
	struct ttm_resource_manager *man;
	enum ttm_caching caching;

	man = ttm_manager_type(bo->bdev, res->mem_type);
	if (man->use_tt) {
		caching = bo->ttm->caching;
		if (bo->ttm->page_flags & TTM_TT_FLAG_DECRYPTED)
			tmp = pgprot_decrypted(tmp);
	} else  {
		caching = res->bus.caching;
	}

	return ttm_prot_from_caching(caching, tmp);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}
EXPORT_SYMBOL(ttm_io_prot);

static int ttm_bo_ioremap(struct ttm_buffer_object *bo,
			  unsigned long offset,
			  unsigned long size,
			  struct ttm_bo_kmap_obj *map)
{
<<<<<<< HEAD
	struct ttm_mem_reg *mem = &bo->mem;

	if (bo->mem.bus.addr) {
		map->bo_kmap_type = ttm_bo_map_premapped;
		map->virtual = (void *)(((u8 *)bo->mem.bus.addr) + offset);
	} else {
		map->bo_kmap_type = ttm_bo_map_iomap;
		if (mem->placement & TTM_PL_FLAG_WC)
			map->virtual = ioremap_wc(bo->mem.bus.base + bo->mem.bus.offset + offset,
						  size);
		else
			map->virtual = ioremap_nocache(bo->mem.bus.base + bo->mem.bus.offset + offset,
						       size);
=======
	struct ttm_resource *mem = bo->resource;

	if (bo->resource->bus.addr) {
		map->bo_kmap_type = ttm_bo_map_premapped;
		map->virtual = ((u8 *)bo->resource->bus.addr) + offset;
	} else {
		resource_size_t res = bo->resource->bus.offset + offset;

		map->bo_kmap_type = ttm_bo_map_iomap;
		if (mem->bus.caching == ttm_write_combined)
			map->virtual = ioremap_wc(res, size);
#ifdef CONFIG_X86
		else if (mem->bus.caching == ttm_cached)
			map->virtual = ioremap_cache(res, size);
#endif
		else
			map->virtual = ioremap(res, size);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	}
	return (!map->virtual) ? -ENOMEM : 0;
}

static int ttm_bo_kmap_ttm(struct ttm_buffer_object *bo,
			   unsigned long start_page,
			   unsigned long num_pages,
			   struct ttm_bo_kmap_obj *map)
{
<<<<<<< HEAD
	struct ttm_mem_reg *mem = &bo->mem; pgprot_t prot;
	struct ttm_tt *ttm = bo->ttm;
=======
	struct ttm_resource *mem = bo->resource;
	struct ttm_operation_ctx ctx = {
		.interruptible = false,
		.no_wait_gpu = false
	};
	struct ttm_tt *ttm = bo->ttm;
	struct ttm_resource_manager *man =
			ttm_manager_type(bo->bdev, bo->resource->mem_type);
	pgprot_t prot;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	int ret;

	BUG_ON(!ttm);

<<<<<<< HEAD
	if (ttm->state == tt_unpopulated) {
		ret = ttm->bdev->driver->ttm_tt_populate(ttm);
		if (ret)
			return ret;
	}

	if (num_pages == 1 && (mem->placement & TTM_PL_FLAG_CACHED)) {
=======
	ret = ttm_tt_populate(bo->bdev, ttm, &ctx);
	if (ret)
		return ret;

	if (num_pages == 1 && ttm->caching == ttm_cached &&
	    !(man->use_tt && (ttm->page_flags & TTM_TT_FLAG_DECRYPTED))) {
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		/*
		 * We're mapping a single page, and the desired
		 * page protection is consistent with the bo.
		 */

		map->bo_kmap_type = ttm_bo_map_kmap;
		map->page = ttm->pages[start_page];
		map->virtual = kmap(map->page);
	} else {
		/*
		 * We need to use vmap to get the desired page protection
		 * or to make the buffer object look contiguous.
		 */
<<<<<<< HEAD
		prot = (mem->placement & TTM_PL_FLAG_CACHED) ?
			PAGE_KERNEL :
			ttm_io_prot(mem->placement, PAGE_KERNEL);
=======
		prot = ttm_io_prot(bo, mem, PAGE_KERNEL);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		map->bo_kmap_type = ttm_bo_map_vmap;
		map->virtual = vmap(ttm->pages + start_page, num_pages,
				    0, prot);
	}
	return (!map->virtual) ? -ENOMEM : 0;
}

<<<<<<< HEAD
=======
/**
 * ttm_bo_kmap
 *
 * @bo: The buffer object.
 * @start_page: The first page to map.
 * @num_pages: Number of pages to map.
 * @map: pointer to a struct ttm_bo_kmap_obj representing the map.
 *
 * Sets up a kernel virtual mapping, using ioremap, vmap or kmap to the
 * data in the buffer object. The ttm_kmap_obj_virtual function can then be
 * used to obtain a virtual address to the data.
 *
 * Returns
 * -ENOMEM: Out of memory.
 * -EINVAL: Invalid range.
 */
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
int ttm_bo_kmap(struct ttm_buffer_object *bo,
		unsigned long start_page, unsigned long num_pages,
		struct ttm_bo_kmap_obj *map)
{
<<<<<<< HEAD
	struct ttm_mem_type_manager *man =
		&bo->bdev->man[bo->mem.mem_type];
	unsigned long offset, size;
	int ret;

	BUG_ON(!list_empty(&bo->swap));
	map->virtual = NULL;
	map->bo = bo;
	if (num_pages > bo->num_pages)
		return -EINVAL;
	if (start_page > bo->num_pages)
		return -EINVAL;
#if 0
	if (num_pages > 1 && !DRM_SUSER(DRM_CURPROC))
		return -EPERM;
#endif
	(void) ttm_mem_io_lock(man, false);
	ret = ttm_mem_io_reserve(bo->bdev, &bo->mem);
	ttm_mem_io_unlock(man);
	if (ret)
		return ret;
	if (!bo->mem.bus.is_iomem) {
=======
	unsigned long offset, size;
	int ret;

	map->virtual = NULL;
	map->bo = bo;
	if (num_pages > PFN_UP(bo->resource->size))
		return -EINVAL;
	if ((start_page + num_pages) > PFN_UP(bo->resource->size))
		return -EINVAL;

	ret = ttm_mem_io_reserve(bo->bdev, bo->resource);
	if (ret)
		return ret;
	if (!bo->resource->bus.is_iomem) {
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		return ttm_bo_kmap_ttm(bo, start_page, num_pages, map);
	} else {
		offset = start_page << PAGE_SHIFT;
		size = num_pages << PAGE_SHIFT;
		return ttm_bo_ioremap(bo, offset, size, map);
	}
}
EXPORT_SYMBOL(ttm_bo_kmap);

<<<<<<< HEAD
void ttm_bo_kunmap(struct ttm_bo_kmap_obj *map)
{
	struct ttm_buffer_object *bo = map->bo;
	struct ttm_mem_type_manager *man =
		&bo->bdev->man[bo->mem.mem_type];

=======
/**
 * ttm_bo_kunmap
 *
 * @map: Object describing the map to unmap.
 *
 * Unmaps a kernel map set up by ttm_bo_kmap.
 */
void ttm_bo_kunmap(struct ttm_bo_kmap_obj *map)
{
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	if (!map->virtual)
		return;
	switch (map->bo_kmap_type) {
	case ttm_bo_map_iomap:
		iounmap(map->virtual);
		break;
	case ttm_bo_map_vmap:
		vunmap(map->virtual);
		break;
	case ttm_bo_map_kmap:
		kunmap(map->page);
		break;
	case ttm_bo_map_premapped:
		break;
	default:
		BUG();
	}
<<<<<<< HEAD
	(void) ttm_mem_io_lock(man, false);
	ttm_mem_io_free(map->bo->bdev, &map->bo->mem);
	ttm_mem_io_unlock(man);
=======
	ttm_mem_io_free(map->bo->bdev, map->bo->resource);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	map->virtual = NULL;
	map->page = NULL;
}
EXPORT_SYMBOL(ttm_bo_kunmap);

<<<<<<< HEAD
int ttm_bo_move_accel_cleanup(struct ttm_buffer_object *bo,
			      void *sync_obj,
			      void *sync_obj_arg,
			      bool evict, bool no_wait_reserve,
			      bool no_wait_gpu,
			      struct ttm_mem_reg *new_mem)
{
	struct ttm_bo_device *bdev = bo->bdev;
	struct ttm_bo_driver *driver = bdev->driver;
	struct ttm_mem_type_manager *man = &bdev->man[new_mem->mem_type];
	struct ttm_mem_reg *old_mem = &bo->mem;
	int ret;
	struct ttm_buffer_object *ghost_obj;
	void *tmp_obj = NULL;

	spin_lock(&bdev->fence_lock);
	if (bo->sync_obj) {
		tmp_obj = bo->sync_obj;
		bo->sync_obj = NULL;
	}
	bo->sync_obj = driver->sync_obj_ref(sync_obj);
	bo->sync_obj_arg = sync_obj_arg;
	if (evict) {
		ret = ttm_bo_wait(bo, false, false, false);
		spin_unlock(&bdev->fence_lock);
		if (tmp_obj)
			driver->sync_obj_unref(&tmp_obj);
		if (ret)
			return ret;

		if ((man->flags & TTM_MEMTYPE_FLAG_FIXED) &&
		    (bo->ttm != NULL)) {
			ttm_tt_unbind(bo->ttm);
			ttm_tt_destroy(bo->ttm);
			bo->ttm = NULL;
		}
		ttm_bo_free_old_node(bo);
	} else {
		/**
		 * This should help pipeline ordinary buffer moves.
		 *
		 * Hang old buffer memory on a new buffer object,
		 * and leave it to be released when the GPU
		 * operation has completed.
		 */

		set_bit(TTM_BO_PRIV_FLAG_MOVING, &bo->priv_flags);
		spin_unlock(&bdev->fence_lock);
		if (tmp_obj)
			driver->sync_obj_unref(&tmp_obj);

		ret = ttm_buffer_object_transfer(bo, &ghost_obj);
		if (ret)
			return ret;

		/**
		 * If we're not moving to fixed memory, the TTM object
		 * needs to stay alive. Otherwhise hang it on the ghost
		 * bo to be unbound and destroyed.
		 */

		if (!(man->flags & TTM_MEMTYPE_FLAG_FIXED))
			ghost_obj->ttm = NULL;
		else
			bo->ttm = NULL;

		ttm_bo_unreserve(ghost_obj);
		ttm_bo_unref(&ghost_obj);
	}

	*old_mem = *new_mem;
	new_mem->mm_node = NULL;
=======
/**
 * ttm_bo_vmap
 *
 * @bo: The buffer object.
 * @map: pointer to a struct iosys_map representing the map.
 *
 * Sets up a kernel virtual mapping, using ioremap or vmap to the
 * data in the buffer object. The parameter @map returns the virtual
 * address as struct iosys_map. Unmap the buffer with ttm_bo_vunmap().
 *
 * Returns
 * -ENOMEM: Out of memory.
 * -EINVAL: Invalid range.
 */
int ttm_bo_vmap(struct ttm_buffer_object *bo, struct iosys_map *map)
{
	struct ttm_resource *mem = bo->resource;
	int ret;

	dma_resv_assert_held(bo->base.resv);

	ret = ttm_mem_io_reserve(bo->bdev, mem);
	if (ret)
		return ret;

	if (mem->bus.is_iomem) {
		void __iomem *vaddr_iomem;

		if (mem->bus.addr)
			vaddr_iomem = (void __iomem *)mem->bus.addr;
		else if (mem->bus.caching == ttm_write_combined)
			vaddr_iomem = ioremap_wc(mem->bus.offset,
						 bo->base.size);
#ifdef CONFIG_X86
		else if (mem->bus.caching == ttm_cached)
			vaddr_iomem = ioremap_cache(mem->bus.offset,
						  bo->base.size);
#endif
		else
			vaddr_iomem = ioremap(mem->bus.offset, bo->base.size);

		if (!vaddr_iomem)
			return -ENOMEM;

		iosys_map_set_vaddr_iomem(map, vaddr_iomem);

	} else {
		struct ttm_operation_ctx ctx = {
			.interruptible = false,
			.no_wait_gpu = false
		};
		struct ttm_tt *ttm = bo->ttm;
		pgprot_t prot;
		void *vaddr;

		ret = ttm_tt_populate(bo->bdev, ttm, &ctx);
		if (ret)
			return ret;

		/*
		 * We need to use vmap to get the desired page protection
		 * or to make the buffer object look contiguous.
		 */
		prot = ttm_io_prot(bo, mem, PAGE_KERNEL);
		vaddr = vmap(ttm->pages, ttm->num_pages, 0, prot);
		if (!vaddr)
			return -ENOMEM;

		iosys_map_set_vaddr(map, vaddr);
	}

	return 0;
}
EXPORT_SYMBOL(ttm_bo_vmap);

/**
 * ttm_bo_vunmap
 *
 * @bo: The buffer object.
 * @map: Object describing the map to unmap.
 *
 * Unmaps a kernel map set up by ttm_bo_vmap().
 */
void ttm_bo_vunmap(struct ttm_buffer_object *bo, struct iosys_map *map)
{
	struct ttm_resource *mem = bo->resource;

	dma_resv_assert_held(bo->base.resv);

	if (iosys_map_is_null(map))
		return;

	if (!map->is_iomem)
		vunmap(map->vaddr);
	else if (!mem->bus.addr)
		iounmap(map->vaddr_iomem);
	iosys_map_clear(map);

	ttm_mem_io_free(bo->bdev, bo->resource);
}
EXPORT_SYMBOL(ttm_bo_vunmap);

static int ttm_bo_wait_free_node(struct ttm_buffer_object *bo,
				 bool dst_use_tt)
{
	long ret;

	ret = dma_resv_wait_timeout(bo->base.resv, DMA_RESV_USAGE_BOOKKEEP,
				    false, 15 * HZ);
	if (ret == 0)
		return -EBUSY;
	if (ret < 0)
		return ret;

	if (!dst_use_tt)
		ttm_bo_tt_destroy(bo);
	ttm_resource_free(bo, &bo->resource);
	return 0;
}

static int ttm_bo_move_to_ghost(struct ttm_buffer_object *bo,
				struct dma_fence *fence,
				bool dst_use_tt)
{
	struct ttm_buffer_object *ghost_obj;
	int ret;

	/**
	 * This should help pipeline ordinary buffer moves.
	 *
	 * Hang old buffer memory on a new buffer object,
	 * and leave it to be released when the GPU
	 * operation has completed.
	 */

	ret = ttm_buffer_object_transfer(bo, &ghost_obj);
	if (ret)
		return ret;

	dma_resv_add_fence(&ghost_obj->base._resv, fence,
			   DMA_RESV_USAGE_KERNEL);

	/**
	 * If we're not moving to fixed memory, the TTM object
	 * needs to stay alive. Otherwhise hang it on the ghost
	 * bo to be unbound and destroyed.
	 */

	if (dst_use_tt)
		ghost_obj->ttm = NULL;
	else
		bo->ttm = NULL;

	dma_resv_unlock(&ghost_obj->base._resv);
	ttm_bo_put(ghost_obj);
	return 0;
}

static void ttm_bo_move_pipeline_evict(struct ttm_buffer_object *bo,
				       struct dma_fence *fence)
{
	struct ttm_device *bdev = bo->bdev;
	struct ttm_resource_manager *from;

	from = ttm_manager_type(bdev, bo->resource->mem_type);

	/**
	 * BO doesn't have a TTM we need to bind/unbind. Just remember
	 * this eviction and free up the allocation
	 */
	spin_lock(&from->move_lock);
	if (!from->move || dma_fence_is_later(fence, from->move)) {
		dma_fence_put(from->move);
		from->move = dma_fence_get(fence);
	}
	spin_unlock(&from->move_lock);

	ttm_resource_free(bo, &bo->resource);
}

/**
 * ttm_bo_move_accel_cleanup - cleanup helper for hw copies
 *
 * @bo: A pointer to a struct ttm_buffer_object.
 * @fence: A fence object that signals when moving is complete.
 * @evict: This is an evict move. Don't return until the buffer is idle.
 * @pipeline: evictions are to be pipelined.
 * @new_mem: struct ttm_resource indicating where to move.
 *
 * Accelerated move function to be called when an accelerated move
 * has been scheduled. The function will create a new temporary buffer object
 * representing the old placement, and put the sync object on both buffer
 * objects. After that the newly created buffer object is unref'd to be
 * destroyed when the move is complete. This will help pipeline
 * buffer moves.
 */
int ttm_bo_move_accel_cleanup(struct ttm_buffer_object *bo,
			      struct dma_fence *fence,
			      bool evict,
			      bool pipeline,
			      struct ttm_resource *new_mem)
{
	struct ttm_device *bdev = bo->bdev;
	struct ttm_resource_manager *from = ttm_manager_type(bdev, bo->resource->mem_type);
	struct ttm_resource_manager *man = ttm_manager_type(bdev, new_mem->mem_type);
	int ret = 0;

	dma_resv_add_fence(bo->base.resv, fence, DMA_RESV_USAGE_KERNEL);
	if (!evict)
		ret = ttm_bo_move_to_ghost(bo, fence, man->use_tt);
	else if (!from->use_tt && pipeline)
		ttm_bo_move_pipeline_evict(bo, fence);
	else
		ret = ttm_bo_wait_free_node(bo, man->use_tt);

	if (ret)
		return ret;

	ttm_bo_assign_mem(bo, new_mem);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	return 0;
}
EXPORT_SYMBOL(ttm_bo_move_accel_cleanup);
<<<<<<< HEAD
=======

/**
 * ttm_bo_move_sync_cleanup - cleanup by waiting for the move to finish
 *
 * @bo: A pointer to a struct ttm_buffer_object.
 * @new_mem: struct ttm_resource indicating where to move.
 *
 * Special case of ttm_bo_move_accel_cleanup where the bo is guaranteed
 * by the caller to be idle. Typically used after memcpy buffer moves.
 */
void ttm_bo_move_sync_cleanup(struct ttm_buffer_object *bo,
			      struct ttm_resource *new_mem)
{
	struct ttm_device *bdev = bo->bdev;
	struct ttm_resource_manager *man = ttm_manager_type(bdev, new_mem->mem_type);
	int ret;

	ret = ttm_bo_wait_free_node(bo, man->use_tt);
	if (WARN_ON(ret))
		return;

	ttm_bo_assign_mem(bo, new_mem);
}
EXPORT_SYMBOL(ttm_bo_move_sync_cleanup);

/**
 * ttm_bo_pipeline_gutting - purge the contents of a bo
 * @bo: The buffer object
 *
 * Purge the contents of a bo, async if the bo is not idle.
 * After a successful call, the bo is left unpopulated in
 * system placement. The function may wait uninterruptible
 * for idle on OOM.
 *
 * Return: 0 if successful, negative error code on failure.
 */
int ttm_bo_pipeline_gutting(struct ttm_buffer_object *bo)
{
	struct ttm_buffer_object *ghost;
	struct ttm_tt *ttm;
	int ret;

	/* If already idle, no need for ghost object dance. */
	if (dma_resv_test_signaled(bo->base.resv, DMA_RESV_USAGE_BOOKKEEP)) {
		if (!bo->ttm) {
			/* See comment below about clearing. */
			ret = ttm_tt_create(bo, true);
			if (ret)
				return ret;
		} else {
			ttm_tt_unpopulate(bo->bdev, bo->ttm);
			if (bo->type == ttm_bo_type_device)
				ttm_tt_mark_for_clear(bo->ttm);
		}
		ttm_resource_free(bo, &bo->resource);
		return 0;
	}

	/*
	 * We need an unpopulated ttm_tt after giving our current one,
	 * if any, to the ghost object. And we can't afford to fail
	 * creating one *after* the operation. If the bo subsequently gets
	 * resurrected, make sure it's cleared (if ttm_bo_type_device)
	 * to avoid leaking sensitive information to user-space.
	 */

	ttm = bo->ttm;
	bo->ttm = NULL;
	ret = ttm_tt_create(bo, true);
	swap(bo->ttm, ttm);
	if (ret)
		return ret;

	ret = ttm_buffer_object_transfer(bo, &ghost);
	if (ret)
		goto error_destroy_tt;

	ret = dma_resv_copy_fences(&ghost->base._resv, bo->base.resv);
	/* Last resort, wait for the BO to be idle when we are OOM */
	if (ret) {
		dma_resv_wait_timeout(bo->base.resv, DMA_RESV_USAGE_BOOKKEEP,
				      false, MAX_SCHEDULE_TIMEOUT);
	}

	dma_resv_unlock(&ghost->base._resv);
	ttm_bo_put(ghost);
	bo->ttm = ttm;
	return 0;

error_destroy_tt:
	ttm_tt_destroy(bo->bdev, ttm);
	return ret;
}
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
