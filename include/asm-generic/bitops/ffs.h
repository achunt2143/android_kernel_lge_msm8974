<<<<<<< HEAD
=======
/* SPDX-License-Identifier: GPL-2.0 */
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#ifndef _ASM_GENERIC_BITOPS_FFS_H_
#define _ASM_GENERIC_BITOPS_FFS_H_

/**
<<<<<<< HEAD
 * ffs - find first bit set
=======
 * generic_ffs - find first bit set
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 * @x: the word to search
 *
 * This is defined the same way as
 * the libc and compiler builtin ffs routines, therefore
<<<<<<< HEAD
 * differs in spirit from the above ffz (man ffs).
 */
static inline int ffs(int x)
=======
 * differs in spirit from ffz (man ffs).
 */
static inline int generic_ffs(int x)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
{
	int r = 1;

	if (!x)
		return 0;
	if (!(x & 0xffff)) {
		x >>= 16;
		r += 16;
	}
	if (!(x & 0xff)) {
		x >>= 8;
		r += 8;
	}
	if (!(x & 0xf)) {
		x >>= 4;
		r += 4;
	}
	if (!(x & 3)) {
		x >>= 2;
		r += 2;
	}
	if (!(x & 1)) {
		x >>= 1;
		r += 1;
	}
	return r;
}

<<<<<<< HEAD
=======
#ifndef __HAVE_ARCH_FFS
#define ffs(x) generic_ffs(x)
#endif

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#endif /* _ASM_GENERIC_BITOPS_FFS_H_ */
