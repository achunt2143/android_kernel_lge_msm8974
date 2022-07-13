<<<<<<< HEAD
=======
// SPDX-License-Identifier: GPL-2.0-or-later
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
/*
 *  PowerPC version
 *    Copyright (C) 1995-1996 Gary Thomas (gdt@linuxppc.org)
 *
 *  Modifications by Paul Mackerras (PowerMac) (paulus@cs.anu.edu.au)
 *  and Cort Dougan (PReP) (cort@cs.nmt.edu)
 *    Copyright (C) 1996 Paul Mackerras
 *
 *  Derived from "arch/i386/mm/init.c"
 *    Copyright (C) 1991, 1992, 1993, 1994  Linus Torvalds
 *
 *  Dave Engebretsen <engebret@us.ibm.com>
 *      Rework for PPC64 port.
<<<<<<< HEAD
 *
 *  This program is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU General Public License
 *  as published by the Free Software Foundation; either version
 *  2 of the License, or (at your option) any later version.
 *
=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 */

#undef DEBUG

#include <linux/signal.h>
#include <linux/sched.h>
#include <linux/kernel.h>
#include <linux/errno.h>
#include <linux/string.h>
#include <linux/types.h>
#include <linux/mman.h>
#include <linux/mm.h>
#include <linux/swap.h>
#include <linux/stddef.h>
#include <linux/vmalloc.h>
#include <linux/init.h>
#include <linux/delay.h>
<<<<<<< HEAD
#include <linux/bootmem.h>
=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#include <linux/highmem.h>
#include <linux/idr.h>
#include <linux/nodemask.h>
#include <linux/module.h>
#include <linux/poison.h>
#include <linux/memblock.h>
#include <linux/hugetlb.h>
#include <linux/slab.h>
<<<<<<< HEAD
=======
#include <linux/of_fdt.h>
#include <linux/libfdt.h>
#include <linux/memremap.h>
#include <linux/memory.h>
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

#include <asm/pgalloc.h>
#include <asm/page.h>
#include <asm/prom.h>
#include <asm/rtas.h>
#include <asm/io.h>
#include <asm/mmu_context.h>
<<<<<<< HEAD
#include <asm/pgtable.h>
#include <asm/mmu.h>
#include <asm/uaccess.h>
=======
#include <asm/mmu.h>
#include <linux/uaccess.h>
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#include <asm/smp.h>
#include <asm/machdep.h>
#include <asm/tlb.h>
#include <asm/eeh.h>
#include <asm/processor.h>
#include <asm/mmzone.h>
#include <asm/cputable.h>
#include <asm/sections.h>
#include <asm/iommu.h>
<<<<<<< HEAD
#include <asm/abs_addr.h>
#include <asm/vdso.h>

#include "mmu_decl.h"

#ifdef CONFIG_PPC_STD_MMU_64
#if PGTABLE_RANGE > USER_VSID_RANGE
#warning Limited user VSID range means pagetable space is wasted
#endif

#if (TASK_SIZE_USER64 < PGTABLE_RANGE) && (TASK_SIZE_USER64 < USER_VSID_RANGE)
#warning TASK_SIZE is smaller than it needs to be.
#endif
#endif /* CONFIG_PPC_STD_MMU_64 */

phys_addr_t memstart_addr = ~0;
EXPORT_SYMBOL_GPL(memstart_addr);
phys_addr_t kernstart_addr;
EXPORT_SYMBOL_GPL(kernstart_addr);

static void pgd_ctor(void *addr)
{
	memset(addr, 0, PGD_TABLE_SIZE);
}

static void pmd_ctor(void *addr)
{
	memset(addr, 0, PMD_TABLE_SIZE);
}

struct kmem_cache *pgtable_cache[MAX_PGTABLE_INDEX_SIZE];

/*
 * Create a kmem_cache() for pagetables.  This is not used for PTE
 * pages - they're linked to struct page, come from the normal free
 * pages pool and have a different entry size (see real_pte_t) to
 * everything else.  Caches created by this function are used for all
 * the higher level pagetables, and for hugepage pagetables.
 */
void pgtable_cache_add(unsigned shift, void (*ctor)(void *))
{
	char *name;
	unsigned long table_size = sizeof(void *) << shift;
	unsigned long align = table_size;

	/* When batching pgtable pointers for RCU freeing, we store
	 * the index size in the low bits.  Table alignment must be
	 * big enough to fit it.
	 *
	 * Likewise, hugeapge pagetable pointers contain a (different)
	 * shift value in the low bits.  All tables must be aligned so
	 * as to leave enough 0 bits in the address to contain it. */
	unsigned long minalign = max(MAX_PGTABLE_INDEX_SIZE + 1,
				     HUGEPD_SHIFT_MASK + 1);
	struct kmem_cache *new;

	/* It would be nice if this was a BUILD_BUG_ON(), but at the
	 * moment, gcc doesn't seem to recognize is_power_of_2 as a
	 * constant expression, so so much for that. */
	BUG_ON(!is_power_of_2(minalign));
	BUG_ON((shift < 1) || (shift > MAX_PGTABLE_INDEX_SIZE));

	if (PGT_CACHE(shift))
		return; /* Already have a cache of this size */

	align = max_t(unsigned long, align, minalign);
	name = kasprintf(GFP_KERNEL, "pgtable-2^%d", shift);
	new = kmem_cache_create(name, table_size, align, 0, ctor);
	PGT_CACHE(shift) = new;

	pr_debug("Allocated pgtable cache for order %d\n", shift);
}


void pgtable_cache_init(void)
{
	pgtable_cache_add(PGD_INDEX_SIZE, pgd_ctor);
	pgtable_cache_add(PMD_INDEX_SIZE, pmd_ctor);
	if (!PGT_CACHE(PGD_INDEX_SIZE) || !PGT_CACHE(PMD_INDEX_SIZE))
		panic("Couldn't allocate pgtable caches");

	/* In all current configs, when the PUD index exists it's the
	 * same size as either the pgd or pmd index.  Verify that the
	 * initialization above has also created a PUD cache.  This
	 * will need re-examiniation if we add new possibilities for
	 * the pagetable layout. */
	BUG_ON(PUD_INDEX_SIZE && !PGT_CACHE(PUD_INDEX_SIZE));
}

#ifdef CONFIG_SPARSEMEM_VMEMMAP
/*
 * Given an address within the vmemmap, determine the pfn of the page that
 * represents the start of the section it is within.  Note that we have to
 * do this by hand as the proffered address may not be correctly aligned.
 * Subtraction of non-aligned pointers produces undefined results.
 */
static unsigned long __meminit vmemmap_section_start(unsigned long page)
{
	unsigned long offset = page - ((unsigned long)(vmemmap));

	/* Return the pfn of the start of the section. */
	return (offset / sizeof(struct page)) & PAGE_SECTION_MASK;
}

/*
 * Check if this vmemmap page is already initialised.  If any section
 * which overlaps this vmemmap page is initialised then this page is
 * initialised already.
 */
static int __meminit vmemmap_populated(unsigned long start, int page_size)
{
	unsigned long end = start + page_size;

	for (; start < end; start += (PAGES_PER_SECTION * sizeof(struct page)))
		if (pfn_valid(vmemmap_section_start(start)))
=======
#include <asm/vdso.h>
#include <asm/hugetlb.h>

#include <mm/mmu_decl.h>

#ifdef CONFIG_SPARSEMEM_VMEMMAP
/*
 * Given an address within the vmemmap, determine the page that
 * represents the start of the subsection it is within.  Note that we have to
 * do this by hand as the proffered address may not be correctly aligned.
 * Subtraction of non-aligned pointers produces undefined results.
 */
static struct page * __meminit vmemmap_subsection_start(unsigned long vmemmap_addr)
{
	unsigned long start_pfn;
	unsigned long offset = vmemmap_addr - ((unsigned long)(vmemmap));

	/* Return the pfn of the start of the section. */
	start_pfn = (offset / sizeof(struct page)) & PAGE_SUBSECTION_MASK;
	return pfn_to_page(start_pfn);
}

/*
 * Since memory is added in sub-section chunks, before creating a new vmemmap
 * mapping, the kernel should check whether there is an existing memmap mapping
 * covering the new subsection added. This is needed because kernel can map
 * vmemmap area using 16MB pages which will cover a memory range of 16G. Such
 * a range covers multiple subsections (2M)
 *
 * If any subsection in the 16G range mapped by vmemmap is valid we consider the
 * vmemmap populated (There is a page table entry already present). We can't do
 * a page table lookup here because with the hash translation we don't keep
 * vmemmap details in linux page table.
 */
int __meminit vmemmap_populated(unsigned long vmemmap_addr, int vmemmap_map_size)
{
	struct page *start;
	unsigned long vmemmap_end = vmemmap_addr + vmemmap_map_size;
	start = vmemmap_subsection_start(vmemmap_addr);

	for (; (unsigned long)start < vmemmap_end; start += PAGES_PER_SUBSECTION)
		/*
		 * pfn valid check here is intended to really check
		 * whether we have any subsection already initialized
		 * in this range.
		 */
		if (pfn_valid(page_to_pfn(start)))
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
			return 1;

	return 0;
}

<<<<<<< HEAD
/* On hash-based CPUs, the vmemmap is bolted in the hash table.
 *
 * On Book3E CPUs, the vmemmap is currently mapped in the top half of
 * the vmalloc space using normal page tables, though the size of
 * pages encoded in the PTEs can be different
 */

#ifdef CONFIG_PPC_BOOK3E
static void __meminit vmemmap_create_mapping(unsigned long start,
					     unsigned long page_size,
					     unsigned long phys)
{
	/* Create a PTE encoding without page size */
	unsigned long i, flags = _PAGE_PRESENT | _PAGE_ACCESSED |
		_PAGE_KERNEL_RW;

	/* PTEs only contain page size encodings up to 32M */
	BUG_ON(mmu_psize_defs[mmu_vmemmap_psize].enc > 0xf);

	/* Encode the size in the PTE */
	flags |= mmu_psize_defs[mmu_vmemmap_psize].enc << 8;

	/* For each PTE for that area, map things. Note that we don't
	 * increment phys because all PTEs are of the large size and
	 * thus must have the low bits clear
	 */
	for (i = 0; i < page_size; i += PAGE_SIZE)
		BUG_ON(map_kernel_page(start + i, phys, flags));
}
#else /* CONFIG_PPC_BOOK3E */
static void __meminit vmemmap_create_mapping(unsigned long start,
					     unsigned long page_size,
					     unsigned long phys)
{
	int  mapped = htab_bolt_mapping(start, start + page_size, phys,
					PAGE_KERNEL, mmu_vmemmap_psize,
					mmu_kernel_ssize);
	BUG_ON(mapped < 0);
}
#endif /* CONFIG_PPC_BOOK3E */

struct vmemmap_backing *vmemmap_list;

static __meminit struct vmemmap_backing * vmemmap_list_alloc(int node)
{
	static struct vmemmap_backing *next;
	static int num_left;

	/* allocate a page when required and hand out chunks */
	if (!next || !num_left) {
=======
/*
 * vmemmap virtual address space management does not have a traditional page
 * table to track which virtual struct pages are backed by physical mapping.
 * The virtual to physical mappings are tracked in a simple linked list
 * format. 'vmemmap_list' maintains the entire vmemmap physical mapping at
 * all times where as the 'next' list maintains the available
 * vmemmap_backing structures which have been deleted from the
 * 'vmemmap_global' list during system runtime (memory hotplug remove
 * operation). The freed 'vmemmap_backing' structures are reused later when
 * new requests come in without allocating fresh memory. This pointer also
 * tracks the allocated 'vmemmap_backing' structures as we allocate one
 * full page memory at a time when we dont have any.
 */
struct vmemmap_backing *vmemmap_list;
static struct vmemmap_backing *next;

/*
 * The same pointer 'next' tracks individual chunks inside the allocated
 * full page during the boot time and again tracks the freed nodes during
 * runtime. It is racy but it does not happen as they are separated by the
 * boot process. Will create problem if some how we have memory hotplug
 * operation during boot !!
 */
static int num_left;
static int num_freed;

static __meminit struct vmemmap_backing * vmemmap_list_alloc(int node)
{
	struct vmemmap_backing *vmem_back;
	/* get from freed entries first */
	if (num_freed) {
		num_freed--;
		vmem_back = next;
		next = next->list;

		return vmem_back;
	}

	/* allocate a page when required and hand out chunks */
	if (!num_left) {
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		next = vmemmap_alloc_block(PAGE_SIZE, node);
		if (unlikely(!next)) {
			WARN_ON(1);
			return NULL;
		}
		num_left = PAGE_SIZE / sizeof(struct vmemmap_backing);
	}

	num_left--;

	return next++;
}

<<<<<<< HEAD
static __meminit void vmemmap_list_populate(unsigned long phys,
					    unsigned long start,
					    int node)
=======
static __meminit int vmemmap_list_populate(unsigned long phys,
					   unsigned long start,
					   int node)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
{
	struct vmemmap_backing *vmem_back;

	vmem_back = vmemmap_list_alloc(node);
	if (unlikely(!vmem_back)) {
<<<<<<< HEAD
		WARN_ON(1);
		return;
=======
		pr_debug("vmemap list allocation failed\n");
		return -ENOMEM;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	}

	vmem_back->phys = phys;
	vmem_back->virt_addr = start;
	vmem_back->list = vmemmap_list;

	vmemmap_list = vmem_back;
<<<<<<< HEAD
}

int __meminit vmemmap_populate(struct page *start_page,
			       unsigned long nr_pages, int node)
{
	unsigned long start = (unsigned long)start_page;
	unsigned long end = (unsigned long)(start_page + nr_pages);
	unsigned long page_size = 1 << mmu_psize_defs[mmu_vmemmap_psize].shift;

	/* Align to the page size of the linear mapping. */
	start = _ALIGN_DOWN(start, page_size);

	pr_debug("vmemmap_populate page %p, %ld pages, node %d\n",
		 start_page, nr_pages, node);
	pr_debug(" -> map %lx..%lx\n", start, end);

	for (; start < end; start += page_size) {
		void *p;

		if (vmemmap_populated(start, page_size))
			continue;

		p = vmemmap_alloc_block(page_size, node);
		if (!p)
			return -ENOMEM;

		vmemmap_list_populate(__pa(p), start, node);
=======
	return 0;
}

bool altmap_cross_boundary(struct vmem_altmap *altmap, unsigned long start,
			   unsigned long page_size)
{
	unsigned long nr_pfn = page_size / sizeof(struct page);
	unsigned long start_pfn = page_to_pfn((struct page *)start);

	if ((start_pfn + nr_pfn - 1) > altmap->end_pfn)
		return true;

	if (start_pfn < altmap->base_pfn)
		return true;

	return false;
}

static int __meminit __vmemmap_populate(unsigned long start, unsigned long end, int node,
					struct vmem_altmap *altmap)
{
	bool altmap_alloc;
	unsigned long page_size = 1 << mmu_psize_defs[mmu_vmemmap_psize].shift;

	/* Align to the page size of the linear mapping. */
	start = ALIGN_DOWN(start, page_size);

	pr_debug("vmemmap_populate %lx..%lx, node %d\n", start, end, node);

	for (; start < end; start += page_size) {
		void *p = NULL;
		int rc;

		/*
		 * This vmemmap range is backing different subsections. If any
		 * of that subsection is marked valid, that means we already
		 * have initialized a page table covering this range and hence
		 * the vmemmap range is populated.
		 */
		if (vmemmap_populated(start, page_size))
			continue;

		/*
		 * Allocate from the altmap first if we have one. This may
		 * fail due to alignment issues when using 16MB hugepages, so
		 * fall back to system memory if the altmap allocation fail.
		 */
		if (altmap && !altmap_cross_boundary(altmap, start, page_size)) {
			p = vmemmap_alloc_block_buf(page_size, node, altmap);
			if (!p)
				pr_debug("altmap block allocation failed, falling back to system memory");
			else
				altmap_alloc = true;
		}
		if (!p) {
			p = vmemmap_alloc_block_buf(page_size, node, NULL);
			altmap_alloc = false;
		}
		if (!p)
			return -ENOMEM;

		if (vmemmap_list_populate(__pa(p), start, node)) {
			/*
			 * If we don't populate vmemap list, we don't have
			 * the ability to free the allocated vmemmap
			 * pages in section_deactivate. Hence free them
			 * here.
			 */
			int nr_pfns = page_size >> PAGE_SHIFT;
			unsigned long page_order = get_order(page_size);

			if (altmap_alloc)
				vmem_altmap_free(altmap, nr_pfns);
			else
				free_pages((unsigned long)p, page_order);
			return -ENOMEM;
		}
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

		pr_debug("      * %016lx..%016lx allocated at %p\n",
			 start, start + page_size, p);

<<<<<<< HEAD
		vmemmap_create_mapping(start, page_size, __pa(p));
=======
		rc = vmemmap_create_mapping(start, page_size, __pa(p));
		if (rc < 0) {
			pr_warn("%s: Unable to create vmemmap mapping: %d\n",
				__func__, rc);
			return -EFAULT;
		}
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	}

	return 0;
}
<<<<<<< HEAD
#endif /* CONFIG_SPARSEMEM_VMEMMAP */

=======

int __meminit vmemmap_populate(unsigned long start, unsigned long end, int node,
			       struct vmem_altmap *altmap)
{

#ifdef CONFIG_PPC_BOOK3S_64
	if (radix_enabled())
		return radix__vmemmap_populate(start, end, node, altmap);
#endif

	return __vmemmap_populate(start, end, node, altmap);
}

#ifdef CONFIG_MEMORY_HOTPLUG
static unsigned long vmemmap_list_free(unsigned long start)
{
	struct vmemmap_backing *vmem_back, *vmem_back_prev;

	vmem_back_prev = vmem_back = vmemmap_list;

	/* look for it with prev pointer recorded */
	for (; vmem_back; vmem_back = vmem_back->list) {
		if (vmem_back->virt_addr == start)
			break;
		vmem_back_prev = vmem_back;
	}

	if (unlikely(!vmem_back))
		return 0;

	/* remove it from vmemmap_list */
	if (vmem_back == vmemmap_list) /* remove head */
		vmemmap_list = vmem_back->list;
	else
		vmem_back_prev->list = vmem_back->list;

	/* next point to this freed entry */
	vmem_back->list = next;
	next = vmem_back;
	num_freed++;

	return vmem_back->phys;
}

static void __ref __vmemmap_free(unsigned long start, unsigned long end,
				 struct vmem_altmap *altmap)
{
	unsigned long page_size = 1 << mmu_psize_defs[mmu_vmemmap_psize].shift;
	unsigned long page_order = get_order(page_size);
	unsigned long alt_start = ~0, alt_end = ~0;
	unsigned long base_pfn;

	start = ALIGN_DOWN(start, page_size);
	if (altmap) {
		alt_start = altmap->base_pfn;
		alt_end = altmap->base_pfn + altmap->reserve + altmap->free;
	}

	pr_debug("vmemmap_free %lx...%lx\n", start, end);

	for (; start < end; start += page_size) {
		unsigned long nr_pages, addr;
		struct page *page;

		/*
		 * We have already marked the subsection we are trying to remove
		 * invalid. So if we want to remove the vmemmap range, we
		 * need to make sure there is no subsection marked valid
		 * in this range.
		 */
		if (vmemmap_populated(start, page_size))
			continue;

		addr = vmemmap_list_free(start);
		if (!addr)
			continue;

		page = pfn_to_page(addr >> PAGE_SHIFT);
		nr_pages = 1 << page_order;
		base_pfn = PHYS_PFN(addr);

		if (base_pfn >= alt_start && base_pfn < alt_end) {
			vmem_altmap_free(altmap, nr_pages);
		} else if (PageReserved(page)) {
			/* allocated from bootmem */
			if (page_size < PAGE_SIZE) {
				/*
				 * this shouldn't happen, but if it is
				 * the case, leave the memory there
				 */
				WARN_ON_ONCE(1);
			} else {
				while (nr_pages--)
					free_reserved_page(page++);
			}
		} else {
			free_pages((unsigned long)(__va(addr)), page_order);
		}

		vmemmap_remove_mapping(start, page_size);
	}
}

void __ref vmemmap_free(unsigned long start, unsigned long end,
			struct vmem_altmap *altmap)
{
#ifdef CONFIG_PPC_BOOK3S_64
	if (radix_enabled())
		return radix__vmemmap_free(start, end, altmap);
#endif
	return __vmemmap_free(start, end, altmap);
}

#endif
void register_page_bootmem_memmap(unsigned long section_nr,
				  struct page *start_page, unsigned long size)
{
}

#endif /* CONFIG_SPARSEMEM_VMEMMAP */

#ifdef CONFIG_PPC_BOOK3S_64
unsigned int mmu_lpid_bits;
#ifdef CONFIG_KVM_BOOK3S_HV_POSSIBLE
EXPORT_SYMBOL_GPL(mmu_lpid_bits);
#endif
unsigned int mmu_pid_bits;

static bool disable_radix = !IS_ENABLED(CONFIG_PPC_RADIX_MMU_DEFAULT);

static int __init parse_disable_radix(char *p)
{
	bool val;

	if (!p)
		val = true;
	else if (kstrtobool(p, &val))
		return -EINVAL;

	disable_radix = val;

	return 0;
}
early_param("disable_radix", parse_disable_radix);

/*
 * If we're running under a hypervisor, we need to check the contents of
 * /chosen/ibm,architecture-vec-5 to see if the hypervisor is willing to do
 * radix.  If not, we clear the radix feature bit so we fall back to hash.
 */
static void __init early_check_vec5(void)
{
	unsigned long root, chosen;
	int size;
	const u8 *vec5;
	u8 mmu_supported;

	root = of_get_flat_dt_root();
	chosen = of_get_flat_dt_subnode_by_name(root, "chosen");
	if (chosen == -FDT_ERR_NOTFOUND) {
		cur_cpu_spec->mmu_features &= ~MMU_FTR_TYPE_RADIX;
		return;
	}
	vec5 = of_get_flat_dt_prop(chosen, "ibm,architecture-vec-5", &size);
	if (!vec5) {
		cur_cpu_spec->mmu_features &= ~MMU_FTR_TYPE_RADIX;
		return;
	}
	if (size <= OV5_INDX(OV5_MMU_SUPPORT)) {
		cur_cpu_spec->mmu_features &= ~MMU_FTR_TYPE_RADIX;
		return;
	}

	/* Check for supported configuration */
	mmu_supported = vec5[OV5_INDX(OV5_MMU_SUPPORT)] &
			OV5_FEAT(OV5_MMU_SUPPORT);
	if (mmu_supported == OV5_FEAT(OV5_MMU_RADIX)) {
		/* Hypervisor only supports radix - check enabled && GTSE */
		if (!early_radix_enabled()) {
			pr_warn("WARNING: Ignoring cmdline option disable_radix\n");
		}
		if (!(vec5[OV5_INDX(OV5_RADIX_GTSE)] &
						OV5_FEAT(OV5_RADIX_GTSE))) {
			cur_cpu_spec->mmu_features &= ~MMU_FTR_GTSE;
		} else
			cur_cpu_spec->mmu_features |= MMU_FTR_GTSE;
		/* Do radix anyway - the hypervisor said we had to */
		cur_cpu_spec->mmu_features |= MMU_FTR_TYPE_RADIX;
	} else if (mmu_supported == OV5_FEAT(OV5_MMU_HASH)) {
		/* Hypervisor only supports hash - disable radix */
		cur_cpu_spec->mmu_features &= ~MMU_FTR_TYPE_RADIX;
		cur_cpu_spec->mmu_features &= ~MMU_FTR_GTSE;
	}
}

static int __init dt_scan_mmu_pid_width(unsigned long node,
					   const char *uname, int depth,
					   void *data)
{
	int size = 0;
	const __be32 *prop;
	const char *type = of_get_flat_dt_prop(node, "device_type", NULL);

	/* We are scanning "cpu" nodes only */
	if (type == NULL || strcmp(type, "cpu") != 0)
		return 0;

	/* Find MMU LPID, PID register size */
	prop = of_get_flat_dt_prop(node, "ibm,mmu-lpid-bits", &size);
	if (prop && size == 4)
		mmu_lpid_bits = be32_to_cpup(prop);

	prop = of_get_flat_dt_prop(node, "ibm,mmu-pid-bits", &size);
	if (prop && size == 4)
		mmu_pid_bits = be32_to_cpup(prop);

	if (!mmu_pid_bits && !mmu_lpid_bits)
		return 0;

	return 1;
}

/*
 * Outside hotplug the kernel uses this value to map the kernel direct map
 * with radix. To be compatible with older kernels, let's keep this value
 * as 16M which is also SECTION_SIZE with SPARSEMEM. We can ideally map
 * things with 1GB size in the case where we don't support hotplug.
 */
#ifndef CONFIG_MEMORY_HOTPLUG
#define DEFAULT_MEMORY_BLOCK_SIZE	SZ_16M
#else
#define DEFAULT_MEMORY_BLOCK_SIZE	MIN_MEMORY_BLOCK_SIZE
#endif

static void update_memory_block_size(unsigned long *block_size, unsigned long mem_size)
{
	unsigned long min_memory_block_size = DEFAULT_MEMORY_BLOCK_SIZE;

	for (; *block_size > min_memory_block_size; *block_size >>= 2) {
		if ((mem_size & *block_size) == 0)
			break;
	}
}

static int __init probe_memory_block_size(unsigned long node, const char *uname, int
					  depth, void *data)
{
	const char *type;
	unsigned long *block_size = (unsigned long *)data;
	const __be32 *reg, *endp;
	int l;

	if (depth != 1)
		return 0;
	/*
	 * If we have dynamic-reconfiguration-memory node, use the
	 * lmb value.
	 */
	if (strcmp(uname, "ibm,dynamic-reconfiguration-memory") == 0) {

		const __be32 *prop;

		prop = of_get_flat_dt_prop(node, "ibm,lmb-size", &l);

		if (!prop || l < dt_root_size_cells * sizeof(__be32))
			/*
			 * Nothing in the device tree
			 */
			*block_size = DEFAULT_MEMORY_BLOCK_SIZE;
		else
			*block_size = of_read_number(prop, dt_root_size_cells);
		/*
		 * We have found the final value. Don't probe further.
		 */
		return 1;
	}
	/*
	 * Find all the device tree nodes of memory type and make sure
	 * the area can be mapped using the memory block size value
	 * we end up using. We start with 1G value and keep reducing
	 * it such that we can map the entire area using memory_block_size.
	 * This will be used on powernv and older pseries that don't
	 * have ibm,lmb-size node.
	 * For ex: with P5 we can end up with
	 * memory@0 -> 128MB
	 * memory@128M -> 64M
	 * This will end up using 64MB  memory block size value.
	 */
	type = of_get_flat_dt_prop(node, "device_type", NULL);
	if (type == NULL || strcmp(type, "memory") != 0)
		return 0;

	reg = of_get_flat_dt_prop(node, "linux,usable-memory", &l);
	if (!reg)
		reg = of_get_flat_dt_prop(node, "reg", &l);
	if (!reg)
		return 0;

	endp = reg + (l / sizeof(__be32));
	while ((endp - reg) >= (dt_root_addr_cells + dt_root_size_cells)) {
		const char *compatible;
		u64 size;

		dt_mem_next_cell(dt_root_addr_cells, &reg);
		size = dt_mem_next_cell(dt_root_size_cells, &reg);

		if (size) {
			update_memory_block_size(block_size, size);
			continue;
		}
		/*
		 * ibm,coherent-device-memory with linux,usable-memory = 0
		 * Force 256MiB block size. Work around for GPUs on P9 PowerNV
		 * linux,usable-memory == 0 implies driver managed memory and
		 * we can't use large memory block size due to hotplug/unplug
		 * limitations.
		 */
		compatible = of_get_flat_dt_prop(node, "compatible", NULL);
		if (compatible && !strcmp(compatible, "ibm,coherent-device-memory")) {
			if (*block_size > SZ_256M)
				*block_size = SZ_256M;
			/*
			 * We keep 256M as the upper limit with GPU present.
			 */
			return 0;
		}
	}
	/* continue looking for other memory device types */
	return 0;
}

/*
 * start with 1G memory block size. Early init will
 * fix this with correct value.
 */
unsigned long memory_block_size __ro_after_init = 1UL << 30;
static void __init early_init_memory_block_size(void)
{
	/*
	 * We need to do memory_block_size probe early so that
	 * radix__early_init_mmu() can use this as limit for
	 * mapping page size.
	 */
	of_scan_flat_dt(probe_memory_block_size, &memory_block_size);
}

void __init mmu_early_init_devtree(void)
{
	bool hvmode = !!(mfmsr() & MSR_HV);

	/* Disable radix mode based on kernel command line. */
	if (disable_radix) {
		if (IS_ENABLED(CONFIG_PPC_64S_HASH_MMU))
			cur_cpu_spec->mmu_features &= ~MMU_FTR_TYPE_RADIX;
		else
			pr_warn("WARNING: Ignoring cmdline option disable_radix\n");
	}

	of_scan_flat_dt(dt_scan_mmu_pid_width, NULL);
	if (hvmode && !mmu_lpid_bits) {
		if (early_cpu_has_feature(CPU_FTR_ARCH_207S))
			mmu_lpid_bits = 12; /* POWER8-10 */
		else
			mmu_lpid_bits = 10; /* POWER7 */
	}
	if (!mmu_pid_bits) {
		if (early_cpu_has_feature(CPU_FTR_ARCH_300))
			mmu_pid_bits = 20; /* POWER9-10 */
	}

	/*
	 * Check /chosen/ibm,architecture-vec-5 if running as a guest.
	 * When running bare-metal, we can use radix if we like
	 * even though the ibm,architecture-vec-5 property created by
	 * skiboot doesn't have the necessary bits set.
	 */
	if (!hvmode)
		early_check_vec5();

	early_init_memory_block_size();

	if (early_radix_enabled()) {
		radix__early_init_devtree();

		/*
		 * We have finalized the translation we are going to use by now.
		 * Radix mode is not limited by RMA / VRMA addressing.
		 * Hence don't limit memblock allocations.
		 */
		ppc64_rma_size = ULONG_MAX;
		memblock_set_current_limit(MEMBLOCK_ALLOC_ANYWHERE);
	} else
		hash__early_init_devtree();

	if (IS_ENABLED(CONFIG_HUGETLB_PAGE_SIZE_VARIABLE))
		hugetlbpage_init_defaultsize();

	if (!(cur_cpu_spec->mmu_features & MMU_FTR_HPTE_TABLE) &&
	    !(cur_cpu_spec->mmu_features & MMU_FTR_TYPE_RADIX))
		panic("kernel does not support any MMU type offered by platform");
}
#endif /* CONFIG_PPC_BOOK3S_64 */
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
