<<<<<<< HEAD
/*
 * FAN53555 Fairchild Digitally Programmable TinyBuck Regulator Driver.
 *
 * Supported Part Numbers:
 * FAN53555UC00X/01X/03X/04X/05X
 *
 * Copyright (c) 2012 Marvell Technology Ltd.
 * Yunfan Zhang <yfzhang@marvell.com>
 *
 * This package is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 *
 */
#include <linux/module.h>
#include <linux/param.h>
#include <linux/err.h>
#include <linux/delay.h>
#include <linux/gpio.h>
#include <linux/platform_device.h>
#include <linux/regulator/driver.h>
#include <linux/regulator/machine.h>
#include <linux/regulator/of_regulator.h>
#include <linux/of.h>
#include <linux/of_gpio.h>
#include <linux/i2c.h>
#include <linux/slab.h>
#include <linux/regmap.h>
#include <linux/regulator/fan53555.h>
=======
// SPDX-License-Identifier: GPL-2.0
//
// FAN53555 Fairchild Digitally Programmable TinyBuck Regulator Driver.
//
// Supported Part Numbers:
// FAN53555UC00X/01X/03X/04X/05X
//
// Copyright (c) 2012 Marvell Technology Ltd.
// Yunfan Zhang <yfzhang@marvell.com>

#include <linux/bitops.h>
#include <linux/err.h>
#include <linux/i2c.h>
#include <linux/module.h>
#include <linux/of.h>
#include <linux/param.h>
#include <linux/platform_device.h>
#include <linux/regmap.h>
#include <linux/regulator/driver.h>
#include <linux/regulator/fan53555.h>
#include <linux/regulator/machine.h>
#include <linux/regulator/of_regulator.h>
#include <linux/slab.h>
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

/* Voltage setting */
#define FAN53555_VSEL0		0x00
#define FAN53555_VSEL1		0x01
<<<<<<< HEAD
=======

#define RK8602_VSEL0		0x06
#define RK8602_VSEL1		0x07

#define TCS4525_VSEL0		0x11
#define TCS4525_VSEL1		0x10
#define TCS4525_TIME		0x13
#define TCS4525_COMMAND		0x14

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
/* Control register */
#define FAN53555_CONTROL	0x02
/* IC Type */
#define FAN53555_ID1		0x03
/* IC mask version */
#define FAN53555_ID2		0x04
/* Monitor register */
#define FAN53555_MONITOR	0x05

/* VSEL bit definitions */
<<<<<<< HEAD
#define VSEL_BUCK_EN	(1 << 7)
#define VSEL_MODE		(1 << 6)
#define VSEL_NSEL_MASK	0x3F
#define VSEL_FULL_MASK	0xFF
/* Chip ID and Verison */
#define DIE_ID		0x0F	/* ID1 */
#define DIE_REV		0x0F	/* ID2 */
#define DIE_13_REV	0x0F	/* DIE Revsion ID of 13 option */

/* Control bit definitions */
#define CTL_OUTPUT_DISCHG	(1 << 7)
#define CTL_SLEW_MASK		(0x7 << 4)
#define CTL_SLEW_SHIFT		4
#define CTL_RESET			(1 << 2)

#define FAN53555_NVOLTAGES	64	/* Numbers of voltages */
=======
#define VSEL_BUCK_EN		BIT(7)
#define VSEL_MODE		BIT(6)
/* Chip ID and Verison */
#define DIE_ID			0x0F	/* ID1 */
#define DIE_REV			0x0F	/* ID2 */
/* Control bit definitions */
#define CTL_OUTPUT_DISCHG	BIT(7)
#define CTL_SLEW_MASK		GENMASK(6, 4)
#define CTL_RESET		BIT(2)
#define CTL_MODE_VSEL0_MODE	BIT(0)
#define CTL_MODE_VSEL1_MODE	BIT(1)

#define FAN53555_NVOLTAGES	64	/* Numbers of voltages */
#define FAN53526_NVOLTAGES	128
#define RK8602_NVOLTAGES	160

#define TCS_VSEL0_MODE		BIT(7)
#define TCS_VSEL1_MODE		BIT(6)

#define TCS_SLEW_MASK		GENMASK(4, 3)

enum fan53555_vendor {
	FAN53526_VENDOR_FAIRCHILD = 0,
	FAN53555_VENDOR_FAIRCHILD,
	FAN53555_VENDOR_ROCKCHIP,	/* RK8600, RK8601 */
	RK8602_VENDOR_ROCKCHIP,		/* RK8602, RK8603 */
	FAN53555_VENDOR_SILERGY,
	FAN53526_VENDOR_TCS,
};

enum {
	FAN53526_CHIP_ID_01 = 1,
};

enum {
	FAN53526_CHIP_REV_08 = 8,
};
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

/* IC Type */
enum {
	FAN53555_CHIP_ID_00 = 0,
	FAN53555_CHIP_ID_01,
	FAN53555_CHIP_ID_02,
	FAN53555_CHIP_ID_03,
	FAN53555_CHIP_ID_04,
	FAN53555_CHIP_ID_05,
<<<<<<< HEAD
};

static const int slew_rate_plan[] = {
	64000,
	32000,
	16000,
	8000,
	4000,
	2000,
	1000,
	500
};

struct fan53555_device_info {
	struct regmap *regmap;
	struct device *dev;
	struct regulator_desc desc;
	struct regulator_dev *rdev;
=======
	FAN53555_CHIP_ID_08 = 8,
};

enum {
	RK8600_CHIP_ID_08 = 8,		/* RK8600, RK8601 */
};

enum {
	RK8602_CHIP_ID_10 = 10,		/* RK8602, RK8603 */
};

enum {
	TCS4525_CHIP_ID_12 = 12,
};

enum {
	TCS4526_CHIP_ID_00 = 0,
};

/* IC mask revision */
enum {
	FAN53555_CHIP_REV_00 = 0x3,
	FAN53555_CHIP_REV_13 = 0xf,
};

enum {
	SILERGY_SYR82X = 8,
	SILERGY_SYR83X = 9,
};

struct fan53555_device_info {
	enum fan53555_vendor vendor;
	struct device *dev;
	struct regulator_desc desc;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	struct regulator_init_data *regulator;
	/* IC Type and Rev */
	int chip_id;
	int chip_rev;
	/* Voltage setting register */
	unsigned int vol_reg;
	unsigned int sleep_reg;
<<<<<<< HEAD
	/* Voltage range and step(linear) */
	unsigned int vsel_min;
	unsigned int vsel_step;
	/* Voltage slew rate limiting */
	unsigned int slew_rate;
	/* Sleep voltage cache */
	unsigned int sleep_vol_cache;

	bool disable_suspend;
};

static int fan53555_get_voltage(struct regulator_dev *rdev)
{
	struct fan53555_device_info *di = rdev_get_drvdata(rdev);
	unsigned int val;
	int rc;

	rc = regmap_read(di->regmap, di->vol_reg, &val);
	if (rc) {
		dev_err(di->dev, "Unable to get voltage rc(%d)", rc);
		return rc;
	}

	return ((val & VSEL_NSEL_MASK) * di->vsel_step) +
		di->vsel_min;
}

static int fan53555_set_voltage(struct regulator_dev *rdev,
			int min_uv, int max_uv, unsigned *selector)
{
	struct fan53555_device_info *di = rdev_get_drvdata(rdev);
	int rc, set_val, cur_uv, new_uv;

	set_val = DIV_ROUND_UP(min_uv - di->vsel_min, di->vsel_step);
	new_uv = (set_val * di->vsel_step) + di->vsel_min;

	if (new_uv > max_uv || max_uv < di->vsel_min) {
		dev_err(di->dev, "Unable to set voltage (%d %d)\n",
			min_uv, max_uv);
	}

	cur_uv = fan53555_get_voltage(rdev);
	if (cur_uv < 0)
		return cur_uv;

	rc = regmap_update_bits(di->regmap, di->vol_reg, VSEL_NSEL_MASK,
				set_val);
	if (rc) {
		dev_err(di->dev, "Unable to set voltage (%d %d)\n",
			min_uv, max_uv);
	} else {
		udelay(DIV_ROUND_UP(abs(new_uv - cur_uv),
			slew_rate_plan[di->slew_rate]));
		*selector = set_val;
	}

	return rc;
}

static int fan53555_list_voltage(struct regulator_dev *rdev,
						unsigned selector)
{
	struct fan53555_device_info *di = rdev_get_drvdata(rdev);

	if (selector >= di->desc.n_voltages)
		return 0;

	return selector * di->vsel_step + di->vsel_min;
}

static int fan53555_set_suspend_voltage(struct regulator_dev *rdev, int uV)
{
	struct fan53555_device_info *di = rdev_get_drvdata(rdev);
	int ret, val;

	if (di->sleep_vol_cache == uV)
		return 0;
	ret = fan53555_set_voltage(rdev, uV, uV, &val);
	if (ret < 0)
		return -EINVAL;
	ret = regmap_update_bits(di->regmap, di->sleep_reg,
					VSEL_NSEL_MASK, val);
	if (ret < 0)
		return -EINVAL;
=======
	unsigned int en_reg;
	unsigned int sleep_en_reg;
	/* Voltage range and step(linear) */
	unsigned int vsel_min;
	unsigned int vsel_step;
	unsigned int vsel_count;
	/* Mode */
	unsigned int mode_reg;
	unsigned int mode_mask;
	/* Sleep voltage cache */
	unsigned int sleep_vol_cache;
	/* Slew rate */
	unsigned int slew_reg;
	unsigned int slew_mask;
	const unsigned int *ramp_delay_table;
	unsigned int n_ramp_values;
	unsigned int slew_rate;
};

static int fan53555_set_suspend_voltage(struct regulator_dev *rdev, int uV)
{
	struct fan53555_device_info *di = rdev_get_drvdata(rdev);
	int ret;

	if (di->sleep_vol_cache == uV)
		return 0;
	ret = regulator_map_voltage_linear(rdev, uV, uV);
	if (ret < 0)
		return ret;
	ret = regmap_update_bits(rdev->regmap, di->sleep_reg,
				 di->desc.vsel_mask, ret);
	if (ret < 0)
		return ret;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	/* Cache the sleep voltage setting.
	 * Might not be the real voltage which is rounded */
	di->sleep_vol_cache = uV;

	return 0;
}

<<<<<<< HEAD
static int fan53555_enable(struct regulator_dev *rdev)
{
	struct fan53555_device_info *di = rdev_get_drvdata(rdev);
	int ret;

	ret = regmap_update_bits(di->regmap, di->vol_reg,
					VSEL_BUCK_EN, VSEL_BUCK_EN);
	if (ret)
		dev_err(di->dev, "Unable to enable regulator, ret = %d\n",
			ret);
	return ret;
}

static int fan53555_disable(struct regulator_dev *rdev)
{
	struct fan53555_device_info *di = rdev_get_drvdata(rdev);
	int ret;

	ret = regmap_update_bits(di->regmap, di->vol_reg,
					VSEL_BUCK_EN, 0);
	if (ret)
		dev_err(di->dev, "Unable to set disable regulator, ret = %d\n",
			ret);
	return ret;
}

static int fan53555_is_enabled(struct regulator_dev *rdev)
{
	struct fan53555_device_info *di = rdev_get_drvdata(rdev);
	int ret;
	u32 val;

	ret = regmap_read(di->regmap, di->vol_reg, &val);
	if (ret) {
		dev_err(di->dev, "Unable to get regulator status, ret = %d\n",
			ret);
		return ret;
	} else {
		return val & VSEL_BUCK_EN;
	}
=======
static int fan53555_set_suspend_enable(struct regulator_dev *rdev)
{
	struct fan53555_device_info *di = rdev_get_drvdata(rdev);

	return regmap_update_bits(rdev->regmap, di->sleep_en_reg,
				  VSEL_BUCK_EN, VSEL_BUCK_EN);
}

static int fan53555_set_suspend_disable(struct regulator_dev *rdev)
{
	struct fan53555_device_info *di = rdev_get_drvdata(rdev);

	return regmap_update_bits(rdev->regmap, di->sleep_en_reg,
				  VSEL_BUCK_EN, 0);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

static int fan53555_set_mode(struct regulator_dev *rdev, unsigned int mode)
{
	struct fan53555_device_info *di = rdev_get_drvdata(rdev);

	switch (mode) {
	case REGULATOR_MODE_FAST:
<<<<<<< HEAD
		regmap_update_bits(di->regmap, di->vol_reg,
				VSEL_MODE, VSEL_MODE);
		break;
	case REGULATOR_MODE_NORMAL:
		regmap_update_bits(di->regmap, di->vol_reg, VSEL_MODE, 0);
=======
		regmap_update_bits(rdev->regmap, di->mode_reg,
				   di->mode_mask, di->mode_mask);
		break;
	case REGULATOR_MODE_NORMAL:
		regmap_update_bits(rdev->regmap, di->vol_reg, di->mode_mask, 0);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		break;
	default:
		return -EINVAL;
	}
	return 0;
}

static unsigned int fan53555_get_mode(struct regulator_dev *rdev)
{
	struct fan53555_device_info *di = rdev_get_drvdata(rdev);
	unsigned int val;
	int ret = 0;

<<<<<<< HEAD
	ret = regmap_read(di->regmap, di->vol_reg, &val);
	if (ret < 0)
		return ret;
	if (val & VSEL_MODE)
=======
	ret = regmap_read(rdev->regmap, di->mode_reg, &val);
	if (ret < 0)
		return ret;
	if (val & di->mode_mask)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		return REGULATOR_MODE_FAST;
	else
		return REGULATOR_MODE_NORMAL;
}

<<<<<<< HEAD
static struct regulator_ops fan53555_regulator_ops = {
	.set_voltage = fan53555_set_voltage,
	.get_voltage = fan53555_get_voltage,
	.list_voltage = fan53555_list_voltage,
	.set_suspend_voltage = fan53555_set_suspend_voltage,
	.enable = fan53555_enable,
	.disable = fan53555_disable,
	.is_enabled = fan53555_is_enabled,
	.set_mode = fan53555_set_mode,
	.get_mode = fan53555_get_mode,
};

static struct regulator_ops fan53555_regulator_disable_suspend_ops = {
	.set_voltage = fan53555_set_voltage,
	.get_voltage = fan53555_get_voltage,
	.list_voltage = fan53555_list_voltage,
	.enable = fan53555_enable,
	.disable = fan53555_disable,
	.is_enabled = fan53555_is_enabled,
	.set_mode = fan53555_set_mode,
	.get_mode = fan53555_get_mode,
};

/* For 00,01,03,05 options:
 * VOUT = 0.60V + NSELx * 10mV, from 0.60 to 1.23V.
 * For 04 option:
 * VOUT = 0.603V + NSELx * 12.826mV, from 0.603 to 1.411V.
 * For 13 option:
 * 13 option, its DIE ID is 0x00 and DIE_REV is 0x0F.
 * VOUT = 0.80V + NSELx * 10mV, from 0.80 to 1.43V.
 * */
static int fan53555_device_setup(struct fan53555_device_info *di,
				struct fan53555_platform_data *pdata)
{
	unsigned int reg, data, mask;

	/* Setup voltage control register */
	switch (pdata->sleep_vsel_id) {
	case FAN53555_VSEL_ID_0:
		di->sleep_reg = FAN53555_VSEL0;
		di->vol_reg = FAN53555_VSEL1;
		break;
	case FAN53555_VSEL_ID_1:
		di->sleep_reg = FAN53555_VSEL1;
		di->vol_reg = FAN53555_VSEL0;
		break;
	default:
		dev_err(di->dev, "Invalid VSEL ID!\n");
		return -EINVAL;
	}
	/* Init voltage range and step */
	switch (di->chip_id) {
	case FAN53555_CHIP_ID_00:
		if (di->chip_rev == DIE_13_REV) {
			di->vsel_min = 800000;
			di->vsel_step = 10000;
			break;
		}
	case FAN53555_CHIP_ID_01:
	case FAN53555_CHIP_ID_03:
	case FAN53555_CHIP_ID_05:
=======
static const unsigned int slew_rates[] = {
	64000,
	32000,
	16000,
	 8000,
	 4000,
	 2000,
	 1000,
	  500,
};

static const unsigned int tcs_slew_rates[] = {
	18700,
	 9300,
	 4600,
	 2300,
};

static const struct regulator_ops fan53555_regulator_ops = {
	.set_voltage_sel = regulator_set_voltage_sel_regmap,
	.get_voltage_sel = regulator_get_voltage_sel_regmap,
	.set_voltage_time_sel = regulator_set_voltage_time_sel,
	.map_voltage = regulator_map_voltage_linear,
	.list_voltage = regulator_list_voltage_linear,
	.set_suspend_voltage = fan53555_set_suspend_voltage,
	.enable = regulator_enable_regmap,
	.disable = regulator_disable_regmap,
	.is_enabled = regulator_is_enabled_regmap,
	.set_mode = fan53555_set_mode,
	.get_mode = fan53555_get_mode,
	.set_ramp_delay = regulator_set_ramp_delay_regmap,
	.set_suspend_enable = fan53555_set_suspend_enable,
	.set_suspend_disable = fan53555_set_suspend_disable,
};

static int fan53526_voltages_setup_fairchild(struct fan53555_device_info *di)
{
	/* Init voltage range and step */
	switch (di->chip_id) {
	case FAN53526_CHIP_ID_01:
		switch (di->chip_rev) {
		case FAN53526_CHIP_REV_08:
			di->vsel_min = 600000;
			di->vsel_step = 6250;
			break;
		default:
			dev_err(di->dev,
				"Chip ID %d with rev %d not supported!\n",
				di->chip_id, di->chip_rev);
			return -EINVAL;
		}
		break;
	default:
		dev_err(di->dev,
			"Chip ID %d not supported!\n", di->chip_id);
		return -EINVAL;
	}

	di->slew_reg = FAN53555_CONTROL;
	di->slew_mask = CTL_SLEW_MASK;
	di->ramp_delay_table = slew_rates;
	di->n_ramp_values = ARRAY_SIZE(slew_rates);
	di->vsel_count = FAN53526_NVOLTAGES;

	return 0;
}

static int fan53555_voltages_setup_fairchild(struct fan53555_device_info *di)
{
	/* Init voltage range and step */
	switch (di->chip_id) {
	case FAN53555_CHIP_ID_00:
		switch (di->chip_rev) {
		case FAN53555_CHIP_REV_00:
			di->vsel_min = 600000;
			di->vsel_step = 10000;
			break;
		case FAN53555_CHIP_REV_13:
			di->vsel_min = 800000;
			di->vsel_step = 10000;
			break;
		default:
			dev_err(di->dev,
				"Chip ID %d with rev %d not supported!\n",
				di->chip_id, di->chip_rev);
			return -EINVAL;
		}
		break;
	case FAN53555_CHIP_ID_01:
	case FAN53555_CHIP_ID_03:
	case FAN53555_CHIP_ID_05:
	case FAN53555_CHIP_ID_08:
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		di->vsel_min = 600000;
		di->vsel_step = 10000;
		break;
	case FAN53555_CHIP_ID_04:
		di->vsel_min = 603000;
		di->vsel_step = 12826;
		break;
	default:
		dev_err(di->dev,
<<<<<<< HEAD
			"Chip ID[%d]\n not supported!\n", di->chip_id);
		return -EINVAL;
	}
	/* Init slew rate */
	if (pdata->slew_rate & 0x7)
		di->slew_rate = pdata->slew_rate;
	else
		di->slew_rate = FAN53555_SLEW_RATE_64MV;
	reg = FAN53555_CONTROL;
	data = di->slew_rate << CTL_SLEW_SHIFT;
	mask = CTL_SLEW_MASK;
	return regmap_update_bits(di->regmap, reg, mask, data);
}

static int fan53555_regulator_register(struct fan53555_device_info *di,
					struct i2c_client *client)
{
	struct regulator_desc *rdesc = &di->desc;

	rdesc->name = "fan53555-reg";
	if (di->disable_suspend)
		rdesc->ops = &fan53555_regulator_disable_suspend_ops;
	else
		rdesc->ops = &fan53555_regulator_ops;
	rdesc->type = REGULATOR_VOLTAGE;
	rdesc->n_voltages = FAN53555_NVOLTAGES;
	rdesc->owner = THIS_MODULE;

	di->rdev = regulator_register(&di->desc, di->dev,
			di->regulator, di, client->dev.of_node);
	return PTR_RET(di->rdev);

}

static struct regmap_config fan53555_regmap_config = {
	.reg_bits = 8,
	.val_bits = 8,
};

static int fan53555_parse_backup_reg(struct i2c_client *client, u32 *sleep_sel)
{
	int rc = -EINVAL;

	rc = of_property_read_u32(client->dev.of_node, "fairchild,backup-vsel",
				sleep_sel);
	if (rc) {
		dev_err(&client->dev, "fairchild,backup-vsel property missing\n");
	} else {
		switch (*sleep_sel) {
		case FAN53555_VSEL_ID_0:
		case FAN53555_VSEL_ID_1:
			break;
		default:
			dev_err(&client->dev, "Invalid VSEL ID!\n");
			rc = -EINVAL;
		}
	}

	return rc;
}

static u32 fan53555_get_slew_rate_reg_value(struct i2c_client *client,
					u32 slew_rate)
{
	u32 index;

	for (index = 0; index < ARRAY_SIZE(slew_rate_plan); index++)
		if (slew_rate == slew_rate_plan[index])
			break;

	if (index == ARRAY_SIZE(slew_rate_plan)) {
		dev_err(&client->dev, "invalid slew rate.\n");
		index = FAN53555_SLEW_RATE_8MV;
	}

	return index;
}

static struct fan53555_platform_data *
	fan53555_get_of_platform_data(struct i2c_client *client)
{
	struct fan53555_platform_data *pdata = NULL;
	struct regulator_init_data *init_data;
	u32 sleep_sel, slew_rate;
	int rc;

	init_data = of_get_regulator_init_data(&client->dev,
			client->dev.of_node);
	if (!init_data) {
		dev_err(&client->dev, "regulator init data is missing\n");
		return pdata;
	}

	rc = of_property_read_u32(client->dev.of_node, "regulator-ramp-delay",
					&slew_rate);
	if (rc)
		slew_rate = slew_rate_plan[FAN53555_SLEW_RATE_8MV];

	if (fan53555_parse_backup_reg(client, &sleep_sel))
		return pdata;

	pdata = devm_kzalloc(&client->dev,
			sizeof(struct fan53555_platform_data), GFP_KERNEL);
	if (!pdata) {
		dev_err(&client->dev,
			"fan53555_platform_data allocation failed.\n");
		return pdata;
	}

	init_data->constraints.input_uV = init_data->constraints.max_uV;
	init_data->constraints.valid_ops_mask |=
		REGULATOR_CHANGE_STATUS	| REGULATOR_CHANGE_VOLTAGE |
		REGULATOR_CHANGE_MODE;
	init_data->constraints.valid_modes_mask =
				REGULATOR_MODE_NORMAL |
				REGULATOR_MODE_FAST;
	init_data->constraints.initial_mode = REGULATOR_MODE_NORMAL;

	pdata->regulator = init_data;
	pdata->slew_rate = fan53555_get_slew_rate_reg_value(client,
							slew_rate);
	pdata->sleep_vsel_id = sleep_sel;

	return pdata;
}

static int fan53555_restore_working_reg(struct device_node *node,
			struct fan53555_device_info *di)
{
	int ret;
	u32 val;

	/* Restore register from back up register */
	ret = regmap_read(di->regmap, di->sleep_reg, &val);
	if (ret < 0) {
		dev_err(di->dev,
			"Failed to get backup data from reg %d, ret = %d\n",
			di->sleep_reg, ret);
		return ret;
	}

	ret = regmap_update_bits(di->regmap,
		di->vol_reg, VSEL_FULL_MASK, val);
	if (ret < 0) {
		dev_err(di->dev,
			"Failed to update working reg %d, ret = %d\n",
			di->vol_reg, ret);
		return ret;
=======
			"Chip ID %d not supported!\n", di->chip_id);
		return -EINVAL;
	}
	di->slew_reg = FAN53555_CONTROL;
	di->slew_mask = CTL_SLEW_MASK;
	di->ramp_delay_table = slew_rates;
	di->n_ramp_values = ARRAY_SIZE(slew_rates);
	di->vsel_count = FAN53555_NVOLTAGES;

	return 0;
}

static int fan53555_voltages_setup_rockchip(struct fan53555_device_info *di)
{
	/* Init voltage range and step */
	switch (di->chip_id) {
	case RK8600_CHIP_ID_08:
		di->vsel_min = 712500;
		di->vsel_step = 12500;
		break;
	default:
		dev_err(di->dev,
			"Chip ID %d not supported!\n", di->chip_id);
		return -EINVAL;
	}
	di->slew_reg = FAN53555_CONTROL;
	di->slew_mask = CTL_SLEW_MASK;
	di->ramp_delay_table = slew_rates;
	di->n_ramp_values = ARRAY_SIZE(slew_rates);
	di->vsel_count = FAN53555_NVOLTAGES;

	return 0;
}

static int rk8602_voltages_setup_rockchip(struct fan53555_device_info *di)
{
	/* Init voltage range and step */
	switch (di->chip_id) {
	case RK8602_CHIP_ID_10:
		di->vsel_min = 500000;
		di->vsel_step = 6250;
		break;
	default:
		dev_err(di->dev,
			"Chip ID %d not supported!\n", di->chip_id);
		return -EINVAL;
	}
	di->slew_reg = FAN53555_CONTROL;
	di->slew_mask = CTL_SLEW_MASK;
	di->ramp_delay_table = slew_rates;
	di->n_ramp_values = ARRAY_SIZE(slew_rates);
	di->vsel_count = RK8602_NVOLTAGES;

	return 0;
}

static int fan53555_voltages_setup_silergy(struct fan53555_device_info *di)
{
	/* Init voltage range and step */
	switch (di->chip_id) {
	case SILERGY_SYR82X:
	case SILERGY_SYR83X:
		di->vsel_min = 712500;
		di->vsel_step = 12500;
		break;
	default:
		dev_err(di->dev,
			"Chip ID %d not supported!\n", di->chip_id);
		return -EINVAL;
	}
	di->slew_reg = FAN53555_CONTROL;
	di->slew_mask = CTL_SLEW_MASK;
	di->ramp_delay_table = slew_rates;
	di->n_ramp_values = ARRAY_SIZE(slew_rates);
	di->vsel_count = FAN53555_NVOLTAGES;

	return 0;
}

static int fan53526_voltages_setup_tcs(struct fan53555_device_info *di)
{
	switch (di->chip_id) {
	case TCS4525_CHIP_ID_12:
	case TCS4526_CHIP_ID_00:
		di->slew_reg = TCS4525_TIME;
		di->slew_mask = TCS_SLEW_MASK;
		di->ramp_delay_table = tcs_slew_rates;
		di->n_ramp_values = ARRAY_SIZE(tcs_slew_rates);

		/* Init voltage range and step */
		di->vsel_min = 600000;
		di->vsel_step = 6250;
		di->vsel_count = FAN53526_NVOLTAGES;
		break;
	default:
		dev_err(di->dev, "Chip ID %d not supported!\n", di->chip_id);
		return -EINVAL;
	}

	return 0;
}

/* For 00,01,03,05 options:
 * VOUT = 0.60V + NSELx * 10mV, from 0.60 to 1.23V.
 * For 04 option:
 * VOUT = 0.603V + NSELx * 12.826mV, from 0.603 to 1.411V.
 * */
static int fan53555_device_setup(struct fan53555_device_info *di,
				struct fan53555_platform_data *pdata)
{
	int ret = 0;

	/* Setup voltage control register */
	switch (di->vendor) {
	case FAN53526_VENDOR_FAIRCHILD:
	case FAN53555_VENDOR_FAIRCHILD:
	case FAN53555_VENDOR_ROCKCHIP:
	case FAN53555_VENDOR_SILERGY:
		switch (pdata->sleep_vsel_id) {
		case FAN53555_VSEL_ID_0:
			di->sleep_reg = FAN53555_VSEL0;
			di->vol_reg = FAN53555_VSEL1;
			break;
		case FAN53555_VSEL_ID_1:
			di->sleep_reg = FAN53555_VSEL1;
			di->vol_reg = FAN53555_VSEL0;
			break;
		default:
			dev_err(di->dev, "Invalid VSEL ID!\n");
			return -EINVAL;
		}
		di->sleep_en_reg = di->sleep_reg;
		di->en_reg = di->vol_reg;
		break;
	case RK8602_VENDOR_ROCKCHIP:
		switch (pdata->sleep_vsel_id) {
		case FAN53555_VSEL_ID_0:
			di->sleep_reg = RK8602_VSEL0;
			di->vol_reg = RK8602_VSEL1;
			di->sleep_en_reg = FAN53555_VSEL0;
			di->en_reg = FAN53555_VSEL1;
			break;
		case FAN53555_VSEL_ID_1:
			di->sleep_reg = RK8602_VSEL1;
			di->vol_reg = RK8602_VSEL0;
			di->sleep_en_reg = FAN53555_VSEL1;
			di->en_reg = FAN53555_VSEL0;
			break;
		default:
			dev_err(di->dev, "Invalid VSEL ID!\n");
			return -EINVAL;
		}
		break;
	case FAN53526_VENDOR_TCS:
		switch (pdata->sleep_vsel_id) {
		case FAN53555_VSEL_ID_0:
			di->sleep_reg = TCS4525_VSEL0;
			di->vol_reg = TCS4525_VSEL1;
			break;
		case FAN53555_VSEL_ID_1:
			di->sleep_reg = TCS4525_VSEL1;
			di->vol_reg = TCS4525_VSEL0;
			break;
		default:
			dev_err(di->dev, "Invalid VSEL ID!\n");
			return -EINVAL;
		}
		di->sleep_en_reg = di->sleep_reg;
		di->en_reg = di->vol_reg;
		break;
	default:
		dev_err(di->dev, "vendor %d not supported!\n", di->vendor);
		return -EINVAL;
	}

	/* Setup mode control register */
	switch (di->vendor) {
	case FAN53526_VENDOR_FAIRCHILD:
		di->mode_reg = FAN53555_CONTROL;

		switch (pdata->sleep_vsel_id) {
		case FAN53555_VSEL_ID_0:
			di->mode_mask = CTL_MODE_VSEL1_MODE;
			break;
		case FAN53555_VSEL_ID_1:
			di->mode_mask = CTL_MODE_VSEL0_MODE;
			break;
		}
		break;
	case FAN53555_VENDOR_FAIRCHILD:
	case FAN53555_VENDOR_ROCKCHIP:
	case FAN53555_VENDOR_SILERGY:
		di->mode_reg = di->vol_reg;
		di->mode_mask = VSEL_MODE;
		break;
	case RK8602_VENDOR_ROCKCHIP:
		di->mode_mask = VSEL_MODE;

		switch (pdata->sleep_vsel_id) {
		case FAN53555_VSEL_ID_0:
			di->mode_reg = FAN53555_VSEL1;
			break;
		case FAN53555_VSEL_ID_1:
			di->mode_reg = FAN53555_VSEL0;
			break;
		}
		break;
	case FAN53526_VENDOR_TCS:
		di->mode_reg = TCS4525_COMMAND;

		switch (pdata->sleep_vsel_id) {
		case FAN53555_VSEL_ID_0:
			di->mode_mask = TCS_VSEL1_MODE;
			break;
		case FAN53555_VSEL_ID_1:
			di->mode_mask = TCS_VSEL0_MODE;
			break;
		}
		break;
	default:
		dev_err(di->dev, "vendor %d not supported!\n", di->vendor);
		return -EINVAL;
	}

	/* Setup voltage range */
	switch (di->vendor) {
	case FAN53526_VENDOR_FAIRCHILD:
		ret = fan53526_voltages_setup_fairchild(di);
		break;
	case FAN53555_VENDOR_FAIRCHILD:
		ret = fan53555_voltages_setup_fairchild(di);
		break;
	case FAN53555_VENDOR_ROCKCHIP:
		ret = fan53555_voltages_setup_rockchip(di);
		break;
	case RK8602_VENDOR_ROCKCHIP:
		ret = rk8602_voltages_setup_rockchip(di);
		break;
	case FAN53555_VENDOR_SILERGY:
		ret = fan53555_voltages_setup_silergy(di);
		break;
	case FAN53526_VENDOR_TCS:
		ret = fan53526_voltages_setup_tcs(di);
		break;
	default:
		dev_err(di->dev, "vendor %d not supported!\n", di->vendor);
		return -EINVAL;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	}

	return ret;
}

<<<<<<< HEAD
static int fan53555_of_init(struct device_node *node,
			struct fan53555_device_info *di)
{
	int ret, gpio;
	enum of_gpio_flags flags;

	if (of_property_read_bool(node, "fairchild,restore-reg")) {
		ret = fan53555_restore_working_reg(node, di);
		if (ret)
			return ret;
	}

	if (of_find_property(node, "fairchild,vsel-gpio", NULL)) {
		gpio = of_get_named_gpio_flags(node, "fairchild,vsel-gpio", 0,
						&flags);

		if (!gpio_is_valid(gpio)) {
			if (gpio != -EPROBE_DEFER)
				dev_err(di->dev, "Could not get vsel, ret = %d\n",
					gpio);
			return gpio;
		}

		ret = devm_gpio_request(di->dev, gpio, "fan53555_vsel");
		if (ret) {
			dev_err(di->dev, "Failed to obtain gpio %d ret = %d\n",
				gpio, ret);
			return ret;
		}

		ret = gpio_direction_output(gpio, flags & OF_GPIO_ACTIVE_LOW ?
							0 : 1);
		if (ret) {
			dev_err(di->dev,
				"Failed to set GPIO %d to: %s, ret = %d",
				gpio, flags & OF_GPIO_ACTIVE_LOW ?
				"GPIO_LOW" : "GPIO_HIGH", ret);
			return ret;
		}
	}

	di->disable_suspend = of_property_read_bool(node,
				"fairchild,disable-suspend");

	return 0;
}

static int __devinit fan53555_regulator_probe(struct i2c_client *client,
				const struct i2c_device_id *id)
{
	struct fan53555_device_info *di;
	struct fan53555_platform_data *pdata;
	unsigned int val;
	int ret;

	if (client->dev.of_node)
		pdata = fan53555_get_of_platform_data(client);
	else
		pdata = client->dev.platform_data;

	if (!pdata || !pdata->regulator) {
		dev_err(&client->dev, "Platform data not found!\n");
		return -ENODEV;
	}

	di = devm_kzalloc(&client->dev, sizeof(struct fan53555_device_info),
					GFP_KERNEL);
	if (!di) {
		dev_err(&client->dev, "Failed to allocate device info data!\n");
		return -ENOMEM;
	}
	di->regmap = devm_regmap_init_i2c(client, &fan53555_regmap_config);
	if (IS_ERR(di->regmap)) {
		dev_err(&client->dev, "Failed to allocate regmap!\n");
		return PTR_ERR(di->regmap);
	}
	di->dev = &client->dev;
	di->regulator = pdata->regulator;
	i2c_set_clientdata(client, di);
	/* Get chip ID */
	ret = regmap_read(di->regmap, FAN53555_ID1, &val);
	if (ret < 0) {
		dev_err(&client->dev, "Failed to get chip ID!\n");
		return -ENODEV;
	}
	di->chip_id = val & DIE_ID;
	/* Get chip revision */
	ret = regmap_read(di->regmap, FAN53555_ID2, &val);
	if (ret < 0) {
		dev_err(&client->dev, "Failed to get chip Rev!\n");
		return -ENODEV;
	}
=======
static int fan53555_regulator_register(struct fan53555_device_info *di,
			struct regulator_config *config)
{
	struct regulator_desc *rdesc = &di->desc;
	struct regulator_dev *rdev;

	rdesc->name = "fan53555-reg";
	rdesc->supply_name = "vin";
	rdesc->ops = &fan53555_regulator_ops;
	rdesc->type = REGULATOR_VOLTAGE;
	rdesc->n_voltages = di->vsel_count;
	rdesc->enable_reg = di->en_reg;
	rdesc->enable_mask = VSEL_BUCK_EN;
	rdesc->min_uV = di->vsel_min;
	rdesc->uV_step = di->vsel_step;
	rdesc->vsel_reg = di->vol_reg;
	rdesc->vsel_mask = BIT(fls(di->vsel_count - 1)) - 1;
	rdesc->ramp_reg = di->slew_reg;
	rdesc->ramp_mask = di->slew_mask;
	rdesc->ramp_delay_table = di->ramp_delay_table;
	rdesc->n_ramp_values = di->n_ramp_values;
	rdesc->owner = THIS_MODULE;

	rdev = devm_regulator_register(di->dev, &di->desc, config);
	return PTR_ERR_OR_ZERO(rdev);
}

static const struct regmap_config fan53555_regmap_config = {
	.reg_bits = 8,
	.val_bits = 8,
};

static struct fan53555_platform_data *fan53555_parse_dt(struct device *dev,
					      struct device_node *np,
					      const struct regulator_desc *desc)
{
	struct fan53555_platform_data *pdata;
	int ret;
	u32 tmp;

	pdata = devm_kzalloc(dev, sizeof(*pdata), GFP_KERNEL);
	if (!pdata)
		return NULL;

	pdata->regulator = of_get_regulator_init_data(dev, np, desc);

	ret = of_property_read_u32(np, "fcs,suspend-voltage-selector",
				   &tmp);
	if (!ret)
		pdata->sleep_vsel_id = tmp;

	return pdata;
}

static const struct of_device_id __maybe_unused fan53555_dt_ids[] = {
	{
		.compatible = "fcs,fan53526",
		.data = (void *)FAN53526_VENDOR_FAIRCHILD,
	}, {
		.compatible = "fcs,fan53555",
		.data = (void *)FAN53555_VENDOR_FAIRCHILD
	}, {
		.compatible = "rockchip,rk8600",
		.data = (void *)FAN53555_VENDOR_ROCKCHIP
	}, {
		.compatible = "rockchip,rk8602",
		.data = (void *)RK8602_VENDOR_ROCKCHIP
	}, {
		.compatible = "silergy,syr827",
		.data = (void *)FAN53555_VENDOR_SILERGY,
	}, {
		.compatible = "silergy,syr828",
		.data = (void *)FAN53555_VENDOR_SILERGY,
	}, {
		.compatible = "tcs,tcs4525",
		.data = (void *)FAN53526_VENDOR_TCS
	}, {
		.compatible = "tcs,tcs4526",
		.data = (void *)FAN53526_VENDOR_TCS
	},
	{ }
};
MODULE_DEVICE_TABLE(of, fan53555_dt_ids);

static int fan53555_regulator_probe(struct i2c_client *client)
{
	struct device_node *np = client->dev.of_node;
	struct fan53555_device_info *di;
	struct fan53555_platform_data *pdata;
	struct regulator_config config = { };
	struct regmap *regmap;
	unsigned int val;
	int ret;

	di = devm_kzalloc(&client->dev, sizeof(struct fan53555_device_info),
					GFP_KERNEL);
	if (!di)
		return -ENOMEM;

	pdata = dev_get_platdata(&client->dev);
	if (!pdata)
		pdata = fan53555_parse_dt(&client->dev, np, &di->desc);

	if (!pdata || !pdata->regulator)
		return dev_err_probe(&client->dev, -ENODEV,
				     "Platform data not found!\n");

	di->regulator = pdata->regulator;
	di->vendor = (uintptr_t)i2c_get_match_data(client);
	if (!dev_fwnode(&client->dev)) {
		/* if no ramp constraint set, get the pdata ramp_delay */
		if (!di->regulator->constraints.ramp_delay) {
			if (pdata->slew_rate >= ARRAY_SIZE(slew_rates))
				return dev_err_probe(&client->dev, -EINVAL,
						     "Invalid slew_rate\n");

			di->regulator->constraints.ramp_delay
					= slew_rates[pdata->slew_rate];
		}
	}

	regmap = devm_regmap_init_i2c(client, &fan53555_regmap_config);
	if (IS_ERR(regmap))
		return dev_err_probe(&client->dev, PTR_ERR(regmap),
				     "Failed to allocate regmap!\n");

	di->dev = &client->dev;
	i2c_set_clientdata(client, di);
	/* Get chip ID */
	ret = regmap_read(regmap, FAN53555_ID1, &val);
	if (ret < 0)
		return dev_err_probe(&client->dev, ret, "Failed to get chip ID!\n");

	di->chip_id = val & DIE_ID;
	/* Get chip revision */
	ret = regmap_read(regmap, FAN53555_ID2, &val);
	if (ret < 0)
		return dev_err_probe(&client->dev, ret, "Failed to get chip Rev!\n");

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	di->chip_rev = val & DIE_REV;
	dev_info(&client->dev, "FAN53555 Option[%d] Rev[%d] Detected!\n",
				di->chip_id, di->chip_rev);
	/* Device init */
	ret = fan53555_device_setup(di, pdata);
<<<<<<< HEAD
	if (ret < 0) {
		dev_err(&client->dev, "Failed to setup device!\n");
		return ret;
	}

	/* Set up from device tree */
	if (client->dev.of_node) {
		ret = fan53555_of_init(client->dev.of_node, di);
		if (ret)
			return ret;
	}

	ret = fan53555_regulator_register(di, client);
	if (ret < 0)
		dev_err(&client->dev, "Failed to register regulator!\n");

	return ret;

}

static int __devexit fan53555_regulator_remove(struct i2c_client *client)
{
	struct fan53555_device_info *di = i2c_get_clientdata(client);

	regulator_unregister(di->rdev);
	return 0;
}

static struct of_device_id fan53555_match_table[] = {
	{ .compatible = "fairchild,fan53555-regulator",},
	{},
};
MODULE_DEVICE_TABLE(of, fan53555_match_table);

static const struct i2c_device_id fan53555_id[] = {
	{"fan53555", -1},
	{ },
};
=======
	if (ret < 0)
		return dev_err_probe(&client->dev, ret, "Failed to setup device!\n");

	/* Register regulator */
	config.dev = di->dev;
	config.init_data = di->regulator;
	config.regmap = regmap;
	config.driver_data = di;
	config.of_node = np;

	ret = fan53555_regulator_register(di, &config);
	if (ret < 0)
		dev_err_probe(&client->dev, ret, "Failed to register regulator!\n");

	return ret;
}

static const struct i2c_device_id fan53555_id[] = {
	{
		.name = "fan53526",
		.driver_data = FAN53526_VENDOR_FAIRCHILD
	}, {
		.name = "fan53555",
		.driver_data = FAN53555_VENDOR_FAIRCHILD
	}, {
		.name = "rk8600",
		.driver_data = FAN53555_VENDOR_ROCKCHIP
	}, {
		.name = "rk8602",
		.driver_data = RK8602_VENDOR_ROCKCHIP
	}, {
		.name = "syr827",
		.driver_data = FAN53555_VENDOR_SILERGY
	}, {
		.name = "syr828",
		.driver_data = FAN53555_VENDOR_SILERGY
	}, {
		.name = "tcs4525",
		.driver_data = FAN53526_VENDOR_TCS
	}, {
		.name = "tcs4526",
		.driver_data = FAN53526_VENDOR_TCS
	},
	{ },
};
MODULE_DEVICE_TABLE(i2c, fan53555_id);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

static struct i2c_driver fan53555_regulator_driver = {
	.driver = {
		.name = "fan53555-regulator",
<<<<<<< HEAD
		.owner = THIS_MODULE,
		.of_match_table = fan53555_match_table,
	},
	.probe = fan53555_regulator_probe,
	.remove = __devexit_p(fan53555_regulator_remove),
	.id_table = fan53555_id,
};

/**
 * fan53555_regulator_init() - initialized fan53555 regulator driver
 * This function registers the fan53555 regulator platform driver.
 *
 * Returns 0 on success or errno on failure.
 */
int __init fan53555_regulator_init(void)
{
	static bool initialized;

	if (initialized)
		return 0;
	else
		initialized = true;

	return i2c_add_driver(&fan53555_regulator_driver);
}
EXPORT_SYMBOL(fan53555_regulator_init);
module_init(fan53555_regulator_init);

static void __exit fan53555_regulator_exit(void)
{
	i2c_del_driver(&fan53555_regulator_driver);
}
module_exit(fan53555_regulator_exit);
=======
		.probe_type = PROBE_PREFER_ASYNCHRONOUS,
		.of_match_table = of_match_ptr(fan53555_dt_ids),
	},
	.probe = fan53555_regulator_probe,
	.id_table = fan53555_id,
};

module_i2c_driver(fan53555_regulator_driver);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

MODULE_AUTHOR("Yunfan Zhang <yfzhang@marvell.com>");
MODULE_DESCRIPTION("FAN53555 regulator driver");
MODULE_LICENSE("GPL v2");
