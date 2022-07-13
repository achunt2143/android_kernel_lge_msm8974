<<<<<<< HEAD
#ifndef __ARCH_S390_ATOMIC__
#define __ARCH_S390_ATOMIC__

/*
 * Copyright 1999,2009 IBM Corp.
 * Author(s): Martin Schwidefsky <schwidefsky@de.ibm.com>,
 *	      Denis Joseph Barrow,
 *	      Arnd Bergmann <arndb@de.ibm.com>,
 *
 * Atomic operations that C can't guarantee us.
 * Useful for resource counting etc.
 * s390 uses 'Compare And Swap' for atomicity in SMP environment.
 *
 */

#include <linux/compiler.h>
#include <linux/types.h>
#include <asm/cmpxchg.h>

#define ATOMIC_INIT(i)  { (i) }

#define __CS_LOOP(ptr, op_val, op_string) ({				\
	int old_val, new_val;						\
	asm volatile(							\
		"	l	%0,%2\n"				\
		"0:	lr	%1,%0\n"				\
		op_string "	%1,%3\n"				\
		"	cs	%0,%1,%2\n"				\
		"	jl	0b"					\
		: "=&d" (old_val), "=&d" (new_val),			\
		  "=Q" (((atomic_t *)(ptr))->counter)			\
		: "d" (op_val),	 "Q" (((atomic_t *)(ptr))->counter)	\
		: "cc", "memory");					\
	new_val;							\
})

static inline int atomic_read(const atomic_t *v)
{
	int c;

	asm volatile(
		"	l	%0,%1\n"
		: "=d" (c) : "Q" (v->counter));
	return c;
}

static inline void atomic_set(atomic_t *v, int i)
{
	asm volatile(
		"	st	%1,%0\n"
		: "=Q" (v->counter) : "d" (i));
}

static inline int atomic_add_return(int i, atomic_t *v)
{
	return __CS_LOOP(v, i, "ar");
}
#define atomic_add(_i, _v)		atomic_add_return(_i, _v)
#define atomic_add_negative(_i, _v)	(atomic_add_return(_i, _v) < 0)
#define atomic_inc(_v)			atomic_add_return(1, _v)
#define atomic_inc_return(_v)		atomic_add_return(1, _v)
#define atomic_inc_and_test(_v)		(atomic_add_return(1, _v) == 0)

static inline int atomic_sub_return(int i, atomic_t *v)
{
	return __CS_LOOP(v, i, "sr");
}
#define atomic_sub(_i, _v)		atomic_sub_return(_i, _v)
#define atomic_sub_and_test(_i, _v)	(atomic_sub_return(_i, _v) == 0)
#define atomic_dec(_v)			atomic_sub_return(1, _v)
#define atomic_dec_return(_v)		atomic_sub_return(1, _v)
#define atomic_dec_and_test(_v)		(atomic_sub_return(1, _v) == 0)

static inline void atomic_clear_mask(unsigned long mask, atomic_t *v)
{
	__CS_LOOP(v, ~mask, "nr");
}

static inline void atomic_set_mask(unsigned long mask, atomic_t *v)
{
	__CS_LOOP(v, mask, "or");
}

#define atomic_xchg(v, new) (xchg(&((v)->counter), new))

static inline int atomic_cmpxchg(atomic_t *v, int old, int new)
{
	asm volatile(
		"	cs	%0,%2,%1"
		: "+d" (old), "=Q" (v->counter)
		: "d" (new), "Q" (v->counter)
		: "cc", "memory");
	return old;
}

static inline int __atomic_add_unless(atomic_t *v, int a, int u)
{
	int c, old;
	c = atomic_read(v);
	for (;;) {
		if (unlikely(c == u))
			break;
		old = atomic_cmpxchg(v, c, c + a);
		if (likely(old == c))
			break;
		c = old;
	}
	return c;
}


#undef __CS_LOOP

#define ATOMIC64_INIT(i)  { (i) }

#ifdef CONFIG_64BIT

#define __CSG_LOOP(ptr, op_val, op_string) ({				\
	long long old_val, new_val;					\
	asm volatile(							\
		"	lg	%0,%2\n"				\
		"0:	lgr	%1,%0\n"				\
		op_string "	%1,%3\n"				\
		"	csg	%0,%1,%2\n"				\
		"	jl	0b"					\
		: "=&d" (old_val), "=&d" (new_val),			\
		  "=Q" (((atomic_t *)(ptr))->counter)			\
		: "d" (op_val),	"Q" (((atomic_t *)(ptr))->counter)	\
		: "cc", "memory");					\
	new_val;							\
})

static inline long long atomic64_read(const atomic64_t *v)
{
	long long c;

	asm volatile(
		"	lg	%0,%1\n"
		: "=d" (c) : "Q" (v->counter));
	return c;
}

static inline void atomic64_set(atomic64_t *v, long long i)
{
	asm volatile(
		"	stg	%1,%0\n"
		: "=Q" (v->counter) : "d" (i));
}

static inline long long atomic64_add_return(long long i, atomic64_t *v)
{
	return __CSG_LOOP(v, i, "agr");
}

static inline long long atomic64_sub_return(long long i, atomic64_t *v)
{
	return __CSG_LOOP(v, i, "sgr");
}

static inline void atomic64_clear_mask(unsigned long mask, atomic64_t *v)
{
	__CSG_LOOP(v, ~mask, "ngr");
}

static inline void atomic64_set_mask(unsigned long mask, atomic64_t *v)
{
	__CSG_LOOP(v, mask, "ogr");
}

#define atomic64_xchg(v, new) (xchg(&((v)->counter), new))

static inline long long atomic64_cmpxchg(atomic64_t *v,
					     long long old, long long new)
{
	asm volatile(
		"	csg	%0,%2,%1"
		: "+d" (old), "=Q" (v->counter)
		: "d" (new), "Q" (v->counter)
		: "cc", "memory");
	return old;
}

#undef __CSG_LOOP

#else /* CONFIG_64BIT */

typedef struct {
	long long counter;
} atomic64_t;

static inline long long atomic64_read(const atomic64_t *v)
{
	register_pair rp;

	asm volatile(
		"	lm	%0,%N0,%1"
		: "=&d" (rp) : "Q" (v->counter)	);
	return rp.pair;
}

static inline void atomic64_set(atomic64_t *v, long long i)
{
	register_pair rp = {.pair = i};

	asm volatile(
		"	stm	%1,%N1,%0"
		: "=Q" (v->counter) : "d" (rp) );
}

static inline long long atomic64_xchg(atomic64_t *v, long long new)
{
	register_pair rp_new = {.pair = new};
	register_pair rp_old;

	asm volatile(
		"	lm	%0,%N0,%1\n"
		"0:	cds	%0,%2,%1\n"
		"	jl	0b\n"
		: "=&d" (rp_old), "=Q" (v->counter)
		: "d" (rp_new), "Q" (v->counter)
		: "cc");
	return rp_old.pair;
}

static inline long long atomic64_cmpxchg(atomic64_t *v,
					 long long old, long long new)
{
	register_pair rp_old = {.pair = old};
	register_pair rp_new = {.pair = new};

	asm volatile(
		"	cds	%0,%2,%1"
		: "+&d" (rp_old), "=Q" (v->counter)
		: "d" (rp_new), "Q" (v->counter)
		: "cc");
	return rp_old.pair;
}


static inline long long atomic64_add_return(long long i, atomic64_t *v)
{
	long long old, new;

	do {
		old = atomic64_read(v);
		new = old + i;
	} while (atomic64_cmpxchg(v, old, new) != old);
	return new;
}

static inline long long atomic64_sub_return(long long i, atomic64_t *v)
{
	long long old, new;

	do {
		old = atomic64_read(v);
		new = old - i;
	} while (atomic64_cmpxchg(v, old, new) != old);
	return new;
}

static inline void atomic64_set_mask(unsigned long long mask, atomic64_t *v)
{
	long long old, new;

	do {
		old = atomic64_read(v);
		new = old | mask;
	} while (atomic64_cmpxchg(v, old, new) != old);
}

static inline void atomic64_clear_mask(unsigned long long mask, atomic64_t *v)
{
	long long old, new;

	do {
		old = atomic64_read(v);
		new = old & mask;
	} while (atomic64_cmpxchg(v, old, new) != old);
}

#endif /* CONFIG_64BIT */

static inline int atomic64_add_unless(atomic64_t *v, long long a, long long u)
{
	long long c, old;

	c = atomic64_read(v);
	for (;;) {
		if (unlikely(c == u))
			break;
		old = atomic64_cmpxchg(v, c, c + a);
		if (likely(old == c))
			break;
		c = old;
	}
	return c != u;
}

static inline long long atomic64_dec_if_positive(atomic64_t *v)
{
	long long c, old, dec;

	c = atomic64_read(v);
	for (;;) {
		dec = c - 1;
		if (unlikely(dec < 0))
			break;
		old = atomic64_cmpxchg((v), c, dec);
		if (likely(old == c))
			break;
		c = old;
	}
	return dec;
}

#define atomic64_add(_i, _v)		atomic64_add_return(_i, _v)
#define atomic64_add_negative(_i, _v)	(atomic64_add_return(_i, _v) < 0)
#define atomic64_inc(_v)		atomic64_add_return(1, _v)
#define atomic64_inc_return(_v)		atomic64_add_return(1, _v)
#define atomic64_inc_and_test(_v)	(atomic64_add_return(1, _v) == 0)
#define atomic64_sub(_i, _v)		atomic64_sub_return(_i, _v)
#define atomic64_sub_and_test(_i, _v)	(atomic64_sub_return(_i, _v) == 0)
#define atomic64_dec(_v)		atomic64_sub_return(1, _v)
#define atomic64_dec_return(_v)		atomic64_sub_return(1, _v)
#define atomic64_dec_and_test(_v)	(atomic64_sub_return(1, _v) == 0)
#define atomic64_inc_not_zero(v)	atomic64_add_unless((v), 1, 0)

#define smp_mb__before_atomic_dec()	smp_mb()
#define smp_mb__after_atomic_dec()	smp_mb()
#define smp_mb__before_atomic_inc()	smp_mb()
#define smp_mb__after_atomic_inc()	smp_mb()
=======
/* SPDX-License-Identifier: GPL-2.0 */
/*
 * Copyright IBM Corp. 1999, 2016
 * Author(s): Martin Schwidefsky <schwidefsky@de.ibm.com>,
 *	      Denis Joseph Barrow,
 *	      Arnd Bergmann,
 */

#ifndef __ARCH_S390_ATOMIC__
#define __ARCH_S390_ATOMIC__

#include <linux/compiler.h>
#include <linux/types.h>
#include <asm/atomic_ops.h>
#include <asm/barrier.h>
#include <asm/cmpxchg.h>

static __always_inline int arch_atomic_read(const atomic_t *v)
{
	return __atomic_read(v);
}
#define arch_atomic_read arch_atomic_read

static __always_inline void arch_atomic_set(atomic_t *v, int i)
{
	__atomic_set(v, i);
}
#define arch_atomic_set arch_atomic_set

static __always_inline int arch_atomic_add_return(int i, atomic_t *v)
{
	return __atomic_add_barrier(i, &v->counter) + i;
}
#define arch_atomic_add_return arch_atomic_add_return

static __always_inline int arch_atomic_fetch_add(int i, atomic_t *v)
{
	return __atomic_add_barrier(i, &v->counter);
}
#define arch_atomic_fetch_add arch_atomic_fetch_add

static __always_inline void arch_atomic_add(int i, atomic_t *v)
{
	__atomic_add(i, &v->counter);
}
#define arch_atomic_add arch_atomic_add

#define arch_atomic_sub(_i, _v)		arch_atomic_add(-(int)(_i), _v)
#define arch_atomic_sub_return(_i, _v)	arch_atomic_add_return(-(int)(_i), _v)
#define arch_atomic_fetch_sub(_i, _v)	arch_atomic_fetch_add(-(int)(_i), _v)

#define ATOMIC_OPS(op)							\
static __always_inline void arch_atomic_##op(int i, atomic_t *v)	\
{									\
	__atomic_##op(i, &v->counter);					\
}									\
static __always_inline int arch_atomic_fetch_##op(int i, atomic_t *v)	\
{									\
	return __atomic_##op##_barrier(i, &v->counter);			\
}

ATOMIC_OPS(and)
ATOMIC_OPS(or)
ATOMIC_OPS(xor)

#undef ATOMIC_OPS

#define arch_atomic_and			arch_atomic_and
#define arch_atomic_or			arch_atomic_or
#define arch_atomic_xor			arch_atomic_xor
#define arch_atomic_fetch_and		arch_atomic_fetch_and
#define arch_atomic_fetch_or		arch_atomic_fetch_or
#define arch_atomic_fetch_xor		arch_atomic_fetch_xor

#define arch_atomic_xchg(v, new)	(arch_xchg(&((v)->counter), new))

static __always_inline int arch_atomic_cmpxchg(atomic_t *v, int old, int new)
{
	return __atomic_cmpxchg(&v->counter, old, new);
}
#define arch_atomic_cmpxchg arch_atomic_cmpxchg

#define ATOMIC64_INIT(i)  { (i) }

static __always_inline s64 arch_atomic64_read(const atomic64_t *v)
{
	return __atomic64_read(v);
}
#define arch_atomic64_read arch_atomic64_read

static __always_inline void arch_atomic64_set(atomic64_t *v, s64 i)
{
	__atomic64_set(v, i);
}
#define arch_atomic64_set arch_atomic64_set

static __always_inline s64 arch_atomic64_add_return(s64 i, atomic64_t *v)
{
	return __atomic64_add_barrier(i, (long *)&v->counter) + i;
}
#define arch_atomic64_add_return arch_atomic64_add_return

static __always_inline s64 arch_atomic64_fetch_add(s64 i, atomic64_t *v)
{
	return __atomic64_add_barrier(i, (long *)&v->counter);
}
#define arch_atomic64_fetch_add arch_atomic64_fetch_add

static __always_inline void arch_atomic64_add(s64 i, atomic64_t *v)
{
	__atomic64_add(i, (long *)&v->counter);
}
#define arch_atomic64_add arch_atomic64_add

#define arch_atomic64_xchg(v, new)	(arch_xchg(&((v)->counter), new))

static __always_inline s64 arch_atomic64_cmpxchg(atomic64_t *v, s64 old, s64 new)
{
	return __atomic64_cmpxchg((long *)&v->counter, old, new);
}
#define arch_atomic64_cmpxchg arch_atomic64_cmpxchg

#define ATOMIC64_OPS(op)							\
static __always_inline void arch_atomic64_##op(s64 i, atomic64_t *v)		\
{										\
	__atomic64_##op(i, (long *)&v->counter);				\
}										\
static __always_inline long arch_atomic64_fetch_##op(s64 i, atomic64_t *v)	\
{										\
	return __atomic64_##op##_barrier(i, (long *)&v->counter);		\
}

ATOMIC64_OPS(and)
ATOMIC64_OPS(or)
ATOMIC64_OPS(xor)

#undef ATOMIC64_OPS

#define arch_atomic64_and		arch_atomic64_and
#define arch_atomic64_or		arch_atomic64_or
#define arch_atomic64_xor		arch_atomic64_xor
#define arch_atomic64_fetch_and		arch_atomic64_fetch_and
#define arch_atomic64_fetch_or		arch_atomic64_fetch_or
#define arch_atomic64_fetch_xor		arch_atomic64_fetch_xor

#define arch_atomic64_sub_return(_i, _v) arch_atomic64_add_return(-(s64)(_i), _v)
#define arch_atomic64_fetch_sub(_i, _v)  arch_atomic64_fetch_add(-(s64)(_i), _v)
#define arch_atomic64_sub(_i, _v)	 arch_atomic64_add(-(s64)(_i), _v)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

#endif /* __ARCH_S390_ATOMIC__  */
