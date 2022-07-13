<<<<<<< HEAD
=======
/* SPDX-License-Identifier: GPL-2.0-only */
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
/*
 * xchg/cmpxchg operations for the Hexagon architecture
 *
 * Copyright (c) 2010-2011, The Linux Foundation. All rights reserved.
<<<<<<< HEAD
 *
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
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 */

#ifndef _ASM_CMPXCHG_H
#define _ASM_CMPXCHG_H

/*
<<<<<<< HEAD
 * __xchg - atomically exchange a register and a memory location
=======
 * __arch_xchg - atomically exchange a register and a memory location
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 * @x: value to swap
 * @ptr: pointer to memory
 * @size:  size of the value
 *
 * Only 4 bytes supported currently.
 *
 * Note:  there was an errata for V2 about .new's and memw_locked.
 *
 */
<<<<<<< HEAD
static inline unsigned long __xchg(unsigned long x, volatile void *ptr,
				   int size)
=======
static inline unsigned long
__arch_xchg(unsigned long x, volatile void *ptr, int size)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
{
	unsigned long retval;

	/*  Can't seem to use printk or panic here, so just stop  */
	if (size != 4) do { asm volatile("brkpt;\n"); } while (1);

	__asm__ __volatile__ (
	"1:	%0 = memw_locked(%1);\n"    /*  load into retval */
	"	memw_locked(%1,P0) = %2;\n" /*  store into memory */
<<<<<<< HEAD
	"	if !P0 jump 1b;\n"
=======
	"	if (!P0) jump 1b;\n"
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	: "=&r" (retval)
	: "r" (ptr), "r" (x)
	: "memory", "p0"
	);
	return retval;
}

/*
 * Atomically swap the contents of a register with memory.  Should be atomic
 * between multiple CPU's and within interrupts on the same CPU.
 */
<<<<<<< HEAD
#define xchg(ptr, v) ((__typeof__(*(ptr)))__xchg((unsigned long)(v), (ptr), \
	sizeof(*(ptr))))
=======
#define arch_xchg(ptr, v) ((__typeof__(*(ptr)))__arch_xchg((unsigned long)(v), (ptr), \
							   sizeof(*(ptr))))
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

/*
 *  see rt-mutex-design.txt; cmpxchg supposedly checks if *ptr == A and swaps.
 *  looks just like atomic_cmpxchg on our arch currently with a bunch of
 *  variable casting.
 */
<<<<<<< HEAD
#define __HAVE_ARCH_CMPXCHG 1

#define cmpxchg(ptr, old, new)					\
=======

#define arch_cmpxchg(ptr, old, new)				\
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
({								\
	__typeof__(ptr) __ptr = (ptr);				\
	__typeof__(*(ptr)) __old = (old);			\
	__typeof__(*(ptr)) __new = (new);			\
	__typeof__(*(ptr)) __oldval = 0;			\
								\
	asm volatile(						\
		"1:	%0 = memw_locked(%1);\n"		\
		"	{ P0 = cmp.eq(%0,%2);\n"		\
		"	  if (!P0.new) jump:nt 2f; }\n"		\
		"	memw_locked(%1,p0) = %3;\n"		\
		"	if (!P0) jump 1b;\n"			\
		"2:\n"						\
		: "=&r" (__oldval)				\
		: "r" (__ptr), "r" (__old), "r" (__new)		\
		: "memory", "p0"				\
	);							\
	__oldval;						\
})

#endif /* _ASM_CMPXCHG_H */
