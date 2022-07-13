<<<<<<< HEAD
=======
// SPDX-License-Identifier: GPL-2.0-only
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
/*
 * LED Class Core
 *
 * Copyright (C) 2005 John Lenz <lenz@cs.wisc.edu>
 * Copyright (C) 2005-2007 Richard Purdie <rpurdie@openedhand.com>
<<<<<<< HEAD
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 */

#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/list.h>
#include <linux/spinlock.h>
#include <linux/device.h>
#include <linux/timer.h>
#include <linux/err.h>
#include <linux/ctype.h>
#include <linux/power_supply.h>
#include <linux/leds.h>
#include "leds.h"
#if defined(CONFIG_MACH_LGE)
#include <mach/board_lge.h>
#endif
#if defined(CONFIG_LEDS_WINDOW_COLOR)
#include <linux/string.h>
#endif

#define LED_BUFF_SIZE 50

static struct class *leds_class;
#ifdef CONFIG_LEDS_PM8941_EMOTIONAL
extern void make_blink_led_pattern(int rgb, int delay_on, int delay_off);
extern void make_onoff_led_pattern(int rgb);
extern void change_led_pattern(int pattern);
extern void make_input_led_pattern(int patterns[],
			int red_start, int red_length, int red_duty,
			int red_pause, int green_start, int green_length,
			int green_duty,	int green_pause, int blue_start,
			int blue_length, int blue_duty, int blue_pause,
			int red_flag, int green_flag, int blue_flag,
			int period);
extern void set_kpdbl_pattern (int pattern);
static int onoff_rgb;
#endif

#if defined(CONFIG_LEDS_KEY_REAR)
extern void make_rear_blink_led_pattern(int delay_on, int delay_off);
#endif

#if defined(CONFIG_LEDS_WINDOW_COLOR)
enum WINDOW_COLORS window_color;
unsigned char win_color[] = "com.lge.systemui.theme.xxxxxx";
#endif

static void led_update_brightness(struct led_classdev *led_cdev)
{
	if (led_cdev->brightness_get)
		led_cdev->brightness = led_cdev->brightness_get(led_cdev);
}

static ssize_t led_brightness_show(struct device *dev,
=======
 */

#include <linux/ctype.h>
#include <linux/device.h>
#include <linux/err.h>
#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/leds.h>
#include <linux/list.h>
#include <linux/module.h>
#include <linux/property.h>
#include <linux/slab.h>
#include <linux/spinlock.h>
#include <linux/timer.h>
#include <uapi/linux/uleds.h>
#include <linux/of.h>
#include "leds.h"

static DEFINE_MUTEX(leds_lookup_lock);
static LIST_HEAD(leds_lookup_list);

static ssize_t brightness_show(struct device *dev,
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		struct device_attribute *attr, char *buf)
{
	struct led_classdev *led_cdev = dev_get_drvdata(dev);

	/* no lock needed for this */
	led_update_brightness(led_cdev);

<<<<<<< HEAD
	return snprintf(buf, LED_BUFF_SIZE, "%u\n", led_cdev->brightness);
}

static ssize_t led_brightness_store(struct device *dev,
		struct device_attribute *attr, const char *buf, size_t size)
{
	struct led_classdev *led_cdev = dev_get_drvdata(dev);
	ssize_t ret = -EINVAL;
	char *after;
	unsigned long state = simple_strtoul(buf, &after, 10);
	size_t count = after - buf;

	if (isspace(*after))
		count++;

	if (count == size) {
		ret = count;

		if (state == LED_OFF)
			led_trigger_remove(led_cdev);
		led_set_brightness(led_cdev, state);
	}

	return ret;
}

static ssize_t led_max_brightness_store(struct device *dev,
		struct device_attribute *attr, const char *buf, size_t size)
{
	struct led_classdev *led_cdev = dev_get_drvdata(dev);
	ssize_t ret = -EINVAL;
	unsigned long state = 0;

	ret = strict_strtoul(buf, 10, &state);
	if (!ret) {
		ret = size;
		if (state > LED_FULL)
			state = LED_FULL;
		led_cdev->max_brightness = state;
		led_set_brightness(led_cdev, led_cdev->brightness);
	}

	return ret;
}

static ssize_t led_max_brightness_show(struct device *dev,
=======
	return sprintf(buf, "%u\n", led_cdev->brightness);
}

static ssize_t brightness_store(struct device *dev,
		struct device_attribute *attr, const char *buf, size_t size)
{
	struct led_classdev *led_cdev = dev_get_drvdata(dev);
	unsigned long state;
	ssize_t ret;

	mutex_lock(&led_cdev->led_access);

	if (led_sysfs_is_disabled(led_cdev)) {
		ret = -EBUSY;
		goto unlock;
	}

	ret = kstrtoul(buf, 10, &state);
	if (ret)
		goto unlock;

	if (state == LED_OFF)
		led_trigger_remove(led_cdev);
	led_set_brightness(led_cdev, state);
	flush_work(&led_cdev->set_brightness_work);

	ret = size;
unlock:
	mutex_unlock(&led_cdev->led_access);
	return ret;
}
static DEVICE_ATTR_RW(brightness);

static ssize_t max_brightness_show(struct device *dev,
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		struct device_attribute *attr, char *buf)
{
	struct led_classdev *led_cdev = dev_get_drvdata(dev);

<<<<<<< HEAD
	return snprintf(buf, LED_BUFF_SIZE, "%u\n", led_cdev->max_brightness);
}

#if defined(CONFIG_MACH_MSM8974_G3_VZW) || defined(CONFIG_MACH_MSM8974_G3_LRA) \
    || defined(CONFIG_MACH_MSM8974_G2_VZW)
static int lge_thm_status;
static ssize_t thermald_status_show(struct device *dev,
		struct device_attribute *attr, char *buf)
{
	return sprintf(buf, "%d\n", lge_thm_status);
}

static ssize_t thermald_status_store(struct device *dev,
		struct device_attribute *attr, const char *buf, size_t size)
{
	struct led_classdev *led_cdev = dev_get_drvdata(dev);
	unsigned long state = 0;
	int rc = 1;

	if (strncmp(buf, "0", 1) == 0)
		lge_thm_status = 0;
	else if (strncmp(buf, "1", 1) == 0) {
		state = LED_FULL;
		led_cdev->max_brightness = state;
		led_set_brightness(led_cdev, led_cdev->brightness);

		lge_thm_status = 1;
	}
	return rc;
}
#endif

static struct device_attribute led_class_attrs[] = {
	__ATTR(brightness, 0644, led_brightness_show, led_brightness_store),
	__ATTR(max_brightness, 0644, led_max_brightness_show,
			led_max_brightness_store),
#if defined(CONFIG_MACH_MSM8974_G3_VZW) || defined(CONFIG_MACH_MSM8974_G3_LRA) \
    || defined(CONFIG_MACH_MSM8974_G2_VZW)
	__ATTR(thermald_status, 0644, thermald_status_show, thermald_status_store),
#endif
#ifdef CONFIG_LEDS_TRIGGERS
	__ATTR(trigger, 0644, led_trigger_show, led_trigger_store),
#endif
	__ATTR_NULL,
};

static void led_timer_function(unsigned long data)
{
	struct led_classdev *led_cdev = (void *)data;
	unsigned long brightness;
	unsigned long delay;

	if (!led_cdev->blink_delay_on || !led_cdev->blink_delay_off) {
		led_set_brightness(led_cdev, LED_OFF);
		return;
	}

	brightness = led_get_brightness(led_cdev);
	if (!brightness) {
		/* Time to switch the LED on. */
		brightness = led_cdev->blink_brightness;
		delay = led_cdev->blink_delay_on;
	} else {
		/* Store the current brightness value to be able
		 * to restore it when the delay_off period is over.
		 */
		led_cdev->blink_brightness = brightness;
		brightness = LED_OFF;
		delay = led_cdev->blink_delay_off;
	}

	led_set_brightness(led_cdev, brightness);

	mod_timer(&led_cdev->blink_timer, jiffies + msecs_to_jiffies(delay));
}

=======
	return sprintf(buf, "%u\n", led_cdev->max_brightness);
}
static DEVICE_ATTR_RO(max_brightness);

#ifdef CONFIG_LEDS_TRIGGERS
static BIN_ATTR(trigger, 0644, led_trigger_read, led_trigger_write, 0);
static struct bin_attribute *led_trigger_bin_attrs[] = {
	&bin_attr_trigger,
	NULL,
};
static const struct attribute_group led_trigger_group = {
	.bin_attrs = led_trigger_bin_attrs,
};
#endif

static struct attribute *led_class_attrs[] = {
	&dev_attr_brightness.attr,
	&dev_attr_max_brightness.attr,
	NULL,
};

static const struct attribute_group led_group = {
	.attrs = led_class_attrs,
};

static const struct attribute_group *led_groups[] = {
	&led_group,
#ifdef CONFIG_LEDS_TRIGGERS
	&led_trigger_group,
#endif
	NULL,
};

#ifdef CONFIG_LEDS_BRIGHTNESS_HW_CHANGED
static ssize_t brightness_hw_changed_show(struct device *dev,
		struct device_attribute *attr, char *buf)
{
	struct led_classdev *led_cdev = dev_get_drvdata(dev);

	if (led_cdev->brightness_hw_changed == -1)
		return -ENODATA;

	return sprintf(buf, "%u\n", led_cdev->brightness_hw_changed);
}

static DEVICE_ATTR_RO(brightness_hw_changed);

static int led_add_brightness_hw_changed(struct led_classdev *led_cdev)
{
	struct device *dev = led_cdev->dev;
	int ret;

	ret = device_create_file(dev, &dev_attr_brightness_hw_changed);
	if (ret) {
		dev_err(dev, "Error creating brightness_hw_changed\n");
		return ret;
	}

	led_cdev->brightness_hw_changed_kn =
		sysfs_get_dirent(dev->kobj.sd, "brightness_hw_changed");
	if (!led_cdev->brightness_hw_changed_kn) {
		dev_err(dev, "Error getting brightness_hw_changed kn\n");
		device_remove_file(dev, &dev_attr_brightness_hw_changed);
		return -ENXIO;
	}

	return 0;
}

static void led_remove_brightness_hw_changed(struct led_classdev *led_cdev)
{
	sysfs_put(led_cdev->brightness_hw_changed_kn);
	device_remove_file(led_cdev->dev, &dev_attr_brightness_hw_changed);
}

void led_classdev_notify_brightness_hw_changed(struct led_classdev *led_cdev, unsigned int brightness)
{
	if (WARN_ON(!led_cdev->brightness_hw_changed_kn))
		return;

	led_cdev->brightness_hw_changed = brightness;
	sysfs_notify_dirent(led_cdev->brightness_hw_changed_kn);
}
EXPORT_SYMBOL_GPL(led_classdev_notify_brightness_hw_changed);
#else
static int led_add_brightness_hw_changed(struct led_classdev *led_cdev)
{
	return 0;
}
static void led_remove_brightness_hw_changed(struct led_classdev *led_cdev)
{
}
#endif

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
/**
 * led_classdev_suspend - suspend an led_classdev.
 * @led_cdev: the led_classdev to suspend.
 */
void led_classdev_suspend(struct led_classdev *led_cdev)
{
	led_cdev->flags |= LED_SUSPENDED;
<<<<<<< HEAD
	led_cdev->brightness_set(led_cdev, 0);
=======
	led_set_brightness_nopm(led_cdev, 0);
	flush_work(&led_cdev->set_brightness_work);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}
EXPORT_SYMBOL_GPL(led_classdev_suspend);

/**
 * led_classdev_resume - resume an led_classdev.
 * @led_cdev: the led_classdev to resume.
 */
void led_classdev_resume(struct led_classdev *led_cdev)
{
<<<<<<< HEAD
	led_cdev->brightness_set(led_cdev, led_cdev->brightness);
=======
	led_set_brightness_nopm(led_cdev, led_cdev->brightness);

	if (led_cdev->flash_resume)
		led_cdev->flash_resume(led_cdev);

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	led_cdev->flags &= ~LED_SUSPENDED;
}
EXPORT_SYMBOL_GPL(led_classdev_resume);

<<<<<<< HEAD
static int led_suspend(struct device *dev, pm_message_t state)
=======
#ifdef CONFIG_PM_SLEEP
static int led_suspend(struct device *dev)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
{
	struct led_classdev *led_cdev = dev_get_drvdata(dev);

	if (led_cdev->flags & LED_CORE_SUSPENDRESUME)
		led_classdev_suspend(led_cdev);

	return 0;
}

static int led_resume(struct device *dev)
{
	struct led_classdev *led_cdev = dev_get_drvdata(dev);

	if (led_cdev->flags & LED_CORE_SUSPENDRESUME)
		led_classdev_resume(led_cdev);

	return 0;
}
<<<<<<< HEAD

/**
 * led_classdev_register - register a new object of led_classdev class.
 * @parent: The device to register.
 * @led_cdev: the led_classdev structure for this device.
 */
int led_classdev_register(struct device *parent, struct led_classdev *led_cdev)
{
	led_cdev->dev = device_create(leds_class, parent, 0, led_cdev,
				      "%s", led_cdev->name);
	if (IS_ERR(led_cdev->dev))
		return PTR_ERR(led_cdev->dev);

#ifdef CONFIG_LEDS_TRIGGERS
	init_rwsem(&led_cdev->trigger_lock);
#endif
=======
#endif

static SIMPLE_DEV_PM_OPS(leds_class_dev_pm_ops, led_suspend, led_resume);

static struct led_classdev *led_module_get(struct device *led_dev)
{
	struct led_classdev *led_cdev;

	if (!led_dev)
		return ERR_PTR(-EPROBE_DEFER);

	led_cdev = dev_get_drvdata(led_dev);

	if (!try_module_get(led_cdev->dev->parent->driver->owner)) {
		put_device(led_cdev->dev);
		return ERR_PTR(-ENODEV);
	}

	return led_cdev;
}

static const struct class leds_class = {
	.name = "leds",
	.dev_groups = led_groups,
	.pm = &leds_class_dev_pm_ops,
};

/**
 * of_led_get() - request a LED device via the LED framework
 * @np: device node to get the LED device from
 * @index: the index of the LED
 *
 * Returns the LED device parsed from the phandle specified in the "leds"
 * property of a device tree node or a negative error-code on failure.
 */
struct led_classdev *of_led_get(struct device_node *np, int index)
{
	struct device *led_dev;
	struct device_node *led_node;

	led_node = of_parse_phandle(np, "leds", index);
	if (!led_node)
		return ERR_PTR(-ENOENT);

	led_dev = class_find_device_by_of_node(&leds_class, led_node);
	of_node_put(led_node);
	put_device(led_dev);

	return led_module_get(led_dev);
}
EXPORT_SYMBOL_GPL(of_led_get);

/**
 * led_put() - release a LED device
 * @led_cdev: LED device
 */
void led_put(struct led_classdev *led_cdev)
{
	module_put(led_cdev->dev->parent->driver->owner);
	put_device(led_cdev->dev);
}
EXPORT_SYMBOL_GPL(led_put);

static void devm_led_release(struct device *dev, void *res)
{
	struct led_classdev **p = res;

	led_put(*p);
}

static struct led_classdev *__devm_led_get(struct device *dev, struct led_classdev *led)
{
	struct led_classdev **dr;

	dr = devres_alloc(devm_led_release, sizeof(struct led_classdev *), GFP_KERNEL);
	if (!dr) {
		led_put(led);
		return ERR_PTR(-ENOMEM);
	}

	*dr = led;
	devres_add(dev, dr);

	return led;
}

/**
 * devm_of_led_get - Resource-managed request of a LED device
 * @dev:	LED consumer
 * @index:	index of the LED to obtain in the consumer
 *
 * The device node of the device is parse to find the request LED device.
 * The LED device returned from this function is automatically released
 * on driver detach.
 *
 * @return a pointer to a LED device or ERR_PTR(errno) on failure.
 */
struct led_classdev *__must_check devm_of_led_get(struct device *dev,
						  int index)
{
	struct led_classdev *led;

	if (!dev)
		return ERR_PTR(-EINVAL);

	led = of_led_get(dev->of_node, index);
	if (IS_ERR(led))
		return led;

	return __devm_led_get(dev, led);
}
EXPORT_SYMBOL_GPL(devm_of_led_get);

/**
 * led_get() - request a LED device via the LED framework
 * @dev: device for which to get the LED device
 * @con_id: name of the LED from the device's point of view
 *
 * @return a pointer to a LED device or ERR_PTR(errno) on failure.
 */
struct led_classdev *led_get(struct device *dev, char *con_id)
{
	struct led_lookup_data *lookup;
	const char *provider = NULL;
	struct device *led_dev;

	mutex_lock(&leds_lookup_lock);
	list_for_each_entry(lookup, &leds_lookup_list, list) {
		if (!strcmp(lookup->dev_id, dev_name(dev)) &&
		    !strcmp(lookup->con_id, con_id)) {
			provider = kstrdup_const(lookup->provider, GFP_KERNEL);
			break;
		}
	}
	mutex_unlock(&leds_lookup_lock);

	if (!provider)
		return ERR_PTR(-ENOENT);

	led_dev = class_find_device_by_name(&leds_class, provider);
	kfree_const(provider);

	return led_module_get(led_dev);
}
EXPORT_SYMBOL_GPL(led_get);

/**
 * devm_led_get() - request a LED device via the LED framework
 * @dev: device for which to get the LED device
 * @con_id: name of the LED from the device's point of view
 *
 * The LED device returned from this function is automatically released
 * on driver detach.
 *
 * @return a pointer to a LED device or ERR_PTR(errno) on failure.
 */
struct led_classdev *devm_led_get(struct device *dev, char *con_id)
{
	struct led_classdev *led;

	led = led_get(dev, con_id);
	if (IS_ERR(led))
		return led;

	return __devm_led_get(dev, led);
}
EXPORT_SYMBOL_GPL(devm_led_get);

/**
 * led_add_lookup() - Add a LED lookup table entry
 * @led_lookup: the lookup table entry to add
 *
 * Add a LED lookup table entry. On systems without devicetree the lookup table
 * is used by led_get() to find LEDs.
 */
void led_add_lookup(struct led_lookup_data *led_lookup)
{
	mutex_lock(&leds_lookup_lock);
	list_add_tail(&led_lookup->list, &leds_lookup_list);
	mutex_unlock(&leds_lookup_lock);
}
EXPORT_SYMBOL_GPL(led_add_lookup);

/**
 * led_remove_lookup() - Remove a LED lookup table entry
 * @led_lookup: the lookup table entry to remove
 */
void led_remove_lookup(struct led_lookup_data *led_lookup)
{
	mutex_lock(&leds_lookup_lock);
	list_del(&led_lookup->list);
	mutex_unlock(&leds_lookup_lock);
}
EXPORT_SYMBOL_GPL(led_remove_lookup);

/**
 * devm_of_led_get_optional - Resource-managed request of an optional LED device
 * @dev:	LED consumer
 * @index:	index of the LED to obtain in the consumer
 *
 * The device node of the device is parsed to find the requested LED device.
 * The LED device returned from this function is automatically released
 * on driver detach.
 *
 * @return a pointer to a LED device, ERR_PTR(errno) on failure and NULL if the
 * led was not found.
 */
struct led_classdev *__must_check devm_of_led_get_optional(struct device *dev,
							int index)
{
	struct led_classdev *led;

	led = devm_of_led_get(dev, index);
	if (IS_ERR(led) && PTR_ERR(led) == -ENOENT)
		return NULL;

	return led;
}
EXPORT_SYMBOL_GPL(devm_of_led_get_optional);

static int led_classdev_next_name(const char *init_name, char *name,
				  size_t len)
{
	unsigned int i = 0;
	int ret = 0;
	struct device *dev;

	strscpy(name, init_name, len);

	while ((ret < len) &&
	       (dev = class_find_device_by_name(&leds_class, name))) {
		put_device(dev);
		ret = snprintf(name, len, "%s_%u", init_name, ++i);
	}

	if (ret >= len)
		return -ENOMEM;

	return i;
}

/**
 * led_classdev_register_ext - register a new object of led_classdev class
 *			       with init data.
 *
 * @parent: parent of LED device
 * @led_cdev: the led_classdev structure for this device.
 * @init_data: LED class device initialization data
 */
int led_classdev_register_ext(struct device *parent,
			      struct led_classdev *led_cdev,
			      struct led_init_data *init_data)
{
	char composed_name[LED_MAX_NAME_SIZE];
	char final_name[LED_MAX_NAME_SIZE];
	const char *proposed_name = composed_name;
	int ret;

	if (init_data) {
		if (init_data->devname_mandatory && !init_data->devicename) {
			dev_err(parent, "Mandatory device name is missing");
			return -EINVAL;
		}
		ret = led_compose_name(parent, init_data, composed_name);
		if (ret < 0)
			return ret;

		if (init_data->fwnode) {
			fwnode_property_read_string(init_data->fwnode,
				"linux,default-trigger",
				&led_cdev->default_trigger);

			if (fwnode_property_present(init_data->fwnode,
						    "retain-state-shutdown"))
				led_cdev->flags |= LED_RETAIN_AT_SHUTDOWN;

			fwnode_property_read_u32(init_data->fwnode,
				"max-brightness",
				&led_cdev->max_brightness);

			if (fwnode_property_present(init_data->fwnode, "color"))
				fwnode_property_read_u32(init_data->fwnode, "color",
							 &led_cdev->color);
		}
	} else {
		proposed_name = led_cdev->name;
	}

	ret = led_classdev_next_name(proposed_name, final_name, sizeof(final_name));
	if (ret < 0)
		return ret;

	if (led_cdev->color >= LED_COLOR_ID_MAX)
		dev_warn(parent, "LED %s color identifier out of range\n", final_name);

	mutex_init(&led_cdev->led_access);
	mutex_lock(&led_cdev->led_access);
	led_cdev->dev = device_create_with_groups(&leds_class, parent, 0,
						  led_cdev, led_cdev->groups, "%s", final_name);
	if (IS_ERR(led_cdev->dev)) {
		mutex_unlock(&led_cdev->led_access);
		return PTR_ERR(led_cdev->dev);
	}
	if (init_data && init_data->fwnode)
		device_set_node(led_cdev->dev, init_data->fwnode);

	if (ret)
		dev_warn(parent, "Led %s renamed to %s due to name collision",
				proposed_name, dev_name(led_cdev->dev));

	if (led_cdev->flags & LED_BRIGHT_HW_CHANGED) {
		ret = led_add_brightness_hw_changed(led_cdev);
		if (ret) {
			device_unregister(led_cdev->dev);
			led_cdev->dev = NULL;
			mutex_unlock(&led_cdev->led_access);
			return ret;
		}
	}

	led_cdev->work_flags = 0;
#ifdef CONFIG_LEDS_TRIGGERS
	init_rwsem(&led_cdev->trigger_lock);
#endif
#ifdef CONFIG_LEDS_BRIGHTNESS_HW_CHANGED
	led_cdev->brightness_hw_changed = -1;
#endif
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	/* add to the list of leds */
	down_write(&leds_list_lock);
	list_add_tail(&led_cdev->node, &leds_list);
	up_write(&leds_list_lock);

	if (!led_cdev->max_brightness)
		led_cdev->max_brightness = LED_FULL;

	led_update_brightness(led_cdev);

<<<<<<< HEAD
	init_timer(&led_cdev->blink_timer);
	led_cdev->blink_timer.function = led_timer_function;
	led_cdev->blink_timer.data = (unsigned long)led_cdev;

#ifdef CONFIG_LEDS_TRIGGERS
	led_trigger_set_default(led_cdev);
#endif

	printk(KERN_DEBUG "Registered led device: %s\n",
=======
	led_init_core(led_cdev);

#ifdef CONFIG_LEDS_TRIGGERS
	/*
	 * If no default trigger was given and hw_control_trigger is set,
	 * make it the default trigger.
	 */
	if (!led_cdev->default_trigger && led_cdev->hw_control_trigger)
		led_cdev->default_trigger = led_cdev->hw_control_trigger;
	led_trigger_set_default(led_cdev);
#endif

	mutex_unlock(&led_cdev->led_access);

	dev_dbg(parent, "Registered led device: %s\n",
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
			led_cdev->name);

	return 0;
}
<<<<<<< HEAD
EXPORT_SYMBOL_GPL(led_classdev_register);

#ifdef CONFIG_LEDS_PM8941_EMOTIONAL
static ssize_t get_pattern(struct device *dev, struct device_attribute *attr, char *buf)
{
	int count = 0;
	/* count = sprintf(buf,"%d %d\n", pattern_num,pattern_on ); */

	return count;
}

static ssize_t set_pattern(struct device *dev, struct device_attribute *attr, const char *buf, size_t size)
{
	ssize_t ret = -EINVAL;
	int pattern_num = 0;

	if (sscanf(buf, "%d", &pattern_num) != 1) {
		printk("[RGB LED] bad arguments ");
	}
	ret = size;

	if (lge_get_boot_mode() <= LGE_BOOT_MODE_CHARGERLOGO) {
		printk("[RGB LED] pattern_num = %d\n", pattern_num);

		if ((pattern_num != 35) && (pattern_num != 36))
			change_led_pattern(pattern_num);
	}

	return ret;
}

static DEVICE_ATTR(setting, 0644, get_pattern, set_pattern);

#if defined(CONFIG_LEDS_WINDOW_COLOR)
static ssize_t get_window_color(struct device *dev, struct device_attribute *attr, char *buf)
{
	return sprintf(buf, " - Window Color is '%s' \n", win_color);
}

static ssize_t set_window_color(struct device *dev, struct device_attribute *attr, const char *buf, size_t size)
{
	ssize_t ret = -EINVAL;
	unsigned char color[30];

	if (sscanf(buf, "%29s", color) != 1) {
		printk("[RGB LED] bad arguments ");
	}
	ret = size;

	printk("[RGB LED] # Window Color [%s] # Set Color [%s]\n", color, win_color);

	memcpy(win_color, color, sizeof(color));

	if (strstr(color, "black") != NULL) {
		window_color = WINDOW_COLOR_BK;
		printk("[RGB LED] window_color is black\n");
	} else if (strstr(color, "white") != NULL) {
		window_color = WINDOW_COLOR_WH;
		printk("[RGB LED] window_color is white\n");
	} else if (strstr(color, "silver") != NULL) {
		window_color = WINDOW_COLOR_SV;
		printk("[RGB LED] window_color is silver\n");
	} else if (strstr(color, "titan") != NULL) {
		window_color = WINDOW_COLOR_TK;
		printk("[RGB LED] window_color is titan\n");
	} else {
		memcpy(win_color, "black", sizeof("black"));
		window_color = WINDOW_COLOR_BK;
		printk("[RGB LED] window_color is default(black)\n");
	}

	return ret;
}

static DEVICE_ATTR(window_color, 0644, get_window_color, set_window_color);
#endif

static ssize_t get_input_pattern(struct device *dev,
						struct device_attribute *attr,
						char *buf)
{
	int count = 0;
	return count;
}

static ssize_t set_input_pattern(struct device *dev,
						struct device_attribute *attr,
						const char *buf, size_t size)
{
	ssize_t ret = -EINVAL;
	int patterns[63];

	int red_start_idx;
	int red_length;
	int red_duty;
	int red_pause_lo;

	int green_start_idx;
	int green_length;
	int green_duty;
	int green_pause_lo;

	int blue_start_idx;
	int blue_length;
	int blue_duty;
	int blue_pause_lo;

	int red_flag;
	int green_flag;
	int blue_flag;
	int period;

	int i = 0;

	if (sscanf(buf, "%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d",
			&patterns[0], &patterns[1], &patterns[2],
			&patterns[3], &patterns[4], &patterns[5],
			&patterns[6], &patterns[7],	&patterns[8],
			&patterns[9], &patterns[10], &patterns[11],
			&patterns[12], &patterns[13], &patterns[14],
			&patterns[15], &patterns[16], &patterns[17],
			&patterns[18], &patterns[19], &patterns[20],
			&patterns[21], &patterns[22], &patterns[23],
			&patterns[24], &patterns[25], &patterns[26],
			&patterns[27], &patterns[28], &patterns[29],
			&patterns[30], &patterns[31], &patterns[32],
			&patterns[33], &patterns[34], &patterns[35],
			&patterns[36], &patterns[37], &patterns[38],
			&patterns[39], &patterns[40], &patterns[41],
			&patterns[42], &patterns[43], &patterns[44],
			&patterns[45], &patterns[46], &patterns[47],
			&patterns[48], &patterns[49], &patterns[50],
			&patterns[51], &patterns[52], &patterns[53],
			&patterns[54], &patterns[55], &patterns[56],
			&patterns[57], &patterns[58], &patterns[59],
			&patterns[60], &patterns[61], &patterns[62],
			&red_start_idx, &red_length, &red_duty,
			&red_pause_lo, &green_start_idx, &green_length,
			&green_duty, &green_pause_lo, &blue_start_idx,
			&blue_length,  &blue_duty,  &blue_pause_lo,
			&red_flag, &green_flag, &blue_flag, &period) != 79){
			printk(KERN_INFO "[RGB LED] bad arguments ");
		}
	ret = size;

	printk("[RGB LED] LUT is \n");
	for (i = 0; i < ARRAY_SIZE(patterns); i++)
		printk(KERN_INFO "%d ", patterns[i]);
	printk("\n");
	printk(KERN_INFO "[RGB LED] [RED] duty_ms:%d, pause_lo:%d, start:%d, length:%d \n",
		   red_duty, red_pause_lo, red_start_idx, red_length);
	printk(KERN_INFO "[RGB LED] [GREEN] duty_ms:%d, pause_lo:%d, start:%d, length:%d \n",
		   green_duty, green_pause_lo, green_start_idx, green_length);
	printk(KERN_INFO "[RGB LED] [BLUE] duty_ms:%d, pause_lo:%d, start:%d, length:%d \n",
		   blue_duty, blue_pause_lo, blue_start_idx, blue_length);
	printk(KERN_INFO "[RGB LED] [FLAG] red:%d, green:%d, blue:%d\n",
		   red_flag, green_flag, blue_flag);
	make_input_led_pattern((int *)&patterns, red_start_idx, red_length,
			red_duty, red_pause_lo, green_start_idx, green_length,
			green_duty, green_pause_lo, blue_start_idx, blue_length,
			blue_duty, blue_pause_lo, red_flag, green_flag,
			blue_flag, period);

	return ret;
}

static DEVICE_ATTR(input_patterns, 0644, get_input_pattern, set_input_pattern);

static ssize_t confirm_blink_pattern(struct device *dev, struct device_attribute *attr, char *buf)
{
	int count = 0;

	/* count = sprintf(buf,"%d %d\n", pattern_num,pattern_on ); */

	return count;
}


static ssize_t make_blink_pattern(struct device *dev, struct device_attribute *attr, const char *buf, size_t size)
{

	ssize_t ret = -EINVAL;
	int rgb;
	int delay_on;
	int delay_off;

	if (sscanf(buf, "0x%06x,%d,%d", &rgb, &delay_on, &delay_off) != 3)
		printk("[RGB LED] make_blink_pattern() bad arguments ");

	ret = size;

	printk("[RGB LED] make_blink_pattern rgb is %06x\n", rgb);
	printk("[RGB LED] make_blink_pattern delay_on is %d\n", delay_on);
	printk("[RGB LED] make_blink_pattern delay_off %d\n", delay_off);
	make_blink_led_pattern(rgb, delay_on, delay_off);

	return ret;
}

static DEVICE_ATTR(blink_patterns, 0644, confirm_blink_pattern, make_blink_pattern);

#if defined(CONFIG_LEDS_KEY_REAR)
static ssize_t get_rear_pattern(struct device *dev, struct device_attribute *attr, char *buf)
{
	return 0;
}

static ssize_t set_rear_pattern(struct device *dev, struct device_attribute *attr, const char *buf, size_t size)
{
	ssize_t ret = -EINVAL;
	int pattern_num = 0;

	if (sscanf(buf, "%d", &pattern_num) != 1) {
		printk("[REAR LED] bad arguments ");
	}
	ret = size;

	if (lge_get_boot_mode() <= LGE_BOOT_MODE_CHARGERLOGO) {
		printk("[REAR LED] pattern_num = %d\n", pattern_num);

		set_kpdbl_pattern(pattern_num);
	}
	return ret;
}

static DEVICE_ATTR(rear_setting, 0644, get_rear_pattern, set_rear_pattern);

static ssize_t confirm_rear_blink_pattern(struct device *dev, struct device_attribute *attr, char *buf)
{
	return 0;
}

static ssize_t make_rear_blink_pattern(struct device *dev, struct device_attribute *attr, const char *buf, size_t size)
{
	ssize_t ret = -EINVAL;
	int rgb;
	int delay_on = 0;
	int delay_off = 0;

	if (sscanf(buf, "0x%06x,%d,%d", &rgb, &delay_on, &delay_off) != 3)
		printk("[RGB LED] make_rear_blink_pattern() bad arguments ");

	ret = size;

	make_rear_blink_led_pattern(delay_on, delay_off);

	return ret;
}

static DEVICE_ATTR(rear_blink_patterns, 0644, confirm_rear_blink_pattern, make_rear_blink_pattern);
#endif

static ssize_t confirm_onoff_pattern(struct device *dev, struct device_attribute *attr, char *buf)
{
	return snprintf(buf, LED_BUFF_SIZE, "0x%06x\n", onoff_rgb);
}

static ssize_t make_onoff_pattern(struct device *dev, struct device_attribute *attr, const char *buf, size_t size)
{

	ssize_t ret = -EINVAL;

	if (sscanf(buf, "0x%06x", &onoff_rgb) != 1)
		printk("[RGB LED] make_onoff_pattern() bad arguments ");

	ret = size;

	/* printk("[RGB LED] make_onoff_rgb is %06x\n",rgb); */
	make_onoff_led_pattern(onoff_rgb);

	return ret;
}

static DEVICE_ATTR(onoff_patterns, 0644, confirm_onoff_pattern, make_onoff_pattern);

int led_pattern_sysfs_register(void)
{
	struct class *lg_rgb;
	struct device *pattern_sysfs_dev;
	lg_rgb = class_create(THIS_MODULE, "lg_rgb_led");
	if (IS_ERR(lg_rgb)) {
		printk("Failed to create class(lg_rgb_led)!\n");
	}
	pattern_sysfs_dev = device_create(lg_rgb, NULL, 0, NULL, "use_patterns");
	if (IS_ERR(pattern_sysfs_dev))
		return PTR_ERR(pattern_sysfs_dev);

	if (device_create_file(pattern_sysfs_dev, &dev_attr_setting) < 0)
		printk("Failed to create device file(%s)!\n", dev_attr_setting.attr.name);

	if (device_create_file(pattern_sysfs_dev, &dev_attr_blink_patterns) < 0)
		printk("Failed to create device file(%s)!\n", dev_attr_blink_patterns.attr.name);

#if defined(CONFIG_LEDS_KEY_REAR)
	if (device_create_file(pattern_sysfs_dev, &dev_attr_rear_setting) < 0)
		printk("Failed to create device file(%s)!\n", dev_attr_rear_setting.attr.name);

	if (device_create_file(pattern_sysfs_dev, &dev_attr_rear_blink_patterns) < 0)
		printk("Failed to create device file(%s)!\n", dev_attr_rear_blink_patterns.attr.name);
#endif

	if (device_create_file(pattern_sysfs_dev, &dev_attr_input_patterns) < 0)
		printk("Failed to create device file(%s)!\n", dev_attr_input_patterns.attr.name);

	if (device_create_file(pattern_sysfs_dev, &dev_attr_onoff_patterns) < 0)
		printk("Failed to create device file(%s)!\n", dev_attr_onoff_patterns.attr.name);

#if defined(CONFIG_LEDS_WINDOW_COLOR)
	if (device_create_file(pattern_sysfs_dev, &dev_attr_window_color) < 0)
		printk("Failed to create device file(%s)!\n", dev_attr_window_color.attr.name);
#endif

	return 0;
}
EXPORT_SYMBOL_GPL(led_pattern_sysfs_register);
#endif
=======
EXPORT_SYMBOL_GPL(led_classdev_register_ext);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

/**
 * led_classdev_unregister - unregisters a object of led_properties class.
 * @led_cdev: the led device to unregister
 *
 * Unregisters a previously registered via led_classdev_register object.
 */
void led_classdev_unregister(struct led_classdev *led_cdev)
{
<<<<<<< HEAD
=======
	if (IS_ERR_OR_NULL(led_cdev->dev))
		return;

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#ifdef CONFIG_LEDS_TRIGGERS
	down_write(&led_cdev->trigger_lock);
	if (led_cdev->trigger)
		led_trigger_set(led_cdev, NULL);
	up_write(&led_cdev->trigger_lock);
#endif

<<<<<<< HEAD
	/* Stop blinking */
	led_brightness_set(led_cdev, LED_OFF);
=======
	led_cdev->flags |= LED_UNREGISTERING;

	/* Stop blinking */
	led_stop_software_blink(led_cdev);

	if (!(led_cdev->flags & LED_RETAIN_AT_SHUTDOWN))
		led_set_brightness(led_cdev, LED_OFF);

	flush_work(&led_cdev->set_brightness_work);

	if (led_cdev->flags & LED_BRIGHT_HW_CHANGED)
		led_remove_brightness_hw_changed(led_cdev);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	device_unregister(led_cdev->dev);

	down_write(&leds_list_lock);
	list_del(&led_cdev->node);
	up_write(&leds_list_lock);
<<<<<<< HEAD
}
EXPORT_SYMBOL_GPL(led_classdev_unregister);

static int __init leds_init(void)
{
	leds_class = class_create(THIS_MODULE, "leds");
	if (IS_ERR(leds_class))
		return PTR_ERR(leds_class);
	leds_class->suspend = led_suspend;
	leds_class->resume = led_resume;
	leds_class->dev_attrs = led_class_attrs;
#ifdef CONFIG_LEDS_PM8941_EMOTIONAL
	led_pattern_sysfs_register();
#endif
	return 0;
=======

	mutex_destroy(&led_cdev->led_access);
}
EXPORT_SYMBOL_GPL(led_classdev_unregister);

static void devm_led_classdev_release(struct device *dev, void *res)
{
	led_classdev_unregister(*(struct led_classdev **)res);
}

/**
 * devm_led_classdev_register_ext - resource managed led_classdev_register_ext()
 *
 * @parent: parent of LED device
 * @led_cdev: the led_classdev structure for this device.
 * @init_data: LED class device initialization data
 */
int devm_led_classdev_register_ext(struct device *parent,
				   struct led_classdev *led_cdev,
				   struct led_init_data *init_data)
{
	struct led_classdev **dr;
	int rc;

	dr = devres_alloc(devm_led_classdev_release, sizeof(*dr), GFP_KERNEL);
	if (!dr)
		return -ENOMEM;

	rc = led_classdev_register_ext(parent, led_cdev, init_data);
	if (rc) {
		devres_free(dr);
		return rc;
	}

	*dr = led_cdev;
	devres_add(parent, dr);

	return 0;
}
EXPORT_SYMBOL_GPL(devm_led_classdev_register_ext);

static int devm_led_classdev_match(struct device *dev, void *res, void *data)
{
	struct led_classdev **p = res;

	if (WARN_ON(!p || !*p))
		return 0;

	return *p == data;
}

/**
 * devm_led_classdev_unregister() - resource managed led_classdev_unregister()
 * @dev: The device to unregister.
 * @led_cdev: the led_classdev structure for this device.
 */
void devm_led_classdev_unregister(struct device *dev,
				  struct led_classdev *led_cdev)
{
	WARN_ON(devres_release(dev,
			       devm_led_classdev_release,
			       devm_led_classdev_match, led_cdev));
}
EXPORT_SYMBOL_GPL(devm_led_classdev_unregister);

static int __init leds_init(void)
{
	return class_register(&leds_class);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

static void __exit leds_exit(void)
{
<<<<<<< HEAD
	class_destroy(leds_class);
=======
	class_unregister(&leds_class);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

subsys_initcall(leds_init);
module_exit(leds_exit);

MODULE_AUTHOR("John Lenz, Richard Purdie");
MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("LED Class Interface");
