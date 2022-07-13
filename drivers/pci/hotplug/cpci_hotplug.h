<<<<<<< HEAD
=======
/* SPDX-License-Identifier: GPL-2.0+ */
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
/*
 * CompactPCI Hot Plug Core Functions
 *
 * Copyright (C) 2002 SOMA Networks, Inc.
 * Copyright (C) 2001 Greg Kroah-Hartman (greg@kroah.com)
 * Copyright (C) 2001 IBM Corp.
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
 * Send feedback to <scottm@somanetworks.com>
 */

#ifndef _CPCI_HOTPLUG_H
#define _CPCI_HOTPLUG_H

#include <linux/types.h>
#include <linux/pci.h>
#include <linux/pci_hotplug.h>

/* PICMG 2.1 R2.0 HS CSR bits: */
#define HS_CSR_INS	0x0080
#define HS_CSR_EXT	0x0040
#define HS_CSR_PI	0x0030
#define HS_CSR_LOO	0x0008
#define HS_CSR_PIE	0x0004
#define HS_CSR_EIM	0x0002
#define HS_CSR_DHA	0x0001

struct slot {
	u8 number;
	unsigned int devfn;
	struct pci_bus *bus;
	struct pci_dev *dev;
<<<<<<< HEAD
	unsigned int extracting;
	struct hotplug_slot *hotplug_slot;
=======
	unsigned int latch_status:1;
	unsigned int adapter_status:1;
	unsigned int extracting;
	struct hotplug_slot hotplug_slot;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	struct list_head slot_list;
};

struct cpci_hp_controller_ops {
<<<<<<< HEAD
	int (*query_enum) (void);
	int (*enable_irq) (void);
	int (*disable_irq) (void);
	int (*check_irq) (void *dev_id);
	int (*hardware_test) (struct slot* slot, u32 value);
	u8  (*get_power) (struct slot* slot);
	int (*set_power) (struct slot* slot, int value);
=======
	int (*query_enum)(void);
	int (*enable_irq)(void);
	int (*disable_irq)(void);
	int (*check_irq)(void *dev_id);
	int (*hardware_test)(struct slot *slot, u32 value);
	u8  (*get_power)(struct slot *slot);
	int (*set_power)(struct slot *slot, int value);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
};

struct cpci_hp_controller {
	unsigned int irq;
	unsigned long irq_flags;
	char *devname;
	void *dev_id;
	char *name;
	struct cpci_hp_controller_ops *ops;
};

static inline const char *slot_name(struct slot *slot)
{
<<<<<<< HEAD
	return hotplug_slot_name(slot->hotplug_slot);
}

extern int cpci_hp_register_controller(struct cpci_hp_controller *controller);
extern int cpci_hp_unregister_controller(struct cpci_hp_controller *controller);
extern int cpci_hp_register_bus(struct pci_bus *bus, u8 first, u8 last);
extern int cpci_hp_unregister_bus(struct pci_bus *bus);
extern int cpci_hp_start(void);
extern int cpci_hp_stop(void);
=======
	return hotplug_slot_name(&slot->hotplug_slot);
}

static inline struct slot *to_slot(struct hotplug_slot *hotplug_slot)
{
	return container_of(hotplug_slot, struct slot, hotplug_slot);
}

int cpci_hp_register_controller(struct cpci_hp_controller *controller);
int cpci_hp_unregister_controller(struct cpci_hp_controller *controller);
int cpci_hp_register_bus(struct pci_bus *bus, u8 first, u8 last);
int cpci_hp_unregister_bus(struct pci_bus *bus);
int cpci_hp_start(void);
int cpci_hp_stop(void);

/* Global variables */
extern int cpci_debug;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

/*
 * Internal function prototypes, these functions should not be used by
 * board/chassis drivers.
 */
<<<<<<< HEAD
extern u8 cpci_get_attention_status(struct slot *slot);
extern u8 cpci_get_latch_status(struct slot *slot);
extern u8 cpci_get_adapter_status(struct slot *slot);
extern u16 cpci_get_hs_csr(struct slot * slot);
extern int cpci_set_attention_status(struct slot *slot, int status);
extern int cpci_check_and_clear_ins(struct slot * slot);
extern int cpci_check_ext(struct slot * slot);
extern int cpci_clear_ext(struct slot * slot);
extern int cpci_led_on(struct slot * slot);
extern int cpci_led_off(struct slot * slot);
extern int cpci_configure_slot(struct slot *slot);
extern int cpci_unconfigure_slot(struct slot *slot);
=======
u8 cpci_get_attention_status(struct slot *slot);
u16 cpci_get_hs_csr(struct slot *slot);
int cpci_set_attention_status(struct slot *slot, int status);
int cpci_check_and_clear_ins(struct slot *slot);
int cpci_check_ext(struct slot *slot);
int cpci_clear_ext(struct slot *slot);
int cpci_led_on(struct slot *slot);
int cpci_led_off(struct slot *slot);
int cpci_configure_slot(struct slot *slot);
int cpci_unconfigure_slot(struct slot *slot);

#ifdef CONFIG_HOTPLUG_PCI_CPCI
int cpci_hotplug_init(int debug);
#else
static inline int cpci_hotplug_init(int debug) { return 0; }
#endif
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

#endif	/* _CPCI_HOTPLUG_H */
