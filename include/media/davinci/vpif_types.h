<<<<<<< HEAD
/*
 * Copyright (C) 2011 Texas Instruments Inc
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation version 2.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307 USA
=======
/* SPDX-License-Identifier: GPL-2.0-only */
/*
 * Copyright (C) 2011 Texas Instruments Inc
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 */
#ifndef _VPIF_TYPES_H
#define _VPIF_TYPES_H

#include <linux/i2c.h>

#define VPIF_CAPTURE_MAX_CHANNELS	2
<<<<<<< HEAD
=======
#define VPIF_DISPLAY_MAX_CHANNELS	2
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

enum vpif_if_type {
	VPIF_IF_BT656,
	VPIF_IF_BT1120,
	VPIF_IF_RAW_BAYER
};

struct vpif_interface {
	enum vpif_if_type if_type;
	unsigned hd_pol:1;
	unsigned vd_pol:1;
	unsigned fid_pol:1;
};

struct vpif_subdev_info {
	const char *name;
	struct i2c_board_info board_info;
<<<<<<< HEAD
	u32 input;
	u32 output;
	unsigned can_route:1;
	struct vpif_interface vpif_if;
=======
};

struct vpif_output {
	struct v4l2_output output;
	const char *subdev_name;
	u32 input_route;
	u32 output_route;
};

struct vpif_display_chan_config {
	const struct vpif_output *outputs;
	int output_count;
	bool clip_en;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
};

struct vpif_display_config {
	int (*set_clock)(int, int);
	struct vpif_subdev_info *subdevinfo;
	int subdev_count;
<<<<<<< HEAD
	const char **output;
	int output_count;
=======
	int i2c_adapter_id;
	struct vpif_display_chan_config chan_config[VPIF_DISPLAY_MAX_CHANNELS];
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	const char *card_name;
};

struct vpif_input {
	struct v4l2_input input;
<<<<<<< HEAD
	const char *subdev_name;
};

struct vpif_capture_chan_config {
	const struct vpif_input *inputs;
=======
	char *subdev_name;
	u32 input_route;
	u32 output_route;
};

struct vpif_capture_chan_config {
	struct vpif_interface vpif_if;
	struct vpif_input *inputs;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	int input_count;
};

struct vpif_capture_config {
	int (*setup_input_channel_mode)(int);
	int (*setup_input_path)(int, const char *);
	struct vpif_capture_chan_config chan_config[VPIF_CAPTURE_MAX_CHANNELS];
	struct vpif_subdev_info *subdev_info;
	int subdev_count;
<<<<<<< HEAD
	const char *card_name;
=======
	int i2c_adapter_id;
	const char *card_name;

	struct v4l2_async_connection *asd[VPIF_CAPTURE_MAX_CHANNELS];
	int asd_sizes[VPIF_CAPTURE_MAX_CHANNELS];
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
};
#endif /* _VPIF_TYPES_H */
