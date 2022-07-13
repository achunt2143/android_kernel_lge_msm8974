<<<<<<< HEAD
=======
// SPDX-License-Identifier: GPL-2.0-or-later
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
/*
 * arch/powerpc/platforms/83xx/mpc834x_itx.c
 *
 * MPC834x ITX board specific routines
 *
 * Maintainer: Kumar Gala <galak@kernel.crashing.org>
<<<<<<< HEAD
 *
 * This program is free software; you can redistribute  it and/or modify it
 * under  the terms of  the GNU General  Public License as published by the
 * Free Software Foundation;  either version 2 of the  License, or (at your
 * option) any later version.
=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 */

#include <linux/stddef.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/errno.h>
#include <linux/reboot.h>
#include <linux/pci.h>
#include <linux/kdev_t.h>
#include <linux/major.h>
#include <linux/console.h>
#include <linux/delay.h>
#include <linux/seq_file.h>
#include <linux/root_dev.h>
#include <linux/of_platform.h>

#include <linux/atomic.h>
#include <asm/time.h>
#include <asm/io.h>
#include <asm/machdep.h>
#include <asm/ipic.h>
#include <asm/irq.h>
<<<<<<< HEAD
#include <asm/prom.h>
=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#include <asm/udbg.h>
#include <sysdev/fsl_soc.h>
#include <sysdev/fsl_pci.h>

#include "mpc83xx.h"

<<<<<<< HEAD
static struct of_device_id __initdata mpc834x_itx_ids[] = {
=======
static const struct of_device_id mpc834x_itx_ids[] __initconst = {
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	{ .compatible = "fsl,pq2pro-localbus", },
	{},
};

static int __init mpc834x_itx_declare_of_platform_devices(void)
{
	mpc83xx_declare_of_platform_devices();
	return of_platform_bus_probe(NULL, mpc834x_itx_ids, NULL);
}
machine_device_initcall(mpc834x_itx, mpc834x_itx_declare_of_platform_devices);

/* ************************************************************************
 *
 * Setup the architecture
 *
 */
static void __init mpc834x_itx_setup_arch(void)
{
<<<<<<< HEAD
	if (ppc_md.progress)
		ppc_md.progress("mpc834x_itx_setup_arch()", 0);

	mpc83xx_setup_pci();
=======
	mpc83xx_setup_arch();
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	mpc834x_usb_cfg();
}

<<<<<<< HEAD
/*
 * Called very early, MMU is off, device-tree isn't unflattened
 */
static int __init mpc834x_itx_probe(void)
{
        unsigned long root = of_get_flat_dt_root();

        return of_flat_dt_is_compatible(root, "MPC834xMITX");
}

define_machine(mpc834x_itx) {
	.name			= "MPC834x ITX",
	.probe			= mpc834x_itx_probe,
	.setup_arch		= mpc834x_itx_setup_arch,
=======
define_machine(mpc834x_itx) {
	.name			= "MPC834x ITX",
	.compatible		= "MPC834xMITX",
	.setup_arch		= mpc834x_itx_setup_arch,
	.discover_phbs  	= mpc83xx_setup_pci,
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
