<<<<<<< HEAD
=======
// SPDX-License-Identifier: GPL-2.0-or-later
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
/*
 * arch/powerpc/platforms/83xx/asp834x.c
 *
 * Analogue & Micro ASP8347 board specific routines
 * clone of mpc834x_itx
 *
 * Copyright 2008 Codehermit
 *
 * Maintainer: Bryan O'Donoghue <bodonoghue@codhermit.ie>
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

#include "mpc83xx.h"

/* ************************************************************************
 *
 * Setup the architecture
 *
 */
static void __init asp834x_setup_arch(void)
{
<<<<<<< HEAD
	if (ppc_md.progress)
		ppc_md.progress("asp834x_setup_arch()", 0);

=======
	mpc83xx_setup_arch();
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	mpc834x_usb_cfg();
}

machine_device_initcall(asp834x, mpc83xx_declare_of_platform_devices);

<<<<<<< HEAD
/*
 * Called very early, MMU is off, device-tree isn't unflattened
 */
static int __init asp834x_probe(void)
{
	unsigned long root = of_get_flat_dt_root();
	return of_flat_dt_is_compatible(root, "analogue-and-micro,asp8347e");
}

define_machine(asp834x) {
	.name			= "ASP8347E",
	.probe			= asp834x_probe,
	.setup_arch		= asp834x_setup_arch,
=======
define_machine(asp834x) {
	.name			= "ASP8347E",
	.compatible		= "analogue-and-micro,asp8347e",
	.setup_arch		= asp834x_setup_arch,
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
