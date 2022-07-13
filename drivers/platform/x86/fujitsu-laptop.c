<<<<<<< HEAD
/*-*-linux-c-*-*/

/*
  Copyright (C) 2007,2008 Jonathan Woithe <jwoithe@physics.adelaide.edu.au>
=======
// SPDX-License-Identifier: GPL-2.0-or-later
/*-*-linux-c-*-*/

/*
  Copyright (C) 2007,2008 Jonathan Woithe <jwoithe@just42.net>
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
  Copyright (C) 2008 Peter Gruber <nokos@gmx.net>
  Copyright (C) 2008 Tony Vroon <tony@linx.net>
  Based on earlier work:
    Copyright (C) 2003 Shane Spencer <shane@bogomip.com>
    Adrian Yee <brewt-fujitsu@brewt.org>

  Templated from msi-laptop.c and thinkpad_acpi.c which is copyright
  by its respective authors.

<<<<<<< HEAD
  This program is free software; you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation; either version 2 of the License, or
  (at your option) any later version.

  This program is distributed in the hope that it will be useful, but
  WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with this program; if not, write to the Free Software
  Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA
  02110-1301, USA.
=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 */

/*
 * fujitsu-laptop.c - Fujitsu laptop support, providing access to additional
 * features made available on a range of Fujitsu laptops including the
 * P2xxx/P5xxx/S6xxx/S7xxx series.
 *
<<<<<<< HEAD
 * This driver exports a few files in /sys/devices/platform/fujitsu-laptop/;
 * others may be added at a later date.
 *
 *   lcd_level - Screen brightness: contains a single integer in the
 *   range 0..7. (rw)
 *
 * In addition to these platform device attributes the driver
 * registers itself in the Linux backlight control subsystem and is
 * available to userspace under /sys/class/backlight/fujitsu-laptop/.
 *
 * Hotkeys present on certain Fujitsu laptops (eg: the S6xxx series) are
 * also supported by this driver.
=======
 * This driver implements a vendor-specific backlight control interface for
 * Fujitsu laptops and provides support for hotkeys present on certain Fujitsu
 * laptops.
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 *
 * This driver has been tested on a Fujitsu Lifebook S6410, S7020 and
 * P8010.  It should work on most P-series and S-series Lifebooks, but
 * YMMV.
 *
 * The module parameter use_alt_lcd_levels switches between different ACPI
 * brightness controls which are used by different Fujitsu laptops.  In most
 * cases the correct method is automatically detected. "use_alt_lcd_levels=1"
 * is applicable for a Fujitsu Lifebook S6410 if autodetection fails.
 *
 */

#define pr_fmt(fmt) KBUILD_MODNAME ": " fmt

#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/acpi.h>
<<<<<<< HEAD
#include <linux/dmi.h>
#include <linux/backlight.h>
#include <linux/input.h>
#include <linux/kfifo.h>
#include <linux/video_output.h>
#include <linux/platform_device.h>
#include <linux/slab.h>
#if defined(CONFIG_LEDS_CLASS) || defined(CONFIG_LEDS_CLASS_MODULE)
#include <linux/leds.h>
#endif

#define FUJITSU_DRIVER_VERSION "0.6.0"

#define FUJITSU_LCD_N_LEVELS 8

#define ACPI_FUJITSU_CLASS              "fujitsu"
#define ACPI_FUJITSU_HID                "FUJ02B1"
#define ACPI_FUJITSU_DRIVER_NAME	"Fujitsu laptop FUJ02B1 ACPI brightness driver"
#define ACPI_FUJITSU_DEVICE_NAME        "Fujitsu FUJ02B1"
#define ACPI_FUJITSU_HOTKEY_HID 	"FUJ02E3"
#define ACPI_FUJITSU_HOTKEY_DRIVER_NAME "Fujitsu laptop FUJ02E3 ACPI hotkeys driver"
#define ACPI_FUJITSU_HOTKEY_DEVICE_NAME "Fujitsu FUJ02E3"

#define ACPI_FUJITSU_NOTIFY_CODE1     0x80

#define ACPI_VIDEO_NOTIFY_INC_BRIGHTNESS     0x86
#define ACPI_VIDEO_NOTIFY_DEC_BRIGHTNESS     0x87

/* FUNC interface - command values */
#define FUNC_RFKILL	0x1000
#define FUNC_LEDS	0x1001
#define FUNC_BUTTONS	0x1002
#define FUNC_BACKLIGHT  0x1004

/* FUNC interface - responses */
#define UNSUPPORTED_CMD 0x80000000

#if defined(CONFIG_LEDS_CLASS) || defined(CONFIG_LEDS_CLASS_MODULE)
/* FUNC interface - LED control */
#define FUNC_LED_OFF	0x1
#define FUNC_LED_ON	0x30001
#define KEYBOARD_LAMPS	0x100
#define LOGOLAMP_POWERON 0x2000
#define LOGOLAMP_ALWAYS  0x4000
#endif

/* Hotkey details */
#define KEY1_CODE	0x410	/* codes for the keys in the GIRB register */
#define KEY2_CODE	0x411
#define KEY3_CODE	0x412
#define KEY4_CODE	0x413

#define MAX_HOTKEY_RINGBUFFER_SIZE 100
#define RINGBUFFERSIZE 40

/* Debugging */
#define FUJLAPTOP_LOG	   ACPI_FUJITSU_HID ": "
#define FUJLAPTOP_ERR	   KERN_ERR FUJLAPTOP_LOG
#define FUJLAPTOP_NOTICE   KERN_NOTICE FUJLAPTOP_LOG
#define FUJLAPTOP_INFO	   KERN_INFO FUJLAPTOP_LOG
#define FUJLAPTOP_DEBUG    KERN_DEBUG FUJLAPTOP_LOG

#define FUJLAPTOP_DBG_ALL	  0xffff
#define FUJLAPTOP_DBG_ERROR	  0x0001
#define FUJLAPTOP_DBG_WARN	  0x0002
#define FUJLAPTOP_DBG_INFO	  0x0004
#define FUJLAPTOP_DBG_TRACE	  0x0008

#define dbg_printk(a_dbg_level, format, arg...) \
	do { if (dbg_level & a_dbg_level) \
		printk(FUJLAPTOP_DEBUG "%s: " format, __func__ , ## arg); \
	} while (0)
#ifdef CONFIG_FUJITSU_LAPTOP_DEBUG
#define vdbg_printk(a_dbg_level, format, arg...) \
	dbg_printk(a_dbg_level, format, ## arg)
#else
#define vdbg_printk(a_dbg_level, format, arg...)
#endif

/* Device controlling the backlight and associated keys */
struct fujitsu_t {
	acpi_handle acpi_handle;
	struct acpi_device *dev;
	struct input_dev *input;
	char phys[32];
	struct backlight_device *bl_device;
	struct platform_device *pf_device;
	int keycode1, keycode2, keycode3, keycode4;

	unsigned int max_brightness;
	unsigned int brightness_changed;
	unsigned int brightness_level;
};

static struct fujitsu_t *fujitsu;
static int use_alt_lcd_levels = -1;
static int disable_brightness_adjust = -1;

/* Device used to access other hotkeys on the laptop */
struct fujitsu_hotkey_t {
	acpi_handle acpi_handle;
	struct acpi_device *dev;
=======
#include <linux/bitops.h>
#include <linux/dmi.h>
#include <linux/backlight.h>
#include <linux/fb.h>
#include <linux/input.h>
#include <linux/input/sparse-keymap.h>
#include <linux/kfifo.h>
#include <linux/leds.h>
#include <linux/platform_device.h>
#include <linux/power_supply.h>
#include <acpi/battery.h>
#include <acpi/video.h>

#define FUJITSU_DRIVER_VERSION		"0.6.0"

#define FUJITSU_LCD_N_LEVELS		8

#define ACPI_FUJITSU_CLASS		"fujitsu"
#define ACPI_FUJITSU_BL_HID		"FUJ02B1"
#define ACPI_FUJITSU_BL_DRIVER_NAME	"Fujitsu laptop FUJ02B1 ACPI brightness driver"
#define ACPI_FUJITSU_BL_DEVICE_NAME	"Fujitsu FUJ02B1"
#define ACPI_FUJITSU_LAPTOP_HID		"FUJ02E3"
#define ACPI_FUJITSU_LAPTOP_DRIVER_NAME	"Fujitsu laptop FUJ02E3 ACPI hotkeys driver"
#define ACPI_FUJITSU_LAPTOP_DEVICE_NAME	"Fujitsu FUJ02E3"

#define ACPI_FUJITSU_NOTIFY_CODE	0x80

/* FUNC interface - command values */
#define FUNC_FLAGS			BIT(12)
#define FUNC_LEDS			(BIT(12) | BIT(0))
#define FUNC_BUTTONS			(BIT(12) | BIT(1))
#define FUNC_BACKLIGHT			(BIT(12) | BIT(2))

/* FUNC interface - responses */
#define UNSUPPORTED_CMD			0x80000000

/* FUNC interface - status flags */
#define FLAG_RFKILL			BIT(5)
#define FLAG_LID			BIT(8)
#define FLAG_DOCK			BIT(9)
#define FLAG_TOUCHPAD_TOGGLE		BIT(26)
#define FLAG_MICMUTE			BIT(29)
#define FLAG_SOFTKEYS			(FLAG_RFKILL | FLAG_TOUCHPAD_TOGGLE | FLAG_MICMUTE)

/* FUNC interface - LED control */
#define FUNC_LED_OFF			BIT(0)
#define FUNC_LED_ON			(BIT(0) | BIT(16) | BIT(17))
#define LOGOLAMP_POWERON		BIT(13)
#define LOGOLAMP_ALWAYS			BIT(14)
#define KEYBOARD_LAMPS			BIT(8)
#define RADIO_LED_ON			BIT(5)
#define ECO_LED				BIT(16)
#define ECO_LED_ON			BIT(19)

/* FUNC interface - backlight power control */
#define BACKLIGHT_PARAM_POWER		BIT(2)
#define BACKLIGHT_OFF			(BIT(0) | BIT(1))
#define BACKLIGHT_ON			0

/* FUNC interface - battery control interface */
#define FUNC_S006_METHOD		0x1006
#define CHARGE_CONTROL_RW		0x21

/* Scancodes read from the GIRB register */
#define KEY1_CODE			0x410
#define KEY2_CODE			0x411
#define KEY3_CODE			0x412
#define KEY4_CODE			0x413
#define KEY5_CODE			0x420

/* Hotkey ringbuffer limits */
#define MAX_HOTKEY_RINGBUFFER_SIZE	100
#define RINGBUFFERSIZE			40

/* Module parameters */
static int use_alt_lcd_levels = -1;
static bool disable_brightness_adjust;

/* Device controlling the backlight and associated keys */
struct fujitsu_bl {
	struct input_dev *input;
	char phys[32];
	struct backlight_device *bl_device;
	unsigned int max_brightness;
	unsigned int brightness_level;
};

static struct fujitsu_bl *fujitsu_bl;

/* Device used to access hotkeys and other features on the laptop */
struct fujitsu_laptop {
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	struct input_dev *input;
	char phys[32];
	struct platform_device *pf_device;
	struct kfifo fifo;
	spinlock_t fifo_lock;
<<<<<<< HEAD
	int rfkill_supported;
	int rfkill_state;
	int logolamp_registered;
	int kblamps_registered;
};

static struct fujitsu_hotkey_t *fujitsu_hotkey;

static void acpi_fujitsu_hotkey_notify(struct acpi_device *device, u32 event);

#if defined(CONFIG_LEDS_CLASS) || defined(CONFIG_LEDS_CLASS_MODULE)
static enum led_brightness logolamp_get(struct led_classdev *cdev);
static void logolamp_set(struct led_classdev *cdev,
			       enum led_brightness brightness);

static struct led_classdev logolamp_led = {
 .name = "fujitsu::logolamp",
 .brightness_get = logolamp_get,
 .brightness_set = logolamp_set
};

static enum led_brightness kblamps_get(struct led_classdev *cdev);
static void kblamps_set(struct led_classdev *cdev,
			       enum led_brightness brightness);

static struct led_classdev kblamps_led = {
 .name = "fujitsu::kblamps",
 .brightness_get = kblamps_get,
 .brightness_set = kblamps_set
};
#endif

#ifdef CONFIG_FUJITSU_LAPTOP_DEBUG
static u32 dbg_level = 0x03;
#endif

static void acpi_fujitsu_notify(struct acpi_device *device, u32 event);

/* Fujitsu ACPI interface function */

static int call_fext_func(int cmd, int arg0, int arg1, int arg2)
{
	acpi_status status = AE_OK;
	union acpi_object params[4] = {
	{ .type = ACPI_TYPE_INTEGER },
	{ .type = ACPI_TYPE_INTEGER },
	{ .type = ACPI_TYPE_INTEGER },
	{ .type = ACPI_TYPE_INTEGER }
	};
	struct acpi_object_list arg_list = { 4, &params[0] };
	struct acpi_buffer output;
	union acpi_object out_obj;
	acpi_handle handle = NULL;

	status = acpi_get_handle(fujitsu_hotkey->acpi_handle, "FUNC", &handle);
	if (ACPI_FAILURE(status)) {
		vdbg_printk(FUJLAPTOP_DBG_ERROR,
				"FUNC interface is not present\n");
		return -ENODEV;
	}

	params[0].integer.value = cmd;
	params[1].integer.value = arg0;
	params[2].integer.value = arg1;
	params[3].integer.value = arg2;

	output.length = sizeof(out_obj);
	output.pointer = &out_obj;

	status = acpi_evaluate_object(handle, NULL, &arg_list, &output);
	if (ACPI_FAILURE(status)) {
		vdbg_printk(FUJLAPTOP_DBG_WARN,
			"FUNC 0x%x (args 0x%x, 0x%x, 0x%x) call failed\n",
				cmd, arg0, arg1, arg2);
		return -ENODEV;
	}

	if (out_obj.type != ACPI_TYPE_INTEGER) {
		vdbg_printk(FUJLAPTOP_DBG_WARN,
			"FUNC 0x%x (args 0x%x, 0x%x, 0x%x) did not "
			"return an integer\n",
			cmd, arg0, arg1, arg2);
		return -ENODEV;
	}

	vdbg_printk(FUJLAPTOP_DBG_TRACE,
		"FUNC 0x%x (args 0x%x, 0x%x, 0x%x) returned 0x%x\n",
			cmd, arg0, arg1, arg2, (int)out_obj.integer.value);
	return out_obj.integer.value;
}

#if defined(CONFIG_LEDS_CLASS) || defined(CONFIG_LEDS_CLASS_MODULE)
/* LED class callbacks */

static void logolamp_set(struct led_classdev *cdev,
			       enum led_brightness brightness)
{
	if (brightness >= LED_FULL) {
		call_fext_func(FUNC_LEDS, 0x1, LOGOLAMP_POWERON, FUNC_LED_ON);
		call_fext_func(FUNC_LEDS, 0x1, LOGOLAMP_ALWAYS, FUNC_LED_ON);
	} else if (brightness >= LED_HALF) {
		call_fext_func(FUNC_LEDS, 0x1, LOGOLAMP_POWERON, FUNC_LED_ON);
		call_fext_func(FUNC_LEDS, 0x1, LOGOLAMP_ALWAYS, FUNC_LED_OFF);
	} else {
		call_fext_func(FUNC_LEDS, 0x1, LOGOLAMP_POWERON, FUNC_LED_OFF);
	}
}

static void kblamps_set(struct led_classdev *cdev,
			       enum led_brightness brightness)
{
	if (brightness >= LED_FULL)
		call_fext_func(FUNC_LEDS, 0x1, KEYBOARD_LAMPS, FUNC_LED_ON);
	else
		call_fext_func(FUNC_LEDS, 0x1, KEYBOARD_LAMPS, FUNC_LED_OFF);
}

static enum led_brightness logolamp_get(struct led_classdev *cdev)
{
	enum led_brightness brightness = LED_OFF;
	int poweron, always;

	poweron = call_fext_func(FUNC_LEDS, 0x2, LOGOLAMP_POWERON, 0x0);
	if (poweron == FUNC_LED_ON) {
		brightness = LED_HALF;
		always = call_fext_func(FUNC_LEDS, 0x2, LOGOLAMP_ALWAYS, 0x0);
		if (always == FUNC_LED_ON)
			brightness = LED_FULL;
	}
	return brightness;
}

static enum led_brightness kblamps_get(struct led_classdev *cdev)
{
	enum led_brightness brightness = LED_OFF;

	if (call_fext_func(FUNC_LEDS, 0x2, KEYBOARD_LAMPS, 0x0) == FUNC_LED_ON)
		brightness = LED_FULL;

	return brightness;
}
#endif

/* Hardware access for LCD brightness control */

static int set_lcd_level(int level)
{
	acpi_status status = AE_OK;
	union acpi_object arg0 = { ACPI_TYPE_INTEGER };
	struct acpi_object_list arg_list = { 1, &arg0 };
	acpi_handle handle = NULL;

	vdbg_printk(FUJLAPTOP_DBG_TRACE, "set lcd level via SBLL [%d]\n",
		    level);

	if (level < 0 || level >= fujitsu->max_brightness)
		return -EINVAL;

	status = acpi_get_handle(fujitsu->acpi_handle, "SBLL", &handle);
	if (ACPI_FAILURE(status)) {
		vdbg_printk(FUJLAPTOP_DBG_ERROR, "SBLL not present\n");
		return -ENODEV;
	}

	arg0.integer.value = level;

	status = acpi_evaluate_object(handle, NULL, &arg_list, NULL);
	if (ACPI_FAILURE(status))
		return -ENODEV;
=======
	int flags_supported;
	int flags_state;
	bool charge_control_supported;
};

static struct acpi_device *fext;

/* Fujitsu ACPI interface function */

static int call_fext_func(struct acpi_device *device,
			  int func, int op, int feature, int state)
{
	union acpi_object params[4] = {
		{ .integer.type = ACPI_TYPE_INTEGER, .integer.value = func },
		{ .integer.type = ACPI_TYPE_INTEGER, .integer.value = op },
		{ .integer.type = ACPI_TYPE_INTEGER, .integer.value = feature },
		{ .integer.type = ACPI_TYPE_INTEGER, .integer.value = state }
	};
	struct acpi_object_list arg_list = { 4, params };
	unsigned long long value;
	acpi_status status;

	status = acpi_evaluate_integer(device->handle, "FUNC", &arg_list,
				       &value);
	if (ACPI_FAILURE(status)) {
		acpi_handle_err(device->handle, "Failed to evaluate FUNC\n");
		return -ENODEV;
	}

	acpi_handle_debug(device->handle,
			  "FUNC 0x%x (args 0x%x, 0x%x, 0x%x) returned 0x%x\n",
			  func, op, feature, state, (int)value);
	return value;
}

/* Battery charge control code */
static ssize_t charge_control_end_threshold_store(struct device *dev,
				struct device_attribute *attr,
				const char *buf, size_t count)
{
	int cc_end_value, s006_cc_return;
	int value, ret;

	ret = kstrtouint(buf, 10, &value);
	if (ret)
		return ret;

	if (value < 50 || value > 100)
		return -EINVAL;

	cc_end_value = value * 0x100 + 0x20;
	s006_cc_return = call_fext_func(fext, FUNC_S006_METHOD,
					CHARGE_CONTROL_RW, cc_end_value, 0x0);
	if (s006_cc_return < 0)
		return s006_cc_return;
	/*
	 * The S006 0x21 method returns 0x00 in case the provided value
	 * is invalid.
	 */
	if (s006_cc_return == 0x00)
		return -EINVAL;

	return count;
}

static ssize_t charge_control_end_threshold_show(struct device *dev,
				struct device_attribute *attr,
				char *buf)
{
	int status;

	status = call_fext_func(fext, FUNC_S006_METHOD,
				CHARGE_CONTROL_RW, 0x21, 0x0);
	if (status < 0)
		return status;

	return sysfs_emit(buf, "%d\n", status);
}

static DEVICE_ATTR_RW(charge_control_end_threshold);

/* ACPI battery hook */
static int fujitsu_battery_add_hook(struct power_supply *battery,
			       struct acpi_battery_hook *hook)
{
	return device_create_file(&battery->dev,
				  &dev_attr_charge_control_end_threshold);
}

static int fujitsu_battery_remove_hook(struct power_supply *battery,
				  struct acpi_battery_hook *hook)
{
	device_remove_file(&battery->dev,
			   &dev_attr_charge_control_end_threshold);

	return 0;
}

static struct acpi_battery_hook battery_hook = {
	.add_battery = fujitsu_battery_add_hook,
	.remove_battery = fujitsu_battery_remove_hook,
	.name = "Fujitsu Battery Extension",
};

/*
 * These functions are intended to be called from acpi_fujitsu_laptop_add and
 * acpi_fujitsu_laptop_remove.
 */
static int fujitsu_battery_charge_control_add(struct acpi_device *device)
{
	struct fujitsu_laptop *priv = acpi_driver_data(device);
	int s006_cc_return;

	priv->charge_control_supported = false;
	/*
	 * Check if the S006 0x21 method exists by trying to get the current
	 * battery charge limit.
	 */
	s006_cc_return = call_fext_func(fext, FUNC_S006_METHOD,
					CHARGE_CONTROL_RW, 0x21, 0x0);
	if (s006_cc_return < 0)
		return s006_cc_return;
	if (s006_cc_return == UNSUPPORTED_CMD)
		return -ENODEV;

	priv->charge_control_supported = true;
	battery_hook_register(&battery_hook);

	return 0;
}

static void fujitsu_battery_charge_control_remove(struct acpi_device *device)
{
	struct fujitsu_laptop *priv = acpi_driver_data(device);

	if (priv->charge_control_supported)
		battery_hook_unregister(&battery_hook);
}

/* Hardware access for LCD brightness control */

static int set_lcd_level(struct acpi_device *device, int level)
{
	struct fujitsu_bl *priv = acpi_driver_data(device);
	acpi_status status;
	char *method;

	switch (use_alt_lcd_levels) {
	case -1:
		if (acpi_has_method(device->handle, "SBL2"))
			method = "SBL2";
		else
			method = "SBLL";
		break;
	case 1:
		method = "SBL2";
		break;
	default:
		method = "SBLL";
		break;
	}

	acpi_handle_debug(device->handle, "set lcd level via %s [%d]\n", method,
			  level);

	if (level < 0 || level >= priv->max_brightness)
		return -EINVAL;

	status = acpi_execute_simple_method(device->handle, method, level);
	if (ACPI_FAILURE(status)) {
		acpi_handle_err(device->handle, "Failed to evaluate %s\n",
				method);
		return -ENODEV;
	}

	priv->brightness_level = level;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	return 0;
}

<<<<<<< HEAD
static int set_lcd_level_alt(int level)
{
	acpi_status status = AE_OK;
	union acpi_object arg0 = { ACPI_TYPE_INTEGER };
	struct acpi_object_list arg_list = { 1, &arg0 };
	acpi_handle handle = NULL;

	vdbg_printk(FUJLAPTOP_DBG_TRACE, "set lcd level via SBL2 [%d]\n",
		    level);

	if (level < 0 || level >= fujitsu->max_brightness)
		return -EINVAL;

	status = acpi_get_handle(fujitsu->acpi_handle, "SBL2", &handle);
	if (ACPI_FAILURE(status)) {
		vdbg_printk(FUJLAPTOP_DBG_ERROR, "SBL2 not present\n");
		return -ENODEV;
	}

	arg0.integer.value = level;

	status = acpi_evaluate_object(handle, NULL, &arg_list, NULL);
	if (ACPI_FAILURE(status))
		return -ENODEV;

	return 0;
}

static int get_lcd_level(void)
{
	unsigned long long state = 0;
	acpi_status status = AE_OK;

	vdbg_printk(FUJLAPTOP_DBG_TRACE, "get lcd level via GBLL\n");

	status =
	    acpi_evaluate_integer(fujitsu->acpi_handle, "GBLL", NULL, &state);
	if (ACPI_FAILURE(status))
		return 0;

	fujitsu->brightness_level = state & 0x0fffffff;

	if (state & 0x80000000)
		fujitsu->brightness_changed = 1;
	else
		fujitsu->brightness_changed = 0;

	return fujitsu->brightness_level;
}

static int get_max_brightness(void)
{
	unsigned long long state = 0;
	acpi_status status = AE_OK;

	vdbg_printk(FUJLAPTOP_DBG_TRACE, "get max lcd level via RBLL\n");

	status =
	    acpi_evaluate_integer(fujitsu->acpi_handle, "RBLL", NULL, &state);
	if (ACPI_FAILURE(status))
		return -1;

	fujitsu->max_brightness = state;

	return fujitsu->max_brightness;
=======
static int get_lcd_level(struct acpi_device *device)
{
	struct fujitsu_bl *priv = acpi_driver_data(device);
	unsigned long long state = 0;
	acpi_status status = AE_OK;

	acpi_handle_debug(device->handle, "get lcd level via GBLL\n");

	status = acpi_evaluate_integer(device->handle, "GBLL", NULL, &state);
	if (ACPI_FAILURE(status))
		return 0;

	priv->brightness_level = state & 0x0fffffff;

	return priv->brightness_level;
}

static int get_max_brightness(struct acpi_device *device)
{
	struct fujitsu_bl *priv = acpi_driver_data(device);
	unsigned long long state = 0;
	acpi_status status = AE_OK;

	acpi_handle_debug(device->handle, "get max lcd level via RBLL\n");

	status = acpi_evaluate_integer(device->handle, "RBLL", NULL, &state);
	if (ACPI_FAILURE(status))
		return -1;

	priv->max_brightness = state;

	return priv->max_brightness;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

/* Backlight device stuff */

static int bl_get_brightness(struct backlight_device *b)
{
<<<<<<< HEAD
	return get_lcd_level();
=======
	struct acpi_device *device = bl_get_data(b);

	return b->props.power == FB_BLANK_POWERDOWN ? 0 : get_lcd_level(device);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

static int bl_update_status(struct backlight_device *b)
{
<<<<<<< HEAD
	int ret;
	if (b->props.power == 4)
		ret = call_fext_func(FUNC_BACKLIGHT, 0x1, 0x4, 0x3);
	else
		ret = call_fext_func(FUNC_BACKLIGHT, 0x1, 0x4, 0x0);
	if (ret != 0)
		vdbg_printk(FUJLAPTOP_DBG_ERROR,
			"Unable to adjust backlight power, error code %i\n",
			ret);

	if (use_alt_lcd_levels)
		ret = set_lcd_level_alt(b->props.brightness);
	else
		ret = set_lcd_level(b->props.brightness);
	if (ret != 0)
		vdbg_printk(FUJLAPTOP_DBG_ERROR,
			"Unable to adjust LCD brightness, error code %i\n",
			ret);
	return ret;
}

static const struct backlight_ops fujitsubl_ops = {
=======
	struct acpi_device *device = bl_get_data(b);

	if (fext) {
		if (b->props.power == FB_BLANK_POWERDOWN)
			call_fext_func(fext, FUNC_BACKLIGHT, 0x1,
				       BACKLIGHT_PARAM_POWER, BACKLIGHT_OFF);
		else
			call_fext_func(fext, FUNC_BACKLIGHT, 0x1,
				       BACKLIGHT_PARAM_POWER, BACKLIGHT_ON);
	}

	return set_lcd_level(device, b->props.brightness);
}

static const struct backlight_ops fujitsu_bl_ops = {
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	.get_brightness = bl_get_brightness,
	.update_status = bl_update_status,
};

<<<<<<< HEAD
/* Platform LCD brightness device */

static ssize_t
show_max_brightness(struct device *dev,
		    struct device_attribute *attr, char *buf)
{

	int ret;

	ret = get_max_brightness();
	if (ret < 0)
		return ret;

	return sprintf(buf, "%i\n", ret);
}

static ssize_t
show_brightness_changed(struct device *dev,
			struct device_attribute *attr, char *buf)
{

	int ret;

	ret = fujitsu->brightness_changed;
	if (ret < 0)
		return ret;

	return sprintf(buf, "%i\n", ret);
}

static ssize_t show_lcd_level(struct device *dev,
			      struct device_attribute *attr, char *buf)
{

	int ret;

	ret = get_lcd_level();
	if (ret < 0)
		return ret;

	return sprintf(buf, "%i\n", ret);
}

static ssize_t store_lcd_level(struct device *dev,
			       struct device_attribute *attr, const char *buf,
			       size_t count)
{

	int level, ret;

	if (sscanf(buf, "%i", &level) != 1
	    || (level < 0 || level >= fujitsu->max_brightness))
		return -EINVAL;

	if (use_alt_lcd_levels)
		ret = set_lcd_level_alt(level);
	else
		ret = set_lcd_level(level);
	if (ret < 0)
		return ret;

	ret = get_lcd_level();
	if (ret < 0)
		return ret;

	return count;
}

static ssize_t
ignore_store(struct device *dev,
	     struct device_attribute *attr, const char *buf, size_t count)
{
	return count;
}

static ssize_t
show_lid_state(struct device *dev,
			struct device_attribute *attr, char *buf)
{
	if (!(fujitsu_hotkey->rfkill_supported & 0x100))
		return sprintf(buf, "unknown\n");
	if (fujitsu_hotkey->rfkill_state & 0x100)
=======
static ssize_t lid_show(struct device *dev, struct device_attribute *attr,
			char *buf)
{
	struct fujitsu_laptop *priv = dev_get_drvdata(dev);

	if (!(priv->flags_supported & FLAG_LID))
		return sprintf(buf, "unknown\n");
	if (priv->flags_state & FLAG_LID)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		return sprintf(buf, "open\n");
	else
		return sprintf(buf, "closed\n");
}

<<<<<<< HEAD
static ssize_t
show_dock_state(struct device *dev,
			struct device_attribute *attr, char *buf)
{
	if (!(fujitsu_hotkey->rfkill_supported & 0x200))
		return sprintf(buf, "unknown\n");
	if (fujitsu_hotkey->rfkill_state & 0x200)
=======
static ssize_t dock_show(struct device *dev, struct device_attribute *attr,
			 char *buf)
{
	struct fujitsu_laptop *priv = dev_get_drvdata(dev);

	if (!(priv->flags_supported & FLAG_DOCK))
		return sprintf(buf, "unknown\n");
	if (priv->flags_state & FLAG_DOCK)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		return sprintf(buf, "docked\n");
	else
		return sprintf(buf, "undocked\n");
}

<<<<<<< HEAD
static ssize_t
show_radios_state(struct device *dev,
			struct device_attribute *attr, char *buf)
{
	if (!(fujitsu_hotkey->rfkill_supported & 0x20))
		return sprintf(buf, "unknown\n");
	if (fujitsu_hotkey->rfkill_state & 0x20)
=======
static ssize_t radios_show(struct device *dev, struct device_attribute *attr,
			   char *buf)
{
	struct fujitsu_laptop *priv = dev_get_drvdata(dev);

	if (!(priv->flags_supported & FLAG_RFKILL))
		return sprintf(buf, "unknown\n");
	if (priv->flags_state & FLAG_RFKILL)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		return sprintf(buf, "on\n");
	else
		return sprintf(buf, "killed\n");
}

<<<<<<< HEAD
static DEVICE_ATTR(max_brightness, 0444, show_max_brightness, ignore_store);
static DEVICE_ATTR(brightness_changed, 0444, show_brightness_changed,
		   ignore_store);
static DEVICE_ATTR(lcd_level, 0644, show_lcd_level, store_lcd_level);
static DEVICE_ATTR(lid, 0444, show_lid_state, ignore_store);
static DEVICE_ATTR(dock, 0444, show_dock_state, ignore_store);
static DEVICE_ATTR(radios, 0444, show_radios_state, ignore_store);

static struct attribute *fujitsupf_attributes[] = {
	&dev_attr_brightness_changed.attr,
	&dev_attr_max_brightness.attr,
	&dev_attr_lcd_level.attr,
=======
static DEVICE_ATTR_RO(lid);
static DEVICE_ATTR_RO(dock);
static DEVICE_ATTR_RO(radios);

static struct attribute *fujitsu_pf_attributes[] = {
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	&dev_attr_lid.attr,
	&dev_attr_dock.attr,
	&dev_attr_radios.attr,
	NULL
};

<<<<<<< HEAD
static struct attribute_group fujitsupf_attribute_group = {
	.attrs = fujitsupf_attributes
};

static struct platform_driver fujitsupf_driver = {
	.driver = {
		   .name = "fujitsu-laptop",
		   .owner = THIS_MODULE,
		   }
};

static void dmi_check_cb_common(const struct dmi_system_id *id)
{
	acpi_handle handle;
	pr_info("Identified laptop model '%s'\n", id->ident);
	if (use_alt_lcd_levels == -1) {
		if (ACPI_SUCCESS(acpi_get_handle(NULL,
				"\\_SB.PCI0.LPCB.FJEX.SBL2", &handle)))
			use_alt_lcd_levels = 1;
		else
			use_alt_lcd_levels = 0;
		vdbg_printk(FUJLAPTOP_DBG_TRACE, "auto-detected usealt as "
			"%i\n", use_alt_lcd_levels);
	}
}

static int dmi_check_cb_s6410(const struct dmi_system_id *id)
{
	dmi_check_cb_common(id);
	fujitsu->keycode1 = KEY_SCREENLOCK;	/* "Lock" */
	fujitsu->keycode2 = KEY_HELP;	/* "Mobility Center" */
	return 1;
}

static int dmi_check_cb_s6420(const struct dmi_system_id *id)
{
	dmi_check_cb_common(id);
	fujitsu->keycode1 = KEY_SCREENLOCK;	/* "Lock" */
	fujitsu->keycode2 = KEY_HELP;	/* "Mobility Center" */
	return 1;
}

static int dmi_check_cb_p8010(const struct dmi_system_id *id)
{
	dmi_check_cb_common(id);
	fujitsu->keycode1 = KEY_HELP;	/* "Support" */
	fujitsu->keycode3 = KEY_SWITCHVIDEOMODE;	/* "Presentation" */
	fujitsu->keycode4 = KEY_WWW;	/* "Internet" */
	return 1;
}

static struct dmi_system_id fujitsu_dmi_table[] = {
	{
	 .ident = "Fujitsu Siemens S6410",
	 .matches = {
		     DMI_MATCH(DMI_SYS_VENDOR, "FUJITSU SIEMENS"),
		     DMI_MATCH(DMI_PRODUCT_NAME, "LIFEBOOK S6410"),
		     },
	 .callback = dmi_check_cb_s6410},
	{
	 .ident = "Fujitsu Siemens S6420",
	 .matches = {
		     DMI_MATCH(DMI_SYS_VENDOR, "FUJITSU SIEMENS"),
		     DMI_MATCH(DMI_PRODUCT_NAME, "LIFEBOOK S6420"),
		     },
	 .callback = dmi_check_cb_s6420},
	{
	 .ident = "Fujitsu LifeBook P8010",
	 .matches = {
		     DMI_MATCH(DMI_SYS_VENDOR, "FUJITSU"),
		     DMI_MATCH(DMI_PRODUCT_NAME, "LifeBook P8010"),
		     },
	 .callback = dmi_check_cb_p8010},
	{}
};

/* ACPI device for LCD brightness control */

static int acpi_fujitsu_add(struct acpi_device *device)
{
	acpi_handle handle;
	int result = 0;
	int state = 0;
	struct input_dev *input;
	int error;

	if (!device)
		return -EINVAL;

	fujitsu->acpi_handle = device->handle;
	sprintf(acpi_device_name(device), "%s", ACPI_FUJITSU_DEVICE_NAME);
	sprintf(acpi_device_class(device), "%s", ACPI_FUJITSU_CLASS);
	device->driver_data = fujitsu;

	fujitsu->input = input = input_allocate_device();
	if (!input) {
		error = -ENOMEM;
		goto err_stop;
	}

	snprintf(fujitsu->phys, sizeof(fujitsu->phys),
		 "%s/video/input0", acpi_device_hid(device));

	input->name = acpi_device_name(device);
	input->phys = fujitsu->phys;
	input->id.bustype = BUS_HOST;
	input->id.product = 0x06;
	input->dev.parent = &device->dev;
	input->evbit[0] = BIT(EV_KEY);
	set_bit(KEY_BRIGHTNESSUP, input->keybit);
	set_bit(KEY_BRIGHTNESSDOWN, input->keybit);
	set_bit(KEY_UNKNOWN, input->keybit);

	error = input_register_device(input);
	if (error)
		goto err_free_input_dev;

	result = acpi_bus_update_power(fujitsu->acpi_handle, &state);
	if (result) {
		pr_err("Error reading power state\n");
		goto err_unregister_input_dev;
	}

	pr_info("ACPI: %s [%s] (%s)\n",
	       acpi_device_name(device), acpi_device_bid(device),
	       !device->power.state ? "on" : "off");

	fujitsu->dev = device;

	if (ACPI_SUCCESS
	    (acpi_get_handle(device->handle, METHOD_NAME__INI, &handle))) {
		vdbg_printk(FUJLAPTOP_DBG_INFO, "Invoking _INI\n");
		if (ACPI_FAILURE
		    (acpi_evaluate_object
		     (device->handle, METHOD_NAME__INI, NULL, NULL)))
			pr_err("_INI Method failed\n");
	}

	/* do config (detect defaults) */
	use_alt_lcd_levels = use_alt_lcd_levels == 1 ? 1 : 0;
	disable_brightness_adjust = disable_brightness_adjust == 1 ? 1 : 0;
	vdbg_printk(FUJLAPTOP_DBG_INFO,
		    "config: [alt interface: %d], [adjust disable: %d]\n",
		    use_alt_lcd_levels, disable_brightness_adjust);

	if (get_max_brightness() <= 0)
		fujitsu->max_brightness = FUJITSU_LCD_N_LEVELS;
	get_lcd_level();

	return result;

err_unregister_input_dev:
	input_unregister_device(input);
	input = NULL;
err_free_input_dev:
	input_free_device(input);
err_stop:
	return result;
}

static int acpi_fujitsu_remove(struct acpi_device *device, int type)
{
	struct fujitsu_t *fujitsu = acpi_driver_data(device);
	struct input_dev *input = fujitsu->input;

	input_unregister_device(input);

	fujitsu->acpi_handle = NULL;
=======
static const struct attribute_group fujitsu_pf_attribute_group = {
	.attrs = fujitsu_pf_attributes
};

static struct platform_driver fujitsu_pf_driver = {
	.driver = {
		   .name = "fujitsu-laptop",
		   }
};

/* ACPI device for LCD brightness control */

static const struct key_entry keymap_backlight[] = {
	{ KE_KEY, true, { KEY_BRIGHTNESSUP } },
	{ KE_KEY, false, { KEY_BRIGHTNESSDOWN } },
	{ KE_END, 0 }
};

static int acpi_fujitsu_bl_input_setup(struct acpi_device *device)
{
	struct fujitsu_bl *priv = acpi_driver_data(device);
	int ret;

	priv->input = devm_input_allocate_device(&device->dev);
	if (!priv->input)
		return -ENOMEM;

	snprintf(priv->phys, sizeof(priv->phys), "%s/video/input0",
		 acpi_device_hid(device));

	priv->input->name = acpi_device_name(device);
	priv->input->phys = priv->phys;
	priv->input->id.bustype = BUS_HOST;
	priv->input->id.product = 0x06;

	ret = sparse_keymap_setup(priv->input, keymap_backlight, NULL);
	if (ret)
		return ret;

	return input_register_device(priv->input);
}

static int fujitsu_backlight_register(struct acpi_device *device)
{
	struct fujitsu_bl *priv = acpi_driver_data(device);
	const struct backlight_properties props = {
		.brightness = priv->brightness_level,
		.max_brightness = priv->max_brightness - 1,
		.type = BACKLIGHT_PLATFORM
	};
	struct backlight_device *bd;

	bd = devm_backlight_device_register(&device->dev, "fujitsu-laptop",
					    &device->dev, device,
					    &fujitsu_bl_ops, &props);
	if (IS_ERR(bd))
		return PTR_ERR(bd);

	priv->bl_device = bd;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	return 0;
}

<<<<<<< HEAD
/* Brightness notify */

static void acpi_fujitsu_notify(struct acpi_device *device, u32 event)
{
	struct input_dev *input;
	int keycode;
	int oldb, newb;

	input = fujitsu->input;

	switch (event) {
	case ACPI_FUJITSU_NOTIFY_CODE1:
		keycode = 0;
		oldb = fujitsu->brightness_level;
		get_lcd_level();
		newb = fujitsu->brightness_level;

		vdbg_printk(FUJLAPTOP_DBG_TRACE,
			    "brightness button event [%i -> %i (%i)]\n",
			    oldb, newb, fujitsu->brightness_changed);

		if (oldb < newb) {
			if (disable_brightness_adjust != 1) {
				if (use_alt_lcd_levels)
					set_lcd_level_alt(newb);
				else
					set_lcd_level(newb);
			}
			acpi_bus_generate_proc_event(fujitsu->dev,
				ACPI_VIDEO_NOTIFY_INC_BRIGHTNESS, 0);
			keycode = KEY_BRIGHTNESSUP;
		} else if (oldb > newb) {
			if (disable_brightness_adjust != 1) {
				if (use_alt_lcd_levels)
					set_lcd_level_alt(newb);
				else
					set_lcd_level(newb);
			}
			acpi_bus_generate_proc_event(fujitsu->dev,
				ACPI_VIDEO_NOTIFY_DEC_BRIGHTNESS, 0);
			keycode = KEY_BRIGHTNESSDOWN;
		}
		break;
	default:
		keycode = KEY_UNKNOWN;
		vdbg_printk(FUJLAPTOP_DBG_WARN,
			    "unsupported event [0x%x]\n", event);
		break;
	}

	if (keycode != 0) {
		input_report_key(input, keycode, 1);
		input_sync(input);
		input_report_key(input, keycode, 0);
		input_sync(input);
	}
=======
static int acpi_fujitsu_bl_add(struct acpi_device *device)
{
	struct fujitsu_bl *priv;
	int ret;

	if (acpi_video_get_backlight_type() != acpi_backlight_vendor)
		return -ENODEV;

	priv = devm_kzalloc(&device->dev, sizeof(*priv), GFP_KERNEL);
	if (!priv)
		return -ENOMEM;

	fujitsu_bl = priv;
	strcpy(acpi_device_name(device), ACPI_FUJITSU_BL_DEVICE_NAME);
	strcpy(acpi_device_class(device), ACPI_FUJITSU_CLASS);
	device->driver_data = priv;

	pr_info("ACPI: %s [%s]\n",
		acpi_device_name(device), acpi_device_bid(device));

	if (get_max_brightness(device) <= 0)
		priv->max_brightness = FUJITSU_LCD_N_LEVELS;
	get_lcd_level(device);

	ret = acpi_fujitsu_bl_input_setup(device);
	if (ret)
		return ret;

	return fujitsu_backlight_register(device);
}

/* Brightness notify */

static void acpi_fujitsu_bl_notify(struct acpi_device *device, u32 event)
{
	struct fujitsu_bl *priv = acpi_driver_data(device);
	int oldb, newb;

	if (event != ACPI_FUJITSU_NOTIFY_CODE) {
		acpi_handle_info(device->handle, "unsupported event [0x%x]\n",
				 event);
		sparse_keymap_report_event(priv->input, -1, 1, true);
		return;
	}

	oldb = priv->brightness_level;
	get_lcd_level(device);
	newb = priv->brightness_level;

	acpi_handle_debug(device->handle,
			  "brightness button event [%i -> %i]\n", oldb, newb);

	if (oldb == newb)
		return;

	if (!disable_brightness_adjust)
		set_lcd_level(device, newb);

	sparse_keymap_report_event(priv->input, oldb < newb, 1, true);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

/* ACPI device for hotkey handling */

<<<<<<< HEAD
static int acpi_fujitsu_hotkey_add(struct acpi_device *device)
{
	acpi_handle handle;
	int result = 0;
	int state = 0;
	struct input_dev *input;
	int error;
	int i;

	if (!device)
		return -EINVAL;

	fujitsu_hotkey->acpi_handle = device->handle;
	sprintf(acpi_device_name(device), "%s",
		ACPI_FUJITSU_HOTKEY_DEVICE_NAME);
	sprintf(acpi_device_class(device), "%s", ACPI_FUJITSU_CLASS);
	device->driver_data = fujitsu_hotkey;

	/* kfifo */
	spin_lock_init(&fujitsu_hotkey->fifo_lock);
	error = kfifo_alloc(&fujitsu_hotkey->fifo, RINGBUFFERSIZE * sizeof(int),
			GFP_KERNEL);
	if (error) {
		pr_err("kfifo_alloc failed\n");
		goto err_stop;
	}

	fujitsu_hotkey->input = input = input_allocate_device();
	if (!input) {
		error = -ENOMEM;
		goto err_free_fifo;
	}

	snprintf(fujitsu_hotkey->phys, sizeof(fujitsu_hotkey->phys),
		 "%s/video/input0", acpi_device_hid(device));

	input->name = acpi_device_name(device);
	input->phys = fujitsu_hotkey->phys;
	input->id.bustype = BUS_HOST;
	input->id.product = 0x06;
	input->dev.parent = &device->dev;

	set_bit(EV_KEY, input->evbit);
	set_bit(fujitsu->keycode1, input->keybit);
	set_bit(fujitsu->keycode2, input->keybit);
	set_bit(fujitsu->keycode3, input->keybit);
	set_bit(fujitsu->keycode4, input->keybit);
	set_bit(KEY_UNKNOWN, input->keybit);

	error = input_register_device(input);
	if (error)
		goto err_free_input_dev;

	result = acpi_bus_update_power(fujitsu_hotkey->acpi_handle, &state);
	if (result) {
		pr_err("Error reading power state\n");
		goto err_unregister_input_dev;
	}

	pr_info("ACPI: %s [%s] (%s)\n",
		acpi_device_name(device), acpi_device_bid(device),
		!device->power.state ? "on" : "off");

	fujitsu_hotkey->dev = device;

	if (ACPI_SUCCESS
	    (acpi_get_handle(device->handle, METHOD_NAME__INI, &handle))) {
		vdbg_printk(FUJLAPTOP_DBG_INFO, "Invoking _INI\n");
		if (ACPI_FAILURE
		    (acpi_evaluate_object
		     (device->handle, METHOD_NAME__INI, NULL, NULL)))
			pr_err("_INI Method failed\n");
	}

	i = 0;
	while (call_fext_func(FUNC_BUTTONS, 0x1, 0x0, 0x0) != 0
		&& (i++) < MAX_HOTKEY_RINGBUFFER_SIZE)
		; /* No action, result is discarded */
	vdbg_printk(FUJLAPTOP_DBG_INFO, "Discarded %i ringbuffer entries\n", i);

	fujitsu_hotkey->rfkill_supported =
		call_fext_func(FUNC_RFKILL, 0x0, 0x0, 0x0);

	/* Make sure our bitmask of supported functions is cleared if the
	   RFKILL function block is not implemented, like on the S7020. */
	if (fujitsu_hotkey->rfkill_supported == UNSUPPORTED_CMD)
		fujitsu_hotkey->rfkill_supported = 0;

	if (fujitsu_hotkey->rfkill_supported)
		fujitsu_hotkey->rfkill_state =
			call_fext_func(FUNC_RFKILL, 0x4, 0x0, 0x0);

	/* Suspect this is a keymap of the application panel, print it */
	pr_info("BTNI: [0x%x]\n", call_fext_func(FUNC_BUTTONS, 0x0, 0x0, 0x0));

#if defined(CONFIG_LEDS_CLASS) || defined(CONFIG_LEDS_CLASS_MODULE)
	if (call_fext_func(FUNC_LEDS, 0x0, 0x0, 0x0) & LOGOLAMP_POWERON) {
		result = led_classdev_register(&fujitsu->pf_device->dev,
						&logolamp_led);
		if (result == 0) {
			fujitsu_hotkey->logolamp_registered = 1;
		} else {
			pr_err("Could not register LED handler for logo lamp, error %i\n",
			       result);
		}
	}

	if ((call_fext_func(FUNC_LEDS, 0x0, 0x0, 0x0) & KEYBOARD_LAMPS) &&
	   (call_fext_func(FUNC_BUTTONS, 0x0, 0x0, 0x0) == 0x0)) {
		result = led_classdev_register(&fujitsu->pf_device->dev,
						&kblamps_led);
		if (result == 0) {
			fujitsu_hotkey->kblamps_registered = 1;
		} else {
			pr_err("Could not register LED handler for keyboard lamps, error %i\n",
			       result);
		}
	}
#endif

	return result;

err_unregister_input_dev:
	input_unregister_device(input);
	input = NULL;
err_free_input_dev:
	input_free_device(input);
err_free_fifo:
	kfifo_free(&fujitsu_hotkey->fifo);
err_stop:
	return result;
}

static int acpi_fujitsu_hotkey_remove(struct acpi_device *device, int type)
{
	struct fujitsu_hotkey_t *fujitsu_hotkey = acpi_driver_data(device);
	struct input_dev *input = fujitsu_hotkey->input;

#if defined(CONFIG_LEDS_CLASS) || defined(CONFIG_LEDS_CLASS_MODULE)
	if (fujitsu_hotkey->logolamp_registered)
		led_classdev_unregister(&logolamp_led);

	if (fujitsu_hotkey->kblamps_registered)
		led_classdev_unregister(&kblamps_led);
#endif

	input_unregister_device(input);

	kfifo_free(&fujitsu_hotkey->fifo);

	fujitsu_hotkey->acpi_handle = NULL;
=======
static const struct key_entry keymap_default[] = {
	{ KE_KEY, KEY1_CODE,            { KEY_PROG1 } },
	{ KE_KEY, KEY2_CODE,            { KEY_PROG2 } },
	{ KE_KEY, KEY3_CODE,            { KEY_PROG3 } },
	{ KE_KEY, KEY4_CODE,            { KEY_PROG4 } },
	{ KE_KEY, KEY5_CODE,            { KEY_RFKILL } },
	/* Soft keys read from status flags */
	{ KE_KEY, FLAG_RFKILL,          { KEY_RFKILL } },
	{ KE_KEY, FLAG_TOUCHPAD_TOGGLE, { KEY_TOUCHPAD_TOGGLE } },
	{ KE_KEY, FLAG_MICMUTE,         { KEY_MICMUTE } },
	{ KE_END, 0 }
};

static const struct key_entry keymap_s64x0[] = {
	{ KE_KEY, KEY1_CODE, { KEY_SCREENLOCK } },	/* "Lock" */
	{ KE_KEY, KEY2_CODE, { KEY_HELP } },		/* "Mobility Center */
	{ KE_KEY, KEY3_CODE, { KEY_PROG3 } },
	{ KE_KEY, KEY4_CODE, { KEY_PROG4 } },
	{ KE_END, 0 }
};

static const struct key_entry keymap_p8010[] = {
	{ KE_KEY, KEY1_CODE, { KEY_HELP } },		/* "Support" */
	{ KE_KEY, KEY2_CODE, { KEY_PROG2 } },
	{ KE_KEY, KEY3_CODE, { KEY_SWITCHVIDEOMODE } },	/* "Presentation" */
	{ KE_KEY, KEY4_CODE, { KEY_WWW } },		/* "WWW" */
	{ KE_END, 0 }
};

static const struct key_entry *keymap = keymap_default;

static int fujitsu_laptop_dmi_keymap_override(const struct dmi_system_id *id)
{
	pr_info("Identified laptop model '%s'\n", id->ident);
	keymap = id->driver_data;
	return 1;
}

static const struct dmi_system_id fujitsu_laptop_dmi_table[] = {
	{
		.callback = fujitsu_laptop_dmi_keymap_override,
		.ident = "Fujitsu Siemens S6410",
		.matches = {
			DMI_MATCH(DMI_SYS_VENDOR, "FUJITSU SIEMENS"),
			DMI_MATCH(DMI_PRODUCT_NAME, "LIFEBOOK S6410"),
		},
		.driver_data = (void *)keymap_s64x0
	},
	{
		.callback = fujitsu_laptop_dmi_keymap_override,
		.ident = "Fujitsu Siemens S6420",
		.matches = {
			DMI_MATCH(DMI_SYS_VENDOR, "FUJITSU SIEMENS"),
			DMI_MATCH(DMI_PRODUCT_NAME, "LIFEBOOK S6420"),
		},
		.driver_data = (void *)keymap_s64x0
	},
	{
		.callback = fujitsu_laptop_dmi_keymap_override,
		.ident = "Fujitsu LifeBook P8010",
		.matches = {
			DMI_MATCH(DMI_SYS_VENDOR, "FUJITSU"),
			DMI_MATCH(DMI_PRODUCT_NAME, "LifeBook P8010"),
		},
		.driver_data = (void *)keymap_p8010
	},
	{}
};

static int acpi_fujitsu_laptop_input_setup(struct acpi_device *device)
{
	struct fujitsu_laptop *priv = acpi_driver_data(device);
	int ret;

	priv->input = devm_input_allocate_device(&device->dev);
	if (!priv->input)
		return -ENOMEM;

	snprintf(priv->phys, sizeof(priv->phys), "%s/input0",
		 acpi_device_hid(device));

	priv->input->name = acpi_device_name(device);
	priv->input->phys = priv->phys;
	priv->input->id.bustype = BUS_HOST;

	dmi_check_system(fujitsu_laptop_dmi_table);
	ret = sparse_keymap_setup(priv->input, keymap, NULL);
	if (ret)
		return ret;

	return input_register_device(priv->input);
}

static int fujitsu_laptop_platform_add(struct acpi_device *device)
{
	struct fujitsu_laptop *priv = acpi_driver_data(device);
	int ret;

	priv->pf_device = platform_device_alloc("fujitsu-laptop", PLATFORM_DEVID_NONE);
	if (!priv->pf_device)
		return -ENOMEM;

	platform_set_drvdata(priv->pf_device, priv);

	ret = platform_device_add(priv->pf_device);
	if (ret)
		goto err_put_platform_device;

	ret = sysfs_create_group(&priv->pf_device->dev.kobj,
				 &fujitsu_pf_attribute_group);
	if (ret)
		goto err_del_platform_device;

	return 0;

err_del_platform_device:
	platform_device_del(priv->pf_device);
err_put_platform_device:
	platform_device_put(priv->pf_device);

	return ret;
}

static void fujitsu_laptop_platform_remove(struct acpi_device *device)
{
	struct fujitsu_laptop *priv = acpi_driver_data(device);

	sysfs_remove_group(&priv->pf_device->dev.kobj,
			   &fujitsu_pf_attribute_group);
	platform_device_unregister(priv->pf_device);
}

static int logolamp_set(struct led_classdev *cdev,
			enum led_brightness brightness)
{
	struct acpi_device *device = to_acpi_device(cdev->dev->parent);
	int poweron = FUNC_LED_ON, always = FUNC_LED_ON;
	int ret;

	if (brightness < LED_HALF)
		poweron = FUNC_LED_OFF;

	if (brightness < LED_FULL)
		always = FUNC_LED_OFF;

	ret = call_fext_func(device, FUNC_LEDS, 0x1, LOGOLAMP_POWERON, poweron);
	if (ret < 0)
		return ret;

	return call_fext_func(device, FUNC_LEDS, 0x1, LOGOLAMP_ALWAYS, always);
}

static enum led_brightness logolamp_get(struct led_classdev *cdev)
{
	struct acpi_device *device = to_acpi_device(cdev->dev->parent);
	int ret;

	ret = call_fext_func(device, FUNC_LEDS, 0x2, LOGOLAMP_ALWAYS, 0x0);
	if (ret == FUNC_LED_ON)
		return LED_FULL;

	ret = call_fext_func(device, FUNC_LEDS, 0x2, LOGOLAMP_POWERON, 0x0);
	if (ret == FUNC_LED_ON)
		return LED_HALF;

	return LED_OFF;
}

static int kblamps_set(struct led_classdev *cdev,
		       enum led_brightness brightness)
{
	struct acpi_device *device = to_acpi_device(cdev->dev->parent);

	if (brightness >= LED_FULL)
		return call_fext_func(device, FUNC_LEDS, 0x1, KEYBOARD_LAMPS,
				      FUNC_LED_ON);
	else
		return call_fext_func(device, FUNC_LEDS, 0x1, KEYBOARD_LAMPS,
				      FUNC_LED_OFF);
}

static enum led_brightness kblamps_get(struct led_classdev *cdev)
{
	struct acpi_device *device = to_acpi_device(cdev->dev->parent);
	enum led_brightness brightness = LED_OFF;

	if (call_fext_func(device,
			   FUNC_LEDS, 0x2, KEYBOARD_LAMPS, 0x0) == FUNC_LED_ON)
		brightness = LED_FULL;

	return brightness;
}

static int radio_led_set(struct led_classdev *cdev,
			 enum led_brightness brightness)
{
	struct acpi_device *device = to_acpi_device(cdev->dev->parent);

	if (brightness >= LED_FULL)
		return call_fext_func(device, FUNC_FLAGS, 0x5, RADIO_LED_ON,
				      RADIO_LED_ON);
	else
		return call_fext_func(device, FUNC_FLAGS, 0x5, RADIO_LED_ON,
				      0x0);
}

static enum led_brightness radio_led_get(struct led_classdev *cdev)
{
	struct acpi_device *device = to_acpi_device(cdev->dev->parent);
	enum led_brightness brightness = LED_OFF;

	if (call_fext_func(device, FUNC_FLAGS, 0x4, 0x0, 0x0) & RADIO_LED_ON)
		brightness = LED_FULL;

	return brightness;
}

static int eco_led_set(struct led_classdev *cdev,
		       enum led_brightness brightness)
{
	struct acpi_device *device = to_acpi_device(cdev->dev->parent);
	int curr;

	curr = call_fext_func(device, FUNC_LEDS, 0x2, ECO_LED, 0x0);
	if (brightness >= LED_FULL)
		return call_fext_func(device, FUNC_LEDS, 0x1, ECO_LED,
				      curr | ECO_LED_ON);
	else
		return call_fext_func(device, FUNC_LEDS, 0x1, ECO_LED,
				      curr & ~ECO_LED_ON);
}

static enum led_brightness eco_led_get(struct led_classdev *cdev)
{
	struct acpi_device *device = to_acpi_device(cdev->dev->parent);
	enum led_brightness brightness = LED_OFF;

	if (call_fext_func(device, FUNC_LEDS, 0x2, ECO_LED, 0x0) & ECO_LED_ON)
		brightness = LED_FULL;

	return brightness;
}

static int acpi_fujitsu_laptop_leds_register(struct acpi_device *device)
{
	struct fujitsu_laptop *priv = acpi_driver_data(device);
	struct led_classdev *led;
	int ret;

	if (call_fext_func(device,
			   FUNC_LEDS, 0x0, 0x0, 0x0) & LOGOLAMP_POWERON) {
		led = devm_kzalloc(&device->dev, sizeof(*led), GFP_KERNEL);
		if (!led)
			return -ENOMEM;

		led->name = "fujitsu::logolamp";
		led->brightness_set_blocking = logolamp_set;
		led->brightness_get = logolamp_get;
		ret = devm_led_classdev_register(&device->dev, led);
		if (ret)
			return ret;
	}

	if ((call_fext_func(device,
			    FUNC_LEDS, 0x0, 0x0, 0x0) & KEYBOARD_LAMPS) &&
	    (call_fext_func(device, FUNC_BUTTONS, 0x0, 0x0, 0x0) == 0x0)) {
		led = devm_kzalloc(&device->dev, sizeof(*led), GFP_KERNEL);
		if (!led)
			return -ENOMEM;

		led->name = "fujitsu::kblamps";
		led->brightness_set_blocking = kblamps_set;
		led->brightness_get = kblamps_get;
		ret = devm_led_classdev_register(&device->dev, led);
		if (ret)
			return ret;
	}

	/*
	 * Some Fujitsu laptops have a radio toggle button in place of a slide
	 * switch and all such machines appear to also have an RF LED.  Based on
	 * comparing DSDT tables of four Fujitsu Lifebook models (E744, E751,
	 * S7110, S8420; the first one has a radio toggle button, the other
	 * three have slide switches), bit 17 of flags_supported (the value
	 * returned by method S000 of ACPI device FUJ02E3) seems to indicate
	 * whether given model has a radio toggle button.
	 */
	if (priv->flags_supported & BIT(17)) {
		led = devm_kzalloc(&device->dev, sizeof(*led), GFP_KERNEL);
		if (!led)
			return -ENOMEM;

		led->name = "fujitsu::radio_led";
		led->brightness_set_blocking = radio_led_set;
		led->brightness_get = radio_led_get;
		led->default_trigger = "rfkill-any";
		ret = devm_led_classdev_register(&device->dev, led);
		if (ret)
			return ret;
	}

	/* Support for eco led is not always signaled in bit corresponding
	 * to the bit used to control the led. According to the DSDT table,
	 * bit 14 seems to indicate presence of said led as well.
	 * Confirm by testing the status.
	 */
	if ((call_fext_func(device, FUNC_LEDS, 0x0, 0x0, 0x0) & BIT(14)) &&
	    (call_fext_func(device,
			    FUNC_LEDS, 0x2, ECO_LED, 0x0) != UNSUPPORTED_CMD)) {
		led = devm_kzalloc(&device->dev, sizeof(*led), GFP_KERNEL);
		if (!led)
			return -ENOMEM;

		led->name = "fujitsu::eco_led";
		led->brightness_set_blocking = eco_led_set;
		led->brightness_get = eco_led_get;
		ret = devm_led_classdev_register(&device->dev, led);
		if (ret)
			return ret;
	}
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	return 0;
}

<<<<<<< HEAD
static void acpi_fujitsu_hotkey_notify(struct acpi_device *device, u32 event)
{
	struct input_dev *input;
	int keycode, keycode_r;
	unsigned int irb = 1;
	int i, status;

	input = fujitsu_hotkey->input;

	if (fujitsu_hotkey->rfkill_supported)
		fujitsu_hotkey->rfkill_state =
			call_fext_func(FUNC_RFKILL, 0x4, 0x0, 0x0);

	switch (event) {
	case ACPI_FUJITSU_NOTIFY_CODE1:
		i = 0;
		while ((irb =
			call_fext_func(FUNC_BUTTONS, 0x1, 0x0, 0x0)) != 0
				&& (i++) < MAX_HOTKEY_RINGBUFFER_SIZE) {
			switch (irb & 0x4ff) {
			case KEY1_CODE:
				keycode = fujitsu->keycode1;
				break;
			case KEY2_CODE:
				keycode = fujitsu->keycode2;
				break;
			case KEY3_CODE:
				keycode = fujitsu->keycode3;
				break;
			case KEY4_CODE:
				keycode = fujitsu->keycode4;
				break;
			case 0:
				keycode = 0;
				break;
			default:
				vdbg_printk(FUJLAPTOP_DBG_WARN,
					    "Unknown GIRB result [%x]\n", irb);
				keycode = -1;
				break;
			}
			if (keycode > 0) {
				vdbg_printk(FUJLAPTOP_DBG_TRACE,
					"Push keycode into ringbuffer [%d]\n",
					keycode);
				status = kfifo_in_locked(&fujitsu_hotkey->fifo,
						   (unsigned char *)&keycode,
						   sizeof(keycode),
						   &fujitsu_hotkey->fifo_lock);
				if (status != sizeof(keycode)) {
					vdbg_printk(FUJLAPTOP_DBG_WARN,
					    "Could not push keycode [0x%x]\n",
					    keycode);
				} else {
					input_report_key(input, keycode, 1);
					input_sync(input);
				}
			} else if (keycode == 0) {
				while ((status =
					kfifo_out_locked(
					 &fujitsu_hotkey->fifo,
					 (unsigned char *) &keycode_r,
					 sizeof(keycode_r),
					 &fujitsu_hotkey->fifo_lock))
					 == sizeof(keycode_r)) {
					input_report_key(input, keycode_r, 0);
					input_sync(input);
					vdbg_printk(FUJLAPTOP_DBG_TRACE,
					  "Pop keycode from ringbuffer [%d]\n",
					  keycode_r);
				}
			}
		}

		break;
	default:
		keycode = KEY_UNKNOWN;
		vdbg_printk(FUJLAPTOP_DBG_WARN,
			    "Unsupported event [0x%x]\n", event);
		input_report_key(input, keycode, 1);
		input_sync(input);
		input_report_key(input, keycode, 0);
		input_sync(input);
		break;
=======
static int acpi_fujitsu_laptop_add(struct acpi_device *device)
{
	struct fujitsu_laptop *priv;
	int ret, i = 0;

	priv = devm_kzalloc(&device->dev, sizeof(*priv), GFP_KERNEL);
	if (!priv)
		return -ENOMEM;

	WARN_ONCE(fext, "More than one FUJ02E3 ACPI device was found.  Driver may not work as intended.");
	fext = device;

	strcpy(acpi_device_name(device), ACPI_FUJITSU_LAPTOP_DEVICE_NAME);
	strcpy(acpi_device_class(device), ACPI_FUJITSU_CLASS);
	device->driver_data = priv;

	/* kfifo */
	spin_lock_init(&priv->fifo_lock);
	ret = kfifo_alloc(&priv->fifo, RINGBUFFERSIZE * sizeof(int),
			  GFP_KERNEL);
	if (ret)
		return ret;

	pr_info("ACPI: %s [%s]\n",
		acpi_device_name(device), acpi_device_bid(device));

	while (call_fext_func(device, FUNC_BUTTONS, 0x1, 0x0, 0x0) != 0 &&
	       i++ < MAX_HOTKEY_RINGBUFFER_SIZE)
		; /* No action, result is discarded */
	acpi_handle_debug(device->handle, "Discarded %i ringbuffer entries\n",
			  i);

	priv->flags_supported = call_fext_func(device, FUNC_FLAGS, 0x0, 0x0,
					       0x0);

	/* Make sure our bitmask of supported functions is cleared if the
	   RFKILL function block is not implemented, like on the S7020. */
	if (priv->flags_supported == UNSUPPORTED_CMD)
		priv->flags_supported = 0;

	if (priv->flags_supported)
		priv->flags_state = call_fext_func(device, FUNC_FLAGS, 0x4, 0x0,
						   0x0);

	/* Suspect this is a keymap of the application panel, print it */
	acpi_handle_info(device->handle, "BTNI: [0x%x]\n",
			 call_fext_func(device, FUNC_BUTTONS, 0x0, 0x0, 0x0));

	/* Sync backlight power status */
	if (fujitsu_bl && fujitsu_bl->bl_device &&
	    acpi_video_get_backlight_type() == acpi_backlight_vendor) {
		if (call_fext_func(fext, FUNC_BACKLIGHT, 0x2,
				   BACKLIGHT_PARAM_POWER, 0x0) == BACKLIGHT_OFF)
			fujitsu_bl->bl_device->props.power = FB_BLANK_POWERDOWN;
		else
			fujitsu_bl->bl_device->props.power = FB_BLANK_UNBLANK;
	}

	ret = acpi_fujitsu_laptop_input_setup(device);
	if (ret)
		goto err_free_fifo;

	ret = acpi_fujitsu_laptop_leds_register(device);
	if (ret)
		goto err_free_fifo;

	ret = fujitsu_laptop_platform_add(device);
	if (ret)
		goto err_free_fifo;

	ret = fujitsu_battery_charge_control_add(device);
	if (ret < 0)
		pr_warn("Unable to register battery charge control: %d\n", ret);

	return 0;

err_free_fifo:
	kfifo_free(&priv->fifo);

	return ret;
}

static void acpi_fujitsu_laptop_remove(struct acpi_device *device)
{
	struct fujitsu_laptop *priv = acpi_driver_data(device);

	fujitsu_battery_charge_control_remove(device);

	fujitsu_laptop_platform_remove(device);

	kfifo_free(&priv->fifo);
}

static void acpi_fujitsu_laptop_press(struct acpi_device *device, int scancode)
{
	struct fujitsu_laptop *priv = acpi_driver_data(device);
	int ret;

	ret = kfifo_in_locked(&priv->fifo, (unsigned char *)&scancode,
			      sizeof(scancode), &priv->fifo_lock);
	if (ret != sizeof(scancode)) {
		dev_info(&priv->input->dev, "Could not push scancode [0x%x]\n",
			 scancode);
		return;
	}
	sparse_keymap_report_event(priv->input, scancode, 1, false);
	dev_dbg(&priv->input->dev, "Push scancode into ringbuffer [0x%x]\n",
		scancode);
}

static void acpi_fujitsu_laptop_release(struct acpi_device *device)
{
	struct fujitsu_laptop *priv = acpi_driver_data(device);
	int scancode, ret;

	while (true) {
		ret = kfifo_out_locked(&priv->fifo, (unsigned char *)&scancode,
				       sizeof(scancode), &priv->fifo_lock);
		if (ret != sizeof(scancode))
			return;
		sparse_keymap_report_event(priv->input, scancode, 0, false);
		dev_dbg(&priv->input->dev,
			"Pop scancode from ringbuffer [0x%x]\n", scancode);
	}
}

static void acpi_fujitsu_laptop_notify(struct acpi_device *device, u32 event)
{
	struct fujitsu_laptop *priv = acpi_driver_data(device);
	unsigned long flags;
	int scancode, i = 0;
	unsigned int irb;

	if (event != ACPI_FUJITSU_NOTIFY_CODE) {
		acpi_handle_info(device->handle, "Unsupported event [0x%x]\n",
				 event);
		sparse_keymap_report_event(priv->input, -1, 1, true);
		return;
	}

	if (priv->flags_supported)
		priv->flags_state = call_fext_func(device, FUNC_FLAGS, 0x4, 0x0,
						   0x0);

	while ((irb = call_fext_func(device,
				     FUNC_BUTTONS, 0x1, 0x0, 0x0)) != 0 &&
	       i++ < MAX_HOTKEY_RINGBUFFER_SIZE) {
		scancode = irb & 0x4ff;
		if (sparse_keymap_entry_from_scancode(priv->input, scancode))
			acpi_fujitsu_laptop_press(device, scancode);
		else if (scancode == 0)
			acpi_fujitsu_laptop_release(device);
		else
			acpi_handle_info(device->handle,
					 "Unknown GIRB result [%x]\n", irb);
	}

	/*
	 * First seen on the Skylake-based Lifebook E736/E746/E756), the
	 * touchpad toggle hotkey (Fn+F4) is handled in software. Other models
	 * have since added additional "soft keys". These are reported in the
	 * status flags queried using FUNC_FLAGS.
	 */
	if (priv->flags_supported & (FLAG_SOFTKEYS)) {
		flags = call_fext_func(device, FUNC_FLAGS, 0x1, 0x0, 0x0);
		flags &= (FLAG_SOFTKEYS);
		for_each_set_bit(i, &flags, BITS_PER_LONG)
			sparse_keymap_report_event(priv->input, BIT(i), 1, true);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	}
}

/* Initialization */

<<<<<<< HEAD
static const struct acpi_device_id fujitsu_device_ids[] = {
	{ACPI_FUJITSU_HID, 0},
	{"", 0},
};

static struct acpi_driver acpi_fujitsu_driver = {
	.name = ACPI_FUJITSU_DRIVER_NAME,
	.class = ACPI_FUJITSU_CLASS,
	.ids = fujitsu_device_ids,
	.ops = {
		.add = acpi_fujitsu_add,
		.remove = acpi_fujitsu_remove,
		.notify = acpi_fujitsu_notify,
		},
};

static const struct acpi_device_id fujitsu_hotkey_device_ids[] = {
	{ACPI_FUJITSU_HOTKEY_HID, 0},
	{"", 0},
};

static struct acpi_driver acpi_fujitsu_hotkey_driver = {
	.name = ACPI_FUJITSU_HOTKEY_DRIVER_NAME,
	.class = ACPI_FUJITSU_CLASS,
	.ids = fujitsu_hotkey_device_ids,
	.ops = {
		.add = acpi_fujitsu_hotkey_add,
		.remove = acpi_fujitsu_hotkey_remove,
		.notify = acpi_fujitsu_hotkey_notify,
		},
};

static int __init fujitsu_init(void)
{
	int ret, result, max_brightness;

	if (acpi_disabled)
		return -ENODEV;

	fujitsu = kzalloc(sizeof(struct fujitsu_t), GFP_KERNEL);
	if (!fujitsu)
		return -ENOMEM;
	fujitsu->keycode1 = KEY_PROG1;
	fujitsu->keycode2 = KEY_PROG2;
	fujitsu->keycode3 = KEY_PROG3;
	fujitsu->keycode4 = KEY_PROG4;
	dmi_check_system(fujitsu_dmi_table);

	result = acpi_bus_register_driver(&acpi_fujitsu_driver);
	if (result < 0) {
		ret = -ENODEV;
		goto fail_acpi;
	}

	/* Register platform stuff */

	fujitsu->pf_device = platform_device_alloc("fujitsu-laptop", -1);
	if (!fujitsu->pf_device) {
		ret = -ENOMEM;
		goto fail_platform_driver;
	}

	ret = platform_device_add(fujitsu->pf_device);
	if (ret)
		goto fail_platform_device1;

	ret =
	    sysfs_create_group(&fujitsu->pf_device->dev.kobj,
			       &fujitsupf_attribute_group);
	if (ret)
		goto fail_platform_device2;

	/* Register backlight stuff */

	if (!acpi_video_backlight_support()) {
		struct backlight_properties props;

		memset(&props, 0, sizeof(struct backlight_properties));
		max_brightness = fujitsu->max_brightness;
		props.type = BACKLIGHT_PLATFORM;
		props.max_brightness = max_brightness - 1;
		fujitsu->bl_device = backlight_device_register("fujitsu-laptop",
							       NULL, NULL,
							       &fujitsubl_ops,
							       &props);
		if (IS_ERR(fujitsu->bl_device)) {
			ret = PTR_ERR(fujitsu->bl_device);
			fujitsu->bl_device = NULL;
			goto fail_sysfs_group;
		}
		fujitsu->bl_device->props.brightness = fujitsu->brightness_level;
	}

	ret = platform_driver_register(&fujitsupf_driver);
	if (ret)
		goto fail_backlight;

	/* Register hotkey driver */

	fujitsu_hotkey = kzalloc(sizeof(struct fujitsu_hotkey_t), GFP_KERNEL);
	if (!fujitsu_hotkey) {
		ret = -ENOMEM;
		goto fail_hotkey;
	}

	result = acpi_bus_register_driver(&acpi_fujitsu_hotkey_driver);
	if (result < 0) {
		ret = -ENODEV;
		goto fail_hotkey1;
	}

	/* Sync backlight power status (needs FUJ02E3 device, hence deferred) */

	if (!acpi_video_backlight_support()) {
		if (call_fext_func(FUNC_BACKLIGHT, 0x2, 0x4, 0x0) == 3)
			fujitsu->bl_device->props.power = 4;
		else
			fujitsu->bl_device->props.power = 0;
	}
=======
static const struct acpi_device_id fujitsu_bl_device_ids[] = {
	{ACPI_FUJITSU_BL_HID, 0},
	{"", 0},
};

static struct acpi_driver acpi_fujitsu_bl_driver = {
	.name = ACPI_FUJITSU_BL_DRIVER_NAME,
	.class = ACPI_FUJITSU_CLASS,
	.ids = fujitsu_bl_device_ids,
	.ops = {
		.add = acpi_fujitsu_bl_add,
		.notify = acpi_fujitsu_bl_notify,
		},
};

static const struct acpi_device_id fujitsu_laptop_device_ids[] = {
	{ACPI_FUJITSU_LAPTOP_HID, 0},
	{"", 0},
};

static struct acpi_driver acpi_fujitsu_laptop_driver = {
	.name = ACPI_FUJITSU_LAPTOP_DRIVER_NAME,
	.class = ACPI_FUJITSU_CLASS,
	.ids = fujitsu_laptop_device_ids,
	.ops = {
		.add = acpi_fujitsu_laptop_add,
		.remove = acpi_fujitsu_laptop_remove,
		.notify = acpi_fujitsu_laptop_notify,
		},
};

static const struct acpi_device_id fujitsu_ids[] __used = {
	{ACPI_FUJITSU_BL_HID, 0},
	{ACPI_FUJITSU_LAPTOP_HID, 0},
	{"", 0}
};
MODULE_DEVICE_TABLE(acpi, fujitsu_ids);

static int __init fujitsu_init(void)
{
	int ret;

	ret = acpi_bus_register_driver(&acpi_fujitsu_bl_driver);
	if (ret)
		return ret;

	/* Register platform stuff */

	ret = platform_driver_register(&fujitsu_pf_driver);
	if (ret)
		goto err_unregister_acpi;

	/* Register laptop driver */

	ret = acpi_bus_register_driver(&acpi_fujitsu_laptop_driver);
	if (ret)
		goto err_unregister_platform_driver;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	pr_info("driver " FUJITSU_DRIVER_VERSION " successfully loaded\n");

	return 0;

<<<<<<< HEAD
fail_hotkey1:
	kfree(fujitsu_hotkey);
fail_hotkey:
	platform_driver_unregister(&fujitsupf_driver);
fail_backlight:
	if (fujitsu->bl_device)
		backlight_device_unregister(fujitsu->bl_device);
fail_sysfs_group:
	sysfs_remove_group(&fujitsu->pf_device->dev.kobj,
			   &fujitsupf_attribute_group);
fail_platform_device2:
	platform_device_del(fujitsu->pf_device);
fail_platform_device1:
	platform_device_put(fujitsu->pf_device);
fail_platform_driver:
	acpi_bus_unregister_driver(&acpi_fujitsu_driver);
fail_acpi:
	kfree(fujitsu);
=======
err_unregister_platform_driver:
	platform_driver_unregister(&fujitsu_pf_driver);
err_unregister_acpi:
	acpi_bus_unregister_driver(&acpi_fujitsu_bl_driver);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	return ret;
}

static void __exit fujitsu_cleanup(void)
{
<<<<<<< HEAD
	acpi_bus_unregister_driver(&acpi_fujitsu_hotkey_driver);

	kfree(fujitsu_hotkey);

	platform_driver_unregister(&fujitsupf_driver);

	if (fujitsu->bl_device)
		backlight_device_unregister(fujitsu->bl_device);

	sysfs_remove_group(&fujitsu->pf_device->dev.kobj,
			   &fujitsupf_attribute_group);

	platform_device_unregister(fujitsu->pf_device);

	acpi_bus_unregister_driver(&acpi_fujitsu_driver);

	kfree(fujitsu);
=======
	acpi_bus_unregister_driver(&acpi_fujitsu_laptop_driver);

	platform_driver_unregister(&fujitsu_pf_driver);

	acpi_bus_unregister_driver(&acpi_fujitsu_bl_driver);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	pr_info("driver unloaded\n");
}

module_init(fujitsu_init);
module_exit(fujitsu_cleanup);

<<<<<<< HEAD
module_param(use_alt_lcd_levels, uint, 0644);
MODULE_PARM_DESC(use_alt_lcd_levels,
		 "Use alternative interface for lcd_levels (needed for Lifebook s6410).");
module_param(disable_brightness_adjust, uint, 0644);
MODULE_PARM_DESC(disable_brightness_adjust, "Disable brightness adjustment .");
#ifdef CONFIG_FUJITSU_LAPTOP_DEBUG
module_param_named(debug, dbg_level, uint, 0644);
MODULE_PARM_DESC(debug, "Sets debug level bit-mask");
#endif
=======
module_param(use_alt_lcd_levels, int, 0644);
MODULE_PARM_DESC(use_alt_lcd_levels, "Interface used for setting LCD brightness level (-1 = auto, 0 = force SBLL, 1 = force SBL2)");
module_param(disable_brightness_adjust, bool, 0644);
MODULE_PARM_DESC(disable_brightness_adjust, "Disable LCD brightness adjustment");
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

MODULE_AUTHOR("Jonathan Woithe, Peter Gruber, Tony Vroon");
MODULE_DESCRIPTION("Fujitsu laptop extras support");
MODULE_VERSION(FUJITSU_DRIVER_VERSION);
MODULE_LICENSE("GPL");
<<<<<<< HEAD

MODULE_ALIAS("dmi:*:svnFUJITSUSIEMENS:*:pvr:rvnFUJITSU:rnFJNB1D3:*:cvrS6410:*");
MODULE_ALIAS("dmi:*:svnFUJITSUSIEMENS:*:pvr:rvnFUJITSU:rnFJNB1E6:*:cvrS6420:*");
MODULE_ALIAS("dmi:*:svnFUJITSU:*:pvr:rvnFUJITSU:rnFJNB19C:*:cvrS7020:*");

static struct pnp_device_id pnp_ids[] __used = {
	{.id = "FUJ02bf"},
	{.id = "FUJ02B1"},
	{.id = "FUJ02E3"},
	{.id = ""}
};

MODULE_DEVICE_TABLE(pnp, pnp_ids);
=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
