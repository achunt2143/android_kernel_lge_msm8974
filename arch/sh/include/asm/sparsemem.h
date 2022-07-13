<<<<<<< HEAD
#ifndef __ASM_SH_SPARSEMEM_H
#define __ASM_SH_SPARSEMEM_H

#ifdef __KERNEL__
/*
 * SECTION_SIZE_BITS		2^N: how big each section will be
 * MAX_PHYSADDR_BITS		2^N: how much physical address space we have
 * MAX_PHYSMEM_BITS		2^N: how much memory we can have in that space
 */
#define SECTION_SIZE_BITS	26
#define MAX_PHYSADDR_BITS	32
#define MAX_PHYSMEM_BITS	32

#endif

=======
/* SPDX-License-Identifier: GPL-2.0 */
#ifndef __ASM_SH_SPARSEMEM_H
#define __ASM_SH_SPARSEMEM_H

/*
 * SECTION_SIZE_BITS		2^N: how big each section will be
 * MAX_PHYSMEM_BITS		2^N: how much physical address space we have
 */
#define SECTION_SIZE_BITS	26
#define MAX_PHYSMEM_BITS	32

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#endif /* __ASM_SH_SPARSEMEM_H */
