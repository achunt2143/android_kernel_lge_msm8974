<<<<<<< HEAD
#ifndef _ASM_POWERPC_PGALLOC_H
#define _ASM_POWERPC_PGALLOC_H
#ifdef __KERNEL__

#include <linux/mm.h>

#ifdef CONFIG_PPC_BOOK3E
extern void tlb_flush_pgtable(struct mmu_gather *tlb, unsigned long address);
#else /* CONFIG_PPC_BOOK3E */
static inline void tlb_flush_pgtable(struct mmu_gather *tlb,
				     unsigned long address)
{
}
#endif /* !CONFIG_PPC_BOOK3E */

static inline void pte_free_kernel(struct mm_struct *mm, pte_t *pte)
{
	free_page((unsigned long)pte);
=======
/* SPDX-License-Identifier: GPL-2.0 */
#ifndef _ASM_POWERPC_PGALLOC_H
#define _ASM_POWERPC_PGALLOC_H

#include <linux/mm.h>

#ifndef MODULE
static inline gfp_t pgtable_gfp_flags(struct mm_struct *mm, gfp_t gfp)
{
	if (unlikely(mm == &init_mm))
		return gfp;
	return gfp | __GFP_ACCOUNT;
}
#else /* !MODULE */
static inline gfp_t pgtable_gfp_flags(struct mm_struct *mm, gfp_t gfp)
{
	return gfp | __GFP_ACCOUNT;
}
#endif /* MODULE */

#define PGALLOC_GFP (GFP_KERNEL | __GFP_ZERO)

pte_t *pte_fragment_alloc(struct mm_struct *mm, int kernel);

static inline pte_t *pte_alloc_one_kernel(struct mm_struct *mm)
{
	return (pte_t *)pte_fragment_alloc(mm, 1);
}

static inline pgtable_t pte_alloc_one(struct mm_struct *mm)
{
	return (pgtable_t)pte_fragment_alloc(mm, 0);
}

void pte_frag_destroy(void *pte_frag);
void pte_fragment_free(unsigned long *table, int kernel);

static inline void pte_free_kernel(struct mm_struct *mm, pte_t *pte)
{
	pte_fragment_free((unsigned long *)pte, 1);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

static inline void pte_free(struct mm_struct *mm, pgtable_t ptepage)
{
<<<<<<< HEAD
	pgtable_page_dtor(ptepage);
	__free_page(ptepage);
}

#ifdef CONFIG_PPC64
#include <asm/pgalloc-64.h>
#else
#include <asm/pgalloc-32.h>
#endif

#ifdef CONFIG_SMP
struct mmu_gather;
extern void tlb_remove_table(struct mmu_gather *, void *);

static inline void pgtable_free_tlb(struct mmu_gather *tlb, void *table, int shift)
{
	unsigned long pgf = (unsigned long)table;
	BUG_ON(shift > MAX_PGTABLE_INDEX_SIZE);
	pgf |= shift;
	tlb_remove_table(tlb, (void *)pgf);
}

static inline void __tlb_remove_table(void *_table)
{
	void *table = (void *)((unsigned long)_table & ~MAX_PGTABLE_INDEX_SIZE);
	unsigned shift = (unsigned long)_table & MAX_PGTABLE_INDEX_SIZE;

	pgtable_free(table, shift);
}
#else /* CONFIG_SMP */
static inline void pgtable_free_tlb(struct mmu_gather *tlb, void *table, unsigned shift)
{
	pgtable_free(table, shift);
}
#endif /* !CONFIG_SMP */

static inline void __pte_free_tlb(struct mmu_gather *tlb, struct page *ptepage,
				  unsigned long address)
{
	tlb_flush_pgtable(tlb, address);
	pgtable_page_dtor(ptepage);
	pgtable_free_tlb(tlb, page_address(ptepage), 0);
}

#endif /* __KERNEL__ */
=======
	pte_fragment_free((unsigned long *)ptepage, 0);
}

/* arch use pte_free_defer() implementation in arch/powerpc/mm/pgtable-frag.c */
#define pte_free_defer pte_free_defer
void pte_free_defer(struct mm_struct *mm, pgtable_t pgtable);

/*
 * Functions that deal with pagetables that could be at any level of
 * the table need to be passed an "index_size" so they know how to
 * handle allocation.  For PTE pages, the allocation size will be
 * (2^index_size * sizeof(pointer)) and allocations are drawn from
 * the kmem_cache in PGT_CACHE(index_size).
 *
 * The maximum index size needs to be big enough to allow any
 * pagetable sizes we need, but small enough to fit in the low bits of
 * any page table pointer.  In other words all pagetables, even tiny
 * ones, must be aligned to allow at least enough low 0 bits to
 * contain this value.  This value is also used as a mask, so it must
 * be one less than a power of two.
 */
#define MAX_PGTABLE_INDEX_SIZE	0xf

extern struct kmem_cache *pgtable_cache[];
#define PGT_CACHE(shift) pgtable_cache[shift]

#ifdef CONFIG_PPC_BOOK3S
#include <asm/book3s/pgalloc.h>
#else
#include <asm/nohash/pgalloc.h>
#endif

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#endif /* _ASM_POWERPC_PGALLOC_H */
