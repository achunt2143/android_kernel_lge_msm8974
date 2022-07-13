<<<<<<< HEAD
#ifndef _ARCH_POWERPC_UACCESS_H
#define _ARCH_POWERPC_UACCESS_H

#ifdef __KERNEL__
#ifndef __ASSEMBLY__

#include <linux/sched.h>
#include <linux/errno.h>
#include <asm/asm-compat.h>
#include <asm/processor.h>
#include <asm/page.h>

#define VERIFY_READ	0
#define VERIFY_WRITE	1

/*
 * The fs value determines whether argument validity checking should be
 * performed or not.  If get_fs() == USER_DS, checking is performed, with
 * get_fs() == KERNEL_DS, checking is bypassed.
 *
 * For historical reasons, these macros are grossly misnamed.
 *
 * The fs/ds values are now the highest legal address in the "segment".
 * This simplifies the checking in the routines below.
 */

#define MAKE_MM_SEG(s)  ((mm_segment_t) { (s) })

#define KERNEL_DS	MAKE_MM_SEG(~0UL)
#ifdef __powerpc64__
/* We use TASK_SIZE_USER64 as TASK_SIZE is not constant */
#define USER_DS		MAKE_MM_SEG(TASK_SIZE_USER64 - 1)
#else
#define USER_DS		MAKE_MM_SEG(TASK_SIZE - 1)
#endif

#define get_ds()	(KERNEL_DS)
#define get_fs()	(current->thread.fs)
#define set_fs(val)	(current->thread.fs = (val))

#define segment_eq(a, b)	((a).seg == (b).seg)

#ifdef __powerpc64__
/*
 * This check is sufficient because there is a large enough
 * gap between user addresses and the kernel addresses
 */
#define __access_ok(addr, size, segment)	\
	(((addr) <= (segment).seg) && ((size) <= (segment).seg))

#else

#define __access_ok(addr, size, segment)	\
	(((addr) <= (segment).seg) &&		\
	 (((size) == 0) || (((size) - 1) <= ((segment).seg - (addr)))))

#endif

#define access_ok(type, addr, size)		\
	(__chk_user_ptr(addr),			\
	 __access_ok((__force unsigned long)(addr), (size), get_fs()))

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

struct exception_table_entry {
	unsigned long insn;
	unsigned long fixup;
};
=======
/* SPDX-License-Identifier: GPL-2.0 */
#ifndef _ARCH_POWERPC_UACCESS_H
#define _ARCH_POWERPC_UACCESS_H

#include <asm/processor.h>
#include <asm/page.h>
#include <asm/extable.h>
#include <asm/kup.h>

#ifdef __powerpc64__
/* We use TASK_SIZE_USER64 as TASK_SIZE is not constant */
#define TASK_SIZE_MAX		TASK_SIZE_USER64
#endif

#include <asm-generic/access_ok.h>
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

/*
 * These are the main single-value transfer routines.  They automatically
 * use the right size if we just have the right pointer type.
 *
 * This gets kind of ugly. We want to return _two_ values in "get_user()"
 * and yet we don't want to do any pointers, because that is too much
 * of a performance impact. Thus we have a few rather ugly macros here,
 * and hide all the ugliness from the user.
 *
 * The "__xxx" versions of the user access functions are versions that
 * do not verify the address space, that must have been done previously
 * with a separate "access_ok()" call (this is used when we do multiple
 * accesses to the same area of user memory).
 *
 * As we use the same address space for kernel and user data on the
 * PowerPC, we can just do these as direct assignments.  (Of course, the
 * exception handling means that it's no longer "just"...)
 *
<<<<<<< HEAD
 * The "user64" versions of the user access functions are versions that
 * allow access of 64-bit data. The "get_user" functions do not
 * properly handle 64-bit data because the value gets down cast to a long.
 * The "put_user" functions already handle 64-bit data properly but we add
 * "user64" versions for completeness
 */
#define get_user(x, ptr) \
	__get_user_check((x), (ptr), sizeof(*(ptr)))
#define put_user(x, ptr) \
	__put_user_check((__typeof__(*(ptr)))(x), (ptr), sizeof(*(ptr)))

#define __get_user(x, ptr) \
	__get_user_nocheck((x), (ptr), sizeof(*(ptr)))
#define __put_user(x, ptr) \
	__put_user_nocheck((__typeof__(*(ptr)))(x), (ptr), sizeof(*(ptr)))

#ifndef __powerpc64__
#define __get_user64(x, ptr) \
	__get_user64_nocheck((x), (ptr), sizeof(*(ptr)))
#define __put_user64(x, ptr) __put_user(x, ptr)
#endif

#define __get_user_inatomic(x, ptr) \
	__get_user_nosleep((x), (ptr), sizeof(*(ptr)))
#define __put_user_inatomic(x, ptr) \
	__put_user_nosleep((__typeof__(*(ptr)))(x), (ptr), sizeof(*(ptr)))

#define __get_user_unaligned __get_user
#define __put_user_unaligned __put_user

extern long __put_user_bad(void);
=======
 */
#define __put_user(x, ptr)					\
({								\
	long __pu_err;						\
	__typeof__(*(ptr)) __user *__pu_addr = (ptr);		\
	__typeof__(*(ptr)) __pu_val = (__typeof__(*(ptr)))(x);	\
	__typeof__(sizeof(*(ptr))) __pu_size = sizeof(*(ptr));	\
								\
	might_fault();						\
	do {							\
		__label__ __pu_failed;				\
								\
		allow_write_to_user(__pu_addr, __pu_size);	\
		__put_user_size_goto(__pu_val, __pu_addr, __pu_size, __pu_failed);	\
		prevent_write_to_user(__pu_addr, __pu_size);	\
		__pu_err = 0;					\
		break;						\
								\
__pu_failed:							\
		prevent_write_to_user(__pu_addr, __pu_size);	\
		__pu_err = -EFAULT;				\
	} while (0);						\
								\
	__pu_err;						\
})

#define put_user(x, ptr)						\
({									\
	__typeof__(*(ptr)) __user *_pu_addr = (ptr);			\
									\
	access_ok(_pu_addr, sizeof(*(ptr))) ?				\
		  __put_user(x, _pu_addr) : -EFAULT;			\
})
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

/*
 * We don't tell gcc that we are accessing memory, but this is OK
 * because we do not write to any memory gcc knows about, so there
 * are no aliasing issues.
 */
<<<<<<< HEAD
#define __put_user_asm(x, addr, err, op)			\
	__asm__ __volatile__(					\
		"1:	" op " %1,0(%2)	# put_user\n"		\
		"2:\n"						\
		".section .fixup,\"ax\"\n"			\
		"3:	li %0,%3\n"				\
		"	b 2b\n"					\
		".previous\n"					\
		".section __ex_table,\"a\"\n"			\
			PPC_LONG_ALIGN "\n"			\
			PPC_LONG "1b,3b\n"			\
		".previous"					\
		: "=r" (err)					\
		: "r" (x), "b" (addr), "i" (-EFAULT), "0" (err))

#ifdef __powerpc64__
#define __put_user_asm2(x, ptr, retval)				\
	  __put_user_asm(x, ptr, retval, "std")
#else /* __powerpc64__ */
#define __put_user_asm2(x, addr, err)				\
	__asm__ __volatile__(					\
		"1:	stw %1,0(%2)\n"				\
		"2:	stw %1+1,4(%2)\n"			\
		"3:\n"						\
		".section .fixup,\"ax\"\n"			\
		"4:	li %0,%3\n"				\
		"	b 3b\n"					\
		".previous\n"					\
		".section __ex_table,\"a\"\n"			\
			PPC_LONG_ALIGN "\n"			\
			PPC_LONG "1b,4b\n"			\
			PPC_LONG "2b,4b\n"			\
		".previous"					\
		: "=r" (err)					\
		: "r" (x), "b" (addr), "i" (-EFAULT), "0" (err))
#endif /* __powerpc64__ */

#define __put_user_size(x, ptr, size, retval)			\
do {								\
	retval = 0;						\
	switch (size) {						\
	  case 1: __put_user_asm(x, ptr, retval, "stb"); break;	\
	  case 2: __put_user_asm(x, ptr, retval, "sth"); break;	\
	  case 4: __put_user_asm(x, ptr, retval, "stw"); break;	\
	  case 8: __put_user_asm2(x, ptr, retval); break;	\
	  default: __put_user_bad();				\
	}							\
} while (0)

#define __put_user_nocheck(x, ptr, size)			\
({								\
	long __pu_err;						\
	__typeof__(*(ptr)) __user *__pu_addr = (ptr);		\
	if (!is_kernel_addr((unsigned long)__pu_addr))		\
		might_sleep();					\
	__chk_user_ptr(ptr);					\
	__put_user_size((x), __pu_addr, (size), __pu_err);	\
	__pu_err;						\
})

#define __put_user_check(x, ptr, size)					\
({									\
	long __pu_err = -EFAULT;					\
	__typeof__(*(ptr)) __user *__pu_addr = (ptr);			\
	might_sleep();							\
	if (access_ok(VERIFY_WRITE, __pu_addr, size))			\
		__put_user_size((x), __pu_addr, (size), __pu_err);	\
	__pu_err;							\
})

#define __put_user_nosleep(x, ptr, size)			\
({								\
	long __pu_err;						\
	__typeof__(*(ptr)) __user *__pu_addr = (ptr);		\
	__chk_user_ptr(ptr);					\
	__put_user_size((x), __pu_addr, (size), __pu_err);	\
	__pu_err;						\
})


extern long __get_user_bad(void);

#define __get_user_asm(x, addr, err, op)		\
	__asm__ __volatile__(				\
		"1:	"op" %1,0(%2)	# get_user\n"	\
=======
/* -mprefixed can generate offsets beyond range, fall back hack */
#ifdef CONFIG_PPC_KERNEL_PREFIXED
#define __put_user_asm_goto(x, addr, label, op)			\
	asm goto(					\
		"1:	" op " %0,0(%1)	# put_user\n"		\
		EX_TABLE(1b, %l2)				\
		:						\
		: "r" (x), "b" (addr)				\
		:						\
		: label)
#else
#define __put_user_asm_goto(x, addr, label, op)			\
	asm goto(					\
		"1:	" op "%U1%X1 %0,%1	# put_user\n"	\
		EX_TABLE(1b, %l2)				\
		:						\
		: "r" (x), "m<>" (*addr)			\
		:						\
		: label)
#endif

#ifdef __powerpc64__
#define __put_user_asm2_goto(x, ptr, label)			\
	__put_user_asm_goto(x, ptr, label, "std")
#else /* __powerpc64__ */
#define __put_user_asm2_goto(x, addr, label)			\
	asm goto(					\
		"1:	stw%X1 %0, %1\n"			\
		"2:	stw%X1 %L0, %L1\n"			\
		EX_TABLE(1b, %l2)				\
		EX_TABLE(2b, %l2)				\
		:						\
		: "r" (x), "m" (*addr)				\
		:						\
		: label)
#endif /* __powerpc64__ */

#define __put_user_size_goto(x, ptr, size, label)		\
do {								\
	__typeof__(*(ptr)) __user *__pus_addr = (ptr);		\
								\
	switch (size) {						\
	case 1: __put_user_asm_goto(x, __pus_addr, label, "stb"); break;	\
	case 2: __put_user_asm_goto(x, __pus_addr, label, "sth"); break;	\
	case 4: __put_user_asm_goto(x, __pus_addr, label, "stw"); break;	\
	case 8: __put_user_asm2_goto(x, __pus_addr, label); break;		\
	default: BUILD_BUG();					\
	}							\
} while (0)

/*
 * This does an atomic 128 byte aligned load from userspace.
 * Upto caller to do enable_kernel_vmx() before calling!
 */
#define __get_user_atomic_128_aligned(kaddr, uaddr, err)		\
	__asm__ __volatile__(				\
		".machine push\n"			\
		".machine altivec\n"			\
		"1:	lvx  0,0,%1	# get user\n"	\
		" 	stvx 0,0,%2	# put kernel\n"	\
		".machine pop\n"			\
		"2:\n"					\
		".section .fixup,\"ax\"\n"		\
		"3:	li %0,%3\n"			\
		"	b 2b\n"				\
		".previous\n"				\
		EX_TABLE(1b, 3b)			\
		: "=r" (err)			\
		: "b" (uaddr), "b" (kaddr), "i" (-EFAULT), "0" (err))

#ifdef CONFIG_CC_HAS_ASM_GOTO_OUTPUT

/* -mprefixed can generate offsets beyond range, fall back hack */
#ifdef CONFIG_PPC_KERNEL_PREFIXED
#define __get_user_asm_goto(x, addr, label, op)			\
	asm_goto_output(					\
		"1:	"op" %0,0(%1)	# get_user\n"		\
		EX_TABLE(1b, %l2)				\
		: "=r" (x)					\
		: "b" (addr)					\
		:						\
		: label)
#else
#define __get_user_asm_goto(x, addr, label, op)			\
	asm_goto_output(					\
		"1:	"op"%U1%X1 %0, %1	# get_user\n"	\
		EX_TABLE(1b, %l2)				\
		: "=r" (x)					\
		: "m<>" (*addr)					\
		:						\
		: label)
#endif

#ifdef __powerpc64__
#define __get_user_asm2_goto(x, addr, label)			\
	__get_user_asm_goto(x, addr, label, "ld")
#else /* __powerpc64__ */
#define __get_user_asm2_goto(x, addr, label)			\
	asm_goto_output(					\
		"1:	lwz%X1 %0, %1\n"			\
		"2:	lwz%X1 %L0, %L1\n"			\
		EX_TABLE(1b, %l2)				\
		EX_TABLE(2b, %l2)				\
		: "=&r" (x)					\
		: "m" (*addr)					\
		:						\
		: label)
#endif /* __powerpc64__ */

#define __get_user_size_goto(x, ptr, size, label)				\
do {										\
	BUILD_BUG_ON(size > sizeof(x));						\
	switch (size) {								\
	case 1: __get_user_asm_goto(x, (u8 __user *)ptr, label, "lbz"); break;	\
	case 2: __get_user_asm_goto(x, (u16 __user *)ptr, label, "lhz"); break;	\
	case 4: __get_user_asm_goto(x, (u32 __user *)ptr, label, "lwz"); break;	\
	case 8: __get_user_asm2_goto(x, (u64 __user *)ptr, label);  break;	\
	default: x = 0; BUILD_BUG();						\
	}									\
} while (0)

#define __get_user_size_allowed(x, ptr, size, retval)			\
do {									\
		__label__ __gus_failed;					\
									\
		__get_user_size_goto(x, ptr, size, __gus_failed);	\
		retval = 0;						\
		break;							\
__gus_failed:								\
		x = 0;							\
		retval = -EFAULT;					\
} while (0)

#else /* CONFIG_CC_HAS_ASM_GOTO_OUTPUT */

#define __get_user_asm(x, addr, err, op)		\
	__asm__ __volatile__(				\
		"1:	"op"%U2%X2 %1, %2	# get_user\n"	\
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		"2:\n"					\
		".section .fixup,\"ax\"\n"		\
		"3:	li %0,%3\n"			\
		"	li %1,0\n"			\
		"	b 2b\n"				\
		".previous\n"				\
<<<<<<< HEAD
		".section __ex_table,\"a\"\n"		\
			PPC_LONG_ALIGN "\n"		\
			PPC_LONG "1b,3b\n"		\
		".previous"				\
		: "=r" (err), "=r" (x)			\
		: "b" (addr), "i" (-EFAULT), "0" (err))
=======
		EX_TABLE(1b, 3b)			\
		: "=r" (err), "=r" (x)			\
		: "m<>" (*addr), "i" (-EFAULT), "0" (err))
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

#ifdef __powerpc64__
#define __get_user_asm2(x, addr, err)			\
	__get_user_asm(x, addr, err, "ld")
#else /* __powerpc64__ */
#define __get_user_asm2(x, addr, err)			\
	__asm__ __volatile__(				\
<<<<<<< HEAD
		"1:	lwz %1,0(%2)\n"			\
		"2:	lwz %1+1,4(%2)\n"		\
=======
		"1:	lwz%X2 %1, %2\n"			\
		"2:	lwz%X2 %L1, %L2\n"		\
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		"3:\n"					\
		".section .fixup,\"ax\"\n"		\
		"4:	li %0,%3\n"			\
		"	li %1,0\n"			\
		"	li %1+1,0\n"			\
		"	b 3b\n"				\
		".previous\n"				\
<<<<<<< HEAD
		".section __ex_table,\"a\"\n"		\
			PPC_LONG_ALIGN "\n"		\
			PPC_LONG "1b,4b\n"		\
			PPC_LONG "2b,4b\n"		\
		".previous"				\
		: "=r" (err), "=&r" (x)			\
		: "b" (addr), "i" (-EFAULT), "0" (err))
#endif /* __powerpc64__ */

#define __get_user_size(x, ptr, size, retval)			\
do {								\
	retval = 0;						\
	__chk_user_ptr(ptr);					\
	if (size > sizeof(x))					\
		(x) = __get_user_bad();				\
	switch (size) {						\
	case 1: __get_user_asm(x, ptr, retval, "lbz"); break;	\
	case 2: __get_user_asm(x, ptr, retval, "lhz"); break;	\
	case 4: __get_user_asm(x, ptr, retval, "lwz"); break;	\
	case 8: __get_user_asm2(x, ptr, retval);  break;	\
	default: (x) = __get_user_bad();			\
	}							\
} while (0)

#define __get_user_nocheck(x, ptr, size)			\
({								\
	long __gu_err;						\
	unsigned long __gu_val;					\
	const __typeof__(*(ptr)) __user *__gu_addr = (ptr);	\
	__chk_user_ptr(ptr);					\
	if (!is_kernel_addr((unsigned long)__gu_addr))		\
		might_sleep();					\
	__get_user_size(__gu_val, __gu_addr, (size), __gu_err);	\
	(x) = (__typeof__(*(ptr)))__gu_val;			\
	__gu_err;						\
})

#ifndef __powerpc64__
#define __get_user64_nocheck(x, ptr, size)			\
({								\
	long __gu_err;						\
	long long __gu_val;					\
	const __typeof__(*(ptr)) __user *__gu_addr = (ptr);	\
	__chk_user_ptr(ptr);					\
	if (!is_kernel_addr((unsigned long)__gu_addr))		\
		might_sleep();					\
	__get_user_size(__gu_val, __gu_addr, (size), __gu_err);	\
	(x) = (__typeof__(*(ptr)))__gu_val;			\
	__gu_err;						\
})
#endif /* __powerpc64__ */

#define __get_user_check(x, ptr, size)					\
({									\
	long __gu_err = -EFAULT;					\
	unsigned long  __gu_val = 0;					\
	const __typeof__(*(ptr)) __user *__gu_addr = (ptr);		\
	might_sleep();							\
	if (access_ok(VERIFY_READ, __gu_addr, (size)))			\
		__get_user_size(__gu_val, __gu_addr, (size), __gu_err);	\
	(x) = (__typeof__(*(ptr)))__gu_val;				\
	__gu_err;							\
})

#define __get_user_nosleep(x, ptr, size)			\
({								\
	long __gu_err;						\
	unsigned long __gu_val;					\
	const __typeof__(*(ptr)) __user *__gu_addr = (ptr);	\
	__chk_user_ptr(ptr);					\
	__get_user_size(__gu_val, __gu_addr, (size), __gu_err);	\
	(x) = (__typeof__(*(ptr)))__gu_val;			\
	__gu_err;						\
})


=======
		EX_TABLE(1b, 4b)			\
		EX_TABLE(2b, 4b)			\
		: "=r" (err), "=&r" (x)			\
		: "m" (*addr), "i" (-EFAULT), "0" (err))
#endif /* __powerpc64__ */

#define __get_user_size_allowed(x, ptr, size, retval)		\
do {								\
	retval = 0;						\
	BUILD_BUG_ON(size > sizeof(x));				\
	switch (size) {						\
	case 1: __get_user_asm(x, (u8 __user *)ptr, retval, "lbz"); break;	\
	case 2: __get_user_asm(x, (u16 __user *)ptr, retval, "lhz"); break;	\
	case 4: __get_user_asm(x, (u32 __user *)ptr, retval, "lwz"); break;	\
	case 8: __get_user_asm2(x, (u64 __user *)ptr, retval);  break;	\
	default: x = 0; BUILD_BUG();				\
	}							\
} while (0)

#define __get_user_size_goto(x, ptr, size, label)		\
do {								\
	long __gus_retval;					\
								\
	__get_user_size_allowed(x, ptr, size, __gus_retval);	\
	if (__gus_retval)					\
		goto label;					\
} while (0)

#endif /* CONFIG_CC_HAS_ASM_GOTO_OUTPUT */

/*
 * This is a type: either unsigned long, if the argument fits into
 * that type, or otherwise unsigned long long.
 */
#define __long_type(x) \
	__typeof__(__builtin_choose_expr(sizeof(x) > sizeof(0UL), 0ULL, 0UL))

#define __get_user(x, ptr)					\
({								\
	long __gu_err;						\
	__long_type(*(ptr)) __gu_val;				\
	__typeof__(*(ptr)) __user *__gu_addr = (ptr);	\
	__typeof__(sizeof(*(ptr))) __gu_size = sizeof(*(ptr));	\
								\
	might_fault();					\
	allow_read_from_user(__gu_addr, __gu_size);		\
	__get_user_size_allowed(__gu_val, __gu_addr, __gu_size, __gu_err);	\
	prevent_read_from_user(__gu_addr, __gu_size);		\
	(x) = (__typeof__(*(ptr)))__gu_val;			\
								\
	__gu_err;						\
})

#define get_user(x, ptr)						\
({									\
	__typeof__(*(ptr)) __user *_gu_addr = (ptr);			\
									\
	access_ok(_gu_addr, sizeof(*(ptr))) ?				\
		  __get_user(x, _gu_addr) :				\
		  ((x) = (__force __typeof__(*(ptr)))0, -EFAULT);	\
})

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
/* more complex routines */

extern unsigned long __copy_tofrom_user(void __user *to,
		const void __user *from, unsigned long size);

<<<<<<< HEAD
#ifndef __powerpc64__

static inline unsigned long copy_from_user(void *to,
		const void __user *from, unsigned long n)
{
	unsigned long over;

	if (access_ok(VERIFY_READ, from, n))
		return __copy_tofrom_user((__force void __user *)to, from, n);
	if ((unsigned long)from < TASK_SIZE) {
		over = (unsigned long)from + n - TASK_SIZE;
		return __copy_tofrom_user((__force void __user *)to, from,
				n - over) + over;
	}
	return n;
}

static inline unsigned long copy_to_user(void __user *to,
		const void *from, unsigned long n)
{
	unsigned long over;

	if (access_ok(VERIFY_WRITE, to, n))
		return __copy_tofrom_user(to, (__force void __user *)from, n);
	if ((unsigned long)to < TASK_SIZE) {
		over = (unsigned long)to + n - TASK_SIZE;
		return __copy_tofrom_user(to, (__force void __user *)from,
				n - over) + over;
	}
	return n;
}

#else /* __powerpc64__ */

#define __copy_in_user(to, from, size) \
	__copy_tofrom_user((to), (from), (size))

extern unsigned long copy_from_user(void *to, const void __user *from,
				    unsigned long n);
extern unsigned long copy_to_user(void __user *to, const void *from,
				  unsigned long n);
extern unsigned long copy_in_user(void __user *to, const void __user *from,
				  unsigned long n);

#endif /* __powerpc64__ */

static inline unsigned long __copy_from_user_inatomic(void *to,
		const void __user *from, unsigned long n)
{
	if (__builtin_constant_p(n) && (n <= 8)) {
		unsigned long ret = 1;

		switch (n) {
		case 1:
			__get_user_size(*(u8 *)to, from, 1, ret);
			break;
		case 2:
			__get_user_size(*(u16 *)to, from, 2, ret);
			break;
		case 4:
			__get_user_size(*(u32 *)to, from, 4, ret);
			break;
		case 8:
			__get_user_size(*(u64 *)to, from, 8, ret);
			break;
		}
		if (ret == 0)
			return 0;
	}
	return __copy_tofrom_user((__force void __user *)to, from, n);
}

static inline unsigned long __copy_to_user_inatomic(void __user *to,
		const void *from, unsigned long n)
{
	if (__builtin_constant_p(n) && (n <= 8)) {
		unsigned long ret = 1;

		switch (n) {
		case 1:
			__put_user_size(*(u8 *)from, (u8 __user *)to, 1, ret);
			break;
		case 2:
			__put_user_size(*(u16 *)from, (u16 __user *)to, 2, ret);
			break;
		case 4:
			__put_user_size(*(u32 *)from, (u32 __user *)to, 4, ret);
			break;
		case 8:
			__put_user_size(*(u64 *)from, (u64 __user *)to, 8, ret);
			break;
		}
		if (ret == 0)
			return 0;
	}
	return __copy_tofrom_user(to, (__force const void __user *)from, n);
}

static inline unsigned long __copy_from_user(void *to,
		const void __user *from, unsigned long size)
{
	might_sleep();
	return __copy_from_user_inatomic(to, from, size);
}

static inline unsigned long __copy_to_user(void __user *to,
		const void *from, unsigned long size)
{
	might_sleep();
	return __copy_to_user_inatomic(to, from, size);
}

extern unsigned long __clear_user(void __user *addr, unsigned long size);

static inline unsigned long clear_user(void __user *addr, unsigned long size)
{
	might_sleep();
	if (likely(access_ok(VERIFY_WRITE, addr, size)))
		return __clear_user(addr, size);
	if ((unsigned long)addr < TASK_SIZE) {
		unsigned long over = (unsigned long)addr + size - TASK_SIZE;
		return __clear_user(addr, size - over) + over;
	}
	return size;
}

extern int __strncpy_from_user(char *dst, const char __user *src, long count);

static inline long strncpy_from_user(char *dst, const char __user *src,
		long count)
{
	might_sleep();
	if (likely(access_ok(VERIFY_READ, src, 1)))
		return __strncpy_from_user(dst, src, count);
	return -EFAULT;
}

/*
 * Return the size of a string (including the ending 0)
 *
 * Return 0 for error
 */
extern int __strnlen_user(const char __user *str, long len, unsigned long top);

/*
 * Returns the length of the string at str (including the null byte),
 * or 0 if we hit a page we can't access,
 * or something > len if we didn't find a null byte.
 *
 * The `top' parameter to __strnlen_user is to make sure that
 * we can never overflow from the user area into kernel space.
 */
static inline int strnlen_user(const char __user *str, long len)
{
	unsigned long top = current->thread.fs.seg;

	if ((unsigned long)str > top)
		return 0;
	return __strnlen_user(str, len, top);
}

#define strlen_user(str)	strnlen_user((str), 0x7ffffffe)

#endif  /* __ASSEMBLY__ */
#endif /* __KERNEL__ */
=======
#ifdef __powerpc64__
static inline unsigned long
raw_copy_in_user(void __user *to, const void __user *from, unsigned long n)
{
	unsigned long ret;

	allow_read_write_user(to, from, n);
	ret = __copy_tofrom_user(to, from, n);
	prevent_read_write_user(to, from, n);
	return ret;
}
#endif /* __powerpc64__ */

static inline unsigned long raw_copy_from_user(void *to,
		const void __user *from, unsigned long n)
{
	unsigned long ret;

	allow_read_from_user(from, n);
	ret = __copy_tofrom_user((__force void __user *)to, from, n);
	prevent_read_from_user(from, n);
	return ret;
}

static inline unsigned long
raw_copy_to_user(void __user *to, const void *from, unsigned long n)
{
	unsigned long ret;

	allow_write_to_user(to, n);
	ret = __copy_tofrom_user(to, (__force const void __user *)from, n);
	prevent_write_to_user(to, n);
	return ret;
}

unsigned long __arch_clear_user(void __user *addr, unsigned long size);

static inline unsigned long __clear_user(void __user *addr, unsigned long size)
{
	unsigned long ret;

	might_fault();
	allow_write_to_user(addr, size);
	ret = __arch_clear_user(addr, size);
	prevent_write_to_user(addr, size);
	return ret;
}

static inline unsigned long clear_user(void __user *addr, unsigned long size)
{
	return likely(access_ok(addr, size)) ? __clear_user(addr, size) : size;
}

extern long strncpy_from_user(char *dst, const char __user *src, long count);
extern __must_check long strnlen_user(const char __user *str, long n);

#ifdef CONFIG_ARCH_HAS_COPY_MC
unsigned long __must_check
copy_mc_generic(void *to, const void *from, unsigned long size);

static inline unsigned long __must_check
copy_mc_to_kernel(void *to, const void *from, unsigned long size)
{
	return copy_mc_generic(to, from, size);
}
#define copy_mc_to_kernel copy_mc_to_kernel

static inline unsigned long __must_check
copy_mc_to_user(void __user *to, const void *from, unsigned long n)
{
	if (check_copy_size(from, n, true)) {
		if (access_ok(to, n)) {
			allow_write_to_user(to, n);
			n = copy_mc_generic((void __force *)to, from, n);
			prevent_write_to_user(to, n);
		}
	}

	return n;
}
#endif

extern long __copy_from_user_flushcache(void *dst, const void __user *src,
		unsigned size);

static __must_check __always_inline bool user_access_begin(const void __user *ptr, size_t len)
{
	if (unlikely(!access_ok(ptr, len)))
		return false;

	might_fault();

	allow_read_write_user((void __user *)ptr, ptr, len);
	return true;
}
#define user_access_begin	user_access_begin
#define user_access_end		prevent_current_access_user
#define user_access_save	prevent_user_access_return
#define user_access_restore	restore_user_access

static __must_check __always_inline bool
user_read_access_begin(const void __user *ptr, size_t len)
{
	if (unlikely(!access_ok(ptr, len)))
		return false;

	might_fault();

	allow_read_from_user(ptr, len);
	return true;
}
#define user_read_access_begin	user_read_access_begin
#define user_read_access_end		prevent_current_read_from_user

static __must_check __always_inline bool
user_write_access_begin(const void __user *ptr, size_t len)
{
	if (unlikely(!access_ok(ptr, len)))
		return false;

	might_fault();

	allow_write_to_user((void __user *)ptr, len);
	return true;
}
#define user_write_access_begin	user_write_access_begin
#define user_write_access_end		prevent_current_write_to_user

#define unsafe_get_user(x, p, e) do {					\
	__long_type(*(p)) __gu_val;				\
	__typeof__(*(p)) __user *__gu_addr = (p);		\
								\
	__get_user_size_goto(__gu_val, __gu_addr, sizeof(*(p)), e); \
	(x) = (__typeof__(*(p)))__gu_val;			\
} while (0)

#define unsafe_put_user(x, p, e) \
	__put_user_size_goto((__typeof__(*(p)))(x), (p), sizeof(*(p)), e)

#define unsafe_copy_from_user(d, s, l, e) \
do {											\
	u8 *_dst = (u8 *)(d);								\
	const u8 __user *_src = (const u8 __user *)(s);					\
	size_t _len = (l);								\
	int _i;										\
											\
	for (_i = 0; _i < (_len & ~(sizeof(u64) - 1)); _i += sizeof(u64))		\
		unsafe_get_user(*(u64 *)(_dst + _i), (u64 __user *)(_src + _i), e);	\
	if (_len & 4) {									\
		unsafe_get_user(*(u32 *)(_dst + _i), (u32 __user *)(_src + _i), e);	\
		_i += 4;								\
	}										\
	if (_len & 2) {									\
		unsafe_get_user(*(u16 *)(_dst + _i), (u16 __user *)(_src + _i), e);	\
		_i += 2;								\
	}										\
	if (_len & 1)									\
		unsafe_get_user(*(u8 *)(_dst + _i), (u8 __user *)(_src + _i), e);	\
} while (0)

#define unsafe_copy_to_user(d, s, l, e) \
do {									\
	u8 __user *_dst = (u8 __user *)(d);				\
	const u8 *_src = (const u8 *)(s);				\
	size_t _len = (l);						\
	int _i;								\
									\
	for (_i = 0; _i < (_len & ~(sizeof(u64) - 1)); _i += sizeof(u64))	\
		unsafe_put_user(*(u64 *)(_src + _i), (u64 __user *)(_dst + _i), e); \
	if (_len & 4) {							\
		unsafe_put_user(*(u32*)(_src + _i), (u32 __user *)(_dst + _i), e); \
		_i += 4;						\
	}								\
	if (_len & 2) {							\
		unsafe_put_user(*(u16*)(_src + _i), (u16 __user *)(_dst + _i), e); \
		_i += 2;						\
	}								\
	if (_len & 1) \
		unsafe_put_user(*(u8*)(_src + _i), (u8 __user *)(_dst + _i), e); \
} while (0)

#define __get_kernel_nofault(dst, src, type, err_label)			\
	__get_user_size_goto(*((type *)(dst)),				\
		(__force type __user *)(src), sizeof(type), err_label)

#define __put_kernel_nofault(dst, src, type, err_label)			\
	__put_user_size_goto(*((type *)(src)),				\
		(__force type __user *)(dst), sizeof(type), err_label)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

#endif	/* _ARCH_POWERPC_UACCESS_H */
