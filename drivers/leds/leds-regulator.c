<<<<<<< HEAD
=======
// SPDX-License-Identifier: GPL-2.0-only
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
/*
 * leds-regulator.c - LED class driver for regulator driven LEDs.
 *
 * Copyright (C) 2009 Antonio Ospite <ospite@studenti.unina.it>
 *
 * Inspired by leds-wm8350 driver.
<<<<<<< HEAD
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 *
 */

#include <linux/module.h>
#include <linux/err.h>
#include <linux/slab.h>
#include <linux/workqueue.h>
=======
 */

#include <linux/module.h>
#include <linux/mod_devicetable.h>
#include <linux/err.h>
#include <linux/slab.h>
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#include <linux/leds.h>
#include <linux/leds-regulator.h>
#include <linux/platform_device.h>
#include <linux/regulator/consumer.h>

#define to_regulator_led(led_cdev) \
	container_of(led_cdev, struct regulator_led, cdev)

struct regulator_led {
	struct led_classdev cdev;
<<<<<<< HEAD
	enum led_brightness value;
	int enabled;
	struct mutex mutex;
	struct work_struct work;
=======
	int enabled;
	struct mutex mutex;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	struct regulator *vcc;
};

static inline int led_regulator_get_max_brightness(struct regulator *supply)
{
	int ret;
	int voltage = regulator_list_voltage(supply, 0);

	if (voltage <= 0)
		return 1;

	/* even if regulator can't change voltages,
	 * we still assume it can change status
	 * and the LED can be turned on and off.
	 */
	ret = regulator_set_voltage(supply, voltage, voltage);
	if (ret < 0)
		return 1;

	return regulator_count_voltages(supply);
}

static int led_regulator_get_voltage(struct regulator *supply,
		enum led_brightness brightness)
{
	if (brightness == 0)
		return -EINVAL;

	return regulator_list_voltage(supply, brightness - 1);
}


static void regulator_led_enable(struct regulator_led *led)
{
	int ret;

	if (led->enabled)
		return;

	ret = regulator_enable(led->vcc);
	if (ret != 0) {
		dev_err(led->cdev.dev, "Failed to enable vcc: %d\n", ret);
		return;
	}

	led->enabled = 1;
}

static void regulator_led_disable(struct regulator_led *led)
{
	int ret;

	if (!led->enabled)
		return;

	ret = regulator_disable(led->vcc);
	if (ret != 0) {
		dev_err(led->cdev.dev, "Failed to disable vcc: %d\n", ret);
		return;
	}

	led->enabled = 0;
}

<<<<<<< HEAD
static void regulator_led_set_value(struct regulator_led *led)
{
	int voltage;
	int ret;

	mutex_lock(&led->mutex);

	if (led->value == LED_OFF) {
=======
static int regulator_led_brightness_set(struct led_classdev *led_cdev,
					 enum led_brightness value)
{
	struct regulator_led *led = to_regulator_led(led_cdev);
	int voltage;
	int ret = 0;

	mutex_lock(&led->mutex);

	if (value == LED_OFF) {
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		regulator_led_disable(led);
		goto out;
	}

	if (led->cdev.max_brightness > 1) {
<<<<<<< HEAD
		voltage = led_regulator_get_voltage(led->vcc, led->value);
		dev_dbg(led->cdev.dev, "brightness: %d voltage: %d\n",
				led->value, voltage);
=======
		voltage = led_regulator_get_voltage(led->vcc, value);
		dev_dbg(led->cdev.dev, "brightness: %d voltage: %d\n",
				value, voltage);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

		ret = regulator_set_voltage(led->vcc, voltage, voltage);
		if (ret != 0)
			dev_err(led->cdev.dev, "Failed to set voltage %d: %d\n",
				voltage, ret);
	}

	regulator_led_enable(led);

out:
	mutex_unlock(&led->mutex);
<<<<<<< HEAD
}

static void led_work(struct work_struct *work)
{
	struct regulator_led *led;

	led = container_of(work, struct regulator_led, work);
	regulator_led_set_value(led);
}

static void regulator_led_brightness_set(struct led_classdev *led_cdev,
			   enum led_brightness value)
{
	struct regulator_led *led = to_regulator_led(led_cdev);

	led->value = value;
	schedule_work(&led->work);
}

static int __devinit regulator_led_probe(struct platform_device *pdev)
{
	struct led_regulator_platform_data *pdata = pdev->dev.platform_data;
=======
	return ret;
}

static int regulator_led_probe(struct platform_device *pdev)
{
	struct led_regulator_platform_data *pdata =
			dev_get_platdata(&pdev->dev);
	struct device *dev = &pdev->dev;
	struct led_init_data init_data = {};
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	struct regulator_led *led;
	struct regulator *vcc;
	int ret = 0;

<<<<<<< HEAD
	if (pdata == NULL) {
		dev_err(&pdev->dev, "no platform data\n");
		return -ENODEV;
	}

	vcc = regulator_get_exclusive(&pdev->dev, "vled");
	if (IS_ERR(vcc)) {
		dev_err(&pdev->dev, "Cannot get vcc for %s\n", pdata->name);
		return PTR_ERR(vcc);
	}

	led = kzalloc(sizeof(*led), GFP_KERNEL);
	if (led == NULL) {
		ret = -ENOMEM;
		goto err_vcc;
	}

	led->cdev.max_brightness = led_regulator_get_max_brightness(vcc);
	if (pdata->brightness > led->cdev.max_brightness) {
		dev_err(&pdev->dev, "Invalid default brightness %d\n",
				pdata->brightness);
		ret = -EINVAL;
		goto err_led;
	}
	led->value = pdata->brightness;

	led->cdev.brightness_set = regulator_led_brightness_set;
	led->cdev.name = pdata->name;
=======
	vcc = devm_regulator_get_exclusive(dev, "vled");
	if (IS_ERR(vcc)) {
		dev_err(dev, "Cannot get vcc\n");
		return PTR_ERR(vcc);
	}

	led = devm_kzalloc(dev, sizeof(*led), GFP_KERNEL);
	if (led == NULL)
		return -ENOMEM;

	init_data.fwnode = dev->fwnode;

	led->cdev.max_brightness = led_regulator_get_max_brightness(vcc);
	/* Legacy platform data label assignment */
	if (pdata) {
		if (pdata->brightness > led->cdev.max_brightness) {
			dev_err(dev, "Invalid default brightness %d\n",
				pdata->brightness);
			return -EINVAL;
		}
		led->cdev.brightness = pdata->brightness;
		init_data.default_label = pdata->name;
	}

	led->cdev.brightness_set_blocking = regulator_led_brightness_set;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	led->cdev.flags |= LED_CORE_SUSPENDRESUME;
	led->vcc = vcc;

	/* to handle correctly an already enabled regulator */
	if (regulator_is_enabled(led->vcc))
		led->enabled = 1;

	mutex_init(&led->mutex);
<<<<<<< HEAD
	INIT_WORK(&led->work, led_work);

	platform_set_drvdata(pdev, led);

	ret = led_classdev_register(&pdev->dev, &led->cdev);
	if (ret < 0) {
		cancel_work_sync(&led->work);
		goto err_led;
	}

	/* to expose the default value to userspace */
	led->cdev.brightness = led->value;

	/* Set the default led status */
	regulator_led_set_value(led);

	return 0;

err_led:
	kfree(led);
err_vcc:
	regulator_put(vcc);
	return ret;
}

static int __devexit regulator_led_remove(struct platform_device *pdev)
=======

	platform_set_drvdata(pdev, led);

	ret = led_classdev_register_ext(dev, &led->cdev, &init_data);
	if (ret < 0)
		return ret;

	return 0;
}

static void regulator_led_remove(struct platform_device *pdev)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
{
	struct regulator_led *led = platform_get_drvdata(pdev);

	led_classdev_unregister(&led->cdev);
<<<<<<< HEAD
	cancel_work_sync(&led->work);
	regulator_led_disable(led);
	regulator_put(led->vcc);
	kfree(led);
	return 0;
}

static struct platform_driver regulator_led_driver = {
	.driver = {
		   .name  = "leds-regulator",
		   .owner = THIS_MODULE,
		   },
	.probe  = regulator_led_probe,
	.remove = __devexit_p(regulator_led_remove),
=======
	regulator_led_disable(led);
}

static const struct of_device_id regulator_led_of_match[] = {
	{ .compatible = "regulator-led", },
	{}
};
MODULE_DEVICE_TABLE(of, regulator_led_of_match);

static struct platform_driver regulator_led_driver = {
	.driver = {
		.name  = "leds-regulator",
		.of_match_table = regulator_led_of_match,
	},
	.probe  = regulator_led_probe,
	.remove_new = regulator_led_remove,
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
};

module_platform_driver(regulator_led_driver);

MODULE_AUTHOR("Antonio Ospite <ospite@studenti.unina.it>");
MODULE_DESCRIPTION("Regulator driven LED driver");
MODULE_LICENSE("GPL");
MODULE_ALIAS("platform:leds-regulator");
