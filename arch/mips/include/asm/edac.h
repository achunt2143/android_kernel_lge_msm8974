<<<<<<< HEAD
#ifndef ASM_EDAC_H
#define ASM_EDAC_H

/* ECC atomic, DMA, SMP and interrupt safe scrub function */

static inline void atomic_scrub(void *va, u32 size)
=======
/* SPDX-License-Identifier: GPL-2.0 */
#ifndef ASM_EDAC_H
#define ASM_EDAC_H

#include <asm/compiler.h>

/* ECC atomic, DMA, SMP and interrupt safe scrub function */

static inline void edac_atomic_scrub(void *va, u32 size)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
{
	unsigned long *virt_addr = va;
	unsigned long temp;
	u32 i;

	for (i = 0; i < size / sizeof(unsigned long); i++) {
		/*
		 * Very carefully read and write to memory atomically
		 * so we are interrupt, DMA and SMP safe.
		 *
		 * Intel: asm("lock; addl $0, %0"::"m"(*virt_addr));
		 */

		__asm__ __volatile__ (
<<<<<<< HEAD
		"	.set	mips2					\n"
		"1:	ll	%0, %1		# atomic_scrub		\n"
		"	addu	%0, $0					\n"
		"	sc	%0, %1					\n"
		"	beqz	%0, 1b					\n"
		"	.set	mips0					\n"
		: "=&r" (temp), "=m" (*virt_addr)
		: "m" (*virt_addr));
=======
		"	.set	push					\n"
		"	.set	mips2					\n"
		"1:	ll	%0, %1		# edac_atomic_scrub	\n"
		"	addu	%0, $0					\n"
		"	sc	%0, %1					\n"
		"	beqz	%0, 1b					\n"
		"	.set	pop					\n"
		: "=&r" (temp), "=" GCC_OFF_SMALL_ASM() (*virt_addr)
		: GCC_OFF_SMALL_ASM() (*virt_addr));
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

		virt_addr++;
	}
}

#endif
