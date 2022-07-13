<<<<<<< HEAD
=======
// SPDX-License-Identifier: GPL-2.0-or-later
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
/*
 * Based on MPC8560 ADS and arch/ppc tqm85xx ports
 *
 * Maintained by Kumar Gala (see MAINTAINERS for contact information)
 *
 * Copyright 2008 Freescale Semiconductor Inc.
 *
 * Copyright (c) 2005-2006 DENX Software Engineering
 * Stefan Roese <sr@denx.de>
 *
 * Based on original work by
 * 	Kumar Gala <kumar.gala@freescale.com>
 *      Copyright 2004 Freescale Semiconductor Inc.
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
#include <linux/pci.h>
#include <linux/kdev_t.h>
#include <linux/delay.h>
#include <linux/seq_file.h>
<<<<<<< HEAD
#include <linux/of_platform.h>
=======
#include <linux/of.h>
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

#include <asm/time.h>
#include <asm/machdep.h>
#include <asm/pci-bridge.h>
#include <asm/mpic.h>
<<<<<<< HEAD
#include <asm/prom.h>
=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#include <mm/mmu_decl.h>
#include <asm/udbg.h>

#include <sysdev/fsl_soc.h>
#include <sysdev/fsl_pci.h>

#include "mpc85xx.h"

#ifdef CONFIG_CPM2
#include <asm/cpm2.h>
#endif /* CONFIG_CPM2 */

static void __init tqm85xx_pic_init(void)
{
	struct mpic *mpic = mpic_alloc(NULL, 0,
			MPIC_BIG_ENDIAN,
			0, 256, " OpenPIC  ");
	BUG_ON(mpic == NULL);
	mpic_init(mpic);

	mpc85xx_cpm2_pic_init();
}

/*
 * Setup the architecture
 */
static void __init tqm85xx_setup_arch(void)
{
<<<<<<< HEAD
#ifdef CONFIG_PCI
	struct device_node *np;
#endif

=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	if (ppc_md.progress)
		ppc_md.progress("tqm85xx_setup_arch()", 0);

#ifdef CONFIG_CPM2
	cpm2_reset();
#endif

<<<<<<< HEAD
#ifdef CONFIG_PCI
	for_each_node_by_type(np, "pci") {
		if (of_device_is_compatible(np, "fsl,mpc8540-pci") ||
		    of_device_is_compatible(np, "fsl,mpc8548-pcie")) {
			struct resource rsrc;
			if (!of_address_to_resource(np, 0, &rsrc)) {
				if ((rsrc.start & 0xfffff) == 0x8000)
					fsl_add_bridge(np, 1);
				else
					fsl_add_bridge(np, 0);
			}
		}
	}
#endif
=======
	fsl_pci_assign_primary();
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

static void tqm85xx_show_cpuinfo(struct seq_file *m)
{
	uint pvid, svid, phid1;

	pvid = mfspr(SPRN_PVR);
	svid = mfspr(SPRN_SVR);

	seq_printf(m, "Vendor\t\t: TQ Components\n");
	seq_printf(m, "PVR\t\t: 0x%x\n", pvid);
	seq_printf(m, "SVR\t\t: 0x%x\n", svid);

	/* Display cpu Pll setting */
	phid1 = mfspr(SPRN_HID1);
	seq_printf(m, "PLL setting\t: 0x%x\n", ((phid1 >> 24) & 0x3f));
}

<<<<<<< HEAD
static void __init tqm85xx_ti1520_fixup(struct pci_dev *pdev)
=======
static void tqm85xx_ti1520_fixup(struct pci_dev *pdev)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
{
	unsigned int val;

	/* Do not do the fixup on other platforms! */
	if (!machine_is(tqm85xx))
		return;

	dev_info(&pdev->dev, "Using TI 1520 fixup on TQM85xx\n");

	/*
	 * Enable P2CCLK bit in system control register
	 * to enable CLOCK output to power chip
	 */
	pci_read_config_dword(pdev, 0x80, &val);
	pci_write_config_dword(pdev, 0x80, val | (1 << 27));

}
DECLARE_PCI_FIXUP_HEADER(PCI_VENDOR_ID_TI, PCI_DEVICE_ID_TI_1520,
		tqm85xx_ti1520_fixup);

<<<<<<< HEAD
machine_device_initcall(tqm85xx, mpc85xx_common_publish_devices);

static const char *board[] __initdata = {
=======
machine_arch_initcall(tqm85xx, mpc85xx_common_publish_devices);

static const char * const board[] __initconst = {
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	"tqc,tqm8540",
	"tqc,tqm8541",
	"tqc,tqm8548",
	"tqc,tqm8555",
	"tqc,tqm8560",
	NULL
};

<<<<<<< HEAD
/*
 * Called very early, device-tree isn't unflattened
 */
static int __init tqm85xx_probe(void)
{
	return of_flat_dt_match(of_get_flat_dt_root(), board);
}

define_machine(tqm85xx) {
	.name			= "TQM85xx",
	.probe			= tqm85xx_probe,
=======
define_machine(tqm85xx) {
	.name			= "TQM85xx",
	.compatibles		= board,
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	.setup_arch		= tqm85xx_setup_arch,
	.init_IRQ		= tqm85xx_pic_init,
	.show_cpuinfo		= tqm85xx_show_cpuinfo,
	.get_irq		= mpic_get_irq,
<<<<<<< HEAD
	.restart		= fsl_rstcr_restart,
	.calibrate_decr		= generic_calibrate_decr,
=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	.progress		= udbg_progress,
};
