<<<<<<< HEAD
=======
// SPDX-License-Identifier: GPL-2.0-only
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
/*
 * arch/arm/mach-orion5x/mpp.c
 *
 * MPP functions for Marvell Orion 5x SoCs
<<<<<<< HEAD
 *
 * This file is licensed under the terms of the GNU General Public
 * License version 2.  This program is licensed "as is" without any
 * warranty of any kind, whether express or implied.
=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 */

#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/io.h>
<<<<<<< HEAD
#include <mach/hardware.h>
#include <plat/mpp.h>
=======
#include <plat/mpp.h>
#include "orion5x.h"
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#include "mpp.h"
#include "common.h"

static unsigned int __init orion5x_variant(void)
{
	u32 dev;
	u32 rev;

	orion5x_pcie_id(&dev, &rev);

	if (dev == MV88F5181_DEV_ID)
		return MPP_F5181_MASK;

	if (dev == MV88F5182_DEV_ID)
		return MPP_F5182_MASK;

	if (dev == MV88F5281_DEV_ID)
		return MPP_F5281_MASK;

	printk(KERN_ERR "MPP setup: unknown orion5x variant "
	       "(dev %#x rev %#x)\n", dev, rev);
	return 0;
}

void __init orion5x_mpp_conf(unsigned int *mpp_list)
{
	orion_mpp_conf(mpp_list, orion5x_variant(),
		       MPP_MAX, ORION5X_DEV_BUS_VIRT_BASE);
}
