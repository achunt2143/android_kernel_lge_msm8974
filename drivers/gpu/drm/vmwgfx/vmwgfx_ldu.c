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
#include "vmwgfx_kms.h"

=======
#include "vmwgfx_bo.h"
#include "vmwgfx_kms.h"

#include <drm/drm_atomic.h>
#include <drm/drm_atomic_helper.h>
#include <drm/drm_fourcc.h>

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

#define vmw_crtc_to_ldu(x) \
	container_of(x, struct vmw_legacy_display_unit, base.crtc)
#define vmw_encoder_to_ldu(x) \
	container_of(x, struct vmw_legacy_display_unit, base.encoder)
#define vmw_connector_to_ldu(x) \
	container_of(x, struct vmw_legacy_display_unit, base.connector)

struct vmw_legacy_display {
	struct list_head active;

	unsigned num_active;
	unsigned last_num_active;

	struct vmw_framebuffer *fb;
};

<<<<<<< HEAD
/**
=======
/*
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 * Display unit using the legacy register interface.
 */
struct vmw_legacy_display_unit {
	struct vmw_display_unit base;

	struct list_head active;
};

static void vmw_ldu_destroy(struct vmw_legacy_display_unit *ldu)
{
	list_del_init(&ldu->active);
<<<<<<< HEAD
	vmw_display_unit_cleanup(&ldu->base);
=======
	vmw_du_cleanup(&ldu->base);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	kfree(ldu);
}


/*
 * Legacy Display Unit CRTC functions
 */

static void vmw_ldu_crtc_destroy(struct drm_crtc *crtc)
{
	vmw_ldu_destroy(vmw_crtc_to_ldu(crtc));
}

static int vmw_ldu_commit_list(struct vmw_private *dev_priv)
{
	struct vmw_legacy_display *lds = dev_priv->ldu_priv;
	struct vmw_legacy_display_unit *entry;
<<<<<<< HEAD
	struct vmw_display_unit *du = NULL;
	struct drm_framebuffer *fb = NULL;
	struct drm_crtc *crtc = NULL;
	int i = 0, ret;
=======
	struct drm_framebuffer *fb = NULL;
	struct drm_crtc *crtc = NULL;
	int i;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	/* If there is no display topology the host just assumes
	 * that the guest will set the same layout as the host.
	 */
	if (!(dev_priv->capabilities & SVGA_CAP_DISPLAY_TOPOLOGY)) {
		int w = 0, h = 0;
		list_for_each_entry(entry, &lds->active, active) {
			crtc = &entry->base.crtc;
			w = max(w, crtc->x + crtc->mode.hdisplay);
			h = max(h, crtc->y + crtc->mode.vdisplay);
<<<<<<< HEAD
			i++;
=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		}

		if (crtc == NULL)
			return 0;
<<<<<<< HEAD
		fb = entry->base.crtc.fb;

		return vmw_kms_write_svga(dev_priv, w, h, fb->pitches[0],
					  fb->bits_per_pixel, fb->depth);
=======
		fb = crtc->primary->state->fb;

		return vmw_kms_write_svga(dev_priv, w, h, fb->pitches[0],
					  fb->format->cpp[0] * 8,
					  fb->format->depth);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	}

	if (!list_empty(&lds->active)) {
		entry = list_entry(lds->active.next, typeof(*entry), active);
<<<<<<< HEAD
		fb = entry->base.crtc.fb;

		vmw_kms_write_svga(dev_priv, fb->width, fb->height, fb->pitches[0],
				   fb->bits_per_pixel, fb->depth);
=======
		fb = entry->base.crtc.primary->state->fb;

		vmw_kms_write_svga(dev_priv, fb->width, fb->height, fb->pitches[0],
				   fb->format->cpp[0] * 8, fb->format->depth);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	}

	/* Make sure we always show something. */
	vmw_write(dev_priv, SVGA_REG_NUM_GUEST_DISPLAYS,
		  lds->num_active ? lds->num_active : 1);

	i = 0;
	list_for_each_entry(entry, &lds->active, active) {
		crtc = &entry->base.crtc;

		vmw_write(dev_priv, SVGA_REG_DISPLAY_ID, i);
		vmw_write(dev_priv, SVGA_REG_DISPLAY_IS_PRIMARY, !i);
		vmw_write(dev_priv, SVGA_REG_DISPLAY_POSITION_X, crtc->x);
		vmw_write(dev_priv, SVGA_REG_DISPLAY_POSITION_Y, crtc->y);
		vmw_write(dev_priv, SVGA_REG_DISPLAY_WIDTH, crtc->mode.hdisplay);
		vmw_write(dev_priv, SVGA_REG_DISPLAY_HEIGHT, crtc->mode.vdisplay);
<<<<<<< HEAD
		vmw_write(dev_priv, SVGA_REG_DISPLAY_ID, SVGA_ID_INVALID);
=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

		i++;
	}

	BUG_ON(i != lds->num_active);

	lds->last_num_active = lds->num_active;

<<<<<<< HEAD

	/* Find the first du with a cursor. */
	list_for_each_entry(entry, &lds->active, active) {
		du = &entry->base;

		if (!du->cursor_dmabuf)
			continue;

		ret = vmw_cursor_update_dmabuf(dev_priv,
					       du->cursor_dmabuf,
					       64, 64,
					       du->hotspot_x,
					       du->hotspot_y);
		if (ret == 0)
			break;

		DRM_ERROR("Could not update cursor image\n");
	}

	return 0;
}

=======
	return 0;
}

/*
 * Pin the buffer in a location suitable for access by the
 * display system.
 */
static int vmw_ldu_fb_pin(struct vmw_framebuffer *vfb)
{
	struct vmw_private *dev_priv = vmw_priv(vfb->base.dev);
	struct vmw_bo *buf;
	int ret;

	buf = vfb->bo ?  vmw_framebuffer_to_vfbd(&vfb->base)->buffer :
		vmw_framebuffer_to_vfbs(&vfb->base)->surface->res.guest_memory_bo;

	if (!buf)
		return 0;
	WARN_ON(dev_priv->active_display_unit != vmw_du_legacy);

	if (dev_priv->active_display_unit == vmw_du_legacy) {
		vmw_overlay_pause_all(dev_priv);
		ret = vmw_bo_pin_in_start_of_vram(dev_priv, buf, false);
		vmw_overlay_resume_all(dev_priv);
	} else
		ret = -EINVAL;

	return ret;
}

static int vmw_ldu_fb_unpin(struct vmw_framebuffer *vfb)
{
	struct vmw_private *dev_priv = vmw_priv(vfb->base.dev);
	struct vmw_bo *buf;

	buf = vfb->bo ?  vmw_framebuffer_to_vfbd(&vfb->base)->buffer :
		vmw_framebuffer_to_vfbs(&vfb->base)->surface->res.guest_memory_bo;

	if (WARN_ON(!buf))
		return 0;

	return vmw_bo_unpin(dev_priv, buf, false);
}

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
static int vmw_ldu_del_active(struct vmw_private *vmw_priv,
			      struct vmw_legacy_display_unit *ldu)
{
	struct vmw_legacy_display *ld = vmw_priv->ldu_priv;
	if (list_empty(&ldu->active))
		return 0;

	/* Must init otherwise list_empty(&ldu->active) will not work. */
	list_del_init(&ldu->active);
	if (--(ld->num_active) == 0) {
		BUG_ON(!ld->fb);
<<<<<<< HEAD
		if (ld->fb->unpin)
			ld->fb->unpin(ld->fb);
=======
		WARN_ON(vmw_ldu_fb_unpin(ld->fb));
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		ld->fb = NULL;
	}

	return 0;
}

static int vmw_ldu_add_active(struct vmw_private *vmw_priv,
			      struct vmw_legacy_display_unit *ldu,
			      struct vmw_framebuffer *vfb)
{
	struct vmw_legacy_display *ld = vmw_priv->ldu_priv;
	struct vmw_legacy_display_unit *entry;
	struct list_head *at;

	BUG_ON(!ld->num_active && ld->fb);
	if (vfb != ld->fb) {
<<<<<<< HEAD
		if (ld->fb && ld->fb->unpin)
			ld->fb->unpin(ld->fb);
		if (vfb->pin)
			vfb->pin(vfb);
=======
		if (ld->fb)
			WARN_ON(vmw_ldu_fb_unpin(ld->fb));
		vmw_svga_enable(vmw_priv);
		WARN_ON(vmw_ldu_fb_pin(vfb));
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		ld->fb = vfb;
	}

	if (!list_empty(&ldu->active))
		return 0;

	at = &ld->active;
	list_for_each_entry(entry, &ld->active, active) {
		if (entry->base.unit > ldu->base.unit)
			break;

		at = &entry->active;
	}

	list_add(&ldu->active, at);

	ld->num_active++;

	return 0;
}

<<<<<<< HEAD
static int vmw_ldu_crtc_set_config(struct drm_mode_set *set)
{
	struct vmw_private *dev_priv;
	struct vmw_legacy_display_unit *ldu;
	struct drm_connector *connector;
	struct drm_display_mode *mode;
	struct drm_encoder *encoder;
	struct vmw_framebuffer *vfb;
	struct drm_framebuffer *fb;
	struct drm_crtc *crtc;

	if (!set)
		return -EINVAL;

	if (!set->crtc)
		return -EINVAL;

	/* get the ldu */
	crtc = set->crtc;
	ldu = vmw_crtc_to_ldu(crtc);
	vfb = set->fb ? vmw_framebuffer_to_vfb(set->fb) : NULL;
	dev_priv = vmw_priv(crtc->dev);

	if (set->num_connectors > 1) {
		DRM_ERROR("to many connectors\n");
		return -EINVAL;
	}

	if (set->num_connectors == 1 &&
	    set->connectors[0] != &ldu->base.connector) {
		DRM_ERROR("connector doesn't match %p %p\n",
			set->connectors[0], &ldu->base.connector);
		return -EINVAL;
	}

	/* ldu only supports one fb active at the time */
	if (dev_priv->ldu_priv->fb && vfb &&
	    !(dev_priv->ldu_priv->num_active == 1 &&
	      !list_empty(&ldu->active)) &&
	    dev_priv->ldu_priv->fb != vfb) {
		DRM_ERROR("Multiple framebuffers not supported\n");
		return -EINVAL;
	}

	/* since they always map one to one these are safe */
	connector = &ldu->base.connector;
	encoder = &ldu->base.encoder;

	/* should we turn the crtc off? */
	if (set->num_connectors == 0 || !set->mode || !set->fb) {

		connector->encoder = NULL;
		encoder->crtc = NULL;
		crtc->fb = NULL;

		vmw_ldu_del_active(dev_priv, ldu);

		return vmw_ldu_commit_list(dev_priv);
	}


	/* we now know we want to set a mode */
	mode = set->mode;
	fb = set->fb;

	if (set->x + mode->hdisplay > fb->width ||
	    set->y + mode->vdisplay > fb->height) {
		DRM_ERROR("set outside of framebuffer\n");
		return -EINVAL;
	}

	vmw_fb_off(dev_priv);

	crtc->fb = fb;
	encoder->crtc = crtc;
	connector->encoder = encoder;
	crtc->x = set->x;
	crtc->y = set->y;
	crtc->mode = *mode;

	vmw_ldu_add_active(dev_priv, ldu, vfb);

	return vmw_ldu_commit_list(dev_priv);
}

static struct drm_crtc_funcs vmw_legacy_crtc_funcs = {
	.save = vmw_du_crtc_save,
	.restore = vmw_du_crtc_restore,
	.cursor_set = vmw_du_crtc_cursor_set,
	.cursor_move = vmw_du_crtc_cursor_move,
	.gamma_set = vmw_du_crtc_gamma_set,
	.destroy = vmw_ldu_crtc_destroy,
	.set_config = vmw_ldu_crtc_set_config,
=======
/**
 * vmw_ldu_crtc_mode_set_nofb - Enable svga
 *
 * @crtc: CRTC associated with the new screen
 *
 * For LDU, just enable the svga
 */
static void vmw_ldu_crtc_mode_set_nofb(struct drm_crtc *crtc)
{
}

/**
 * vmw_ldu_crtc_atomic_enable - Noop
 *
 * @crtc: CRTC associated with the new screen
 * @state: Unused
 *
 * This is called after a mode set has been completed.  Here's
 * usually a good place to call vmw_ldu_add_active/vmw_ldu_del_active
 * but since for LDU the display plane is closely tied to the
 * CRTC, it makes more sense to do those at plane update time.
 */
static void vmw_ldu_crtc_atomic_enable(struct drm_crtc *crtc,
				       struct drm_atomic_state *state)
{
}

/**
 * vmw_ldu_crtc_atomic_disable - Turns off CRTC
 *
 * @crtc: CRTC to be turned off
 * @state: Unused
 */
static void vmw_ldu_crtc_atomic_disable(struct drm_crtc *crtc,
					struct drm_atomic_state *state)
{
}

static const struct drm_crtc_funcs vmw_legacy_crtc_funcs = {
	.gamma_set = vmw_du_crtc_gamma_set,
	.destroy = vmw_ldu_crtc_destroy,
	.reset = vmw_du_crtc_reset,
	.atomic_duplicate_state = vmw_du_crtc_duplicate_state,
	.atomic_destroy_state = vmw_du_crtc_destroy_state,
	.set_config = drm_atomic_helper_set_config,
	.page_flip = drm_atomic_helper_page_flip,
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
};


/*
 * Legacy Display Unit encoder functions
 */

static void vmw_ldu_encoder_destroy(struct drm_encoder *encoder)
{
	vmw_ldu_destroy(vmw_encoder_to_ldu(encoder));
}

<<<<<<< HEAD
static struct drm_encoder_funcs vmw_legacy_encoder_funcs = {
=======
static const struct drm_encoder_funcs vmw_legacy_encoder_funcs = {
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	.destroy = vmw_ldu_encoder_destroy,
};

/*
 * Legacy Display Unit connector functions
 */

static void vmw_ldu_connector_destroy(struct drm_connector *connector)
{
	vmw_ldu_destroy(vmw_connector_to_ldu(connector));
}

<<<<<<< HEAD
static struct drm_connector_funcs vmw_legacy_connector_funcs = {
	.dpms = vmw_du_connector_dpms,
	.save = vmw_du_connector_save,
	.restore = vmw_du_connector_restore,
	.detect = vmw_du_connector_detect,
	.fill_modes = vmw_du_connector_fill_modes,
	.set_property = vmw_du_connector_set_property,
	.destroy = vmw_ldu_connector_destroy,
};

static int vmw_ldu_init(struct vmw_private *dev_priv, unsigned unit)
{
	struct vmw_legacy_display_unit *ldu;
	struct drm_device *dev = dev_priv->dev;
	struct drm_connector *connector;
	struct drm_encoder *encoder;
	struct drm_crtc *crtc;
=======
static const struct drm_connector_funcs vmw_legacy_connector_funcs = {
	.dpms = vmw_du_connector_dpms,
	.detect = vmw_du_connector_detect,
	.fill_modes = drm_helper_probe_single_connector_modes,
	.destroy = vmw_ldu_connector_destroy,
	.reset = vmw_du_connector_reset,
	.atomic_duplicate_state = vmw_du_connector_duplicate_state,
	.atomic_destroy_state = vmw_du_connector_destroy_state,
};

static const struct
drm_connector_helper_funcs vmw_ldu_connector_helper_funcs = {
	.get_modes = vmw_connector_get_modes,
	.mode_valid = vmw_connector_mode_valid
};

static int vmw_kms_ldu_do_bo_dirty(struct vmw_private *dev_priv,
				   struct vmw_framebuffer *framebuffer,
				   unsigned int flags, unsigned int color,
				   struct drm_mode_rect *clips,
				   unsigned int num_clips);

/*
 * Legacy Display Plane Functions
 */

static void
vmw_ldu_primary_plane_atomic_update(struct drm_plane *plane,
				    struct drm_atomic_state *state)
{
	struct drm_plane_state *old_state = drm_atomic_get_old_plane_state(state,
									   plane);
	struct drm_plane_state *new_state = drm_atomic_get_new_plane_state(state,
									   plane);
	struct vmw_private *dev_priv;
	struct vmw_legacy_display_unit *ldu;
	struct vmw_framebuffer *vfb;
	struct drm_framebuffer *fb;
	struct drm_crtc *crtc = new_state->crtc ?: old_state->crtc;

	ldu = vmw_crtc_to_ldu(crtc);
	dev_priv = vmw_priv(plane->dev);
	fb       = new_state->fb;

	vfb = (fb) ? vmw_framebuffer_to_vfb(fb) : NULL;

	if (vfb)
		vmw_ldu_add_active(dev_priv, ldu, vfb);
	else
		vmw_ldu_del_active(dev_priv, ldu);

	vmw_ldu_commit_list(dev_priv);

	if (vfb && vmw_cmd_supported(dev_priv)) {
		struct drm_mode_rect fb_rect = {
			.x1 = 0,
			.y1 = 0,
			.x2 = vfb->base.width,
			.y2 = vfb->base.height
		};
		struct drm_mode_rect *damage_rects = drm_plane_get_damage_clips(new_state);
		u32 rect_count = drm_plane_get_damage_clips_count(new_state);
		int ret;

		if (!damage_rects) {
			damage_rects = &fb_rect;
			rect_count = 1;
		}

		ret = vmw_kms_ldu_do_bo_dirty(dev_priv, vfb, 0, 0, damage_rects, rect_count);

		drm_WARN_ONCE(plane->dev, ret,
			"vmw_kms_ldu_do_bo_dirty failed with: ret=%d\n", ret);

		vmw_cmd_flush(dev_priv, false);
	}
}

static const struct drm_plane_funcs vmw_ldu_plane_funcs = {
	.update_plane = drm_atomic_helper_update_plane,
	.disable_plane = drm_atomic_helper_disable_plane,
	.destroy = vmw_du_primary_plane_destroy,
	.reset = vmw_du_plane_reset,
	.atomic_duplicate_state = vmw_du_plane_duplicate_state,
	.atomic_destroy_state = vmw_du_plane_destroy_state,
};

static const struct drm_plane_funcs vmw_ldu_cursor_funcs = {
	.update_plane = drm_atomic_helper_update_plane,
	.disable_plane = drm_atomic_helper_disable_plane,
	.destroy = vmw_du_cursor_plane_destroy,
	.reset = vmw_du_plane_reset,
	.atomic_duplicate_state = vmw_du_plane_duplicate_state,
	.atomic_destroy_state = vmw_du_plane_destroy_state,
};

/*
 * Atomic Helpers
 */
static const struct
drm_plane_helper_funcs vmw_ldu_cursor_plane_helper_funcs = {
	.atomic_check = vmw_du_cursor_plane_atomic_check,
	.atomic_update = vmw_du_cursor_plane_atomic_update,
	.prepare_fb = vmw_du_cursor_plane_prepare_fb,
	.cleanup_fb = vmw_du_cursor_plane_cleanup_fb,
};

static const struct
drm_plane_helper_funcs vmw_ldu_primary_plane_helper_funcs = {
	.atomic_check = vmw_du_primary_plane_atomic_check,
	.atomic_update = vmw_ldu_primary_plane_atomic_update,
};

static const struct drm_crtc_helper_funcs vmw_ldu_crtc_helper_funcs = {
	.mode_set_nofb = vmw_ldu_crtc_mode_set_nofb,
	.atomic_check = vmw_du_crtc_atomic_check,
	.atomic_begin = vmw_du_crtc_atomic_begin,
	.atomic_flush = vmw_du_crtc_atomic_flush,
	.atomic_enable = vmw_ldu_crtc_atomic_enable,
	.atomic_disable = vmw_ldu_crtc_atomic_disable,
};


static int vmw_ldu_init(struct vmw_private *dev_priv, unsigned unit)
{
	struct vmw_legacy_display_unit *ldu;
	struct drm_device *dev = &dev_priv->drm;
	struct drm_connector *connector;
	struct drm_encoder *encoder;
	struct drm_plane *primary;
	struct vmw_cursor_plane *cursor;
	struct drm_crtc *crtc;
	int ret;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	ldu = kzalloc(sizeof(*ldu), GFP_KERNEL);
	if (!ldu)
		return -ENOMEM;

	ldu->base.unit = unit;
	crtc = &ldu->base.crtc;
	encoder = &ldu->base.encoder;
	connector = &ldu->base.connector;
<<<<<<< HEAD
=======
	primary = &ldu->base.primary;
	cursor = &ldu->base.cursor;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	INIT_LIST_HEAD(&ldu->active);

	ldu->base.pref_active = (unit == 0);
	ldu->base.pref_width = dev_priv->initial_width;
	ldu->base.pref_height = dev_priv->initial_height;
<<<<<<< HEAD
	ldu->base.pref_mode = NULL;
	ldu->base.is_implicit = true;

	drm_connector_init(dev, connector, &vmw_legacy_connector_funcs,
			   DRM_MODE_CONNECTOR_VIRTUAL);
	connector->status = vmw_du_connector_detect(connector, true);

	drm_encoder_init(dev, encoder, &vmw_legacy_encoder_funcs,
			 DRM_MODE_ENCODER_VIRTUAL);
	drm_mode_connector_attach_encoder(connector, encoder);
	encoder->possible_crtcs = (1 << unit);
	encoder->possible_clones = 0;

	drm_crtc_init(dev, crtc, &vmw_legacy_crtc_funcs);

	drm_mode_crtc_set_gamma_size(crtc, 256);

	drm_connector_attach_property(connector,
				      dev->mode_config.dirty_info_property,
				      1);

	return 0;
}

int vmw_kms_init_legacy_display_system(struct vmw_private *dev_priv)
{
	struct drm_device *dev = dev_priv->dev;
	int i, ret;

	if (dev_priv->ldu_priv) {
		DRM_INFO("ldu system already on\n");
=======

	/*
	 * Remove this after enabling atomic because property values can
	 * only exist in a state object
	 */
	ldu->base.is_implicit = true;

	/* Initialize primary plane */
	ret = drm_universal_plane_init(dev, primary,
				       0, &vmw_ldu_plane_funcs,
				       vmw_primary_plane_formats,
				       ARRAY_SIZE(vmw_primary_plane_formats),
				       NULL, DRM_PLANE_TYPE_PRIMARY, NULL);
	if (ret) {
		DRM_ERROR("Failed to initialize primary plane");
		goto err_free;
	}

	drm_plane_helper_add(primary, &vmw_ldu_primary_plane_helper_funcs);

	/*
	 * We're going to be using traces and software cursors
	 */
	if (vmw_cmd_supported(dev_priv)) {
		/* Initialize cursor plane */
		ret = drm_universal_plane_init(dev, &cursor->base,
					       0, &vmw_ldu_cursor_funcs,
					       vmw_cursor_plane_formats,
					       ARRAY_SIZE(vmw_cursor_plane_formats),
					       NULL, DRM_PLANE_TYPE_CURSOR, NULL);
		if (ret) {
			DRM_ERROR("Failed to initialize cursor plane");
			drm_plane_cleanup(&ldu->base.primary);
			goto err_free;
		}

		drm_plane_helper_add(&cursor->base, &vmw_ldu_cursor_plane_helper_funcs);
	}

	ret = drm_connector_init(dev, connector, &vmw_legacy_connector_funcs,
				 DRM_MODE_CONNECTOR_VIRTUAL);
	if (ret) {
		DRM_ERROR("Failed to initialize connector\n");
		goto err_free;
	}

	drm_connector_helper_add(connector, &vmw_ldu_connector_helper_funcs);
	connector->status = vmw_du_connector_detect(connector, true);

	ret = drm_encoder_init(dev, encoder, &vmw_legacy_encoder_funcs,
			       DRM_MODE_ENCODER_VIRTUAL, NULL);
	if (ret) {
		DRM_ERROR("Failed to initialize encoder\n");
		goto err_free_connector;
	}

	(void) drm_connector_attach_encoder(connector, encoder);
	encoder->possible_crtcs = (1 << unit);
	encoder->possible_clones = 0;

	ret = drm_connector_register(connector);
	if (ret) {
		DRM_ERROR("Failed to register connector\n");
		goto err_free_encoder;
	}

	ret = drm_crtc_init_with_planes(dev, crtc, primary,
		      vmw_cmd_supported(dev_priv) ? &cursor->base : NULL,
		      &vmw_legacy_crtc_funcs, NULL);
	if (ret) {
		DRM_ERROR("Failed to initialize CRTC\n");
		goto err_free_unregister;
	}

	drm_crtc_helper_add(crtc, &vmw_ldu_crtc_helper_funcs);

	drm_mode_crtc_set_gamma_size(crtc, 256);

	drm_object_attach_property(&connector->base,
				   dev_priv->hotplug_mode_update_property, 1);
	drm_object_attach_property(&connector->base,
				   dev->mode_config.suggested_x_property, 0);
	drm_object_attach_property(&connector->base,
				   dev->mode_config.suggested_y_property, 0);
	if (dev_priv->implicit_placement_property)
		drm_object_attach_property
			(&connector->base,
			 dev_priv->implicit_placement_property,
			 1);

	return 0;

err_free_unregister:
	drm_connector_unregister(connector);
err_free_encoder:
	drm_encoder_cleanup(encoder);
err_free_connector:
	drm_connector_cleanup(connector);
err_free:
	kfree(ldu);
	return ret;
}

int vmw_kms_ldu_init_display(struct vmw_private *dev_priv)
{
	struct drm_device *dev = &dev_priv->drm;
	int i, ret;
	int num_display_units = (dev_priv->capabilities & SVGA_CAP_MULTIMON) ?
					VMWGFX_NUM_DISPLAY_UNITS : 1;

	if (unlikely(dev_priv->ldu_priv)) {
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		return -EINVAL;
	}

	dev_priv->ldu_priv = kmalloc(sizeof(*dev_priv->ldu_priv), GFP_KERNEL);
	if (!dev_priv->ldu_priv)
		return -ENOMEM;

	INIT_LIST_HEAD(&dev_priv->ldu_priv->active);
	dev_priv->ldu_priv->num_active = 0;
	dev_priv->ldu_priv->last_num_active = 0;
	dev_priv->ldu_priv->fb = NULL;

<<<<<<< HEAD
	/* for old hardware without multimon only enable one display */
	if (dev_priv->capabilities & SVGA_CAP_MULTIMON)
		ret = drm_vblank_init(dev, VMWGFX_NUM_DISPLAY_UNITS);
	else
		ret = drm_vblank_init(dev, 1);
	if (ret != 0)
		goto err_free;

	ret = drm_mode_create_dirty_info_property(dev);
	if (ret != 0)
		goto err_vblank_cleanup;

	if (dev_priv->capabilities & SVGA_CAP_MULTIMON)
		for (i = 0; i < VMWGFX_NUM_DISPLAY_UNITS; ++i)
			vmw_ldu_init(dev_priv, i);
	else
		vmw_ldu_init(dev_priv, 0);

	return 0;

err_vblank_cleanup:
	drm_vblank_cleanup(dev);
=======
	vmw_kms_create_implicit_placement_property(dev_priv);

	for (i = 0; i < num_display_units; ++i) {
		ret = vmw_ldu_init(dev_priv, i);
		if (ret != 0)
			goto err_free;
	}

	dev_priv->active_display_unit = vmw_du_legacy;

	drm_mode_config_reset(dev);

	return 0;

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
err_free:
	kfree(dev_priv->ldu_priv);
	dev_priv->ldu_priv = NULL;
	return ret;
}

<<<<<<< HEAD
int vmw_kms_close_legacy_display_system(struct vmw_private *dev_priv)
{
	struct drm_device *dev = dev_priv->dev;

	if (!dev_priv->ldu_priv)
		return -ENOSYS;

	drm_vblank_cleanup(dev);

=======
int vmw_kms_ldu_close_display(struct vmw_private *dev_priv)
{
	if (!dev_priv->ldu_priv)
		return -ENOSYS;

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	BUG_ON(!list_empty(&dev_priv->ldu_priv->active));

	kfree(dev_priv->ldu_priv);

	return 0;
}
<<<<<<< HEAD
=======


static int vmw_kms_ldu_do_bo_dirty(struct vmw_private *dev_priv,
				   struct vmw_framebuffer *framebuffer,
				   unsigned int flags, unsigned int color,
				   struct drm_mode_rect *clips,
				   unsigned int num_clips)
{
	size_t fifo_size;
	int i;

	struct {
		uint32_t header;
		SVGAFifoCmdUpdate body;
	} *cmd;

	fifo_size = sizeof(*cmd) * num_clips;
	cmd = VMW_CMD_RESERVE(dev_priv, fifo_size);
	if (unlikely(cmd == NULL))
		return -ENOMEM;

	memset(cmd, 0, fifo_size);
	for (i = 0; i < num_clips; i++, clips++) {
		cmd[i].header = SVGA_CMD_UPDATE;
		cmd[i].body.x = clips->x1;
		cmd[i].body.y = clips->y1;
		cmd[i].body.width = clips->x2 - clips->x1;
		cmd[i].body.height = clips->y2 - clips->y1;
	}

	vmw_cmd_commit(dev_priv, fifo_size);
	return 0;
}
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
