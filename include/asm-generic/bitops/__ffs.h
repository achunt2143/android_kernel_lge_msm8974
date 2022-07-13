<<<<<<< HEAD
=======
/* SPDX-License-Identifier: GPL-2.0 */
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#ifndef _ASM_GENERIC_BITOPS___FFS_H_
#define _ASM_GENERIC_BITOPS___FFS_H_

#include <asm/types.h>

/**
<<<<<<< HEAD
 * __ffs - find first bit in word.
=======
 * generic___ffs - find first bit in word.
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 * @word: The word to search
 *
 * Undefined if no bit exists, so code should check against 0 first.
 */
<<<<<<< HEAD
static __always_inline unsigned long __ffs(unsigned long word)
=======
static __always_inline unsigned long generic___ffs(unsigned long word)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
{
	int num = 0;

#if BITS_PER_LONG == 64
	if ((word & 0xffffffff) == 0) {
		num += 32;
		word >>= 32;
	}
#endif
	if ((word & 0xffff) == 0) {
		num += 16;
		word >>= 16;
	}
	if ((word & 0xff) == 0) {
		num += 8;
		word >>= 8;
	}
	if ((word & 0xf) == 0) {
		num += 4;
		word >>= 4;
	}
	if ((word & 0x3) == 0) {
		num += 2;
		word >>= 2;
	}
	if ((word & 0x1) == 0)
		num += 1;
	return num;
}

<<<<<<< HEAD
=======
#ifndef __HAVE_ARCH___FFS
#define __ffs(word) generic___ffs(word)
#endif

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#endif /* _ASM_GENERIC_BITOPS___FFS_H_ */
