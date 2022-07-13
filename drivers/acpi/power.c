<<<<<<< HEAD
/*
 *  acpi_power.c - ACPI Bus Power Management ($Revision: 39 $)
 *
 *  Copyright (C) 2001, 2002 Andy Grover <andrew.grover@intel.com>
 *  Copyright (C) 2001, 2002 Paul Diefenbaugh <paul.s.diefenbaugh@intel.com>
 *
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
=======
// SPDX-License-Identifier: GPL-2.0-or-later
/*
 * drivers/acpi/power.c - ACPI Power Resources management.
 *
 * Copyright (C) 2001 - 2015 Intel Corp.
 * Author: Andy Grover <andrew.grover@intel.com>
 * Author: Paul Diefenbaugh <paul.s.diefenbaugh@intel.com>
 * Author: Rafael J. Wysocki <rafael.j.wysocki@intel.com>
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 */

/*
 * ACPI power-managed devices may be controlled in two ways:
 * 1. via "Device Specific (D-State) Control"
 * 2. via "Power Resource Control".
<<<<<<< HEAD
 * This module is used to manage devices relying on Power Resource Control.
 * 
 * An ACPI "power resource object" describes a software controllable power
 * plane, clock plane, or other resource used by a power managed device.
=======
 * The code below deals with ACPI Power Resources control.
 *
 * An ACPI "power resource object" represents a software controllable power
 * plane, clock plane, or other resource depended on by a device.
 *
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 * A device may rely on multiple power resources, and a power resource
 * may be shared by multiple devices.
 */

<<<<<<< HEAD
=======
#define pr_fmt(fmt) "ACPI: PM: " fmt

#include <linux/dmi.h>
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/init.h>
#include <linux/types.h>
#include <linux/slab.h>
#include <linux/pm_runtime.h>
<<<<<<< HEAD
#include <acpi/acpi_bus.h>
#include <acpi/acpi_drivers.h>
#include "sleep.h"
#include "internal.h"

#define PREFIX "ACPI: "

#define _COMPONENT			ACPI_POWER_COMPONENT
ACPI_MODULE_NAME("power");
#define ACPI_POWER_CLASS		"power_resource"
#define ACPI_POWER_DEVICE_NAME		"Power Resource"
#define ACPI_POWER_FILE_INFO		"info"
#define ACPI_POWER_FILE_STATUS		"state"
=======
#include <linux/sysfs.h>
#include <linux/acpi.h>
#include "sleep.h"
#include "internal.h"

#define ACPI_POWER_CLASS		"power_resource"
#define ACPI_POWER_DEVICE_NAME		"Power Resource"
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#define ACPI_POWER_RESOURCE_STATE_OFF	0x00
#define ACPI_POWER_RESOURCE_STATE_ON	0x01
#define ACPI_POWER_RESOURCE_STATE_UNKNOWN 0xFF

<<<<<<< HEAD
static int acpi_power_add(struct acpi_device *device);
static int acpi_power_remove(struct acpi_device *device, int type);
static int acpi_power_resume(struct acpi_device *device);

static const struct acpi_device_id power_device_ids[] = {
	{ACPI_POWER_HID, 0},
	{"", 0},
};
MODULE_DEVICE_TABLE(acpi, power_device_ids);

static struct acpi_driver acpi_power_driver = {
	.name = "power",
	.class = ACPI_POWER_CLASS,
	.ids = power_device_ids,
	.ops = {
		.add = acpi_power_add,
		.remove = acpi_power_remove,
		.resume = acpi_power_resume,
		},
};

/*
 * A power managed device
 * A device may rely on multiple power resources.
 * */
struct acpi_power_managed_device {
	struct device *dev; /* The physical device */
	acpi_handle *handle;
};

struct acpi_power_resource_device {
	struct acpi_power_managed_device *device;
	struct acpi_power_resource_device *next;
};

struct acpi_power_resource {
	struct acpi_device * device;
	acpi_bus_id name;
	u32 system_level;
	u32 order;
	unsigned int ref_count;
	struct mutex resource_lock;

	/* List of devices relying on this power resource */
	struct acpi_power_resource_device *devices;
	struct mutex devices_lock;
};

static struct list_head acpi_power_resource_list;
=======
struct acpi_power_dependent_device {
	struct device *dev;
	struct list_head node;
};

struct acpi_power_resource {
	struct acpi_device device;
	struct list_head list_node;
	u32 system_level;
	u32 order;
	unsigned int ref_count;
	u8 state;
	struct mutex resource_lock;
	struct list_head dependents;
};

struct acpi_power_resource_entry {
	struct list_head node;
	struct acpi_power_resource *resource;
};

static LIST_HEAD(acpi_power_resource_list);
static DEFINE_MUTEX(power_resource_list_lock);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

/* --------------------------------------------------------------------------
                             Power Resource Management
   -------------------------------------------------------------------------- */

<<<<<<< HEAD
static int
acpi_power_get_context(acpi_handle handle,
		       struct acpi_power_resource **resource)
{
	int result = 0;
	struct acpi_device *device = NULL;


	if (!resource)
		return -ENODEV;

	result = acpi_bus_get_device(handle, &device);
	if (result) {
		printk(KERN_WARNING PREFIX "Getting context [%p]\n", handle);
		return result;
	}

	*resource = acpi_driver_data(device);
	if (!*resource)
		return -ENODEV;

	return 0;
}

static int acpi_power_get_state(acpi_handle handle, int *state)
{
	acpi_status status = AE_OK;
	unsigned long long sta = 0;
	char node_name[5];
	struct acpi_buffer buffer = { sizeof(node_name), node_name };


	if (!handle || !state)
		return -EINVAL;
=======
static inline const char *resource_dev_name(struct acpi_power_resource *pr)
{
	return dev_name(&pr->device.dev);
}

static inline
struct acpi_power_resource *to_power_resource(struct acpi_device *device)
{
	return container_of(device, struct acpi_power_resource, device);
}

static struct acpi_power_resource *acpi_power_get_context(acpi_handle handle)
{
	struct acpi_device *device = acpi_fetch_acpi_dev(handle);

	if (!device)
		return NULL;

	return to_power_resource(device);
}

static int acpi_power_resources_list_add(acpi_handle handle,
					 struct list_head *list)
{
	struct acpi_power_resource *resource = acpi_power_get_context(handle);
	struct acpi_power_resource_entry *entry;

	if (!resource || !list)
		return -EINVAL;

	entry = kzalloc(sizeof(*entry), GFP_KERNEL);
	if (!entry)
		return -ENOMEM;

	entry->resource = resource;
	if (!list_empty(list)) {
		struct acpi_power_resource_entry *e;

		list_for_each_entry(e, list, node)
			if (e->resource->order > resource->order) {
				list_add_tail(&entry->node, &e->node);
				return 0;
			}
	}
	list_add_tail(&entry->node, list);
	return 0;
}

void acpi_power_resources_list_free(struct list_head *list)
{
	struct acpi_power_resource_entry *entry, *e;

	list_for_each_entry_safe(entry, e, list, node) {
		list_del(&entry->node);
		kfree(entry);
	}
}

static bool acpi_power_resource_is_dup(union acpi_object *package,
				       unsigned int start, unsigned int i)
{
	acpi_handle rhandle, dup;
	unsigned int j;

	/* The caller is expected to check the package element types */
	rhandle = package->package.elements[i].reference.handle;
	for (j = start; j < i; j++) {
		dup = package->package.elements[j].reference.handle;
		if (dup == rhandle)
			return true;
	}

	return false;
}

int acpi_extract_power_resources(union acpi_object *package, unsigned int start,
				 struct list_head *list)
{
	unsigned int i;
	int err = 0;

	for (i = start; i < package->package.count; i++) {
		union acpi_object *element = &package->package.elements[i];
		struct acpi_device *rdev;
		acpi_handle rhandle;

		if (element->type != ACPI_TYPE_LOCAL_REFERENCE) {
			err = -ENODATA;
			break;
		}
		rhandle = element->reference.handle;
		if (!rhandle) {
			err = -ENODEV;
			break;
		}

		/* Some ACPI tables contain duplicate power resource references */
		if (acpi_power_resource_is_dup(package, start, i))
			continue;

		rdev = acpi_add_power_resource(rhandle);
		if (!rdev) {
			err = -ENODEV;
			break;
		}
		err = acpi_power_resources_list_add(rhandle, list);
		if (err)
			break;
	}
	if (err)
		acpi_power_resources_list_free(list);

	return err;
}

static int __get_state(acpi_handle handle, u8 *state)
{
	acpi_status status = AE_OK;
	unsigned long long sta = 0;
	u8 cur_state;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	status = acpi_evaluate_integer(handle, "_STA", NULL, &sta);
	if (ACPI_FAILURE(status))
		return -ENODEV;

<<<<<<< HEAD
	*state = (sta & 0x01)?ACPI_POWER_RESOURCE_STATE_ON:
			      ACPI_POWER_RESOURCE_STATE_OFF;

	acpi_get_name(handle, ACPI_SINGLE_NAME, &buffer);

	ACPI_DEBUG_PRINT((ACPI_DB_INFO, "Resource [%s] is %s\n",
			  node_name,
				*state ? "on" : "off"));

	return 0;
}

static int acpi_power_get_list_state(struct acpi_handle_list *list, int *state)
{
	int cur_state;
	int i = 0;
=======
	cur_state = sta & ACPI_POWER_RESOURCE_STATE_ON;

	acpi_handle_debug(handle, "Power resource is %s\n",
			  cur_state ? "on" : "off");

	*state = cur_state;
	return 0;
}

static int acpi_power_get_state(struct acpi_power_resource *resource, u8 *state)
{
	if (resource->state == ACPI_POWER_RESOURCE_STATE_UNKNOWN) {
		int ret;

		ret = __get_state(resource->device.handle, &resource->state);
		if (ret)
			return ret;
	}

	*state = resource->state;
	return 0;
}

static int acpi_power_get_list_state(struct list_head *list, u8 *state)
{
	struct acpi_power_resource_entry *entry;
	u8 cur_state = ACPI_POWER_RESOURCE_STATE_OFF;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	if (!list || !state)
		return -EINVAL;

	/* The state of the list is 'on' IFF all resources are 'on'. */
<<<<<<< HEAD

	for (i = 0; i < list->count; i++) {
		struct acpi_power_resource *resource;
		acpi_handle handle = list->handles[i];
		int result;

		result = acpi_power_get_context(handle, &resource);
		if (result)
			return result;

		mutex_lock(&resource->resource_lock);

		result = acpi_power_get_state(handle, &cur_state);

		mutex_unlock(&resource->resource_lock);

=======
	list_for_each_entry(entry, list, node) {
		struct acpi_power_resource *resource = entry->resource;
		int result;

		mutex_lock(&resource->resource_lock);
		result = acpi_power_get_state(resource, &cur_state);
		mutex_unlock(&resource->resource_lock);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		if (result)
			return result;

		if (cur_state != ACPI_POWER_RESOURCE_STATE_ON)
			break;
	}

<<<<<<< HEAD
	ACPI_DEBUG_PRINT((ACPI_DB_INFO, "Resource list is %s\n",
			  cur_state ? "on" : "off"));

	*state = cur_state;

	return 0;
}

/* Resume the device when all power resources in _PR0 are on */
static void acpi_power_on_device(struct acpi_power_managed_device *device)
{
	struct acpi_device *acpi_dev;
	acpi_handle handle = device->handle;
	int state;

	if (acpi_bus_get_device(handle, &acpi_dev))
		return;

	if(acpi_power_get_inferred_state(acpi_dev, &state))
		return;

	if (state == ACPI_STATE_D0 && pm_runtime_suspended(device->dev))
		pm_request_resume(device->dev);
=======
	pr_debug("Power resource list is %s\n", cur_state ? "on" : "off");

	*state = cur_state;
	return 0;
}

static int
acpi_power_resource_add_dependent(struct acpi_power_resource *resource,
				  struct device *dev)
{
	struct acpi_power_dependent_device *dep;
	int ret = 0;

	mutex_lock(&resource->resource_lock);
	list_for_each_entry(dep, &resource->dependents, node) {
		/* Only add it once */
		if (dep->dev == dev)
			goto unlock;
	}

	dep = kzalloc(sizeof(*dep), GFP_KERNEL);
	if (!dep) {
		ret = -ENOMEM;
		goto unlock;
	}

	dep->dev = dev;
	list_add_tail(&dep->node, &resource->dependents);
	dev_dbg(dev, "added power dependency to [%s]\n",
		resource_dev_name(resource));

unlock:
	mutex_unlock(&resource->resource_lock);
	return ret;
}

static void
acpi_power_resource_remove_dependent(struct acpi_power_resource *resource,
				     struct device *dev)
{
	struct acpi_power_dependent_device *dep;

	mutex_lock(&resource->resource_lock);
	list_for_each_entry(dep, &resource->dependents, node) {
		if (dep->dev == dev) {
			list_del(&dep->node);
			kfree(dep);
			dev_dbg(dev, "removed power dependency to [%s]\n",
				resource_dev_name(resource));
			break;
		}
	}
	mutex_unlock(&resource->resource_lock);
}

/**
 * acpi_device_power_add_dependent - Add dependent device of this ACPI device
 * @adev: ACPI device pointer
 * @dev: Dependent device
 *
 * If @adev has non-empty _PR0 the @dev is added as dependent device to all
 * power resources returned by it. This means that whenever these power
 * resources are turned _ON the dependent devices get runtime resumed. This
 * is needed for devices such as PCI to allow its driver to re-initialize
 * it after it went to D0uninitialized.
 *
 * If @adev does not have _PR0 this does nothing.
 *
 * Returns %0 in case of success and negative errno otherwise.
 */
int acpi_device_power_add_dependent(struct acpi_device *adev,
				    struct device *dev)
{
	struct acpi_power_resource_entry *entry;
	struct list_head *resources;
	int ret;

	if (!adev->flags.power_manageable)
		return 0;

	resources = &adev->power.states[ACPI_STATE_D0].resources;
	list_for_each_entry(entry, resources, node) {
		ret = acpi_power_resource_add_dependent(entry->resource, dev);
		if (ret)
			goto err;
	}

	return 0;

err:
	list_for_each_entry(entry, resources, node)
		acpi_power_resource_remove_dependent(entry->resource, dev);

	return ret;
}

/**
 * acpi_device_power_remove_dependent - Remove dependent device
 * @adev: ACPI device pointer
 * @dev: Dependent device
 *
 * Does the opposite of acpi_device_power_add_dependent() and removes the
 * dependent device if it is found. Can be called to @adev that does not
 * have _PR0 as well.
 */
void acpi_device_power_remove_dependent(struct acpi_device *adev,
					struct device *dev)
{
	struct acpi_power_resource_entry *entry;
	struct list_head *resources;

	if (!adev->flags.power_manageable)
		return;

	resources = &adev->power.states[ACPI_STATE_D0].resources;
	list_for_each_entry_reverse(entry, resources, node)
		acpi_power_resource_remove_dependent(entry->resource, dev);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

static int __acpi_power_on(struct acpi_power_resource *resource)
{
<<<<<<< HEAD
	acpi_status status = AE_OK;

	status = acpi_evaluate_object(resource->device->handle, "_ON", NULL, NULL);
	if (ACPI_FAILURE(status))
		return -ENODEV;

	/* Update the power resource's _device_ power state */
	resource->device->power.state = ACPI_STATE_D0;

	ACPI_DEBUG_PRINT((ACPI_DB_INFO, "Power resource [%s] turned on\n",
			  resource->name));
=======
	acpi_handle handle = resource->device.handle;
	struct acpi_power_dependent_device *dep;
	acpi_status status = AE_OK;

	status = acpi_evaluate_object(handle, "_ON", NULL, NULL);
	if (ACPI_FAILURE(status)) {
		resource->state = ACPI_POWER_RESOURCE_STATE_UNKNOWN;
		return -ENODEV;
	}

	resource->state = ACPI_POWER_RESOURCE_STATE_ON;

	acpi_handle_debug(handle, "Power resource turned on\n");

	/*
	 * If there are other dependents on this power resource we need to
	 * resume them now so that their drivers can re-initialize the
	 * hardware properly after it went back to D0.
	 */
	if (list_empty(&resource->dependents) ||
	    list_is_singular(&resource->dependents))
		return 0;

	list_for_each_entry(dep, &resource->dependents, node) {
		dev_dbg(dep->dev, "runtime resuming because [%s] turned on\n",
			resource_dev_name(resource));
		pm_request_resume(dep->dev);
	}
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	return 0;
}

<<<<<<< HEAD
static int acpi_power_on(acpi_handle handle)
{
	int result = 0;
	bool resume_device = false;
	struct acpi_power_resource *resource = NULL;
	struct acpi_power_resource_device *device_list;

	result = acpi_power_get_context(handle, &resource);
	if (result)
		return result;

	mutex_lock(&resource->resource_lock);

	if (resource->ref_count++) {
		ACPI_DEBUG_PRINT((ACPI_DB_INFO,
				  "Power resource [%s] already on",
				  resource->name));
=======
static int acpi_power_on_unlocked(struct acpi_power_resource *resource)
{
	int result = 0;

	if (resource->ref_count++) {
		acpi_handle_debug(resource->device.handle,
				  "Power resource already on\n");
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	} else {
		result = __acpi_power_on(resource);
		if (result)
			resource->ref_count--;
<<<<<<< HEAD
		else
			resume_device = true;
	}

	mutex_unlock(&resource->resource_lock);

	if (!resume_device)
		return result;

	mutex_lock(&resource->devices_lock);

	device_list = resource->devices;
	while (device_list) {
		acpi_power_on_device(device_list->device);
		device_list = device_list->next;
	}

	mutex_unlock(&resource->devices_lock);

	return result;
}

static int acpi_power_off(acpi_handle handle)
{
	int result = 0;
	acpi_status status = AE_OK;
	struct acpi_power_resource *resource = NULL;

	result = acpi_power_get_context(handle, &resource);
	if (result)
		return result;

	mutex_lock(&resource->resource_lock);

	if (!resource->ref_count) {
		ACPI_DEBUG_PRINT((ACPI_DB_INFO,
				  "Power resource [%s] already off",
				  resource->name));
		goto unlock;
	}

	if (--resource->ref_count) {
		ACPI_DEBUG_PRINT((ACPI_DB_INFO,
				  "Power resource [%s] still in use\n",
				  resource->name));
		goto unlock;
	}

	status = acpi_evaluate_object(resource->device->handle, "_OFF", NULL, NULL);
	if (ACPI_FAILURE(status)) {
		result = -ENODEV;
	} else {
		/* Update the power resource's _device_ power state */
		resource->device->power.state = ACPI_STATE_D3;

		ACPI_DEBUG_PRINT((ACPI_DB_INFO,
				  "Power resource [%s] turned off\n",
				  resource->name));
	}

 unlock:
	mutex_unlock(&resource->resource_lock);

	return result;
}

static void __acpi_power_off_list(struct acpi_handle_list *list, int num_res)
{
	int i;

	for (i = num_res - 1; i >= 0 ; i--)
		acpi_power_off(list->handles[i]);
}

static void acpi_power_off_list(struct acpi_handle_list *list)
{
	__acpi_power_off_list(list, list->count);
}

static int acpi_power_on_list(struct acpi_handle_list *list)
{
	int result = 0;
	int i;

	for (i = 0; i < list->count; i++) {
		result = acpi_power_on(list->handles[i]);
		if (result) {
			__acpi_power_off_list(list, i);
			break;
		}
	}

	return result;
}

static void __acpi_power_resource_unregister_device(struct device *dev,
		acpi_handle res_handle)
{
	struct acpi_power_resource *resource = NULL;
	struct acpi_power_resource_device *prev, *curr;

	if (acpi_power_get_context(res_handle, &resource))
		return;

	mutex_lock(&resource->devices_lock);
	prev = NULL;
	curr = resource->devices;
	while (curr) {
		if (curr->device->dev == dev) {
			if (!prev)
				resource->devices = curr->next;
			else
				prev->next = curr->next;

			kfree(curr);
			break;
		}

		prev = curr;
		curr = curr->next;
	}
	mutex_unlock(&resource->devices_lock);
}

/* Unlink dev from all power resources in _PR0 */
void acpi_power_resource_unregister_device(struct device *dev, acpi_handle handle)
{
	struct acpi_device *acpi_dev;
	struct acpi_handle_list *list;
	int i;

	if (!dev || !handle)
		return;

	if (acpi_bus_get_device(handle, &acpi_dev))
		return;

	list = &acpi_dev->power.states[ACPI_STATE_D0].resources;

	for (i = 0; i < list->count; i++)
		__acpi_power_resource_unregister_device(dev,
			list->handles[i]);
}

static int __acpi_power_resource_register_device(
	struct acpi_power_managed_device *powered_device, acpi_handle handle)
{
	struct acpi_power_resource *resource = NULL;
	struct acpi_power_resource_device *power_resource_device;
	int result;

	result = acpi_power_get_context(handle, &resource);
	if (result)
		return result;

	power_resource_device = kzalloc(
		sizeof(*power_resource_device), GFP_KERNEL);
	if (!power_resource_device)
		return -ENOMEM;

	power_resource_device->device = powered_device;

	mutex_lock(&resource->devices_lock);
	power_resource_device->next = resource->devices;
	resource->devices = power_resource_device;
	mutex_unlock(&resource->devices_lock);
=======
	}
	return result;
}

static int acpi_power_on(struct acpi_power_resource *resource)
{
	int result;

	mutex_lock(&resource->resource_lock);
	result = acpi_power_on_unlocked(resource);
	mutex_unlock(&resource->resource_lock);
	return result;
}

static int __acpi_power_off(struct acpi_power_resource *resource)
{
	acpi_handle handle = resource->device.handle;
	acpi_status status;

	status = acpi_evaluate_object(handle, "_OFF", NULL, NULL);
	if (ACPI_FAILURE(status)) {
		resource->state = ACPI_POWER_RESOURCE_STATE_UNKNOWN;
		return -ENODEV;
	}

	resource->state = ACPI_POWER_RESOURCE_STATE_OFF;

	acpi_handle_debug(handle, "Power resource turned off\n");
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	return 0;
}

<<<<<<< HEAD
/* Link dev to all power resources in _PR0 */
int acpi_power_resource_register_device(struct device *dev, acpi_handle handle)
{
	struct acpi_device *acpi_dev;
	struct acpi_handle_list *list;
	struct acpi_power_managed_device *powered_device;
	int i, ret;

	if (!dev || !handle)
		return -ENODEV;

	ret = acpi_bus_get_device(handle, &acpi_dev);
	if (ret)
		goto no_power_resource;

	if (!acpi_dev->power.flags.power_resources)
		goto no_power_resource;

	powered_device = kzalloc(sizeof(*powered_device), GFP_KERNEL);
	if (!powered_device)
		return -ENOMEM;

	powered_device->dev = dev;
	powered_device->handle = handle;

	list = &acpi_dev->power.states[ACPI_STATE_D0].resources;

	for (i = 0; i < list->count; i++) {
		ret = __acpi_power_resource_register_device(powered_device,
			list->handles[i]);

		if (ret) {
			acpi_power_resource_unregister_device(dev, handle);
			break;
		}
	}

	return ret;

no_power_resource:
	printk(KERN_DEBUG PREFIX "Invalid Power Resource to register!");
	return -ENODEV;
}

=======
static int acpi_power_off_unlocked(struct acpi_power_resource *resource)
{
	int result = 0;

	if (!resource->ref_count) {
		acpi_handle_debug(resource->device.handle,
				  "Power resource already off\n");
		return 0;
	}

	if (--resource->ref_count) {
		acpi_handle_debug(resource->device.handle,
				  "Power resource still in use\n");
	} else {
		result = __acpi_power_off(resource);
		if (result)
			resource->ref_count++;
	}
	return result;
}

static int acpi_power_off(struct acpi_power_resource *resource)
{
	int result;

	mutex_lock(&resource->resource_lock);
	result = acpi_power_off_unlocked(resource);
	mutex_unlock(&resource->resource_lock);
	return result;
}

static int acpi_power_off_list(struct list_head *list)
{
	struct acpi_power_resource_entry *entry;
	int result = 0;

	list_for_each_entry_reverse(entry, list, node) {
		result = acpi_power_off(entry->resource);
		if (result)
			goto err;
	}
	return 0;

 err:
	list_for_each_entry_continue(entry, list, node)
		acpi_power_on(entry->resource);

	return result;
}

static int acpi_power_on_list(struct list_head *list)
{
	struct acpi_power_resource_entry *entry;
	int result = 0;

	list_for_each_entry(entry, list, node) {
		result = acpi_power_on(entry->resource);
		if (result)
			goto err;
	}
	return 0;

 err:
	list_for_each_entry_continue_reverse(entry, list, node)
		acpi_power_off(entry->resource);

	return result;
}

static struct attribute *attrs[] = {
	NULL,
};

static const struct attribute_group attr_groups[] = {
	[ACPI_STATE_D0] = {
		.name = "power_resources_D0",
		.attrs = attrs,
	},
	[ACPI_STATE_D1] = {
		.name = "power_resources_D1",
		.attrs = attrs,
	},
	[ACPI_STATE_D2] = {
		.name = "power_resources_D2",
		.attrs = attrs,
	},
	[ACPI_STATE_D3_HOT] = {
		.name = "power_resources_D3hot",
		.attrs = attrs,
	},
};

static const struct attribute_group wakeup_attr_group = {
	.name = "power_resources_wakeup",
	.attrs = attrs,
};

static void acpi_power_hide_list(struct acpi_device *adev,
				 struct list_head *resources,
				 const struct attribute_group *attr_group)
{
	struct acpi_power_resource_entry *entry;

	if (list_empty(resources))
		return;

	list_for_each_entry_reverse(entry, resources, node) {
		struct acpi_device *res_dev = &entry->resource->device;

		sysfs_remove_link_from_group(&adev->dev.kobj,
					     attr_group->name,
					     dev_name(&res_dev->dev));
	}
	sysfs_remove_group(&adev->dev.kobj, attr_group);
}

static void acpi_power_expose_list(struct acpi_device *adev,
				   struct list_head *resources,
				   const struct attribute_group *attr_group)
{
	struct acpi_power_resource_entry *entry;
	int ret;

	if (list_empty(resources))
		return;

	ret = sysfs_create_group(&adev->dev.kobj, attr_group);
	if (ret)
		return;

	list_for_each_entry(entry, resources, node) {
		struct acpi_device *res_dev = &entry->resource->device;

		ret = sysfs_add_link_to_group(&adev->dev.kobj,
					      attr_group->name,
					      &res_dev->dev.kobj,
					      dev_name(&res_dev->dev));
		if (ret) {
			acpi_power_hide_list(adev, resources, attr_group);
			break;
		}
	}
}

static void acpi_power_expose_hide(struct acpi_device *adev,
				   struct list_head *resources,
				   const struct attribute_group *attr_group,
				   bool expose)
{
	if (expose)
		acpi_power_expose_list(adev, resources, attr_group);
	else
		acpi_power_hide_list(adev, resources, attr_group);
}

void acpi_power_add_remove_device(struct acpi_device *adev, bool add)
{
	int state;

	if (adev->wakeup.flags.valid)
		acpi_power_expose_hide(adev, &adev->wakeup.resources,
				       &wakeup_attr_group, add);

	if (!adev->power.flags.power_resources)
		return;

	for (state = ACPI_STATE_D0; state <= ACPI_STATE_D3_HOT; state++)
		acpi_power_expose_hide(adev,
				       &adev->power.states[state].resources,
				       &attr_groups[state], add);
}

int acpi_power_wakeup_list_init(struct list_head *list, int *system_level_p)
{
	struct acpi_power_resource_entry *entry;
	int system_level = 5;

	list_for_each_entry(entry, list, node) {
		struct acpi_power_resource *resource = entry->resource;
		u8 state;

		mutex_lock(&resource->resource_lock);

		/*
		 * Make sure that the power resource state and its reference
		 * counter value are consistent with each other.
		 */
		if (!resource->ref_count &&
		    !acpi_power_get_state(resource, &state) &&
		    state == ACPI_POWER_RESOURCE_STATE_ON)
			__acpi_power_off(resource);

		if (system_level > resource->system_level)
			system_level = resource->system_level;

		mutex_unlock(&resource->resource_lock);
	}
	*system_level_p = system_level;
	return 0;
}

/* --------------------------------------------------------------------------
                             Device Power Management
   -------------------------------------------------------------------------- */

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
/**
 * acpi_device_sleep_wake - execute _DSW (Device Sleep Wake) or (deprecated in
 *                          ACPI 3.0) _PSW (Power State Wake)
 * @dev: Device to handle.
 * @enable: 0 - disable, 1 - enable the wake capabilities of the device.
 * @sleep_state: Target sleep state of the system.
 * @dev_state: Target power state of the device.
 *
 * Execute _DSW (Device Sleep Wake) or (deprecated in ACPI 3.0) _PSW (Power
 * State Wake) for the device, if present.  On failure reset the device's
 * wakeup.flags.valid flag.
 *
 * RETURN VALUE:
 * 0 if either _DSW or _PSW has been successfully executed
 * 0 if neither _DSW nor _PSW has been found
 * -ENODEV if the execution of either _DSW or _PSW has failed
 */
int acpi_device_sleep_wake(struct acpi_device *dev,
<<<<<<< HEAD
                           int enable, int sleep_state, int dev_state)
=======
			   int enable, int sleep_state, int dev_state)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
{
	union acpi_object in_arg[3];
	struct acpi_object_list arg_list = { 3, in_arg };
	acpi_status status = AE_OK;

	/*
	 * Try to execute _DSW first.
	 *
<<<<<<< HEAD
	 * Three agruments are needed for the _DSW object:
=======
	 * Three arguments are needed for the _DSW object:
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	 * Argument 0: enable/disable the wake capabilities
	 * Argument 1: target system state
	 * Argument 2: target device state
	 * When _DSW object is called to disable the wake capabilities, maybe
<<<<<<< HEAD
	 * the first argument is filled. The values of the other two agruments
=======
	 * the first argument is filled. The values of the other two arguments
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	 * are meaningless.
	 */
	in_arg[0].type = ACPI_TYPE_INTEGER;
	in_arg[0].integer.value = enable;
	in_arg[1].type = ACPI_TYPE_INTEGER;
	in_arg[1].integer.value = sleep_state;
	in_arg[2].type = ACPI_TYPE_INTEGER;
	in_arg[2].integer.value = dev_state;
	status = acpi_evaluate_object(dev->handle, "_DSW", &arg_list, NULL);
	if (ACPI_SUCCESS(status)) {
		return 0;
	} else if (status != AE_NOT_FOUND) {
<<<<<<< HEAD
		printk(KERN_ERR PREFIX "_DSW execution failed\n");
=======
		acpi_handle_info(dev->handle, "_DSW execution failed\n");
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		dev->wakeup.flags.valid = 0;
		return -ENODEV;
	}

	/* Execute _PSW */
<<<<<<< HEAD
	arg_list.count = 1;
	in_arg[0].integer.value = enable;
	status = acpi_evaluate_object(dev->handle, "_PSW", &arg_list, NULL);
	if (ACPI_FAILURE(status) && (status != AE_NOT_FOUND)) {
		printk(KERN_ERR PREFIX "_PSW execution failed\n");
=======
	status = acpi_execute_simple_method(dev->handle, "_PSW", enable);
	if (ACPI_FAILURE(status) && (status != AE_NOT_FOUND)) {
		acpi_handle_info(dev->handle, "_PSW execution failed\n");
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		dev->wakeup.flags.valid = 0;
		return -ENODEV;
	}

	return 0;
}

/*
 * Prepare a wakeup device, two steps (Ref ACPI 2.0:P229):
<<<<<<< HEAD
 * 1. Power on the power resources required for the wakeup device 
=======
 * 1. Power on the power resources required for the wakeup device
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 * 2. Execute _DSW (Device Sleep Wake) or (deprecated in ACPI 3.0) _PSW (Power
 *    State Wake) for the device, if present
 */
int acpi_enable_wakeup_device_power(struct acpi_device *dev, int sleep_state)
{
<<<<<<< HEAD
	int i, err = 0;
=======
	int err = 0;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	if (!dev || !dev->wakeup.flags.valid)
		return -EINVAL;

	mutex_lock(&acpi_device_lock);

<<<<<<< HEAD
	if (dev->wakeup.prepare_count++)
		goto out;

	/* Open power resource */
	for (i = 0; i < dev->wakeup.resources.count; i++) {
		int ret = acpi_power_on(dev->wakeup.resources.handles[i]);
		if (ret) {
			printk(KERN_ERR PREFIX "Transition power state\n");
			dev->wakeup.flags.valid = 0;
			err = -ENODEV;
			goto err_out;
		}
	}

	/*
	 * Passing 3 as the third argument below means the device may be placed
	 * in arbitrary power state afterwards.
	 */
	err = acpi_device_sleep_wake(dev, 1, sleep_state, 3);

 err_out:
	if (err)
		dev->wakeup.prepare_count = 0;
=======
	dev_dbg(&dev->dev, "Enabling wakeup power (count %d)\n",
		dev->wakeup.prepare_count);

	if (dev->wakeup.prepare_count++)
		goto out;

	err = acpi_power_on_list(&dev->wakeup.resources);
	if (err) {
		dev_err(&dev->dev, "Cannot turn on wakeup power resources\n");
		dev->wakeup.flags.valid = 0;
		goto out;
	}

	/*
	 * Passing 3 as the third argument below means the device may be
	 * put into arbitrary power state afterward.
	 */
	err = acpi_device_sleep_wake(dev, 1, sleep_state, 3);
	if (err) {
		acpi_power_off_list(&dev->wakeup.resources);
		dev->wakeup.prepare_count = 0;
		goto out;
	}

	dev_dbg(&dev->dev, "Wakeup power enabled\n");
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

 out:
	mutex_unlock(&acpi_device_lock);
	return err;
}

/*
 * Shutdown a wakeup device, counterpart of above method
 * 1. Execute _DSW (Device Sleep Wake) or (deprecated in ACPI 3.0) _PSW (Power
 *    State Wake) for the device, if present
 * 2. Shutdown down the power resources
 */
int acpi_disable_wakeup_device_power(struct acpi_device *dev)
{
<<<<<<< HEAD
	int i, err = 0;
=======
	struct acpi_power_resource_entry *entry;
	int err = 0;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	if (!dev || !dev->wakeup.flags.valid)
		return -EINVAL;

	mutex_lock(&acpi_device_lock);

<<<<<<< HEAD
	if (--dev->wakeup.prepare_count > 0)
		goto out;

	/*
	 * Executing the code below even if prepare_count is already zero when
	 * the function is called may be useful, for example for initialisation.
	 */
	if (dev->wakeup.prepare_count < 0)
		dev->wakeup.prepare_count = 0;
=======
	dev_dbg(&dev->dev, "Disabling wakeup power (count %d)\n",
		dev->wakeup.prepare_count);

	/* Do nothing if wakeup power has not been enabled for this device. */
	if (dev->wakeup.prepare_count <= 0)
		goto out;

	if (--dev->wakeup.prepare_count > 0)
		goto out;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	err = acpi_device_sleep_wake(dev, 0, 0, 0);
	if (err)
		goto out;

<<<<<<< HEAD
	/* Close power resource */
	for (i = 0; i < dev->wakeup.resources.count; i++) {
		int ret = acpi_power_off(dev->wakeup.resources.handles[i]);
		if (ret) {
			printk(KERN_ERR PREFIX "Transition power state\n");
			dev->wakeup.flags.valid = 0;
			err = -ENODEV;
			goto out;
		}
	}
=======
	/*
	 * All of the power resources in the list need to be turned off even if
	 * there are errors.
	 */
	list_for_each_entry(entry, &dev->wakeup.resources, node) {
		int ret;

		ret = acpi_power_off(entry->resource);
		if (ret && !err)
			err = ret;
	}
	if (err) {
		dev_err(&dev->dev, "Cannot turn off wakeup power resources\n");
		dev->wakeup.flags.valid = 0;
		goto out;
	}

	dev_dbg(&dev->dev, "Wakeup power disabled\n");
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

 out:
	mutex_unlock(&acpi_device_lock);
	return err;
}

<<<<<<< HEAD
/* --------------------------------------------------------------------------
                             Device Power Management
   -------------------------------------------------------------------------- */

int acpi_power_get_inferred_state(struct acpi_device *device, int *state)
{
	int result = 0;
	struct acpi_handle_list *list = NULL;
	int list_state = 0;
=======
int acpi_power_get_inferred_state(struct acpi_device *device, int *state)
{
	u8 list_state = ACPI_POWER_RESOURCE_STATE_OFF;
	int result = 0;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	int i = 0;

	if (!device || !state)
		return -EINVAL;

	/*
	 * We know a device's inferred power state when all the resources
	 * required for a given D-state are 'on'.
	 */
<<<<<<< HEAD
	for (i = ACPI_STATE_D0; i < ACPI_STATE_D3_HOT; i++) {
		list = &device->power.states[i].resources;
		if (list->count < 1)
=======
	for (i = ACPI_STATE_D0; i <= ACPI_STATE_D3_HOT; i++) {
		struct list_head *list = &device->power.states[i].resources;

		if (list_empty(list))
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
			continue;

		result = acpi_power_get_list_state(list, &list_state);
		if (result)
			return result;

		if (list_state == ACPI_POWER_RESOURCE_STATE_ON) {
			*state = i;
			return 0;
		}
	}

<<<<<<< HEAD
	*state = ACPI_STATE_D3;
=======
	*state = device->power.states[ACPI_STATE_D3_COLD].flags.valid ?
		ACPI_STATE_D3_COLD : ACPI_STATE_D3_HOT;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	return 0;
}

int acpi_power_on_resources(struct acpi_device *device, int state)
{
<<<<<<< HEAD
	if (!device || state < ACPI_STATE_D0 || state > ACPI_STATE_D3)
=======
	if (!device || state < ACPI_STATE_D0 || state > ACPI_STATE_D3_HOT)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		return -EINVAL;

	return acpi_power_on_list(&device->power.states[state].resources);
}

int acpi_power_transition(struct acpi_device *device, int state)
{
	int result = 0;

	if (!device || (state < ACPI_STATE_D0) || (state > ACPI_STATE_D3_COLD))
		return -EINVAL;

<<<<<<< HEAD
	if (device->power.state == state)
=======
	if (device->power.state == state || !device->flags.power_manageable)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		return 0;

	if ((device->power.state < ACPI_STATE_D0)
	    || (device->power.state > ACPI_STATE_D3_COLD))
		return -ENODEV;

<<<<<<< HEAD
	/* TBD: Resources must be ordered. */

=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	/*
	 * First we reference all power resources required in the target list
	 * (e.g. so the device doesn't lose power while transitioning).  Then,
	 * we dereference all power resources used in the current list.
	 */
	if (state < ACPI_STATE_D3_COLD)
		result = acpi_power_on_list(
			&device->power.states[state].resources);

	if (!result && device->power.state < ACPI_STATE_D3_COLD)
		acpi_power_off_list(
			&device->power.states[device->power.state].resources);

	/* We shouldn't change the state unless the above operations succeed. */
	device->power.state = result ? ACPI_STATE_UNKNOWN : state;

	return result;
}

<<<<<<< HEAD
/* --------------------------------------------------------------------------
                                Driver Interface
   -------------------------------------------------------------------------- */

static int acpi_power_add(struct acpi_device *device)
{
	int result = 0, state;
	acpi_status status = AE_OK;
	struct acpi_power_resource *resource = NULL;
	union acpi_object acpi_object;
	struct acpi_buffer buffer = { sizeof(acpi_object), &acpi_object };


	if (!device)
		return -EINVAL;

	resource = kzalloc(sizeof(struct acpi_power_resource), GFP_KERNEL);
	if (!resource)
		return -ENOMEM;

	resource->device = device;
	mutex_init(&resource->resource_lock);
	mutex_init(&resource->devices_lock);
	strcpy(resource->name, device->pnp.bus_id);
	strcpy(acpi_device_name(device), ACPI_POWER_DEVICE_NAME);
	strcpy(acpi_device_class(device), ACPI_POWER_CLASS);
	device->driver_data = resource;

	/* Evalute the object to get the system level and resource order. */
	status = acpi_evaluate_object(device->handle, NULL, NULL, &buffer);
	if (ACPI_FAILURE(status)) {
		result = -ENODEV;
		goto end;
	}
	resource->system_level = acpi_object.power_resource.system_level;
	resource->order = acpi_object.power_resource.resource_order;

	result = acpi_power_get_state(device->handle, &state);
	if (result)
		goto end;

	switch (state) {
	case ACPI_POWER_RESOURCE_STATE_ON:
		device->power.state = ACPI_STATE_D0;
		break;
	case ACPI_POWER_RESOURCE_STATE_OFF:
		device->power.state = ACPI_STATE_D3;
		break;
	default:
		device->power.state = ACPI_STATE_UNKNOWN;
		break;
	}

	printk(KERN_INFO PREFIX "%s [%s] (%s)\n", acpi_device_name(device),
	       acpi_device_bid(device), state ? "on" : "off");

      end:
	if (result)
		kfree(resource);

	return result;
}

static int acpi_power_remove(struct acpi_device *device, int type)
{
	struct acpi_power_resource *resource;

	if (!device)
		return -EINVAL;

	resource = acpi_driver_data(device);
	if (!resource)
		return -EINVAL;

	kfree(resource);

	return 0;
}

static int acpi_power_resume(struct acpi_device *device)
{
	int result = 0, state;
	struct acpi_power_resource *resource;

	if (!device)
		return -EINVAL;

	resource = acpi_driver_data(device);
	if (!resource)
		return -EINVAL;

	mutex_lock(&resource->resource_lock);

	result = acpi_power_get_state(device->handle, &state);
	if (result)
		goto unlock;

	if (state == ACPI_POWER_RESOURCE_STATE_OFF && resource->ref_count)
		result = __acpi_power_on(resource);

 unlock:
	mutex_unlock(&resource->resource_lock);

	return result;
}

int __init acpi_power_init(void)
{
	INIT_LIST_HEAD(&acpi_power_resource_list);
	return acpi_bus_register_driver(&acpi_power_driver);
=======
static void acpi_release_power_resource(struct device *dev)
{
	struct acpi_device *device = to_acpi_device(dev);
	struct acpi_power_resource *resource;

	resource = container_of(device, struct acpi_power_resource, device);

	mutex_lock(&power_resource_list_lock);
	list_del(&resource->list_node);
	mutex_unlock(&power_resource_list_lock);

	acpi_free_pnp_ids(&device->pnp);
	kfree(resource);
}

static ssize_t resource_in_use_show(struct device *dev,
				    struct device_attribute *attr,
				    char *buf)
{
	struct acpi_power_resource *resource;

	resource = to_power_resource(to_acpi_device(dev));
	return sprintf(buf, "%u\n", !!resource->ref_count);
}
static DEVICE_ATTR_RO(resource_in_use);

static void acpi_power_sysfs_remove(struct acpi_device *device)
{
	device_remove_file(&device->dev, &dev_attr_resource_in_use);
}

static void acpi_power_add_resource_to_list(struct acpi_power_resource *resource)
{
	mutex_lock(&power_resource_list_lock);

	if (!list_empty(&acpi_power_resource_list)) {
		struct acpi_power_resource *r;

		list_for_each_entry(r, &acpi_power_resource_list, list_node)
			if (r->order > resource->order) {
				list_add_tail(&resource->list_node, &r->list_node);
				goto out;
			}
	}
	list_add_tail(&resource->list_node, &acpi_power_resource_list);

 out:
	mutex_unlock(&power_resource_list_lock);
}

struct acpi_device *acpi_add_power_resource(acpi_handle handle)
{
	struct acpi_device *device = acpi_fetch_acpi_dev(handle);
	struct acpi_power_resource *resource;
	union acpi_object acpi_object;
	struct acpi_buffer buffer = { sizeof(acpi_object), &acpi_object };
	acpi_status status;
	u8 state_dummy;
	int result;

	if (device)
		return device;

	resource = kzalloc(sizeof(*resource), GFP_KERNEL);
	if (!resource)
		return NULL;

	device = &resource->device;
	acpi_init_device_object(device, handle, ACPI_BUS_TYPE_POWER,
				acpi_release_power_resource);
	mutex_init(&resource->resource_lock);
	INIT_LIST_HEAD(&resource->list_node);
	INIT_LIST_HEAD(&resource->dependents);
	strcpy(acpi_device_name(device), ACPI_POWER_DEVICE_NAME);
	strcpy(acpi_device_class(device), ACPI_POWER_CLASS);
	device->power.state = ACPI_STATE_UNKNOWN;
	device->flags.match_driver = true;

	/* Evaluate the object to get the system level and resource order. */
	status = acpi_evaluate_object(handle, NULL, NULL, &buffer);
	if (ACPI_FAILURE(status))
		goto err;

	resource->system_level = acpi_object.power_resource.system_level;
	resource->order = acpi_object.power_resource.resource_order;
	resource->state = ACPI_POWER_RESOURCE_STATE_UNKNOWN;

	/* Get the initial state or just flip it on if that fails. */
	if (acpi_power_get_state(resource, &state_dummy))
		__acpi_power_on(resource);

	acpi_handle_info(handle, "New power resource\n");

	result = acpi_tie_acpi_dev(device);
	if (result)
		goto err;

	result = acpi_device_add(device);
	if (result)
		goto err;

	if (!device_create_file(&device->dev, &dev_attr_resource_in_use))
		device->remove = acpi_power_sysfs_remove;

	acpi_power_add_resource_to_list(resource);
	acpi_device_add_finalize(device);
	return device;

 err:
	acpi_release_power_resource(&device->dev);
	return NULL;
}

#ifdef CONFIG_ACPI_SLEEP
void acpi_resume_power_resources(void)
{
	struct acpi_power_resource *resource;

	mutex_lock(&power_resource_list_lock);

	list_for_each_entry(resource, &acpi_power_resource_list, list_node) {
		int result;
		u8 state;

		mutex_lock(&resource->resource_lock);

		resource->state = ACPI_POWER_RESOURCE_STATE_UNKNOWN;
		result = acpi_power_get_state(resource, &state);
		if (result) {
			mutex_unlock(&resource->resource_lock);
			continue;
		}

		if (state == ACPI_POWER_RESOURCE_STATE_OFF
		    && resource->ref_count) {
			acpi_handle_debug(resource->device.handle, "Turning ON\n");
			__acpi_power_on(resource);
		}

		mutex_unlock(&resource->resource_lock);
	}

	mutex_unlock(&power_resource_list_lock);
}
#endif

static const struct dmi_system_id dmi_leave_unused_power_resources_on[] = {
	{
		/*
		 * The Toshiba Click Mini has a CPR3 power-resource which must
		 * be on for the touchscreen to work, but which is not in any
		 * _PR? lists. The other 2 affected power-resources are no-ops.
		 */
		.matches = {
			DMI_MATCH(DMI_SYS_VENDOR, "TOSHIBA"),
			DMI_MATCH(DMI_PRODUCT_NAME, "SATELLITE Click Mini L9W-B"),
		},
	},
	{}
};

/**
 * acpi_turn_off_unused_power_resources - Turn off power resources not in use.
 */
void acpi_turn_off_unused_power_resources(void)
{
	struct acpi_power_resource *resource;

	if (dmi_check_system(dmi_leave_unused_power_resources_on))
		return;

	mutex_lock(&power_resource_list_lock);

	list_for_each_entry_reverse(resource, &acpi_power_resource_list, list_node) {
		mutex_lock(&resource->resource_lock);

		if (!resource->ref_count &&
		    resource->state == ACPI_POWER_RESOURCE_STATE_ON) {
			acpi_handle_debug(resource->device.handle, "Turning OFF\n");
			__acpi_power_off(resource);
		}

		mutex_unlock(&resource->resource_lock);
	}

	mutex_unlock(&power_resource_list_lock);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}
