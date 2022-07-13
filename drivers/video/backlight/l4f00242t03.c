<<<<<<< HEAD
=======
// SPDX-License-Identifier: GPL-2.0-only
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
/*
 * l4f00242t03.c -- support for Epson L4F00242T03 LCD
 *
 * Copyright 2007-2009 Freescale Semiconductor, Inc. All Rights Reserved.
 *
 * Copyright (c) 2009 Alberto Panizzo <maramaopercheseimorto@gmail.com>
<<<<<<< HEAD
 * 	Inspired by Marek Vasut work in l4f00242t03.c
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 */

=======
 *	Inspired by Marek Vasut work in l4f00242t03.c
 */

#define pr_fmt(fmt) KBUILD_MODNAME ": " fmt

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#include <linux/device.h>
#include <linux/kernel.h>
#include <linux/delay.h>
#include <linux/module.h>
<<<<<<< HEAD
#include <linux/gpio.h>
#include <linux/lcd.h>
#include <linux/slab.h>
#include <linux/regulator/consumer.h>

#include <linux/spi/spi.h>
#include <linux/spi/l4f00242t03.h>
=======
#include <linux/gpio/consumer.h>
#include <linux/lcd.h>
#include <linux/slab.h>
#include <linux/regulator/consumer.h>
#include <linux/spi/spi.h>
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

struct l4f00242t03_priv {
	struct spi_device	*spi;
	struct lcd_device	*ld;
	int lcd_state;
	struct regulator *io_reg;
	struct regulator *core_reg;
<<<<<<< HEAD
};


static void l4f00242t03_reset(unsigned int gpio)
{
	pr_debug("l4f00242t03_reset.\n");
	gpio_set_value(gpio, 1);
	mdelay(100);
	gpio_set_value(gpio, 0);
	mdelay(10);	/* tRES >= 100us */
	gpio_set_value(gpio, 1);
=======
	struct gpio_desc *reset;
	struct gpio_desc *enable;
};

static void l4f00242t03_reset(struct gpio_desc *gpiod)
{
	pr_debug("l4f00242t03_reset.\n");
	gpiod_set_value(gpiod, 1);
	mdelay(100);
	gpiod_set_value(gpiod, 0);
	mdelay(10);	/* tRES >= 100us */
	gpiod_set_value(gpiod, 1);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	mdelay(20);
}

#define param(x) ((x) | 0x100)

static void l4f00242t03_lcd_init(struct spi_device *spi)
{
<<<<<<< HEAD
	struct l4f00242t03_pdata *pdata = spi->dev.platform_data;
	struct l4f00242t03_priv *priv = dev_get_drvdata(&spi->dev);
	const u16 cmd[] = { 0x36, param(0), 0x3A, param(0x60) };

	dev_dbg(&spi->dev, "initializing LCD\n");

	regulator_set_voltage(priv->io_reg, 1800000, 1800000);
	regulator_enable(priv->io_reg);

	regulator_set_voltage(priv->core_reg, 2800000, 2800000);
	regulator_enable(priv->core_reg);

	l4f00242t03_reset(pdata->reset_gpio);

	gpio_set_value(pdata->data_enable_gpio, 1);
=======
	struct l4f00242t03_priv *priv = spi_get_drvdata(spi);
	const u16 cmd[] = { 0x36, param(0), 0x3A, param(0x60) };
	int ret;

	dev_dbg(&spi->dev, "initializing LCD\n");

	ret = regulator_set_voltage(priv->io_reg, 1800000, 1800000);
	if (ret) {
		dev_err(&spi->dev, "failed to set the IO regulator voltage.\n");
		return;
	}
	ret = regulator_enable(priv->io_reg);
	if (ret) {
		dev_err(&spi->dev, "failed to enable the IO regulator.\n");
		return;
	}

	ret = regulator_set_voltage(priv->core_reg, 2800000, 2800000);
	if (ret) {
		dev_err(&spi->dev, "failed to set the core regulator voltage.\n");
		regulator_disable(priv->io_reg);
		return;
	}
	ret = regulator_enable(priv->core_reg);
	if (ret) {
		dev_err(&spi->dev, "failed to enable the core regulator.\n");
		regulator_disable(priv->io_reg);
		return;
	}

	l4f00242t03_reset(priv->reset);

	gpiod_set_value(priv->enable, 1);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	msleep(60);
	spi_write(spi, (const u8 *)cmd, ARRAY_SIZE(cmd) * sizeof(u16));
}

static void l4f00242t03_lcd_powerdown(struct spi_device *spi)
{
<<<<<<< HEAD
	struct l4f00242t03_pdata *pdata = spi->dev.platform_data;
	struct l4f00242t03_priv *priv = dev_get_drvdata(&spi->dev);

	dev_dbg(&spi->dev, "Powering down LCD\n");

	gpio_set_value(pdata->data_enable_gpio, 0);
=======
	struct l4f00242t03_priv *priv = spi_get_drvdata(spi);

	dev_dbg(&spi->dev, "Powering down LCD\n");

	gpiod_set_value(priv->enable, 0);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	regulator_disable(priv->io_reg);
	regulator_disable(priv->core_reg);
}

static int l4f00242t03_lcd_power_get(struct lcd_device *ld)
{
	struct l4f00242t03_priv *priv = lcd_get_data(ld);

	return priv->lcd_state;
}

static int l4f00242t03_lcd_power_set(struct lcd_device *ld, int power)
{
	struct l4f00242t03_priv *priv = lcd_get_data(ld);
	struct spi_device *spi = priv->spi;

	const u16 slpout = 0x11;
	const u16 dison = 0x29;

	const u16 slpin = 0x10;
	const u16 disoff = 0x28;

	if (power <= FB_BLANK_NORMAL) {
		if (priv->lcd_state <= FB_BLANK_NORMAL) {
			/* Do nothing, the LCD is running */
		} else if (priv->lcd_state < FB_BLANK_POWERDOWN) {
			dev_dbg(&spi->dev, "Resuming LCD\n");

			spi_write(spi, (const u8 *)&slpout, sizeof(u16));
			msleep(60);
			spi_write(spi, (const u8 *)&dison, sizeof(u16));
		} else {
			/* priv->lcd_state == FB_BLANK_POWERDOWN */
			l4f00242t03_lcd_init(spi);
			priv->lcd_state = FB_BLANK_VSYNC_SUSPEND;
			l4f00242t03_lcd_power_set(priv->ld, power);
		}
	} else if (power < FB_BLANK_POWERDOWN) {
		if (priv->lcd_state <= FB_BLANK_NORMAL) {
			/* Send the display in standby */
			dev_dbg(&spi->dev, "Standby the LCD\n");

			spi_write(spi, (const u8 *)&disoff, sizeof(u16));
			msleep(60);
			spi_write(spi, (const u8 *)&slpin, sizeof(u16));
		} else if (priv->lcd_state < FB_BLANK_POWERDOWN) {
			/* Do nothing, the LCD is already in standby */
		} else {
			/* priv->lcd_state == FB_BLANK_POWERDOWN */
			l4f00242t03_lcd_init(spi);
			priv->lcd_state = FB_BLANK_UNBLANK;
			l4f00242t03_lcd_power_set(ld, power);
		}
	} else {
		/* power == FB_BLANK_POWERDOWN */
		if (priv->lcd_state != FB_BLANK_POWERDOWN) {
			/* Clear the screen before shutting down */
			spi_write(spi, (const u8 *)&disoff, sizeof(u16));
			msleep(60);
			l4f00242t03_lcd_powerdown(spi);
		}
	}

	priv->lcd_state = power;

	return 0;
}

static struct lcd_ops l4f_ops = {
	.set_power	= l4f00242t03_lcd_power_set,
	.get_power	= l4f00242t03_lcd_power_get,
};

<<<<<<< HEAD
static int __devinit l4f00242t03_probe(struct spi_device *spi)
{
	struct l4f00242t03_priv *priv;
	struct l4f00242t03_pdata *pdata = spi->dev.platform_data;
	int ret;

	if (pdata == NULL) {
		dev_err(&spi->dev, "Uninitialized platform data.\n");
		return -EINVAL;
	}

	priv = kzalloc(sizeof(struct l4f00242t03_priv), GFP_KERNEL);

	if (priv == NULL) {
		dev_err(&spi->dev, "No memory for this device.\n");
		return -ENOMEM;
	}

	dev_set_drvdata(&spi->dev, priv);
=======
static int l4f00242t03_probe(struct spi_device *spi)
{
	struct l4f00242t03_priv *priv;

	priv = devm_kzalloc(&spi->dev, sizeof(struct l4f00242t03_priv),
				GFP_KERNEL);
	if (priv == NULL)
		return -ENOMEM;

	spi_set_drvdata(spi, priv);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	spi->bits_per_word = 9;
	spi_setup(spi);

	priv->spi = spi;

<<<<<<< HEAD
	ret = gpio_request_one(pdata->reset_gpio, GPIOF_OUT_INIT_HIGH,
						"lcd l4f00242t03 reset");
	if (ret) {
		dev_err(&spi->dev,
			"Unable to get the lcd l4f00242t03 reset gpio.\n");
		goto err;
	}

	ret = gpio_request_one(pdata->data_enable_gpio, GPIOF_OUT_INIT_LOW,
						"lcd l4f00242t03 data enable");
	if (ret) {
		dev_err(&spi->dev,
			"Unable to get the lcd l4f00242t03 data en gpio.\n");
		goto err2;
	}

	priv->io_reg = regulator_get(&spi->dev, "vdd");
	if (IS_ERR(priv->io_reg)) {
		ret = PTR_ERR(priv->io_reg);
		dev_err(&spi->dev, "%s: Unable to get the IO regulator\n",
		       __func__);
		goto err3;
	}

	priv->core_reg = regulator_get(&spi->dev, "vcore");
	if (IS_ERR(priv->core_reg)) {
		ret = PTR_ERR(priv->core_reg);
		dev_err(&spi->dev, "%s: Unable to get the core regulator\n",
		       __func__);
		goto err4;
	}

	priv->ld = lcd_device_register("l4f00242t03",
					&spi->dev, priv, &l4f_ops);
	if (IS_ERR(priv->ld)) {
		ret = PTR_ERR(priv->ld);
		goto err5;
	}
=======
	priv->reset = devm_gpiod_get(&spi->dev, "reset", GPIOD_OUT_HIGH);
	if (IS_ERR(priv->reset))
		return dev_err_probe(&spi->dev, PTR_ERR(priv->reset),
				     "Unable to get the lcd l4f00242t03 reset gpio.\n");
	gpiod_set_consumer_name(priv->reset, "lcd l4f00242t03 reset");

	priv->enable = devm_gpiod_get(&spi->dev, "enable", GPIOD_OUT_LOW);
	if (IS_ERR(priv->enable))
		return dev_err_probe(&spi->dev, PTR_ERR(priv->enable),
				     "Unable to get the lcd l4f00242t03 data en gpio.\n");
	gpiod_set_consumer_name(priv->enable, "lcd l4f00242t03 data enable");

	priv->io_reg = devm_regulator_get(&spi->dev, "vdd");
	if (IS_ERR(priv->io_reg))
		return dev_err_probe(&spi->dev, PTR_ERR(priv->io_reg),
				     "%s: Unable to get the IO regulator\n",
				     __func__);

	priv->core_reg = devm_regulator_get(&spi->dev, "vcore");
	if (IS_ERR(priv->core_reg))
		return dev_err_probe(&spi->dev, PTR_ERR(priv->core_reg),
				     "%s: Unable to get the core regulator\n",
				     __func__);

	priv->ld = devm_lcd_device_register(&spi->dev, "l4f00242t03", &spi->dev,
					priv, &l4f_ops);
	if (IS_ERR(priv->ld))
		return PTR_ERR(priv->ld);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	/* Init the LCD */
	l4f00242t03_lcd_init(spi);
	priv->lcd_state = FB_BLANK_VSYNC_SUSPEND;
	l4f00242t03_lcd_power_set(priv->ld, FB_BLANK_UNBLANK);

	dev_info(&spi->dev, "Epson l4f00242t03 lcd probed.\n");

	return 0;
<<<<<<< HEAD

err5:
	regulator_put(priv->core_reg);
err4:
	regulator_put(priv->io_reg);
err3:
	gpio_free(pdata->data_enable_gpio);
err2:
	gpio_free(pdata->reset_gpio);
err:
	kfree(priv);

	return ret;
}

static int __devexit l4f00242t03_remove(struct spi_device *spi)
{
	struct l4f00242t03_priv *priv = dev_get_drvdata(&spi->dev);
	struct l4f00242t03_pdata *pdata = priv->spi->dev.platform_data;

	l4f00242t03_lcd_power_set(priv->ld, FB_BLANK_POWERDOWN);
	lcd_device_unregister(priv->ld);

	dev_set_drvdata(&spi->dev, NULL);

	gpio_free(pdata->data_enable_gpio);
	gpio_free(pdata->reset_gpio);

	regulator_put(priv->io_reg);
	regulator_put(priv->core_reg);

	kfree(priv);

	return 0;
=======
}

static void l4f00242t03_remove(struct spi_device *spi)
{
	struct l4f00242t03_priv *priv = spi_get_drvdata(spi);

	l4f00242t03_lcd_power_set(priv->ld, FB_BLANK_POWERDOWN);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

static void l4f00242t03_shutdown(struct spi_device *spi)
{
<<<<<<< HEAD
	struct l4f00242t03_priv *priv = dev_get_drvdata(&spi->dev);
=======
	struct l4f00242t03_priv *priv = spi_get_drvdata(spi);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	if (priv)
		l4f00242t03_lcd_power_set(priv->ld, FB_BLANK_POWERDOWN);

}

static struct spi_driver l4f00242t03_driver = {
	.driver = {
		.name	= "l4f00242t03",
<<<<<<< HEAD
		.owner	= THIS_MODULE,
	},
	.probe		= l4f00242t03_probe,
	.remove		= __devexit_p(l4f00242t03_remove),
=======
	},
	.probe		= l4f00242t03_probe,
	.remove		= l4f00242t03_remove,
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	.shutdown	= l4f00242t03_shutdown,
};

module_spi_driver(l4f00242t03_driver);

MODULE_AUTHOR("Alberto Panizzo <maramaopercheseimorto@gmail.com>");
MODULE_DESCRIPTION("EPSON L4F00242T03 LCD");
MODULE_LICENSE("GPL v2");
