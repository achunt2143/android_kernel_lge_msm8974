<<<<<<< HEAD
#ifndef __SPARC_BARRIER_H
#define __SPARC_BARRIER_H

/* XXX Change this if we ever use a PSO mode kernel. */
#define mb()	__asm__ __volatile__ ("" : : : "memory")
#define rmb()	mb()
#define wmb()	mb()
#define read_barrier_depends()	do { } while(0)
#define set_mb(__var, __value)  do { __var = __value; mb(); } while(0)
#define smp_mb()	__asm__ __volatile__("":::"memory")
#define smp_rmb()	__asm__ __volatile__("":::"memory")
#define smp_wmb()	__asm__ __volatile__("":::"memory")
#define smp_read_barrier_depends()	do { } while(0)
=======
/* SPDX-License-Identifier: GPL-2.0 */
#ifndef __SPARC_BARRIER_H
#define __SPARC_BARRIER_H

#include <asm-generic/barrier.h>
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

#endif /* !(__SPARC_BARRIER_H) */
