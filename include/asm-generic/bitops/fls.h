<<<<<<< HEAD
=======
/* SPDX-License-Identifier: GPL-2.0 */
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#ifndef _ASM_GENERIC_BITOPS_FLS_H_
#define _ASM_GENERIC_BITOPS_FLS_H_

/**
<<<<<<< HEAD
 * fls - find last (most-significant) bit set
=======
 * generic_fls - find last (most-significant) bit set
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 * @x: the word to search
 *
 * This is defined the same way as ffs.
 * Note fls(0) = 0, fls(1) = 1, fls(0x80000000) = 32.
 */

<<<<<<< HEAD
static __always_inline int fls(int x)
=======
static __always_inline int generic_fls(unsigned int x)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
{
	int r = 32;

	if (!x)
		return 0;
	if (!(x & 0xffff0000u)) {
		x <<= 16;
		r -= 16;
	}
	if (!(x & 0xff000000u)) {
		x <<= 8;
		r -= 8;
	}
	if (!(x & 0xf0000000u)) {
		x <<= 4;
		r -= 4;
	}
	if (!(x & 0xc0000000u)) {
		x <<= 2;
		r -= 2;
	}
	if (!(x & 0x80000000u)) {
		x <<= 1;
		r -= 1;
	}
	return r;
}

<<<<<<< HEAD
=======
#ifndef __HAVE_ARCH_FLS
#define fls(x) generic_fls(x)
#endif

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#endif /* _ASM_GENERIC_BITOPS_FLS_H_ */
