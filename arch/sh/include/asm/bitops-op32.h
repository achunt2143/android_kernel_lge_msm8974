<<<<<<< HEAD
#ifndef __ASM_SH_BITOPS_OP32_H
#define __ASM_SH_BITOPS_OP32_H

=======
/* SPDX-License-Identifier: GPL-2.0 */
#ifndef __ASM_SH_BITOPS_OP32_H
#define __ASM_SH_BITOPS_OP32_H

#include <linux/bits.h>

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
/*
 * The bit modifying instructions on SH-2A are only capable of working
 * with a 3-bit immediate, which signifies the shift position for the bit
 * being worked on.
 */
#if defined(__BIG_ENDIAN)
#define BITOP_LE_SWIZZLE	((BITS_PER_LONG-1) & ~0x7)
#define BYTE_NUMBER(nr)		((nr ^ BITOP_LE_SWIZZLE) / BITS_PER_BYTE)
#define BYTE_OFFSET(nr)		((nr ^ BITOP_LE_SWIZZLE) % BITS_PER_BYTE)
#else
#define BYTE_NUMBER(nr)		((nr) / BITS_PER_BYTE)
#define BYTE_OFFSET(nr)		((nr) % BITS_PER_BYTE)
#endif

<<<<<<< HEAD
#define IS_IMMEDIATE(nr)	(__builtin_constant_p(nr))

static inline void __set_bit(int nr, volatile unsigned long *addr)
{
	if (IS_IMMEDIATE(nr)) {
=======
static __always_inline void
arch___set_bit(unsigned long nr, volatile unsigned long *addr)
{
	if (__builtin_constant_p(nr)) {
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		__asm__ __volatile__ (
			"bset.b %1, @(%O2,%0)		! __set_bit\n\t"
			: "+r" (addr)
			: "i" (BYTE_OFFSET(nr)), "i" (BYTE_NUMBER(nr))
			: "t", "memory"
		);
	} else {
		unsigned long mask = BIT_MASK(nr);
		unsigned long *p = ((unsigned long *)addr) + BIT_WORD(nr);

		*p |= mask;
	}
}

<<<<<<< HEAD
static inline void __clear_bit(int nr, volatile unsigned long *addr)
{
	if (IS_IMMEDIATE(nr)) {
=======
static __always_inline void
arch___clear_bit(unsigned long nr, volatile unsigned long *addr)
{
	if (__builtin_constant_p(nr)) {
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		__asm__ __volatile__ (
			"bclr.b %1, @(%O2,%0)		! __clear_bit\n\t"
			: "+r" (addr)
			: "i" (BYTE_OFFSET(nr)),
			  "i" (BYTE_NUMBER(nr))
			: "t", "memory"
		);
	} else {
		unsigned long mask = BIT_MASK(nr);
		unsigned long *p = ((unsigned long *)addr) + BIT_WORD(nr);

		*p &= ~mask;
	}
}

/**
<<<<<<< HEAD
 * __change_bit - Toggle a bit in memory
=======
 * arch___change_bit - Toggle a bit in memory
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 * @nr: the bit to change
 * @addr: the address to start counting from
 *
 * Unlike change_bit(), this function is non-atomic and may be reordered.
 * If it's called on the same region of memory simultaneously, the effect
 * may be that only one operation succeeds.
 */
<<<<<<< HEAD
static inline void __change_bit(int nr, volatile unsigned long *addr)
{
	if (IS_IMMEDIATE(nr)) {
=======
static __always_inline void
arch___change_bit(unsigned long nr, volatile unsigned long *addr)
{
	if (__builtin_constant_p(nr)) {
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		__asm__ __volatile__ (
			"bxor.b %1, @(%O2,%0)		! __change_bit\n\t"
			: "+r" (addr)
			: "i" (BYTE_OFFSET(nr)),
			  "i" (BYTE_NUMBER(nr))
			: "t", "memory"
		);
	} else {
		unsigned long mask = BIT_MASK(nr);
		unsigned long *p = ((unsigned long *)addr) + BIT_WORD(nr);

		*p ^= mask;
	}
}

/**
<<<<<<< HEAD
 * __test_and_set_bit - Set a bit and return its old value
=======
 * arch___test_and_set_bit - Set a bit and return its old value
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 * @nr: Bit to set
 * @addr: Address to count from
 *
 * This operation is non-atomic and can be reordered.
 * If two examples of this operation race, one can appear to succeed
 * but actually fail.  You must protect multiple accesses with a lock.
 */
<<<<<<< HEAD
static inline int __test_and_set_bit(int nr, volatile unsigned long *addr)
=======
static __always_inline bool
arch___test_and_set_bit(unsigned long nr, volatile unsigned long *addr)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
{
	unsigned long mask = BIT_MASK(nr);
	unsigned long *p = ((unsigned long *)addr) + BIT_WORD(nr);
	unsigned long old = *p;

	*p = old | mask;
	return (old & mask) != 0;
}

/**
<<<<<<< HEAD
 * __test_and_clear_bit - Clear a bit and return its old value
=======
 * arch___test_and_clear_bit - Clear a bit and return its old value
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 * @nr: Bit to clear
 * @addr: Address to count from
 *
 * This operation is non-atomic and can be reordered.
 * If two examples of this operation race, one can appear to succeed
 * but actually fail.  You must protect multiple accesses with a lock.
 */
<<<<<<< HEAD
static inline int __test_and_clear_bit(int nr, volatile unsigned long *addr)
=======
static __always_inline bool
arch___test_and_clear_bit(unsigned long nr, volatile unsigned long *addr)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
{
	unsigned long mask = BIT_MASK(nr);
	unsigned long *p = ((unsigned long *)addr) + BIT_WORD(nr);
	unsigned long old = *p;

	*p = old & ~mask;
	return (old & mask) != 0;
}

/* WARNING: non atomic and it can be reordered! */
<<<<<<< HEAD
static inline int __test_and_change_bit(int nr,
					    volatile unsigned long *addr)
=======
static __always_inline bool
arch___test_and_change_bit(unsigned long nr, volatile unsigned long *addr)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
{
	unsigned long mask = BIT_MASK(nr);
	unsigned long *p = ((unsigned long *)addr) + BIT_WORD(nr);
	unsigned long old = *p;

	*p = old ^ mask;
	return (old & mask) != 0;
}

<<<<<<< HEAD
/**
 * test_bit - Determine whether a bit is set
 * @nr: bit number to test
 * @addr: Address to start counting from
 */
static inline int test_bit(int nr, const volatile unsigned long *addr)
{
	return 1UL & (addr[BIT_WORD(nr)] >> (nr & (BITS_PER_LONG-1)));
}
=======
#define arch_test_bit generic_test_bit
#define arch_test_bit_acquire generic_test_bit_acquire

#include <asm-generic/bitops/non-instrumented-non-atomic.h>
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

#endif /* __ASM_SH_BITOPS_OP32_H */
