<<<<<<< HEAD
=======
// SPDX-License-Identifier: GPL-2.0-or-later
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
/*
 * AmigaOne platform setup
 *
 * Copyright 2008 Gerhard Pircher (gerhard_pircher@gmx.net)
 *
 *   Based on original amigaone_setup.c source code
 * Copyright 2003 by Hans-Joerg Frieden and Thomas Frieden
<<<<<<< HEAD
 *
 * This program is free software; you can redistribute  it and/or modify it
 * under  the terms of  the GNU General  Public License as published by the
 * Free Software Foundation;  either version 2 of the  License, or (at your
 * option) any later version.
 */

=======
 */

#include <linux/irqdomain.h>
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#include <linux/kernel.h>
#include <linux/of.h>
#include <linux/of_address.h>
#include <linux/seq_file.h>
#include <generated/utsrelease.h>

#include <asm/machdep.h>
#include <asm/cputable.h>
#include <asm/pci-bridge.h>
#include <asm/i8259.h>
#include <asm/time.h>
#include <asm/udbg.h>
<<<<<<< HEAD

extern void __flush_disable_L1(void);

void amigaone_show_cpuinfo(struct seq_file *m)
=======
#include <asm/dma.h>

extern void __flush_disable_L1(void);

static void amigaone_show_cpuinfo(struct seq_file *m)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
{
	seq_printf(m, "vendor\t\t: Eyetech Ltd.\n");
}

static int __init amigaone_add_bridge(struct device_node *dev)
{
	const u32 *cfg_addr, *cfg_data;
	int len;
	const int *bus_range;
	struct pci_controller *hose;

<<<<<<< HEAD
	printk(KERN_INFO "Adding PCI host bridge %s\n", dev->full_name);
=======
	printk(KERN_INFO "Adding PCI host bridge %pOF\n", dev);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	cfg_addr = of_get_address(dev, 0, NULL, NULL);
	cfg_data = of_get_address(dev, 1, NULL, NULL);
	if ((cfg_addr == NULL) || (cfg_data == NULL))
		return -ENODEV;

	bus_range = of_get_property(dev, "bus-range", &len);
	if ((bus_range == NULL) || (len < 2 * sizeof(int)))
<<<<<<< HEAD
		printk(KERN_WARNING "Can't get bus-range for %s, assume"
		       " bus 0\n", dev->full_name);
=======
		printk(KERN_WARNING "Can't get bus-range for %pOF, assume"
		       " bus 0\n", dev);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	hose = pcibios_alloc_controller(dev);
	if (hose == NULL)
		return -ENOMEM;

	hose->first_busno = bus_range ? bus_range[0] : 0;
	hose->last_busno = bus_range ? bus_range[1] : 0xff;

	setup_indirect_pci(hose, cfg_addr[0], cfg_data[0], 0);

	/* Interpret the "ranges" property */
	/* This also maps the I/O region and sets isa_io/mem_base */
	pci_process_bridge_OF_ranges(hose, dev, 1);

	return 0;
}

<<<<<<< HEAD
void __init amigaone_setup_arch(void)
=======
static void __init amigaone_setup_arch(void)
{
	if (ppc_md.progress)
		ppc_md.progress("Linux/PPC "UTS_RELEASE"\n", 0);
}

static void __init amigaone_discover_phbs(void)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
{
	struct device_node *np;
	int phb = -ENODEV;

	/* Lookup PCI host bridges. */
	for_each_compatible_node(np, "pci", "mai-logic,articia-s")
		phb = amigaone_add_bridge(np);

	BUG_ON(phb != 0);
<<<<<<< HEAD

	if (ppc_md.progress)
		ppc_md.progress("Linux/PPC "UTS_RELEASE"\n", 0);
}

void __init amigaone_init_IRQ(void)
=======
}

static void __init amigaone_init_IRQ(void)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
{
	struct device_node *pic, *np = NULL;
	const unsigned long *prop = NULL;
	unsigned long int_ack = 0;

	/* Search for ISA interrupt controller. */
	pic = of_find_compatible_node(NULL, "interrupt-controller",
	                              "pnpPNP,000");
	BUG_ON(pic == NULL);

	/* Look for interrupt acknowledge address in the PCI root node. */
	np = of_find_compatible_node(NULL, "pci", "mai-logic,articia-s");
	if (np) {
		prop = of_get_property(np, "8259-interrupt-acknowledge", NULL);
		if (prop)
			int_ack = prop[0];
		of_node_put(np);
	}

	if (int_ack == 0)
		printk(KERN_WARNING "Cannot find PCI interrupt acknowledge"
		       " address, polling\n");

	i8259_init(pic, int_ack);
	ppc_md.get_irq = i8259_irq;
	irq_set_default_host(i8259_get_host());
}

static int __init request_isa_regions(void)
{
	request_region(0x00, 0x20, "dma1");
	request_region(0x40, 0x20, "timer");
	request_region(0x80, 0x10, "dma page reg");
	request_region(0xc0, 0x20, "dma2");

	return 0;
}
machine_device_initcall(amigaone, request_isa_regions);

<<<<<<< HEAD
void amigaone_restart(char *cmd)
=======
static void __noreturn amigaone_restart(char *cmd)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
{
	local_irq_disable();

	/* Flush and disable caches. */
	__flush_disable_L1();

        /* Set SRR0 to the reset vector and turn on MSR_IP. */
	mtspr(SPRN_SRR0, 0xfff00100);
	mtspr(SPRN_SRR1, MSR_IP);

	/* Do an rfi to jump back to firmware. */
	__asm__ __volatile__("rfi" : : : "memory");

	/* Not reached. */
	while (1);
}

static int __init amigaone_probe(void)
{
<<<<<<< HEAD
	unsigned long root = of_get_flat_dt_root();

	if (of_flat_dt_is_compatible(root, "eyetech,amigaone")) {
		/*
		 * Coherent memory access cause complete system lockup! Thus
		 * disable this CPU feature, even if the CPU needs it.
		 */
		cur_cpu_spec->cpu_features &= ~CPU_FTR_NEED_COHERENT;

		ISA_DMA_THRESHOLD = 0x00ffffff;
		DMA_MODE_READ = 0x44;
		DMA_MODE_WRITE = 0x48;

		return 1;
	}

	return 0;
=======
	/*
	 * Coherent memory access cause complete system lockup! Thus
	 * disable this CPU feature, even if the CPU needs it.
	 */
	cur_cpu_spec->cpu_features &= ~CPU_FTR_NEED_COHERENT;

	DMA_MODE_READ = 0x44;
	DMA_MODE_WRITE = 0x48;

	return 1;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

define_machine(amigaone) {
	.name			= "AmigaOne",
<<<<<<< HEAD
	.probe			= amigaone_probe,
	.setup_arch		= amigaone_setup_arch,
	.show_cpuinfo		= amigaone_show_cpuinfo,
	.init_IRQ		= amigaone_init_IRQ,
	.restart		= amigaone_restart,
	.calibrate_decr		= generic_calibrate_decr,
=======
	.compatible		= "eyetech,amigaone",
	.probe			= amigaone_probe,
	.setup_arch		= amigaone_setup_arch,
	.discover_phbs		= amigaone_discover_phbs,
	.show_cpuinfo		= amigaone_show_cpuinfo,
	.init_IRQ		= amigaone_init_IRQ,
	.restart		= amigaone_restart,
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	.progress		= udbg_progress,
};
