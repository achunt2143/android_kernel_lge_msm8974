<<<<<<< HEAD
=======
// SPDX-License-Identifier: GPL-2.0-or-later
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
/*
 * wm831x-i2c.c  --  I2C access for Wolfson WM831x PMICs
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
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#include <linux/i2c.h>
#include <linux/delay.h>
#include <linux/mfd/core.h>
#include <linux/slab.h>
#include <linux/err.h>
<<<<<<< HEAD
=======
#include <linux/of.h>
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#include <linux/regmap.h>

#include <linux/mfd/wm831x/core.h>
#include <linux/mfd/wm831x/pdata.h>

<<<<<<< HEAD
static int wm831x_i2c_probe(struct i2c_client *i2c,
			    const struct i2c_device_id *id)
{
	struct wm831x *wm831x;
	int ret;

=======
static int wm831x_i2c_probe(struct i2c_client *i2c)
{
	struct wm831x_pdata *pdata = dev_get_platdata(&i2c->dev);
	struct wm831x *wm831x;
	enum wm831x_parent type;
	int ret;

	type = (uintptr_t)i2c_get_match_data(i2c);
	if (!type) {
		dev_err(&i2c->dev, "Failed to match device\n");
		return -ENODEV;
	}

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	wm831x = devm_kzalloc(&i2c->dev, sizeof(struct wm831x), GFP_KERNEL);
	if (wm831x == NULL)
		return -ENOMEM;

	i2c_set_clientdata(i2c, wm831x);
	wm831x->dev = &i2c->dev;
<<<<<<< HEAD
=======
	wm831x->type = type;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	wm831x->regmap = devm_regmap_init_i2c(i2c, &wm831x_regmap_config);
	if (IS_ERR(wm831x->regmap)) {
		ret = PTR_ERR(wm831x->regmap);
		dev_err(wm831x->dev, "Failed to allocate register map: %d\n",
			ret);
		return ret;
	}

<<<<<<< HEAD
	return wm831x_device_init(wm831x, id->driver_data, i2c->irq);
}

static int wm831x_i2c_remove(struct i2c_client *i2c)
{
	struct wm831x *wm831x = i2c_get_clientdata(i2c);

	wm831x_device_exit(wm831x);

	return 0;
=======
	if (pdata)
		memcpy(&wm831x->pdata, pdata, sizeof(*pdata));

	return wm831x_device_init(wm831x, i2c->irq);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

static int wm831x_i2c_suspend(struct device *dev)
{
	struct wm831x *wm831x = dev_get_drvdata(dev);

	return wm831x_device_suspend(wm831x);
}

<<<<<<< HEAD
static void wm831x_i2c_shutdown(struct i2c_client *i2c)
{
	struct wm831x *wm831x = i2c_get_clientdata(i2c);

	wm831x_device_shutdown(wm831x);
=======
static int wm831x_i2c_poweroff(struct device *dev)
{
	struct wm831x *wm831x = dev_get_drvdata(dev);

	wm831x_device_shutdown(wm831x);

	return 0;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

static const struct i2c_device_id wm831x_i2c_id[] = {
	{ "wm8310", WM8310 },
	{ "wm8311", WM8311 },
	{ "wm8312", WM8312 },
	{ "wm8320", WM8320 },
	{ "wm8321", WM8321 },
	{ "wm8325", WM8325 },
	{ "wm8326", WM8326 },
	{ }
};
<<<<<<< HEAD
MODULE_DEVICE_TABLE(i2c, wm831x_i2c_id);

static const struct dev_pm_ops wm831x_pm_ops = {
	.suspend = wm831x_i2c_suspend,
=======

static const struct dev_pm_ops wm831x_pm_ops = {
	.suspend = wm831x_i2c_suspend,
	.poweroff = wm831x_i2c_poweroff,
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
};

static struct i2c_driver wm831x_i2c_driver = {
	.driver = {
		.name = "wm831x",
<<<<<<< HEAD
		.owner = THIS_MODULE,
		.pm = &wm831x_pm_ops,
	},
	.probe = wm831x_i2c_probe,
	.remove = wm831x_i2c_remove,
	.shutdown = wm831x_i2c_shutdown,
=======
		.pm = &wm831x_pm_ops,
		.of_match_table = of_match_ptr(wm831x_of_match),
		.suppress_bind_attrs = true,
	},
	.probe = wm831x_i2c_probe,
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	.id_table = wm831x_i2c_id,
};

static int __init wm831x_i2c_init(void)
{
	int ret;

	ret = i2c_add_driver(&wm831x_i2c_driver);
	if (ret != 0)
		pr_err("Failed to register wm831x I2C driver: %d\n", ret);

	return ret;
}
subsys_initcall(wm831x_i2c_init);
<<<<<<< HEAD

static void __exit wm831x_i2c_exit(void)
{
	i2c_del_driver(&wm831x_i2c_driver);
}
module_exit(wm831x_i2c_exit);
=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
