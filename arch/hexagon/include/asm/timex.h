<<<<<<< HEAD
/*
 * Copyright (c) 2010-2011, The Linux Foundation. All rights reserved.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 and
 * only version 2 as published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA
 * 02110-1301, USA.
=======
/* SPDX-License-Identifier: GPL-2.0-only */
/*
 * Copyright (c) 2010-2011, The Linux Foundation. All rights reserved.
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 */

#ifndef _ASM_TIMEX_H
#define _ASM_TIMEX_H

#include <asm-generic/timex.h>
<<<<<<< HEAD
#include <asm/timer-regs.h>

/* Using TCX0 as our clock.  CLOCK_TICK_RATE scheduled to be removed. */
#define CLOCK_TICK_RATE              TCX0_CLK_RATE
=======
#include <asm/hexagon_vm.h>

/* Using TCX0 as our clock.  CLOCK_TICK_RATE scheduled to be removed. */
#define CLOCK_TICK_RATE              19200
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

#define ARCH_HAS_READ_CURRENT_TIMER

static inline int read_current_timer(unsigned long *timer_val)
{
<<<<<<< HEAD
	*timer_val = (unsigned long) __vmgettime();
=======
	*timer_val = __vmgettime();
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	return 0;
}

#endif
