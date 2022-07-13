<<<<<<< HEAD
=======
/* SPDX-License-Identifier: GPL-2.0 */
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#ifndef _ASM_POWERPC_TIMEX_H
#define _ASM_POWERPC_TIMEX_H

#ifdef __KERNEL__

/*
 * PowerPC architecture timex specifications
 */

#include <asm/cputable.h>
<<<<<<< HEAD
#include <asm/reg.h>
=======
#include <asm/vdso/timebase.h>
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

#define CLOCK_TICK_RATE	1024000 /* Underlying HZ */

typedef unsigned long cycles_t;

static inline cycles_t get_cycles(void)
{
<<<<<<< HEAD
#ifdef __powerpc64__
	return mftb();
#else
	cycles_t ret;

	/*
	 * For the "cycle" counter we use the timebase lower half.
	 * Currently only used on SMP.
	 */

	ret = 0;

	__asm__ __volatile__(
		"97:	mftb %0\n"
		"99:\n"
		".section __ftr_fixup,\"a\"\n"
		".align 2\n"
		"98:\n"
		"	.long %1\n"
		"	.long 0\n"
		"	.long 97b-98b\n"
		"	.long 99b-98b\n"
		"	.long 0\n"
		"	.long 0\n"
		".previous"
		: "=r" (ret) : "i" (CPU_FTR_601));
	return ret;
#endif
}
=======
	return mftb();
}
#define get_cycles get_cycles
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

#endif	/* __KERNEL__ */
#endif	/* _ASM_POWERPC_TIMEX_H */
