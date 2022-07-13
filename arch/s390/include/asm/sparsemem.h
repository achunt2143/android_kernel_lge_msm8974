<<<<<<< HEAD
#ifndef _ASM_S390_SPARSEMEM_H
#define _ASM_S390_SPARSEMEM_H

#ifdef CONFIG_64BIT

#define SECTION_SIZE_BITS	28
#define MAX_PHYSADDR_BITS	46
#define MAX_PHYSMEM_BITS	46

#else

#define SECTION_SIZE_BITS	25
#define MAX_PHYSADDR_BITS	31
#define MAX_PHYSMEM_BITS	31

#endif /* CONFIG_64BIT */
=======
/* SPDX-License-Identifier: GPL-2.0 */
#ifndef _ASM_S390_SPARSEMEM_H
#define _ASM_S390_SPARSEMEM_H

#define SECTION_SIZE_BITS	28
#define MAX_PHYSMEM_BITS	CONFIG_MAX_PHYSMEM_BITS
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

#endif /* _ASM_S390_SPARSEMEM_H */
