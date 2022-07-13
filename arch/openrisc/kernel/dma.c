<<<<<<< HEAD
=======
// SPDX-License-Identifier: GPL-2.0-or-later
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
/*
 * OpenRISC Linux
 *
 * Linux architectural port borrowing liberally from similar works of
 * others.  All original copyrights apply as per the original source
 * declaration.
 *
 * Modifications for the OpenRISC architecture:
 * Copyright (C) 2003 Matjaz Breskvar <phoenix@bsemi.com>
 * Copyright (C) 2010-2011 Jonas Bonn <jonas@southpole.se>
 *
<<<<<<< HEAD
 *      This program is free software; you can redistribute it and/or
 *      modify it under the terms of the GNU General Public License
 *      as published by the Free Software Foundation; either version
 *      2 of the License, or (at your option) any later version.
 *
 * DMA mapping callbacks...
 * As alloc_coherent is the only DMA callback being used currently, that's
 * the only thing implemented properly.  The rest need looking into...
 */

#include <linux/dma-mapping.h>
#include <linux/dma-debug.h>
=======
 * DMA mapping callbacks...
 */

#include <linux/dma-map-ops.h>
#include <linux/pagewalk.h>
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

#include <asm/cpuinfo.h>
#include <asm/spr_defs.h>
#include <asm/tlbflush.h>

<<<<<<< HEAD
static int page_set_nocache(pte_t *pte, unsigned long addr,
			    unsigned long next, struct mm_walk *walk)
{
	unsigned long cl;
=======
static int
page_set_nocache(pte_t *pte, unsigned long addr,
		 unsigned long next, struct mm_walk *walk)
{
	unsigned long cl;
	struct cpuinfo_or1k *cpuinfo = &cpuinfo_or1k[smp_processor_id()];
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	pte_val(*pte) |= _PAGE_CI;

	/*
	 * Flush the page out of the TLB so that the new page flags get
	 * picked up next time there's an access
	 */
<<<<<<< HEAD
	flush_tlb_page(NULL, addr);

	/* Flush page out of dcache */
	for (cl = __pa(addr); cl < __pa(next); cl += cpuinfo.dcache_block_size)
=======
	flush_tlb_kernel_range(addr, addr + PAGE_SIZE);

	/* Flush page out of dcache */
	for (cl = __pa(addr); cl < __pa(next); cl += cpuinfo->dcache_block_size)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		mtspr(SPR_DCBFR, cl);

	return 0;
}

<<<<<<< HEAD
static int page_clear_nocache(pte_t *pte, unsigned long addr,
			      unsigned long next, struct mm_walk *walk)
=======
static const struct mm_walk_ops set_nocache_walk_ops = {
	.pte_entry		= page_set_nocache,
};

static int
page_clear_nocache(pte_t *pte, unsigned long addr,
		   unsigned long next, struct mm_walk *walk)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
{
	pte_val(*pte) &= ~_PAGE_CI;

	/*
	 * Flush the page out of the TLB so that the new page flags get
	 * picked up next time there's an access
	 */
<<<<<<< HEAD
	flush_tlb_page(NULL, addr);
=======
	flush_tlb_kernel_range(addr, addr + PAGE_SIZE);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	return 0;
}

<<<<<<< HEAD
/*
 * Alloc "coherent" memory, which for OpenRISC means simply uncached.
 *
 * This function effectively just calls __get_free_pages, sets the
 * cache-inhibit bit on those pages, and makes sure that the pages are
 * flushed out of the cache before they are used.
 *
 */
void *or1k_dma_alloc_coherent(struct device *dev, size_t size,
			      dma_addr_t *dma_handle, gfp_t gfp)
{
	unsigned long va;
	void *page;
	struct mm_walk walk = {
		.pte_entry = page_set_nocache,
		.mm = &init_mm
	};

	page = alloc_pages_exact(size, gfp);
	if (!page)
		return NULL;

	/* This gives us the real physical address of the first page. */
	*dma_handle = __pa(page);

	va = (unsigned long)page;
=======
static const struct mm_walk_ops clear_nocache_walk_ops = {
	.pte_entry		= page_clear_nocache,
};

void *arch_dma_set_uncached(void *cpu_addr, size_t size)
{
	unsigned long va = (unsigned long)cpu_addr;
	int error;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	/*
	 * We need to iterate through the pages, clearing the dcache for
	 * them and setting the cache-inhibit bit.
	 */
<<<<<<< HEAD
	if (walk_page_range(va, va + size, &walk)) {
		free_pages_exact(page, size);
		return NULL;
	}

	return (void *)va;
}

void or1k_dma_free_coherent(struct device *dev, size_t size, void *vaddr,
			    dma_addr_t dma_handle)
{
	unsigned long va = (unsigned long)vaddr;
	struct mm_walk walk = {
		.pte_entry = page_clear_nocache,
		.mm = &init_mm
	};

	/* walk_page_range shouldn't be able to fail here */
	WARN_ON(walk_page_range(va, va + size, &walk));

	free_pages_exact(vaddr, size);
}

dma_addr_t or1k_map_page(struct device *dev, struct page *page,
			 unsigned long offset, size_t size,
			 enum dma_data_direction dir,
			 struct dma_attrs *attrs)
{
	unsigned long cl;
	dma_addr_t addr = page_to_phys(page) + offset;
=======
	mmap_write_lock(&init_mm);
	error = walk_page_range_novma(&init_mm, va, va + size,
			&set_nocache_walk_ops, NULL, NULL);
	mmap_write_unlock(&init_mm);

	if (error)
		return ERR_PTR(error);
	return cpu_addr;
}

void arch_dma_clear_uncached(void *cpu_addr, size_t size)
{
	unsigned long va = (unsigned long)cpu_addr;

	mmap_write_lock(&init_mm);
	/* walk_page_range shouldn't be able to fail here */
	WARN_ON(walk_page_range_novma(&init_mm, va, va + size,
			&clear_nocache_walk_ops, NULL, NULL));
	mmap_write_unlock(&init_mm);
}

void arch_sync_dma_for_device(phys_addr_t addr, size_t size,
		enum dma_data_direction dir)
{
	unsigned long cl;
	struct cpuinfo_or1k *cpuinfo = &cpuinfo_or1k[smp_processor_id()];
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	switch (dir) {
	case DMA_TO_DEVICE:
		/* Flush the dcache for the requested range */
		for (cl = addr; cl < addr + size;
<<<<<<< HEAD
		     cl += cpuinfo.dcache_block_size)
=======
		     cl += cpuinfo->dcache_block_size)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
			mtspr(SPR_DCBFR, cl);
		break;
	case DMA_FROM_DEVICE:
		/* Invalidate the dcache for the requested range */
		for (cl = addr; cl < addr + size;
<<<<<<< HEAD
		     cl += cpuinfo.dcache_block_size)
=======
		     cl += cpuinfo->dcache_block_size)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
			mtspr(SPR_DCBIR, cl);
		break;
	default:
		/*
		 * NOTE: If dir == DMA_BIDIRECTIONAL then there's no need to
		 * flush nor invalidate the cache here as the area will need
		 * to be manually synced anyway.
		 */
		break;
	}
<<<<<<< HEAD

	return addr;
}

void or1k_unmap_page(struct device *dev, dma_addr_t dma_handle,
		     size_t size, enum dma_data_direction dir,
		     struct dma_attrs *attrs)
{
	/* Nothing special to do here... */
}

int or1k_map_sg(struct device *dev, struct scatterlist *sg,
		int nents, enum dma_data_direction dir,
		struct dma_attrs *attrs)
{
	struct scatterlist *s;
	int i;

	for_each_sg(sg, s, nents, i) {
		s->dma_address = or1k_map_page(dev, sg_page(s), s->offset,
					       s->length, dir, NULL);
	}

	return nents;
}

void or1k_unmap_sg(struct device *dev, struct scatterlist *sg,
		   int nents, enum dma_data_direction dir,
		   struct dma_attrs *attrs)
{
	struct scatterlist *s;
	int i;

	for_each_sg(sg, s, nents, i) {
		or1k_unmap_page(dev, sg_dma_address(s), sg_dma_len(s), dir, NULL);
	}
}

void or1k_sync_single_for_cpu(struct device *dev,
			      dma_addr_t dma_handle, size_t size,
			      enum dma_data_direction dir)
{
	unsigned long cl;
	dma_addr_t addr = dma_handle;

	/* Invalidate the dcache for the requested range */
	for (cl = addr; cl < addr + size; cl += cpuinfo.dcache_block_size)
		mtspr(SPR_DCBIR, cl);
}

void or1k_sync_single_for_device(struct device *dev,
			         dma_addr_t dma_handle, size_t size,
			         enum dma_data_direction dir)
{
	unsigned long cl;
	dma_addr_t addr = dma_handle;

	/* Flush the dcache for the requested range */
	for (cl = addr; cl < addr + size; cl += cpuinfo.dcache_block_size)
		mtspr(SPR_DCBFR, cl);
}

/* Number of entries preallocated for DMA-API debugging */
#define PREALLOC_DMA_DEBUG_ENTRIES (1 << 16)

static int __init dma_init(void)
{
	dma_debug_init(PREALLOC_DMA_DEBUG_ENTRIES);

	return 0;
}
fs_initcall(dma_init);
=======
}
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
