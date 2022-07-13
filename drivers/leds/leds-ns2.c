<<<<<<< HEAD
=======
// SPDX-License-Identifier: GPL-2.0-or-later
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
/*
 * leds-ns2.c - Driver for the Network Space v2 (and parents) dual-GPIO LED
 *
 * Copyright (C) 2010 LaCie
 *
 * Author: Simon Guinot <sguinot@lacie.com>
 *
 * Based on leds-gpio.c by Raphael Assenat <raph@8d.com>
<<<<<<< HEAD
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */

#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/platform_device.h>
#include <linux/slab.h>
#include <linux/gpio.h>
#include <linux/leds.h>
#include <linux/module.h>
#include <mach/leds-ns2.h>

/*
 * The Network Space v2 dual-GPIO LED is wired to a CPLD and can blink in
 * relation with the SATA activity. This capability is exposed through the
 * "sata" sysfs attribute.
 *
 * The following array detail the different LED registers and the combination
 * of their possible values:
 *
 *  cmd_led   |  slow_led  | /SATA active | LED state
 *            |            |              |
 *     1      |     0      |      x       |  off
 *     -      |     1      |      x       |  on
 *     0      |     0      |      1       |  on
 *     0      |     0      |      0       |  blink (rate 300ms)
 */
=======
 */

#include <linux/kernel.h>
#include <linux/platform_device.h>
#include <linux/slab.h>
#include <linux/gpio/consumer.h>
#include <linux/leds.h>
#include <linux/module.h>
#include <linux/of.h>
#include "leds.h"
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

enum ns2_led_modes {
	NS_V2_LED_OFF,
	NS_V2_LED_ON,
	NS_V2_LED_SATA,
};

<<<<<<< HEAD
struct ns2_led_mode_value {
	enum ns2_led_modes	mode;
	int			cmd_level;
	int			slow_level;
};

static struct ns2_led_mode_value ns2_led_modval[] = {
	{ NS_V2_LED_OFF	, 1, 0 },
	{ NS_V2_LED_ON	, 0, 1 },
	{ NS_V2_LED_ON	, 1, 1 },
	{ NS_V2_LED_SATA, 0, 0 },
};

struct ns2_led_data {
	struct led_classdev	cdev;
	unsigned		cmd;
	unsigned		slow;
	unsigned char		sata; /* True when SATA mode active. */
	rwlock_t		rw_lock; /* Lock GPIOs. */
};

static int ns2_led_get_mode(struct ns2_led_data *led_dat,
			    enum ns2_led_modes *mode)
{
	int i;
	int ret = -EINVAL;
	int cmd_level;
	int slow_level;

	read_lock_irq(&led_dat->rw_lock);

	cmd_level = gpio_get_value(led_dat->cmd);
	slow_level = gpio_get_value(led_dat->slow);

	for (i = 0; i < ARRAY_SIZE(ns2_led_modval); i++) {
		if (cmd_level == ns2_led_modval[i].cmd_level &&
		    slow_level == ns2_led_modval[i].slow_level) {
			*mode = ns2_led_modval[i].mode;
			ret = 0;
			break;
		}
	}

	read_unlock_irq(&led_dat->rw_lock);

	return ret;
}

static void ns2_led_set_mode(struct ns2_led_data *led_dat,
			     enum ns2_led_modes mode)
=======
/*
 * If the size of this structure or types of its members is changed,
 * the filling of array modval in function ns2_led_register must be changed
 * accordingly.
 */
struct ns2_led_modval {
	u32			mode;
	u32			cmd_level;
	u32			slow_level;
} __packed;

/*
 * The Network Space v2 dual-GPIO LED is wired to a CPLD. Three different LED
 * modes are available: off, on and SATA activity blinking. The LED modes are
 * controlled through two GPIOs (command and slow): each combination of values
 * for the command/slow GPIOs corresponds to a LED mode.
 */

struct ns2_led {
	struct led_classdev	cdev;
	struct gpio_desc	*cmd;
	struct gpio_desc	*slow;
	bool			can_sleep;
	unsigned char		sata; /* True when SATA mode active. */
	rwlock_t		rw_lock; /* Lock GPIOs. */
	int			num_modes;
	struct ns2_led_modval	*modval;
};

static int ns2_led_get_mode(struct ns2_led *led, enum ns2_led_modes *mode)
{
	int i;
	int cmd_level;
	int slow_level;

	cmd_level = gpiod_get_value_cansleep(led->cmd);
	slow_level = gpiod_get_value_cansleep(led->slow);

	for (i = 0; i < led->num_modes; i++) {
		if (cmd_level == led->modval[i].cmd_level &&
		    slow_level == led->modval[i].slow_level) {
			*mode = led->modval[i].mode;
			return 0;
		}
	}

	return -EINVAL;
}

static void ns2_led_set_mode(struct ns2_led *led, enum ns2_led_modes mode)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
{
	int i;
	unsigned long flags;

<<<<<<< HEAD
	write_lock_irqsave(&led_dat->rw_lock, flags);

	for (i = 0; i < ARRAY_SIZE(ns2_led_modval); i++) {
		if (mode == ns2_led_modval[i].mode) {
			gpio_set_value(led_dat->cmd,
				       ns2_led_modval[i].cmd_level);
			gpio_set_value(led_dat->slow,
				       ns2_led_modval[i].slow_level);
		}
	}

	write_unlock_irqrestore(&led_dat->rw_lock, flags);
=======
	for (i = 0; i < led->num_modes; i++)
		if (mode == led->modval[i].mode)
			break;

	if (i == led->num_modes)
		return;

	write_lock_irqsave(&led->rw_lock, flags);

	if (!led->can_sleep) {
		gpiod_set_value(led->cmd, led->modval[i].cmd_level);
		gpiod_set_value(led->slow, led->modval[i].slow_level);
		goto exit_unlock;
	}

	gpiod_set_value_cansleep(led->cmd, led->modval[i].cmd_level);
	gpiod_set_value_cansleep(led->slow, led->modval[i].slow_level);

exit_unlock:
	write_unlock_irqrestore(&led->rw_lock, flags);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

static void ns2_led_set(struct led_classdev *led_cdev,
			enum led_brightness value)
{
<<<<<<< HEAD
	struct ns2_led_data *led_dat =
		container_of(led_cdev, struct ns2_led_data, cdev);
=======
	struct ns2_led *led = container_of(led_cdev, struct ns2_led, cdev);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	enum ns2_led_modes mode;

	if (value == LED_OFF)
		mode = NS_V2_LED_OFF;
<<<<<<< HEAD
	else if (led_dat->sata)
=======
	else if (led->sata)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		mode = NS_V2_LED_SATA;
	else
		mode = NS_V2_LED_ON;

<<<<<<< HEAD
	ns2_led_set_mode(led_dat, mode);
=======
	ns2_led_set_mode(led, mode);
}

static int ns2_led_set_blocking(struct led_classdev *led_cdev,
			enum led_brightness value)
{
	ns2_led_set(led_cdev, value);
	return 0;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

static ssize_t ns2_led_sata_store(struct device *dev,
				  struct device_attribute *attr,
				  const char *buff, size_t count)
{
	struct led_classdev *led_cdev = dev_get_drvdata(dev);
<<<<<<< HEAD
	struct ns2_led_data *led_dat =
		container_of(led_cdev, struct ns2_led_data, cdev);
	int ret;
	unsigned long enable;
	enum ns2_led_modes mode;

	ret = strict_strtoul(buff, 10, &enable);
=======
	struct ns2_led *led = container_of(led_cdev, struct ns2_led, cdev);
	int ret;
	unsigned long enable;

	ret = kstrtoul(buff, 10, &enable);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	if (ret < 0)
		return ret;

	enable = !!enable;

<<<<<<< HEAD
	if (led_dat->sata == enable)
		return count;

	ret = ns2_led_get_mode(led_dat, &mode);
	if (ret < 0)
		return ret;

	if (enable && mode == NS_V2_LED_ON)
		ns2_led_set_mode(led_dat, NS_V2_LED_SATA);
	if (!enable && mode == NS_V2_LED_SATA)
		ns2_led_set_mode(led_dat, NS_V2_LED_ON);

	led_dat->sata = enable;

=======
	if (led->sata == enable)
		goto exit;

	led->sata = enable;

	if (!led_get_brightness(led_cdev))
		goto exit;

	if (enable)
		ns2_led_set_mode(led, NS_V2_LED_SATA);
	else
		ns2_led_set_mode(led, NS_V2_LED_ON);

exit:
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	return count;
}

static ssize_t ns2_led_sata_show(struct device *dev,
				 struct device_attribute *attr, char *buf)
{
	struct led_classdev *led_cdev = dev_get_drvdata(dev);
<<<<<<< HEAD
	struct ns2_led_data *led_dat =
		container_of(led_cdev, struct ns2_led_data, cdev);

	return sprintf(buf, "%d\n", led_dat->sata);
=======
	struct ns2_led *led = container_of(led_cdev, struct ns2_led, cdev);

	return sprintf(buf, "%d\n", led->sata);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

static DEVICE_ATTR(sata, 0644, ns2_led_sata_show, ns2_led_sata_store);

<<<<<<< HEAD
static int __devinit
create_ns2_led(struct platform_device *pdev, struct ns2_led_data *led_dat,
	       const struct ns2_led *template)
{
	int ret;
	enum ns2_led_modes mode;

	ret = gpio_request(template->cmd, template->name);
	if (ret == 0) {
		ret = gpio_direction_output(template->cmd,
					    gpio_get_value(template->cmd));
		if (ret)
			gpio_free(template->cmd);
	}
	if (ret) {
		dev_err(&pdev->dev, "%s: failed to setup command GPIO\n",
			template->name);
	}

	ret = gpio_request(template->slow, template->name);
	if (ret == 0) {
		ret = gpio_direction_output(template->slow,
					    gpio_get_value(template->slow));
		if (ret)
			gpio_free(template->slow);
	}
	if (ret) {
		dev_err(&pdev->dev, "%s: failed to setup slow GPIO\n",
			template->name);
		goto err_free_cmd;
	}

	rwlock_init(&led_dat->rw_lock);

	led_dat->cdev.name = template->name;
	led_dat->cdev.default_trigger = template->default_trigger;
	led_dat->cdev.blink_set = NULL;
	led_dat->cdev.brightness_set = ns2_led_set;
	led_dat->cdev.flags |= LED_CORE_SUSPENDRESUME;
	led_dat->cmd = template->cmd;
	led_dat->slow = template->slow;

	ret = ns2_led_get_mode(led_dat, &mode);
	if (ret < 0)
		goto err_free_slow;

	/* Set LED initial state. */
	led_dat->sata = (mode == NS_V2_LED_SATA) ? 1 : 0;
	led_dat->cdev.brightness =
		(mode == NS_V2_LED_OFF) ? LED_OFF : LED_FULL;

	ret = led_classdev_register(&pdev->dev, &led_dat->cdev);
	if (ret < 0)
		goto err_free_slow;

	ret = device_create_file(led_dat->cdev.dev, &dev_attr_sata);
	if (ret < 0)
		goto err_free_cdev;

	return 0;

err_free_cdev:
	led_classdev_unregister(&led_dat->cdev);
err_free_slow:
	gpio_free(led_dat->slow);
err_free_cmd:
	gpio_free(led_dat->cmd);

	return ret;
}

static void delete_ns2_led(struct ns2_led_data *led_dat)
{
	device_remove_file(led_dat->cdev.dev, &dev_attr_sata);
	led_classdev_unregister(&led_dat->cdev);
	gpio_free(led_dat->cmd);
	gpio_free(led_dat->slow);
}

static int __devinit ns2_led_probe(struct platform_device *pdev)
{
	struct ns2_led_platform_data *pdata = pdev->dev.platform_data;
	struct ns2_led_data *leds_data;
	int i;
	int ret;

	if (!pdata)
		return -EINVAL;

	leds_data = kzalloc(sizeof(struct ns2_led_data) *
			    pdata->num_leds, GFP_KERNEL);
	if (!leds_data)
		return -ENOMEM;

	for (i = 0; i < pdata->num_leds; i++) {
		ret = create_ns2_led(pdev, &leds_data[i], &pdata->leds[i]);
		if (ret < 0)
			goto err;

	}

	platform_set_drvdata(pdev, leds_data);

	return 0;

err:
	for (i = i - 1; i >= 0; i--)
		delete_ns2_led(&leds_data[i]);

	kfree(leds_data);
=======
static struct attribute *ns2_led_attrs[] = {
	&dev_attr_sata.attr,
	NULL
};
ATTRIBUTE_GROUPS(ns2_led);

static int ns2_led_register(struct device *dev, struct fwnode_handle *node,
			    struct ns2_led *led)
{
	struct led_init_data init_data = {};
	struct ns2_led_modval *modval;
	enum ns2_led_modes mode;
	int nmodes, ret;

	led->cmd = devm_fwnode_gpiod_get_index(dev, node, "cmd", 0, GPIOD_ASIS,
					       fwnode_get_name(node));
	if (IS_ERR(led->cmd))
		return PTR_ERR(led->cmd);

	led->slow = devm_fwnode_gpiod_get_index(dev, node, "slow", 0,
						GPIOD_ASIS,
						fwnode_get_name(node));
	if (IS_ERR(led->slow))
		return PTR_ERR(led->slow);

	ret = fwnode_property_count_u32(node, "modes-map");
	if (ret < 0 || ret % 3) {
		dev_err(dev, "Missing or malformed modes-map for %pfw\n", node);
		return -EINVAL;
	}

	nmodes = ret / 3;
	modval = devm_kcalloc(dev, nmodes, sizeof(*modval), GFP_KERNEL);
	if (!modval)
		return -ENOMEM;

	fwnode_property_read_u32_array(node, "modes-map", (void *)modval,
				       nmodes * 3);

	rwlock_init(&led->rw_lock);

	led->cdev.blink_set = NULL;
	led->cdev.flags |= LED_CORE_SUSPENDRESUME;
	led->cdev.groups = ns2_led_groups;
	led->can_sleep = gpiod_cansleep(led->cmd) || gpiod_cansleep(led->slow);
	if (led->can_sleep)
		led->cdev.brightness_set_blocking = ns2_led_set_blocking;
	else
		led->cdev.brightness_set = ns2_led_set;
	led->num_modes = nmodes;
	led->modval = modval;

	ret = ns2_led_get_mode(led, &mode);
	if (ret < 0)
		return ret;

	/* Set LED initial state. */
	led->sata = (mode == NS_V2_LED_SATA) ? 1 : 0;
	led->cdev.brightness = (mode == NS_V2_LED_OFF) ? LED_OFF : LED_FULL;

	init_data.fwnode = node;

	ret = devm_led_classdev_register_ext(dev, &led->cdev, &init_data);
	if (ret)
		dev_err(dev, "Failed to register LED for node %pfw\n", node);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	return ret;
}

<<<<<<< HEAD
static int __devexit ns2_led_remove(struct platform_device *pdev)
{
	int i;
	struct ns2_led_platform_data *pdata = pdev->dev.platform_data;
	struct ns2_led_data *leds_data;

	leds_data = platform_get_drvdata(pdev);

	for (i = 0; i < pdata->num_leds; i++)
		delete_ns2_led(&leds_data[i]);

	kfree(leds_data);
	platform_set_drvdata(pdev, NULL);
=======
static int ns2_led_probe(struct platform_device *pdev)
{
	struct device *dev = &pdev->dev;
	struct fwnode_handle *child;
	struct ns2_led *leds;
	int count;
	int ret;

	count = device_get_child_node_count(dev);
	if (!count)
		return -ENODEV;

	leds = devm_kcalloc(dev, count, sizeof(*leds), GFP_KERNEL);
	if (!leds)
		return -ENOMEM;

	device_for_each_child_node(dev, child) {
		ret = ns2_led_register(dev, child, leds++);
		if (ret) {
			fwnode_handle_put(child);
			return ret;
		}
	}
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	return 0;
}

<<<<<<< HEAD
static struct platform_driver ns2_led_driver = {
	.probe		= ns2_led_probe,
	.remove		= __devexit_p(ns2_led_remove),
	.driver		= {
		.name	= "leds-ns2",
		.owner	= THIS_MODULE,
=======
static const struct of_device_id of_ns2_leds_match[] = {
	{ .compatible = "lacie,ns2-leds", },
	{},
};
MODULE_DEVICE_TABLE(of, of_ns2_leds_match);

static struct platform_driver ns2_led_driver = {
	.probe		= ns2_led_probe,
	.driver		= {
		.name		= "leds-ns2",
		.of_match_table	= of_ns2_leds_match,
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	},
};

module_platform_driver(ns2_led_driver);

MODULE_AUTHOR("Simon Guinot <sguinot@lacie.com>");
MODULE_DESCRIPTION("Network Space v2 LED driver");
MODULE_LICENSE("GPL");
MODULE_ALIAS("platform:leds-ns2");
