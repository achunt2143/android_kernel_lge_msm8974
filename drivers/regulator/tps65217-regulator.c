<<<<<<< HEAD
=======
// SPDX-License-Identifier: GPL-2.0-only
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
/*
 * tps65217-regulator.c
 *
 * Regulator driver for TPS65217 PMIC
 *
<<<<<<< HEAD
 * Copyright (C) 2011 Texas Instruments Incorporated - http://www.ti.com/
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation version 2.
 *
 * This program is distributed "as is" WITHOUT ANY WARRANTY of any
 * kind, whether express or implied; without even the implied warranty
 * of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
=======
 * Copyright (C) 2011 Texas Instruments Incorporated - https://www.ti.com/
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 */

#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/device.h>
#include <linux/init.h>
#include <linux/err.h>
#include <linux/platform_device.h>

<<<<<<< HEAD
=======
#include <linux/regulator/of_regulator.h>
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#include <linux/regulator/driver.h>
#include <linux/regulator/machine.h>
#include <linux/mfd/tps65217.h>

<<<<<<< HEAD
#define TPS65217_REGULATOR(_name, _id, _ops, _n)	\
	{						\
		.name		= _name,		\
		.id		= _id,			\
=======
#define TPS65217_REGULATOR(_name, _id, _of_match, _ops, _n, _vr, _vm, _em, \
			   _t, _lr, _nlr,  _sr, _sm)	\
	{						\
		.name		= _name,		\
		.id		= _id,			\
		.of_match       = of_match_ptr(_of_match),    \
		.regulators_node= of_match_ptr("regulators"), \
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		.ops		= &_ops,		\
		.n_voltages	= _n,			\
		.type		= REGULATOR_VOLTAGE,	\
		.owner		= THIS_MODULE,		\
<<<<<<< HEAD
	}						\

#define TPS65217_INFO(_nm, _min, _max, _f1, _f2, _t, _n, _em, _vr, _vm)	\
	{						\
		.name		= _nm,			\
		.min_uV		= _min,			\
		.max_uV		= _max,			\
		.vsel_to_uv	= _f1,			\
		.uv_to_vsel	= _f2,			\
		.table		= _t,			\
		.table_len	= _n,			\
		.enable_mask	= _em,			\
		.set_vout_reg	= _vr,			\
		.set_vout_mask	= _vm,			\
	}

static const int LDO1_VSEL_table[] = {
=======
		.vsel_reg	= _vr,			\
		.vsel_mask	= _vm,			\
		.enable_reg	= TPS65217_REG_ENABLE,	\
		.enable_mask	= _em,			\
		.volt_table	= _t,			\
		.linear_ranges	= _lr,			\
		.n_linear_ranges = _nlr,		\
		.bypass_reg	= _sr,			\
		.bypass_mask	= _sm,			\
	}						\

static const unsigned int LDO1_VSEL_table[] = {
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	1000000, 1100000, 1200000, 1250000,
	1300000, 1350000, 1400000, 1500000,
	1600000, 1800000, 2500000, 2750000,
	2800000, 3000000, 3100000, 3300000,
};

<<<<<<< HEAD
static int tps65217_vsel_to_uv1(unsigned int vsel)
{
	int uV = 0;

	if (vsel > 63)
		return -EINVAL;

	if (vsel <= 24)
		uV = vsel * 25000 + 900000;
	else if (vsel <= 52)
		uV = (vsel - 24) * 50000 + 1500000;
	else if (vsel < 56)
		uV = (vsel - 52) * 100000 + 2900000;
	else
		uV = 3300000;

	return uV;
}

static int tps65217_uv_to_vsel1(int uV, unsigned int *vsel)
{
	if ((uV < 0) && (uV > 3300000))
		return -EINVAL;

	if (uV <= 1500000)
		*vsel = DIV_ROUND_UP(uV - 900000, 25000);
	else if (uV <= 2900000)
		*vsel = 24 + DIV_ROUND_UP(uV - 1500000, 50000);
	else if (uV < 3300000)
		*vsel = 52 + DIV_ROUND_UP(uV - 2900000, 100000);
	else
		*vsel = 56;

	return 0;
}

static int tps65217_vsel_to_uv2(unsigned int vsel)
{
	int uV = 0;

	if (vsel > 31)
		return -EINVAL;

	if (vsel <= 8)
		uV = vsel * 50000 + 1500000;
	else if (vsel <= 13)
		uV = (vsel - 8) * 100000 + 1900000;
	else
		uV = (vsel - 13) * 50000 + 2400000;

	return uV;
}

static int tps65217_uv_to_vsel2(int uV, unsigned int *vsel)
{
	if ((uV < 0) && (uV > 3300000))
		return -EINVAL;

	if (uV <= 1900000)
		*vsel = DIV_ROUND_UP(uV - 1500000, 50000);
	else if (uV <= 2400000)
		*vsel = 8 + DIV_ROUND_UP(uV - 1900000, 100000);
	else
		*vsel = 13 + DIV_ROUND_UP(uV - 2400000, 50000);

	return 0;
}

static struct tps_info tps65217_pmic_regs[] = {
	TPS65217_INFO("DCDC1", 900000, 1800000, tps65217_vsel_to_uv1,
			tps65217_uv_to_vsel1, NULL, 64, TPS65217_ENABLE_DC1_EN,
			TPS65217_REG_DEFDCDC1, TPS65217_DEFDCDCX_DCDC_MASK),
	TPS65217_INFO("DCDC2", 900000, 3300000, tps65217_vsel_to_uv1,
			tps65217_uv_to_vsel1, NULL, 64, TPS65217_ENABLE_DC2_EN,
			TPS65217_REG_DEFDCDC2, TPS65217_DEFDCDCX_DCDC_MASK),
	TPS65217_INFO("DCDC3", 900000, 1500000, tps65217_vsel_to_uv1,
			tps65217_uv_to_vsel1, NULL, 64, TPS65217_ENABLE_DC3_EN,
			TPS65217_REG_DEFDCDC3, TPS65217_DEFDCDCX_DCDC_MASK),
	TPS65217_INFO("LDO1", 1000000, 3300000, NULL, NULL, LDO1_VSEL_table,
			16, TPS65217_ENABLE_LDO1_EN, TPS65217_REG_DEFLDO1,
			TPS65217_DEFLDO1_LDO1_MASK),
	TPS65217_INFO("LDO2", 900000, 3300000, tps65217_vsel_to_uv1,
			tps65217_uv_to_vsel1, NULL, 64, TPS65217_ENABLE_LDO2_EN,
			TPS65217_REG_DEFLDO2, TPS65217_DEFLDO2_LDO2_MASK),
	TPS65217_INFO("LDO3", 1800000, 3300000, tps65217_vsel_to_uv2,
			tps65217_uv_to_vsel2, NULL, 32,
			TPS65217_ENABLE_LS1_EN | TPS65217_DEFLDO3_LDO3_EN,
			TPS65217_REG_DEFLS1, TPS65217_DEFLDO3_LDO3_MASK),
	TPS65217_INFO("LDO4", 1800000, 3300000, tps65217_vsel_to_uv2,
			tps65217_uv_to_vsel2, NULL, 32,
			TPS65217_ENABLE_LS2_EN | TPS65217_DEFLDO4_LDO4_EN,
			TPS65217_REG_DEFLS2, TPS65217_DEFLDO4_LDO4_MASK),
};

static int tps65217_pmic_is_enabled(struct regulator_dev *dev)
{
	int ret;
	struct tps65217 *tps = rdev_get_drvdata(dev);
	unsigned int data, rid = rdev_get_id(dev);

	if (rid < TPS65217_DCDC_1 || rid > TPS65217_LDO_4)
		return -EINVAL;

	ret = tps65217_reg_read(tps, TPS65217_REG_ENABLE, &data);
	if (ret)
		return ret;

	return (data & tps->info[rid]->enable_mask) ? 1 : 0;
}
=======
static const struct linear_range tps65217_uv1_ranges[] = {
	REGULATOR_LINEAR_RANGE(900000, 0, 24, 25000),
	REGULATOR_LINEAR_RANGE(1550000, 25, 52, 50000),
	REGULATOR_LINEAR_RANGE(3000000, 53, 55, 100000),
	REGULATOR_LINEAR_RANGE(3300000, 56, 63, 0),
};

static const struct linear_range tps65217_uv2_ranges[] = {
	REGULATOR_LINEAR_RANGE(1500000, 0, 8, 50000),
	REGULATOR_LINEAR_RANGE(2000000, 9, 13, 100000),
	REGULATOR_LINEAR_RANGE(2450000, 14, 31, 50000),
};
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

static int tps65217_pmic_enable(struct regulator_dev *dev)
{
	struct tps65217 *tps = rdev_get_drvdata(dev);
<<<<<<< HEAD
	unsigned int rid = rdev_get_id(dev);
=======
	int rid = rdev_get_id(dev);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	if (rid < TPS65217_DCDC_1 || rid > TPS65217_LDO_4)
		return -EINVAL;

	/* Enable the regulator and password protection is level 1 */
	return tps65217_set_bits(tps, TPS65217_REG_ENABLE,
<<<<<<< HEAD
				tps->info[rid]->enable_mask,
				tps->info[rid]->enable_mask,
				TPS65217_PROTECT_L1);
=======
				 dev->desc->enable_mask, dev->desc->enable_mask,
				 TPS65217_PROTECT_L1);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

static int tps65217_pmic_disable(struct regulator_dev *dev)
{
	struct tps65217 *tps = rdev_get_drvdata(dev);
<<<<<<< HEAD
	unsigned int rid = rdev_get_id(dev);
=======
	int rid = rdev_get_id(dev);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	if (rid < TPS65217_DCDC_1 || rid > TPS65217_LDO_4)
		return -EINVAL;

	/* Disable the regulator and password protection is level 1 */
	return tps65217_clear_bits(tps, TPS65217_REG_ENABLE,
<<<<<<< HEAD
			tps->info[rid]->enable_mask, TPS65217_PROTECT_L1);
}

static int tps65217_pmic_get_voltage_sel(struct regulator_dev *dev)
{
	int ret;
	struct tps65217 *tps = rdev_get_drvdata(dev);
	unsigned int selector, rid = rdev_get_id(dev);

	if (rid < TPS65217_DCDC_1 || rid > TPS65217_LDO_4)
		return -EINVAL;

	ret = tps65217_reg_read(tps, tps->info[rid]->set_vout_reg, &selector);
	if (ret)
		return ret;

	selector &= tps->info[rid]->set_vout_mask;

	return selector;
}

static int tps65217_pmic_ldo1_set_voltage_sel(struct regulator_dev *dev,
						unsigned selector)
{
	struct tps65217 *tps = rdev_get_drvdata(dev);
	int ldo = rdev_get_id(dev);

	if (ldo != TPS65217_LDO_1)
		return -EINVAL;

	if (selector >= tps->info[ldo]->table_len)
		return -EINVAL;

	/* Set the voltage based on vsel value and write protect level is 2 */
	return tps65217_set_bits(tps, tps->info[ldo]->set_vout_reg,
					tps->info[ldo]->set_vout_mask,
					selector, TPS65217_PROTECT_L2);
}

static int tps65217_pmic_set_voltage(struct regulator_dev *dev,
				  int min_uV, int max_uV, unsigned *selector)
=======
				   dev->desc->enable_mask, TPS65217_PROTECT_L1);
}

static int tps65217_pmic_set_voltage_sel(struct regulator_dev *dev,
					 unsigned selector)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
{
	int ret;
	struct tps65217 *tps = rdev_get_drvdata(dev);
	unsigned int rid = rdev_get_id(dev);

<<<<<<< HEAD
	/* LDO1 implements set_voltage_sel callback */
	if (rid == TPS65217_LDO_1)
		return -EINVAL;

	if (rid < TPS65217_DCDC_1 || rid > TPS65217_LDO_4)
		return -EINVAL;

	if (min_uV < tps->info[rid]->min_uV
		|| min_uV > tps->info[rid]->max_uV)
		return -EINVAL;

	if (max_uV < tps->info[rid]->min_uV
		|| max_uV > tps->info[rid]->max_uV)
		return -EINVAL;

	ret = tps->info[rid]->uv_to_vsel(min_uV, selector);
	if (ret)
		return ret;

	/* Set the voltage based on vsel value and write protect level is 2 */
	ret = tps65217_set_bits(tps, tps->info[rid]->set_vout_reg,
				tps->info[rid]->set_vout_mask,
				*selector, TPS65217_PROTECT_L2);
=======
	/* Set the voltage based on vsel value and write protect level is 2 */
	ret = tps65217_set_bits(tps, dev->desc->vsel_reg, dev->desc->vsel_mask,
				selector, TPS65217_PROTECT_L2);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	/* Set GO bit for DCDCx to initiate voltage transistion */
	switch (rid) {
	case TPS65217_DCDC_1 ... TPS65217_DCDC_3:
		ret = tps65217_set_bits(tps, TPS65217_REG_DEFSLEW,
				       TPS65217_DEFSLEW_GO, TPS65217_DEFSLEW_GO,
				       TPS65217_PROTECT_L2);
		break;
	}

	return ret;
}

<<<<<<< HEAD
static int tps65217_pmic_list_voltage(struct regulator_dev *dev,
					unsigned selector)
=======
static int tps65217_pmic_set_suspend_enable(struct regulator_dev *dev)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
{
	struct tps65217 *tps = rdev_get_drvdata(dev);
	unsigned int rid = rdev_get_id(dev);

<<<<<<< HEAD
	if (rid < TPS65217_DCDC_1 || rid > TPS65217_LDO_4)
		return -EINVAL;

	if (selector >= tps->info[rid]->table_len)
		return -EINVAL;

	if (tps->info[rid]->table)
		return tps->info[rid]->table[selector];

	return tps->info[rid]->vsel_to_uv(selector);
}

/* Operations permitted on DCDCx, LDO2, LDO3 and LDO4 */
static struct regulator_ops tps65217_pmic_ops = {
	.is_enabled		= tps65217_pmic_is_enabled,
	.enable			= tps65217_pmic_enable,
	.disable		= tps65217_pmic_disable,
	.get_voltage_sel	= tps65217_pmic_get_voltage_sel,
	.set_voltage		= tps65217_pmic_set_voltage,
	.list_voltage		= tps65217_pmic_list_voltage,
};

/* Operations permitted on LDO1 */
static struct regulator_ops tps65217_pmic_ldo1_ops = {
	.is_enabled		= tps65217_pmic_is_enabled,
	.enable			= tps65217_pmic_enable,
	.disable		= tps65217_pmic_disable,
	.get_voltage_sel	= tps65217_pmic_get_voltage_sel,
	.set_voltage_sel	= tps65217_pmic_ldo1_set_voltage_sel,
	.list_voltage		= tps65217_pmic_list_voltage,
};

static struct regulator_desc regulators[] = {
	TPS65217_REGULATOR("DCDC1", TPS65217_DCDC_1, tps65217_pmic_ops, 64),
	TPS65217_REGULATOR("DCDC2", TPS65217_DCDC_2, tps65217_pmic_ops, 64),
	TPS65217_REGULATOR("DCDC3", TPS65217_DCDC_3, tps65217_pmic_ops, 64),
	TPS65217_REGULATOR("LDO1", TPS65217_LDO_1, tps65217_pmic_ldo1_ops, 16),
	TPS65217_REGULATOR("LDO2", TPS65217_LDO_2, tps65217_pmic_ops, 64),
	TPS65217_REGULATOR("LDO3", TPS65217_LDO_3, tps65217_pmic_ops, 32),
	TPS65217_REGULATOR("LDO4", TPS65217_LDO_4, tps65217_pmic_ops, 32),
};

static int __devinit tps65217_regulator_probe(struct platform_device *pdev)
{
	struct regulator_dev *rdev;
	struct tps65217 *tps;
	struct tps_info *info = &tps65217_pmic_regs[pdev->id];

	/* Already set by core driver */
	tps = dev_to_tps65217(pdev->dev.parent);
	tps->info[pdev->id] = info;

	rdev = regulator_register(&regulators[pdev->id], &pdev->dev,
				  pdev->dev.platform_data, tps, NULL);
	if (IS_ERR(rdev))
		return PTR_ERR(rdev);

	platform_set_drvdata(pdev, rdev);

	return 0;
}

static int __devexit tps65217_regulator_remove(struct platform_device *pdev)
{
	struct regulator_dev *rdev = platform_get_drvdata(pdev);

	platform_set_drvdata(pdev, NULL);
	regulator_unregister(rdev);
=======
	if (rid > TPS65217_LDO_4)
		return -EINVAL;

	return tps65217_clear_bits(tps, dev->desc->bypass_reg,
				   dev->desc->bypass_mask,
				   TPS65217_PROTECT_L1);
}

static int tps65217_pmic_set_suspend_disable(struct regulator_dev *dev)
{
	struct tps65217 *tps = rdev_get_drvdata(dev);
	unsigned int rid = rdev_get_id(dev);

	if (rid > TPS65217_LDO_4)
		return -EINVAL;

	if (!tps->strobes[rid])
		return -EINVAL;

	return tps65217_set_bits(tps, dev->desc->bypass_reg,
				 dev->desc->bypass_mask,
				 tps->strobes[rid], TPS65217_PROTECT_L1);
}

/* Operations permitted on DCDCx, LDO2, LDO3 and LDO4 */
static const struct regulator_ops tps65217_pmic_ops = {
	.is_enabled		= regulator_is_enabled_regmap,
	.enable			= tps65217_pmic_enable,
	.disable		= tps65217_pmic_disable,
	.get_voltage_sel	= regulator_get_voltage_sel_regmap,
	.set_voltage_sel	= tps65217_pmic_set_voltage_sel,
	.list_voltage		= regulator_list_voltage_linear_range,
	.map_voltage		= regulator_map_voltage_linear_range,
	.set_suspend_enable	= tps65217_pmic_set_suspend_enable,
	.set_suspend_disable	= tps65217_pmic_set_suspend_disable,
};

/* Operations permitted on LDO1 */
static const struct regulator_ops tps65217_pmic_ldo1_ops = {
	.is_enabled		= regulator_is_enabled_regmap,
	.enable			= tps65217_pmic_enable,
	.disable		= tps65217_pmic_disable,
	.get_voltage_sel	= regulator_get_voltage_sel_regmap,
	.set_voltage_sel	= tps65217_pmic_set_voltage_sel,
	.list_voltage		= regulator_list_voltage_table,
	.map_voltage		= regulator_map_voltage_ascend,
	.set_suspend_enable	= tps65217_pmic_set_suspend_enable,
	.set_suspend_disable	= tps65217_pmic_set_suspend_disable,
};

static const struct regulator_desc regulators[] = {
	TPS65217_REGULATOR("DCDC1", TPS65217_DCDC_1, "dcdc1",
			   tps65217_pmic_ops, 64, TPS65217_REG_DEFDCDC1,
			   TPS65217_DEFDCDCX_DCDC_MASK, TPS65217_ENABLE_DC1_EN,
			   NULL, tps65217_uv1_ranges,
			   ARRAY_SIZE(tps65217_uv1_ranges), TPS65217_REG_SEQ1,
			   TPS65217_SEQ1_DC1_SEQ_MASK),
	TPS65217_REGULATOR("DCDC2", TPS65217_DCDC_2, "dcdc2",
			   tps65217_pmic_ops, 64, TPS65217_REG_DEFDCDC2,
			   TPS65217_DEFDCDCX_DCDC_MASK, TPS65217_ENABLE_DC2_EN,
			   NULL, tps65217_uv1_ranges,
			   ARRAY_SIZE(tps65217_uv1_ranges), TPS65217_REG_SEQ1,
			   TPS65217_SEQ1_DC2_SEQ_MASK),
	TPS65217_REGULATOR("DCDC3", TPS65217_DCDC_3, "dcdc3",
			   tps65217_pmic_ops, 64, TPS65217_REG_DEFDCDC3,
			   TPS65217_DEFDCDCX_DCDC_MASK, TPS65217_ENABLE_DC3_EN,
			   NULL, tps65217_uv1_ranges,
			   ARRAY_SIZE(tps65217_uv1_ranges), TPS65217_REG_SEQ2,
			   TPS65217_SEQ2_DC3_SEQ_MASK),
	TPS65217_REGULATOR("LDO1", TPS65217_LDO_1, "ldo1",
			   tps65217_pmic_ldo1_ops, 16, TPS65217_REG_DEFLDO1,
			   TPS65217_DEFLDO1_LDO1_MASK, TPS65217_ENABLE_LDO1_EN,
			   LDO1_VSEL_table, NULL, 0, TPS65217_REG_SEQ2,
			   TPS65217_SEQ2_LDO1_SEQ_MASK),
	TPS65217_REGULATOR("LDO2", TPS65217_LDO_2, "ldo2", tps65217_pmic_ops,
			   64, TPS65217_REG_DEFLDO2,
			   TPS65217_DEFLDO2_LDO2_MASK, TPS65217_ENABLE_LDO2_EN,
			   NULL, tps65217_uv1_ranges,
			   ARRAY_SIZE(tps65217_uv1_ranges), TPS65217_REG_SEQ3,
			   TPS65217_SEQ3_LDO2_SEQ_MASK),
	TPS65217_REGULATOR("LDO3", TPS65217_LDO_3, "ldo3", tps65217_pmic_ops,
			   32, TPS65217_REG_DEFLS1, TPS65217_DEFLDO3_LDO3_MASK,
			   TPS65217_ENABLE_LS1_EN | TPS65217_DEFLDO3_LDO3_EN,
			   NULL, tps65217_uv2_ranges,
			   ARRAY_SIZE(tps65217_uv2_ranges), TPS65217_REG_SEQ3,
			   TPS65217_SEQ3_LDO3_SEQ_MASK),
	TPS65217_REGULATOR("LDO4", TPS65217_LDO_4, "ldo4", tps65217_pmic_ops,
			   32, TPS65217_REG_DEFLS2, TPS65217_DEFLDO4_LDO4_MASK,
			   TPS65217_ENABLE_LS2_EN | TPS65217_DEFLDO4_LDO4_EN,
			   NULL, tps65217_uv2_ranges,
			   ARRAY_SIZE(tps65217_uv2_ranges), TPS65217_REG_SEQ4,
			   TPS65217_SEQ4_LDO4_SEQ_MASK),
};

static int tps65217_regulator_probe(struct platform_device *pdev)
{
	struct tps65217 *tps = dev_get_drvdata(pdev->dev.parent);
	struct tps65217_board *pdata = dev_get_platdata(tps->dev);
	struct regulator_dev *rdev;
	struct regulator_config config = { };
	int i, ret;
	unsigned int val;

	/* Allocate memory for strobes */
	tps->strobes = devm_kcalloc(&pdev->dev,
				    TPS65217_NUM_REGULATOR, sizeof(u8),
				    GFP_KERNEL);
	if (!tps->strobes)
		return -ENOMEM;

	platform_set_drvdata(pdev, tps);

	for (i = 0; i < TPS65217_NUM_REGULATOR; i++) {
		/* Register the regulators */
		config.dev = tps->dev;
		if (pdata)
			config.init_data = pdata->tps65217_init_data[i];
		config.driver_data = tps;
		config.regmap = tps->regmap;

		rdev = devm_regulator_register(&pdev->dev, &regulators[i],
					       &config);
		if (IS_ERR(rdev)) {
			dev_err(tps->dev, "failed to register %s regulator\n",
				pdev->name);
			return PTR_ERR(rdev);
		}

		/* Store default strobe info */
		ret = tps65217_reg_read(tps, regulators[i].bypass_reg, &val);
		if (ret)
			return ret;

		tps->strobes[i] = val & regulators[i].bypass_mask;
	}
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	return 0;
}

static struct platform_driver tps65217_regulator_driver = {
	.driver = {
		.name = "tps65217-pmic",
<<<<<<< HEAD
	},
	.probe = tps65217_regulator_probe,
	.remove = __devexit_p(tps65217_regulator_remove),
=======
		.probe_type = PROBE_PREFER_ASYNCHRONOUS,
	},
	.probe = tps65217_regulator_probe,
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
};

static int __init tps65217_regulator_init(void)
{
	return platform_driver_register(&tps65217_regulator_driver);
}
subsys_initcall(tps65217_regulator_init);

static void __exit tps65217_regulator_exit(void)
{
	platform_driver_unregister(&tps65217_regulator_driver);
}
module_exit(tps65217_regulator_exit);

MODULE_AUTHOR("AnilKumar Ch <anilkumar@ti.com>");
MODULE_DESCRIPTION("TPS65217 voltage regulator driver");
MODULE_ALIAS("platform:tps65217-pmic");
MODULE_LICENSE("GPL v2");
