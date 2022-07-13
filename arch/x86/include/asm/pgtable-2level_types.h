<<<<<<< HEAD
=======
/* SPDX-License-Identifier: GPL-2.0 */
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#ifndef _ASM_X86_PGTABLE_2LEVEL_DEFS_H
#define _ASM_X86_PGTABLE_2LEVEL_DEFS_H

#ifndef __ASSEMBLY__
#include <linux/types.h>

typedef unsigned long	pteval_t;
typedef unsigned long	pmdval_t;
typedef unsigned long	pudval_t;
<<<<<<< HEAD
=======
typedef unsigned long	p4dval_t;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
typedef unsigned long	pgdval_t;
typedef unsigned long	pgprotval_t;

typedef union {
	pteval_t pte;
	pteval_t pte_low;
} pte_t;
#endif	/* !__ASSEMBLY__ */

#define SHARED_KERNEL_PMD	0
<<<<<<< HEAD
#define PAGETABLE_LEVELS	2
=======

#define ARCH_PAGE_TABLE_SYNC_MASK	PGTBL_PMD_MODIFIED
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

/*
 * traditional i386 two-level paging structure:
 */

#define PGDIR_SHIFT	22
#define PTRS_PER_PGD	1024


/*
 * the i386 is two-level, so we don't really have any
 * PMD directory physically.
 */

#define PTRS_PER_PTE	1024

<<<<<<< HEAD
=======
/* This covers all VMSPLIT_* and VMSPLIT_*_OPT variants */
#define PGD_KERNEL_START	(CONFIG_PAGE_OFFSET >> PGDIR_SHIFT)

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#endif /* _ASM_X86_PGTABLE_2LEVEL_DEFS_H */
