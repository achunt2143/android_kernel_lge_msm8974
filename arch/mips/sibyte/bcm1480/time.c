<<<<<<< HEAD
/*
 * Copyright (C) 2000,2001,2004 Broadcom Corporation
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
=======
// SPDX-License-Identifier: GPL-2.0-or-later
/*
 * Copyright (C) 2000,2001,2004 Broadcom Corporation
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 */
#include <linux/init.h>

extern void sb1480_clockevent_init(void);
extern void sb1480_clocksource_init(void);

void __init plat_time_init(void)
{
	sb1480_clocksource_init();
	sb1480_clockevent_init();
}
