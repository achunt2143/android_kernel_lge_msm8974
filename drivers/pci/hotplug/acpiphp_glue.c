<<<<<<< HEAD
=======
// SPDX-License-Identifier: GPL-2.0+
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
/*
 * ACPI PCI HotPlug glue functions to ACPI CA subsystem
 *
 * Copyright (C) 2002,2003 Takayoshi Kochi (t-kochi@bq.jp.nec.com)
 * Copyright (C) 2002 Hiroshi Aono (h-aono@ap.jp.nec.com)
 * Copyright (C) 2002,2003 NEC Corporation
<<<<<<< HEAD
 * Copyright (C) 2003-2005 Matthew Wilcox (matthew.wilcox@hp.com)
=======
 * Copyright (C) 2003-2005 Matthew Wilcox (willy@infradead.org)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 * Copyright (C) 2003-2005 Hewlett Packard
 * Copyright (C) 2005 Rajesh Shah (rajesh.shah@intel.com)
 * Copyright (C) 2005 Intel Corporation
 *
 * All rights reserved.
 *
<<<<<<< HEAD
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
 *
=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 * Send feedback to <kristen.c.accardi@intel.com>
 *
 */

/*
 * Lifetime rules for pci_dev:
 *  - The one in acpiphp_bridge has its refcount elevated by pci_get_slot()
 *    when the bridge is scanned and it loses a refcount when the bridge
 *    is removed.
 *  - When a P2P bridge is present, we elevate the refcount on the subordinate
<<<<<<< HEAD
 *    bus. It loses the refcount when the the driver unloads.
 */

#include <linux/init.h>
=======
 *    bus. It loses the refcount when the driver unloads.
 */

#define pr_fmt(fmt) "acpiphp_glue: " fmt

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#include <linux/module.h>

#include <linux/kernel.h>
#include <linux/pci.h>
#include <linux/pci_hotplug.h>
#include <linux/pci-acpi.h>
<<<<<<< HEAD
=======
#include <linux/pm_runtime.h>
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#include <linux/mutex.h>
#include <linux/slab.h>
#include <linux/acpi.h>

#include "../pci.h"
#include "acpiphp.h"

static LIST_HEAD(bridge_list);
<<<<<<< HEAD

#define MY_NAME "acpiphp_glue"

static void handle_hotplug_event_bridge (acpi_handle, u32, void *);
static void acpiphp_sanitize_bus(struct pci_bus *bus);
static void acpiphp_set_hpp_values(struct pci_bus *bus);
static void handle_hotplug_event_func(acpi_handle handle, u32 type, void *context);

/* callback routine to check for the existence of a pci dock device */
static acpi_status
is_pci_dock_device(acpi_handle handle, u32 lvl, void *context, void **rv)
{
	int *count = (int *)context;

	if (is_dock_device(handle)) {
		(*count)++;
		return AE_CTRL_TERMINATE;
	} else {
		return AE_OK;
	}
}

/*
 * the _DCK method can do funny things... and sometimes not
 * hah-hah funny.
 *
 * TBD - figure out a way to only call fixups for
 * systems that require them.
 */
static int post_dock_fixups(struct notifier_block *nb, unsigned long val,
	void *v)
{
	struct acpiphp_func *func = container_of(nb, struct acpiphp_func, nb);
	struct pci_bus *bus = func->slot->bridge->pci_bus;
	u32 buses;

	if (!bus->self)
		return  NOTIFY_OK;
=======
static DEFINE_MUTEX(bridge_mutex);

static int acpiphp_hotplug_notify(struct acpi_device *adev, u32 type);
static void acpiphp_post_dock_fixup(struct acpi_device *adev);
static void acpiphp_sanitize_bus(struct pci_bus *bus);
static void hotplug_event(u32 type, struct acpiphp_context *context);
static void free_bridge(struct kref *kref);

/**
 * acpiphp_init_context - Create hotplug context and grab a reference to it.
 * @adev: ACPI device object to create the context for.
 *
 * Call under acpi_hp_context_lock.
 */
static struct acpiphp_context *acpiphp_init_context(struct acpi_device *adev)
{
	struct acpiphp_context *context;

	context = kzalloc(sizeof(*context), GFP_KERNEL);
	if (!context)
		return NULL;

	context->refcount = 1;
	context->hp.notify = acpiphp_hotplug_notify;
	context->hp.fixup = acpiphp_post_dock_fixup;
	acpi_set_hp_context(adev, &context->hp);
	return context;
}

/**
 * acpiphp_get_context - Get hotplug context and grab a reference to it.
 * @adev: ACPI device object to get the context for.
 *
 * Call under acpi_hp_context_lock.
 */
static struct acpiphp_context *acpiphp_get_context(struct acpi_device *adev)
{
	struct acpiphp_context *context;

	if (!adev->hp)
		return NULL;

	context = to_acpiphp_context(adev->hp);
	context->refcount++;
	return context;
}

/**
 * acpiphp_put_context - Drop a reference to ACPI hotplug context.
 * @context: ACPI hotplug context to drop a reference to.
 *
 * The context object is removed if there are no more references to it.
 *
 * Call under acpi_hp_context_lock.
 */
static void acpiphp_put_context(struct acpiphp_context *context)
{
	if (--context->refcount)
		return;

	WARN_ON(context->bridge);
	context->hp.self->hp = NULL;
	kfree(context);
}

static inline void get_bridge(struct acpiphp_bridge *bridge)
{
	kref_get(&bridge->ref);
}

static inline void put_bridge(struct acpiphp_bridge *bridge)
{
	kref_put(&bridge->ref, free_bridge);
}

static struct acpiphp_context *acpiphp_grab_context(struct acpi_device *adev)
{
	struct acpiphp_context *context;

	acpi_lock_hp_context();

	context = acpiphp_get_context(adev);
	if (!context)
		goto unlock;

	if (context->func.parent->is_going_away) {
		acpiphp_put_context(context);
		context = NULL;
		goto unlock;
	}

	get_bridge(context->func.parent);
	acpiphp_put_context(context);

unlock:
	acpi_unlock_hp_context();
	return context;
}

static void acpiphp_let_context_go(struct acpiphp_context *context)
{
	put_bridge(context->func.parent);
}

static void free_bridge(struct kref *kref)
{
	struct acpiphp_context *context;
	struct acpiphp_bridge *bridge;
	struct acpiphp_slot *slot, *next;
	struct acpiphp_func *func, *tmp;

	acpi_lock_hp_context();

	bridge = container_of(kref, struct acpiphp_bridge, ref);

	list_for_each_entry_safe(slot, next, &bridge->slots, node) {
		list_for_each_entry_safe(func, tmp, &slot->funcs, sibling)
			acpiphp_put_context(func_to_context(func));

		kfree(slot);
	}

	context = bridge->context;
	/* Root bridges will not have hotplug context. */
	if (context) {
		/* Release the reference taken by acpiphp_enumerate_slots(). */
		put_bridge(context->func.parent);
		context->bridge = NULL;
		acpiphp_put_context(context);
	}

	put_device(&bridge->pci_bus->dev);
	pci_dev_put(bridge->pci_dev);
	kfree(bridge);

	acpi_unlock_hp_context();
}

/**
 * acpiphp_post_dock_fixup - Post-dock fixups for PCI devices.
 * @adev: ACPI device object corresponding to a PCI device.
 *
 * TBD - figure out a way to only call fixups for systems that require them.
 */
static void acpiphp_post_dock_fixup(struct acpi_device *adev)
{
	struct acpiphp_context *context = acpiphp_grab_context(adev);
	struct pci_bus *bus;
	u32 buses;

	if (!context)
		return;

	bus = context->func.slot->bus;
	if (!bus->self)
		goto out;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	/* fixup bad _DCK function that rewrites
	 * secondary bridge on slot
	 */
<<<<<<< HEAD
	pci_read_config_dword(bus->self,
			PCI_PRIMARY_BUS,
			&buses);

	if (((buses >> 8) & 0xff) != bus->secondary) {
		buses = (buses & 0xff000000)
			| ((unsigned int)(bus->primary)     <<  0)
			| ((unsigned int)(bus->secondary)   <<  8)
			| ((unsigned int)(bus->subordinate) << 16);
		pci_write_config_dword(bus->self, PCI_PRIMARY_BUS, buses);
	}
	return NOTIFY_OK;
}


static const struct acpi_dock_ops acpiphp_dock_ops = {
	.handler = handle_hotplug_event_func,
};

/* callback routine to register each ACPI PCI slot object */
static acpi_status
register_slot(acpi_handle handle, u32 lvl, void *context, void **rv)
{
	struct acpiphp_bridge *bridge = (struct acpiphp_bridge *)context;
	struct acpiphp_slot *slot;
	struct acpiphp_func *newfunc;
	acpi_handle tmp;
	acpi_status status = AE_OK;
	unsigned long long adr, sun;
	int device, function, retval;
	struct pci_bus *pbus = bridge->pci_bus;
	struct pci_dev *pdev;

	if (!acpi_pci_check_ejectable(pbus, handle) && !is_dock_device(handle))
=======
	pci_read_config_dword(bus->self, PCI_PRIMARY_BUS, &buses);

	if (((buses >> 8) & 0xff) != bus->busn_res.start) {
		buses = (buses & 0xff000000)
			| ((unsigned int)(bus->primary)     <<  0)
			| ((unsigned int)(bus->busn_res.start)   <<  8)
			| ((unsigned int)(bus->busn_res.end) << 16);
		pci_write_config_dword(bus->self, PCI_PRIMARY_BUS, buses);
	}

 out:
	acpiphp_let_context_go(context);
}

/**
 * acpiphp_add_context - Add ACPIPHP context to an ACPI device object.
 * @handle: ACPI handle of the object to add a context to.
 * @lvl: Not used.
 * @data: The object's parent ACPIPHP bridge.
 * @rv: Not used.
 */
static acpi_status acpiphp_add_context(acpi_handle handle, u32 lvl, void *data,
				       void **rv)
{
	struct acpi_device *adev = acpi_fetch_acpi_dev(handle);
	struct acpiphp_bridge *bridge = data;
	struct acpiphp_context *context;
	struct acpiphp_slot *slot;
	struct acpiphp_func *newfunc;
	acpi_status status = AE_OK;
	unsigned long long adr;
	int device, function;
	struct pci_bus *pbus = bridge->pci_bus;
	struct pci_dev *pdev = bridge->pci_dev;
	u32 val;

	if (!adev)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		return AE_OK;

	status = acpi_evaluate_integer(handle, "_ADR", NULL, &adr);
	if (ACPI_FAILURE(status)) {
<<<<<<< HEAD
		warn("can't evaluate _ADR (%#x)\n", status);
=======
		if (status != AE_NOT_FOUND)
			acpi_handle_warn(handle,
				"can't evaluate _ADR (%#x)\n", status);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		return AE_OK;
	}

	device = (adr >> 16) & 0xffff;
	function = adr & 0xffff;

<<<<<<< HEAD
	pdev = pbus->self;
	if (pdev && pci_is_pcie(pdev)) {
		tmp = acpi_find_root_bridge_handle(pdev);
		if (tmp) {
			struct acpi_pci_root *root = acpi_pci_find_root(tmp);

			if (root && (root->osc_control_set &
					OSC_PCI_EXPRESS_NATIVE_HP_CONTROL))
				return AE_OK;
		}
	}

	newfunc = kzalloc(sizeof(struct acpiphp_func), GFP_KERNEL);
	if (!newfunc)
		return AE_NO_MEMORY;

	INIT_LIST_HEAD(&newfunc->sibling);
	newfunc->handle = handle;
	newfunc->function = function;

	if (ACPI_SUCCESS(acpi_get_handle(handle, "_EJ0", &tmp)))
		newfunc->flags = FUNC_HAS_EJ0;

	if (ACPI_SUCCESS(acpi_get_handle(handle, "_STA", &tmp)))
		newfunc->flags |= FUNC_HAS_STA;

	if (ACPI_SUCCESS(acpi_get_handle(handle, "_PS0", &tmp)))
		newfunc->flags |= FUNC_HAS_PS0;

	if (ACPI_SUCCESS(acpi_get_handle(handle, "_PS3", &tmp)))
		newfunc->flags |= FUNC_HAS_PS3;

	if (ACPI_SUCCESS(acpi_get_handle(handle, "_DCK", &tmp)))
		newfunc->flags |= FUNC_HAS_DCK;

	status = acpi_evaluate_integer(handle, "_SUN", NULL, &sun);
	if (ACPI_FAILURE(status)) {
		/*
		 * use the count of the number of slots we've found
		 * for the number of the slot
		 */
		sun = bridge->nr_slots+1;
	}

	/* search for objects that share the same slot */
	for (slot = bridge->slots; slot; slot = slot->next)
		if (slot->device == device) {
			if (slot->sun != sun)
				warn("sibling found, but _SUN doesn't match!\n");
			break;
		}

	if (!slot) {
		slot = kzalloc(sizeof(struct acpiphp_slot), GFP_KERNEL);
		if (!slot) {
			kfree(newfunc);
			return AE_NO_MEMORY;
		}

		slot->bridge = bridge;
		slot->device = device;
		slot->sun = sun;
		INIT_LIST_HEAD(&slot->funcs);
		mutex_init(&slot->crit_sect);

		slot->next = bridge->slots;
		bridge->slots = slot;

		bridge->nr_slots++;

		dbg("found ACPI PCI Hotplug slot %llu at PCI %04x:%02x:%02x\n",
		    slot->sun, pci_domain_nr(pbus), pbus->number, device);
		retval = acpiphp_register_hotplug_slot(slot);
		if (retval) {
			if (retval == -EBUSY)
				warn("Slot %llu already registered by another "
					"hotplug driver\n", slot->sun);
			else
				warn("acpiphp_register_hotplug_slot failed "
					"(err code = 0x%x)\n", retval);
			goto err_exit;
		}
	}

	newfunc->slot = slot;
	list_add_tail(&newfunc->sibling, &slot->funcs);

	pdev = pci_get_slot(pbus, PCI_DEVFN(device, function));
	if (pdev) {
		slot->flags |= (SLOT_ENABLED | SLOT_POWEREDON);
		pci_dev_put(pdev);
	}

	if (is_dock_device(handle)) {
		/* we don't want to call this device's _EJ0
		 * because we want the dock notify handler
		 * to call it after it calls _DCK
		 */
		newfunc->flags &= ~FUNC_HAS_EJ0;
		if (register_hotplug_dock_device(handle,
			&acpiphp_dock_ops, newfunc))
			dbg("failed to register dock device\n");

		/* we need to be notified when dock events happen
		 * outside of the hotplug operation, since we may
		 * need to do fixups before we can hotplug.
		 */
		newfunc->nb.notifier_call = post_dock_fixups;
		if (register_dock_notifier(&newfunc->nb))
			dbg("failed to register a dock notifier");
	}

	/* install notify handler */
	if (!(newfunc->flags & FUNC_HAS_DCK)) {
		status = acpi_install_notify_handler(handle,
					     ACPI_SYSTEM_NOTIFY,
					     handle_hotplug_event_func,
					     newfunc);

		if (ACPI_FAILURE(status))
			err("failed to register interrupt notify handler\n");
	} else
		status = AE_OK;

	return status;

 err_exit:
	bridge->nr_slots--;
	bridge->slots = slot->next;
	kfree(slot);
	kfree(newfunc);
=======
	acpi_lock_hp_context();
	context = acpiphp_init_context(adev);
	if (!context) {
		acpi_unlock_hp_context();
		acpi_handle_err(handle, "No hotplug context\n");
		return AE_NOT_EXIST;
	}
	newfunc = &context->func;
	newfunc->function = function;
	newfunc->parent = bridge;
	acpi_unlock_hp_context();

	/*
	 * If this is a dock device, its _EJ0 should be executed by the dock
	 * notify handler after calling _DCK.
	 */
	if (!is_dock_device(adev) && acpi_has_method(handle, "_EJ0"))
		newfunc->flags = FUNC_HAS_EJ0;

	if (acpi_has_method(handle, "_STA"))
		newfunc->flags |= FUNC_HAS_STA;

	/* search for objects that share the same slot */
	list_for_each_entry(slot, &bridge->slots, node)
		if (slot->device == device)
			goto slot_found;

	slot = kzalloc(sizeof(struct acpiphp_slot), GFP_KERNEL);
	if (!slot) {
		acpi_lock_hp_context();
		acpiphp_put_context(context);
		acpi_unlock_hp_context();
		return AE_NO_MEMORY;
	}

	slot->bus = bridge->pci_bus;
	slot->device = device;
	INIT_LIST_HEAD(&slot->funcs);

	list_add_tail(&slot->node, &bridge->slots);

	/*
	 * Expose slots to user space for functions that have _EJ0 or _RMV or
	 * are located in dock stations.  Do not expose them for devices handled
	 * by the native PCIe hotplug (PCIeHP) or standard PCI hotplug
	 * (SHPCHP), because that code is supposed to expose slots to user
	 * space in those cases.
	 */
	if ((acpi_pci_check_ejectable(pbus, handle) || is_dock_device(adev))
	    && !(pdev && hotplug_is_native(pdev))) {
		unsigned long long sun;
		int retval;

		bridge->nr_slots++;
		status = acpi_evaluate_integer(handle, "_SUN", NULL, &sun);
		if (ACPI_FAILURE(status))
			sun = bridge->nr_slots;

		pr_debug("found ACPI PCI Hotplug slot %llu at PCI %04x:%02x:%02x\n",
		    sun, pci_domain_nr(pbus), pbus->number, device);

		retval = acpiphp_register_hotplug_slot(slot, sun);
		if (retval) {
			slot->slot = NULL;
			bridge->nr_slots--;
			if (retval == -EBUSY)
				pr_warn("Slot %llu already registered by another hotplug driver\n", sun);
			else
				pr_warn("acpiphp_register_hotplug_slot failed (err code = 0x%x)\n", retval);
		}
		/* Even if the slot registration fails, we can still use it. */
	}

 slot_found:
	newfunc->slot = slot;
	list_add_tail(&newfunc->sibling, &slot->funcs);

	if (pci_bus_read_dev_vendor_id(pbus, PCI_DEVFN(device, function),
				       &val, 60*1000))
		slot->flags |= SLOT_ENABLED;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	return AE_OK;
}

<<<<<<< HEAD

/* see if it's worth looking at this bridge */
static int detect_ejectable_slots(acpi_handle handle)
{
	int found = acpi_pci_detect_ejectable(handle);
	if (!found) {
		acpi_walk_namespace(ACPI_TYPE_DEVICE, handle, (u32)1,
				    is_pci_dock_device, NULL, (void *)&found, NULL);
	}
	return found;
}

/* initialize miscellaneous stuff for both root and PCI-to-PCI bridge */
static void init_bridge_misc(struct acpiphp_bridge *bridge)
{
	acpi_status status;

	/* must be added to the list prior to calling register_slot */
	list_add(&bridge->list, &bridge_list);

	/* register all slot objects under this bridge */
	status = acpi_walk_namespace(ACPI_TYPE_DEVICE, bridge->handle, (u32)1,
				     register_slot, NULL, bridge, NULL);
	if (ACPI_FAILURE(status)) {
		list_del(&bridge->list);
		return;
	}

	/* install notify handler */
	if (bridge->type != BRIDGE_TYPE_HOST) {
		if ((bridge->flags & BRIDGE_HAS_EJ0) && bridge->func) {
			status = acpi_remove_notify_handler(bridge->func->handle,
						ACPI_SYSTEM_NOTIFY,
						handle_hotplug_event_func);
			if (ACPI_FAILURE(status))
				err("failed to remove notify handler\n");
		}
		status = acpi_install_notify_handler(bridge->handle,
					     ACPI_SYSTEM_NOTIFY,
					     handle_hotplug_event_bridge,
					     bridge);

		if (ACPI_FAILURE(status)) {
			err("failed to register interrupt notify handler\n");
		}
	}
}


/* find acpiphp_func from acpiphp_bridge */
static struct acpiphp_func *acpiphp_bridge_handle_to_function(acpi_handle handle)
{
	struct acpiphp_bridge *bridge;
	struct acpiphp_slot *slot;
	struct acpiphp_func *func;

	list_for_each_entry(bridge, &bridge_list, list) {
		for (slot = bridge->slots; slot; slot = slot->next) {
			list_for_each_entry(func, &slot->funcs, sibling) {
				if (func->handle == handle)
					return func;
			}
		}
	}

	return NULL;
}


static inline void config_p2p_bridge_flags(struct acpiphp_bridge *bridge)
{
	acpi_handle dummy_handle;

	if (ACPI_SUCCESS(acpi_get_handle(bridge->handle,
					"_STA", &dummy_handle)))
		bridge->flags |= BRIDGE_HAS_STA;

	if (ACPI_SUCCESS(acpi_get_handle(bridge->handle,
					"_EJ0", &dummy_handle)))
		bridge->flags |= BRIDGE_HAS_EJ0;

	if (ACPI_SUCCESS(acpi_get_handle(bridge->handle,
					"_PS0", &dummy_handle)))
		bridge->flags |= BRIDGE_HAS_PS0;

	if (ACPI_SUCCESS(acpi_get_handle(bridge->handle,
					"_PS3", &dummy_handle)))
		bridge->flags |= BRIDGE_HAS_PS3;

	/* is this ejectable p2p bridge? */
	if (bridge->flags & BRIDGE_HAS_EJ0) {
		struct acpiphp_func *func;

		dbg("found ejectable p2p bridge\n");

		/* make link between PCI bridge and PCI function */
		func = acpiphp_bridge_handle_to_function(bridge->handle);
		if (!func)
			return;
		bridge->func = func;
		func->bridge = bridge;
	}
}


/* allocate and initialize host bridge data structure */
static void add_host_bridge(acpi_handle *handle)
{
	struct acpiphp_bridge *bridge;
	struct acpi_pci_root *root = acpi_pci_find_root(handle);

	bridge = kzalloc(sizeof(struct acpiphp_bridge), GFP_KERNEL);
	if (bridge == NULL)
		return;

	bridge->type = BRIDGE_TYPE_HOST;
	bridge->handle = handle;

	bridge->pci_bus = root->bus;

	spin_lock_init(&bridge->res_lock);

	init_bridge_misc(bridge);
}


/* allocate and initialize PCI-to-PCI bridge data structure */
static void add_p2p_bridge(acpi_handle *handle)
{
	struct acpiphp_bridge *bridge;

	bridge = kzalloc(sizeof(struct acpiphp_bridge), GFP_KERNEL);
	if (bridge == NULL) {
		err("out of memory\n");
		return;
	}

	bridge->type = BRIDGE_TYPE_P2P;
	bridge->handle = handle;
	config_p2p_bridge_flags(bridge);

	bridge->pci_dev = acpi_get_pci_dev(handle);
	bridge->pci_bus = bridge->pci_dev->subordinate;
	if (!bridge->pci_bus) {
		err("This is not a PCI-to-PCI bridge!\n");
		goto err;
	}

	/*
	 * Grab a ref to the subordinate PCI bus in case the bus is
	 * removed via PCI core logical hotplug. The ref pins the bus
	 * (which we access during module unload).
	 */
	get_device(&bridge->pci_bus->dev);
	spin_lock_init(&bridge->res_lock);

	init_bridge_misc(bridge);
	return;
 err:
	pci_dev_put(bridge->pci_dev);
	kfree(bridge);
	return;
}


/* callback routine to find P2P bridges */
static acpi_status
find_p2p_bridge(acpi_handle handle, u32 lvl, void *context, void **rv)
{
	acpi_status status;
	struct pci_dev *dev;

	dev = acpi_get_pci_dev(handle);
	if (!dev || !dev->subordinate)
		goto out;

	/* check if this bridge has ejectable slots */
	if ((detect_ejectable_slots(handle) > 0)) {
		dbg("found PCI-to-PCI bridge at PCI %s\n", pci_name(dev));
		add_p2p_bridge(handle);
	}

	/* search P2P bridges under this p2p bridge */
	status = acpi_walk_namespace(ACPI_TYPE_DEVICE, handle, (u32)1,
				     find_p2p_bridge, NULL, NULL, NULL);
	if (ACPI_FAILURE(status))
		warn("find_p2p_bridge failed (error code = 0x%x)\n", status);

 out:
	pci_dev_put(dev);
	return AE_OK;
}


/* find hot-pluggable slots, and then find P2P bridge */
static int add_bridge(acpi_handle handle)
{
	acpi_status status;
	unsigned long long tmp;
	acpi_handle dummy_handle;

	/* if the bridge doesn't have _STA, we assume it is always there */
	status = acpi_get_handle(handle, "_STA", &dummy_handle);
	if (ACPI_SUCCESS(status)) {
		status = acpi_evaluate_integer(handle, "_STA", NULL, &tmp);
		if (ACPI_FAILURE(status)) {
			dbg("%s: _STA evaluation failure\n", __func__);
			return 0;
		}
		if ((tmp & ACPI_STA_FUNCTIONING) == 0)
			/* don't register this object */
			return 0;
	}

	/* check if this bridge has ejectable slots */
	if (detect_ejectable_slots(handle) > 0) {
		dbg("found PCI host-bus bridge with hot-pluggable slots\n");
		add_host_bridge(handle);
	}

	/* search P2P bridges under this host bridge */
	status = acpi_walk_namespace(ACPI_TYPE_DEVICE, handle, (u32)1,
				     find_p2p_bridge, NULL, NULL, NULL);

	if (ACPI_FAILURE(status))
		warn("find_p2p_bridge failed (error code = 0x%x)\n", status);

	return 0;
}

static struct acpiphp_bridge *acpiphp_handle_to_bridge(acpi_handle handle)
{
	struct acpiphp_bridge *bridge;

	list_for_each_entry(bridge, &bridge_list, list)
		if (bridge->handle == handle)
			return bridge;

	return NULL;
}

static void cleanup_bridge(struct acpiphp_bridge *bridge)
{
	struct acpiphp_slot *slot, *next;
	struct acpiphp_func *func, *tmp;
	acpi_status status;
	acpi_handle handle = bridge->handle;

	status = acpi_remove_notify_handler(handle, ACPI_SYSTEM_NOTIFY,
					    handle_hotplug_event_bridge);
	if (ACPI_FAILURE(status))
		err("failed to remove notify handler\n");

	if ((bridge->type != BRIDGE_TYPE_HOST) &&
	    ((bridge->flags & BRIDGE_HAS_EJ0) && bridge->func)) {
		status = acpi_install_notify_handler(bridge->func->handle,
						ACPI_SYSTEM_NOTIFY,
						handle_hotplug_event_func,
						bridge->func);
		if (ACPI_FAILURE(status))
			err("failed to install interrupt notify handler\n");
	}

	slot = bridge->slots;
	while (slot) {
		next = slot->next;
		list_for_each_entry_safe(func, tmp, &slot->funcs, sibling) {
			if (is_dock_device(func->handle)) {
				unregister_hotplug_dock_device(func->handle);
				unregister_dock_notifier(&func->nb);
			}
			if (!(func->flags & FUNC_HAS_DCK)) {
				status = acpi_remove_notify_handler(func->handle,
						ACPI_SYSTEM_NOTIFY,
						handle_hotplug_event_func);
				if (ACPI_FAILURE(status))
					err("failed to remove notify handler\n");
			}
			list_del(&func->sibling);
			kfree(func);
		}
		acpiphp_unregister_hotplug_slot(slot);
		list_del(&slot->funcs);
		kfree(slot);
		slot = next;
	}

	/*
	 * Only P2P bridges have a pci_dev
	 */
	if (bridge->pci_dev)
		put_device(&bridge->pci_bus->dev);

	pci_dev_put(bridge->pci_dev);
	list_del(&bridge->list);
	kfree(bridge);
}

static acpi_status
cleanup_p2p_bridge(acpi_handle handle, u32 lvl, void *context, void **rv)
{
	struct acpiphp_bridge *bridge;

	/* cleanup p2p bridges under this P2P bridge
	   in a depth-first manner */
	acpi_walk_namespace(ACPI_TYPE_DEVICE, handle, (u32)1,
				cleanup_p2p_bridge, NULL, NULL, NULL);

	bridge = acpiphp_handle_to_bridge(handle);
	if (bridge)
		cleanup_bridge(bridge);

	return AE_OK;
}

static void remove_bridge(acpi_handle handle)
{
	struct acpiphp_bridge *bridge;

	/* cleanup p2p bridges under this host bridge
	   in a depth-first manner */
	acpi_walk_namespace(ACPI_TYPE_DEVICE, handle,
				(u32)1, cleanup_p2p_bridge, NULL, NULL, NULL);

	/*
	 * On root bridges with hotplug slots directly underneath (ie,
	 * no p2p bridge between), we call cleanup_bridge(). 
	 *
	 * The else clause cleans up root bridges that either had no
	 * hotplug slots at all, or had a p2p bridge underneath.
	 */
	bridge = acpiphp_handle_to_bridge(handle);
	if (bridge)
		cleanup_bridge(bridge);
	else
		acpi_remove_notify_handler(handle, ACPI_SYSTEM_NOTIFY,
					   handle_hotplug_event_bridge);
}

static int power_on_slot(struct acpiphp_slot *slot)
{
	acpi_status status;
	struct acpiphp_func *func;
	int retval = 0;

	/* if already enabled, just skip */
	if (slot->flags & SLOT_POWEREDON)
		goto err_exit;

	list_for_each_entry(func, &slot->funcs, sibling) {
		if (func->flags & FUNC_HAS_PS0) {
			dbg("%s: executing _PS0\n", __func__);
			status = acpi_evaluate_object(func->handle, "_PS0", NULL, NULL);
			if (ACPI_FAILURE(status)) {
				warn("%s: _PS0 failed\n", __func__);
				retval = -1;
				goto err_exit;
			} else
				break;
		}
	}

	/* TBD: evaluate _STA to check if the slot is enabled */

	slot->flags |= SLOT_POWEREDON;

 err_exit:
	return retval;
}


static int power_off_slot(struct acpiphp_slot *slot)
{
	acpi_status status;
	struct acpiphp_func *func;

	int retval = 0;

	/* if already disabled, just skip */
	if ((slot->flags & SLOT_POWEREDON) == 0)
		goto err_exit;

	list_for_each_entry(func, &slot->funcs, sibling) {
		if (func->flags & FUNC_HAS_PS3) {
			status = acpi_evaluate_object(func->handle, "_PS3", NULL, NULL);
			if (ACPI_FAILURE(status)) {
				warn("%s: _PS3 failed\n", __func__);
				retval = -1;
				goto err_exit;
			} else
				break;
		}
	}

	/* TBD: evaluate _STA to check if the slot is disabled */

	slot->flags &= (~SLOT_POWEREDON);

 err_exit:
	return retval;
}



=======
static void cleanup_bridge(struct acpiphp_bridge *bridge)
{
	struct acpiphp_slot *slot;
	struct acpiphp_func *func;

	list_for_each_entry(slot, &bridge->slots, node) {
		list_for_each_entry(func, &slot->funcs, sibling) {
			struct acpi_device *adev = func_to_acpi_device(func);

			acpi_lock_hp_context();
			adev->hp->notify = NULL;
			adev->hp->fixup = NULL;
			acpi_unlock_hp_context();
		}
		slot->flags |= SLOT_IS_GOING_AWAY;
		if (slot->slot)
			acpiphp_unregister_hotplug_slot(slot);
	}

	mutex_lock(&bridge_mutex);
	list_del(&bridge->list);
	mutex_unlock(&bridge_mutex);

	acpi_lock_hp_context();
	bridge->is_going_away = true;
	acpi_unlock_hp_context();
}

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
/**
 * acpiphp_max_busnr - return the highest reserved bus number under the given bus.
 * @bus: bus to start search with
 */
static unsigned char acpiphp_max_busnr(struct pci_bus *bus)
{
<<<<<<< HEAD
	struct list_head *tmp;
=======
	struct pci_bus *tmp;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	unsigned char max, n;

	/*
	 * pci_bus_max_busnr will return the highest
	 * reserved busnr for all these children.
	 * that is equivalent to the bus->subordinate
	 * value.  We don't want to use the parent's
	 * bus->subordinate value because it could have
	 * padding in it.
	 */
<<<<<<< HEAD
	max = bus->secondary;

	list_for_each(tmp, &bus->children) {
		n = pci_bus_max_busnr(pci_bus_b(tmp));
=======
	max = bus->busn_res.start;

	list_for_each_entry(tmp, &bus->children, node) {
		n = pci_bus_max_busnr(tmp);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		if (n > max)
			max = n;
	}
	return max;
}

<<<<<<< HEAD

/**
 * acpiphp_bus_add - add a new bus to acpi subsystem
 * @func: acpiphp_func of the bridge
 */
static int acpiphp_bus_add(struct acpiphp_func *func)
{
	acpi_handle phandle;
	struct acpi_device *device, *pdevice;
	int ret_val;

	acpi_get_parent(func->handle, &phandle);
	if (acpi_bus_get_device(phandle, &pdevice)) {
		dbg("no parent device, assuming NULL\n");
		pdevice = NULL;
	}
	if (!acpi_bus_get_device(func->handle, &device)) {
		dbg("bus exists... trim\n");
		/* this shouldn't be in here, so remove
		 * the bus then re-add it...
		 */
		ret_val = acpi_bus_trim(device, 1);
		dbg("acpi_bus_trim return %x\n", ret_val);
	}

	ret_val = acpi_bus_add(&device, pdevice, func->handle,
		ACPI_BUS_TYPE_DEVICE);
	if (ret_val) {
		dbg("error adding bus, %x\n",
			-ret_val);
		goto acpiphp_bus_add_out;
	}
	ret_val = acpi_bus_start(device);

acpiphp_bus_add_out:
	return ret_val;
}


/**
 * acpiphp_bus_trim - trim a bus from acpi subsystem
 * @handle: handle to acpi namespace
 */
static int acpiphp_bus_trim(acpi_handle handle)
{
	struct acpi_device *device;
	int retval;

	retval = acpi_bus_get_device(handle, &device);
	if (retval) {
		dbg("acpi_device not found\n");
		return retval;
	}

	retval = acpi_bus_trim(device, 1);
	if (retval)
		err("cannot remove from acpi list\n");

	return retval;
}

static void acpiphp_set_acpi_region(struct acpiphp_slot *slot)
{
	struct acpiphp_func *func;
	union acpi_object params[2];
	struct acpi_object_list arg_list;

	list_for_each_entry(func, &slot->funcs, sibling) {
		arg_list.count = 2;
		arg_list.pointer = params;
		params[0].type = ACPI_TYPE_INTEGER;
		params[0].integer.value = ACPI_ADR_SPACE_PCI_CONFIG;
		params[1].type = ACPI_TYPE_INTEGER;
		params[1].integer.value = 1;
		/* _REG is optional, we don't care about if there is failure */
		acpi_evaluate_object(func->handle, "_REG", &arg_list, NULL);
=======
static void acpiphp_set_acpi_region(struct acpiphp_slot *slot)
{
	struct acpiphp_func *func;

	list_for_each_entry(func, &slot->funcs, sibling) {
		/* _REG is optional, we don't care about if there is failure */
		acpi_evaluate_reg(func_to_handle(func),
				  ACPI_ADR_SPACE_PCI_CONFIG,
				  ACPI_REG_CONNECT);
	}
}

static void check_hotplug_bridge(struct acpiphp_slot *slot, struct pci_dev *dev)
{
	struct acpiphp_func *func;

	/* quirk, or pcie could set it already */
	if (dev->is_hotplug_bridge)
		return;

	/*
	 * In the PCIe case, only Root Ports and Downstream Ports are capable of
	 * accommodating hotplug devices, so avoid marking Upstream Ports as
	 * "hotplug bridges".
	 */
	if (pci_is_pcie(dev) && pci_pcie_type(dev) == PCI_EXP_TYPE_UPSTREAM)
		return;

	list_for_each_entry(func, &slot->funcs, sibling) {
		if (PCI_FUNC(dev->devfn) == func->function) {
			dev->is_hotplug_bridge = 1;
			break;
		}
	}
}

static int acpiphp_rescan_slot(struct acpiphp_slot *slot)
{
	struct acpiphp_func *func;

	list_for_each_entry(func, &slot->funcs, sibling) {
		struct acpi_device *adev = func_to_acpi_device(func);

		acpi_bus_scan(adev->handle);
		if (acpi_device_enumerated(adev))
			acpi_device_set_power(adev, ACPI_STATE_D0);
	}
	return pci_scan_slot(slot->bus, PCI_DEVFN(slot->device, 0));
}

static void acpiphp_native_scan_bridge(struct pci_dev *bridge)
{
	struct pci_bus *bus = bridge->subordinate;
	struct pci_dev *dev;
	int max;

	if (!bus)
		return;

	max = bus->busn_res.start;
	/* Scan already configured non-hotplug bridges */
	for_each_pci_bridge(dev, bus) {
		if (!hotplug_is_native(dev))
			max = pci_scan_bridge(bus, dev, max, 0);
	}

	/* Scan non-hotplug bridges that need to be reconfigured */
	for_each_pci_bridge(dev, bus) {
		if (hotplug_is_native(dev))
			continue;

		max = pci_scan_bridge(bus, dev, max, 1);
		if (dev->subordinate) {
			pcibios_resource_survey_bus(dev->subordinate);
			pci_bus_size_bridges(dev->subordinate);
			pci_bus_assign_resources(dev->subordinate);
		}
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	}
}

/**
<<<<<<< HEAD
 * enable_device - enable, configure a slot
 * @slot: slot to be enabled
=======
 * enable_slot - enable, configure a slot
 * @slot: slot to be enabled
 * @bridge: true if enable is for the whole bridge (not a single slot)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 *
 * This function should be called per *physical slot*,
 * not per each slot object in ACPI namespace.
 */
<<<<<<< HEAD
static int __ref enable_device(struct acpiphp_slot *slot)
{
	struct pci_dev *dev;
	struct pci_bus *bus = slot->bridge->pci_bus;
	struct acpiphp_func *func;
	int retval = 0;
	int num, max, pass;
	acpi_status status;

	if (slot->flags & SLOT_ENABLED)
		goto err_exit;

	num = pci_scan_slot(bus, PCI_DEVFN(slot->device, 0));
	if (num == 0) {
		/* Maybe only part of funcs are added. */
		dbg("No new device found\n");
		goto err_exit;
	}

	max = acpiphp_max_busnr(bus);
	for (pass = 0; pass < 2; pass++) {
		list_for_each_entry(dev, &bus->devices, bus_list) {
			if (PCI_SLOT(dev->devfn) != slot->device)
				continue;
			if (dev->hdr_type == PCI_HEADER_TYPE_BRIDGE ||
			    dev->hdr_type == PCI_HEADER_TYPE_CARDBUS) {
				max = pci_scan_bridge(bus, dev, max, pass);
				if (pass && dev->subordinate)
					pci_bus_size_bridges(dev->subordinate);
			}
		}
	}

	list_for_each_entry(func, &slot->funcs, sibling)
		acpiphp_bus_add(func);

	pci_bus_assign_resources(bus);
	acpiphp_sanitize_bus(bus);
	acpiphp_set_hpp_values(bus);
	acpiphp_set_acpi_region(slot);
	pci_enable_bridges(bus);

	list_for_each_entry(dev, &bus->devices, bus_list) {
		/* Assume that newly added devices are powered on already. */
		if (!dev->is_added)
=======
static void enable_slot(struct acpiphp_slot *slot, bool bridge)
{
	struct pci_dev *dev;
	struct pci_bus *bus = slot->bus;
	struct acpiphp_func *func;

	if (bridge && bus->self && hotplug_is_native(bus->self)) {
		/*
		 * If native hotplug is used, it will take care of hotplug
		 * slot management and resource allocation for hotplug
		 * bridges. However, ACPI hotplug may still be used for
		 * non-hotplug bridges to bring in additional devices such
		 * as a Thunderbolt host controller.
		 */
		for_each_pci_bridge(dev, bus) {
			if (PCI_SLOT(dev->devfn) == slot->device)
				acpiphp_native_scan_bridge(dev);
		}
	} else {
		LIST_HEAD(add_list);
		int max, pass;

		acpiphp_rescan_slot(slot);
		max = acpiphp_max_busnr(bus);
		for (pass = 0; pass < 2; pass++) {
			for_each_pci_bridge(dev, bus) {
				if (PCI_SLOT(dev->devfn) != slot->device)
					continue;

				max = pci_scan_bridge(bus, dev, max, pass);
				if (pass && dev->subordinate) {
					check_hotplug_bridge(slot, dev);
					pcibios_resource_survey_bus(dev->subordinate);
					__pci_bus_size_bridges(dev->subordinate,
							       &add_list);
				}
			}
		}
		__pci_bus_assign_resources(bus, &add_list, NULL);
	}

	acpiphp_sanitize_bus(bus);
	pcie_bus_configure_settings(bus);
	acpiphp_set_acpi_region(slot);

	list_for_each_entry(dev, &bus->devices, bus_list) {
		/* Assume that newly added devices are powered on already. */
		if (!pci_dev_is_added(dev))
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
			dev->current_state = PCI_D0;
	}

	pci_bus_add_devices(bus);

	slot->flags |= SLOT_ENABLED;
	list_for_each_entry(func, &slot->funcs, sibling) {
		dev = pci_get_slot(bus, PCI_DEVFN(slot->device,
						  func->function));
		if (!dev) {
			/* Do not set SLOT_ENABLED flag if some funcs
			   are not added. */
<<<<<<< HEAD
			slot->flags &= (~SLOT_ENABLED);
			continue;
		}

		if (dev->hdr_type != PCI_HEADER_TYPE_BRIDGE &&
		    dev->hdr_type != PCI_HEADER_TYPE_CARDBUS) {
			pci_dev_put(dev);
			continue;
		}

		status = find_p2p_bridge(func->handle, (u32)1, bus, NULL);
		if (ACPI_FAILURE(status))
			warn("find_p2p_bridge failed (error code = 0x%x)\n",
				status);
		pci_dev_put(dev);
	}


 err_exit:
	return retval;
}

static void disable_bridges(struct pci_bus *bus)
{
	struct pci_dev *dev;
	list_for_each_entry(dev, &bus->devices, bus_list) {
		if (dev->subordinate) {
			disable_bridges(dev->subordinate);
			pci_disable_device(dev);
		}
	}
}

/**
 * disable_device - disable a slot
 * @slot: ACPI PHP slot
 */
static int disable_device(struct acpiphp_slot *slot)
{
	struct acpiphp_func *func;
	struct pci_dev *pdev;
	struct pci_bus *bus = slot->bridge->pci_bus;

	/* The slot will be enabled when func 0 is added, so check
	   func 0 before disable the slot. */
	pdev = pci_get_slot(bus, PCI_DEVFN(slot->device, 0));
	if (!pdev)
		goto err_exit;

	list_for_each_entry(func, &slot->funcs, sibling) {
		if (func->bridge) {
			/* cleanup p2p bridges under this P2P bridge */
			cleanup_p2p_bridge(func->bridge->handle,
						(u32)1, NULL, NULL);
			func->bridge = NULL;
		}

		pdev = pci_get_slot(slot->bridge->pci_bus,
				    PCI_DEVFN(slot->device, func->function));
		if (pdev) {
			pci_stop_bus_device(pdev);
			if (pdev->subordinate) {
				disable_bridges(pdev->subordinate);
				pci_disable_device(pdev);
			}
			__pci_remove_bus_device(pdev);
			pci_dev_put(pdev);
		}
	}

	list_for_each_entry(func, &slot->funcs, sibling) {
		acpiphp_bus_trim(func->handle);
	}

	slot->flags &= (~SLOT_ENABLED);

err_exit:
	return 0;
}

=======
			slot->flags &= ~SLOT_ENABLED;
			continue;
		}
		pci_dev_put(dev);
	}
}

/**
 * disable_slot - disable a slot
 * @slot: ACPI PHP slot
 */
static void disable_slot(struct acpiphp_slot *slot)
{
	struct pci_bus *bus = slot->bus;
	struct pci_dev *dev, *prev;
	struct acpiphp_func *func;

	/*
	 * enable_slot() enumerates all functions in this device via
	 * pci_scan_slot(), whether they have associated ACPI hotplug
	 * methods (_EJ0, etc.) or not.  Therefore, we remove all functions
	 * here.
	 */
	list_for_each_entry_safe_reverse(dev, prev, &bus->devices, bus_list)
		if (PCI_SLOT(dev->devfn) == slot->device)
			pci_stop_and_remove_bus_device(dev);

	list_for_each_entry(func, &slot->funcs, sibling)
		acpi_bus_trim(func_to_acpi_device(func));

	slot->flags &= ~SLOT_ENABLED;
}

static bool slot_no_hotplug(struct acpiphp_slot *slot)
{
	struct pci_bus *bus = slot->bus;
	struct pci_dev *dev;

	list_for_each_entry(dev, &bus->devices, bus_list) {
		if (PCI_SLOT(dev->devfn) == slot->device && dev->ignore_hotplug)
			return true;
	}
	return false;
}
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

/**
 * get_slot_status - get ACPI slot status
 * @slot: ACPI PHP slot
 *
 * If a slot has _STA for each function and if any one of them
 * returned non-zero status, return it.
 *
 * If a slot doesn't have _STA and if any one of its functions'
 * configuration space is configured, return 0x0f as a _STA.
 *
 * Otherwise return 0.
 */
static unsigned int get_slot_status(struct acpiphp_slot *slot)
{
<<<<<<< HEAD
	acpi_status status;
	unsigned long long sta = 0;
	u32 dvid;
	struct acpiphp_func *func;

	list_for_each_entry(func, &slot->funcs, sibling) {
		if (func->flags & FUNC_HAS_STA) {
			status = acpi_evaluate_integer(func->handle, "_STA", NULL, &sta);
			if (ACPI_SUCCESS(status) && sta)
				break;
		} else {
			pci_bus_read_config_dword(slot->bridge->pci_bus,
						  PCI_DEVFN(slot->device,
							    func->function),
						  PCI_VENDOR_ID, &dvid);
			if (dvid != 0xffffffff) {
=======
	unsigned long long sta = 0;
	struct acpiphp_func *func;
	u32 dvid;

	list_for_each_entry(func, &slot->funcs, sibling) {
		if (func->flags & FUNC_HAS_STA) {
			acpi_status status;

			status = acpi_evaluate_integer(func_to_handle(func),
						       "_STA", NULL, &sta);
			if (ACPI_SUCCESS(status) && sta)
				break;
		} else {
			if (pci_bus_read_dev_vendor_id(slot->bus,
					PCI_DEVFN(slot->device, func->function),
					&dvid, 0)) {
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
				sta = ACPI_STA_ALL;
				break;
			}
		}
	}

<<<<<<< HEAD
	return (unsigned int)sta;
}

/**
 * acpiphp_eject_slot - physically eject the slot
 * @slot: ACPI PHP slot
 */
int acpiphp_eject_slot(struct acpiphp_slot *slot)
{
	acpi_status status;
	struct acpiphp_func *func;
	struct acpi_object_list arg_list;
	union acpi_object arg;

	list_for_each_entry(func, &slot->funcs, sibling) {
		/* We don't want to call _EJ0 on non-existing functions. */
		if ((func->flags & FUNC_HAS_EJ0)) {
			/* _EJ0 method take one argument */
			arg_list.count = 1;
			arg_list.pointer = &arg;
			arg.type = ACPI_TYPE_INTEGER;
			arg.integer.value = 1;

			status = acpi_evaluate_object(func->handle, "_EJ0", &arg_list, NULL);
			if (ACPI_FAILURE(status)) {
				warn("%s: _EJ0 failed\n", __func__);
				return -1;
			} else
				break;
		}
	}
	return 0;
=======
	if (!sta) {
		/*
		 * Check for the slot itself since it may be that the
		 * ACPI slot is a device below PCIe upstream port so in
		 * that case it may not even be reachable yet.
		 */
		if (pci_bus_read_dev_vendor_id(slot->bus,
				PCI_DEVFN(slot->device, 0), &dvid, 0)) {
			sta = ACPI_STA_ALL;
		}
	}

	return (unsigned int)sta;
}

static inline bool device_status_valid(unsigned int sta)
{
	/*
	 * ACPI spec says that _STA may return bit 0 clear with bit 3 set
	 * if the device is valid but does not require a device driver to be
	 * loaded (Section 6.3.7 of ACPI 5.0A).
	 */
	unsigned int mask = ACPI_STA_DEVICE_ENABLED | ACPI_STA_DEVICE_FUNCTIONING;
	return (sta & mask) == mask;
}

/**
 * trim_stale_devices - remove PCI devices that are not responding.
 * @dev: PCI device to start walking the hierarchy from.
 */
static void trim_stale_devices(struct pci_dev *dev)
{
	struct acpi_device *adev = ACPI_COMPANION(&dev->dev);
	struct pci_bus *bus = dev->subordinate;
	bool alive = dev->ignore_hotplug;

	if (adev) {
		acpi_status status;
		unsigned long long sta;

		status = acpi_evaluate_integer(adev->handle, "_STA", NULL, &sta);
		alive = alive || (ACPI_SUCCESS(status) && device_status_valid(sta));
	}
	if (!alive)
		alive = pci_device_is_present(dev);

	if (!alive) {
		pci_dev_set_disconnected(dev, NULL);
		if (pci_has_subordinate(dev))
			pci_walk_bus(dev->subordinate, pci_dev_set_disconnected,
				     NULL);

		pci_stop_and_remove_bus_device(dev);
		if (adev)
			acpi_bus_trim(adev);
	} else if (bus) {
		struct pci_dev *child, *tmp;

		/* The device is a bridge. so check the bus below it. */
		pm_runtime_get_sync(&dev->dev);
		list_for_each_entry_safe_reverse(child, tmp, &bus->devices, bus_list)
			trim_stale_devices(child);

		pm_runtime_put(&dev->dev);
	}
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

/**
 * acpiphp_check_bridge - re-enumerate devices
 * @bridge: where to begin re-enumeration
 *
 * Iterate over all slots under this bridge and make sure that if a
 * card is present they are enabled, and if not they are disabled.
 */
<<<<<<< HEAD
static int acpiphp_check_bridge(struct acpiphp_bridge *bridge)
{
	struct acpiphp_slot *slot;
	int retval = 0;
	int enabled, disabled;

	enabled = disabled = 0;

	for (slot = bridge->slots; slot; slot = slot->next) {
		unsigned int status = get_slot_status(slot);
		if (slot->flags & SLOT_ENABLED) {
			if (status == ACPI_STA_ALL)
				continue;
			retval = acpiphp_disable_slot(slot);
			if (retval) {
				err("Error occurred in disabling\n");
				goto err_exit;
			} else {
				acpiphp_eject_slot(slot);
			}
			disabled++;
		} else {
			if (status != ACPI_STA_ALL)
				continue;
			retval = acpiphp_enable_slot(slot);
			if (retval) {
				err("Error occurred in enabling\n");
				goto err_exit;
			}
			enabled++;
		}
	}

	dbg("%s: %d enabled, %d disabled\n", __func__, enabled, disabled);

 err_exit:
	return retval;
}

static void acpiphp_set_hpp_values(struct pci_bus *bus)
{
	struct pci_dev *dev;

	list_for_each_entry(dev, &bus->devices, bus_list)
		pci_configure_slot(dev);
=======
static void acpiphp_check_bridge(struct acpiphp_bridge *bridge)
{
	struct acpiphp_slot *slot;

	/* Bail out if the bridge is going away. */
	if (bridge->is_going_away)
		return;

	if (bridge->pci_dev)
		pm_runtime_get_sync(&bridge->pci_dev->dev);

	list_for_each_entry(slot, &bridge->slots, node) {
		struct pci_bus *bus = slot->bus;
		struct pci_dev *dev, *tmp;

		if (slot_no_hotplug(slot)) {
			; /* do nothing */
		} else if (device_status_valid(get_slot_status(slot))) {
			/* remove stale devices if any */
			list_for_each_entry_safe_reverse(dev, tmp,
							 &bus->devices, bus_list)
				if (PCI_SLOT(dev->devfn) == slot->device)
					trim_stale_devices(dev);

			/* configure all functions */
			enable_slot(slot, true);
		} else {
			disable_slot(slot);
		}
	}

	if (bridge->pci_dev)
		pm_runtime_put(&bridge->pci_dev->dev);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

/*
 * Remove devices for which we could not assign resources, call
 * arch specific code to fix-up the bus
 */
static void acpiphp_sanitize_bus(struct pci_bus *bus)
{
<<<<<<< HEAD
	struct pci_dev *dev;
	int i;
	unsigned long type_mask = IORESOURCE_IO | IORESOURCE_MEM;

	list_for_each_entry(dev, &bus->devices, bus_list) {
		for (i=0; i<PCI_BRIDGE_RESOURCES; i++) {
=======
	struct pci_dev *dev, *tmp;
	int i;
	unsigned long type_mask = IORESOURCE_IO | IORESOURCE_MEM;

	list_for_each_entry_safe_reverse(dev, tmp, &bus->devices, bus_list) {
		for (i = 0; i < PCI_BRIDGE_RESOURCES; i++) {
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
			struct resource *res = &dev->resource[i];
			if ((res->flags & type_mask) && !res->start &&
					res->end) {
				/* Could not assign a required resources
				 * for this device, remove it */
				pci_stop_and_remove_bus_device(dev);
				break;
			}
		}
	}
}

<<<<<<< HEAD
/* Program resources in newly inserted bridge */
static int acpiphp_configure_bridge (acpi_handle handle)
{
	struct pci_bus *bus;

	if (acpi_is_root_bridge(handle)) {
		struct acpi_pci_root *root = acpi_pci_find_root(handle);
		bus = root->bus;
	} else {
		struct pci_dev *pdev = acpi_get_pci_dev(handle);
		bus = pdev->subordinate;
		pci_dev_put(pdev);
	}

	pci_bus_size_bridges(bus);
	pci_bus_assign_resources(bus);
	acpiphp_sanitize_bus(bus);
	acpiphp_set_hpp_values(bus);
	pci_enable_bridges(bus);
	return 0;
}

static void handle_bridge_insertion(acpi_handle handle, u32 type)
{
	struct acpi_device *device, *pdevice;
	acpi_handle phandle;

	if ((type != ACPI_NOTIFY_BUS_CHECK) &&
			(type != ACPI_NOTIFY_DEVICE_CHECK)) {
		err("unexpected notification type %d\n", type);
		return;
	}

	acpi_get_parent(handle, &phandle);
	if (acpi_bus_get_device(phandle, &pdevice)) {
		dbg("no parent device, assuming NULL\n");
		pdevice = NULL;
	}
	if (acpi_bus_add(&device, pdevice, handle, ACPI_BUS_TYPE_DEVICE)) {
		err("cannot add bridge to acpi list\n");
		return;
	}
	if (!acpiphp_configure_bridge(handle) &&
		!acpi_bus_start(device))
		add_bridge(handle);
	else
		err("cannot configure and start bridge\n");

}

=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
/*
 * ACPI event handlers
 */

<<<<<<< HEAD
static acpi_status
count_sub_bridges(acpi_handle handle, u32 lvl, void *context, void **rv)
{
	int *count = (int *)context;
	struct acpiphp_bridge *bridge;

	bridge = acpiphp_handle_to_bridge(handle);
	if (bridge)
		(*count)++;
	return AE_OK ;
}

static acpi_status
check_sub_bridges(acpi_handle handle, u32 lvl, void *context, void **rv)
{
	struct acpiphp_bridge *bridge;
	char objname[64];
	struct acpi_buffer buffer = { .length = sizeof(objname),
				      .pointer = objname };

	bridge = acpiphp_handle_to_bridge(handle);
	if (bridge) {
		acpi_get_name(handle, ACPI_FULL_PATHNAME, &buffer);
		dbg("%s: re-enumerating slots under %s\n",
			__func__, objname);
		acpiphp_check_bridge(bridge);
	}
	return AE_OK ;
}

struct acpiphp_hp_work {
	struct work_struct work;
	acpi_handle handle;
	u32 type;
	void *context;
};

static void alloc_acpiphp_hp_work(acpi_handle handle, u32 type,
				  void *context,
				  void (*func)(struct work_struct *work))
{
	struct acpiphp_hp_work *hp_work;
	int ret;

	hp_work = kmalloc(sizeof(*hp_work), GFP_KERNEL);
	if (!hp_work)
		return;

	hp_work->handle = handle;
	hp_work->type = type;
	hp_work->context = context;

	INIT_WORK(&hp_work->work, func);
	ret = queue_work(kacpi_hotplug_wq, &hp_work->work);
	if (!ret)
		kfree(hp_work);
}

static void _handle_hotplug_event_bridge(struct work_struct *work)
{
	struct acpiphp_bridge *bridge;
	char objname[64];
	struct acpi_buffer buffer = { .length = sizeof(objname),
				      .pointer = objname };
	struct acpi_device *device;
	int num_sub_bridges = 0;
	struct acpiphp_hp_work *hp_work;
	acpi_handle handle;
	u32 type;

	hp_work = container_of(work, struct acpiphp_hp_work, work);
	handle = hp_work->handle;
	type = hp_work->type;

	if (acpi_bus_get_device(handle, &device)) {
		/* This bridge must have just been physically inserted */
		handle_bridge_insertion(handle, type);
		goto out;
	}

	bridge = acpiphp_handle_to_bridge(handle);
	if (type == ACPI_NOTIFY_BUS_CHECK) {
		acpi_walk_namespace(ACPI_TYPE_DEVICE, handle, ACPI_UINT32_MAX,
			count_sub_bridges, NULL, &num_sub_bridges, NULL);
	}

	if (!bridge && !num_sub_bridges) {
		err("cannot get bridge info\n");
		goto out;
	}

	acpi_get_name(handle, ACPI_FULL_PATHNAME, &buffer);
=======
void acpiphp_check_host_bridge(struct acpi_device *adev)
{
	struct acpiphp_bridge *bridge = NULL;

	acpi_lock_hp_context();
	if (adev->hp) {
		bridge = to_acpiphp_root_context(adev->hp)->root_bridge;
		if (bridge)
			get_bridge(bridge);
	}
	acpi_unlock_hp_context();
	if (bridge) {
		pci_lock_rescan_remove();

		acpiphp_check_bridge(bridge);

		pci_unlock_rescan_remove();
		put_bridge(bridge);
	}
}

static int acpiphp_disable_and_eject_slot(struct acpiphp_slot *slot);

static void hotplug_event(u32 type, struct acpiphp_context *context)
{
	acpi_handle handle = context->hp.self->handle;
	struct acpiphp_func *func = &context->func;
	struct acpiphp_slot *slot = func->slot;
	struct acpiphp_bridge *bridge;

	acpi_lock_hp_context();
	bridge = context->bridge;
	if (bridge)
		get_bridge(bridge);

	acpi_unlock_hp_context();

	pci_lock_rescan_remove();
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	switch (type) {
	case ACPI_NOTIFY_BUS_CHECK:
		/* bus re-enumerate */
<<<<<<< HEAD
		dbg("%s: Bus check notify on %s\n", __func__, objname);
		if (bridge) {
			dbg("%s: re-enumerating slots under %s\n",
				__func__, objname);
			acpiphp_check_bridge(bridge);
		}
		if (num_sub_bridges)
			acpi_walk_namespace(ACPI_TYPE_DEVICE, handle,
				ACPI_UINT32_MAX, check_sub_bridges, NULL, NULL, NULL);
=======
		acpi_handle_debug(handle, "Bus check in %s()\n", __func__);
		if (bridge)
			acpiphp_check_bridge(bridge);
		else if (!(slot->flags & SLOT_IS_GOING_AWAY))
			enable_slot(slot, false);

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		break;

	case ACPI_NOTIFY_DEVICE_CHECK:
		/* device check */
<<<<<<< HEAD
		dbg("%s: Device check notify on %s\n", __func__, objname);
		acpiphp_check_bridge(bridge);
		break;

	case ACPI_NOTIFY_DEVICE_WAKE:
		/* wake event */
		dbg("%s: Device wake notify on %s\n", __func__, objname);
		break;

	case ACPI_NOTIFY_EJECT_REQUEST:
		/* request device eject */
		dbg("%s: Device eject notify on %s\n", __func__, objname);
		if ((bridge->type != BRIDGE_TYPE_HOST) &&
		    (bridge->flags & BRIDGE_HAS_EJ0)) {
			struct acpiphp_slot *slot;
			slot = bridge->func->slot;
			if (!acpiphp_disable_slot(slot))
				acpiphp_eject_slot(slot);
		}
		break;

	case ACPI_NOTIFY_FREQUENCY_MISMATCH:
		printk(KERN_ERR "Device %s cannot be configured due"
				" to a frequency mismatch\n", objname);
		break;

	case ACPI_NOTIFY_BUS_MODE_MISMATCH:
		printk(KERN_ERR "Device %s cannot be configured due"
				" to a bus mode mismatch\n", objname);
		break;

	case ACPI_NOTIFY_POWER_FAULT:
		printk(KERN_ERR "Device %s has suffered a power fault\n",
				objname);
		break;

	default:
		warn("notify_handler: unknown event type 0x%x for %s\n", type, objname);
		break;
	}

out:
	kfree(hp_work); /* allocated in handle_hotplug_event_bridge */
}

/**
 * handle_hotplug_event_bridge - handle ACPI event on bridges
 * @handle: Notify()'ed acpi_handle
 * @type: Notify code
 * @context: pointer to acpiphp_bridge structure
 *
 * Handles ACPI event notification on {host,p2p} bridges.
 */
static void handle_hotplug_event_bridge(acpi_handle handle, u32 type,
					void *context)
{
	/*
	 * Currently the code adds all hotplug events to the kacpid_wq
	 * queue when it should add hotplug events to the kacpi_hotplug_wq.
	 * The proper way to fix this is to reorganize the code so that
	 * drivers (dock, etc.) do not call acpi_os_execute(), etc.
	 * For now just re-add this work to the kacpi_hotplug_wq so we
	 * don't deadlock on hotplug actions.
	 */
	alloc_acpiphp_hp_work(handle, type, context,
			      _handle_hotplug_event_bridge);
}

static void _handle_hotplug_event_func(struct work_struct *work)
{
	struct acpiphp_func *func;
	char objname[64];
	struct acpi_buffer buffer = { .length = sizeof(objname),
				      .pointer = objname };
	struct acpiphp_hp_work *hp_work;
	acpi_handle handle;
	u32 type;
	void *context;

	hp_work = container_of(work, struct acpiphp_hp_work, work);
	handle = hp_work->handle;
	type = hp_work->type;
	context = hp_work->context;

	acpi_get_name(handle, ACPI_FULL_PATHNAME, &buffer);

	func = (struct acpiphp_func *)context;

	switch (type) {
	case ACPI_NOTIFY_BUS_CHECK:
		/* bus re-enumerate */
		dbg("%s: Bus check notify on %s\n", __func__, objname);
		acpiphp_enable_slot(func->slot);
		break;

	case ACPI_NOTIFY_DEVICE_CHECK:
		/* device check : re-enumerate from parent bus */
		dbg("%s: Device check notify on %s\n", __func__, objname);
		acpiphp_check_bridge(func->slot->bridge);
		break;

	case ACPI_NOTIFY_DEVICE_WAKE:
		/* wake event */
		dbg("%s: Device wake notify on %s\n", __func__, objname);
		break;

	case ACPI_NOTIFY_EJECT_REQUEST:
		/* request device eject */
		dbg("%s: Device eject notify on %s\n", __func__, objname);
		if (!(acpiphp_disable_slot(func->slot)))
			acpiphp_eject_slot(func->slot);
		break;

	default:
		warn("notify_handler: unknown event type 0x%x for %s\n", type, objname);
		break;
	}

	kfree(hp_work); /* allocated in handle_hotplug_event_func */
}

/**
 * handle_hotplug_event_func - handle ACPI event on functions (i.e. slots)
 * @handle: Notify()'ed acpi_handle
 * @type: Notify code
 * @context: pointer to acpiphp_func structure
 *
 * Handles ACPI event notification on slots.
 */
static void handle_hotplug_event_func(acpi_handle handle, u32 type,
				      void *context)
{
	/*
	 * Currently the code adds all hotplug events to the kacpid_wq
	 * queue when it should add hotplug events to the kacpi_hotplug_wq.
	 * The proper way to fix this is to reorganize the code so that
	 * drivers (dock, etc.) do not call acpi_os_execute(), etc.
	 * For now just re-add this work to the kacpi_hotplug_wq so we
	 * don't deadlock on hotplug actions.
	 */
	alloc_acpiphp_hp_work(handle, type, context,
			      _handle_hotplug_event_func);
}

static acpi_status
find_root_bridges(acpi_handle handle, u32 lvl, void *context, void **rv)
{
	int *count = (int *)context;

	if (!acpi_is_root_bridge(handle))
		return AE_OK;

	(*count)++;
	acpi_install_notify_handler(handle, ACPI_SYSTEM_NOTIFY,
				    handle_hotplug_event_bridge, NULL);

	return AE_OK ;
}

static struct acpi_pci_driver acpi_pci_hp_driver = {
	.add =		add_bridge,
	.remove =	remove_bridge,
};

/**
 * acpiphp_glue_init - initializes all PCI hotplug - ACPI glue data structures
 */
int __init acpiphp_glue_init(void)
{
	int num = 0;

	acpi_walk_namespace(ACPI_TYPE_DEVICE, ACPI_ROOT_OBJECT,
			ACPI_UINT32_MAX, find_root_bridges, NULL, &num, NULL);

	if (num <= 0)
		return -1;
	else
		acpi_pci_register_driver(&acpi_pci_hp_driver);

	return 0;
}


/**
 * acpiphp_glue_exit - terminates all PCI hotplug - ACPI glue data structures
 *
 * This function frees all data allocated in acpiphp_glue_init().
 */
void  acpiphp_glue_exit(void)
{
	acpi_pci_unregister_driver(&acpi_pci_hp_driver);
}


/**
 * acpiphp_get_num_slots - count number of slots in a system
 */
int __init acpiphp_get_num_slots(void)
{
	struct acpiphp_bridge *bridge;
	int num_slots = 0;

	list_for_each_entry(bridge, &bridge_list, list) {
		dbg("Bus %04x:%02x has %d slot%s\n",
				pci_domain_nr(bridge->pci_bus),
				bridge->pci_bus->number, bridge->nr_slots,
				bridge->nr_slots == 1 ? "" : "s");
		num_slots += bridge->nr_slots;
	}

	dbg("Total %d slots\n", num_slots);
	return num_slots;
}


#if 0
/**
 * acpiphp_for_each_slot - call function for each slot
 * @fn: callback function
 * @data: context to be passed to callback function
 */
static int acpiphp_for_each_slot(acpiphp_callback fn, void *data)
{
	struct list_head *node;
	struct acpiphp_bridge *bridge;
	struct acpiphp_slot *slot;
	int retval = 0;

	list_for_each (node, &bridge_list) {
		bridge = (struct acpiphp_bridge *)node;
		for (slot = bridge->slots; slot; slot = slot->next) {
			retval = fn(slot, data);
			if (!retval)
				goto err_exit;
		}
	}

 err_exit:
	return retval;
}
#endif

=======
		acpi_handle_debug(handle, "Device check in %s()\n", __func__);
		if (bridge) {
			acpiphp_check_bridge(bridge);
		} else if (!(slot->flags & SLOT_IS_GOING_AWAY)) {
			/*
			 * Check if anything has changed in the slot and rescan
			 * from the parent if that's the case.
			 */
			if (acpiphp_rescan_slot(slot))
				acpiphp_check_bridge(func->parent);
		}
		break;

	case ACPI_NOTIFY_EJECT_REQUEST:
		/* request device eject */
		acpi_handle_debug(handle, "Eject request in %s()\n", __func__);
		acpiphp_disable_and_eject_slot(slot);
		break;
	}

	pci_unlock_rescan_remove();
	if (bridge)
		put_bridge(bridge);
}

static int acpiphp_hotplug_notify(struct acpi_device *adev, u32 type)
{
	struct acpiphp_context *context;

	context = acpiphp_grab_context(adev);
	if (!context)
		return -ENODATA;

	hotplug_event(type, context);
	acpiphp_let_context_go(context);
	return 0;
}

/**
 * acpiphp_enumerate_slots - Enumerate PCI slots for a given bus.
 * @bus: PCI bus to enumerate the slots for.
 *
 * A "slot" is an object associated with a PCI device number.  All functions
 * (PCI devices) with the same bus and device number belong to the same slot.
 */
void acpiphp_enumerate_slots(struct pci_bus *bus)
{
	struct acpiphp_bridge *bridge;
	struct acpi_device *adev;
	acpi_handle handle;
	acpi_status status;

	if (acpiphp_disabled)
		return;

	adev = ACPI_COMPANION(bus->bridge);
	if (!adev)
		return;

	handle = adev->handle;
	bridge = kzalloc(sizeof(struct acpiphp_bridge), GFP_KERNEL);
	if (!bridge)
		return;

	INIT_LIST_HEAD(&bridge->slots);
	kref_init(&bridge->ref);
	bridge->pci_dev = pci_dev_get(bus->self);
	bridge->pci_bus = bus;

	/*
	 * Grab a ref to the subordinate PCI bus in case the bus is
	 * removed via PCI core logical hotplug. The ref pins the bus
	 * (which we access during module unload).
	 */
	get_device(&bus->dev);

	acpi_lock_hp_context();
	if (pci_is_root_bus(bridge->pci_bus)) {
		struct acpiphp_root_context *root_context;

		root_context = kzalloc(sizeof(*root_context), GFP_KERNEL);
		if (!root_context)
			goto err;

		root_context->root_bridge = bridge;
		acpi_set_hp_context(adev, &root_context->hp);
	} else {
		struct acpiphp_context *context;

		/*
		 * This bridge should have been registered as a hotplug function
		 * under its parent, so the context should be there, unless the
		 * parent is going to be handled by pciehp, in which case this
		 * bridge is not interesting to us either.
		 */
		context = acpiphp_get_context(adev);
		if (!context)
			goto err;

		bridge->context = context;
		context->bridge = bridge;
		/* Get a reference to the parent bridge. */
		get_bridge(context->func.parent);
	}
	acpi_unlock_hp_context();

	/* Must be added to the list prior to calling acpiphp_add_context(). */
	mutex_lock(&bridge_mutex);
	list_add(&bridge->list, &bridge_list);
	mutex_unlock(&bridge_mutex);

	/* register all slot objects under this bridge */
	status = acpi_walk_namespace(ACPI_TYPE_DEVICE, handle, 1,
				     acpiphp_add_context, NULL, bridge, NULL);
	if (ACPI_FAILURE(status)) {
		acpi_handle_err(handle, "failed to register slots\n");
		cleanup_bridge(bridge);
		put_bridge(bridge);
	}
	return;

 err:
	acpi_unlock_hp_context();
	put_device(&bus->dev);
	pci_dev_put(bridge->pci_dev);
	kfree(bridge);
}

static void acpiphp_drop_bridge(struct acpiphp_bridge *bridge)
{
	if (pci_is_root_bus(bridge->pci_bus)) {
		struct acpiphp_root_context *root_context;
		struct acpi_device *adev;

		acpi_lock_hp_context();
		adev = ACPI_COMPANION(bridge->pci_bus->bridge);
		root_context = to_acpiphp_root_context(adev->hp);
		adev->hp = NULL;
		acpi_unlock_hp_context();
		kfree(root_context);
	}
	cleanup_bridge(bridge);
	put_bridge(bridge);
}

/**
 * acpiphp_remove_slots - Remove slot objects associated with a given bus.
 * @bus: PCI bus to remove the slot objects for.
 */
void acpiphp_remove_slots(struct pci_bus *bus)
{
	struct acpiphp_bridge *bridge;

	if (acpiphp_disabled)
		return;

	mutex_lock(&bridge_mutex);
	list_for_each_entry(bridge, &bridge_list, list)
		if (bridge->pci_bus == bus) {
			mutex_unlock(&bridge_mutex);
			acpiphp_drop_bridge(bridge);
			return;
		}

	mutex_unlock(&bridge_mutex);
}
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

/**
 * acpiphp_enable_slot - power on slot
 * @slot: ACPI PHP slot
 */
int acpiphp_enable_slot(struct acpiphp_slot *slot)
{
<<<<<<< HEAD
	int retval;

	mutex_lock(&slot->crit_sect);

	/* wake up all functions */
	retval = power_on_slot(slot);
	if (retval)
		goto err_exit;

	if (get_slot_status(slot) == ACPI_STA_ALL) {
		/* configure all functions */
		retval = enable_device(slot);
		if (retval)
			power_off_slot(slot);
	} else {
		dbg("%s: Slot status is not ACPI_STA_ALL\n", __func__);
		power_off_slot(slot);
	}

 err_exit:
	mutex_unlock(&slot->crit_sect);
	return retval;
}

/**
 * acpiphp_disable_slot - power off slot
 * @slot: ACPI PHP slot
 */
int acpiphp_disable_slot(struct acpiphp_slot *slot)
{
	int retval = 0;

	mutex_lock(&slot->crit_sect);

	/* unconfigure all functions */
	retval = disable_device(slot);
	if (retval)
		goto err_exit;

	/* power off all functions */
	retval = power_off_slot(slot);
	if (retval)
		goto err_exit;

 err_exit:
	mutex_unlock(&slot->crit_sect);
	return retval;
}

=======
	pci_lock_rescan_remove();

	if (slot->flags & SLOT_IS_GOING_AWAY) {
		pci_unlock_rescan_remove();
		return -ENODEV;
	}

	/* configure all functions */
	if (!(slot->flags & SLOT_ENABLED))
		enable_slot(slot, false);

	pci_unlock_rescan_remove();
	return 0;
}

/**
 * acpiphp_disable_and_eject_slot - power off and eject slot
 * @slot: ACPI PHP slot
 */
static int acpiphp_disable_and_eject_slot(struct acpiphp_slot *slot)
{
	struct acpiphp_func *func;

	if (slot->flags & SLOT_IS_GOING_AWAY)
		return -ENODEV;

	/* unconfigure all functions */
	disable_slot(slot);

	list_for_each_entry(func, &slot->funcs, sibling)
		if (func->flags & FUNC_HAS_EJ0) {
			acpi_handle handle = func_to_handle(func);

			if (ACPI_FAILURE(acpi_evaluate_ej0(handle)))
				acpi_handle_err(handle, "_EJ0 failed\n");

			break;
		}

	return 0;
}

int acpiphp_disable_slot(struct acpiphp_slot *slot)
{
	int ret;

	/*
	 * Acquire acpi_scan_lock to ensure that the execution of _EJ0 in
	 * acpiphp_disable_and_eject_slot() will be synchronized properly.
	 */
	acpi_scan_lock_acquire();
	pci_lock_rescan_remove();
	ret = acpiphp_disable_and_eject_slot(slot);
	pci_unlock_rescan_remove();
	acpi_scan_lock_release();
	return ret;
}
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

/*
 * slot enabled:  1
 * slot disabled: 0
 */
u8 acpiphp_get_power_status(struct acpiphp_slot *slot)
{
<<<<<<< HEAD
	return (slot->flags & SLOT_POWEREDON);
}


=======
	return (slot->flags & SLOT_ENABLED);
}

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
/*
 * latch   open:  1
 * latch closed:  0
 */
u8 acpiphp_get_latch_status(struct acpiphp_slot *slot)
{
<<<<<<< HEAD
	unsigned int sta;

	sta = get_slot_status(slot);

	return (sta & ACPI_STA_SHOW_IN_UI) ? 0 : 1;
}


=======
	return !(get_slot_status(slot) & ACPI_STA_DEVICE_UI);
}

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
/*
 * adapter presence : 1
 *          absence : 0
 */
u8 acpiphp_get_adapter_status(struct acpiphp_slot *slot)
{
<<<<<<< HEAD
	unsigned int sta;

	sta = get_slot_status(slot);

	return (sta == 0) ? 0 : 1;
=======
	return !!get_slot_status(slot);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}
