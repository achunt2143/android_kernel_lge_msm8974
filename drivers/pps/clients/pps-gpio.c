<<<<<<< HEAD
/*
 * pps-gpio.c -- PPS client driver using GPIO
 *
 *
 * Copyright (C) 2010 Ricardo Martins <rasm@fe.up.pt>
 * Copyright (C) 2011 James Nuss <jamesnuss@nanometrics.ca>
 *
 *   This program is free software; you can redistribute it and/or modify
 *   it under the terms of the GNU General Public License as published by
 *   the Free Software Foundation; either version 2 of the License, or
 *   (at your option) any later version.
 *
 *   This program is distributed in the hope that it will be useful,
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *   GNU General Public License for more details.
 *
 *   You should have received a copy of the GNU General Public License
 *   along with this program; if not, write to the Free Software
 *   Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
=======
// SPDX-License-Identifier: GPL-2.0-or-later
/*
 * pps-gpio.c -- PPS client driver using GPIO
 *
 * Copyright (C) 2010 Ricardo Martins <rasm@fe.up.pt>
 * Copyright (C) 2011 James Nuss <jamesnuss@nanometrics.ca>
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 */

#define PPS_GPIO_NAME "pps-gpio"
#define pr_fmt(fmt) PPS_GPIO_NAME ": " fmt

#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/interrupt.h>
<<<<<<< HEAD
=======
#include <linux/mod_devicetable.h>
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#include <linux/module.h>
#include <linux/platform_device.h>
#include <linux/slab.h>
#include <linux/pps_kernel.h>
<<<<<<< HEAD
#include <linux/pps-gpio.h>
#include <linux/gpio.h>
#include <linux/list.h>
=======
#include <linux/gpio/consumer.h>
#include <linux/list.h>
#include <linux/property.h>
#include <linux/timer.h>
#include <linux/jiffies.h>
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

/* Info for each registered platform device */
struct pps_gpio_device_data {
	int irq;			/* IRQ used as PPS source */
	struct pps_device *pps;		/* PPS source device */
	struct pps_source_info info;	/* PPS source information */
<<<<<<< HEAD
	const struct pps_gpio_platform_data *pdata;
=======
	struct gpio_desc *gpio_pin;	/* GPIO port descriptors */
	struct gpio_desc *echo_pin;
	struct timer_list echo_timer;	/* timer to reset echo active state */
	bool assert_falling_edge;
	bool capture_clear;
	unsigned int echo_active_ms;	/* PPS echo active duration */
	unsigned long echo_timeout;	/* timer timeout value in jiffies */
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
};

/*
 * Report the PPS event
 */

static irqreturn_t pps_gpio_irq_handler(int irq, void *data)
{
	const struct pps_gpio_device_data *info;
	struct pps_event_time ts;
	int rising_edge;

	/* Get the time stamp first */
	pps_get_ts(&ts);

	info = data;

<<<<<<< HEAD
	rising_edge = gpio_get_value(info->pdata->gpio_pin);
	if ((rising_edge && !info->pdata->assert_falling_edge) ||
			(!rising_edge && info->pdata->assert_falling_edge))
		pps_event(info->pps, &ts, PPS_CAPTUREASSERT, NULL);
	else if (info->pdata->capture_clear &&
			((rising_edge && info->pdata->assert_falling_edge) ||
			 (!rising_edge && !info->pdata->assert_falling_edge)))
		pps_event(info->pps, &ts, PPS_CAPTURECLEAR, NULL);
=======
	rising_edge = gpiod_get_value(info->gpio_pin);
	if ((rising_edge && !info->assert_falling_edge) ||
			(!rising_edge && info->assert_falling_edge))
		pps_event(info->pps, &ts, PPS_CAPTUREASSERT, data);
	else if (info->capture_clear &&
			((rising_edge && info->assert_falling_edge) ||
			(!rising_edge && !info->assert_falling_edge)))
		pps_event(info->pps, &ts, PPS_CAPTURECLEAR, data);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	return IRQ_HANDLED;
}

<<<<<<< HEAD
static int pps_gpio_setup(struct platform_device *pdev)
{
	int ret;
	const struct pps_gpio_platform_data *pdata = pdev->dev.platform_data;

	ret = gpio_request(pdata->gpio_pin, pdata->gpio_label);
	if (ret) {
		pr_warning("failed to request GPIO %u\n", pdata->gpio_pin);
		return -EINVAL;
	}

	ret = gpio_direction_input(pdata->gpio_pin);
	if (ret) {
		pr_warning("failed to set pin direction\n");
		gpio_free(pdata->gpio_pin);
		return -EINVAL;
	}
=======
/* This function will only be called when an ECHO GPIO is defined */
static void pps_gpio_echo(struct pps_device *pps, int event, void *data)
{
	/* add_timer() needs to write into info->echo_timer */
	struct pps_gpio_device_data *info = data;

	switch (event) {
	case PPS_CAPTUREASSERT:
		if (pps->params.mode & PPS_ECHOASSERT)
			gpiod_set_value(info->echo_pin, 1);
		break;

	case PPS_CAPTURECLEAR:
		if (pps->params.mode & PPS_ECHOCLEAR)
			gpiod_set_value(info->echo_pin, 1);
		break;
	}

	/* fire the timer */
	if (info->pps->params.mode & (PPS_ECHOASSERT | PPS_ECHOCLEAR)) {
		info->echo_timer.expires = jiffies + info->echo_timeout;
		add_timer(&info->echo_timer);
	}
}

/* Timer callback to reset the echo pin to the inactive state */
static void pps_gpio_echo_timer_callback(struct timer_list *t)
{
	const struct pps_gpio_device_data *info;

	info = from_timer(info, t, echo_timer);

	gpiod_set_value(info->echo_pin, 0);
}

static int pps_gpio_setup(struct device *dev)
{
	struct pps_gpio_device_data *data = dev_get_drvdata(dev);
	int ret;
	u32 value;

	data->gpio_pin = devm_gpiod_get(dev, NULL, GPIOD_IN);
	if (IS_ERR(data->gpio_pin))
		return dev_err_probe(dev, PTR_ERR(data->gpio_pin),
				     "failed to request PPS GPIO\n");

	data->assert_falling_edge =
		device_property_read_bool(dev, "assert-falling-edge");

	data->echo_pin = devm_gpiod_get_optional(dev, "echo", GPIOD_OUT_LOW);
	if (IS_ERR(data->echo_pin))
		return dev_err_probe(dev, PTR_ERR(data->echo_pin),
				     "failed to request ECHO GPIO\n");

	if (!data->echo_pin)
		return 0;

	ret = device_property_read_u32(dev, "echo-active-ms", &value);
	if (ret) {
		dev_err(dev, "failed to get echo-active-ms from FW\n");
		return ret;
	}

	/* sanity check on echo_active_ms */
	if (!value || value > 999) {
		dev_err(dev, "echo-active-ms: %u - bad value from FW\n", value);
		return -EINVAL;
	}

	data->echo_active_ms = value;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	return 0;
}

static unsigned long
<<<<<<< HEAD
get_irqf_trigger_flags(const struct pps_gpio_platform_data *pdata)
{
	unsigned long flags = pdata->assert_falling_edge ?
		IRQF_TRIGGER_FALLING : IRQF_TRIGGER_RISING;

	if (pdata->capture_clear) {
=======
get_irqf_trigger_flags(const struct pps_gpio_device_data *data)
{
	unsigned long flags = data->assert_falling_edge ?
		IRQF_TRIGGER_FALLING : IRQF_TRIGGER_RISING;

	if (data->capture_clear) {
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		flags |= ((flags & IRQF_TRIGGER_RISING) ?
				IRQF_TRIGGER_FALLING : IRQF_TRIGGER_RISING);
	}

	return flags;
}

static int pps_gpio_probe(struct platform_device *pdev)
{
	struct pps_gpio_device_data *data;
<<<<<<< HEAD
	int irq;
	int ret;
	int err;
	int pps_default_params;
	const struct pps_gpio_platform_data *pdata = pdev->dev.platform_data;


	/* GPIO setup */
	ret = pps_gpio_setup(pdev);
	if (ret)
		return -EINVAL;

	/* IRQ setup */
	irq = gpio_to_irq(pdata->gpio_pin);
	if (irq < 0) {
		pr_err("failed to map GPIO to IRQ: %d\n", irq);
		err = -EINVAL;
		goto return_error;
	}

	/* allocate space for device info */
	data = kzalloc(sizeof(struct pps_gpio_device_data), GFP_KERNEL);
	if (data == NULL) {
		err = -ENOMEM;
		goto return_error;
	}
=======
	struct device *dev = &pdev->dev;
	int ret;
	int pps_default_params;

	/* allocate space for device info */
	data = devm_kzalloc(dev, sizeof(*data), GFP_KERNEL);
	if (!data)
		return -ENOMEM;

	dev_set_drvdata(dev, data);

	/* GPIO setup */
	ret = pps_gpio_setup(dev);
	if (ret)
		return ret;

	/* IRQ setup */
	ret = gpiod_to_irq(data->gpio_pin);
	if (ret < 0) {
		dev_err(dev, "failed to map GPIO to IRQ: %d\n", ret);
		return -EINVAL;
	}
	data->irq = ret;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	/* initialize PPS specific parts of the bookkeeping data structure. */
	data->info.mode = PPS_CAPTUREASSERT | PPS_OFFSETASSERT |
		PPS_ECHOASSERT | PPS_CANWAIT | PPS_TSFMT_TSPEC;
<<<<<<< HEAD
	if (pdata->capture_clear)
=======
	if (data->capture_clear)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		data->info.mode |= PPS_CAPTURECLEAR | PPS_OFFSETCLEAR |
			PPS_ECHOCLEAR;
	data->info.owner = THIS_MODULE;
	snprintf(data->info.name, PPS_MAX_NAME_LEN - 1, "%s.%d",
		 pdev->name, pdev->id);
<<<<<<< HEAD

	/* register PPS source */
	pps_default_params = PPS_CAPTUREASSERT | PPS_OFFSETASSERT;
	if (pdata->capture_clear)
		pps_default_params |= PPS_CAPTURECLEAR | PPS_OFFSETCLEAR;
	data->pps = pps_register_source(&data->info, pps_default_params);
	if (data->pps == NULL) {
		kfree(data);
		pr_err("failed to register IRQ %d as PPS source\n", irq);
		err = -EINVAL;
		goto return_error;
	}

	data->irq = irq;
	data->pdata = pdata;

	/* register IRQ interrupt handler */
	ret = request_irq(irq, pps_gpio_irq_handler,
			get_irqf_trigger_flags(pdata), data->info.name, data);
	if (ret) {
		pps_unregister_source(data->pps);
		kfree(data);
		pr_err("failed to acquire IRQ %d\n", irq);
		err = -EINVAL;
		goto return_error;
	}

	platform_set_drvdata(pdev, data);
	dev_info(data->pps->dev, "Registered IRQ %d as PPS source\n", irq);

	return 0;

return_error:
	gpio_free(pdata->gpio_pin);
	return err;
=======
	if (data->echo_pin) {
		data->info.echo = pps_gpio_echo;
		data->echo_timeout = msecs_to_jiffies(data->echo_active_ms);
		timer_setup(&data->echo_timer, pps_gpio_echo_timer_callback, 0);
	}

	/* register PPS source */
	pps_default_params = PPS_CAPTUREASSERT | PPS_OFFSETASSERT;
	if (data->capture_clear)
		pps_default_params |= PPS_CAPTURECLEAR | PPS_OFFSETCLEAR;
	data->pps = pps_register_source(&data->info, pps_default_params);
	if (IS_ERR(data->pps)) {
		dev_err(dev, "failed to register IRQ %d as PPS source\n",
			data->irq);
		return PTR_ERR(data->pps);
	}

	/* register IRQ interrupt handler */
	ret = devm_request_irq(dev, data->irq, pps_gpio_irq_handler,
			get_irqf_trigger_flags(data), data->info.name, data);
	if (ret) {
		pps_unregister_source(data->pps);
		dev_err(dev, "failed to acquire IRQ %d\n", data->irq);
		return -EINVAL;
	}

	dev_info(data->pps->dev, "Registered IRQ %d as PPS source\n",
		 data->irq);

	return 0;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

static int pps_gpio_remove(struct platform_device *pdev)
{
	struct pps_gpio_device_data *data = platform_get_drvdata(pdev);
<<<<<<< HEAD
	const struct pps_gpio_platform_data *pdata = data->pdata;

	platform_set_drvdata(pdev, NULL);
	free_irq(data->irq, data);
	gpio_free(pdata->gpio_pin);
	pps_unregister_source(data->pps);
	pr_info("removed IRQ %d as PPS source\n", data->irq);
	kfree(data);
	return 0;
}

static struct platform_driver pps_gpio_driver = {
	.probe		= pps_gpio_probe,
	.remove		=  __devexit_p(pps_gpio_remove),
	.driver		= {
		.name	= PPS_GPIO_NAME,
		.owner	= THIS_MODULE
	},
};

static int __init pps_gpio_init(void)
{
	int ret = platform_driver_register(&pps_gpio_driver);
	if (ret < 0)
		pr_err("failed to register platform driver\n");
	return ret;
}

static void __exit pps_gpio_exit(void)
{
	platform_driver_unregister(&pps_gpio_driver);
	pr_debug("unregistered platform driver\n");
}

module_init(pps_gpio_init);
module_exit(pps_gpio_exit);

=======

	pps_unregister_source(data->pps);
	del_timer_sync(&data->echo_timer);
	/* reset echo pin in any case */
	gpiod_set_value(data->echo_pin, 0);
	dev_info(&pdev->dev, "removed IRQ %d as PPS source\n", data->irq);
	return 0;
}

static const struct of_device_id pps_gpio_dt_ids[] = {
	{ .compatible = "pps-gpio", },
	{ /* sentinel */ }
};
MODULE_DEVICE_TABLE(of, pps_gpio_dt_ids);

static struct platform_driver pps_gpio_driver = {
	.probe		= pps_gpio_probe,
	.remove		= pps_gpio_remove,
	.driver		= {
		.name	= PPS_GPIO_NAME,
		.of_match_table	= pps_gpio_dt_ids,
	},
};

module_platform_driver(pps_gpio_driver);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
MODULE_AUTHOR("Ricardo Martins <rasm@fe.up.pt>");
MODULE_AUTHOR("James Nuss <jamesnuss@nanometrics.ca>");
MODULE_DESCRIPTION("Use GPIO pin as PPS source");
MODULE_LICENSE("GPL");
<<<<<<< HEAD
MODULE_VERSION("1.0.0");
=======
MODULE_VERSION("1.2.0");
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
