<<<<<<< HEAD
=======
// SPDX-License-Identifier: GPL-2.0-only
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
/*
 * Backlight driver for Pandora handheld.
 * Pandora uses TWL4030 PWM0 -> TPS61161 combo for control backlight.
 * Based on pwm_bl.c
 *
 * Copyright 2009,2012 Gražvydas Ignotas <notasas@gmail.com>
<<<<<<< HEAD
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 */

#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/platform_device.h>
#include <linux/delay.h>
#include <linux/fb.h>
#include <linux/backlight.h>
<<<<<<< HEAD
#include <linux/i2c/twl.h>
=======
#include <linux/mfd/twl.h>
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#include <linux/err.h>

#define TWL_PWM0_ON		0x00
#define TWL_PWM0_OFF		0x01

#define TWL_INTBR_GPBR1		0x0c
#define TWL_INTBR_PMBR1		0x0d

#define TWL_PMBR1_PWM0_MUXMASK	0x0c
#define TWL_PMBR1_PWM0		0x04
#define PWM0_CLK_ENABLE		BIT(0)
#define PWM0_ENABLE		BIT(2)

/* range accepted by hardware */
#define MIN_VALUE 9
#define MAX_VALUE 63
#define MAX_USER_VALUE (MAX_VALUE - MIN_VALUE)

<<<<<<< HEAD
#define PANDORABL_WAS_OFF BL_CORE_DRIVER1
=======
struct pandora_private {
	unsigned old_state;
#define PANDORABL_WAS_OFF 1
};
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

static int pandora_backlight_update_status(struct backlight_device *bl)
{
	int brightness = bl->props.brightness;
<<<<<<< HEAD
=======
	struct pandora_private *priv = bl_get_data(bl);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	u8 r;

	if (bl->props.power != FB_BLANK_UNBLANK)
		brightness = 0;
	if (bl->props.state & BL_CORE_FBBLANK)
		brightness = 0;
	if (bl->props.state & BL_CORE_SUSPENDED)
		brightness = 0;

	if ((unsigned int)brightness > MAX_USER_VALUE)
		brightness = MAX_USER_VALUE;

	if (brightness == 0) {
<<<<<<< HEAD
		if (bl->props.state & PANDORABL_WAS_OFF)
=======
		if (priv->old_state == PANDORABL_WAS_OFF)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
			goto done;

		/* first disable PWM0 output, then clock */
		twl_i2c_read_u8(TWL4030_MODULE_INTBR, &r, TWL_INTBR_GPBR1);
		r &= ~PWM0_ENABLE;
		twl_i2c_write_u8(TWL4030_MODULE_INTBR, r, TWL_INTBR_GPBR1);
		r &= ~PWM0_CLK_ENABLE;
		twl_i2c_write_u8(TWL4030_MODULE_INTBR, r, TWL_INTBR_GPBR1);

		goto done;
	}

<<<<<<< HEAD
	if (bl->props.state & PANDORABL_WAS_OFF) {
=======
	if (priv->old_state == PANDORABL_WAS_OFF) {
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		/*
		 * set PWM duty cycle to max. TPS61161 seems to use this
		 * to calibrate it's PWM sensitivity when it starts.
		 */
<<<<<<< HEAD
		twl_i2c_write_u8(TWL4030_MODULE_PWM0, MAX_VALUE,
					TWL_PWM0_OFF);
=======
		twl_i2c_write_u8(TWL_MODULE_PWM, MAX_VALUE, TWL_PWM0_OFF);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

		/* first enable clock, then PWM0 out */
		twl_i2c_read_u8(TWL4030_MODULE_INTBR, &r, TWL_INTBR_GPBR1);
		r &= ~PWM0_ENABLE;
		r |= PWM0_CLK_ENABLE;
		twl_i2c_write_u8(TWL4030_MODULE_INTBR, r, TWL_INTBR_GPBR1);
		r |= PWM0_ENABLE;
		twl_i2c_write_u8(TWL4030_MODULE_INTBR, r, TWL_INTBR_GPBR1);

		/*
		 * TI made it very easy to enable digital control, so easy that
		 * it often triggers unintentionally and disabes PWM control,
		 * so wait until 1 wire mode detection window ends.
		 */
		usleep_range(2000, 10000);
	}

<<<<<<< HEAD
	twl_i2c_write_u8(TWL4030_MODULE_PWM0, MIN_VALUE + brightness,
				TWL_PWM0_OFF);

done:
	if (brightness != 0)
		bl->props.state &= ~PANDORABL_WAS_OFF;
	else
		bl->props.state |= PANDORABL_WAS_OFF;
=======
	twl_i2c_write_u8(TWL_MODULE_PWM, MIN_VALUE + brightness, TWL_PWM0_OFF);

done:
	if (brightness != 0)
		priv->old_state = 0;
	else
		priv->old_state = PANDORABL_WAS_OFF;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	return 0;
}

<<<<<<< HEAD
static int pandora_backlight_get_brightness(struct backlight_device *bl)
{
	return bl->props.brightness;
}

static const struct backlight_ops pandora_backlight_ops = {
	.options	= BL_CORE_SUSPENDRESUME,
	.update_status	= pandora_backlight_update_status,
	.get_brightness	= pandora_backlight_get_brightness,
=======
static const struct backlight_ops pandora_backlight_ops = {
	.options	= BL_CORE_SUSPENDRESUME,
	.update_status	= pandora_backlight_update_status,
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
};

static int pandora_backlight_probe(struct platform_device *pdev)
{
	struct backlight_properties props;
	struct backlight_device *bl;
<<<<<<< HEAD
	u8 r;

	memset(&props, 0, sizeof(props));
	props.max_brightness = MAX_USER_VALUE;
	props.type = BACKLIGHT_RAW;
	bl = backlight_device_register(pdev->name, &pdev->dev,
			NULL, &pandora_backlight_ops, &props);
=======
	struct pandora_private *priv;
	u8 r;

	priv = devm_kmalloc(&pdev->dev, sizeof(*priv), GFP_KERNEL);
	if (!priv)
		return -ENOMEM;

	memset(&props, 0, sizeof(props));
	props.max_brightness = MAX_USER_VALUE;
	props.type = BACKLIGHT_RAW;
	bl = devm_backlight_device_register(&pdev->dev, pdev->name, &pdev->dev,
					priv, &pandora_backlight_ops, &props);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	if (IS_ERR(bl)) {
		dev_err(&pdev->dev, "failed to register backlight\n");
		return PTR_ERR(bl);
	}

	platform_set_drvdata(pdev, bl);

	/* 64 cycle period, ON position 0 */
<<<<<<< HEAD
	twl_i2c_write_u8(TWL4030_MODULE_PWM0, 0x80, TWL_PWM0_ON);

	bl->props.state |= PANDORABL_WAS_OFF;
=======
	twl_i2c_write_u8(TWL_MODULE_PWM, 0x80, TWL_PWM0_ON);

	priv->old_state = PANDORABL_WAS_OFF;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	bl->props.brightness = MAX_USER_VALUE;
	backlight_update_status(bl);

	/* enable PWM function in pin mux */
	twl_i2c_read_u8(TWL4030_MODULE_INTBR, &r, TWL_INTBR_PMBR1);
	r &= ~TWL_PMBR1_PWM0_MUXMASK;
	r |= TWL_PMBR1_PWM0;
	twl_i2c_write_u8(TWL4030_MODULE_INTBR, r, TWL_INTBR_PMBR1);

	return 0;
}

<<<<<<< HEAD
static int pandora_backlight_remove(struct platform_device *pdev)
{
	struct backlight_device *bl = platform_get_drvdata(pdev);
	backlight_device_unregister(bl);
	return 0;
}

static struct platform_driver pandora_backlight_driver = {
	.driver		= {
		.name	= "pandora-backlight",
		.owner	= THIS_MODULE,
	},
	.probe		= pandora_backlight_probe,
	.remove		= pandora_backlight_remove,
=======
static struct platform_driver pandora_backlight_driver = {
	.driver		= {
		.name	= "pandora-backlight",
	},
	.probe		= pandora_backlight_probe,
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
};

module_platform_driver(pandora_backlight_driver);

MODULE_AUTHOR("Gražvydas Ignotas <notasas@gmail.com>");
MODULE_DESCRIPTION("Pandora Backlight Driver");
MODULE_LICENSE("GPL");
MODULE_ALIAS("platform:pandora-backlight");
