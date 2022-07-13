<<<<<<< HEAD
=======
// SPDX-License-Identifier: GPL-2.0-only
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
/*
 * System Specific setup for Traverse Technologies GEOS.
 * At the moment this means setup of GPIO control of LEDs.
 *
 * Copyright (C) 2008 Constantin Baranov <const@mimas.ru>
 * Copyright (C) 2011 Ed Wildgoose <kernel@wildgooses.com>
 *                and Philip Prindeville <philipp@redfish-solutions.com>
 *
 * TODO: There are large similarities with leds-net5501.c
 * by Alessandro Zummo <a.zummo@towertech.it>
 * In the future leds-net5501.c should be migrated over to platform
<<<<<<< HEAD
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2
 * as published by the Free Software Foundation.
=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 */

#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/io.h>
#include <linux/string.h>
<<<<<<< HEAD
#include <linux/module.h>
#include <linux/leds.h>
#include <linux/platform_device.h>
#include <linux/gpio.h>
#include <linux/input.h>
#include <linux/gpio_keys.h>
=======
#include <linux/leds.h>
#include <linux/platform_device.h>
#include <linux/input.h>
#include <linux/gpio_keys.h>
#include <linux/gpio/machine.h>
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#include <linux/dmi.h>

#include <asm/geode.h>

static struct gpio_keys_button geos_gpio_buttons[] = {
	{
		.code = KEY_RESTART,
		.gpio = 3,
		.active_low = 1,
		.desc = "Reset button",
		.type = EV_KEY,
		.wakeup = 0,
		.debounce_interval = 100,
		.can_disable = 0,
	}
};
static struct gpio_keys_platform_data geos_buttons_data = {
	.buttons = geos_gpio_buttons,
	.nbuttons = ARRAY_SIZE(geos_gpio_buttons),
	.poll_interval = 20,
};

static struct platform_device geos_buttons_dev = {
	.name = "gpio-keys-polled",
	.id = 1,
	.dev = {
		.platform_data = &geos_buttons_data,
	}
};

static struct gpio_led geos_leds[] = {
	{
		.name = "geos:1",
<<<<<<< HEAD
		.gpio = 6,
		.default_trigger = "default-on",
		.active_low = 1,
	},
	{
		.name = "geos:2",
		.gpio = 25,
		.default_trigger = "default-off",
		.active_low = 1,
	},
	{
		.name = "geos:3",
		.gpio = 27,
		.default_trigger = "default-off",
		.active_low = 1,
=======
		.default_trigger = "default-on",
	},
	{
		.name = "geos:2",
		.default_trigger = "default-off",
	},
	{
		.name = "geos:3",
		.default_trigger = "default-off",
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	},
};

static struct gpio_led_platform_data geos_leds_data = {
	.num_leds = ARRAY_SIZE(geos_leds),
	.leds = geos_leds,
};

<<<<<<< HEAD
=======
static struct gpiod_lookup_table geos_leds_gpio_table = {
	.dev_id = "leds-gpio",
	.table = {
		/* The Geode GPIOs should be on the CS5535 companion chip */
		GPIO_LOOKUP_IDX("cs5535-gpio", 6, NULL, 0, GPIO_ACTIVE_LOW),
		GPIO_LOOKUP_IDX("cs5535-gpio", 25, NULL, 1, GPIO_ACTIVE_LOW),
		GPIO_LOOKUP_IDX("cs5535-gpio", 27, NULL, 2, GPIO_ACTIVE_LOW),
		{ }
	},
};

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
static struct platform_device geos_leds_dev = {
	.name = "leds-gpio",
	.id = -1,
	.dev.platform_data = &geos_leds_data,
};

<<<<<<< HEAD
static struct __initdata platform_device *geos_devs[] = {
=======
static struct platform_device *geos_devs[] __initdata = {
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	&geos_buttons_dev,
	&geos_leds_dev,
};

static void __init register_geos(void)
{
	/* Setup LED control through leds-gpio driver */
<<<<<<< HEAD
=======
	gpiod_add_lookup_table(&geos_leds_gpio_table);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	platform_add_devices(geos_devs, ARRAY_SIZE(geos_devs));
}

static int __init geos_init(void)
{
	const char *vendor, *product;

	if (!is_geode())
		return 0;

	vendor = dmi_get_system_info(DMI_SYS_VENDOR);
	if (!vendor || strcmp(vendor, "Traverse Technologies"))
		return 0;

	product = dmi_get_system_info(DMI_PRODUCT_NAME);
	if (!product || strcmp(product, "Geos"))
		return 0;

	printk(KERN_INFO "%s: system is recognized as \"%s %s\"\n",
	       KBUILD_MODNAME, vendor, product);

	register_geos();

	return 0;
}
<<<<<<< HEAD

module_init(geos_init);

MODULE_AUTHOR("Philip Prindeville <philipp@redfish-solutions.com>");
MODULE_DESCRIPTION("Traverse Technologies Geos System Setup");
MODULE_LICENSE("GPL");
=======
device_initcall(geos_init);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
