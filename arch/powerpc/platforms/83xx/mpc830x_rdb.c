<<<<<<< HEAD
=======
// SPDX-License-Identifier: GPL-2.0-or-later
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
/*
 * arch/powerpc/platforms/83xx/mpc830x_rdb.c
 *
 * Description: MPC830x RDB board specific routines.
 * This file is based on mpc831x_rdb.c
 *
 * Copyright (C) Freescale Semiconductor, Inc. 2009. All rights reserved.
 * Copyright (C) 2010. Ilya Yanok, Emcraft Systems, yanok@emcraft.com
<<<<<<< HEAD
 *
 * This program is free software; you can redistribute  it and/or modify it
 * under  the terms of  the GNU General  Public License as published by the
 * Free Software Foundation;  either version 2 of the  License, or (at your
 * option) any later version.
=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 */

#include <linux/pci.h>
#include <linux/of_platform.h>
#include <asm/time.h>
#include <asm/ipic.h>
#include <asm/udbg.h>
#include <sysdev/fsl_pci.h>
#include <sysdev/fsl_soc.h>
#include "mpc83xx.h"

/*
 * Setup the architecture
 */
static void __init mpc830x_rdb_setup_arch(void)
{
<<<<<<< HEAD
	if (ppc_md.progress)
		ppc_md.progress("mpc830x_rdb_setup_arch()", 0);

	mpc83xx_setup_pci();
=======
	mpc83xx_setup_arch();
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	mpc831x_usb_cfg();
}

static const char *board[] __initdata = {
	"MPC8308RDB",
	"fsl,mpc8308rdb",
	"denx,mpc8308_p1m",
	NULL
};

<<<<<<< HEAD
/*
 * Called very early, MMU is off, device-tree isn't unflattened
 */
static int __init mpc830x_rdb_probe(void)
{
	return of_flat_dt_match(of_get_flat_dt_root(), board);
}

=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
machine_device_initcall(mpc830x_rdb, mpc83xx_declare_of_platform_devices);

define_machine(mpc830x_rdb) {
	.name			= "MPC830x RDB",
<<<<<<< HEAD
	.probe			= mpc830x_rdb_probe,
	.setup_arch		= mpc830x_rdb_setup_arch,
=======
	.compatibles		= board,
	.setup_arch		= mpc830x_rdb_setup_arch,
	.discover_phbs		= mpc83xx_setup_pci,
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	.init_IRQ		= mpc83xx_ipic_init_IRQ,
	.get_irq		= ipic_get_irq,
	.restart		= mpc83xx_restart,
	.time_init		= mpc83xx_time_init,
<<<<<<< HEAD
	.calibrate_decr		= generic_calibrate_decr,
=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	.progress		= udbg_progress,
};
