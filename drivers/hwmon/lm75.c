<<<<<<< HEAD
=======
// SPDX-License-Identifier: GPL-2.0-or-later
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
/*
 * lm75.c - Part of lm_sensors, Linux kernel modules for hardware
 *	 monitoring
 * Copyright (c) 1998, 1999  Frodo Looijaard <frodol@dds.nl>
<<<<<<< HEAD
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 */

#include <linux/module.h>
#include <linux/init.h>
<<<<<<< HEAD
=======
#include <linux/interrupt.h>
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#include <linux/slab.h>
#include <linux/jiffies.h>
#include <linux/i2c.h>
#include <linux/hwmon.h>
<<<<<<< HEAD
#include <linux/hwmon-sysfs.h>
#include <linux/err.h>
#include <linux/mutex.h>
#include "lm75.h"


=======
#include <linux/err.h>
#include <linux/of.h>
#include <linux/regmap.h>
#include <linux/util_macros.h>
#include <linux/regulator/consumer.h>
#include "lm75.h"

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
/*
 * This driver handles the LM75 and compatible digital temperature sensors.
 */

enum lm75_type {		/* keep sorted in alphabetical order */
	adt75,
<<<<<<< HEAD
	ds1775,
	ds75,
	lm75,
	lm75a,
	max6625,
	max6626,
	mcp980x,
	stds75,
=======
	as6200,
	at30ts74,
	ds1775,
	ds75,
	ds7505,
	g751,
	lm75,
	lm75a,
	lm75b,
	max6625,
	max6626,
	max31725,
	mcp980x,
	pct2075,
	stds75,
	stlm75,
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	tcn75,
	tmp100,
	tmp101,
	tmp105,
<<<<<<< HEAD
	tmp175,
	tmp275,
	tmp75,
=======
	tmp112,
	tmp175,
	tmp275,
	tmp75,
	tmp75b,
	tmp75c,
	tmp1075,
};

/**
 * struct lm75_params - lm75 configuration parameters.
 * @config_reg_16bits:	Configure register size is 2 bytes.
 * @set_mask:		Bits to set in configuration register when configuring
 *			the chip.
 * @clr_mask:		Bits to clear in configuration register when configuring
 *			the chip.
 * @default_resolution:	Default number of bits to represent the temperature
 *			value.
 * @resolution_limits:	Limit register resolution. Optional. Should be set if
 *			the resolution of limit registers does not match the
 *			resolution of the temperature register.
 * @resolutions:	List of resolutions associated with sample times.
 *			Optional. Should be set if num_sample_times is larger
 *			than 1, and if the resolution changes with sample times.
 *			If set, number of entries must match num_sample_times.
 * @default_sample_time:Sample time to be set by default.
 * @num_sample_times:	Number of possible sample times to be set. Optional.
 *			Should be set if the number of sample times is larger
 *			than one.
 * @sample_times:	All the possible sample times to be set. Mandatory if
 *			num_sample_times is larger than 1. If set, number of
 *			entries must match num_sample_times.
 * @alarm:		Alarm bit is supported.
 */

struct lm75_params {
	bool			config_reg_16bits;
	u16			set_mask;
	u16			clr_mask;
	u8			default_resolution;
	u8			resolution_limits;
	const u8		*resolutions;
	unsigned int		default_sample_time;
	u8			num_sample_times;
	const unsigned int	*sample_times;
	bool			alarm;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
};

/* Addresses scanned */
static const unsigned short normal_i2c[] = { 0x48, 0x49, 0x4a, 0x4b, 0x4c,
					0x4d, 0x4e, 0x4f, I2C_CLIENT_END };

<<<<<<< HEAD

/* The LM75 registers */
#define LM75_REG_CONF		0x01
static const u8 LM75_REG_TEMP[3] = {
	0x00,		/* input */
	0x03,		/* max */
	0x02,		/* hyst */
};

/* Each client has this additional data */
struct lm75_data {
	struct device		*hwmon_dev;
	struct mutex		update_lock;
	u8			orig_conf;
	char			valid;		/* !=0 if registers are valid */
	unsigned long		last_updated;	/* In jiffies */
	u16			temp[3];	/* Register values,
						   0 = input
						   1 = max
						   2 = hyst */
};

static int lm75_read_value(struct i2c_client *client, u8 reg);
static int lm75_write_value(struct i2c_client *client, u8 reg, u16 value);
static struct lm75_data *lm75_update_device(struct device *dev);


/*-----------------------------------------------------------------------*/

/* sysfs attributes for hwmon */

static ssize_t show_temp(struct device *dev, struct device_attribute *da,
			 char *buf)
{
	struct sensor_device_attribute *attr = to_sensor_dev_attr(da);
	struct lm75_data *data = lm75_update_device(dev);

	if (IS_ERR(data))
		return PTR_ERR(data);

	return sprintf(buf, "%d\n",
		       LM75_TEMP_FROM_REG(data->temp[attr->index]));
}

static ssize_t set_temp(struct device *dev, struct device_attribute *da,
			const char *buf, size_t count)
{
	struct sensor_device_attribute *attr = to_sensor_dev_attr(da);
	struct i2c_client *client = to_i2c_client(dev);
	struct lm75_data *data = i2c_get_clientdata(client);
	int nr = attr->index;
	long temp;
	int error;

	error = kstrtol(buf, 10, &temp);
	if (error)
		return error;

	mutex_lock(&data->update_lock);
	data->temp[nr] = LM75_TEMP_TO_REG(temp);
	lm75_write_value(client, LM75_REG_TEMP[nr], data->temp[nr]);
	mutex_unlock(&data->update_lock);
	return count;
}

static SENSOR_DEVICE_ATTR(temp1_max, S_IWUSR | S_IRUGO,
			show_temp, set_temp, 1);
static SENSOR_DEVICE_ATTR(temp1_max_hyst, S_IWUSR | S_IRUGO,
			show_temp, set_temp, 2);
static SENSOR_DEVICE_ATTR(temp1_input, S_IRUGO, show_temp, NULL, 0);

static struct attribute *lm75_attributes[] = {
	&sensor_dev_attr_temp1_input.dev_attr.attr,
	&sensor_dev_attr_temp1_max.dev_attr.attr,
	&sensor_dev_attr_temp1_max_hyst.dev_attr.attr,

	NULL
};

static const struct attribute_group lm75_group = {
	.attrs = lm75_attributes,
};

/*-----------------------------------------------------------------------*/

/* device probe and removal */

static int
lm75_probe(struct i2c_client *client, const struct i2c_device_id *id)
{
	struct lm75_data *data;
	int status;
	u8 set_mask, clr_mask;
	int new;
=======
/* The LM75 registers */
#define LM75_REG_TEMP		0x00
#define LM75_REG_CONF		0x01
#define LM75_REG_HYST		0x02
#define LM75_REG_MAX		0x03
#define PCT2075_REG_IDLE	0x04

/* Each client has this additional data */
struct lm75_data {
	struct i2c_client		*client;
	struct regmap			*regmap;
	struct regulator		*vs;
	u16				orig_conf;
	u16				current_conf;
	u8				resolution;	/* In bits, 9 to 16 */
	unsigned int			sample_time;	/* In ms */
	enum lm75_type			kind;
	const struct lm75_params	*params;
};

/*-----------------------------------------------------------------------*/

static const u8 lm75_sample_set_masks[] = { 0 << 5, 1 << 5, 2 << 5, 3 << 5 };

#define LM75_SAMPLE_CLEAR_MASK	(3 << 5)

/* The structure below stores the configuration values of the supported devices.
 * In case of being supported multiple configurations, the default one must
 * always be the first element of the array
 */
static const struct lm75_params device_params[] = {
	[adt75] = {
		.clr_mask = 1 << 5,	/* not one-shot mode */
		.default_resolution = 12,
		.default_sample_time = MSEC_PER_SEC / 10,
	},
	[as6200] = {
		.config_reg_16bits = true,
		.set_mask = 0x94C0,	/* 8 sample/s, 4 CF, positive polarity */
		.default_resolution = 12,
		.default_sample_time = 125,
		.num_sample_times = 4,
		.sample_times = (unsigned int []){ 125, 250, 1000, 4000 },
		.alarm = true,
	},
	[at30ts74] = {
		.set_mask = 3 << 5,	/* 12-bit mode*/
		.default_resolution = 12,
		.default_sample_time = 200,
		.num_sample_times = 4,
		.sample_times = (unsigned int []){ 25, 50, 100, 200 },
		.resolutions = (u8 []) {9, 10, 11, 12 },
	},
	[ds1775] = {
		.clr_mask = 3 << 5,
		.set_mask = 2 << 5,	/* 11-bit mode */
		.default_resolution = 11,
		.default_sample_time = 500,
		.num_sample_times = 4,
		.sample_times = (unsigned int []){ 125, 250, 500, 1000 },
		.resolutions = (u8 []) {9, 10, 11, 12 },
	},
	[ds75] = {
		.clr_mask = 3 << 5,
		.set_mask = 2 << 5,	/* 11-bit mode */
		.default_resolution = 11,
		.default_sample_time = 600,
		.num_sample_times = 4,
		.sample_times = (unsigned int []){ 150, 300, 600, 1200 },
		.resolutions = (u8 []) {9, 10, 11, 12 },
	},
	[stds75] = {
		.clr_mask = 3 << 5,
		.set_mask = 2 << 5,	/* 11-bit mode */
		.default_resolution = 11,
		.default_sample_time = 600,
		.num_sample_times = 4,
		.sample_times = (unsigned int []){ 150, 300, 600, 1200 },
		.resolutions = (u8 []) {9, 10, 11, 12 },
	},
	[stlm75] = {
		.default_resolution = 9,
		.default_sample_time = MSEC_PER_SEC / 6,
	},
	[ds7505] = {
		.set_mask = 3 << 5,	/* 12-bit mode*/
		.default_resolution = 12,
		.default_sample_time = 200,
		.num_sample_times = 4,
		.sample_times = (unsigned int []){ 25, 50, 100, 200 },
		.resolutions = (u8 []) {9, 10, 11, 12 },
	},
	[g751] = {
		.default_resolution = 9,
		.default_sample_time = MSEC_PER_SEC / 10,
	},
	[lm75] = {
		.default_resolution = 9,
		.default_sample_time = MSEC_PER_SEC / 10,
	},
	[lm75a] = {
		.default_resolution = 9,
		.default_sample_time = MSEC_PER_SEC / 10,
	},
	[lm75b] = {
		.default_resolution = 11,
		.default_sample_time = MSEC_PER_SEC / 10,
	},
	[max6625] = {
		.default_resolution = 9,
		.default_sample_time = MSEC_PER_SEC / 7,
	},
	[max6626] = {
		.default_resolution = 12,
		.default_sample_time = MSEC_PER_SEC / 7,
		.resolution_limits = 9,
	},
	[max31725] = {
		.default_resolution = 16,
		.default_sample_time = MSEC_PER_SEC / 20,
	},
	[tcn75] = {
		.default_resolution = 9,
		.default_sample_time = MSEC_PER_SEC / 18,
	},
	[pct2075] = {
		.default_resolution = 11,
		.default_sample_time = MSEC_PER_SEC / 10,
		.num_sample_times = 31,
		.sample_times = (unsigned int []){ 100, 200, 300, 400, 500, 600,
		700, 800, 900, 1000, 1100, 1200, 1300, 1400, 1500, 1600, 1700,
		1800, 1900, 2000, 2100, 2200, 2300, 2400, 2500, 2600, 2700,
		2800, 2900, 3000, 3100 },
	},
	[mcp980x] = {
		.set_mask = 3 << 5,	/* 12-bit mode */
		.clr_mask = 1 << 7,	/* not one-shot mode */
		.default_resolution = 12,
		.resolution_limits = 9,
		.default_sample_time = 240,
		.num_sample_times = 4,
		.sample_times = (unsigned int []){ 30, 60, 120, 240 },
		.resolutions = (u8 []) {9, 10, 11, 12 },
	},
	[tmp100] = {
		.set_mask = 3 << 5,	/* 12-bit mode */
		.clr_mask = 1 << 7,	/* not one-shot mode */
		.default_resolution = 12,
		.default_sample_time = 320,
		.num_sample_times = 4,
		.sample_times = (unsigned int []){ 40, 80, 160, 320 },
		.resolutions = (u8 []) {9, 10, 11, 12 },
	},
	[tmp101] = {
		.set_mask = 3 << 5,	/* 12-bit mode */
		.clr_mask = 1 << 7,	/* not one-shot mode */
		.default_resolution = 12,
		.default_sample_time = 320,
		.num_sample_times = 4,
		.sample_times = (unsigned int []){ 40, 80, 160, 320 },
		.resolutions = (u8 []) {9, 10, 11, 12 },
	},
	[tmp105] = {
		.set_mask = 3 << 5,	/* 12-bit mode */
		.clr_mask = 1 << 7,	/* not one-shot mode*/
		.default_resolution = 12,
		.default_sample_time = 220,
		.num_sample_times = 4,
		.sample_times = (unsigned int []){ 28, 55, 110, 220 },
		.resolutions = (u8 []) {9, 10, 11, 12 },
	},
	[tmp112] = {
		.config_reg_16bits = true,
		.set_mask = 0x60C0,	/* 12-bit mode, 8 samples / second */
		.clr_mask = 1 << 15,	/* no one-shot mode*/
		.default_resolution = 12,
		.default_sample_time = 125,
		.num_sample_times = 4,
		.sample_times = (unsigned int []){ 125, 250, 1000, 4000 },
	},
	[tmp175] = {
		.set_mask = 3 << 5,	/* 12-bit mode */
		.clr_mask = 1 << 7,	/* not one-shot mode*/
		.default_resolution = 12,
		.default_sample_time = 220,
		.num_sample_times = 4,
		.sample_times = (unsigned int []){ 28, 55, 110, 220 },
		.resolutions = (u8 []) {9, 10, 11, 12 },
	},
	[tmp275] = {
		.set_mask = 3 << 5,	/* 12-bit mode */
		.clr_mask = 1 << 7,	/* not one-shot mode*/
		.default_resolution = 12,
		.default_sample_time = 220,
		.num_sample_times = 4,
		.sample_times = (unsigned int []){ 28, 55, 110, 220 },
		.resolutions = (u8 []) {9, 10, 11, 12 },
	},
	[tmp75] = {
		.set_mask = 3 << 5,	/* 12-bit mode */
		.clr_mask = 1 << 7,	/* not one-shot mode*/
		.default_resolution = 12,
		.default_sample_time = 220,
		.num_sample_times = 4,
		.sample_times = (unsigned int []){ 28, 55, 110, 220 },
		.resolutions = (u8 []) {9, 10, 11, 12 },
	},
	[tmp75b] = { /* not one-shot mode, Conversion rate 37Hz */
		.clr_mask = 1 << 7 | 3 << 5,
		.default_resolution = 12,
		.default_sample_time = MSEC_PER_SEC / 37,
		.sample_times = (unsigned int []){ MSEC_PER_SEC / 37,
			MSEC_PER_SEC / 18,
			MSEC_PER_SEC / 9, MSEC_PER_SEC / 4 },
		.num_sample_times = 4,
	},
	[tmp75c] = {
		.clr_mask = 1 << 5,	/*not one-shot mode*/
		.default_resolution = 12,
		.default_sample_time = MSEC_PER_SEC / 12,
	},
	[tmp1075] = { /* not one-shot mode, 27.5 ms sample rate */
		.clr_mask = 1 << 5 | 1 << 6 | 1 << 7,
		.default_resolution = 12,
		.default_sample_time = 28,
		.num_sample_times = 4,
		.sample_times = (unsigned int []){ 28, 55, 110, 220 },
	}
};

static inline long lm75_reg_to_mc(s16 temp, u8 resolution)
{
	return ((temp >> (16 - resolution)) * 1000) >> (resolution - 8);
}

static int lm75_write_config(struct lm75_data *data, u16 set_mask,
			     u16 clr_mask)
{
	unsigned int value;

	clr_mask |= LM75_SHUTDOWN << (8 * data->params->config_reg_16bits);
	value = data->current_conf & ~clr_mask;
	value |= set_mask;

	if (data->current_conf != value) {
		s32 err;
		if (data->params->config_reg_16bits)
			err = regmap_write(data->regmap, LM75_REG_CONF, value);
		else
			err = i2c_smbus_write_byte_data(data->client,
							LM75_REG_CONF,
							value);
		if (err)
			return err;
		data->current_conf = value;
	}
	return 0;
}

static int lm75_read_config(struct lm75_data *data)
{
	int ret;
	unsigned int status;

	if (data->params->config_reg_16bits) {
		ret = regmap_read(data->regmap, LM75_REG_CONF, &status);
		return ret ? ret : status;
	}

	return i2c_smbus_read_byte_data(data->client, LM75_REG_CONF);
}

static irqreturn_t lm75_alarm_handler(int irq, void *private)
{
	struct device *hwmon_dev = private;

	hwmon_notify_event(hwmon_dev, hwmon_temp, hwmon_temp_alarm, 0);
	return IRQ_HANDLED;
}

static int lm75_read(struct device *dev, enum hwmon_sensor_types type,
		     u32 attr, int channel, long *val)
{
	struct lm75_data *data = dev_get_drvdata(dev);
	unsigned int regval;
	int err, reg;

	switch (type) {
	case hwmon_chip:
		switch (attr) {
		case hwmon_chip_update_interval:
			*val = data->sample_time;
			break;
		default:
			return -EINVAL;
		}
		break;
	case hwmon_temp:
		switch (attr) {
		case hwmon_temp_input:
			reg = LM75_REG_TEMP;
			break;
		case hwmon_temp_max:
			reg = LM75_REG_MAX;
			break;
		case hwmon_temp_max_hyst:
			reg = LM75_REG_HYST;
			break;
		case hwmon_temp_alarm:
			reg = LM75_REG_CONF;
			break;
		default:
			return -EINVAL;
		}
		err = regmap_read(data->regmap, reg, &regval);
		if (err < 0)
			return err;

		if (attr == hwmon_temp_alarm) {
			switch (data->kind) {
			case as6200:
				*val = (regval >> 5) & 0x1;
				break;
			default:
				return -EINVAL;
			}
		} else {
			*val = lm75_reg_to_mc(regval, data->resolution);
		}
		break;
	default:
		return -EINVAL;
	}
	return 0;
}

static int lm75_write_temp(struct device *dev, u32 attr, long temp)
{
	struct lm75_data *data = dev_get_drvdata(dev);
	u8 resolution;
	int reg;

	switch (attr) {
	case hwmon_temp_max:
		reg = LM75_REG_MAX;
		break;
	case hwmon_temp_max_hyst:
		reg = LM75_REG_HYST;
		break;
	default:
		return -EINVAL;
	}

	/*
	 * Resolution of limit registers is assumed to be the same as the
	 * temperature input register resolution unless given explicitly.
	 */
	if (data->params->resolution_limits)
		resolution = data->params->resolution_limits;
	else
		resolution = data->resolution;

	temp = clamp_val(temp, LM75_TEMP_MIN, LM75_TEMP_MAX);
	temp = DIV_ROUND_CLOSEST(temp  << (resolution - 8),
				 1000) << (16 - resolution);

	return regmap_write(data->regmap, reg, (u16)temp);
}

static int lm75_update_interval(struct device *dev, long val)
{
	struct lm75_data *data = dev_get_drvdata(dev);
	unsigned int reg;
	u8 index;
	s32 err;

	index = find_closest(val, data->params->sample_times,
			     (int)data->params->num_sample_times);

	switch (data->kind) {
	default:
		err = lm75_write_config(data, lm75_sample_set_masks[index],
					LM75_SAMPLE_CLEAR_MASK);
		if (err)
			return err;

		data->sample_time = data->params->sample_times[index];
		if (data->params->resolutions)
			data->resolution = data->params->resolutions[index];
		break;
	case tmp112:
	case as6200:
		err = regmap_read(data->regmap, LM75_REG_CONF, &reg);
		if (err < 0)
			return err;
		reg &= ~0x00c0;
		reg |= (3 - index) << 6;
		err = regmap_write(data->regmap, LM75_REG_CONF, reg);
		if (err < 0)
			return err;
		data->sample_time = data->params->sample_times[index];
		break;
	case pct2075:
		err = i2c_smbus_write_byte_data(data->client, PCT2075_REG_IDLE,
						index + 1);
		if (err)
			return err;
		data->sample_time = data->params->sample_times[index];
		break;
	}
	return 0;
}

static int lm75_write_chip(struct device *dev, u32 attr, long val)
{
	switch (attr) {
	case hwmon_chip_update_interval:
		return lm75_update_interval(dev, val);
	default:
		return -EINVAL;
	}
	return 0;
}

static int lm75_write(struct device *dev, enum hwmon_sensor_types type,
		      u32 attr, int channel, long val)
{
	switch (type) {
	case hwmon_chip:
		return lm75_write_chip(dev, attr, val);
	case hwmon_temp:
		return lm75_write_temp(dev, attr, val);
	default:
		return -EINVAL;
	}
	return 0;
}

static umode_t lm75_is_visible(const void *data, enum hwmon_sensor_types type,
			       u32 attr, int channel)
{
	const struct lm75_data *config_data = data;

	switch (type) {
	case hwmon_chip:
		switch (attr) {
		case hwmon_chip_update_interval:
			if (config_data->params->num_sample_times > 1)
				return 0644;
			return 0444;
		}
		break;
	case hwmon_temp:
		switch (attr) {
		case hwmon_temp_input:
			return 0444;
		case hwmon_temp_max:
		case hwmon_temp_max_hyst:
			return 0644;
		case hwmon_temp_alarm:
			if (config_data->params->alarm)
				return 0444;
			break;
		}
		break;
	default:
		break;
	}
	return 0;
}

static const struct hwmon_channel_info * const lm75_info[] = {
	HWMON_CHANNEL_INFO(chip,
			   HWMON_C_REGISTER_TZ | HWMON_C_UPDATE_INTERVAL),
	HWMON_CHANNEL_INFO(temp,
			   HWMON_T_INPUT | HWMON_T_MAX | HWMON_T_MAX_HYST |
			   HWMON_T_ALARM),
	NULL
};

static const struct hwmon_ops lm75_hwmon_ops = {
	.is_visible = lm75_is_visible,
	.read = lm75_read,
	.write = lm75_write,
};

static const struct hwmon_chip_info lm75_chip_info = {
	.ops = &lm75_hwmon_ops,
	.info = lm75_info,
};

static bool lm75_is_writeable_reg(struct device *dev, unsigned int reg)
{
	return reg != LM75_REG_TEMP;
}

static bool lm75_is_volatile_reg(struct device *dev, unsigned int reg)
{
	return reg == LM75_REG_TEMP || reg == LM75_REG_CONF;
}

static const struct regmap_config lm75_regmap_config = {
	.reg_bits = 8,
	.val_bits = 16,
	.max_register = PCT2075_REG_IDLE,
	.writeable_reg = lm75_is_writeable_reg,
	.volatile_reg = lm75_is_volatile_reg,
	.val_format_endian = REGMAP_ENDIAN_BIG,
	.cache_type = REGCACHE_MAPLE,
	.use_single_read = true,
	.use_single_write = true,
};

static void lm75_disable_regulator(void *data)
{
	struct lm75_data *lm75 = data;

	regulator_disable(lm75->vs);
}

static void lm75_remove(void *data)
{
	struct lm75_data *lm75 = data;
	struct i2c_client *client = lm75->client;

	i2c_smbus_write_byte_data(client, LM75_REG_CONF, lm75->orig_conf);
}

static const struct i2c_device_id lm75_ids[];

static int lm75_probe(struct i2c_client *client)
{
	struct device *dev = &client->dev;
	struct device *hwmon_dev;
	struct lm75_data *data;
	int status, err;
	enum lm75_type kind;

	if (client->dev.of_node)
		kind = (uintptr_t)of_device_get_match_data(&client->dev);
	else
		kind = i2c_match_id(lm75_ids, client)->driver_data;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	if (!i2c_check_functionality(client->adapter,
			I2C_FUNC_SMBUS_BYTE_DATA | I2C_FUNC_SMBUS_WORD_DATA))
		return -EIO;

<<<<<<< HEAD
	data = kzalloc(sizeof(struct lm75_data), GFP_KERNEL);
	if (!data)
		return -ENOMEM;

	i2c_set_clientdata(client, data);
	mutex_init(&data->update_lock);
=======
	data = devm_kzalloc(dev, sizeof(struct lm75_data), GFP_KERNEL);
	if (!data)
		return -ENOMEM;

	data->client = client;
	data->kind = kind;

	data->vs = devm_regulator_get(dev, "vs");
	if (IS_ERR(data->vs))
		return PTR_ERR(data->vs);

	data->regmap = devm_regmap_init_i2c(client, &lm75_regmap_config);
	if (IS_ERR(data->regmap))
		return PTR_ERR(data->regmap);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	/* Set to LM75 resolution (9 bits, 1/2 degree C) and range.
	 * Then tweak to be more precise when appropriate.
	 */
<<<<<<< HEAD
	set_mask = 0;
	clr_mask = (1 << 0)			/* continuous conversions */
		| (1 << 6) | (1 << 5);		/* 9-bit mode */

	/* configure as specified */
	status = lm75_read_value(client, LM75_REG_CONF);
	if (status < 0) {
		dev_dbg(&client->dev, "Can't read config? %d\n", status);
		goto exit_free;
	}
	data->orig_conf = status;
	new = status & ~clr_mask;
	new |= set_mask;
	if (status != new)
		lm75_write_value(client, LM75_REG_CONF, new);
	dev_dbg(&client->dev, "Config %02x\n", new);

	/* Register sysfs hooks */
	status = sysfs_create_group(&client->dev.kobj, &lm75_group);
	if (status)
		goto exit_free;

	data->hwmon_dev = hwmon_device_register(&client->dev);
	if (IS_ERR(data->hwmon_dev)) {
		status = PTR_ERR(data->hwmon_dev);
		goto exit_remove;
	}

	dev_info(&client->dev, "%s: sensor '%s'\n",
		 dev_name(data->hwmon_dev), client->name);

	return 0;

exit_remove:
	sysfs_remove_group(&client->dev.kobj, &lm75_group);
exit_free:
	kfree(data);
	return status;
}

static int lm75_remove(struct i2c_client *client)
{
	struct lm75_data *data = i2c_get_clientdata(client);

	hwmon_device_unregister(data->hwmon_dev);
	sysfs_remove_group(&client->dev.kobj, &lm75_group);
	lm75_write_value(client, LM75_REG_CONF, data->orig_conf);
	kfree(data);
	return 0;
=======

	data->params = &device_params[data->kind];

	/* Save default sample time and resolution*/
	data->sample_time = data->params->default_sample_time;
	data->resolution = data->params->default_resolution;

	/* Enable the power */
	err = regulator_enable(data->vs);
	if (err) {
		dev_err(dev, "failed to enable regulator: %d\n", err);
		return err;
	}

	err = devm_add_action_or_reset(dev, lm75_disable_regulator, data);
	if (err)
		return err;

	/* Cache original configuration */
	status = lm75_read_config(data);
	if (status < 0) {
		dev_dbg(dev, "Can't read config? %d\n", status);
		return status;
	}
	data->orig_conf = status;
	data->current_conf = status;

	err = lm75_write_config(data, data->params->set_mask,
				data->params->clr_mask);
	if (err)
		return err;

	err = devm_add_action_or_reset(dev, lm75_remove, data);
	if (err)
		return err;

	hwmon_dev = devm_hwmon_device_register_with_info(dev, client->name,
							 data, &lm75_chip_info,
							 NULL);
	if (IS_ERR(hwmon_dev))
		return PTR_ERR(hwmon_dev);

	if (client->irq) {
		if (data->params->alarm) {
			err = devm_request_threaded_irq(dev,
							client->irq,
							NULL,
							&lm75_alarm_handler,
							IRQF_ONESHOT,
							client->name,
							hwmon_dev);
			if (err)
				return err;
		} else {
			 /* alarm is only supported for chips with alarm bit */
			dev_err(dev, "alarm interrupt is not supported\n");
		}
	}

	dev_info(dev, "%s: sensor '%s'\n", dev_name(hwmon_dev), client->name);

	return 0;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

static const struct i2c_device_id lm75_ids[] = {
	{ "adt75", adt75, },
<<<<<<< HEAD
	{ "ds1775", ds1775, },
	{ "ds75", ds75, },
	{ "lm75", lm75, },
	{ "lm75a", lm75a, },
	{ "max6625", max6625, },
	{ "max6626", max6626, },
	{ "mcp980x", mcp980x, },
	{ "stds75", stds75, },
=======
	{ "as6200", as6200, },
	{ "at30ts74", at30ts74, },
	{ "ds1775", ds1775, },
	{ "ds75", ds75, },
	{ "ds7505", ds7505, },
	{ "g751", g751, },
	{ "lm75", lm75, },
	{ "lm75a", lm75a, },
	{ "lm75b", lm75b, },
	{ "max6625", max6625, },
	{ "max6626", max6626, },
	{ "max31725", max31725, },
	{ "max31726", max31725, },
	{ "mcp980x", mcp980x, },
	{ "pct2075", pct2075, },
	{ "stds75", stds75, },
	{ "stlm75", stlm75, },
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	{ "tcn75", tcn75, },
	{ "tmp100", tmp100, },
	{ "tmp101", tmp101, },
	{ "tmp105", tmp105, },
<<<<<<< HEAD
	{ "tmp175", tmp175, },
	{ "tmp275", tmp275, },
	{ "tmp75", tmp75, },
=======
	{ "tmp112", tmp112, },
	{ "tmp175", tmp175, },
	{ "tmp275", tmp275, },
	{ "tmp75", tmp75, },
	{ "tmp75b", tmp75b, },
	{ "tmp75c", tmp75c, },
	{ "tmp1075", tmp1075, },
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	{ /* LIST END */ }
};
MODULE_DEVICE_TABLE(i2c, lm75_ids);

<<<<<<< HEAD
=======
static const struct of_device_id __maybe_unused lm75_of_match[] = {
	{
		.compatible = "adi,adt75",
		.data = (void *)adt75
	},
	{
		.compatible = "ams,as6200",
		.data = (void *)as6200
	},
	{
		.compatible = "atmel,at30ts74",
		.data = (void *)at30ts74
	},
	{
		.compatible = "dallas,ds1775",
		.data = (void *)ds1775
	},
	{
		.compatible = "dallas,ds75",
		.data = (void *)ds75
	},
	{
		.compatible = "dallas,ds7505",
		.data = (void *)ds7505
	},
	{
		.compatible = "gmt,g751",
		.data = (void *)g751
	},
	{
		.compatible = "national,lm75",
		.data = (void *)lm75
	},
	{
		.compatible = "national,lm75a",
		.data = (void *)lm75a
	},
	{
		.compatible = "national,lm75b",
		.data = (void *)lm75b
	},
	{
		.compatible = "maxim,max6625",
		.data = (void *)max6625
	},
	{
		.compatible = "maxim,max6626",
		.data = (void *)max6626
	},
	{
		.compatible = "maxim,max31725",
		.data = (void *)max31725
	},
	{
		.compatible = "maxim,max31726",
		.data = (void *)max31725
	},
	{
		.compatible = "maxim,mcp980x",
		.data = (void *)mcp980x
	},
	{
		.compatible = "nxp,pct2075",
		.data = (void *)pct2075
	},
	{
		.compatible = "st,stds75",
		.data = (void *)stds75
	},
	{
		.compatible = "st,stlm75",
		.data = (void *)stlm75
	},
	{
		.compatible = "microchip,tcn75",
		.data = (void *)tcn75
	},
	{
		.compatible = "ti,tmp100",
		.data = (void *)tmp100
	},
	{
		.compatible = "ti,tmp101",
		.data = (void *)tmp101
	},
	{
		.compatible = "ti,tmp105",
		.data = (void *)tmp105
	},
	{
		.compatible = "ti,tmp112",
		.data = (void *)tmp112
	},
	{
		.compatible = "ti,tmp175",
		.data = (void *)tmp175
	},
	{
		.compatible = "ti,tmp275",
		.data = (void *)tmp275
	},
	{
		.compatible = "ti,tmp75",
		.data = (void *)tmp75
	},
	{
		.compatible = "ti,tmp75b",
		.data = (void *)tmp75b
	},
	{
		.compatible = "ti,tmp75c",
		.data = (void *)tmp75c
	},
	{
		.compatible = "ti,tmp1075",
		.data = (void *)tmp1075
	},
	{ },
};
MODULE_DEVICE_TABLE(of, lm75_of_match);

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#define LM75A_ID 0xA1

/* Return 0 if detection is successful, -ENODEV otherwise */
static int lm75_detect(struct i2c_client *new_client,
		       struct i2c_board_info *info)
{
	struct i2c_adapter *adapter = new_client->adapter;
	int i;
	int conf, hyst, os;
	bool is_lm75a = 0;

	if (!i2c_check_functionality(adapter, I2C_FUNC_SMBUS_BYTE_DATA |
				     I2C_FUNC_SMBUS_WORD_DATA))
		return -ENODEV;

	/*
	 * Now, we do the remaining detection. There is no identification-
	 * dedicated register so we have to rely on several tricks:
	 * unused bits, registers cycling over 8-address boundaries,
	 * addresses 0x04-0x07 returning the last read value.
	 * The cycling+unused addresses combination is not tested,
	 * since it would significantly slow the detection down and would
	 * hardly add any value.
	 *
	 * The National Semiconductor LM75A is different than earlier
	 * LM75s.  It has an ID byte of 0xaX (where X is the chip
	 * revision, with 1 being the only revision in existence) in
	 * register 7, and unused registers return 0xff rather than the
	 * last read value.
	 *
	 * Note that this function only detects the original National
	 * Semiconductor LM75 and the LM75A. Clones from other vendors
	 * aren't detected, on purpose, because they are typically never
	 * found on PC hardware. They are found on embedded designs where
	 * they can be instantiated explicitly so detection is not needed.
	 * The absence of identification registers on all these clones
	 * would make their exhaustive detection very difficult and weak,
	 * and odds are that the driver would bind to unsupported devices.
	 */

	/* Unused bits */
	conf = i2c_smbus_read_byte_data(new_client, 1);
	if (conf & 0xe0)
		return -ENODEV;

	/* First check for LM75A */
	if (i2c_smbus_read_byte_data(new_client, 7) == LM75A_ID) {
<<<<<<< HEAD
		/* LM75A returns 0xff on unused registers so
		   just to be sure we check for that too. */
=======
		/*
		 * LM75A returns 0xff on unused registers so
		 * just to be sure we check for that too.
		 */
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		if (i2c_smbus_read_byte_data(new_client, 4) != 0xff
		 || i2c_smbus_read_byte_data(new_client, 5) != 0xff
		 || i2c_smbus_read_byte_data(new_client, 6) != 0xff)
			return -ENODEV;
		is_lm75a = 1;
		hyst = i2c_smbus_read_byte_data(new_client, 2);
		os = i2c_smbus_read_byte_data(new_client, 3);
	} else { /* Traditional style LM75 detection */
		/* Unused addresses */
		hyst = i2c_smbus_read_byte_data(new_client, 2);
		if (i2c_smbus_read_byte_data(new_client, 4) != hyst
		 || i2c_smbus_read_byte_data(new_client, 5) != hyst
		 || i2c_smbus_read_byte_data(new_client, 6) != hyst
		 || i2c_smbus_read_byte_data(new_client, 7) != hyst)
			return -ENODEV;
		os = i2c_smbus_read_byte_data(new_client, 3);
		if (i2c_smbus_read_byte_data(new_client, 4) != os
		 || i2c_smbus_read_byte_data(new_client, 5) != os
		 || i2c_smbus_read_byte_data(new_client, 6) != os
		 || i2c_smbus_read_byte_data(new_client, 7) != os)
			return -ENODEV;
	}
<<<<<<< HEAD
=======
	/*
	 * It is very unlikely that this is a LM75 if both
	 * hysteresis and temperature limit registers are 0.
	 */
	if (hyst == 0 && os == 0)
		return -ENODEV;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	/* Addresses cycling */
	for (i = 8; i <= 248; i += 40) {
		if (i2c_smbus_read_byte_data(new_client, i + 1) != conf
		 || i2c_smbus_read_byte_data(new_client, i + 2) != hyst
		 || i2c_smbus_read_byte_data(new_client, i + 3) != os)
			return -ENODEV;
		if (is_lm75a && i2c_smbus_read_byte_data(new_client, i + 7)
				!= LM75A_ID)
			return -ENODEV;
	}

<<<<<<< HEAD
	strlcpy(info->type, is_lm75a ? "lm75a" : "lm75", I2C_NAME_SIZE);
=======
	strscpy(info->type, is_lm75a ? "lm75a" : "lm75", I2C_NAME_SIZE);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	return 0;
}

#ifdef CONFIG_PM
static int lm75_suspend(struct device *dev)
{
	int status;
	struct i2c_client *client = to_i2c_client(dev);
<<<<<<< HEAD
	status = lm75_read_value(client, LM75_REG_CONF);
=======

	status = i2c_smbus_read_byte_data(client, LM75_REG_CONF);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	if (status < 0) {
		dev_dbg(&client->dev, "Can't read config? %d\n", status);
		return status;
	}
	status = status | LM75_SHUTDOWN;
<<<<<<< HEAD
	lm75_write_value(client, LM75_REG_CONF, status);
=======
	i2c_smbus_write_byte_data(client, LM75_REG_CONF, status);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	return 0;
}

static int lm75_resume(struct device *dev)
{
	int status;
	struct i2c_client *client = to_i2c_client(dev);
<<<<<<< HEAD
	status = lm75_read_value(client, LM75_REG_CONF);
=======

	status = i2c_smbus_read_byte_data(client, LM75_REG_CONF);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	if (status < 0) {
		dev_dbg(&client->dev, "Can't read config? %d\n", status);
		return status;
	}
	status = status & ~LM75_SHUTDOWN;
<<<<<<< HEAD
	lm75_write_value(client, LM75_REG_CONF, status);
=======
	i2c_smbus_write_byte_data(client, LM75_REG_CONF, status);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	return 0;
}

static const struct dev_pm_ops lm75_dev_pm_ops = {
	.suspend	= lm75_suspend,
	.resume		= lm75_resume,
};
#define LM75_DEV_PM_OPS (&lm75_dev_pm_ops)
#else
#define LM75_DEV_PM_OPS NULL
#endif /* CONFIG_PM */

static struct i2c_driver lm75_driver = {
	.class		= I2C_CLASS_HWMON,
	.driver = {
		.name	= "lm75",
<<<<<<< HEAD
		.pm	= LM75_DEV_PM_OPS,
	},
	.probe		= lm75_probe,
	.remove		= lm75_remove,
=======
		.of_match_table = of_match_ptr(lm75_of_match),
		.pm	= LM75_DEV_PM_OPS,
	},
	.probe		= lm75_probe,
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	.id_table	= lm75_ids,
	.detect		= lm75_detect,
	.address_list	= normal_i2c,
};

<<<<<<< HEAD
/*-----------------------------------------------------------------------*/

/* register access */

/*
 * All registers are word-sized, except for the configuration register.
 * LM75 uses a high-byte first convention, which is exactly opposite to
 * the SMBus standard.
 */
static int lm75_read_value(struct i2c_client *client, u8 reg)
{
	if (reg == LM75_REG_CONF)
		return i2c_smbus_read_byte_data(client, reg);
	else
		return i2c_smbus_read_word_swapped(client, reg);
}

static int lm75_write_value(struct i2c_client *client, u8 reg, u16 value)
{
	if (reg == LM75_REG_CONF)
		return i2c_smbus_write_byte_data(client, reg, value);
	else
		return i2c_smbus_write_word_swapped(client, reg, value);
}

static struct lm75_data *lm75_update_device(struct device *dev)
{
	struct i2c_client *client = to_i2c_client(dev);
	struct lm75_data *data = i2c_get_clientdata(client);
	struct lm75_data *ret = data;

	mutex_lock(&data->update_lock);

	if (time_after(jiffies, data->last_updated + HZ + HZ / 2)
	    || !data->valid) {
		int i;
		dev_dbg(&client->dev, "Starting lm75 update\n");

		for (i = 0; i < ARRAY_SIZE(data->temp); i++) {
			int status;

			status = lm75_read_value(client, LM75_REG_TEMP[i]);
			if (unlikely(status < 0)) {
				dev_dbg(dev,
					"LM75: Failed to read value: reg %d, error %d\n",
					LM75_REG_TEMP[i], status);
				ret = ERR_PTR(status);
				data->valid = 0;
				goto abort;
			}
			data->temp[i] = status;
		}
		data->last_updated = jiffies;
		data->valid = 1;
	}

abort:
	mutex_unlock(&data->update_lock);
	return ret;
}

=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
module_i2c_driver(lm75_driver);

MODULE_AUTHOR("Frodo Looijaard <frodol@dds.nl>");
MODULE_DESCRIPTION("LM75 driver");
MODULE_LICENSE("GPL");
