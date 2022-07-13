<<<<<<< HEAD
=======
/* SPDX-License-Identifier: GPL-2.0-only */
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
/*
 *  linux/drivers/mmc/core/bus.h
 *
 *  Copyright (C) 2003 Russell King, All Rights Reserved.
 *  Copyright 2007 Pierre Ossman
<<<<<<< HEAD
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 */
#ifndef _MMC_CORE_BUS_H
#define _MMC_CORE_BUS_H

<<<<<<< HEAD
=======
#include <linux/device.h>
#include <linux/sysfs.h>

struct mmc_host;
struct mmc_card;

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#define MMC_DEV_ATTR(name, fmt, args...)					\
static ssize_t mmc_##name##_show (struct device *dev, struct device_attribute *attr, char *buf)	\
{										\
	struct mmc_card *card = mmc_dev_to_card(dev);				\
<<<<<<< HEAD
	return sprintf(buf, fmt, args);						\
=======
	return sysfs_emit(buf, fmt, args);					\
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}										\
static DEVICE_ATTR(name, S_IRUGO, mmc_##name##_show, NULL)

struct mmc_card *mmc_alloc_card(struct mmc_host *host,
<<<<<<< HEAD
	struct device_type *type);
=======
				const struct device_type *type);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
int mmc_add_card(struct mmc_card *card);
void mmc_remove_card(struct mmc_card *card);

int mmc_register_bus(void);
void mmc_unregister_bus(void);

<<<<<<< HEAD
#endif

=======
struct mmc_driver {
	struct device_driver drv;
	int (*probe)(struct mmc_card *card);
	void (*remove)(struct mmc_card *card);
	void (*shutdown)(struct mmc_card *card);
};

int mmc_register_driver(struct mmc_driver *drv);
void mmc_unregister_driver(struct mmc_driver *drv);

#endif
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
