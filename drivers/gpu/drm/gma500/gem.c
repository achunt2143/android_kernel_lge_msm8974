<<<<<<< HEAD
=======
// SPDX-License-Identifier: GPL-2.0-only
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
/*
 *  psb GEM interface
 *
 * Copyright (c) 2011, Intel Corporation.
 *
<<<<<<< HEAD
 * This program is free software; you can redistribute it and/or modify it
 * under the terms and conditions of the GNU General Public License,
 * version 2, as published by the Free Software Foundation.
 *
 * This program is distributed in the hope it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for
 * more details.
 *
 * You should have received a copy of the GNU General Public License along with
 * this program; if not, write to the Free Software Foundation, Inc.,
 * 51 Franklin St - Fifth Floor, Boston, MA 02110-1301 USA.
 *
=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 * Authors: Alan Cox
 *
 * TODO:
 *	-	we need to work out if the MMU is relevant (eg for
 *		accelerated operations on a GEM object)
 */

<<<<<<< HEAD
#include <drm/drmP.h>
#include <drm/drm.h>
#include "gma_drm.h"
#include "psb_drv.h"

int psb_gem_init_object(struct drm_gem_object *obj)
{
	return -EINVAL;
}

void psb_gem_free_object(struct drm_gem_object *obj)
{
	struct gtt_range *gtt = container_of(obj, struct gtt_range, gem);
	drm_gem_object_release_wrap(obj);
	/* This must occur last as it frees up the memory of the GEM object */
	psb_gtt_free_range(obj->dev, gtt);
}

int psb_gem_get_aperture(struct drm_device *dev, void *data,
				struct drm_file *file)
{
	return -EINVAL;
}

/**
 *	psb_gem_dumb_map_gtt	-	buffer mapping for dumb interface
 *	@file: our drm client file
 *	@dev: drm device
 *	@handle: GEM handle to the object (from dumb_create)
 *
 *	Do the necessary setup to allow the mapping of the frame buffer
 *	into user memory. We don't have to do much here at the moment.
 */
int psb_gem_dumb_map_gtt(struct drm_file *file, struct drm_device *dev,
			 uint32_t handle, uint64_t *offset)
{
	int ret = 0;
	struct drm_gem_object *obj;

	if (!(dev->driver->driver_features & DRIVER_GEM))
		return -ENODEV;

	mutex_lock(&dev->struct_mutex);

	/* GEM does all our handle to object mapping */
	obj = drm_gem_object_lookup(dev, file, handle);
	if (obj == NULL) {
		ret = -ENOENT;
		goto unlock;
	}
	/* What validation is needed here ? */

	/* Make it mmapable */
	if (!obj->map_list.map) {
		ret = gem_create_mmap_offset(obj);
		if (ret)
			goto out;
	}
	/* GEM should really work out the hash offsets for us */
	*offset = (u64)obj->map_list.hash.key << PAGE_SHIFT;
out:
	drm_gem_object_unreference(obj);
unlock:
	mutex_unlock(&dev->struct_mutex);
	return ret;
}

/**
 *	psb_gem_create		-	create a mappable object
 *	@file: the DRM file of the client
 *	@dev: our device
 *	@size: the size requested
 *	@handlep: returned handle (opaque number)
 *
 *	Create a GEM object, fill in the boilerplate and attach a handle to
 *	it so that userspace can speak about it. This does the core work
 *	for the various methods that do/will create GEM objects for things
 */
static int psb_gem_create(struct drm_file *file,
	struct drm_device *dev, uint64_t size, uint32_t *handlep)
{
	struct gtt_range *r;
	int ret;
	u32 handle;

	size = roundup(size, PAGE_SIZE);

	/* Allocate our object - for now a direct gtt range which is not
	   stolen memory backed */
	r = psb_gtt_alloc_range(dev, size, "gem", 0);
	if (r == NULL) {
		dev_err(dev->dev, "no memory for %lld byte GEM object\n", size);
		return -ENOSPC;
	}
	/* Initialize the extra goodies GEM needs to do all the hard work */
	if (drm_gem_object_init(dev, &r->gem, size) != 0) {
		psb_gtt_free_range(dev, r);
		/* GEM doesn't give an error code so use -ENOMEM */
		dev_err(dev->dev, "GEM init failed for %lld\n", size);
		return -ENOMEM;
	}
	/* Give the object a handle so we can carry it more easily */
	ret = drm_gem_handle_create(file, &r->gem, &handle);
	if (ret) {
		dev_err(dev->dev, "GEM handle failed for %p, %lld\n",
							&r->gem, size);
		drm_gem_object_release(&r->gem);
		psb_gtt_free_range(dev, r);
		return ret;
	}
	/* We have the initial and handle reference but need only one now */
	drm_gem_object_unreference(&r->gem);
	*handlep = handle;
	return 0;
=======
#include <linux/pagemap.h>

#include <asm/set_memory.h>

#include <drm/drm.h>
#include <drm/drm_vma_manager.h>

#include "gem.h"
#include "psb_drv.h"

/*
 * PSB GEM object
 */

int psb_gem_pin(struct psb_gem_object *pobj)
{
	struct drm_gem_object *obj = &pobj->base;
	struct drm_device *dev = obj->dev;
	struct drm_psb_private *dev_priv = to_drm_psb_private(dev);
	u32 gpu_base = dev_priv->gtt.gatt_start;
	struct page **pages;
	unsigned int npages;
	int ret;

	ret = dma_resv_lock(obj->resv, NULL);
	if (drm_WARN_ONCE(dev, ret, "dma_resv_lock() failed, ret=%d\n", ret))
		return ret;

	if (pobj->in_gart || pobj->stolen)
		goto out; /* already mapped */

	pages = drm_gem_get_pages(obj);
	if (IS_ERR(pages)) {
		ret = PTR_ERR(pages);
		goto err_dma_resv_unlock;
	}

	npages = obj->size / PAGE_SIZE;

	set_pages_array_wc(pages, npages);

	psb_gtt_insert_pages(dev_priv, &pobj->resource, pages);
	psb_mmu_insert_pages(psb_mmu_get_default_pd(dev_priv->mmu), pages,
			     (gpu_base + pobj->offset), npages, 0, 0,
			     PSB_MMU_CACHED_MEMORY);

	pobj->pages = pages;

out:
	++pobj->in_gart;
	dma_resv_unlock(obj->resv);

	return 0;

err_dma_resv_unlock:
	dma_resv_unlock(obj->resv);
	return ret;
}

void psb_gem_unpin(struct psb_gem_object *pobj)
{
	struct drm_gem_object *obj = &pobj->base;
	struct drm_device *dev = obj->dev;
	struct drm_psb_private *dev_priv = to_drm_psb_private(dev);
	u32 gpu_base = dev_priv->gtt.gatt_start;
	unsigned long npages;
	int ret;

	ret = dma_resv_lock(obj->resv, NULL);
	if (drm_WARN_ONCE(dev, ret, "dma_resv_lock() failed, ret=%d\n", ret))
		return;

	WARN_ON(!pobj->in_gart);

	--pobj->in_gart;

	if (pobj->in_gart || pobj->stolen)
		goto out;

	npages = obj->size / PAGE_SIZE;

	psb_mmu_remove_pages(psb_mmu_get_default_pd(dev_priv->mmu),
			     (gpu_base + pobj->offset), npages, 0, 0);
	psb_gtt_remove_pages(dev_priv, &pobj->resource);

	/* Reset caching flags */
	set_pages_array_wb(pobj->pages, npages);

	drm_gem_put_pages(obj, pobj->pages, true, false);
	pobj->pages = NULL;

out:
	dma_resv_unlock(obj->resv);
}

static vm_fault_t psb_gem_fault(struct vm_fault *vmf);

static void psb_gem_free_object(struct drm_gem_object *obj)
{
	struct psb_gem_object *pobj = to_psb_gem_object(obj);

	/* Undo the mmap pin if we are destroying the object */
	if (pobj->mmapping)
		psb_gem_unpin(pobj);

	drm_gem_object_release(obj);

	WARN_ON(pobj->in_gart && !pobj->stolen);

	release_resource(&pobj->resource);
	kfree(pobj);
}

static const struct vm_operations_struct psb_gem_vm_ops = {
	.fault = psb_gem_fault,
	.open = drm_gem_vm_open,
	.close = drm_gem_vm_close,
};

static const struct drm_gem_object_funcs psb_gem_object_funcs = {
	.free = psb_gem_free_object,
	.vm_ops = &psb_gem_vm_ops,
};

struct psb_gem_object *
psb_gem_create(struct drm_device *dev, u64 size, const char *name, bool stolen, u32 align)
{
	struct drm_psb_private *dev_priv = to_drm_psb_private(dev);
	struct psb_gem_object *pobj;
	struct drm_gem_object *obj;
	int ret;

	size = roundup(size, PAGE_SIZE);

	pobj = kzalloc(sizeof(*pobj), GFP_KERNEL);
	if (!pobj)
		return ERR_PTR(-ENOMEM);
	obj = &pobj->base;

	/* GTT resource */

	ret = psb_gtt_allocate_resource(dev_priv, &pobj->resource, name, size, align, stolen,
					&pobj->offset);
	if (ret)
		goto err_kfree;

	if (stolen) {
		pobj->stolen = true;
		pobj->in_gart = 1;
	}

	/* GEM object */

	obj->funcs = &psb_gem_object_funcs;

	if (stolen) {
		drm_gem_private_object_init(dev, obj, size);
	} else {
		ret = drm_gem_object_init(dev, obj, size);
		if (ret)
			goto err_release_resource;

		/* Limit the object to 32-bit mappings */
		mapping_set_gfp_mask(obj->filp->f_mapping, GFP_KERNEL | __GFP_DMA32);
	}

	return pobj;

err_release_resource:
	release_resource(&pobj->resource);
err_kfree:
	kfree(pobj);
	return ERR_PTR(ret);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

/**
 *	psb_gem_dumb_create	-	create a dumb buffer
<<<<<<< HEAD
 *	@drm_file: our client file
=======
 *	@file: our client file
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 *	@dev: our device
 *	@args: the requested arguments copied from userspace
 *
 *	Allocate a buffer suitable for use for a frame buffer of the
 *	form described by user space. Give userspace a handle by which
 *	to reference it.
 */
int psb_gem_dumb_create(struct drm_file *file, struct drm_device *dev,
			struct drm_mode_create_dumb *args)
{
<<<<<<< HEAD
	args->pitch = ALIGN(args->width * ((args->bpp + 7) / 8), 64);
	args->size = args->pitch * args->height;
	return psb_gem_create(file, dev, args->size, &args->handle);
}

/**
 *	psb_gem_dumb_destroy	-	destroy a dumb buffer
 *	@file: client file
 *	@dev: our DRM device
 *	@handle: the object handle
 *
 *	Destroy a handle that was created via psb_gem_dumb_create, at least
 *	we hope it was created that way. i915 seems to assume the caller
 *	does the checking but that might be worth review ! FIXME
 */
int psb_gem_dumb_destroy(struct drm_file *file, struct drm_device *dev,
			uint32_t handle)
{
	/* No special work needed, drop the reference and see what falls out */
	return drm_gem_handle_delete(file, handle);
=======
	size_t pitch, size;
	struct psb_gem_object *pobj;
	struct drm_gem_object *obj;
	u32 handle;
	int ret;

	pitch = args->width * DIV_ROUND_UP(args->bpp, 8);
	pitch = ALIGN(pitch, 64);

	size = pitch * args->height;
	size = roundup(size, PAGE_SIZE);
	if (!size)
		return -EINVAL;

	pobj = psb_gem_create(dev, size, "gem", false, PAGE_SIZE);
	if (IS_ERR(pobj))
		return PTR_ERR(pobj);
	obj = &pobj->base;

	ret = drm_gem_handle_create(file, obj, &handle);
	if (ret)
		goto err_drm_gem_object_put;

	drm_gem_object_put(obj);

	args->pitch = pitch;
	args->size = size;
	args->handle = handle;

	return 0;

err_drm_gem_object_put:
	drm_gem_object_put(obj);
	return ret;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

/**
 *	psb_gem_fault		-	pagefault handler for GEM objects
<<<<<<< HEAD
 *	@vma: the VMA of the GEM object
=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 *	@vmf: fault detail
 *
 *	Invoked when a fault occurs on an mmap of a GEM managed area. GEM
 *	does most of the work for us including the actual map/unmap calls
 *	but we need to do the actual page work.
 *
 *	This code eventually needs to handle faulting objects in and out
 *	of the GTT and repacking it when we run out of space. We can put
 *	that off for now and for our simple uses
 *
 *	The VMA was set up by GEM. In doing so it also ensured that the
 *	vma->vm_private_data points to the GEM object that is backing this
 *	mapping.
 */
<<<<<<< HEAD
int psb_gem_fault(struct vm_area_struct *vma, struct vm_fault *vmf)
{
	struct drm_gem_object *obj;
	struct gtt_range *r;
	int ret;
=======
static vm_fault_t psb_gem_fault(struct vm_fault *vmf)
{
	struct vm_area_struct *vma = vmf->vma;
	struct drm_gem_object *obj;
	struct psb_gem_object *pobj;
	int err;
	vm_fault_t ret;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	unsigned long pfn;
	pgoff_t page_offset;
	struct drm_device *dev;
	struct drm_psb_private *dev_priv;

	obj = vma->vm_private_data;	/* GEM object */
	dev = obj->dev;
<<<<<<< HEAD
	dev_priv = dev->dev_private;

	r = container_of(obj, struct gtt_range, gem);	/* Get the gtt range */

	/* Make sure we don't parallel update on a fault, nor move or remove
	   something from beneath our feet */
	mutex_lock(&dev->struct_mutex);

	/* For now the mmap pins the object and it stays pinned. As things
	   stand that will do us no harm */
	if (r->mmapping == 0) {
		ret = psb_gtt_pin(r);
		if (ret < 0) {
			dev_err(dev->dev, "gma500: pin failed: %d\n", ret);
			goto fail;
		}
		r->mmapping = 1;
=======
	dev_priv = to_drm_psb_private(dev);

	pobj = to_psb_gem_object(obj);

	/* Make sure we don't parallel update on a fault, nor move or remove
	   something from beneath our feet */
	mutex_lock(&dev_priv->mmap_mutex);

	/* For now the mmap pins the object and it stays pinned. As things
	   stand that will do us no harm */
	if (pobj->mmapping == 0) {
		err = psb_gem_pin(pobj);
		if (err < 0) {
			dev_err(dev->dev, "gma500: pin failed: %d\n", err);
			ret = vmf_error(err);
			goto fail;
		}
		pobj->mmapping = 1;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	}

	/* Page relative to the VMA start - we must calculate this ourselves
	   because vmf->pgoff is the fake GEM offset */
<<<<<<< HEAD
	page_offset = ((unsigned long) vmf->virtual_address - vma->vm_start)
				>> PAGE_SHIFT;

	/* CPU view of the page, don't go via the GART for CPU writes */
	if (r->stolen)
		pfn = (dev_priv->stolen_base + r->offset) >> PAGE_SHIFT;
	else
		pfn = page_to_pfn(r->pages[page_offset]);
	ret = vm_insert_pfn(vma, (unsigned long)vmf->virtual_address, pfn);

fail:
	mutex_unlock(&dev->struct_mutex);
	switch (ret) {
	case 0:
	case -ERESTARTSYS:
	case -EINTR:
		return VM_FAULT_NOPAGE;
	case -ENOMEM:
		return VM_FAULT_OOM;
	default:
		return VM_FAULT_SIGBUS;
	}
}

static int psb_gem_create_stolen(struct drm_file *file, struct drm_device *dev,
						int size, u32 *handle)
{
	struct gtt_range *gtt = psb_gtt_alloc_range(dev, size, "gem", 1);
	if (gtt == NULL)
		return -ENOMEM;
	if (drm_gem_private_object_init(dev, &gtt->gem, size) != 0)
		goto free_gtt;
	if (drm_gem_handle_create(file, &gtt->gem, handle) == 0)
		return 0;
free_gtt:
	psb_gtt_free_range(dev, gtt);
	return -ENOMEM;
}

/*
 *	GEM interfaces for our specific client
 */
int psb_gem_create_ioctl(struct drm_device *dev, void *data,
					struct drm_file *file)
{
	struct drm_psb_gem_create *args = data;
	int ret;
	if (args->flags & GMA_GEM_CREATE_STOLEN) {
		ret = psb_gem_create_stolen(file, dev, args->size,
							&args->handle);
		if (ret == 0)
			return 0;
		/* Fall throguh */
		args->flags &= ~GMA_GEM_CREATE_STOLEN;
	}
	return psb_gem_create(file, dev, args->size, &args->handle);
}

int psb_gem_mmap_ioctl(struct drm_device *dev, void *data,
					struct drm_file *file)
{
	struct drm_psb_gem_mmap *args = data;
	return dev->driver->dumb_map_offset(file, dev,
						args->handle, &args->offset);
}

=======
	page_offset = (vmf->address - vma->vm_start) >> PAGE_SHIFT;

	/* CPU view of the page, don't go via the GART for CPU writes */
	if (pobj->stolen)
		pfn = (dev_priv->stolen_base + pobj->offset) >> PAGE_SHIFT;
	else
		pfn = page_to_pfn(pobj->pages[page_offset]);
	ret = vmf_insert_pfn(vma, vmf->address, pfn);
fail:
	mutex_unlock(&dev_priv->mmap_mutex);

	return ret;
}

/*
 * Memory management
 */

/* Insert vram stolen pages into the GTT. */
static void psb_gem_mm_populate_stolen(struct drm_psb_private *pdev)
{
	struct drm_device *dev = &pdev->dev;
	unsigned int pfn_base;
	unsigned int i, num_pages;
	uint32_t pte;

	pfn_base = pdev->stolen_base >> PAGE_SHIFT;
	num_pages = pdev->vram_stolen_size >> PAGE_SHIFT;

	drm_dbg(dev, "Set up %u stolen pages starting at 0x%08x, GTT offset %dK\n",
		num_pages, pfn_base << PAGE_SHIFT, 0);

	for (i = 0; i < num_pages; ++i) {
		pte = psb_gtt_mask_pte(pfn_base + i, PSB_MMU_CACHED_MEMORY);
		iowrite32(pte, pdev->gtt_map + i);
	}

	(void)ioread32(pdev->gtt_map + i - 1);
}

int psb_gem_mm_init(struct drm_device *dev)
{
	struct drm_psb_private *dev_priv = to_drm_psb_private(dev);
	struct pci_dev *pdev = to_pci_dev(dev->dev);
	unsigned long stolen_size, vram_stolen_size;
	struct psb_gtt *pg;
	int ret;

	mutex_init(&dev_priv->mmap_mutex);

	pg = &dev_priv->gtt;

	pci_read_config_dword(pdev, PSB_BSM, &dev_priv->stolen_base);
	vram_stolen_size = pg->gtt_phys_start - dev_priv->stolen_base - PAGE_SIZE;

	stolen_size = vram_stolen_size;

	dev_dbg(dev->dev, "Stolen memory base 0x%x, size %luK\n",
		dev_priv->stolen_base, vram_stolen_size / 1024);

	pg->stolen_size = stolen_size;
	dev_priv->vram_stolen_size = vram_stolen_size;

	dev_priv->vram_addr = ioremap_wc(dev_priv->stolen_base, stolen_size);
	if (!dev_priv->vram_addr) {
		dev_err(dev->dev, "Failure to map stolen base.\n");
		ret = -ENOMEM;
		goto err_mutex_destroy;
	}

	psb_gem_mm_populate_stolen(dev_priv);

	return 0;

err_mutex_destroy:
	mutex_destroy(&dev_priv->mmap_mutex);
	return ret;
}

void psb_gem_mm_fini(struct drm_device *dev)
{
	struct drm_psb_private *dev_priv = to_drm_psb_private(dev);

	iounmap(dev_priv->vram_addr);

	mutex_destroy(&dev_priv->mmap_mutex);
}

/* Re-insert all pinned GEM objects into GTT. */
static void psb_gem_mm_populate_resources(struct drm_psb_private *pdev)
{
	unsigned int restored = 0, total = 0, size = 0;
	struct resource *r = pdev->gtt_mem->child;
	struct drm_device *dev = &pdev->dev;
	struct psb_gem_object *pobj;

	while (r) {
		/*
		 * TODO: GTT restoration needs a refactoring, so that we don't have to touch
		 *       struct psb_gem_object here. The type represents a GEM object and is
		 *       not related to the GTT itself.
		 */
		pobj = container_of(r, struct psb_gem_object, resource);
		if (pobj->pages) {
			psb_gtt_insert_pages(pdev, &pobj->resource, pobj->pages);
			size += resource_size(&pobj->resource);
			++restored;
		}
		r = r->sibling;
		++total;
	}

	drm_dbg(dev, "Restored %u of %u gtt ranges (%u KB)", restored, total, (size / 1024));
}

int psb_gem_mm_resume(struct drm_device *dev)
{
	struct drm_psb_private *dev_priv = to_drm_psb_private(dev);
	struct pci_dev *pdev = to_pci_dev(dev->dev);
	unsigned long stolen_size, vram_stolen_size;
	struct psb_gtt *pg;

	pg = &dev_priv->gtt;

	pci_read_config_dword(pdev, PSB_BSM, &dev_priv->stolen_base);
	vram_stolen_size = pg->gtt_phys_start - dev_priv->stolen_base - PAGE_SIZE;

	stolen_size = vram_stolen_size;

	dev_dbg(dev->dev, "Stolen memory base 0x%x, size %luK\n", dev_priv->stolen_base,
		vram_stolen_size / 1024);

	if (stolen_size != pg->stolen_size) {
		dev_err(dev->dev, "GTT resume error.\n");
		return -EINVAL;
	}

	psb_gem_mm_populate_stolen(dev_priv);
	psb_gem_mm_populate_resources(dev_priv);

	return 0;
}
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
