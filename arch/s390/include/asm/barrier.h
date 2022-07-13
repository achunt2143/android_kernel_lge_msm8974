<<<<<<< HEAD
=======
/* SPDX-License-Identifier: GPL-2.0 */
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
/*
 * Copyright IBM Corp. 1999, 2009
 *
 * Author(s): Martin Schwidefsky <schwidefsky@de.ibm.com>
 */

#ifndef __ASM_BARRIER_H
#define __ASM_BARRIER_H

/*
 * Force strict CPU ordering.
 * And yes, this is required on UP too when we're talking
 * to devices.
<<<<<<< HEAD
 *
 * This is very similar to the ppc eieio/sync instruction in that is
 * does a checkpoint syncronisation & makes sure that 
 * all memory ops have completed wrt other CPU's ( see 7-15 POP  DJB ).
 */

#define eieio()	asm volatile("bcr 15,0" : : : "memory")
#define SYNC_OTHER_CORES(x)   eieio()
#define mb()    eieio()
#define rmb()   eieio()
#define wmb()   eieio()
#define read_barrier_depends() do { } while(0)
#define smp_mb()       mb()
#define smp_rmb()      rmb()
#define smp_wmb()      wmb()
#define smp_read_barrier_depends()    read_barrier_depends()
#define smp_mb__before_clear_bit()     smp_mb()
#define smp_mb__after_clear_bit()      smp_mb()

#define set_mb(var, value)      do { var = value; mb(); } while (0)
=======
 */

#ifdef CONFIG_HAVE_MARCH_Z196_FEATURES
/* Fast-BCR without checkpoint synchronization */
#define __ASM_BCR_SERIALIZE "bcr 14,0\n"
#else
#define __ASM_BCR_SERIALIZE "bcr 15,0\n"
#endif

static __always_inline void bcr_serialize(void)
{
	asm volatile(__ASM_BCR_SERIALIZE : : : "memory");
}

#define __mb()		bcr_serialize()
#define __rmb()		barrier()
#define __wmb()		barrier()
#define __dma_rmb()	__mb()
#define __dma_wmb()	__mb()
#define __smp_mb()	__mb()
#define __smp_rmb()	__rmb()
#define __smp_wmb()	__wmb()

#define __smp_store_release(p, v)					\
do {									\
	compiletime_assert_atomic_type(*p);				\
	barrier();							\
	WRITE_ONCE(*p, v);						\
} while (0)

#define __smp_load_acquire(p)						\
({									\
	typeof(*p) ___p1 = READ_ONCE(*p);				\
	compiletime_assert_atomic_type(*p);				\
	barrier();							\
	___p1;								\
})

#define __smp_mb__before_atomic()	barrier()
#define __smp_mb__after_atomic()	barrier()

/**
 * array_index_mask_nospec - generate a mask for array_idx() that is
 * ~0UL when the bounds check succeeds and 0 otherwise
 * @index: array element index
 * @size: number of elements in array
 */
#define array_index_mask_nospec array_index_mask_nospec
static inline unsigned long array_index_mask_nospec(unsigned long index,
						    unsigned long size)
{
	unsigned long mask;

	if (__builtin_constant_p(size) && size > 0) {
		asm("	clgr	%2,%1\n"
		    "	slbgr	%0,%0\n"
		    :"=d" (mask) : "d" (size-1), "d" (index) :"cc");
		return mask;
	}
	asm("	clgr	%1,%2\n"
	    "	slbgr	%0,%0\n"
	    :"=d" (mask) : "d" (size), "d" (index) :"cc");
	return ~mask;
}

#include <asm-generic/barrier.h>
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

#endif /* __ASM_BARRIER_H */
