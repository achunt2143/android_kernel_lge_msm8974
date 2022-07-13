<<<<<<< HEAD
/*
 * arch/xtensa/kernel/pci-dma.c
 *
 * DMA coherent memory allocation.
 *
 * This program is free software; you can redistribute  it and/or modify it
 * under  the terms of  the GNU General  Public License as published by the
 * Free Software Foundation;  either version 2 of the  License, or (at your
 * option) any later version.
 *
 * Copyright (C) 2002 - 2005 Tensilica Inc.
=======
// SPDX-License-Identifier: GPL-2.0-or-later
/*
 * DMA coherent memory allocation.
 *
 * Copyright (C) 2002 - 2005 Tensilica Inc.
 * Copyright (C) 2015 Cadence Design Systems Inc.
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 *
 * Based on version for i386.
 *
 * Chris Zankel <chris@zankel.net>
 * Joe Taylor <joe@tensilica.com, joetylr@yahoo.com>
 */

<<<<<<< HEAD
#include <linux/types.h>
#include <linux/mm.h>
#include <linux/string.h>
#include <linux/pci.h>
#include <linux/gfp.h>
#include <asm/io.h>
#include <asm/cacheflush.h>

/*
 * Note: We assume that the full memory space is always mapped to 'kseg'
 *	 Otherwise we have to use page attributes (not implemented).
 */

void *
dma_alloc_coherent(struct device *dev,size_t size,dma_addr_t *handle,gfp_t flag)
{
	unsigned long ret;
	unsigned long uncached = 0;

	/* ignore region speicifiers */

	flag &= ~(__GFP_DMA | __GFP_HIGHMEM);

	if (dev == NULL || (dev->coherent_dma_mask < 0xffffffff))
		flag |= GFP_DMA;
	ret = (unsigned long)__get_free_pages(flag, get_order(size));

	if (ret == 0)
		return NULL;

	/* We currently don't support coherent memory outside KSEG */

	BUG_ON(ret < XCHAL_KSEG_CACHED_VADDR ||
	       ret > XCHAL_KSEG_CACHED_VADDR + XCHAL_KSEG_SIZE - 1);


	if (ret != 0) {
		memset((void*) ret, 0, size);
		uncached = ret+XCHAL_KSEG_BYPASS_VADDR-XCHAL_KSEG_CACHED_VADDR;
		*handle = virt_to_bus((void*)ret);
		__flush_invalidate_dcache_range(ret, size);
	}

	return (void*)uncached;
}

void dma_free_coherent(struct device *hwdev, size_t size,
			 void *vaddr, dma_addr_t dma_handle)
{
	unsigned long addr = (unsigned long)vaddr +
		XCHAL_KSEG_CACHED_VADDR - XCHAL_KSEG_BYPASS_VADDR;

	BUG_ON(addr < XCHAL_KSEG_CACHED_VADDR ||
	       addr > XCHAL_KSEG_CACHED_VADDR + XCHAL_KSEG_SIZE - 1);

	free_pages(addr, get_order(size));
}


void consistent_sync(void *vaddr, size_t size, int direction)
{
	switch (direction) {
	case PCI_DMA_NONE:
		BUG();
	case PCI_DMA_FROMDEVICE:        /* invalidate only */
		__invalidate_dcache_range((unsigned long)vaddr,
				          (unsigned long)size);
		break;

	case PCI_DMA_TODEVICE:          /* writeback only */
	case PCI_DMA_BIDIRECTIONAL:     /* writeback and invalidate */
		__flush_invalidate_dcache_range((unsigned long)vaddr,
				    		(unsigned long)size);
		break;
	}
}
=======
#include <linux/dma-map-ops.h>
#include <linux/dma-direct.h>
#include <linux/gfp.h>
#include <linux/highmem.h>
#include <linux/mm.h>
#include <linux/types.h>
#include <asm/cacheflush.h>
#include <asm/io.h>
#include <asm/platform.h>

static void do_cache_op(phys_addr_t paddr, size_t size,
			void (*fn)(unsigned long, unsigned long))
{
	unsigned long off = paddr & (PAGE_SIZE - 1);
	unsigned long pfn = PFN_DOWN(paddr);
	struct page *page = pfn_to_page(pfn);

	if (!PageHighMem(page))
		fn((unsigned long)phys_to_virt(paddr), size);
	else
		while (size > 0) {
			size_t sz = min_t(size_t, size, PAGE_SIZE - off);
			void *vaddr = kmap_atomic(page);

			fn((unsigned long)vaddr + off, sz);
			kunmap_atomic(vaddr);
			off = 0;
			++page;
			size -= sz;
		}
}

void arch_sync_dma_for_cpu(phys_addr_t paddr, size_t size,
		enum dma_data_direction dir)
{
	switch (dir) {
	case DMA_BIDIRECTIONAL:
	case DMA_FROM_DEVICE:
		do_cache_op(paddr, size, __invalidate_dcache_range);
		break;

	case DMA_NONE:
		BUG();
		break;

	default:
		break;
	}
}

void arch_sync_dma_for_device(phys_addr_t paddr, size_t size,
		enum dma_data_direction dir)
{
	switch (dir) {
	case DMA_BIDIRECTIONAL:
	case DMA_TO_DEVICE:
		if (XCHAL_DCACHE_IS_WRITEBACK)
			do_cache_op(paddr, size, __flush_dcache_range);
		break;

	case DMA_NONE:
		BUG();
		break;

	default:
		break;
	}
}

void arch_dma_prep_coherent(struct page *page, size_t size)
{
	__invalidate_dcache_range((unsigned long)page_address(page), size);
}

/*
 * Memory caching is platform-dependent in noMMU xtensa configurations.
 * This function should be implemented in platform code in order to enable
 * coherent DMA memory operations when CONFIG_MMU is not enabled.
 */
#ifdef CONFIG_MMU
void *arch_dma_set_uncached(void *p, size_t size)
{
	return p + XCHAL_KSEG_BYPASS_VADDR - XCHAL_KSEG_CACHED_VADDR;
}
#endif /* CONFIG_MMU */
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
