<<<<<<< HEAD
=======
/* SPDX-License-Identifier: GPL-2.0 */
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#ifndef _ASM_SPARC64_HUGETLB_H
#define _ASM_SPARC64_HUGETLB_H

#include <asm/page.h>

<<<<<<< HEAD

void set_huge_pte_at(struct mm_struct *mm, unsigned long addr,
		     pte_t *ptep, pte_t pte);

pte_t huge_ptep_get_and_clear(struct mm_struct *mm, unsigned long addr,
			      pte_t *ptep);

void hugetlb_prefault_arch_hook(struct mm_struct *mm);

static inline int is_hugepage_only_range(struct mm_struct *mm,
					 unsigned long addr,
					 unsigned long len) {
	return 0;
}

/*
 * If the arch doesn't supply something else, assume that hugepage
 * size aligned regions are ok without further preparation.
 */
static inline int prepare_hugepage_range(struct file *file,
			unsigned long addr, unsigned long len)
{
	if (len & ~HPAGE_MASK)
		return -EINVAL;
	if (addr & ~HPAGE_MASK)
		return -EINVAL;
	return 0;
}

static inline void hugetlb_free_pgd_range(struct mmu_gather *tlb,
					  unsigned long addr, unsigned long end,
					  unsigned long floor,
					  unsigned long ceiling)
{
	free_pgd_range(tlb, addr, end, floor, ceiling);
}

static inline void huge_ptep_clear_flush(struct vm_area_struct *vma,
					 unsigned long addr, pte_t *ptep)
{
}

static inline int huge_pte_none(pte_t pte)
{
	return pte_none(pte);
}

static inline pte_t huge_pte_wrprotect(pte_t pte)
{
	return pte_wrprotect(pte);
}

=======
#ifdef CONFIG_HUGETLB_PAGE
struct pud_huge_patch_entry {
	unsigned int addr;
	unsigned int insn;
};
extern struct pud_huge_patch_entry __pud_huge_patch, __pud_huge_patch_end;
#endif

#define __HAVE_ARCH_HUGE_SET_HUGE_PTE_AT
void set_huge_pte_at(struct mm_struct *mm, unsigned long addr,
		     pte_t *ptep, pte_t pte, unsigned long sz);
void __set_huge_pte_at(struct mm_struct *mm, unsigned long addr,
		     pte_t *ptep, pte_t pte);

#define __HAVE_ARCH_HUGE_PTEP_GET_AND_CLEAR
pte_t huge_ptep_get_and_clear(struct mm_struct *mm, unsigned long addr,
			      pte_t *ptep);

#define __HAVE_ARCH_HUGE_PTEP_CLEAR_FLUSH
static inline pte_t huge_ptep_clear_flush(struct vm_area_struct *vma,
					  unsigned long addr, pte_t *ptep)
{
	return *ptep;
}

#define __HAVE_ARCH_HUGE_PTEP_SET_WRPROTECT
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
static inline void huge_ptep_set_wrprotect(struct mm_struct *mm,
					   unsigned long addr, pte_t *ptep)
{
	pte_t old_pte = *ptep;
<<<<<<< HEAD
	set_huge_pte_at(mm, addr, ptep, pte_wrprotect(old_pte));
}

=======
	__set_huge_pte_at(mm, addr, ptep, pte_wrprotect(old_pte));
}

#define __HAVE_ARCH_HUGE_PTEP_SET_ACCESS_FLAGS
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
static inline int huge_ptep_set_access_flags(struct vm_area_struct *vma,
					     unsigned long addr, pte_t *ptep,
					     pte_t pte, int dirty)
{
	int changed = !pte_same(*ptep, pte);
	if (changed) {
<<<<<<< HEAD
		set_huge_pte_at(vma->vm_mm, addr, ptep, pte);
=======
		__set_huge_pte_at(vma->vm_mm, addr, ptep, pte);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		flush_tlb_page(vma, addr);
	}
	return changed;
}

<<<<<<< HEAD
static inline pte_t huge_ptep_get(pte_t *ptep)
{
	return *ptep;
}

static inline int arch_prepare_hugepage(struct page *page)
{
	return 0;
}

static inline void arch_release_hugepage(struct page *page)
{
}
=======
#define __HAVE_ARCH_HUGETLB_FREE_PGD_RANGE
void hugetlb_free_pgd_range(struct mmu_gather *tlb, unsigned long addr,
			    unsigned long end, unsigned long floor,
			    unsigned long ceiling);

#include <asm-generic/hugetlb.h>
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

#endif /* _ASM_SPARC64_HUGETLB_H */
