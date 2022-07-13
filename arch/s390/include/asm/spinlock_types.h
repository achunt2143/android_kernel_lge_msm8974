<<<<<<< HEAD
#ifndef __ASM_SPINLOCK_TYPES_H
#define __ASM_SPINLOCK_TYPES_H

#ifndef __LINUX_SPINLOCK_TYPES_H
=======
/* SPDX-License-Identifier: GPL-2.0 */
#ifndef __ASM_SPINLOCK_TYPES_H
#define __ASM_SPINLOCK_TYPES_H

#ifndef __LINUX_SPINLOCK_TYPES_RAW_H
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
# error "please don't include this file directly"
#endif

typedef struct {
<<<<<<< HEAD
	volatile unsigned int owner_cpu;
} __attribute__ ((aligned (4))) arch_spinlock_t;

#define __ARCH_SPIN_LOCK_UNLOCKED	{ 0 }

typedef struct {
	volatile unsigned int lock;
=======
	int lock;
} arch_spinlock_t;

#define __ARCH_SPIN_LOCK_UNLOCKED { .lock = 0, }

typedef struct {
	int cnts;
	arch_spinlock_t wait;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
} arch_rwlock_t;

#define __ARCH_RW_LOCK_UNLOCKED		{ 0 }

#endif
