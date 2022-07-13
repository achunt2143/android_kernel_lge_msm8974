<<<<<<< HEAD
/*
 * Copyright (C) ST-Ericsson SA 2011
 * Author: Lee Jones <lee.jones@linaro.org> for ST-Ericsson.
 * License terms:  GNU General Public License (GPL), version 2
=======
/* SPDX-License-Identifier: GPL-2.0-only */
/*
 * Copyright (C) ST-Ericsson SA 2011
 * Author: Lee Jones <lee.jones@linaro.org> for ST-Ericsson.
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 */
#ifndef __SOC_BUS_H
#define __SOC_BUS_H

#include <linux/device.h>

struct soc_device_attribute {
	const char *machine;
	const char *family;
	const char *revision;
<<<<<<< HEAD
	const char *soc_id;
=======
	const char *serial_number;
	const char *soc_id;
	const void *data;
	const struct attribute_group *custom_attr_group;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
};

/**
 * soc_device_register - register SoC as a device
 * @soc_plat_dev_attr: Attributes passed from platform to be attributed to a SoC
 */
struct soc_device *soc_device_register(
	struct soc_device_attribute *soc_plat_dev_attr);

/**
 * soc_device_unregister - unregister SoC device
 * @dev: SoC device to be unregistered
 */
void soc_device_unregister(struct soc_device *soc_dev);

/**
 * soc_device_to_device - helper function to fetch struct device
 * @soc: Previously registered SoC device container
 */
struct device *soc_device_to_device(struct soc_device *soc);

<<<<<<< HEAD
=======
#ifdef CONFIG_SOC_BUS
const struct soc_device_attribute *soc_device_match(
	const struct soc_device_attribute *matches);
#else
static inline const struct soc_device_attribute *soc_device_match(
	const struct soc_device_attribute *matches) { return NULL; }
#endif

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#endif /* __SOC_BUS_H */
