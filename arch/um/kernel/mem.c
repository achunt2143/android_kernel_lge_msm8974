<<<<<<< HEAD
/*
 * Copyright (C) 2000 - 2007 Jeff Dike (jdike@{addtoit,linux.intel}.com)
 * Licensed under the GPL
=======
// SPDX-License-Identifier: GPL-2.0
/*
 * Copyright (C) 2000 - 2007 Jeff Dike (jdike@{addtoit,linux.intel}.com)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 */

#include <linux/stddef.h>
#include <linux/module.h>
<<<<<<< HEAD
#include <linux/bootmem.h>
=======
#include <linux/memblock.h>
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#include <linux/highmem.h>
#include <linux/mm.h>
#include <linux/swap.h>
#include <linux/slab.h>
#include <asm/fixmap.h>
#include <asm/page.h>
<<<<<<< HEAD
#include "as-layout.h"
#include "init.h"
#include "kern.h"
#include "kern_util.h"
#include "mem_user.h"
#include "os.h"
=======
#include <as-layout.h>
#include <init.h>
#include <kern.h>
#include <kern_util.h>
#include <mem_user.h>
#include <os.h>
#include <linux/sched/task.h>

#ifdef CONFIG_KASAN
int kasan_um_is_ready;
void kasan_init(void)
{
	/*
	 * kasan_map_memory will map all of the required address space and
	 * the host machine will allocate physical memory as necessary.
	 */
	kasan_map_memory((void *)KASAN_SHADOW_START, KASAN_SHADOW_SIZE);
	init_task.kasan_depth = 0;
	kasan_um_is_ready = true;
}

static void (*kasan_init_ptr)(void)
__section(".kasan_init") __used
= kasan_init;
#endif
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

/* allocated in paging_init, zeroed in mem_init, and unchanged thereafter */
unsigned long *empty_zero_page = NULL;
EXPORT_SYMBOL(empty_zero_page);
<<<<<<< HEAD
/* allocated in paging_init and unchanged thereafter */
static unsigned long *empty_bad_page = NULL;
=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

/*
 * Initialized during boot, and readonly for initializing page tables
 * afterwards
 */
pgd_t swapper_pg_dir[PTRS_PER_PGD];

/* Initialized at boot time, and readonly after that */
unsigned long long highmem;
<<<<<<< HEAD
=======
EXPORT_SYMBOL(highmem);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
int kmalloc_ok = 0;

/* Used during early boot */
static unsigned long brk_end;

<<<<<<< HEAD
#ifdef CONFIG_HIGHMEM
static void setup_highmem(unsigned long highmem_start,
			  unsigned long highmem_len)
{
	struct page *page;
	unsigned long highmem_pfn;
	int i;

	highmem_pfn = __pa(highmem_start) >> PAGE_SHIFT;
	for (i = 0; i < highmem_len >> PAGE_SHIFT; i++) {
		page = &mem_map[highmem_pfn + i];
		ClearPageReserved(page);
		init_page_count(page);
		__free_page(page);
	}
}
#endif

=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
void __init mem_init(void)
{
	/* clear the zero-page */
	memset(empty_zero_page, 0, PAGE_SIZE);

	/* Map in the area just after the brk now that kmalloc is about
	 * to be turned on.
	 */
	brk_end = (unsigned long) UML_ROUND_UP(sbrk(0));
	map_memory(brk_end, __pa(brk_end), uml_reserved - brk_end, 1, 1, 0);
<<<<<<< HEAD
	free_bootmem(__pa(brk_end), uml_reserved - brk_end);
	uml_reserved = brk_end;

	/* this will put all low memory onto the freelists */
	totalram_pages = free_all_bootmem();
	max_low_pfn = totalram_pages;
#ifdef CONFIG_HIGHMEM
	totalhigh_pages = highmem >> PAGE_SHIFT;
	totalram_pages += totalhigh_pages;
#endif
	num_physpages = totalram_pages;
	max_pfn = totalram_pages;
	printk(KERN_INFO "Memory: %luk available\n",
	       nr_free_pages() << (PAGE_SHIFT-10));
	kmalloc_ok = 1;

#ifdef CONFIG_HIGHMEM
	setup_highmem(end_iomem, highmem);
#endif
=======
	memblock_free((void *)brk_end, uml_reserved - brk_end);
	uml_reserved = brk_end;

	/* this will put all low memory onto the freelists */
	memblock_free_all();
	max_low_pfn = totalram_pages();
	max_pfn = max_low_pfn;
	kmalloc_ok = 1;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

/*
 * Create a page table and place a pointer to it in a middle page
 * directory entry.
 */
static void __init one_page_table_init(pmd_t *pmd)
{
	if (pmd_none(*pmd)) {
<<<<<<< HEAD
		pte_t *pte = (pte_t *) alloc_bootmem_low_pages(PAGE_SIZE);
		set_pmd(pmd, __pmd(_KERNPG_TABLE +
					   (unsigned long) __pa(pte)));
		if (pte != pte_offset_kernel(pmd, 0))
			BUG();
=======
		pte_t *pte = (pte_t *) memblock_alloc_low(PAGE_SIZE,
							  PAGE_SIZE);
		if (!pte)
			panic("%s: Failed to allocate %lu bytes align=%lx\n",
			      __func__, PAGE_SIZE, PAGE_SIZE);

		set_pmd(pmd, __pmd(_KERNPG_TABLE +
					   (unsigned long) __pa(pte)));
		BUG_ON(pte != pte_offset_kernel(pmd, 0));
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	}
}

static void __init one_md_table_init(pud_t *pud)
{
#ifdef CONFIG_3_LEVEL_PGTABLES
<<<<<<< HEAD
	pmd_t *pmd_table = (pmd_t *) alloc_bootmem_low_pages(PAGE_SIZE);
	set_pud(pud, __pud(_KERNPG_TABLE + (unsigned long) __pa(pmd_table)));
	if (pmd_table != pmd_offset(pud, 0))
		BUG();
=======
	pmd_t *pmd_table = (pmd_t *) memblock_alloc_low(PAGE_SIZE, PAGE_SIZE);
	if (!pmd_table)
		panic("%s: Failed to allocate %lu bytes align=%lx\n",
		      __func__, PAGE_SIZE, PAGE_SIZE);

	set_pud(pud, __pud(_KERNPG_TABLE + (unsigned long) __pa(pmd_table)));
	BUG_ON(pmd_table != pmd_offset(pud, 0));
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#endif
}

static void __init fixrange_init(unsigned long start, unsigned long end,
				 pgd_t *pgd_base)
{
	pgd_t *pgd;
<<<<<<< HEAD
=======
	p4d_t *p4d;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	pud_t *pud;
	pmd_t *pmd;
	int i, j;
	unsigned long vaddr;

	vaddr = start;
	i = pgd_index(vaddr);
	j = pmd_index(vaddr);
	pgd = pgd_base + i;

	for ( ; (i < PTRS_PER_PGD) && (vaddr < end); pgd++, i++) {
<<<<<<< HEAD
		pud = pud_offset(pgd, vaddr);
=======
		p4d = p4d_offset(pgd, vaddr);
		pud = pud_offset(p4d, vaddr);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		if (pud_none(*pud))
			one_md_table_init(pud);
		pmd = pmd_offset(pud, vaddr);
		for (; (j < PTRS_PER_PMD) && (vaddr < end); pmd++, j++) {
			one_page_table_init(pmd);
			vaddr += PMD_SIZE;
		}
		j = 0;
	}
}

<<<<<<< HEAD
#ifdef CONFIG_HIGHMEM
pte_t *kmap_pte;
pgprot_t kmap_prot;

#define kmap_get_fixmap_pte(vaddr)					\
	pte_offset_kernel(pmd_offset(pud_offset(pgd_offset_k(vaddr), (vaddr)),\
				     (vaddr)), (vaddr))

static void __init kmap_init(void)
{
	unsigned long kmap_vstart;

	/* cache the first kmap pte */
	kmap_vstart = __fix_to_virt(FIX_KMAP_BEGIN);
	kmap_pte = kmap_get_fixmap_pte(kmap_vstart);

	kmap_prot = PAGE_KERNEL;
}

static void __init init_highmem(void)
{
	pgd_t *pgd;
	pud_t *pud;
	pmd_t *pmd;
	pte_t *pte;
	unsigned long vaddr;

	/*
	 * Permanent kmaps:
	 */
	vaddr = PKMAP_BASE;
	fixrange_init(vaddr, vaddr + PAGE_SIZE*LAST_PKMAP, swapper_pg_dir);

	pgd = swapper_pg_dir + pgd_index(vaddr);
	pud = pud_offset(pgd, vaddr);
	pmd = pmd_offset(pud, vaddr);
	pte = pte_offset_kernel(pmd, vaddr);
	pkmap_page_table = pte;

	kmap_init();
}
#endif /* CONFIG_HIGHMEM */

=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
static void __init fixaddr_user_init( void)
{
#ifdef CONFIG_ARCH_REUSE_HOST_VSYSCALL_AREA
	long size = FIXADDR_USER_END - FIXADDR_USER_START;
<<<<<<< HEAD
	pgd_t *pgd;
	pud_t *pud;
	pmd_t *pmd;
=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	pte_t *pte;
	phys_t p;
	unsigned long v, vaddr = FIXADDR_USER_START;

	if (!size)
		return;

	fixrange_init( FIXADDR_USER_START, FIXADDR_USER_END, swapper_pg_dir);
<<<<<<< HEAD
	v = (unsigned long) alloc_bootmem_low_pages(size);
=======
	v = (unsigned long) memblock_alloc_low(size, PAGE_SIZE);
	if (!v)
		panic("%s: Failed to allocate %lu bytes align=%lx\n",
		      __func__, size, PAGE_SIZE);

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	memcpy((void *) v , (void *) FIXADDR_USER_START, size);
	p = __pa(v);
	for ( ; size > 0; size -= PAGE_SIZE, vaddr += PAGE_SIZE,
		      p += PAGE_SIZE) {
<<<<<<< HEAD
		pgd = swapper_pg_dir + pgd_index(vaddr);
		pud = pud_offset(pgd, vaddr);
		pmd = pmd_offset(pud, vaddr);
		pte = pte_offset_kernel(pmd, vaddr);
=======
		pte = virt_to_kpte(vaddr);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		pte_set_val(*pte, p, PAGE_READONLY);
	}
#endif
}

void __init paging_init(void)
{
<<<<<<< HEAD
	unsigned long zones_size[MAX_NR_ZONES], vaddr;
	int i;

	empty_zero_page = (unsigned long *) alloc_bootmem_low_pages(PAGE_SIZE);
	empty_bad_page = (unsigned long *) alloc_bootmem_low_pages(PAGE_SIZE);
	for (i = 0; i < ARRAY_SIZE(zones_size); i++)
		zones_size[i] = 0;

	zones_size[ZONE_NORMAL] = (end_iomem >> PAGE_SHIFT) -
		(uml_physmem >> PAGE_SHIFT);
#ifdef CONFIG_HIGHMEM
	zones_size[ZONE_HIGHMEM] = highmem >> PAGE_SHIFT;
#endif
	free_area_init(zones_size);
=======
	unsigned long max_zone_pfn[MAX_NR_ZONES] = { 0 };
	unsigned long vaddr;

	empty_zero_page = (unsigned long *) memblock_alloc_low(PAGE_SIZE,
							       PAGE_SIZE);
	if (!empty_zero_page)
		panic("%s: Failed to allocate %lu bytes align=%lx\n",
		      __func__, PAGE_SIZE, PAGE_SIZE);

	max_zone_pfn[ZONE_NORMAL] = end_iomem >> PAGE_SHIFT;
	free_area_init(max_zone_pfn);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	/*
	 * Fixed mappings, only the page table structure has to be
	 * created - mappings will be set by set_fixmap():
	 */
	vaddr = __fix_to_virt(__end_of_fixed_addresses - 1) & PMD_MASK;
	fixrange_init(vaddr, FIXADDR_TOP, swapper_pg_dir);

	fixaddr_user_init();
<<<<<<< HEAD

#ifdef CONFIG_HIGHMEM
	init_highmem();
#endif
=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

/*
 * This can't do anything because nothing in the kernel image can be freed
 * since it's not in kernel physical memory.
 */

void free_initmem(void)
{
}

<<<<<<< HEAD
#ifdef CONFIG_BLK_DEV_INITRD
void free_initrd_mem(unsigned long start, unsigned long end)
{
	if (start < end)
		printk(KERN_INFO "Freeing initrd memory: %ldk freed\n",
		       (end - start) >> 10);
	for (; start < end; start += PAGE_SIZE) {
		ClearPageReserved(virt_to_page(start));
		init_page_count(virt_to_page(start));
		free_page(start);
		totalram_pages++;
	}
}
#endif

=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
/* Allocate and free page tables. */

pgd_t *pgd_alloc(struct mm_struct *mm)
{
	pgd_t *pgd = (pgd_t *)__get_free_page(GFP_KERNEL);

	if (pgd) {
		memset(pgd, 0, USER_PTRS_PER_PGD * sizeof(pgd_t));
		memcpy(pgd + USER_PTRS_PER_PGD,
		       swapper_pg_dir + USER_PTRS_PER_PGD,
		       (PTRS_PER_PGD - USER_PTRS_PER_PGD) * sizeof(pgd_t));
	}
	return pgd;
}

<<<<<<< HEAD
void pgd_free(struct mm_struct *mm, pgd_t *pgd)
{
	free_page((unsigned long) pgd);
}

pte_t *pte_alloc_one_kernel(struct mm_struct *mm, unsigned long address)
{
	pte_t *pte;

	pte = (pte_t *)__get_free_page(GFP_KERNEL|__GFP_REPEAT|__GFP_ZERO);
	return pte;
}

pgtable_t pte_alloc_one(struct mm_struct *mm, unsigned long address)
{
	struct page *pte;

	pte = alloc_page(GFP_KERNEL|__GFP_REPEAT|__GFP_ZERO);
	if (pte)
		pgtable_page_ctor(pte);
	return pte;
}

#ifdef CONFIG_3_LEVEL_PGTABLES
pmd_t *pmd_alloc_one(struct mm_struct *mm, unsigned long address)
{
	pmd_t *pmd = (pmd_t *) __get_free_page(GFP_KERNEL);

	if (pmd)
		memset(pmd, 0, PAGE_SIZE);

	return pmd;
}
#endif

=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
void *uml_kmalloc(int size, int flags)
{
	return kmalloc(size, flags);
}
<<<<<<< HEAD
=======

static const pgprot_t protection_map[16] = {
	[VM_NONE]					= PAGE_NONE,
	[VM_READ]					= PAGE_READONLY,
	[VM_WRITE]					= PAGE_COPY,
	[VM_WRITE | VM_READ]				= PAGE_COPY,
	[VM_EXEC]					= PAGE_READONLY,
	[VM_EXEC | VM_READ]				= PAGE_READONLY,
	[VM_EXEC | VM_WRITE]				= PAGE_COPY,
	[VM_EXEC | VM_WRITE | VM_READ]			= PAGE_COPY,
	[VM_SHARED]					= PAGE_NONE,
	[VM_SHARED | VM_READ]				= PAGE_READONLY,
	[VM_SHARED | VM_WRITE]				= PAGE_SHARED,
	[VM_SHARED | VM_WRITE | VM_READ]		= PAGE_SHARED,
	[VM_SHARED | VM_EXEC]				= PAGE_READONLY,
	[VM_SHARED | VM_EXEC | VM_READ]			= PAGE_READONLY,
	[VM_SHARED | VM_EXEC | VM_WRITE]		= PAGE_SHARED,
	[VM_SHARED | VM_EXEC | VM_WRITE | VM_READ]	= PAGE_SHARED
};
DECLARE_VM_GET_PAGE_PROT
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
