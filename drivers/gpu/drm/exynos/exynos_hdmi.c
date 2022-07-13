<<<<<<< HEAD
=======
// SPDX-License-Identifier: GPL-2.0-or-later
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
/*
 * Copyright (C) 2011 Samsung Electronics Co.Ltd
 * Authors:
 * Seung-Woo Kim <sw0312.kim@samsung.com>
 *	Inki Dae <inki.dae@samsung.com>
 *	Joonyoung Shim <jy0922.shim@samsung.com>
 *
 * Based on drivers/media/video/s5p-tv/hdmi_drv.c
<<<<<<< HEAD
 *
 * This program is free software; you can redistribute  it and/or modify it
 * under  the terms of  the GNU General  Public License as published by the
 * Free Software Foundation;  either version 2 of the  License, or (at your
 * option) any later version.
 *
 */

#include "drmP.h"
#include "drm_edid.h"
#include "drm_crtc_helper.h"

#include "regs-hdmi.h"

#include <linux/kernel.h>
#include <linux/spinlock.h>
#include <linux/wait.h>
#include <linux/i2c.h>
#include <linux/module.h>
#include <linux/platform_device.h>
#include <linux/interrupt.h>
#include <linux/irq.h>
#include <linux/delay.h>
#include <linux/pm_runtime.h>
#include <linux/clk.h>
#include <linux/regulator/consumer.h>

#include <drm/exynos_drm.h>

#include "exynos_drm_drv.h"
#include "exynos_drm_hdmi.h"

#include "exynos_hdmi.h"

#define MAX_WIDTH		1920
#define MAX_HEIGHT		1080
#define get_hdmi_context(dev)	platform_get_drvdata(to_platform_device(dev))

struct hdmi_resources {
	struct clk			*hdmi;
	struct clk			*sclk_hdmi;
	struct clk			*sclk_pixel;
	struct clk			*sclk_hdmiphy;
	struct clk			*hdmiphy;
	struct regulator_bulk_data	*regul_bulk;
	int				regul_count;
};

struct hdmi_context {
	struct device			*dev;
	struct drm_device		*drm_dev;
	struct fb_videomode		*default_timing;
	unsigned int			is_v13:1;
	unsigned int			default_win;
	unsigned int			default_bpp;
	bool				hpd_handle;
	bool				enabled;

	struct resource			*regs_res;
	void __iomem			*regs;
	unsigned int			irq;
	struct workqueue_struct		*wq;
	struct work_struct		hotplug_work;

	struct i2c_client		*ddc_port;
	struct i2c_client		*hdmiphy_port;

	/* current hdmiphy conf index */
	int cur_conf;

	struct hdmi_resources		res;
	void				*parent_ctx;
};

/* HDMI Version 1.3 */
static const u8 hdmiphy_v13_conf27[32] = {
	0x01, 0x05, 0x00, 0xD8, 0x10, 0x1C, 0x30, 0x40,
	0x6B, 0x10, 0x02, 0x51, 0xDF, 0xF2, 0x54, 0x87,
	0x84, 0x00, 0x30, 0x38, 0x00, 0x08, 0x10, 0xE0,
	0x22, 0x40, 0xE3, 0x26, 0x00, 0x00, 0x00, 0x00,
};

static const u8 hdmiphy_v13_conf27_027[32] = {
	0x01, 0x05, 0x00, 0xD4, 0x10, 0x9C, 0x09, 0x64,
	0x6B, 0x10, 0x02, 0x51, 0xDF, 0xF2, 0x54, 0x87,
	0x84, 0x00, 0x30, 0x38, 0x00, 0x08, 0x10, 0xE0,
	0x22, 0x40, 0xE3, 0x26, 0x00, 0x00, 0x00, 0x00,
};

static const u8 hdmiphy_v13_conf74_175[32] = {
	0x01, 0x05, 0x00, 0xD8, 0x10, 0x9C, 0xef, 0x5B,
	0x6D, 0x10, 0x01, 0x51, 0xef, 0xF3, 0x54, 0xb9,
	0x84, 0x00, 0x30, 0x38, 0x00, 0x08, 0x10, 0xE0,
	0x22, 0x40, 0xa5, 0x26, 0x01, 0x00, 0x00, 0x00,
};

static const u8 hdmiphy_v13_conf74_25[32] = {
	0x01, 0x05, 0x00, 0xd8, 0x10, 0x9c, 0xf8, 0x40,
	0x6a, 0x10, 0x01, 0x51, 0xff, 0xf1, 0x54, 0xba,
	0x84, 0x00, 0x10, 0x38, 0x00, 0x08, 0x10, 0xe0,
	0x22, 0x40, 0xa4, 0x26, 0x01, 0x00, 0x00, 0x00,
};

static const u8 hdmiphy_v13_conf148_5[32] = {
	0x01, 0x05, 0x00, 0xD8, 0x10, 0x9C, 0xf8, 0x40,
	0x6A, 0x18, 0x00, 0x51, 0xff, 0xF1, 0x54, 0xba,
	0x84, 0x00, 0x10, 0x38, 0x00, 0x08, 0x10, 0xE0,
	0x22, 0x40, 0xa4, 0x26, 0x02, 0x00, 0x00, 0x00,
};

struct hdmi_v13_tg_regs {
	u8 cmd;
	u8 h_fsz_l;
	u8 h_fsz_h;
	u8 hact_st_l;
	u8 hact_st_h;
	u8 hact_sz_l;
	u8 hact_sz_h;
	u8 v_fsz_l;
	u8 v_fsz_h;
	u8 vsync_l;
	u8 vsync_h;
	u8 vsync2_l;
	u8 vsync2_h;
	u8 vact_st_l;
	u8 vact_st_h;
	u8 vact_sz_l;
	u8 vact_sz_h;
	u8 field_chg_l;
	u8 field_chg_h;
	u8 vact_st2_l;
	u8 vact_st2_h;
	u8 vsync_top_hdmi_l;
	u8 vsync_top_hdmi_h;
	u8 vsync_bot_hdmi_l;
	u8 vsync_bot_hdmi_h;
	u8 field_top_hdmi_l;
	u8 field_top_hdmi_h;
	u8 field_bot_hdmi_l;
	u8 field_bot_hdmi_h;
};

struct hdmi_v13_core_regs {
	u8 h_blank[2];
	u8 v_blank[3];
	u8 h_v_line[3];
	u8 vsync_pol[1];
	u8 int_pro_mode[1];
	u8 v_blank_f[3];
	u8 h_sync_gen[3];
	u8 v_sync_gen1[3];
	u8 v_sync_gen2[3];
	u8 v_sync_gen3[3];
};

struct hdmi_v13_preset_conf {
	struct hdmi_v13_core_regs core;
	struct hdmi_v13_tg_regs tg;
};

struct hdmi_v13_conf {
	int width;
	int height;
	int vrefresh;
	bool interlace;
	const u8 *hdmiphy_data;
	const struct hdmi_v13_preset_conf *conf;
};

static const struct hdmi_v13_preset_conf hdmi_v13_conf_480p = {
	.core = {
		.h_blank = {0x8a, 0x00},
		.v_blank = {0x0d, 0x6a, 0x01},
		.h_v_line = {0x0d, 0xa2, 0x35},
		.vsync_pol = {0x01},
		.int_pro_mode = {0x00},
		.v_blank_f = {0x00, 0x00, 0x00},
		.h_sync_gen = {0x0e, 0x30, 0x11},
		.v_sync_gen1 = {0x0f, 0x90, 0x00},
		/* other don't care */
	},
	.tg = {
		0x00, /* cmd */
		0x5a, 0x03, /* h_fsz */
		0x8a, 0x00, 0xd0, 0x02, /* hact */
		0x0d, 0x02, /* v_fsz */
		0x01, 0x00, 0x33, 0x02, /* vsync */
		0x2d, 0x00, 0xe0, 0x01, /* vact */
		0x33, 0x02, /* field_chg */
		0x49, 0x02, /* vact_st2 */
		0x01, 0x00, 0x33, 0x02, /* vsync top/bot */
		0x01, 0x00, 0x33, 0x02, /* field top/bot */
	},
};

static const struct hdmi_v13_preset_conf hdmi_v13_conf_720p60 = {
	.core = {
		.h_blank = {0x72, 0x01},
		.v_blank = {0xee, 0xf2, 0x00},
		.h_v_line = {0xee, 0x22, 0x67},
		.vsync_pol = {0x00},
		.int_pro_mode = {0x00},
		.v_blank_f = {0x00, 0x00, 0x00}, /* don't care */
		.h_sync_gen = {0x6c, 0x50, 0x02},
		.v_sync_gen1 = {0x0a, 0x50, 0x00},
		.v_sync_gen2 = {0x01, 0x10, 0x00},
		.v_sync_gen3 = {0x01, 0x10, 0x00},
		/* other don't care */
	},
	.tg = {
		0x00, /* cmd */
		0x72, 0x06, /* h_fsz */
		0x71, 0x01, 0x01, 0x05, /* hact */
		0xee, 0x02, /* v_fsz */
		0x01, 0x00, 0x33, 0x02, /* vsync */
		0x1e, 0x00, 0xd0, 0x02, /* vact */
		0x33, 0x02, /* field_chg */
		0x49, 0x02, /* vact_st2 */
		0x01, 0x00, 0x01, 0x00, /* vsync top/bot */
		0x01, 0x00, 0x33, 0x02, /* field top/bot */
	},
};

static const struct hdmi_v13_preset_conf hdmi_v13_conf_1080i50 = {
	.core = {
		.h_blank = {0xd0, 0x02},
		.v_blank = {0x32, 0xB2, 0x00},
		.h_v_line = {0x65, 0x04, 0xa5},
		.vsync_pol = {0x00},
		.int_pro_mode = {0x01},
		.v_blank_f = {0x49, 0x2A, 0x23},
		.h_sync_gen = {0x0E, 0xEA, 0x08},
		.v_sync_gen1 = {0x07, 0x20, 0x00},
		.v_sync_gen2 = {0x39, 0x42, 0x23},
		.v_sync_gen3 = {0x38, 0x87, 0x73},
		/* other don't care */
	},
	.tg = {
		0x00, /* cmd */
		0x50, 0x0A, /* h_fsz */
		0xCF, 0x02, 0x81, 0x07, /* hact */
		0x65, 0x04, /* v_fsz */
		0x01, 0x00, 0x33, 0x02, /* vsync */
		0x16, 0x00, 0x1c, 0x02, /* vact */
		0x33, 0x02, /* field_chg */
		0x49, 0x02, /* vact_st2 */
		0x01, 0x00, 0x33, 0x02, /* vsync top/bot */
		0x01, 0x00, 0x33, 0x02, /* field top/bot */
	},
};

static const struct hdmi_v13_preset_conf hdmi_v13_conf_1080p50 = {
	.core = {
		.h_blank = {0xd0, 0x02},
		.v_blank = {0x65, 0x6c, 0x01},
		.h_v_line = {0x65, 0x04, 0xa5},
		.vsync_pol = {0x00},
		.int_pro_mode = {0x00},
		.v_blank_f = {0x00, 0x00, 0x00}, /* don't care */
		.h_sync_gen = {0x0e, 0xea, 0x08},
		.v_sync_gen1 = {0x09, 0x40, 0x00},
		.v_sync_gen2 = {0x01, 0x10, 0x00},
		.v_sync_gen3 = {0x01, 0x10, 0x00},
		/* other don't care */
	},
	.tg = {
		0x00, /* cmd */
		0x50, 0x0A, /* h_fsz */
		0xCF, 0x02, 0x81, 0x07, /* hact */
		0x65, 0x04, /* v_fsz */
		0x01, 0x00, 0x33, 0x02, /* vsync */
		0x2d, 0x00, 0x38, 0x04, /* vact */
		0x33, 0x02, /* field_chg */
		0x48, 0x02, /* vact_st2 */
		0x01, 0x00, 0x01, 0x00, /* vsync top/bot */
		0x01, 0x00, 0x33, 0x02, /* field top/bot */
	},
};

static const struct hdmi_v13_preset_conf hdmi_v13_conf_1080i60 = {
	.core = {
		.h_blank = {0x18, 0x01},
		.v_blank = {0x32, 0xB2, 0x00},
		.h_v_line = {0x65, 0x84, 0x89},
		.vsync_pol = {0x00},
		.int_pro_mode = {0x01},
		.v_blank_f = {0x49, 0x2A, 0x23},
		.h_sync_gen = {0x56, 0x08, 0x02},
		.v_sync_gen1 = {0x07, 0x20, 0x00},
		.v_sync_gen2 = {0x39, 0x42, 0x23},
		.v_sync_gen3 = {0xa4, 0x44, 0x4a},
		/* other don't care */
	},
	.tg = {
		0x00, /* cmd */
		0x98, 0x08, /* h_fsz */
		0x17, 0x01, 0x81, 0x07, /* hact */
		0x65, 0x04, /* v_fsz */
		0x01, 0x00, 0x33, 0x02, /* vsync */
		0x16, 0x00, 0x1c, 0x02, /* vact */
		0x33, 0x02, /* field_chg */
		0x49, 0x02, /* vact_st2 */
		0x01, 0x00, 0x33, 0x02, /* vsync top/bot */
		0x01, 0x00, 0x33, 0x02, /* field top/bot */
	},
};

static const struct hdmi_v13_preset_conf hdmi_v13_conf_1080p60 = {
	.core = {
		.h_blank = {0x18, 0x01},
		.v_blank = {0x65, 0x6c, 0x01},
		.h_v_line = {0x65, 0x84, 0x89},
		.vsync_pol = {0x00},
		.int_pro_mode = {0x00},
		.v_blank_f = {0x00, 0x00, 0x00}, /* don't care */
		.h_sync_gen = {0x56, 0x08, 0x02},
		.v_sync_gen1 = {0x09, 0x40, 0x00},
		.v_sync_gen2 = {0x01, 0x10, 0x00},
		.v_sync_gen3 = {0x01, 0x10, 0x00},
		/* other don't care */
	},
	.tg = {
		0x00, /* cmd */
		0x98, 0x08, /* h_fsz */
		0x17, 0x01, 0x81, 0x07, /* hact */
		0x65, 0x04, /* v_fsz */
		0x01, 0x00, 0x33, 0x02, /* vsync */
		0x2d, 0x00, 0x38, 0x04, /* vact */
		0x33, 0x02, /* field_chg */
		0x48, 0x02, /* vact_st2 */
		0x01, 0x00, 0x01, 0x00, /* vsync top/bot */
		0x01, 0x00, 0x33, 0x02, /* field top/bot */
	},
};

static const struct hdmi_v13_conf hdmi_v13_confs[] = {
	{ 1280, 720, 60, false, hdmiphy_v13_conf74_25, &hdmi_v13_conf_720p60 },
	{ 1280, 720, 50, false, hdmiphy_v13_conf74_25, &hdmi_v13_conf_720p60 },
	{ 720, 480, 60, false, hdmiphy_v13_conf27_027, &hdmi_v13_conf_480p },
	{ 1920, 1080, 50, true, hdmiphy_v13_conf74_25, &hdmi_v13_conf_1080i50 },
	{ 1920, 1080, 50, false, hdmiphy_v13_conf148_5,
				 &hdmi_v13_conf_1080p50 },
	{ 1920, 1080, 60, true, hdmiphy_v13_conf74_25, &hdmi_v13_conf_1080i60 },
	{ 1920, 1080, 60, false, hdmiphy_v13_conf148_5,
				 &hdmi_v13_conf_1080p60 },
};

/* HDMI Version 1.4 */
static const u8 hdmiphy_conf27_027[32] = {
	0x01, 0xd1, 0x2d, 0x72, 0x40, 0x64, 0x12, 0x08,
	0x43, 0xa0, 0x0e, 0xd9, 0x45, 0xa0, 0xac, 0x80,
	0x08, 0x80, 0x11, 0x04, 0x02, 0x22, 0x44, 0x86,
	0x54, 0xe3, 0x24, 0x00, 0x00, 0x00, 0x01, 0x00,
};

static const u8 hdmiphy_conf74_25[32] = {
	0x01, 0xd1, 0x1f, 0x10, 0x40, 0x40, 0xf8, 0x08,
	0x81, 0xa0, 0xba, 0xd8, 0x45, 0xa0, 0xac, 0x80,
	0x3c, 0x80, 0x11, 0x04, 0x02, 0x22, 0x44, 0x86,
	0x54, 0xa5, 0x24, 0x01, 0x00, 0x00, 0x01, 0x00,
};

static const u8 hdmiphy_conf148_5[32] = {
	0x01, 0xd1, 0x1f, 0x00, 0x40, 0x40, 0xf8, 0x08,
	0x81, 0xa0, 0xba, 0xd8, 0x45, 0xa0, 0xac, 0x80,
	0x3c, 0x80, 0x11, 0x04, 0x02, 0x22, 0x44, 0x86,
	0x54, 0x4b, 0x25, 0x03, 0x00, 0x00, 0x01, 0x00,
};

struct hdmi_tg_regs {
	u8 cmd;
	u8 h_fsz_l;
	u8 h_fsz_h;
	u8 hact_st_l;
	u8 hact_st_h;
	u8 hact_sz_l;
	u8 hact_sz_h;
	u8 v_fsz_l;
	u8 v_fsz_h;
	u8 vsync_l;
	u8 vsync_h;
	u8 vsync2_l;
	u8 vsync2_h;
	u8 vact_st_l;
	u8 vact_st_h;
	u8 vact_sz_l;
	u8 vact_sz_h;
	u8 field_chg_l;
	u8 field_chg_h;
	u8 vact_st2_l;
	u8 vact_st2_h;
	u8 vact_st3_l;
	u8 vact_st3_h;
	u8 vact_st4_l;
	u8 vact_st4_h;
	u8 vsync_top_hdmi_l;
	u8 vsync_top_hdmi_h;
	u8 vsync_bot_hdmi_l;
	u8 vsync_bot_hdmi_h;
	u8 field_top_hdmi_l;
	u8 field_top_hdmi_h;
	u8 field_bot_hdmi_l;
	u8 field_bot_hdmi_h;
	u8 tg_3d;
};

struct hdmi_core_regs {
	u8 h_blank[2];
	u8 v2_blank[2];
	u8 v1_blank[2];
	u8 v_line[2];
	u8 h_line[2];
	u8 hsync_pol[1];
	u8 vsync_pol[1];
	u8 int_pro_mode[1];
	u8 v_blank_f0[2];
	u8 v_blank_f1[2];
	u8 h_sync_start[2];
	u8 h_sync_end[2];
	u8 v_sync_line_bef_2[2];
	u8 v_sync_line_bef_1[2];
	u8 v_sync_line_aft_2[2];
	u8 v_sync_line_aft_1[2];
	u8 v_sync_line_aft_pxl_2[2];
	u8 v_sync_line_aft_pxl_1[2];
	u8 v_blank_f2[2]; /* for 3D mode */
	u8 v_blank_f3[2]; /* for 3D mode */
	u8 v_blank_f4[2]; /* for 3D mode */
	u8 v_blank_f5[2]; /* for 3D mode */
	u8 v_sync_line_aft_3[2];
	u8 v_sync_line_aft_4[2];
	u8 v_sync_line_aft_5[2];
	u8 v_sync_line_aft_6[2];
	u8 v_sync_line_aft_pxl_3[2];
	u8 v_sync_line_aft_pxl_4[2];
	u8 v_sync_line_aft_pxl_5[2];
	u8 v_sync_line_aft_pxl_6[2];
	u8 vact_space_1[2];
	u8 vact_space_2[2];
	u8 vact_space_3[2];
	u8 vact_space_4[2];
	u8 vact_space_5[2];
	u8 vact_space_6[2];
};

struct hdmi_preset_conf {
	struct hdmi_core_regs core;
	struct hdmi_tg_regs tg;
};

struct hdmi_conf {
	int width;
	int height;
	int vrefresh;
	bool interlace;
	const u8 *hdmiphy_data;
	const struct hdmi_preset_conf *conf;
};

static const struct hdmi_preset_conf hdmi_conf_480p60 = {
	.core = {
		.h_blank = {0x8a, 0x00},
		.v2_blank = {0x0d, 0x02},
		.v1_blank = {0x2d, 0x00},
		.v_line = {0x0d, 0x02},
		.h_line = {0x5a, 0x03},
		.hsync_pol = {0x01},
		.vsync_pol = {0x01},
		.int_pro_mode = {0x00},
		.v_blank_f0 = {0xff, 0xff},
		.v_blank_f1 = {0xff, 0xff},
		.h_sync_start = {0x0e, 0x00},
		.h_sync_end = {0x4c, 0x00},
		.v_sync_line_bef_2 = {0x0f, 0x00},
		.v_sync_line_bef_1 = {0x09, 0x00},
		.v_sync_line_aft_2 = {0xff, 0xff},
		.v_sync_line_aft_1 = {0xff, 0xff},
		.v_sync_line_aft_pxl_2 = {0xff, 0xff},
		.v_sync_line_aft_pxl_1 = {0xff, 0xff},
		.v_blank_f2 = {0xff, 0xff},
		.v_blank_f3 = {0xff, 0xff},
		.v_blank_f4 = {0xff, 0xff},
		.v_blank_f5 = {0xff, 0xff},
		.v_sync_line_aft_3 = {0xff, 0xff},
		.v_sync_line_aft_4 = {0xff, 0xff},
		.v_sync_line_aft_5 = {0xff, 0xff},
		.v_sync_line_aft_6 = {0xff, 0xff},
		.v_sync_line_aft_pxl_3 = {0xff, 0xff},
		.v_sync_line_aft_pxl_4 = {0xff, 0xff},
		.v_sync_line_aft_pxl_5 = {0xff, 0xff},
		.v_sync_line_aft_pxl_6 = {0xff, 0xff},
		.vact_space_1 = {0xff, 0xff},
		.vact_space_2 = {0xff, 0xff},
		.vact_space_3 = {0xff, 0xff},
		.vact_space_4 = {0xff, 0xff},
		.vact_space_5 = {0xff, 0xff},
		.vact_space_6 = {0xff, 0xff},
		/* other don't care */
	},
	.tg = {
		0x00, /* cmd */
		0x5a, 0x03, /* h_fsz */
		0x8a, 0x00, 0xd0, 0x02, /* hact */
		0x0d, 0x02, /* v_fsz */
		0x01, 0x00, 0x33, 0x02, /* vsync */
		0x2d, 0x00, 0xe0, 0x01, /* vact */
		0x33, 0x02, /* field_chg */
		0x48, 0x02, /* vact_st2 */
		0x00, 0x00, /* vact_st3 */
		0x00, 0x00, /* vact_st4 */
		0x01, 0x00, 0x01, 0x00, /* vsync top/bot */
		0x01, 0x00, 0x33, 0x02, /* field top/bot */
		0x00, /* 3d FP */
	},
};

static const struct hdmi_preset_conf hdmi_conf_720p50 = {
	.core = {
		.h_blank = {0xbc, 0x02},
		.v2_blank = {0xee, 0x02},
		.v1_blank = {0x1e, 0x00},
		.v_line = {0xee, 0x02},
		.h_line = {0xbc, 0x07},
		.hsync_pol = {0x00},
		.vsync_pol = {0x00},
		.int_pro_mode = {0x00},
		.v_blank_f0 = {0xff, 0xff},
		.v_blank_f1 = {0xff, 0xff},
		.h_sync_start = {0xb6, 0x01},
		.h_sync_end = {0xde, 0x01},
		.v_sync_line_bef_2 = {0x0a, 0x00},
		.v_sync_line_bef_1 = {0x05, 0x00},
		.v_sync_line_aft_2 = {0xff, 0xff},
		.v_sync_line_aft_1 = {0xff, 0xff},
		.v_sync_line_aft_pxl_2 = {0xff, 0xff},
		.v_sync_line_aft_pxl_1 = {0xff, 0xff},
		.v_blank_f2 = {0xff, 0xff},
		.v_blank_f3 = {0xff, 0xff},
		.v_blank_f4 = {0xff, 0xff},
		.v_blank_f5 = {0xff, 0xff},
		.v_sync_line_aft_3 = {0xff, 0xff},
		.v_sync_line_aft_4 = {0xff, 0xff},
		.v_sync_line_aft_5 = {0xff, 0xff},
		.v_sync_line_aft_6 = {0xff, 0xff},
		.v_sync_line_aft_pxl_3 = {0xff, 0xff},
		.v_sync_line_aft_pxl_4 = {0xff, 0xff},
		.v_sync_line_aft_pxl_5 = {0xff, 0xff},
		.v_sync_line_aft_pxl_6 = {0xff, 0xff},
		.vact_space_1 = {0xff, 0xff},
		.vact_space_2 = {0xff, 0xff},
		.vact_space_3 = {0xff, 0xff},
		.vact_space_4 = {0xff, 0xff},
		.vact_space_5 = {0xff, 0xff},
		.vact_space_6 = {0xff, 0xff},
		/* other don't care */
	},
	.tg = {
		0x00, /* cmd */
		0xbc, 0x07, /* h_fsz */
		0xbc, 0x02, 0x00, 0x05, /* hact */
		0xee, 0x02, /* v_fsz */
		0x01, 0x00, 0x33, 0x02, /* vsync */
		0x1e, 0x00, 0xd0, 0x02, /* vact */
		0x33, 0x02, /* field_chg */
		0x48, 0x02, /* vact_st2 */
		0x00, 0x00, /* vact_st3 */
		0x00, 0x00, /* vact_st4 */
		0x01, 0x00, 0x01, 0x00, /* vsync top/bot */
		0x01, 0x00, 0x33, 0x02, /* field top/bot */
		0x00, /* 3d FP */
	},
};

static const struct hdmi_preset_conf hdmi_conf_720p60 = {
	.core = {
		.h_blank = {0x72, 0x01},
		.v2_blank = {0xee, 0x02},
		.v1_blank = {0x1e, 0x00},
		.v_line = {0xee, 0x02},
		.h_line = {0x72, 0x06},
		.hsync_pol = {0x00},
		.vsync_pol = {0x00},
		.int_pro_mode = {0x00},
		.v_blank_f0 = {0xff, 0xff},
		.v_blank_f1 = {0xff, 0xff},
		.h_sync_start = {0x6c, 0x00},
		.h_sync_end = {0x94, 0x00},
		.v_sync_line_bef_2 = {0x0a, 0x00},
		.v_sync_line_bef_1 = {0x05, 0x00},
		.v_sync_line_aft_2 = {0xff, 0xff},
		.v_sync_line_aft_1 = {0xff, 0xff},
		.v_sync_line_aft_pxl_2 = {0xff, 0xff},
		.v_sync_line_aft_pxl_1 = {0xff, 0xff},
		.v_blank_f2 = {0xff, 0xff},
		.v_blank_f3 = {0xff, 0xff},
		.v_blank_f4 = {0xff, 0xff},
		.v_blank_f5 = {0xff, 0xff},
		.v_sync_line_aft_3 = {0xff, 0xff},
		.v_sync_line_aft_4 = {0xff, 0xff},
		.v_sync_line_aft_5 = {0xff, 0xff},
		.v_sync_line_aft_6 = {0xff, 0xff},
		.v_sync_line_aft_pxl_3 = {0xff, 0xff},
		.v_sync_line_aft_pxl_4 = {0xff, 0xff},
		.v_sync_line_aft_pxl_5 = {0xff, 0xff},
		.v_sync_line_aft_pxl_6 = {0xff, 0xff},
		.vact_space_1 = {0xff, 0xff},
		.vact_space_2 = {0xff, 0xff},
		.vact_space_3 = {0xff, 0xff},
		.vact_space_4 = {0xff, 0xff},
		.vact_space_5 = {0xff, 0xff},
		.vact_space_6 = {0xff, 0xff},
		/* other don't care */
	},
	.tg = {
		0x00, /* cmd */
		0x72, 0x06, /* h_fsz */
		0x72, 0x01, 0x00, 0x05, /* hact */
		0xee, 0x02, /* v_fsz */
		0x01, 0x00, 0x33, 0x02, /* vsync */
		0x1e, 0x00, 0xd0, 0x02, /* vact */
		0x33, 0x02, /* field_chg */
		0x48, 0x02, /* vact_st2 */
		0x00, 0x00, /* vact_st3 */
		0x00, 0x00, /* vact_st4 */
		0x01, 0x00, 0x01, 0x00, /* vsync top/bot */
		0x01, 0x00, 0x33, 0x02, /* field top/bot */
		0x00, /* 3d FP */
	},
};

static const struct hdmi_preset_conf hdmi_conf_1080i50 = {
	.core = {
		.h_blank = {0xd0, 0x02},
		.v2_blank = {0x32, 0x02},
		.v1_blank = {0x16, 0x00},
		.v_line = {0x65, 0x04},
		.h_line = {0x50, 0x0a},
		.hsync_pol = {0x00},
		.vsync_pol = {0x00},
		.int_pro_mode = {0x01},
		.v_blank_f0 = {0x49, 0x02},
		.v_blank_f1 = {0x65, 0x04},
		.h_sync_start = {0x0e, 0x02},
		.h_sync_end = {0x3a, 0x02},
		.v_sync_line_bef_2 = {0x07, 0x00},
		.v_sync_line_bef_1 = {0x02, 0x00},
		.v_sync_line_aft_2 = {0x39, 0x02},
		.v_sync_line_aft_1 = {0x34, 0x02},
		.v_sync_line_aft_pxl_2 = {0x38, 0x07},
		.v_sync_line_aft_pxl_1 = {0x38, 0x07},
		.v_blank_f2 = {0xff, 0xff},
		.v_blank_f3 = {0xff, 0xff},
		.v_blank_f4 = {0xff, 0xff},
		.v_blank_f5 = {0xff, 0xff},
		.v_sync_line_aft_3 = {0xff, 0xff},
		.v_sync_line_aft_4 = {0xff, 0xff},
		.v_sync_line_aft_5 = {0xff, 0xff},
		.v_sync_line_aft_6 = {0xff, 0xff},
		.v_sync_line_aft_pxl_3 = {0xff, 0xff},
		.v_sync_line_aft_pxl_4 = {0xff, 0xff},
		.v_sync_line_aft_pxl_5 = {0xff, 0xff},
		.v_sync_line_aft_pxl_6 = {0xff, 0xff},
		.vact_space_1 = {0xff, 0xff},
		.vact_space_2 = {0xff, 0xff},
		.vact_space_3 = {0xff, 0xff},
		.vact_space_4 = {0xff, 0xff},
		.vact_space_5 = {0xff, 0xff},
		.vact_space_6 = {0xff, 0xff},
		/* other don't care */
	},
	.tg = {
		0x00, /* cmd */
		0x50, 0x0a, /* h_fsz */
		0xd0, 0x02, 0x80, 0x07, /* hact */
		0x65, 0x04, /* v_fsz */
		0x01, 0x00, 0x33, 0x02, /* vsync */
		0x16, 0x00, 0x1c, 0x02, /* vact */
		0x33, 0x02, /* field_chg */
		0x49, 0x02, /* vact_st2 */
		0x00, 0x00, /* vact_st3 */
		0x00, 0x00, /* vact_st4 */
		0x01, 0x00, 0x33, 0x02, /* vsync top/bot */
		0x01, 0x00, 0x33, 0x02, /* field top/bot */
		0x00, /* 3d FP */
	},
};

static const struct hdmi_preset_conf hdmi_conf_1080i60 = {
	.core = {
		.h_blank = {0x18, 0x01},
		.v2_blank = {0x32, 0x02},
		.v1_blank = {0x16, 0x00},
		.v_line = {0x65, 0x04},
		.h_line = {0x98, 0x08},
		.hsync_pol = {0x00},
		.vsync_pol = {0x00},
		.int_pro_mode = {0x01},
		.v_blank_f0 = {0x49, 0x02},
		.v_blank_f1 = {0x65, 0x04},
		.h_sync_start = {0x56, 0x00},
		.h_sync_end = {0x82, 0x00},
		.v_sync_line_bef_2 = {0x07, 0x00},
		.v_sync_line_bef_1 = {0x02, 0x00},
		.v_sync_line_aft_2 = {0x39, 0x02},
		.v_sync_line_aft_1 = {0x34, 0x02},
		.v_sync_line_aft_pxl_2 = {0xa4, 0x04},
		.v_sync_line_aft_pxl_1 = {0xa4, 0x04},
		.v_blank_f2 = {0xff, 0xff},
		.v_blank_f3 = {0xff, 0xff},
		.v_blank_f4 = {0xff, 0xff},
		.v_blank_f5 = {0xff, 0xff},
		.v_sync_line_aft_3 = {0xff, 0xff},
		.v_sync_line_aft_4 = {0xff, 0xff},
		.v_sync_line_aft_5 = {0xff, 0xff},
		.v_sync_line_aft_6 = {0xff, 0xff},
		.v_sync_line_aft_pxl_3 = {0xff, 0xff},
		.v_sync_line_aft_pxl_4 = {0xff, 0xff},
		.v_sync_line_aft_pxl_5 = {0xff, 0xff},
		.v_sync_line_aft_pxl_6 = {0xff, 0xff},
		.vact_space_1 = {0xff, 0xff},
		.vact_space_2 = {0xff, 0xff},
		.vact_space_3 = {0xff, 0xff},
		.vact_space_4 = {0xff, 0xff},
		.vact_space_5 = {0xff, 0xff},
		.vact_space_6 = {0xff, 0xff},
		/* other don't care */
	},
	.tg = {
		0x00, /* cmd */
		0x98, 0x08, /* h_fsz */
		0x18, 0x01, 0x80, 0x07, /* hact */
		0x65, 0x04, /* v_fsz */
		0x01, 0x00, 0x33, 0x02, /* vsync */
		0x16, 0x00, 0x1c, 0x02, /* vact */
		0x33, 0x02, /* field_chg */
		0x49, 0x02, /* vact_st2 */
		0x00, 0x00, /* vact_st3 */
		0x00, 0x00, /* vact_st4 */
		0x01, 0x00, 0x33, 0x02, /* vsync top/bot */
		0x01, 0x00, 0x33, 0x02, /* field top/bot */
		0x00, /* 3d FP */
	},
};

static const struct hdmi_preset_conf hdmi_conf_1080p50 = {
	.core = {
		.h_blank = {0xd0, 0x02},
		.v2_blank = {0x65, 0x04},
		.v1_blank = {0x2d, 0x00},
		.v_line = {0x65, 0x04},
		.h_line = {0x50, 0x0a},
		.hsync_pol = {0x00},
		.vsync_pol = {0x00},
		.int_pro_mode = {0x00},
		.v_blank_f0 = {0xff, 0xff},
		.v_blank_f1 = {0xff, 0xff},
		.h_sync_start = {0x0e, 0x02},
		.h_sync_end = {0x3a, 0x02},
		.v_sync_line_bef_2 = {0x09, 0x00},
		.v_sync_line_bef_1 = {0x04, 0x00},
		.v_sync_line_aft_2 = {0xff, 0xff},
		.v_sync_line_aft_1 = {0xff, 0xff},
		.v_sync_line_aft_pxl_2 = {0xff, 0xff},
		.v_sync_line_aft_pxl_1 = {0xff, 0xff},
		.v_blank_f2 = {0xff, 0xff},
		.v_blank_f3 = {0xff, 0xff},
		.v_blank_f4 = {0xff, 0xff},
		.v_blank_f5 = {0xff, 0xff},
		.v_sync_line_aft_3 = {0xff, 0xff},
		.v_sync_line_aft_4 = {0xff, 0xff},
		.v_sync_line_aft_5 = {0xff, 0xff},
		.v_sync_line_aft_6 = {0xff, 0xff},
		.v_sync_line_aft_pxl_3 = {0xff, 0xff},
		.v_sync_line_aft_pxl_4 = {0xff, 0xff},
		.v_sync_line_aft_pxl_5 = {0xff, 0xff},
		.v_sync_line_aft_pxl_6 = {0xff, 0xff},
		.vact_space_1 = {0xff, 0xff},
		.vact_space_2 = {0xff, 0xff},
		.vact_space_3 = {0xff, 0xff},
		.vact_space_4 = {0xff, 0xff},
		.vact_space_5 = {0xff, 0xff},
		.vact_space_6 = {0xff, 0xff},
		/* other don't care */
	},
	.tg = {
		0x00, /* cmd */
		0x50, 0x0a, /* h_fsz */
		0xd0, 0x02, 0x80, 0x07, /* hact */
		0x65, 0x04, /* v_fsz */
		0x01, 0x00, 0x33, 0x02, /* vsync */
		0x2d, 0x00, 0x38, 0x04, /* vact */
		0x33, 0x02, /* field_chg */
		0x48, 0x02, /* vact_st2 */
		0x00, 0x00, /* vact_st3 */
		0x00, 0x00, /* vact_st4 */
		0x01, 0x00, 0x01, 0x00, /* vsync top/bot */
		0x01, 0x00, 0x33, 0x02, /* field top/bot */
		0x00, /* 3d FP */
	},
};

static const struct hdmi_preset_conf hdmi_conf_1080p60 = {
	.core = {
		.h_blank = {0x18, 0x01},
		.v2_blank = {0x65, 0x04},
		.v1_blank = {0x2d, 0x00},
		.v_line = {0x65, 0x04},
		.h_line = {0x98, 0x08},
		.hsync_pol = {0x00},
		.vsync_pol = {0x00},
		.int_pro_mode = {0x00},
		.v_blank_f0 = {0xff, 0xff},
		.v_blank_f1 = {0xff, 0xff},
		.h_sync_start = {0x56, 0x00},
		.h_sync_end = {0x82, 0x00},
		.v_sync_line_bef_2 = {0x09, 0x00},
		.v_sync_line_bef_1 = {0x04, 0x00},
		.v_sync_line_aft_2 = {0xff, 0xff},
		.v_sync_line_aft_1 = {0xff, 0xff},
		.v_sync_line_aft_pxl_2 = {0xff, 0xff},
		.v_sync_line_aft_pxl_1 = {0xff, 0xff},
		.v_blank_f2 = {0xff, 0xff},
		.v_blank_f3 = {0xff, 0xff},
		.v_blank_f4 = {0xff, 0xff},
		.v_blank_f5 = {0xff, 0xff},
		.v_sync_line_aft_3 = {0xff, 0xff},
		.v_sync_line_aft_4 = {0xff, 0xff},
		.v_sync_line_aft_5 = {0xff, 0xff},
		.v_sync_line_aft_6 = {0xff, 0xff},
		.v_sync_line_aft_pxl_3 = {0xff, 0xff},
		.v_sync_line_aft_pxl_4 = {0xff, 0xff},
		.v_sync_line_aft_pxl_5 = {0xff, 0xff},
		.v_sync_line_aft_pxl_6 = {0xff, 0xff},
		/* other don't care */
	},
	.tg = {
		0x00, /* cmd */
		0x98, 0x08, /* h_fsz */
		0x18, 0x01, 0x80, 0x07, /* hact */
		0x65, 0x04, /* v_fsz */
		0x01, 0x00, 0x33, 0x02, /* vsync */
		0x2d, 0x00, 0x38, 0x04, /* vact */
		0x33, 0x02, /* field_chg */
		0x48, 0x02, /* vact_st2 */
		0x00, 0x00, /* vact_st3 */
		0x00, 0x00, /* vact_st4 */
		0x01, 0x00, 0x01, 0x00, /* vsync top/bot */
		0x01, 0x00, 0x33, 0x02, /* field top/bot */
		0x00, /* 3d FP */
	},
};

static const struct hdmi_conf hdmi_confs[] = {
	{ 720, 480, 60, false, hdmiphy_conf27_027, &hdmi_conf_480p60 },
	{ 1280, 720, 50, false, hdmiphy_conf74_25, &hdmi_conf_720p50 },
	{ 1280, 720, 60, false, hdmiphy_conf74_25, &hdmi_conf_720p60 },
	{ 1920, 1080, 50, true, hdmiphy_conf74_25, &hdmi_conf_1080i50 },
	{ 1920, 1080, 60, true, hdmiphy_conf74_25, &hdmi_conf_1080i60 },
	{ 1920, 1080, 50, false, hdmiphy_conf148_5, &hdmi_conf_1080p50 },
	{ 1920, 1080, 60, false, hdmiphy_conf148_5, &hdmi_conf_1080p60 },
};


static inline u32 hdmi_reg_read(struct hdmi_context *hdata, u32 reg_id)
{
	return readl(hdata->regs + reg_id);
=======
 */

#include <drm/exynos_drm.h>
#include <linux/clk.h>
#include <linux/component.h>
#include <linux/delay.h>
#include <linux/gpio/consumer.h>
#include <linux/hdmi.h>
#include <linux/i2c.h>
#include <linux/interrupt.h>
#include <linux/io.h>
#include <linux/irq.h>
#include <linux/kernel.h>
#include <linux/mfd/syscon.h>
#include <linux/of.h>
#include <linux/of_address.h>
#include <linux/of_graph.h>
#include <linux/platform_device.h>
#include <linux/pm_runtime.h>
#include <linux/regmap.h>
#include <linux/regulator/consumer.h>
#include <linux/wait.h>

#include <sound/hdmi-codec.h>
#include <media/cec-notifier.h>

#include <drm/drm_atomic_helper.h>
#include <drm/drm_bridge.h>
#include <drm/drm_edid.h>
#include <drm/drm_print.h>
#include <drm/drm_probe_helper.h>
#include <drm/drm_simple_kms_helper.h>

#include "exynos_drm_crtc.h"
#include "regs-hdmi.h"

#define HOTPLUG_DEBOUNCE_MS		1100

enum hdmi_type {
	HDMI_TYPE13,
	HDMI_TYPE14,
	HDMI_TYPE_COUNT
};

#define HDMI_MAPPED_BASE 0xffff0000

enum hdmi_mapped_regs {
	HDMI_PHY_STATUS = HDMI_MAPPED_BASE,
	HDMI_PHY_RSTOUT,
	HDMI_ACR_CON,
	HDMI_ACR_MCTS0,
	HDMI_ACR_CTS0,
	HDMI_ACR_N0
};

static const u32 hdmi_reg_map[][HDMI_TYPE_COUNT] = {
	{ HDMI_V13_PHY_STATUS, HDMI_PHY_STATUS_0 },
	{ HDMI_V13_PHY_RSTOUT, HDMI_V14_PHY_RSTOUT },
	{ HDMI_V13_ACR_CON, HDMI_V14_ACR_CON },
	{ HDMI_V13_ACR_MCTS0, HDMI_V14_ACR_MCTS0 },
	{ HDMI_V13_ACR_CTS0, HDMI_V14_ACR_CTS0 },
	{ HDMI_V13_ACR_N0, HDMI_V14_ACR_N0 },
};

static const char * const supply[] = {
	"vdd",
	"vdd_osc",
	"vdd_pll",
};

struct hdmiphy_config {
	int pixel_clock;
	u8 conf[32];
};

struct hdmiphy_configs {
	int count;
	const struct hdmiphy_config *data;
};

struct string_array_spec {
	int count;
	const char * const *data;
};

#define INIT_ARRAY_SPEC(a) { .count = ARRAY_SIZE(a), .data = a }

struct hdmi_driver_data {
	unsigned int type;
	unsigned int is_apb_phy:1;
	unsigned int has_sysreg:1;
	struct hdmiphy_configs phy_confs;
	struct string_array_spec clk_gates;
	/*
	 * Array of triplets (p_off, p_on, clock), where p_off and p_on are
	 * required parents of clock when HDMI-PHY is respectively off or on.
	 */
	struct string_array_spec clk_muxes;
};

struct hdmi_audio {
	struct platform_device		*pdev;
	struct hdmi_audio_infoframe	infoframe;
	struct hdmi_codec_params	params;
	bool				mute;
};

struct hdmi_context {
	struct drm_encoder		encoder;
	struct device			*dev;
	struct drm_device		*drm_dev;
	struct drm_connector		connector;
	bool				dvi_mode;
	struct delayed_work		hotplug_work;
	struct cec_notifier		*notifier;
	const struct hdmi_driver_data	*drv_data;

	void __iomem			*regs;
	void __iomem			*regs_hdmiphy;
	struct i2c_client		*hdmiphy_port;
	struct i2c_adapter		*ddc_adpt;
	struct gpio_desc		*hpd_gpio;
	int				irq;
	struct regmap			*pmureg;
	struct regmap			*sysreg;
	struct clk			**clk_gates;
	struct clk			**clk_muxes;
	struct regulator_bulk_data	regul_bulk[ARRAY_SIZE(supply)];
	struct regulator		*reg_hdmi_en;
	struct exynos_drm_clk		phy_clk;
	struct drm_bridge		*bridge;

	/* mutex protecting subsequent fields below */
	struct mutex			mutex;
	struct hdmi_audio		audio;
	bool				powered;
};

static inline struct hdmi_context *encoder_to_hdmi(struct drm_encoder *e)
{
	return container_of(e, struct hdmi_context, encoder);
}

static inline struct hdmi_context *connector_to_hdmi(struct drm_connector *c)
{
	return container_of(c, struct hdmi_context, connector);
}

static const struct hdmiphy_config hdmiphy_v13_configs[] = {
	{
		.pixel_clock = 27000000,
		.conf = {
			0x01, 0x05, 0x00, 0xD8, 0x10, 0x1C, 0x30, 0x40,
			0x6B, 0x10, 0x02, 0x51, 0xDF, 0xF2, 0x54, 0x87,
			0x84, 0x00, 0x30, 0x38, 0x00, 0x08, 0x10, 0xE0,
			0x22, 0x40, 0xE3, 0x26, 0x00, 0x00, 0x00, 0x80,
		},
	},
	{
		.pixel_clock = 27027000,
		.conf = {
			0x01, 0x05, 0x00, 0xD4, 0x10, 0x9C, 0x09, 0x64,
			0x6B, 0x10, 0x02, 0x51, 0xDF, 0xF2, 0x54, 0x87,
			0x84, 0x00, 0x30, 0x38, 0x00, 0x08, 0x10, 0xE0,
			0x22, 0x40, 0xE3, 0x26, 0x00, 0x00, 0x00, 0x80,
		},
	},
	{
		.pixel_clock = 74176000,
		.conf = {
			0x01, 0x05, 0x00, 0xD8, 0x10, 0x9C, 0xef, 0x5B,
			0x6D, 0x10, 0x01, 0x51, 0xef, 0xF3, 0x54, 0xb9,
			0x84, 0x00, 0x30, 0x38, 0x00, 0x08, 0x10, 0xE0,
			0x22, 0x40, 0xa5, 0x26, 0x01, 0x00, 0x00, 0x80,
		},
	},
	{
		.pixel_clock = 74250000,
		.conf = {
			0x01, 0x05, 0x00, 0xd8, 0x10, 0x9c, 0xf8, 0x40,
			0x6a, 0x10, 0x01, 0x51, 0xff, 0xf1, 0x54, 0xba,
			0x84, 0x00, 0x10, 0x38, 0x00, 0x08, 0x10, 0xe0,
			0x22, 0x40, 0xa4, 0x26, 0x01, 0x00, 0x00, 0x80,
		},
	},
	{
		.pixel_clock = 148500000,
		.conf = {
			0x01, 0x05, 0x00, 0xD8, 0x10, 0x9C, 0xf8, 0x40,
			0x6A, 0x18, 0x00, 0x51, 0xff, 0xF1, 0x54, 0xba,
			0x84, 0x00, 0x10, 0x38, 0x00, 0x08, 0x10, 0xE0,
			0x22, 0x40, 0xa4, 0x26, 0x02, 0x00, 0x00, 0x80,
		},
	},
};

static const struct hdmiphy_config hdmiphy_v14_configs[] = {
	{
		.pixel_clock = 25200000,
		.conf = {
			0x01, 0x51, 0x2A, 0x75, 0x40, 0x01, 0x00, 0x08,
			0x82, 0x80, 0xfc, 0xd8, 0x45, 0xa0, 0xac, 0x80,
			0x08, 0x80, 0x11, 0x04, 0x02, 0x22, 0x44, 0x86,
			0x54, 0xf4, 0x24, 0x00, 0x00, 0x00, 0x01, 0x80,
		},
	},
	{
		.pixel_clock = 27000000,
		.conf = {
			0x01, 0xd1, 0x22, 0x51, 0x40, 0x08, 0xfc, 0x20,
			0x98, 0xa0, 0xcb, 0xd8, 0x45, 0xa0, 0xac, 0x80,
			0x06, 0x80, 0x11, 0x04, 0x02, 0x22, 0x44, 0x86,
			0x54, 0xe4, 0x24, 0x00, 0x00, 0x00, 0x01, 0x80,
		},
	},
	{
		.pixel_clock = 27027000,
		.conf = {
			0x01, 0xd1, 0x2d, 0x72, 0x40, 0x64, 0x12, 0x08,
			0x43, 0xa0, 0x0e, 0xd9, 0x45, 0xa0, 0xac, 0x80,
			0x08, 0x80, 0x11, 0x04, 0x02, 0x22, 0x44, 0x86,
			0x54, 0xe3, 0x24, 0x00, 0x00, 0x00, 0x01, 0x80,
		},
	},
	{
		.pixel_clock = 36000000,
		.conf = {
			0x01, 0x51, 0x2d, 0x55, 0x40, 0x01, 0x00, 0x08,
			0x82, 0x80, 0x0e, 0xd9, 0x45, 0xa0, 0xac, 0x80,
			0x08, 0x80, 0x11, 0x04, 0x02, 0x22, 0x44, 0x86,
			0x54, 0xab, 0x24, 0x00, 0x00, 0x00, 0x01, 0x80,
		},
	},
	{
		.pixel_clock = 40000000,
		.conf = {
			0x01, 0x51, 0x32, 0x55, 0x40, 0x01, 0x00, 0x08,
			0x82, 0x80, 0x2c, 0xd9, 0x45, 0xa0, 0xac, 0x80,
			0x08, 0x80, 0x11, 0x04, 0x02, 0x22, 0x44, 0x86,
			0x54, 0x9a, 0x24, 0x00, 0x00, 0x00, 0x01, 0x80,
		},
	},
	{
		.pixel_clock = 65000000,
		.conf = {
			0x01, 0xd1, 0x36, 0x34, 0x40, 0x1e, 0x0a, 0x08,
			0x82, 0xa0, 0x45, 0xd9, 0x45, 0xa0, 0xac, 0x80,
			0x08, 0x80, 0x11, 0x04, 0x02, 0x22, 0x44, 0x86,
			0x54, 0xbd, 0x24, 0x01, 0x00, 0x00, 0x01, 0x80,
		},
	},
	{
		.pixel_clock = 71000000,
		.conf = {
			0x01, 0xd1, 0x3b, 0x35, 0x40, 0x0c, 0x04, 0x08,
			0x85, 0xa0, 0x63, 0xd9, 0x45, 0xa0, 0xac, 0x80,
			0x08, 0x80, 0x11, 0x04, 0x02, 0x22, 0x44, 0x86,
			0x54, 0xad, 0x24, 0x01, 0x00, 0x00, 0x01, 0x80,
		},
	},
	{
		.pixel_clock = 73250000,
		.conf = {
			0x01, 0xd1, 0x3d, 0x35, 0x40, 0x18, 0x02, 0x08,
			0x83, 0xa0, 0x6e, 0xd9, 0x45, 0xa0, 0xac, 0x80,
			0x08, 0x80, 0x11, 0x04, 0x02, 0x22, 0x44, 0x86,
			0x54, 0xa8, 0x24, 0x01, 0x00, 0x00, 0x01, 0x80,
		},
	},
	{
		.pixel_clock = 74176000,
		.conf = {
			0x01, 0xd1, 0x3e, 0x35, 0x40, 0x5b, 0xde, 0x08,
			0x82, 0xa0, 0x73, 0xd9, 0x45, 0xa0, 0xac, 0x80,
			0x56, 0x80, 0x11, 0x04, 0x02, 0x22, 0x44, 0x86,
			0x54, 0xa6, 0x24, 0x01, 0x00, 0x00, 0x01, 0x80,
		},
	},
	{
		.pixel_clock = 74250000,
		.conf = {
			0x01, 0xd1, 0x1f, 0x10, 0x40, 0x40, 0xf8, 0x08,
			0x81, 0xa0, 0xba, 0xd8, 0x45, 0xa0, 0xac, 0x80,
			0x3c, 0x80, 0x11, 0x04, 0x02, 0x22, 0x44, 0x86,
			0x54, 0xa5, 0x24, 0x01, 0x00, 0x00, 0x01, 0x80,
		},
	},
	{
		.pixel_clock = 83500000,
		.conf = {
			0x01, 0xd1, 0x23, 0x11, 0x40, 0x0c, 0xfb, 0x08,
			0x85, 0xa0, 0xd1, 0xd8, 0x45, 0xa0, 0xac, 0x80,
			0x08, 0x80, 0x11, 0x04, 0x02, 0x22, 0x44, 0x86,
			0x54, 0x93, 0x24, 0x01, 0x00, 0x00, 0x01, 0x80,
		},
	},
	{
		.pixel_clock = 85500000,
		.conf = {
			0x01, 0xd1, 0x24, 0x11, 0x40, 0x40, 0xd0, 0x08,
			0x84, 0xa0, 0xd6, 0xd8, 0x45, 0xa0, 0xac, 0x80,
			0x08, 0x80, 0x11, 0x04, 0x02, 0x22, 0x44, 0x86,
			0x54, 0x90, 0x24, 0x01, 0x00, 0x00, 0x01, 0x80,
		},
	},
	{
		.pixel_clock = 106500000,
		.conf = {
			0x01, 0xd1, 0x2c, 0x12, 0x40, 0x0c, 0x09, 0x08,
			0x84, 0xa0, 0x0a, 0xd9, 0x45, 0xa0, 0xac, 0x80,
			0x08, 0x80, 0x11, 0x04, 0x02, 0x22, 0x44, 0x86,
			0x54, 0x73, 0x24, 0x01, 0x00, 0x00, 0x01, 0x80,
		},
	},
	{
		.pixel_clock = 108000000,
		.conf = {
			0x01, 0x51, 0x2d, 0x15, 0x40, 0x01, 0x00, 0x08,
			0x82, 0x80, 0x0e, 0xd9, 0x45, 0xa0, 0xac, 0x80,
			0x08, 0x80, 0x11, 0x04, 0x02, 0x22, 0x44, 0x86,
			0x54, 0xc7, 0x25, 0x03, 0x00, 0x00, 0x01, 0x80,
		},
	},
	{
		.pixel_clock = 115500000,
		.conf = {
			0x01, 0xd1, 0x30, 0x12, 0x40, 0x40, 0x10, 0x08,
			0x80, 0x80, 0x21, 0xd9, 0x45, 0xa0, 0xac, 0x80,
			0x08, 0x80, 0x11, 0x04, 0x02, 0x22, 0x44, 0x86,
			0x54, 0xaa, 0x25, 0x03, 0x00, 0x00, 0x01, 0x80,
		},
	},
	{
		.pixel_clock = 119000000,
		.conf = {
			0x01, 0xd1, 0x32, 0x1a, 0x40, 0x30, 0xd8, 0x08,
			0x04, 0xa0, 0x2a, 0xd9, 0x45, 0xa0, 0xac, 0x80,
			0x08, 0x80, 0x11, 0x04, 0x02, 0x22, 0x44, 0x86,
			0x54, 0x9d, 0x25, 0x03, 0x00, 0x00, 0x01, 0x80,
		},
	},
	{
		.pixel_clock = 146250000,
		.conf = {
			0x01, 0xd1, 0x3d, 0x15, 0x40, 0x18, 0xfd, 0x08,
			0x83, 0xa0, 0x6e, 0xd9, 0x45, 0xa0, 0xac, 0x80,
			0x08, 0x80, 0x11, 0x04, 0x02, 0x22, 0x44, 0x86,
			0x54, 0x50, 0x25, 0x03, 0x00, 0x00, 0x01, 0x80,
		},
	},
	{
		.pixel_clock = 148500000,
		.conf = {
			0x01, 0xd1, 0x1f, 0x00, 0x40, 0x40, 0xf8, 0x08,
			0x81, 0xa0, 0xba, 0xd8, 0x45, 0xa0, 0xac, 0x80,
			0x3c, 0x80, 0x11, 0x04, 0x02, 0x22, 0x44, 0x86,
			0x54, 0x4b, 0x25, 0x03, 0x00, 0x00, 0x01, 0x80,
		},
	},
};

static const struct hdmiphy_config hdmiphy_5420_configs[] = {
	{
		.pixel_clock = 25200000,
		.conf = {
			0x01, 0x52, 0x3F, 0x55, 0x40, 0x01, 0x00, 0xC8,
			0x82, 0xC8, 0xBD, 0xD8, 0x45, 0xA0, 0xAC, 0x80,
			0x06, 0x80, 0x01, 0x84, 0x05, 0x02, 0x24, 0x66,
			0x54, 0xF4, 0x24, 0x00, 0x00, 0x00, 0x01, 0x80,
		},
	},
	{
		.pixel_clock = 27000000,
		.conf = {
			0x01, 0xD1, 0x22, 0x51, 0x40, 0x08, 0xFC, 0xE0,
			0x98, 0xE8, 0xCB, 0xD8, 0x45, 0xA0, 0xAC, 0x80,
			0x06, 0x80, 0x09, 0x84, 0x05, 0x02, 0x24, 0x66,
			0x54, 0xE4, 0x24, 0x00, 0x00, 0x00, 0x01, 0x80,
		},
	},
	{
		.pixel_clock = 27027000,
		.conf = {
			0x01, 0xD1, 0x2D, 0x72, 0x40, 0x64, 0x12, 0xC8,
			0x43, 0xE8, 0x0E, 0xD9, 0x45, 0xA0, 0xAC, 0x80,
			0x26, 0x80, 0x09, 0x84, 0x05, 0x02, 0x24, 0x66,
			0x54, 0xE3, 0x24, 0x00, 0x00, 0x00, 0x01, 0x80,
		},
	},
	{
		.pixel_clock = 36000000,
		.conf = {
			0x01, 0x51, 0x2D, 0x55, 0x40, 0x40, 0x00, 0xC8,
			0x02, 0xC8, 0x0E, 0xD9, 0x45, 0xA0, 0xAC, 0x80,
			0x08, 0x80, 0x09, 0x84, 0x05, 0x02, 0x24, 0x66,
			0x54, 0xAB, 0x24, 0x00, 0x00, 0x00, 0x01, 0x80,
		},
	},
	{
		.pixel_clock = 40000000,
		.conf = {
			0x01, 0xD1, 0x21, 0x31, 0x40, 0x3C, 0x28, 0xC8,
			0x87, 0xE8, 0xC8, 0xD8, 0x45, 0xA0, 0xAC, 0x80,
			0x08, 0x80, 0x09, 0x84, 0x05, 0x02, 0x24, 0x66,
			0x54, 0x9A, 0x24, 0x00, 0x00, 0x00, 0x01, 0x80,
		},
	},
	{
		.pixel_clock = 65000000,
		.conf = {
			0x01, 0xD1, 0x36, 0x34, 0x40, 0x0C, 0x04, 0xC8,
			0x82, 0xE8, 0x45, 0xD9, 0x45, 0xA0, 0xAC, 0x80,
			0x08, 0x80, 0x09, 0x84, 0x05, 0x02, 0x24, 0x66,
			0x54, 0xBD, 0x24, 0x01, 0x00, 0x00, 0x01, 0x80,
		},
	},
	{
		.pixel_clock = 71000000,
		.conf = {
			0x01, 0xD1, 0x3B, 0x35, 0x40, 0x0C, 0x04, 0xC8,
			0x85, 0xE8, 0x63, 0xD9, 0x45, 0xA0, 0xAC, 0x80,
			0x08, 0x80, 0x09, 0x84, 0x05, 0x02, 0x24, 0x66,
			0x54, 0x57, 0x24, 0x00, 0x00, 0x00, 0x01, 0x80,
		},
	},
	{
		.pixel_clock = 73250000,
		.conf = {
			0x01, 0xD1, 0x1F, 0x10, 0x40, 0x78, 0x8D, 0xC8,
			0x81, 0xE8, 0xB7, 0xD8, 0x45, 0xA0, 0xAC, 0x80,
			0x56, 0x80, 0x09, 0x84, 0x05, 0x02, 0x24, 0x66,
			0x54, 0xA8, 0x24, 0x01, 0x00, 0x00, 0x01, 0x80,
		},
	},
	{
		.pixel_clock = 74176000,
		.conf = {
			0x01, 0xD1, 0x1F, 0x10, 0x40, 0x5B, 0xEF, 0xC8,
			0x81, 0xE8, 0xB9, 0xD8, 0x45, 0xA0, 0xAC, 0x80,
			0x56, 0x80, 0x09, 0x84, 0x05, 0x02, 0x24, 0x66,
			0x54, 0xA6, 0x24, 0x01, 0x00, 0x00, 0x01, 0x80,
		},
	},
	{
		.pixel_clock = 74250000,
		.conf = {
			0x01, 0xD1, 0x1F, 0x10, 0x40, 0x40, 0xF8, 0x08,
			0x81, 0xE8, 0xBA, 0xD8, 0x45, 0xA0, 0xAC, 0x80,
			0x26, 0x80, 0x09, 0x84, 0x05, 0x22, 0x24, 0x66,
			0x54, 0xA5, 0x24, 0x01, 0x00, 0x00, 0x01, 0x80,
		},
	},
	{
		.pixel_clock = 83500000,
		.conf = {
			0x01, 0xD1, 0x23, 0x11, 0x40, 0x0C, 0xFB, 0xC8,
			0x85, 0xE8, 0xD1, 0xD8, 0x45, 0xA0, 0xAC, 0x80,
			0x08, 0x80, 0x09, 0x84, 0x05, 0x02, 0x24, 0x66,
			0x54, 0x4A, 0x24, 0x00, 0x00, 0x00, 0x01, 0x80,
		},
	},
	{
		.pixel_clock = 88750000,
		.conf = {
			0x01, 0xD1, 0x25, 0x11, 0x40, 0x18, 0xFF, 0xC8,
			0x83, 0xE8, 0xDE, 0xD8, 0x45, 0xA0, 0xAC, 0x80,
			0x08, 0x80, 0x09, 0x84, 0x05, 0x02, 0x24, 0x66,
			0x54, 0x45, 0x24, 0x00, 0x00, 0x00, 0x01, 0x80,
		},
	},
	{
		.pixel_clock = 106500000,
		.conf = {
			0x01, 0xD1, 0x2C, 0x12, 0x40, 0x0C, 0x09, 0xC8,
			0x84, 0xE8, 0x0A, 0xD9, 0x45, 0xA0, 0xAC, 0x80,
			0x08, 0x80, 0x09, 0x84, 0x05, 0x02, 0x24, 0x66,
			0x54, 0x73, 0x24, 0x01, 0x00, 0x00, 0x01, 0x80,
		},
	},
	{
		.pixel_clock = 108000000,
		.conf = {
			0x01, 0x51, 0x2D, 0x15, 0x40, 0x01, 0x00, 0xC8,
			0x82, 0xC8, 0x0E, 0xD9, 0x45, 0xA0, 0xAC, 0x80,
			0x08, 0x80, 0x09, 0x84, 0x05, 0x02, 0x24, 0x66,
			0x54, 0xC7, 0x25, 0x03, 0x00, 0x00, 0x01, 0x80,
		},
	},
	{
		.pixel_clock = 115500000,
		.conf = {
			0x01, 0xD1, 0x30, 0x14, 0x40, 0x0C, 0x03, 0xC8,
			0x88, 0xE8, 0x21, 0xD9, 0x45, 0xA0, 0xAC, 0x80,
			0x08, 0x80, 0x09, 0x84, 0x05, 0x02, 0x24, 0x66,
			0x54, 0x6A, 0x24, 0x01, 0x00, 0x00, 0x01, 0x80,
		},
	},
	{
		.pixel_clock = 146250000,
		.conf = {
			0x01, 0xD1, 0x3D, 0x15, 0x40, 0x18, 0xFD, 0xC8,
			0x83, 0xE8, 0x6E, 0xD9, 0x45, 0xA0, 0xAC, 0x80,
			0x08, 0x80, 0x09, 0x84, 0x05, 0x02, 0x24, 0x66,
			0x54, 0x54, 0x24, 0x01, 0x00, 0x00, 0x01, 0x80,
		},
	},
	{
		.pixel_clock = 148500000,
		.conf = {
			0x01, 0xD1, 0x1F, 0x00, 0x40, 0x40, 0xF8, 0x08,
			0x81, 0xE8, 0xBA, 0xD8, 0x45, 0xA0, 0xAC, 0x80,
			0x26, 0x80, 0x09, 0x84, 0x05, 0x22, 0x24, 0x66,
			0x54, 0x4B, 0x25, 0x03, 0x00, 0x80, 0x01, 0x80,
		},
	},
	{
		.pixel_clock = 154000000,
		.conf = {
			0x01, 0xD1, 0x20, 0x01, 0x40, 0x30, 0x08, 0xCC,
			0x8C, 0xE8, 0xC1, 0xD8, 0x45, 0xA0, 0xAC, 0x80,
			0x08, 0x80, 0x09, 0x84, 0x05, 0x02, 0x24, 0x86,
			0x54, 0x3F, 0x25, 0x03, 0x00, 0x00, 0x01, 0x80,
		},
	},
};

static const struct hdmiphy_config hdmiphy_5433_configs[] = {
	{
		.pixel_clock = 27000000,
		.conf = {
			0x01, 0x51, 0x2d, 0x75, 0x01, 0x00, 0x88, 0x02,
			0x72, 0x50, 0x44, 0x8c, 0x27, 0x00, 0x7c, 0xac,
			0xd6, 0x2b, 0x67, 0x00, 0x00, 0x04, 0x00, 0x30,
			0x08, 0x10, 0x01, 0x01, 0x48, 0x40, 0x00, 0x40,
		},
	},
	{
		.pixel_clock = 27027000,
		.conf = {
			0x01, 0x51, 0x2d, 0x72, 0x64, 0x09, 0x88, 0xc3,
			0x71, 0x50, 0x44, 0x8c, 0x27, 0x00, 0x7c, 0xac,
			0xd6, 0x2b, 0x67, 0x00, 0x00, 0x04, 0x00, 0x30,
			0x08, 0x10, 0x01, 0x01, 0x48, 0x40, 0x00, 0x40,
		},
	},
	{
		.pixel_clock = 40000000,
		.conf = {
			0x01, 0x51, 0x32, 0x55, 0x01, 0x00, 0x88, 0x02,
			0x4d, 0x50, 0x44, 0x8C, 0x27, 0x00, 0x7C, 0xAC,
			0xD6, 0x2B, 0x67, 0x00, 0x00, 0x04, 0x00, 0x30,
			0x08, 0x10, 0x01, 0x01, 0x48, 0x40, 0x00, 0x40,
		},
	},
	{
		.pixel_clock = 50000000,
		.conf = {
			0x01, 0x51, 0x34, 0x40, 0x64, 0x09, 0x88, 0xc3,
			0x3d, 0x50, 0x44, 0x8C, 0x27, 0x00, 0x7C, 0xAC,
			0xD6, 0x2B, 0x67, 0x00, 0x00, 0x04, 0x00, 0x30,
			0x08, 0x10, 0x01, 0x01, 0x48, 0x40, 0x00, 0x40,
		},
	},
	{
		.pixel_clock = 65000000,
		.conf = {
			0x01, 0x51, 0x36, 0x31, 0x40, 0x10, 0x04, 0xc6,
			0x2e, 0xe8, 0x44, 0x8C, 0x27, 0x00, 0x7C, 0xAC,
			0xD6, 0x2B, 0x67, 0x00, 0x00, 0x04, 0x00, 0x30,
			0x08, 0x10, 0x01, 0x01, 0x48, 0x40, 0x00, 0x40,
		},
	},
	{
		.pixel_clock = 74176000,
		.conf = {
			0x01, 0x51, 0x3E, 0x35, 0x5B, 0xDE, 0x88, 0x42,
			0x53, 0x51, 0x44, 0x8C, 0x27, 0x00, 0x7C, 0xAC,
			0xD6, 0x2B, 0x67, 0x00, 0x00, 0x04, 0x00, 0x30,
			0x08, 0x10, 0x01, 0x01, 0x48, 0x40, 0x00, 0x40,
		},
	},
	{
		.pixel_clock = 74250000,
		.conf = {
			0x01, 0x51, 0x3E, 0x35, 0x40, 0xF0, 0x88, 0xC2,
			0x52, 0x51, 0x44, 0x8C, 0x27, 0x00, 0x7C, 0xAC,
			0xD6, 0x2B, 0x67, 0x00, 0x00, 0x04, 0x00, 0x30,
			0x08, 0x10, 0x01, 0x01, 0x48, 0x40, 0x00, 0x40,
		},
	},
	{
		.pixel_clock = 108000000,
		.conf = {
			0x01, 0x51, 0x2d, 0x15, 0x01, 0x00, 0x88, 0x02,
			0x72, 0x52, 0x44, 0x8C, 0x27, 0x00, 0x7C, 0xAC,
			0xD6, 0x2B, 0x67, 0x00, 0x00, 0x04, 0x00, 0x30,
			0x08, 0x10, 0x01, 0x01, 0x48, 0x40, 0x00, 0x40,
		},
	},
	{
		.pixel_clock = 148500000,
		.conf = {
			0x01, 0x51, 0x1f, 0x00, 0x40, 0xf8, 0x88, 0xc1,
			0x52, 0x52, 0x24, 0x0c, 0x24, 0x0f, 0x7c, 0xa5,
			0xd4, 0x2b, 0x87, 0x00, 0x00, 0x04, 0x00, 0x30,
			0x08, 0x10, 0x01, 0x01, 0x48, 0x4a, 0x00, 0x40,
		},
	},
	{
		.pixel_clock = 297000000,
		.conf = {
			0x01, 0x51, 0x3E, 0x05, 0x40, 0xF0, 0x88, 0xC2,
			0x52, 0x53, 0x44, 0x8C, 0x27, 0x00, 0x7C, 0xAC,
			0xD6, 0x2B, 0x67, 0x00, 0x00, 0x04, 0x00, 0x30,
			0x08, 0x10, 0x01, 0x01, 0x48, 0x40, 0x00, 0x40,
		},
	},
};

static const char * const hdmi_clk_gates4[] = {
	"hdmi", "sclk_hdmi"
};

static const char * const hdmi_clk_muxes4[] = {
	"sclk_pixel", "sclk_hdmiphy", "mout_hdmi"
};

static const char * const hdmi_clk_gates5433[] = {
	"hdmi_pclk", "hdmi_i_pclk", "i_tmds_clk", "i_pixel_clk", "i_spdif_clk"
};

static const char * const hdmi_clk_muxes5433[] = {
	"oscclk", "tmds_clko", "tmds_clko_user",
	"oscclk", "pixel_clko", "pixel_clko_user"
};

static const struct hdmi_driver_data exynos4210_hdmi_driver_data = {
	.type		= HDMI_TYPE13,
	.phy_confs	= INIT_ARRAY_SPEC(hdmiphy_v13_configs),
	.clk_gates	= INIT_ARRAY_SPEC(hdmi_clk_gates4),
	.clk_muxes	= INIT_ARRAY_SPEC(hdmi_clk_muxes4),
};

static const struct hdmi_driver_data exynos4212_hdmi_driver_data = {
	.type		= HDMI_TYPE14,
	.phy_confs	= INIT_ARRAY_SPEC(hdmiphy_v14_configs),
	.clk_gates	= INIT_ARRAY_SPEC(hdmi_clk_gates4),
	.clk_muxes	= INIT_ARRAY_SPEC(hdmi_clk_muxes4),
};

static const struct hdmi_driver_data exynos5420_hdmi_driver_data = {
	.type		= HDMI_TYPE14,
	.is_apb_phy	= 1,
	.phy_confs	= INIT_ARRAY_SPEC(hdmiphy_5420_configs),
	.clk_gates	= INIT_ARRAY_SPEC(hdmi_clk_gates4),
	.clk_muxes	= INIT_ARRAY_SPEC(hdmi_clk_muxes4),
};

static const struct hdmi_driver_data exynos5433_hdmi_driver_data = {
	.type		= HDMI_TYPE14,
	.is_apb_phy	= 1,
	.has_sysreg     = 1,
	.phy_confs	= INIT_ARRAY_SPEC(hdmiphy_5433_configs),
	.clk_gates	= INIT_ARRAY_SPEC(hdmi_clk_gates5433),
	.clk_muxes	= INIT_ARRAY_SPEC(hdmi_clk_muxes5433),
};

static inline u32 hdmi_map_reg(struct hdmi_context *hdata, u32 reg_id)
{
	if ((reg_id & 0xffff0000) == HDMI_MAPPED_BASE)
		return hdmi_reg_map[reg_id & 0xffff][hdata->drv_data->type];
	return reg_id;
}

static inline u32 hdmi_reg_read(struct hdmi_context *hdata, u32 reg_id)
{
	return readl(hdata->regs + hdmi_map_reg(hdata, reg_id));
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

static inline void hdmi_reg_writeb(struct hdmi_context *hdata,
				 u32 reg_id, u8 value)
{
<<<<<<< HEAD
	writeb(value, hdata->regs + reg_id);
=======
	writel(value, hdata->regs + hdmi_map_reg(hdata, reg_id));
}

static inline void hdmi_reg_writev(struct hdmi_context *hdata, u32 reg_id,
				   int bytes, u32 val)
{
	reg_id = hdmi_map_reg(hdata, reg_id);

	while (--bytes >= 0) {
		writel(val & 0xff, hdata->regs + reg_id);
		val >>= 8;
		reg_id += 4;
	}
}

static inline void hdmi_reg_write_buf(struct hdmi_context *hdata, u32 reg_id,
				      u8 *buf, int size)
{
	for (reg_id = hdmi_map_reg(hdata, reg_id); size; --size, reg_id += 4)
		writel(*buf++, hdata->regs + reg_id);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

static inline void hdmi_reg_writemask(struct hdmi_context *hdata,
				 u32 reg_id, u32 value, u32 mask)
{
<<<<<<< HEAD
	u32 old = readl(hdata->regs + reg_id);
=======
	u32 old;

	reg_id = hdmi_map_reg(hdata, reg_id);
	old = readl(hdata->regs + reg_id);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	value = (value & mask) | (old & ~mask);
	writel(value, hdata->regs + reg_id);
}

<<<<<<< HEAD
static void hdmi_v13_regs_dump(struct hdmi_context *hdata, char *prefix)
{
#define DUMPREG(reg_id) \
	DRM_DEBUG_KMS("%s:" #reg_id " = %08x\n", prefix, \
	readl(hdata->regs + reg_id))
	DRM_DEBUG_KMS("%s: ---- CONTROL REGISTERS ----\n", prefix);
	DUMPREG(HDMI_INTC_FLAG);
	DUMPREG(HDMI_INTC_CON);
	DUMPREG(HDMI_HPD_STATUS);
	DUMPREG(HDMI_V13_PHY_RSTOUT);
	DUMPREG(HDMI_V13_PHY_VPLL);
	DUMPREG(HDMI_V13_PHY_CMU);
	DUMPREG(HDMI_V13_CORE_RSTOUT);

	DRM_DEBUG_KMS("%s: ---- CORE REGISTERS ----\n", prefix);
	DUMPREG(HDMI_CON_0);
	DUMPREG(HDMI_CON_1);
	DUMPREG(HDMI_CON_2);
	DUMPREG(HDMI_SYS_STATUS);
	DUMPREG(HDMI_V13_PHY_STATUS);
	DUMPREG(HDMI_STATUS_EN);
	DUMPREG(HDMI_HPD);
	DUMPREG(HDMI_MODE_SEL);
	DUMPREG(HDMI_V13_HPD_GEN);
	DUMPREG(HDMI_V13_DC_CONTROL);
	DUMPREG(HDMI_V13_VIDEO_PATTERN_GEN);

	DRM_DEBUG_KMS("%s: ---- CORE SYNC REGISTERS ----\n", prefix);
	DUMPREG(HDMI_H_BLANK_0);
	DUMPREG(HDMI_H_BLANK_1);
	DUMPREG(HDMI_V13_V_BLANK_0);
	DUMPREG(HDMI_V13_V_BLANK_1);
	DUMPREG(HDMI_V13_V_BLANK_2);
	DUMPREG(HDMI_V13_H_V_LINE_0);
	DUMPREG(HDMI_V13_H_V_LINE_1);
	DUMPREG(HDMI_V13_H_V_LINE_2);
	DUMPREG(HDMI_VSYNC_POL);
	DUMPREG(HDMI_INT_PRO_MODE);
	DUMPREG(HDMI_V13_V_BLANK_F_0);
	DUMPREG(HDMI_V13_V_BLANK_F_1);
	DUMPREG(HDMI_V13_V_BLANK_F_2);
	DUMPREG(HDMI_V13_H_SYNC_GEN_0);
	DUMPREG(HDMI_V13_H_SYNC_GEN_1);
	DUMPREG(HDMI_V13_H_SYNC_GEN_2);
	DUMPREG(HDMI_V13_V_SYNC_GEN_1_0);
	DUMPREG(HDMI_V13_V_SYNC_GEN_1_1);
	DUMPREG(HDMI_V13_V_SYNC_GEN_1_2);
	DUMPREG(HDMI_V13_V_SYNC_GEN_2_0);
	DUMPREG(HDMI_V13_V_SYNC_GEN_2_1);
	DUMPREG(HDMI_V13_V_SYNC_GEN_2_2);
	DUMPREG(HDMI_V13_V_SYNC_GEN_3_0);
	DUMPREG(HDMI_V13_V_SYNC_GEN_3_1);
	DUMPREG(HDMI_V13_V_SYNC_GEN_3_2);

	DRM_DEBUG_KMS("%s: ---- TG REGISTERS ----\n", prefix);
	DUMPREG(HDMI_TG_CMD);
	DUMPREG(HDMI_TG_H_FSZ_L);
	DUMPREG(HDMI_TG_H_FSZ_H);
	DUMPREG(HDMI_TG_HACT_ST_L);
	DUMPREG(HDMI_TG_HACT_ST_H);
	DUMPREG(HDMI_TG_HACT_SZ_L);
	DUMPREG(HDMI_TG_HACT_SZ_H);
	DUMPREG(HDMI_TG_V_FSZ_L);
	DUMPREG(HDMI_TG_V_FSZ_H);
	DUMPREG(HDMI_TG_VSYNC_L);
	DUMPREG(HDMI_TG_VSYNC_H);
	DUMPREG(HDMI_TG_VSYNC2_L);
	DUMPREG(HDMI_TG_VSYNC2_H);
	DUMPREG(HDMI_TG_VACT_ST_L);
	DUMPREG(HDMI_TG_VACT_ST_H);
	DUMPREG(HDMI_TG_VACT_SZ_L);
	DUMPREG(HDMI_TG_VACT_SZ_H);
	DUMPREG(HDMI_TG_FIELD_CHG_L);
	DUMPREG(HDMI_TG_FIELD_CHG_H);
	DUMPREG(HDMI_TG_VACT_ST2_L);
	DUMPREG(HDMI_TG_VACT_ST2_H);
	DUMPREG(HDMI_TG_VSYNC_TOP_HDMI_L);
	DUMPREG(HDMI_TG_VSYNC_TOP_HDMI_H);
	DUMPREG(HDMI_TG_VSYNC_BOT_HDMI_L);
	DUMPREG(HDMI_TG_VSYNC_BOT_HDMI_H);
	DUMPREG(HDMI_TG_FIELD_TOP_HDMI_L);
	DUMPREG(HDMI_TG_FIELD_TOP_HDMI_H);
	DUMPREG(HDMI_TG_FIELD_BOT_HDMI_L);
	DUMPREG(HDMI_TG_FIELD_BOT_HDMI_H);
#undef DUMPREG
}

static void hdmi_v14_regs_dump(struct hdmi_context *hdata, char *prefix)
{
	int i;

#define DUMPREG(reg_id) \
	DRM_DEBUG_KMS("%s:" #reg_id " = %08x\n", prefix, \
	readl(hdata->regs + reg_id))

	DRM_DEBUG_KMS("%s: ---- CONTROL REGISTERS ----\n", prefix);
	DUMPREG(HDMI_INTC_CON);
	DUMPREG(HDMI_INTC_FLAG);
	DUMPREG(HDMI_HPD_STATUS);
	DUMPREG(HDMI_INTC_CON_1);
	DUMPREG(HDMI_INTC_FLAG_1);
	DUMPREG(HDMI_PHY_STATUS_0);
	DUMPREG(HDMI_PHY_STATUS_PLL);
	DUMPREG(HDMI_PHY_CON_0);
	DUMPREG(HDMI_PHY_RSTOUT);
	DUMPREG(HDMI_PHY_VPLL);
	DUMPREG(HDMI_PHY_CMU);
	DUMPREG(HDMI_CORE_RSTOUT);

	DRM_DEBUG_KMS("%s: ---- CORE REGISTERS ----\n", prefix);
	DUMPREG(HDMI_CON_0);
	DUMPREG(HDMI_CON_1);
	DUMPREG(HDMI_CON_2);
	DUMPREG(HDMI_SYS_STATUS);
	DUMPREG(HDMI_PHY_STATUS_0);
	DUMPREG(HDMI_STATUS_EN);
	DUMPREG(HDMI_HPD);
	DUMPREG(HDMI_MODE_SEL);
	DUMPREG(HDMI_ENC_EN);
	DUMPREG(HDMI_DC_CONTROL);
	DUMPREG(HDMI_VIDEO_PATTERN_GEN);

	DRM_DEBUG_KMS("%s: ---- CORE SYNC REGISTERS ----\n", prefix);
	DUMPREG(HDMI_H_BLANK_0);
	DUMPREG(HDMI_H_BLANK_1);
	DUMPREG(HDMI_V2_BLANK_0);
	DUMPREG(HDMI_V2_BLANK_1);
	DUMPREG(HDMI_V1_BLANK_0);
	DUMPREG(HDMI_V1_BLANK_1);
	DUMPREG(HDMI_V_LINE_0);
	DUMPREG(HDMI_V_LINE_1);
	DUMPREG(HDMI_H_LINE_0);
	DUMPREG(HDMI_H_LINE_1);
	DUMPREG(HDMI_HSYNC_POL);

	DUMPREG(HDMI_VSYNC_POL);
	DUMPREG(HDMI_INT_PRO_MODE);
	DUMPREG(HDMI_V_BLANK_F0_0);
	DUMPREG(HDMI_V_BLANK_F0_1);
	DUMPREG(HDMI_V_BLANK_F1_0);
	DUMPREG(HDMI_V_BLANK_F1_1);

	DUMPREG(HDMI_H_SYNC_START_0);
	DUMPREG(HDMI_H_SYNC_START_1);
	DUMPREG(HDMI_H_SYNC_END_0);
	DUMPREG(HDMI_H_SYNC_END_1);

	DUMPREG(HDMI_V_SYNC_LINE_BEF_2_0);
	DUMPREG(HDMI_V_SYNC_LINE_BEF_2_1);
	DUMPREG(HDMI_V_SYNC_LINE_BEF_1_0);
	DUMPREG(HDMI_V_SYNC_LINE_BEF_1_1);

	DUMPREG(HDMI_V_SYNC_LINE_AFT_2_0);
	DUMPREG(HDMI_V_SYNC_LINE_AFT_2_1);
	DUMPREG(HDMI_V_SYNC_LINE_AFT_1_0);
	DUMPREG(HDMI_V_SYNC_LINE_AFT_1_1);

	DUMPREG(HDMI_V_SYNC_LINE_AFT_PXL_2_0);
	DUMPREG(HDMI_V_SYNC_LINE_AFT_PXL_2_1);
	DUMPREG(HDMI_V_SYNC_LINE_AFT_PXL_1_0);
	DUMPREG(HDMI_V_SYNC_LINE_AFT_PXL_1_1);

	DUMPREG(HDMI_V_BLANK_F2_0);
	DUMPREG(HDMI_V_BLANK_F2_1);
	DUMPREG(HDMI_V_BLANK_F3_0);
	DUMPREG(HDMI_V_BLANK_F3_1);
	DUMPREG(HDMI_V_BLANK_F4_0);
	DUMPREG(HDMI_V_BLANK_F4_1);
	DUMPREG(HDMI_V_BLANK_F5_0);
	DUMPREG(HDMI_V_BLANK_F5_1);

	DUMPREG(HDMI_V_SYNC_LINE_AFT_3_0);
	DUMPREG(HDMI_V_SYNC_LINE_AFT_3_1);
	DUMPREG(HDMI_V_SYNC_LINE_AFT_4_0);
	DUMPREG(HDMI_V_SYNC_LINE_AFT_4_1);
	DUMPREG(HDMI_V_SYNC_LINE_AFT_5_0);
	DUMPREG(HDMI_V_SYNC_LINE_AFT_5_1);
	DUMPREG(HDMI_V_SYNC_LINE_AFT_6_0);
	DUMPREG(HDMI_V_SYNC_LINE_AFT_6_1);

	DUMPREG(HDMI_V_SYNC_LINE_AFT_PXL_3_0);
	DUMPREG(HDMI_V_SYNC_LINE_AFT_PXL_3_1);
	DUMPREG(HDMI_V_SYNC_LINE_AFT_PXL_4_0);
	DUMPREG(HDMI_V_SYNC_LINE_AFT_PXL_4_1);
	DUMPREG(HDMI_V_SYNC_LINE_AFT_PXL_5_0);
	DUMPREG(HDMI_V_SYNC_LINE_AFT_PXL_5_1);
	DUMPREG(HDMI_V_SYNC_LINE_AFT_PXL_6_0);
	DUMPREG(HDMI_V_SYNC_LINE_AFT_PXL_6_1);

	DUMPREG(HDMI_VACT_SPACE_1_0);
	DUMPREG(HDMI_VACT_SPACE_1_1);
	DUMPREG(HDMI_VACT_SPACE_2_0);
	DUMPREG(HDMI_VACT_SPACE_2_1);
	DUMPREG(HDMI_VACT_SPACE_3_0);
	DUMPREG(HDMI_VACT_SPACE_3_1);
	DUMPREG(HDMI_VACT_SPACE_4_0);
	DUMPREG(HDMI_VACT_SPACE_4_1);
	DUMPREG(HDMI_VACT_SPACE_5_0);
	DUMPREG(HDMI_VACT_SPACE_5_1);
	DUMPREG(HDMI_VACT_SPACE_6_0);
	DUMPREG(HDMI_VACT_SPACE_6_1);

	DRM_DEBUG_KMS("%s: ---- TG REGISTERS ----\n", prefix);
	DUMPREG(HDMI_TG_CMD);
	DUMPREG(HDMI_TG_H_FSZ_L);
	DUMPREG(HDMI_TG_H_FSZ_H);
	DUMPREG(HDMI_TG_HACT_ST_L);
	DUMPREG(HDMI_TG_HACT_ST_H);
	DUMPREG(HDMI_TG_HACT_SZ_L);
	DUMPREG(HDMI_TG_HACT_SZ_H);
	DUMPREG(HDMI_TG_V_FSZ_L);
	DUMPREG(HDMI_TG_V_FSZ_H);
	DUMPREG(HDMI_TG_VSYNC_L);
	DUMPREG(HDMI_TG_VSYNC_H);
	DUMPREG(HDMI_TG_VSYNC2_L);
	DUMPREG(HDMI_TG_VSYNC2_H);
	DUMPREG(HDMI_TG_VACT_ST_L);
	DUMPREG(HDMI_TG_VACT_ST_H);
	DUMPREG(HDMI_TG_VACT_SZ_L);
	DUMPREG(HDMI_TG_VACT_SZ_H);
	DUMPREG(HDMI_TG_FIELD_CHG_L);
	DUMPREG(HDMI_TG_FIELD_CHG_H);
	DUMPREG(HDMI_TG_VACT_ST2_L);
	DUMPREG(HDMI_TG_VACT_ST2_H);
	DUMPREG(HDMI_TG_VACT_ST3_L);
	DUMPREG(HDMI_TG_VACT_ST3_H);
	DUMPREG(HDMI_TG_VACT_ST4_L);
	DUMPREG(HDMI_TG_VACT_ST4_H);
	DUMPREG(HDMI_TG_VSYNC_TOP_HDMI_L);
	DUMPREG(HDMI_TG_VSYNC_TOP_HDMI_H);
	DUMPREG(HDMI_TG_VSYNC_BOT_HDMI_L);
	DUMPREG(HDMI_TG_VSYNC_BOT_HDMI_H);
	DUMPREG(HDMI_TG_FIELD_TOP_HDMI_L);
	DUMPREG(HDMI_TG_FIELD_TOP_HDMI_H);
	DUMPREG(HDMI_TG_FIELD_BOT_HDMI_L);
	DUMPREG(HDMI_TG_FIELD_BOT_HDMI_H);
	DUMPREG(HDMI_TG_3D);

	DRM_DEBUG_KMS("%s: ---- PACKET REGISTERS ----\n", prefix);
	DUMPREG(HDMI_AVI_CON);
	DUMPREG(HDMI_AVI_HEADER0);
	DUMPREG(HDMI_AVI_HEADER1);
	DUMPREG(HDMI_AVI_HEADER2);
	DUMPREG(HDMI_AVI_CHECK_SUM);
	DUMPREG(HDMI_VSI_CON);
	DUMPREG(HDMI_VSI_HEADER0);
	DUMPREG(HDMI_VSI_HEADER1);
	DUMPREG(HDMI_VSI_HEADER2);
	for (i = 0; i < 7; ++i)
		DUMPREG(HDMI_VSI_DATA(i));

#undef DUMPREG
}

static void hdmi_regs_dump(struct hdmi_context *hdata, char *prefix)
{
	if (hdata->is_v13)
		hdmi_v13_regs_dump(hdata, prefix);
	else
		hdmi_v14_regs_dump(hdata, prefix);
}

static int hdmi_v13_conf_index(struct drm_display_mode *mode)
{
	int i;

	for (i = 0; i < ARRAY_SIZE(hdmi_v13_confs); ++i)
		if (hdmi_v13_confs[i].width == mode->hdisplay &&
				hdmi_v13_confs[i].height == mode->vdisplay &&
				hdmi_v13_confs[i].vrefresh == mode->vrefresh &&
				hdmi_v13_confs[i].interlace ==
				((mode->flags & DRM_MODE_FLAG_INTERLACE) ?
				 true : false))
			return i;

	return -EINVAL;
}

static int hdmi_v14_conf_index(struct drm_display_mode *mode)
{
	int i;

	for (i = 0; i < ARRAY_SIZE(hdmi_confs); ++i)
		if (hdmi_confs[i].width == mode->hdisplay &&
				hdmi_confs[i].height == mode->vdisplay &&
				hdmi_confs[i].vrefresh == mode->vrefresh &&
				hdmi_confs[i].interlace ==
				((mode->flags & DRM_MODE_FLAG_INTERLACE) ?
				 true : false))
			return i;

	return -EINVAL;
}

static int hdmi_conf_index(struct hdmi_context *hdata,
			   struct drm_display_mode *mode)
{
	if (hdata->is_v13)
		return hdmi_v13_conf_index(mode);

	return hdmi_v14_conf_index(mode);
}

static bool hdmi_is_connected(void *ctx)
{
	struct hdmi_context *hdata = ctx;
	u32 val = hdmi_reg_read(hdata, HDMI_HPD_STATUS);

	if (val)
		return true;

	return false;
}

static int hdmi_get_edid(void *ctx, struct drm_connector *connector,
				u8 *edid, int len)
{
	struct edid *raw_edid;
	struct hdmi_context *hdata = ctx;

	DRM_DEBUG_KMS("[%d] %s\n", __LINE__, __func__);

	if (!hdata->ddc_port)
		return -ENODEV;

	raw_edid = drm_get_edid(connector, hdata->ddc_port->adapter);
	if (raw_edid) {
		memcpy(edid, raw_edid, min((1 + raw_edid->extensions)
					* EDID_LENGTH, len));
		DRM_DEBUG_KMS("width[%d] x height[%d]\n",
				raw_edid->width_cm, raw_edid->height_cm);
	} else {
		return -ENODEV;
	}

	return 0;
}

static int hdmi_v13_check_timing(struct fb_videomode *check_timing)
{
	int i;

	DRM_DEBUG_KMS("valid mode : xres=%d, yres=%d, refresh=%d, intl=%d\n",
			check_timing->xres, check_timing->yres,
			check_timing->refresh, (check_timing->vmode &
			FB_VMODE_INTERLACED) ? true : false);

	for (i = 0; i < ARRAY_SIZE(hdmi_v13_confs); ++i)
		if (hdmi_v13_confs[i].width == check_timing->xres &&
			hdmi_v13_confs[i].height == check_timing->yres &&
			hdmi_v13_confs[i].vrefresh == check_timing->refresh &&
			hdmi_v13_confs[i].interlace ==
			((check_timing->vmode & FB_VMODE_INTERLACED) ?
			 true : false))
				return 0;

	/* TODO */

	return -EINVAL;
}

static int hdmi_v14_check_timing(struct fb_videomode *check_timing)
{
	int i;

	DRM_DEBUG_KMS("valid mode : xres=%d, yres=%d, refresh=%d, intl=%d\n",
			check_timing->xres, check_timing->yres,
			check_timing->refresh, (check_timing->vmode &
			FB_VMODE_INTERLACED) ? true : false);

	for (i = 0; i < ARRAY_SIZE(hdmi_confs); i++)
		if (hdmi_confs[i].width == check_timing->xres &&
			hdmi_confs[i].height == check_timing->yres &&
			hdmi_confs[i].vrefresh == check_timing->refresh &&
			hdmi_confs[i].interlace ==
			((check_timing->vmode & FB_VMODE_INTERLACED) ?
			 true : false))
				return 0;

	/* TODO */

	return -EINVAL;
}

static int hdmi_check_timing(void *ctx, void *timing)
{
	struct hdmi_context *hdata = ctx;
	struct fb_videomode *check_timing = timing;

	DRM_DEBUG_KMS("[%d] %s\n", __LINE__, __func__);

	DRM_DEBUG_KMS("[%d]x[%d] [%d]Hz [%x]\n", check_timing->xres,
			check_timing->yres, check_timing->refresh,
			check_timing->vmode);

	if (hdata->is_v13)
		return hdmi_v13_check_timing(check_timing);
	else
		return hdmi_v14_check_timing(check_timing);
}

static int hdmi_display_power_on(void *ctx, int mode)
{
	DRM_DEBUG_KMS("[%d] %s\n", __LINE__, __func__);

	switch (mode) {
	case DRM_MODE_DPMS_ON:
		DRM_DEBUG_KMS("hdmi [on]\n");
		break;
	case DRM_MODE_DPMS_STANDBY:
		break;
	case DRM_MODE_DPMS_SUSPEND:
		break;
	case DRM_MODE_DPMS_OFF:
		DRM_DEBUG_KMS("hdmi [off]\n");
		break;
	default:
		break;
	}

	return 0;
}

static void hdmi_set_acr(u32 freq, u8 *acr)
{
	u32 n, cts;

	switch (freq) {
	case 32000:
		n = 4096;
		cts = 27000;
		break;
	case 44100:
		n = 6272;
		cts = 30000;
		break;
	case 88200:
		n = 12544;
		cts = 30000;
		break;
	case 176400:
		n = 25088;
		cts = 30000;
		break;
	case 48000:
		n = 6144;
		cts = 27000;
		break;
	case 96000:
		n = 12288;
		cts = 27000;
		break;
	case 192000:
		n = 24576;
		cts = 27000;
		break;
	default:
		n = 0;
		cts = 0;
		break;
	}

	acr[1] = cts >> 16;
	acr[2] = cts >> 8 & 0xff;
	acr[3] = cts & 0xff;

	acr[4] = n >> 16;
	acr[5] = n >> 8 & 0xff;
	acr[6] = n & 0xff;
}

static void hdmi_reg_acr(struct hdmi_context *hdata, u8 *acr)
{
	hdmi_reg_writeb(hdata, HDMI_ACR_N0, acr[6]);
	hdmi_reg_writeb(hdata, HDMI_ACR_N1, acr[5]);
	hdmi_reg_writeb(hdata, HDMI_ACR_N2, acr[4]);
	hdmi_reg_writeb(hdata, HDMI_ACR_MCTS0, acr[3]);
	hdmi_reg_writeb(hdata, HDMI_ACR_MCTS1, acr[2]);
	hdmi_reg_writeb(hdata, HDMI_ACR_MCTS2, acr[1]);
	hdmi_reg_writeb(hdata, HDMI_ACR_CTS0, acr[3]);
	hdmi_reg_writeb(hdata, HDMI_ACR_CTS1, acr[2]);
	hdmi_reg_writeb(hdata, HDMI_ACR_CTS2, acr[1]);

	if (hdata->is_v13)
		hdmi_reg_writeb(hdata, HDMI_V13_ACR_CON, 4);
	else
		hdmi_reg_writeb(hdata, HDMI_ACR_CON, 4);
}

static void hdmi_audio_init(struct hdmi_context *hdata)
{
	u32 sample_rate, bits_per_sample, frame_size_code;
	u32 data_num, bit_ch, sample_frq;
	u32 val;
	u8 acr[7];

	sample_rate = 44100;
	bits_per_sample = 16;
	frame_size_code = 0;

	switch (bits_per_sample) {
	case 20:
		data_num = 2;
		bit_ch  = 1;
		break;
	case 24:
		data_num = 3;
		bit_ch  = 1;
		break;
	default:
		data_num = 1;
		bit_ch  = 0;
		break;
	}

	hdmi_set_acr(sample_rate, acr);
	hdmi_reg_acr(hdata, acr);
=======
static int hdmiphy_reg_write_buf(struct hdmi_context *hdata,
			u32 reg_offset, const u8 *buf, u32 len)
{
	if ((reg_offset + len) > 32)
		return -EINVAL;

	if (hdata->hdmiphy_port) {
		int ret;

		ret = i2c_master_send(hdata->hdmiphy_port, buf, len);
		if (ret == len)
			return 0;
		return ret;
	} else {
		int i;
		for (i = 0; i < len; i++)
			writel(buf[i], hdata->regs_hdmiphy +
				((reg_offset + i)<<2));
		return 0;
	}
}

static int hdmi_clk_enable_gates(struct hdmi_context *hdata)
{
	int i, ret;

	for (i = 0; i < hdata->drv_data->clk_gates.count; ++i) {
		ret = clk_prepare_enable(hdata->clk_gates[i]);
		if (!ret)
			continue;

		dev_err(hdata->dev, "Cannot enable clock '%s', %d\n",
			hdata->drv_data->clk_gates.data[i], ret);
		while (i--)
			clk_disable_unprepare(hdata->clk_gates[i]);
		return ret;
	}

	return 0;
}

static void hdmi_clk_disable_gates(struct hdmi_context *hdata)
{
	int i = hdata->drv_data->clk_gates.count;

	while (i--)
		clk_disable_unprepare(hdata->clk_gates[i]);
}

static int hdmi_clk_set_parents(struct hdmi_context *hdata, bool to_phy)
{
	struct device *dev = hdata->dev;
	int ret = 0;
	int i;

	for (i = 0; i < hdata->drv_data->clk_muxes.count; i += 3) {
		struct clk **c = &hdata->clk_muxes[i];

		ret = clk_set_parent(c[2], c[to_phy]);
		if (!ret)
			continue;

		dev_err(dev, "Cannot set clock parent of '%s' to '%s', %d\n",
			hdata->drv_data->clk_muxes.data[i + 2],
			hdata->drv_data->clk_muxes.data[i + to_phy], ret);
	}

	return ret;
}

static int hdmi_audio_infoframe_apply(struct hdmi_context *hdata)
{
	struct hdmi_audio_infoframe *infoframe = &hdata->audio.infoframe;
	u8 buf[HDMI_INFOFRAME_SIZE(AUDIO)];
	int len;

	len = hdmi_audio_infoframe_pack(infoframe, buf, sizeof(buf));
	if (len < 0)
		return len;

	hdmi_reg_writeb(hdata, HDMI_AUI_CON, HDMI_AUI_CON_EVERY_VSYNC);
	hdmi_reg_write_buf(hdata, HDMI_AUI_HEADER0, buf, len);

	return 0;
}

static void hdmi_reg_infoframes(struct hdmi_context *hdata)
{
	struct drm_display_mode *m = &hdata->encoder.crtc->state->mode;
	union hdmi_infoframe frm;
	u8 buf[25];
	int ret;

	if (hdata->dvi_mode) {
		hdmi_reg_writeb(hdata, HDMI_AVI_CON,
				HDMI_AVI_CON_DO_NOT_TRANSMIT);
		hdmi_reg_writeb(hdata, HDMI_VSI_CON,
				HDMI_VSI_CON_DO_NOT_TRANSMIT);
		hdmi_reg_writeb(hdata, HDMI_AUI_CON, HDMI_AUI_CON_NO_TRAN);
		return;
	}

	ret = drm_hdmi_avi_infoframe_from_display_mode(&frm.avi,
						       &hdata->connector, m);
	if (!ret)
		ret = hdmi_avi_infoframe_pack(&frm.avi, buf, sizeof(buf));
	if (ret > 0) {
		hdmi_reg_writeb(hdata, HDMI_AVI_CON, HDMI_AVI_CON_EVERY_VSYNC);
		hdmi_reg_write_buf(hdata, HDMI_AVI_HEADER0, buf, ret);
	} else {
		DRM_INFO("%s: invalid AVI infoframe (%d)\n", __func__, ret);
	}

	ret = drm_hdmi_vendor_infoframe_from_display_mode(&frm.vendor.hdmi,
							  &hdata->connector, m);
	if (!ret)
		ret = hdmi_vendor_infoframe_pack(&frm.vendor.hdmi, buf,
				sizeof(buf));
	if (ret > 0) {
		hdmi_reg_writeb(hdata, HDMI_VSI_CON, HDMI_VSI_CON_EVERY_VSYNC);
		hdmi_reg_write_buf(hdata, HDMI_VSI_HEADER0, buf, 3);
		hdmi_reg_write_buf(hdata, HDMI_VSI_DATA(0), buf + 3, ret - 3);
	}

	hdmi_audio_infoframe_apply(hdata);
}

static enum drm_connector_status hdmi_detect(struct drm_connector *connector,
				bool force)
{
	struct hdmi_context *hdata = connector_to_hdmi(connector);

	if (gpiod_get_value(hdata->hpd_gpio))
		return connector_status_connected;

	cec_notifier_set_phys_addr(hdata->notifier, CEC_PHYS_ADDR_INVALID);
	return connector_status_disconnected;
}

static void hdmi_connector_destroy(struct drm_connector *connector)
{
	struct hdmi_context *hdata = connector_to_hdmi(connector);

	cec_notifier_conn_unregister(hdata->notifier);

	drm_connector_unregister(connector);
	drm_connector_cleanup(connector);
}

static const struct drm_connector_funcs hdmi_connector_funcs = {
	.fill_modes = drm_helper_probe_single_connector_modes,
	.detect = hdmi_detect,
	.destroy = hdmi_connector_destroy,
	.reset = drm_atomic_helper_connector_reset,
	.atomic_duplicate_state = drm_atomic_helper_connector_duplicate_state,
	.atomic_destroy_state = drm_atomic_helper_connector_destroy_state,
};

static int hdmi_get_modes(struct drm_connector *connector)
{
	struct hdmi_context *hdata = connector_to_hdmi(connector);
	struct edid *edid;
	int ret;

	if (!hdata->ddc_adpt)
		return 0;

	edid = drm_get_edid(connector, hdata->ddc_adpt);
	if (!edid)
		return 0;

	hdata->dvi_mode = !connector->display_info.is_hdmi;
	DRM_DEV_DEBUG_KMS(hdata->dev, "%s : width[%d] x height[%d]\n",
			  (hdata->dvi_mode ? "dvi monitor" : "hdmi monitor"),
			  edid->width_cm, edid->height_cm);

	drm_connector_update_edid_property(connector, edid);
	cec_notifier_set_phys_addr_from_edid(hdata->notifier, edid);

	ret = drm_add_edid_modes(connector, edid);

	kfree(edid);

	return ret;
}

static int hdmi_find_phy_conf(struct hdmi_context *hdata, u32 pixel_clock)
{
	const struct hdmiphy_configs *confs = &hdata->drv_data->phy_confs;
	int i;

	for (i = 0; i < confs->count; i++)
		if (confs->data[i].pixel_clock == pixel_clock)
			return i;

	DRM_DEV_DEBUG_KMS(hdata->dev, "Could not find phy config for %d\n",
			  pixel_clock);
	return -EINVAL;
}

static enum drm_mode_status hdmi_mode_valid(struct drm_connector *connector,
					    struct drm_display_mode *mode)
{
	struct hdmi_context *hdata = connector_to_hdmi(connector);
	int ret;

	DRM_DEV_DEBUG_KMS(hdata->dev,
			  "xres=%d, yres=%d, refresh=%d, intl=%d clock=%d\n",
			  mode->hdisplay, mode->vdisplay,
			  drm_mode_vrefresh(mode),
			  (mode->flags & DRM_MODE_FLAG_INTERLACE) ? true :
			  false, mode->clock * 1000);

	ret = hdmi_find_phy_conf(hdata, mode->clock * 1000);
	if (ret < 0)
		return MODE_BAD;

	return MODE_OK;
}

static const struct drm_connector_helper_funcs hdmi_connector_helper_funcs = {
	.get_modes = hdmi_get_modes,
	.mode_valid = hdmi_mode_valid,
};

static int hdmi_create_connector(struct drm_encoder *encoder)
{
	struct hdmi_context *hdata = encoder_to_hdmi(encoder);
	struct drm_connector *connector = &hdata->connector;
	struct cec_connector_info conn_info;
	int ret;

	connector->interlace_allowed = true;
	connector->polled = DRM_CONNECTOR_POLL_HPD;

	ret = drm_connector_init_with_ddc(hdata->drm_dev, connector,
					  &hdmi_connector_funcs,
					  DRM_MODE_CONNECTOR_HDMIA,
					  hdata->ddc_adpt);
	if (ret) {
		DRM_DEV_ERROR(hdata->dev,
			      "Failed to initialize connector with drm\n");
		return ret;
	}

	drm_connector_helper_add(connector, &hdmi_connector_helper_funcs);
	drm_connector_attach_encoder(connector, encoder);

	if (hdata->bridge)
		ret = drm_bridge_attach(encoder, hdata->bridge, NULL, 0);

	cec_fill_conn_info_from_drm(&conn_info, connector);

	hdata->notifier = cec_notifier_conn_register(hdata->dev, NULL,
						     &conn_info);
	if (!hdata->notifier) {
		ret = -ENOMEM;
		DRM_DEV_ERROR(hdata->dev, "Failed to allocate CEC notifier\n");
	}

	return ret;
}

static bool hdmi_mode_fixup(struct drm_encoder *encoder,
			    const struct drm_display_mode *mode,
			    struct drm_display_mode *adjusted_mode)
{
	struct drm_device *dev = encoder->dev;
	struct drm_connector *connector;
	struct drm_display_mode *m;
	struct drm_connector_list_iter conn_iter;
	int mode_ok;

	drm_mode_set_crtcinfo(adjusted_mode, 0);

	drm_connector_list_iter_begin(dev, &conn_iter);
	drm_for_each_connector_iter(connector, &conn_iter) {
		if (connector->encoder == encoder)
			break;
	}
	if (connector)
		drm_connector_get(connector);
	drm_connector_list_iter_end(&conn_iter);

	if (!connector)
		return true;

	mode_ok = hdmi_mode_valid(connector, adjusted_mode);

	if (mode_ok == MODE_OK)
		goto cleanup;

	/*
	 * Find the most suitable mode and copy it to adjusted_mode.
	 */
	list_for_each_entry(m, &connector->modes, head) {
		mode_ok = hdmi_mode_valid(connector, m);

		if (mode_ok == MODE_OK) {
			DRM_INFO("desired mode doesn't exist so\n");
			DRM_INFO("use the most suitable mode among modes.\n");

			DRM_DEV_DEBUG_KMS(dev->dev,
					  "Adjusted Mode: [%d]x[%d] [%d]Hz\n",
					  m->hdisplay, m->vdisplay,
					  drm_mode_vrefresh(m));

			drm_mode_copy(adjusted_mode, m);
			break;
		}
	}

cleanup:
	drm_connector_put(connector);

	return true;
}

static void hdmi_reg_acr(struct hdmi_context *hdata, u32 freq)
{
	u32 n, cts;

	cts = (freq % 9) ? 27000 : 30000;
	n = 128 * freq / (27000000 / cts);

	hdmi_reg_writev(hdata, HDMI_ACR_N0, 3, n);
	hdmi_reg_writev(hdata, HDMI_ACR_MCTS0, 3, cts);
	hdmi_reg_writev(hdata, HDMI_ACR_CTS0, 3, cts);
	hdmi_reg_writeb(hdata, HDMI_ACR_CON, 4);
}

static void hdmi_audio_config(struct hdmi_context *hdata)
{
	u32 bit_ch = 1;
	u32 data_num, val;
	int i;

	switch (hdata->audio.params.sample_width) {
	case 20:
		data_num = 2;
		break;
	case 24:
		data_num = 3;
		break;
	default:
		data_num = 1;
		bit_ch = 0;
		break;
	}

	hdmi_reg_acr(hdata, hdata->audio.params.sample_rate);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	hdmi_reg_writeb(hdata, HDMI_I2S_MUX_CON, HDMI_I2S_IN_DISABLE
				| HDMI_I2S_AUD_I2S | HDMI_I2S_CUV_I2S_ENABLE
				| HDMI_I2S_MUX_ENABLE);

	hdmi_reg_writeb(hdata, HDMI_I2S_MUX_CH, HDMI_I2S_CH0_EN
			| HDMI_I2S_CH1_EN | HDMI_I2S_CH2_EN);

	hdmi_reg_writeb(hdata, HDMI_I2S_MUX_CUV, HDMI_I2S_CUV_RL_EN);
<<<<<<< HEAD

	sample_frq = (sample_rate == 44100) ? 0 :
			(sample_rate == 48000) ? 2 :
			(sample_rate == 32000) ? 3 :
			(sample_rate == 96000) ? 0xa : 0x0;

=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	hdmi_reg_writeb(hdata, HDMI_I2S_CLK_CON, HDMI_I2S_CLK_DIS);
	hdmi_reg_writeb(hdata, HDMI_I2S_CLK_CON, HDMI_I2S_CLK_EN);

	val = hdmi_reg_read(hdata, HDMI_I2S_DSD_CON) | 0x01;
	hdmi_reg_writeb(hdata, HDMI_I2S_DSD_CON, val);

	/* Configuration I2S input ports. Configure I2S_PIN_SEL_0~4 */
	hdmi_reg_writeb(hdata, HDMI_I2S_PIN_SEL_0, HDMI_I2S_SEL_SCLK(5)
			| HDMI_I2S_SEL_LRCK(6));
<<<<<<< HEAD
	hdmi_reg_writeb(hdata, HDMI_I2S_PIN_SEL_1, HDMI_I2S_SEL_SDATA1(1)
			| HDMI_I2S_SEL_SDATA2(4));
	hdmi_reg_writeb(hdata, HDMI_I2S_PIN_SEL_2, HDMI_I2S_SEL_SDATA3(1)
			| HDMI_I2S_SEL_SDATA2(2));
=======

	hdmi_reg_writeb(hdata, HDMI_I2S_PIN_SEL_1, HDMI_I2S_SEL_SDATA1(3)
			| HDMI_I2S_SEL_SDATA0(4));

	hdmi_reg_writeb(hdata, HDMI_I2S_PIN_SEL_2, HDMI_I2S_SEL_SDATA3(1)
			| HDMI_I2S_SEL_SDATA2(2));

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	hdmi_reg_writeb(hdata, HDMI_I2S_PIN_SEL_3, HDMI_I2S_SEL_DSD(0));

	/* I2S_CON_1 & 2 */
	hdmi_reg_writeb(hdata, HDMI_I2S_CON_1, HDMI_I2S_SCLK_FALLING_EDGE
			| HDMI_I2S_L_CH_LOW_POL);
	hdmi_reg_writeb(hdata, HDMI_I2S_CON_2, HDMI_I2S_MSB_FIRST_MODE
			| HDMI_I2S_SET_BIT_CH(bit_ch)
			| HDMI_I2S_SET_SDATA_BIT(data_num)
			| HDMI_I2S_BASIC_FORMAT);

<<<<<<< HEAD
	/* Configure register related to CUV information */
	hdmi_reg_writeb(hdata, HDMI_I2S_CH_ST_0, HDMI_I2S_CH_STATUS_MODE_0
			| HDMI_I2S_2AUD_CH_WITHOUT_PREEMPH
			| HDMI_I2S_COPYRIGHT
			| HDMI_I2S_LINEAR_PCM
			| HDMI_I2S_CONSUMER_FORMAT);
	hdmi_reg_writeb(hdata, HDMI_I2S_CH_ST_1, HDMI_I2S_CD_PLAYER);
	hdmi_reg_writeb(hdata, HDMI_I2S_CH_ST_2, HDMI_I2S_SET_SOURCE_NUM(0));
	hdmi_reg_writeb(hdata, HDMI_I2S_CH_ST_3, HDMI_I2S_CLK_ACCUR_LEVEL_2
			| HDMI_I2S_SET_SMP_FREQ(sample_frq));
	hdmi_reg_writeb(hdata, HDMI_I2S_CH_ST_4,
			HDMI_I2S_ORG_SMP_FREQ_44_1
			| HDMI_I2S_WORD_LEN_MAX24_24BITS
			| HDMI_I2S_WORD_LEN_MAX_24BITS);
=======
	/* Configuration of the audio channel status registers */
	for (i = 0; i < HDMI_I2S_CH_ST_MAXNUM; i++)
		hdmi_reg_writeb(hdata, HDMI_I2S_CH_ST(i),
				hdata->audio.params.iec.status[i]);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	hdmi_reg_writeb(hdata, HDMI_I2S_CH_ST_CON, HDMI_I2S_CH_STATUS_RELOAD);
}

<<<<<<< HEAD
static void hdmi_audio_control(struct hdmi_context *hdata, bool onoff)
{
	u32 mod;

	mod = hdmi_reg_read(hdata, HDMI_MODE_SEL);
	if (mod & HDMI_DVI_MODE_EN)
		return;

	hdmi_reg_writeb(hdata, HDMI_AUI_CON, onoff ? 2 : 0);
	hdmi_reg_writemask(hdata, HDMI_CON_0, onoff ?
			HDMI_ASP_EN : HDMI_ASP_DIS, HDMI_ASP_MASK);
}

static void hdmi_conf_reset(struct hdmi_context *hdata)
{
	u32 reg;

	/* disable hpd handle for drm */
	hdata->hpd_handle = false;

	if (hdata->is_v13)
		reg = HDMI_V13_CORE_RSTOUT;
	else
		reg = HDMI_CORE_RSTOUT;

	/* resetting HDMI core */
	hdmi_reg_writemask(hdata, reg,  0, HDMI_CORE_SW_RSTOUT);
	mdelay(10);
	hdmi_reg_writemask(hdata, reg, ~0, HDMI_CORE_SW_RSTOUT);
	mdelay(10);

	/* enable hpd handle for drm */
	hdata->hpd_handle = true;
=======
static void hdmi_audio_control(struct hdmi_context *hdata)
{
	bool enable = !hdata->audio.mute;

	if (hdata->dvi_mode)
		return;

	hdmi_reg_writeb(hdata, HDMI_AUI_CON, enable ?
			HDMI_AVI_CON_EVERY_VSYNC : HDMI_AUI_CON_NO_TRAN);
	hdmi_reg_writemask(hdata, HDMI_CON_0, enable ?
			HDMI_ASP_EN : HDMI_ASP_DIS, HDMI_ASP_MASK);
}

static void hdmi_start(struct hdmi_context *hdata, bool start)
{
	struct drm_display_mode *m = &hdata->encoder.crtc->state->mode;
	u32 val = start ? HDMI_TG_EN : 0;

	if (m->flags & DRM_MODE_FLAG_INTERLACE)
		val |= HDMI_FIELD_EN;

	hdmi_reg_writemask(hdata, HDMI_CON_0, val, HDMI_EN);
	hdmi_reg_writemask(hdata, HDMI_TG_CMD, val, HDMI_TG_EN | HDMI_FIELD_EN);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

static void hdmi_conf_init(struct hdmi_context *hdata)
{
<<<<<<< HEAD
	/* disable hpd handle for drm */
	hdata->hpd_handle = false;

	/* enable HPD interrupts */
	hdmi_reg_writemask(hdata, HDMI_INTC_CON, 0, HDMI_INTC_EN_GLOBAL |
		HDMI_INTC_EN_HPD_PLUG | HDMI_INTC_EN_HPD_UNPLUG);
	mdelay(10);
	hdmi_reg_writemask(hdata, HDMI_INTC_CON, ~0, HDMI_INTC_EN_GLOBAL |
		HDMI_INTC_EN_HPD_PLUG | HDMI_INTC_EN_HPD_UNPLUG);
=======
	/* disable HPD interrupts from HDMI IP block, use GPIO instead */
	hdmi_reg_writemask(hdata, HDMI_INTC_CON, 0, HDMI_INTC_EN_GLOBAL |
		HDMI_INTC_EN_HPD_PLUG | HDMI_INTC_EN_HPD_UNPLUG);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	/* choose HDMI mode */
	hdmi_reg_writemask(hdata, HDMI_MODE_SEL,
		HDMI_MODE_HDMI_EN, HDMI_MODE_MASK);
<<<<<<< HEAD
	/* disable bluescreen */
	hdmi_reg_writemask(hdata, HDMI_CON_0, 0, HDMI_BLUE_SCR_EN);

	if (hdata->is_v13) {
=======
	/* apply video pre-amble and guard band in HDMI mode only */
	hdmi_reg_writeb(hdata, HDMI_CON_2, 0);
	/* disable bluescreen */
	hdmi_reg_writemask(hdata, HDMI_CON_0, 0, HDMI_BLUE_SCR_EN);

	if (hdata->dvi_mode) {
		hdmi_reg_writemask(hdata, HDMI_MODE_SEL,
				HDMI_MODE_DVI_EN, HDMI_MODE_MASK);
		hdmi_reg_writeb(hdata, HDMI_CON_2,
				HDMI_VID_PREAMBLE_DIS | HDMI_GUARD_BAND_DIS);
	}

	if (hdata->drv_data->type == HDMI_TYPE13) {
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		/* choose bluescreen (fecal) color */
		hdmi_reg_writeb(hdata, HDMI_V13_BLUE_SCREEN_0, 0x12);
		hdmi_reg_writeb(hdata, HDMI_V13_BLUE_SCREEN_1, 0x34);
		hdmi_reg_writeb(hdata, HDMI_V13_BLUE_SCREEN_2, 0x56);

		/* enable AVI packet every vsync, fixes purple line problem */
		hdmi_reg_writeb(hdata, HDMI_V13_AVI_CON, 0x02);
		/* force RGB, look to CEA-861-D, table 7 for more detail */
		hdmi_reg_writeb(hdata, HDMI_V13_AVI_BYTE(0), 0 << 5);
		hdmi_reg_writemask(hdata, HDMI_CON_1, 0x10 << 5, 0x11 << 5);

		hdmi_reg_writeb(hdata, HDMI_V13_SPD_CON, 0x02);
		hdmi_reg_writeb(hdata, HDMI_V13_AUI_CON, 0x02);
		hdmi_reg_writeb(hdata, HDMI_V13_ACR_CON, 0x04);
	} else {
<<<<<<< HEAD
		/* enable AVI packet every vsync, fixes purple line problem */
		hdmi_reg_writeb(hdata, HDMI_AVI_CON, 0x02);
		hdmi_reg_writeb(hdata, HDMI_AVI_BYTE(1), 2 << 5);
		hdmi_reg_writemask(hdata, HDMI_CON_1, 2, 3 << 5);
	}

	/* enable hpd handle for drm */
	hdata->hpd_handle = true;
}

static void hdmi_v13_timing_apply(struct hdmi_context *hdata)
{
	const struct hdmi_v13_preset_conf *conf =
		hdmi_v13_confs[hdata->cur_conf].conf;
	const struct hdmi_v13_core_regs *core = &conf->core;
	const struct hdmi_v13_tg_regs *tg = &conf->tg;
	int tries;

	/* setting core registers */
	hdmi_reg_writeb(hdata, HDMI_H_BLANK_0, core->h_blank[0]);
	hdmi_reg_writeb(hdata, HDMI_H_BLANK_1, core->h_blank[1]);
	hdmi_reg_writeb(hdata, HDMI_V13_V_BLANK_0, core->v_blank[0]);
	hdmi_reg_writeb(hdata, HDMI_V13_V_BLANK_1, core->v_blank[1]);
	hdmi_reg_writeb(hdata, HDMI_V13_V_BLANK_2, core->v_blank[2]);
	hdmi_reg_writeb(hdata, HDMI_V13_H_V_LINE_0, core->h_v_line[0]);
	hdmi_reg_writeb(hdata, HDMI_V13_H_V_LINE_1, core->h_v_line[1]);
	hdmi_reg_writeb(hdata, HDMI_V13_H_V_LINE_2, core->h_v_line[2]);
	hdmi_reg_writeb(hdata, HDMI_VSYNC_POL, core->vsync_pol[0]);
	hdmi_reg_writeb(hdata, HDMI_INT_PRO_MODE, core->int_pro_mode[0]);
	hdmi_reg_writeb(hdata, HDMI_V13_V_BLANK_F_0, core->v_blank_f[0]);
	hdmi_reg_writeb(hdata, HDMI_V13_V_BLANK_F_1, core->v_blank_f[1]);
	hdmi_reg_writeb(hdata, HDMI_V13_V_BLANK_F_2, core->v_blank_f[2]);
	hdmi_reg_writeb(hdata, HDMI_V13_H_SYNC_GEN_0, core->h_sync_gen[0]);
	hdmi_reg_writeb(hdata, HDMI_V13_H_SYNC_GEN_1, core->h_sync_gen[1]);
	hdmi_reg_writeb(hdata, HDMI_V13_H_SYNC_GEN_2, core->h_sync_gen[2]);
	hdmi_reg_writeb(hdata, HDMI_V13_V_SYNC_GEN_1_0, core->v_sync_gen1[0]);
	hdmi_reg_writeb(hdata, HDMI_V13_V_SYNC_GEN_1_1, core->v_sync_gen1[1]);
	hdmi_reg_writeb(hdata, HDMI_V13_V_SYNC_GEN_1_2, core->v_sync_gen1[2]);
	hdmi_reg_writeb(hdata, HDMI_V13_V_SYNC_GEN_2_0, core->v_sync_gen2[0]);
	hdmi_reg_writeb(hdata, HDMI_V13_V_SYNC_GEN_2_1, core->v_sync_gen2[1]);
	hdmi_reg_writeb(hdata, HDMI_V13_V_SYNC_GEN_2_2, core->v_sync_gen2[2]);
	hdmi_reg_writeb(hdata, HDMI_V13_V_SYNC_GEN_3_0, core->v_sync_gen3[0]);
	hdmi_reg_writeb(hdata, HDMI_V13_V_SYNC_GEN_3_1, core->v_sync_gen3[1]);
	hdmi_reg_writeb(hdata, HDMI_V13_V_SYNC_GEN_3_2, core->v_sync_gen3[2]);
	/* Timing generator registers */
	hdmi_reg_writeb(hdata, HDMI_TG_H_FSZ_L, tg->h_fsz_l);
	hdmi_reg_writeb(hdata, HDMI_TG_H_FSZ_H, tg->h_fsz_h);
	hdmi_reg_writeb(hdata, HDMI_TG_HACT_ST_L, tg->hact_st_l);
	hdmi_reg_writeb(hdata, HDMI_TG_HACT_ST_H, tg->hact_st_h);
	hdmi_reg_writeb(hdata, HDMI_TG_HACT_SZ_L, tg->hact_sz_l);
	hdmi_reg_writeb(hdata, HDMI_TG_HACT_SZ_H, tg->hact_sz_h);
	hdmi_reg_writeb(hdata, HDMI_TG_V_FSZ_L, tg->v_fsz_l);
	hdmi_reg_writeb(hdata, HDMI_TG_V_FSZ_H, tg->v_fsz_h);
	hdmi_reg_writeb(hdata, HDMI_TG_VSYNC_L, tg->vsync_l);
	hdmi_reg_writeb(hdata, HDMI_TG_VSYNC_H, tg->vsync_h);
	hdmi_reg_writeb(hdata, HDMI_TG_VSYNC2_L, tg->vsync2_l);
	hdmi_reg_writeb(hdata, HDMI_TG_VSYNC2_H, tg->vsync2_h);
	hdmi_reg_writeb(hdata, HDMI_TG_VACT_ST_L, tg->vact_st_l);
	hdmi_reg_writeb(hdata, HDMI_TG_VACT_ST_H, tg->vact_st_h);
	hdmi_reg_writeb(hdata, HDMI_TG_VACT_SZ_L, tg->vact_sz_l);
	hdmi_reg_writeb(hdata, HDMI_TG_VACT_SZ_H, tg->vact_sz_h);
	hdmi_reg_writeb(hdata, HDMI_TG_FIELD_CHG_L, tg->field_chg_l);
	hdmi_reg_writeb(hdata, HDMI_TG_FIELD_CHG_H, tg->field_chg_h);
	hdmi_reg_writeb(hdata, HDMI_TG_VACT_ST2_L, tg->vact_st2_l);
	hdmi_reg_writeb(hdata, HDMI_TG_VACT_ST2_H, tg->vact_st2_h);
	hdmi_reg_writeb(hdata, HDMI_TG_VSYNC_TOP_HDMI_L, tg->vsync_top_hdmi_l);
	hdmi_reg_writeb(hdata, HDMI_TG_VSYNC_TOP_HDMI_H, tg->vsync_top_hdmi_h);
	hdmi_reg_writeb(hdata, HDMI_TG_VSYNC_BOT_HDMI_L, tg->vsync_bot_hdmi_l);
	hdmi_reg_writeb(hdata, HDMI_TG_VSYNC_BOT_HDMI_H, tg->vsync_bot_hdmi_h);
	hdmi_reg_writeb(hdata, HDMI_TG_FIELD_TOP_HDMI_L, tg->field_top_hdmi_l);
	hdmi_reg_writeb(hdata, HDMI_TG_FIELD_TOP_HDMI_H, tg->field_top_hdmi_h);
	hdmi_reg_writeb(hdata, HDMI_TG_FIELD_BOT_HDMI_L, tg->field_bot_hdmi_l);
	hdmi_reg_writeb(hdata, HDMI_TG_FIELD_BOT_HDMI_H, tg->field_bot_hdmi_h);

	/* waiting for HDMIPHY's PLL to get to steady state */
	for (tries = 100; tries; --tries) {
		u32 val = hdmi_reg_read(hdata, HDMI_V13_PHY_STATUS);
		if (val & HDMI_PHY_STATUS_READY)
			break;
		mdelay(1);
	}
	/* steady state not achieved */
	if (tries == 0) {
		DRM_ERROR("hdmiphy's pll could not reach steady state.\n");
		hdmi_regs_dump(hdata, "timing apply");
	}

	clk_disable(hdata->res.sclk_hdmi);
	clk_set_parent(hdata->res.sclk_hdmi, hdata->res.sclk_hdmiphy);
	clk_enable(hdata->res.sclk_hdmi);

	/* enable HDMI and timing generator */
	hdmi_reg_writemask(hdata, HDMI_CON_0, ~0, HDMI_EN);
	if (core->int_pro_mode[0])
		hdmi_reg_writemask(hdata, HDMI_TG_CMD, ~0, HDMI_TG_EN |
				HDMI_FIELD_EN);
	else
		hdmi_reg_writemask(hdata, HDMI_TG_CMD, ~0, HDMI_TG_EN);
}

static void hdmi_v14_timing_apply(struct hdmi_context *hdata)
{
	const struct hdmi_preset_conf *conf = hdmi_confs[hdata->cur_conf].conf;
	const struct hdmi_core_regs *core = &conf->core;
	const struct hdmi_tg_regs *tg = &conf->tg;
	int tries;

	/* setting core registers */
	hdmi_reg_writeb(hdata, HDMI_H_BLANK_0, core->h_blank[0]);
	hdmi_reg_writeb(hdata, HDMI_H_BLANK_1, core->h_blank[1]);
	hdmi_reg_writeb(hdata, HDMI_V2_BLANK_0, core->v2_blank[0]);
	hdmi_reg_writeb(hdata, HDMI_V2_BLANK_1, core->v2_blank[1]);
	hdmi_reg_writeb(hdata, HDMI_V1_BLANK_0, core->v1_blank[0]);
	hdmi_reg_writeb(hdata, HDMI_V1_BLANK_1, core->v1_blank[1]);
	hdmi_reg_writeb(hdata, HDMI_V_LINE_0, core->v_line[0]);
	hdmi_reg_writeb(hdata, HDMI_V_LINE_1, core->v_line[1]);
	hdmi_reg_writeb(hdata, HDMI_H_LINE_0, core->h_line[0]);
	hdmi_reg_writeb(hdata, HDMI_H_LINE_1, core->h_line[1]);
	hdmi_reg_writeb(hdata, HDMI_HSYNC_POL, core->hsync_pol[0]);
	hdmi_reg_writeb(hdata, HDMI_VSYNC_POL, core->vsync_pol[0]);
	hdmi_reg_writeb(hdata, HDMI_INT_PRO_MODE, core->int_pro_mode[0]);
	hdmi_reg_writeb(hdata, HDMI_V_BLANK_F0_0, core->v_blank_f0[0]);
	hdmi_reg_writeb(hdata, HDMI_V_BLANK_F0_1, core->v_blank_f0[1]);
	hdmi_reg_writeb(hdata, HDMI_V_BLANK_F1_0, core->v_blank_f1[0]);
	hdmi_reg_writeb(hdata, HDMI_V_BLANK_F1_1, core->v_blank_f1[1]);
	hdmi_reg_writeb(hdata, HDMI_H_SYNC_START_0, core->h_sync_start[0]);
	hdmi_reg_writeb(hdata, HDMI_H_SYNC_START_1, core->h_sync_start[1]);
	hdmi_reg_writeb(hdata, HDMI_H_SYNC_END_0, core->h_sync_end[0]);
	hdmi_reg_writeb(hdata, HDMI_H_SYNC_END_1, core->h_sync_end[1]);
	hdmi_reg_writeb(hdata, HDMI_V_SYNC_LINE_BEF_2_0,
			core->v_sync_line_bef_2[0]);
	hdmi_reg_writeb(hdata, HDMI_V_SYNC_LINE_BEF_2_1,
			core->v_sync_line_bef_2[1]);
	hdmi_reg_writeb(hdata, HDMI_V_SYNC_LINE_BEF_1_0,
			core->v_sync_line_bef_1[0]);
	hdmi_reg_writeb(hdata, HDMI_V_SYNC_LINE_BEF_1_1,
			core->v_sync_line_bef_1[1]);
	hdmi_reg_writeb(hdata, HDMI_V_SYNC_LINE_AFT_2_0,
			core->v_sync_line_aft_2[0]);
	hdmi_reg_writeb(hdata, HDMI_V_SYNC_LINE_AFT_2_1,
			core->v_sync_line_aft_2[1]);
	hdmi_reg_writeb(hdata, HDMI_V_SYNC_LINE_AFT_1_0,
			core->v_sync_line_aft_1[0]);
	hdmi_reg_writeb(hdata, HDMI_V_SYNC_LINE_AFT_1_1,
			core->v_sync_line_aft_1[1]);
	hdmi_reg_writeb(hdata, HDMI_V_SYNC_LINE_AFT_PXL_2_0,
			core->v_sync_line_aft_pxl_2[0]);
	hdmi_reg_writeb(hdata, HDMI_V_SYNC_LINE_AFT_PXL_2_1,
			core->v_sync_line_aft_pxl_2[1]);
	hdmi_reg_writeb(hdata, HDMI_V_SYNC_LINE_AFT_PXL_1_0,
			core->v_sync_line_aft_pxl_1[0]);
	hdmi_reg_writeb(hdata, HDMI_V_SYNC_LINE_AFT_PXL_1_1,
			core->v_sync_line_aft_pxl_1[1]);
	hdmi_reg_writeb(hdata, HDMI_V_BLANK_F2_0, core->v_blank_f2[0]);
	hdmi_reg_writeb(hdata, HDMI_V_BLANK_F2_1, core->v_blank_f2[1]);
	hdmi_reg_writeb(hdata, HDMI_V_BLANK_F3_0, core->v_blank_f3[0]);
	hdmi_reg_writeb(hdata, HDMI_V_BLANK_F3_1, core->v_blank_f3[1]);
	hdmi_reg_writeb(hdata, HDMI_V_BLANK_F4_0, core->v_blank_f4[0]);
	hdmi_reg_writeb(hdata, HDMI_V_BLANK_F4_1, core->v_blank_f4[1]);
	hdmi_reg_writeb(hdata, HDMI_V_BLANK_F5_0, core->v_blank_f5[0]);
	hdmi_reg_writeb(hdata, HDMI_V_BLANK_F5_1, core->v_blank_f5[1]);
	hdmi_reg_writeb(hdata, HDMI_V_SYNC_LINE_AFT_3_0,
			core->v_sync_line_aft_3[0]);
	hdmi_reg_writeb(hdata, HDMI_V_SYNC_LINE_AFT_3_1,
			core->v_sync_line_aft_3[1]);
	hdmi_reg_writeb(hdata, HDMI_V_SYNC_LINE_AFT_4_0,
			core->v_sync_line_aft_4[0]);
	hdmi_reg_writeb(hdata, HDMI_V_SYNC_LINE_AFT_4_1,
			core->v_sync_line_aft_4[1]);
	hdmi_reg_writeb(hdata, HDMI_V_SYNC_LINE_AFT_5_0,
			core->v_sync_line_aft_5[0]);
	hdmi_reg_writeb(hdata, HDMI_V_SYNC_LINE_AFT_5_1,
			core->v_sync_line_aft_5[1]);
	hdmi_reg_writeb(hdata, HDMI_V_SYNC_LINE_AFT_6_0,
			core->v_sync_line_aft_6[0]);
	hdmi_reg_writeb(hdata, HDMI_V_SYNC_LINE_AFT_6_1,
			core->v_sync_line_aft_6[1]);
	hdmi_reg_writeb(hdata, HDMI_V_SYNC_LINE_AFT_PXL_3_0,
			core->v_sync_line_aft_pxl_3[0]);
	hdmi_reg_writeb(hdata, HDMI_V_SYNC_LINE_AFT_PXL_3_1,
			core->v_sync_line_aft_pxl_3[1]);
	hdmi_reg_writeb(hdata, HDMI_V_SYNC_LINE_AFT_PXL_4_0,
			core->v_sync_line_aft_pxl_4[0]);
	hdmi_reg_writeb(hdata, HDMI_V_SYNC_LINE_AFT_PXL_4_1,
			core->v_sync_line_aft_pxl_4[1]);
	hdmi_reg_writeb(hdata, HDMI_V_SYNC_LINE_AFT_PXL_5_0,
			core->v_sync_line_aft_pxl_5[0]);
	hdmi_reg_writeb(hdata, HDMI_V_SYNC_LINE_AFT_PXL_5_1,
			core->v_sync_line_aft_pxl_5[1]);
	hdmi_reg_writeb(hdata, HDMI_V_SYNC_LINE_AFT_PXL_6_0,
			core->v_sync_line_aft_pxl_6[0]);
	hdmi_reg_writeb(hdata, HDMI_V_SYNC_LINE_AFT_PXL_6_1,
			core->v_sync_line_aft_pxl_6[1]);
	hdmi_reg_writeb(hdata, HDMI_VACT_SPACE_1_0, core->vact_space_1[0]);
	hdmi_reg_writeb(hdata, HDMI_VACT_SPACE_1_1, core->vact_space_1[1]);
	hdmi_reg_writeb(hdata, HDMI_VACT_SPACE_2_0, core->vact_space_2[0]);
	hdmi_reg_writeb(hdata, HDMI_VACT_SPACE_2_1, core->vact_space_2[1]);
	hdmi_reg_writeb(hdata, HDMI_VACT_SPACE_3_0, core->vact_space_3[0]);
	hdmi_reg_writeb(hdata, HDMI_VACT_SPACE_3_1, core->vact_space_3[1]);
	hdmi_reg_writeb(hdata, HDMI_VACT_SPACE_4_0, core->vact_space_4[0]);
	hdmi_reg_writeb(hdata, HDMI_VACT_SPACE_4_1, core->vact_space_4[1]);
	hdmi_reg_writeb(hdata, HDMI_VACT_SPACE_5_0, core->vact_space_5[0]);
	hdmi_reg_writeb(hdata, HDMI_VACT_SPACE_5_1, core->vact_space_5[1]);
	hdmi_reg_writeb(hdata, HDMI_VACT_SPACE_6_0, core->vact_space_6[0]);
	hdmi_reg_writeb(hdata, HDMI_VACT_SPACE_6_1, core->vact_space_6[1]);

	/* Timing generator registers */
	hdmi_reg_writeb(hdata, HDMI_TG_H_FSZ_L, tg->h_fsz_l);
	hdmi_reg_writeb(hdata, HDMI_TG_H_FSZ_H, tg->h_fsz_h);
	hdmi_reg_writeb(hdata, HDMI_TG_HACT_ST_L, tg->hact_st_l);
	hdmi_reg_writeb(hdata, HDMI_TG_HACT_ST_H, tg->hact_st_h);
	hdmi_reg_writeb(hdata, HDMI_TG_HACT_SZ_L, tg->hact_sz_l);
	hdmi_reg_writeb(hdata, HDMI_TG_HACT_SZ_H, tg->hact_sz_h);
	hdmi_reg_writeb(hdata, HDMI_TG_V_FSZ_L, tg->v_fsz_l);
	hdmi_reg_writeb(hdata, HDMI_TG_V_FSZ_H, tg->v_fsz_h);
	hdmi_reg_writeb(hdata, HDMI_TG_VSYNC_L, tg->vsync_l);
	hdmi_reg_writeb(hdata, HDMI_TG_VSYNC_H, tg->vsync_h);
	hdmi_reg_writeb(hdata, HDMI_TG_VSYNC2_L, tg->vsync2_l);
	hdmi_reg_writeb(hdata, HDMI_TG_VSYNC2_H, tg->vsync2_h);
	hdmi_reg_writeb(hdata, HDMI_TG_VACT_ST_L, tg->vact_st_l);
	hdmi_reg_writeb(hdata, HDMI_TG_VACT_ST_H, tg->vact_st_h);
	hdmi_reg_writeb(hdata, HDMI_TG_VACT_SZ_L, tg->vact_sz_l);
	hdmi_reg_writeb(hdata, HDMI_TG_VACT_SZ_H, tg->vact_sz_h);
	hdmi_reg_writeb(hdata, HDMI_TG_FIELD_CHG_L, tg->field_chg_l);
	hdmi_reg_writeb(hdata, HDMI_TG_FIELD_CHG_H, tg->field_chg_h);
	hdmi_reg_writeb(hdata, HDMI_TG_VACT_ST2_L, tg->vact_st2_l);
	hdmi_reg_writeb(hdata, HDMI_TG_VACT_ST2_H, tg->vact_st2_h);
	hdmi_reg_writeb(hdata, HDMI_TG_VACT_ST3_L, tg->vact_st3_l);
	hdmi_reg_writeb(hdata, HDMI_TG_VACT_ST3_H, tg->vact_st3_h);
	hdmi_reg_writeb(hdata, HDMI_TG_VACT_ST4_L, tg->vact_st4_l);
	hdmi_reg_writeb(hdata, HDMI_TG_VACT_ST4_H, tg->vact_st4_h);
	hdmi_reg_writeb(hdata, HDMI_TG_VSYNC_TOP_HDMI_L, tg->vsync_top_hdmi_l);
	hdmi_reg_writeb(hdata, HDMI_TG_VSYNC_TOP_HDMI_H, tg->vsync_top_hdmi_h);
	hdmi_reg_writeb(hdata, HDMI_TG_VSYNC_BOT_HDMI_L, tg->vsync_bot_hdmi_l);
	hdmi_reg_writeb(hdata, HDMI_TG_VSYNC_BOT_HDMI_H, tg->vsync_bot_hdmi_h);
	hdmi_reg_writeb(hdata, HDMI_TG_FIELD_TOP_HDMI_L, tg->field_top_hdmi_l);
	hdmi_reg_writeb(hdata, HDMI_TG_FIELD_TOP_HDMI_H, tg->field_top_hdmi_h);
	hdmi_reg_writeb(hdata, HDMI_TG_FIELD_BOT_HDMI_L, tg->field_bot_hdmi_l);
	hdmi_reg_writeb(hdata, HDMI_TG_FIELD_BOT_HDMI_H, tg->field_bot_hdmi_h);
	hdmi_reg_writeb(hdata, HDMI_TG_3D, tg->tg_3d);

	/* waiting for HDMIPHY's PLL to get to steady state */
	for (tries = 100; tries; --tries) {
		u32 val = hdmi_reg_read(hdata, HDMI_PHY_STATUS_0);
		if (val & HDMI_PHY_STATUS_READY)
			break;
		mdelay(1);
	}
	/* steady state not achieved */
	if (tries == 0) {
		DRM_ERROR("hdmiphy's pll could not reach steady state.\n");
		hdmi_regs_dump(hdata, "timing apply");
	}

	clk_disable(hdata->res.sclk_hdmi);
	clk_set_parent(hdata->res.sclk_hdmi, hdata->res.sclk_hdmiphy);
	clk_enable(hdata->res.sclk_hdmi);

	/* enable HDMI and timing generator */
	hdmi_reg_writemask(hdata, HDMI_CON_0, ~0, HDMI_EN);
	if (core->int_pro_mode[0])
		hdmi_reg_writemask(hdata, HDMI_TG_CMD, ~0, HDMI_TG_EN |
				HDMI_FIELD_EN);
	else
		hdmi_reg_writemask(hdata, HDMI_TG_CMD, ~0, HDMI_TG_EN);
}

static void hdmi_timing_apply(struct hdmi_context *hdata)
{
	if (hdata->is_v13)
		hdmi_v13_timing_apply(hdata);
	else
		hdmi_v14_timing_apply(hdata);
=======
		hdmi_reg_infoframes(hdata);

		/* enable AVI packet every vsync, fixes purple line problem */
		hdmi_reg_writemask(hdata, HDMI_CON_1, 2, 3 << 5);
	}
}

static void hdmiphy_wait_for_pll(struct hdmi_context *hdata)
{
	int tries;

	for (tries = 0; tries < 10; ++tries) {
		u32 val = hdmi_reg_read(hdata, HDMI_PHY_STATUS);

		if (val & HDMI_PHY_STATUS_READY) {
			DRM_DEV_DEBUG_KMS(hdata->dev,
					  "PLL stabilized after %d tries\n",
					  tries);
			return;
		}
		usleep_range(10, 20);
	}

	DRM_DEV_ERROR(hdata->dev, "PLL could not reach steady state\n");
}

static void hdmi_v13_mode_apply(struct hdmi_context *hdata)
{
	struct drm_display_mode *m = &hdata->encoder.crtc->state->mode;
	unsigned int val;

	hdmi_reg_writev(hdata, HDMI_H_BLANK_0, 2, m->htotal - m->hdisplay);
	hdmi_reg_writev(hdata, HDMI_V13_H_V_LINE_0, 3,
			(m->htotal << 12) | m->vtotal);

	val = (m->flags & DRM_MODE_FLAG_NVSYNC) ? 1 : 0;
	hdmi_reg_writev(hdata, HDMI_VSYNC_POL, 1, val);

	val = (m->flags & DRM_MODE_FLAG_INTERLACE) ? 1 : 0;
	hdmi_reg_writev(hdata, HDMI_INT_PRO_MODE, 1, val);

	val = (m->hsync_start - m->hdisplay - 2);
	val |= ((m->hsync_end - m->hdisplay - 2) << 10);
	val |= ((m->flags & DRM_MODE_FLAG_NHSYNC) ? 1 : 0)<<20;
	hdmi_reg_writev(hdata, HDMI_V13_H_SYNC_GEN_0, 3, val);

	/*
	 * Quirk requirement for exynos HDMI IP design,
	 * 2 pixels less than the actual calculation for hsync_start
	 * and end.
	 */

	/* Following values & calculations differ for different type of modes */
	if (m->flags & DRM_MODE_FLAG_INTERLACE) {
		val = ((m->vsync_end - m->vdisplay) / 2);
		val |= ((m->vsync_start - m->vdisplay) / 2) << 12;
		hdmi_reg_writev(hdata, HDMI_V13_V_SYNC_GEN_1_0, 3, val);

		val = m->vtotal / 2;
		val |= ((m->vtotal - m->vdisplay) / 2) << 11;
		hdmi_reg_writev(hdata, HDMI_V13_V_BLANK_0, 3, val);

		val = (m->vtotal +
			((m->vsync_end - m->vsync_start) * 4) + 5) / 2;
		val |= m->vtotal << 11;
		hdmi_reg_writev(hdata, HDMI_V13_V_BLANK_F_0, 3, val);

		val = ((m->vtotal / 2) + 7);
		val |= ((m->vtotal / 2) + 2) << 12;
		hdmi_reg_writev(hdata, HDMI_V13_V_SYNC_GEN_2_0, 3, val);

		val = ((m->htotal / 2) + (m->hsync_start - m->hdisplay));
		val |= ((m->htotal / 2) +
			(m->hsync_start - m->hdisplay)) << 12;
		hdmi_reg_writev(hdata, HDMI_V13_V_SYNC_GEN_3_0, 3, val);

		hdmi_reg_writev(hdata, HDMI_TG_VACT_ST_L, 2,
				(m->vtotal - m->vdisplay) / 2);
		hdmi_reg_writev(hdata, HDMI_TG_VACT_SZ_L, 2, m->vdisplay / 2);

		hdmi_reg_writev(hdata, HDMI_TG_VACT_ST2_L, 2, 0x249);
	} else {
		val = m->vtotal;
		val |= (m->vtotal - m->vdisplay) << 11;
		hdmi_reg_writev(hdata, HDMI_V13_V_BLANK_0, 3, val);

		hdmi_reg_writev(hdata, HDMI_V13_V_BLANK_F_0, 3, 0);

		val = (m->vsync_end - m->vdisplay);
		val |= ((m->vsync_start - m->vdisplay) << 12);
		hdmi_reg_writev(hdata, HDMI_V13_V_SYNC_GEN_1_0, 3, val);

		hdmi_reg_writev(hdata, HDMI_V13_V_SYNC_GEN_2_0, 3, 0x1001);
		hdmi_reg_writev(hdata, HDMI_V13_V_SYNC_GEN_3_0, 3, 0x1001);
		hdmi_reg_writev(hdata, HDMI_TG_VACT_ST_L, 2,
				m->vtotal - m->vdisplay);
		hdmi_reg_writev(hdata, HDMI_TG_VACT_SZ_L, 2, m->vdisplay);
	}

	hdmi_reg_writev(hdata, HDMI_TG_H_FSZ_L, 2, m->htotal);
	hdmi_reg_writev(hdata, HDMI_TG_HACT_ST_L, 2, m->htotal - m->hdisplay);
	hdmi_reg_writev(hdata, HDMI_TG_HACT_SZ_L, 2, m->hdisplay);
	hdmi_reg_writev(hdata, HDMI_TG_V_FSZ_L, 2, m->vtotal);
}

static void hdmi_v14_mode_apply(struct hdmi_context *hdata)
{
	struct drm_display_mode *m = &hdata->encoder.crtc->state->mode;
	struct drm_display_mode *am =
				&hdata->encoder.crtc->state->adjusted_mode;
	int hquirk = 0;

	/*
	 * In case video mode coming from CRTC differs from requested one HDMI
	 * sometimes is able to almost properly perform conversion - only
	 * first line is distorted.
	 */
	if ((m->vdisplay != am->vdisplay) &&
	    (m->hdisplay == 1280 || m->hdisplay == 1024 || m->hdisplay == 1366))
		hquirk = 258;

	hdmi_reg_writev(hdata, HDMI_H_BLANK_0, 2, m->htotal - m->hdisplay);
	hdmi_reg_writev(hdata, HDMI_V_LINE_0, 2, m->vtotal);
	hdmi_reg_writev(hdata, HDMI_H_LINE_0, 2, m->htotal);
	hdmi_reg_writev(hdata, HDMI_HSYNC_POL, 1,
			(m->flags & DRM_MODE_FLAG_NHSYNC) ? 1 : 0);
	hdmi_reg_writev(hdata, HDMI_VSYNC_POL, 1,
			(m->flags & DRM_MODE_FLAG_NVSYNC) ? 1 : 0);
	hdmi_reg_writev(hdata, HDMI_INT_PRO_MODE, 1,
			(m->flags & DRM_MODE_FLAG_INTERLACE) ? 1 : 0);

	/*
	 * Quirk requirement for exynos 5 HDMI IP design,
	 * 2 pixels less than the actual calculation for hsync_start
	 * and end.
	 */

	/* Following values & calculations differ for different type of modes */
	if (m->flags & DRM_MODE_FLAG_INTERLACE) {
		hdmi_reg_writev(hdata, HDMI_V_SYNC_LINE_BEF_2_0, 2,
			(m->vsync_end - m->vdisplay) / 2);
		hdmi_reg_writev(hdata, HDMI_V_SYNC_LINE_BEF_1_0, 2,
			(m->vsync_start - m->vdisplay) / 2);
		hdmi_reg_writev(hdata, HDMI_V2_BLANK_0, 2, m->vtotal / 2);
		hdmi_reg_writev(hdata, HDMI_V1_BLANK_0, 2,
				(m->vtotal - m->vdisplay) / 2);
		hdmi_reg_writev(hdata, HDMI_V_BLANK_F0_0, 2,
				m->vtotal - m->vdisplay / 2);
		hdmi_reg_writev(hdata, HDMI_V_BLANK_F1_0, 2, m->vtotal);
		hdmi_reg_writev(hdata, HDMI_V_SYNC_LINE_AFT_2_0, 2,
				(m->vtotal / 2) + 7);
		hdmi_reg_writev(hdata, HDMI_V_SYNC_LINE_AFT_1_0, 2,
				(m->vtotal / 2) + 2);
		hdmi_reg_writev(hdata, HDMI_V_SYNC_LINE_AFT_PXL_2_0, 2,
			(m->htotal / 2) + (m->hsync_start - m->hdisplay));
		hdmi_reg_writev(hdata, HDMI_V_SYNC_LINE_AFT_PXL_1_0, 2,
			(m->htotal / 2) + (m->hsync_start - m->hdisplay));
		hdmi_reg_writev(hdata, HDMI_TG_VACT_ST_L, 2,
				(m->vtotal - m->vdisplay) / 2);
		hdmi_reg_writev(hdata, HDMI_TG_VACT_SZ_L, 2, m->vdisplay / 2);
		hdmi_reg_writev(hdata, HDMI_TG_VACT_ST2_L, 2,
				m->vtotal - m->vdisplay / 2);
		hdmi_reg_writev(hdata, HDMI_TG_VSYNC2_L, 2,
				(m->vtotal / 2) + 1);
		hdmi_reg_writev(hdata, HDMI_TG_VSYNC_BOT_HDMI_L, 2,
				(m->vtotal / 2) + 1);
		hdmi_reg_writev(hdata, HDMI_TG_FIELD_BOT_HDMI_L, 2,
				(m->vtotal / 2) + 1);
		hdmi_reg_writev(hdata, HDMI_TG_VACT_ST3_L, 2, 0x0);
		hdmi_reg_writev(hdata, HDMI_TG_VACT_ST4_L, 2, 0x0);
	} else {
		hdmi_reg_writev(hdata, HDMI_V_SYNC_LINE_BEF_2_0, 2,
			m->vsync_end - m->vdisplay);
		hdmi_reg_writev(hdata, HDMI_V_SYNC_LINE_BEF_1_0, 2,
			m->vsync_start - m->vdisplay);
		hdmi_reg_writev(hdata, HDMI_V2_BLANK_0, 2, m->vtotal);
		hdmi_reg_writev(hdata, HDMI_V1_BLANK_0, 2,
				m->vtotal - m->vdisplay);
		hdmi_reg_writev(hdata, HDMI_V_BLANK_F0_0, 2, 0xffff);
		hdmi_reg_writev(hdata, HDMI_V_BLANK_F1_0, 2, 0xffff);
		hdmi_reg_writev(hdata, HDMI_V_SYNC_LINE_AFT_2_0, 2, 0xffff);
		hdmi_reg_writev(hdata, HDMI_V_SYNC_LINE_AFT_1_0, 2, 0xffff);
		hdmi_reg_writev(hdata, HDMI_V_SYNC_LINE_AFT_PXL_2_0, 2, 0xffff);
		hdmi_reg_writev(hdata, HDMI_V_SYNC_LINE_AFT_PXL_1_0, 2, 0xffff);
		hdmi_reg_writev(hdata, HDMI_TG_VACT_ST_L, 2,
				m->vtotal - m->vdisplay);
		hdmi_reg_writev(hdata, HDMI_TG_VACT_SZ_L, 2, m->vdisplay);
	}

	hdmi_reg_writev(hdata, HDMI_H_SYNC_START_0, 2,
			m->hsync_start - m->hdisplay - 2);
	hdmi_reg_writev(hdata, HDMI_H_SYNC_END_0, 2,
			m->hsync_end - m->hdisplay - 2);
	hdmi_reg_writev(hdata, HDMI_VACT_SPACE_1_0, 2, 0xffff);
	hdmi_reg_writev(hdata, HDMI_VACT_SPACE_2_0, 2, 0xffff);
	hdmi_reg_writev(hdata, HDMI_VACT_SPACE_3_0, 2, 0xffff);
	hdmi_reg_writev(hdata, HDMI_VACT_SPACE_4_0, 2, 0xffff);
	hdmi_reg_writev(hdata, HDMI_VACT_SPACE_5_0, 2, 0xffff);
	hdmi_reg_writev(hdata, HDMI_VACT_SPACE_6_0, 2, 0xffff);
	hdmi_reg_writev(hdata, HDMI_V_BLANK_F2_0, 2, 0xffff);
	hdmi_reg_writev(hdata, HDMI_V_BLANK_F3_0, 2, 0xffff);
	hdmi_reg_writev(hdata, HDMI_V_BLANK_F4_0, 2, 0xffff);
	hdmi_reg_writev(hdata, HDMI_V_BLANK_F5_0, 2, 0xffff);
	hdmi_reg_writev(hdata, HDMI_V_SYNC_LINE_AFT_3_0, 2, 0xffff);
	hdmi_reg_writev(hdata, HDMI_V_SYNC_LINE_AFT_4_0, 2, 0xffff);
	hdmi_reg_writev(hdata, HDMI_V_SYNC_LINE_AFT_5_0, 2, 0xffff);
	hdmi_reg_writev(hdata, HDMI_V_SYNC_LINE_AFT_6_0, 2, 0xffff);
	hdmi_reg_writev(hdata, HDMI_V_SYNC_LINE_AFT_PXL_3_0, 2, 0xffff);
	hdmi_reg_writev(hdata, HDMI_V_SYNC_LINE_AFT_PXL_4_0, 2, 0xffff);
	hdmi_reg_writev(hdata, HDMI_V_SYNC_LINE_AFT_PXL_5_0, 2, 0xffff);
	hdmi_reg_writev(hdata, HDMI_V_SYNC_LINE_AFT_PXL_6_0, 2, 0xffff);

	hdmi_reg_writev(hdata, HDMI_TG_H_FSZ_L, 2, m->htotal);
	hdmi_reg_writev(hdata, HDMI_TG_HACT_ST_L, 2,
					m->htotal - m->hdisplay - hquirk);
	hdmi_reg_writev(hdata, HDMI_TG_HACT_SZ_L, 2, m->hdisplay + hquirk);
	hdmi_reg_writev(hdata, HDMI_TG_V_FSZ_L, 2, m->vtotal);
	if (hdata->drv_data == &exynos5433_hdmi_driver_data)
		hdmi_reg_writeb(hdata, HDMI_TG_DECON_EN, 1);
}

static void hdmi_mode_apply(struct hdmi_context *hdata)
{
	if (hdata->drv_data->type == HDMI_TYPE13)
		hdmi_v13_mode_apply(hdata);
	else
		hdmi_v14_mode_apply(hdata);

	hdmi_start(hdata, true);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

static void hdmiphy_conf_reset(struct hdmi_context *hdata)
{
<<<<<<< HEAD
	u8 buffer[2];
	u32 reg;

	clk_disable(hdata->res.sclk_hdmi);
	clk_set_parent(hdata->res.sclk_hdmi, hdata->res.sclk_pixel);
	clk_enable(hdata->res.sclk_hdmi);

	/* operation mode */
	buffer[0] = 0x1f;
	buffer[1] = 0x00;

	if (hdata->hdmiphy_port)
		i2c_master_send(hdata->hdmiphy_port, buffer, 2);

	if (hdata->is_v13)
		reg = HDMI_V13_PHY_RSTOUT;
	else
		reg = HDMI_PHY_RSTOUT;

	/* reset hdmiphy */
	hdmi_reg_writemask(hdata, reg, ~0, HDMI_PHY_SW_RSTOUT);
	mdelay(10);
	hdmi_reg_writemask(hdata, reg,  0, HDMI_PHY_SW_RSTOUT);
	mdelay(10);
=======
	hdmi_reg_writemask(hdata, HDMI_CORE_RSTOUT, 0, 1);
	usleep_range(10000, 12000);
	hdmi_reg_writemask(hdata, HDMI_CORE_RSTOUT, ~0, 1);
	usleep_range(10000, 12000);
	hdmi_reg_writemask(hdata, HDMI_PHY_RSTOUT, ~0, HDMI_PHY_SW_RSTOUT);
	usleep_range(10000, 12000);
	hdmi_reg_writemask(hdata, HDMI_PHY_RSTOUT, 0, HDMI_PHY_SW_RSTOUT);
	usleep_range(10000, 12000);
}

static void hdmiphy_enable_mode_set(struct hdmi_context *hdata, bool enable)
{
	u8 v = enable ? HDMI_PHY_ENABLE_MODE_SET : HDMI_PHY_DISABLE_MODE_SET;

	if (hdata->drv_data == &exynos5433_hdmi_driver_data)
		writel(v, hdata->regs_hdmiphy + HDMIPHY5433_MODE_SET_DONE);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

static void hdmiphy_conf_apply(struct hdmi_context *hdata)
{
<<<<<<< HEAD
	const u8 *hdmiphy_data;
	u8 buffer[32];
	u8 operation[2];
	u8 read_buffer[32] = {0, };
	int ret;
	int i;

	if (!hdata->hdmiphy_port) {
		DRM_ERROR("hdmiphy is not attached\n");
		return;
	}

	/* pixel clock */
	if (hdata->is_v13)
		hdmiphy_data = hdmi_v13_confs[hdata->cur_conf].hdmiphy_data;
	else
		hdmiphy_data = hdmi_confs[hdata->cur_conf].hdmiphy_data;

	memcpy(buffer, hdmiphy_data, 32);
	ret = i2c_master_send(hdata->hdmiphy_port, buffer, 32);
	if (ret != 32) {
		DRM_ERROR("failed to configure HDMIPHY via I2C\n");
		return;
	}

	mdelay(10);

	/* operation mode */
	operation[0] = 0x1f;
	operation[1] = 0x80;

	ret = i2c_master_send(hdata->hdmiphy_port, operation, 2);
	if (ret != 2) {
		DRM_ERROR("failed to enable hdmiphy\n");
		return;
	}

	ret = i2c_master_recv(hdata->hdmiphy_port, read_buffer, 32);
	if (ret < 0) {
		DRM_ERROR("failed to read hdmiphy config\n");
		return;
	}

	for (i = 0; i < ret; i++)
		DRM_DEBUG_KMS("hdmiphy[0x%02x] write[0x%02x] - "
			"recv [0x%02x]\n", i, buffer[i], read_buffer[i]);
}

static void hdmi_conf_apply(struct hdmi_context *hdata)
{
	DRM_DEBUG_KMS("[%d] %s\n", __LINE__, __func__);

	hdmiphy_conf_reset(hdata);
	hdmiphy_conf_apply(hdata);

	hdmi_conf_reset(hdata);
	hdmi_conf_init(hdata);
	hdmi_audio_init(hdata);

	/* setting core registers */
	hdmi_timing_apply(hdata);
	hdmi_audio_control(hdata, true);

	hdmi_regs_dump(hdata, "start");
}

static void hdmi_mode_fixup(void *ctx, struct drm_connector *connector,
				struct drm_display_mode *mode,
				struct drm_display_mode *adjusted_mode)
{
	struct drm_display_mode *m;
	struct hdmi_context *hdata = ctx;
	int index;

	DRM_DEBUG_KMS("[%d] %s\n", __LINE__, __func__);

	drm_mode_set_crtcinfo(adjusted_mode, 0);

	if (hdata->is_v13)
		index = hdmi_v13_conf_index(adjusted_mode);
	else
		index = hdmi_v14_conf_index(adjusted_mode);

	/* just return if user desired mode exists. */
	if (index >= 0)
		return;

	/*
	 * otherwise, find the most suitable mode among modes and change it
	 * to adjusted_mode.
	 */
	list_for_each_entry(m, &connector->modes, head) {
		if (hdata->is_v13)
			index = hdmi_v13_conf_index(m);
		else
			index = hdmi_v14_conf_index(m);

		if (index >= 0) {
			DRM_INFO("desired mode doesn't exist so\n");
			DRM_INFO("use the most suitable mode among modes.\n");
			memcpy(adjusted_mode, m, sizeof(*m));
			break;
		}
	}
}

static void hdmi_mode_set(void *ctx, void *mode)
{
	struct hdmi_context *hdata = ctx;
	int conf_idx;

	DRM_DEBUG_KMS("[%d] %s\n", __LINE__, __func__);

	conf_idx = hdmi_conf_index(hdata, mode);
	if (conf_idx >= 0)
		hdata->cur_conf = conf_idx;
	else
		DRM_DEBUG_KMS("not supported mode\n");
}

static void hdmi_get_max_resol(void *ctx, unsigned int *width,
					unsigned int *height)
{
	DRM_DEBUG_KMS("[%d] %s\n", __LINE__, __func__);

	*width = MAX_WIDTH;
	*height = MAX_HEIGHT;
}

static void hdmi_commit(void *ctx)
{
	struct hdmi_context *hdata = ctx;

	DRM_DEBUG_KMS("[%d] %s\n", __LINE__, __func__);

	hdmi_conf_apply(hdata);

	hdata->enabled = true;
}

static void hdmi_disable(void *ctx)
{
	struct hdmi_context *hdata = ctx;

	DRM_DEBUG_KMS("[%d] %s\n", __LINE__, __func__);

	if (hdata->enabled) {
		hdmi_audio_control(hdata, false);
		hdmiphy_conf_reset(hdata);
		hdmi_conf_reset(hdata);
	}
}

static struct exynos_hdmi_ops hdmi_ops = {
	/* display */
	.is_connected	= hdmi_is_connected,
	.get_edid	= hdmi_get_edid,
	.check_timing	= hdmi_check_timing,
	.power_on	= hdmi_display_power_on,

	/* manager */
	.mode_fixup	= hdmi_mode_fixup,
	.mode_set	= hdmi_mode_set,
	.get_max_resol	= hdmi_get_max_resol,
	.commit		= hdmi_commit,
	.disable	= hdmi_disable,
};

/*
 * Handle hotplug events outside the interrupt handler proper.
 */
static void hdmi_hotplug_func(struct work_struct *work)
{
	struct hdmi_context *hdata =
		container_of(work, struct hdmi_context, hotplug_work);
	struct exynos_drm_hdmi_context *ctx =
		(struct exynos_drm_hdmi_context *)hdata->parent_ctx;

	drm_helper_hpd_irq_event(ctx->drm_dev);
}

static irqreturn_t hdmi_irq_handler(int irq, void *arg)
{
	struct exynos_drm_hdmi_context *ctx = arg;
	struct hdmi_context *hdata = ctx->ctx;
	u32 intc_flag;

	intc_flag = hdmi_reg_read(hdata, HDMI_INTC_FLAG);
	/* clearing flags for HPD plug/unplug */
	if (intc_flag & HDMI_INTC_FLAG_HPD_UNPLUG) {
		DRM_DEBUG_KMS("unplugged, handling:%d\n", hdata->hpd_handle);
		hdmi_reg_writemask(hdata, HDMI_INTC_FLAG, ~0,
			HDMI_INTC_FLAG_HPD_UNPLUG);
	}
	if (intc_flag & HDMI_INTC_FLAG_HPD_PLUG) {
		DRM_DEBUG_KMS("plugged, handling:%d\n", hdata->hpd_handle);
		hdmi_reg_writemask(hdata, HDMI_INTC_FLAG, ~0,
			HDMI_INTC_FLAG_HPD_PLUG);
	}

	if (ctx->drm_dev && hdata->hpd_handle)
		queue_work(hdata->wq, &hdata->hotplug_work);
=======
	struct drm_display_mode *m = &hdata->encoder.crtc->state->mode;
	int ret;
	const u8 *phy_conf;

	ret = hdmi_find_phy_conf(hdata, m->clock * 1000);
	if (ret < 0) {
		DRM_DEV_ERROR(hdata->dev, "failed to find hdmiphy conf\n");
		return;
	}
	phy_conf = hdata->drv_data->phy_confs.data[ret].conf;

	hdmi_clk_set_parents(hdata, false);

	hdmiphy_conf_reset(hdata);

	hdmiphy_enable_mode_set(hdata, true);
	ret = hdmiphy_reg_write_buf(hdata, 0, phy_conf, 32);
	if (ret) {
		DRM_DEV_ERROR(hdata->dev, "failed to configure hdmiphy\n");
		return;
	}
	hdmiphy_enable_mode_set(hdata, false);
	hdmi_clk_set_parents(hdata, true);
	usleep_range(10000, 12000);
	hdmiphy_wait_for_pll(hdata);
}

/* Should be called with hdata->mutex mutex held */
static void hdmi_conf_apply(struct hdmi_context *hdata)
{
	hdmi_start(hdata, false);
	hdmi_conf_init(hdata);
	hdmi_audio_config(hdata);
	hdmi_mode_apply(hdata);
	hdmi_audio_control(hdata);
}

static void hdmi_set_refclk(struct hdmi_context *hdata, bool on)
{
	if (!hdata->sysreg)
		return;

	regmap_update_bits(hdata->sysreg, EXYNOS5433_SYSREG_DISP_HDMI_PHY,
			   SYSREG_HDMI_REFCLK_INT_CLK, on ? ~0 : 0);
}

/* Should be called with hdata->mutex mutex held. */
static void hdmiphy_enable(struct hdmi_context *hdata)
{
	int ret;

	if (hdata->powered)
		return;

	ret = pm_runtime_resume_and_get(hdata->dev);
	if (ret < 0) {
		dev_err(hdata->dev, "failed to enable HDMIPHY device.\n");
		return;
	}

	if (regulator_bulk_enable(ARRAY_SIZE(supply), hdata->regul_bulk))
		DRM_DEV_DEBUG_KMS(hdata->dev,
				  "failed to enable regulator bulk\n");

	regmap_update_bits(hdata->pmureg, PMU_HDMI_PHY_CONTROL,
			PMU_HDMI_PHY_ENABLE_BIT, 1);

	hdmi_set_refclk(hdata, true);

	hdmi_reg_writemask(hdata, HDMI_PHY_CON_0, 0, HDMI_PHY_POWER_OFF_EN);

	hdmiphy_conf_apply(hdata);

	hdata->powered = true;
}

/* Should be called with hdata->mutex mutex held. */
static void hdmiphy_disable(struct hdmi_context *hdata)
{
	if (!hdata->powered)
		return;

	hdmi_reg_writemask(hdata, HDMI_CON_0, 0, HDMI_EN);

	hdmi_reg_writemask(hdata, HDMI_PHY_CON_0, ~0, HDMI_PHY_POWER_OFF_EN);

	hdmi_set_refclk(hdata, false);

	regmap_update_bits(hdata->pmureg, PMU_HDMI_PHY_CONTROL,
			PMU_HDMI_PHY_ENABLE_BIT, 0);

	regulator_bulk_disable(ARRAY_SIZE(supply), hdata->regul_bulk);

	pm_runtime_put_sync(hdata->dev);

	hdata->powered = false;
}

static void hdmi_enable(struct drm_encoder *encoder)
{
	struct hdmi_context *hdata = encoder_to_hdmi(encoder);

	mutex_lock(&hdata->mutex);

	hdmiphy_enable(hdata);
	hdmi_conf_apply(hdata);

	mutex_unlock(&hdata->mutex);
}

static void hdmi_disable(struct drm_encoder *encoder)
{
	struct hdmi_context *hdata = encoder_to_hdmi(encoder);

	mutex_lock(&hdata->mutex);

	if (hdata->powered) {
		/*
		 * The SFRs of VP and Mixer are updated by Vertical Sync of
		 * Timing generator which is a part of HDMI so the sequence
		 * to disable TV Subsystem should be as following,
		 *	VP -> Mixer -> HDMI
		 *
		 * To achieve such sequence HDMI is disabled together with
		 * HDMI PHY, via pipe clock callback.
		 */
		mutex_unlock(&hdata->mutex);
		cancel_delayed_work(&hdata->hotplug_work);
		if (hdata->notifier)
			cec_notifier_phys_addr_invalidate(hdata->notifier);
		return;
	}

	mutex_unlock(&hdata->mutex);
}

static const struct drm_encoder_helper_funcs exynos_hdmi_encoder_helper_funcs = {
	.mode_fixup	= hdmi_mode_fixup,
	.enable		= hdmi_enable,
	.disable	= hdmi_disable,
};

static void hdmi_audio_shutdown(struct device *dev, void *data)
{
	struct hdmi_context *hdata = dev_get_drvdata(dev);

	mutex_lock(&hdata->mutex);

	hdata->audio.mute = true;

	if (hdata->powered)
		hdmi_audio_control(hdata);

	mutex_unlock(&hdata->mutex);
}

static int hdmi_audio_hw_params(struct device *dev, void *data,
				struct hdmi_codec_daifmt *daifmt,
				struct hdmi_codec_params *params)
{
	struct hdmi_context *hdata = dev_get_drvdata(dev);

	if (daifmt->fmt != HDMI_I2S || daifmt->bit_clk_inv ||
	    daifmt->frame_clk_inv || daifmt->bit_clk_provider ||
	    daifmt->frame_clk_provider) {
		dev_err(dev, "%s: Bad flags %d %d %d %d\n", __func__,
			daifmt->bit_clk_inv, daifmt->frame_clk_inv,
			daifmt->bit_clk_provider,
			daifmt->frame_clk_provider);
		return -EINVAL;
	}

	mutex_lock(&hdata->mutex);

	hdata->audio.params = *params;

	if (hdata->powered) {
		hdmi_audio_config(hdata);
		hdmi_audio_infoframe_apply(hdata);
	}

	mutex_unlock(&hdata->mutex);

	return 0;
}

static int hdmi_audio_mute(struct device *dev, void *data,
			   bool mute, int direction)
{
	struct hdmi_context *hdata = dev_get_drvdata(dev);

	mutex_lock(&hdata->mutex);

	hdata->audio.mute = mute;

	if (hdata->powered)
		hdmi_audio_control(hdata);

	mutex_unlock(&hdata->mutex);

	return 0;
}

static int hdmi_audio_get_eld(struct device *dev, void *data, uint8_t *buf,
			      size_t len)
{
	struct hdmi_context *hdata = dev_get_drvdata(dev);
	struct drm_connector *connector = &hdata->connector;

	memcpy(buf, connector->eld, min(sizeof(connector->eld), len));

	return 0;
}

static const struct hdmi_codec_ops audio_codec_ops = {
	.hw_params = hdmi_audio_hw_params,
	.audio_shutdown = hdmi_audio_shutdown,
	.mute_stream = hdmi_audio_mute,
	.get_eld = hdmi_audio_get_eld,
	.no_capture_mute = 1,
};

static int hdmi_register_audio_device(struct hdmi_context *hdata)
{
	struct hdmi_codec_pdata codec_data = {
		.ops = &audio_codec_ops,
		.max_i2s_channels = 6,
		.i2s = 1,
	};

	hdata->audio.pdev = platform_device_register_data(
		hdata->dev, HDMI_CODEC_DRV_NAME, PLATFORM_DEVID_AUTO,
		&codec_data, sizeof(codec_data));

	return PTR_ERR_OR_ZERO(hdata->audio.pdev);
}

static void hdmi_hotplug_work_func(struct work_struct *work)
{
	struct hdmi_context *hdata;

	hdata = container_of(work, struct hdmi_context, hotplug_work.work);

	if (hdata->drm_dev)
		drm_helper_hpd_irq_event(hdata->drm_dev);
}

static irqreturn_t hdmi_irq_thread(int irq, void *arg)
{
	struct hdmi_context *hdata = arg;

	mod_delayed_work(system_wq, &hdata->hotplug_work,
			msecs_to_jiffies(HOTPLUG_DEBOUNCE_MS));
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	return IRQ_HANDLED;
}

<<<<<<< HEAD
static int __devinit hdmi_resources_init(struct hdmi_context *hdata)
{
	struct device *dev = hdata->dev;
	struct hdmi_resources *res = &hdata->res;
	static char *supply[] = {
		"hdmi-en",
		"vdd",
		"vdd_osc",
		"vdd_pll",
	};
	int i, ret;

	DRM_DEBUG_KMS("HDMI resource init\n");

	memset(res, 0, sizeof *res);

	/* get clocks, power */
	res->hdmi = clk_get(dev, "hdmi");
	if (IS_ERR_OR_NULL(res->hdmi)) {
		DRM_ERROR("failed to get clock 'hdmi'\n");
		goto fail;
	}
	res->sclk_hdmi = clk_get(dev, "sclk_hdmi");
	if (IS_ERR_OR_NULL(res->sclk_hdmi)) {
		DRM_ERROR("failed to get clock 'sclk_hdmi'\n");
		goto fail;
	}
	res->sclk_pixel = clk_get(dev, "sclk_pixel");
	if (IS_ERR_OR_NULL(res->sclk_pixel)) {
		DRM_ERROR("failed to get clock 'sclk_pixel'\n");
		goto fail;
	}
	res->sclk_hdmiphy = clk_get(dev, "sclk_hdmiphy");
	if (IS_ERR_OR_NULL(res->sclk_hdmiphy)) {
		DRM_ERROR("failed to get clock 'sclk_hdmiphy'\n");
		goto fail;
	}
	res->hdmiphy = clk_get(dev, "hdmiphy");
	if (IS_ERR_OR_NULL(res->hdmiphy)) {
		DRM_ERROR("failed to get clock 'hdmiphy'\n");
		goto fail;
	}

	clk_set_parent(res->sclk_hdmi, res->sclk_pixel);

	res->regul_bulk = kzalloc(ARRAY_SIZE(supply) *
		sizeof res->regul_bulk[0], GFP_KERNEL);
	if (!res->regul_bulk) {
		DRM_ERROR("failed to get memory for regulators\n");
		goto fail;
	}
	for (i = 0; i < ARRAY_SIZE(supply); ++i) {
		res->regul_bulk[i].supply = supply[i];
		res->regul_bulk[i].consumer = NULL;
	}
	ret = regulator_bulk_get(dev, ARRAY_SIZE(supply), res->regul_bulk);
	if (ret) {
		DRM_ERROR("failed to get regulators\n");
		goto fail;
	}
	res->regul_count = ARRAY_SIZE(supply);

	return 0;
fail:
	DRM_ERROR("HDMI resource init - failed\n");
	return -ENODEV;
}

static int hdmi_resources_cleanup(struct hdmi_context *hdata)
{
	struct hdmi_resources *res = &hdata->res;

	regulator_bulk_free(res->regul_count, res->regul_bulk);
	/* kfree is NULL-safe */
	kfree(res->regul_bulk);
	if (!IS_ERR_OR_NULL(res->hdmiphy))
		clk_put(res->hdmiphy);
	if (!IS_ERR_OR_NULL(res->sclk_hdmiphy))
		clk_put(res->sclk_hdmiphy);
	if (!IS_ERR_OR_NULL(res->sclk_pixel))
		clk_put(res->sclk_pixel);
	if (!IS_ERR_OR_NULL(res->sclk_hdmi))
		clk_put(res->sclk_hdmi);
	if (!IS_ERR_OR_NULL(res->hdmi))
		clk_put(res->hdmi);
	memset(res, 0, sizeof *res);
=======
static int hdmi_clks_get(struct hdmi_context *hdata,
			 const struct string_array_spec *names,
			 struct clk **clks)
{
	struct device *dev = hdata->dev;
	int i;

	for (i = 0; i < names->count; ++i) {
		struct clk *clk = devm_clk_get(dev, names->data[i]);

		if (IS_ERR(clk)) {
			int ret = PTR_ERR(clk);

			dev_err(dev, "Cannot get clock %s, %d\n",
				names->data[i], ret);

			return ret;
		}

		clks[i] = clk;
	}
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	return 0;
}

<<<<<<< HEAD
static void hdmi_resource_poweron(struct hdmi_context *hdata)
{
	struct hdmi_resources *res = &hdata->res;

	DRM_DEBUG_KMS("[%d] %s\n", __LINE__, __func__);

	/* turn HDMI power on */
	regulator_bulk_enable(res->regul_count, res->regul_bulk);
	/* power-on hdmi physical interface */
	clk_enable(res->hdmiphy);
	/* turn clocks on */
	clk_enable(res->hdmi);
	clk_enable(res->sclk_hdmi);

	hdmiphy_conf_reset(hdata);
	hdmi_conf_reset(hdata);
	hdmi_conf_init(hdata);
	hdmi_audio_init(hdata);
}

static void hdmi_resource_poweroff(struct hdmi_context *hdata)
{
	struct hdmi_resources *res = &hdata->res;

	DRM_DEBUG_KMS("[%d] %s\n", __LINE__, __func__);

	/* turn clocks off */
	clk_disable(res->sclk_hdmi);
	clk_disable(res->hdmi);
	/* power-off hdmiphy */
	clk_disable(res->hdmiphy);
	/* turn HDMI power off */
	regulator_bulk_disable(res->regul_count, res->regul_bulk);
}

static int hdmi_runtime_suspend(struct device *dev)
{
	struct exynos_drm_hdmi_context *ctx = get_hdmi_context(dev);

	DRM_DEBUG_KMS("%s\n", __func__);

	hdmi_resource_poweroff(ctx->ctx);

	return 0;
}

static int hdmi_runtime_resume(struct device *dev)
{
	struct exynos_drm_hdmi_context *ctx = get_hdmi_context(dev);

	DRM_DEBUG_KMS("%s\n", __func__);

	hdmi_resource_poweron(ctx->ctx);

	return 0;
}

static const struct dev_pm_ops hdmi_pm_ops = {
	.runtime_suspend = hdmi_runtime_suspend,
	.runtime_resume	 = hdmi_runtime_resume,
};

static struct i2c_client *hdmi_ddc, *hdmi_hdmiphy;

void hdmi_attach_ddc_client(struct i2c_client *ddc)
{
	if (ddc)
		hdmi_ddc = ddc;
}

void hdmi_attach_hdmiphy_client(struct i2c_client *hdmiphy)
{
	if (hdmiphy)
		hdmi_hdmiphy = hdmiphy;
}

static int __devinit hdmi_probe(struct platform_device *pdev)
{
	struct device *dev = &pdev->dev;
	struct exynos_drm_hdmi_context *drm_hdmi_ctx;
	struct hdmi_context *hdata;
	struct exynos_drm_hdmi_pdata *pdata;
	struct resource *res;
	int ret;

	DRM_DEBUG_KMS("[%d]\n", __LINE__);

	pdata = pdev->dev.platform_data;
	if (!pdata) {
		DRM_ERROR("no platform data specified\n");
		return -EINVAL;
	}

	drm_hdmi_ctx = kzalloc(sizeof(*drm_hdmi_ctx), GFP_KERNEL);
	if (!drm_hdmi_ctx) {
		DRM_ERROR("failed to allocate common hdmi context.\n");
		return -ENOMEM;
	}

	hdata = kzalloc(sizeof(struct hdmi_context), GFP_KERNEL);
	if (!hdata) {
		DRM_ERROR("out of memory\n");
		kfree(drm_hdmi_ctx);
		return -ENOMEM;
	}

	drm_hdmi_ctx->ctx = (void *)hdata;
	hdata->parent_ctx = (void *)drm_hdmi_ctx;

	platform_set_drvdata(pdev, drm_hdmi_ctx);

	hdata->is_v13 = pdata->is_v13;
	hdata->default_win = pdata->default_win;
	hdata->default_timing = &pdata->timing;
	hdata->default_bpp = pdata->bpp;
	hdata->dev = dev;

	ret = hdmi_resources_init(hdata);
	if (ret) {
		ret = -EINVAL;
		goto err_data;
	}

	res = platform_get_resource(pdev, IORESOURCE_MEM, 0);
	if (!res) {
		DRM_ERROR("failed to find registers\n");
		ret = -ENOENT;
		goto err_resource;
	}

	hdata->regs_res = request_mem_region(res->start, resource_size(res),
					   dev_name(dev));
	if (!hdata->regs_res) {
		DRM_ERROR("failed to claim register region\n");
		ret = -ENOENT;
		goto err_resource;
	}

	hdata->regs = ioremap(res->start, resource_size(res));
	if (!hdata->regs) {
		DRM_ERROR("failed to map registers\n");
		ret = -ENXIO;
		goto err_req_region;
	}

	/* DDC i2c driver */
	if (i2c_add_driver(&ddc_driver)) {
		DRM_ERROR("failed to register ddc i2c driver\n");
		ret = -ENOENT;
		goto err_iomap;
	}

	hdata->ddc_port = hdmi_ddc;

	/* hdmiphy i2c driver */
	if (i2c_add_driver(&hdmiphy_driver)) {
		DRM_ERROR("failed to register hdmiphy i2c driver\n");
		ret = -ENOENT;
		goto err_ddc;
	}

	hdata->hdmiphy_port = hdmi_hdmiphy;

	res = platform_get_resource(pdev, IORESOURCE_IRQ, 0);
	if (res == NULL) {
		DRM_ERROR("get interrupt resource failed.\n");
		ret = -ENXIO;
		goto err_hdmiphy;
	}

	/* create workqueue and hotplug work */
	hdata->wq = alloc_workqueue("exynos-drm-hdmi",
			WQ_UNBOUND | WQ_NON_REENTRANT, 1);
	if (hdata->wq == NULL) {
		DRM_ERROR("Failed to create workqueue.\n");
		ret = -ENOMEM;
		goto err_hdmiphy;
	}
	INIT_WORK(&hdata->hotplug_work, hdmi_hotplug_func);

	/* register hpd interrupt */
	ret = request_irq(res->start, hdmi_irq_handler, 0, "drm_hdmi",
				drm_hdmi_ctx);
	if (ret) {
		DRM_ERROR("request interrupt failed.\n");
		goto err_workqueue;
	}
	hdata->irq = res->start;

	/* register specific callbacks to common hdmi. */
	exynos_hdmi_ops_register(&hdmi_ops);

	hdmi_resource_poweron(hdata);

	return 0;

err_workqueue:
	destroy_workqueue(hdata->wq);
err_hdmiphy:
	i2c_del_driver(&hdmiphy_driver);
err_ddc:
	i2c_del_driver(&ddc_driver);
err_iomap:
	iounmap(hdata->regs);
err_req_region:
	release_mem_region(hdata->regs_res->start,
			resource_size(hdata->regs_res));
err_resource:
	hdmi_resources_cleanup(hdata);
err_data:
	kfree(hdata);
	kfree(drm_hdmi_ctx);
	return ret;
}

static int __devexit hdmi_remove(struct platform_device *pdev)
{
	struct exynos_drm_hdmi_context *ctx = platform_get_drvdata(pdev);
	struct hdmi_context *hdata = ctx->ctx;

	DRM_DEBUG_KMS("[%d] %s\n", __LINE__, __func__);

	hdmi_resource_poweroff(hdata);

	disable_irq(hdata->irq);
	free_irq(hdata->irq, hdata);

	cancel_work_sync(&hdata->hotplug_work);
	destroy_workqueue(hdata->wq);

	hdmi_resources_cleanup(hdata);

	iounmap(hdata->regs);

	release_mem_region(hdata->regs_res->start,
			resource_size(hdata->regs_res));

	/* hdmiphy i2c driver */
	i2c_del_driver(&hdmiphy_driver);
	/* DDC i2c driver */
	i2c_del_driver(&ddc_driver);

	kfree(hdata);
=======
static int hdmi_clk_init(struct hdmi_context *hdata)
{
	const struct hdmi_driver_data *drv_data = hdata->drv_data;
	int count = drv_data->clk_gates.count + drv_data->clk_muxes.count;
	struct device *dev = hdata->dev;
	struct clk **clks;
	int ret;

	if (!count)
		return 0;

	clks = devm_kcalloc(dev, count, sizeof(*clks), GFP_KERNEL);
	if (!clks)
		return -ENOMEM;

	hdata->clk_gates = clks;
	hdata->clk_muxes = clks + drv_data->clk_gates.count;

	ret = hdmi_clks_get(hdata, &drv_data->clk_gates, hdata->clk_gates);
	if (ret)
		return ret;

	return hdmi_clks_get(hdata, &drv_data->clk_muxes, hdata->clk_muxes);
}


static void hdmiphy_clk_enable(struct exynos_drm_clk *clk, bool enable)
{
	struct hdmi_context *hdata = container_of(clk, struct hdmi_context,
						  phy_clk);
	mutex_lock(&hdata->mutex);

	if (enable)
		hdmiphy_enable(hdata);
	else
		hdmiphy_disable(hdata);

	mutex_unlock(&hdata->mutex);
}

static int hdmi_bridge_init(struct hdmi_context *hdata)
{
	struct device *dev = hdata->dev;
	struct device_node *ep, *np;

	ep = of_graph_get_endpoint_by_regs(dev->of_node, 1, -1);
	if (!ep)
		return 0;

	np = of_graph_get_remote_port_parent(ep);
	of_node_put(ep);
	if (!np) {
		DRM_DEV_ERROR(dev, "failed to get remote port parent");
		return -EINVAL;
	}

	hdata->bridge = of_drm_find_bridge(np);
	of_node_put(np);

	if (!hdata->bridge)
		return -EPROBE_DEFER;

	return 0;
}

static int hdmi_resources_init(struct hdmi_context *hdata)
{
	struct device *dev = hdata->dev;
	int i, ret;

	DRM_DEV_DEBUG_KMS(dev, "HDMI resource init\n");

	hdata->hpd_gpio = devm_gpiod_get(dev, "hpd", GPIOD_IN);
	if (IS_ERR(hdata->hpd_gpio)) {
		DRM_DEV_ERROR(dev, "cannot get hpd gpio property\n");
		return PTR_ERR(hdata->hpd_gpio);
	}

	hdata->irq = gpiod_to_irq(hdata->hpd_gpio);
	if (hdata->irq < 0) {
		DRM_DEV_ERROR(dev, "failed to get GPIO irq\n");
		return  hdata->irq;
	}

	ret = hdmi_clk_init(hdata);
	if (ret)
		return ret;

	ret = hdmi_clk_set_parents(hdata, false);
	if (ret)
		return ret;

	for (i = 0; i < ARRAY_SIZE(supply); ++i)
		hdata->regul_bulk[i].supply = supply[i];

	ret = devm_regulator_bulk_get(dev, ARRAY_SIZE(supply), hdata->regul_bulk);
	if (ret)
		return dev_err_probe(dev, ret, "failed to get regulators\n");

	hdata->reg_hdmi_en = devm_regulator_get_optional(dev, "hdmi-en");

	if (PTR_ERR(hdata->reg_hdmi_en) != -ENODEV)
		if (IS_ERR(hdata->reg_hdmi_en))
			return PTR_ERR(hdata->reg_hdmi_en);

	return hdmi_bridge_init(hdata);
}

static const struct of_device_id hdmi_match_types[] = {
	{
		.compatible = "samsung,exynos4210-hdmi",
		.data = &exynos4210_hdmi_driver_data,
	}, {
		.compatible = "samsung,exynos4212-hdmi",
		.data = &exynos4212_hdmi_driver_data,
	}, {
		.compatible = "samsung,exynos5420-hdmi",
		.data = &exynos5420_hdmi_driver_data,
	}, {
		.compatible = "samsung,exynos5433-hdmi",
		.data = &exynos5433_hdmi_driver_data,
	}, {
		/* end node */
	}
};
MODULE_DEVICE_TABLE (of, hdmi_match_types);

static int hdmi_bind(struct device *dev, struct device *master, void *data)
{
	struct drm_device *drm_dev = data;
	struct hdmi_context *hdata = dev_get_drvdata(dev);
	struct drm_encoder *encoder = &hdata->encoder;
	struct exynos_drm_crtc *crtc;
	int ret;

	hdata->drm_dev = drm_dev;

	hdata->phy_clk.enable = hdmiphy_clk_enable;

	drm_simple_encoder_init(drm_dev, encoder, DRM_MODE_ENCODER_TMDS);

	drm_encoder_helper_add(encoder, &exynos_hdmi_encoder_helper_funcs);

	ret = exynos_drm_set_possible_crtcs(encoder, EXYNOS_DISPLAY_TYPE_HDMI);
	if (ret < 0)
		return ret;

	crtc = exynos_drm_crtc_get_by_type(drm_dev, EXYNOS_DISPLAY_TYPE_HDMI);
	if (IS_ERR(crtc))
		return PTR_ERR(crtc);
	crtc->pipe_clk = &hdata->phy_clk;

	ret = hdmi_create_connector(encoder);
	if (ret) {
		DRM_DEV_ERROR(dev, "failed to create connector ret = %d\n",
			      ret);
		drm_encoder_cleanup(encoder);
		return ret;
	}

	return 0;
}

static void hdmi_unbind(struct device *dev, struct device *master, void *data)
{
}

static const struct component_ops hdmi_component_ops = {
	.bind	= hdmi_bind,
	.unbind = hdmi_unbind,
};

static int hdmi_get_ddc_adapter(struct hdmi_context *hdata)
{
	const char *compatible_str = "samsung,exynos4210-hdmiddc";
	struct device_node *np;
	struct i2c_adapter *adpt;

	np = of_find_compatible_node(NULL, NULL, compatible_str);
	if (np)
		np = of_get_next_parent(np);
	else
		np = of_parse_phandle(hdata->dev->of_node, "ddc", 0);

	if (!np) {
		DRM_DEV_ERROR(hdata->dev,
			      "Failed to find ddc node in device tree\n");
		return -ENODEV;
	}

	adpt = of_find_i2c_adapter_by_node(np);
	of_node_put(np);

	if (!adpt) {
		DRM_INFO("Failed to get ddc i2c adapter by node\n");
		return -EPROBE_DEFER;
	}

	hdata->ddc_adpt = adpt;

	return 0;
}

static int hdmi_get_phy_io(struct hdmi_context *hdata)
{
	const char *compatible_str = "samsung,exynos4212-hdmiphy";
	struct device_node *np;
	int ret = 0;

	np = of_find_compatible_node(NULL, NULL, compatible_str);
	if (!np) {
		np = of_parse_phandle(hdata->dev->of_node, "phy", 0);
		if (!np) {
			DRM_DEV_ERROR(hdata->dev,
				      "Failed to find hdmiphy node in device tree\n");
			return -ENODEV;
		}
	}

	if (hdata->drv_data->is_apb_phy) {
		hdata->regs_hdmiphy = of_iomap(np, 0);
		if (!hdata->regs_hdmiphy) {
			DRM_DEV_ERROR(hdata->dev,
				      "failed to ioremap hdmi phy\n");
			ret = -ENOMEM;
			goto out;
		}
	} else {
		hdata->hdmiphy_port = of_find_i2c_device_by_node(np);
		if (!hdata->hdmiphy_port) {
			DRM_INFO("Failed to get hdmi phy i2c client\n");
			ret = -EPROBE_DEFER;
			goto out;
		}
	}

out:
	of_node_put(np);
	return ret;
}

static int hdmi_probe(struct platform_device *pdev)
{
	struct hdmi_audio_infoframe *audio_infoframe;
	struct device *dev = &pdev->dev;
	struct hdmi_context *hdata;
	int ret;

	hdata = devm_kzalloc(dev, sizeof(struct hdmi_context), GFP_KERNEL);
	if (!hdata)
		return -ENOMEM;

	hdata->drv_data = of_device_get_match_data(dev);

	platform_set_drvdata(pdev, hdata);

	hdata->dev = dev;

	mutex_init(&hdata->mutex);

	ret = hdmi_resources_init(hdata);
	if (ret) {
		if (ret != -EPROBE_DEFER)
			DRM_DEV_ERROR(dev, "hdmi_resources_init failed\n");
		return ret;
	}

	hdata->regs = devm_platform_ioremap_resource(pdev, 0);
	if (IS_ERR(hdata->regs)) {
		ret = PTR_ERR(hdata->regs);
		return ret;
	}

	ret = hdmi_get_ddc_adapter(hdata);
	if (ret)
		return ret;

	ret = hdmi_get_phy_io(hdata);
	if (ret)
		goto err_ddc;

	INIT_DELAYED_WORK(&hdata->hotplug_work, hdmi_hotplug_work_func);

	ret = devm_request_threaded_irq(dev, hdata->irq, NULL,
			hdmi_irq_thread, IRQF_TRIGGER_RISING |
			IRQF_TRIGGER_FALLING | IRQF_ONESHOT,
			"hdmi", hdata);
	if (ret) {
		DRM_DEV_ERROR(dev, "failed to register hdmi interrupt\n");
		goto err_hdmiphy;
	}

	hdata->pmureg = syscon_regmap_lookup_by_phandle(dev->of_node,
			"samsung,syscon-phandle");
	if (IS_ERR(hdata->pmureg)) {
		DRM_DEV_ERROR(dev, "syscon regmap lookup failed.\n");
		ret = -EPROBE_DEFER;
		goto err_hdmiphy;
	}

	if (hdata->drv_data->has_sysreg) {
		hdata->sysreg = syscon_regmap_lookup_by_phandle(dev->of_node,
				"samsung,sysreg-phandle");
		if (IS_ERR(hdata->sysreg)) {
			DRM_DEV_ERROR(dev, "sysreg regmap lookup failed.\n");
			ret = -EPROBE_DEFER;
			goto err_hdmiphy;
		}
	}

	if (!IS_ERR(hdata->reg_hdmi_en)) {
		ret = regulator_enable(hdata->reg_hdmi_en);
		if (ret) {
			DRM_DEV_ERROR(dev,
			      "failed to enable hdmi-en regulator\n");
			goto err_hdmiphy;
		}
	}

	pm_runtime_enable(dev);

	audio_infoframe = &hdata->audio.infoframe;
	hdmi_audio_infoframe_init(audio_infoframe);
	audio_infoframe->coding_type = HDMI_AUDIO_CODING_TYPE_STREAM;
	audio_infoframe->sample_size = HDMI_AUDIO_SAMPLE_SIZE_STREAM;
	audio_infoframe->sample_frequency = HDMI_AUDIO_SAMPLE_FREQUENCY_STREAM;
	audio_infoframe->channels = 2;

	ret = hdmi_register_audio_device(hdata);
	if (ret)
		goto err_rpm_disable;

	ret = component_add(&pdev->dev, &hdmi_component_ops);
	if (ret)
		goto err_unregister_audio;

	return ret;

err_unregister_audio:
	platform_device_unregister(hdata->audio.pdev);

err_rpm_disable:
	pm_runtime_disable(dev);
	if (!IS_ERR(hdata->reg_hdmi_en))
		regulator_disable(hdata->reg_hdmi_en);
err_hdmiphy:
	if (hdata->hdmiphy_port)
		put_device(&hdata->hdmiphy_port->dev);
	if (hdata->regs_hdmiphy)
		iounmap(hdata->regs_hdmiphy);
err_ddc:
	put_device(&hdata->ddc_adpt->dev);

	return ret;
}

static void hdmi_remove(struct platform_device *pdev)
{
	struct hdmi_context *hdata = platform_get_drvdata(pdev);

	cancel_delayed_work_sync(&hdata->hotplug_work);

	component_del(&pdev->dev, &hdmi_component_ops);
	platform_device_unregister(hdata->audio.pdev);

	pm_runtime_disable(&pdev->dev);

	if (!IS_ERR(hdata->reg_hdmi_en))
		regulator_disable(hdata->reg_hdmi_en);

	if (hdata->hdmiphy_port)
		put_device(&hdata->hdmiphy_port->dev);

	if (hdata->regs_hdmiphy)
		iounmap(hdata->regs_hdmiphy);

	put_device(&hdata->ddc_adpt->dev);

	mutex_destroy(&hdata->mutex);
}

static int __maybe_unused exynos_hdmi_suspend(struct device *dev)
{
	struct hdmi_context *hdata = dev_get_drvdata(dev);

	hdmi_clk_disable_gates(hdata);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	return 0;
}

<<<<<<< HEAD
struct platform_driver hdmi_driver = {
	.probe		= hdmi_probe,
	.remove		= __devexit_p(hdmi_remove),
	.driver		= {
		.name	= "exynos4-hdmi",
		.owner	= THIS_MODULE,
		.pm = &hdmi_pm_ops,
=======
static int __maybe_unused exynos_hdmi_resume(struct device *dev)
{
	struct hdmi_context *hdata = dev_get_drvdata(dev);
	int ret;

	ret = hdmi_clk_enable_gates(hdata);
	if (ret < 0)
		return ret;

	return 0;
}

static const struct dev_pm_ops exynos_hdmi_pm_ops = {
	SET_RUNTIME_PM_OPS(exynos_hdmi_suspend, exynos_hdmi_resume, NULL)
	SET_SYSTEM_SLEEP_PM_OPS(pm_runtime_force_suspend,
				pm_runtime_force_resume)
};

struct platform_driver hdmi_driver = {
	.probe		= hdmi_probe,
	.remove_new	= hdmi_remove,
	.driver		= {
		.name	= "exynos-hdmi",
		.owner	= THIS_MODULE,
		.pm	= &exynos_hdmi_pm_ops,
		.of_match_table = hdmi_match_types,
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	},
};
