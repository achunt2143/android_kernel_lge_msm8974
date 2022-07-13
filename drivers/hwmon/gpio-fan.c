<<<<<<< HEAD
=======
// SPDX-License-Identifier: GPL-2.0-or-later
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
/*
 * gpio-fan.c - Hwmon driver for fans connected to GPIO lines.
 *
 * Copyright (C) 2010 LaCie
 *
 * Author: Simon Guinot <sguinot@lacie.com>
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
=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 */

#include <linux/module.h>
#include <linux/init.h>
#include <linux/slab.h>
#include <linux/interrupt.h>
#include <linux/irq.h>
#include <linux/platform_device.h>
#include <linux/err.h>
<<<<<<< HEAD
#include <linux/mutex.h>
#include <linux/hwmon.h>
#include <linux/gpio.h>
#include <linux/gpio-fan.h>

struct gpio_fan_data {
	struct platform_device	*pdev;
	struct device		*hwmon_dev;
	struct mutex		lock; /* lock GPIOs operations. */
	int			num_ctrl;
	unsigned		*ctrl;
	int			num_speed;
	struct gpio_fan_speed	*speed;
	int			speed_index;
#ifdef CONFIG_PM
	int			resume_speed;
#endif
	bool			pwm_enable;
	struct gpio_fan_alarm	*alarm;
=======
#include <linux/kstrtox.h>
#include <linux/mutex.h>
#include <linux/hwmon.h>
#include <linux/gpio/consumer.h>
#include <linux/of.h>
#include <linux/of_platform.h>
#include <linux/thermal.h>

struct gpio_fan_speed {
	int rpm;
	int ctrl_val;
};

struct gpio_fan_data {
	struct device		*dev;
	struct device		*hwmon_dev;
	/* Cooling device if any */
	struct thermal_cooling_device *cdev;
	struct mutex		lock; /* lock GPIOs operations. */
	int			num_gpios;
	struct gpio_desc	**gpios;
	int			num_speed;
	struct gpio_fan_speed	*speed;
	int			speed_index;
	int			resume_speed;
	bool			pwm_enable;
	struct gpio_desc	*alarm_gpio;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	struct work_struct	alarm_work;
};

/*
 * Alarm GPIO.
 */

static void fan_alarm_notify(struct work_struct *ws)
{
	struct gpio_fan_data *fan_data =
		container_of(ws, struct gpio_fan_data, alarm_work);

<<<<<<< HEAD
	sysfs_notify(&fan_data->pdev->dev.kobj, NULL, "fan1_alarm");
	kobject_uevent(&fan_data->pdev->dev.kobj, KOBJ_CHANGE);
=======
	sysfs_notify(&fan_data->hwmon_dev->kobj, NULL, "fan1_alarm");
	kobject_uevent(&fan_data->hwmon_dev->kobj, KOBJ_CHANGE);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

static irqreturn_t fan_alarm_irq_handler(int irq, void *dev_id)
{
	struct gpio_fan_data *fan_data = dev_id;

	schedule_work(&fan_data->alarm_work);

	return IRQ_NONE;
}

<<<<<<< HEAD
static ssize_t show_fan_alarm(struct device *dev,
			      struct device_attribute *attr, char *buf)
{
	struct gpio_fan_data *fan_data = dev_get_drvdata(dev);
	struct gpio_fan_alarm *alarm = fan_data->alarm;
	int value = gpio_get_value(alarm->gpio);

	if (alarm->active_low)
		value = !value;

	return sprintf(buf, "%d\n", value);
}

static DEVICE_ATTR(fan1_alarm, S_IRUGO, show_fan_alarm, NULL);

static int fan_alarm_init(struct gpio_fan_data *fan_data,
			  struct gpio_fan_alarm *alarm)
{
	int err;
	int alarm_irq;
	struct platform_device *pdev = fan_data->pdev;

	fan_data->alarm = alarm;

	err = gpio_request(alarm->gpio, "GPIO fan alarm");
	if (err)
		return err;

	err = gpio_direction_input(alarm->gpio);
	if (err)
		goto err_free_gpio;

	err = device_create_file(&pdev->dev, &dev_attr_fan1_alarm);
	if (err)
		goto err_free_gpio;
=======
static ssize_t fan1_alarm_show(struct device *dev,
			       struct device_attribute *attr, char *buf)
{
	struct gpio_fan_data *fan_data = dev_get_drvdata(dev);

	return sprintf(buf, "%d\n",
		       gpiod_get_value_cansleep(fan_data->alarm_gpio));
}

static DEVICE_ATTR_RO(fan1_alarm);

static int fan_alarm_init(struct gpio_fan_data *fan_data)
{
	int alarm_irq;
	struct device *dev = fan_data->dev;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	/*
	 * If the alarm GPIO don't support interrupts, just leave
	 * without initializing the fail notification support.
	 */
<<<<<<< HEAD
	alarm_irq = gpio_to_irq(alarm->gpio);
	if (alarm_irq < 0)
=======
	alarm_irq = gpiod_to_irq(fan_data->alarm_gpio);
	if (alarm_irq <= 0)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		return 0;

	INIT_WORK(&fan_data->alarm_work, fan_alarm_notify);
	irq_set_irq_type(alarm_irq, IRQ_TYPE_EDGE_BOTH);
<<<<<<< HEAD
	err = request_irq(alarm_irq, fan_alarm_irq_handler, IRQF_SHARED,
			  "GPIO fan alarm", fan_data);
	if (err)
		goto err_free_sysfs;

	return 0;

err_free_sysfs:
	device_remove_file(&pdev->dev, &dev_attr_fan1_alarm);
err_free_gpio:
	gpio_free(alarm->gpio);

	return err;
}

static void fan_alarm_free(struct gpio_fan_data *fan_data)
{
	struct platform_device *pdev = fan_data->pdev;
	int alarm_irq = gpio_to_irq(fan_data->alarm->gpio);

	if (alarm_irq >= 0)
		free_irq(alarm_irq, fan_data);
	device_remove_file(&pdev->dev, &dev_attr_fan1_alarm);
	gpio_free(fan_data->alarm->gpio);
=======
	return devm_request_irq(dev, alarm_irq, fan_alarm_irq_handler,
				IRQF_SHARED, "GPIO fan alarm", fan_data);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

/*
 * Control GPIOs.
 */

/* Must be called with fan_data->lock held, except during initialization. */
static void __set_fan_ctrl(struct gpio_fan_data *fan_data, int ctrl_val)
{
	int i;

<<<<<<< HEAD
	for (i = 0; i < fan_data->num_ctrl; i++)
		gpio_set_value(fan_data->ctrl[i], (ctrl_val >> i) & 1);
=======
	for (i = 0; i < fan_data->num_gpios; i++)
		gpiod_set_value_cansleep(fan_data->gpios[i],
					 (ctrl_val >> i) & 1);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

static int __get_fan_ctrl(struct gpio_fan_data *fan_data)
{
	int i;
	int ctrl_val = 0;

<<<<<<< HEAD
	for (i = 0; i < fan_data->num_ctrl; i++) {
		int value;

		value = gpio_get_value(fan_data->ctrl[i]);
=======
	for (i = 0; i < fan_data->num_gpios; i++) {
		int value;

		value = gpiod_get_value_cansleep(fan_data->gpios[i]);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		ctrl_val |= (value << i);
	}
	return ctrl_val;
}

/* Must be called with fan_data->lock held, except during initialization. */
static void set_fan_speed(struct gpio_fan_data *fan_data, int speed_index)
{
	if (fan_data->speed_index == speed_index)
		return;

	__set_fan_ctrl(fan_data, fan_data->speed[speed_index].ctrl_val);
	fan_data->speed_index = speed_index;
}

static int get_fan_speed_index(struct gpio_fan_data *fan_data)
{
	int ctrl_val = __get_fan_ctrl(fan_data);
	int i;

	for (i = 0; i < fan_data->num_speed; i++)
		if (fan_data->speed[i].ctrl_val == ctrl_val)
			return i;

<<<<<<< HEAD
	dev_warn(&fan_data->pdev->dev,
		 "missing speed array entry for GPIO value 0x%x\n", ctrl_val);

	return -EINVAL;
}

static int rpm_to_speed_index(struct gpio_fan_data *fan_data, int rpm)
=======
	dev_warn(fan_data->dev,
		 "missing speed array entry for GPIO value 0x%x\n", ctrl_val);

	return -ENODEV;
}

static int rpm_to_speed_index(struct gpio_fan_data *fan_data, unsigned long rpm)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
{
	struct gpio_fan_speed *speed = fan_data->speed;
	int i;

	for (i = 0; i < fan_data->num_speed; i++)
		if (speed[i].rpm >= rpm)
			return i;

	return fan_data->num_speed - 1;
}

<<<<<<< HEAD
static ssize_t show_pwm(struct device *dev,
			struct device_attribute *attr, char *buf)
=======
static ssize_t pwm1_show(struct device *dev, struct device_attribute *attr,
			 char *buf)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
{
	struct gpio_fan_data *fan_data = dev_get_drvdata(dev);
	u8 pwm = fan_data->speed_index * 255 / (fan_data->num_speed - 1);

	return sprintf(buf, "%d\n", pwm);
}

<<<<<<< HEAD
static ssize_t set_pwm(struct device *dev, struct device_attribute *attr,
		       const char *buf, size_t count)
=======
static ssize_t pwm1_store(struct device *dev, struct device_attribute *attr,
			  const char *buf, size_t count)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
{
	struct gpio_fan_data *fan_data = dev_get_drvdata(dev);
	unsigned long pwm;
	int speed_index;
	int ret = count;

	if (kstrtoul(buf, 10, &pwm) || pwm > 255)
		return -EINVAL;

	mutex_lock(&fan_data->lock);

	if (!fan_data->pwm_enable) {
		ret = -EPERM;
		goto exit_unlock;
	}

	speed_index = DIV_ROUND_UP(pwm * (fan_data->num_speed - 1), 255);
	set_fan_speed(fan_data, speed_index);

exit_unlock:
	mutex_unlock(&fan_data->lock);

	return ret;
}

<<<<<<< HEAD
static ssize_t show_pwm_enable(struct device *dev,
			       struct device_attribute *attr, char *buf)
=======
static ssize_t pwm1_enable_show(struct device *dev,
				struct device_attribute *attr, char *buf)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
{
	struct gpio_fan_data *fan_data = dev_get_drvdata(dev);

	return sprintf(buf, "%d\n", fan_data->pwm_enable);
}

<<<<<<< HEAD
static ssize_t set_pwm_enable(struct device *dev, struct device_attribute *attr,
			      const char *buf, size_t count)
=======
static ssize_t pwm1_enable_store(struct device *dev,
				 struct device_attribute *attr,
				 const char *buf, size_t count)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
{
	struct gpio_fan_data *fan_data = dev_get_drvdata(dev);
	unsigned long val;

	if (kstrtoul(buf, 10, &val) || val > 1)
		return -EINVAL;

	if (fan_data->pwm_enable == val)
		return count;

	mutex_lock(&fan_data->lock);

	fan_data->pwm_enable = val;

	/* Disable manual control mode: set fan at full speed. */
	if (val == 0)
		set_fan_speed(fan_data, fan_data->num_speed - 1);

	mutex_unlock(&fan_data->lock);

	return count;
}

<<<<<<< HEAD
static ssize_t show_pwm_mode(struct device *dev,
			     struct device_attribute *attr, char *buf)
=======
static ssize_t pwm1_mode_show(struct device *dev,
			      struct device_attribute *attr, char *buf)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
{
	return sprintf(buf, "0\n");
}

<<<<<<< HEAD
static ssize_t show_rpm_min(struct device *dev,
			    struct device_attribute *attr, char *buf)
=======
static ssize_t fan1_min_show(struct device *dev,
			     struct device_attribute *attr, char *buf)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
{
	struct gpio_fan_data *fan_data = dev_get_drvdata(dev);

	return sprintf(buf, "%d\n", fan_data->speed[0].rpm);
}

<<<<<<< HEAD
static ssize_t show_rpm_max(struct device *dev,
			    struct device_attribute *attr, char *buf)
=======
static ssize_t fan1_max_show(struct device *dev,
			     struct device_attribute *attr, char *buf)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
{
	struct gpio_fan_data *fan_data = dev_get_drvdata(dev);

	return sprintf(buf, "%d\n",
		       fan_data->speed[fan_data->num_speed - 1].rpm);
}

<<<<<<< HEAD
static ssize_t show_rpm(struct device *dev,
			struct device_attribute *attr, char *buf)
=======
static ssize_t fan1_input_show(struct device *dev,
			       struct device_attribute *attr, char *buf)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
{
	struct gpio_fan_data *fan_data = dev_get_drvdata(dev);

	return sprintf(buf, "%d\n", fan_data->speed[fan_data->speed_index].rpm);
}

static ssize_t set_rpm(struct device *dev, struct device_attribute *attr,
		       const char *buf, size_t count)
{
	struct gpio_fan_data *fan_data = dev_get_drvdata(dev);
	unsigned long rpm;
	int ret = count;

	if (kstrtoul(buf, 10, &rpm))
		return -EINVAL;

	mutex_lock(&fan_data->lock);

	if (!fan_data->pwm_enable) {
		ret = -EPERM;
		goto exit_unlock;
	}

	set_fan_speed(fan_data, rpm_to_speed_index(fan_data, rpm));

exit_unlock:
	mutex_unlock(&fan_data->lock);

	return ret;
}

<<<<<<< HEAD
static DEVICE_ATTR(pwm1, S_IRUGO | S_IWUSR, show_pwm, set_pwm);
static DEVICE_ATTR(pwm1_enable, S_IRUGO | S_IWUSR,
		   show_pwm_enable, set_pwm_enable);
static DEVICE_ATTR(pwm1_mode, S_IRUGO, show_pwm_mode, NULL);
static DEVICE_ATTR(fan1_min, S_IRUGO, show_rpm_min, NULL);
static DEVICE_ATTR(fan1_max, S_IRUGO, show_rpm_max, NULL);
static DEVICE_ATTR(fan1_input, S_IRUGO, show_rpm, NULL);
static DEVICE_ATTR(fan1_target, S_IRUGO | S_IWUSR, show_rpm, set_rpm);

static struct attribute *gpio_fan_ctrl_attributes[] = {
	&dev_attr_pwm1.attr,
=======
static DEVICE_ATTR_RW(pwm1);
static DEVICE_ATTR_RW(pwm1_enable);
static DEVICE_ATTR_RO(pwm1_mode);
static DEVICE_ATTR_RO(fan1_min);
static DEVICE_ATTR_RO(fan1_max);
static DEVICE_ATTR_RO(fan1_input);
static DEVICE_ATTR(fan1_target, 0644, fan1_input_show, set_rpm);

static umode_t gpio_fan_is_visible(struct kobject *kobj,
				   struct attribute *attr, int index)
{
	struct device *dev = kobj_to_dev(kobj);
	struct gpio_fan_data *data = dev_get_drvdata(dev);

	if (index == 0 && !data->alarm_gpio)
		return 0;
	if (index > 0 && !data->gpios)
		return 0;

	return attr->mode;
}

static struct attribute *gpio_fan_attributes[] = {
	&dev_attr_fan1_alarm.attr,		/* 0 */
	&dev_attr_pwm1.attr,			/* 1 */
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	&dev_attr_pwm1_enable.attr,
	&dev_attr_pwm1_mode.attr,
	&dev_attr_fan1_input.attr,
	&dev_attr_fan1_target.attr,
	&dev_attr_fan1_min.attr,
	&dev_attr_fan1_max.attr,
	NULL
};

<<<<<<< HEAD
static const struct attribute_group gpio_fan_ctrl_group = {
	.attrs = gpio_fan_ctrl_attributes,
};

static int fan_ctrl_init(struct gpio_fan_data *fan_data,
			 struct gpio_fan_platform_data *pdata)
{
	struct platform_device *pdev = fan_data->pdev;
	int num_ctrl = pdata->num_ctrl;
	unsigned *ctrl = pdata->ctrl;
	int i, err;

	for (i = 0; i < num_ctrl; i++) {
		err = gpio_request(ctrl[i], "GPIO fan control");
		if (err)
			goto err_free_gpio;

		err = gpio_direction_output(ctrl[i], gpio_get_value(ctrl[i]));
		if (err) {
			gpio_free(ctrl[i]);
			goto err_free_gpio;
		}
	}

	fan_data->num_ctrl = num_ctrl;
	fan_data->ctrl = ctrl;
	fan_data->num_speed = pdata->num_speed;
	fan_data->speed = pdata->speed;
	fan_data->pwm_enable = true; /* Enable manual fan speed control. */
	fan_data->speed_index = get_fan_speed_index(fan_data);
	if (fan_data->speed_index < 0) {
		err = -ENODEV;
		goto err_free_gpio;
	}

	err = sysfs_create_group(&pdev->dev.kobj, &gpio_fan_ctrl_group);
	if (err)
		goto err_free_gpio;

	return 0;

err_free_gpio:
	for (i = i - 1; i >= 0; i--)
		gpio_free(ctrl[i]);

	return err;
}

static void fan_ctrl_free(struct gpio_fan_data *fan_data)
{
	struct platform_device *pdev = fan_data->pdev;
	int i;

	sysfs_remove_group(&pdev->dev.kobj, &gpio_fan_ctrl_group);
	for (i = 0; i < fan_data->num_ctrl; i++)
		gpio_free(fan_data->ctrl[i]);
}

/*
 * Platform driver.
 */

static ssize_t show_name(struct device *dev,
			 struct device_attribute *attr, char *buf)
{
	return sprintf(buf, "gpio-fan\n");
}

static DEVICE_ATTR(name, S_IRUGO, show_name, NULL);

static int __devinit gpio_fan_probe(struct platform_device *pdev)
{
	int err;
	struct gpio_fan_data *fan_data;
	struct gpio_fan_platform_data *pdata = pdev->dev.platform_data;

	if (!pdata)
		return -EINVAL;

	fan_data = kzalloc(sizeof(struct gpio_fan_data), GFP_KERNEL);
	if (!fan_data)
		return -ENOMEM;

	fan_data->pdev = pdev;
	platform_set_drvdata(pdev, fan_data);
	mutex_init(&fan_data->lock);

	/* Configure alarm GPIO if available. */
	if (pdata->alarm) {
		err = fan_alarm_init(fan_data, pdata->alarm);
		if (err)
			goto err_free_data;
	}

	/* Configure control GPIOs if available. */
	if (pdata->ctrl && pdata->num_ctrl > 0) {
		if (!pdata->speed || pdata->num_speed <= 1) {
			err = -EINVAL;
			goto err_free_alarm;
		}
		err = fan_ctrl_init(fan_data, pdata);
		if (err)
			goto err_free_alarm;
	}

	err = device_create_file(&pdev->dev, &dev_attr_name);
	if (err)
		goto err_free_ctrl;

	/* Make this driver part of hwmon class. */
	fan_data->hwmon_dev = hwmon_device_register(&pdev->dev);
	if (IS_ERR(fan_data->hwmon_dev)) {
		err = PTR_ERR(fan_data->hwmon_dev);
		goto err_remove_name;
	}

	dev_info(&pdev->dev, "GPIO fan initialized\n");

	return 0;

err_remove_name:
	device_remove_file(&pdev->dev, &dev_attr_name);
err_free_ctrl:
	if (fan_data->ctrl)
		fan_ctrl_free(fan_data);
err_free_alarm:
	if (fan_data->alarm)
		fan_alarm_free(fan_data);
err_free_data:
	platform_set_drvdata(pdev, NULL);
	kfree(fan_data);

	return err;
}

static int __devexit gpio_fan_remove(struct platform_device *pdev)
{
	struct gpio_fan_data *fan_data = platform_get_drvdata(pdev);

	hwmon_device_unregister(fan_data->hwmon_dev);
	device_remove_file(&pdev->dev, &dev_attr_name);
	if (fan_data->alarm)
		fan_alarm_free(fan_data);
	if (fan_data->ctrl)
		fan_ctrl_free(fan_data);
	kfree(fan_data);
=======
static const struct attribute_group gpio_fan_group = {
	.attrs = gpio_fan_attributes,
	.is_visible = gpio_fan_is_visible,
};

static const struct attribute_group *gpio_fan_groups[] = {
	&gpio_fan_group,
	NULL
};

static int fan_ctrl_init(struct gpio_fan_data *fan_data)
{
	int num_gpios = fan_data->num_gpios;
	struct gpio_desc **gpios = fan_data->gpios;
	int i, err;

	for (i = 0; i < num_gpios; i++) {
		/*
		 * The GPIO descriptors were retrieved with GPIOD_ASIS so here
		 * we set the GPIO into output mode, carefully preserving the
		 * current value by setting it to whatever it is already set
		 * (no surprise changes in default fan speed).
		 */
		err = gpiod_direction_output(gpios[i],
					gpiod_get_value_cansleep(gpios[i]));
		if (err)
			return err;
	}

	fan_data->pwm_enable = true; /* Enable manual fan speed control. */
	fan_data->speed_index = get_fan_speed_index(fan_data);
	if (fan_data->speed_index < 0)
		return fan_data->speed_index;

	return 0;
}

static int gpio_fan_get_max_state(struct thermal_cooling_device *cdev,
				  unsigned long *state)
{
	struct gpio_fan_data *fan_data = cdev->devdata;

	if (!fan_data)
		return -EINVAL;

	*state = fan_data->num_speed - 1;
	return 0;
}

static int gpio_fan_get_cur_state(struct thermal_cooling_device *cdev,
				  unsigned long *state)
{
	struct gpio_fan_data *fan_data = cdev->devdata;

	if (!fan_data)
		return -EINVAL;

	*state = fan_data->speed_index;
	return 0;
}

static int gpio_fan_set_cur_state(struct thermal_cooling_device *cdev,
				  unsigned long state)
{
	struct gpio_fan_data *fan_data = cdev->devdata;

	if (!fan_data)
		return -EINVAL;

	if (state >= fan_data->num_speed)
		return -EINVAL;

	set_fan_speed(fan_data, state);
	return 0;
}

static const struct thermal_cooling_device_ops gpio_fan_cool_ops = {
	.get_max_state = gpio_fan_get_max_state,
	.get_cur_state = gpio_fan_get_cur_state,
	.set_cur_state = gpio_fan_set_cur_state,
};

/*
 * Translate OpenFirmware node properties into platform_data
 */
static int gpio_fan_get_of_data(struct gpio_fan_data *fan_data)
{
	struct gpio_fan_speed *speed;
	struct device *dev = fan_data->dev;
	struct device_node *np = dev->of_node;
	struct gpio_desc **gpios;
	unsigned i;
	u32 u;
	struct property *prop;
	const __be32 *p;

	/* Alarm GPIO if one exists */
	fan_data->alarm_gpio = devm_gpiod_get_optional(dev, "alarm", GPIOD_IN);
	if (IS_ERR(fan_data->alarm_gpio))
		return PTR_ERR(fan_data->alarm_gpio);

	/* Fill GPIO pin array */
	fan_data->num_gpios = gpiod_count(dev, NULL);
	if (fan_data->num_gpios <= 0) {
		if (fan_data->alarm_gpio)
			return 0;
		dev_err(dev, "DT properties empty / missing");
		return -ENODEV;
	}
	gpios = devm_kcalloc(dev,
			     fan_data->num_gpios, sizeof(struct gpio_desc *),
			     GFP_KERNEL);
	if (!gpios)
		return -ENOMEM;
	for (i = 0; i < fan_data->num_gpios; i++) {
		gpios[i] = devm_gpiod_get_index(dev, NULL, i, GPIOD_ASIS);
		if (IS_ERR(gpios[i]))
			return PTR_ERR(gpios[i]);
	}
	fan_data->gpios = gpios;

	/* Get number of RPM/ctrl_val pairs in speed map */
	prop = of_find_property(np, "gpio-fan,speed-map", &i);
	if (!prop) {
		dev_err(dev, "gpio-fan,speed-map DT property missing");
		return -ENODEV;
	}
	i = i / sizeof(u32);
	if (i == 0 || i & 1) {
		dev_err(dev, "gpio-fan,speed-map contains zero/odd number of entries");
		return -ENODEV;
	}
	fan_data->num_speed = i / 2;

	/*
	 * Populate speed map
	 * Speed map is in the form <RPM ctrl_val RPM ctrl_val ...>
	 * this needs splitting into pairs to create gpio_fan_speed structs
	 */
	speed = devm_kcalloc(dev,
			fan_data->num_speed, sizeof(struct gpio_fan_speed),
			GFP_KERNEL);
	if (!speed)
		return -ENOMEM;
	p = NULL;
	for (i = 0; i < fan_data->num_speed; i++) {
		p = of_prop_next_u32(prop, p, &u);
		if (!p)
			return -ENODEV;
		speed[i].rpm = u;
		p = of_prop_next_u32(prop, p, &u);
		if (!p)
			return -ENODEV;
		speed[i].ctrl_val = u;
	}
	fan_data->speed = speed;

	return 0;
}

static const struct of_device_id of_gpio_fan_match[] = {
	{ .compatible = "gpio-fan", },
	{},
};
MODULE_DEVICE_TABLE(of, of_gpio_fan_match);

static void gpio_fan_stop(void *data)
{
	set_fan_speed(data, 0);
}

static int gpio_fan_probe(struct platform_device *pdev)
{
	int err;
	struct gpio_fan_data *fan_data;
	struct device *dev = &pdev->dev;
	struct device_node *np = dev->of_node;

	fan_data = devm_kzalloc(dev, sizeof(struct gpio_fan_data),
				GFP_KERNEL);
	if (!fan_data)
		return -ENOMEM;

	fan_data->dev = dev;
	err = gpio_fan_get_of_data(fan_data);
	if (err)
		return err;

	platform_set_drvdata(pdev, fan_data);
	mutex_init(&fan_data->lock);

	/* Configure control GPIOs if available. */
	if (fan_data->gpios && fan_data->num_gpios > 0) {
		if (!fan_data->speed || fan_data->num_speed <= 1)
			return -EINVAL;
		err = fan_ctrl_init(fan_data);
		if (err)
			return err;
		err = devm_add_action_or_reset(dev, gpio_fan_stop, fan_data);
		if (err)
			return err;
	}

	/* Make this driver part of hwmon class. */
	fan_data->hwmon_dev =
		devm_hwmon_device_register_with_groups(dev,
						       "gpio_fan", fan_data,
						       gpio_fan_groups);
	if (IS_ERR(fan_data->hwmon_dev))
		return PTR_ERR(fan_data->hwmon_dev);

	/* Configure alarm GPIO if available. */
	if (fan_data->alarm_gpio) {
		err = fan_alarm_init(fan_data);
		if (err)
			return err;
	}

	/* Optional cooling device register for Device tree platforms */
	fan_data->cdev = devm_thermal_of_cooling_device_register(dev, np,
				"gpio-fan", fan_data, &gpio_fan_cool_ops);

	dev_info(dev, "GPIO fan initialized\n");
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	return 0;
}

<<<<<<< HEAD
#ifdef CONFIG_PM
static int gpio_fan_suspend(struct platform_device *pdev, pm_message_t state)
{
	struct gpio_fan_data *fan_data = platform_get_drvdata(pdev);

	if (fan_data->ctrl) {
=======
static void gpio_fan_shutdown(struct platform_device *pdev)
{
	struct gpio_fan_data *fan_data = platform_get_drvdata(pdev);

	if (fan_data->gpios)
		set_fan_speed(fan_data, 0);
}

static int gpio_fan_suspend(struct device *dev)
{
	struct gpio_fan_data *fan_data = dev_get_drvdata(dev);

	if (fan_data->gpios) {
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		fan_data->resume_speed = fan_data->speed_index;
		set_fan_speed(fan_data, 0);
	}

	return 0;
}

<<<<<<< HEAD
static int gpio_fan_resume(struct platform_device *pdev)
{
	struct gpio_fan_data *fan_data = platform_get_drvdata(pdev);

	if (fan_data->ctrl)
=======
static int gpio_fan_resume(struct device *dev)
{
	struct gpio_fan_data *fan_data = dev_get_drvdata(dev);

	if (fan_data->gpios)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		set_fan_speed(fan_data, fan_data->resume_speed);

	return 0;
}
<<<<<<< HEAD
#else
#define gpio_fan_suspend NULL
#define gpio_fan_resume NULL
#endif

static struct platform_driver gpio_fan_driver = {
	.probe		= gpio_fan_probe,
	.remove		= __devexit_p(gpio_fan_remove),
	.suspend	= gpio_fan_suspend,
	.resume		= gpio_fan_resume,
	.driver	= {
		.name	= "gpio-fan",
=======

static DEFINE_SIMPLE_DEV_PM_OPS(gpio_fan_pm, gpio_fan_suspend, gpio_fan_resume);

static struct platform_driver gpio_fan_driver = {
	.probe		= gpio_fan_probe,
	.shutdown	= gpio_fan_shutdown,
	.driver	= {
		.name	= "gpio-fan",
		.pm	= pm_sleep_ptr(&gpio_fan_pm),
		.of_match_table = of_gpio_fan_match,
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	},
};

module_platform_driver(gpio_fan_driver);

MODULE_AUTHOR("Simon Guinot <sguinot@lacie.com>");
MODULE_DESCRIPTION("GPIO FAN driver");
MODULE_LICENSE("GPL");
MODULE_ALIAS("platform:gpio-fan");
