<<<<<<< HEAD
=======
// SPDX-License-Identifier: GPL-2.0-or-later
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
/*
 * misc setup functions for MPC83xx
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
#include <linux/of_platform.h>
#include <linux/pci.h>

<<<<<<< HEAD
#include <asm/io.h>
#include <asm/hw_irq.h>
#include <asm/ipic.h>
#include <asm/qe_ic.h>
#include <sysdev/fsl_soc.h>
#include <sysdev/fsl_pci.h>

=======
#include <asm/debug.h>
#include <asm/io.h>
#include <asm/hw_irq.h>
#include <asm/ipic.h>
#include <asm/fixmap.h>

#include <sysdev/fsl_soc.h>
#include <sysdev/fsl_pci.h>

#include <mm/mmu_decl.h>

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#include "mpc83xx.h"

static __be32 __iomem *restart_reg_base;

static int __init mpc83xx_restart_init(void)
{
	/* map reset restart_reg_baseister space */
	restart_reg_base = ioremap(get_immrbase() + 0x900, 0xff);

	return 0;
}

arch_initcall(mpc83xx_restart_init);

<<<<<<< HEAD
void mpc83xx_restart(char *cmd)
=======
void __noreturn mpc83xx_restart(char *cmd)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
{
#define RST_OFFSET	0x00000900
#define RST_PROT_REG	0x00000018
#define RST_CTRL_REG	0x0000001c

	local_irq_disable();

	if (restart_reg_base) {
		/* enable software reset "RSTE" */
		out_be32(restart_reg_base + (RST_PROT_REG >> 2), 0x52535445);

		/* set software hard reset */
		out_be32(restart_reg_base + (RST_CTRL_REG >> 2), 0x2);
	} else {
		printk (KERN_EMERG "Error: Restart registers not mapped, spinning!\n");
	}

	for (;;) ;
}

long __init mpc83xx_time_init(void)
{
#define SPCR_OFFSET	0x00000110
#define SPCR_TBEN	0x00400000
	__be32 __iomem *spcr = ioremap(get_immrbase() + SPCR_OFFSET, 4);
	__be32 tmp;

	tmp = in_be32(spcr);
	out_be32(spcr, tmp | SPCR_TBEN);

	iounmap(spcr);

	return 0;
}

void __init mpc83xx_ipic_init_IRQ(void)
{
	struct device_node *np;

	/* looking for fsl,pq2pro-pic which is asl compatible with fsl,ipic */
	np = of_find_compatible_node(NULL, NULL, "fsl,ipic");
	if (!np)
		np = of_find_node_by_type(NULL, "ipic");
	if (!np)
		return;

	ipic_init(np, 0);

	of_node_put(np);

	/* Initialize the default interrupt mapping priorities,
	 * in case the boot rom changed something on us.
	 */
	ipic_set_default_priority();
}

<<<<<<< HEAD
#ifdef CONFIG_QUICC_ENGINE
void __init mpc83xx_qe_init_IRQ(void)
{
	struct device_node *np;

	np = of_find_compatible_node(NULL, NULL, "fsl,qe-ic");
	if (!np) {
		np = of_find_node_by_type(NULL, "qeic");
		if (!np)
			return;
	}
	qe_ic_init(np, 0, qe_ic_cascade_low_ipic, qe_ic_cascade_high_ipic);
	of_node_put(np);
}

void __init mpc83xx_ipic_and_qe_init_IRQ(void)
{
	mpc83xx_ipic_init_IRQ();
	mpc83xx_qe_init_IRQ();
}
#endif /* CONFIG_QUICC_ENGINE */

static struct of_device_id __initdata of_bus_ids[] = {
=======
static const struct of_device_id of_bus_ids[] __initconst = {
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	{ .type = "soc", },
	{ .compatible = "soc", },
	{ .compatible = "simple-bus" },
	{ .compatible = "gianfar" },
	{ .compatible = "gpio-leds", },
	{ .type = "qe", },
	{ .compatible = "fsl,qe", },
	{},
};

int __init mpc83xx_declare_of_platform_devices(void)
{
	of_platform_bus_probe(NULL, of_bus_ids, NULL);
	return 0;
}

#ifdef CONFIG_PCI
void __init mpc83xx_setup_pci(void)
{
	struct device_node *np;

	for_each_compatible_node(np, "pci", "fsl,mpc8349-pci")
		mpc83xx_add_bridge(np);
	for_each_compatible_node(np, "pci", "fsl,mpc8314-pcie")
		mpc83xx_add_bridge(np);
}
#endif
<<<<<<< HEAD
=======

void __init mpc83xx_setup_arch(void)
{
	phys_addr_t immrbase = get_immrbase();
	int immrsize = IS_ALIGNED(immrbase, SZ_2M) ? SZ_2M : SZ_1M;
	unsigned long va = fix_to_virt(FIX_IMMR_BASE);

	if (ppc_md.progress)
		ppc_md.progress("mpc83xx_setup_arch()", 0);

	setbat(-1, va, immrbase, immrsize, PAGE_KERNEL_NCG);
	update_bats();
}

int machine_check_83xx(struct pt_regs *regs)
{
	u32 mask = 1 << (31 - IPIC_MCP_WDT);

	if (!(regs->msr & SRR1_MCE_MCP) || !(ipic_get_mcp_status() & mask))
		return machine_check_generic(regs);
	ipic_clear_mcp_status(mask);

	if (debugger_fault_handler(regs))
		return 1;

	die("Watchdog NMI Reset", regs, 0);

	return 1;
}
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
