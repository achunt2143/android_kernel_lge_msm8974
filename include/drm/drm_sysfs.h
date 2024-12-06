<<<<<<< HEAD
#ifndef _DRM_SYSFS_H_
#define _DRM_SYSFS_H_

/**
 * This minimalistic include file is intended for users (read TTM) that
 * don't want to include the full drmP.h file.
 */

extern int drm_class_device_register(struct device *dev);
extern void drm_class_device_unregister(struct device *dev);

=======
/* SPDX-License-Identifier: GPL-2.0 */
#ifndef _DRM_SYSFS_H_
#define _DRM_SYSFS_H_

struct drm_device;
struct device;
struct drm_connector;
struct drm_property;

int drm_class_device_register(struct device *dev);
void drm_class_device_unregister(struct device *dev);

void drm_sysfs_hotplug_event(struct drm_device *dev);
void drm_sysfs_connector_hotplug_event(struct drm_connector *connector);
void drm_sysfs_connector_property_event(struct drm_connector *connector,
					struct drm_property *property);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#endif
