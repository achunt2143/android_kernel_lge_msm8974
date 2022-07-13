<<<<<<< HEAD
=======
// SPDX-License-Identifier: GPL-2.0-only
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
/*
 * Backlight driver for Maxim MAX8925
 *
 * Copyright (C) 2009 Marvell International Ltd.
 *      Haojian Zhuang <haojian.zhuang@marvell.com>
<<<<<<< HEAD
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 */

#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/platform_device.h>
#include <linux/fb.h>
#include <linux/i2c.h>
#include <linux/backlight.h>
#include <linux/mfd/max8925.h>
#include <linux/slab.h>
#include <linux/module.h>

#define MAX_BRIGHTNESS		(0xff)
#define MIN_BRIGHTNESS		(0)

#define LWX_FREQ(x)		(((x - 601) / 100) & 0x7)

struct max8925_backlight_data {
	struct max8925_chip	*chip;

<<<<<<< HEAD
	int		current_brightness;
=======
	int	current_brightness;
	int	reg_mode_cntl;
	int	reg_cntl;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
};

static int max8925_backlight_set(struct backlight_device *bl, int brightness)
{
	struct max8925_backlight_data *data = bl_get_data(bl);
	struct max8925_chip *chip = data->chip;
	unsigned char value;
	int ret;

	if (brightness > MAX_BRIGHTNESS)
		value = MAX_BRIGHTNESS;
	else
		value = brightness;

<<<<<<< HEAD
	ret = max8925_reg_write(chip->i2c, MAX8925_WLED_CNTL, value);
=======
	ret = max8925_reg_write(chip->i2c, data->reg_cntl, value);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	if (ret < 0)
		goto out;

	if (!data->current_brightness && brightness)
		/* enable WLED output */
<<<<<<< HEAD
		ret = max8925_set_bits(chip->i2c, MAX8925_WLED_MODE_CNTL, 1, 1);
	else if (!brightness)
		/* disable WLED output */
		ret = max8925_set_bits(chip->i2c, MAX8925_WLED_MODE_CNTL, 1, 0);
=======
		ret = max8925_set_bits(chip->i2c, data->reg_mode_cntl, 1, 1);
	else if (!brightness)
		/* disable WLED output */
		ret = max8925_set_bits(chip->i2c, data->reg_mode_cntl, 1, 0);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	if (ret < 0)
		goto out;
	dev_dbg(chip->dev, "set brightness %d\n", value);
	data->current_brightness = value;
	return 0;
out:
	dev_dbg(chip->dev, "set brightness %d failure with return value:%d\n",
		value, ret);
	return ret;
}

static int max8925_backlight_update_status(struct backlight_device *bl)
{
<<<<<<< HEAD
	int brightness = bl->props.brightness;

	if (bl->props.power != FB_BLANK_UNBLANK)
		brightness = 0;

	if (bl->props.fb_blank != FB_BLANK_UNBLANK)
		brightness = 0;

	if (bl->props.state & BL_CORE_SUSPENDED)
		brightness = 0;

	return max8925_backlight_set(bl, brightness);
=======
	return max8925_backlight_set(bl, backlight_get_brightness(bl));
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

static int max8925_backlight_get_brightness(struct backlight_device *bl)
{
	struct max8925_backlight_data *data = bl_get_data(bl);
	struct max8925_chip *chip = data->chip;
	int ret;

<<<<<<< HEAD
	ret = max8925_reg_read(chip->i2c, MAX8925_WLED_CNTL);
=======
	ret = max8925_reg_read(chip->i2c, data->reg_cntl);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	if (ret < 0)
		return -EINVAL;
	data->current_brightness = ret;
	dev_dbg(chip->dev, "get brightness %d\n", data->current_brightness);
	return ret;
}

static const struct backlight_ops max8925_backlight_ops = {
	.options	= BL_CORE_SUSPENDRESUME,
	.update_status	= max8925_backlight_update_status,
	.get_brightness	= max8925_backlight_get_brightness,
};

<<<<<<< HEAD
static int __devinit max8925_backlight_probe(struct platform_device *pdev)
{
	struct max8925_chip *chip = dev_get_drvdata(pdev->dev.parent);
	struct max8925_platform_data *max8925_pdata;
	struct max8925_backlight_pdata *pdata = NULL;
=======
static void max8925_backlight_dt_init(struct platform_device *pdev)
{
	struct device_node *nproot = pdev->dev.parent->of_node, *np;
	struct max8925_backlight_pdata *pdata;
	u32 val;

	if (!nproot || !IS_ENABLED(CONFIG_OF))
		return;

	pdata = devm_kzalloc(&pdev->dev,
			     sizeof(struct max8925_backlight_pdata),
			     GFP_KERNEL);
	if (!pdata)
		return;

	np = of_get_child_by_name(nproot, "backlight");
	if (!np) {
		dev_err(&pdev->dev, "failed to find backlight node\n");
		return;
	}

	if (!of_property_read_u32(np, "maxim,max8925-dual-string", &val))
		pdata->dual_string = val;

	of_node_put(np);

	pdev->dev.platform_data = pdata;
}

static int max8925_backlight_probe(struct platform_device *pdev)
{
	struct max8925_chip *chip = dev_get_drvdata(pdev->dev.parent);
	struct max8925_backlight_pdata *pdata;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	struct max8925_backlight_data *data;
	struct backlight_device *bl;
	struct backlight_properties props;
	struct resource *res;
<<<<<<< HEAD
	char name[MAX8925_NAME_SIZE];
	unsigned char value;
	int ret;

	res = platform_get_resource(pdev, IORESOURCE_IO, 0);
	if (res == NULL) {
		dev_err(&pdev->dev, "No I/O resource!\n");
		return -EINVAL;
	}

	if (pdev->dev.parent->platform_data) {
		max8925_pdata = pdev->dev.parent->platform_data;
		pdata = max8925_pdata->backlight;
	}

	if (!pdata) {
		dev_err(&pdev->dev, "platform data isn't assigned to "
			"backlight\n");
		return -EINVAL;
	}
=======
	unsigned char value;
	int ret = 0;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	data = devm_kzalloc(&pdev->dev, sizeof(struct max8925_backlight_data),
			    GFP_KERNEL);
	if (data == NULL)
		return -ENOMEM;
<<<<<<< HEAD
	strncpy(name, res->name, MAX8925_NAME_SIZE);
=======

	res = platform_get_resource(pdev, IORESOURCE_REG, 0);
	if (!res) {
		dev_err(&pdev->dev, "No REG resource for mode control!\n");
		return -ENXIO;
	}
	data->reg_mode_cntl = res->start;
	res = platform_get_resource(pdev, IORESOURCE_REG, 1);
	if (!res) {
		dev_err(&pdev->dev, "No REG resource for control!\n");
		return -ENXIO;
	}
	data->reg_cntl = res->start;

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	data->chip = chip;
	data->current_brightness = 0;

	memset(&props, 0, sizeof(struct backlight_properties));
	props.type = BACKLIGHT_RAW;
	props.max_brightness = MAX_BRIGHTNESS;
<<<<<<< HEAD
	bl = backlight_device_register(name, &pdev->dev, data,
=======
	bl = devm_backlight_device_register(&pdev->dev, "max8925-backlight",
					&pdev->dev, data,
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
					&max8925_backlight_ops, &props);
	if (IS_ERR(bl)) {
		dev_err(&pdev->dev, "failed to register backlight\n");
		return PTR_ERR(bl);
	}
	bl->props.brightness = MAX_BRIGHTNESS;

	platform_set_drvdata(pdev, bl);

	value = 0;
<<<<<<< HEAD
	if (pdata->lxw_scl)
		value |= (1 << 7);
	if (pdata->lxw_freq)
		value |= (LWX_FREQ(pdata->lxw_freq) << 4);
	if (pdata->dual_string)
		value |= (1 << 1);
	ret = max8925_set_bits(chip->i2c, MAX8925_WLED_MODE_CNTL, 0xfe, value);
	if (ret < 0)
		goto out;

	backlight_update_status(bl);
	return 0;
out:
	backlight_device_unregister(bl);
	return ret;
}

static int __devexit max8925_backlight_remove(struct platform_device *pdev)
{
	struct backlight_device *bl = platform_get_drvdata(pdev);

	backlight_device_unregister(bl);
	return 0;
=======
	if (!pdev->dev.platform_data)
		max8925_backlight_dt_init(pdev);

	pdata = pdev->dev.platform_data;
	if (pdata) {
		if (pdata->lxw_scl)
			value |= (1 << 7);
		if (pdata->lxw_freq)
			value |= (LWX_FREQ(pdata->lxw_freq) << 4);
		if (pdata->dual_string)
			value |= (1 << 1);
	}
	ret = max8925_set_bits(chip->i2c, data->reg_mode_cntl, 0xfe, value);
	if (ret < 0)
		return ret;
	backlight_update_status(bl);
	return 0;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

static struct platform_driver max8925_backlight_driver = {
	.driver		= {
		.name	= "max8925-backlight",
<<<<<<< HEAD
		.owner	= THIS_MODULE,
	},
	.probe		= max8925_backlight_probe,
	.remove		= __devexit_p(max8925_backlight_remove),
=======
	},
	.probe		= max8925_backlight_probe,
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
};

module_platform_driver(max8925_backlight_driver);

MODULE_DESCRIPTION("Backlight Driver for Maxim MAX8925");
MODULE_AUTHOR("Haojian Zhuang <haojian.zhuang@marvell.com>");
MODULE_LICENSE("GPL");
MODULE_ALIAS("platform:max8925-backlight");
