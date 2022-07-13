<<<<<<< HEAD
=======
/* SPDX-License-Identifier: GPL-2.0 */
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
/*
 * Various ISA level dependent constants.
 * Most of the following constants reflect the different layout
 * of Coprocessor 0 registers.
 *
 * Copyright (c) 1998 Harald Koerfgen
 */

#ifndef __ASM_ISADEP_H
#define __ASM_ISADEP_H

<<<<<<< HEAD
#if defined(CONFIG_CPU_R3000) || defined(CONFIG_CPU_TX39XX)
=======
#if defined(CONFIG_CPU_R3000)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
/*
 * R2000 or R3000
 */

/*
 * kernel or user mode? (CP0_STATUS)
 */
#define KU_MASK 0x08
<<<<<<< HEAD
#define	KU_USER 0x08
=======
#define KU_USER 0x08
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#define KU_KERN 0x00

#else
/*
 * kernel or user mode?
 */
#define KU_MASK 0x18
<<<<<<< HEAD
#define	KU_USER 0x10
=======
#define KU_USER 0x10
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#define KU_KERN 0x00

#endif

#endif /* __ASM_ISADEP_H */
