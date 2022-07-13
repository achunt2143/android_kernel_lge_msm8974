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
=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 * Send feedback to <greg@kroah.com>, <kristen.c.accardi@intel.com>
 *
 */

<<<<<<< HEAD
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/types.h>
#include <linux/slab.h>
#include <linux/pci.h>
#include "../pci.h"
#include "pciehp.h"

static void interrupt_event_handler(struct work_struct *work);

static int queue_interrupt_event(struct slot *p_slot, u32 event_type)
{
	struct event_info *info;

	info = kmalloc(sizeof(*info), GFP_ATOMIC);
	if (!info)
		return -ENOMEM;

	info->event_type = event_type;
	info->p_slot = p_slot;
	INIT_WORK(&info->work, interrupt_event_handler);

	queue_work(p_slot->wq, &info->work);

	return 0;
}

u8 pciehp_handle_attention_button(struct slot *p_slot)
{
	u32 event_type;
	struct controller *ctrl = p_slot->ctrl;

	/* Attention Button Change */
	ctrl_dbg(ctrl, "Attention button interrupt received\n");

	/*
	 *  Button pressed - See if need to TAKE ACTION!!!
	 */
	ctrl_info(ctrl, "Button pressed on Slot(%s)\n", slot_name(p_slot));
	event_type = INT_BUTTON_PRESS;

	queue_interrupt_event(p_slot, event_type);

	return 0;
}

u8 pciehp_handle_switch_change(struct slot *p_slot)
{
	u8 getstatus;
	u32 event_type;
	struct controller *ctrl = p_slot->ctrl;

	/* Switch Change */
	ctrl_dbg(ctrl, "Switch interrupt received\n");

	pciehp_get_latch_status(p_slot, &getstatus);
	if (getstatus) {
		/*
		 * Switch opened
		 */
		ctrl_info(ctrl, "Latch open on Slot(%s)\n", slot_name(p_slot));
		event_type = INT_SWITCH_OPEN;
	} else {
		/*
		 *  Switch closed
		 */
		ctrl_info(ctrl, "Latch close on Slot(%s)\n", slot_name(p_slot));
		event_type = INT_SWITCH_CLOSE;
	}

	queue_interrupt_event(p_slot, event_type);

	return 1;
}

u8 pciehp_handle_presence_change(struct slot *p_slot)
{
	u32 event_type;
	u8 presence_save;
	struct controller *ctrl = p_slot->ctrl;

	/* Presence Change */
	ctrl_dbg(ctrl, "Presence/Notify input change\n");

	/* Switch is open, assume a presence change
	 * Save the presence state
	 */
	pciehp_get_adapter_status(p_slot, &presence_save);
	if (presence_save) {
		/*
		 * Card Present
		 */
		ctrl_info(ctrl, "Card present on Slot(%s)\n", slot_name(p_slot));
		event_type = INT_PRESENCE_ON;
	} else {
		/*
		 * Not Present
		 */
		ctrl_info(ctrl, "Card not present on Slot(%s)\n",
			  slot_name(p_slot));
		event_type = INT_PRESENCE_OFF;
	}

	queue_interrupt_event(p_slot, event_type);

	return 1;
}

u8 pciehp_handle_power_fault(struct slot *p_slot)
{
	u32 event_type;
	struct controller *ctrl = p_slot->ctrl;

	/* power fault */
	ctrl_dbg(ctrl, "Power fault interrupt received\n");
	ctrl_err(ctrl, "Power fault on slot %s\n", slot_name(p_slot));
	event_type = INT_POWER_FAULT;
	ctrl_info(ctrl, "Power fault bit %x set\n", 0);
	queue_interrupt_event(p_slot, event_type);

	return 1;
}

=======
#define dev_fmt(fmt) "pciehp: " fmt

#include <linux/kernel.h>
#include <linux/types.h>
#include <linux/pm_runtime.h>
#include <linux/pci.h>
#include "pciehp.h"

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
/* The following routines constitute the bulk of the
   hotplug controller logic
 */

<<<<<<< HEAD
static void set_slot_off(struct controller *ctrl, struct slot * pslot)
{
	/* turn off slot, turn on Amber LED, turn off Green LED if supported*/
	if (POWER_CTRL(ctrl)) {
		if (pciehp_power_off_slot(pslot)) {
			ctrl_err(ctrl,
				 "Issue of Slot Power Off command failed\n");
			return;
		}
=======
#define SAFE_REMOVAL	 true
#define SURPRISE_REMOVAL false

static void set_slot_off(struct controller *ctrl)
{
	/*
	 * Turn off slot, turn on attention indicator, turn off power
	 * indicator
	 */
	if (POWER_CTRL(ctrl)) {
		pciehp_power_off_slot(ctrl);

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		/*
		 * After turning power off, we must wait for at least 1 second
		 * before taking any action that relies on power having been
		 * removed from the slot/adapter.
		 */
		msleep(1000);
	}

<<<<<<< HEAD
	if (PWR_LED(ctrl))
		pciehp_green_led_off(pslot);

	if (ATTN_LED(ctrl)) {
		if (pciehp_set_attention_status(pslot, 1)) {
			ctrl_err(ctrl,
				 "Issue of Set Attention Led command failed\n");
			return;
		}
	}
=======
	pciehp_set_indicators(ctrl, PCI_EXP_SLTCTL_PWR_IND_OFF,
			      PCI_EXP_SLTCTL_ATTN_IND_ON);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

/**
 * board_added - Called after a board has been added to the system.
<<<<<<< HEAD
 * @p_slot: &slot where board is added
=======
 * @ctrl: PCIe hotplug controller where board is added
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 *
 * Turns power on for the board.
 * Configures board.
 */
<<<<<<< HEAD
static int board_added(struct slot *p_slot)
{
	int retval = 0;
	struct controller *ctrl = p_slot->ctrl;
=======
static int board_added(struct controller *ctrl)
{
	int retval = 0;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	struct pci_bus *parent = ctrl->pcie->port->subordinate;

	if (POWER_CTRL(ctrl)) {
		/* Power on slot */
<<<<<<< HEAD
		retval = pciehp_power_on_slot(p_slot);
=======
		retval = pciehp_power_on_slot(ctrl);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		if (retval)
			return retval;
	}

<<<<<<< HEAD
	if (PWR_LED(ctrl))
		pciehp_green_led_blink(p_slot);

	/* Check link training status */
	retval = pciehp_check_link_status(ctrl);
	if (retval) {
		ctrl_err(ctrl, "Failed to check link status\n");
		goto err_exit;
	}

	/* Check for a power fault */
	if (ctrl->power_fault_detected || pciehp_query_power_fault(p_slot)) {
		ctrl_err(ctrl, "Power fault on slot %s\n", slot_name(p_slot));
=======
	pciehp_set_indicators(ctrl, PCI_EXP_SLTCTL_PWR_IND_BLINK,
			      INDICATOR_NOOP);

	/* Check link training status */
	retval = pciehp_check_link_status(ctrl);
	if (retval)
		goto err_exit;

	/* Check for a power fault */
	if (ctrl->power_fault_detected || pciehp_query_power_fault(ctrl)) {
		ctrl_err(ctrl, "Slot(%s): Power fault\n", slot_name(ctrl));
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		retval = -EIO;
		goto err_exit;
	}

<<<<<<< HEAD
	retval = pciehp_configure_device(p_slot);
	if (retval) {
		ctrl_err(ctrl, "Cannot add device at %04x:%02x:00\n",
			 pci_domain_nr(parent), parent->number);
		goto err_exit;
	}

	if (PWR_LED(ctrl))
		pciehp_green_led_on(p_slot);

	return 0;

err_exit:
	set_slot_off(ctrl, p_slot);
=======
	retval = pciehp_configure_device(ctrl);
	if (retval) {
		if (retval != -EEXIST) {
			ctrl_err(ctrl, "Cannot add device at %04x:%02x:00\n",
				 pci_domain_nr(parent), parent->number);
			goto err_exit;
		}
	}

	pciehp_set_indicators(ctrl, PCI_EXP_SLTCTL_PWR_IND_ON,
			      PCI_EXP_SLTCTL_ATTN_IND_OFF);
	return 0;

err_exit:
	set_slot_off(ctrl);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	return retval;
}

/**
<<<<<<< HEAD
 * remove_board - Turns off slot and LEDs
 * @p_slot: slot where board is being removed
 */
static int remove_board(struct slot *p_slot)
{
	int retval = 0;
	struct controller *ctrl = p_slot->ctrl;

	retval = pciehp_unconfigure_device(p_slot);
	if (retval)
		return retval;

	if (POWER_CTRL(ctrl)) {
		/* power off slot */
		retval = pciehp_power_off_slot(p_slot);
		if (retval) {
			ctrl_err(ctrl,
				 "Issue of Slot Disable command failed\n");
			return retval;
		}
=======
 * remove_board - Turn off slot and Power Indicator
 * @ctrl: PCIe hotplug controller where board is being removed
 * @safe_removal: whether the board is safely removed (versus surprise removed)
 */
static void remove_board(struct controller *ctrl, bool safe_removal)
{
	pciehp_unconfigure_device(ctrl, safe_removal);

	if (POWER_CTRL(ctrl)) {
		pciehp_power_off_slot(ctrl);

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		/*
		 * After turning power off, we must wait for at least 1 second
		 * before taking any action that relies on power having been
		 * removed from the slot/adapter.
		 */
		msleep(1000);
<<<<<<< HEAD
	}

	/* turn off Green LED */
	if (PWR_LED(ctrl))
		pciehp_green_led_off(p_slot);

	return 0;
}

struct power_work_info {
	struct slot *p_slot;
	struct work_struct work;
};

/**
 * pciehp_power_thread - handle pushbutton events
 * @work: &struct work_struct describing work to be done
 *
 * Scheduled procedure to handle blocking stuff for the pushbuttons.
 * Handles all pending events and exits.
 */
static void pciehp_power_thread(struct work_struct *work)
{
	struct power_work_info *info =
		container_of(work, struct power_work_info, work);
	struct slot *p_slot = info->p_slot;

	mutex_lock(&p_slot->lock);
	switch (p_slot->state) {
	case POWEROFF_STATE:
		mutex_unlock(&p_slot->lock);
		ctrl_dbg(p_slot->ctrl,
			 "Disabling domain:bus:device=%04x:%02x:00\n",
			 pci_domain_nr(p_slot->ctrl->pcie->port->subordinate),
			 p_slot->ctrl->pcie->port->subordinate->number);
		pciehp_disable_slot(p_slot);
		mutex_lock(&p_slot->lock);
		p_slot->state = STATIC_STATE;
		break;
	case POWERON_STATE:
		mutex_unlock(&p_slot->lock);
		if (pciehp_enable_slot(p_slot) && PWR_LED(p_slot->ctrl))
			pciehp_green_led_off(p_slot);
		mutex_lock(&p_slot->lock);
		p_slot->state = STATIC_STATE;
		break;
	default:
		break;
	}
	mutex_unlock(&p_slot->lock);

	kfree(info);
=======

		/* Ignore link or presence changes caused by power off */
		atomic_and(~(PCI_EXP_SLTSTA_DLLSC | PCI_EXP_SLTSTA_PDC),
			   &ctrl->pending_events);
	}

	pciehp_set_indicators(ctrl, PCI_EXP_SLTCTL_PWR_IND_OFF,
			      INDICATOR_NOOP);
}

static int pciehp_enable_slot(struct controller *ctrl);
static int pciehp_disable_slot(struct controller *ctrl, bool safe_removal);

void pciehp_request(struct controller *ctrl, int action)
{
	atomic_or(action, &ctrl->pending_events);
	if (!pciehp_poll_mode)
		irq_wake_thread(ctrl->pcie->irq, ctrl);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

void pciehp_queue_pushbutton_work(struct work_struct *work)
{
<<<<<<< HEAD
	struct slot *p_slot = container_of(work, struct slot, work.work);
	struct power_work_info *info;

	info = kmalloc(sizeof(*info), GFP_KERNEL);
	if (!info) {
		ctrl_err(p_slot->ctrl, "%s: Cannot allocate memory\n",
			 __func__);
		return;
	}
	info->p_slot = p_slot;
	INIT_WORK(&info->work, pciehp_power_thread);

	mutex_lock(&p_slot->lock);
	switch (p_slot->state) {
	case BLINKINGOFF_STATE:
		p_slot->state = POWEROFF_STATE;
		break;
	case BLINKINGON_STATE:
		p_slot->state = POWERON_STATE;
		break;
	default:
		kfree(info);
		goto out;
	}
	queue_work(p_slot->wq, &info->work);
 out:
	mutex_unlock(&p_slot->lock);
}

/*
 * Note: This function must be called with slot->lock held
 */
static void handle_button_press_event(struct slot *p_slot)
{
	struct controller *ctrl = p_slot->ctrl;
	u8 getstatus;

	switch (p_slot->state) {
	case STATIC_STATE:
		pciehp_get_power_status(p_slot, &getstatus);
		if (getstatus) {
			p_slot->state = BLINKINGOFF_STATE;
			ctrl_info(ctrl,
				  "PCI slot #%s - powering off due to button "
				  "press.\n", slot_name(p_slot));
		} else {
			p_slot->state = BLINKINGON_STATE;
			ctrl_info(ctrl,
				  "PCI slot #%s - powering on due to button "
				  "press.\n", slot_name(p_slot));
		}
		/* blink green LED and turn off amber */
		if (PWR_LED(ctrl))
			pciehp_green_led_blink(p_slot);
		if (ATTN_LED(ctrl))
			pciehp_set_attention_status(p_slot, 0);

		queue_delayed_work(p_slot->wq, &p_slot->work, 5*HZ);
=======
	struct controller *ctrl = container_of(work, struct controller,
					       button_work.work);

	mutex_lock(&ctrl->state_lock);
	switch (ctrl->state) {
	case BLINKINGOFF_STATE:
		pciehp_request(ctrl, DISABLE_SLOT);
		break;
	case BLINKINGON_STATE:
		pciehp_request(ctrl, PCI_EXP_SLTSTA_PDC);
		break;
	default:
		break;
	}
	mutex_unlock(&ctrl->state_lock);
}

void pciehp_handle_button_press(struct controller *ctrl)
{
	mutex_lock(&ctrl->state_lock);
	switch (ctrl->state) {
	case OFF_STATE:
	case ON_STATE:
		if (ctrl->state == ON_STATE) {
			ctrl->state = BLINKINGOFF_STATE;
			ctrl_info(ctrl, "Slot(%s): Button press: will power off in 5 sec\n",
				  slot_name(ctrl));
		} else {
			ctrl->state = BLINKINGON_STATE;
			ctrl_info(ctrl, "Slot(%s): Button press: will power on in 5 sec\n",
				  slot_name(ctrl));
		}
		/* blink power indicator and turn off attention */
		pciehp_set_indicators(ctrl, PCI_EXP_SLTCTL_PWR_IND_BLINK,
				      PCI_EXP_SLTCTL_ATTN_IND_OFF);
		schedule_delayed_work(&ctrl->button_work, 5 * HZ);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		break;
	case BLINKINGOFF_STATE:
	case BLINKINGON_STATE:
		/*
		 * Cancel if we are still blinking; this means that we
		 * press the attention again before the 5 sec. limit
		 * expires to cancel hot-add or hot-remove
		 */
<<<<<<< HEAD
		ctrl_info(ctrl, "Button cancel on Slot(%s)\n", slot_name(p_slot));
		cancel_delayed_work(&p_slot->work);
		if (p_slot->state == BLINKINGOFF_STATE) {
			if (PWR_LED(ctrl))
				pciehp_green_led_on(p_slot);
		} else {
			if (PWR_LED(ctrl))
				pciehp_green_led_off(p_slot);
		}
		if (ATTN_LED(ctrl))
			pciehp_set_attention_status(p_slot, 0);
		ctrl_info(ctrl, "PCI slot #%s - action canceled "
			  "due to button press\n", slot_name(p_slot));
		p_slot->state = STATIC_STATE;
		break;
	case POWEROFF_STATE:
	case POWERON_STATE:
		/*
		 * Ignore if the slot is on power-on or power-off state;
		 * this means that the previous attention button action
		 * to hot-add or hot-remove is undergoing
		 */
		ctrl_info(ctrl, "Button ignore on Slot(%s)\n", slot_name(p_slot));
		break;
	default:
		ctrl_warn(ctrl, "Not a valid state\n");
		break;
	}
}

/*
 * Note: This function must be called with slot->lock held
 */
static void handle_surprise_event(struct slot *p_slot)
{
	u8 getstatus;
	struct power_work_info *info;

	info = kmalloc(sizeof(*info), GFP_KERNEL);
	if (!info) {
		ctrl_err(p_slot->ctrl, "%s: Cannot allocate memory\n",
			 __func__);
		return;
	}
	info->p_slot = p_slot;
	INIT_WORK(&info->work, pciehp_power_thread);

	pciehp_get_adapter_status(p_slot, &getstatus);
	if (!getstatus)
		p_slot->state = POWEROFF_STATE;
	else
		p_slot->state = POWERON_STATE;

	queue_work(p_slot->wq, &info->work);
}

static void interrupt_event_handler(struct work_struct *work)
{
	struct event_info *info = container_of(work, struct event_info, work);
	struct slot *p_slot = info->p_slot;
	struct controller *ctrl = p_slot->ctrl;

	mutex_lock(&p_slot->lock);
	switch (info->event_type) {
	case INT_BUTTON_PRESS:
		handle_button_press_event(p_slot);
		break;
	case INT_POWER_FAULT:
		if (!POWER_CTRL(ctrl))
			break;
		if (ATTN_LED(ctrl))
			pciehp_set_attention_status(p_slot, 1);
		if (PWR_LED(ctrl))
			pciehp_green_led_off(p_slot);
		break;
	case INT_PRESENCE_ON:
	case INT_PRESENCE_OFF:
		if (!HP_SUPR_RM(ctrl))
			break;
		ctrl_dbg(ctrl, "Surprise Removal\n");
		handle_surprise_event(p_slot);
		break;
	default:
		break;
	}
	mutex_unlock(&p_slot->lock);

	kfree(info);
}

int pciehp_enable_slot(struct slot *p_slot)
{
	u8 getstatus = 0;
	int rc;
	struct controller *ctrl = p_slot->ctrl;

	rc = pciehp_get_adapter_status(p_slot, &getstatus);
	if (rc || !getstatus) {
		ctrl_info(ctrl, "No adapter on slot(%s)\n", slot_name(p_slot));
		return -ENODEV;
	}
	if (MRL_SENS(p_slot->ctrl)) {
		rc = pciehp_get_latch_status(p_slot, &getstatus);
		if (rc || getstatus) {
			ctrl_info(ctrl, "Latch open on slot(%s)\n",
				  slot_name(p_slot));
=======
		cancel_delayed_work(&ctrl->button_work);
		if (ctrl->state == BLINKINGOFF_STATE) {
			ctrl->state = ON_STATE;
			pciehp_set_indicators(ctrl, PCI_EXP_SLTCTL_PWR_IND_ON,
					      PCI_EXP_SLTCTL_ATTN_IND_OFF);
			ctrl_info(ctrl, "Slot(%s): Button press: canceling request to power off\n",
				  slot_name(ctrl));
		} else {
			ctrl->state = OFF_STATE;
			pciehp_set_indicators(ctrl, PCI_EXP_SLTCTL_PWR_IND_OFF,
					      PCI_EXP_SLTCTL_ATTN_IND_OFF);
			ctrl_info(ctrl, "Slot(%s): Button press: canceling request to power on\n",
				  slot_name(ctrl));
		}
		break;
	default:
		ctrl_err(ctrl, "Slot(%s): Button press: ignoring invalid state %#x\n",
			 slot_name(ctrl), ctrl->state);
		break;
	}
	mutex_unlock(&ctrl->state_lock);
}

void pciehp_handle_disable_request(struct controller *ctrl)
{
	mutex_lock(&ctrl->state_lock);
	switch (ctrl->state) {
	case BLINKINGON_STATE:
	case BLINKINGOFF_STATE:
		cancel_delayed_work(&ctrl->button_work);
		break;
	}
	ctrl->state = POWEROFF_STATE;
	mutex_unlock(&ctrl->state_lock);

	ctrl->request_result = pciehp_disable_slot(ctrl, SAFE_REMOVAL);
}

void pciehp_handle_presence_or_link_change(struct controller *ctrl, u32 events)
{
	int present, link_active;

	/*
	 * If the slot is on and presence or link has changed, turn it off.
	 * Even if it's occupied again, we cannot assume the card is the same.
	 */
	mutex_lock(&ctrl->state_lock);
	switch (ctrl->state) {
	case BLINKINGOFF_STATE:
		cancel_delayed_work(&ctrl->button_work);
		fallthrough;
	case ON_STATE:
		ctrl->state = POWEROFF_STATE;
		mutex_unlock(&ctrl->state_lock);
		if (events & PCI_EXP_SLTSTA_DLLSC)
			ctrl_info(ctrl, "Slot(%s): Link Down\n",
				  slot_name(ctrl));
		if (events & PCI_EXP_SLTSTA_PDC)
			ctrl_info(ctrl, "Slot(%s): Card not present\n",
				  slot_name(ctrl));
		pciehp_disable_slot(ctrl, SURPRISE_REMOVAL);
		break;
	default:
		mutex_unlock(&ctrl->state_lock);
		break;
	}

	/* Turn the slot on if it's occupied or link is up */
	mutex_lock(&ctrl->state_lock);
	present = pciehp_card_present(ctrl);
	link_active = pciehp_check_link_active(ctrl);
	if (present <= 0 && link_active <= 0) {
		if (ctrl->state == BLINKINGON_STATE) {
			ctrl->state = OFF_STATE;
			cancel_delayed_work(&ctrl->button_work);
			pciehp_set_indicators(ctrl, PCI_EXP_SLTCTL_PWR_IND_OFF,
					      INDICATOR_NOOP);
			ctrl_info(ctrl, "Slot(%s): Card not present\n",
				  slot_name(ctrl));
		}
		mutex_unlock(&ctrl->state_lock);
		return;
	}

	switch (ctrl->state) {
	case BLINKINGON_STATE:
		cancel_delayed_work(&ctrl->button_work);
		fallthrough;
	case OFF_STATE:
		ctrl->state = POWERON_STATE;
		mutex_unlock(&ctrl->state_lock);
		if (present)
			ctrl_info(ctrl, "Slot(%s): Card present\n",
				  slot_name(ctrl));
		if (link_active)
			ctrl_info(ctrl, "Slot(%s): Link Up\n",
				  slot_name(ctrl));
		ctrl->request_result = pciehp_enable_slot(ctrl);
		break;
	default:
		mutex_unlock(&ctrl->state_lock);
		break;
	}
}

static int __pciehp_enable_slot(struct controller *ctrl)
{
	u8 getstatus = 0;

	if (MRL_SENS(ctrl)) {
		pciehp_get_latch_status(ctrl, &getstatus);
		if (getstatus) {
			ctrl_info(ctrl, "Slot(%s): Latch open\n",
				  slot_name(ctrl));
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
			return -ENODEV;
		}
	}

<<<<<<< HEAD
	if (POWER_CTRL(p_slot->ctrl)) {
		rc = pciehp_get_power_status(p_slot, &getstatus);
		if (rc || getstatus) {
			ctrl_info(ctrl, "Already enabled on slot(%s)\n",
				  slot_name(p_slot));
=======
	if (POWER_CTRL(ctrl)) {
		pciehp_get_power_status(ctrl, &getstatus);
		if (getstatus) {
			ctrl_info(ctrl, "Slot(%s): Already enabled\n",
				  slot_name(ctrl));
			return 0;
		}
	}

	return board_added(ctrl);
}

static int pciehp_enable_slot(struct controller *ctrl)
{
	int ret;

	pm_runtime_get_sync(&ctrl->pcie->port->dev);
	ret = __pciehp_enable_slot(ctrl);
	if (ret && ATTN_BUTTN(ctrl))
		/* may be blinking */
		pciehp_set_indicators(ctrl, PCI_EXP_SLTCTL_PWR_IND_OFF,
				      INDICATOR_NOOP);
	pm_runtime_put(&ctrl->pcie->port->dev);

	mutex_lock(&ctrl->state_lock);
	ctrl->state = ret ? OFF_STATE : ON_STATE;
	mutex_unlock(&ctrl->state_lock);

	return ret;
}

static int __pciehp_disable_slot(struct controller *ctrl, bool safe_removal)
{
	u8 getstatus = 0;

	if (POWER_CTRL(ctrl)) {
		pciehp_get_power_status(ctrl, &getstatus);
		if (!getstatus) {
			ctrl_info(ctrl, "Slot(%s): Already disabled\n",
				  slot_name(ctrl));
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
			return -EINVAL;
		}
	}

<<<<<<< HEAD
	pciehp_get_latch_status(p_slot, &getstatus);

	rc = board_added(p_slot);
	if (rc) {
		pciehp_get_latch_status(p_slot, &getstatus);
	}
	return rc;
}


int pciehp_disable_slot(struct slot *p_slot)
{
	u8 getstatus = 0;
	int ret = 0;
	struct controller *ctrl = p_slot->ctrl;

	if (!p_slot->ctrl)
		return 1;

	if (!HP_SUPR_RM(p_slot->ctrl)) {
		ret = pciehp_get_adapter_status(p_slot, &getstatus);
		if (ret || !getstatus) {
			ctrl_info(ctrl, "No adapter on slot(%s)\n",
				  slot_name(p_slot));
			return -ENODEV;
		}
	}

	if (MRL_SENS(p_slot->ctrl)) {
		ret = pciehp_get_latch_status(p_slot, &getstatus);
		if (ret || getstatus) {
			ctrl_info(ctrl, "Latch open on slot(%s)\n",
				  slot_name(p_slot));
			return -ENODEV;
		}
	}

	if (POWER_CTRL(p_slot->ctrl)) {
		ret = pciehp_get_power_status(p_slot, &getstatus);
		if (ret || !getstatus) {
			ctrl_info(ctrl, "Already disabled on slot(%s)\n",
				  slot_name(p_slot));
			return -EINVAL;
		}
	}

	return remove_board(p_slot);
}

int pciehp_sysfs_enable_slot(struct slot *p_slot)
{
	int retval = -ENODEV;
	struct controller *ctrl = p_slot->ctrl;

	mutex_lock(&p_slot->lock);
	switch (p_slot->state) {
	case BLINKINGON_STATE:
		cancel_delayed_work(&p_slot->work);
	case STATIC_STATE:
		p_slot->state = POWERON_STATE;
		mutex_unlock(&p_slot->lock);
		retval = pciehp_enable_slot(p_slot);
		mutex_lock(&p_slot->lock);
		p_slot->state = STATIC_STATE;
		break;
	case POWERON_STATE:
		ctrl_info(ctrl, "Slot %s is already in powering on state\n",
			  slot_name(p_slot));
		break;
	case BLINKINGOFF_STATE:
	case POWEROFF_STATE:
		ctrl_info(ctrl, "Already enabled on slot %s\n",
			  slot_name(p_slot));
		break;
	default:
		ctrl_err(ctrl, "Not a valid state on slot %s\n",
			 slot_name(p_slot));
		break;
	}
	mutex_unlock(&p_slot->lock);

	return retval;
}

int pciehp_sysfs_disable_slot(struct slot *p_slot)
{
	int retval = -ENODEV;
	struct controller *ctrl = p_slot->ctrl;

	mutex_lock(&p_slot->lock);
	switch (p_slot->state) {
	case BLINKINGOFF_STATE:
		cancel_delayed_work(&p_slot->work);
	case STATIC_STATE:
		p_slot->state = POWEROFF_STATE;
		mutex_unlock(&p_slot->lock);
		retval = pciehp_disable_slot(p_slot);
		mutex_lock(&p_slot->lock);
		p_slot->state = STATIC_STATE;
		break;
	case POWEROFF_STATE:
		ctrl_info(ctrl, "Slot %s is already in powering off state\n",
			  slot_name(p_slot));
		break;
	case BLINKINGON_STATE:
	case POWERON_STATE:
		ctrl_info(ctrl, "Already disabled on slot %s\n",
			  slot_name(p_slot));
		break;
	default:
		ctrl_err(ctrl, "Not a valid state on slot %s\n",
			 slot_name(p_slot));
		break;
	}
	mutex_unlock(&p_slot->lock);

	return retval;
=======
	remove_board(ctrl, safe_removal);
	return 0;
}

static int pciehp_disable_slot(struct controller *ctrl, bool safe_removal)
{
	int ret;

	pm_runtime_get_sync(&ctrl->pcie->port->dev);
	ret = __pciehp_disable_slot(ctrl, safe_removal);
	pm_runtime_put(&ctrl->pcie->port->dev);

	mutex_lock(&ctrl->state_lock);
	ctrl->state = OFF_STATE;
	mutex_unlock(&ctrl->state_lock);

	return ret;
}

int pciehp_sysfs_enable_slot(struct hotplug_slot *hotplug_slot)
{
	struct controller *ctrl = to_ctrl(hotplug_slot);

	mutex_lock(&ctrl->state_lock);
	switch (ctrl->state) {
	case BLINKINGON_STATE:
	case OFF_STATE:
		mutex_unlock(&ctrl->state_lock);
		/*
		 * The IRQ thread becomes a no-op if the user pulls out the
		 * card before the thread wakes up, so initialize to -ENODEV.
		 */
		ctrl->request_result = -ENODEV;
		pciehp_request(ctrl, PCI_EXP_SLTSTA_PDC);
		wait_event(ctrl->requester,
			   !atomic_read(&ctrl->pending_events) &&
			   !ctrl->ist_running);
		return ctrl->request_result;
	case POWERON_STATE:
		ctrl_info(ctrl, "Slot(%s): Already in powering on state\n",
			  slot_name(ctrl));
		break;
	case BLINKINGOFF_STATE:
	case ON_STATE:
	case POWEROFF_STATE:
		ctrl_info(ctrl, "Slot(%s): Already enabled\n",
			  slot_name(ctrl));
		break;
	default:
		ctrl_err(ctrl, "Slot(%s): Invalid state %#x\n",
			 slot_name(ctrl), ctrl->state);
		break;
	}
	mutex_unlock(&ctrl->state_lock);

	return -ENODEV;
}

int pciehp_sysfs_disable_slot(struct hotplug_slot *hotplug_slot)
{
	struct controller *ctrl = to_ctrl(hotplug_slot);

	mutex_lock(&ctrl->state_lock);
	switch (ctrl->state) {
	case BLINKINGOFF_STATE:
	case ON_STATE:
		mutex_unlock(&ctrl->state_lock);
		pciehp_request(ctrl, DISABLE_SLOT);
		wait_event(ctrl->requester,
			   !atomic_read(&ctrl->pending_events) &&
			   !ctrl->ist_running);
		return ctrl->request_result;
	case POWEROFF_STATE:
		ctrl_info(ctrl, "Slot(%s): Already in powering off state\n",
			  slot_name(ctrl));
		break;
	case BLINKINGON_STATE:
	case OFF_STATE:
	case POWERON_STATE:
		ctrl_info(ctrl, "Slot(%s): Already disabled\n",
			  slot_name(ctrl));
		break;
	default:
		ctrl_err(ctrl, "Slot(%s): Invalid state %#x\n",
			 slot_name(ctrl), ctrl->state);
		break;
	}
	mutex_unlock(&ctrl->state_lock);

	return -ENODEV;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}
