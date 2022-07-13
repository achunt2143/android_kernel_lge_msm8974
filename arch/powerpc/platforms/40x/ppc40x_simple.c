<<<<<<< HEAD
=======
// SPDX-License-Identifier: GPL-2.0-only
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
/*
 * Generic PowerPC 40x platform support
 *
 * Copyright 2008 IBM Corporation
 *
<<<<<<< HEAD
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the
 * Free Software Foundation; version 2 of the License.
 *
=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 * This implements simple platform support for PowerPC 44x chips.  This is
 * mostly used for eval boards or other simple and "generic" 44x boards.  If
 * your board has custom functions or hardware, then you will likely want to
 * implement your own board.c file to accommodate it.
 */

#include <asm/machdep.h>
#include <asm/pci-bridge.h>
#include <asm/ppc4xx.h>
<<<<<<< HEAD
#include <asm/prom.h>
=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#include <asm/time.h>
#include <asm/udbg.h>
#include <asm/uic.h>

#include <linux/init.h>
#include <linux/of_platform.h>

<<<<<<< HEAD
static __initdata struct of_device_id ppc40x_of_bus[] = {
=======
static const struct of_device_id ppc40x_of_bus[] __initconst = {
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	{ .compatible = "ibm,plb3", },
	{ .compatible = "ibm,plb4", },
	{ .compatible = "ibm,opb", },
	{ .compatible = "ibm,ebc", },
	{ .compatible = "simple-bus", },
	{},
};

static int __init ppc40x_device_probe(void)
{
	of_platform_bus_probe(NULL, ppc40x_of_bus, NULL);

	return 0;
}
machine_device_initcall(ppc40x_simple, ppc40x_device_probe);

/* This is the list of boards that can be supported by this simple
 * platform code.  This does _not_ mean the boards are compatible,
 * as they most certainly are not from a device tree perspective.
 * However, their differences are handled by the device tree and the
 * drivers and therefore they don't need custom board support files.
 *
 * Again, if your board needs to do things differently then create a
 * board.c file for it rather than adding it to this list.
 */
<<<<<<< HEAD
static const char *board[] __initdata = {
=======
static const char * const board[] __initconst = {
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	"amcc,acadia",
	"amcc,haleakala",
	"amcc,kilauea",
	"amcc,makalu",
	"apm,klondike",
	"est,hotfoot",
	"plathome,obs600",
	NULL
};

static int __init ppc40x_probe(void)
{
<<<<<<< HEAD
	if (of_flat_dt_match(of_get_flat_dt_root(), board)) {
		pci_set_flags(PCI_REASSIGN_ALL_RSRC);
		return 1;
	}

	return 0;
=======
	pci_set_flags(PCI_REASSIGN_ALL_RSRC);
	return 1;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

define_machine(ppc40x_simple) {
	.name = "PowerPC 40x Platform",
<<<<<<< HEAD
=======
	.compatibles = board,
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	.probe = ppc40x_probe,
	.progress = udbg_progress,
	.init_IRQ = uic_init_tree,
	.get_irq = uic_get_irq,
	.restart = ppc4xx_reset_system,
<<<<<<< HEAD
	.calibrate_decr = generic_calibrate_decr,
=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
};
