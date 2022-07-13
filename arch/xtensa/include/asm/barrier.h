/*
 * This file is subject to the terms and conditions of the GNU General Public
 * License.  See the file "COPYING" in the main directory of this archive
 * for more details.
 *
<<<<<<< HEAD
 * Copyright (C) 2001 - 2005 Tensilica Inc.
=======
 * Copyright (C) 2001 - 2012 Tensilica Inc.
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 */

#ifndef _XTENSA_SYSTEM_H
#define _XTENSA_SYSTEM_H

<<<<<<< HEAD
#define smp_read_barrier_depends() do { } while(0)
#define read_barrier_depends() do { } while(0)

#define mb()  barrier()
#define rmb() mb()
#define wmb() mb()

#ifdef CONFIG_SMP
#error smp_* not defined
#else
#define smp_mb()	barrier()
#define smp_rmb()	barrier()
#define smp_wmb()	barrier()
#endif

#define set_mb(var, value)	do { var = value; mb(); } while (0)
=======
#include <asm/core.h>

#define __mb()  ({ __asm__ __volatile__("memw" : : : "memory"); })
#define __rmb() barrier()
#define __wmb() __mb()

#ifdef CONFIG_SMP
#define __smp_mb() __mb()
#define __smp_rmb() __rmb()
#define __smp_wmb() __wmb()
#endif

#if XCHAL_HAVE_S32C1I
#define __smp_mb__before_atomic()		barrier()
#define __smp_mb__after_atomic()		barrier()
#endif

#include <asm-generic/barrier.h>
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

#endif /* _XTENSA_SYSTEM_H */
