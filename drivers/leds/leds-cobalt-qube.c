<<<<<<< HEAD
=======
// SPDX-License-Identifier: GPL-2.0-only
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
/*
 * Copyright 2006 - Florian Fainelli <florian@openwrt.org>
 *
 * Control the Cobalt Qube/RaQ front LED
 */
<<<<<<< HEAD
#include <linux/init.h>
=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#include <linux/io.h>
#include <linux/ioport.h>
#include <linux/leds.h>
#include <linux/module.h>
#include <linux/platform_device.h>
#include <linux/types.h>

#define LED_FRONT_LEFT	0x01
#define LED_FRONT_RIGHT	0x02

static void __iomem *led_port;
static u8 led_value;

static void qube_front_led_set(struct led_classdev *led_cdev,
			       enum led_brightness brightness)
{
	if (brightness)
		led_value = LED_FRONT_LEFT | LED_FRONT_RIGHT;
	else
		led_value = ~(LED_FRONT_LEFT | LED_FRONT_RIGHT);
	writeb(led_value, led_port);
}

static struct led_classdev qube_front_led = {
	.name			= "qube::front",
	.brightness		= LED_FULL,
	.brightness_set		= qube_front_led_set,
	.default_trigger	= "default-on",
};

<<<<<<< HEAD
static int __devinit cobalt_qube_led_probe(struct platform_device *pdev)
{
	struct resource *res;
	int retval;
=======
static int cobalt_qube_led_probe(struct platform_device *pdev)
{
	struct resource *res;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	res = platform_get_resource(pdev, IORESOURCE_MEM, 0);
	if (!res)
		return -EBUSY;

<<<<<<< HEAD
	led_port = ioremap(res->start, resource_size(res));
=======
	led_port = devm_ioremap(&pdev->dev, res->start, resource_size(res));
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	if (!led_port)
		return -ENOMEM;

	led_value = LED_FRONT_LEFT | LED_FRONT_RIGHT;
	writeb(led_value, led_port);

<<<<<<< HEAD
	retval = led_classdev_register(&pdev->dev, &qube_front_led);
	if (retval)
		goto err_iounmap;

	return 0;

err_iounmap:
	iounmap(led_port);
	led_port = NULL;

	return retval;
}

static int __devexit cobalt_qube_led_remove(struct platform_device *pdev)
{
	led_classdev_unregister(&qube_front_led);

	if (led_port) {
		iounmap(led_port);
		led_port = NULL;
	}

	return 0;
=======
	return devm_led_classdev_register(&pdev->dev, &qube_front_led);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

static struct platform_driver cobalt_qube_led_driver = {
	.probe	= cobalt_qube_led_probe,
<<<<<<< HEAD
	.remove	= __devexit_p(cobalt_qube_led_remove),
	.driver	= {
		.name	= "cobalt-qube-leds",
		.owner	= THIS_MODULE,
=======
	.driver	= {
		.name	= "cobalt-qube-leds",
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	},
};

module_platform_driver(cobalt_qube_led_driver);

MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("Front LED support for Cobalt Server");
MODULE_AUTHOR("Florian Fainelli <florian@openwrt.org>");
MODULE_ALIAS("platform:cobalt-qube-leds");
