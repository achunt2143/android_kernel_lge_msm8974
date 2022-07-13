<<<<<<< HEAD
/*
 * Flash support for OMAP1
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
=======
// SPDX-License-Identifier: GPL-2.0-only
/*
 * Flash support for OMAP1
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 */

#include <linux/io.h>
#include <linux/mtd/mtd.h>
#include <linux/mtd/map.h>
<<<<<<< HEAD

#include <plat/tc.h>
#include <plat/flash.h>

#include <mach/hardware.h>
=======
#include <linux/soc/ti/omap1-io.h>

#include "tc.h"

#include "flash.h"

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

void omap1_set_vpp(struct platform_device *pdev, int enable)
{
	u32 l;

	l = omap_readl(EMIFS_CONFIG);
	if (enable)
		l |= OMAP_EMIFS_CONFIG_WP;
	else
		l &= ~OMAP_EMIFS_CONFIG_WP;
	omap_writel(l, EMIFS_CONFIG);
}
