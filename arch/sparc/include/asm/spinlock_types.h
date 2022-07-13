<<<<<<< HEAD
#ifndef __SPARC_SPINLOCK_TYPES_H
#define __SPARC_SPINLOCK_TYPES_H

#ifndef __LINUX_SPINLOCK_TYPES_H
# error "please don't include this file directly"
#endif
=======
/* SPDX-License-Identifier: GPL-2.0 */
#ifndef __SPARC_SPINLOCK_TYPES_H
#define __SPARC_SPINLOCK_TYPES_H

#ifdef CONFIG_QUEUED_SPINLOCKS
#include <asm-generic/qspinlock_types.h>
#else
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

typedef struct {
	volatile unsigned char lock;
} arch_spinlock_t;

#define __ARCH_SPIN_LOCK_UNLOCKED	{ 0 }
<<<<<<< HEAD

=======
#endif /* CONFIG_QUEUED_SPINLOCKS */

#ifdef CONFIG_QUEUED_RWLOCKS
#include <asm-generic/qrwlock_types.h>
#else
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
typedef struct {
	volatile unsigned int lock;
} arch_rwlock_t;

#define __ARCH_RW_LOCK_UNLOCKED		{ 0 }
<<<<<<< HEAD

=======
#endif /* CONFIG_QUEUED_RWLOCKS */
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#endif
