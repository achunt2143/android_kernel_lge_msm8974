<<<<<<< HEAD
#ifndef __UM_TIMEX_H
#define __UM_TIMEX_H

typedef unsigned long cycles_t;

static inline cycles_t get_cycles (void)
{
	return 0;
}

#define CLOCK_TICK_RATE (HZ)

=======
/* SPDX-License-Identifier: GPL-2.0 */
#ifndef __UM_TIMEX_H
#define __UM_TIMEX_H

#define CLOCK_TICK_RATE (HZ)

#include <asm-generic/timex.h>

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#endif
