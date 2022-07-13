<<<<<<< HEAD
=======
// SPDX-License-Identifier: GPL-2.0
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
/*
 * linux/arch/m68k/mm/sun3mmu.c
 *
 * Implementations of mm routines specific to the sun3 MMU.
 *
 * Moved here 8/20/1999 Sam Creasey
 *
 */

#include <linux/signal.h>
#include <linux/sched.h>
#include <linux/mm.h>
#include <linux/swap.h>
#include <linux/kernel.h>
#include <linux/string.h>
#include <linux/types.h>
#include <linux/init.h>
<<<<<<< HEAD
#include <linux/bootmem.h>

#include <asm/setup.h>
#include <asm/uaccess.h>
#include <asm/page.h>
#include <asm/pgtable.h>
#include <asm/machdep.h>
#include <asm/io.h>

extern void mmu_emu_init (unsigned long bootmem_end);
=======
#include <linux/memblock.h>

#include <asm/setup.h>
#include <linux/uaccess.h>
#include <asm/page.h>
#include <asm/machdep.h>
#include <asm/io.h>

#include "../sun3/sun3.h"
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

const char bad_pmd_string[] = "Bad pmd in pte_alloc: %08lx\n";

extern unsigned long num_pages;

<<<<<<< HEAD
void free_initmem(void)
{
}

=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
/* For the sun3 we try to follow the i386 paging_init() more closely */
/* start_mem and end_mem have PAGE_OFFSET added already */
/* now sets up tables using sun3 PTEs rather than i386 as before. --m */
void __init paging_init(void)
{
	pgd_t * pg_dir;
	pte_t * pg_table;
	int i;
	unsigned long address;
	unsigned long next_pgtable;
	unsigned long bootmem_end;
<<<<<<< HEAD
	unsigned long zones_size[MAX_NR_ZONES] = { 0, };
	unsigned long size;

#ifdef TEST_VERIFY_AREA
	wp_works_ok = 0;
#endif
	empty_zero_page = alloc_bootmem_pages(PAGE_SIZE);
=======
	unsigned long max_zone_pfn[MAX_NR_ZONES] = { 0, };
	unsigned long size;

	empty_zero_page = memblock_alloc(PAGE_SIZE, PAGE_SIZE);
	if (!empty_zero_page)
		panic("%s: Failed to allocate %lu bytes align=0x%lx\n",
		      __func__, PAGE_SIZE, PAGE_SIZE);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	address = PAGE_OFFSET;
	pg_dir = swapper_pg_dir;
	memset (swapper_pg_dir, 0, sizeof (swapper_pg_dir));
	memset (kernel_pg_dir,  0, sizeof (kernel_pg_dir));

	size = num_pages * sizeof(pte_t);
	size = (size + PAGE_SIZE) & ~(PAGE_SIZE-1);

<<<<<<< HEAD
	next_pgtable = (unsigned long)alloc_bootmem_pages(size);
=======
	next_pgtable = (unsigned long)memblock_alloc(size, PAGE_SIZE);
	if (!next_pgtable)
		panic("%s: Failed to allocate %lu bytes align=0x%lx\n",
		      __func__, size, PAGE_SIZE);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	bootmem_end = (next_pgtable + size + PAGE_SIZE) & PAGE_MASK;

	/* Map whole memory from PAGE_OFFSET (0x0E000000) */
	pg_dir += PAGE_OFFSET >> PGDIR_SHIFT;

	while (address < (unsigned long)high_memory) {
		pg_table = (pte_t *) __pa (next_pgtable);
		next_pgtable += PTRS_PER_PTE * sizeof (pte_t);
		pgd_val(*pg_dir) = (unsigned long) pg_table;
		pg_dir++;

		/* now change pg_table to kernel virtual addresses */
		pg_table = (pte_t *) __va ((unsigned long) pg_table);
		for (i=0; i<PTRS_PER_PTE; ++i, ++pg_table) {
<<<<<<< HEAD
			pte_t pte = pfn_pte(virt_to_pfn(address), PAGE_INIT);
=======
			pte_t pte = pfn_pte(virt_to_pfn((void *)address), PAGE_INIT);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
			if (address >= (unsigned long)high_memory)
				pte_val (pte) = 0;
			set_pte (pg_table, pte);
			address += PAGE_SIZE;
		}
	}

	mmu_emu_init(bootmem_end);

	current->mm = NULL;

	/* memory sizing is a hack stolen from motorola.c..  hope it works for us */
<<<<<<< HEAD
	zones_size[ZONE_DMA] = ((unsigned long)high_memory - PAGE_OFFSET) >> PAGE_SHIFT;

	/* I really wish I knew why the following change made things better...  -- Sam */
/*	free_area_init(zones_size); */
	free_area_init_node(0, zones_size,
			    (__pa(PAGE_OFFSET) >> PAGE_SHIFT) + 1, NULL);
=======
	max_zone_pfn[ZONE_DMA] = ((unsigned long)high_memory) >> PAGE_SHIFT;

	/* I really wish I knew why the following change made things better...  -- Sam */
	free_area_init(max_zone_pfn);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)


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
