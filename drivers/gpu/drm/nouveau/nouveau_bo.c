/*
 * Copyright 2007 Dave Airlied
 * All Rights Reserved.
 *
 * Permission is hereby granted, free of charge, to any person obtaining a
 * copy of this software and associated documentation files (the "Software"),
 * to deal in the Software without restriction, including without limitation
 * the rights to use, copy, modify, merge, publish, distribute, sublicense,
 * and/or sell copies of the Software, and to permit persons to whom the
 * Software is furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice (including the next
 * paragraph) shall be included in all copies or substantial portions of the
 * Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.  IN NO EVENT SHALL
 * VA LINUX SYSTEMS AND/OR ITS SUPPLIERS BE LIABLE FOR ANY CLAIM, DAMAGES OR
 * OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE,
 * ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
 * OTHER DEALINGS IN THE SOFTWARE.
 */
/*
 * Authors: Dave Airlied <airlied@linux.ie>
 *	    Ben Skeggs   <darktama@iinet.net.au>
 *	    Jeremy Kolb  <jkolb@brandeis.edu>
 */

<<<<<<< HEAD
#include "drmP.h"
#include "ttm/ttm_page_alloc.h"

#include "nouveau_drm.h"
#include "nouveau_drv.h"
#include "nouveau_dma.h"
#include "nouveau_mm.h"
#include "nouveau_vm.h"

#include <linux/log2.h>
#include <linux/slab.h>
=======
#include <linux/dma-mapping.h>
#include <drm/ttm/ttm_tt.h>

#include "nouveau_drv.h"
#include "nouveau_chan.h"
#include "nouveau_fence.h"

#include "nouveau_bo.h"
#include "nouveau_ttm.h"
#include "nouveau_gem.h"
#include "nouveau_mem.h"
#include "nouveau_vmm.h"

#include <nvif/class.h>
#include <nvif/if500b.h>
#include <nvif/if900b.h>

static int nouveau_ttm_tt_bind(struct ttm_device *bdev, struct ttm_tt *ttm,
			       struct ttm_resource *reg);
static void nouveau_ttm_tt_unbind(struct ttm_device *bdev, struct ttm_tt *ttm);

/*
 * NV10-NV40 tiling helpers
 */

static void
nv10_bo_update_tile_region(struct drm_device *dev, struct nouveau_drm_tile *reg,
			   u32 addr, u32 size, u32 pitch, u32 flags)
{
	struct nouveau_drm *drm = nouveau_drm(dev);
	int i = reg - drm->tile.reg;
	struct nvkm_fb *fb = nvxx_fb(&drm->client.device);
	struct nvkm_fb_tile *tile = &fb->tile.region[i];

	nouveau_fence_unref(&reg->fence);

	if (tile->pitch)
		nvkm_fb_tile_fini(fb, i, tile);

	if (pitch)
		nvkm_fb_tile_init(fb, i, addr, size, pitch, flags, tile);

	nvkm_fb_tile_prog(fb, i, tile);
}

static struct nouveau_drm_tile *
nv10_bo_get_tile_region(struct drm_device *dev, int i)
{
	struct nouveau_drm *drm = nouveau_drm(dev);
	struct nouveau_drm_tile *tile = &drm->tile.reg[i];

	spin_lock(&drm->tile.lock);

	if (!tile->used &&
	    (!tile->fence || nouveau_fence_done(tile->fence)))
		tile->used = true;
	else
		tile = NULL;

	spin_unlock(&drm->tile.lock);
	return tile;
}

static void
nv10_bo_put_tile_region(struct drm_device *dev, struct nouveau_drm_tile *tile,
			struct dma_fence *fence)
{
	struct nouveau_drm *drm = nouveau_drm(dev);

	if (tile) {
		spin_lock(&drm->tile.lock);
		tile->fence = (struct nouveau_fence *)dma_fence_get(fence);
		tile->used = false;
		spin_unlock(&drm->tile.lock);
	}
}

static struct nouveau_drm_tile *
nv10_bo_set_tiling(struct drm_device *dev, u32 addr,
		   u32 size, u32 pitch, u32 zeta)
{
	struct nouveau_drm *drm = nouveau_drm(dev);
	struct nvkm_fb *fb = nvxx_fb(&drm->client.device);
	struct nouveau_drm_tile *tile, *found = NULL;
	int i;

	for (i = 0; i < fb->tile.regions; i++) {
		tile = nv10_bo_get_tile_region(dev, i);

		if (pitch && !found) {
			found = tile;
			continue;

		} else if (tile && fb->tile.region[i].pitch) {
			/* Kill an unused tile region. */
			nv10_bo_update_tile_region(dev, tile, 0, 0, 0, 0);
		}

		nv10_bo_put_tile_region(dev, tile, NULL);
	}

	if (found)
		nv10_bo_update_tile_region(dev, found, addr, size, pitch, zeta);
	return found;
}
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

static void
nouveau_bo_del_ttm(struct ttm_buffer_object *bo)
{
<<<<<<< HEAD
	struct drm_nouveau_private *dev_priv = nouveau_bdev(bo->bdev);
	struct drm_device *dev = dev_priv->dev;
	struct nouveau_bo *nvbo = nouveau_bo(bo);

	if (unlikely(nvbo->gem))
		DRM_ERROR("bo %p still attached to GEM object\n", bo);

	nv10_mem_put_tile_region(dev, nvbo->tile, NULL);
	kfree(nvbo);
}

static void
nouveau_bo_fixup_align(struct nouveau_bo *nvbo, u32 flags,
		       int *align, int *size)
{
	struct drm_nouveau_private *dev_priv = nouveau_bdev(nvbo->bo.bdev);

	if (dev_priv->card_type < NV_50) {
		if (nvbo->tile_mode) {
			if (dev_priv->chipset >= 0x40) {
				*align = 65536;
				*size = roundup(*size, 64 * nvbo->tile_mode);

			} else if (dev_priv->chipset >= 0x30) {
				*align = 32768;
				*size = roundup(*size, 64 * nvbo->tile_mode);

			} else if (dev_priv->chipset >= 0x20) {
				*align = 16384;
				*size = roundup(*size, 64 * nvbo->tile_mode);

			} else if (dev_priv->chipset >= 0x10) {
				*align = 16384;
				*size = roundup(*size, 32 * nvbo->tile_mode);
			}
		}
	} else {
		*size = roundup(*size, (1 << nvbo->page_shift));
		*align = max((1 <<  nvbo->page_shift), *align);
	}

	*size = roundup(*size, PAGE_SIZE);
}

int
nouveau_bo_new(struct drm_device *dev, int size, int align,
	       uint32_t flags, uint32_t tile_mode, uint32_t tile_flags,
	       struct nouveau_bo **pnvbo)
{
	struct drm_nouveau_private *dev_priv = dev->dev_private;
	struct nouveau_bo *nvbo;
	size_t acc_size;
	int ret;

	nvbo = kzalloc(sizeof(struct nouveau_bo), GFP_KERNEL);
	if (!nvbo)
		return -ENOMEM;
	INIT_LIST_HEAD(&nvbo->head);
	INIT_LIST_HEAD(&nvbo->entry);
	INIT_LIST_HEAD(&nvbo->vma_list);
	nvbo->tile_mode = tile_mode;
	nvbo->tile_flags = tile_flags;
	nvbo->bo.bdev = &dev_priv->ttm.bdev;

	nvbo->page_shift = 12;
	if (dev_priv->bar1_vm) {
		if (!(flags & TTM_PL_FLAG_TT) && size > 256 * 1024)
			nvbo->page_shift = dev_priv->bar1_vm->lpg_shift;
	}

	nouveau_bo_fixup_align(nvbo, flags, &align, &size);
	nvbo->bo.mem.num_pages = size >> PAGE_SHIFT;
	nouveau_bo_placement_set(nvbo, flags, 0);

	acc_size = ttm_bo_dma_acc_size(&dev_priv->ttm.bdev, size,
				       sizeof(struct nouveau_bo));

	ret = ttm_bo_init(&dev_priv->ttm.bdev, &nvbo->bo, size,
			  ttm_bo_type_device, &nvbo->placement,
			  align >> PAGE_SHIFT, 0, false, NULL, acc_size,
			  nouveau_bo_del_ttm);
=======
	struct nouveau_drm *drm = nouveau_bdev(bo->bdev);
	struct drm_device *dev = drm->dev;
	struct nouveau_bo *nvbo = nouveau_bo(bo);

	WARN_ON(nvbo->bo.pin_count > 0);
	nouveau_bo_del_io_reserve_lru(bo);
	nv10_bo_put_tile_region(dev, nvbo->tile, NULL);

	/*
	 * If nouveau_bo_new() allocated this buffer, the GEM object was never
	 * initialized, so don't attempt to release it.
	 */
	if (bo->base.dev) {
		/* Gem objects not being shared with other VMs get their
		 * dma_resv from a root GEM object.
		 */
		if (nvbo->no_share)
			drm_gem_object_put(nvbo->r_obj);

		drm_gem_object_release(&bo->base);
	} else {
		dma_resv_fini(&bo->base._resv);
	}

	kfree(nvbo);
}

static inline u64
roundup_64(u64 x, u32 y)
{
	x += y - 1;
	do_div(x, y);
	return x * y;
}

static void
nouveau_bo_fixup_align(struct nouveau_bo *nvbo, int *align, u64 *size)
{
	struct nouveau_drm *drm = nouveau_bdev(nvbo->bo.bdev);
	struct nvif_device *device = &drm->client.device;

	if (device->info.family < NV_DEVICE_INFO_V0_TESLA) {
		if (nvbo->mode) {
			if (device->info.chipset >= 0x40) {
				*align = 65536;
				*size = roundup_64(*size, 64 * nvbo->mode);

			} else if (device->info.chipset >= 0x30) {
				*align = 32768;
				*size = roundup_64(*size, 64 * nvbo->mode);

			} else if (device->info.chipset >= 0x20) {
				*align = 16384;
				*size = roundup_64(*size, 64 * nvbo->mode);

			} else if (device->info.chipset >= 0x10) {
				*align = 16384;
				*size = roundup_64(*size, 32 * nvbo->mode);
			}
		}
	} else {
		*size = roundup_64(*size, (1 << nvbo->page));
		*align = max((1 <<  nvbo->page), *align);
	}

	*size = roundup_64(*size, PAGE_SIZE);
}

struct nouveau_bo *
nouveau_bo_alloc(struct nouveau_cli *cli, u64 *size, int *align, u32 domain,
		 u32 tile_mode, u32 tile_flags, bool internal)
{
	struct nouveau_drm *drm = cli->drm;
	struct nouveau_bo *nvbo;
	struct nvif_mmu *mmu = &cli->mmu;
	struct nvif_vmm *vmm = &nouveau_cli_vmm(cli)->vmm;
	int i, pi = -1;

	if (!*size) {
		NV_WARN(drm, "skipped size %016llx\n", *size);
		return ERR_PTR(-EINVAL);
	}

	nvbo = kzalloc(sizeof(struct nouveau_bo), GFP_KERNEL);
	if (!nvbo)
		return ERR_PTR(-ENOMEM);

	INIT_LIST_HEAD(&nvbo->head);
	INIT_LIST_HEAD(&nvbo->entry);
	INIT_LIST_HEAD(&nvbo->vma_list);
	nvbo->bo.bdev = &drm->ttm.bdev;

	/* This is confusing, and doesn't actually mean we want an uncached
	 * mapping, but is what NOUVEAU_GEM_DOMAIN_COHERENT gets translated
	 * into in nouveau_gem_new().
	 */
	if (domain & NOUVEAU_GEM_DOMAIN_COHERENT) {
		/* Determine if we can get a cache-coherent map, forcing
		 * uncached mapping if we can't.
		 */
		if (!nouveau_drm_use_coherent_gpu_mapping(drm))
			nvbo->force_coherent = true;
	}

	nvbo->contig = !(tile_flags & NOUVEAU_GEM_TILE_NONCONTIG);
	if (!nouveau_cli_uvmm(cli) || internal) {
		/* for BO noVM allocs, don't assign kinds */
		if (cli->device.info.family >= NV_DEVICE_INFO_V0_FERMI) {
			nvbo->kind = (tile_flags & 0x0000ff00) >> 8;
			if (!nvif_mmu_kind_valid(mmu, nvbo->kind)) {
				kfree(nvbo);
				return ERR_PTR(-EINVAL);
			}

			nvbo->comp = mmu->kind[nvbo->kind] != nvbo->kind;
		} else if (cli->device.info.family >= NV_DEVICE_INFO_V0_TESLA) {
			nvbo->kind = (tile_flags & 0x00007f00) >> 8;
			nvbo->comp = (tile_flags & 0x00030000) >> 16;
			if (!nvif_mmu_kind_valid(mmu, nvbo->kind)) {
				kfree(nvbo);
				return ERR_PTR(-EINVAL);
			}
		} else {
			nvbo->zeta = (tile_flags & 0x00000007);
		}
		nvbo->mode = tile_mode;

		/* Determine the desirable target GPU page size for the buffer. */
		for (i = 0; i < vmm->page_nr; i++) {
			/* Because we cannot currently allow VMM maps to fail
			 * during buffer migration, we need to determine page
			 * size for the buffer up-front, and pre-allocate its
			 * page tables.
			 *
			 * Skip page sizes that can't support needed domains.
			 */
			if (cli->device.info.family > NV_DEVICE_INFO_V0_CURIE &&
			    (domain & NOUVEAU_GEM_DOMAIN_VRAM) && !vmm->page[i].vram)
				continue;
			if ((domain & NOUVEAU_GEM_DOMAIN_GART) &&
			    (!vmm->page[i].host || vmm->page[i].shift > PAGE_SHIFT))
				continue;

			/* Select this page size if it's the first that supports
			 * the potential memory domains, or when it's compatible
			 * with the requested compression settings.
			 */
			if (pi < 0 || !nvbo->comp || vmm->page[i].comp)
				pi = i;

			/* Stop once the buffer is larger than the current page size. */
			if (*size >= 1ULL << vmm->page[i].shift)
				break;
		}

		if (WARN_ON(pi < 0)) {
			kfree(nvbo);
			return ERR_PTR(-EINVAL);
		}

		/* Disable compression if suitable settings couldn't be found. */
		if (nvbo->comp && !vmm->page[pi].comp) {
			if (mmu->object.oclass >= NVIF_CLASS_MMU_GF100)
				nvbo->kind = mmu->kind[nvbo->kind];
			nvbo->comp = 0;
		}
		nvbo->page = vmm->page[pi].shift;
	} else {
		/* reject other tile flags when in VM mode. */
		if (tile_mode)
			return ERR_PTR(-EINVAL);
		if (tile_flags & ~NOUVEAU_GEM_TILE_NONCONTIG)
			return ERR_PTR(-EINVAL);

		/* Determine the desirable target GPU page size for the buffer. */
		for (i = 0; i < vmm->page_nr; i++) {
			/* Because we cannot currently allow VMM maps to fail
			 * during buffer migration, we need to determine page
			 * size for the buffer up-front, and pre-allocate its
			 * page tables.
			 *
			 * Skip page sizes that can't support needed domains.
			 */
			if ((domain & NOUVEAU_GEM_DOMAIN_VRAM) && !vmm->page[i].vram)
				continue;
			if ((domain & NOUVEAU_GEM_DOMAIN_GART) &&
			    (!vmm->page[i].host || vmm->page[i].shift > PAGE_SHIFT))
				continue;

			/* pick the last one as it will be smallest. */
			pi = i;

			/* Stop once the buffer is larger than the current page size. */
			if (*size >= 1ULL << vmm->page[i].shift)
				break;
		}
		if (WARN_ON(pi < 0)) {
			kfree(nvbo);
			return ERR_PTR(-EINVAL);
		}
		nvbo->page = vmm->page[pi].shift;
	}

	nouveau_bo_fixup_align(nvbo, align, size);

	return nvbo;
}

int
nouveau_bo_init(struct nouveau_bo *nvbo, u64 size, int align, u32 domain,
		struct sg_table *sg, struct dma_resv *robj)
{
	int type = sg ? ttm_bo_type_sg : ttm_bo_type_device;
	int ret;
	struct ttm_operation_ctx ctx = {
		.interruptible = false,
		.no_wait_gpu = false,
		.resv = robj,
	};

	nouveau_bo_placement_set(nvbo, domain, 0);
	INIT_LIST_HEAD(&nvbo->io_reserve_lru);

	ret = ttm_bo_init_reserved(nvbo->bo.bdev, &nvbo->bo, type,
				   &nvbo->placement, align >> PAGE_SHIFT, &ctx,
				   sg, robj, nouveau_bo_del_ttm);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	if (ret) {
		/* ttm will call nouveau_bo_del_ttm if it fails.. */
		return ret;
	}

<<<<<<< HEAD
=======
	if (!robj)
		ttm_bo_unreserve(&nvbo->bo);

	return 0;
}

int
nouveau_bo_new(struct nouveau_cli *cli, u64 size, int align,
	       uint32_t domain, uint32_t tile_mode, uint32_t tile_flags,
	       struct sg_table *sg, struct dma_resv *robj,
	       struct nouveau_bo **pnvbo)
{
	struct nouveau_bo *nvbo;
	int ret;

	nvbo = nouveau_bo_alloc(cli, &size, &align, domain, tile_mode,
				tile_flags, true);
	if (IS_ERR(nvbo))
		return PTR_ERR(nvbo);

	nvbo->bo.base.size = size;
	dma_resv_init(&nvbo->bo.base._resv);
	drm_vma_node_reset(&nvbo->bo.base.vma_node);

	/* This must be called before ttm_bo_init_reserved(). Subsequent
	 * bo_move() callbacks might already iterate the GEMs GPUVA list.
	 */
	drm_gem_gpuva_init(&nvbo->bo.base);

	ret = nouveau_bo_init(nvbo, size, align, domain, sg, robj);
	if (ret)
		return ret;

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	*pnvbo = nvbo;
	return 0;
}

static void
<<<<<<< HEAD
set_placement_list(uint32_t *pl, unsigned *n, uint32_t type, uint32_t flags)
{
	*n = 0;

	if (type & TTM_PL_FLAG_VRAM)
		pl[(*n)++] = TTM_PL_FLAG_VRAM | flags;
	if (type & TTM_PL_FLAG_TT)
		pl[(*n)++] = TTM_PL_FLAG_TT | flags;
	if (type & TTM_PL_FLAG_SYSTEM)
		pl[(*n)++] = TTM_PL_FLAG_SYSTEM | flags;
}

static void
set_placement_range(struct nouveau_bo *nvbo, uint32_t type)
{
	struct drm_nouveau_private *dev_priv = nouveau_bdev(nvbo->bo.bdev);
	int vram_pages = dev_priv->vram_size >> PAGE_SHIFT;

	if (dev_priv->card_type == NV_10 &&
	    nvbo->tile_mode && (type & TTM_PL_FLAG_VRAM) &&
	    nvbo->bo.mem.num_pages < vram_pages / 4) {
=======
set_placement_range(struct nouveau_bo *nvbo, uint32_t domain)
{
	struct nouveau_drm *drm = nouveau_bdev(nvbo->bo.bdev);
	u64 vram_size = drm->client.device.info.ram_size;
	unsigned i, fpfn, lpfn;

	if (drm->client.device.info.family == NV_DEVICE_INFO_V0_CELSIUS &&
	    nvbo->mode && (domain & NOUVEAU_GEM_DOMAIN_VRAM) &&
	    nvbo->bo.base.size < vram_size / 4) {
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		/*
		 * Make sure that the color and depth buffers are handled
		 * by independent memory controller units. Up to a 9x
		 * speed up when alpha-blending and depth-test are enabled
		 * at the same time.
		 */
<<<<<<< HEAD
		if (nvbo->tile_flags & NOUVEAU_GEM_TILE_ZETA) {
			nvbo->placement.fpfn = vram_pages / 2;
			nvbo->placement.lpfn = ~0;
		} else {
			nvbo->placement.fpfn = 0;
			nvbo->placement.lpfn = vram_pages / 2;
=======
		if (nvbo->zeta) {
			fpfn = (vram_size / 2) >> PAGE_SHIFT;
			lpfn = ~0;
		} else {
			fpfn = 0;
			lpfn = (vram_size / 2) >> PAGE_SHIFT;
		}
		for (i = 0; i < nvbo->placement.num_placement; ++i) {
			nvbo->placements[i].fpfn = fpfn;
			nvbo->placements[i].lpfn = lpfn;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		}
	}
}

void
<<<<<<< HEAD
nouveau_bo_placement_set(struct nouveau_bo *nvbo, uint32_t type, uint32_t busy)
{
	struct ttm_placement *pl = &nvbo->placement;
	uint32_t flags = TTM_PL_MASK_CACHING |
		(nvbo->pin_refcnt ? TTM_PL_FLAG_NO_EVICT : 0);

	pl->placement = nvbo->placements;
	set_placement_list(nvbo->placements, &pl->num_placement,
			   type, flags);

	pl->busy_placement = nvbo->busy_placements;
	set_placement_list(nvbo->busy_placements, &pl->num_busy_placement,
			   type | busy, flags);

	set_placement_range(nvbo, type);
}

int
nouveau_bo_pin(struct nouveau_bo *nvbo, uint32_t memtype)
{
	struct drm_nouveau_private *dev_priv = nouveau_bdev(nvbo->bo.bdev);
	struct ttm_buffer_object *bo = &nvbo->bo;
	int ret;

	if (nvbo->pin_refcnt && !(memtype & (1 << bo->mem.mem_type))) {
		NV_ERROR(nouveau_bdev(bo->bdev)->dev,
			 "bo %p pinned elsewhere: 0x%08x vs 0x%08x\n", bo,
			 1 << bo->mem.mem_type, memtype);
		return -EINVAL;
	}

	if (nvbo->pin_refcnt++)
		return 0;

	ret = ttm_bo_reserve(bo, false, false, false, 0);
	if (ret)
		goto out;

	nouveau_bo_placement_set(nvbo, memtype, 0);

	ret = nouveau_bo_validate(nvbo, false, false, false);
	if (ret == 0) {
		switch (bo->mem.mem_type) {
		case TTM_PL_VRAM:
			dev_priv->fb_aper_free -= bo->mem.size;
			break;
		case TTM_PL_TT:
			dev_priv->gart_info.aper_free -= bo->mem.size;
=======
nouveau_bo_placement_set(struct nouveau_bo *nvbo, uint32_t domain,
			 uint32_t busy)
{
	unsigned int *n = &nvbo->placement.num_placement;
	struct ttm_place *pl = nvbo->placements;

	domain |= busy;

	*n = 0;
	if (domain & NOUVEAU_GEM_DOMAIN_VRAM) {
		pl[*n].mem_type = TTM_PL_VRAM;
		pl[*n].flags = busy & NOUVEAU_GEM_DOMAIN_VRAM ?
			TTM_PL_FLAG_FALLBACK : 0;
		(*n)++;
	}
	if (domain & NOUVEAU_GEM_DOMAIN_GART) {
		pl[*n].mem_type = TTM_PL_TT;
		pl[*n].flags = busy & NOUVEAU_GEM_DOMAIN_GART ?
			TTM_PL_FLAG_FALLBACK : 0;
		(*n)++;
	}
	if (domain & NOUVEAU_GEM_DOMAIN_CPU) {
		pl[*n].mem_type = TTM_PL_SYSTEM;
		pl[*n].flags = busy & NOUVEAU_GEM_DOMAIN_CPU ?
			TTM_PL_FLAG_FALLBACK : 0;
		(*n)++;
	}

	nvbo->placement.placement = nvbo->placements;
	set_placement_range(nvbo, domain);
}

int
nouveau_bo_pin(struct nouveau_bo *nvbo, uint32_t domain, bool contig)
{
	struct nouveau_drm *drm = nouveau_bdev(nvbo->bo.bdev);
	struct ttm_buffer_object *bo = &nvbo->bo;
	bool force = false, evict = false;
	int ret;

	ret = ttm_bo_reserve(bo, false, false, NULL);
	if (ret)
		return ret;

	if (drm->client.device.info.family >= NV_DEVICE_INFO_V0_TESLA &&
	    domain == NOUVEAU_GEM_DOMAIN_VRAM && contig) {
		if (!nvbo->contig) {
			nvbo->contig = true;
			force = true;
			evict = true;
		}
	}

	if (nvbo->bo.pin_count) {
		bool error = evict;

		switch (bo->resource->mem_type) {
		case TTM_PL_VRAM:
			error |= !(domain & NOUVEAU_GEM_DOMAIN_VRAM);
			break;
		case TTM_PL_TT:
			error |= !(domain & NOUVEAU_GEM_DOMAIN_GART);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
			break;
		default:
			break;
		}
<<<<<<< HEAD
	}
	ttm_bo_unreserve(bo);
out:
	if (unlikely(ret))
		nvbo->pin_refcnt--;
=======

		if (error) {
			NV_ERROR(drm, "bo %p pinned elsewhere: "
				      "0x%08x vs 0x%08x\n", bo,
				 bo->resource->mem_type, domain);
			ret = -EBUSY;
		}
		ttm_bo_pin(&nvbo->bo);
		goto out;
	}

	if (evict) {
		nouveau_bo_placement_set(nvbo, NOUVEAU_GEM_DOMAIN_GART, 0);
		ret = nouveau_bo_validate(nvbo, false, false);
		if (ret)
			goto out;
	}

	nouveau_bo_placement_set(nvbo, domain, 0);
	ret = nouveau_bo_validate(nvbo, false, false);
	if (ret)
		goto out;

	ttm_bo_pin(&nvbo->bo);

	switch (bo->resource->mem_type) {
	case TTM_PL_VRAM:
		drm->gem.vram_available -= bo->base.size;
		break;
	case TTM_PL_TT:
		drm->gem.gart_available -= bo->base.size;
		break;
	default:
		break;
	}

out:
	if (force && ret)
		nvbo->contig = false;
	ttm_bo_unreserve(bo);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	return ret;
}

int
nouveau_bo_unpin(struct nouveau_bo *nvbo)
{
<<<<<<< HEAD
	struct drm_nouveau_private *dev_priv = nouveau_bdev(nvbo->bo.bdev);
	struct ttm_buffer_object *bo = &nvbo->bo;
	int ret;

	if (--nvbo->pin_refcnt)
		return 0;

	ret = ttm_bo_reserve(bo, false, false, false, 0);
	if (ret)
		return ret;

	nouveau_bo_placement_set(nvbo, bo->mem.placement, 0);

	ret = nouveau_bo_validate(nvbo, false, false, false);
	if (ret == 0) {
		switch (bo->mem.mem_type) {
		case TTM_PL_VRAM:
			dev_priv->fb_aper_free += bo->mem.size;
			break;
		case TTM_PL_TT:
			dev_priv->gart_info.aper_free += bo->mem.size;
=======
	struct nouveau_drm *drm = nouveau_bdev(nvbo->bo.bdev);
	struct ttm_buffer_object *bo = &nvbo->bo;
	int ret;

	ret = ttm_bo_reserve(bo, false, false, NULL);
	if (ret)
		return ret;

	ttm_bo_unpin(&nvbo->bo);
	if (!nvbo->bo.pin_count) {
		switch (bo->resource->mem_type) {
		case TTM_PL_VRAM:
			drm->gem.vram_available += bo->base.size;
			break;
		case TTM_PL_TT:
			drm->gem.gart_available += bo->base.size;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
			break;
		default:
			break;
		}
	}

	ttm_bo_unreserve(bo);
<<<<<<< HEAD
	return ret;
=======
	return 0;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

int
nouveau_bo_map(struct nouveau_bo *nvbo)
{
	int ret;

<<<<<<< HEAD
	ret = ttm_bo_reserve(&nvbo->bo, false, false, false, 0);
	if (ret)
		return ret;

	ret = ttm_bo_kmap(&nvbo->bo, 0, nvbo->bo.mem.num_pages, &nvbo->kmap);
=======
	ret = ttm_bo_reserve(&nvbo->bo, false, false, NULL);
	if (ret)
		return ret;

	ret = ttm_bo_kmap(&nvbo->bo, 0, PFN_UP(nvbo->bo.base.size), &nvbo->kmap);

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	ttm_bo_unreserve(&nvbo->bo);
	return ret;
}

void
nouveau_bo_unmap(struct nouveau_bo *nvbo)
{
<<<<<<< HEAD
	if (nvbo)
		ttm_bo_kunmap(&nvbo->kmap);
=======
	if (!nvbo)
		return;

	ttm_bo_kunmap(&nvbo->kmap);
}

void
nouveau_bo_sync_for_device(struct nouveau_bo *nvbo)
{
	struct nouveau_drm *drm = nouveau_bdev(nvbo->bo.bdev);
	struct ttm_tt *ttm_dma = (struct ttm_tt *)nvbo->bo.ttm;
	int i, j;

	if (!ttm_dma || !ttm_dma->dma_address)
		return;
	if (!ttm_dma->pages) {
		NV_DEBUG(drm, "ttm_dma 0x%p: pages NULL\n", ttm_dma);
		return;
	}

	/* Don't waste time looping if the object is coherent */
	if (nvbo->force_coherent)
		return;

	i = 0;
	while (i < ttm_dma->num_pages) {
		struct page *p = ttm_dma->pages[i];
		size_t num_pages = 1;

		for (j = i + 1; j < ttm_dma->num_pages; ++j) {
			if (++p != ttm_dma->pages[j])
				break;

			++num_pages;
		}
		dma_sync_single_for_device(drm->dev->dev,
					   ttm_dma->dma_address[i],
					   num_pages * PAGE_SIZE, DMA_TO_DEVICE);
		i += num_pages;
	}
}

void
nouveau_bo_sync_for_cpu(struct nouveau_bo *nvbo)
{
	struct nouveau_drm *drm = nouveau_bdev(nvbo->bo.bdev);
	struct ttm_tt *ttm_dma = (struct ttm_tt *)nvbo->bo.ttm;
	int i, j;

	if (!ttm_dma || !ttm_dma->dma_address)
		return;
	if (!ttm_dma->pages) {
		NV_DEBUG(drm, "ttm_dma 0x%p: pages NULL\n", ttm_dma);
		return;
	}

	/* Don't waste time looping if the object is coherent */
	if (nvbo->force_coherent)
		return;

	i = 0;
	while (i < ttm_dma->num_pages) {
		struct page *p = ttm_dma->pages[i];
		size_t num_pages = 1;

		for (j = i + 1; j < ttm_dma->num_pages; ++j) {
			if (++p != ttm_dma->pages[j])
				break;

			++num_pages;
		}

		dma_sync_single_for_cpu(drm->dev->dev, ttm_dma->dma_address[i],
					num_pages * PAGE_SIZE, DMA_FROM_DEVICE);
		i += num_pages;
	}
}

void nouveau_bo_add_io_reserve_lru(struct ttm_buffer_object *bo)
{
	struct nouveau_drm *drm = nouveau_bdev(bo->bdev);
	struct nouveau_bo *nvbo = nouveau_bo(bo);

	mutex_lock(&drm->ttm.io_reserve_mutex);
	list_move_tail(&nvbo->io_reserve_lru, &drm->ttm.io_reserve_lru);
	mutex_unlock(&drm->ttm.io_reserve_mutex);
}

void nouveau_bo_del_io_reserve_lru(struct ttm_buffer_object *bo)
{
	struct nouveau_drm *drm = nouveau_bdev(bo->bdev);
	struct nouveau_bo *nvbo = nouveau_bo(bo);

	mutex_lock(&drm->ttm.io_reserve_mutex);
	list_del_init(&nvbo->io_reserve_lru);
	mutex_unlock(&drm->ttm.io_reserve_mutex);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

int
nouveau_bo_validate(struct nouveau_bo *nvbo, bool interruptible,
<<<<<<< HEAD
		    bool no_wait_reserve, bool no_wait_gpu)
{
	int ret;

	ret = ttm_bo_validate(&nvbo->bo, &nvbo->placement, interruptible,
			      no_wait_reserve, no_wait_gpu);
	if (ret)
		return ret;

	return 0;
}

u16
nouveau_bo_rd16(struct nouveau_bo *nvbo, unsigned index)
{
	bool is_iomem;
	u16 *mem = ttm_kmap_obj_virtual(&nvbo->kmap, &is_iomem);
	mem = &mem[index];
	if (is_iomem)
		return ioread16_native((void __force __iomem *)mem);
	else
		return *mem;
=======
		    bool no_wait_gpu)
{
	struct ttm_operation_ctx ctx = { interruptible, no_wait_gpu };
	int ret;

	ret = ttm_bo_validate(&nvbo->bo, &nvbo->placement, &ctx);
	if (ret)
		return ret;

	nouveau_bo_sync_for_device(nvbo);

	return 0;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

void
nouveau_bo_wr16(struct nouveau_bo *nvbo, unsigned index, u16 val)
{
	bool is_iomem;
	u16 *mem = ttm_kmap_obj_virtual(&nvbo->kmap, &is_iomem);
<<<<<<< HEAD
	mem = &mem[index];
=======

	mem += index;

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	if (is_iomem)
		iowrite16_native(val, (void __force __iomem *)mem);
	else
		*mem = val;
}

u32
nouveau_bo_rd32(struct nouveau_bo *nvbo, unsigned index)
{
	bool is_iomem;
	u32 *mem = ttm_kmap_obj_virtual(&nvbo->kmap, &is_iomem);
<<<<<<< HEAD
	mem = &mem[index];
=======

	mem += index;

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	if (is_iomem)
		return ioread32_native((void __force __iomem *)mem);
	else
		return *mem;
}

void
nouveau_bo_wr32(struct nouveau_bo *nvbo, unsigned index, u32 val)
{
	bool is_iomem;
	u32 *mem = ttm_kmap_obj_virtual(&nvbo->kmap, &is_iomem);
<<<<<<< HEAD
	mem = &mem[index];
=======

	mem += index;

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	if (is_iomem)
		iowrite32_native(val, (void __force __iomem *)mem);
	else
		*mem = val;
}

static struct ttm_tt *
<<<<<<< HEAD
nouveau_ttm_tt_create(struct ttm_bo_device *bdev,
		      unsigned long size, uint32_t page_flags,
		      struct page *dummy_read_page)
{
	struct drm_nouveau_private *dev_priv = nouveau_bdev(bdev);
	struct drm_device *dev = dev_priv->dev;

	switch (dev_priv->gart_info.type) {
#if __OS_HAS_AGP
	case NOUVEAU_GART_AGP:
		return ttm_agp_tt_create(bdev, dev->agp->bridge,
					 size, page_flags, dummy_read_page);
#endif
	case NOUVEAU_GART_PDMA:
	case NOUVEAU_GART_HW:
		return nouveau_sgdma_create_ttm(bdev, size, page_flags,
						dummy_read_page);
	default:
		NV_ERROR(dev, "Unknown GART type %d\n",
			 dev_priv->gart_info.type);
		break;
	}

	return NULL;
}

static int
nouveau_bo_invalidate_caches(struct ttm_bo_device *bdev, uint32_t flags)
{
	/* We'll do this from user space. */
	return 0;
}

static int
nouveau_bo_init_mem_type(struct ttm_bo_device *bdev, uint32_t type,
			 struct ttm_mem_type_manager *man)
{
	struct drm_nouveau_private *dev_priv = nouveau_bdev(bdev);
	struct drm_device *dev = dev_priv->dev;

	switch (type) {
	case TTM_PL_SYSTEM:
		man->flags = TTM_MEMTYPE_FLAG_MAPPABLE;
		man->available_caching = TTM_PL_MASK_CACHING;
		man->default_caching = TTM_PL_FLAG_CACHED;
		break;
	case TTM_PL_VRAM:
		if (dev_priv->card_type >= NV_50) {
			man->func = &nouveau_vram_manager;
			man->io_reserve_fastpath = false;
			man->use_io_reserve_lru = true;
		} else {
			man->func = &ttm_bo_manager_func;
		}
		man->flags = TTM_MEMTYPE_FLAG_FIXED |
			     TTM_MEMTYPE_FLAG_MAPPABLE;
		man->available_caching = TTM_PL_FLAG_UNCACHED |
					 TTM_PL_FLAG_WC;
		man->default_caching = TTM_PL_FLAG_WC;
		break;
	case TTM_PL_TT:
		if (dev_priv->card_type >= NV_50)
			man->func = &nouveau_gart_manager;
		else
			man->func = &ttm_bo_manager_func;
		switch (dev_priv->gart_info.type) {
		case NOUVEAU_GART_AGP:
			man->flags = TTM_MEMTYPE_FLAG_MAPPABLE;
			man->available_caching = TTM_PL_FLAG_UNCACHED |
				TTM_PL_FLAG_WC;
			man->default_caching = TTM_PL_FLAG_WC;
			break;
		case NOUVEAU_GART_PDMA:
		case NOUVEAU_GART_HW:
			man->flags = TTM_MEMTYPE_FLAG_MAPPABLE |
				     TTM_MEMTYPE_FLAG_CMA;
			man->available_caching = TTM_PL_MASK_CACHING;
			man->default_caching = TTM_PL_FLAG_CACHED;
			break;
		default:
			NV_ERROR(dev, "Unknown GART type: %d\n",
				 dev_priv->gart_info.type);
			return -EINVAL;
		}
		break;
	default:
		NV_ERROR(dev, "Unsupported memory type %u\n", (unsigned)type);
		return -EINVAL;
	}
	return 0;
=======
nouveau_ttm_tt_create(struct ttm_buffer_object *bo, uint32_t page_flags)
{
#if IS_ENABLED(CONFIG_AGP)
	struct nouveau_drm *drm = nouveau_bdev(bo->bdev);

	if (drm->agp.bridge) {
		return ttm_agp_tt_create(bo, drm->agp.bridge, page_flags);
	}
#endif

	return nouveau_sgdma_create_ttm(bo, page_flags);
}

static int
nouveau_ttm_tt_bind(struct ttm_device *bdev, struct ttm_tt *ttm,
		    struct ttm_resource *reg)
{
#if IS_ENABLED(CONFIG_AGP)
	struct nouveau_drm *drm = nouveau_bdev(bdev);
#endif
	if (!reg)
		return -EINVAL;
#if IS_ENABLED(CONFIG_AGP)
	if (drm->agp.bridge)
		return ttm_agp_bind(ttm, reg);
#endif
	return nouveau_sgdma_bind(bdev, ttm, reg);
}

static void
nouveau_ttm_tt_unbind(struct ttm_device *bdev, struct ttm_tt *ttm)
{
#if IS_ENABLED(CONFIG_AGP)
	struct nouveau_drm *drm = nouveau_bdev(bdev);

	if (drm->agp.bridge) {
		ttm_agp_unbind(ttm);
		return;
	}
#endif
	nouveau_sgdma_unbind(bdev, ttm);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

static void
nouveau_bo_evict_flags(struct ttm_buffer_object *bo, struct ttm_placement *pl)
{
	struct nouveau_bo *nvbo = nouveau_bo(bo);

<<<<<<< HEAD
	switch (bo->mem.mem_type) {
	case TTM_PL_VRAM:
		nouveau_bo_placement_set(nvbo, TTM_PL_FLAG_TT,
					 TTM_PL_FLAG_SYSTEM);
		break;
	default:
		nouveau_bo_placement_set(nvbo, TTM_PL_FLAG_SYSTEM, 0);
=======
	switch (bo->resource->mem_type) {
	case TTM_PL_VRAM:
		nouveau_bo_placement_set(nvbo, NOUVEAU_GEM_DOMAIN_GART,
					 NOUVEAU_GEM_DOMAIN_CPU);
		break;
	default:
		nouveau_bo_placement_set(nvbo, NOUVEAU_GEM_DOMAIN_CPU, 0);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		break;
	}

	*pl = nvbo->placement;
}

<<<<<<< HEAD

/* GPU-assisted copy using NV_MEMORY_TO_MEMORY_FORMAT, can access
 * TTM_PL_{VRAM,TT} directly.
 */

static int
nouveau_bo_move_accel_cleanup(struct nouveau_channel *chan,
			      struct nouveau_bo *nvbo, bool evict,
			      bool no_wait_reserve, bool no_wait_gpu,
			      struct ttm_mem_reg *new_mem)
{
	struct nouveau_fence *fence = NULL;
	int ret;

	ret = nouveau_fence_new(chan, &fence, true);
	if (ret)
		return ret;

	ret = ttm_bo_move_accel_cleanup(&nvbo->bo, fence, NULL, evict,
					no_wait_reserve, no_wait_gpu, new_mem);
	nouveau_fence_unref(&fence);
	return ret;
}

static int
nvc0_bo_move_m2mf(struct nouveau_channel *chan, struct ttm_buffer_object *bo,
		  struct ttm_mem_reg *old_mem, struct ttm_mem_reg *new_mem)
{
	struct nouveau_mem *node = old_mem->mm_node;
	u64 src_offset = node->vma[0].offset;
	u64 dst_offset = node->vma[1].offset;
	u32 page_count = new_mem->num_pages;
	int ret;

	page_count = new_mem->num_pages;
	while (page_count) {
		int line_count = (page_count > 2047) ? 2047 : page_count;

		ret = RING_SPACE(chan, 12);
		if (ret)
			return ret;

		BEGIN_NVC0(chan, 2, NvSubM2MF, 0x0238, 2);
		OUT_RING  (chan, upper_32_bits(dst_offset));
		OUT_RING  (chan, lower_32_bits(dst_offset));
		BEGIN_NVC0(chan, 2, NvSubM2MF, 0x030c, 6);
		OUT_RING  (chan, upper_32_bits(src_offset));
		OUT_RING  (chan, lower_32_bits(src_offset));
		OUT_RING  (chan, PAGE_SIZE); /* src_pitch */
		OUT_RING  (chan, PAGE_SIZE); /* dst_pitch */
		OUT_RING  (chan, PAGE_SIZE); /* line_length */
		OUT_RING  (chan, line_count);
		BEGIN_NVC0(chan, 2, NvSubM2MF, 0x0300, 1);
		OUT_RING  (chan, 0x00100110);

		page_count -= line_count;
		src_offset += (PAGE_SIZE * line_count);
		dst_offset += (PAGE_SIZE * line_count);
	}

	return 0;
}

static int
nv50_bo_move_m2mf(struct nouveau_channel *chan, struct ttm_buffer_object *bo,
		  struct ttm_mem_reg *old_mem, struct ttm_mem_reg *new_mem)
{
	struct nouveau_mem *node = old_mem->mm_node;
	struct nouveau_bo *nvbo = nouveau_bo(bo);
	u64 length = (new_mem->num_pages << PAGE_SHIFT);
	u64 src_offset = node->vma[0].offset;
	u64 dst_offset = node->vma[1].offset;
	int ret;

	while (length) {
		u32 amount, stride, height;

		amount  = min(length, (u64)(4 * 1024 * 1024));
		stride  = 16 * 4;
		height  = amount / stride;

		if (new_mem->mem_type == TTM_PL_VRAM &&
		    nouveau_bo_tile_layout(nvbo)) {
			ret = RING_SPACE(chan, 8);
			if (ret)
				return ret;

			BEGIN_RING(chan, NvSubM2MF, 0x0200, 7);
			OUT_RING  (chan, 0);
			OUT_RING  (chan, 0);
			OUT_RING  (chan, stride);
			OUT_RING  (chan, height);
			OUT_RING  (chan, 1);
			OUT_RING  (chan, 0);
			OUT_RING  (chan, 0);
		} else {
			ret = RING_SPACE(chan, 2);
			if (ret)
				return ret;

			BEGIN_RING(chan, NvSubM2MF, 0x0200, 1);
			OUT_RING  (chan, 1);
		}
		if (old_mem->mem_type == TTM_PL_VRAM &&
		    nouveau_bo_tile_layout(nvbo)) {
			ret = RING_SPACE(chan, 8);
			if (ret)
				return ret;

			BEGIN_RING(chan, NvSubM2MF, 0x021c, 7);
			OUT_RING  (chan, 0);
			OUT_RING  (chan, 0);
			OUT_RING  (chan, stride);
			OUT_RING  (chan, height);
			OUT_RING  (chan, 1);
			OUT_RING  (chan, 0);
			OUT_RING  (chan, 0);
		} else {
			ret = RING_SPACE(chan, 2);
			if (ret)
				return ret;

			BEGIN_RING(chan, NvSubM2MF, 0x021c, 1);
			OUT_RING  (chan, 1);
		}

		ret = RING_SPACE(chan, 14);
		if (ret)
			return ret;

		BEGIN_RING(chan, NvSubM2MF, 0x0238, 2);
		OUT_RING  (chan, upper_32_bits(src_offset));
		OUT_RING  (chan, upper_32_bits(dst_offset));
		BEGIN_RING(chan, NvSubM2MF, 0x030c, 8);
		OUT_RING  (chan, lower_32_bits(src_offset));
		OUT_RING  (chan, lower_32_bits(dst_offset));
		OUT_RING  (chan, stride);
		OUT_RING  (chan, stride);
		OUT_RING  (chan, stride);
		OUT_RING  (chan, height);
		OUT_RING  (chan, 0x00000101);
		OUT_RING  (chan, 0x00000000);
		BEGIN_RING(chan, NvSubM2MF, NV_MEMORY_TO_MEMORY_FORMAT_NOP, 1);
		OUT_RING  (chan, 0);

		length -= amount;
		src_offset += amount;
		dst_offset += amount;
	}

	return 0;
}

static inline uint32_t
nouveau_bo_mem_ctxdma(struct ttm_buffer_object *bo,
		      struct nouveau_channel *chan, struct ttm_mem_reg *mem)
{
	if (mem->mem_type == TTM_PL_TT)
		return chan->gart_handle;
	return chan->vram_handle;
}

static int
nv04_bo_move_m2mf(struct nouveau_channel *chan, struct ttm_buffer_object *bo,
		  struct ttm_mem_reg *old_mem, struct ttm_mem_reg *new_mem)
{
	u32 src_offset = old_mem->start << PAGE_SHIFT;
	u32 dst_offset = new_mem->start << PAGE_SHIFT;
	u32 page_count = new_mem->num_pages;
	int ret;

	ret = RING_SPACE(chan, 3);
	if (ret)
		return ret;

	BEGIN_RING(chan, NvSubM2MF, NV_MEMORY_TO_MEMORY_FORMAT_DMA_SOURCE, 2);
	OUT_RING  (chan, nouveau_bo_mem_ctxdma(bo, chan, old_mem));
	OUT_RING  (chan, nouveau_bo_mem_ctxdma(bo, chan, new_mem));

	page_count = new_mem->num_pages;
	while (page_count) {
		int line_count = (page_count > 2047) ? 2047 : page_count;

		ret = RING_SPACE(chan, 11);
		if (ret)
			return ret;

		BEGIN_RING(chan, NvSubM2MF,
				 NV_MEMORY_TO_MEMORY_FORMAT_OFFSET_IN, 8);
		OUT_RING  (chan, src_offset);
		OUT_RING  (chan, dst_offset);
		OUT_RING  (chan, PAGE_SIZE); /* src_pitch */
		OUT_RING  (chan, PAGE_SIZE); /* dst_pitch */
		OUT_RING  (chan, PAGE_SIZE); /* line_length */
		OUT_RING  (chan, line_count);
		OUT_RING  (chan, 0x00000101);
		OUT_RING  (chan, 0x00000000);
		BEGIN_RING(chan, NvSubM2MF, NV_MEMORY_TO_MEMORY_FORMAT_NOP, 1);
		OUT_RING  (chan, 0);

		page_count -= line_count;
		src_offset += (PAGE_SIZE * line_count);
		dst_offset += (PAGE_SIZE * line_count);
	}

	return 0;
}

static int
nouveau_vma_getmap(struct nouveau_channel *chan, struct nouveau_bo *nvbo,
		   struct ttm_mem_reg *mem, struct nouveau_vma *vma)
{
	struct nouveau_mem *node = mem->mm_node;
	int ret;

	ret = nouveau_vm_get(chan->vm, mem->num_pages << PAGE_SHIFT,
			     node->page_shift, NV_MEM_ACCESS_RO, vma);
	if (ret)
		return ret;

	if (mem->mem_type == TTM_PL_VRAM)
		nouveau_vm_map(vma, node);
	else
		nouveau_vm_map_sg(vma, 0, mem->num_pages << PAGE_SHIFT, node);

=======
static int
nouveau_bo_move_prep(struct nouveau_drm *drm, struct ttm_buffer_object *bo,
		     struct ttm_resource *reg)
{
	struct nouveau_mem *old_mem = nouveau_mem(bo->resource);
	struct nouveau_mem *new_mem = nouveau_mem(reg);
	struct nvif_vmm *vmm = &drm->client.vmm.vmm;
	int ret;

	ret = nvif_vmm_get(vmm, LAZY, false, old_mem->mem.page, 0,
			   old_mem->mem.size, &old_mem->vma[0]);
	if (ret)
		return ret;

	ret = nvif_vmm_get(vmm, LAZY, false, new_mem->mem.page, 0,
			   new_mem->mem.size, &old_mem->vma[1]);
	if (ret)
		goto done;

	ret = nouveau_mem_map(old_mem, vmm, &old_mem->vma[0]);
	if (ret)
		goto done;

	ret = nouveau_mem_map(new_mem, vmm, &old_mem->vma[1]);
done:
	if (ret) {
		nvif_vmm_put(vmm, &old_mem->vma[1]);
		nvif_vmm_put(vmm, &old_mem->vma[0]);
	}
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	return 0;
}

static int
<<<<<<< HEAD
nouveau_bo_move_m2mf(struct ttm_buffer_object *bo, int evict, bool intr,
		     bool no_wait_reserve, bool no_wait_gpu,
		     struct ttm_mem_reg *new_mem)
{
	struct drm_nouveau_private *dev_priv = nouveau_bdev(bo->bdev);
	struct nouveau_channel *chan = chan = dev_priv->channel;
	struct nouveau_bo *nvbo = nouveau_bo(bo);
	struct ttm_mem_reg *old_mem = &bo->mem;
	int ret;

	mutex_lock_nested(&chan->mutex, NOUVEAU_KCHANNEL_MUTEX);

	/* create temporary vmas for the transfer and attach them to the
	 * old nouveau_mem node, these will get cleaned up after ttm has
	 * destroyed the ttm_mem_reg
	 */
	if (dev_priv->card_type >= NV_50) {
		struct nouveau_mem *node = old_mem->mm_node;

		ret = nouveau_vma_getmap(chan, nvbo, old_mem, &node->vma[0]);
		if (ret)
			goto out;

		ret = nouveau_vma_getmap(chan, nvbo, new_mem, &node->vma[1]);
		if (ret)
			goto out;
	}

	if (dev_priv->card_type < NV_50)
		ret = nv04_bo_move_m2mf(chan, bo, &bo->mem, new_mem);
	else
	if (dev_priv->card_type < NV_C0)
		ret = nv50_bo_move_m2mf(chan, bo, &bo->mem, new_mem);
	else
		ret = nvc0_bo_move_m2mf(chan, bo, &bo->mem, new_mem);
	if (ret == 0) {
		ret = nouveau_bo_move_accel_cleanup(chan, nvbo, evict,
						    no_wait_reserve,
						    no_wait_gpu, new_mem);
	}

out:
	mutex_unlock(&chan->mutex);
	return ret;
}

static int
nouveau_bo_move_flipd(struct ttm_buffer_object *bo, bool evict, bool intr,
		      bool no_wait_reserve, bool no_wait_gpu,
		      struct ttm_mem_reg *new_mem)
{
	u32 placement_memtype = TTM_PL_FLAG_TT | TTM_PL_MASK_CACHING;
	struct ttm_placement placement;
	struct ttm_mem_reg tmp_mem;
	int ret;

	placement.fpfn = placement.lpfn = 0;
	placement.num_placement = placement.num_busy_placement = 1;
	placement.placement = placement.busy_placement = &placement_memtype;

	tmp_mem = *new_mem;
	tmp_mem.mm_node = NULL;
	ret = ttm_bo_mem_space(bo, &placement, &tmp_mem, intr, no_wait_reserve, no_wait_gpu);
	if (ret)
		return ret;

	ret = ttm_tt_bind(bo->ttm, &tmp_mem);
	if (ret)
		goto out;

	ret = nouveau_bo_move_m2mf(bo, true, intr, no_wait_reserve, no_wait_gpu, &tmp_mem);
	if (ret)
		goto out;

	ret = ttm_bo_move_ttm(bo, true, no_wait_reserve, no_wait_gpu, new_mem);
out:
	ttm_bo_mem_put(bo, &tmp_mem);
	return ret;
}

static int
nouveau_bo_move_flips(struct ttm_buffer_object *bo, bool evict, bool intr,
		      bool no_wait_reserve, bool no_wait_gpu,
		      struct ttm_mem_reg *new_mem)
{
	u32 placement_memtype = TTM_PL_FLAG_TT | TTM_PL_MASK_CACHING;
	struct ttm_placement placement;
	struct ttm_mem_reg tmp_mem;
	int ret;

	placement.fpfn = placement.lpfn = 0;
	placement.num_placement = placement.num_busy_placement = 1;
	placement.placement = placement.busy_placement = &placement_memtype;

	tmp_mem = *new_mem;
	tmp_mem.mm_node = NULL;
	ret = ttm_bo_mem_space(bo, &placement, &tmp_mem, intr, no_wait_reserve, no_wait_gpu);
	if (ret)
		return ret;

	ret = ttm_bo_move_ttm(bo, true, no_wait_reserve, no_wait_gpu, &tmp_mem);
	if (ret)
		goto out;

	ret = nouveau_bo_move_m2mf(bo, true, intr, no_wait_reserve, no_wait_gpu, new_mem);
	if (ret)
		goto out;

out:
	ttm_bo_mem_put(bo, &tmp_mem);
	return ret;
}

static void
nouveau_bo_move_ntfy(struct ttm_buffer_object *bo, struct ttm_mem_reg *new_mem)
{
	struct nouveau_bo *nvbo = nouveau_bo(bo);
	struct nouveau_vma *vma;
=======
nouveau_bo_move_m2mf(struct ttm_buffer_object *bo, int evict,
		     struct ttm_operation_ctx *ctx,
		     struct ttm_resource *new_reg)
{
	struct nouveau_drm *drm = nouveau_bdev(bo->bdev);
	struct nouveau_channel *chan = drm->ttm.chan;
	struct nouveau_cli *cli = (void *)chan->user.client;
	struct nouveau_fence *fence;
	int ret;

	/* create temporary vmas for the transfer and attach them to the
	 * old nvkm_mem node, these will get cleaned up after ttm has
	 * destroyed the ttm_resource
	 */
	if (drm->client.device.info.family >= NV_DEVICE_INFO_V0_TESLA) {
		ret = nouveau_bo_move_prep(drm, bo, new_reg);
		if (ret)
			return ret;
	}

	if (drm_drv_uses_atomic_modeset(drm->dev))
		mutex_lock(&cli->mutex);
	else
		mutex_lock_nested(&cli->mutex, SINGLE_DEPTH_NESTING);

	ret = nouveau_fence_sync(nouveau_bo(bo), chan, true, ctx->interruptible);
	if (ret)
		goto out_unlock;

	ret = drm->ttm.move(chan, bo, bo->resource, new_reg);
	if (ret)
		goto out_unlock;

	ret = nouveau_fence_new(&fence, chan);
	if (ret)
		goto out_unlock;

	/* TODO: figure out a better solution here
	 *
	 * wait on the fence here explicitly as going through
	 * ttm_bo_move_accel_cleanup somehow doesn't seem to do it.
	 *
	 * Without this the operation can timeout and we'll fallback to a
	 * software copy, which might take several minutes to finish.
	 */
	nouveau_fence_wait(fence, false, false);
	ret = ttm_bo_move_accel_cleanup(bo, &fence->base, evict, false,
					new_reg);
	nouveau_fence_unref(&fence);

out_unlock:
	mutex_unlock(&cli->mutex);
	return ret;
}

void
nouveau_bo_move_init(struct nouveau_drm *drm)
{
	static const struct _method_table {
		const char *name;
		int engine;
		s32 oclass;
		int (*exec)(struct nouveau_channel *,
			    struct ttm_buffer_object *,
			    struct ttm_resource *, struct ttm_resource *);
		int (*init)(struct nouveau_channel *, u32 handle);
	} _methods[] = {
		{  "COPY", 4, 0xc7b5, nve0_bo_move_copy, nve0_bo_move_init },
		{  "GRCE", 0, 0xc7b5, nve0_bo_move_copy, nvc0_bo_move_init },
		{  "COPY", 4, 0xc6b5, nve0_bo_move_copy, nve0_bo_move_init },
		{  "GRCE", 0, 0xc6b5, nve0_bo_move_copy, nvc0_bo_move_init },
		{  "COPY", 4, 0xc5b5, nve0_bo_move_copy, nve0_bo_move_init },
		{  "GRCE", 0, 0xc5b5, nve0_bo_move_copy, nvc0_bo_move_init },
		{  "COPY", 4, 0xc3b5, nve0_bo_move_copy, nve0_bo_move_init },
		{  "GRCE", 0, 0xc3b5, nve0_bo_move_copy, nvc0_bo_move_init },
		{  "COPY", 4, 0xc1b5, nve0_bo_move_copy, nve0_bo_move_init },
		{  "GRCE", 0, 0xc1b5, nve0_bo_move_copy, nvc0_bo_move_init },
		{  "COPY", 4, 0xc0b5, nve0_bo_move_copy, nve0_bo_move_init },
		{  "GRCE", 0, 0xc0b5, nve0_bo_move_copy, nvc0_bo_move_init },
		{  "COPY", 4, 0xb0b5, nve0_bo_move_copy, nve0_bo_move_init },
		{  "GRCE", 0, 0xb0b5, nve0_bo_move_copy, nvc0_bo_move_init },
		{  "COPY", 4, 0xa0b5, nve0_bo_move_copy, nve0_bo_move_init },
		{  "GRCE", 0, 0xa0b5, nve0_bo_move_copy, nvc0_bo_move_init },
		{ "COPY1", 5, 0x90b8, nvc0_bo_move_copy, nvc0_bo_move_init },
		{ "COPY0", 4, 0x90b5, nvc0_bo_move_copy, nvc0_bo_move_init },
		{  "COPY", 0, 0x85b5, nva3_bo_move_copy, nv50_bo_move_init },
		{ "CRYPT", 0, 0x74c1, nv84_bo_move_exec, nv50_bo_move_init },
		{  "M2MF", 0, 0x9039, nvc0_bo_move_m2mf, nvc0_bo_move_init },
		{  "M2MF", 0, 0x5039, nv50_bo_move_m2mf, nv50_bo_move_init },
		{  "M2MF", 0, 0x0039, nv04_bo_move_m2mf, nv04_bo_move_init },
		{},
	};
	const struct _method_table *mthd = _methods;
	const char *name = "CPU";
	int ret;

	do {
		struct nouveau_channel *chan;

		if (mthd->engine)
			chan = drm->cechan;
		else
			chan = drm->channel;
		if (chan == NULL)
			continue;

		ret = nvif_object_ctor(&chan->user, "ttmBoMove",
				       mthd->oclass | (mthd->engine << 16),
				       mthd->oclass, NULL, 0,
				       &drm->ttm.copy);
		if (ret == 0) {
			ret = mthd->init(chan, drm->ttm.copy.handle);
			if (ret) {
				nvif_object_dtor(&drm->ttm.copy);
				continue;
			}

			drm->ttm.move = mthd->exec;
			drm->ttm.chan = chan;
			name = mthd->name;
			break;
		}
	} while ((++mthd)->exec);

	NV_INFO(drm, "MM: using %s for buffer copies\n", name);
}

static void nouveau_bo_move_ntfy(struct ttm_buffer_object *bo,
				 struct ttm_resource *new_reg)
{
	struct nouveau_mem *mem = new_reg ? nouveau_mem(new_reg) : NULL;
	struct nouveau_bo *nvbo = nouveau_bo(bo);
	struct nouveau_vma *vma;
	long ret;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	/* ttm can now (stupidly) pass the driver bos it didn't create... */
	if (bo->destroy != nouveau_bo_del_ttm)
		return;

<<<<<<< HEAD
	list_for_each_entry(vma, &nvbo->vma_list, head) {
		if (new_mem && new_mem->mem_type == TTM_PL_VRAM) {
			nouveau_vm_map(vma, new_mem->mm_node);
		} else
		if (new_mem && new_mem->mem_type == TTM_PL_TT &&
		    nvbo->page_shift == vma->vm->spg_shift) {
			nouveau_vm_map_sg(vma, 0, new_mem->
					  num_pages << PAGE_SHIFT,
					  new_mem->mm_node);
		} else {
			nouveau_vm_unmap(vma);
		}
	}
}

static int
nouveau_bo_vm_bind(struct ttm_buffer_object *bo, struct ttm_mem_reg *new_mem,
		   struct nouveau_tile_reg **new_tile)
{
	struct drm_nouveau_private *dev_priv = nouveau_bdev(bo->bdev);
	struct drm_device *dev = dev_priv->dev;
	struct nouveau_bo *nvbo = nouveau_bo(bo);
	u64 offset = new_mem->start << PAGE_SHIFT;

	*new_tile = NULL;
	if (new_mem->mem_type != TTM_PL_VRAM)
		return 0;

	if (dev_priv->card_type >= NV_10) {
		*new_tile = nv10_mem_set_tiling(dev, offset, new_mem->size,
						nvbo->tile_mode,
						nvbo->tile_flags);
=======
	nouveau_bo_del_io_reserve_lru(bo);

	if (mem && new_reg->mem_type != TTM_PL_SYSTEM &&
	    mem->mem.page == nvbo->page) {
		list_for_each_entry(vma, &nvbo->vma_list, head) {
			nouveau_vma_map(vma, mem);
		}
		nouveau_uvmm_bo_map_all(nvbo, mem);
	} else {
		list_for_each_entry(vma, &nvbo->vma_list, head) {
			ret = dma_resv_wait_timeout(bo->base.resv,
						    DMA_RESV_USAGE_BOOKKEEP,
						    false, 15 * HZ);
			WARN_ON(ret <= 0);
			nouveau_vma_unmap(vma);
		}
		nouveau_uvmm_bo_unmap_all(nvbo);
	}

	if (new_reg)
		nvbo->offset = (new_reg->start << PAGE_SHIFT);

}

static int
nouveau_bo_vm_bind(struct ttm_buffer_object *bo, struct ttm_resource *new_reg,
		   struct nouveau_drm_tile **new_tile)
{
	struct nouveau_drm *drm = nouveau_bdev(bo->bdev);
	struct drm_device *dev = drm->dev;
	struct nouveau_bo *nvbo = nouveau_bo(bo);
	u64 offset = new_reg->start << PAGE_SHIFT;

	*new_tile = NULL;
	if (new_reg->mem_type != TTM_PL_VRAM)
		return 0;

	if (drm->client.device.info.family >= NV_DEVICE_INFO_V0_CELSIUS) {
		*new_tile = nv10_bo_set_tiling(dev, offset, bo->base.size,
					       nvbo->mode, nvbo->zeta);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	}

	return 0;
}

static void
nouveau_bo_vm_cleanup(struct ttm_buffer_object *bo,
<<<<<<< HEAD
		      struct nouveau_tile_reg *new_tile,
		      struct nouveau_tile_reg **old_tile)
{
	struct drm_nouveau_private *dev_priv = nouveau_bdev(bo->bdev);
	struct drm_device *dev = dev_priv->dev;

	nv10_mem_put_tile_region(dev, *old_tile, bo->sync_obj);
=======
		      struct nouveau_drm_tile *new_tile,
		      struct nouveau_drm_tile **old_tile)
{
	struct nouveau_drm *drm = nouveau_bdev(bo->bdev);
	struct drm_device *dev = drm->dev;
	struct dma_fence *fence;
	int ret;

	ret = dma_resv_get_singleton(bo->base.resv, DMA_RESV_USAGE_WRITE,
				     &fence);
	if (ret)
		dma_resv_wait_timeout(bo->base.resv, DMA_RESV_USAGE_WRITE,
				      false, MAX_SCHEDULE_TIMEOUT);

	nv10_bo_put_tile_region(dev, *old_tile, fence);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	*old_tile = new_tile;
}

static int
<<<<<<< HEAD
nouveau_bo_move(struct ttm_buffer_object *bo, bool evict, bool intr,
		bool no_wait_reserve, bool no_wait_gpu,
		struct ttm_mem_reg *new_mem)
{
	struct drm_nouveau_private *dev_priv = nouveau_bdev(bo->bdev);
	struct nouveau_bo *nvbo = nouveau_bo(bo);
	struct ttm_mem_reg *old_mem = &bo->mem;
	struct nouveau_tile_reg *new_tile = NULL;
	int ret = 0;

	if (dev_priv->card_type < NV_50) {
		ret = nouveau_bo_vm_bind(bo, new_mem, &new_tile);
=======
nouveau_bo_move(struct ttm_buffer_object *bo, bool evict,
		struct ttm_operation_ctx *ctx,
		struct ttm_resource *new_reg,
		struct ttm_place *hop)
{
	struct nouveau_drm *drm = nouveau_bdev(bo->bdev);
	struct nouveau_bo *nvbo = nouveau_bo(bo);
	struct drm_gem_object *obj = &bo->base;
	struct ttm_resource *old_reg = bo->resource;
	struct nouveau_drm_tile *new_tile = NULL;
	int ret = 0;

	if (new_reg->mem_type == TTM_PL_TT) {
		ret = nouveau_ttm_tt_bind(bo->bdev, bo->ttm, new_reg);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		if (ret)
			return ret;
	}

<<<<<<< HEAD
	/* Fake bo copy. */
	if (old_mem->mem_type == TTM_PL_SYSTEM && !bo->ttm) {
		BUG_ON(bo->mem.mm_node != NULL);
		bo->mem = *new_mem;
		new_mem->mm_node = NULL;
		goto out;
	}

	/* Software copy if the card isn't up and running yet. */
	if (!dev_priv->channel) {
		ret = ttm_bo_move_memcpy(bo, evict, no_wait_reserve, no_wait_gpu, new_mem);
=======
	drm_gpuvm_bo_gem_evict(obj, evict);
	nouveau_bo_move_ntfy(bo, new_reg);
	ret = ttm_bo_wait_ctx(bo, ctx);
	if (ret)
		goto out_ntfy;

	if (drm->client.device.info.family < NV_DEVICE_INFO_V0_TESLA) {
		ret = nouveau_bo_vm_bind(bo, new_reg, &new_tile);
		if (ret)
			goto out_ntfy;
	}

	/* Fake bo copy. */
	if (!old_reg || (old_reg->mem_type == TTM_PL_SYSTEM &&
			 !bo->ttm)) {
		ttm_bo_move_null(bo, new_reg);
		goto out;
	}

	if (old_reg->mem_type == TTM_PL_SYSTEM &&
	    new_reg->mem_type == TTM_PL_TT) {
		ttm_bo_move_null(bo, new_reg);
		goto out;
	}

	if (old_reg->mem_type == TTM_PL_TT &&
	    new_reg->mem_type == TTM_PL_SYSTEM) {
		nouveau_ttm_tt_unbind(bo->bdev, bo->ttm);
		ttm_resource_free(bo, &bo->resource);
		ttm_bo_assign_mem(bo, new_reg);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		goto out;
	}

	/* Hardware assisted copy. */
<<<<<<< HEAD
	if (new_mem->mem_type == TTM_PL_SYSTEM)
		ret = nouveau_bo_move_flipd(bo, evict, intr, no_wait_reserve, no_wait_gpu, new_mem);
	else if (old_mem->mem_type == TTM_PL_SYSTEM)
		ret = nouveau_bo_move_flips(bo, evict, intr, no_wait_reserve, no_wait_gpu, new_mem);
	else
		ret = nouveau_bo_move_m2mf(bo, evict, intr, no_wait_reserve, no_wait_gpu, new_mem);

	if (!ret)
		goto out;

	/* Fallback to software copy. */
	ret = ttm_bo_move_memcpy(bo, evict, no_wait_reserve, no_wait_gpu, new_mem);

out:
	if (dev_priv->card_type < NV_50) {
=======
	if (drm->ttm.move) {
		if ((old_reg->mem_type == TTM_PL_SYSTEM &&
		     new_reg->mem_type == TTM_PL_VRAM) ||
		    (old_reg->mem_type == TTM_PL_VRAM &&
		     new_reg->mem_type == TTM_PL_SYSTEM)) {
			hop->fpfn = 0;
			hop->lpfn = 0;
			hop->mem_type = TTM_PL_TT;
			hop->flags = 0;
			return -EMULTIHOP;
		}
		ret = nouveau_bo_move_m2mf(bo, evict, ctx,
					   new_reg);
	} else
		ret = -ENODEV;

	if (ret) {
		/* Fallback to software copy. */
		ret = ttm_bo_move_memcpy(bo, ctx, new_reg);
	}

out:
	if (drm->client.device.info.family < NV_DEVICE_INFO_V0_TESLA) {
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		if (ret)
			nouveau_bo_vm_cleanup(bo, NULL, &new_tile);
		else
			nouveau_bo_vm_cleanup(bo, new_tile, &nvbo->tile);
	}
<<<<<<< HEAD

	return ret;
}

static int
nouveau_bo_verify_access(struct ttm_buffer_object *bo, struct file *filp)
{
	return 0;
}

static int
nouveau_ttm_io_mem_reserve(struct ttm_bo_device *bdev, struct ttm_mem_reg *mem)
{
	struct ttm_mem_type_manager *man = &bdev->man[mem->mem_type];
	struct drm_nouveau_private *dev_priv = nouveau_bdev(bdev);
	struct drm_device *dev = dev_priv->dev;
	int ret;

	mem->bus.addr = NULL;
	mem->bus.offset = 0;
	mem->bus.size = mem->num_pages << PAGE_SHIFT;
	mem->bus.base = 0;
	mem->bus.is_iomem = false;
	if (!(man->flags & TTM_MEMTYPE_FLAG_MAPPABLE))
		return -EINVAL;
	switch (mem->mem_type) {
	case TTM_PL_SYSTEM:
		/* System memory */
		return 0;
	case TTM_PL_TT:
#if __OS_HAS_AGP
		if (dev_priv->gart_info.type == NOUVEAU_GART_AGP) {
			mem->bus.offset = mem->start << PAGE_SHIFT;
			mem->bus.base = dev_priv->gart_info.aper_base;
			mem->bus.is_iomem = !dev->agp->cant_use_aperture;
		}
#endif
		break;
	case TTM_PL_VRAM:
	{
		struct nouveau_mem *node = mem->mm_node;
		u8 page_shift;

		if (!dev_priv->bar1_vm) {
			mem->bus.offset = mem->start << PAGE_SHIFT;
			mem->bus.base = pci_resource_start(dev->pdev, 1);
			mem->bus.is_iomem = true;
			break;
		}

		if (dev_priv->card_type >= NV_C0)
			page_shift = node->page_shift;
		else
			page_shift = 12;

		ret = nouveau_vm_get(dev_priv->bar1_vm, mem->bus.size,
				     page_shift, NV_MEM_ACCESS_RW,
				     &node->bar_vma);
		if (ret)
			return ret;

		nouveau_vm_map(&node->bar_vma, node);
		if (ret) {
			nouveau_vm_put(&node->bar_vma);
			return ret;
		}

		mem->bus.offset = node->bar_vma.offset;
		if (dev_priv->card_type == NV_50) /*XXX*/
			mem->bus.offset -= 0x0020000000ULL;
		mem->bus.base = pci_resource_start(dev->pdev, 1);
		mem->bus.is_iomem = true;
	}
		break;
	default:
		return -EINVAL;
	}
	return 0;
}

static void
nouveau_ttm_io_mem_free(struct ttm_bo_device *bdev, struct ttm_mem_reg *mem)
{
	struct drm_nouveau_private *dev_priv = nouveau_bdev(bdev);
	struct nouveau_mem *node = mem->mm_node;

	if (!dev_priv->bar1_vm || mem->mem_type != TTM_PL_VRAM)
		return;

	if (!node->bar_vma.node)
		return;

	nouveau_vm_unmap(&node->bar_vma);
	nouveau_vm_put(&node->bar_vma);
}

static int
nouveau_ttm_fault_reserve_notify(struct ttm_buffer_object *bo)
{
	struct drm_nouveau_private *dev_priv = nouveau_bdev(bo->bdev);
	struct nouveau_bo *nvbo = nouveau_bo(bo);
=======
out_ntfy:
	if (ret) {
		nouveau_bo_move_ntfy(bo, bo->resource);
		drm_gpuvm_bo_gem_evict(obj, !evict);
	}
	return ret;
}

static void
nouveau_ttm_io_mem_free_locked(struct nouveau_drm *drm,
			       struct ttm_resource *reg)
{
	struct nouveau_mem *mem = nouveau_mem(reg);

	if (drm->client.mem->oclass >= NVIF_CLASS_MEM_NV50) {
		switch (reg->mem_type) {
		case TTM_PL_TT:
			if (mem->kind)
				nvif_object_unmap_handle(&mem->mem.object);
			break;
		case TTM_PL_VRAM:
			nvif_object_unmap_handle(&mem->mem.object);
			break;
		default:
			break;
		}
	}
}

static int
nouveau_ttm_io_mem_reserve(struct ttm_device *bdev, struct ttm_resource *reg)
{
	struct nouveau_drm *drm = nouveau_bdev(bdev);
	struct nvkm_device *device = nvxx_device(&drm->client.device);
	struct nouveau_mem *mem = nouveau_mem(reg);
	struct nvif_mmu *mmu = &drm->client.mmu;
	int ret;

	mutex_lock(&drm->ttm.io_reserve_mutex);
retry:
	switch (reg->mem_type) {
	case TTM_PL_SYSTEM:
		/* System memory */
		ret = 0;
		goto out;
	case TTM_PL_TT:
#if IS_ENABLED(CONFIG_AGP)
		if (drm->agp.bridge) {
			reg->bus.offset = (reg->start << PAGE_SHIFT) +
				drm->agp.base;
			reg->bus.is_iomem = !drm->agp.cma;
			reg->bus.caching = ttm_write_combined;
		}
#endif
		if (drm->client.mem->oclass < NVIF_CLASS_MEM_NV50 ||
		    !mem->kind) {
			/* untiled */
			ret = 0;
			break;
		}
		fallthrough;	/* tiled memory */
	case TTM_PL_VRAM:
		reg->bus.offset = (reg->start << PAGE_SHIFT) +
			device->func->resource_addr(device, 1);
		reg->bus.is_iomem = true;

		/* Some BARs do not support being ioremapped WC */
		if (drm->client.device.info.family >= NV_DEVICE_INFO_V0_TESLA &&
		    mmu->type[drm->ttm.type_vram].type & NVIF_MEM_UNCACHED)
			reg->bus.caching = ttm_uncached;
		else
			reg->bus.caching = ttm_write_combined;

		if (drm->client.mem->oclass >= NVIF_CLASS_MEM_NV50) {
			union {
				struct nv50_mem_map_v0 nv50;
				struct gf100_mem_map_v0 gf100;
			} args;
			u64 handle, length;
			u32 argc = 0;

			switch (mem->mem.object.oclass) {
			case NVIF_CLASS_MEM_NV50:
				args.nv50.version = 0;
				args.nv50.ro = 0;
				args.nv50.kind = mem->kind;
				args.nv50.comp = mem->comp;
				argc = sizeof(args.nv50);
				break;
			case NVIF_CLASS_MEM_GF100:
				args.gf100.version = 0;
				args.gf100.ro = 0;
				args.gf100.kind = mem->kind;
				argc = sizeof(args.gf100);
				break;
			default:
				WARN_ON(1);
				break;
			}

			ret = nvif_object_map_handle(&mem->mem.object,
						     &args, argc,
						     &handle, &length);
			if (ret != 1) {
				if (WARN_ON(ret == 0))
					ret = -EINVAL;
				goto out;
			}

			reg->bus.offset = handle;
		}
		ret = 0;
		break;
	default:
		ret = -EINVAL;
	}

out:
	if (ret == -ENOSPC) {
		struct nouveau_bo *nvbo;

		nvbo = list_first_entry_or_null(&drm->ttm.io_reserve_lru,
						typeof(*nvbo),
						io_reserve_lru);
		if (nvbo) {
			list_del_init(&nvbo->io_reserve_lru);
			drm_vma_node_unmap(&nvbo->bo.base.vma_node,
					   bdev->dev_mapping);
			nouveau_ttm_io_mem_free_locked(drm, nvbo->bo.resource);
			nvbo->bo.resource->bus.offset = 0;
			nvbo->bo.resource->bus.addr = NULL;
			goto retry;
		}

	}
	mutex_unlock(&drm->ttm.io_reserve_mutex);
	return ret;
}

static void
nouveau_ttm_io_mem_free(struct ttm_device *bdev, struct ttm_resource *reg)
{
	struct nouveau_drm *drm = nouveau_bdev(bdev);

	mutex_lock(&drm->ttm.io_reserve_mutex);
	nouveau_ttm_io_mem_free_locked(drm, reg);
	mutex_unlock(&drm->ttm.io_reserve_mutex);
}

vm_fault_t nouveau_ttm_fault_reserve_notify(struct ttm_buffer_object *bo)
{
	struct nouveau_drm *drm = nouveau_bdev(bo->bdev);
	struct nouveau_bo *nvbo = nouveau_bo(bo);
	struct nvkm_device *device = nvxx_device(&drm->client.device);
	u32 mappable = device->func->resource_size(device, 1) >> PAGE_SHIFT;
	int i, ret;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	/* as long as the bo isn't in vram, and isn't tiled, we've got
	 * nothing to do here.
	 */
<<<<<<< HEAD
	if (bo->mem.mem_type != TTM_PL_VRAM) {
		if (dev_priv->card_type < NV_50 ||
		    !nouveau_bo_tile_layout(nvbo))
			return 0;
	}

	/* make sure bo is in mappable vram */
	if (bo->mem.start + bo->mem.num_pages < dev_priv->fb_mappable_pages)
		return 0;


	nvbo->placement.fpfn = 0;
	nvbo->placement.lpfn = dev_priv->fb_mappable_pages;
	nouveau_bo_placement_set(nvbo, TTM_PL_FLAG_VRAM, 0);
	return nouveau_bo_validate(nvbo, false, true, false);
}

void
nouveau_bo_fence(struct nouveau_bo *nvbo, struct nouveau_fence *fence)
{
	struct nouveau_fence *old_fence;

	if (likely(fence))
		nouveau_fence_ref(fence);

	spin_lock(&nvbo->bo.bdev->fence_lock);
	old_fence = nvbo->bo.sync_obj;
	nvbo->bo.sync_obj = fence;
	spin_unlock(&nvbo->bo.bdev->fence_lock);

	nouveau_fence_unref(&old_fence);
}

static int
nouveau_ttm_tt_populate(struct ttm_tt *ttm)
{
	struct ttm_dma_tt *ttm_dma = (void *)ttm;
	struct drm_nouveau_private *dev_priv;
	struct drm_device *dev;
	unsigned i;
	int r;

	if (ttm->state != tt_unpopulated)
		return 0;

	dev_priv = nouveau_bdev(ttm->bdev);
	dev = dev_priv->dev;

#if __OS_HAS_AGP
	if (dev_priv->gart_info.type == NOUVEAU_GART_AGP) {
		return ttm_agp_tt_populate(ttm);
	}
#endif

#ifdef CONFIG_SWIOTLB
	if (swiotlb_nr_tbl()) {
		return ttm_dma_populate((void *)ttm, dev->dev);
	}
#endif

	r = ttm_pool_populate(ttm);
	if (r) {
		return r;
	}

	for (i = 0; i < ttm->num_pages; i++) {
		ttm_dma->dma_address[i] = pci_map_page(dev->pdev, ttm->pages[i],
						   0, PAGE_SIZE,
						   PCI_DMA_BIDIRECTIONAL);
		if (pci_dma_mapping_error(dev->pdev, ttm_dma->dma_address[i])) {
			while (--i) {
				pci_unmap_page(dev->pdev, ttm_dma->dma_address[i],
					       PAGE_SIZE, PCI_DMA_BIDIRECTIONAL);
				ttm_dma->dma_address[i] = 0;
			}
			ttm_pool_unpopulate(ttm);
			return -EFAULT;
		}
	}
	return 0;
}

static void
nouveau_ttm_tt_unpopulate(struct ttm_tt *ttm)
{
	struct ttm_dma_tt *ttm_dma = (void *)ttm;
	struct drm_nouveau_private *dev_priv;
	struct drm_device *dev;
	unsigned i;

	dev_priv = nouveau_bdev(ttm->bdev);
	dev = dev_priv->dev;

#if __OS_HAS_AGP
	if (dev_priv->gart_info.type == NOUVEAU_GART_AGP) {
		ttm_agp_tt_unpopulate(ttm);
		return;
	}
#endif

#ifdef CONFIG_SWIOTLB
	if (swiotlb_nr_tbl()) {
		ttm_dma_unpopulate((void *)ttm, dev->dev);
		return;
	}
#endif

	for (i = 0; i < ttm->num_pages; i++) {
		if (ttm_dma->dma_address[i]) {
			pci_unmap_page(dev->pdev, ttm_dma->dma_address[i],
				       PAGE_SIZE, PCI_DMA_BIDIRECTIONAL);
		}
	}

	ttm_pool_unpopulate(ttm);
}

struct ttm_bo_driver nouveau_bo_driver = {
	.ttm_tt_create = &nouveau_ttm_tt_create,
	.ttm_tt_populate = &nouveau_ttm_tt_populate,
	.ttm_tt_unpopulate = &nouveau_ttm_tt_unpopulate,
	.invalidate_caches = nouveau_bo_invalidate_caches,
	.init_mem_type = nouveau_bo_init_mem_type,
	.evict_flags = nouveau_bo_evict_flags,
	.move_notify = nouveau_bo_move_ntfy,
	.move = nouveau_bo_move,
	.verify_access = nouveau_bo_verify_access,
	.sync_obj_signaled = __nouveau_fence_signalled,
	.sync_obj_wait = __nouveau_fence_wait,
	.sync_obj_flush = __nouveau_fence_flush,
	.sync_obj_unref = __nouveau_fence_unref,
	.sync_obj_ref = __nouveau_fence_ref,
	.fault_reserve_notify = &nouveau_ttm_fault_reserve_notify,
	.io_mem_reserve = &nouveau_ttm_io_mem_reserve,
	.io_mem_free = &nouveau_ttm_io_mem_free,
};

struct nouveau_vma *
nouveau_bo_vma_find(struct nouveau_bo *nvbo, struct nouveau_vm *vm)
{
	struct nouveau_vma *vma;
	list_for_each_entry(vma, &nvbo->vma_list, head) {
		if (vma->vm == vm)
			return vma;
	}

	return NULL;
}

int
nouveau_bo_vma_add(struct nouveau_bo *nvbo, struct nouveau_vm *vm,
		   struct nouveau_vma *vma)
{
	const u32 size = nvbo->bo.mem.num_pages << PAGE_SHIFT;
	struct nouveau_mem *node = nvbo->bo.mem.mm_node;
	int ret;

	ret = nouveau_vm_get(vm, size, nvbo->page_shift,
			     NV_MEM_ACCESS_RW, vma);
	if (ret)
		return ret;

	if (nvbo->bo.mem.mem_type == TTM_PL_VRAM)
		nouveau_vm_map(vma, nvbo->bo.mem.mm_node);
	else
	if (nvbo->bo.mem.mem_type == TTM_PL_TT)
		nouveau_vm_map_sg(vma, 0, size, node);

	list_add_tail(&vma->head, &nvbo->vma_list);
	vma->refcount = 1;
	return 0;
}

void
nouveau_bo_vma_del(struct nouveau_bo *nvbo, struct nouveau_vma *vma)
{
	if (vma->node) {
		if (nvbo->bo.mem.mem_type != TTM_PL_SYSTEM) {
			spin_lock(&nvbo->bo.bdev->fence_lock);
			ttm_bo_wait(&nvbo->bo, false, false, false);
			spin_unlock(&nvbo->bo.bdev->fence_lock);
			nouveau_vm_unmap(vma);
		}

		nouveau_vm_put(vma);
		list_del(&vma->head);
	}
}
=======
	if (bo->resource->mem_type != TTM_PL_VRAM) {
		if (drm->client.device.info.family < NV_DEVICE_INFO_V0_TESLA ||
		    !nvbo->kind)
			return 0;

		if (bo->resource->mem_type != TTM_PL_SYSTEM)
			return 0;

		nouveau_bo_placement_set(nvbo, NOUVEAU_GEM_DOMAIN_GART, 0);

	} else {
		/* make sure bo is in mappable vram */
		if (drm->client.device.info.family >= NV_DEVICE_INFO_V0_TESLA ||
		    bo->resource->start + PFN_UP(bo->resource->size) < mappable)
			return 0;

		for (i = 0; i < nvbo->placement.num_placement; ++i) {
			nvbo->placements[i].fpfn = 0;
			nvbo->placements[i].lpfn = mappable;
		}

		nouveau_bo_placement_set(nvbo, NOUVEAU_GEM_DOMAIN_VRAM, 0);
	}

	ret = nouveau_bo_validate(nvbo, false, false);
	if (unlikely(ret == -EBUSY || ret == -ERESTARTSYS))
		return VM_FAULT_NOPAGE;
	else if (unlikely(ret))
		return VM_FAULT_SIGBUS;

	ttm_bo_move_to_lru_tail_unlocked(bo);
	return 0;
}

static int
nouveau_ttm_tt_populate(struct ttm_device *bdev,
			struct ttm_tt *ttm, struct ttm_operation_ctx *ctx)
{
	struct ttm_tt *ttm_dma = (void *)ttm;
	struct nouveau_drm *drm;
	bool slave = !!(ttm->page_flags & TTM_TT_FLAG_EXTERNAL);

	if (ttm_tt_is_populated(ttm))
		return 0;

	if (slave && ttm->sg) {
		drm_prime_sg_to_dma_addr_array(ttm->sg, ttm_dma->dma_address,
					       ttm->num_pages);
		return 0;
	}

	drm = nouveau_bdev(bdev);

	return ttm_pool_alloc(&drm->ttm.bdev.pool, ttm, ctx);
}

static void
nouveau_ttm_tt_unpopulate(struct ttm_device *bdev,
			  struct ttm_tt *ttm)
{
	struct nouveau_drm *drm;
	bool slave = !!(ttm->page_flags & TTM_TT_FLAG_EXTERNAL);

	if (slave)
		return;

	nouveau_ttm_tt_unbind(bdev, ttm);

	drm = nouveau_bdev(bdev);

	return ttm_pool_free(&drm->ttm.bdev.pool, ttm);
}

static void
nouveau_ttm_tt_destroy(struct ttm_device *bdev,
		       struct ttm_tt *ttm)
{
#if IS_ENABLED(CONFIG_AGP)
	struct nouveau_drm *drm = nouveau_bdev(bdev);
	if (drm->agp.bridge) {
		ttm_agp_destroy(ttm);
		return;
	}
#endif
	nouveau_sgdma_destroy(bdev, ttm);
}

void
nouveau_bo_fence(struct nouveau_bo *nvbo, struct nouveau_fence *fence, bool exclusive)
{
	struct dma_resv *resv = nvbo->bo.base.resv;

	if (!fence)
		return;

	dma_resv_add_fence(resv, &fence->base, exclusive ?
			   DMA_RESV_USAGE_WRITE : DMA_RESV_USAGE_READ);
}

static void
nouveau_bo_delete_mem_notify(struct ttm_buffer_object *bo)
{
	nouveau_bo_move_ntfy(bo, NULL);
}

struct ttm_device_funcs nouveau_bo_driver = {
	.ttm_tt_create = &nouveau_ttm_tt_create,
	.ttm_tt_populate = &nouveau_ttm_tt_populate,
	.ttm_tt_unpopulate = &nouveau_ttm_tt_unpopulate,
	.ttm_tt_destroy = &nouveau_ttm_tt_destroy,
	.eviction_valuable = ttm_bo_eviction_valuable,
	.evict_flags = nouveau_bo_evict_flags,
	.delete_mem_notify = nouveau_bo_delete_mem_notify,
	.move = nouveau_bo_move,
	.io_mem_reserve = &nouveau_ttm_io_mem_reserve,
	.io_mem_free = &nouveau_ttm_io_mem_free,
};
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
