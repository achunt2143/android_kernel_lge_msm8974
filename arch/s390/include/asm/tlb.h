<<<<<<< HEAD
=======
/* SPDX-License-Identifier: GPL-2.0 */
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#ifndef _S390_TLB_H
#define _S390_TLB_H

/*
 * TLB flushing on s390 is complicated. The following requirement
 * from the principles of operation is the most arduous:
 *
 * "A valid table entry must not be changed while it is attached
 * to any CPU and may be used for translation by that CPU except to
 * (1) invalidate the entry by using INVALIDATE PAGE TABLE ENTRY,
 * or INVALIDATE DAT TABLE ENTRY, (2) alter bits 56-63 of a page
 * table entry, or (3) make a change by means of a COMPARE AND SWAP
 * AND PURGE instruction that purges the TLB."
 *
 * The modification of a pte of an active mm struct therefore is
 * a two step process: i) invalidate the pte, ii) store the new pte.
 * This is true for the page protection bit as well.
 * The only possible optimization is to flush at the beginning of
 * a tlb_gather_mmu cycle if the mm_struct is currently not in use.
 *
 * Pages used for the page tables is a different story. FIXME: more
 */

<<<<<<< HEAD
#include <linux/mm.h>
#include <linux/pagemap.h>
#include <linux/swap.h>
#include <asm/processor.h>
#include <asm/pgalloc.h>
#include <asm/tlbflush.h>

struct mmu_gather {
	struct mm_struct *mm;
	struct mmu_table_batch *batch;
	unsigned int fullmm;
};

struct mmu_table_batch {
	struct rcu_head		rcu;
	unsigned int		nr;
	void			*tables[0];
};

#define MAX_TABLE_BATCH		\
	((PAGE_SIZE - sizeof(struct mmu_table_batch)) / sizeof(void *))

extern void tlb_table_flush(struct mmu_gather *tlb);
extern void tlb_remove_table(struct mmu_gather *tlb, void *table);

static inline void tlb_gather_mmu(struct mmu_gather *tlb,
				  struct mm_struct *mm,
				  unsigned int full_mm_flush)
{
	tlb->mm = mm;
	tlb->fullmm = full_mm_flush;
	tlb->batch = NULL;
	if (tlb->fullmm)
		__tlb_flush_mm(mm);
}

static inline void tlb_flush_mmu(struct mmu_gather *tlb)
{
	tlb_table_flush(tlb);
}

static inline void tlb_finish_mmu(struct mmu_gather *tlb,
				  unsigned long start, unsigned long end)
{
	tlb_table_flush(tlb);
}
=======
void __tlb_remove_table(void *_table);
static inline void tlb_flush(struct mmu_gather *tlb);
static inline bool __tlb_remove_page_size(struct mmu_gather *tlb,
		struct page *page, bool delay_rmap, int page_size);
static inline bool __tlb_remove_folio_pages(struct mmu_gather *tlb,
		struct page *page, unsigned int nr_pages, bool delay_rmap);

#define tlb_flush tlb_flush
#define pte_free_tlb pte_free_tlb
#define pmd_free_tlb pmd_free_tlb
#define p4d_free_tlb p4d_free_tlb
#define pud_free_tlb pud_free_tlb

#include <asm/tlbflush.h>
#include <asm-generic/tlb.h>
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

/*
 * Release the page cache reference for a pte removed by
 * tlb_ptep_clear_flush. In both flush modes the tlb for a page cache page
 * has already been freed, so just do free_page_and_swap_cache.
<<<<<<< HEAD
 */
static inline int __tlb_remove_page(struct mmu_gather *tlb, struct page *page)
{
	free_page_and_swap_cache(page);
	return 1; /* avoid calling tlb_flush_mmu */
}

static inline void tlb_remove_page(struct mmu_gather *tlb, struct page *page)
{
	free_page_and_swap_cache(page);
=======
 *
 * s390 doesn't delay rmap removal.
 */
static inline bool __tlb_remove_page_size(struct mmu_gather *tlb,
		struct page *page, bool delay_rmap, int page_size)
{
	VM_WARN_ON_ONCE(delay_rmap);

	free_page_and_swap_cache(page);
	return false;
}

static inline bool __tlb_remove_folio_pages(struct mmu_gather *tlb,
		struct page *page, unsigned int nr_pages, bool delay_rmap)
{
	struct encoded_page *encoded_pages[] = {
		encode_page(page, ENCODED_PAGE_BIT_NR_PAGES_NEXT),
		encode_nr_pages(nr_pages),
	};

	VM_WARN_ON_ONCE(delay_rmap);
	VM_WARN_ON_ONCE(page_folio(page) != page_folio(page + nr_pages - 1));

	free_pages_and_swap_cache(encoded_pages, ARRAY_SIZE(encoded_pages));
	return false;
}

static inline void tlb_flush(struct mmu_gather *tlb)
{
	__tlb_flush_mm_lazy(tlb->mm);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

/*
 * pte_free_tlb frees a pte table and clears the CRSTE for the
 * page table from the tlb.
 */
static inline void pte_free_tlb(struct mmu_gather *tlb, pgtable_t pte,
<<<<<<< HEAD
				unsigned long address)
{
	if (!tlb->fullmm)
		return page_table_free_rcu(tlb, (unsigned long *) pte);
	page_table_free(tlb->mm, (unsigned long *) pte);
=======
                                unsigned long address)
{
	__tlb_adjust_range(tlb, address, PAGE_SIZE);
	tlb->mm->context.flush_mm = 1;
	tlb->freed_tables = 1;
	tlb->cleared_pmds = 1;
	if (mm_alloc_pgste(tlb->mm))
		gmap_unlink(tlb->mm, (unsigned long *)pte, address);
	tlb_remove_ptdesc(tlb, pte);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

/*
 * pmd_free_tlb frees a pmd table and clears the CRSTE for the
 * segment table entry from the tlb.
 * If the mm uses a two level page table the single pmd is freed
 * as the pgd. pmd_free_tlb checks the asce_limit against 2GB
 * to avoid the double free of the pmd in this case.
 */
static inline void pmd_free_tlb(struct mmu_gather *tlb, pmd_t *pmd,
				unsigned long address)
{
<<<<<<< HEAD
#ifdef __s390x__
	if (tlb->mm->context.asce_limit <= (1UL << 31))
		return;
	if (!tlb->fullmm)
		return tlb_remove_table(tlb, pmd);
	crst_table_free(tlb->mm, (unsigned long *) pmd);
#endif
=======
	if (mm_pmd_folded(tlb->mm))
		return;
	pagetable_pmd_dtor(virt_to_ptdesc(pmd));
	__tlb_adjust_range(tlb, address, PAGE_SIZE);
	tlb->mm->context.flush_mm = 1;
	tlb->freed_tables = 1;
	tlb->cleared_puds = 1;
	tlb_remove_ptdesc(tlb, pmd);
}

/*
 * p4d_free_tlb frees a pud table and clears the CRSTE for the
 * region second table entry from the tlb.
 * If the mm uses a four level page table the single p4d is freed
 * as the pgd. p4d_free_tlb checks the asce_limit against 8PB
 * to avoid the double free of the p4d in this case.
 */
static inline void p4d_free_tlb(struct mmu_gather *tlb, p4d_t *p4d,
				unsigned long address)
{
	if (mm_p4d_folded(tlb->mm))
		return;
	__tlb_adjust_range(tlb, address, PAGE_SIZE);
	tlb->mm->context.flush_mm = 1;
	tlb->freed_tables = 1;
	tlb_remove_ptdesc(tlb, p4d);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

/*
 * pud_free_tlb frees a pud table and clears the CRSTE for the
 * region third table entry from the tlb.
 * If the mm uses a three level page table the single pud is freed
 * as the pgd. pud_free_tlb checks the asce_limit against 4TB
 * to avoid the double free of the pud in this case.
 */
static inline void pud_free_tlb(struct mmu_gather *tlb, pud_t *pud,
				unsigned long address)
{
<<<<<<< HEAD
#ifdef __s390x__
	if (tlb->mm->context.asce_limit <= (1UL << 42))
		return;
	if (!tlb->fullmm)
		return tlb_remove_table(tlb, pud);
	crst_table_free(tlb->mm, (unsigned long *) pud);
#endif
}

#define tlb_start_vma(tlb, vma)			do { } while (0)
#define tlb_end_vma(tlb, vma)			do { } while (0)
#define tlb_remove_tlb_entry(tlb, ptep, addr)	do { } while (0)
#define tlb_migrate_finish(mm)			do { } while (0)
=======
	if (mm_pud_folded(tlb->mm))
		return;
	tlb->mm->context.flush_mm = 1;
	tlb->freed_tables = 1;
	tlb->cleared_p4ds = 1;
	tlb_remove_ptdesc(tlb, pud);
}

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

#endif /* _S390_TLB_H */
