<<<<<<< HEAD
=======
// SPDX-License-Identifier: GPL-2.0+
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
/*
 * PCI Express Hot Plug Controller Driver
 *
 * Copyright (C) 1995,2001 Compaq Computer Corporation
 * Copyright (C) 2001 Greg Kroah-Hartman (greg@kroah.com)
 * Copyright (C) 2001 IBM Corp.
 * Copyright (C) 2003-2004 Intel Corporation
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
 * Send feedback to <greg@kroah.com>, <kristen.c.accardi@intel.com>
 *
 */

#include <linux/module.h>
=======
 * Send feedback to <greg@kroah.com>, <kristen.c.accardi@intel.com>
 *
 * Authors:
 *   Dan Zink <dan.zink@compaq.com>
 *   Greg Kroah-Hartman <greg@kroah.com>
 *   Dely Sy <dely.l.sy@intel.com>"
 */

#define pr_fmt(fmt) "pciehp: " fmt
#define dev_fmt pr_fmt

#include <linux/bitfield.h>
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#include <linux/moduleparam.h>
#include <linux/kernel.h>
#include <linux/slab.h>
#include <linux/types.h>
#include <linux/pci.h>
#include "pciehp.h"
<<<<<<< HEAD
#include <linux/interrupt.h>
#include <linux/time.h>

/* Global variables */
bool pciehp_debug;
bool pciehp_poll_mode;
int pciehp_poll_time;
bool pciehp_force;

#define DRIVER_VERSION	"0.4"
#define DRIVER_AUTHOR	"Dan Zink <dan.zink@compaq.com>, Greg Kroah-Hartman <greg@kroah.com>, Dely Sy <dely.l.sy@intel.com>"
#define DRIVER_DESC	"PCI Express Hot Plug Controller Driver"

MODULE_AUTHOR(DRIVER_AUTHOR);
MODULE_DESCRIPTION(DRIVER_DESC);
MODULE_LICENSE("GPL");

module_param(pciehp_debug, bool, 0644);
module_param(pciehp_poll_mode, bool, 0644);
module_param(pciehp_poll_time, int, 0644);
module_param(pciehp_force, bool, 0644);
MODULE_PARM_DESC(pciehp_debug, "Debugging mode enabled or not");
MODULE_PARM_DESC(pciehp_poll_mode, "Using polling mechanism for hot-plug events or not");
MODULE_PARM_DESC(pciehp_poll_time, "Polling mechanism frequency, in seconds");
MODULE_PARM_DESC(pciehp_force, "Force pciehp, even if OSHP is missing");

#define PCIE_MODULE_NAME "pciehp"

static int set_attention_status (struct hotplug_slot *slot, u8 value);
static int enable_slot		(struct hotplug_slot *slot);
static int disable_slot		(struct hotplug_slot *slot);
static int get_power_status	(struct hotplug_slot *slot, u8 *value);
static int get_attention_status	(struct hotplug_slot *slot, u8 *value);
static int get_latch_status	(struct hotplug_slot *slot, u8 *value);
static int get_adapter_status	(struct hotplug_slot *slot, u8 *value);

/**
 * release_slot - free up the memory used by a slot
 * @hotplug_slot: slot to free
 */
static void release_slot(struct hotplug_slot *hotplug_slot)
{
	struct slot *slot = hotplug_slot->private;

	ctrl_dbg(slot->ctrl, "%s: physical_slot = %s\n",
		 __func__, hotplug_slot_name(hotplug_slot));

	kfree(hotplug_slot->ops);
	kfree(hotplug_slot->info);
	kfree(hotplug_slot);
}

static int init_slot(struct controller *ctrl)
{
	struct slot *slot = ctrl->slot;
	struct hotplug_slot *hotplug = NULL;
	struct hotplug_slot_info *info = NULL;
	struct hotplug_slot_ops *ops = NULL;
	char name[SLOT_NAME_SIZE];
	int retval = -ENOMEM;

	hotplug = kzalloc(sizeof(*hotplug), GFP_KERNEL);
	if (!hotplug)
		goto out;

	info = kzalloc(sizeof(*info), GFP_KERNEL);
	if (!info)
		goto out;
=======

#include "../pci.h"

/* Global variables */
bool pciehp_poll_mode;
int pciehp_poll_time;

/*
 * not really modular, but the easiest way to keep compat with existing
 * bootargs behaviour is to continue using module_param here.
 */
module_param(pciehp_poll_mode, bool, 0644);
module_param(pciehp_poll_time, int, 0644);
MODULE_PARM_DESC(pciehp_poll_mode, "Using polling mechanism for hot-plug events or not");
MODULE_PARM_DESC(pciehp_poll_time, "Polling mechanism frequency, in seconds");

static int set_attention_status(struct hotplug_slot *slot, u8 value);
static int get_power_status(struct hotplug_slot *slot, u8 *value);
static int get_latch_status(struct hotplug_slot *slot, u8 *value);
static int get_adapter_status(struct hotplug_slot *slot, u8 *value);

static int init_slot(struct controller *ctrl)
{
	struct hotplug_slot_ops *ops;
	char name[SLOT_NAME_SIZE];
	int retval;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	/* Setup hotplug slot ops */
	ops = kzalloc(sizeof(*ops), GFP_KERNEL);
	if (!ops)
<<<<<<< HEAD
		goto out;
	ops->enable_slot = enable_slot;
	ops->disable_slot = disable_slot;
	ops->get_power_status = get_power_status;
	ops->get_adapter_status = get_adapter_status;
	if (MRL_SENS(ctrl))
		ops->get_latch_status = get_latch_status;
	if (ATTN_LED(ctrl)) {
		ops->get_attention_status = get_attention_status;
		ops->set_attention_status = set_attention_status;
	}

	/* register this slot with the hotplug pci core */
	hotplug->info = info;
	hotplug->private = slot;
	hotplug->release = &release_slot;
	hotplug->ops = ops;
	slot->hotplug_slot = hotplug;
	snprintf(name, SLOT_NAME_SIZE, "%u", PSN(ctrl));

	ctrl_dbg(ctrl, "Registering domain:bus:dev=%04x:%02x:00 sun=%x\n",
		 pci_domain_nr(ctrl->pcie->port->subordinate),
		 ctrl->pcie->port->subordinate->number, PSN(ctrl));
	retval = pci_hp_register(hotplug,
				 ctrl->pcie->port->subordinate, 0, name);
	if (retval)
		ctrl_err(ctrl,
			 "pci_hp_register failed with error %d\n", retval);
out:
	if (retval) {
		kfree(ops);
		kfree(info);
		kfree(hotplug);
=======
		return -ENOMEM;

	ops->enable_slot = pciehp_sysfs_enable_slot;
	ops->disable_slot = pciehp_sysfs_disable_slot;
	ops->get_power_status = get_power_status;
	ops->get_adapter_status = get_adapter_status;
	ops->reset_slot = pciehp_reset_slot;
	if (MRL_SENS(ctrl))
		ops->get_latch_status = get_latch_status;
	if (ATTN_LED(ctrl)) {
		ops->get_attention_status = pciehp_get_attention_status;
		ops->set_attention_status = set_attention_status;
	} else if (ctrl->pcie->port->hotplug_user_indicators) {
		ops->get_attention_status = pciehp_get_raw_indicator_status;
		ops->set_attention_status = pciehp_set_raw_indicator_status;
	}

	/* register this slot with the hotplug pci core */
	ctrl->hotplug_slot.ops = ops;
	snprintf(name, SLOT_NAME_SIZE, "%u", PSN(ctrl));

	retval = pci_hp_initialize(&ctrl->hotplug_slot,
				   ctrl->pcie->port->subordinate, 0, name);
	if (retval) {
		ctrl_err(ctrl, "pci_hp_initialize failed: error %d\n", retval);
		kfree(ops);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	}
	return retval;
}

static void cleanup_slot(struct controller *ctrl)
{
<<<<<<< HEAD
	pci_hp_deregister(ctrl->slot->hotplug_slot);
}

/*
 * set_attention_status - Turns the Amber LED for a slot on, off or blink
 */
static int set_attention_status(struct hotplug_slot *hotplug_slot, u8 status)
{
	struct slot *slot = hotplug_slot->private;

	ctrl_dbg(slot->ctrl, "%s: physical_slot = %s\n",
		  __func__, slot_name(slot));

	return pciehp_set_attention_status(slot, status);
}


static int enable_slot(struct hotplug_slot *hotplug_slot)
{
	struct slot *slot = hotplug_slot->private;

	ctrl_dbg(slot->ctrl, "%s: physical_slot = %s\n",
		 __func__, slot_name(slot));

	return pciehp_sysfs_enable_slot(slot);
}


static int disable_slot(struct hotplug_slot *hotplug_slot)
{
	struct slot *slot = hotplug_slot->private;

	ctrl_dbg(slot->ctrl, "%s: physical_slot = %s\n",
		  __func__, slot_name(slot));

	return pciehp_sysfs_disable_slot(slot);
=======
	struct hotplug_slot *hotplug_slot = &ctrl->hotplug_slot;

	pci_hp_destroy(hotplug_slot);
	kfree(hotplug_slot->ops);
}

/*
 * set_attention_status - Turns the Attention Indicator on, off or blinking
 */
static int set_attention_status(struct hotplug_slot *hotplug_slot, u8 status)
{
	struct controller *ctrl = to_ctrl(hotplug_slot);
	struct pci_dev *pdev = ctrl->pcie->port;

	if (status)
		status = FIELD_PREP(PCI_EXP_SLTCTL_AIC, status);
	else
		status = PCI_EXP_SLTCTL_ATTN_IND_OFF;

	pci_config_pm_runtime_get(pdev);
	pciehp_set_indicators(ctrl, INDICATOR_NOOP, status);
	pci_config_pm_runtime_put(pdev);
	return 0;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

static int get_power_status(struct hotplug_slot *hotplug_slot, u8 *value)
{
<<<<<<< HEAD
	struct slot *slot = hotplug_slot->private;

	ctrl_dbg(slot->ctrl, "%s: physical_slot = %s\n",
		  __func__, slot_name(slot));

	return pciehp_get_power_status(slot, value);
}

static int get_attention_status(struct hotplug_slot *hotplug_slot, u8 *value)
{
	struct slot *slot = hotplug_slot->private;

	ctrl_dbg(slot->ctrl, "%s: physical_slot = %s\n",
		  __func__, slot_name(slot));

	return pciehp_get_attention_status(slot, value);
=======
	struct controller *ctrl = to_ctrl(hotplug_slot);
	struct pci_dev *pdev = ctrl->pcie->port;

	pci_config_pm_runtime_get(pdev);
	pciehp_get_power_status(ctrl, value);
	pci_config_pm_runtime_put(pdev);
	return 0;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

static int get_latch_status(struct hotplug_slot *hotplug_slot, u8 *value)
{
<<<<<<< HEAD
	struct slot *slot = hotplug_slot->private;

	ctrl_dbg(slot->ctrl, "%s: physical_slot = %s\n",
		 __func__, slot_name(slot));

	return pciehp_get_latch_status(slot, value);
=======
	struct controller *ctrl = to_ctrl(hotplug_slot);
	struct pci_dev *pdev = ctrl->pcie->port;

	pci_config_pm_runtime_get(pdev);
	pciehp_get_latch_status(ctrl, value);
	pci_config_pm_runtime_put(pdev);
	return 0;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

static int get_adapter_status(struct hotplug_slot *hotplug_slot, u8 *value)
{
<<<<<<< HEAD
	struct slot *slot = hotplug_slot->private;

	ctrl_dbg(slot->ctrl, "%s: physical_slot = %s\n",
		 __func__, slot_name(slot));

	return pciehp_get_adapter_status(slot, value);
=======
	struct controller *ctrl = to_ctrl(hotplug_slot);
	struct pci_dev *pdev = ctrl->pcie->port;
	int ret;

	pci_config_pm_runtime_get(pdev);
	ret = pciehp_card_present_or_link_active(ctrl);
	pci_config_pm_runtime_put(pdev);
	if (ret < 0)
		return ret;

	*value = ret;
	return 0;
}

/**
 * pciehp_check_presence() - synthesize event if presence has changed
 * @ctrl: controller to check
 *
 * On probe and resume, an explicit presence check is necessary to bring up an
 * occupied slot or bring down an unoccupied slot.  This can't be triggered by
 * events in the Slot Status register, they may be stale and are therefore
 * cleared.  Secondly, sending an interrupt for "events that occur while
 * interrupt generation is disabled [when] interrupt generation is subsequently
 * enabled" is optional per PCIe r4.0, sec 6.7.3.4.
 */
static void pciehp_check_presence(struct controller *ctrl)
{
	int occupied;

	down_read_nested(&ctrl->reset_lock, ctrl->depth);
	mutex_lock(&ctrl->state_lock);

	occupied = pciehp_card_present_or_link_active(ctrl);
	if ((occupied > 0 && (ctrl->state == OFF_STATE ||
			  ctrl->state == BLINKINGON_STATE)) ||
	    (!occupied && (ctrl->state == ON_STATE ||
			   ctrl->state == BLINKINGOFF_STATE)))
		pciehp_request(ctrl, PCI_EXP_SLTSTA_PDC);

	mutex_unlock(&ctrl->state_lock);
	up_read(&ctrl->reset_lock);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

static int pciehp_probe(struct pcie_device *dev)
{
	int rc;
	struct controller *ctrl;
<<<<<<< HEAD
	struct slot *slot;
	u8 occupied, poweron;

	if (pciehp_force)
		dev_info(&dev->device,
			 "Bypassing BIOS check for pciehp use on %s\n",
			 pci_name(dev->port));
	else if (pciehp_acpi_slot_detection_check(dev->port))
		goto err_out_none;

	if (!dev->port->subordinate) {
		/* Can happen if we run out of bus numbers during probe */
		dev_err(&dev->device,
			"Hotplug bridge without secondary bus, ignoring\n");
		goto err_out_none;
=======

	/* If this is not a "hotplug" service, we have no business here. */
	if (dev->service != PCIE_PORT_SERVICE_HP)
		return -ENODEV;

	if (!dev->port->subordinate) {
		/* Can happen if we run out of bus numbers during probe */
		pci_err(dev->port,
			"Hotplug bridge without secondary bus, ignoring\n");
		return -ENODEV;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	}

	ctrl = pcie_init(dev);
	if (!ctrl) {
<<<<<<< HEAD
		dev_err(&dev->device, "Controller initialization failed\n");
		goto err_out_none;
=======
		pci_err(dev->port, "Controller initialization failed\n");
		return -ENODEV;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	}
	set_service_data(dev, ctrl);

	/* Setup the slot information structures */
	rc = init_slot(ctrl);
	if (rc) {
		if (rc == -EBUSY)
<<<<<<< HEAD
			ctrl_warn(ctrl, "Slot already registered by another "
				  "hotplug driver\n");
		else
			ctrl_err(ctrl, "Slot initialization failed\n");
=======
			ctrl_warn(ctrl, "Slot already registered by another hotplug driver\n");
		else
			ctrl_err(ctrl, "Slot initialization failed (%d)\n", rc);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		goto err_out_release_ctlr;
	}

	/* Enable events after we have setup the data structures */
	rc = pcie_init_notification(ctrl);
	if (rc) {
<<<<<<< HEAD
		ctrl_err(ctrl, "Notification initialization failed\n");
		goto err_out_free_ctrl_slot;
	}

	/* Check if slot is occupied */
	slot = ctrl->slot;
	pciehp_get_adapter_status(slot, &occupied);
	pciehp_get_power_status(slot, &poweron);
	if (occupied && pciehp_force)
		pciehp_enable_slot(slot);
	/* If empty slot's power status is on, turn power off */
	if (!occupied && poweron && POWER_CTRL(ctrl))
		pciehp_power_off_slot(slot);

	return 0;

=======
		ctrl_err(ctrl, "Notification initialization failed (%d)\n", rc);
		goto err_out_free_ctrl_slot;
	}

	/* Publish to user space */
	rc = pci_hp_add(&ctrl->hotplug_slot);
	if (rc) {
		ctrl_err(ctrl, "Publication to user space failed (%d)\n", rc);
		goto err_out_shutdown_notification;
	}

	pciehp_check_presence(ctrl);

	return 0;

err_out_shutdown_notification:
	pcie_shutdown_notification(ctrl);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
err_out_free_ctrl_slot:
	cleanup_slot(ctrl);
err_out_release_ctlr:
	pciehp_release_ctrl(ctrl);
<<<<<<< HEAD
err_out_none:
=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	return -ENODEV;
}

static void pciehp_remove(struct pcie_device *dev)
{
	struct controller *ctrl = get_service_data(dev);

<<<<<<< HEAD
=======
	pci_hp_del(&ctrl->hotplug_slot);
	pcie_shutdown_notification(ctrl);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	cleanup_slot(ctrl);
	pciehp_release_ctrl(ctrl);
}

#ifdef CONFIG_PM
<<<<<<< HEAD
static int pciehp_suspend (struct pcie_device *dev)
{
	dev_info(&dev->device, "%s ENTRY\n", __func__);
	return 0;
}

static int pciehp_resume (struct pcie_device *dev)
{
	dev_info(&dev->device, "%s ENTRY\n", __func__);
	if (pciehp_force) {
		struct controller *ctrl = get_service_data(dev);
		struct slot *slot;
		u8 status;

		/* reinitialize the chipset's event detection logic */
		pcie_enable_notification(ctrl);

		slot = ctrl->slot;

		/* Check if slot is occupied */
		pciehp_get_adapter_status(slot, &status);
		if (status)
			pciehp_enable_slot(slot);
		else
			pciehp_disable_slot(slot);
	}
	return 0;
}
#endif /* PM */

static struct pcie_port_service_driver hpdriver_portdrv = {
	.name		= PCIE_MODULE_NAME,
=======
static bool pme_is_native(struct pcie_device *dev)
{
	const struct pci_host_bridge *host;

	host = pci_find_host_bridge(dev->port->bus);
	return pcie_ports_native || host->native_pme;
}

static void pciehp_disable_interrupt(struct pcie_device *dev)
{
	/*
	 * Disable hotplug interrupt so that it does not trigger
	 * immediately when the downstream link goes down.
	 */
	if (pme_is_native(dev))
		pcie_disable_interrupt(get_service_data(dev));
}

#ifdef CONFIG_PM_SLEEP
static int pciehp_suspend(struct pcie_device *dev)
{
	/*
	 * If the port is already runtime suspended we can keep it that
	 * way.
	 */
	if (dev_pm_skip_suspend(&dev->port->dev))
		return 0;

	pciehp_disable_interrupt(dev);
	return 0;
}

static int pciehp_resume_noirq(struct pcie_device *dev)
{
	struct controller *ctrl = get_service_data(dev);

	/* pci_restore_state() just wrote to the Slot Control register */
	ctrl->cmd_started = jiffies;
	ctrl->cmd_busy = true;

	/* clear spurious events from rediscovery of inserted card */
	if (ctrl->state == ON_STATE || ctrl->state == BLINKINGOFF_STATE)
		pcie_clear_hotplug_events(ctrl);

	return 0;
}
#endif

static int pciehp_resume(struct pcie_device *dev)
{
	struct controller *ctrl = get_service_data(dev);

	if (pme_is_native(dev))
		pcie_enable_interrupt(ctrl);

	pciehp_check_presence(ctrl);

	return 0;
}

static int pciehp_runtime_suspend(struct pcie_device *dev)
{
	pciehp_disable_interrupt(dev);
	return 0;
}

static int pciehp_runtime_resume(struct pcie_device *dev)
{
	struct controller *ctrl = get_service_data(dev);

	/* pci_restore_state() just wrote to the Slot Control register */
	ctrl->cmd_started = jiffies;
	ctrl->cmd_busy = true;

	/* clear spurious events from rediscovery of inserted card */
	if ((ctrl->state == ON_STATE || ctrl->state == BLINKINGOFF_STATE) &&
	     pme_is_native(dev))
		pcie_clear_hotplug_events(ctrl);

	return pciehp_resume(dev);
}
#endif /* PM */

static struct pcie_port_service_driver hpdriver_portdrv = {
	.name		= "pciehp",
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	.port_type	= PCIE_ANY_PORT,
	.service	= PCIE_PORT_SERVICE_HP,

	.probe		= pciehp_probe,
	.remove		= pciehp_remove,

#ifdef	CONFIG_PM
<<<<<<< HEAD
	.suspend	= pciehp_suspend,
	.resume		= pciehp_resume,
#endif	/* PM */
};

static int __init pcied_init(void)
{
	int retval = 0;

	pciehp_firmware_init();
	retval = pcie_port_service_register(&hpdriver_portdrv);
 	dbg("pcie_port_service_register = %d\n", retval);
  	info(DRIVER_DESC " version: " DRIVER_VERSION "\n");
	if (retval)
		dbg("Failure to register service\n");

	return retval;
}

static void __exit pcied_cleanup(void)
{
	dbg("unload_pciehpd()\n");
	pcie_port_service_unregister(&hpdriver_portdrv);
	info(DRIVER_DESC " version: " DRIVER_VERSION " unloaded\n");
}

module_init(pcied_init);
module_exit(pcied_cleanup);
=======
#ifdef	CONFIG_PM_SLEEP
	.suspend	= pciehp_suspend,
	.resume_noirq	= pciehp_resume_noirq,
	.resume		= pciehp_resume,
#endif
	.runtime_suspend = pciehp_runtime_suspend,
	.runtime_resume	= pciehp_runtime_resume,
#endif	/* PM */

	.slot_reset	= pciehp_slot_reset,
};

int __init pcie_hp_init(void)
{
	int retval = 0;

	retval = pcie_port_service_register(&hpdriver_portdrv);
	pr_debug("pcie_port_service_register = %d\n", retval);
	if (retval)
		pr_debug("Failure to register service\n");

	return retval;
}
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
