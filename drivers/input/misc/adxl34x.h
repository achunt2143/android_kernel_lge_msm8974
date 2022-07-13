<<<<<<< HEAD
=======
/* SPDX-License-Identifier: GPL-2.0-or-later */
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
/*
 * ADXL345/346 Three-Axis Digital Accelerometers (I2C/SPI Interface)
 *
 * Enter bugs at http://blackfin.uclinux.org/
 *
 * Copyright (C) 2009 Michael Hennerich, Analog Devices Inc.
<<<<<<< HEAD
 * Licensed under the GPL-2 or later.
=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 */

#ifndef _ADXL34X_H_
#define _ADXL34X_H_

struct device;
struct adxl34x;

struct adxl34x_bus_ops {
	u16 bustype;
	int (*read)(struct device *, unsigned char);
	int (*read_block)(struct device *, unsigned char, int, void *);
	int (*write)(struct device *, unsigned char, unsigned char);
};

<<<<<<< HEAD
void adxl34x_suspend(struct adxl34x *ac);
void adxl34x_resume(struct adxl34x *ac);
struct adxl34x *adxl34x_probe(struct device *dev, int irq,
			      bool fifo_delay_default,
			      const struct adxl34x_bus_ops *bops);
int adxl34x_remove(struct adxl34x *ac);
=======
struct adxl34x *adxl34x_probe(struct device *dev, int irq,
			      bool fifo_delay_default,
			      const struct adxl34x_bus_ops *bops);
void adxl34x_remove(struct adxl34x *ac);

extern const struct dev_pm_ops adxl34x_pm;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

#endif
