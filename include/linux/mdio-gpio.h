<<<<<<< HEAD
/*
 * MDIO-GPIO bus platform data structures
 *
 * Copyright (C) 2008, Paulius Zaleckas <paulius.zaleckas@teltonika.lt>
 *
 * This file is licensed under the terms of the GNU General Public License
 * version 2. This program is licensed "as is" without any warranty of any
 * kind, whether express or implied.
 */

#ifndef __LINUX_MDIO_GPIO_H
#define __LINUX_MDIO_GPIO_H

#include <linux/mdio-bitbang.h>

struct mdio_gpio_platform_data {
	/* GPIO numbers for bus pins */
	unsigned int mdc;
	unsigned int mdio;

	unsigned int phy_mask;
	int irqs[PHY_MAX_ADDR];
	/* reset callback */
	int (*reset)(struct mii_bus *bus);
};

#endif /* __LINUX_MDIO_GPIO_H */
=======
/* SPDX-License-Identifier: GPL-2.0 */
#ifndef __LINUX_MDIO_GPIO_H
#define __LINUX_MDIO_GPIO_H

#define MDIO_GPIO_MDC	0
#define MDIO_GPIO_MDIO	1
#define MDIO_GPIO_MDO	2

#endif
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
