<<<<<<< HEAD
=======
// SPDX-License-Identifier: GPL-2.0-only
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
/*
 *  Copyright 2010
 *  by Konrad Rzeszutek Wilk <konrad.wilk@oracle.com>
 *
 * This code provides a IOMMU for Xen PV guests with PCI passthrough.
 *
<<<<<<< HEAD
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License v2.0 as published by
 * the Free Software Foundation
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 * PV guests under Xen are running in an non-contiguous memory architecture.
 *
 * When PCI pass-through is utilized, this necessitates an IOMMU for
 * translating bus (DMA) to virtual and vice-versa and also providing a
 * mechanism to have contiguous pages for device drivers operations (say DMA
 * operations).
 *
 * Specifically, under Xen the Linux idea of pages is an illusion. It
 * assumes that pages start at zero and go up to the available memory. To
 * help with that, the Linux Xen MMU provides a lookup mechanism to
 * translate the page frame numbers (PFN) to machine frame numbers (MFN)
 * and vice-versa. The MFN are the "real" frame numbers. Furthermore
 * memory is not contiguous. Xen hypervisor stitches memory for guests
 * from different pools, which means there is no guarantee that PFN==MFN
 * and PFN+1==MFN+1. Lastly with Xen 4.0, pages (in debug mode) are
 * allocated in descending order (high to low), meaning the guest might
 * never get any MFN's under the 4GB mark.
<<<<<<< HEAD
 *
 */

#include <linux/bootmem.h>
#include <linux/dma-mapping.h>
=======
 */

#define pr_fmt(fmt) "xen:" KBUILD_MODNAME ": " fmt

#include <linux/memblock.h>
#include <linux/dma-direct.h>
#include <linux/dma-map-ops.h>
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#include <linux/export.h>
#include <xen/swiotlb-xen.h>
#include <xen/page.h>
#include <xen/xen-ops.h>
#include <xen/hvc-console.h>
<<<<<<< HEAD
/*
 * Used to do a quick range check in swiotlb_tbl_unmap_single and
 * swiotlb_tbl_sync_single_*, to see if the memory was in fact allocated by this
 * API.
 */

static char *xen_io_tlb_start, *xen_io_tlb_end;
static unsigned long xen_io_tlb_nslabs;
=======

#include <asm/dma-mapping.h>

#include <trace/events/swiotlb.h>
#define MAX_DMA_BITS 32

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
/*
 * Quick lookup value of the bus address of the IOTLB.
 */

<<<<<<< HEAD
u64 start_dma_addr;

static dma_addr_t xen_phys_to_bus(phys_addr_t paddr)
{
	return phys_to_machine(XPADDR(paddr)).maddr;
}

static phys_addr_t xen_bus_to_phys(dma_addr_t baddr)
{
	return machine_to_phys(XMADDR(baddr)).paddr;
}

static dma_addr_t xen_virt_to_bus(void *address)
{
	return xen_phys_to_bus(virt_to_phys(address));
}

static int check_pages_physically_contiguous(unsigned long pfn,
					     unsigned int offset,
					     size_t length)
{
	unsigned long next_mfn;
	int i;
	int nr_pages;

	next_mfn = pfn_to_mfn(pfn);
	nr_pages = (offset + length + PAGE_SIZE-1) >> PAGE_SHIFT;

	for (i = 1; i < nr_pages; i++) {
		if (pfn_to_mfn(++pfn) != ++next_mfn)
			return 0;
	}
	return 1;
}

static int range_straddles_page_boundary(phys_addr_t p, size_t size)
{
	unsigned long pfn = PFN_DOWN(p);
	unsigned int offset = p & ~PAGE_MASK;

	if (offset + size <= PAGE_SIZE)
		return 0;
	if (check_pages_physically_contiguous(pfn, offset, size))
		return 0;
	return 1;
}

static int is_xen_swiotlb_buffer(dma_addr_t dma_addr)
{
	unsigned long mfn = PFN_DOWN(dma_addr);
	unsigned long pfn = mfn_to_local_pfn(mfn);
	phys_addr_t paddr;
=======
static inline phys_addr_t xen_phys_to_bus(struct device *dev, phys_addr_t paddr)
{
	unsigned long bfn = pfn_to_bfn(XEN_PFN_DOWN(paddr));
	phys_addr_t baddr = (phys_addr_t)bfn << XEN_PAGE_SHIFT;

	baddr |= paddr & ~XEN_PAGE_MASK;
	return baddr;
}

static inline dma_addr_t xen_phys_to_dma(struct device *dev, phys_addr_t paddr)
{
	return phys_to_dma(dev, xen_phys_to_bus(dev, paddr));
}

static inline phys_addr_t xen_bus_to_phys(struct device *dev,
					  phys_addr_t baddr)
{
	unsigned long xen_pfn = bfn_to_pfn(XEN_PFN_DOWN(baddr));
	phys_addr_t paddr = (xen_pfn << XEN_PAGE_SHIFT) |
			    (baddr & ~XEN_PAGE_MASK);

	return paddr;
}

static inline phys_addr_t xen_dma_to_phys(struct device *dev,
					  dma_addr_t dma_addr)
{
	return xen_bus_to_phys(dev, dma_to_phys(dev, dma_addr));
}

static inline int range_straddles_page_boundary(phys_addr_t p, size_t size)
{
	unsigned long next_bfn, xen_pfn = XEN_PFN_DOWN(p);
	unsigned int i, nr_pages = XEN_PFN_UP(xen_offset_in_page(p) + size);

	next_bfn = pfn_to_bfn(xen_pfn);

	for (i = 1; i < nr_pages; i++)
		if (pfn_to_bfn(++xen_pfn) != ++next_bfn)
			return 1;

	return 0;
}

static int is_xen_swiotlb_buffer(struct device *dev, dma_addr_t dma_addr)
{
	unsigned long bfn = XEN_PFN_DOWN(dma_to_phys(dev, dma_addr));
	unsigned long xen_pfn = bfn_to_local_pfn(bfn);
	phys_addr_t paddr = (phys_addr_t)xen_pfn << XEN_PAGE_SHIFT;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	/* If the address is outside our domain, it CAN
	 * have the same virtual address as another address
	 * in our domain. Therefore _only_ check address within our domain.
	 */
<<<<<<< HEAD
	if (pfn_valid(pfn)) {
		paddr = PFN_PHYS(pfn);
		return paddr >= virt_to_phys(xen_io_tlb_start) &&
		       paddr < virt_to_phys(xen_io_tlb_end);
	}
	return 0;
}

static int max_dma_bits = 32;

static int
xen_swiotlb_fixup(void *buf, size_t size, unsigned long nslabs)
{
	int i, rc;
	int dma_bits;

	dma_bits = get_order(IO_TLB_SEGSIZE << IO_TLB_SHIFT) + PAGE_SHIFT;

	i = 0;
	do {
		int slabs = min(nslabs - i, (unsigned long)IO_TLB_SEGSIZE);

		do {
			rc = xen_create_contiguous_region(
				(unsigned long)buf + (i << IO_TLB_SHIFT),
				get_order(slabs << IO_TLB_SHIFT),
				dma_bits);
		} while (rc && dma_bits++ < max_dma_bits);
		if (rc)
			return rc;

		i += slabs;
=======
	if (pfn_valid(PFN_DOWN(paddr)))
		return is_swiotlb_buffer(dev, paddr);
	return 0;
}

#ifdef CONFIG_X86
int xen_swiotlb_fixup(void *buf, unsigned long nslabs)
{
	int rc;
	unsigned int order = get_order(IO_TLB_SEGSIZE << IO_TLB_SHIFT);
	unsigned int i, dma_bits = order + PAGE_SHIFT;
	dma_addr_t dma_handle;
	phys_addr_t p = virt_to_phys(buf);

	BUILD_BUG_ON(IO_TLB_SEGSIZE & (IO_TLB_SEGSIZE - 1));
	BUG_ON(nslabs % IO_TLB_SEGSIZE);

	i = 0;
	do {
		do {
			rc = xen_create_contiguous_region(
				p + (i << IO_TLB_SHIFT), order,
				dma_bits, &dma_handle);
		} while (rc && dma_bits++ < MAX_DMA_BITS);
		if (rc)
			return rc;

		i += IO_TLB_SEGSIZE;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	} while (i < nslabs);
	return 0;
}

<<<<<<< HEAD
void __init xen_swiotlb_init(int verbose)
{
	unsigned long bytes;
	int rc = -ENOMEM;
	unsigned long nr_tbl;
	char *m = NULL;
	unsigned int repeat = 3;

	nr_tbl = swiotlb_nr_tbl();
	if (nr_tbl)
		xen_io_tlb_nslabs = nr_tbl;
	else {
		xen_io_tlb_nslabs = (64 * 1024 * 1024 >> IO_TLB_SHIFT);
		xen_io_tlb_nslabs = ALIGN(xen_io_tlb_nslabs, IO_TLB_SEGSIZE);
	}
retry:
	bytes = xen_io_tlb_nslabs << IO_TLB_SHIFT;

	/*
	 * Get IO TLB memory from any location.
	 */
	xen_io_tlb_start = alloc_bootmem_pages(PAGE_ALIGN(bytes));
	if (!xen_io_tlb_start) {
		m = "Cannot allocate Xen-SWIOTLB buffer!\n";
		goto error;
	}
	xen_io_tlb_end = xen_io_tlb_start + bytes;
	/*
	 * And replace that memory with pages under 4GB.
	 */
	rc = xen_swiotlb_fixup(xen_io_tlb_start,
			       bytes,
			       xen_io_tlb_nslabs);
	if (rc) {
		free_bootmem(__pa(xen_io_tlb_start), PAGE_ALIGN(bytes));
		m = "Failed to get contiguous memory for DMA from Xen!\n"\
		    "You either: don't have the permissions, do not have"\
		    " enough free memory under 4GB, or the hypervisor memory"\
		    "is too fragmented!";
		goto error;
	}
	start_dma_addr = xen_virt_to_bus(xen_io_tlb_start);
	swiotlb_init_with_tbl(xen_io_tlb_start, xen_io_tlb_nslabs, verbose);

	return;
error:
	if (repeat--) {
		xen_io_tlb_nslabs = max(1024UL, /* Min is 2MB */
					(xen_io_tlb_nslabs >> 1));
		printk(KERN_INFO "Xen-SWIOTLB: Lowering to %luMB\n",
		      (xen_io_tlb_nslabs << IO_TLB_SHIFT) >> 20);
		goto retry;
	}
	xen_raw_printk("%s (rc:%d)", m, rc);
	panic("%s (rc:%d)", m, rc);
}

void *
xen_swiotlb_alloc_coherent(struct device *hwdev, size_t size,
			   dma_addr_t *dma_handle, gfp_t flags,
			   struct dma_attrs *attrs)
{
	void *ret;
	int order = get_order(size);
	u64 dma_mask = DMA_BIT_MASK(32);
	unsigned long vstart;
	phys_addr_t phys;
	dma_addr_t dev_addr;

	/*
	* Ignore region specifiers - the kernel's ideas of
	* pseudo-phys memory layout has nothing to do with the
	* machine physical layout.  We can't allocate highmem
	* because we can't return a pointer to it.
	*/
	flags &= ~(__GFP_DMA | __GFP_HIGHMEM);

	if (dma_alloc_from_coherent(hwdev, size, dma_handle, &ret))
		return ret;

	vstart = __get_free_pages(flags, order);
	ret = (void *)vstart;

	if (!ret)
		return ret;

	if (hwdev && hwdev->coherent_dma_mask)
		dma_mask = dma_alloc_coherent_mask(hwdev, flags);

	phys = virt_to_phys(ret);
	dev_addr = xen_phys_to_bus(phys);
	if (((dev_addr + size - 1 <= dma_mask)) &&
	    !range_straddles_page_boundary(phys, size))
		*dma_handle = dev_addr;
	else {
		if (xen_create_contiguous_region(vstart, order,
						 fls64(dma_mask)) != 0) {
			free_pages(vstart, order);
			return NULL;
		}
		*dma_handle = virt_to_machine(ret).maddr;
	}
	memset(ret, 0, size);
	return ret;
}
EXPORT_SYMBOL_GPL(xen_swiotlb_alloc_coherent);

void
xen_swiotlb_free_coherent(struct device *hwdev, size_t size, void *vaddr,
			  dma_addr_t dev_addr, struct dma_attrs *attrs)
{
	int order = get_order(size);
	phys_addr_t phys;
	u64 dma_mask = DMA_BIT_MASK(32);

	if (dma_release_from_coherent(hwdev, order, vaddr))
		return;

	if (hwdev && hwdev->coherent_dma_mask)
		dma_mask = hwdev->coherent_dma_mask;

	phys = virt_to_phys(vaddr);

	if (((dev_addr + size - 1 > dma_mask)) ||
	    range_straddles_page_boundary(phys, size))
		xen_destroy_contiguous_region((unsigned long)vaddr, order);

	free_pages((unsigned long)vaddr, order);
}
EXPORT_SYMBOL_GPL(xen_swiotlb_free_coherent);

=======
static void *
xen_swiotlb_alloc_coherent(struct device *dev, size_t size,
		dma_addr_t *dma_handle, gfp_t flags, unsigned long attrs)
{
	u64 dma_mask = dev->coherent_dma_mask;
	int order = get_order(size);
	phys_addr_t phys;
	void *ret;

	/* Align the allocation to the Xen page size */
	size = 1UL << (order + XEN_PAGE_SHIFT);

	ret = (void *)__get_free_pages(flags, get_order(size));
	if (!ret)
		return ret;
	phys = virt_to_phys(ret);

	*dma_handle = xen_phys_to_dma(dev, phys);
	if (*dma_handle + size - 1 > dma_mask ||
	    range_straddles_page_boundary(phys, size)) {
		if (xen_create_contiguous_region(phys, order, fls64(dma_mask),
				dma_handle) != 0)
			goto out_free_pages;
		SetPageXenRemapped(virt_to_page(ret));
	}

	memset(ret, 0, size);
	return ret;

out_free_pages:
	free_pages((unsigned long)ret, get_order(size));
	return NULL;
}

static void
xen_swiotlb_free_coherent(struct device *dev, size_t size, void *vaddr,
		dma_addr_t dma_handle, unsigned long attrs)
{
	phys_addr_t phys = virt_to_phys(vaddr);
	int order = get_order(size);

	/* Convert the size to actually allocated. */
	size = 1UL << (order + XEN_PAGE_SHIFT);

	if (WARN_ON_ONCE(dma_handle + size - 1 > dev->coherent_dma_mask) ||
	    WARN_ON_ONCE(range_straddles_page_boundary(phys, size)))
	    	return;

	if (TestClearPageXenRemapped(virt_to_page(vaddr)))
		xen_destroy_contiguous_region(phys, order);
	free_pages((unsigned long)vaddr, get_order(size));
}
#endif /* CONFIG_X86 */
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

/*
 * Map a single buffer of the indicated size for DMA in streaming mode.  The
 * physical address to use is returned.
 *
 * Once the device is given the dma address, the device owns this memory until
 * either xen_swiotlb_unmap_page or xen_swiotlb_dma_sync_single is performed.
 */
<<<<<<< HEAD
dma_addr_t xen_swiotlb_map_page(struct device *dev, struct page *page,
				unsigned long offset, size_t size,
				enum dma_data_direction dir,
				struct dma_attrs *attrs)
{
	phys_addr_t phys = page_to_phys(page) + offset;
	dma_addr_t dev_addr = xen_phys_to_bus(phys);
	void *map;
=======
static dma_addr_t xen_swiotlb_map_page(struct device *dev, struct page *page,
				unsigned long offset, size_t size,
				enum dma_data_direction dir,
				unsigned long attrs)
{
	phys_addr_t map, phys = page_to_phys(page) + offset;
	dma_addr_t dev_addr = xen_phys_to_dma(dev, phys);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	BUG_ON(dir == DMA_NONE);
	/*
	 * If the address happens to be in the device's DMA window,
	 * we can safely return the device addr and not worry about bounce
	 * buffering it.
	 */
<<<<<<< HEAD
	if (dma_capable(dev, dev_addr, size) &&
	    !range_straddles_page_boundary(phys, size) && !swiotlb_force)
		return dev_addr;
=======
	if (dma_capable(dev, dev_addr, size, true) &&
	    !range_straddles_page_boundary(phys, size) &&
		!xen_arch_need_swiotlb(dev, phys, dev_addr) &&
		!is_swiotlb_force_bounce(dev))
		goto done;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	/*
	 * Oh well, have to allocate and map a bounce buffer.
	 */
<<<<<<< HEAD
	map = swiotlb_tbl_map_single(dev, start_dma_addr, phys, size, dir);
	if (!map)
		return DMA_ERROR_CODE;

	dev_addr = xen_virt_to_bus(map);
=======
	trace_swiotlb_bounced(dev, dev_addr, size);

	map = swiotlb_tbl_map_single(dev, phys, size, size, 0, dir, attrs);
	if (map == (phys_addr_t)DMA_MAPPING_ERROR)
		return DMA_MAPPING_ERROR;

	phys = map;
	dev_addr = xen_phys_to_dma(dev, map);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	/*
	 * Ensure that the address returned is DMA'ble
	 */
<<<<<<< HEAD
	if (!dma_capable(dev, dev_addr, size)) {
		swiotlb_tbl_unmap_single(dev, map, size, dir);
		dev_addr = 0;
	}
	return dev_addr;
}
EXPORT_SYMBOL_GPL(xen_swiotlb_map_page);
=======
	if (unlikely(!dma_capable(dev, dev_addr, size, true))) {
		swiotlb_tbl_unmap_single(dev, map, size, dir,
				attrs | DMA_ATTR_SKIP_CPU_SYNC);
		return DMA_MAPPING_ERROR;
	}

done:
	if (!dev_is_dma_coherent(dev) && !(attrs & DMA_ATTR_SKIP_CPU_SYNC)) {
		if (pfn_valid(PFN_DOWN(dma_to_phys(dev, dev_addr))))
			arch_sync_dma_for_device(phys, size, dir);
		else
			xen_dma_sync_for_device(dev, dev_addr, size, dir);
	}
	return dev_addr;
}
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

/*
 * Unmap a single streaming mode DMA translation.  The dma_addr and size must
 * match what was provided for in a previous xen_swiotlb_map_page call.  All
 * other usages are undefined.
 *
 * After this call, reads by the cpu to the buffer are guaranteed to see
 * whatever the device wrote there.
 */
<<<<<<< HEAD
static void xen_unmap_single(struct device *hwdev, dma_addr_t dev_addr,
			     size_t size, enum dma_data_direction dir)
{
	phys_addr_t paddr = xen_bus_to_phys(dev_addr);

	BUG_ON(dir == DMA_NONE);

	/* NOTE: We use dev_addr here, not paddr! */
	if (is_xen_swiotlb_buffer(dev_addr)) {
		swiotlb_tbl_unmap_single(hwdev, phys_to_virt(paddr), size, dir);
		return;
	}

	if (dir != DMA_FROM_DEVICE)
		return;

	/*
	 * phys_to_virt doesn't work with hihgmem page but we could
	 * call dma_mark_clean() with hihgmem page here. However, we
	 * are fine since dma_mark_clean() is null on POWERPC. We can
	 * make dma_mark_clean() take a physical address if necessary.
	 */
	dma_mark_clean(phys_to_virt(paddr), size);
}

void xen_swiotlb_unmap_page(struct device *hwdev, dma_addr_t dev_addr,
			    size_t size, enum dma_data_direction dir,
			    struct dma_attrs *attrs)
{
	xen_unmap_single(hwdev, dev_addr, size, dir);
}
EXPORT_SYMBOL_GPL(xen_swiotlb_unmap_page);

/*
 * Make physical memory consistent for a single streaming mode DMA translation
 * after a transfer.
 *
 * If you perform a xen_swiotlb_map_page() but wish to interrogate the buffer
 * using the cpu, yet do not wish to teardown the dma mapping, you must
 * call this function before doing so.  At the next point you give the dma
 * address back to the card, you must first perform a
 * xen_swiotlb_dma_sync_for_device, and then the device again owns the buffer
 */
static void
xen_swiotlb_sync_single(struct device *hwdev, dma_addr_t dev_addr,
			size_t size, enum dma_data_direction dir,
			enum dma_sync_target target)
{
	phys_addr_t paddr = xen_bus_to_phys(dev_addr);

	BUG_ON(dir == DMA_NONE);

	/* NOTE: We use dev_addr here, not paddr! */
	if (is_xen_swiotlb_buffer(dev_addr)) {
		swiotlb_tbl_sync_single(hwdev, phys_to_virt(paddr), size, dir,
				       target);
		return;
	}

	if (dir != DMA_FROM_DEVICE)
		return;

	dma_mark_clean(phys_to_virt(paddr), size);
}

void
xen_swiotlb_sync_single_for_cpu(struct device *hwdev, dma_addr_t dev_addr,
				size_t size, enum dma_data_direction dir)
{
	xen_swiotlb_sync_single(hwdev, dev_addr, size, dir, SYNC_FOR_CPU);
}
EXPORT_SYMBOL_GPL(xen_swiotlb_sync_single_for_cpu);

void
xen_swiotlb_sync_single_for_device(struct device *hwdev, dma_addr_t dev_addr,
				   size_t size, enum dma_data_direction dir)
{
	xen_swiotlb_sync_single(hwdev, dev_addr, size, dir, SYNC_FOR_DEVICE);
}
EXPORT_SYMBOL_GPL(xen_swiotlb_sync_single_for_device);

/*
 * Map a set of buffers described by scatterlist in streaming mode for DMA.
 * This is the scatter-gather version of the above xen_swiotlb_map_page
 * interface.  Here the scatter gather list elements are each tagged with the
 * appropriate dma address and length.  They are obtained via
 * sg_dma_{address,length}(SG).
 *
 * NOTE: An implementation may be able to use a smaller number of
 *       DMA address/length pairs than there are SG table elements.
 *       (for example via virtual mapping capabilities)
 *       The routine returns the number of addr/length pairs actually
 *       used, at most nents.
 *
 * Device ownership issues as mentioned above for xen_swiotlb_map_page are the
 * same here.
 */
int
xen_swiotlb_map_sg_attrs(struct device *hwdev, struct scatterlist *sgl,
			 int nelems, enum dma_data_direction dir,
			 struct dma_attrs *attrs)
=======
static void xen_swiotlb_unmap_page(struct device *hwdev, dma_addr_t dev_addr,
		size_t size, enum dma_data_direction dir, unsigned long attrs)
{
	phys_addr_t paddr = xen_dma_to_phys(hwdev, dev_addr);

	BUG_ON(dir == DMA_NONE);

	if (!dev_is_dma_coherent(hwdev) && !(attrs & DMA_ATTR_SKIP_CPU_SYNC)) {
		if (pfn_valid(PFN_DOWN(dma_to_phys(hwdev, dev_addr))))
			arch_sync_dma_for_cpu(paddr, size, dir);
		else
			xen_dma_sync_for_cpu(hwdev, dev_addr, size, dir);
	}

	/* NOTE: We use dev_addr here, not paddr! */
	if (is_xen_swiotlb_buffer(hwdev, dev_addr))
		swiotlb_tbl_unmap_single(hwdev, paddr, size, dir, attrs);
}

static void
xen_swiotlb_sync_single_for_cpu(struct device *dev, dma_addr_t dma_addr,
		size_t size, enum dma_data_direction dir)
{
	phys_addr_t paddr = xen_dma_to_phys(dev, dma_addr);

	if (!dev_is_dma_coherent(dev)) {
		if (pfn_valid(PFN_DOWN(dma_to_phys(dev, dma_addr))))
			arch_sync_dma_for_cpu(paddr, size, dir);
		else
			xen_dma_sync_for_cpu(dev, dma_addr, size, dir);
	}

	if (is_xen_swiotlb_buffer(dev, dma_addr))
		swiotlb_sync_single_for_cpu(dev, paddr, size, dir);
}

static void
xen_swiotlb_sync_single_for_device(struct device *dev, dma_addr_t dma_addr,
		size_t size, enum dma_data_direction dir)
{
	phys_addr_t paddr = xen_dma_to_phys(dev, dma_addr);

	if (is_xen_swiotlb_buffer(dev, dma_addr))
		swiotlb_sync_single_for_device(dev, paddr, size, dir);

	if (!dev_is_dma_coherent(dev)) {
		if (pfn_valid(PFN_DOWN(dma_to_phys(dev, dma_addr))))
			arch_sync_dma_for_device(paddr, size, dir);
		else
			xen_dma_sync_for_device(dev, dma_addr, size, dir);
	}
}

/*
 * Unmap a set of streaming mode DMA translations.  Again, cpu read rules
 * concerning calls here are the same as for swiotlb_unmap_page() above.
 */
static void
xen_swiotlb_unmap_sg(struct device *hwdev, struct scatterlist *sgl, int nelems,
		enum dma_data_direction dir, unsigned long attrs)
{
	struct scatterlist *sg;
	int i;

	BUG_ON(dir == DMA_NONE);

	for_each_sg(sgl, sg, nelems, i)
		xen_swiotlb_unmap_page(hwdev, sg->dma_address, sg_dma_len(sg),
				dir, attrs);

}

static int
xen_swiotlb_map_sg(struct device *dev, struct scatterlist *sgl, int nelems,
		enum dma_data_direction dir, unsigned long attrs)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
{
	struct scatterlist *sg;
	int i;

	BUG_ON(dir == DMA_NONE);

	for_each_sg(sgl, sg, nelems, i) {
<<<<<<< HEAD
		phys_addr_t paddr = sg_phys(sg);
		dma_addr_t dev_addr = xen_phys_to_bus(paddr);

		if (swiotlb_force ||
		    !dma_capable(hwdev, dev_addr, sg->length) ||
		    range_straddles_page_boundary(paddr, sg->length)) {
			void *map = swiotlb_tbl_map_single(hwdev,
							   start_dma_addr,
							   sg_phys(sg),
							   sg->length, dir);
			if (!map) {
				/* Don't panic here, we expect map_sg users
				   to do proper error handling. */
				xen_swiotlb_unmap_sg_attrs(hwdev, sgl, i, dir,
							   attrs);
				sgl[0].dma_length = 0;
				return DMA_ERROR_CODE;
			}
			sg->dma_address = xen_virt_to_bus(map);
		} else
			sg->dma_address = dev_addr;
		sg->dma_length = sg->length;
	}
	return nelems;
}
EXPORT_SYMBOL_GPL(xen_swiotlb_map_sg_attrs);

int
xen_swiotlb_map_sg(struct device *hwdev, struct scatterlist *sgl, int nelems,
		   enum dma_data_direction dir)
{
	return xen_swiotlb_map_sg_attrs(hwdev, sgl, nelems, dir, NULL);
}
EXPORT_SYMBOL_GPL(xen_swiotlb_map_sg);

/*
 * Unmap a set of streaming mode DMA translations.  Again, cpu read rules
 * concerning calls here are the same as for swiotlb_unmap_page() above.
 */
void
xen_swiotlb_unmap_sg_attrs(struct device *hwdev, struct scatterlist *sgl,
			   int nelems, enum dma_data_direction dir,
			   struct dma_attrs *attrs)
{
	struct scatterlist *sg;
	int i;

	BUG_ON(dir == DMA_NONE);

	for_each_sg(sgl, sg, nelems, i)
		xen_unmap_single(hwdev, sg->dma_address, sg->dma_length, dir);

}
EXPORT_SYMBOL_GPL(xen_swiotlb_unmap_sg_attrs);

void
xen_swiotlb_unmap_sg(struct device *hwdev, struct scatterlist *sgl, int nelems,
		     enum dma_data_direction dir)
{
	return xen_swiotlb_unmap_sg_attrs(hwdev, sgl, nelems, dir, NULL);
}
EXPORT_SYMBOL_GPL(xen_swiotlb_unmap_sg);

/*
 * Make physical memory consistent for a set of streaming mode DMA translations
 * after a transfer.
 *
 * The same as swiotlb_sync_single_* but for a scatter-gather list, same rules
 * and usage.
 */
static void
xen_swiotlb_sync_sg(struct device *hwdev, struct scatterlist *sgl,
		    int nelems, enum dma_data_direction dir,
		    enum dma_sync_target target)
{
	struct scatterlist *sg;
	int i;

	for_each_sg(sgl, sg, nelems, i)
		xen_swiotlb_sync_single(hwdev, sg->dma_address,
					sg->dma_length, dir, target);
}

void
xen_swiotlb_sync_sg_for_cpu(struct device *hwdev, struct scatterlist *sg,
			    int nelems, enum dma_data_direction dir)
{
	xen_swiotlb_sync_sg(hwdev, sg, nelems, dir, SYNC_FOR_CPU);
}
EXPORT_SYMBOL_GPL(xen_swiotlb_sync_sg_for_cpu);

void
xen_swiotlb_sync_sg_for_device(struct device *hwdev, struct scatterlist *sg,
			       int nelems, enum dma_data_direction dir)
{
	xen_swiotlb_sync_sg(hwdev, sg, nelems, dir, SYNC_FOR_DEVICE);
}
EXPORT_SYMBOL_GPL(xen_swiotlb_sync_sg_for_device);

int
xen_swiotlb_dma_mapping_error(struct device *hwdev, dma_addr_t dma_addr)
{
	return !dma_addr;
}
EXPORT_SYMBOL_GPL(xen_swiotlb_dma_mapping_error);
=======
		sg->dma_address = xen_swiotlb_map_page(dev, sg_page(sg),
				sg->offset, sg->length, dir, attrs);
		if (sg->dma_address == DMA_MAPPING_ERROR)
			goto out_unmap;
		sg_dma_len(sg) = sg->length;
	}

	return nelems;
out_unmap:
	xen_swiotlb_unmap_sg(dev, sgl, i, dir, attrs | DMA_ATTR_SKIP_CPU_SYNC);
	sg_dma_len(sgl) = 0;
	return -EIO;
}

static void
xen_swiotlb_sync_sg_for_cpu(struct device *dev, struct scatterlist *sgl,
			    int nelems, enum dma_data_direction dir)
{
	struct scatterlist *sg;
	int i;

	for_each_sg(sgl, sg, nelems, i) {
		xen_swiotlb_sync_single_for_cpu(dev, sg->dma_address,
				sg->length, dir);
	}
}

static void
xen_swiotlb_sync_sg_for_device(struct device *dev, struct scatterlist *sgl,
			       int nelems, enum dma_data_direction dir)
{
	struct scatterlist *sg;
	int i;

	for_each_sg(sgl, sg, nelems, i) {
		xen_swiotlb_sync_single_for_device(dev, sg->dma_address,
				sg->length, dir);
	}
}
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

/*
 * Return whether the given device DMA address mask can be supported
 * properly.  For example, if your device can only drive the low 24-bits
 * during bus mastering, then you would pass 0x00ffffff as the mask to
 * this function.
 */
<<<<<<< HEAD
int
xen_swiotlb_dma_supported(struct device *hwdev, u64 mask)
{
	return xen_virt_to_bus(xen_io_tlb_end - 1) <= mask;
}
EXPORT_SYMBOL_GPL(xen_swiotlb_dma_supported);
=======
static int
xen_swiotlb_dma_supported(struct device *hwdev, u64 mask)
{
	return xen_phys_to_dma(hwdev, default_swiotlb_limit()) <= mask;
}

const struct dma_map_ops xen_swiotlb_dma_ops = {
#ifdef CONFIG_X86
	.alloc = xen_swiotlb_alloc_coherent,
	.free = xen_swiotlb_free_coherent,
#else
	.alloc = dma_direct_alloc,
	.free = dma_direct_free,
#endif
	.sync_single_for_cpu = xen_swiotlb_sync_single_for_cpu,
	.sync_single_for_device = xen_swiotlb_sync_single_for_device,
	.sync_sg_for_cpu = xen_swiotlb_sync_sg_for_cpu,
	.sync_sg_for_device = xen_swiotlb_sync_sg_for_device,
	.map_sg = xen_swiotlb_map_sg,
	.unmap_sg = xen_swiotlb_unmap_sg,
	.map_page = xen_swiotlb_map_page,
	.unmap_page = xen_swiotlb_unmap_page,
	.dma_supported = xen_swiotlb_dma_supported,
	.mmap = dma_common_mmap,
	.get_sgtable = dma_common_get_sgtable,
	.alloc_pages = dma_common_alloc_pages,
	.free_pages = dma_common_free_pages,
	.max_mapping_size = swiotlb_max_mapping_size,
};
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
