/*
 * fixmap.h: compile-time virtual memory allocation
 *
 * This file is subject to the terms and conditions of the GNU General Public
 * License.  See the file "COPYING" in the main directory of this archive
 * for more details.
 *
 * Copyright (C) 1998 Ingo Molnar
 *
 * Copyright 2008 Freescale Semiconductor Inc.
 *   Port to powerpc added by Kumar Gala
 */

#ifndef _ASM_FIXMAP_H
#define _ASM_FIXMAP_H

#ifndef __ASSEMBLY__
<<<<<<< HEAD
#include <linux/kernel.h>
#include <asm/page.h>
#ifdef CONFIG_HIGHMEM
#include <linux/threads.h>
#include <asm/kmap_types.h>
#endif

#define FIXADDR_TOP	((unsigned long)(-PAGE_SIZE))

=======
#include <linux/sizes.h>
#include <linux/pgtable.h>
#include <asm/page.h>
#ifdef CONFIG_HIGHMEM
#include <linux/threads.h>
#include <asm/kmap_size.h>
#endif

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
/*
 * Here we define all the compile-time 'special' virtual
 * addresses. The point is to have a constant address at
 * compile time, but to set the physical address only
 * in the boot process. We allocate these special addresses
 * from the end of virtual memory (0xfffff000) backwards.
 * Also this lets us do fail-safe vmalloc(), we
 * can guarantee that these special addresses and
 * vmalloc()-ed addresses never overlap.
 *
 * these 'compile-time allocated' memory buffers are
 * fixed-size 4k pages. (or larger if used with an increment
 * highger than 1) use fixmap_set(idx,phys) to associate
 * physical memory with fixmap indices.
 *
 * TLB entries of such buffers will not be flushed across
 * task switches.
 */
enum fixed_addresses {
	FIX_HOLE,
<<<<<<< HEAD
	/* reserve the top 128K for early debugging purposes */
	FIX_EARLY_DEBUG_TOP = FIX_HOLE,
	FIX_EARLY_DEBUG_BASE = FIX_EARLY_DEBUG_TOP+((128*1024)/PAGE_SIZE)-1,
#ifdef CONFIG_HIGHMEM
	FIX_KMAP_BEGIN,	/* reserved pte's for temporary kernel mappings */
	FIX_KMAP_END = FIX_KMAP_BEGIN+(KM_TYPE_NR*NR_CPUS)-1,
#endif
	/* FIX_PCIE_MCFG, */
	__end_of_fixed_addresses
};

extern void __set_fixmap (enum fixed_addresses idx,
					phys_addr_t phys, pgprot_t flags);

#define set_fixmap(idx, phys) \
		__set_fixmap(idx, phys, PAGE_KERNEL)
/*
 * Some hardware wants to get fixmapped without caching.
 */
#define set_fixmap_nocache(idx, phys) \
		__set_fixmap(idx, phys, PAGE_KERNEL_NCG)

#define clear_fixmap(idx) \
		__set_fixmap(idx, 0, __pgprot(0))

#define __FIXADDR_SIZE	(__end_of_fixed_addresses << PAGE_SHIFT)
#define FIXADDR_START		(FIXADDR_TOP - __FIXADDR_SIZE)

#define __fix_to_virt(x)	(FIXADDR_TOP - ((x) << PAGE_SHIFT))
#define __virt_to_fix(x)	((FIXADDR_TOP - ((x)&PAGE_MASK)) >> PAGE_SHIFT)

extern void __this_fixmap_does_not_exist(void);

/*
 * 'index to address' translation. If anyone tries to use the idx
 * directly without tranlation, we catch the bug with a NULL-deference
 * kernel oops. Illegal ranges of incoming indices are caught too.
 */
static __always_inline unsigned long fix_to_virt(const unsigned int idx)
{
	/*
	 * this branch gets completely eliminated after inlining,
	 * except when someone tries to use fixaddr indices in an
	 * illegal way. (such as mixing up address types or using
	 * out-of-range indices).
	 *
	 * If it doesn't get removed, the linker will complain
	 * loudly with a reasonably clear error message..
	 */
	if (idx >= __end_of_fixed_addresses)
		__this_fixmap_does_not_exist();

        return __fix_to_virt(idx);
}

static inline unsigned long virt_to_fix(const unsigned long vaddr)
{
	BUG_ON(vaddr >= FIXADDR_TOP || vaddr < FIXADDR_START);
	return __virt_to_fix(vaddr);
}
=======
#ifdef CONFIG_PPC32
	/* reserve the top 128K for early debugging purposes */
	FIX_EARLY_DEBUG_TOP = FIX_HOLE,
	FIX_EARLY_DEBUG_BASE = FIX_EARLY_DEBUG_TOP+(ALIGN(SZ_128K, PAGE_SIZE)/PAGE_SIZE)-1,
#ifdef CONFIG_HIGHMEM
	FIX_KMAP_BEGIN,	/* reserved pte's for temporary kernel mappings */
	FIX_KMAP_END = FIX_KMAP_BEGIN + (KM_MAX_IDX * NR_CPUS) - 1,
#endif
#ifdef CONFIG_PPC_8xx
	/* For IMMR we need an aligned 512K area */
#define FIX_IMMR_SIZE	(512 * 1024 / PAGE_SIZE)
	FIX_IMMR_START,
	FIX_IMMR_BASE = __ALIGN_MASK(FIX_IMMR_START, FIX_IMMR_SIZE - 1) - 1 +
		       FIX_IMMR_SIZE,
#endif
#ifdef CONFIG_PPC_83xx
	/* For IMMR we need an aligned 2M area */
#define FIX_IMMR_SIZE	(SZ_2M / PAGE_SIZE)
	FIX_IMMR_START,
	FIX_IMMR_BASE = __ALIGN_MASK(FIX_IMMR_START, FIX_IMMR_SIZE - 1) - 1 +
		       FIX_IMMR_SIZE,
#endif
	/* FIX_PCIE_MCFG, */
#endif /* CONFIG_PPC32 */
	__end_of_permanent_fixed_addresses,

#define NR_FIX_BTMAPS		(SZ_256K / PAGE_SIZE)
#define FIX_BTMAPS_SLOTS	16
#define TOTAL_FIX_BTMAPS	(NR_FIX_BTMAPS * FIX_BTMAPS_SLOTS)

	FIX_BTMAP_END = __end_of_permanent_fixed_addresses,
	FIX_BTMAP_BEGIN = FIX_BTMAP_END + TOTAL_FIX_BTMAPS - 1,
	__end_of_fixed_addresses
};

#define __FIXADDR_SIZE	(__end_of_fixed_addresses << PAGE_SHIFT)
#define FIXADDR_START		(FIXADDR_TOP - __FIXADDR_SIZE)

#define FIXMAP_ALIGNED_SIZE	(ALIGN(FIXADDR_TOP, PGDIR_SIZE) - \
				 ALIGN_DOWN(FIXADDR_START, PGDIR_SIZE))
#define FIXMAP_PTE_SIZE	(FIXMAP_ALIGNED_SIZE / PGDIR_SIZE * PTE_TABLE_SIZE)

#define FIXMAP_PAGE_NOCACHE PAGE_KERNEL_NCG
#define FIXMAP_PAGE_IO	PAGE_KERNEL_NCG

#include <asm-generic/fixmap.h>

static inline void __set_fixmap(enum fixed_addresses idx,
				phys_addr_t phys, pgprot_t flags)
{
	BUILD_BUG_ON(IS_ENABLED(CONFIG_PPC64) && __FIXADDR_SIZE > FIXADDR_SIZE);

	if (__builtin_constant_p(idx))
		BUILD_BUG_ON(idx >= __end_of_fixed_addresses);
	else if (WARN_ON(idx >= __end_of_fixed_addresses))
		return;
	if (pgprot_val(flags))
		map_kernel_page(__fix_to_virt(idx), phys, flags);
	else
		unmap_kernel_page(__fix_to_virt(idx));
}

#define __early_set_fixmap	__set_fixmap

#ifdef CONFIG_PPC_8xx
#define VIRT_IMMR_BASE (__fix_to_virt(FIX_IMMR_BASE))
#endif
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

#endif /* !__ASSEMBLY__ */
#endif
