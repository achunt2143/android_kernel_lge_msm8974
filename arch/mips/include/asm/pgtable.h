/*
 * This file is subject to the terms and conditions of the GNU General Public
 * License.  See the file "COPYING" in the main directory of this archive
 * for more details.
 *
 * Copyright (C) 2003 Ralf Baechle
 */
#ifndef _ASM_PGTABLE_H
#define _ASM_PGTABLE_H

<<<<<<< HEAD
=======
#include <linux/mm_types.h>
#include <linux/mmzone.h>
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#ifdef CONFIG_32BIT
#include <asm/pgtable-32.h>
#endif
#ifdef CONFIG_64BIT
#include <asm/pgtable-64.h>
#endif

<<<<<<< HEAD
#include <asm/io.h>
#include <asm/pgtable-bits.h>
=======
#include <asm/cmpxchg.h>
#include <asm/io.h>
#include <asm/pgtable-bits.h>
#include <asm/cpu-features.h>
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

struct mm_struct;
struct vm_area_struct;

<<<<<<< HEAD
#define PAGE_NONE	__pgprot(_PAGE_PRESENT | _CACHE_CACHABLE_NONCOHERENT)
#define PAGE_SHARED	__pgprot(_PAGE_PRESENT | _PAGE_WRITE | (kernel_uses_smartmips_rixi ? 0 : _PAGE_READ) | \
				 _page_cachable_default)
#define PAGE_COPY	__pgprot(_PAGE_PRESENT | (kernel_uses_smartmips_rixi ? 0 : _PAGE_READ) | \
				 (kernel_uses_smartmips_rixi ?  _PAGE_NO_EXEC : 0) | _page_cachable_default)
#define PAGE_READONLY	__pgprot(_PAGE_PRESENT | (kernel_uses_smartmips_rixi ? 0 : _PAGE_READ) | \
				 _page_cachable_default)
#define PAGE_KERNEL	__pgprot(_PAGE_PRESENT | __READABLE | __WRITEABLE | \
				 _PAGE_GLOBAL | _page_cachable_default)
#define PAGE_USERIO	__pgprot(_PAGE_PRESENT | (kernel_uses_smartmips_rixi ? 0 : _PAGE_READ) | _PAGE_WRITE | \
				 _page_cachable_default)
=======
#define PAGE_SHARED	vm_get_page_prot(VM_READ|VM_WRITE|VM_SHARED)

#define PAGE_KERNEL	__pgprot(_PAGE_PRESENT | __READABLE | __WRITEABLE | \
				 _PAGE_GLOBAL | _page_cachable_default)
#define PAGE_KERNEL_NC	__pgprot(_PAGE_PRESENT | __READABLE | __WRITEABLE | \
				 _PAGE_GLOBAL | _CACHE_CACHABLE_NONCOHERENT)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#define PAGE_KERNEL_UNCACHED __pgprot(_PAGE_PRESENT | __READABLE | \
			__WRITEABLE | _PAGE_GLOBAL | _CACHE_UNCACHED)

/*
 * If _PAGE_NO_EXEC is not defined, we can't do page protection for
 * execute, and consider it to be the same as read. Also, write
 * permissions imply read permissions. This is the closest we can get
 * by reasonable means..
 */

<<<<<<< HEAD
/*
 * Dummy values to fill the table in mmap.c
 * The real values will be generated at runtime
 */
#define __P000 __pgprot(0)
#define __P001 __pgprot(0)
#define __P010 __pgprot(0)
#define __P011 __pgprot(0)
#define __P100 __pgprot(0)
#define __P101 __pgprot(0)
#define __P110 __pgprot(0)
#define __P111 __pgprot(0)

#define __S000 __pgprot(0)
#define __S001 __pgprot(0)
#define __S010 __pgprot(0)
#define __S011 __pgprot(0)
#define __S100 __pgprot(0)
#define __S101 __pgprot(0)
#define __S110 __pgprot(0)
#define __S111 __pgprot(0)

extern unsigned long _page_cachable_default;
=======
extern unsigned long _page_cachable_default;
extern void __update_cache(unsigned long address, pte_t pte);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

/*
 * ZERO_PAGE is a global shared page that is always zero; used
 * for zero-mapped memory areas etc..
 */

extern unsigned long empty_zero_page;
extern unsigned long zero_page_mask;

#define ZERO_PAGE(vaddr) \
	(virt_to_page((void *)(empty_zero_page + (((unsigned long)(vaddr)) & zero_page_mask))))
<<<<<<< HEAD

#define is_zero_pfn is_zero_pfn
static inline int is_zero_pfn(unsigned long pfn)
{
	extern unsigned long zero_pfn;
	unsigned long offset_from_zero_pfn = pfn - zero_pfn;
	return offset_from_zero_pfn <= (zero_page_mask >> PAGE_SHIFT);
}

#define my_zero_pfn(addr)	page_to_pfn(ZERO_PAGE(addr))
=======
#define __HAVE_COLOR_ZERO_PAGE
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

extern void paging_init(void);

/*
 * Conversion functions: convert a page and protection to a page entry,
 * and a page entry and page directory to the page they refer to.
 */
#define pmd_phys(pmd)		virt_to_phys((void *)pmd_val(pmd))
<<<<<<< HEAD
#define pmd_page(pmd)		(pfn_to_page(pmd_phys(pmd) >> PAGE_SHIFT))
#define pmd_page_vaddr(pmd)	pmd_val(pmd)

#if defined(CONFIG_64BIT_PHYS_ADDR) && defined(CONFIG_CPU_MIPS32)

#define pte_none(pte)		(!(((pte).pte_low | (pte).pte_high) & ~_PAGE_GLOBAL))
#define pte_present(pte)	((pte).pte_low & _PAGE_PRESENT)
=======

static inline unsigned long pmd_pfn(pmd_t pmd)
{
	return pmd_val(pmd) >> PFN_PTE_SHIFT;
}

#ifndef CONFIG_MIPS_HUGE_TLB_SUPPORT
#define pmd_page(pmd)		(pfn_to_page(pmd_phys(pmd) >> PAGE_SHIFT))
#endif /* CONFIG_MIPS_HUGE_TLB_SUPPORT */

#define pmd_page_vaddr(pmd)	pmd_val(pmd)

#define htw_stop()							\
do {									\
	unsigned long __flags;						\
									\
	if (cpu_has_htw) {						\
		local_irq_save(__flags);				\
		if(!raw_current_cpu_data.htw_seq++) {			\
			write_c0_pwctl(read_c0_pwctl() &		\
				       ~(1 << MIPS_PWCTL_PWEN_SHIFT));	\
			back_to_back_c0_hazard();			\
		}							\
		local_irq_restore(__flags);				\
	}								\
} while(0)

#define htw_start()							\
do {									\
	unsigned long __flags;						\
									\
	if (cpu_has_htw) {						\
		local_irq_save(__flags);				\
		if (!--raw_current_cpu_data.htw_seq) {			\
			write_c0_pwctl(read_c0_pwctl() |		\
				       (1 << MIPS_PWCTL_PWEN_SHIFT));	\
			back_to_back_c0_hazard();			\
		}							\
		local_irq_restore(__flags);				\
	}								\
} while(0)

#if defined(CONFIG_PHYS_ADDR_T_64BIT) && defined(CONFIG_CPU_MIPS32)

#ifdef CONFIG_XPA
# define pte_none(pte)		(!(((pte).pte_high) & ~_PAGE_GLOBAL))
#else
# define pte_none(pte)		(!(((pte).pte_low | (pte).pte_high) & ~_PAGE_GLOBAL))
#endif

#define pte_present(pte)	((pte).pte_low & _PAGE_PRESENT)
#define pte_no_exec(pte)	((pte).pte_low & _PAGE_NO_EXEC)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

static inline void set_pte(pte_t *ptep, pte_t pte)
{
	ptep->pte_high = pte.pte_high;
	smp_wmb();
	ptep->pte_low = pte.pte_low;
<<<<<<< HEAD
	//printk("pte_high %x pte_low %x\n", ptep->pte_high, ptep->pte_low);

	if (pte.pte_low & _PAGE_GLOBAL) {
=======

#ifdef CONFIG_XPA
	if (pte.pte_high & _PAGE_GLOBAL) {
#else
	if (pte.pte_low & _PAGE_GLOBAL) {
#endif
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		pte_t *buddy = ptep_buddy(ptep);
		/*
		 * Make sure the buddy is global too (if it's !none,
		 * it better already be global)
		 */
		if (pte_none(*buddy)) {
<<<<<<< HEAD
			buddy->pte_low  |= _PAGE_GLOBAL;
=======
			if (!IS_ENABLED(CONFIG_XPA))
				buddy->pte_low |= _PAGE_GLOBAL;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
			buddy->pte_high |= _PAGE_GLOBAL;
		}
	}
}
<<<<<<< HEAD
#define set_pte_at(mm, addr, ptep, pteval) set_pte(ptep, pteval)
=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

static inline void pte_clear(struct mm_struct *mm, unsigned long addr, pte_t *ptep)
{
	pte_t null = __pte(0);

<<<<<<< HEAD
	/* Preserve global status for the pair */
	if (ptep_buddy(ptep)->pte_low & _PAGE_GLOBAL)
		null.pte_low = null.pte_high = _PAGE_GLOBAL;

	set_pte_at(mm, addr, ptep, null);
=======
	htw_stop();
	/* Preserve global status for the pair */
	if (IS_ENABLED(CONFIG_XPA)) {
		if (ptep_buddy(ptep)->pte_high & _PAGE_GLOBAL)
			null.pte_high = _PAGE_GLOBAL;
	} else {
		if (ptep_buddy(ptep)->pte_low & _PAGE_GLOBAL)
			null.pte_low = null.pte_high = _PAGE_GLOBAL;
	}

	set_pte(ptep, null);
	htw_start();
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}
#else

#define pte_none(pte)		(!(pte_val(pte) & ~_PAGE_GLOBAL))
#define pte_present(pte)	(pte_val(pte) & _PAGE_PRESENT)
<<<<<<< HEAD
=======
#define pte_no_exec(pte)	(pte_val(pte) & _PAGE_NO_EXEC)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

/*
 * Certain architectures need to do special things when pte's
 * within a page table are directly modified.  Thus, the following
 * hook is made available.
 */
static inline void set_pte(pte_t *ptep, pte_t pteval)
{
	*ptep = pteval;
<<<<<<< HEAD
#if !defined(CONFIG_CPU_R3000) && !defined(CONFIG_CPU_TX39XX)
=======
#if !defined(CONFIG_CPU_R3K_TLB)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	if (pte_val(pteval) & _PAGE_GLOBAL) {
		pte_t *buddy = ptep_buddy(ptep);
		/*
		 * Make sure the buddy is global too (if it's !none,
		 * it better already be global)
		 */
<<<<<<< HEAD
#ifdef CONFIG_SMP
		/*
		 * For SMP, multiple CPUs can race, so we need to do
		 * this atomically.
		 */
#ifdef CONFIG_64BIT
#define LL_INSN "lld"
#define SC_INSN "scd"
#else /* CONFIG_32BIT */
#define LL_INSN "ll"
#define SC_INSN "sc"
#endif
		unsigned long page_global = _PAGE_GLOBAL;
		unsigned long tmp;

		__asm__ __volatile__ (
			"	.set	push\n"
			"	.set	noreorder\n"
			"1:	" LL_INSN "	%[tmp], %[buddy]\n"
			"	bnez	%[tmp], 2f\n"
			"	 or	%[tmp], %[tmp], %[global]\n"
			"	" SC_INSN "	%[tmp], %[buddy]\n"
			"	beqz	%[tmp], 1b\n"
			"	 nop\n"
			"2:\n"
			"	.set pop"
			: [buddy] "+m" (buddy->pte),
			  [tmp] "=&r" (tmp)
			: [global] "r" (page_global));
#else /* !CONFIG_SMP */
		if (pte_none(*buddy))
			pte_val(*buddy) = pte_val(*buddy) | _PAGE_GLOBAL;
#endif /* CONFIG_SMP */
	}
#endif
}
#define set_pte_at(mm, addr, ptep, pteval) set_pte(ptep, pteval)

static inline void pte_clear(struct mm_struct *mm, unsigned long addr, pte_t *ptep)
{
#if !defined(CONFIG_CPU_R3000) && !defined(CONFIG_CPU_TX39XX)
	/* Preserve global status for the pair */
	if (pte_val(*ptep_buddy(ptep)) & _PAGE_GLOBAL)
		set_pte_at(mm, addr, ptep, __pte(_PAGE_GLOBAL));
	else
#endif
		set_pte_at(mm, addr, ptep, __pte(0));
}
#endif

=======
# if defined(CONFIG_PHYS_ADDR_T_64BIT) && !defined(CONFIG_CPU_MIPS32)
		cmpxchg64(&buddy->pte, 0, _PAGE_GLOBAL);
# else
		cmpxchg(&buddy->pte, 0, _PAGE_GLOBAL);
# endif
	}
#endif
}

static inline void pte_clear(struct mm_struct *mm, unsigned long addr, pte_t *ptep)
{
	htw_stop();
#if !defined(CONFIG_CPU_R3K_TLB)
	/* Preserve global status for the pair */
	if (pte_val(*ptep_buddy(ptep)) & _PAGE_GLOBAL)
		set_pte(ptep, __pte(_PAGE_GLOBAL));
	else
#endif
		set_pte(ptep, __pte(0));
	htw_start();
}
#endif

static inline void set_ptes(struct mm_struct *mm, unsigned long addr,
		pte_t *ptep, pte_t pte, unsigned int nr)
{
	unsigned int i;
	bool do_sync = false;

	for (i = 0; i < nr; i++) {
		if (!pte_present(pte))
			continue;
		if (pte_present(ptep[i]) &&
		    (pte_pfn(ptep[i]) == pte_pfn(pte)))
			continue;
		do_sync = true;
	}

	if (do_sync)
		__update_cache(addr, pte);

	for (;;) {
		set_pte(ptep, pte);
		if (--nr == 0)
			break;
		ptep++;
		pte = __pte(pte_val(pte) + (1UL << PFN_PTE_SHIFT));
	}
}
#define set_ptes set_ptes

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
/*
 * (pmds are folded into puds so this doesn't get actually called,
 * but the define is needed for a generic inline function.)
 */
#define set_pmd(pmdptr, pmdval) do { *(pmdptr) = (pmdval); } while(0)

#ifndef __PAGETABLE_PMD_FOLDED
/*
 * (puds are folded into pgds so this doesn't get actually called,
 * but the define is needed for a generic inline function.)
 */
#define set_pud(pudptr, pudval) do { *(pudptr) = (pudval); } while(0)
#endif

#define PGD_T_LOG2	(__builtin_ffs(sizeof(pgd_t)) - 1)
#define PMD_T_LOG2	(__builtin_ffs(sizeof(pmd_t)) - 1)
#define PTE_T_LOG2	(__builtin_ffs(sizeof(pte_t)) - 1)

/*
 * We used to declare this array with size but gcc 3.3 and older are not able
 * to find that this expression is a constant, so the size is dropped.
 */
extern pgd_t swapper_pg_dir[];

/*
<<<<<<< HEAD
 * The following only work if pte_present() is true.
 * Undefined behaviour if not..
 */
#if defined(CONFIG_64BIT_PHYS_ADDR) && defined(CONFIG_CPU_MIPS32)
static inline int pte_write(pte_t pte)	{ return pte.pte_low & _PAGE_WRITE; }
static inline int pte_dirty(pte_t pte)	{ return pte.pte_low & _PAGE_MODIFIED; }
static inline int pte_young(pte_t pte)	{ return pte.pte_low & _PAGE_ACCESSED; }
static inline int pte_file(pte_t pte)	{ return pte.pte_low & _PAGE_FILE; }

static inline pte_t pte_wrprotect(pte_t pte)
{
	pte.pte_low  &= ~(_PAGE_WRITE | _PAGE_SILENT_WRITE);
=======
 * Platform specific pte_special() and pte_mkspecial() definitions
 * are required only when ARCH_HAS_PTE_SPECIAL is enabled.
 */
#if defined(CONFIG_ARCH_HAS_PTE_SPECIAL)
#if defined(CONFIG_PHYS_ADDR_T_64BIT) && defined(CONFIG_CPU_MIPS32)
static inline int pte_special(pte_t pte)
{
	return pte.pte_low & _PAGE_SPECIAL;
}

static inline pte_t pte_mkspecial(pte_t pte)
{
	pte.pte_low |= _PAGE_SPECIAL;
	return pte;
}
#else
static inline int pte_special(pte_t pte)
{
	return pte_val(pte) & _PAGE_SPECIAL;
}

static inline pte_t pte_mkspecial(pte_t pte)
{
	pte_val(pte) |= _PAGE_SPECIAL;
	return pte;
}
#endif
#endif /* CONFIG_ARCH_HAS_PTE_SPECIAL */

/*
 * The following only work if pte_present() is true.
 * Undefined behaviour if not..
 */
#if defined(CONFIG_PHYS_ADDR_T_64BIT) && defined(CONFIG_CPU_MIPS32)
static inline int pte_write(pte_t pte)	{ return pte.pte_low & _PAGE_WRITE; }
static inline int pte_dirty(pte_t pte)	{ return pte.pte_low & _PAGE_MODIFIED; }
static inline int pte_young(pte_t pte)	{ return pte.pte_low & _PAGE_ACCESSED; }

static inline pte_t pte_wrprotect(pte_t pte)
{
	pte.pte_low  &= ~_PAGE_WRITE;
	if (!IS_ENABLED(CONFIG_XPA))
		pte.pte_low &= ~_PAGE_SILENT_WRITE;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	pte.pte_high &= ~_PAGE_SILENT_WRITE;
	return pte;
}

static inline pte_t pte_mkclean(pte_t pte)
{
<<<<<<< HEAD
	pte.pte_low  &= ~(_PAGE_MODIFIED | _PAGE_SILENT_WRITE);
=======
	pte.pte_low  &= ~_PAGE_MODIFIED;
	if (!IS_ENABLED(CONFIG_XPA))
		pte.pte_low &= ~_PAGE_SILENT_WRITE;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	pte.pte_high &= ~_PAGE_SILENT_WRITE;
	return pte;
}

static inline pte_t pte_mkold(pte_t pte)
{
<<<<<<< HEAD
	pte.pte_low  &= ~(_PAGE_ACCESSED | _PAGE_SILENT_READ);
=======
	pte.pte_low  &= ~_PAGE_ACCESSED;
	if (!IS_ENABLED(CONFIG_XPA))
		pte.pte_low &= ~_PAGE_SILENT_READ;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	pte.pte_high &= ~_PAGE_SILENT_READ;
	return pte;
}

<<<<<<< HEAD
static inline pte_t pte_mkwrite(pte_t pte)
{
	pte.pte_low |= _PAGE_WRITE;
	if (pte.pte_low & _PAGE_MODIFIED) {
		pte.pte_low  |= _PAGE_SILENT_WRITE;
=======
static inline pte_t pte_mkwrite_novma(pte_t pte)
{
	pte.pte_low |= _PAGE_WRITE;
	if (pte.pte_low & _PAGE_MODIFIED) {
		if (!IS_ENABLED(CONFIG_XPA))
			pte.pte_low |= _PAGE_SILENT_WRITE;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		pte.pte_high |= _PAGE_SILENT_WRITE;
	}
	return pte;
}

static inline pte_t pte_mkdirty(pte_t pte)
{
	pte.pte_low |= _PAGE_MODIFIED;
	if (pte.pte_low & _PAGE_WRITE) {
<<<<<<< HEAD
		pte.pte_low  |= _PAGE_SILENT_WRITE;
=======
		if (!IS_ENABLED(CONFIG_XPA))
			pte.pte_low |= _PAGE_SILENT_WRITE;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		pte.pte_high |= _PAGE_SILENT_WRITE;
	}
	return pte;
}

static inline pte_t pte_mkyoung(pte_t pte)
{
	pte.pte_low |= _PAGE_ACCESSED;
<<<<<<< HEAD
	if (pte.pte_low & _PAGE_READ) {
		pte.pte_low  |= _PAGE_SILENT_READ;
=======
	if (!(pte.pte_low & _PAGE_NO_READ)) {
		if (!IS_ENABLED(CONFIG_XPA))
			pte.pte_low |= _PAGE_SILENT_READ;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		pte.pte_high |= _PAGE_SILENT_READ;
	}
	return pte;
}
#else
static inline int pte_write(pte_t pte)	{ return pte_val(pte) & _PAGE_WRITE; }
static inline int pte_dirty(pte_t pte)	{ return pte_val(pte) & _PAGE_MODIFIED; }
static inline int pte_young(pte_t pte)	{ return pte_val(pte) & _PAGE_ACCESSED; }
<<<<<<< HEAD
static inline int pte_file(pte_t pte)	{ return pte_val(pte) & _PAGE_FILE; }
=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

static inline pte_t pte_wrprotect(pte_t pte)
{
	pte_val(pte) &= ~(_PAGE_WRITE | _PAGE_SILENT_WRITE);
	return pte;
}

static inline pte_t pte_mkclean(pte_t pte)
{
<<<<<<< HEAD
	pte_val(pte) &= ~(_PAGE_MODIFIED|_PAGE_SILENT_WRITE);
=======
	pte_val(pte) &= ~(_PAGE_MODIFIED | _PAGE_SILENT_WRITE);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	return pte;
}

static inline pte_t pte_mkold(pte_t pte)
{
<<<<<<< HEAD
	pte_val(pte) &= ~(_PAGE_ACCESSED|_PAGE_SILENT_READ);
	return pte;
}

static inline pte_t pte_mkwrite(pte_t pte)
=======
	pte_val(pte) &= ~(_PAGE_ACCESSED | _PAGE_SILENT_READ);
	return pte;
}

static inline pte_t pte_mkwrite_novma(pte_t pte)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
{
	pte_val(pte) |= _PAGE_WRITE;
	if (pte_val(pte) & _PAGE_MODIFIED)
		pte_val(pte) |= _PAGE_SILENT_WRITE;
	return pte;
}

static inline pte_t pte_mkdirty(pte_t pte)
{
<<<<<<< HEAD
	pte_val(pte) |= _PAGE_MODIFIED;
=======
	pte_val(pte) |= _PAGE_MODIFIED | _PAGE_SOFT_DIRTY;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	if (pte_val(pte) & _PAGE_WRITE)
		pte_val(pte) |= _PAGE_SILENT_WRITE;
	return pte;
}

static inline pte_t pte_mkyoung(pte_t pte)
{
	pte_val(pte) |= _PAGE_ACCESSED;
<<<<<<< HEAD
	if (kernel_uses_smartmips_rixi) {
		if (!(pte_val(pte) & _PAGE_NO_READ))
			pte_val(pte) |= _PAGE_SILENT_READ;
	} else {
		if (pte_val(pte) & _PAGE_READ)
			pte_val(pte) |= _PAGE_SILENT_READ;
	}
	return pte;
}

#ifdef _PAGE_HUGE
=======
	if (!(pte_val(pte) & _PAGE_NO_READ))
		pte_val(pte) |= _PAGE_SILENT_READ;
	return pte;
}

#define pte_sw_mkyoung	pte_mkyoung

#ifdef CONFIG_MIPS_HUGE_TLB_SUPPORT
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
static inline int pte_huge(pte_t pte)	{ return pte_val(pte) & _PAGE_HUGE; }

static inline pte_t pte_mkhuge(pte_t pte)
{
	pte_val(pte) |= _PAGE_HUGE;
	return pte;
}
<<<<<<< HEAD
#endif /* _PAGE_HUGE */
#endif
static inline int pte_special(pte_t pte)	{ return 0; }
static inline pte_t pte_mkspecial(pte_t pte)	{ return pte; }

/*
 * Macro to make mark a page protection value as "uncacheable".  Note
=======

#define pmd_write pmd_write
static inline int pmd_write(pmd_t pmd)
{
	return !!(pmd_val(pmd) & _PAGE_WRITE);
}

static inline struct page *pmd_page(pmd_t pmd)
{
	if (pmd_val(pmd) & _PAGE_HUGE)
		return pfn_to_page(pmd_pfn(pmd));

	return pfn_to_page(pmd_phys(pmd) >> PAGE_SHIFT);
}
#endif /* CONFIG_MIPS_HUGE_TLB_SUPPORT */

#ifdef CONFIG_HAVE_ARCH_SOFT_DIRTY
static inline bool pte_soft_dirty(pte_t pte)
{
	return pte_val(pte) & _PAGE_SOFT_DIRTY;
}
#define pte_swp_soft_dirty pte_soft_dirty

static inline pte_t pte_mksoft_dirty(pte_t pte)
{
	pte_val(pte) |= _PAGE_SOFT_DIRTY;
	return pte;
}
#define pte_swp_mksoft_dirty pte_mksoft_dirty

static inline pte_t pte_clear_soft_dirty(pte_t pte)
{
	pte_val(pte) &= ~(_PAGE_SOFT_DIRTY);
	return pte;
}
#define pte_swp_clear_soft_dirty pte_clear_soft_dirty

#endif /* CONFIG_HAVE_ARCH_SOFT_DIRTY */

#endif

/*
 * Macro to make mark a page protection value as "uncacheable".	 Note
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 * that "protection" is really a misnomer here as the protection value
 * contains the memory attribute bits, dirty bits, and various other
 * bits as well.
 */
#define pgprot_noncached pgprot_noncached

static inline pgprot_t pgprot_noncached(pgprot_t _prot)
{
	unsigned long prot = pgprot_val(_prot);

	prot = (prot & ~_CACHE_MASK) | _CACHE_UNCACHED;

	return __pgprot(prot);
}

<<<<<<< HEAD
=======
#define pgprot_writecombine pgprot_writecombine

static inline pgprot_t pgprot_writecombine(pgprot_t _prot)
{
	unsigned long prot = pgprot_val(_prot);

	/* cpu_data[0].writecombine is already shifted by _CACHE_SHIFT */
	prot = (prot & ~_CACHE_MASK) | cpu_data[0].writecombine;

	return __pgprot(prot);
}

static inline void flush_tlb_fix_spurious_fault(struct vm_area_struct *vma,
						unsigned long address,
						pte_t *ptep)
{
}

#define __HAVE_ARCH_PTE_SAME
static inline int pte_same(pte_t pte_a, pte_t pte_b)
{
	return pte_val(pte_a) == pte_val(pte_b);
}

#define __HAVE_ARCH_PTEP_SET_ACCESS_FLAGS
static inline int ptep_set_access_flags(struct vm_area_struct *vma,
					unsigned long address, pte_t *ptep,
					pte_t entry, int dirty)
{
	if (!pte_same(*ptep, entry))
		set_pte(ptep, entry);
	/*
	 * update_mmu_cache will unconditionally execute, handling both
	 * the case that the PTE changed and the spurious fault case.
	 */
	return true;
}

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
/*
 * Conversion functions: convert a page and protection to a page entry,
 * and a page entry and page directory to the page they refer to.
 */
#define mk_pte(page, pgprot)	pfn_pte(page_to_pfn(page), (pgprot))

<<<<<<< HEAD
#if defined(CONFIG_64BIT_PHYS_ADDR) && defined(CONFIG_CPU_MIPS32)
static inline pte_t pte_modify(pte_t pte, pgprot_t newprot)
{
	pte.pte_low  &= _PAGE_CHG_MASK;
	pte.pte_high &= ~0x3f;
	pte.pte_low  |= pgprot_val(newprot);
	pte.pte_high |= pgprot_val(newprot) & 0x3f;
=======
#if defined(CONFIG_XPA)
static inline pte_t pte_modify(pte_t pte, pgprot_t newprot)
{
	pte.pte_low  &= (_PAGE_MODIFIED | _PAGE_ACCESSED | _PFNX_MASK);
	pte.pte_high &= (_PFN_MASK | _CACHE_MASK);
	pte.pte_low  |= pgprot_val(newprot) & ~_PFNX_MASK;
	pte.pte_high |= pgprot_val(newprot) & ~(_PFN_MASK | _CACHE_MASK);
	return pte;
}
#elif defined(CONFIG_PHYS_ADDR_T_64BIT) && defined(CONFIG_CPU_MIPS32)
static inline pte_t pte_modify(pte_t pte, pgprot_t newprot)
{
	pte.pte_low  &= _PAGE_CHG_MASK;
	pte.pte_high &= (_PFN_MASK | _CACHE_MASK);
	pte.pte_low  |= pgprot_val(newprot);
	pte.pte_high |= pgprot_val(newprot) & ~(_PFN_MASK | _CACHE_MASK);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	return pte;
}
#else
static inline pte_t pte_modify(pte_t pte, pgprot_t newprot)
{
<<<<<<< HEAD
	return __pte((pte_val(pte) & _PAGE_CHG_MASK) | pgprot_val(newprot));
}
#endif


extern void __update_tlb(struct vm_area_struct *vma, unsigned long address,
	pte_t pte);
extern void __update_cache(struct vm_area_struct *vma, unsigned long address,
	pte_t pte);

static inline void update_mmu_cache(struct vm_area_struct *vma,
	unsigned long address, pte_t *ptep)
{
	pte_t pte = *ptep;
	__update_tlb(vma, address, pte);
	__update_cache(vma, address, pte);
}

#define kern_addr_valid(addr)	(1)

#ifdef CONFIG_64BIT_PHYS_ADDR
extern int remap_pfn_range(struct vm_area_struct *vma, unsigned long from, unsigned long pfn, unsigned long size, pgprot_t prot);

static inline int io_remap_pfn_range(struct vm_area_struct *vma,
		unsigned long vaddr,
		unsigned long pfn,
		unsigned long size,
		pgprot_t prot)
{
	phys_t phys_addr_high = fixup_bigphys_addr(pfn << PAGE_SHIFT, size);
	return remap_pfn_range(vma, vaddr, phys_addr_high >> PAGE_SHIFT, size, prot);
}
#else
#define io_remap_pfn_range(vma, vaddr, pfn, size, prot)		\
		remap_pfn_range(vma, vaddr, pfn, size, prot)
#endif

#include <asm-generic/pgtable.h>

/*
 * uncached accelerated TLB map for video memory access
 */
#ifdef CONFIG_CPU_SUPPORTS_UNCACHED_ACCELERATED
#define __HAVE_PHYS_MEM_ACCESS_PROT

struct file;
pgprot_t phys_mem_access_prot(struct file *file, unsigned long pfn,
		unsigned long size, pgprot_t vma_prot);
int phys_mem_access_prot_allowed(struct file *file, unsigned long pfn,
		unsigned long size, pgprot_t *vma_prot);
#endif

=======
	pte_val(pte) &= _PAGE_CHG_MASK;
	pte_val(pte) |= pgprot_val(newprot) & ~_PAGE_CHG_MASK;
	if ((pte_val(pte) & _PAGE_ACCESSED) && !(pte_val(pte) & _PAGE_NO_READ))
		pte_val(pte) |= _PAGE_SILENT_READ;
	return pte;
}
#endif

#if defined(CONFIG_PHYS_ADDR_T_64BIT) && defined(CONFIG_CPU_MIPS32)
static inline int pte_swp_exclusive(pte_t pte)
{
	return pte.pte_low & _PAGE_SWP_EXCLUSIVE;
}

static inline pte_t pte_swp_mkexclusive(pte_t pte)
{
	pte.pte_low |= _PAGE_SWP_EXCLUSIVE;
	return pte;
}

static inline pte_t pte_swp_clear_exclusive(pte_t pte)
{
	pte.pte_low &= ~_PAGE_SWP_EXCLUSIVE;
	return pte;
}
#else
static inline int pte_swp_exclusive(pte_t pte)
{
	return pte_val(pte) & _PAGE_SWP_EXCLUSIVE;
}

static inline pte_t pte_swp_mkexclusive(pte_t pte)
{
	pte_val(pte) |= _PAGE_SWP_EXCLUSIVE;
	return pte;
}

static inline pte_t pte_swp_clear_exclusive(pte_t pte)
{
	pte_val(pte) &= ~_PAGE_SWP_EXCLUSIVE;
	return pte;
}
#endif

extern void __update_tlb(struct vm_area_struct *vma, unsigned long address,
	pte_t pte);

static inline void update_mmu_cache_range(struct vm_fault *vmf,
		struct vm_area_struct *vma, unsigned long address,
		pte_t *ptep, unsigned int nr)
{
	for (;;) {
		pte_t pte = *ptep;
		__update_tlb(vma, address, pte);
		if (--nr == 0)
			break;
		ptep++;
		address += PAGE_SIZE;
	}
}
#define update_mmu_cache(vma, address, ptep) \
	update_mmu_cache_range(NULL, vma, address, ptep, 1)

#define	__HAVE_ARCH_UPDATE_MMU_TLB
#define update_mmu_tlb	update_mmu_cache

static inline void update_mmu_cache_pmd(struct vm_area_struct *vma,
	unsigned long address, pmd_t *pmdp)
{
	pte_t pte = *(pte_t *)pmdp;

	__update_tlb(vma, address, pte);
}

/*
 * Allow physical addresses to be fixed up to help 36-bit peripherals.
 */
#ifdef CONFIG_MIPS_FIXUP_BIGPHYS_ADDR
phys_addr_t fixup_bigphys_addr(phys_addr_t addr, phys_addr_t size);
int io_remap_pfn_range(struct vm_area_struct *vma, unsigned long vaddr,
		unsigned long pfn, unsigned long size, pgprot_t prot);
#define io_remap_pfn_range io_remap_pfn_range
#else
#define fixup_bigphys_addr(addr, size)	(addr)
#endif /* CONFIG_MIPS_FIXUP_BIGPHYS_ADDR */

#ifdef CONFIG_TRANSPARENT_HUGEPAGE

/* We don't have hardware dirty/accessed bits, generic_pmdp_establish is fine.*/
#define pmdp_establish generic_pmdp_establish

#define has_transparent_hugepage has_transparent_hugepage
extern int has_transparent_hugepage(void);

static inline int pmd_trans_huge(pmd_t pmd)
{
	return !!(pmd_val(pmd) & _PAGE_HUGE);
}

static inline pmd_t pmd_mkhuge(pmd_t pmd)
{
	pmd_val(pmd) |= _PAGE_HUGE;

	return pmd;
}

extern void set_pmd_at(struct mm_struct *mm, unsigned long addr,
		       pmd_t *pmdp, pmd_t pmd);

static inline pmd_t pmd_wrprotect(pmd_t pmd)
{
	pmd_val(pmd) &= ~(_PAGE_WRITE | _PAGE_SILENT_WRITE);
	return pmd;
}

static inline pmd_t pmd_mkwrite_novma(pmd_t pmd)
{
	pmd_val(pmd) |= _PAGE_WRITE;
	if (pmd_val(pmd) & _PAGE_MODIFIED)
		pmd_val(pmd) |= _PAGE_SILENT_WRITE;

	return pmd;
}

#define pmd_dirty pmd_dirty
static inline int pmd_dirty(pmd_t pmd)
{
	return !!(pmd_val(pmd) & _PAGE_MODIFIED);
}

static inline pmd_t pmd_mkclean(pmd_t pmd)
{
	pmd_val(pmd) &= ~(_PAGE_MODIFIED | _PAGE_SILENT_WRITE);
	return pmd;
}

static inline pmd_t pmd_mkdirty(pmd_t pmd)
{
	pmd_val(pmd) |= _PAGE_MODIFIED | _PAGE_SOFT_DIRTY;
	if (pmd_val(pmd) & _PAGE_WRITE)
		pmd_val(pmd) |= _PAGE_SILENT_WRITE;

	return pmd;
}

#define pmd_young pmd_young
static inline int pmd_young(pmd_t pmd)
{
	return !!(pmd_val(pmd) & _PAGE_ACCESSED);
}

static inline pmd_t pmd_mkold(pmd_t pmd)
{
	pmd_val(pmd) &= ~(_PAGE_ACCESSED|_PAGE_SILENT_READ);

	return pmd;
}

static inline pmd_t pmd_mkyoung(pmd_t pmd)
{
	pmd_val(pmd) |= _PAGE_ACCESSED;

	if (!(pmd_val(pmd) & _PAGE_NO_READ))
		pmd_val(pmd) |= _PAGE_SILENT_READ;

	return pmd;
}

#ifdef CONFIG_HAVE_ARCH_SOFT_DIRTY
static inline int pmd_soft_dirty(pmd_t pmd)
{
	return !!(pmd_val(pmd) & _PAGE_SOFT_DIRTY);
}

static inline pmd_t pmd_mksoft_dirty(pmd_t pmd)
{
	pmd_val(pmd) |= _PAGE_SOFT_DIRTY;
	return pmd;
}

static inline pmd_t pmd_clear_soft_dirty(pmd_t pmd)
{
	pmd_val(pmd) &= ~(_PAGE_SOFT_DIRTY);
	return pmd;
}

#endif /* CONFIG_HAVE_ARCH_SOFT_DIRTY */

/* Extern to avoid header file madness */
extern pmd_t mk_pmd(struct page *page, pgprot_t prot);

static inline pmd_t pmd_modify(pmd_t pmd, pgprot_t newprot)
{
	pmd_val(pmd) = (pmd_val(pmd) & (_PAGE_CHG_MASK | _PAGE_HUGE)) |
		       (pgprot_val(newprot) & ~_PAGE_CHG_MASK);
	return pmd;
}

static inline pmd_t pmd_mkinvalid(pmd_t pmd)
{
	pmd_val(pmd) &= ~(_PAGE_PRESENT | _PAGE_VALID | _PAGE_DIRTY);

	return pmd;
}

/*
 * The generic version pmdp_huge_get_and_clear uses a version of pmd_clear() with a
 * different prototype.
 */
#define __HAVE_ARCH_PMDP_HUGE_GET_AND_CLEAR
static inline pmd_t pmdp_huge_get_and_clear(struct mm_struct *mm,
					    unsigned long address, pmd_t *pmdp)
{
	pmd_t old = *pmdp;

	pmd_clear(pmdp);

	return old;
}

#endif /* CONFIG_TRANSPARENT_HUGEPAGE */

#ifdef _PAGE_HUGE
#define pmd_leaf(pmd)	((pmd_val(pmd) & _PAGE_HUGE) != 0)
#define pud_leaf(pud)	((pud_val(pud) & _PAGE_HUGE) != 0)
#endif

#define gup_fast_permitted(start, end)	(!cpu_has_dc_aliases)

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
/*
 * We provide our own get_unmapped area to cope with the virtual aliasing
 * constraints placed on us by the cache architecture.
 */
#define HAVE_ARCH_UNMAPPED_AREA
#define HAVE_ARCH_UNMAPPED_AREA_TOPDOWN

<<<<<<< HEAD
/*
 * No page table caches to initialise
 */
#define pgtable_cache_init()	do { } while (0)

=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#endif /* _ASM_PGTABLE_H */
