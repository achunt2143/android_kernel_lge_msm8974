<<<<<<< HEAD
=======
/* SPDX-License-Identifier: GPL-2.0-or-later */
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
/*
 * AD7879/AD7889 touchscreen (bus interfaces)
 *
 * Copyright (C) 2008-2010 Michael Hennerich, Analog Devices Inc.
<<<<<<< HEAD
 *
 * Licensed under the GPL-2 or later.
=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 */

#ifndef _AD7879_H_
#define _AD7879_H_

<<<<<<< HEAD
#include <linux/types.h>

struct ad7879;
struct device;

struct ad7879_bus_ops {
	u16 bustype;
	int (*read)(struct device *dev, u8 reg);
	int (*multi_read)(struct device *dev, u8 first_reg, u8 count, u16 *buf);
	int (*write)(struct device *dev, u8 reg, u16 val);
};

extern const struct dev_pm_ops ad7879_pm_ops;

struct ad7879 *ad7879_probe(struct device *dev, u8 devid, unsigned irq,
			    const struct ad7879_bus_ops *bops);
void ad7879_remove(struct ad7879 *);
=======
#include <linux/pm.h>
#include <linux/types.h>

struct attribute_group;
struct device;
struct regmap;

extern const struct attribute_group *ad7879_groups[];
extern const struct dev_pm_ops ad7879_pm_ops;

int ad7879_probe(struct device *dev, struct regmap *regmap,
		 int irq, u16 bustype, u8 devid);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

#endif
