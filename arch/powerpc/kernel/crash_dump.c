<<<<<<< HEAD
=======
// SPDX-License-Identifier: GPL-2.0-only
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
/*
 * Routines for doing kexec-based kdump.
 *
 * Copyright (C) 2005, IBM Corp.
 *
 * Created by: Michael Ellerman
<<<<<<< HEAD
 *
 * This source code is licensed under the GNU General Public License,
 * Version 2.  See the file COPYING for more details.
=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 */

#undef DEBUG

#include <linux/crash_dump.h>
<<<<<<< HEAD
#include <linux/bootmem.h>
#include <linux/memblock.h>
#include <asm/code-patching.h>
#include <asm/kdump.h>
#include <asm/prom.h>
#include <asm/firmware.h>
#include <asm/uaccess.h>
#include <asm/rtas.h>
=======
#include <linux/io.h>
#include <linux/memblock.h>
#include <linux/of.h>
#include <asm/code-patching.h>
#include <asm/kdump.h>
#include <asm/firmware.h>
#include <linux/uio.h>
#include <asm/rtas.h>
#include <asm/inst.h>
#include <asm/fadump.h>
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

#ifdef DEBUG
#include <asm/udbg.h>
#define DBG(fmt...) udbg_printf(fmt)
#else
#define DBG(fmt...)
#endif

#ifndef CONFIG_NONSTATIC_KERNEL
void __init reserve_kdump_trampoline(void)
{
	memblock_reserve(0, KDUMP_RESERVE_LIMIT);
}

static void __init create_trampoline(unsigned long addr)
{
<<<<<<< HEAD
	unsigned int *p = (unsigned int *)addr;
=======
	u32 *p = (u32 *)addr;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	/* The maximum range of a single instruction branch, is the current
	 * instruction's address + (32 MB - 4) bytes. For the trampoline we
	 * need to branch to current address + 32 MB. So we insert a nop at
	 * the trampoline address, then the next instruction (+ 4 bytes)
	 * does a branch to (32 MB - 4). The net effect is that when we
	 * branch to "addr" we jump to ("addr" + 32 MB). Although it requires
	 * two instructions it doesn't require any registers.
	 */
<<<<<<< HEAD
	patch_instruction(p, PPC_INST_NOP);
	patch_branch(++p, addr + PHYSICAL_START, 0);
=======
	patch_instruction(p, ppc_inst(PPC_RAW_NOP()));
	patch_branch(p + 1, addr + PHYSICAL_START, 0);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

void __init setup_kdump_trampoline(void)
{
	unsigned long i;

	DBG(" -> setup_kdump_trampoline()\n");

	for (i = KDUMP_TRAMPOLINE_START; i < KDUMP_TRAMPOLINE_END; i += 8) {
		create_trampoline(i);
	}

#ifdef CONFIG_PPC_PSERIES
	create_trampoline(__pa(system_reset_fwnmi) - PHYSICAL_START);
	create_trampoline(__pa(machine_check_fwnmi) - PHYSICAL_START);
#endif /* CONFIG_PPC_PSERIES */

	DBG(" <- setup_kdump_trampoline()\n");
}
#endif /* CONFIG_NONSTATIC_KERNEL */

<<<<<<< HEAD
static int __init parse_savemaxmem(char *p)
{
	if (p)
		saved_max_pfn = (memparse(p, &p) >> PAGE_SHIFT) - 1;

	return 1;
}
__setup("savemaxmem=", parse_savemaxmem);


static size_t copy_oldmem_vaddr(void *vaddr, char *buf, size_t csize,
                               unsigned long offset, int userbuf)
{
	if (userbuf) {
		if (copy_to_user((char __user *)buf, (vaddr + offset), csize))
			return -EFAULT;
	} else
		memcpy(buf, (vaddr + offset), csize);

	return csize;
}

/**
 * copy_oldmem_page - copy one page from "oldmem"
 * @pfn: page frame number to be copied
 * @buf: target memory address for the copy; this can be in kernel address
 *      space or user address space (see @userbuf)
 * @csize: number of bytes to copy
 * @offset: offset in bytes into the page (based on pfn) to begin the copy
 * @userbuf: if set, @buf is in user address space, use copy_to_user(),
 *      otherwise @buf is in kernel address space, use memcpy().
 *
 * Copy a page from "oldmem". For this page, there is no pte mapped
 * in the current kernel. We stitch up a pte, similar to kmap_atomic.
 */
ssize_t copy_oldmem_page(unsigned long pfn, char *buf,
			size_t csize, unsigned long offset, int userbuf)
=======
ssize_t copy_oldmem_page(struct iov_iter *iter, unsigned long pfn,
			size_t csize, unsigned long offset)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
{
	void  *vaddr;
	phys_addr_t paddr;

	if (!csize)
		return 0;

	csize = min_t(size_t, csize, PAGE_SIZE);
	paddr = pfn << PAGE_SHIFT;

	if (memblock_is_region_memory(paddr, csize)) {
		vaddr = __va(paddr);
<<<<<<< HEAD
		csize = copy_oldmem_vaddr(vaddr, buf, csize, offset, userbuf);
	} else {
		vaddr = __ioremap(paddr, PAGE_SIZE, 0);
		csize = copy_oldmem_vaddr(vaddr, buf, csize, offset, userbuf);
=======
		csize = copy_to_iter(vaddr + offset, csize, iter);
	} else {
		vaddr = ioremap_cache(paddr, PAGE_SIZE);
		csize = copy_to_iter(vaddr + offset, csize, iter);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		iounmap(vaddr);
	}

	return csize;
}

<<<<<<< HEAD
=======
/*
 * Return true only when kexec based kernel dump capturing method is used.
 * This ensures all restritions applied for kdump case are not automatically
 * applied for fadump case.
 */
bool is_kdump_kernel(void)
{
	return !is_fadump_active() && elfcorehdr_addr != ELFCORE_ADDR_MAX;
}
EXPORT_SYMBOL_GPL(is_kdump_kernel);

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#ifdef CONFIG_PPC_RTAS
/*
 * The crashkernel region will almost always overlap the RTAS region, so
 * we have to be careful when shrinking the crashkernel region.
 */
void crash_free_reserved_phys_range(unsigned long begin, unsigned long end)
{
	unsigned long addr;
<<<<<<< HEAD
	const u32 *basep, *sizep;
=======
	const __be32 *basep, *sizep;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	unsigned int rtas_start = 0, rtas_end = 0;

	basep = of_get_property(rtas.dev, "linux,rtas-base", NULL);
	sizep = of_get_property(rtas.dev, "rtas-size", NULL);

	if (basep && sizep) {
<<<<<<< HEAD
		rtas_start = *basep;
		rtas_end = *basep + *sizep;
=======
		rtas_start = be32_to_cpup(basep);
		rtas_end = rtas_start + be32_to_cpup(sizep);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	}

	for (addr = begin; addr < end; addr += PAGE_SIZE) {
		/* Does this page overlap with the RTAS region? */
		if (addr <= rtas_end && ((addr + PAGE_SIZE) > rtas_start))
			continue;

<<<<<<< HEAD
		ClearPageReserved(pfn_to_page(addr >> PAGE_SHIFT));
		init_page_count(pfn_to_page(addr >> PAGE_SHIFT));
		free_page((unsigned long)__va(addr));
		totalram_pages++;
=======
		free_reserved_page(pfn_to_page(addr >> PAGE_SHIFT));
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	}
}
#endif
