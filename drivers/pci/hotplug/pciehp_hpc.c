<<<<<<< HEAD
=======
// SPDX-License-Identifier: GPL-2.0+
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
/*
 * PCI Express PCI Hot Plug Driver
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
 * Send feedback to <greg@kroah.com>,<kristen.c.accardi@intel.com>
 *
 */

#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/types.h>
#include <linux/signal.h>
#include <linux/jiffies.h>
#include <linux/timer.h>
#include <linux/pci.h>
#include <linux/interrupt.h>
#include <linux/time.h>
=======
 * Send feedback to <greg@kroah.com>,<kristen.c.accardi@intel.com>
 */

#define dev_fmt(fmt) "pciehp: " fmt

#include <linux/bitfield.h>
#include <linux/dmi.h>
#include <linux/kernel.h>
#include <linux/types.h>
#include <linux/jiffies.h>
#include <linux/kthread.h>
#include <linux/pci.h>
#include <linux/pm_runtime.h>
#include <linux/interrupt.h>
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#include <linux/slab.h>

#include "../pci.h"
#include "pciehp.h"

<<<<<<< HEAD
static inline int pciehp_readw(struct controller *ctrl, int reg, u16 *value)
{
	struct pci_dev *dev = ctrl->pcie->port;
	return pci_read_config_word(dev, pci_pcie_cap(dev) + reg, value);
}

static inline int pciehp_readl(struct controller *ctrl, int reg, u32 *value)
{
	struct pci_dev *dev = ctrl->pcie->port;
	return pci_read_config_dword(dev, pci_pcie_cap(dev) + reg, value);
}

static inline int pciehp_writew(struct controller *ctrl, int reg, u16 value)
{
	struct pci_dev *dev = ctrl->pcie->port;
	return pci_write_config_word(dev, pci_pcie_cap(dev) + reg, value);
}

static inline int pciehp_writel(struct controller *ctrl, int reg, u32 value)
{
	struct pci_dev *dev = ctrl->pcie->port;
	return pci_write_config_dword(dev, pci_pcie_cap(dev) + reg, value);
}

/* Power Control Command */
#define POWER_ON	0
#define POWER_OFF	PCI_EXP_SLTCTL_PCC

static irqreturn_t pcie_isr(int irq, void *dev_id);
static void start_int_poll_timer(struct controller *ctrl, int sec);

/* This is the interrupt polling timeout function. */
static void int_poll_timeout(unsigned long data)
{
	struct controller *ctrl = (struct controller *)data;

	/* Poll for interrupt events.  regs == NULL => polling */
	pcie_isr(0, ctrl);

	init_timer(&ctrl->poll_timer);
	if (!pciehp_poll_time)
		pciehp_poll_time = 2; /* default polling interval is 2 sec */

	start_int_poll_timer(ctrl, pciehp_poll_time);
}

/* This function starts the interrupt polling timer. */
static void start_int_poll_timer(struct controller *ctrl, int sec)
{
	/* Clamp to sane value */
	if ((sec <= 0) || (sec > 60))
        	sec = 2;

	ctrl->poll_timer.function = &int_poll_timeout;
	ctrl->poll_timer.data = (unsigned long)ctrl;
	ctrl->poll_timer.expires = jiffies + sec * HZ;
	add_timer(&ctrl->poll_timer);
}
=======
static const struct dmi_system_id inband_presence_disabled_dmi_table[] = {
	/*
	 * Match all Dell systems, as some Dell systems have inband
	 * presence disabled on NVMe slots (but don't support the bit to
	 * report it). Setting inband presence disabled should have no
	 * negative effect, except on broken hotplug slots that never
	 * assert presence detect--and those will still work, they will
	 * just have a bit of extra delay before being probed.
	 */
	{
		.ident = "Dell System",
		.matches = {
			DMI_MATCH(DMI_OEM_STRING, "Dell System"),
		},
	},
	{}
};

static inline struct pci_dev *ctrl_dev(struct controller *ctrl)
{
	return ctrl->pcie->port;
}

static irqreturn_t pciehp_isr(int irq, void *dev_id);
static irqreturn_t pciehp_ist(int irq, void *dev_id);
static int pciehp_poll(void *data);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

static inline int pciehp_request_irq(struct controller *ctrl)
{
	int retval, irq = ctrl->pcie->irq;

<<<<<<< HEAD
	/* Install interrupt polling timer. Start with 10 sec delay */
	if (pciehp_poll_mode) {
		init_timer(&ctrl->poll_timer);
		start_int_poll_timer(ctrl, 10);
		return 0;
	}

	/* Installs the interrupt handler */
	retval = request_irq(irq, pcie_isr, IRQF_SHARED, MY_NAME, ctrl);
=======
	if (pciehp_poll_mode) {
		ctrl->poll_thread = kthread_run(&pciehp_poll, ctrl,
						"pciehp_poll-%s",
						slot_name(ctrl));
		return PTR_ERR_OR_ZERO(ctrl->poll_thread);
	}

	/* Installs the interrupt handler */
	retval = request_threaded_irq(irq, pciehp_isr, pciehp_ist,
				      IRQF_SHARED, "pciehp", ctrl);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	if (retval)
		ctrl_err(ctrl, "Cannot get irq %d for the hotplug controller\n",
			 irq);
	return retval;
}

static inline void pciehp_free_irq(struct controller *ctrl)
{
	if (pciehp_poll_mode)
<<<<<<< HEAD
		del_timer_sync(&ctrl->poll_timer);
=======
		kthread_stop(ctrl->poll_thread);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	else
		free_irq(ctrl->pcie->irq, ctrl);
}

<<<<<<< HEAD
static int pcie_poll_cmd(struct controller *ctrl)
{
	u16 slot_status;
	int err, timeout = 1000;

	err = pciehp_readw(ctrl, PCI_EXP_SLTSTA, &slot_status);
	if (!err && (slot_status & PCI_EXP_SLTSTA_CC)) {
		pciehp_writew(ctrl, PCI_EXP_SLTSTA, PCI_EXP_SLTSTA_CC);
		return 1;
	}
	while (timeout > 0) {
		msleep(10);
		timeout -= 10;
		err = pciehp_readw(ctrl, PCI_EXP_SLTSTA, &slot_status);
		if (!err && (slot_status & PCI_EXP_SLTSTA_CC)) {
			pciehp_writew(ctrl, PCI_EXP_SLTSTA, PCI_EXP_SLTSTA_CC);
			return 1;
		}
	}
	return 0;	/* timeout */
}

static void pcie_wait_cmd(struct controller *ctrl, int poll)
{
	unsigned int msecs = pciehp_poll_mode ? 2500 : 1000;
	unsigned long timeout = msecs_to_jiffies(msecs);
	int rc;

	if (poll)
		rc = pcie_poll_cmd(ctrl);
	else
		rc = wait_event_timeout(ctrl->queue, !ctrl->cmd_busy, timeout);
	if (!rc)
		ctrl_dbg(ctrl, "Command not completed in 1000 msec\n");
=======
static int pcie_poll_cmd(struct controller *ctrl, int timeout)
{
	struct pci_dev *pdev = ctrl_dev(ctrl);
	u16 slot_status;

	do {
		pcie_capability_read_word(pdev, PCI_EXP_SLTSTA, &slot_status);
		if (PCI_POSSIBLE_ERROR(slot_status)) {
			ctrl_info(ctrl, "%s: no response from device\n",
				  __func__);
			return 0;
		}

		if (slot_status & PCI_EXP_SLTSTA_CC) {
			pcie_capability_write_word(pdev, PCI_EXP_SLTSTA,
						   PCI_EXP_SLTSTA_CC);
			ctrl->cmd_busy = 0;
			smp_mb();
			return 1;
		}
		msleep(10);
		timeout -= 10;
	} while (timeout >= 0);
	return 0;	/* timeout */
}

static void pcie_wait_cmd(struct controller *ctrl)
{
	unsigned int msecs = pciehp_poll_mode ? 2500 : 1000;
	unsigned long duration = msecs_to_jiffies(msecs);
	unsigned long cmd_timeout = ctrl->cmd_started + duration;
	unsigned long now, timeout;
	int rc;

	/*
	 * If the controller does not generate notifications for command
	 * completions, we never need to wait between writes.
	 */
	if (NO_CMD_CMPL(ctrl))
		return;

	if (!ctrl->cmd_busy)
		return;

	/*
	 * Even if the command has already timed out, we want to call
	 * pcie_poll_cmd() so it can clear PCI_EXP_SLTSTA_CC.
	 */
	now = jiffies;
	if (time_before_eq(cmd_timeout, now))
		timeout = 1;
	else
		timeout = cmd_timeout - now;

	if (ctrl->slot_ctrl & PCI_EXP_SLTCTL_HPIE &&
	    ctrl->slot_ctrl & PCI_EXP_SLTCTL_CCIE)
		rc = wait_event_timeout(ctrl->queue, !ctrl->cmd_busy, timeout);
	else
		rc = pcie_poll_cmd(ctrl, jiffies_to_msecs(timeout));

	if (!rc)
		ctrl_info(ctrl, "Timeout on hotplug command %#06x (issued %u msec ago)\n",
			  ctrl->slot_ctrl,
			  jiffies_to_msecs(jiffies - ctrl->cmd_started));
}

#define CC_ERRATUM_MASK		(PCI_EXP_SLTCTL_PCC |	\
				 PCI_EXP_SLTCTL_PIC |	\
				 PCI_EXP_SLTCTL_AIC |	\
				 PCI_EXP_SLTCTL_EIC)

static void pcie_do_write_cmd(struct controller *ctrl, u16 cmd,
			      u16 mask, bool wait)
{
	struct pci_dev *pdev = ctrl_dev(ctrl);
	u16 slot_ctrl_orig, slot_ctrl;

	mutex_lock(&ctrl->ctrl_lock);

	/*
	 * Always wait for any previous command that might still be in progress
	 */
	pcie_wait_cmd(ctrl);

	pcie_capability_read_word(pdev, PCI_EXP_SLTCTL, &slot_ctrl);
	if (PCI_POSSIBLE_ERROR(slot_ctrl)) {
		ctrl_info(ctrl, "%s: no response from device\n", __func__);
		goto out;
	}

	slot_ctrl_orig = slot_ctrl;
	slot_ctrl &= ~mask;
	slot_ctrl |= (cmd & mask);
	ctrl->cmd_busy = 1;
	smp_mb();
	ctrl->slot_ctrl = slot_ctrl;
	pcie_capability_write_word(pdev, PCI_EXP_SLTCTL, slot_ctrl);
	ctrl->cmd_started = jiffies;

	/*
	 * Controllers with the Intel CF118 and similar errata advertise
	 * Command Completed support, but they only set Command Completed
	 * if we change the "Control" bits for power, power indicator,
	 * attention indicator, or interlock.  If we only change the
	 * "Enable" bits, they never set the Command Completed bit.
	 */
	if (pdev->broken_cmd_compl &&
	    (slot_ctrl_orig & CC_ERRATUM_MASK) == (slot_ctrl & CC_ERRATUM_MASK))
		ctrl->cmd_busy = 0;

	/*
	 * Optionally wait for the hardware to be ready for a new command,
	 * indicating completion of the above issued command.
	 */
	if (wait)
		pcie_wait_cmd(ctrl);

out:
	mutex_unlock(&ctrl->ctrl_lock);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

/**
 * pcie_write_cmd - Issue controller command
 * @ctrl: controller to which the command is issued
 * @cmd:  command value written to slot control register
 * @mask: bitmask of slot control register to be modified
 */
<<<<<<< HEAD
static int pcie_write_cmd(struct controller *ctrl, u16 cmd, u16 mask)
{
	int retval = 0;
	u16 slot_status;
	u16 slot_ctrl;

	mutex_lock(&ctrl->ctrl_lock);

	retval = pciehp_readw(ctrl, PCI_EXP_SLTSTA, &slot_status);
	if (retval) {
		ctrl_err(ctrl, "%s: Cannot read SLOTSTATUS register\n",
			 __func__);
		goto out;
	}

	if (slot_status & PCI_EXP_SLTSTA_CC) {
		if (!ctrl->no_cmd_complete) {
			/*
			 * After 1 sec and CMD_COMPLETED still not set, just
			 * proceed forward to issue the next command according
			 * to spec. Just print out the error message.
			 */
			ctrl_dbg(ctrl, "CMD_COMPLETED not clear after 1 sec\n");
		} else if (!NO_CMD_CMPL(ctrl)) {
			/*
			 * This controller semms to notify of command completed
			 * event even though it supports none of power
			 * controller, attention led, power led and EMI.
			 */
			ctrl_dbg(ctrl, "Unexpected CMD_COMPLETED. Need to "
				 "wait for command completed event.\n");
			ctrl->no_cmd_complete = 0;
		} else {
			ctrl_dbg(ctrl, "Unexpected CMD_COMPLETED. Maybe "
				 "the controller is broken.\n");
		}
	}

	retval = pciehp_readw(ctrl, PCI_EXP_SLTCTL, &slot_ctrl);
	if (retval) {
		ctrl_err(ctrl, "%s: Cannot read SLOTCTRL register\n", __func__);
		goto out;
	}

	slot_ctrl &= ~mask;
	slot_ctrl |= (cmd & mask);
	ctrl->cmd_busy = 1;
	smp_mb();
	retval = pciehp_writew(ctrl, PCI_EXP_SLTCTL, slot_ctrl);
	if (retval)
		ctrl_err(ctrl, "Cannot write to SLOTCTRL register\n");

	/*
	 * Wait for command completion.
	 */
	if (!retval && !ctrl->no_cmd_complete) {
		int poll = 0;
		/*
		 * if hotplug interrupt is not enabled or command
		 * completed interrupt is not enabled, we need to poll
		 * command completed event.
		 */
		if (!(slot_ctrl & PCI_EXP_SLTCTL_HPIE) ||
		    !(slot_ctrl & PCI_EXP_SLTCTL_CCIE))
			poll = 1;
                pcie_wait_cmd(ctrl, poll);
	}
 out:
	mutex_unlock(&ctrl->ctrl_lock);
	return retval;
}

static bool check_link_active(struct controller *ctrl)
{
	bool ret = false;
	u16 lnk_status;

	if (pciehp_readw(ctrl, PCI_EXP_LNKSTA, &lnk_status))
		return ret;

	ret = !!(lnk_status & PCI_EXP_LNKSTA_DLLLA);

	if (ret)
		ctrl_dbg(ctrl, "%s: lnk_status = %x\n", __func__, lnk_status);
=======
static void pcie_write_cmd(struct controller *ctrl, u16 cmd, u16 mask)
{
	pcie_do_write_cmd(ctrl, cmd, mask, true);
}

/* Same as above without waiting for the hardware to latch */
static void pcie_write_cmd_nowait(struct controller *ctrl, u16 cmd, u16 mask)
{
	pcie_do_write_cmd(ctrl, cmd, mask, false);
}

/**
 * pciehp_check_link_active() - Is the link active
 * @ctrl: PCIe hotplug controller
 *
 * Check whether the downstream link is currently active. Note it is
 * possible that the card is removed immediately after this so the
 * caller may need to take it into account.
 *
 * If the hotplug controller itself is not available anymore returns
 * %-ENODEV.
 */
int pciehp_check_link_active(struct controller *ctrl)
{
	struct pci_dev *pdev = ctrl_dev(ctrl);
	u16 lnk_status;
	int ret;

	ret = pcie_capability_read_word(pdev, PCI_EXP_LNKSTA, &lnk_status);
	if (ret == PCIBIOS_DEVICE_NOT_FOUND || PCI_POSSIBLE_ERROR(lnk_status))
		return -ENODEV;

	ret = !!(lnk_status & PCI_EXP_LNKSTA_DLLLA);
	ctrl_dbg(ctrl, "%s: lnk_status = %x\n", __func__, lnk_status);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	return ret;
}

<<<<<<< HEAD
static void __pcie_wait_link_active(struct controller *ctrl, bool active)
{
	int timeout = 1000;

	if (check_link_active(ctrl) == active)
		return;
	while (timeout > 0) {
		msleep(10);
		timeout -= 10;
		if (check_link_active(ctrl) == active)
			return;
	}
	ctrl_dbg(ctrl, "Data Link Layer Link Active not %s in 1000 msec\n",
			active ? "set" : "cleared");
}

static void pcie_wait_link_active(struct controller *ctrl)
{
	__pcie_wait_link_active(ctrl, true);
}

static void pcie_wait_link_not_active(struct controller *ctrl)
{
	__pcie_wait_link_active(ctrl, false);
}

=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
static bool pci_bus_check_dev(struct pci_bus *bus, int devfn)
{
	u32 l;
	int count = 0;
	int delay = 1000, step = 20;
	bool found = false;

	do {
		found = pci_bus_read_dev_vendor_id(bus, devfn, &l, 0);
		count++;

		if (found)
			break;

		msleep(step);
		delay -= step;
	} while (delay > 0);

<<<<<<< HEAD
	if (count > 1 && pciehp_debug)
		printk(KERN_DEBUG "pci %04x:%02x:%02x.%d id reading try %d times with interval %d ms to get %08x\n",
=======
	if (count > 1)
		pr_debug("pci %04x:%02x:%02x.%d id reading try %d times with interval %d ms to get %08x\n",
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
			pci_domain_nr(bus), bus->number, PCI_SLOT(devfn),
			PCI_FUNC(devfn), count, step, l);

	return found;
}

<<<<<<< HEAD
int pciehp_check_link_status(struct controller *ctrl)
{
	u16 lnk_status;
	int retval = 0;
	bool found = false;

        /*
         * Data Link Layer Link Active Reporting must be capable for
         * hot-plug capable downstream port. But old controller might
         * not implement it. In this case, we wait for 1000 ms.
         */
        if (ctrl->link_active_reporting)
                pcie_wait_link_active(ctrl);
        else
                msleep(1000);

	/* wait 100ms before read pci conf, and try in 1s */
	msleep(100);
	found = pci_bus_check_dev(ctrl->pcie->port->subordinate,
					PCI_DEVFN(0, 0));

	retval = pciehp_readw(ctrl, PCI_EXP_LNKSTA, &lnk_status);
	if (retval) {
		ctrl_err(ctrl, "Cannot read LNKSTATUS register\n");
		return retval;
	}

	ctrl_dbg(ctrl, "%s: lnk_status = %x\n", __func__, lnk_status);
	if ((lnk_status & PCI_EXP_LNKSTA_LT) ||
	    !(lnk_status & PCI_EXP_LNKSTA_NLW)) {
		ctrl_err(ctrl, "Link Training Error occurs \n");
		retval = -1;
		return retval;
=======
static void pcie_wait_for_presence(struct pci_dev *pdev)
{
	int timeout = 1250;
	u16 slot_status;

	do {
		pcie_capability_read_word(pdev, PCI_EXP_SLTSTA, &slot_status);
		if (slot_status & PCI_EXP_SLTSTA_PDS)
			return;
		msleep(10);
		timeout -= 10;
	} while (timeout > 0);
}

int pciehp_check_link_status(struct controller *ctrl)
{
	struct pci_dev *pdev = ctrl_dev(ctrl);
	bool found;
	u16 lnk_status;

	if (!pcie_wait_for_link(pdev, true)) {
		ctrl_info(ctrl, "Slot(%s): No link\n", slot_name(ctrl));
		return -1;
	}

	if (ctrl->inband_presence_disabled)
		pcie_wait_for_presence(pdev);

	found = pci_bus_check_dev(ctrl->pcie->port->subordinate,
					PCI_DEVFN(0, 0));

	/* ignore link or presence changes up to this point */
	if (found)
		atomic_and(~(PCI_EXP_SLTSTA_DLLSC | PCI_EXP_SLTSTA_PDC),
			   &ctrl->pending_events);

	pcie_capability_read_word(pdev, PCI_EXP_LNKSTA, &lnk_status);
	ctrl_dbg(ctrl, "%s: lnk_status = %x\n", __func__, lnk_status);
	if ((lnk_status & PCI_EXP_LNKSTA_LT) ||
	    !(lnk_status & PCI_EXP_LNKSTA_NLW)) {
		ctrl_info(ctrl, "Slot(%s): Cannot train link: status %#06x\n",
			  slot_name(ctrl), lnk_status);
		return -1;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	}

	pcie_update_link_speed(ctrl->pcie->port->subordinate, lnk_status);

<<<<<<< HEAD
	if (!found && !retval)
		retval = -1;

	return retval;
=======
	if (!found) {
		ctrl_info(ctrl, "Slot(%s): No device found\n",
			  slot_name(ctrl));
		return -1;
	}

	return 0;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

static int __pciehp_link_set(struct controller *ctrl, bool enable)
{
<<<<<<< HEAD
	u16 lnk_ctrl;
	int retval = 0;

	retval = pciehp_readw(ctrl, PCI_EXP_LNKCTL, &lnk_ctrl);
	if (retval) {
		ctrl_err(ctrl, "Cannot read LNKCTRL register\n");
		return retval;
	}

	if (enable)
		lnk_ctrl &= ~PCI_EXP_LNKCTL_LD;
	else
		lnk_ctrl |= PCI_EXP_LNKCTL_LD;

	retval = pciehp_writew(ctrl, PCI_EXP_LNKCTL, lnk_ctrl);
	if (retval) {
		ctrl_err(ctrl, "Cannot write LNKCTRL register\n");
		return retval;
	}
	ctrl_dbg(ctrl, "%s: lnk_ctrl = %x\n", __func__, lnk_ctrl);

	return retval;
=======
	struct pci_dev *pdev = ctrl_dev(ctrl);

	pcie_capability_clear_and_set_word(pdev, PCI_EXP_LNKCTL,
					   PCI_EXP_LNKCTL_LD,
					   enable ? 0 : PCI_EXP_LNKCTL_LD);

	return 0;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

static int pciehp_link_enable(struct controller *ctrl)
{
	return __pciehp_link_set(ctrl, true);
}

<<<<<<< HEAD
static int pciehp_link_disable(struct controller *ctrl)
{
	return __pciehp_link_set(ctrl, false);
}

int pciehp_get_attention_status(struct slot *slot, u8 *status)
{
	struct controller *ctrl = slot->ctrl;
	u16 slot_ctrl;
	u8 atten_led_state;
	int retval = 0;

	retval = pciehp_readw(ctrl, PCI_EXP_SLTCTL, &slot_ctrl);
	if (retval) {
		ctrl_err(ctrl, "%s: Cannot read SLOTCTRL register\n", __func__);
		return retval;
	}

	ctrl_dbg(ctrl, "%s: SLOTCTRL %x, value read %x\n", __func__,
		 pci_pcie_cap(ctrl->pcie->port) + PCI_EXP_SLTCTL, slot_ctrl);

	atten_led_state = (slot_ctrl & PCI_EXP_SLTCTL_AIC) >> 6;

	switch (atten_led_state) {
	case 0:
		*status = 0xFF;	/* Reserved */
		break;
	case 1:
		*status = 1;	/* On */
		break;
	case 2:
		*status = 2;	/* Blink */
		break;
	case 3:
=======
int pciehp_get_raw_indicator_status(struct hotplug_slot *hotplug_slot,
				    u8 *status)
{
	struct controller *ctrl = to_ctrl(hotplug_slot);
	struct pci_dev *pdev = ctrl_dev(ctrl);
	u16 slot_ctrl;

	pci_config_pm_runtime_get(pdev);
	pcie_capability_read_word(pdev, PCI_EXP_SLTCTL, &slot_ctrl);
	pci_config_pm_runtime_put(pdev);
	*status = (slot_ctrl & (PCI_EXP_SLTCTL_AIC | PCI_EXP_SLTCTL_PIC)) >> 6;
	return 0;
}

int pciehp_get_attention_status(struct hotplug_slot *hotplug_slot, u8 *status)
{
	struct controller *ctrl = to_ctrl(hotplug_slot);
	struct pci_dev *pdev = ctrl_dev(ctrl);
	u16 slot_ctrl;

	pci_config_pm_runtime_get(pdev);
	pcie_capability_read_word(pdev, PCI_EXP_SLTCTL, &slot_ctrl);
	pci_config_pm_runtime_put(pdev);
	ctrl_dbg(ctrl, "%s: SLOTCTRL %x, value read %x\n", __func__,
		 pci_pcie_cap(ctrl->pcie->port) + PCI_EXP_SLTCTL, slot_ctrl);

	switch (slot_ctrl & PCI_EXP_SLTCTL_AIC) {
	case PCI_EXP_SLTCTL_ATTN_IND_ON:
		*status = 1;	/* On */
		break;
	case PCI_EXP_SLTCTL_ATTN_IND_BLINK:
		*status = 2;	/* Blink */
		break;
	case PCI_EXP_SLTCTL_ATTN_IND_OFF:
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		*status = 0;	/* Off */
		break;
	default:
		*status = 0xFF;
		break;
	}

	return 0;
}

<<<<<<< HEAD
int pciehp_get_power_status(struct slot *slot, u8 *status)
{
	struct controller *ctrl = slot->ctrl;
	u16 slot_ctrl;
	u8 pwr_state;
	int	retval = 0;

	retval = pciehp_readw(ctrl, PCI_EXP_SLTCTL, &slot_ctrl);
	if (retval) {
		ctrl_err(ctrl, "%s: Cannot read SLOTCTRL register\n", __func__);
		return retval;
	}
	ctrl_dbg(ctrl, "%s: SLOTCTRL %x value read %x\n", __func__,
		 pci_pcie_cap(ctrl->pcie->port) + PCI_EXP_SLTCTL, slot_ctrl);

	pwr_state = (slot_ctrl & PCI_EXP_SLTCTL_PCC) >> 10;

	switch (pwr_state) {
	case 0:
		*status = 1;
		break;
	case 1:
		*status = 0;
=======
void pciehp_get_power_status(struct controller *ctrl, u8 *status)
{
	struct pci_dev *pdev = ctrl_dev(ctrl);
	u16 slot_ctrl;

	pcie_capability_read_word(pdev, PCI_EXP_SLTCTL, &slot_ctrl);
	ctrl_dbg(ctrl, "%s: SLOTCTRL %x value read %x\n", __func__,
		 pci_pcie_cap(ctrl->pcie->port) + PCI_EXP_SLTCTL, slot_ctrl);

	switch (slot_ctrl & PCI_EXP_SLTCTL_PCC) {
	case PCI_EXP_SLTCTL_PWR_ON:
		*status = 1;	/* On */
		break;
	case PCI_EXP_SLTCTL_PWR_OFF:
		*status = 0;	/* Off */
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		break;
	default:
		*status = 0xFF;
		break;
	}
<<<<<<< HEAD

	return retval;
}

int pciehp_get_latch_status(struct slot *slot, u8 *status)
{
	struct controller *ctrl = slot->ctrl;
	u16 slot_status;
	int retval;

	retval = pciehp_readw(ctrl, PCI_EXP_SLTSTA, &slot_status);
	if (retval) {
		ctrl_err(ctrl, "%s: Cannot read SLOTSTATUS register\n",
			 __func__);
		return retval;
	}
	*status = !!(slot_status & PCI_EXP_SLTSTA_MRLSS);
	return 0;
}

int pciehp_get_adapter_status(struct slot *slot, u8 *status)
{
	struct controller *ctrl = slot->ctrl;
	u16 slot_status;
	int retval;

	retval = pciehp_readw(ctrl, PCI_EXP_SLTSTA, &slot_status);
	if (retval) {
		ctrl_err(ctrl, "%s: Cannot read SLOTSTATUS register\n",
			 __func__);
		return retval;
	}
	*status = !!(slot_status & PCI_EXP_SLTSTA_PDS);
	return 0;
}

int pciehp_query_power_fault(struct slot *slot)
{
	struct controller *ctrl = slot->ctrl;
	u16 slot_status;
	int retval;

	retval = pciehp_readw(ctrl, PCI_EXP_SLTSTA, &slot_status);
	if (retval) {
		ctrl_err(ctrl, "Cannot check for power fault\n");
		return retval;
	}
	return !!(slot_status & PCI_EXP_SLTSTA_PFD);
}

int pciehp_set_attention_status(struct slot *slot, u8 value)
{
	struct controller *ctrl = slot->ctrl;
	u16 slot_cmd;
	u16 cmd_mask;

	cmd_mask = PCI_EXP_SLTCTL_AIC;
	switch (value) {
	case 0 :	/* turn off */
		slot_cmd = 0x00C0;
		break;
	case 1:		/* turn on */
		slot_cmd = 0x0040;
		break;
	case 2:		/* turn blink */
		slot_cmd = 0x0080;
		break;
	default:
		return -EINVAL;
	}
	ctrl_dbg(ctrl, "%s: SLOTCTRL %x write cmd %x\n", __func__,
		 pci_pcie_cap(ctrl->pcie->port) + PCI_EXP_SLTCTL, slot_cmd);
	return pcie_write_cmd(ctrl, slot_cmd, cmd_mask);
}

void pciehp_green_led_on(struct slot *slot)
{
	struct controller *ctrl = slot->ctrl;
	u16 slot_cmd;
	u16 cmd_mask;

	slot_cmd = 0x0100;
	cmd_mask = PCI_EXP_SLTCTL_PIC;
	pcie_write_cmd(ctrl, slot_cmd, cmd_mask);
	ctrl_dbg(ctrl, "%s: SLOTCTRL %x write cmd %x\n", __func__,
		 pci_pcie_cap(ctrl->pcie->port) + PCI_EXP_SLTCTL, slot_cmd);
}

void pciehp_green_led_off(struct slot *slot)
{
	struct controller *ctrl = slot->ctrl;
	u16 slot_cmd;
	u16 cmd_mask;

	slot_cmd = 0x0300;
	cmd_mask = PCI_EXP_SLTCTL_PIC;
	pcie_write_cmd(ctrl, slot_cmd, cmd_mask);
	ctrl_dbg(ctrl, "%s: SLOTCTRL %x write cmd %x\n", __func__,
		 pci_pcie_cap(ctrl->pcie->port) + PCI_EXP_SLTCTL, slot_cmd);
}

void pciehp_green_led_blink(struct slot *slot)
{
	struct controller *ctrl = slot->ctrl;
	u16 slot_cmd;
	u16 cmd_mask;

	slot_cmd = 0x0200;
	cmd_mask = PCI_EXP_SLTCTL_PIC;
	pcie_write_cmd(ctrl, slot_cmd, cmd_mask);
	ctrl_dbg(ctrl, "%s: SLOTCTRL %x write cmd %x\n", __func__,
		 pci_pcie_cap(ctrl->pcie->port) + PCI_EXP_SLTCTL, slot_cmd);
}

int pciehp_power_on_slot(struct slot * slot)
{
	struct controller *ctrl = slot->ctrl;
	u16 slot_cmd;
	u16 cmd_mask;
	u16 slot_status;
	int retval = 0;

	/* Clear sticky power-fault bit from previous power failures */
	retval = pciehp_readw(ctrl, PCI_EXP_SLTSTA, &slot_status);
	if (retval) {
		ctrl_err(ctrl, "%s: Cannot read SLOTSTATUS register\n",
			 __func__);
		return retval;
	}
	slot_status &= PCI_EXP_SLTSTA_PFD;
	if (slot_status) {
		retval = pciehp_writew(ctrl, PCI_EXP_SLTSTA, slot_status);
		if (retval) {
			ctrl_err(ctrl,
				 "%s: Cannot write to SLOTSTATUS register\n",
				 __func__);
			return retval;
		}
	}
	ctrl->power_fault_detected = 0;

	slot_cmd = POWER_ON;
	cmd_mask = PCI_EXP_SLTCTL_PCC;
	retval = pcie_write_cmd(ctrl, slot_cmd, cmd_mask);
	if (retval) {
		ctrl_err(ctrl, "Write %x command failed!\n", slot_cmd);
		return retval;
	}
	ctrl_dbg(ctrl, "%s: SLOTCTRL %x write cmd %x\n", __func__,
		 pci_pcie_cap(ctrl->pcie->port) + PCI_EXP_SLTCTL, slot_cmd);
=======
}

void pciehp_get_latch_status(struct controller *ctrl, u8 *status)
{
	struct pci_dev *pdev = ctrl_dev(ctrl);
	u16 slot_status;

	pcie_capability_read_word(pdev, PCI_EXP_SLTSTA, &slot_status);
	*status = !!(slot_status & PCI_EXP_SLTSTA_MRLSS);
}

/**
 * pciehp_card_present() - Is the card present
 * @ctrl: PCIe hotplug controller
 *
 * Function checks whether the card is currently present in the slot and
 * in that case returns true. Note it is possible that the card is
 * removed immediately after the check so the caller may need to take
 * this into account.
 *
 * It the hotplug controller itself is not available anymore returns
 * %-ENODEV.
 */
int pciehp_card_present(struct controller *ctrl)
{
	struct pci_dev *pdev = ctrl_dev(ctrl);
	u16 slot_status;
	int ret;

	ret = pcie_capability_read_word(pdev, PCI_EXP_SLTSTA, &slot_status);
	if (ret == PCIBIOS_DEVICE_NOT_FOUND || PCI_POSSIBLE_ERROR(slot_status))
		return -ENODEV;

	return !!(slot_status & PCI_EXP_SLTSTA_PDS);
}

/**
 * pciehp_card_present_or_link_active() - whether given slot is occupied
 * @ctrl: PCIe hotplug controller
 *
 * Unlike pciehp_card_present(), which determines presence solely from the
 * Presence Detect State bit, this helper also returns true if the Link Active
 * bit is set.  This is a concession to broken hotplug ports which hardwire
 * Presence Detect State to zero, such as Wilocity's [1ae9:0200].
 *
 * Returns: %1 if the slot is occupied and %0 if it is not. If the hotplug
 *	    port is not present anymore returns %-ENODEV.
 */
int pciehp_card_present_or_link_active(struct controller *ctrl)
{
	int ret;

	ret = pciehp_card_present(ctrl);
	if (ret)
		return ret;

	return pciehp_check_link_active(ctrl);
}

int pciehp_query_power_fault(struct controller *ctrl)
{
	struct pci_dev *pdev = ctrl_dev(ctrl);
	u16 slot_status;

	pcie_capability_read_word(pdev, PCI_EXP_SLTSTA, &slot_status);
	return !!(slot_status & PCI_EXP_SLTSTA_PFD);
}

int pciehp_set_raw_indicator_status(struct hotplug_slot *hotplug_slot,
				    u8 status)
{
	struct controller *ctrl = to_ctrl(hotplug_slot);
	struct pci_dev *pdev = ctrl_dev(ctrl);

	pci_config_pm_runtime_get(pdev);
	pcie_write_cmd_nowait(ctrl, FIELD_PREP(PCI_EXP_SLTCTL_AIC, status),
			      PCI_EXP_SLTCTL_AIC | PCI_EXP_SLTCTL_PIC);
	pci_config_pm_runtime_put(pdev);
	return 0;
}

/**
 * pciehp_set_indicators() - set attention indicator, power indicator, or both
 * @ctrl: PCIe hotplug controller
 * @pwr: one of:
 *	PCI_EXP_SLTCTL_PWR_IND_ON
 *	PCI_EXP_SLTCTL_PWR_IND_BLINK
 *	PCI_EXP_SLTCTL_PWR_IND_OFF
 * @attn: one of:
 *	PCI_EXP_SLTCTL_ATTN_IND_ON
 *	PCI_EXP_SLTCTL_ATTN_IND_BLINK
 *	PCI_EXP_SLTCTL_ATTN_IND_OFF
 *
 * Either @pwr or @attn can also be INDICATOR_NOOP to leave that indicator
 * unchanged.
 */
void pciehp_set_indicators(struct controller *ctrl, int pwr, int attn)
{
	u16 cmd = 0, mask = 0;

	if (PWR_LED(ctrl) && pwr != INDICATOR_NOOP) {
		cmd |= (pwr & PCI_EXP_SLTCTL_PIC);
		mask |= PCI_EXP_SLTCTL_PIC;
	}

	if (ATTN_LED(ctrl) && attn != INDICATOR_NOOP) {
		cmd |= (attn & PCI_EXP_SLTCTL_AIC);
		mask |= PCI_EXP_SLTCTL_AIC;
	}

	if (cmd) {
		pcie_write_cmd_nowait(ctrl, cmd, mask);
		ctrl_dbg(ctrl, "%s: SLOTCTRL %x write cmd %x\n", __func__,
			 pci_pcie_cap(ctrl->pcie->port) + PCI_EXP_SLTCTL, cmd);
	}
}

int pciehp_power_on_slot(struct controller *ctrl)
{
	struct pci_dev *pdev = ctrl_dev(ctrl);
	u16 slot_status;
	int retval;

	/* Clear power-fault bit from previous power failures */
	pcie_capability_read_word(pdev, PCI_EXP_SLTSTA, &slot_status);
	if (slot_status & PCI_EXP_SLTSTA_PFD)
		pcie_capability_write_word(pdev, PCI_EXP_SLTSTA,
					   PCI_EXP_SLTSTA_PFD);
	ctrl->power_fault_detected = 0;

	pcie_write_cmd(ctrl, PCI_EXP_SLTCTL_PWR_ON, PCI_EXP_SLTCTL_PCC);
	ctrl_dbg(ctrl, "%s: SLOTCTRL %x write cmd %x\n", __func__,
		 pci_pcie_cap(ctrl->pcie->port) + PCI_EXP_SLTCTL,
		 PCI_EXP_SLTCTL_PWR_ON);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	retval = pciehp_link_enable(ctrl);
	if (retval)
		ctrl_err(ctrl, "%s: Can not enable the link!\n", __func__);

	return retval;
}

<<<<<<< HEAD
int pciehp_power_off_slot(struct slot * slot)
{
	struct controller *ctrl = slot->ctrl;
	u16 slot_cmd;
	u16 cmd_mask;
	int retval;

	/* Disable the link at first */
	pciehp_link_disable(ctrl);
	/* wait the link is down */
	if (ctrl->link_active_reporting)
		pcie_wait_link_not_active(ctrl);
	else
		msleep(1000);

	slot_cmd = POWER_OFF;
	cmd_mask = PCI_EXP_SLTCTL_PCC;
	retval = pcie_write_cmd(ctrl, slot_cmd, cmd_mask);
	if (retval) {
		ctrl_err(ctrl, "Write command failed!\n");
		return retval;
	}
	ctrl_dbg(ctrl, "%s: SLOTCTRL %x write cmd %x\n", __func__,
		 pci_pcie_cap(ctrl->pcie->port) + PCI_EXP_SLTCTL, slot_cmd);
	return 0;
}

static irqreturn_t pcie_isr(int irq, void *dev_id)
{
	struct controller *ctrl = (struct controller *)dev_id;
	struct slot *slot = ctrl->slot;
	u16 detected, intr_loc;

	/*
	 * In order to guarantee that all interrupt events are
	 * serviced, we need to re-inspect Slot Status register after
	 * clearing what is presumed to be the last pending interrupt.
	 */
	intr_loc = 0;
	do {
		if (pciehp_readw(ctrl, PCI_EXP_SLTSTA, &detected)) {
			ctrl_err(ctrl, "%s: Cannot read SLOTSTATUS\n",
				 __func__);
			return IRQ_NONE;
		}

		detected &= (PCI_EXP_SLTSTA_ABP | PCI_EXP_SLTSTA_PFD |
			     PCI_EXP_SLTSTA_MRLSC | PCI_EXP_SLTSTA_PDC |
			     PCI_EXP_SLTSTA_CC);
		detected &= ~intr_loc;
		intr_loc |= detected;
		if (!intr_loc)
			return IRQ_NONE;
		if (detected && pciehp_writew(ctrl, PCI_EXP_SLTSTA, intr_loc)) {
			ctrl_err(ctrl, "%s: Cannot write to SLOTSTATUS\n",
				 __func__);
			return IRQ_NONE;
		}
	} while (detected);

	ctrl_dbg(ctrl, "%s: intr_loc %x\n", __func__, intr_loc);

	/* Check Command Complete Interrupt Pending */
	if (intr_loc & PCI_EXP_SLTSTA_CC) {
		ctrl->cmd_busy = 0;
		smp_mb();
		wake_up(&ctrl->queue);
	}

	if (!(intr_loc & ~PCI_EXP_SLTSTA_CC))
		return IRQ_HANDLED;

	/* Check MRL Sensor Changed */
	if (intr_loc & PCI_EXP_SLTSTA_MRLSC)
		pciehp_handle_switch_change(slot);

	/* Check Attention Button Pressed */
	if (intr_loc & PCI_EXP_SLTSTA_ABP)
		pciehp_handle_attention_button(slot);

	/* Check Presence Detect Changed */
	if (intr_loc & PCI_EXP_SLTSTA_PDC)
		pciehp_handle_presence_change(slot);

	/* Check Power Fault Detected */
	if ((intr_loc & PCI_EXP_SLTSTA_PFD) && !ctrl->power_fault_detected) {
		ctrl->power_fault_detected = 1;
		pciehp_handle_power_fault(slot);
	}
	return IRQ_HANDLED;
}

int pciehp_get_max_lnk_width(struct slot *slot,
				 enum pcie_link_width *value)
{
	struct controller *ctrl = slot->ctrl;
	enum pcie_link_width lnk_wdth;
	u32	lnk_cap;
	int retval = 0;

	retval = pciehp_readl(ctrl, PCI_EXP_LNKCAP, &lnk_cap);
	if (retval) {
		ctrl_err(ctrl, "%s: Cannot read LNKCAP register\n", __func__);
		return retval;
	}

	switch ((lnk_cap & PCI_EXP_LNKSTA_NLW) >> 4){
	case 0:
		lnk_wdth = PCIE_LNK_WIDTH_RESRV;
		break;
	case 1:
		lnk_wdth = PCIE_LNK_X1;
		break;
	case 2:
		lnk_wdth = PCIE_LNK_X2;
		break;
	case 4:
		lnk_wdth = PCIE_LNK_X4;
		break;
	case 8:
		lnk_wdth = PCIE_LNK_X8;
		break;
	case 12:
		lnk_wdth = PCIE_LNK_X12;
		break;
	case 16:
		lnk_wdth = PCIE_LNK_X16;
		break;
	case 32:
		lnk_wdth = PCIE_LNK_X32;
		break;
	default:
		lnk_wdth = PCIE_LNK_WIDTH_UNKNOWN;
		break;
	}

	*value = lnk_wdth;
	ctrl_dbg(ctrl, "Max link width = %d\n", lnk_wdth);

	return retval;
}

int pciehp_get_cur_lnk_width(struct slot *slot,
				 enum pcie_link_width *value)
{
	struct controller *ctrl = slot->ctrl;
	enum pcie_link_width lnk_wdth = PCIE_LNK_WIDTH_UNKNOWN;
	int retval = 0;
	u16 lnk_status;

	retval = pciehp_readw(ctrl, PCI_EXP_LNKSTA, &lnk_status);
	if (retval) {
		ctrl_err(ctrl, "%s: Cannot read LNKSTATUS register\n",
			 __func__);
		return retval;
	}

	switch ((lnk_status & PCI_EXP_LNKSTA_NLW) >> 4){
	case 0:
		lnk_wdth = PCIE_LNK_WIDTH_RESRV;
		break;
	case 1:
		lnk_wdth = PCIE_LNK_X1;
		break;
	case 2:
		lnk_wdth = PCIE_LNK_X2;
		break;
	case 4:
		lnk_wdth = PCIE_LNK_X4;
		break;
	case 8:
		lnk_wdth = PCIE_LNK_X8;
		break;
	case 12:
		lnk_wdth = PCIE_LNK_X12;
		break;
	case 16:
		lnk_wdth = PCIE_LNK_X16;
		break;
	case 32:
		lnk_wdth = PCIE_LNK_X32;
		break;
	default:
		lnk_wdth = PCIE_LNK_WIDTH_UNKNOWN;
		break;
	}

	*value = lnk_wdth;
	ctrl_dbg(ctrl, "Current link width = %d\n", lnk_wdth);

	return retval;
}

int pcie_enable_notification(struct controller *ctrl)
=======
void pciehp_power_off_slot(struct controller *ctrl)
{
	pcie_write_cmd(ctrl, PCI_EXP_SLTCTL_PWR_OFF, PCI_EXP_SLTCTL_PCC);
	ctrl_dbg(ctrl, "%s: SLOTCTRL %x write cmd %x\n", __func__,
		 pci_pcie_cap(ctrl->pcie->port) + PCI_EXP_SLTCTL,
		 PCI_EXP_SLTCTL_PWR_OFF);
}

static void pciehp_ignore_dpc_link_change(struct controller *ctrl,
					  struct pci_dev *pdev, int irq)
{
	/*
	 * Ignore link changes which occurred while waiting for DPC recovery.
	 * Could be several if DPC triggered multiple times consecutively.
	 */
	synchronize_hardirq(irq);
	atomic_and(~PCI_EXP_SLTSTA_DLLSC, &ctrl->pending_events);
	if (pciehp_poll_mode)
		pcie_capability_write_word(pdev, PCI_EXP_SLTSTA,
					   PCI_EXP_SLTSTA_DLLSC);
	ctrl_info(ctrl, "Slot(%s): Link Down/Up ignored (recovered by DPC)\n",
		  slot_name(ctrl));

	/*
	 * If the link is unexpectedly down after successful recovery,
	 * the corresponding link change may have been ignored above.
	 * Synthesize it to ensure that it is acted on.
	 */
	down_read_nested(&ctrl->reset_lock, ctrl->depth);
	if (!pciehp_check_link_active(ctrl))
		pciehp_request(ctrl, PCI_EXP_SLTSTA_DLLSC);
	up_read(&ctrl->reset_lock);
}

static irqreturn_t pciehp_isr(int irq, void *dev_id)
{
	struct controller *ctrl = (struct controller *)dev_id;
	struct pci_dev *pdev = ctrl_dev(ctrl);
	struct device *parent = pdev->dev.parent;
	u16 status, events = 0;

	/*
	 * Interrupts only occur in D3hot or shallower and only if enabled
	 * in the Slot Control register (PCIe r4.0, sec 6.7.3.4).
	 */
	if (pdev->current_state == PCI_D3cold ||
	    (!(ctrl->slot_ctrl & PCI_EXP_SLTCTL_HPIE) && !pciehp_poll_mode))
		return IRQ_NONE;

	/*
	 * Keep the port accessible by holding a runtime PM ref on its parent.
	 * Defer resume of the parent to the IRQ thread if it's suspended.
	 * Mask the interrupt until then.
	 */
	if (parent) {
		pm_runtime_get_noresume(parent);
		if (!pm_runtime_active(parent)) {
			pm_runtime_put(parent);
			disable_irq_nosync(irq);
			atomic_or(RERUN_ISR, &ctrl->pending_events);
			return IRQ_WAKE_THREAD;
		}
	}

read_status:
	pcie_capability_read_word(pdev, PCI_EXP_SLTSTA, &status);
	if (PCI_POSSIBLE_ERROR(status)) {
		ctrl_info(ctrl, "%s: no response from device\n", __func__);
		if (parent)
			pm_runtime_put(parent);
		return IRQ_NONE;
	}

	/*
	 * Slot Status contains plain status bits as well as event
	 * notification bits; right now we only want the event bits.
	 */
	status &= PCI_EXP_SLTSTA_ABP | PCI_EXP_SLTSTA_PFD |
		  PCI_EXP_SLTSTA_PDC | PCI_EXP_SLTSTA_CC |
		  PCI_EXP_SLTSTA_DLLSC;

	/*
	 * If we've already reported a power fault, don't report it again
	 * until we've done something to handle it.
	 */
	if (ctrl->power_fault_detected)
		status &= ~PCI_EXP_SLTSTA_PFD;
	else if (status & PCI_EXP_SLTSTA_PFD)
		ctrl->power_fault_detected = true;

	events |= status;
	if (!events) {
		if (parent)
			pm_runtime_put(parent);
		return IRQ_NONE;
	}

	if (status) {
		pcie_capability_write_word(pdev, PCI_EXP_SLTSTA, status);

		/*
		 * In MSI mode, all event bits must be zero before the port
		 * will send a new interrupt (PCIe Base Spec r5.0 sec 6.7.3.4).
		 * So re-read the Slot Status register in case a bit was set
		 * between read and write.
		 */
		if (pci_dev_msi_enabled(pdev) && !pciehp_poll_mode)
			goto read_status;
	}

	ctrl_dbg(ctrl, "pending interrupts %#06x from Slot Status\n", events);
	if (parent)
		pm_runtime_put(parent);

	/*
	 * Command Completed notifications are not deferred to the
	 * IRQ thread because it may be waiting for their arrival.
	 */
	if (events & PCI_EXP_SLTSTA_CC) {
		ctrl->cmd_busy = 0;
		smp_mb();
		wake_up(&ctrl->queue);

		if (events == PCI_EXP_SLTSTA_CC)
			return IRQ_HANDLED;

		events &= ~PCI_EXP_SLTSTA_CC;
	}

	if (pdev->ignore_hotplug) {
		ctrl_dbg(ctrl, "ignoring hotplug event %#06x\n", events);
		return IRQ_HANDLED;
	}

	/* Save pending events for consumption by IRQ thread. */
	atomic_or(events, &ctrl->pending_events);
	return IRQ_WAKE_THREAD;
}

static irqreturn_t pciehp_ist(int irq, void *dev_id)
{
	struct controller *ctrl = (struct controller *)dev_id;
	struct pci_dev *pdev = ctrl_dev(ctrl);
	irqreturn_t ret;
	u32 events;

	ctrl->ist_running = true;
	pci_config_pm_runtime_get(pdev);

	/* rerun pciehp_isr() if the port was inaccessible on interrupt */
	if (atomic_fetch_and(~RERUN_ISR, &ctrl->pending_events) & RERUN_ISR) {
		ret = pciehp_isr(irq, dev_id);
		enable_irq(irq);
		if (ret != IRQ_WAKE_THREAD)
			goto out;
	}

	synchronize_hardirq(irq);
	events = atomic_xchg(&ctrl->pending_events, 0);
	if (!events) {
		ret = IRQ_NONE;
		goto out;
	}

	/* Check Attention Button Pressed */
	if (events & PCI_EXP_SLTSTA_ABP)
		pciehp_handle_button_press(ctrl);

	/* Check Power Fault Detected */
	if (events & PCI_EXP_SLTSTA_PFD) {
		ctrl_err(ctrl, "Slot(%s): Power fault\n", slot_name(ctrl));
		pciehp_set_indicators(ctrl, PCI_EXP_SLTCTL_PWR_IND_OFF,
				      PCI_EXP_SLTCTL_ATTN_IND_ON);
	}

	/*
	 * Ignore Link Down/Up events caused by Downstream Port Containment
	 * if recovery from the error succeeded.
	 */
	if ((events & PCI_EXP_SLTSTA_DLLSC) && pci_dpc_recovered(pdev) &&
	    ctrl->state == ON_STATE) {
		events &= ~PCI_EXP_SLTSTA_DLLSC;
		pciehp_ignore_dpc_link_change(ctrl, pdev, irq);
	}

	/*
	 * Disable requests have higher priority than Presence Detect Changed
	 * or Data Link Layer State Changed events.
	 */
	down_read_nested(&ctrl->reset_lock, ctrl->depth);
	if (events & DISABLE_SLOT)
		pciehp_handle_disable_request(ctrl);
	else if (events & (PCI_EXP_SLTSTA_PDC | PCI_EXP_SLTSTA_DLLSC))
		pciehp_handle_presence_or_link_change(ctrl, events);
	up_read(&ctrl->reset_lock);

	ret = IRQ_HANDLED;
out:
	pci_config_pm_runtime_put(pdev);
	ctrl->ist_running = false;
	wake_up(&ctrl->requester);
	return ret;
}

static int pciehp_poll(void *data)
{
	struct controller *ctrl = data;

	schedule_timeout_idle(10 * HZ); /* start with 10 sec delay */

	while (!kthread_should_stop()) {
		/* poll for interrupt events or user requests */
		while (pciehp_isr(IRQ_NOTCONNECTED, ctrl) == IRQ_WAKE_THREAD ||
		       atomic_read(&ctrl->pending_events))
			pciehp_ist(IRQ_NOTCONNECTED, ctrl);

		if (pciehp_poll_time <= 0 || pciehp_poll_time > 60)
			pciehp_poll_time = 2; /* clamp to sane value */

		schedule_timeout_idle(pciehp_poll_time * HZ);
	}

	return 0;
}

static void pcie_enable_notification(struct controller *ctrl)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
{
	u16 cmd, mask;

	/*
	 * TBD: Power fault detected software notification support.
	 *
	 * Power fault detected software notification is not enabled
	 * now, because it caused power fault detected interrupt storm
	 * on some machines. On those machines, power fault detected
	 * bit in the slot status register was set again immediately
	 * when it is cleared in the interrupt service routine, and
	 * next power fault detected interrupt was notified again.
	 */
<<<<<<< HEAD
	cmd = PCI_EXP_SLTCTL_PDCE;
	if (ATTN_BUTTN(ctrl))
		cmd |= PCI_EXP_SLTCTL_ABPE;
	if (MRL_SENS(ctrl))
		cmd |= PCI_EXP_SLTCTL_MRLSCE;
	if (!pciehp_poll_mode)
		cmd |= PCI_EXP_SLTCTL_HPIE | PCI_EXP_SLTCTL_CCIE;

	mask = (PCI_EXP_SLTCTL_PDCE | PCI_EXP_SLTCTL_ABPE |
		PCI_EXP_SLTCTL_MRLSCE | PCI_EXP_SLTCTL_PFDE |
		PCI_EXP_SLTCTL_HPIE | PCI_EXP_SLTCTL_CCIE);

	if (pcie_write_cmd(ctrl, cmd, mask)) {
		ctrl_err(ctrl, "Cannot enable software notification\n");
		return -1;
	}
	return 0;
=======

	/*
	 * Always enable link events: thus link-up and link-down shall
	 * always be treated as hotplug and unplug respectively. Enable
	 * presence detect only if Attention Button is not present.
	 */
	cmd = PCI_EXP_SLTCTL_DLLSCE;
	if (ATTN_BUTTN(ctrl))
		cmd |= PCI_EXP_SLTCTL_ABPE;
	else
		cmd |= PCI_EXP_SLTCTL_PDCE;
	if (!pciehp_poll_mode)
		cmd |= PCI_EXP_SLTCTL_HPIE;
	if (!pciehp_poll_mode && !NO_CMD_CMPL(ctrl))
		cmd |= PCI_EXP_SLTCTL_CCIE;

	mask = (PCI_EXP_SLTCTL_PDCE | PCI_EXP_SLTCTL_ABPE |
		PCI_EXP_SLTCTL_PFDE |
		PCI_EXP_SLTCTL_HPIE | PCI_EXP_SLTCTL_CCIE |
		PCI_EXP_SLTCTL_DLLSCE);

	pcie_write_cmd_nowait(ctrl, cmd, mask);
	ctrl_dbg(ctrl, "%s: SLOTCTRL %x write cmd %x\n", __func__,
		 pci_pcie_cap(ctrl->pcie->port) + PCI_EXP_SLTCTL, cmd);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

static void pcie_disable_notification(struct controller *ctrl)
{
	u16 mask;
<<<<<<< HEAD
=======

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	mask = (PCI_EXP_SLTCTL_PDCE | PCI_EXP_SLTCTL_ABPE |
		PCI_EXP_SLTCTL_MRLSCE | PCI_EXP_SLTCTL_PFDE |
		PCI_EXP_SLTCTL_HPIE | PCI_EXP_SLTCTL_CCIE |
		PCI_EXP_SLTCTL_DLLSCE);
<<<<<<< HEAD
	if (pcie_write_cmd(ctrl, 0, mask))
		ctrl_warn(ctrl, "Cannot disable software notification\n");
=======
	pcie_write_cmd(ctrl, 0, mask);
	ctrl_dbg(ctrl, "%s: SLOTCTRL %x write cmd %x\n", __func__,
		 pci_pcie_cap(ctrl->pcie->port) + PCI_EXP_SLTCTL, 0);
}

void pcie_clear_hotplug_events(struct controller *ctrl)
{
	pcie_capability_write_word(ctrl_dev(ctrl), PCI_EXP_SLTSTA,
				   PCI_EXP_SLTSTA_PDC | PCI_EXP_SLTSTA_DLLSC);
}

void pcie_enable_interrupt(struct controller *ctrl)
{
	u16 mask;

	mask = PCI_EXP_SLTCTL_HPIE | PCI_EXP_SLTCTL_DLLSCE;
	pcie_write_cmd(ctrl, mask, mask);
}

void pcie_disable_interrupt(struct controller *ctrl)
{
	u16 mask;

	/*
	 * Mask hot-plug interrupt to prevent it triggering immediately
	 * when the link goes inactive (we still get PME when any of the
	 * enabled events is detected). Same goes with Link Layer State
	 * changed event which generates PME immediately when the link goes
	 * inactive so mask it as well.
	 */
	mask = PCI_EXP_SLTCTL_HPIE | PCI_EXP_SLTCTL_DLLSCE;
	pcie_write_cmd(ctrl, 0, mask);
}

/**
 * pciehp_slot_reset() - ignore link event caused by error-induced hot reset
 * @dev: PCI Express port service device
 *
 * Called from pcie_portdrv_slot_reset() after AER or DPC initiated a reset
 * further up in the hierarchy to recover from an error.  The reset was
 * propagated down to this hotplug port.  Ignore the resulting link flap.
 * If the link failed to retrain successfully, synthesize the ignored event.
 * Surprise removal during reset is detected through Presence Detect Changed.
 */
int pciehp_slot_reset(struct pcie_device *dev)
{
	struct controller *ctrl = get_service_data(dev);

	if (ctrl->state != ON_STATE)
		return 0;

	pcie_capability_write_word(dev->port, PCI_EXP_SLTSTA,
				   PCI_EXP_SLTSTA_DLLSC);

	if (!pciehp_check_link_active(ctrl))
		pciehp_request(ctrl, PCI_EXP_SLTSTA_DLLSC);

	return 0;
}

/*
 * pciehp has a 1:1 bus:slot relationship so we ultimately want a secondary
 * bus reset of the bridge, but at the same time we want to ensure that it is
 * not seen as a hot-unplug, followed by the hot-plug of the device. Thus,
 * disable link state notification and presence detection change notification
 * momentarily, if we see that they could interfere. Also, clear any spurious
 * events after.
 */
int pciehp_reset_slot(struct hotplug_slot *hotplug_slot, bool probe)
{
	struct controller *ctrl = to_ctrl(hotplug_slot);
	struct pci_dev *pdev = ctrl_dev(ctrl);
	u16 stat_mask = 0, ctrl_mask = 0;
	int rc;

	if (probe)
		return 0;

	down_write_nested(&ctrl->reset_lock, ctrl->depth);

	if (!ATTN_BUTTN(ctrl)) {
		ctrl_mask |= PCI_EXP_SLTCTL_PDCE;
		stat_mask |= PCI_EXP_SLTSTA_PDC;
	}
	ctrl_mask |= PCI_EXP_SLTCTL_DLLSCE;
	stat_mask |= PCI_EXP_SLTSTA_DLLSC;

	pcie_write_cmd(ctrl, 0, ctrl_mask);
	ctrl_dbg(ctrl, "%s: SLOTCTRL %x write cmd %x\n", __func__,
		 pci_pcie_cap(ctrl->pcie->port) + PCI_EXP_SLTCTL, 0);

	rc = pci_bridge_secondary_bus_reset(ctrl->pcie->port);

	pcie_capability_write_word(pdev, PCI_EXP_SLTSTA, stat_mask);
	pcie_write_cmd_nowait(ctrl, ctrl_mask, ctrl_mask);
	ctrl_dbg(ctrl, "%s: SLOTCTRL %x write cmd %x\n", __func__,
		 pci_pcie_cap(ctrl->pcie->port) + PCI_EXP_SLTCTL, ctrl_mask);

	up_write(&ctrl->reset_lock);
	return rc;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

int pcie_init_notification(struct controller *ctrl)
{
	if (pciehp_request_irq(ctrl))
		return -1;
<<<<<<< HEAD
	if (pcie_enable_notification(ctrl)) {
		pciehp_free_irq(ctrl);
		return -1;
	}
=======
	pcie_enable_notification(ctrl);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	ctrl->notification_enabled = 1;
	return 0;
}

<<<<<<< HEAD
static void pcie_shutdown_notification(struct controller *ctrl)
=======
void pcie_shutdown_notification(struct controller *ctrl)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
{
	if (ctrl->notification_enabled) {
		pcie_disable_notification(ctrl);
		pciehp_free_irq(ctrl);
		ctrl->notification_enabled = 0;
	}
}

<<<<<<< HEAD
static int pcie_init_slot(struct controller *ctrl)
{
	struct slot *slot;
	char name[32];

	slot = kzalloc(sizeof(*slot), GFP_KERNEL);
	if (!slot)
		return -ENOMEM;

	snprintf(name, sizeof(name), "pciehp-%u", PSN(ctrl));
	slot->wq = alloc_workqueue(name, 0, 0);
	if (!slot->wq)
		goto abort;

	slot->ctrl = ctrl;
	mutex_init(&slot->lock);
	INIT_DELAYED_WORK(&slot->work, pciehp_queue_pushbutton_work);
	ctrl->slot = slot;
	return 0;
abort:
	kfree(slot);
	return -ENOMEM;
}

static void pcie_cleanup_slot(struct controller *ctrl)
{
	struct slot *slot = ctrl->slot;
	cancel_delayed_work(&slot->work);
	destroy_workqueue(slot->wq);
	kfree(slot);
}

static inline void dbg_ctrl(struct controller *ctrl)
{
	int i;
	u16 reg16;
	struct pci_dev *pdev = ctrl->pcie->port;

	if (!pciehp_debug)
		return;

	ctrl_info(ctrl, "Hotplug Controller:\n");
	ctrl_info(ctrl, "  Seg/Bus/Dev/Func/IRQ : %s IRQ %d\n",
		  pci_name(pdev), pdev->irq);
	ctrl_info(ctrl, "  Vendor ID            : 0x%04x\n", pdev->vendor);
	ctrl_info(ctrl, "  Device ID            : 0x%04x\n", pdev->device);
	ctrl_info(ctrl, "  Subsystem ID         : 0x%04x\n",
		  pdev->subsystem_device);
	ctrl_info(ctrl, "  Subsystem Vendor ID  : 0x%04x\n",
		  pdev->subsystem_vendor);
	ctrl_info(ctrl, "  PCIe Cap offset      : 0x%02x\n",
		  pci_pcie_cap(pdev));
	for (i = 0; i < DEVICE_COUNT_RESOURCE; i++) {
		if (!pci_resource_len(pdev, i))
			continue;
		ctrl_info(ctrl, "  PCI resource [%d]     : %pR\n",
			  i, &pdev->resource[i]);
	}
	ctrl_info(ctrl, "Slot Capabilities      : 0x%08x\n", ctrl->slot_cap);
	ctrl_info(ctrl, "  Physical Slot Number : %d\n", PSN(ctrl));
	ctrl_info(ctrl, "  Attention Button     : %3s\n",
		  ATTN_BUTTN(ctrl) ? "yes" : "no");
	ctrl_info(ctrl, "  Power Controller     : %3s\n",
		  POWER_CTRL(ctrl) ? "yes" : "no");
	ctrl_info(ctrl, "  MRL Sensor           : %3s\n",
		  MRL_SENS(ctrl)   ? "yes" : "no");
	ctrl_info(ctrl, "  Attention Indicator  : %3s\n",
		  ATTN_LED(ctrl)   ? "yes" : "no");
	ctrl_info(ctrl, "  Power Indicator      : %3s\n",
		  PWR_LED(ctrl)    ? "yes" : "no");
	ctrl_info(ctrl, "  Hot-Plug Surprise    : %3s\n",
		  HP_SUPR_RM(ctrl) ? "yes" : "no");
	ctrl_info(ctrl, "  EMI Present          : %3s\n",
		  EMI(ctrl)        ? "yes" : "no");
	ctrl_info(ctrl, "  Command Completed    : %3s\n",
		  NO_CMD_CMPL(ctrl) ? "no" : "yes");
	pciehp_readw(ctrl, PCI_EXP_SLTSTA, &reg16);
	ctrl_info(ctrl, "Slot Status            : 0x%04x\n", reg16);
	pciehp_readw(ctrl, PCI_EXP_SLTCTL, &reg16);
	ctrl_info(ctrl, "Slot Control           : 0x%04x\n", reg16);
=======
static inline void dbg_ctrl(struct controller *ctrl)
{
	struct pci_dev *pdev = ctrl->pcie->port;
	u16 reg16;

	ctrl_dbg(ctrl, "Slot Capabilities      : 0x%08x\n", ctrl->slot_cap);
	pcie_capability_read_word(pdev, PCI_EXP_SLTSTA, &reg16);
	ctrl_dbg(ctrl, "Slot Status            : 0x%04x\n", reg16);
	pcie_capability_read_word(pdev, PCI_EXP_SLTCTL, &reg16);
	ctrl_dbg(ctrl, "Slot Control           : 0x%04x\n", reg16);
}

#define FLAG(x, y)	(((x) & (y)) ? '+' : '-')

static inline int pcie_hotplug_depth(struct pci_dev *dev)
{
	struct pci_bus *bus = dev->bus;
	int depth = 0;

	while (bus->parent) {
		bus = bus->parent;
		if (bus->self && bus->self->is_hotplug_bridge)
			depth++;
	}

	return depth;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

struct controller *pcie_init(struct pcie_device *dev)
{
	struct controller *ctrl;
<<<<<<< HEAD
	u32 slot_cap, link_cap;
	struct pci_dev *pdev = dev->port;

	ctrl = kzalloc(sizeof(*ctrl), GFP_KERNEL);
	if (!ctrl) {
		dev_err(&dev->device, "%s: Out of memory\n", __func__);
		goto abort;
	}
	ctrl->pcie = dev;
	if (!pci_pcie_cap(pdev)) {
		ctrl_err(ctrl, "Cannot find PCI Express capability\n");
		goto abort_ctrl;
	}
	if (pciehp_readl(ctrl, PCI_EXP_SLTCAP, &slot_cap)) {
		ctrl_err(ctrl, "Cannot read SLOTCAP register\n");
		goto abort_ctrl;
	}

	ctrl->slot_cap = slot_cap;
	mutex_init(&ctrl->ctrl_lock);
	init_waitqueue_head(&ctrl->queue);
	dbg_ctrl(ctrl);
	/*
	 * Controller doesn't notify of command completion if the "No
	 * Command Completed Support" bit is set in Slot Capability
	 * register or the controller supports none of power
	 * controller, attention led, power led and EMI.
	 */
	if (NO_CMD_CMPL(ctrl) ||
	    !(POWER_CTRL(ctrl) | ATTN_LED(ctrl) | PWR_LED(ctrl) | EMI(ctrl)))
	    ctrl->no_cmd_complete = 1;

        /* Check if Data Link Layer Link Active Reporting is implemented */
        if (pciehp_readl(ctrl, PCI_EXP_LNKCAP, &link_cap)) {
                ctrl_err(ctrl, "%s: Cannot read LNKCAP register\n", __func__);
                goto abort_ctrl;
        }
        if (link_cap & PCI_EXP_LNKCAP_DLLLARC) {
                ctrl_dbg(ctrl, "Link Active Reporting supported\n");
                ctrl->link_active_reporting = 1;
        }

	/* Clear all remaining event bits in Slot Status register */
	if (pciehp_writew(ctrl, PCI_EXP_SLTSTA, 0x1f))
		goto abort_ctrl;

	/* Disable sotfware notification */
	pcie_disable_notification(ctrl);

	ctrl_info(ctrl, "HPC vendor_id %x device_id %x ss_vid %x ss_did %x\n",
		  pdev->vendor, pdev->device, pdev->subsystem_vendor,
		  pdev->subsystem_device);

	if (pcie_init_slot(ctrl))
		goto abort_ctrl;

	return ctrl;

abort_ctrl:
	kfree(ctrl);
abort:
	return NULL;
=======
	u32 slot_cap, slot_cap2;
	u8 poweron;
	struct pci_dev *pdev = dev->port;
	struct pci_bus *subordinate = pdev->subordinate;

	ctrl = kzalloc(sizeof(*ctrl), GFP_KERNEL);
	if (!ctrl)
		return NULL;

	ctrl->pcie = dev;
	ctrl->depth = pcie_hotplug_depth(dev->port);
	pcie_capability_read_dword(pdev, PCI_EXP_SLTCAP, &slot_cap);

	if (pdev->hotplug_user_indicators)
		slot_cap &= ~(PCI_EXP_SLTCAP_AIP | PCI_EXP_SLTCAP_PIP);

	/*
	 * We assume no Thunderbolt controllers support Command Complete events,
	 * but some controllers falsely claim they do.
	 */
	if (pdev->is_thunderbolt)
		slot_cap |= PCI_EXP_SLTCAP_NCCS;

	ctrl->slot_cap = slot_cap;
	mutex_init(&ctrl->ctrl_lock);
	mutex_init(&ctrl->state_lock);
	init_rwsem(&ctrl->reset_lock);
	init_waitqueue_head(&ctrl->requester);
	init_waitqueue_head(&ctrl->queue);
	INIT_DELAYED_WORK(&ctrl->button_work, pciehp_queue_pushbutton_work);
	dbg_ctrl(ctrl);

	down_read(&pci_bus_sem);
	ctrl->state = list_empty(&subordinate->devices) ? OFF_STATE : ON_STATE;
	up_read(&pci_bus_sem);

	pcie_capability_read_dword(pdev, PCI_EXP_SLTCAP2, &slot_cap2);
	if (slot_cap2 & PCI_EXP_SLTCAP2_IBPD) {
		pcie_write_cmd_nowait(ctrl, PCI_EXP_SLTCTL_IBPD_DISABLE,
				      PCI_EXP_SLTCTL_IBPD_DISABLE);
		ctrl->inband_presence_disabled = 1;
	}

	if (dmi_first_match(inband_presence_disabled_dmi_table))
		ctrl->inband_presence_disabled = 1;

	/* Clear all remaining event bits in Slot Status register. */
	pcie_capability_write_word(pdev, PCI_EXP_SLTSTA,
		PCI_EXP_SLTSTA_ABP | PCI_EXP_SLTSTA_PFD |
		PCI_EXP_SLTSTA_MRLSC | PCI_EXP_SLTSTA_CC |
		PCI_EXP_SLTSTA_DLLSC | PCI_EXP_SLTSTA_PDC);

	ctrl_info(ctrl, "Slot #%d AttnBtn%c PwrCtrl%c MRL%c AttnInd%c PwrInd%c HotPlug%c Surprise%c Interlock%c NoCompl%c IbPresDis%c LLActRep%c%s\n",
		FIELD_GET(PCI_EXP_SLTCAP_PSN, slot_cap),
		FLAG(slot_cap, PCI_EXP_SLTCAP_ABP),
		FLAG(slot_cap, PCI_EXP_SLTCAP_PCP),
		FLAG(slot_cap, PCI_EXP_SLTCAP_MRLSP),
		FLAG(slot_cap, PCI_EXP_SLTCAP_AIP),
		FLAG(slot_cap, PCI_EXP_SLTCAP_PIP),
		FLAG(slot_cap, PCI_EXP_SLTCAP_HPC),
		FLAG(slot_cap, PCI_EXP_SLTCAP_HPS),
		FLAG(slot_cap, PCI_EXP_SLTCAP_EIP),
		FLAG(slot_cap, PCI_EXP_SLTCAP_NCCS),
		FLAG(slot_cap2, PCI_EXP_SLTCAP2_IBPD),
		FLAG(pdev->link_active_reporting, true),
		pdev->broken_cmd_compl ? " (with Cmd Compl erratum)" : "");

	/*
	 * If empty slot's power status is on, turn power off.  The IRQ isn't
	 * requested yet, so avoid triggering a notification with this command.
	 */
	if (POWER_CTRL(ctrl)) {
		pciehp_get_power_status(ctrl, &poweron);
		if (!pciehp_card_present_or_link_active(ctrl) && poweron) {
			pcie_disable_notification(ctrl);
			pciehp_power_off_slot(ctrl);
		}
	}

	return ctrl;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

void pciehp_release_ctrl(struct controller *ctrl)
{
<<<<<<< HEAD
	pcie_shutdown_notification(ctrl);
	pcie_cleanup_slot(ctrl);
	kfree(ctrl);
}
=======
	cancel_delayed_work_sync(&ctrl->button_work);
	kfree(ctrl);
}

static void quirk_cmd_compl(struct pci_dev *pdev)
{
	u32 slot_cap;

	if (pci_is_pcie(pdev)) {
		pcie_capability_read_dword(pdev, PCI_EXP_SLTCAP, &slot_cap);
		if (slot_cap & PCI_EXP_SLTCAP_HPC &&
		    !(slot_cap & PCI_EXP_SLTCAP_NCCS))
			pdev->broken_cmd_compl = 1;
	}
}
DECLARE_PCI_FIXUP_CLASS_EARLY(PCI_VENDOR_ID_INTEL, PCI_ANY_ID,
			      PCI_CLASS_BRIDGE_PCI, 8, quirk_cmd_compl);
DECLARE_PCI_FIXUP_CLASS_EARLY(PCI_VENDOR_ID_QCOM, 0x010e,
			      PCI_CLASS_BRIDGE_PCI, 8, quirk_cmd_compl);
DECLARE_PCI_FIXUP_CLASS_EARLY(PCI_VENDOR_ID_QCOM, 0x0110,
			      PCI_CLASS_BRIDGE_PCI, 8, quirk_cmd_compl);
DECLARE_PCI_FIXUP_CLASS_EARLY(PCI_VENDOR_ID_QCOM, 0x0400,
			      PCI_CLASS_BRIDGE_PCI, 8, quirk_cmd_compl);
DECLARE_PCI_FIXUP_CLASS_EARLY(PCI_VENDOR_ID_QCOM, 0x0401,
			      PCI_CLASS_BRIDGE_PCI, 8, quirk_cmd_compl);
DECLARE_PCI_FIXUP_CLASS_EARLY(PCI_VENDOR_ID_HXT, 0x0401,
			      PCI_CLASS_BRIDGE_PCI, 8, quirk_cmd_compl);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
