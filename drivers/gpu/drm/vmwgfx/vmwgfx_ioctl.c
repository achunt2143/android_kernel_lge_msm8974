<<<<<<< HEAD
/**************************************************************************
 *
 * Copyright © 2009 VMware, Inc., Palo Alto, CA., USA
 * All Rights Reserved.
=======
// SPDX-License-Identifier: GPL-2.0 OR MIT
/**************************************************************************
 *
 * Copyright 2009-2022 VMware, Inc., Palo Alto, CA., USA
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
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

#include "vmwgfx_drv.h"
<<<<<<< HEAD
#include "vmwgfx_drm.h"
#include "vmwgfx_kms.h"

=======
#include "vmwgfx_devcaps.h"
#include "vmwgfx_kms.h"

#include <drm/vmwgfx_drm.h>
#include <linux/pci.h>

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
int vmw_getparam_ioctl(struct drm_device *dev, void *data,
		       struct drm_file *file_priv)
{
	struct vmw_private *dev_priv = vmw_priv(dev);
	struct drm_vmw_getparam_arg *param =
	    (struct drm_vmw_getparam_arg *)data;
<<<<<<< HEAD
=======
	struct vmw_fpriv *vmw_fp = vmw_fpriv(file_priv);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	switch (param->param) {
	case DRM_VMW_PARAM_NUM_STREAMS:
		param->value = vmw_overlay_num_overlays(dev_priv);
		break;
	case DRM_VMW_PARAM_NUM_FREE_STREAMS:
		param->value = vmw_overlay_num_free_overlays(dev_priv);
		break;
	case DRM_VMW_PARAM_3D:
<<<<<<< HEAD
		param->value = vmw_fifo_have_3d(dev_priv) ? 1 : 0;
=======
		param->value = vmw_supports_3d(dev_priv) ? 1 : 0;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		break;
	case DRM_VMW_PARAM_HW_CAPS:
		param->value = dev_priv->capabilities;
		break;
<<<<<<< HEAD
	case DRM_VMW_PARAM_FIFO_CAPS:
		param->value = dev_priv->fifo.capabilities;
		break;
	case DRM_VMW_PARAM_MAX_FB_SIZE:
		param->value = dev_priv->vram_size;
		break;
	case DRM_VMW_PARAM_FIFO_HW_VERSION:
	{
		__le32 __iomem *fifo_mem = dev_priv->mmio_virt;
		const struct vmw_fifo_state *fifo = &dev_priv->fifo;

		param->value =
			ioread32(fifo_mem +
				 ((fifo->capabilities &
				   SVGA_FIFO_CAP_3D_HWVERSION_REVISED) ?
				  SVGA_FIFO_3D_HWVERSION_REVISED :
				  SVGA_FIFO_3D_HWVERSION));
		break;
	}
	default:
		DRM_ERROR("Illegal vmwgfx get param request: %d\n",
			  param->param);
=======
	case DRM_VMW_PARAM_HW_CAPS2:
		param->value = dev_priv->capabilities2;
		break;
	case DRM_VMW_PARAM_FIFO_CAPS:
		param->value = vmw_fifo_caps(dev_priv);
		break;
	case DRM_VMW_PARAM_MAX_FB_SIZE:
		param->value = dev_priv->max_primary_mem;
		break;
	case DRM_VMW_PARAM_FIFO_HW_VERSION:
	{
		if ((dev_priv->capabilities & SVGA_CAP_GBOBJECTS))
			param->value = SVGA3D_HWVERSION_WS8_B1;
		else
			param->value = vmw_fifo_mem_read(
					       dev_priv,
					       ((vmw_fifo_caps(dev_priv) &
						 SVGA_FIFO_CAP_3D_HWVERSION_REVISED) ?
							SVGA_FIFO_3D_HWVERSION_REVISED :
							SVGA_FIFO_3D_HWVERSION));
		break;
	}
	case DRM_VMW_PARAM_MAX_SURF_MEMORY:
		if ((dev_priv->capabilities & SVGA_CAP_GBOBJECTS) &&
		    !vmw_fp->gb_aware)
			param->value = dev_priv->max_mob_pages * PAGE_SIZE / 2;
		else
			param->value = dev_priv->memory_size;
		break;
	case DRM_VMW_PARAM_3D_CAPS_SIZE:
		param->value = vmw_devcaps_size(dev_priv, vmw_fp->gb_aware);
		break;
	case DRM_VMW_PARAM_MAX_MOB_MEMORY:
		vmw_fp->gb_aware = true;
		param->value = dev_priv->max_mob_pages * PAGE_SIZE;
		break;
	case DRM_VMW_PARAM_MAX_MOB_SIZE:
		param->value = dev_priv->max_mob_size;
		break;
	case DRM_VMW_PARAM_SCREEN_TARGET:
		param->value =
			(dev_priv->active_display_unit == vmw_du_screen_target);
		break;
	case DRM_VMW_PARAM_DX:
		param->value = has_sm4_context(dev_priv);
		break;
	case DRM_VMW_PARAM_SM4_1:
		param->value = has_sm4_1_context(dev_priv);
		break;
	case DRM_VMW_PARAM_SM5:
		param->value = has_sm5_context(dev_priv);
		break;
	case DRM_VMW_PARAM_GL43:
		param->value = has_gl43_context(dev_priv);
		break;
	case DRM_VMW_PARAM_DEVICE_ID:
		param->value = to_pci_dev(dev_priv->drm.dev)->device;
		break;
	default:
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		return -EINVAL;
	}

	return 0;
}


int vmw_get_cap_3d_ioctl(struct drm_device *dev, void *data,
			 struct drm_file *file_priv)
{
	struct drm_vmw_get_3d_cap_arg *arg =
		(struct drm_vmw_get_3d_cap_arg *) data;
	struct vmw_private *dev_priv = vmw_priv(dev);
	uint32_t size;
<<<<<<< HEAD
	__le32 __iomem *fifo_mem;
	void __user *buffer = (void __user *)((unsigned long)(arg->buffer));
	void *bounce;
	int ret;

	if (unlikely(arg->pad64 != 0)) {
		DRM_ERROR("Illegal GET_3D_CAP argument.\n");
		return -EINVAL;
	}

	size = (SVGA_FIFO_3D_CAPS_LAST - SVGA_FIFO_3D_CAPS + 1) << 2;
=======
	void __user *buffer = (void __user *)((unsigned long)(arg->buffer));
	void *bounce = NULL;
	int ret;
	struct vmw_fpriv *vmw_fp = vmw_fpriv(file_priv);

	if (unlikely(arg->pad64 != 0 || arg->max_size == 0)) {
		VMW_DEBUG_USER("Illegal GET_3D_CAP argument.\n");
		return -EINVAL;
	}

	size = vmw_devcaps_size(dev_priv, vmw_fp->gb_aware);
	if (unlikely(size == 0)) {
		DRM_ERROR("Failed to figure out the devcaps size (no 3D).\n");
		return -ENOMEM;
	}
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	if (arg->max_size < size)
		size = arg->max_size;

<<<<<<< HEAD
	bounce = vmalloc(size);
=======
	bounce = vzalloc(size);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	if (unlikely(bounce == NULL)) {
		DRM_ERROR("Failed to allocate bounce buffer for 3D caps.\n");
		return -ENOMEM;
	}

<<<<<<< HEAD
	fifo_mem = dev_priv->mmio_virt;
	memcpy_fromio(bounce, &fifo_mem[SVGA_FIFO_3D_CAPS], size);

	ret = copy_to_user(buffer, bounce, size);
=======
	ret = vmw_devcaps_copy(dev_priv, vmw_fp->gb_aware, bounce, size);
	if (unlikely (ret != 0))
		goto out_err;

	ret = copy_to_user(buffer, bounce, size);
	if (ret)
		ret = -EFAULT;
out_err:
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	vfree(bounce);

	if (unlikely(ret != 0))
		DRM_ERROR("Failed to report 3D caps info.\n");

	return ret;
}

int vmw_present_ioctl(struct drm_device *dev, void *data,
		      struct drm_file *file_priv)
{
	struct ttm_object_file *tfile = vmw_fpriv(file_priv)->tfile;
	struct vmw_private *dev_priv = vmw_priv(dev);
	struct drm_vmw_present_arg *arg =
		(struct drm_vmw_present_arg *)data;
	struct vmw_surface *surface;
<<<<<<< HEAD
	struct vmw_master *vmaster = vmw_master(file_priv->master);
	struct drm_vmw_rect __user *clips_ptr;
	struct drm_vmw_rect *clips = NULL;
	struct drm_mode_object *obj;
	struct vmw_framebuffer *vfb;
=======
	struct drm_vmw_rect __user *clips_ptr;
	struct drm_vmw_rect *clips = NULL;
	struct drm_framebuffer *fb;
	struct vmw_framebuffer *vfb;
	struct vmw_resource *res;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	uint32_t num_clips;
	int ret;

	num_clips = arg->num_clips;
<<<<<<< HEAD
	clips_ptr = (struct drm_vmw_rect *)(unsigned long)arg->clips_ptr;
=======
	clips_ptr = (struct drm_vmw_rect __user *)(unsigned long)arg->clips_ptr;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	if (unlikely(num_clips == 0))
		return 0;

	if (clips_ptr == NULL) {
<<<<<<< HEAD
		DRM_ERROR("Variable clips_ptr must be specified.\n");
=======
		VMW_DEBUG_USER("Variable clips_ptr must be specified.\n");
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		ret = -EINVAL;
		goto out_clips;
	}

	clips = kcalloc(num_clips, sizeof(*clips), GFP_KERNEL);
	if (clips == NULL) {
		DRM_ERROR("Failed to allocate clip rect list.\n");
		ret = -ENOMEM;
		goto out_clips;
	}

	ret = copy_from_user(clips, clips_ptr, num_clips * sizeof(*clips));
	if (ret) {
		DRM_ERROR("Failed to copy clip rects from userspace.\n");
		ret = -EFAULT;
		goto out_no_copy;
	}

<<<<<<< HEAD
	ret = mutex_lock_interruptible(&dev->mode_config.mutex);
	if (unlikely(ret != 0)) {
		ret = -ERESTARTSYS;
		goto out_no_mode_mutex;
	}

	obj = drm_mode_object_find(dev, arg->fb_id, DRM_MODE_OBJECT_FB);
	if (!obj) {
		DRM_ERROR("Invalid framebuffer id.\n");
		ret = -EINVAL;
		goto out_no_fb;
	}
	vfb = vmw_framebuffer_to_vfb(obj_to_fb(obj));

	ret = ttm_read_lock(&vmaster->lock, true);
	if (unlikely(ret != 0))
		goto out_no_ttm_lock;

	ret = vmw_user_surface_lookup_handle(dev_priv, tfile, arg->sid,
					     &surface);
	if (ret)
		goto out_no_surface;

=======
	drm_modeset_lock_all(dev);

	fb = drm_framebuffer_lookup(dev, file_priv, arg->fb_id);
	if (!fb) {
		VMW_DEBUG_USER("Invalid framebuffer id.\n");
		ret = -ENOENT;
		goto out_no_fb;
	}
	vfb = vmw_framebuffer_to_vfb(fb);

	ret = vmw_user_resource_lookup_handle(dev_priv, tfile, arg->sid,
					      user_surface_converter,
					      &res);
	if (ret)
		goto out_no_surface;

	surface = vmw_res_to_srf(res);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	ret = vmw_kms_present(dev_priv, file_priv,
			      vfb, surface, arg->sid,
			      arg->dest_x, arg->dest_y,
			      clips, num_clips);

	/* vmw_user_surface_lookup takes one ref so does new_fb */
	vmw_surface_unreference(&surface);

out_no_surface:
<<<<<<< HEAD
	ttm_read_unlock(&vmaster->lock);
out_no_ttm_lock:
out_no_fb:
	mutex_unlock(&dev->mode_config.mutex);
out_no_mode_mutex:
=======
	drm_framebuffer_put(fb);
out_no_fb:
	drm_modeset_unlock_all(dev);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
out_no_copy:
	kfree(clips);
out_clips:
	return ret;
}

int vmw_present_readback_ioctl(struct drm_device *dev, void *data,
			       struct drm_file *file_priv)
{
	struct vmw_private *dev_priv = vmw_priv(dev);
	struct drm_vmw_present_readback_arg *arg =
		(struct drm_vmw_present_readback_arg *)data;
	struct drm_vmw_fence_rep __user *user_fence_rep =
		(struct drm_vmw_fence_rep __user *)
		(unsigned long)arg->fence_rep;
<<<<<<< HEAD
	struct vmw_master *vmaster = vmw_master(file_priv->master);
	struct drm_vmw_rect __user *clips_ptr;
	struct drm_vmw_rect *clips = NULL;
	struct drm_mode_object *obj;
=======
	struct drm_vmw_rect __user *clips_ptr;
	struct drm_vmw_rect *clips = NULL;
	struct drm_framebuffer *fb;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	struct vmw_framebuffer *vfb;
	uint32_t num_clips;
	int ret;

	num_clips = arg->num_clips;
<<<<<<< HEAD
	clips_ptr = (struct drm_vmw_rect *)(unsigned long)arg->clips_ptr;
=======
	clips_ptr = (struct drm_vmw_rect __user *)(unsigned long)arg->clips_ptr;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	if (unlikely(num_clips == 0))
		return 0;

	if (clips_ptr == NULL) {
<<<<<<< HEAD
		DRM_ERROR("Argument clips_ptr must be specified.\n");
=======
		VMW_DEBUG_USER("Argument clips_ptr must be specified.\n");
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		ret = -EINVAL;
		goto out_clips;
	}

	clips = kcalloc(num_clips, sizeof(*clips), GFP_KERNEL);
	if (clips == NULL) {
		DRM_ERROR("Failed to allocate clip rect list.\n");
		ret = -ENOMEM;
		goto out_clips;
	}

	ret = copy_from_user(clips, clips_ptr, num_clips * sizeof(*clips));
	if (ret) {
		DRM_ERROR("Failed to copy clip rects from userspace.\n");
		ret = -EFAULT;
		goto out_no_copy;
	}

<<<<<<< HEAD
	ret = mutex_lock_interruptible(&dev->mode_config.mutex);
	if (unlikely(ret != 0)) {
		ret = -ERESTARTSYS;
		goto out_no_mode_mutex;
	}

	obj = drm_mode_object_find(dev, arg->fb_id, DRM_MODE_OBJECT_FB);
	if (!obj) {
		DRM_ERROR("Invalid framebuffer id.\n");
		ret = -EINVAL;
		goto out_no_fb;
	}

	vfb = vmw_framebuffer_to_vfb(obj_to_fb(obj));
	if (!vfb->dmabuf) {
		DRM_ERROR("Framebuffer not dmabuf backed.\n");
		ret = -EINVAL;
		goto out_no_fb;
	}

	ret = ttm_read_lock(&vmaster->lock, true);
	if (unlikely(ret != 0))
		goto out_no_ttm_lock;
=======
	drm_modeset_lock_all(dev);

	fb = drm_framebuffer_lookup(dev, file_priv, arg->fb_id);
	if (!fb) {
		VMW_DEBUG_USER("Invalid framebuffer id.\n");
		ret = -ENOENT;
		goto out_no_fb;
	}

	vfb = vmw_framebuffer_to_vfb(fb);
	if (!vfb->bo) {
		VMW_DEBUG_USER("Framebuffer not buffer backed.\n");
		ret = -EINVAL;
		goto out_no_ttm_lock;
	}
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	ret = vmw_kms_readback(dev_priv, file_priv,
			       vfb, user_fence_rep,
			       clips, num_clips);

<<<<<<< HEAD
	ttm_read_unlock(&vmaster->lock);
out_no_ttm_lock:
out_no_fb:
	mutex_unlock(&dev->mode_config.mutex);
out_no_mode_mutex:
=======
out_no_ttm_lock:
	drm_framebuffer_put(fb);
out_no_fb:
	drm_modeset_unlock_all(dev);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
out_no_copy:
	kfree(clips);
out_clips:
	return ret;
}
<<<<<<< HEAD


/**
 * vmw_fops_poll - wrapper around the drm_poll function
 *
 * @filp: See the linux fops poll documentation.
 * @wait: See the linux fops poll documentation.
 *
 * Wrapper around the drm_poll function that makes sure the device is
 * processing the fifo if drm_poll decides to wait.
 */
unsigned int vmw_fops_poll(struct file *filp, struct poll_table_struct *wait)
{
	struct drm_file *file_priv = filp->private_data;
	struct vmw_private *dev_priv =
		vmw_priv(file_priv->minor->dev);

	vmw_fifo_ping_host(dev_priv, SVGA_SYNC_GENERIC);
	return drm_poll(filp, wait);
}


/**
 * vmw_fops_read - wrapper around the drm_read function
 *
 * @filp: See the linux fops read documentation.
 * @buffer: See the linux fops read documentation.
 * @count: See the linux fops read documentation.
 * offset: See the linux fops read documentation.
 *
 * Wrapper around the drm_read function that makes sure the device is
 * processing the fifo if drm_read decides to wait.
 */
ssize_t vmw_fops_read(struct file *filp, char __user *buffer,
		      size_t count, loff_t *offset)
{
	struct drm_file *file_priv = filp->private_data;
	struct vmw_private *dev_priv =
		vmw_priv(file_priv->minor->dev);

	vmw_fifo_ping_host(dev_priv, SVGA_SYNC_GENERIC);
	return drm_read(filp, buffer, count, offset);
}
=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
