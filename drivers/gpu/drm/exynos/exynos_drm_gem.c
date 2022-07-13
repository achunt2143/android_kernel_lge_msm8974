<<<<<<< HEAD
=======
// SPDX-License-Identifier: GPL-2.0-or-later
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
/* exynos_drm_gem.c
 *
 * Copyright (c) 2011 Samsung Electronics Co., Ltd.
 * Author: Inki Dae <inki.dae@samsung.com>
<<<<<<< HEAD
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

#include "drmP.h"
#include "drm.h"

#include <linux/shmem_fs.h>
=======
 */


#include <linux/dma-buf.h>
#include <linux/pfn_t.h>
#include <linux/shmem_fs.h>
#include <linux/module.h>

#include <drm/drm_prime.h>
#include <drm/drm_vma_manager.h>
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#include <drm/exynos_drm.h>

#include "exynos_drm_drv.h"
#include "exynos_drm_gem.h"
<<<<<<< HEAD
#include "exynos_drm_buf.h"

static unsigned int convert_to_vm_err_msg(int msg)
{
	unsigned int out_msg;

	switch (msg) {
	case 0:
	case -ERESTARTSYS:
	case -EINTR:
		out_msg = VM_FAULT_NOPAGE;
		break;

	case -ENOMEM:
		out_msg = VM_FAULT_OOM;
		break;

	default:
		out_msg = VM_FAULT_SIGBUS;
		break;
	}

	return out_msg;
}

static int check_gem_flags(unsigned int flags)
{
	if (flags & ~(EXYNOS_BO_MASK)) {
		DRM_ERROR("invalid flags.\n");
		return -EINVAL;
	}

	return 0;
}

static unsigned long roundup_gem_size(unsigned long size, unsigned int flags)
{
	if (!IS_NONCONTIG_BUFFER(flags)) {
		if (size >= SZ_1M)
			return roundup(size, SECTION_SIZE);
		else if (size >= SZ_64K)
			return roundup(size, SZ_64K);
		else
			goto out;
	}
out:
	return roundup(size, PAGE_SIZE);
}

static struct page **exynos_gem_get_pages(struct drm_gem_object *obj,
						gfp_t gfpmask)
{
	struct inode *inode;
	struct address_space *mapping;
	struct page *p, **pages;
	int i, npages;

	/* This is the shared memory object that backs the GEM resource */
	inode = obj->filp->f_path.dentry->d_inode;
	mapping = inode->i_mapping;

	npages = obj->size >> PAGE_SHIFT;

	pages = drm_malloc_ab(npages, sizeof(struct page *));
	if (pages == NULL)
		return ERR_PTR(-ENOMEM);

	gfpmask |= mapping_gfp_mask(mapping);

	for (i = 0; i < npages; i++) {
		p = shmem_read_mapping_page_gfp(mapping, i, gfpmask);
		if (IS_ERR(p))
			goto fail;
		pages[i] = p;
	}

	return pages;

fail:
	while (i--)
		page_cache_release(pages[i]);

	drm_free_large(pages);
	return ERR_PTR(PTR_ERR(p));
}

static void exynos_gem_put_pages(struct drm_gem_object *obj,
					struct page **pages,
					bool dirty, bool accessed)
{
	int i, npages;

	npages = obj->size >> PAGE_SHIFT;

	for (i = 0; i < npages; i++) {
		if (dirty)
			set_page_dirty(pages[i]);

		if (accessed)
			mark_page_accessed(pages[i]);

		/* Undo the reference we took when populating the table */
		page_cache_release(pages[i]);
	}

	drm_free_large(pages);
}

static int exynos_drm_gem_map_pages(struct drm_gem_object *obj,
					struct vm_area_struct *vma,
					unsigned long f_vaddr,
					pgoff_t page_offset)
{
	struct exynos_drm_gem_obj *exynos_gem_obj = to_exynos_gem_obj(obj);
	struct exynos_drm_gem_buf *buf = exynos_gem_obj->buffer;
	unsigned long pfn;

	if (exynos_gem_obj->flags & EXYNOS_BO_NONCONTIG) {
		if (!buf->pages)
			return -EINTR;

		pfn = page_to_pfn(buf->pages[page_offset++]);
	} else
		pfn = (buf->dma_addr >> PAGE_SHIFT) + page_offset;

	return vm_insert_mixed(vma, f_vaddr, pfn);
}

static int exynos_drm_gem_get_pages(struct drm_gem_object *obj)
{
	struct exynos_drm_gem_obj *exynos_gem_obj = to_exynos_gem_obj(obj);
	struct exynos_drm_gem_buf *buf = exynos_gem_obj->buffer;
	struct scatterlist *sgl;
	struct page **pages;
	unsigned int npages, i = 0;
	int ret;

	if (buf->pages) {
		DRM_DEBUG_KMS("already allocated.\n");
		return -EINVAL;
	}

	pages = exynos_gem_get_pages(obj, GFP_KERNEL);
	if (IS_ERR(pages)) {
		DRM_ERROR("failed to get pages.\n");
		return PTR_ERR(pages);
	}

	npages = obj->size >> PAGE_SHIFT;

	buf->sgt = kzalloc(sizeof(struct sg_table), GFP_KERNEL);
	if (!buf->sgt) {
		DRM_ERROR("failed to allocate sg table.\n");
		ret = -ENOMEM;
		goto err;
	}

	ret = sg_alloc_table(buf->sgt, npages, GFP_KERNEL);
	if (ret < 0) {
		DRM_ERROR("failed to initialize sg table.\n");
		ret = -EFAULT;
		goto err1;
	}

	sgl = buf->sgt->sgl;

	/* set all pages to sg list. */
	while (i < npages) {
		sg_set_page(sgl, pages[i], PAGE_SIZE, 0);
		sg_dma_address(sgl) = page_to_phys(pages[i]);
		i++;
		sgl = sg_next(sgl);
	}

	/* add some codes for UNCACHED type here. TODO */

	buf->pages = pages;
	return ret;
err1:
	kfree(buf->sgt);
	buf->sgt = NULL;
err:
	exynos_gem_put_pages(obj, pages, true, false);
	return ret;

}

static void exynos_drm_gem_put_pages(struct drm_gem_object *obj)
{
	struct exynos_drm_gem_obj *exynos_gem_obj = to_exynos_gem_obj(obj);
	struct exynos_drm_gem_buf *buf = exynos_gem_obj->buffer;

	/*
	 * if buffer typs is EXYNOS_BO_NONCONTIG then release all pages
	 * allocated at gem fault handler.
	 */
	sg_free_table(buf->sgt);
	kfree(buf->sgt);
	buf->sgt = NULL;

	exynos_gem_put_pages(obj, buf->pages, true, false);
	buf->pages = NULL;

	/* add some codes for UNCACHED type here. TODO */
=======

MODULE_IMPORT_NS(DMA_BUF);

static int exynos_drm_gem_mmap(struct drm_gem_object *obj, struct vm_area_struct *vma);

static int exynos_drm_alloc_buf(struct exynos_drm_gem *exynos_gem, bool kvmap)
{
	struct drm_device *dev = exynos_gem->base.dev;
	unsigned long attr = 0;

	if (exynos_gem->dma_addr) {
		DRM_DEV_DEBUG_KMS(to_dma_dev(dev), "already allocated.\n");
		return 0;
	}

	/*
	 * if EXYNOS_BO_CONTIG, fully physically contiguous memory
	 * region will be allocated else physically contiguous
	 * as possible.
	 */
	if (!(exynos_gem->flags & EXYNOS_BO_NONCONTIG))
		attr |= DMA_ATTR_FORCE_CONTIGUOUS;

	/*
	 * if EXYNOS_BO_WC or EXYNOS_BO_NONCACHABLE, writecombine mapping
	 * else cachable mapping.
	 */
	if (exynos_gem->flags & EXYNOS_BO_WC ||
			!(exynos_gem->flags & EXYNOS_BO_CACHABLE))
		attr |= DMA_ATTR_WRITE_COMBINE;

	/* FBDev emulation requires kernel mapping */
	if (!kvmap)
		attr |= DMA_ATTR_NO_KERNEL_MAPPING;

	exynos_gem->dma_attrs = attr;
	exynos_gem->cookie = dma_alloc_attrs(to_dma_dev(dev), exynos_gem->size,
					     &exynos_gem->dma_addr, GFP_KERNEL,
					     exynos_gem->dma_attrs);
	if (!exynos_gem->cookie) {
		DRM_DEV_ERROR(to_dma_dev(dev), "failed to allocate buffer.\n");
		return -ENOMEM;
	}

	if (kvmap)
		exynos_gem->kvaddr = exynos_gem->cookie;

	DRM_DEV_DEBUG_KMS(to_dma_dev(dev), "dma_addr(0x%lx), size(0x%lx)\n",
			(unsigned long)exynos_gem->dma_addr, exynos_gem->size);
	return 0;
}

static void exynos_drm_free_buf(struct exynos_drm_gem *exynos_gem)
{
	struct drm_device *dev = exynos_gem->base.dev;

	if (!exynos_gem->dma_addr) {
		DRM_DEV_DEBUG_KMS(dev->dev, "dma_addr is invalid.\n");
		return;
	}

	DRM_DEV_DEBUG_KMS(dev->dev, "dma_addr(0x%lx), size(0x%lx)\n",
			(unsigned long)exynos_gem->dma_addr, exynos_gem->size);

	dma_free_attrs(to_dma_dev(dev), exynos_gem->size, exynos_gem->cookie,
			(dma_addr_t)exynos_gem->dma_addr,
			exynos_gem->dma_attrs);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

static int exynos_drm_gem_handle_create(struct drm_gem_object *obj,
					struct drm_file *file_priv,
					unsigned int *handle)
{
	int ret;

	/*
	 * allocate a id of idr table where the obj is registered
	 * and handle has the id what user can see.
	 */
	ret = drm_gem_handle_create(file_priv, obj, handle);
	if (ret)
		return ret;

<<<<<<< HEAD
	DRM_DEBUG_KMS("gem handle = 0x%x\n", *handle);

	/* drop reference from allocate - handle holds it now. */
	drm_gem_object_unreference_unlocked(obj);
=======
	DRM_DEV_DEBUG_KMS(to_dma_dev(obj->dev), "gem handle = 0x%x\n", *handle);

	/* drop reference from allocate - handle holds it now. */
	drm_gem_object_put(obj);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	return 0;
}

<<<<<<< HEAD
void exynos_drm_gem_destroy(struct exynos_drm_gem_obj *exynos_gem_obj)
{
	struct drm_gem_object *obj;

	DRM_DEBUG_KMS("%s\n", __FILE__);

	if (!exynos_gem_obj)
		return;

	obj = &exynos_gem_obj->base;

	DRM_DEBUG_KMS("handle count = %d\n", atomic_read(&obj->handle_count));

	if ((exynos_gem_obj->flags & EXYNOS_BO_NONCONTIG) &&
			exynos_gem_obj->buffer->pages)
		exynos_drm_gem_put_pages(obj);
	else
		exynos_drm_free_buf(obj->dev, exynos_gem_obj->flags,
					exynos_gem_obj->buffer);

	exynos_drm_fini_buf(obj->dev, exynos_gem_obj->buffer);
	exynos_gem_obj->buffer = NULL;

	if (obj->map_list.map)
		drm_gem_free_mmap_offset(obj);
=======
void exynos_drm_gem_destroy(struct exynos_drm_gem *exynos_gem)
{
	struct drm_gem_object *obj = &exynos_gem->base;

	DRM_DEV_DEBUG_KMS(to_dma_dev(obj->dev), "handle count = %d\n",
			  obj->handle_count);

	/*
	 * do not release memory region from exporter.
	 *
	 * the region will be released by exporter
	 * once dmabuf's refcount becomes 0.
	 */
	if (obj->import_attach)
		drm_prime_gem_destroy(obj, exynos_gem->sgt);
	else
		exynos_drm_free_buf(exynos_gem);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	/* release file pointer to gem object. */
	drm_gem_object_release(obj);

<<<<<<< HEAD
	kfree(exynos_gem_obj);
	exynos_gem_obj = NULL;
}

static struct exynos_drm_gem_obj *exynos_drm_gem_init(struct drm_device *dev,
						      unsigned long size)
{
	struct exynos_drm_gem_obj *exynos_gem_obj;
	struct drm_gem_object *obj;
	int ret;

	exynos_gem_obj = kzalloc(sizeof(*exynos_gem_obj), GFP_KERNEL);
	if (!exynos_gem_obj) {
		DRM_ERROR("failed to allocate exynos gem object\n");
		return NULL;
	}

	exynos_gem_obj->size = size;
	obj = &exynos_gem_obj->base;

	ret = drm_gem_object_init(dev, obj, size);
	if (ret < 0) {
		DRM_ERROR("failed to initialize gem object\n");
		kfree(exynos_gem_obj);
		return NULL;
	}

	DRM_DEBUG_KMS("created file object = 0x%x\n", (unsigned int)obj->filp);

	return exynos_gem_obj;
}

struct exynos_drm_gem_obj *exynos_drm_gem_create(struct drm_device *dev,
						unsigned int flags,
						unsigned long size)
{
	struct exynos_drm_gem_obj *exynos_gem_obj;
	struct exynos_drm_gem_buf *buf;
	int ret;

	if (!size) {
		DRM_ERROR("invalid size.\n");
		return ERR_PTR(-EINVAL);
	}

	size = roundup_gem_size(size, flags);
	DRM_DEBUG_KMS("%s\n", __FILE__);

	ret = check_gem_flags(flags);
	if (ret)
		return ERR_PTR(ret);

	buf = exynos_drm_init_buf(dev, size);
	if (!buf)
		return ERR_PTR(-ENOMEM);

	exynos_gem_obj = exynos_drm_gem_init(dev, size);
	if (!exynos_gem_obj) {
		ret = -ENOMEM;
		goto err_fini_buf;
	}

	exynos_gem_obj->buffer = buf;

	/* set memory type and cache attribute from user side. */
	exynos_gem_obj->flags = flags;

	/*
	 * allocate all pages as desired size if user wants to allocate
	 * physically non-continuous memory.
	 */
	if (flags & EXYNOS_BO_NONCONTIG) {
		ret = exynos_drm_gem_get_pages(&exynos_gem_obj->base);
		if (ret < 0) {
			drm_gem_object_release(&exynos_gem_obj->base);
			goto err_fini_buf;
		}
	} else {
		ret = exynos_drm_alloc_buf(dev, buf, flags);
		if (ret < 0) {
			drm_gem_object_release(&exynos_gem_obj->base);
			goto err_fini_buf;
		}
	}

	return exynos_gem_obj;

err_fini_buf:
	exynos_drm_fini_buf(dev, buf);
	return ERR_PTR(ret);
=======
	kfree(exynos_gem);
}

static const struct vm_operations_struct exynos_drm_gem_vm_ops = {
	.open = drm_gem_vm_open,
	.close = drm_gem_vm_close,
};

static const struct drm_gem_object_funcs exynos_drm_gem_object_funcs = {
	.free = exynos_drm_gem_free_object,
	.get_sg_table = exynos_drm_gem_prime_get_sg_table,
	.mmap = exynos_drm_gem_mmap,
	.vm_ops = &exynos_drm_gem_vm_ops,
};

static struct exynos_drm_gem *exynos_drm_gem_init(struct drm_device *dev,
						  unsigned long size)
{
	struct exynos_drm_gem *exynos_gem;
	struct drm_gem_object *obj;
	int ret;

	exynos_gem = kzalloc(sizeof(*exynos_gem), GFP_KERNEL);
	if (!exynos_gem)
		return ERR_PTR(-ENOMEM);

	exynos_gem->size = size;
	obj = &exynos_gem->base;

	obj->funcs = &exynos_drm_gem_object_funcs;

	ret = drm_gem_object_init(dev, obj, size);
	if (ret < 0) {
		DRM_DEV_ERROR(dev->dev, "failed to initialize gem object\n");
		kfree(exynos_gem);
		return ERR_PTR(ret);
	}

	ret = drm_gem_create_mmap_offset(obj);
	if (ret < 0) {
		drm_gem_object_release(obj);
		kfree(exynos_gem);
		return ERR_PTR(ret);
	}

	DRM_DEV_DEBUG_KMS(dev->dev, "created file object = %pK\n", obj->filp);

	return exynos_gem;
}

struct exynos_drm_gem *exynos_drm_gem_create(struct drm_device *dev,
					     unsigned int flags,
					     unsigned long size,
					     bool kvmap)
{
	struct exynos_drm_gem *exynos_gem;
	int ret;

	if (flags & ~(EXYNOS_BO_MASK)) {
		DRM_DEV_ERROR(dev->dev,
			      "invalid GEM buffer flags: %u\n", flags);
		return ERR_PTR(-EINVAL);
	}

	if (!size) {
		DRM_DEV_ERROR(dev->dev, "invalid GEM buffer size: %lu\n", size);
		return ERR_PTR(-EINVAL);
	}

	size = roundup(size, PAGE_SIZE);

	exynos_gem = exynos_drm_gem_init(dev, size);
	if (IS_ERR(exynos_gem))
		return exynos_gem;

	if (!is_drm_iommu_supported(dev) && (flags & EXYNOS_BO_NONCONTIG)) {
		/*
		 * when no IOMMU is available, all allocated buffers are
		 * contiguous anyway, so drop EXYNOS_BO_NONCONTIG flag
		 */
		flags &= ~EXYNOS_BO_NONCONTIG;
		DRM_WARN("Non-contiguous allocation is not supported without IOMMU, falling back to contiguous buffer\n");
	}

	/* set memory type and cache attribute from user side. */
	exynos_gem->flags = flags;

	ret = exynos_drm_alloc_buf(exynos_gem, kvmap);
	if (ret < 0) {
		drm_gem_object_release(&exynos_gem->base);
		kfree(exynos_gem);
		return ERR_PTR(ret);
	}

	return exynos_gem;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

int exynos_drm_gem_create_ioctl(struct drm_device *dev, void *data,
				struct drm_file *file_priv)
{
	struct drm_exynos_gem_create *args = data;
<<<<<<< HEAD
	struct exynos_drm_gem_obj *exynos_gem_obj;
	int ret;

	DRM_DEBUG_KMS("%s\n", __FILE__);

	exynos_gem_obj = exynos_drm_gem_create(dev, args->flags, args->size);
	if (IS_ERR(exynos_gem_obj))
		return PTR_ERR(exynos_gem_obj);

	ret = exynos_drm_gem_handle_create(&exynos_gem_obj->base, file_priv,
			&args->handle);
	if (ret) {
		exynos_drm_gem_destroy(exynos_gem_obj);
=======
	struct exynos_drm_gem *exynos_gem;
	int ret;

	exynos_gem = exynos_drm_gem_create(dev, args->flags, args->size, false);
	if (IS_ERR(exynos_gem))
		return PTR_ERR(exynos_gem);

	ret = exynos_drm_gem_handle_create(&exynos_gem->base, file_priv,
					   &args->handle);
	if (ret) {
		exynos_drm_gem_destroy(exynos_gem);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		return ret;
	}

	return 0;
}

<<<<<<< HEAD
void *exynos_drm_gem_get_dma_addr(struct drm_device *dev,
					unsigned int gem_handle,
					struct drm_file *file_priv)
{
	struct exynos_drm_gem_obj *exynos_gem_obj;
	struct drm_gem_object *obj;

	obj = drm_gem_object_lookup(dev, file_priv, gem_handle);
	if (!obj) {
		DRM_ERROR("failed to lookup gem object.\n");
		return ERR_PTR(-EINVAL);
	}

	exynos_gem_obj = to_exynos_gem_obj(obj);

	if (exynos_gem_obj->flags & EXYNOS_BO_NONCONTIG) {
		DRM_DEBUG_KMS("not support NONCONTIG type.\n");
		drm_gem_object_unreference_unlocked(obj);

		/* TODO */
		return ERR_PTR(-EINVAL);
	}

	return &exynos_gem_obj->buffer->dma_addr;
}

void exynos_drm_gem_put_dma_addr(struct drm_device *dev,
					unsigned int gem_handle,
					struct drm_file *file_priv)
{
	struct exynos_drm_gem_obj *exynos_gem_obj;
	struct drm_gem_object *obj;

	obj = drm_gem_object_lookup(dev, file_priv, gem_handle);
	if (!obj) {
		DRM_ERROR("failed to lookup gem object.\n");
		return;
	}

	exynos_gem_obj = to_exynos_gem_obj(obj);

	if (exynos_gem_obj->flags & EXYNOS_BO_NONCONTIG) {
		DRM_DEBUG_KMS("not support NONCONTIG type.\n");
		drm_gem_object_unreference_unlocked(obj);

		/* TODO */
		return;
	}

	drm_gem_object_unreference_unlocked(obj);

	/*
	 * decrease obj->refcount one more time because we has already
	 * increased it at exynos_drm_gem_get_dma_addr().
	 */
	drm_gem_object_unreference_unlocked(obj);
}

int exynos_drm_gem_map_offset_ioctl(struct drm_device *dev, void *data,
				    struct drm_file *file_priv)
{
	struct drm_exynos_gem_map_off *args = data;

	DRM_DEBUG_KMS("%s\n", __FILE__);

	DRM_DEBUG_KMS("handle = 0x%x, offset = 0x%lx\n",
			args->handle, (unsigned long)args->offset);

	if (!(dev->driver->driver_features & DRIVER_GEM)) {
		DRM_ERROR("does not support GEM.\n");
		return -ENODEV;
	}

	return exynos_drm_gem_dumb_map_offset(file_priv, dev, args->handle,
			&args->offset);
}

static int exynos_drm_gem_mmap_buffer(struct file *filp,
				      struct vm_area_struct *vma)
{
	struct drm_gem_object *obj = filp->private_data;
	struct exynos_drm_gem_obj *exynos_gem_obj = to_exynos_gem_obj(obj);
	struct exynos_drm_gem_buf *buffer;
	unsigned long pfn, vm_size, usize, uaddr = vma->vm_start;
	int ret;

	DRM_DEBUG_KMS("%s\n", __FILE__);

	vma->vm_flags |= (VM_IO | VM_RESERVED);

	/* in case of direct mapping, always having non-cachable attribute */
	vma->vm_page_prot = pgprot_noncached(vma->vm_page_prot);

	vm_size = usize = vma->vm_end - vma->vm_start;

	/*
	 * a buffer contains information to physically continuous memory
	 * allocated by user request or at framebuffer creation.
	 */
	buffer = exynos_gem_obj->buffer;

	/* check if user-requested size is valid. */
	if (vm_size > buffer->size)
		return -EINVAL;

	if (exynos_gem_obj->flags & EXYNOS_BO_NONCONTIG) {
		int i = 0;

		if (!buffer->pages)
			return -EINVAL;

		vma->vm_flags |= VM_MIXEDMAP;

		do {
			ret = vm_insert_page(vma, uaddr, buffer->pages[i++]);
			if (ret) {
				DRM_ERROR("failed to remap user space.\n");
				return ret;
			}

			uaddr += PAGE_SIZE;
			usize -= PAGE_SIZE;
		} while (usize > 0);
	} else {
		/*
		 * get page frame number to physical memory to be mapped
		 * to user space.
		 */
		pfn = ((unsigned long)exynos_gem_obj->buffer->dma_addr) >>
								PAGE_SHIFT;

		DRM_DEBUG_KMS("pfn = 0x%lx\n", pfn);

		if (remap_pfn_range(vma, vma->vm_start, pfn, vm_size,
					vma->vm_page_prot)) {
			DRM_ERROR("failed to remap pfn range.\n");
			return -EAGAIN;
		}
=======
int exynos_drm_gem_map_ioctl(struct drm_device *dev, void *data,
			     struct drm_file *file_priv)
{
	struct drm_exynos_gem_map *args = data;

	return drm_gem_dumb_map_offset(file_priv, dev, args->handle,
				       &args->offset);
}

struct exynos_drm_gem *exynos_drm_gem_get(struct drm_file *filp,
					  unsigned int gem_handle)
{
	struct drm_gem_object *obj;

	obj = drm_gem_object_lookup(filp, gem_handle);
	if (!obj)
		return NULL;
	return to_exynos_gem(obj);
}

static int exynos_drm_gem_mmap_buffer(struct exynos_drm_gem *exynos_gem,
				      struct vm_area_struct *vma)
{
	struct drm_device *drm_dev = exynos_gem->base.dev;
	unsigned long vm_size;
	int ret;

	vm_flags_clear(vma, VM_PFNMAP);
	vma->vm_pgoff = 0;

	vm_size = vma->vm_end - vma->vm_start;

	/* check if user-requested size is valid. */
	if (vm_size > exynos_gem->size)
		return -EINVAL;

	ret = dma_mmap_attrs(to_dma_dev(drm_dev), vma, exynos_gem->cookie,
			     exynos_gem->dma_addr, exynos_gem->size,
			     exynos_gem->dma_attrs);
	if (ret < 0) {
		DRM_ERROR("failed to mmap.\n");
		return ret;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	}

	return 0;
}

<<<<<<< HEAD
static const struct file_operations exynos_drm_gem_fops = {
	.mmap = exynos_drm_gem_mmap_buffer,
};

int exynos_drm_gem_mmap_ioctl(struct drm_device *dev, void *data,
			      struct drm_file *file_priv)
{
	struct drm_exynos_gem_mmap *args = data;
	struct drm_gem_object *obj;
	unsigned int addr;

	DRM_DEBUG_KMS("%s\n", __FILE__);

	if (!(dev->driver->driver_features & DRIVER_GEM)) {
		DRM_ERROR("does not support GEM.\n");
		return -ENODEV;
	}

	obj = drm_gem_object_lookup(dev, file_priv, args->handle);
	if (!obj) {
		DRM_ERROR("failed to lookup gem object.\n");
		return -EINVAL;
	}

	obj->filp->f_op = &exynos_drm_gem_fops;
	obj->filp->private_data = obj;

	addr = vm_mmap(obj->filp, 0, args->size,
			PROT_READ | PROT_WRITE, MAP_SHARED, 0);

	drm_gem_object_unreference_unlocked(obj);

	if (IS_ERR((void *)addr))
		return PTR_ERR((void *)addr);

	args->mapped = addr;

	DRM_DEBUG_KMS("mapped = 0x%lx\n", (unsigned long)args->mapped);

	return 0;
}

int exynos_drm_gem_init_object(struct drm_gem_object *obj)
{
	DRM_DEBUG_KMS("%s\n", __FILE__);
=======
int exynos_drm_gem_get_ioctl(struct drm_device *dev, void *data,
				      struct drm_file *file_priv)
{
	struct exynos_drm_gem *exynos_gem;
	struct drm_exynos_gem_info *args = data;
	struct drm_gem_object *obj;

	obj = drm_gem_object_lookup(file_priv, args->handle);
	if (!obj) {
		DRM_DEV_ERROR(dev->dev, "failed to lookup gem object.\n");
		return -EINVAL;
	}

	exynos_gem = to_exynos_gem(obj);

	args->flags = exynos_gem->flags;
	args->size = exynos_gem->size;

	drm_gem_object_put(obj);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	return 0;
}

void exynos_drm_gem_free_object(struct drm_gem_object *obj)
{
<<<<<<< HEAD
	DRM_DEBUG_KMS("%s\n", __FILE__);

	exynos_drm_gem_destroy(to_exynos_gem_obj(obj));
=======
	exynos_drm_gem_destroy(to_exynos_gem(obj));
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

int exynos_drm_gem_dumb_create(struct drm_file *file_priv,
			       struct drm_device *dev,
			       struct drm_mode_create_dumb *args)
{
<<<<<<< HEAD
	struct exynos_drm_gem_obj *exynos_gem_obj;
	int ret;

	DRM_DEBUG_KMS("%s\n", __FILE__);

	/*
	 * alocate memory to be used for framebuffer.
=======
	struct exynos_drm_gem *exynos_gem;
	unsigned int flags;
	int ret;

	/*
	 * allocate memory to be used for framebuffer.
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	 * - this callback would be called by user application
	 *	with DRM_IOCTL_MODE_CREATE_DUMB command.
	 */

<<<<<<< HEAD
	args->pitch = args->width * args->bpp >> 3;
	args->size = PAGE_ALIGN(args->pitch * args->height);

	exynos_gem_obj = exynos_drm_gem_create(dev, args->flags, args->size);
	if (IS_ERR(exynos_gem_obj))
		return PTR_ERR(exynos_gem_obj);

	ret = exynos_drm_gem_handle_create(&exynos_gem_obj->base, file_priv,
			&args->handle);
	if (ret) {
		exynos_drm_gem_destroy(exynos_gem_obj);
=======
	args->pitch = args->width * ((args->bpp + 7) / 8);
	args->size = args->pitch * args->height;

	if (is_drm_iommu_supported(dev))
		flags = EXYNOS_BO_NONCONTIG | EXYNOS_BO_WC;
	else
		flags = EXYNOS_BO_CONTIG | EXYNOS_BO_WC;

	exynos_gem = exynos_drm_gem_create(dev, flags, args->size, false);
	if (IS_ERR(exynos_gem)) {
		dev_warn(dev->dev, "FB allocation failed.\n");
		return PTR_ERR(exynos_gem);
	}

	ret = exynos_drm_gem_handle_create(&exynos_gem->base, file_priv,
					   &args->handle);
	if (ret) {
		exynos_drm_gem_destroy(exynos_gem);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		return ret;
	}

	return 0;
}

<<<<<<< HEAD
int exynos_drm_gem_dumb_map_offset(struct drm_file *file_priv,
				   struct drm_device *dev, uint32_t handle,
				   uint64_t *offset)
{
	struct exynos_drm_gem_obj *exynos_gem_obj;
	struct drm_gem_object *obj;
	int ret = 0;

	DRM_DEBUG_KMS("%s\n", __FILE__);

	mutex_lock(&dev->struct_mutex);

	/*
	 * get offset of memory allocated for drm framebuffer.
	 * - this callback would be called by user application
	 *	with DRM_IOCTL_MODE_MAP_DUMB command.
	 */

	obj = drm_gem_object_lookup(dev, file_priv, handle);
	if (!obj) {
		DRM_ERROR("failed to lookup gem object.\n");
		ret = -EINVAL;
		goto unlock;
	}

	exynos_gem_obj = to_exynos_gem_obj(obj);

	if (!exynos_gem_obj->base.map_list.map) {
		ret = drm_gem_create_mmap_offset(&exynos_gem_obj->base);
		if (ret)
			goto out;
	}

	*offset = (u64)exynos_gem_obj->base.map_list.hash.key << PAGE_SHIFT;
	DRM_DEBUG_KMS("offset = 0x%lx\n", (unsigned long)*offset);

out:
	drm_gem_object_unreference(obj);
unlock:
	mutex_unlock(&dev->struct_mutex);
	return ret;
}

int exynos_drm_gem_dumb_destroy(struct drm_file *file_priv,
				struct drm_device *dev,
				unsigned int handle)
{
	int ret;

	DRM_DEBUG_KMS("%s\n", __FILE__);

	/*
	 * obj->refcount and obj->handle_count are decreased and
	 * if both them are 0 then exynos_drm_gem_free_object()
	 * would be called by callback to release resources.
	 */
	ret = drm_gem_handle_delete(file_priv, handle);
	if (ret < 0) {
		DRM_ERROR("failed to delete drm_gem_handle.\n");
		return ret;
	}

	return 0;
}

int exynos_drm_gem_fault(struct vm_area_struct *vma, struct vm_fault *vmf)
{
	struct drm_gem_object *obj = vma->vm_private_data;
	struct drm_device *dev = obj->dev;
	unsigned long f_vaddr;
	pgoff_t page_offset;
	int ret;

	page_offset = ((unsigned long)vmf->virtual_address -
			vma->vm_start) >> PAGE_SHIFT;
	f_vaddr = (unsigned long)vmf->virtual_address;

	mutex_lock(&dev->struct_mutex);

	ret = exynos_drm_gem_map_pages(obj, vma, f_vaddr, page_offset);
	if (ret < 0)
		DRM_ERROR("failed to map pages.\n");

	mutex_unlock(&dev->struct_mutex);

	return convert_to_vm_err_msg(ret);
}

int exynos_drm_gem_mmap(struct file *filp, struct vm_area_struct *vma)
{
	int ret;

	DRM_DEBUG_KMS("%s\n", __FILE__);

	/* set vm_area_struct. */
	ret = drm_gem_mmap(filp, vma);
	if (ret < 0) {
		DRM_ERROR("failed to mmap.\n");
		return ret;
	}

	vma->vm_flags &= ~VM_PFNMAP;
	vma->vm_flags |= VM_MIXEDMAP;

	return ret;
=======
static int exynos_drm_gem_mmap(struct drm_gem_object *obj, struct vm_area_struct *vma)
{
	struct exynos_drm_gem *exynos_gem = to_exynos_gem(obj);
	int ret;

	if (obj->import_attach)
		return dma_buf_mmap(obj->dma_buf, vma, 0);

	vm_flags_set(vma, VM_IO | VM_DONTEXPAND | VM_DONTDUMP);

	DRM_DEV_DEBUG_KMS(to_dma_dev(obj->dev), "flags = 0x%x\n",
			  exynos_gem->flags);

	/* non-cachable as default. */
	if (exynos_gem->flags & EXYNOS_BO_CACHABLE)
		vma->vm_page_prot = vm_get_page_prot(vma->vm_flags);
	else if (exynos_gem->flags & EXYNOS_BO_WC)
		vma->vm_page_prot =
			pgprot_writecombine(vm_get_page_prot(vma->vm_flags));
	else
		vma->vm_page_prot =
			pgprot_noncached(vm_get_page_prot(vma->vm_flags));

	ret = exynos_drm_gem_mmap_buffer(exynos_gem, vma);
	if (ret)
		goto err_close_vm;

	return ret;

err_close_vm:
	drm_gem_vm_close(vma);

	return ret;
}

/* low-level interface prime helpers */
struct drm_gem_object *exynos_drm_gem_prime_import(struct drm_device *dev,
					    struct dma_buf *dma_buf)
{
	return drm_gem_prime_import_dev(dev, dma_buf, to_dma_dev(dev));
}

struct sg_table *exynos_drm_gem_prime_get_sg_table(struct drm_gem_object *obj)
{
	struct exynos_drm_gem *exynos_gem = to_exynos_gem(obj);
	struct drm_device *drm_dev = obj->dev;
	struct sg_table *sgt;
	int ret;

	sgt = kzalloc(sizeof(*sgt), GFP_KERNEL);
	if (!sgt)
		return ERR_PTR(-ENOMEM);

	ret = dma_get_sgtable_attrs(to_dma_dev(drm_dev), sgt, exynos_gem->cookie,
				    exynos_gem->dma_addr, exynos_gem->size,
				    exynos_gem->dma_attrs);
	if (ret) {
		DRM_ERROR("failed to get sgtable, %d\n", ret);
		kfree(sgt);
		return ERR_PTR(ret);
	}

	return sgt;
}

struct drm_gem_object *
exynos_drm_gem_prime_import_sg_table(struct drm_device *dev,
				     struct dma_buf_attachment *attach,
				     struct sg_table *sgt)
{
	struct exynos_drm_gem *exynos_gem;

	/* check if the entries in the sg_table are contiguous */
	if (drm_prime_get_contiguous_size(sgt) < attach->dmabuf->size) {
		DRM_ERROR("buffer chunks must be mapped contiguously");
		return ERR_PTR(-EINVAL);
	}

	exynos_gem = exynos_drm_gem_init(dev, attach->dmabuf->size);
	if (IS_ERR(exynos_gem))
		return ERR_CAST(exynos_gem);

	/*
	 * Buffer has been mapped as contiguous into DMA address space,
	 * but if there is IOMMU, it can be either CONTIG or NONCONTIG.
	 * We assume a simplified logic below:
	 */
	if (is_drm_iommu_supported(dev))
		exynos_gem->flags |= EXYNOS_BO_NONCONTIG;
	else
		exynos_gem->flags |= EXYNOS_BO_CONTIG;

	exynos_gem->dma_addr = sg_dma_address(sgt->sgl);
	exynos_gem->sgt = sgt;
	return &exynos_gem->base;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}
