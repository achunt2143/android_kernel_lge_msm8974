<<<<<<< HEAD
=======
// SPDX-License-Identifier: GPL-2.0-only
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
/*
 * OMAP3 voltage domain data
 *
 * Copyright (C) 2011 Texas Instruments, Inc.
<<<<<<< HEAD
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 */
#include <linux/kernel.h>
#include <linux/init.h>

#include "voltage.h"

static struct voltagedomain omap2_voltdm_core = {
	.name = "core",
};

static struct voltagedomain omap2_voltdm_wkup = {
	.name = "wakeup",
};

static struct voltagedomain *voltagedomains_omap2[] __initdata = {
	&omap2_voltdm_core,
	&omap2_voltdm_wkup,
	NULL,
};

void __init omap2xxx_voltagedomains_init(void)
{
	voltdm_init(voltagedomains_omap2);
}
