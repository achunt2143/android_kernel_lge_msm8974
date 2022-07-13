<<<<<<< HEAD
=======
// SPDX-License-Identifier: GPL-2.0-only
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
/*
 * LEDs driver for Dialog Semiconductor DA9030/DA9034
 *
 * Copyright (C) 2008 Compulab, Ltd.
<<<<<<< HEAD
 * 	Mike Rapoport <mike@compulab.co.il>
 *
 * Copyright (C) 2006-2008 Marvell International Ltd.
 * 	Eric Miao <eric.miao@marvell.com>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
=======
 *	Mike Rapoport <mike@compulab.co.il>
 *
 * Copyright (C) 2006-2008 Marvell International Ltd.
 *	Eric Miao <eric.miao@marvell.com>
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 */

#include <linux/module.h>
#include <linux/kernel.h>
<<<<<<< HEAD
#include <linux/init.h>
#include <linux/platform_device.h>
#include <linux/leds.h>
#include <linux/workqueue.h>
=======
#include <linux/platform_device.h>
#include <linux/leds.h>
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#include <linux/mfd/da903x.h>
#include <linux/slab.h>

#define DA9030_LED1_CONTROL	0x20
#define DA9030_LED2_CONTROL	0x21
#define DA9030_LED3_CONTROL	0x22
#define DA9030_LED4_CONTROL	0x23
#define DA9030_LEDPC_CONTROL	0x24
#define DA9030_MISC_CONTROL_A	0x26	/* Vibrator Control */

#define DA9034_LED1_CONTROL	0x35
#define DA9034_LED2_CONTROL	0x36
#define DA9034_VIBRA		0x40

struct da903x_led {
	struct led_classdev	cdev;
<<<<<<< HEAD
	struct work_struct	work;
	struct device		*master;
	enum led_brightness	new_brightness;
=======
	struct device		*master;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	int			id;
	int			flags;
};

#define DA9030_LED_OFFSET(id)	((id) - DA9030_ID_LED_1)
#define DA9034_LED_OFFSET(id)	((id) - DA9034_ID_LED_1)

<<<<<<< HEAD
static void da903x_led_work(struct work_struct *work)
{
	struct da903x_led *led = container_of(work, struct da903x_led, work);
	uint8_t val;
	int offset;
=======
static int da903x_led_set(struct led_classdev *led_cdev,
			   enum led_brightness value)
{
	struct da903x_led *led =
			container_of(led_cdev, struct da903x_led, cdev);
	uint8_t val;
	int offset, ret = -EINVAL;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	switch (led->id) {
	case DA9030_ID_LED_1:
	case DA9030_ID_LED_2:
	case DA9030_ID_LED_3:
	case DA9030_ID_LED_4:
	case DA9030_ID_LED_PC:
		offset = DA9030_LED_OFFSET(led->id);
		val = led->flags & ~0x87;
<<<<<<< HEAD
		val |= (led->new_brightness) ? 0x80 : 0; /* EN bit */
		val |= (0x7 - (led->new_brightness >> 5)) & 0x7; /* PWM<2:0> */
		da903x_write(led->master, DA9030_LED1_CONTROL + offset, val);
		break;
	case DA9030_ID_VIBRA:
		val = led->flags & ~0x80;
		val |= (led->new_brightness) ? 0x80 : 0; /* EN bit */
		da903x_write(led->master, DA9030_MISC_CONTROL_A, val);
=======
		val |= value ? 0x80 : 0; /* EN bit */
		val |= (0x7 - (value >> 5)) & 0x7; /* PWM<2:0> */
		ret = da903x_write(led->master, DA9030_LED1_CONTROL + offset,
				   val);
		break;
	case DA9030_ID_VIBRA:
		val = led->flags & ~0x80;
		val |= value ? 0x80 : 0; /* EN bit */
		ret = da903x_write(led->master, DA9030_MISC_CONTROL_A, val);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		break;
	case DA9034_ID_LED_1:
	case DA9034_ID_LED_2:
		offset = DA9034_LED_OFFSET(led->id);
<<<<<<< HEAD
		val = (led->new_brightness * 0x5f / LED_FULL) & 0x7f;
		val |= (led->flags & DA9034_LED_RAMP) ? 0x80 : 0;
		da903x_write(led->master, DA9034_LED1_CONTROL + offset, val);
		break;
	case DA9034_ID_VIBRA:
		val = led->new_brightness & 0xfe;
		da903x_write(led->master, DA9034_VIBRA, val);
		break;
	}
}

static void da903x_led_set(struct led_classdev *led_cdev,
			   enum led_brightness value)
{
	struct da903x_led *led;
	
	led = container_of(led_cdev, struct da903x_led, cdev);
	led->new_brightness = value;
	schedule_work(&led->work);
}

static int __devinit da903x_led_probe(struct platform_device *pdev)
{
	struct led_info *pdata = pdev->dev.platform_data;
=======
		val = (value * 0x5f / LED_FULL) & 0x7f;
		val |= (led->flags & DA9034_LED_RAMP) ? 0x80 : 0;
		ret = da903x_write(led->master, DA9034_LED1_CONTROL + offset,
				   val);
		break;
	case DA9034_ID_VIBRA:
		val = value & 0xfe;
		ret = da903x_write(led->master, DA9034_VIBRA, val);
		break;
	}

	return ret;
}

static int da903x_led_probe(struct platform_device *pdev)
{
	struct led_info *pdata = dev_get_platdata(&pdev->dev);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	struct da903x_led *led;
	int id, ret;

	if (pdata == NULL)
		return 0;

	id = pdev->id;

	if (!((id >= DA9030_ID_LED_1 && id <= DA9030_ID_VIBRA) ||
	      (id >= DA9034_ID_LED_1 && id <= DA9034_ID_VIBRA))) {
		dev_err(&pdev->dev, "invalid LED ID (%d) specified\n", id);
		return -EINVAL;
	}

<<<<<<< HEAD
	led = kzalloc(sizeof(struct da903x_led), GFP_KERNEL);
	if (led == NULL) {
		dev_err(&pdev->dev, "failed to alloc memory for LED%d\n", id);
		return -ENOMEM;
	}

	led->cdev.name = pdata->name;
	led->cdev.default_trigger = pdata->default_trigger;
	led->cdev.brightness_set = da903x_led_set;
=======
	led = devm_kzalloc(&pdev->dev, sizeof(struct da903x_led), GFP_KERNEL);
	if (!led)
		return -ENOMEM;

	led->cdev.name = pdata->name;
	led->cdev.default_trigger = pdata->default_trigger;
	led->cdev.brightness_set_blocking = da903x_led_set;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	led->cdev.brightness = LED_OFF;

	led->id = id;
	led->flags = pdata->flags;
	led->master = pdev->dev.parent;
<<<<<<< HEAD
	led->new_brightness = LED_OFF;

	INIT_WORK(&led->work, da903x_led_work);
=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	ret = led_classdev_register(led->master, &led->cdev);
	if (ret) {
		dev_err(&pdev->dev, "failed to register LED %d\n", id);
<<<<<<< HEAD
		goto err;
	}

	platform_set_drvdata(pdev, led);
	return 0;

err:
	kfree(led);
	return ret;
}

static int __devexit da903x_led_remove(struct platform_device *pdev)
=======
		return ret;
	}

	platform_set_drvdata(pdev, led);

	return 0;
}

static void da903x_led_remove(struct platform_device *pdev)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
{
	struct da903x_led *led = platform_get_drvdata(pdev);

	led_classdev_unregister(&led->cdev);
<<<<<<< HEAD
	kfree(led);
	return 0;
=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

static struct platform_driver da903x_led_driver = {
	.driver	= {
		.name	= "da903x-led",
<<<<<<< HEAD
		.owner	= THIS_MODULE,
	},
	.probe		= da903x_led_probe,
	.remove		= __devexit_p(da903x_led_remove),
=======
	},
	.probe		= da903x_led_probe,
	.remove_new	= da903x_led_remove,
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
};

module_platform_driver(da903x_led_driver);

MODULE_DESCRIPTION("LEDs driver for Dialog Semiconductor DA9030/DA9034");
<<<<<<< HEAD
MODULE_AUTHOR("Eric Miao <eric.miao@marvell.com>"
	      "Mike Rapoport <mike@compulab.co.il>");
=======
MODULE_AUTHOR("Eric Miao <eric.miao@marvell.com>");
MODULE_AUTHOR("Mike Rapoport <mike@compulab.co.il>");
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
MODULE_LICENSE("GPL");
MODULE_ALIAS("platform:da903x-led");
