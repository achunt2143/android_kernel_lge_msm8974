<<<<<<< HEAD
/*
 * Copyright 2007 Freescale Semiconductor, Inc. All Rights Reserved.
 * Copyright 2008 Juergen Beisert, kernel@pengutronix.de
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston,
 * MA 02110-1301, USA.
=======
// SPDX-License-Identifier: GPL-2.0-or-later
/*
 * Copyright 2007 Freescale Semiconductor, Inc. All Rights Reserved.
 * Copyright 2008 Juergen Beisert, kernel@pengutronix.de
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 */

/*
 * i.MX27 specific CPU detection code
 */

#include <linux/io.h>
<<<<<<< HEAD
#include <linux/module.h>

#include <mach/hardware.h>
=======
#include <linux/of_address.h>
#include <linux/module.h>

#include "hardware.h"
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

static int mx27_cpu_rev = -1;
static int mx27_cpu_partnumber;

#define SYS_CHIP_ID             0x00    /* The offset of CHIP ID register */
<<<<<<< HEAD

static int mx27_read_cpu_rev(void)
{
	u32 val;
=======
#define SYSCTRL_OFFSET		0x800	/* Offset from CCM base address */

static int mx27_read_cpu_rev(void)
{
	void __iomem *ccm_base;
	struct device_node *np;
	u32 val;

	np = of_find_compatible_node(NULL, NULL, "fsl,imx27-ccm");
	ccm_base = of_iomap(np, 0);
	of_node_put(np);
	BUG_ON(!ccm_base);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	/*
	 * now we have access to the IO registers. As we need
	 * the silicon revision very early we read it here to
	 * avoid any further hooks
	*/
<<<<<<< HEAD
	val = __raw_readl(MX27_IO_ADDRESS(MX27_SYSCTRL_BASE_ADDR
				+ SYS_CHIP_ID));
=======
	val = imx_readl(ccm_base + SYSCTRL_OFFSET + SYS_CHIP_ID);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	mx27_cpu_partnumber = (int)((val >> 12) & 0xFFFF);

	switch (val >> 28) {
	case 0:
		return IMX_CHIP_REVISION_1_0;
	case 1:
		return IMX_CHIP_REVISION_2_0;
	case 2:
		return IMX_CHIP_REVISION_2_1;
	default:
		return IMX_CHIP_REVISION_UNKNOWN;
	}
}

/*
 * Returns:
 *	the silicon revision of the cpu
 *	-EINVAL - not a mx27
 */
int mx27_revision(void)
{
	if (mx27_cpu_rev == -1)
		mx27_cpu_rev = mx27_read_cpu_rev();

	if (mx27_cpu_partnumber != 0x8821)
		return -EINVAL;

	return mx27_cpu_rev;
}
EXPORT_SYMBOL(mx27_revision);
