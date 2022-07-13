<<<<<<< HEAD
=======
/* SPDX-License-Identifier: GPL-2.0 */
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#ifndef _ASM_PGALLOC_H
#define _ASM_PGALLOC_H

#include <linux/gfp.h>
#include <linux/mm.h>
#include <linux/threads.h>
#include <asm/processor.h>
#include <asm/fixmap.h>

#include <asm/cache.h>

<<<<<<< HEAD
/* Allocate the top level pgd (page directory)
 *
 * Here (for 64 bit kernels) we implement a Hybrid L2/L3 scheme: we
 * allocate the first pmd adjacent to the pgd.  This means that we can
 * subtract a constant offset to get to it.  The pmd and pgd sizes are
 * arranged so that a single pmd covers 4GB (giving a full 64-bit
 * process access to 8TB) so our lookups are effectively L2 for the
 * first 4GB of the kernel (i.e. for all ILP32 processes and all the
 * kernel for machines with under 4GB of memory) */
static inline pgd_t *pgd_alloc(struct mm_struct *mm)
{
	pgd_t *pgd = (pgd_t *)__get_free_pages(GFP_KERNEL,
					       PGD_ALLOC_ORDER);
	pgd_t *actual_pgd = pgd;

	if (likely(pgd != NULL)) {
		memset(pgd, 0, PAGE_SIZE<<PGD_ALLOC_ORDER);
#ifdef CONFIG_64BIT
		actual_pgd += PTRS_PER_PGD;
		/* Populate first pmd with allocated memory.  We mark it
		 * with PxD_FLAG_ATTACHED as a signal to the system that this
		 * pmd entry may not be cleared. */
		__pgd_val_set(*actual_pgd, (PxD_FLAG_PRESENT | 
				        PxD_FLAG_VALID | 
					PxD_FLAG_ATTACHED) 
			+ (__u32)(__pa((unsigned long)pgd) >> PxD_VALUE_SHIFT));
		/* The first pmd entry also is marked with _PAGE_GATEWAY as
		 * a signal that this pmd may not be freed */
		__pgd_val_set(*pgd, PxD_FLAG_ATTACHED);
#endif
	}
	return actual_pgd;
=======
#define __HAVE_ARCH_PMD_ALLOC_ONE
#define __HAVE_ARCH_PMD_FREE
#define __HAVE_ARCH_PGD_FREE
#include <asm-generic/pgalloc.h>

/* Allocate the top level pgd (page directory) */
static inline pgd_t *pgd_alloc(struct mm_struct *mm)
{
	pgd_t *pgd;

	pgd = (pgd_t *) __get_free_pages(GFP_KERNEL, PGD_TABLE_ORDER);
	if (unlikely(pgd == NULL))
		return NULL;

	memset(pgd, 0, PAGE_SIZE << PGD_TABLE_ORDER);

	return pgd;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

static inline void pgd_free(struct mm_struct *mm, pgd_t *pgd)
{
<<<<<<< HEAD
#ifdef CONFIG_64BIT
	pgd -= PTRS_PER_PGD;
#endif
	free_pages((unsigned long)pgd, PGD_ALLOC_ORDER);
}

#if PT_NLEVELS == 3

/* Three Level Page Table Support for pmd's */

static inline void pgd_populate(struct mm_struct *mm, pgd_t *pgd, pmd_t *pmd)
{
	__pgd_val_set(*pgd, (PxD_FLAG_PRESENT | PxD_FLAG_VALID) +
		        (__u32)(__pa((unsigned long)pmd) >> PxD_VALUE_SHIFT));
=======
	free_pages((unsigned long)pgd, PGD_TABLE_ORDER);
}

#if CONFIG_PGTABLE_LEVELS == 3

/* Three Level Page Table Support for pmd's */

static inline void pud_populate(struct mm_struct *mm, pud_t *pud, pmd_t *pmd)
{
	set_pud(pud, __pud((PxD_FLAG_PRESENT | PxD_FLAG_VALID) +
			(__u32)(__pa((unsigned long)pmd) >> PxD_VALUE_SHIFT)));
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

static inline pmd_t *pmd_alloc_one(struct mm_struct *mm, unsigned long address)
{
<<<<<<< HEAD
	pmd_t *pmd = (pmd_t *)__get_free_pages(GFP_KERNEL|__GFP_REPEAT,
					       PMD_ORDER);
	if (pmd)
		memset(pmd, 0, PAGE_SIZE<<PMD_ORDER);
=======
	pmd_t *pmd;

	pmd = (pmd_t *)__get_free_pages(GFP_PGTABLE_KERNEL, PMD_TABLE_ORDER);
	if (likely(pmd))
		memset ((void *)pmd, 0, PAGE_SIZE << PMD_TABLE_ORDER);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	return pmd;
}

static inline void pmd_free(struct mm_struct *mm, pmd_t *pmd)
{
<<<<<<< HEAD
#ifdef CONFIG_64BIT
	if(pmd_flag(*pmd) & PxD_FLAG_ATTACHED)
		/* This is the permanent pmd attached to the pgd;
		 * cannot free it */
		return;
#endif
	free_pages((unsigned long)pmd, PMD_ORDER);
}

#else

/* Two Level Page Table Support for pmd's */

/*
 * allocating and freeing a pmd is trivial: the 1-entry pmd is
 * inside the pgd, so has no extra memory associated with it.
 */

#define pmd_alloc_one(mm, addr)		({ BUG(); ((pmd_t *)2); })
#define pmd_free(mm, x)			do { } while (0)
#define pgd_populate(mm, pmd, pte)	BUG()

=======
	free_pages((unsigned long)pmd, PMD_TABLE_ORDER);
}
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#endif

static inline void
pmd_populate_kernel(struct mm_struct *mm, pmd_t *pmd, pte_t *pte)
{
<<<<<<< HEAD
#ifdef CONFIG_64BIT
	/* preserve the gateway marker if this is the beginning of
	 * the permanent pmd */
	if(pmd_flag(*pmd) & PxD_FLAG_ATTACHED)
		__pmd_val_set(*pmd, (PxD_FLAG_PRESENT |
				 PxD_FLAG_VALID |
				 PxD_FLAG_ATTACHED) 
			+ (__u32)(__pa((unsigned long)pte) >> PxD_VALUE_SHIFT));
	else
#endif
		__pmd_val_set(*pmd, (PxD_FLAG_PRESENT | PxD_FLAG_VALID) 
			+ (__u32)(__pa((unsigned long)pte) >> PxD_VALUE_SHIFT));
=======
	set_pmd(pmd, __pmd((PxD_FLAG_PRESENT | PxD_FLAG_VALID)
		+ (__u32)(__pa((unsigned long)pte) >> PxD_VALUE_SHIFT)));
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

#define pmd_populate(mm, pmd, pte_page) \
	pmd_populate_kernel(mm, pmd, page_address(pte_page))
<<<<<<< HEAD
#define pmd_pgtable(pmd) pmd_page(pmd)

static inline pgtable_t
pte_alloc_one(struct mm_struct *mm, unsigned long address)
{
	struct page *page = alloc_page(GFP_KERNEL|__GFP_REPEAT|__GFP_ZERO);
	if (page)
		pgtable_page_ctor(page);
	return page;
}

static inline pte_t *
pte_alloc_one_kernel(struct mm_struct *mm, unsigned long addr)
{
	pte_t *pte = (pte_t *)__get_free_page(GFP_KERNEL|__GFP_REPEAT|__GFP_ZERO);
	return pte;
}

static inline void pte_free_kernel(struct mm_struct *mm, pte_t *pte)
{
	free_page((unsigned long)pte);
}

static inline void pte_free(struct mm_struct *mm, struct page *pte)
{
	pgtable_page_dtor(pte);
	pte_free_kernel(mm, page_address(pte));
}

#define check_pgt_cache()	do { } while (0)
=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

#endif
