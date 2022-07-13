<<<<<<< HEAD
=======
/* SPDX-License-Identifier: GPL-2.0-or-later */
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
/*
 * AD714X CapTouch Programmable Controller driver (bus interfaces)
 *
 * Copyright 2009-2011 Analog Devices Inc.
<<<<<<< HEAD
 *
 * Licensed under the GPL-2 or later.
=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 */

#ifndef _AD714X_H_
#define _AD714X_H_

<<<<<<< HEAD
=======
#include <linux/pm.h>
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#include <linux/types.h>

#define STAGE_NUM              12

struct device;
struct ad714x_platform_data;
struct ad714x_driver_data;
struct ad714x_chip;

typedef int (*ad714x_read_t)(struct ad714x_chip *, unsigned short, unsigned short *, size_t);
typedef int (*ad714x_write_t)(struct ad714x_chip *, unsigned short, unsigned short);

struct ad714x_chip {
	unsigned short l_state;
	unsigned short h_state;
	unsigned short c_state;
	unsigned short adc_reg[STAGE_NUM];
	unsigned short amb_reg[STAGE_NUM];
	unsigned short sensor_val[STAGE_NUM];

	struct ad714x_platform_data *hw;
	struct ad714x_driver_data *sw;

	int irq;
	struct device *dev;
	ad714x_read_t read;
	ad714x_write_t write;

	struct mutex mutex;

	unsigned product;
	unsigned version;

	__be16 xfer_buf[16] ____cacheline_aligned;

};

<<<<<<< HEAD
int ad714x_disable(struct ad714x_chip *ad714x);
int ad714x_enable(struct ad714x_chip *ad714x);
struct ad714x_chip *ad714x_probe(struct device *dev, u16 bus_type, int irq,
				 ad714x_read_t read, ad714x_write_t write);
void ad714x_remove(struct ad714x_chip *ad714x);
=======
extern const struct dev_pm_ops ad714x_pm;
struct ad714x_chip *ad714x_probe(struct device *dev, u16 bus_type, int irq,
				 ad714x_read_t read, ad714x_write_t write);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

#endif
