<<<<<<< HEAD
=======
// SPDX-License-Identifier: GPL-2.0-only
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
/*
 * scan.c - support for transforming the ACPI namespace into individual objects
 */

<<<<<<< HEAD
=======
#define pr_fmt(fmt) "ACPI: " fmt

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#include <linux/module.h>
#include <linux/init.h>
#include <linux/slab.h>
#include <linux/kernel.h>
#include <linux/acpi.h>
<<<<<<< HEAD
#include <linux/signal.h>
#include <linux/kthread.h>
#include <linux/dmi.h>

#include <acpi/acpi_drivers.h>

#include "internal.h"

#define _COMPONENT		ACPI_BUS_COMPONENT
ACPI_MODULE_NAME("scan");
#define STRUCT_TO_INT(s)	(*((int*)&s))
extern struct acpi_device *acpi_root;
=======
#include <linux/acpi_iort.h>
#include <linux/acpi_viot.h>
#include <linux/iommu.h>
#include <linux/signal.h>
#include <linux/kthread.h>
#include <linux/dmi.h>
#include <linux/dma-map-ops.h>
#include <linux/platform_data/x86/apple.h>
#include <linux/pgtable.h>
#include <linux/crc32.h>
#include <linux/dma-direct.h>

#include "internal.h"
#include "sleep.h"
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

#define ACPI_BUS_CLASS			"system_bus"
#define ACPI_BUS_HID			"LNXSYBUS"
#define ACPI_BUS_DEVICE_NAME		"System Bus"

<<<<<<< HEAD
#define ACPI_IS_ROOT_DEVICE(device)    (!(device)->parent)

static const char *dummy_hid = "device";

static LIST_HEAD(acpi_device_list);
static LIST_HEAD(acpi_bus_id_list);
DEFINE_MUTEX(acpi_device_lock);
LIST_HEAD(acpi_wakeup_device_list);

struct acpi_device_bus_id{
	char bus_id[15];
	unsigned int instance_no;
	struct list_head node;
};

/*
 * Creates hid/cid(s) string needed for modalias and uevent
 * e.g. on a device with hid:IBM0001 and cid:ACPI0001 you get:
 * char *modalias: "acpi:IBM0001:ACPI0001"
*/
static int create_modalias(struct acpi_device *acpi_dev, char *modalias,
			   int size)
{
	int len;
	int count;
	struct acpi_hardware_id *id;

	if (list_empty(&acpi_dev->pnp.ids))
		return 0;

	len = snprintf(modalias, size, "acpi:");
	size -= len;

	list_for_each_entry(id, &acpi_dev->pnp.ids, list) {
		count = snprintf(&modalias[len], size, "%s:", id->id);
		if (count < 0 || count >= size)
			return -EINVAL;
		len += count;
		size -= count;
	}

	modalias[len] = '\0';
	return len;
}

static ssize_t
acpi_device_modalias_show(struct device *dev, struct device_attribute *attr, char *buf) {
	struct acpi_device *acpi_dev = to_acpi_device(dev);
	int len;

	/* Device has no HID and no CID or string is >1024 */
	len = create_modalias(acpi_dev, buf, 1024);
	if (len <= 0)
		return 0;
	buf[len++] = '\n';
	return len;
}
static DEVICE_ATTR(modalias, 0444, acpi_device_modalias_show, NULL);

static void acpi_bus_hot_remove_device(void *context)
{
	struct acpi_device *device;
	acpi_handle handle = context;
	struct acpi_object_list arg_list;
	union acpi_object arg;
	acpi_status status = AE_OK;

	if (acpi_bus_get_device(handle, &device))
		return;

	if (!device)
		return;

	ACPI_DEBUG_PRINT((ACPI_DB_INFO,
		"Hot-removing device %s...\n", dev_name(&device->dev)));

	if (acpi_bus_trim(device, 1)) {
		printk(KERN_ERR PREFIX
				"Removing device failed\n");
		return;
	}

	/* power off device */
	status = acpi_evaluate_object(handle, "_PS3", NULL, NULL);
	if (ACPI_FAILURE(status) && status != AE_NOT_FOUND)
		printk(KERN_WARNING PREFIX
				"Power-off device failed\n");

	if (device->flags.lockable) {
		arg_list.count = 1;
		arg_list.pointer = &arg;
		arg.type = ACPI_TYPE_INTEGER;
		arg.integer.value = 0;
		acpi_evaluate_object(handle, "_LCK", &arg_list, NULL);
	}

	arg_list.count = 1;
	arg_list.pointer = &arg;
	arg.type = ACPI_TYPE_INTEGER;
	arg.integer.value = 1;

	/*
	 * TBD: _EJD support.
	 */
	status = acpi_evaluate_object(handle, "_EJ0", &arg_list, NULL);
	if (ACPI_FAILURE(status))
		printk(KERN_WARNING PREFIX
				"Eject device failed\n");

	return;
}

static ssize_t
acpi_eject_store(struct device *d, struct device_attribute *attr,
		const char *buf, size_t count)
{
	int ret = count;
	acpi_status status;
	acpi_object_type type = 0;
	struct acpi_device *acpi_device = to_acpi_device(d);

	if ((!count) || (buf[0] != '1')) {
		return -EINVAL;
	}
#ifndef FORCE_EJECT
	if (acpi_device->driver == NULL) {
		ret = -ENODEV;
		goto err;
	}
#endif
	status = acpi_get_type(acpi_device->handle, &type);
	if (ACPI_FAILURE(status) || (!acpi_device->flags.ejectable)) {
		ret = -ENODEV;
		goto err;
	}

	acpi_os_hotplug_execute(acpi_bus_hot_remove_device, acpi_device->handle);
err:
	return ret;
}

static DEVICE_ATTR(eject, 0200, NULL, acpi_eject_store);

static ssize_t
acpi_device_hid_show(struct device *dev, struct device_attribute *attr, char *buf) {
	struct acpi_device *acpi_dev = to_acpi_device(dev);

	return sprintf(buf, "%s\n", acpi_device_hid(acpi_dev));
}
static DEVICE_ATTR(hid, 0444, acpi_device_hid_show, NULL);

static ssize_t
acpi_device_path_show(struct device *dev, struct device_attribute *attr, char *buf) {
	struct acpi_device *acpi_dev = to_acpi_device(dev);
	struct acpi_buffer path = {ACPI_ALLOCATE_BUFFER, NULL};
	int result;

	result = acpi_get_name(acpi_dev->handle, ACPI_FULL_PATHNAME, &path);
	if (result)
		goto end;

	result = sprintf(buf, "%s\n", (char*)path.pointer);
	kfree(path.pointer);
end:
	return result;
}
static DEVICE_ATTR(path, 0444, acpi_device_path_show, NULL);

static int acpi_device_setup_files(struct acpi_device *dev)
{
	acpi_status status;
	acpi_handle temp;
	int result = 0;

	/*
	 * Devices gotten from FADT don't have a "path" attribute
	 */
	if (dev->handle) {
		result = device_create_file(&dev->dev, &dev_attr_path);
		if (result)
			goto end;
	}

	if (!list_empty(&dev->pnp.ids)) {
		result = device_create_file(&dev->dev, &dev_attr_hid);
		if (result)
			goto end;

		result = device_create_file(&dev->dev, &dev_attr_modalias);
		if (result)
			goto end;
	}

        /*
         * If device has _EJ0, 'eject' file is created that is used to trigger
         * hot-removal function from userland.
         */
	status = acpi_get_handle(dev->handle, "_EJ0", &temp);
	if (ACPI_SUCCESS(status))
		result = device_create_file(&dev->dev, &dev_attr_eject);
end:
	return result;
}

static void acpi_device_remove_files(struct acpi_device *dev)
{
	acpi_status status;
	acpi_handle temp;

	/*
	 * If device has _EJ0, 'eject' file is created that is used to trigger
	 * hot-removal function from userland.
	 */
	status = acpi_get_handle(dev->handle, "_EJ0", &temp);
	if (ACPI_SUCCESS(status))
		device_remove_file(&dev->dev, &dev_attr_eject);

	device_remove_file(&dev->dev, &dev_attr_modalias);
	device_remove_file(&dev->dev, &dev_attr_hid);
	if (dev->handle)
		device_remove_file(&dev->dev, &dev_attr_path);
}
/* --------------------------------------------------------------------------
			ACPI Bus operations
   -------------------------------------------------------------------------- */

int acpi_match_device_ids(struct acpi_device *device,
			  const struct acpi_device_id *ids)
{
	const struct acpi_device_id *id;
	struct acpi_hardware_id *hwid;

	/*
	 * If the device is not present, it is unnecessary to load device
	 * driver for it.
	 */
	if (!device->status.present)
		return -ENODEV;

	for (id = ids; id->id[0]; id++)
		list_for_each_entry(hwid, &device->pnp.ids, list)
			if (!strcmp((char *) id->id, hwid->id))
				return 0;

	return -ENOENT;
}
EXPORT_SYMBOL(acpi_match_device_ids);

static void acpi_free_ids(struct acpi_device *device)
{
	struct acpi_hardware_id *id, *tmp;

	list_for_each_entry_safe(id, tmp, &device->pnp.ids, list) {
		kfree(id->id);
		kfree(id);
=======
#define INVALID_ACPI_HANDLE	((acpi_handle)ZERO_PAGE(0))

static const char *dummy_hid = "device";

static LIST_HEAD(acpi_dep_list);
static DEFINE_MUTEX(acpi_dep_list_lock);
LIST_HEAD(acpi_bus_id_list);
static DEFINE_MUTEX(acpi_scan_lock);
static LIST_HEAD(acpi_scan_handlers_list);
DEFINE_MUTEX(acpi_device_lock);
LIST_HEAD(acpi_wakeup_device_list);
static DEFINE_MUTEX(acpi_hp_context_lock);

/*
 * The UART device described by the SPCR table is the only object which needs
 * special-casing. Everything else is covered by ACPI namespace paths in STAO
 * table.
 */
static u64 spcr_uart_addr;

void acpi_scan_lock_acquire(void)
{
	mutex_lock(&acpi_scan_lock);
}
EXPORT_SYMBOL_GPL(acpi_scan_lock_acquire);

void acpi_scan_lock_release(void)
{
	mutex_unlock(&acpi_scan_lock);
}
EXPORT_SYMBOL_GPL(acpi_scan_lock_release);

void acpi_lock_hp_context(void)
{
	mutex_lock(&acpi_hp_context_lock);
}

void acpi_unlock_hp_context(void)
{
	mutex_unlock(&acpi_hp_context_lock);
}

void acpi_initialize_hp_context(struct acpi_device *adev,
				struct acpi_hotplug_context *hp,
				int (*notify)(struct acpi_device *, u32),
				void (*uevent)(struct acpi_device *, u32))
{
	acpi_lock_hp_context();
	hp->notify = notify;
	hp->uevent = uevent;
	acpi_set_hp_context(adev, hp);
	acpi_unlock_hp_context();
}
EXPORT_SYMBOL_GPL(acpi_initialize_hp_context);

int acpi_scan_add_handler(struct acpi_scan_handler *handler)
{
	if (!handler)
		return -EINVAL;

	list_add_tail(&handler->list_node, &acpi_scan_handlers_list);
	return 0;
}

int acpi_scan_add_handler_with_hotplug(struct acpi_scan_handler *handler,
				       const char *hotplug_profile_name)
{
	int error;

	error = acpi_scan_add_handler(handler);
	if (error)
		return error;

	acpi_sysfs_add_hotplug_profile(&handler->hotplug, hotplug_profile_name);
	return 0;
}

bool acpi_scan_is_offline(struct acpi_device *adev, bool uevent)
{
	struct acpi_device_physical_node *pn;
	bool offline = true;
	char *envp[] = { "EVENT=offline", NULL };

	/*
	 * acpi_container_offline() calls this for all of the container's
	 * children under the container's physical_node_lock lock.
	 */
	mutex_lock_nested(&adev->physical_node_lock, SINGLE_DEPTH_NESTING);

	list_for_each_entry(pn, &adev->physical_node_list, node)
		if (device_supports_offline(pn->dev) && !pn->dev->offline) {
			if (uevent)
				kobject_uevent_env(&pn->dev->kobj, KOBJ_CHANGE, envp);

			offline = false;
			break;
		}

	mutex_unlock(&adev->physical_node_lock);
	return offline;
}

static acpi_status acpi_bus_offline(acpi_handle handle, u32 lvl, void *data,
				    void **ret_p)
{
	struct acpi_device *device = acpi_fetch_acpi_dev(handle);
	struct acpi_device_physical_node *pn;
	bool second_pass = (bool)data;
	acpi_status status = AE_OK;

	if (!device)
		return AE_OK;

	if (device->handler && !device->handler->hotplug.enabled) {
		*ret_p = &device->dev;
		return AE_SUPPORT;
	}

	mutex_lock(&device->physical_node_lock);

	list_for_each_entry(pn, &device->physical_node_list, node) {
		int ret;

		if (second_pass) {
			/* Skip devices offlined by the first pass. */
			if (pn->put_online)
				continue;
		} else {
			pn->put_online = false;
		}
		ret = device_offline(pn->dev);
		if (ret >= 0) {
			pn->put_online = !ret;
		} else {
			*ret_p = pn->dev;
			if (second_pass) {
				status = AE_ERROR;
				break;
			}
		}
	}

	mutex_unlock(&device->physical_node_lock);

	return status;
}

static acpi_status acpi_bus_online(acpi_handle handle, u32 lvl, void *data,
				   void **ret_p)
{
	struct acpi_device *device = acpi_fetch_acpi_dev(handle);
	struct acpi_device_physical_node *pn;

	if (!device)
		return AE_OK;

	mutex_lock(&device->physical_node_lock);

	list_for_each_entry(pn, &device->physical_node_list, node)
		if (pn->put_online) {
			device_online(pn->dev);
			pn->put_online = false;
		}

	mutex_unlock(&device->physical_node_lock);

	return AE_OK;
}

static int acpi_scan_try_to_offline(struct acpi_device *device)
{
	acpi_handle handle = device->handle;
	struct device *errdev = NULL;
	acpi_status status;

	/*
	 * Carry out two passes here and ignore errors in the first pass,
	 * because if the devices in question are memory blocks and
	 * CONFIG_MEMCG is set, one of the blocks may hold data structures
	 * that the other blocks depend on, but it is not known in advance which
	 * block holds them.
	 *
	 * If the first pass is successful, the second one isn't needed, though.
	 */
	status = acpi_walk_namespace(ACPI_TYPE_ANY, handle, ACPI_UINT32_MAX,
				     NULL, acpi_bus_offline, (void *)false,
				     (void **)&errdev);
	if (status == AE_SUPPORT) {
		dev_warn(errdev, "Offline disabled.\n");
		acpi_walk_namespace(ACPI_TYPE_ANY, handle, ACPI_UINT32_MAX,
				    acpi_bus_online, NULL, NULL, NULL);
		return -EPERM;
	}
	acpi_bus_offline(handle, 0, (void *)false, (void **)&errdev);
	if (errdev) {
		errdev = NULL;
		acpi_walk_namespace(ACPI_TYPE_ANY, handle, ACPI_UINT32_MAX,
				    NULL, acpi_bus_offline, (void *)true,
				    (void **)&errdev);
		if (!errdev)
			acpi_bus_offline(handle, 0, (void *)true,
					 (void **)&errdev);

		if (errdev) {
			dev_warn(errdev, "Offline failed.\n");
			acpi_bus_online(handle, 0, NULL, NULL);
			acpi_walk_namespace(ACPI_TYPE_ANY, handle,
					    ACPI_UINT32_MAX, acpi_bus_online,
					    NULL, NULL, NULL);
			return -EBUSY;
		}
	}
	return 0;
}

static int acpi_scan_check_and_detach(struct acpi_device *adev, void *check)
{
	struct acpi_scan_handler *handler = adev->handler;

	acpi_dev_for_each_child_reverse(adev, acpi_scan_check_and_detach, check);

	if (check) {
		acpi_bus_get_status(adev);
		/*
		 * Skip devices that are still there and take the enabled
		 * flag into account.
		 */
		if (acpi_device_is_enabled(adev))
			return 0;

		/* Skip device that have not been enumerated. */
		if (!acpi_device_enumerated(adev)) {
			dev_dbg(&adev->dev, "Still not enumerated\n");
			return 0;
		}
	}

	adev->flags.match_driver = false;
	if (handler) {
		if (handler->detach)
			handler->detach(adev);

		adev->handler = NULL;
	} else {
		device_release_driver(&adev->dev);
	}
	/*
	 * Most likely, the device is going away, so put it into D3cold before
	 * that.
	 */
	acpi_device_set_power(adev, ACPI_STATE_D3_COLD);
	adev->flags.initialized = false;
	acpi_device_clear_enumerated(adev);

	return 0;
}

static void acpi_scan_check_subtree(struct acpi_device *adev)
{
	acpi_scan_check_and_detach(adev, (void *)true);
}

static int acpi_scan_hot_remove(struct acpi_device *device)
{
	acpi_handle handle = device->handle;
	unsigned long long sta;
	acpi_status status;

	if (device->handler && device->handler->hotplug.demand_offline) {
		if (!acpi_scan_is_offline(device, true))
			return -EBUSY;
	} else {
		int error = acpi_scan_try_to_offline(device);
		if (error)
			return error;
	}

	acpi_handle_debug(handle, "Ejecting\n");

	acpi_bus_trim(device);

	acpi_evaluate_lck(handle, 0);
	/*
	 * TBD: _EJD support.
	 */
	status = acpi_evaluate_ej0(handle);
	if (status == AE_NOT_FOUND)
		return -ENODEV;
	else if (ACPI_FAILURE(status))
		return -EIO;

	/*
	 * Verify if eject was indeed successful.  If not, log an error
	 * message.  No need to call _OST since _EJ0 call was made OK.
	 */
	status = acpi_evaluate_integer(handle, "_STA", NULL, &sta);
	if (ACPI_FAILURE(status)) {
		acpi_handle_warn(handle,
			"Status check after eject failed (0x%x)\n", status);
	} else if (sta & ACPI_STA_DEVICE_ENABLED) {
		acpi_handle_warn(handle,
			"Eject incomplete - status 0x%llx\n", sta);
	}

	return 0;
}

static int acpi_scan_rescan_bus(struct acpi_device *adev)
{
	struct acpi_scan_handler *handler = adev->handler;
	int ret;

	if (handler && handler->hotplug.scan_dependent)
		ret = handler->hotplug.scan_dependent(adev);
	else
		ret = acpi_bus_scan(adev->handle);

	if (ret)
		dev_info(&adev->dev, "Namespace scan failure\n");

	return ret;
}

static int acpi_scan_device_check(struct acpi_device *adev)
{
	struct acpi_device *parent;

	acpi_scan_check_subtree(adev);

	if (!acpi_device_is_present(adev))
		return 0;

	/*
	 * This function is only called for device objects for which matching
	 * scan handlers exist.  The only situation in which the scan handler
	 * is not attached to this device object yet is when the device has
	 * just appeared (either it wasn't present at all before or it was
	 * removed and then added again).
	 */
	if (adev->handler) {
		dev_dbg(&adev->dev, "Already enumerated\n");
		return 0;
	}

	parent = acpi_dev_parent(adev);
	if (!parent)
		parent = adev;

	return acpi_scan_rescan_bus(parent);
}

static int acpi_scan_bus_check(struct acpi_device *adev)
{
	acpi_scan_check_subtree(adev);

	return acpi_scan_rescan_bus(adev);
}

static int acpi_generic_hotplug_event(struct acpi_device *adev, u32 type)
{
	switch (type) {
	case ACPI_NOTIFY_BUS_CHECK:
		return acpi_scan_bus_check(adev);
	case ACPI_NOTIFY_DEVICE_CHECK:
		return acpi_scan_device_check(adev);
	case ACPI_NOTIFY_EJECT_REQUEST:
	case ACPI_OST_EC_OSPM_EJECT:
		if (adev->handler && !adev->handler->hotplug.enabled) {
			dev_info(&adev->dev, "Eject disabled\n");
			return -EPERM;
		}
		acpi_evaluate_ost(adev->handle, ACPI_NOTIFY_EJECT_REQUEST,
				  ACPI_OST_SC_EJECT_IN_PROGRESS, NULL);
		return acpi_scan_hot_remove(adev);
	}
	return -EINVAL;
}

void acpi_device_hotplug(struct acpi_device *adev, u32 src)
{
	u32 ost_code = ACPI_OST_SC_NON_SPECIFIC_FAILURE;
	int error = -ENODEV;

	lock_device_hotplug();
	mutex_lock(&acpi_scan_lock);

	/*
	 * The device object's ACPI handle cannot become invalid as long as we
	 * are holding acpi_scan_lock, but it might have become invalid before
	 * that lock was acquired.
	 */
	if (adev->handle == INVALID_ACPI_HANDLE)
		goto err_out;

	if (adev->flags.is_dock_station) {
		error = dock_notify(adev, src);
	} else if (adev->flags.hotplug_notify) {
		error = acpi_generic_hotplug_event(adev, src);
	} else {
		int (*notify)(struct acpi_device *, u32);

		acpi_lock_hp_context();
		notify = adev->hp ? adev->hp->notify : NULL;
		acpi_unlock_hp_context();
		/*
		 * There may be additional notify handlers for device objects
		 * without the .event() callback, so ignore them here.
		 */
		if (notify)
			error = notify(adev, src);
		else
			goto out;
	}
	switch (error) {
	case 0:
		ost_code = ACPI_OST_SC_SUCCESS;
		break;
	case -EPERM:
		ost_code = ACPI_OST_SC_EJECT_NOT_SUPPORTED;
		break;
	case -EBUSY:
		ost_code = ACPI_OST_SC_DEVICE_BUSY;
		break;
	default:
		ost_code = ACPI_OST_SC_NON_SPECIFIC_FAILURE;
		break;
	}

 err_out:
	acpi_evaluate_ost(adev->handle, src, ost_code, NULL);

 out:
	acpi_put_acpi_dev(adev);
	mutex_unlock(&acpi_scan_lock);
	unlock_device_hotplug();
}

static void acpi_free_power_resources_lists(struct acpi_device *device)
{
	int i;

	if (device->wakeup.flags.valid)
		acpi_power_resources_list_free(&device->wakeup.resources);

	if (!device->power.flags.power_resources)
		return;

	for (i = ACPI_STATE_D0; i <= ACPI_STATE_D3_HOT; i++) {
		struct acpi_device_power_state *ps = &device->power.states[i];
		acpi_power_resources_list_free(&ps->resources);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	}
}

static void acpi_device_release(struct device *dev)
{
	struct acpi_device *acpi_dev = to_acpi_device(dev);

<<<<<<< HEAD
	acpi_free_ids(acpi_dev);
	kfree(acpi_dev);
}

static int acpi_device_suspend(struct device *dev, pm_message_t state)
{
	struct acpi_device *acpi_dev = to_acpi_device(dev);
	struct acpi_driver *acpi_drv = acpi_dev->driver;

	if (acpi_drv && acpi_drv->ops.suspend)
		return acpi_drv->ops.suspend(acpi_dev, state);
	return 0;
}

static int acpi_device_resume(struct device *dev)
{
	struct acpi_device *acpi_dev = to_acpi_device(dev);
	struct acpi_driver *acpi_drv = acpi_dev->driver;

	if (acpi_drv && acpi_drv->ops.resume)
		return acpi_drv->ops.resume(acpi_dev);
	return 0;
}

static int acpi_bus_match(struct device *dev, struct device_driver *drv)
{
	struct acpi_device *acpi_dev = to_acpi_device(dev);
	struct acpi_driver *acpi_drv = to_acpi_driver(drv);

	return !acpi_match_device_ids(acpi_dev, acpi_drv->ids);
}

static int acpi_device_uevent(struct device *dev, struct kobj_uevent_env *env)
{
	struct acpi_device *acpi_dev = to_acpi_device(dev);
	int len;

	if (list_empty(&acpi_dev->pnp.ids))
		return 0;

	if (add_uevent_var(env, "MODALIAS="))
		return -ENOMEM;
	len = create_modalias(acpi_dev, &env->buf[env->buflen - 1],
			      sizeof(env->buf) - env->buflen);
	if (len >= (sizeof(env->buf) - env->buflen))
		return -ENOMEM;
	env->buflen += len;
	return 0;
}

static void acpi_device_notify(acpi_handle handle, u32 event, void *data)
{
	struct acpi_device *device = data;

	device->driver->ops.notify(device, event);
}

static acpi_status acpi_device_notify_fixed(void *data)
{
	struct acpi_device *device = data;

	/* Fixed hardware devices have no handles */
	acpi_device_notify(NULL, ACPI_FIXED_HARDWARE_EVENT, device);
	return AE_OK;
}

static int acpi_device_install_notify_handler(struct acpi_device *device)
{
	acpi_status status;

	if (device->device_type == ACPI_BUS_TYPE_POWER_BUTTON)
		status =
		    acpi_install_fixed_event_handler(ACPI_EVENT_POWER_BUTTON,
						     acpi_device_notify_fixed,
						     device);
	else if (device->device_type == ACPI_BUS_TYPE_SLEEP_BUTTON)
		status =
		    acpi_install_fixed_event_handler(ACPI_EVENT_SLEEP_BUTTON,
						     acpi_device_notify_fixed,
						     device);
	else
		status = acpi_install_notify_handler(device->handle,
						     ACPI_DEVICE_NOTIFY,
						     acpi_device_notify,
						     device);

	if (ACPI_FAILURE(status))
		return -EINVAL;
	return 0;
}

static void acpi_device_remove_notify_handler(struct acpi_device *device)
{
	if (device->device_type == ACPI_BUS_TYPE_POWER_BUTTON)
		acpi_remove_fixed_event_handler(ACPI_EVENT_POWER_BUTTON,
						acpi_device_notify_fixed);
	else if (device->device_type == ACPI_BUS_TYPE_SLEEP_BUTTON)
		acpi_remove_fixed_event_handler(ACPI_EVENT_SLEEP_BUTTON,
						acpi_device_notify_fixed);
	else
		acpi_remove_notify_handler(device->handle, ACPI_DEVICE_NOTIFY,
					   acpi_device_notify);
}

static int acpi_bus_driver_init(struct acpi_device *, struct acpi_driver *);
static int acpi_start_single_object(struct acpi_device *);
static int acpi_device_probe(struct device * dev)
{
	struct acpi_device *acpi_dev = to_acpi_device(dev);
	struct acpi_driver *acpi_drv = to_acpi_driver(dev->driver);
	int ret;

	ret = acpi_bus_driver_init(acpi_dev, acpi_drv);
	if (!ret) {
		if (acpi_dev->bus_ops.acpi_op_start)
			acpi_start_single_object(acpi_dev);

		if (acpi_drv->ops.notify) {
			ret = acpi_device_install_notify_handler(acpi_dev);
			if (ret) {
				if (acpi_drv->ops.remove)
					acpi_drv->ops.remove(acpi_dev,
						     acpi_dev->removal_type);
				return ret;
			}
		}

		ACPI_DEBUG_PRINT((ACPI_DB_INFO,
			"Found driver [%s] for device [%s]\n",
			acpi_drv->name, acpi_dev->pnp.bus_id));
		get_device(dev);
	}
	return ret;
}

static int acpi_device_remove(struct device * dev)
{
	struct acpi_device *acpi_dev = to_acpi_device(dev);
	struct acpi_driver *acpi_drv = acpi_dev->driver;

	if (acpi_drv) {
		if (acpi_drv->ops.notify)
			acpi_device_remove_notify_handler(acpi_dev);
		if (acpi_drv->ops.remove)
			acpi_drv->ops.remove(acpi_dev, acpi_dev->removal_type);
	}
	acpi_dev->driver = NULL;
	acpi_dev->driver_data = NULL;

	put_device(dev);
	return 0;
}

struct bus_type acpi_bus_type = {
	.name		= "acpi",
	.suspend	= acpi_device_suspend,
	.resume		= acpi_device_resume,
	.match		= acpi_bus_match,
	.probe		= acpi_device_probe,
	.remove		= acpi_device_remove,
	.uevent		= acpi_device_uevent,
};

static int acpi_device_register(struct acpi_device *device)
{
	int result;
	struct acpi_device_bus_id *acpi_device_bus_id, *new_bus_id;
	int found = 0;
=======
	acpi_free_properties(acpi_dev);
	acpi_free_pnp_ids(&acpi_dev->pnp);
	acpi_free_power_resources_lists(acpi_dev);
	kfree(acpi_dev);
}

static void acpi_device_del(struct acpi_device *device)
{
	struct acpi_device_bus_id *acpi_device_bus_id;

	mutex_lock(&acpi_device_lock);

	list_for_each_entry(acpi_device_bus_id, &acpi_bus_id_list, node)
		if (!strcmp(acpi_device_bus_id->bus_id,
			    acpi_device_hid(device))) {
			ida_free(&acpi_device_bus_id->instance_ida,
				 device->pnp.instance_no);
			if (ida_is_empty(&acpi_device_bus_id->instance_ida)) {
				list_del(&acpi_device_bus_id->node);
				kfree_const(acpi_device_bus_id->bus_id);
				kfree(acpi_device_bus_id);
			}
			break;
		}

	list_del(&device->wakeup_list);

	mutex_unlock(&acpi_device_lock);

	acpi_power_add_remove_device(device, false);
	acpi_device_remove_files(device);
	if (device->remove)
		device->remove(device);

	device_del(&device->dev);
}

static BLOCKING_NOTIFIER_HEAD(acpi_reconfig_chain);

static LIST_HEAD(acpi_device_del_list);
static DEFINE_MUTEX(acpi_device_del_lock);

static void acpi_device_del_work_fn(struct work_struct *work_not_used)
{
	for (;;) {
		struct acpi_device *adev;

		mutex_lock(&acpi_device_del_lock);

		if (list_empty(&acpi_device_del_list)) {
			mutex_unlock(&acpi_device_del_lock);
			break;
		}
		adev = list_first_entry(&acpi_device_del_list,
					struct acpi_device, del_list);
		list_del(&adev->del_list);

		mutex_unlock(&acpi_device_del_lock);

		blocking_notifier_call_chain(&acpi_reconfig_chain,
					     ACPI_RECONFIG_DEVICE_REMOVE, adev);

		acpi_device_del(adev);
		/*
		 * Drop references to all power resources that might have been
		 * used by the device.
		 */
		acpi_power_transition(adev, ACPI_STATE_D3_COLD);
		acpi_dev_put(adev);
	}
}

/**
 * acpi_scan_drop_device - Drop an ACPI device object.
 * @handle: Handle of an ACPI namespace node, not used.
 * @context: Address of the ACPI device object to drop.
 *
 * This is invoked by acpi_ns_delete_node() during the removal of the ACPI
 * namespace node the device object pointed to by @context is attached to.
 *
 * The unregistration is carried out asynchronously to avoid running
 * acpi_device_del() under the ACPICA's namespace mutex and the list is used to
 * ensure the correct ordering (the device objects must be unregistered in the
 * same order in which the corresponding namespace nodes are deleted).
 */
static void acpi_scan_drop_device(acpi_handle handle, void *context)
{
	static DECLARE_WORK(work, acpi_device_del_work_fn);
	struct acpi_device *adev = context;

	mutex_lock(&acpi_device_del_lock);

	/*
	 * Use the ACPI hotplug workqueue which is ordered, so this work item
	 * won't run after any hotplug work items submitted subsequently.  That
	 * prevents attempts to register device objects identical to those being
	 * deleted from happening concurrently (such attempts result from
	 * hotplug events handled via the ACPI hotplug workqueue).  It also will
	 * run after all of the work items submitted previously, which helps
	 * those work items to ensure that they are not accessing stale device
	 * objects.
	 */
	if (list_empty(&acpi_device_del_list))
		acpi_queue_hotplug_work(&work);

	list_add_tail(&adev->del_list, &acpi_device_del_list);
	/* Make acpi_ns_validate_handle() return NULL for this handle. */
	adev->handle = INVALID_ACPI_HANDLE;

	mutex_unlock(&acpi_device_del_lock);
}

static struct acpi_device *handle_to_device(acpi_handle handle,
					    void (*callback)(void *))
{
	struct acpi_device *adev = NULL;
	acpi_status status;

	status = acpi_get_data_full(handle, acpi_scan_drop_device,
				    (void **)&adev, callback);
	if (ACPI_FAILURE(status) || !adev) {
		acpi_handle_debug(handle, "No context!\n");
		return NULL;
	}
	return adev;
}

/**
 * acpi_fetch_acpi_dev - Retrieve ACPI device object.
 * @handle: ACPI handle associated with the requested ACPI device object.
 *
 * Return a pointer to the ACPI device object associated with @handle, if
 * present, or NULL otherwise.
 */
struct acpi_device *acpi_fetch_acpi_dev(acpi_handle handle)
{
	return handle_to_device(handle, NULL);
}
EXPORT_SYMBOL_GPL(acpi_fetch_acpi_dev);

static void get_acpi_device(void *dev)
{
	acpi_dev_get(dev);
}

/**
 * acpi_get_acpi_dev - Retrieve ACPI device object and reference count it.
 * @handle: ACPI handle associated with the requested ACPI device object.
 *
 * Return a pointer to the ACPI device object associated with @handle and bump
 * up that object's reference counter (under the ACPI Namespace lock), if
 * present, or return NULL otherwise.
 *
 * The ACPI device object reference acquired by this function needs to be
 * dropped via acpi_dev_put().
 */
struct acpi_device *acpi_get_acpi_dev(acpi_handle handle)
{
	return handle_to_device(handle, get_acpi_device);
}
EXPORT_SYMBOL_GPL(acpi_get_acpi_dev);

static struct acpi_device_bus_id *acpi_device_bus_id_match(const char *dev_id)
{
	struct acpi_device_bus_id *acpi_device_bus_id;

	/* Find suitable bus_id and instance number in acpi_bus_id_list. */
	list_for_each_entry(acpi_device_bus_id, &acpi_bus_id_list, node) {
		if (!strcmp(acpi_device_bus_id->bus_id, dev_id))
			return acpi_device_bus_id;
	}
	return NULL;
}

static int acpi_device_set_name(struct acpi_device *device,
				struct acpi_device_bus_id *acpi_device_bus_id)
{
	struct ida *instance_ida = &acpi_device_bus_id->instance_ida;
	int result;

	result = ida_alloc(instance_ida, GFP_KERNEL);
	if (result < 0)
		return result;

	device->pnp.instance_no = result;
	dev_set_name(&device->dev, "%s:%02x", acpi_device_bus_id->bus_id, result);
	return 0;
}

int acpi_tie_acpi_dev(struct acpi_device *adev)
{
	acpi_handle handle = adev->handle;
	acpi_status status;

	if (!handle)
		return 0;

	status = acpi_attach_data(handle, acpi_scan_drop_device, adev);
	if (ACPI_FAILURE(status)) {
		acpi_handle_err(handle, "Unable to attach device data\n");
		return -ENODEV;
	}

	return 0;
}

static void acpi_store_pld_crc(struct acpi_device *adev)
{
	struct acpi_pld_info *pld;
	acpi_status status;

	status = acpi_get_physical_device_location(adev->handle, &pld);
	if (ACPI_FAILURE(status))
		return;

	adev->pld_crc = crc32(~0, pld, sizeof(*pld));
	ACPI_FREE(pld);
}

int acpi_device_add(struct acpi_device *device)
{
	struct acpi_device_bus_id *acpi_device_bus_id;
	int result;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	/*
	 * Linkage
	 * -------
	 * Link this device to its parent and siblings.
	 */
<<<<<<< HEAD
	INIT_LIST_HEAD(&device->children);
	INIT_LIST_HEAD(&device->node);
	INIT_LIST_HEAD(&device->wakeup_list);

	new_bus_id = kzalloc(sizeof(struct acpi_device_bus_id), GFP_KERNEL);
	if (!new_bus_id) {
		printk(KERN_ERR PREFIX "Memory allocation error\n");
		return -ENOMEM;
	}

	mutex_lock(&acpi_device_lock);
	/*
	 * Find suitable bus_id and instance number in acpi_bus_id_list
	 * If failed, create one and link it into acpi_bus_id_list
	 */
	list_for_each_entry(acpi_device_bus_id, &acpi_bus_id_list, node) {
		if (!strcmp(acpi_device_bus_id->bus_id,
			    acpi_device_hid(device))) {
			acpi_device_bus_id->instance_no++;
			found = 1;
			kfree(new_bus_id);
			break;
		}
	}
	if (!found) {
		acpi_device_bus_id = new_bus_id;
		strcpy(acpi_device_bus_id->bus_id, acpi_device_hid(device));
		acpi_device_bus_id->instance_no = 0;
		list_add_tail(&acpi_device_bus_id->node, &acpi_bus_id_list);
	}
	dev_set_name(&device->dev, "%s:%02x", acpi_device_bus_id->bus_id, acpi_device_bus_id->instance_no);

	if (device->parent)
		list_add_tail(&device->node, &device->parent->children);

	if (device->wakeup.flags.valid)
		list_add_tail(&device->wakeup_list, &acpi_wakeup_device_list);
	mutex_unlock(&acpi_device_lock);

	if (device->parent)
		device->dev.parent = &device->parent->dev;
	device->dev.bus = &acpi_bus_type;
	device->dev.release = &acpi_device_release;
	result = device_register(&device->dev);
	if (result) {
		dev_err(&device->dev, "Error registering device\n");
		goto end;
=======
	INIT_LIST_HEAD(&device->wakeup_list);
	INIT_LIST_HEAD(&device->physical_node_list);
	INIT_LIST_HEAD(&device->del_list);
	mutex_init(&device->physical_node_lock);

	mutex_lock(&acpi_device_lock);

	acpi_device_bus_id = acpi_device_bus_id_match(acpi_device_hid(device));
	if (acpi_device_bus_id) {
		result = acpi_device_set_name(device, acpi_device_bus_id);
		if (result)
			goto err_unlock;
	} else {
		acpi_device_bus_id = kzalloc(sizeof(*acpi_device_bus_id),
					     GFP_KERNEL);
		if (!acpi_device_bus_id) {
			result = -ENOMEM;
			goto err_unlock;
		}
		acpi_device_bus_id->bus_id =
			kstrdup_const(acpi_device_hid(device), GFP_KERNEL);
		if (!acpi_device_bus_id->bus_id) {
			kfree(acpi_device_bus_id);
			result = -ENOMEM;
			goto err_unlock;
		}

		ida_init(&acpi_device_bus_id->instance_ida);

		result = acpi_device_set_name(device, acpi_device_bus_id);
		if (result) {
			kfree_const(acpi_device_bus_id->bus_id);
			kfree(acpi_device_bus_id);
			goto err_unlock;
		}

		list_add_tail(&acpi_device_bus_id->node, &acpi_bus_id_list);
	}

	if (device->wakeup.flags.valid)
		list_add_tail(&device->wakeup_list, &acpi_wakeup_device_list);

	acpi_store_pld_crc(device);

	mutex_unlock(&acpi_device_lock);

	result = device_add(&device->dev);
	if (result) {
		dev_err(&device->dev, "Error registering device\n");
		goto err;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	}

	result = acpi_device_setup_files(device);
	if (result)
<<<<<<< HEAD
		printk(KERN_ERR PREFIX "Error creating sysfs interface for device %s\n",
		       dev_name(&device->dev));

	device->removal_type = ACPI_BUS_REMOVAL_NORMAL;
	return 0;
end:
	mutex_lock(&acpi_device_lock);
	if (device->parent)
		list_del(&device->node);
	list_del(&device->wakeup_list);
	mutex_unlock(&acpi_device_lock);
	return result;
}

static void acpi_device_unregister(struct acpi_device *device, int type)
{
	mutex_lock(&acpi_device_lock);
	if (device->parent)
		list_del(&device->node);

	list_del(&device->wakeup_list);
	mutex_unlock(&acpi_device_lock);

	acpi_detach_data(device->handle, acpi_bus_data_handler);

	acpi_device_remove_files(device);
	device_unregister(&device->dev);
}

/* --------------------------------------------------------------------------
                                 Driver Management
   -------------------------------------------------------------------------- */
/**
 * acpi_bus_driver_init - add a device to a driver
 * @device: the device to add and initialize
 * @driver: driver for the device
 *
 * Used to initialize a device via its device driver.  Called whenever a
 * driver is bound to a device.  Invokes the driver's add() ops.
 */
static int
acpi_bus_driver_init(struct acpi_device *device, struct acpi_driver *driver)
{
	int result = 0;

	if (!device || !driver)
		return -EINVAL;

	if (!driver->ops.add)
		return -ENOSYS;

	result = driver->ops.add(device);
	if (result) {
		device->driver = NULL;
		device->driver_data = NULL;
		return result;
	}

	device->driver = driver;

	/*
	 * TBD - Configuration Management: Assign resources to device based
	 * upon possible configuration and currently allocated resources.
	 */

	ACPI_DEBUG_PRINT((ACPI_DB_INFO,
			  "Driver successfully bound to device\n"));
	return 0;
}

static int acpi_start_single_object(struct acpi_device *device)
{
	int result = 0;
	struct acpi_driver *driver;


	if (!(driver = device->driver))
		return 0;

	if (driver->ops.start) {
		result = driver->ops.start(device);
		if (result && driver->ops.remove)
			driver->ops.remove(device, ACPI_BUS_REMOVAL_NORMAL);
	}
=======
		pr_err("Error creating sysfs interface for device %s\n",
		       dev_name(&device->dev));

	return 0;

err:
	mutex_lock(&acpi_device_lock);

	list_del(&device->wakeup_list);

err_unlock:
	mutex_unlock(&acpi_device_lock);

	acpi_detach_data(device->handle, acpi_scan_drop_device);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	return result;
}

<<<<<<< HEAD
/**
 * acpi_bus_register_driver - register a driver with the ACPI bus
 * @driver: driver being registered
 *
 * Registers a driver with the ACPI bus.  Searches the namespace for all
 * devices that match the driver's criteria and binds.  Returns zero for
 * success or a negative error status for failure.
 */
int acpi_bus_register_driver(struct acpi_driver *driver)
{
	int ret;

	if (acpi_disabled)
		return -ENODEV;
	driver->drv.name = driver->name;
	driver->drv.bus = &acpi_bus_type;
	driver->drv.owner = driver->owner;

	ret = driver_register(&driver->drv);
	return ret;
}

EXPORT_SYMBOL(acpi_bus_register_driver);

/**
 * acpi_bus_unregister_driver - unregisters a driver with the APIC bus
 * @driver: driver to unregister
 *
 * Unregisters a driver with the ACPI bus.  Searches the namespace for all
 * devices that match the driver's criteria and unbinds.
 */
void acpi_bus_unregister_driver(struct acpi_driver *driver)
{
	driver_unregister(&driver->drv);
}

EXPORT_SYMBOL(acpi_bus_unregister_driver);

/* --------------------------------------------------------------------------
                                 Device Enumeration
   -------------------------------------------------------------------------- */
static struct acpi_device *acpi_bus_get_parent(acpi_handle handle)
{
	acpi_status status;
	int ret;
	struct acpi_device *device;
=======
/* --------------------------------------------------------------------------
                                 Device Enumeration
   -------------------------------------------------------------------------- */
static bool acpi_info_matches_ids(struct acpi_device_info *info,
				  const char * const ids[])
{
	struct acpi_pnp_device_id_list *cid_list = NULL;
	int i, index;

	if (!(info->valid & ACPI_VALID_HID))
		return false;

	index = match_string(ids, -1, info->hardware_id.string);
	if (index >= 0)
		return true;

	if (info->valid & ACPI_VALID_CID)
		cid_list = &info->compatible_id_list;

	if (!cid_list)
		return false;

	for (i = 0; i < cid_list->count; i++) {
		index = match_string(ids, -1, cid_list->ids[i].string);
		if (index >= 0)
			return true;
	}

	return false;
}

/* List of HIDs for which we ignore matching ACPI devices, when checking _DEP lists. */
static const char * const acpi_ignore_dep_ids[] = {
	"PNP0D80", /* Windows-compatible System Power Management Controller */
	"INT33BD", /* Intel Baytrail Mailbox Device */
	"LATT2021", /* Lattice FW Update Client Driver */
	NULL
};

/* List of HIDs for which we honor deps of matching ACPI devs, when checking _DEP lists. */
static const char * const acpi_honor_dep_ids[] = {
	"INT3472", /* Camera sensor PMIC / clk and regulator info */
	"INTC1059", /* IVSC (TGL) driver must be loaded to allow i2c access to camera sensors */
	"INTC1095", /* IVSC (ADL) driver must be loaded to allow i2c access to camera sensors */
	"INTC100A", /* IVSC (RPL) driver must be loaded to allow i2c access to camera sensors */
	"INTC10CF", /* IVSC (MTL) driver must be loaded to allow i2c access to camera sensors */
	NULL
};

static struct acpi_device *acpi_find_parent_acpi_dev(acpi_handle handle)
{
	struct acpi_device *adev;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	/*
	 * Fixed hardware devices do not appear in the namespace and do not
	 * have handles, but we fabricate acpi_devices for them, so we have
	 * to deal with them specially.
	 */
<<<<<<< HEAD
	if (handle == NULL)
		return acpi_root;

	do {
		status = acpi_get_parent(handle, &handle);
		if (status == AE_NULL_ENTRY)
			return NULL;
		if (ACPI_FAILURE(status))
			return acpi_root;

		ret = acpi_bus_get_device(handle, &device);
		if (ret == 0)
			return device;
	} while (1);
=======
	if (!handle)
		return acpi_root;

	do {
		acpi_status status;

		status = acpi_get_parent(handle, &handle);
		if (ACPI_FAILURE(status)) {
			if (status != AE_NULL_ENTRY)
				return acpi_root;

			return NULL;
		}
		adev = acpi_fetch_acpi_dev(handle);
	} while (!adev);
	return adev;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

acpi_status
acpi_bus_get_ejd(acpi_handle handle, acpi_handle *ejd)
{
	acpi_status status;
	acpi_handle tmp;
	struct acpi_buffer buffer = {ACPI_ALLOCATE_BUFFER, NULL};
	union acpi_object *obj;

	status = acpi_get_handle(handle, "_EJD", &tmp);
	if (ACPI_FAILURE(status))
		return status;

	status = acpi_evaluate_object(handle, "_EJD", NULL, &buffer);
	if (ACPI_SUCCESS(status)) {
		obj = buffer.pointer;
		status = acpi_get_handle(ACPI_ROOT_OBJECT, obj->string.pointer,
					 ejd);
		kfree(buffer.pointer);
	}
	return status;
}
EXPORT_SYMBOL_GPL(acpi_bus_get_ejd);

<<<<<<< HEAD
void acpi_bus_data_handler(acpi_handle handle, void *context)
{

	/* TBD */

	return;
}

static int acpi_bus_get_perf_flags(struct acpi_device *device)
{
	device->performance.state = ACPI_STATE_UNKNOWN;
	return 0;
}

static acpi_status
acpi_bus_extract_wakeup_device_power_package(acpi_handle handle,
					     struct acpi_device_wakeup *wakeup)
{
=======
static int acpi_bus_extract_wakeup_device_power_package(struct acpi_device *dev)
{
	acpi_handle handle = dev->handle;
	struct acpi_device_wakeup *wakeup = &dev->wakeup;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	struct acpi_buffer buffer = { ACPI_ALLOCATE_BUFFER, NULL };
	union acpi_object *package = NULL;
	union acpi_object *element = NULL;
	acpi_status status;
<<<<<<< HEAD
	int i = 0;

	if (!wakeup)
		return AE_BAD_PARAMETER;
=======
	int err = -ENODATA;

	INIT_LIST_HEAD(&wakeup->resources);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	/* _PRW */
	status = acpi_evaluate_object(handle, "_PRW", NULL, &buffer);
	if (ACPI_FAILURE(status)) {
<<<<<<< HEAD
		ACPI_EXCEPTION((AE_INFO, status, "Evaluating _PRW"));
		return status;
=======
		acpi_handle_info(handle, "_PRW evaluation failed: %s\n",
				 acpi_format_exception(status));
		return err;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	}

	package = (union acpi_object *)buffer.pointer;

<<<<<<< HEAD
	if (!package || (package->package.count < 2)) {
		status = AE_BAD_DATA;
		goto out;
	}

	element = &(package->package.elements[0]);
	if (!element) {
		status = AE_BAD_DATA;
		goto out;
	}
=======
	if (!package || package->package.count < 2)
		goto out;

	element = &(package->package.elements[0]);
	if (!element)
		goto out;

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	if (element->type == ACPI_TYPE_PACKAGE) {
		if ((element->package.count < 2) ||
		    (element->package.elements[0].type !=
		     ACPI_TYPE_LOCAL_REFERENCE)
<<<<<<< HEAD
		    || (element->package.elements[1].type != ACPI_TYPE_INTEGER)) {
			status = AE_BAD_DATA;
			goto out;
		}
=======
		    || (element->package.elements[1].type != ACPI_TYPE_INTEGER))
			goto out;

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		wakeup->gpe_device =
		    element->package.elements[0].reference.handle;
		wakeup->gpe_number =
		    (u32) element->package.elements[1].integer.value;
	} else if (element->type == ACPI_TYPE_INTEGER) {
		wakeup->gpe_device = NULL;
		wakeup->gpe_number = element->integer.value;
	} else {
<<<<<<< HEAD
		status = AE_BAD_DATA;
=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		goto out;
	}

	element = &(package->package.elements[1]);
<<<<<<< HEAD
	if (element->type != ACPI_TYPE_INTEGER) {
		status = AE_BAD_DATA;
		goto out;
	}
	wakeup->sleep_state = element->integer.value;

	if ((package->package.count - 2) > ACPI_MAX_HANDLES) {
		status = AE_NO_MEMORY;
		goto out;
	}
	wakeup->resources.count = package->package.count - 2;
	for (i = 0; i < wakeup->resources.count; i++) {
		element = &(package->package.elements[i + 2]);
		if (element->type != ACPI_TYPE_LOCAL_REFERENCE) {
			status = AE_BAD_DATA;
			goto out;
		}

		wakeup->resources.handles[i] = element->reference.handle;
	}

	acpi_setup_gpe_for_wake(handle, wakeup->gpe_device, wakeup->gpe_number);

 out:
	kfree(buffer.pointer);

	return status;
}

static void acpi_bus_set_run_wake_flags(struct acpi_device *device)
{
	struct acpi_device_id button_device_ids[] = {
		{"PNP0C0C", 0},
		{"PNP0C0D", 0},
		{"PNP0C0E", 0},
		{"", 0},
	};
	acpi_status status;
	acpi_event_status event_status;

	device->wakeup.flags.notifier_present = 0;

	/* Power button, Lid switch always enable wakeup */
	if (!acpi_match_device_ids(device, button_device_ids)) {
		device->wakeup.flags.run_wake = 1;
		if (!acpi_match_device_ids(device, &button_device_ids[1])) {
			/* Do not use Lid/sleep button for S5 wakeup */
			if (device->wakeup.sleep_state == ACPI_STATE_S5)
				device->wakeup.sleep_state = ACPI_STATE_S4;
		}
		device_set_wakeup_capable(&device->dev, true);
		return;
	}

	status = acpi_get_gpe_status(device->wakeup.gpe_device,
					device->wakeup.gpe_number,
						&event_status);
	if (status == AE_OK)
		device->wakeup.flags.run_wake =
				!!(event_status & ACPI_EVENT_FLAG_HANDLE);
=======
	if (element->type != ACPI_TYPE_INTEGER)
		goto out;

	wakeup->sleep_state = element->integer.value;

	err = acpi_extract_power_resources(package, 2, &wakeup->resources);
	if (err)
		goto out;

	if (!list_empty(&wakeup->resources)) {
		int sleep_state;

		err = acpi_power_wakeup_list_init(&wakeup->resources,
						  &sleep_state);
		if (err) {
			acpi_handle_warn(handle, "Retrieving current states "
					 "of wakeup power resources failed\n");
			acpi_power_resources_list_free(&wakeup->resources);
			goto out;
		}
		if (sleep_state < wakeup->sleep_state) {
			acpi_handle_warn(handle, "Overriding _PRW sleep state "
					 "(S%d) by S%d from power resources\n",
					 (int)wakeup->sleep_state, sleep_state);
			wakeup->sleep_state = sleep_state;
		}
	}

 out:
	kfree(buffer.pointer);
	return err;
}

/* Do not use a button for S5 wakeup */
#define ACPI_AVOID_WAKE_FROM_S5		BIT(0)

static bool acpi_wakeup_gpe_init(struct acpi_device *device)
{
	static const struct acpi_device_id button_device_ids[] = {
		{"PNP0C0C", 0},				/* Power button */
		{"PNP0C0D", ACPI_AVOID_WAKE_FROM_S5},	/* Lid */
		{"PNP0C0E", ACPI_AVOID_WAKE_FROM_S5},	/* Sleep button */
		{"", 0},
	};
	struct acpi_device_wakeup *wakeup = &device->wakeup;
	const struct acpi_device_id *match;
	acpi_status status;

	wakeup->flags.notifier_present = 0;

	/* Power button, Lid switch always enable wakeup */
	match = acpi_match_acpi_device(button_device_ids, device);
	if (match) {
		if ((match->driver_data & ACPI_AVOID_WAKE_FROM_S5) &&
		    wakeup->sleep_state == ACPI_STATE_S5)
			wakeup->sleep_state = ACPI_STATE_S4;
		acpi_mark_gpe_for_wake(wakeup->gpe_device, wakeup->gpe_number);
		device_set_wakeup_capable(&device->dev, true);
		return true;
	}

	status = acpi_setup_gpe_for_wake(device->handle, wakeup->gpe_device,
					 wakeup->gpe_number);
	return ACPI_SUCCESS(status);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

static void acpi_bus_get_wakeup_device_flags(struct acpi_device *device)
{
<<<<<<< HEAD
	acpi_handle temp;
	acpi_status status = 0;
	int psw_error;

	/* Presence of _PRW indicates wake capable */
	status = acpi_get_handle(device->handle, "_PRW", &temp);
	if (ACPI_FAILURE(status))
		return;

	status = acpi_bus_extract_wakeup_device_power_package(device->handle,
							      &device->wakeup);
	if (ACPI_FAILURE(status)) {
		ACPI_EXCEPTION((AE_INFO, status, "Extracting _PRW package"));
		return;
	}

	device->wakeup.flags.valid = 1;
	device->wakeup.prepare_count = 0;
	acpi_bus_set_run_wake_flags(device);
	/* Call _PSW/_DSW object to disable its ability to wake the sleeping
	 * system for the ACPI device with the _PRW object.
	 * The _PSW object is depreciated in ACPI 3.0 and is replaced by _DSW.
	 * So it is necessary to call _DSW object first. Only when it is not
	 * present will the _PSW object used.
	 */
	psw_error = acpi_device_sleep_wake(device, 0, 0, 0);
	if (psw_error)
		ACPI_DEBUG_PRINT((ACPI_DB_INFO,
				"error in _DSW or _PSW evaluation\n"));
}

static void acpi_bus_add_power_resource(acpi_handle handle);

static int acpi_bus_get_power_flags(struct acpi_device *device)
{
	acpi_status status = 0;
	acpi_handle handle = NULL;
	u32 i = 0;

=======
	int err;

	/* Presence of _PRW indicates wake capable */
	if (!acpi_has_method(device->handle, "_PRW"))
		return;

	err = acpi_bus_extract_wakeup_device_power_package(device);
	if (err) {
		dev_err(&device->dev, "Unable to extract wakeup power resources");
		return;
	}

	device->wakeup.flags.valid = acpi_wakeup_gpe_init(device);
	device->wakeup.prepare_count = 0;
	/*
	 * Call _PSW/_DSW object to disable its ability to wake the sleeping
	 * system for the ACPI device with the _PRW object.
	 * The _PSW object is deprecated in ACPI 3.0 and is replaced by _DSW.
	 * So it is necessary to call _DSW object first. Only when it is not
	 * present will the _PSW object used.
	 */
	err = acpi_device_sleep_wake(device, 0, 0, 0);
	if (err)
		pr_debug("error in _DSW or _PSW evaluation\n");
}

static void acpi_bus_init_power_state(struct acpi_device *device, int state)
{
	struct acpi_device_power_state *ps = &device->power.states[state];
	char pathname[5] = { '_', 'P', 'R', '0' + state, '\0' };
	struct acpi_buffer buffer = { ACPI_ALLOCATE_BUFFER, NULL };
	acpi_status status;

	INIT_LIST_HEAD(&ps->resources);

	/* Evaluate "_PRx" to get referenced power resources */
	status = acpi_evaluate_object(device->handle, pathname, NULL, &buffer);
	if (ACPI_SUCCESS(status)) {
		union acpi_object *package = buffer.pointer;

		if (buffer.length && package
		    && package->type == ACPI_TYPE_PACKAGE
		    && package->package.count)
			acpi_extract_power_resources(package, 0, &ps->resources);

		ACPI_FREE(buffer.pointer);
	}

	/* Evaluate "_PSx" to see if we can do explicit sets */
	pathname[2] = 'S';
	if (acpi_has_method(device->handle, pathname))
		ps->flags.explicit_set = 1;

	/* State is valid if there are means to put the device into it. */
	if (!list_empty(&ps->resources) || ps->flags.explicit_set)
		ps->flags.valid = 1;

	ps->power = -1;		/* Unknown - driver assigned */
	ps->latency = -1;	/* Unknown - driver assigned */
}

static void acpi_bus_get_power_flags(struct acpi_device *device)
{
	unsigned long long dsc = ACPI_STATE_D0;
	u32 i;

	/* Presence of _PS0|_PR0 indicates 'power manageable' */
	if (!acpi_has_method(device->handle, "_PS0") &&
	    !acpi_has_method(device->handle, "_PR0"))
		return;

	device->flags.power_manageable = 1;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	/*
	 * Power Management Flags
	 */
<<<<<<< HEAD
	status = acpi_get_handle(device->handle, "_PSC", &handle);
	if (ACPI_SUCCESS(status))
		device->power.flags.explicit_get = 1;
	status = acpi_get_handle(device->handle, "_IRC", &handle);
	if (ACPI_SUCCESS(status))
		device->power.flags.inrush_current = 1;

	/*
	 * Enumerate supported power management states
	 */
	for (i = ACPI_STATE_D0; i <= ACPI_STATE_D3_HOT; i++) {
		struct acpi_device_power_state *ps = &device->power.states[i];
		char object_name[5] = { '_', 'P', 'R', '0' + i, '\0' };

		/* Evaluate "_PRx" to se if power resources are referenced */
		acpi_evaluate_reference(device->handle, object_name, NULL,
					&ps->resources);
		if (ps->resources.count) {
			int j;

			device->power.flags.power_resources = 1;
			for (j = 0; j < ps->resources.count; j++)
				acpi_bus_add_power_resource(ps->resources.handles[j]);
		}

		/* Evaluate "_PSx" to see if we can do explicit sets */
		object_name[2] = 'S';
		status = acpi_get_handle(device->handle, object_name, &handle);
		if (ACPI_SUCCESS(status))
			ps->flags.explicit_set = 1;

		/*
		 * State is valid if there are means to put the device into it.
		 * D3hot is only valid if _PR3 present.
		 */
		if (ps->resources.count ||
		    (ps->flags.explicit_set && i < ACPI_STATE_D3_HOT))
			ps->flags.valid = 1;

		ps->power = -1;	/* Unknown - driver assigned */
		ps->latency = -1;	/* Unknown - driver assigned */
	}

	/* Set defaults for D0 and D3 states (always valid) */
	device->power.states[ACPI_STATE_D0].flags.valid = 1;
	device->power.states[ACPI_STATE_D0].power = 100;
	device->power.states[ACPI_STATE_D3].flags.valid = 1;
	device->power.states[ACPI_STATE_D3].power = 0;

	/* Set D3cold's explicit_set flag if _PS3 exists. */
	if (device->power.states[ACPI_STATE_D3_HOT].flags.explicit_set)
		device->power.states[ACPI_STATE_D3_COLD].flags.explicit_set = 1;

	acpi_bus_init_power(device);

	return 0;
}

static int acpi_bus_get_flags(struct acpi_device *device)
{
	acpi_status status = AE_OK;
	acpi_handle temp = NULL;


	/* Presence of _STA indicates 'dynamic_status' */
	status = acpi_get_handle(device->handle, "_STA", &temp);
	if (ACPI_SUCCESS(status))
		device->flags.dynamic_status = 1;

	/* Presence of _RMV indicates 'removable' */
	status = acpi_get_handle(device->handle, "_RMV", &temp);
	if (ACPI_SUCCESS(status))
		device->flags.removable = 1;

	/* Presence of _EJD|_EJ0 indicates 'ejectable' */
	status = acpi_get_handle(device->handle, "_EJD", &temp);
	if (ACPI_SUCCESS(status))
		device->flags.ejectable = 1;
	else {
		status = acpi_get_handle(device->handle, "_EJ0", &temp);
		if (ACPI_SUCCESS(status))
			device->flags.ejectable = 1;
	}

	/* Presence of _LCK indicates 'lockable' */
	status = acpi_get_handle(device->handle, "_LCK", &temp);
	if (ACPI_SUCCESS(status))
		device->flags.lockable = 1;

	/* Power resources cannot be power manageable. */
	if (device->device_type == ACPI_BUS_TYPE_POWER)
		return 0;

	/* Presence of _PS0|_PR0 indicates 'power manageable' */
	status = acpi_get_handle(device->handle, "_PS0", &temp);
	if (ACPI_FAILURE(status))
		status = acpi_get_handle(device->handle, "_PR0", &temp);
	if (ACPI_SUCCESS(status))
		device->flags.power_manageable = 1;

	/* TBD: Performance management */

	return 0;
=======
	if (acpi_has_method(device->handle, "_PSC"))
		device->power.flags.explicit_get = 1;

	if (acpi_has_method(device->handle, "_IRC"))
		device->power.flags.inrush_current = 1;

	if (acpi_has_method(device->handle, "_DSW"))
		device->power.flags.dsw_present = 1;

	acpi_evaluate_integer(device->handle, "_DSC", NULL, &dsc);
	device->power.state_for_enumeration = dsc;

	/*
	 * Enumerate supported power management states
	 */
	for (i = ACPI_STATE_D0; i <= ACPI_STATE_D3_HOT; i++)
		acpi_bus_init_power_state(device, i);

	INIT_LIST_HEAD(&device->power.states[ACPI_STATE_D3_COLD].resources);

	/* Set the defaults for D0 and D3hot (always supported). */
	device->power.states[ACPI_STATE_D0].flags.valid = 1;
	device->power.states[ACPI_STATE_D0].power = 100;
	device->power.states[ACPI_STATE_D3_HOT].flags.valid = 1;

	/*
	 * Use power resources only if the D0 list of them is populated, because
	 * some platforms may provide _PR3 only to indicate D3cold support and
	 * in those cases the power resources list returned by it may be bogus.
	 */
	if (!list_empty(&device->power.states[ACPI_STATE_D0].resources)) {
		device->power.flags.power_resources = 1;
		/*
		 * D3cold is supported if the D3hot list of power resources is
		 * not empty.
		 */
		if (!list_empty(&device->power.states[ACPI_STATE_D3_HOT].resources))
			device->power.states[ACPI_STATE_D3_COLD].flags.valid = 1;
	}

	if (acpi_bus_init_power(device))
		device->flags.power_manageable = 0;
}

static void acpi_bus_get_flags(struct acpi_device *device)
{
	/* Presence of _STA indicates 'dynamic_status' */
	if (acpi_has_method(device->handle, "_STA"))
		device->flags.dynamic_status = 1;

	/* Presence of _RMV indicates 'removable' */
	if (acpi_has_method(device->handle, "_RMV"))
		device->flags.removable = 1;

	/* Presence of _EJD|_EJ0 indicates 'ejectable' */
	if (acpi_has_method(device->handle, "_EJD") ||
	    acpi_has_method(device->handle, "_EJ0"))
		device->flags.ejectable = 1;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

static void acpi_device_get_busid(struct acpi_device *device)
{
	char bus_id[5] = { '?', 0 };
	struct acpi_buffer buffer = { sizeof(bus_id), bus_id };
	int i = 0;

	/*
	 * Bus ID
	 * ------
	 * The device's Bus ID is simply the object name.
	 * TBD: Shouldn't this value be unique (within the ACPI namespace)?
	 */
<<<<<<< HEAD
	if (ACPI_IS_ROOT_DEVICE(device)) {
=======
	if (!acpi_dev_parent(device)) {
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		strcpy(device->pnp.bus_id, "ACPI");
		return;
	}

	switch (device->device_type) {
	case ACPI_BUS_TYPE_POWER_BUTTON:
		strcpy(device->pnp.bus_id, "PWRF");
		break;
	case ACPI_BUS_TYPE_SLEEP_BUTTON:
		strcpy(device->pnp.bus_id, "SLPF");
		break;
<<<<<<< HEAD
=======
	case ACPI_BUS_TYPE_ECDT_EC:
		strcpy(device->pnp.bus_id, "ECDT");
		break;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	default:
		acpi_get_name(device->handle, ACPI_SINGLE_NAME, &buffer);
		/* Clean up trailing underscores (if any) */
		for (i = 3; i > 1; i--) {
			if (bus_id[i] == '_')
				bus_id[i] = '\0';
			else
				break;
		}
		strcpy(device->pnp.bus_id, bus_id);
		break;
	}
}

/*
<<<<<<< HEAD
 * acpi_bay_match - see if a device is an ejectable driver bay
=======
 * acpi_ata_match - see if an acpi object is an ATA device
 *
 * If an acpi object has one of the ACPI ATA methods defined,
 * then we can safely call it an ATA device.
 */
bool acpi_ata_match(acpi_handle handle)
{
	return acpi_has_method(handle, "_GTF") ||
	       acpi_has_method(handle, "_GTM") ||
	       acpi_has_method(handle, "_STM") ||
	       acpi_has_method(handle, "_SDD");
}

/*
 * acpi_bay_match - see if an acpi object is an ejectable driver bay
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 *
 * If an acpi object is ejectable and has one of the ACPI ATA methods defined,
 * then we can safely call it an ejectable drive bay
 */
<<<<<<< HEAD
static int acpi_bay_match(struct acpi_device *device){
	acpi_status status;
	acpi_handle handle;
	acpi_handle tmp;
	acpi_handle phandle;

	handle = device->handle;

	status = acpi_get_handle(handle, "_EJ0", &tmp);
	if (ACPI_FAILURE(status))
		return -ENODEV;

	if ((ACPI_SUCCESS(acpi_get_handle(handle, "_GTF", &tmp))) ||
		(ACPI_SUCCESS(acpi_get_handle(handle, "_GTM", &tmp))) ||
		(ACPI_SUCCESS(acpi_get_handle(handle, "_STM", &tmp))) ||
		(ACPI_SUCCESS(acpi_get_handle(handle, "_SDD", &tmp))))
		return 0;

	if (acpi_get_parent(handle, &phandle))
		return -ENODEV;

        if ((ACPI_SUCCESS(acpi_get_handle(phandle, "_GTF", &tmp))) ||
                (ACPI_SUCCESS(acpi_get_handle(phandle, "_GTM", &tmp))) ||
                (ACPI_SUCCESS(acpi_get_handle(phandle, "_STM", &tmp))) ||
                (ACPI_SUCCESS(acpi_get_handle(phandle, "_SDD", &tmp))))
                return 0;

	return -ENODEV;
}

/*
 * acpi_dock_match - see if a device has a _DCK method
 */
static int acpi_dock_match(struct acpi_device *device)
{
	acpi_handle tmp;
	return acpi_get_handle(device->handle, "_DCK", &tmp);
}

=======
bool acpi_bay_match(acpi_handle handle)
{
	acpi_handle phandle;

	if (!acpi_has_method(handle, "_EJ0"))
		return false;
	if (acpi_ata_match(handle))
		return true;
	if (ACPI_FAILURE(acpi_get_parent(handle, &phandle)))
		return false;

	return acpi_ata_match(phandle);
}

bool acpi_device_is_battery(struct acpi_device *adev)
{
	struct acpi_hardware_id *hwid;

	list_for_each_entry(hwid, &adev->pnp.ids, list)
		if (!strcmp("PNP0C0A", hwid->id))
			return true;

	return false;
}

static bool is_ejectable_bay(struct acpi_device *adev)
{
	acpi_handle handle = adev->handle;

	if (acpi_has_method(handle, "_EJ0") && acpi_device_is_battery(adev))
		return true;

	return acpi_bay_match(handle);
}

/*
 * acpi_dock_match - see if an acpi object has a _DCK method
 */
bool acpi_dock_match(acpi_handle handle)
{
	return acpi_has_method(handle, "_DCK");
}

static acpi_status
acpi_backlight_cap_match(acpi_handle handle, u32 level, void *context,
			  void **return_value)
{
	long *cap = context;

	if (acpi_has_method(handle, "_BCM") &&
	    acpi_has_method(handle, "_BCL")) {
		acpi_handle_debug(handle, "Found generic backlight support\n");
		*cap |= ACPI_VIDEO_BACKLIGHT;
		/* We have backlight support, no need to scan further */
		return AE_CTRL_TERMINATE;
	}
	return 0;
}

/* Returns true if the ACPI object is a video device which can be
 * handled by video.ko.
 * The device will get a Linux specific CID added in scan.c to
 * identify the device as an ACPI graphics device
 * Be aware that the graphics device may not be physically present
 * Use acpi_video_get_capabilities() to detect general ACPI video
 * capabilities of present cards
 */
long acpi_is_video_device(acpi_handle handle)
{
	long video_caps = 0;

	/* Is this device able to support video switching ? */
	if (acpi_has_method(handle, "_DOD") || acpi_has_method(handle, "_DOS"))
		video_caps |= ACPI_VIDEO_OUTPUT_SWITCHING;

	/* Is this device able to retrieve a video ROM ? */
	if (acpi_has_method(handle, "_ROM"))
		video_caps |= ACPI_VIDEO_ROM_AVAILABLE;

	/* Is this device able to configure which video head to be POSTed ? */
	if (acpi_has_method(handle, "_VPO") &&
	    acpi_has_method(handle, "_GPD") &&
	    acpi_has_method(handle, "_SPD"))
		video_caps |= ACPI_VIDEO_DEVICE_POSTING;

	/* Only check for backlight functionality if one of the above hit. */
	if (video_caps)
		acpi_walk_namespace(ACPI_TYPE_DEVICE, handle,
				    ACPI_UINT32_MAX, acpi_backlight_cap_match, NULL,
				    &video_caps, NULL);

	return video_caps;
}
EXPORT_SYMBOL(acpi_is_video_device);

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
const char *acpi_device_hid(struct acpi_device *device)
{
	struct acpi_hardware_id *hid;

	if (list_empty(&device->pnp.ids))
		return dummy_hid;

	hid = list_first_entry(&device->pnp.ids, struct acpi_hardware_id, list);
	return hid->id;
}
EXPORT_SYMBOL(acpi_device_hid);

<<<<<<< HEAD
static void acpi_add_id(struct acpi_device *device, const char *dev_id)
=======
static void acpi_add_id(struct acpi_device_pnp *pnp, const char *dev_id)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
{
	struct acpi_hardware_id *id;

	id = kmalloc(sizeof(*id), GFP_KERNEL);
	if (!id)
		return;

<<<<<<< HEAD
	id->id = kstrdup(dev_id, GFP_KERNEL);
=======
	id->id = kstrdup_const(dev_id, GFP_KERNEL);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	if (!id->id) {
		kfree(id);
		return;
	}

<<<<<<< HEAD
	list_add_tail(&id->list, &device->pnp.ids);
=======
	list_add_tail(&id->list, &pnp->ids);
	pnp->type.hardware_id = 1;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

/*
 * Old IBM workstations have a DSDT bug wherein the SMBus object
 * lacks the SMBUS01 HID and the methods do not have the necessary "_"
 * prefix.  Work around this.
 */
<<<<<<< HEAD
static int acpi_ibm_smbus_match(struct acpi_device *device)
{
	acpi_handle h_dummy;
	struct acpi_buffer path = {ACPI_ALLOCATE_BUFFER, NULL};
	int result;

	if (!dmi_name_in_vendors("IBM"))
		return -ENODEV;

	/* Look for SMBS object */
	result = acpi_get_name(device->handle, ACPI_SINGLE_NAME, &path);
	if (result)
		return result;

	if (strcmp("SMBS", path.pointer)) {
		result = -ENODEV;
		goto out;
	}

	/* Does it have the necessary (but misnamed) methods? */
	result = -ENODEV;
	if (ACPI_SUCCESS(acpi_get_handle(device->handle, "SBI", &h_dummy)) &&
	    ACPI_SUCCESS(acpi_get_handle(device->handle, "SBR", &h_dummy)) &&
	    ACPI_SUCCESS(acpi_get_handle(device->handle, "SBW", &h_dummy)))
		result = 0;
out:
	kfree(path.pointer);
	return result;
}

static void acpi_device_set_id(struct acpi_device *device)
{
	acpi_status status;
	struct acpi_device_info *info;
	struct acpica_device_id_list *cid_list;
	int i;

	switch (device->device_type) {
	case ACPI_BUS_TYPE_DEVICE:
		if (ACPI_IS_ROOT_DEVICE(device)) {
			acpi_add_id(device, ACPI_SYSTEM_HID);
			break;
		}

		status = acpi_get_object_info(device->handle, &info);
		if (ACPI_FAILURE(status)) {
			printk(KERN_ERR PREFIX "%s: Error reading device info\n", __func__);
			return;
		}

		if (info->valid & ACPI_VALID_HID)
			acpi_add_id(device, info->hardware_id.string);
		if (info->valid & ACPI_VALID_CID) {
			cid_list = &info->compatible_id_list;
			for (i = 0; i < cid_list->count; i++)
				acpi_add_id(device, cid_list->ids[i].string);
		}
		if (info->valid & ACPI_VALID_ADR) {
			device->pnp.bus_address = info->address;
			device->flags.bus_address = 1;
		}
=======
static bool acpi_ibm_smbus_match(acpi_handle handle)
{
	char node_name[ACPI_PATH_SEGMENT_LENGTH];
	struct acpi_buffer path = { sizeof(node_name), node_name };

	if (!dmi_name_in_vendors("IBM"))
		return false;

	/* Look for SMBS object */
	if (ACPI_FAILURE(acpi_get_name(handle, ACPI_SINGLE_NAME, &path)) ||
	    strcmp("SMBS", path.pointer))
		return false;

	/* Does it have the necessary (but misnamed) methods? */
	if (acpi_has_method(handle, "SBI") &&
	    acpi_has_method(handle, "SBR") &&
	    acpi_has_method(handle, "SBW"))
		return true;

	return false;
}

static bool acpi_object_is_system_bus(acpi_handle handle)
{
	acpi_handle tmp;

	if (ACPI_SUCCESS(acpi_get_handle(NULL, "\\_SB", &tmp)) &&
	    tmp == handle)
		return true;
	if (ACPI_SUCCESS(acpi_get_handle(NULL, "\\_TZ", &tmp)) &&
	    tmp == handle)
		return true;

	return false;
}

static void acpi_set_pnp_ids(acpi_handle handle, struct acpi_device_pnp *pnp,
			     int device_type)
{
	struct acpi_device_info *info = NULL;
	struct acpi_pnp_device_id_list *cid_list;
	int i;

	switch (device_type) {
	case ACPI_BUS_TYPE_DEVICE:
		if (handle == ACPI_ROOT_OBJECT) {
			acpi_add_id(pnp, ACPI_SYSTEM_HID);
			break;
		}

		acpi_get_object_info(handle, &info);
		if (!info) {
			pr_err("%s: Error reading device info\n", __func__);
			return;
		}

		if (info->valid & ACPI_VALID_HID) {
			acpi_add_id(pnp, info->hardware_id.string);
			pnp->type.platform_id = 1;
		}
		if (info->valid & ACPI_VALID_CID) {
			cid_list = &info->compatible_id_list;
			for (i = 0; i < cid_list->count; i++)
				acpi_add_id(pnp, cid_list->ids[i].string);
		}
		if (info->valid & ACPI_VALID_ADR) {
			pnp->bus_address = info->address;
			pnp->type.bus_address = 1;
		}
		if (info->valid & ACPI_VALID_UID)
			pnp->unique_id = kstrdup(info->unique_id.string,
							GFP_KERNEL);
		if (info->valid & ACPI_VALID_CLS)
			acpi_add_id(pnp, info->class_code.string);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

		kfree(info);

		/*
		 * Some devices don't reliably have _HIDs & _CIDs, so add
		 * synthetic HIDs to make sure drivers can find them.
		 */
<<<<<<< HEAD
		if (acpi_is_video_device(device))
			acpi_add_id(device, ACPI_VIDEO_HID);
		else if (ACPI_SUCCESS(acpi_bay_match(device)))
			acpi_add_id(device, ACPI_BAY_HID);
		else if (ACPI_SUCCESS(acpi_dock_match(device)))
			acpi_add_id(device, ACPI_DOCK_HID);
		else if (!acpi_ibm_smbus_match(device))
			acpi_add_id(device, ACPI_SMBUS_IBM_HID);
		else if (list_empty(&device->pnp.ids) &&
			 ACPI_IS_ROOT_DEVICE(device->parent)) {
			acpi_add_id(device, ACPI_BUS_HID); /* \_SB, LNXSYBUS */
			strcpy(device->pnp.device_name, ACPI_BUS_DEVICE_NAME);
			strcpy(device->pnp.device_class, ACPI_BUS_CLASS);
=======
		if (acpi_is_video_device(handle)) {
			acpi_add_id(pnp, ACPI_VIDEO_HID);
			pnp->type.backlight = 1;
			break;
		}
		if (acpi_bay_match(handle))
			acpi_add_id(pnp, ACPI_BAY_HID);
		else if (acpi_dock_match(handle))
			acpi_add_id(pnp, ACPI_DOCK_HID);
		else if (acpi_ibm_smbus_match(handle))
			acpi_add_id(pnp, ACPI_SMBUS_IBM_HID);
		else if (list_empty(&pnp->ids) &&
			 acpi_object_is_system_bus(handle)) {
			/* \_SB, \_TZ, LNXSYBUS */
			acpi_add_id(pnp, ACPI_BUS_HID);
			strcpy(pnp->device_name, ACPI_BUS_DEVICE_NAME);
			strcpy(pnp->device_class, ACPI_BUS_CLASS);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		}

		break;
	case ACPI_BUS_TYPE_POWER:
<<<<<<< HEAD
		acpi_add_id(device, ACPI_POWER_HID);
		break;
	case ACPI_BUS_TYPE_PROCESSOR:
		acpi_add_id(device, ACPI_PROCESSOR_OBJECT_HID);
		break;
	case ACPI_BUS_TYPE_THERMAL:
		acpi_add_id(device, ACPI_THERMAL_HID);
		break;
	case ACPI_BUS_TYPE_POWER_BUTTON:
		acpi_add_id(device, ACPI_BUTTON_HID_POWERF);
		break;
	case ACPI_BUS_TYPE_SLEEP_BUTTON:
		acpi_add_id(device, ACPI_BUTTON_HID_SLEEPF);
=======
		acpi_add_id(pnp, ACPI_POWER_HID);
		break;
	case ACPI_BUS_TYPE_PROCESSOR:
		acpi_add_id(pnp, ACPI_PROCESSOR_OBJECT_HID);
		break;
	case ACPI_BUS_TYPE_THERMAL:
		acpi_add_id(pnp, ACPI_THERMAL_HID);
		break;
	case ACPI_BUS_TYPE_POWER_BUTTON:
		acpi_add_id(pnp, ACPI_BUTTON_HID_POWERF);
		break;
	case ACPI_BUS_TYPE_SLEEP_BUTTON:
		acpi_add_id(pnp, ACPI_BUTTON_HID_SLEEPF);
		break;
	case ACPI_BUS_TYPE_ECDT_EC:
		acpi_add_id(pnp, ACPI_ECDT_HID);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		break;
	}
}

<<<<<<< HEAD
static int acpi_device_set_context(struct acpi_device *device)
{
	acpi_status status;

	/*
	 * Context
	 * -------
	 * Attach this 'struct acpi_device' to the ACPI object.  This makes
	 * resolutions from handle->device very efficient.  Fixed hardware
	 * devices have no handles, so we skip them.
	 */
	if (!device->handle)
		return 0;

	status = acpi_attach_data(device->handle,
				  acpi_bus_data_handler, device);
	if (ACPI_SUCCESS(status))
		return 0;

	printk(KERN_ERR PREFIX "Error attaching device data\n");
	return -ENODEV;
}

static int acpi_bus_remove(struct acpi_device *dev, int rmdevice)
{
	if (!dev)
		return -EINVAL;

	dev->removal_type = ACPI_BUS_REMOVAL_EJECT;
	device_release_driver(&dev->dev);

	if (!rmdevice)
		return 0;

	/*
	 * unbind _ADR-Based Devices when hot removal
	 */
	if (dev->flags.bus_address) {
		if ((dev->parent) && (dev->parent->ops.unbind))
			dev->parent->ops.unbind(dev);
	}
	acpi_device_unregister(dev, ACPI_BUS_REMOVAL_EJECT);

	return 0;
}

static int acpi_add_single_object(struct acpi_device **child,
				  acpi_handle handle, int type,
				  unsigned long long sta,
				  struct acpi_bus_ops *ops)
{
	int result;
	struct acpi_device *device;
	struct acpi_buffer buffer = { ACPI_ALLOCATE_BUFFER, NULL };

	device = kzalloc(sizeof(struct acpi_device), GFP_KERNEL);
	if (!device) {
		printk(KERN_ERR PREFIX "Memory allocation error\n");
		return -ENOMEM;
	}

	INIT_LIST_HEAD(&device->pnp.ids);
	device->device_type = type;
	device->handle = handle;
	device->parent = acpi_bus_get_parent(handle);
	device->bus_ops = *ops; /* workround for not call .start */
	STRUCT_TO_INT(device->status) = sta;

	acpi_device_get_busid(device);

	/*
	 * Flags
	 * -----
	 * Note that we only look for object handles -- cannot evaluate objects
	 * until we know the device is present and properly initialized.
	 */
	result = acpi_bus_get_flags(device);
	if (result)
		goto end;

	/*
	 * Initialize Device
	 * -----------------
	 * TBD: Synch with Core's enumeration/initialization process.
	 */
	acpi_device_set_id(device);

	/*
	 * Power Management
	 * ----------------
	 */
	if (device->flags.power_manageable) {
		result = acpi_bus_get_power_flags(device);
		if (result)
			goto end;
	}

	/*
	 * Wakeup device management
	 *-----------------------
	 */
	acpi_bus_get_wakeup_device_flags(device);

	/*
	 * Performance Management
	 * ----------------------
	 */
	if (device->flags.performance_manageable) {
		result = acpi_bus_get_perf_flags(device);
		if (result)
			goto end;
	}

	if ((result = acpi_device_set_context(device)))
		goto end;

	result = acpi_device_register(device);

	/*
	 * Bind _ADR-Based Devices when hot add
	 */
	if (device->flags.bus_address) {
		if (device->parent && device->parent->ops.bind)
			device->parent->ops.bind(device);
	}

end:
	if (!result) {
		acpi_get_name(handle, ACPI_FULL_PATHNAME, &buffer);
		ACPI_DEBUG_PRINT((ACPI_DB_INFO,
			"Adding %s [%s] parent %s\n", dev_name(&device->dev),
			 (char *) buffer.pointer,
			 device->parent ? dev_name(&device->parent->dev) :
					  "(null)"));
		kfree(buffer.pointer);
		*child = device;
	} else
		acpi_device_release(&device->dev);

	return result;
}

#define ACPI_STA_DEFAULT (ACPI_STA_DEVICE_PRESENT | ACPI_STA_DEVICE_ENABLED | \
			  ACPI_STA_DEVICE_UI      | ACPI_STA_DEVICE_FUNCTIONING)

static void acpi_bus_add_power_resource(acpi_handle handle)
{
	struct acpi_bus_ops ops = {
		.acpi_op_add = 1,
		.acpi_op_start = 1,
	};
	struct acpi_device *device = NULL;

	acpi_bus_get_device(handle, &device);
	if (!device)
		acpi_add_single_object(&device, handle, ACPI_BUS_TYPE_POWER,
					ACPI_STA_DEFAULT, &ops);
}

static int acpi_bus_type_and_status(acpi_handle handle, int *type,
				    unsigned long long *sta)
{
	acpi_status status;
	acpi_object_type acpi_type;

	status = acpi_get_type(handle, &acpi_type);
	if (ACPI_FAILURE(status))
		return -ENODEV;

	switch (acpi_type) {
	case ACPI_TYPE_ANY:		/* for ACPI_ROOT_OBJECT */
	case ACPI_TYPE_DEVICE:
		*type = ACPI_BUS_TYPE_DEVICE;
		status = acpi_bus_get_status_handle(handle, sta);
		if (ACPI_FAILURE(status))
			return -ENODEV;
		break;
	case ACPI_TYPE_PROCESSOR:
		*type = ACPI_BUS_TYPE_PROCESSOR;
		status = acpi_bus_get_status_handle(handle, sta);
		if (ACPI_FAILURE(status))
			return -ENODEV;
		break;
	case ACPI_TYPE_THERMAL:
		*type = ACPI_BUS_TYPE_THERMAL;
		*sta = ACPI_STA_DEFAULT;
		break;
	case ACPI_TYPE_POWER:
		*type = ACPI_BUS_TYPE_POWER;
		*sta = ACPI_STA_DEFAULT;
		break;
	default:
		return -ENODEV;
	}

	return 0;
}

static acpi_status acpi_bus_check_add(acpi_handle handle, u32 lvl,
				      void *context, void **return_value)
{
	struct acpi_bus_ops *ops = context;
	int type;
	unsigned long long sta;
	struct acpi_device *device;
	acpi_status status;
	int result;

	result = acpi_bus_type_and_status(handle, &type, &sta);
	if (result)
		return AE_OK;

	if (!(sta & ACPI_STA_DEVICE_PRESENT) &&
	    !(sta & ACPI_STA_DEVICE_FUNCTIONING)) {
		struct acpi_device_wakeup wakeup;
		acpi_handle temp;

		status = acpi_get_handle(handle, "_PRW", &temp);
		if (ACPI_SUCCESS(status))
			acpi_bus_extract_wakeup_device_power_package(handle,
								     &wakeup);
		return AE_CTRL_DEPTH;
	}

	/*
	 * We may already have an acpi_device from a previous enumeration.  If
	 * so, we needn't add it again, but we may still have to start it.
	 */
	device = NULL;
	acpi_bus_get_device(handle, &device);
	if (ops->acpi_op_add && !device)
		acpi_add_single_object(&device, handle, type, sta, ops);

	if (!device)
		return AE_CTRL_DEPTH;

	if (ops->acpi_op_start && !(ops->acpi_op_add)) {
		status = acpi_start_single_object(device);
		if (ACPI_FAILURE(status))
			return AE_CTRL_DEPTH;
	}

	if (!*return_value)
		*return_value = device;
	return AE_OK;
}

static int acpi_bus_scan(acpi_handle handle, struct acpi_bus_ops *ops,
			 struct acpi_device **child)
{
	acpi_status status;
	void *device = NULL;

	status = acpi_bus_check_add(handle, 0, ops, &device);
	if (ACPI_SUCCESS(status))
		acpi_walk_namespace(ACPI_TYPE_ANY, handle, ACPI_UINT32_MAX,
				    acpi_bus_check_add, NULL, ops, &device);

	if (child)
		*child = device;

	if (device)
		return 0;
	else
		return -ENODEV;
}

/*
 * acpi_bus_add and acpi_bus_start
 *
 * scan a given ACPI tree and (probably recently hot-plugged)
 * create and add or starts found devices.
 *
 * If no devices were found -ENODEV is returned which does not
 * mean that this is a real error, there just have been no suitable
 * ACPI objects in the table trunk from which the kernel could create
 * a device and add/start an appropriate driver.
 */

int
acpi_bus_add(struct acpi_device **child,
	     struct acpi_device *parent, acpi_handle handle, int type)
{
	struct acpi_bus_ops ops;

	memset(&ops, 0, sizeof(ops));
	ops.acpi_op_add = 1;

	return acpi_bus_scan(handle, &ops, child);
}
EXPORT_SYMBOL(acpi_bus_add);

int acpi_bus_start(struct acpi_device *device)
{
	struct acpi_bus_ops ops;
	int result;

	if (!device)
		return -EINVAL;

	memset(&ops, 0, sizeof(ops));
	ops.acpi_op_start = 1;

	result = acpi_bus_scan(device->handle, &ops, NULL);

	acpi_update_all_gpes();

	return result;
}
EXPORT_SYMBOL(acpi_bus_start);

int acpi_bus_trim(struct acpi_device *start, int rmdevice)
{
	acpi_status status;
	struct acpi_device *parent, *child;
	acpi_handle phandle, chandle;
	acpi_object_type type;
	u32 level = 1;
	int err = 0;

	parent = start;
	phandle = start->handle;
	child = chandle = NULL;

	while ((level > 0) && parent && (!err)) {
		status = acpi_get_next_object(ACPI_TYPE_ANY, phandle,
					      chandle, &chandle);

		/*
		 * If this scope is exhausted then move our way back up.
		 */
		if (ACPI_FAILURE(status)) {
			level--;
			chandle = phandle;
			acpi_get_parent(phandle, &phandle);
			child = parent;
			parent = parent->parent;

			if (level == 0)
				err = acpi_bus_remove(child, rmdevice);
			else
				err = acpi_bus_remove(child, 1);

			continue;
		}

		status = acpi_get_type(chandle, &type);
		if (ACPI_FAILURE(status)) {
			continue;
		}
		/*
		 * If there is a device corresponding to chandle then
		 * parse it (depth-first).
		 */
		if (acpi_bus_get_device(chandle, &child) == 0) {
			level++;
			phandle = chandle;
			chandle = NULL;
			parent = child;
		}
		continue;
	}
	return err;
}
EXPORT_SYMBOL_GPL(acpi_bus_trim);

static int acpi_bus_scan_fixed(void)
{
	int result = 0;
	struct acpi_device *device = NULL;
	struct acpi_bus_ops ops;

	memset(&ops, 0, sizeof(ops));
	ops.acpi_op_add = 1;
	ops.acpi_op_start = 1;

	/*
	 * Enumerate all fixed-feature devices.
	 */
	if ((acpi_gbl_FADT.flags & ACPI_FADT_POWER_BUTTON) == 0) {
		result = acpi_add_single_object(&device, NULL,
						ACPI_BUS_TYPE_POWER_BUTTON,
						ACPI_STA_DEFAULT,
						&ops);
	}

	if ((acpi_gbl_FADT.flags & ACPI_FADT_SLEEP_BUTTON) == 0) {
		result = acpi_add_single_object(&device, NULL,
						ACPI_BUS_TYPE_SLEEP_BUTTON,
						ACPI_STA_DEFAULT,
						&ops);
	}

	return result;
}

int __init acpi_scan_init(void)
{
	int result;
	struct acpi_bus_ops ops;

	memset(&ops, 0, sizeof(ops));
	ops.acpi_op_add = 1;
	ops.acpi_op_start = 1;

	result = bus_register(&acpi_bus_type);
	if (result) {
		/* We don't want to quit even if we failed to add suspend/resume */
		printk(KERN_ERR PREFIX "Could not register bus type\n");
	}

	acpi_power_init();

	/*
	 * Enumerate devices in the ACPI namespace.
	 */
	result = acpi_bus_scan(ACPI_ROOT_OBJECT, &ops, &acpi_root);

	if (!result)
		result = acpi_bus_scan_fixed();

	if (result)
		acpi_device_unregister(acpi_root, ACPI_BUS_REMOVAL_NORMAL);
	else
		acpi_update_all_gpes();

	return result;
}
=======
void acpi_free_pnp_ids(struct acpi_device_pnp *pnp)
{
	struct acpi_hardware_id *id, *tmp;

	list_for_each_entry_safe(id, tmp, &pnp->ids, list) {
		kfree_const(id->id);
		kfree(id);
	}
	kfree(pnp->unique_id);
}

/**
 * acpi_dma_supported - Check DMA support for the specified device.
 * @adev: The pointer to acpi device
 *
 * Return false if DMA is not supported. Otherwise, return true
 */
bool acpi_dma_supported(const struct acpi_device *adev)
{
	if (!adev)
		return false;

	if (adev->flags.cca_seen)
		return true;

	/*
	* Per ACPI 6.0 sec 6.2.17, assume devices can do cache-coherent
	* DMA on "Intel platforms".  Presumably that includes all x86 and
	* ia64, and other arches will set CONFIG_ACPI_CCA_REQUIRED=y.
	*/
	if (!IS_ENABLED(CONFIG_ACPI_CCA_REQUIRED))
		return true;

	return false;
}

/**
 * acpi_get_dma_attr - Check the supported DMA attr for the specified device.
 * @adev: The pointer to acpi device
 *
 * Return enum dev_dma_attr.
 */
enum dev_dma_attr acpi_get_dma_attr(struct acpi_device *adev)
{
	if (!acpi_dma_supported(adev))
		return DEV_DMA_NOT_SUPPORTED;

	if (adev->flags.coherent_dma)
		return DEV_DMA_COHERENT;
	else
		return DEV_DMA_NON_COHERENT;
}

/**
 * acpi_dma_get_range() - Get device DMA parameters.
 *
 * @dev: device to configure
 * @map: pointer to DMA ranges result
 *
 * Evaluate DMA regions and return pointer to DMA regions on
 * parsing success; it does not update the passed in values on failure.
 *
 * Return 0 on success, < 0 on failure.
 */
int acpi_dma_get_range(struct device *dev, const struct bus_dma_region **map)
{
	struct acpi_device *adev;
	LIST_HEAD(list);
	struct resource_entry *rentry;
	int ret;
	struct device *dma_dev = dev;
	struct bus_dma_region *r;

	/*
	 * Walk the device tree chasing an ACPI companion with a _DMA
	 * object while we go. Stop if we find a device with an ACPI
	 * companion containing a _DMA method.
	 */
	do {
		adev = ACPI_COMPANION(dma_dev);
		if (adev && acpi_has_method(adev->handle, METHOD_NAME__DMA))
			break;

		dma_dev = dma_dev->parent;
	} while (dma_dev);

	if (!dma_dev)
		return -ENODEV;

	if (!acpi_has_method(adev->handle, METHOD_NAME__CRS)) {
		acpi_handle_warn(adev->handle, "_DMA is valid only if _CRS is present\n");
		return -EINVAL;
	}

	ret = acpi_dev_get_dma_resources(adev, &list);
	if (ret > 0) {
		r = kcalloc(ret + 1, sizeof(*r), GFP_KERNEL);
		if (!r) {
			ret = -ENOMEM;
			goto out;
		}

		*map = r;

		list_for_each_entry(rentry, &list, node) {
			if (rentry->res->start >= rentry->res->end) {
				kfree(*map);
				*map = NULL;
				ret = -EINVAL;
				dev_dbg(dma_dev, "Invalid DMA regions configuration\n");
				goto out;
			}

			r->cpu_start = rentry->res->start;
			r->dma_start = rentry->res->start - rentry->offset;
			r->size = resource_size(rentry->res);
			r++;
		}
	}
 out:
	acpi_dev_free_resource_list(&list);

	return ret >= 0 ? 0 : ret;
}

#ifdef CONFIG_IOMMU_API
int acpi_iommu_fwspec_init(struct device *dev, u32 id,
			   struct fwnode_handle *fwnode,
			   const struct iommu_ops *ops)
{
	int ret = iommu_fwspec_init(dev, fwnode, ops);

	if (!ret)
		ret = iommu_fwspec_add_ids(dev, &id, 1);

	return ret;
}

static inline const struct iommu_ops *acpi_iommu_fwspec_ops(struct device *dev)
{
	struct iommu_fwspec *fwspec = dev_iommu_fwspec_get(dev);

	return fwspec ? fwspec->ops : NULL;
}

static int acpi_iommu_configure_id(struct device *dev, const u32 *id_in)
{
	int err;
	const struct iommu_ops *ops;

	/* Serialise to make dev->iommu stable under our potential fwspec */
	mutex_lock(&iommu_probe_device_lock);
	/*
	 * If we already translated the fwspec there is nothing left to do,
	 * return the iommu_ops.
	 */
	ops = acpi_iommu_fwspec_ops(dev);
	if (ops) {
		mutex_unlock(&iommu_probe_device_lock);
		return 0;
	}

	err = iort_iommu_configure_id(dev, id_in);
	if (err && err != -EPROBE_DEFER)
		err = viot_iommu_configure(dev);
	mutex_unlock(&iommu_probe_device_lock);

	/*
	 * If we have reason to believe the IOMMU driver missed the initial
	 * iommu_probe_device() call for dev, replay it to get things in order.
	 */
	if (!err && dev->bus)
		err = iommu_probe_device(dev);

	/* Ignore all other errors apart from EPROBE_DEFER */
	if (err == -EPROBE_DEFER) {
		return err;
	} else if (err) {
		dev_dbg(dev, "Adding to IOMMU failed: %d\n", err);
		return -ENODEV;
	}
	if (!acpi_iommu_fwspec_ops(dev))
		return -ENODEV;
	return 0;
}

#else /* !CONFIG_IOMMU_API */

int acpi_iommu_fwspec_init(struct device *dev, u32 id,
			   struct fwnode_handle *fwnode,
			   const struct iommu_ops *ops)
{
	return -ENODEV;
}

static int acpi_iommu_configure_id(struct device *dev, const u32 *id_in)
{
	return -ENODEV;
}

#endif /* !CONFIG_IOMMU_API */

/**
 * acpi_dma_configure_id - Set-up DMA configuration for the device.
 * @dev: The pointer to the device
 * @attr: device dma attributes
 * @input_id: input device id const value pointer
 */
int acpi_dma_configure_id(struct device *dev, enum dev_dma_attr attr,
			  const u32 *input_id)
{
	int ret;

	if (attr == DEV_DMA_NOT_SUPPORTED) {
		set_dma_ops(dev, &dma_dummy_ops);
		return 0;
	}

	acpi_arch_dma_setup(dev);

	ret = acpi_iommu_configure_id(dev, input_id);
	if (ret == -EPROBE_DEFER)
		return -EPROBE_DEFER;

	/*
	 * Historically this routine doesn't fail driver probing due to errors
	 * in acpi_iommu_configure_id()
	 */

	arch_setup_dma_ops(dev, 0, U64_MAX, attr == DEV_DMA_COHERENT);

	return 0;
}
EXPORT_SYMBOL_GPL(acpi_dma_configure_id);

static void acpi_init_coherency(struct acpi_device *adev)
{
	unsigned long long cca = 0;
	acpi_status status;
	struct acpi_device *parent = acpi_dev_parent(adev);

	if (parent && parent->flags.cca_seen) {
		/*
		 * From ACPI spec, OSPM will ignore _CCA if an ancestor
		 * already saw one.
		 */
		adev->flags.cca_seen = 1;
		cca = parent->flags.coherent_dma;
	} else {
		status = acpi_evaluate_integer(adev->handle, "_CCA",
					       NULL, &cca);
		if (ACPI_SUCCESS(status))
			adev->flags.cca_seen = 1;
		else if (!IS_ENABLED(CONFIG_ACPI_CCA_REQUIRED))
			/*
			 * If architecture does not specify that _CCA is
			 * required for DMA-able devices (e.g. x86),
			 * we default to _CCA=1.
			 */
			cca = 1;
		else
			acpi_handle_debug(adev->handle,
					  "ACPI device is missing _CCA.\n");
	}

	adev->flags.coherent_dma = cca;
}

static int acpi_check_serial_bus_slave(struct acpi_resource *ares, void *data)
{
	bool *is_serial_bus_slave_p = data;

	if (ares->type != ACPI_RESOURCE_TYPE_SERIAL_BUS)
		return 1;

	*is_serial_bus_slave_p = true;

	 /* no need to do more checking */
	return -1;
}

static bool acpi_is_indirect_io_slave(struct acpi_device *device)
{
	struct acpi_device *parent = acpi_dev_parent(device);
	static const struct acpi_device_id indirect_io_hosts[] = {
		{"HISI0191", 0},
		{}
	};

	return parent && !acpi_match_device_ids(parent, indirect_io_hosts);
}

static bool acpi_device_enumeration_by_parent(struct acpi_device *device)
{
	struct list_head resource_list;
	bool is_serial_bus_slave = false;
	static const struct acpi_device_id ignore_serial_bus_ids[] = {
	/*
	 * These devices have multiple SerialBus resources and a client
	 * device must be instantiated for each of them, each with
	 * its own device id.
	 * Normally we only instantiate one client device for the first
	 * resource, using the ACPI HID as id. These special cases are handled
	 * by the drivers/platform/x86/serial-multi-instantiate.c driver, which
	 * knows which client device id to use for each resource.
	 */
		{"BSG1160", },
		{"BSG2150", },
		{"CSC3551", },
		{"CSC3554", },
		{"CSC3556", },
		{"CSC3557", },
		{"INT33FE", },
		{"INT3515", },
		/* Non-conforming _HID for Cirrus Logic already released */
		{"CLSA0100", },
		{"CLSA0101", },
	/*
	 * Some ACPI devs contain SerialBus resources even though they are not
	 * attached to a serial bus at all.
	 */
		{ACPI_VIDEO_HID, },
		{"MSHW0028", },
	/*
	 * HIDs of device with an UartSerialBusV2 resource for which userspace
	 * expects a regular tty cdev to be created (instead of the in kernel
	 * serdev) and which have a kernel driver which expects a platform_dev
	 * such as the rfkill-gpio driver.
	 */
		{"BCM4752", },
		{"LNV4752", },
		{}
	};

	if (acpi_is_indirect_io_slave(device))
		return true;

	/* Macs use device properties in lieu of _CRS resources */
	if (x86_apple_machine &&
	    (fwnode_property_present(&device->fwnode, "spiSclkPeriod") ||
	     fwnode_property_present(&device->fwnode, "i2cAddress") ||
	     fwnode_property_present(&device->fwnode, "baud")))
		return true;

	if (!acpi_match_device_ids(device, ignore_serial_bus_ids))
		return false;

	INIT_LIST_HEAD(&resource_list);
	acpi_dev_get_resources(device, &resource_list,
			       acpi_check_serial_bus_slave,
			       &is_serial_bus_slave);
	acpi_dev_free_resource_list(&resource_list);

	return is_serial_bus_slave;
}

void acpi_init_device_object(struct acpi_device *device, acpi_handle handle,
			     int type, void (*release)(struct device *))
{
	struct acpi_device *parent = acpi_find_parent_acpi_dev(handle);

	INIT_LIST_HEAD(&device->pnp.ids);
	device->device_type = type;
	device->handle = handle;
	device->dev.parent = parent ? &parent->dev : NULL;
	device->dev.release = release;
	device->dev.bus = &acpi_bus_type;
	fwnode_init(&device->fwnode, &acpi_device_fwnode_ops);
	acpi_set_device_status(device, ACPI_STA_DEFAULT);
	acpi_device_get_busid(device);
	acpi_set_pnp_ids(handle, &device->pnp, type);
	acpi_init_properties(device);
	acpi_bus_get_flags(device);
	device->flags.match_driver = false;
	device->flags.initialized = true;
	device->flags.enumeration_by_parent =
		acpi_device_enumeration_by_parent(device);
	acpi_device_clear_enumerated(device);
	device_initialize(&device->dev);
	dev_set_uevent_suppress(&device->dev, true);
	acpi_init_coherency(device);
}

static void acpi_scan_dep_init(struct acpi_device *adev)
{
	struct acpi_dep_data *dep;

	list_for_each_entry(dep, &acpi_dep_list, node) {
		if (dep->consumer == adev->handle) {
			if (dep->honor_dep)
				adev->flags.honor_deps = 1;

			if (!dep->met)
				adev->dep_unmet++;
		}
	}
}

void acpi_device_add_finalize(struct acpi_device *device)
{
	dev_set_uevent_suppress(&device->dev, false);
	kobject_uevent(&device->dev.kobj, KOBJ_ADD);
}

static void acpi_scan_init_status(struct acpi_device *adev)
{
	if (acpi_bus_get_status(adev))
		acpi_set_device_status(adev, 0);
}

static int acpi_add_single_object(struct acpi_device **child,
				  acpi_handle handle, int type, bool dep_init)
{
	struct acpi_device *device;
	bool release_dep_lock = false;
	int result;

	device = kzalloc(sizeof(struct acpi_device), GFP_KERNEL);
	if (!device)
		return -ENOMEM;

	acpi_init_device_object(device, handle, type, acpi_device_release);
	/*
	 * Getting the status is delayed till here so that we can call
	 * acpi_bus_get_status() and use its quirk handling.  Note that
	 * this must be done before the get power-/wakeup_dev-flags calls.
	 */
	if (type == ACPI_BUS_TYPE_DEVICE || type == ACPI_BUS_TYPE_PROCESSOR) {
		if (dep_init) {
			mutex_lock(&acpi_dep_list_lock);
			/*
			 * Hold the lock until the acpi_tie_acpi_dev() call
			 * below to prevent concurrent acpi_scan_clear_dep()
			 * from deleting a dependency list entry without
			 * updating dep_unmet for the device.
			 */
			release_dep_lock = true;
			acpi_scan_dep_init(device);
		}
		acpi_scan_init_status(device);
	}

	acpi_bus_get_power_flags(device);
	acpi_bus_get_wakeup_device_flags(device);

	result = acpi_tie_acpi_dev(device);

	if (release_dep_lock)
		mutex_unlock(&acpi_dep_list_lock);

	if (!result)
		result = acpi_device_add(device);

	if (result) {
		acpi_device_release(&device->dev);
		return result;
	}

	acpi_power_add_remove_device(device, true);
	acpi_device_add_finalize(device);

	acpi_handle_debug(handle, "Added as %s, parent %s\n",
			  dev_name(&device->dev), device->dev.parent ?
				dev_name(device->dev.parent) : "(null)");

	*child = device;
	return 0;
}

static acpi_status acpi_get_resource_memory(struct acpi_resource *ares,
					    void *context)
{
	struct resource *res = context;

	if (acpi_dev_resource_memory(ares, res))
		return AE_CTRL_TERMINATE;

	return AE_OK;
}

static bool acpi_device_should_be_hidden(acpi_handle handle)
{
	acpi_status status;
	struct resource res;

	/* Check if it should ignore the UART device */
	if (!(spcr_uart_addr && acpi_has_method(handle, METHOD_NAME__CRS)))
		return false;

	/*
	 * The UART device described in SPCR table is assumed to have only one
	 * memory resource present. So we only look for the first one here.
	 */
	status = acpi_walk_resources(handle, METHOD_NAME__CRS,
				     acpi_get_resource_memory, &res);
	if (ACPI_FAILURE(status) || res.start != spcr_uart_addr)
		return false;

	acpi_handle_info(handle, "The UART device @%pa in SPCR table will be hidden\n",
			 &res.start);

	return true;
}

bool acpi_device_is_present(const struct acpi_device *adev)
{
	return adev->status.present || adev->status.functional;
}

bool acpi_device_is_enabled(const struct acpi_device *adev)
{
	return adev->status.present && adev->status.enabled;
}

static bool acpi_scan_handler_matching(struct acpi_scan_handler *handler,
				       const char *idstr,
				       const struct acpi_device_id **matchid)
{
	const struct acpi_device_id *devid;

	if (handler->match)
		return handler->match(idstr, matchid);

	for (devid = handler->ids; devid->id[0]; devid++)
		if (!strcmp((char *)devid->id, idstr)) {
			if (matchid)
				*matchid = devid;

			return true;
		}

	return false;
}

static struct acpi_scan_handler *acpi_scan_match_handler(const char *idstr,
					const struct acpi_device_id **matchid)
{
	struct acpi_scan_handler *handler;

	list_for_each_entry(handler, &acpi_scan_handlers_list, list_node)
		if (acpi_scan_handler_matching(handler, idstr, matchid))
			return handler;

	return NULL;
}

void acpi_scan_hotplug_enabled(struct acpi_hotplug_profile *hotplug, bool val)
{
	if (!!hotplug->enabled == !!val)
		return;

	mutex_lock(&acpi_scan_lock);

	hotplug->enabled = val;

	mutex_unlock(&acpi_scan_lock);
}

static void acpi_scan_init_hotplug(struct acpi_device *adev)
{
	struct acpi_hardware_id *hwid;

	if (acpi_dock_match(adev->handle) || is_ejectable_bay(adev)) {
		acpi_dock_add(adev);
		return;
	}
	list_for_each_entry(hwid, &adev->pnp.ids, list) {
		struct acpi_scan_handler *handler;

		handler = acpi_scan_match_handler(hwid->id, NULL);
		if (handler) {
			adev->flags.hotplug_notify = true;
			break;
		}
	}
}

static u32 acpi_scan_check_dep(acpi_handle handle)
{
	struct acpi_handle_list dep_devices;
	u32 count;
	int i;

	/*
	 * Check for _HID here to avoid deferring the enumeration of:
	 * 1. PCI devices.
	 * 2. ACPI nodes describing USB ports.
	 * Still, checking for _HID catches more then just these cases ...
	 */
	if (!acpi_has_method(handle, "_DEP") || !acpi_has_method(handle, "_HID"))
		return 0;

	if (!acpi_evaluate_reference(handle, "_DEP", NULL, &dep_devices)) {
		acpi_handle_debug(handle, "Failed to evaluate _DEP.\n");
		return 0;
	}

	for (count = 0, i = 0; i < dep_devices.count; i++) {
		struct acpi_device_info *info;
		struct acpi_dep_data *dep;
		bool skip, honor_dep;
		acpi_status status;

		status = acpi_get_object_info(dep_devices.handles[i], &info);
		if (ACPI_FAILURE(status)) {
			acpi_handle_debug(handle, "Error reading _DEP device info\n");
			continue;
		}

		skip = acpi_info_matches_ids(info, acpi_ignore_dep_ids);
		honor_dep = acpi_info_matches_ids(info, acpi_honor_dep_ids);
		kfree(info);

		if (skip)
			continue;

		dep = kzalloc(sizeof(*dep), GFP_KERNEL);
		if (!dep)
			continue;

		count++;

		dep->supplier = dep_devices.handles[i];
		dep->consumer = handle;
		dep->honor_dep = honor_dep;

		mutex_lock(&acpi_dep_list_lock);
		list_add_tail(&dep->node , &acpi_dep_list);
		mutex_unlock(&acpi_dep_list_lock);
	}

	acpi_handle_list_free(&dep_devices);
	return count;
}

static acpi_status acpi_scan_check_crs_csi2_cb(acpi_handle handle, u32 a, void *b, void **c)
{
	acpi_mipi_check_crs_csi2(handle);
	return AE_OK;
}

static acpi_status acpi_bus_check_add(acpi_handle handle, bool first_pass,
				      struct acpi_device **adev_p)
{
	struct acpi_device *device = acpi_fetch_acpi_dev(handle);
	acpi_object_type acpi_type;
	int type;

	if (device)
		goto out;

	if (ACPI_FAILURE(acpi_get_type(handle, &acpi_type)))
		return AE_OK;

	switch (acpi_type) {
	case ACPI_TYPE_DEVICE:
		if (acpi_device_should_be_hidden(handle))
			return AE_OK;

		if (first_pass) {
			acpi_mipi_check_crs_csi2(handle);

			/* Bail out if there are dependencies. */
			if (acpi_scan_check_dep(handle) > 0) {
				/*
				 * The entire CSI-2 connection graph needs to be
				 * extracted before any drivers or scan handlers
				 * are bound to struct device objects, so scan
				 * _CRS CSI-2 resource descriptors for all
				 * devices below the current handle.
				 */
				acpi_walk_namespace(ACPI_TYPE_DEVICE, handle,
						    ACPI_UINT32_MAX,
						    acpi_scan_check_crs_csi2_cb,
						    NULL, NULL, NULL);
				return AE_CTRL_DEPTH;
			}
		}

		fallthrough;
	case ACPI_TYPE_ANY:	/* for ACPI_ROOT_OBJECT */
		type = ACPI_BUS_TYPE_DEVICE;
		break;

	case ACPI_TYPE_PROCESSOR:
		type = ACPI_BUS_TYPE_PROCESSOR;
		break;

	case ACPI_TYPE_THERMAL:
		type = ACPI_BUS_TYPE_THERMAL;
		break;

	case ACPI_TYPE_POWER:
		acpi_add_power_resource(handle);
		fallthrough;
	default:
		return AE_OK;
	}

	/*
	 * If first_pass is true at this point, the device has no dependencies,
	 * or the creation of the device object would have been postponed above.
	 */
	acpi_add_single_object(&device, handle, type, !first_pass);
	if (!device)
		return AE_CTRL_DEPTH;

	acpi_scan_init_hotplug(device);

out:
	if (!*adev_p)
		*adev_p = device;

	return AE_OK;
}

static acpi_status acpi_bus_check_add_1(acpi_handle handle, u32 lvl_not_used,
					void *not_used, void **ret_p)
{
	return acpi_bus_check_add(handle, true, (struct acpi_device **)ret_p);
}

static acpi_status acpi_bus_check_add_2(acpi_handle handle, u32 lvl_not_used,
					void *not_used, void **ret_p)
{
	return acpi_bus_check_add(handle, false, (struct acpi_device **)ret_p);
}

static void acpi_default_enumeration(struct acpi_device *device)
{
	/*
	 * Do not enumerate devices with enumeration_by_parent flag set as
	 * they will be enumerated by their respective parents.
	 */
	if (!device->flags.enumeration_by_parent) {
		acpi_create_platform_device(device, NULL);
		acpi_device_set_enumerated(device);
	} else {
		blocking_notifier_call_chain(&acpi_reconfig_chain,
					     ACPI_RECONFIG_DEVICE_ADD, device);
	}
}

static const struct acpi_device_id generic_device_ids[] = {
	{ACPI_DT_NAMESPACE_HID, },
	{"", },
};

static int acpi_generic_device_attach(struct acpi_device *adev,
				      const struct acpi_device_id *not_used)
{
	/*
	 * Since ACPI_DT_NAMESPACE_HID is the only ID handled here, the test
	 * below can be unconditional.
	 */
	if (adev->data.of_compatible)
		acpi_default_enumeration(adev);

	return 1;
}

static struct acpi_scan_handler generic_device_handler = {
	.ids = generic_device_ids,
	.attach = acpi_generic_device_attach,
};

static int acpi_scan_attach_handler(struct acpi_device *device)
{
	struct acpi_hardware_id *hwid;
	int ret = 0;

	list_for_each_entry(hwid, &device->pnp.ids, list) {
		const struct acpi_device_id *devid;
		struct acpi_scan_handler *handler;

		handler = acpi_scan_match_handler(hwid->id, &devid);
		if (handler) {
			if (!handler->attach) {
				device->pnp.type.platform_id = 0;
				continue;
			}
			device->handler = handler;
			ret = handler->attach(device, devid);
			if (ret > 0)
				break;

			device->handler = NULL;
			if (ret < 0)
				break;
		}
	}

	return ret;
}

static int acpi_bus_attach(struct acpi_device *device, void *first_pass)
{
	bool skip = !first_pass && device->flags.visited;
	acpi_handle ejd;
	int ret;

	if (skip)
		goto ok;

	if (ACPI_SUCCESS(acpi_bus_get_ejd(device->handle, &ejd)))
		register_dock_dependent_device(device, ejd);

	acpi_bus_get_status(device);
	/* Skip devices that are not ready for enumeration (e.g. not present) */
	if (!acpi_dev_ready_for_enumeration(device)) {
		device->flags.initialized = false;
		acpi_device_clear_enumerated(device);
		device->flags.power_manageable = 0;
		return 0;
	}
	if (device->handler)
		goto ok;

	if (!device->flags.initialized) {
		device->flags.power_manageable =
			device->power.states[ACPI_STATE_D0].flags.valid;
		if (acpi_bus_init_power(device))
			device->flags.power_manageable = 0;

		device->flags.initialized = true;
	} else if (device->flags.visited) {
		goto ok;
	}

	ret = acpi_scan_attach_handler(device);
	if (ret < 0)
		return 0;

	device->flags.match_driver = true;
	if (ret > 0 && !device->flags.enumeration_by_parent) {
		acpi_device_set_enumerated(device);
		goto ok;
	}

	ret = device_attach(&device->dev);
	if (ret < 0)
		return 0;

	if (device->pnp.type.platform_id || device->flags.enumeration_by_parent)
		acpi_default_enumeration(device);
	else
		acpi_device_set_enumerated(device);

ok:
	acpi_dev_for_each_child(device, acpi_bus_attach, first_pass);

	if (!skip && device->handler && device->handler->hotplug.notify_online)
		device->handler->hotplug.notify_online(device);

	return 0;
}

static int acpi_dev_get_next_consumer_dev_cb(struct acpi_dep_data *dep, void *data)
{
	struct acpi_device **adev_p = data;
	struct acpi_device *adev = *adev_p;

	/*
	 * If we're passed a 'previous' consumer device then we need to skip
	 * any consumers until we meet the previous one, and then NULL @data
	 * so the next one can be returned.
	 */
	if (adev) {
		if (dep->consumer == adev->handle)
			*adev_p = NULL;

		return 0;
	}

	adev = acpi_get_acpi_dev(dep->consumer);
	if (adev) {
		*(struct acpi_device **)data = adev;
		return 1;
	}
	/* Continue parsing if the device object is not present. */
	return 0;
}

struct acpi_scan_clear_dep_work {
	struct work_struct work;
	struct acpi_device *adev;
};

static void acpi_scan_clear_dep_fn(struct work_struct *work)
{
	struct acpi_scan_clear_dep_work *cdw;

	cdw = container_of(work, struct acpi_scan_clear_dep_work, work);

	acpi_scan_lock_acquire();
	acpi_bus_attach(cdw->adev, (void *)true);
	acpi_scan_lock_release();

	acpi_dev_put(cdw->adev);
	kfree(cdw);
}

static bool acpi_scan_clear_dep_queue(struct acpi_device *adev)
{
	struct acpi_scan_clear_dep_work *cdw;

	if (adev->dep_unmet)
		return false;

	cdw = kmalloc(sizeof(*cdw), GFP_KERNEL);
	if (!cdw)
		return false;

	cdw->adev = adev;
	INIT_WORK(&cdw->work, acpi_scan_clear_dep_fn);
	/*
	 * Since the work function may block on the lock until the entire
	 * initial enumeration of devices is complete, put it into the unbound
	 * workqueue.
	 */
	queue_work(system_unbound_wq, &cdw->work);

	return true;
}

static void acpi_scan_delete_dep_data(struct acpi_dep_data *dep)
{
	list_del(&dep->node);
	kfree(dep);
}

static int acpi_scan_clear_dep(struct acpi_dep_data *dep, void *data)
{
	struct acpi_device *adev = acpi_get_acpi_dev(dep->consumer);

	if (adev) {
		adev->dep_unmet--;
		if (!acpi_scan_clear_dep_queue(adev))
			acpi_dev_put(adev);
	}

	if (dep->free_when_met)
		acpi_scan_delete_dep_data(dep);
	else
		dep->met = true;

	return 0;
}

/**
 * acpi_walk_dep_device_list - Apply a callback to every entry in acpi_dep_list
 * @handle:	The ACPI handle of the supplier device
 * @callback:	Pointer to the callback function to apply
 * @data:	Pointer to some data to pass to the callback
 *
 * The return value of the callback determines this function's behaviour. If 0
 * is returned we continue to iterate over acpi_dep_list. If a positive value
 * is returned then the loop is broken but this function returns 0. If a
 * negative value is returned by the callback then the loop is broken and that
 * value is returned as the final error.
 */
static int acpi_walk_dep_device_list(acpi_handle handle,
				int (*callback)(struct acpi_dep_data *, void *),
				void *data)
{
	struct acpi_dep_data *dep, *tmp;
	int ret = 0;

	mutex_lock(&acpi_dep_list_lock);
	list_for_each_entry_safe(dep, tmp, &acpi_dep_list, node) {
		if (dep->supplier == handle) {
			ret = callback(dep, data);
			if (ret)
				break;
		}
	}
	mutex_unlock(&acpi_dep_list_lock);

	return ret > 0 ? 0 : ret;
}

/**
 * acpi_dev_clear_dependencies - Inform consumers that the device is now active
 * @supplier: Pointer to the supplier &struct acpi_device
 *
 * Clear dependencies on the given device.
 */
void acpi_dev_clear_dependencies(struct acpi_device *supplier)
{
	acpi_walk_dep_device_list(supplier->handle, acpi_scan_clear_dep, NULL);
}
EXPORT_SYMBOL_GPL(acpi_dev_clear_dependencies);

/**
 * acpi_dev_ready_for_enumeration - Check if the ACPI device is ready for enumeration
 * @device: Pointer to the &struct acpi_device to check
 *
 * Check if the device is present and has no unmet dependencies.
 *
 * Return true if the device is ready for enumeratino. Otherwise, return false.
 */
bool acpi_dev_ready_for_enumeration(const struct acpi_device *device)
{
	if (device->flags.honor_deps && device->dep_unmet)
		return false;

	return acpi_device_is_present(device);
}
EXPORT_SYMBOL_GPL(acpi_dev_ready_for_enumeration);

/**
 * acpi_dev_get_next_consumer_dev - Return the next adev dependent on @supplier
 * @supplier: Pointer to the dependee device
 * @start: Pointer to the current dependent device
 *
 * Returns the next &struct acpi_device which declares itself dependent on
 * @supplier via the _DEP buffer, parsed from the acpi_dep_list.
 *
 * If the returned adev is not passed as @start to this function, the caller is
 * responsible for putting the reference to adev when it is no longer needed.
 */
struct acpi_device *acpi_dev_get_next_consumer_dev(struct acpi_device *supplier,
						   struct acpi_device *start)
{
	struct acpi_device *adev = start;

	acpi_walk_dep_device_list(supplier->handle,
				  acpi_dev_get_next_consumer_dev_cb, &adev);

	acpi_dev_put(start);

	if (adev == start)
		return NULL;

	return adev;
}
EXPORT_SYMBOL_GPL(acpi_dev_get_next_consumer_dev);

static void acpi_scan_postponed_branch(acpi_handle handle)
{
	struct acpi_device *adev = NULL;

	if (ACPI_FAILURE(acpi_bus_check_add(handle, false, &adev)))
		return;

	acpi_walk_namespace(ACPI_TYPE_ANY, handle, ACPI_UINT32_MAX,
			    acpi_bus_check_add_2, NULL, NULL, (void **)&adev);

	/*
	 * Populate the ACPI _CRS CSI-2 software nodes for the ACPI devices that
	 * have been added above.
	 */
	acpi_mipi_init_crs_csi2_swnodes();

	acpi_bus_attach(adev, NULL);
}

static void acpi_scan_postponed(void)
{
	struct acpi_dep_data *dep, *tmp;

	mutex_lock(&acpi_dep_list_lock);

	list_for_each_entry_safe(dep, tmp, &acpi_dep_list, node) {
		acpi_handle handle = dep->consumer;

		/*
		 * In case there are multiple acpi_dep_list entries with the
		 * same consumer, skip the current entry if the consumer device
		 * object corresponding to it is present already.
		 */
		if (!acpi_fetch_acpi_dev(handle)) {
			/*
			 * Even though the lock is released here, tmp is
			 * guaranteed to be valid, because none of the list
			 * entries following dep is marked as "free when met"
			 * and so they cannot be deleted.
			 */
			mutex_unlock(&acpi_dep_list_lock);

			acpi_scan_postponed_branch(handle);

			mutex_lock(&acpi_dep_list_lock);
		}

		if (dep->met)
			acpi_scan_delete_dep_data(dep);
		else
			dep->free_when_met = true;
	}

	mutex_unlock(&acpi_dep_list_lock);
}

/**
 * acpi_bus_scan - Add ACPI device node objects in a given namespace scope.
 * @handle: Root of the namespace scope to scan.
 *
 * Scan a given ACPI tree (probably recently hot-plugged) and create and add
 * found devices.
 *
 * If no devices were found, -ENODEV is returned, but it does not mean that
 * there has been a real error.  There just have been no suitable ACPI objects
 * in the table trunk from which the kernel could create a device and add an
 * appropriate driver.
 *
 * Must be called under acpi_scan_lock.
 */
int acpi_bus_scan(acpi_handle handle)
{
	struct acpi_device *device = NULL;

	/* Pass 1: Avoid enumerating devices with missing dependencies. */

	if (ACPI_SUCCESS(acpi_bus_check_add(handle, true, &device)))
		acpi_walk_namespace(ACPI_TYPE_ANY, handle, ACPI_UINT32_MAX,
				    acpi_bus_check_add_1, NULL, NULL,
				    (void **)&device);

	if (!device)
		return -ENODEV;

	/*
	 * Set up ACPI _CRS CSI-2 software nodes using information extracted
	 * from the _CRS CSI-2 resource descriptors during the ACPI namespace
	 * walk above and MIPI DisCo for Imaging device properties.
	 */
	acpi_mipi_scan_crs_csi2();
	acpi_mipi_init_crs_csi2_swnodes();

	acpi_bus_attach(device, (void *)true);

	/* Pass 2: Enumerate all of the remaining devices. */

	acpi_scan_postponed();

	acpi_mipi_crs_csi2_cleanup();

	return 0;
}
EXPORT_SYMBOL(acpi_bus_scan);

/**
 * acpi_bus_trim - Detach scan handlers and drivers from ACPI device objects.
 * @adev: Root of the ACPI namespace scope to walk.
 *
 * Must be called under acpi_scan_lock.
 */
void acpi_bus_trim(struct acpi_device *adev)
{
	acpi_scan_check_and_detach(adev, NULL);
}
EXPORT_SYMBOL_GPL(acpi_bus_trim);

int acpi_bus_register_early_device(int type)
{
	struct acpi_device *device = NULL;
	int result;

	result = acpi_add_single_object(&device, NULL, type, false);
	if (result)
		return result;

	device->flags.match_driver = true;
	return device_attach(&device->dev);
}
EXPORT_SYMBOL_GPL(acpi_bus_register_early_device);

static void acpi_bus_scan_fixed(void)
{
	if (!(acpi_gbl_FADT.flags & ACPI_FADT_POWER_BUTTON)) {
		struct acpi_device *adev = NULL;

		acpi_add_single_object(&adev, NULL, ACPI_BUS_TYPE_POWER_BUTTON,
				       false);
		if (adev) {
			adev->flags.match_driver = true;
			if (device_attach(&adev->dev) >= 0)
				device_init_wakeup(&adev->dev, true);
			else
				dev_dbg(&adev->dev, "No driver\n");
		}
	}

	if (!(acpi_gbl_FADT.flags & ACPI_FADT_SLEEP_BUTTON)) {
		struct acpi_device *adev = NULL;

		acpi_add_single_object(&adev, NULL, ACPI_BUS_TYPE_SLEEP_BUTTON,
				       false);
		if (adev) {
			adev->flags.match_driver = true;
			if (device_attach(&adev->dev) < 0)
				dev_dbg(&adev->dev, "No driver\n");
		}
	}
}

static void __init acpi_get_spcr_uart_addr(void)
{
	acpi_status status;
	struct acpi_table_spcr *spcr_ptr;

	status = acpi_get_table(ACPI_SIG_SPCR, 0,
				(struct acpi_table_header **)&spcr_ptr);
	if (ACPI_FAILURE(status)) {
		pr_warn("STAO table present, but SPCR is missing\n");
		return;
	}

	spcr_uart_addr = spcr_ptr->serial_port.address;
	acpi_put_table((struct acpi_table_header *)spcr_ptr);
}

static bool acpi_scan_initialized;

void __init acpi_scan_init(void)
{
	acpi_status status;
	struct acpi_table_stao *stao_ptr;

	acpi_pci_root_init();
	acpi_pci_link_init();
	acpi_processor_init();
	acpi_platform_init();
	acpi_lpss_init();
	acpi_apd_init();
	acpi_cmos_rtc_init();
	acpi_container_init();
	acpi_memory_hotplug_init();
	acpi_watchdog_init();
	acpi_pnp_init();
	acpi_int340x_thermal_init();
	acpi_init_lpit();

	acpi_scan_add_handler(&generic_device_handler);

	/*
	 * If there is STAO table, check whether it needs to ignore the UART
	 * device in SPCR table.
	 */
	status = acpi_get_table(ACPI_SIG_STAO, 0,
				(struct acpi_table_header **)&stao_ptr);
	if (ACPI_SUCCESS(status)) {
		if (stao_ptr->header.length > sizeof(struct acpi_table_stao))
			pr_info("STAO Name List not yet supported.\n");

		if (stao_ptr->ignore_uart)
			acpi_get_spcr_uart_addr();

		acpi_put_table((struct acpi_table_header *)stao_ptr);
	}

	acpi_gpe_apply_masked_gpes();
	acpi_update_all_gpes();

	/*
	 * Although we call __add_memory() that is documented to require the
	 * device_hotplug_lock, it is not necessary here because this is an
	 * early code when userspace or any other code path cannot trigger
	 * hotplug/hotunplug operations.
	 */
	mutex_lock(&acpi_scan_lock);
	/*
	 * Enumerate devices in the ACPI namespace.
	 */
	if (acpi_bus_scan(ACPI_ROOT_OBJECT))
		goto unlock;

	acpi_root = acpi_fetch_acpi_dev(ACPI_ROOT_OBJECT);
	if (!acpi_root)
		goto unlock;

	/* Fixed feature devices do not exist on HW-reduced platform */
	if (!acpi_gbl_reduced_hardware)
		acpi_bus_scan_fixed();

	acpi_turn_off_unused_power_resources();

	acpi_scan_initialized = true;

unlock:
	mutex_unlock(&acpi_scan_lock);
}

static struct acpi_probe_entry *ape;
static int acpi_probe_count;
static DEFINE_MUTEX(acpi_probe_mutex);

static int __init acpi_match_madt(union acpi_subtable_headers *header,
				  const unsigned long end)
{
	if (!ape->subtable_valid || ape->subtable_valid(&header->common, ape))
		if (!ape->probe_subtbl(header, end))
			acpi_probe_count++;

	return 0;
}

int __init __acpi_probe_device_table(struct acpi_probe_entry *ap_head, int nr)
{
	int count = 0;

	if (acpi_disabled)
		return 0;

	mutex_lock(&acpi_probe_mutex);
	for (ape = ap_head; nr; ape++, nr--) {
		if (ACPI_COMPARE_NAMESEG(ACPI_SIG_MADT, ape->id)) {
			acpi_probe_count = 0;
			acpi_table_parse_madt(ape->type, acpi_match_madt, 0);
			count += acpi_probe_count;
		} else {
			int res;
			res = acpi_table_parse(ape->id, ape->probe_table);
			if (!res)
				count++;
		}
	}
	mutex_unlock(&acpi_probe_mutex);

	return count;
}

static void acpi_table_events_fn(struct work_struct *work)
{
	acpi_scan_lock_acquire();
	acpi_bus_scan(ACPI_ROOT_OBJECT);
	acpi_scan_lock_release();

	kfree(work);
}

void acpi_scan_table_notify(void)
{
	struct work_struct *work;

	if (!acpi_scan_initialized)
		return;

	work = kmalloc(sizeof(*work), GFP_KERNEL);
	if (!work)
		return;

	INIT_WORK(work, acpi_table_events_fn);
	schedule_work(work);
}

int acpi_reconfig_notifier_register(struct notifier_block *nb)
{
	return blocking_notifier_chain_register(&acpi_reconfig_chain, nb);
}
EXPORT_SYMBOL(acpi_reconfig_notifier_register);

int acpi_reconfig_notifier_unregister(struct notifier_block *nb)
{
	return blocking_notifier_chain_unregister(&acpi_reconfig_chain, nb);
}
EXPORT_SYMBOL(acpi_reconfig_notifier_unregister);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
