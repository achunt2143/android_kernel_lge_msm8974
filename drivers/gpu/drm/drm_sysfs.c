<<<<<<< HEAD
=======
// SPDX-License-Identifier: GPL-2.0-only
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

/*
 * drm_sysfs.c - Modifications to drm_sysfs_class.c to support
 *               extra sysfs attribute from DRM. Normal drm_sysfs_class
 *               does not allow adding attributes.
 *
 * Copyright (c) 2004 Jon Smirl <jonsmirl@gmail.com>
 * Copyright (c) 2003-2004 Greg Kroah-Hartman <greg@kroah.com>
 * Copyright (c) 2003-2004 IBM Corp.
<<<<<<< HEAD
 *
 * This file is released under the GPLv2
 *
 */

#include <linux/device.h>
#include <linux/kdev_t.h>
#include <linux/gfp.h>
#include <linux/err.h>
#include <linux/export.h>

#include "drm_sysfs.h"
#include "drm_core.h"
#include "drmP.h"

#define to_drm_minor(d) container_of(d, struct drm_minor, kdev)
#define to_drm_connector(d) container_of(d, struct drm_connector, kdev)
=======
 */

#include <linux/acpi.h>
#include <linux/component.h>
#include <linux/device.h>
#include <linux/err.h>
#include <linux/export.h>
#include <linux/gfp.h>
#include <linux/i2c.h>
#include <linux/kdev_t.h>
#include <linux/property.h>
#include <linux/slab.h>

#include <drm/drm_accel.h>
#include <drm/drm_connector.h>
#include <drm/drm_device.h>
#include <drm/drm_file.h>
#include <drm/drm_modes.h>
#include <drm/drm_print.h>
#include <drm/drm_property.h>
#include <drm/drm_sysfs.h>

#include "drm_internal.h"
#include "drm_crtc_internal.h"

#define to_drm_minor(d) dev_get_drvdata(d)
#define to_drm_connector(d) dev_get_drvdata(d)

/**
 * DOC: overview
 *
 * DRM provides very little additional support to drivers for sysfs
 * interactions, beyond just all the standard stuff. Drivers who want to expose
 * additional sysfs properties and property groups can attach them at either
 * &drm_device.dev or &drm_connector.kdev.
 *
 * Registration is automatically handled when calling drm_dev_register(), or
 * drm_connector_register() in case of hot-plugged connectors. Unregistration is
 * also automatically handled by drm_dev_unregister() and
 * drm_connector_unregister().
 */
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

static struct device_type drm_sysfs_device_minor = {
	.name = "drm_minor"
};

<<<<<<< HEAD
/**
 * drm_class_suspend - DRM class suspend hook
 * @dev: Linux device to suspend
 * @state: power state to enter
 *
 * Just figures out what the actual struct drm_device associated with
 * @dev is and calls its suspend hook, if present.
 */
static int drm_class_suspend(struct device *dev, pm_message_t state)
{
	if (dev->type == &drm_sysfs_device_minor) {
		struct drm_minor *drm_minor = to_drm_minor(dev);
		struct drm_device *drm_dev = drm_minor->dev;

		if (drm_minor->type == DRM_MINOR_LEGACY &&
		    !drm_core_check_feature(drm_dev, DRIVER_MODESET) &&
		    drm_dev->driver->suspend)
			return drm_dev->driver->suspend(drm_dev, state);
	}
	return 0;
}

/**
 * drm_class_resume - DRM class resume hook
 * @dev: Linux device to resume
 *
 * Just figures out what the actual struct drm_device associated with
 * @dev is and calls its resume hook, if present.
 */
static int drm_class_resume(struct device *dev)
{
	if (dev->type == &drm_sysfs_device_minor) {
		struct drm_minor *drm_minor = to_drm_minor(dev);
		struct drm_device *drm_dev = drm_minor->dev;

		if (drm_minor->type == DRM_MINOR_LEGACY &&
		    !drm_core_check_feature(drm_dev, DRIVER_MODESET) &&
		    drm_dev->driver->resume)
			return drm_dev->driver->resume(drm_dev);
	}
	return 0;
}

static char *drm_devnode(struct device *dev, umode_t *mode)
=======
static struct device_type drm_sysfs_device_connector = {
	.name = "drm_connector",
};

struct class *drm_class;

#ifdef CONFIG_ACPI
static bool drm_connector_acpi_bus_match(struct device *dev)
{
	return dev->type == &drm_sysfs_device_connector;
}

static struct acpi_device *drm_connector_acpi_find_companion(struct device *dev)
{
	struct drm_connector *connector = to_drm_connector(dev);

	return to_acpi_device_node(connector->fwnode);
}

static struct acpi_bus_type drm_connector_acpi_bus = {
	.name = "drm_connector",
	.match = drm_connector_acpi_bus_match,
	.find_companion = drm_connector_acpi_find_companion,
};

static void drm_sysfs_acpi_register(void)
{
	register_acpi_bus_type(&drm_connector_acpi_bus);
}

static void drm_sysfs_acpi_unregister(void)
{
	unregister_acpi_bus_type(&drm_connector_acpi_bus);
}
#else
static void drm_sysfs_acpi_register(void) { }
static void drm_sysfs_acpi_unregister(void) { }
#endif

static char *drm_devnode(const struct device *dev, umode_t *mode)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
{
	return kasprintf(GFP_KERNEL, "dri/%s", dev_name(dev));
}

<<<<<<< HEAD
static CLASS_ATTR_STRING(version, S_IRUGO,
		CORE_NAME " "
		__stringify(CORE_MAJOR) "."
		__stringify(CORE_MINOR) "."
		__stringify(CORE_PATCHLEVEL) " "
		CORE_DATE);

/**
 * drm_sysfs_create - create a struct drm_sysfs_class structure
 * @owner: pointer to the module that is to "own" this struct drm_sysfs_class
 * @name: pointer to a string for the name of this class.
 *
 * This is used to create DRM class pointer that can then be used
 * in calls to drm_sysfs_device_add().
 *
 * Note, the pointer created here is to be destroyed when finished by making a
 * call to drm_sysfs_destroy().
 */
struct class *drm_sysfs_create(struct module *owner, char *name)
{
	struct class *class;
	int err;

	class = class_create(owner, name);
	if (IS_ERR(class)) {
		err = PTR_ERR(class);
		goto err_out;
	}

	class->suspend = drm_class_suspend;
	class->resume = drm_class_resume;

	err = class_create_file(class, &class_attr_version.attr);
	if (err)
		goto err_out_class;

	class->devnode = drm_devnode;

	return class;

err_out_class:
	class_destroy(class);
err_out:
	return ERR_PTR(err);
=======
static int typec_connector_bind(struct device *dev,
				struct device *typec_connector, void *data)
{
	int ret;

	ret = sysfs_create_link(&dev->kobj, &typec_connector->kobj, "typec_connector");
	if (ret)
		return ret;

	ret = sysfs_create_link(&typec_connector->kobj, &dev->kobj, "drm_connector");
	if (ret)
		sysfs_remove_link(&dev->kobj, "typec_connector");

	return ret;
}

static void typec_connector_unbind(struct device *dev,
				   struct device *typec_connector, void *data)
{
	sysfs_remove_link(&typec_connector->kobj, "drm_connector");
	sysfs_remove_link(&dev->kobj, "typec_connector");
}

static const struct component_ops typec_connector_ops = {
	.bind = typec_connector_bind,
	.unbind = typec_connector_unbind,
};

static CLASS_ATTR_STRING(version, S_IRUGO, "drm 1.1.0 20060810");

/**
 * drm_sysfs_init - initialize sysfs helpers
 *
 * This is used to create the DRM class, which is the implicit parent of any
 * other top-level DRM sysfs objects.
 *
 * You must call drm_sysfs_destroy() to release the allocated resources.
 *
 * Return: 0 on success, negative error code on failure.
 */
int drm_sysfs_init(void)
{
	int err;

	drm_class = class_create("drm");
	if (IS_ERR(drm_class))
		return PTR_ERR(drm_class);

	err = class_create_file(drm_class, &class_attr_version.attr);
	if (err) {
		class_destroy(drm_class);
		drm_class = NULL;
		return err;
	}

	drm_class->devnode = drm_devnode;

	drm_sysfs_acpi_register();
	return 0;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

/**
 * drm_sysfs_destroy - destroys DRM class
 *
 * Destroy the DRM device class.
 */
void drm_sysfs_destroy(void)
{
<<<<<<< HEAD
	if ((drm_class == NULL) || (IS_ERR(drm_class)))
		return;
	class_remove_file(drm_class, &class_attr_version.attr);
	class_destroy(drm_class);
}

/**
 * drm_sysfs_device_release - do nothing
 * @dev: Linux device
 *
 * Normally, this would free the DRM device associated with @dev, along
 * with cleaning up any other stuff.  But we do that in the DRM core, so
 * this function can just return and hope that the core does its job.
 */
static void drm_sysfs_device_release(struct device *dev)
{
	memset(dev, 0, sizeof(struct device));
	return;
=======
	if (IS_ERR_OR_NULL(drm_class))
		return;
	drm_sysfs_acpi_unregister();
	class_remove_file(drm_class, &class_attr_version.attr);
	class_destroy(drm_class);
	drm_class = NULL;
}

static void drm_sysfs_release(struct device *dev)
{
	kfree(dev);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

/*
 * Connector properties
 */
<<<<<<< HEAD
=======
static ssize_t status_store(struct device *device,
			   struct device_attribute *attr,
			   const char *buf, size_t count)
{
	struct drm_connector *connector = to_drm_connector(device);
	struct drm_device *dev = connector->dev;
	enum drm_connector_force old_force;
	int ret;

	ret = mutex_lock_interruptible(&dev->mode_config.mutex);
	if (ret)
		return ret;

	old_force = connector->force;

	if (sysfs_streq(buf, "detect"))
		connector->force = 0;
	else if (sysfs_streq(buf, "on"))
		connector->force = DRM_FORCE_ON;
	else if (sysfs_streq(buf, "on-digital"))
		connector->force = DRM_FORCE_ON_DIGITAL;
	else if (sysfs_streq(buf, "off"))
		connector->force = DRM_FORCE_OFF;
	else
		ret = -EINVAL;

	if (old_force != connector->force || !connector->force) {
		DRM_DEBUG_KMS("[CONNECTOR:%d:%s] force updated from %d to %d or reprobing\n",
			      connector->base.id,
			      connector->name,
			      old_force, connector->force);

		connector->funcs->fill_modes(connector,
					     dev->mode_config.max_width,
					     dev->mode_config.max_height);
	}

	mutex_unlock(&dev->mode_config.mutex);

	return ret ? ret : count;
}

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
static ssize_t status_show(struct device *device,
			   struct device_attribute *attr,
			   char *buf)
{
	struct drm_connector *connector = to_drm_connector(device);
	enum drm_connector_status status;
<<<<<<< HEAD
	int ret;

	ret = mutex_lock_interruptible(&connector->dev->mode_config.mutex);
	if (ret)
		return ret;

	status = connector->funcs->detect(connector, true);
	mutex_unlock(&connector->dev->mode_config.mutex);

	return snprintf(buf, PAGE_SIZE, "%s\n",
			drm_get_connector_status_name(status));
=======

	status = READ_ONCE(connector->status);

	return sysfs_emit(buf, "%s\n",
			  drm_get_connector_status_name(status));
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

static ssize_t dpms_show(struct device *device,
			   struct device_attribute *attr,
			   char *buf)
{
	struct drm_connector *connector = to_drm_connector(device);
<<<<<<< HEAD
	struct drm_device *dev = connector->dev;
	uint64_t dpms_status;
	int ret;

	ret = drm_connector_property_get_value(connector,
					    dev->mode_config.dpms_property,
					    &dpms_status);
	if (ret)
		return 0;

	return snprintf(buf, PAGE_SIZE, "%s\n",
			drm_get_dpms_name((int)dpms_status));
=======
	int dpms;

	dpms = READ_ONCE(connector->dpms);

	return sysfs_emit(buf, "%s\n", drm_get_dpms_name(dpms));
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

static ssize_t enabled_show(struct device *device,
			    struct device_attribute *attr,
			   char *buf)
{
	struct drm_connector *connector = to_drm_connector(device);
<<<<<<< HEAD

	return snprintf(buf, PAGE_SIZE, "%s\n", connector->encoder ? "enabled" :
			"disabled");
=======
	bool enabled;

	enabled = READ_ONCE(connector->encoder);

	return sysfs_emit(buf, enabled ? "enabled\n" : "disabled\n");
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

static ssize_t edid_show(struct file *filp, struct kobject *kobj,
			 struct bin_attribute *attr, char *buf, loff_t off,
			 size_t count)
{
<<<<<<< HEAD
	struct device *connector_dev = container_of(kobj, struct device, kobj);
	struct drm_connector *connector = to_drm_connector(connector_dev);
	unsigned char *edid;
	size_t size;

	if (!connector->edid_blob_ptr)
		return 0;
=======
	struct device *connector_dev = kobj_to_dev(kobj);
	struct drm_connector *connector = to_drm_connector(connector_dev);
	unsigned char *edid;
	size_t size;
	ssize_t ret = 0;

	mutex_lock(&connector->dev->mode_config.mutex);
	if (!connector->edid_blob_ptr)
		goto unlock;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	edid = connector->edid_blob_ptr->data;
	size = connector->edid_blob_ptr->length;
	if (!edid)
<<<<<<< HEAD
		return 0;

	if (off >= size)
		return 0;
=======
		goto unlock;

	if (off >= size)
		goto unlock;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	if (off + count > size)
		count = size - off;
	memcpy(buf, edid + off, count);

<<<<<<< HEAD
	return count;
=======
	ret = count;
unlock:
	mutex_unlock(&connector->dev->mode_config.mutex);

	return ret;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

static ssize_t modes_show(struct device *device,
			   struct device_attribute *attr,
			   char *buf)
{
	struct drm_connector *connector = to_drm_connector(device);
	struct drm_display_mode *mode;
	int written = 0;

<<<<<<< HEAD
	list_for_each_entry(mode, &connector->modes, head) {
		written += snprintf(buf + written, PAGE_SIZE - written, "%s\n",
				    mode->name);
	}
=======
	mutex_lock(&connector->dev->mode_config.mutex);
	list_for_each_entry(mode, &connector->modes, head) {
		written += scnprintf(buf + written, PAGE_SIZE - written, "%s\n",
				    mode->name);
	}
	mutex_unlock(&connector->dev->mode_config.mutex);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	return written;
}

<<<<<<< HEAD
static ssize_t subconnector_show(struct device *device,
			   struct device_attribute *attr,
			   char *buf)
{
	struct drm_connector *connector = to_drm_connector(device);
	struct drm_device *dev = connector->dev;
	struct drm_property *prop = NULL;
	uint64_t subconnector;
	int is_tv = 0;
	int ret;

	switch (connector->connector_type) {
		case DRM_MODE_CONNECTOR_DVII:
			prop = dev->mode_config.dvi_i_subconnector_property;
			break;
		case DRM_MODE_CONNECTOR_Composite:
		case DRM_MODE_CONNECTOR_SVIDEO:
		case DRM_MODE_CONNECTOR_Component:
		case DRM_MODE_CONNECTOR_TV:
			prop = dev->mode_config.tv_subconnector_property;
			is_tv = 1;
			break;
		default:
			DRM_ERROR("Wrong connector type for this property\n");
			return 0;
	}

	if (!prop) {
		DRM_ERROR("Unable to find subconnector property\n");
		return 0;
	}

	ret = drm_connector_property_get_value(connector, prop, &subconnector);
	if (ret)
		return 0;

	return snprintf(buf, PAGE_SIZE, "%s", is_tv ?
			drm_get_tv_subconnector_name((int)subconnector) :
			drm_get_dvi_i_subconnector_name((int)subconnector));
}

static ssize_t select_subconnector_show(struct device *device,
			   struct device_attribute *attr,
			   char *buf)
{
	struct drm_connector *connector = to_drm_connector(device);
	struct drm_device *dev = connector->dev;
	struct drm_property *prop = NULL;
	uint64_t subconnector;
	int is_tv = 0;
	int ret;

	switch (connector->connector_type) {
		case DRM_MODE_CONNECTOR_DVII:
			prop = dev->mode_config.dvi_i_select_subconnector_property;
			break;
		case DRM_MODE_CONNECTOR_Composite:
		case DRM_MODE_CONNECTOR_SVIDEO:
		case DRM_MODE_CONNECTOR_Component:
		case DRM_MODE_CONNECTOR_TV:
			prop = dev->mode_config.tv_select_subconnector_property;
			is_tv = 1;
			break;
		default:
			DRM_ERROR("Wrong connector type for this property\n");
			return 0;
	}

	if (!prop) {
		DRM_ERROR("Unable to find select subconnector property\n");
		return 0;
	}

	ret = drm_connector_property_get_value(connector, prop, &subconnector);
	if (ret)
		return 0;

	return snprintf(buf, PAGE_SIZE, "%s", is_tv ?
			drm_get_tv_select_name((int)subconnector) :
			drm_get_dvi_i_select_name((int)subconnector));
}

static struct device_attribute connector_attrs[] = {
	__ATTR_RO(status),
	__ATTR_RO(enabled),
	__ATTR_RO(dpms),
	__ATTR_RO(modes),
};

/* These attributes are for both DVI-I connectors and all types of tv-out. */
static struct device_attribute connector_attrs_opt1[] = {
	__ATTR_RO(subconnector),
	__ATTR_RO(select_subconnector),
=======
static ssize_t connector_id_show(struct device *device,
				 struct device_attribute *attr,
				 char *buf)
{
	struct drm_connector *connector = to_drm_connector(device);

	return sysfs_emit(buf, "%d\n", connector->base.id);
}

static DEVICE_ATTR_RW(status);
static DEVICE_ATTR_RO(enabled);
static DEVICE_ATTR_RO(dpms);
static DEVICE_ATTR_RO(modes);
static DEVICE_ATTR_RO(connector_id);

static struct attribute *connector_dev_attrs[] = {
	&dev_attr_status.attr,
	&dev_attr_enabled.attr,
	&dev_attr_dpms.attr,
	&dev_attr_modes.attr,
	&dev_attr_connector_id.attr,
	NULL
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
};

static struct bin_attribute edid_attr = {
	.attr.name = "edid",
	.attr.mode = 0444,
	.size = 0,
	.read = edid_show,
};

<<<<<<< HEAD
/**
 * drm_sysfs_connector_add - add an connector to sysfs
 * @connector: connector to add
 *
 * Create an connector device in sysfs, along with its associated connector
 * properties (so far, connection status, dpms, mode list & edid) and
 * generate a hotplug event so userspace knows there's a new connector
 * available.
 *
 * Note:
 * This routine should only be called *once* for each DRM minor registered.
 * A second call for an already registered device will trigger the BUG_ON
 * below.
 */
int drm_sysfs_connector_add(struct drm_connector *connector)
{
	struct drm_device *dev = connector->dev;
	int attr_cnt = 0;
	int opt_cnt = 0;
	int i;
	int ret = 0;

	/* We shouldn't get called more than once for the same connector */
	BUG_ON(device_is_registered(&connector->kdev));

	connector->kdev.parent = &dev->primary->kdev;
	connector->kdev.class = drm_class;
	connector->kdev.release = drm_sysfs_device_release;

	DRM_DEBUG("adding \"%s\" to sysfs\n",
		  drm_get_connector_name(connector));

	dev_set_name(&connector->kdev, "card%d-%s",
		     dev->primary->index, drm_get_connector_name(connector));
	ret = device_register(&connector->kdev);

	if (ret) {
		DRM_ERROR("failed to register connector device: %d\n", ret);
		goto out;
	}

	/* Standard attributes */

	for (attr_cnt = 0; attr_cnt < ARRAY_SIZE(connector_attrs); attr_cnt++) {
		ret = device_create_file(&connector->kdev, &connector_attrs[attr_cnt]);
		if (ret)
			goto err_out_files;
	}

	/* Optional attributes */
	/*
	 * In the long run it maybe a good idea to make one set of
	 * optionals per connector type.
	 */
	switch (connector->connector_type) {
		case DRM_MODE_CONNECTOR_DVII:
		case DRM_MODE_CONNECTOR_Composite:
		case DRM_MODE_CONNECTOR_SVIDEO:
		case DRM_MODE_CONNECTOR_Component:
		case DRM_MODE_CONNECTOR_TV:
			for (opt_cnt = 0; opt_cnt < ARRAY_SIZE(connector_attrs_opt1); opt_cnt++) {
				ret = device_create_file(&connector->kdev, &connector_attrs_opt1[opt_cnt]);
				if (ret)
					goto err_out_files;
			}
			break;
		default:
			break;
	}

	ret = sysfs_create_bin_file(&connector->kdev.kobj, &edid_attr);
	if (ret)
		goto err_out_files;

	/* Let userspace know we have a new connector */
	drm_sysfs_hotplug_event(dev);

	return 0;

err_out_files:
	for (i = 0; i < opt_cnt; i++)
		device_remove_file(&connector->kdev, &connector_attrs_opt1[i]);
	for (i = 0; i < attr_cnt; i++)
		device_remove_file(&connector->kdev, &connector_attrs[i]);
	device_unregister(&connector->kdev);

out:
	return ret;
}
EXPORT_SYMBOL(drm_sysfs_connector_add);

/**
 * drm_sysfs_connector_remove - remove an connector device from sysfs
 * @connector: connector to remove
 *
 * Remove @connector and its associated attributes from sysfs.  Note that
 * the device model core will take care of sending the "remove" uevent
 * at this time, so we don't need to do it.
 *
 * Note:
 * This routine should only be called if the connector was previously
 * successfully registered.  If @connector hasn't been registered yet,
 * you'll likely see a panic somewhere deep in sysfs code when called.
 */
void drm_sysfs_connector_remove(struct drm_connector *connector)
{
	int i;

	if (!connector->kdev.parent)
		return;
	DRM_DEBUG("removing \"%s\" from sysfs\n",
		  drm_get_connector_name(connector));

	for (i = 0; i < ARRAY_SIZE(connector_attrs); i++)
		device_remove_file(&connector->kdev, &connector_attrs[i]);
	sysfs_remove_bin_file(&connector->kdev.kobj, &edid_attr);
	device_unregister(&connector->kdev);
	connector->kdev.parent = NULL;
}
EXPORT_SYMBOL(drm_sysfs_connector_remove);
=======
static struct bin_attribute *connector_bin_attrs[] = {
	&edid_attr,
	NULL
};

static const struct attribute_group connector_dev_group = {
	.attrs = connector_dev_attrs,
	.bin_attrs = connector_bin_attrs,
};

static const struct attribute_group *connector_dev_groups[] = {
	&connector_dev_group,
	NULL
};

int drm_sysfs_connector_add(struct drm_connector *connector)
{
	struct drm_device *dev = connector->dev;
	struct device *kdev;
	int r;

	if (connector->kdev)
		return 0;

	kdev = kzalloc(sizeof(*kdev), GFP_KERNEL);
	if (!kdev)
		return -ENOMEM;

	device_initialize(kdev);
	kdev->class = drm_class;
	kdev->type = &drm_sysfs_device_connector;
	kdev->parent = dev->primary->kdev;
	kdev->groups = connector_dev_groups;
	kdev->release = drm_sysfs_release;
	dev_set_drvdata(kdev, connector);

	r = dev_set_name(kdev, "card%d-%s", dev->primary->index, connector->name);
	if (r)
		goto err_free;

	DRM_DEBUG("adding \"%s\" to sysfs\n",
		  connector->name);

	r = device_add(kdev);
	if (r) {
		drm_err(dev, "failed to register connector device: %d\n", r);
		goto err_free;
	}

	connector->kdev = kdev;

	if (dev_fwnode(kdev)) {
		r = component_add(kdev, &typec_connector_ops);
		if (r)
			drm_err(dev, "failed to add component to create link to typec connector\n");
	}

	return 0;

err_free:
	put_device(kdev);
	return r;
}

int drm_sysfs_connector_add_late(struct drm_connector *connector)
{
	if (connector->ddc)
		return sysfs_create_link(&connector->kdev->kobj,
					 &connector->ddc->dev.kobj, "ddc");

	return 0;
}

void drm_sysfs_connector_remove_early(struct drm_connector *connector)
{
	if (connector->ddc)
		sysfs_remove_link(&connector->kdev->kobj, "ddc");
}

void drm_sysfs_connector_remove(struct drm_connector *connector)
{
	if (!connector->kdev)
		return;

	if (dev_fwnode(connector->kdev))
		component_del(connector->kdev, &typec_connector_ops);

	DRM_DEBUG("removing \"%s\" from sysfs\n",
		  connector->name);

	device_unregister(connector->kdev);
	connector->kdev = NULL;
}

void drm_sysfs_lease_event(struct drm_device *dev)
{
	char *event_string = "LEASE=1";
	char *envp[] = { event_string, NULL };

	DRM_DEBUG("generating lease event\n");

	kobject_uevent_env(&dev->primary->kdev->kobj, KOBJ_CHANGE, envp);
}
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

/**
 * drm_sysfs_hotplug_event - generate a DRM uevent
 * @dev: DRM device
 *
 * Send a uevent for the DRM device specified by @dev.  Currently we only
 * set HOTPLUG=1 in the uevent environment, but this could be expanded to
 * deal with other types of events.
<<<<<<< HEAD
=======
 *
 * Any new uapi should be using the drm_sysfs_connector_status_event()
 * for uevents on connector status change.
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 */
void drm_sysfs_hotplug_event(struct drm_device *dev)
{
	char *event_string = "HOTPLUG=1";
	char *envp[] = { event_string, NULL };

	DRM_DEBUG("generating hotplug event\n");

<<<<<<< HEAD
	kobject_uevent_env(&dev->primary->kdev.kobj, KOBJ_CHANGE, envp);
=======
	kobject_uevent_env(&dev->primary->kdev->kobj, KOBJ_CHANGE, envp);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}
EXPORT_SYMBOL(drm_sysfs_hotplug_event);

/**
<<<<<<< HEAD
 * drm_sysfs_device_add - adds a class device to sysfs for a character driver
 * @dev: DRM device to be added
 * @head: DRM head in question
 *
 * Add a DRM device to the DRM's device model class.  We use @dev's PCI device
 * as the parent for the Linux device, and make sure it has a file containing
 * the driver we're using (for userspace compatibility).
 */
int drm_sysfs_device_add(struct drm_minor *minor)
{
	int err;
	char *minor_str;

	minor->kdev.parent = minor->dev->dev;

	minor->kdev.class = drm_class;
	minor->kdev.release = drm_sysfs_device_release;
	minor->kdev.devt = minor->device;
	minor->kdev.type = &drm_sysfs_device_minor;
	if (minor->type == DRM_MINOR_CONTROL)
		minor_str = "controlD%d";
        else if (minor->type == DRM_MINOR_RENDER)
                minor_str = "renderD%d";
        else
                minor_str = "card%d";

	dev_set_name(&minor->kdev, minor_str, minor->index);

	err = device_register(&minor->kdev);
	if (err) {
		DRM_ERROR("device add failed: %d\n", err);
		goto err_out;
	}

	return 0;

err_out:
	return err;
}

/**
 * drm_sysfs_device_remove - remove DRM device
 * @dev: DRM device to remove
 *
 * This call unregisters and cleans up a class device that was created with a
 * call to drm_sysfs_device_add()
 */
void drm_sysfs_device_remove(struct drm_minor *minor)
{
	if (minor->kdev.parent)
		device_unregister(&minor->kdev);
	minor->kdev.parent = NULL;
}


/**
 * drm_class_device_register - Register a struct device in the drm class.
 *
 * @dev: pointer to struct device to register.
 *
 * @dev should have all relevant members pre-filled with the exception
 * of the class member. In particular, the device_type member must
 * be set.
 */

int drm_class_device_register(struct device *dev)
{
=======
 * drm_sysfs_connector_hotplug_event - generate a DRM uevent for any connector
 * change
 * @connector: connector which has changed
 *
 * Send a uevent for the DRM connector specified by @connector. This will send
 * a uevent with the properties HOTPLUG=1 and CONNECTOR.
 */
void drm_sysfs_connector_hotplug_event(struct drm_connector *connector)
{
	struct drm_device *dev = connector->dev;
	char hotplug_str[] = "HOTPLUG=1", conn_id[21];
	char *envp[] = { hotplug_str, conn_id, NULL };

	snprintf(conn_id, sizeof(conn_id),
		 "CONNECTOR=%u", connector->base.id);

	drm_dbg_kms(connector->dev,
		    "[CONNECTOR:%d:%s] generating connector hotplug event\n",
		    connector->base.id, connector->name);

	kobject_uevent_env(&dev->primary->kdev->kobj, KOBJ_CHANGE, envp);
}
EXPORT_SYMBOL(drm_sysfs_connector_hotplug_event);

/**
 * drm_sysfs_connector_property_event - generate a DRM uevent for connector
 * property change
 * @connector: connector on which property changed
 * @property: connector property which has changed.
 *
 * Send a uevent for the specified DRM connector and property.  Currently we
 * set HOTPLUG=1 and connector id along with the attached property id
 * related to the change.
 */
void drm_sysfs_connector_property_event(struct drm_connector *connector,
					struct drm_property *property)
{
	struct drm_device *dev = connector->dev;
	char hotplug_str[] = "HOTPLUG=1", conn_id[21], prop_id[21];
	char *envp[4] = { hotplug_str, conn_id, prop_id, NULL };

	WARN_ON(!drm_mode_obj_find_prop_id(&connector->base,
					   property->base.id));

	snprintf(conn_id, ARRAY_SIZE(conn_id),
		 "CONNECTOR=%u", connector->base.id);
	snprintf(prop_id, ARRAY_SIZE(prop_id),
		 "PROPERTY=%u", property->base.id);

	drm_dbg_kms(connector->dev,
		    "[CONNECTOR:%d:%s] generating connector property event for [PROP:%d:%s]\n",
		    connector->base.id, connector->name,
		    property->base.id, property->name);

	kobject_uevent_env(&dev->primary->kdev->kobj, KOBJ_CHANGE, envp);
}
EXPORT_SYMBOL(drm_sysfs_connector_property_event);

struct device *drm_sysfs_minor_alloc(struct drm_minor *minor)
{
	const char *minor_str;
	struct device *kdev;
	int r;

	kdev = kzalloc(sizeof(*kdev), GFP_KERNEL);
	if (!kdev)
		return ERR_PTR(-ENOMEM);

	device_initialize(kdev);

	if (minor->type == DRM_MINOR_ACCEL) {
		minor_str = "accel%d";
		accel_set_device_instance_params(kdev, minor->index);
	} else {
		if (minor->type == DRM_MINOR_RENDER)
			minor_str = "renderD%d";
		else
			minor_str = "card%d";

		kdev->devt = MKDEV(DRM_MAJOR, minor->index);
		kdev->class = drm_class;
		kdev->type = &drm_sysfs_device_minor;
	}

	kdev->parent = minor->dev->dev;
	kdev->release = drm_sysfs_release;
	dev_set_drvdata(kdev, minor);

	r = dev_set_name(kdev, minor_str, minor->index);
	if (r < 0)
		goto err_free;

	return kdev;

err_free:
	put_device(kdev);
	return ERR_PTR(r);
}

/**
 * drm_class_device_register - register new device with the DRM sysfs class
 * @dev: device to register
 *
 * Registers a new &struct device within the DRM sysfs class. Essentially only
 * used by ttm to have a place for its global settings. Drivers should never use
 * this.
 */
int drm_class_device_register(struct device *dev)
{
	if (!drm_class || IS_ERR(drm_class))
		return -ENOENT;

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	dev->class = drm_class;
	return device_register(dev);
}
EXPORT_SYMBOL_GPL(drm_class_device_register);

<<<<<<< HEAD
=======
/**
 * drm_class_device_unregister - unregister device with the DRM sysfs class
 * @dev: device to unregister
 *
 * Unregisters a &struct device from the DRM sysfs class. Essentially only used
 * by ttm to have a place for its global settings. Drivers should never use
 * this.
 */
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
void drm_class_device_unregister(struct device *dev)
{
	return device_unregister(dev);
}
EXPORT_SYMBOL_GPL(drm_class_device_unregister);
