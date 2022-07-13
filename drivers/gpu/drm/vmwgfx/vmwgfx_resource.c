<<<<<<< HEAD
/**************************************************************************
 *
 * Copyright © 2009 VMware, Inc., Palo Alto, CA., USA
 * All Rights Reserved.
=======
// SPDX-License-Identifier: GPL-2.0 OR MIT
/**************************************************************************
 *
 * Copyright 2009-2023 VMware, Inc., Palo Alto, CA., USA
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

<<<<<<< HEAD
#include "vmwgfx_drv.h"
#include "vmwgfx_drm.h"
#include "ttm/ttm_object.h"
#include "ttm/ttm_placement.h"
#include "drmP.h"

struct vmw_user_context {
	struct ttm_base_object base;
	struct vmw_resource res;
};

struct vmw_user_surface {
	struct ttm_base_object base;
	struct vmw_surface srf;
	uint32_t size;
};

struct vmw_user_dma_buffer {
	struct ttm_base_object base;
	struct vmw_dma_buffer dma;
};

struct vmw_bo_user_rep {
	uint32_t handle;
	uint64_t map_handle;
};

struct vmw_stream {
	struct vmw_resource res;
	uint32_t stream_id;
};

struct vmw_user_stream {
	struct ttm_base_object base;
	struct vmw_stream stream;
};

struct vmw_surface_offset {
	uint32_t face;
	uint32_t mip;
	uint32_t bo_offset;
};


static uint64_t vmw_user_context_size;
static uint64_t vmw_user_surface_size;
static uint64_t vmw_user_stream_size;

static inline struct vmw_dma_buffer *
vmw_dma_buffer(struct ttm_buffer_object *bo)
{
	return container_of(bo, struct vmw_dma_buffer, base);
}

static inline struct vmw_user_dma_buffer *
vmw_user_dma_buffer(struct ttm_buffer_object *bo)
{
	struct vmw_dma_buffer *vmw_bo = vmw_dma_buffer(bo);
	return container_of(vmw_bo, struct vmw_user_dma_buffer, dma);
=======
#include <drm/ttm/ttm_placement.h>

#include "vmwgfx_binding.h"
#include "vmwgfx_bo.h"
#include "vmwgfx_drv.h"
#include "vmwgfx_resource_priv.h"

#define VMW_RES_EVICT_ERR_COUNT 10

/**
 * vmw_resource_mob_attach - Mark a resource as attached to its backing mob
 * @res: The resource
 */
void vmw_resource_mob_attach(struct vmw_resource *res)
{
	struct vmw_bo *gbo = res->guest_memory_bo;
	struct rb_node **new = &gbo->res_tree.rb_node, *parent = NULL;

	dma_resv_assert_held(gbo->tbo.base.resv);
	res->used_prio = (res->res_dirty) ? res->func->dirty_prio :
		res->func->prio;

	while (*new) {
		struct vmw_resource *this =
			container_of(*new, struct vmw_resource, mob_node);

		parent = *new;
		new = (res->guest_memory_offset < this->guest_memory_offset) ?
			&((*new)->rb_left) : &((*new)->rb_right);
	}

	rb_link_node(&res->mob_node, parent, new);
	rb_insert_color(&res->mob_node, &gbo->res_tree);

	vmw_bo_prio_add(gbo, res->used_prio);
}

/**
 * vmw_resource_mob_detach - Mark a resource as detached from its backing mob
 * @res: The resource
 */
void vmw_resource_mob_detach(struct vmw_resource *res)
{
	struct vmw_bo *gbo = res->guest_memory_bo;

	dma_resv_assert_held(gbo->tbo.base.resv);
	if (vmw_resource_mob_attached(res)) {
		rb_erase(&res->mob_node, &gbo->res_tree);
		RB_CLEAR_NODE(&res->mob_node);
		vmw_bo_prio_del(gbo, res->used_prio);
	}
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

struct vmw_resource *vmw_resource_reference(struct vmw_resource *res)
{
	kref_get(&res->kref);
	return res;
}

<<<<<<< HEAD
=======
struct vmw_resource *
vmw_resource_reference_unless_doomed(struct vmw_resource *res)
{
	return kref_get_unless_zero(&res->kref) ? res : NULL;
}
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

/**
 * vmw_resource_release_id - release a resource id to the id manager.
 *
 * @res: Pointer to the resource.
 *
 * Release the resource id to the resource id manager and set it to -1
 */
<<<<<<< HEAD
static void vmw_resource_release_id(struct vmw_resource *res)
{
	struct vmw_private *dev_priv = res->dev_priv;

	write_lock(&dev_priv->resource_lock);
	if (res->id != -1)
		idr_remove(res->idr, res->id);
	res->id = -1;
	write_unlock(&dev_priv->resource_lock);
=======
void vmw_resource_release_id(struct vmw_resource *res)
{
	struct vmw_private *dev_priv = res->dev_priv;
	struct idr *idr = &dev_priv->res_idr[res->func->res_type];

	spin_lock(&dev_priv->resource_lock);
	if (res->id != -1)
		idr_remove(idr, res->id);
	res->id = -1;
	spin_unlock(&dev_priv->resource_lock);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

static void vmw_resource_release(struct kref *kref)
{
	struct vmw_resource *res =
	    container_of(kref, struct vmw_resource, kref);
	struct vmw_private *dev_priv = res->dev_priv;
<<<<<<< HEAD
	int id = res->id;
	struct idr *idr = res->idr;

	res->avail = false;
	if (res->remove_from_lists != NULL)
		res->remove_from_lists(res);
	write_unlock(&dev_priv->resource_lock);

	if (likely(res->hw_destroy != NULL))
		res->hw_destroy(res);

=======
	int id;
	int ret;
	struct idr *idr = &dev_priv->res_idr[res->func->res_type];

	spin_lock(&dev_priv->resource_lock);
	list_del_init(&res->lru_head);
	spin_unlock(&dev_priv->resource_lock);
	if (res->guest_memory_bo) {
		struct ttm_buffer_object *bo = &res->guest_memory_bo->tbo;

		ret = ttm_bo_reserve(bo, false, false, NULL);
		BUG_ON(ret);
		if (vmw_resource_mob_attached(res) &&
		    res->func->unbind != NULL) {
			struct ttm_validate_buffer val_buf;

			val_buf.bo = bo;
			val_buf.num_shared = 0;
			res->func->unbind(res, false, &val_buf);
		}
		res->guest_memory_size = false;
		vmw_resource_mob_detach(res);
		if (res->dirty)
			res->func->dirty_free(res);
		if (res->coherent)
			vmw_bo_dirty_release(res->guest_memory_bo);
		ttm_bo_unreserve(bo);
		vmw_user_bo_unref(&res->guest_memory_bo);
	}

	if (likely(res->hw_destroy != NULL)) {
		mutex_lock(&dev_priv->binding_mutex);
		vmw_binding_res_list_kill(&res->binding_head);
		mutex_unlock(&dev_priv->binding_mutex);
		res->hw_destroy(res);
	}

	id = res->id;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	if (res->res_free != NULL)
		res->res_free(res);
	else
		kfree(res);

<<<<<<< HEAD
	write_lock(&dev_priv->resource_lock);

	if (id != -1)
		idr_remove(idr, id);
=======
	spin_lock(&dev_priv->resource_lock);
	if (id != -1)
		idr_remove(idr, id);
	spin_unlock(&dev_priv->resource_lock);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

void vmw_resource_unreference(struct vmw_resource **p_res)
{
	struct vmw_resource *res = *p_res;
<<<<<<< HEAD
	struct vmw_private *dev_priv = res->dev_priv;

	*p_res = NULL;
	write_lock(&dev_priv->resource_lock);
	kref_put(&res->kref, vmw_resource_release);
	write_unlock(&dev_priv->resource_lock);
=======

	*p_res = NULL;
	kref_put(&res->kref, vmw_resource_release);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}


/**
 * vmw_resource_alloc_id - release a resource id to the id manager.
 *
<<<<<<< HEAD
 * @dev_priv: Pointer to the device private structure.
=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 * @res: Pointer to the resource.
 *
 * Allocate the lowest free resource from the resource manager, and set
 * @res->id to that id. Returns 0 on success and -ENOMEM on failure.
 */
<<<<<<< HEAD
static int vmw_resource_alloc_id(struct vmw_private *dev_priv,
				 struct vmw_resource *res)
{
	int ret;

	BUG_ON(res->id != -1);

	do {
		if (unlikely(idr_pre_get(res->idr, GFP_KERNEL) == 0))
			return -ENOMEM;

		write_lock(&dev_priv->resource_lock);
		ret = idr_get_new_above(res->idr, res, 1, &res->id);
		write_unlock(&dev_priv->resource_lock);

	} while (ret == -EAGAIN);

	return ret;
}


static int vmw_resource_init(struct vmw_private *dev_priv,
			     struct vmw_resource *res,
			     struct idr *idr,
			     enum ttm_object_type obj_type,
			     bool delay_id,
			     void (*res_free) (struct vmw_resource *res),
			     void (*remove_from_lists)
			     (struct vmw_resource *res))
=======
int vmw_resource_alloc_id(struct vmw_resource *res)
{
	struct vmw_private *dev_priv = res->dev_priv;
	int ret;
	struct idr *idr = &dev_priv->res_idr[res->func->res_type];

	BUG_ON(res->id != -1);

	idr_preload(GFP_KERNEL);
	spin_lock(&dev_priv->resource_lock);

	ret = idr_alloc(idr, res, 1, 0, GFP_NOWAIT);
	if (ret >= 0)
		res->id = ret;

	spin_unlock(&dev_priv->resource_lock);
	idr_preload_end();
	return ret < 0 ? ret : 0;
}

/**
 * vmw_resource_init - initialize a struct vmw_resource
 *
 * @dev_priv:       Pointer to a device private struct.
 * @res:            The struct vmw_resource to initialize.
 * @delay_id:       Boolean whether to defer device id allocation until
 *                  the first validation.
 * @res_free:       Resource destructor.
 * @func:           Resource function table.
 */
int vmw_resource_init(struct vmw_private *dev_priv, struct vmw_resource *res,
		      bool delay_id,
		      void (*res_free) (struct vmw_resource *res),
		      const struct vmw_res_func *func)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
{
	kref_init(&res->kref);
	res->hw_destroy = NULL;
	res->res_free = res_free;
<<<<<<< HEAD
	res->remove_from_lists = remove_from_lists;
	res->res_type = obj_type;
	res->idr = idr;
	res->avail = false;
	res->dev_priv = dev_priv;
	INIT_LIST_HEAD(&res->query_head);
	INIT_LIST_HEAD(&res->validate_head);
	res->id = -1;
	if (delay_id)
		return 0;
	else
		return vmw_resource_alloc_id(dev_priv, res);
}

/**
 * vmw_resource_activate
 *
 * @res:        Pointer to the newly created resource
 * @hw_destroy: Destroy function. NULL if none.
 *
 * Activate a resource after the hardware has been made aware of it.
 * Set tye destroy function to @destroy. Typically this frees the
 * resource and destroys the hardware resources associated with it.
 * Activate basically means that the function vmw_resource_lookup will
 * find it.
 */

static void vmw_resource_activate(struct vmw_resource *res,
				  void (*hw_destroy) (struct vmw_resource *))
{
	struct vmw_private *dev_priv = res->dev_priv;

	write_lock(&dev_priv->resource_lock);
	res->avail = true;
	res->hw_destroy = hw_destroy;
	write_unlock(&dev_priv->resource_lock);
}

struct vmw_resource *vmw_resource_lookup(struct vmw_private *dev_priv,
					 struct idr *idr, int id)
{
	struct vmw_resource *res;

	read_lock(&dev_priv->resource_lock);
	res = idr_find(idr, id);
	if (res && res->avail)
		kref_get(&res->kref);
	else
		res = NULL;
	read_unlock(&dev_priv->resource_lock);

	if (unlikely(res == NULL))
		return NULL;

	return res;
}

/**
 * Context management:
 */

static void vmw_hw_context_destroy(struct vmw_resource *res)
{

	struct vmw_private *dev_priv = res->dev_priv;
	struct {
		SVGA3dCmdHeader header;
		SVGA3dCmdDestroyContext body;
	} *cmd;


	vmw_execbuf_release_pinned_bo(dev_priv, true, res->id);

	cmd = vmw_fifo_reserve(dev_priv, sizeof(*cmd));
	if (unlikely(cmd == NULL)) {
		DRM_ERROR("Failed reserving FIFO space for surface "
			  "destruction.\n");
		return;
	}

	cmd->header.id = cpu_to_le32(SVGA_3D_CMD_CONTEXT_DESTROY);
	cmd->header.size = cpu_to_le32(sizeof(cmd->body));
	cmd->body.cid = cpu_to_le32(res->id);

	vmw_fifo_commit(dev_priv, sizeof(*cmd));
	vmw_3d_resource_dec(dev_priv, false);
}

static int vmw_context_init(struct vmw_private *dev_priv,
			    struct vmw_resource *res,
			    void (*res_free) (struct vmw_resource *res))
{
	int ret;

	struct {
		SVGA3dCmdHeader header;
		SVGA3dCmdDefineContext body;
	} *cmd;

	ret = vmw_resource_init(dev_priv, res, &dev_priv->context_idr,
				VMW_RES_CONTEXT, false, res_free, NULL);

	if (unlikely(ret != 0)) {
		DRM_ERROR("Failed to allocate a resource id.\n");
		goto out_early;
	}

	if (unlikely(res->id >= SVGA3D_MAX_CONTEXT_IDS)) {
		DRM_ERROR("Out of hw context ids.\n");
		vmw_resource_unreference(&res);
		return -ENOMEM;
	}

	cmd = vmw_fifo_reserve(dev_priv, sizeof(*cmd));
	if (unlikely(cmd == NULL)) {
		DRM_ERROR("Fifo reserve failed.\n");
		vmw_resource_unreference(&res);
		return -ENOMEM;
	}

	cmd->header.id = cpu_to_le32(SVGA_3D_CMD_CONTEXT_DEFINE);
	cmd->header.size = cpu_to_le32(sizeof(cmd->body));
	cmd->body.cid = cpu_to_le32(res->id);

	vmw_fifo_commit(dev_priv, sizeof(*cmd));
	(void) vmw_3d_resource_inc(dev_priv, false);
	vmw_resource_activate(res, vmw_hw_context_destroy);
	return 0;

out_early:
	if (res_free == NULL)
		kfree(res);
	else
		res_free(res);
	return ret;
}

struct vmw_resource *vmw_context_alloc(struct vmw_private *dev_priv)
{
	struct vmw_resource *res = kmalloc(sizeof(*res), GFP_KERNEL);
	int ret;

	if (unlikely(res == NULL))
		return NULL;

	ret = vmw_context_init(dev_priv, res, NULL);
	return (ret == 0) ? res : NULL;
}

/**
 * User-space context management:
 */

static void vmw_user_context_free(struct vmw_resource *res)
{
	struct vmw_user_context *ctx =
	    container_of(res, struct vmw_user_context, res);
	struct vmw_private *dev_priv = res->dev_priv;

	kfree(ctx);
	ttm_mem_global_free(vmw_mem_glob(dev_priv),
			    vmw_user_context_size);
}

/**
 * This function is called when user space has no more references on the
 * base object. It releases the base-object's reference on the resource object.
 */

static void vmw_user_context_base_release(struct ttm_base_object **p_base)
{
	struct ttm_base_object *base = *p_base;
	struct vmw_user_context *ctx =
	    container_of(base, struct vmw_user_context, base);
	struct vmw_resource *res = &ctx->res;

	*p_base = NULL;
	vmw_resource_unreference(&res);
}

int vmw_context_destroy_ioctl(struct drm_device *dev, void *data,
			      struct drm_file *file_priv)
{
	struct vmw_private *dev_priv = vmw_priv(dev);
	struct vmw_resource *res;
	struct vmw_user_context *ctx;
	struct drm_vmw_context_arg *arg = (struct drm_vmw_context_arg *)data;
	struct ttm_object_file *tfile = vmw_fpriv(file_priv)->tfile;
	int ret = 0;

	res = vmw_resource_lookup(dev_priv, &dev_priv->context_idr, arg->cid);
	if (unlikely(res == NULL))
		return -EINVAL;

	if (res->res_free != &vmw_user_context_free) {
		ret = -EINVAL;
		goto out;
	}

	ctx = container_of(res, struct vmw_user_context, res);
	if (ctx->base.tfile != tfile && !ctx->base.shareable) {
		ret = -EPERM;
		goto out;
	}

	ttm_ref_object_base_unref(tfile, ctx->base.hash.key, TTM_REF_USAGE);
out:
	vmw_resource_unreference(&res);
	return ret;
}

int vmw_context_define_ioctl(struct drm_device *dev, void *data,
			     struct drm_file *file_priv)
{
	struct vmw_private *dev_priv = vmw_priv(dev);
	struct vmw_user_context *ctx;
	struct vmw_resource *res;
	struct vmw_resource *tmp;
	struct drm_vmw_context_arg *arg = (struct drm_vmw_context_arg *)data;
	struct ttm_object_file *tfile = vmw_fpriv(file_priv)->tfile;
	struct vmw_master *vmaster = vmw_master(file_priv->master);
	int ret;


	/*
	 * Approximate idr memory usage with 128 bytes. It will be limited
	 * by maximum number_of contexts anyway.
	 */

	if (unlikely(vmw_user_context_size == 0))
		vmw_user_context_size = ttm_round_pot(sizeof(*ctx)) + 128;

	ret = ttm_read_lock(&vmaster->lock, true);
	if (unlikely(ret != 0))
		return ret;

	ret = ttm_mem_global_alloc(vmw_mem_glob(dev_priv),
				   vmw_user_context_size,
				   false, true);
	if (unlikely(ret != 0)) {
		if (ret != -ERESTARTSYS)
			DRM_ERROR("Out of graphics memory for context"
				  " creation.\n");
		goto out_unlock;
	}

	ctx = kmalloc(sizeof(*ctx), GFP_KERNEL);
	if (unlikely(ctx == NULL)) {
		ttm_mem_global_free(vmw_mem_glob(dev_priv),
				    vmw_user_context_size);
		ret = -ENOMEM;
		goto out_unlock;
	}

	res = &ctx->res;
	ctx->base.shareable = false;
	ctx->base.tfile = NULL;

	/*
	 * From here on, the destructor takes over resource freeing.
	 */

	ret = vmw_context_init(dev_priv, res, vmw_user_context_free);
	if (unlikely(ret != 0))
		goto out_unlock;

	tmp = vmw_resource_reference(&ctx->res);
	ret = ttm_base_object_init(tfile, &ctx->base, false, VMW_RES_CONTEXT,
				   &vmw_user_context_base_release, NULL);

	if (unlikely(ret != 0)) {
		vmw_resource_unreference(&tmp);
		goto out_err;
	}

	arg->cid = res->id;
out_err:
	vmw_resource_unreference(&res);
out_unlock:
	ttm_read_unlock(&vmaster->lock);
	return ret;

}

int vmw_context_check(struct vmw_private *dev_priv,
		      struct ttm_object_file *tfile,
		      int id,
		      struct vmw_resource **p_res)
{
	struct vmw_resource *res;
	int ret = 0;

	read_lock(&dev_priv->resource_lock);
	res = idr_find(&dev_priv->context_idr, id);
	if (res && res->avail) {
		struct vmw_user_context *ctx =
			container_of(res, struct vmw_user_context, res);
		if (ctx->base.tfile != tfile && !ctx->base.shareable)
			ret = -EPERM;
		if (p_res)
			*p_res = vmw_resource_reference(res);
	} else
		ret = -EINVAL;
	read_unlock(&dev_priv->resource_lock);
=======
	res->dev_priv = dev_priv;
	res->func = func;
	RB_CLEAR_NODE(&res->mob_node);
	INIT_LIST_HEAD(&res->lru_head);
	INIT_LIST_HEAD(&res->binding_head);
	res->id = -1;
	res->guest_memory_bo = NULL;
	res->guest_memory_offset = 0;
	res->guest_memory_dirty = false;
	res->res_dirty = false;
	res->coherent = false;
	res->used_prio = 3;
	res->dirty = NULL;
	if (delay_id)
		return 0;
	else
		return vmw_resource_alloc_id(res);
}


/**
 * vmw_user_resource_lookup_handle - lookup a struct resource from a
 * TTM user-space handle and perform basic type checks
 *
 * @dev_priv:     Pointer to a device private struct
 * @tfile:        Pointer to a struct ttm_object_file identifying the caller
 * @handle:       The TTM user-space handle
 * @converter:    Pointer to an object describing the resource type
 * @p_res:        On successful return the location pointed to will contain
 *                a pointer to a refcounted struct vmw_resource.
 *
 * If the handle can't be found or is associated with an incorrect resource
 * type, -EINVAL will be returned.
 */
int vmw_user_resource_lookup_handle(struct vmw_private *dev_priv,
				    struct ttm_object_file *tfile,
				    uint32_t handle,
				    const struct vmw_user_resource_conv
				    *converter,
				    struct vmw_resource **p_res)
{
	struct ttm_base_object *base;
	struct vmw_resource *res;
	int ret = -EINVAL;

	base = ttm_base_object_lookup(tfile, handle);
	if (unlikely(!base))
		return -EINVAL;

	if (unlikely(ttm_base_object_type(base) != converter->object_type))
		goto out_bad_resource;

	res = converter->base_obj_to_res(base);
	kref_get(&res->kref);

	*p_res = res;
	ret = 0;

out_bad_resource:
	ttm_base_object_unref(&base);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	return ret;
}

<<<<<<< HEAD
struct vmw_bpp {
	uint8_t bpp;
	uint8_t s_bpp;
};

/*
 * Size table for the supported SVGA3D surface formats. It consists of
 * two values. The bpp value and the s_bpp value which is short for
 * "stride bits per pixel" The values are given in such a way that the
 * minimum stride for the image is calculated using
 *
 * min_stride = w*s_bpp
 *
 * and the total memory requirement for the image is
 *
 * h*min_stride*bpp/s_bpp
 *
 */
static const struct vmw_bpp vmw_sf_bpp[] = {
	[SVGA3D_FORMAT_INVALID] = {0, 0},
	[SVGA3D_X8R8G8B8] = {32, 32},
	[SVGA3D_A8R8G8B8] = {32, 32},
	[SVGA3D_R5G6B5] = {16, 16},
	[SVGA3D_X1R5G5B5] = {16, 16},
	[SVGA3D_A1R5G5B5] = {16, 16},
	[SVGA3D_A4R4G4B4] = {16, 16},
	[SVGA3D_Z_D32] = {32, 32},
	[SVGA3D_Z_D16] = {16, 16},
	[SVGA3D_Z_D24S8] = {32, 32},
	[SVGA3D_Z_D15S1] = {16, 16},
	[SVGA3D_LUMINANCE8] = {8, 8},
	[SVGA3D_LUMINANCE4_ALPHA4] = {8, 8},
	[SVGA3D_LUMINANCE16] = {16, 16},
	[SVGA3D_LUMINANCE8_ALPHA8] = {16, 16},
	[SVGA3D_DXT1] = {4, 16},
	[SVGA3D_DXT2] = {8, 32},
	[SVGA3D_DXT3] = {8, 32},
	[SVGA3D_DXT4] = {8, 32},
	[SVGA3D_DXT5] = {8, 32},
	[SVGA3D_BUMPU8V8] = {16, 16},
	[SVGA3D_BUMPL6V5U5] = {16, 16},
	[SVGA3D_BUMPX8L8V8U8] = {32, 32},
	[SVGA3D_ARGB_S10E5] = {16, 16},
	[SVGA3D_ARGB_S23E8] = {32, 32},
	[SVGA3D_A2R10G10B10] = {32, 32},
	[SVGA3D_V8U8] = {16, 16},
	[SVGA3D_Q8W8V8U8] = {32, 32},
	[SVGA3D_CxV8U8] = {16, 16},
	[SVGA3D_X8L8V8U8] = {32, 32},
	[SVGA3D_A2W10V10U10] = {32, 32},
	[SVGA3D_ALPHA8] = {8, 8},
	[SVGA3D_R_S10E5] = {16, 16},
	[SVGA3D_R_S23E8] = {32, 32},
	[SVGA3D_RG_S10E5] = {16, 16},
	[SVGA3D_RG_S23E8] = {32, 32},
	[SVGA3D_BUFFER] = {8, 8},
	[SVGA3D_Z_D24X8] = {32, 32},
	[SVGA3D_V16U16] = {32, 32},
	[SVGA3D_G16R16] = {32, 32},
	[SVGA3D_A16B16G16R16] = {64,  64},
	[SVGA3D_UYVY] = {12, 12},
	[SVGA3D_YUY2] = {12, 12},
	[SVGA3D_NV12] = {12, 8},
	[SVGA3D_AYUV] = {32, 32},
	[SVGA3D_BC4_UNORM] = {4,  16},
	[SVGA3D_BC5_UNORM] = {8,  32},
	[SVGA3D_Z_DF16] = {16,  16},
	[SVGA3D_Z_DF24] = {24,  24},
	[SVGA3D_Z_D24S8_INT] = {32,  32}
};


/**
 * Surface management.
 */

struct vmw_surface_dma {
	SVGA3dCmdHeader header;
	SVGA3dCmdSurfaceDMA body;
	SVGA3dCopyBox cb;
	SVGA3dCmdSurfaceDMASuffix suffix;
};

struct vmw_surface_define {
	SVGA3dCmdHeader header;
	SVGA3dCmdDefineSurface body;
};

struct vmw_surface_destroy {
	SVGA3dCmdHeader header;
	SVGA3dCmdDestroySurface body;
};


/**
 * vmw_surface_dma_size - Compute fifo size for a dma command.
 *
 * @srf: Pointer to a struct vmw_surface
 *
 * Computes the required size for a surface dma command for backup or
 * restoration of the surface represented by @srf.
 */
static inline uint32_t vmw_surface_dma_size(const struct vmw_surface *srf)
{
	return srf->num_sizes * sizeof(struct vmw_surface_dma);
}


/**
 * vmw_surface_define_size - Compute fifo size for a surface define command.
 *
 * @srf: Pointer to a struct vmw_surface
 *
 * Computes the required size for a surface define command for the definition
 * of the surface represented by @srf.
 */
static inline uint32_t vmw_surface_define_size(const struct vmw_surface *srf)
{
	return sizeof(struct vmw_surface_define) + srf->num_sizes *
		sizeof(SVGA3dSize);
}


/**
 * vmw_surface_destroy_size - Compute fifo size for a surface destroy command.
 *
 * Computes the required size for a surface destroy command for the destruction
 * of a hw surface.
 */
static inline uint32_t vmw_surface_destroy_size(void)
{
	return sizeof(struct vmw_surface_destroy);
}

/**
 * vmw_surface_destroy_encode - Encode a surface_destroy command.
 *
 * @id: The surface id
 * @cmd_space: Pointer to memory area in which the commands should be encoded.
 */
static void vmw_surface_destroy_encode(uint32_t id,
				       void *cmd_space)
{
	struct vmw_surface_destroy *cmd = (struct vmw_surface_destroy *)
		cmd_space;

	cmd->header.id = SVGA_3D_CMD_SURFACE_DESTROY;
	cmd->header.size = sizeof(cmd->body);
	cmd->body.sid = id;
}

/**
 * vmw_surface_define_encode - Encode a surface_define command.
 *
 * @srf: Pointer to a struct vmw_surface object.
 * @cmd_space: Pointer to memory area in which the commands should be encoded.
 */
static void vmw_surface_define_encode(const struct vmw_surface *srf,
				      void *cmd_space)
{
	struct vmw_surface_define *cmd = (struct vmw_surface_define *)
		cmd_space;
	struct drm_vmw_size *src_size;
	SVGA3dSize *cmd_size;
	uint32_t cmd_len;
	int i;

	cmd_len = sizeof(cmd->body) + srf->num_sizes * sizeof(SVGA3dSize);

	cmd->header.id = SVGA_3D_CMD_SURFACE_DEFINE;
	cmd->header.size = cmd_len;
	cmd->body.sid = srf->res.id;
	cmd->body.surfaceFlags = srf->flags;
	cmd->body.format = cpu_to_le32(srf->format);
	for (i = 0; i < DRM_VMW_MAX_SURFACE_FACES; ++i)
		cmd->body.face[i].numMipLevels = srf->mip_levels[i];

	cmd += 1;
	cmd_size = (SVGA3dSize *) cmd;
	src_size = srf->sizes;

	for (i = 0; i < srf->num_sizes; ++i, cmd_size++, src_size++) {
		cmd_size->width = src_size->width;
		cmd_size->height = src_size->height;
		cmd_size->depth = src_size->depth;
	}
}


/**
 * vmw_surface_dma_encode - Encode a surface_dma command.
 *
 * @srf: Pointer to a struct vmw_surface object.
 * @cmd_space: Pointer to memory area in which the commands should be encoded.
 * @ptr: Pointer to an SVGAGuestPtr indicating where the surface contents
 * should be placed or read from.
 * @to_surface: Boolean whether to DMA to the surface or from the surface.
 */
static void vmw_surface_dma_encode(struct vmw_surface *srf,
				   void *cmd_space,
				   const SVGAGuestPtr *ptr,
				   bool to_surface)
{
	uint32_t i;
	uint32_t bpp = vmw_sf_bpp[srf->format].bpp;
	uint32_t stride_bpp = vmw_sf_bpp[srf->format].s_bpp;
	struct vmw_surface_dma *cmd = (struct vmw_surface_dma *)cmd_space;

	for (i = 0; i < srf->num_sizes; ++i) {
		SVGA3dCmdHeader *header = &cmd->header;
		SVGA3dCmdSurfaceDMA *body = &cmd->body;
		SVGA3dCopyBox *cb = &cmd->cb;
		SVGA3dCmdSurfaceDMASuffix *suffix = &cmd->suffix;
		const struct vmw_surface_offset *cur_offset = &srf->offsets[i];
		const struct drm_vmw_size *cur_size = &srf->sizes[i];

		header->id = SVGA_3D_CMD_SURFACE_DMA;
		header->size = sizeof(*body) + sizeof(*cb) + sizeof(*suffix);

		body->guest.ptr = *ptr;
		body->guest.ptr.offset += cur_offset->bo_offset;
		body->guest.pitch = (cur_size->width * stride_bpp + 7) >> 3;
		body->host.sid = srf->res.id;
		body->host.face = cur_offset->face;
		body->host.mipmap = cur_offset->mip;
		body->transfer = ((to_surface) ?  SVGA3D_WRITE_HOST_VRAM :
				  SVGA3D_READ_HOST_VRAM);
		cb->x = 0;
		cb->y = 0;
		cb->z = 0;
		cb->srcx = 0;
		cb->srcy = 0;
		cb->srcz = 0;
		cb->w = cur_size->width;
		cb->h = cur_size->height;
		cb->d = cur_size->depth;

		suffix->suffixSize = sizeof(*suffix);
		suffix->maximumOffset = body->guest.pitch*cur_size->height*
			cur_size->depth*bpp / stride_bpp;
		suffix->flags.discard = 0;
		suffix->flags.unsynchronized = 0;
		suffix->flags.reserved = 0;
		++cmd;
	}
};


static void vmw_hw_surface_destroy(struct vmw_resource *res)
{

	struct vmw_private *dev_priv = res->dev_priv;
	struct vmw_surface *srf;
	void *cmd;

	if (res->id != -1) {

		cmd = vmw_fifo_reserve(dev_priv, vmw_surface_destroy_size());
		if (unlikely(cmd == NULL)) {
			DRM_ERROR("Failed reserving FIFO space for surface "
				  "destruction.\n");
			return;
		}

		vmw_surface_destroy_encode(res->id, cmd);
		vmw_fifo_commit(dev_priv, vmw_surface_destroy_size());

		/*
		 * used_memory_size_atomic, or separate lock
		 * to avoid taking dev_priv::cmdbuf_mutex in
		 * the destroy path.
		 */

		mutex_lock(&dev_priv->cmdbuf_mutex);
		srf = container_of(res, struct vmw_surface, res);
		dev_priv->used_memory_size -= srf->backup_size;
		mutex_unlock(&dev_priv->cmdbuf_mutex);

	}
	vmw_3d_resource_dec(dev_priv, false);
}

void vmw_surface_res_free(struct vmw_resource *res)
{
	struct vmw_surface *srf = container_of(res, struct vmw_surface, res);

	if (srf->backup)
		ttm_bo_unref(&srf->backup);
	kfree(srf->offsets);
	kfree(srf->sizes);
	kfree(srf->snooper.image);
	kfree(srf);
}


/**
 * vmw_surface_do_validate - make a surface available to the device.
 *
 * @dev_priv: Pointer to a device private struct.
 * @srf: Pointer to a struct vmw_surface.
 *
 * If the surface doesn't have a hw id, allocate one, and optionally
 * DMA the backed up surface contents to the device.
 *
 * Returns -EBUSY if there wasn't sufficient device resources to
 * complete the validation. Retry after freeing up resources.
 *
 * May return other errors if the kernel is out of guest resources.
 */
int vmw_surface_do_validate(struct vmw_private *dev_priv,
			    struct vmw_surface *srf)
{
	struct vmw_resource *res = &srf->res;
	struct list_head val_list;
	struct ttm_validate_buffer val_buf;
	uint32_t submit_size;
	uint8_t *cmd;
	int ret;

	if (likely(res->id != -1))
		return 0;

	if (unlikely(dev_priv->used_memory_size + srf->backup_size >=
		     dev_priv->memory_size))
		return -EBUSY;

	/*
	 * Reserve- and validate the backup DMA bo.
	 */

	if (srf->backup) {
		INIT_LIST_HEAD(&val_list);
		val_buf.bo = ttm_bo_reference(srf->backup);
		val_buf.new_sync_obj_arg = (void *)((unsigned long)
						    DRM_VMW_FENCE_FLAG_EXEC);
		list_add_tail(&val_buf.head, &val_list);
		ret = ttm_eu_reserve_buffers(&val_list);
		if (unlikely(ret != 0))
			goto out_no_reserve;

		ret = ttm_bo_validate(srf->backup, &vmw_srf_placement,
				      true, false, false);
		if (unlikely(ret != 0))
			goto out_no_validate;
	}

	/*
	 * Alloc id for the resource.
	 */

	ret = vmw_resource_alloc_id(dev_priv, res);
	if (unlikely(ret != 0)) {
		DRM_ERROR("Failed to allocate a surface id.\n");
		goto out_no_id;
	}
	if (unlikely(res->id >= SVGA3D_MAX_SURFACE_IDS)) {
		ret = -EBUSY;
		goto out_no_fifo;
	}


	/*
	 * Encode surface define- and dma commands.
	 */

	submit_size = vmw_surface_define_size(srf);
	if (srf->backup)
		submit_size += vmw_surface_dma_size(srf);

	cmd = vmw_fifo_reserve(dev_priv, submit_size);
	if (unlikely(cmd == NULL)) {
		DRM_ERROR("Failed reserving FIFO space for surface "
			  "validation.\n");
		ret = -ENOMEM;
		goto out_no_fifo;
	}

	vmw_surface_define_encode(srf, cmd);
	if (srf->backup) {
		SVGAGuestPtr ptr;

		cmd += vmw_surface_define_size(srf);
		vmw_bo_get_guest_ptr(srf->backup, &ptr);
		vmw_surface_dma_encode(srf, cmd, &ptr, true);
	}

	vmw_fifo_commit(dev_priv, submit_size);

	/*
	 * Create a fence object and fence the backup buffer.
	 */

	if (srf->backup) {
		struct vmw_fence_obj *fence;

		(void) vmw_execbuf_fence_commands(NULL, dev_priv,
						  &fence, NULL);
		ttm_eu_fence_buffer_objects(&val_list, fence);
		if (likely(fence != NULL))
			vmw_fence_obj_unreference(&fence);
		ttm_bo_unref(&val_buf.bo);
		ttm_bo_unref(&srf->backup);
	}

	/*
	 * Surface memory usage accounting.
	 */

	dev_priv->used_memory_size += srf->backup_size;

	return 0;

out_no_fifo:
	vmw_resource_release_id(res);
out_no_id:
out_no_validate:
	if (srf->backup)
		ttm_eu_backoff_reservation(&val_list);
out_no_reserve:
	if (srf->backup)
		ttm_bo_unref(&val_buf.bo);
	return ret;
}

/**
 * vmw_surface_evict - Evict a hw surface.
 *
 * @dev_priv: Pointer to a device private struct.
 * @srf: Pointer to a struct vmw_surface
 *
 * DMA the contents of a hw surface to a backup guest buffer object,
 * and destroy the hw surface, releasing its id.
 */
int vmw_surface_evict(struct vmw_private *dev_priv,
		      struct vmw_surface *srf)
{
	struct vmw_resource *res = &srf->res;
	struct list_head val_list;
	struct ttm_validate_buffer val_buf;
	uint32_t submit_size;
	uint8_t *cmd;
	int ret;
	struct vmw_fence_obj *fence;
	SVGAGuestPtr ptr;

	BUG_ON(res->id == -1);

	/*
	 * Create a surface backup buffer object.
	 */

	if (!srf->backup) {
		ret = ttm_bo_create(&dev_priv->bdev, srf->backup_size,
				    ttm_bo_type_device,
				    &vmw_srf_placement, 0, 0, true,
				    NULL, &srf->backup);
		if (unlikely(ret != 0))
			return ret;
	}

	/*
	 * Reserve- and validate the backup DMA bo.
	 */

	INIT_LIST_HEAD(&val_list);
	val_buf.bo = ttm_bo_reference(srf->backup);
	val_buf.new_sync_obj_arg = (void *)(unsigned long)
		DRM_VMW_FENCE_FLAG_EXEC;
	list_add_tail(&val_buf.head, &val_list);
	ret = ttm_eu_reserve_buffers(&val_list);
	if (unlikely(ret != 0))
		goto out_no_reserve;

	ret = ttm_bo_validate(srf->backup, &vmw_srf_placement,
			      true, false, false);
	if (unlikely(ret != 0))
		goto out_no_validate;


	/*
	 * Encode the dma- and surface destroy commands.
	 */

	submit_size = vmw_surface_dma_size(srf) + vmw_surface_destroy_size();
	cmd = vmw_fifo_reserve(dev_priv, submit_size);
	if (unlikely(cmd == NULL)) {
		DRM_ERROR("Failed reserving FIFO space for surface "
			  "eviction.\n");
		ret = -ENOMEM;
		goto out_no_fifo;
	}

	vmw_bo_get_guest_ptr(srf->backup, &ptr);
	vmw_surface_dma_encode(srf, cmd, &ptr, false);
	cmd += vmw_surface_dma_size(srf);
	vmw_surface_destroy_encode(res->id, cmd);
	vmw_fifo_commit(dev_priv, submit_size);

	/*
	 * Surface memory usage accounting.
	 */

	dev_priv->used_memory_size -= srf->backup_size;

	/*
	 * Create a fence object and fence the DMA buffer.
	 */

	(void) vmw_execbuf_fence_commands(NULL, dev_priv,
					  &fence, NULL);
	ttm_eu_fence_buffer_objects(&val_list, fence);
	if (likely(fence != NULL))
		vmw_fence_obj_unreference(&fence);
	ttm_bo_unref(&val_buf.bo);

	/*
	 * Release the surface ID.
	 */

	vmw_resource_release_id(res);

	return 0;

out_no_fifo:
out_no_validate:
	if (srf->backup)
		ttm_eu_backoff_reservation(&val_list);
out_no_reserve:
	ttm_bo_unref(&val_buf.bo);
	ttm_bo_unref(&srf->backup);
	return ret;
}


/**
 * vmw_surface_validate - make a surface available to the device, evicting
 * other surfaces if needed.
 *
 * @dev_priv: Pointer to a device private struct.
 * @srf: Pointer to a struct vmw_surface.
 *
 * Try to validate a surface and if it fails due to limited device resources,
 * repeatedly try to evict other surfaces until the request can be
 * acommodated.
 *
 * May return errors if out of resources.
 */
int vmw_surface_validate(struct vmw_private *dev_priv,
			 struct vmw_surface *srf)
{
	int ret;
	struct vmw_surface *evict_srf;

	do {
		write_lock(&dev_priv->resource_lock);
		list_del_init(&srf->lru_head);
		write_unlock(&dev_priv->resource_lock);

		ret = vmw_surface_do_validate(dev_priv, srf);
		if (likely(ret != -EBUSY))
			break;

		write_lock(&dev_priv->resource_lock);
		if (list_empty(&dev_priv->surface_lru)) {
			DRM_ERROR("Out of device memory for surfaces.\n");
			ret = -EBUSY;
			write_unlock(&dev_priv->resource_lock);
			break;
		}

		evict_srf = vmw_surface_reference
			(list_first_entry(&dev_priv->surface_lru,
					  struct vmw_surface,
					  lru_head));
		list_del_init(&evict_srf->lru_head);

		write_unlock(&dev_priv->resource_lock);
		(void) vmw_surface_evict(dev_priv, evict_srf);

		vmw_surface_unreference(&evict_srf);

	} while (1);

	if (unlikely(ret != 0 && srf->res.id != -1)) {
		write_lock(&dev_priv->resource_lock);
		list_add_tail(&srf->lru_head, &dev_priv->surface_lru);
		write_unlock(&dev_priv->resource_lock);
	}

	return ret;
}


/**
 * vmw_surface_remove_from_lists - Remove surface resources from lookup lists
 *
 * @res: Pointer to a struct vmw_resource embedded in a struct vmw_surface
 *
 * As part of the resource destruction, remove the surface from any
 * lookup lists.
 */
static void vmw_surface_remove_from_lists(struct vmw_resource *res)
{
	struct vmw_surface *srf = container_of(res, struct vmw_surface, res);

	list_del_init(&srf->lru_head);
}

int vmw_surface_init(struct vmw_private *dev_priv,
		     struct vmw_surface *srf,
		     void (*res_free) (struct vmw_resource *res))
{
	int ret;
	struct vmw_resource *res = &srf->res;

	BUG_ON(res_free == NULL);
	INIT_LIST_HEAD(&srf->lru_head);
	ret = vmw_resource_init(dev_priv, res, &dev_priv->surface_idr,
				VMW_RES_SURFACE, true, res_free,
				vmw_surface_remove_from_lists);

	if (unlikely(ret != 0))
		res_free(res);

	/*
	 * The surface won't be visible to hardware until a
	 * surface validate.
	 */

	(void) vmw_3d_resource_inc(dev_priv, false);
	vmw_resource_activate(res, vmw_hw_surface_destroy);
	return ret;
}

static void vmw_user_surface_free(struct vmw_resource *res)
{
	struct vmw_surface *srf = container_of(res, struct vmw_surface, res);
	struct vmw_user_surface *user_srf =
	    container_of(srf, struct vmw_user_surface, srf);
	struct vmw_private *dev_priv = srf->res.dev_priv;
	uint32_t size = user_srf->size;

	if (srf->backup)
		ttm_bo_unref(&srf->backup);
	kfree(srf->offsets);
	kfree(srf->sizes);
	kfree(srf->snooper.image);
	kfree(user_srf);
	ttm_mem_global_free(vmw_mem_glob(dev_priv), size);
}

/**
 * vmw_resource_unreserve - unreserve resources previously reserved for
 * command submission.
 *
 * @list_head: list of resources to unreserve.
 *
 * Currently only surfaces are considered, and unreserving a surface
 * means putting it back on the device's surface lru list,
 * so that it can be evicted if necessary.
 * This function traverses the resource list and
 * checks whether resources are surfaces, and in that case puts them back
 * on the device's surface LRU list.
 */
void vmw_resource_unreserve(struct list_head *list)
{
	struct vmw_resource *res;
	struct vmw_surface *srf;
	rwlock_t *lock = NULL;

	list_for_each_entry(res, list, validate_head) {

		if (res->res_free != &vmw_surface_res_free &&
		    res->res_free != &vmw_user_surface_free)
			continue;

		if (unlikely(lock == NULL)) {
			lock = &res->dev_priv->resource_lock;
			write_lock(lock);
		}

		srf = container_of(res, struct vmw_surface, res);
		list_del_init(&srf->lru_head);
		list_add_tail(&srf->lru_head, &res->dev_priv->surface_lru);
	}

	if (lock != NULL)
		write_unlock(lock);
}

/**
 * Helper function that looks either a surface or dmabuf.
=======
/*
 * Helper function that looks either a surface or bo.
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 *
 * The pointer this pointed at by out_surf and out_buf needs to be null.
 */
int vmw_user_lookup_handle(struct vmw_private *dev_priv,
<<<<<<< HEAD
			   struct ttm_object_file *tfile,
			   uint32_t handle,
			   struct vmw_surface **out_surf,
			   struct vmw_dma_buffer **out_buf)
{
=======
			   struct drm_file *filp,
			   uint32_t handle,
			   struct vmw_surface **out_surf,
			   struct vmw_bo **out_buf)
{
	struct ttm_object_file *tfile = vmw_fpriv(filp)->tfile;
	struct vmw_resource *res;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	int ret;

	BUG_ON(*out_surf || *out_buf);

<<<<<<< HEAD
	ret = vmw_user_surface_lookup_handle(dev_priv, tfile, handle, out_surf);
	if (!ret)
		return 0;

	ret = vmw_user_dmabuf_lookup(tfile, handle, out_buf);
	return ret;
}


int vmw_user_surface_lookup_handle(struct vmw_private *dev_priv,
				   struct ttm_object_file *tfile,
				   uint32_t handle, struct vmw_surface **out)
{
	struct vmw_resource *res;
	struct vmw_surface *srf;
	struct vmw_user_surface *user_srf;
	struct ttm_base_object *base;
	int ret = -EINVAL;

	base = ttm_base_object_lookup(tfile, handle);
	if (unlikely(base == NULL))
		return -EINVAL;

	if (unlikely(base->object_type != VMW_RES_SURFACE))
		goto out_bad_resource;

	user_srf = container_of(base, struct vmw_user_surface, base);
	srf = &user_srf->srf;
	res = &srf->res;

	read_lock(&dev_priv->resource_lock);

	if (!res->avail || res->res_free != &vmw_user_surface_free) {
		read_unlock(&dev_priv->resource_lock);
		goto out_bad_resource;
	}

	kref_get(&res->kref);
	read_unlock(&dev_priv->resource_lock);

	*out = srf;
	ret = 0;

out_bad_resource:
	ttm_base_object_unref(&base);

	return ret;
}

static void vmw_user_surface_base_release(struct ttm_base_object **p_base)
{
	struct ttm_base_object *base = *p_base;
	struct vmw_user_surface *user_srf =
	    container_of(base, struct vmw_user_surface, base);
	struct vmw_resource *res = &user_srf->srf.res;

	*p_base = NULL;
	vmw_resource_unreference(&res);
}

int vmw_surface_destroy_ioctl(struct drm_device *dev, void *data,
			      struct drm_file *file_priv)
{
	struct drm_vmw_surface_arg *arg = (struct drm_vmw_surface_arg *)data;
	struct ttm_object_file *tfile = vmw_fpriv(file_priv)->tfile;

	return ttm_ref_object_base_unref(tfile, arg->sid, TTM_REF_USAGE);
}

int vmw_surface_define_ioctl(struct drm_device *dev, void *data,
			     struct drm_file *file_priv)
{
	struct vmw_private *dev_priv = vmw_priv(dev);
	struct vmw_user_surface *user_srf;
	struct vmw_surface *srf;
	struct vmw_resource *res;
	struct vmw_resource *tmp;
	union drm_vmw_surface_create_arg *arg =
	    (union drm_vmw_surface_create_arg *)data;
	struct drm_vmw_surface_create_req *req = &arg->req;
	struct drm_vmw_surface_arg *rep = &arg->rep;
	struct ttm_object_file *tfile = vmw_fpriv(file_priv)->tfile;
	struct drm_vmw_size __user *user_sizes;
	int ret;
	int i, j;
	uint32_t cur_bo_offset;
	struct drm_vmw_size *cur_size;
	struct vmw_surface_offset *cur_offset;
	uint32_t stride_bpp;
	uint32_t bpp;
	uint32_t num_sizes;
	uint32_t size;
	struct vmw_master *vmaster = vmw_master(file_priv->master);

	if (unlikely(vmw_user_surface_size == 0))
		vmw_user_surface_size = ttm_round_pot(sizeof(*user_srf)) +
			128;

	num_sizes = 0;
	for (i = 0; i < DRM_VMW_MAX_SURFACE_FACES; ++i)
		num_sizes += req->mip_levels[i];

	if (num_sizes > DRM_VMW_MAX_SURFACE_FACES *
	    DRM_VMW_MAX_MIP_LEVELS)
		return -EINVAL;

	size = vmw_user_surface_size + 128 +
		ttm_round_pot(num_sizes * sizeof(struct drm_vmw_size)) +
		ttm_round_pot(num_sizes * sizeof(struct vmw_surface_offset));


	ret = ttm_read_lock(&vmaster->lock, true);
	if (unlikely(ret != 0))
		return ret;

	ret = ttm_mem_global_alloc(vmw_mem_glob(dev_priv),
				   size, false, true);
	if (unlikely(ret != 0)) {
		if (ret != -ERESTARTSYS)
			DRM_ERROR("Out of graphics memory for surface"
				  " creation.\n");
		goto out_unlock;
	}

	user_srf = kmalloc(sizeof(*user_srf), GFP_KERNEL);
	if (unlikely(user_srf == NULL)) {
		ret = -ENOMEM;
		goto out_no_user_srf;
	}

	srf = &user_srf->srf;
	res = &srf->res;

	srf->flags = req->flags;
	srf->format = req->format;
	srf->scanout = req->scanout;
	srf->backup = NULL;

	memcpy(srf->mip_levels, req->mip_levels, sizeof(srf->mip_levels));
	srf->num_sizes = num_sizes;
	user_srf->size = size;

	srf->sizes = kmalloc(srf->num_sizes * sizeof(*srf->sizes), GFP_KERNEL);
	if (unlikely(srf->sizes == NULL)) {
		ret = -ENOMEM;
		goto out_no_sizes;
	}
	srf->offsets = kmalloc(srf->num_sizes * sizeof(*srf->offsets),
			       GFP_KERNEL);
	if (unlikely(srf->sizes == NULL)) {
		ret = -ENOMEM;
		goto out_no_offsets;
	}

	user_sizes = (struct drm_vmw_size __user *)(unsigned long)
	    req->size_addr;

	ret = copy_from_user(srf->sizes, user_sizes,
			     srf->num_sizes * sizeof(*srf->sizes));
	if (unlikely(ret != 0)) {
		ret = -EFAULT;
		goto out_no_copy;
	}

	cur_bo_offset = 0;
	cur_offset = srf->offsets;
	cur_size = srf->sizes;

	bpp = vmw_sf_bpp[srf->format].bpp;
	stride_bpp = vmw_sf_bpp[srf->format].s_bpp;

	for (i = 0; i < DRM_VMW_MAX_SURFACE_FACES; ++i) {
		for (j = 0; j < srf->mip_levels[i]; ++j) {
			uint32_t stride =
				(cur_size->width * stride_bpp + 7) >> 3;

			cur_offset->face = i;
			cur_offset->mip = j;
			cur_offset->bo_offset = cur_bo_offset;
			cur_bo_offset += stride * cur_size->height *
				cur_size->depth * bpp / stride_bpp;
			++cur_offset;
			++cur_size;
		}
	}
	srf->backup_size = cur_bo_offset;

	if (srf->scanout &&
	    srf->num_sizes == 1 &&
	    srf->sizes[0].width == 64 &&
	    srf->sizes[0].height == 64 &&
	    srf->format == SVGA3D_A8R8G8B8) {

		/* allocate image area and clear it */
		srf->snooper.image = kzalloc(64 * 64 * 4, GFP_KERNEL);
		if (!srf->snooper.image) {
			DRM_ERROR("Failed to allocate cursor_image\n");
			ret = -ENOMEM;
			goto out_no_copy;
		}
	} else {
		srf->snooper.image = NULL;
	}
	srf->snooper.crtc = NULL;

	user_srf->base.shareable = false;
	user_srf->base.tfile = NULL;

	/**
	 * From this point, the generic resource management functions
	 * destroy the object on failure.
	 */

	ret = vmw_surface_init(dev_priv, srf, vmw_user_surface_free);
	if (unlikely(ret != 0))
		goto out_unlock;

	tmp = vmw_resource_reference(&srf->res);
	ret = ttm_base_object_init(tfile, &user_srf->base,
				   req->shareable, VMW_RES_SURFACE,
				   &vmw_user_surface_base_release, NULL);

	if (unlikely(ret != 0)) {
		vmw_resource_unreference(&tmp);
		vmw_resource_unreference(&res);
		goto out_unlock;
	}

	rep->sid = user_srf->base.hash.key;
	if (rep->sid == SVGA3D_INVALID_ID)
		DRM_ERROR("Created bad Surface ID.\n");

	vmw_resource_unreference(&res);

	ttm_read_unlock(&vmaster->lock);
	return 0;
out_no_copy:
	kfree(srf->offsets);
out_no_offsets:
	kfree(srf->sizes);
out_no_sizes:
	kfree(user_srf);
out_no_user_srf:
	ttm_mem_global_free(vmw_mem_glob(dev_priv), size);
out_unlock:
	ttm_read_unlock(&vmaster->lock);
	return ret;
}

int vmw_surface_reference_ioctl(struct drm_device *dev, void *data,
				struct drm_file *file_priv)
{
	union drm_vmw_surface_reference_arg *arg =
	    (union drm_vmw_surface_reference_arg *)data;
	struct drm_vmw_surface_arg *req = &arg->req;
	struct drm_vmw_surface_create_req *rep = &arg->rep;
	struct ttm_object_file *tfile = vmw_fpriv(file_priv)->tfile;
	struct vmw_surface *srf;
	struct vmw_user_surface *user_srf;
	struct drm_vmw_size __user *user_sizes;
	struct ttm_base_object *base;
	int ret = -EINVAL;

	base = ttm_base_object_lookup(tfile, req->sid);
	if (unlikely(base == NULL)) {
		DRM_ERROR("Could not find surface to reference.\n");
		return -EINVAL;
	}

	if (unlikely(base->object_type != VMW_RES_SURFACE))
		goto out_bad_resource;

	user_srf = container_of(base, struct vmw_user_surface, base);
	srf = &user_srf->srf;

	ret = ttm_ref_object_add(tfile, &user_srf->base, TTM_REF_USAGE, NULL);
	if (unlikely(ret != 0)) {
		DRM_ERROR("Could not add a reference to a surface.\n");
		goto out_no_reference;
	}

	rep->flags = srf->flags;
	rep->format = srf->format;
	memcpy(rep->mip_levels, srf->mip_levels, sizeof(srf->mip_levels));
	user_sizes = (struct drm_vmw_size __user *)(unsigned long)
	    rep->size_addr;

	if (user_sizes)
		ret = copy_to_user(user_sizes, srf->sizes,
				   srf->num_sizes * sizeof(*srf->sizes));
	if (unlikely(ret != 0)) {
		DRM_ERROR("copy_to_user failed %p %u\n",
			  user_sizes, srf->num_sizes);
		ret = -EFAULT;
	}
out_bad_resource:
out_no_reference:
	ttm_base_object_unref(&base);

	return ret;
}

int vmw_surface_check(struct vmw_private *dev_priv,
		      struct ttm_object_file *tfile,
		      uint32_t handle, int *id)
{
	struct ttm_base_object *base;
	struct vmw_user_surface *user_srf;

	int ret = -EPERM;

	base = ttm_base_object_lookup(tfile, handle);
	if (unlikely(base == NULL))
		return -EINVAL;

	if (unlikely(base->object_type != VMW_RES_SURFACE))
		goto out_bad_surface;

	user_srf = container_of(base, struct vmw_user_surface, base);
	*id = user_srf->srf.res.id;
	ret = 0;

out_bad_surface:
	/**
	 * FIXME: May deadlock here when called from the
	 * command parsing code.
	 */

	ttm_base_object_unref(&base);
=======
	ret = vmw_user_resource_lookup_handle(dev_priv, tfile, handle,
					      user_surface_converter,
					      &res);
	if (!ret) {
		*out_surf = vmw_res_to_srf(res);
		return 0;
	}

	*out_surf = NULL;
	ret = vmw_user_bo_lookup(filp, handle, out_buf);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	return ret;
}

/**
<<<<<<< HEAD
 * Buffer management.
 */
void vmw_dmabuf_bo_free(struct ttm_buffer_object *bo)
{
	struct vmw_dma_buffer *vmw_bo = vmw_dma_buffer(bo);

	kfree(vmw_bo);
}

int vmw_dmabuf_init(struct vmw_private *dev_priv,
		    struct vmw_dma_buffer *vmw_bo,
		    size_t size, struct ttm_placement *placement,
		    bool interruptible,
		    void (*bo_free) (struct ttm_buffer_object *bo))
{
	struct ttm_bo_device *bdev = &dev_priv->bdev;
	size_t acc_size;
	int ret;

	BUG_ON(!bo_free);

	acc_size = ttm_bo_acc_size(bdev, size, sizeof(struct vmw_dma_buffer));
	memset(vmw_bo, 0, sizeof(*vmw_bo));

	INIT_LIST_HEAD(&vmw_bo->validate_list);

	ret = ttm_bo_init(bdev, &vmw_bo->base, size,
			  ttm_bo_type_device, placement,
			  0, 0, interruptible,
			  NULL, acc_size, bo_free);
	return ret;
}

static void vmw_user_dmabuf_destroy(struct ttm_buffer_object *bo)
{
	struct vmw_user_dma_buffer *vmw_user_bo = vmw_user_dma_buffer(bo);

	kfree(vmw_user_bo);
}

static void vmw_user_dmabuf_release(struct ttm_base_object **p_base)
{
	struct vmw_user_dma_buffer *vmw_user_bo;
	struct ttm_base_object *base = *p_base;
	struct ttm_buffer_object *bo;

	*p_base = NULL;

	if (unlikely(base == NULL))
		return;

	vmw_user_bo = container_of(base, struct vmw_user_dma_buffer, base);
	bo = &vmw_user_bo->dma.base;
	ttm_bo_unref(&bo);
}

int vmw_dmabuf_alloc_ioctl(struct drm_device *dev, void *data,
			   struct drm_file *file_priv)
{
	struct vmw_private *dev_priv = vmw_priv(dev);
	union drm_vmw_alloc_dmabuf_arg *arg =
	    (union drm_vmw_alloc_dmabuf_arg *)data;
	struct drm_vmw_alloc_dmabuf_req *req = &arg->req;
	struct drm_vmw_dmabuf_rep *rep = &arg->rep;
	struct vmw_user_dma_buffer *vmw_user_bo;
	struct ttm_buffer_object *tmp;
	struct vmw_master *vmaster = vmw_master(file_priv->master);
	int ret;

	vmw_user_bo = kzalloc(sizeof(*vmw_user_bo), GFP_KERNEL);
	if (unlikely(vmw_user_bo == NULL))
		return -ENOMEM;

	ret = ttm_read_lock(&vmaster->lock, true);
	if (unlikely(ret != 0)) {
		kfree(vmw_user_bo);
		return ret;
	}

	ret = vmw_dmabuf_init(dev_priv, &vmw_user_bo->dma, req->size,
			      &vmw_vram_sys_placement, true,
			      &vmw_user_dmabuf_destroy);
	if (unlikely(ret != 0))
		goto out_no_dmabuf;

	tmp = ttm_bo_reference(&vmw_user_bo->dma.base);
	ret = ttm_base_object_init(vmw_fpriv(file_priv)->tfile,
				   &vmw_user_bo->base,
				   false,
				   ttm_buffer_type,
				   &vmw_user_dmabuf_release, NULL);
	if (unlikely(ret != 0))
		goto out_no_base_object;
	else {
		rep->handle = vmw_user_bo->base.hash.key;
		rep->map_handle = vmw_user_bo->dma.base.addr_space_offset;
		rep->cur_gmr_id = vmw_user_bo->base.hash.key;
		rep->cur_gmr_offset = 0;
	}

out_no_base_object:
	ttm_bo_unref(&tmp);
out_no_dmabuf:
	ttm_read_unlock(&vmaster->lock);

	return ret;
}

int vmw_dmabuf_unref_ioctl(struct drm_device *dev, void *data,
			   struct drm_file *file_priv)
{
	struct drm_vmw_unref_dmabuf_arg *arg =
	    (struct drm_vmw_unref_dmabuf_arg *)data;

	return ttm_ref_object_base_unref(vmw_fpriv(file_priv)->tfile,
					 arg->handle,
					 TTM_REF_USAGE);
}

uint32_t vmw_dmabuf_validate_node(struct ttm_buffer_object *bo,
				  uint32_t cur_validate_node)
{
	struct vmw_dma_buffer *vmw_bo = vmw_dma_buffer(bo);

	if (likely(vmw_bo->on_validate_list))
		return vmw_bo->cur_validate_node;

	vmw_bo->cur_validate_node = cur_validate_node;
	vmw_bo->on_validate_list = true;

	return cur_validate_node;
}

void vmw_dmabuf_validate_clear(struct ttm_buffer_object *bo)
{
	struct vmw_dma_buffer *vmw_bo = vmw_dma_buffer(bo);

	vmw_bo->on_validate_list = false;
}

int vmw_user_dmabuf_lookup(struct ttm_object_file *tfile,
			   uint32_t handle, struct vmw_dma_buffer **out)
{
	struct vmw_user_dma_buffer *vmw_user_bo;
	struct ttm_base_object *base;

	base = ttm_base_object_lookup(tfile, handle);
	if (unlikely(base == NULL)) {
		printk(KERN_ERR "Invalid buffer object handle 0x%08lx.\n",
		       (unsigned long)handle);
		return -ESRCH;
	}

	if (unlikely(base->object_type != ttm_buffer_type)) {
		ttm_base_object_unref(&base);
		printk(KERN_ERR "Invalid buffer object handle 0x%08lx.\n",
		       (unsigned long)handle);
		return -EINVAL;
	}

	vmw_user_bo = container_of(base, struct vmw_user_dma_buffer, base);
	(void)ttm_bo_reference(&vmw_user_bo->dma.base);
	ttm_base_object_unref(&base);
	*out = &vmw_user_bo->dma;

	return 0;
}

/*
 * Stream management
 */

static void vmw_stream_destroy(struct vmw_resource *res)
{
	struct vmw_private *dev_priv = res->dev_priv;
	struct vmw_stream *stream;
	int ret;

	DRM_INFO("%s: unref\n", __func__);
	stream = container_of(res, struct vmw_stream, res);

	ret = vmw_overlay_unref(dev_priv, stream->stream_id);
	WARN_ON(ret != 0);
}

static int vmw_stream_init(struct vmw_private *dev_priv,
			   struct vmw_stream *stream,
			   void (*res_free) (struct vmw_resource *res))
{
	struct vmw_resource *res = &stream->res;
	int ret;

	ret = vmw_resource_init(dev_priv, res, &dev_priv->stream_idr,
				VMW_RES_STREAM, false, res_free, NULL);

	if (unlikely(ret != 0)) {
		if (res_free == NULL)
			kfree(stream);
		else
			res_free(&stream->res);
		return ret;
	}

	ret = vmw_overlay_claim(dev_priv, &stream->stream_id);
	if (ret) {
		vmw_resource_unreference(&res);
		return ret;
	}

	DRM_INFO("%s: claimed\n", __func__);

	vmw_resource_activate(&stream->res, vmw_stream_destroy);
=======
 * vmw_resource_buf_alloc - Allocate a guest memory buffer for a resource.
 *
 * @res:            The resource for which to allocate a gbo buffer.
 * @interruptible:  Whether any sleeps during allocation should be
 *                  performed while interruptible.
 */
static int vmw_resource_buf_alloc(struct vmw_resource *res,
				  bool interruptible)
{
	unsigned long size = PFN_ALIGN(res->guest_memory_size);
	struct vmw_bo *gbo;
	struct vmw_bo_params bo_params = {
		.domain = res->func->domain,
		.busy_domain = res->func->busy_domain,
		.bo_type = ttm_bo_type_device,
		.size = res->guest_memory_size,
		.pin = false
	};
	int ret;

	if (likely(res->guest_memory_bo)) {
		BUG_ON(res->guest_memory_bo->tbo.base.size < size);
		return 0;
	}

	ret = vmw_gem_object_create(res->dev_priv, &bo_params, &gbo);
	if (unlikely(ret != 0))
		goto out_no_bo;

	res->guest_memory_bo = gbo;

out_no_bo:
	return ret;
}

/**
 * vmw_resource_do_validate - Make a resource up-to-date and visible
 *                            to the device.
 *
 * @res:            The resource to make visible to the device.
 * @val_buf:        Information about a buffer possibly
 *                  containing backup data if a bind operation is needed.
 * @dirtying:       Transfer dirty regions.
 *
 * On hardware resource shortage, this function returns -EBUSY and
 * should be retried once resources have been freed up.
 */
static int vmw_resource_do_validate(struct vmw_resource *res,
				    struct ttm_validate_buffer *val_buf,
				    bool dirtying)
{
	int ret = 0;
	const struct vmw_res_func *func = res->func;

	if (unlikely(res->id == -1)) {
		ret = func->create(res);
		if (unlikely(ret != 0))
			return ret;
	}

	if (func->bind &&
	    ((func->needs_guest_memory && !vmw_resource_mob_attached(res) &&
	      val_buf->bo) ||
	     (!func->needs_guest_memory && val_buf->bo))) {
		ret = func->bind(res, val_buf);
		if (unlikely(ret != 0))
			goto out_bind_failed;
		if (func->needs_guest_memory)
			vmw_resource_mob_attach(res);
	}

	/*
	 * Handle the case where the backup mob is marked coherent but
	 * the resource isn't.
	 */
	if (func->dirty_alloc && vmw_resource_mob_attached(res) &&
	    !res->coherent) {
		if (res->guest_memory_bo->dirty && !res->dirty) {
			ret = func->dirty_alloc(res);
			if (ret)
				return ret;
		} else if (!res->guest_memory_bo->dirty && res->dirty) {
			func->dirty_free(res);
		}
	}

	/*
	 * Transfer the dirty regions to the resource and update
	 * the resource.
	 */
	if (res->dirty) {
		if (dirtying && !res->res_dirty) {
			pgoff_t start = res->guest_memory_offset >> PAGE_SHIFT;
			pgoff_t end = __KERNEL_DIV_ROUND_UP
				(res->guest_memory_offset + res->guest_memory_size,
				 PAGE_SIZE);

			vmw_bo_dirty_unmap(res->guest_memory_bo, start, end);
		}

		vmw_bo_dirty_transfer_to_res(res);
		return func->dirty_sync(res);
	}

	return 0;

out_bind_failed:
	func->destroy(res);

	return ret;
}

/**
 * vmw_resource_unreserve - Unreserve a resource previously reserved for
 * command submission.
 *
 * @res:               Pointer to the struct vmw_resource to unreserve.
 * @dirty_set:         Change dirty status of the resource.
 * @dirty:             When changing dirty status indicates the new status.
 * @switch_guest_memory: Guest memory buffer has been switched.
 * @new_guest_memory_bo: Pointer to new guest memory buffer if command submission
 *                     switched. May be NULL.
 * @new_guest_memory_offset: New gbo offset if @switch_guest_memory is true.
 *
 * Currently unreserving a resource means putting it back on the device's
 * resource lru list, so that it can be evicted if necessary.
 */
void vmw_resource_unreserve(struct vmw_resource *res,
			    bool dirty_set,
			    bool dirty,
			    bool switch_guest_memory,
			    struct vmw_bo *new_guest_memory_bo,
			    unsigned long new_guest_memory_offset)
{
	struct vmw_private *dev_priv = res->dev_priv;

	if (!list_empty(&res->lru_head))
		return;

	if (switch_guest_memory && new_guest_memory_bo != res->guest_memory_bo) {
		if (res->guest_memory_bo) {
			vmw_resource_mob_detach(res);
			if (res->coherent)
				vmw_bo_dirty_release(res->guest_memory_bo);
			vmw_user_bo_unref(&res->guest_memory_bo);
		}

		if (new_guest_memory_bo) {
			res->guest_memory_bo = vmw_user_bo_ref(new_guest_memory_bo);

			/*
			 * The validation code should already have added a
			 * dirty tracker here.
			 */
			WARN_ON(res->coherent && !new_guest_memory_bo->dirty);

			vmw_resource_mob_attach(res);
		} else {
			res->guest_memory_bo = NULL;
		}
	} else if (switch_guest_memory && res->coherent) {
		vmw_bo_dirty_release(res->guest_memory_bo);
	}

	if (switch_guest_memory)
		res->guest_memory_offset = new_guest_memory_offset;

	if (dirty_set)
		res->res_dirty = dirty;

	if (!res->func->may_evict || res->id == -1 || res->pin_count)
		return;

	spin_lock(&dev_priv->resource_lock);
	list_add_tail(&res->lru_head,
		      &res->dev_priv->res_lru[res->func->res_type]);
	spin_unlock(&dev_priv->resource_lock);
}

/**
 * vmw_resource_check_buffer - Check whether a backup buffer is needed
 *                             for a resource and in that case, allocate
 *                             one, reserve and validate it.
 *
 * @ticket:         The ww acquire context to use, or NULL if trylocking.
 * @res:            The resource for which to allocate a backup buffer.
 * @interruptible:  Whether any sleeps during allocation should be
 *                  performed while interruptible.
 * @val_buf:        On successful return contains data about the
 *                  reserved and validated backup buffer.
 */
static int
vmw_resource_check_buffer(struct ww_acquire_ctx *ticket,
			  struct vmw_resource *res,
			  bool interruptible,
			  struct ttm_validate_buffer *val_buf)
{
	struct ttm_operation_ctx ctx = { true, false };
	struct list_head val_list;
	bool guest_memory_dirty = false;
	int ret;

	if (unlikely(!res->guest_memory_bo)) {
		ret = vmw_resource_buf_alloc(res, interruptible);
		if (unlikely(ret != 0))
			return ret;
	}

	INIT_LIST_HEAD(&val_list);
	ttm_bo_get(&res->guest_memory_bo->tbo);
	val_buf->bo = &res->guest_memory_bo->tbo;
	val_buf->num_shared = 0;
	list_add_tail(&val_buf->head, &val_list);
	ret = ttm_eu_reserve_buffers(ticket, &val_list, interruptible, NULL);
	if (unlikely(ret != 0))
		goto out_no_reserve;

	if (res->func->needs_guest_memory && !vmw_resource_mob_attached(res))
		return 0;

	guest_memory_dirty = res->guest_memory_dirty;
	vmw_bo_placement_set(res->guest_memory_bo, res->func->domain,
			     res->func->busy_domain);
	ret = ttm_bo_validate(&res->guest_memory_bo->tbo,
			      &res->guest_memory_bo->placement,
			      &ctx);

	if (unlikely(ret != 0))
		goto out_no_validate;

	return 0;

out_no_validate:
	ttm_eu_backoff_reservation(ticket, &val_list);
out_no_reserve:
	ttm_bo_put(val_buf->bo);
	val_buf->bo = NULL;
	if (guest_memory_dirty)
		vmw_user_bo_unref(&res->guest_memory_bo);

	return ret;
}

/*
 * vmw_resource_reserve - Reserve a resource for command submission
 *
 * @res:            The resource to reserve.
 *
 * This function takes the resource off the LRU list and make sure
 * a guest memory buffer is present for guest-backed resources.
 * However, the buffer may not be bound to the resource at this
 * point.
 *
 */
int vmw_resource_reserve(struct vmw_resource *res, bool interruptible,
			 bool no_guest_memory)
{
	struct vmw_private *dev_priv = res->dev_priv;
	int ret;

	spin_lock(&dev_priv->resource_lock);
	list_del_init(&res->lru_head);
	spin_unlock(&dev_priv->resource_lock);

	if (res->func->needs_guest_memory && !res->guest_memory_bo &&
	    !no_guest_memory) {
		ret = vmw_resource_buf_alloc(res, interruptible);
		if (unlikely(ret != 0)) {
			DRM_ERROR("Failed to allocate a guest memory buffer "
				  "of size %lu. bytes\n",
				  (unsigned long) res->guest_memory_size);
			return ret;
		}
	}

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	return 0;
}

/**
<<<<<<< HEAD
 * User-space context management:
 */

static void vmw_user_stream_free(struct vmw_resource *res)
{
	struct vmw_user_stream *stream =
	    container_of(res, struct vmw_user_stream, stream.res);
	struct vmw_private *dev_priv = res->dev_priv;

	kfree(stream);
	ttm_mem_global_free(vmw_mem_glob(dev_priv),
			    vmw_user_stream_size);
}

/**
 * This function is called when user space has no more references on the
 * base object. It releases the base-object's reference on the resource object.
 */

static void vmw_user_stream_base_release(struct ttm_base_object **p_base)
{
	struct ttm_base_object *base = *p_base;
	struct vmw_user_stream *stream =
	    container_of(base, struct vmw_user_stream, base);
	struct vmw_resource *res = &stream->stream.res;

	*p_base = NULL;
	vmw_resource_unreference(&res);
}

int vmw_stream_unref_ioctl(struct drm_device *dev, void *data,
			   struct drm_file *file_priv)
{
	struct vmw_private *dev_priv = vmw_priv(dev);
	struct vmw_resource *res;
	struct vmw_user_stream *stream;
	struct drm_vmw_stream_arg *arg = (struct drm_vmw_stream_arg *)data;
	struct ttm_object_file *tfile = vmw_fpriv(file_priv)->tfile;
	int ret = 0;

	res = vmw_resource_lookup(dev_priv, &dev_priv->stream_idr, arg->stream_id);
	if (unlikely(res == NULL))
		return -EINVAL;

	if (res->res_free != &vmw_user_stream_free) {
		ret = -EINVAL;
		goto out;
	}

	stream = container_of(res, struct vmw_user_stream, stream.res);
	if (stream->base.tfile != tfile) {
		ret = -EINVAL;
		goto out;
	}

	ttm_ref_object_base_unref(tfile, stream->base.hash.key, TTM_REF_USAGE);
out:
	vmw_resource_unreference(&res);
	return ret;
}

int vmw_stream_claim_ioctl(struct drm_device *dev, void *data,
			   struct drm_file *file_priv)
{
	struct vmw_private *dev_priv = vmw_priv(dev);
	struct vmw_user_stream *stream;
	struct vmw_resource *res;
	struct vmw_resource *tmp;
	struct drm_vmw_stream_arg *arg = (struct drm_vmw_stream_arg *)data;
	struct ttm_object_file *tfile = vmw_fpriv(file_priv)->tfile;
	struct vmw_master *vmaster = vmw_master(file_priv->master);
	int ret;

	/*
	 * Approximate idr memory usage with 128 bytes. It will be limited
	 * by maximum number_of streams anyway?
	 */

	if (unlikely(vmw_user_stream_size == 0))
		vmw_user_stream_size = ttm_round_pot(sizeof(*stream)) + 128;

	ret = ttm_read_lock(&vmaster->lock, true);
	if (unlikely(ret != 0))
		return ret;

	ret = ttm_mem_global_alloc(vmw_mem_glob(dev_priv),
				   vmw_user_stream_size,
				   false, true);
	if (unlikely(ret != 0)) {
		if (ret != -ERESTARTSYS)
			DRM_ERROR("Out of graphics memory for stream"
				  " creation.\n");
		goto out_unlock;
	}


	stream = kmalloc(sizeof(*stream), GFP_KERNEL);
	if (unlikely(stream == NULL)) {
		ttm_mem_global_free(vmw_mem_glob(dev_priv),
				    vmw_user_stream_size);
		ret = -ENOMEM;
		goto out_unlock;
	}

	res = &stream->stream.res;
	stream->base.shareable = false;
	stream->base.tfile = NULL;

	/*
	 * From here on, the destructor takes over resource freeing.
	 */

	ret = vmw_stream_init(dev_priv, &stream->stream, vmw_user_stream_free);
	if (unlikely(ret != 0))
		goto out_unlock;

	tmp = vmw_resource_reference(res);
	ret = ttm_base_object_init(tfile, &stream->base, false, VMW_RES_STREAM,
				   &vmw_user_stream_base_release, NULL);

	if (unlikely(ret != 0)) {
		vmw_resource_unreference(&tmp);
		goto out_err;
	}

	arg->stream_id = res->id;
out_err:
	vmw_resource_unreference(&res);
out_unlock:
	ttm_read_unlock(&vmaster->lock);
	return ret;
}

int vmw_user_stream_lookup(struct vmw_private *dev_priv,
			   struct ttm_object_file *tfile,
			   uint32_t *inout_id, struct vmw_resource **out)
{
	struct vmw_user_stream *stream;
	struct vmw_resource *res;
	int ret;

	res = vmw_resource_lookup(dev_priv, &dev_priv->stream_idr, *inout_id);
	if (unlikely(res == NULL))
		return -EINVAL;

	if (res->res_free != &vmw_user_stream_free) {
		ret = -EINVAL;
		goto err_ref;
	}

	stream = container_of(res, struct vmw_user_stream, stream.res);
	if (stream->base.tfile != tfile) {
		ret = -EPERM;
		goto err_ref;
	}

	*inout_id = stream->stream.stream_id;
	*out = res;
	return 0;
err_ref:
	vmw_resource_unreference(&res);
=======
 * vmw_resource_backoff_reservation - Unreserve and unreference a
 *                                    guest memory buffer
 *.
 * @ticket:         The ww acquire ctx used for reservation.
 * @val_buf:        Guest memory buffer information.
 */
static void
vmw_resource_backoff_reservation(struct ww_acquire_ctx *ticket,
				 struct ttm_validate_buffer *val_buf)
{
	struct list_head val_list;

	if (likely(val_buf->bo == NULL))
		return;

	INIT_LIST_HEAD(&val_list);
	list_add_tail(&val_buf->head, &val_list);
	ttm_eu_backoff_reservation(ticket, &val_list);
	ttm_bo_put(val_buf->bo);
	val_buf->bo = NULL;
}

/**
 * vmw_resource_do_evict - Evict a resource, and transfer its data
 *                         to a backup buffer.
 *
 * @ticket:         The ww acquire ticket to use, or NULL if trylocking.
 * @res:            The resource to evict.
 * @interruptible:  Whether to wait interruptible.
 */
static int vmw_resource_do_evict(struct ww_acquire_ctx *ticket,
				 struct vmw_resource *res, bool interruptible)
{
	struct ttm_validate_buffer val_buf;
	const struct vmw_res_func *func = res->func;
	int ret;

	BUG_ON(!func->may_evict);

	val_buf.bo = NULL;
	val_buf.num_shared = 0;
	ret = vmw_resource_check_buffer(ticket, res, interruptible, &val_buf);
	if (unlikely(ret != 0))
		return ret;

	if (unlikely(func->unbind != NULL &&
		     (!func->needs_guest_memory || vmw_resource_mob_attached(res)))) {
		ret = func->unbind(res, res->res_dirty, &val_buf);
		if (unlikely(ret != 0))
			goto out_no_unbind;
		vmw_resource_mob_detach(res);
	}
	ret = func->destroy(res);
	res->guest_memory_dirty = true;
	res->res_dirty = false;
out_no_unbind:
	vmw_resource_backoff_reservation(ticket, &val_buf);

	return ret;
}


/**
 * vmw_resource_validate - Make a resource up-to-date and visible
 *                         to the device.
 * @res: The resource to make visible to the device.
 * @intr: Perform waits interruptible if possible.
 * @dirtying: Pending GPU operation will dirty the resource
 *
 * On successful return, any backup DMA buffer pointed to by @res->backup will
 * be reserved and validated.
 * On hardware resource shortage, this function will repeatedly evict
 * resources of the same type until the validation succeeds.
 *
 * Return: Zero on success, -ERESTARTSYS if interrupted, negative error code
 * on failure.
 */
int vmw_resource_validate(struct vmw_resource *res, bool intr,
			  bool dirtying)
{
	int ret;
	struct vmw_resource *evict_res;
	struct vmw_private *dev_priv = res->dev_priv;
	struct list_head *lru_list = &dev_priv->res_lru[res->func->res_type];
	struct ttm_validate_buffer val_buf;
	unsigned err_count = 0;

	if (!res->func->create)
		return 0;

	val_buf.bo = NULL;
	val_buf.num_shared = 0;
	if (res->guest_memory_bo)
		val_buf.bo = &res->guest_memory_bo->tbo;
	do {
		ret = vmw_resource_do_validate(res, &val_buf, dirtying);
		if (likely(ret != -EBUSY))
			break;

		spin_lock(&dev_priv->resource_lock);
		if (list_empty(lru_list) || !res->func->may_evict) {
			DRM_ERROR("Out of device device resources "
				  "for %s.\n", res->func->type_name);
			ret = -EBUSY;
			spin_unlock(&dev_priv->resource_lock);
			break;
		}

		evict_res = vmw_resource_reference
			(list_first_entry(lru_list, struct vmw_resource,
					  lru_head));
		list_del_init(&evict_res->lru_head);

		spin_unlock(&dev_priv->resource_lock);

		/* Trylock backup buffers with a NULL ticket. */
		ret = vmw_resource_do_evict(NULL, evict_res, intr);
		if (unlikely(ret != 0)) {
			spin_lock(&dev_priv->resource_lock);
			list_add_tail(&evict_res->lru_head, lru_list);
			spin_unlock(&dev_priv->resource_lock);
			if (ret == -ERESTARTSYS ||
			    ++err_count > VMW_RES_EVICT_ERR_COUNT) {
				vmw_resource_unreference(&evict_res);
				goto out_no_validate;
			}
		}

		vmw_resource_unreference(&evict_res);
	} while (1);

	if (unlikely(ret != 0))
		goto out_no_validate;
	else if (!res->func->needs_guest_memory && res->guest_memory_bo) {
		WARN_ON_ONCE(vmw_resource_mob_attached(res));
		vmw_user_bo_unref(&res->guest_memory_bo);
	}

	return 0;

out_no_validate:
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	return ret;
}


<<<<<<< HEAD
int vmw_dumb_create(struct drm_file *file_priv,
		    struct drm_device *dev,
		    struct drm_mode_create_dumb *args)
{
	struct vmw_private *dev_priv = vmw_priv(dev);
	struct vmw_master *vmaster = vmw_master(file_priv->master);
	struct vmw_user_dma_buffer *vmw_user_bo;
	struct ttm_buffer_object *tmp;
	int ret;

	args->pitch = args->width * ((args->bpp + 7) / 8);
	args->size = args->pitch * args->height;

	vmw_user_bo = kzalloc(sizeof(*vmw_user_bo), GFP_KERNEL);
	if (vmw_user_bo == NULL)
		return -ENOMEM;

	ret = ttm_read_lock(&vmaster->lock, true);
	if (ret != 0) {
		kfree(vmw_user_bo);
		return ret;
	}

	ret = vmw_dmabuf_init(dev_priv, &vmw_user_bo->dma, args->size,
			      &vmw_vram_sys_placement, true,
			      &vmw_user_dmabuf_destroy);
	if (ret != 0)
		goto out_no_dmabuf;

	tmp = ttm_bo_reference(&vmw_user_bo->dma.base);
	ret = ttm_base_object_init(vmw_fpriv(file_priv)->tfile,
				   &vmw_user_bo->base,
				   false,
				   ttm_buffer_type,
				   &vmw_user_dmabuf_release, NULL);
	if (unlikely(ret != 0))
		goto out_no_base_object;

	args->handle = vmw_user_bo->base.hash.key;

out_no_base_object:
	ttm_bo_unref(&tmp);
out_no_dmabuf:
	ttm_read_unlock(&vmaster->lock);
	return ret;
}

int vmw_dumb_map_offset(struct drm_file *file_priv,
			struct drm_device *dev, uint32_t handle,
			uint64_t *offset)
{
	struct ttm_object_file *tfile = vmw_fpriv(file_priv)->tfile;
	struct vmw_dma_buffer *out_buf;
	int ret;

	ret = vmw_user_dmabuf_lookup(tfile, handle, &out_buf);
	if (ret != 0)
		return -EINVAL;

	*offset = out_buf->base.addr_space_offset;
	vmw_dmabuf_unreference(&out_buf);
	return 0;
}

int vmw_dumb_destroy(struct drm_file *file_priv,
		     struct drm_device *dev,
		     uint32_t handle)
{
	return ttm_ref_object_base_unref(vmw_fpriv(file_priv)->tfile,
					 handle, TTM_REF_USAGE);
=======
/**
 * vmw_resource_unbind_list
 *
 * @vbo: Pointer to the current backing MOB.
 *
 * Evicts the Guest Backed hardware resource if the backup
 * buffer is being moved out of MOB memory.
 * Note that this function will not race with the resource
 * validation code, since resource validation and eviction
 * both require the backup buffer to be reserved.
 */
void vmw_resource_unbind_list(struct vmw_bo *vbo)
{
	struct ttm_validate_buffer val_buf = {
		.bo = &vbo->tbo,
		.num_shared = 0
	};

	dma_resv_assert_held(vbo->tbo.base.resv);
	while (!RB_EMPTY_ROOT(&vbo->res_tree)) {
		struct rb_node *node = vbo->res_tree.rb_node;
		struct vmw_resource *res =
			container_of(node, struct vmw_resource, mob_node);

		if (!WARN_ON_ONCE(!res->func->unbind))
			(void) res->func->unbind(res, res->res_dirty, &val_buf);

		res->guest_memory_size = true;
		res->res_dirty = false;
		vmw_resource_mob_detach(res);
	}

	(void) ttm_bo_wait(&vbo->tbo, false, false);
}


/**
 * vmw_query_readback_all - Read back cached query states
 *
 * @dx_query_mob: Buffer containing the DX query MOB
 *
 * Read back cached states from the device if they exist.  This function
 * assumes binding_mutex is held.
 */
int vmw_query_readback_all(struct vmw_bo *dx_query_mob)
{
	struct vmw_resource *dx_query_ctx;
	struct vmw_private *dev_priv;
	struct {
		SVGA3dCmdHeader header;
		SVGA3dCmdDXReadbackAllQuery body;
	} *cmd;


	/* No query bound, so do nothing */
	if (!dx_query_mob || !dx_query_mob->dx_query_ctx)
		return 0;

	dx_query_ctx = dx_query_mob->dx_query_ctx;
	dev_priv     = dx_query_ctx->dev_priv;

	cmd = VMW_CMD_CTX_RESERVE(dev_priv, sizeof(*cmd), dx_query_ctx->id);
	if (unlikely(cmd == NULL))
		return -ENOMEM;

	cmd->header.id   = SVGA_3D_CMD_DX_READBACK_ALL_QUERY;
	cmd->header.size = sizeof(cmd->body);
	cmd->body.cid    = dx_query_ctx->id;

	vmw_cmd_commit(dev_priv, sizeof(*cmd));

	/* Triggers a rebind the next time affected context is bound */
	dx_query_mob->dx_query_ctx = NULL;

	return 0;
}



/**
 * vmw_query_move_notify - Read back cached query states
 *
 * @bo: The TTM buffer object about to move.
 * @old_mem: The memory region @bo is moving from.
 * @new_mem: The memory region @bo is moving to.
 *
 * Called before the query MOB is swapped out to read back cached query
 * states from the device.
 */
void vmw_query_move_notify(struct ttm_buffer_object *bo,
			   struct ttm_resource *old_mem,
			   struct ttm_resource *new_mem)
{
	struct vmw_bo *dx_query_mob;
	struct ttm_device *bdev = bo->bdev;
	struct vmw_private *dev_priv = vmw_priv_from_ttm(bdev);

	mutex_lock(&dev_priv->binding_mutex);

	/* If BO is being moved from MOB to system memory */
	if (old_mem &&
	    new_mem->mem_type == TTM_PL_SYSTEM &&
	    old_mem->mem_type == VMW_PL_MOB) {
		struct vmw_fence_obj *fence;

		dx_query_mob = to_vmw_bo(&bo->base);
		if (!dx_query_mob || !dx_query_mob->dx_query_ctx) {
			mutex_unlock(&dev_priv->binding_mutex);
			return;
		}

		(void) vmw_query_readback_all(dx_query_mob);
		mutex_unlock(&dev_priv->binding_mutex);

		/* Create a fence and attach the BO to it */
		(void) vmw_execbuf_fence_commands(NULL, dev_priv, &fence, NULL);
		vmw_bo_fence_single(bo, fence);

		if (fence != NULL)
			vmw_fence_obj_unreference(&fence);

		(void) ttm_bo_wait(bo, false, false);
	} else
		mutex_unlock(&dev_priv->binding_mutex);
}

/**
 * vmw_resource_needs_backup - Return whether a resource needs a backup buffer.
 *
 * @res:            The resource being queried.
 */
bool vmw_resource_needs_backup(const struct vmw_resource *res)
{
	return res->func->needs_guest_memory;
}

/**
 * vmw_resource_evict_type - Evict all resources of a specific type
 *
 * @dev_priv:       Pointer to a device private struct
 * @type:           The resource type to evict
 *
 * To avoid thrashing starvation or as part of the hibernation sequence,
 * try to evict all evictable resources of a specific type.
 */
static void vmw_resource_evict_type(struct vmw_private *dev_priv,
				    enum vmw_res_type type)
{
	struct list_head *lru_list = &dev_priv->res_lru[type];
	struct vmw_resource *evict_res;
	unsigned err_count = 0;
	int ret;
	struct ww_acquire_ctx ticket;

	do {
		spin_lock(&dev_priv->resource_lock);

		if (list_empty(lru_list))
			goto out_unlock;

		evict_res = vmw_resource_reference(
			list_first_entry(lru_list, struct vmw_resource,
					 lru_head));
		list_del_init(&evict_res->lru_head);
		spin_unlock(&dev_priv->resource_lock);

		/* Wait lock backup buffers with a ticket. */
		ret = vmw_resource_do_evict(&ticket, evict_res, false);
		if (unlikely(ret != 0)) {
			spin_lock(&dev_priv->resource_lock);
			list_add_tail(&evict_res->lru_head, lru_list);
			spin_unlock(&dev_priv->resource_lock);
			if (++err_count > VMW_RES_EVICT_ERR_COUNT) {
				vmw_resource_unreference(&evict_res);
				return;
			}
		}

		vmw_resource_unreference(&evict_res);
	} while (1);

out_unlock:
	spin_unlock(&dev_priv->resource_lock);
}

/**
 * vmw_resource_evict_all - Evict all evictable resources
 *
 * @dev_priv:       Pointer to a device private struct
 *
 * To avoid thrashing starvation or as part of the hibernation sequence,
 * evict all evictable resources. In particular this means that all
 * guest-backed resources that are registered with the device are
 * evicted and the OTable becomes clean.
 */
void vmw_resource_evict_all(struct vmw_private *dev_priv)
{
	enum vmw_res_type type;

	mutex_lock(&dev_priv->cmdbuf_mutex);

	for (type = 0; type < vmw_res_max; ++type)
		vmw_resource_evict_type(dev_priv, type);

	mutex_unlock(&dev_priv->cmdbuf_mutex);
}

/*
 * vmw_resource_pin - Add a pin reference on a resource
 *
 * @res: The resource to add a pin reference on
 *
 * This function adds a pin reference, and if needed validates the resource.
 * Having a pin reference means that the resource can never be evicted, and
 * its id will never change as long as there is a pin reference.
 * This function returns 0 on success and a negative error code on failure.
 */
int vmw_resource_pin(struct vmw_resource *res, bool interruptible)
{
	struct ttm_operation_ctx ctx = { interruptible, false };
	struct vmw_private *dev_priv = res->dev_priv;
	int ret;

	mutex_lock(&dev_priv->cmdbuf_mutex);
	ret = vmw_resource_reserve(res, interruptible, false);
	if (ret)
		goto out_no_reserve;

	if (res->pin_count == 0) {
		struct vmw_bo *vbo = NULL;

		if (res->guest_memory_bo) {
			vbo = res->guest_memory_bo;

			ret = ttm_bo_reserve(&vbo->tbo, interruptible, false, NULL);
			if (ret)
				goto out_no_validate;
			if (!vbo->tbo.pin_count) {
				vmw_bo_placement_set(vbo,
						     res->func->domain,
						     res->func->busy_domain);
				ret = ttm_bo_validate
					(&vbo->tbo,
					 &vbo->placement,
					 &ctx);
				if (ret) {
					ttm_bo_unreserve(&vbo->tbo);
					goto out_no_validate;
				}
			}

			/* Do we really need to pin the MOB as well? */
			vmw_bo_pin_reserved(vbo, true);
		}
		ret = vmw_resource_validate(res, interruptible, true);
		if (vbo)
			ttm_bo_unreserve(&vbo->tbo);
		if (ret)
			goto out_no_validate;
	}
	res->pin_count++;

out_no_validate:
	vmw_resource_unreserve(res, false, false, false, NULL, 0UL);
out_no_reserve:
	mutex_unlock(&dev_priv->cmdbuf_mutex);

	return ret;
}

/**
 * vmw_resource_unpin - Remove a pin reference from a resource
 *
 * @res: The resource to remove a pin reference from
 *
 * Having a pin reference means that the resource can never be evicted, and
 * its id will never change as long as there is a pin reference.
 */
void vmw_resource_unpin(struct vmw_resource *res)
{
	struct vmw_private *dev_priv = res->dev_priv;
	int ret;

	mutex_lock(&dev_priv->cmdbuf_mutex);

	ret = vmw_resource_reserve(res, false, true);
	WARN_ON(ret);

	WARN_ON(res->pin_count == 0);
	if (--res->pin_count == 0 && res->guest_memory_bo) {
		struct vmw_bo *vbo = res->guest_memory_bo;

		(void) ttm_bo_reserve(&vbo->tbo, false, false, NULL);
		vmw_bo_pin_reserved(vbo, false);
		ttm_bo_unreserve(&vbo->tbo);
	}

	vmw_resource_unreserve(res, false, false, false, NULL, 0UL);

	mutex_unlock(&dev_priv->cmdbuf_mutex);
}

/**
 * vmw_res_type - Return the resource type
 *
 * @res: Pointer to the resource
 */
enum vmw_res_type vmw_res_type(const struct vmw_resource *res)
{
	return res->func->res_type;
}

/**
 * vmw_resource_dirty_update - Update a resource's dirty tracker with a
 * sequential range of touched backing store memory.
 * @res: The resource.
 * @start: The first page touched.
 * @end: The last page touched + 1.
 */
void vmw_resource_dirty_update(struct vmw_resource *res, pgoff_t start,
			       pgoff_t end)
{
	if (res->dirty)
		res->func->dirty_range_add(res, start << PAGE_SHIFT,
					   end << PAGE_SHIFT);
}

/**
 * vmw_resources_clean - Clean resources intersecting a mob range
 * @vbo: The mob buffer object
 * @start: The mob page offset starting the range
 * @end: The mob page offset ending the range
 * @num_prefault: Returns how many pages including the first have been
 * cleaned and are ok to prefault
 */
int vmw_resources_clean(struct vmw_bo *vbo, pgoff_t start,
			pgoff_t end, pgoff_t *num_prefault)
{
	struct rb_node *cur = vbo->res_tree.rb_node;
	struct vmw_resource *found = NULL;
	unsigned long res_start = start << PAGE_SHIFT;
	unsigned long res_end = end << PAGE_SHIFT;
	unsigned long last_cleaned = 0;

	/*
	 * Find the resource with lowest backup_offset that intersects the
	 * range.
	 */
	while (cur) {
		struct vmw_resource *cur_res =
			container_of(cur, struct vmw_resource, mob_node);

		if (cur_res->guest_memory_offset >= res_end) {
			cur = cur->rb_left;
		} else if (cur_res->guest_memory_offset + cur_res->guest_memory_size <=
			   res_start) {
			cur = cur->rb_right;
		} else {
			found = cur_res;
			cur = cur->rb_left;
			/* Continue to look for resources with lower offsets */
		}
	}

	/*
	 * In order of increasing guest_memory_offset, clean dirty resources
	 * intersecting the range.
	 */
	while (found) {
		if (found->res_dirty) {
			int ret;

			if (!found->func->clean)
				return -EINVAL;

			ret = found->func->clean(found);
			if (ret)
				return ret;

			found->res_dirty = false;
		}
		last_cleaned = found->guest_memory_offset + found->guest_memory_size;
		cur = rb_next(&found->mob_node);
		if (!cur)
			break;

		found = container_of(cur, struct vmw_resource, mob_node);
		if (found->guest_memory_offset >= res_end)
			break;
	}

	/*
	 * Set number of pages allowed prefaulting and fence the buffer object
	 */
	*num_prefault = 1;
	if (last_cleaned > res_start) {
		struct ttm_buffer_object *bo = &vbo->tbo;

		*num_prefault = __KERNEL_DIV_ROUND_UP(last_cleaned - res_start,
						      PAGE_SIZE);
		vmw_bo_fence_single(bo, NULL);
	}

	return 0;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}
