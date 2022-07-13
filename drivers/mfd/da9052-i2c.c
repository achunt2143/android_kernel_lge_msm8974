<<<<<<< HEAD
=======
// SPDX-License-Identifier: GPL-2.0-or-later
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
/*
 * I2C access for DA9052 PMICs.
 *
 * Copyright(c) 2011 Dialog Semiconductor Ltd.
 *
 * Author: David Dajun Chen <dchen@diasemi.com>
<<<<<<< HEAD
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 */

#include <linux/device.h>
#include <linux/module.h>
#include <linux/input.h>
#include <linux/mfd/core.h>
#include <linux/i2c.h>
#include <linux/err.h>
<<<<<<< HEAD
=======
#include <linux/of.h>
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

#include <linux/mfd/da9052/da9052.h>
#include <linux/mfd/da9052/reg.h>

<<<<<<< HEAD
static int da9052_i2c_enable_multiwrite(struct da9052 *da9052)
=======

/* I2C safe register check */
static inline bool i2c_safe_reg(unsigned char reg)
{
	switch (reg) {
	case DA9052_STATUS_A_REG:
	case DA9052_STATUS_B_REG:
	case DA9052_STATUS_C_REG:
	case DA9052_STATUS_D_REG:
	case DA9052_ADC_RES_L_REG:
	case DA9052_ADC_RES_H_REG:
	case DA9052_VDD_RES_REG:
	case DA9052_ICHG_AV_REG:
	case DA9052_TBAT_RES_REG:
	case DA9052_ADCIN4_RES_REG:
	case DA9052_ADCIN5_RES_REG:
	case DA9052_ADCIN6_RES_REG:
	case DA9052_TJUNC_RES_REG:
	case DA9052_TSI_X_MSB_REG:
	case DA9052_TSI_Y_MSB_REG:
	case DA9052_TSI_LSB_REG:
	case DA9052_TSI_Z_MSB_REG:
		return true;
	default:
		return false;
	}
}

/*
 * There is an issue with DA9052 and DA9053_AA/BA/BB PMIC where the PMIC
 * gets lockup up or fails to respond following a system reset.
 * This fix is to follow any read or write with a dummy read to a safe
 * register.
 */
static int da9052_i2c_fix(struct da9052 *da9052, unsigned char reg)
{
	int val;

	switch (da9052->chip_id) {
	case DA9052:
	case DA9053_AA:
	case DA9053_BA:
	case DA9053_BB:
		/* A dummy read to a safe register address. */
		if (!i2c_safe_reg(reg))
			return regmap_read(da9052->regmap,
					   DA9052_PARK_REGISTER,
					   &val);
		break;
	case DA9053_BC:
	default:
		/*
		 * For other chips parking of I2C register
		 * to a safe place is not required.
		 */
		break;
	}

	return 0;
}

/*
 * According to errata item 24, multiwrite mode should be avoided
 * in order to prevent register data corruption after power-down.
 */
static int da9052_i2c_disable_multiwrite(struct da9052 *da9052)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
{
	int reg_val, ret;

	ret = regmap_read(da9052->regmap, DA9052_CONTROL_B_REG, &reg_val);
	if (ret < 0)
		return ret;

<<<<<<< HEAD
	if (reg_val & DA9052_CONTROL_B_WRITEMODE) {
		reg_val &= ~DA9052_CONTROL_B_WRITEMODE;
=======
	if (!(reg_val & DA9052_CONTROL_B_WRITEMODE)) {
		reg_val |= DA9052_CONTROL_B_WRITEMODE;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		ret = regmap_write(da9052->regmap, DA9052_CONTROL_B_REG,
				   reg_val);
		if (ret < 0)
			return ret;
	}

	return 0;
}

<<<<<<< HEAD
static int __devinit da9052_i2c_probe(struct i2c_client *client,
				       const struct i2c_device_id *id)
{
	struct da9052 *da9052;
	int ret;

	da9052 = kzalloc(sizeof(struct da9052), GFP_KERNEL);
	if (!da9052)
		return -ENOMEM;

	if (!i2c_check_functionality(client->adapter,
				     I2C_FUNC_SMBUS_BYTE_DATA)) {
		dev_info(&client->dev, "Error in %s:i2c_check_functionality\n",
			 __func__);
		ret = -ENODEV;
		goto err;
	}

	da9052->dev = &client->dev;
	da9052->chip_irq = client->irq;

	i2c_set_clientdata(client, da9052);

	da9052->regmap = regmap_init_i2c(client, &da9052_regmap_config);
	if (IS_ERR(da9052->regmap)) {
		ret = PTR_ERR(da9052->regmap);
		dev_err(&client->dev, "Failed to allocate register map: %d\n",
			ret);
		goto err;
	}

	ret = da9052_i2c_enable_multiwrite(da9052);
	if (ret < 0)
		goto err_regmap;

	ret = da9052_device_init(da9052, id->driver_data);
	if (ret != 0)
		goto err_regmap;

	return 0;

err_regmap:
	regmap_exit(da9052->regmap);
err:
	kfree(da9052);
	return ret;
}

static int __devexit da9052_i2c_remove(struct i2c_client *client)
{
	struct da9052 *da9052 = i2c_get_clientdata(client);

	da9052_device_exit(da9052);
	regmap_exit(da9052->regmap);
	kfree(da9052);

	return 0;
}

static struct i2c_device_id da9052_i2c_id[] = {
=======
static const struct i2c_device_id da9052_i2c_id[] = {
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	{"da9052", DA9052},
	{"da9053-aa", DA9053_AA},
	{"da9053-ba", DA9053_BA},
	{"da9053-bb", DA9053_BB},
<<<<<<< HEAD
	{}
};

static struct i2c_driver da9052_i2c_driver = {
	.probe = da9052_i2c_probe,
	.remove = __devexit_p(da9052_i2c_remove),
	.id_table = da9052_i2c_id,
	.driver = {
		.name = "da9052",
		.owner = THIS_MODULE,
=======
	{"da9053-bc", DA9053_BC},
	{}
};
MODULE_DEVICE_TABLE(i2c, da9052_i2c_id);

#ifdef CONFIG_OF
static const struct of_device_id dialog_dt_ids[] = {
	{ .compatible = "dlg,da9052", .data = &da9052_i2c_id[0] },
	{ .compatible = "dlg,da9053-aa", .data = &da9052_i2c_id[1] },
	{ .compatible = "dlg,da9053-ba", .data = &da9052_i2c_id[2] },
	{ .compatible = "dlg,da9053-bb", .data = &da9052_i2c_id[3] },
	{ .compatible = "dlg,da9053-bc", .data = &da9052_i2c_id[4] },
	{ /* sentinel */ }
};
#endif

static int da9052_i2c_probe(struct i2c_client *client)
{
	const struct i2c_device_id *id = i2c_client_get_device_id(client);
	struct da9052 *da9052;
	int ret;

	da9052 = devm_kzalloc(&client->dev, sizeof(struct da9052), GFP_KERNEL);
	if (!da9052)
		return -ENOMEM;

	da9052->dev = &client->dev;
	da9052->chip_irq = client->irq;
	da9052->fix_io = da9052_i2c_fix;

	i2c_set_clientdata(client, da9052);

	da9052->regmap = devm_regmap_init_i2c(client, &da9052_regmap_config);
	if (IS_ERR(da9052->regmap)) {
		ret = PTR_ERR(da9052->regmap);
		dev_err(&client->dev, "Failed to allocate register map: %d\n",
			ret);
		return ret;
	}

	ret = da9052_i2c_disable_multiwrite(da9052);
	if (ret < 0)
		return ret;

#ifdef CONFIG_OF
	if (!id)
		id = of_device_get_match_data(&client->dev);
#endif

	if (!id) {
		ret = -ENODEV;
		dev_err(&client->dev, "id is null.\n");
		return ret;
	}

	return da9052_device_init(da9052, id->driver_data);
}

static void da9052_i2c_remove(struct i2c_client *client)
{
	struct da9052 *da9052 = i2c_get_clientdata(client);

	da9052_device_exit(da9052);
}

static struct i2c_driver da9052_i2c_driver = {
	.probe = da9052_i2c_probe,
	.remove = da9052_i2c_remove,
	.id_table = da9052_i2c_id,
	.driver = {
		.name = "da9052",
#ifdef CONFIG_OF
		.of_match_table = dialog_dt_ids,
#endif
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	},
};

static int __init da9052_i2c_init(void)
{
	int ret;

	ret = i2c_add_driver(&da9052_i2c_driver);
	if (ret != 0) {
		pr_err("DA9052 I2C registration failed %d\n", ret);
		return ret;
	}

	return 0;
}
subsys_initcall(da9052_i2c_init);

static void __exit da9052_i2c_exit(void)
{
	i2c_del_driver(&da9052_i2c_driver);
}
module_exit(da9052_i2c_exit);

MODULE_AUTHOR("David Dajun Chen <dchen@diasemi.com>");
MODULE_DESCRIPTION("I2C driver for Dialog DA9052 PMIC");
<<<<<<< HEAD
MODULE_LICENSE("GPL");
=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
