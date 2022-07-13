/*
 * Copyright 2009 Jerome Glisse.
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
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NON-INFRINGEMENT. IN NO EVENT SHALL
 * THE COPYRIGHT HOLDERS, AUTHORS AND/OR ITS SUPPLIERS BE LIABLE FOR ANY CLAIM,
 * DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR
 * OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE
 * USE OR OTHER DEALINGS IN THE SOFTWARE.
 *
 * The above copyright notice and this permission notice (including the
 * next paragraph) shall be included in all copies or substantial portions
 * of the Software.
 *
 */
/*
 * Authors:
 *    Jerome Glisse <glisse@freedesktop.org>
 *    Thomas Hellstrom <thomas-at-tungstengraphics-dot-com>
 *    Dave Airlie
 */
<<<<<<< HEAD
#include <linux/list.h>
#include <linux/slab.h>
#include <drm/drmP.h>
#include "radeon_drm.h"
#include "radeon.h"
#include "radeon_trace.h"


int radeon_ttm_init(struct radeon_device *rdev);
void radeon_ttm_fini(struct radeon_device *rdev);
=======

#include <linux/io.h>
#include <linux/list.h>
#include <linux/slab.h>

#include <drm/drm_cache.h>
#include <drm/drm_prime.h>
#include <drm/radeon_drm.h>

#include "radeon.h"
#include "radeon_trace.h"
#include "radeon_ttm.h"

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
static void radeon_bo_clear_surface_reg(struct radeon_bo *bo);

/*
 * To exclude mutual BO access we rely on bo_reserve exclusion, as all
 * function are calling it.
 */

<<<<<<< HEAD
void radeon_bo_clear_va(struct radeon_bo *bo)
{
	struct radeon_bo_va *bo_va, *tmp;

	list_for_each_entry_safe(bo_va, tmp, &bo->va, bo_list) {
		/* remove from all vm address space */
		mutex_lock(&bo_va->vm->mutex);
		list_del(&bo_va->vm_list);
		mutex_unlock(&bo_va->vm->mutex);
		list_del(&bo_va->bo_list);
		kfree(bo_va);
	}
}

=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
static void radeon_ttm_bo_destroy(struct ttm_buffer_object *tbo)
{
	struct radeon_bo *bo;

	bo = container_of(tbo, struct radeon_bo, tbo);
<<<<<<< HEAD
=======

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	mutex_lock(&bo->rdev->gem.mutex);
	list_del_init(&bo->list);
	mutex_unlock(&bo->rdev->gem.mutex);
	radeon_bo_clear_surface_reg(bo);
<<<<<<< HEAD
	radeon_bo_clear_va(bo);
	drm_gem_object_release(&bo->gem_base);
=======
	WARN_ON_ONCE(!list_empty(&bo->va));
	if (bo->tbo.base.import_attach)
		drm_prime_gem_destroy(&bo->tbo.base, bo->tbo.sg);
	drm_gem_object_release(&bo->tbo.base);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	kfree(bo);
}

bool radeon_ttm_bo_is_radeon_bo(struct ttm_buffer_object *bo)
{
	if (bo->destroy == &radeon_ttm_bo_destroy)
		return true;
	return false;
}

void radeon_ttm_placement_from_domain(struct radeon_bo *rbo, u32 domain)
{
<<<<<<< HEAD
	u32 c = 0;

	rbo->placement.fpfn = 0;
	rbo->placement.lpfn = 0;
	rbo->placement.placement = rbo->placements;
	rbo->placement.busy_placement = rbo->placements;
	if (domain & RADEON_GEM_DOMAIN_VRAM)
		rbo->placements[c++] = TTM_PL_FLAG_WC | TTM_PL_FLAG_UNCACHED |
					TTM_PL_FLAG_VRAM;
	if (domain & RADEON_GEM_DOMAIN_GTT)
		rbo->placements[c++] = TTM_PL_MASK_CACHING | TTM_PL_FLAG_TT;
	if (domain & RADEON_GEM_DOMAIN_CPU)
		rbo->placements[c++] = TTM_PL_MASK_CACHING | TTM_PL_FLAG_SYSTEM;
	if (!c)
		rbo->placements[c++] = TTM_PL_MASK_CACHING | TTM_PL_FLAG_SYSTEM;
	rbo->placement.num_placement = c;
	rbo->placement.num_busy_placement = c;
}

int radeon_bo_create(struct radeon_device *rdev,
		     unsigned long size, int byte_align, bool kernel, u32 domain,
=======
	u32 c = 0, i;

	rbo->placement.placement = rbo->placements;
	if (domain & RADEON_GEM_DOMAIN_VRAM) {
		/* Try placing BOs which don't need CPU access outside of the
		 * CPU accessible part of VRAM
		 */
		if ((rbo->flags & RADEON_GEM_NO_CPU_ACCESS) &&
		    rbo->rdev->mc.visible_vram_size < rbo->rdev->mc.real_vram_size) {
			rbo->placements[c].fpfn =
				rbo->rdev->mc.visible_vram_size >> PAGE_SHIFT;
			rbo->placements[c].mem_type = TTM_PL_VRAM;
			rbo->placements[c++].flags = 0;
		}

		rbo->placements[c].fpfn = 0;
		rbo->placements[c].mem_type = TTM_PL_VRAM;
		rbo->placements[c++].flags = 0;
	}

	if (domain & RADEON_GEM_DOMAIN_GTT) {
		rbo->placements[c].fpfn = 0;
		rbo->placements[c].mem_type = TTM_PL_TT;
		rbo->placements[c++].flags = 0;
	}

	if (domain & RADEON_GEM_DOMAIN_CPU) {
		rbo->placements[c].fpfn = 0;
		rbo->placements[c].mem_type = TTM_PL_SYSTEM;
		rbo->placements[c++].flags = 0;
	}
	if (!c) {
		rbo->placements[c].fpfn = 0;
		rbo->placements[c].mem_type = TTM_PL_SYSTEM;
		rbo->placements[c++].flags = 0;
	}

	rbo->placement.num_placement = c;

	for (i = 0; i < c; ++i) {
		if ((rbo->flags & RADEON_GEM_CPU_ACCESS) &&
		    (rbo->placements[i].mem_type == TTM_PL_VRAM) &&
		    !rbo->placements[i].fpfn)
			rbo->placements[i].lpfn =
				rbo->rdev->mc.visible_vram_size >> PAGE_SHIFT;
		else
			rbo->placements[i].lpfn = 0;
	}
}

int radeon_bo_create(struct radeon_device *rdev,
		     unsigned long size, int byte_align, bool kernel,
		     u32 domain, u32 flags, struct sg_table *sg,
		     struct dma_resv *resv,
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		     struct radeon_bo **bo_ptr)
{
	struct radeon_bo *bo;
	enum ttm_bo_type type;
	unsigned long page_align = roundup(byte_align, PAGE_SIZE) >> PAGE_SHIFT;
<<<<<<< HEAD
	unsigned long max_size = 0;
	size_t acc_size;
=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	int r;

	size = ALIGN(size, PAGE_SIZE);

<<<<<<< HEAD
	if (unlikely(rdev->mman.bdev.dev_mapping == NULL)) {
		rdev->mman.bdev.dev_mapping = rdev->ddev->dev_mapping;
	}
	if (kernel) {
		type = ttm_bo_type_kernel;
=======
	if (kernel) {
		type = ttm_bo_type_kernel;
	} else if (sg) {
		type = ttm_bo_type_sg;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	} else {
		type = ttm_bo_type_device;
	}
	*bo_ptr = NULL;

<<<<<<< HEAD
	/* maximun bo size is the minimun btw visible vram and gtt size */
	max_size = min(rdev->mc.visible_vram_size, rdev->mc.gtt_size);
	if ((page_align << PAGE_SHIFT) >= max_size) {
		printk(KERN_WARNING "%s:%d alloc size %ldM bigger than %ldMb limit\n",
			__func__, __LINE__, page_align  >> (20 - PAGE_SHIFT), max_size >> 20);
		return -ENOMEM;
	}

	acc_size = ttm_bo_dma_acc_size(&rdev->mman.bdev, size,
				       sizeof(struct radeon_bo));

retry:
	bo = kzalloc(sizeof(struct radeon_bo), GFP_KERNEL);
	if (bo == NULL)
		return -ENOMEM;
	r = drm_gem_object_init(rdev->ddev, &bo->gem_base, size);
	if (unlikely(r)) {
		kfree(bo);
		return r;
	}
	bo->rdev = rdev;
	bo->gem_base.driver_private = NULL;
	bo->surface_reg = -1;
	INIT_LIST_HEAD(&bo->list);
	INIT_LIST_HEAD(&bo->va);
	radeon_ttm_placement_from_domain(bo, domain);
	/* Kernel allocation are uninterruptible */
	mutex_lock(&rdev->vram_mutex);
	r = ttm_bo_init(&rdev->mman.bdev, &bo->tbo, size, type,
			&bo->placement, page_align, 0, !kernel, NULL,
			acc_size, &radeon_ttm_bo_destroy);
	mutex_unlock(&rdev->vram_mutex);
	if (unlikely(r != 0)) {
		if (r != -ERESTARTSYS) {
			if (domain == RADEON_GEM_DOMAIN_VRAM) {
				domain |= RADEON_GEM_DOMAIN_GTT;
				goto retry;
			}
			dev_err(rdev->dev,
				"object_init failed for (%lu, 0x%08X)\n",
				size, domain);
		}
=======
	bo = kzalloc(sizeof(struct radeon_bo), GFP_KERNEL);
	if (bo == NULL)
		return -ENOMEM;
	drm_gem_private_object_init(rdev->ddev, &bo->tbo.base, size);
	bo->rdev = rdev;
	bo->surface_reg = -1;
	INIT_LIST_HEAD(&bo->list);
	INIT_LIST_HEAD(&bo->va);
	bo->initial_domain = domain & (RADEON_GEM_DOMAIN_VRAM |
				       RADEON_GEM_DOMAIN_GTT |
				       RADEON_GEM_DOMAIN_CPU);

	bo->flags = flags;
	/* PCI GART is always snooped */
	if (!(rdev->flags & RADEON_IS_PCIE))
		bo->flags &= ~(RADEON_GEM_GTT_WC | RADEON_GEM_GTT_UC);

	/* Write-combined CPU mappings of GTT cause GPU hangs with RV6xx
	 * See https://bugs.freedesktop.org/show_bug.cgi?id=91268
	 */
	if (rdev->family >= CHIP_RV610 && rdev->family <= CHIP_RV635)
		bo->flags &= ~(RADEON_GEM_GTT_WC | RADEON_GEM_GTT_UC);

#ifdef CONFIG_X86_32
	/* XXX: Write-combined CPU mappings of GTT seem broken on 32-bit
	 * See https://bugs.freedesktop.org/show_bug.cgi?id=84627
	 */
	bo->flags &= ~(RADEON_GEM_GTT_WC | RADEON_GEM_GTT_UC);
#elif defined(CONFIG_X86) && !defined(CONFIG_X86_PAT)
	/* Don't try to enable write-combining when it can't work, or things
	 * may be slow
	 * See https://bugs.freedesktop.org/show_bug.cgi?id=88758
	 */
#ifndef CONFIG_COMPILE_TEST
#warning Please enable CONFIG_MTRR and CONFIG_X86_PAT for better performance \
	 thanks to write-combining
#endif

	if (bo->flags & RADEON_GEM_GTT_WC)
		DRM_INFO_ONCE("Please enable CONFIG_MTRR and CONFIG_X86_PAT for "
			      "better performance thanks to write-combining\n");
	bo->flags &= ~(RADEON_GEM_GTT_WC | RADEON_GEM_GTT_UC);
#else
	/* For architectures that don't support WC memory,
	 * mask out the WC flag from the BO
	 */
	if (!drm_arch_can_wc_memory())
		bo->flags &= ~RADEON_GEM_GTT_WC;
#endif

	radeon_ttm_placement_from_domain(bo, domain);
	/* Kernel allocation are uninterruptible */
	down_read(&rdev->pm.mclk_lock);
	r = ttm_bo_init_validate(&rdev->mman.bdev, &bo->tbo, type,
				 &bo->placement, page_align, !kernel, sg, resv,
				 &radeon_ttm_bo_destroy);
	up_read(&rdev->pm.mclk_lock);
	if (unlikely(r != 0)) {
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		return r;
	}
	*bo_ptr = bo;

	trace_radeon_bo_create(bo);

	return 0;
}

int radeon_bo_kmap(struct radeon_bo *bo, void **ptr)
{
	bool is_iomem;
<<<<<<< HEAD
	int r;
=======
	long r;

	r = dma_resv_wait_timeout(bo->tbo.base.resv, DMA_RESV_USAGE_KERNEL,
				  false, MAX_SCHEDULE_TIMEOUT);
	if (r < 0)
		return r;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	if (bo->kptr) {
		if (ptr) {
			*ptr = bo->kptr;
		}
		return 0;
	}
<<<<<<< HEAD
	r = ttm_bo_kmap(&bo->tbo, 0, bo->tbo.num_pages, &bo->kmap);
=======
	r = ttm_bo_kmap(&bo->tbo, 0, PFN_UP(bo->tbo.base.size), &bo->kmap);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	if (r) {
		return r;
	}
	bo->kptr = ttm_kmap_obj_virtual(&bo->kmap, &is_iomem);
	if (ptr) {
		*ptr = bo->kptr;
	}
	radeon_bo_check_tiling(bo, 0, 0);
	return 0;
}

void radeon_bo_kunmap(struct radeon_bo *bo)
{
	if (bo->kptr == NULL)
		return;
	bo->kptr = NULL;
	radeon_bo_check_tiling(bo, 0, 0);
	ttm_bo_kunmap(&bo->kmap);
}

<<<<<<< HEAD
void radeon_bo_unref(struct radeon_bo **bo)
{
	struct ttm_buffer_object *tbo;
	struct radeon_device *rdev;

	if ((*bo) == NULL)
		return;
	rdev = (*bo)->rdev;
	tbo = &((*bo)->tbo);
	mutex_lock(&rdev->vram_mutex);
	ttm_bo_unref(&tbo);
	mutex_unlock(&rdev->vram_mutex);
	if (tbo == NULL)
		*bo = NULL;
=======
struct radeon_bo *radeon_bo_ref(struct radeon_bo *bo)
{
	if (bo == NULL)
		return NULL;

	ttm_bo_get(&bo->tbo);
	return bo;
}

void radeon_bo_unref(struct radeon_bo **bo)
{
	struct ttm_buffer_object *tbo;

	if ((*bo) == NULL)
		return;
	tbo = &((*bo)->tbo);
	ttm_bo_put(tbo);
	*bo = NULL;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

int radeon_bo_pin_restricted(struct radeon_bo *bo, u32 domain, u64 max_offset,
			     u64 *gpu_addr)
{
<<<<<<< HEAD
	int r, i;

	if (bo->pin_count) {
		bo->pin_count++;
=======
	struct ttm_operation_ctx ctx = { false, false };
	int r, i;

	if (radeon_ttm_tt_has_userptr(bo->rdev, bo->tbo.ttm))
		return -EPERM;

	if (bo->tbo.pin_count) {
		ttm_bo_pin(&bo->tbo);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		if (gpu_addr)
			*gpu_addr = radeon_bo_gpu_offset(bo);

		if (max_offset != 0) {
			u64 domain_start;

			if (domain == RADEON_GEM_DOMAIN_VRAM)
				domain_start = bo->rdev->mc.vram_start;
			else
				domain_start = bo->rdev->mc.gtt_start;
			WARN_ON_ONCE(max_offset <
				     (radeon_bo_gpu_offset(bo) - domain_start));
		}

		return 0;
	}
<<<<<<< HEAD
	radeon_ttm_placement_from_domain(bo, domain);
	if (domain == RADEON_GEM_DOMAIN_VRAM) {
		/* force to pin into visible video ram */
		bo->placement.lpfn = bo->rdev->mc.visible_vram_size >> PAGE_SHIFT;
	}
	if (max_offset) {
		u64 lpfn = max_offset >> PAGE_SHIFT;

		if (!bo->placement.lpfn)
			bo->placement.lpfn = bo->rdev->mc.gtt_size >> PAGE_SHIFT;

		if (lpfn < bo->placement.lpfn)
			bo->placement.lpfn = lpfn;
	}
	for (i = 0; i < bo->placement.num_placement; i++)
		bo->placements[i] |= TTM_PL_FLAG_NO_EVICT;
	r = ttm_bo_validate(&bo->tbo, &bo->placement, false, false, false);
	if (likely(r == 0)) {
		bo->pin_count = 1;
		if (gpu_addr != NULL)
			*gpu_addr = radeon_bo_gpu_offset(bo);
	}
	if (unlikely(r != 0))
		dev_err(bo->rdev->dev, "%p pin failed\n", bo);
=======
	if (bo->prime_shared_count && domain == RADEON_GEM_DOMAIN_VRAM) {
		/* A BO shared as a dma-buf cannot be sensibly migrated to VRAM */
		return -EINVAL;
	}

	radeon_ttm_placement_from_domain(bo, domain);
	for (i = 0; i < bo->placement.num_placement; i++) {
		/* force to pin into visible video ram */
		if ((bo->placements[i].mem_type == TTM_PL_VRAM) &&
		    !(bo->flags & RADEON_GEM_NO_CPU_ACCESS) &&
		    (!max_offset || max_offset > bo->rdev->mc.visible_vram_size))
			bo->placements[i].lpfn =
				bo->rdev->mc.visible_vram_size >> PAGE_SHIFT;
		else
			bo->placements[i].lpfn = max_offset >> PAGE_SHIFT;
	}

	r = ttm_bo_validate(&bo->tbo, &bo->placement, &ctx);
	if (likely(r == 0)) {
		ttm_bo_pin(&bo->tbo);
		if (gpu_addr != NULL)
			*gpu_addr = radeon_bo_gpu_offset(bo);
		if (domain == RADEON_GEM_DOMAIN_VRAM)
			bo->rdev->vram_pin_size += radeon_bo_size(bo);
		else
			bo->rdev->gart_pin_size += radeon_bo_size(bo);
	} else {
		dev_err(bo->rdev->dev, "%p pin failed\n", bo);
	}
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	return r;
}

int radeon_bo_pin(struct radeon_bo *bo, u32 domain, u64 *gpu_addr)
{
	return radeon_bo_pin_restricted(bo, domain, 0, gpu_addr);
}

<<<<<<< HEAD
int radeon_bo_unpin(struct radeon_bo *bo)
{
	int r, i;

	if (!bo->pin_count) {
		dev_warn(bo->rdev->dev, "%p unpin not necessary\n", bo);
		return 0;
	}
	bo->pin_count--;
	if (bo->pin_count)
		return 0;
	for (i = 0; i < bo->placement.num_placement; i++)
		bo->placements[i] &= ~TTM_PL_FLAG_NO_EVICT;
	r = ttm_bo_validate(&bo->tbo, &bo->placement, false, false, false);
	if (unlikely(r != 0))
		dev_err(bo->rdev->dev, "%p validate failed for unpin\n", bo);
	return r;
=======
void radeon_bo_unpin(struct radeon_bo *bo)
{
	ttm_bo_unpin(&bo->tbo);
	if (!bo->tbo.pin_count) {
		if (bo->tbo.resource->mem_type == TTM_PL_VRAM)
			bo->rdev->vram_pin_size -= radeon_bo_size(bo);
		else
			bo->rdev->gart_pin_size -= radeon_bo_size(bo);
	}
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

int radeon_bo_evict_vram(struct radeon_device *rdev)
{
<<<<<<< HEAD
	/* late 2.6.33 fix IGP hibernate - we need pm ops to do this correct */
	if (0 && (rdev->flags & RADEON_IS_IGP)) {
=======
	struct ttm_device *bdev = &rdev->mman.bdev;
	struct ttm_resource_manager *man;

	/* late 2.6.33 fix IGP hibernate - we need pm ops to do this correct */
#ifndef CONFIG_HIBERNATION
	if (rdev->flags & RADEON_IS_IGP) {
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		if (rdev->mc.igp_sideport_enabled == false)
			/* Useless to evict on IGP chips */
			return 0;
	}
<<<<<<< HEAD
	return ttm_bo_evict_mm(&rdev->mman.bdev, TTM_PL_VRAM);
=======
#endif
	man = ttm_manager_type(bdev, TTM_PL_VRAM);
	if (!man)
		return 0;
	return ttm_resource_manager_evict_all(bdev, man);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

void radeon_bo_force_delete(struct radeon_device *rdev)
{
	struct radeon_bo *bo, *n;

	if (list_empty(&rdev->gem.objects)) {
		return;
	}
	dev_err(rdev->dev, "Userspace still has active objects !\n");
	list_for_each_entry_safe(bo, n, &rdev->gem.objects, list) {
<<<<<<< HEAD
		mutex_lock(&rdev->ddev->struct_mutex);
		dev_err(rdev->dev, "%p %p %lu %lu force free\n",
			&bo->gem_base, bo, (unsigned long)bo->gem_base.size,
			*((unsigned long *)&bo->gem_base.refcount));
=======
		dev_err(rdev->dev, "%p %p %lu %lu force free\n",
			&bo->tbo.base, bo, (unsigned long)bo->tbo.base.size,
			*((unsigned long *)&bo->tbo.base.refcount));
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		mutex_lock(&bo->rdev->gem.mutex);
		list_del_init(&bo->list);
		mutex_unlock(&bo->rdev->gem.mutex);
		/* this should unref the ttm bo */
<<<<<<< HEAD
		drm_gem_object_unreference(&bo->gem_base);
		mutex_unlock(&rdev->ddev->struct_mutex);
=======
		drm_gem_object_put(&bo->tbo.base);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	}
}

int radeon_bo_init(struct radeon_device *rdev)
{
<<<<<<< HEAD
	/* Add an MTRR for the VRAM */
	rdev->mc.vram_mtrr = mtrr_add(rdev->mc.aper_base, rdev->mc.aper_size,
			MTRR_TYPE_WRCOMB, 1);
=======
	/* reserve PAT memory space to WC for VRAM */
	arch_io_reserve_memtype_wc(rdev->mc.aper_base,
				   rdev->mc.aper_size);

	/* Add an MTRR for the VRAM */
	if (!rdev->fastfb_working) {
		rdev->mc.vram_mtrr = arch_phys_wc_add(rdev->mc.aper_base,
						      rdev->mc.aper_size);
	}
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	DRM_INFO("Detected VRAM RAM=%lluM, BAR=%lluM\n",
		rdev->mc.mc_vram_size >> 20,
		(unsigned long long)rdev->mc.aper_size >> 20);
	DRM_INFO("RAM width %dbits %cDR\n",
			rdev->mc.vram_width, rdev->mc.vram_is_ddr ? 'D' : 'S');
	return radeon_ttm_init(rdev);
}

void radeon_bo_fini(struct radeon_device *rdev)
{
	radeon_ttm_fini(rdev);
<<<<<<< HEAD
}

void radeon_bo_list_add_object(struct radeon_bo_list *lobj,
				struct list_head *head)
{
	if (lobj->wdomain) {
		list_add(&lobj->tv.head, head);
	} else {
		list_add_tail(&lobj->tv.head, head);
	}
}

int radeon_bo_list_validate(struct list_head *head)
{
	struct radeon_bo_list *lobj;
	struct radeon_bo *bo;
	u32 domain;
	int r;

	r = ttm_eu_reserve_buffers(head);
	if (unlikely(r != 0)) {
		return r;
	}
	list_for_each_entry(lobj, head, tv.head) {
		bo = lobj->bo;
		if (!bo->pin_count) {
			domain = lobj->wdomain ? lobj->wdomain : lobj->rdomain;
			
		retry:
			radeon_ttm_placement_from_domain(bo, domain);
			r = ttm_bo_validate(&bo->tbo, &bo->placement,
						true, false, false);
			if (unlikely(r)) {
				if (r != -ERESTARTSYS && domain == RADEON_GEM_DOMAIN_VRAM) {
					domain |= RADEON_GEM_DOMAIN_GTT;
					goto retry;
				}
=======
	arch_phys_wc_del(rdev->mc.vram_mtrr);
	arch_io_free_memtype_wc(rdev->mc.aper_base, rdev->mc.aper_size);
}

/* Returns how many bytes TTM can move per IB.
 */
static u64 radeon_bo_get_threshold_for_moves(struct radeon_device *rdev)
{
	u64 real_vram_size = rdev->mc.real_vram_size;
	struct ttm_resource_manager *man =
		ttm_manager_type(&rdev->mman.bdev, TTM_PL_VRAM);
	u64 vram_usage = ttm_resource_manager_usage(man);

	/* This function is based on the current VRAM usage.
	 *
	 * - If all of VRAM is free, allow relocating the number of bytes that
	 *   is equal to 1/4 of the size of VRAM for this IB.

	 * - If more than one half of VRAM is occupied, only allow relocating
	 *   1 MB of data for this IB.
	 *
	 * - From 0 to one half of used VRAM, the threshold decreases
	 *   linearly.
	 *         __________________
	 * 1/4 of -|\               |
	 * VRAM    | \              |
	 *         |  \             |
	 *         |   \            |
	 *         |    \           |
	 *         |     \          |
	 *         |      \         |
	 *         |       \________|1 MB
	 *         |----------------|
	 *    VRAM 0 %             100 %
	 *         used            used
	 *
	 * Note: It's a threshold, not a limit. The threshold must be crossed
	 * for buffer relocations to stop, so any buffer of an arbitrary size
	 * can be moved as long as the threshold isn't crossed before
	 * the relocation takes place. We don't want to disable buffer
	 * relocations completely.
	 *
	 * The idea is that buffers should be placed in VRAM at creation time
	 * and TTM should only do a minimum number of relocations during
	 * command submission. In practice, you need to submit at least
	 * a dozen IBs to move all buffers to VRAM if they are in GTT.
	 *
	 * Also, things can get pretty crazy under memory pressure and actual
	 * VRAM usage can change a lot, so playing safe even at 50% does
	 * consistently increase performance.
	 */

	u64 half_vram = real_vram_size >> 1;
	u64 half_free_vram = vram_usage >= half_vram ? 0 : half_vram - vram_usage;
	u64 bytes_moved_threshold = half_free_vram >> 1;
	return max(bytes_moved_threshold, 1024*1024ull);
}

int radeon_bo_list_validate(struct radeon_device *rdev,
			    struct ww_acquire_ctx *ticket,
			    struct list_head *head, int ring)
{
	struct ttm_operation_ctx ctx = { true, false };
	struct radeon_bo_list *lobj;
	struct list_head duplicates;
	int r;
	u64 bytes_moved = 0, initial_bytes_moved;
	u64 bytes_moved_threshold = radeon_bo_get_threshold_for_moves(rdev);

	INIT_LIST_HEAD(&duplicates);
	r = ttm_eu_reserve_buffers(ticket, head, true, &duplicates);
	if (unlikely(r != 0)) {
		return r;
	}

	list_for_each_entry(lobj, head, tv.head) {
		struct radeon_bo *bo = lobj->robj;
		if (!bo->tbo.pin_count) {
			u32 domain = lobj->preferred_domains;
			u32 allowed = lobj->allowed_domains;
			u32 current_domain =
				radeon_mem_type_to_domain(bo->tbo.resource->mem_type);

			/* Check if this buffer will be moved and don't move it
			 * if we have moved too many buffers for this IB already.
			 *
			 * Note that this allows moving at least one buffer of
			 * any size, because it doesn't take the current "bo"
			 * into account. We don't want to disallow buffer moves
			 * completely.
			 */
			if ((allowed & current_domain) != 0 &&
			    (domain & current_domain) == 0 && /* will be moved */
			    bytes_moved > bytes_moved_threshold) {
				/* don't move it */
				domain = current_domain;
			}

		retry:
			radeon_ttm_placement_from_domain(bo, domain);
			if (ring == R600_RING_TYPE_UVD_INDEX)
				radeon_uvd_force_into_uvd_segment(bo, allowed);

			initial_bytes_moved = atomic64_read(&rdev->num_bytes_moved);
			r = ttm_bo_validate(&bo->tbo, &bo->placement, &ctx);
			bytes_moved += atomic64_read(&rdev->num_bytes_moved) -
				       initial_bytes_moved;

			if (unlikely(r)) {
				if (r != -ERESTARTSYS &&
				    domain != lobj->allowed_domains) {
					domain = lobj->allowed_domains;
					goto retry;
				}
				ttm_eu_backoff_reservation(ticket, head);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
				return r;
			}
		}
		lobj->gpu_offset = radeon_bo_gpu_offset(bo);
		lobj->tiling_flags = bo->tiling_flags;
	}
<<<<<<< HEAD
	return 0;
}

int radeon_bo_fbdev_mmap(struct radeon_bo *bo,
			     struct vm_area_struct *vma)
{
	return ttm_fbdev_mmap(vma, &bo->tbo);
=======

	list_for_each_entry(lobj, &duplicates, tv.head) {
		lobj->gpu_offset = radeon_bo_gpu_offset(lobj->robj);
		lobj->tiling_flags = lobj->robj->tiling_flags;
	}

	return 0;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

int radeon_bo_get_surface_reg(struct radeon_bo *bo)
{
	struct radeon_device *rdev = bo->rdev;
	struct radeon_surface_reg *reg;
	struct radeon_bo *old_object;
	int steal;
	int i;

<<<<<<< HEAD
	BUG_ON(!atomic_read(&bo->tbo.reserved));
=======
	dma_resv_assert_held(bo->tbo.base.resv);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	if (!bo->tiling_flags)
		return 0;

	if (bo->surface_reg >= 0) {
<<<<<<< HEAD
		reg = &rdev->surface_regs[bo->surface_reg];
=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		i = bo->surface_reg;
		goto out;
	}

	steal = -1;
	for (i = 0; i < RADEON_GEM_MAX_SURFACES; i++) {

		reg = &rdev->surface_regs[i];
		if (!reg->bo)
			break;

		old_object = reg->bo;
<<<<<<< HEAD
		if (old_object->pin_count == 0)
=======
		if (old_object->tbo.pin_count == 0)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
			steal = i;
	}

	/* if we are all out */
	if (i == RADEON_GEM_MAX_SURFACES) {
		if (steal == -1)
			return -ENOMEM;
		/* find someone with a surface reg and nuke their BO */
		reg = &rdev->surface_regs[steal];
		old_object = reg->bo;
		/* blow away the mapping */
		DRM_DEBUG("stealing surface reg %d from %p\n", steal, old_object);
		ttm_bo_unmap_virtual(&old_object->tbo);
		old_object->surface_reg = -1;
		i = steal;
	}

	bo->surface_reg = i;
	reg->bo = bo;

out:
	radeon_set_surface_reg(rdev, i, bo->tiling_flags, bo->pitch,
<<<<<<< HEAD
			       bo->tbo.mem.start << PAGE_SHIFT,
			       bo->tbo.num_pages << PAGE_SHIFT);
=======
			       bo->tbo.resource->start << PAGE_SHIFT,
			       bo->tbo.base.size);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	return 0;
}

static void radeon_bo_clear_surface_reg(struct radeon_bo *bo)
{
	struct radeon_device *rdev = bo->rdev;
	struct radeon_surface_reg *reg;

	if (bo->surface_reg == -1)
		return;

	reg = &rdev->surface_regs[bo->surface_reg];
	radeon_clear_surface_reg(rdev, bo->surface_reg);

	reg->bo = NULL;
	bo->surface_reg = -1;
}

int radeon_bo_set_tiling_flags(struct radeon_bo *bo,
				uint32_t tiling_flags, uint32_t pitch)
{
	struct radeon_device *rdev = bo->rdev;
	int r;

	if (rdev->family >= CHIP_CEDAR) {
		unsigned bankw, bankh, mtaspect, tilesplit, stilesplit;

		bankw = (tiling_flags >> RADEON_TILING_EG_BANKW_SHIFT) & RADEON_TILING_EG_BANKW_MASK;
		bankh = (tiling_flags >> RADEON_TILING_EG_BANKH_SHIFT) & RADEON_TILING_EG_BANKH_MASK;
		mtaspect = (tiling_flags >> RADEON_TILING_EG_MACRO_TILE_ASPECT_SHIFT) & RADEON_TILING_EG_MACRO_TILE_ASPECT_MASK;
		tilesplit = (tiling_flags >> RADEON_TILING_EG_TILE_SPLIT_SHIFT) & RADEON_TILING_EG_TILE_SPLIT_MASK;
		stilesplit = (tiling_flags >> RADEON_TILING_EG_STENCIL_TILE_SPLIT_SHIFT) & RADEON_TILING_EG_STENCIL_TILE_SPLIT_MASK;
		switch (bankw) {
		case 0:
		case 1:
		case 2:
		case 4:
		case 8:
			break;
		default:
			return -EINVAL;
		}
		switch (bankh) {
		case 0:
		case 1:
		case 2:
		case 4:
		case 8:
			break;
		default:
			return -EINVAL;
		}
		switch (mtaspect) {
		case 0:
		case 1:
		case 2:
		case 4:
		case 8:
			break;
		default:
			return -EINVAL;
		}
		if (tilesplit > 6) {
			return -EINVAL;
		}
		if (stilesplit > 6) {
			return -EINVAL;
		}
	}
	r = radeon_bo_reserve(bo, false);
	if (unlikely(r != 0))
		return r;
	bo->tiling_flags = tiling_flags;
	bo->pitch = pitch;
	radeon_bo_unreserve(bo);
	return 0;
}

void radeon_bo_get_tiling_flags(struct radeon_bo *bo,
				uint32_t *tiling_flags,
				uint32_t *pitch)
{
<<<<<<< HEAD
	BUG_ON(!atomic_read(&bo->tbo.reserved));
=======
	dma_resv_assert_held(bo->tbo.base.resv);

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	if (tiling_flags)
		*tiling_flags = bo->tiling_flags;
	if (pitch)
		*pitch = bo->pitch;
}

int radeon_bo_check_tiling(struct radeon_bo *bo, bool has_moved,
				bool force_drop)
{
<<<<<<< HEAD
	BUG_ON(!atomic_read(&bo->tbo.reserved));
=======
	if (!force_drop)
		dma_resv_assert_held(bo->tbo.base.resv);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	if (!(bo->tiling_flags & RADEON_TILING_SURFACE))
		return 0;

	if (force_drop) {
		radeon_bo_clear_surface_reg(bo);
		return 0;
	}

<<<<<<< HEAD
	if (bo->tbo.mem.mem_type != TTM_PL_VRAM) {
=======
	if (bo->tbo.resource->mem_type != TTM_PL_VRAM) {
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		if (!has_moved)
			return 0;

		if (bo->surface_reg >= 0)
			radeon_bo_clear_surface_reg(bo);
		return 0;
	}

	if ((bo->surface_reg >= 0) && !has_moved)
		return 0;

	return radeon_bo_get_surface_reg(bo);
}

<<<<<<< HEAD
void radeon_bo_move_notify(struct ttm_buffer_object *bo,
			   struct ttm_mem_reg *mem)
{
	struct radeon_bo *rbo;
	if (!radeon_ttm_bo_is_radeon_bo(bo))
		return;
=======
void radeon_bo_move_notify(struct ttm_buffer_object *bo)
{
	struct radeon_bo *rbo;

	if (!radeon_ttm_bo_is_radeon_bo(bo))
		return;

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	rbo = container_of(bo, struct radeon_bo, tbo);
	radeon_bo_check_tiling(rbo, 0, 1);
	radeon_vm_bo_invalidate(rbo->rdev, rbo);
}

<<<<<<< HEAD
int radeon_bo_fault_reserve_notify(struct ttm_buffer_object *bo)
{
	struct radeon_device *rdev;
	struct radeon_bo *rbo;
	unsigned long offset, size;
	int r;
=======
vm_fault_t radeon_bo_fault_reserve_notify(struct ttm_buffer_object *bo)
{
	struct ttm_operation_ctx ctx = { false, false };
	struct radeon_device *rdev;
	struct radeon_bo *rbo;
	unsigned long offset, size, lpfn;
	int i, r;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	if (!radeon_ttm_bo_is_radeon_bo(bo))
		return 0;
	rbo = container_of(bo, struct radeon_bo, tbo);
	radeon_bo_check_tiling(rbo, 0, 0);
	rdev = rbo->rdev;
<<<<<<< HEAD
	if (bo->mem.mem_type == TTM_PL_VRAM) {
		size = bo->mem.num_pages << PAGE_SHIFT;
		offset = bo->mem.start << PAGE_SHIFT;
		if ((offset + size) > rdev->mc.visible_vram_size) {
			/* hurrah the memory is not visible ! */
			radeon_ttm_placement_from_domain(rbo, RADEON_GEM_DOMAIN_VRAM);
			rbo->placement.lpfn = rdev->mc.visible_vram_size >> PAGE_SHIFT;
			r = ttm_bo_validate(bo, &rbo->placement, false, true, false);
			if (unlikely(r != 0))
				return r;
			offset = bo->mem.start << PAGE_SHIFT;
			/* this should not happen */
			if ((offset + size) > rdev->mc.visible_vram_size)
				return -EINVAL;
		}
	}
	return 0;
}

int radeon_bo_wait(struct radeon_bo *bo, u32 *mem_type, bool no_wait)
{
	int r;

	r = ttm_bo_reserve(&bo->tbo, true, no_wait, false, 0);
	if (unlikely(r != 0))
		return r;
	spin_lock(&bo->tbo.bdev->fence_lock);
	if (mem_type)
		*mem_type = bo->tbo.mem.mem_type;
	if (bo->tbo.sync_obj)
		r = ttm_bo_wait(&bo->tbo, true, true, no_wait);
	spin_unlock(&bo->tbo.bdev->fence_lock);
	ttm_bo_unreserve(&bo->tbo);
	return r;
}


/**
 * radeon_bo_reserve - reserve bo
 * @bo:		bo structure
 * @no_wait:		don't sleep while trying to reserve (return -EBUSY)
 *
 * Returns:
 * -EBUSY: buffer is busy and @no_wait is true
 * -ERESTARTSYS: A wait for the buffer to become unreserved was interrupted by
 * a signal. Release all buffer reservations and return to user-space.
 */
int radeon_bo_reserve(struct radeon_bo *bo, bool no_wait)
{
	int r;

	r = ttm_bo_reserve(&bo->tbo, true, no_wait, false, 0);
	if (unlikely(r != 0)) {
		if (r != -ERESTARTSYS)
			dev_err(bo->rdev->dev, "%p reserve failed\n", bo);
		return r;
	}
	return 0;
}

/* object have to be reserved */
struct radeon_bo_va *radeon_bo_va(struct radeon_bo *rbo, struct radeon_vm *vm)
{
	struct radeon_bo_va *bo_va;

	list_for_each_entry(bo_va, &rbo->va, bo_list) {
		if (bo_va->vm == vm) {
			return bo_va;
		}
	}
	return NULL;
=======
	if (bo->resource->mem_type != TTM_PL_VRAM)
		return 0;

	size = bo->resource->size;
	offset = bo->resource->start << PAGE_SHIFT;
	if ((offset + size) <= rdev->mc.visible_vram_size)
		return 0;

	/* Can't move a pinned BO to visible VRAM */
	if (rbo->tbo.pin_count > 0)
		return VM_FAULT_SIGBUS;

	/* hurrah the memory is not visible ! */
	radeon_ttm_placement_from_domain(rbo, RADEON_GEM_DOMAIN_VRAM);
	lpfn =	rdev->mc.visible_vram_size >> PAGE_SHIFT;
	for (i = 0; i < rbo->placement.num_placement; i++) {
		/* Force into visible VRAM */
		if ((rbo->placements[i].mem_type == TTM_PL_VRAM) &&
		    (!rbo->placements[i].lpfn || rbo->placements[i].lpfn > lpfn))
			rbo->placements[i].lpfn = lpfn;
	}
	r = ttm_bo_validate(bo, &rbo->placement, &ctx);
	if (unlikely(r == -ENOMEM)) {
		radeon_ttm_placement_from_domain(rbo, RADEON_GEM_DOMAIN_GTT);
		r = ttm_bo_validate(bo, &rbo->placement, &ctx);
	} else if (likely(!r)) {
		offset = bo->resource->start << PAGE_SHIFT;
		/* this should never happen */
		if ((offset + size) > rdev->mc.visible_vram_size)
			return VM_FAULT_SIGBUS;
	}

	if (unlikely(r == -EBUSY || r == -ERESTARTSYS))
		return VM_FAULT_NOPAGE;
	else if (unlikely(r))
		return VM_FAULT_SIGBUS;

	ttm_bo_move_to_lru_tail_unlocked(bo);
	return 0;
}

/**
 * radeon_bo_fence - add fence to buffer object
 *
 * @bo: buffer object in question
 * @fence: fence to add
 * @shared: true if fence should be added shared
 *
 */
void radeon_bo_fence(struct radeon_bo *bo, struct radeon_fence *fence,
		     bool shared)
{
	struct dma_resv *resv = bo->tbo.base.resv;
	int r;

	r = dma_resv_reserve_fences(resv, 1);
	if (r) {
		/* As last resort on OOM we block for the fence */
		dma_fence_wait(&fence->base, false);
		return;
	}

	dma_resv_add_fence(resv, &fence->base, shared ?
			   DMA_RESV_USAGE_READ : DMA_RESV_USAGE_WRITE);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}
