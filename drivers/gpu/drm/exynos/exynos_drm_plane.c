<<<<<<< HEAD
/*
 * Copyright (C) 2011 Samsung Electronics Co.Ltd
 * Authors: Joonyoung Shim <jy0922.shim@samsung.com>
 *
 * This program is free software; you can redistribute  it and/or modify it
 * under  the terms of  the GNU General  Public License as published by the
 * Free Software Foundation;  either version 2 of the  License, or (at your
 * option) any later version.
 *
 */

#include "drmP.h"

#include "exynos_drm.h"
#include "exynos_drm_crtc.h"
#include "exynos_drm_drv.h"
#include "exynos_drm_encoder.h"

struct exynos_plane {
	struct drm_plane		base;
	struct exynos_drm_overlay	overlay;
	bool				enabled;
};

static const uint32_t formats[] = {
	DRM_FORMAT_XRGB8888,
	DRM_FORMAT_ARGB8888,
	DRM_FORMAT_NV12,
	DRM_FORMAT_NV12M,
	DRM_FORMAT_NV12MT,
};

static int
exynos_update_plane(struct drm_plane *plane, struct drm_crtc *crtc,
		     struct drm_framebuffer *fb, int crtc_x, int crtc_y,
		     unsigned int crtc_w, unsigned int crtc_h,
		     uint32_t src_x, uint32_t src_y,
		     uint32_t src_w, uint32_t src_h)
{
	struct exynos_plane *exynos_plane =
		container_of(plane, struct exynos_plane, base);
	struct exynos_drm_overlay *overlay = &exynos_plane->overlay;
	struct exynos_drm_crtc_pos pos;
	unsigned int x = src_x >> 16;
	unsigned int y = src_y >> 16;
	int ret;

	DRM_DEBUG_KMS("[%d] %s\n", __LINE__, __func__);

	memset(&pos, 0, sizeof(struct exynos_drm_crtc_pos));
	pos.crtc_x = crtc_x;
	pos.crtc_y = crtc_y;
	pos.crtc_w = crtc_w;
	pos.crtc_h = crtc_h;

	pos.fb_x = x;
	pos.fb_y = y;

	/* TODO: scale feature */
	ret = exynos_drm_overlay_update(overlay, fb, &crtc->mode, &pos);
	if (ret < 0)
		return ret;

	exynos_drm_fn_encoder(crtc, overlay,
			exynos_drm_encoder_crtc_mode_set);
	exynos_drm_fn_encoder(crtc, &overlay->zpos,
			exynos_drm_encoder_crtc_plane_commit);

	exynos_plane->enabled = true;

	return 0;
}

static int exynos_disable_plane(struct drm_plane *plane)
{
	struct exynos_plane *exynos_plane =
		container_of(plane, struct exynos_plane, base);
	struct exynos_drm_overlay *overlay = &exynos_plane->overlay;

	DRM_DEBUG_KMS("[%d] %s\n", __LINE__, __func__);

	if (!exynos_plane->enabled)
		return 0;

	exynos_drm_fn_encoder(plane->crtc, &overlay->zpos,
			exynos_drm_encoder_crtc_disable);

	exynos_plane->enabled = false;
	exynos_plane->overlay.zpos = DEFAULT_ZPOS;

	return 0;
}

static void exynos_plane_destroy(struct drm_plane *plane)
{
	struct exynos_plane *exynos_plane =
		container_of(plane, struct exynos_plane, base);

	DRM_DEBUG_KMS("[%d] %s\n", __LINE__, __func__);

	exynos_disable_plane(plane);
	drm_plane_cleanup(plane);
	kfree(exynos_plane);
}

static struct drm_plane_funcs exynos_plane_funcs = {
	.update_plane	= exynos_update_plane,
	.disable_plane	= exynos_disable_plane,
	.destroy	= exynos_plane_destroy,
};

int exynos_plane_init(struct drm_device *dev, unsigned int nr)
{
	struct exynos_plane *exynos_plane;
	uint32_t possible_crtcs;

	exynos_plane = kzalloc(sizeof(struct exynos_plane), GFP_KERNEL);
	if (!exynos_plane)
		return -ENOMEM;

	/* all CRTCs are available */
	possible_crtcs = (1 << MAX_CRTC) - 1;

	exynos_plane->overlay.zpos = DEFAULT_ZPOS;

	return drm_plane_init(dev, &exynos_plane->base, possible_crtcs,
			      &exynos_plane_funcs, formats, ARRAY_SIZE(formats),
			      false);
}

int exynos_plane_set_zpos_ioctl(struct drm_device *dev, void *data,
				struct drm_file *file_priv)
{
	struct drm_exynos_plane_set_zpos *zpos_req = data;
	struct drm_mode_object *obj;
	struct drm_plane *plane;
	struct exynos_plane *exynos_plane;
	int ret = 0;

	DRM_DEBUG_KMS("[%d] %s\n", __LINE__, __func__);

	if (!drm_core_check_feature(dev, DRIVER_MODESET))
		return -EINVAL;

	if (zpos_req->zpos < 0 || zpos_req->zpos >= MAX_PLANE) {
		if (zpos_req->zpos != DEFAULT_ZPOS) {
			DRM_ERROR("zpos not within limits\n");
			return -EINVAL;
		}
	}

	mutex_lock(&dev->mode_config.mutex);

	obj = drm_mode_object_find(dev, zpos_req->plane_id,
			DRM_MODE_OBJECT_PLANE);
	if (!obj) {
		DRM_DEBUG_KMS("Unknown plane ID %d\n",
			      zpos_req->plane_id);
		ret = -EINVAL;
		goto out;
	}

	plane = obj_to_plane(obj);
	exynos_plane = container_of(plane, struct exynos_plane, base);

	exynos_plane->overlay.zpos = zpos_req->zpos;

out:
	mutex_unlock(&dev->mode_config.mutex);
	return ret;
}
=======
// SPDX-License-Identifier: GPL-2.0-or-later
/*
 * Copyright (C) 2011 Samsung Electronics Co.Ltd
 * Authors: Joonyoung Shim <jy0922.shim@samsung.com>
 */


#include <drm/drm_atomic.h>
#include <drm/drm_atomic_helper.h>
#include <drm/drm_blend.h>
#include <drm/drm_framebuffer.h>
#include <drm/exynos_drm.h>

#include "exynos_drm_crtc.h"
#include "exynos_drm_drv.h"
#include "exynos_drm_fb.h"
#include "exynos_drm_gem.h"
#include "exynos_drm_plane.h"

/*
 * This function is to get X or Y size shown via screen. This needs length and
 * start position of CRTC.
 *
 *      <--- length --->
 * CRTC ----------------
 *      ^ start        ^ end
 *
 * There are six cases from a to f.
 *
 *             <----- SCREEN ----->
 *             0                 last
 *   ----------|------------------|----------
 * CRTCs
 * a -------
 *        b -------
 *        c --------------------------
 *                 d --------
 *                           e -------
 *                                  f -------
 */
static int exynos_plane_get_size(int start, unsigned length, unsigned last)
{
	int end = start + length;
	int size = 0;

	if (start <= 0) {
		if (end > 0)
			size = min_t(unsigned, end, last);
	} else if (start <= last) {
		size = min_t(unsigned, last - start, length);
	}

	return size;
}

static void exynos_plane_mode_set(struct exynos_drm_plane_state *exynos_state)
{
	struct drm_plane_state *state = &exynos_state->base;
	struct drm_crtc *crtc = state->crtc;
	struct drm_crtc_state *crtc_state =
			drm_atomic_get_existing_crtc_state(state->state, crtc);
	struct drm_display_mode *mode = &crtc_state->adjusted_mode;
	int crtc_x, crtc_y;
	unsigned int crtc_w, crtc_h;
	unsigned int src_x, src_y;
	unsigned int src_w, src_h;
	unsigned int actual_w;
	unsigned int actual_h;

	/*
	 * The original src/dest coordinates are stored in exynos_state->base,
	 * but we want to keep another copy internal to our driver that we can
	 * clip/modify ourselves.
	 */

	crtc_x = state->crtc_x;
	crtc_y = state->crtc_y;
	crtc_w = state->crtc_w;
	crtc_h = state->crtc_h;

	src_x = state->src_x >> 16;
	src_y = state->src_y >> 16;
	src_w = state->src_w >> 16;
	src_h = state->src_h >> 16;

	/* set ratio */
	exynos_state->h_ratio = (src_w << 16) / crtc_w;
	exynos_state->v_ratio = (src_h << 16) / crtc_h;

	/* clip to visible area */
	actual_w = exynos_plane_get_size(crtc_x, crtc_w, mode->hdisplay);
	actual_h = exynos_plane_get_size(crtc_y, crtc_h, mode->vdisplay);

	if (crtc_x < 0) {
		if (actual_w)
			src_x += ((-crtc_x) * exynos_state->h_ratio) >> 16;
		crtc_x = 0;
	}

	if (crtc_y < 0) {
		if (actual_h)
			src_y += ((-crtc_y) * exynos_state->v_ratio) >> 16;
		crtc_y = 0;
	}

	/* set drm framebuffer data. */
	exynos_state->src.x = src_x;
	exynos_state->src.y = src_y;
	exynos_state->src.w = (actual_w * exynos_state->h_ratio) >> 16;
	exynos_state->src.h = (actual_h * exynos_state->v_ratio) >> 16;

	/* set plane range to be displayed. */
	exynos_state->crtc.x = crtc_x;
	exynos_state->crtc.y = crtc_y;
	exynos_state->crtc.w = actual_w;
	exynos_state->crtc.h = actual_h;

	DRM_DEV_DEBUG_KMS(crtc->dev->dev,
			  "plane : offset_x/y(%d,%d), width/height(%d,%d)",
			  exynos_state->crtc.x, exynos_state->crtc.y,
			  exynos_state->crtc.w, exynos_state->crtc.h);
}

static void exynos_drm_plane_reset(struct drm_plane *plane)
{
	struct exynos_drm_plane *exynos_plane = to_exynos_plane(plane);
	struct exynos_drm_plane_state *exynos_state;

	if (plane->state) {
		exynos_state = to_exynos_plane_state(plane->state);
		__drm_atomic_helper_plane_destroy_state(plane->state);
		kfree(exynos_state);
		plane->state = NULL;
	}

	exynos_state = kzalloc(sizeof(*exynos_state), GFP_KERNEL);
	if (exynos_state) {
		__drm_atomic_helper_plane_reset(plane, &exynos_state->base);
		plane->state->zpos = exynos_plane->config->zpos;
	}
}

static struct drm_plane_state *
exynos_drm_plane_duplicate_state(struct drm_plane *plane)
{
	struct exynos_drm_plane_state *exynos_state;
	struct exynos_drm_plane_state *copy;

	exynos_state = to_exynos_plane_state(plane->state);
	copy = kzalloc(sizeof(*exynos_state), GFP_KERNEL);
	if (!copy)
		return NULL;

	__drm_atomic_helper_plane_duplicate_state(plane, &copy->base);
	return &copy->base;
}

static void exynos_drm_plane_destroy_state(struct drm_plane *plane,
					   struct drm_plane_state *old_state)
{
	struct exynos_drm_plane_state *old_exynos_state =
					to_exynos_plane_state(old_state);
	__drm_atomic_helper_plane_destroy_state(old_state);
	kfree(old_exynos_state);
}

static struct drm_plane_funcs exynos_plane_funcs = {
	.update_plane	= drm_atomic_helper_update_plane,
	.disable_plane	= drm_atomic_helper_disable_plane,
	.destroy	= drm_plane_cleanup,
	.reset		= exynos_drm_plane_reset,
	.atomic_duplicate_state = exynos_drm_plane_duplicate_state,
	.atomic_destroy_state = exynos_drm_plane_destroy_state,
};

static int
exynos_drm_plane_check_format(const struct exynos_drm_plane_config *config,
			      struct exynos_drm_plane_state *state)
{
	struct drm_framebuffer *fb = state->base.fb;
	struct drm_device *dev = fb->dev;

	switch (fb->modifier) {
	case DRM_FORMAT_MOD_SAMSUNG_64_32_TILE:
		if (!(config->capabilities & EXYNOS_DRM_PLANE_CAP_TILE))
			return -ENOTSUPP;
		break;

	case DRM_FORMAT_MOD_LINEAR:
		break;

	default:
		DRM_DEV_ERROR(dev->dev, "unsupported pixel format modifier");
		return -ENOTSUPP;
	}

	return 0;
}

static int
exynos_drm_plane_check_size(const struct exynos_drm_plane_config *config,
			    struct exynos_drm_plane_state *state)
{
	struct drm_crtc *crtc = state->base.crtc;
	bool width_ok = false, height_ok = false;

	if (config->capabilities & EXYNOS_DRM_PLANE_CAP_SCALE)
		return 0;

	if (state->src.w == state->crtc.w)
		width_ok = true;

	if (state->src.h == state->crtc.h)
		height_ok = true;

	if ((config->capabilities & EXYNOS_DRM_PLANE_CAP_DOUBLE) &&
	    state->h_ratio == (1 << 15))
		width_ok = true;

	if ((config->capabilities & EXYNOS_DRM_PLANE_CAP_DOUBLE) &&
	    state->v_ratio == (1 << 15))
		height_ok = true;

	if (width_ok && height_ok)
		return 0;

	DRM_DEV_DEBUG_KMS(crtc->dev->dev, "scaling mode is not supported");
	return -ENOTSUPP;
}

static int exynos_plane_atomic_check(struct drm_plane *plane,
				     struct drm_atomic_state *state)
{
	struct drm_plane_state *new_plane_state = drm_atomic_get_new_plane_state(state,
										 plane);
	struct exynos_drm_plane *exynos_plane = to_exynos_plane(plane);
	struct exynos_drm_plane_state *exynos_state =
						to_exynos_plane_state(new_plane_state);
	int ret = 0;

	if (!new_plane_state->crtc || !new_plane_state->fb)
		return 0;

	/* translate state into exynos_state */
	exynos_plane_mode_set(exynos_state);

	ret = exynos_drm_plane_check_format(exynos_plane->config, exynos_state);
	if (ret)
		return ret;

	ret = exynos_drm_plane_check_size(exynos_plane->config, exynos_state);
	return ret;
}

static void exynos_plane_atomic_update(struct drm_plane *plane,
				       struct drm_atomic_state *state)
{
	struct drm_plane_state *new_state = drm_atomic_get_new_plane_state(state,
								           plane);
	struct exynos_drm_crtc *exynos_crtc = to_exynos_crtc(new_state->crtc);
	struct exynos_drm_plane *exynos_plane = to_exynos_plane(plane);

	if (!new_state->crtc)
		return;

	if (exynos_crtc->ops->update_plane)
		exynos_crtc->ops->update_plane(exynos_crtc, exynos_plane);
}

static void exynos_plane_atomic_disable(struct drm_plane *plane,
					struct drm_atomic_state *state)
{
	struct drm_plane_state *old_state = drm_atomic_get_old_plane_state(state, plane);
	struct exynos_drm_plane *exynos_plane = to_exynos_plane(plane);
	struct exynos_drm_crtc *exynos_crtc = to_exynos_crtc(old_state->crtc);

	if (!old_state->crtc)
		return;

	if (exynos_crtc->ops->disable_plane)
		exynos_crtc->ops->disable_plane(exynos_crtc, exynos_plane);
}

static const struct drm_plane_helper_funcs plane_helper_funcs = {
	.atomic_check = exynos_plane_atomic_check,
	.atomic_update = exynos_plane_atomic_update,
	.atomic_disable = exynos_plane_atomic_disable,
};

static void exynos_plane_attach_zpos_property(struct drm_plane *plane,
					      int zpos, bool immutable)
{
	if (immutable)
		drm_plane_create_zpos_immutable_property(plane, zpos);
	else
		drm_plane_create_zpos_property(plane, zpos, 0, MAX_PLANE - 1);
}

int exynos_plane_init(struct drm_device *dev,
		      struct exynos_drm_plane *exynos_plane, unsigned int index,
		      const struct exynos_drm_plane_config *config)
{
	int err;
	unsigned int supported_modes = BIT(DRM_MODE_BLEND_PIXEL_NONE) |
				       BIT(DRM_MODE_BLEND_PREMULTI) |
				       BIT(DRM_MODE_BLEND_COVERAGE);
	struct drm_plane *plane = &exynos_plane->base;

	err = drm_universal_plane_init(dev, &exynos_plane->base,
				       1 << dev->mode_config.num_crtc,
				       &exynos_plane_funcs,
				       config->pixel_formats,
				       config->num_pixel_formats,
				       NULL, config->type, NULL);
	if (err) {
		DRM_DEV_ERROR(dev->dev, "failed to initialize plane\n");
		return err;
	}

	drm_plane_helper_add(&exynos_plane->base, &plane_helper_funcs);

	exynos_plane->index = index;
	exynos_plane->config = config;

	exynos_plane_attach_zpos_property(&exynos_plane->base, config->zpos,
			   !(config->capabilities & EXYNOS_DRM_PLANE_CAP_ZPOS));

	if (config->capabilities & EXYNOS_DRM_PLANE_CAP_PIX_BLEND)
		drm_plane_create_blend_mode_property(plane, supported_modes);

	if (config->capabilities & EXYNOS_DRM_PLANE_CAP_WIN_BLEND)
		drm_plane_create_alpha_property(plane);

	return 0;
}
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
