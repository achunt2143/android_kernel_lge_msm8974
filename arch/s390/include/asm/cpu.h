<<<<<<< HEAD
/*
 *    Copyright IBM Corp. 2000,2009
=======
/* SPDX-License-Identifier: GPL-2.0 */
/*
 *    Copyright IBM Corp. 2000, 2009
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 *    Author(s): Hartmut Penner <hp@de.ibm.com>,
 *		 Martin Schwidefsky <schwidefsky@de.ibm.com>,
 *		 Christian Ehrhardt <ehrhardt@de.ibm.com>,
 */

#ifndef _ASM_S390_CPU_H
#define _ASM_S390_CPU_H

<<<<<<< HEAD
#define MAX_CPU_ADDRESS 255

#ifndef __ASSEMBLY__

#include <linux/types.h>
=======
#ifndef __ASSEMBLY__

#include <linux/types.h>
#include <linux/jump_label.h>
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

struct cpuid
{
	unsigned int version :	8;
	unsigned int ident   : 24;
	unsigned int machine : 16;
	unsigned int unused  : 16;
} __attribute__ ((packed, aligned(8)));

<<<<<<< HEAD
=======
DECLARE_STATIC_KEY_FALSE(cpu_has_bear);

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#endif /* __ASSEMBLY__ */
#endif /* _ASM_S390_CPU_H */
