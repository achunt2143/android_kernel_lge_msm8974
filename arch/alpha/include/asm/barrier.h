<<<<<<< HEAD
#ifndef __BARRIER_H
#define __BARRIER_H

#include <asm/compiler.h>

#define mb() \
__asm__ __volatile__("mb": : :"memory")

#define rmb() \
__asm__ __volatile__("mb": : :"memory")

#define wmb() \
__asm__ __volatile__("wmb": : :"memory")

#define read_barrier_depends() \
__asm__ __volatile__("mb": : :"memory")

#ifdef CONFIG_SMP
#define __ASM_SMP_MB	"\tmb\n"
#define smp_mb()	mb()
#define smp_rmb()	rmb()
#define smp_wmb()	wmb()
#define smp_read_barrier_depends()	read_barrier_depends()
#else
#define __ASM_SMP_MB
#define smp_mb()	barrier()
#define smp_rmb()	barrier()
#define smp_wmb()	barrier()
#define smp_read_barrier_depends()	do { } while (0)
#endif

#define set_mb(var, value) \
do { var = value; mb(); } while (0)
=======
/* SPDX-License-Identifier: GPL-2.0 */
#ifndef __BARRIER_H
#define __BARRIER_H

#define mb()	__asm__ __volatile__("mb": : :"memory")
#define rmb()	__asm__ __volatile__("mb": : :"memory")
#define wmb()	__asm__ __volatile__("wmb": : :"memory")

#define __smp_load_acquire(p)						\
({									\
	compiletime_assert_atomic_type(*p);				\
	__READ_ONCE(*p);						\
})

#ifdef CONFIG_SMP
#define __ASM_SMP_MB	"\tmb\n"
#else
#define __ASM_SMP_MB
#endif

#include <asm-generic/barrier.h>
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

#endif		/* __BARRIER_H */
