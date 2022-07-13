<<<<<<< HEAD
=======
// SPDX-License-Identifier: GPL-2.0-or-later
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
/*
 * Sam440ep board specific routines based off bamboo.c code
 * original copyrights below
 *
 * Wade Farnsworth <wfarnsworth@mvista.com>
 * Copyright 2004 MontaVista Software Inc.
 *
 * Rewritten and ported to the merged powerpc tree:
 * Josh Boyer <jwboyer@linux.vnet.ibm.com>
 * Copyright 2007 IBM Corporation
 *
 * Modified from bamboo.c for sam440ep:
 * Copyright 2008 Giuseppe Coviello <gicoviello@gmail.com>
<<<<<<< HEAD
 *
 * This program is free software; you can redistribute  it and/or modify it
 * under  the terms of  the GNU General  Public License as published by the
 * Free Software Foundation;  either version 2 of the  License, or (at your
 * option) any later version.
=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 */
#include <linux/init.h>
#include <linux/of_platform.h>

#include <asm/machdep.h>
<<<<<<< HEAD
#include <asm/prom.h>
=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#include <asm/udbg.h>
#include <asm/time.h>
#include <asm/uic.h>
#include <asm/pci-bridge.h>
#include <asm/ppc4xx.h>
#include <linux/i2c.h>

<<<<<<< HEAD
static __initdata struct of_device_id sam440ep_of_bus[] = {
=======
static const struct of_device_id sam440ep_of_bus[] __initconst = {
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	{ .compatible = "ibm,plb4", },
	{ .compatible = "ibm,opb", },
	{ .compatible = "ibm,ebc", },
	{},
};

static int __init sam440ep_device_probe(void)
{
	of_platform_bus_probe(NULL, sam440ep_of_bus, NULL);

	return 0;
}
machine_device_initcall(sam440ep, sam440ep_device_probe);

static int __init sam440ep_probe(void)
{
<<<<<<< HEAD
	unsigned long root = of_get_flat_dt_root();

	if (!of_flat_dt_is_compatible(root, "acube,sam440ep"))
		return 0;

=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	pci_set_flags(PCI_REASSIGN_ALL_RSRC);

	return 1;
}

define_machine(sam440ep) {
	.name 			= "Sam440ep",
<<<<<<< HEAD
=======
	.compatible		= "acube,sam440ep",
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	.probe 			= sam440ep_probe,
	.progress 		= udbg_progress,
	.init_IRQ 		= uic_init_tree,
	.get_irq 		= uic_get_irq,
	.restart		= ppc4xx_reset_system,
<<<<<<< HEAD
	.calibrate_decr 	= generic_calibrate_decr,
=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
};

static struct i2c_board_info sam440ep_rtc_info = {
	.type = "m41st85",
	.addr = 0x68,
	.irq = -1,
};

<<<<<<< HEAD
static int sam440ep_setup_rtc(void)
=======
static int __init sam440ep_setup_rtc(void)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
{
	return i2c_register_board_info(0, &sam440ep_rtc_info, 1);
}
machine_device_initcall(sam440ep, sam440ep_setup_rtc);
