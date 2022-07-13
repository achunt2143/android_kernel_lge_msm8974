<<<<<<< HEAD
=======
// SPDX-License-Identifier: GPL-2.0+
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
/*
 * TI TPS6591x GPIO driver
 *
 * Copyright 2010 Texas Instruments Inc.
 *
 * Author: Graeme Gregory <gg@slimlogic.co.uk>
<<<<<<< HEAD
 * Author: Jorge Eduardo Candelaria jedu@slimlogic.co.uk>
 *
 *  This program is free software; you can redistribute it and/or modify it
 *  under  the terms of the GNU General  Public License as published by the
 *  Free Software Foundation;  either version 2 of the License, or (at your
 *  option) any later version.
 *
 */

#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/errno.h>
#include <linux/gpio.h>
#include <linux/i2c.h>
#include <linux/mfd/tps65910.h>

static int tps65910_gpio_get(struct gpio_chip *gc, unsigned offset)
{
	struct tps65910 *tps65910 = container_of(gc, struct tps65910, gpio);
	uint8_t val;

	tps65910->read(tps65910, TPS65910_GPIO0 + offset, 1, &val);
=======
 * Author: Jorge Eduardo Candelaria <jedu@slimlogic.co.uk>
 */

#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/errno.h>
#include <linux/gpio/driver.h>
#include <linux/i2c.h>
#include <linux/platform_device.h>
#include <linux/mfd/tps65910.h>
#include <linux/of.h>

struct tps65910_gpio {
	struct gpio_chip gpio_chip;
	struct tps65910 *tps65910;
};

static int tps65910_gpio_get(struct gpio_chip *gc, unsigned offset)
{
	struct tps65910_gpio *tps65910_gpio = gpiochip_get_data(gc);
	struct tps65910 *tps65910 = tps65910_gpio->tps65910;
	unsigned int val;

	regmap_read(tps65910->regmap, TPS65910_GPIO0 + offset, &val);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	if (val & GPIO_STS_MASK)
		return 1;

	return 0;
}

static void tps65910_gpio_set(struct gpio_chip *gc, unsigned offset,
			      int value)
{
<<<<<<< HEAD
	struct tps65910 *tps65910 = container_of(gc, struct tps65910, gpio);

	if (value)
		tps65910_set_bits(tps65910, TPS65910_GPIO0 + offset,
						GPIO_SET_MASK);
	else
		tps65910_clear_bits(tps65910, TPS65910_GPIO0 + offset,
=======
	struct tps65910_gpio *tps65910_gpio = gpiochip_get_data(gc);
	struct tps65910 *tps65910 = tps65910_gpio->tps65910;

	if (value)
		regmap_set_bits(tps65910->regmap, TPS65910_GPIO0 + offset,
						GPIO_SET_MASK);
	else
		regmap_clear_bits(tps65910->regmap, TPS65910_GPIO0 + offset,
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
						GPIO_SET_MASK);
}

static int tps65910_gpio_output(struct gpio_chip *gc, unsigned offset,
				int value)
{
<<<<<<< HEAD
	struct tps65910 *tps65910 = container_of(gc, struct tps65910, gpio);
=======
	struct tps65910_gpio *tps65910_gpio = gpiochip_get_data(gc);
	struct tps65910 *tps65910 = tps65910_gpio->tps65910;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	/* Set the initial value */
	tps65910_gpio_set(gc, offset, value);

<<<<<<< HEAD
	return tps65910_set_bits(tps65910, TPS65910_GPIO0 + offset,
=======
	return regmap_set_bits(tps65910->regmap, TPS65910_GPIO0 + offset,
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
						GPIO_CFG_MASK);
}

static int tps65910_gpio_input(struct gpio_chip *gc, unsigned offset)
{
<<<<<<< HEAD
	struct tps65910 *tps65910 = container_of(gc, struct tps65910, gpio);

	return tps65910_clear_bits(tps65910, TPS65910_GPIO0 + offset,
						GPIO_CFG_MASK);
}

void tps65910_gpio_init(struct tps65910 *tps65910, int gpio_base)
{
	int ret;
	struct tps65910_board *board_data;

	if (!gpio_base)
		return;

	tps65910->gpio.owner		= THIS_MODULE;
	tps65910->gpio.label		= tps65910->i2c_client->name;
	tps65910->gpio.dev		= tps65910->dev;
	tps65910->gpio.base		= gpio_base;

	switch(tps65910_chip_id(tps65910)) {
	case TPS65910:
		tps65910->gpio.ngpio	= TPS65910_NUM_GPIO;
		break;
	case TPS65911:
		tps65910->gpio.ngpio	= TPS65911_NUM_GPIO;
		break;
	default:
		return;
	}
	tps65910->gpio.can_sleep	= 1;

	tps65910->gpio.direction_input	= tps65910_gpio_input;
	tps65910->gpio.direction_output	= tps65910_gpio_output;
	tps65910->gpio.set		= tps65910_gpio_set;
	tps65910->gpio.get		= tps65910_gpio_get;

	/* Configure sleep control for gpios */
	board_data = dev_get_platdata(tps65910->dev);
	if (board_data) {
		int i;
		for (i = 0; i < tps65910->gpio.ngpio; ++i) {
			if (board_data->en_gpio_sleep[i]) {
				ret = tps65910_set_bits(tps65910,
					TPS65910_GPIO0 + i, GPIO_SLEEP_MASK);
				if (ret < 0)
					dev_warn(tps65910->dev,
						"GPIO Sleep setting failed\n");
			}
		}
	}

	ret = gpiochip_add(&tps65910->gpio);

	if (ret)
		dev_warn(tps65910->dev, "GPIO registration failed: %d\n", ret);
}
=======
	struct tps65910_gpio *tps65910_gpio = gpiochip_get_data(gc);
	struct tps65910 *tps65910 = tps65910_gpio->tps65910;

	return regmap_clear_bits(tps65910->regmap, TPS65910_GPIO0 + offset,
						GPIO_CFG_MASK);
}

#ifdef CONFIG_OF
static struct tps65910_board *tps65910_parse_dt_for_gpio(struct device *dev,
		struct tps65910 *tps65910, int chip_ngpio)
{
	struct tps65910_board *tps65910_board = tps65910->of_plat_data;
	unsigned int prop_array[TPS6591X_MAX_NUM_GPIO];
	int ngpio = min(chip_ngpio, TPS6591X_MAX_NUM_GPIO);
	int ret;
	int idx;

	tps65910_board->gpio_base = -1;
	ret = of_property_read_u32_array(tps65910->dev->of_node,
			"ti,en-gpio-sleep", prop_array, ngpio);
	if (ret < 0) {
		dev_dbg(dev, "ti,en-gpio-sleep not specified\n");
		return tps65910_board;
	}

	for (idx = 0; idx < ngpio; idx++)
		tps65910_board->en_gpio_sleep[idx] = (prop_array[idx] != 0);

	return tps65910_board;
}
#else
static struct tps65910_board *tps65910_parse_dt_for_gpio(struct device *dev,
		struct tps65910 *tps65910, int chip_ngpio)
{
	return NULL;
}
#endif

static int tps65910_gpio_probe(struct platform_device *pdev)
{
	struct tps65910 *tps65910 = dev_get_drvdata(pdev->dev.parent);
	struct tps65910_board *pdata = dev_get_platdata(tps65910->dev);
	struct tps65910_gpio *tps65910_gpio;
	int ret;
	int i;

	device_set_node(&pdev->dev, dev_fwnode(pdev->dev.parent));

	tps65910_gpio = devm_kzalloc(&pdev->dev,
				sizeof(*tps65910_gpio), GFP_KERNEL);
	if (!tps65910_gpio)
		return -ENOMEM;

	tps65910_gpio->tps65910 = tps65910;

	tps65910_gpio->gpio_chip.owner = THIS_MODULE;
	tps65910_gpio->gpio_chip.label = tps65910->i2c_client->name;

	switch (tps65910_chip_id(tps65910)) {
	case TPS65910:
		tps65910_gpio->gpio_chip.ngpio = TPS65910_NUM_GPIO;
		break;
	case TPS65911:
		tps65910_gpio->gpio_chip.ngpio = TPS65911_NUM_GPIO;
		break;
	default:
		return -EINVAL;
	}
	tps65910_gpio->gpio_chip.can_sleep = true;
	tps65910_gpio->gpio_chip.direction_input = tps65910_gpio_input;
	tps65910_gpio->gpio_chip.direction_output = tps65910_gpio_output;
	tps65910_gpio->gpio_chip.set	= tps65910_gpio_set;
	tps65910_gpio->gpio_chip.get	= tps65910_gpio_get;
	tps65910_gpio->gpio_chip.parent = &pdev->dev;

	if (pdata && pdata->gpio_base)
		tps65910_gpio->gpio_chip.base = pdata->gpio_base;
	else
		tps65910_gpio->gpio_chip.base = -1;

	if (!pdata && tps65910->dev->of_node)
		pdata = tps65910_parse_dt_for_gpio(&pdev->dev, tps65910,
			tps65910_gpio->gpio_chip.ngpio);

	if (!pdata)
		goto skip_init;

	/* Configure sleep control for gpios if provided */
	for (i = 0; i < tps65910_gpio->gpio_chip.ngpio; ++i) {
		if (!pdata->en_gpio_sleep[i])
			continue;

		ret = regmap_set_bits(tps65910->regmap,
			TPS65910_GPIO0 + i, GPIO_SLEEP_MASK);
		if (ret < 0)
			dev_warn(tps65910->dev,
				"GPIO Sleep setting failed with err %d\n", ret);
	}

skip_init:
	return devm_gpiochip_add_data(&pdev->dev, &tps65910_gpio->gpio_chip,
				      tps65910_gpio);
}

static struct platform_driver tps65910_gpio_driver = {
	.driver.name    = "tps65910-gpio",
	.probe		= tps65910_gpio_probe,
};

static int __init tps65910_gpio_init(void)
{
	return platform_driver_register(&tps65910_gpio_driver);
}
subsys_initcall(tps65910_gpio_init);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
