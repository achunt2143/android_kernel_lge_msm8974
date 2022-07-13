<<<<<<< HEAD
=======
// SPDX-License-Identifier: GPL-2.0
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
/*
 * linux/arch/m68k/sun3/dvma.c
 *
 * Written by Sam Creasey
 *
 * Sun3 IOMMU routines used for dvma accesses.
 *
 */

<<<<<<< HEAD
#include <linux/kernel.h>
#include <linux/mm.h>
#include <linux/bootmem.h>
#include <linux/list.h>
#include <asm/page.h>
#include <asm/pgtable.h>
=======
#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/mm.h>
#include <linux/memblock.h>
#include <linux/list.h>
#include <asm/page.h>
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#include <asm/sun3mmu.h>
#include <asm/dvma.h>


static unsigned long ptelist[120];

static unsigned long dvma_page(unsigned long kaddr, unsigned long vaddr)
{
	unsigned long pte;
	unsigned long j;
	pte_t ptep;

	j = *(volatile unsigned long *)kaddr;
	*(volatile unsigned long *)kaddr = j;

<<<<<<< HEAD
	ptep = pfn_pte(virt_to_pfn(kaddr), PAGE_KERNEL);
	pte = pte_val(ptep);
//		printk("dvma_remap: addr %lx -> %lx pte %08lx len %x\n",
//		       kaddr, vaddr, pte, len);
=======
	ptep = pfn_pte(virt_to_pfn((void *)kaddr), PAGE_KERNEL);
	pte = pte_val(ptep);
//	pr_info("dvma_remap: addr %lx -> %lx pte %08lx\n", kaddr, vaddr, pte);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	if(ptelist[(vaddr & 0xff000) >> PAGE_SHIFT] != pte) {
		sun3_put_pte(vaddr, pte);
		ptelist[(vaddr & 0xff000) >> PAGE_SHIFT] = pte;
	}

	return (vaddr + (kaddr & ~PAGE_MASK));

}

int dvma_map_iommu(unsigned long kaddr, unsigned long baddr,
			      int len)
{

	unsigned long end;
	unsigned long vaddr;

	vaddr = dvma_btov(baddr);

	end = vaddr + len;

	while(vaddr < end) {
		dvma_page(kaddr, vaddr);
		kaddr += PAGE_SIZE;
		vaddr += PAGE_SIZE;
	}

	return 0;

}

<<<<<<< HEAD
void sun3_dvma_init(void)
{

	memset(ptelist, 0, sizeof(ptelist));


=======
void __init sun3_dvma_init(void)
{
	memset(ptelist, 0, sizeof(ptelist));
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}
