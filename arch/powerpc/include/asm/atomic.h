<<<<<<< HEAD
=======
/* SPDX-License-Identifier: GPL-2.0 */
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#ifndef _ASM_POWERPC_ATOMIC_H_
#define _ASM_POWERPC_ATOMIC_H_

/*
 * PowerPC atomic operations
 */

#ifdef __KERNEL__
#include <linux/types.h>
#include <asm/cmpxchg.h>
<<<<<<< HEAD

#define ATOMIC_INIT(i)		{ (i) }

static __inline__ int atomic_read(const atomic_t *v)
{
	int t;

	__asm__ __volatile__("lwz%U1%X1 %0,%1" : "=r"(t) : "m"(v->counter));

	return t;
}

static __inline__ void atomic_set(atomic_t *v, int i)
{
	__asm__ __volatile__("stw%U0%X0 %1,%0" : "=m"(v->counter) : "r"(i));
}

static __inline__ void atomic_add(int a, atomic_t *v)
{
	int t;

	__asm__ __volatile__(
"1:	lwarx	%0,0,%3		# atomic_add\n\
	add	%0,%2,%0\n"
	PPC405_ERR77(0,%3)
"	stwcx.	%0,0,%3 \n\
	bne-	1b"
	: "=&r" (t), "+m" (v->counter)
	: "r" (a), "r" (&v->counter)
	: "cc");
}

static __inline__ int atomic_add_return(int a, atomic_t *v)
{
	int t;

	__asm__ __volatile__(
	PPC_ATOMIC_ENTRY_BARRIER
"1:	lwarx	%0,0,%2		# atomic_add_return\n\
	add	%0,%1,%0\n"
	PPC405_ERR77(0,%2)
"	stwcx.	%0,0,%2 \n\
	bne-	1b"
	PPC_ATOMIC_EXIT_BARRIER
	: "=&r" (t)
	: "r" (a), "r" (&v->counter)
	: "cc", "memory");

	return t;
}

#define atomic_add_negative(a, v)	(atomic_add_return((a), (v)) < 0)

static __inline__ void atomic_sub(int a, atomic_t *v)
{
	int t;

	__asm__ __volatile__(
"1:	lwarx	%0,0,%3		# atomic_sub\n\
	subf	%0,%2,%0\n"
	PPC405_ERR77(0,%3)
"	stwcx.	%0,0,%3 \n\
	bne-	1b"
	: "=&r" (t), "+m" (v->counter)
	: "r" (a), "r" (&v->counter)
	: "cc");
}

static __inline__ int atomic_sub_return(int a, atomic_t *v)
{
	int t;

	__asm__ __volatile__(
	PPC_ATOMIC_ENTRY_BARRIER
"1:	lwarx	%0,0,%2		# atomic_sub_return\n\
	subf	%0,%1,%0\n"
	PPC405_ERR77(0,%2)
"	stwcx.	%0,0,%2 \n\
	bne-	1b"
	PPC_ATOMIC_EXIT_BARRIER
	: "=&r" (t)
	: "r" (a), "r" (&v->counter)
	: "cc", "memory");

	return t;
}

static __inline__ void atomic_inc(atomic_t *v)
{
	int t;

	__asm__ __volatile__(
"1:	lwarx	%0,0,%2		# atomic_inc\n\
	addic	%0,%0,1\n"
	PPC405_ERR77(0,%2)
"	stwcx.	%0,0,%2 \n\
	bne-	1b"
	: "=&r" (t), "+m" (v->counter)
	: "r" (&v->counter)
	: "cc", "xer");
}

static __inline__ int atomic_inc_return(atomic_t *v)
{
	int t;

	__asm__ __volatile__(
	PPC_ATOMIC_ENTRY_BARRIER
"1:	lwarx	%0,0,%1		# atomic_inc_return\n\
	addic	%0,%0,1\n"
	PPC405_ERR77(0,%1)
"	stwcx.	%0,0,%1 \n\
	bne-	1b"
	PPC_ATOMIC_EXIT_BARRIER
	: "=&r" (t)
	: "r" (&v->counter)
	: "cc", "xer", "memory");

	return t;
}

/*
 * atomic_inc_and_test - increment and test
 * @v: pointer of type atomic_t
 *
 * Atomically increments @v by 1
 * and returns true if the result is zero, or false for all
 * other cases.
 */
#define atomic_inc_and_test(v) (atomic_inc_return(v) == 0)

static __inline__ void atomic_dec(atomic_t *v)
{
	int t;

	__asm__ __volatile__(
"1:	lwarx	%0,0,%2		# atomic_dec\n\
	addic	%0,%0,-1\n"
	PPC405_ERR77(0,%2)\
"	stwcx.	%0,0,%2\n\
	bne-	1b"
	: "=&r" (t), "+m" (v->counter)
	: "r" (&v->counter)
	: "cc", "xer");
}

static __inline__ int atomic_dec_return(atomic_t *v)
{
	int t;

	__asm__ __volatile__(
	PPC_ATOMIC_ENTRY_BARRIER
"1:	lwarx	%0,0,%1		# atomic_dec_return\n\
	addic	%0,%0,-1\n"
	PPC405_ERR77(0,%1)
"	stwcx.	%0,0,%1\n\
	bne-	1b"
	PPC_ATOMIC_EXIT_BARRIER
	: "=&r" (t)
	: "r" (&v->counter)
	: "cc", "xer", "memory");
=======
#include <asm/barrier.h>
#include <asm/asm-const.h>

/*
 * Since *_return_relaxed and {cmp}xchg_relaxed are implemented with
 * a "bne-" instruction at the end, so an isync is enough as a acquire barrier
 * on the platform without lwsync.
 */
#define __atomic_acquire_fence()					\
	__asm__ __volatile__(PPC_ACQUIRE_BARRIER "" : : : "memory")

#define __atomic_release_fence()					\
	__asm__ __volatile__(PPC_RELEASE_BARRIER "" : : : "memory")

static __inline__ int arch_atomic_read(const atomic_t *v)
{
	int t;

	/* -mprefixed can generate offsets beyond range, fall back hack */
	if (IS_ENABLED(CONFIG_PPC_KERNEL_PREFIXED))
		__asm__ __volatile__("lwz %0,0(%1)" : "=r"(t) : "b"(&v->counter));
	else
		__asm__ __volatile__("lwz%U1%X1 %0,%1" : "=r"(t) : "m<>"(v->counter));
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	return t;
}

<<<<<<< HEAD
#define atomic_cmpxchg(v, o, n) (cmpxchg(&((v)->counter), (o), (n)))
#define atomic_xchg(v, new) (xchg(&((v)->counter), new))

/**
 * __atomic_add_unless - add unless the number is a given value
=======
static __inline__ void arch_atomic_set(atomic_t *v, int i)
{
	/* -mprefixed can generate offsets beyond range, fall back hack */
	if (IS_ENABLED(CONFIG_PPC_KERNEL_PREFIXED))
		__asm__ __volatile__("stw %1,0(%2)" : "=m"(v->counter) : "r"(i), "b"(&v->counter));
	else
		__asm__ __volatile__("stw%U0%X0 %1,%0" : "=m<>"(v->counter) : "r"(i));
}

#define ATOMIC_OP(op, asm_op, suffix, sign, ...)			\
static __inline__ void arch_atomic_##op(int a, atomic_t *v)		\
{									\
	int t;								\
									\
	__asm__ __volatile__(						\
"1:	lwarx	%0,0,%3		# atomic_" #op "\n"			\
	#asm_op "%I2" suffix " %0,%0,%2\n"				\
"	stwcx.	%0,0,%3 \n"						\
"	bne-	1b\n"							\
	: "=&r" (t), "+m" (v->counter)					\
	: "r"#sign (a), "r" (&v->counter)				\
	: "cc", ##__VA_ARGS__);						\
}									\

#define ATOMIC_OP_RETURN_RELAXED(op, asm_op, suffix, sign, ...)		\
static inline int arch_atomic_##op##_return_relaxed(int a, atomic_t *v)	\
{									\
	int t;								\
									\
	__asm__ __volatile__(						\
"1:	lwarx	%0,0,%3		# atomic_" #op "_return_relaxed\n"	\
	#asm_op "%I2" suffix " %0,%0,%2\n"				\
"	stwcx.	%0,0,%3\n"						\
"	bne-	1b\n"							\
	: "=&r" (t), "+m" (v->counter)					\
	: "r"#sign (a), "r" (&v->counter)				\
	: "cc", ##__VA_ARGS__);						\
									\
	return t;							\
}

#define ATOMIC_FETCH_OP_RELAXED(op, asm_op, suffix, sign, ...)		\
static inline int arch_atomic_fetch_##op##_relaxed(int a, atomic_t *v)	\
{									\
	int res, t;							\
									\
	__asm__ __volatile__(						\
"1:	lwarx	%0,0,%4		# atomic_fetch_" #op "_relaxed\n"	\
	#asm_op "%I3" suffix " %1,%0,%3\n"				\
"	stwcx.	%1,0,%4\n"						\
"	bne-	1b\n"							\
	: "=&r" (res), "=&r" (t), "+m" (v->counter)			\
	: "r"#sign (a), "r" (&v->counter)				\
	: "cc", ##__VA_ARGS__);						\
									\
	return res;							\
}

#define ATOMIC_OPS(op, asm_op, suffix, sign, ...)			\
	ATOMIC_OP(op, asm_op, suffix, sign, ##__VA_ARGS__)		\
	ATOMIC_OP_RETURN_RELAXED(op, asm_op, suffix, sign, ##__VA_ARGS__)\
	ATOMIC_FETCH_OP_RELAXED(op, asm_op, suffix, sign, ##__VA_ARGS__)

ATOMIC_OPS(add, add, "c", I, "xer")
ATOMIC_OPS(sub, sub, "c", I, "xer")

#define arch_atomic_add_return_relaxed arch_atomic_add_return_relaxed
#define arch_atomic_sub_return_relaxed arch_atomic_sub_return_relaxed

#define arch_atomic_fetch_add_relaxed arch_atomic_fetch_add_relaxed
#define arch_atomic_fetch_sub_relaxed arch_atomic_fetch_sub_relaxed

#undef ATOMIC_OPS
#define ATOMIC_OPS(op, asm_op, suffix, sign)				\
	ATOMIC_OP(op, asm_op, suffix, sign)				\
	ATOMIC_FETCH_OP_RELAXED(op, asm_op, suffix, sign)

ATOMIC_OPS(and, and, ".", K)
ATOMIC_OPS(or, or, "", K)
ATOMIC_OPS(xor, xor, "", K)

#define arch_atomic_fetch_and_relaxed arch_atomic_fetch_and_relaxed
#define arch_atomic_fetch_or_relaxed  arch_atomic_fetch_or_relaxed
#define arch_atomic_fetch_xor_relaxed arch_atomic_fetch_xor_relaxed

#undef ATOMIC_OPS
#undef ATOMIC_FETCH_OP_RELAXED
#undef ATOMIC_OP_RETURN_RELAXED
#undef ATOMIC_OP

/**
 * atomic_fetch_add_unless - add unless the number is a given value
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 * @v: pointer of type atomic_t
 * @a: the amount to add to v...
 * @u: ...unless v is equal to u.
 *
 * Atomically adds @a to @v, so long as it was not @u.
 * Returns the old value of @v.
 */
<<<<<<< HEAD
static __inline__ int __atomic_add_unless(atomic_t *v, int a, int u)
=======
static __inline__ int arch_atomic_fetch_add_unless(atomic_t *v, int a, int u)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
{
	int t;

	__asm__ __volatile__ (
	PPC_ATOMIC_ENTRY_BARRIER
<<<<<<< HEAD
"1:	lwarx	%0,0,%1		# __atomic_add_unless\n\
	cmpw	0,%0,%3 \n\
	beq-	2f \n\
	add	%0,%2,%0 \n"
	PPC405_ERR77(0,%2)
"	stwcx.	%0,0,%1 \n\
	bne-	1b \n"
	PPC_ATOMIC_EXIT_BARRIER
"	subf	%0,%2,%0 \n\
2:"
	: "=&r" (t)
	: "r" (&v->counter), "r" (a), "r" (u)
	: "cc", "memory");

	return t;
}

/**
 * atomic_inc_not_zero - increment unless the number is zero
 * @v: pointer of type atomic_t
 *
 * Atomically increments @v by 1, so long as @v is non-zero.
 * Returns non-zero if @v was non-zero, and zero otherwise.
 */
static __inline__ int atomic_inc_not_zero(atomic_t *v)
{
	int t1, t2;

	__asm__ __volatile__ (
	PPC_ATOMIC_ENTRY_BARRIER
"1:	lwarx	%0,0,%2		# atomic_inc_not_zero\n\
	cmpwi	0,%0,0\n\
	beq-	2f\n\
	addic	%1,%0,1\n"
	PPC405_ERR77(0,%2)
"	stwcx.	%1,0,%2\n\
	bne-	1b\n"
	PPC_ATOMIC_EXIT_BARRIER
	"\n\
2:"
	: "=&r" (t1), "=&r" (t2)
	: "r" (&v->counter)
	: "cc", "xer", "memory");

	return t1;
}
#define atomic_inc_not_zero(v) atomic_inc_not_zero((v))

#define atomic_sub_and_test(a, v)	(atomic_sub_return((a), (v)) == 0)
#define atomic_dec_and_test(v)		(atomic_dec_return((v)) == 0)
=======
"1:	lwarx	%0,0,%1		# atomic_fetch_add_unless\n\
	cmpw	0,%0,%3 \n\
	beq	2f \n\
	add%I2c	%0,%0,%2 \n"
"	stwcx.	%0,0,%1 \n\
	bne-	1b \n"
	PPC_ATOMIC_EXIT_BARRIER
"	sub%I2c	%0,%0,%2 \n\
2:"
	: "=&r" (t)
	: "r" (&v->counter), "rI" (a), "r" (u)
	: "cc", "memory", "xer");

	return t;
}
#define arch_atomic_fetch_add_unless arch_atomic_fetch_add_unless
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

/*
 * Atomically test *v and decrement if it is greater than 0.
 * The function returns the old value of *v minus 1, even if
 * the atomic variable, v, was not decremented.
 */
<<<<<<< HEAD
static __inline__ int atomic_dec_if_positive(atomic_t *v)
=======
static __inline__ int arch_atomic_dec_if_positive(atomic_t *v)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
{
	int t;

	__asm__ __volatile__(
	PPC_ATOMIC_ENTRY_BARRIER
"1:	lwarx	%0,0,%1		# atomic_dec_if_positive\n\
	cmpwi	%0,1\n\
	addi	%0,%0,-1\n\
	blt-	2f\n"
<<<<<<< HEAD
	PPC405_ERR77(0,%1)
=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
"	stwcx.	%0,0,%1\n\
	bne-	1b"
	PPC_ATOMIC_EXIT_BARRIER
	"\n\
2:"	: "=&b" (t)
	: "r" (&v->counter)
	: "cc", "memory");

	return t;
}
<<<<<<< HEAD

#define smp_mb__before_atomic_dec()     smp_mb()
#define smp_mb__after_atomic_dec()      smp_mb()
#define smp_mb__before_atomic_inc()     smp_mb()
#define smp_mb__after_atomic_inc()      smp_mb()
=======
#define arch_atomic_dec_if_positive arch_atomic_dec_if_positive
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

#ifdef __powerpc64__

#define ATOMIC64_INIT(i)	{ (i) }

<<<<<<< HEAD
static __inline__ long atomic64_read(const atomic64_t *v)
{
	long t;

	__asm__ __volatile__("ld%U1%X1 %0,%1" : "=r"(t) : "m"(v->counter));
=======
static __inline__ s64 arch_atomic64_read(const atomic64_t *v)
{
	s64 t;

	/* -mprefixed can generate offsets beyond range, fall back hack */
	if (IS_ENABLED(CONFIG_PPC_KERNEL_PREFIXED))
		__asm__ __volatile__("ld %0,0(%1)" : "=r"(t) : "b"(&v->counter));
	else
		__asm__ __volatile__("ld%U1%X1 %0,%1" : "=r"(t) : "m<>"(v->counter));
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	return t;
}

<<<<<<< HEAD
static __inline__ void atomic64_set(atomic64_t *v, long i)
{
	__asm__ __volatile__("std%U0%X0 %1,%0" : "=m"(v->counter) : "r"(i));
}

static __inline__ void atomic64_add(long a, atomic64_t *v)
{
	long t;

	__asm__ __volatile__(
"1:	ldarx	%0,0,%3		# atomic64_add\n\
	add	%0,%2,%0\n\
	stdcx.	%0,0,%3 \n\
	bne-	1b"
	: "=&r" (t), "+m" (v->counter)
	: "r" (a), "r" (&v->counter)
	: "cc");
}

static __inline__ long atomic64_add_return(long a, atomic64_t *v)
{
	long t;

	__asm__ __volatile__(
	PPC_ATOMIC_ENTRY_BARRIER
"1:	ldarx	%0,0,%2		# atomic64_add_return\n\
	add	%0,%1,%0\n\
	stdcx.	%0,0,%2 \n\
	bne-	1b"
	PPC_ATOMIC_EXIT_BARRIER
	: "=&r" (t)
	: "r" (a), "r" (&v->counter)
	: "cc", "memory");

	return t;
}

#define atomic64_add_negative(a, v)	(atomic64_add_return((a), (v)) < 0)

static __inline__ void atomic64_sub(long a, atomic64_t *v)
{
	long t;

	__asm__ __volatile__(
"1:	ldarx	%0,0,%3		# atomic64_sub\n\
	subf	%0,%2,%0\n\
	stdcx.	%0,0,%3 \n\
	bne-	1b"
	: "=&r" (t), "+m" (v->counter)
	: "r" (a), "r" (&v->counter)
	: "cc");
}

static __inline__ long atomic64_sub_return(long a, atomic64_t *v)
{
	long t;

	__asm__ __volatile__(
	PPC_ATOMIC_ENTRY_BARRIER
"1:	ldarx	%0,0,%2		# atomic64_sub_return\n\
	subf	%0,%1,%0\n\
	stdcx.	%0,0,%2 \n\
	bne-	1b"
	PPC_ATOMIC_EXIT_BARRIER
	: "=&r" (t)
	: "r" (a), "r" (&v->counter)
	: "cc", "memory");

	return t;
}

static __inline__ void atomic64_inc(atomic64_t *v)
{
	long t;
=======
static __inline__ void arch_atomic64_set(atomic64_t *v, s64 i)
{
	/* -mprefixed can generate offsets beyond range, fall back hack */
	if (IS_ENABLED(CONFIG_PPC_KERNEL_PREFIXED))
		__asm__ __volatile__("std %1,0(%2)" : "=m"(v->counter) : "r"(i), "b"(&v->counter));
	else
		__asm__ __volatile__("std%U0%X0 %1,%0" : "=m<>"(v->counter) : "r"(i));
}

#define ATOMIC64_OP(op, asm_op)						\
static __inline__ void arch_atomic64_##op(s64 a, atomic64_t *v)		\
{									\
	s64 t;								\
									\
	__asm__ __volatile__(						\
"1:	ldarx	%0,0,%3		# atomic64_" #op "\n"			\
	#asm_op " %0,%2,%0\n"						\
"	stdcx.	%0,0,%3 \n"						\
"	bne-	1b\n"							\
	: "=&r" (t), "+m" (v->counter)					\
	: "r" (a), "r" (&v->counter)					\
	: "cc");							\
}

#define ATOMIC64_OP_RETURN_RELAXED(op, asm_op)				\
static inline s64							\
arch_atomic64_##op##_return_relaxed(s64 a, atomic64_t *v)		\
{									\
	s64 t;								\
									\
	__asm__ __volatile__(						\
"1:	ldarx	%0,0,%3		# atomic64_" #op "_return_relaxed\n"	\
	#asm_op " %0,%2,%0\n"						\
"	stdcx.	%0,0,%3\n"						\
"	bne-	1b\n"							\
	: "=&r" (t), "+m" (v->counter)					\
	: "r" (a), "r" (&v->counter)					\
	: "cc");							\
									\
	return t;							\
}

#define ATOMIC64_FETCH_OP_RELAXED(op, asm_op)				\
static inline s64							\
arch_atomic64_fetch_##op##_relaxed(s64 a, atomic64_t *v)		\
{									\
	s64 res, t;							\
									\
	__asm__ __volatile__(						\
"1:	ldarx	%0,0,%4		# atomic64_fetch_" #op "_relaxed\n"	\
	#asm_op " %1,%3,%0\n"						\
"	stdcx.	%1,0,%4\n"						\
"	bne-	1b\n"							\
	: "=&r" (res), "=&r" (t), "+m" (v->counter)			\
	: "r" (a), "r" (&v->counter)					\
	: "cc");							\
									\
	return res;							\
}

#define ATOMIC64_OPS(op, asm_op)					\
	ATOMIC64_OP(op, asm_op)						\
	ATOMIC64_OP_RETURN_RELAXED(op, asm_op)				\
	ATOMIC64_FETCH_OP_RELAXED(op, asm_op)

ATOMIC64_OPS(add, add)
ATOMIC64_OPS(sub, subf)

#define arch_atomic64_add_return_relaxed arch_atomic64_add_return_relaxed
#define arch_atomic64_sub_return_relaxed arch_atomic64_sub_return_relaxed

#define arch_atomic64_fetch_add_relaxed arch_atomic64_fetch_add_relaxed
#define arch_atomic64_fetch_sub_relaxed arch_atomic64_fetch_sub_relaxed

#undef ATOMIC64_OPS
#define ATOMIC64_OPS(op, asm_op)					\
	ATOMIC64_OP(op, asm_op)						\
	ATOMIC64_FETCH_OP_RELAXED(op, asm_op)

ATOMIC64_OPS(and, and)
ATOMIC64_OPS(or, or)
ATOMIC64_OPS(xor, xor)

#define arch_atomic64_fetch_and_relaxed arch_atomic64_fetch_and_relaxed
#define arch_atomic64_fetch_or_relaxed  arch_atomic64_fetch_or_relaxed
#define arch_atomic64_fetch_xor_relaxed arch_atomic64_fetch_xor_relaxed

#undef ATOPIC64_OPS
#undef ATOMIC64_FETCH_OP_RELAXED
#undef ATOMIC64_OP_RETURN_RELAXED
#undef ATOMIC64_OP

static __inline__ void arch_atomic64_inc(atomic64_t *v)
{
	s64 t;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	__asm__ __volatile__(
"1:	ldarx	%0,0,%2		# atomic64_inc\n\
	addic	%0,%0,1\n\
	stdcx.	%0,0,%2 \n\
	bne-	1b"
	: "=&r" (t), "+m" (v->counter)
	: "r" (&v->counter)
	: "cc", "xer");
}
<<<<<<< HEAD

static __inline__ long atomic64_inc_return(atomic64_t *v)
{
	long t;

	__asm__ __volatile__(
	PPC_ATOMIC_ENTRY_BARRIER
"1:	ldarx	%0,0,%1		# atomic64_inc_return\n\
	addic	%0,%0,1\n\
	stdcx.	%0,0,%1 \n\
	bne-	1b"
	PPC_ATOMIC_EXIT_BARRIER
	: "=&r" (t)
	: "r" (&v->counter)
	: "cc", "xer", "memory");
=======
#define arch_atomic64_inc arch_atomic64_inc

static __inline__ s64 arch_atomic64_inc_return_relaxed(atomic64_t *v)
{
	s64 t;

	__asm__ __volatile__(
"1:	ldarx	%0,0,%2		# atomic64_inc_return_relaxed\n"
"	addic	%0,%0,1\n"
"	stdcx.	%0,0,%2\n"
"	bne-	1b"
	: "=&r" (t), "+m" (v->counter)
	: "r" (&v->counter)
	: "cc", "xer");
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	return t;
}

<<<<<<< HEAD
/*
 * atomic64_inc_and_test - increment and test
 * @v: pointer of type atomic64_t
 *
 * Atomically increments @v by 1
 * and returns true if the result is zero, or false for all
 * other cases.
 */
#define atomic64_inc_and_test(v) (atomic64_inc_return(v) == 0)

static __inline__ void atomic64_dec(atomic64_t *v)
{
	long t;
=======
static __inline__ void arch_atomic64_dec(atomic64_t *v)
{
	s64 t;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	__asm__ __volatile__(
"1:	ldarx	%0,0,%2		# atomic64_dec\n\
	addic	%0,%0,-1\n\
	stdcx.	%0,0,%2\n\
	bne-	1b"
	: "=&r" (t), "+m" (v->counter)
	: "r" (&v->counter)
	: "cc", "xer");
}
<<<<<<< HEAD

static __inline__ long atomic64_dec_return(atomic64_t *v)
{
	long t;

	__asm__ __volatile__(
	PPC_ATOMIC_ENTRY_BARRIER
"1:	ldarx	%0,0,%1		# atomic64_dec_return\n\
	addic	%0,%0,-1\n\
	stdcx.	%0,0,%1\n\
	bne-	1b"
	PPC_ATOMIC_EXIT_BARRIER
	: "=&r" (t)
	: "r" (&v->counter)
	: "cc", "xer", "memory");
=======
#define arch_atomic64_dec arch_atomic64_dec

static __inline__ s64 arch_atomic64_dec_return_relaxed(atomic64_t *v)
{
	s64 t;

	__asm__ __volatile__(
"1:	ldarx	%0,0,%2		# atomic64_dec_return_relaxed\n"
"	addic	%0,%0,-1\n"
"	stdcx.	%0,0,%2\n"
"	bne-	1b"
	: "=&r" (t), "+m" (v->counter)
	: "r" (&v->counter)
	: "cc", "xer");
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	return t;
}

<<<<<<< HEAD
#define atomic64_sub_and_test(a, v)	(atomic64_sub_return((a), (v)) == 0)
#define atomic64_dec_and_test(v)	(atomic64_dec_return((v)) == 0)
=======
#define arch_atomic64_inc_return_relaxed arch_atomic64_inc_return_relaxed
#define arch_atomic64_dec_return_relaxed arch_atomic64_dec_return_relaxed
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

/*
 * Atomically test *v and decrement if it is greater than 0.
 * The function returns the old value of *v minus 1.
 */
<<<<<<< HEAD
static __inline__ long atomic64_dec_if_positive(atomic64_t *v)
{
	long t;
=======
static __inline__ s64 arch_atomic64_dec_if_positive(atomic64_t *v)
{
	s64 t;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	__asm__ __volatile__(
	PPC_ATOMIC_ENTRY_BARRIER
"1:	ldarx	%0,0,%1		# atomic64_dec_if_positive\n\
	addic.	%0,%0,-1\n\
	blt-	2f\n\
	stdcx.	%0,0,%1\n\
	bne-	1b"
	PPC_ATOMIC_EXIT_BARRIER
	"\n\
2:"	: "=&r" (t)
	: "r" (&v->counter)
	: "cc", "xer", "memory");

	return t;
}
<<<<<<< HEAD

#define atomic64_cmpxchg(v, o, n) (cmpxchg(&((v)->counter), (o), (n)))
#define atomic64_xchg(v, new) (xchg(&((v)->counter), new))

/**
 * atomic64_add_unless - add unless the number is a given value
=======
#define arch_atomic64_dec_if_positive arch_atomic64_dec_if_positive

/**
 * atomic64_fetch_add_unless - add unless the number is a given value
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 * @v: pointer of type atomic64_t
 * @a: the amount to add to v...
 * @u: ...unless v is equal to u.
 *
 * Atomically adds @a to @v, so long as it was not @u.
 * Returns the old value of @v.
 */
<<<<<<< HEAD
static __inline__ int atomic64_add_unless(atomic64_t *v, long a, long u)
{
	long t;

	__asm__ __volatile__ (
	PPC_ATOMIC_ENTRY_BARRIER
"1:	ldarx	%0,0,%1		# __atomic_add_unless\n\
	cmpd	0,%0,%3 \n\
	beq-	2f \n\
=======
static __inline__ s64 arch_atomic64_fetch_add_unless(atomic64_t *v, s64 a, s64 u)
{
	s64 t;

	__asm__ __volatile__ (
	PPC_ATOMIC_ENTRY_BARRIER
"1:	ldarx	%0,0,%1		# atomic64_fetch_add_unless\n\
	cmpd	0,%0,%3 \n\
	beq	2f \n\
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	add	%0,%2,%0 \n"
"	stdcx.	%0,0,%1 \n\
	bne-	1b \n"
	PPC_ATOMIC_EXIT_BARRIER
"	subf	%0,%2,%0 \n\
2:"
	: "=&r" (t)
	: "r" (&v->counter), "r" (a), "r" (u)
	: "cc", "memory");

<<<<<<< HEAD
	return t != u;
}
=======
	return t;
}
#define arch_atomic64_fetch_add_unless arch_atomic64_fetch_add_unless
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

/**
 * atomic_inc64_not_zero - increment unless the number is zero
 * @v: pointer of type atomic64_t
 *
 * Atomically increments @v by 1, so long as @v is non-zero.
 * Returns non-zero if @v was non-zero, and zero otherwise.
 */
<<<<<<< HEAD
static __inline__ long atomic64_inc_not_zero(atomic64_t *v)
{
	long t1, t2;
=======
static __inline__ int arch_atomic64_inc_not_zero(atomic64_t *v)
{
	s64 t1, t2;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	__asm__ __volatile__ (
	PPC_ATOMIC_ENTRY_BARRIER
"1:	ldarx	%0,0,%2		# atomic64_inc_not_zero\n\
	cmpdi	0,%0,0\n\
	beq-	2f\n\
	addic	%1,%0,1\n\
	stdcx.	%1,0,%2\n\
	bne-	1b\n"
	PPC_ATOMIC_EXIT_BARRIER
	"\n\
2:"
	: "=&r" (t1), "=&r" (t2)
	: "r" (&v->counter)
	: "cc", "xer", "memory");

<<<<<<< HEAD
	return t1;
}
=======
	return t1 != 0;
}
#define arch_atomic64_inc_not_zero(v) arch_atomic64_inc_not_zero((v))
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

#endif /* __powerpc64__ */

#endif /* __KERNEL__ */
#endif /* _ASM_POWERPC_ATOMIC_H_ */
