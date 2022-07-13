<<<<<<< HEAD
=======
/* SPDX-License-Identifier: GPL-2.0 */
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
/*
 * Cache operations
 *
 * Copyright (C) 2007-2009 Michal Simek <monstr@monstr.eu>
 * Copyright (C) 2007-2009 PetaLogix
 * Copyright (C) 2003 John Williams <jwilliams@itee.uq.edu.au>
<<<<<<< HEAD
 *
 * This file is subject to the terms and conditions of the GNU General
 * Public License. See the file COPYING in the main directory of this
 * archive for more details.
=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 */

#ifndef _ASM_MICROBLAZE_CACHE_H
#define _ASM_MICROBLAZE_CACHE_H

#include <asm/registers.h>

#define L1_CACHE_SHIFT 5
/* word-granular cache in microblaze */
#define L1_CACHE_BYTES	(1 << L1_CACHE_SHIFT)

#define SMP_CACHE_BYTES	L1_CACHE_BYTES

<<<<<<< HEAD
=======
/* MS be sure that SLAB allocates aligned objects */
#define ARCH_DMA_MINALIGN	L1_CACHE_BYTES

#define ARCH_SLAB_MINALIGN	L1_CACHE_BYTES

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#endif /* _ASM_MICROBLAZE_CACHE_H */
