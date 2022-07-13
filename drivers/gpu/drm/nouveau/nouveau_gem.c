/*
 * Copyright (C) 2008 Ben Skeggs.
 * All Rights Reserved.
 *
 * Permission is hereby granted, free of charge, to any person obtaining
 * a copy of this software and associated documentation files (the
 * "Software"), to deal in the Software without restriction, including
 * without limitation the rights to use, copy, modify, merge, publish,
 * distribute, sublicense, and/or sell copies of the Software, and to
 * permit persons to whom the Software is furnished to do so, subject to
 * the following conditions:
 *
 * The above copyright notice and this permission notice (including the
 * next paragraph) shall be included in all copies or substantial
 * portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
 * IN NO EVENT SHALL THE COPYRIGHT OWNER(S) AND/OR ITS SUPPLIERS BE
 * LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION
 * OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION
 * WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 *
 */
<<<<<<< HEAD
#include "drmP.h"
#include "drm.h"

#include "nouveau_drv.h"
#include "nouveau_drm.h"
#include "nouveau_dma.h"

#define nouveau_gem_pushbuf_sync(chan) 0

int
nouveau_gem_object_new(struct drm_gem_object *gem)
{
	return 0;
}

void
nouveau_gem_object_del(struct drm_gem_object *gem)
{
	struct nouveau_bo *nvbo = gem->driver_private;
	struct ttm_buffer_object *bo = &nvbo->bo;

	if (!nvbo)
		return;
	nvbo->gem = NULL;

	if (unlikely(nvbo->pin_refcnt)) {
		nvbo->pin_refcnt = 1;
		nouveau_bo_unpin(nvbo);
	}

	ttm_bo_unref(&bo);

	drm_gem_object_release(gem);
	kfree(gem);
=======

#include <drm/drm_gem_ttm_helper.h>

#include "nouveau_drv.h"
#include "nouveau_dma.h"
#include "nouveau_fence.h"
#include "nouveau_abi16.h"

#include "nouveau_ttm.h"
#include "nouveau_gem.h"
#include "nouveau_mem.h"
#include "nouveau_vmm.h"

#include <nvif/class.h>
#include <nvif/push206e.h>

static vm_fault_t nouveau_ttm_fault(struct vm_fault *vmf)
{
	struct vm_area_struct *vma = vmf->vma;
	struct ttm_buffer_object *bo = vma->vm_private_data;
	pgprot_t prot;
	vm_fault_t ret;

	ret = ttm_bo_vm_reserve(bo, vmf);
	if (ret)
		return ret;

	ret = nouveau_ttm_fault_reserve_notify(bo);
	if (ret)
		goto error_unlock;

	nouveau_bo_del_io_reserve_lru(bo);
	prot = vm_get_page_prot(vma->vm_flags);
	ret = ttm_bo_vm_fault_reserved(vmf, prot, TTM_BO_VM_NUM_PREFAULT);
	nouveau_bo_add_io_reserve_lru(bo);
	if (ret == VM_FAULT_RETRY && !(vmf->flags & FAULT_FLAG_RETRY_NOWAIT))
		return ret;

error_unlock:
	dma_resv_unlock(bo->base.resv);
	return ret;
}

static const struct vm_operations_struct nouveau_ttm_vm_ops = {
	.fault = nouveau_ttm_fault,
	.open = ttm_bo_vm_open,
	.close = ttm_bo_vm_close,
	.access = ttm_bo_vm_access
};

void
nouveau_gem_object_del(struct drm_gem_object *gem)
{
	struct nouveau_bo *nvbo = nouveau_gem_object(gem);
	struct nouveau_drm *drm = nouveau_bdev(nvbo->bo.bdev);
	struct device *dev = drm->dev->dev;
	int ret;

	ret = pm_runtime_get_sync(dev);
	if (WARN_ON(ret < 0 && ret != -EACCES)) {
		pm_runtime_put_autosuspend(dev);
		return;
	}

	if (gem->import_attach)
		drm_prime_gem_destroy(gem, nvbo->bo.sg);

	ttm_bo_put(&nvbo->bo);

	pm_runtime_mark_last_busy(dev);
	pm_runtime_put_autosuspend(dev);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

int
nouveau_gem_object_open(struct drm_gem_object *gem, struct drm_file *file_priv)
{
<<<<<<< HEAD
	struct nouveau_fpriv *fpriv = nouveau_fpriv(file_priv);
	struct nouveau_bo *nvbo = nouveau_gem_object(gem);
	struct nouveau_vma *vma;
	int ret;

	if (!fpriv->vm)
		return 0;

	ret = ttm_bo_reserve(&nvbo->bo, false, false, false, 0);
	if (ret)
		return ret;

	vma = nouveau_bo_vma_find(nvbo, fpriv->vm);
	if (!vma) {
		vma = kzalloc(sizeof(*vma), GFP_KERNEL);
		if (!vma) {
			ret = -ENOMEM;
			goto out;
		}

		ret = nouveau_bo_vma_add(nvbo, fpriv->vm, vma);
		if (ret) {
			kfree(vma);
			goto out;
		}
	} else {
		vma->refcount++;
	}

=======
	struct nouveau_cli *cli = nouveau_cli(file_priv);
	struct nouveau_bo *nvbo = nouveau_gem_object(gem);
	struct nouveau_drm *drm = nouveau_bdev(nvbo->bo.bdev);
	struct device *dev = drm->dev->dev;
	struct nouveau_uvmm *uvmm = nouveau_cli_uvmm(cli);
	struct nouveau_vmm *vmm = nouveau_cli_vmm(cli);
	struct nouveau_vma *vma;
	int ret;

	if (vmm->vmm.object.oclass < NVIF_CLASS_VMM_NV50)
		return 0;

	if (nvbo->no_share && uvmm &&
	    drm_gpuvm_resv(&uvmm->base) != nvbo->bo.base.resv)
		return -EPERM;

	ret = ttm_bo_reserve(&nvbo->bo, false, false, NULL);
	if (ret)
		return ret;

	ret = pm_runtime_get_sync(dev);
	if (ret < 0 && ret != -EACCES) {
		pm_runtime_put_autosuspend(dev);
		goto out;
	}

	/* only create a VMA on binding */
	if (!nouveau_cli_uvmm(cli))
		ret = nouveau_vma_new(nvbo, vmm, &vma);
	else
		ret = 0;
	pm_runtime_mark_last_busy(dev);
	pm_runtime_put_autosuspend(dev);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
out:
	ttm_bo_unreserve(&nvbo->bo);
	return ret;
}

<<<<<<< HEAD
void
nouveau_gem_object_close(struct drm_gem_object *gem, struct drm_file *file_priv)
{
	struct nouveau_fpriv *fpriv = nouveau_fpriv(file_priv);
	struct nouveau_bo *nvbo = nouveau_gem_object(gem);
	struct nouveau_vma *vma;
	int ret;

	if (!fpriv->vm)
		return;

	ret = ttm_bo_reserve(&nvbo->bo, false, false, false, 0);
	if (ret)
		return;

	vma = nouveau_bo_vma_find(nvbo, fpriv->vm);
	if (vma) {
		if (--vma->refcount == 0) {
			nouveau_bo_vma_del(nvbo, vma);
			kfree(vma);
=======
struct nouveau_gem_object_unmap {
	struct nouveau_cli_work work;
	struct nouveau_vma *vma;
};

static void
nouveau_gem_object_delete(struct nouveau_vma *vma)
{
	nouveau_fence_unref(&vma->fence);
	nouveau_vma_del(&vma);
}

static void
nouveau_gem_object_delete_work(struct nouveau_cli_work *w)
{
	struct nouveau_gem_object_unmap *work =
		container_of(w, typeof(*work), work);
	nouveau_gem_object_delete(work->vma);
	kfree(work);
}

static void
nouveau_gem_object_unmap(struct nouveau_bo *nvbo, struct nouveau_vma *vma)
{
	struct dma_fence *fence = vma->fence ? &vma->fence->base : NULL;
	struct nouveau_gem_object_unmap *work;

	list_del_init(&vma->head);

	if (!fence) {
		nouveau_gem_object_delete(vma);
		return;
	}

	if (!(work = kmalloc(sizeof(*work), GFP_KERNEL))) {
		WARN_ON(dma_fence_wait_timeout(fence, false, 2 * HZ) <= 0);
		nouveau_gem_object_delete(vma);
		return;
	}

	work->work.func = nouveau_gem_object_delete_work;
	work->vma = vma;
	nouveau_cli_work_queue(vma->vmm->cli, fence, &work->work);
}

void
nouveau_gem_object_close(struct drm_gem_object *gem, struct drm_file *file_priv)
{
	struct nouveau_cli *cli = nouveau_cli(file_priv);
	struct nouveau_bo *nvbo = nouveau_gem_object(gem);
	struct nouveau_drm *drm = nouveau_bdev(nvbo->bo.bdev);
	struct device *dev = drm->dev->dev;
	struct nouveau_vmm *vmm = nouveau_cli_vmm(cli);
	struct nouveau_vma *vma;
	int ret;

	if (vmm->vmm.object.oclass < NVIF_CLASS_VMM_NV50)
		return;

	if (nouveau_cli_uvmm(cli))
		return;

	ret = ttm_bo_reserve(&nvbo->bo, false, false, NULL);
	if (ret)
		return;

	vma = nouveau_vma_find(nvbo, vmm);
	if (vma) {
		if (--vma->refs == 0) {
			ret = pm_runtime_get_sync(dev);
			if (!WARN_ON(ret < 0 && ret != -EACCES)) {
				nouveau_gem_object_unmap(nvbo, vma);
				pm_runtime_mark_last_busy(dev);
			}
			pm_runtime_put_autosuspend(dev);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		}
	}
	ttm_bo_unreserve(&nvbo->bo);
}

<<<<<<< HEAD
int
nouveau_gem_new(struct drm_device *dev, int size, int align, uint32_t domain,
		uint32_t tile_mode, uint32_t tile_flags,
		struct nouveau_bo **pnvbo)
{
	struct drm_nouveau_private *dev_priv = dev->dev_private;
	struct nouveau_bo *nvbo;
	u32 flags = 0;
	int ret;

	if (domain & NOUVEAU_GEM_DOMAIN_VRAM)
		flags |= TTM_PL_FLAG_VRAM;
	if (domain & NOUVEAU_GEM_DOMAIN_GART)
		flags |= TTM_PL_FLAG_TT;
	if (!flags || domain & NOUVEAU_GEM_DOMAIN_CPU)
		flags |= TTM_PL_FLAG_SYSTEM;

	ret = nouveau_bo_new(dev, size, align, flags, tile_mode,
			     tile_flags, pnvbo);
	if (ret)
		return ret;
	nvbo = *pnvbo;
=======
const struct drm_gem_object_funcs nouveau_gem_object_funcs = {
	.free = nouveau_gem_object_del,
	.open = nouveau_gem_object_open,
	.close = nouveau_gem_object_close,
	.export = nouveau_gem_prime_export,
	.pin = nouveau_gem_prime_pin,
	.unpin = nouveau_gem_prime_unpin,
	.get_sg_table = nouveau_gem_prime_get_sg_table,
	.vmap = drm_gem_ttm_vmap,
	.vunmap = drm_gem_ttm_vunmap,
	.mmap = drm_gem_ttm_mmap,
	.vm_ops = &nouveau_ttm_vm_ops,
};

int
nouveau_gem_new(struct nouveau_cli *cli, u64 size, int align, uint32_t domain,
		uint32_t tile_mode, uint32_t tile_flags,
		struct nouveau_bo **pnvbo)
{
	struct nouveau_drm *drm = cli->drm;
	struct nouveau_uvmm *uvmm = nouveau_cli_uvmm(cli);
	struct dma_resv *resv = NULL;
	struct nouveau_bo *nvbo;
	int ret;

	if (domain & NOUVEAU_GEM_DOMAIN_NO_SHARE) {
		if (unlikely(!uvmm))
			return -EINVAL;

		resv = drm_gpuvm_resv(&uvmm->base);
	}

	if (!(domain & (NOUVEAU_GEM_DOMAIN_VRAM | NOUVEAU_GEM_DOMAIN_GART)))
		domain |= NOUVEAU_GEM_DOMAIN_CPU;

	nvbo = nouveau_bo_alloc(cli, &size, &align, domain, tile_mode,
				tile_flags, false);
	if (IS_ERR(nvbo))
		return PTR_ERR(nvbo);

	nvbo->bo.base.funcs = &nouveau_gem_object_funcs;
	nvbo->no_share = domain & NOUVEAU_GEM_DOMAIN_NO_SHARE;

	/* Initialize the embedded gem-object. We return a single gem-reference
	 * to the caller, instead of a normal nouveau_bo ttm reference. */
	ret = drm_gem_object_init(drm->dev, &nvbo->bo.base, size);
	if (ret) {
		drm_gem_object_release(&nvbo->bo.base);
		kfree(nvbo);
		return ret;
	}

	if (resv)
		dma_resv_lock(resv, NULL);

	ret = nouveau_bo_init(nvbo, size, align, domain, NULL, resv);

	if (resv)
		dma_resv_unlock(resv);

	if (ret)
		return ret;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	/* we restrict allowed domains on nv50+ to only the types
	 * that were requested at creation time.  not possibly on
	 * earlier chips without busting the ABI.
	 */
	nvbo->valid_domains = NOUVEAU_GEM_DOMAIN_VRAM |
			      NOUVEAU_GEM_DOMAIN_GART;
<<<<<<< HEAD
	if (dev_priv->card_type >= NV_50)
		nvbo->valid_domains &= domain;

	nvbo->gem = drm_gem_object_alloc(dev, nvbo->bo.mem.size);
	if (!nvbo->gem) {
		nouveau_bo_ref(NULL, pnvbo);
		return -ENOMEM;
	}

	nvbo->bo.persistent_swap_storage = nvbo->gem->filp;
	nvbo->gem->driver_private = nvbo;
=======
	if (drm->client.device.info.family >= NV_DEVICE_INFO_V0_TESLA)
		nvbo->valid_domains &= domain;

	if (nvbo->no_share) {
		nvbo->r_obj = drm_gpuvm_resv_obj(&uvmm->base);
		drm_gem_object_get(nvbo->r_obj);
	}

	*pnvbo = nvbo;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	return 0;
}

static int
nouveau_gem_info(struct drm_file *file_priv, struct drm_gem_object *gem,
		 struct drm_nouveau_gem_info *rep)
{
<<<<<<< HEAD
	struct nouveau_fpriv *fpriv = nouveau_fpriv(file_priv);
	struct nouveau_bo *nvbo = nouveau_gem_object(gem);
=======
	struct nouveau_cli *cli = nouveau_cli(file_priv);
	struct nouveau_bo *nvbo = nouveau_gem_object(gem);
	struct nouveau_vmm *vmm = nouveau_cli_vmm(cli);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	struct nouveau_vma *vma;

	if (is_power_of_2(nvbo->valid_domains))
		rep->domain = nvbo->valid_domains;
<<<<<<< HEAD
	else if (nvbo->bo.mem.mem_type == TTM_PL_TT)
		rep->domain = NOUVEAU_GEM_DOMAIN_GART;
	else
		rep->domain = NOUVEAU_GEM_DOMAIN_VRAM;
	rep->offset = nvbo->bo.offset;
	if (fpriv->vm) {
		vma = nouveau_bo_vma_find(nvbo, fpriv->vm);
		if (!vma)
			return -EINVAL;

		rep->offset = vma->offset;
	}

	rep->size = nvbo->bo.mem.num_pages << PAGE_SHIFT;
	rep->map_handle = nvbo->bo.addr_space_offset;
	rep->tile_mode = nvbo->tile_mode;
	rep->tile_flags = nvbo->tile_flags;
=======
	else if (nvbo->bo.resource->mem_type == TTM_PL_TT)
		rep->domain = NOUVEAU_GEM_DOMAIN_GART;
	else
		rep->domain = NOUVEAU_GEM_DOMAIN_VRAM;
	rep->offset = nvbo->offset;
	if (vmm->vmm.object.oclass >= NVIF_CLASS_VMM_NV50 &&
	    !nouveau_cli_uvmm(cli)) {
		vma = nouveau_vma_find(nvbo, vmm);
		if (!vma)
			return -EINVAL;

		rep->offset = vma->addr;
	} else
		rep->offset = 0;

	rep->size = nvbo->bo.base.size;
	rep->map_handle = drm_vma_node_offset_addr(&nvbo->bo.base.vma_node);
	rep->tile_mode = nvbo->mode;
	rep->tile_flags = nvbo->contig ? 0 : NOUVEAU_GEM_TILE_NONCONTIG;
	if (cli->device.info.family >= NV_DEVICE_INFO_V0_FERMI)
		rep->tile_flags |= nvbo->kind << 8;
	else
	if (cli->device.info.family >= NV_DEVICE_INFO_V0_TESLA)
		rep->tile_flags |= nvbo->kind << 8 | nvbo->comp << 16;
	else
		rep->tile_flags |= nvbo->zeta;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	return 0;
}

int
nouveau_gem_ioctl_new(struct drm_device *dev, void *data,
		      struct drm_file *file_priv)
{
<<<<<<< HEAD
	struct drm_nouveau_private *dev_priv = dev->dev_private;
=======
	struct nouveau_cli *cli = nouveau_cli(file_priv);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	struct drm_nouveau_gem_new *req = data;
	struct nouveau_bo *nvbo = NULL;
	int ret = 0;

<<<<<<< HEAD
	if (unlikely(dev_priv->ttm.bdev.dev_mapping == NULL))
		dev_priv->ttm.bdev.dev_mapping = dev_priv->dev->dev_mapping;

	if (!dev_priv->engine.vram.flags_valid(dev, req->info.tile_flags)) {
		NV_ERROR(dev, "bad page flags: 0x%08x\n", req->info.tile_flags);
		return -EINVAL;
	}

	ret = nouveau_gem_new(dev, req->info.size, req->align,
=======
	/* If uvmm wasn't initialized until now disable it completely to prevent
	 * userspace from mixing up UAPIs.
	 */
	nouveau_cli_disable_uvmm_noinit(cli);

	ret = nouveau_gem_new(cli, req->info.size, req->align,
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
			      req->info.domain, req->info.tile_mode,
			      req->info.tile_flags, &nvbo);
	if (ret)
		return ret;

<<<<<<< HEAD
	ret = drm_gem_handle_create(file_priv, nvbo->gem, &req->info.handle);
	if (ret == 0) {
		ret = nouveau_gem_info(file_priv, nvbo->gem, &req->info);
=======
	ret = drm_gem_handle_create(file_priv, &nvbo->bo.base,
				    &req->info.handle);
	if (ret == 0) {
		ret = nouveau_gem_info(file_priv, &nvbo->bo.base, &req->info);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		if (ret)
			drm_gem_handle_delete(file_priv, req->info.handle);
	}

	/* drop reference from allocate - handle holds it now */
<<<<<<< HEAD
	drm_gem_object_unreference_unlocked(nvbo->gem);
=======
	drm_gem_object_put(&nvbo->bo.base);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	return ret;
}

static int
nouveau_gem_set_domain(struct drm_gem_object *gem, uint32_t read_domains,
		       uint32_t write_domains, uint32_t valid_domains)
{
<<<<<<< HEAD
	struct nouveau_bo *nvbo = gem->driver_private;
	struct ttm_buffer_object *bo = &nvbo->bo;
	uint32_t domains = valid_domains & nvbo->valid_domains &
		(write_domains ? write_domains : read_domains);
	uint32_t pref_flags = 0, valid_flags = 0;
=======
	struct nouveau_bo *nvbo = nouveau_gem_object(gem);
	struct ttm_buffer_object *bo = &nvbo->bo;
	uint32_t domains = valid_domains & nvbo->valid_domains &
		(write_domains ? write_domains : read_domains);
	uint32_t pref_domains = 0;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	if (!domains)
		return -EINVAL;

<<<<<<< HEAD
	if (valid_domains & NOUVEAU_GEM_DOMAIN_VRAM)
		valid_flags |= TTM_PL_FLAG_VRAM;

	if (valid_domains & NOUVEAU_GEM_DOMAIN_GART)
		valid_flags |= TTM_PL_FLAG_TT;

	if ((domains & NOUVEAU_GEM_DOMAIN_VRAM) &&
	    bo->mem.mem_type == TTM_PL_VRAM)
		pref_flags |= TTM_PL_FLAG_VRAM;

	else if ((domains & NOUVEAU_GEM_DOMAIN_GART) &&
		 bo->mem.mem_type == TTM_PL_TT)
		pref_flags |= TTM_PL_FLAG_TT;

	else if (domains & NOUVEAU_GEM_DOMAIN_VRAM)
		pref_flags |= TTM_PL_FLAG_VRAM;

	else
		pref_flags |= TTM_PL_FLAG_TT;

	nouveau_bo_placement_set(nvbo, pref_flags, valid_flags);
=======
	valid_domains &= ~(NOUVEAU_GEM_DOMAIN_VRAM | NOUVEAU_GEM_DOMAIN_GART);

	if ((domains & NOUVEAU_GEM_DOMAIN_VRAM) &&
	    bo->resource->mem_type == TTM_PL_VRAM)
		pref_domains |= NOUVEAU_GEM_DOMAIN_VRAM;

	else if ((domains & NOUVEAU_GEM_DOMAIN_GART) &&
		 bo->resource->mem_type == TTM_PL_TT)
		pref_domains |= NOUVEAU_GEM_DOMAIN_GART;

	else if (domains & NOUVEAU_GEM_DOMAIN_VRAM)
		pref_domains |= NOUVEAU_GEM_DOMAIN_VRAM;

	else
		pref_domains |= NOUVEAU_GEM_DOMAIN_GART;

	nouveau_bo_placement_set(nvbo, pref_domains, valid_domains);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	return 0;
}

struct validate_op {
<<<<<<< HEAD
	struct list_head vram_list;
	struct list_head gart_list;
	struct list_head both_list;
};

static void
validate_fini_list(struct list_head *list, struct nouveau_fence *fence)
{
	struct list_head *entry, *tmp;
	struct nouveau_bo *nvbo;

	list_for_each_safe(entry, tmp, list) {
		nvbo = list_entry(entry, struct nouveau_bo, entry);

		if (likely(fence))
			nouveau_bo_fence(nvbo, fence);
=======
	struct list_head list;
	struct ww_acquire_ctx ticket;
};

static void
validate_fini_no_ticket(struct validate_op *op, struct nouveau_channel *chan,
			struct nouveau_fence *fence,
			struct drm_nouveau_gem_pushbuf_bo *pbbo)
{
	struct nouveau_bo *nvbo;
	struct drm_nouveau_gem_pushbuf_bo *b;

	while (!list_empty(&op->list)) {
		nvbo = list_entry(op->list.next, struct nouveau_bo, entry);
		b = &pbbo[nvbo->pbbo_index];

		if (likely(fence)) {
			nouveau_bo_fence(nvbo, fence, !!b->write_domains);

			if (chan->vmm->vmm.object.oclass >= NVIF_CLASS_VMM_NV50) {
				struct nouveau_vma *vma =
					(void *)(unsigned long)b->user_priv;
				nouveau_fence_unref(&vma->fence);
				dma_fence_get(&fence->base);
				vma->fence = fence;
			}
		}
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

		if (unlikely(nvbo->validate_mapped)) {
			ttm_bo_kunmap(&nvbo->kmap);
			nvbo->validate_mapped = false;
		}

		list_del(&nvbo->entry);
		nvbo->reserved_by = NULL;
		ttm_bo_unreserve(&nvbo->bo);
<<<<<<< HEAD
		drm_gem_object_unreference_unlocked(nvbo->gem);
=======
		drm_gem_object_put(&nvbo->bo.base);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	}
}

static void
<<<<<<< HEAD
validate_fini(struct validate_op *op, struct nouveau_fence* fence)
{
	validate_fini_list(&op->vram_list, fence);
	validate_fini_list(&op->gart_list, fence);
	validate_fini_list(&op->both_list, fence);
=======
validate_fini(struct validate_op *op, struct nouveau_channel *chan,
	      struct nouveau_fence *fence,
	      struct drm_nouveau_gem_pushbuf_bo *pbbo)
{
	validate_fini_no_ticket(op, chan, fence, pbbo);
	ww_acquire_fini(&op->ticket);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

static int
validate_init(struct nouveau_channel *chan, struct drm_file *file_priv,
	      struct drm_nouveau_gem_pushbuf_bo *pbbo,
	      int nr_buffers, struct validate_op *op)
{
<<<<<<< HEAD
	struct drm_device *dev = chan->dev;
	struct drm_nouveau_private *dev_priv = dev->dev_private;
	uint32_t sequence;
	int trycnt = 0;
	int ret, i;

	sequence = atomic_add_return(1, &dev_priv->ttm.validate_sequence);
retry:
	if (++trycnt > 100000) {
		NV_ERROR(dev, "%s failed and gave up.\n", __func__);
=======
	struct nouveau_cli *cli = nouveau_cli(file_priv);
	int trycnt = 0;
	int ret = -EINVAL, i;
	struct nouveau_bo *res_bo = NULL;
	LIST_HEAD(gart_list);
	LIST_HEAD(vram_list);
	LIST_HEAD(both_list);

	ww_acquire_init(&op->ticket, &reservation_ww_class);
retry:
	if (++trycnt > 100000) {
		NV_PRINTK(err, cli, "%s failed and gave up.\n", __func__);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		return -EINVAL;
	}

	for (i = 0; i < nr_buffers; i++) {
		struct drm_nouveau_gem_pushbuf_bo *b = &pbbo[i];
		struct drm_gem_object *gem;
		struct nouveau_bo *nvbo;

<<<<<<< HEAD
		gem = drm_gem_object_lookup(dev, file_priv, b->handle);
		if (!gem) {
			NV_ERROR(dev, "Unknown handle 0x%08x\n", b->handle);
			validate_fini(op, NULL);
			return -ENOENT;
		}
		nvbo = gem->driver_private;

		if (nvbo->reserved_by && nvbo->reserved_by == file_priv) {
			NV_ERROR(dev, "multiple instances of buffer %d on "
				      "validation list\n", b->handle);
			validate_fini(op, NULL);
			return -EINVAL;
		}

		ret = ttm_bo_reserve(&nvbo->bo, true, false, true, sequence);
		if (ret) {
			validate_fini(op, NULL);
			if (unlikely(ret == -EAGAIN))
				ret = ttm_bo_wait_unreserved(&nvbo->bo, true);
			drm_gem_object_unreference_unlocked(gem);
			if (unlikely(ret)) {
				if (ret != -ERESTARTSYS)
					NV_ERROR(dev, "fail reserve\n");
				return ret;
			}
			goto retry;
		}

		b->user_priv = (uint64_t)(unsigned long)nvbo;
=======
		gem = drm_gem_object_lookup(file_priv, b->handle);
		if (!gem) {
			NV_PRINTK(err, cli, "Unknown handle 0x%08x\n", b->handle);
			ret = -ENOENT;
			break;
		}
		nvbo = nouveau_gem_object(gem);
		if (nvbo == res_bo) {
			res_bo = NULL;
			drm_gem_object_put(gem);
			continue;
		}

		if (nvbo->reserved_by && nvbo->reserved_by == file_priv) {
			NV_PRINTK(err, cli, "multiple instances of buffer %d on "
				      "validation list\n", b->handle);
			drm_gem_object_put(gem);
			ret = -EINVAL;
			break;
		}

		ret = ttm_bo_reserve(&nvbo->bo, true, false, &op->ticket);
		if (ret) {
			list_splice_tail_init(&vram_list, &op->list);
			list_splice_tail_init(&gart_list, &op->list);
			list_splice_tail_init(&both_list, &op->list);
			validate_fini_no_ticket(op, chan, NULL, NULL);
			if (unlikely(ret == -EDEADLK)) {
				ret = ttm_bo_reserve_slowpath(&nvbo->bo, true,
							      &op->ticket);
				if (!ret)
					res_bo = nvbo;
			}
			if (unlikely(ret)) {
				if (ret != -ERESTARTSYS)
					NV_PRINTK(err, cli, "fail reserve\n");
				break;
			}
		}

		if (chan->vmm->vmm.object.oclass >= NVIF_CLASS_VMM_NV50) {
			struct nouveau_vmm *vmm = chan->vmm;
			struct nouveau_vma *vma = nouveau_vma_find(nvbo, vmm);
			if (!vma) {
				NV_PRINTK(err, cli, "vma not found!\n");
				ret = -EINVAL;
				break;
			}

			b->user_priv = (uint64_t)(unsigned long)vma;
		} else {
			b->user_priv = (uint64_t)(unsigned long)nvbo;
		}

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		nvbo->reserved_by = file_priv;
		nvbo->pbbo_index = i;
		if ((b->valid_domains & NOUVEAU_GEM_DOMAIN_VRAM) &&
		    (b->valid_domains & NOUVEAU_GEM_DOMAIN_GART))
<<<<<<< HEAD
			list_add_tail(&nvbo->entry, &op->both_list);
		else
		if (b->valid_domains & NOUVEAU_GEM_DOMAIN_VRAM)
			list_add_tail(&nvbo->entry, &op->vram_list);
		else
		if (b->valid_domains & NOUVEAU_GEM_DOMAIN_GART)
			list_add_tail(&nvbo->entry, &op->gart_list);
		else {
			NV_ERROR(dev, "invalid valid domains: 0x%08x\n",
				 b->valid_domains);
			list_add_tail(&nvbo->entry, &op->both_list);
			validate_fini(op, NULL);
			return -EINVAL;
		}
	}

	return 0;
}

static int
validate_sync(struct nouveau_channel *chan, struct nouveau_bo *nvbo)
{
	struct nouveau_fence *fence = NULL;
	int ret = 0;

	spin_lock(&nvbo->bo.bdev->fence_lock);
	if (nvbo->bo.sync_obj)
		fence = nouveau_fence_ref(nvbo->bo.sync_obj);
	spin_unlock(&nvbo->bo.bdev->fence_lock);

	if (fence) {
		ret = nouveau_fence_sync(fence, chan);
		nouveau_fence_unref(&fence);
	}

	return ret;
}

static int
validate_list(struct nouveau_channel *chan, struct list_head *list,
	      struct drm_nouveau_gem_pushbuf_bo *pbbo, uint64_t user_pbbo_ptr)
{
	struct drm_nouveau_private *dev_priv = chan->dev->dev_private;
	struct drm_nouveau_gem_pushbuf_bo __user *upbbo =
				(void __force __user *)(uintptr_t)user_pbbo_ptr;
	struct drm_device *dev = chan->dev;
=======
			list_add_tail(&nvbo->entry, &both_list);
		else
		if (b->valid_domains & NOUVEAU_GEM_DOMAIN_VRAM)
			list_add_tail(&nvbo->entry, &vram_list);
		else
		if (b->valid_domains & NOUVEAU_GEM_DOMAIN_GART)
			list_add_tail(&nvbo->entry, &gart_list);
		else {
			NV_PRINTK(err, cli, "invalid valid domains: 0x%08x\n",
				 b->valid_domains);
			list_add_tail(&nvbo->entry, &both_list);
			ret = -EINVAL;
			break;
		}
		if (nvbo == res_bo)
			goto retry;
	}

	ww_acquire_done(&op->ticket);
	list_splice_tail(&vram_list, &op->list);
	list_splice_tail(&gart_list, &op->list);
	list_splice_tail(&both_list, &op->list);
	if (ret)
		validate_fini(op, chan, NULL, NULL);
	return ret;

}

static int
validate_list(struct nouveau_channel *chan, struct nouveau_cli *cli,
	      struct list_head *list, struct drm_nouveau_gem_pushbuf_bo *pbbo)
{
	struct nouveau_drm *drm = chan->drm;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	struct nouveau_bo *nvbo;
	int ret, relocs = 0;

	list_for_each_entry(nvbo, list, entry) {
		struct drm_nouveau_gem_pushbuf_bo *b = &pbbo[nvbo->pbbo_index];

<<<<<<< HEAD
		ret = validate_sync(chan, nvbo);
		if (unlikely(ret)) {
			NV_ERROR(dev, "fail pre-validate sync\n");
			return ret;
		}

		ret = nouveau_gem_set_domain(nvbo->gem, b->read_domains,
					     b->write_domains,
					     b->valid_domains);
		if (unlikely(ret)) {
			NV_ERROR(dev, "fail set_domain\n");
			return ret;
		}

		ret = nouveau_bo_validate(nvbo, true, false, false);
		if (unlikely(ret)) {
			if (ret != -ERESTARTSYS)
				NV_ERROR(dev, "fail ttm_validate\n");
			return ret;
		}

		ret = validate_sync(chan, nvbo);
		if (unlikely(ret)) {
			NV_ERROR(dev, "fail post-validate sync\n");
			return ret;
		}

		if (dev_priv->card_type < NV_50) {
			if (nvbo->bo.offset == b->presumed.offset &&
			    ((nvbo->bo.mem.mem_type == TTM_PL_VRAM &&
			      b->presumed.domain & NOUVEAU_GEM_DOMAIN_VRAM) ||
			     (nvbo->bo.mem.mem_type == TTM_PL_TT &&
			      b->presumed.domain & NOUVEAU_GEM_DOMAIN_GART)))
				continue;

			if (nvbo->bo.mem.mem_type == TTM_PL_TT)
				b->presumed.domain = NOUVEAU_GEM_DOMAIN_GART;
			else
				b->presumed.domain = NOUVEAU_GEM_DOMAIN_VRAM;
			b->presumed.offset = nvbo->bo.offset;
			b->presumed.valid = 0;
			relocs++;

			if (DRM_COPY_TO_USER(&upbbo[nvbo->pbbo_index].presumed,
					     &b->presumed, sizeof(b->presumed)))
				return -EFAULT;
=======
		ret = nouveau_gem_set_domain(&nvbo->bo.base, b->read_domains,
					     b->write_domains,
					     b->valid_domains);
		if (unlikely(ret)) {
			NV_PRINTK(err, cli, "fail set_domain\n");
			return ret;
		}

		ret = nouveau_bo_validate(nvbo, true, false);
		if (unlikely(ret)) {
			if (ret != -ERESTARTSYS)
				NV_PRINTK(err, cli, "fail ttm_validate\n");
			return ret;
		}

		ret = nouveau_fence_sync(nvbo, chan, !!b->write_domains, true);
		if (unlikely(ret)) {
			if (ret != -ERESTARTSYS)
				NV_PRINTK(err, cli, "fail post-validate sync\n");
			return ret;
		}

		if (drm->client.device.info.family < NV_DEVICE_INFO_V0_TESLA) {
			if (nvbo->offset == b->presumed.offset &&
			    ((nvbo->bo.resource->mem_type == TTM_PL_VRAM &&
			      b->presumed.domain & NOUVEAU_GEM_DOMAIN_VRAM) ||
			     (nvbo->bo.resource->mem_type == TTM_PL_TT &&
			      b->presumed.domain & NOUVEAU_GEM_DOMAIN_GART)))
				continue;

			if (nvbo->bo.resource->mem_type == TTM_PL_TT)
				b->presumed.domain = NOUVEAU_GEM_DOMAIN_GART;
			else
				b->presumed.domain = NOUVEAU_GEM_DOMAIN_VRAM;
			b->presumed.offset = nvbo->offset;
			b->presumed.valid = 0;
			relocs++;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		}
	}

	return relocs;
}

static int
nouveau_gem_pushbuf_validate(struct nouveau_channel *chan,
			     struct drm_file *file_priv,
			     struct drm_nouveau_gem_pushbuf_bo *pbbo,
<<<<<<< HEAD
			     uint64_t user_buffers, int nr_buffers,
			     struct validate_op *op, int *apply_relocs)
{
	struct drm_device *dev = chan->dev;
	int ret, relocs = 0;

	INIT_LIST_HEAD(&op->vram_list);
	INIT_LIST_HEAD(&op->gart_list);
	INIT_LIST_HEAD(&op->both_list);
=======
			     int nr_buffers,
			     struct validate_op *op, bool *apply_relocs)
{
	struct nouveau_cli *cli = nouveau_cli(file_priv);
	int ret;

	INIT_LIST_HEAD(&op->list);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	if (nr_buffers == 0)
		return 0;

	ret = validate_init(chan, file_priv, pbbo, nr_buffers, op);
	if (unlikely(ret)) {
		if (ret != -ERESTARTSYS)
<<<<<<< HEAD
			NV_ERROR(dev, "validate_init\n");
		return ret;
	}

	ret = validate_list(chan, &op->vram_list, pbbo, user_buffers);
	if (unlikely(ret < 0)) {
		if (ret != -ERESTARTSYS)
			NV_ERROR(dev, "validate vram_list\n");
		validate_fini(op, NULL);
		return ret;
	}
	relocs += ret;

	ret = validate_list(chan, &op->gart_list, pbbo, user_buffers);
	if (unlikely(ret < 0)) {
		if (ret != -ERESTARTSYS)
			NV_ERROR(dev, "validate gart_list\n");
		validate_fini(op, NULL);
		return ret;
	}
	relocs += ret;

	ret = validate_list(chan, &op->both_list, pbbo, user_buffers);
	if (unlikely(ret < 0)) {
		if (ret != -ERESTARTSYS)
			NV_ERROR(dev, "validate both_list\n");
		validate_fini(op, NULL);
		return ret;
	}
	relocs += ret;

	*apply_relocs = relocs;
	return 0;
}

static inline void *
u_memcpya(uint64_t user, unsigned nmemb, unsigned size)
{
	void *mem;
	void __user *userptr = (void __force __user *)(uintptr_t)user;

	mem = kmalloc(nmemb * size, GFP_KERNEL);
	if (!mem)
		return ERR_PTR(-ENOMEM);

	if (DRM_COPY_FROM_USER(mem, userptr, nmemb * size)) {
		kfree(mem);
		return ERR_PTR(-EFAULT);
	}

	return mem;
}

static int
nouveau_gem_pushbuf_reloc_apply(struct drm_device *dev,
				struct drm_nouveau_gem_pushbuf *req,
				struct drm_nouveau_gem_pushbuf_bo *bo)
{
	struct drm_nouveau_gem_pushbuf_reloc *reloc = NULL;
	int ret = 0;
	unsigned i;

	reloc = u_memcpya(req->relocs, req->nr_relocs, sizeof(*reloc));
	if (IS_ERR(reloc))
		return PTR_ERR(reloc);

=======
			NV_PRINTK(err, cli, "validate_init\n");
		return ret;
	}

	ret = validate_list(chan, cli, &op->list, pbbo);
	if (unlikely(ret < 0)) {
		if (ret != -ERESTARTSYS)
			NV_PRINTK(err, cli, "validating bo list\n");
		validate_fini(op, chan, NULL, NULL);
		return ret;
	} else if (ret > 0) {
		*apply_relocs = true;
	}

	return 0;
}

static int
nouveau_gem_pushbuf_reloc_apply(struct nouveau_cli *cli,
				struct drm_nouveau_gem_pushbuf *req,
				struct drm_nouveau_gem_pushbuf_reloc *reloc,
				struct drm_nouveau_gem_pushbuf_bo *bo)
{
	int ret = 0;
	unsigned i;

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	for (i = 0; i < req->nr_relocs; i++) {
		struct drm_nouveau_gem_pushbuf_reloc *r = &reloc[i];
		struct drm_nouveau_gem_pushbuf_bo *b;
		struct nouveau_bo *nvbo;
		uint32_t data;
<<<<<<< HEAD

		if (unlikely(r->bo_index > req->nr_buffers)) {
			NV_ERROR(dev, "reloc bo index invalid\n");
=======
		long lret;

		if (unlikely(r->bo_index >= req->nr_buffers)) {
			NV_PRINTK(err, cli, "reloc bo index invalid\n");
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
			ret = -EINVAL;
			break;
		}

		b = &bo[r->bo_index];
		if (b->presumed.valid)
			continue;

<<<<<<< HEAD
		if (unlikely(r->reloc_bo_index > req->nr_buffers)) {
			NV_ERROR(dev, "reloc container bo index invalid\n");
=======
		if (unlikely(r->reloc_bo_index >= req->nr_buffers)) {
			NV_PRINTK(err, cli, "reloc container bo index invalid\n");
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
			ret = -EINVAL;
			break;
		}
		nvbo = (void *)(unsigned long)bo[r->reloc_bo_index].user_priv;

		if (unlikely(r->reloc_bo_offset + 4 >
<<<<<<< HEAD
			     nvbo->bo.mem.num_pages << PAGE_SHIFT)) {
			NV_ERROR(dev, "reloc outside of bo\n");
=======
			     nvbo->bo.base.size)) {
			NV_PRINTK(err, cli, "reloc outside of bo\n");
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
			ret = -EINVAL;
			break;
		}

		if (!nvbo->kmap.virtual) {
<<<<<<< HEAD
			ret = ttm_bo_kmap(&nvbo->bo, 0, nvbo->bo.mem.num_pages,
					  &nvbo->kmap);
			if (ret) {
				NV_ERROR(dev, "failed kmap for reloc\n");
=======
			ret = ttm_bo_kmap(&nvbo->bo, 0, PFN_UP(nvbo->bo.base.size),
					  &nvbo->kmap);
			if (ret) {
				NV_PRINTK(err, cli, "failed kmap for reloc\n");
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
				break;
			}
			nvbo->validate_mapped = true;
		}

		if (r->flags & NOUVEAU_GEM_RELOC_LOW)
			data = b->presumed.offset + r->data;
		else
		if (r->flags & NOUVEAU_GEM_RELOC_HIGH)
			data = (b->presumed.offset + r->data) >> 32;
		else
			data = r->data;

		if (r->flags & NOUVEAU_GEM_RELOC_OR) {
			if (b->presumed.domain == NOUVEAU_GEM_DOMAIN_GART)
				data |= r->tor;
			else
				data |= r->vor;
		}

<<<<<<< HEAD
		spin_lock(&nvbo->bo.bdev->fence_lock);
		ret = ttm_bo_wait(&nvbo->bo, false, false, false);
		spin_unlock(&nvbo->bo.bdev->fence_lock);
		if (ret) {
			NV_ERROR(dev, "reloc wait_idle failed: %d\n", ret);
=======
		lret = dma_resv_wait_timeout(nvbo->bo.base.resv,
					     DMA_RESV_USAGE_BOOKKEEP,
					     false, 15 * HZ);
		if (!lret)
			ret = -EBUSY;
		else if (lret > 0)
			ret = 0;
		else
			ret = lret;

		if (ret) {
			NV_PRINTK(err, cli, "reloc wait_idle failed: %d\n",
				  ret);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
			break;
		}

		nouveau_bo_wr32(nvbo, r->reloc_bo_offset >> 2, data);
	}

<<<<<<< HEAD
	kfree(reloc);
=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	return ret;
}

int
nouveau_gem_ioctl_pushbuf(struct drm_device *dev, void *data,
			  struct drm_file *file_priv)
{
<<<<<<< HEAD
	struct drm_nouveau_private *dev_priv = dev->dev_private;
	struct drm_nouveau_gem_pushbuf *req = data;
	struct drm_nouveau_gem_pushbuf_push *push;
	struct drm_nouveau_gem_pushbuf_bo *bo;
	struct nouveau_channel *chan;
	struct validate_op op;
	struct nouveau_fence *fence = NULL;
	int i, j, ret = 0, do_reloc = 0;

	chan = nouveau_channel_get(file_priv, req->channel);
	if (IS_ERR(chan))
		return PTR_ERR(chan);

	req->vram_available = dev_priv->fb_aper_free;
	req->gart_available = dev_priv->gart_info.aper_free;
=======
	struct nouveau_abi16 *abi16 = nouveau_abi16_get(file_priv);
	struct nouveau_cli *cli = nouveau_cli(file_priv);
	struct nouveau_abi16_chan *temp;
	struct nouveau_drm *drm = nouveau_drm(dev);
	struct drm_nouveau_gem_pushbuf *req = data;
	struct drm_nouveau_gem_pushbuf_push *push;
	struct drm_nouveau_gem_pushbuf_reloc *reloc = NULL;
	struct drm_nouveau_gem_pushbuf_bo *bo;
	struct nouveau_channel *chan = NULL;
	struct validate_op op;
	struct nouveau_fence *fence = NULL;
	int i, j, ret = 0;
	bool do_reloc = false, sync = false;

	if (unlikely(!abi16))
		return -ENOMEM;

	if (unlikely(nouveau_cli_uvmm(cli)))
		return nouveau_abi16_put(abi16, -ENOSYS);

	list_for_each_entry(temp, &abi16->channels, head) {
		if (temp->chan->chid == req->channel) {
			chan = temp->chan;
			break;
		}
	}

	if (!chan)
		return nouveau_abi16_put(abi16, -ENOENT);
	if (unlikely(atomic_read(&chan->killed)))
		return nouveau_abi16_put(abi16, -ENODEV);

	sync = req->vram_available & NOUVEAU_GEM_PUSHBUF_SYNC;

	req->vram_available = drm->gem.vram_available;
	req->gart_available = drm->gem.gart_available;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	if (unlikely(req->nr_push == 0))
		goto out_next;

	if (unlikely(req->nr_push > NOUVEAU_GEM_MAX_PUSH)) {
<<<<<<< HEAD
		NV_ERROR(dev, "pushbuf push count exceeds limit: %d max %d\n",
			 req->nr_push, NOUVEAU_GEM_MAX_PUSH);
		nouveau_channel_put(&chan);
		return -EINVAL;
	}

	if (unlikely(req->nr_buffers > NOUVEAU_GEM_MAX_BUFFERS)) {
		NV_ERROR(dev, "pushbuf bo count exceeds limit: %d max %d\n",
			 req->nr_buffers, NOUVEAU_GEM_MAX_BUFFERS);
		nouveau_channel_put(&chan);
		return -EINVAL;
	}

	if (unlikely(req->nr_relocs > NOUVEAU_GEM_MAX_RELOCS)) {
		NV_ERROR(dev, "pushbuf reloc count exceeds limit: %d max %d\n",
			 req->nr_relocs, NOUVEAU_GEM_MAX_RELOCS);
		nouveau_channel_put(&chan);
		return -EINVAL;
	}

	push = u_memcpya(req->push, req->nr_push, sizeof(*push));
	if (IS_ERR(push)) {
		nouveau_channel_put(&chan);
		return PTR_ERR(push);
	}

	bo = u_memcpya(req->buffers, req->nr_buffers, sizeof(*bo));
	if (IS_ERR(bo)) {
		kfree(push);
		nouveau_channel_put(&chan);
		return PTR_ERR(bo);
=======
		NV_PRINTK(err, cli, "pushbuf push count exceeds limit: %d max %d\n",
			 req->nr_push, NOUVEAU_GEM_MAX_PUSH);
		return nouveau_abi16_put(abi16, -EINVAL);
	}

	if (unlikely(req->nr_buffers > NOUVEAU_GEM_MAX_BUFFERS)) {
		NV_PRINTK(err, cli, "pushbuf bo count exceeds limit: %d max %d\n",
			 req->nr_buffers, NOUVEAU_GEM_MAX_BUFFERS);
		return nouveau_abi16_put(abi16, -EINVAL);
	}

	if (unlikely(req->nr_relocs > NOUVEAU_GEM_MAX_RELOCS)) {
		NV_PRINTK(err, cli, "pushbuf reloc count exceeds limit: %d max %d\n",
			 req->nr_relocs, NOUVEAU_GEM_MAX_RELOCS);
		return nouveau_abi16_put(abi16, -EINVAL);
	}

	push = u_memcpya(req->push, req->nr_push, sizeof(*push));
	if (IS_ERR(push))
		return nouveau_abi16_put(abi16, PTR_ERR(push));

	bo = u_memcpya(req->buffers, req->nr_buffers, sizeof(*bo));
	if (IS_ERR(bo)) {
		u_free(push);
		return nouveau_abi16_put(abi16, PTR_ERR(bo));
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	}

	/* Ensure all push buffers are on validate list */
	for (i = 0; i < req->nr_push; i++) {
		if (push[i].bo_index >= req->nr_buffers) {
<<<<<<< HEAD
			NV_ERROR(dev, "push %d buffer not in list\n", i);
=======
			NV_PRINTK(err, cli, "push %d buffer not in list\n", i);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
			ret = -EINVAL;
			goto out_prevalid;
		}
	}

	/* Validate buffer list */
<<<<<<< HEAD
	ret = nouveau_gem_pushbuf_validate(chan, file_priv, bo, req->buffers,
					   req->nr_buffers, &op, &do_reloc);
	if (ret) {
		if (ret != -ERESTARTSYS)
			NV_ERROR(dev, "validate: %d\n", ret);
=======
revalidate:
	ret = nouveau_gem_pushbuf_validate(chan, file_priv, bo,
					   req->nr_buffers, &op, &do_reloc);
	if (ret) {
		if (ret != -ERESTARTSYS)
			NV_PRINTK(err, cli, "validate: %d\n", ret);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		goto out_prevalid;
	}

	/* Apply any relocations that are required */
	if (do_reloc) {
<<<<<<< HEAD
		ret = nouveau_gem_pushbuf_reloc_apply(dev, req, bo);
		if (ret) {
			NV_ERROR(dev, "reloc apply: %d\n", ret);
=======
		if (!reloc) {
			validate_fini(&op, chan, NULL, bo);
			reloc = u_memcpya(req->relocs, req->nr_relocs, sizeof(*reloc));
			if (IS_ERR(reloc)) {
				ret = PTR_ERR(reloc);
				goto out_prevalid;
			}

			goto revalidate;
		}

		ret = nouveau_gem_pushbuf_reloc_apply(cli, req, reloc, bo);
		if (ret) {
			NV_PRINTK(err, cli, "reloc apply: %d\n", ret);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
			goto out;
		}
	}

	if (chan->dma.ib_max) {
<<<<<<< HEAD
		ret = nouveau_dma_wait(chan, req->nr_push + 1, 6);
		if (ret) {
			NV_INFO(dev, "nv50cal_space: %d\n", ret);
=======
		ret = nouveau_dma_wait(chan, req->nr_push + 1, 16);
		if (ret) {
			NV_PRINTK(err, cli, "nv50cal_space: %d\n", ret);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
			goto out;
		}

		for (i = 0; i < req->nr_push; i++) {
<<<<<<< HEAD
			struct nouveau_bo *nvbo = (void *)(unsigned long)
				bo[push[i].bo_index].user_priv;

			nv50_dma_push(chan, nvbo, push[i].offset,
				      push[i].length);
		}
	} else
	if (dev_priv->chipset >= 0x25) {
		ret = RING_SPACE(chan, req->nr_push * 2);
		if (ret) {
			NV_ERROR(dev, "cal_space: %d\n", ret);
=======
			struct nouveau_vma *vma = (void *)(unsigned long)
				bo[push[i].bo_index].user_priv;
			u64 addr = vma->addr + push[i].offset;
			u32 length = push[i].length & ~NOUVEAU_GEM_PUSHBUF_NO_PREFETCH;
			bool no_prefetch = push[i].length & NOUVEAU_GEM_PUSHBUF_NO_PREFETCH;

			nv50_dma_push(chan, addr, length, no_prefetch);
		}
	} else
	if (drm->client.device.info.chipset >= 0x25) {
		ret = PUSH_WAIT(chan->chan.push, req->nr_push * 2);
		if (ret) {
			NV_PRINTK(err, cli, "cal_space: %d\n", ret);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
			goto out;
		}

		for (i = 0; i < req->nr_push; i++) {
			struct nouveau_bo *nvbo = (void *)(unsigned long)
				bo[push[i].bo_index].user_priv;
<<<<<<< HEAD
			struct drm_mm_node *mem = nvbo->bo.mem.mm_node;

			OUT_RING(chan, ((mem->start << PAGE_SHIFT) +
					push[i].offset) | 2);
			OUT_RING(chan, 0);
		}
	} else {
		ret = RING_SPACE(chan, req->nr_push * (2 + NOUVEAU_DMA_SKIPS));
		if (ret) {
			NV_ERROR(dev, "jmp_space: %d\n", ret);
=======

			PUSH_CALL(chan->chan.push, nvbo->offset + push[i].offset);
			PUSH_DATA(chan->chan.push, 0);
		}
	} else {
		ret = PUSH_WAIT(chan->chan.push, req->nr_push * (2 + NOUVEAU_DMA_SKIPS));
		if (ret) {
			NV_PRINTK(err, cli, "jmp_space: %d\n", ret);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
			goto out;
		}

		for (i = 0; i < req->nr_push; i++) {
			struct nouveau_bo *nvbo = (void *)(unsigned long)
				bo[push[i].bo_index].user_priv;
<<<<<<< HEAD
			struct drm_mm_node *mem = nvbo->bo.mem.mm_node;
			uint32_t cmd;

			cmd = chan->pushbuf_base + ((chan->dma.cur + 2) << 2);
=======
			uint32_t cmd;

			cmd = chan->push.addr + ((chan->dma.cur + 2) << 2);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
			cmd |= 0x20000000;
			if (unlikely(cmd != req->suffix0)) {
				if (!nvbo->kmap.virtual) {
					ret = ttm_bo_kmap(&nvbo->bo, 0,
<<<<<<< HEAD
							  nvbo->bo.mem.
							  num_pages,
=======
							  PFN_UP(nvbo->bo.base.size),
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
							  &nvbo->kmap);
					if (ret) {
						WIND_RING(chan);
						goto out;
					}
					nvbo->validate_mapped = true;
				}

				nouveau_bo_wr32(nvbo, (push[i].offset +
						push[i].length - 8) / 4, cmd);
			}

<<<<<<< HEAD
			OUT_RING(chan, ((mem->start << PAGE_SHIFT) +
					push[i].offset) | 0x20000000);
			OUT_RING(chan, 0);
			for (j = 0; j < NOUVEAU_DMA_SKIPS; j++)
				OUT_RING(chan, 0);
		}
	}

	ret = nouveau_fence_new(chan, &fence, true);
	if (ret) {
		NV_ERROR(dev, "error fencing pushbuf: %d\n", ret);
=======
			PUSH_JUMP(chan->chan.push, nvbo->offset + push[i].offset);
			PUSH_DATA(chan->chan.push, 0);
			for (j = 0; j < NOUVEAU_DMA_SKIPS; j++)
				PUSH_DATA(chan->chan.push, 0);
		}
	}

	ret = nouveau_fence_new(&fence, chan);
	if (ret) {
		NV_PRINTK(err, cli, "error fencing pushbuf: %d\n", ret);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		WIND_RING(chan);
		goto out;
	}

<<<<<<< HEAD
out:
	validate_fini(&op, fence);
	nouveau_fence_unref(&fence);

out_prevalid:
	kfree(bo);
	kfree(push);
=======
	if (sync) {
		if (!(ret = nouveau_fence_wait(fence, false, false))) {
			if ((ret = dma_fence_get_status(&fence->base)) == 1)
				ret = 0;
		}
	}

out:
	validate_fini(&op, chan, fence, bo);
	nouveau_fence_unref(&fence);

	if (do_reloc) {
		struct drm_nouveau_gem_pushbuf_bo __user *upbbo =
			u64_to_user_ptr(req->buffers);

		for (i = 0; i < req->nr_buffers; i++) {
			if (bo[i].presumed.valid)
				continue;

			if (copy_to_user(&upbbo[i].presumed, &bo[i].presumed,
					 sizeof(bo[i].presumed))) {
				ret = -EFAULT;
				break;
			}
		}
	}
out_prevalid:
	if (!IS_ERR(reloc))
		u_free(reloc);
	u_free(bo);
	u_free(push);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

out_next:
	if (chan->dma.ib_max) {
		req->suffix0 = 0x00000000;
		req->suffix1 = 0x00000000;
	} else
<<<<<<< HEAD
	if (dev_priv->chipset >= 0x25) {
=======
	if (drm->client.device.info.chipset >= 0x25) {
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		req->suffix0 = 0x00020000;
		req->suffix1 = 0x00000000;
	} else {
		req->suffix0 = 0x20000000 |
<<<<<<< HEAD
			      (chan->pushbuf_base + ((chan->dma.cur + 2) << 2));
		req->suffix1 = 0x00000000;
	}

	nouveau_channel_put(&chan);
	return ret;
}

static inline uint32_t
domain_to_ttm(struct nouveau_bo *nvbo, uint32_t domain)
{
	uint32_t flags = 0;

	if (domain & NOUVEAU_GEM_DOMAIN_VRAM)
		flags |= TTM_PL_FLAG_VRAM;
	if (domain & NOUVEAU_GEM_DOMAIN_GART)
		flags |= TTM_PL_FLAG_TT;

	return flags;
=======
			      (chan->push.addr + ((chan->dma.cur + 2) << 2));
		req->suffix1 = 0x00000000;
	}

	return nouveau_abi16_put(abi16, ret);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

int
nouveau_gem_ioctl_cpu_prep(struct drm_device *dev, void *data,
			   struct drm_file *file_priv)
{
	struct drm_nouveau_gem_cpu_prep *req = data;
	struct drm_gem_object *gem;
	struct nouveau_bo *nvbo;
	bool no_wait = !!(req->flags & NOUVEAU_GEM_CPU_PREP_NOWAIT);
<<<<<<< HEAD
	int ret = -EINVAL;

	gem = drm_gem_object_lookup(dev, file_priv, req->handle);
=======
	bool write = !!(req->flags & NOUVEAU_GEM_CPU_PREP_WRITE);
	long lret;
	int ret;

	gem = drm_gem_object_lookup(file_priv, req->handle);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	if (!gem)
		return -ENOENT;
	nvbo = nouveau_gem_object(gem);

<<<<<<< HEAD
	spin_lock(&nvbo->bo.bdev->fence_lock);
	ret = ttm_bo_wait(&nvbo->bo, true, true, no_wait);
	spin_unlock(&nvbo->bo.bdev->fence_lock);
	drm_gem_object_unreference_unlocked(gem);
=======
	lret = dma_resv_wait_timeout(nvbo->bo.base.resv,
				     dma_resv_usage_rw(write), true,
				     no_wait ? 0 : 30 * HZ);
	if (!lret)
		ret = -EBUSY;
	else if (lret > 0)
		ret = 0;
	else
		ret = lret;

	nouveau_bo_sync_for_cpu(nvbo);
	drm_gem_object_put(gem);

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	return ret;
}

int
nouveau_gem_ioctl_cpu_fini(struct drm_device *dev, void *data,
			   struct drm_file *file_priv)
{
<<<<<<< HEAD
=======
	struct drm_nouveau_gem_cpu_fini *req = data;
	struct drm_gem_object *gem;
	struct nouveau_bo *nvbo;

	gem = drm_gem_object_lookup(file_priv, req->handle);
	if (!gem)
		return -ENOENT;
	nvbo = nouveau_gem_object(gem);

	nouveau_bo_sync_for_device(nvbo);
	drm_gem_object_put(gem);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	return 0;
}

int
nouveau_gem_ioctl_info(struct drm_device *dev, void *data,
		       struct drm_file *file_priv)
{
	struct drm_nouveau_gem_info *req = data;
	struct drm_gem_object *gem;
	int ret;

<<<<<<< HEAD
	gem = drm_gem_object_lookup(dev, file_priv, req->handle);
=======
	gem = drm_gem_object_lookup(file_priv, req->handle);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	if (!gem)
		return -ENOENT;

	ret = nouveau_gem_info(file_priv, gem, req);
<<<<<<< HEAD
	drm_gem_object_unreference_unlocked(gem);
=======
	drm_gem_object_put(gem);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	return ret;
}

