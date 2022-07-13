<<<<<<< HEAD
=======
// SPDX-License-Identifier: GPL-2.0-or-later
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
/*
 *    Copyright (C) 2006 Benjamin Herrenschmidt, IBM Corp.
 *			 <benh@kernel.crashing.org>
 *    and		 Arnd Bergmann, IBM Corp.
<<<<<<< HEAD
 *
 *  This program is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU General Public License
 *  as published by the Free Software Foundation; either version
 *  2 of the License, or (at your option) any later version.
 *
=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 */

#undef DEBUG

#include <linux/string.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/export.h>
#include <linux/mod_devicetable.h>
#include <linux/pci.h>
<<<<<<< HEAD
#include <linux/of.h>
#include <linux/of_device.h>
#include <linux/of_platform.h>
=======
#include <linux/platform_device.h>
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#include <linux/atomic.h>

#include <asm/errno.h>
#include <asm/topology.h>
#include <asm/pci-bridge.h>
#include <asm/ppc-pci.h>
#include <asm/eeh.h>

#ifdef CONFIG_PPC_OF_PLATFORM_PCI

/* The probing of PCI controllers from of_platform is currently
 * 64 bits only, mostly due to gratuitous differences between
 * the 32 and 64 bits PCI code on PowerPC and the 32 bits one
 * lacking some bits needed here.
 */

<<<<<<< HEAD
static int __devinit of_pci_phb_probe(struct platform_device *dev)
=======
static int of_pci_phb_probe(struct platform_device *dev)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
{
	struct pci_controller *phb;

	/* Check if we can do that ... */
	if (ppc_md.pci_setup_phb == NULL)
		return -ENODEV;

<<<<<<< HEAD
	pr_info("Setting up PCI bus %s\n", dev->dev.of_node->full_name);
=======
	pr_info("Setting up PCI bus %pOF\n", dev->dev.of_node);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	/* Alloc and setup PHB data structure */
	phb = pcibios_alloc_controller(dev->dev.of_node);
	if (!phb)
		return -ENODEV;

	/* Setup parent in sysfs */
	phb->parent = &dev->dev;

	/* Setup the PHB using arch provided callback */
	if (ppc_md.pci_setup_phb(phb)) {
		pcibios_free_controller(phb);
		return -ENODEV;
	}

	/* Process "ranges" property */
	pci_process_bridge_OF_ranges(phb, dev->dev.of_node, 0);

	/* Init pci_dn data structures */
	pci_devs_phb_init_dynamic(phb);

<<<<<<< HEAD
	/* Create EEH devices for the PHB */
	eeh_dev_phb_init_dynamic(phb);

	/* Register devices with EEH */
#ifdef CONFIG_EEH
	if (dev->dev.of_node->child)
		eeh_add_device_tree_early(dev->dev.of_node);
#endif /* CONFIG_EEH */
=======
	/* Create EEH PE for the PHB */
	eeh_phb_pe_create(phb);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	/* Scan the bus */
	pcibios_scan_phb(phb);
	if (phb->bus == NULL)
		return -ENXIO;

	/* Claim resources. This might need some rework as well depending
<<<<<<< HEAD
	 * wether we are doing probe-only or not, like assigning unassigned
=======
	 * whether we are doing probe-only or not, like assigning unassigned
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	 * resources etc...
	 */
	pcibios_claim_one_bus(phb->bus);

<<<<<<< HEAD
	/* Finish EEH setup */
#ifdef CONFIG_EEH
	eeh_add_device_tree_late(phb->bus);
#endif

=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	/* Add probed PCI devices to the device model */
	pci_bus_add_devices(phb->bus);

	return 0;
}

<<<<<<< HEAD
static struct of_device_id of_pci_phb_ids[] = {
=======
static const struct of_device_id of_pci_phb_ids[] = {
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	{ .type = "pci", },
	{ .type = "pcix", },
	{ .type = "pcie", },
	{ .type = "pciex", },
	{ .type = "ht", },
	{}
};

static struct platform_driver of_pci_phb_driver = {
	.probe = of_pci_phb_probe,
	.driver = {
		.name = "of-pci",
<<<<<<< HEAD
		.owner = THIS_MODULE,
=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		.of_match_table = of_pci_phb_ids,
	},
};

<<<<<<< HEAD
static __init int of_pci_phb_init(void)
{
	return platform_driver_register(&of_pci_phb_driver);
}

device_initcall(of_pci_phb_init);
=======
builtin_platform_driver(of_pci_phb_driver);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

#endif /* CONFIG_PPC_OF_PLATFORM_PCI */
