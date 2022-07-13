<<<<<<< HEAD
#ifndef LINUX_SPI_MAX7301_H
#define LINUX_SPI_MAX7301_H

#include <linux/gpio.h>
=======
/* SPDX-License-Identifier: GPL-2.0 */
#ifndef LINUX_SPI_MAX7301_H
#define LINUX_SPI_MAX7301_H

#include <linux/gpio/driver.h>
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

/*
 * Some registers must be read back to modify.
 * To save time we cache them here in memory
 */
struct max7301 {
	struct mutex	lock;
	u8		port_config[8];	/* field 0 is unused */
	u32		out_level;	/* cached output levels */
	u32		input_pullup_active;
	struct gpio_chip chip;
	struct device *dev;
	int (*write)(struct device *dev, unsigned int reg, unsigned int val);
	int (*read)(struct device *dev, unsigned int reg);
};

struct max7301_platform_data {
	/* number assigned to the first GPIO */
	unsigned	base;
	/*
	 * bitmask controlling the pullup configuration,
	 *
	 * _note_ the 4 lowest bits are unused, because the first 4
	 * ports of the controller are not used, too.
	 */
	u32		input_pullup_active;
};

<<<<<<< HEAD
extern int __max730x_remove(struct device *dev);
=======
extern void __max730x_remove(struct device *dev);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
extern int __max730x_probe(struct max7301 *ts);
#endif
