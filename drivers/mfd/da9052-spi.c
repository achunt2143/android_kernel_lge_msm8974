<<<<<<< HEAD
=======
// SPDX-License-Identifier: GPL-2.0-or-later
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
/*
 * SPI access for Dialog DA9052 PMICs.
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
#include <linux/spi/spi.h>
#include <linux/err.h>

#include <linux/mfd/da9052/da9052.h>

<<<<<<< HEAD
static int __devinit da9052_spi_probe(struct spi_device *spi)
{
	int ret;
	const struct spi_device_id *id = spi_get_device_id(spi);
	struct da9052 *da9052 = kzalloc(sizeof(struct da9052), GFP_KERNEL);

	if (!da9052)
		return -ENOMEM;

	spi->mode = SPI_MODE_0 | SPI_CPOL;
=======
static int da9052_spi_probe(struct spi_device *spi)
{
	struct regmap_config config;
	int ret;
	const struct spi_device_id *id = spi_get_device_id(spi);
	struct da9052 *da9052;

	da9052 = devm_kzalloc(&spi->dev, sizeof(struct da9052), GFP_KERNEL);
	if (!da9052)
		return -ENOMEM;

	spi->mode = SPI_MODE_0;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	spi->bits_per_word = 8;
	spi_setup(spi);

	da9052->dev = &spi->dev;
	da9052->chip_irq = spi->irq;

<<<<<<< HEAD
	dev_set_drvdata(&spi->dev, da9052);

	da9052_regmap_config.read_flag_mask = 1;
	da9052_regmap_config.write_flag_mask = 0;

	da9052->regmap = regmap_init_spi(spi, &da9052_regmap_config);
=======
	spi_set_drvdata(spi, da9052);

	config = da9052_regmap_config;
	config.read_flag_mask = 1;
	config.reg_bits = 7;
	config.pad_bits = 1;
	config.val_bits = 8;
	config.use_single_read = true;
	config.use_single_write = true;

	da9052->regmap = devm_regmap_init_spi(spi, &config);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	if (IS_ERR(da9052->regmap)) {
		ret = PTR_ERR(da9052->regmap);
		dev_err(&spi->dev, "Failed to allocate register map: %d\n",
			ret);
<<<<<<< HEAD
		goto err;
	}

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

static int __devexit da9052_spi_remove(struct spi_device *spi)
{
	struct da9052 *da9052 = dev_get_drvdata(&spi->dev);

	da9052_device_exit(da9052);
	regmap_exit(da9052->regmap);
	kfree(da9052);

	return 0;
}

static struct spi_device_id da9052_spi_id[] = {
=======
		return ret;
	}

	return da9052_device_init(da9052, id->driver_data);
}

static void da9052_spi_remove(struct spi_device *spi)
{
	struct da9052 *da9052 = spi_get_drvdata(spi);

	da9052_device_exit(da9052);
}

static const struct spi_device_id da9052_spi_id[] = {
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	{"da9052", DA9052},
	{"da9053-aa", DA9053_AA},
	{"da9053-ba", DA9053_BA},
	{"da9053-bb", DA9053_BB},
<<<<<<< HEAD
=======
	{"da9053-bc", DA9053_BC},
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	{}
};

static struct spi_driver da9052_spi_driver = {
	.probe = da9052_spi_probe,
<<<<<<< HEAD
	.remove = __devexit_p(da9052_spi_remove),
	.id_table = da9052_spi_id,
	.driver = {
		.name = "da9052",
		.bus = &spi_bus_type,
		.owner = THIS_MODULE,
=======
	.remove = da9052_spi_remove,
	.id_table = da9052_spi_id,
	.driver = {
		.name = "da9052",
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	},
};

static int __init da9052_spi_init(void)
{
	int ret;

	ret = spi_register_driver(&da9052_spi_driver);
	if (ret != 0) {
		pr_err("Failed to register DA9052 SPI driver, %d\n", ret);
		return ret;
	}

	return 0;
}
subsys_initcall(da9052_spi_init);

static void __exit da9052_spi_exit(void)
{
	spi_unregister_driver(&da9052_spi_driver);
}
module_exit(da9052_spi_exit);

MODULE_AUTHOR("David Dajun Chen <dchen@diasemi.com>");
MODULE_DESCRIPTION("SPI driver for Dialog DA9052 PMIC");
<<<<<<< HEAD
MODULE_LICENSE("GPL");
=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
