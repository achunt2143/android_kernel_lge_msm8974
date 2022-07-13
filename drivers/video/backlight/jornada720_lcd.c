<<<<<<< HEAD
=======
// SPDX-License-Identifier: GPL-2.0-or-later
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
/*
 *
 * LCD driver for HP Jornada 700 series (710/720/728)
 * Copyright (C) 2006-2009 Kristoffer Ericson <kristoffer.ericson@gmail.com>
<<<<<<< HEAD
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License version
 * 2 or any later version as published by the Free Software Foundation.
 *
=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 */

#include <linux/device.h>
#include <linux/fb.h>
#include <linux/kernel.h>
#include <linux/lcd.h>
#include <linux/module.h>
#include <linux/platform_device.h>
#include <linux/delay.h>

#include <mach/jornada720.h>
#include <mach/hardware.h>

#include <video/s1d13xxxfb.h>

#define LCD_MAX_CONTRAST	0xff
#define LCD_DEF_CONTRAST	0x80

<<<<<<< HEAD
static int jornada_lcd_get_power(struct lcd_device *dev)
{
	/* LDD2 in PPC = LCD POWER */
	if (PPSR & PPC_LDD2)
		return FB_BLANK_UNBLANK;	/* PW ON */
	else
		return FB_BLANK_POWERDOWN;	/* PW OFF */
}

static int jornada_lcd_get_contrast(struct lcd_device *dev)
{
	int ret;

	if (jornada_lcd_get_power(dev) != FB_BLANK_UNBLANK)
=======
static int jornada_lcd_get_power(struct lcd_device *ld)
{
	return PPSR & PPC_LDD2 ? FB_BLANK_UNBLANK : FB_BLANK_POWERDOWN;
}

static int jornada_lcd_get_contrast(struct lcd_device *ld)
{
	int ret;

	if (jornada_lcd_get_power(ld) != FB_BLANK_UNBLANK)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		return 0;

	jornada_ssp_start();

<<<<<<< HEAD
	if (jornada_ssp_byte(GETCONTRAST) != TXDUMMY) {
		printk(KERN_ERR "lcd: get contrast failed\n");
		jornada_ssp_end();
		return -ETIMEDOUT;
	} else {
		ret = jornada_ssp_byte(TXDUMMY);
		jornada_ssp_end();
		return ret;
	}
}

static int jornada_lcd_set_contrast(struct lcd_device *dev, int value)
{
	int ret;
=======
	if (jornada_ssp_byte(GETCONTRAST) == TXDUMMY) {
		ret = jornada_ssp_byte(TXDUMMY);
		goto success;
	}

	dev_err(&ld->dev, "failed to set contrast\n");
	ret = -ETIMEDOUT;

success:
	jornada_ssp_end();
	return ret;
}

static int jornada_lcd_set_contrast(struct lcd_device *ld, int value)
{
	int ret = 0;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	jornada_ssp_start();

	/* start by sending our set contrast cmd to mcu */
<<<<<<< HEAD
	ret = jornada_ssp_byte(SETCONTRAST);

	/* push the new value */
	if (jornada_ssp_byte(value) != TXDUMMY) {
		printk(KERN_ERR "lcd : set contrast failed\n");
		jornada_ssp_end();
		return -ETIMEDOUT;
	}

	/* if we get here we can assume everything went well */
	jornada_ssp_end();

	return 0;
}

static int jornada_lcd_set_power(struct lcd_device *dev, int power)
=======
	if (jornada_ssp_byte(SETCONTRAST) == TXDUMMY) {
		/* if successful push the new value */
		if (jornada_ssp_byte(value) == TXDUMMY)
			goto success;
	}

	dev_err(&ld->dev, "failed to set contrast\n");
	ret = -ETIMEDOUT;

success:
	jornada_ssp_end();
	return ret;
}

static int jornada_lcd_set_power(struct lcd_device *ld, int power)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
{
	if (power != FB_BLANK_UNBLANK) {
		PPSR &= ~PPC_LDD2;
		PPDR |= PPC_LDD2;
<<<<<<< HEAD
	} else
		PPSR |= PPC_LDD2;
=======
	} else {
		PPSR |= PPC_LDD2;
	}
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	return 0;
}

static struct lcd_ops jornada_lcd_props = {
	.get_contrast = jornada_lcd_get_contrast,
	.set_contrast = jornada_lcd_set_contrast,
	.get_power = jornada_lcd_get_power,
	.set_power = jornada_lcd_set_power,
};

static int jornada_lcd_probe(struct platform_device *pdev)
{
	struct lcd_device *lcd_device;
	int ret;

<<<<<<< HEAD
	lcd_device = lcd_device_register(S1D_DEVICENAME, &pdev->dev, NULL, &jornada_lcd_props);

	if (IS_ERR(lcd_device)) {
		ret = PTR_ERR(lcd_device);
		printk(KERN_ERR "lcd : failed to register device\n");
=======
	lcd_device = devm_lcd_device_register(&pdev->dev, S1D_DEVICENAME,
					&pdev->dev, NULL, &jornada_lcd_props);

	if (IS_ERR(lcd_device)) {
		ret = PTR_ERR(lcd_device);
		dev_err(&pdev->dev, "failed to register device\n");
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		return ret;
	}

	platform_set_drvdata(pdev, lcd_device);

	/* lets set our default values */
	jornada_lcd_set_contrast(lcd_device, LCD_DEF_CONTRAST);
	jornada_lcd_set_power(lcd_device, FB_BLANK_UNBLANK);
	/* give it some time to startup */
	msleep(100);

	return 0;
}

<<<<<<< HEAD
static int jornada_lcd_remove(struct platform_device *pdev)
{
	struct lcd_device *lcd_device = platform_get_drvdata(pdev);

	lcd_device_unregister(lcd_device);

	return 0;
}

static struct platform_driver jornada_lcd_driver = {
	.probe	= jornada_lcd_probe,
	.remove	= jornada_lcd_remove,
=======
static struct platform_driver jornada_lcd_driver = {
	.probe	= jornada_lcd_probe,
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	.driver	= {
		.name	= "jornada_lcd",
	},
};

module_platform_driver(jornada_lcd_driver);

MODULE_AUTHOR("Kristoffer Ericson <kristoffer.ericson@gmail.com>");
MODULE_DESCRIPTION("HP Jornada 710/720/728 LCD driver");
MODULE_LICENSE("GPL");
