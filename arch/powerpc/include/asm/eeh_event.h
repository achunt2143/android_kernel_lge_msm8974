<<<<<<< HEAD
/*
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307 USA
=======
/* SPDX-License-Identifier: GPL-2.0-or-later */
/*
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 *
 * Copyright (c) 2005 Linas Vepstas <linas@linas.org>
 */

#ifndef ASM_POWERPC_EEH_EVENT_H
#define ASM_POWERPC_EEH_EVENT_H
#ifdef __KERNEL__

/*
 * structure holding pci controller data that describes a
 * change in the isolation status of a PCI slot.  A pointer
 * to this struct is passed as the data pointer in a notify
 * callback.
 */
struct eeh_event {
	struct list_head	list;	/* to form event queue	*/
<<<<<<< HEAD
	struct eeh_dev		*edev;	/* EEH device		*/
};

int eeh_send_failure_event(struct eeh_dev *edev);
struct eeh_dev *handle_eeh_events(struct eeh_event *);
=======
	struct eeh_pe		*pe;	/* EEH PE		*/
};

int eeh_event_init(void);
int eeh_send_failure_event(struct eeh_pe *pe);
int __eeh_send_failure_event(struct eeh_pe *pe);
void eeh_remove_event(struct eeh_pe *pe, bool force);
void eeh_handle_normal_event(struct eeh_pe *pe);
void eeh_handle_special_event(void);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

#endif /* __KERNEL__ */
#endif /* ASM_POWERPC_EEH_EVENT_H */
