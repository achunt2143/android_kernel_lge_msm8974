<<<<<<< HEAD
=======
/* SPDX-License-Identifier: GPL-2.0 */
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#ifndef _ASM_X86_UACCESS_64_H
#define _ASM_X86_UACCESS_64_H

/*
 * User space memory access functions
 */
#include <linux/compiler.h>
<<<<<<< HEAD
#include <linux/errno.h>
#include <linux/lockdep.h>
#include <asm/alternative.h>
#include <asm/cpufeature.h>
#include <asm/page.h>
=======
#include <linux/lockdep.h>
#include <linux/kasan-checks.h>
#include <asm/alternative.h>
#include <asm/cpufeatures.h>
#include <asm/page.h>
#include <asm/percpu.h>

#ifdef CONFIG_ADDRESS_MASKING
/*
 * Mask out tag bits from the address.
 */
static inline unsigned long __untagged_addr(unsigned long addr)
{
	asm (ALTERNATIVE("",
			 "and " __percpu_arg([mask]) ", %[addr]", X86_FEATURE_LAM)
	     : [addr] "+r" (addr)
	     : [mask] "m" (__my_cpu_var(tlbstate_untag_mask)));

	return addr;
}

#define untagged_addr(addr)	({					\
	unsigned long __addr = (__force unsigned long)(addr);		\
	(__force __typeof__(addr))__untagged_addr(__addr);		\
})

static inline unsigned long __untagged_addr_remote(struct mm_struct *mm,
						   unsigned long addr)
{
	mmap_assert_locked(mm);
	return addr & (mm)->context.untag_mask;
}

#define untagged_addr_remote(mm, addr)	({				\
	unsigned long __addr = (__force unsigned long)(addr);		\
	(__force __typeof__(addr))__untagged_addr_remote(mm, __addr);	\
})

#endif

/*
 * The virtual address space space is logically divided into a kernel
 * half and a user half.  When cast to a signed type, user pointers
 * are positive and kernel pointers are negative.
 */
#define valid_user_address(x) ((__force long)(x) >= 0)

/*
 * User pointers can have tag bits on x86-64.  This scheme tolerates
 * arbitrary values in those bits rather then masking them off.
 *
 * Enforce two rules:
 * 1. 'ptr' must be in the user half of the address space
 * 2. 'ptr+size' must not overflow into kernel addresses
 *
 * Note that addresses around the sign change are not valid addresses,
 * and will GP-fault even with LAM enabled if the sign bit is set (see
 * "CR3.LAM_SUP" that can narrow the canonicality check if we ever
 * enable it, but not remove it entirely).
 *
 * So the "overflow into kernel addresses" does not imply some sudden
 * exact boundary at the sign bit, and we can allow a lot of slop on the
 * size check.
 *
 * In fact, we could probably remove the size check entirely, since
 * any kernel accesses will be in increasing address order starting
 * at 'ptr', and even if the end might be in kernel space, we'll
 * hit the GP faults for non-canonical accesses before we ever get
 * there.
 *
 * That's a separate optimization, for now just handle the small
 * constant case.
 */
static inline bool __access_ok(const void __user *ptr, unsigned long size)
{
	if (__builtin_constant_p(size <= PAGE_SIZE) && size <= PAGE_SIZE) {
		return valid_user_address(ptr);
	} else {
		unsigned long sum = size + (__force unsigned long)ptr;

		return valid_user_address(sum) && sum >= (__force unsigned long)ptr;
	}
}
#define __access_ok __access_ok
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

/*
 * Copy To/From Userspace
 */

/* Handles exceptions in both to and from, but doesn't do access_ok */
__must_check unsigned long
<<<<<<< HEAD
copy_user_generic_string(void *to, const void *from, unsigned len);
__must_check unsigned long
copy_user_generic_unrolled(void *to, const void *from, unsigned len);

static __always_inline __must_check unsigned long
copy_user_generic(void *to, const void *from, unsigned len)
{
	unsigned ret;

	alternative_call(copy_user_generic_unrolled,
			 copy_user_generic_string,
			 X86_FEATURE_REP_GOOD,
			 ASM_OUTPUT2("=a" (ret), "=D" (to), "=S" (from),
				     "=d" (len)),
			 "1" (to), "2" (from), "3" (len)
			 : "memory", "rcx", "r8", "r9", "r10", "r11");
	return ret;
}

__must_check unsigned long
_copy_to_user(void __user *to, const void *from, unsigned len);
__must_check unsigned long
_copy_from_user(void *to, const void __user *from, unsigned len);
__must_check unsigned long
copy_in_user(void __user *to, const void __user *from, unsigned len);

static inline unsigned long __must_check copy_from_user(void *to,
					  const void __user *from,
					  unsigned long n)
{
	int sz = __compiletime_object_size(to);

	might_fault();
	if (likely(sz == -1 || sz >= n))
		n = _copy_from_user(to, from, n);
#ifdef CONFIG_DEBUG_VM
	else
		WARN(1, "Buffer overflow detected!\n");
#endif
	return n;
}

static __always_inline __must_check
int copy_to_user(void __user *dst, const void *src, unsigned size)
{
	might_fault();

	return _copy_to_user(dst, src, size);
}

static __always_inline __must_check
int __copy_from_user(void *dst, const void __user *src, unsigned size)
{
	int ret = 0;

	might_fault();
	if (!__builtin_constant_p(size))
		return copy_user_generic(dst, (__force void *)src, size);
	switch (size) {
	case 1:__get_user_asm(*(u8 *)dst, (u8 __user *)src,
			      ret, "b", "b", "=q", 1);
		return ret;
	case 2:__get_user_asm(*(u16 *)dst, (u16 __user *)src,
			      ret, "w", "w", "=r", 2);
		return ret;
	case 4:__get_user_asm(*(u32 *)dst, (u32 __user *)src,
			      ret, "l", "k", "=r", 4);
		return ret;
	case 8:__get_user_asm(*(u64 *)dst, (u64 __user *)src,
			      ret, "q", "", "=r", 8);
		return ret;
	case 10:
		__get_user_asm(*(u64 *)dst, (u64 __user *)src,
			       ret, "q", "", "=r", 10);
		if (unlikely(ret))
			return ret;
		__get_user_asm(*(u16 *)(8 + (char *)dst),
			       (u16 __user *)(8 + (char __user *)src),
			       ret, "w", "w", "=r", 2);
		return ret;
	case 16:
		__get_user_asm(*(u64 *)dst, (u64 __user *)src,
			       ret, "q", "", "=r", 16);
		if (unlikely(ret))
			return ret;
		__get_user_asm(*(u64 *)(8 + (char *)dst),
			       (u64 __user *)(8 + (char __user *)src),
			       ret, "q", "", "=r", 8);
		return ret;
	default:
		return copy_user_generic(dst, (__force void *)src, size);
	}
}

static __always_inline __must_check
int __copy_to_user(void __user *dst, const void *src, unsigned size)
{
	int ret = 0;

	might_fault();
	if (!__builtin_constant_p(size))
		return copy_user_generic((__force void *)dst, src, size);
	switch (size) {
	case 1:__put_user_asm(*(u8 *)src, (u8 __user *)dst,
			      ret, "b", "b", "iq", 1);
		return ret;
	case 2:__put_user_asm(*(u16 *)src, (u16 __user *)dst,
			      ret, "w", "w", "ir", 2);
		return ret;
	case 4:__put_user_asm(*(u32 *)src, (u32 __user *)dst,
			      ret, "l", "k", "ir", 4);
		return ret;
	case 8:__put_user_asm(*(u64 *)src, (u64 __user *)dst,
			      ret, "q", "", "er", 8);
		return ret;
	case 10:
		__put_user_asm(*(u64 *)src, (u64 __user *)dst,
			       ret, "q", "", "er", 10);
		if (unlikely(ret))
			return ret;
		asm("":::"memory");
		__put_user_asm(4[(u16 *)src], 4 + (u16 __user *)dst,
			       ret, "w", "w", "ir", 2);
		return ret;
	case 16:
		__put_user_asm(*(u64 *)src, (u64 __user *)dst,
			       ret, "q", "", "er", 16);
		if (unlikely(ret))
			return ret;
		asm("":::"memory");
		__put_user_asm(1[(u64 *)src], 1 + (u64 __user *)dst,
			       ret, "q", "", "er", 8);
		return ret;
	default:
		return copy_user_generic((__force void *)dst, src, size);
	}
}

static __always_inline __must_check
int __copy_in_user(void __user *dst, const void __user *src, unsigned size)
{
	int ret = 0;

	might_fault();
	if (!__builtin_constant_p(size))
		return copy_user_generic((__force void *)dst,
					 (__force void *)src, size);
	switch (size) {
	case 1: {
		u8 tmp;
		__get_user_asm(tmp, (u8 __user *)src,
			       ret, "b", "b", "=q", 1);
		if (likely(!ret))
			__put_user_asm(tmp, (u8 __user *)dst,
				       ret, "b", "b", "iq", 1);
		return ret;
	}
	case 2: {
		u16 tmp;
		__get_user_asm(tmp, (u16 __user *)src,
			       ret, "w", "w", "=r", 2);
		if (likely(!ret))
			__put_user_asm(tmp, (u16 __user *)dst,
				       ret, "w", "w", "ir", 2);
		return ret;
	}

	case 4: {
		u32 tmp;
		__get_user_asm(tmp, (u32 __user *)src,
			       ret, "l", "k", "=r", 4);
		if (likely(!ret))
			__put_user_asm(tmp, (u32 __user *)dst,
				       ret, "l", "k", "ir", 4);
		return ret;
	}
	case 8: {
		u64 tmp;
		__get_user_asm(tmp, (u64 __user *)src,
			       ret, "q", "", "=r", 8);
		if (likely(!ret))
			__put_user_asm(tmp, (u64 __user *)dst,
				       ret, "q", "", "er", 8);
		return ret;
	}
	default:
		return copy_user_generic((__force void *)dst,
					 (__force void *)src, size);
	}
}

__must_check long strnlen_user(const char __user *str, long n);
__must_check long __strnlen_user(const char __user *str, long n);
__must_check long strlen_user(const char __user *str);
__must_check unsigned long clear_user(void __user *mem, unsigned long len);
__must_check unsigned long __clear_user(void __user *mem, unsigned long len);

static __must_check __always_inline int
__copy_from_user_inatomic(void *dst, const void __user *src, unsigned size)
{
	return copy_user_generic(dst, (__force const void *)src, size);
}

static __must_check __always_inline int
__copy_to_user_inatomic(void __user *dst, const void *src, unsigned size)
=======
rep_movs_alternative(void *to, const void *from, unsigned len);

static __always_inline __must_check unsigned long
copy_user_generic(void *to, const void *from, unsigned long len)
{
	stac();
	/*
	 * If CPU has FSRM feature, use 'rep movs'.
	 * Otherwise, use rep_movs_alternative.
	 */
	asm volatile(
		"1:\n\t"
		ALTERNATIVE("rep movsb",
			    "call rep_movs_alternative", ALT_NOT(X86_FEATURE_FSRM))
		"2:\n"
		_ASM_EXTABLE_UA(1b, 2b)
		:"+c" (len), "+D" (to), "+S" (from), ASM_CALL_CONSTRAINT
		: : "memory", "rax");
	clac();
	return len;
}

static __always_inline __must_check unsigned long
raw_copy_from_user(void *dst, const void __user *src, unsigned long size)
{
	return copy_user_generic(dst, (__force void *)src, size);
}

static __always_inline __must_check unsigned long
raw_copy_to_user(void __user *dst, const void *src, unsigned long size)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
{
	return copy_user_generic((__force void *)dst, src, size);
}

<<<<<<< HEAD
extern long __copy_user_nocache(void *dst, const void __user *src,
				unsigned size, int zerorest);

static inline int
__copy_from_user_nocache(void *dst, const void __user *src, unsigned size)
{
	might_sleep();
	return __copy_user_nocache(dst, src, size, 1);
}
=======
extern long __copy_user_nocache(void *dst, const void __user *src, unsigned size);
extern long __copy_user_flushcache(void *dst, const void __user *src, unsigned size);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

static inline int
__copy_from_user_inatomic_nocache(void *dst, const void __user *src,
				  unsigned size)
{
<<<<<<< HEAD
	return __copy_user_nocache(dst, src, size, 0);
}

unsigned long
copy_user_handle_tail(char *to, char *from, unsigned len, unsigned zerorest);

=======
	long ret;
	kasan_check_write(dst, size);
	stac();
	ret = __copy_user_nocache(dst, src, size);
	clac();
	return ret;
}

static inline int
__copy_from_user_flushcache(void *dst, const void __user *src, unsigned size)
{
	kasan_check_write(dst, size);
	return __copy_user_flushcache(dst, src, size);
}

/*
 * Zero Userspace.
 */

__must_check unsigned long
rep_stos_alternative(void __user *addr, unsigned long len);

static __always_inline __must_check unsigned long __clear_user(void __user *addr, unsigned long size)
{
	might_fault();
	stac();

	/*
	 * No memory constraint because it doesn't change any memory gcc
	 * knows about.
	 */
	asm volatile(
		"1:\n\t"
		ALTERNATIVE("rep stosb",
			    "call rep_stos_alternative", ALT_NOT(X86_FEATURE_FSRS))
		"2:\n"
	       _ASM_EXTABLE_UA(1b, 2b)
	       : "+c" (size), "+D" (addr), ASM_CALL_CONSTRAINT
	       : "a" (0));

	clac();

	return size;
}

static __always_inline unsigned long clear_user(void __user *to, unsigned long n)
{
	if (__access_ok(to, n))
		return __clear_user(to, n);
	return n;
}
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#endif /* _ASM_X86_UACCESS_64_H */
