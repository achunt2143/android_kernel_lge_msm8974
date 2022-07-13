<<<<<<< HEAD
=======
/* SPDX-License-Identifier: GPL-2.0 */
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#ifndef _ASM_GENERIC_BITOPS_LE_H_
#define _ASM_GENERIC_BITOPS_LE_H_

#include <asm/types.h>
#include <asm/byteorder.h>

#if defined(__LITTLE_ENDIAN)

#define BITOP_LE_SWIZZLE	0

<<<<<<< HEAD
static inline unsigned long find_next_zero_bit_le(const void *addr,
		unsigned long size, unsigned long offset)
{
	return find_next_zero_bit(addr, size, offset);
}

static inline unsigned long find_next_bit_le(const void *addr,
		unsigned long size, unsigned long offset)
{
	return find_next_bit(addr, size, offset);
}

static inline unsigned long find_first_zero_bit_le(const void *addr,
		unsigned long size)
{
	return find_first_zero_bit(addr, size);
}

=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#elif defined(__BIG_ENDIAN)

#define BITOP_LE_SWIZZLE	((BITS_PER_LONG-1) & ~0x7)

<<<<<<< HEAD
#ifndef find_next_zero_bit_le
extern unsigned long find_next_zero_bit_le(const void *addr,
		unsigned long size, unsigned long offset);
#endif

#ifndef find_next_bit_le
extern unsigned long find_next_bit_le(const void *addr,
		unsigned long size, unsigned long offset);
#endif

#ifndef find_first_zero_bit_le
#define find_first_zero_bit_le(addr, size) \
	find_next_zero_bit_le((addr), (size), 0)
#endif

#else
#error "Please fix <asm/byteorder.h>"
#endif
=======
#endif

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

static inline int test_bit_le(int nr, const void *addr)
{
	return test_bit(nr ^ BITOP_LE_SWIZZLE, addr);
}

<<<<<<< HEAD
=======
static inline void set_bit_le(int nr, void *addr)
{
	set_bit(nr ^ BITOP_LE_SWIZZLE, addr);
}

static inline void clear_bit_le(int nr, void *addr)
{
	clear_bit(nr ^ BITOP_LE_SWIZZLE, addr);
}

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
static inline void __set_bit_le(int nr, void *addr)
{
	__set_bit(nr ^ BITOP_LE_SWIZZLE, addr);
}

static inline void __clear_bit_le(int nr, void *addr)
{
	__clear_bit(nr ^ BITOP_LE_SWIZZLE, addr);
}

static inline int test_and_set_bit_le(int nr, void *addr)
{
	return test_and_set_bit(nr ^ BITOP_LE_SWIZZLE, addr);
}

static inline int test_and_clear_bit_le(int nr, void *addr)
{
	return test_and_clear_bit(nr ^ BITOP_LE_SWIZZLE, addr);
}

static inline int __test_and_set_bit_le(int nr, void *addr)
{
	return __test_and_set_bit(nr ^ BITOP_LE_SWIZZLE, addr);
}

static inline int __test_and_clear_bit_le(int nr, void *addr)
{
	return __test_and_clear_bit(nr ^ BITOP_LE_SWIZZLE, addr);
}

#endif /* _ASM_GENERIC_BITOPS_LE_H_ */
