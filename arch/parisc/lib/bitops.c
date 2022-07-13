<<<<<<< HEAD
=======
// SPDX-License-Identifier: GPL-2.0
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
/*
 * bitops.c: atomic operations which got too long to be inlined all over
 *      the place.
 * 
 * Copyright 1999 Philipp Rumpf (prumpf@tux.org)
 * Copyright 2000 Grant Grundler (grundler@cup.hp.com)
 */

#include <linux/kernel.h>
#include <linux/spinlock.h>
#include <linux/atomic.h>

#ifdef CONFIG_SMP
arch_spinlock_t __atomic_hash[ATOMIC_HASH_SIZE] __lock_aligned = {
	[0 ... (ATOMIC_HASH_SIZE-1)]  = __ARCH_SPIN_LOCK_UNLOCKED
};
#endif

#ifdef CONFIG_64BIT
<<<<<<< HEAD
unsigned long __xchg64(unsigned long x, unsigned long *ptr)
=======
unsigned long notrace __xchg64(unsigned long x, volatile unsigned long *ptr)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
{
	unsigned long temp, flags;

	_atomic_spin_lock_irqsave(ptr, flags);
	temp = *ptr;
	*ptr = x;
	_atomic_spin_unlock_irqrestore(ptr, flags);
	return temp;
}
#endif

<<<<<<< HEAD
unsigned long __xchg32(int x, int *ptr)
=======
unsigned long notrace __xchg32(int x, volatile int *ptr)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
{
	unsigned long flags;
	long temp;

	_atomic_spin_lock_irqsave(ptr, flags);
	temp = (long) *ptr;	/* XXX - sign extension wanted? */
	*ptr = x;
	_atomic_spin_unlock_irqrestore(ptr, flags);
	return (unsigned long)temp;
}


<<<<<<< HEAD
unsigned long __xchg8(char x, char *ptr)
=======
unsigned long notrace __xchg8(char x, volatile char *ptr)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
{
	unsigned long flags;
	long temp;

	_atomic_spin_lock_irqsave(ptr, flags);
	temp = (long) *ptr;	/* XXX - sign extension wanted? */
	*ptr = x;
	_atomic_spin_unlock_irqrestore(ptr, flags);
	return (unsigned long)temp;
}


<<<<<<< HEAD
#ifdef CONFIG_64BIT
unsigned long __cmpxchg_u64(volatile unsigned long *ptr, unsigned long old, unsigned long new)
{
	unsigned long flags;
	unsigned long prev;
=======
u64 notrace __cmpxchg_u64(volatile u64 *ptr, u64 old, u64 new)
{
	unsigned long flags;
	u64 prev;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	_atomic_spin_lock_irqsave(ptr, flags);
	if ((prev = *ptr) == old)
		*ptr = new;
	_atomic_spin_unlock_irqrestore(ptr, flags);
	return prev;
}
<<<<<<< HEAD
#endif

unsigned long __cmpxchg_u32(volatile unsigned int *ptr, unsigned int old, unsigned int new)
=======

unsigned long notrace __cmpxchg_u32(volatile unsigned int *ptr, unsigned int old, unsigned int new)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
{
	unsigned long flags;
	unsigned int prev;

	_atomic_spin_lock_irqsave(ptr, flags);
	if ((prev = *ptr) == old)
		*ptr = new;
	_atomic_spin_unlock_irqrestore(ptr, flags);
	return (unsigned long)prev;
}
<<<<<<< HEAD
=======

u8 notrace __cmpxchg_u8(volatile u8 *ptr, u8 old, u8 new)
{
	unsigned long flags;
	u8 prev;

	_atomic_spin_lock_irqsave(ptr, flags);
	if ((prev = *ptr) == old)
		*ptr = new;
	_atomic_spin_unlock_irqrestore(ptr, flags);
	return prev;
}
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
