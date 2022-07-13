/*
<<<<<<< HEAD
 * Copyright © 2008 Intel Corporation
=======
 * Copyright © 2008-2015 Intel Corporation
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
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
 * THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS
 * IN THE SOFTWARE.
 *
 * Authors:
 *    Eric Anholt <eric@anholt.net>
 *
 */

<<<<<<< HEAD
#include "drmP.h"
#include "drm.h"
#include "i915_drm.h"
#include "i915_drv.h"
#include "i915_trace.h"
#include "intel_drv.h"
#include <linux/shmem_fs.h>
#include <linux/slab.h>
#include <linux/swap.h>
#include <linux/pci.h>

static __must_check int i915_gem_object_flush_gpu_write_domain(struct drm_i915_gem_object *obj);
static void i915_gem_object_flush_gtt_write_domain(struct drm_i915_gem_object *obj);
static void i915_gem_object_flush_cpu_write_domain(struct drm_i915_gem_object *obj);
static __must_check int i915_gem_object_set_to_cpu_domain(struct drm_i915_gem_object *obj,
							  bool write);
static __must_check int i915_gem_object_set_cpu_read_domain_range(struct drm_i915_gem_object *obj,
								  uint64_t offset,
								  uint64_t size);
static void i915_gem_object_set_to_full_cpu_read_domain(struct drm_i915_gem_object *obj);
static __must_check int i915_gem_object_bind_to_gtt(struct drm_i915_gem_object *obj,
						    unsigned alignment,
						    bool map_and_fenceable);
static void i915_gem_clear_fence_reg(struct drm_device *dev,
				     struct drm_i915_fence_reg *reg);
static int i915_gem_phys_pwrite(struct drm_device *dev,
				struct drm_i915_gem_object *obj,
				struct drm_i915_gem_pwrite *args,
				struct drm_file *file);
static void i915_gem_free_object_tail(struct drm_i915_gem_object *obj);

static int i915_gem_inactive_shrink(struct shrinker *shrinker,
				    struct shrink_control *sc);
static void i915_gem_object_truncate(struct drm_i915_gem_object *obj);

/* some bookkeeping */
static void i915_gem_info_add_obj(struct drm_i915_private *dev_priv,
				  size_t size)
{
	dev_priv->mm.object_count++;
	dev_priv->mm.object_memory += size;
}

static void i915_gem_info_remove_obj(struct drm_i915_private *dev_priv,
				     size_t size)
{
	dev_priv->mm.object_count--;
	dev_priv->mm.object_memory -= size;
}

static int
i915_gem_wait_for_error(struct drm_device *dev)
{
	struct drm_i915_private *dev_priv = dev->dev_private;
	struct completion *x = &dev_priv->error_completion;
	unsigned long flags;
	int ret;

	if (!atomic_read(&dev_priv->mm.wedged))
		return 0;

	ret = wait_for_completion_interruptible(x);
	if (ret)
		return ret;

	if (atomic_read(&dev_priv->mm.wedged)) {
		/* GPU is hung, bump the completion count to account for
		 * the token we just consumed so that we never hit zero and
		 * end up waiting upon a subsequent completion event that
		 * will never happen.
		 */
		spin_lock_irqsave(&x->wait.lock, flags);
		x->done++;
		spin_unlock_irqrestore(&x->wait.lock, flags);
	}
	return 0;
}

int i915_mutex_lock_interruptible(struct drm_device *dev)
{
	int ret;

	ret = i915_gem_wait_for_error(dev);
	if (ret)
		return ret;

	ret = mutex_lock_interruptible(&dev->struct_mutex);
	if (ret)
		return ret;

	WARN_ON(i915_verify_lists(dev));
	return 0;
}

static inline bool
i915_gem_object_is_inactive(struct drm_i915_gem_object *obj)
{
	return obj->gtt_space && !obj->active && obj->pin_count == 0;
}

void i915_gem_do_init(struct drm_device *dev,
		      unsigned long start,
		      unsigned long mappable_end,
		      unsigned long end)
{
	drm_i915_private_t *dev_priv = dev->dev_private;

	drm_mm_init(&dev_priv->mm.gtt_space, start, end - start);

	dev_priv->mm.gtt_start = start;
	dev_priv->mm.gtt_mappable_end = mappable_end;
	dev_priv->mm.gtt_end = end;
	dev_priv->mm.gtt_total = end - start;
	dev_priv->mm.mappable_gtt_total = min(end, mappable_end) - start;

	/* Take over this portion of the GTT */
	intel_gtt_clear_range(start / PAGE_SIZE, (end-start) / PAGE_SIZE);
}

int
i915_gem_init_ioctl(struct drm_device *dev, void *data,
		    struct drm_file *file)
{
	struct drm_i915_gem_init *args = data;

	if (args->gtt_start >= args->gtt_end ||
	    (args->gtt_end | args->gtt_start) & (PAGE_SIZE - 1))
		return -EINVAL;

	mutex_lock(&dev->struct_mutex);
	i915_gem_do_init(dev, args->gtt_start, args->gtt_end, args->gtt_end);
	mutex_unlock(&dev->struct_mutex);

	return 0;
=======
#include <linux/dma-fence-array.h>
#include <linux/kthread.h>
#include <linux/dma-resv.h>
#include <linux/shmem_fs.h>
#include <linux/slab.h>
#include <linux/stop_machine.h>
#include <linux/swap.h>
#include <linux/pci.h>
#include <linux/dma-buf.h>
#include <linux/mman.h>

#include <drm/drm_cache.h>
#include <drm/drm_vma_manager.h>

#include "display/intel_display.h"

#include "gem/i915_gem_clflush.h"
#include "gem/i915_gem_context.h"
#include "gem/i915_gem_ioctls.h"
#include "gem/i915_gem_mman.h"
#include "gem/i915_gem_object_frontbuffer.h"
#include "gem/i915_gem_pm.h"
#include "gem/i915_gem_region.h"
#include "gt/intel_engine_user.h"
#include "gt/intel_gt.h"
#include "gt/intel_gt_pm.h"
#include "gt/intel_workarounds.h"

#include "i915_drv.h"
#include "i915_file_private.h"
#include "i915_trace.h"
#include "i915_vgpu.h"
#include "intel_clock_gating.h"

static int
insert_mappable_node(struct i915_ggtt *ggtt, struct drm_mm_node *node, u32 size)
{
	int err;

	err = mutex_lock_interruptible(&ggtt->vm.mutex);
	if (err)
		return err;

	memset(node, 0, sizeof(*node));
	err = drm_mm_insert_node_in_range(&ggtt->vm.mm, node,
					  size, 0, I915_COLOR_UNEVICTABLE,
					  0, ggtt->mappable_end,
					  DRM_MM_INSERT_LOW);

	mutex_unlock(&ggtt->vm.mutex);

	return err;
}

static void
remove_mappable_node(struct i915_ggtt *ggtt, struct drm_mm_node *node)
{
	mutex_lock(&ggtt->vm.mutex);
	drm_mm_remove_node(node);
	mutex_unlock(&ggtt->vm.mutex);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

int
i915_gem_get_aperture_ioctl(struct drm_device *dev, void *data,
			    struct drm_file *file)
{
<<<<<<< HEAD
	struct drm_i915_private *dev_priv = dev->dev_private;
	struct drm_i915_gem_get_aperture *args = data;
	struct drm_i915_gem_object *obj;
	size_t pinned;

	if (!(dev->driver->driver_features & DRIVER_GEM))
		return -ENODEV;

	pinned = 0;
	mutex_lock(&dev->struct_mutex);
	list_for_each_entry(obj, &dev_priv->mm.pinned_list, mm_list)
		pinned += obj->gtt_space->size;
	mutex_unlock(&dev->struct_mutex);

	args->aper_size = dev_priv->mm.gtt_total;
=======
	struct drm_i915_private *i915 = to_i915(dev);
	struct i915_ggtt *ggtt = to_gt(i915)->ggtt;
	struct drm_i915_gem_get_aperture *args = data;
	struct i915_vma *vma;
	u64 pinned;

	if (mutex_lock_interruptible(&ggtt->vm.mutex))
		return -EINTR;

	pinned = ggtt->vm.reserved;
	list_for_each_entry(vma, &ggtt->vm.bound_list, vm_link)
		if (i915_vma_is_pinned(vma))
			pinned += vma->node.size;

	mutex_unlock(&ggtt->vm.mutex);

	args->aper_size = ggtt->vm.total;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	args->aper_available_size = args->aper_size - pinned;

	return 0;
}

<<<<<<< HEAD
static int
i915_gem_create(struct drm_file *file,
		struct drm_device *dev,
		uint64_t size,
		uint32_t *handle_p)
{
	struct drm_i915_gem_object *obj;
	int ret;
	u32 handle;

	size = roundup(size, PAGE_SIZE);
	if (size == 0)
		return -EINVAL;

	/* Allocate the new object */
	obj = i915_gem_alloc_object(dev, size);
	if (obj == NULL)
		return -ENOMEM;

	ret = drm_gem_handle_create(file, &obj->base, &handle);
	if (ret) {
		drm_gem_object_release(&obj->base);
		i915_gem_info_remove_obj(dev->dev_private, obj->base.size);
		kfree(obj);
		return ret;
	}

	/* drop reference from allocate - handle holds it now */
	drm_gem_object_unreference(&obj->base);
	trace_i915_gem_object_create(obj);

	*handle_p = handle;
	return 0;
}

int
i915_gem_dumb_create(struct drm_file *file,
		     struct drm_device *dev,
		     struct drm_mode_create_dumb *args)
{
	/* have to work out size/pitch and return them */
	args->pitch = ALIGN(args->width * ((args->bpp + 7) / 8), 64);
	args->size = args->pitch * args->height;
	return i915_gem_create(file, dev,
			       args->size, &args->handle);
}

int i915_gem_dumb_destroy(struct drm_file *file,
			  struct drm_device *dev,
			  uint32_t handle)
{
	return drm_gem_handle_delete(file, handle);
}

/**
 * Creates a new mm object and returns a handle to it.
 */
int
i915_gem_create_ioctl(struct drm_device *dev, void *data,
		      struct drm_file *file)
{
	struct drm_i915_gem_create *args = data;
	return i915_gem_create(file, dev,
			       args->size, &args->handle);
}

static int i915_gem_object_needs_bit17_swizzle(struct drm_i915_gem_object *obj)
{
	drm_i915_private_t *dev_priv = obj->base.dev->dev_private;

	return dev_priv->mm.bit_6_swizzle_x == I915_BIT_6_SWIZZLE_9_10_17 &&
		obj->tiling_mode != I915_TILING_NONE;
}

/**
 * This is the fast shmem pread path, which attempts to copy_from_user directly
 * from the backing pages of the object to the user's address space.  On a
 * fault, it fails so we can fall back to i915_gem_shmem_pwrite_slow().
 */
static int
i915_gem_shmem_pread_fast(struct drm_device *dev,
			  struct drm_i915_gem_object *obj,
			  struct drm_i915_gem_pread *args,
			  struct drm_file *file)
{
	struct address_space *mapping = obj->base.filp->f_path.dentry->d_inode->i_mapping;
	ssize_t remain;
	loff_t offset;
	char __user *user_data;
	int page_offset, page_length;

	user_data = (char __user *) (uintptr_t) args->data_ptr;
	remain = args->size;

	offset = args->offset;

	while (remain > 0) {
		struct page *page;
		char *vaddr;
		int ret;

		/* Operation in this page
		 *
		 * page_offset = offset within page
		 * page_length = bytes to copy for this page
		 */
		page_offset = offset_in_page(offset);
		page_length = remain;
		if ((page_offset + remain) > PAGE_SIZE)
			page_length = PAGE_SIZE - page_offset;

		page = shmem_read_mapping_page(mapping, offset >> PAGE_SHIFT);
		if (IS_ERR(page))
			return PTR_ERR(page);

		vaddr = kmap_atomic(page);
		ret = __copy_to_user_inatomic(user_data,
					      vaddr + page_offset,
					      page_length);
		kunmap_atomic(vaddr);

		mark_page_accessed(page);
		page_cache_release(page);
		if (ret)
			return -EFAULT;

		remain -= page_length;
		user_data += page_length;
		offset += page_length;
	}

	return 0;
}

static inline int
__copy_to_user_swizzled(char __user *cpu_vaddr,
			const char *gpu_vaddr, int gpu_offset,
			int length)
{
	int ret, cpu_offset = 0;

	while (length > 0) {
		int cacheline_end = ALIGN(gpu_offset + 1, 64);
		int this_length = min(cacheline_end - gpu_offset, length);
		int swizzled_gpu_offset = gpu_offset ^ 64;

		ret = __copy_to_user(cpu_vaddr + cpu_offset,
				     gpu_vaddr + swizzled_gpu_offset,
				     this_length);
		if (ret)
			return ret + length;

		cpu_offset += this_length;
		gpu_offset += this_length;
		length -= this_length;
	}

	return 0;
}

static inline int
__copy_from_user_swizzled(char __user *gpu_vaddr, int gpu_offset,
			  const char *cpu_vaddr,
			  int length)
{
	int ret, cpu_offset = 0;

	while (length > 0) {
		int cacheline_end = ALIGN(gpu_offset + 1, 64);
		int this_length = min(cacheline_end - gpu_offset, length);
		int swizzled_gpu_offset = gpu_offset ^ 64;

		ret = __copy_from_user(gpu_vaddr + swizzled_gpu_offset,
				       cpu_vaddr + cpu_offset,
				       this_length);
		if (ret)
			return ret + length;

		cpu_offset += this_length;
		gpu_offset += this_length;
		length -= this_length;
	}

	return 0;
}

/**
 * This is the fallback shmem pread path, which allocates temporary storage
 * in kernel space to copy_to_user into outside of the struct_mutex, so we
 * can copy out of the object's backing pages while holding the struct mutex
 * and not take page faults.
 */
static int
i915_gem_shmem_pread_slow(struct drm_device *dev,
			  struct drm_i915_gem_object *obj,
			  struct drm_i915_gem_pread *args,
			  struct drm_file *file)
{
	struct address_space *mapping = obj->base.filp->f_path.dentry->d_inode->i_mapping;
	char __user *user_data;
	ssize_t remain;
	loff_t offset;
	int shmem_page_offset, page_length, ret;
	int obj_do_bit17_swizzling, page_do_bit17_swizzling;

	user_data = (char __user *) (uintptr_t) args->data_ptr;
	remain = args->size;

	obj_do_bit17_swizzling = i915_gem_object_needs_bit17_swizzle(obj);

	offset = args->offset;

	mutex_unlock(&dev->struct_mutex);

	while (remain > 0) {
		struct page *page;
		char *vaddr;

		/* Operation in this page
		 *
		 * shmem_page_offset = offset within page in shmem file
		 * page_length = bytes to copy for this page
		 */
		shmem_page_offset = offset_in_page(offset);
		page_length = remain;
		if ((shmem_page_offset + page_length) > PAGE_SIZE)
			page_length = PAGE_SIZE - shmem_page_offset;

		page = shmem_read_mapping_page(mapping, offset >> PAGE_SHIFT);
		if (IS_ERR(page)) {
			ret = PTR_ERR(page);
			goto out;
		}

		page_do_bit17_swizzling = obj_do_bit17_swizzling &&
			(page_to_phys(page) & (1 << 17)) != 0;

		vaddr = kmap(page);
		if (page_do_bit17_swizzling)
			ret = __copy_to_user_swizzled(user_data,
						      vaddr, shmem_page_offset,
						      page_length);
		else
			ret = __copy_to_user(user_data,
					     vaddr + shmem_page_offset,
					     page_length);
		kunmap(page);

		mark_page_accessed(page);
		page_cache_release(page);

		if (ret) {
			ret = -EFAULT;
			goto out;
		}

		remain -= page_length;
		user_data += page_length;
		offset += page_length;
	}

out:
	mutex_lock(&dev->struct_mutex);
	/* Fixup: Kill any reinstated backing storage pages */
	if (obj->madv == __I915_MADV_PURGED)
		i915_gem_object_truncate(obj);

	return ret;
}

/**
 * Reads data from the object referenced by handle.
 *
 * On error, the contents of *data are undefined.
 */
int
i915_gem_pread_ioctl(struct drm_device *dev, void *data,
		     struct drm_file *file)
{
	struct drm_i915_gem_pread *args = data;
	struct drm_i915_gem_object *obj;
	int ret = 0;

	if (args->size == 0)
		return 0;

	if (!access_ok(VERIFY_WRITE,
		       (char __user *)(uintptr_t)args->data_ptr,
		       args->size))
		return -EFAULT;

	ret = fault_in_pages_writeable((char __user *)(uintptr_t)args->data_ptr,
				       args->size);
	if (ret)
		return -EFAULT;

	ret = i915_mutex_lock_interruptible(dev);
	if (ret)
		return ret;

	obj = to_intel_bo(drm_gem_object_lookup(dev, file, args->handle));
	if (&obj->base == NULL) {
		ret = -ENOENT;
		goto unlock;
	}

	/* Bounds check source.  */
	if (args->offset > obj->base.size ||
	    args->size > obj->base.size - args->offset) {
		ret = -EINVAL;
		goto out;
	}

	trace_i915_gem_object_pread(obj, args->offset, args->size);

	ret = i915_gem_object_set_cpu_read_domain_range(obj,
							args->offset,
							args->size);
	if (ret)
		goto out;

	ret = -EFAULT;
	if (!i915_gem_object_needs_bit17_swizzle(obj))
		ret = i915_gem_shmem_pread_fast(dev, obj, args, file);
	if (ret == -EFAULT)
		ret = i915_gem_shmem_pread_slow(dev, obj, args, file);

out:
	drm_gem_object_unreference(&obj->base);
unlock:
	mutex_unlock(&dev->struct_mutex);
	return ret;
}

/* This is the fast write path which cannot handle
 * page faults in the source data
 */

static inline int
fast_user_write(struct io_mapping *mapping,
		loff_t page_base, int page_offset,
		char __user *user_data,
		int length)
{
	char *vaddr_atomic;
	unsigned long unwritten;

	vaddr_atomic = io_mapping_map_atomic_wc(mapping, page_base);
	unwritten = __copy_from_user_inatomic_nocache(vaddr_atomic + page_offset,
						      user_data, length);
	io_mapping_unmap_atomic(vaddr_atomic);
	return unwritten;
}

/* Here's the write path which can sleep for
 * page faults
 */

static inline void
slow_kernel_write(struct io_mapping *mapping,
		  loff_t gtt_base, int gtt_offset,
		  struct page *user_page, int user_offset,
		  int length)
{
	char __iomem *dst_vaddr;
	char *src_vaddr;

	dst_vaddr = io_mapping_map_wc(mapping, gtt_base);
	src_vaddr = kmap(user_page);

	memcpy_toio(dst_vaddr + gtt_offset,
		    src_vaddr + user_offset,
		    length);

	kunmap(user_page);
	io_mapping_unmap(dst_vaddr);
}

/**
 * This is the fast pwrite path, where we copy the data directly from the
 * user into the GTT, uncached.
 */
static int
i915_gem_gtt_pwrite_fast(struct drm_device *dev,
			 struct drm_i915_gem_object *obj,
			 struct drm_i915_gem_pwrite *args,
			 struct drm_file *file)
{
	drm_i915_private_t *dev_priv = dev->dev_private;
	ssize_t remain;
	loff_t offset, page_base;
	char __user *user_data;
	int page_offset, page_length;

	user_data = (char __user *) (uintptr_t) args->data_ptr;
	remain = args->size;

	offset = obj->gtt_offset + args->offset;
=======
int i915_gem_object_unbind(struct drm_i915_gem_object *obj,
			   unsigned long flags)
{
	struct intel_runtime_pm *rpm = &to_i915(obj->base.dev)->runtime_pm;
	bool vm_trylock = !!(flags & I915_GEM_OBJECT_UNBIND_VM_TRYLOCK);
	LIST_HEAD(still_in_list);
	intel_wakeref_t wakeref;
	struct i915_vma *vma;
	int ret;

	assert_object_held(obj);

	if (list_empty(&obj->vma.list))
		return 0;

	/*
	 * As some machines use ACPI to handle runtime-resume callbacks, and
	 * ACPI is quite kmalloc happy, we cannot resume beneath the vm->mutex
	 * as they are required by the shrinker. Ergo, we wake the device up
	 * first just in case.
	 */
	wakeref = intel_runtime_pm_get(rpm);

try_again:
	ret = 0;
	spin_lock(&obj->vma.lock);
	while (!ret && (vma = list_first_entry_or_null(&obj->vma.list,
						       struct i915_vma,
						       obj_link))) {
		list_move_tail(&vma->obj_link, &still_in_list);
		if (!i915_vma_is_bound(vma, I915_VMA_BIND_MASK))
			continue;

		if (flags & I915_GEM_OBJECT_UNBIND_TEST) {
			ret = -EBUSY;
			break;
		}

		/*
		 * Requiring the vm destructor to take the object lock
		 * before destroying a vma would help us eliminate the
		 * i915_vm_tryget() here, AND thus also the barrier stuff
		 * at the end. That's an easy fix, but sleeping locks in
		 * a kthread should generally be avoided.
		 */
		ret = -EAGAIN;
		if (!i915_vm_tryget(vma->vm))
			break;

		spin_unlock(&obj->vma.lock);

		/*
		 * Since i915_vma_parked() takes the object lock
		 * before vma destruction, it won't race us here,
		 * and destroy the vma from under us.
		 */

		ret = -EBUSY;
		if (flags & I915_GEM_OBJECT_UNBIND_ASYNC) {
			assert_object_held(vma->obj);
			ret = i915_vma_unbind_async(vma, vm_trylock);
		}

		if (ret == -EBUSY && (flags & I915_GEM_OBJECT_UNBIND_ACTIVE ||
				      !i915_vma_is_active(vma))) {
			if (vm_trylock) {
				if (mutex_trylock(&vma->vm->mutex)) {
					ret = __i915_vma_unbind(vma);
					mutex_unlock(&vma->vm->mutex);
				}
			} else {
				ret = i915_vma_unbind(vma);
			}
		}

		i915_vm_put(vma->vm);
		spin_lock(&obj->vma.lock);
	}
	list_splice_init(&still_in_list, &obj->vma.list);
	spin_unlock(&obj->vma.lock);

	if (ret == -EAGAIN && flags & I915_GEM_OBJECT_UNBIND_BARRIER) {
		rcu_barrier(); /* flush the i915_vm_release() */
		goto try_again;
	}

	intel_runtime_pm_put(rpm, wakeref);

	return ret;
}

static int
shmem_pread(struct page *page, int offset, int len, char __user *user_data,
	    bool needs_clflush)
{
	char *vaddr;
	int ret;

	vaddr = kmap(page);

	if (needs_clflush)
		drm_clflush_virt_range(vaddr + offset, len);

	ret = __copy_to_user(user_data, vaddr + offset, len);

	kunmap(page);

	return ret ? -EFAULT : 0;
}

static int
i915_gem_shmem_pread(struct drm_i915_gem_object *obj,
		     struct drm_i915_gem_pread *args)
{
	unsigned int needs_clflush;
	char __user *user_data;
	unsigned long offset;
	pgoff_t idx;
	u64 remain;
	int ret;

	ret = i915_gem_object_lock_interruptible(obj, NULL);
	if (ret)
		return ret;

	ret = i915_gem_object_pin_pages(obj);
	if (ret)
		goto err_unlock;

	ret = i915_gem_object_prepare_read(obj, &needs_clflush);
	if (ret)
		goto err_unpin;

	i915_gem_object_finish_access(obj);
	i915_gem_object_unlock(obj);

	remain = args->size;
	user_data = u64_to_user_ptr(args->data_ptr);
	offset = offset_in_page(args->offset);
	for (idx = args->offset >> PAGE_SHIFT; remain; idx++) {
		struct page *page = i915_gem_object_get_page(obj, idx);
		unsigned int length = min_t(u64, remain, PAGE_SIZE - offset);

		ret = shmem_pread(page, offset, length, user_data,
				  needs_clflush);
		if (ret)
			break;

		remain -= length;
		user_data += length;
		offset = 0;
	}

	i915_gem_object_unpin_pages(obj);
	return ret;

err_unpin:
	i915_gem_object_unpin_pages(obj);
err_unlock:
	i915_gem_object_unlock(obj);
	return ret;
}

static inline bool
gtt_user_read(struct io_mapping *mapping,
	      loff_t base, int offset,
	      char __user *user_data, int length)
{
	void __iomem *vaddr;
	unsigned long unwritten;

	/* We can use the cpu mem copy function because this is X86. */
	vaddr = io_mapping_map_atomic_wc(mapping, base);
	unwritten = __copy_to_user_inatomic(user_data,
					    (void __force *)vaddr + offset,
					    length);
	io_mapping_unmap_atomic(vaddr);
	if (unwritten) {
		vaddr = io_mapping_map_wc(mapping, base, PAGE_SIZE);
		unwritten = copy_to_user(user_data,
					 (void __force *)vaddr + offset,
					 length);
		io_mapping_unmap(vaddr);
	}
	return unwritten;
}

static struct i915_vma *i915_gem_gtt_prepare(struct drm_i915_gem_object *obj,
					     struct drm_mm_node *node,
					     bool write)
{
	struct drm_i915_private *i915 = to_i915(obj->base.dev);
	struct i915_ggtt *ggtt = to_gt(i915)->ggtt;
	struct i915_vma *vma;
	struct i915_gem_ww_ctx ww;
	int ret;

	i915_gem_ww_ctx_init(&ww, true);
retry:
	vma = ERR_PTR(-ENODEV);
	ret = i915_gem_object_lock(obj, &ww);
	if (ret)
		goto err_ww;

	ret = i915_gem_object_set_to_gtt_domain(obj, write);
	if (ret)
		goto err_ww;

	if (!i915_gem_object_is_tiled(obj))
		vma = i915_gem_object_ggtt_pin_ww(obj, &ww, NULL, 0, 0,
						  PIN_MAPPABLE |
						  PIN_NONBLOCK /* NOWARN */ |
						  PIN_NOEVICT);
	if (vma == ERR_PTR(-EDEADLK)) {
		ret = -EDEADLK;
		goto err_ww;
	} else if (!IS_ERR(vma)) {
		node->start = i915_ggtt_offset(vma);
		node->flags = 0;
	} else {
		ret = insert_mappable_node(ggtt, node, PAGE_SIZE);
		if (ret)
			goto err_ww;
		GEM_BUG_ON(!drm_mm_node_allocated(node));
		vma = NULL;
	}

	ret = i915_gem_object_pin_pages(obj);
	if (ret) {
		if (drm_mm_node_allocated(node)) {
			ggtt->vm.clear_range(&ggtt->vm, node->start, node->size);
			remove_mappable_node(ggtt, node);
		} else {
			i915_vma_unpin(vma);
		}
	}

err_ww:
	if (ret == -EDEADLK) {
		ret = i915_gem_ww_ctx_backoff(&ww);
		if (!ret)
			goto retry;
	}
	i915_gem_ww_ctx_fini(&ww);

	return ret ? ERR_PTR(ret) : vma;
}

static void i915_gem_gtt_cleanup(struct drm_i915_gem_object *obj,
				 struct drm_mm_node *node,
				 struct i915_vma *vma)
{
	struct drm_i915_private *i915 = to_i915(obj->base.dev);
	struct i915_ggtt *ggtt = to_gt(i915)->ggtt;

	i915_gem_object_unpin_pages(obj);
	if (drm_mm_node_allocated(node)) {
		ggtt->vm.clear_range(&ggtt->vm, node->start, node->size);
		remove_mappable_node(ggtt, node);
	} else {
		i915_vma_unpin(vma);
	}
}

static int
i915_gem_gtt_pread(struct drm_i915_gem_object *obj,
		   const struct drm_i915_gem_pread *args)
{
	struct drm_i915_private *i915 = to_i915(obj->base.dev);
	struct i915_ggtt *ggtt = to_gt(i915)->ggtt;
	unsigned long remain, offset;
	intel_wakeref_t wakeref;
	struct drm_mm_node node;
	void __user *user_data;
	struct i915_vma *vma;
	int ret = 0;

	if (overflows_type(args->size, remain) ||
	    overflows_type(args->offset, offset))
		return -EINVAL;

	wakeref = intel_runtime_pm_get(&i915->runtime_pm);

	vma = i915_gem_gtt_prepare(obj, &node, false);
	if (IS_ERR(vma)) {
		ret = PTR_ERR(vma);
		goto out_rpm;
	}

	user_data = u64_to_user_ptr(args->data_ptr);
	remain = args->size;
	offset = args->offset;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	while (remain > 0) {
		/* Operation in this page
		 *
		 * page_base = page offset within aperture
		 * page_offset = offset within page
		 * page_length = bytes to copy for this page
		 */
<<<<<<< HEAD
		page_base = offset & PAGE_MASK;
		page_offset = offset_in_page(offset);
		page_length = remain;
		if ((page_offset + remain) > PAGE_SIZE)
			page_length = PAGE_SIZE - page_offset;

		/* If we get a fault while copying data, then (presumably) our
		 * source page isn't available.  Return the error and we'll
		 * retry in the slow path.
		 */
		if (fast_user_write(dev_priv->mm.gtt_mapping, page_base,
				    page_offset, user_data, page_length))
			return -EFAULT;
=======
		u32 page_base = node.start;
		unsigned page_offset = offset_in_page(offset);
		unsigned page_length = PAGE_SIZE - page_offset;
		page_length = remain < page_length ? remain : page_length;
		if (drm_mm_node_allocated(&node)) {
			ggtt->vm.insert_page(&ggtt->vm,
					     i915_gem_object_get_dma_address(obj,
									     offset >> PAGE_SHIFT),
					     node.start,
					     i915_gem_get_pat_index(i915,
								    I915_CACHE_NONE), 0);
		} else {
			page_base += offset & PAGE_MASK;
		}

		if (gtt_user_read(&ggtt->iomap, page_base, page_offset,
				  user_data, page_length)) {
			ret = -EFAULT;
			break;
		}
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

		remain -= page_length;
		user_data += page_length;
		offset += page_length;
	}

<<<<<<< HEAD
	return 0;
}

/**
 * This is the fallback GTT pwrite path, which uses get_user_pages to pin
 * the memory and maps it using kmap_atomic for copying.
 *
 * This code resulted in x11perf -rgb10text consuming about 10% more CPU
 * than using i915_gem_gtt_pwrite_fast on a G45 (32-bit).
 */
static int
i915_gem_gtt_pwrite_slow(struct drm_device *dev,
			 struct drm_i915_gem_object *obj,
			 struct drm_i915_gem_pwrite *args,
			 struct drm_file *file)
{
	drm_i915_private_t *dev_priv = dev->dev_private;
	ssize_t remain;
	loff_t gtt_page_base, offset;
	loff_t first_data_page, last_data_page, num_pages;
	loff_t pinned_pages, i;
	struct page **user_pages;
	struct mm_struct *mm = current->mm;
	int gtt_page_offset, data_page_offset, data_page_index, page_length;
	int ret;
	uint64_t data_ptr = args->data_ptr;

	remain = args->size;

	/* Pin the user pages containing the data.  We can't fault while
	 * holding the struct mutex, and all of the pwrite implementations
	 * want to hold it while dereferencing the user data.
	 */
	first_data_page = data_ptr / PAGE_SIZE;
	last_data_page = (data_ptr + args->size - 1) / PAGE_SIZE;
	num_pages = last_data_page - first_data_page + 1;

	user_pages = drm_malloc_ab(num_pages, sizeof(struct page *));
	if (user_pages == NULL)
		return -ENOMEM;

	mutex_unlock(&dev->struct_mutex);
	down_read(&mm->mmap_sem);
	pinned_pages = get_user_pages(current, mm, (uintptr_t)args->data_ptr,
				      num_pages, 0, 0, user_pages, NULL);
	up_read(&mm->mmap_sem);
	mutex_lock(&dev->struct_mutex);
	if (pinned_pages < num_pages) {
		ret = -EFAULT;
		goto out_unpin_pages;
	}

	ret = i915_gem_object_set_to_gtt_domain(obj, true);
	if (ret)
		goto out_unpin_pages;

	ret = i915_gem_object_put_fence(obj);
	if (ret)
		goto out_unpin_pages;

	offset = obj->gtt_offset + args->offset;

	while (remain > 0) {
		/* Operation in this page
		 *
		 * gtt_page_base = page offset within aperture
		 * gtt_page_offset = offset within page in aperture
		 * data_page_index = page number in get_user_pages return
		 * data_page_offset = offset with data_page_index page.
		 * page_length = bytes to copy for this page
		 */
		gtt_page_base = offset & PAGE_MASK;
		gtt_page_offset = offset_in_page(offset);
		data_page_index = data_ptr / PAGE_SIZE - first_data_page;
		data_page_offset = offset_in_page(data_ptr);

		page_length = remain;
		if ((gtt_page_offset + page_length) > PAGE_SIZE)
			page_length = PAGE_SIZE - gtt_page_offset;
		if ((data_page_offset + page_length) > PAGE_SIZE)
			page_length = PAGE_SIZE - data_page_offset;

		slow_kernel_write(dev_priv->mm.gtt_mapping,
				  gtt_page_base, gtt_page_offset,
				  user_pages[data_page_index],
				  data_page_offset,
				  page_length);

		remain -= page_length;
		offset += page_length;
		data_ptr += page_length;
	}

out_unpin_pages:
	for (i = 0; i < pinned_pages; i++)
		page_cache_release(user_pages[i]);
	drm_free_large(user_pages);

=======
	i915_gem_gtt_cleanup(obj, &node, vma);
out_rpm:
	intel_runtime_pm_put(&i915->runtime_pm, wakeref);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	return ret;
}

/**
<<<<<<< HEAD
 * This is the fast shmem pwrite path, which attempts to directly
 * copy_from_user into the kmapped pages backing the object.
 */
static int
i915_gem_shmem_pwrite_fast(struct drm_device *dev,
			   struct drm_i915_gem_object *obj,
			   struct drm_i915_gem_pwrite *args,
			   struct drm_file *file)
{
	struct address_space *mapping = obj->base.filp->f_path.dentry->d_inode->i_mapping;
	ssize_t remain;
	loff_t offset;
	char __user *user_data;
	int page_offset, page_length;

	user_data = (char __user *) (uintptr_t) args->data_ptr;
	remain = args->size;

	offset = args->offset;
	obj->dirty = 1;

	while (remain > 0) {
		struct page *page;
		char *vaddr;
		int ret;

		/* Operation in this page
		 *
		 * page_offset = offset within page
		 * page_length = bytes to copy for this page
		 */
		page_offset = offset_in_page(offset);
		page_length = remain;
		if ((page_offset + remain) > PAGE_SIZE)
			page_length = PAGE_SIZE - page_offset;

		page = shmem_read_mapping_page(mapping, offset >> PAGE_SHIFT);
		if (IS_ERR(page))
			return PTR_ERR(page);

		vaddr = kmap_atomic(page);
		ret = __copy_from_user_inatomic(vaddr + page_offset,
						user_data,
						page_length);
		kunmap_atomic(vaddr);

		set_page_dirty(page);
		mark_page_accessed(page);
		page_cache_release(page);

		/* If we get a fault while copying data, then (presumably) our
		 * source page isn't available.  Return the error and we'll
		 * retry in the slow path.
		 */
		if (ret)
			return -EFAULT;
=======
 * i915_gem_pread_ioctl - Reads data from the object referenced by handle.
 * @dev: drm device pointer
 * @data: ioctl data blob
 * @file: drm file pointer
 *
 * On error, the contents of *data are undefined.
 */
int
i915_gem_pread_ioctl(struct drm_device *dev, void *data,
		     struct drm_file *file)
{
	struct drm_i915_private *i915 = to_i915(dev);
	struct drm_i915_gem_pread *args = data;
	struct drm_i915_gem_object *obj;
	int ret;

	/* PREAD is disallowed for all platforms after TGL-LP.  This also
	 * covers all platforms with local memory.
	 */
	if (GRAPHICS_VER(i915) >= 12 && !IS_TIGERLAKE(i915))
		return -EOPNOTSUPP;

	if (args->size == 0)
		return 0;

	if (!access_ok(u64_to_user_ptr(args->data_ptr),
		       args->size))
		return -EFAULT;

	obj = i915_gem_object_lookup(file, args->handle);
	if (!obj)
		return -ENOENT;

	/* Bounds check source.  */
	if (range_overflows_t(u64, args->offset, args->size, obj->base.size)) {
		ret = -EINVAL;
		goto out;
	}

	trace_i915_gem_object_pread(obj, args->offset, args->size);
	ret = -ENODEV;
	if (obj->ops->pread)
		ret = obj->ops->pread(obj, args);
	if (ret != -ENODEV)
		goto out;

	ret = i915_gem_object_wait(obj,
				   I915_WAIT_INTERRUPTIBLE,
				   MAX_SCHEDULE_TIMEOUT);
	if (ret)
		goto out;

	ret = i915_gem_shmem_pread(obj, args);
	if (ret == -EFAULT || ret == -ENODEV)
		ret = i915_gem_gtt_pread(obj, args);

out:
	i915_gem_object_put(obj);
	return ret;
}

/* This is the fast write path which cannot handle
 * page faults in the source data
 */

static inline bool
ggtt_write(struct io_mapping *mapping,
	   loff_t base, int offset,
	   char __user *user_data, int length)
{
	void __iomem *vaddr;
	unsigned long unwritten;

	/* We can use the cpu mem copy function because this is X86. */
	vaddr = io_mapping_map_atomic_wc(mapping, base);
	unwritten = __copy_from_user_inatomic_nocache((void __force *)vaddr + offset,
						      user_data, length);
	io_mapping_unmap_atomic(vaddr);
	if (unwritten) {
		vaddr = io_mapping_map_wc(mapping, base, PAGE_SIZE);
		unwritten = copy_from_user((void __force *)vaddr + offset,
					   user_data, length);
		io_mapping_unmap(vaddr);
	}

	return unwritten;
}

/**
 * i915_gem_gtt_pwrite_fast - This is the fast pwrite path, where we copy the data directly from the
 * user into the GTT, uncached.
 * @obj: i915 GEM object
 * @args: pwrite arguments structure
 */
static int
i915_gem_gtt_pwrite_fast(struct drm_i915_gem_object *obj,
			 const struct drm_i915_gem_pwrite *args)
{
	struct drm_i915_private *i915 = to_i915(obj->base.dev);
	struct i915_ggtt *ggtt = to_gt(i915)->ggtt;
	struct intel_runtime_pm *rpm = &i915->runtime_pm;
	unsigned long remain, offset;
	intel_wakeref_t wakeref;
	struct drm_mm_node node;
	struct i915_vma *vma;
	void __user *user_data;
	int ret = 0;

	if (overflows_type(args->size, remain) ||
	    overflows_type(args->offset, offset))
		return -EINVAL;

	if (i915_gem_object_has_struct_page(obj)) {
		/*
		 * Avoid waking the device up if we can fallback, as
		 * waking/resuming is very slow (worst-case 10-100 ms
		 * depending on PCI sleeps and our own resume time).
		 * This easily dwarfs any performance advantage from
		 * using the cache bypass of indirect GGTT access.
		 */
		wakeref = intel_runtime_pm_get_if_in_use(rpm);
		if (!wakeref)
			return -EFAULT;
	} else {
		/* No backing pages, no fallback, we must force GGTT access */
		wakeref = intel_runtime_pm_get(rpm);
	}

	vma = i915_gem_gtt_prepare(obj, &node, true);
	if (IS_ERR(vma)) {
		ret = PTR_ERR(vma);
		goto out_rpm;
	}

	i915_gem_object_invalidate_frontbuffer(obj, ORIGIN_CPU);

	user_data = u64_to_user_ptr(args->data_ptr);
	offset = args->offset;
	remain = args->size;
	while (remain) {
		/* Operation in this page
		 *
		 * page_base = page offset within aperture
		 * page_offset = offset within page
		 * page_length = bytes to copy for this page
		 */
		u32 page_base = node.start;
		unsigned int page_offset = offset_in_page(offset);
		unsigned int page_length = PAGE_SIZE - page_offset;
		page_length = remain < page_length ? remain : page_length;
		if (drm_mm_node_allocated(&node)) {
			/* flush the write before we modify the GGTT */
			intel_gt_flush_ggtt_writes(ggtt->vm.gt);
			ggtt->vm.insert_page(&ggtt->vm,
					     i915_gem_object_get_dma_address(obj,
									     offset >> PAGE_SHIFT),
					     node.start,
					     i915_gem_get_pat_index(i915,
								    I915_CACHE_NONE), 0);
			wmb(); /* flush modifications to the GGTT (insert_page) */
		} else {
			page_base += offset & PAGE_MASK;
		}
		/* If we get a fault while copying data, then (presumably) our
		 * source page isn't available.  Return the error and we'll
		 * retry in the slow path.
		 * If the object is non-shmem backed, we retry again with the
		 * path that handles page fault.
		 */
		if (ggtt_write(&ggtt->iomap, page_base, page_offset,
			       user_data, page_length)) {
			ret = -EFAULT;
			break;
		}
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

		remain -= page_length;
		user_data += page_length;
		offset += page_length;
	}

<<<<<<< HEAD
	return 0;
}

/**
 * This is the fallback shmem pwrite path, which uses get_user_pages to pin
 * the memory and maps it using kmap_atomic for copying.
 *
 * This avoids taking mmap_sem for faulting on the user's address while the
 * struct_mutex is held.
 */
static int
i915_gem_shmem_pwrite_slow(struct drm_device *dev,
			   struct drm_i915_gem_object *obj,
			   struct drm_i915_gem_pwrite *args,
			   struct drm_file *file)
{
	struct address_space *mapping = obj->base.filp->f_path.dentry->d_inode->i_mapping;
	ssize_t remain;
	loff_t offset;
	char __user *user_data;
	int shmem_page_offset, page_length, ret;
	int obj_do_bit17_swizzling, page_do_bit17_swizzling;

	user_data = (char __user *) (uintptr_t) args->data_ptr;
	remain = args->size;

	obj_do_bit17_swizzling = i915_gem_object_needs_bit17_swizzle(obj);

	offset = args->offset;
	obj->dirty = 1;

	mutex_unlock(&dev->struct_mutex);

	while (remain > 0) {
		struct page *page;
		char *vaddr;

		/* Operation in this page
		 *
		 * shmem_page_offset = offset within page in shmem file
		 * page_length = bytes to copy for this page
		 */
		shmem_page_offset = offset_in_page(offset);

		page_length = remain;
		if ((shmem_page_offset + page_length) > PAGE_SIZE)
			page_length = PAGE_SIZE - shmem_page_offset;

		page = shmem_read_mapping_page(mapping, offset >> PAGE_SHIFT);
		if (IS_ERR(page)) {
			ret = PTR_ERR(page);
			goto out;
		}

		page_do_bit17_swizzling = obj_do_bit17_swizzling &&
			(page_to_phys(page) & (1 << 17)) != 0;

		vaddr = kmap(page);
		if (page_do_bit17_swizzling)
			ret = __copy_from_user_swizzled(vaddr, shmem_page_offset,
							user_data,
							page_length);
		else
			ret = __copy_from_user(vaddr + shmem_page_offset,
					       user_data,
					       page_length);
		kunmap(page);

		set_page_dirty(page);
		mark_page_accessed(page);
		page_cache_release(page);

		if (ret) {
			ret = -EFAULT;
			goto out;
		}

		remain -= page_length;
		user_data += page_length;
		offset += page_length;
	}

out:
	mutex_lock(&dev->struct_mutex);
	/* Fixup: Kill any reinstated backing storage pages */
	if (obj->madv == __I915_MADV_PURGED)
		i915_gem_object_truncate(obj);
	/* and flush dirty cachelines in case the object isn't in the cpu write
	 * domain anymore. */
	if (obj->base.write_domain != I915_GEM_DOMAIN_CPU) {
		i915_gem_clflush_object(obj);
		intel_gtt_chipset_flush();
	}

=======
	intel_gt_flush_ggtt_writes(ggtt->vm.gt);
	i915_gem_object_flush_frontbuffer(obj, ORIGIN_CPU);

	i915_gem_gtt_cleanup(obj, &node, vma);
out_rpm:
	intel_runtime_pm_put(rpm, wakeref);
	return ret;
}

/* Per-page copy function for the shmem pwrite fastpath.
 * Flushes invalid cachelines before writing to the target if
 * needs_clflush_before is set and flushes out any written cachelines after
 * writing if needs_clflush is set.
 */
static int
shmem_pwrite(struct page *page, int offset, int len, char __user *user_data,
	     bool needs_clflush_before,
	     bool needs_clflush_after)
{
	char *vaddr;
	int ret;

	vaddr = kmap(page);

	if (needs_clflush_before)
		drm_clflush_virt_range(vaddr + offset, len);

	ret = __copy_from_user(vaddr + offset, user_data, len);
	if (!ret && needs_clflush_after)
		drm_clflush_virt_range(vaddr + offset, len);

	kunmap(page);

	return ret ? -EFAULT : 0;
}

static int
i915_gem_shmem_pwrite(struct drm_i915_gem_object *obj,
		      const struct drm_i915_gem_pwrite *args)
{
	unsigned int partial_cacheline_write;
	unsigned int needs_clflush;
	void __user *user_data;
	unsigned long offset;
	pgoff_t idx;
	u64 remain;
	int ret;

	ret = i915_gem_object_lock_interruptible(obj, NULL);
	if (ret)
		return ret;

	ret = i915_gem_object_pin_pages(obj);
	if (ret)
		goto err_unlock;

	ret = i915_gem_object_prepare_write(obj, &needs_clflush);
	if (ret)
		goto err_unpin;

	i915_gem_object_finish_access(obj);
	i915_gem_object_unlock(obj);

	/* If we don't overwrite a cacheline completely we need to be
	 * careful to have up-to-date data by first clflushing. Don't
	 * overcomplicate things and flush the entire patch.
	 */
	partial_cacheline_write = 0;
	if (needs_clflush & CLFLUSH_BEFORE)
		partial_cacheline_write = boot_cpu_data.x86_clflush_size - 1;

	user_data = u64_to_user_ptr(args->data_ptr);
	remain = args->size;
	offset = offset_in_page(args->offset);
	for (idx = args->offset >> PAGE_SHIFT; remain; idx++) {
		struct page *page = i915_gem_object_get_page(obj, idx);
		unsigned int length = min_t(u64, remain, PAGE_SIZE - offset);

		ret = shmem_pwrite(page, offset, length, user_data,
				   (offset | length) & partial_cacheline_write,
				   needs_clflush & CLFLUSH_AFTER);
		if (ret)
			break;

		remain -= length;
		user_data += length;
		offset = 0;
	}

	i915_gem_object_flush_frontbuffer(obj, ORIGIN_CPU);

	i915_gem_object_unpin_pages(obj);
	return ret;

err_unpin:
	i915_gem_object_unpin_pages(obj);
err_unlock:
	i915_gem_object_unlock(obj);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	return ret;
}

/**
<<<<<<< HEAD
 * Writes data to the object referenced by handle.
=======
 * i915_gem_pwrite_ioctl - Writes data to the object referenced by handle.
 * @dev: drm device
 * @data: ioctl data blob
 * @file: drm file
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 *
 * On error, the contents of the buffer that were to be modified are undefined.
 */
int
i915_gem_pwrite_ioctl(struct drm_device *dev, void *data,
		      struct drm_file *file)
{
<<<<<<< HEAD
=======
	struct drm_i915_private *i915 = to_i915(dev);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	struct drm_i915_gem_pwrite *args = data;
	struct drm_i915_gem_object *obj;
	int ret;

<<<<<<< HEAD
	if (args->size == 0)
		return 0;

	if (!access_ok(VERIFY_READ,
		       (char __user *)(uintptr_t)args->data_ptr,
		       args->size))
		return -EFAULT;

	ret = fault_in_pages_readable((char __user *)(uintptr_t)args->data_ptr,
				      args->size);
	if (ret)
		return -EFAULT;

	ret = i915_mutex_lock_interruptible(dev);
	if (ret)
		return ret;

	obj = to_intel_bo(drm_gem_object_lookup(dev, file, args->handle));
	if (&obj->base == NULL) {
		ret = -ENOENT;
		goto unlock;
	}

	/* Bounds check destination. */
	if (args->offset > obj->base.size ||
	    args->size > obj->base.size - args->offset) {
		ret = -EINVAL;
		goto out;
=======
	/* PWRITE is disallowed for all platforms after TGL-LP.  This also
	 * covers all platforms with local memory.
	 */
	if (GRAPHICS_VER(i915) >= 12 && !IS_TIGERLAKE(i915))
		return -EOPNOTSUPP;

	if (args->size == 0)
		return 0;

	if (!access_ok(u64_to_user_ptr(args->data_ptr), args->size))
		return -EFAULT;

	obj = i915_gem_object_lookup(file, args->handle);
	if (!obj)
		return -ENOENT;

	/* Bounds check destination. */
	if (range_overflows_t(u64, args->offset, args->size, obj->base.size)) {
		ret = -EINVAL;
		goto err;
	}

	/* Writes not allowed into this read-only object */
	if (i915_gem_object_is_readonly(obj)) {
		ret = -EINVAL;
		goto err;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	}

	trace_i915_gem_object_pwrite(obj, args->offset, args->size);

<<<<<<< HEAD
=======
	ret = -ENODEV;
	if (obj->ops->pwrite)
		ret = obj->ops->pwrite(obj, args);
	if (ret != -ENODEV)
		goto err;

	ret = i915_gem_object_wait(obj,
				   I915_WAIT_INTERRUPTIBLE |
				   I915_WAIT_ALL,
				   MAX_SCHEDULE_TIMEOUT);
	if (ret)
		goto err;

	ret = -EFAULT;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	/* We can only do the GTT pwrite on untiled buffers, as otherwise
	 * it would end up going through the fenced access, and we'll get
	 * different detiling behavior between reading and writing.
	 * pread/pwrite currently are reading and writing from the CPU
	 * perspective, requiring manual detiling by the client.
	 */
<<<<<<< HEAD
	if (obj->phys_obj) {
		ret = i915_gem_phys_pwrite(dev, obj, args, file);
		goto out;
	}

	if (obj->gtt_space &&
	    obj->tiling_mode == I915_TILING_NONE &&
	    obj->base.write_domain != I915_GEM_DOMAIN_CPU) {
		ret = i915_gem_object_pin(obj, 0, true);
		if (ret)
			goto out;

		ret = i915_gem_object_set_to_gtt_domain(obj, true);
		if (ret)
			goto out_unpin;

		ret = i915_gem_object_put_fence(obj);
		if (ret)
			goto out_unpin;

		ret = i915_gem_gtt_pwrite_fast(dev, obj, args, file);
		if (ret == -EFAULT)
			ret = i915_gem_gtt_pwrite_slow(dev, obj, args, file);

out_unpin:
		i915_gem_object_unpin(obj);

		if (ret != -EFAULT)
			goto out;
		/* Fall through to the shmfs paths because the gtt paths might
		 * fail with non-page-backed user pointers (e.g. gtt mappings
		 * when moving data between textures). */
	}

	ret = i915_gem_object_set_to_cpu_domain(obj, 1);
	if (ret)
		goto out;

	ret = -EFAULT;
	if (!i915_gem_object_needs_bit17_swizzle(obj))
		ret = i915_gem_shmem_pwrite_fast(dev, obj, args, file);
	if (ret == -EFAULT)
		ret = i915_gem_shmem_pwrite_slow(dev, obj, args, file);

out:
	drm_gem_object_unreference(&obj->base);
unlock:
	mutex_unlock(&dev->struct_mutex);
	return ret;
}

/**
 * Called when user space prepares to use an object with the CPU, either
 * through the mmap ioctl's mapping or a GTT mapping.
 */
int
i915_gem_set_domain_ioctl(struct drm_device *dev, void *data,
			  struct drm_file *file)
{
	struct drm_i915_gem_set_domain *args = data;
	struct drm_i915_gem_object *obj;
	uint32_t read_domains = args->read_domains;
	uint32_t write_domain = args->write_domain;
	int ret;

	if (!(dev->driver->driver_features & DRIVER_GEM))
		return -ENODEV;

	/* Only handle setting domains to types used by the CPU. */
	if (write_domain & I915_GEM_GPU_DOMAINS)
		return -EINVAL;

	if (read_domains & I915_GEM_GPU_DOMAINS)
		return -EINVAL;

	/* Having something in the write domain implies it's in the read
	 * domain, and only that read domain.  Enforce that in the request.
	 */
	if (write_domain != 0 && read_domains != write_domain)
		return -EINVAL;

	ret = i915_mutex_lock_interruptible(dev);
	if (ret)
		return ret;

	obj = to_intel_bo(drm_gem_object_lookup(dev, file, args->handle));
	if (&obj->base == NULL) {
		ret = -ENOENT;
		goto unlock;
	}

	if (read_domains & I915_GEM_DOMAIN_GTT) {
		ret = i915_gem_object_set_to_gtt_domain(obj, write_domain != 0);

		/* Silently promote "you're not bound, there was nothing to do"
		 * to success, since the client was just asking us to
		 * make sure everything was done.
		 */
		if (ret == -EINVAL)
			ret = 0;
	} else {
		ret = i915_gem_object_set_to_cpu_domain(obj, write_domain != 0);
	}

	drm_gem_object_unreference(&obj->base);
unlock:
	mutex_unlock(&dev->struct_mutex);
=======
	if (!i915_gem_object_has_struct_page(obj) ||
	    i915_gem_cpu_write_needs_clflush(obj))
		/* Note that the gtt paths might fail with non-page-backed user
		 * pointers (e.g. gtt mappings when moving data between
		 * textures). Fallback to the shmem path in that case.
		 */
		ret = i915_gem_gtt_pwrite_fast(obj, args);

	if (ret == -EFAULT || ret == -ENOSPC) {
		if (i915_gem_object_has_struct_page(obj))
			ret = i915_gem_shmem_pwrite(obj, args);
	}

err:
	i915_gem_object_put(obj);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	return ret;
}

/**
<<<<<<< HEAD
 * Called when user space has done writes to this buffer
=======
 * i915_gem_sw_finish_ioctl - Called when user space has done writes to this buffer
 * @dev: drm device
 * @data: ioctl data blob
 * @file: drm file
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 */
int
i915_gem_sw_finish_ioctl(struct drm_device *dev, void *data,
			 struct drm_file *file)
{
	struct drm_i915_gem_sw_finish *args = data;
	struct drm_i915_gem_object *obj;
<<<<<<< HEAD
	int ret = 0;

	if (!(dev->driver->driver_features & DRIVER_GEM))
		return -ENODEV;

	ret = i915_mutex_lock_interruptible(dev);
	if (ret)
		return ret;

	obj = to_intel_bo(drm_gem_object_lookup(dev, file, args->handle));
	if (&obj->base == NULL) {
		ret = -ENOENT;
		goto unlock;
	}

	/* Pinned buffers may be scanout, so flush the cache */
	if (obj->pin_count)
		i915_gem_object_flush_cpu_write_domain(obj);

	drm_gem_object_unreference(&obj->base);
unlock:
	mutex_unlock(&dev->struct_mutex);
	return ret;
}

/**
 * Maps the contents of an object, returning the address it is mapped
 * into.
 *
 * While the mapping holds a reference on the contents of the object, it doesn't
 * imply a ref on the object itself.
 */
int
i915_gem_mmap_ioctl(struct drm_device *dev, void *data,
		    struct drm_file *file)
{
	struct drm_i915_gem_mmap *args = data;
	struct drm_gem_object *obj;
	unsigned long addr;

	if (!(dev->driver->driver_features & DRIVER_GEM))
		return -ENODEV;

	obj = drm_gem_object_lookup(dev, file, args->handle);
	if (obj == NULL)
		return -ENOENT;

	addr = vm_mmap(obj->filp, 0, args->size,
		       PROT_READ | PROT_WRITE, MAP_SHARED,
		       args->offset);
	drm_gem_object_unreference_unlocked(obj);
	if (IS_ERR((void *)addr))
		return addr;

	args->addr_ptr = (uint64_t) addr;
=======

	obj = i915_gem_object_lookup(file, args->handle);
	if (!obj)
		return -ENOENT;

	/*
	 * Proxy objects are barred from CPU access, so there is no
	 * need to ban sw_finish as it is a nop.
	 */

	/* Pinned buffers may be scanout, so flush the cache */
	i915_gem_object_flush_if_display(obj);
	i915_gem_object_put(obj);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	return 0;
}

<<<<<<< HEAD
/**
 * i915_gem_fault - fault a page into the GTT
 * vma: VMA in question
 * vmf: fault info
 *
 * The fault handler is set up by drm_gem_mmap() when a object is GTT mapped
 * from userspace.  The fault handler takes care of binding the object to
 * the GTT (if needed), allocating and programming a fence register (again,
 * only if needed based on whether the old reg is still valid or the object
 * is tiled) and inserting a new PTE into the faulting process.
 *
 * Note that the faulting process may involve evicting existing objects
 * from the GTT and/or fence registers to make room.  So performance may
 * suffer if the GTT working set is large or there are few fence registers
 * left.
 */
int i915_gem_fault(struct vm_area_struct *vma, struct vm_fault *vmf)
{
	struct drm_i915_gem_object *obj = to_intel_bo(vma->vm_private_data);
	struct drm_device *dev = obj->base.dev;
	drm_i915_private_t *dev_priv = dev->dev_private;
	pgoff_t page_offset;
	unsigned long pfn;
	int ret = 0;
	bool write = !!(vmf->flags & FAULT_FLAG_WRITE);

	/* We don't use vmf->pgoff since that has the fake offset */
	page_offset = ((unsigned long)vmf->virtual_address - vma->vm_start) >>
		PAGE_SHIFT;

	ret = i915_mutex_lock_interruptible(dev);
	if (ret)
		goto out;

	trace_i915_gem_object_fault(obj, page_offset, true, write);

	/* Now bind it into the GTT if needed */
	if (!obj->map_and_fenceable) {
		ret = i915_gem_object_unbind(obj);
		if (ret)
			goto unlock;
	}
	if (!obj->gtt_space) {
		ret = i915_gem_object_bind_to_gtt(obj, 0, true);
		if (ret)
			goto unlock;

		ret = i915_gem_object_set_to_gtt_domain(obj, write);
		if (ret)
			goto unlock;
	}

	if (obj->tiling_mode == I915_TILING_NONE)
		ret = i915_gem_object_put_fence(obj);
	else
		ret = i915_gem_object_get_fence(obj, NULL);
	if (ret)
		goto unlock;

	if (i915_gem_object_is_inactive(obj))
		list_move_tail(&obj->mm_list, &dev_priv->mm.inactive_list);

	obj->fault_mappable = true;

	pfn = ((dev->agp->base + obj->gtt_offset) >> PAGE_SHIFT) +
		page_offset;

	/* Finally, remap it using the new GTT offset */
	ret = vm_insert_pfn(vma, (unsigned long)vmf->virtual_address, pfn);
unlock:
	mutex_unlock(&dev->struct_mutex);
out:
	switch (ret) {
	case -EIO:
	case -EAGAIN:
		/* Give the error handler a chance to run and move the
		 * objects off the GPU active list. Next time we service the
		 * fault, we should be able to transition the page into the
		 * GTT without touching the GPU (and so avoid further
		 * EIO/EGAIN). If the GPU is wedged, then there is no issue
		 * with coherency, just lost writes.
		 */
		set_need_resched();
	case 0:
	case -ERESTARTSYS:
	case -EINTR:
	case -EBUSY:
		/*
		 * EBUSY is ok: this just means that another thread
		 * already did the job.
		 */
		return VM_FAULT_NOPAGE;
	case -ENOMEM:
		return VM_FAULT_OOM;
	default:
		return VM_FAULT_SIGBUS;
	}
}

/**
 * i915_gem_release_mmap - remove physical page mappings
 * @obj: obj in question
 *
 * Preserve the reservation of the mmapping with the DRM core code, but
 * relinquish ownership of the pages back to the system.
 *
 * It is vital that we remove the page mapping if we have mapped a tiled
 * object through the GTT and then lose the fence register due to
 * resource pressure. Similarly if the object has been moved out of the
 * aperture, than pages mapped into userspace must be revoked. Removing the
 * mapping will then trigger a page fault on the next user access, allowing
 * fixup by i915_gem_fault().
 */
void
i915_gem_release_mmap(struct drm_i915_gem_object *obj)
{
	if (!obj->fault_mappable)
		return;

	if (obj->base.dev->dev_mapping)
		unmap_mapping_range(obj->base.dev->dev_mapping,
				    (loff_t)obj->base.map_list.hash.key<<PAGE_SHIFT,
				    obj->base.size, 1);

	obj->fault_mappable = false;
}

static uint32_t
i915_gem_get_gtt_size(struct drm_device *dev, uint32_t size, int tiling_mode)
{
	uint32_t gtt_size;

	if (INTEL_INFO(dev)->gen >= 4 ||
	    tiling_mode == I915_TILING_NONE)
		return size;

	/* Previous chips need a power-of-two fence region when tiling */
	if (INTEL_INFO(dev)->gen == 3)
		gtt_size = 1024*1024;
	else
		gtt_size = 512*1024;

	while (gtt_size < size)
		gtt_size <<= 1;

	return gtt_size;
}

/**
 * i915_gem_get_gtt_alignment - return required GTT alignment for an object
 * @obj: object to check
 *
 * Return the required GTT alignment for an object, taking into account
 * potential fence register mapping.
 */
static uint32_t
i915_gem_get_gtt_alignment(struct drm_device *dev,
			   uint32_t size,
			   int tiling_mode)
{
	/*
	 * Minimum alignment is 4k (GTT page size), but might be greater
	 * if a fence register is needed for the object.
	 */
	if (INTEL_INFO(dev)->gen >= 4 ||
	    tiling_mode == I915_TILING_NONE)
		return 4096;

	/*
	 * Previous chips need to be aligned to the size of the smallest
	 * fence register that can contain the object.
	 */
	return i915_gem_get_gtt_size(dev, size, tiling_mode);
}

/**
 * i915_gem_get_unfenced_gtt_alignment - return required GTT alignment for an
 *					 unfenced object
 * @dev: the device
 * @size: size of the object
 * @tiling_mode: tiling mode of the object
 *
 * Return the required GTT alignment for an object, only taking into account
 * unfenced tiled surface requirements.
 */
uint32_t
i915_gem_get_unfenced_gtt_alignment(struct drm_device *dev,
				    uint32_t size,
				    int tiling_mode)
{
	/*
	 * Minimum alignment is 4k (GTT page size) for sane hw.
	 */
	if (INTEL_INFO(dev)->gen >= 4 || IS_G33(dev) ||
	    tiling_mode == I915_TILING_NONE)
		return 4096;

	/* Previous hardware however needs to be aligned to a power-of-two
	 * tile height. The simplest method for determining this is to reuse
	 * the power-of-tile object size.
	 */
	return i915_gem_get_gtt_size(dev, size, tiling_mode);
}

int
i915_gem_mmap_gtt(struct drm_file *file,
		  struct drm_device *dev,
		  uint32_t handle,
		  uint64_t *offset)
{
	struct drm_i915_private *dev_priv = dev->dev_private;
	struct drm_i915_gem_object *obj;
	int ret;

	if (!(dev->driver->driver_features & DRIVER_GEM))
		return -ENODEV;

	ret = i915_mutex_lock_interruptible(dev);
	if (ret)
		return ret;

	obj = to_intel_bo(drm_gem_object_lookup(dev, file, handle));
	if (&obj->base == NULL) {
		ret = -ENOENT;
		goto unlock;
	}

	if (obj->base.size > dev_priv->mm.gtt_mappable_end) {
		ret = -E2BIG;
		goto out;
	}

	if (obj->madv != I915_MADV_WILLNEED) {
		DRM_ERROR("Attempting to mmap a purgeable buffer\n");
		ret = -EINVAL;
		goto out;
	}

	if (!obj->base.map_list.map) {
		ret = drm_gem_create_mmap_offset(&obj->base);
		if (ret)
			goto out;
	}

	*offset = (u64)obj->base.map_list.hash.key << PAGE_SHIFT;

out:
	drm_gem_object_unreference(&obj->base);
unlock:
	mutex_unlock(&dev->struct_mutex);
	return ret;
}

/**
 * i915_gem_mmap_gtt_ioctl - prepare an object for GTT mmap'ing
 * @dev: DRM device
 * @data: GTT mapping ioctl data
 * @file: GEM object info
 *
 * Simply returns the fake offset to userspace so it can mmap it.
 * The mmap call will end up in drm_gem_mmap(), which will set things
 * up so we can get faults in the handler above.
 *
 * The fault handler will take care of binding the object into the GTT
 * (since it may have been evicted to make room for something), allocating
 * a fence register, and mapping the appropriate aperture address into
 * userspace.
 */
int
i915_gem_mmap_gtt_ioctl(struct drm_device *dev, void *data,
			struct drm_file *file)
{
	struct drm_i915_gem_mmap_gtt *args = data;

	if (!(dev->driver->driver_features & DRIVER_GEM))
		return -ENODEV;

	return i915_gem_mmap_gtt(file, dev, args->handle, &args->offset);
}


static int
i915_gem_object_get_pages_gtt(struct drm_i915_gem_object *obj,
			      gfp_t gfpmask)
{
	int page_count, i;
	struct address_space *mapping;
	struct inode *inode;
	struct page *page;

	/* Get the list of pages out of our struct file.  They'll be pinned
	 * at this point until we release them.
	 */
	page_count = obj->base.size / PAGE_SIZE;
	BUG_ON(obj->pages != NULL);
	obj->pages = drm_malloc_ab(page_count, sizeof(struct page *));
	if (obj->pages == NULL)
		return -ENOMEM;

	inode = obj->base.filp->f_path.dentry->d_inode;
	mapping = inode->i_mapping;
	gfpmask |= mapping_gfp_mask(mapping);

	for (i = 0; i < page_count; i++) {
		page = shmem_read_mapping_page_gfp(mapping, i, gfpmask);
		if (IS_ERR(page))
			goto err_pages;

		obj->pages[i] = page;
	}

	if (i915_gem_object_needs_bit17_swizzle(obj))
		i915_gem_object_do_bit_17_swizzle(obj);

	return 0;

err_pages:
	while (i--)
		page_cache_release(obj->pages[i]);

	drm_free_large(obj->pages);
	obj->pages = NULL;
	return PTR_ERR(page);
}

static void
i915_gem_object_put_pages_gtt(struct drm_i915_gem_object *obj)
{
	int page_count = obj->base.size / PAGE_SIZE;
	int i;

	BUG_ON(obj->madv == __I915_MADV_PURGED);

	if (i915_gem_object_needs_bit17_swizzle(obj))
		i915_gem_object_save_bit_17_swizzle(obj);

	if (obj->madv == I915_MADV_DONTNEED)
		obj->dirty = 0;

	for (i = 0; i < page_count; i++) {
		if (obj->dirty)
			set_page_dirty(obj->pages[i]);

		if (obj->madv == I915_MADV_WILLNEED)
			mark_page_accessed(obj->pages[i]);

		page_cache_release(obj->pages[i]);
	}
	obj->dirty = 0;

	drm_free_large(obj->pages);
	obj->pages = NULL;
}

void
i915_gem_object_move_to_active(struct drm_i915_gem_object *obj,
			       struct intel_ring_buffer *ring,
			       u32 seqno)
{
	struct drm_device *dev = obj->base.dev;
	struct drm_i915_private *dev_priv = dev->dev_private;

	BUG_ON(ring == NULL);
	obj->ring = ring;

	/* Add a reference if we're newly entering the active list. */
	if (!obj->active) {
		drm_gem_object_reference(&obj->base);
		obj->active = 1;
	}

	/* Move from whatever list we were on to the tail of execution. */
	list_move_tail(&obj->mm_list, &dev_priv->mm.active_list);
	list_move_tail(&obj->ring_list, &ring->active_list);

	obj->last_rendering_seqno = seqno;

	if (obj->fenced_gpu_access) {
		obj->last_fenced_seqno = seqno;
		obj->last_fenced_ring = ring;

		/* Bump MRU to take account of the delayed flush */
		if (obj->fence_reg != I915_FENCE_REG_NONE) {
			struct drm_i915_fence_reg *reg;

			reg = &dev_priv->fence_regs[obj->fence_reg];
			list_move_tail(&reg->lru_list,
				       &dev_priv->mm.fence_list);
		}
	}
}

static void
i915_gem_object_move_off_active(struct drm_i915_gem_object *obj)
{
	list_del_init(&obj->ring_list);
	obj->last_rendering_seqno = 0;
	obj->last_fenced_seqno = 0;
}

static void
i915_gem_object_move_to_flushing(struct drm_i915_gem_object *obj)
{
	struct drm_device *dev = obj->base.dev;
	drm_i915_private_t *dev_priv = dev->dev_private;

	BUG_ON(!obj->active);
	list_move_tail(&obj->mm_list, &dev_priv->mm.flushing_list);

	i915_gem_object_move_off_active(obj);
}

static void
i915_gem_object_move_to_inactive(struct drm_i915_gem_object *obj)
{
	struct drm_device *dev = obj->base.dev;
	struct drm_i915_private *dev_priv = dev->dev_private;

	if (obj->pin_count != 0)
		list_move_tail(&obj->mm_list, &dev_priv->mm.pinned_list);
	else
		list_move_tail(&obj->mm_list, &dev_priv->mm.inactive_list);

	BUG_ON(!list_empty(&obj->gpu_write_list));
	BUG_ON(!obj->active);
	obj->ring = NULL;
	obj->last_fenced_ring = NULL;

	i915_gem_object_move_off_active(obj);
	obj->fenced_gpu_access = false;

	obj->active = 0;
	obj->pending_gpu_write = false;
	drm_gem_object_unreference(&obj->base);

	WARN_ON(i915_verify_lists(dev));
}

/* Immediately discard the backing storage */
static void
i915_gem_object_truncate(struct drm_i915_gem_object *obj)
{
	struct inode *inode;

	/* Our goal here is to return as much of the memory as
	 * is possible back to the system as we are called from OOM.
	 * To do this we must instruct the shmfs to drop all of its
	 * backing pages, *now*.
	 */
	inode = obj->base.filp->f_path.dentry->d_inode;
	shmem_truncate_range(inode, 0, (loff_t)-1);

	obj->madv = __I915_MADV_PURGED;
}

static inline int
i915_gem_object_is_purgeable(struct drm_i915_gem_object *obj)
{
	return obj->madv == I915_MADV_DONTNEED;
}

static void
i915_gem_process_flushing_list(struct intel_ring_buffer *ring,
			       uint32_t flush_domains)
{
	struct drm_i915_gem_object *obj, *next;

	list_for_each_entry_safe(obj, next,
				 &ring->gpu_write_list,
				 gpu_write_list) {
		if (obj->base.write_domain & flush_domains) {
			uint32_t old_write_domain = obj->base.write_domain;

			obj->base.write_domain = 0;
			list_del_init(&obj->gpu_write_list);
			i915_gem_object_move_to_active(obj, ring,
						       i915_gem_next_request_seqno(ring));

			trace_i915_gem_object_change_domain(obj,
							    obj->base.read_domains,
							    old_write_domain);
		}
	}
}

static u32
i915_gem_get_seqno(struct drm_device *dev)
{
	drm_i915_private_t *dev_priv = dev->dev_private;
	u32 seqno = dev_priv->next_seqno;

	/* reserve 0 for non-seqno */
	if (++dev_priv->next_seqno == 0)
		dev_priv->next_seqno = 1;

	return seqno;
}

u32
i915_gem_next_request_seqno(struct intel_ring_buffer *ring)
{
	if (ring->outstanding_lazy_request == 0)
		ring->outstanding_lazy_request = i915_gem_get_seqno(ring->dev);

	return ring->outstanding_lazy_request;
}

int
i915_add_request(struct intel_ring_buffer *ring,
		 struct drm_file *file,
		 struct drm_i915_gem_request *request)
{
	drm_i915_private_t *dev_priv = ring->dev->dev_private;
	uint32_t seqno;
	u32 request_ring_position;
	int was_empty;
	int ret;

	BUG_ON(request == NULL);
	seqno = i915_gem_next_request_seqno(ring);

	/* Record the position of the start of the request so that
	 * should we detect the updated seqno part-way through the
	 * GPU processing the request, we never over-estimate the
	 * position of the head.
	 */
	request_ring_position = intel_ring_get_tail(ring);

	ret = ring->add_request(ring, &seqno);
	if (ret)
	    return ret;

	trace_i915_gem_request_add(ring, seqno);

	request->seqno = seqno;
	request->ring = ring;
	request->tail = request_ring_position;
	request->emitted_jiffies = jiffies;
	was_empty = list_empty(&ring->request_list);
	list_add_tail(&request->list, &ring->request_list);

	if (file) {
		struct drm_i915_file_private *file_priv = file->driver_priv;

		spin_lock(&file_priv->mm.lock);
		request->file_priv = file_priv;
		list_add_tail(&request->client_list,
			      &file_priv->mm.request_list);
		spin_unlock(&file_priv->mm.lock);
	}

	ring->outstanding_lazy_request = 0;

	if (!dev_priv->mm.suspended) {
		if (i915_enable_hangcheck) {
			mod_timer(&dev_priv->hangcheck_timer,
				  jiffies +
				  msecs_to_jiffies(DRM_I915_HANGCHECK_PERIOD));
		}
		if (was_empty)
			queue_delayed_work(dev_priv->wq,
					   &dev_priv->mm.retire_work, HZ);
	}
	return 0;
}

static inline void
i915_gem_request_remove_from_client(struct drm_i915_gem_request *request)
{
	struct drm_i915_file_private *file_priv = request->file_priv;

	if (!file_priv)
		return;

	spin_lock(&file_priv->mm.lock);
	if (request->file_priv) {
		list_del(&request->client_list);
		request->file_priv = NULL;
	}
	spin_unlock(&file_priv->mm.lock);
}

static void i915_gem_reset_ring_lists(struct drm_i915_private *dev_priv,
				      struct intel_ring_buffer *ring)
{
	while (!list_empty(&ring->request_list)) {
		struct drm_i915_gem_request *request;

		request = list_first_entry(&ring->request_list,
					   struct drm_i915_gem_request,
					   list);

		list_del(&request->list);
		i915_gem_request_remove_from_client(request);
		kfree(request);
	}

	while (!list_empty(&ring->active_list)) {
		struct drm_i915_gem_object *obj;

		obj = list_first_entry(&ring->active_list,
				       struct drm_i915_gem_object,
				       ring_list);

		obj->base.write_domain = 0;
		list_del_init(&obj->gpu_write_list);
		i915_gem_object_move_to_inactive(obj);
	}
}

static void i915_gem_reset_fences(struct drm_device *dev)
{
	struct drm_i915_private *dev_priv = dev->dev_private;
	int i;

	for (i = 0; i < dev_priv->num_fence_regs; i++) {
		struct drm_i915_fence_reg *reg = &dev_priv->fence_regs[i];
		struct drm_i915_gem_object *obj = reg->obj;

		if (!obj)
			continue;

		if (obj->tiling_mode)
			i915_gem_release_mmap(obj);

		reg->obj->fence_reg = I915_FENCE_REG_NONE;
		reg->obj->fenced_gpu_access = false;
		reg->obj->last_fenced_seqno = 0;
		reg->obj->last_fenced_ring = NULL;
		i915_gem_clear_fence_reg(dev, reg);
	}
}

void i915_gem_reset(struct drm_device *dev)
{
	struct drm_i915_private *dev_priv = dev->dev_private;
	struct drm_i915_gem_object *obj;
	int i;

	for (i = 0; i < I915_NUM_RINGS; i++)
		i915_gem_reset_ring_lists(dev_priv, &dev_priv->ring[i]);

	/* Remove anything from the flushing lists. The GPU cache is likely
	 * to be lost on reset along with the data, so simply move the
	 * lost bo to the inactive list.
	 */
	while (!list_empty(&dev_priv->mm.flushing_list)) {
		obj = list_first_entry(&dev_priv->mm.flushing_list,
				      struct drm_i915_gem_object,
				      mm_list);

		obj->base.write_domain = 0;
		list_del_init(&obj->gpu_write_list);
		i915_gem_object_move_to_inactive(obj);
	}

	/* Move everything out of the GPU domains to ensure we do any
	 * necessary invalidation upon reuse.
	 */
	list_for_each_entry(obj,
			    &dev_priv->mm.inactive_list,
			    mm_list)
	{
		obj->base.read_domains &= ~I915_GEM_GPU_DOMAINS;
	}

	/* The fence registers are invalidated so clear them out */
	i915_gem_reset_fences(dev);
}

/**
 * This function clears the request list as sequence numbers are passed.
 */
void
i915_gem_retire_requests_ring(struct intel_ring_buffer *ring)
{
	uint32_t seqno;
	int i;

	if (list_empty(&ring->request_list))
		return;

	WARN_ON(i915_verify_lists(ring->dev));

	seqno = ring->get_seqno(ring);

	for (i = 0; i < ARRAY_SIZE(ring->sync_seqno); i++)
		if (seqno >= ring->sync_seqno[i])
			ring->sync_seqno[i] = 0;

	while (!list_empty(&ring->request_list)) {
		struct drm_i915_gem_request *request;

		request = list_first_entry(&ring->request_list,
					   struct drm_i915_gem_request,
					   list);

		if (!i915_seqno_passed(seqno, request->seqno))
			break;

		trace_i915_gem_request_retire(ring, request->seqno);
		/* We know the GPU must have read the request to have
		 * sent us the seqno + interrupt, so use the position
		 * of tail of the request to update the last known position
		 * of the GPU head.
		 */
		ring->last_retired_head = request->tail;

		list_del(&request->list);
		i915_gem_request_remove_from_client(request);
		kfree(request);
	}

	/* Move any buffers on the active list that are no longer referenced
	 * by the ringbuffer to the flushing/inactive lists as appropriate.
	 */
	while (!list_empty(&ring->active_list)) {
		struct drm_i915_gem_object *obj;

		obj = list_first_entry(&ring->active_list,
				      struct drm_i915_gem_object,
				      ring_list);

		if (!i915_seqno_passed(seqno, obj->last_rendering_seqno))
			break;

		if (obj->base.write_domain != 0)
			i915_gem_object_move_to_flushing(obj);
		else
			i915_gem_object_move_to_inactive(obj);
	}

	if (unlikely(ring->trace_irq_seqno &&
		     i915_seqno_passed(seqno, ring->trace_irq_seqno))) {
		ring->irq_put(ring);
		ring->trace_irq_seqno = 0;
	}

	WARN_ON(i915_verify_lists(ring->dev));
}

void
i915_gem_retire_requests(struct drm_device *dev)
{
	drm_i915_private_t *dev_priv = dev->dev_private;
	int i;

	if (!list_empty(&dev_priv->mm.deferred_free_list)) {
	    struct drm_i915_gem_object *obj, *next;

	    /* We must be careful that during unbind() we do not
	     * accidentally infinitely recurse into retire requests.
	     * Currently:
	     *   retire -> free -> unbind -> wait -> retire_ring
	     */
	    list_for_each_entry_safe(obj, next,
				     &dev_priv->mm.deferred_free_list,
				     mm_list)
		    i915_gem_free_object_tail(obj);
	}

	for (i = 0; i < I915_NUM_RINGS; i++)
		i915_gem_retire_requests_ring(&dev_priv->ring[i]);
}

static void
i915_gem_retire_work_handler(struct work_struct *work)
{
	drm_i915_private_t *dev_priv;
	struct drm_device *dev;
	bool idle;
	int i;

	dev_priv = container_of(work, drm_i915_private_t,
				mm.retire_work.work);
	dev = dev_priv->dev;

	/* Come back later if the device is busy... */
	if (!mutex_trylock(&dev->struct_mutex)) {
		queue_delayed_work(dev_priv->wq, &dev_priv->mm.retire_work, HZ);
		return;
	}

	i915_gem_retire_requests(dev);

	/* Send a periodic flush down the ring so we don't hold onto GEM
	 * objects indefinitely.
	 */
	idle = true;
	for (i = 0; i < I915_NUM_RINGS; i++) {
		struct intel_ring_buffer *ring = &dev_priv->ring[i];

		if (!list_empty(&ring->gpu_write_list)) {
			struct drm_i915_gem_request *request;
			int ret;

			ret = i915_gem_flush_ring(ring,
						  0, I915_GEM_GPU_DOMAINS);
			request = kzalloc(sizeof(*request), GFP_KERNEL);
			if (ret || request == NULL ||
			    i915_add_request(ring, NULL, request))
			    kfree(request);
		}

		idle &= list_empty(&ring->request_list);
	}

	if (!dev_priv->mm.suspended && !idle)
		queue_delayed_work(dev_priv->wq, &dev_priv->mm.retire_work, HZ);

	mutex_unlock(&dev->struct_mutex);
}

/**
 * Waits for a sequence number to be signaled, and cleans up the
 * request and object lists appropriately for that event.
 */
int
i915_wait_request(struct intel_ring_buffer *ring,
		  uint32_t seqno,
		  bool do_retire)
{
	drm_i915_private_t *dev_priv = ring->dev->dev_private;
	u32 ier;
	int ret = 0;

	BUG_ON(seqno == 0);

	if (atomic_read(&dev_priv->mm.wedged)) {
		struct completion *x = &dev_priv->error_completion;
		bool recovery_complete;
		unsigned long flags;

		/* Give the error handler a chance to run. */
		spin_lock_irqsave(&x->wait.lock, flags);
		recovery_complete = x->done > 0;
		spin_unlock_irqrestore(&x->wait.lock, flags);

		return recovery_complete ? -EIO : -EAGAIN;
	}

	if (seqno == ring->outstanding_lazy_request) {
		struct drm_i915_gem_request *request;

		request = kzalloc(sizeof(*request), GFP_KERNEL);
		if (request == NULL)
			return -ENOMEM;

		ret = i915_add_request(ring, NULL, request);
		if (ret) {
			kfree(request);
			return ret;
		}

		seqno = request->seqno;
	}

	if (!i915_seqno_passed(ring->get_seqno(ring), seqno)) {
		if (HAS_PCH_SPLIT(ring->dev))
			ier = I915_READ(DEIER) | I915_READ(GTIER);
		else
			ier = I915_READ(IER);
		if (!ier) {
			DRM_ERROR("something (likely vbetool) disabled "
				  "interrupts, re-enabling\n");
			ring->dev->driver->irq_preinstall(ring->dev);
			ring->dev->driver->irq_postinstall(ring->dev);
		}

		trace_i915_gem_request_wait_begin(ring, seqno);

		ring->waiting_seqno = seqno;
		if (ring->irq_get(ring)) {
			if (dev_priv->mm.interruptible)
				ret = wait_event_interruptible(ring->irq_queue,
							       i915_seqno_passed(ring->get_seqno(ring), seqno)
							       || atomic_read(&dev_priv->mm.wedged));
			else
				wait_event(ring->irq_queue,
					   i915_seqno_passed(ring->get_seqno(ring), seqno)
					   || atomic_read(&dev_priv->mm.wedged));

			ring->irq_put(ring);
		} else if (wait_for_atomic(i915_seqno_passed(ring->get_seqno(ring),
							     seqno) ||
					   atomic_read(&dev_priv->mm.wedged), 3000))
			ret = -EBUSY;
		ring->waiting_seqno = 0;

		trace_i915_gem_request_wait_end(ring, seqno);
	}
	if (atomic_read(&dev_priv->mm.wedged))
		ret = -EAGAIN;

	/* Directly dispatch request retiring.  While we have the work queue
	 * to handle this, the waiter on a request often wants an associated
	 * buffer to have made it to the inactive list, and we would need
	 * a separate wait queue to handle that.
	 */
	if (ret == 0 && do_retire)
		i915_gem_retire_requests_ring(ring);

	return ret;
}

/**
 * Ensures that all rendering to the object has completed and the object is
 * safe to unbind from the GTT or access from the CPU.
 */
int
i915_gem_object_wait_rendering(struct drm_i915_gem_object *obj)
{
	int ret;

	/* This function only exists to support waiting for existing rendering,
	 * not for emitting required flushes.
	 */
	BUG_ON((obj->base.write_domain & I915_GEM_GPU_DOMAINS) != 0);

	/* If there is rendering queued on the buffer being evicted, wait for
	 * it.
	 */
	if (obj->active) {
		ret = i915_wait_request(obj->ring, obj->last_rendering_seqno,
					true);
		if (ret)
			return ret;
	}

	return 0;
}

static void i915_gem_object_finish_gtt(struct drm_i915_gem_object *obj)
{
	u32 old_write_domain, old_read_domains;

	/* Act a barrier for all accesses through the GTT */
	mb();

	/* Force a pagefault for domain tracking on next user access */
	i915_gem_release_mmap(obj);

	if ((obj->base.read_domains & I915_GEM_DOMAIN_GTT) == 0)
		return;

	old_read_domains = obj->base.read_domains;
	old_write_domain = obj->base.write_domain;

	obj->base.read_domains &= ~I915_GEM_DOMAIN_GTT;
	obj->base.write_domain &= ~I915_GEM_DOMAIN_GTT;

	trace_i915_gem_object_change_domain(obj,
					    old_read_domains,
					    old_write_domain);
}

/**
 * Unbinds an object from the GTT aperture.
 */
int
i915_gem_object_unbind(struct drm_i915_gem_object *obj)
{
	drm_i915_private_t *dev_priv = obj->base.dev->dev_private;
	int ret = 0;

	if (obj->gtt_space == NULL)
		return 0;

	if (obj->pin_count != 0) {
		DRM_ERROR("Attempting to unbind pinned buffer\n");
		return -EINVAL;
	}

	ret = i915_gem_object_finish_gpu(obj);
	if (ret == -ERESTARTSYS)
		return ret;
	/* Continue on if we fail due to EIO, the GPU is hung so we
	 * should be safe and we need to cleanup or else we might
	 * cause memory corruption through use-after-free.
	 */

	i915_gem_object_finish_gtt(obj);

	/* Move the object to the CPU domain to ensure that
	 * any possible CPU writes while it's not in the GTT
	 * are flushed when we go to remap it.
	 */
	if (ret == 0)
		ret = i915_gem_object_set_to_cpu_domain(obj, 1);
	if (ret == -ERESTARTSYS)
		return ret;
	if (ret) {
		/* In the event of a disaster, abandon all caches and
		 * hope for the best.
		 */
		i915_gem_clflush_object(obj);
		obj->base.read_domains = obj->base.write_domain = I915_GEM_DOMAIN_CPU;
	}

	/* release the fence reg _after_ flushing */
	ret = i915_gem_object_put_fence(obj);
	if (ret == -ERESTARTSYS)
		return ret;

	trace_i915_gem_object_unbind(obj);

	i915_gem_gtt_unbind_object(obj);
	if (obj->has_aliasing_ppgtt_mapping) {
		i915_ppgtt_unbind_object(dev_priv->mm.aliasing_ppgtt, obj);
		obj->has_aliasing_ppgtt_mapping = 0;
	}

	i915_gem_object_put_pages_gtt(obj);

	list_del_init(&obj->gtt_list);
	list_del_init(&obj->mm_list);
	/* Avoid an unnecessary call to unbind on rebind. */
	obj->map_and_fenceable = true;

	drm_mm_put_block(obj->gtt_space);
	obj->gtt_space = NULL;
	obj->gtt_offset = 0;

	if (i915_gem_object_is_purgeable(obj))
		i915_gem_object_truncate(obj);

	return ret;
}

int
i915_gem_flush_ring(struct intel_ring_buffer *ring,
		    uint32_t invalidate_domains,
		    uint32_t flush_domains)
{
	int ret;

	if (((invalidate_domains | flush_domains) & I915_GEM_GPU_DOMAINS) == 0)
		return 0;

	trace_i915_gem_ring_flush(ring, invalidate_domains, flush_domains);

	ret = ring->flush(ring, invalidate_domains, flush_domains);
	if (ret)
		return ret;

	if (flush_domains & I915_GEM_GPU_DOMAINS)
		i915_gem_process_flushing_list(ring, flush_domains);

	return 0;
}

static int i915_ring_idle(struct intel_ring_buffer *ring, bool do_retire)
{
	int ret;

	if (list_empty(&ring->gpu_write_list) && list_empty(&ring->active_list))
		return 0;

	if (!list_empty(&ring->gpu_write_list)) {
		ret = i915_gem_flush_ring(ring,
				    I915_GEM_GPU_DOMAINS, I915_GEM_GPU_DOMAINS);
		if (ret)
			return ret;
	}

	return i915_wait_request(ring, i915_gem_next_request_seqno(ring),
				 do_retire);
}

int i915_gpu_idle(struct drm_device *dev, bool do_retire)
{
	drm_i915_private_t *dev_priv = dev->dev_private;
	int ret, i;

	/* Flush everything onto the inactive list. */
	for (i = 0; i < I915_NUM_RINGS; i++) {
		ret = i915_ring_idle(&dev_priv->ring[i], do_retire);
		if (ret)
			return ret;
	}

	return 0;
}

static int sandybridge_write_fence_reg(struct drm_i915_gem_object *obj,
				       struct intel_ring_buffer *pipelined)
{
	struct drm_device *dev = obj->base.dev;
	drm_i915_private_t *dev_priv = dev->dev_private;
	u32 size = obj->gtt_space->size;
	int regnum = obj->fence_reg;
	uint64_t val;

	/* Adjust fence size to match tiled area */
	if (obj->tiling_mode != I915_TILING_NONE) {
		uint32_t row_size = obj->stride *
			(obj->tiling_mode == I915_TILING_Y ? 32 : 8);
		size = (size / row_size) * row_size;
	}

	val = (uint64_t)((obj->gtt_offset + size - 4096) &
			 0xfffff000) << 32;
	val |= obj->gtt_offset & 0xfffff000;
	val |= (uint64_t)((obj->stride / 128) - 1) <<
		SANDYBRIDGE_FENCE_PITCH_SHIFT;

	if (obj->tiling_mode == I915_TILING_Y)
		val |= 1 << I965_FENCE_TILING_Y_SHIFT;
	val |= I965_FENCE_REG_VALID;

	if (pipelined) {
		int ret = intel_ring_begin(pipelined, 6);
		if (ret)
			return ret;

		intel_ring_emit(pipelined, MI_NOOP);
		intel_ring_emit(pipelined, MI_LOAD_REGISTER_IMM(2));
		intel_ring_emit(pipelined, FENCE_REG_SANDYBRIDGE_0 + regnum*8);
		intel_ring_emit(pipelined, (u32)val);
		intel_ring_emit(pipelined, FENCE_REG_SANDYBRIDGE_0 + regnum*8 + 4);
		intel_ring_emit(pipelined, (u32)(val >> 32));
		intel_ring_advance(pipelined);
	} else
		I915_WRITE64(FENCE_REG_SANDYBRIDGE_0 + regnum * 8, val);

	return 0;
}

static int i965_write_fence_reg(struct drm_i915_gem_object *obj,
				struct intel_ring_buffer *pipelined)
{
	struct drm_device *dev = obj->base.dev;
	drm_i915_private_t *dev_priv = dev->dev_private;
	u32 size = obj->gtt_space->size;
	int regnum = obj->fence_reg;
	uint64_t val;

	/* Adjust fence size to match tiled area */
	if (obj->tiling_mode != I915_TILING_NONE) {
		uint32_t row_size = obj->stride *
			(obj->tiling_mode == I915_TILING_Y ? 32 : 8);
		size = (size / row_size) * row_size;
	}

	val = (uint64_t)((obj->gtt_offset + size - 4096) &
		    0xfffff000) << 32;
	val |= obj->gtt_offset & 0xfffff000;
	val |= ((obj->stride / 128) - 1) << I965_FENCE_PITCH_SHIFT;
	if (obj->tiling_mode == I915_TILING_Y)
		val |= 1 << I965_FENCE_TILING_Y_SHIFT;
	val |= I965_FENCE_REG_VALID;

	if (pipelined) {
		int ret = intel_ring_begin(pipelined, 6);
		if (ret)
			return ret;

		intel_ring_emit(pipelined, MI_NOOP);
		intel_ring_emit(pipelined, MI_LOAD_REGISTER_IMM(2));
		intel_ring_emit(pipelined, FENCE_REG_965_0 + regnum*8);
		intel_ring_emit(pipelined, (u32)val);
		intel_ring_emit(pipelined, FENCE_REG_965_0 + regnum*8 + 4);
		intel_ring_emit(pipelined, (u32)(val >> 32));
		intel_ring_advance(pipelined);
	} else
		I915_WRITE64(FENCE_REG_965_0 + regnum * 8, val);

	return 0;
}

static int i915_write_fence_reg(struct drm_i915_gem_object *obj,
				struct intel_ring_buffer *pipelined)
{
	struct drm_device *dev = obj->base.dev;
	drm_i915_private_t *dev_priv = dev->dev_private;
	u32 size = obj->gtt_space->size;
	u32 fence_reg, val, pitch_val;
	int tile_width;

	if (WARN((obj->gtt_offset & ~I915_FENCE_START_MASK) ||
		 (size & -size) != size ||
		 (obj->gtt_offset & (size - 1)),
		 "object 0x%08x [fenceable? %d] not 1M or pot-size (0x%08x) aligned\n",
		 obj->gtt_offset, obj->map_and_fenceable, size))
		return -EINVAL;

	if (obj->tiling_mode == I915_TILING_Y && HAS_128_BYTE_Y_TILING(dev))
		tile_width = 128;
	else
		tile_width = 512;

	/* Note: pitch better be a power of two tile widths */
	pitch_val = obj->stride / tile_width;
	pitch_val = ffs(pitch_val) - 1;

	val = obj->gtt_offset;
	if (obj->tiling_mode == I915_TILING_Y)
		val |= 1 << I830_FENCE_TILING_Y_SHIFT;
	val |= I915_FENCE_SIZE_BITS(size);
	val |= pitch_val << I830_FENCE_PITCH_SHIFT;
	val |= I830_FENCE_REG_VALID;

	fence_reg = obj->fence_reg;
	if (fence_reg < 8)
		fence_reg = FENCE_REG_830_0 + fence_reg * 4;
	else
		fence_reg = FENCE_REG_945_8 + (fence_reg - 8) * 4;

	if (pipelined) {
		int ret = intel_ring_begin(pipelined, 4);
		if (ret)
			return ret;

		intel_ring_emit(pipelined, MI_NOOP);
		intel_ring_emit(pipelined, MI_LOAD_REGISTER_IMM(1));
		intel_ring_emit(pipelined, fence_reg);
		intel_ring_emit(pipelined, val);
		intel_ring_advance(pipelined);
	} else
		I915_WRITE(fence_reg, val);

	return 0;
}

static int i830_write_fence_reg(struct drm_i915_gem_object *obj,
				struct intel_ring_buffer *pipelined)
{
	struct drm_device *dev = obj->base.dev;
	drm_i915_private_t *dev_priv = dev->dev_private;
	u32 size = obj->gtt_space->size;
	int regnum = obj->fence_reg;
	uint32_t val;
	uint32_t pitch_val;

	if (WARN((obj->gtt_offset & ~I830_FENCE_START_MASK) ||
		 (size & -size) != size ||
		 (obj->gtt_offset & (size - 1)),
		 "object 0x%08x not 512K or pot-size 0x%08x aligned\n",
		 obj->gtt_offset, size))
		return -EINVAL;

	pitch_val = obj->stride / 128;
	pitch_val = ffs(pitch_val) - 1;

	val = obj->gtt_offset;
	if (obj->tiling_mode == I915_TILING_Y)
		val |= 1 << I830_FENCE_TILING_Y_SHIFT;
	val |= I830_FENCE_SIZE_BITS(size);
	val |= pitch_val << I830_FENCE_PITCH_SHIFT;
	val |= I830_FENCE_REG_VALID;

	if (pipelined) {
		int ret = intel_ring_begin(pipelined, 4);
		if (ret)
			return ret;

		intel_ring_emit(pipelined, MI_NOOP);
		intel_ring_emit(pipelined, MI_LOAD_REGISTER_IMM(1));
		intel_ring_emit(pipelined, FENCE_REG_830_0 + regnum*4);
		intel_ring_emit(pipelined, val);
		intel_ring_advance(pipelined);
	} else
		I915_WRITE(FENCE_REG_830_0 + regnum * 4, val);

	return 0;
}

static bool ring_passed_seqno(struct intel_ring_buffer *ring, u32 seqno)
{
	return i915_seqno_passed(ring->get_seqno(ring), seqno);
}

static int
i915_gem_object_flush_fence(struct drm_i915_gem_object *obj,
			    struct intel_ring_buffer *pipelined)
{
	int ret;

	if (obj->fenced_gpu_access) {
		if (obj->base.write_domain & I915_GEM_GPU_DOMAINS) {
			ret = i915_gem_flush_ring(obj->last_fenced_ring,
						  0, obj->base.write_domain);
			if (ret)
				return ret;
		}

		obj->fenced_gpu_access = false;
	}

	if (obj->last_fenced_seqno && pipelined != obj->last_fenced_ring) {
		if (!ring_passed_seqno(obj->last_fenced_ring,
				       obj->last_fenced_seqno)) {
			ret = i915_wait_request(obj->last_fenced_ring,
						obj->last_fenced_seqno,
						true);
			if (ret)
				return ret;
		}

		obj->last_fenced_seqno = 0;
		obj->last_fenced_ring = NULL;
	}

	/* Ensure that all CPU reads are completed before installing a fence
	 * and all writes before removing the fence.
	 */
	if (obj->base.read_domains & I915_GEM_DOMAIN_GTT)
		mb();

	return 0;
}

int
i915_gem_object_put_fence(struct drm_i915_gem_object *obj)
{
	int ret;

	if (obj->tiling_mode)
		i915_gem_release_mmap(obj);

	ret = i915_gem_object_flush_fence(obj, NULL);
	if (ret)
		return ret;

	if (obj->fence_reg != I915_FENCE_REG_NONE) {
		struct drm_i915_private *dev_priv = obj->base.dev->dev_private;

		WARN_ON(dev_priv->fence_regs[obj->fence_reg].pin_count);
		i915_gem_clear_fence_reg(obj->base.dev,
					 &dev_priv->fence_regs[obj->fence_reg]);

		obj->fence_reg = I915_FENCE_REG_NONE;
	}

	return 0;
}

static struct drm_i915_fence_reg *
i915_find_fence_reg(struct drm_device *dev,
		    struct intel_ring_buffer *pipelined)
{
	struct drm_i915_private *dev_priv = dev->dev_private;
	struct drm_i915_fence_reg *reg, *first, *avail;
	int i;

	/* First try to find a free reg */
	avail = NULL;
	for (i = dev_priv->fence_reg_start; i < dev_priv->num_fence_regs; i++) {
		reg = &dev_priv->fence_regs[i];
		if (!reg->obj)
			return reg;

		if (!reg->pin_count)
			avail = reg;
	}

	if (avail == NULL)
		return NULL;

	/* None available, try to steal one or wait for a user to finish */
	avail = first = NULL;
	list_for_each_entry(reg, &dev_priv->mm.fence_list, lru_list) {
		if (reg->pin_count)
			continue;

		if (first == NULL)
			first = reg;

		if (!pipelined ||
		    !reg->obj->last_fenced_ring ||
		    reg->obj->last_fenced_ring == pipelined) {
			avail = reg;
			break;
		}
	}

	if (avail == NULL)
		avail = first;

	return avail;
}

static void i915_gem_write_fence__ipi(void *data)
{
	wbinvd();
}

/**
 * i915_gem_object_get_fence - set up a fence reg for an object
 * @obj: object to map through a fence reg
 * @pipelined: ring on which to queue the change, or NULL for CPU access
 * @interruptible: must we wait uninterruptibly for the register to retire?
 *
 * When mapping objects through the GTT, userspace wants to be able to write
 * to them without having to worry about swizzling if the object is tiled.
 *
 * This function walks the fence regs looking for a free one for @obj,
 * stealing one if it can't find any.
 *
 * It then sets up the reg based on the object's properties: address, pitch
 * and tiling format.
 */
int
i915_gem_object_get_fence(struct drm_i915_gem_object *obj,
			  struct intel_ring_buffer *pipelined)
{
	struct drm_device *dev = obj->base.dev;
	struct drm_i915_private *dev_priv = dev->dev_private;
	struct drm_i915_fence_reg *reg;
	int ret;

	/* XXX disable pipelining. There are bugs. Shocking. */
	pipelined = NULL;

	/* Just update our place in the LRU if our fence is getting reused. */
	if (obj->fence_reg != I915_FENCE_REG_NONE) {
		reg = &dev_priv->fence_regs[obj->fence_reg];
		list_move_tail(&reg->lru_list, &dev_priv->mm.fence_list);

		if (obj->tiling_changed) {
			ret = i915_gem_object_flush_fence(obj, pipelined);
			if (ret)
				return ret;

			if (!obj->fenced_gpu_access && !obj->last_fenced_seqno)
				pipelined = NULL;

			if (pipelined) {
				reg->setup_seqno =
					i915_gem_next_request_seqno(pipelined);
				obj->last_fenced_seqno = reg->setup_seqno;
				obj->last_fenced_ring = pipelined;
			}

			goto update;
		}

		if (!pipelined) {
			if (reg->setup_seqno) {
				if (!ring_passed_seqno(obj->last_fenced_ring,
						       reg->setup_seqno)) {
					ret = i915_wait_request(obj->last_fenced_ring,
								reg->setup_seqno,
								true);
					if (ret)
						return ret;
				}

				reg->setup_seqno = 0;
			}
		} else if (obj->last_fenced_ring &&
			   obj->last_fenced_ring != pipelined) {
			ret = i915_gem_object_flush_fence(obj, pipelined);
			if (ret)
				return ret;
		}

		return 0;
	}

	reg = i915_find_fence_reg(dev, pipelined);
	if (reg == NULL)
		return -EDEADLK;

	ret = i915_gem_object_flush_fence(obj, pipelined);
	if (ret)
		return ret;

	if (reg->obj) {
		struct drm_i915_gem_object *old = reg->obj;

		drm_gem_object_reference(&old->base);

		if (old->tiling_mode)
			i915_gem_release_mmap(old);

		ret = i915_gem_object_flush_fence(old, pipelined);
		if (ret) {
			drm_gem_object_unreference(&old->base);
			return ret;
		}

		if (old->last_fenced_seqno == 0 && obj->last_fenced_seqno == 0)
			pipelined = NULL;

		old->fence_reg = I915_FENCE_REG_NONE;
		old->last_fenced_ring = pipelined;
		old->last_fenced_seqno =
			pipelined ? i915_gem_next_request_seqno(pipelined) : 0;

		drm_gem_object_unreference(&old->base);
	} else if (obj->last_fenced_seqno == 0)
		pipelined = NULL;

	reg->obj = obj;
	list_move_tail(&reg->lru_list, &dev_priv->mm.fence_list);
	obj->fence_reg = reg - dev_priv->fence_regs;
	obj->last_fenced_ring = pipelined;

	reg->setup_seqno =
		pipelined ? i915_gem_next_request_seqno(pipelined) : 0;
	obj->last_fenced_seqno = reg->setup_seqno;

update:
	obj->tiling_changed = false;
	switch (INTEL_INFO(dev)->gen) {
	case 7:
	case 6:
		/* In order to fully serialize access to the fenced region and
		 * the update to the fence register we need to take extreme
		 * measures on SNB+. In theory, the write to the fence register
		 * flushes all memory transactions before, and coupled with the
		 * mb() placed around the register write we serialise all memory
		 * operations with respect to the changes in the tiler. Yet, on
		 * SNB+ we need to take a step further and emit an explicit wbinvd()
		 * on each processor in order to manually flush all memory
		 * transactions before updating the fence register.
		 */
		on_each_cpu(i915_gem_write_fence__ipi, NULL, 1);
		ret = sandybridge_write_fence_reg(obj, pipelined);
		break;
	case 5:
	case 4:
		ret = i965_write_fence_reg(obj, pipelined);
		break;
	case 3:
		ret = i915_write_fence_reg(obj, pipelined);
		break;
	case 2:
		ret = i830_write_fence_reg(obj, pipelined);
		break;
	}

	return ret;
}

/**
 * i915_gem_clear_fence_reg - clear out fence register info
 * @obj: object to clear
 *
 * Zeroes out the fence register itself and clears out the associated
 * data structures in dev_priv and obj.
 */
static void
i915_gem_clear_fence_reg(struct drm_device *dev,
			 struct drm_i915_fence_reg *reg)
{
	drm_i915_private_t *dev_priv = dev->dev_private;
	uint32_t fence_reg = reg - dev_priv->fence_regs;

	switch (INTEL_INFO(dev)->gen) {
	case 7:
	case 6:
		I915_WRITE64(FENCE_REG_SANDYBRIDGE_0 + fence_reg*8, 0);
		break;
	case 5:
	case 4:
		I915_WRITE64(FENCE_REG_965_0 + fence_reg*8, 0);
		break;
	case 3:
		if (fence_reg >= 8)
			fence_reg = FENCE_REG_945_8 + (fence_reg - 8) * 4;
		else
	case 2:
			fence_reg = FENCE_REG_830_0 + fence_reg * 4;

		I915_WRITE(fence_reg, 0);
		break;
	}

	list_del_init(&reg->lru_list);
	reg->obj = NULL;
	reg->setup_seqno = 0;
	reg->pin_count = 0;
}

/**
 * Finds free space in the GTT aperture and binds the object there.
 */
static int
i915_gem_object_bind_to_gtt(struct drm_i915_gem_object *obj,
			    unsigned alignment,
			    bool map_and_fenceable)
{
	struct drm_device *dev = obj->base.dev;
	drm_i915_private_t *dev_priv = dev->dev_private;
	struct drm_mm_node *free_space;
	gfp_t gfpmask = __GFP_NORETRY | __GFP_NOWARN;
	u32 size, fence_size, fence_alignment, unfenced_alignment;
	bool mappable, fenceable;
	int ret;

	if (obj->madv != I915_MADV_WILLNEED) {
		DRM_ERROR("Attempting to bind a purgeable object\n");
		return -EINVAL;
	}

	fence_size = i915_gem_get_gtt_size(dev,
					   obj->base.size,
					   obj->tiling_mode);
	fence_alignment = i915_gem_get_gtt_alignment(dev,
						     obj->base.size,
						     obj->tiling_mode);
	unfenced_alignment =
		i915_gem_get_unfenced_gtt_alignment(dev,
						    obj->base.size,
						    obj->tiling_mode);

	if (alignment == 0)
		alignment = map_and_fenceable ? fence_alignment :
						unfenced_alignment;
	if (map_and_fenceable && alignment & (fence_alignment - 1)) {
		DRM_ERROR("Invalid object alignment requested %u\n", alignment);
		return -EINVAL;
	}

	size = map_and_fenceable ? fence_size : obj->base.size;

	/* If the object is bigger than the entire aperture, reject it early
	 * before evicting everything in a vain attempt to find space.
	 */
	if (obj->base.size >
	    (map_and_fenceable ? dev_priv->mm.gtt_mappable_end : dev_priv->mm.gtt_total)) {
		DRM_ERROR("Attempting to bind an object larger than the aperture\n");
		return -E2BIG;
	}

 search_free:
	if (map_and_fenceable)
		free_space =
			drm_mm_search_free_in_range(&dev_priv->mm.gtt_space,
						    size, alignment, 0,
						    dev_priv->mm.gtt_mappable_end,
						    0);
	else
		free_space = drm_mm_search_free(&dev_priv->mm.gtt_space,
						size, alignment, 0);

	if (free_space != NULL) {
		if (map_and_fenceable)
			obj->gtt_space =
				drm_mm_get_block_range_generic(free_space,
							       size, alignment, 0,
							       dev_priv->mm.gtt_mappable_end,
							       0);
		else
			obj->gtt_space =
				drm_mm_get_block(free_space, size, alignment);
	}
	if (obj->gtt_space == NULL) {
		/* If the gtt is empty and we're still having trouble
		 * fitting our object in, we're out of memory.
		 */
		ret = i915_gem_evict_something(dev, size, alignment,
					       map_and_fenceable);
		if (ret)
			return ret;

		goto search_free;
	}

	ret = i915_gem_object_get_pages_gtt(obj, gfpmask);
	if (ret) {
		drm_mm_put_block(obj->gtt_space);
		obj->gtt_space = NULL;

		if (ret == -ENOMEM) {
			/* first try to reclaim some memory by clearing the GTT */
			ret = i915_gem_evict_everything(dev, false);
			if (ret) {
				/* now try to shrink everyone else */
				if (gfpmask) {
					gfpmask = 0;
					goto search_free;
				}

				return -ENOMEM;
			}

			goto search_free;
		}

		return ret;
	}

	ret = i915_gem_gtt_bind_object(obj);
	if (ret) {
		i915_gem_object_put_pages_gtt(obj);
		drm_mm_put_block(obj->gtt_space);
		obj->gtt_space = NULL;

		if (i915_gem_evict_everything(dev, false))
			return ret;

		goto search_free;
	}

	list_add_tail(&obj->gtt_list, &dev_priv->mm.gtt_list);
	list_add_tail(&obj->mm_list, &dev_priv->mm.inactive_list);

	/* Assert that the object is not currently in any GPU domain. As it
	 * wasn't in the GTT, there shouldn't be any way it could have been in
	 * a GPU cache
	 */
	BUG_ON(obj->base.read_domains & I915_GEM_GPU_DOMAINS);
	BUG_ON(obj->base.write_domain & I915_GEM_GPU_DOMAINS);

	obj->gtt_offset = obj->gtt_space->start;

	fenceable =
		obj->gtt_space->size == fence_size &&
		(obj->gtt_space->start & (fence_alignment - 1)) == 0;

	mappable =
		obj->gtt_offset + obj->base.size <= dev_priv->mm.gtt_mappable_end;

	obj->map_and_fenceable = mappable && fenceable;

	trace_i915_gem_object_bind(obj, map_and_fenceable);
	return 0;
}

void
i915_gem_clflush_object(struct drm_i915_gem_object *obj)
{
	/* If we don't have a page list set up, then we're not pinned
	 * to GPU, and we can ignore the cache flush because it'll happen
	 * again at bind time.
	 */
	if (obj->pages == NULL)
		return;

	/* If the GPU is snooping the contents of the CPU cache,
	 * we do not need to manually clear the CPU cache lines.  However,
	 * the caches are only snooped when the render cache is
	 * flushed/invalidated.  As we always have to emit invalidations
	 * and flushes when moving into and out of the RENDER domain, correct
	 * snooping behaviour occurs naturally as the result of our domain
	 * tracking.
	 */
	if (obj->cache_level != I915_CACHE_NONE)
		return;

	trace_i915_gem_object_clflush(obj);

	drm_clflush_pages(obj->pages, obj->base.size / PAGE_SIZE);
}

/** Flushes any GPU write domain for the object if it's dirty. */
static int
i915_gem_object_flush_gpu_write_domain(struct drm_i915_gem_object *obj)
{
	if ((obj->base.write_domain & I915_GEM_GPU_DOMAINS) == 0)
		return 0;

	/* Queue the GPU write cache flushing we need. */
	return i915_gem_flush_ring(obj->ring, 0, obj->base.write_domain);
}

/** Flushes the GTT write domain for the object if it's dirty. */
static void
i915_gem_object_flush_gtt_write_domain(struct drm_i915_gem_object *obj)
{
	uint32_t old_write_domain;

	if (obj->base.write_domain != I915_GEM_DOMAIN_GTT)
		return;

	/* No actual flushing is required for the GTT write domain.  Writes
	 * to it immediately go to main memory as far as we know, so there's
	 * no chipset flush.  It also doesn't land in render cache.
	 *
	 * However, we do have to enforce the order so that all writes through
	 * the GTT land before any writes to the device, such as updates to
	 * the GATT itself.
	 */
	wmb();

	old_write_domain = obj->base.write_domain;
	obj->base.write_domain = 0;

	trace_i915_gem_object_change_domain(obj,
					    obj->base.read_domains,
					    old_write_domain);
}

/** Flushes the CPU write domain for the object if it's dirty. */
static void
i915_gem_object_flush_cpu_write_domain(struct drm_i915_gem_object *obj)
{
	uint32_t old_write_domain;

	if (obj->base.write_domain != I915_GEM_DOMAIN_CPU)
		return;

	i915_gem_clflush_object(obj);
	intel_gtt_chipset_flush();
	old_write_domain = obj->base.write_domain;
	obj->base.write_domain = 0;

	trace_i915_gem_object_change_domain(obj,
					    obj->base.read_domains,
					    old_write_domain);
}

/**
 * Moves a single object to the GTT read, and possibly write domain.
 *
 * This function returns when the move is complete, including waiting on
 * flushes to occur.
 */
int
i915_gem_object_set_to_gtt_domain(struct drm_i915_gem_object *obj, bool write)
{
	uint32_t old_write_domain, old_read_domains;
	int ret;

	/* Not valid to be called on unbound objects. */
	if (obj->gtt_space == NULL)
		return -EINVAL;

	if (obj->base.write_domain == I915_GEM_DOMAIN_GTT)
		return 0;

	ret = i915_gem_object_flush_gpu_write_domain(obj);
	if (ret)
		return ret;

	if (obj->pending_gpu_write || write) {
		ret = i915_gem_object_wait_rendering(obj);
		if (ret)
			return ret;
	}

	i915_gem_object_flush_cpu_write_domain(obj);

	old_write_domain = obj->base.write_domain;
	old_read_domains = obj->base.read_domains;

	/* It should now be out of any other write domains, and we can update
	 * the domain values for our changes.
	 */
	BUG_ON((obj->base.write_domain & ~I915_GEM_DOMAIN_GTT) != 0);
	obj->base.read_domains |= I915_GEM_DOMAIN_GTT;
	if (write) {
		obj->base.read_domains = I915_GEM_DOMAIN_GTT;
		obj->base.write_domain = I915_GEM_DOMAIN_GTT;
		obj->dirty = 1;
	}

	trace_i915_gem_object_change_domain(obj,
					    old_read_domains,
					    old_write_domain);

	return 0;
}

int i915_gem_object_set_cache_level(struct drm_i915_gem_object *obj,
				    enum i915_cache_level cache_level)
{
	struct drm_device *dev = obj->base.dev;
	drm_i915_private_t *dev_priv = dev->dev_private;
	int ret;

	if (obj->cache_level == cache_level)
		return 0;

	if (obj->pin_count) {
		DRM_DEBUG("can not change the cache level of pinned objects\n");
		return -EBUSY;
	}

	if (obj->gtt_space) {
		ret = i915_gem_object_finish_gpu(obj);
		if (ret)
			return ret;

		i915_gem_object_finish_gtt(obj);

		/* Before SandyBridge, you could not use tiling or fence
		 * registers with snooped memory, so relinquish any fences
		 * currently pointing to our region in the aperture.
		 */
		if (INTEL_INFO(obj->base.dev)->gen < 6) {
			ret = i915_gem_object_put_fence(obj);
			if (ret)
				return ret;
		}

		i915_gem_gtt_rebind_object(obj, cache_level);
		if (obj->has_aliasing_ppgtt_mapping)
			i915_ppgtt_bind_object(dev_priv->mm.aliasing_ppgtt,
					       obj, cache_level);
	}

	if (cache_level == I915_CACHE_NONE) {
		u32 old_read_domains, old_write_domain;

		/* If we're coming from LLC cached, then we haven't
		 * actually been tracking whether the data is in the
		 * CPU cache or not, since we only allow one bit set
		 * in obj->write_domain and have been skipping the clflushes.
		 * Just set it to the CPU cache for now.
		 */
		WARN_ON(obj->base.write_domain & ~I915_GEM_DOMAIN_CPU);
		WARN_ON(obj->base.read_domains & ~I915_GEM_DOMAIN_CPU);

		old_read_domains = obj->base.read_domains;
		old_write_domain = obj->base.write_domain;

		obj->base.read_domains = I915_GEM_DOMAIN_CPU;
		obj->base.write_domain = I915_GEM_DOMAIN_CPU;

		trace_i915_gem_object_change_domain(obj,
						    old_read_domains,
						    old_write_domain);
	}

	obj->cache_level = cache_level;
	return 0;
}

/*
 * Prepare buffer for display plane (scanout, cursors, etc).
 * Can be called from an uninterruptible phase (modesetting) and allows
 * any flushes to be pipelined (for pageflips).
 *
 * For the display plane, we want to be in the GTT but out of any write
 * domains. So in many ways this looks like set_to_gtt_domain() apart from the
 * ability to pipeline the waits, pinning and any additional subtleties
 * that may differentiate the display plane from ordinary buffers.
 */
int
i915_gem_object_pin_to_display_plane(struct drm_i915_gem_object *obj,
				     u32 alignment,
				     struct intel_ring_buffer *pipelined)
{
	u32 old_read_domains, old_write_domain;
	int ret;

	ret = i915_gem_object_flush_gpu_write_domain(obj);
	if (ret)
		return ret;

	if (pipelined != obj->ring) {
		ret = i915_gem_object_wait_rendering(obj);
		if (ret == -ERESTARTSYS)
			return ret;
	}

	/* The display engine is not coherent with the LLC cache on gen6.  As
	 * a result, we make sure that the pinning that is about to occur is
	 * done with uncached PTEs. This is lowest common denominator for all
	 * chipsets.
	 *
	 * However for gen6+, we could do better by using the GFDT bit instead
	 * of uncaching, which would allow us to flush all the LLC-cached data
	 * with that bit in the PTE to main memory with just one PIPE_CONTROL.
	 */
	ret = i915_gem_object_set_cache_level(obj, I915_CACHE_NONE);
	if (ret)
		return ret;

	/* As the user may map the buffer once pinned in the display plane
	 * (e.g. libkms for the bootup splash), we have to ensure that we
	 * always use map_and_fenceable for all scanout buffers.
	 */
	ret = i915_gem_object_pin(obj, alignment, true);
	if (ret)
		return ret;

	i915_gem_object_flush_cpu_write_domain(obj);

	old_write_domain = obj->base.write_domain;
	old_read_domains = obj->base.read_domains;

	/* It should now be out of any other write domains, and we can update
	 * the domain values for our changes.
	 */
	BUG_ON((obj->base.write_domain & ~I915_GEM_DOMAIN_GTT) != 0);
	obj->base.read_domains |= I915_GEM_DOMAIN_GTT;

	trace_i915_gem_object_change_domain(obj,
					    old_read_domains,
					    old_write_domain);

	return 0;
}

int
i915_gem_object_finish_gpu(struct drm_i915_gem_object *obj)
{
	int ret;

	if ((obj->base.read_domains & I915_GEM_GPU_DOMAINS) == 0)
		return 0;

	if (obj->base.write_domain & I915_GEM_GPU_DOMAINS) {
		ret = i915_gem_flush_ring(obj->ring, 0, obj->base.write_domain);
		if (ret)
			return ret;
	}

	ret = i915_gem_object_wait_rendering(obj);
	if (ret)
		return ret;

	/* Ensure that we invalidate the GPU's caches and TLBs. */
	obj->base.read_domains &= ~I915_GEM_GPU_DOMAINS;
	return 0;
}

/**
 * Moves a single object to the CPU read, and possibly write domain.
 *
 * This function returns when the move is complete, including waiting on
 * flushes to occur.
 */
static int
i915_gem_object_set_to_cpu_domain(struct drm_i915_gem_object *obj, bool write)
{
	uint32_t old_write_domain, old_read_domains;
	int ret;

	if (obj->base.write_domain == I915_GEM_DOMAIN_CPU)
		return 0;

	ret = i915_gem_object_flush_gpu_write_domain(obj);
	if (ret)
		return ret;

	ret = i915_gem_object_wait_rendering(obj);
	if (ret)
		return ret;

	i915_gem_object_flush_gtt_write_domain(obj);

	/* If we have a partially-valid cache of the object in the CPU,
	 * finish invalidating it and free the per-page flags.
	 */
	i915_gem_object_set_to_full_cpu_read_domain(obj);

	old_write_domain = obj->base.write_domain;
	old_read_domains = obj->base.read_domains;

	/* Flush the CPU cache if it's still invalid. */
	if ((obj->base.read_domains & I915_GEM_DOMAIN_CPU) == 0) {
		i915_gem_clflush_object(obj);

		obj->base.read_domains |= I915_GEM_DOMAIN_CPU;
	}

	/* It should now be out of any other write domains, and we can update
	 * the domain values for our changes.
	 */
	BUG_ON((obj->base.write_domain & ~I915_GEM_DOMAIN_CPU) != 0);

	/* If we're writing through the CPU, then the GPU read domains will
	 * need to be invalidated at next use.
	 */
	if (write) {
		obj->base.read_domains = I915_GEM_DOMAIN_CPU;
		obj->base.write_domain = I915_GEM_DOMAIN_CPU;
	}

	trace_i915_gem_object_change_domain(obj,
					    old_read_domains,
					    old_write_domain);

	return 0;
}

/**
 * Moves the object from a partially CPU read to a full one.
 *
 * Note that this only resolves i915_gem_object_set_cpu_read_domain_range(),
 * and doesn't handle transitioning from !(read_domains & I915_GEM_DOMAIN_CPU).
 */
static void
i915_gem_object_set_to_full_cpu_read_domain(struct drm_i915_gem_object *obj)
{
	if (!obj->page_cpu_valid)
		return;

	/* If we're partially in the CPU read domain, finish moving it in.
	 */
	if (obj->base.read_domains & I915_GEM_DOMAIN_CPU) {
		int i;

		for (i = 0; i <= (obj->base.size - 1) / PAGE_SIZE; i++) {
			if (obj->page_cpu_valid[i])
				continue;
			drm_clflush_pages(obj->pages + i, 1);
		}
	}

	/* Free the page_cpu_valid mappings which are now stale, whether
	 * or not we've got I915_GEM_DOMAIN_CPU.
	 */
	kfree(obj->page_cpu_valid);
	obj->page_cpu_valid = NULL;
}

/**
 * Set the CPU read domain on a range of the object.
 *
 * The object ends up with I915_GEM_DOMAIN_CPU in its read flags although it's
 * not entirely valid.  The page_cpu_valid member of the object flags which
 * pages have been flushed, and will be respected by
 * i915_gem_object_set_to_cpu_domain() if it's called on to get a valid mapping
 * of the whole object.
 *
 * This function returns when the move is complete, including waiting on
 * flushes to occur.
 */
static int
i915_gem_object_set_cpu_read_domain_range(struct drm_i915_gem_object *obj,
					  uint64_t offset, uint64_t size)
{
	uint32_t old_read_domains;
	int i, ret;

	if (offset == 0 && size == obj->base.size)
		return i915_gem_object_set_to_cpu_domain(obj, 0);

	ret = i915_gem_object_flush_gpu_write_domain(obj);
	if (ret)
		return ret;

	ret = i915_gem_object_wait_rendering(obj);
	if (ret)
		return ret;

	i915_gem_object_flush_gtt_write_domain(obj);

	/* If we're already fully in the CPU read domain, we're done. */
	if (obj->page_cpu_valid == NULL &&
	    (obj->base.read_domains & I915_GEM_DOMAIN_CPU) != 0)
		return 0;

	/* Otherwise, create/clear the per-page CPU read domain flag if we're
	 * newly adding I915_GEM_DOMAIN_CPU
	 */
	if (obj->page_cpu_valid == NULL) {
		obj->page_cpu_valid = kzalloc(obj->base.size / PAGE_SIZE,
					      GFP_KERNEL);
		if (obj->page_cpu_valid == NULL)
			return -ENOMEM;
	} else if ((obj->base.read_domains & I915_GEM_DOMAIN_CPU) == 0)
		memset(obj->page_cpu_valid, 0, obj->base.size / PAGE_SIZE);

	/* Flush the cache on any pages that are still invalid from the CPU's
	 * perspective.
	 */
	for (i = offset / PAGE_SIZE; i <= (offset + size - 1) / PAGE_SIZE;
	     i++) {
		if (obj->page_cpu_valid[i])
			continue;

		drm_clflush_pages(obj->pages + i, 1);

		obj->page_cpu_valid[i] = 1;
	}

	/* It should now be out of any other write domains, and we can update
	 * the domain values for our changes.
	 */
	BUG_ON((obj->base.write_domain & ~I915_GEM_DOMAIN_CPU) != 0);

	old_read_domains = obj->base.read_domains;
	obj->base.read_domains |= I915_GEM_DOMAIN_CPU;

	trace_i915_gem_object_change_domain(obj,
					    old_read_domains,
					    obj->base.write_domain);

	return 0;
}

/* Throttle our rendering by waiting until the ring has completed our requests
 * emitted over 20 msec ago.
 *
 * Note that if we were to use the current jiffies each time around the loop,
 * we wouldn't escape the function with any frames outstanding if the time to
 * render a frame was over 20ms.
 *
 * This should get us reasonable parallelism between CPU and GPU but also
 * relatively low latency when blocking on a particular request to finish.
 */
static int
i915_gem_ring_throttle(struct drm_device *dev, struct drm_file *file)
{
	struct drm_i915_private *dev_priv = dev->dev_private;
	struct drm_i915_file_private *file_priv = file->driver_priv;
	unsigned long recent_enough = jiffies - msecs_to_jiffies(20);
	struct drm_i915_gem_request *request;
	struct intel_ring_buffer *ring = NULL;
	u32 seqno = 0;
	int ret;

	if (atomic_read(&dev_priv->mm.wedged))
		return -EIO;

	spin_lock(&file_priv->mm.lock);
	list_for_each_entry(request, &file_priv->mm.request_list, client_list) {
		if (time_after_eq(request->emitted_jiffies, recent_enough))
			break;

		ring = request->ring;
		seqno = request->seqno;
	}
	spin_unlock(&file_priv->mm.lock);

	if (seqno == 0)
		return 0;

	ret = 0;
	if (!i915_seqno_passed(ring->get_seqno(ring), seqno)) {
		/* And wait for the seqno passing without holding any locks and
		 * causing extra latency for others. This is safe as the irq
		 * generation is designed to be run atomically and so is
		 * lockless.
		 */
		if (ring->irq_get(ring)) {
			ret = wait_event_interruptible(ring->irq_queue,
						       i915_seqno_passed(ring->get_seqno(ring), seqno)
						       || atomic_read(&dev_priv->mm.wedged));
			ring->irq_put(ring);

			if (ret == 0 && atomic_read(&dev_priv->mm.wedged))
				ret = -EIO;
		} else if (wait_for_atomic(i915_seqno_passed(ring->get_seqno(ring),
							     seqno) ||
				    atomic_read(&dev_priv->mm.wedged), 3000)) {
			ret = -EBUSY;
		}
	}

	if (ret == 0)
		queue_delayed_work(dev_priv->wq, &dev_priv->mm.retire_work, 0);

	return ret;
}

int
i915_gem_object_pin(struct drm_i915_gem_object *obj,
		    uint32_t alignment,
		    bool map_and_fenceable)
{
	struct drm_device *dev = obj->base.dev;
	struct drm_i915_private *dev_priv = dev->dev_private;
	int ret;

	if (WARN_ON(obj->pin_count == DRM_I915_GEM_OBJECT_MAX_PIN_COUNT))
		return -EBUSY;
	WARN_ON(i915_verify_lists(dev));

	if (obj->gtt_space != NULL) {
		if ((alignment && obj->gtt_offset & (alignment - 1)) ||
		    (map_and_fenceable && !obj->map_and_fenceable)) {
			WARN(obj->pin_count,
			     "bo is already pinned with incorrect alignment:"
			     " offset=%x, req.alignment=%x, req.map_and_fenceable=%d,"
			     " obj->map_and_fenceable=%d\n",
			     obj->gtt_offset, alignment,
			     map_and_fenceable,
			     obj->map_and_fenceable);
			ret = i915_gem_object_unbind(obj);
			if (ret)
				return ret;
		}
	}

	if (obj->gtt_space == NULL) {
		ret = i915_gem_object_bind_to_gtt(obj, alignment,
						  map_and_fenceable);
		if (ret)
			return ret;
	}

	if (obj->pin_count++ == 0) {
		if (!obj->active)
			list_move_tail(&obj->mm_list,
				       &dev_priv->mm.pinned_list);
	}
	obj->pin_mappable |= map_and_fenceable;

	WARN_ON(i915_verify_lists(dev));
	return 0;
}

void
i915_gem_object_unpin(struct drm_i915_gem_object *obj)
{
	struct drm_device *dev = obj->base.dev;
	drm_i915_private_t *dev_priv = dev->dev_private;

	WARN_ON(i915_verify_lists(dev));
	BUG_ON(obj->pin_count == 0);
	BUG_ON(obj->gtt_space == NULL);

	if (--obj->pin_count == 0) {
		if (!obj->active)
			list_move_tail(&obj->mm_list,
				       &dev_priv->mm.inactive_list);
		obj->pin_mappable = false;
	}
	WARN_ON(i915_verify_lists(dev));
}

int
i915_gem_pin_ioctl(struct drm_device *dev, void *data,
		   struct drm_file *file)
{
	struct drm_i915_gem_pin *args = data;
	struct drm_i915_gem_object *obj;
	int ret;

	ret = i915_mutex_lock_interruptible(dev);
	if (ret)
		return ret;

	obj = to_intel_bo(drm_gem_object_lookup(dev, file, args->handle));
	if (&obj->base == NULL) {
		ret = -ENOENT;
		goto unlock;
	}

	if (obj->madv != I915_MADV_WILLNEED) {
		DRM_ERROR("Attempting to pin a purgeable buffer\n");
		ret = -EINVAL;
		goto out;
	}

	if (obj->pin_filp != NULL && obj->pin_filp != file) {
		DRM_ERROR("Already pinned in i915_gem_pin_ioctl(): %d\n",
			  args->handle);
		ret = -EINVAL;
		goto out;
	}

	if (obj->user_pin_count == 0) {
		ret = i915_gem_object_pin(obj, args->alignment, true);
		if (ret)
			goto out;
	}

	obj->user_pin_count++;
	obj->pin_filp = file;

	/* XXX - flush the CPU caches for pinned objects
	 * as the X server doesn't manage domains yet
	 */
	i915_gem_object_flush_cpu_write_domain(obj);
	args->offset = obj->gtt_offset;
out:
	drm_gem_object_unreference(&obj->base);
unlock:
	mutex_unlock(&dev->struct_mutex);
	return ret;
}

int
i915_gem_unpin_ioctl(struct drm_device *dev, void *data,
		     struct drm_file *file)
{
	struct drm_i915_gem_pin *args = data;
	struct drm_i915_gem_object *obj;
	int ret;

	ret = i915_mutex_lock_interruptible(dev);
	if (ret)
		return ret;

	obj = to_intel_bo(drm_gem_object_lookup(dev, file, args->handle));
	if (&obj->base == NULL) {
		ret = -ENOENT;
		goto unlock;
	}

	if (obj->pin_filp != file) {
		DRM_ERROR("Not pinned by caller in i915_gem_pin_ioctl(): %d\n",
			  args->handle);
		ret = -EINVAL;
		goto out;
	}
	obj->user_pin_count--;
	if (obj->user_pin_count == 0) {
		obj->pin_filp = NULL;
		i915_gem_object_unpin(obj);
	}

out:
	drm_gem_object_unreference(&obj->base);
unlock:
	mutex_unlock(&dev->struct_mutex);
	return ret;
}

int
i915_gem_busy_ioctl(struct drm_device *dev, void *data,
		    struct drm_file *file)
{
	struct drm_i915_gem_busy *args = data;
	struct drm_i915_gem_object *obj;
	int ret;

	ret = i915_mutex_lock_interruptible(dev);
	if (ret)
		return ret;

	obj = to_intel_bo(drm_gem_object_lookup(dev, file, args->handle));
	if (&obj->base == NULL) {
		ret = -ENOENT;
		goto unlock;
	}

	/* Count all active objects as busy, even if they are currently not used
	 * by the gpu. Users of this interface expect objects to eventually
	 * become non-busy without any further actions, therefore emit any
	 * necessary flushes here.
	 */
	args->busy = obj->active;
	if (args->busy) {
		/* Unconditionally flush objects, even when the gpu still uses this
		 * object. Userspace calling this function indicates that it wants to
		 * use this buffer rather sooner than later, so issuing the required
		 * flush earlier is beneficial.
		 */
		if (obj->base.write_domain & I915_GEM_GPU_DOMAINS) {
			ret = i915_gem_flush_ring(obj->ring,
						  0, obj->base.write_domain);
		} else if (obj->ring->outstanding_lazy_request ==
			   obj->last_rendering_seqno) {
			struct drm_i915_gem_request *request;

			/* This ring is not being cleared by active usage,
			 * so emit a request to do so.
			 */
			request = kzalloc(sizeof(*request), GFP_KERNEL);
			if (request) {
				ret = i915_add_request(obj->ring, NULL, request);
				if (ret)
					kfree(request);
			} else
				ret = -ENOMEM;
		}

		/* Update the active list for the hardware's current position.
		 * Otherwise this only updates on a delayed timer or when irqs
		 * are actually unmasked, and our working set ends up being
		 * larger than required.
		 */
		i915_gem_retire_requests_ring(obj->ring);

		args->busy = obj->active;
	}

	drm_gem_object_unreference(&obj->base);
unlock:
	mutex_unlock(&dev->struct_mutex);
	return ret;
}

int
i915_gem_throttle_ioctl(struct drm_device *dev, void *data,
			struct drm_file *file_priv)
{
	return i915_gem_ring_throttle(dev, file_priv);
=======
void i915_gem_runtime_suspend(struct drm_i915_private *i915)
{
	struct drm_i915_gem_object *obj, *on;
	int i;

	/*
	 * Only called during RPM suspend. All users of the userfault_list
	 * must be holding an RPM wakeref to ensure that this can not
	 * run concurrently with themselves (and use the struct_mutex for
	 * protection between themselves).
	 */

	list_for_each_entry_safe(obj, on,
				 &to_gt(i915)->ggtt->userfault_list, userfault_link)
		__i915_gem_object_release_mmap_gtt(obj);

	list_for_each_entry_safe(obj, on,
				 &i915->runtime_pm.lmem_userfault_list, userfault_link)
		i915_gem_object_runtime_pm_release_mmap_offset(obj);

	/*
	 * The fence will be lost when the device powers down. If any were
	 * in use by hardware (i.e. they are pinned), we should not be powering
	 * down! All other fences will be reacquired by the user upon waking.
	 */
	for (i = 0; i < to_gt(i915)->ggtt->num_fences; i++) {
		struct i915_fence_reg *reg = &to_gt(i915)->ggtt->fence_regs[i];

		/*
		 * Ideally we want to assert that the fence register is not
		 * live at this point (i.e. that no piece of code will be
		 * trying to write through fence + GTT, as that both violates
		 * our tracking of activity and associated locking/barriers,
		 * but also is illegal given that the hw is powered down).
		 *
		 * Previously we used reg->pin_count as a "liveness" indicator.
		 * That is not sufficient, and we need a more fine-grained
		 * tool if we want to have a sanity check here.
		 */

		if (!reg->vma)
			continue;

		GEM_BUG_ON(i915_vma_has_userfault(reg->vma));
		reg->dirty = true;
	}
}

static void discard_ggtt_vma(struct i915_vma *vma)
{
	struct drm_i915_gem_object *obj = vma->obj;

	spin_lock(&obj->vma.lock);
	if (!RB_EMPTY_NODE(&vma->obj_node)) {
		rb_erase(&vma->obj_node, &obj->vma.tree);
		RB_CLEAR_NODE(&vma->obj_node);
	}
	spin_unlock(&obj->vma.lock);
}

struct i915_vma *
i915_gem_object_ggtt_pin_ww(struct drm_i915_gem_object *obj,
			    struct i915_gem_ww_ctx *ww,
			    const struct i915_gtt_view *view,
			    u64 size, u64 alignment, u64 flags)
{
	struct drm_i915_private *i915 = to_i915(obj->base.dev);
	struct i915_ggtt *ggtt = to_gt(i915)->ggtt;
	struct i915_vma *vma;
	int ret;

	GEM_WARN_ON(!ww);

	if (flags & PIN_MAPPABLE &&
	    (!view || view->type == I915_GTT_VIEW_NORMAL)) {
		/*
		 * If the required space is larger than the available
		 * aperture, we will not able to find a slot for the
		 * object and unbinding the object now will be in
		 * vain. Worse, doing so may cause us to ping-pong
		 * the object in and out of the Global GTT and
		 * waste a lot of cycles under the mutex.
		 */
		if (obj->base.size > ggtt->mappable_end)
			return ERR_PTR(-E2BIG);

		/*
		 * If NONBLOCK is set the caller is optimistically
		 * trying to cache the full object within the mappable
		 * aperture, and *must* have a fallback in place for
		 * situations where we cannot bind the object. We
		 * can be a little more lax here and use the fallback
		 * more often to avoid costly migrations of ourselves
		 * and other objects within the aperture.
		 *
		 * Half-the-aperture is used as a simple heuristic.
		 * More interesting would to do search for a free
		 * block prior to making the commitment to unbind.
		 * That caters for the self-harm case, and with a
		 * little more heuristics (e.g. NOFAULT, NOEVICT)
		 * we could try to minimise harm to others.
		 */
		if (flags & PIN_NONBLOCK &&
		    obj->base.size > ggtt->mappable_end / 2)
			return ERR_PTR(-ENOSPC);
	}

new_vma:
	vma = i915_vma_instance(obj, &ggtt->vm, view);
	if (IS_ERR(vma))
		return vma;

	if (i915_vma_misplaced(vma, size, alignment, flags)) {
		if (flags & PIN_NONBLOCK) {
			if (i915_vma_is_pinned(vma) || i915_vma_is_active(vma))
				return ERR_PTR(-ENOSPC);

			/*
			 * If this misplaced vma is too big (i.e, at-least
			 * half the size of aperture) or hasn't been pinned
			 * mappable before, we ignore the misplacement when
			 * PIN_NONBLOCK is set in order to avoid the ping-pong
			 * issue described above. In other words, we try to
			 * avoid the costly operation of unbinding this vma
			 * from the GGTT and rebinding it back because there
			 * may not be enough space for this vma in the aperture.
			 */
			if (flags & PIN_MAPPABLE &&
			    (vma->fence_size > ggtt->mappable_end / 2 ||
			    !i915_vma_is_map_and_fenceable(vma)))
				return ERR_PTR(-ENOSPC);
		}

		if (i915_vma_is_pinned(vma) || i915_vma_is_active(vma)) {
			discard_ggtt_vma(vma);
			goto new_vma;
		}

		ret = i915_vma_unbind(vma);
		if (ret)
			return ERR_PTR(ret);
	}

	ret = i915_vma_pin_ww(vma, ww, size, alignment, flags | PIN_GLOBAL);

	if (ret)
		return ERR_PTR(ret);

	if (vma->fence && !i915_gem_object_is_tiled(obj)) {
		mutex_lock(&ggtt->vm.mutex);
		i915_vma_revoke_fence(vma);
		mutex_unlock(&ggtt->vm.mutex);
	}

	ret = i915_vma_wait_for_bind(vma);
	if (ret) {
		i915_vma_unpin(vma);
		return ERR_PTR(ret);
	}

	return vma;
}

struct i915_vma * __must_check
i915_gem_object_ggtt_pin(struct drm_i915_gem_object *obj,
			 const struct i915_gtt_view *view,
			 u64 size, u64 alignment, u64 flags)
{
	struct i915_gem_ww_ctx ww;
	struct i915_vma *ret;
	int err;

	for_i915_gem_ww(&ww, err, true) {
		err = i915_gem_object_lock(obj, &ww);
		if (err)
			continue;

		ret = i915_gem_object_ggtt_pin_ww(obj, &ww, view, size,
						  alignment, flags);
		if (IS_ERR(ret))
			err = PTR_ERR(ret);
	}

	return err ? ERR_PTR(err) : ret;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

int
i915_gem_madvise_ioctl(struct drm_device *dev, void *data,
		       struct drm_file *file_priv)
{
<<<<<<< HEAD
	struct drm_i915_gem_madvise *args = data;
	struct drm_i915_gem_object *obj;
	int ret;
=======
	struct drm_i915_private *i915 = to_i915(dev);
	struct drm_i915_gem_madvise *args = data;
	struct drm_i915_gem_object *obj;
	int err;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	switch (args->madv) {
	case I915_MADV_DONTNEED:
	case I915_MADV_WILLNEED:
	    break;
	default:
	    return -EINVAL;
	}

<<<<<<< HEAD
	ret = i915_mutex_lock_interruptible(dev);
	if (ret)
		return ret;

	obj = to_intel_bo(drm_gem_object_lookup(dev, file_priv, args->handle));
	if (&obj->base == NULL) {
		ret = -ENOENT;
		goto unlock;
	}

	if (obj->pin_count) {
		ret = -EINVAL;
		goto out;
	}

	if (obj->madv != __I915_MADV_PURGED)
		obj->madv = args->madv;

	/* if the object is no longer bound, discard its backing storage */
	if (i915_gem_object_is_purgeable(obj) &&
	    obj->gtt_space == NULL)
		i915_gem_object_truncate(obj);

	args->retained = obj->madv != __I915_MADV_PURGED;

out:
	drm_gem_object_unreference(&obj->base);
unlock:
	mutex_unlock(&dev->struct_mutex);
	return ret;
}

struct drm_i915_gem_object *i915_gem_alloc_object(struct drm_device *dev,
						  size_t size)
{
	struct drm_i915_private *dev_priv = dev->dev_private;
	struct drm_i915_gem_object *obj;
	struct address_space *mapping;

	obj = kzalloc(sizeof(*obj), GFP_KERNEL);
	if (obj == NULL)
		return NULL;

	if (drm_gem_object_init(dev, &obj->base, size) != 0) {
		kfree(obj);
		return NULL;
	}

	mapping = obj->base.filp->f_path.dentry->d_inode->i_mapping;
	mapping_set_gfp_mask(mapping, GFP_HIGHUSER | __GFP_RECLAIMABLE);

	i915_gem_info_add_obj(dev_priv, size);

	obj->base.write_domain = I915_GEM_DOMAIN_CPU;
	obj->base.read_domains = I915_GEM_DOMAIN_CPU;

	if (HAS_LLC(dev)) {
		/* On some devices, we can have the GPU use the LLC (the CPU
		 * cache) for about a 10% performance improvement
		 * compared to uncached.  Graphics requests other than
		 * display scanout are coherent with the CPU in
		 * accessing this cache.  This means in this mode we
		 * don't need to clflush on the CPU side, and on the
		 * GPU side we only need to flush internal caches to
		 * get data visible to the CPU.
		 *
		 * However, we maintain the display planes as UC, and so
		 * need to rebind when first used as such.
		 */
		obj->cache_level = I915_CACHE_LLC;
	} else
		obj->cache_level = I915_CACHE_NONE;

	obj->base.driver_private = NULL;
	obj->fence_reg = I915_FENCE_REG_NONE;
	INIT_LIST_HEAD(&obj->mm_list);
	INIT_LIST_HEAD(&obj->gtt_list);
	INIT_LIST_HEAD(&obj->ring_list);
	INIT_LIST_HEAD(&obj->exec_list);
	INIT_LIST_HEAD(&obj->gpu_write_list);
	obj->madv = I915_MADV_WILLNEED;
	/* Avoid an unnecessary call to unbind on the first bind. */
	obj->map_and_fenceable = true;

	return obj;
}

int i915_gem_init_object(struct drm_gem_object *obj)
{
	BUG();

	return 0;
}

static void i915_gem_free_object_tail(struct drm_i915_gem_object *obj)
{
	struct drm_device *dev = obj->base.dev;
	drm_i915_private_t *dev_priv = dev->dev_private;
	int ret;

	ret = i915_gem_object_unbind(obj);
	if (ret == -ERESTARTSYS) {
		list_move(&obj->mm_list,
			  &dev_priv->mm.deferred_free_list);
		return;
	}

	trace_i915_gem_object_destroy(obj);

	if (obj->base.map_list.map)
		drm_gem_free_mmap_offset(&obj->base);

	drm_gem_object_release(&obj->base);
	i915_gem_info_remove_obj(dev_priv, obj->base.size);

	kfree(obj->page_cpu_valid);
	kfree(obj->bit_17);
	kfree(obj);
}

void i915_gem_free_object(struct drm_gem_object *gem_obj)
{
	struct drm_i915_gem_object *obj = to_intel_bo(gem_obj);
	struct drm_device *dev = obj->base.dev;

	while (obj->pin_count > 0)
		i915_gem_object_unpin(obj);

	if (obj->phys_obj)
		i915_gem_detach_phys_object(dev, obj);

	i915_gem_free_object_tail(obj);
}

int
i915_gem_idle(struct drm_device *dev)
{
	drm_i915_private_t *dev_priv = dev->dev_private;
	int ret;

	mutex_lock(&dev->struct_mutex);

	if (dev_priv->mm.suspended) {
		mutex_unlock(&dev->struct_mutex);
		return 0;
	}

	ret = i915_gpu_idle(dev, true);
	if (ret) {
		mutex_unlock(&dev->struct_mutex);
		return ret;
	}

	/* Under UMS, be paranoid and evict. */
	if (!drm_core_check_feature(dev, DRIVER_MODESET)) {
		ret = i915_gem_evict_inactive(dev, false);
		if (ret) {
			mutex_unlock(&dev->struct_mutex);
			return ret;
		}
	}

	i915_gem_reset_fences(dev);

	/* Hack!  Don't let anybody do execbuf while we don't control the chip.
	 * We need to replace this with a semaphore, or something.
	 * And not confound mm.suspended!
	 */
	dev_priv->mm.suspended = 1;
	del_timer_sync(&dev_priv->hangcheck_timer);

	i915_kernel_lost_context(dev);
	i915_gem_cleanup_ringbuffer(dev);

	mutex_unlock(&dev->struct_mutex);

	/* Cancel the retire work handler, which should be idle now. */
	cancel_delayed_work_sync(&dev_priv->mm.retire_work);

	return 0;
}

void i915_gem_init_swizzling(struct drm_device *dev)
{
	drm_i915_private_t *dev_priv = dev->dev_private;

	if (INTEL_INFO(dev)->gen < 5 ||
	    dev_priv->mm.bit_6_swizzle_x == I915_BIT_6_SWIZZLE_NONE)
		return;

	I915_WRITE(DISP_ARB_CTL, I915_READ(DISP_ARB_CTL) |
				 DISP_TILE_SURFACE_SWIZZLING);

	if (IS_GEN5(dev))
		return;

	I915_WRITE(TILECTL, I915_READ(TILECTL) | TILECTL_SWZCTL);
	if (IS_GEN6(dev))
		I915_WRITE(ARB_MODE, ARB_MODE_ENABLE(ARB_MODE_SWIZZLE_SNB));
	else
		I915_WRITE(ARB_MODE, ARB_MODE_ENABLE(ARB_MODE_SWIZZLE_IVB));
}

void i915_gem_init_ppgtt(struct drm_device *dev)
{
	drm_i915_private_t *dev_priv = dev->dev_private;
	uint32_t pd_offset;
	struct intel_ring_buffer *ring;
	struct i915_hw_ppgtt *ppgtt = dev_priv->mm.aliasing_ppgtt;
	uint32_t __iomem *pd_addr;
	uint32_t pd_entry;
	int i;

	if (!dev_priv->mm.aliasing_ppgtt)
		return;


	pd_addr = dev_priv->mm.gtt->gtt + ppgtt->pd_offset/sizeof(uint32_t);
	for (i = 0; i < ppgtt->num_pd_entries; i++) {
		dma_addr_t pt_addr;

		if (dev_priv->mm.gtt->needs_dmar)
			pt_addr = ppgtt->pt_dma_addr[i];
		else
			pt_addr = page_to_phys(ppgtt->pt_pages[i]);

		pd_entry = GEN6_PDE_ADDR_ENCODE(pt_addr);
		pd_entry |= GEN6_PDE_VALID;

		writel(pd_entry, pd_addr + i);
	}
	readl(pd_addr);

	pd_offset = ppgtt->pd_offset;
	pd_offset /= 64; /* in cachelines, */
	pd_offset <<= 16;

	if (INTEL_INFO(dev)->gen == 6) {
		uint32_t ecochk = I915_READ(GAM_ECOCHK);
		I915_WRITE(GAM_ECOCHK, ecochk | ECOCHK_SNB_BIT |
				       ECOCHK_PPGTT_CACHE64B);
		I915_WRITE(GFX_MODE, GFX_MODE_ENABLE(GFX_PPGTT_ENABLE));
	} else if (INTEL_INFO(dev)->gen >= 7) {
		I915_WRITE(GAM_ECOCHK, ECOCHK_PPGTT_CACHE64B);
		/* GFX_MODE is per-ring on gen7+ */
	}

	for (i = 0; i < I915_NUM_RINGS; i++) {
		ring = &dev_priv->ring[i];

		if (INTEL_INFO(dev)->gen >= 7)
			I915_WRITE(RING_MODE_GEN7(ring),
				   GFX_MODE_ENABLE(GFX_PPGTT_ENABLE));

		I915_WRITE(RING_PP_DIR_DCLV(ring), PP_DIR_DCLV_2G);
		I915_WRITE(RING_PP_DIR_BASE(ring), pd_offset);
	}
}

int
i915_gem_init_hw(struct drm_device *dev)
{
	drm_i915_private_t *dev_priv = dev->dev_private;
	int ret;

	i915_gem_init_swizzling(dev);

	ret = intel_init_render_ring_buffer(dev);
	if (ret)
		return ret;

	if (HAS_BSD(dev)) {
		ret = intel_init_bsd_ring_buffer(dev);
		if (ret)
			goto cleanup_render_ring;
	}

	if (HAS_BLT(dev)) {
		ret = intel_init_blt_ring_buffer(dev);
		if (ret)
			goto cleanup_bsd_ring;
	}

	dev_priv->next_seqno = 1;

	i915_gem_init_ppgtt(dev);

	return 0;

cleanup_bsd_ring:
	intel_cleanup_ring_buffer(&dev_priv->ring[VCS]);
cleanup_render_ring:
	intel_cleanup_ring_buffer(&dev_priv->ring[RCS]);
	return ret;
}

void
i915_gem_cleanup_ringbuffer(struct drm_device *dev)
{
	drm_i915_private_t *dev_priv = dev->dev_private;
	int i;

	for (i = 0; i < I915_NUM_RINGS; i++)
		intel_cleanup_ring_buffer(&dev_priv->ring[i]);
}

int
i915_gem_entervt_ioctl(struct drm_device *dev, void *data,
		       struct drm_file *file_priv)
{
	drm_i915_private_t *dev_priv = dev->dev_private;
	int ret, i;

	if (drm_core_check_feature(dev, DRIVER_MODESET))
		return 0;

	if (atomic_read(&dev_priv->mm.wedged)) {
		DRM_ERROR("Reenabling wedged hardware, good luck\n");
		atomic_set(&dev_priv->mm.wedged, 0);
	}

	mutex_lock(&dev->struct_mutex);
	dev_priv->mm.suspended = 0;

	ret = i915_gem_init_hw(dev);
	if (ret != 0) {
		mutex_unlock(&dev->struct_mutex);
		return ret;
	}

	BUG_ON(!list_empty(&dev_priv->mm.active_list));
	BUG_ON(!list_empty(&dev_priv->mm.flushing_list));
	BUG_ON(!list_empty(&dev_priv->mm.inactive_list));
	for (i = 0; i < I915_NUM_RINGS; i++) {
		BUG_ON(!list_empty(&dev_priv->ring[i].active_list));
		BUG_ON(!list_empty(&dev_priv->ring[i].request_list));
	}
	mutex_unlock(&dev->struct_mutex);

	ret = drm_irq_install(dev);
	if (ret)
		goto cleanup_ringbuffer;

	return 0;

cleanup_ringbuffer:
	mutex_lock(&dev->struct_mutex);
	i915_gem_cleanup_ringbuffer(dev);
	dev_priv->mm.suspended = 1;
	mutex_unlock(&dev->struct_mutex);

	return ret;
}

int
i915_gem_leavevt_ioctl(struct drm_device *dev, void *data,
		       struct drm_file *file_priv)
{
	if (drm_core_check_feature(dev, DRIVER_MODESET))
		return 0;

	drm_irq_uninstall(dev);
	return i915_gem_idle(dev);
}

void
i915_gem_lastclose(struct drm_device *dev)
{
	int ret;

	if (drm_core_check_feature(dev, DRIVER_MODESET))
		return;

	ret = i915_gem_idle(dev);
	if (ret)
		DRM_ERROR("failed to idle hardware: %d\n", ret);
}

static void
init_ring_lists(struct intel_ring_buffer *ring)
{
	INIT_LIST_HEAD(&ring->active_list);
	INIT_LIST_HEAD(&ring->request_list);
	INIT_LIST_HEAD(&ring->gpu_write_list);
}

void
i915_gem_load(struct drm_device *dev)
{
	int i;
	drm_i915_private_t *dev_priv = dev->dev_private;

	INIT_LIST_HEAD(&dev_priv->mm.active_list);
	INIT_LIST_HEAD(&dev_priv->mm.flushing_list);
	INIT_LIST_HEAD(&dev_priv->mm.inactive_list);
	INIT_LIST_HEAD(&dev_priv->mm.pinned_list);
	INIT_LIST_HEAD(&dev_priv->mm.fence_list);
	INIT_LIST_HEAD(&dev_priv->mm.deferred_free_list);
	INIT_LIST_HEAD(&dev_priv->mm.gtt_list);
	for (i = 0; i < I915_NUM_RINGS; i++)
		init_ring_lists(&dev_priv->ring[i]);
	for (i = 0; i < I915_MAX_NUM_FENCES; i++)
		INIT_LIST_HEAD(&dev_priv->fence_regs[i].lru_list);
	INIT_DELAYED_WORK(&dev_priv->mm.retire_work,
			  i915_gem_retire_work_handler);
	init_completion(&dev_priv->error_completion);

	/* On GEN3 we really need to make sure the ARB C3 LP bit is set */
	if (IS_GEN3(dev)) {
		u32 tmp = I915_READ(MI_ARB_STATE);
		if (!(tmp & MI_ARB_C3_LP_WRITE_ENABLE)) {
			/* arb state is a masked write, so set bit + bit in mask */
			tmp = MI_ARB_C3_LP_WRITE_ENABLE | (MI_ARB_C3_LP_WRITE_ENABLE << MI_ARB_MASK_SHIFT);
			I915_WRITE(MI_ARB_STATE, tmp);
		}
	}

	dev_priv->relative_constants_mode = I915_EXEC_CONSTANTS_REL_GENERAL;

	/* Old X drivers will take 0-2 for front, back, depth buffers */
	if (!drm_core_check_feature(dev, DRIVER_MODESET))
		dev_priv->fence_reg_start = 3;

	if (INTEL_INFO(dev)->gen >= 4 || IS_I945G(dev) || IS_I945GM(dev) || IS_G33(dev))
		dev_priv->num_fence_regs = 16;
	else
		dev_priv->num_fence_regs = 8;

	/* Initialize fence registers to zero */
	for (i = 0; i < dev_priv->num_fence_regs; i++) {
		i915_gem_clear_fence_reg(dev, &dev_priv->fence_regs[i]);
	}

	i915_gem_detect_bit_6_swizzle(dev);
	init_waitqueue_head(&dev_priv->pending_flip_queue);

	dev_priv->mm.interruptible = true;

	dev_priv->mm.inactive_shrinker.shrink = i915_gem_inactive_shrink;
	dev_priv->mm.inactive_shrinker.seeks = DEFAULT_SEEKS;
	register_shrinker(&dev_priv->mm.inactive_shrinker);
}

/*
 * Create a physically contiguous memory object for this object
 * e.g. for cursor + overlay regs
 */
static int i915_gem_init_phys_object(struct drm_device *dev,
				     int id, int size, int align)
{
	drm_i915_private_t *dev_priv = dev->dev_private;
	struct drm_i915_gem_phys_object *phys_obj;
	int ret;

	if (dev_priv->mm.phys_objs[id - 1] || !size)
		return 0;

	phys_obj = kzalloc(sizeof(struct drm_i915_gem_phys_object), GFP_KERNEL);
	if (!phys_obj)
		return -ENOMEM;

	phys_obj->id = id;

	phys_obj->handle = drm_pci_alloc(dev, size, align);
	if (!phys_obj->handle) {
		ret = -ENOMEM;
		goto kfree_obj;
	}
#ifdef CONFIG_X86
	set_memory_wc((unsigned long)phys_obj->handle->vaddr, phys_obj->handle->size / PAGE_SIZE);
#endif

	dev_priv->mm.phys_objs[id - 1] = phys_obj;

	return 0;
kfree_obj:
	kfree(phys_obj);
	return ret;
}

static void i915_gem_free_phys_object(struct drm_device *dev, int id)
{
	drm_i915_private_t *dev_priv = dev->dev_private;
	struct drm_i915_gem_phys_object *phys_obj;

	if (!dev_priv->mm.phys_objs[id - 1])
		return;

	phys_obj = dev_priv->mm.phys_objs[id - 1];
	if (phys_obj->cur_obj) {
		i915_gem_detach_phys_object(dev, phys_obj->cur_obj);
	}

#ifdef CONFIG_X86
	set_memory_wb((unsigned long)phys_obj->handle->vaddr, phys_obj->handle->size / PAGE_SIZE);
#endif
	drm_pci_free(dev, phys_obj->handle);
	kfree(phys_obj);
	dev_priv->mm.phys_objs[id - 1] = NULL;
}

void i915_gem_free_all_phys_object(struct drm_device *dev)
{
	int i;

	for (i = I915_GEM_PHYS_CURSOR_0; i <= I915_MAX_PHYS_OBJECT; i++)
		i915_gem_free_phys_object(dev, i);
}

void i915_gem_detach_phys_object(struct drm_device *dev,
				 struct drm_i915_gem_object *obj)
{
	struct address_space *mapping = obj->base.filp->f_path.dentry->d_inode->i_mapping;
	char *vaddr;
	int i;
	int page_count;

	if (!obj->phys_obj)
		return;
	vaddr = obj->phys_obj->handle->vaddr;

	page_count = obj->base.size / PAGE_SIZE;
	for (i = 0; i < page_count; i++) {
		struct page *page = shmem_read_mapping_page(mapping, i);
		if (!IS_ERR(page)) {
			char *dst = kmap_atomic(page);
			memcpy(dst, vaddr + i*PAGE_SIZE, PAGE_SIZE);
			kunmap_atomic(dst);

			drm_clflush_pages(&page, 1);

			set_page_dirty(page);
			mark_page_accessed(page);
			page_cache_release(page);
		}
	}
	intel_gtt_chipset_flush();

	obj->phys_obj->cur_obj = NULL;
	obj->phys_obj = NULL;
}

int
i915_gem_attach_phys_object(struct drm_device *dev,
			    struct drm_i915_gem_object *obj,
			    int id,
			    int align)
{
	struct address_space *mapping = obj->base.filp->f_path.dentry->d_inode->i_mapping;
	drm_i915_private_t *dev_priv = dev->dev_private;
	int ret = 0;
	int page_count;
	int i;

	if (id > I915_MAX_PHYS_OBJECT)
		return -EINVAL;

	if (obj->phys_obj) {
		if (obj->phys_obj->id == id)
			return 0;
		i915_gem_detach_phys_object(dev, obj);
	}

	/* create a new object */
	if (!dev_priv->mm.phys_objs[id - 1]) {
		ret = i915_gem_init_phys_object(dev, id,
						obj->base.size, align);
		if (ret) {
			DRM_ERROR("failed to init phys object %d size: %zu\n",
				  id, obj->base.size);
			return ret;
		}
	}

	/* bind to the object */
	obj->phys_obj = dev_priv->mm.phys_objs[id - 1];
	obj->phys_obj->cur_obj = obj;

	page_count = obj->base.size / PAGE_SIZE;

	for (i = 0; i < page_count; i++) {
		struct page *page;
		char *dst, *src;

		page = shmem_read_mapping_page(mapping, i);
		if (IS_ERR(page))
			return PTR_ERR(page);

		src = kmap_atomic(page);
		dst = obj->phys_obj->handle->vaddr + (i * PAGE_SIZE);
		memcpy(dst, src, PAGE_SIZE);
		kunmap_atomic(src);

		mark_page_accessed(page);
		page_cache_release(page);
	}

	return 0;
}

static int
i915_gem_phys_pwrite(struct drm_device *dev,
		     struct drm_i915_gem_object *obj,
		     struct drm_i915_gem_pwrite *args,
		     struct drm_file *file_priv)
{
	void *vaddr = obj->phys_obj->handle->vaddr + args->offset;
	char __user *user_data = (char __user *) (uintptr_t) args->data_ptr;

	if (__copy_from_user_inatomic_nocache(vaddr, user_data, args->size)) {
		unsigned long unwritten;

		/* The physical object once assigned is fixed for the lifetime
		 * of the obj, so we can safely drop the lock and continue
		 * to access vaddr.
		 */
		mutex_unlock(&dev->struct_mutex);
		unwritten = copy_from_user(vaddr, user_data, args->size);
		mutex_lock(&dev->struct_mutex);
		if (unwritten)
			return -EFAULT;
	}

	intel_gtt_chipset_flush();
	return 0;
}

void i915_gem_release(struct drm_device *dev, struct drm_file *file)
{
	struct drm_i915_file_private *file_priv = file->driver_priv;

	/* Clean up our request list when the client is going away, so that
	 * later retire_requests won't dereference our soon-to-be-gone
	 * file_priv.
	 */
	spin_lock(&file_priv->mm.lock);
	while (!list_empty(&file_priv->mm.request_list)) {
		struct drm_i915_gem_request *request;

		request = list_first_entry(&file_priv->mm.request_list,
					   struct drm_i915_gem_request,
					   client_list);
		list_del(&request->client_list);
		request->file_priv = NULL;
	}
	spin_unlock(&file_priv->mm.lock);
}

static int
i915_gpu_is_active(struct drm_device *dev)
{
	drm_i915_private_t *dev_priv = dev->dev_private;
	int lists_empty;

	lists_empty = list_empty(&dev_priv->mm.flushing_list) &&
		      list_empty(&dev_priv->mm.active_list);

	return !lists_empty;
}

static int
i915_gem_inactive_shrink(struct shrinker *shrinker, struct shrink_control *sc)
{
	struct drm_i915_private *dev_priv =
		container_of(shrinker,
			     struct drm_i915_private,
			     mm.inactive_shrinker);
	struct drm_device *dev = dev_priv->dev;
	struct drm_i915_gem_object *obj, *next;
	int nr_to_scan = sc->nr_to_scan;
	int cnt;

	if (!mutex_trylock(&dev->struct_mutex))
		return 0;

	/* "fast-path" to count number of available objects */
	if (nr_to_scan == 0) {
		cnt = 0;
		list_for_each_entry(obj,
				    &dev_priv->mm.inactive_list,
				    mm_list)
			cnt++;
		mutex_unlock(&dev->struct_mutex);
		return cnt / 100 * sysctl_vfs_cache_pressure;
	}

rescan:
	/* first scan for clean buffers */
	i915_gem_retire_requests(dev);

	list_for_each_entry_safe(obj, next,
				 &dev_priv->mm.inactive_list,
				 mm_list) {
		if (i915_gem_object_is_purgeable(obj)) {
			if (i915_gem_object_unbind(obj) == 0 &&
			    --nr_to_scan == 0)
				break;
		}
	}

	/* second pass, evict/count anything still on the inactive list */
	cnt = 0;
	list_for_each_entry_safe(obj, next,
				 &dev_priv->mm.inactive_list,
				 mm_list) {
		if (nr_to_scan &&
		    i915_gem_object_unbind(obj) == 0)
			nr_to_scan--;
		else
			cnt++;
	}

	if (nr_to_scan && i915_gpu_is_active(dev)) {
		/*
		 * We are desperate for pages, so as a last resort, wait
		 * for the GPU to finish and discard whatever we can.
		 * This has a dramatic impact to reduce the number of
		 * OOM-killer events whilst running the GPU aggressively.
		 */
		if (i915_gpu_idle(dev, true) == 0)
			goto rescan;
	}
	mutex_unlock(&dev->struct_mutex);
	return cnt / 100 * sysctl_vfs_cache_pressure;
}
=======
	obj = i915_gem_object_lookup(file_priv, args->handle);
	if (!obj)
		return -ENOENT;

	err = i915_gem_object_lock_interruptible(obj, NULL);
	if (err)
		goto out;

	if (i915_gem_object_has_pages(obj) &&
	    i915_gem_object_is_tiled(obj) &&
	    i915->gem_quirks & GEM_QUIRK_PIN_SWIZZLED_PAGES) {
		if (obj->mm.madv == I915_MADV_WILLNEED) {
			GEM_BUG_ON(!i915_gem_object_has_tiling_quirk(obj));
			i915_gem_object_clear_tiling_quirk(obj);
			i915_gem_object_make_shrinkable(obj);
		}
		if (args->madv == I915_MADV_WILLNEED) {
			GEM_BUG_ON(i915_gem_object_has_tiling_quirk(obj));
			i915_gem_object_make_unshrinkable(obj);
			i915_gem_object_set_tiling_quirk(obj);
		}
	}

	if (obj->mm.madv != __I915_MADV_PURGED) {
		obj->mm.madv = args->madv;
		if (obj->ops->adjust_lru)
			obj->ops->adjust_lru(obj);
	}

	if (i915_gem_object_has_pages(obj) ||
	    i915_gem_object_has_self_managed_shrink_list(obj)) {
		unsigned long flags;

		spin_lock_irqsave(&i915->mm.obj_lock, flags);
		if (!list_empty(&obj->mm.link)) {
			struct list_head *list;

			if (obj->mm.madv != I915_MADV_WILLNEED)
				list = &i915->mm.purge_list;
			else
				list = &i915->mm.shrink_list;
			list_move_tail(&obj->mm.link, list);

		}
		spin_unlock_irqrestore(&i915->mm.obj_lock, flags);
	}

	/* if the object is no longer attached, discard its backing storage */
	if (obj->mm.madv == I915_MADV_DONTNEED &&
	    !i915_gem_object_has_pages(obj))
		i915_gem_object_truncate(obj);

	args->retained = obj->mm.madv != __I915_MADV_PURGED;

	i915_gem_object_unlock(obj);
out:
	i915_gem_object_put(obj);
	return err;
}

/*
 * A single pass should suffice to release all the freed objects (along most
 * call paths), but be a little more paranoid in that freeing the objects does
 * take a little amount of time, during which the rcu callbacks could have added
 * new objects into the freed list, and armed the work again.
 */
void i915_gem_drain_freed_objects(struct drm_i915_private *i915)
{
	while (atomic_read(&i915->mm.free_count)) {
		flush_work(&i915->mm.free_work);
		drain_workqueue(i915->bdev.wq);
		rcu_barrier();
	}
}

/*
 * Similar to objects above (see i915_gem_drain_freed-objects), in general we
 * have workers that are armed by RCU and then rearm themselves in their
 * callbacks. To be paranoid, we need to drain the workqueue a second time after
 * waiting for the RCU grace period so that we catch work queued via RCU from
 * the first pass. As neither drain_workqueue() nor flush_workqueue() report a
 * result, we make an assumption that we only don't require more than 3 passes
 * to catch all _recursive_ RCU delayed work.
 */
void i915_gem_drain_workqueue(struct drm_i915_private *i915)
{
	int i;

	for (i = 0; i < 3; i++) {
		flush_workqueue(i915->wq);
		rcu_barrier();
		i915_gem_drain_freed_objects(i915);
	}

	drain_workqueue(i915->wq);
}

int i915_gem_init(struct drm_i915_private *dev_priv)
{
	struct intel_gt *gt;
	unsigned int i;
	int ret;

	/*
	 * In the proccess of replacing cache_level with pat_index a tricky
	 * dependency is created on the definition of the enum i915_cache_level.
	 * in case this enum is changed, PTE encode would be broken.
	 * Add a WARNING here. And remove when we completely quit using this
	 * enum
	 */
	BUILD_BUG_ON(I915_CACHE_NONE != 0 ||
		     I915_CACHE_LLC != 1 ||
		     I915_CACHE_L3_LLC != 2 ||
		     I915_CACHE_WT != 3 ||
		     I915_MAX_CACHE_LEVEL != 4);

	/* We need to fallback to 4K pages if host doesn't support huge gtt. */
	if (intel_vgpu_active(dev_priv) && !intel_vgpu_has_huge_gtt(dev_priv))
		RUNTIME_INFO(dev_priv)->page_sizes = I915_GTT_PAGE_SIZE_4K;

	for_each_gt(gt, dev_priv, i) {
		intel_uc_fetch_firmwares(&gt->uc);
		intel_wopcm_init(&gt->wopcm);
		if (GRAPHICS_VER(dev_priv) >= 8)
			setup_private_pat(gt);
	}

	ret = i915_init_ggtt(dev_priv);
	if (ret) {
		GEM_BUG_ON(ret == -EIO);
		goto err_unlock;
	}

	/*
	 * Despite its name intel_clock_gating_init applies both display
	 * clock gating workarounds; GT mmio workarounds and the occasional
	 * GT power context workaround. Worse, sometimes it includes a context
	 * register workaround which we need to apply before we record the
	 * default HW state for all contexts.
	 *
	 * FIXME: break up the workarounds and apply them at the right time!
	 */
	intel_clock_gating_init(dev_priv);

	for_each_gt(gt, dev_priv, i) {
		ret = intel_gt_init(gt);
		if (ret)
			goto err_unlock;
	}

	/*
	 * Register engines early to ensure the engine list is in its final
	 * rb-tree form, lowering the amount of code that has to deal with
	 * the intermediate llist state.
	 */
	intel_engines_driver_register(dev_priv);

	return 0;

	/*
	 * Unwinding is complicated by that we want to handle -EIO to mean
	 * disable GPU submission but keep KMS alive. We want to mark the
	 * HW as irrevisibly wedged, but keep enough state around that the
	 * driver doesn't explode during runtime.
	 */
err_unlock:
	i915_gem_drain_workqueue(dev_priv);

	if (ret != -EIO) {
		for_each_gt(gt, dev_priv, i) {
			intel_gt_driver_remove(gt);
			intel_gt_driver_release(gt);
			intel_uc_cleanup_firmwares(&gt->uc);
		}
	}

	if (ret == -EIO) {
		/*
		 * Allow engines or uC initialisation to fail by marking the GPU
		 * as wedged. But we only want to do this when the GPU is angry,
		 * for all other failure, such as an allocation failure, bail.
		 */
		for_each_gt(gt, dev_priv, i) {
			if (!intel_gt_is_wedged(gt)) {
				i915_probe_error(dev_priv,
						 "Failed to initialize GPU, declaring it wedged!\n");
				intel_gt_set_wedged(gt);
			}
		}

		/* Minimal basic recovery for KMS */
		ret = i915_ggtt_enable_hw(dev_priv);
		i915_ggtt_resume(to_gt(dev_priv)->ggtt);
		intel_clock_gating_init(dev_priv);
	}

	i915_gem_drain_freed_objects(dev_priv);

	return ret;
}

void i915_gem_driver_register(struct drm_i915_private *i915)
{
	i915_gem_driver_register__shrinker(i915);
}

void i915_gem_driver_unregister(struct drm_i915_private *i915)
{
	i915_gem_driver_unregister__shrinker(i915);
}

void i915_gem_driver_remove(struct drm_i915_private *dev_priv)
{
	struct intel_gt *gt;
	unsigned int i;

	i915_gem_suspend_late(dev_priv);
	for_each_gt(gt, dev_priv, i)
		intel_gt_driver_remove(gt);
	dev_priv->uabi_engines = RB_ROOT;

	/* Flush any outstanding unpin_work. */
	i915_gem_drain_workqueue(dev_priv);
}

void i915_gem_driver_release(struct drm_i915_private *dev_priv)
{
	struct intel_gt *gt;
	unsigned int i;

	for_each_gt(gt, dev_priv, i) {
		intel_gt_driver_release(gt);
		intel_uc_cleanup_firmwares(&gt->uc);
	}

	/* Flush any outstanding work, including i915_gem_context.release_work. */
	i915_gem_drain_workqueue(dev_priv);

	drm_WARN_ON(&dev_priv->drm, !list_empty(&dev_priv->gem.contexts.list));
}

static void i915_gem_init__mm(struct drm_i915_private *i915)
{
	spin_lock_init(&i915->mm.obj_lock);

	init_llist_head(&i915->mm.free_list);

	INIT_LIST_HEAD(&i915->mm.purge_list);
	INIT_LIST_HEAD(&i915->mm.shrink_list);

	i915_gem_init__objects(i915);
}

void i915_gem_init_early(struct drm_i915_private *dev_priv)
{
	i915_gem_init__mm(dev_priv);
	i915_gem_init__contexts(dev_priv);
}

void i915_gem_cleanup_early(struct drm_i915_private *dev_priv)
{
	i915_gem_drain_workqueue(dev_priv);
	GEM_BUG_ON(!llist_empty(&dev_priv->mm.free_list));
	GEM_BUG_ON(atomic_read(&dev_priv->mm.free_count));
	drm_WARN_ON(&dev_priv->drm, dev_priv->mm.shrink_count);
}

int i915_gem_open(struct drm_i915_private *i915, struct drm_file *file)
{
	struct drm_i915_file_private *file_priv;
	struct i915_drm_client *client;
	int ret = -ENOMEM;

	drm_dbg(&i915->drm, "\n");

	file_priv = kzalloc(sizeof(*file_priv), GFP_KERNEL);
	if (!file_priv)
		goto err_alloc;

	client = i915_drm_client_alloc();
	if (!client)
		goto err_client;

	file->driver_priv = file_priv;
	file_priv->i915 = i915;
	file_priv->file = file;
	file_priv->client = client;

	file_priv->bsd_engine = -1;
	file_priv->hang_timestamp = jiffies;

	ret = i915_gem_context_open(i915, file);
	if (ret)
		goto err_context;

	return 0;

err_context:
	i915_drm_client_put(client);
err_client:
	kfree(file_priv);
err_alloc:
	return ret;
}

#if IS_ENABLED(CONFIG_DRM_I915_SELFTEST)
#include "selftests/mock_gem_device.c"
#include "selftests/i915_gem.c"
#endif
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
