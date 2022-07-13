<<<<<<< HEAD
/*
 *  arch/s390/mm/vmem.c
 *
 *    Copyright IBM Corp. 2006
 *    Author(s): Heiko Carstens <heiko.carstens@de.ibm.com>
 */

#include <linux/bootmem.h>
#include <linux/pfn.h>
#include <linux/mm.h>
#include <linux/module.h>
#include <linux/list.h>
#include <linux/hugetlb.h>
#include <linux/slab.h>
#include <asm/pgalloc.h>
#include <asm/pgtable.h>
#include <asm/setup.h>
#include <asm/tlbflush.h>
#include <asm/sections.h>

static DEFINE_MUTEX(vmem_mutex);

struct memory_segment {
	struct list_head list;
	unsigned long start;
	unsigned long size;
};

static LIST_HEAD(mem_segs);

static void __ref *vmem_alloc_pages(unsigned int order)
{
	if (slab_is_available())
		return (void *)__get_free_pages(GFP_KERNEL, order);
	return alloc_bootmem_pages((1 << order) * PAGE_SIZE);
}

static inline pud_t *vmem_pud_alloc(void)
{
	pud_t *pud = NULL;

#ifdef CONFIG_64BIT
	pud = vmem_alloc_pages(2);
	if (!pud)
		return NULL;
	clear_table((unsigned long *) pud, _REGION3_ENTRY_EMPTY, PAGE_SIZE * 4);
#endif
	return pud;
}

static inline pmd_t *vmem_pmd_alloc(void)
{
	pmd_t *pmd = NULL;

#ifdef CONFIG_64BIT
	pmd = vmem_alloc_pages(2);
	if (!pmd)
		return NULL;
	clear_table((unsigned long *) pmd, _SEGMENT_ENTRY_EMPTY, PAGE_SIZE * 4);
#endif
	return pmd;
}

static pte_t __ref *vmem_pte_alloc(unsigned long address)
{
	pte_t *pte;

	if (slab_is_available())
		pte = (pte_t *) page_table_alloc(&init_mm, address);
	else
		pte = alloc_bootmem(PTRS_PER_PTE * sizeof(pte_t));
	if (!pte)
		return NULL;
	clear_table((unsigned long *) pte, _PAGE_TYPE_EMPTY,
		    PTRS_PER_PTE * sizeof(pte_t));
	return pte;
}

/*
 * Add a physical memory range to the 1:1 mapping.
 */
static int vmem_add_mem(unsigned long start, unsigned long size, int ro)
{
	unsigned long address;
	pgd_t *pg_dir;
	pud_t *pu_dir;
	pmd_t *pm_dir;
	pte_t *pt_dir;
	pte_t  pte;
	int ret = -ENOMEM;

	for (address = start; address < start + size; address += PAGE_SIZE) {
		pg_dir = pgd_offset_k(address);
		if (pgd_none(*pg_dir)) {
			pu_dir = vmem_pud_alloc();
			if (!pu_dir)
				goto out;
			pgd_populate(&init_mm, pg_dir, pu_dir);
		}

		pu_dir = pud_offset(pg_dir, address);
		if (pud_none(*pu_dir)) {
			pm_dir = vmem_pmd_alloc();
			if (!pm_dir)
				goto out;
			pud_populate(&init_mm, pu_dir, pm_dir);
		}

		pte = mk_pte_phys(address, __pgprot(ro ? _PAGE_RO : 0));
		pm_dir = pmd_offset(pu_dir, address);

#ifdef __s390x__
		if (MACHINE_HAS_HPAGE && !(address & ~HPAGE_MASK) &&
		    (address + HPAGE_SIZE <= start + size) &&
		    (address >= HPAGE_SIZE)) {
			pte_val(pte) |= _SEGMENT_ENTRY_LARGE;
			pmd_val(*pm_dir) = pte_val(pte);
			address += HPAGE_SIZE - PAGE_SIZE;
			continue;
		}
#endif
		if (pmd_none(*pm_dir)) {
			pt_dir = vmem_pte_alloc(address);
			if (!pt_dir)
				goto out;
			pmd_populate(&init_mm, pm_dir, pt_dir);
		}

		pt_dir = pte_offset_kernel(pm_dir, address);
		*pt_dir = pte;
	}
	ret = 0;
out:
	flush_tlb_kernel_range(start, start + size);
	return ret;
=======
// SPDX-License-Identifier: GPL-2.0
/*
 *    Copyright IBM Corp. 2006
 */

#include <linux/memory_hotplug.h>
#include <linux/memblock.h>
#include <linux/pfn.h>
#include <linux/mm.h>
#include <linux/init.h>
#include <linux/list.h>
#include <linux/hugetlb.h>
#include <linux/slab.h>
#include <linux/sort.h>
#include <asm/page-states.h>
#include <asm/cacheflush.h>
#include <asm/nospec-branch.h>
#include <asm/ctlreg.h>
#include <asm/pgalloc.h>
#include <asm/setup.h>
#include <asm/tlbflush.h>
#include <asm/sections.h>
#include <asm/set_memory.h>

static DEFINE_MUTEX(vmem_mutex);

static void __ref *vmem_alloc_pages(unsigned int order)
{
	unsigned long size = PAGE_SIZE << order;

	if (slab_is_available())
		return (void *)__get_free_pages(GFP_KERNEL, order);
	return memblock_alloc(size, size);
}

static void vmem_free_pages(unsigned long addr, int order, struct vmem_altmap *altmap)
{
	if (altmap) {
		vmem_altmap_free(altmap, 1 << order);
		return;
	}
	/* We don't expect boot memory to be removed ever. */
	if (!slab_is_available() ||
	    WARN_ON_ONCE(PageReserved(virt_to_page((void *)addr))))
		return;
	free_pages(addr, order);
}

void *vmem_crst_alloc(unsigned long val)
{
	unsigned long *table;

	table = vmem_alloc_pages(CRST_ALLOC_ORDER);
	if (!table)
		return NULL;
	crst_table_init(table, val);
	__arch_set_page_dat(table, 1UL << CRST_ALLOC_ORDER);
	return table;
}

pte_t __ref *vmem_pte_alloc(void)
{
	unsigned long size = PTRS_PER_PTE * sizeof(pte_t);
	pte_t *pte;

	if (slab_is_available())
		pte = (pte_t *) page_table_alloc(&init_mm);
	else
		pte = (pte_t *) memblock_alloc(size, size);
	if (!pte)
		return NULL;
	memset64((u64 *)pte, _PAGE_INVALID, PTRS_PER_PTE);
	__arch_set_page_dat(pte, 1);
	return pte;
}

static void vmem_pte_free(unsigned long *table)
{
	/* We don't expect boot memory to be removed ever. */
	if (!slab_is_available() ||
	    WARN_ON_ONCE(PageReserved(virt_to_page(table))))
		return;
	page_table_free(&init_mm, table);
}

#define PAGE_UNUSED 0xFD

/*
 * The unused vmemmap range, which was not yet memset(PAGE_UNUSED) ranges
 * from unused_sub_pmd_start to next PMD_SIZE boundary.
 */
static unsigned long unused_sub_pmd_start;

static void vmemmap_flush_unused_sub_pmd(void)
{
	if (!unused_sub_pmd_start)
		return;
	memset((void *)unused_sub_pmd_start, PAGE_UNUSED,
	       ALIGN(unused_sub_pmd_start, PMD_SIZE) - unused_sub_pmd_start);
	unused_sub_pmd_start = 0;
}

static void vmemmap_mark_sub_pmd_used(unsigned long start, unsigned long end)
{
	/*
	 * As we expect to add in the same granularity as we remove, it's
	 * sufficient to mark only some piece used to block the memmap page from
	 * getting removed (just in case the memmap never gets initialized,
	 * e.g., because the memory block never gets onlined).
	 */
	memset((void *)start, 0, sizeof(struct page));
}

static void vmemmap_use_sub_pmd(unsigned long start, unsigned long end)
{
	/*
	 * We only optimize if the new used range directly follows the
	 * previously unused range (esp., when populating consecutive sections).
	 */
	if (unused_sub_pmd_start == start) {
		unused_sub_pmd_start = end;
		if (likely(IS_ALIGNED(unused_sub_pmd_start, PMD_SIZE)))
			unused_sub_pmd_start = 0;
		return;
	}
	vmemmap_flush_unused_sub_pmd();
	vmemmap_mark_sub_pmd_used(start, end);
}

static void vmemmap_use_new_sub_pmd(unsigned long start, unsigned long end)
{
	unsigned long page = ALIGN_DOWN(start, PMD_SIZE);

	vmemmap_flush_unused_sub_pmd();

	/* Could be our memmap page is filled with PAGE_UNUSED already ... */
	vmemmap_mark_sub_pmd_used(start, end);

	/* Mark the unused parts of the new memmap page PAGE_UNUSED. */
	if (!IS_ALIGNED(start, PMD_SIZE))
		memset((void *)page, PAGE_UNUSED, start - page);
	/*
	 * We want to avoid memset(PAGE_UNUSED) when populating the vmemmap of
	 * consecutive sections. Remember for the last added PMD the last
	 * unused range in the populated PMD.
	 */
	if (!IS_ALIGNED(end, PMD_SIZE))
		unused_sub_pmd_start = end;
}

/* Returns true if the PMD is completely unused and can be freed. */
static bool vmemmap_unuse_sub_pmd(unsigned long start, unsigned long end)
{
	unsigned long page = ALIGN_DOWN(start, PMD_SIZE);

	vmemmap_flush_unused_sub_pmd();
	memset((void *)start, PAGE_UNUSED, end - start);
	return !memchr_inv((void *)page, PAGE_UNUSED, PMD_SIZE);
}

/* __ref: we'll only call vmemmap_alloc_block() via vmemmap_populate() */
static int __ref modify_pte_table(pmd_t *pmd, unsigned long addr,
				  unsigned long end, bool add, bool direct,
				  struct vmem_altmap *altmap)
{
	unsigned long prot, pages = 0;
	int ret = -ENOMEM;
	pte_t *pte;

	prot = pgprot_val(PAGE_KERNEL);
	if (!MACHINE_HAS_NX)
		prot &= ~_PAGE_NOEXEC;

	pte = pte_offset_kernel(pmd, addr);
	for (; addr < end; addr += PAGE_SIZE, pte++) {
		if (!add) {
			if (pte_none(*pte))
				continue;
			if (!direct)
				vmem_free_pages((unsigned long)pfn_to_virt(pte_pfn(*pte)), get_order(PAGE_SIZE), altmap);
			pte_clear(&init_mm, addr, pte);
		} else if (pte_none(*pte)) {
			if (!direct) {
				void *new_page = vmemmap_alloc_block_buf(PAGE_SIZE, NUMA_NO_NODE, altmap);

				if (!new_page)
					goto out;
				set_pte(pte, __pte(__pa(new_page) | prot));
			} else {
				set_pte(pte, __pte(__pa(addr) | prot));
			}
		} else {
			continue;
		}
		pages++;
	}
	ret = 0;
out:
	if (direct)
		update_page_count(PG_DIRECT_MAP_4K, add ? pages : -pages);
	return ret;
}

static void try_free_pte_table(pmd_t *pmd, unsigned long start)
{
	pte_t *pte;
	int i;

	/* We can safely assume this is fully in 1:1 mapping & vmemmap area */
	pte = pte_offset_kernel(pmd, start);
	for (i = 0; i < PTRS_PER_PTE; i++, pte++) {
		if (!pte_none(*pte))
			return;
	}
	vmem_pte_free((unsigned long *) pmd_deref(*pmd));
	pmd_clear(pmd);
}

/* __ref: we'll only call vmemmap_alloc_block() via vmemmap_populate() */
static int __ref modify_pmd_table(pud_t *pud, unsigned long addr,
				  unsigned long end, bool add, bool direct,
				  struct vmem_altmap *altmap)
{
	unsigned long next, prot, pages = 0;
	int ret = -ENOMEM;
	pmd_t *pmd;
	pte_t *pte;

	prot = pgprot_val(SEGMENT_KERNEL);
	if (!MACHINE_HAS_NX)
		prot &= ~_SEGMENT_ENTRY_NOEXEC;

	pmd = pmd_offset(pud, addr);
	for (; addr < end; addr = next, pmd++) {
		next = pmd_addr_end(addr, end);
		if (!add) {
			if (pmd_none(*pmd))
				continue;
			if (pmd_leaf(*pmd)) {
				if (IS_ALIGNED(addr, PMD_SIZE) &&
				    IS_ALIGNED(next, PMD_SIZE)) {
					if (!direct)
						vmem_free_pages(pmd_deref(*pmd), get_order(PMD_SIZE), altmap);
					pmd_clear(pmd);
					pages++;
				} else if (!direct && vmemmap_unuse_sub_pmd(addr, next)) {
					vmem_free_pages(pmd_deref(*pmd), get_order(PMD_SIZE), altmap);
					pmd_clear(pmd);
				}
				continue;
			}
		} else if (pmd_none(*pmd)) {
			if (IS_ALIGNED(addr, PMD_SIZE) &&
			    IS_ALIGNED(next, PMD_SIZE) &&
			    MACHINE_HAS_EDAT1 && direct &&
			    !debug_pagealloc_enabled()) {
				set_pmd(pmd, __pmd(__pa(addr) | prot));
				pages++;
				continue;
			} else if (!direct && MACHINE_HAS_EDAT1) {
				void *new_page;

				/*
				 * Use 1MB frames for vmemmap if available. We
				 * always use large frames even if they are only
				 * partially used. Otherwise we would have also
				 * page tables since vmemmap_populate gets
				 * called for each section separately.
				 */
				new_page = vmemmap_alloc_block_buf(PMD_SIZE, NUMA_NO_NODE, altmap);
				if (new_page) {
					set_pmd(pmd, __pmd(__pa(new_page) | prot));
					if (!IS_ALIGNED(addr, PMD_SIZE) ||
					    !IS_ALIGNED(next, PMD_SIZE)) {
						vmemmap_use_new_sub_pmd(addr, next);
					}
					continue;
				}
			}
			pte = vmem_pte_alloc();
			if (!pte)
				goto out;
			pmd_populate(&init_mm, pmd, pte);
		} else if (pmd_leaf(*pmd)) {
			if (!direct)
				vmemmap_use_sub_pmd(addr, next);
			continue;
		}
		ret = modify_pte_table(pmd, addr, next, add, direct, altmap);
		if (ret)
			goto out;
		if (!add)
			try_free_pte_table(pmd, addr & PMD_MASK);
	}
	ret = 0;
out:
	if (direct)
		update_page_count(PG_DIRECT_MAP_1M, add ? pages : -pages);
	return ret;
}

static void try_free_pmd_table(pud_t *pud, unsigned long start)
{
	pmd_t *pmd;
	int i;

	pmd = pmd_offset(pud, start);
	for (i = 0; i < PTRS_PER_PMD; i++, pmd++)
		if (!pmd_none(*pmd))
			return;
	vmem_free_pages(pud_deref(*pud), CRST_ALLOC_ORDER, NULL);
	pud_clear(pud);
}

static int modify_pud_table(p4d_t *p4d, unsigned long addr, unsigned long end,
			    bool add, bool direct, struct vmem_altmap *altmap)
{
	unsigned long next, prot, pages = 0;
	int ret = -ENOMEM;
	pud_t *pud;
	pmd_t *pmd;

	prot = pgprot_val(REGION3_KERNEL);
	if (!MACHINE_HAS_NX)
		prot &= ~_REGION_ENTRY_NOEXEC;
	pud = pud_offset(p4d, addr);
	for (; addr < end; addr = next, pud++) {
		next = pud_addr_end(addr, end);
		if (!add) {
			if (pud_none(*pud))
				continue;
			if (pud_leaf(*pud)) {
				if (IS_ALIGNED(addr, PUD_SIZE) &&
				    IS_ALIGNED(next, PUD_SIZE)) {
					pud_clear(pud);
					pages++;
				}
				continue;
			}
		} else if (pud_none(*pud)) {
			if (IS_ALIGNED(addr, PUD_SIZE) &&
			    IS_ALIGNED(next, PUD_SIZE) &&
			    MACHINE_HAS_EDAT2 && direct &&
			    !debug_pagealloc_enabled()) {
				set_pud(pud, __pud(__pa(addr) | prot));
				pages++;
				continue;
			}
			pmd = vmem_crst_alloc(_SEGMENT_ENTRY_EMPTY);
			if (!pmd)
				goto out;
			pud_populate(&init_mm, pud, pmd);
		} else if (pud_leaf(*pud)) {
			continue;
		}
		ret = modify_pmd_table(pud, addr, next, add, direct, altmap);
		if (ret)
			goto out;
		if (!add)
			try_free_pmd_table(pud, addr & PUD_MASK);
	}
	ret = 0;
out:
	if (direct)
		update_page_count(PG_DIRECT_MAP_2G, add ? pages : -pages);
	return ret;
}

static void try_free_pud_table(p4d_t *p4d, unsigned long start)
{
	pud_t *pud;
	int i;

	pud = pud_offset(p4d, start);
	for (i = 0; i < PTRS_PER_PUD; i++, pud++) {
		if (!pud_none(*pud))
			return;
	}
	vmem_free_pages(p4d_deref(*p4d), CRST_ALLOC_ORDER, NULL);
	p4d_clear(p4d);
}

static int modify_p4d_table(pgd_t *pgd, unsigned long addr, unsigned long end,
			    bool add, bool direct, struct vmem_altmap *altmap)
{
	unsigned long next;
	int ret = -ENOMEM;
	p4d_t *p4d;
	pud_t *pud;

	p4d = p4d_offset(pgd, addr);
	for (; addr < end; addr = next, p4d++) {
		next = p4d_addr_end(addr, end);
		if (!add) {
			if (p4d_none(*p4d))
				continue;
		} else if (p4d_none(*p4d)) {
			pud = vmem_crst_alloc(_REGION3_ENTRY_EMPTY);
			if (!pud)
				goto out;
			p4d_populate(&init_mm, p4d, pud);
		}
		ret = modify_pud_table(p4d, addr, next, add, direct, altmap);
		if (ret)
			goto out;
		if (!add)
			try_free_pud_table(p4d, addr & P4D_MASK);
	}
	ret = 0;
out:
	return ret;
}

static void try_free_p4d_table(pgd_t *pgd, unsigned long start)
{
	p4d_t *p4d;
	int i;

	p4d = p4d_offset(pgd, start);
	for (i = 0; i < PTRS_PER_P4D; i++, p4d++) {
		if (!p4d_none(*p4d))
			return;
	}
	vmem_free_pages(pgd_deref(*pgd), CRST_ALLOC_ORDER, NULL);
	pgd_clear(pgd);
}

static int modify_pagetable(unsigned long start, unsigned long end, bool add,
			    bool direct, struct vmem_altmap *altmap)
{
	unsigned long addr, next;
	int ret = -ENOMEM;
	pgd_t *pgd;
	p4d_t *p4d;

	if (WARN_ON_ONCE(!PAGE_ALIGNED(start | end)))
		return -EINVAL;
	/* Don't mess with any tables not fully in 1:1 mapping & vmemmap area */
	if (WARN_ON_ONCE(end > VMALLOC_START))
		return -EINVAL;
	for (addr = start; addr < end; addr = next) {
		next = pgd_addr_end(addr, end);
		pgd = pgd_offset_k(addr);

		if (!add) {
			if (pgd_none(*pgd))
				continue;
		} else if (pgd_none(*pgd)) {
			p4d = vmem_crst_alloc(_REGION2_ENTRY_EMPTY);
			if (!p4d)
				goto out;
			pgd_populate(&init_mm, pgd, p4d);
		}
		ret = modify_p4d_table(pgd, addr, next, add, direct, altmap);
		if (ret)
			goto out;
		if (!add)
			try_free_p4d_table(pgd, addr & PGDIR_MASK);
	}
	ret = 0;
out:
	if (!add)
		flush_tlb_kernel_range(start, end);
	return ret;
}

static int add_pagetable(unsigned long start, unsigned long end, bool direct,
			 struct vmem_altmap *altmap)
{
	return modify_pagetable(start, end, true, direct, altmap);
}

static int remove_pagetable(unsigned long start, unsigned long end, bool direct,
			    struct vmem_altmap *altmap)
{
	return modify_pagetable(start, end, false, direct, altmap);
}

/*
 * Add a physical memory range to the 1:1 mapping.
 */
static int vmem_add_range(unsigned long start, unsigned long size)
{
	start = (unsigned long)__va(start);
	return add_pagetable(start, start + size, true, NULL);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

/*
 * Remove a physical memory range from the 1:1 mapping.
<<<<<<< HEAD
 * Currently only invalidates page table entries.
 */
static void vmem_remove_range(unsigned long start, unsigned long size)
{
	unsigned long address;
	pgd_t *pg_dir;
	pud_t *pu_dir;
	pmd_t *pm_dir;
	pte_t *pt_dir;
	pte_t  pte;

	pte_val(pte) = _PAGE_TYPE_EMPTY;
	for (address = start; address < start + size; address += PAGE_SIZE) {
		pg_dir = pgd_offset_k(address);
		pu_dir = pud_offset(pg_dir, address);
		if (pud_none(*pu_dir))
			continue;
		pm_dir = pmd_offset(pu_dir, address);
		if (pmd_none(*pm_dir))
			continue;

		if (pmd_huge(*pm_dir)) {
			pmd_clear(pm_dir);
			address += HPAGE_SIZE - PAGE_SIZE;
			continue;
		}

		pt_dir = pte_offset_kernel(pm_dir, address);
		*pt_dir = pte;
	}
	flush_tlb_kernel_range(start, start + size);
=======
 */
static void vmem_remove_range(unsigned long start, unsigned long size)
{
	start = (unsigned long)__va(start);
	remove_pagetable(start, start + size, true, NULL);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

/*
 * Add a backed mem_map array to the virtual mem_map array.
 */
<<<<<<< HEAD
int __meminit vmemmap_populate(struct page *start, unsigned long nr, int node)
{
	unsigned long address, start_addr, end_addr;
	pgd_t *pg_dir;
	pud_t *pu_dir;
	pmd_t *pm_dir;
	pte_t *pt_dir;
	pte_t  pte;
	int ret = -ENOMEM;

	start_addr = (unsigned long) start;
	end_addr = (unsigned long) (start + nr);

	for (address = start_addr; address < end_addr; address += PAGE_SIZE) {
		pg_dir = pgd_offset_k(address);
		if (pgd_none(*pg_dir)) {
			pu_dir = vmem_pud_alloc();
			if (!pu_dir)
				goto out;
			pgd_populate(&init_mm, pg_dir, pu_dir);
		}

		pu_dir = pud_offset(pg_dir, address);
		if (pud_none(*pu_dir)) {
			pm_dir = vmem_pmd_alloc();
			if (!pm_dir)
				goto out;
			pud_populate(&init_mm, pu_dir, pm_dir);
		}

		pm_dir = pmd_offset(pu_dir, address);
		if (pmd_none(*pm_dir)) {
			pt_dir = vmem_pte_alloc(address);
			if (!pt_dir)
				goto out;
			pmd_populate(&init_mm, pm_dir, pt_dir);
		}

		pt_dir = pte_offset_kernel(pm_dir, address);
		if (pte_none(*pt_dir)) {
			unsigned long new_page;

			new_page =__pa(vmem_alloc_pages(0));
			if (!new_page)
				goto out;
			pte = pfn_pte(new_page >> PAGE_SHIFT, PAGE_KERNEL);
			*pt_dir = pte;
		}
	}
	memset(start, 0, nr * sizeof(struct page));
	ret = 0;
out:
	flush_tlb_kernel_range(start_addr, end_addr);
	return ret;
}

/*
 * Add memory segment to the segment list if it doesn't overlap with
 * an already present segment.
 */
static int insert_memory_segment(struct memory_segment *seg)
{
	struct memory_segment *tmp;

	if (seg->start + seg->size > VMEM_MAX_PHYS ||
	    seg->start + seg->size < seg->start)
		return -ERANGE;

	list_for_each_entry(tmp, &mem_segs, list) {
		if (seg->start >= tmp->start + tmp->size)
			continue;
		if (seg->start + seg->size <= tmp->start)
			continue;
		return -ENOSPC;
	}
	list_add(&seg->list, &mem_segs);
	return 0;
}

/*
 * Remove memory segment from the segment list.
 */
static void remove_memory_segment(struct memory_segment *seg)
{
	list_del(&seg->list);
}

static void __remove_shared_memory(struct memory_segment *seg)
{
	remove_memory_segment(seg);
	vmem_remove_range(seg->start, seg->size);
}

int vmem_remove_mapping(unsigned long start, unsigned long size)
{
	struct memory_segment *seg;
	int ret;

	mutex_lock(&vmem_mutex);

	ret = -ENOENT;
	list_for_each_entry(seg, &mem_segs, list) {
		if (seg->start == start && seg->size == size)
			break;
	}

	if (seg->start != start || seg->size != size)
		goto out;

	ret = 0;
	__remove_shared_memory(seg);
	kfree(seg);
out:
=======
int __meminit vmemmap_populate(unsigned long start, unsigned long end, int node,
			       struct vmem_altmap *altmap)
{
	int ret;

	mutex_lock(&vmem_mutex);
	/* We don't care about the node, just use NUMA_NO_NODE on allocations */
	ret = add_pagetable(start, end, false, altmap);
	if (ret)
		remove_pagetable(start, end, false, altmap);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	mutex_unlock(&vmem_mutex);
	return ret;
}

<<<<<<< HEAD
int vmem_add_mapping(unsigned long start, unsigned long size)
{
	struct memory_segment *seg;
	int ret;

	mutex_lock(&vmem_mutex);
	ret = -ENOMEM;
	seg = kzalloc(sizeof(*seg), GFP_KERNEL);
	if (!seg)
		goto out;
	seg->start = start;
	seg->size = size;

	ret = insert_memory_segment(seg);
	if (ret)
		goto out_free;

	ret = vmem_add_mem(start, size, 0);
	if (ret)
		goto out_remove;
	goto out;

out_remove:
	__remove_shared_memory(seg);
out_free:
	kfree(seg);
out:
=======
#ifdef CONFIG_MEMORY_HOTPLUG

void vmemmap_free(unsigned long start, unsigned long end,
		  struct vmem_altmap *altmap)
{
	mutex_lock(&vmem_mutex);
	remove_pagetable(start, end, false, altmap);
	mutex_unlock(&vmem_mutex);
}

#endif

void vmem_remove_mapping(unsigned long start, unsigned long size)
{
	mutex_lock(&vmem_mutex);
	vmem_remove_range(start, size);
	mutex_unlock(&vmem_mutex);
}

struct range arch_get_mappable_range(void)
{
	struct range mhp_range;

	mhp_range.start = 0;
	mhp_range.end = max_mappable - 1;
	return mhp_range;
}

int vmem_add_mapping(unsigned long start, unsigned long size)
{
	struct range range = arch_get_mappable_range();
	int ret;

	if (start < range.start ||
	    start + size > range.end + 1 ||
	    start + size < start)
		return -ERANGE;

	mutex_lock(&vmem_mutex);
	ret = vmem_add_range(start, size);
	if (ret)
		vmem_remove_range(start, size);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	mutex_unlock(&vmem_mutex);
	return ret;
}

/*
<<<<<<< HEAD
 * map whole physical memory to virtual memory (identity mapping)
 * we reserve enough space in the vmalloc area for vmemmap to hotplug
 * additional memory segments.
 */
void __init vmem_map_init(void)
{
	unsigned long ro_start, ro_end;
	unsigned long start, end;
	int i;

	ro_start = ((unsigned long)&_stext) & PAGE_MASK;
	ro_end = PFN_ALIGN((unsigned long)&_eshared);
	for (i = 0; i < MEMORY_CHUNKS && memory_chunk[i].size > 0; i++) {
		if (memory_chunk[i].type == CHUNK_CRASHK ||
		    memory_chunk[i].type == CHUNK_OLDMEM)
			continue;
		start = memory_chunk[i].addr;
		end = memory_chunk[i].addr + memory_chunk[i].size;
		if (start >= ro_end || end <= ro_start)
			vmem_add_mem(start, end - start, 0);
		else if (start >= ro_start && end <= ro_end)
			vmem_add_mem(start, end - start, 1);
		else if (start >= ro_start) {
			vmem_add_mem(start, ro_end - start, 1);
			vmem_add_mem(ro_end, end - ro_end, 0);
		} else if (end < ro_end) {
			vmem_add_mem(start, ro_start - start, 0);
			vmem_add_mem(ro_start, end - ro_start, 1);
		} else {
			vmem_add_mem(start, ro_start - start, 0);
			vmem_add_mem(ro_start, ro_end - ro_start, 1);
			vmem_add_mem(ro_end, end - ro_end, 0);
		}
	}
}

/*
 * Convert memory chunk array to a memory segment list so there is a single
 * list that contains both r/w memory and shared memory segments.
 */
static int __init vmem_convert_memory_chunk(void)
{
	struct memory_segment *seg;
	int i;

	mutex_lock(&vmem_mutex);
	for (i = 0; i < MEMORY_CHUNKS; i++) {
		if (!memory_chunk[i].size)
			continue;
		if (memory_chunk[i].type == CHUNK_CRASHK ||
		    memory_chunk[i].type == CHUNK_OLDMEM)
			continue;
		seg = kzalloc(sizeof(*seg), GFP_KERNEL);
		if (!seg)
			panic("Out of memory...\n");
		seg->start = memory_chunk[i].addr;
		seg->size = memory_chunk[i].size;
		insert_memory_segment(seg);
	}
	mutex_unlock(&vmem_mutex);
	return 0;
}

core_initcall(vmem_convert_memory_chunk);
=======
 * Allocate new or return existing page-table entry, but do not map it
 * to any physical address. If missing, allocate segment- and region-
 * table entries along. Meeting a large segment- or region-table entry
 * while traversing is an error, since the function is expected to be
 * called against virtual regions reserved for 4KB mappings only.
 */
pte_t *vmem_get_alloc_pte(unsigned long addr, bool alloc)
{
	pte_t *ptep = NULL;
	pgd_t *pgd;
	p4d_t *p4d;
	pud_t *pud;
	pmd_t *pmd;
	pte_t *pte;

	pgd = pgd_offset_k(addr);
	if (pgd_none(*pgd)) {
		if (!alloc)
			goto out;
		p4d = vmem_crst_alloc(_REGION2_ENTRY_EMPTY);
		if (!p4d)
			goto out;
		pgd_populate(&init_mm, pgd, p4d);
	}
	p4d = p4d_offset(pgd, addr);
	if (p4d_none(*p4d)) {
		if (!alloc)
			goto out;
		pud = vmem_crst_alloc(_REGION3_ENTRY_EMPTY);
		if (!pud)
			goto out;
		p4d_populate(&init_mm, p4d, pud);
	}
	pud = pud_offset(p4d, addr);
	if (pud_none(*pud)) {
		if (!alloc)
			goto out;
		pmd = vmem_crst_alloc(_SEGMENT_ENTRY_EMPTY);
		if (!pmd)
			goto out;
		pud_populate(&init_mm, pud, pmd);
	} else if (WARN_ON_ONCE(pud_leaf(*pud))) {
		goto out;
	}
	pmd = pmd_offset(pud, addr);
	if (pmd_none(*pmd)) {
		if (!alloc)
			goto out;
		pte = vmem_pte_alloc();
		if (!pte)
			goto out;
		pmd_populate(&init_mm, pmd, pte);
	} else if (WARN_ON_ONCE(pmd_leaf(*pmd))) {
		goto out;
	}
	ptep = pte_offset_kernel(pmd, addr);
out:
	return ptep;
}

int __vmem_map_4k_page(unsigned long addr, unsigned long phys, pgprot_t prot, bool alloc)
{
	pte_t *ptep, pte;

	if (!IS_ALIGNED(addr, PAGE_SIZE))
		return -EINVAL;
	ptep = vmem_get_alloc_pte(addr, alloc);
	if (!ptep)
		return -ENOMEM;
	__ptep_ipte(addr, ptep, 0, 0, IPTE_GLOBAL);
	pte = mk_pte_phys(phys, prot);
	set_pte(ptep, pte);
	return 0;
}

int vmem_map_4k_page(unsigned long addr, unsigned long phys, pgprot_t prot)
{
	int rc;

	mutex_lock(&vmem_mutex);
	rc = __vmem_map_4k_page(addr, phys, prot, true);
	mutex_unlock(&vmem_mutex);
	return rc;
}

void vmem_unmap_4k_page(unsigned long addr)
{
	pte_t *ptep;

	mutex_lock(&vmem_mutex);
	ptep = virt_to_kpte(addr);
	__ptep_ipte(addr, ptep, 0, 0, IPTE_GLOBAL);
	pte_clear(&init_mm, addr, ptep);
	mutex_unlock(&vmem_mutex);
}

void __init vmem_map_init(void)
{
	__set_memory_rox(_stext, _etext);
	__set_memory_ro(_etext, __end_rodata);
	__set_memory_rox(_sinittext, _einittext);
	__set_memory_rox(__stext_amode31, __etext_amode31);
	/*
	 * If the BEAR-enhancement facility is not installed the first
	 * prefix page is used to return to the previous context with
	 * an LPSWE instruction and therefore must be executable.
	 */
	if (!static_key_enabled(&cpu_has_bear))
		set_memory_x(0, 1);
	if (debug_pagealloc_enabled()) {
		/*
		 * Use RELOC_HIDE() as long as __va(0) translates to NULL,
		 * since performing pointer arithmetic on a NULL pointer
		 * has undefined behavior and generates compiler warnings.
		 */
		__set_memory_4k(__va(0), RELOC_HIDE(__va(0), ident_map_size));
	}
	if (MACHINE_HAS_NX)
		system_ctl_set_bit(0, CR0_INSTRUCTION_EXEC_PROTECTION_BIT);
	pr_info("Write protected kernel read-only data: %luk\n",
		(unsigned long)(__end_rodata - _stext) >> 10);
}
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
