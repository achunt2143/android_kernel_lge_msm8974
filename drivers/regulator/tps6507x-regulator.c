<<<<<<< HEAD
=======
// SPDX-License-Identifier: GPL-2.0-only
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
/*
 * tps6507x-regulator.c
 *
 * Regulator driver for TPS65073 PMIC
 *
<<<<<<< HEAD
 * Copyright (C) 2009 Texas Instrument Incorporated - http://www.ti.com/
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation version 2.
 *
 * This program is distributed "as is" WITHOUT ANY WARRANTY of any kind,
 * whether express or implied; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * General Public License for more details.
=======
 * Copyright (C) 2009 Texas Instrument Incorporated - https://www.ti.com/
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 */

#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/init.h>
#include <linux/err.h>
#include <linux/platform_device.h>
#include <linux/regulator/driver.h>
#include <linux/regulator/machine.h>
#include <linux/regulator/tps6507x.h>
<<<<<<< HEAD
#include <linux/delay.h>
#include <linux/slab.h>
#include <linux/mfd/tps6507x.h>
=======
#include <linux/of.h>
#include <linux/slab.h>
#include <linux/mfd/tps6507x.h>
#include <linux/regulator/of_regulator.h>
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

/* DCDC's */
#define TPS6507X_DCDC_1				0
#define TPS6507X_DCDC_2				1
#define TPS6507X_DCDC_3				2
/* LDOs */
#define TPS6507X_LDO_1				3
#define TPS6507X_LDO_2				4

#define TPS6507X_MAX_REG_ID			TPS6507X_LDO_2

/* Number of step-down converters available */
#define TPS6507X_NUM_DCDC			3
/* Number of LDO voltage regulators  available */
#define TPS6507X_NUM_LDO			2
/* Number of total regulators available */
#define TPS6507X_NUM_REGULATOR		(TPS6507X_NUM_DCDC + TPS6507X_NUM_LDO)

<<<<<<< HEAD
/* Supported voltage values for regulators (in milliVolts) */
static const u16 VDCDCx_VSEL_table[] = {
	725, 750, 775, 800,
	825, 850, 875, 900,
	925, 950, 975, 1000,
	1025, 1050, 1075, 1100,
	1125, 1150, 1175, 1200,
	1225, 1250, 1275, 1300,
	1325, 1350, 1375, 1400,
	1425, 1450, 1475, 1500,
	1550, 1600, 1650, 1700,
	1750, 1800, 1850, 1900,
	1950, 2000, 2050, 2100,
	2150, 2200, 2250, 2300,
	2350, 2400, 2450, 2500,
	2550, 2600, 2650, 2700,
	2750, 2800, 2850, 2900,
	3000, 3100, 3200, 3300,
};

static const u16 LDO1_VSEL_table[] = {
	1000, 1100, 1200, 1250,
	1300, 1350, 1400, 1500,
	1600, 1800, 2500, 2750,
	2800, 3000, 3100, 3300,
};

static const u16 LDO2_VSEL_table[] = {
	725, 750, 775, 800,
	825, 850, 875, 900,
	925, 950, 975, 1000,
	1025, 1050, 1075, 1100,
	1125, 1150, 1175, 1200,
	1225, 1250, 1275, 1300,
	1325, 1350, 1375, 1400,
	1425, 1450, 1475, 1500,
	1550, 1600, 1650, 1700,
	1750, 1800, 1850, 1900,
	1950, 2000, 2050, 2100,
	2150, 2200, 2250, 2300,
	2350, 2400, 2450, 2500,
	2550, 2600, 2650, 2700,
	2750, 2800, 2850, 2900,
	3000, 3100, 3200, 3300,
};

struct tps_info {
	const char *name;
	unsigned min_uV;
	unsigned max_uV;
	u8 table_len;
	const u16 *table;
=======
/* Supported voltage values for regulators (in microVolts) */
static const unsigned int VDCDCx_VSEL_table[] = {
	725000, 750000, 775000, 800000,
	825000, 850000, 875000, 900000,
	925000, 950000, 975000, 1000000,
	1025000, 1050000, 1075000, 1100000,
	1125000, 1150000, 1175000, 1200000,
	1225000, 1250000, 1275000, 1300000,
	1325000, 1350000, 1375000, 1400000,
	1425000, 1450000, 1475000, 1500000,
	1550000, 1600000, 1650000, 1700000,
	1750000, 1800000, 1850000, 1900000,
	1950000, 2000000, 2050000, 2100000,
	2150000, 2200000, 2250000, 2300000,
	2350000, 2400000, 2450000, 2500000,
	2550000, 2600000, 2650000, 2700000,
	2750000, 2800000, 2850000, 2900000,
	3000000, 3100000, 3200000, 3300000,
};

static const unsigned int LDO1_VSEL_table[] = {
	1000000, 1100000, 1200000, 1250000,
	1300000, 1350000, 1400000, 1500000,
	1600000, 1800000, 2500000, 2750000,
	2800000, 3000000, 3100000, 3300000,
};

/* The voltage mapping table for LDO2 is the same as VDCDCx */
#define LDO2_VSEL_table VDCDCx_VSEL_table

struct tps_info {
	const char *name;
	u8 table_len;
	const unsigned int *table;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	/* Does DCDC high or the low register defines output voltage? */
	bool defdcdc_default;
};

static struct tps_info tps6507x_pmic_regs[] = {
	{
		.name = "VDCDC1",
<<<<<<< HEAD
		.min_uV = 725000,
		.max_uV = 3300000,
=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		.table_len = ARRAY_SIZE(VDCDCx_VSEL_table),
		.table = VDCDCx_VSEL_table,
	},
	{
		.name = "VDCDC2",
<<<<<<< HEAD
		.min_uV = 725000,
		.max_uV = 3300000,
=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		.table_len = ARRAY_SIZE(VDCDCx_VSEL_table),
		.table = VDCDCx_VSEL_table,
	},
	{
		.name = "VDCDC3",
<<<<<<< HEAD
		.min_uV = 725000,
		.max_uV = 3300000,
=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		.table_len = ARRAY_SIZE(VDCDCx_VSEL_table),
		.table = VDCDCx_VSEL_table,
	},
	{
		.name = "LDO1",
<<<<<<< HEAD
		.min_uV = 1000000,
		.max_uV = 3300000,
=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		.table_len = ARRAY_SIZE(LDO1_VSEL_table),
		.table = LDO1_VSEL_table,
	},
	{
		.name = "LDO2",
<<<<<<< HEAD
		.min_uV = 725000,
		.max_uV = 3300000,
=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		.table_len = ARRAY_SIZE(LDO2_VSEL_table),
		.table = LDO2_VSEL_table,
	},
};

struct tps6507x_pmic {
	struct regulator_desc desc[TPS6507X_NUM_REGULATOR];
	struct tps6507x_dev *mfd;
<<<<<<< HEAD
	struct regulator_dev *rdev[TPS6507X_NUM_REGULATOR];
=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	struct tps_info *info[TPS6507X_NUM_REGULATOR];
	struct mutex io_lock;
};
static inline int tps6507x_pmic_read(struct tps6507x_pmic *tps, u8 reg)
{
	u8 val;
	int err;

	err = tps->mfd->read_dev(tps->mfd, reg, 1, &val);

	if (err)
		return err;

	return val;
}

static inline int tps6507x_pmic_write(struct tps6507x_pmic *tps, u8 reg, u8 val)
{
	return tps->mfd->write_dev(tps->mfd, reg, 1, &val);
}

static int tps6507x_pmic_set_bits(struct tps6507x_pmic *tps, u8 reg, u8 mask)
{
	int err, data;

	mutex_lock(&tps->io_lock);

	data = tps6507x_pmic_read(tps, reg);
	if (data < 0) {
		dev_err(tps->mfd->dev, "Read from reg 0x%x failed\n", reg);
		err = data;
		goto out;
	}

	data |= mask;
	err = tps6507x_pmic_write(tps, reg, data);
	if (err)
		dev_err(tps->mfd->dev, "Write for reg 0x%x failed\n", reg);

out:
	mutex_unlock(&tps->io_lock);
	return err;
}

static int tps6507x_pmic_clear_bits(struct tps6507x_pmic *tps, u8 reg, u8 mask)
{
	int err, data;

	mutex_lock(&tps->io_lock);

	data = tps6507x_pmic_read(tps, reg);
	if (data < 0) {
		dev_err(tps->mfd->dev, "Read from reg 0x%x failed\n", reg);
		err = data;
		goto out;
	}

	data &= ~mask;
	err = tps6507x_pmic_write(tps, reg, data);
	if (err)
		dev_err(tps->mfd->dev, "Write for reg 0x%x failed\n", reg);

out:
	mutex_unlock(&tps->io_lock);
	return err;
}

static int tps6507x_pmic_reg_read(struct tps6507x_pmic *tps, u8 reg)
{
	int data;

	mutex_lock(&tps->io_lock);

	data = tps6507x_pmic_read(tps, reg);
	if (data < 0)
		dev_err(tps->mfd->dev, "Read from reg 0x%x failed\n", reg);

	mutex_unlock(&tps->io_lock);
	return data;
}

static int tps6507x_pmic_reg_write(struct tps6507x_pmic *tps, u8 reg, u8 val)
{
	int err;

	mutex_lock(&tps->io_lock);

	err = tps6507x_pmic_write(tps, reg, val);
	if (err < 0)
		dev_err(tps->mfd->dev, "Write for reg 0x%x failed\n", reg);

	mutex_unlock(&tps->io_lock);
	return err;
}

static int tps6507x_pmic_is_enabled(struct regulator_dev *dev)
{
	struct tps6507x_pmic *tps = rdev_get_drvdata(dev);
	int data, rid = rdev_get_id(dev);
	u8 shift;

	if (rid < TPS6507X_DCDC_1 || rid > TPS6507X_LDO_2)
		return -EINVAL;

	shift = TPS6507X_MAX_REG_ID - rid;
	data = tps6507x_pmic_reg_read(tps, TPS6507X_REG_CON_CTRL1);

	if (data < 0)
		return data;
	else
		return (data & 1<<shift) ? 1 : 0;
}

static int tps6507x_pmic_enable(struct regulator_dev *dev)
{
	struct tps6507x_pmic *tps = rdev_get_drvdata(dev);
	int rid = rdev_get_id(dev);
	u8 shift;

	if (rid < TPS6507X_DCDC_1 || rid > TPS6507X_LDO_2)
		return -EINVAL;

	shift = TPS6507X_MAX_REG_ID - rid;
	return tps6507x_pmic_set_bits(tps, TPS6507X_REG_CON_CTRL1, 1 << shift);
}

static int tps6507x_pmic_disable(struct regulator_dev *dev)
{
	struct tps6507x_pmic *tps = rdev_get_drvdata(dev);
	int rid = rdev_get_id(dev);
	u8 shift;

	if (rid < TPS6507X_DCDC_1 || rid > TPS6507X_LDO_2)
		return -EINVAL;

	shift = TPS6507X_MAX_REG_ID - rid;
	return tps6507x_pmic_clear_bits(tps, TPS6507X_REG_CON_CTRL1,
					1 << shift);
}

<<<<<<< HEAD
static int tps6507x_pmic_get_voltage(struct regulator_dev *dev)
=======
static int tps6507x_pmic_get_voltage_sel(struct regulator_dev *dev)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
{
	struct tps6507x_pmic *tps = rdev_get_drvdata(dev);
	int data, rid = rdev_get_id(dev);
	u8 reg, mask;

	switch (rid) {
	case TPS6507X_DCDC_1:
		reg = TPS6507X_REG_DEFDCDC1;
		mask = TPS6507X_DEFDCDCX_DCDC_MASK;
		break;
	case TPS6507X_DCDC_2:
		if (tps->info[rid]->defdcdc_default)
			reg = TPS6507X_REG_DEFDCDC2_HIGH;
		else
			reg = TPS6507X_REG_DEFDCDC2_LOW;
		mask = TPS6507X_DEFDCDCX_DCDC_MASK;
		break;
	case TPS6507X_DCDC_3:
		if (tps->info[rid]->defdcdc_default)
			reg = TPS6507X_REG_DEFDCDC3_HIGH;
		else
			reg = TPS6507X_REG_DEFDCDC3_LOW;
		mask = TPS6507X_DEFDCDCX_DCDC_MASK;
		break;
	case TPS6507X_LDO_1:
		reg = TPS6507X_REG_LDO_CTRL1;
		mask = TPS6507X_REG_LDO_CTRL1_LDO1_MASK;
		break;
	case TPS6507X_LDO_2:
		reg = TPS6507X_REG_DEFLDO2;
		mask = TPS6507X_REG_DEFLDO2_LDO2_MASK;
		break;
	default:
		return -EINVAL;
	}

	data = tps6507x_pmic_reg_read(tps, reg);
	if (data < 0)
		return data;

	data &= mask;
<<<<<<< HEAD
	return tps->info[rid]->table[data] * 1000;
=======
	return data;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

static int tps6507x_pmic_set_voltage_sel(struct regulator_dev *dev,
					  unsigned selector)
{
	struct tps6507x_pmic *tps = rdev_get_drvdata(dev);
	int data, rid = rdev_get_id(dev);
	u8 reg, mask;

	switch (rid) {
	case TPS6507X_DCDC_1:
		reg = TPS6507X_REG_DEFDCDC1;
		mask = TPS6507X_DEFDCDCX_DCDC_MASK;
		break;
	case TPS6507X_DCDC_2:
		if (tps->info[rid]->defdcdc_default)
			reg = TPS6507X_REG_DEFDCDC2_HIGH;
		else
			reg = TPS6507X_REG_DEFDCDC2_LOW;
		mask = TPS6507X_DEFDCDCX_DCDC_MASK;
		break;
	case TPS6507X_DCDC_3:
		if (tps->info[rid]->defdcdc_default)
			reg = TPS6507X_REG_DEFDCDC3_HIGH;
		else
			reg = TPS6507X_REG_DEFDCDC3_LOW;
		mask = TPS6507X_DEFDCDCX_DCDC_MASK;
		break;
	case TPS6507X_LDO_1:
		reg = TPS6507X_REG_LDO_CTRL1;
		mask = TPS6507X_REG_LDO_CTRL1_LDO1_MASK;
		break;
	case TPS6507X_LDO_2:
		reg = TPS6507X_REG_DEFLDO2;
		mask = TPS6507X_REG_DEFLDO2_LDO2_MASK;
		break;
	default:
		return -EINVAL;
	}

	data = tps6507x_pmic_reg_read(tps, reg);
	if (data < 0)
		return data;

	data &= ~mask;
	data |= selector;

	return tps6507x_pmic_reg_write(tps, reg, data);
}

<<<<<<< HEAD
static int tps6507x_pmic_list_voltage(struct regulator_dev *dev,
					unsigned selector)
{
	struct tps6507x_pmic *tps = rdev_get_drvdata(dev);
	int rid = rdev_get_id(dev);

	if (rid < TPS6507X_DCDC_1 || rid > TPS6507X_LDO_2)
		return -EINVAL;

	if (selector >= tps->info[rid]->table_len)
		return -EINVAL;
	else
		return tps->info[rid]->table[selector] * 1000;
}

static struct regulator_ops tps6507x_pmic_ops = {
	.is_enabled = tps6507x_pmic_is_enabled,
	.enable = tps6507x_pmic_enable,
	.disable = tps6507x_pmic_disable,
	.get_voltage = tps6507x_pmic_get_voltage,
	.set_voltage_sel = tps6507x_pmic_set_voltage_sel,
	.list_voltage = tps6507x_pmic_list_voltage,
};

static __devinit int tps6507x_pmic_probe(struct platform_device *pdev)
{
	struct tps6507x_dev *tps6507x_dev = dev_get_drvdata(pdev->dev.parent);
	struct tps_info *info = &tps6507x_pmic_regs[0];
	struct regulator_init_data *init_data;
=======
static const struct regulator_ops tps6507x_pmic_ops = {
	.is_enabled = tps6507x_pmic_is_enabled,
	.enable = tps6507x_pmic_enable,
	.disable = tps6507x_pmic_disable,
	.get_voltage_sel = tps6507x_pmic_get_voltage_sel,
	.set_voltage_sel = tps6507x_pmic_set_voltage_sel,
	.list_voltage = regulator_list_voltage_table,
	.map_voltage = regulator_map_voltage_ascend,
};

static int tps6507x_pmic_of_parse_cb(struct device_node *np,
				     const struct regulator_desc *desc,
				     struct regulator_config *config)
{
	struct tps6507x_pmic *tps = config->driver_data;
	struct tps_info *info = tps->info[desc->id];
	u32 prop;
	int ret;

	ret = of_property_read_u32(np, "ti,defdcdc_default", &prop);
	if (!ret)
		info->defdcdc_default = prop;

	return 0;
}

static int tps6507x_pmic_probe(struct platform_device *pdev)
{
	struct tps6507x_dev *tps6507x_dev = dev_get_drvdata(pdev->dev.parent);
	struct tps_info *info = &tps6507x_pmic_regs[0];
	struct regulator_config config = { };
	struct regulator_init_data *init_data = NULL;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	struct regulator_dev *rdev;
	struct tps6507x_pmic *tps;
	struct tps6507x_board *tps_board;
	int i;
<<<<<<< HEAD
	int error;
=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	/**
	 * tps_board points to pmic related constants
	 * coming from the board-evm file.
	 */

	tps_board = dev_get_platdata(tps6507x_dev->dev);
<<<<<<< HEAD
	if (!tps_board)
		return -EINVAL;

	/**
	 * init_data points to array of regulator_init structures
	 * coming from the board-evm file.
	 */
	init_data = tps_board->tps6507x_pmic_init_data;
	if (!init_data)
		return -EINVAL;

	tps = kzalloc(sizeof(*tps), GFP_KERNEL);
=======
	if (tps_board)
		init_data = tps_board->tps6507x_pmic_init_data;

	tps = devm_kzalloc(&pdev->dev, sizeof(*tps), GFP_KERNEL);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	if (!tps)
		return -ENOMEM;

	mutex_init(&tps->io_lock);

	/* common for all regulators */
	tps->mfd = tps6507x_dev;

<<<<<<< HEAD
	for (i = 0; i < TPS6507X_NUM_REGULATOR; i++, info++, init_data++) {
		/* Register the regulators */
		tps->info[i] = info;
		if (init_data->driver_data) {
			struct tps6507x_reg_platform_data *data =
							init_data->driver_data;
			tps->info[i]->defdcdc_default = data->defdcdc_default;
		}

		tps->desc[i].name = info->name;
		tps->desc[i].id = i;
		tps->desc[i].n_voltages = info->table_len;
=======
	for (i = 0; i < TPS6507X_NUM_REGULATOR; i++, info++) {
		/* Register the regulators */
		tps->info[i] = info;
		if (init_data && init_data[i].driver_data) {
			struct tps6507x_reg_platform_data *data =
					init_data[i].driver_data;
			info->defdcdc_default = data->defdcdc_default;
		}

		tps->desc[i].name = info->name;
		tps->desc[i].of_match = of_match_ptr(info->name);
		tps->desc[i].regulators_node = of_match_ptr("regulators");
		tps->desc[i].of_parse_cb = tps6507x_pmic_of_parse_cb;
		tps->desc[i].id = i;
		tps->desc[i].n_voltages = info->table_len;
		tps->desc[i].volt_table = info->table;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		tps->desc[i].ops = &tps6507x_pmic_ops;
		tps->desc[i].type = REGULATOR_VOLTAGE;
		tps->desc[i].owner = THIS_MODULE;

<<<<<<< HEAD
		rdev = regulator_register(&tps->desc[i],
					tps6507x_dev->dev, init_data, tps, NULL);
=======
		config.dev = tps6507x_dev->dev;
		config.init_data = init_data;
		config.driver_data = tps;

		rdev = devm_regulator_register(&pdev->dev, &tps->desc[i],
					       &config);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		if (IS_ERR(rdev)) {
			dev_err(tps6507x_dev->dev,
				"failed to register %s regulator\n",
				pdev->name);
<<<<<<< HEAD
			error = PTR_ERR(rdev);
			goto fail;
		}

		/* Save regulator for cleanup */
		tps->rdev[i] = rdev;
=======
			return PTR_ERR(rdev);
		}
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	}

	tps6507x_dev->pmic = tps;
	platform_set_drvdata(pdev, tps6507x_dev);

	return 0;
<<<<<<< HEAD

fail:
	while (--i >= 0)
		regulator_unregister(tps->rdev[i]);

	kfree(tps);
	return error;
}

static int __devexit tps6507x_pmic_remove(struct platform_device *pdev)
{
	struct tps6507x_dev *tps6507x_dev = platform_get_drvdata(pdev);
	struct tps6507x_pmic *tps = tps6507x_dev->pmic;
	int i;

	for (i = 0; i < TPS6507X_NUM_REGULATOR; i++)
		regulator_unregister(tps->rdev[i]);

	kfree(tps);

	return 0;
=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

static struct platform_driver tps6507x_pmic_driver = {
	.driver = {
		.name = "tps6507x-pmic",
<<<<<<< HEAD
		.owner = THIS_MODULE,
	},
	.probe = tps6507x_pmic_probe,
	.remove = __devexit_p(tps6507x_pmic_remove),
=======
		.probe_type = PROBE_PREFER_ASYNCHRONOUS,
	},
	.probe = tps6507x_pmic_probe,
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
};

static int __init tps6507x_pmic_init(void)
{
	return platform_driver_register(&tps6507x_pmic_driver);
}
subsys_initcall(tps6507x_pmic_init);

static void __exit tps6507x_pmic_cleanup(void)
{
	platform_driver_unregister(&tps6507x_pmic_driver);
}
module_exit(tps6507x_pmic_cleanup);

MODULE_AUTHOR("Texas Instruments");
MODULE_DESCRIPTION("TPS6507x voltage regulator driver");
MODULE_LICENSE("GPL v2");
MODULE_ALIAS("platform:tps6507x-pmic");
