<<<<<<< HEAD
=======
// SPDX-License-Identifier: GPL-2.0-or-later
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
/*
 *  acpi_thermal.c - ACPI Thermal Zone Driver ($Revision: 41 $)
 *
 *  Copyright (C) 2001, 2002 Andy Grover <andrew.grover@intel.com>
 *  Copyright (C) 2001, 2002 Paul Diefenbaugh <paul.s.diefenbaugh@intel.com>
 *
<<<<<<< HEAD
 * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or (at
 *  your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful, but
 *  WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 *  General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License along
 *  with this program; if not, write to the Free Software Foundation, Inc.,
 *  59 Temple Place, Suite 330, Boston, MA 02111-1307 USA.
 *
 * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 *
=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 *  This driver fully implements the ACPI thermal policy as described in the
 *  ACPI 2.0 Specification.
 *
 *  TBD: 1. Implement passive cooling hysteresis.
 *       2. Enhance passive cooling (CPU) states/limit interface to support
 *          concepts of 'multiple limiters', upper/lower limits, etc.
<<<<<<< HEAD
 *
 */

=======
 */

#define pr_fmt(fmt) "ACPI: thermal: " fmt

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/dmi.h>
#include <linux/init.h>
#include <linux/slab.h>
#include <linux/types.h>
#include <linux/jiffies.h>
#include <linux/kmod.h>
#include <linux/reboot.h>
#include <linux/device.h>
<<<<<<< HEAD
#include <asm/uaccess.h>
#include <linux/thermal.h>
#include <acpi/acpi_bus.h>
#include <acpi/acpi_drivers.h>

#define PREFIX "ACPI: "

#define ACPI_THERMAL_CLASS		"thermal_zone"
#define ACPI_THERMAL_DEVICE_NAME	"Thermal Zone"
#define ACPI_THERMAL_FILE_STATE		"state"
#define ACPI_THERMAL_FILE_TEMPERATURE	"temperature"
#define ACPI_THERMAL_FILE_TRIP_POINTS	"trip_points"
#define ACPI_THERMAL_FILE_COOLING_MODE	"cooling_mode"
#define ACPI_THERMAL_FILE_POLLING_FREQ	"polling_frequency"
=======
#include <linux/thermal.h>
#include <linux/acpi.h>
#include <linux/workqueue.h>
#include <linux/uaccess.h>
#include <linux/units.h>

#include "internal.h"

#define ACPI_THERMAL_CLASS		"thermal_zone"
#define ACPI_THERMAL_DEVICE_NAME	"Thermal Zone"
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#define ACPI_THERMAL_NOTIFY_TEMPERATURE	0x80
#define ACPI_THERMAL_NOTIFY_THRESHOLDS	0x81
#define ACPI_THERMAL_NOTIFY_DEVICES	0x82
#define ACPI_THERMAL_NOTIFY_CRITICAL	0xF0
#define ACPI_THERMAL_NOTIFY_HOT		0xF1
#define ACPI_THERMAL_MODE_ACTIVE	0x00

<<<<<<< HEAD
#define ACPI_THERMAL_MAX_ACTIVE	10
#define ACPI_THERMAL_MAX_LIMIT_STR_LEN 65

#define _COMPONENT		ACPI_THERMAL_COMPONENT
ACPI_MODULE_NAME("thermal");

MODULE_AUTHOR("Paul Diefenbaugh");
MODULE_DESCRIPTION("ACPI Thermal Zone Driver");
MODULE_LICENSE("GPL");
=======
#define ACPI_THERMAL_MAX_ACTIVE		10
#define ACPI_THERMAL_MAX_LIMIT_STR_LEN	65

#define ACPI_THERMAL_TRIP_PASSIVE	(-1)

#define ACPI_THERMAL_MAX_NR_TRIPS	(ACPI_THERMAL_MAX_ACTIVE + 3)

/*
 * This exception is thrown out in two cases:
 * 1.An invalid trip point becomes invalid or a valid trip point becomes invalid
 *   when re-evaluating the AML code.
 * 2.TODO: Devices listed in _PSL, _ALx, _TZD may change.
 *   We need to re-bind the cooling devices of a thermal zone when this occurs.
 */
#define ACPI_THERMAL_TRIPS_EXCEPTION(tz, str) \
do { \
	acpi_handle_info(tz->device->handle, \
			 "ACPI thermal trip point %s changed\n" \
			 "Please report to linux-acpi@vger.kernel.org\n", str); \
} while (0)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

static int act;
module_param(act, int, 0644);
MODULE_PARM_DESC(act, "Disable or override all lowest active trip points.");

static int crt;
module_param(crt, int, 0644);
MODULE_PARM_DESC(crt, "Disable or lower all critical trip points.");

static int tzp;
module_param(tzp, int, 0444);
MODULE_PARM_DESC(tzp, "Thermal zone polling frequency, in 1/10 seconds.");

<<<<<<< HEAD
static int nocrt;
module_param(nocrt, int, 0);
MODULE_PARM_DESC(nocrt, "Set to take no action upon ACPI thermal zone critical trips points.");

=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
static int off;
module_param(off, int, 0);
MODULE_PARM_DESC(off, "Set to disable ACPI thermal support.");

static int psv;
module_param(psv, int, 0644);
MODULE_PARM_DESC(psv, "Disable or override all passive trip points.");

<<<<<<< HEAD
static int acpi_thermal_add(struct acpi_device *device);
static int acpi_thermal_remove(struct acpi_device *device, int type);
static int acpi_thermal_resume(struct acpi_device *device);
static void acpi_thermal_notify(struct acpi_device *device, u32 event);

static const struct acpi_device_id  thermal_device_ids[] = {
	{ACPI_THERMAL_HID, 0},
	{"", 0},
};
MODULE_DEVICE_TABLE(acpi, thermal_device_ids);

static struct acpi_driver acpi_thermal_driver = {
	.name = "thermal",
	.class = ACPI_THERMAL_CLASS,
	.ids = thermal_device_ids,
	.ops = {
		.add = acpi_thermal_add,
		.remove = acpi_thermal_remove,
		.resume = acpi_thermal_resume,
		.notify = acpi_thermal_notify,
		},
};

struct acpi_thermal_state {
	u8 critical:1;
	u8 hot:1;
	u8 passive:1;
	u8 active:1;
	u8 reserved:4;
	int active_index;
};

struct acpi_thermal_state_flags {
	u8 valid:1;
	u8 enabled:1;
	u8 reserved:6;
};

struct acpi_thermal_critical {
	struct acpi_thermal_state_flags flags;
	unsigned long temperature;
};

struct acpi_thermal_hot {
	struct acpi_thermal_state_flags flags;
	unsigned long temperature;
};

struct acpi_thermal_passive {
	struct acpi_thermal_state_flags flags;
	unsigned long temperature;
	unsigned long tc1;
	unsigned long tc2;
	unsigned long tsp;
	struct acpi_handle_list devices;
};

struct acpi_thermal_active {
	struct acpi_thermal_state_flags flags;
	unsigned long temperature;
	struct acpi_handle_list devices;
};

struct acpi_thermal_trips {
	struct acpi_thermal_critical critical;
	struct acpi_thermal_hot hot;
=======
static struct workqueue_struct *acpi_thermal_pm_queue;

struct acpi_thermal_trip {
	unsigned long temp_dk;
	struct acpi_handle_list devices;
};

struct acpi_thermal_passive {
	struct acpi_thermal_trip trip;
	unsigned long tc1;
	unsigned long tc2;
	unsigned long delay;
};

struct acpi_thermal_active {
	struct acpi_thermal_trip trip;
};

struct acpi_thermal_trips {
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	struct acpi_thermal_passive passive;
	struct acpi_thermal_active active[ACPI_THERMAL_MAX_ACTIVE];
};

<<<<<<< HEAD
struct acpi_thermal_flags {
	u8 cooling_mode:1;	/* _SCP */
	u8 devices:1;		/* _TZD */
	u8 reserved:6;
};

struct acpi_thermal {
	struct acpi_device * device;
	acpi_bus_id name;
	unsigned long temperature;
	unsigned long last_temperature;
	unsigned long polling_frequency;
	volatile u8 zombie;
	struct acpi_thermal_flags flags;
	struct acpi_thermal_state state;
	struct acpi_thermal_trips trips;
	struct acpi_handle_list devices;
	struct thermal_zone_device *thermal_zone;
	int tz_enabled;
	int kelvin_offset;
	struct mutex lock;
=======
struct acpi_thermal {
	struct acpi_device *device;
	acpi_bus_id name;
	unsigned long temp_dk;
	unsigned long last_temp_dk;
	unsigned long polling_frequency;
	volatile u8 zombie;
	struct acpi_thermal_trips trips;
	struct thermal_zone_device *thermal_zone;
	int kelvin_offset;	/* in millidegrees */
	struct work_struct thermal_check_work;
	struct mutex thermal_check_lock;
	refcount_t thermal_check_count;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
};

/* --------------------------------------------------------------------------
                             Thermal Zone Management
   -------------------------------------------------------------------------- */

static int acpi_thermal_get_temperature(struct acpi_thermal *tz)
{
	acpi_status status = AE_OK;
	unsigned long long tmp;

	if (!tz)
		return -EINVAL;

<<<<<<< HEAD
	tz->last_temperature = tz->temperature;
=======
	tz->last_temp_dk = tz->temp_dk;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	status = acpi_evaluate_integer(tz->device->handle, "_TMP", NULL, &tmp);
	if (ACPI_FAILURE(status))
		return -ENODEV;

<<<<<<< HEAD
	tz->temperature = tmp;
	ACPI_DEBUG_PRINT((ACPI_DB_INFO, "Temperature is %lu dK\n",
			  tz->temperature));
=======
	tz->temp_dk = tmp;

	acpi_handle_debug(tz->device->handle, "Temperature is %lu dK\n",
			  tz->temp_dk);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	return 0;
}

static int acpi_thermal_get_polling_frequency(struct acpi_thermal *tz)
{
	acpi_status status = AE_OK;
	unsigned long long tmp;

	if (!tz)
		return -EINVAL;

	status = acpi_evaluate_integer(tz->device->handle, "_TZP", NULL, &tmp);
	if (ACPI_FAILURE(status))
		return -ENODEV;

	tz->polling_frequency = tmp;
<<<<<<< HEAD
	ACPI_DEBUG_PRINT((ACPI_DB_INFO, "Polling frequency is %lu dS\n",
			  tz->polling_frequency));
=======
	acpi_handle_debug(tz->device->handle, "Polling frequency is %lu dS\n",
			  tz->polling_frequency);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	return 0;
}

<<<<<<< HEAD
static int acpi_thermal_set_cooling_mode(struct acpi_thermal *tz, int mode)
{
	acpi_status status = AE_OK;
	union acpi_object arg0 = { ACPI_TYPE_INTEGER };
	struct acpi_object_list arg_list = { 1, &arg0 };
	acpi_handle handle = NULL;


	if (!tz)
		return -EINVAL;

	status = acpi_get_handle(tz->device->handle, "_SCP", &handle);
	if (ACPI_FAILURE(status)) {
		ACPI_DEBUG_PRINT((ACPI_DB_INFO, "_SCP not present\n"));
		return -ENODEV;
	}

	arg0.integer.value = mode;

	status = acpi_evaluate_object(handle, NULL, &arg_list, NULL);
	if (ACPI_FAILURE(status))
		return -ENODEV;
=======
static int acpi_thermal_temp(struct acpi_thermal *tz, int temp_deci_k)
{
	if (temp_deci_k == THERMAL_TEMP_INVALID)
		return THERMAL_TEMP_INVALID;

	return deci_kelvin_to_millicelsius_with_offset(temp_deci_k,
						       tz->kelvin_offset);
}

static bool acpi_thermal_trip_valid(struct acpi_thermal_trip *acpi_trip)
{
	return acpi_trip->temp_dk != THERMAL_TEMP_INVALID;
}

static int active_trip_index(struct acpi_thermal *tz,
			     struct acpi_thermal_trip *acpi_trip)
{
	struct acpi_thermal_active *active;

	active = container_of(acpi_trip, struct acpi_thermal_active, trip);
	return active - tz->trips.active;
}

static long get_passive_temp(struct acpi_thermal *tz)
{
	int temp;

	if (acpi_passive_trip_temp(tz->device, &temp))
		return THERMAL_TEMP_INVALID;

	return temp;
}

static long get_active_temp(struct acpi_thermal *tz, int index)
{
	int temp;

	if (acpi_active_trip_temp(tz->device, index, &temp))
		return THERMAL_TEMP_INVALID;

	/*
	 * If an override has been provided, apply it so there are no active
	 * trips with thresholds greater than the override.
	 */
	if (act > 0) {
		unsigned long long override = celsius_to_deci_kelvin(act);

		if (temp > override)
			return override;
	}
	return temp;
}

static void acpi_thermal_update_trip(struct acpi_thermal *tz,
				     const struct thermal_trip *trip)
{
	struct acpi_thermal_trip *acpi_trip = trip->priv;

	if (trip->type == THERMAL_TRIP_PASSIVE) {
		if (psv > 0)
			return;

		acpi_trip->temp_dk = get_passive_temp(tz);
	} else {
		int index = active_trip_index(tz, acpi_trip);

		acpi_trip->temp_dk = get_active_temp(tz, index);
	}

	if (!acpi_thermal_trip_valid(acpi_trip))
		ACPI_THERMAL_TRIPS_EXCEPTION(tz, "state");
}

static bool update_trip_devices(struct acpi_thermal *tz,
				struct acpi_thermal_trip *acpi_trip,
				int index, bool compare)
{
	struct acpi_handle_list devices = { 0 };
	char method[] = "_PSL";

	if (index != ACPI_THERMAL_TRIP_PASSIVE) {
		method[1] = 'A';
		method[2] = 'L';
		method[3] = '0' + index;
	}

	if (!acpi_evaluate_reference(tz->device->handle, method, NULL, &devices)) {
		acpi_handle_info(tz->device->handle, "%s evaluation failure\n", method);
		return false;
	}

	if (acpi_handle_list_equal(&acpi_trip->devices, &devices)) {
		acpi_handle_list_free(&devices);
		return true;
	}

	if (compare)
		ACPI_THERMAL_TRIPS_EXCEPTION(tz, "device");

	acpi_handle_list_replace(&acpi_trip->devices, &devices);
	return true;
}

static void acpi_thermal_update_trip_devices(struct acpi_thermal *tz,
					     const struct thermal_trip *trip)
{
	struct acpi_thermal_trip *acpi_trip = trip->priv;
	int index = trip->type == THERMAL_TRIP_PASSIVE ?
			ACPI_THERMAL_TRIP_PASSIVE : active_trip_index(tz, acpi_trip);

	if (update_trip_devices(tz, acpi_trip, index, true))
		return;

	acpi_trip->temp_dk = THERMAL_TEMP_INVALID;
	ACPI_THERMAL_TRIPS_EXCEPTION(tz, "state");
}

struct adjust_trip_data {
	struct acpi_thermal *tz;
	u32 event;
};

static int acpi_thermal_adjust_trip(struct thermal_trip *trip, void *data)
{
	struct acpi_thermal_trip *acpi_trip = trip->priv;
	struct adjust_trip_data *atd = data;
	struct acpi_thermal *tz = atd->tz;
	int temp;

	if (!acpi_trip || !acpi_thermal_trip_valid(acpi_trip))
		return 0;

	if (atd->event == ACPI_THERMAL_NOTIFY_THRESHOLDS)
		acpi_thermal_update_trip(tz, trip);
	else
		acpi_thermal_update_trip_devices(tz, trip);

	if (acpi_thermal_trip_valid(acpi_trip))
		temp = acpi_thermal_temp(tz, acpi_trip->temp_dk);
	else
		temp = THERMAL_TEMP_INVALID;

	thermal_zone_set_trip_temp(tz->thermal_zone, trip, temp);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	return 0;
}

<<<<<<< HEAD
#define ACPI_TRIPS_CRITICAL	0x01
#define ACPI_TRIPS_HOT		0x02
#define ACPI_TRIPS_PASSIVE	0x04
#define ACPI_TRIPS_ACTIVE	0x08
#define ACPI_TRIPS_DEVICES	0x10

#define ACPI_TRIPS_REFRESH_THRESHOLDS	(ACPI_TRIPS_PASSIVE | ACPI_TRIPS_ACTIVE)
#define ACPI_TRIPS_REFRESH_DEVICES	ACPI_TRIPS_DEVICES

#define ACPI_TRIPS_INIT      (ACPI_TRIPS_CRITICAL | ACPI_TRIPS_HOT |	\
			      ACPI_TRIPS_PASSIVE | ACPI_TRIPS_ACTIVE |	\
			      ACPI_TRIPS_DEVICES)

/*
 * This exception is thrown out in two cases:
 * 1.An invalid trip point becomes invalid or a valid trip point becomes invalid
 *   when re-evaluating the AML code.
 * 2.TODO: Devices listed in _PSL, _ALx, _TZD may change.
 *   We need to re-bind the cooling devices of a thermal zone when this occurs.
 */
#define ACPI_THERMAL_TRIPS_EXCEPTION(flags, str)	\
do {	\
	if (flags != ACPI_TRIPS_INIT)	\
		ACPI_EXCEPTION((AE_INFO, AE_ERROR,	\
		"ACPI thermal trip point %s changed\n"	\
		"Please send acpidump to linux-acpi@vger.kernel.org\n", str)); \
} while (0)

static int acpi_thermal_trips_update(struct acpi_thermal *tz, int flag)
{
	acpi_status status = AE_OK;
	unsigned long long tmp;
	struct acpi_handle_list devices;
	int valid = 0;
	int i;

	/* Critical Shutdown */
	if (flag & ACPI_TRIPS_CRITICAL) {
		status = acpi_evaluate_integer(tz->device->handle,
				"_CRT", NULL, &tmp);
		tz->trips.critical.temperature = tmp;
		/*
		 * Treat freezing temperatures as invalid as well; some
		 * BIOSes return really low values and cause reboots at startup.
		 * Below zero (Celsius) values clearly aren't right for sure..
		 * ... so lets discard those as invalid.
		 */
		if (ACPI_FAILURE(status)) {
			tz->trips.critical.flags.valid = 0;
			ACPI_DEBUG_PRINT((ACPI_DB_INFO,
					  "No critical threshold\n"));
		} else if (tmp <= 2732) {
			printk(KERN_WARNING FW_BUG "Invalid critical threshold "
			       "(%llu)\n", tmp);
			tz->trips.critical.flags.valid = 0;
		} else {
			tz->trips.critical.flags.valid = 1;
			ACPI_DEBUG_PRINT((ACPI_DB_INFO,
					  "Found critical threshold [%lu]\n",
					  tz->trips.critical.temperature));
		}
		if (tz->trips.critical.flags.valid == 1) {
			if (crt == -1) {
				tz->trips.critical.flags.valid = 0;
			} else if (crt > 0) {
				unsigned long crt_k = CELSIUS_TO_KELVIN(crt);
				/*
				 * Allow override critical threshold
				 */
				if (crt_k > tz->trips.critical.temperature)
					printk(KERN_WARNING PREFIX
						"Critical threshold %d C\n", crt);
				tz->trips.critical.temperature = crt_k;
			}
		}
	}

	/* Critical Sleep (optional) */
	if (flag & ACPI_TRIPS_HOT) {
		status = acpi_evaluate_integer(tz->device->handle,
				"_HOT", NULL, &tmp);
		if (ACPI_FAILURE(status)) {
			tz->trips.hot.flags.valid = 0;
			ACPI_DEBUG_PRINT((ACPI_DB_INFO,
					"No hot threshold\n"));
		} else {
			tz->trips.hot.temperature = tmp;
			tz->trips.hot.flags.valid = 1;
			ACPI_DEBUG_PRINT((ACPI_DB_INFO,
					"Found hot threshold [%lu]\n",
					tz->trips.critical.temperature));
		}
	}

	/* Passive (optional) */
	if (((flag & ACPI_TRIPS_PASSIVE) && tz->trips.passive.flags.valid) ||
		(flag == ACPI_TRIPS_INIT)) {
		valid = tz->trips.passive.flags.valid;
		if (psv == -1) {
			status = AE_SUPPORT;
		} else if (psv > 0) {
			tmp = CELSIUS_TO_KELVIN(psv);
			status = AE_OK;
		} else {
			status = acpi_evaluate_integer(tz->device->handle,
				"_PSV", NULL, &tmp);
		}

		if (ACPI_FAILURE(status))
			tz->trips.passive.flags.valid = 0;
		else {
			tz->trips.passive.temperature = tmp;
			tz->trips.passive.flags.valid = 1;
			if (flag == ACPI_TRIPS_INIT) {
				status = acpi_evaluate_integer(
						tz->device->handle, "_TC1",
						NULL, &tmp);
				if (ACPI_FAILURE(status))
					tz->trips.passive.flags.valid = 0;
				else
					tz->trips.passive.tc1 = tmp;
				status = acpi_evaluate_integer(
						tz->device->handle, "_TC2",
						NULL, &tmp);
				if (ACPI_FAILURE(status))
					tz->trips.passive.flags.valid = 0;
				else
					tz->trips.passive.tc2 = tmp;
				status = acpi_evaluate_integer(
						tz->device->handle, "_TSP",
						NULL, &tmp);
				if (ACPI_FAILURE(status))
					tz->trips.passive.flags.valid = 0;
				else
					tz->trips.passive.tsp = tmp;
			}
		}
	}
	if ((flag & ACPI_TRIPS_DEVICES) && tz->trips.passive.flags.valid) {
		memset(&devices, 0, sizeof(struct acpi_handle_list));
		status = acpi_evaluate_reference(tz->device->handle, "_PSL",
							NULL, &devices);
		if (ACPI_FAILURE(status)) {
			printk(KERN_WARNING PREFIX
				"Invalid passive threshold\n");
			tz->trips.passive.flags.valid = 0;
		}
		else
			tz->trips.passive.flags.valid = 1;

		if (memcmp(&tz->trips.passive.devices, &devices,
				sizeof(struct acpi_handle_list))) {
			memcpy(&tz->trips.passive.devices, &devices,
				sizeof(struct acpi_handle_list));
			ACPI_THERMAL_TRIPS_EXCEPTION(flag, "device");
		}
	}
	if ((flag & ACPI_TRIPS_PASSIVE) || (flag & ACPI_TRIPS_DEVICES)) {
		if (valid != tz->trips.passive.flags.valid)
				ACPI_THERMAL_TRIPS_EXCEPTION(flag, "state");
	}

	/* Active (optional) */
	for (i = 0; i < ACPI_THERMAL_MAX_ACTIVE; i++) {
		char name[5] = { '_', 'A', 'C', ('0' + i), '\0' };
		valid = tz->trips.active[i].flags.valid;

		if (act == -1)
			break; /* disable all active trip points */

		if ((flag == ACPI_TRIPS_INIT) || ((flag & ACPI_TRIPS_ACTIVE) &&
			tz->trips.active[i].flags.valid)) {
			status = acpi_evaluate_integer(tz->device->handle,
							name, NULL, &tmp);
			if (ACPI_FAILURE(status)) {
				tz->trips.active[i].flags.valid = 0;
				if (i == 0)
					break;
				if (act <= 0)
					break;
				if (i == 1)
					tz->trips.active[0].temperature =
						CELSIUS_TO_KELVIN(act);
				else
					/*
					 * Don't allow override higher than
					 * the next higher trip point
					 */
					tz->trips.active[i - 1].temperature =
						(tz->trips.active[i - 2].temperature <
						CELSIUS_TO_KELVIN(act) ?
						tz->trips.active[i - 2].temperature :
						CELSIUS_TO_KELVIN(act));
				break;
			} else {
				tz->trips.active[i].temperature = tmp;
				tz->trips.active[i].flags.valid = 1;
			}
		}

		name[2] = 'L';
		if ((flag & ACPI_TRIPS_DEVICES) && tz->trips.active[i].flags.valid ) {
			memset(&devices, 0, sizeof(struct acpi_handle_list));
			status = acpi_evaluate_reference(tz->device->handle,
						name, NULL, &devices);
			if (ACPI_FAILURE(status)) {
				printk(KERN_WARNING PREFIX
					"Invalid active%d threshold\n", i);
				tz->trips.active[i].flags.valid = 0;
			}
			else
				tz->trips.active[i].flags.valid = 1;

			if (memcmp(&tz->trips.active[i].devices, &devices,
					sizeof(struct acpi_handle_list))) {
				memcpy(&tz->trips.active[i].devices, &devices,
					sizeof(struct acpi_handle_list));
				ACPI_THERMAL_TRIPS_EXCEPTION(flag, "device");
			}
		}
		if ((flag & ACPI_TRIPS_ACTIVE) || (flag & ACPI_TRIPS_DEVICES))
			if (valid != tz->trips.active[i].flags.valid)
				ACPI_THERMAL_TRIPS_EXCEPTION(flag, "state");

		if (!tz->trips.active[i].flags.valid)
			break;
	}

	if (flag & ACPI_TRIPS_DEVICES) {
		memset(&devices, 0, sizeof(struct acpi_handle_list));
		status = acpi_evaluate_reference(tz->device->handle, "_TZD",
						NULL, &devices);
		if (memcmp(&tz->devices, &devices,
				sizeof(struct acpi_handle_list))) {
			memcpy(&tz->devices, &devices,
				sizeof(struct acpi_handle_list));
			ACPI_THERMAL_TRIPS_EXCEPTION(flag, "device");
		}
	}

	return 0;
}

static int acpi_thermal_get_trip_points(struct acpi_thermal *tz)
{
	int i, valid, ret = acpi_thermal_trips_update(tz, ACPI_TRIPS_INIT);

	if (ret)
		return ret;

	valid = tz->trips.critical.flags.valid |
		tz->trips.hot.flags.valid |
		tz->trips.passive.flags.valid;

	for (i = 0; i < ACPI_THERMAL_MAX_ACTIVE; i++)
		valid |= tz->trips.active[i].flags.valid;

	if (!valid) {
		printk(KERN_WARNING FW_BUG "No valid trip found\n");
		return -ENODEV;
	}
	return 0;
}

static void acpi_thermal_check(void *data)
{
	struct acpi_thermal *tz = data;

	thermal_zone_device_update(tz->thermal_zone);
}

/* sys I/F for generic thermal sysfs support */
#define KELVIN_TO_MILLICELSIUS(t, off) (((t) - (off)) * 100)

static int thermal_get_temp(struct thermal_zone_device *thermal,
			    unsigned long *temp)
{
	struct acpi_thermal *tz = thermal->devdata;
=======
static void acpi_queue_thermal_check(struct acpi_thermal *tz)
{
	if (!work_pending(&tz->thermal_check_work))
		queue_work(acpi_thermal_pm_queue, &tz->thermal_check_work);
}

static void acpi_thermal_trips_update(struct acpi_thermal *tz, u32 event)
{
	struct adjust_trip_data atd = { .tz = tz, .event = event };
	struct acpi_device *adev = tz->device;

	/*
	 * Use thermal_zone_for_each_trip() to carry out the trip points
	 * update, so as to protect thermal_get_trend() from getting stale
	 * trip point temperatures and to prevent thermal_zone_device_update()
	 * invoked from acpi_thermal_check_fn() from producing inconsistent
	 * results.
	 */
	thermal_zone_for_each_trip(tz->thermal_zone,
				   acpi_thermal_adjust_trip, &atd);
	acpi_queue_thermal_check(tz);
	acpi_bus_generate_netlink_event(adev->pnp.device_class,
					dev_name(&adev->dev), event, 0);
}

static int acpi_thermal_get_critical_trip(struct acpi_thermal *tz)
{
	int temp;

	if (crt > 0) {
		temp = celsius_to_deci_kelvin(crt);
		goto set;
	}
	if (crt == -1) {
		acpi_handle_debug(tz->device->handle, "Critical threshold disabled\n");
		return THERMAL_TEMP_INVALID;
	}

	if (acpi_critical_trip_temp(tz->device, &temp))
		return THERMAL_TEMP_INVALID;

	if (temp <= 2732) {
		/*
		 * Below zero (Celsius) values clearly aren't right for sure,
		 * so discard them as invalid.
		 */
		pr_info(FW_BUG "Invalid critical threshold (%d)\n", temp);
		return THERMAL_TEMP_INVALID;
	}

set:
	acpi_handle_debug(tz->device->handle, "Critical threshold [%d]\n", temp);
	return temp;
}

static int acpi_thermal_get_hot_trip(struct acpi_thermal *tz)
{
	int temp;

	if (acpi_hot_trip_temp(tz->device, &temp) || temp == THERMAL_TEMP_INVALID) {
		acpi_handle_debug(tz->device->handle, "No hot threshold\n");
		return THERMAL_TEMP_INVALID;
	}

	acpi_handle_debug(tz->device->handle, "Hot threshold [%d]\n", temp);
	return temp;
}

static bool passive_trip_params_init(struct acpi_thermal *tz)
{
	unsigned long long tmp;
	acpi_status status;

	status = acpi_evaluate_integer(tz->device->handle, "_TC1", NULL, &tmp);
	if (ACPI_FAILURE(status))
		return false;

	tz->trips.passive.tc1 = tmp;

	status = acpi_evaluate_integer(tz->device->handle, "_TC2", NULL, &tmp);
	if (ACPI_FAILURE(status))
		return false;

	tz->trips.passive.tc2 = tmp;

	status = acpi_evaluate_integer(tz->device->handle, "_TFP", NULL, &tmp);
	if (ACPI_SUCCESS(status)) {
		tz->trips.passive.delay = tmp;
		return true;
	}

	status = acpi_evaluate_integer(tz->device->handle, "_TSP", NULL, &tmp);
	if (ACPI_FAILURE(status))
		return false;

	tz->trips.passive.delay = tmp * 100;

	return true;
}

static bool acpi_thermal_init_trip(struct acpi_thermal *tz, int index)
{
	struct acpi_thermal_trip *acpi_trip;
	long temp;

	if (index == ACPI_THERMAL_TRIP_PASSIVE) {
		acpi_trip = &tz->trips.passive.trip;

		if (psv == -1)
			goto fail;

		if (!passive_trip_params_init(tz))
			goto fail;

		temp = psv > 0 ? celsius_to_deci_kelvin(psv) :
				 get_passive_temp(tz);
	} else {
		acpi_trip = &tz->trips.active[index].trip;

		if (act == -1)
			goto fail;

		temp = get_active_temp(tz, index);
	}

	if (temp == THERMAL_TEMP_INVALID)
		goto fail;

	if (!update_trip_devices(tz, acpi_trip, index, false))
		goto fail;

	acpi_trip->temp_dk = temp;
	return true;

fail:
	acpi_trip->temp_dk = THERMAL_TEMP_INVALID;
	return false;
}

static void acpi_thermal_get_trip_points(struct acpi_thermal *tz)
{
	int i;

	acpi_thermal_init_trip(tz, ACPI_THERMAL_TRIP_PASSIVE);

	for (i = 0; i < ACPI_THERMAL_MAX_ACTIVE; i++) {
		if (!acpi_thermal_init_trip(tz, i))
			break;
	}

	while (++i < ACPI_THERMAL_MAX_ACTIVE)
		tz->trips.active[i].trip.temp_dk = THERMAL_TEMP_INVALID;
}

/* sys I/F for generic thermal sysfs support */

static int thermal_get_temp(struct thermal_zone_device *thermal, int *temp)
{
	struct acpi_thermal *tz = thermal_zone_device_priv(thermal);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	int result;

	if (!tz)
		return -EINVAL;

	result = acpi_thermal_get_temperature(tz);
	if (result)
		return result;

<<<<<<< HEAD
	*temp = KELVIN_TO_MILLICELSIUS(tz->temperature, tz->kelvin_offset);
	return 0;
}

static const char enabled[] = "kernel";
static const char disabled[] = "user";
static int thermal_get_mode(struct thermal_zone_device *thermal,
				enum thermal_device_mode *mode)
{
	struct acpi_thermal *tz = thermal->devdata;

	if (!tz)
		return -EINVAL;

	*mode = tz->tz_enabled ? THERMAL_DEVICE_ENABLED :
		THERMAL_DEVICE_DISABLED;

	return 0;
}

static int thermal_set_mode(struct thermal_zone_device *thermal,
				enum thermal_device_mode mode)
{
	struct acpi_thermal *tz = thermal->devdata;
	int enable;

	if (!tz)
		return -EINVAL;

	/*
	 * enable/disable thermal management from ACPI thermal driver
	 */
	if (mode == THERMAL_DEVICE_ENABLED)
		enable = 1;
	else if (mode == THERMAL_DEVICE_DISABLED)
		enable = 0;
	else
		return -EINVAL;

	if (enable != tz->tz_enabled) {
		tz->tz_enabled = enable;
		ACPI_DEBUG_PRINT((ACPI_DB_INFO,
			"%s ACPI thermal control\n",
			tz->tz_enabled ? enabled : disabled));
		acpi_thermal_check(tz);
	}
	return 0;
}

static int thermal_get_trip_type(struct thermal_zone_device *thermal,
				 int trip, enum thermal_trip_type *type)
{
	struct acpi_thermal *tz = thermal->devdata;
	int i;

	if (!tz || trip < 0)
		return -EINVAL;

	if (tz->trips.critical.flags.valid) {
		if (!trip) {
			*type = THERMAL_TRIP_CRITICAL;
			return 0;
		}
		trip--;
	}

	if (tz->trips.hot.flags.valid) {
		if (!trip) {
			*type = THERMAL_TRIP_HOT;
			return 0;
		}
		trip--;
	}

	if (tz->trips.passive.flags.valid) {
		if (!trip) {
			*type = THERMAL_TRIP_PASSIVE;
			return 0;
		}
		trip--;
	}

	for (i = 0; i < ACPI_THERMAL_MAX_ACTIVE &&
		tz->trips.active[i].flags.valid; i++) {
		if (!trip) {
			*type = THERMAL_TRIP_ACTIVE;
			return 0;
		}
		trip--;
	}

	return -EINVAL;
}

static int thermal_get_trip_temp(struct thermal_zone_device *thermal,
				 int trip, unsigned long *temp)
{
	struct acpi_thermal *tz = thermal->devdata;
	int i;

	if (!tz || trip < 0)
		return -EINVAL;

	if (tz->trips.critical.flags.valid) {
		if (!trip) {
			*temp = KELVIN_TO_MILLICELSIUS(
				tz->trips.critical.temperature,
				tz->kelvin_offset);
			return 0;
		}
		trip--;
	}

	if (tz->trips.hot.flags.valid) {
		if (!trip) {
			*temp = KELVIN_TO_MILLICELSIUS(
				tz->trips.hot.temperature,
				tz->kelvin_offset);
			return 0;
		}
		trip--;
	}

	if (tz->trips.passive.flags.valid) {
		if (!trip) {
			*temp = KELVIN_TO_MILLICELSIUS(
				tz->trips.passive.temperature,
				tz->kelvin_offset);
			return 0;
		}
		trip--;
	}

	for (i = 0; i < ACPI_THERMAL_MAX_ACTIVE &&
		tz->trips.active[i].flags.valid; i++) {
		if (!trip) {
			*temp = KELVIN_TO_MILLICELSIUS(
				tz->trips.active[i].temperature,
				tz->kelvin_offset);
			return 0;
		}
		trip--;
	}

	return -EINVAL;
}

static int thermal_get_crit_temp(struct thermal_zone_device *thermal,
				unsigned long *temperature) {
	struct acpi_thermal *tz = thermal->devdata;

	if (tz->trips.critical.flags.valid) {
		*temperature = KELVIN_TO_MILLICELSIUS(
				tz->trips.critical.temperature,
				tz->kelvin_offset);
		return 0;
	} else
		return -EINVAL;
}

static int thermal_notify(struct thermal_zone_device *thermal, int trip,
			   enum thermal_trip_type trip_type)
{
	u8 type = 0;
	struct acpi_thermal *tz = thermal->devdata;

	if (trip_type == THERMAL_TRIP_CRITICAL)
		type = ACPI_THERMAL_NOTIFY_CRITICAL;
	else if (trip_type == THERMAL_TRIP_HOT)
		type = ACPI_THERMAL_NOTIFY_HOT;
	else
		return 0;

	acpi_bus_generate_proc_event(tz->device, type, 1);
	acpi_bus_generate_netlink_event(tz->device->pnp.device_class,
					dev_name(&tz->device->dev), type, 1);

	if (trip_type == THERMAL_TRIP_CRITICAL && nocrt)
		return 1;

	return 0;
}

typedef int (*cb)(struct thermal_zone_device *, int,
		  struct thermal_cooling_device *);
static int acpi_thermal_cooling_device_cb(struct thermal_zone_device *thermal,
					struct thermal_cooling_device *cdev,
					cb action)
{
	struct acpi_device *device = cdev->devdata;
	struct acpi_thermal *tz = thermal->devdata;
	struct acpi_device *dev;
	acpi_status status;
	acpi_handle handle;
	int i;
	int j;
	int trip = -1;
	int result = 0;

	if (tz->trips.critical.flags.valid)
		trip++;

	if (tz->trips.hot.flags.valid)
		trip++;

	if (tz->trips.passive.flags.valid) {
		trip++;
		for (i = 0; i < tz->trips.passive.devices.count;
		    i++) {
			handle = tz->trips.passive.devices.handles[i];
			status = acpi_bus_get_device(handle, &dev);
			if (ACPI_SUCCESS(status) && (dev == device)) {
				result = action(thermal, trip, cdev);
				if (result)
					goto failed;
			}
		}
	}

	for (i = 0; i < ACPI_THERMAL_MAX_ACTIVE; i++) {
		if (!tz->trips.active[i].flags.valid)
			break;
		trip++;
		for (j = 0;
		    j < tz->trips.active[i].devices.count;
		    j++) {
			handle = tz->trips.active[i].devices.handles[j];
			status = acpi_bus_get_device(handle, &dev);
			if (ACPI_SUCCESS(status) && (dev == device)) {
				result = action(thermal, trip, cdev);
				if (result)
					goto failed;
			}
		}
	}

	for (i = 0; i < tz->devices.count; i++) {
		handle = tz->devices.handles[i];
		status = acpi_bus_get_device(handle, &dev);
		if (ACPI_SUCCESS(status) && (dev == device)) {
			result = action(thermal, -1, cdev);
			if (result)
				goto failed;
		}
	}

failed:
	return result;
=======
	*temp = deci_kelvin_to_millicelsius_with_offset(tz->temp_dk,
							tz->kelvin_offset);
	return 0;
}

static int thermal_get_trend(struct thermal_zone_device *thermal,
			     const struct thermal_trip *trip,
			     enum thermal_trend *trend)
{
	struct acpi_thermal *tz = thermal_zone_device_priv(thermal);
	struct acpi_thermal_trip *acpi_trip;
	int t;

	if (!tz || !trip)
		return -EINVAL;

	acpi_trip = trip->priv;
	if (!acpi_trip || !acpi_thermal_trip_valid(acpi_trip))
		return -EINVAL;

	switch (trip->type) {
	case THERMAL_TRIP_PASSIVE:
		t = tz->trips.passive.tc1 * (tz->temp_dk -
						tz->last_temp_dk) +
			tz->trips.passive.tc2 * (tz->temp_dk -
						acpi_trip->temp_dk);
		if (t > 0)
			*trend = THERMAL_TREND_RAISING;
		else if (t < 0)
			*trend = THERMAL_TREND_DROPPING;
		else
			*trend = THERMAL_TREND_STABLE;

		return 0;

	case THERMAL_TRIP_ACTIVE:
		t = acpi_thermal_temp(tz, tz->temp_dk);
		if (t <= trip->temperature)
			break;

		*trend = THERMAL_TREND_RAISING;

		return 0;

	default:
		break;
	}

	return -EINVAL;
}

static void acpi_thermal_zone_device_hot(struct thermal_zone_device *thermal)
{
	struct acpi_thermal *tz = thermal_zone_device_priv(thermal);

	acpi_bus_generate_netlink_event(tz->device->pnp.device_class,
					dev_name(&tz->device->dev),
					ACPI_THERMAL_NOTIFY_HOT, 1);
}

static void acpi_thermal_zone_device_critical(struct thermal_zone_device *thermal)
{
	struct acpi_thermal *tz = thermal_zone_device_priv(thermal);

	acpi_bus_generate_netlink_event(tz->device->pnp.device_class,
					dev_name(&tz->device->dev),
					ACPI_THERMAL_NOTIFY_CRITICAL, 1);

	thermal_zone_device_critical(thermal);
}

struct acpi_thermal_bind_data {
	struct thermal_zone_device *thermal;
	struct thermal_cooling_device *cdev;
	bool bind;
};

static int bind_unbind_cdev_cb(struct thermal_trip *trip, void *arg)
{
	struct acpi_thermal_trip *acpi_trip = trip->priv;
	struct acpi_thermal_bind_data *bd = arg;
	struct thermal_zone_device *thermal = bd->thermal;
	struct thermal_cooling_device *cdev = bd->cdev;
	struct acpi_device *cdev_adev = cdev->devdata;
	int i;

	/* Skip critical and hot trips. */
	if (!acpi_trip)
		return 0;

	for (i = 0; i < acpi_trip->devices.count; i++) {
		acpi_handle handle = acpi_trip->devices.handles[i];
		struct acpi_device *adev = acpi_fetch_acpi_dev(handle);

		if (adev != cdev_adev)
			continue;

		if (bd->bind) {
			int ret;

			ret = thermal_bind_cdev_to_trip(thermal, trip, cdev,
							THERMAL_NO_LIMIT,
							THERMAL_NO_LIMIT,
							THERMAL_WEIGHT_DEFAULT);
			if (ret)
				return ret;
		} else {
			thermal_unbind_cdev_from_trip(thermal, trip, cdev);
		}
	}

	return 0;
}

static int acpi_thermal_bind_unbind_cdev(struct thermal_zone_device *thermal,
					 struct thermal_cooling_device *cdev,
					 bool bind)
{
	struct acpi_thermal_bind_data bd = {
		.thermal = thermal, .cdev = cdev, .bind = bind
	};

	return for_each_thermal_trip(thermal, bind_unbind_cdev_cb, &bd);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

static int
acpi_thermal_bind_cooling_device(struct thermal_zone_device *thermal,
<<<<<<< HEAD
					struct thermal_cooling_device *cdev)
{
	return acpi_thermal_cooling_device_cb(thermal, cdev,
				thermal_zone_bind_cooling_device);
=======
				 struct thermal_cooling_device *cdev)
{
	return acpi_thermal_bind_unbind_cdev(thermal, cdev, true);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

static int
acpi_thermal_unbind_cooling_device(struct thermal_zone_device *thermal,
<<<<<<< HEAD
					struct thermal_cooling_device *cdev)
{
	return acpi_thermal_cooling_device_cb(thermal, cdev,
				thermal_zone_unbind_cooling_device);
=======
				   struct thermal_cooling_device *cdev)
{
	return acpi_thermal_bind_unbind_cdev(thermal, cdev, false);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

static const struct thermal_zone_device_ops acpi_thermal_zone_ops = {
	.bind = acpi_thermal_bind_cooling_device,
	.unbind	= acpi_thermal_unbind_cooling_device,
	.get_temp = thermal_get_temp,
<<<<<<< HEAD
	.get_mode = thermal_get_mode,
	.set_mode = thermal_set_mode,
	.get_trip_type = thermal_get_trip_type,
	.get_trip_temp = thermal_get_trip_temp,
	.get_crit_temp = thermal_get_crit_temp,
	.notify = thermal_notify,
};

static int acpi_thermal_register_thermal_zone(struct acpi_thermal *tz)
{
	int trips = 0;
	int result;
	acpi_status status;
	int i;

	if (tz->trips.critical.flags.valid)
		trips++;

	if (tz->trips.hot.flags.valid)
		trips++;

	if (tz->trips.passive.flags.valid)
		trips++;

	for (i = 0; i < ACPI_THERMAL_MAX_ACTIVE &&
			tz->trips.active[i].flags.valid; i++, trips++);

	if (tz->trips.passive.flags.valid)
		tz->thermal_zone =
			thermal_zone_device_register("acpitz", trips, tz,
						     &acpi_thermal_zone_ops,
						     tz->trips.passive.tc1,
						     tz->trips.passive.tc2,
						     tz->trips.passive.tsp*100,
						     tz->polling_frequency*100);
	else
		tz->thermal_zone =
			thermal_zone_device_register("acpitz", trips, tz,
						     &acpi_thermal_zone_ops,
						     0, 0, 0,
						     tz->polling_frequency*100);
	if (IS_ERR(tz->thermal_zone))
		return -ENODEV;

	result = sysfs_create_link(&tz->device->dev.kobj,
				   &tz->thermal_zone->device.kobj, "thermal_zone");
	if (result)
		return result;

	result = sysfs_create_link(&tz->thermal_zone->device.kobj,
				   &tz->device->dev.kobj, "device");
	if (result)
		return result;

	status = acpi_attach_data(tz->device->handle,
				  acpi_bus_private_data_handler,
				  tz->thermal_zone);
	if (ACPI_FAILURE(status)) {
		printk(KERN_ERR PREFIX
				"Error attaching device data\n");
		return -ENODEV;
	}

	tz->tz_enabled = 1;

	dev_info(&tz->device->dev, "registered as thermal_zone%d\n",
		 tz->thermal_zone->id);
	return 0;
=======
	.get_trend = thermal_get_trend,
	.hot = acpi_thermal_zone_device_hot,
	.critical = acpi_thermal_zone_device_critical,
};

static int acpi_thermal_zone_sysfs_add(struct acpi_thermal *tz)
{
	struct device *tzdev = thermal_zone_device(tz->thermal_zone);
	int ret;

	ret = sysfs_create_link(&tz->device->dev.kobj,
				&tzdev->kobj, "thermal_zone");
	if (ret)
		return ret;

	ret = sysfs_create_link(&tzdev->kobj,
				   &tz->device->dev.kobj, "device");
	if (ret)
		sysfs_remove_link(&tz->device->dev.kobj, "thermal_zone");

	return ret;
}

static void acpi_thermal_zone_sysfs_remove(struct acpi_thermal *tz)
{
	struct device *tzdev = thermal_zone_device(tz->thermal_zone);

	sysfs_remove_link(&tz->device->dev.kobj, "thermal_zone");
	sysfs_remove_link(&tzdev->kobj, "device");
}

static int acpi_thermal_register_thermal_zone(struct acpi_thermal *tz,
					      const struct thermal_trip *trip_table,
					      unsigned int trip_count,
					      int passive_delay)
{
	int result;

	if (trip_count)
		tz->thermal_zone = thermal_zone_device_register_with_trips(
					"acpitz", trip_table, trip_count, tz,
					&acpi_thermal_zone_ops, NULL, passive_delay,
					tz->polling_frequency * 100);
	else
		tz->thermal_zone = thermal_tripless_zone_device_register(
					"acpitz", tz, &acpi_thermal_zone_ops, NULL);

	if (IS_ERR(tz->thermal_zone))
		return PTR_ERR(tz->thermal_zone);

	result = acpi_thermal_zone_sysfs_add(tz);
	if (result)
		goto unregister_tzd;

	result = thermal_zone_device_enable(tz->thermal_zone);
	if (result)
		goto remove_links;

	dev_info(&tz->device->dev, "registered as thermal_zone%d\n",
		 thermal_zone_device_id(tz->thermal_zone));

	return 0;

remove_links:
	acpi_thermal_zone_sysfs_remove(tz);
unregister_tzd:
	thermal_zone_device_unregister(tz->thermal_zone);

	return result;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

static void acpi_thermal_unregister_thermal_zone(struct acpi_thermal *tz)
{
<<<<<<< HEAD
	sysfs_remove_link(&tz->device->dev.kobj, "thermal_zone");
	sysfs_remove_link(&tz->thermal_zone->device.kobj, "device");
	thermal_zone_device_unregister(tz->thermal_zone);
	tz->thermal_zone = NULL;
	acpi_detach_data(tz->device->handle, acpi_bus_private_data_handler);
=======
	thermal_zone_device_disable(tz->thermal_zone);
	acpi_thermal_zone_sysfs_remove(tz);
	thermal_zone_device_unregister(tz->thermal_zone);
	tz->thermal_zone = NULL;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}


/* --------------------------------------------------------------------------
                                 Driver Interface
   -------------------------------------------------------------------------- */

<<<<<<< HEAD
static void acpi_thermal_notify(struct acpi_device *device, u32 event)
{
	struct acpi_thermal *tz = acpi_driver_data(device);


=======
static void acpi_thermal_notify(acpi_handle handle, u32 event, void *data)
{
	struct acpi_device *device = data;
	struct acpi_thermal *tz = acpi_driver_data(device);

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	if (!tz)
		return;

	switch (event) {
	case ACPI_THERMAL_NOTIFY_TEMPERATURE:
<<<<<<< HEAD
		acpi_thermal_check(tz);
		break;
	case ACPI_THERMAL_NOTIFY_THRESHOLDS:
		acpi_thermal_trips_update(tz, ACPI_TRIPS_REFRESH_THRESHOLDS);
		acpi_thermal_check(tz);
		acpi_bus_generate_proc_event(device, event, 0);
		acpi_bus_generate_netlink_event(device->pnp.device_class,
						  dev_name(&device->dev), event, 0);
		break;
	case ACPI_THERMAL_NOTIFY_DEVICES:
		acpi_thermal_trips_update(tz, ACPI_TRIPS_REFRESH_DEVICES);
		acpi_thermal_check(tz);
		acpi_bus_generate_proc_event(device, event, 0);
		acpi_bus_generate_netlink_event(device->pnp.device_class,
						  dev_name(&device->dev), event, 0);
		break;
	default:
		ACPI_DEBUG_PRINT((ACPI_DB_INFO,
				  "Unsupported event [0x%x]\n", event));
=======
		acpi_queue_thermal_check(tz);
		break;
	case ACPI_THERMAL_NOTIFY_THRESHOLDS:
	case ACPI_THERMAL_NOTIFY_DEVICES:
		acpi_thermal_trips_update(tz, event);
		break;
	default:
		acpi_handle_debug(device->handle, "Unsupported event [0x%x]\n",
				  event);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		break;
	}
}

<<<<<<< HEAD
static int acpi_thermal_get_info(struct acpi_thermal *tz)
{
	int result = 0;


	if (!tz)
		return -EINVAL;

	/* Get trip points [_CRT, _PSV, etc.] (required) */
	result = acpi_thermal_get_trip_points(tz);
	if (result)
		return result;

	/* Get temperature [_TMP] (required) */
	result = acpi_thermal_get_temperature(tz);
	if (result)
		return result;

	/* Set the cooling mode [_SCP] to active cooling (default) */
	result = acpi_thermal_set_cooling_mode(tz, ACPI_THERMAL_MODE_ACTIVE);
	if (!result)
		tz->flags.cooling_mode = 1;

	/* Get default polling frequency [_TZP] (optional) */
	if (tzp)
		tz->polling_frequency = tzp;
	else
		acpi_thermal_get_polling_frequency(tz);

	return 0;
=======
/*
 * On some platforms, the AML code has dependency about
 * the evaluating order of _TMP and _CRT/_HOT/_PSV/_ACx.
 * 1. On HP Pavilion G4-1016tx, _TMP must be invoked after
 *    /_CRT/_HOT/_PSV/_ACx, or else system will be power off.
 * 2. On HP Compaq 6715b/6715s, the return value of _PSV is 0
 *    if _TMP has never been evaluated.
 *
 * As this dependency is totally transparent to OS, evaluate
 * all of them once, in the order of _CRT/_HOT/_PSV/_ACx,
 * _TMP, before they are actually used.
 */
static void acpi_thermal_aml_dependency_fix(struct acpi_thermal *tz)
{
	acpi_handle handle = tz->device->handle;
	unsigned long long value;
	int i;

	acpi_evaluate_integer(handle, "_CRT", NULL, &value);
	acpi_evaluate_integer(handle, "_HOT", NULL, &value);
	acpi_evaluate_integer(handle, "_PSV", NULL, &value);
	for (i = 0; i < ACPI_THERMAL_MAX_ACTIVE; i++) {
		char name[5] = { '_', 'A', 'C', ('0' + i), '\0' };
		acpi_status status;

		status = acpi_evaluate_integer(handle, name, NULL, &value);
		if (status == AE_NOT_FOUND)
			break;
	}
	acpi_evaluate_integer(handle, "_TMP", NULL, &value);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

/*
 * The exact offset between Kelvin and degree Celsius is 273.15. However ACPI
 * handles temperature values with a single decimal place. As a consequence,
 * some implementations use an offset of 273.1 and others use an offset of
 * 273.2. Try to find out which one is being used, to present the most
 * accurate and visually appealing number.
 *
 * The heuristic below should work for all ACPI thermal zones which have a
 * critical trip point with a value being a multiple of 0.5 degree Celsius.
 */
<<<<<<< HEAD
static void acpi_thermal_guess_offset(struct acpi_thermal *tz)
{
	if (tz->trips.critical.flags.valid &&
	    (tz->trips.critical.temperature % 5) == 1)
		tz->kelvin_offset = 2731;
	else
		tz->kelvin_offset = 2732;
=======
static void acpi_thermal_guess_offset(struct acpi_thermal *tz, long crit_temp)
{
	if (crit_temp != THERMAL_TEMP_INVALID && crit_temp % 5 == 1)
		tz->kelvin_offset = 273100;
	else
		tz->kelvin_offset = 273200;
}

static void acpi_thermal_check_fn(struct work_struct *work)
{
	struct acpi_thermal *tz = container_of(work, struct acpi_thermal,
					       thermal_check_work);

	/*
	 * In general, it is not sufficient to check the pending bit, because
	 * subsequent instances of this function may be queued after one of them
	 * has started running (e.g. if _TMP sleeps).  Avoid bailing out if just
	 * one of them is running, though, because it may have done the actual
	 * check some time ago, so allow at least one of them to block on the
	 * mutex while another one is running the update.
	 */
	if (!refcount_dec_not_one(&tz->thermal_check_count))
		return;

	mutex_lock(&tz->thermal_check_lock);

	thermal_zone_device_update(tz->thermal_zone, THERMAL_EVENT_UNSPECIFIED);

	refcount_inc(&tz->thermal_check_count);

	mutex_unlock(&tz->thermal_check_lock);
}

static void acpi_thermal_free_thermal_zone(struct acpi_thermal *tz)
{
	int i;

	acpi_handle_list_free(&tz->trips.passive.trip.devices);
	for (i = 0; i < ACPI_THERMAL_MAX_ACTIVE; i++)
		acpi_handle_list_free(&tz->trips.active[i].trip.devices);

	kfree(tz);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

static int acpi_thermal_add(struct acpi_device *device)
{
<<<<<<< HEAD
	int result = 0;
	struct acpi_thermal *tz = NULL;

=======
	struct thermal_trip trip_table[ACPI_THERMAL_MAX_NR_TRIPS] = { 0 };
	struct acpi_thermal_trip *acpi_trip;
	struct thermal_trip *trip;
	struct acpi_thermal *tz;
	int crit_temp, hot_temp;
	int passive_delay = 0;
	int result;
	int i;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	if (!device)
		return -EINVAL;

	tz = kzalloc(sizeof(struct acpi_thermal), GFP_KERNEL);
	if (!tz)
		return -ENOMEM;

	tz->device = device;
	strcpy(tz->name, device->pnp.bus_id);
	strcpy(acpi_device_name(device), ACPI_THERMAL_DEVICE_NAME);
	strcpy(acpi_device_class(device), ACPI_THERMAL_CLASS);
	device->driver_data = tz;
<<<<<<< HEAD
	mutex_init(&tz->lock);


	result = acpi_thermal_get_info(tz);
	if (result)
		goto free_memory;

	acpi_thermal_guess_offset(tz);

	result = acpi_thermal_register_thermal_zone(tz);
	if (result)
		goto free_memory;

	printk(KERN_INFO PREFIX "%s [%s] (%ld C)\n",
	       acpi_device_name(device), acpi_device_bid(device),
	       KELVIN_TO_CELSIUS(tz->temperature));
	goto end;

free_memory:
	kfree(tz);
end:
	return result;
}

static int acpi_thermal_remove(struct acpi_device *device, int type)
{
	struct acpi_thermal *tz = NULL;

	if (!device || !acpi_driver_data(device))
		return -EINVAL;

	tz = acpi_driver_data(device);

	acpi_thermal_unregister_thermal_zone(tz);
	mutex_destroy(&tz->lock);
	kfree(tz);
	return 0;
}

static int acpi_thermal_resume(struct acpi_device *device)
{
	struct acpi_thermal *tz = NULL;
	int i, j, power_state, result;


	if (!device || !acpi_driver_data(device))
		return -EINVAL;

	tz = acpi_driver_data(device);

	for (i = 0; i < ACPI_THERMAL_MAX_ACTIVE; i++) {
		if (!(&tz->trips.active[i]))
			break;
		if (!tz->trips.active[i].flags.valid)
			break;
		tz->trips.active[i].flags.enabled = 1;
		for (j = 0; j < tz->trips.active[i].devices.count; j++) {
			result = acpi_bus_update_power(
					tz->trips.active[i].devices.handles[j],
					&power_state);
			if (result || (power_state != ACPI_STATE_D0)) {
				tz->trips.active[i].flags.enabled = 0;
				break;
			}
		}
		tz->state.active |= tz->trips.active[i].flags.enabled;
	}

	acpi_thermal_check(tz);

	return AE_OK;
}

static int thermal_act(const struct dmi_system_id *d) {

	if (act == 0) {
		printk(KERN_NOTICE "ACPI: %s detected: "
			"disabling all active thermal trip points\n", d->ident);
=======

	acpi_thermal_aml_dependency_fix(tz);

	/* Get trip points [_CRT, _PSV, etc.] (required). */
	acpi_thermal_get_trip_points(tz);

	crit_temp = acpi_thermal_get_critical_trip(tz);
	hot_temp = acpi_thermal_get_hot_trip(tz);

	/* Get temperature [_TMP] (required). */
	result = acpi_thermal_get_temperature(tz);
	if (result)
		goto free_memory;

	/* Set the cooling mode [_SCP] to active cooling. */
	acpi_execute_simple_method(tz->device->handle, "_SCP",
				   ACPI_THERMAL_MODE_ACTIVE);

	/* Determine the default polling frequency [_TZP]. */
	if (tzp)
		tz->polling_frequency = tzp;
	else
		acpi_thermal_get_polling_frequency(tz);

	acpi_thermal_guess_offset(tz, crit_temp);

	trip = trip_table;

	if (crit_temp != THERMAL_TEMP_INVALID) {
		trip->type = THERMAL_TRIP_CRITICAL;
		trip->temperature = acpi_thermal_temp(tz, crit_temp);
		trip++;
	}

	if (hot_temp != THERMAL_TEMP_INVALID) {
		trip->type = THERMAL_TRIP_HOT;
		trip->temperature = acpi_thermal_temp(tz, hot_temp);
		trip++;
	}

	acpi_trip = &tz->trips.passive.trip;
	if (acpi_thermal_trip_valid(acpi_trip)) {
		passive_delay = tz->trips.passive.delay;

		trip->type = THERMAL_TRIP_PASSIVE;
		trip->temperature = acpi_thermal_temp(tz, acpi_trip->temp_dk);
		trip->priv = acpi_trip;
		trip++;
	}

	for (i = 0; i < ACPI_THERMAL_MAX_ACTIVE; i++) {
		acpi_trip =  &tz->trips.active[i].trip;

		if (!acpi_thermal_trip_valid(acpi_trip))
			break;

		trip->type = THERMAL_TRIP_ACTIVE;
		trip->temperature = acpi_thermal_temp(tz, acpi_trip->temp_dk);
		trip->priv = acpi_trip;
		trip++;
	}

	if (trip == trip_table)
		pr_warn(FW_BUG "No valid trip points!\n");

	result = acpi_thermal_register_thermal_zone(tz, trip_table,
						    trip - trip_table,
						    passive_delay);
	if (result)
		goto free_memory;

	refcount_set(&tz->thermal_check_count, 3);
	mutex_init(&tz->thermal_check_lock);
	INIT_WORK(&tz->thermal_check_work, acpi_thermal_check_fn);

	pr_info("%s [%s] (%ld C)\n", acpi_device_name(device),
		acpi_device_bid(device), deci_kelvin_to_celsius(tz->temp_dk));

	result = acpi_dev_install_notify_handler(device, ACPI_DEVICE_NOTIFY,
						 acpi_thermal_notify, device);
	if (result)
		goto flush_wq;

	return 0;

flush_wq:
	flush_workqueue(acpi_thermal_pm_queue);
	acpi_thermal_unregister_thermal_zone(tz);
free_memory:
	acpi_thermal_free_thermal_zone(tz);

	return result;
}

static void acpi_thermal_remove(struct acpi_device *device)
{
	struct acpi_thermal *tz;

	if (!device || !acpi_driver_data(device))
		return;

	tz = acpi_driver_data(device);

	acpi_dev_remove_notify_handler(device, ACPI_DEVICE_NOTIFY,
				       acpi_thermal_notify);

	flush_workqueue(acpi_thermal_pm_queue);
	acpi_thermal_unregister_thermal_zone(tz);
	acpi_thermal_free_thermal_zone(tz);
}

#ifdef CONFIG_PM_SLEEP
static int acpi_thermal_suspend(struct device *dev)
{
	/* Make sure the previously queued thermal check work has been done */
	flush_workqueue(acpi_thermal_pm_queue);
	return 0;
}

static int acpi_thermal_resume(struct device *dev)
{
	struct acpi_thermal *tz;
	int i, j, power_state;

	if (!dev)
		return -EINVAL;

	tz = acpi_driver_data(to_acpi_device(dev));
	if (!tz)
		return -EINVAL;

	for (i = 0; i < ACPI_THERMAL_MAX_ACTIVE; i++) {
		struct acpi_thermal_trip *acpi_trip = &tz->trips.active[i].trip;

		if (!acpi_thermal_trip_valid(acpi_trip))
			break;

		for (j = 0; j < acpi_trip->devices.count; j++) {
			acpi_bus_update_power(acpi_trip->devices.handles[j],
					      &power_state);
		}
	}

	acpi_queue_thermal_check(tz);

	return AE_OK;
}
#else
#define acpi_thermal_suspend	NULL
#define acpi_thermal_resume	NULL
#endif
static SIMPLE_DEV_PM_OPS(acpi_thermal_pm, acpi_thermal_suspend, acpi_thermal_resume);

static const struct acpi_device_id  thermal_device_ids[] = {
	{ACPI_THERMAL_HID, 0},
	{"", 0},
};
MODULE_DEVICE_TABLE(acpi, thermal_device_ids);

static struct acpi_driver acpi_thermal_driver = {
	.name = "thermal",
	.class = ACPI_THERMAL_CLASS,
	.ids = thermal_device_ids,
	.ops = {
		.add = acpi_thermal_add,
		.remove = acpi_thermal_remove,
		},
	.drv.pm = &acpi_thermal_pm,
};

static int thermal_act(const struct dmi_system_id *d)
{
	if (act == 0) {
		pr_notice("%s detected: disabling all active thermal trip points\n",
			  d->ident);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		act = -1;
	}
	return 0;
}
<<<<<<< HEAD
static int thermal_nocrt(const struct dmi_system_id *d) {

	printk(KERN_NOTICE "ACPI: %s detected: "
		"disabling all critical thermal trip point actions.\n", d->ident);
	nocrt = 1;
	return 0;
}
static int thermal_tzp(const struct dmi_system_id *d) {

	if (tzp == 0) {
		printk(KERN_NOTICE "ACPI: %s detected: "
			"enabling thermal zone polling\n", d->ident);
=======

static int thermal_nocrt(const struct dmi_system_id *d)
{
	pr_notice("%s detected: disabling all critical thermal trip point actions.\n",
		  d->ident);
	crt = -1;
	return 0;
}

static int thermal_tzp(const struct dmi_system_id *d)
{
	if (tzp == 0) {
		pr_notice("%s detected: enabling thermal zone polling\n",
			  d->ident);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		tzp = 300;	/* 300 dS = 30 Seconds */
	}
	return 0;
}
<<<<<<< HEAD
static int thermal_psv(const struct dmi_system_id *d) {

	if (psv == 0) {
		printk(KERN_NOTICE "ACPI: %s detected: "
			"disabling all passive thermal trip points\n", d->ident);
=======

static int thermal_psv(const struct dmi_system_id *d)
{
	if (psv == 0) {
		pr_notice("%s detected: disabling all passive thermal trip points\n",
			  d->ident);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		psv = -1;
	}
	return 0;
}

<<<<<<< HEAD
static struct dmi_system_id thermal_dmi_table[] __initdata = {
=======
static const struct dmi_system_id thermal_dmi_table[] __initconst = {
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	/*
	 * Award BIOS on this AOpen makes thermal control almost worthless.
	 * http://bugzilla.kernel.org/show_bug.cgi?id=8842
	 */
	{
	 .callback = thermal_act,
	 .ident = "AOpen i915GMm-HFS",
	 .matches = {
		DMI_MATCH(DMI_BOARD_VENDOR, "AOpen"),
		DMI_MATCH(DMI_BOARD_NAME, "i915GMm-HFS"),
		},
	},
	{
	 .callback = thermal_psv,
	 .ident = "AOpen i915GMm-HFS",
	 .matches = {
		DMI_MATCH(DMI_BOARD_VENDOR, "AOpen"),
		DMI_MATCH(DMI_BOARD_NAME, "i915GMm-HFS"),
		},
	},
	{
	 .callback = thermal_tzp,
	 .ident = "AOpen i915GMm-HFS",
	 .matches = {
		DMI_MATCH(DMI_BOARD_VENDOR, "AOpen"),
		DMI_MATCH(DMI_BOARD_NAME, "i915GMm-HFS"),
		},
	},
	{
	 .callback = thermal_nocrt,
	 .ident = "Gigabyte GA-7ZX",
	 .matches = {
		DMI_MATCH(DMI_BOARD_VENDOR, "Gigabyte Technology Co., Ltd."),
		DMI_MATCH(DMI_BOARD_NAME, "7ZX"),
		},
	},
	{}
};

static int __init acpi_thermal_init(void)
{
<<<<<<< HEAD
	int result = 0;
=======
	int result;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	dmi_check_system(thermal_dmi_table);

	if (off) {
<<<<<<< HEAD
		printk(KERN_NOTICE "ACPI: thermal control disabled\n");
		return -ENODEV;
	}

	result = acpi_bus_register_driver(&acpi_thermal_driver);
	if (result < 0)
		return -ENODEV;

=======
		pr_notice("thermal control disabled\n");
		return -ENODEV;
	}

	acpi_thermal_pm_queue = alloc_workqueue("acpi_thermal_pm",
						WQ_HIGHPRI | WQ_MEM_RECLAIM, 0);
	if (!acpi_thermal_pm_queue)
		return -ENODEV;

	result = acpi_bus_register_driver(&acpi_thermal_driver);
	if (result < 0) {
		destroy_workqueue(acpi_thermal_pm_queue);
		return -ENODEV;
	}

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	return 0;
}

static void __exit acpi_thermal_exit(void)
{
<<<<<<< HEAD

	acpi_bus_unregister_driver(&acpi_thermal_driver);

	return;
=======
	acpi_bus_unregister_driver(&acpi_thermal_driver);
	destroy_workqueue(acpi_thermal_pm_queue);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

module_init(acpi_thermal_init);
module_exit(acpi_thermal_exit);
<<<<<<< HEAD
=======

MODULE_IMPORT_NS(ACPI_THERMAL);
MODULE_AUTHOR("Paul Diefenbaugh");
MODULE_DESCRIPTION("ACPI Thermal Zone Driver");
MODULE_LICENSE("GPL");
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
