<<<<<<< HEAD
=======
// SPDX-License-Identifier: GPL-2.0-or-later
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
/*
 * dummy.c
 *
 * Copyright 2010 Wolfson Microelectronics PLC.
 *
 * Author: Mark Brown <broonie@opensource.wolfsonmicro.com>
 *
<<<<<<< HEAD
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 2 of the
 * License, or (at your option) any later version.
 *
=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 * This is useful for systems with mixed controllable and
 * non-controllable regulators, as well as for allowing testing on
 * systems with no controllable regulators.
 */

#include <linux/err.h>
#include <linux/export.h>
#include <linux/platform_device.h>
#include <linux/regulator/driver.h>
#include <linux/regulator/machine.h>

#include "dummy.h"

struct regulator_dev *dummy_regulator_rdev;

<<<<<<< HEAD
static struct regulator_init_data dummy_initdata;

static struct regulator_ops dummy_ops;

static struct regulator_desc dummy_desc = {
	.name = "dummy",
=======
static const struct regulator_init_data dummy_initdata = {
	.constraints = {
		.always_on = 1,
	},
};

static const struct regulator_ops dummy_ops;

static const struct regulator_desc dummy_desc = {
	.name = "regulator-dummy",
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	.id = -1,
	.type = REGULATOR_VOLTAGE,
	.owner = THIS_MODULE,
	.ops = &dummy_ops,
};

<<<<<<< HEAD
static int __devinit dummy_regulator_probe(struct platform_device *pdev)
{
	int ret;

	dummy_regulator_rdev = regulator_register(&dummy_desc, NULL,
						  &dummy_initdata, NULL, NULL);
=======
static int dummy_regulator_probe(struct platform_device *pdev)
{
	struct regulator_config config = { };
	int ret;

	config.dev = &pdev->dev;
	config.init_data = &dummy_initdata;

	dummy_regulator_rdev = devm_regulator_register(&pdev->dev, &dummy_desc,
						       &config);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	if (IS_ERR(dummy_regulator_rdev)) {
		ret = PTR_ERR(dummy_regulator_rdev);
		pr_err("Failed to register regulator: %d\n", ret);
		return ret;
	}

	return 0;
}

static struct platform_driver dummy_regulator_driver = {
	.probe		= dummy_regulator_probe,
	.driver		= {
		.name		= "reg-dummy",
<<<<<<< HEAD
		.owner		= THIS_MODULE,
=======
		.probe_type	= PROBE_PREFER_ASYNCHRONOUS,
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	},
};

static struct platform_device *dummy_pdev;

void __init regulator_dummy_init(void)
{
	int ret;

	dummy_pdev = platform_device_alloc("reg-dummy", -1);
	if (!dummy_pdev) {
		pr_err("Failed to allocate dummy regulator device\n");
		return;
	}

	ret = platform_device_add(dummy_pdev);
	if (ret != 0) {
		pr_err("Failed to register dummy regulator device: %d\n", ret);
		platform_device_put(dummy_pdev);
		return;
	}

	ret = platform_driver_register(&dummy_regulator_driver);
	if (ret != 0) {
		pr_err("Failed to register dummy regulator driver: %d\n", ret);
		platform_device_unregister(dummy_pdev);
	}
}
