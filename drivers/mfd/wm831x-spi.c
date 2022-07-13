<<<<<<< HEAD
=======
// SPDX-License-Identifier: GPL-2.0-or-later
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
/*
 * wm831x-spi.c  --  SPI access for Wolfson WM831x PMICs
 *
 * Copyright 2009,2010 Wolfson Microelectronics PLC.
 *
 * Author: Mark Brown <broonie@opensource.wolfsonmicro.com>
<<<<<<< HEAD
 *
 *  This program is free software; you can redistribute  it and/or modify it
 *  under  the terms of  the GNU General  Public License as published by the
 *  Free Software Foundation;  either version 2 of the  License, or (at your
 *  option) any later version.
 *
 */

#include <linux/kernel.h>
#include <linux/module.h>
=======
 */

#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/of.h>
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#include <linux/pm.h>
#include <linux/spi/spi.h>
#include <linux/regmap.h>
#include <linux/err.h>

#include <linux/mfd/wm831x/core.h>

<<<<<<< HEAD
static int __devinit wm831x_spi_probe(struct spi_device *spi)
{
	const struct spi_device_id *id = spi_get_device_id(spi);
=======
static int wm831x_spi_probe(struct spi_device *spi)
{
	struct wm831x_pdata *pdata = dev_get_platdata(&spi->dev);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	struct wm831x *wm831x;
	enum wm831x_parent type;
	int ret;

<<<<<<< HEAD
	type = (enum wm831x_parent)id->driver_data;
=======
	type = (uintptr_t)spi_get_device_match_data(spi);
	if (!type) {
		dev_err(&spi->dev, "Failed to match device\n");
		return -ENODEV;
	}
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	wm831x = devm_kzalloc(&spi->dev, sizeof(struct wm831x), GFP_KERNEL);
	if (wm831x == NULL)
		return -ENOMEM;

<<<<<<< HEAD
	spi->bits_per_word = 16;
	spi->mode = SPI_MODE_0;

	dev_set_drvdata(&spi->dev, wm831x);
	wm831x->dev = &spi->dev;
=======
	spi->mode = SPI_MODE_0;

	spi_set_drvdata(spi, wm831x);
	wm831x->dev = &spi->dev;
	wm831x->type = type;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	wm831x->regmap = devm_regmap_init_spi(spi, &wm831x_regmap_config);
	if (IS_ERR(wm831x->regmap)) {
		ret = PTR_ERR(wm831x->regmap);
		dev_err(wm831x->dev, "Failed to allocate register map: %d\n",
			ret);
		return ret;
	}

<<<<<<< HEAD
	return wm831x_device_init(wm831x, type, spi->irq);
}

static int __devexit wm831x_spi_remove(struct spi_device *spi)
{
	struct wm831x *wm831x = dev_get_drvdata(&spi->dev);

	wm831x_device_exit(wm831x);

	return 0;
=======
	if (pdata)
		memcpy(&wm831x->pdata, pdata, sizeof(*pdata));

	return wm831x_device_init(wm831x, spi->irq);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

static int wm831x_spi_suspend(struct device *dev)
{
	struct wm831x *wm831x = dev_get_drvdata(dev);

	return wm831x_device_suspend(wm831x);
}

<<<<<<< HEAD
static void wm831x_spi_shutdown(struct spi_device *spi)
{
	struct wm831x *wm831x = dev_get_drvdata(&spi->dev);

	wm831x_device_shutdown(wm831x);
=======
static int wm831x_spi_poweroff(struct device *dev)
{
	struct wm831x *wm831x = dev_get_drvdata(dev);

	wm831x_device_shutdown(wm831x);

	return 0;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

static const struct dev_pm_ops wm831x_spi_pm = {
	.freeze = wm831x_spi_suspend,
	.suspend = wm831x_spi_suspend,
<<<<<<< HEAD
=======
	.poweroff = wm831x_spi_poweroff,
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
};

static const struct spi_device_id wm831x_spi_ids[] = {
	{ "wm8310", WM8310 },
	{ "wm8311", WM8311 },
	{ "wm8312", WM8312 },
	{ "wm8320", WM8320 },
	{ "wm8321", WM8321 },
	{ "wm8325", WM8325 },
	{ "wm8326", WM8326 },
	{ },
};
<<<<<<< HEAD
MODULE_DEVICE_TABLE(spi, wm831x_spi_ids);
=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

static struct spi_driver wm831x_spi_driver = {
	.driver = {
		.name	= "wm831x",
<<<<<<< HEAD
		.owner	= THIS_MODULE,
		.pm	= &wm831x_spi_pm,
	},
	.id_table	= wm831x_spi_ids,
	.probe		= wm831x_spi_probe,
	.remove		= __devexit_p(wm831x_spi_remove),
	.shutdown	= wm831x_spi_shutdown,
=======
		.pm	= &wm831x_spi_pm,
		.of_match_table = of_match_ptr(wm831x_of_match),
		.suppress_bind_attrs = true,
	},
	.id_table	= wm831x_spi_ids,
	.probe		= wm831x_spi_probe,
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
};

static int __init wm831x_spi_init(void)
{
	int ret;

	ret = spi_register_driver(&wm831x_spi_driver);
	if (ret != 0)
		pr_err("Failed to register WM831x SPI driver: %d\n", ret);

	return 0;
}
subsys_initcall(wm831x_spi_init);
<<<<<<< HEAD

static void __exit wm831x_spi_exit(void)
{
	spi_unregister_driver(&wm831x_spi_driver);
}
module_exit(wm831x_spi_exit);

MODULE_DESCRIPTION("SPI support for WM831x/2x AudioPlus PMICs");
MODULE_LICENSE("GPL");
MODULE_AUTHOR("Mark Brown");
=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
