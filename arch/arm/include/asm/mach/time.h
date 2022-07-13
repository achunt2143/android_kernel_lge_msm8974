<<<<<<< HEAD
=======
/* SPDX-License-Identifier: GPL-2.0-only */
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
/*
 * arch/arm/include/asm/mach/time.h
 *
 * Copyright (C) 2004 MontaVista Software, Inc.
<<<<<<< HEAD
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 */
#ifndef __ASM_ARM_MACH_TIME_H
#define __ASM_ARM_MACH_TIME_H

<<<<<<< HEAD
/*
 * This is our kernel timer structure.
 *
 * - init
 *   Initialise the kernels jiffy timer source, claim interrupt
 *   using setup_irq.  This is called early on during initialisation
 *   while interrupts are still disabled on the local CPU.
 * - suspend
 *   Suspend the kernel jiffy timer source, if necessary.  This
 *   is called with interrupts disabled, after all normal devices
 *   have been suspended.  If no action is required, set this to
 *   NULL.
 * - resume
 *   Resume the kernel jiffy timer source, if necessary.  This
 *   is called with interrupts disabled before any normal devices
 *   are resumed.  If no action is required, set this to NULL.
 * - offset
 *   Return the timer offset in microseconds since the last timer
 *   interrupt.  Note: this must take account of any unprocessed
 *   timer interrupt which may be pending.
 */
struct sys_timer {
	void			(*init)(void);
	void			(*suspend)(void);
	void			(*resume)(void);
#ifdef CONFIG_ARCH_USES_GETTIMEOFFSET
	unsigned long		(*offset)(void);
#endif
};

extern void timer_tick(void);
=======
typedef void (*clock_access_fn)(struct timespec64 *);
extern int register_persistent_clock(clock_access_fn read_persistent);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

#endif
