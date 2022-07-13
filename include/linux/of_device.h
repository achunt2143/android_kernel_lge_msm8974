<<<<<<< HEAD
#ifndef _LINUX_OF_DEVICE_H
#define _LINUX_OF_DEVICE_H

#include <linux/platform_device.h>
#include <linux/of_platform.h> /* temporary until merge */

#ifdef CONFIG_OF_DEVICE
#include <linux/of.h>
#include <linux/mod_devicetable.h>

struct device;

extern const struct of_device_id *of_match_device(
	const struct of_device_id *matches, const struct device *dev);
extern void of_device_make_bus_id(struct device *dev);
=======
/* SPDX-License-Identifier: GPL-2.0 */
#ifndef _LINUX_OF_DEVICE_H
#define _LINUX_OF_DEVICE_H

#include <linux/device/driver.h>

struct device;
struct of_device_id;
struct kobj_uevent_env;

#ifdef CONFIG_OF
extern const struct of_device_id *of_match_device(
	const struct of_device_id *matches, const struct device *dev);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

/**
 * of_driver_match_device - Tell if a driver's of_match_table matches a device.
 * @drv: the device_driver structure to test
 * @dev: the device structure to match against
 */
static inline int of_driver_match_device(struct device *dev,
					 const struct device_driver *drv)
{
	return of_match_device(drv->of_match_table, dev) != NULL;
}

<<<<<<< HEAD
extern struct platform_device *of_dev_get(struct platform_device *dev);
extern void of_dev_put(struct platform_device *dev);

extern int of_device_add(struct platform_device *pdev);
extern int of_device_register(struct platform_device *ofdev);
extern void of_device_unregister(struct platform_device *ofdev);

extern ssize_t of_device_get_modalias(struct device *dev,
					char *str, ssize_t len);

extern void of_device_uevent(struct device *dev, struct kobj_uevent_env *env);
extern int of_device_uevent_modalias(struct device *dev, struct kobj_uevent_env *env);

static inline void of_device_node_put(struct device *dev)
{
	of_node_put(dev->of_node);
}

#else /* CONFIG_OF_DEVICE */

static inline int of_driver_match_device(struct device *dev,
					 struct device_driver *drv)
=======
extern ssize_t of_device_modalias(struct device *dev, char *str, ssize_t len);

extern void of_device_uevent(const struct device *dev, struct kobj_uevent_env *env);
extern int of_device_uevent_modalias(const struct device *dev, struct kobj_uevent_env *env);

int of_dma_configure_id(struct device *dev,
		     struct device_node *np,
		     bool force_dma, const u32 *id);
static inline int of_dma_configure(struct device *dev,
				   struct device_node *np,
				   bool force_dma)
{
	return of_dma_configure_id(dev, np, force_dma, NULL);
}

void of_device_make_bus_id(struct device *dev);

#else /* CONFIG_OF */

static inline int of_driver_match_device(struct device *dev,
					 const struct device_driver *drv)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
{
	return 0;
}

<<<<<<< HEAD
static inline void of_device_uevent(struct device *dev,
			struct kobj_uevent_env *env) { }

static inline int of_device_uevent_modalias(struct device *dev,
				   struct kobj_uevent_env *env)
=======
static inline void of_device_uevent(const struct device *dev,
			struct kobj_uevent_env *env) { }

static inline int of_device_modalias(struct device *dev,
				     char *str, ssize_t len)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
{
	return -ENODEV;
}

<<<<<<< HEAD
static inline void of_device_node_put(struct device *dev) { }
=======
static inline int of_device_uevent_modalias(const struct device *dev,
				   struct kobj_uevent_env *env)
{
	return -ENODEV;
}
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

static inline const struct of_device_id *of_match_device(
		const struct of_device_id *matches, const struct device *dev)
{
	return NULL;
}
<<<<<<< HEAD
#endif /* CONFIG_OF_DEVICE */
=======

static inline int of_dma_configure_id(struct device *dev,
				      struct device_node *np,
				      bool force_dma,
				      const u32 *id)
{
	return 0;
}
static inline int of_dma_configure(struct device *dev,
				   struct device_node *np,
				   bool force_dma)
{
	return 0;
}

static inline void of_device_make_bus_id(struct device *dev) {}

#endif /* CONFIG_OF */
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

#endif /* _LINUX_OF_DEVICE_H */
