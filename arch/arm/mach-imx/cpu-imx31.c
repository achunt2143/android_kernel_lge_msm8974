<<<<<<< HEAD
=======
// SPDX-License-Identifier: GPL-2.0-or-later
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
/*
 * MX31 CPU type detection
 *
 * Copyright (c) 2009 Daniel Mack <daniel@caiaq.de>
<<<<<<< HEAD
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 */

#include <linux/module.h>
#include <linux/io.h>
#include <mach/hardware.h>
#include <mach/iim.h>
#include <mach/common.h>
=======
 */

#include <linux/module.h>
#include <linux/of_address.h>
#include <linux/io.h>

#include "common.h"
#include "hardware.h"
#include "iim.h"
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

static int mx31_cpu_rev = -1;

static struct {
	u8 srev;
	const char *name;
	unsigned int rev;
} mx31_cpu_type[] = {
	{ .srev = 0x00, .name = "i.MX31(L)", .rev = IMX_CHIP_REVISION_1_0 },
	{ .srev = 0x10, .name = "i.MX31",    .rev = IMX_CHIP_REVISION_1_1 },
	{ .srev = 0x11, .name = "i.MX31L",   .rev = IMX_CHIP_REVISION_1_1 },
	{ .srev = 0x12, .name = "i.MX31",    .rev = IMX_CHIP_REVISION_1_1 },
	{ .srev = 0x13, .name = "i.MX31L",   .rev = IMX_CHIP_REVISION_1_1 },
	{ .srev = 0x14, .name = "i.MX31",    .rev = IMX_CHIP_REVISION_1_2 },
	{ .srev = 0x15, .name = "i.MX31L",   .rev = IMX_CHIP_REVISION_1_2 },
	{ .srev = 0x28, .name = "i.MX31",    .rev = IMX_CHIP_REVISION_2_0 },
	{ .srev = 0x29, .name = "i.MX31L",   .rev = IMX_CHIP_REVISION_2_0 },
};

static int mx31_read_cpu_rev(void)
{
<<<<<<< HEAD
	u32 i, srev;

	/* read SREV register from IIM module */
	srev = __raw_readl(MX31_IO_ADDRESS(MX31_IIM_BASE_ADDR + MXC_IIMSREV));
=======
	void __iomem *iim_base;
	struct device_node *np;
	u32 i, srev;

	np = of_find_compatible_node(NULL, NULL, "fsl,imx31-iim");
	iim_base = of_iomap(np, 0);
	of_node_put(np);
	BUG_ON(!iim_base);

	/* read SREV register from IIM module */
	srev = imx_readl(iim_base + MXC_IIMSREV);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	srev &= 0xff;

	for (i = 0; i < ARRAY_SIZE(mx31_cpu_type); i++)
		if (srev == mx31_cpu_type[i].srev) {
			imx_print_silicon_rev(mx31_cpu_type[i].name,
						mx31_cpu_type[i].rev);
			return mx31_cpu_type[i].rev;
		}

	imx_print_silicon_rev("i.MX31", IMX_CHIP_REVISION_UNKNOWN);
	return IMX_CHIP_REVISION_UNKNOWN;
}

int mx31_revision(void)
{
	if (mx31_cpu_rev == -1)
		mx31_cpu_rev = mx31_read_cpu_rev();

	return mx31_cpu_rev;
}
EXPORT_SYMBOL(mx31_revision);
