<<<<<<< HEAD
/*
 * LEDs driver for LT3593 controllers
 *
 * See the datasheet at http://cds.linear.com/docs/Datasheet/3593f.pdf
 *
 * Copyright (c) 2009 Daniel Mack <daniel@caiaq.de>
 *
 * Based on leds-gpio.c,
 *
 *   Copyright (C) 2007 8D Technologies inc.
 *   Raphael Assenat <raph@8d.com>
 *   Copyright (C) 2008 Freescale Semiconductor, Inc.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 */

#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/platform_device.h>
#include <linux/leds.h>
#include <linux/workqueue.h>
#include <linux/delay.h>
#include <linux/gpio.h>
#include <linux/slab.h>
#include <linux/module.h>

struct lt3593_led_data {
	struct led_classdev cdev;
	unsigned gpio;
	struct work_struct work;
	u8 new_level;
};

static void lt3593_led_work(struct work_struct *work)
{
	int pulses;
	struct lt3593_led_data *led_dat =
		container_of(work, struct lt3593_led_data, work);
=======
// SPDX-License-Identifier: GPL-2.0
// Copyright (c) 2009,2018 Daniel Mack <daniel@zonque.org>

#include <linux/kernel.h>
#include <linux/platform_device.h>
#include <linux/leds.h>
#include <linux/delay.h>
#include <linux/gpio/consumer.h>
#include <linux/slab.h>
#include <linux/mod_devicetable.h>
#include <linux/module.h>
#include <linux/property.h>

#define LED_LT3593_NAME "lt3593"

struct lt3593_led_data {
	struct led_classdev cdev;
	struct gpio_desc *gpiod;
};

static int lt3593_led_set(struct led_classdev *led_cdev,
			   enum led_brightness value)
{
	struct lt3593_led_data *led_dat =
		container_of(led_cdev, struct lt3593_led_data, cdev);
	int pulses;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	/*
	 * The LT3593 resets its internal current level register to the maximum
	 * level on the first falling edge on the control pin. Each following
	 * falling edge decreases the current level by 625uA. Up to 32 pulses
	 * can be sent, so the maximum power reduction is 20mA.
	 * After a timeout of 128us, the value is taken from the register and
	 * applied is to the output driver.
	 */

<<<<<<< HEAD
	if (led_dat->new_level == 0) {
		gpio_set_value_cansleep(led_dat->gpio, 0);
		return;
	}

	pulses = 32 - (led_dat->new_level * 32) / 255;

	if (pulses == 0) {
		gpio_set_value_cansleep(led_dat->gpio, 0);
		mdelay(1);
		gpio_set_value_cansleep(led_dat->gpio, 1);
		return;
	}

	gpio_set_value_cansleep(led_dat->gpio, 1);

	while (pulses--) {
		gpio_set_value_cansleep(led_dat->gpio, 0);
		udelay(1);
		gpio_set_value_cansleep(led_dat->gpio, 1);
		udelay(1);
	}
}

static void lt3593_led_set(struct led_classdev *led_cdev,
	enum led_brightness value)
{
	struct lt3593_led_data *led_dat =
		container_of(led_cdev, struct lt3593_led_data, cdev);

	led_dat->new_level = value;
	schedule_work(&led_dat->work);
}

static int __devinit create_lt3593_led(const struct gpio_led *template,
	struct lt3593_led_data *led_dat, struct device *parent)
{
	int ret, state;

	/* skip leds on GPIOs that aren't available */
	if (!gpio_is_valid(template->gpio)) {
		printk(KERN_INFO "%s: skipping unavailable LT3593 LED at gpio %d (%s)\n",
				KBUILD_MODNAME, template->gpio, template->name);
		return 0;
	}

	ret = gpio_request(template->gpio, template->name);
	if (ret < 0)
		return ret;

	led_dat->cdev.name = template->name;
	led_dat->cdev.default_trigger = template->default_trigger;
	led_dat->gpio = template->gpio;

	led_dat->cdev.brightness_set = lt3593_led_set;

	state = (template->default_state == LEDS_GPIO_DEFSTATE_ON);
	led_dat->cdev.brightness = state ? LED_FULL : LED_OFF;

	if (!template->retain_state_suspended)
		led_dat->cdev.flags |= LED_CORE_SUSPENDRESUME;

	ret = gpio_direction_output(led_dat->gpio, state);
	if (ret < 0)
		goto err;

	INIT_WORK(&led_dat->work, lt3593_led_work);

	ret = led_classdev_register(parent, &led_dat->cdev);
	if (ret < 0)
		goto err;

	printk(KERN_INFO "%s: registered LT3593 LED '%s' at GPIO %d\n",
		KBUILD_MODNAME, template->name, template->gpio);

	return 0;

err:
	gpio_free(led_dat->gpio);
	return ret;
}

static void delete_lt3593_led(struct lt3593_led_data *led)
{
	if (!gpio_is_valid(led->gpio))
		return;

	led_classdev_unregister(&led->cdev);
	cancel_work_sync(&led->work);
	gpio_free(led->gpio);
}

static int __devinit lt3593_led_probe(struct platform_device *pdev)
{
	struct gpio_led_platform_data *pdata = pdev->dev.platform_data;
	struct lt3593_led_data *leds_data;
	int i, ret = 0;

	if (!pdata)
		return -EBUSY;

	leds_data = kzalloc(sizeof(struct lt3593_led_data) * pdata->num_leds,
				GFP_KERNEL);
	if (!leds_data)
		return -ENOMEM;

	for (i = 0; i < pdata->num_leds; i++) {
		ret = create_lt3593_led(&pdata->leds[i], &leds_data[i],
				      &pdev->dev);
		if (ret < 0)
			goto err;
	}

	platform_set_drvdata(pdev, leds_data);

	return 0;

err:
	for (i = i - 1; i >= 0; i--)
		delete_lt3593_led(&leds_data[i]);

	kfree(leds_data);

	return ret;
}

static int __devexit lt3593_led_remove(struct platform_device *pdev)
{
	int i;
	struct gpio_led_platform_data *pdata = pdev->dev.platform_data;
	struct lt3593_led_data *leds_data;

	leds_data = platform_get_drvdata(pdev);

	for (i = 0; i < pdata->num_leds; i++)
		delete_lt3593_led(&leds_data[i]);

	kfree(leds_data);
=======
	if (value == 0) {
		gpiod_set_value_cansleep(led_dat->gpiod, 0);
		return 0;
	}

	pulses = 32 - (value * 32) / 255;

	if (pulses == 0) {
		gpiod_set_value_cansleep(led_dat->gpiod, 0);
		mdelay(1);
		gpiod_set_value_cansleep(led_dat->gpiod, 1);
		return 0;
	}

	gpiod_set_value_cansleep(led_dat->gpiod, 1);

	while (pulses--) {
		gpiod_set_value_cansleep(led_dat->gpiod, 0);
		udelay(1);
		gpiod_set_value_cansleep(led_dat->gpiod, 1);
		udelay(1);
	}

	return 0;
}

static int lt3593_led_probe(struct platform_device *pdev)
{
	struct device *dev = &pdev->dev;
	struct lt3593_led_data *led_data;
	struct fwnode_handle *child;
	int ret, state = LEDS_GPIO_DEFSTATE_OFF;
	struct led_init_data init_data = {};
	const char *tmp;

	led_data = devm_kzalloc(dev, sizeof(*led_data), GFP_KERNEL);
	if (!led_data)
		return -ENOMEM;

	if (device_get_child_node_count(dev) != 1) {
		dev_err(dev, "Device must have exactly one LED sub-node.");
		return -EINVAL;
	}

	led_data->gpiod = devm_gpiod_get(dev, "lltc,ctrl", 0);
	if (IS_ERR(led_data->gpiod))
		return PTR_ERR(led_data->gpiod);

	child = device_get_next_child_node(dev, NULL);

	if (!fwnode_property_read_string(child, "default-state", &tmp)) {
		if (!strcmp(tmp, "on"))
			state = LEDS_GPIO_DEFSTATE_ON;
	}

	led_data->cdev.brightness_set_blocking = lt3593_led_set;
	led_data->cdev.brightness = state ? LED_FULL : LED_OFF;

	init_data.fwnode = child;
	init_data.devicename = LED_LT3593_NAME;
	init_data.default_label = ":";

	ret = devm_led_classdev_register_ext(dev, &led_data->cdev, &init_data);
	fwnode_handle_put(child);
	if (ret < 0)
		return ret;

	platform_set_drvdata(pdev, led_data);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	return 0;
}

<<<<<<< HEAD
static struct platform_driver lt3593_led_driver = {
	.probe		= lt3593_led_probe,
	.remove		= __devexit_p(lt3593_led_remove),
	.driver		= {
		.name	= "leds-lt3593",
		.owner	= THIS_MODULE,
=======
static const struct of_device_id of_lt3593_leds_match[] = {
	{ .compatible = "lltc,lt3593", },
	{},
};
MODULE_DEVICE_TABLE(of, of_lt3593_leds_match);

static struct platform_driver lt3593_led_driver = {
	.probe		= lt3593_led_probe,
	.driver		= {
		.name	= "leds-lt3593",
		.of_match_table = of_lt3593_leds_match,
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	},
};

module_platform_driver(lt3593_led_driver);

<<<<<<< HEAD
MODULE_AUTHOR("Daniel Mack <daniel@caiaq.de>");
MODULE_DESCRIPTION("LED driver for LT3593 controllers");
MODULE_LICENSE("GPL");
=======
MODULE_AUTHOR("Daniel Mack <daniel@zonque.org>");
MODULE_DESCRIPTION("LED driver for LT3593 controllers");
MODULE_LICENSE("GPL v2");
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
MODULE_ALIAS("platform:leds-lt3593");
