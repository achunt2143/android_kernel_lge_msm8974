<<<<<<< HEAD
=======
// SPDX-License-Identifier: GPL-2.0-only
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
/*
 * Copyright (C) 2012 Red Hat
 *
 * based in parts on udlfb.c:
 * Copyright (C) 2009 Roberto De Ioris <roberto@unbit.it>
 * Copyright (C) 2009 Jaya Kumar <jayakumar.lkml@gmail.com>
 * Copyright (C) 2009 Bernie Thompson <bernie@plugable.com>
<<<<<<< HEAD

 * This file is subject to the terms and conditions of the GNU General Public
 * License v2. See the file COPYING in the main directory of this archive for
 * more details.
 */

#include "drmP.h"
#include "drm_crtc.h"
#include "drm_crtc_helper.h"
#include "udl_drv.h"

/*
 * All DisplayLink bulk operations start with 0xAF, followed by specific code
 * All operations are written to buffers which then later get sent to device
 */
static char *udl_set_register(char *buf, u8 reg, u8 val)
{
	*buf++ = 0xAF;
	*buf++ = 0x20;
	*buf++ = reg;
	*buf++ = val;
=======
 */

#include <linux/bitfield.h>

#include <drm/drm_atomic.h>
#include <drm/drm_atomic_helper.h>
#include <drm/drm_crtc_helper.h>
#include <drm/drm_damage_helper.h>
#include <drm/drm_drv.h>
#include <drm/drm_edid.h>
#include <drm/drm_fourcc.h>
#include <drm/drm_gem_atomic_helper.h>
#include <drm/drm_gem_framebuffer_helper.h>
#include <drm/drm_gem_shmem_helper.h>
#include <drm/drm_modeset_helper_vtables.h>
#include <drm/drm_probe_helper.h>
#include <drm/drm_vblank.h>

#include "udl_drv.h"
#include "udl_proto.h"

/*
 * All DisplayLink bulk operations start with 0xaf (UDL_MSG_BULK), followed by
 * a specific command code. All operations are written to a command buffer, which
 * the driver sends to the device.
 */
static char *udl_set_register(char *buf, u8 reg, u8 val)
{
	*buf++ = UDL_MSG_BULK;
	*buf++ = UDL_CMD_WRITEREG;
	*buf++ = reg;
	*buf++ = val;

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	return buf;
}

static char *udl_vidreg_lock(char *buf)
{
<<<<<<< HEAD
	return udl_set_register(buf, 0xFF, 0x00);
=======
	return udl_set_register(buf, UDL_REG_VIDREG, UDL_VIDREG_LOCK);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

static char *udl_vidreg_unlock(char *buf)
{
<<<<<<< HEAD
	return udl_set_register(buf, 0xFF, 0xFF);
}

/*
 * On/Off for driving the DisplayLink framebuffer to the display
 *  0x00 H and V sync on
 *  0x01 H and V sync off (screen blank but powered)
 *  0x07 DPMS powerdown (requires modeset to come back)
 */
static char *udl_enable_hvsync(char *buf, bool enable)
{
	if (enable)
		return udl_set_register(buf, 0x1F, 0x00);
	else
		return udl_set_register(buf, 0x1F, 0x07);
=======
	return udl_set_register(buf, UDL_REG_VIDREG, UDL_VIDREG_UNLOCK);
}

static char *udl_set_blank_mode(char *buf, u8 mode)
{
	return udl_set_register(buf, UDL_REG_BLANKMODE, mode);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

static char *udl_set_color_depth(char *buf, u8 selection)
{
<<<<<<< HEAD
	return udl_set_register(buf, 0x00, selection);
}

static char *udl_set_base16bpp(char *wrptr, u32 base)
{
	/* the base pointer is 16 bits wide, 0x20 is hi byte. */
	wrptr = udl_set_register(wrptr, 0x20, base >> 16);
	wrptr = udl_set_register(wrptr, 0x21, base >> 8);
	return udl_set_register(wrptr, 0x22, base);
=======
	return udl_set_register(buf, UDL_REG_COLORDEPTH, selection);
}

static char *udl_set_base16bpp(char *buf, u32 base)
{
	/* the base pointer is 24 bits wide, 0x20 is hi byte. */
	u8 reg20 = FIELD_GET(UDL_BASE_ADDR2_MASK, base);
	u8 reg21 = FIELD_GET(UDL_BASE_ADDR1_MASK, base);
	u8 reg22 = FIELD_GET(UDL_BASE_ADDR0_MASK, base);

	buf = udl_set_register(buf, UDL_REG_BASE16BPP_ADDR2, reg20);
	buf = udl_set_register(buf, UDL_REG_BASE16BPP_ADDR1, reg21);
	buf = udl_set_register(buf, UDL_REG_BASE16BPP_ADDR0, reg22);

	return buf;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

/*
 * DisplayLink HW has separate 16bpp and 8bpp framebuffers.
 * In 24bpp modes, the low 323 RGB bits go in the 8bpp framebuffer
 */
<<<<<<< HEAD
static char *udl_set_base8bpp(char *wrptr, u32 base)
{
	wrptr = udl_set_register(wrptr, 0x26, base >> 16);
	wrptr = udl_set_register(wrptr, 0x27, base >> 8);
	return udl_set_register(wrptr, 0x28, base);
=======
static char *udl_set_base8bpp(char *buf, u32 base)
{
	/* the base pointer is 24 bits wide, 0x26 is hi byte. */
	u8 reg26 = FIELD_GET(UDL_BASE_ADDR2_MASK, base);
	u8 reg27 = FIELD_GET(UDL_BASE_ADDR1_MASK, base);
	u8 reg28 = FIELD_GET(UDL_BASE_ADDR0_MASK, base);

	buf = udl_set_register(buf, UDL_REG_BASE8BPP_ADDR2, reg26);
	buf = udl_set_register(buf, UDL_REG_BASE8BPP_ADDR1, reg27);
	buf = udl_set_register(buf, UDL_REG_BASE8BPP_ADDR0, reg28);

	return buf;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

static char *udl_set_register_16(char *wrptr, u8 reg, u16 value)
{
	wrptr = udl_set_register(wrptr, reg, value >> 8);
	return udl_set_register(wrptr, reg+1, value);
}

/*
 * This is kind of weird because the controller takes some
 * register values in a different byte order than other registers.
 */
static char *udl_set_register_16be(char *wrptr, u8 reg, u16 value)
{
	wrptr = udl_set_register(wrptr, reg, value);
	return udl_set_register(wrptr, reg+1, value >> 8);
}

/*
 * LFSR is linear feedback shift register. The reason we have this is
 * because the display controller needs to minimize the clock depth of
 * various counters used in the display path. So this code reverses the
 * provided value into the lfsr16 value by counting backwards to get
 * the value that needs to be set in the hardware comparator to get the
 * same actual count. This makes sense once you read above a couple of
 * times and think about it from a hardware perspective.
 */
static u16 udl_lfsr16(u16 actual_count)
{
	u32 lv = 0xFFFF; /* This is the lfsr value that the hw starts with */

	while (actual_count--) {
		lv =	 ((lv << 1) |
			(((lv >> 15) ^ (lv >> 4) ^ (lv >> 2) ^ (lv >> 1)) & 1))
			& 0xFFFF;
	}

	return (u16) lv;
}

/*
 * This does LFSR conversion on the value that is to be written.
 * See LFSR explanation above for more detail.
 */
static char *udl_set_register_lfsr16(char *wrptr, u8 reg, u16 value)
{
	return udl_set_register_16(wrptr, reg, udl_lfsr16(value));
}

/*
<<<<<<< HEAD
 * This takes a standard fbdev screeninfo struct and all of its monitor mode
 * details and converts them into the DisplayLink equivalent register commands.
  ERR(vreg(dev,               0x00, (color_depth == 16) ? 0 : 1));
  ERR(vreg_lfsr16(dev,        0x01, xDisplayStart));
  ERR(vreg_lfsr16(dev,        0x03, xDisplayEnd));
  ERR(vreg_lfsr16(dev,        0x05, yDisplayStart));
  ERR(vreg_lfsr16(dev,        0x07, yDisplayEnd));
  ERR(vreg_lfsr16(dev,        0x09, xEndCount));
  ERR(vreg_lfsr16(dev,        0x0B, hSyncStart));
  ERR(vreg_lfsr16(dev,        0x0D, hSyncEnd));
  ERR(vreg_big_endian(dev,    0x0F, hPixels));
  ERR(vreg_lfsr16(dev,        0x11, yEndCount));
  ERR(vreg_lfsr16(dev,        0x13, vSyncStart));
  ERR(vreg_lfsr16(dev,        0x15, vSyncEnd));
  ERR(vreg_big_endian(dev,    0x17, vPixels));
  ERR(vreg_little_endian(dev, 0x1B, pixelClock5KHz));

  ERR(vreg(dev,               0x1F, 0));

  ERR(vbuf(dev, WRITE_VIDREG_UNLOCK, DSIZEOF(WRITE_VIDREG_UNLOCK)));
 */
static char *udl_set_vid_cmds(char *wrptr, struct drm_display_mode *mode)
{
	u16 xds, yds;
	u16 xde, yde;
	u16 yec;

	/* x display start */
	xds = mode->crtc_htotal - mode->crtc_hsync_start;
	wrptr = udl_set_register_lfsr16(wrptr, 0x01, xds);
	/* x display end */
	xde = xds + mode->crtc_hdisplay;
	wrptr = udl_set_register_lfsr16(wrptr, 0x03, xde);

	/* y display start */
	yds = mode->crtc_vtotal - mode->crtc_vsync_start;
	wrptr = udl_set_register_lfsr16(wrptr, 0x05, yds);
	/* y display end */
	yde = yds + mode->crtc_vdisplay;
	wrptr = udl_set_register_lfsr16(wrptr, 0x07, yde);

	/* x end count is active + blanking - 1 */
	wrptr = udl_set_register_lfsr16(wrptr, 0x09,
					mode->crtc_htotal - 1);

	/* libdlo hardcodes hsync start to 1 */
	wrptr = udl_set_register_lfsr16(wrptr, 0x0B, 1);

	/* hsync end is width of sync pulse + 1 */
	wrptr = udl_set_register_lfsr16(wrptr, 0x0D,
					mode->crtc_hsync_end - mode->crtc_hsync_start + 1);

	/* hpixels is active pixels */
	wrptr = udl_set_register_16(wrptr, 0x0F, mode->hdisplay);

	/* yendcount is vertical active + vertical blanking */
	yec = mode->crtc_vtotal;
	wrptr = udl_set_register_lfsr16(wrptr, 0x11, yec);

	/* libdlo hardcodes vsync start to 0 */
	wrptr = udl_set_register_lfsr16(wrptr, 0x13, 0);

	/* vsync end is width of vsync pulse */
	wrptr = udl_set_register_lfsr16(wrptr, 0x15, mode->crtc_vsync_end - mode->crtc_vsync_start);

	/* vpixels is active pixels */
	wrptr = udl_set_register_16(wrptr, 0x17, mode->crtc_vdisplay);

	wrptr = udl_set_register_16be(wrptr, 0x1B,
				      mode->clock / 5);

	return wrptr;
}

static int udl_crtc_write_mode_to_hw(struct drm_crtc *crtc)
{
	struct drm_device *dev = crtc->dev;
	struct udl_device *udl = dev->dev_private;
	struct urb *urb;
	char *buf;
	int retval;
=======
 * Takes a DRM display mode and converts it into the DisplayLink
 * equivalent register commands.
 */
static char *udl_set_display_mode(char *buf, struct drm_display_mode *mode)
{
	u16 reg01 = mode->crtc_htotal - mode->crtc_hsync_start;
	u16 reg03 = reg01 + mode->crtc_hdisplay;
	u16 reg05 = mode->crtc_vtotal - mode->crtc_vsync_start;
	u16 reg07 = reg05 + mode->crtc_vdisplay;
	u16 reg09 = mode->crtc_htotal - 1;
	u16 reg0b = 1; /* libdlo hardcodes hsync start to 1 */
	u16 reg0d = mode->crtc_hsync_end - mode->crtc_hsync_start + 1;
	u16 reg0f = mode->hdisplay;
	u16 reg11 = mode->crtc_vtotal;
	u16 reg13 = 0; /* libdlo hardcodes vsync start to 0 */
	u16 reg15 = mode->crtc_vsync_end - mode->crtc_vsync_start;
	u16 reg17 = mode->crtc_vdisplay;
	u16 reg1b = mode->clock / 5;

	buf = udl_set_register_lfsr16(buf, UDL_REG_XDISPLAYSTART, reg01);
	buf = udl_set_register_lfsr16(buf, UDL_REG_XDISPLAYEND, reg03);
	buf = udl_set_register_lfsr16(buf, UDL_REG_YDISPLAYSTART, reg05);
	buf = udl_set_register_lfsr16(buf, UDL_REG_YDISPLAYEND, reg07);
	buf = udl_set_register_lfsr16(buf, UDL_REG_XENDCOUNT, reg09);
	buf = udl_set_register_lfsr16(buf, UDL_REG_HSYNCSTART, reg0b);
	buf = udl_set_register_lfsr16(buf, UDL_REG_HSYNCEND, reg0d);
	buf = udl_set_register_16(buf, UDL_REG_HPIXELS, reg0f);
	buf = udl_set_register_lfsr16(buf, UDL_REG_YENDCOUNT, reg11);
	buf = udl_set_register_lfsr16(buf, UDL_REG_VSYNCSTART, reg13);
	buf = udl_set_register_lfsr16(buf, UDL_REG_VSYNCEND, reg15);
	buf = udl_set_register_16(buf, UDL_REG_VPIXELS, reg17);
	buf = udl_set_register_16be(buf, UDL_REG_PIXELCLOCK5KHZ, reg1b);

	return buf;
}

static char *udl_dummy_render(char *wrptr)
{
	*wrptr++ = UDL_MSG_BULK;
	*wrptr++ = UDL_CMD_WRITECOPY16;
	*wrptr++ = 0x00; /* from addr */
	*wrptr++ = 0x00;
	*wrptr++ = 0x00;
	*wrptr++ = 0x01; /* one pixel */
	*wrptr++ = 0x00; /* to address */
	*wrptr++ = 0x00;
	*wrptr++ = 0x00;
	return wrptr;
}

static long udl_log_cpp(unsigned int cpp)
{
	if (WARN_ON(!is_power_of_2(cpp)))
		return -EINVAL;
	return __ffs(cpp);
}

static int udl_handle_damage(struct drm_framebuffer *fb,
			     const struct iosys_map *map,
			     const struct drm_rect *clip)
{
	struct drm_device *dev = fb->dev;
	void *vaddr = map->vaddr; /* TODO: Use mapping abstraction properly */
	int i, ret;
	char *cmd;
	struct urb *urb;
	int log_bpp;

	ret = udl_log_cpp(fb->format->cpp[0]);
	if (ret < 0)
		return ret;
	log_bpp = ret;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	urb = udl_get_urb(dev);
	if (!urb)
		return -ENOMEM;
<<<<<<< HEAD

	buf = (char *)urb->transfer_buffer;

	memcpy(buf, udl->mode_buf, udl->mode_buf_len);
	retval = udl_submit_urb(dev, urb, udl->mode_buf_len);
	DRM_INFO("write mode info %d\n", udl->mode_buf_len);
	return retval;
}


static void udl_crtc_dpms(struct drm_crtc *crtc, int mode)
{
	struct drm_device *dev = crtc->dev;
	struct udl_device *udl = dev->dev_private;
	int retval;

	if (mode == DRM_MODE_DPMS_OFF) {
		char *buf;
		struct urb *urb;
		urb = udl_get_urb(dev);
		if (!urb)
			return;

		buf = (char *)urb->transfer_buffer;
		buf = udl_vidreg_lock(buf);
		buf = udl_enable_hvsync(buf, false);
		buf = udl_vidreg_unlock(buf);

		retval = udl_submit_urb(dev, urb, buf - (char *)
					urb->transfer_buffer);
	} else {
		if (udl->mode_buf_len == 0) {
			DRM_ERROR("Trying to enable DPMS with no mode\n");
			return;
		}
		udl_crtc_write_mode_to_hw(crtc);
	}

}

static bool udl_crtc_mode_fixup(struct drm_crtc *crtc,
				  struct drm_display_mode *mode,
				  struct drm_display_mode *adjusted_mode)

{
	return true;
}

#if 0
static int
udl_pipe_set_base_atomic(struct drm_crtc *crtc, struct drm_framebuffer *fb,
			   int x, int y, enum mode_set_atomic state)
{
	return 0;
}

static int
udl_pipe_set_base(struct drm_crtc *crtc, int x, int y,
		    struct drm_framebuffer *old_fb)
{
	return 0;
}
#endif

static int udl_crtc_mode_set(struct drm_crtc *crtc,
			       struct drm_display_mode *mode,
			       struct drm_display_mode *adjusted_mode,
			       int x, int y,
			       struct drm_framebuffer *old_fb)

{
	struct drm_device *dev = crtc->dev;
	struct udl_framebuffer *ufb = to_udl_fb(crtc->fb);
	struct udl_device *udl = dev->dev_private;
	char *buf;
	char *wrptr;
	int color_depth = 0;

	buf = (char *)udl->mode_buf;

	/* for now we just clip 24 -> 16 - if we fix that fix this */
	/*if  (crtc->fb->bits_per_pixel != 16)
	  color_depth = 1; */

	/* This first section has to do with setting the base address on the
	* controller * associated with the display. There are 2 base
	* pointers, currently, we only * use the 16 bpp segment.
	*/
	wrptr = udl_vidreg_lock(buf);
	wrptr = udl_set_color_depth(wrptr, color_depth);
	/* set base for 16bpp segment to 0 */
	wrptr = udl_set_base16bpp(wrptr, 0);
	/* set base for 8bpp segment to end of fb */
	wrptr = udl_set_base8bpp(wrptr, 2 * mode->vdisplay * mode->hdisplay);

	wrptr = udl_set_vid_cmds(wrptr, adjusted_mode);
	wrptr = udl_enable_hvsync(wrptr, true);
	wrptr = udl_vidreg_unlock(wrptr);

	ufb->active_16 = true;
	if (old_fb) {
		struct udl_framebuffer *uold_fb = to_udl_fb(old_fb);
		uold_fb->active_16 = false;
	}
	udl->mode_buf_len = wrptr - buf;

	/* damage all of it */
	udl_handle_damage(ufb, 0, 0, ufb->base.width, ufb->base.height);
	return 0;
}


static void udl_crtc_disable(struct drm_crtc *crtc)
{


}

static void udl_crtc_destroy(struct drm_crtc *crtc)
{
	drm_crtc_cleanup(crtc);
	kfree(crtc);
}

static void udl_load_lut(struct drm_crtc *crtc)
{
}

static void udl_crtc_prepare(struct drm_crtc *crtc)
{
}

static void udl_crtc_commit(struct drm_crtc *crtc)
{
	udl_crtc_dpms(crtc, DRM_MODE_DPMS_ON);
}

static struct drm_crtc_helper_funcs udl_helper_funcs = {
	.dpms = udl_crtc_dpms,
	.mode_fixup = udl_crtc_mode_fixup,
	.mode_set = udl_crtc_mode_set,
	.prepare = udl_crtc_prepare,
	.commit = udl_crtc_commit,
	.disable = udl_crtc_disable,
	.load_lut = udl_load_lut,
};

static const struct drm_crtc_funcs udl_crtc_funcs = {
	.set_config = drm_crtc_helper_set_config,
	.destroy = udl_crtc_destroy,
};

int udl_crtc_init(struct drm_device *dev)
{
	struct drm_crtc *crtc;

	crtc = kzalloc(sizeof(struct drm_crtc) + sizeof(struct drm_connector *), GFP_KERNEL);
	if (crtc == NULL)
		return -ENOMEM;

	drm_crtc_init(dev, crtc, &udl_crtc_funcs);
	drm_crtc_helper_add(crtc, &udl_helper_funcs);
=======
	cmd = urb->transfer_buffer;

	for (i = clip->y1; i < clip->y2; i++) {
		const int line_offset = fb->pitches[0] * i;
		const int byte_offset = line_offset + (clip->x1 << log_bpp);
		const int dev_byte_offset = (fb->width * i + clip->x1) << log_bpp;
		const int byte_width = drm_rect_width(clip) << log_bpp;
		ret = udl_render_hline(dev, log_bpp, &urb, (char *)vaddr,
				       &cmd, byte_offset, dev_byte_offset,
				       byte_width);
		if (ret)
			return ret;
	}

	if (cmd > (char *)urb->transfer_buffer) {
		/* Send partial buffer remaining before exiting */
		int len;
		if (cmd < (char *)urb->transfer_buffer + urb->transfer_buffer_length)
			*cmd++ = UDL_MSG_BULK;
		len = cmd - (char *)urb->transfer_buffer;
		ret = udl_submit_urb(dev, urb, len);
	} else {
		udl_urb_completion(urb);
	}

	return 0;
}

/*
 * Primary plane
 */

static const uint32_t udl_primary_plane_formats[] = {
	DRM_FORMAT_RGB565,
	DRM_FORMAT_XRGB8888,
};

static const uint64_t udl_primary_plane_fmtmods[] = {
	DRM_FORMAT_MOD_LINEAR,
	DRM_FORMAT_MOD_INVALID
};

static int udl_primary_plane_helper_atomic_check(struct drm_plane *plane,
						 struct drm_atomic_state *state)
{
	struct drm_plane_state *new_plane_state = drm_atomic_get_new_plane_state(state, plane);
	struct drm_crtc *new_crtc = new_plane_state->crtc;
	struct drm_crtc_state *new_crtc_state = NULL;

	if (new_crtc)
		new_crtc_state = drm_atomic_get_new_crtc_state(state, new_crtc);

	return drm_atomic_helper_check_plane_state(new_plane_state, new_crtc_state,
						   DRM_PLANE_NO_SCALING,
						   DRM_PLANE_NO_SCALING,
						   false, false);
}

static void udl_primary_plane_helper_atomic_update(struct drm_plane *plane,
						   struct drm_atomic_state *state)
{
	struct drm_device *dev = plane->dev;
	struct drm_plane_state *plane_state = drm_atomic_get_new_plane_state(state, plane);
	struct drm_shadow_plane_state *shadow_plane_state = to_drm_shadow_plane_state(plane_state);
	struct drm_framebuffer *fb = plane_state->fb;
	struct drm_plane_state *old_plane_state = drm_atomic_get_old_plane_state(state, plane);
	struct drm_atomic_helper_damage_iter iter;
	struct drm_rect damage;
	int ret, idx;

	if (!fb)
		return; /* no framebuffer; plane is disabled */

	ret = drm_gem_fb_begin_cpu_access(fb, DMA_FROM_DEVICE);
	if (ret)
		return;

	if (!drm_dev_enter(dev, &idx))
		goto out_drm_gem_fb_end_cpu_access;

	drm_atomic_helper_damage_iter_init(&iter, old_plane_state, plane_state);
	drm_atomic_for_each_plane_damage(&iter, &damage) {
		udl_handle_damage(fb, &shadow_plane_state->data[0], &damage);
	}

	drm_dev_exit(idx);

out_drm_gem_fb_end_cpu_access:
	drm_gem_fb_end_cpu_access(fb, DMA_FROM_DEVICE);
}

static const struct drm_plane_helper_funcs udl_primary_plane_helper_funcs = {
	DRM_GEM_SHADOW_PLANE_HELPER_FUNCS,
	.atomic_check = udl_primary_plane_helper_atomic_check,
	.atomic_update = udl_primary_plane_helper_atomic_update,
};

static const struct drm_plane_funcs udl_primary_plane_funcs = {
	.update_plane = drm_atomic_helper_update_plane,
	.disable_plane = drm_atomic_helper_disable_plane,
	.destroy = drm_plane_cleanup,
	DRM_GEM_SHADOW_PLANE_FUNCS,
};

/*
 * CRTC
 */

static void udl_crtc_helper_atomic_enable(struct drm_crtc *crtc, struct drm_atomic_state *state)
{
	struct drm_device *dev = crtc->dev;
	struct drm_crtc_state *crtc_state = drm_atomic_get_new_crtc_state(state, crtc);
	struct drm_display_mode *mode = &crtc_state->mode;
	struct urb *urb;
	char *buf;
	int idx;

	if (!drm_dev_enter(dev, &idx))
		return;

	urb = udl_get_urb(dev);
	if (!urb)
		goto out;

	buf = (char *)urb->transfer_buffer;
	buf = udl_vidreg_lock(buf);
	buf = udl_set_color_depth(buf, UDL_COLORDEPTH_16BPP);
	/* set base for 16bpp segment to 0 */
	buf = udl_set_base16bpp(buf, 0);
	/* set base for 8bpp segment to end of fb */
	buf = udl_set_base8bpp(buf, 2 * mode->vdisplay * mode->hdisplay);
	buf = udl_set_display_mode(buf, mode);
	buf = udl_set_blank_mode(buf, UDL_BLANKMODE_ON);
	buf = udl_vidreg_unlock(buf);
	buf = udl_dummy_render(buf);

	udl_submit_urb(dev, urb, buf - (char *)urb->transfer_buffer);

out:
	drm_dev_exit(idx);
}

static void udl_crtc_helper_atomic_disable(struct drm_crtc *crtc, struct drm_atomic_state *state)
{
	struct drm_device *dev = crtc->dev;
	struct urb *urb;
	char *buf;
	int idx;

	if (!drm_dev_enter(dev, &idx))
		return;

	urb = udl_get_urb(dev);
	if (!urb)
		goto out;

	buf = (char *)urb->transfer_buffer;
	buf = udl_vidreg_lock(buf);
	buf = udl_set_blank_mode(buf, UDL_BLANKMODE_POWERDOWN);
	buf = udl_vidreg_unlock(buf);
	buf = udl_dummy_render(buf);

	udl_submit_urb(dev, urb, buf - (char *)urb->transfer_buffer);

out:
	drm_dev_exit(idx);
}

static const struct drm_crtc_helper_funcs udl_crtc_helper_funcs = {
	.atomic_check = drm_crtc_helper_atomic_check,
	.atomic_enable = udl_crtc_helper_atomic_enable,
	.atomic_disable = udl_crtc_helper_atomic_disable,
};

static const struct drm_crtc_funcs udl_crtc_funcs = {
	.reset = drm_atomic_helper_crtc_reset,
	.destroy = drm_crtc_cleanup,
	.set_config = drm_atomic_helper_set_config,
	.page_flip = drm_atomic_helper_page_flip,
	.atomic_duplicate_state = drm_atomic_helper_crtc_duplicate_state,
	.atomic_destroy_state = drm_atomic_helper_crtc_destroy_state,
};

/*
 * Encoder
 */

static const struct drm_encoder_funcs udl_encoder_funcs = {
	.destroy = drm_encoder_cleanup,
};

/*
 * Connector
 */

static int udl_connector_helper_get_modes(struct drm_connector *connector)
{
	struct udl_connector *udl_connector = to_udl_connector(connector);

	drm_connector_update_edid_property(connector, udl_connector->edid);
	if (udl_connector->edid)
		return drm_add_edid_modes(connector, udl_connector->edid);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	return 0;
}

<<<<<<< HEAD
static const struct drm_mode_config_funcs udl_mode_funcs = {
	.fb_create = udl_fb_user_fb_create,
	.output_poll_changed = NULL,
=======
static const struct drm_connector_helper_funcs udl_connector_helper_funcs = {
	.get_modes = udl_connector_helper_get_modes,
};

static int udl_get_edid_block(void *data, u8 *buf, unsigned int block, size_t len)
{
	struct udl_device *udl = data;
	struct drm_device *dev = &udl->drm;
	struct usb_device *udev = udl_to_usb_device(udl);
	u8 *read_buff;
	int ret;
	size_t i;

	read_buff = kmalloc(2, GFP_KERNEL);
	if (!read_buff)
		return -ENOMEM;

	for (i = 0; i < len; i++) {
		int bval = (i + block * EDID_LENGTH) << 8;

		ret = usb_control_msg(udev, usb_rcvctrlpipe(udev, 0),
				      0x02, (0x80 | (0x02 << 5)), bval,
				      0xA1, read_buff, 2, USB_CTRL_GET_TIMEOUT);
		if (ret < 0) {
			drm_err(dev, "Read EDID byte %zu failed err %x\n", i, ret);
			goto err_kfree;
		} else if (ret < 1) {
			ret = -EIO;
			drm_err(dev, "Read EDID byte %zu failed\n", i);
			goto err_kfree;
		}

		buf[i] = read_buff[1];
	}

	kfree(read_buff);

	return 0;

err_kfree:
	kfree(read_buff);
	return ret;
}

static enum drm_connector_status udl_connector_detect(struct drm_connector *connector, bool force)
{
	struct drm_device *dev = connector->dev;
	struct udl_device *udl = to_udl(dev);
	struct udl_connector *udl_connector = to_udl_connector(connector);
	enum drm_connector_status status = connector_status_disconnected;
	int idx;

	/* cleanup previous EDID */
	kfree(udl_connector->edid);
	udl_connector->edid = NULL;

	if (!drm_dev_enter(dev, &idx))
		return connector_status_disconnected;

	udl_connector->edid = drm_do_get_edid(connector, udl_get_edid_block, udl);
	if (udl_connector->edid)
		status = connector_status_connected;

	drm_dev_exit(idx);

	return status;
}

static void udl_connector_destroy(struct drm_connector *connector)
{
	struct udl_connector *udl_connector = to_udl_connector(connector);

	drm_connector_cleanup(connector);
	kfree(udl_connector->edid);
	kfree(udl_connector);
}

static const struct drm_connector_funcs udl_connector_funcs = {
	.reset = drm_atomic_helper_connector_reset,
	.detect = udl_connector_detect,
	.fill_modes = drm_helper_probe_single_connector_modes,
	.destroy = udl_connector_destroy,
	.atomic_duplicate_state = drm_atomic_helper_connector_duplicate_state,
	.atomic_destroy_state = drm_atomic_helper_connector_destroy_state,
};

struct drm_connector *udl_connector_init(struct drm_device *dev)
{
	struct udl_connector *udl_connector;
	struct drm_connector *connector;
	int ret;

	udl_connector = kzalloc(sizeof(*udl_connector), GFP_KERNEL);
	if (!udl_connector)
		return ERR_PTR(-ENOMEM);

	connector = &udl_connector->connector;
	ret = drm_connector_init(dev, connector, &udl_connector_funcs, DRM_MODE_CONNECTOR_VGA);
	if (ret)
		goto err_kfree;

	drm_connector_helper_add(connector, &udl_connector_helper_funcs);

	connector->polled = DRM_CONNECTOR_POLL_HPD |
			    DRM_CONNECTOR_POLL_CONNECT |
			    DRM_CONNECTOR_POLL_DISCONNECT;

	return connector;

err_kfree:
	kfree(udl_connector);
	return ERR_PTR(ret);
}

/*
 * Modesetting
 */

static enum drm_mode_status udl_mode_config_mode_valid(struct drm_device *dev,
						       const struct drm_display_mode *mode)
{
	struct udl_device *udl = to_udl(dev);

	if (udl->sku_pixel_limit) {
		if (mode->vdisplay * mode->hdisplay > udl->sku_pixel_limit)
			return MODE_MEM;
	}

	return MODE_OK;
}

static const struct drm_mode_config_funcs udl_mode_config_funcs = {
	.fb_create = drm_gem_fb_create_with_dirty,
	.mode_valid = udl_mode_config_mode_valid,
	.atomic_check  = drm_atomic_helper_check,
	.atomic_commit = drm_atomic_helper_commit,
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
};

int udl_modeset_init(struct drm_device *dev)
{
<<<<<<< HEAD
	struct drm_encoder *encoder;
	drm_mode_config_init(dev);

	dev->mode_config.min_width = 640;
	dev->mode_config.min_height = 480;

	dev->mode_config.max_width = 2048;
	dev->mode_config.max_height = 2048;

	dev->mode_config.prefer_shadow = 0;
	dev->mode_config.preferred_depth = 24;

	dev->mode_config.funcs = (void *)&udl_mode_funcs;

	drm_mode_create_dirty_info_property(dev);

	udl_crtc_init(dev);

	encoder = udl_encoder_init(dev);

	udl_connector_init(dev, encoder);

	return 0;
}

void udl_modeset_cleanup(struct drm_device *dev)
{
	drm_mode_config_cleanup(dev);
}
=======
	struct udl_device *udl = to_udl(dev);
	struct drm_plane *primary_plane;
	struct drm_crtc *crtc;
	struct drm_encoder *encoder;
	struct drm_connector *connector;
	int ret;

	ret = drmm_mode_config_init(dev);
	if (ret)
		return ret;

	dev->mode_config.min_width = 640;
	dev->mode_config.min_height = 480;
	dev->mode_config.max_width = 2048;
	dev->mode_config.max_height = 2048;
	dev->mode_config.preferred_depth = 16;
	dev->mode_config.funcs = &udl_mode_config_funcs;

	primary_plane = &udl->primary_plane;
	ret = drm_universal_plane_init(dev, primary_plane, 0,
				       &udl_primary_plane_funcs,
				       udl_primary_plane_formats,
				       ARRAY_SIZE(udl_primary_plane_formats),
				       udl_primary_plane_fmtmods,
				       DRM_PLANE_TYPE_PRIMARY, NULL);
	if (ret)
		return ret;
	drm_plane_helper_add(primary_plane, &udl_primary_plane_helper_funcs);
	drm_plane_enable_fb_damage_clips(primary_plane);

	crtc = &udl->crtc;
	ret = drm_crtc_init_with_planes(dev, crtc, primary_plane, NULL,
					&udl_crtc_funcs, NULL);
	if (ret)
		return ret;
	drm_crtc_helper_add(crtc, &udl_crtc_helper_funcs);

	encoder = &udl->encoder;
	ret = drm_encoder_init(dev, encoder, &udl_encoder_funcs, DRM_MODE_ENCODER_DAC, NULL);
	if (ret)
		return ret;
	encoder->possible_crtcs = drm_crtc_mask(crtc);

	connector = udl_connector_init(dev);
	if (IS_ERR(connector))
		return PTR_ERR(connector);
	ret = drm_connector_attach_encoder(connector, encoder);
	if (ret)
		return ret;

	drm_mode_config_reset(dev);

	return 0;
}
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
