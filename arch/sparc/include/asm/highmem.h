<<<<<<< HEAD
=======
/* SPDX-License-Identifier: GPL-2.0 */
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
/*
 * highmem.h: virtual kernel memory mappings for high memory
 *
 * Used in CONFIG_HIGHMEM systems for memory pages which
 * are not addressable by direct kernel virtual addresses.
 *
 * Copyright (C) 1999 Gerhard Wichert, Siemens AG
 *		      Gerhard.Wichert@pdb.siemens.de
 *
 *
 * Redesigned the x86 32-bit VM architecture to deal with 
 * up to 16 Terrabyte physical memory. With current x86 CPUs
 * we now support up to 64 Gigabytes physical RAM.
 *
 * Copyright (C) 1999 Ingo Molnar <mingo@redhat.com>
 */

#ifndef _ASM_HIGHMEM_H
#define _ASM_HIGHMEM_H

#ifdef __KERNEL__

#include <linux/interrupt.h>
<<<<<<< HEAD
#include <asm/fixmap.h>
#include <asm/vaddrs.h>
#include <asm/kmap_types.h>
#include <asm/pgtable.h>
=======
#include <linux/pgtable.h>
#include <asm/vaddrs.h>
#include <asm/pgtsrmmu.h>
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

/* declarations for highmem.c */
extern unsigned long highstart_pfn, highend_pfn;

<<<<<<< HEAD
extern pte_t *kmap_pte;
extern pgprot_t kmap_prot;
extern pte_t *pkmap_page_table;

extern void kmap_init(void) __init;

=======
#define kmap_prot __pgprot(SRMMU_ET_PTE | SRMMU_PRIV | SRMMU_CACHE)
extern pte_t *pkmap_page_table;

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
/*
 * Right now we initialize only a single pte table. It can be extended
 * easily, subsequent pte tables have to be allocated in one physical
 * chunk of RAM.  Currently the simplest way to do this is to align the
 * pkmap region on a pagetable boundary (4MB).
 */
#define LAST_PKMAP 1024
#define PKMAP_SIZE (LAST_PKMAP << PAGE_SHIFT)
#define PKMAP_BASE PMD_ALIGN(SRMMU_NOCACHE_VADDR + (SRMMU_MAX_NOCACHE_PAGES << PAGE_SHIFT))

#define LAST_PKMAP_MASK (LAST_PKMAP - 1)
#define PKMAP_NR(virt)  ((virt - PKMAP_BASE) >> PAGE_SHIFT)
#define PKMAP_ADDR(nr)  (PKMAP_BASE + ((nr) << PAGE_SHIFT))

#define PKMAP_END (PKMAP_ADDR(LAST_PKMAP))

<<<<<<< HEAD
extern void *kmap_high(struct page *page);
extern void kunmap_high(struct page *page);

static inline void *kmap(struct page *page)
{
	BUG_ON(in_interrupt());
	if (!PageHighMem(page))
		return page_address(page);
	return kmap_high(page);
}

static inline void kunmap(struct page *page)
{
	BUG_ON(in_interrupt());
	if (!PageHighMem(page))
		return;
	kunmap_high(page);
}

extern void *kmap_atomic(struct page *page);
extern void __kunmap_atomic(void *kvaddr);
extern struct page *kmap_atomic_to_page(void *vaddr);

#define flush_cache_kmaps()	flush_cache_all()

=======
#define flush_cache_kmaps()	flush_cache_all()

/* FIXME: Use __flush_*_one(vaddr) instead of flush_*_all() -- Anton */
#define arch_kmap_local_pre_map(vaddr, pteval)	flush_cache_all()
#define arch_kmap_local_pre_unmap(vaddr)	flush_cache_all()
#define arch_kmap_local_post_map(vaddr, pteval)	flush_tlb_all()
#define arch_kmap_local_post_unmap(vaddr)	flush_tlb_all()

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#endif /* __KERNEL__ */

#endif /* _ASM_HIGHMEM_H */
