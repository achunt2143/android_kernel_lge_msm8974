<<<<<<< HEAD
#ifndef __PARISC_BARRIER_H
#define __PARISC_BARRIER_H

/*
** This is simply the barrier() macro from linux/kernel.h but when serial.c
** uses tqueue.h uses smp_mb() defined using barrier(), linux/kernel.h
** hasn't yet been included yet so it fails, thus repeating the macro here.
**
** PA-RISC architecture allows for weakly ordered memory accesses although
** none of the processors use it. There is a strong ordered bit that is
** set in the O-bit of the page directory entry. Operating systems that
** can not tolerate out of order accesses should set this bit when mapping
** pages. The O-bit of the PSW should also be set to 1 (I don't believe any
** of the processor implemented the PSW O-bit). The PCX-W ERS states that
** the TLB O-bit is not implemented so the page directory does not need to
** have the O-bit set when mapping pages (section 3.1). This section also
** states that the PSW Y, Z, G, and O bits are not implemented.
** So it looks like nothing needs to be done for parisc-linux (yet).
** (thanks to chada for the above comment -ggg)
**
** The __asm__ op below simple prevents gcc/ld from reordering
** instructions across the mb() "call".
*/
#define mb()		__asm__ __volatile__("":::"memory")	/* barrier() */
#define rmb()		mb()
#define wmb()		mb()
#define smp_mb()	mb()
#define smp_rmb()	mb()
#define smp_wmb()	mb()
#define smp_read_barrier_depends()	do { } while(0)
#define read_barrier_depends()		do { } while(0)

#define set_mb(var, value)		do { var = value; mb(); } while (0)

#endif /* __PARISC_BARRIER_H */
=======
/* SPDX-License-Identifier: GPL-2.0 */
#ifndef __ASM_BARRIER_H
#define __ASM_BARRIER_H

#include <asm/alternative.h>

#ifndef __ASSEMBLY__

/* The synchronize caches instruction executes as a nop on systems in
   which all memory references are performed in order. */
#define synchronize_caches() asm volatile("sync" \
	ALTERNATIVE(ALT_COND_NO_SMP, INSN_NOP) \
	: : : "memory")

#if defined(CONFIG_SMP)
#define mb()		do { synchronize_caches(); } while (0)
#define rmb()		mb()
#define wmb()		mb()
#define dma_rmb()	mb()
#define dma_wmb()	mb()
#else
#define mb()		barrier()
#define rmb()		barrier()
#define wmb()		barrier()
#define dma_rmb()	barrier()
#define dma_wmb()	barrier()
#endif

#define __smp_mb()	mb()
#define __smp_rmb()	mb()
#define __smp_wmb()	mb()

#define __smp_store_release(p, v)					\
do {									\
	typeof(p) __p = (p);						\
        union { typeof(*p) __val; char __c[1]; } __u =			\
                { .__val = (__force typeof(*p)) (v) };			\
	compiletime_assert_atomic_type(*p);				\
	switch (sizeof(*p)) {						\
	case 1:								\
		asm volatile("stb,ma %0,0(%1)"				\
				: : "r"(*(__u8 *)__u.__c), "r"(__p)	\
				: "memory");				\
		break;							\
	case 2:								\
		asm volatile("sth,ma %0,0(%1)"				\
				: : "r"(*(__u16 *)__u.__c), "r"(__p)	\
				: "memory");				\
		break;							\
	case 4:								\
		asm volatile("stw,ma %0,0(%1)"				\
				: : "r"(*(__u32 *)__u.__c), "r"(__p)	\
				: "memory");				\
		break;							\
	case 8:								\
		if (IS_ENABLED(CONFIG_64BIT))				\
			asm volatile("std,ma %0,0(%1)"			\
				: : "r"(*(__u64 *)__u.__c), "r"(__p)	\
				: "memory");				\
		break;							\
	}								\
} while (0)

#define __smp_load_acquire(p)						\
({									\
	union { typeof(*p) __val; char __c[1]; } __u;			\
	typeof(p) __p = (p);						\
	compiletime_assert_atomic_type(*p);				\
	switch (sizeof(*p)) {						\
	case 1:								\
		asm volatile("ldb,ma 0(%1),%0"				\
				: "=r"(*(__u8 *)__u.__c) : "r"(__p)	\
				: "memory");				\
		break;							\
	case 2:								\
		asm volatile("ldh,ma 0(%1),%0"				\
				: "=r"(*(__u16 *)__u.__c) : "r"(__p)	\
				: "memory");				\
		break;							\
	case 4:								\
		asm volatile("ldw,ma 0(%1),%0"				\
				: "=r"(*(__u32 *)__u.__c) : "r"(__p)	\
				: "memory");				\
		break;							\
	case 8:								\
		if (IS_ENABLED(CONFIG_64BIT))				\
			asm volatile("ldd,ma 0(%1),%0"			\
				: "=r"(*(__u64 *)__u.__c) : "r"(__p)	\
				: "memory");				\
		break;							\
	}								\
	__u.__val;							\
})
#include <asm-generic/barrier.h>

#endif /* !__ASSEMBLY__ */
#endif /* __ASM_BARRIER_H */
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
