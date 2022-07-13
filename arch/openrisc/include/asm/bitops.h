<<<<<<< HEAD
=======
/* SPDX-License-Identifier: GPL-2.0-or-later */
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
/*
 * OpenRISC Linux
 *
 * Linux architectural port borrowing liberally from similar works of
 * others.  All original copyrights apply as per the original source
 * declaration.
 *
 * OpenRISC implementation:
 * Copyright (C) 2003 Matjaz Breskvar <phoenix@bsemi.com>
 * Copyright (C) 2010-2011 Jonas Bonn <jonas@southpole.se>
 * et al.
<<<<<<< HEAD
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 */

#ifndef __ASM_OPENRISC_BITOPS_H
#define __ASM_OPENRISC_BITOPS_H

/*
 * Where we haven't written assembly versions yet, we fall back to the
 * generic implementations.  Otherwise, we pull in our (hopefully)
 * optimized versions.
 */

#include <linux/irqflags.h>
#include <linux/compiler.h>
<<<<<<< HEAD

/*
 * clear_bit may not imply a memory barrier
 */
#ifndef smp_mb__before_clear_bit
#define smp_mb__before_clear_bit()	smp_mb()
#define smp_mb__after_clear_bit()	smp_mb()
#endif
=======
#include <asm/barrier.h>
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

#include <asm/bitops/__ffs.h>
#include <asm-generic/bitops/ffz.h>
#include <asm/bitops/fls.h>
#include <asm/bitops/__fls.h>
#include <asm-generic/bitops/fls64.h>
<<<<<<< HEAD
#include <asm-generic/bitops/find.h>
=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

#ifndef _LINUX_BITOPS_H
#error only <linux/bitops.h> can be included directly
#endif

#include <asm-generic/bitops/sched.h>
#include <asm/bitops/ffs.h>
#include <asm-generic/bitops/hweight.h>
#include <asm-generic/bitops/lock.h>

<<<<<<< HEAD
#include <asm-generic/bitops/atomic.h>
#include <asm-generic/bitops/non-atomic.h>
=======
#include <asm/bitops/atomic.h>
#include <asm-generic/bitops/non-atomic.h>
#include <asm-generic/bitops/le.h>
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#include <asm-generic/bitops/ext2-atomic.h>

#endif /* __ASM_GENERIC_BITOPS_H */
