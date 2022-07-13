<<<<<<< HEAD
/*
 * Copyright (c) 2010, Intel Corporation.
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms and conditions of the GNU General Public License,
 * version 2, as published by the Free Software Foundation.
 *
 * This program is distributed in the hope it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for
 * more details.
 *
 * You should have received a copy of the GNU General Public License along with
 * this program; if not, write to the Free Software Foundation, Inc., 59 Temple
 * Place - Suite 330, Boston, MA 02111-1307 USA.
 *
=======
/* SPDX-License-Identifier: GPL-2.0-only */
/*
 * Copyright (c) 2010, Intel Corporation.
 *
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 * Author: John Fastabend <john.r.fastabend@intel.com>
 */

#ifndef _DCB_EVENT_H
#define _DCB_EVENT_H

<<<<<<< HEAD
=======
struct notifier_block;

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
enum dcbevent_notif_type {
	DCB_APP_EVENT = 1,
};

#ifdef CONFIG_DCB
<<<<<<< HEAD
extern int register_dcbevent_notifier(struct notifier_block *nb);
extern int unregister_dcbevent_notifier(struct notifier_block *nb);
extern int call_dcbevent_notifiers(unsigned long val, void *v);
=======
int register_dcbevent_notifier(struct notifier_block *nb);
int unregister_dcbevent_notifier(struct notifier_block *nb);
int call_dcbevent_notifiers(unsigned long val, void *v);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#else
static inline int
register_dcbevent_notifier(struct notifier_block *nb)
{
	return 0;
}

static inline int unregister_dcbevent_notifier(struct notifier_block *nb)
{
	return 0;
}

static inline int call_dcbevent_notifiers(unsigned long val, void *v)
{
	return 0;
}
#endif /* CONFIG_DCB */

#endif
