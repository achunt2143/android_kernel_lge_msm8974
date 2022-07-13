<<<<<<< HEAD
=======
// SPDX-License-Identifier: GPL-2.0-or-later
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
/*
 * PCI Dynamic LPAR, PCI Hot Plug and PCI EEH recovery code
 * for RPA-compliant PPC64 platform.
 * Copyright (C) 2003 Linda Xie <lxie@us.ibm.com>
 * Copyright (C) 2005 International Business Machines
 *
 * Updates, 2005, John Rose <johnrose@austin.ibm.com>
 * Updates, 2005, Linas Vepstas <linas@austin.ibm.com>
<<<<<<< HEAD
 *
 * All rights reserved.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or (at
 * your option) any later version.
 *
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY OR FITNESS FOR A PARTICULAR PURPOSE, GOOD TITLE or
 * NON INFRINGEMENT.  See the GNU General Public License for more
 * details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 */

#include <linux/pci.h>
#include <linux/export.h>
#include <asm/pci-bridge.h>
#include <asm/ppc-pci.h>
#include <asm/firmware.h>
#include <asm/eeh.h>

<<<<<<< HEAD
static struct pci_bus *
find_bus_among_children(struct pci_bus *bus,
                        struct device_node *dn)
{
	struct pci_bus *child = NULL;
	struct list_head *tmp;
	struct device_node *busdn;

	busdn = pci_bus_to_OF_node(bus);
	if (busdn == dn)
		return bus;

	list_for_each(tmp, &bus->children) {
		child = find_bus_among_children(pci_bus_b(tmp), dn);
		if (child)
			break;
	};
	return child;
}

struct pci_bus *
pcibios_find_pci_bus(struct device_node *dn)
{
	struct pci_dn *pdn = dn->data;

	if (!pdn  || !pdn->phb || !pdn->phb->bus)
		return NULL;

	return find_bus_among_children(pdn->phb->bus, dn);
}
EXPORT_SYMBOL_GPL(pcibios_find_pci_bus);

/**
 * pcibios_remove_pci_devices - remove all devices under this bus
 *
 * Remove all of the PCI devices under this bus both from the
 * linux pci device tree, and from the powerpc EEH address cache.
 */
void pcibios_remove_pci_devices(struct pci_bus *bus)
{
 	struct pci_dev *dev, *tmp;
	struct pci_bus *child_bus;

	/* First go down child busses */
	list_for_each_entry(child_bus, &bus->children, node)
		pcibios_remove_pci_devices(child_bus);

	pr_debug("PCI: Removing devices on bus %04x:%02x\n",
		 pci_domain_nr(bus),  bus->number);
	list_for_each_entry_safe(dev, tmp, &bus->devices, bus_list) {
		pr_debug("     * Removing %s...\n", pci_name(dev));
		eeh_remove_bus_device(dev);
 		pci_stop_and_remove_bus_device(dev);
 	}
}
EXPORT_SYMBOL_GPL(pcibios_remove_pci_devices);

/**
 * pcibios_add_pci_devices - adds new pci devices to bus
 *
 * This routine will find and fixup new pci devices under
 * the indicated bus. This routine presumes that there
 * might already be some devices under this bridge, so
 * it carefully tries to add only new devices.  (And that
 * is how this routine differs from other, similar pcibios
 * routines.)
 */
void pcibios_add_pci_devices(struct pci_bus * bus)
{
	int slotno, num, mode, pass, max;
	struct pci_dev *dev;
	struct device_node *dn = pci_bus_to_OF_node(bus);

	eeh_add_device_tree_early(dn);

	mode = PCI_PROBE_NORMAL;
	if (ppc_md.pci_probe_mode)
		mode = ppc_md.pci_probe_mode(bus);

	if (mode == PCI_PROBE_DEVTREE) {
		/* use ofdt-based probe */
		of_rescan_bus(dn, bus);
	} else if (mode == PCI_PROBE_NORMAL) {
		/* use legacy probe */
		slotno = PCI_SLOT(PCI_DN(dn->child)->devfn);
		num = pci_scan_slot(bus, PCI_DEVFN(slotno, 0));
		if (!num)
			return;
		pcibios_setup_bus_devices(bus);
		max = bus->secondary;
		for (pass=0; pass < 2; pass++)
			list_for_each_entry(dev, &bus->devices, bus_list) {
			if (dev->hdr_type == PCI_HEADER_TYPE_BRIDGE ||
			    dev->hdr_type == PCI_HEADER_TYPE_CARDBUS)
				max = pci_scan_bridge(bus, dev, max, pass);
		}
	}
	pcibios_finish_adding_to_bus(bus);
}
EXPORT_SYMBOL_GPL(pcibios_add_pci_devices);

struct pci_controller * __devinit init_phb_dynamic(struct device_node *dn)
{
	struct pci_controller *phb;

	pr_debug("PCI: Initializing new hotplug PHB %s\n", dn->full_name);
=======
#include "pseries.h"

struct pci_controller *init_phb_dynamic(struct device_node *dn)
{
	struct pci_controller *phb;

	pr_debug("PCI: Initializing new hotplug PHB %pOF\n", dn);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	phb = pcibios_alloc_controller(dn);
	if (!phb)
		return NULL;
	rtas_setup_phb(phb);
	pci_process_bridge_OF_ranges(phb, dn, 0);
<<<<<<< HEAD

	pci_devs_phb_init_dynamic(phb);

	/* Create EEH devices for the PHB */
	eeh_dev_phb_init_dynamic(phb);

	if (dn->child)
		eeh_add_device_tree_early(dn);
=======
	phb->controller_ops = pseries_pci_controller_ops;

	pci_devs_phb_init_dynamic(phb);

	pseries_msi_allocate_domains(phb);

	ppc_iommu_register_device(phb);

	/* Create EEH devices for the PHB */
	eeh_phb_pe_create(phb);

	if (dn->child)
		pseries_eeh_init_edev_recursive(PCI_DN(dn));
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	pcibios_scan_phb(phb);
	pcibios_finish_adding_to_bus(phb->bus);

	return phb;
}
EXPORT_SYMBOL_GPL(init_phb_dynamic);

/* RPA-specific bits for removing PHBs */
int remove_phb_dynamic(struct pci_controller *phb)
{
	struct pci_bus *b = phb->bus;
<<<<<<< HEAD
=======
	struct pci_host_bridge *host_bridge = to_pci_host_bridge(b->bridge);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	struct resource *res;
	int rc, i;

	pr_debug("PCI: Removing PHB %04x:%02x...\n",
		 pci_domain_nr(b), b->number);

	/* We cannot to remove a root bus that has children */
	if (!(list_empty(&b->children) && list_empty(&b->devices)))
		return -EBUSY;

	/* We -know- there aren't any child devices anymore at this stage
	 * and thus, we can safely unmap the IO space as it's not in use
	 */
	res = &phb->io_resource;
	if (res->flags & IORESOURCE_IO) {
		rc = pcibios_unmap_io_space(b);
		if (rc) {
			printk(KERN_ERR "%s: failed to unmap IO on bus %s\n",
			       __func__, b->name);
			return 1;
		}
	}

<<<<<<< HEAD
	/* Unregister the bridge device from sysfs and remove the PCI bus */
	device_unregister(b->bridge);
	phb->bus = NULL;
	pci_remove_bus(b);
=======
	ppc_iommu_unregister_device(phb);

	pseries_msi_free_domains(phb);

	/* Keep a reference so phb isn't freed yet */
	get_device(&host_bridge->dev);

	/* Remove the PCI bus and unregister the bridge device from sysfs */
	phb->bus = NULL;
	pci_remove_bus(b);
	host_bridge->bus = NULL;
	device_unregister(&host_bridge->dev);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	/* Now release the IO resource */
	if (res->flags & IORESOURCE_IO)
		release_resource(res);

	/* Release memory resources */
	for (i = 0; i < 3; ++i) {
		res = &phb->mem_resources[i];
		if (!(res->flags & IORESOURCE_MEM))
			continue;
		release_resource(res);
	}

<<<<<<< HEAD
	/* Free pci_controller data structure */
	pcibios_free_controller(phb);
=======
	/*
	 * The pci_controller data structure is freed by
	 * the pcibios_free_controller_deferred() callback;
	 * see pseries_root_bridge_prepare().
	 */
	put_device(&host_bridge->dev);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	return 0;
}
EXPORT_SYMBOL_GPL(remove_phb_dynamic);
