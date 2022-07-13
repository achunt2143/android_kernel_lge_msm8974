/*
 * Copyright © 2006 Keith Packard
 * Copyright © 2007-2008 Dave Airlie
 * Copyright © 2007-2008 Intel Corporation
 *   Jesse Barnes <jesse.barnes@intel.com>
 *
 * Permission is hereby granted, free of charge, to any person obtaining a
 * copy of this software and associated documentation files (the "Software"),
 * to deal in the Software without restriction, including without limitation
 * the rights to use, copy, modify, merge, publish, distribute, sublicense,
 * and/or sell copies of the Software, and to permit persons to whom the
 * Software is furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.  IN NO EVENT SHALL
 * THE COPYRIGHT HOLDER(S) OR AUTHOR(S) BE LIABLE FOR ANY CLAIM, DAMAGES OR
 * OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE,
 * ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
 * OTHER DEALINGS IN THE SOFTWARE.
 */
#ifndef __DRM_CRTC_H__
#define __DRM_CRTC_H__

<<<<<<< HEAD
#include <linux/i2c.h>
#include <linux/spinlock.h>
#include <linux/types.h>
#include <linux/idr.h>
#include <linux/fb.h>

#include <drm/drm_fourcc.h>

struct drm_device;
struct drm_mode_set;
struct drm_framebuffer;


#define DRM_MODE_OBJECT_CRTC 0xcccccccc
#define DRM_MODE_OBJECT_CONNECTOR 0xc0c0c0c0
#define DRM_MODE_OBJECT_ENCODER 0xe0e0e0e0
#define DRM_MODE_OBJECT_MODE 0xdededede
#define DRM_MODE_OBJECT_PROPERTY 0xb0b0b0b0
#define DRM_MODE_OBJECT_FB 0xfbfbfbfb
#define DRM_MODE_OBJECT_BLOB 0xbbbbbbbb
#define DRM_MODE_OBJECT_PLANE 0xeeeeeeee

struct drm_mode_object {
	uint32_t id;
	uint32_t type;
};

/*
 * Note on terminology:  here, for brevity and convenience, we refer to connector
 * control chips as 'CRTCs'.  They can control any type of connector, VGA, LVDS,
 * DVI, etc.  And 'screen' refers to the whole of the visible display, which
 * may span multiple monitors (and therefore multiple CRTC and connector
 * structures).
 */

enum drm_mode_status {
    MODE_OK	= 0,	/* Mode OK */
    MODE_HSYNC,		/* hsync out of range */
    MODE_VSYNC,		/* vsync out of range */
    MODE_H_ILLEGAL,	/* mode has illegal horizontal timings */
    MODE_V_ILLEGAL,	/* mode has illegal horizontal timings */
    MODE_BAD_WIDTH,	/* requires an unsupported linepitch */
    MODE_NOMODE,	/* no mode with a matching name */
    MODE_NO_INTERLACE,	/* interlaced mode not supported */
    MODE_NO_DBLESCAN,	/* doublescan mode not supported */
    MODE_NO_VSCAN,	/* multiscan mode not supported */
    MODE_MEM,		/* insufficient video memory */
    MODE_VIRTUAL_X,	/* mode width too large for specified virtual size */
    MODE_VIRTUAL_Y,	/* mode height too large for specified virtual size */
    MODE_MEM_VIRT,	/* insufficient video memory given virtual size */
    MODE_NOCLOCK,	/* no fixed clock available */
    MODE_CLOCK_HIGH,	/* clock required is too high */
    MODE_CLOCK_LOW,	/* clock required is too low */
    MODE_CLOCK_RANGE,	/* clock/mode isn't in a ClockRange */
    MODE_BAD_HVALUE,	/* horizontal timing was out of range */
    MODE_BAD_VVALUE,	/* vertical timing was out of range */
    MODE_BAD_VSCAN,	/* VScan value out of range */
    MODE_HSYNC_NARROW,	/* horizontal sync too narrow */
    MODE_HSYNC_WIDE,	/* horizontal sync too wide */
    MODE_HBLANK_NARROW,	/* horizontal blanking too narrow */
    MODE_HBLANK_WIDE,	/* horizontal blanking too wide */
    MODE_VSYNC_NARROW,	/* vertical sync too narrow */
    MODE_VSYNC_WIDE,	/* vertical sync too wide */
    MODE_VBLANK_NARROW,	/* vertical blanking too narrow */
    MODE_VBLANK_WIDE,	/* vertical blanking too wide */
    MODE_PANEL,         /* exceeds panel dimensions */
    MODE_INTERLACE_WIDTH, /* width too large for interlaced mode */
    MODE_ONE_WIDTH,     /* only one width is supported */
    MODE_ONE_HEIGHT,    /* only one height is supported */
    MODE_ONE_SIZE,      /* only one resolution is supported */
    MODE_NO_REDUCED,    /* monitor doesn't accept reduced blanking */
    MODE_UNVERIFIED = -3, /* mode needs to reverified */
    MODE_BAD = -2,	/* unspecified reason */
    MODE_ERROR	= -1	/* error condition */
};

#define DRM_MODE_TYPE_CLOCK_CRTC_C (DRM_MODE_TYPE_CLOCK_C | \
				    DRM_MODE_TYPE_CRTC_C)

#define DRM_MODE(nm, t, c, hd, hss, hse, ht, hsk, vd, vss, vse, vt, vs, f) \
	.name = nm, .status = 0, .type = (t), .clock = (c), \
	.hdisplay = (hd), .hsync_start = (hss), .hsync_end = (hse), \
	.htotal = (ht), .hskew = (hsk), .vdisplay = (vd), \
	.vsync_start = (vss), .vsync_end = (vse), .vtotal = (vt), \
	.vscan = (vs), .flags = (f), .vrefresh = 0

#define CRTC_INTERLACE_HALVE_V 0x1 /* halve V values for interlacing */

struct drm_display_mode {
	/* Header */
	struct list_head head;
	struct drm_mode_object base;

	char name[DRM_DISPLAY_MODE_LEN];

	enum drm_mode_status status;
	unsigned int type;

	/* Proposed mode values */
	int clock;		/* in kHz */
	int hdisplay;
	int hsync_start;
	int hsync_end;
	int htotal;
	int hskew;
	int vdisplay;
	int vsync_start;
	int vsync_end;
	int vtotal;
	int vscan;
	unsigned int flags;

	/* Addressable image size (may be 0 for projectors, etc.) */
	int width_mm;
	int height_mm;

	/* Actual mode we give to hw */
	int clock_index;
	int synth_clock;
	int crtc_hdisplay;
	int crtc_hblank_start;
	int crtc_hblank_end;
	int crtc_hsync_start;
	int crtc_hsync_end;
	int crtc_htotal;
	int crtc_hskew;
	int crtc_vdisplay;
	int crtc_vblank_start;
	int crtc_vblank_end;
	int crtc_vsync_start;
	int crtc_vsync_end;
	int crtc_vtotal;
	int crtc_hadjusted;
	int crtc_vadjusted;

	/* Driver private mode info */
	int private_size;
	int *private;
	int private_flags;

	int vrefresh;		/* in Hz */
	int hsync;		/* in kHz */
};

enum drm_connector_status {
	connector_status_connected = 1,
	connector_status_disconnected = 2,
	connector_status_unknown = 3,
};

enum subpixel_order {
	SubPixelUnknown = 0,
	SubPixelHorizontalRGB,
	SubPixelHorizontalBGR,
	SubPixelVerticalRGB,
	SubPixelVerticalBGR,
	SubPixelNone,
};

#define DRM_COLOR_FORMAT_RGB444		(1<<0)
#define DRM_COLOR_FORMAT_YCRCB444	(1<<1)
#define DRM_COLOR_FORMAT_YCRCB422	(1<<2)
/*
 * Describes a given display (e.g. CRT or flat panel) and its limitations.
 */
struct drm_display_info {
	char name[DRM_DISPLAY_INFO_LEN];

	/* Physical size */
        unsigned int width_mm;
	unsigned int height_mm;

	/* Clock limits FIXME: storage format */
	unsigned int min_vfreq, max_vfreq;
	unsigned int min_hfreq, max_hfreq;
	unsigned int pixel_clock;
	unsigned int bpc;

	enum subpixel_order subpixel_order;
	u32 color_formats;

	u8 cea_rev;

	char *raw_edid; /* if any */
};

struct drm_framebuffer_funcs {
	void (*destroy)(struct drm_framebuffer *framebuffer);
	int (*create_handle)(struct drm_framebuffer *fb,
			     struct drm_file *file_priv,
			     unsigned int *handle);
	/**
	 * Optinal callback for the dirty fb ioctl.
	 *
	 * Userspace can notify the driver via this callback
	 * that a area of the framebuffer has changed and should
	 * be flushed to the display hardware.
	 *
	 * See documentation in drm_mode.h for the struct
	 * drm_mode_fb_dirty_cmd for more information as all
	 * the semantics and arguments have a one to one mapping
	 * on this function.
	 */
	int (*dirty)(struct drm_framebuffer *framebuffer,
		     struct drm_file *file_priv, unsigned flags,
		     unsigned color, struct drm_clip_rect *clips,
		     unsigned num_clips);
};

struct drm_framebuffer {
	struct drm_device *dev;
	struct list_head head;
	struct drm_mode_object base;
	const struct drm_framebuffer_funcs *funcs;
	unsigned int pitches[4];
	unsigned int offsets[4];
	unsigned int width;
	unsigned int height;
	/* depth can be 15 or 16 */
	unsigned int depth;
	int bits_per_pixel;
	int flags;
	uint32_t pixel_format; /* fourcc format */
	struct list_head filp_head;
	/* if you are using the helper */
	void *helper_private;
};

struct drm_property_blob {
	struct drm_mode_object base;
	struct list_head head;
	unsigned int length;
	unsigned char data[];
};

struct drm_property_enum {
	uint64_t value;
	struct list_head head;
	char name[DRM_PROP_NAME_LEN];
};

struct drm_property {
	struct list_head head;
	struct drm_mode_object base;
	uint32_t flags;
	char name[DRM_PROP_NAME_LEN];
	uint32_t num_values;
	uint64_t *values;

	struct list_head enum_blob_list;
};

struct drm_crtc;
struct drm_connector;
struct drm_encoder;
struct drm_pending_vblank_event;
struct drm_plane;

/**
 * drm_crtc_funcs - control CRTCs for a given device
 * @reset: reset CRTC after state has been invalidate (e.g. resume)
 * @dpms: control display power levels
 * @save: save CRTC state
 * @resore: restore CRTC state
 * @lock: lock the CRTC
 * @unlock: unlock the CRTC
 * @shadow_allocate: allocate shadow pixmap
 * @shadow_create: create shadow pixmap for rotation support
 * @shadow_destroy: free shadow pixmap
 * @mode_fixup: fixup proposed mode
 * @mode_set: set the desired mode on the CRTC
 * @gamma_set: specify color ramp for CRTC
 * @destroy: deinit and free object.
=======
#include <linux/spinlock.h>
#include <linux/types.h>
#include <drm/drm_modeset_lock.h>
#include <drm/drm_mode_object.h>
#include <drm/drm_modes.h>
#include <drm/drm_device.h>
#include <drm/drm_plane.h>
#include <drm/drm_debugfs_crc.h>
#include <drm/drm_mode_config.h>

struct drm_connector;
struct drm_device;
struct drm_framebuffer;
struct drm_mode_set;
struct drm_file;
struct drm_printer;
struct drm_self_refresh_data;
struct device_node;
struct edid;

static inline int64_t U642I64(uint64_t val)
{
	return (int64_t)*((int64_t *)&val);
}
static inline uint64_t I642U64(int64_t val)
{
	return (uint64_t)*((uint64_t *)&val);
}

struct drm_crtc;
struct drm_pending_vblank_event;
struct drm_plane;
struct drm_bridge;
struct drm_atomic_state;

struct drm_crtc_helper_funcs;
struct drm_plane_helper_funcs;

/**
 * struct drm_crtc_state - mutable CRTC state
 *
 * Note that the distinction between @enable and @active is rather subtle:
 * Flipping @active while @enable is set without changing anything else may
 * never return in a failure from the &drm_mode_config_funcs.atomic_check
 * callback. Userspace assumes that a DPMS On will always succeed. In other
 * words: @enable controls resource assignment, @active controls the actual
 * hardware state.
 *
 * The three booleans active_changed, connectors_changed and mode_changed are
 * intended to indicate whether a full modeset is needed, rather than strictly
 * describing what has changed in a commit. See also:
 * drm_atomic_crtc_needs_modeset()
 */
struct drm_crtc_state {
	/** @crtc: backpointer to the CRTC */
	struct drm_crtc *crtc;

	/**
	 * @enable: Whether the CRTC should be enabled, gates all other state.
	 * This controls reservations of shared resources. Actual hardware state
	 * is controlled by @active.
	 */
	bool enable;

	/**
	 * @active: Whether the CRTC is actively displaying (used for DPMS).
	 * Implies that @enable is set. The driver must not release any shared
	 * resources if @active is set to false but @enable still true, because
	 * userspace expects that a DPMS ON always succeeds.
	 *
	 * Hence drivers must not consult @active in their various
	 * &drm_mode_config_funcs.atomic_check callback to reject an atomic
	 * commit. They can consult it to aid in the computation of derived
	 * hardware state, since even in the DPMS OFF state the display hardware
	 * should be as much powered down as when the CRTC is completely
	 * disabled through setting @enable to false.
	 */
	bool active;

	/**
	 * @planes_changed: Planes on this crtc are updated. Used by the atomic
	 * helpers and drivers to steer the atomic commit control flow.
	 */
	bool planes_changed : 1;

	/**
	 * @mode_changed: @mode or @enable has been changed. Used by the atomic
	 * helpers and drivers to steer the atomic commit control flow. See also
	 * drm_atomic_crtc_needs_modeset().
	 *
	 * Drivers are supposed to set this for any CRTC state changes that
	 * require a full modeset. They can also reset it to false if e.g. a
	 * @mode change can be done without a full modeset by only changing
	 * scaler settings.
	 */
	bool mode_changed : 1;

	/**
	 * @active_changed: @active has been toggled. Used by the atomic
	 * helpers and drivers to steer the atomic commit control flow. See also
	 * drm_atomic_crtc_needs_modeset().
	 */
	bool active_changed : 1;

	/**
	 * @connectors_changed: Connectors to this crtc have been updated,
	 * either in their state or routing. Used by the atomic
	 * helpers and drivers to steer the atomic commit control flow. See also
	 * drm_atomic_crtc_needs_modeset().
	 *
	 * Drivers are supposed to set this as-needed from their own atomic
	 * check code, e.g. from &drm_encoder_helper_funcs.atomic_check
	 */
	bool connectors_changed : 1;
	/**
	 * @zpos_changed: zpos values of planes on this crtc have been updated.
	 * Used by the atomic helpers and drivers to steer the atomic commit
	 * control flow.
	 */
	bool zpos_changed : 1;
	/**
	 * @color_mgmt_changed: Color management properties have changed
	 * (@gamma_lut, @degamma_lut or @ctm). Used by the atomic helpers and
	 * drivers to steer the atomic commit control flow.
	 */
	bool color_mgmt_changed : 1;

	/**
	 * @no_vblank:
	 *
	 * Reflects the ability of a CRTC to send VBLANK events. This state
	 * usually depends on the pipeline configuration. If set to true, DRM
	 * atomic helpers will send out a fake VBLANK event during display
	 * updates after all hardware changes have been committed. This is
	 * implemented in drm_atomic_helper_fake_vblank().
	 *
	 * One usage is for drivers and/or hardware without support for VBLANK
	 * interrupts. Such drivers typically do not initialize vblanking
	 * (i.e., call drm_vblank_init() with the number of CRTCs). For CRTCs
	 * without initialized vblanking, this field is set to true in
	 * drm_atomic_helper_check_modeset(), and a fake VBLANK event will be
	 * send out on each update of the display pipeline by
	 * drm_atomic_helper_fake_vblank().
	 *
	 * Another usage is CRTCs feeding a writeback connector operating in
	 * oneshot mode. In this case the fake VBLANK event is only generated
	 * when a job is queued to the writeback connector, and we want the
	 * core to fake VBLANK events when this part of the pipeline hasn't
	 * changed but others had or when the CRTC and connectors are being
	 * disabled.
	 *
	 * __drm_atomic_helper_crtc_duplicate_state() will not reset the value
	 * from the current state, the CRTC driver is then responsible for
	 * updating this field when needed.
	 *
	 * Note that the combination of &drm_crtc_state.event == NULL and
	 * &drm_crtc_state.no_blank == true is valid and usually used when the
	 * writeback connector attached to the CRTC has a new job queued. In
	 * this case the driver will send the VBLANK event on its own when the
	 * writeback job is complete.
	 */
	bool no_vblank : 1;

	/**
	 * @plane_mask: Bitmask of drm_plane_mask(plane) of planes attached to
	 * this CRTC.
	 */
	u32 plane_mask;

	/**
	 * @connector_mask: Bitmask of drm_connector_mask(connector) of
	 * connectors attached to this CRTC.
	 */
	u32 connector_mask;

	/**
	 * @encoder_mask: Bitmask of drm_encoder_mask(encoder) of encoders
	 * attached to this CRTC.
	 */
	u32 encoder_mask;

	/**
	 * @adjusted_mode:
	 *
	 * Internal display timings which can be used by the driver to handle
	 * differences between the mode requested by userspace in @mode and what
	 * is actually programmed into the hardware.
	 *
	 * For drivers using &drm_bridge, this stores hardware display timings
	 * used between the CRTC and the first bridge. For other drivers, the
	 * meaning of the adjusted_mode field is purely driver implementation
	 * defined information, and will usually be used to store the hardware
	 * display timings used between the CRTC and encoder blocks.
	 */
	struct drm_display_mode adjusted_mode;

	/**
	 * @mode:
	 *
	 * Display timings requested by userspace. The driver should try to
	 * match the refresh rate as close as possible (but note that it's
	 * undefined what exactly is close enough, e.g. some of the HDMI modes
	 * only differ in less than 1% of the refresh rate). The active width
	 * and height as observed by userspace for positioning planes must match
	 * exactly.
	 *
	 * For external connectors where the sink isn't fixed (like with a
	 * built-in panel), this mode here should match the physical mode on the
	 * wire to the last details (i.e. including sync polarities and
	 * everything).
	 */
	struct drm_display_mode mode;

	/**
	 * @mode_blob: &drm_property_blob for @mode, for exposing the mode to
	 * atomic userspace.
	 */
	struct drm_property_blob *mode_blob;

	/**
	 * @degamma_lut:
	 *
	 * Lookup table for converting framebuffer pixel data before apply the
	 * color conversion matrix @ctm. See drm_crtc_enable_color_mgmt(). The
	 * blob (if not NULL) is an array of &struct drm_color_lut.
	 */
	struct drm_property_blob *degamma_lut;

	/**
	 * @ctm:
	 *
	 * Color transformation matrix. See drm_crtc_enable_color_mgmt(). The
	 * blob (if not NULL) is a &struct drm_color_ctm.
	 */
	struct drm_property_blob *ctm;

	/**
	 * @gamma_lut:
	 *
	 * Lookup table for converting pixel data after the color conversion
	 * matrix @ctm.  See drm_crtc_enable_color_mgmt(). The blob (if not
	 * NULL) is an array of &struct drm_color_lut.
	 *
	 * Note that for mostly historical reasons stemming from Xorg heritage,
	 * this is also used to store the color map (also sometimes color lut,
	 * CLUT or color palette) for indexed formats like DRM_FORMAT_C8.
	 */
	struct drm_property_blob *gamma_lut;

	/**
	 * @target_vblank:
	 *
	 * Target vertical blank period when a page flip
	 * should take effect.
	 */
	u32 target_vblank;

	/**
	 * @async_flip:
	 *
	 * This is set when DRM_MODE_PAGE_FLIP_ASYNC is set in the legacy
	 * PAGE_FLIP IOCTL. It's not wired up for the atomic IOCTL itself yet.
	 */
	bool async_flip;

	/**
	 * @vrr_enabled:
	 *
	 * Indicates if variable refresh rate should be enabled for the CRTC.
	 * Support for the requested vrr state will depend on driver and
	 * hardware capabiltiy - lacking support is not treated as failure.
	 */
	bool vrr_enabled;

	/**
	 * @self_refresh_active:
	 *
	 * Used by the self refresh helpers to denote when a self refresh
	 * transition is occurring. This will be set on enable/disable callbacks
	 * when self refresh is being enabled or disabled. In some cases, it may
	 * not be desirable to fully shut off the crtc during self refresh.
	 * CRTC's can inspect this flag and determine the best course of action.
	 */
	bool self_refresh_active;

	/**
	 * @scaling_filter:
	 *
	 * Scaling filter to be applied
	 */
	enum drm_scaling_filter scaling_filter;

	/**
	 * @event:
	 *
	 * Optional pointer to a DRM event to signal upon completion of the
	 * state update. The driver must send out the event when the atomic
	 * commit operation completes. There are two cases:
	 *
	 *  - The event is for a CRTC which is being disabled through this
	 *    atomic commit. In that case the event can be send out any time
	 *    after the hardware has stopped scanning out the current
	 *    framebuffers. It should contain the timestamp and counter for the
	 *    last vblank before the display pipeline was shut off. The simplest
	 *    way to achieve that is calling drm_crtc_send_vblank_event()
	 *    somewhen after drm_crtc_vblank_off() has been called.
	 *
	 *  - For a CRTC which is enabled at the end of the commit (even when it
	 *    undergoes an full modeset) the vblank timestamp and counter must
	 *    be for the vblank right before the first frame that scans out the
	 *    new set of buffers. Again the event can only be sent out after the
	 *    hardware has stopped scanning out the old buffers.
	 *
	 *  - Events for disabled CRTCs are not allowed, and drivers can ignore
	 *    that case.
	 *
	 * For very simple hardware without VBLANK interrupt, enabling
	 * &struct drm_crtc_state.no_vblank makes DRM's atomic commit helpers
	 * send a fake VBLANK event at the end of the display update after all
	 * hardware changes have been applied. See
	 * drm_atomic_helper_fake_vblank().
	 *
	 * For more complex hardware this
	 * can be handled by the drm_crtc_send_vblank_event() function,
	 * which the driver should call on the provided event upon completion of
	 * the atomic commit. Note that if the driver supports vblank signalling
	 * and timestamping the vblank counters and timestamps must agree with
	 * the ones returned from page flip events. With the current vblank
	 * helper infrastructure this can be achieved by holding a vblank
	 * reference while the page flip is pending, acquired through
	 * drm_crtc_vblank_get() and released with drm_crtc_vblank_put().
	 * Drivers are free to implement their own vblank counter and timestamp
	 * tracking though, e.g. if they have accurate timestamp registers in
	 * hardware.
	 *
	 * For hardware which supports some means to synchronize vblank
	 * interrupt delivery with committing display state there's also
	 * drm_crtc_arm_vblank_event(). See the documentation of that function
	 * for a detailed discussion of the constraints it needs to be used
	 * safely.
	 *
	 * If the device can't notify of flip completion in a race-free way
	 * at all, then the event should be armed just after the page flip is
	 * committed. In the worst case the driver will send the event to
	 * userspace one frame too late. This doesn't allow for a real atomic
	 * update, but it should avoid tearing.
	 */
	struct drm_pending_vblank_event *event;

	/**
	 * @commit:
	 *
	 * This tracks how the commit for this update proceeds through the
	 * various phases. This is never cleared, except when we destroy the
	 * state, so that subsequent commits can synchronize with previous ones.
	 */
	struct drm_crtc_commit *commit;

	/** @state: backpointer to global drm_atomic_state */
	struct drm_atomic_state *state;
};

/**
 * struct drm_crtc_funcs - control CRTCs for a given device
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 *
 * The drm_crtc_funcs structure is the central CRTC management structure
 * in the DRM.  Each CRTC controls one or more connectors (note that the name
 * CRTC is simply historical, a CRTC may control LVDS, VGA, DVI, TV out, etc.
 * connectors, not just CRTs).
 *
 * Each driver is responsible for filling out this structure at startup time,
 * in addition to providing other modesetting features, like i2c and DDC
 * bus accessors.
 */
struct drm_crtc_funcs {
<<<<<<< HEAD
	/* Save CRTC state */
	void (*save)(struct drm_crtc *crtc); /* suspend? */
	/* Restore CRTC state */
	void (*restore)(struct drm_crtc *crtc); /* resume? */
	/* Reset CRTC state */
	void (*reset)(struct drm_crtc *crtc);

	/* cursor controls */
	int (*cursor_set)(struct drm_crtc *crtc, struct drm_file *file_priv,
			  uint32_t handle, uint32_t width, uint32_t height);
	int (*cursor_move)(struct drm_crtc *crtc, int x, int y);

	/* Set gamma on the CRTC */
	void (*gamma_set)(struct drm_crtc *crtc, u16 *r, u16 *g, u16 *b,
			  uint32_t start, uint32_t size);
	/* Object destroy routine */
	void (*destroy)(struct drm_crtc *crtc);

	int (*set_config)(struct drm_mode_set *set);

	/*
	 * Flip to the given framebuffer.  This implements the page
	 * flip ioctl described in drm_mode.h, specifically, the
	 * implementation must return immediately and block all
	 * rendering to the current fb until the flip has completed.
	 * If userspace set the event flag in the ioctl, the event
	 * argument will point to an event to send back when the flip
	 * completes, otherwise it will be NULL.
	 */
	int (*page_flip)(struct drm_crtc *crtc,
			 struct drm_framebuffer *fb,
			 struct drm_pending_vblank_event *event);
};

/**
 * drm_crtc - central CRTC control structure
 * @dev: parent DRM device
 * @head: list management
 * @base: base KMS object for ID tracking etc.
 * @enabled: is this CRTC enabled?
 * @mode: current mode timings
 * @hwmode: mode timings as programmed to hw regs
 * @x: x position on screen
 * @y: y position on screen
 * @funcs: CRTC control functions
 * @gamma_size: size of gamma ramp
 * @gamma_store: gamma ramp values
 * @framedur_ns: precise frame timing
 * @framedur_ns: precise line timing
 * @pixeldur_ns: precise pixel timing
 * @helper_private: mid-layer private data
=======
	/**
	 * @reset:
	 *
	 * Reset CRTC hardware and software state to off. This function isn't
	 * called by the core directly, only through drm_mode_config_reset().
	 * It's not a helper hook only for historical reasons.
	 *
	 * Atomic drivers can use drm_atomic_helper_crtc_reset() to reset
	 * atomic state using this hook.
	 */
	void (*reset)(struct drm_crtc *crtc);

	/**
	 * @cursor_set:
	 *
	 * Update the cursor image. The cursor position is relative to the CRTC
	 * and can be partially or fully outside of the visible area.
	 *
	 * Note that contrary to all other KMS functions the legacy cursor entry
	 * points don't take a framebuffer object, but instead take directly a
	 * raw buffer object id from the driver's buffer manager (which is
	 * either GEM or TTM for current drivers).
	 *
	 * This entry point is deprecated, drivers should instead implement
	 * universal plane support and register a proper cursor plane using
	 * drm_crtc_init_with_planes().
	 *
	 * This callback is optional
	 *
	 * RETURNS:
	 *
	 * 0 on success or a negative error code on failure.
	 */
	int (*cursor_set)(struct drm_crtc *crtc, struct drm_file *file_priv,
			  uint32_t handle, uint32_t width, uint32_t height);

	/**
	 * @cursor_set2:
	 *
	 * Update the cursor image, including hotspot information. The hotspot
	 * must not affect the cursor position in CRTC coordinates, but is only
	 * meant as a hint for virtualized display hardware to coordinate the
	 * guests and hosts cursor position. The cursor hotspot is relative to
	 * the cursor image. Otherwise this works exactly like @cursor_set.
	 *
	 * This entry point is deprecated, drivers should instead implement
	 * universal plane support and register a proper cursor plane using
	 * drm_crtc_init_with_planes().
	 *
	 * This callback is optional.
	 *
	 * RETURNS:
	 *
	 * 0 on success or a negative error code on failure.
	 */
	int (*cursor_set2)(struct drm_crtc *crtc, struct drm_file *file_priv,
			   uint32_t handle, uint32_t width, uint32_t height,
			   int32_t hot_x, int32_t hot_y);

	/**
	 * @cursor_move:
	 *
	 * Update the cursor position. The cursor does not need to be visible
	 * when this hook is called.
	 *
	 * This entry point is deprecated, drivers should instead implement
	 * universal plane support and register a proper cursor plane using
	 * drm_crtc_init_with_planes().
	 *
	 * This callback is optional.
	 *
	 * RETURNS:
	 *
	 * 0 on success or a negative error code on failure.
	 */
	int (*cursor_move)(struct drm_crtc *crtc, int x, int y);

	/**
	 * @gamma_set:
	 *
	 * Set gamma on the CRTC.
	 *
	 * This callback is optional.
	 *
	 * Atomic drivers who want to support gamma tables should implement the
	 * atomic color management support, enabled by calling
	 * drm_crtc_enable_color_mgmt(), which then supports the legacy gamma
	 * interface through the drm_atomic_helper_legacy_gamma_set()
	 * compatibility implementation.
	 */
	int (*gamma_set)(struct drm_crtc *crtc, u16 *r, u16 *g, u16 *b,
			 uint32_t size,
			 struct drm_modeset_acquire_ctx *ctx);

	/**
	 * @destroy:
	 *
	 * Clean up CRTC resources. This is only called at driver unload time
	 * through drm_mode_config_cleanup() since a CRTC cannot be hotplugged
	 * in DRM.
	 */
	void (*destroy)(struct drm_crtc *crtc);

	/**
	 * @set_config:
	 *
	 * This is the main legacy entry point to change the modeset state on a
	 * CRTC. All the details of the desired configuration are passed in a
	 * &struct drm_mode_set - see there for details.
	 *
	 * Drivers implementing atomic modeset should use
	 * drm_atomic_helper_set_config() to implement this hook.
	 *
	 * RETURNS:
	 *
	 * 0 on success or a negative error code on failure.
	 */
	int (*set_config)(struct drm_mode_set *set,
			  struct drm_modeset_acquire_ctx *ctx);

	/**
	 * @page_flip:
	 *
	 * Legacy entry point to schedule a flip to the given framebuffer.
	 *
	 * Page flipping is a synchronization mechanism that replaces the frame
	 * buffer being scanned out by the CRTC with a new frame buffer during
	 * vertical blanking, avoiding tearing (except when requested otherwise
	 * through the DRM_MODE_PAGE_FLIP_ASYNC flag). When an application
	 * requests a page flip the DRM core verifies that the new frame buffer
	 * is large enough to be scanned out by the CRTC in the currently
	 * configured mode and then calls this hook with a pointer to the new
	 * frame buffer.
	 *
	 * The driver must wait for any pending rendering to the new framebuffer
	 * to complete before executing the flip. It should also wait for any
	 * pending rendering from other drivers if the underlying buffer is a
	 * shared dma-buf.
	 *
	 * An application can request to be notified when the page flip has
	 * completed. The drm core will supply a &struct drm_event in the event
	 * parameter in this case. This can be handled by the
	 * drm_crtc_send_vblank_event() function, which the driver should call on
	 * the provided event upon completion of the flip. Note that if
	 * the driver supports vblank signalling and timestamping the vblank
	 * counters and timestamps must agree with the ones returned from page
	 * flip events. With the current vblank helper infrastructure this can
	 * be achieved by holding a vblank reference while the page flip is
	 * pending, acquired through drm_crtc_vblank_get() and released with
	 * drm_crtc_vblank_put(). Drivers are free to implement their own vblank
	 * counter and timestamp tracking though, e.g. if they have accurate
	 * timestamp registers in hardware.
	 *
	 * This callback is optional.
	 *
	 * NOTE:
	 *
	 * Very early versions of the KMS ABI mandated that the driver must
	 * block (but not reject) any rendering to the old framebuffer until the
	 * flip operation has completed and the old framebuffer is no longer
	 * visible. This requirement has been lifted, and userspace is instead
	 * expected to request delivery of an event and wait with recycling old
	 * buffers until such has been received.
	 *
	 * RETURNS:
	 *
	 * 0 on success or a negative error code on failure. Note that if a
	 * page flip operation is already pending the callback should return
	 * -EBUSY. Pageflips on a disabled CRTC (either by setting a NULL mode
	 * or just runtime disabled through DPMS respectively the new atomic
	 * "ACTIVE" state) should result in an -EINVAL error code. Note that
	 * drm_atomic_helper_page_flip() checks this already for atomic drivers.
	 */
	int (*page_flip)(struct drm_crtc *crtc,
			 struct drm_framebuffer *fb,
			 struct drm_pending_vblank_event *event,
			 uint32_t flags,
			 struct drm_modeset_acquire_ctx *ctx);

	/**
	 * @page_flip_target:
	 *
	 * Same as @page_flip but with an additional parameter specifying the
	 * absolute target vertical blank period (as reported by
	 * drm_crtc_vblank_count()) when the flip should take effect.
	 *
	 * Note that the core code calls drm_crtc_vblank_get before this entry
	 * point, and will call drm_crtc_vblank_put if this entry point returns
	 * any non-0 error code. It's the driver's responsibility to call
	 * drm_crtc_vblank_put after this entry point returns 0, typically when
	 * the flip completes.
	 */
	int (*page_flip_target)(struct drm_crtc *crtc,
				struct drm_framebuffer *fb,
				struct drm_pending_vblank_event *event,
				uint32_t flags, uint32_t target,
				struct drm_modeset_acquire_ctx *ctx);

	/**
	 * @set_property:
	 *
	 * This is the legacy entry point to update a property attached to the
	 * CRTC.
	 *
	 * This callback is optional if the driver does not support any legacy
	 * driver-private properties. For atomic drivers it is not used because
	 * property handling is done entirely in the DRM core.
	 *
	 * RETURNS:
	 *
	 * 0 on success or a negative error code on failure.
	 */
	int (*set_property)(struct drm_crtc *crtc,
			    struct drm_property *property, uint64_t val);

	/**
	 * @atomic_duplicate_state:
	 *
	 * Duplicate the current atomic state for this CRTC and return it.
	 * The core and helpers guarantee that any atomic state duplicated with
	 * this hook and still owned by the caller (i.e. not transferred to the
	 * driver by calling &drm_mode_config_funcs.atomic_commit) will be
	 * cleaned up by calling the @atomic_destroy_state hook in this
	 * structure.
	 *
	 * This callback is mandatory for atomic drivers.
	 *
	 * Atomic drivers which don't subclass &struct drm_crtc_state should use
	 * drm_atomic_helper_crtc_duplicate_state(). Drivers that subclass the
	 * state structure to extend it with driver-private state should use
	 * __drm_atomic_helper_crtc_duplicate_state() to make sure shared state is
	 * duplicated in a consistent fashion across drivers.
	 *
	 * It is an error to call this hook before &drm_crtc.state has been
	 * initialized correctly.
	 *
	 * NOTE:
	 *
	 * If the duplicate state references refcounted resources this hook must
	 * acquire a reference for each of them. The driver must release these
	 * references again in @atomic_destroy_state.
	 *
	 * RETURNS:
	 *
	 * Duplicated atomic state or NULL when the allocation failed.
	 */
	struct drm_crtc_state *(*atomic_duplicate_state)(struct drm_crtc *crtc);

	/**
	 * @atomic_destroy_state:
	 *
	 * Destroy a state duplicated with @atomic_duplicate_state and release
	 * or unreference all resources it references
	 *
	 * This callback is mandatory for atomic drivers.
	 */
	void (*atomic_destroy_state)(struct drm_crtc *crtc,
				     struct drm_crtc_state *state);

	/**
	 * @atomic_set_property:
	 *
	 * Decode a driver-private property value and store the decoded value
	 * into the passed-in state structure. Since the atomic core decodes all
	 * standardized properties (even for extensions beyond the core set of
	 * properties which might not be implemented by all drivers) this
	 * requires drivers to subclass the state structure.
	 *
	 * Such driver-private properties should really only be implemented for
	 * truly hardware/vendor specific state. Instead it is preferred to
	 * standardize atomic extension and decode the properties used to expose
	 * such an extension in the core.
	 *
	 * Do not call this function directly, use
	 * drm_atomic_crtc_set_property() instead.
	 *
	 * This callback is optional if the driver does not support any
	 * driver-private atomic properties.
	 *
	 * NOTE:
	 *
	 * This function is called in the state assembly phase of atomic
	 * modesets, which can be aborted for any reason (including on
	 * userspace's request to just check whether a configuration would be
	 * possible). Drivers MUST NOT touch any persistent state (hardware or
	 * software) or data structures except the passed in @state parameter.
	 *
	 * Also since userspace controls in which order properties are set this
	 * function must not do any input validation (since the state update is
	 * incomplete and hence likely inconsistent). Instead any such input
	 * validation must be done in the various atomic_check callbacks.
	 *
	 * RETURNS:
	 *
	 * 0 if the property has been found, -EINVAL if the property isn't
	 * implemented by the driver (which should never happen, the core only
	 * asks for properties attached to this CRTC). No other validation is
	 * allowed by the driver. The core already checks that the property
	 * value is within the range (integer, valid enum value, ...) the driver
	 * set when registering the property.
	 */
	int (*atomic_set_property)(struct drm_crtc *crtc,
				   struct drm_crtc_state *state,
				   struct drm_property *property,
				   uint64_t val);
	/**
	 * @atomic_get_property:
	 *
	 * Reads out the decoded driver-private property. This is used to
	 * implement the GETCRTC IOCTL.
	 *
	 * Do not call this function directly, use
	 * drm_atomic_crtc_get_property() instead.
	 *
	 * This callback is optional if the driver does not support any
	 * driver-private atomic properties.
	 *
	 * RETURNS:
	 *
	 * 0 on success, -EINVAL if the property isn't implemented by the
	 * driver (which should never happen, the core only asks for
	 * properties attached to this CRTC).
	 */
	int (*atomic_get_property)(struct drm_crtc *crtc,
				   const struct drm_crtc_state *state,
				   struct drm_property *property,
				   uint64_t *val);

	/**
	 * @late_register:
	 *
	 * This optional hook can be used to register additional userspace
	 * interfaces attached to the crtc like debugfs interfaces.
	 * It is called late in the driver load sequence from drm_dev_register().
	 * Everything added from this callback should be unregistered in
	 * the early_unregister callback.
	 *
	 * Returns:
	 *
	 * 0 on success, or a negative error code on failure.
	 */
	int (*late_register)(struct drm_crtc *crtc);

	/**
	 * @early_unregister:
	 *
	 * This optional hook should be used to unregister the additional
	 * userspace interfaces attached to the crtc from
	 * @late_register. It is called from drm_dev_unregister(),
	 * early in the driver unload sequence to disable userspace access
	 * before data structures are torndown.
	 */
	void (*early_unregister)(struct drm_crtc *crtc);

	/**
	 * @set_crc_source:
	 *
	 * Changes the source of CRC checksums of frames at the request of
	 * userspace, typically for testing purposes. The sources available are
	 * specific of each driver and a %NULL value indicates that CRC
	 * generation is to be switched off.
	 *
	 * When CRC generation is enabled, the driver should call
	 * drm_crtc_add_crc_entry() at each frame, providing any information
	 * that characterizes the frame contents in the crcN arguments, as
	 * provided from the configured source. Drivers must accept an "auto"
	 * source name that will select a default source for this CRTC.
	 *
	 * This may trigger an atomic modeset commit if necessary, to enable CRC
	 * generation.
	 *
	 * Note that "auto" can depend upon the current modeset configuration,
	 * e.g. it could pick an encoder or output specific CRC sampling point.
	 *
	 * This callback is optional if the driver does not support any CRC
	 * generation functionality.
	 *
	 * RETURNS:
	 *
	 * 0 on success or a negative error code on failure.
	 */
	int (*set_crc_source)(struct drm_crtc *crtc, const char *source);

	/**
	 * @verify_crc_source:
	 *
	 * verifies the source of CRC checksums of frames before setting the
	 * source for CRC and during crc open. Source parameter can be NULL
	 * while disabling crc source.
	 *
	 * This callback is optional if the driver does not support any CRC
	 * generation functionality.
	 *
	 * RETURNS:
	 *
	 * 0 on success or a negative error code on failure.
	 */
	int (*verify_crc_source)(struct drm_crtc *crtc, const char *source,
				 size_t *values_cnt);
	/**
	 * @get_crc_sources:
	 *
	 * Driver callback for getting a list of all the available sources for
	 * CRC generation. This callback depends upon verify_crc_source, So
	 * verify_crc_source callback should be implemented before implementing
	 * this. Driver can pass full list of available crc sources, this
	 * callback does the verification on each crc-source before passing it
	 * to userspace.
	 *
	 * This callback is optional if the driver does not support exporting of
	 * possible CRC sources list.
	 *
	 * RETURNS:
	 *
	 * a constant character pointer to the list of all the available CRC
	 * sources. On failure driver should return NULL. count should be
	 * updated with number of sources in list. if zero we don't process any
	 * source from the list.
	 */
	const char *const *(*get_crc_sources)(struct drm_crtc *crtc,
					      size_t *count);

	/**
	 * @atomic_print_state:
	 *
	 * If driver subclasses &struct drm_crtc_state, it should implement
	 * this optional hook for printing additional driver specific state.
	 *
	 * Do not call this directly, use drm_atomic_crtc_print_state()
	 * instead.
	 */
	void (*atomic_print_state)(struct drm_printer *p,
				   const struct drm_crtc_state *state);

	/**
	 * @get_vblank_counter:
	 *
	 * Driver callback for fetching a raw hardware vblank counter for the
	 * CRTC. It's meant to be used by new drivers as the replacement of
	 * &drm_driver.get_vblank_counter hook.
	 *
	 * This callback is optional. If a device doesn't have a hardware
	 * counter, the driver can simply leave the hook as NULL. The DRM core
	 * will account for missed vblank events while interrupts where disabled
	 * based on system timestamps.
	 *
	 * Wraparound handling and loss of events due to modesetting is dealt
	 * with in the DRM core code, as long as drivers call
	 * drm_crtc_vblank_off() and drm_crtc_vblank_on() when disabling or
	 * enabling a CRTC.
	 *
	 * See also &drm_device.vblank_disable_immediate and
	 * &drm_device.max_vblank_count.
	 *
	 * Returns:
	 *
	 * Raw vblank counter value.
	 */
	u32 (*get_vblank_counter)(struct drm_crtc *crtc);

	/**
	 * @enable_vblank:
	 *
	 * Enable vblank interrupts for the CRTC. It's meant to be used by
	 * new drivers as the replacement of &drm_driver.enable_vblank hook.
	 *
	 * Returns:
	 *
	 * Zero on success, appropriate errno if the vblank interrupt cannot
	 * be enabled.
	 */
	int (*enable_vblank)(struct drm_crtc *crtc);

	/**
	 * @disable_vblank:
	 *
	 * Disable vblank interrupts for the CRTC. It's meant to be used by
	 * new drivers as the replacement of &drm_driver.disable_vblank hook.
	 */
	void (*disable_vblank)(struct drm_crtc *crtc);

	/**
	 * @get_vblank_timestamp:
	 *
	 * Called by drm_get_last_vbltimestamp(). Should return a precise
	 * timestamp when the most recent vblank interval ended or will end.
	 *
	 * Specifically, the timestamp in @vblank_time should correspond as
	 * closely as possible to the time when the first video scanline of
	 * the video frame after the end of vblank will start scanning out,
	 * the time immediately after end of the vblank interval. If the
	 * @crtc is currently inside vblank, this will be a time in the future.
	 * If the @crtc is currently scanning out a frame, this will be the
	 * past start time of the current scanout. This is meant to adhere
	 * to the OpenML OML_sync_control extension specification.
	 *
	 * Parameters:
	 *
	 * crtc:
	 *     CRTC for which timestamp should be returned.
	 * max_error:
	 *     Maximum allowable timestamp error in nanoseconds.
	 *     Implementation should strive to provide timestamp
	 *     with an error of at most max_error nanoseconds.
	 *     Returns true upper bound on error for timestamp.
	 * vblank_time:
	 *     Target location for returned vblank timestamp.
	 * in_vblank_irq:
	 *     True when called from drm_crtc_handle_vblank().  Some drivers
	 *     need to apply some workarounds for gpu-specific vblank irq quirks
	 *     if flag is set.
	 *
	 * Returns:
	 *
	 * True on success, false on failure, which means the core should
	 * fallback to a simple timestamp taken in drm_crtc_handle_vblank().
	 */
	bool (*get_vblank_timestamp)(struct drm_crtc *crtc,
				     int *max_error,
				     ktime_t *vblank_time,
				     bool in_vblank_irq);
};

/**
 * struct drm_crtc - central CRTC control structure
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 *
 * Each CRTC may have one or more connectors associated with it.  This structure
 * allows the CRTC to be controlled.
 */
struct drm_crtc {
<<<<<<< HEAD
	struct drm_device *dev;
	struct list_head head;

	struct drm_mode_object base;

	/* framebuffer the connector is currently bound to */
	struct drm_framebuffer *fb;

	bool enabled;

	/* Requested mode from modesetting. */
	struct drm_display_mode mode;

	/* Programmed mode in hw, after adjustments for encoders,
	 * crtc, panel scaling etc. Needed for timestamping etc.
	 */
	struct drm_display_mode hwmode;

	int x, y;
	const struct drm_crtc_funcs *funcs;

	/* CRTC gamma size for reporting to userspace */
	uint32_t gamma_size;
	uint16_t *gamma_store;

	/* Constants needed for precise vblank and swap timestamping. */
	s64 framedur_ns, linedur_ns, pixeldur_ns;

	/* if you are using the helper */
	void *helper_private;
};


/**
 * drm_connector_funcs - control connectors on a given device
 * @dpms: set power state (see drm_crtc_funcs above)
 * @save: save connector state
 * @restore: restore connector state
 * @reset: reset connector after state has been invalidate (e.g. resume)
 * @mode_valid: is this mode valid on the given connector?
 * @mode_fixup: try to fixup proposed mode for this connector
 * @mode_set: set this mode
 * @detect: is this connector active?
 * @get_modes: get mode list for this connector
 * @set_property: property for this connector may need update
 * @destroy: make object go away
 * @force: notify the driver the connector is forced on
 *
 * Each CRTC may have one or more connectors attached to it.  The functions
 * below allow the core DRM code to control connectors, enumerate available modes,
 * etc.
 */
struct drm_connector_funcs {
	void (*dpms)(struct drm_connector *connector, int mode);
	void (*save)(struct drm_connector *connector);
	void (*restore)(struct drm_connector *connector);
	void (*reset)(struct drm_connector *connector);

	/* Check to see if anything is attached to the connector.
	 * @force is set to false whilst polling, true when checking the
	 * connector due to user request. @force can be used by the driver
	 * to avoid expensive, destructive operations during automated
	 * probing.
	 */
	enum drm_connector_status (*detect)(struct drm_connector *connector,
					    bool force);
	int (*fill_modes)(struct drm_connector *connector, uint32_t max_width, uint32_t max_height);
	int (*set_property)(struct drm_connector *connector, struct drm_property *property,
			     uint64_t val);
	void (*destroy)(struct drm_connector *connector);
	void (*force)(struct drm_connector *connector);
};

/**
 * drm_encoder_funcs - encoder controls
 * @reset: reset state (e.g. at init or resume time)
 * @destroy: cleanup and free associated data
 *
 * Encoders sit between CRTCs and connectors.
 */
struct drm_encoder_funcs {
	void (*reset)(struct drm_encoder *encoder);
	void (*destroy)(struct drm_encoder *encoder);
};

#define DRM_CONNECTOR_MAX_UMODES 16
#define DRM_CONNECTOR_MAX_PROPERTY 16
#define DRM_CONNECTOR_LEN 32
#define DRM_CONNECTOR_MAX_ENCODER 3

/**
 * drm_encoder - central DRM encoder structure
 * @dev: parent DRM device
 * @head: list management
 * @base: base KMS object
 * @encoder_type: one of the %DRM_MODE_ENCODER_<foo> types in drm_mode.h
 * @possible_crtcs: bitmask of potential CRTC bindings
 * @possible_clones: bitmask of potential sibling encoders for cloning
 * @crtc: currently bound CRTC
 * @funcs: control functions
 * @helper_private: mid-layer private data
 *
 * CRTCs drive pixels to encoders, which convert them into signals
 * appropriate for a given connector or set of connectors.
 */
struct drm_encoder {
	struct drm_device *dev;
	struct list_head head;

	struct drm_mode_object base;
	int encoder_type;
	uint32_t possible_crtcs;
	uint32_t possible_clones;

	struct drm_crtc *crtc;
	const struct drm_encoder_funcs *funcs;
	void *helper_private;
};

enum drm_connector_force {
	DRM_FORCE_UNSPECIFIED,
	DRM_FORCE_OFF,
	DRM_FORCE_ON,         /* force on analog part normally */
	DRM_FORCE_ON_DIGITAL, /* for DVI-I use digital connector */
};

/* should we poll this connector for connects and disconnects */
/* hot plug detectable */
#define DRM_CONNECTOR_POLL_HPD (1 << 0)
/* poll for connections */
#define DRM_CONNECTOR_POLL_CONNECT (1 << 1)
/* can cleanly poll for disconnections without flickering the screen */
/* DACs should rarely do this without a lot of testing */
#define DRM_CONNECTOR_POLL_DISCONNECT (1 << 2)

#define MAX_ELD_BYTES	128

/**
 * drm_connector - central DRM connector control structure
 * @dev: parent DRM device
 * @kdev: kernel device for sysfs attributes
 * @attr: sysfs attributes
 * @head: list management
 * @base: base KMS object
 * @connector_type: one of the %DRM_MODE_CONNECTOR_<foo> types from drm_mode.h
 * @connector_type_id: index into connector type enum
 * @interlace_allowed: can this connector handle interlaced modes?
 * @doublescan_allowed: can this connector handle doublescan?
 * @modes: modes available on this connector (from fill_modes() + user)
 * @status: one of the drm_connector_status enums (connected, not, or unknown)
 * @probed_modes: list of modes derived directly from the display
 * @display_info: information about attached display (e.g. from EDID)
 * @funcs: connector control functions
 * @user_modes: user added mode list
 * @edid_blob_ptr: DRM property containing EDID if present
 * @property_ids: property tracking for this connector
 * @property_values: value pointers or data for properties
 * @polled: a %DRM_CONNECTOR_POLL_<foo> value for core driven polling
 * @dpms: current dpms state
 * @helper_private: mid-layer private data
 * @force: a %DRM_FORCE_<foo> state for forced mode sets
 * @encoder_ids: valid encoders for this connector
 * @encoder: encoder driving this connector, if any
 * @eld: EDID-like data, if present
 * @dvi_dual: dual link DVI, if found
 * @max_tmds_clock: max clock rate, if found
 * @latency_present: AV delay info from ELD, if found
 * @video_latency: video latency info from ELD, if found
 * @audio_latency: audio latency info from ELD, if found
 * @null_edid_counter: track sinks that give us all zeros for the EDID
 *
 * Each connector may be connected to one or more CRTCs, or may be clonable by
 * another connector if they can share a CRTC.  Each connector also has a specific
 * position in the broader display (referred to as a 'screen' though it could
 * span multiple monitors).
 */
struct drm_connector {
	struct drm_device *dev;
	struct device kdev;
	struct device_attribute *attr;
	struct list_head head;

	struct drm_mode_object base;

	int connector_type;
	int connector_type_id;
	bool interlace_allowed;
	bool doublescan_allowed;
	struct list_head modes; /* list of modes on this connector */

	enum drm_connector_status status;

	/* these are modes added by probing with DDC or the BIOS */
	struct list_head probed_modes;

	struct drm_display_info display_info;
	const struct drm_connector_funcs *funcs;

	struct list_head user_modes;
	struct drm_property_blob *edid_blob_ptr;
	u32 property_ids[DRM_CONNECTOR_MAX_PROPERTY];
	uint64_t property_values[DRM_CONNECTOR_MAX_PROPERTY];

	uint8_t polled; /* DRM_CONNECTOR_POLL_* */

	/* requested DPMS state */
	int dpms;

	void *helper_private;

	/* forced on connector */
	enum drm_connector_force force;
	uint32_t encoder_ids[DRM_CONNECTOR_MAX_ENCODER];
	struct drm_encoder *encoder; /* currently active encoder */

	/* EDID bits */
	uint8_t eld[MAX_ELD_BYTES];
	bool dvi_dual;
	int max_tmds_clock;	/* in MHz */
	bool latency_present[2];
	int video_latency[2];	/* [0]: progressive, [1]: interlaced */
	int audio_latency[2];
	int null_edid_counter; /* needed to workaround some HW bugs where we get all 0s */
};

/**
 * drm_plane_funcs - driver plane control functions
 * @update_plane: update the plane configuration
 * @disable_plane: shut down the plane
 * @destroy: clean up plane resources
 */
struct drm_plane_funcs {
	int (*update_plane)(struct drm_plane *plane,
			    struct drm_crtc *crtc, struct drm_framebuffer *fb,
			    int crtc_x, int crtc_y,
			    unsigned int crtc_w, unsigned int crtc_h,
			    uint32_t src_x, uint32_t src_y,
			    uint32_t src_w, uint32_t src_h);
	int (*disable_plane)(struct drm_plane *plane);
	void (*destroy)(struct drm_plane *plane);
};

/**
 * drm_plane - central DRM plane control structure
 * @dev: DRM device this plane belongs to
 * @head: for list management
 * @base: base mode object
 * @possible_crtcs: pipes this plane can be bound to
 * @format_types: array of formats supported by this plane
 * @format_count: number of formats supported
 * @crtc: currently bound CRTC
 * @fb: currently bound fb
 * @gamma_size: size of gamma table
 * @gamma_store: gamma correction table
 * @enabled: enabled flag
 * @funcs: helper functions
 * @helper_private: storage for drver layer
 */
struct drm_plane {
	struct drm_device *dev;
	struct list_head head;

	struct drm_mode_object base;

	uint32_t possible_crtcs;
	uint32_t *format_types;
	uint32_t format_count;

	struct drm_crtc *crtc;
	struct drm_framebuffer *fb;

	/* CRTC gamma size for reporting to userspace */
	uint32_t gamma_size;
	uint16_t *gamma_store;

	bool enabled;

	const struct drm_plane_funcs *funcs;
	void *helper_private;
};

/**
 * drm_mode_set - new values for a CRTC config change
 * @head: list management
=======
	/** @dev: parent DRM device */
	struct drm_device *dev;
	/** @port: OF node used by drm_of_find_possible_crtcs(). */
	struct device_node *port;
	/**
	 * @head:
	 *
	 * List of all CRTCs on @dev, linked from &drm_mode_config.crtc_list.
	 * Invariant over the lifetime of @dev and therefore does not need
	 * locking.
	 */
	struct list_head head;

	/** @name: human readable name, can be overwritten by the driver */
	char *name;

	/**
	 * @mutex:
	 *
	 * This provides a read lock for the overall CRTC state (mode, dpms
	 * state, ...) and a write lock for everything which can be update
	 * without a full modeset (fb, cursor data, CRTC properties ...). A full
	 * modeset also need to grab &drm_mode_config.connection_mutex.
	 *
	 * For atomic drivers specifically this protects @state.
	 */
	struct drm_modeset_lock mutex;

	/** @base: base KMS object for ID tracking etc. */
	struct drm_mode_object base;

	/**
	 * @primary:
	 * Primary plane for this CRTC. Note that this is only
	 * relevant for legacy IOCTL, it specifies the plane implicitly used by
	 * the SETCRTC and PAGE_FLIP IOCTLs. It does not have any significance
	 * beyond that.
	 */
	struct drm_plane *primary;

	/**
	 * @cursor:
	 * Cursor plane for this CRTC. Note that this is only relevant for
	 * legacy IOCTL, it specifies the plane implicitly used by the SETCURSOR
	 * and SETCURSOR2 IOCTLs. It does not have any significance
	 * beyond that.
	 */
	struct drm_plane *cursor;

	/**
	 * @index: Position inside the mode_config.list, can be used as an array
	 * index. It is invariant over the lifetime of the CRTC.
	 */
	unsigned index;

	/**
	 * @cursor_x: Current x position of the cursor, used for universal
	 * cursor planes because the SETCURSOR IOCTL only can update the
	 * framebuffer without supplying the coordinates. Drivers should not use
	 * this directly, atomic drivers should look at &drm_plane_state.crtc_x
	 * of the cursor plane instead.
	 */
	int cursor_x;
	/**
	 * @cursor_y: Current y position of the cursor, used for universal
	 * cursor planes because the SETCURSOR IOCTL only can update the
	 * framebuffer without supplying the coordinates. Drivers should not use
	 * this directly, atomic drivers should look at &drm_plane_state.crtc_y
	 * of the cursor plane instead.
	 */
	int cursor_y;

	/**
	 * @enabled:
	 *
	 * Is this CRTC enabled? Should only be used by legacy drivers, atomic
	 * drivers should instead consult &drm_crtc_state.enable and
	 * &drm_crtc_state.active. Atomic drivers can update this by calling
	 * drm_atomic_helper_update_legacy_modeset_state().
	 */
	bool enabled;

	/**
	 * @mode:
	 *
	 * Current mode timings. Should only be used by legacy drivers, atomic
	 * drivers should instead consult &drm_crtc_state.mode. Atomic drivers
	 * can update this by calling
	 * drm_atomic_helper_update_legacy_modeset_state().
	 */
	struct drm_display_mode mode;

	/**
	 * @hwmode:
	 *
	 * Programmed mode in hw, after adjustments for encoders, crtc, panel
	 * scaling etc. Should only be used by legacy drivers, for high
	 * precision vblank timestamps in
	 * drm_crtc_vblank_helper_get_vblank_timestamp().
	 *
	 * Note that atomic drivers should not use this, but instead use
	 * &drm_crtc_state.adjusted_mode. And for high-precision timestamps
	 * drm_crtc_vblank_helper_get_vblank_timestamp() used
	 * &drm_vblank_crtc.hwmode,
	 * which is filled out by calling drm_calc_timestamping_constants().
	 */
	struct drm_display_mode hwmode;

	/**
	 * @x:
	 * x position on screen. Should only be used by legacy drivers, atomic
	 * drivers should look at &drm_plane_state.crtc_x of the primary plane
	 * instead. Updated by calling
	 * drm_atomic_helper_update_legacy_modeset_state().
	 */
	int x;
	/**
	 * @y:
	 * y position on screen. Should only be used by legacy drivers, atomic
	 * drivers should look at &drm_plane_state.crtc_y of the primary plane
	 * instead. Updated by calling
	 * drm_atomic_helper_update_legacy_modeset_state().
	 */
	int y;

	/** @funcs: CRTC control functions */
	const struct drm_crtc_funcs *funcs;

	/**
	 * @gamma_size: Size of legacy gamma ramp reported to userspace. Set up
	 * by calling drm_mode_crtc_set_gamma_size().
	 *
	 * Note that atomic drivers need to instead use
	 * &drm_crtc_state.gamma_lut. See drm_crtc_enable_color_mgmt().
	 */
	uint32_t gamma_size;

	/**
	 * @gamma_store: Gamma ramp values used by the legacy SETGAMMA and
	 * GETGAMMA IOCTls. Set up by calling drm_mode_crtc_set_gamma_size().
	 *
	 * Note that atomic drivers need to instead use
	 * &drm_crtc_state.gamma_lut. See drm_crtc_enable_color_mgmt().
	 */
	uint16_t *gamma_store;

	/** @helper_private: mid-layer private data */
	const struct drm_crtc_helper_funcs *helper_private;

	/** @properties: property tracking for this CRTC */
	struct drm_object_properties properties;

	/**
	 * @scaling_filter_property: property to apply a particular filter while
	 * scaling.
	 */
	struct drm_property *scaling_filter_property;

	/**
	 * @state:
	 *
	 * Current atomic state for this CRTC.
	 *
	 * This is protected by @mutex. Note that nonblocking atomic commits
	 * access the current CRTC state without taking locks. Either by going
	 * through the &struct drm_atomic_state pointers, see
	 * for_each_oldnew_crtc_in_state(), for_each_old_crtc_in_state() and
	 * for_each_new_crtc_in_state(). Or through careful ordering of atomic
	 * commit operations as implemented in the atomic helpers, see
	 * &struct drm_crtc_commit.
	 */
	struct drm_crtc_state *state;

	/**
	 * @commit_list:
	 *
	 * List of &drm_crtc_commit structures tracking pending commits.
	 * Protected by @commit_lock. This list holds its own full reference,
	 * as does the ongoing commit.
	 *
	 * "Note that the commit for a state change is also tracked in
	 * &drm_crtc_state.commit. For accessing the immediately preceding
	 * commit in an atomic update it is recommended to just use that
	 * pointer in the old CRTC state, since accessing that doesn't need
	 * any locking or list-walking. @commit_list should only be used to
	 * stall for framebuffer cleanup that's signalled through
	 * &drm_crtc_commit.cleanup_done."
	 */
	struct list_head commit_list;

	/**
	 * @commit_lock:
	 *
	 * Spinlock to protect @commit_list.
	 */
	spinlock_t commit_lock;

	/**
	 * @debugfs_entry:
	 *
	 * Debugfs directory for this CRTC.
	 */
	struct dentry *debugfs_entry;

	/**
	 * @crc:
	 *
	 * Configuration settings of CRC capture.
	 */
	struct drm_crtc_crc crc;

	/**
	 * @fence_context:
	 *
	 * timeline context used for fence operations.
	 */
	unsigned int fence_context;

	/**
	 * @fence_lock:
	 *
	 * spinlock to protect the fences in the fence_context.
	 */
	spinlock_t fence_lock;
	/**
	 * @fence_seqno:
	 *
	 * Seqno variable used as monotonic counter for the fences
	 * created on the CRTC's timeline.
	 */
	unsigned long fence_seqno;

	/**
	 * @timeline_name:
	 *
	 * The name of the CRTC's fence timeline.
	 */
	char timeline_name[32];

	/**
	 * @self_refresh_data: Holds the state for the self refresh helpers
	 *
	 * Initialized via drm_self_refresh_helper_init().
	 */
	struct drm_self_refresh_data *self_refresh_data;
};

/**
 * struct drm_mode_set - new values for a CRTC config change
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 * @fb: framebuffer to use for new config
 * @crtc: CRTC whose configuration we're about to change
 * @mode: mode timings to use
 * @x: position of this CRTC relative to @fb
 * @y: position of this CRTC relative to @fb
 * @connectors: array of connectors to drive with this CRTC if possible
 * @num_connectors: size of @connectors array
 *
<<<<<<< HEAD
 * Represents a single crtc the connectors that it drives with what mode
 * and from which framebuffer it scans out from.
 *
 * This is used to set modes.
 */
struct drm_mode_set {
	struct list_head head;

=======
 * This represents a modeset configuration for the legacy SETCRTC ioctl and is
 * also used internally. Atomic drivers instead use &drm_atomic_state.
 */
struct drm_mode_set {
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	struct drm_framebuffer *fb;
	struct drm_crtc *crtc;
	struct drm_display_mode *mode;

	uint32_t x;
	uint32_t y;

	struct drm_connector **connectors;
	size_t num_connectors;
};

<<<<<<< HEAD
/**
 * struct drm_mode_config_funcs - basic driver provided mode setting functions
 * @fb_create: create a new framebuffer object
 * @output_poll_changed: function to handle output configuration changes
 *
 * Some global (i.e. not per-CRTC, connector, etc) mode setting functions that
 * involve drivers.
 */
struct drm_mode_config_funcs {
	struct drm_framebuffer *(*fb_create)(struct drm_device *dev,
					     struct drm_file *file_priv,
					     struct drm_mode_fb_cmd2 *mode_cmd);
	void (*output_poll_changed)(struct drm_device *dev);
};

/**
 * drm_mode_group - group of mode setting resources for potential sub-grouping
 * @num_crtcs: CRTC count
 * @num_encoders: encoder count
 * @num_connectors: connector count
 * @id_list: list of KMS object IDs in this group
 *
 * Currently this simply tracks the global mode setting state.  But in the
 * future it could allow groups of objects to be set aside into independent
 * control groups for use by different user level processes (e.g. two X servers
 * running simultaneously on different heads, each with their own mode
 * configuration and freedom of mode setting).
 */
struct drm_mode_group {
	uint32_t num_crtcs;
	uint32_t num_encoders;
	uint32_t num_connectors;

	/* list of object IDs for this group */
	uint32_t *id_list;
};

/**
 * drm_mode_config - Mode configuration control structure
 * @mutex: mutex protecting KMS related lists and structures
 * @idr_mutex: mutex for KMS ID allocation and management
 * @crtc_idr: main KMS ID tracking object
 * @num_fb: number of fbs available
 * @fb_list: list of framebuffers available
 * @num_connector: number of connectors on this device
 * @connector_list: list of connector objects
 * @num_encoder: number of encoders on this device
 * @encoder_list: list of encoder objects
 * @num_crtc: number of CRTCs on this device
 * @crtc_list: list of CRTC objects
 * @min_width: minimum pixel width on this device
 * @min_height: minimum pixel height on this device
 * @max_width: maximum pixel width on this device
 * @max_height: maximum pixel height on this device
 * @funcs: core driver provided mode setting functions
 * @fb_base: base address of the framebuffer
 * @poll_enabled: track polling status for this device
 * @output_poll_work: delayed work for polling in process context
 * @*_property: core property tracking
 *
 * Core mode resource tracking structure.  All CRTC, encoders, and connectors
 * enumerated by the driver are added here, as are global properties.  Some
 * global restrictions are also here, e.g. dimension restrictions.
 */
struct drm_mode_config {
	struct mutex mutex; /* protects configuration (mode lists etc.) */
	struct mutex idr_mutex; /* for IDR management */
	struct idr crtc_idr; /* use this idr for all IDs, fb, crtc, connector, modes - just makes life easier */
	/* this is limited to one for now */
	int num_fb;
	struct list_head fb_list;
	int num_connector;
	struct list_head connector_list;
	int num_encoder;
	struct list_head encoder_list;
	int num_plane;
	struct list_head plane_list;

	int num_crtc;
	struct list_head crtc_list;

	struct list_head property_list;

	int min_width, min_height;
	int max_width, max_height;
	struct drm_mode_config_funcs *funcs;
	resource_size_t fb_base;

	/* output poll support */
	bool poll_enabled;
	struct delayed_work output_poll_work;

	/* pointers to standard properties */
	struct list_head property_blob_list;
	struct drm_property *edid_property;
	struct drm_property *dpms_property;

	/* DVI-I properties */
	struct drm_property *dvi_i_subconnector_property;
	struct drm_property *dvi_i_select_subconnector_property;

	/* TV properties */
	struct drm_property *tv_subconnector_property;
	struct drm_property *tv_select_subconnector_property;
	struct drm_property *tv_mode_property;
	struct drm_property *tv_left_margin_property;
	struct drm_property *tv_right_margin_property;
	struct drm_property *tv_top_margin_property;
	struct drm_property *tv_bottom_margin_property;
	struct drm_property *tv_brightness_property;
	struct drm_property *tv_contrast_property;
	struct drm_property *tv_flicker_reduction_property;
	struct drm_property *tv_overscan_property;
	struct drm_property *tv_saturation_property;
	struct drm_property *tv_hue_property;

	/* Optional properties */
	struct drm_property *scaling_mode_property;
	struct drm_property *dithering_mode_property;
	struct drm_property *dirty_info_property;

	/* dumb ioctl parameters */
	uint32_t preferred_depth, prefer_shadow;
};

#define obj_to_crtc(x) container_of(x, struct drm_crtc, base)
#define obj_to_connector(x) container_of(x, struct drm_connector, base)
#define obj_to_encoder(x) container_of(x, struct drm_encoder, base)
#define obj_to_mode(x) container_of(x, struct drm_display_mode, base)
#define obj_to_fb(x) container_of(x, struct drm_framebuffer, base)
#define obj_to_property(x) container_of(x, struct drm_property, base)
#define obj_to_blob(x) container_of(x, struct drm_property_blob, base)
#define obj_to_plane(x) container_of(x, struct drm_plane, base)

struct drm_prop_enum_list {
	int type;
	char *name;
};

extern int drm_crtc_init(struct drm_device *dev,
			 struct drm_crtc *crtc,
			 const struct drm_crtc_funcs *funcs);
extern void drm_crtc_cleanup(struct drm_crtc *crtc);

extern int drm_connector_init(struct drm_device *dev,
			      struct drm_connector *connector,
			      const struct drm_connector_funcs *funcs,
			      int connector_type);

extern void drm_connector_cleanup(struct drm_connector *connector);
/* helper to unplug all connectors from sysfs for device */
extern void drm_connector_unplug_all(struct drm_device *dev);

extern int drm_encoder_init(struct drm_device *dev,
			    struct drm_encoder *encoder,
			    const struct drm_encoder_funcs *funcs,
			    int encoder_type);

extern int drm_plane_init(struct drm_device *dev,
			  struct drm_plane *plane,
			  unsigned long possible_crtcs,
			  const struct drm_plane_funcs *funcs,
			  const uint32_t *formats, uint32_t format_count,
			  bool priv);
extern void drm_plane_cleanup(struct drm_plane *plane);

extern void drm_encoder_cleanup(struct drm_encoder *encoder);

extern char *drm_get_connector_name(struct drm_connector *connector);
extern char *drm_get_dpms_name(int val);
extern char *drm_get_dvi_i_subconnector_name(int val);
extern char *drm_get_dvi_i_select_name(int val);
extern char *drm_get_tv_subconnector_name(int val);
extern char *drm_get_tv_select_name(int val);
extern void drm_fb_release(struct drm_file *file_priv);
extern int drm_mode_group_init_legacy_group(struct drm_device *dev, struct drm_mode_group *group);
extern struct edid *drm_get_edid(struct drm_connector *connector,
				 struct i2c_adapter *adapter);
extern int drm_add_edid_modes(struct drm_connector *connector, struct edid *edid);
extern void drm_mode_probed_add(struct drm_connector *connector, struct drm_display_mode *mode);
extern void drm_mode_remove(struct drm_connector *connector, struct drm_display_mode *mode);
extern void drm_mode_copy(struct drm_display_mode *dst, const struct drm_display_mode *src);
extern struct drm_display_mode *drm_mode_duplicate(struct drm_device *dev,
						   const struct drm_display_mode *mode);
extern void drm_mode_debug_printmodeline(struct drm_display_mode *mode);
extern void drm_mode_config_init(struct drm_device *dev);
extern void drm_mode_config_reset(struct drm_device *dev);
extern void drm_mode_config_cleanup(struct drm_device *dev);
extern void drm_mode_set_name(struct drm_display_mode *mode);
extern bool drm_mode_equal(struct drm_display_mode *mode1, struct drm_display_mode *mode2);
extern int drm_mode_width(struct drm_display_mode *mode);
extern int drm_mode_height(struct drm_display_mode *mode);

/* for us by fb module */
extern int drm_mode_attachmode_crtc(struct drm_device *dev,
				    struct drm_crtc *crtc,
				    const struct drm_display_mode *mode);
extern int drm_mode_detachmode_crtc(struct drm_device *dev, struct drm_display_mode *mode);

extern struct drm_display_mode *drm_mode_create(struct drm_device *dev);
extern void drm_mode_destroy(struct drm_device *dev, struct drm_display_mode *mode);
extern void drm_mode_list_concat(struct list_head *head,
				 struct list_head *new);
extern void drm_mode_validate_size(struct drm_device *dev,
				   struct list_head *mode_list,
				   int maxX, int maxY, int maxPitch);
extern void drm_mode_prune_invalid(struct drm_device *dev,
				   struct list_head *mode_list, bool verbose);
extern void drm_mode_sort(struct list_head *mode_list);
extern int drm_mode_hsync(const struct drm_display_mode *mode);
extern int drm_mode_vrefresh(const struct drm_display_mode *mode);
extern void drm_mode_set_crtcinfo(struct drm_display_mode *p,
				  int adjust_flags);
extern void drm_mode_connector_list_update(struct drm_connector *connector);
extern int drm_mode_connector_update_edid_property(struct drm_connector *connector,
						struct edid *edid);
extern int drm_connector_property_set_value(struct drm_connector *connector,
					 struct drm_property *property,
					 uint64_t value);
extern int drm_connector_property_get_value(struct drm_connector *connector,
					 struct drm_property *property,
					 uint64_t *value);
extern struct drm_display_mode *drm_crtc_mode_create(struct drm_device *dev);
extern void drm_framebuffer_set_object(struct drm_device *dev,
				       unsigned long handle);
extern int drm_framebuffer_init(struct drm_device *dev,
				struct drm_framebuffer *fb,
				const struct drm_framebuffer_funcs *funcs);
extern void drm_framebuffer_cleanup(struct drm_framebuffer *fb);
extern int drmfb_probe(struct drm_device *dev, struct drm_crtc *crtc);
extern int drmfb_remove(struct drm_device *dev, struct drm_framebuffer *fb);
extern void drm_crtc_probe_connector_modes(struct drm_device *dev, int maxX, int maxY);
extern bool drm_crtc_in_use(struct drm_crtc *crtc);

extern int drm_connector_attach_property(struct drm_connector *connector,
				      struct drm_property *property, uint64_t init_val);
extern struct drm_property *drm_property_create(struct drm_device *dev, int flags,
						const char *name, int num_values);
extern struct drm_property *drm_property_create_enum(struct drm_device *dev, int flags,
					 const char *name,
					 const struct drm_prop_enum_list *props,
					 int num_values);
struct drm_property *drm_property_create_range(struct drm_device *dev, int flags,
					 const char *name,
					 uint64_t min, uint64_t max);
extern void drm_property_destroy(struct drm_device *dev, struct drm_property *property);
extern int drm_property_add_enum(struct drm_property *property, int index,
				 uint64_t value, const char *name);
extern int drm_mode_create_dvi_i_properties(struct drm_device *dev);
extern int drm_mode_create_tv_properties(struct drm_device *dev, int num_formats,
				     char *formats[]);
extern int drm_mode_create_scaling_mode_property(struct drm_device *dev);
extern int drm_mode_create_dithering_property(struct drm_device *dev);
extern int drm_mode_create_dirty_info_property(struct drm_device *dev);
extern char *drm_get_encoder_name(struct drm_encoder *encoder);

extern int drm_mode_connector_attach_encoder(struct drm_connector *connector,
					     struct drm_encoder *encoder);
extern void drm_mode_connector_detach_encoder(struct drm_connector *connector,
					   struct drm_encoder *encoder);
extern int drm_mode_crtc_set_gamma_size(struct drm_crtc *crtc,
					 int gamma_size);
extern struct drm_mode_object *drm_mode_object_find(struct drm_device *dev,
		uint32_t id, uint32_t type);
/* IOCTLs */
extern int drm_mode_getresources(struct drm_device *dev,
				 void *data, struct drm_file *file_priv);
extern int drm_mode_getplane_res(struct drm_device *dev, void *data,
				   struct drm_file *file_priv);
extern int drm_mode_getcrtc(struct drm_device *dev,
			    void *data, struct drm_file *file_priv);
extern int drm_mode_getconnector(struct drm_device *dev,
			      void *data, struct drm_file *file_priv);
extern int drm_mode_setcrtc(struct drm_device *dev,
			    void *data, struct drm_file *file_priv);
extern int drm_mode_getplane(struct drm_device *dev,
			       void *data, struct drm_file *file_priv);
extern int drm_mode_setplane(struct drm_device *dev,
			       void *data, struct drm_file *file_priv);
extern int drm_mode_cursor_ioctl(struct drm_device *dev,
				void *data, struct drm_file *file_priv);
extern int drm_mode_addfb(struct drm_device *dev,
			  void *data, struct drm_file *file_priv);
extern int drm_mode_addfb2(struct drm_device *dev,
			   void *data, struct drm_file *file_priv);
extern uint32_t drm_mode_legacy_fb_format(uint32_t bpp, uint32_t depth);
extern int drm_mode_rmfb(struct drm_device *dev,
			 void *data, struct drm_file *file_priv);
extern int drm_mode_getfb(struct drm_device *dev,
			  void *data, struct drm_file *file_priv);
extern int drm_mode_dirtyfb_ioctl(struct drm_device *dev,
				  void *data, struct drm_file *file_priv);
extern int drm_mode_addmode_ioctl(struct drm_device *dev,
				  void *data, struct drm_file *file_priv);
extern int drm_mode_rmmode_ioctl(struct drm_device *dev,
				 void *data, struct drm_file *file_priv);
extern int drm_mode_attachmode_ioctl(struct drm_device *dev,
				     void *data, struct drm_file *file_priv);
extern int drm_mode_detachmode_ioctl(struct drm_device *dev,
				     void *data, struct drm_file *file_priv);

extern int drm_mode_getproperty_ioctl(struct drm_device *dev,
				      void *data, struct drm_file *file_priv);
extern int drm_mode_getblob_ioctl(struct drm_device *dev,
				  void *data, struct drm_file *file_priv);
extern int drm_mode_connector_property_set_ioctl(struct drm_device *dev,
					      void *data, struct drm_file *file_priv);
extern int drm_mode_hotplug_ioctl(struct drm_device *dev,
				  void *data, struct drm_file *file_priv);
extern int drm_mode_replacefb(struct drm_device *dev,
			      void *data, struct drm_file *file_priv);
extern int drm_mode_getencoder(struct drm_device *dev,
			       void *data, struct drm_file *file_priv);
extern int drm_mode_gamma_get_ioctl(struct drm_device *dev,
				    void *data, struct drm_file *file_priv);
extern int drm_mode_gamma_set_ioctl(struct drm_device *dev,
				    void *data, struct drm_file *file_priv);
extern u8 *drm_find_cea_extension(struct edid *edid);
extern bool drm_detect_hdmi_monitor(struct edid *edid);
extern bool drm_detect_monitor_audio(struct edid *edid);
extern int drm_mode_page_flip_ioctl(struct drm_device *dev,
				    void *data, struct drm_file *file_priv);
extern struct drm_display_mode *drm_cvt_mode(struct drm_device *dev,
				int hdisplay, int vdisplay, int vrefresh,
				bool reduced, bool interlaced, bool margins);
extern struct drm_display_mode *drm_gtf_mode(struct drm_device *dev,
				int hdisplay, int vdisplay, int vrefresh,
				bool interlaced, int margins);
extern struct drm_display_mode *drm_gtf_mode_complex(struct drm_device *dev,
				int hdisplay, int vdisplay, int vrefresh,
				bool interlaced, int margins, int GTF_M,
				int GTF_2C, int GTF_K, int GTF_2J);
extern int drm_add_modes_noedid(struct drm_connector *connector,
				int hdisplay, int vdisplay);

extern int drm_edid_header_is_valid(const u8 *raw_edid);
extern bool drm_edid_block_valid(u8 *raw_edid);
extern bool drm_edid_is_valid(struct edid *edid);
struct drm_display_mode *drm_mode_find_dmt(struct drm_device *dev,
					   int hsize, int vsize, int fresh);

extern int drm_mode_create_dumb_ioctl(struct drm_device *dev,
				      void *data, struct drm_file *file_priv);
extern int drm_mode_mmap_dumb_ioctl(struct drm_device *dev,
				    void *data, struct drm_file *file_priv);
extern int drm_mode_destroy_dumb_ioctl(struct drm_device *dev,
				      void *data, struct drm_file *file_priv);

extern void drm_fb_get_bpp_depth(uint32_t format, unsigned int *depth,
				 int *bpp);
=======
#define obj_to_crtc(x) container_of(x, struct drm_crtc, base)

__printf(6, 7)
int drm_crtc_init_with_planes(struct drm_device *dev,
			      struct drm_crtc *crtc,
			      struct drm_plane *primary,
			      struct drm_plane *cursor,
			      const struct drm_crtc_funcs *funcs,
			      const char *name, ...);

__printf(6, 7)
int drmm_crtc_init_with_planes(struct drm_device *dev,
			       struct drm_crtc *crtc,
			       struct drm_plane *primary,
			       struct drm_plane *cursor,
			       const struct drm_crtc_funcs *funcs,
			       const char *name, ...);

void drm_crtc_cleanup(struct drm_crtc *crtc);

__printf(7, 8)
void *__drmm_crtc_alloc_with_planes(struct drm_device *dev,
				    size_t size, size_t offset,
				    struct drm_plane *primary,
				    struct drm_plane *cursor,
				    const struct drm_crtc_funcs *funcs,
				    const char *name, ...);

/**
 * drmm_crtc_alloc_with_planes - Allocate and initialize a new CRTC object with
 *    specified primary and cursor planes.
 * @dev: DRM device
 * @type: the type of the struct which contains struct &drm_crtc
 * @member: the name of the &drm_crtc within @type.
 * @primary: Primary plane for CRTC
 * @cursor: Cursor plane for CRTC
 * @funcs: callbacks for the new CRTC
 * @name: printf style format string for the CRTC name, or NULL for default name
 *
 * Allocates and initializes a new crtc object. Cleanup is automatically
 * handled through registering drmm_crtc_cleanup() with drmm_add_action().
 *
 * The @drm_crtc_funcs.destroy hook must be NULL.
 *
 * Returns:
 * Pointer to new crtc, or ERR_PTR on failure.
 */
#define drmm_crtc_alloc_with_planes(dev, type, member, primary, cursor, funcs, name, ...) \
	((type *)__drmm_crtc_alloc_with_planes(dev, sizeof(type), \
					       offsetof(type, member), \
					       primary, cursor, funcs, \
					       name, ##__VA_ARGS__))

/**
 * drm_crtc_index - find the index of a registered CRTC
 * @crtc: CRTC to find index for
 *
 * Given a registered CRTC, return the index of that CRTC within a DRM
 * device's list of CRTCs.
 */
static inline unsigned int drm_crtc_index(const struct drm_crtc *crtc)
{
	return crtc->index;
}

/**
 * drm_crtc_mask - find the mask of a registered CRTC
 * @crtc: CRTC to find mask for
 *
 * Given a registered CRTC, return the mask bit of that CRTC for the
 * &drm_encoder.possible_crtcs and &drm_plane.possible_crtcs fields.
 */
static inline uint32_t drm_crtc_mask(const struct drm_crtc *crtc)
{
	return 1 << drm_crtc_index(crtc);
}

int drm_mode_set_config_internal(struct drm_mode_set *set);
struct drm_crtc *drm_crtc_from_index(struct drm_device *dev, int idx);

/**
 * drm_crtc_find - look up a CRTC object from its ID
 * @dev: DRM device
 * @file_priv: drm file to check for lease against.
 * @id: &drm_mode_object ID
 *
 * This can be used to look up a CRTC from its userspace ID. Only used by
 * drivers for legacy IOCTLs and interface, nowadays extensions to the KMS
 * userspace interface should be done using &drm_property.
 */
static inline struct drm_crtc *drm_crtc_find(struct drm_device *dev,
		struct drm_file *file_priv,
		uint32_t id)
{
	struct drm_mode_object *mo;
	mo = drm_mode_object_find(dev, file_priv, id, DRM_MODE_OBJECT_CRTC);
	return mo ? obj_to_crtc(mo) : NULL;
}

/**
 * drm_for_each_crtc - iterate over all CRTCs
 * @crtc: a &struct drm_crtc as the loop cursor
 * @dev: the &struct drm_device
 *
 * Iterate over all CRTCs of @dev.
 */
#define drm_for_each_crtc(crtc, dev) \
	list_for_each_entry(crtc, &(dev)->mode_config.crtc_list, head)

/**
 * drm_for_each_crtc_reverse - iterate over all CRTCs in reverse order
 * @crtc: a &struct drm_crtc as the loop cursor
 * @dev: the &struct drm_device
 *
 * Iterate over all CRTCs of @dev.
 */
#define drm_for_each_crtc_reverse(crtc, dev) \
	list_for_each_entry_reverse(crtc, &(dev)->mode_config.crtc_list, head)

int drm_crtc_create_scaling_filter_property(struct drm_crtc *crtc,
					    unsigned int supported_filters);

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#endif /* __DRM_CRTC_H__ */
