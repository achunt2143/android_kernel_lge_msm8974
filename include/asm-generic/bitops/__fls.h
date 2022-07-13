<<<<<<< HEAD
=======
/* SPDX-License-Identifier: GPL-2.0 */
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#ifndef _ASM_GENERIC_BITOPS___FLS_H_
#define _ASM_GENERIC_BITOPS___FLS_H_

#include <asm/types.h>

/**
<<<<<<< HEAD
 * __fls - find last (most-significant) set bit in a long word
=======
 * generic___fls - find last (most-significant) set bit in a long word
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 * @word: the word to search
 *
 * Undefined if no set bit exists, so code should check against 0 first.
 */
<<<<<<< HEAD
static __always_inline unsigned long __fls(unsigned long word)
=======
static __always_inline unsigned long generic___fls(unsigned long word)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
{
	int num = BITS_PER_LONG - 1;

#if BITS_PER_LONG == 64
	if (!(word & (~0ul << 32))) {
		num -= 32;
		word <<= 32;
	}
#endif
	if (!(word & (~0ul << (BITS_PER_LONG-16)))) {
		num -= 16;
		word <<= 16;
	}
	if (!(word & (~0ul << (BITS_PER_LONG-8)))) {
		num -= 8;
		word <<= 8;
	}
	if (!(word & (~0ul << (BITS_PER_LONG-4)))) {
		num -= 4;
		word <<= 4;
	}
	if (!(word & (~0ul << (BITS_PER_LONG-2)))) {
		num -= 2;
		word <<= 2;
	}
	if (!(word & (~0ul << (BITS_PER_LONG-1))))
		num -= 1;
	return num;
}

<<<<<<< HEAD
=======
#ifndef __HAVE_ARCH___FLS
#define __fls(word) generic___fls(word)
#endif

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#endif /* _ASM_GENERIC_BITOPS___FLS_H_ */
