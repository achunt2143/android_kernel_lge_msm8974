<<<<<<< HEAD
/*
 * include/asm-xtensa/pgalloc.h
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 *
=======
/* SPDX-License-Identifier: GPL-2.0-only */
/*
 * include/asm-xtensa/pgalloc.h
 *
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 * Copyright (C) 2001-2007 Tensilica Inc.
 */

#ifndef _XTENSA_PGALLOC_H
#define _XTENSA_PGALLOC_H

<<<<<<< HEAD
#ifdef __KERNEL__

#include <linux/highmem.h>
#include <linux/slab.h>

=======
#ifdef CONFIG_MMU
#include <linux/highmem.h>
#include <linux/slab.h>

#define __HAVE_ARCH_PTE_ALLOC_ONE_KERNEL
#define __HAVE_ARCH_PTE_ALLOC_ONE
#include <asm-generic/pgalloc.h>

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
/*
 * Allocating and freeing a pmd is trivial: the 1-entry pmd is
 * inside the pgd, so has no extra memory associated with it.
 */

#define pmd_populate_kernel(mm, pmdp, ptep)				     \
	(pmd_val(*(pmdp)) = ((unsigned long)ptep))
#define pmd_populate(mm, pmdp, page)					     \
	(pmd_val(*(pmdp)) = ((unsigned long)page_to_virt(page)))
<<<<<<< HEAD
#define pmd_pgtable(pmd) pmd_page(pmd)
=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

static inline pgd_t*
pgd_alloc(struct mm_struct *mm)
{
<<<<<<< HEAD
	return (pgd_t*) __get_free_pages(GFP_KERNEL | __GFP_ZERO, PGD_ORDER);
}

static inline void pgd_free(struct mm_struct *mm, pgd_t *pgd)
{
	free_page((unsigned long)pgd);
}

/* Use a slab cache for the pte pages (see also sparc64 implementation) */

extern struct kmem_cache *pgtable_cache;

static inline pte_t *pte_alloc_one_kernel(struct mm_struct *mm, 
					 unsigned long address)
{
	return kmem_cache_alloc(pgtable_cache, GFP_KERNEL|__GFP_REPEAT);
}

static inline pgtable_t pte_alloc_one(struct mm_struct *mm,
					unsigned long addr)
{
	struct page *page;

	page = virt_to_page(pte_alloc_one_kernel(mm, addr));
	pgtable_page_ctor(page);
	return page;
}

static inline void pte_free_kernel(struct mm_struct *mm, pte_t *pte)
{
	kmem_cache_free(pgtable_cache, pte);
}

static inline void pte_free(struct mm_struct *mm, pgtable_t pte)
{
	pgtable_page_dtor(pte);
	kmem_cache_free(pgtable_cache, page_address(pte));
}
#define pmd_pgtable(pmd) pmd_page(pmd)

#endif /* __KERNEL__ */
=======
	return (pgd_t*) __get_free_page(GFP_KERNEL | __GFP_ZERO);
}

static inline void ptes_clear(pte_t *ptep)
{
	int i;

	for (i = 0; i < PTRS_PER_PTE; i++)
		pte_clear(NULL, 0, ptep + i);
}

static inline pte_t *pte_alloc_one_kernel(struct mm_struct *mm)
{
	pte_t *ptep;

	ptep = (pte_t *)__pte_alloc_one_kernel(mm);
	if (!ptep)
		return NULL;
	ptes_clear(ptep);
	return ptep;
}

static inline pgtable_t pte_alloc_one(struct mm_struct *mm)
{
	struct page *page;

	page = __pte_alloc_one(mm, GFP_PGTABLE_USER);
	if (!page)
		return NULL;
	ptes_clear(page_address(page));
	return page;
}

#endif /* CONFIG_MMU */

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#endif /* _XTENSA_PGALLOC_H */
