<<<<<<< HEAD
/*
 * s5m8767.c
 *
 * Copyright (c) 2011 Samsung Electronics Co., Ltd
 *              http://www.samsung.com
 *
 *  This program is free software; you can redistribute  it and/or modify it
 *  under  the terms of  the GNU General  Public License as published by the
 *  Free Software Foundation;  either version 2 of the  License, or (at your
 *  option) any later version.
 *
 */

#include <linux/bug.h>
#include <linux/delay.h>
#include <linux/err.h>
#include <linux/gpio.h>
#include <linux/slab.h>
=======
// SPDX-License-Identifier: GPL-2.0+
//
// Copyright (c) 2011 Samsung Electronics Co., Ltd
//              http://www.samsung.com

#include <linux/err.h>
#include <linux/of_gpio.h>
#include <linux/gpio/consumer.h>
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#include <linux/module.h>
#include <linux/platform_device.h>
#include <linux/regulator/driver.h>
#include <linux/regulator/machine.h>
<<<<<<< HEAD
#include <linux/mfd/s5m87xx/s5m-core.h>
#include <linux/mfd/s5m87xx/s5m-pmic.h>

struct s5m8767_info {
	struct device *dev;
	struct s5m87xx_dev *iodev;
	int num_regulators;
	struct regulator_dev **rdev;
=======
#include <linux/mfd/samsung/core.h>
#include <linux/mfd/samsung/s5m8767.h>
#include <linux/regulator/of_regulator.h>
#include <linux/regmap.h>

#define S5M8767_OPMODE_NORMAL_MODE 0x1

struct s5m8767_info {
	struct device *dev;
	struct sec_pmic_dev *iodev;
	int num_regulators;
	struct sec_opmode_data *opmode;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	int ramp_delay;
	bool buck2_ramp;
	bool buck3_ramp;
	bool buck4_ramp;

	bool buck2_gpiodvs;
	bool buck3_gpiodvs;
	bool buck4_gpiodvs;
	u8 buck2_vol[8];
	u8 buck3_vol[8];
	u8 buck4_vol[8];
	int buck_gpios[3];
<<<<<<< HEAD
	int buck_gpioindex;
};

struct s5m_voltage_desc {
=======
	int buck_ds[3];
	int buck_gpioindex;
};

struct sec_voltage_desc {
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	int max;
	int min;
	int step;
};

<<<<<<< HEAD
static const struct s5m_voltage_desc buck_voltage_val1 = {
=======
static const struct sec_voltage_desc buck_voltage_val1 = {
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	.max = 2225000,
	.min =  650000,
	.step =   6250,
};

<<<<<<< HEAD
static const struct s5m_voltage_desc buck_voltage_val2 = {
=======
static const struct sec_voltage_desc buck_voltage_val2 = {
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	.max = 1600000,
	.min =  600000,
	.step =   6250,
};

<<<<<<< HEAD
static const struct s5m_voltage_desc buck_voltage_val3 = {
=======
static const struct sec_voltage_desc buck_voltage_val3 = {
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	.max = 3000000,
	.min =  750000,
	.step =  12500,
};

<<<<<<< HEAD
static const struct s5m_voltage_desc ldo_voltage_val1 = {
=======
static const struct sec_voltage_desc ldo_voltage_val1 = {
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	.max = 3950000,
	.min =  800000,
	.step =  50000,
};

<<<<<<< HEAD
static const struct s5m_voltage_desc ldo_voltage_val2 = {
=======
static const struct sec_voltage_desc ldo_voltage_val2 = {
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	.max = 2375000,
	.min =  800000,
	.step =  25000,
};

<<<<<<< HEAD
static const struct s5m_voltage_desc *reg_voltage_map[] = {
=======
static const struct sec_voltage_desc *reg_voltage_map[] = {
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	[S5M8767_LDO1] = &ldo_voltage_val2,
	[S5M8767_LDO2] = &ldo_voltage_val2,
	[S5M8767_LDO3] = &ldo_voltage_val1,
	[S5M8767_LDO4] = &ldo_voltage_val1,
	[S5M8767_LDO5] = &ldo_voltage_val1,
	[S5M8767_LDO6] = &ldo_voltage_val2,
	[S5M8767_LDO7] = &ldo_voltage_val2,
	[S5M8767_LDO8] = &ldo_voltage_val2,
	[S5M8767_LDO9] = &ldo_voltage_val1,
	[S5M8767_LDO10] = &ldo_voltage_val1,
	[S5M8767_LDO11] = &ldo_voltage_val1,
	[S5M8767_LDO12] = &ldo_voltage_val1,
	[S5M8767_LDO13] = &ldo_voltage_val1,
	[S5M8767_LDO14] = &ldo_voltage_val1,
	[S5M8767_LDO15] = &ldo_voltage_val2,
	[S5M8767_LDO16] = &ldo_voltage_val1,
	[S5M8767_LDO17] = &ldo_voltage_val1,
	[S5M8767_LDO18] = &ldo_voltage_val1,
	[S5M8767_LDO19] = &ldo_voltage_val1,
	[S5M8767_LDO20] = &ldo_voltage_val1,
	[S5M8767_LDO21] = &ldo_voltage_val1,
	[S5M8767_LDO22] = &ldo_voltage_val1,
	[S5M8767_LDO23] = &ldo_voltage_val1,
	[S5M8767_LDO24] = &ldo_voltage_val1,
	[S5M8767_LDO25] = &ldo_voltage_val1,
	[S5M8767_LDO26] = &ldo_voltage_val1,
	[S5M8767_LDO27] = &ldo_voltage_val1,
	[S5M8767_LDO28] = &ldo_voltage_val1,
	[S5M8767_BUCK1] = &buck_voltage_val1,
	[S5M8767_BUCK2] = &buck_voltage_val2,
	[S5M8767_BUCK3] = &buck_voltage_val2,
	[S5M8767_BUCK4] = &buck_voltage_val2,
	[S5M8767_BUCK5] = &buck_voltage_val1,
	[S5M8767_BUCK6] = &buck_voltage_val1,
<<<<<<< HEAD
	[S5M8767_BUCK7] = NULL,
	[S5M8767_BUCK8] = NULL,
	[S5M8767_BUCK9] = &buck_voltage_val3,
};

static int s5m8767_list_voltage(struct regulator_dev *rdev,
				unsigned int selector)
{
	const struct s5m_voltage_desc *desc;
	int reg_id = rdev_get_id(rdev);
	int val;

	if (reg_id >= ARRAY_SIZE(reg_voltage_map) || reg_id < 0)
		return -EINVAL;

	desc = reg_voltage_map[reg_id];
	if (desc == NULL)
		return -EINVAL;

	val = desc->min + desc->step * selector;
	if (val > desc->max)
		return -EINVAL;

	return val;
}

static int s5m8767_get_register(struct regulator_dev *rdev, int *reg)
{
	int reg_id = rdev_get_id(rdev);
=======
	[S5M8767_BUCK7] = &buck_voltage_val3,
	[S5M8767_BUCK8] = &buck_voltage_val3,
	[S5M8767_BUCK9] = &buck_voltage_val3,
};

static const unsigned int s5m8767_opmode_reg[][4] = {
	/* {OFF, ON, LOWPOWER, SUSPEND} */
	/* LDO1 ... LDO28 */
	{0x0, 0x3, 0x2, 0x1}, /* LDO1 */
	{0x0, 0x3, 0x2, 0x1},
	{0x0, 0x3, 0x2, 0x1},
	{0x0, 0x0, 0x0, 0x0},
	{0x0, 0x3, 0x2, 0x1}, /* LDO5 */
	{0x0, 0x3, 0x2, 0x1},
	{0x0, 0x3, 0x2, 0x1},
	{0x0, 0x3, 0x2, 0x1},
	{0x0, 0x3, 0x2, 0x1},
	{0x0, 0x3, 0x2, 0x1}, /* LDO10 */
	{0x0, 0x3, 0x2, 0x1},
	{0x0, 0x3, 0x2, 0x1},
	{0x0, 0x3, 0x2, 0x1},
	{0x0, 0x3, 0x2, 0x1},
	{0x0, 0x3, 0x2, 0x1}, /* LDO15 */
	{0x0, 0x3, 0x2, 0x1},
	{0x0, 0x3, 0x2, 0x1},
	{0x0, 0x0, 0x0, 0x0},
	{0x0, 0x3, 0x2, 0x1},
	{0x0, 0x3, 0x2, 0x1}, /* LDO20 */
	{0x0, 0x3, 0x2, 0x1},
	{0x0, 0x3, 0x2, 0x1},
	{0x0, 0x0, 0x0, 0x0},
	{0x0, 0x3, 0x2, 0x1},
	{0x0, 0x3, 0x2, 0x1}, /* LDO25 */
	{0x0, 0x3, 0x2, 0x1},
	{0x0, 0x3, 0x2, 0x1},
	{0x0, 0x3, 0x2, 0x1}, /* LDO28 */

	/* BUCK1 ... BUCK9 */
	{0x0, 0x3, 0x1, 0x1}, /* BUCK1 */
	{0x0, 0x3, 0x1, 0x1},
	{0x0, 0x3, 0x1, 0x1},
	{0x0, 0x3, 0x1, 0x1},
	{0x0, 0x3, 0x2, 0x1}, /* BUCK5 */
	{0x0, 0x3, 0x1, 0x1},
	{0x0, 0x3, 0x1, 0x1},
	{0x0, 0x3, 0x1, 0x1},
	{0x0, 0x3, 0x1, 0x1}, /* BUCK9 */
};

static int s5m8767_get_register(struct s5m8767_info *s5m8767, int reg_id,
				int *reg, int *enable_ctrl)
{
	int i;
	unsigned int mode;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	switch (reg_id) {
	case S5M8767_LDO1 ... S5M8767_LDO2:
		*reg = S5M8767_REG_LDO1CTRL + (reg_id - S5M8767_LDO1);
		break;
	case S5M8767_LDO3 ... S5M8767_LDO28:
		*reg = S5M8767_REG_LDO3CTRL + (reg_id - S5M8767_LDO3);
		break;
	case S5M8767_BUCK1:
		*reg = S5M8767_REG_BUCK1CTRL1;
		break;
	case S5M8767_BUCK2 ... S5M8767_BUCK4:
		*reg = S5M8767_REG_BUCK2CTRL + (reg_id - S5M8767_BUCK2) * 9;
		break;
	case S5M8767_BUCK5:
		*reg = S5M8767_REG_BUCK5CTRL1;
		break;
	case S5M8767_BUCK6 ... S5M8767_BUCK9:
		*reg = S5M8767_REG_BUCK6CTRL1 + (reg_id - S5M8767_BUCK6) * 2;
		break;
	default:
		return -EINVAL;
	}

<<<<<<< HEAD
	return 0;
}

static int s5m8767_reg_is_enabled(struct regulator_dev *rdev)
{
	struct s5m8767_info *s5m8767 = rdev_get_drvdata(rdev);
	int ret, reg;
	int mask = 0xc0, pattern = 0xc0;
	u8 val;

	ret = s5m8767_get_register(rdev, &reg);
	if (ret == -EINVAL)
		return 1;
	else if (ret)
		return ret;

	ret = s5m_reg_read(s5m8767->iodev, reg, &val);
	if (ret)
		return ret;

	return (val & mask) == pattern;
}

static int s5m8767_reg_enable(struct regulator_dev *rdev)
{
	struct s5m8767_info *s5m8767 = rdev_get_drvdata(rdev);
	int ret, reg;
	int mask = 0xc0, pattern = 0xc0;

	ret = s5m8767_get_register(rdev, &reg);
	if (ret)
		return ret;

	return s5m_reg_update(s5m8767->iodev, reg, pattern, mask);
}

static int s5m8767_reg_disable(struct regulator_dev *rdev)
{
	struct s5m8767_info *s5m8767 = rdev_get_drvdata(rdev);
	int ret, reg;
	int  mask = 0xc0, pattern = 0xc0;

	ret = s5m8767_get_register(rdev, &reg);
	if (ret)
		return ret;

	return s5m_reg_update(s5m8767->iodev, reg, ~pattern, mask);
}

static int s5m8767_get_voltage_register(struct regulator_dev *rdev, int *_reg)
{
	struct s5m8767_info *s5m8767 = rdev_get_drvdata(rdev);
	int reg_id = rdev_get_id(rdev);
=======
	for (i = 0; i < s5m8767->num_regulators; i++) {
		if (s5m8767->opmode[i].id == reg_id) {
			mode = s5m8767->opmode[i].mode;
			break;
		}
	}

	if (i >= s5m8767->num_regulators)
		return -EINVAL;

	*enable_ctrl = s5m8767_opmode_reg[reg_id][mode] << S5M8767_ENCTRL_SHIFT;

	return 0;
}

static int s5m8767_get_vsel_reg(int reg_id, struct s5m8767_info *s5m8767)
{
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	int reg;

	switch (reg_id) {
	case S5M8767_LDO1 ... S5M8767_LDO2:
		reg = S5M8767_REG_LDO1CTRL + (reg_id - S5M8767_LDO1);
		break;
	case S5M8767_LDO3 ... S5M8767_LDO28:
		reg = S5M8767_REG_LDO3CTRL + (reg_id - S5M8767_LDO3);
		break;
	case S5M8767_BUCK1:
		reg = S5M8767_REG_BUCK1CTRL2;
		break;
	case S5M8767_BUCK2:
		reg = S5M8767_REG_BUCK2DVS1;
		if (s5m8767->buck2_gpiodvs)
			reg += s5m8767->buck_gpioindex;
		break;
	case S5M8767_BUCK3:
		reg = S5M8767_REG_BUCK3DVS1;
		if (s5m8767->buck3_gpiodvs)
			reg += s5m8767->buck_gpioindex;
		break;
	case S5M8767_BUCK4:
		reg = S5M8767_REG_BUCK4DVS1;
		if (s5m8767->buck4_gpiodvs)
			reg += s5m8767->buck_gpioindex;
		break;
	case S5M8767_BUCK5:
		reg = S5M8767_REG_BUCK5CTRL2;
		break;
	case S5M8767_BUCK6 ... S5M8767_BUCK9:
		reg = S5M8767_REG_BUCK6CTRL2 + (reg_id - S5M8767_BUCK6) * 2;
		break;
	default:
		return -EINVAL;
	}

<<<<<<< HEAD
	*_reg = reg;

	return 0;
}

static int s5m8767_get_voltage_sel(struct regulator_dev *rdev)
{
	struct s5m8767_info *s5m8767 = rdev_get_drvdata(rdev);
	int reg, mask, ret;
	int reg_id = rdev_get_id(rdev);
	u8 val;

	ret = s5m8767_get_voltage_register(rdev, &reg);
	if (ret)
		return ret;

	mask = (reg_id < S5M8767_BUCK1) ? 0x3f : 0xff;

	ret = s5m_reg_read(s5m8767->iodev, reg, &val);
	if (ret)
		return ret;

	val &= mask;

	return val;
}

static int s5m8767_convert_voltage_to_sel(
		const struct s5m_voltage_desc *desc,
		int min_vol, int max_vol)
=======
	return reg;
}

static int s5m8767_convert_voltage_to_sel(const struct sec_voltage_desc *desc,
					  int min_vol)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
{
	int selector = 0;

	if (desc == NULL)
		return -EINVAL;

<<<<<<< HEAD
	if (max_vol < desc->min || min_vol > desc->max)
		return -EINVAL;

	selector = (min_vol - desc->min) / desc->step;

	if (desc->min + desc->step * selector > max_vol)
=======
	if (min_vol > desc->max)
		return -EINVAL;

	if (min_vol < desc->min)
		min_vol = desc->min;

	selector = DIV_ROUND_UP(min_vol - desc->min, desc->step);

	if (desc->min + desc->step * selector > desc->max)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		return -EINVAL;

	return selector;
}

<<<<<<< HEAD
static int s5m8767_set_voltage(struct regulator_dev *rdev,
				int min_uV, int max_uV, unsigned *selector)
{
	struct s5m8767_info *s5m8767 = rdev_get_drvdata(rdev);
	const struct s5m_voltage_desc *desc;
	int reg_id = rdev_get_id(rdev);
	int sel, reg, mask, ret;
	u8 val;

	switch (reg_id) {
	case S5M8767_LDO1 ... S5M8767_LDO28:
		mask = 0x3f;
		break;
	case S5M8767_BUCK1 ... S5M8767_BUCK6:
		mask = 0xff;
=======
static inline int s5m8767_set_high(struct s5m8767_info *s5m8767)
{
	int temp_index = s5m8767->buck_gpioindex;

	gpio_set_value(s5m8767->buck_gpios[0], (temp_index >> 2) & 0x1);
	gpio_set_value(s5m8767->buck_gpios[1], (temp_index >> 1) & 0x1);
	gpio_set_value(s5m8767->buck_gpios[2], temp_index & 0x1);

	return 0;
}

static inline int s5m8767_set_low(struct s5m8767_info *s5m8767)
{
	int temp_index = s5m8767->buck_gpioindex;

	gpio_set_value(s5m8767->buck_gpios[2], temp_index & 0x1);
	gpio_set_value(s5m8767->buck_gpios[1], (temp_index >> 1) & 0x1);
	gpio_set_value(s5m8767->buck_gpios[0], (temp_index >> 2) & 0x1);

	return 0;
}

static int s5m8767_set_voltage_sel(struct regulator_dev *rdev,
				   unsigned selector)
{
	struct s5m8767_info *s5m8767 = rdev_get_drvdata(rdev);
	int reg_id = rdev_get_id(rdev);
	int old_index, index = 0;
	u8 *buck234_vol = NULL;

	switch (reg_id) {
	case S5M8767_LDO1 ... S5M8767_LDO28:
		break;
	case S5M8767_BUCK1 ... S5M8767_BUCK6:
		if (reg_id == S5M8767_BUCK2 && s5m8767->buck2_gpiodvs)
			buck234_vol = &s5m8767->buck2_vol[0];
		else if (reg_id == S5M8767_BUCK3 && s5m8767->buck3_gpiodvs)
			buck234_vol = &s5m8767->buck3_vol[0];
		else if (reg_id == S5M8767_BUCK4 && s5m8767->buck4_gpiodvs)
			buck234_vol = &s5m8767->buck4_vol[0];
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		break;
	case S5M8767_BUCK7 ... S5M8767_BUCK8:
		return -EINVAL;
	case S5M8767_BUCK9:
<<<<<<< HEAD
		mask = 0xff;
=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		break;
	default:
		return -EINVAL;
	}

<<<<<<< HEAD
	desc = reg_voltage_map[reg_id];

	sel = s5m8767_convert_voltage_to_sel(desc, min_uV, max_uV);
	if (sel < 0)
		return sel;

	ret = s5m8767_get_voltage_register(rdev, &reg);
	if (ret)
		return ret;

	s5m_reg_read(s5m8767->iodev, reg, &val);
	val &= ~mask;
	val |= sel;

	ret = s5m_reg_write(s5m8767->iodev, reg, val);
	*selector = sel;

	return ret;
}

static inline void s5m8767_set_high(struct s5m8767_info *s5m8767)
{
	int temp_index = s5m8767->buck_gpioindex;

	gpio_set_value(s5m8767->buck_gpios[0], (temp_index >> 2) & 0x1);
	gpio_set_value(s5m8767->buck_gpios[1], (temp_index >> 1) & 0x1);
	gpio_set_value(s5m8767->buck_gpios[2], temp_index & 0x1);
}

static inline void s5m8767_set_low(struct s5m8767_info *s5m8767)
{
	int temp_index = s5m8767->buck_gpioindex;

	gpio_set_value(s5m8767->buck_gpios[2], temp_index & 0x1);
	gpio_set_value(s5m8767->buck_gpios[1], (temp_index >> 1) & 0x1);
	gpio_set_value(s5m8767->buck_gpios[0], (temp_index >> 2) & 0x1);
}

static int s5m8767_set_voltage_buck(struct regulator_dev *rdev,
				    int min_uV, int max_uV, unsigned *selector)
{
	struct s5m8767_info *s5m8767 = rdev_get_drvdata(rdev);
	int reg_id = rdev_get_id(rdev);
	const struct s5m_voltage_desc *desc;
	int new_val, old_val, i = 0;

	if (reg_id < S5M8767_BUCK1 || reg_id > S5M8767_BUCK6)
		return -EINVAL;

	switch (reg_id) {
	case S5M8767_BUCK1:
		return s5m8767_set_voltage(rdev, min_uV, max_uV, selector);
	case S5M8767_BUCK2 ... S5M8767_BUCK4:
		break;
	case S5M8767_BUCK5 ... S5M8767_BUCK6:
		return s5m8767_set_voltage(rdev, min_uV, max_uV, selector);
	case S5M8767_BUCK9:
		return s5m8767_set_voltage(rdev, min_uV, max_uV, selector);
	}

	desc = reg_voltage_map[reg_id];
	new_val = s5m8767_convert_voltage_to_sel(desc, min_uV, max_uV);
	if (new_val < 0)
		return new_val;

	switch (reg_id) {
	case S5M8767_BUCK2:
		if (s5m8767->buck2_gpiodvs) {
			while (s5m8767->buck2_vol[i] != new_val)
				i++;
		} else
			return s5m8767_set_voltage(rdev, min_uV,
						   max_uV, selector);
		break;
	case S5M8767_BUCK3:
		if (s5m8767->buck3_gpiodvs) {
			while (s5m8767->buck3_vol[i] != new_val)
				i++;
		} else
			return s5m8767_set_voltage(rdev, min_uV,
						   max_uV, selector);
		break;
	case S5M8767_BUCK4:
		if (s5m8767->buck3_gpiodvs) {
			while (s5m8767->buck4_vol[i] != new_val)
				i++;
		} else
			return s5m8767_set_voltage(rdev, min_uV,
						   max_uV, selector);
		break;
	}

	old_val = s5m8767->buck_gpioindex;
	s5m8767->buck_gpioindex = i;

	if (i > old_val)
		s5m8767_set_high(s5m8767);
	else
		s5m8767_set_low(s5m8767);

	*selector = new_val;
	return 0;
=======
	/* buck234_vol != NULL means to control buck234 voltage via DVS GPIO */
	if (buck234_vol) {
		while (*buck234_vol != selector) {
			buck234_vol++;
			index++;
		}
		old_index = s5m8767->buck_gpioindex;
		s5m8767->buck_gpioindex = index;

		if (index > old_index)
			return s5m8767_set_high(s5m8767);
		else
			return s5m8767_set_low(s5m8767);
	} else {
		return regulator_set_voltage_sel_regmap(rdev, selector);
	}
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

static int s5m8767_set_voltage_time_sel(struct regulator_dev *rdev,
					     unsigned int old_sel,
					     unsigned int new_sel)
{
	struct s5m8767_info *s5m8767 = rdev_get_drvdata(rdev);
<<<<<<< HEAD
	const struct s5m_voltage_desc *desc;
	int reg_id = rdev_get_id(rdev);

	desc = reg_voltage_map[reg_id];

	if (old_sel < new_sel)
		return DIV_ROUND_UP(desc->step * (new_sel - old_sel),
=======

	if ((old_sel < new_sel) && s5m8767->ramp_delay)
		return DIV_ROUND_UP(rdev->desc->uV_step * (new_sel - old_sel),
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
					s5m8767->ramp_delay * 1000);
	return 0;
}

<<<<<<< HEAD
static struct regulator_ops s5m8767_ldo_ops = {
	.list_voltage		= s5m8767_list_voltage,
	.is_enabled		= s5m8767_reg_is_enabled,
	.enable			= s5m8767_reg_enable,
	.disable		= s5m8767_reg_disable,
	.get_voltage_sel	= s5m8767_get_voltage_sel,
	.set_voltage		= s5m8767_set_voltage,
	.set_voltage_time_sel	= s5m8767_set_voltage_time_sel,
};

static struct regulator_ops s5m8767_buck_ops = {
	.list_voltage		= s5m8767_list_voltage,
	.is_enabled		= s5m8767_reg_is_enabled,
	.enable			= s5m8767_reg_enable,
	.disable		= s5m8767_reg_disable,
	.get_voltage_sel	= s5m8767_get_voltage_sel,
	.set_voltage		= s5m8767_set_voltage_buck,
	.set_voltage_time_sel	= s5m8767_set_voltage_time_sel,
};

#define regulator_desc_ldo(num)		{	\
	.name		= "LDO"#num,		\
	.id		= S5M8767_LDO##num,	\
	.ops		= &s5m8767_ldo_ops,	\
	.type		= REGULATOR_VOLTAGE,	\
	.owner		= THIS_MODULE,		\
}
#define regulator_desc_buck(num)	{	\
	.name		= "BUCK"#num,		\
	.id		= S5M8767_BUCK##num,	\
	.ops		= &s5m8767_buck_ops,	\
=======
static const struct regulator_ops s5m8767_ops = {
	.list_voltage		= regulator_list_voltage_linear,
	.is_enabled		= regulator_is_enabled_regmap,
	.enable			= regulator_enable_regmap,
	.disable		= regulator_disable_regmap,
	.get_voltage_sel	= regulator_get_voltage_sel_regmap,
	.set_voltage_sel	= s5m8767_set_voltage_sel,
	.set_voltage_time_sel	= s5m8767_set_voltage_time_sel,
};

static const struct regulator_ops s5m8767_buck78_ops = {
	.list_voltage		= regulator_list_voltage_linear,
	.is_enabled		= regulator_is_enabled_regmap,
	.enable			= regulator_enable_regmap,
	.disable		= regulator_disable_regmap,
	.get_voltage_sel	= regulator_get_voltage_sel_regmap,
	.set_voltage_sel	= regulator_set_voltage_sel_regmap,
};

#define s5m8767_regulator_desc(_name) {		\
	.name		= #_name,		\
	.id		= S5M8767_##_name,	\
	.ops		= &s5m8767_ops,		\
	.type		= REGULATOR_VOLTAGE,	\
	.owner		= THIS_MODULE,		\
}

#define s5m8767_regulator_buck78_desc(_name) {	\
	.name		= #_name,		\
	.id		= S5M8767_##_name,	\
	.ops		= &s5m8767_buck78_ops,	\
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	.type		= REGULATOR_VOLTAGE,	\
	.owner		= THIS_MODULE,		\
}

static struct regulator_desc regulators[] = {
<<<<<<< HEAD
	regulator_desc_ldo(1),
	regulator_desc_ldo(2),
	regulator_desc_ldo(3),
	regulator_desc_ldo(4),
	regulator_desc_ldo(5),
	regulator_desc_ldo(6),
	regulator_desc_ldo(7),
	regulator_desc_ldo(8),
	regulator_desc_ldo(9),
	regulator_desc_ldo(10),
	regulator_desc_ldo(11),
	regulator_desc_ldo(12),
	regulator_desc_ldo(13),
	regulator_desc_ldo(14),
	regulator_desc_ldo(15),
	regulator_desc_ldo(16),
	regulator_desc_ldo(17),
	regulator_desc_ldo(18),
	regulator_desc_ldo(19),
	regulator_desc_ldo(20),
	regulator_desc_ldo(21),
	regulator_desc_ldo(22),
	regulator_desc_ldo(23),
	regulator_desc_ldo(24),
	regulator_desc_ldo(25),
	regulator_desc_ldo(26),
	regulator_desc_ldo(27),
	regulator_desc_ldo(28),
	regulator_desc_buck(1),
	regulator_desc_buck(2),
	regulator_desc_buck(3),
	regulator_desc_buck(4),
	regulator_desc_buck(5),
	regulator_desc_buck(6),
	regulator_desc_buck(7),
	regulator_desc_buck(8),
	regulator_desc_buck(9),
};

static __devinit int s5m8767_pmic_probe(struct platform_device *pdev)
{
	struct s5m87xx_dev *iodev = dev_get_drvdata(pdev->dev.parent);
	struct s5m_platform_data *pdata = dev_get_platdata(iodev->dev);
	struct regulator_dev **rdev;
	struct s5m8767_info *s5m8767;
	int i, ret, size;
=======
	s5m8767_regulator_desc(LDO1),
	s5m8767_regulator_desc(LDO2),
	s5m8767_regulator_desc(LDO3),
	s5m8767_regulator_desc(LDO4),
	s5m8767_regulator_desc(LDO5),
	s5m8767_regulator_desc(LDO6),
	s5m8767_regulator_desc(LDO7),
	s5m8767_regulator_desc(LDO8),
	s5m8767_regulator_desc(LDO9),
	s5m8767_regulator_desc(LDO10),
	s5m8767_regulator_desc(LDO11),
	s5m8767_regulator_desc(LDO12),
	s5m8767_regulator_desc(LDO13),
	s5m8767_regulator_desc(LDO14),
	s5m8767_regulator_desc(LDO15),
	s5m8767_regulator_desc(LDO16),
	s5m8767_regulator_desc(LDO17),
	s5m8767_regulator_desc(LDO18),
	s5m8767_regulator_desc(LDO19),
	s5m8767_regulator_desc(LDO20),
	s5m8767_regulator_desc(LDO21),
	s5m8767_regulator_desc(LDO22),
	s5m8767_regulator_desc(LDO23),
	s5m8767_regulator_desc(LDO24),
	s5m8767_regulator_desc(LDO25),
	s5m8767_regulator_desc(LDO26),
	s5m8767_regulator_desc(LDO27),
	s5m8767_regulator_desc(LDO28),
	s5m8767_regulator_desc(BUCK1),
	s5m8767_regulator_desc(BUCK2),
	s5m8767_regulator_desc(BUCK3),
	s5m8767_regulator_desc(BUCK4),
	s5m8767_regulator_desc(BUCK5),
	s5m8767_regulator_desc(BUCK6),
	s5m8767_regulator_buck78_desc(BUCK7),
	s5m8767_regulator_buck78_desc(BUCK8),
	s5m8767_regulator_desc(BUCK9),
};

/*
 * Enable GPIO control over BUCK9 in regulator_config for that regulator.
 */
static void s5m8767_regulator_config_ext_control(struct s5m8767_info *s5m8767,
		struct sec_regulator_data *rdata,
		struct regulator_config *config)
{
	int i, mode = 0;

	if (rdata->id != S5M8767_BUCK9)
		return;

	/* Check if opmode for regulator matches S5M8767_ENCTRL_USE_GPIO */
	for (i = 0; i < s5m8767->num_regulators; i++) {
		const struct sec_opmode_data *opmode = &s5m8767->opmode[i];
		if (opmode->id == rdata->id) {
			mode = s5m8767_opmode_reg[rdata->id][opmode->mode];
			break;
		}
	}
	if (mode != S5M8767_ENCTRL_USE_GPIO) {
		dev_warn(s5m8767->dev,
				"ext-control for %pOFn: mismatched op_mode (%x), ignoring\n",
				rdata->reg_node, mode);
		return;
	}

	if (!rdata->ext_control_gpiod) {
		dev_warn(s5m8767->dev,
				"ext-control for %pOFn: GPIO not valid, ignoring\n",
			 rdata->reg_node);
		return;
	}

	config->ena_gpiod = rdata->ext_control_gpiod;
}

/*
 * Turn on GPIO control over BUCK9.
 */
static int s5m8767_enable_ext_control(struct s5m8767_info *s5m8767,
		struct regulator_dev *rdev)
{
	int id = rdev_get_id(rdev);
	int ret, reg, enable_ctrl;

	if (id != S5M8767_BUCK9)
		return -EINVAL;

	ret = s5m8767_get_register(s5m8767, id, &reg, &enable_ctrl);
	if (ret)
		return ret;

	return regmap_update_bits(s5m8767->iodev->regmap_pmic,
			reg, S5M8767_ENCTRL_MASK,
			S5M8767_ENCTRL_USE_GPIO << S5M8767_ENCTRL_SHIFT);
}


#ifdef CONFIG_OF
static int s5m8767_pmic_dt_parse_dvs_gpio(struct sec_pmic_dev *iodev,
			struct sec_platform_data *pdata,
			struct device_node *pmic_np)
{
	int i, gpio;

	for (i = 0; i < 3; i++) {
		gpio = of_get_named_gpio(pmic_np,
					"s5m8767,pmic-buck-dvs-gpios", i);
		if (!gpio_is_valid(gpio)) {
			dev_err(iodev->dev, "invalid gpio[%d]: %d\n", i, gpio);
			return -EINVAL;
		}
		pdata->buck_gpios[i] = gpio;
	}
	return 0;
}

static int s5m8767_pmic_dt_parse_ds_gpio(struct sec_pmic_dev *iodev,
			struct sec_platform_data *pdata,
			struct device_node *pmic_np)
{
	int i, gpio;

	for (i = 0; i < 3; i++) {
		gpio = of_get_named_gpio(pmic_np,
					"s5m8767,pmic-buck-ds-gpios", i);
		if (!gpio_is_valid(gpio)) {
			dev_err(iodev->dev, "invalid gpio[%d]: %d\n", i, gpio);
			return -EINVAL;
		}
		pdata->buck_ds[i] = gpio;
	}
	return 0;
}

static int s5m8767_pmic_dt_parse_pdata(struct platform_device *pdev,
					struct sec_platform_data *pdata)
{
	struct sec_pmic_dev *iodev = dev_get_drvdata(pdev->dev.parent);
	struct device_node *pmic_np, *regulators_np, *reg_np;
	struct sec_regulator_data *rdata;
	struct sec_opmode_data *rmode;
	unsigned int i, dvs_voltage_nr = 8, ret;

	pmic_np = iodev->dev->of_node;
	if (!pmic_np) {
		dev_err(iodev->dev, "could not find pmic sub-node\n");
		return -ENODEV;
	}

	regulators_np = of_get_child_by_name(pmic_np, "regulators");
	if (!regulators_np) {
		dev_err(iodev->dev, "could not find regulators sub-node\n");
		return -EINVAL;
	}

	/* count the number of regulators to be supported in pmic */
	pdata->num_regulators = of_get_child_count(regulators_np);

	rdata = devm_kcalloc(&pdev->dev,
			     pdata->num_regulators, sizeof(*rdata),
			     GFP_KERNEL);
	if (!rdata) {
		of_node_put(regulators_np);
		return -ENOMEM;
	}

	rmode = devm_kcalloc(&pdev->dev,
			     pdata->num_regulators, sizeof(*rmode),
			     GFP_KERNEL);
	if (!rmode) {
		of_node_put(regulators_np);
		return -ENOMEM;
	}

	pdata->regulators = rdata;
	pdata->opmode = rmode;
	for_each_child_of_node(regulators_np, reg_np) {
		for (i = 0; i < ARRAY_SIZE(regulators); i++)
			if (of_node_name_eq(reg_np, regulators[i].name))
				break;

		if (i == ARRAY_SIZE(regulators)) {
			dev_warn(iodev->dev,
			"don't know how to configure regulator %pOFn\n",
			reg_np);
			continue;
		}

		rdata->ext_control_gpiod = devm_fwnode_gpiod_get(
			&pdev->dev,
			of_fwnode_handle(reg_np),
			"s5m8767,pmic-ext-control",
			GPIOD_OUT_HIGH | GPIOD_FLAGS_BIT_NONEXCLUSIVE,
			"s5m8767");
		if (PTR_ERR(rdata->ext_control_gpiod) == -ENOENT) {
			rdata->ext_control_gpiod = NULL;
		} else if (IS_ERR(rdata->ext_control_gpiod)) {
			of_node_put(reg_np);
			of_node_put(regulators_np);
			return PTR_ERR(rdata->ext_control_gpiod);
		}

		rdata->id = i;
		rdata->initdata = of_get_regulator_init_data(
						&pdev->dev, reg_np,
						&regulators[i]);
		rdata->reg_node = reg_np;
		rdata++;
		rmode->id = i;
		if (of_property_read_u32(reg_np, "op_mode",
				&rmode->mode)) {
			dev_warn(iodev->dev,
				"no op_mode property at %pOF\n",
				reg_np);

			rmode->mode = S5M8767_OPMODE_NORMAL_MODE;
		}
		rmode++;
	}

	of_node_put(regulators_np);

	if (of_property_read_bool(pmic_np, "s5m8767,pmic-buck2-uses-gpio-dvs")) {
		pdata->buck2_gpiodvs = true;

		if (of_property_read_u32_array(pmic_np,
				"s5m8767,pmic-buck2-dvs-voltage",
				pdata->buck2_voltage, dvs_voltage_nr)) {
			dev_err(iodev->dev, "buck2 voltages not specified\n");
			return -EINVAL;
		}
	}

	if (of_property_read_bool(pmic_np, "s5m8767,pmic-buck3-uses-gpio-dvs")) {
		pdata->buck3_gpiodvs = true;

		if (of_property_read_u32_array(pmic_np,
				"s5m8767,pmic-buck3-dvs-voltage",
				pdata->buck3_voltage, dvs_voltage_nr)) {
			dev_err(iodev->dev, "buck3 voltages not specified\n");
			return -EINVAL;
		}
	}

	if (of_property_read_bool(pmic_np, "s5m8767,pmic-buck4-uses-gpio-dvs")) {
		pdata->buck4_gpiodvs = true;

		if (of_property_read_u32_array(pmic_np,
				"s5m8767,pmic-buck4-dvs-voltage",
				pdata->buck4_voltage, dvs_voltage_nr)) {
			dev_err(iodev->dev, "buck4 voltages not specified\n");
			return -EINVAL;
		}
	}

	if (pdata->buck2_gpiodvs || pdata->buck3_gpiodvs ||
						pdata->buck4_gpiodvs) {
		ret = s5m8767_pmic_dt_parse_dvs_gpio(iodev, pdata, pmic_np);
		if (ret)
			return -EINVAL;

		if (of_property_read_u32(pmic_np,
				"s5m8767,pmic-buck-default-dvs-idx",
				&pdata->buck_default_idx)) {
			pdata->buck_default_idx = 0;
		} else {
			if (pdata->buck_default_idx >= 8) {
				pdata->buck_default_idx = 0;
				dev_info(iodev->dev,
				"invalid value for default dvs index, use 0\n");
			}
		}
	}

	ret = s5m8767_pmic_dt_parse_ds_gpio(iodev, pdata, pmic_np);
	if (ret)
		return -EINVAL;

	pdata->buck2_ramp_enable = of_property_read_bool(pmic_np, "s5m8767,pmic-buck2-ramp-enable");
	pdata->buck3_ramp_enable = of_property_read_bool(pmic_np, "s5m8767,pmic-buck3-ramp-enable");
	pdata->buck4_ramp_enable = of_property_read_bool(pmic_np, "s5m8767,pmic-buck4-ramp-enable");

	if (pdata->buck2_ramp_enable || pdata->buck3_ramp_enable
			|| pdata->buck4_ramp_enable) {
		if (of_property_read_u32(pmic_np, "s5m8767,pmic-buck-ramp-delay",
				&pdata->buck_ramp_delay))
			pdata->buck_ramp_delay = 0;
	}

	return 0;
}
#else
static int s5m8767_pmic_dt_parse_pdata(struct platform_device *pdev,
					struct sec_platform_data *pdata)
{
	return 0;
}
#endif /* CONFIG_OF */

static int s5m8767_pmic_probe(struct platform_device *pdev)
{
	struct sec_pmic_dev *iodev = dev_get_drvdata(pdev->dev.parent);
	struct sec_platform_data *pdata = iodev->pdata;
	struct regulator_config config = { };
	struct s5m8767_info *s5m8767;
	int i, ret, buck_init;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	if (!pdata) {
		dev_err(pdev->dev.parent, "Platform data not supplied\n");
		return -ENODEV;
	}

<<<<<<< HEAD
=======
	if (iodev->dev->of_node) {
		ret = s5m8767_pmic_dt_parse_pdata(pdev, pdata);
		if (ret)
			return ret;
	}

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	if (pdata->buck2_gpiodvs) {
		if (pdata->buck3_gpiodvs || pdata->buck4_gpiodvs) {
			dev_err(&pdev->dev, "S5M8767 GPIO DVS NOT VALID\n");
			return -EINVAL;
		}
	}

	if (pdata->buck3_gpiodvs) {
		if (pdata->buck2_gpiodvs || pdata->buck4_gpiodvs) {
			dev_err(&pdev->dev, "S5M8767 GPIO DVS NOT VALID\n");
			return -EINVAL;
		}
	}

	if (pdata->buck4_gpiodvs) {
		if (pdata->buck2_gpiodvs || pdata->buck3_gpiodvs) {
			dev_err(&pdev->dev, "S5M8767 GPIO DVS NOT VALID\n");
			return -EINVAL;
		}
	}

	s5m8767 = devm_kzalloc(&pdev->dev, sizeof(struct s5m8767_info),
				GFP_KERNEL);
	if (!s5m8767)
		return -ENOMEM;

<<<<<<< HEAD
	size = sizeof(struct regulator_dev *) * (S5M8767_REG_MAX - 2);
	s5m8767->rdev = devm_kzalloc(&pdev->dev, size, GFP_KERNEL);
	if (!s5m8767->rdev)
		return -ENOMEM;

	rdev = s5m8767->rdev;
	s5m8767->dev = &pdev->dev;
	s5m8767->iodev = iodev;
	s5m8767->num_regulators = S5M8767_REG_MAX - 2;
=======
	s5m8767->dev = &pdev->dev;
	s5m8767->iodev = iodev;
	s5m8767->num_regulators = pdata->num_regulators;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	platform_set_drvdata(pdev, s5m8767);

	s5m8767->buck_gpioindex = pdata->buck_default_idx;
	s5m8767->buck2_gpiodvs = pdata->buck2_gpiodvs;
	s5m8767->buck3_gpiodvs = pdata->buck3_gpiodvs;
	s5m8767->buck4_gpiodvs = pdata->buck4_gpiodvs;
	s5m8767->buck_gpios[0] = pdata->buck_gpios[0];
	s5m8767->buck_gpios[1] = pdata->buck_gpios[1];
	s5m8767->buck_gpios[2] = pdata->buck_gpios[2];
<<<<<<< HEAD
=======
	s5m8767->buck_ds[0] = pdata->buck_ds[0];
	s5m8767->buck_ds[1] = pdata->buck_ds[1];
	s5m8767->buck_ds[2] = pdata->buck_ds[2];

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	s5m8767->ramp_delay = pdata->buck_ramp_delay;
	s5m8767->buck2_ramp = pdata->buck2_ramp_enable;
	s5m8767->buck3_ramp = pdata->buck3_ramp_enable;
	s5m8767->buck4_ramp = pdata->buck4_ramp_enable;
<<<<<<< HEAD
=======
	s5m8767->opmode = pdata->opmode;

	buck_init = s5m8767_convert_voltage_to_sel(&buck_voltage_val2,
						   pdata->buck2_init);

	regmap_write(s5m8767->iodev->regmap_pmic, S5M8767_REG_BUCK2DVS2,
			buck_init);

	buck_init = s5m8767_convert_voltage_to_sel(&buck_voltage_val2,
						   pdata->buck3_init);

	regmap_write(s5m8767->iodev->regmap_pmic, S5M8767_REG_BUCK3DVS2,
			buck_init);

	buck_init = s5m8767_convert_voltage_to_sel(&buck_voltage_val2,
						   pdata->buck4_init);

	regmap_write(s5m8767->iodev->regmap_pmic, S5M8767_REG_BUCK4DVS2,
			buck_init);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	for (i = 0; i < 8; i++) {
		if (s5m8767->buck2_gpiodvs) {
			s5m8767->buck2_vol[i] =
				s5m8767_convert_voltage_to_sel(
						&buck_voltage_val2,
<<<<<<< HEAD
						pdata->buck2_voltage[i],
						pdata->buck2_voltage[i] +
						buck_voltage_val2.step);
=======
						pdata->buck2_voltage[i]);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		}

		if (s5m8767->buck3_gpiodvs) {
			s5m8767->buck3_vol[i] =
				s5m8767_convert_voltage_to_sel(
						&buck_voltage_val2,
<<<<<<< HEAD
						pdata->buck3_voltage[i],
						pdata->buck3_voltage[i] +
						buck_voltage_val2.step);
=======
						pdata->buck3_voltage[i]);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		}

		if (s5m8767->buck4_gpiodvs) {
			s5m8767->buck4_vol[i] =
				s5m8767_convert_voltage_to_sel(
						&buck_voltage_val2,
<<<<<<< HEAD
						pdata->buck4_voltage[i],
						pdata->buck4_voltage[i] +
						buck_voltage_val2.step);
=======
						pdata->buck4_voltage[i]);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		}
	}

	if (pdata->buck2_gpiodvs || pdata->buck3_gpiodvs ||
<<<<<<< HEAD
		pdata->buck4_gpiodvs) {
		if (gpio_is_valid(pdata->buck_gpios[0]) &&
			gpio_is_valid(pdata->buck_gpios[1]) &&
			gpio_is_valid(pdata->buck_gpios[2])) {
			ret = gpio_request(pdata->buck_gpios[0],
						"S5M8767 SET1");
			if (ret == -EBUSY)
				dev_warn(&pdev->dev, "Duplicated gpio request for SET1\n");

			ret = gpio_request(pdata->buck_gpios[1],
					   "S5M8767 SET2");
			if (ret == -EBUSY)
				dev_warn(&pdev->dev, "Duplicated gpio request for SET2\n");

			ret = gpio_request(pdata->buck_gpios[2],
					   "S5M8767 SET3");
			if (ret == -EBUSY)
				dev_warn(&pdev->dev, "Duplicated gpio request for SET3\n");
			/* SET1 GPIO */
			gpio_direction_output(pdata->buck_gpios[0],
					(s5m8767->buck_gpioindex >> 2) & 0x1);
			/* SET2 GPIO */
			gpio_direction_output(pdata->buck_gpios[1],
					(s5m8767->buck_gpioindex >> 1) & 0x1);
			/* SET3 GPIO */
			gpio_direction_output(pdata->buck_gpios[2],
					(s5m8767->buck_gpioindex >> 0) & 0x1);
			ret = 0;
		} else {
			dev_err(&pdev->dev, "GPIO NOT VALID\n");
			ret = -EINVAL;
			return ret;
		}
	}

	s5m_reg_update(s5m8767->iodev, S5M8767_REG_BUCK2CTRL,
			(pdata->buck2_gpiodvs) ? (1 << 1) : (0 << 1), 1 << 1);
	s5m_reg_update(s5m8767->iodev, S5M8767_REG_BUCK3CTRL,
			(pdata->buck3_gpiodvs) ? (1 << 1) : (0 << 1), 1 << 1);
	s5m_reg_update(s5m8767->iodev, S5M8767_REG_BUCK4CTRL,
			(pdata->buck4_gpiodvs) ? (1 << 1) : (0 << 1), 1 << 1);
=======
						pdata->buck4_gpiodvs) {

		if (!gpio_is_valid(pdata->buck_gpios[0]) ||
			!gpio_is_valid(pdata->buck_gpios[1]) ||
			!gpio_is_valid(pdata->buck_gpios[2])) {
			dev_err(&pdev->dev, "GPIO NOT VALID\n");
			return -EINVAL;
		}

		ret = devm_gpio_request(&pdev->dev, pdata->buck_gpios[0],
					"S5M8767 SET1");
		if (ret)
			return ret;

		ret = devm_gpio_request(&pdev->dev, pdata->buck_gpios[1],
					"S5M8767 SET2");
		if (ret)
			return ret;

		ret = devm_gpio_request(&pdev->dev, pdata->buck_gpios[2],
					"S5M8767 SET3");
		if (ret)
			return ret;

		/* SET1 GPIO */
		gpio_direction_output(pdata->buck_gpios[0],
				(s5m8767->buck_gpioindex >> 2) & 0x1);
		/* SET2 GPIO */
		gpio_direction_output(pdata->buck_gpios[1],
				(s5m8767->buck_gpioindex >> 1) & 0x1);
		/* SET3 GPIO */
		gpio_direction_output(pdata->buck_gpios[2],
				(s5m8767->buck_gpioindex >> 0) & 0x1);
	}

	ret = devm_gpio_request(&pdev->dev, pdata->buck_ds[0], "S5M8767 DS2");
	if (ret)
		return ret;

	ret = devm_gpio_request(&pdev->dev, pdata->buck_ds[1], "S5M8767 DS3");
	if (ret)
		return ret;

	ret = devm_gpio_request(&pdev->dev, pdata->buck_ds[2], "S5M8767 DS4");
	if (ret)
		return ret;

	/* DS2 GPIO */
	gpio_direction_output(pdata->buck_ds[0], 0x0);
	/* DS3 GPIO */
	gpio_direction_output(pdata->buck_ds[1], 0x0);
	/* DS4 GPIO */
	gpio_direction_output(pdata->buck_ds[2], 0x0);

	regmap_update_bits(s5m8767->iodev->regmap_pmic,
			   S5M8767_REG_BUCK2CTRL, 1 << 1,
			   (pdata->buck2_gpiodvs) ? (1 << 1) : (0 << 1));
	regmap_update_bits(s5m8767->iodev->regmap_pmic,
			   S5M8767_REG_BUCK3CTRL, 1 << 1,
			   (pdata->buck3_gpiodvs) ? (1 << 1) : (0 << 1));
	regmap_update_bits(s5m8767->iodev->regmap_pmic,
			   S5M8767_REG_BUCK4CTRL, 1 << 1,
			   (pdata->buck4_gpiodvs) ? (1 << 1) : (0 << 1));
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	/* Initialize GPIO DVS registers */
	for (i = 0; i < 8; i++) {
		if (s5m8767->buck2_gpiodvs) {
<<<<<<< HEAD
			s5m_reg_write(s5m8767->iodev, S5M8767_REG_BUCK2DVS1 + i,
					   s5m8767->buck2_vol[i]);
		}

		if (s5m8767->buck3_gpiodvs) {
			s5m_reg_write(s5m8767->iodev, S5M8767_REG_BUCK3DVS1 + i,
					   s5m8767->buck3_vol[i]);
		}

		if (s5m8767->buck4_gpiodvs) {
			s5m_reg_write(s5m8767->iodev, S5M8767_REG_BUCK4DVS1 + i,
					   s5m8767->buck4_vol[i]);
		}
	}
	s5m_reg_update(s5m8767->iodev, S5M8767_REG_BUCK2CTRL, 0x78, 0xff);
	s5m_reg_update(s5m8767->iodev, S5M8767_REG_BUCK3CTRL, 0x58, 0xff);
	s5m_reg_update(s5m8767->iodev, S5M8767_REG_BUCK4CTRL, 0x78, 0xff);

	if (s5m8767->buck2_ramp)
		s5m_reg_update(s5m8767->iodev, S5M8767_REG_DVSRAMP, 0x08, 0x08);

	if (s5m8767->buck3_ramp)
		s5m_reg_update(s5m8767->iodev, S5M8767_REG_DVSRAMP, 0x04, 0x04);

	if (s5m8767->buck4_ramp)
		s5m_reg_update(s5m8767->iodev, S5M8767_REG_DVSRAMP, 0x02, 0x02);

	if (s5m8767->buck2_ramp || s5m8767->buck3_ramp
		|| s5m8767->buck4_ramp) {
		switch (s5m8767->ramp_delay) {
		case 15:
			s5m_reg_update(s5m8767->iodev, S5M8767_REG_DVSRAMP,
					0xc0, 0xf0);
			break;
		case 25:
			s5m_reg_update(s5m8767->iodev, S5M8767_REG_DVSRAMP,
					0xd0, 0xf0);
			break;
		case 50:
			s5m_reg_update(s5m8767->iodev, S5M8767_REG_DVSRAMP,
					0xe0, 0xf0);
			break;
		case 100:
			s5m_reg_update(s5m8767->iodev, S5M8767_REG_DVSRAMP,
					0xf0, 0xf0);
			break;
		default:
			s5m_reg_update(s5m8767->iodev, S5M8767_REG_DVSRAMP,
					0x90, 0xf0);
		}
	}

	for (i = 0; i < pdata->num_regulators; i++) {
		const struct s5m_voltage_desc *desc;
		int id = pdata->regulators[i].id;

		desc = reg_voltage_map[id];
		if (desc)
			regulators[id].n_voltages =
				(desc->max - desc->min) / desc->step + 1;

		rdev[i] = regulator_register(&regulators[id], s5m8767->dev,
				pdata->regulators[i].initdata, s5m8767, NULL);
		if (IS_ERR(rdev[i])) {
			ret = PTR_ERR(rdev[i]);
			dev_err(s5m8767->dev, "regulator init failed for %d\n",
					id);
			rdev[i] = NULL;
			goto err;
=======
			regmap_write(s5m8767->iodev->regmap_pmic,
					S5M8767_REG_BUCK2DVS1 + i,
					s5m8767->buck2_vol[i]);
		}

		if (s5m8767->buck3_gpiodvs) {
			regmap_write(s5m8767->iodev->regmap_pmic,
					S5M8767_REG_BUCK3DVS1 + i,
					s5m8767->buck3_vol[i]);
		}

		if (s5m8767->buck4_gpiodvs) {
			regmap_write(s5m8767->iodev->regmap_pmic,
					S5M8767_REG_BUCK4DVS1 + i,
					s5m8767->buck4_vol[i]);
		}
	}

	if (s5m8767->buck2_ramp)
		regmap_update_bits(s5m8767->iodev->regmap_pmic,
				S5M8767_REG_DVSRAMP, 0x08, 0x08);

	if (s5m8767->buck3_ramp)
		regmap_update_bits(s5m8767->iodev->regmap_pmic,
				S5M8767_REG_DVSRAMP, 0x04, 0x04);

	if (s5m8767->buck4_ramp)
		regmap_update_bits(s5m8767->iodev->regmap_pmic,
				S5M8767_REG_DVSRAMP, 0x02, 0x02);

	if (s5m8767->buck2_ramp || s5m8767->buck3_ramp
		|| s5m8767->buck4_ramp) {
		unsigned int val;
		switch (s5m8767->ramp_delay) {
		case 5:
			val = S5M8767_DVS_BUCK_RAMP_5;
			break;
		case 10:
			val = S5M8767_DVS_BUCK_RAMP_10;
			break;
		case 25:
			val = S5M8767_DVS_BUCK_RAMP_25;
			break;
		case 50:
			val = S5M8767_DVS_BUCK_RAMP_50;
			break;
		case 100:
			val = S5M8767_DVS_BUCK_RAMP_100;
			break;
		default:
			val = S5M8767_DVS_BUCK_RAMP_10;
		}
		regmap_update_bits(s5m8767->iodev->regmap_pmic,
					S5M8767_REG_DVSRAMP,
					S5M8767_DVS_BUCK_RAMP_MASK,
					val << S5M8767_DVS_BUCK_RAMP_SHIFT);
	}

	for (i = 0; i < pdata->num_regulators; i++) {
		const struct sec_voltage_desc *desc;
		unsigned int id = pdata->regulators[i].id;
		int enable_reg, enable_val;
		struct regulator_dev *rdev;

		BUILD_BUG_ON(ARRAY_SIZE(regulators) != ARRAY_SIZE(reg_voltage_map));
		if (WARN_ON_ONCE(id >= ARRAY_SIZE(regulators)))
			continue;

		desc = reg_voltage_map[id];
		if (desc) {
			regulators[id].n_voltages =
				(desc->max - desc->min) / desc->step + 1;
			regulators[id].min_uV = desc->min;
			regulators[id].uV_step = desc->step;
			regulators[id].vsel_reg =
				s5m8767_get_vsel_reg(id, s5m8767);
			if (id < S5M8767_BUCK1)
				regulators[id].vsel_mask = 0x3f;
			else
				regulators[id].vsel_mask = 0xff;

			ret = s5m8767_get_register(s5m8767, id, &enable_reg,
					     &enable_val);
			if (ret) {
				dev_err(s5m8767->dev, "error reading registers\n");
				return ret;
			}
			regulators[id].enable_reg = enable_reg;
			regulators[id].enable_mask = S5M8767_ENCTRL_MASK;
			regulators[id].enable_val = enable_val;
		}

		config.dev = s5m8767->dev;
		config.init_data = pdata->regulators[i].initdata;
		config.driver_data = s5m8767;
		config.regmap = iodev->regmap_pmic;
		config.of_node = pdata->regulators[i].reg_node;
		config.ena_gpiod = NULL;
		if (pdata->regulators[i].ext_control_gpiod) {
			/* Assigns config.ena_gpiod */
			s5m8767_regulator_config_ext_control(s5m8767,
					&pdata->regulators[i], &config);

			/*
			 * Hand the GPIO descriptor management over to the
			 * regulator core, remove it from devres management.
			 */
			devm_gpiod_unhinge(s5m8767->dev, config.ena_gpiod);
		}
		rdev = devm_regulator_register(&pdev->dev, &regulators[id],
						  &config);
		if (IS_ERR(rdev)) {
			ret = PTR_ERR(rdev);
			dev_err(s5m8767->dev, "regulator init failed for %d\n",
					id);
			return ret;
		}

		if (pdata->regulators[i].ext_control_gpiod) {
			ret = s5m8767_enable_ext_control(s5m8767, rdev);
			if (ret < 0) {
				dev_err(s5m8767->dev,
						"failed to enable gpio control over %s: %d\n",
						rdev->desc->name, ret);
				return ret;
			}
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		}
	}

	return 0;
<<<<<<< HEAD
err:
	for (i = 0; i < s5m8767->num_regulators; i++)
		if (rdev[i])
			regulator_unregister(rdev[i]);

	return ret;
}

static int __devexit s5m8767_pmic_remove(struct platform_device *pdev)
{
	struct s5m8767_info *s5m8767 = platform_get_drvdata(pdev);
	struct regulator_dev **rdev = s5m8767->rdev;
	int i;

	for (i = 0; i < s5m8767->num_regulators; i++)
		if (rdev[i])
			regulator_unregister(rdev[i]);

	return 0;
=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

static const struct platform_device_id s5m8767_pmic_id[] = {
	{ "s5m8767-pmic", 0},
	{ },
};
MODULE_DEVICE_TABLE(platform, s5m8767_pmic_id);

static struct platform_driver s5m8767_pmic_driver = {
	.driver = {
		.name = "s5m8767-pmic",
<<<<<<< HEAD
		.owner = THIS_MODULE,
	},
	.probe = s5m8767_pmic_probe,
	.remove = __devexit_p(s5m8767_pmic_remove),
	.id_table = s5m8767_pmic_id,
};

static int __init s5m8767_pmic_init(void)
{
	return platform_driver_register(&s5m8767_pmic_driver);
}
subsys_initcall(s5m8767_pmic_init);

static void __exit s5m8767_pmic_exit(void)
{
	platform_driver_unregister(&s5m8767_pmic_driver);
}
module_exit(s5m8767_pmic_exit);

/* Module information */
MODULE_AUTHOR("Sangbeom Kim <sbkim73@samsung.com>");
MODULE_DESCRIPTION("SAMSUNG S5M8767 Regulator Driver");
=======
		.probe_type = PROBE_PREFER_ASYNCHRONOUS,
	},
	.probe = s5m8767_pmic_probe,
	.id_table = s5m8767_pmic_id,
};
module_platform_driver(s5m8767_pmic_driver);

/* Module information */
MODULE_AUTHOR("Sangbeom Kim <sbkim73@samsung.com>");
MODULE_DESCRIPTION("Samsung S5M8767 Regulator Driver");
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
MODULE_LICENSE("GPL");
