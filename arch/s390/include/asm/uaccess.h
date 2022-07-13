<<<<<<< HEAD
/*
 *  include/asm-s390/uaccess.h
 *
 *  S390 version
 *    Copyright (C) 1999,2000 IBM Deutschland Entwicklung GmbH, IBM Corporation
 *    Author(s): Hartmut Penner (hp@de.ibm.com),
 *               Martin Schwidefsky (schwidefsky@de.ibm.com)
=======
/* SPDX-License-Identifier: GPL-2.0 */
/*
 *  S390 version
 *    Copyright IBM Corp. 1999, 2000
 *    Author(s): Hartmut Penner (hp@de.ibm.com),
 *		 Martin Schwidefsky (schwidefsky@de.ibm.com)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 *
 *  Derived from "include/asm-i386/uaccess.h"
 */
#ifndef __S390_UACCESS_H
#define __S390_UACCESS_H

/*
 * User space memory access functions
 */
<<<<<<< HEAD
#include <linux/sched.h>
#include <linux/errno.h>
#include <asm/ctl_reg.h>

#define VERIFY_READ     0
#define VERIFY_WRITE    1


/*
 * The fs value determines whether argument validity checking should be
 * performed or not.  If get_fs() == USER_DS, checking is performed, with
 * get_fs() == KERNEL_DS, checking is bypassed.
 *
 * For historical reasons, these macros are grossly misnamed.
 */

#define MAKE_MM_SEG(a)  ((mm_segment_t) { (a) })


#define KERNEL_DS       MAKE_MM_SEG(0)
#define USER_DS         MAKE_MM_SEG(1)

#define get_ds()        (KERNEL_DS)
#define get_fs()        (current->thread.mm_segment)

#define set_fs(x) \
({									\
	unsigned long __pto;						\
	current->thread.mm_segment = (x);				\
	__pto = current->thread.mm_segment.ar4 ?			\
		S390_lowcore.user_asce : S390_lowcore.kernel_asce;	\
	__ctl_load(__pto, 7, 7);					\
})

#define segment_eq(a,b) ((a).ar4 == (b).ar4)

#define __access_ok(addr, size)	\
({				\
	__chk_user_ptr(addr);	\
	1;			\
})

#define access_ok(type, addr, size) __access_ok(addr, size)

/*
 * The exception table consists of pairs of addresses: the first is the
 * address of an instruction that is allowed to fault, and the second is
 * the address at which the program should continue.  No registers are
 * modified, so it is entirely up to the continuation code to figure out
 * what to do.
 *
 * All the routines below use bits of fixup code that are out of line
 * with the main instruction path.  This means when everything is well,
 * we don't even have to jump over them.  Further, they do not intrude
 * on our cache or tlb entries.
 */

struct exception_table_entry
{
        unsigned long insn, fixup;
};

struct uaccess_ops {
	size_t (*copy_from_user)(size_t, const void __user *, void *);
	size_t (*copy_from_user_small)(size_t, const void __user *, void *);
	size_t (*copy_to_user)(size_t, void __user *, const void *);
	size_t (*copy_to_user_small)(size_t, void __user *, const void *);
	size_t (*copy_in_user)(size_t, void __user *, const void __user *);
	size_t (*clear_user)(size_t, void __user *);
	size_t (*strnlen_user)(size_t, const char __user *);
	size_t (*strncpy_from_user)(size_t, const char __user *, char *);
	int (*futex_atomic_op)(int op, u32 __user *, int oparg, int *old);
	int (*futex_atomic_cmpxchg)(u32 *, u32 __user *, u32 old, u32 new);
};

extern struct uaccess_ops uaccess;
extern struct uaccess_ops uaccess_std;
extern struct uaccess_ops uaccess_mvcos;
extern struct uaccess_ops uaccess_mvcos_switch;
extern struct uaccess_ops uaccess_pt;

extern int __handle_fault(unsigned long, unsigned long, int);

static inline int __put_user_fn(size_t size, void __user *ptr, void *x)
{
	size = uaccess.copy_to_user_small(size, ptr, x);
	return size ? -EFAULT : size;
}

static inline int __get_user_fn(size_t size, const void __user *ptr, void *x)
{
	size = uaccess.copy_from_user_small(size, ptr, x);
	return size ? -EFAULT : size;
=======
#include <asm/asm-extable.h>
#include <asm/processor.h>
#include <asm/extable.h>
#include <asm/facility.h>
#include <asm-generic/access_ok.h>

void debug_user_asce(int exit);

unsigned long __must_check
raw_copy_from_user(void *to, const void __user *from, unsigned long n);

unsigned long __must_check
raw_copy_to_user(void __user *to, const void *from, unsigned long n);

#ifndef CONFIG_KASAN
#define INLINE_COPY_FROM_USER
#define INLINE_COPY_TO_USER
#endif

unsigned long __must_check
_copy_from_user_key(void *to, const void __user *from, unsigned long n, unsigned long key);

static __always_inline unsigned long __must_check
copy_from_user_key(void *to, const void __user *from, unsigned long n, unsigned long key)
{
	if (check_copy_size(to, n, false))
		n = _copy_from_user_key(to, from, n, key);
	return n;
}

unsigned long __must_check
_copy_to_user_key(void __user *to, const void *from, unsigned long n, unsigned long key);

static __always_inline unsigned long __must_check
copy_to_user_key(void __user *to, const void *from, unsigned long n, unsigned long key)
{
	if (check_copy_size(from, n, true))
		n = _copy_to_user_key(to, from, n, key);
	return n;
}

union oac {
	unsigned int val;
	struct {
		struct {
			unsigned short key : 4;
			unsigned short	   : 4;
			unsigned short as  : 2;
			unsigned short	   : 4;
			unsigned short k   : 1;
			unsigned short a   : 1;
		} oac1;
		struct {
			unsigned short key : 4;
			unsigned short	   : 4;
			unsigned short as  : 2;
			unsigned short	   : 4;
			unsigned short k   : 1;
			unsigned short a   : 1;
		} oac2;
	};
};

int __noreturn __put_user_bad(void);

#define __put_user_asm(to, from, size)					\
({									\
	union oac __oac_spec = {					\
		.oac1.as = PSW_BITS_AS_SECONDARY,			\
		.oac1.a = 1,						\
	};								\
	int __rc;							\
									\
	asm volatile(							\
		"	lr	0,%[spec]\n"				\
		"0:	mvcos	%[_to],%[_from],%[_size]\n"		\
		"1:	xr	%[rc],%[rc]\n"				\
		"2:\n"							\
		EX_TABLE_UA_STORE(0b, 2b, %[rc])			\
		EX_TABLE_UA_STORE(1b, 2b, %[rc])			\
		: [rc] "=&d" (__rc), [_to] "+Q" (*(to))			\
		: [_size] "d" (size), [_from] "Q" (*(from)),		\
		  [spec] "d" (__oac_spec.val)				\
		: "cc", "0");						\
	__rc;								\
})

static __always_inline int __put_user_fn(void *x, void __user *ptr, unsigned long size)
{
	int rc;

	switch (size) {
	case 1:
		rc = __put_user_asm((unsigned char __user *)ptr,
				    (unsigned char *)x,
				    size);
		break;
	case 2:
		rc = __put_user_asm((unsigned short __user *)ptr,
				    (unsigned short *)x,
				    size);
		break;
	case 4:
		rc = __put_user_asm((unsigned int __user *)ptr,
				    (unsigned int *)x,
				    size);
		break;
	case 8:
		rc = __put_user_asm((unsigned long __user *)ptr,
				    (unsigned long *)x,
				    size);
		break;
	default:
		__put_user_bad();
		break;
	}
	return rc;
}

int __noreturn __get_user_bad(void);

#define __get_user_asm(to, from, size)					\
({									\
	union oac __oac_spec = {					\
		.oac2.as = PSW_BITS_AS_SECONDARY,			\
		.oac2.a = 1,						\
	};								\
	int __rc;							\
									\
	asm volatile(							\
		"	lr	0,%[spec]\n"				\
		"0:	mvcos	0(%[_to]),%[_from],%[_size]\n"		\
		"1:	xr	%[rc],%[rc]\n"				\
		"2:\n"							\
		EX_TABLE_UA_LOAD_MEM(0b, 2b, %[rc], %[_to], %[_ksize])	\
		EX_TABLE_UA_LOAD_MEM(1b, 2b, %[rc], %[_to], %[_ksize])	\
		: [rc] "=&d" (__rc), "=Q" (*(to))			\
		: [_size] "d" (size), [_from] "Q" (*(from)),		\
		  [spec] "d" (__oac_spec.val), [_to] "a" (to),		\
		  [_ksize] "K" (size)					\
		: "cc", "0");						\
	__rc;								\
})

static __always_inline int __get_user_fn(void *x, const void __user *ptr, unsigned long size)
{
	int rc;

	switch (size) {
	case 1:
		rc = __get_user_asm((unsigned char *)x,
				    (unsigned char __user *)ptr,
				    size);
		break;
	case 2:
		rc = __get_user_asm((unsigned short *)x,
				    (unsigned short __user *)ptr,
				    size);
		break;
	case 4:
		rc = __get_user_asm((unsigned int *)x,
				    (unsigned int __user *)ptr,
				    size);
		break;
	case 8:
		rc = __get_user_asm((unsigned long *)x,
				    (unsigned long __user *)ptr,
				    size);
		break;
	default:
		__get_user_bad();
		break;
	}
	return rc;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

/*
 * These are the main single-value transfer routines.  They automatically
 * use the right size if we just have the right pointer type.
 */
<<<<<<< HEAD
#define __put_user(x, ptr) \
({								\
	__typeof__(*(ptr)) __x = (x);				\
	int __pu_err = -EFAULT;					\
        __chk_user_ptr(ptr);                                    \
	switch (sizeof (*(ptr))) {				\
	case 1:							\
	case 2:							\
	case 4:							\
	case 8:							\
		__pu_err = __put_user_fn(sizeof (*(ptr)),	\
					 ptr, &__x);		\
		break;						\
	default:						\
		__put_user_bad();				\
		break;						\
	 }							\
	__pu_err;						\
})

#define put_user(x, ptr)					\
({								\
	might_fault();						\
	__put_user(x, ptr);					\
})


extern int __put_user_bad(void) __attribute__((noreturn));

#define __get_user(x, ptr)					\
({								\
	int __gu_err = -EFAULT;					\
	__chk_user_ptr(ptr);					\
	switch (sizeof(*(ptr))) {				\
	case 1: {						\
		unsigned char __x;				\
		__gu_err = __get_user_fn(sizeof (*(ptr)),	\
					 ptr, &__x);		\
		(x) = *(__force __typeof__(*(ptr)) *) &__x;	\
		break;						\
	};							\
	case 2: {						\
		unsigned short __x;				\
		__gu_err = __get_user_fn(sizeof (*(ptr)),	\
					 ptr, &__x);		\
		(x) = *(__force __typeof__(*(ptr)) *) &__x;	\
		break;						\
	};							\
	case 4: {						\
		unsigned int __x;				\
		__gu_err = __get_user_fn(sizeof (*(ptr)),	\
					 ptr, &__x);		\
		(x) = *(__force __typeof__(*(ptr)) *) &__x;	\
		break;						\
	};							\
	case 8: {						\
		unsigned long long __x;				\
		__gu_err = __get_user_fn(sizeof (*(ptr)),	\
					 ptr, &__x);		\
		(x) = *(__force __typeof__(*(ptr)) *) &__x;	\
		break;						\
	};							\
	default:						\
		__get_user_bad();				\
		break;						\
	}							\
	__gu_err;						\
})

#define get_user(x, ptr)					\
({								\
	might_fault();						\
	__get_user(x, ptr);					\
})

extern int __get_user_bad(void) __attribute__((noreturn));

#define __put_user_unaligned __put_user
#define __get_user_unaligned __get_user

/**
 * __copy_to_user: - Copy a block of data into user space, with less checking.
 * @to:   Destination address, in user space.
 * @from: Source address, in kernel space.
 * @n:    Number of bytes to copy.
 *
 * Context: User context only.  This function may sleep.
 *
 * Copy data from kernel space to user space.  Caller must check
 * the specified block with access_ok() before calling this function.
 *
 * Returns number of bytes that could not be copied.
 * On success, this will be zero.
 */
static inline unsigned long __must_check
__copy_to_user(void __user *to, const void *from, unsigned long n)
{
	if (__builtin_constant_p(n) && (n <= 256))
		return uaccess.copy_to_user_small(n, to, from);
	else
		return uaccess.copy_to_user(n, to, from);
}

#define __copy_to_user_inatomic __copy_to_user
#define __copy_from_user_inatomic __copy_from_user

/**
 * copy_to_user: - Copy a block of data into user space.
 * @to:   Destination address, in user space.
 * @from: Source address, in kernel space.
 * @n:    Number of bytes to copy.
 *
 * Context: User context only.  This function may sleep.
 *
 * Copy data from kernel space to user space.
 *
 * Returns number of bytes that could not be copied.
 * On success, this will be zero.
 */
static inline unsigned long __must_check
copy_to_user(void __user *to, const void *from, unsigned long n)
{
	might_fault();
	if (access_ok(VERIFY_WRITE, to, n))
		n = __copy_to_user(to, from, n);
	return n;
}

/**
 * __copy_from_user: - Copy a block of data from user space, with less checking.
 * @to:   Destination address, in kernel space.
 * @from: Source address, in user space.
 * @n:    Number of bytes to copy.
 *
 * Context: User context only.  This function may sleep.
 *
 * Copy data from user space to kernel space.  Caller must check
 * the specified block with access_ok() before calling this function.
 *
 * Returns number of bytes that could not be copied.
 * On success, this will be zero.
 *
 * If some data could not be copied, this function will pad the copied
 * data to the requested size using zero bytes.
 */
static inline unsigned long __must_check
__copy_from_user(void *to, const void __user *from, unsigned long n)
{
	if (__builtin_constant_p(n) && (n <= 256))
		return uaccess.copy_from_user_small(n, from, to);
	else
		return uaccess.copy_from_user(n, from, to);
}

extern void copy_from_user_overflow(void)
#ifdef CONFIG_DEBUG_STRICT_USER_COPY_CHECKS
__compiletime_warning("copy_from_user() buffer size is not provably correct")
#endif
;

/**
 * copy_from_user: - Copy a block of data from user space.
 * @to:   Destination address, in kernel space.
 * @from: Source address, in user space.
 * @n:    Number of bytes to copy.
 *
 * Context: User context only.  This function may sleep.
 *
 * Copy data from user space to kernel space.
 *
 * Returns number of bytes that could not be copied.
 * On success, this will be zero.
 *
 * If some data could not be copied, this function will pad the copied
 * data to the requested size using zero bytes.
 */
static inline unsigned long __must_check
copy_from_user(void *to, const void __user *from, unsigned long n)
{
	unsigned int sz = __compiletime_object_size(to);

	might_fault();
	if (unlikely(sz != -1 && sz < n)) {
		copy_from_user_overflow();
		return n;
	}
	if (access_ok(VERIFY_READ, from, n))
		n = __copy_from_user(to, from, n);
	else
		memset(to, 0, n);
	return n;
}

static inline unsigned long __must_check
__copy_in_user(void __user *to, const void __user *from, unsigned long n)
{
	return uaccess.copy_in_user(n, to, from);
}

static inline unsigned long __must_check
copy_in_user(void __user *to, const void __user *from, unsigned long n)
{
	might_fault();
	if (__access_ok(from,n) && __access_ok(to,n))
		n = __copy_in_user(to, from, n);
	return n;
}

/*
 * Copy a null terminated string from userspace.
 */
static inline long __must_check
strncpy_from_user(char *dst, const char __user *src, long count)
{
        long res = -EFAULT;
	might_fault();
        if (access_ok(VERIFY_READ, src, 1))
		res = uaccess.strncpy_from_user(count, src, dst);
        return res;
}

static inline unsigned long
strnlen_user(const char __user * src, unsigned long n)
{
	might_fault();
	return uaccess.strnlen_user(n, src);
}

/**
 * strlen_user: - Get the size of a string in user space.
 * @str: The string to measure.
 *
 * Context: User context only.  This function may sleep.
 *
 * Get the size of a NUL-terminated string in user space.
 *
 * Returns the size of the string INCLUDING the terminating NUL.
 * On exception, returns 0.
 *
 * If there is a limit on the length of a valid string, you may wish to
 * consider using strnlen_user() instead.
 */
#define strlen_user(str) strnlen_user(str, ~0UL)
=======
#define __put_user(x, ptr)						\
({									\
	__typeof__(*(ptr)) __x = (x);					\
	int __pu_err = -EFAULT;						\
									\
	__chk_user_ptr(ptr);						\
	switch (sizeof(*(ptr))) {					\
	case 1:								\
	case 2:								\
	case 4:								\
	case 8:								\
		__pu_err = __put_user_fn(&__x, ptr, sizeof(*(ptr)));	\
		break;							\
	default:							\
		__put_user_bad();					\
		break;							\
	}								\
	__builtin_expect(__pu_err, 0);					\
})

#define put_user(x, ptr)						\
({									\
	might_fault();							\
	__put_user(x, ptr);						\
})

#define __get_user(x, ptr)						\
({									\
	int __gu_err = -EFAULT;						\
									\
	__chk_user_ptr(ptr);						\
	switch (sizeof(*(ptr))) {					\
	case 1: {							\
		unsigned char __x;					\
									\
		__gu_err = __get_user_fn(&__x, ptr, sizeof(*(ptr)));	\
		(x) = *(__force __typeof__(*(ptr)) *)&__x;		\
		break;							\
	};								\
	case 2: {							\
		unsigned short __x;					\
									\
		__gu_err = __get_user_fn(&__x, ptr, sizeof(*(ptr)));	\
		(x) = *(__force __typeof__(*(ptr)) *)&__x;		\
		break;							\
	};								\
	case 4: {							\
		unsigned int __x;					\
									\
		__gu_err = __get_user_fn(&__x, ptr, sizeof(*(ptr)));	\
		(x) = *(__force __typeof__(*(ptr)) *)&__x;		\
		break;							\
	};								\
	case 8: {							\
		unsigned long __x;					\
									\
		__gu_err = __get_user_fn(&__x, ptr, sizeof(*(ptr)));	\
		(x) = *(__force __typeof__(*(ptr)) *)&__x;		\
		break;							\
	};								\
	default:							\
		__get_user_bad();					\
		break;							\
	}								\
	__builtin_expect(__gu_err, 0);					\
})

#define get_user(x, ptr)						\
({									\
	might_fault();							\
	__get_user(x, ptr);						\
})

/*
 * Copy a null terminated string from userspace.
 */
long __must_check strncpy_from_user(char *dst, const char __user *src, long count);

long __must_check strnlen_user(const char __user *src, long count);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

/*
 * Zero Userspace
 */
<<<<<<< HEAD

static inline unsigned long __must_check
__clear_user(void __user *to, unsigned long n)
{
	return uaccess.clear_user(n, to);
}

static inline unsigned long __must_check
clear_user(void __user *to, unsigned long n)
{
	might_fault();
	if (access_ok(VERIFY_WRITE, to, n))
		n = uaccess.clear_user(n, to);
	return n;
}

extern int memcpy_real(void *, void *, size_t);
extern void copy_to_absolute_zero(void *dest, void *src, size_t count);
extern int copy_to_user_real(void __user *dest, void *src, size_t count);
extern int copy_from_user_real(void *dest, void __user *src, size_t count);
=======
unsigned long __must_check __clear_user(void __user *to, unsigned long size);

static inline unsigned long __must_check clear_user(void __user *to, unsigned long n)
{
	might_fault();
	return __clear_user(to, n);
}

void *s390_kernel_write(void *dst, const void *src, size_t size);

int __noreturn __put_kernel_bad(void);

#define __put_kernel_asm(val, to, insn)					\
({									\
	int __rc;							\
									\
	asm volatile(							\
		"0:   " insn "  %[_val],%[_to]\n"			\
		"1:	xr	%[rc],%[rc]\n"				\
		"2:\n"							\
		EX_TABLE_UA_STORE(0b, 2b, %[rc])			\
		EX_TABLE_UA_STORE(1b, 2b, %[rc])			\
		: [rc] "=d" (__rc), [_to] "+Q" (*(to))			\
		: [_val] "d" (val)					\
		: "cc");						\
	__rc;								\
})

#define __put_kernel_nofault(dst, src, type, err_label)			\
do {									\
	unsigned long __x = (unsigned long)(*((type *)(src)));		\
	int __pk_err;							\
									\
	switch (sizeof(type)) {						\
	case 1:								\
		__pk_err = __put_kernel_asm(__x, (type *)(dst), "stc"); \
		break;							\
	case 2:								\
		__pk_err = __put_kernel_asm(__x, (type *)(dst), "sth"); \
		break;							\
	case 4:								\
		__pk_err = __put_kernel_asm(__x, (type *)(dst), "st");	\
		break;							\
	case 8:								\
		__pk_err = __put_kernel_asm(__x, (type *)(dst), "stg"); \
		break;							\
	default:							\
		__pk_err = __put_kernel_bad();				\
		break;							\
	}								\
	if (unlikely(__pk_err))						\
		goto err_label;						\
} while (0)

int __noreturn __get_kernel_bad(void);

#define __get_kernel_asm(val, from, insn)				\
({									\
	int __rc;							\
									\
	asm volatile(							\
		"0:   " insn "  %[_val],%[_from]\n"			\
		"1:	xr	%[rc],%[rc]\n"				\
		"2:\n"							\
		EX_TABLE_UA_LOAD_REG(0b, 2b, %[rc], %[_val])		\
		EX_TABLE_UA_LOAD_REG(1b, 2b, %[rc], %[_val])		\
		: [rc] "=d" (__rc), [_val] "=d" (val)			\
		: [_from] "Q" (*(from))					\
		: "cc");						\
	__rc;								\
})

#define __get_kernel_nofault(dst, src, type, err_label)			\
do {									\
	int __gk_err;							\
									\
	switch (sizeof(type)) {						\
	case 1: {							\
		unsigned char __x;					\
									\
		__gk_err = __get_kernel_asm(__x, (type *)(src), "ic");	\
		*((type *)(dst)) = (type)__x;				\
		break;							\
	};								\
	case 2: {							\
		unsigned short __x;					\
									\
		__gk_err = __get_kernel_asm(__x, (type *)(src), "lh");	\
		*((type *)(dst)) = (type)__x;				\
		break;							\
	};								\
	case 4: {							\
		unsigned int __x;					\
									\
		__gk_err = __get_kernel_asm(__x, (type *)(src), "l");	\
		*((type *)(dst)) = (type)__x;				\
		break;							\
	};								\
	case 8: {							\
		unsigned long __x;					\
									\
		__gk_err = __get_kernel_asm(__x, (type *)(src), "lg");	\
		*((type *)(dst)) = (type)__x;				\
		break;							\
	};								\
	default:							\
		__gk_err = __get_kernel_bad();				\
		break;							\
	}								\
	if (unlikely(__gk_err))						\
		goto err_label;						\
} while (0)

void __cmpxchg_user_key_called_with_bad_pointer(void);

#define CMPXCHG_USER_KEY_MAX_LOOPS 128

static __always_inline int __cmpxchg_user_key(unsigned long address, void *uval,
					      __uint128_t old, __uint128_t new,
					      unsigned long key, int size)
{
	int rc = 0;

	switch (size) {
	case 1: {
		unsigned int prev, shift, mask, _old, _new;
		unsigned long count;

		shift = (3 ^ (address & 3)) << 3;
		address ^= address & 3;
		_old = ((unsigned int)old & 0xff) << shift;
		_new = ((unsigned int)new & 0xff) << shift;
		mask = ~(0xff << shift);
		asm volatile(
			"	spka	0(%[key])\n"
			"	sacf	256\n"
			"	llill	%[count],%[max_loops]\n"
			"0:	l	%[prev],%[address]\n"
			"1:	nr	%[prev],%[mask]\n"
			"	xilf	%[mask],0xffffffff\n"
			"	or	%[new],%[prev]\n"
			"	or	%[prev],%[tmp]\n"
			"2:	lr	%[tmp],%[prev]\n"
			"3:	cs	%[prev],%[new],%[address]\n"
			"4:	jnl	5f\n"
			"	xr	%[tmp],%[prev]\n"
			"	xr	%[new],%[tmp]\n"
			"	nr	%[tmp],%[mask]\n"
			"	jnz	5f\n"
			"	brct	%[count],2b\n"
			"5:	sacf	768\n"
			"	spka	%[default_key]\n"
			EX_TABLE_UA_LOAD_REG(0b, 5b, %[rc], %[prev])
			EX_TABLE_UA_LOAD_REG(1b, 5b, %[rc], %[prev])
			EX_TABLE_UA_LOAD_REG(3b, 5b, %[rc], %[prev])
			EX_TABLE_UA_LOAD_REG(4b, 5b, %[rc], %[prev])
			: [rc] "+&d" (rc),
			  [prev] "=&d" (prev),
			  [address] "+Q" (*(int *)address),
			  [tmp] "+&d" (_old),
			  [new] "+&d" (_new),
			  [mask] "+&d" (mask),
			  [count] "=a" (count)
			: [key] "%[count]" (key << 4),
			  [default_key] "J" (PAGE_DEFAULT_KEY),
			  [max_loops] "J" (CMPXCHG_USER_KEY_MAX_LOOPS)
			: "memory", "cc");
		*(unsigned char *)uval = prev >> shift;
		if (!count)
			rc = -EAGAIN;
		return rc;
	}
	case 2: {
		unsigned int prev, shift, mask, _old, _new;
		unsigned long count;

		shift = (2 ^ (address & 2)) << 3;
		address ^= address & 2;
		_old = ((unsigned int)old & 0xffff) << shift;
		_new = ((unsigned int)new & 0xffff) << shift;
		mask = ~(0xffff << shift);
		asm volatile(
			"	spka	0(%[key])\n"
			"	sacf	256\n"
			"	llill	%[count],%[max_loops]\n"
			"0:	l	%[prev],%[address]\n"
			"1:	nr	%[prev],%[mask]\n"
			"	xilf	%[mask],0xffffffff\n"
			"	or	%[new],%[prev]\n"
			"	or	%[prev],%[tmp]\n"
			"2:	lr	%[tmp],%[prev]\n"
			"3:	cs	%[prev],%[new],%[address]\n"
			"4:	jnl	5f\n"
			"	xr	%[tmp],%[prev]\n"
			"	xr	%[new],%[tmp]\n"
			"	nr	%[tmp],%[mask]\n"
			"	jnz	5f\n"
			"	brct	%[count],2b\n"
			"5:	sacf	768\n"
			"	spka	%[default_key]\n"
			EX_TABLE_UA_LOAD_REG(0b, 5b, %[rc], %[prev])
			EX_TABLE_UA_LOAD_REG(1b, 5b, %[rc], %[prev])
			EX_TABLE_UA_LOAD_REG(3b, 5b, %[rc], %[prev])
			EX_TABLE_UA_LOAD_REG(4b, 5b, %[rc], %[prev])
			: [rc] "+&d" (rc),
			  [prev] "=&d" (prev),
			  [address] "+Q" (*(int *)address),
			  [tmp] "+&d" (_old),
			  [new] "+&d" (_new),
			  [mask] "+&d" (mask),
			  [count] "=a" (count)
			: [key] "%[count]" (key << 4),
			  [default_key] "J" (PAGE_DEFAULT_KEY),
			  [max_loops] "J" (CMPXCHG_USER_KEY_MAX_LOOPS)
			: "memory", "cc");
		*(unsigned short *)uval = prev >> shift;
		if (!count)
			rc = -EAGAIN;
		return rc;
	}
	case 4:	{
		unsigned int prev = old;

		asm volatile(
			"	spka	0(%[key])\n"
			"	sacf	256\n"
			"0:	cs	%[prev],%[new],%[address]\n"
			"1:	sacf	768\n"
			"	spka	%[default_key]\n"
			EX_TABLE_UA_LOAD_REG(0b, 1b, %[rc], %[prev])
			EX_TABLE_UA_LOAD_REG(1b, 1b, %[rc], %[prev])
			: [rc] "+&d" (rc),
			  [prev] "+&d" (prev),
			  [address] "+Q" (*(int *)address)
			: [new] "d" ((unsigned int)new),
			  [key] "a" (key << 4),
			  [default_key] "J" (PAGE_DEFAULT_KEY)
			: "memory", "cc");
		*(unsigned int *)uval = prev;
		return rc;
	}
	case 8: {
		unsigned long prev = old;

		asm volatile(
			"	spka	0(%[key])\n"
			"	sacf	256\n"
			"0:	csg	%[prev],%[new],%[address]\n"
			"1:	sacf	768\n"
			"	spka	%[default_key]\n"
			EX_TABLE_UA_LOAD_REG(0b, 1b, %[rc], %[prev])
			EX_TABLE_UA_LOAD_REG(1b, 1b, %[rc], %[prev])
			: [rc] "+&d" (rc),
			  [prev] "+&d" (prev),
			  [address] "+QS" (*(long *)address)
			: [new] "d" ((unsigned long)new),
			  [key] "a" (key << 4),
			  [default_key] "J" (PAGE_DEFAULT_KEY)
			: "memory", "cc");
		*(unsigned long *)uval = prev;
		return rc;
	}
	case 16: {
		__uint128_t prev = old;

		asm volatile(
			"	spka	0(%[key])\n"
			"	sacf	256\n"
			"0:	cdsg	%[prev],%[new],%[address]\n"
			"1:	sacf	768\n"
			"	spka	%[default_key]\n"
			EX_TABLE_UA_LOAD_REGPAIR(0b, 1b, %[rc], %[prev])
			EX_TABLE_UA_LOAD_REGPAIR(1b, 1b, %[rc], %[prev])
			: [rc] "+&d" (rc),
			  [prev] "+&d" (prev),
			  [address] "+QS" (*(__int128_t *)address)
			: [new] "d" (new),
			  [key] "a" (key << 4),
			  [default_key] "J" (PAGE_DEFAULT_KEY)
			: "memory", "cc");
		*(__uint128_t *)uval = prev;
		return rc;
	}
	}
	__cmpxchg_user_key_called_with_bad_pointer();
	return rc;
}

/**
 * cmpxchg_user_key() - cmpxchg with user space target, honoring storage keys
 * @ptr: User space address of value to compare to @old and exchange with
 *	 @new. Must be aligned to sizeof(*@ptr).
 * @uval: Address where the old value of *@ptr is written to.
 * @old: Old value. Compared to the content pointed to by @ptr in order to
 *	 determine if the exchange occurs. The old value read from *@ptr is
 *	 written to *@uval.
 * @new: New value to place at *@ptr.
 * @key: Access key to use for checking storage key protection.
 *
 * Perform a cmpxchg on a user space target, honoring storage key protection.
 * @key alone determines how key checking is performed, neither
 * storage-protection-override nor fetch-protection-override apply.
 * The caller must compare *@uval and @old to determine if values have been
 * exchanged. In case of an exception *@uval is set to zero.
 *
 * Return:     0: cmpxchg executed
 *	       -EFAULT: an exception happened when trying to access *@ptr
 *	       -EAGAIN: maxed out number of retries (byte and short only)
 */
#define cmpxchg_user_key(ptr, uval, old, new, key)			\
({									\
	__typeof__(ptr) __ptr = (ptr);					\
	__typeof__(uval) __uval = (uval);				\
									\
	BUILD_BUG_ON(sizeof(*(__ptr)) != sizeof(*(__uval)));		\
	might_fault();							\
	__chk_user_ptr(__ptr);						\
	__cmpxchg_user_key((unsigned long)(__ptr), (void *)(__uval),	\
			   (old), (new), (key), sizeof(*(__ptr)));	\
})
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

#endif /* __S390_UACCESS_H */
