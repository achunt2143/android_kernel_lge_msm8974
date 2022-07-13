<<<<<<< HEAD
#ifndef _ASM_POWERPC_PGTABLE_H
#define _ASM_POWERPC_PGTABLE_H
#ifdef __KERNEL__

#ifndef __ASSEMBLY__
#include <asm/processor.h>		/* For TASK_SIZE */
#include <asm/mmu.h>
#include <asm/page.h>

struct mm_struct;

#ifdef CONFIG_DEBUG_VM
extern void assert_pte_locked(struct mm_struct *mm, unsigned long addr);
#else /* CONFIG_DEBUG_VM */
static inline void assert_pte_locked(struct mm_struct *mm, unsigned long addr)
{
}
#endif /* !CONFIG_DEBUG_VM */

#endif /* !__ASSEMBLY__ */

#if defined(CONFIG_PPC64)
#  include <asm/pgtable-ppc64.h>
#else
#  include <asm/pgtable-ppc32.h>
#endif

#ifndef __ASSEMBLY__

/* Generic accessors to PTE bits */
static inline int pte_write(pte_t pte)		{ return pte_val(pte) & _PAGE_RW; }
static inline int pte_dirty(pte_t pte)		{ return pte_val(pte) & _PAGE_DIRTY; }
static inline int pte_young(pte_t pte)		{ return pte_val(pte) & _PAGE_ACCESSED; }
static inline int pte_file(pte_t pte)		{ return pte_val(pte) & _PAGE_FILE; }
static inline int pte_special(pte_t pte)	{ return pte_val(pte) & _PAGE_SPECIAL; }
static inline int pte_present(pte_t pte)	{ return pte_val(pte) & _PAGE_PRESENT; }
static inline int pte_none(pte_t pte)		{ return (pte_val(pte) & ~_PTE_NONE_MASK) == 0; }
static inline pgprot_t pte_pgprot(pte_t pte)	{ return __pgprot(pte_val(pte) & PAGE_PROT_BITS); }

/* Conversion functions: convert a page and protection to a page entry,
 * and a page entry and page directory to the page they refer to.
 *
 * Even if PTEs can be unsigned long long, a PFN is always an unsigned
 * long for now.
 */
static inline pte_t pfn_pte(unsigned long pfn, pgprot_t pgprot) {
	return __pte(((pte_basic_t)(pfn) << PTE_RPN_SHIFT) |
		     pgprot_val(pgprot)); }
static inline unsigned long pte_pfn(pte_t pte)	{
	return pte_val(pte) >> PTE_RPN_SHIFT; }
=======
/* SPDX-License-Identifier: GPL-2.0 */
#ifndef _ASM_POWERPC_PGTABLE_H
#define _ASM_POWERPC_PGTABLE_H

#ifndef __ASSEMBLY__
#include <linux/mmdebug.h>
#include <linux/mmzone.h>
#include <asm/processor.h>		/* For TASK_SIZE */
#include <asm/mmu.h>
#include <asm/page.h>
#include <asm/tlbflush.h>

struct mm_struct;

#endif /* !__ASSEMBLY__ */

#ifdef CONFIG_PPC_BOOK3S
#include <asm/book3s/pgtable.h>
#else
#include <asm/nohash/pgtable.h>
#endif /* !CONFIG_PPC_BOOK3S */

/*
 * Protection used for kernel text. We want the debuggers to be able to
 * set breakpoints anywhere, so don't write protect the kernel text
 * on platforms where such control is possible.
 */
#if defined(CONFIG_KGDB) || defined(CONFIG_XMON) || defined(CONFIG_BDI_SWITCH) || \
	defined(CONFIG_KPROBES) || defined(CONFIG_DYNAMIC_FTRACE)
#define PAGE_KERNEL_TEXT	PAGE_KERNEL_X
#else
#define PAGE_KERNEL_TEXT	PAGE_KERNEL_ROX
#endif

/* Make modules code happy. We don't set RO yet */
#define PAGE_KERNEL_EXEC	PAGE_KERNEL_X

/* Advertise special mapping type for AGP */
#define PAGE_AGP		(PAGE_KERNEL_NC)
#define HAVE_PAGE_AGP

#ifndef __ASSEMBLY__

#define PFN_PTE_SHIFT		PTE_RPN_SHIFT

void set_ptes(struct mm_struct *mm, unsigned long addr, pte_t *ptep,
		pte_t pte, unsigned int nr);
#define set_ptes set_ptes
#define update_mmu_cache(vma, addr, ptep) \
	update_mmu_cache_range(NULL, vma, addr, ptep, 1)

#ifndef MAX_PTRS_PER_PGD
#define MAX_PTRS_PER_PGD PTRS_PER_PGD
#endif
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

/* Keep these as a macros to avoid include dependency mess */
#define pte_page(x)		pfn_to_page(pte_pfn(x))
#define mk_pte(page, pgprot)	pfn_pte(page_to_pfn(page), (pgprot))

<<<<<<< HEAD
/* Generic modifiers for PTE bits */
static inline pte_t pte_wrprotect(pte_t pte) {
	pte_val(pte) &= ~(_PAGE_RW | _PAGE_HWWRITE); return pte; }
static inline pte_t pte_mkclean(pte_t pte) {
	pte_val(pte) &= ~(_PAGE_DIRTY | _PAGE_HWWRITE); return pte; }
static inline pte_t pte_mkold(pte_t pte) {
	pte_val(pte) &= ~_PAGE_ACCESSED; return pte; }
static inline pte_t pte_mkwrite(pte_t pte) {
	pte_val(pte) |= _PAGE_RW; return pte; }
static inline pte_t pte_mkdirty(pte_t pte) {
	pte_val(pte) |= _PAGE_DIRTY; return pte; }
static inline pte_t pte_mkyoung(pte_t pte) {
	pte_val(pte) |= _PAGE_ACCESSED; return pte; }
static inline pte_t pte_mkspecial(pte_t pte) {
	pte_val(pte) |= _PAGE_SPECIAL; return pte; }
static inline pte_t pte_mkhuge(pte_t pte) {
	return pte; }
static inline pte_t pte_modify(pte_t pte, pgprot_t newprot)
{
	pte_val(pte) = (pte_val(pte) & _PAGE_CHG_MASK) | pgprot_val(newprot);
	return pte;
}


/* Insert a PTE, top-level function is out of line. It uses an inline
 * low level function in the respective pgtable-* files
 */
extern void set_pte_at(struct mm_struct *mm, unsigned long addr, pte_t *ptep,
		       pte_t pte);

/* This low level function performs the actual PTE insertion
 * Setting the PTE depends on the MMU type and other factors. It's
 * an horrible mess that I'm not going to try to clean up now but
 * I'm keeping it in one place rather than spread around
 */
static inline void __set_pte_at(struct mm_struct *mm, unsigned long addr,
				pte_t *ptep, pte_t pte, int percpu)
{
#if defined(CONFIG_PPC_STD_MMU_32) && defined(CONFIG_SMP) && !defined(CONFIG_PTE_64BIT)
	/* First case is 32-bit Hash MMU in SMP mode with 32-bit PTEs. We use the
	 * helper pte_update() which does an atomic update. We need to do that
	 * because a concurrent invalidation can clear _PAGE_HASHPTE. If it's a
	 * per-CPU PTE such as a kmap_atomic, we do a simple update preserving
	 * the hash bits instead (ie, same as the non-SMP case)
	 */
	if (percpu)
		*ptep = __pte((pte_val(*ptep) & _PAGE_HASHPTE)
			      | (pte_val(pte) & ~_PAGE_HASHPTE));
	else
		pte_update(ptep, ~_PAGE_HASHPTE, pte_val(pte));

#elif defined(CONFIG_PPC32) && defined(CONFIG_PTE_64BIT)
	/* Second case is 32-bit with 64-bit PTE.  In this case, we
	 * can just store as long as we do the two halves in the right order
	 * with a barrier in between. This is possible because we take care,
	 * in the hash code, to pre-invalidate if the PTE was already hashed,
	 * which synchronizes us with any concurrent invalidation.
	 * In the percpu case, we also fallback to the simple update preserving
	 * the hash bits
	 */
	if (percpu) {
		*ptep = __pte((pte_val(*ptep) & _PAGE_HASHPTE)
			      | (pte_val(pte) & ~_PAGE_HASHPTE));
		return;
	}
#if _PAGE_HASHPTE != 0
	if (pte_val(*ptep) & _PAGE_HASHPTE)
		flush_hash_entry(mm, ptep, addr);
#endif
	__asm__ __volatile__("\
		stw%U0%X0 %2,%0\n\
		eieio\n\
		stw%U0%X0 %L2,%1"
	: "=m" (*ptep), "=m" (*((unsigned char *)ptep+4))
	: "r" (pte) : "memory");

#elif defined(CONFIG_PPC_STD_MMU_32)
	/* Third case is 32-bit hash table in UP mode, we need to preserve
	 * the _PAGE_HASHPTE bit since we may not have invalidated the previous
	 * translation in the hash yet (done in a subsequent flush_tlb_xxx())
	 * and see we need to keep track that this PTE needs invalidating
	 */
	*ptep = __pte((pte_val(*ptep) & _PAGE_HASHPTE)
		      | (pte_val(pte) & ~_PAGE_HASHPTE));

#else
	/* Anything else just stores the PTE normally. That covers all 64-bit
	 * cases, and 32-bit non-hash with 32-bit PTEs.
	 */
	*ptep = pte;
#endif
}


#define __HAVE_ARCH_PTEP_SET_ACCESS_FLAGS
extern int ptep_set_access_flags(struct vm_area_struct *vma, unsigned long address,
				 pte_t *ptep, pte_t entry, int dirty);

/*
 * Macro to mark a page protection value as "uncacheable".
 */

#define _PAGE_CACHE_CTL	(_PAGE_COHERENT | _PAGE_GUARDED | _PAGE_NO_CACHE | \
			 _PAGE_WRITETHRU)

#define pgprot_noncached(prot)	  (__pgprot((pgprot_val(prot) & ~_PAGE_CACHE_CTL) | \
				            _PAGE_NO_CACHE | _PAGE_GUARDED))

#define pgprot_noncached_wc(prot) (__pgprot((pgprot_val(prot) & ~_PAGE_CACHE_CTL) | \
				            _PAGE_NO_CACHE))

#define pgprot_cached(prot)       (__pgprot((pgprot_val(prot) & ~_PAGE_CACHE_CTL) | \
				            _PAGE_COHERENT))

#define pgprot_cached_wthru(prot) (__pgprot((pgprot_val(prot) & ~_PAGE_CACHE_CTL) | \
				            _PAGE_COHERENT | _PAGE_WRITETHRU))

#define pgprot_cached_noncoherent(prot) \
		(__pgprot(pgprot_val(prot) & ~_PAGE_CACHE_CTL))

#define pgprot_writecombine pgprot_noncached_wc

struct file;
extern pgprot_t phys_mem_access_prot(struct file *file, unsigned long pfn,
				     unsigned long size, pgprot_t vma_prot);
#define __HAVE_PHYS_MEM_ACCESS_PROT

=======
static inline unsigned long pte_pfn(pte_t pte)
{
	return (pte_val(pte) & PTE_RPN_MASK) >> PTE_RPN_SHIFT;
}

/*
 * Select all bits except the pfn
 */
static inline pgprot_t pte_pgprot(pte_t pte)
{
	unsigned long pte_flags;

	pte_flags = pte_val(pte) & ~PTE_RPN_MASK;
	return __pgprot(pte_flags);
}

static inline pgprot_t pgprot_nx(pgprot_t prot)
{
	return pte_pgprot(pte_exprotect(__pte(pgprot_val(prot))));
}
#define pgprot_nx pgprot_nx

#ifndef pmd_page_vaddr
static inline const void *pmd_page_vaddr(pmd_t pmd)
{
	return __va(pmd_val(pmd) & ~PMD_MASKED_BITS);
}
#define pmd_page_vaddr pmd_page_vaddr
#endif
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
/*
 * ZERO_PAGE is a global shared page that is always zero: used
 * for zero-mapped memory areas etc..
 */
extern unsigned long empty_zero_page[];
#define ZERO_PAGE(vaddr) (virt_to_page(empty_zero_page))

extern pgd_t swapper_pg_dir[];

extern void paging_init(void);
<<<<<<< HEAD

/*
 * kern_addr_valid is intended to indicate whether an address is a valid
 * kernel address.  Most 32-bit archs define it as always true (like this)
 * but most 64-bit archs actually perform a test.  What should we do here?
 */
#define kern_addr_valid(addr)	(1)

#define io_remap_pfn_range(vma, vaddr, pfn, size, prot)		\
		remap_pfn_range(vma, vaddr, pfn, size, prot)

#include <asm-generic/pgtable.h>

=======
void poking_init(void);

extern unsigned long ioremap_bot;
extern const pgprot_t protection_map[16];

/* can we use this in kvm */
unsigned long vmalloc_to_phys(void *vmalloc_addr);

void pgtable_cache_add(unsigned int shift);

pte_t *early_pte_alloc_kernel(pmd_t *pmdp, unsigned long va);

#if defined(CONFIG_STRICT_KERNEL_RWX) || defined(CONFIG_PPC32)
void mark_initmem_nx(void);
#else
static inline void mark_initmem_nx(void) { }
#endif

#define __HAVE_ARCH_PTEP_SET_ACCESS_FLAGS
int ptep_set_access_flags(struct vm_area_struct *vma, unsigned long address,
			  pte_t *ptep, pte_t entry, int dirty);

pgprot_t __phys_mem_access_prot(unsigned long pfn, unsigned long size,
				pgprot_t vma_prot);

struct file;
static inline pgprot_t phys_mem_access_prot(struct file *file, unsigned long pfn,
					    unsigned long size, pgprot_t vma_prot)
{
	return __phys_mem_access_prot(pfn, size, vma_prot);
}
#define __HAVE_PHYS_MEM_ACCESS_PROT

void __update_mmu_cache(struct vm_area_struct *vma, unsigned long address, pte_t *ptep);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

/*
 * This gets called at the end of handling a page fault, when
 * the kernel has put a new PTE into the page table for the process.
 * We use it to ensure coherency between the i-cache and d-cache
 * for the page which has just been mapped in.
 * On machines which use an MMU hash table, we use this to put a
 * corresponding HPTE into the hash table ahead of time, instead of
 * waiting for the inevitable extra hash-table miss exception.
 */
<<<<<<< HEAD
extern void update_mmu_cache(struct vm_area_struct *, unsigned long, pte_t *);

extern int gup_hugepd(hugepd_t *hugepd, unsigned pdshift, unsigned long addr,
		      unsigned long end, int write, struct page **pages, int *nr);

#endif /* __ASSEMBLY__ */

#endif /* __KERNEL__ */
=======
static inline void update_mmu_cache_range(struct vm_fault *vmf,
		struct vm_area_struct *vma, unsigned long address,
		pte_t *ptep, unsigned int nr)
{
	if ((mmu_has_feature(MMU_FTR_HPTE_TABLE) && !radix_enabled()) ||
	    (IS_ENABLED(CONFIG_PPC_E500) && IS_ENABLED(CONFIG_HUGETLB_PAGE)))
		__update_mmu_cache(vma, address, ptep);
}

/*
 * When used, PTE_FRAG_NR is defined in subarch pgtable.h
 * so we are sure it is included when arriving here.
 */
#ifdef PTE_FRAG_NR
static inline void *pte_frag_get(mm_context_t *ctx)
{
	return ctx->pte_frag;
}

static inline void pte_frag_set(mm_context_t *ctx, void *p)
{
	ctx->pte_frag = p;
}
#else
#define PTE_FRAG_NR		1
#define PTE_FRAG_SIZE_SHIFT	PAGE_SHIFT
#define PTE_FRAG_SIZE		(1UL << PTE_FRAG_SIZE_SHIFT)

static inline void *pte_frag_get(mm_context_t *ctx)
{
	return NULL;
}

static inline void pte_frag_set(mm_context_t *ctx, void *p)
{
}
#endif

#define pmd_pgtable pmd_pgtable
static inline pgtable_t pmd_pgtable(pmd_t pmd)
{
	return (pgtable_t)pmd_page_vaddr(pmd);
}

#ifdef CONFIG_PPC64
int __meminit vmemmap_populated(unsigned long vmemmap_addr, int vmemmap_map_size);
bool altmap_cross_boundary(struct vmem_altmap *altmap, unsigned long start,
			   unsigned long page_size);
/*
 * mm/memory_hotplug.c:mhp_supports_memmap_on_memory goes into details
 * some of the restrictions. We don't check for PMD_SIZE because our
 * vmemmap allocation code can fallback correctly. The pageblock
 * alignment requirement is met using altmap->reserve blocks.
 */
#define arch_supports_memmap_on_memory arch_supports_memmap_on_memory
static inline bool arch_supports_memmap_on_memory(unsigned long vmemmap_size)
{
	if (!radix_enabled())
		return false;
	/*
	 * With 4K page size and 2M PMD_SIZE, we can align
	 * things better with memory block size value
	 * starting from 128MB. Hence align things with PMD_SIZE.
	 */
	if (IS_ENABLED(CONFIG_PPC_4K_PAGES))
		return IS_ALIGNED(vmemmap_size, PMD_SIZE);
	return true;
}

#endif /* CONFIG_PPC64 */

#endif /* __ASSEMBLY__ */

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#endif /* _ASM_POWERPC_PGTABLE_H */
