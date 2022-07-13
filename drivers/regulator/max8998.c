<<<<<<< HEAD
/*
 * max8998.c - Voltage regulator driver for the Maxim 8998
 *
 *  Copyright (C) 2009-2010 Samsung Electronics
 *  Kyungmin Park <kyungmin.park@samsung.com>
 *  Marek Szyprowski <m.szyprowski@samsung.com>
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
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */
=======
// SPDX-License-Identifier: GPL-2.0+
//
// max8998.c - Voltage regulator driver for the Maxim 8998
//
//  Copyright (C) 2009-2010 Samsung Electronics
//  Kyungmin Park <kyungmin.park@samsung.com>
//  Marek Szyprowski <m.szyprowski@samsung.com>
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

#include <linux/module.h>
#include <linux/init.h>
#include <linux/i2c.h>
#include <linux/err.h>
<<<<<<< HEAD
#include <linux/gpio.h>
#include <linux/slab.h>
#include <linux/interrupt.h>
#include <linux/mutex.h>
#include <linux/delay.h>
#include <linux/platform_device.h>
#include <linux/regulator/driver.h>
=======
#include <linux/bits.h>
#include <linux/gpio/consumer.h>
#include <linux/slab.h>
#include <linux/interrupt.h>
#include <linux/mutex.h>
#include <linux/of.h>
#include <linux/platform_device.h>
#include <linux/regulator/driver.h>
#include <linux/regulator/of_regulator.h>
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#include <linux/mfd/max8998.h>
#include <linux/mfd/max8998-private.h>

struct max8998_data {
	struct device		*dev;
	struct max8998_dev	*iodev;
	int			num_regulators;
<<<<<<< HEAD
	struct regulator_dev	**rdev;
=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	u8                      buck1_vol[4]; /* voltages for selection */
	u8                      buck2_vol[2];
	unsigned int		buck1_idx; /* index to last changed voltage */
					   /* value in a set */
	unsigned int		buck2_idx;
<<<<<<< HEAD
};

struct voltage_map_desc {
	int min;
	int max;
	int step;
};

/* Voltage maps */
static const struct voltage_map_desc ldo23_voltage_map_desc = {
	.min = 800,	.step = 50,	.max = 1300,
};
static const struct voltage_map_desc ldo456711_voltage_map_desc = {
	.min = 1600,	.step = 100,	.max = 3600,
};
static const struct voltage_map_desc ldo8_voltage_map_desc = {
	.min = 3000,	.step = 100,	.max = 3600,
};
static const struct voltage_map_desc ldo9_voltage_map_desc = {
	.min = 2800,	.step = 100,	.max = 3100,
};
static const struct voltage_map_desc ldo10_voltage_map_desc = {
	.min = 950,	.step = 50,	.max = 1300,
};
static const struct voltage_map_desc ldo1213_voltage_map_desc = {
	.min = 800,	.step = 100,	.max = 3300,
};
static const struct voltage_map_desc ldo1415_voltage_map_desc = {
	.min = 1200,	.step = 100,	.max = 3300,
};
static const struct voltage_map_desc ldo1617_voltage_map_desc = {
	.min = 1600,	.step = 100,	.max = 3600,
};
static const struct voltage_map_desc buck12_voltage_map_desc = {
	.min = 750,	.step = 25,	.max = 1525,
};
static const struct voltage_map_desc buck3_voltage_map_desc = {
	.min = 1600,	.step = 100,	.max = 3600,
};
static const struct voltage_map_desc buck4_voltage_map_desc = {
	.min = 800,	.step = 100,	.max = 2300,
};

static const struct voltage_map_desc *ldo_voltage_map[] = {
	NULL,
	NULL,
	&ldo23_voltage_map_desc,	/* LDO2 */
	&ldo23_voltage_map_desc,	/* LDO3 */
	&ldo456711_voltage_map_desc,	/* LDO4 */
	&ldo456711_voltage_map_desc,	/* LDO5 */
	&ldo456711_voltage_map_desc,	/* LDO6 */
	&ldo456711_voltage_map_desc,	/* LDO7 */
	&ldo8_voltage_map_desc,		/* LDO8 */
	&ldo9_voltage_map_desc,		/* LDO9 */
	&ldo10_voltage_map_desc,	/* LDO10 */
	&ldo456711_voltage_map_desc,	/* LDO11 */
	&ldo1213_voltage_map_desc,	/* LDO12 */
	&ldo1213_voltage_map_desc,	/* LDO13 */
	&ldo1415_voltage_map_desc,	/* LDO14 */
	&ldo1415_voltage_map_desc,	/* LDO15 */
	&ldo1617_voltage_map_desc,	/* LDO16 */
	&ldo1617_voltage_map_desc,	/* LDO17 */
	&buck12_voltage_map_desc,	/* BUCK1 */
	&buck12_voltage_map_desc,	/* BUCK2 */
	&buck3_voltage_map_desc,	/* BUCK3 */
	&buck4_voltage_map_desc,	/* BUCK4 */
};

static int max8998_list_voltage(struct regulator_dev *rdev,
				unsigned int selector)
{
	const struct voltage_map_desc *desc;
	int ldo = rdev_get_id(rdev);
	int val;

	if (ldo >= ARRAY_SIZE(ldo_voltage_map))
		return -EINVAL;

	desc = ldo_voltage_map[ldo];
	if (desc == NULL)
		return -EINVAL;

	val = desc->min + desc->step * selector;
	if (val > desc->max)
		return -EINVAL;

	return val * 1000;
}

=======
	struct gpio_desc	*buck1_gpio1;
	struct gpio_desc	*buck1_gpio2;
	struct gpio_desc	*buck2_gpio;
};

static const unsigned int charger_current_table[] = {
	90000, 380000, 475000, 550000, 570000, 600000, 700000, 800000,
};

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
static int max8998_get_enable_register(struct regulator_dev *rdev,
					int *reg, int *shift)
{
	int ldo = rdev_get_id(rdev);

	switch (ldo) {
	case MAX8998_LDO2 ... MAX8998_LDO5:
		*reg = MAX8998_REG_ONOFF1;
		*shift = 3 - (ldo - MAX8998_LDO2);
		break;
	case MAX8998_LDO6 ... MAX8998_LDO13:
		*reg = MAX8998_REG_ONOFF2;
		*shift = 7 - (ldo - MAX8998_LDO6);
		break;
	case MAX8998_LDO14 ... MAX8998_LDO17:
		*reg = MAX8998_REG_ONOFF3;
		*shift = 7 - (ldo - MAX8998_LDO14);
		break;
	case MAX8998_BUCK1 ... MAX8998_BUCK4:
		*reg = MAX8998_REG_ONOFF1;
		*shift = 7 - (ldo - MAX8998_BUCK1);
		break;
	case MAX8998_EN32KHZ_AP ... MAX8998_ENVICHG:
		*reg = MAX8998_REG_ONOFF4;
		*shift = 7 - (ldo - MAX8998_EN32KHZ_AP);
		break;
	case MAX8998_ESAFEOUT1 ... MAX8998_ESAFEOUT2:
		*reg = MAX8998_REG_CHGR2;
		*shift = 7 - (ldo - MAX8998_ESAFEOUT1);
		break;
<<<<<<< HEAD
=======
	case MAX8998_CHARGER:
		*reg = MAX8998_REG_CHGR2;
		*shift = 0;
		break;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	default:
		return -EINVAL;
	}

	return 0;
}

static int max8998_ldo_is_enabled(struct regulator_dev *rdev)
{
	struct max8998_data *max8998 = rdev_get_drvdata(rdev);
	struct i2c_client *i2c = max8998->iodev->i2c;
	int ret, reg, shift = 8;
	u8 val;

	ret = max8998_get_enable_register(rdev, &reg, &shift);
	if (ret)
		return ret;

	ret = max8998_read_reg(i2c, reg, &val);
	if (ret)
		return ret;

	return val & (1 << shift);
}

<<<<<<< HEAD
=======
static int max8998_ldo_is_enabled_inverted(struct regulator_dev *rdev)
{
	return (!max8998_ldo_is_enabled(rdev));
}

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
static int max8998_ldo_enable(struct regulator_dev *rdev)
{
	struct max8998_data *max8998 = rdev_get_drvdata(rdev);
	struct i2c_client *i2c = max8998->iodev->i2c;
	int reg, shift = 8, ret;

	ret = max8998_get_enable_register(rdev, &reg, &shift);
	if (ret)
		return ret;

	return max8998_update_reg(i2c, reg, 1<<shift, 1<<shift);
}

static int max8998_ldo_disable(struct regulator_dev *rdev)
{
	struct max8998_data *max8998 = rdev_get_drvdata(rdev);
	struct i2c_client *i2c = max8998->iodev->i2c;
	int reg, shift = 8, ret;

	ret = max8998_get_enable_register(rdev, &reg, &shift);
	if (ret)
		return ret;

	return max8998_update_reg(i2c, reg, 0, 1<<shift);
}

static int max8998_get_voltage_register(struct regulator_dev *rdev,
				int *_reg, int *_shift, int *_mask)
{
	int ldo = rdev_get_id(rdev);
	struct max8998_data *max8998 = rdev_get_drvdata(rdev);
	int reg, shift = 0, mask = 0xff;

	switch (ldo) {
	case MAX8998_LDO2 ... MAX8998_LDO3:
		reg = MAX8998_REG_LDO2_LDO3;
		mask = 0xf;
		if (ldo == MAX8998_LDO2)
			shift = 4;
		else
			shift = 0;
		break;
	case MAX8998_LDO4 ... MAX8998_LDO7:
		reg = MAX8998_REG_LDO4 + (ldo - MAX8998_LDO4);
		break;
	case MAX8998_LDO8 ... MAX8998_LDO9:
		reg = MAX8998_REG_LDO8_LDO9;
		mask = 0xf;
		if (ldo == MAX8998_LDO8)
			shift = 4;
		else
			shift = 0;
		break;
	case MAX8998_LDO10 ... MAX8998_LDO11:
		reg = MAX8998_REG_LDO10_LDO11;
		if (ldo == MAX8998_LDO10) {
			shift = 5;
			mask = 0x7;
		} else {
			shift = 0;
			mask = 0x1f;
		}
		break;
	case MAX8998_LDO12 ... MAX8998_LDO17:
		reg = MAX8998_REG_LDO12 + (ldo - MAX8998_LDO12);
		break;
	case MAX8998_BUCK1:
		reg = MAX8998_REG_BUCK1_VOLTAGE1 + max8998->buck1_idx;
		break;
	case MAX8998_BUCK2:
		reg = MAX8998_REG_BUCK2_VOLTAGE1 + max8998->buck2_idx;
		break;
	case MAX8998_BUCK3:
		reg = MAX8998_REG_BUCK3;
		break;
	case MAX8998_BUCK4:
		reg = MAX8998_REG_BUCK4;
		break;
	default:
		return -EINVAL;
	}

	*_reg = reg;
	*_shift = shift;
	*_mask = mask;

	return 0;
}

<<<<<<< HEAD
static int max8998_get_voltage(struct regulator_dev *rdev)
=======
static int max8998_get_voltage_sel(struct regulator_dev *rdev)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
{
	struct max8998_data *max8998 = rdev_get_drvdata(rdev);
	struct i2c_client *i2c = max8998->iodev->i2c;
	int reg, shift = 0, mask, ret;
	u8 val;

	ret = max8998_get_voltage_register(rdev, &reg, &shift, &mask);
	if (ret)
		return ret;

	ret = max8998_read_reg(i2c, reg, &val);
	if (ret)
		return ret;

	val >>= shift;
	val &= mask;

<<<<<<< HEAD
	return max8998_list_voltage(rdev, val);
}

static int max8998_set_voltage_ldo(struct regulator_dev *rdev,
				   int min_uV, int max_uV, unsigned *selector)
{
	struct max8998_data *max8998 = rdev_get_drvdata(rdev);
	struct i2c_client *i2c = max8998->iodev->i2c;
	int min_vol = min_uV / 1000, max_vol = max_uV / 1000;
	const struct voltage_map_desc *desc;
	int ldo = rdev_get_id(rdev);
	int reg, shift = 0, mask, ret;
	int i = 0;

	if (ldo >= ARRAY_SIZE(ldo_voltage_map))
		return -EINVAL;

	desc = ldo_voltage_map[ldo];
	if (desc == NULL)
		return -EINVAL;

	if (max_vol < desc->min || min_vol > desc->max)
		return -EINVAL;

	while (desc->min + desc->step*i < min_vol &&
	       desc->min + desc->step*i < desc->max)
		i++;

	if (desc->min + desc->step*i > max_vol)
		return -EINVAL;

	*selector = i;
=======
	return val;
}

static int max8998_set_voltage_ldo_sel(struct regulator_dev *rdev,
				       unsigned selector)
{
	struct max8998_data *max8998 = rdev_get_drvdata(rdev);
	struct i2c_client *i2c = max8998->iodev->i2c;
	int reg, shift = 0, mask, ret;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	ret = max8998_get_voltage_register(rdev, &reg, &shift, &mask);
	if (ret)
		return ret;

<<<<<<< HEAD
	ret = max8998_update_reg(i2c, reg, i<<shift, mask<<shift);
=======
	ret = max8998_update_reg(i2c, reg, selector<<shift, mask<<shift);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	return ret;
}

<<<<<<< HEAD
static inline void buck1_gpio_set(int gpio1, int gpio2, int v)
{
	gpio_set_value(gpio1, v & 0x1);
	gpio_set_value(gpio2, (v >> 1) & 0x1);
}

static inline void buck2_gpio_set(int gpio, int v)
{
	gpio_set_value(gpio, v & 0x1);
}

static int max8998_set_voltage_buck(struct regulator_dev *rdev,
				    int min_uV, int max_uV, unsigned *selector)
{
	struct max8998_data *max8998 = rdev_get_drvdata(rdev);
	struct max8998_platform_data *pdata =
		dev_get_platdata(max8998->iodev->dev);
	struct i2c_client *i2c = max8998->iodev->i2c;
	int min_vol = min_uV / 1000, max_vol = max_uV / 1000;
	const struct voltage_map_desc *desc;
	int buck = rdev_get_id(rdev);
	int reg, shift = 0, mask, ret;
	int difference = 0, i = 0, j = 0, previous_vol = 0;
	u8 val = 0;
	static u8 buck1_last_val;

	if (buck >= ARRAY_SIZE(ldo_voltage_map))
		return -EINVAL;

	desc = ldo_voltage_map[buck];

	if (desc == NULL)
		return -EINVAL;

	if (max_vol < desc->min || min_vol > desc->max)
		return -EINVAL;

	while (desc->min + desc->step*i < min_vol &&
	       desc->min + desc->step*i < desc->max)
		i++;

	if (desc->min + desc->step*i > max_vol)
		return -EINVAL;

	*selector = i;

=======
static inline void buck1_gpio_set(struct gpio_desc *gpio1, struct gpio_desc *gpio2, int v)
{
	gpiod_set_value(gpio1, v & 0x1);
	gpiod_set_value(gpio2, (v >> 1) & 0x1);
}

static inline void buck2_gpio_set(struct gpio_desc *gpio, int v)
{
	gpiod_set_value(gpio, v & 0x1);
}

static int max8998_set_voltage_buck_sel(struct regulator_dev *rdev,
					unsigned selector)
{
	struct max8998_data *max8998 = rdev_get_drvdata(rdev);
	struct max8998_platform_data *pdata = max8998->iodev->pdata;
	struct i2c_client *i2c = max8998->iodev->i2c;
	int buck = rdev_get_id(rdev);
	int reg, shift = 0, mask, ret, j;
	static u8 buck1_last_val;

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	ret = max8998_get_voltage_register(rdev, &reg, &shift, &mask);
	if (ret)
		return ret;

<<<<<<< HEAD
	previous_vol = max8998_get_voltage(rdev);

	/* Check if voltage needs to be changed */
	/* if previous_voltage equal new voltage, return */
	if (previous_vol == max8998_list_voltage(rdev, i)) {
		dev_dbg(max8998->dev, "No voltage change, old:%d, new:%d\n",
			previous_vol, max8998_list_voltage(rdev, i));
		return ret;
	}

	switch (buck) {
	case MAX8998_BUCK1:
		dev_dbg(max8998->dev,
			"BUCK1, i:%d, buck1_vol1:%d, buck1_vol2:%d\n"
			"buck1_vol3:%d, buck1_vol4:%d\n",
			i, max8998->buck1_vol[0], max8998->buck1_vol[1],
			max8998->buck1_vol[2], max8998->buck1_vol[3]);

		if (gpio_is_valid(pdata->buck1_set1) &&
		    gpio_is_valid(pdata->buck1_set2)) {
=======
	switch (buck) {
	case MAX8998_BUCK1:
		dev_dbg(max8998->dev,
			"BUCK1, selector:%d, buck1_vol1:%d, buck1_vol2:%d\n"
			"buck1_vol3:%d, buck1_vol4:%d\n",
			selector, max8998->buck1_vol[0], max8998->buck1_vol[1],
			max8998->buck1_vol[2], max8998->buck1_vol[3]);

		if (max8998->buck1_gpio1 && max8998->buck1_gpio2) {
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

			/* check if requested voltage */
			/* value is already defined */
			for (j = 0; j < ARRAY_SIZE(max8998->buck1_vol); j++) {
<<<<<<< HEAD
				if (max8998->buck1_vol[j] == i) {
					max8998->buck1_idx = j;
					buck1_gpio_set(pdata->buck1_set1,
						       pdata->buck1_set2, j);
=======
				if (max8998->buck1_vol[j] == selector) {
					max8998->buck1_idx = j;
					buck1_gpio_set(max8998->buck1_gpio1,
						       max8998->buck1_gpio2, j);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
					goto buck1_exit;
				}
			}

			if (pdata->buck_voltage_lock)
				return -EINVAL;

			/* no predefine regulator found */
			max8998->buck1_idx = (buck1_last_val % 2) + 2;
			dev_dbg(max8998->dev, "max8998->buck1_idx:%d\n",
				max8998->buck1_idx);
<<<<<<< HEAD
			max8998->buck1_vol[max8998->buck1_idx] = i;
			ret = max8998_get_voltage_register(rdev, &reg,
							   &shift,
							   &mask);
			ret = max8998_write_reg(i2c, reg, i);
			buck1_gpio_set(pdata->buck1_set1,
				       pdata->buck1_set2, max8998->buck1_idx);
			buck1_last_val++;
buck1_exit:
			dev_dbg(max8998->dev, "%s: SET1:%d, SET2:%d\n",
				i2c->name, gpio_get_value(pdata->buck1_set1),
				gpio_get_value(pdata->buck1_set2));
			break;
		} else {
			ret = max8998_write_reg(i2c, reg, i);
=======
			max8998->buck1_vol[max8998->buck1_idx] = selector;
			ret = max8998_get_voltage_register(rdev, &reg,
							   &shift,
							   &mask);
			ret = max8998_write_reg(i2c, reg, selector);
			buck1_gpio_set(max8998->buck1_gpio1,
				       max8998->buck1_gpio2, max8998->buck1_idx);
			buck1_last_val++;
buck1_exit:
			dev_dbg(max8998->dev, "%s: SET1:%d, SET2:%d\n",
				i2c->name, gpiod_get_value(max8998->buck1_gpio1),
				gpiod_get_value(max8998->buck1_gpio2));
			break;
		} else {
			ret = max8998_write_reg(i2c, reg, selector);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		}
		break;

	case MAX8998_BUCK2:
		dev_dbg(max8998->dev,
<<<<<<< HEAD
			"BUCK2, i:%d buck2_vol1:%d, buck2_vol2:%d\n"
			, i, max8998->buck2_vol[0], max8998->buck2_vol[1]);
		if (gpio_is_valid(pdata->buck2_set3)) {

			/* check if requested voltage */
			/* value is already defined */
			for (j = 0; j < ARRAY_SIZE(max8998->buck2_vol); j++) {
				if (max8998->buck2_vol[j] == i) {
					max8998->buck2_idx = j;
					buck2_gpio_set(pdata->buck2_set3, j);
=======
			"BUCK2, selector:%d buck2_vol1:%d, buck2_vol2:%d\n",
			selector, max8998->buck2_vol[0], max8998->buck2_vol[1]);
		if (max8998->buck2_gpio) {
			/* check if requested voltage */
			/* value is already defined */
			for (j = 0; j < ARRAY_SIZE(max8998->buck2_vol); j++) {
				if (max8998->buck2_vol[j] == selector) {
					max8998->buck2_idx = j;
					buck2_gpio_set(max8998->buck2_gpio, j);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
					goto buck2_exit;
				}
			}

			if (pdata->buck_voltage_lock)
				return -EINVAL;

			max8998_get_voltage_register(rdev,
					&reg, &shift, &mask);
<<<<<<< HEAD
			ret = max8998_write_reg(i2c, reg, i);
			max8998->buck2_vol[max8998->buck2_idx] = i;
			buck2_gpio_set(pdata->buck2_set3, max8998->buck2_idx);
buck2_exit:
			dev_dbg(max8998->dev, "%s: SET3:%d\n", i2c->name,
				gpio_get_value(pdata->buck2_set3));
		} else {
			ret = max8998_write_reg(i2c, reg, i);
=======
			ret = max8998_write_reg(i2c, reg, selector);
			max8998->buck2_vol[max8998->buck2_idx] = selector;
			buck2_gpio_set(max8998->buck2_gpio, max8998->buck2_idx);
buck2_exit:
			dev_dbg(max8998->dev, "%s: SET3:%d\n", i2c->name,
				gpiod_get_value(max8998->buck2_gpio));
		} else {
			ret = max8998_write_reg(i2c, reg, selector);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		}
		break;

	case MAX8998_BUCK3:
	case MAX8998_BUCK4:
<<<<<<< HEAD
		ret = max8998_update_reg(i2c, reg, i<<shift, mask<<shift);
		break;
	}

	/* Voltage stabilization */
	max8998_read_reg(i2c, MAX8998_REG_ONOFF4, &val);

	/* lp3974 hasn't got ENRAMP bit - ramp is assumed as true */
	/* MAX8998 has ENRAMP bit implemented, so test it*/
	if (max8998->iodev->type == TYPE_MAX8998 && !(val & MAX8998_ENRAMP))
		return ret;

	difference = desc->min + desc->step*i - previous_vol/1000;
	if (difference > 0)
		udelay(DIV_ROUND_UP(difference, (val & 0x0f) + 1));

	return ret;
}

static struct regulator_ops max8998_ldo_ops = {
	.list_voltage		= max8998_list_voltage,
	.is_enabled		= max8998_ldo_is_enabled,
	.enable			= max8998_ldo_enable,
	.disable		= max8998_ldo_disable,
	.get_voltage		= max8998_get_voltage,
	.set_voltage		= max8998_set_voltage_ldo,
	.set_suspend_enable	= max8998_ldo_enable,
	.set_suspend_disable	= max8998_ldo_disable,
};

static struct regulator_ops max8998_buck_ops = {
	.list_voltage		= max8998_list_voltage,
	.is_enabled		= max8998_ldo_is_enabled,
	.enable			= max8998_ldo_enable,
	.disable		= max8998_ldo_disable,
	.get_voltage		= max8998_get_voltage,
	.set_voltage		= max8998_set_voltage_buck,
	.set_suspend_enable	= max8998_ldo_enable,
	.set_suspend_disable	= max8998_ldo_disable,
};

static struct regulator_ops max8998_others_ops = {
	.is_enabled		= max8998_ldo_is_enabled,
	.enable			= max8998_ldo_enable,
	.disable		= max8998_ldo_disable,
	.set_suspend_enable	= max8998_ldo_enable,
	.set_suspend_disable	= max8998_ldo_disable,
};

static struct regulator_desc regulators[] = {
	{
		.name		= "LDO2",
		.id		= MAX8998_LDO2,
		.ops		= &max8998_ldo_ops,
		.type		= REGULATOR_VOLTAGE,
		.owner		= THIS_MODULE,
	}, {
		.name		= "LDO3",
		.id		= MAX8998_LDO3,
		.ops		= &max8998_ldo_ops,
		.type		= REGULATOR_VOLTAGE,
		.owner		= THIS_MODULE,
	}, {
		.name		= "LDO4",
		.id		= MAX8998_LDO4,
		.ops		= &max8998_ldo_ops,
		.type		= REGULATOR_VOLTAGE,
		.owner		= THIS_MODULE,
	}, {
		.name		= "LDO5",
		.id		= MAX8998_LDO5,
		.ops		= &max8998_ldo_ops,
		.type		= REGULATOR_VOLTAGE,
		.owner		= THIS_MODULE,
	}, {
		.name		= "LDO6",
		.id		= MAX8998_LDO6,
		.ops		= &max8998_ldo_ops,
		.type		= REGULATOR_VOLTAGE,
		.owner		= THIS_MODULE,
	}, {
		.name		= "LDO7",
		.id		= MAX8998_LDO7,
		.ops		= &max8998_ldo_ops,
		.type		= REGULATOR_VOLTAGE,
		.owner		= THIS_MODULE,
	}, {
		.name		= "LDO8",
		.id		= MAX8998_LDO8,
		.ops		= &max8998_ldo_ops,
		.type		= REGULATOR_VOLTAGE,
		.owner		= THIS_MODULE,
	}, {
		.name		= "LDO9",
		.id		= MAX8998_LDO9,
		.ops		= &max8998_ldo_ops,
		.type		= REGULATOR_VOLTAGE,
		.owner		= THIS_MODULE,
	}, {
		.name		= "LDO10",
		.id		= MAX8998_LDO10,
		.ops		= &max8998_ldo_ops,
		.type		= REGULATOR_VOLTAGE,
		.owner		= THIS_MODULE,
	}, {
		.name		= "LDO11",
		.id		= MAX8998_LDO11,
		.ops		= &max8998_ldo_ops,
		.type		= REGULATOR_VOLTAGE,
		.owner		= THIS_MODULE,
	}, {
		.name		= "LDO12",
		.id		= MAX8998_LDO12,
		.ops		= &max8998_ldo_ops,
		.type		= REGULATOR_VOLTAGE,
		.owner		= THIS_MODULE,
	}, {
		.name		= "LDO13",
		.id		= MAX8998_LDO13,
		.ops		= &max8998_ldo_ops,
		.type		= REGULATOR_VOLTAGE,
		.owner		= THIS_MODULE,
	}, {
		.name		= "LDO14",
		.id		= MAX8998_LDO14,
		.ops		= &max8998_ldo_ops,
		.type		= REGULATOR_VOLTAGE,
		.owner		= THIS_MODULE,
	}, {
		.name		= "LDO15",
		.id		= MAX8998_LDO15,
		.ops		= &max8998_ldo_ops,
		.type		= REGULATOR_VOLTAGE,
		.owner		= THIS_MODULE,
	}, {
		.name		= "LDO16",
		.id		= MAX8998_LDO16,
		.ops		= &max8998_ldo_ops,
		.type		= REGULATOR_VOLTAGE,
		.owner		= THIS_MODULE,
	}, {
		.name		= "LDO17",
		.id		= MAX8998_LDO17,
		.ops		= &max8998_ldo_ops,
		.type		= REGULATOR_VOLTAGE,
		.owner		= THIS_MODULE,
	}, {
		.name		= "BUCK1",
		.id		= MAX8998_BUCK1,
		.ops		= &max8998_buck_ops,
		.type		= REGULATOR_VOLTAGE,
		.owner		= THIS_MODULE,
	}, {
		.name		= "BUCK2",
		.id		= MAX8998_BUCK2,
		.ops		= &max8998_buck_ops,
		.type		= REGULATOR_VOLTAGE,
		.owner		= THIS_MODULE,
	}, {
		.name		= "BUCK3",
		.id		= MAX8998_BUCK3,
		.ops		= &max8998_buck_ops,
		.type		= REGULATOR_VOLTAGE,
		.owner		= THIS_MODULE,
	}, {
		.name		= "BUCK4",
		.id		= MAX8998_BUCK4,
		.ops		= &max8998_buck_ops,
		.type		= REGULATOR_VOLTAGE,
		.owner		= THIS_MODULE,
	}, {
		.name		= "EN32KHz AP",
		.id		= MAX8998_EN32KHZ_AP,
		.ops		= &max8998_others_ops,
		.type		= REGULATOR_VOLTAGE,
		.owner		= THIS_MODULE,
	}, {
		.name		= "EN32KHz CP",
		.id		= MAX8998_EN32KHZ_CP,
		.ops		= &max8998_others_ops,
		.type		= REGULATOR_VOLTAGE,
		.owner		= THIS_MODULE,
	}, {
		.name		= "ENVICHG",
		.id		= MAX8998_ENVICHG,
		.ops		= &max8998_others_ops,
		.type		= REGULATOR_VOLTAGE,
		.owner		= THIS_MODULE,
	}, {
		.name		= "ESAFEOUT1",
		.id		= MAX8998_ESAFEOUT1,
		.ops		= &max8998_others_ops,
		.type		= REGULATOR_VOLTAGE,
		.owner		= THIS_MODULE,
	}, {
		.name		= "ESAFEOUT2",
		.id		= MAX8998_ESAFEOUT2,
		.ops		= &max8998_others_ops,
		.type		= REGULATOR_VOLTAGE,
		.owner		= THIS_MODULE,
	}
};

static __devinit int max8998_pmic_probe(struct platform_device *pdev)
{
	struct max8998_dev *iodev = dev_get_drvdata(pdev->dev.parent);
	struct max8998_platform_data *pdata = dev_get_platdata(iodev->dev);
	struct regulator_dev **rdev;
	struct max8998_data *max8998;
	struct i2c_client *i2c;
	int i, ret, size;
=======
		ret = max8998_update_reg(i2c, reg, selector<<shift,
					 mask<<shift);
		break;
	}

	return ret;
}

static int max8998_set_voltage_buck_time_sel(struct regulator_dev *rdev,
					     unsigned int old_selector,
					     unsigned int new_selector)
{
	struct max8998_data *max8998 = rdev_get_drvdata(rdev);
	struct i2c_client *i2c = max8998->iodev->i2c;
	int buck = rdev_get_id(rdev);
	u8 val = 0;
	int difference, ret;

	if (buck < MAX8998_BUCK1 || buck > MAX8998_BUCK4)
		return -EINVAL;

	/* Voltage stabilization */
	ret = max8998_read_reg(i2c, MAX8998_REG_ONOFF4, &val);
	if (ret)
		return ret;

	/* lp3974 hasn't got ENRAMP bit - ramp is assumed as true */
	/* MAX8998 has ENRAMP bit implemented, so test it*/
	if (max8998->iodev->type == TYPE_MAX8998 && !(val & MAX8998_ENRAMP))
		return 0;

	difference = (new_selector - old_selector) * rdev->desc->uV_step / 1000;
	if (difference > 0)
		return DIV_ROUND_UP(difference, (val & 0x0f) + 1);

	return 0;
}

static int max8998_set_current_limit(struct regulator_dev *rdev,
				     int min_uA, int max_uA)
{
	struct max8998_data *max8998 = rdev_get_drvdata(rdev);
	struct i2c_client *i2c = max8998->iodev->i2c;
	unsigned int n_currents = rdev->desc->n_current_limits;
	int i, sel = -1;

	if (n_currents == 0)
		return -EINVAL;

	if (rdev->desc->curr_table) {
		const unsigned int *curr_table = rdev->desc->curr_table;
		bool ascend = curr_table[n_currents - 1] > curr_table[0];

		/* search for closest to maximum */
		if (ascend) {
			for (i = n_currents - 1; i >= 0; i--) {
				if (min_uA <= curr_table[i] &&
				    curr_table[i] <= max_uA) {
					sel = i;
					break;
				}
			}
		} else {
			for (i = 0; i < n_currents; i++) {
				if (min_uA <= curr_table[i] &&
				    curr_table[i] <= max_uA) {
					sel = i;
					break;
				}
			}
		}
	}

	if (sel < 0)
		return -EINVAL;

	sel <<= ffs(rdev->desc->csel_mask) - 1;

	return max8998_update_reg(i2c, rdev->desc->csel_reg,
				  sel, rdev->desc->csel_mask);
}

static int max8998_get_current_limit(struct regulator_dev *rdev)
{
	struct max8998_data *max8998 = rdev_get_drvdata(rdev);
	struct i2c_client *i2c = max8998->iodev->i2c;
	u8 val;
	int ret;

	ret = max8998_read_reg(i2c, rdev->desc->csel_reg, &val);
	if (ret != 0)
		return ret;

	val &= rdev->desc->csel_mask;
	val >>= ffs(rdev->desc->csel_mask) - 1;

	if (rdev->desc->curr_table) {
		if (val >= rdev->desc->n_current_limits)
			return -EINVAL;

		return rdev->desc->curr_table[val];
	}

	return -EINVAL;
}

static const struct regulator_ops max8998_ldo_ops = {
	.list_voltage		= regulator_list_voltage_linear,
	.map_voltage		= regulator_map_voltage_linear,
	.is_enabled		= max8998_ldo_is_enabled,
	.enable			= max8998_ldo_enable,
	.disable		= max8998_ldo_disable,
	.get_voltage_sel	= max8998_get_voltage_sel,
	.set_voltage_sel	= max8998_set_voltage_ldo_sel,
};

static const struct regulator_ops max8998_buck_ops = {
	.list_voltage		= regulator_list_voltage_linear,
	.map_voltage		= regulator_map_voltage_linear,
	.is_enabled		= max8998_ldo_is_enabled,
	.enable			= max8998_ldo_enable,
	.disable		= max8998_ldo_disable,
	.get_voltage_sel	= max8998_get_voltage_sel,
	.set_voltage_sel	= max8998_set_voltage_buck_sel,
	.set_voltage_time_sel	= max8998_set_voltage_buck_time_sel,
};

static const struct regulator_ops max8998_charger_ops = {
	.set_current_limit	= max8998_set_current_limit,
	.get_current_limit	= max8998_get_current_limit,
	.is_enabled		= max8998_ldo_is_enabled_inverted,
	/* Swapped as register is inverted */
	.enable			= max8998_ldo_disable,
	.disable		= max8998_ldo_enable,
};

static const struct regulator_ops max8998_others_ops = {
	.is_enabled		= max8998_ldo_is_enabled,
	.enable			= max8998_ldo_enable,
	.disable		= max8998_ldo_disable,
};

#define MAX8998_LINEAR_REG(_name, _ops, _min, _step, _max) \
	{ \
		.name = #_name, \
		.id = MAX8998_##_name, \
		.ops = _ops, \
		.min_uV = (_min), \
		.uV_step = (_step), \
		.n_voltages = ((_max) - (_min)) / (_step) + 1, \
		.type = REGULATOR_VOLTAGE, \
		.owner = THIS_MODULE, \
	}

#define MAX8998_CURRENT_REG(_name, _ops, _table, _reg, _mask) \
	{ \
		.name = #_name, \
		.id = MAX8998_##_name, \
		.ops = _ops, \
		.curr_table = _table, \
		.n_current_limits = ARRAY_SIZE(_table), \
		.csel_reg = _reg, \
		.csel_mask = _mask, \
		.type = REGULATOR_CURRENT, \
		.owner = THIS_MODULE, \
	}

#define MAX8998_OTHERS_REG(_name, _id) \
	{ \
		.name = #_name, \
		.id = _id, \
		.ops = &max8998_others_ops, \
		.type = REGULATOR_VOLTAGE, \
		.owner = THIS_MODULE, \
	}

static const struct regulator_desc regulators[] = {
	MAX8998_LINEAR_REG(LDO2, &max8998_ldo_ops, 800000, 50000, 1300000),
	MAX8998_LINEAR_REG(LDO3, &max8998_ldo_ops, 800000, 50000, 1300000),
	MAX8998_LINEAR_REG(LDO4, &max8998_ldo_ops, 1600000, 100000, 3600000),
	MAX8998_LINEAR_REG(LDO5, &max8998_ldo_ops, 1600000, 100000, 3600000),
	MAX8998_LINEAR_REG(LDO6, &max8998_ldo_ops, 1600000, 100000, 3600000),
	MAX8998_LINEAR_REG(LDO7, &max8998_ldo_ops, 1600000, 100000, 3600000),
	MAX8998_LINEAR_REG(LDO8, &max8998_ldo_ops, 3000000, 100000, 3600000),
	MAX8998_LINEAR_REG(LDO9, &max8998_ldo_ops, 2800000, 100000, 3100000),
	MAX8998_LINEAR_REG(LDO10, &max8998_ldo_ops, 950000, 50000, 1300000),
	MAX8998_LINEAR_REG(LDO11, &max8998_ldo_ops, 1600000, 100000, 3600000),
	MAX8998_LINEAR_REG(LDO12, &max8998_ldo_ops, 800000, 100000, 3300000),
	MAX8998_LINEAR_REG(LDO13, &max8998_ldo_ops, 800000, 100000, 3300000),
	MAX8998_LINEAR_REG(LDO14, &max8998_ldo_ops, 1200000, 100000, 3300000),
	MAX8998_LINEAR_REG(LDO15, &max8998_ldo_ops, 1200000, 100000, 3300000),
	MAX8998_LINEAR_REG(LDO16, &max8998_ldo_ops, 1600000, 100000, 3600000),
	MAX8998_LINEAR_REG(LDO17, &max8998_ldo_ops, 1600000, 100000, 3600000),
	MAX8998_LINEAR_REG(BUCK1, &max8998_buck_ops, 750000, 25000, 1525000),
	MAX8998_LINEAR_REG(BUCK2, &max8998_buck_ops, 750000, 25000, 1525000),
	MAX8998_LINEAR_REG(BUCK3, &max8998_buck_ops, 1600000, 100000, 3600000),
	MAX8998_LINEAR_REG(BUCK4, &max8998_buck_ops, 800000, 100000, 2300000),
	MAX8998_OTHERS_REG(EN32KHz-AP, MAX8998_EN32KHZ_AP),
	MAX8998_OTHERS_REG(EN32KHz-CP, MAX8998_EN32KHZ_CP),
	MAX8998_OTHERS_REG(ENVICHG, MAX8998_ENVICHG),
	MAX8998_OTHERS_REG(ESAFEOUT1, MAX8998_ESAFEOUT1),
	MAX8998_OTHERS_REG(ESAFEOUT2, MAX8998_ESAFEOUT2),
	MAX8998_CURRENT_REG(CHARGER, &max8998_charger_ops,
			    charger_current_table, MAX8998_REG_CHGR1, 0x7),
};

static int max8998_pmic_dt_parse_pdata(struct max8998_dev *iodev,
					struct max8998_platform_data *pdata)
{
	struct device_node *pmic_np = iodev->dev->of_node;
	struct device_node *regulators_np, *reg_np;
	struct max8998_regulator_data *rdata;
	unsigned int i;
	int ret;

	regulators_np = of_get_child_by_name(pmic_np, "regulators");
	if (!regulators_np) {
		dev_err(iodev->dev, "could not find regulators sub-node\n");
		return -EINVAL;
	}

	/* count the number of regulators to be supported in pmic */
	pdata->num_regulators = of_get_child_count(regulators_np);

	rdata = devm_kcalloc(iodev->dev,
			     pdata->num_regulators, sizeof(*rdata),
			     GFP_KERNEL);
	if (!rdata) {
		of_node_put(regulators_np);
		return -ENOMEM;
	}

	pdata->regulators = rdata;
	for (i = 0; i < ARRAY_SIZE(regulators); ++i) {
		reg_np = of_get_child_by_name(regulators_np,
							regulators[i].name);
		if (!reg_np)
			continue;

		rdata->id = regulators[i].id;
		rdata->initdata = of_get_regulator_init_data(iodev->dev,
							     reg_np,
							     &regulators[i]);
		rdata->reg_node = reg_np;
		++rdata;
	}
	pdata->num_regulators = rdata - pdata->regulators;

	of_node_put(reg_np);
	of_node_put(regulators_np);

	pdata->buck_voltage_lock = of_property_read_bool(pmic_np, "max8998,pmic-buck-voltage-lock");

	ret = of_property_read_u32(pmic_np,
					"max8998,pmic-buck1-default-dvs-idx",
					&pdata->buck1_default_idx);
	if (!ret && pdata->buck1_default_idx >= 4) {
		pdata->buck1_default_idx = 0;
		dev_warn(iodev->dev, "invalid value for default dvs index, using 0 instead\n");
	}

	ret = of_property_read_u32(pmic_np,
					"max8998,pmic-buck2-default-dvs-idx",
					&pdata->buck2_default_idx);
	if (!ret && pdata->buck2_default_idx >= 2) {
		pdata->buck2_default_idx = 0;
		dev_warn(iodev->dev, "invalid value for default dvs index, using 0 instead\n");
	}

	ret = of_property_read_u32_array(pmic_np,
					"max8998,pmic-buck1-dvs-voltage",
					pdata->buck1_voltage,
					ARRAY_SIZE(pdata->buck1_voltage));
	if (ret) {
		dev_err(iodev->dev, "buck1 voltages not specified\n");
		return -EINVAL;
	}

	ret = of_property_read_u32_array(pmic_np,
					"max8998,pmic-buck2-dvs-voltage",
					pdata->buck2_voltage,
					ARRAY_SIZE(pdata->buck2_voltage));
	if (ret) {
		dev_err(iodev->dev, "buck2 voltages not specified\n");
		return -EINVAL;
	}

	return 0;
}

static int max8998_pmic_probe(struct platform_device *pdev)
{
	struct max8998_dev *iodev = dev_get_drvdata(pdev->dev.parent);
	struct max8998_platform_data *pdata = iodev->pdata;
	struct regulator_config config = { };
	struct regulator_dev *rdev;
	struct max8998_data *max8998;
	struct i2c_client *i2c;
	enum gpiod_flags flags;
	int i, ret;
	unsigned int v;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	if (!pdata) {
		dev_err(pdev->dev.parent, "No platform init data supplied\n");
		return -ENODEV;
	}

<<<<<<< HEAD
	max8998 = kzalloc(sizeof(struct max8998_data), GFP_KERNEL);
	if (!max8998)
		return -ENOMEM;

	size = sizeof(struct regulator_dev *) * pdata->num_regulators;
	max8998->rdev = kzalloc(size, GFP_KERNEL);
	if (!max8998->rdev) {
		kfree(max8998);
		return -ENOMEM;
	}

	rdev = max8998->rdev;
=======
	if (IS_ENABLED(CONFIG_OF) && iodev->dev->of_node) {
		ret = max8998_pmic_dt_parse_pdata(iodev, pdata);
		if (ret)
			return ret;
	}

	max8998 = devm_kzalloc(&pdev->dev, sizeof(struct max8998_data),
			       GFP_KERNEL);
	if (!max8998)
		return -ENOMEM;

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	max8998->dev = &pdev->dev;
	max8998->iodev = iodev;
	max8998->num_regulators = pdata->num_regulators;
	platform_set_drvdata(pdev, max8998);
	i2c = max8998->iodev->i2c;

	max8998->buck1_idx = pdata->buck1_default_idx;
	max8998->buck2_idx = pdata->buck2_default_idx;

<<<<<<< HEAD
	/* NOTE: */
	/* For unused GPIO NOT marked as -1 (thereof equal to 0)  WARN_ON */
	/* will be displayed */

	/* Check if MAX8998 voltage selection GPIOs are defined */
	if (gpio_is_valid(pdata->buck1_set1) &&
	    gpio_is_valid(pdata->buck1_set2)) {
		/* Check if SET1 is not equal to 0 */
		if (!pdata->buck1_set1) {
			printk(KERN_ERR "MAX8998 SET1 GPIO defined as 0 !\n");
			WARN_ON(!pdata->buck1_set1);
			ret = -EIO;
			goto err_free_mem;
		}
		/* Check if SET2 is not equal to 0 */
		if (!pdata->buck1_set2) {
			printk(KERN_ERR "MAX8998 SET2 GPIO defined as 0 !\n");
			WARN_ON(!pdata->buck1_set2);
			ret = -EIO;
			goto err_free_mem;
		}

		gpio_request(pdata->buck1_set1, "MAX8998 BUCK1_SET1");
		gpio_direction_output(pdata->buck1_set1,
				      max8998->buck1_idx & 0x1);


		gpio_request(pdata->buck1_set2, "MAX8998 BUCK1_SET2");
		gpio_direction_output(pdata->buck1_set2,
				      (max8998->buck1_idx >> 1) & 0x1);
		/* Set predefined value for BUCK1 register 1 */
		i = 0;
		while (buck12_voltage_map_desc.min +
		       buck12_voltage_map_desc.step*i
		       < (pdata->buck1_voltage1 / 1000))
			i++;
		max8998->buck1_vol[0] = i;
		ret = max8998_write_reg(i2c, MAX8998_REG_BUCK1_VOLTAGE1, i);
		if (ret)
			goto err_free_mem;

		/* Set predefined value for BUCK1 register 2 */
		i = 0;
		while (buck12_voltage_map_desc.min +
		       buck12_voltage_map_desc.step*i
		       < (pdata->buck1_voltage2 / 1000))
			i++;

		max8998->buck1_vol[1] = i;
		ret = max8998_write_reg(i2c, MAX8998_REG_BUCK1_VOLTAGE2, i);
		if (ret)
			goto err_free_mem;

		/* Set predefined value for BUCK1 register 3 */
		i = 0;
		while (buck12_voltage_map_desc.min +
		       buck12_voltage_map_desc.step*i
		       < (pdata->buck1_voltage3 / 1000))
			i++;

		max8998->buck1_vol[2] = i;
		ret = max8998_write_reg(i2c, MAX8998_REG_BUCK1_VOLTAGE3, i);
		if (ret)
			goto err_free_mem;

		/* Set predefined value for BUCK1 register 4 */
		i = 0;
		while (buck12_voltage_map_desc.min +
		       buck12_voltage_map_desc.step*i
		       < (pdata->buck1_voltage4 / 1000))
			i++;

		max8998->buck1_vol[3] = i;
		ret = max8998_write_reg(i2c, MAX8998_REG_BUCK1_VOLTAGE4, i);
		if (ret)
			goto err_free_mem;

	}

	if (gpio_is_valid(pdata->buck2_set3)) {
		/* Check if SET3 is not equal to 0 */
		if (!pdata->buck2_set3) {
			printk(KERN_ERR "MAX8998 SET3 GPIO defined as 0 !\n");
			WARN_ON(!pdata->buck2_set3);
			ret = -EIO;
			goto err_free_mem;
		}
		gpio_request(pdata->buck2_set3, "MAX8998 BUCK2_SET3");
		gpio_direction_output(pdata->buck2_set3,
				      max8998->buck2_idx & 0x1);

		/* BUCK2 register 1 */
		i = 0;
		while (buck12_voltage_map_desc.min +
		       buck12_voltage_map_desc.step*i
		       < (pdata->buck2_voltage1 / 1000))
			i++;
		max8998->buck2_vol[0] = i;
		ret = max8998_write_reg(i2c, MAX8998_REG_BUCK2_VOLTAGE1, i);
		if (ret)
			goto err_free_mem;

		/* BUCK2 register 2 */
		i = 0;
		while (buck12_voltage_map_desc.min +
		       buck12_voltage_map_desc.step*i
		       < (pdata->buck2_voltage2 / 1000))
			i++;
		max8998->buck2_vol[1] = i;
		ret = max8998_write_reg(i2c, MAX8998_REG_BUCK2_VOLTAGE2, i);
		if (ret)
			goto err_free_mem;
	}

	for (i = 0; i < pdata->num_regulators; i++) {
		const struct voltage_map_desc *desc;
		int id = pdata->regulators[i].id;
		int index = id - MAX8998_LDO2;

		desc = ldo_voltage_map[id];
		if (desc && regulators[index].ops != &max8998_others_ops) {
			int count = (desc->max - desc->min) / desc->step + 1;
			regulators[index].n_voltages = count;
		}
		rdev[i] = regulator_register(&regulators[index], max8998->dev,
				pdata->regulators[i].initdata, max8998, NULL);
		if (IS_ERR(rdev[i])) {
			ret = PTR_ERR(rdev[i]);
			dev_err(max8998->dev, "regulator init failed\n");
			rdev[i] = NULL;
			goto err;
		}
	}


	return 0;
err:
	for (i = 0; i < max8998->num_regulators; i++)
		if (rdev[i])
			regulator_unregister(rdev[i]);

err_free_mem:
	kfree(max8998->rdev);
	kfree(max8998);

	return ret;
}

static int __devexit max8998_pmic_remove(struct platform_device *pdev)
{
	struct max8998_data *max8998 = platform_get_drvdata(pdev);
	struct regulator_dev **rdev = max8998->rdev;
	int i;

	for (i = 0; i < max8998->num_regulators; i++)
		if (rdev[i])
			regulator_unregister(rdev[i]);

	kfree(max8998->rdev);
	kfree(max8998);

=======
	/* Check if MAX8998 voltage selection GPIOs are defined */
	flags = (max8998->buck1_idx & BIT(0)) ? GPIOD_OUT_HIGH : GPIOD_OUT_LOW;
	max8998->buck1_gpio1 = devm_gpiod_get_index_optional(iodev->dev,
							     "max8998,pmic-buck1-dvs",
							     0,
							     flags);
	if (IS_ERR(max8998->buck1_gpio1))
		return dev_err_probe(&pdev->dev, PTR_ERR(max8998->buck1_gpio1),
				     "could not get BUCK1 GPIO1\n");
	gpiod_set_consumer_name(max8998->buck1_gpio1, "MAX8998 BUCK1_SET1");

	flags = (max8998->buck1_idx & BIT(1)) ? GPIOD_OUT_HIGH : GPIOD_OUT_LOW;
	max8998->buck1_gpio2 = devm_gpiod_get_index_optional(iodev->dev,
							     "max8998,pmic-buck1-dvs",
							     1,
							     flags);
	if (IS_ERR(max8998->buck1_gpio2))
		return dev_err_probe(&pdev->dev, PTR_ERR(max8998->buck1_gpio2),
				     "could not get BUCK1 GPIO2\n");
	gpiod_set_consumer_name(max8998->buck1_gpio1, "MAX8998 BUCK1_SET2");

	flags = (max8998->buck2_idx & BIT(0)) ? GPIOD_OUT_HIGH : GPIOD_OUT_LOW;
	max8998->buck2_gpio = devm_gpiod_get_index_optional(iodev->dev,
							     "max8998,pmic-buck2-dvs",
							     0,
							     flags);
	if (IS_ERR(max8998->buck2_gpio))
		return dev_err_probe(&pdev->dev, PTR_ERR(max8998->buck2_gpio),
				     "could not get BUCK2 GPIO\n");
	gpiod_set_consumer_name(max8998->buck1_gpio1, "MAX8998 BUCK2_SET3");

	if (max8998->buck1_gpio1 && max8998->buck1_gpio2) {
		/* Set predefined values for BUCK1 registers */
		for (v = 0; v < ARRAY_SIZE(pdata->buck1_voltage); ++v) {
			int index = MAX8998_BUCK1 - MAX8998_LDO2;

			i = 0;
			while (regulators[index].min_uV +
			       regulators[index].uV_step * i
			       < pdata->buck1_voltage[v])
				i++;

			max8998->buck1_vol[v] = i;
			ret = max8998_write_reg(i2c,
					MAX8998_REG_BUCK1_VOLTAGE1 + v, i);
			if (ret)
				return ret;
		}
	}

	if (max8998->buck2_gpio) {
		/* Set predefined values for BUCK2 registers */
		for (v = 0; v < ARRAY_SIZE(pdata->buck2_voltage); ++v) {
			int index = MAX8998_BUCK2 - MAX8998_LDO2;

			i = 0;
			while (regulators[index].min_uV +
			       regulators[index].uV_step * i
			       < pdata->buck2_voltage[v])
				i++;

			max8998->buck2_vol[v] = i;
			ret = max8998_write_reg(i2c,
					MAX8998_REG_BUCK2_VOLTAGE1 + v, i);
			if (ret)
				return ret;
		}
	}

	for (i = 0; i < pdata->num_regulators; i++) {
		int index = pdata->regulators[i].id - MAX8998_LDO2;

		config.dev = max8998->dev;
		config.of_node = pdata->regulators[i].reg_node;
		config.init_data = pdata->regulators[i].initdata;
		config.driver_data = max8998;

		rdev = devm_regulator_register(&pdev->dev, &regulators[index],
					       &config);
		if (IS_ERR(rdev)) {
			ret = PTR_ERR(rdev);
			dev_err(max8998->dev, "regulator %s init failed (%d)\n",
						regulators[index].name, ret);
			return ret;
		}
	}

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	return 0;
}

static const struct platform_device_id max8998_pmic_id[] = {
	{ "max8998-pmic", TYPE_MAX8998 },
	{ "lp3974-pmic", TYPE_LP3974 },
	{ }
};
MODULE_DEVICE_TABLE(platform, max8998_pmic_id);

static struct platform_driver max8998_pmic_driver = {
	.driver = {
		.name = "max8998-pmic",
<<<<<<< HEAD
		.owner = THIS_MODULE,
	},
	.probe = max8998_pmic_probe,
	.remove = __devexit_p(max8998_pmic_remove),
=======
		.probe_type = PROBE_PREFER_ASYNCHRONOUS,
	},
	.probe = max8998_pmic_probe,
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	.id_table = max8998_pmic_id,
};

static int __init max8998_pmic_init(void)
{
	return platform_driver_register(&max8998_pmic_driver);
}
subsys_initcall(max8998_pmic_init);

static void __exit max8998_pmic_cleanup(void)
{
	platform_driver_unregister(&max8998_pmic_driver);
}
module_exit(max8998_pmic_cleanup);

MODULE_DESCRIPTION("MAXIM 8998 voltage regulator driver");
MODULE_AUTHOR("Kyungmin Park <kyungmin.park@samsung.com>");
MODULE_LICENSE("GPL");
