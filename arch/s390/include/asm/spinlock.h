<<<<<<< HEAD
/*
 *  include/asm-s390/spinlock.h
 *
 *  S390 version
 *    Copyright (C) 1999 IBM Deutschland Entwicklung GmbH, IBM Corporation
=======
/* SPDX-License-Identifier: GPL-2.0 */
/*
 *  S390 version
 *    Copyright IBM Corp. 1999
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 *    Author(s): Martin Schwidefsky (schwidefsky@de.ibm.com)
 *
 *  Derived from "include/asm-i386/spinlock.h"
 */

#ifndef __ASM_SPINLOCK_H
#define __ASM_SPINLOCK_H

#include <linux/smp.h>
<<<<<<< HEAD

extern int spin_retry;

static inline int
_raw_compare_and_swap(volatile unsigned int *lock,
		      unsigned int old, unsigned int new)
{
	asm volatile(
		"	cs	%0,%3,%1"
		: "=d" (old), "=Q" (*lock)
		: "0" (old), "d" (new), "Q" (*lock)
		: "cc", "memory" );
	return old;
}
=======
#include <asm/atomic_ops.h>
#include <asm/barrier.h>
#include <asm/processor.h>
#include <asm/alternative.h>

#define SPINLOCK_LOCKVAL (S390_lowcore.spinlock_lockval)

extern int spin_retry;

bool arch_vcpu_is_preempted(int cpu);

#define vcpu_is_preempted arch_vcpu_is_preempted
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

/*
 * Simple spin lock operations.  There are two variants, one clears IRQ's
 * on the local processor, one does not.
 *
 * We make no fairness assumptions. They have a cost.
 *
 * (the type definitions are in asm/spinlock_types.h)
 */

<<<<<<< HEAD
#define arch_spin_is_locked(x) ((x)->owner_cpu != 0)
#define arch_spin_unlock_wait(lock) \
	do { while (arch_spin_is_locked(lock)) \
		 arch_spin_relax(lock); } while (0)

extern void arch_spin_lock_wait(arch_spinlock_t *);
extern void arch_spin_lock_wait_flags(arch_spinlock_t *, unsigned long flags);
extern int arch_spin_trylock_retry(arch_spinlock_t *);
extern void arch_spin_relax(arch_spinlock_t *lock);

static inline void arch_spin_lock(arch_spinlock_t *lp)
{
	int old;

	old = _raw_compare_and_swap(&lp->owner_cpu, 0, ~smp_processor_id());
	if (likely(old == 0))
		return;
	arch_spin_lock_wait(lp);
}

static inline void arch_spin_lock_flags(arch_spinlock_t *lp,
					 unsigned long flags)
{
	int old;

	old = _raw_compare_and_swap(&lp->owner_cpu, 0, ~smp_processor_id());
	if (likely(old == 0))
		return;
	arch_spin_lock_wait_flags(lp, flags);
=======
void arch_spin_relax(arch_spinlock_t *lock);
#define arch_spin_relax	arch_spin_relax

void arch_spin_lock_wait(arch_spinlock_t *);
int arch_spin_trylock_retry(arch_spinlock_t *);
void arch_spin_lock_setup(int cpu);

static inline u32 arch_spin_lockval(int cpu)
{
	return cpu + 1;
}

static inline int arch_spin_value_unlocked(arch_spinlock_t lock)
{
	return lock.lock == 0;
}

static inline int arch_spin_is_locked(arch_spinlock_t *lp)
{
	return READ_ONCE(lp->lock) != 0;
}

static inline int arch_spin_trylock_once(arch_spinlock_t *lp)
{
	barrier();
	return likely(__atomic_cmpxchg_bool(&lp->lock, 0, SPINLOCK_LOCKVAL));
}

static inline void arch_spin_lock(arch_spinlock_t *lp)
{
	if (!arch_spin_trylock_once(lp))
		arch_spin_lock_wait(lp);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

static inline int arch_spin_trylock(arch_spinlock_t *lp)
{
<<<<<<< HEAD
	int old;

	old = _raw_compare_and_swap(&lp->owner_cpu, 0, ~smp_processor_id());
	if (likely(old == 0))
		return 1;
	return arch_spin_trylock_retry(lp);
=======
	if (!arch_spin_trylock_once(lp))
		return arch_spin_trylock_retry(lp);
	return 1;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

static inline void arch_spin_unlock(arch_spinlock_t *lp)
{
<<<<<<< HEAD
	_raw_compare_and_swap(&lp->owner_cpu, lp->owner_cpu, 0);
}
		
=======
	typecheck(int, lp->lock);
	kcsan_release();
	asm_inline volatile(
		ALTERNATIVE("nop", ".insn rre,0xb2fa0000,7,0", 49) /* NIAI 7 */
		"	sth	%1,%0\n"
		: "=R" (((unsigned short *) &lp->lock)[1])
		: "d" (0) : "cc", "memory");
}

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
/*
 * Read-write spinlocks, allowing multiple readers
 * but only one writer.
 *
 * NOTE! it is quite common to have readers in interrupts
 * but no interrupt writers. For those circumstances we
 * can "mix" irq-safe locks - any writer needs to get a
 * irq-safe write-lock, but readers can get non-irqsafe
 * read-locks.
 */

<<<<<<< HEAD
/**
 * read_can_lock - would read_trylock() succeed?
 * @lock: the rwlock in question.
 */
#define arch_read_can_lock(x) ((int)(x)->lock >= 0)

/**
 * write_can_lock - would write_trylock() succeed?
 * @lock: the rwlock in question.
 */
#define arch_write_can_lock(x) ((x)->lock == 0)

extern void _raw_read_lock_wait(arch_rwlock_t *lp);
extern void _raw_read_lock_wait_flags(arch_rwlock_t *lp, unsigned long flags);
extern int _raw_read_trylock_retry(arch_rwlock_t *lp);
extern void _raw_write_lock_wait(arch_rwlock_t *lp);
extern void _raw_write_lock_wait_flags(arch_rwlock_t *lp, unsigned long flags);
extern int _raw_write_trylock_retry(arch_rwlock_t *lp);

static inline void arch_read_lock(arch_rwlock_t *rw)
{
	unsigned int old;
	old = rw->lock & 0x7fffffffU;
	if (_raw_compare_and_swap(&rw->lock, old, old + 1) != old)
		_raw_read_lock_wait(rw);
}

static inline void arch_read_lock_flags(arch_rwlock_t *rw, unsigned long flags)
{
	unsigned int old;
	old = rw->lock & 0x7fffffffU;
	if (_raw_compare_and_swap(&rw->lock, old, old + 1) != old)
		_raw_read_lock_wait_flags(rw, flags);
=======
#define arch_read_relax(rw) barrier()
#define arch_write_relax(rw) barrier()

void arch_read_lock_wait(arch_rwlock_t *lp);
void arch_write_lock_wait(arch_rwlock_t *lp);

static inline void arch_read_lock(arch_rwlock_t *rw)
{
	int old;

	old = __atomic_add(1, &rw->cnts);
	if (old & 0xffff0000)
		arch_read_lock_wait(rw);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

static inline void arch_read_unlock(arch_rwlock_t *rw)
{
<<<<<<< HEAD
	unsigned int old, cmp;

	old = rw->lock;
	do {
		cmp = old;
		old = _raw_compare_and_swap(&rw->lock, old, old - 1);
	} while (cmp != old);
=======
	__atomic_add_const_barrier(-1, &rw->cnts);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

static inline void arch_write_lock(arch_rwlock_t *rw)
{
<<<<<<< HEAD
	if (unlikely(_raw_compare_and_swap(&rw->lock, 0, 0x80000000) != 0))
		_raw_write_lock_wait(rw);
}

static inline void arch_write_lock_flags(arch_rwlock_t *rw, unsigned long flags)
{
	if (unlikely(_raw_compare_and_swap(&rw->lock, 0, 0x80000000) != 0))
		_raw_write_lock_wait_flags(rw, flags);
=======
	if (!__atomic_cmpxchg_bool(&rw->cnts, 0, 0x30000))
		arch_write_lock_wait(rw);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

static inline void arch_write_unlock(arch_rwlock_t *rw)
{
<<<<<<< HEAD
	_raw_compare_and_swap(&rw->lock, 0x80000000, 0);
}

static inline int arch_read_trylock(arch_rwlock_t *rw)
{
	unsigned int old;
	old = rw->lock & 0x7fffffffU;
	if (likely(_raw_compare_and_swap(&rw->lock, old, old + 1) == old))
		return 1;
	return _raw_read_trylock_retry(rw);
=======
	__atomic_add_barrier(-0x30000, &rw->cnts);
}


static inline int arch_read_trylock(arch_rwlock_t *rw)
{
	int old;

	old = READ_ONCE(rw->cnts);
	return (!(old & 0xffff0000) &&
		__atomic_cmpxchg_bool(&rw->cnts, old, old + 1));
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

static inline int arch_write_trylock(arch_rwlock_t *rw)
{
<<<<<<< HEAD
	if (likely(_raw_compare_and_swap(&rw->lock, 0, 0x80000000) == 0))
		return 1;
	return _raw_write_trylock_retry(rw);
}

#define arch_read_relax(lock)	cpu_relax()
#define arch_write_relax(lock)	cpu_relax()

=======
	int old;

	old = READ_ONCE(rw->cnts);
	return !old && __atomic_cmpxchg_bool(&rw->cnts, 0, 0x30000);
}

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#endif /* __ASM_SPINLOCK_H */
