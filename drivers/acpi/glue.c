<<<<<<< HEAD
=======
// SPDX-License-Identifier: GPL-2.0-only
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
/*
 * Link physical devices with ACPI devices support
 *
 * Copyright (c) 2005 David Shaohua Li <shaohua.li@intel.com>
 * Copyright (c) 2005 Intel Corp.
<<<<<<< HEAD
 *
 * This file is released under the GPLv2.
 */
=======
 */

#define pr_fmt(fmt) "ACPI: " fmt

#include <linux/acpi_iort.h>
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#include <linux/export.h>
#include <linux/init.h>
#include <linux/list.h>
#include <linux/device.h>
#include <linux/slab.h>
#include <linux/rwsem.h>
#include <linux/acpi.h>
<<<<<<< HEAD

#include "internal.h"

#define ACPI_GLUE_DEBUG	0
#if ACPI_GLUE_DEBUG
#define DBG(x...) printk(PREFIX x)
#else
#define DBG(x...) do { } while(0)
#endif
static LIST_HEAD(bus_type_list);
static DECLARE_RWSEM(bus_type_sem);

=======
#include <linux/dma-mapping.h>
#include <linux/pci.h>
#include <linux/pci-acpi.h>
#include <linux/platform_device.h>

#include "internal.h"

static LIST_HEAD(bus_type_list);
static DECLARE_RWSEM(bus_type_sem);

#define PHYSICAL_NODE_STRING "physical_node"
#define PHYSICAL_NODE_NAME_SIZE (sizeof(PHYSICAL_NODE_STRING) + 10)

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
int register_acpi_bus_type(struct acpi_bus_type *type)
{
	if (acpi_disabled)
		return -ENODEV;
<<<<<<< HEAD
	if (type && type->bus && type->find_device) {
		down_write(&bus_type_sem);
		list_add_tail(&type->list, &bus_type_list);
		up_write(&bus_type_sem);
		printk(KERN_INFO PREFIX "bus type %s registered\n",
		       type->bus->name);
=======
	if (type && type->match && type->find_companion) {
		down_write(&bus_type_sem);
		list_add_tail(&type->list, &bus_type_list);
		up_write(&bus_type_sem);
		pr_info("bus type %s registered\n", type->name);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		return 0;
	}
	return -ENODEV;
}
<<<<<<< HEAD
=======
EXPORT_SYMBOL_GPL(register_acpi_bus_type);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

int unregister_acpi_bus_type(struct acpi_bus_type *type)
{
	if (acpi_disabled)
		return 0;
	if (type) {
		down_write(&bus_type_sem);
		list_del_init(&type->list);
		up_write(&bus_type_sem);
<<<<<<< HEAD
		printk(KERN_INFO PREFIX "ACPI bus type %s unregistered\n",
		       type->bus->name);
=======
		pr_info("bus type %s unregistered\n", type->name);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		return 0;
	}
	return -ENODEV;
}
<<<<<<< HEAD

static struct acpi_bus_type *acpi_get_bus_type(struct bus_type *type)
=======
EXPORT_SYMBOL_GPL(unregister_acpi_bus_type);

static struct acpi_bus_type *acpi_get_bus_type(struct device *dev)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
{
	struct acpi_bus_type *tmp, *ret = NULL;

	down_read(&bus_type_sem);
	list_for_each_entry(tmp, &bus_type_list, list) {
<<<<<<< HEAD
		if (tmp->bus == type) {
=======
		if (tmp->match(dev)) {
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
			ret = tmp;
			break;
		}
	}
	up_read(&bus_type_sem);
	return ret;
}

<<<<<<< HEAD
static int acpi_find_bridge_device(struct device *dev, acpi_handle * handle)
{
	struct acpi_bus_type *tmp;
	int ret = -ENODEV;

	down_read(&bus_type_sem);
	list_for_each_entry(tmp, &bus_type_list, list) {
		if (tmp->find_bridge && !tmp->find_bridge(dev, handle)) {
			ret = 0;
			break;
		}
	}
	up_read(&bus_type_sem);
	return ret;
}

/* Get device's handler per its address under its parent */
struct acpi_find_child {
	acpi_handle handle;
	u64 address;
};

static acpi_status
do_acpi_find_child(acpi_handle handle, u32 lvl, void *context, void **rv)
{
	acpi_status status;
	struct acpi_device_info *info;
	struct acpi_find_child *find = context;

	status = acpi_get_object_info(handle, &info);
	if (ACPI_SUCCESS(status)) {
		if ((info->address == find->address)
			&& (info->valid & ACPI_VALID_ADR))
			find->handle = handle;
		kfree(info);
	}
	return AE_OK;
}

acpi_handle acpi_get_child(acpi_handle parent, u64 address)
{
	struct acpi_find_child find = { NULL, address };

	if (!parent)
		return NULL;
	acpi_walk_namespace(ACPI_TYPE_DEVICE, parent,
			    1, do_acpi_find_child, NULL, &find, NULL);
	return find.handle;
}

EXPORT_SYMBOL(acpi_get_child);

/* Link ACPI devices with physical devices */
static void acpi_glue_data_handler(acpi_handle handle,
				   void *context)
{
	/* we provide an empty handler */
}

/* Note: a success call will increase reference count by one */
struct device *acpi_get_physical_device(acpi_handle handle)
{
	acpi_status status;
	struct device *dev;

	status = acpi_get_data(handle, acpi_glue_data_handler, (void **)&dev);
	if (ACPI_SUCCESS(status))
		return get_device(dev);
	return NULL;
}

EXPORT_SYMBOL(acpi_get_physical_device);

static int acpi_bind_one(struct device *dev, acpi_handle handle)
{
	struct acpi_device *acpi_dev;
	acpi_status status;

	if (dev->archdata.acpi_handle) {
		dev_warn(dev, "Drivers changed 'acpi_handle'\n");
		return -EINVAL;
	}
	get_device(dev);
	status = acpi_attach_data(handle, acpi_glue_data_handler, dev);
	if (ACPI_FAILURE(status)) {
		put_device(dev);
		return -EINVAL;
	}
	dev->archdata.acpi_handle = handle;

	status = acpi_bus_get_device(handle, &acpi_dev);
	if (!ACPI_FAILURE(status)) {
		int ret;

		ret = sysfs_create_link(&dev->kobj, &acpi_dev->dev.kobj,
				"firmware_node");
		ret = sysfs_create_link(&acpi_dev->dev.kobj, &dev->kobj,
				"physical_node");
		if (acpi_dev->wakeup.flags.valid)
			device_set_wakeup_capable(dev, true);
	}

	return 0;
}

static int acpi_unbind_one(struct device *dev)
{
	if (!dev->archdata.acpi_handle)
		return 0;
	if (dev == acpi_get_physical_device(dev->archdata.acpi_handle)) {
		struct acpi_device *acpi_dev;

		/* acpi_get_physical_device increase refcnt by one */
		put_device(dev);

		if (!acpi_bus_get_device(dev->archdata.acpi_handle,
					&acpi_dev)) {
			sysfs_remove_link(&dev->kobj, "firmware_node");
			sysfs_remove_link(&acpi_dev->dev.kobj, "physical_node");
		}

		acpi_detach_data(dev->archdata.acpi_handle,
				 acpi_glue_data_handler);
		dev->archdata.acpi_handle = NULL;
		/* acpi_bind_one increase refcnt by one */
		put_device(dev);
	} else {
		dev_err(dev, "Oops, 'acpi_handle' corrupt\n");
	}
	return 0;
}

static int acpi_platform_notify(struct device *dev)
{
	struct acpi_bus_type *type;
	acpi_handle handle;
	int ret = -EINVAL;

	if (!dev->bus || !dev->parent) {
		/* bridge devices genernally haven't bus or parent */
		ret = acpi_find_bridge_device(dev, &handle);
		goto end;
	}
	type = acpi_get_bus_type(dev->bus);
	if (!type) {
		DBG("No ACPI bus support for %s\n", dev_name(dev));
		ret = -EINVAL;
		goto end;
	}
	if ((ret = type->find_device(dev, &handle)) != 0)
		DBG("Can't get handler for %s\n", dev_name(dev));
      end:
	if (!ret)
		acpi_bind_one(dev, handle);

#if ACPI_GLUE_DEBUG
	if (!ret) {
		struct acpi_buffer buffer = { ACPI_ALLOCATE_BUFFER, NULL };

		acpi_get_name(dev->archdata.acpi_handle,
			      ACPI_FULL_PATHNAME, &buffer);
		DBG("Device %s -> %s\n", dev_name(dev), (char *)buffer.pointer);
		kfree(buffer.pointer);
	} else
		DBG("Device %s -> No ACPI support\n", dev_name(dev));
#endif

	return ret;
}

static int acpi_platform_notify_remove(struct device *dev)
{
	acpi_unbind_one(dev);
	return 0;
}

int __init init_acpi_device_notify(void)
{
	if (platform_notify || platform_notify_remove) {
		printk(KERN_ERR PREFIX "Can't use platform_notify\n");
		return 0;
	}
	platform_notify = acpi_platform_notify;
	platform_notify_remove = acpi_platform_notify_remove;
	return 0;
=======
#define FIND_CHILD_MIN_SCORE	1
#define FIND_CHILD_MID_SCORE	2
#define FIND_CHILD_MAX_SCORE	3

static int match_any(struct acpi_device *adev, void *not_used)
{
	return 1;
}

static bool acpi_dev_has_children(struct acpi_device *adev)
{
	return acpi_dev_for_each_child(adev, match_any, NULL) > 0;
}

static int find_child_checks(struct acpi_device *adev, bool check_children)
{
	unsigned long long sta;
	acpi_status status;

	if (check_children && !acpi_dev_has_children(adev))
		return -ENODEV;

	status = acpi_evaluate_integer(adev->handle, "_STA", NULL, &sta);
	if (status == AE_NOT_FOUND) {
		/*
		 * Special case: backlight device objects without _STA are
		 * preferred to other objects with the same _ADR value, because
		 * it is more likely that they are actually useful.
		 */
		if (adev->pnp.type.backlight)
			return FIND_CHILD_MID_SCORE;

		return FIND_CHILD_MIN_SCORE;
	}

	if (ACPI_FAILURE(status) || !(sta & ACPI_STA_DEVICE_ENABLED))
		return -ENODEV;

	/*
	 * If the device has a _HID returning a valid ACPI/PNP device ID, it is
	 * better to make it look less attractive here, so that the other device
	 * with the same _ADR value (that may not have a valid device ID) can be
	 * matched going forward.  [This means a second spec violation in a row,
	 * so whatever we do here is best effort anyway.]
	 */
	if (adev->pnp.type.platform_id)
		return FIND_CHILD_MIN_SCORE;

	return FIND_CHILD_MAX_SCORE;
}

struct find_child_walk_data {
	struct acpi_device *adev;
	u64 address;
	int score;
	bool check_sta;
	bool check_children;
};

static int check_one_child(struct acpi_device *adev, void *data)
{
	struct find_child_walk_data *wd = data;
	int score;

	if (!adev->pnp.type.bus_address || acpi_device_adr(adev) != wd->address)
		return 0;

	if (!wd->adev) {
		/*
		 * This is the first matching object, so save it.  If it is not
		 * necessary to look for any other matching objects, stop the
		 * search.
		 */
		wd->adev = adev;
		return !(wd->check_sta || wd->check_children);
	}

	/*
	 * There is more than one matching device object with the same _ADR
	 * value.  That really is unexpected, so we are kind of beyond the scope
	 * of the spec here.  We have to choose which one to return, though.
	 *
	 * First, get the score for the previously found object and terminate
	 * the walk if it is maximum.
	*/
	if (!wd->score) {
		score = find_child_checks(wd->adev, wd->check_children);
		if (score == FIND_CHILD_MAX_SCORE)
			return 1;

		wd->score = score;
	}
	/*
	 * Second, if the object that has just been found has a better score,
	 * replace the previously found one with it and terminate the walk if
	 * the new score is maximum.
	 */
	score = find_child_checks(adev, wd->check_children);
	if (score > wd->score) {
		wd->adev = adev;
		if (score == FIND_CHILD_MAX_SCORE)
			return 1;

		wd->score = score;
	}

	/* Continue, because there may be better matches. */
	return 0;
}

static struct acpi_device *acpi_find_child(struct acpi_device *parent,
					   u64 address, bool check_children,
					   bool check_sta)
{
	struct find_child_walk_data wd = {
		.address = address,
		.check_children = check_children,
		.check_sta = check_sta,
		.adev = NULL,
		.score = 0,
	};

	if (parent)
		acpi_dev_for_each_child(parent, check_one_child, &wd);

	return wd.adev;
}

struct acpi_device *acpi_find_child_device(struct acpi_device *parent,
					   u64 address, bool check_children)
{
	return acpi_find_child(parent, address, check_children, true);
}
EXPORT_SYMBOL_GPL(acpi_find_child_device);

struct acpi_device *acpi_find_child_by_adr(struct acpi_device *adev,
					   acpi_bus_address adr)
{
	return acpi_find_child(adev, adr, false, false);
}
EXPORT_SYMBOL_GPL(acpi_find_child_by_adr);

static void acpi_physnode_link_name(char *buf, unsigned int node_id)
{
	if (node_id > 0)
		snprintf(buf, PHYSICAL_NODE_NAME_SIZE,
			 PHYSICAL_NODE_STRING "%u", node_id);
	else
		strcpy(buf, PHYSICAL_NODE_STRING);
}

int acpi_bind_one(struct device *dev, struct acpi_device *acpi_dev)
{
	struct acpi_device_physical_node *physical_node, *pn;
	char physical_node_name[PHYSICAL_NODE_NAME_SIZE];
	struct list_head *physnode_list;
	unsigned int node_id;
	int retval = -EINVAL;

	if (has_acpi_companion(dev)) {
		if (acpi_dev) {
			dev_warn(dev, "ACPI companion already set\n");
			return -EINVAL;
		} else {
			acpi_dev = ACPI_COMPANION(dev);
		}
	}
	if (!acpi_dev)
		return -EINVAL;

	acpi_dev_get(acpi_dev);
	get_device(dev);
	physical_node = kzalloc(sizeof(*physical_node), GFP_KERNEL);
	if (!physical_node) {
		retval = -ENOMEM;
		goto err;
	}

	mutex_lock(&acpi_dev->physical_node_lock);

	/*
	 * Keep the list sorted by node_id so that the IDs of removed nodes can
	 * be recycled easily.
	 */
	physnode_list = &acpi_dev->physical_node_list;
	node_id = 0;
	list_for_each_entry(pn, &acpi_dev->physical_node_list, node) {
		/* Sanity check. */
		if (pn->dev == dev) {
			mutex_unlock(&acpi_dev->physical_node_lock);

			dev_warn(dev, "Already associated with ACPI node\n");
			kfree(physical_node);
			if (ACPI_COMPANION(dev) != acpi_dev)
				goto err;

			put_device(dev);
			acpi_dev_put(acpi_dev);
			return 0;
		}
		if (pn->node_id == node_id) {
			physnode_list = &pn->node;
			node_id++;
		}
	}

	physical_node->node_id = node_id;
	physical_node->dev = dev;
	list_add(&physical_node->node, physnode_list);
	acpi_dev->physical_node_count++;

	if (!has_acpi_companion(dev))
		ACPI_COMPANION_SET(dev, acpi_dev);

	acpi_physnode_link_name(physical_node_name, node_id);
	retval = sysfs_create_link(&acpi_dev->dev.kobj, &dev->kobj,
				   physical_node_name);
	if (retval)
		dev_err(&acpi_dev->dev, "Failed to create link %s (%d)\n",
			physical_node_name, retval);

	retval = sysfs_create_link(&dev->kobj, &acpi_dev->dev.kobj,
				   "firmware_node");
	if (retval)
		dev_err(dev, "Failed to create link firmware_node (%d)\n",
			retval);

	mutex_unlock(&acpi_dev->physical_node_lock);

	if (acpi_dev->wakeup.flags.valid)
		device_set_wakeup_capable(dev, true);

	return 0;

 err:
	ACPI_COMPANION_SET(dev, NULL);
	put_device(dev);
	acpi_dev_put(acpi_dev);
	return retval;
}
EXPORT_SYMBOL_GPL(acpi_bind_one);

int acpi_unbind_one(struct device *dev)
{
	struct acpi_device *acpi_dev = ACPI_COMPANION(dev);
	struct acpi_device_physical_node *entry;

	if (!acpi_dev)
		return 0;

	mutex_lock(&acpi_dev->physical_node_lock);

	list_for_each_entry(entry, &acpi_dev->physical_node_list, node)
		if (entry->dev == dev) {
			char physnode_name[PHYSICAL_NODE_NAME_SIZE];

			list_del(&entry->node);
			acpi_dev->physical_node_count--;

			acpi_physnode_link_name(physnode_name, entry->node_id);
			sysfs_remove_link(&acpi_dev->dev.kobj, physnode_name);
			sysfs_remove_link(&dev->kobj, "firmware_node");
			ACPI_COMPANION_SET(dev, NULL);
			/* Drop references taken by acpi_bind_one(). */
			put_device(dev);
			acpi_dev_put(acpi_dev);
			kfree(entry);
			break;
		}

	mutex_unlock(&acpi_dev->physical_node_lock);
	return 0;
}
EXPORT_SYMBOL_GPL(acpi_unbind_one);

void acpi_device_notify(struct device *dev)
{
	struct acpi_device *adev;
	int ret;

	ret = acpi_bind_one(dev, NULL);
	if (ret) {
		struct acpi_bus_type *type = acpi_get_bus_type(dev);

		if (!type)
			goto err;

		adev = type->find_companion(dev);
		if (!adev) {
			dev_dbg(dev, "ACPI companion not found\n");
			goto err;
		}
		ret = acpi_bind_one(dev, adev);
		if (ret)
			goto err;

		if (type->setup) {
			type->setup(dev);
			goto done;
		}
	} else {
		adev = ACPI_COMPANION(dev);

		if (dev_is_pci(dev)) {
			pci_acpi_setup(dev, adev);
			goto done;
		} else if (dev_is_platform(dev)) {
			acpi_configure_pmsi_domain(dev);
		}
	}

	if (adev->handler && adev->handler->bind)
		adev->handler->bind(dev);

done:
	acpi_handle_debug(ACPI_HANDLE(dev), "Bound to device %s\n",
			  dev_name(dev));

	return;

err:
	dev_dbg(dev, "No ACPI support\n");
}

void acpi_device_notify_remove(struct device *dev)
{
	struct acpi_device *adev = ACPI_COMPANION(dev);

	if (!adev)
		return;

	if (dev_is_pci(dev))
		pci_acpi_cleanup(dev, adev);
	else if (adev->handler && adev->handler->unbind)
		adev->handler->unbind(dev);

	acpi_unbind_one(dev);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}
