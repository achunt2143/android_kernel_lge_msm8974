/*
 * Atomic operations that C can't guarantee us.  Useful for
 * resource counting etc..
 *
 * But use these as seldom as possible since they are much more slower
 * than regular operations.
 *
 * This file is subject to the terms and conditions of the GNU General Public
 * License.  See the file "COPYING" in the main directory of this archive
 * for more details.
 *
 * Copyright (C) 1996, 97, 99, 2000, 03, 04, 06 by Ralf Baechle
 */
#ifndef _ASM_ATOMIC_H
#define _ASM_ATOMIC_H

#include <linux/irqflags.h>
#include <linux/types.h>
<<<<<<< HEAD
#include <asm/barrier.h>
#include <asm/cpu-features.h>
#include <asm/cmpxchg.h>
#include <asm/war.h>

#define ATOMIC_INIT(i)    { (i) }

/*
 * atomic_read - read atomic variable
 * @v: pointer of type atomic_t
 *
 * Atomically reads the value of @v.
 */
#define atomic_read(v)		(*(volatile int *)&(v)->counter)

/*
 * atomic_set - set atomic variable
 * @v: pointer of type atomic_t
 * @i: required value
 *
 * Atomically sets the value of @v to @i.
 */
#define atomic_set(v, i)		((v)->counter = (i))

/*
 * atomic_add - add integer to atomic variable
 * @i: integer value to add
 * @v: pointer of type atomic_t
 *
 * Atomically adds @i to @v.
 */
static __inline__ void atomic_add(int i, atomic_t * v)
{
	if (kernel_uses_llsc && R10000_LLSC_WAR) {
		int temp;

		__asm__ __volatile__(
		"	.set	mips3					\n"
		"1:	ll	%0, %1		# atomic_add		\n"
		"	addu	%0, %2					\n"
		"	sc	%0, %1					\n"
		"	beqzl	%0, 1b					\n"
		"	.set	mips0					\n"
		: "=&r" (temp), "=m" (v->counter)
		: "Ir" (i), "m" (v->counter));
	} else if (kernel_uses_llsc) {
		int temp;

		do {
			__asm__ __volatile__(
			"	.set	mips3				\n"
			"	ll	%0, %1		# atomic_add	\n"
			"	addu	%0, %2				\n"
			"	sc	%0, %1				\n"
			"	.set	mips0				\n"
			: "=&r" (temp), "=m" (v->counter)
			: "Ir" (i), "m" (v->counter));
		} while (unlikely(!temp));
	} else {
		unsigned long flags;

		raw_local_irq_save(flags);
		v->counter += i;
		raw_local_irq_restore(flags);
	}
}

/*
 * atomic_sub - subtract the atomic variable
 * @i: integer value to subtract
 * @v: pointer of type atomic_t
 *
 * Atomically subtracts @i from @v.
 */
static __inline__ void atomic_sub(int i, atomic_t * v)
{
	if (kernel_uses_llsc && R10000_LLSC_WAR) {
		int temp;

		__asm__ __volatile__(
		"	.set	mips3					\n"
		"1:	ll	%0, %1		# atomic_sub		\n"
		"	subu	%0, %2					\n"
		"	sc	%0, %1					\n"
		"	beqzl	%0, 1b					\n"
		"	.set	mips0					\n"
		: "=&r" (temp), "=m" (v->counter)
		: "Ir" (i), "m" (v->counter));
	} else if (kernel_uses_llsc) {
		int temp;

		do {
			__asm__ __volatile__(
			"	.set	mips3				\n"
			"	ll	%0, %1		# atomic_sub	\n"
			"	subu	%0, %2				\n"
			"	sc	%0, %1				\n"
			"	.set	mips0				\n"
			: "=&r" (temp), "=m" (v->counter)
			: "Ir" (i), "m" (v->counter));
		} while (unlikely(!temp));
	} else {
		unsigned long flags;

		raw_local_irq_save(flags);
		v->counter -= i;
		raw_local_irq_restore(flags);
	}
}

/*
 * Same as above, but return the result value
 */
static __inline__ int atomic_add_return(int i, atomic_t * v)
{
	int result;

	smp_mb__before_llsc();

	if (kernel_uses_llsc && R10000_LLSC_WAR) {
		int temp;

		__asm__ __volatile__(
		"	.set	mips3					\n"
		"1:	ll	%1, %2		# atomic_add_return	\n"
		"	addu	%0, %1, %3				\n"
		"	sc	%0, %2					\n"
		"	beqzl	%0, 1b					\n"
		"	addu	%0, %1, %3				\n"
		"	.set	mips0					\n"
		: "=&r" (result), "=&r" (temp), "=m" (v->counter)
		: "Ir" (i), "m" (v->counter)
		: "memory");
	} else if (kernel_uses_llsc) {
		int temp;

		do {
			__asm__ __volatile__(
			"	.set	mips3				\n"
			"	ll	%1, %2	# atomic_add_return	\n"
			"	addu	%0, %1, %3			\n"
			"	sc	%0, %2				\n"
			"	.set	mips0				\n"
			: "=&r" (result), "=&r" (temp), "=m" (v->counter)
			: "Ir" (i), "m" (v->counter)
			: "memory");
		} while (unlikely(!result));

		result = temp + i;
	} else {
		unsigned long flags;

		raw_local_irq_save(flags);
		result = v->counter;
		result += i;
		v->counter = result;
		raw_local_irq_restore(flags);
	}

	smp_llsc_mb();

	return result;
}

static __inline__ int atomic_sub_return(int i, atomic_t * v)
{
	int result;

	smp_mb__before_llsc();

	if (kernel_uses_llsc && R10000_LLSC_WAR) {
		int temp;

		__asm__ __volatile__(
		"	.set	mips3					\n"
		"1:	ll	%1, %2		# atomic_sub_return	\n"
		"	subu	%0, %1, %3				\n"
		"	sc	%0, %2					\n"
		"	beqzl	%0, 1b					\n"
		"	subu	%0, %1, %3				\n"
		"	.set	mips0					\n"
		: "=&r" (result), "=&r" (temp), "=m" (v->counter)
		: "Ir" (i), "m" (v->counter)
		: "memory");

		result = temp - i;
	} else if (kernel_uses_llsc) {
		int temp;

		do {
			__asm__ __volatile__(
			"	.set	mips3				\n"
			"	ll	%1, %2	# atomic_sub_return	\n"
			"	subu	%0, %1, %3			\n"
			"	sc	%0, %2				\n"
			"	.set	mips0				\n"
			: "=&r" (result), "=&r" (temp), "=m" (v->counter)
			: "Ir" (i), "m" (v->counter)
			: "memory");
		} while (unlikely(!result));

		result = temp - i;
	} else {
		unsigned long flags;

		raw_local_irq_save(flags);
		result = v->counter;
		result -= i;
		v->counter = result;
		raw_local_irq_restore(flags);
	}

	smp_llsc_mb();

	return result;
}
=======
#include <asm/asm.h>
#include <asm/barrier.h>
#include <asm/compiler.h>
#include <asm/cpu-features.h>
#include <asm/cmpxchg.h>
#include <asm/sync.h>

#define ATOMIC_OPS(pfx, type)						\
static __always_inline type arch_##pfx##_read(const pfx##_t *v)		\
{									\
	return READ_ONCE(v->counter);					\
}									\
									\
static __always_inline void arch_##pfx##_set(pfx##_t *v, type i)	\
{									\
	WRITE_ONCE(v->counter, i);					\
}									\

ATOMIC_OPS(atomic, int)

#ifdef CONFIG_64BIT
# define ATOMIC64_INIT(i)	{ (i) }
ATOMIC_OPS(atomic64, s64)
#endif

#define ATOMIC_OP(pfx, op, type, c_op, asm_op, ll, sc)			\
static __inline__ void arch_##pfx##_##op(type i, pfx##_t * v)		\
{									\
	type temp;							\
									\
	if (!kernel_uses_llsc) {					\
		unsigned long flags;					\
									\
		raw_local_irq_save(flags);				\
		v->counter c_op i;					\
		raw_local_irq_restore(flags);				\
		return;							\
	}								\
									\
	__asm__ __volatile__(						\
	"	.set	push					\n"	\
	"	.set	" MIPS_ISA_LEVEL "			\n"	\
	"	" __SYNC(full, loongson3_war) "			\n"	\
	"1:	" #ll "	%0, %1		# " #pfx "_" #op "	\n"	\
	"	" #asm_op " %0, %2				\n"	\
	"	" #sc "	%0, %1					\n"	\
	"\t" __stringify(SC_BEQZ) "	%0, 1b			\n"	\
	"	.set	pop					\n"	\
	: "=&r" (temp), "+" GCC_OFF_SMALL_ASM() (v->counter)		\
	: "Ir" (i) : __LLSC_CLOBBER);					\
}

#define ATOMIC_OP_RETURN(pfx, op, type, c_op, asm_op, ll, sc)		\
static __inline__ type							\
arch_##pfx##_##op##_return_relaxed(type i, pfx##_t * v)			\
{									\
	type temp, result;						\
									\
	if (!kernel_uses_llsc) {					\
		unsigned long flags;					\
									\
		raw_local_irq_save(flags);				\
		result = v->counter;					\
		result c_op i;						\
		v->counter = result;					\
		raw_local_irq_restore(flags);				\
		return result;						\
	}								\
									\
	__asm__ __volatile__(						\
	"	.set	push					\n"	\
	"	.set	" MIPS_ISA_LEVEL "			\n"	\
	"	" __SYNC(full, loongson3_war) "			\n"	\
	"1:	" #ll "	%1, %2		# " #pfx "_" #op "_return\n"	\
	"	" #asm_op " %0, %1, %3				\n"	\
	"	" #sc "	%0, %2					\n"	\
	"\t" __stringify(SC_BEQZ) "	%0, 1b			\n"	\
	"	" #asm_op " %0, %1, %3				\n"	\
	"	.set	pop					\n"	\
	: "=&r" (result), "=&r" (temp),					\
	  "+" GCC_OFF_SMALL_ASM() (v->counter)				\
	: "Ir" (i) : __LLSC_CLOBBER);					\
									\
	return result;							\
}

#define ATOMIC_FETCH_OP(pfx, op, type, c_op, asm_op, ll, sc)		\
static __inline__ type							\
arch_##pfx##_fetch_##op##_relaxed(type i, pfx##_t * v)			\
{									\
	int temp, result;						\
									\
	if (!kernel_uses_llsc) {					\
		unsigned long flags;					\
									\
		raw_local_irq_save(flags);				\
		result = v->counter;					\
		v->counter c_op i;					\
		raw_local_irq_restore(flags);				\
		return result;						\
	}								\
									\
	__asm__ __volatile__(						\
	"	.set	push					\n"	\
	"	.set	" MIPS_ISA_LEVEL "			\n"	\
	"	" __SYNC(full, loongson3_war) "			\n"	\
	"1:	" #ll "	%1, %2		# " #pfx "_fetch_" #op "\n"	\
	"	" #asm_op " %0, %1, %3				\n"	\
	"	" #sc "	%0, %2					\n"	\
	"\t" __stringify(SC_BEQZ) "	%0, 1b			\n"	\
	"	.set	pop					\n"	\
	"	move	%0, %1					\n"	\
	: "=&r" (result), "=&r" (temp),					\
	  "+" GCC_OFF_SMALL_ASM() (v->counter)				\
	: "Ir" (i) : __LLSC_CLOBBER);					\
									\
	return result;							\
}

#undef ATOMIC_OPS
#define ATOMIC_OPS(pfx, op, type, c_op, asm_op, ll, sc)			\
	ATOMIC_OP(pfx, op, type, c_op, asm_op, ll, sc)			\
	ATOMIC_OP_RETURN(pfx, op, type, c_op, asm_op, ll, sc)		\
	ATOMIC_FETCH_OP(pfx, op, type, c_op, asm_op, ll, sc)

ATOMIC_OPS(atomic, add, int, +=, addu, ll, sc)
ATOMIC_OPS(atomic, sub, int, -=, subu, ll, sc)

#define arch_atomic_add_return_relaxed	arch_atomic_add_return_relaxed
#define arch_atomic_sub_return_relaxed	arch_atomic_sub_return_relaxed
#define arch_atomic_fetch_add_relaxed	arch_atomic_fetch_add_relaxed
#define arch_atomic_fetch_sub_relaxed	arch_atomic_fetch_sub_relaxed

#ifdef CONFIG_64BIT
ATOMIC_OPS(atomic64, add, s64, +=, daddu, lld, scd)
ATOMIC_OPS(atomic64, sub, s64, -=, dsubu, lld, scd)
# define arch_atomic64_add_return_relaxed	arch_atomic64_add_return_relaxed
# define arch_atomic64_sub_return_relaxed	arch_atomic64_sub_return_relaxed
# define arch_atomic64_fetch_add_relaxed	arch_atomic64_fetch_add_relaxed
# define arch_atomic64_fetch_sub_relaxed	arch_atomic64_fetch_sub_relaxed
#endif /* CONFIG_64BIT */

#undef ATOMIC_OPS
#define ATOMIC_OPS(pfx, op, type, c_op, asm_op, ll, sc)			\
	ATOMIC_OP(pfx, op, type, c_op, asm_op, ll, sc)			\
	ATOMIC_FETCH_OP(pfx, op, type, c_op, asm_op, ll, sc)

ATOMIC_OPS(atomic, and, int, &=, and, ll, sc)
ATOMIC_OPS(atomic, or, int, |=, or, ll, sc)
ATOMIC_OPS(atomic, xor, int, ^=, xor, ll, sc)

#define arch_atomic_fetch_and_relaxed	arch_atomic_fetch_and_relaxed
#define arch_atomic_fetch_or_relaxed	arch_atomic_fetch_or_relaxed
#define arch_atomic_fetch_xor_relaxed	arch_atomic_fetch_xor_relaxed

#ifdef CONFIG_64BIT
ATOMIC_OPS(atomic64, and, s64, &=, and, lld, scd)
ATOMIC_OPS(atomic64, or, s64, |=, or, lld, scd)
ATOMIC_OPS(atomic64, xor, s64, ^=, xor, lld, scd)
# define arch_atomic64_fetch_and_relaxed	arch_atomic64_fetch_and_relaxed
# define arch_atomic64_fetch_or_relaxed		arch_atomic64_fetch_or_relaxed
# define arch_atomic64_fetch_xor_relaxed	arch_atomic64_fetch_xor_relaxed
#endif

#undef ATOMIC_OPS
#undef ATOMIC_FETCH_OP
#undef ATOMIC_OP_RETURN
#undef ATOMIC_OP
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

/*
 * atomic_sub_if_positive - conditionally subtract integer from atomic variable
 * @i: integer value to subtract
 * @v: pointer of type atomic_t
 *
 * Atomically test @v and subtract @i if @v is greater or equal than @i.
 * The function returns the old value of @v minus @i.
 */
<<<<<<< HEAD
static __inline__ int atomic_sub_if_positive(int i, atomic_t * v)
{
	int result;

	smp_mb__before_llsc();

	if (kernel_uses_llsc && R10000_LLSC_WAR) {
		int temp;

		__asm__ __volatile__(
		"	.set	mips3					\n"
		"1:	ll	%1, %2		# atomic_sub_if_positive\n"
		"	subu	%0, %1, %3				\n"
		"	bltz	%0, 1f					\n"
		"	sc	%0, %2					\n"
		"	.set	noreorder				\n"
		"	beqzl	%0, 1b					\n"
		"	 subu	%0, %1, %3				\n"
		"	.set	reorder					\n"
		"1:							\n"
		"	.set	mips0					\n"
		: "=&r" (result), "=&r" (temp), "=m" (v->counter)
		: "Ir" (i), "m" (v->counter)
		: "memory");
	} else if (kernel_uses_llsc) {
		int temp;

		__asm__ __volatile__(
		"	.set	mips3					\n"
		"1:	ll	%1, %2		# atomic_sub_if_positive\n"
		"	subu	%0, %1, %3				\n"
		"	bltz	%0, 1f					\n"
		"	sc	%0, %2					\n"
		"	.set	noreorder				\n"
		"	beqz	%0, 1b					\n"
		"	 subu	%0, %1, %3				\n"
		"	.set	reorder					\n"
		"1:							\n"
		"	.set	mips0					\n"
		: "=&r" (result), "=&r" (temp), "=m" (v->counter)
		: "Ir" (i), "m" (v->counter)
		: "memory");
	} else {
		unsigned long flags;

		raw_local_irq_save(flags);
		result = v->counter;
		result -= i;
		if (result >= 0)
			v->counter = result;
		raw_local_irq_restore(flags);
	}

	smp_llsc_mb();

	return result;
}

#define atomic_cmpxchg(v, o, n) (cmpxchg(&((v)->counter), (o), (n)))
#define atomic_xchg(v, new) (xchg(&((v)->counter), (new)))

/**
 * __atomic_add_unless - add unless the number is a given value
 * @v: pointer of type atomic_t
 * @a: the amount to add to v...
 * @u: ...unless v is equal to u.
 *
 * Atomically adds @a to @v, so long as it was not @u.
 * Returns the old value of @v.
 */
static __inline__ int __atomic_add_unless(atomic_t *v, int a, int u)
{
	int c, old;
	c = atomic_read(v);
	for (;;) {
		if (unlikely(c == (u)))
			break;
		old = atomic_cmpxchg((v), c, c + (a));
		if (likely(old == c))
			break;
		c = old;
	}
	return c;
}

#define atomic_dec_return(v) atomic_sub_return(1, (v))
#define atomic_inc_return(v) atomic_add_return(1, (v))

/*
 * atomic_sub_and_test - subtract value from variable and test result
 * @i: integer value to subtract
 * @v: pointer of type atomic_t
 *
 * Atomically subtracts @i from @v and returns
 * true if the result is zero, or false for all
 * other cases.
 */
#define atomic_sub_and_test(i, v) (atomic_sub_return((i), (v)) == 0)

/*
 * atomic_inc_and_test - increment and test
 * @v: pointer of type atomic_t
 *
 * Atomically increments @v by 1
 * and returns true if the result is zero, or false for all
 * other cases.
 */
#define atomic_inc_and_test(v) (atomic_inc_return(v) == 0)

/*
 * atomic_dec_and_test - decrement by 1 and test
 * @v: pointer of type atomic_t
 *
 * Atomically decrements @v by 1 and
 * returns true if the result is 0, or false for all other
 * cases.
 */
#define atomic_dec_and_test(v) (atomic_sub_return(1, (v)) == 0)

/*
 * atomic_dec_if_positive - decrement by 1 if old value positive
 * @v: pointer of type atomic_t
 */
#define atomic_dec_if_positive(v)	atomic_sub_if_positive(1, v)

/*
 * atomic_inc - increment atomic variable
 * @v: pointer of type atomic_t
 *
 * Atomically increments @v by 1.
 */
#define atomic_inc(v) atomic_add(1, (v))

/*
 * atomic_dec - decrement and test
 * @v: pointer of type atomic_t
 *
 * Atomically decrements @v by 1.
 */
#define atomic_dec(v) atomic_sub(1, (v))

/*
 * atomic_add_negative - add and test if negative
 * @v: pointer of type atomic_t
 * @i: integer value to add
 *
 * Atomically adds @i to @v and returns true
 * if the result is negative, or false when
 * result is greater than or equal to zero.
 */
#define atomic_add_negative(i, v) (atomic_add_return(i, (v)) < 0)

#ifdef CONFIG_64BIT

#define ATOMIC64_INIT(i)    { (i) }

/*
 * atomic64_read - read atomic variable
 * @v: pointer of type atomic64_t
 *
 */
#define atomic64_read(v)	(*(volatile long *)&(v)->counter)

/*
 * atomic64_set - set atomic variable
 * @v: pointer of type atomic64_t
 * @i: required value
 */
#define atomic64_set(v, i)	((v)->counter = (i))

/*
 * atomic64_add - add integer to atomic variable
 * @i: integer value to add
 * @v: pointer of type atomic64_t
 *
 * Atomically adds @i to @v.
 */
static __inline__ void atomic64_add(long i, atomic64_t * v)
{
	if (kernel_uses_llsc && R10000_LLSC_WAR) {
		long temp;

		__asm__ __volatile__(
		"	.set	mips3					\n"
		"1:	lld	%0, %1		# atomic64_add		\n"
		"	daddu	%0, %2					\n"
		"	scd	%0, %1					\n"
		"	beqzl	%0, 1b					\n"
		"	.set	mips0					\n"
		: "=&r" (temp), "=m" (v->counter)
		: "Ir" (i), "m" (v->counter));
	} else if (kernel_uses_llsc) {
		long temp;

		do {
			__asm__ __volatile__(
			"	.set	mips3				\n"
			"	lld	%0, %1		# atomic64_add	\n"
			"	daddu	%0, %2				\n"
			"	scd	%0, %1				\n"
			"	.set	mips0				\n"
			: "=&r" (temp), "=m" (v->counter)
			: "Ir" (i), "m" (v->counter));
		} while (unlikely(!temp));
	} else {
		unsigned long flags;

		raw_local_irq_save(flags);
		v->counter += i;
		raw_local_irq_restore(flags);
	}
}

/*
 * atomic64_sub - subtract the atomic variable
 * @i: integer value to subtract
 * @v: pointer of type atomic64_t
 *
 * Atomically subtracts @i from @v.
 */
static __inline__ void atomic64_sub(long i, atomic64_t * v)
{
	if (kernel_uses_llsc && R10000_LLSC_WAR) {
		long temp;

		__asm__ __volatile__(
		"	.set	mips3					\n"
		"1:	lld	%0, %1		# atomic64_sub		\n"
		"	dsubu	%0, %2					\n"
		"	scd	%0, %1					\n"
		"	beqzl	%0, 1b					\n"
		"	.set	mips0					\n"
		: "=&r" (temp), "=m" (v->counter)
		: "Ir" (i), "m" (v->counter));
	} else if (kernel_uses_llsc) {
		long temp;

		do {
			__asm__ __volatile__(
			"	.set	mips3				\n"
			"	lld	%0, %1		# atomic64_sub	\n"
			"	dsubu	%0, %2				\n"
			"	scd	%0, %1				\n"
			"	.set	mips0				\n"
			: "=&r" (temp), "=m" (v->counter)
			: "Ir" (i), "m" (v->counter));
		} while (unlikely(!temp));
	} else {
		unsigned long flags;

		raw_local_irq_save(flags);
		v->counter -= i;
		raw_local_irq_restore(flags);
	}
}

/*
 * Same as above, but return the result value
 */
static __inline__ long atomic64_add_return(long i, atomic64_t * v)
{
	long result;

	smp_mb__before_llsc();

	if (kernel_uses_llsc && R10000_LLSC_WAR) {
		long temp;

		__asm__ __volatile__(
		"	.set	mips3					\n"
		"1:	lld	%1, %2		# atomic64_add_return	\n"
		"	daddu	%0, %1, %3				\n"
		"	scd	%0, %2					\n"
		"	beqzl	%0, 1b					\n"
		"	daddu	%0, %1, %3				\n"
		"	.set	mips0					\n"
		: "=&r" (result), "=&r" (temp), "=m" (v->counter)
		: "Ir" (i), "m" (v->counter)
		: "memory");
	} else if (kernel_uses_llsc) {
		long temp;

		do {
			__asm__ __volatile__(
			"	.set	mips3				\n"
			"	lld	%1, %2	# atomic64_add_return	\n"
			"	daddu	%0, %1, %3			\n"
			"	scd	%0, %2				\n"
			"	.set	mips0				\n"
			: "=&r" (result), "=&r" (temp), "=m" (v->counter)
			: "Ir" (i), "m" (v->counter)
			: "memory");
		} while (unlikely(!result));

		result = temp + i;
	} else {
		unsigned long flags;

		raw_local_irq_save(flags);
		result = v->counter;
		result += i;
		v->counter = result;
		raw_local_irq_restore(flags);
	}

	smp_llsc_mb();

	return result;
}

static __inline__ long atomic64_sub_return(long i, atomic64_t * v)
{
	long result;

	smp_mb__before_llsc();

	if (kernel_uses_llsc && R10000_LLSC_WAR) {
		long temp;

		__asm__ __volatile__(
		"	.set	mips3					\n"
		"1:	lld	%1, %2		# atomic64_sub_return	\n"
		"	dsubu	%0, %1, %3				\n"
		"	scd	%0, %2					\n"
		"	beqzl	%0, 1b					\n"
		"	dsubu	%0, %1, %3				\n"
		"	.set	mips0					\n"
		: "=&r" (result), "=&r" (temp), "=m" (v->counter)
		: "Ir" (i), "m" (v->counter)
		: "memory");
	} else if (kernel_uses_llsc) {
		long temp;

		do {
			__asm__ __volatile__(
			"	.set	mips3				\n"
			"	lld	%1, %2	# atomic64_sub_return	\n"
			"	dsubu	%0, %1, %3			\n"
			"	scd	%0, %2				\n"
			"	.set	mips0				\n"
			: "=&r" (result), "=&r" (temp), "=m" (v->counter)
			: "Ir" (i), "m" (v->counter)
			: "memory");
		} while (unlikely(!result));

		result = temp - i;
	} else {
		unsigned long flags;

		raw_local_irq_save(flags);
		result = v->counter;
		result -= i;
		v->counter = result;
		raw_local_irq_restore(flags);
	}

	smp_llsc_mb();

	return result;
}

/*
 * atomic64_sub_if_positive - conditionally subtract integer from atomic variable
 * @i: integer value to subtract
 * @v: pointer of type atomic64_t
 *
 * Atomically test @v and subtract @i if @v is greater or equal than @i.
 * The function returns the old value of @v minus @i.
 */
static __inline__ long atomic64_sub_if_positive(long i, atomic64_t * v)
{
	long result;

	smp_mb__before_llsc();

	if (kernel_uses_llsc && R10000_LLSC_WAR) {
		long temp;

		__asm__ __volatile__(
		"	.set	mips3					\n"
		"1:	lld	%1, %2		# atomic64_sub_if_positive\n"
		"	dsubu	%0, %1, %3				\n"
		"	bltz	%0, 1f					\n"
		"	scd	%0, %2					\n"
		"	.set	noreorder				\n"
		"	beqzl	%0, 1b					\n"
		"	 dsubu	%0, %1, %3				\n"
		"	.set	reorder					\n"
		"1:							\n"
		"	.set	mips0					\n"
		: "=&r" (result), "=&r" (temp), "=m" (v->counter)
		: "Ir" (i), "m" (v->counter)
		: "memory");
	} else if (kernel_uses_llsc) {
		long temp;

		__asm__ __volatile__(
		"	.set	mips3					\n"
		"1:	lld	%1, %2		# atomic64_sub_if_positive\n"
		"	dsubu	%0, %1, %3				\n"
		"	bltz	%0, 1f					\n"
		"	scd	%0, %2					\n"
		"	.set	noreorder				\n"
		"	beqz	%0, 1b					\n"
		"	 dsubu	%0, %1, %3				\n"
		"	.set	reorder					\n"
		"1:							\n"
		"	.set	mips0					\n"
		: "=&r" (result), "=&r" (temp), "=m" (v->counter)
		: "Ir" (i), "m" (v->counter)
		: "memory");
	} else {
		unsigned long flags;

		raw_local_irq_save(flags);
		result = v->counter;
		result -= i;
		if (result >= 0)
			v->counter = result;
		raw_local_irq_restore(flags);
	}

	smp_llsc_mb();

	return result;
}

#define atomic64_cmpxchg(v, o, n) \
	((__typeof__((v)->counter))cmpxchg(&((v)->counter), (o), (n)))
#define atomic64_xchg(v, new) (xchg(&((v)->counter), (new)))

/**
 * atomic64_add_unless - add unless the number is a given value
 * @v: pointer of type atomic64_t
 * @a: the amount to add to v...
 * @u: ...unless v is equal to u.
 *
 * Atomically adds @a to @v, so long as it was not @u.
 * Returns true iff @v was not @u.
 */
static __inline__ int atomic64_add_unless(atomic64_t *v, long a, long u)
{
	long c, old;
	c = atomic64_read(v);
	for (;;) {
		if (unlikely(c == (u)))
			break;
		old = atomic64_cmpxchg((v), c, c + (a));
		if (likely(old == c))
			break;
		c = old;
	}
	return c != (u);
}

#define atomic64_inc_not_zero(v) atomic64_add_unless((v), 1, 0)

#define atomic64_dec_return(v) atomic64_sub_return(1, (v))
#define atomic64_inc_return(v) atomic64_add_return(1, (v))

/*
 * atomic64_sub_and_test - subtract value from variable and test result
 * @i: integer value to subtract
 * @v: pointer of type atomic64_t
 *
 * Atomically subtracts @i from @v and returns
 * true if the result is zero, or false for all
 * other cases.
 */
#define atomic64_sub_and_test(i, v) (atomic64_sub_return((i), (v)) == 0)

/*
 * atomic64_inc_and_test - increment and test
 * @v: pointer of type atomic64_t
 *
 * Atomically increments @v by 1
 * and returns true if the result is zero, or false for all
 * other cases.
 */
#define atomic64_inc_and_test(v) (atomic64_inc_return(v) == 0)

/*
 * atomic64_dec_and_test - decrement by 1 and test
 * @v: pointer of type atomic64_t
 *
 * Atomically decrements @v by 1 and
 * returns true if the result is 0, or false for all other
 * cases.
 */
#define atomic64_dec_and_test(v) (atomic64_sub_return(1, (v)) == 0)

/*
 * atomic64_dec_if_positive - decrement by 1 if old value positive
 * @v: pointer of type atomic64_t
 */
#define atomic64_dec_if_positive(v)	atomic64_sub_if_positive(1, v)

/*
 * atomic64_inc - increment atomic variable
 * @v: pointer of type atomic64_t
 *
 * Atomically increments @v by 1.
 */
#define atomic64_inc(v) atomic64_add(1, (v))

/*
 * atomic64_dec - decrement and test
 * @v: pointer of type atomic64_t
 *
 * Atomically decrements @v by 1.
 */
#define atomic64_dec(v) atomic64_sub(1, (v))

/*
 * atomic64_add_negative - add and test if negative
 * @v: pointer of type atomic64_t
 * @i: integer value to add
 *
 * Atomically adds @i to @v and returns true
 * if the result is negative, or false when
 * result is greater than or equal to zero.
 */
#define atomic64_add_negative(i, v) (atomic64_add_return(i, (v)) < 0)

#endif /* CONFIG_64BIT */

/*
 * atomic*_return operations are serializing but not the non-*_return
 * versions.
 */
#define smp_mb__before_atomic_dec()	smp_mb__before_llsc()
#define smp_mb__after_atomic_dec()	smp_llsc_mb()
#define smp_mb__before_atomic_inc()	smp_mb__before_llsc()
#define smp_mb__after_atomic_inc()	smp_llsc_mb()
=======
#define ATOMIC_SIP_OP(pfx, type, op, ll, sc)				\
static __inline__ type arch_##pfx##_sub_if_positive(type i, pfx##_t * v)	\
{									\
	type temp, result;						\
									\
	smp_mb__before_atomic();					\
									\
	if (!kernel_uses_llsc) {					\
		unsigned long flags;					\
									\
		raw_local_irq_save(flags);				\
		result = v->counter;					\
		result -= i;						\
		if (result >= 0)					\
			v->counter = result;				\
		raw_local_irq_restore(flags);				\
		smp_mb__after_atomic();					\
		return result;						\
	}								\
									\
	__asm__ __volatile__(						\
	"	.set	push					\n"	\
	"	.set	" MIPS_ISA_LEVEL "			\n"	\
	"	" __SYNC(full, loongson3_war) "			\n"	\
	"1:	" #ll "	%1, %2		# atomic_sub_if_positive\n"	\
	"	.set	pop					\n"	\
	"	" #op "	%0, %1, %3				\n"	\
	"	move	%1, %0					\n"	\
	"	bltz	%0, 2f					\n"	\
	"	.set	push					\n"	\
	"	.set	" MIPS_ISA_LEVEL "			\n"	\
	"	" #sc "	%1, %2					\n"	\
	"	" __stringify(SC_BEQZ) "	%1, 1b		\n"	\
	"2:	" __SYNC(full, loongson3_war) "			\n"	\
	"	.set	pop					\n"	\
	: "=&r" (result), "=&r" (temp),					\
	  "+" GCC_OFF_SMALL_ASM() (v->counter)				\
	: "Ir" (i)							\
	: __LLSC_CLOBBER);						\
									\
	/*								\
	 * In the Loongson3 workaround case we already have a		\
	 * completion barrier at 2: above, which is needed due to the	\
	 * bltz that can branch	to code outside of the LL/SC loop. As	\
	 * such, we don't need to emit another barrier here.		\
	 */								\
	if (__SYNC_loongson3_war == 0)					\
		smp_mb__after_atomic();					\
									\
	return result;							\
}

ATOMIC_SIP_OP(atomic, int, subu, ll, sc)
#define arch_atomic_dec_if_positive(v)	arch_atomic_sub_if_positive(1, v)

#ifdef CONFIG_64BIT
ATOMIC_SIP_OP(atomic64, s64, dsubu, lld, scd)
#define arch_atomic64_dec_if_positive(v)	arch_atomic64_sub_if_positive(1, v)
#endif

#undef ATOMIC_SIP_OP
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

#endif /* _ASM_ATOMIC_H */
