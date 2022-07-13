<<<<<<< HEAD
=======
/* SPDX-License-Identifier: GPL-2.0-or-later */
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#ifndef __ASM_POWERPC_XMON_H
#define __ASM_POWERPC_XMON_H

/*
 * Copyrignt (C) 2006 IBM Corp
<<<<<<< HEAD
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version
 * 2 of the License, or (at your option) any later version.
=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 */

#ifdef __KERNEL__

#include <linux/irqreturn.h>

#ifdef CONFIG_XMON
extern void xmon_setup(void);
<<<<<<< HEAD
extern void xmon_register_spus(struct list_head *list);
=======
void __init xmon_register_spus(struct list_head *list);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
struct pt_regs;
extern int xmon(struct pt_regs *excp);
extern irqreturn_t xmon_irq(int, void *);
#else
<<<<<<< HEAD
static inline void xmon_setup(void) { };
static inline void xmon_register_spus(struct list_head *list) { };
=======
static inline void xmon_setup(void) { }
static inline void xmon_register_spus(struct list_head *list) { }
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#endif

#if defined(CONFIG_XMON) && defined(CONFIG_SMP)
extern int cpus_are_in_xmon(void);
#endif

<<<<<<< HEAD
=======
extern __printf(1, 2) void xmon_printf(const char *format, ...);

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#endif /* __KERNEL __ */
#endif /* __ASM_POWERPC_XMON_H */
