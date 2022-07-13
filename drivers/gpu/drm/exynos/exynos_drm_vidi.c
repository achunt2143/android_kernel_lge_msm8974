<<<<<<< HEAD
=======
// SPDX-License-Identifier: GPL-2.0-or-later
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
/* exynos_drm_vidi.c
 *
 * Copyright (C) 2012 Samsung Electronics Co.Ltd
 * Authors:
 *	Inki Dae <inki.dae@samsung.com>
<<<<<<< HEAD
 *
 * This program is free software; you can redistribute  it and/or modify it
 * under  the terms of  the GNU General  Public License as published by the
 * Free Software Foundation;  either version 2 of the  License, or (at your
 * option) any later version.
 *
 */
#include "drmP.h"

#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/platform_device.h>

#include <drm/exynos_drm.h>

#include "drm_edid.h"
#include "drm_crtc_helper.h"

#include "exynos_drm_drv.h"
#include "exynos_drm_crtc.h"
#include "exynos_drm_encoder.h"
=======
 */

#include <linux/component.h>
#include <linux/kernel.h>
#include <linux/platform_device.h>
#include <linux/timer.h>

#include <drm/drm_atomic_helper.h>
#include <drm/drm_edid.h>
#include <drm/drm_framebuffer.h>
#include <drm/drm_probe_helper.h>
#include <drm/drm_simple_kms_helper.h>
#include <drm/drm_vblank.h>
#include <drm/exynos_drm.h>

#include "exynos_drm_crtc.h"
#include "exynos_drm_drv.h"
#include "exynos_drm_fb.h"
#include "exynos_drm_plane.h"
#include "exynos_drm_vidi.h"

/* VIDI uses fixed refresh rate of 50Hz */
#define VIDI_REFRESH_TIME (1000 / 50)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

/* vidi has totally three virtual windows. */
#define WINDOWS_NR		3

<<<<<<< HEAD
#define get_vidi_context(dev)	platform_get_drvdata(to_platform_device(dev))

struct vidi_win_data {
	unsigned int		offset_x;
	unsigned int		offset_y;
	unsigned int		ovl_width;
	unsigned int		ovl_height;
	unsigned int		fb_width;
	unsigned int		fb_height;
	unsigned int		bpp;
	dma_addr_t		dma_addr;
	void __iomem		*vaddr;
	unsigned int		buf_offsize;
	unsigned int		line_size;	/* bytes */
	bool			enabled;
};

struct vidi_context {
	struct exynos_drm_subdrv	subdrv;
	struct drm_crtc			*crtc;
	struct vidi_win_data		win_data[WINDOWS_NR];
	struct edid			*raw_edid;
	unsigned int			clkdiv;
	unsigned int			default_win;
	unsigned long			irq_flags;
	unsigned int			connected;
	bool				vblank_on;
	bool				suspended;
	struct work_struct		work;
	struct mutex			lock;
};

=======
#define ctx_from_connector(c)	container_of(c, struct vidi_context, \
					connector)

struct vidi_context {
	struct drm_encoder		encoder;
	struct drm_device		*drm_dev;
	struct device			*dev;
	struct exynos_drm_crtc		*crtc;
	struct drm_connector		connector;
	struct exynos_drm_plane		planes[WINDOWS_NR];
	struct edid			*raw_edid;
	unsigned int			clkdiv;
	unsigned int			connected;
	bool				suspended;
	struct timer_list		timer;
	struct mutex			lock;
};

static inline struct vidi_context *encoder_to_vidi(struct drm_encoder *e)
{
	return container_of(e, struct vidi_context, encoder);
}

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
static const char fake_edid_info[] = {
	0x00, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x00, 0x4c, 0x2d, 0x05, 0x05,
	0x00, 0x00, 0x00, 0x00, 0x30, 0x12, 0x01, 0x03, 0x80, 0x10, 0x09, 0x78,
	0x0a, 0xee, 0x91, 0xa3, 0x54, 0x4c, 0x99, 0x26, 0x0f, 0x50, 0x54, 0xbd,
	0xee, 0x00, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01,
	0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x66, 0x21, 0x50, 0xb0, 0x51, 0x00,
	0x1b, 0x30, 0x40, 0x70, 0x36, 0x00, 0xa0, 0x5a, 0x00, 0x00, 0x00, 0x1e,
	0x01, 0x1d, 0x00, 0x72, 0x51, 0xd0, 0x1e, 0x20, 0x6e, 0x28, 0x55, 0x00,
	0xa0, 0x5a, 0x00, 0x00, 0x00, 0x1e, 0x00, 0x00, 0x00, 0xfd, 0x00, 0x18,
	0x4b, 0x1a, 0x44, 0x17, 0x00, 0x0a, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20,
	0x00, 0x00, 0x00, 0xfc, 0x00, 0x53, 0x41, 0x4d, 0x53, 0x55, 0x4e, 0x47,
	0x0a, 0x20, 0x20, 0x20, 0x20, 0x20, 0x01, 0xbc, 0x02, 0x03, 0x1e, 0xf1,
	0x46, 0x84, 0x05, 0x03, 0x10, 0x20, 0x22, 0x23, 0x09, 0x07, 0x07, 0x83,
	0x01, 0x00, 0x00, 0xe2, 0x00, 0x0f, 0x67, 0x03, 0x0c, 0x00, 0x10, 0x00,
	0xb8, 0x2d, 0x01, 0x1d, 0x80, 0x18, 0x71, 0x1c, 0x16, 0x20, 0x58, 0x2c,
	0x25, 0x00, 0xa0, 0x5a, 0x00, 0x00, 0x00, 0x9e, 0x8c, 0x0a, 0xd0, 0x8a,
	0x20, 0xe0, 0x2d, 0x10, 0x10, 0x3e, 0x96, 0x00, 0xa0, 0x5a, 0x00, 0x00,
	0x00, 0x18, 0x02, 0x3a, 0x80, 0x18, 0x71, 0x38, 0x2d, 0x40, 0x58, 0x2c,
	0x45, 0x00, 0xa0, 0x5a, 0x00, 0x00, 0x00, 0x1e, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x06
};

<<<<<<< HEAD
static void vidi_fake_vblank_handler(struct work_struct *work);

static bool vidi_display_is_connected(struct device *dev)
{
	struct vidi_context *ctx = get_vidi_context(dev);

	DRM_DEBUG_KMS("%s\n", __FILE__);

	/*
	 * connection request would come from user side
	 * to do hotplug through specific ioctl.
	 */
	return ctx->connected ? true : false;
}

static int vidi_get_edid(struct device *dev, struct drm_connector *connector,
				u8 *edid, int len)
{
	struct vidi_context *ctx = get_vidi_context(dev);
	struct edid *raw_edid;

	DRM_DEBUG_KMS("%s\n", __FILE__);

	/*
	 * the edid data comes from user side and it would be set
	 * to ctx->raw_edid through specific ioctl.
	 */
	if (!ctx->raw_edid) {
		DRM_DEBUG_KMS("raw_edid is null.\n");
		return -EFAULT;
	}

	raw_edid = kzalloc(len, GFP_KERNEL);
	if (!raw_edid) {
		DRM_DEBUG_KMS("failed to allocate raw_edid.\n");
		return -ENOMEM;
	}

	memcpy(raw_edid, ctx->raw_edid, min((1 + ctx->raw_edid->extensions)
						* EDID_LENGTH, len));

	/* attach the edid data to connector. */
	connector->display_info.raw_edid = (char *)raw_edid;

	memcpy(edid, ctx->raw_edid, min((1 + ctx->raw_edid->extensions)
					* EDID_LENGTH, len));

	return 0;
}

static void *vidi_get_panel(struct device *dev)
{
	DRM_DEBUG_KMS("%s\n", __FILE__);

	/* TODO. */

	return NULL;
}

static int vidi_check_timing(struct device *dev, void *timing)
{
	DRM_DEBUG_KMS("%s\n", __FILE__);

	/* TODO. */

	return 0;
}

static int vidi_display_power_on(struct device *dev, int mode)
{
	DRM_DEBUG_KMS("%s\n", __FILE__);

	/* TODO */

	return 0;
}

static struct exynos_drm_display_ops vidi_display_ops = {
	.type = EXYNOS_DISPLAY_TYPE_VIDI,
	.is_connected = vidi_display_is_connected,
	.get_edid = vidi_get_edid,
	.get_panel = vidi_get_panel,
	.check_timing = vidi_check_timing,
	.power_on = vidi_display_power_on,
};

static void vidi_dpms(struct device *subdrv_dev, int mode)
{
	struct vidi_context *ctx = get_vidi_context(subdrv_dev);

	DRM_DEBUG_KMS("%s, %d\n", __FILE__, mode);

	mutex_lock(&ctx->lock);

	switch (mode) {
	case DRM_MODE_DPMS_ON:
		/* TODO. */
		break;
	case DRM_MODE_DPMS_STANDBY:
	case DRM_MODE_DPMS_SUSPEND:
	case DRM_MODE_DPMS_OFF:
		/* TODO. */
		break;
	default:
		DRM_DEBUG_KMS("unspecified mode %d\n", mode);
		break;
	}

	mutex_unlock(&ctx->lock);
}

static void vidi_apply(struct device *subdrv_dev)
{
	struct vidi_context *ctx = get_vidi_context(subdrv_dev);
	struct exynos_drm_manager *mgr = ctx->subdrv.manager;
	struct exynos_drm_manager_ops *mgr_ops = mgr->ops;
	struct exynos_drm_overlay_ops *ovl_ops = mgr->overlay_ops;
	struct vidi_win_data *win_data;
	int i;

	DRM_DEBUG_KMS("%s\n", __FILE__);

	for (i = 0; i < WINDOWS_NR; i++) {
		win_data = &ctx->win_data[i];
		if (win_data->enabled && (ovl_ops && ovl_ops->commit))
			ovl_ops->commit(subdrv_dev, i);
	}

	if (mgr_ops && mgr_ops->commit)
		mgr_ops->commit(subdrv_dev);
}

static void vidi_commit(struct device *dev)
{
	struct vidi_context *ctx = get_vidi_context(dev);

	DRM_DEBUG_KMS("%s\n", __FILE__);

	if (ctx->suspended)
		return;
}

static int vidi_enable_vblank(struct device *dev)
{
	struct vidi_context *ctx = get_vidi_context(dev);

	DRM_DEBUG_KMS("%s\n", __FILE__);
=======
static const uint32_t formats[] = {
	DRM_FORMAT_XRGB8888,
	DRM_FORMAT_ARGB8888,
	DRM_FORMAT_NV12,
};

static const enum drm_plane_type vidi_win_types[WINDOWS_NR] = {
	DRM_PLANE_TYPE_PRIMARY,
	DRM_PLANE_TYPE_OVERLAY,
	DRM_PLANE_TYPE_CURSOR,
};

static int vidi_enable_vblank(struct exynos_drm_crtc *crtc)
{
	struct vidi_context *ctx = crtc->ctx;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	if (ctx->suspended)
		return -EPERM;

<<<<<<< HEAD
	if (!test_and_set_bit(0, &ctx->irq_flags))
		ctx->vblank_on = true;
=======
	mod_timer(&ctx->timer,
		jiffies + msecs_to_jiffies(VIDI_REFRESH_TIME) - 1);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	return 0;
}

<<<<<<< HEAD
static void vidi_disable_vblank(struct device *dev)
{
	struct vidi_context *ctx = get_vidi_context(dev);

	DRM_DEBUG_KMS("%s\n", __FILE__);
=======
static void vidi_disable_vblank(struct exynos_drm_crtc *crtc)
{
}

static void vidi_update_plane(struct exynos_drm_crtc *crtc,
			      struct exynos_drm_plane *plane)
{
	struct drm_plane_state *state = plane->base.state;
	struct vidi_context *ctx = crtc->ctx;
	dma_addr_t addr;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	if (ctx->suspended)
		return;

<<<<<<< HEAD
	if (test_and_clear_bit(0, &ctx->irq_flags))
		ctx->vblank_on = false;
}

static struct exynos_drm_manager_ops vidi_manager_ops = {
	.dpms = vidi_dpms,
	.apply = vidi_apply,
	.commit = vidi_commit,
	.enable_vblank = vidi_enable_vblank,
	.disable_vblank = vidi_disable_vblank,
};

static void vidi_win_mode_set(struct device *dev,
			      struct exynos_drm_overlay *overlay)
{
	struct vidi_context *ctx = get_vidi_context(dev);
	struct vidi_win_data *win_data;
	int win;
	unsigned long offset;

	DRM_DEBUG_KMS("%s\n", __FILE__);

	if (!overlay) {
		dev_err(dev, "overlay is NULL\n");
		return;
	}

	win = overlay->zpos;
	if (win == DEFAULT_ZPOS)
		win = ctx->default_win;

	if (win < 0 || win > WINDOWS_NR)
		return;

	offset = overlay->fb_x * (overlay->bpp >> 3);
	offset += overlay->fb_y * overlay->pitch;

	DRM_DEBUG_KMS("offset = 0x%lx, pitch = %x\n", offset, overlay->pitch);

	win_data = &ctx->win_data[win];

	win_data->offset_x = overlay->crtc_x;
	win_data->offset_y = overlay->crtc_y;
	win_data->ovl_width = overlay->crtc_width;
	win_data->ovl_height = overlay->crtc_height;
	win_data->fb_width = overlay->fb_width;
	win_data->fb_height = overlay->fb_height;
	win_data->dma_addr = overlay->dma_addr[0] + offset;
	win_data->vaddr = overlay->vaddr[0] + offset;
	win_data->bpp = overlay->bpp;
	win_data->buf_offsize = (overlay->fb_width - overlay->crtc_width) *
				(overlay->bpp >> 3);
	win_data->line_size = overlay->crtc_width * (overlay->bpp >> 3);

	/*
	 * some parts of win_data should be transferred to user side
	 * through specific ioctl.
	 */

	DRM_DEBUG_KMS("offset_x = %d, offset_y = %d\n",
			win_data->offset_x, win_data->offset_y);
	DRM_DEBUG_KMS("ovl_width = %d, ovl_height = %d\n",
			win_data->ovl_width, win_data->ovl_height);
	DRM_DEBUG_KMS("paddr = 0x%lx, vaddr = 0x%lx\n",
			(unsigned long)win_data->dma_addr,
			(unsigned long)win_data->vaddr);
	DRM_DEBUG_KMS("fb_width = %d, crtc_width = %d\n",
			overlay->fb_width, overlay->crtc_width);
}

static void vidi_win_commit(struct device *dev, int zpos)
{
	struct vidi_context *ctx = get_vidi_context(dev);
	struct vidi_win_data *win_data;
	int win = zpos;

	DRM_DEBUG_KMS("%s\n", __FILE__);

	if (ctx->suspended)
		return;

	if (win == DEFAULT_ZPOS)
		win = ctx->default_win;

	if (win < 0 || win > WINDOWS_NR)
		return;

	win_data = &ctx->win_data[win];

	win_data->enabled = true;

	DRM_DEBUG_KMS("dma_addr = 0x%x\n", win_data->dma_addr);

	if (ctx->vblank_on)
		schedule_work(&ctx->work);
}

static void vidi_win_disable(struct device *dev, int zpos)
{
	struct vidi_context *ctx = get_vidi_context(dev);
	struct vidi_win_data *win_data;
	int win = zpos;

	DRM_DEBUG_KMS("%s\n", __FILE__);

	if (win == DEFAULT_ZPOS)
		win = ctx->default_win;

	if (win < 0 || win > WINDOWS_NR)
		return;

	win_data = &ctx->win_data[win];
	win_data->enabled = false;

	/* TODO. */
}

static struct exynos_drm_overlay_ops vidi_overlay_ops = {
	.mode_set = vidi_win_mode_set,
	.commit = vidi_win_commit,
	.disable = vidi_win_disable,
};

static struct exynos_drm_manager vidi_manager = {
	.pipe		= -1,
	.ops		= &vidi_manager_ops,
	.overlay_ops	= &vidi_overlay_ops,
	.display_ops	= &vidi_display_ops,
};

static void vidi_finish_pageflip(struct drm_device *drm_dev, int crtc)
{
	struct exynos_drm_private *dev_priv = drm_dev->dev_private;
	struct drm_pending_vblank_event *e, *t;
	struct timeval now;
	unsigned long flags;
	bool is_checked = false;

	spin_lock_irqsave(&drm_dev->event_lock, flags);

	list_for_each_entry_safe(e, t, &dev_priv->pageflip_event_list,
			base.link) {
		/* if event's pipe isn't same as crtc then ignore it. */
		if (crtc != e->pipe)
			continue;

		is_checked = true;

		do_gettimeofday(&now);
		e->event.sequence = 0;
		e->event.tv_sec = now.tv_sec;
		e->event.tv_usec = now.tv_usec;

		list_move_tail(&e->base.link, &e->base.file_priv->event_list);
		wake_up_interruptible(&e->base.file_priv->event_wait);
	}

	if (is_checked) {
		/*
		 * call drm_vblank_put only in case that drm_vblank_get was
		 * called.
		 */
		if (atomic_read(&drm_dev->vblank_refcount[crtc]) > 0)
			drm_vblank_put(drm_dev, crtc);

		/*
		 * don't off vblank if vblank_disable_allowed is 1,
		 * because vblank would be off by timer handler.
		 */
		if (!drm_dev->vblank_disable_allowed)
			drm_vblank_off(drm_dev, crtc);
	}

	spin_unlock_irqrestore(&drm_dev->event_lock, flags);
}

static void vidi_fake_vblank_handler(struct work_struct *work)
{
	struct vidi_context *ctx = container_of(work, struct vidi_context,
					work);
	struct exynos_drm_subdrv *subdrv = &ctx->subdrv;
	struct exynos_drm_manager *manager = subdrv->manager;

	if (manager->pipe < 0)
		return;

	/* refresh rate is about 50Hz. */
	usleep_range(16000, 20000);

	drm_handle_vblank(subdrv->drm_dev, manager->pipe);
	vidi_finish_pageflip(subdrv->drm_dev, manager->pipe);
}

static int vidi_subdrv_probe(struct drm_device *drm_dev, struct device *dev)
{
	DRM_DEBUG_KMS("%s\n", __FILE__);

	/*
	 * enable drm irq mode.
	 * - with irq_enabled = 1, we can use the vblank feature.
	 *
	 * P.S. note that we wouldn't use drm irq handler but
	 *	just specific driver own one instead because
	 *	drm framework supports only one irq handler.
	 */
	drm_dev->irq_enabled = 1;

	/*
	 * with vblank_disable_allowed = 1, vblank interrupt will be disabled
	 * by drm timer once a current process gives up ownership of
	 * vblank event.(after drm_vblank_put function is called)
	 */
	drm_dev->vblank_disable_allowed = 1;

	return 0;
}

static void vidi_subdrv_remove(struct drm_device *drm_dev)
{
	DRM_DEBUG_KMS("%s\n", __FILE__);

	/* TODO. */
}

static int vidi_power_on(struct vidi_context *ctx, bool enable)
{
	struct exynos_drm_subdrv *subdrv = &ctx->subdrv;
	struct device *dev = subdrv->dev;

	DRM_DEBUG_KMS("%s\n", __FILE__);

	if (enable != false && enable != true)
		return -EINVAL;

	if (enable) {
		ctx->suspended = false;

		/* if vblank was enabled status, enable it again. */
		if (test_and_clear_bit(0, &ctx->irq_flags))
			vidi_enable_vblank(dev);

		vidi_apply(dev);
	} else {
		ctx->suspended = true;
	}

	return 0;
}

static int vidi_show_connection(struct device *dev,
				struct device_attribute *attr, char *buf)
{
	int rc;
	struct vidi_context *ctx = get_vidi_context(dev);
=======
	addr = exynos_drm_fb_dma_addr(state->fb, 0);
	DRM_DEV_DEBUG_KMS(ctx->dev, "dma_addr = %pad\n", &addr);
}

static void vidi_atomic_enable(struct exynos_drm_crtc *crtc)
{
	struct vidi_context *ctx = crtc->ctx;

	mutex_lock(&ctx->lock);

	ctx->suspended = false;

	mutex_unlock(&ctx->lock);

	drm_crtc_vblank_on(&crtc->base);
}

static void vidi_atomic_disable(struct exynos_drm_crtc *crtc)
{
	struct vidi_context *ctx = crtc->ctx;

	drm_crtc_vblank_off(&crtc->base);

	mutex_lock(&ctx->lock);

	ctx->suspended = true;

	mutex_unlock(&ctx->lock);
}

static const struct exynos_drm_crtc_ops vidi_crtc_ops = {
	.atomic_enable = vidi_atomic_enable,
	.atomic_disable = vidi_atomic_disable,
	.enable_vblank = vidi_enable_vblank,
	.disable_vblank = vidi_disable_vblank,
	.update_plane = vidi_update_plane,
	.atomic_flush = exynos_crtc_handle_event,
};

static void vidi_fake_vblank_timer(struct timer_list *t)
{
	struct vidi_context *ctx = from_timer(ctx, t, timer);

	if (drm_crtc_handle_vblank(&ctx->crtc->base))
		mod_timer(&ctx->timer,
			jiffies + msecs_to_jiffies(VIDI_REFRESH_TIME) - 1);
}

static ssize_t vidi_show_connection(struct device *dev,
				struct device_attribute *attr, char *buf)
{
	struct vidi_context *ctx = dev_get_drvdata(dev);
	int rc;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	mutex_lock(&ctx->lock);

	rc = sprintf(buf, "%d\n", ctx->connected);

	mutex_unlock(&ctx->lock);

	return rc;
}

<<<<<<< HEAD
static int vidi_store_connection(struct device *dev,
				struct device_attribute *attr,
				const char *buf, size_t len)
{
	struct vidi_context *ctx = get_vidi_context(dev);
	int ret;

	DRM_DEBUG_KMS("%s\n", __FILE__);

=======
static ssize_t vidi_store_connection(struct device *dev,
				struct device_attribute *attr,
				const char *buf, size_t len)
{
	struct vidi_context *ctx = dev_get_drvdata(dev);
	int ret;

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	ret = kstrtoint(buf, 0, &ctx->connected);
	if (ret)
		return ret;

	if (ctx->connected > 1)
		return -EINVAL;

<<<<<<< HEAD
	DRM_DEBUG_KMS("requested connection.\n");

	drm_helper_hpd_irq_event(ctx->subdrv.drm_dev);
=======
	/* use fake edid data for test. */
	if (!ctx->raw_edid)
		ctx->raw_edid = (struct edid *)fake_edid_info;

	/* if raw_edid isn't same as fake data then it can't be tested. */
	if (ctx->raw_edid != (struct edid *)fake_edid_info) {
		DRM_DEV_DEBUG_KMS(dev, "edid data is not fake data.\n");
		return -EINVAL;
	}

	DRM_DEV_DEBUG_KMS(dev, "requested connection.\n");

	drm_helper_hpd_irq_event(ctx->drm_dev);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	return len;
}

static DEVICE_ATTR(connection, 0644, vidi_show_connection,
			vidi_store_connection);

<<<<<<< HEAD
int vidi_connection_ioctl(struct drm_device *drm_dev, void *data,
				struct drm_file *file_priv)
{
	struct vidi_context *ctx = NULL;
	struct drm_encoder *encoder;
	struct exynos_drm_manager *manager;
	struct exynos_drm_display_ops *display_ops;
	struct drm_exynos_vidi_connection *vidi = data;

	DRM_DEBUG_KMS("%s\n", __FILE__);

	if (!vidi) {
		DRM_DEBUG_KMS("user data for vidi is null.\n");
		return -EINVAL;
	}

	if (!vidi->edid) {
		DRM_DEBUG_KMS("edid data is null.\n");
=======
static struct attribute *vidi_attrs[] = {
	&dev_attr_connection.attr,
	NULL,
};
ATTRIBUTE_GROUPS(vidi);

int vidi_connection_ioctl(struct drm_device *drm_dev, void *data,
				struct drm_file *file_priv)
{
	struct vidi_context *ctx = dev_get_drvdata(drm_dev->dev);
	struct drm_exynos_vidi_connection *vidi = data;

	if (!vidi) {
		DRM_DEV_DEBUG_KMS(ctx->dev,
				  "user data for vidi is null.\n");
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		return -EINVAL;
	}

	if (vidi->connection > 1) {
<<<<<<< HEAD
		DRM_DEBUG_KMS("connection should be 0 or 1.\n");
		return -EINVAL;
	}

	list_for_each_entry(encoder, &drm_dev->mode_config.encoder_list,
								head) {
		manager = exynos_drm_get_manager(encoder);
		display_ops = manager->display_ops;

		if (display_ops->type == EXYNOS_DISPLAY_TYPE_VIDI) {
			ctx = get_vidi_context(manager->dev);
			break;
		}
	}

	if (!ctx) {
		DRM_DEBUG_KMS("not found virtual device type encoder.\n");
=======
		DRM_DEV_DEBUG_KMS(ctx->dev,
				  "connection should be 0 or 1.\n");
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		return -EINVAL;
	}

	if (ctx->connected == vidi->connection) {
<<<<<<< HEAD
		DRM_DEBUG_KMS("same connection request.\n");
		return -EINVAL;
	}

	if (vidi->connection)
		ctx->raw_edid = (struct edid *)vidi->edid;

	ctx->connected = vidi->connection;
	drm_helper_hpd_irq_event(ctx->subdrv.drm_dev);
=======
		DRM_DEV_DEBUG_KMS(ctx->dev,
				  "same connection request.\n");
		return -EINVAL;
	}

	if (vidi->connection) {
		struct edid *raw_edid;

		raw_edid = (struct edid *)(unsigned long)vidi->edid;
		if (!drm_edid_is_valid(raw_edid)) {
			DRM_DEV_DEBUG_KMS(ctx->dev,
					  "edid data is invalid.\n");
			return -EINVAL;
		}
		ctx->raw_edid = drm_edid_duplicate(raw_edid);
		if (!ctx->raw_edid) {
			DRM_DEV_DEBUG_KMS(ctx->dev,
					  "failed to allocate raw_edid.\n");
			return -ENOMEM;
		}
	} else {
		/*
		 * with connection = 0, free raw_edid
		 * only if raw edid data isn't same as fake data.
		 */
		if (ctx->raw_edid && ctx->raw_edid !=
				(struct edid *)fake_edid_info) {
			kfree(ctx->raw_edid);
			ctx->raw_edid = NULL;
		}
	}

	ctx->connected = vidi->connection;
	drm_helper_hpd_irq_event(ctx->drm_dev);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	return 0;
}

<<<<<<< HEAD
static int __devinit vidi_probe(struct platform_device *pdev)
{
	struct device *dev = &pdev->dev;
	struct vidi_context *ctx;
	struct exynos_drm_subdrv *subdrv;
	int ret;

	DRM_DEBUG_KMS("%s\n", __FILE__);

	ctx = kzalloc(sizeof(*ctx), GFP_KERNEL);
	if (!ctx)
		return -ENOMEM;

	ctx->default_win = 0;

	INIT_WORK(&ctx->work, vidi_fake_vblank_handler);

	/* for test */
	ctx->raw_edid = (struct edid *)fake_edid_info;

	subdrv = &ctx->subdrv;
	subdrv->dev = dev;
	subdrv->manager = &vidi_manager;
	subdrv->probe = vidi_subdrv_probe;
	subdrv->remove = vidi_subdrv_remove;
=======
static enum drm_connector_status vidi_detect(struct drm_connector *connector,
			bool force)
{
	struct vidi_context *ctx = ctx_from_connector(connector);

	/*
	 * connection request would come from user side
	 * to do hotplug through specific ioctl.
	 */
	return ctx->connected ? connector_status_connected :
			connector_status_disconnected;
}

static void vidi_connector_destroy(struct drm_connector *connector)
{
}

static const struct drm_connector_funcs vidi_connector_funcs = {
	.fill_modes = drm_helper_probe_single_connector_modes,
	.detect = vidi_detect,
	.destroy = vidi_connector_destroy,
	.reset = drm_atomic_helper_connector_reset,
	.atomic_duplicate_state = drm_atomic_helper_connector_duplicate_state,
	.atomic_destroy_state = drm_atomic_helper_connector_destroy_state,
};

static int vidi_get_modes(struct drm_connector *connector)
{
	struct vidi_context *ctx = ctx_from_connector(connector);
	struct edid *edid;
	int edid_len;

	/*
	 * the edid data comes from user side and it would be set
	 * to ctx->raw_edid through specific ioctl.
	 */
	if (!ctx->raw_edid) {
		DRM_DEV_DEBUG_KMS(ctx->dev, "raw_edid is null.\n");
		return 0;
	}

	edid_len = (1 + ctx->raw_edid->extensions) * EDID_LENGTH;
	edid = kmemdup(ctx->raw_edid, edid_len, GFP_KERNEL);
	if (!edid) {
		DRM_DEV_DEBUG_KMS(ctx->dev, "failed to allocate edid\n");
		return 0;
	}

	drm_connector_update_edid_property(connector, edid);

	return drm_add_edid_modes(connector, edid);
}

static const struct drm_connector_helper_funcs vidi_connector_helper_funcs = {
	.get_modes = vidi_get_modes,
};

static int vidi_create_connector(struct drm_encoder *encoder)
{
	struct vidi_context *ctx = encoder_to_vidi(encoder);
	struct drm_connector *connector = &ctx->connector;
	int ret;

	connector->polled = DRM_CONNECTOR_POLL_HPD;

	ret = drm_connector_init(ctx->drm_dev, connector,
			&vidi_connector_funcs, DRM_MODE_CONNECTOR_VIRTUAL);
	if (ret) {
		DRM_DEV_ERROR(ctx->dev,
			      "Failed to initialize connector with drm\n");
		return ret;
	}

	drm_connector_helper_add(connector, &vidi_connector_helper_funcs);
	drm_connector_attach_encoder(connector, encoder);

	return 0;
}

static void exynos_vidi_mode_set(struct drm_encoder *encoder,
			       struct drm_display_mode *mode,
			       struct drm_display_mode *adjusted_mode)
{
}

static void exynos_vidi_enable(struct drm_encoder *encoder)
{
}

static void exynos_vidi_disable(struct drm_encoder *encoder)
{
}

static const struct drm_encoder_helper_funcs exynos_vidi_encoder_helper_funcs = {
	.mode_set = exynos_vidi_mode_set,
	.enable = exynos_vidi_enable,
	.disable = exynos_vidi_disable,
};

static int vidi_bind(struct device *dev, struct device *master, void *data)
{
	struct vidi_context *ctx = dev_get_drvdata(dev);
	struct drm_device *drm_dev = data;
	struct drm_encoder *encoder = &ctx->encoder;
	struct exynos_drm_plane *exynos_plane;
	struct exynos_drm_plane_config plane_config = { 0 };
	unsigned int i;
	int ret;

	ctx->drm_dev = drm_dev;

	plane_config.pixel_formats = formats;
	plane_config.num_pixel_formats = ARRAY_SIZE(formats);

	for (i = 0; i < WINDOWS_NR; i++) {
		plane_config.zpos = i;
		plane_config.type = vidi_win_types[i];

		ret = exynos_plane_init(drm_dev, &ctx->planes[i], i,
					&plane_config);
		if (ret)
			return ret;
	}

	exynos_plane = &ctx->planes[DEFAULT_WIN];
	ctx->crtc = exynos_drm_crtc_create(drm_dev, &exynos_plane->base,
			EXYNOS_DISPLAY_TYPE_VIDI, &vidi_crtc_ops, ctx);
	if (IS_ERR(ctx->crtc)) {
		DRM_DEV_ERROR(dev, "failed to create crtc.\n");
		return PTR_ERR(ctx->crtc);
	}

	drm_simple_encoder_init(drm_dev, encoder, DRM_MODE_ENCODER_TMDS);

	drm_encoder_helper_add(encoder, &exynos_vidi_encoder_helper_funcs);

	ret = exynos_drm_set_possible_crtcs(encoder, EXYNOS_DISPLAY_TYPE_VIDI);
	if (ret < 0)
		return ret;

	ret = vidi_create_connector(encoder);
	if (ret) {
		DRM_DEV_ERROR(dev, "failed to create connector ret = %d\n",
			      ret);
		drm_encoder_cleanup(encoder);
		return ret;
	}

	return 0;
}


static void vidi_unbind(struct device *dev, struct device *master, void *data)
{
	struct vidi_context *ctx = dev_get_drvdata(dev);

	del_timer_sync(&ctx->timer);
}

static const struct component_ops vidi_component_ops = {
	.bind	= vidi_bind,
	.unbind = vidi_unbind,
};

static int vidi_probe(struct platform_device *pdev)
{
	struct vidi_context *ctx;
	struct device *dev = &pdev->dev;

	ctx = devm_kzalloc(dev, sizeof(*ctx), GFP_KERNEL);
	if (!ctx)
		return -ENOMEM;

	ctx->dev = dev;

	timer_setup(&ctx->timer, vidi_fake_vblank_timer, 0);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	mutex_init(&ctx->lock);

	platform_set_drvdata(pdev, ctx);

<<<<<<< HEAD
	ret = device_create_file(&pdev->dev, &dev_attr_connection);
	if (ret < 0)
		DRM_INFO("failed to create connection sysfs.\n");

	exynos_drm_subdrv_register(subdrv);

	return 0;
}

static int __devexit vidi_remove(struct platform_device *pdev)
{
	struct vidi_context *ctx = platform_get_drvdata(pdev);

	DRM_DEBUG_KMS("%s\n", __FILE__);

	exynos_drm_subdrv_unregister(&ctx->subdrv);

	kfree(ctx);

	return 0;
}

#ifdef CONFIG_PM_SLEEP
static int vidi_suspend(struct device *dev)
{
	struct vidi_context *ctx = get_vidi_context(dev);

	return vidi_power_on(ctx, false);
}

static int vidi_resume(struct device *dev)
{
	struct vidi_context *ctx = get_vidi_context(dev);

	return vidi_power_on(ctx, true);
}
#endif

static const struct dev_pm_ops vidi_pm_ops = {
	SET_SYSTEM_SLEEP_PM_OPS(vidi_suspend, vidi_resume)
};

struct platform_driver vidi_driver = {
	.probe		= vidi_probe,
	.remove		= __devexit_p(vidi_remove),
	.driver		= {
		.name	= "exynos-drm-vidi",
		.owner	= THIS_MODULE,
		.pm	= &vidi_pm_ops,
=======
	return component_add(dev, &vidi_component_ops);
}

static void vidi_remove(struct platform_device *pdev)
{
	struct vidi_context *ctx = platform_get_drvdata(pdev);

	if (ctx->raw_edid != (struct edid *)fake_edid_info) {
		kfree(ctx->raw_edid);
		ctx->raw_edid = NULL;
	}

	component_del(&pdev->dev, &vidi_component_ops);
}

struct platform_driver vidi_driver = {
	.probe		= vidi_probe,
	.remove_new	= vidi_remove,
	.driver		= {
		.name	= "exynos-drm-vidi",
		.owner	= THIS_MODULE,
		.dev_groups = vidi_groups,
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	},
};
