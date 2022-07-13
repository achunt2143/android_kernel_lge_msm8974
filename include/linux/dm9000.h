<<<<<<< HEAD
=======
/* SPDX-License-Identifier: GPL-2.0-only */
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
/* include/linux/dm9000.h
 *
 * Copyright (c) 2004 Simtec Electronics
 *   Ben Dooks <ben@simtec.co.uk>
 *
 * Header file for dm9000 platform data
<<<<<<< HEAD
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 *
=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
*/

#ifndef __DM9000_PLATFORM_DATA
#define __DM9000_PLATFORM_DATA __FILE__

<<<<<<< HEAD
=======
#include <linux/if_ether.h>

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
/* IO control flags */

#define DM9000_PLATF_8BITONLY	(0x0001)
#define DM9000_PLATF_16BITONLY	(0x0002)
#define DM9000_PLATF_32BITONLY	(0x0004)
#define DM9000_PLATF_EXT_PHY	(0x0008)
#define DM9000_PLATF_NO_EEPROM	(0x0010)
#define DM9000_PLATF_SIMPLE_PHY (0x0020)  /* Use NSR to find LinkStatus */

/* platform data for platform device structure's platform_data field */

struct dm9000_plat_data {
	unsigned int	flags;
<<<<<<< HEAD
	unsigned char	dev_addr[6];
=======
	unsigned char	dev_addr[ETH_ALEN];
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	/* allow replacement IO routines */

	void	(*inblk)(void __iomem *reg, void *data, int len);
	void	(*outblk)(void __iomem *reg, void *data, int len);
	void	(*dumpblk)(void __iomem *reg, int len);
};

#endif /* __DM9000_PLATFORM_DATA */

