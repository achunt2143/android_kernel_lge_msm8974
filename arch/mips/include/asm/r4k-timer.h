/*
 * This file is subject to the terms and conditions of the GNU General Public
 * License.  See the file "COPYING" in the main directory of this archive
 * for more details.
 *
 * Copyright (C) 2008 by Ralf Baechle (ralf@linux-mips.org)
 */
<<<<<<< HEAD
#ifndef __ASM_R4K_TYPES_H
#define __ASM_R4K_TYPES_H
=======
#ifndef __ASM_R4K_TIMER_H
#define __ASM_R4K_TIMER_H
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

#include <linux/compiler.h>

#ifdef CONFIG_SYNC_R4K

<<<<<<< HEAD
extern void synchronise_count_master(void);
extern void synchronise_count_slave(void);

#else

static inline void synchronise_count_master(void)
{
}

static inline void synchronise_count_slave(void)
=======
extern void synchronise_count_master(int cpu);
extern void synchronise_count_slave(int cpu);

#else

static inline void synchronise_count_master(int cpu)
{
}

static inline void synchronise_count_slave(int cpu)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
{
}

#endif

<<<<<<< HEAD
#endif /* __ASM_R4K_TYPES_H */
=======
#endif /* __ASM_R4K_TIMER_H */
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
