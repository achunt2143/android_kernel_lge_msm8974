<<<<<<< HEAD
=======
/* SPDX-License-Identifier: GPL-2.0 */
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#ifndef __ARCH_M68K_ATOMIC__
#define __ARCH_M68K_ATOMIC__

#include <linux/types.h>
#include <linux/irqflags.h>
#include <asm/cmpxchg.h>
<<<<<<< HEAD
=======
#include <asm/barrier.h>
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

/*
 * Atomic operations that C can't guarantee us.  Useful for
 * resource counting etc..
 */

/*
 * We do not have SMP m68k systems, so we don't have to deal with that.
 */

<<<<<<< HEAD
#define ATOMIC_INIT(i)	{ (i) }

#define atomic_read(v)		(*(volatile int *)&(v)->counter)
#define atomic_set(v, i)	(((v)->counter) = i)
=======
#define arch_atomic_read(v)	READ_ONCE((v)->counter)
#define arch_atomic_set(v, i)	WRITE_ONCE(((v)->counter), (i))
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

/*
 * The ColdFire parts cannot do some immediate to memory operations,
 * so for them we do not specify the "i" asm constraint.
 */
#ifdef CONFIG_COLDFIRE
#define	ASM_DI	"d"
#else
#define	ASM_DI	"di"
#endif

<<<<<<< HEAD
static inline void atomic_add(int i, atomic_t *v)
{
	__asm__ __volatile__("addl %1,%0" : "+m" (*v) : ASM_DI (i));
}

static inline void atomic_sub(int i, atomic_t *v)
{
	__asm__ __volatile__("subl %1,%0" : "+m" (*v) : ASM_DI (i));
}

static inline void atomic_inc(atomic_t *v)
{
	__asm__ __volatile__("addql #1,%0" : "+m" (*v));
}

static inline void atomic_dec(atomic_t *v)
{
	__asm__ __volatile__("subql #1,%0" : "+m" (*v));
}

static inline int atomic_dec_and_test(atomic_t *v)
=======
#define ATOMIC_OP(op, c_op, asm_op)					\
static inline void arch_atomic_##op(int i, atomic_t *v)			\
{									\
	__asm__ __volatile__(#asm_op "l %1,%0" : "+m" (*v) : ASM_DI (i));\
}									\

#ifdef CONFIG_RMW_INSNS

#define ATOMIC_OP_RETURN(op, c_op, asm_op)				\
static inline int arch_atomic_##op##_return(int i, atomic_t *v)		\
{									\
	int t, tmp;							\
									\
	__asm__ __volatile__(						\
			"1:	movel %2,%1\n"				\
			"	" #asm_op "l %3,%1\n"			\
			"	casl %2,%1,%0\n"			\
			"	jne 1b"					\
			: "+m" (*v), "=&d" (t), "=&d" (tmp)		\
			: "di" (i), "2" (arch_atomic_read(v)));		\
	return t;							\
}

#define ATOMIC_FETCH_OP(op, c_op, asm_op)				\
static inline int arch_atomic_fetch_##op(int i, atomic_t *v)		\
{									\
	int t, tmp;							\
									\
	__asm__ __volatile__(						\
			"1:	movel %2,%1\n"				\
			"	" #asm_op "l %3,%1\n"			\
			"	casl %2,%1,%0\n"			\
			"	jne 1b"					\
			: "+m" (*v), "=&d" (t), "=&d" (tmp)		\
			: "di" (i), "2" (arch_atomic_read(v)));		\
	return tmp;							\
}

#else

#define ATOMIC_OP_RETURN(op, c_op, asm_op)				\
static inline int arch_atomic_##op##_return(int i, atomic_t * v)	\
{									\
	unsigned long flags;						\
	int t;								\
									\
	local_irq_save(flags);						\
	t = (v->counter c_op i);					\
	local_irq_restore(flags);					\
									\
	return t;							\
}

#define ATOMIC_FETCH_OP(op, c_op, asm_op)				\
static inline int arch_atomic_fetch_##op(int i, atomic_t * v)		\
{									\
	unsigned long flags;						\
	int t;								\
									\
	local_irq_save(flags);						\
	t = v->counter;							\
	v->counter c_op i;						\
	local_irq_restore(flags);					\
									\
	return t;							\
}

#endif /* CONFIG_RMW_INSNS */

#define ATOMIC_OPS(op, c_op, asm_op)					\
	ATOMIC_OP(op, c_op, asm_op)					\
	ATOMIC_OP_RETURN(op, c_op, asm_op)				\
	ATOMIC_FETCH_OP(op, c_op, asm_op)

ATOMIC_OPS(add, +=, add)
ATOMIC_OPS(sub, -=, sub)

#define arch_atomic_add_return			arch_atomic_add_return
#define arch_atomic_sub_return			arch_atomic_sub_return
#define arch_atomic_fetch_add			arch_atomic_fetch_add
#define arch_atomic_fetch_sub			arch_atomic_fetch_sub

#undef ATOMIC_OPS
#define ATOMIC_OPS(op, c_op, asm_op)					\
	ATOMIC_OP(op, c_op, asm_op)					\
	ATOMIC_FETCH_OP(op, c_op, asm_op)

ATOMIC_OPS(and, &=, and)
ATOMIC_OPS(or, |=, or)
ATOMIC_OPS(xor, ^=, eor)

#define arch_atomic_fetch_and			arch_atomic_fetch_and
#define arch_atomic_fetch_or			arch_atomic_fetch_or
#define arch_atomic_fetch_xor			arch_atomic_fetch_xor

#undef ATOMIC_OPS
#undef ATOMIC_FETCH_OP
#undef ATOMIC_OP_RETURN
#undef ATOMIC_OP

static inline void arch_atomic_inc(atomic_t *v)
{
	__asm__ __volatile__("addql #1,%0" : "+m" (*v));
}
#define arch_atomic_inc arch_atomic_inc

static inline void arch_atomic_dec(atomic_t *v)
{
	__asm__ __volatile__("subql #1,%0" : "+m" (*v));
}
#define arch_atomic_dec arch_atomic_dec

static inline int arch_atomic_dec_and_test(atomic_t *v)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
{
	char c;
	__asm__ __volatile__("subql #1,%1; seq %0" : "=d" (c), "+m" (*v));
	return c != 0;
}
<<<<<<< HEAD

static inline int atomic_dec_and_test_lt(atomic_t *v)
=======
#define arch_atomic_dec_and_test arch_atomic_dec_and_test

static inline int arch_atomic_dec_and_test_lt(atomic_t *v)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
{
	char c;
	__asm__ __volatile__(
		"subql #1,%1; slt %0"
		: "=d" (c), "=m" (*v)
		: "m" (*v));
	return c != 0;
}

<<<<<<< HEAD
static inline int atomic_inc_and_test(atomic_t *v)
=======
static inline int arch_atomic_inc_and_test(atomic_t *v)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
{
	char c;
	__asm__ __volatile__("addql #1,%1; seq %0" : "=d" (c), "+m" (*v));
	return c != 0;
}
<<<<<<< HEAD

#ifdef CONFIG_RMW_INSNS

static inline int atomic_add_return(int i, atomic_t *v)
{
	int t, tmp;

	__asm__ __volatile__(
			"1:	movel %2,%1\n"
			"	addl %3,%1\n"
			"	casl %2,%1,%0\n"
			"	jne 1b"
			: "+m" (*v), "=&d" (t), "=&d" (tmp)
			: "g" (i), "2" (atomic_read(v)));
	return t;
}

static inline int atomic_sub_return(int i, atomic_t *v)
{
	int t, tmp;

	__asm__ __volatile__(
			"1:	movel %2,%1\n"
			"	subl %3,%1\n"
			"	casl %2,%1,%0\n"
			"	jne 1b"
			: "+m" (*v), "=&d" (t), "=&d" (tmp)
			: "g" (i), "2" (atomic_read(v)));
	return t;
}

#define atomic_cmpxchg(v, o, n) ((int)cmpxchg(&((v)->counter), (o), (n)))
#define atomic_xchg(v, new) (xchg(&((v)->counter), new))

#else /* !CONFIG_RMW_INSNS */

static inline int atomic_add_return(int i, atomic_t * v)
{
	unsigned long flags;
	int t;

	local_irq_save(flags);
	t = atomic_read(v);
	t += i;
	atomic_set(v, t);
	local_irq_restore(flags);

	return t;
}

static inline int atomic_sub_return(int i, atomic_t * v)
{
	unsigned long flags;
	int t;

	local_irq_save(flags);
	t = atomic_read(v);
	t -= i;
	atomic_set(v, t);
	local_irq_restore(flags);

	return t;
}

static inline int atomic_cmpxchg(atomic_t *v, int old, int new)
=======
#define arch_atomic_inc_and_test arch_atomic_inc_and_test

#ifndef CONFIG_RMW_INSNS

static inline int arch_atomic_cmpxchg(atomic_t *v, int old, int new)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
{
	unsigned long flags;
	int prev;

	local_irq_save(flags);
<<<<<<< HEAD
	prev = atomic_read(v);
	if (prev == old)
		atomic_set(v, new);
	local_irq_restore(flags);
	return prev;
}

static inline int atomic_xchg(atomic_t *v, int new)
=======
	prev = arch_atomic_read(v);
	if (prev == old)
		arch_atomic_set(v, new);
	local_irq_restore(flags);
	return prev;
}
#define arch_atomic_cmpxchg arch_atomic_cmpxchg

static inline int arch_atomic_xchg(atomic_t *v, int new)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
{
	unsigned long flags;
	int prev;

	local_irq_save(flags);
<<<<<<< HEAD
	prev = atomic_read(v);
	atomic_set(v, new);
	local_irq_restore(flags);
	return prev;
}

#endif /* !CONFIG_RMW_INSNS */

#define atomic_dec_return(v)	atomic_sub_return(1, (v))
#define atomic_inc_return(v)	atomic_add_return(1, (v))

static inline int atomic_sub_and_test(int i, atomic_t *v)
=======
	prev = arch_atomic_read(v);
	arch_atomic_set(v, new);
	local_irq_restore(flags);
	return prev;
}
#define arch_atomic_xchg arch_atomic_xchg

#endif /* !CONFIG_RMW_INSNS */

static inline int arch_atomic_sub_and_test(int i, atomic_t *v)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
{
	char c;
	__asm__ __volatile__("subl %2,%1; seq %0"
			     : "=d" (c), "+m" (*v)
			     : ASM_DI (i));
	return c != 0;
}
<<<<<<< HEAD

static inline int atomic_add_negative(int i, atomic_t *v)
=======
#define arch_atomic_sub_and_test arch_atomic_sub_and_test

static inline int arch_atomic_add_negative(int i, atomic_t *v)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
{
	char c;
	__asm__ __volatile__("addl %2,%1; smi %0"
			     : "=d" (c), "+m" (*v)
			     : ASM_DI (i));
	return c != 0;
}
<<<<<<< HEAD

static inline void atomic_clear_mask(unsigned long mask, unsigned long *v)
{
	__asm__ __volatile__("andl %1,%0" : "+m" (*v) : ASM_DI (~(mask)));
}

static inline void atomic_set_mask(unsigned long mask, unsigned long *v)
{
	__asm__ __volatile__("orl %1,%0" : "+m" (*v) : ASM_DI (mask));
}

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


/* Atomic operations are already serializing */
#define smp_mb__before_atomic_dec()	barrier()
#define smp_mb__after_atomic_dec()	barrier()
#define smp_mb__before_atomic_inc()	barrier()
#define smp_mb__after_atomic_inc()	barrier()
=======
#define arch_atomic_add_negative arch_atomic_add_negative
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

#endif /* __ARCH_M68K_ATOMIC __ */
