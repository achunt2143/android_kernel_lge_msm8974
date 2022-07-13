/*
<<<<<<< HEAD
 * include/asm-xtensa/mmu.h
 *
=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 * This file is subject to the terms and conditions of the GNU General Public
 * License.  See the file "COPYING" in the main directory of this archive
 * for more details.
 *
<<<<<<< HEAD
 * Copyright (C) 2001 - 2005 Tensilica Inc.
=======
 * Copyright (C) 2001 - 2013 Tensilica Inc.
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 */

#ifndef _XTENSA_MMU_H
#define _XTENSA_MMU_H

#ifndef CONFIG_MMU
<<<<<<< HEAD
#include <asm/nommu.h>
#else

/* Default "unsigned long" context */
typedef unsigned long mm_context_t;
=======
#include <asm-generic/mmu.h>
#else

typedef struct {
	unsigned long asid[NR_CPUS];
	unsigned int cpu;
} mm_context_t;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

#endif /* CONFIG_MMU */
#endif	/* _XTENSA_MMU_H */
