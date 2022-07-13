<<<<<<< HEAD
=======
// SPDX-License-Identifier: GPL-2.0+
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
/*
 * Interface for Dynamic Logical Partitioning of I/O Slots on
 * RPA-compliant PPC64 platform.
 *
 * John Rose <johnrose@austin.ibm.com>
 * Linda Xie <lxie@us.ibm.com>
 *
 * October 2003
 *
 * Copyright (C) 2003 IBM.
<<<<<<< HEAD
 *
 *      This program is free software; you can redistribute it and/or
 *      modify it under the terms of the GNU General Public License
 *      as published by the Free Software Foundation; either version
 *      2 of the License, or (at your option) any later version.
=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 */

#undef DEBUG

#include <linux/init.h>
#include <linux/module.h>
<<<<<<< HEAD
=======
#include <linux/of.h>
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#include <linux/pci.h>
#include <linux/string.h>
#include <linux/vmalloc.h>

#include <asm/pci-bridge.h>
#include <linux/mutex.h>
#include <asm/rtas.h>
#include <asm/vio.h>
<<<<<<< HEAD
=======
#include <linux/firmware.h>
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

#include "../pci.h"
#include "rpaphp.h"
#include "rpadlpar.h"

static DEFINE_MUTEX(rpadlpar_mutex);

#define DLPAR_MODULE_NAME "rpadlpar_io"

#define NODE_TYPE_VIO  1
#define NODE_TYPE_SLOT 2
#define NODE_TYPE_PHB  3

static struct device_node *find_vio_slot_node(char *drc_name)
{
	struct device_node *parent = of_find_node_by_name(NULL, "vdevice");
<<<<<<< HEAD
	struct device_node *dn = NULL;
	char *name;
=======
	struct device_node *dn;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	int rc;

	if (!parent)
		return NULL;

<<<<<<< HEAD
	while ((dn = of_get_next_child(parent, dn))) {
		rc = rpaphp_get_drc_props(dn, NULL, &name, NULL, NULL);
		if ((rc == 0) && (!strcmp(drc_name, name)))
			break;
	}
=======
	for_each_child_of_node(parent, dn) {
		rc = rpaphp_check_drc_props(dn, drc_name, NULL);
		if (rc == 0)
			break;
	}
	of_node_put(parent);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	return dn;
}

/* Find dlpar-capable pci node that contains the specified name and type */
static struct device_node *find_php_slot_pci_node(char *drc_name,
						  char *drc_type)
{
<<<<<<< HEAD
	struct device_node *np = NULL;
	char *name;
	char *type;
	int rc;

	while ((np = of_find_node_by_name(np, "pci"))) {
		rc = rpaphp_get_drc_props(np, NULL, &name, &type, NULL);
		if (rc == 0)
			if (!strcmp(drc_name, name) && !strcmp(drc_type, type))
				break;
=======
	struct device_node *np;
	int rc;

	for_each_node_by_name(np, "pci") {
		rc = rpaphp_check_drc_props(np, drc_name, drc_type);
		if (rc == 0)
			break;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	}

	return np;
}

<<<<<<< HEAD
=======
/* Returns a device_node with its reference count incremented */
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
static struct device_node *find_dlpar_node(char *drc_name, int *node_type)
{
	struct device_node *dn;

	dn = find_php_slot_pci_node(drc_name, "SLOT");
	if (dn) {
		*node_type = NODE_TYPE_SLOT;
		return dn;
	}

	dn = find_php_slot_pci_node(drc_name, "PHB");
	if (dn) {
		*node_type = NODE_TYPE_PHB;
		return dn;
	}

	dn = find_vio_slot_node(drc_name);
	if (dn) {
		*node_type = NODE_TYPE_VIO;
		return dn;
	}

	return NULL;
}

/**
 * find_php_slot - return hotplug slot structure for device node
 * @dn: target &device_node
 *
 * This routine will return the hotplug slot structure
 * for a given device node. Note that built-in PCI slots
 * may be dlpar-able, but not hot-pluggable, so this routine
 * will return NULL for built-in PCI slots.
 */
static struct slot *find_php_slot(struct device_node *dn)
{
<<<<<<< HEAD
	struct list_head *tmp, *n;
	struct slot *slot;

	list_for_each_safe(tmp, n, &rpaphp_slot_head) {
		slot = list_entry(tmp, struct slot, rpaphp_slot_list);
=======
	struct slot *slot, *next;

	list_for_each_entry_safe(slot, next, &rpaphp_slot_head,
				 rpaphp_slot_list) {
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		if (slot->dn == dn)
			return slot;
	}

	return NULL;
}

static struct pci_dev *dlpar_find_new_dev(struct pci_bus *parent,
					struct device_node *dev_dn)
{
	struct pci_dev *tmp = NULL;
	struct device_node *child_dn;

	list_for_each_entry(tmp, &parent->devices, bus_list) {
		child_dn = pci_device_to_OF_node(tmp);
		if (child_dn == dev_dn)
			return tmp;
	}
	return NULL;
}

static void dlpar_pci_add_bus(struct device_node *dn)
{
	struct pci_dn *pdn = PCI_DN(dn);
	struct pci_controller *phb = pdn->phb;
	struct pci_dev *dev = NULL;

<<<<<<< HEAD
	eeh_add_device_tree_early(dn);
=======
	pseries_eeh_init_edev_recursive(pdn);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	/* Add EADS device to PHB bus, adding new entry to bus->devices */
	dev = of_create_pci_dev(dn, phb->bus, pdn->devfn);
	if (!dev) {
<<<<<<< HEAD
		printk(KERN_ERR "%s: failed to create pci dev for %s\n",
				__func__, dn->full_name);
=======
		printk(KERN_ERR "%s: failed to create pci dev for %pOF\n",
				__func__, dn);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		return;
	}

	/* Scan below the new bridge */
<<<<<<< HEAD
	if (dev->hdr_type == PCI_HEADER_TYPE_BRIDGE ||
	    dev->hdr_type == PCI_HEADER_TYPE_CARDBUS)
=======
	if (pci_is_bridge(dev))
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		of_scan_pci_bridge(dev);

	/* Map IO space for child bus, which may or may not succeed */
	pcibios_map_io_space(dev->subordinate);

	/* Finish adding it : resource allocation, adding devices, etc...
	 * Note that we need to perform the finish pass on the -parent-
	 * bus of the EADS bridge so the bridge device itself gets
	 * properly added
	 */
	pcibios_finish_adding_to_bus(phb->bus);
}

static int dlpar_add_pci_slot(char *drc_name, struct device_node *dn)
{
	struct pci_dev *dev;
	struct pci_controller *phb;

<<<<<<< HEAD
	if (pcibios_find_pci_bus(dn))
=======
	if (pci_find_bus_by_node(dn))
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		return -EINVAL;

	/* Add pci bus */
	dlpar_pci_add_bus(dn);

	/* Confirm new bridge dev was created */
	phb = PCI_DN(dn)->phb;
	dev = dlpar_find_new_dev(phb->bus, dn);

	if (!dev) {
		printk(KERN_ERR "%s: unable to add bus %s\n", __func__,
			drc_name);
		return -EIO;
	}

	if (dev->hdr_type != PCI_HEADER_TYPE_BRIDGE) {
		printk(KERN_ERR "%s: unexpected header type %d, unable to add bus %s\n",
			__func__, dev->hdr_type, drc_name);
		return -EIO;
	}

	/* Add hotplug slot */
	if (rpaphp_add_slot(dn)) {
		printk(KERN_ERR "%s: unable to add hotplug slot %s\n",
			__func__, drc_name);
		return -EIO;
	}
	return 0;
}

static int dlpar_remove_phb(char *drc_name, struct device_node *dn)
{
	struct slot *slot;
	struct pci_dn *pdn;
	int rc = 0;

<<<<<<< HEAD
	if (!pcibios_find_pci_bus(dn))
		return -EINVAL;

	/* If pci slot is hotplugable, use hotplug to remove it */
=======
	if (!pci_find_bus_by_node(dn))
		return -EINVAL;

	/* If pci slot is hotpluggable, use hotplug to remove it */
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	slot = find_php_slot(dn);
	if (slot && rpaphp_deregister_slot(slot)) {
		printk(KERN_ERR "%s: unable to remove hotplug slot %s\n",
		       __func__, drc_name);
		return -EIO;
	}

	pdn = dn->data;
	BUG_ON(!pdn || !pdn->phb);
	rc = remove_phb_dynamic(pdn->phb);
	if (rc < 0)
		return rc;

	pdn->phb = NULL;

	return 0;
}

static int dlpar_add_phb(char *drc_name, struct device_node *dn)
{
	struct pci_controller *phb;

	if (PCI_DN(dn) && PCI_DN(dn)->phb) {
		/* PHB already exists */
		return -EINVAL;
	}

	phb = init_phb_dynamic(dn);
	if (!phb)
		return -EIO;

	if (rpaphp_add_slot(dn)) {
		printk(KERN_ERR "%s: unable to add hotplug slot %s\n",
			__func__, drc_name);
		return -EIO;
	}
	return 0;
}

static int dlpar_add_vio_slot(char *drc_name, struct device_node *dn)
{
<<<<<<< HEAD
	if (vio_find_node(dn))
		return -EINVAL;
=======
	struct vio_dev *vio_dev;

	vio_dev = vio_find_node(dn);
	if (vio_dev) {
		put_device(&vio_dev->dev);
		return -EINVAL;
	}
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	if (!vio_register_device_node(dn)) {
		printk(KERN_ERR
			"%s: failed to register vio node %s\n",
			__func__, drc_name);
		return -EIO;
	}
	return 0;
}

/**
 * dlpar_add_slot - DLPAR add an I/O Slot
 * @drc_name: drc-name of newly added slot
 *
 * Make the hotplug module and the kernel aware of a newly added I/O Slot.
 * Return Codes:
 * 0			Success
 * -ENODEV		Not a valid drc_name
 * -EINVAL		Slot already added
 * -ERESTARTSYS		Signalled before obtaining lock
 * -EIO			Internal PCI Error
 */
int dlpar_add_slot(char *drc_name)
{
	struct device_node *dn = NULL;
	int node_type;
	int rc = -EIO;

	if (mutex_lock_interruptible(&rpadlpar_mutex))
		return -ERESTARTSYS;

	/* Find newly added node */
	dn = find_dlpar_node(drc_name, &node_type);
	if (!dn) {
		rc = -ENODEV;
		goto exit;
	}

	switch (node_type) {
		case NODE_TYPE_VIO:
			rc = dlpar_add_vio_slot(drc_name, dn);
			break;
		case NODE_TYPE_SLOT:
			rc = dlpar_add_pci_slot(drc_name, dn);
			break;
		case NODE_TYPE_PHB:
			rc = dlpar_add_phb(drc_name, dn);
			break;
	}
<<<<<<< HEAD
=======
	of_node_put(dn);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	printk(KERN_INFO "%s: slot %s added\n", DLPAR_MODULE_NAME, drc_name);
exit:
	mutex_unlock(&rpadlpar_mutex);
	return rc;
}

/**
 * dlpar_remove_vio_slot - DLPAR remove a virtual I/O Slot
 * @drc_name: drc-name of newly added slot
 * @dn: &device_node
 *
 * Remove the kernel and hotplug representations of an I/O Slot.
 * Return Codes:
 * 0			Success
 * -EINVAL		Vio dev doesn't exist
 */
static int dlpar_remove_vio_slot(char *drc_name, struct device_node *dn)
{
	struct vio_dev *vio_dev;

	vio_dev = vio_find_node(dn);
	if (!vio_dev)
		return -EINVAL;

	vio_unregister_device(vio_dev);
<<<<<<< HEAD
=======

	put_device(&vio_dev->dev);

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	return 0;
}

/**
 * dlpar_remove_pci_slot - DLPAR remove a PCI I/O Slot
 * @drc_name: drc-name of newly added slot
 * @dn: &device_node
 *
 * Remove the kernel and hotplug representations of a PCI I/O Slot.
 * Return Codes:
 * 0			Success
 * -ENODEV		Not a valid drc_name
 * -EIO			Internal PCI Error
 */
<<<<<<< HEAD
int dlpar_remove_pci_slot(char *drc_name, struct device_node *dn)
{
	struct pci_bus *bus;
	struct slot *slot;

	bus = pcibios_find_pci_bus(dn);
	if (!bus)
		return -EINVAL;
=======
static int dlpar_remove_pci_slot(char *drc_name, struct device_node *dn)
{
	struct pci_bus *bus;
	struct slot *slot;
	int ret = 0;

	pci_lock_rescan_remove();

	bus = pci_find_bus_by_node(dn);
	if (!bus) {
		ret = -EINVAL;
		goto out;
	}
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	pr_debug("PCI: Removing PCI slot below EADS bridge %s\n",
		 bus->self ? pci_name(bus->self) : "<!PHB!>");

	slot = find_php_slot(dn);
	if (slot) {
		pr_debug("PCI: Removing hotplug slot for %04x:%02x...\n",
			 pci_domain_nr(bus), bus->number);

		if (rpaphp_deregister_slot(slot)) {
			printk(KERN_ERR
				"%s: unable to remove hotplug slot %s\n",
				__func__, drc_name);
<<<<<<< HEAD
			return -EIO;
=======
			ret = -EIO;
			goto out;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		}
	}

	/* Remove all devices below slot */
<<<<<<< HEAD
	pcibios_remove_pci_devices(bus);
=======
	pci_hp_remove_devices(bus);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	/* Unmap PCI IO space */
	if (pcibios_unmap_io_space(bus)) {
		printk(KERN_ERR "%s: failed to unmap bus range\n",
			__func__);
<<<<<<< HEAD
		return -ERANGE;
=======
		ret = -ERANGE;
		goto out;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	}

	/* Remove the EADS bridge device itself */
	BUG_ON(!bus->self);
	pr_debug("PCI: Now removing bridge device %s\n", pci_name(bus->self));
<<<<<<< HEAD
	eeh_remove_bus_device(bus->self);
	pci_stop_and_remove_bus_device(bus->self);

	return 0;
=======
	pci_stop_and_remove_bus_device(bus->self);

 out:
	pci_unlock_rescan_remove();
	return ret;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

/**
 * dlpar_remove_slot - DLPAR remove an I/O Slot
 * @drc_name: drc-name of newly added slot
 *
 * Remove the kernel and hotplug representations of an I/O Slot.
 * Return Codes:
 * 0			Success
 * -ENODEV		Not a valid drc_name
 * -EINVAL		Slot already removed
 * -ERESTARTSYS		Signalled before obtaining lock
 * -EIO			Internal Error
 */
int dlpar_remove_slot(char *drc_name)
{
	struct device_node *dn;
	int node_type;
	int rc = 0;

	if (mutex_lock_interruptible(&rpadlpar_mutex))
		return -ERESTARTSYS;

	dn = find_dlpar_node(drc_name, &node_type);
	if (!dn) {
		rc = -ENODEV;
		goto exit;
	}

	switch (node_type) {
		case NODE_TYPE_VIO:
			rc = dlpar_remove_vio_slot(drc_name, dn);
			break;
		case NODE_TYPE_PHB:
			rc = dlpar_remove_phb(drc_name, dn);
			break;
		case NODE_TYPE_SLOT:
			rc = dlpar_remove_pci_slot(drc_name, dn);
			break;
	}
<<<<<<< HEAD
=======
	of_node_put(dn);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	vm_unmap_aliases();

	printk(KERN_INFO "%s: slot %s removed\n", DLPAR_MODULE_NAME, drc_name);
exit:
	mutex_unlock(&rpadlpar_mutex);
	return rc;
}

static inline int is_dlpar_capable(void)
{
	int rc = rtas_token("ibm,configure-connector");

	return (int) (rc != RTAS_UNKNOWN_SERVICE);
}

<<<<<<< HEAD
int __init rpadlpar_io_init(void)
{
	int rc = 0;
=======
static int __init rpadlpar_io_init(void)
{
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	if (!is_dlpar_capable()) {
		printk(KERN_WARNING "%s: partition not DLPAR capable\n",
			__func__);
		return -EPERM;
	}

<<<<<<< HEAD
	rc = dlpar_sysfs_init();
	return rc;
}

void rpadlpar_io_exit(void)
{
	dlpar_sysfs_exit();
	return;
=======
	return dlpar_sysfs_init();
}

static void __exit rpadlpar_io_exit(void)
{
	dlpar_sysfs_exit();
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

module_init(rpadlpar_io_init);
module_exit(rpadlpar_io_exit);
MODULE_LICENSE("GPL");
<<<<<<< HEAD
=======
MODULE_DESCRIPTION("RPA Dynamic Logical Partitioning driver for I/O slots");
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
