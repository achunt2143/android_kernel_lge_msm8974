<<<<<<< HEAD
#ifndef _ASM_X86_SPARSEMEM_H
#define _ASM_X86_SPARSEMEM_H

=======
/* SPDX-License-Identifier: GPL-2.0 */
#ifndef _ASM_X86_SPARSEMEM_H
#define _ASM_X86_SPARSEMEM_H

#include <linux/types.h>

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#ifdef CONFIG_SPARSEMEM
/*
 * generic non-linear memory support:
 *
 * 1) we will not split memory into more chunks than will fit into the flags
 *    field of the struct page
 *
 * SECTION_SIZE_BITS		2^n: size of each section
<<<<<<< HEAD
 * MAX_PHYSADDR_BITS		2^n: max size of physical address space
 * MAX_PHYSMEM_BITS		2^n: how much memory we can have in that space
=======
 * MAX_PHYSMEM_BITS		2^n: max size of physical address space
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 *
 */

#ifdef CONFIG_X86_32
# ifdef CONFIG_X86_PAE
#  define SECTION_SIZE_BITS	29
<<<<<<< HEAD
#  define MAX_PHYSADDR_BITS	36
#  define MAX_PHYSMEM_BITS	36
# else
#  define SECTION_SIZE_BITS	26
#  define MAX_PHYSADDR_BITS	32
=======
#  define MAX_PHYSMEM_BITS	36
# else
#  define SECTION_SIZE_BITS	26
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#  define MAX_PHYSMEM_BITS	32
# endif
#else /* CONFIG_X86_32 */
# define SECTION_SIZE_BITS	27 /* matt - 128 is convenient right now */
<<<<<<< HEAD
# define MAX_PHYSADDR_BITS	44
# define MAX_PHYSMEM_BITS	46
#endif

#endif /* CONFIG_SPARSEMEM */
=======
# define MAX_PHYSMEM_BITS	(pgtable_l5_enabled() ? 52 : 46)
#endif

#endif /* CONFIG_SPARSEMEM */

#ifndef __ASSEMBLY__
#ifdef CONFIG_NUMA_KEEP_MEMINFO
extern int phys_to_target_node(phys_addr_t start);
#define phys_to_target_node phys_to_target_node
extern int memory_add_physaddr_to_nid(u64 start);
#define memory_add_physaddr_to_nid memory_add_physaddr_to_nid
extern int numa_fill_memblks(u64 start, u64 end);
#define numa_fill_memblks numa_fill_memblks
#endif
#endif /* __ASSEMBLY__ */

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#endif /* _ASM_X86_SPARSEMEM_H */
