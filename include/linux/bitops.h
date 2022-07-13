<<<<<<< HEAD
#ifndef _LINUX_BITOPS_H
#define _LINUX_BITOPS_H
#include <asm/types.h>

#ifdef	__KERNEL__
#define BIT(nr)			(1UL << (nr))
#define BIT_MASK(nr)		(1UL << ((nr) % BITS_PER_LONG))
#define BIT_WORD(nr)		((nr) / BITS_PER_LONG)
#define BITS_PER_BYTE		8
#define BITS_TO_LONGS(nr)	DIV_ROUND_UP(nr, BITS_PER_BYTE * sizeof(long))
#endif

=======
/* SPDX-License-Identifier: GPL-2.0 */
#ifndef _LINUX_BITOPS_H
#define _LINUX_BITOPS_H

#include <asm/types.h>
#include <linux/bits.h>
#include <linux/typecheck.h>

#include <uapi/linux/kernel.h>

/* Set bits in the first 'n' bytes when loaded from memory */
#ifdef __LITTLE_ENDIAN
#  define aligned_byte_mask(n) ((1UL << 8*(n))-1)
#else
#  define aligned_byte_mask(n) (~0xffUL << (BITS_PER_LONG - 8 - 8*(n)))
#endif

#define BITS_PER_TYPE(type)	(sizeof(type) * BITS_PER_BYTE)
#define BITS_TO_LONGS(nr)	__KERNEL_DIV_ROUND_UP(nr, BITS_PER_TYPE(long))
#define BITS_TO_U64(nr)		__KERNEL_DIV_ROUND_UP(nr, BITS_PER_TYPE(u64))
#define BITS_TO_U32(nr)		__KERNEL_DIV_ROUND_UP(nr, BITS_PER_TYPE(u32))
#define BITS_TO_BYTES(nr)	__KERNEL_DIV_ROUND_UP(nr, BITS_PER_TYPE(char))

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
extern unsigned int __sw_hweight8(unsigned int w);
extern unsigned int __sw_hweight16(unsigned int w);
extern unsigned int __sw_hweight32(unsigned int w);
extern unsigned long __sw_hweight64(__u64 w);

/*
<<<<<<< HEAD
=======
 * Defined here because those may be needed by architecture-specific static
 * inlines.
 */

#include <asm-generic/bitops/generic-non-atomic.h>

/*
 * Many architecture-specific non-atomic bitops contain inline asm code and due
 * to that the compiler can't optimize them to compile-time expressions or
 * constants. In contrary, generic_*() helpers are defined in pure C and
 * compilers optimize them just well.
 * Therefore, to make `unsigned long foo = 0; __set_bit(BAR, &foo)` effectively
 * equal to `unsigned long foo = BIT(BAR)`, pick the generic C alternative when
 * the arguments can be resolved at compile time. That expression itself is a
 * constant and doesn't bring any functional changes to the rest of cases.
 * The casts to `uintptr_t` are needed to mitigate `-Waddress` warnings when
 * passing a bitmap from .bss or .data (-> `!!addr` is always true).
 */
#define bitop(op, nr, addr)						\
	((__builtin_constant_p(nr) &&					\
	  __builtin_constant_p((uintptr_t)(addr) != (uintptr_t)NULL) &&	\
	  (uintptr_t)(addr) != (uintptr_t)NULL &&			\
	  __builtin_constant_p(*(const unsigned long *)(addr))) ?	\
	 const##op(nr, addr) : op(nr, addr))

#define __set_bit(nr, addr)		bitop(___set_bit, nr, addr)
#define __clear_bit(nr, addr)		bitop(___clear_bit, nr, addr)
#define __change_bit(nr, addr)		bitop(___change_bit, nr, addr)
#define __test_and_set_bit(nr, addr)	bitop(___test_and_set_bit, nr, addr)
#define __test_and_clear_bit(nr, addr)	bitop(___test_and_clear_bit, nr, addr)
#define __test_and_change_bit(nr, addr)	bitop(___test_and_change_bit, nr, addr)
#define test_bit(nr, addr)		bitop(_test_bit, nr, addr)
#define test_bit_acquire(nr, addr)	bitop(_test_bit_acquire, nr, addr)

/*
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 * Include this here because some architectures need generic_ffs/fls in
 * scope
 */
#include <asm/bitops.h>

<<<<<<< HEAD
#define for_each_set_bit(bit, addr, size) \
	for ((bit) = find_first_bit((addr), (size));		\
	     (bit) < (size);					\
	     (bit) = find_next_bit((addr), (size), (bit) + 1))

/* same as for_each_set_bit() but use bit as value to start with */
#define for_each_set_bit_from(bit, addr, size) \
	for ((bit) = find_next_bit((addr), (size), (bit));	\
	     (bit) < (size);					\
	     (bit) = find_next_bit((addr), (size), (bit) + 1))

#define for_each_clear_bit(bit, addr, size) \
	for ((bit) = find_first_zero_bit((addr), (size));	\
	     (bit) < (size);					\
	     (bit) = find_next_zero_bit((addr), (size), (bit) + 1))

/* same as for_each_clear_bit() but use bit as value to start with */
#define for_each_clear_bit_from(bit, addr, size) \
	for ((bit) = find_next_zero_bit((addr), (size), (bit));	\
	     (bit) < (size);					\
	     (bit) = find_next_zero_bit((addr), (size), (bit) + 1))

static __inline__ int get_bitmask_order(unsigned int count)
=======
/* Check that the bitops prototypes are sane */
#define __check_bitop_pr(name)						\
	static_assert(__same_type(arch_##name, generic_##name) &&	\
		      __same_type(const_##name, generic_##name) &&	\
		      __same_type(_##name, generic_##name))

__check_bitop_pr(__set_bit);
__check_bitop_pr(__clear_bit);
__check_bitop_pr(__change_bit);
__check_bitop_pr(__test_and_set_bit);
__check_bitop_pr(__test_and_clear_bit);
__check_bitop_pr(__test_and_change_bit);
__check_bitop_pr(test_bit);

#undef __check_bitop_pr

static inline int get_bitmask_order(unsigned int count)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
{
	int order;

	order = fls(count);
	return order;	/* We could be slightly more clever with -1 here... */
}

<<<<<<< HEAD
static __inline__ int get_count_order(unsigned int count)
{
	int order;

	order = fls(count) - 1;
	if (count & (count - 1))
		order++;
	return order;
}

static inline unsigned long hweight_long(unsigned long w)
{
	return sizeof(w) == 4 ? hweight32(w) : hweight64(w);
=======
static __always_inline unsigned long hweight_long(unsigned long w)
{
	return sizeof(w) == 4 ? hweight32(w) : hweight64((__u64)w);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

/**
 * rol64 - rotate a 64-bit value left
 * @word: value to rotate
 * @shift: bits to roll
 */
static inline __u64 rol64(__u64 word, unsigned int shift)
{
<<<<<<< HEAD
	return (word << shift) | (word >> (64 - shift));
=======
	return (word << (shift & 63)) | (word >> ((-shift) & 63));
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

/**
 * ror64 - rotate a 64-bit value right
 * @word: value to rotate
 * @shift: bits to roll
 */
static inline __u64 ror64(__u64 word, unsigned int shift)
{
<<<<<<< HEAD
	return (word >> shift) | (word << (64 - shift));
=======
	return (word >> (shift & 63)) | (word << ((-shift) & 63));
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

/**
 * rol32 - rotate a 32-bit value left
 * @word: value to rotate
 * @shift: bits to roll
 */
static inline __u32 rol32(__u32 word, unsigned int shift)
{
<<<<<<< HEAD
	return (word << shift) | (word >> (32 - shift));
=======
	return (word << (shift & 31)) | (word >> ((-shift) & 31));
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

/**
 * ror32 - rotate a 32-bit value right
 * @word: value to rotate
 * @shift: bits to roll
 */
static inline __u32 ror32(__u32 word, unsigned int shift)
{
<<<<<<< HEAD
	return (word >> shift) | (word << (32 - shift));
=======
	return (word >> (shift & 31)) | (word << ((-shift) & 31));
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

/**
 * rol16 - rotate a 16-bit value left
 * @word: value to rotate
 * @shift: bits to roll
 */
static inline __u16 rol16(__u16 word, unsigned int shift)
{
<<<<<<< HEAD
	return (word << shift) | (word >> (16 - shift));
=======
	return (word << (shift & 15)) | (word >> ((-shift) & 15));
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

/**
 * ror16 - rotate a 16-bit value right
 * @word: value to rotate
 * @shift: bits to roll
 */
static inline __u16 ror16(__u16 word, unsigned int shift)
{
<<<<<<< HEAD
	return (word >> shift) | (word << (16 - shift));
=======
	return (word >> (shift & 15)) | (word << ((-shift) & 15));
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

/**
 * rol8 - rotate an 8-bit value left
 * @word: value to rotate
 * @shift: bits to roll
 */
static inline __u8 rol8(__u8 word, unsigned int shift)
{
<<<<<<< HEAD
	return (word << shift) | (word >> (8 - shift));
=======
	return (word << (shift & 7)) | (word >> ((-shift) & 7));
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

/**
 * ror8 - rotate an 8-bit value right
 * @word: value to rotate
 * @shift: bits to roll
 */
static inline __u8 ror8(__u8 word, unsigned int shift)
{
<<<<<<< HEAD
	return (word >> shift) | (word << (8 - shift));
=======
	return (word >> (shift & 7)) | (word << ((-shift) & 7));
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

/**
 * sign_extend32 - sign extend a 32-bit value using specified bit as sign-bit
 * @value: value to sign extend
 * @index: 0 based bit index (0<=index<32) to sign bit
<<<<<<< HEAD
 */
static inline __s32 sign_extend32(__u32 value, int index)
=======
 *
 * This is safe to use for 16- and 8-bit types as well.
 */
static __always_inline __s32 sign_extend32(__u32 value, int index)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
{
	__u8 shift = 31 - index;
	return (__s32)(value << shift) >> shift;
}

<<<<<<< HEAD
=======
/**
 * sign_extend64 - sign extend a 64-bit value using specified bit as sign-bit
 * @value: value to sign extend
 * @index: 0 based bit index (0<=index<64) to sign bit
 */
static __always_inline __s64 sign_extend64(__u64 value, int index)
{
	__u8 shift = 63 - index;
	return (__s64)(value << shift) >> shift;
}

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
static inline unsigned fls_long(unsigned long l)
{
	if (sizeof(l) == 4)
		return fls(l);
	return fls64(l);
}

<<<<<<< HEAD
=======
static inline int get_count_order(unsigned int count)
{
	if (count == 0)
		return -1;

	return fls(--count);
}

/**
 * get_count_order_long - get order after rounding @l up to power of 2
 * @l: parameter
 *
 * it is same as get_count_order() but with long type parameter
 */
static inline int get_count_order_long(unsigned long l)
{
	if (l == 0UL)
		return -1;
	return (int)fls_long(--l);
}

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
/**
 * __ffs64 - find first set bit in a 64 bit word
 * @word: The 64 bit word
 *
<<<<<<< HEAD
 * On 64 bit arches this is a synomyn for __ffs
=======
 * On 64 bit arches this is a synonym for __ffs
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 * The result is not defined if no bits are set, so check that @word
 * is non-zero before calling this.
 */
static inline unsigned long __ffs64(u64 word)
{
#if BITS_PER_LONG == 32
	if (((u32)word) == 0UL)
		return __ffs((u32)(word >> 32)) + 32;
#elif BITS_PER_LONG != 64
#error BITS_PER_LONG not 32 or 64
#endif
	return __ffs((unsigned long)word);
}

<<<<<<< HEAD
#ifdef __KERNEL__

#ifndef set_mask_bits
#define set_mask_bits(ptr, _mask, _bits)	\
({								\
	const typeof(*ptr) mask = (_mask), bits = (_bits);	\
	typeof(*ptr) old, new;					\
								\
	do {							\
		old = ACCESS_ONCE(*ptr);			\
		new = (old & ~mask) | bits;			\
	} while (cmpxchg(ptr, old, new) != old);		\
								\
	new;							\
})
#endif

#ifndef find_last_bit
/**
 * find_last_bit - find the last set bit in a memory region
 * @addr: The address to start the search at
 * @size: The maximum size to search
 *
 * Returns the bit number of the first set bit, or size.
 */
extern unsigned long find_last_bit(const unsigned long *addr,
				   unsigned long size);
=======
/**
 * fns - find N'th set bit in a word
 * @word: The word to search
 * @n: Bit to find
 */
static inline unsigned long fns(unsigned long word, unsigned int n)
{
	unsigned int bit;

	while (word) {
		bit = __ffs(word);
		if (n-- == 0)
			return bit;
		__clear_bit(bit, &word);
	}

	return BITS_PER_LONG;
}

/**
 * assign_bit - Assign value to a bit in memory
 * @nr: the bit to set
 * @addr: the address to start counting from
 * @value: the value to assign
 */
static __always_inline void assign_bit(long nr, volatile unsigned long *addr,
				       bool value)
{
	if (value)
		set_bit(nr, addr);
	else
		clear_bit(nr, addr);
}

static __always_inline void __assign_bit(long nr, volatile unsigned long *addr,
					 bool value)
{
	if (value)
		__set_bit(nr, addr);
	else
		__clear_bit(nr, addr);
}

/**
 * __ptr_set_bit - Set bit in a pointer's value
 * @nr: the bit to set
 * @addr: the address of the pointer variable
 *
 * Example:
 *	void *p = foo();
 *	__ptr_set_bit(bit, &p);
 */
#define __ptr_set_bit(nr, addr)                         \
	({                                              \
		typecheck_pointer(*(addr));             \
		__set_bit(nr, (unsigned long *)(addr)); \
	})

/**
 * __ptr_clear_bit - Clear bit in a pointer's value
 * @nr: the bit to clear
 * @addr: the address of the pointer variable
 *
 * Example:
 *	void *p = foo();
 *	__ptr_clear_bit(bit, &p);
 */
#define __ptr_clear_bit(nr, addr)                         \
	({                                                \
		typecheck_pointer(*(addr));               \
		__clear_bit(nr, (unsigned long *)(addr)); \
	})

/**
 * __ptr_test_bit - Test bit in a pointer's value
 * @nr: the bit to test
 * @addr: the address of the pointer variable
 *
 * Example:
 *	void *p = foo();
 *	if (__ptr_test_bit(bit, &p)) {
 *	        ...
 *	} else {
 *		...
 *	}
 */
#define __ptr_test_bit(nr, addr)                       \
	({                                             \
		typecheck_pointer(*(addr));            \
		test_bit(nr, (unsigned long *)(addr)); \
	})

#ifdef __KERNEL__

#ifndef set_mask_bits
#define set_mask_bits(ptr, mask, bits)	\
({								\
	const typeof(*(ptr)) mask__ = (mask), bits__ = (bits);	\
	typeof(*(ptr)) old__, new__;				\
								\
	old__ = READ_ONCE(*(ptr));				\
	do {							\
		new__ = (old__ & ~mask__) | bits__;		\
	} while (!try_cmpxchg(ptr, &old__, new__));		\
								\
	old__;							\
})
#endif

#ifndef bit_clear_unless
#define bit_clear_unless(ptr, clear, test)	\
({								\
	const typeof(*(ptr)) clear__ = (clear), test__ = (test);\
	typeof(*(ptr)) old__, new__;				\
								\
	old__ = READ_ONCE(*(ptr));				\
	do {							\
		if (old__ & test__)				\
			break;					\
		new__ = old__ & ~clear__;			\
	} while (!try_cmpxchg(ptr, &old__, new__));		\
								\
	!(old__ & test__);					\
})
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#endif

#endif /* __KERNEL__ */
#endif
