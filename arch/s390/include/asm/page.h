<<<<<<< HEAD
/*
 *  include/asm-s390/page.h
 *
 *  S390 version
 *    Copyright (C) 1999,2000 IBM Deutschland Entwicklung GmbH, IBM Corporation
=======
/* SPDX-License-Identifier: GPL-2.0 */
/*
 *  S390 version
 *    Copyright IBM Corp. 1999, 2000
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 *    Author(s): Hartmut Penner (hp@de.ibm.com)
 */

#ifndef _S390_PAGE_H
#define _S390_PAGE_H

#include <linux/const.h>
#include <asm/types.h>

<<<<<<< HEAD
/* PAGE_SHIFT determines the page size */
#define PAGE_SHIFT      12
#define PAGE_SIZE	(_AC(1,UL) << PAGE_SHIFT)
#define PAGE_MASK       (~(PAGE_SIZE-1))
#define PAGE_DEFAULT_ACC	0
=======
#define _PAGE_SHIFT	CONFIG_PAGE_SHIFT
#define _PAGE_SIZE	(_AC(1, UL) << _PAGE_SHIFT)
#define _PAGE_MASK	(~(_PAGE_SIZE - 1))

/* PAGE_SHIFT determines the page size */
#define PAGE_SHIFT	_PAGE_SHIFT
#define PAGE_SIZE	_PAGE_SIZE
#define PAGE_MASK	_PAGE_MASK
#define PAGE_DEFAULT_ACC	_AC(0, UL)
/* storage-protection override */
#define PAGE_SPO_ACC		9
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#define PAGE_DEFAULT_KEY	(PAGE_DEFAULT_ACC << 4)

#define HPAGE_SHIFT	20
#define HPAGE_SIZE	(1UL << HPAGE_SHIFT)
#define HPAGE_MASK	(~(HPAGE_SIZE - 1))
#define HUGETLB_PAGE_ORDER	(HPAGE_SHIFT - PAGE_SHIFT)
<<<<<<< HEAD
=======
#define HUGE_MAX_HSTATE		2
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

#define ARCH_HAS_SETCLEAR_HUGE_PTE
#define ARCH_HAS_HUGE_PTE_TYPE
#define ARCH_HAS_PREPARE_HUGEPAGE
#define ARCH_HAS_HUGEPAGE_CLEAR_FLUSH

<<<<<<< HEAD
#include <asm/setup.h>
#ifndef __ASSEMBLY__

static inline void clear_page(void *page)
{
	if (MACHINE_HAS_PFMF) {
		asm volatile(
			"	.insn	rre,0xb9af0000,%0,%1"
			: : "d" (0x10000), "a" (page) : "memory", "cc");
	} else {
		register unsigned long reg1 asm ("1") = 0;
		register void *reg2 asm ("2") = page;
		register unsigned long reg3 asm ("3") = 4096;
		asm volatile(
			"	mvcl	2,0"
			: "+d" (reg2), "+d" (reg3) : "d" (reg1)
			: "memory", "cc");
	}
}

static inline void copy_page(void *to, void *from)
{
	if (MACHINE_HAS_MVPG) {
		register unsigned long reg0 asm ("0") = 0;
		asm volatile(
			"	mvpg	%0,%1"
			: : "a" (to), "a" (from), "d" (reg0)
			: "memory", "cc");
	} else
		asm volatile(
			"	mvc	0(256,%0),0(%1)\n"
			"	mvc	256(256,%0),256(%1)\n"
			"	mvc	512(256,%0),512(%1)\n"
			"	mvc	768(256,%0),768(%1)\n"
			"	mvc	1024(256,%0),1024(%1)\n"
			"	mvc	1280(256,%0),1280(%1)\n"
			"	mvc	1536(256,%0),1536(%1)\n"
			"	mvc	1792(256,%0),1792(%1)\n"
			"	mvc	2048(256,%0),2048(%1)\n"
			"	mvc	2304(256,%0),2304(%1)\n"
			"	mvc	2560(256,%0),2560(%1)\n"
			"	mvc	2816(256,%0),2816(%1)\n"
			"	mvc	3072(256,%0),3072(%1)\n"
			"	mvc	3328(256,%0),3328(%1)\n"
			"	mvc	3584(256,%0),3584(%1)\n"
			"	mvc	3840(256,%0),3840(%1)\n"
			: : "a" (to), "a" (from) : "memory");
=======
#define HAVE_ARCH_HUGETLB_UNMAPPED_AREA

#include <asm/setup.h>
#ifndef __ASSEMBLY__

void __storage_key_init_range(unsigned long start, unsigned long end);

static inline void storage_key_init_range(unsigned long start, unsigned long end)
{
	if (PAGE_DEFAULT_KEY != 0)
		__storage_key_init_range(start, end);
}

#define clear_page(page)	memset((page), 0, PAGE_SIZE)

/*
 * copy_page uses the mvcl instruction with 0xb0 padding byte in order to
 * bypass caches when copying a page. Especially when copying huge pages
 * this keeps L1 and L2 data caches alive.
 */
static inline void copy_page(void *to, void *from)
{
	union register_pair dst, src;

	dst.even = (unsigned long) to;
	dst.odd  = 0x1000;
	src.even = (unsigned long) from;
	src.odd  = 0xb0001000;

	asm volatile(
		"	mvcl	%[dst],%[src]"
		: [dst] "+&d" (dst.pair), [src] "+&d" (src.pair)
		: : "memory", "cc");
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

#define clear_user_page(page, vaddr, pg)	clear_page(page)
#define copy_user_page(to, from, vaddr, pg)	copy_page(to, from)

<<<<<<< HEAD
#define __alloc_zeroed_user_highpage(movableflags, vma, vaddr) \
	alloc_page_vma(GFP_HIGHUSER | __GFP_ZERO | movableflags, vma, vaddr)
#define __HAVE_ARCH_ALLOC_ZEROED_USER_HIGHPAGE
=======
#define vma_alloc_zeroed_movable_folio(vma, vaddr) \
	vma_alloc_folio(GFP_HIGHUSER_MOVABLE | __GFP_ZERO, 0, vma, vaddr, false)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

/*
 * These are used to make use of C type-checking..
 */

typedef struct { unsigned long pgprot; } pgprot_t;
typedef struct { unsigned long pgste; } pgste_t;
typedef struct { unsigned long pte; } pte_t;
typedef struct { unsigned long pmd; } pmd_t;
typedef struct { unsigned long pud; } pud_t;
<<<<<<< HEAD
=======
typedef struct { unsigned long p4d; } p4d_t;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
typedef struct { unsigned long pgd; } pgd_t;
typedef pte_t *pgtable_t;

#define pgprot_val(x)	((x).pgprot)
#define pgste_val(x)	((x).pgste)
<<<<<<< HEAD
#define pte_val(x)	((x).pte)
#define pmd_val(x)	((x).pmd)
#define pud_val(x)	((x).pud)
#define pgd_val(x)      ((x).pgd)
=======

static inline unsigned long pte_val(pte_t pte)
{
	return pte.pte;
}

static inline unsigned long pmd_val(pmd_t pmd)
{
	return pmd.pmd;
}

static inline unsigned long pud_val(pud_t pud)
{
	return pud.pud;
}

static inline unsigned long p4d_val(p4d_t p4d)
{
	return p4d.p4d;
}

static inline unsigned long pgd_val(pgd_t pgd)
{
	return pgd.pgd;
}
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

#define __pgste(x)	((pgste_t) { (x) } )
#define __pte(x)        ((pte_t) { (x) } )
#define __pmd(x)        ((pmd_t) { (x) } )
#define __pud(x)	((pud_t) { (x) } )
<<<<<<< HEAD
=======
#define __p4d(x)	((p4d_t) { (x) } )
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#define __pgd(x)        ((pgd_t) { (x) } )
#define __pgprot(x)     ((pgprot_t) { (x) } )

static inline void page_set_storage_key(unsigned long addr,
					unsigned char skey, int mapped)
{
	if (!mapped)
		asm volatile(".insn rrf,0xb22b0000,%0,%1,8,0"
			     : : "d" (skey), "a" (addr));
	else
		asm volatile("sske %0,%1" : : "d" (skey), "a" (addr));
}

static inline unsigned char page_get_storage_key(unsigned long addr)
{
	unsigned char skey;

	asm volatile("iske %0,%1" : "=d" (skey) : "a" (addr));
	return skey;
}

static inline int page_reset_referenced(unsigned long addr)
{
<<<<<<< HEAD
	unsigned int ipm;
=======
	int cc;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	asm volatile(
		"	rrbe	0,%1\n"
		"	ipm	%0\n"
<<<<<<< HEAD
		: "=d" (ipm) : "a" (addr) : "cc");
	return !!(ipm & 0x20000000);
=======
		"	srl	%0,28\n"
		: "=d" (cc) : "a" (addr) : "cc");
	return cc;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

/* Bits int the storage key */
#define _PAGE_CHANGED		0x02	/* HW changed bit		*/
#define _PAGE_REFERENCED	0x04	/* HW referenced bit		*/
#define _PAGE_FP_BIT		0x08	/* HW fetch protection bit	*/
#define _PAGE_ACC_BITS		0xf0	/* HW access control bits	*/

<<<<<<< HEAD
/*
 * Test and clear dirty bit in storage key.
 * We can't clear the changed bit atomically. This is a potential
 * race against modification of the referenced bit. This function
 * should therefore only be called if it is not mapped in any
 * address space.
 */
#define __HAVE_ARCH_PAGE_TEST_AND_CLEAR_DIRTY
static inline int page_test_and_clear_dirty(unsigned long pfn, int mapped)
{
	unsigned char skey;

	skey = page_get_storage_key(pfn << PAGE_SHIFT);
	if (!(skey & _PAGE_CHANGED))
		return 0;
	page_set_storage_key(pfn << PAGE_SHIFT, skey & ~_PAGE_CHANGED, mapped);
	return 1;
}

/*
 * Test and clear referenced bit in storage key.
 */
#define __HAVE_ARCH_PAGE_TEST_AND_CLEAR_YOUNG
static inline int page_test_and_clear_young(unsigned long pfn)
{
	return page_reset_referenced(pfn << PAGE_SHIFT);
}

struct page;
void arch_free_page(struct page *page, int order);
void arch_alloc_page(struct page *page, int order);
void arch_set_page_states(int make_stable);
=======
struct page;
void arch_free_page(struct page *page, int order);
void arch_alloc_page(struct page *page, int order);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

static inline int devmem_is_allowed(unsigned long pfn)
{
	return 0;
}

#define HAVE_ARCH_FREE_PAGE
#define HAVE_ARCH_ALLOC_PAGE

<<<<<<< HEAD
#endif /* !__ASSEMBLY__ */

#define __PAGE_OFFSET           0x0UL
#define PAGE_OFFSET             0x0UL
#define __pa(x)                 (unsigned long)(x)
#define __va(x)                 (void *)(unsigned long)(x)
#define virt_to_page(kaddr)	pfn_to_page(__pa(kaddr) >> PAGE_SHIFT)
#define page_to_phys(page)	(page_to_pfn(page) << PAGE_SHIFT)
#define virt_addr_valid(kaddr)	pfn_valid(__pa(kaddr) >> PAGE_SHIFT)

#define VM_DATA_DEFAULT_FLAGS	(VM_READ | VM_WRITE | \
				 VM_MAYREAD | VM_MAYWRITE | VM_MAYEXEC)

#include <asm-generic/memory_model.h>
#include <asm-generic/getorder.h>

#define __HAVE_ARCH_GATE_AREA 1

=======
#if IS_ENABLED(CONFIG_PGSTE)
int arch_make_page_accessible(struct page *page);
#define HAVE_ARCH_MAKE_PAGE_ACCESSIBLE
#endif

#define __PAGE_OFFSET		0x0UL
#define PAGE_OFFSET		0x0UL

#define __pa_nodebug(x)		((unsigned long)(x))

#ifdef __DECOMPRESSOR

#define __pa(x)			__pa_nodebug(x)
#define __pa32(x)		__pa(x)
#define __va(x)			((void *)(unsigned long)(x))

#else /* __DECOMPRESSOR */

#ifdef CONFIG_DEBUG_VIRTUAL

unsigned long __phys_addr(unsigned long x, bool is_31bit);

#else /* CONFIG_DEBUG_VIRTUAL */

static inline unsigned long __phys_addr(unsigned long x, bool is_31bit)
{
	return __pa_nodebug(x);
}

#endif /* CONFIG_DEBUG_VIRTUAL */

#define __pa(x)			__phys_addr((unsigned long)(x), false)
#define __pa32(x)		__phys_addr((unsigned long)(x), true)
#define __va(x)			((void *)(unsigned long)(x))

#endif /* __DECOMPRESSOR */

#define phys_to_pfn(phys)	((phys) >> PAGE_SHIFT)
#define pfn_to_phys(pfn)	((pfn) << PAGE_SHIFT)

#define phys_to_page(phys)	pfn_to_page(phys_to_pfn(phys))
#define page_to_phys(page)	pfn_to_phys(page_to_pfn(page))

static inline void *pfn_to_virt(unsigned long pfn)
{
	return __va(pfn_to_phys(pfn));
}

static inline unsigned long virt_to_pfn(const void *kaddr)
{
	return phys_to_pfn(__pa(kaddr));
}

#define pfn_to_kaddr(pfn)	pfn_to_virt(pfn)

#define virt_to_page(kaddr)	pfn_to_page(virt_to_pfn(kaddr))
#define page_to_virt(page)	pfn_to_virt(page_to_pfn(page))

#define virt_addr_valid(kaddr)	pfn_valid(phys_to_pfn(__pa_nodebug(kaddr)))

#define VM_DATA_DEFAULT_FLAGS	VM_DATA_FLAGS_NON_EXEC

#endif /* !__ASSEMBLY__ */

#include <asm-generic/memory_model.h>
#include <asm-generic/getorder.h>

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#endif /* _S390_PAGE_H */
