<<<<<<< HEAD
=======
// SPDX-License-Identifier: GPL-2.0
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
/*
 * Virtual Memory Map support
 *
 * (C) 2007 sgi. Christoph Lameter.
 *
 * Virtual memory maps allow VM primitives pfn_to_page, page_to_pfn,
 * virt_to_page, page_address() to be implemented as a base offset
 * calculation without memory access.
 *
 * However, virtual mappings need a page table and TLBs. Many Linux
 * architectures already map their physical space using 1-1 mappings
 * via TLBs. For those arches the virtual memory map is essentially
 * for free if we use the same page size as the 1-1 mappings. In that
 * case the overhead consists of a few additional pages that are
 * allocated to create a view of memory for vmemmap.
 *
 * The architecture is expected to provide a vmemmap_populate() function
 * to instantiate the mapping.
 */
#include <linux/mm.h>
#include <linux/mmzone.h>
<<<<<<< HEAD
#include <linux/bootmem.h>
=======
#include <linux/memblock.h>
#include <linux/memremap.h>
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#include <linux/highmem.h>
#include <linux/slab.h>
#include <linux/spinlock.h>
#include <linux/vmalloc.h>
#include <linux/sched.h>
<<<<<<< HEAD
#include <asm/dma.h>
#include <asm/pgalloc.h>
#include <asm/pgtable.h>
=======

#include <asm/dma.h>
#include <asm/pgalloc.h>
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

/*
 * Allocate a block of memory to be used to back the virtual memory map
 * or to back the page tables that are used to create the mapping.
 * Uses the main allocators if they are available, else bootmem.
 */

<<<<<<< HEAD
static void * __init_refok __earlyonly_bootmem_alloc(int node,
=======
static void * __ref __earlyonly_bootmem_alloc(int node,
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
				unsigned long size,
				unsigned long align,
				unsigned long goal)
{
<<<<<<< HEAD
	return __alloc_bootmem_node_high(NODE_DATA(node), size, align, goal);
}

static void *vmemmap_buf;
static void *vmemmap_buf_end;

=======
	return memblock_alloc_try_nid_raw(size, align, goal,
					       MEMBLOCK_ALLOC_ACCESSIBLE, node);
}

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
void * __meminit vmemmap_alloc_block(unsigned long size, int node)
{
	/* If the main allocator is up use that, fallback to bootmem. */
	if (slab_is_available()) {
<<<<<<< HEAD
		struct page *page;

		if (node_state(node, N_HIGH_MEMORY))
			page = alloc_pages_node(node,
				GFP_KERNEL | __GFP_ZERO, get_order(size));
		else
			page = alloc_pages(GFP_KERNEL | __GFP_ZERO,
				get_order(size));
		if (page)
			return page_address(page);
=======
		gfp_t gfp_mask = GFP_KERNEL|__GFP_RETRY_MAYFAIL|__GFP_NOWARN;
		int order = get_order(size);
		static bool warned;
		struct page *page;

		page = alloc_pages_node(node, gfp_mask, order);
		if (page)
			return page_address(page);

		if (!warned) {
			warn_alloc(gfp_mask & ~__GFP_NOWARN, NULL,
				   "vmemmap alloc failure: order:%u", order);
			warned = true;
		}
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		return NULL;
	} else
		return __earlyonly_bootmem_alloc(node, size, size,
				__pa(MAX_DMA_ADDRESS));
}

<<<<<<< HEAD
/* need to make sure size is all the same during early stage */
void * __meminit vmemmap_alloc_block_buf(unsigned long size, int node)
{
	void *ptr;

	if (!vmemmap_buf)
		return vmemmap_alloc_block(size, node);

	/* take the from buf */
	ptr = (void *)ALIGN((unsigned long)vmemmap_buf, size);
	if (ptr + size > vmemmap_buf_end)
		return vmemmap_alloc_block(size, node);

	vmemmap_buf = ptr + size;

	return ptr;
}

void __meminit vmemmap_verify(pte_t *pte, int node,
				unsigned long start, unsigned long end)
{
	unsigned long pfn = pte_pfn(*pte);
	int actual_node = early_pfn_to_nid(pfn);

	if (node_distance(actual_node, node) > LOCAL_DISTANCE)
		printk(KERN_WARNING "[%lx-%lx] potential offnode "
			"page_structs\n", start, end - 1);
}

pte_t * __meminit vmemmap_pte_populate(pmd_t *pmd, unsigned long addr, int node)
{
	pte_t *pte = pte_offset_kernel(pmd, addr);
	if (pte_none(*pte)) {
		pte_t entry;
		void *p = vmemmap_alloc_block_buf(PAGE_SIZE, node);
		if (!p)
			return NULL;
=======
static void * __meminit altmap_alloc_block_buf(unsigned long size,
					       struct vmem_altmap *altmap);

/* need to make sure size is all the same during early stage */
void * __meminit vmemmap_alloc_block_buf(unsigned long size, int node,
					 struct vmem_altmap *altmap)
{
	void *ptr;

	if (altmap)
		return altmap_alloc_block_buf(size, altmap);

	ptr = sparse_buffer_alloc(size);
	if (!ptr)
		ptr = vmemmap_alloc_block(size, node);
	return ptr;
}

static unsigned long __meminit vmem_altmap_next_pfn(struct vmem_altmap *altmap)
{
	return altmap->base_pfn + altmap->reserve + altmap->alloc
		+ altmap->align;
}

static unsigned long __meminit vmem_altmap_nr_free(struct vmem_altmap *altmap)
{
	unsigned long allocated = altmap->alloc + altmap->align;

	if (altmap->free > allocated)
		return altmap->free - allocated;
	return 0;
}

static void * __meminit altmap_alloc_block_buf(unsigned long size,
					       struct vmem_altmap *altmap)
{
	unsigned long pfn, nr_pfns, nr_align;

	if (size & ~PAGE_MASK) {
		pr_warn_once("%s: allocations must be multiple of PAGE_SIZE (%ld)\n",
				__func__, size);
		return NULL;
	}

	pfn = vmem_altmap_next_pfn(altmap);
	nr_pfns = size >> PAGE_SHIFT;
	nr_align = 1UL << find_first_bit(&nr_pfns, BITS_PER_LONG);
	nr_align = ALIGN(pfn, nr_align) - pfn;
	if (nr_pfns + nr_align > vmem_altmap_nr_free(altmap))
		return NULL;

	altmap->alloc += nr_pfns;
	altmap->align += nr_align;
	pfn += nr_align;

	pr_debug("%s: pfn: %#lx alloc: %ld align: %ld nr: %#lx\n",
			__func__, pfn, altmap->alloc, altmap->align, nr_pfns);
	return __va(__pfn_to_phys(pfn));
}

void __meminit vmemmap_verify(pte_t *pte, int node,
				unsigned long start, unsigned long end)
{
	unsigned long pfn = pte_pfn(ptep_get(pte));
	int actual_node = early_pfn_to_nid(pfn);

	if (node_distance(actual_node, node) > LOCAL_DISTANCE)
		pr_warn_once("[%lx-%lx] potential offnode page_structs\n",
			start, end - 1);
}

pte_t * __meminit vmemmap_pte_populate(pmd_t *pmd, unsigned long addr, int node,
				       struct vmem_altmap *altmap,
				       struct page *reuse)
{
	pte_t *pte = pte_offset_kernel(pmd, addr);
	if (pte_none(ptep_get(pte))) {
		pte_t entry;
		void *p;

		if (!reuse) {
			p = vmemmap_alloc_block_buf(PAGE_SIZE, node, altmap);
			if (!p)
				return NULL;
		} else {
			/*
			 * When a PTE/PMD entry is freed from the init_mm
			 * there's a free_pages() call to this page allocated
			 * above. Thus this get_page() is paired with the
			 * put_page_testzero() on the freeing path.
			 * This can only called by certain ZONE_DEVICE path,
			 * and through vmemmap_populate_compound_pages() when
			 * slab is available.
			 */
			get_page(reuse);
			p = page_to_virt(reuse);
		}
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		entry = pfn_pte(__pa(p) >> PAGE_SHIFT, PAGE_KERNEL);
		set_pte_at(&init_mm, addr, pte, entry);
	}
	return pte;
}

<<<<<<< HEAD
=======
static void * __meminit vmemmap_alloc_block_zero(unsigned long size, int node)
{
	void *p = vmemmap_alloc_block(size, node);

	if (!p)
		return NULL;
	memset(p, 0, size);

	return p;
}

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
pmd_t * __meminit vmemmap_pmd_populate(pud_t *pud, unsigned long addr, int node)
{
	pmd_t *pmd = pmd_offset(pud, addr);
	if (pmd_none(*pmd)) {
<<<<<<< HEAD
		void *p = vmemmap_alloc_block(PAGE_SIZE, node);
=======
		void *p = vmemmap_alloc_block_zero(PAGE_SIZE, node);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		if (!p)
			return NULL;
		pmd_populate_kernel(&init_mm, pmd, p);
	}
	return pmd;
}

<<<<<<< HEAD
pud_t * __meminit vmemmap_pud_populate(pgd_t *pgd, unsigned long addr, int node)
{
	pud_t *pud = pud_offset(pgd, addr);
	if (pud_none(*pud)) {
		void *p = vmemmap_alloc_block(PAGE_SIZE, node);
		if (!p)
			return NULL;
=======
void __weak __meminit pmd_init(void *addr)
{
}

pud_t * __meminit vmemmap_pud_populate(p4d_t *p4d, unsigned long addr, int node)
{
	pud_t *pud = pud_offset(p4d, addr);
	if (pud_none(*pud)) {
		void *p = vmemmap_alloc_block_zero(PAGE_SIZE, node);
		if (!p)
			return NULL;
		pmd_init(p);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		pud_populate(&init_mm, pud, p);
	}
	return pud;
}

<<<<<<< HEAD
=======
void __weak __meminit pud_init(void *addr)
{
}

p4d_t * __meminit vmemmap_p4d_populate(pgd_t *pgd, unsigned long addr, int node)
{
	p4d_t *p4d = p4d_offset(pgd, addr);
	if (p4d_none(*p4d)) {
		void *p = vmemmap_alloc_block_zero(PAGE_SIZE, node);
		if (!p)
			return NULL;
		pud_init(p);
		p4d_populate(&init_mm, p4d, p);
	}
	return p4d;
}

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
pgd_t * __meminit vmemmap_pgd_populate(unsigned long addr, int node)
{
	pgd_t *pgd = pgd_offset_k(addr);
	if (pgd_none(*pgd)) {
<<<<<<< HEAD
		void *p = vmemmap_alloc_block(PAGE_SIZE, node);
=======
		void *p = vmemmap_alloc_block_zero(PAGE_SIZE, node);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		if (!p)
			return NULL;
		pgd_populate(&init_mm, pgd, p);
	}
	return pgd;
}

<<<<<<< HEAD
int __meminit vmemmap_populate_basepages(struct page *start_page,
						unsigned long size, int node)
{
	unsigned long addr = (unsigned long)start_page;
	unsigned long end = (unsigned long)(start_page + size);
	pgd_t *pgd;
=======
static pte_t * __meminit vmemmap_populate_address(unsigned long addr, int node,
					      struct vmem_altmap *altmap,
					      struct page *reuse)
{
	pgd_t *pgd;
	p4d_t *p4d;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	pud_t *pud;
	pmd_t *pmd;
	pte_t *pte;

<<<<<<< HEAD
	for (; addr < end; addr += PAGE_SIZE) {
		pgd = vmemmap_pgd_populate(addr, node);
		if (!pgd)
			return -ENOMEM;
		pud = vmemmap_pud_populate(pgd, addr, node);
		if (!pud)
			return -ENOMEM;
		pmd = vmemmap_pmd_populate(pud, addr, node);
		if (!pmd)
			return -ENOMEM;
		pte = vmemmap_pte_populate(pmd, addr, node);
		if (!pte)
			return -ENOMEM;
		vmemmap_verify(pte, node, addr, addr + PAGE_SIZE);
=======
	pgd = vmemmap_pgd_populate(addr, node);
	if (!pgd)
		return NULL;
	p4d = vmemmap_p4d_populate(pgd, addr, node);
	if (!p4d)
		return NULL;
	pud = vmemmap_pud_populate(p4d, addr, node);
	if (!pud)
		return NULL;
	pmd = vmemmap_pmd_populate(pud, addr, node);
	if (!pmd)
		return NULL;
	pte = vmemmap_pte_populate(pmd, addr, node, altmap, reuse);
	if (!pte)
		return NULL;
	vmemmap_verify(pte, node, addr, addr + PAGE_SIZE);

	return pte;
}

static int __meminit vmemmap_populate_range(unsigned long start,
					    unsigned long end, int node,
					    struct vmem_altmap *altmap,
					    struct page *reuse)
{
	unsigned long addr = start;
	pte_t *pte;

	for (; addr < end; addr += PAGE_SIZE) {
		pte = vmemmap_populate_address(addr, node, altmap, reuse);
		if (!pte)
			return -ENOMEM;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	}

	return 0;
}

<<<<<<< HEAD
struct page * __meminit sparse_mem_map_populate(unsigned long pnum, int nid)
{
	struct page *map = pfn_to_page(pnum * PAGES_PER_SECTION);
	int error = vmemmap_populate(map, PAGES_PER_SECTION, nid);
	if (error)
		return NULL;

	return map;
}

void __init sparse_mem_maps_populate_node(struct page **map_map,
					  unsigned long pnum_begin,
					  unsigned long pnum_end,
					  unsigned long map_count, int nodeid)
{
	unsigned long pnum;
	unsigned long size = sizeof(struct page) * PAGES_PER_SECTION;
	void *vmemmap_buf_start;

	size = ALIGN(size, PMD_SIZE);
	vmemmap_buf_start = __earlyonly_bootmem_alloc(nodeid, size * map_count,
			 PMD_SIZE, __pa(MAX_DMA_ADDRESS));

	if (vmemmap_buf_start) {
		vmemmap_buf = vmemmap_buf_start;
		vmemmap_buf_end = vmemmap_buf_start + size * map_count;
	}

	for (pnum = pnum_begin; pnum < pnum_end; pnum++) {
		struct mem_section *ms;

		if (!present_section_nr(pnum))
			continue;

		map_map[pnum] = sparse_mem_map_populate(pnum, nodeid);
		if (map_map[pnum])
			continue;
		ms = __nr_to_section(pnum);
		printk(KERN_ERR "%s: sparsemem memory map backing failed "
			"some memory will not be available.\n", __func__);
		ms->section_mem_map = 0;
	}

	if (vmemmap_buf_start) {
		/* need to free left buf */
		free_bootmem(__pa(vmemmap_buf), vmemmap_buf_end - vmemmap_buf);
		vmemmap_buf = NULL;
		vmemmap_buf_end = NULL;
	}
=======
int __meminit vmemmap_populate_basepages(unsigned long start, unsigned long end,
					 int node, struct vmem_altmap *altmap)
{
	return vmemmap_populate_range(start, end, node, altmap, NULL);
}

void __weak __meminit vmemmap_set_pmd(pmd_t *pmd, void *p, int node,
				      unsigned long addr, unsigned long next)
{
}

int __weak __meminit vmemmap_check_pmd(pmd_t *pmd, int node,
				       unsigned long addr, unsigned long next)
{
	return 0;
}

int __meminit vmemmap_populate_hugepages(unsigned long start, unsigned long end,
					 int node, struct vmem_altmap *altmap)
{
	unsigned long addr;
	unsigned long next;
	pgd_t *pgd;
	p4d_t *p4d;
	pud_t *pud;
	pmd_t *pmd;

	for (addr = start; addr < end; addr = next) {
		next = pmd_addr_end(addr, end);

		pgd = vmemmap_pgd_populate(addr, node);
		if (!pgd)
			return -ENOMEM;

		p4d = vmemmap_p4d_populate(pgd, addr, node);
		if (!p4d)
			return -ENOMEM;

		pud = vmemmap_pud_populate(p4d, addr, node);
		if (!pud)
			return -ENOMEM;

		pmd = pmd_offset(pud, addr);
		if (pmd_none(READ_ONCE(*pmd))) {
			void *p;

			p = vmemmap_alloc_block_buf(PMD_SIZE, node, altmap);
			if (p) {
				vmemmap_set_pmd(pmd, p, node, addr, next);
				continue;
			} else if (altmap) {
				/*
				 * No fallback: In any case we care about, the
				 * altmap should be reasonably sized and aligned
				 * such that vmemmap_alloc_block_buf() will always
				 * succeed. For consistency with the PTE case,
				 * return an error here as failure could indicate
				 * a configuration issue with the size of the altmap.
				 */
				return -ENOMEM;
			}
		} else if (vmemmap_check_pmd(pmd, node, addr, next))
			continue;
		if (vmemmap_populate_basepages(addr, next, node, altmap))
			return -ENOMEM;
	}
	return 0;
}

#ifndef vmemmap_populate_compound_pages
/*
 * For compound pages bigger than section size (e.g. x86 1G compound
 * pages with 2M subsection size) fill the rest of sections as tail
 * pages.
 *
 * Note that memremap_pages() resets @nr_range value and will increment
 * it after each range successful onlining. Thus the value or @nr_range
 * at section memmap populate corresponds to the in-progress range
 * being onlined here.
 */
static bool __meminit reuse_compound_section(unsigned long start_pfn,
					     struct dev_pagemap *pgmap)
{
	unsigned long nr_pages = pgmap_vmemmap_nr(pgmap);
	unsigned long offset = start_pfn -
		PHYS_PFN(pgmap->ranges[pgmap->nr_range].start);

	return !IS_ALIGNED(offset, nr_pages) && nr_pages > PAGES_PER_SUBSECTION;
}

static pte_t * __meminit compound_section_tail_page(unsigned long addr)
{
	pte_t *pte;

	addr -= PAGE_SIZE;

	/*
	 * Assuming sections are populated sequentially, the previous section's
	 * page data can be reused.
	 */
	pte = pte_offset_kernel(pmd_off_k(addr), addr);
	if (!pte)
		return NULL;

	return pte;
}

static int __meminit vmemmap_populate_compound_pages(unsigned long start_pfn,
						     unsigned long start,
						     unsigned long end, int node,
						     struct dev_pagemap *pgmap)
{
	unsigned long size, addr;
	pte_t *pte;
	int rc;

	if (reuse_compound_section(start_pfn, pgmap)) {
		pte = compound_section_tail_page(start);
		if (!pte)
			return -ENOMEM;

		/*
		 * Reuse the page that was populated in the prior iteration
		 * with just tail struct pages.
		 */
		return vmemmap_populate_range(start, end, node, NULL,
					      pte_page(ptep_get(pte)));
	}

	size = min(end - start, pgmap_vmemmap_nr(pgmap) * sizeof(struct page));
	for (addr = start; addr < end; addr += size) {
		unsigned long next, last = addr + size;

		/* Populate the head page vmemmap page */
		pte = vmemmap_populate_address(addr, node, NULL, NULL);
		if (!pte)
			return -ENOMEM;

		/* Populate the tail pages vmemmap page */
		next = addr + PAGE_SIZE;
		pte = vmemmap_populate_address(next, node, NULL, NULL);
		if (!pte)
			return -ENOMEM;

		/*
		 * Reuse the previous page for the rest of tail pages
		 * See layout diagram in Documentation/mm/vmemmap_dedup.rst
		 */
		next += PAGE_SIZE;
		rc = vmemmap_populate_range(next, last, node, NULL,
					    pte_page(ptep_get(pte)));
		if (rc)
			return -ENOMEM;
	}

	return 0;
}

#endif

struct page * __meminit __populate_section_memmap(unsigned long pfn,
		unsigned long nr_pages, int nid, struct vmem_altmap *altmap,
		struct dev_pagemap *pgmap)
{
	unsigned long start = (unsigned long) pfn_to_page(pfn);
	unsigned long end = start + nr_pages * sizeof(struct page);
	int r;

	if (WARN_ON_ONCE(!IS_ALIGNED(pfn, PAGES_PER_SUBSECTION) ||
		!IS_ALIGNED(nr_pages, PAGES_PER_SUBSECTION)))
		return NULL;

	if (vmemmap_can_optimize(altmap, pgmap))
		r = vmemmap_populate_compound_pages(pfn, start, end, nid, pgmap);
	else
		r = vmemmap_populate(start, end, nid, altmap);

	if (r < 0)
		return NULL;

	return pfn_to_page(pfn);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}
