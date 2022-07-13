/*
 * This file is subject to the terms and conditions of the GNU General Public
 * License.  See the file "COPYING" in the main directory of this archive
 * for more details.
 *
 * Copyright (C) 2006 by Ralf Baechle (ralf@linux-mips.org)
 */
#ifndef __ASM_BARRIER_H
#define __ASM_BARRIER_H

#include <asm/addrspace.h>
<<<<<<< HEAD

/*
 * read_barrier_depends - Flush all pending reads that subsequents reads
 * depend on.
 *
 * No data-dependent reads from memory-like regions are ever reordered
 * over this barrier.  All reads preceding this primitive are guaranteed
 * to access memory (but not necessarily other CPUs' caches) before any
 * reads following this primitive that depend on the data return by
 * any of the preceding reads.  This primitive is much lighter weight than
 * rmb() on most CPUs, and is never heavier weight than is
 * rmb().
 *
 * These ordering constraints are respected by both the local CPU
 * and the compiler.
 *
 * Ordering is not guaranteed by anything other than these primitives,
 * not even by data dependencies.  See the documentation for
 * memory_barrier() for examples and URLs to more information.
 *
 * For example, the following code would force ordering (the initial
 * value of "a" is zero, "b" is one, and "p" is "&a"):
 *
 * <programlisting>
 *	CPU 0				CPU 1
 *
 *	b = 2;
 *	memory_barrier();
 *	p = &b;				q = p;
 *					read_barrier_depends();
 *					d = *q;
 * </programlisting>
 *
 * because the read of "*q" depends on the read of "p" and these
 * two reads are separated by a read_barrier_depends().  However,
 * the following code, with the same initial values for "a" and "b":
 *
 * <programlisting>
 *	CPU 0				CPU 1
 *
 *	a = 2;
 *	memory_barrier();
 *	b = 3;				y = b;
 *					read_barrier_depends();
 *					x = a;
 * </programlisting>
 *
 * does not enforce ordering, since there is no data dependency between
 * the read of "a" and the read of "b".  Therefore, on some CPUs, such
 * as Alpha, "y" could be set to 3 and "x" to 0.  Use rmb()
 * in cases like this where there are no data dependencies.
 */

#define read_barrier_depends()		do { } while(0)
#define smp_read_barrier_depends()	do { } while(0)

#ifdef CONFIG_CPU_HAS_SYNC
#define __sync()				\
	__asm__ __volatile__(			\
		".set	push\n\t"		\
		".set	noreorder\n\t"		\
		".set	mips2\n\t"		\
		"sync\n\t"			\
		".set	pop"			\
		: /* no output */		\
		: /* no input */		\
		: "memory")
#else
#define __sync()	do { } while(0)
#endif
=======
#include <asm/sync.h>

static inline void __sync(void)
{
	asm volatile(__SYNC(full, always) ::: "memory");
}

static inline void rmb(void)
{
	asm volatile(__SYNC(rmb, always) ::: "memory");
}
#define rmb rmb

static inline void wmb(void)
{
	asm volatile(__SYNC(wmb, always) ::: "memory");
}
#define wmb wmb

#define fast_mb()	__sync()
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

#define __fast_iob()				\
	__asm__ __volatile__(			\
		".set	push\n\t"		\
		".set	noreorder\n\t"		\
		"lw	$0,%0\n\t"		\
		"nop\n\t"			\
		".set	pop"			\
		: /* no output */		\
		: "m" (*(int *)CKSEG1)		\
		: "memory")
#ifdef CONFIG_CPU_CAVIUM_OCTEON
<<<<<<< HEAD
# define OCTEON_SYNCW_STR	".set push\n.set arch=octeon\nsyncw\nsyncw\n.set pop\n"
# define __syncw() 	__asm__ __volatile__(OCTEON_SYNCW_STR : : : "memory")

# define fast_wmb()	__syncw()
# define fast_rmb()	barrier()
# define fast_mb()	__sync()
# define fast_iob()	do { } while (0)
#else /* ! CONFIG_CPU_CAVIUM_OCTEON */
# define fast_wmb()	__sync()
# define fast_rmb()	__sync()
# define fast_mb()	__sync()
=======
# define fast_iob()	do { } while (0)
#else /* ! CONFIG_CPU_CAVIUM_OCTEON */
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
# ifdef CONFIG_SGI_IP28
#  define fast_iob()				\
	__asm__ __volatile__(			\
		".set	push\n\t"		\
		".set	noreorder\n\t"		\
		"lw	$0,%0\n\t"		\
		"sync\n\t"			\
		"lw	$0,%0\n\t"		\
		".set	pop"			\
		: /* no output */		\
		: "m" (*(int *)CKSEG1ADDR(0x1fa00004)) \
		: "memory")
# else
#  define fast_iob()				\
	do {					\
		__sync();			\
		__fast_iob();			\
	} while (0)
# endif
#endif /* CONFIG_CPU_CAVIUM_OCTEON */

#ifdef CONFIG_CPU_HAS_WB

#include <asm/wbflush.h>

<<<<<<< HEAD
#define wmb()		fast_wmb()
#define rmb()		fast_rmb()
=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#define mb()		wbflush()
#define iob()		wbflush()

#else /* !CONFIG_CPU_HAS_WB */

<<<<<<< HEAD
#define wmb()		fast_wmb()
#define rmb()		fast_rmb()
=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#define mb()		fast_mb()
#define iob()		fast_iob()

#endif /* !CONFIG_CPU_HAS_WB */

<<<<<<< HEAD
#if defined(CONFIG_WEAK_ORDERING) && defined(CONFIG_SMP)
# ifdef CONFIG_CPU_CAVIUM_OCTEON
#  define smp_mb()	__sync()
#  define smp_rmb()	barrier()
#  define smp_wmb()	__syncw()
# else
#  define smp_mb()	__asm__ __volatile__("sync" : : :"memory")
#  define smp_rmb()	__asm__ __volatile__("sync" : : :"memory")
#  define smp_wmb()	__asm__ __volatile__("sync" : : :"memory")
# endif
#else
#define smp_mb()	barrier()
#define smp_rmb()	barrier()
#define smp_wmb()	barrier()
#endif

#if defined(CONFIG_WEAK_REORDERING_BEYOND_LLSC) && defined(CONFIG_SMP)
#define __WEAK_LLSC_MB		"       sync	\n"
#else
#define __WEAK_LLSC_MB		"		\n"
#endif

#define set_mb(var, value) \
	do { var = value; smp_mb(); } while (0)

#define smp_llsc_mb()	__asm__ __volatile__(__WEAK_LLSC_MB : : :"memory")

#ifdef CONFIG_CPU_CAVIUM_OCTEON
#define smp_mb__before_llsc() smp_wmb()
=======
#if defined(CONFIG_WEAK_ORDERING)
# define __smp_mb()	__sync()
# define __smp_rmb()	rmb()
# define __smp_wmb()	wmb()
#else
# define __smp_mb()	barrier()
# define __smp_rmb()	barrier()
# define __smp_wmb()	barrier()
#endif

/*
 * When LL/SC does imply order, it must also be a compiler barrier to avoid the
 * compiler from reordering where the CPU will not. When it does not imply
 * order, the compiler is also free to reorder across the LL/SC loop and
 * ordering will be done by smp_llsc_mb() and friends.
 */
#if defined(CONFIG_WEAK_REORDERING_BEYOND_LLSC) && defined(CONFIG_SMP)
# define __WEAK_LLSC_MB		sync
# define smp_llsc_mb() \
	__asm__ __volatile__(__stringify(__WEAK_LLSC_MB) : : :"memory")
# define __LLSC_CLOBBER
#else
# define __WEAK_LLSC_MB
# define smp_llsc_mb()		do { } while (0)
# define __LLSC_CLOBBER		"memory"
#endif

#ifdef CONFIG_CPU_CAVIUM_OCTEON
#define smp_mb__before_llsc() smp_wmb()
#define __smp_mb__before_llsc() __smp_wmb()
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
/* Cause previous writes to become visible on all CPUs as soon as possible */
#define nudge_writes() __asm__ __volatile__(".set push\n\t"		\
					    ".set arch=octeon\n\t"	\
					    "syncw\n\t"			\
					    ".set pop" : : : "memory")
#else
#define smp_mb__before_llsc() smp_llsc_mb()
<<<<<<< HEAD
#define nudge_writes() mb()
#endif

=======
#define __smp_mb__before_llsc() smp_llsc_mb()
#define nudge_writes() mb()
#endif

/*
 * In the Loongson3 LL/SC workaround case, all of our LL/SC loops already have
 * a completion barrier immediately preceding the LL instruction. Therefore we
 * can skip emitting a barrier from __smp_mb__before_atomic().
 */
#ifdef CONFIG_CPU_LOONGSON3_WORKAROUNDS
# define __smp_mb__before_atomic()
#else
# define __smp_mb__before_atomic()	__smp_mb__before_llsc()
#endif

#define __smp_mb__after_atomic()	smp_llsc_mb()

static inline void sync_ginv(void)
{
	asm volatile(__SYNC(ginv, always));
}

#include <asm-generic/barrier.h>

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#endif /* __ASM_BARRIER_H */
