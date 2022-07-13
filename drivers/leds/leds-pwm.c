<<<<<<< HEAD
=======
// SPDX-License-Identifier: GPL-2.0-only
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
/*
 * linux/drivers/leds-pwm.c
 *
 * simple PWM based LED control
 *
 * Copyright 2009 Luotao Fu @ Pengutronix (l.fu@pengutronix.de)
 *
 * based on leds-gpio.c by Raphael Assenat <raph@8d.com>
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
<<<<<<< HEAD
#include <linux/init.h>
#include <linux/platform_device.h>
#include <linux/fb.h>
#include <linux/leds.h>
#include <linux/err.h>
#include <linux/pwm.h>
#include <linux/leds_pwm.h>
#include <linux/slab.h>
=======
#include <linux/platform_device.h>
#include <linux/of.h>
#include <linux/leds.h>
#include <linux/err.h>
#include <linux/pwm.h>
#include <linux/slab.h>
#include "leds.h"

struct led_pwm {
	const char	*name;
	u8		active_low;
	u8		default_state;
	unsigned int	max_brightness;
};
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

struct led_pwm_data {
	struct led_classdev	cdev;
	struct pwm_device	*pwm;
<<<<<<< HEAD
	unsigned int 		active_low;
	unsigned int		period;
};

static void led_pwm_set(struct led_classdev *led_cdev,
	enum led_brightness brightness)
=======
	struct pwm_state	pwmstate;
	unsigned int		active_low;
};

struct led_pwm_priv {
	int num_leds;
	struct led_pwm_data leds[];
};

static int led_pwm_set(struct led_classdev *led_cdev,
		       enum led_brightness brightness)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
{
	struct led_pwm_data *led_dat =
		container_of(led_cdev, struct led_pwm_data, cdev);
	unsigned int max = led_dat->cdev.max_brightness;
<<<<<<< HEAD
	unsigned int period =  led_dat->period;

	if (brightness == 0) {
		pwm_config(led_dat->pwm, 0, period);
		pwm_disable(led_dat->pwm);
	} else {
		pwm_config(led_dat->pwm, brightness * period / max, period);
		pwm_enable(led_dat->pwm);
	}
=======
	unsigned long long duty = led_dat->pwmstate.period;

	duty *= brightness;
	do_div(duty, max);

	if (led_dat->active_low)
		duty = led_dat->pwmstate.period - duty;

	led_dat->pwmstate.duty_cycle = duty;
	led_dat->pwmstate.enabled = true;
	return pwm_apply_might_sleep(led_dat->pwm, &led_dat->pwmstate);
}

__attribute__((nonnull))
static int led_pwm_add(struct device *dev, struct led_pwm_priv *priv,
		       struct led_pwm *led, struct fwnode_handle *fwnode)
{
	struct led_pwm_data *led_data = &priv->leds[priv->num_leds];
	struct led_init_data init_data = { .fwnode = fwnode };
	int ret;

	led_data->active_low = led->active_low;
	led_data->cdev.name = led->name;
	led_data->cdev.brightness = LED_OFF;
	led_data->cdev.max_brightness = led->max_brightness;
	led_data->cdev.flags = LED_CORE_SUSPENDRESUME;

	led_data->pwm = devm_fwnode_pwm_get(dev, fwnode, NULL);
	if (IS_ERR(led_data->pwm))
		return dev_err_probe(dev, PTR_ERR(led_data->pwm),
				     "unable to request PWM for %s\n",
				     led->name);

	led_data->cdev.brightness_set_blocking = led_pwm_set;

	/* init PWM state */
	switch (led->default_state) {
	case LEDS_DEFSTATE_KEEP:
		pwm_get_state(led_data->pwm, &led_data->pwmstate);
		if (led_data->pwmstate.period)
			break;
		led->default_state = LEDS_DEFSTATE_OFF;
		dev_warn(dev,
			"failed to read period for %s, default to off",
			led->name);
		fallthrough;
	default:
		pwm_init_state(led_data->pwm, &led_data->pwmstate);
		break;
	}

	/* set brightness */
	switch (led->default_state) {
	case LEDS_DEFSTATE_ON:
		led_data->cdev.brightness = led->max_brightness;
		break;
	case LEDS_DEFSTATE_KEEP:
		{
		uint64_t brightness;

		brightness = led->max_brightness;
		brightness *= led_data->pwmstate.duty_cycle;
		do_div(brightness, led_data->pwmstate.period);
		led_data->cdev.brightness = brightness;
		}
		break;
	}

	ret = devm_led_classdev_register_ext(dev, &led_data->cdev, &init_data);
	if (ret) {
		dev_err(dev, "failed to register PWM led for %s: %d\n",
			led->name, ret);
		return ret;
	}

	if (led->default_state != LEDS_DEFSTATE_KEEP) {
		ret = led_pwm_set(&led_data->cdev, led_data->cdev.brightness);
		if (ret) {
			dev_err(dev, "failed to set led PWM value for %s: %d",
				led->name, ret);
			return ret;
		}
	}

	priv->num_leds++;
	return 0;
}

static int led_pwm_create_fwnode(struct device *dev, struct led_pwm_priv *priv)
{
	struct fwnode_handle *fwnode;
	struct led_pwm led;
	int ret;

	device_for_each_child_node(dev, fwnode) {
		memset(&led, 0, sizeof(led));

		ret = fwnode_property_read_string(fwnode, "label", &led.name);
		if (ret && is_of_node(fwnode))
			led.name = to_of_node(fwnode)->name;

		if (!led.name) {
			ret = -EINVAL;
			goto err_child_out;
		}

		led.active_low = fwnode_property_read_bool(fwnode,
							   "active-low");
		fwnode_property_read_u32(fwnode, "max-brightness",
					 &led.max_brightness);

		led.default_state = led_init_default_state_get(fwnode);

		ret = led_pwm_add(dev, priv, &led, fwnode);
		if (ret)
			goto err_child_out;
	}

	return 0;

err_child_out:
	fwnode_handle_put(fwnode);
	return ret;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

static int led_pwm_probe(struct platform_device *pdev)
{
<<<<<<< HEAD
	struct led_pwm_platform_data *pdata = pdev->dev.platform_data;
	struct led_pwm *cur_led;
	struct led_pwm_data *leds_data, *led_dat;
	int i, ret = 0;

	if (!pdata)
		return -EBUSY;

	leds_data = kzalloc(sizeof(struct led_pwm_data) * pdata->num_leds,
				GFP_KERNEL);
	if (!leds_data)
		return -ENOMEM;

	for (i = 0; i < pdata->num_leds; i++) {
		cur_led = &pdata->leds[i];
		led_dat = &leds_data[i];

		led_dat->pwm = pwm_request(cur_led->pwm_id,
				cur_led->name);
		if (IS_ERR(led_dat->pwm)) {
			ret = PTR_ERR(led_dat->pwm);
			dev_err(&pdev->dev, "unable to request PWM %d\n",
					cur_led->pwm_id);
			goto err;
		}

		led_dat->cdev.name = cur_led->name;
		led_dat->cdev.default_trigger = cur_led->default_trigger;
		led_dat->active_low = cur_led->active_low;
		led_dat->period = cur_led->pwm_period_ns;
		led_dat->cdev.brightness_set = led_pwm_set;
		led_dat->cdev.brightness = LED_OFF;
		led_dat->cdev.max_brightness = cur_led->max_brightness;
		led_dat->cdev.flags |= LED_CORE_SUSPENDRESUME;

		ret = led_classdev_register(&pdev->dev, &led_dat->cdev);
		if (ret < 0) {
			pwm_free(led_dat->pwm);
			goto err;
		}
	}

	platform_set_drvdata(pdev, leds_data);

	return 0;

err:
	if (i > 0) {
		for (i = i - 1; i >= 0; i--) {
			led_classdev_unregister(&leds_data[i].cdev);
			pwm_free(leds_data[i].pwm);
		}
	}

	kfree(leds_data);

	return ret;
}

static int __devexit led_pwm_remove(struct platform_device *pdev)
{
	int i;
	struct led_pwm_platform_data *pdata = pdev->dev.platform_data;
	struct led_pwm_data *leds_data;

	leds_data = platform_get_drvdata(pdev);

	for (i = 0; i < pdata->num_leds; i++) {
		led_classdev_unregister(&leds_data[i].cdev);
		pwm_free(leds_data[i].pwm);
	}

	kfree(leds_data);
=======
	struct led_pwm_priv *priv;
	int ret = 0;
	int count;

	count = device_get_child_node_count(&pdev->dev);

	if (!count)
		return -EINVAL;

	priv = devm_kzalloc(&pdev->dev, struct_size(priv, leds, count),
			    GFP_KERNEL);
	if (!priv)
		return -ENOMEM;

	ret = led_pwm_create_fwnode(&pdev->dev, priv);

	if (ret)
		return ret;

	platform_set_drvdata(pdev, priv);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	return 0;
}

<<<<<<< HEAD
static struct platform_driver led_pwm_driver = {
	.probe		= led_pwm_probe,
	.remove		= __devexit_p(led_pwm_remove),
	.driver		= {
		.name	= "leds_pwm",
		.owner	= THIS_MODULE,
=======
static const struct of_device_id of_pwm_leds_match[] = {
	{ .compatible = "pwm-leds", },
	{},
};
MODULE_DEVICE_TABLE(of, of_pwm_leds_match);

static struct platform_driver led_pwm_driver = {
	.probe		= led_pwm_probe,
	.driver		= {
		.name	= "leds_pwm",
		.of_match_table = of_pwm_leds_match,
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	},
};

module_platform_driver(led_pwm_driver);

MODULE_AUTHOR("Luotao Fu <l.fu@pengutronix.de>");
<<<<<<< HEAD
MODULE_DESCRIPTION("PWM LED driver for PXA");
MODULE_LICENSE("GPL");
=======
MODULE_DESCRIPTION("generic PWM LED driver");
MODULE_LICENSE("GPL v2");
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
MODULE_ALIAS("platform:leds-pwm");
