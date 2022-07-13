<<<<<<< HEAD
=======
// SPDX-License-Identifier: GPL-2.0
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
/*
 *  linux/arch/parisc/mm/init.c
 *
 *  Copyright (C) 1995	Linus Torvalds
 *  Copyright 1999 SuSE GmbH
 *    changed by Philipp Rumpf
 *  Copyright 1999 Philipp Rumpf (prumpf@tux.org)
 *  Copyright 2004 Randolph Chung (tausq@debian.org)
 *  Copyright 2006-2007 Helge Deller (deller@gmx.de)
 *
 */


#include <linux/module.h>
#include <linux/mm.h>
<<<<<<< HEAD
#include <linux/bootmem.h>
#include <linux/gfp.h>
#include <linux/delay.h>
#include <linux/init.h>
#include <linux/pci.h>		/* for hppa_dma_ops and pcxl_dma_ops */
=======
#include <linux/memblock.h>
#include <linux/gfp.h>
#include <linux/delay.h>
#include <linux/init.h>
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#include <linux/initrd.h>
#include <linux/swap.h>
#include <linux/unistd.h>
#include <linux/nodemask.h>	/* for node_online_map */
<<<<<<< HEAD
#include <linux/pagemap.h>	/* for release_pages and page_cache_release */

#include <asm/pgalloc.h>
#include <asm/pgtable.h>
=======
#include <linux/pagemap.h>	/* for release_pages */
#include <linux/compat.h>

#include <asm/pgalloc.h>
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#include <asm/tlb.h>
#include <asm/pdc_chassis.h>
#include <asm/mmzone.h>
#include <asm/sections.h>
<<<<<<< HEAD

extern int  data_start;

#ifdef CONFIG_DISCONTIGMEM
struct node_map_data node_data[MAX_NUMNODES] __read_mostly;
unsigned char pfnnid_map[PFNNID_MAP_MAX] __read_mostly;
#endif

static struct resource data_resource = {
	.name	= "Kernel data",
	.flags	= IORESOURCE_BUSY | IORESOURCE_MEM,
=======
#include <asm/msgbuf.h>
#include <asm/sparsemem.h>
#include <asm/asm-offsets.h>
#include <asm/shmbuf.h>

extern int  data_start;
extern void parisc_kernel_start(void);	/* Kernel entry point in head.S */

#if CONFIG_PGTABLE_LEVELS == 3
pmd_t pmd0[PTRS_PER_PMD] __section(".data..vm0.pmd") __attribute__ ((aligned(PAGE_SIZE)));
#endif

pgd_t swapper_pg_dir[PTRS_PER_PGD] __section(".data..vm0.pgd") __attribute__ ((aligned(PAGE_SIZE)));
pte_t pg0[PT_INITIAL * PTRS_PER_PTE] __section(".data..vm0.pte") __attribute__ ((aligned(PAGE_SIZE)));

static struct resource data_resource = {
	.name	= "Kernel data",
	.flags	= IORESOURCE_BUSY | IORESOURCE_SYSTEM_RAM,
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
};

static struct resource code_resource = {
	.name	= "Kernel code",
<<<<<<< HEAD
	.flags	= IORESOURCE_BUSY | IORESOURCE_MEM,
=======
	.flags	= IORESOURCE_BUSY | IORESOURCE_SYSTEM_RAM,
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
};

static struct resource pdcdata_resource = {
	.name	= "PDC data (Page Zero)",
	.start	= 0,
	.end	= 0x9ff,
	.flags	= IORESOURCE_BUSY | IORESOURCE_MEM,
};

<<<<<<< HEAD
static struct resource sysram_resources[MAX_PHYSMEM_RANGES] __read_mostly;
=======
static struct resource sysram_resources[MAX_PHYSMEM_RANGES] __ro_after_init;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

/* The following array is initialized from the firmware specific
 * information retrieved in kernel/inventory.c.
 */

<<<<<<< HEAD
physmem_range_t pmem_ranges[MAX_PHYSMEM_RANGES] __read_mostly;
int npmem_ranges __read_mostly;

#ifdef CONFIG_64BIT
#define MAX_MEM         (~0UL)
=======
physmem_range_t pmem_ranges[MAX_PHYSMEM_RANGES] __initdata;
int npmem_ranges __initdata;

#ifdef CONFIG_64BIT
#define MAX_MEM         (1UL << MAX_PHYSMEM_BITS)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#else /* !CONFIG_64BIT */
#define MAX_MEM         (3584U*1024U*1024U)
#endif /* !CONFIG_64BIT */

static unsigned long mem_limit __read_mostly = MAX_MEM;

static void __init mem_limit_func(void)
{
	char *cp, *end;
	unsigned long limit;

	/* We need this before __setup() functions are called */

	limit = MAX_MEM;
	for (cp = boot_command_line; *cp; ) {
		if (memcmp(cp, "mem=", 4) == 0) {
			cp += 4;
			limit = memparse(cp, &end);
			if (end != cp)
				break;
			cp = end;
		} else {
			while (*cp != ' ' && *cp)
				++cp;
			while (*cp == ' ')
				++cp;
		}
	}

	if (limit < mem_limit)
		mem_limit = limit;
}

#define MAX_GAP (0x40000000UL >> PAGE_SHIFT)

static void __init setup_bootmem(void)
{
<<<<<<< HEAD
	unsigned long bootmap_size;
	unsigned long mem_max;
	unsigned long bootmap_pages;
	unsigned long bootmap_start_pfn;
	unsigned long bootmap_pfn;
#ifndef CONFIG_DISCONTIGMEM
=======
	unsigned long mem_max;
#ifndef CONFIG_SPARSEMEM
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	physmem_range_t pmem_holes[MAX_PHYSMEM_RANGES - 1];
	int npmem_holes;
#endif
	int i, sysram_resource_count;

	disable_sr_hashing(); /* Turn off space register hashing */

	/*
	 * Sort the ranges. Since the number of ranges is typically
	 * small, and performance is not an issue here, just do
	 * a simple insertion sort.
	 */

	for (i = 1; i < npmem_ranges; i++) {
		int j;

		for (j = i; j > 0; j--) {
<<<<<<< HEAD
			unsigned long tmp;

=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
			if (pmem_ranges[j-1].start_pfn <
			    pmem_ranges[j].start_pfn) {

				break;
			}
<<<<<<< HEAD
			tmp = pmem_ranges[j-1].start_pfn;
			pmem_ranges[j-1].start_pfn = pmem_ranges[j].start_pfn;
			pmem_ranges[j].start_pfn = tmp;
			tmp = pmem_ranges[j-1].pages;
			pmem_ranges[j-1].pages = pmem_ranges[j].pages;
			pmem_ranges[j].pages = tmp;
		}
	}

#ifndef CONFIG_DISCONTIGMEM
=======
			swap(pmem_ranges[j-1], pmem_ranges[j]);
		}
	}

#ifndef CONFIG_SPARSEMEM
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	/*
	 * Throw out ranges that are too far apart (controlled by
	 * MAX_GAP).
	 */

	for (i = 1; i < npmem_ranges; i++) {
		if (pmem_ranges[i].start_pfn -
			(pmem_ranges[i-1].start_pfn +
			 pmem_ranges[i-1].pages) > MAX_GAP) {
			npmem_ranges = i;
			printk("Large gap in memory detected (%ld pages). "
<<<<<<< HEAD
			       "Consider turning on CONFIG_DISCONTIGMEM\n",
=======
			       "Consider turning on CONFIG_SPARSEMEM\n",
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
			       pmem_ranges[i].start_pfn -
			       (pmem_ranges[i-1].start_pfn +
			        pmem_ranges[i-1].pages));
			break;
		}
	}
#endif

<<<<<<< HEAD
	if (npmem_ranges > 1) {

		/* Print the memory ranges */

		printk(KERN_INFO "Memory Ranges:\n");

		for (i = 0; i < npmem_ranges; i++) {
			unsigned long start;
			unsigned long size;

			size = (pmem_ranges[i].pages << PAGE_SHIFT);
			start = (pmem_ranges[i].start_pfn << PAGE_SHIFT);
			printk(KERN_INFO "%2d) Start 0x%016lx End 0x%016lx Size %6ld MB\n",
				i,start, start + (size - 1), size >> 20);
		}
	}

	sysram_resource_count = npmem_ranges;
	for (i = 0; i < sysram_resource_count; i++) {
		struct resource *res = &sysram_resources[i];
		res->name = "System RAM";
		res->start = pmem_ranges[i].start_pfn << PAGE_SHIFT;
		res->end = res->start + (pmem_ranges[i].pages << PAGE_SHIFT)-1;
		res->flags = IORESOURCE_MEM | IORESOURCE_BUSY;
		request_resource(&iomem_resource, res);
	}
=======
	/* Print the memory ranges */
	pr_info("Memory Ranges:\n");

	for (i = 0; i < npmem_ranges; i++) {
		struct resource *res = &sysram_resources[i];
		unsigned long start;
		unsigned long size;

		size = (pmem_ranges[i].pages << PAGE_SHIFT);
		start = (pmem_ranges[i].start_pfn << PAGE_SHIFT);
		pr_info("%2d) Start 0x%016lx End 0x%016lx Size %6ld MB\n",
			i, start, start + (size - 1), size >> 20);

		/* request memory resource */
		res->name = "System RAM";
		res->start = start;
		res->end = start + size - 1;
		res->flags = IORESOURCE_SYSTEM_RAM | IORESOURCE_BUSY;
		request_resource(&iomem_resource, res);
	}

	sysram_resource_count = npmem_ranges;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	/*
	 * For 32 bit kernels we limit the amount of memory we can
	 * support, in order to preserve enough kernel address space
	 * for other purposes. For 64 bit kernels we don't normally
	 * limit the memory, but this mechanism can be used to
	 * artificially limit the amount of memory (and it is written
	 * to work with multiple memory ranges).
	 */

	mem_limit_func();       /* check for "mem=" argument */

	mem_max = 0;
<<<<<<< HEAD
	num_physpages = 0;
=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	for (i = 0; i < npmem_ranges; i++) {
		unsigned long rsize;

		rsize = pmem_ranges[i].pages << PAGE_SHIFT;
		if ((mem_max + rsize) > mem_limit) {
			printk(KERN_WARNING "Memory truncated to %ld MB\n", mem_limit >> 20);
			if (mem_max == mem_limit)
				npmem_ranges = i;
			else {
				pmem_ranges[i].pages =   (mem_limit >> PAGE_SHIFT)
						       - (mem_max >> PAGE_SHIFT);
				npmem_ranges = i + 1;
				mem_max = mem_limit;
			}
<<<<<<< HEAD
	        num_physpages += pmem_ranges[i].pages;
			break;
		}
	    num_physpages += pmem_ranges[i].pages;
=======
			break;
		}
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		mem_max += rsize;
	}

	printk(KERN_INFO "Total Memory: %ld MB\n",mem_max >> 20);

<<<<<<< HEAD
#ifndef CONFIG_DISCONTIGMEM
	/* Merge the ranges, keeping track of the holes */

=======
#ifndef CONFIG_SPARSEMEM
	/* Merge the ranges, keeping track of the holes */
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	{
		unsigned long end_pfn;
		unsigned long hole_pages;

		npmem_holes = 0;
		end_pfn = pmem_ranges[0].start_pfn + pmem_ranges[0].pages;
		for (i = 1; i < npmem_ranges; i++) {

			hole_pages = pmem_ranges[i].start_pfn - end_pfn;
			if (hole_pages) {
				pmem_holes[npmem_holes].start_pfn = end_pfn;
				pmem_holes[npmem_holes++].pages = hole_pages;
				end_pfn += hole_pages;
			}
			end_pfn += pmem_ranges[i].pages;
		}

		pmem_ranges[0].pages = end_pfn - pmem_ranges[0].start_pfn;
		npmem_ranges = 1;
	}
#endif

<<<<<<< HEAD
	bootmap_pages = 0;
	for (i = 0; i < npmem_ranges; i++)
		bootmap_pages += bootmem_bootmap_pages(pmem_ranges[i].pages);

	bootmap_start_pfn = PAGE_ALIGN(__pa((unsigned long) &_end)) >> PAGE_SHIFT;

#ifdef CONFIG_DISCONTIGMEM
	for (i = 0; i < MAX_PHYSMEM_RANGES; i++) {
		memset(NODE_DATA(i), 0, sizeof(pg_data_t));
		NODE_DATA(i)->bdata = &bootmem_node_data[i];
	}
	memset(pfnnid_map, 0xff, sizeof(pfnnid_map));

	for (i = 0; i < npmem_ranges; i++) {
		node_set_state(i, N_NORMAL_MEMORY);
		node_set_online(i);
	}
#endif

	/*
	 * Initialize and free the full range of memory in each range.
	 * Note that the only writing these routines do are to the bootmap,
	 * and we've made sure to locate the bootmap properly so that they
	 * won't be writing over anything important.
	 */

	bootmap_pfn = bootmap_start_pfn;
=======
	/*
	 * Initialize and free the full range of memory in each range.
	 */

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	max_pfn = 0;
	for (i = 0; i < npmem_ranges; i++) {
		unsigned long start_pfn;
		unsigned long npages;
<<<<<<< HEAD
=======
		unsigned long start;
		unsigned long size;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

		start_pfn = pmem_ranges[i].start_pfn;
		npages = pmem_ranges[i].pages;

<<<<<<< HEAD
		bootmap_size = init_bootmem_node(NODE_DATA(i),
						bootmap_pfn,
						start_pfn,
						(start_pfn + npages) );
		free_bootmem_node(NODE_DATA(i),
				  (start_pfn << PAGE_SHIFT),
				  (npages << PAGE_SHIFT) );
		bootmap_pfn += (bootmap_size + PAGE_SIZE - 1) >> PAGE_SHIFT;
=======
		start = start_pfn << PAGE_SHIFT;
		size = npages << PAGE_SHIFT;

		/* add system RAM memblock */
		memblock_add(start, size);

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		if ((start_pfn + npages) > max_pfn)
			max_pfn = start_pfn + npages;
	}

<<<<<<< HEAD
=======
	/*
	 * We can't use memblock top-down allocations because we only
	 * created the initial mapping up to KERNEL_INITIAL_SIZE in
	 * the assembly bootup code.
	 */
	memblock_set_bottom_up(true);

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	/* IOMMU is always used to access "high mem" on those boxes
	 * that can support enough mem that a PCI device couldn't
	 * directly DMA to any physical addresses.
	 * ISA DMA support will need to revisit this.
	 */
	max_low_pfn = max_pfn;

<<<<<<< HEAD
	/* bootmap sizing messed up? */
	BUG_ON((bootmap_pfn - bootmap_start_pfn) != bootmap_pages);

=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	/* reserve PAGE0 pdc memory, kernel text/data/bss & bootmap */

#define PDC_CONSOLE_IO_IODC_SIZE 32768

<<<<<<< HEAD
	reserve_bootmem_node(NODE_DATA(0), 0UL,
			(unsigned long)(PAGE0->mem_free +
				PDC_CONSOLE_IO_IODC_SIZE), BOOTMEM_DEFAULT);
	reserve_bootmem_node(NODE_DATA(0), __pa((unsigned long)_text),
			(unsigned long)(_end - _text), BOOTMEM_DEFAULT);
	reserve_bootmem_node(NODE_DATA(0), (bootmap_start_pfn << PAGE_SHIFT),
			((bootmap_pfn - bootmap_start_pfn) << PAGE_SHIFT),
			BOOTMEM_DEFAULT);

#ifndef CONFIG_DISCONTIGMEM
=======
	memblock_reserve(0UL, (unsigned long)(PAGE0->mem_free +
				PDC_CONSOLE_IO_IODC_SIZE));
	memblock_reserve(__pa(KERNEL_BINARY_TEXT_START),
			(unsigned long)(_end - KERNEL_BINARY_TEXT_START));

#ifndef CONFIG_SPARSEMEM
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	/* reserve the holes */

	for (i = 0; i < npmem_holes; i++) {
<<<<<<< HEAD
		reserve_bootmem_node(NODE_DATA(0),
				(pmem_holes[i].start_pfn << PAGE_SHIFT),
				(pmem_holes[i].pages << PAGE_SHIFT),
				BOOTMEM_DEFAULT);
=======
		memblock_reserve((pmem_holes[i].start_pfn << PAGE_SHIFT),
				(pmem_holes[i].pages << PAGE_SHIFT));
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	}
#endif

#ifdef CONFIG_BLK_DEV_INITRD
	if (initrd_start) {
		printk(KERN_INFO "initrd: %08lx-%08lx\n", initrd_start, initrd_end);
		if (__pa(initrd_start) < mem_max) {
			unsigned long initrd_reserve;

			if (__pa(initrd_end) > mem_max) {
				initrd_reserve = mem_max - __pa(initrd_start);
			} else {
				initrd_reserve = initrd_end - initrd_start;
			}
			initrd_below_start_ok = 1;
			printk(KERN_INFO "initrd: reserving %08lx-%08lx (mem_max %08lx)\n", __pa(initrd_start), __pa(initrd_start) + initrd_reserve, mem_max);

<<<<<<< HEAD
			reserve_bootmem_node(NODE_DATA(0), __pa(initrd_start),
					initrd_reserve, BOOTMEM_DEFAULT);
=======
			memblock_reserve(__pa(initrd_start), initrd_reserve);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		}
	}
#endif

	data_resource.start =  virt_to_phys(&data_start);
	data_resource.end = virt_to_phys(_end) - 1;
	code_resource.start = virt_to_phys(_text);
	code_resource.end = virt_to_phys(&data_start)-1;

	/* We don't know which region the kernel will be in, so try
	 * all of them.
	 */
	for (i = 0; i < sysram_resource_count; i++) {
		struct resource *res = &sysram_resources[i];
		request_resource(res, &code_resource);
		request_resource(res, &data_resource);
	}
	request_resource(&sysram_resources[0], &pdcdata_resource);
<<<<<<< HEAD
}

static void __init map_pages(unsigned long start_vaddr,
			     unsigned long start_paddr, unsigned long size,
			     pgprot_t pgprot, int force)
{
	pgd_t *pg_dir;
=======

	/* Initialize Page Deallocation Table (PDT) and check for bad memory. */
	pdc_pdt_init();

	memblock_allow_resize();
	memblock_dump_all();
}

static bool kernel_set_to_readonly;

static void __ref map_pages(unsigned long start_vaddr,
			    unsigned long start_paddr, unsigned long size,
			    pgprot_t pgprot, int force)
{
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	pmd_t *pmd;
	pte_t *pg_table;
	unsigned long end_paddr;
	unsigned long start_pmd;
	unsigned long start_pte;
	unsigned long tmp1;
	unsigned long tmp2;
	unsigned long address;
	unsigned long vaddr;
	unsigned long ro_start;
	unsigned long ro_end;
<<<<<<< HEAD
	unsigned long fv_addr;
	unsigned long gw_addr;
	extern const unsigned long fault_vector_20;
	extern void * const linux_gateway_page;

	ro_start = __pa((unsigned long)_text);
	ro_end   = __pa((unsigned long)&data_start);
	fv_addr  = __pa((unsigned long)&fault_vector_20) & PAGE_MASK;
	gw_addr  = __pa((unsigned long)&linux_gateway_page) & PAGE_MASK;

	end_paddr = start_paddr + size;

	pg_dir = pgd_offset_k(start_vaddr);

#if PTRS_PER_PMD == 1
	start_pmd = 0;
#else
	start_pmd = ((start_vaddr >> PMD_SHIFT) & (PTRS_PER_PMD - 1));
#endif
=======
	unsigned long kernel_start, kernel_end;

	ro_start = __pa((unsigned long)_text);
	ro_end   = __pa((unsigned long)&data_start);
	kernel_start = __pa((unsigned long)&__init_begin);
	kernel_end  = __pa((unsigned long)&_end);

	end_paddr = start_paddr + size;

	/* for 2-level configuration PTRS_PER_PMD is 0 so start_pmd will be 0 */
	start_pmd = ((start_vaddr >> PMD_SHIFT) & (PTRS_PER_PMD - 1));
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	start_pte = ((start_vaddr >> PAGE_SHIFT) & (PTRS_PER_PTE - 1));

	address = start_paddr;
	vaddr = start_vaddr;
	while (address < end_paddr) {
<<<<<<< HEAD
#if PTRS_PER_PMD == 1
		pmd = (pmd_t *)__pa(pg_dir);
#else
		pmd = (pmd_t *)pgd_address(*pg_dir);

		/*
		 * pmd is physical at this point
		 */

		if (!pmd) {
			pmd = (pmd_t *) alloc_bootmem_low_pages_node(NODE_DATA(0), PAGE_SIZE << PMD_ORDER);
			pmd = (pmd_t *) __pa(pmd);
		}

		pgd_populate(NULL, pg_dir, __va(pmd));
#endif
		pg_dir++;

		/* now change pmd to kernel virtual addresses */

		pmd = (pmd_t *)__va(pmd) + start_pmd;
		for (tmp1 = start_pmd; tmp1 < PTRS_PER_PMD; tmp1++, pmd++) {

			/*
			 * pg_table is physical at this point
			 */

			pg_table = (pte_t *)pmd_address(*pmd);
			if (!pg_table) {
				pg_table = (pte_t *)
					alloc_bootmem_low_pages_node(NODE_DATA(0), PAGE_SIZE);
				pg_table = (pte_t *) __pa(pg_table);
			}

			pmd_populate_kernel(NULL, pmd, __va(pg_table));

			/* now change pg_table to kernel virtual addresses */

			pg_table = (pte_t *) __va(pg_table) + start_pte;
			for (tmp2 = start_pte; tmp2 < PTRS_PER_PTE; tmp2++, pg_table++) {
				pte_t pte;

				/*
				 * Map the fault vector writable so we can
				 * write the HPMC checksum.
				 */
				if (force)
					pte =  __mk_pte(address, pgprot);
				else if (core_kernel_text(vaddr) &&
					 address != fv_addr)
					pte = __mk_pte(address, PAGE_KERNEL_EXEC);
				else
#if defined(CONFIG_PARISC_PAGE_SIZE_4KB)
				if (address >= ro_start && address < ro_end
							&& address != fv_addr
							&& address != gw_addr)
					pte = __mk_pte(address, PAGE_KERNEL_RO);
				else
#endif
					pte = __mk_pte(address, pgprot);

				if (address >= end_paddr) {
					if (force)
						break;
					else
						pte_val(pte) = 0;
				}

=======
		pgd_t *pgd = pgd_offset_k(vaddr);
		p4d_t *p4d = p4d_offset(pgd, vaddr);
		pud_t *pud = pud_offset(p4d, vaddr);

#if CONFIG_PGTABLE_LEVELS == 3
		if (pud_none(*pud)) {
			pmd = memblock_alloc(PAGE_SIZE << PMD_TABLE_ORDER,
					     PAGE_SIZE << PMD_TABLE_ORDER);
			if (!pmd)
				panic("pmd allocation failed.\n");
			pud_populate(NULL, pud, pmd);
		}
#endif

		pmd = pmd_offset(pud, vaddr);
		for (tmp1 = start_pmd; tmp1 < PTRS_PER_PMD; tmp1++, pmd++) {
			if (pmd_none(*pmd)) {
				pg_table = memblock_alloc(PAGE_SIZE, PAGE_SIZE);
				if (!pg_table)
					panic("page table allocation failed\n");
				pmd_populate_kernel(NULL, pmd, pg_table);
			}

			pg_table = pte_offset_kernel(pmd, vaddr);
			for (tmp2 = start_pte; tmp2 < PTRS_PER_PTE; tmp2++, pg_table++) {
				pte_t pte;
				pgprot_t prot;
				bool huge = false;

				if (force) {
					prot = pgprot;
				} else if (address < kernel_start || address >= kernel_end) {
					/* outside kernel memory */
					prot = PAGE_KERNEL;
				} else if (!kernel_set_to_readonly) {
					/* still initializing, allow writing to RO memory */
					prot = PAGE_KERNEL_RWX;
					huge = true;
				} else if (address >= ro_start) {
					/* Code (ro) and Data areas */
					prot = (address < ro_end) ?
						PAGE_KERNEL_EXEC : PAGE_KERNEL;
					huge = true;
				} else {
					prot = PAGE_KERNEL;
				}

				pte = __mk_pte(address, prot);
				if (huge)
					pte = pte_mkhuge(pte);

				if (address >= end_paddr)
					break;

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
				set_pte(pg_table, pte);

				address += PAGE_SIZE;
				vaddr += PAGE_SIZE;
			}
			start_pte = 0;

			if (address >= end_paddr)
			    break;
		}
		start_pmd = 0;
	}
}

<<<<<<< HEAD
void free_initmem(void)
{
	unsigned long addr;
	unsigned long init_begin = (unsigned long)__init_begin;
	unsigned long init_end = (unsigned long)__init_end;
=======
void __init set_kernel_text_rw(int enable_read_write)
{
	unsigned long start = (unsigned long) __init_begin;
	unsigned long end   = (unsigned long) &data_start;

	map_pages(start, __pa(start), end-start,
		PAGE_KERNEL_RWX, enable_read_write ? 1:0);

	/* force the kernel to see the new page table entries */
	flush_cache_all();
	flush_tlb_all();
}

void free_initmem(void)
{
	unsigned long init_begin = (unsigned long)__init_begin;
	unsigned long init_end = (unsigned long)__init_end;
	unsigned long kernel_end  = (unsigned long)&_end;

	/* Remap kernel text and data, but do not touch init section yet. */
	kernel_set_to_readonly = true;
	map_pages(init_end, __pa(init_end), kernel_end - init_end,
		  PAGE_KERNEL, 0);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	/* The init text pages are marked R-X.  We have to
	 * flush the icache and mark them RW-
	 *
<<<<<<< HEAD
	 * This is tricky, because map_pages is in the init section.
=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	 * Do a dummy remap of the data section first (the data
	 * section is already PAGE_KERNEL) to pull in the TLB entries
	 * for map_kernel */
	map_pages(init_begin, __pa(init_begin), init_end - init_begin,
		  PAGE_KERNEL_RWX, 1);
	/* now remap at PAGE_KERNEL since the TLB is pre-primed to execute
	 * map_pages */
	map_pages(init_begin, __pa(init_begin), init_end - init_begin,
		  PAGE_KERNEL, 1);

	/* force the kernel to see the new TLB entries */
<<<<<<< HEAD
	__flush_tlb_range(0, init_begin, init_end);
	/* Attempt to catch anyone trying to execute code here
	 * by filling the page with BRK insns.
	 */
	memset((void *)init_begin, 0x00, init_end - init_begin);
=======
	__flush_tlb_range(0, init_begin, kernel_end);

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	/* finally dump all the instructions which were cached, since the
	 * pages are no-longer executable */
	flush_icache_range(init_begin, init_end);
	
<<<<<<< HEAD
	for (addr = init_begin; addr < init_end; addr += PAGE_SIZE) {
		ClearPageReserved(virt_to_page(addr));
		init_page_count(virt_to_page(addr));
		free_page(addr);
		num_physpages++;
		totalram_pages++;
	}

	/* set up a new led state on systems shipped LED State panel */
	pdc_chassis_send_status(PDC_CHASSIS_DIRECT_BCOMPLETE);
	
	printk(KERN_INFO "Freeing unused kernel memory: %luk freed\n",
		(init_end - init_begin) >> 10);
}


#ifdef CONFIG_DEBUG_RODATA
=======
	free_initmem_default(POISON_FREE_INITMEM);

	/* set up a new led state on systems shipped LED State panel */
	pdc_chassis_send_status(PDC_CHASSIS_DIRECT_BCOMPLETE);
}


#ifdef CONFIG_STRICT_KERNEL_RWX
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
void mark_rodata_ro(void)
{
	/* rodata memory was already mapped with KERNEL_RO access rights by
           pagetable_init() and map_pages(). No need to do additional stuff here */
<<<<<<< HEAD
	printk (KERN_INFO "Write protecting the kernel read-only data: %luk\n",
		(unsigned long)(__end_rodata - __start_rodata) >> 10);
=======
	unsigned long roai_size = __end_ro_after_init - __start_ro_after_init;

	pr_info("Write protected read-only-after-init data: %luk\n", roai_size >> 10);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}
#endif


/*
 * Just an arbitrary offset to serve as a "hole" between mapping areas
 * (between top of physical memory and a potential pcxl dma mapping
 * area, and below the vmalloc mapping area).
 *
 * The current 32K value just means that there will be a 32K "hole"
 * between mapping areas. That means that  any out-of-bounds memory
 * accesses will hopefully be caught. The vmalloc() routines leaves
 * a hole of 4kB between each vmalloced area for the same reason.
 */

 /* Leave room for gateway page expansion */
#if KERNEL_MAP_START < GATEWAY_PAGE_SIZE
#error KERNEL_MAP_START is in gateway reserved region
#endif
#define MAP_START (KERNEL_MAP_START)

#define VM_MAP_OFFSET  (32*1024)
#define SET_MAP_OFFSET(x) ((void *)(((unsigned long)(x) + VM_MAP_OFFSET) \
				     & ~(VM_MAP_OFFSET-1)))

<<<<<<< HEAD
void *parisc_vmalloc_start __read_mostly;
EXPORT_SYMBOL(parisc_vmalloc_start);

#ifdef CONFIG_PA11
unsigned long pcxl_dma_start __read_mostly;
#endif

void __init mem_init(void)
{
	int codesize, reservedpages, datasize, initsize;
=======
void *parisc_vmalloc_start __ro_after_init;
EXPORT_SYMBOL(parisc_vmalloc_start);

void __init mem_init(void)
{
	/* Do sanity checks on IPC (compat) structures */
	BUILD_BUG_ON(sizeof(struct ipc64_perm) != 48);
#ifndef CONFIG_64BIT
	BUILD_BUG_ON(sizeof(struct semid64_ds) != 80);
	BUILD_BUG_ON(sizeof(struct msqid64_ds) != 104);
	BUILD_BUG_ON(sizeof(struct shmid64_ds) != 104);
#endif
#ifdef CONFIG_COMPAT
	BUILD_BUG_ON(sizeof(struct compat_ipc64_perm) != sizeof(struct ipc64_perm));
	BUILD_BUG_ON(sizeof(struct compat_semid64_ds) != 80);
	BUILD_BUG_ON(sizeof(struct compat_msqid64_ds) != 104);
	BUILD_BUG_ON(sizeof(struct compat_shmid64_ds) != 104);
#endif
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	/* Do sanity checks on page table constants */
	BUILD_BUG_ON(PTE_ENTRY_SIZE != sizeof(pte_t));
	BUILD_BUG_ON(PMD_ENTRY_SIZE != sizeof(pmd_t));
	BUILD_BUG_ON(PGD_ENTRY_SIZE != sizeof(pgd_t));
	BUILD_BUG_ON(PAGE_SHIFT + BITS_PER_PTE + BITS_PER_PMD + BITS_PER_PGD
			> BITS_PER_LONG);
<<<<<<< HEAD

	high_memory = __va((max_pfn << PAGE_SHIFT));

#ifndef CONFIG_DISCONTIGMEM
	max_mapnr = page_to_pfn(virt_to_page(high_memory - 1)) + 1;
	totalram_pages += free_all_bootmem();
#else
	{
		int i;

		for (i = 0; i < npmem_ranges; i++)
			totalram_pages += free_all_bootmem_node(NODE_DATA(i));
	}
#endif

	codesize = (unsigned long)_etext - (unsigned long)_text;
	datasize = (unsigned long)_edata - (unsigned long)_etext;
	initsize = (unsigned long)__init_end - (unsigned long)__init_begin;

	reservedpages = 0;
{
	unsigned long pfn;
#ifdef CONFIG_DISCONTIGMEM
	int i;

	for (i = 0; i < npmem_ranges; i++) {
		for (pfn = node_start_pfn(i); pfn < node_end_pfn(i); pfn++) {
			if (PageReserved(pfn_to_page(pfn)))
				reservedpages++;
		}
	}
#else /* !CONFIG_DISCONTIGMEM */
	for (pfn = 0; pfn < max_pfn; pfn++) {
		/*
		 * Only count reserved RAM pages
		 */
		if (PageReserved(pfn_to_page(pfn)))
			reservedpages++;
	}
#endif
}

#ifdef CONFIG_PA11
	if (hppa_dma_ops == &pcxl_dma_ops) {
		pcxl_dma_start = (unsigned long)SET_MAP_OFFSET(MAP_START);
		parisc_vmalloc_start = SET_MAP_OFFSET(pcxl_dma_start
						+ PCXL_DMA_MAP_SIZE);
	} else {
		pcxl_dma_start = 0;
		parisc_vmalloc_start = SET_MAP_OFFSET(MAP_START);
	}
#else
	parisc_vmalloc_start = SET_MAP_OFFSET(MAP_START);
#endif

	printk(KERN_INFO "Memory: %luk/%luk available (%dk kernel code, %dk reserved, %dk data, %dk init)\n",
		nr_free_pages() << (PAGE_SHIFT-10),
		num_physpages << (PAGE_SHIFT-10),
		codesize >> 10,
		reservedpages << (PAGE_SHIFT-10),
		datasize >> 10,
		initsize >> 10
	);

#ifdef CONFIG_DEBUG_KERNEL /* double-sanity-check paranoia */
	printk("virtual kernel memory layout:\n"
	       "    vmalloc : 0x%p - 0x%p   (%4ld MB)\n"
	       "    memory  : 0x%p - 0x%p   (%4ld MB)\n"
	       "      .init : 0x%p - 0x%p   (%4ld kB)\n"
	       "      .data : 0x%p - 0x%p   (%4ld kB)\n"
	       "      .text : 0x%p - 0x%p   (%4ld kB)\n",
=======
#if CONFIG_PGTABLE_LEVELS == 3
	BUILD_BUG_ON(PT_INITIAL > PTRS_PER_PMD);
#else
	BUILD_BUG_ON(PT_INITIAL > PTRS_PER_PGD);
#endif

#ifdef CONFIG_64BIT
	/* avoid ldil_%L() asm statements to sign-extend into upper 32-bits */
	BUILD_BUG_ON(__PAGE_OFFSET >= 0x80000000);
	BUILD_BUG_ON(TMPALIAS_MAP_START >= 0x80000000);
#endif

	high_memory = __va((max_pfn << PAGE_SHIFT));
	set_max_mapnr(max_low_pfn);
	memblock_free_all();

#ifdef CONFIG_PA11
	if (boot_cpu_data.cpu_type == pcxl2 || boot_cpu_data.cpu_type == pcxl) {
		pcxl_dma_start = (unsigned long)SET_MAP_OFFSET(MAP_START);
		parisc_vmalloc_start = SET_MAP_OFFSET(pcxl_dma_start
						+ PCXL_DMA_MAP_SIZE);
	} else
#endif
		parisc_vmalloc_start = SET_MAP_OFFSET(MAP_START);

#if 0
	/*
	 * Do not expose the virtual kernel memory layout to userspace.
	 * But keep code for debugging purposes.
	 */
	printk("virtual kernel memory layout:\n"
	       "     vmalloc : 0x%px - 0x%px   (%4ld MB)\n"
	       "     fixmap  : 0x%px - 0x%px   (%4ld kB)\n"
	       "     memory  : 0x%px - 0x%px   (%4ld MB)\n"
	       "       .init : 0x%px - 0x%px   (%4ld kB)\n"
	       "       .data : 0x%px - 0x%px   (%4ld kB)\n"
	       "       .text : 0x%px - 0x%px   (%4ld kB)\n",
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	       (void*)VMALLOC_START, (void*)VMALLOC_END,
	       (VMALLOC_END - VMALLOC_START) >> 20,

<<<<<<< HEAD
=======
	       (void *)FIXMAP_START, (void *)(FIXMAP_START + FIXMAP_SIZE),
	       (unsigned long)(FIXMAP_SIZE / 1024),

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	       __va(0), high_memory,
	       ((unsigned long)high_memory - (unsigned long)__va(0)) >> 20,

	       __init_begin, __init_end,
	       ((unsigned long)__init_end - (unsigned long)__init_begin) >> 10,

	       _etext, _edata,
	       ((unsigned long)_edata - (unsigned long)_etext) >> 10,

	       _text, _etext,
	       ((unsigned long)_etext - (unsigned long)_text) >> 10);
#endif
}

<<<<<<< HEAD
unsigned long *empty_zero_page __read_mostly;
EXPORT_SYMBOL(empty_zero_page);

void show_mem(unsigned int filter)
{
	int i,free = 0,total = 0,reserved = 0;
	int shared = 0, cached = 0;

	printk(KERN_INFO "Mem-info:\n");
	show_free_areas(filter);
	if (filter & SHOW_MEM_FILTER_PAGE_COUNT)
		return;
#ifndef CONFIG_DISCONTIGMEM
	i = max_mapnr;
	while (i-- > 0) {
		total++;
		if (PageReserved(mem_map+i))
			reserved++;
		else if (PageSwapCache(mem_map+i))
			cached++;
		else if (!page_count(&mem_map[i]))
			free++;
		else
			shared += page_count(&mem_map[i]) - 1;
	}
#else
	for (i = 0; i < npmem_ranges; i++) {
		int j;

		for (j = node_start_pfn(i); j < node_end_pfn(i); j++) {
			struct page *p;
			unsigned long flags;

			pgdat_resize_lock(NODE_DATA(i), &flags);
			p = nid_page_nr(i, j) - node_start_pfn(i);

			total++;
			if (PageReserved(p))
				reserved++;
			else if (PageSwapCache(p))
				cached++;
			else if (!page_count(p))
				free++;
			else
				shared += page_count(p) - 1;
			pgdat_resize_unlock(NODE_DATA(i), &flags);
        	}
	}
#endif
	printk(KERN_INFO "%d pages of RAM\n", total);
	printk(KERN_INFO "%d reserved pages\n", reserved);
	printk(KERN_INFO "%d pages shared\n", shared);
	printk(KERN_INFO "%d pages swap cached\n", cached);


#ifdef CONFIG_DISCONTIGMEM
	{
		struct zonelist *zl;
		int i, j;

		for (i = 0; i < npmem_ranges; i++) {
			zl = node_zonelist(i, 0);
			for (j = 0; j < MAX_NR_ZONES; j++) {
				struct zoneref *z;
				struct zone *zone;

				printk("Zone list for zone %d on node %d: ", j, i);
				for_each_zone_zonelist(zone, z, zl, j)
					printk("[%d/%s] ", zone_to_nid(zone),
								zone->name);
				printk("\n");
			}
		}
	}
#endif
}

=======
unsigned long *empty_zero_page __ro_after_init;
EXPORT_SYMBOL(empty_zero_page);

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
/*
 * pagetable_init() sets up the page tables
 *
 * Note that gateway_init() places the Linux gateway page at page 0.
 * Since gateway pages cannot be dereferenced this has the desirable
 * side effect of trapping those pesky NULL-reference errors in the
 * kernel.
 */
static void __init pagetable_init(void)
{
	int range;

	/* Map each physical memory range to its kernel vaddr */

	for (range = 0; range < npmem_ranges; range++) {
		unsigned long start_paddr;
<<<<<<< HEAD
		unsigned long end_paddr;
		unsigned long size;

		start_paddr = pmem_ranges[range].start_pfn << PAGE_SHIFT;
		end_paddr = start_paddr + (pmem_ranges[range].pages << PAGE_SHIFT);
=======
		unsigned long size;

		start_paddr = pmem_ranges[range].start_pfn << PAGE_SHIFT;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		size = pmem_ranges[range].pages << PAGE_SHIFT;

		map_pages((unsigned long)__va(start_paddr), start_paddr,
			  size, PAGE_KERNEL, 0);
	}

#ifdef CONFIG_BLK_DEV_INITRD
	if (initrd_end && initrd_end > mem_limit) {
		printk(KERN_INFO "initrd: mapping %08lx-%08lx\n", initrd_start, initrd_end);
		map_pages(initrd_start, __pa(initrd_start),
			  initrd_end - initrd_start, PAGE_KERNEL, 0);
	}
#endif

<<<<<<< HEAD
	empty_zero_page = alloc_bootmem_pages(PAGE_SIZE);
	memset(empty_zero_page, 0, PAGE_SIZE);
=======
	empty_zero_page = memblock_alloc(PAGE_SIZE, PAGE_SIZE);
	if (!empty_zero_page)
		panic("zero page allocation failed.\n");

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

static void __init gateway_init(void)
{
	unsigned long linux_gateway_page_addr;
	/* FIXME: This is 'const' in order to trick the compiler
	   into not treating it as DP-relative data. */
	extern void * const linux_gateway_page;

	linux_gateway_page_addr = LINUX_GATEWAY_ADDR & PAGE_MASK;

	/*
	 * Setup Linux Gateway page.
	 *
	 * The Linux gateway page will reside in kernel space (on virtual
	 * page 0), so it doesn't need to be aliased into user space.
	 */

	map_pages(linux_gateway_page_addr, __pa(&linux_gateway_page),
		  PAGE_SIZE, PAGE_GATEWAY, 1);
}

<<<<<<< HEAD
#ifdef CONFIG_HPUX
void
map_hpux_gateway_page(struct task_struct *tsk, struct mm_struct *mm)
{
	pgd_t *pg_dir;
	pmd_t *pmd;
	pte_t *pg_table;
	unsigned long start_pmd;
	unsigned long start_pte;
	unsigned long address;
	unsigned long hpux_gw_page_addr;
	/* FIXME: This is 'const' in order to trick the compiler
	   into not treating it as DP-relative data. */
	extern void * const hpux_gateway_page;

	hpux_gw_page_addr = HPUX_GATEWAY_ADDR & PAGE_MASK;

	/*
	 * Setup HP-UX Gateway page.
	 *
	 * The HP-UX gateway page resides in the user address space,
	 * so it needs to be aliased into each process.
	 */

	pg_dir = pgd_offset(mm,hpux_gw_page_addr);

#if PTRS_PER_PMD == 1
	start_pmd = 0;
#else
	start_pmd = ((hpux_gw_page_addr >> PMD_SHIFT) & (PTRS_PER_PMD - 1));
#endif
	start_pte = ((hpux_gw_page_addr >> PAGE_SHIFT) & (PTRS_PER_PTE - 1));

	address = __pa(&hpux_gateway_page);
#if PTRS_PER_PMD == 1
	pmd = (pmd_t *)__pa(pg_dir);
#else
	pmd = (pmd_t *) pgd_address(*pg_dir);

	/*
	 * pmd is physical at this point
	 */

	if (!pmd) {
		pmd = (pmd_t *) get_zeroed_page(GFP_KERNEL);
		pmd = (pmd_t *) __pa(pmd);
	}

	__pgd_val_set(*pg_dir, PxD_FLAG_PRESENT | PxD_FLAG_VALID | (unsigned long) pmd);
#endif
	/* now change pmd to kernel virtual addresses */

	pmd = (pmd_t *)__va(pmd) + start_pmd;

	/*
	 * pg_table is physical at this point
	 */

	pg_table = (pte_t *) pmd_address(*pmd);
	if (!pg_table)
		pg_table = (pte_t *) __pa(get_zeroed_page(GFP_KERNEL));

	__pmd_val_set(*pmd, PxD_FLAG_PRESENT | PxD_FLAG_VALID | (unsigned long) pg_table);

	/* now change pg_table to kernel virtual addresses */

	pg_table = (pte_t *) __va(pg_table) + start_pte;
	set_pte(pg_table, __mk_pte(address, PAGE_GATEWAY));
}
EXPORT_SYMBOL(map_hpux_gateway_page);
#endif

void __init paging_init(void)
{
	int i;

	setup_bootmem();
	pagetable_init();
	gateway_init();
	flush_cache_all_local(); /* start with known state */
	flush_tlb_all_local(NULL);

	for (i = 0; i < npmem_ranges; i++) {
		unsigned long zones_size[MAX_NR_ZONES] = { 0, };

		zones_size[ZONE_NORMAL] = pmem_ranges[i].pages;

#ifdef CONFIG_DISCONTIGMEM
		/* Need to initialize the pfnnid_map before we can initialize
		   the zone */
		{
		    int j;
		    for (j = (pmem_ranges[i].start_pfn >> PFNNID_SHIFT);
			 j <= ((pmem_ranges[i].start_pfn + pmem_ranges[i].pages) >> PFNNID_SHIFT);
			 j++) {
			pfnnid_map[j] = i;
		    }
		}
#endif

		free_area_init_node(i, zones_size,
				pmem_ranges[i].start_pfn, NULL);
	}
}

=======
static void __init fixmap_init(void)
{
	unsigned long addr = FIXMAP_START;
	unsigned long end = FIXMAP_START + FIXMAP_SIZE;
	pgd_t *pgd = pgd_offset_k(addr);
	p4d_t *p4d = p4d_offset(pgd, addr);
	pud_t *pud = pud_offset(p4d, addr);
	pmd_t *pmd;

	BUILD_BUG_ON(FIXMAP_SIZE > PMD_SIZE);

#if CONFIG_PGTABLE_LEVELS == 3
	if (pud_none(*pud)) {
		pmd = memblock_alloc(PAGE_SIZE << PMD_TABLE_ORDER,
				     PAGE_SIZE << PMD_TABLE_ORDER);
		if (!pmd)
			panic("fixmap: pmd allocation failed.\n");
		pud_populate(NULL, pud, pmd);
	}
#endif

	pmd = pmd_offset(pud, addr);
	do {
		pte_t *pte = memblock_alloc(PAGE_SIZE, PAGE_SIZE);
		if (!pte)
			panic("fixmap: pte allocation failed.\n");

		pmd_populate_kernel(&init_mm, pmd, pte);

		addr += PAGE_SIZE;
	} while (addr < end);
}

static void __init parisc_bootmem_free(void)
{
	unsigned long max_zone_pfn[MAX_NR_ZONES] = { 0, };

	max_zone_pfn[0] = memblock_end_of_DRAM();

	free_area_init(max_zone_pfn);
}

void __init paging_init(void)
{
	setup_bootmem();
	pagetable_init();
	gateway_init();
	fixmap_init();
	flush_cache_all_local(); /* start with known state */
	flush_tlb_all_local(NULL);

	sparse_init();
	parisc_bootmem_free();
}

static void alloc_btlb(unsigned long start, unsigned long end, int *slot,
			unsigned long entry_info)
{
	const int slot_max = btlb_info.fixed_range_info.num_comb;
	int min_num_pages = btlb_info.min_size;
	unsigned long size;

	/* map at minimum 4 pages */
	if (min_num_pages < 4)
		min_num_pages = 4;

	size = HUGEPAGE_SIZE;
	while (start < end && *slot < slot_max && size >= PAGE_SIZE) {
		/* starting address must have same alignment as size! */
		/* if correctly aligned and fits in double size, increase */
		if (((start & (2 * size - 1)) == 0) &&
		    (end - start) >= (2 * size)) {
			size <<= 1;
			continue;
		}
		/* if current size alignment is too big, try smaller size */
		if ((start & (size - 1)) != 0) {
			size >>= 1;
			continue;
		}
		if ((end - start) >= size) {
			if ((size >> PAGE_SHIFT) >= min_num_pages)
				pdc_btlb_insert(start >> PAGE_SHIFT, __pa(start) >> PAGE_SHIFT,
					size >> PAGE_SHIFT, entry_info, *slot);
			(*slot)++;
			start += size;
			continue;
		}
		size /= 2;
		continue;
	}
}

void btlb_init_per_cpu(void)
{
	unsigned long s, t, e;
	int slot;

	/* BTLBs are not available on 64-bit CPUs */
	if (IS_ENABLED(CONFIG_PA20))
		return;
	else if (pdc_btlb_info(&btlb_info) < 0) {
		memset(&btlb_info, 0, sizeof btlb_info);
	}

	/* insert BLTLBs for code and data segments */
	s = (uintptr_t) dereference_function_descriptor(&_stext);
	e = (uintptr_t) dereference_function_descriptor(&_etext);
	t = (uintptr_t) dereference_function_descriptor(&_sdata);
	BUG_ON(t != e);

	/* code segments */
	slot = 0;
	alloc_btlb(s, e, &slot, 0x13800000);

	/* sanity check */
	t = (uintptr_t) dereference_function_descriptor(&_edata);
	e = (uintptr_t) dereference_function_descriptor(&__bss_start);
	BUG_ON(t != e);

	/* data segments */
	s = (uintptr_t) dereference_function_descriptor(&_sdata);
	e = (uintptr_t) dereference_function_descriptor(&__bss_stop);
	alloc_btlb(s, e, &slot, 0x11800000);
}

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#ifdef CONFIG_PA20

/*
 * Currently, all PA20 chips have 18 bit protection IDs, which is the
 * limiting factor (space ids are 32 bits).
 */

#define NR_SPACE_IDS 262144

#else

/*
 * Currently we have a one-to-one relationship between space IDs and
 * protection IDs. Older parisc chips (PCXS, PCXT, PCXL, PCXL2) only
 * support 15 bit protection IDs, so that is the limiting factor.
 * PCXT' has 18 bit protection IDs, but only 16 bit spaceids, so it's
 * probably not worth the effort for a special case here.
 */

#define NR_SPACE_IDS 32768

#endif  /* !CONFIG_PA20 */

#define RECYCLE_THRESHOLD (NR_SPACE_IDS / 2)
#define SID_ARRAY_SIZE  (NR_SPACE_IDS / (8 * sizeof(long)))

static unsigned long space_id[SID_ARRAY_SIZE] = { 1 }; /* disallow space 0 */
static unsigned long dirty_space_id[SID_ARRAY_SIZE];
static unsigned long space_id_index;
static unsigned long free_space_ids = NR_SPACE_IDS - 1;
<<<<<<< HEAD
static unsigned long dirty_space_ids = 0;
=======
static unsigned long dirty_space_ids;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

static DEFINE_SPINLOCK(sid_lock);

unsigned long alloc_sid(void)
{
	unsigned long index;

	spin_lock(&sid_lock);

	if (free_space_ids == 0) {
		if (dirty_space_ids != 0) {
			spin_unlock(&sid_lock);
			flush_tlb_all(); /* flush_tlb_all() calls recycle_sids() */
			spin_lock(&sid_lock);
		}
		BUG_ON(free_space_ids == 0);
	}

	free_space_ids--;

	index = find_next_zero_bit(space_id, NR_SPACE_IDS, space_id_index);
<<<<<<< HEAD
	space_id[index >> SHIFT_PER_LONG] |= (1L << (index & (BITS_PER_LONG - 1)));
=======
	space_id[BIT_WORD(index)] |= BIT_MASK(index);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	space_id_index = index;

	spin_unlock(&sid_lock);

	return index << SPACEID_SHIFT;
}

void free_sid(unsigned long spaceid)
{
	unsigned long index = spaceid >> SPACEID_SHIFT;
<<<<<<< HEAD
	unsigned long *dirty_space_offset;

	dirty_space_offset = dirty_space_id + (index >> SHIFT_PER_LONG);
	index &= (BITS_PER_LONG - 1);

	spin_lock(&sid_lock);

	BUG_ON(*dirty_space_offset & (1L << index)); /* attempt to free space id twice */

	*dirty_space_offset |= (1L << index);
=======
	unsigned long *dirty_space_offset, mask;

	dirty_space_offset = &dirty_space_id[BIT_WORD(index)];
	mask = BIT_MASK(index);

	spin_lock(&sid_lock);

	BUG_ON(*dirty_space_offset & mask); /* attempt to free space id twice */

	*dirty_space_offset |= mask;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	dirty_space_ids++;

	spin_unlock(&sid_lock);
}


#ifdef CONFIG_SMP
static void get_dirty_sids(unsigned long *ndirtyptr,unsigned long *dirty_array)
{
	int i;

	/* NOTE: sid_lock must be held upon entry */

	*ndirtyptr = dirty_space_ids;
	if (dirty_space_ids != 0) {
	    for (i = 0; i < SID_ARRAY_SIZE; i++) {
		dirty_array[i] = dirty_space_id[i];
		dirty_space_id[i] = 0;
	    }
	    dirty_space_ids = 0;
	}

	return;
}

static void recycle_sids(unsigned long ndirty,unsigned long *dirty_array)
{
	int i;

	/* NOTE: sid_lock must be held upon entry */

	if (ndirty != 0) {
		for (i = 0; i < SID_ARRAY_SIZE; i++) {
			space_id[i] ^= dirty_array[i];
		}

		free_space_ids += ndirty;
		space_id_index = 0;
	}
}

#else /* CONFIG_SMP */

static void recycle_sids(void)
{
	int i;

	/* NOTE: sid_lock must be held upon entry */

	if (dirty_space_ids != 0) {
		for (i = 0; i < SID_ARRAY_SIZE; i++) {
			space_id[i] ^= dirty_space_id[i];
			dirty_space_id[i] = 0;
		}

		free_space_ids += dirty_space_ids;
		dirty_space_ids = 0;
		space_id_index = 0;
	}
}
#endif

/*
 * flush_tlb_all() calls recycle_sids(), since whenever the entire tlb is
 * purged, we can safely reuse the space ids that were released but
 * not flushed from the tlb.
 */

#ifdef CONFIG_SMP

static unsigned long recycle_ndirty;
static unsigned long recycle_dirty_array[SID_ARRAY_SIZE];
static unsigned int recycle_inuse;

void flush_tlb_all(void)
{
	int do_recycle;

	do_recycle = 0;
	spin_lock(&sid_lock);
<<<<<<< HEAD
=======
	__inc_irq_stat(irq_tlb_count);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	if (dirty_space_ids > RECYCLE_THRESHOLD) {
	    BUG_ON(recycle_inuse);  /* FIXME: Use a semaphore/wait queue here */
	    get_dirty_sids(&recycle_ndirty,recycle_dirty_array);
	    recycle_inuse++;
	    do_recycle++;
	}
	spin_unlock(&sid_lock);
	on_each_cpu(flush_tlb_all_local, NULL, 1);
	if (do_recycle) {
	    spin_lock(&sid_lock);
	    recycle_sids(recycle_ndirty,recycle_dirty_array);
	    recycle_inuse = 0;
	    spin_unlock(&sid_lock);
	}
}
#else
void flush_tlb_all(void)
{
	spin_lock(&sid_lock);
<<<<<<< HEAD
=======
	__inc_irq_stat(irq_tlb_count);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	flush_tlb_all_local(NULL);
	recycle_sids();
	spin_unlock(&sid_lock);
}
#endif

<<<<<<< HEAD
#ifdef CONFIG_BLK_DEV_INITRD
void free_initrd_mem(unsigned long start, unsigned long end)
{
	if (start >= end)
		return;
	printk(KERN_INFO "Freeing initrd memory: %ldk freed\n", (end - start) >> 10);
	for (; start < end; start += PAGE_SIZE) {
		ClearPageReserved(virt_to_page(start));
		init_page_count(virt_to_page(start));
		free_page(start);
		num_physpages++;
		totalram_pages++;
	}
}
#endif
=======
static const pgprot_t protection_map[16] = {
	[VM_NONE]					= PAGE_NONE,
	[VM_READ]					= PAGE_READONLY,
	[VM_WRITE]					= PAGE_NONE,
	[VM_WRITE | VM_READ]				= PAGE_READONLY,
	[VM_EXEC]					= PAGE_EXECREAD,
	[VM_EXEC | VM_READ]				= PAGE_EXECREAD,
	[VM_EXEC | VM_WRITE]				= PAGE_EXECREAD,
	[VM_EXEC | VM_WRITE | VM_READ]			= PAGE_EXECREAD,
	[VM_SHARED]					= PAGE_NONE,
	[VM_SHARED | VM_READ]				= PAGE_READONLY,
	[VM_SHARED | VM_WRITE]				= PAGE_WRITEONLY,
	[VM_SHARED | VM_WRITE | VM_READ]		= PAGE_SHARED,
	[VM_SHARED | VM_EXEC]				= PAGE_EXECREAD,
	[VM_SHARED | VM_EXEC | VM_READ]			= PAGE_EXECREAD,
	[VM_SHARED | VM_EXEC | VM_WRITE]		= PAGE_RWX,
	[VM_SHARED | VM_EXEC | VM_WRITE | VM_READ]	= PAGE_RWX
};
DECLARE_VM_GET_PAGE_PROT
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
