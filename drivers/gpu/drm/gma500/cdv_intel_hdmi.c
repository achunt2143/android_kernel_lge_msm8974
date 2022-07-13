/*
 * Copyright © 2006-2011 Intel Corporation
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
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
 * DEALINGS IN THE SOFTWARE.
 *
 * Authors:
 *	jim liu <jim.liu@intel.com>
<<<<<<< HEAD
 *
 * FIXME:
 *	We should probably make this generic and share it with Medfield
 */

#include <drm/drmP.h>
#include <drm/drm.h>
#include <drm/drm_crtc.h>
#include <drm/drm_edid.h>
#include "psb_intel_drv.h"
#include "psb_drv.h"
#include "psb_intel_reg.h"
#include "cdv_device.h"
#include <linux/pm_runtime.h>
=======
 */

#include <linux/pm_runtime.h>

#include <drm/drm.h>
#include <drm/drm_crtc.h>
#include <drm/drm_crtc_helper.h>
#include <drm/drm_edid.h>
#include <drm/drm_modeset_helper_vtables.h>
#include <drm/drm_simple_kms_helper.h>

#include "cdv_device.h"
#include "psb_drv.h"
#include "psb_intel_drv.h"
#include "psb_intel_reg.h"
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

/* hdmi control bits */
#define HDMI_NULL_PACKETS_DURING_VSYNC	(1 << 9)
#define HDMI_BORDER_ENABLE		(1 << 7)
#define HDMI_AUDIO_ENABLE		(1 << 6)
#define HDMI_VSYNC_ACTIVE_HIGH		(1 << 4)
#define HDMI_HSYNC_ACTIVE_HIGH		(1 << 3)
/* hdmi-b control bits */
#define	HDMIB_PIPE_B_SELECT		(1 << 30)


struct mid_intel_hdmi_priv {
	u32 hdmi_reg;
	u32 save_HDMIB;
	bool has_hdmi_sink;
	bool has_hdmi_audio;
	/* Should set this when detect hotplug */
	bool hdmi_device_connected;
<<<<<<< HEAD
	struct mdfld_hdmi_i2c *i2c_bus;
	struct i2c_adapter *hdmi_i2c_adapter;	/* for control functions */
=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	struct drm_device *dev;
};

static void cdv_hdmi_mode_set(struct drm_encoder *encoder,
			struct drm_display_mode *mode,
			struct drm_display_mode *adjusted_mode)
{
	struct drm_device *dev = encoder->dev;
<<<<<<< HEAD
	struct psb_intel_encoder *psb_intel_encoder = to_psb_intel_encoder(encoder);
	struct mid_intel_hdmi_priv *hdmi_priv = psb_intel_encoder->dev_priv;
	u32 hdmib;
	struct drm_crtc *crtc = encoder->crtc;
	struct psb_intel_crtc *intel_crtc = to_psb_intel_crtc(crtc);
=======
	struct gma_encoder *gma_encoder = to_gma_encoder(encoder);
	struct mid_intel_hdmi_priv *hdmi_priv = gma_encoder->dev_priv;
	u32 hdmib;
	struct drm_crtc *crtc = encoder->crtc;
	struct gma_crtc *gma_crtc = to_gma_crtc(crtc);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	hdmib = (2 << 10);

	if (adjusted_mode->flags & DRM_MODE_FLAG_PVSYNC)
		hdmib |= HDMI_VSYNC_ACTIVE_HIGH;
	if (adjusted_mode->flags & DRM_MODE_FLAG_PHSYNC)
		hdmib |= HDMI_HSYNC_ACTIVE_HIGH;

<<<<<<< HEAD
	if (intel_crtc->pipe == 1)
=======
	if (gma_crtc->pipe == 1)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		hdmib |= HDMIB_PIPE_B_SELECT;

	if (hdmi_priv->has_hdmi_audio) {
		hdmib |= HDMI_AUDIO_ENABLE;
		hdmib |= HDMI_NULL_PACKETS_DURING_VSYNC;
	}

	REG_WRITE(hdmi_priv->hdmi_reg, hdmib);
	REG_READ(hdmi_priv->hdmi_reg);
}

<<<<<<< HEAD
static bool cdv_hdmi_mode_fixup(struct drm_encoder *encoder,
				  struct drm_display_mode *mode,
				  struct drm_display_mode *adjusted_mode)
{
	return true;
}

static void cdv_hdmi_dpms(struct drm_encoder *encoder, int mode)
{
	struct drm_device *dev = encoder->dev;
	struct psb_intel_encoder *psb_intel_encoder =
						to_psb_intel_encoder(encoder);
	struct mid_intel_hdmi_priv *hdmi_priv = psb_intel_encoder->dev_priv;
=======
static void cdv_hdmi_dpms(struct drm_encoder *encoder, int mode)
{
	struct drm_device *dev = encoder->dev;
	struct gma_encoder *gma_encoder = to_gma_encoder(encoder);
	struct mid_intel_hdmi_priv *hdmi_priv = gma_encoder->dev_priv;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	u32 hdmib;

	hdmib = REG_READ(hdmi_priv->hdmi_reg);

	if (mode != DRM_MODE_DPMS_ON)
		REG_WRITE(hdmi_priv->hdmi_reg, hdmib & ~HDMIB_PORT_EN);
	else
		REG_WRITE(hdmi_priv->hdmi_reg, hdmib | HDMIB_PORT_EN);
	REG_READ(hdmi_priv->hdmi_reg);
}

static void cdv_hdmi_save(struct drm_connector *connector)
{
	struct drm_device *dev = connector->dev;
<<<<<<< HEAD
	struct psb_intel_encoder *psb_intel_encoder =
					psb_intel_attached_encoder(connector);
	struct mid_intel_hdmi_priv *hdmi_priv = psb_intel_encoder->dev_priv;
=======
	struct gma_encoder *gma_encoder = gma_attached_encoder(connector);
	struct mid_intel_hdmi_priv *hdmi_priv = gma_encoder->dev_priv;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	hdmi_priv->save_HDMIB = REG_READ(hdmi_priv->hdmi_reg);
}

static void cdv_hdmi_restore(struct drm_connector *connector)
{
	struct drm_device *dev = connector->dev;
<<<<<<< HEAD
	struct psb_intel_encoder *psb_intel_encoder =
					psb_intel_attached_encoder(connector);
	struct mid_intel_hdmi_priv *hdmi_priv = psb_intel_encoder->dev_priv;
=======
	struct gma_encoder *gma_encoder = gma_attached_encoder(connector);
	struct mid_intel_hdmi_priv *hdmi_priv = gma_encoder->dev_priv;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	REG_WRITE(hdmi_priv->hdmi_reg, hdmi_priv->save_HDMIB);
	REG_READ(hdmi_priv->hdmi_reg);
}

static enum drm_connector_status cdv_hdmi_detect(
				struct drm_connector *connector, bool force)
{
<<<<<<< HEAD
	struct psb_intel_encoder *psb_intel_encoder =
					psb_intel_attached_encoder(connector);
	struct psb_intel_connector *psb_intel_connector =
					to_psb_intel_connector(connector);
	struct mid_intel_hdmi_priv *hdmi_priv = psb_intel_encoder->dev_priv;
	struct edid *edid = NULL;
	enum drm_connector_status status = connector_status_disconnected;

	edid = drm_get_edid(connector, &psb_intel_encoder->i2c_bus->adapter);
=======
	struct gma_encoder *gma_encoder = gma_attached_encoder(connector);
	struct mid_intel_hdmi_priv *hdmi_priv = gma_encoder->dev_priv;
	struct edid *edid = NULL;
	enum drm_connector_status status = connector_status_disconnected;

	edid = drm_get_edid(connector, connector->ddc);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	hdmi_priv->has_hdmi_sink = false;
	hdmi_priv->has_hdmi_audio = false;
	if (edid) {
		if (edid->input & DRM_EDID_INPUT_DIGITAL) {
			status = connector_status_connected;
			hdmi_priv->has_hdmi_sink =
						drm_detect_hdmi_monitor(edid);
			hdmi_priv->has_hdmi_audio =
						drm_detect_monitor_audio(edid);
		}
<<<<<<< HEAD

		psb_intel_connector->base.display_info.raw_edid = NULL;
=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		kfree(edid);
	}
	return status;
}

static int cdv_hdmi_set_property(struct drm_connector *connector,
				       struct drm_property *property,
				       uint64_t value)
{
	struct drm_encoder *encoder = connector->encoder;

	if (!strcmp(property->name, "scaling mode") && encoder) {
<<<<<<< HEAD
		struct psb_intel_crtc *crtc = to_psb_intel_crtc(encoder->crtc);
=======
		struct gma_crtc *crtc = to_gma_crtc(encoder->crtc);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		bool centre;
		uint64_t curValue;

		if (!crtc)
			return -1;

		switch (value) {
		case DRM_MODE_SCALE_FULLSCREEN:
			break;
		case DRM_MODE_SCALE_NO_SCALE:
			break;
		case DRM_MODE_SCALE_ASPECT:
			break;
		default:
			return -1;
		}

<<<<<<< HEAD
		if (drm_connector_property_get_value(connector,
=======
		if (drm_object_property_get_value(&connector->base,
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
							property, &curValue))
			return -1;

		if (curValue == value)
			return 0;

<<<<<<< HEAD
		if (drm_connector_property_set_value(connector,
=======
		if (drm_object_property_set_value(&connector->base,
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
							property, value))
			return -1;

		centre = (curValue == DRM_MODE_SCALE_NO_SCALE) ||
			(value == DRM_MODE_SCALE_NO_SCALE);

		if (crtc->saved_mode.hdisplay != 0 &&
		    crtc->saved_mode.vdisplay != 0) {
			if (centre) {
				if (!drm_crtc_helper_set_mode(encoder->crtc, &crtc->saved_mode,
<<<<<<< HEAD
					    encoder->crtc->x, encoder->crtc->y, encoder->crtc->fb))
					return -1;
			} else {
				struct drm_encoder_helper_funcs *helpers
=======
					    encoder->crtc->x, encoder->crtc->y, encoder->crtc->primary->fb))
					return -1;
			} else {
				const struct drm_encoder_helper_funcs *helpers
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
						    = encoder->helper_private;
				helpers->mode_set(encoder, &crtc->saved_mode,
					     &crtc->saved_adjusted_mode);
			}
		}
	}
	return 0;
}

/*
 * Return the list of HDMI DDC modes if available.
 */
static int cdv_hdmi_get_modes(struct drm_connector *connector)
{
<<<<<<< HEAD
	struct psb_intel_encoder *psb_intel_encoder =
					psb_intel_attached_encoder(connector);
	struct edid *edid = NULL;
	int ret = 0;

	edid = drm_get_edid(connector, &psb_intel_encoder->i2c_bus->adapter);
	if (edid) {
		drm_mode_connector_update_edid_property(connector, edid);
=======
	struct edid *edid = NULL;
	int ret = 0;

	edid = drm_get_edid(connector, connector->ddc);
	if (edid) {
		drm_connector_update_edid_property(connector, edid);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		ret = drm_add_edid_modes(connector, edid);
		kfree(edid);
	}
	return ret;
}

<<<<<<< HEAD
static int cdv_hdmi_mode_valid(struct drm_connector *connector,
				 struct drm_display_mode *mode)
{
	struct drm_psb_private *dev_priv = connector->dev->dev_private;

=======
static enum drm_mode_status cdv_hdmi_mode_valid(struct drm_connector *connector,
				 struct drm_display_mode *mode)
{
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	if (mode->clock > 165000)
		return MODE_CLOCK_HIGH;
	if (mode->clock < 20000)
		return MODE_CLOCK_HIGH;

	/* just in case */
	if (mode->flags & DRM_MODE_FLAG_DBLSCAN)
		return MODE_NO_DBLESCAN;

	/* just in case */
	if (mode->flags & DRM_MODE_FLAG_INTERLACE)
		return MODE_NO_INTERLACE;

<<<<<<< HEAD
	/* We assume worst case scenario of 32 bpp here, since we don't know */
	if ((ALIGN(mode->hdisplay * 4, 64) * mode->vdisplay) >
	    dev_priv->vram_stolen_size)
		return MODE_MEM;

=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	return MODE_OK;
}

static void cdv_hdmi_destroy(struct drm_connector *connector)
{
<<<<<<< HEAD
	struct psb_intel_encoder *psb_intel_encoder =
					psb_intel_attached_encoder(connector);

	if (psb_intel_encoder->i2c_bus)
		psb_intel_i2c_destroy(psb_intel_encoder->i2c_bus);
	drm_sysfs_connector_remove(connector);
	drm_connector_cleanup(connector);
	kfree(connector);
=======
	struct gma_connector *gma_connector = to_gma_connector(connector);
	struct gma_i2c_chan *ddc_bus = to_gma_i2c_chan(connector->ddc);

	gma_i2c_destroy(ddc_bus);
	drm_connector_cleanup(connector);
	kfree(gma_connector);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

static const struct drm_encoder_helper_funcs cdv_hdmi_helper_funcs = {
	.dpms = cdv_hdmi_dpms,
<<<<<<< HEAD
	.mode_fixup = cdv_hdmi_mode_fixup,
	.prepare = psb_intel_encoder_prepare,
	.mode_set = cdv_hdmi_mode_set,
	.commit = psb_intel_encoder_commit,
=======
	.prepare = gma_encoder_prepare,
	.mode_set = cdv_hdmi_mode_set,
	.commit = gma_encoder_commit,
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
};

static const struct drm_connector_helper_funcs
					cdv_hdmi_connector_helper_funcs = {
	.get_modes = cdv_hdmi_get_modes,
	.mode_valid = cdv_hdmi_mode_valid,
<<<<<<< HEAD
	.best_encoder = psb_intel_best_encoder,
=======
	.best_encoder = gma_best_encoder,
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
};

static const struct drm_connector_funcs cdv_hdmi_connector_funcs = {
	.dpms = drm_helper_connector_dpms,
<<<<<<< HEAD
	.save = cdv_hdmi_save,
	.restore = cdv_hdmi_restore,
=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	.detect = cdv_hdmi_detect,
	.fill_modes = drm_helper_probe_single_connector_modes,
	.set_property = cdv_hdmi_set_property,
	.destroy = cdv_hdmi_destroy,
};

void cdv_hdmi_init(struct drm_device *dev,
			struct psb_intel_mode_device *mode_dev, int reg)
{
<<<<<<< HEAD
	struct psb_intel_encoder *psb_intel_encoder;
	struct psb_intel_connector *psb_intel_connector;
	struct drm_connector *connector;
	struct drm_encoder *encoder;
	struct mid_intel_hdmi_priv *hdmi_priv;
	int ddc_bus;

	psb_intel_encoder = kzalloc(sizeof(struct psb_intel_encoder),
				    GFP_KERNEL);

	if (!psb_intel_encoder)
		return;

	psb_intel_connector = kzalloc(sizeof(struct psb_intel_connector),
				      GFP_KERNEL);

	if (!psb_intel_connector)
		goto err_connector;

	hdmi_priv = kzalloc(sizeof(struct mid_intel_hdmi_priv), GFP_KERNEL);

	if (!hdmi_priv)
		goto err_priv;

	connector = &psb_intel_connector->base;
	encoder = &psb_intel_encoder->base;
	drm_connector_init(dev, connector,
			   &cdv_hdmi_connector_funcs,
			   DRM_MODE_CONNECTOR_DVID);

	drm_encoder_init(dev, encoder, &psb_intel_lvds_enc_funcs,
			 DRM_MODE_ENCODER_TMDS);

	psb_intel_connector_attach_encoder(psb_intel_connector,
					   psb_intel_encoder);
	psb_intel_encoder->type = INTEL_OUTPUT_HDMI;
	hdmi_priv->hdmi_reg = reg;
	hdmi_priv->has_hdmi_sink = false;
	psb_intel_encoder->dev_priv = hdmi_priv;

	drm_encoder_helper_add(encoder, &cdv_hdmi_helper_funcs);
=======
	struct gma_encoder *gma_encoder;
	struct gma_connector *gma_connector;
	struct drm_connector *connector;
	struct mid_intel_hdmi_priv *hdmi_priv;
	struct gma_i2c_chan *ddc_bus;
	int ddc_reg;
	int ret;

	gma_encoder = kzalloc(sizeof(struct gma_encoder), GFP_KERNEL);
	if (!gma_encoder)
		return;

	gma_connector = kzalloc(sizeof(struct gma_connector), GFP_KERNEL);
	if (!gma_connector)
		goto err_free_encoder;

	hdmi_priv = kzalloc(sizeof(struct mid_intel_hdmi_priv), GFP_KERNEL);
	if (!hdmi_priv)
		goto err_free_connector;

	connector = &gma_connector->base;
	connector->polled = DRM_CONNECTOR_POLL_HPD;
	gma_connector->save = cdv_hdmi_save;
	gma_connector->restore = cdv_hdmi_restore;

	switch (reg) {
	case SDVOB:
		ddc_reg = GPIOE;
		gma_encoder->ddi_select = DDI0_SELECT;
		break;
	case SDVOC:
		ddc_reg = GPIOD;
		gma_encoder->ddi_select = DDI1_SELECT;
		break;
	default:
		DRM_ERROR("unknown reg 0x%x for HDMI\n", reg);
		goto err_free_hdmi_priv;
	}

	ddc_bus = gma_i2c_create(dev, ddc_reg,
				 (reg == SDVOB) ? "HDMIB" : "HDMIC");
	if (!ddc_bus) {
		dev_err(dev->dev, "No ddc adapter available!\n");
		goto err_free_hdmi_priv;
	}

	ret = drm_connector_init_with_ddc(dev, connector,
					  &cdv_hdmi_connector_funcs,
					  DRM_MODE_CONNECTOR_DVID,
					  &ddc_bus->base);
	if (ret)
		goto err_ddc_destroy;

	ret = drm_simple_encoder_init(dev, &gma_encoder->base,
				      DRM_MODE_ENCODER_TMDS);
	if (ret)
		goto err_connector_cleanup;

	gma_connector_attach_encoder(gma_connector, gma_encoder);
	gma_encoder->type = INTEL_OUTPUT_HDMI;
	hdmi_priv->hdmi_reg = reg;
	hdmi_priv->has_hdmi_sink = false;
	gma_encoder->dev_priv = hdmi_priv;

	drm_encoder_helper_add(&gma_encoder->base, &cdv_hdmi_helper_funcs);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	drm_connector_helper_add(connector,
				 &cdv_hdmi_connector_helper_funcs);
	connector->display_info.subpixel_order = SubPixelHorizontalRGB;
	connector->interlace_allowed = false;
	connector->doublescan_allowed = false;

<<<<<<< HEAD
	drm_connector_attach_property(connector,
				      dev->mode_config.scaling_mode_property,
				      DRM_MODE_SCALE_FULLSCREEN);

	switch (reg) {
	case SDVOB:
		ddc_bus = GPIOE;
		break;
	case SDVOC:
		ddc_bus = GPIOD;
		break;
	default:
		DRM_ERROR("unknown reg 0x%x for HDMI\n", reg);
		goto failed_ddc;
		break;
	}

	psb_intel_encoder->i2c_bus = psb_intel_i2c_create(dev,
				ddc_bus, (reg == SDVOB) ? "HDMIB" : "HDMIC");

	if (!psb_intel_encoder->i2c_bus) {
		dev_err(dev->dev, "No ddc adapter available!\n");
		goto failed_ddc;
	}

	hdmi_priv->hdmi_i2c_adapter =
				&(psb_intel_encoder->i2c_bus->adapter);
	hdmi_priv->dev = dev;
	drm_sysfs_connector_add(connector);
	return;

failed_ddc:
	drm_encoder_cleanup(encoder);
	drm_connector_cleanup(connector);
err_priv:
	kfree(psb_intel_connector);
err_connector:
	kfree(psb_intel_encoder);
=======
	drm_object_attach_property(&connector->base,
				      dev->mode_config.scaling_mode_property,
				      DRM_MODE_SCALE_FULLSCREEN);

	hdmi_priv->dev = dev;
	return;

err_connector_cleanup:
	drm_connector_cleanup(connector);
err_ddc_destroy:
	gma_i2c_destroy(ddc_bus);
err_free_hdmi_priv:
	kfree(hdmi_priv);
err_free_connector:
	kfree(gma_connector);
err_free_encoder:
	kfree(gma_encoder);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}
