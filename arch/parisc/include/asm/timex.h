<<<<<<< HEAD
=======
/* SPDX-License-Identifier: GPL-2.0 */
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
/*
 * linux/include/asm-parisc/timex.h
 *
 * PARISC architecture timex specifications
 */
#ifndef _ASMPARISC_TIMEX_H
#define _ASMPARISC_TIMEX_H

<<<<<<< HEAD
=======
#include <asm/special_insns.h>
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

#define CLOCK_TICK_RATE	1193180 /* Underlying HZ */

typedef unsigned long cycles_t;

<<<<<<< HEAD
static inline cycles_t get_cycles (void)
{
	return mfctl(16);
}
=======
static inline cycles_t get_cycles(void)
{
	return mfctl(16);
}
#define get_cycles get_cycles
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

#endif
