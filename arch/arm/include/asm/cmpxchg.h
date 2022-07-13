<<<<<<< HEAD
=======
/* SPDX-License-Identifier: GPL-2.0 */
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#ifndef __ASM_ARM_CMPXCHG_H
#define __ASM_ARM_CMPXCHG_H

#include <linux/irqflags.h>
<<<<<<< HEAD
=======
#include <linux/prefetch.h>
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#include <asm/barrier.h>

#if defined(CONFIG_CPU_SA1100) || defined(CONFIG_CPU_SA110)
/*
 * On the StrongARM, "swp" is terminally broken since it bypasses the
 * cache totally.  This means that the cache becomes inconsistent, and,
 * since we use normal loads/stores as well, this is really bad.
 * Typically, this causes oopsen in filp_close, but could have other,
 * more disastrous effects.  There are two work-arounds:
 *  1. Disable interrupts and emulate the atomic swap
 *  2. Clean the cache, perform atomic swap, flush the cache
 *
 * We choose (1) since its the "easiest" to achieve here and is not
 * dependent on the processor type.
 *
 * NOTE that this solution won't work on an SMP system, so explcitly
 * forbid it here.
 */
#define swp_is_buggy
#endif

<<<<<<< HEAD
static inline unsigned long __xchg(unsigned long x, volatile void *ptr, int size)
=======
static inline unsigned long
__arch_xchg(unsigned long x, volatile void *ptr, int size)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
{
	extern void __bad_xchg(volatile void *, int);
	unsigned long ret;
#ifdef swp_is_buggy
	unsigned long flags;
#endif
#if __LINUX_ARM_ARCH__ >= 6
	unsigned int tmp;
#endif

<<<<<<< HEAD
	smp_mb();

	switch (size) {
#if __LINUX_ARM_ARCH__ >= 6
=======
	prefetchw((const void *)ptr);

	switch (size) {
#if __LINUX_ARM_ARCH__ >= 6
#ifndef CONFIG_CPU_V6 /* MIN ARCH >= V6K */
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	case 1:
		asm volatile("@	__xchg1\n"
		"1:	ldrexb	%0, [%3]\n"
		"	strexb	%1, %2, [%3]\n"
		"	teq	%1, #0\n"
		"	bne	1b"
			: "=&r" (ret), "=&r" (tmp)
			: "r" (x), "r" (ptr)
			: "memory", "cc");
		break;
<<<<<<< HEAD
=======
	case 2:
		asm volatile("@	__xchg2\n"
		"1:	ldrexh	%0, [%3]\n"
		"	strexh	%1, %2, [%3]\n"
		"	teq	%1, #0\n"
		"	bne	1b"
			: "=&r" (ret), "=&r" (tmp)
			: "r" (x), "r" (ptr)
			: "memory", "cc");
		break;
#endif
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	case 4:
		asm volatile("@	__xchg4\n"
		"1:	ldrex	%0, [%3]\n"
		"	strex	%1, %2, [%3]\n"
		"	teq	%1, #0\n"
		"	bne	1b"
			: "=&r" (ret), "=&r" (tmp)
			: "r" (x), "r" (ptr)
			: "memory", "cc");
		break;
#elif defined(swp_is_buggy)
#ifdef CONFIG_SMP
#error SMP is not supported on this platform
#endif
	case 1:
		raw_local_irq_save(flags);
		ret = *(volatile unsigned char *)ptr;
		*(volatile unsigned char *)ptr = x;
		raw_local_irq_restore(flags);
		break;

	case 4:
		raw_local_irq_save(flags);
		ret = *(volatile unsigned long *)ptr;
		*(volatile unsigned long *)ptr = x;
		raw_local_irq_restore(flags);
		break;
#else
	case 1:
		asm volatile("@	__xchg1\n"
		"	swpb	%0, %1, [%2]"
			: "=&r" (ret)
			: "r" (x), "r" (ptr)
			: "memory", "cc");
		break;
	case 4:
		asm volatile("@	__xchg4\n"
		"	swp	%0, %1, [%2]"
			: "=&r" (ret)
			: "r" (x), "r" (ptr)
			: "memory", "cc");
		break;
#endif
	default:
<<<<<<< HEAD
		__bad_xchg(ptr, size), ret = 0;
		break;
	}
	smp_mb();
=======
		/* Cause a link-time error, the xchg() size is not supported */
		__bad_xchg(ptr, size), ret = 0;
		break;
	}
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	return ret;
}

<<<<<<< HEAD
#define xchg(ptr,x) \
	((__typeof__(*(ptr)))__xchg((unsigned long)(x),(ptr),sizeof(*(ptr))))
=======
#define arch_xchg_relaxed(ptr, x) ({					\
	(__typeof__(*(ptr)))__arch_xchg((unsigned long)(x), (ptr),	\
					sizeof(*(ptr)));		\
})
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

#include <asm-generic/cmpxchg-local.h>

#if __LINUX_ARM_ARCH__ < 6
/* min ARCH < ARMv6 */

#ifdef CONFIG_SMP
#error "SMP is not supported on this platform"
#endif

<<<<<<< HEAD
=======
#define arch_xchg arch_xchg_relaxed

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
/*
 * cmpxchg_local and cmpxchg64_local are atomic wrt current CPU. Always make
 * them available.
 */
<<<<<<< HEAD
#define cmpxchg_local(ptr, o, n)				  	       \
	((__typeof__(*(ptr)))__cmpxchg_local_generic((ptr), (unsigned long)(o),\
			(unsigned long)(n), sizeof(*(ptr))))
#define cmpxchg64_local(ptr, o, n) __cmpxchg64_local_generic((ptr), (o), (n))

#ifndef CONFIG_SMP
#include <asm-generic/cmpxchg.h>
#endif
=======
#define arch_cmpxchg_local(ptr, o, n) ({				\
	(__typeof(*ptr))__generic_cmpxchg_local((ptr),			\
					        (unsigned long)(o),	\
					        (unsigned long)(n),	\
					        sizeof(*(ptr)));	\
})

#define arch_cmpxchg64_local(ptr, o, n) __generic_cmpxchg64_local((ptr), (o), (n))

#include <asm-generic/cmpxchg.h>
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

#else	/* min ARCH >= ARMv6 */

extern void __bad_cmpxchg(volatile void *ptr, int size);

/*
 * cmpxchg only support 32-bits operands on ARMv6.
 */

static inline unsigned long __cmpxchg(volatile void *ptr, unsigned long old,
				      unsigned long new, int size)
{
	unsigned long oldval, res;

<<<<<<< HEAD
=======
	prefetchw((const void *)ptr);

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	switch (size) {
#ifndef CONFIG_CPU_V6	/* min ARCH >= ARMv6K */
	case 1:
		do {
			asm volatile("@ __cmpxchg1\n"
			"	ldrexb	%1, [%2]\n"
			"	mov	%0, #0\n"
			"	teq	%1, %3\n"
			"	strexbeq %0, %4, [%2]\n"
				: "=&r" (res), "=&r" (oldval)
				: "r" (ptr), "Ir" (old), "r" (new)
				: "memory", "cc");
		} while (res);
		break;
	case 2:
		do {
			asm volatile("@ __cmpxchg1\n"
			"	ldrexh	%1, [%2]\n"
			"	mov	%0, #0\n"
			"	teq	%1, %3\n"
			"	strexheq %0, %4, [%2]\n"
				: "=&r" (res), "=&r" (oldval)
				: "r" (ptr), "Ir" (old), "r" (new)
				: "memory", "cc");
		} while (res);
		break;
#endif
	case 4:
		do {
			asm volatile("@ __cmpxchg4\n"
			"	ldrex	%1, [%2]\n"
			"	mov	%0, #0\n"
			"	teq	%1, %3\n"
			"	strexeq %0, %4, [%2]\n"
				: "=&r" (res), "=&r" (oldval)
				: "r" (ptr), "Ir" (old), "r" (new)
				: "memory", "cc");
		} while (res);
		break;
	default:
		__bad_cmpxchg(ptr, size);
		oldval = 0;
	}

	return oldval;
}

<<<<<<< HEAD
static inline unsigned long __cmpxchg_mb(volatile void *ptr, unsigned long old,
					 unsigned long new, int size)
{
	unsigned long ret;

	smp_mb();
	ret = __cmpxchg(ptr, old, new, size);
	smp_mb();

	return ret;
}

#define cmpxchg(ptr,o,n)						\
	((__typeof__(*(ptr)))__cmpxchg_mb((ptr),			\
					  (unsigned long)(o),		\
					  (unsigned long)(n),		\
					  sizeof(*(ptr))))
=======
#define arch_cmpxchg_relaxed(ptr,o,n) ({				\
	(__typeof__(*(ptr)))__cmpxchg((ptr),				\
				      (unsigned long)(o),		\
				      (unsigned long)(n),		\
				      sizeof(*(ptr)));			\
})
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

static inline unsigned long __cmpxchg_local(volatile void *ptr,
					    unsigned long old,
					    unsigned long new, int size)
{
	unsigned long ret;

	switch (size) {
#ifdef CONFIG_CPU_V6	/* min ARCH == ARMv6 */
	case 1:
	case 2:
<<<<<<< HEAD
		ret = __cmpxchg_local_generic(ptr, old, new, size);
=======
		ret = __generic_cmpxchg_local(ptr, old, new, size);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		break;
#endif
	default:
		ret = __cmpxchg(ptr, old, new, size);
	}

	return ret;
}

<<<<<<< HEAD
#define cmpxchg_local(ptr,o,n)						\
	((__typeof__(*(ptr)))__cmpxchg_local((ptr),			\
				       (unsigned long)(o),		\
				       (unsigned long)(n),		\
				       sizeof(*(ptr))))

#ifndef CONFIG_CPU_V6	/* min ARCH >= ARMv6K */

/*
 * Note : ARMv7-M (currently unsupported by Linux) does not support
 * ldrexd/strexd. If ARMv7-M is ever supported by the Linux kernel, it should
 * not be allowed to use __cmpxchg64.
 */
static inline unsigned long long __cmpxchg64(volatile void *ptr,
					     unsigned long long old,
					     unsigned long long new)
{
	register unsigned long long oldval asm("r0");
	register unsigned long long __old asm("r2") = old;
	register unsigned long long __new asm("r4") = new;
	unsigned long res;

	do {
		asm volatile(
		"	@ __cmpxchg8\n"
		"	ldrexd	%1, %H1, [%2]\n"
		"	mov	%0, #0\n"
		"	teq	%1, %3\n"
		"	teqeq	%H1, %H3\n"
		"	strexdeq %0, %4, %H4, [%2]\n"
			: "=&r" (res), "=&r" (oldval)
			: "r" (ptr), "Ir" (__old), "r" (__new)
			: "memory", "cc");
	} while (res);
=======
#define arch_cmpxchg_local(ptr, o, n) ({				\
	(__typeof(*ptr))__cmpxchg_local((ptr),				\
				        (unsigned long)(o),		\
				        (unsigned long)(n),		\
				        sizeof(*(ptr)));		\
})

static inline unsigned long long __cmpxchg64(unsigned long long *ptr,
					     unsigned long long old,
					     unsigned long long new)
{
	unsigned long long oldval;
	unsigned long res;

	prefetchw(ptr);

	__asm__ __volatile__(
"1:	ldrexd		%1, %H1, [%3]\n"
"	teq		%1, %4\n"
"	teqeq		%H1, %H4\n"
"	bne		2f\n"
"	strexd		%0, %5, %H5, [%3]\n"
"	teq		%0, #0\n"
"	bne		1b\n"
"2:"
	: "=&r" (res), "=&r" (oldval), "+Qo" (*ptr)
	: "r" (ptr), "r" (old), "r" (new)
	: "cc");
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	return oldval;
}

<<<<<<< HEAD
static inline unsigned long long __cmpxchg64_mb(volatile void *ptr,
						unsigned long long old,
						unsigned long long new)
{
	unsigned long long ret;

	smp_mb();
	ret = __cmpxchg64(ptr, old, new);
	smp_mb();

	return ret;
}

#define cmpxchg64(ptr,o,n)						\
	((__typeof__(*(ptr)))__cmpxchg64_mb((ptr),			\
					    (unsigned long long)(o),	\
					    (unsigned long long)(n)))

#define cmpxchg64_local(ptr,o,n)					\
	((__typeof__(*(ptr)))__cmpxchg64((ptr),				\
					 (unsigned long long)(o),	\
					 (unsigned long long)(n)))

#else /* min ARCH = ARMv6 */

#define cmpxchg64_local(ptr, o, n) __cmpxchg64_local_generic((ptr), (o), (n))

#endif
=======
#define arch_cmpxchg64_relaxed(ptr, o, n) ({				\
	(__typeof__(*(ptr)))__cmpxchg64((ptr),				\
					(unsigned long long)(o),	\
					(unsigned long long)(n));	\
})

#define arch_cmpxchg64_local(ptr, o, n) arch_cmpxchg64_relaxed((ptr), (o), (n))
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

#endif	/* __LINUX_ARM_ARCH__ >= 6 */

#endif /* __ASM_ARM_CMPXCHG_H */
