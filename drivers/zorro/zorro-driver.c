/*
 *  Zorro Driver Services
 *
 *  Copyright (C) 2003 Geert Uytterhoeven
 *
 *  Loosely based on drivers/pci/pci-driver.c
 *
 *  This file is subject to the terms and conditions of the GNU General Public
 *  License.  See the file COPYING in the main directory of this archive
 *  for more details.
 */

#include <linux/init.h>
#include <linux/module.h>
#include <linux/zorro.h>

<<<<<<< HEAD
=======
#include "zorro.h"

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

    /**
     *  zorro_match_device - Tell if a Zorro device structure has a matching
     *                       Zorro device id structure
     *  @ids: array of Zorro device id structures to search in
     *  @dev: the Zorro device structure to match against
     *
     *  Used by a driver to check whether a Zorro device present in the
     *  system is in its list of supported devices. Returns the matching
     *  zorro_device_id structure or %NULL if there is no match.
     */

<<<<<<< HEAD
const struct zorro_device_id *
=======
static const struct zorro_device_id *
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
zorro_match_device(const struct zorro_device_id *ids,
		   const struct zorro_dev *z)
{
	while (ids->id) {
		if (ids->id == ZORRO_WILDCARD || ids->id == z->id)
			return ids;
		ids++;
	}
	return NULL;
}
<<<<<<< HEAD
EXPORT_SYMBOL(zorro_match_device);
=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)


static int zorro_device_probe(struct device *dev)
{
	int error = 0;
	struct zorro_driver *drv = to_zorro_driver(dev->driver);
	struct zorro_dev *z = to_zorro_dev(dev);

<<<<<<< HEAD
	if (!z->driver && drv->probe) {
=======
	if (drv->probe) {
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		const struct zorro_device_id *id;

		id = zorro_match_device(drv->id_table, z);
		if (id)
			error = drv->probe(z, id);
<<<<<<< HEAD
		if (error >= 0) {
			z->driver = drv;
			error = 0;
		}
=======
		if (error >= 0)
			error = 0;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	}
	return error;
}


<<<<<<< HEAD
static int zorro_device_remove(struct device *dev)
=======
static void zorro_device_remove(struct device *dev)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
{
	struct zorro_dev *z = to_zorro_dev(dev);
	struct zorro_driver *drv = to_zorro_driver(dev->driver);

<<<<<<< HEAD
	if (drv) {
		if (drv->remove)
			drv->remove(z);
		z->driver = NULL;
	}
	return 0;
=======
	if (drv->remove)
		drv->remove(z);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}


    /**
     *  zorro_register_driver - register a new Zorro driver
     *  @drv: the driver structure to register
     *
     *  Adds the driver structure to the list of registered drivers
     *  Returns zero or a negative error value.
     */

int zorro_register_driver(struct zorro_driver *drv)
{
	/* initialize common driver fields */
	drv->driver.name = drv->name;
	drv->driver.bus = &zorro_bus_type;

	/* register with core */
	return driver_register(&drv->driver);
}
EXPORT_SYMBOL(zorro_register_driver);


    /**
     *  zorro_unregister_driver - unregister a zorro driver
     *  @drv: the driver structure to unregister
     *
     *  Deletes the driver structure from the list of registered Zorro drivers,
     *  gives it a chance to clean up by calling its remove() function for
     *  each device it was responsible for, and marks those devices as
     *  driverless.
     */

void zorro_unregister_driver(struct zorro_driver *drv)
{
	driver_unregister(&drv->driver);
}
EXPORT_SYMBOL(zorro_unregister_driver);


    /**
     *  zorro_bus_match - Tell if a Zorro device structure has a matching Zorro
     *                    device id structure
     *  @ids: array of Zorro device id structures to search in
     *  @dev: the Zorro device structure to match against
     *
<<<<<<< HEAD
     *  Used by a driver to check whether a Zorro device present in the
     *  system is in its list of supported devices.Returns the matching
     *  zorro_device_id structure or %NULL if there is no match.
=======
     *  Used by the driver core to check whether a Zorro device present in the
     *  system is in a driver's list of supported devices.  Returns 1 if
     *  supported, and 0 if there is no match.
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
     */

static int zorro_bus_match(struct device *dev, struct device_driver *drv)
{
	struct zorro_dev *z = to_zorro_dev(dev);
	struct zorro_driver *zorro_drv = to_zorro_driver(drv);
	const struct zorro_device_id *ids = zorro_drv->id_table;

	if (!ids)
		return 0;

<<<<<<< HEAD
	while (ids->id) {
		if (ids->id == ZORRO_WILDCARD || ids->id == z->id)
			return 1;
		ids++;
	}
	return 0;
}

static int zorro_uevent(struct device *dev, struct kobj_uevent_env *env)
{
#ifdef CONFIG_HOTPLUG
	struct zorro_dev *z;
=======
	return !!zorro_match_device(ids, z);
}

static int zorro_uevent(const struct device *dev, struct kobj_uevent_env *env)
{
	const struct zorro_dev *z;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	if (!dev)
		return -ENODEV;

	z = to_zorro_dev(dev);
	if (!z)
		return -ENODEV;

	if (add_uevent_var(env, "ZORRO_ID=%08X", z->id) ||
	    add_uevent_var(env, "ZORRO_SLOT_NAME=%s", dev_name(dev)) ||
	    add_uevent_var(env, "ZORRO_SLOT_ADDR=%04X", z->slotaddr) ||
	    add_uevent_var(env, "MODALIAS=" ZORRO_DEVICE_MODALIAS_FMT, z->id))
		return -ENOMEM;

	return 0;
<<<<<<< HEAD
#else /* !CONFIG_HOTPLUG */
	return -ENODEV;
#endif /* !CONFIG_HOTPLUG */
}

struct bus_type zorro_bus_type = {
	.name	= "zorro",
	.match	= zorro_bus_match,
	.uevent	= zorro_uevent,
	.probe	= zorro_device_probe,
	.remove	= zorro_device_remove,
=======
}

const struct bus_type zorro_bus_type = {
	.name		= "zorro",
	.dev_name	= "zorro",
	.dev_groups	= zorro_device_attribute_groups,
	.match		= zorro_bus_match,
	.uevent		= zorro_uevent,
	.probe		= zorro_device_probe,
	.remove		= zorro_device_remove,
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
};
EXPORT_SYMBOL(zorro_bus_type);


static int __init zorro_driver_init(void)
{
	return bus_register(&zorro_bus_type);
}

postcore_initcall(zorro_driver_init);

