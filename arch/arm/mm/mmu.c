<<<<<<< HEAD
=======
// SPDX-License-Identifier: GPL-2.0-only
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
/*
 *  linux/arch/arm/mm/mmu.c
 *
 *  Copyright (C) 1995-2005 Russell King
<<<<<<< HEAD
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 */
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/errno.h>
#include <linux/init.h>
#include <linux/mman.h>
#include <linux/nodemask.h>
#include <linux/memblock.h>
#include <linux/fs.h>
#include <linux/vmalloc.h>
<<<<<<< HEAD

#include <asm/cp15.h>
#include <asm/cputype.h>
#include <asm/sections.h>
#include <asm/cachetype.h>
#include <asm/setup.h>
#include <asm/sizes.h>
#include <asm/smp_plat.h>
=======
#include <linux/sizes.h>

#include <asm/cp15.h>
#include <asm/cputype.h>
#include <asm/cachetype.h>
#include <asm/sections.h>
#include <asm/setup.h>
#include <asm/smp_plat.h>
#include <asm/tcm.h>
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#include <asm/tlb.h>
#include <asm/highmem.h>
#include <asm/system_info.h>
#include <asm/traps.h>
<<<<<<< HEAD
#include <asm/mmu_writeable.h>

#include <asm/mach/arch.h>
#include <asm/mach/map.h>

#include <asm/user_accessible_timer.h>

#include "mm.h"

=======
#include <asm/procinfo.h>
#include <asm/page.h>
#include <asm/pgalloc.h>
#include <asm/kasan_def.h>

#include <asm/mach/arch.h>
#include <asm/mach/map.h>
#include <asm/mach/pci.h>
#include <asm/fixmap.h>

#include "fault.h"
#include "mm.h"

extern unsigned long __atags_pointer;

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
/*
 * empty_zero_page is a special page that is used for
 * zero-initialized data and COW.
 */
struct page *empty_zero_page;
EXPORT_SYMBOL(empty_zero_page);

/*
 * The pmd table for the upper-most set of pages.
 */
pmd_t *top_pmd;

<<<<<<< HEAD
=======
pmdval_t user_pmd_table = _PAGE_USER_TABLE;

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#define CPOLICY_UNCACHED	0
#define CPOLICY_BUFFERED	1
#define CPOLICY_WRITETHROUGH	2
#define CPOLICY_WRITEBACK	3
#define CPOLICY_WRITEALLOC	4

<<<<<<< HEAD
#define RX_AREA_START           _text
#define RX_AREA_END             __start_rodata

=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
static unsigned int cachepolicy __initdata = CPOLICY_WRITEBACK;
static unsigned int ecc_mask __initdata = 0;
pgprot_t pgprot_user;
pgprot_t pgprot_kernel;

EXPORT_SYMBOL(pgprot_user);
EXPORT_SYMBOL(pgprot_kernel);

struct cachepolicy {
	const char	policy[16];
	unsigned int	cr_mask;
	pmdval_t	pmd;
	pteval_t	pte;
};

static struct cachepolicy cache_policies[] __initdata = {
	{
		.policy		= "uncached",
		.cr_mask	= CR_W|CR_C,
		.pmd		= PMD_SECT_UNCACHED,
		.pte		= L_PTE_MT_UNCACHED,
	}, {
		.policy		= "buffered",
		.cr_mask	= CR_C,
		.pmd		= PMD_SECT_BUFFERED,
		.pte		= L_PTE_MT_BUFFERABLE,
	}, {
		.policy		= "writethrough",
		.cr_mask	= 0,
		.pmd		= PMD_SECT_WT,
		.pte		= L_PTE_MT_WRITETHROUGH,
	}, {
		.policy		= "writeback",
		.cr_mask	= 0,
		.pmd		= PMD_SECT_WB,
		.pte		= L_PTE_MT_WRITEBACK,
	}, {
		.policy		= "writealloc",
		.cr_mask	= 0,
		.pmd		= PMD_SECT_WBWA,
		.pte		= L_PTE_MT_WRITEALLOC,
	}
};

<<<<<<< HEAD
/*
 * These are useful for identifying cache coherency
 * problems by allowing the cache or the cache and
 * writebuffer to be turned off.  (Note: the write
 * buffer should not be on and the cache off).
 */
static int __init early_cachepolicy(char *p)
{
	int i;
=======
#ifdef CONFIG_CPU_CP15
static unsigned long initial_pmd_value __initdata = 0;

/*
 * Initialise the cache_policy variable with the initial state specified
 * via the "pmd" value.  This is used to ensure that on ARMv6 and later,
 * the C code sets the page tables up with the same policy as the head
 * assembly code, which avoids an illegal state where the TLBs can get
 * confused.  See comments in early_cachepolicy() for more information.
 */
void __init init_default_cache_policy(unsigned long pmd)
{
	int i;

	initial_pmd_value = pmd;

	pmd &= PMD_SECT_CACHE_MASK;

	for (i = 0; i < ARRAY_SIZE(cache_policies); i++)
		if (cache_policies[i].pmd == pmd) {
			cachepolicy = i;
			break;
		}

	if (i == ARRAY_SIZE(cache_policies))
		pr_err("ERROR: could not find cache policy\n");
}

/*
 * These are useful for identifying cache coherency problems by allowing
 * the cache or the cache and writebuffer to be turned off.  (Note: the
 * write buffer should not be on and the cache off).
 */
static int __init early_cachepolicy(char *p)
{
	int i, selected = -1;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	for (i = 0; i < ARRAY_SIZE(cache_policies); i++) {
		int len = strlen(cache_policies[i].policy);

		if (memcmp(p, cache_policies[i].policy, len) == 0) {
<<<<<<< HEAD
			cachepolicy = i;
			cr_alignment &= ~cache_policies[i].cr_mask;
			cr_no_alignment &= ~cache_policies[i].cr_mask;
			break;
		}
	}
	if (i == ARRAY_SIZE(cache_policies))
		printk(KERN_ERR "ERROR: unknown or unsupported cache policy\n");
=======
			selected = i;
			break;
		}
	}

	if (selected == -1)
		pr_err("ERROR: unknown or unsupported cache policy\n");

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	/*
	 * This restriction is partly to do with the way we boot; it is
	 * unpredictable to have memory mapped using two different sets of
	 * memory attributes (shared, type, and cache attribs).  We can not
	 * change these attributes once the initial assembly has setup the
	 * page tables.
	 */
<<<<<<< HEAD
	if (cpu_architecture() >= CPU_ARCH_ARMv6) {
		printk(KERN_WARNING "Only cachepolicy=writeback supported on ARMv6 and later\n");
		cachepolicy = CPOLICY_WRITEBACK;
	}
	flush_cache_all();
	set_cr(cr_alignment);
=======
	if (cpu_architecture() >= CPU_ARCH_ARMv6 && selected != cachepolicy) {
		pr_warn("Only cachepolicy=%s supported on ARMv6 and later\n",
			cache_policies[cachepolicy].policy);
		return 0;
	}

	if (selected != cachepolicy) {
		unsigned long cr = __clear_cr(cache_policies[selected].cr_mask);
		cachepolicy = selected;
		flush_cache_all();
		set_cr(cr);
	}
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	return 0;
}
early_param("cachepolicy", early_cachepolicy);

static int __init early_nocache(char *__unused)
{
	char *p = "buffered";
<<<<<<< HEAD
	printk(KERN_WARNING "nocache is deprecated; use cachepolicy=%s\n", p);
=======
	pr_warn("nocache is deprecated; use cachepolicy=%s\n", p);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	early_cachepolicy(p);
	return 0;
}
early_param("nocache", early_nocache);

static int __init early_nowrite(char *__unused)
{
	char *p = "uncached";
<<<<<<< HEAD
	printk(KERN_WARNING "nowb is deprecated; use cachepolicy=%s\n", p);
=======
	pr_warn("nowb is deprecated; use cachepolicy=%s\n", p);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	early_cachepolicy(p);
	return 0;
}
early_param("nowb", early_nowrite);

#ifndef CONFIG_ARM_LPAE
static int __init early_ecc(char *p)
{
	if (memcmp(p, "on", 2) == 0)
		ecc_mask = PMD_PROTECTION;
	else if (memcmp(p, "off", 3) == 0)
		ecc_mask = 0;
	return 0;
}
early_param("ecc", early_ecc);
#endif

<<<<<<< HEAD
static int __init noalign_setup(char *__unused)
{
	cr_alignment &= ~CR_A;
	cr_no_alignment &= ~CR_A;
	set_cr(cr_alignment);
=======
#else /* ifdef CONFIG_CPU_CP15 */

static int __init early_cachepolicy(char *p)
{
	pr_warn("cachepolicy kernel parameter not supported without cp15\n");
	return 0;
}
early_param("cachepolicy", early_cachepolicy);

static int __init noalign_setup(char *__unused)
{
	pr_warn("noalign kernel parameter not supported without cp15\n");
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	return 1;
}
__setup("noalign", noalign_setup);

<<<<<<< HEAD
#ifndef CONFIG_SMP
void adjust_cr(unsigned long mask, unsigned long set)
{
	unsigned long flags;

	mask &= ~CR_A;

	set &= mask;

	local_irq_save(flags);

	cr_no_alignment = (cr_no_alignment & ~mask) | set;
	cr_alignment = (cr_alignment & ~mask) | set;

	set_cr((get_cr() & ~mask) | set);

	local_irq_restore(flags);
}
#endif

#define PROT_PTE_DEVICE		L_PTE_PRESENT|L_PTE_YOUNG|L_PTE_DIRTY|L_PTE_XN
#define PROT_SECT_DEVICE	PMD_TYPE_SECT|PMD_SECT_AP_WRITE

static struct mem_type mem_types[] = {
=======
#endif /* ifdef CONFIG_CPU_CP15 / else */

#define PROT_PTE_DEVICE		L_PTE_PRESENT|L_PTE_YOUNG|L_PTE_DIRTY|L_PTE_XN
#define PROT_PTE_S2_DEVICE	PROT_PTE_DEVICE
#define PROT_SECT_DEVICE	PMD_TYPE_SECT|PMD_SECT_AP_WRITE

static struct mem_type mem_types[] __ro_after_init = {
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	[MT_DEVICE] = {		  /* Strongly ordered / ARMv6 shared device */
		.prot_pte	= PROT_PTE_DEVICE | L_PTE_MT_DEV_SHARED |
				  L_PTE_SHARED,
		.prot_l1	= PMD_TYPE_TABLE,
		.prot_sect	= PROT_SECT_DEVICE | PMD_SECT_S,
		.domain		= DOMAIN_IO,
	},
	[MT_DEVICE_NONSHARED] = { /* ARMv6 non-shared device */
		.prot_pte	= PROT_PTE_DEVICE | L_PTE_MT_DEV_NONSHARED,
		.prot_l1	= PMD_TYPE_TABLE,
		.prot_sect	= PROT_SECT_DEVICE,
		.domain		= DOMAIN_IO,
	},
<<<<<<< HEAD
	[MT_DEVICE_CACHED] = {	  /* ioremap_cached */
=======
	[MT_DEVICE_CACHED] = {	  /* ioremap_cache */
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		.prot_pte	= PROT_PTE_DEVICE | L_PTE_MT_DEV_CACHED,
		.prot_l1	= PMD_TYPE_TABLE,
		.prot_sect	= PROT_SECT_DEVICE | PMD_SECT_WB,
		.domain		= DOMAIN_IO,
<<<<<<< HEAD
	},	
=======
	},
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	[MT_DEVICE_WC] = {	/* ioremap_wc */
		.prot_pte	= PROT_PTE_DEVICE | L_PTE_MT_DEV_WC,
		.prot_l1	= PMD_TYPE_TABLE,
		.prot_sect	= PROT_SECT_DEVICE,
		.domain		= DOMAIN_IO,
	},
	[MT_UNCACHED] = {
		.prot_pte	= PROT_PTE_DEVICE,
		.prot_l1	= PMD_TYPE_TABLE,
		.prot_sect	= PMD_TYPE_SECT | PMD_SECT_XN,
		.domain		= DOMAIN_IO,
	},
	[MT_CACHECLEAN] = {
		.prot_sect = PMD_TYPE_SECT | PMD_SECT_XN,
		.domain    = DOMAIN_KERNEL,
	},
#ifndef CONFIG_ARM_LPAE
	[MT_MINICLEAN] = {
		.prot_sect = PMD_TYPE_SECT | PMD_SECT_XN | PMD_SECT_MINICACHE,
		.domain    = DOMAIN_KERNEL,
	},
#endif
	[MT_LOW_VECTORS] = {
		.prot_pte  = L_PTE_PRESENT | L_PTE_YOUNG | L_PTE_DIRTY |
				L_PTE_RDONLY,
		.prot_l1   = PMD_TYPE_TABLE,
<<<<<<< HEAD
		.domain    = DOMAIN_USER,
=======
		.domain    = DOMAIN_VECTORS,
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	},
	[MT_HIGH_VECTORS] = {
		.prot_pte  = L_PTE_PRESENT | L_PTE_YOUNG | L_PTE_DIRTY |
				L_PTE_USER | L_PTE_RDONLY,
		.prot_l1   = PMD_TYPE_TABLE,
<<<<<<< HEAD
		.domain    = DOMAIN_USER,
	},
	[MT_MEMORY] = {
=======
		.domain    = DOMAIN_VECTORS,
	},
	[MT_MEMORY_RWX] = {
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		.prot_pte  = L_PTE_PRESENT | L_PTE_YOUNG | L_PTE_DIRTY,
		.prot_l1   = PMD_TYPE_TABLE,
		.prot_sect = PMD_TYPE_SECT | PMD_SECT_AP_WRITE,
		.domain    = DOMAIN_KERNEL,
	},
<<<<<<< HEAD
	[MT_MEMORY_R] = {
		.prot_sect = PMD_TYPE_SECT | PMD_SECT_XN,
		.domain    = DOMAIN_KERNEL,
	},
	[MT_MEMORY_RW] = {
		.prot_sect = PMD_TYPE_SECT | PMD_SECT_AP_WRITE | PMD_SECT_XN,
		.domain    = DOMAIN_KERNEL,
	},
	[MT_MEMORY_RX] = {
		.prot_sect = PMD_TYPE_SECT,
=======
	[MT_MEMORY_RW] = {
		.prot_pte  = L_PTE_PRESENT | L_PTE_YOUNG | L_PTE_DIRTY |
			     L_PTE_XN,
		.prot_l1   = PMD_TYPE_TABLE,
		.prot_sect = PMD_TYPE_SECT | PMD_SECT_AP_WRITE,
		.domain    = DOMAIN_KERNEL,
	},
	[MT_MEMORY_RO] = {
		.prot_pte  = L_PTE_PRESENT | L_PTE_YOUNG | L_PTE_DIRTY |
			     L_PTE_XN | L_PTE_RDONLY,
		.prot_l1   = PMD_TYPE_TABLE,
#ifdef CONFIG_ARM_LPAE
		.prot_sect = PMD_TYPE_SECT | L_PMD_SECT_RDONLY | PMD_SECT_AP2,
#else
		.prot_sect = PMD_TYPE_SECT,
#endif
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		.domain    = DOMAIN_KERNEL,
	},
	[MT_ROM] = {
		.prot_sect = PMD_TYPE_SECT,
		.domain    = DOMAIN_KERNEL,
	},
<<<<<<< HEAD
	[MT_MEMORY_NONCACHED] = {
=======
	[MT_MEMORY_RWX_NONCACHED] = {
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		.prot_pte  = L_PTE_PRESENT | L_PTE_YOUNG | L_PTE_DIRTY |
				L_PTE_MT_BUFFERABLE,
		.prot_l1   = PMD_TYPE_TABLE,
		.prot_sect = PMD_TYPE_SECT | PMD_SECT_AP_WRITE,
		.domain    = DOMAIN_KERNEL,
	},
<<<<<<< HEAD
	[MT_MEMORY_DTCM] = {
=======
	[MT_MEMORY_RW_DTCM] = {
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		.prot_pte  = L_PTE_PRESENT | L_PTE_YOUNG | L_PTE_DIRTY |
				L_PTE_XN,
		.prot_l1   = PMD_TYPE_TABLE,
		.prot_sect = PMD_TYPE_SECT | PMD_SECT_XN,
		.domain    = DOMAIN_KERNEL,
	},
<<<<<<< HEAD
	[MT_MEMORY_ITCM] = {
=======
	[MT_MEMORY_RWX_ITCM] = {
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		.prot_pte  = L_PTE_PRESENT | L_PTE_YOUNG | L_PTE_DIRTY,
		.prot_l1   = PMD_TYPE_TABLE,
		.domain    = DOMAIN_KERNEL,
	},
<<<<<<< HEAD
	[MT_MEMORY_SO] = {
		.prot_pte  = L_PTE_PRESENT | L_PTE_YOUNG | L_PTE_DIRTY |
				L_PTE_MT_UNCACHED,
=======
	[MT_MEMORY_RW_SO] = {
		.prot_pte  = L_PTE_PRESENT | L_PTE_YOUNG | L_PTE_DIRTY |
				L_PTE_MT_UNCACHED | L_PTE_XN,
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		.prot_l1   = PMD_TYPE_TABLE,
		.prot_sect = PMD_TYPE_SECT | PMD_SECT_AP_WRITE | PMD_SECT_S |
				PMD_SECT_UNCACHED | PMD_SECT_XN,
		.domain    = DOMAIN_KERNEL,
	},
	[MT_MEMORY_DMA_READY] = {
		.prot_pte  = L_PTE_PRESENT | L_PTE_YOUNG | L_PTE_DIRTY |
				L_PTE_XN,
		.prot_l1   = PMD_TYPE_TABLE,
		.domain    = DOMAIN_KERNEL,
	},
<<<<<<< HEAD
	[MT_DEVICE_USER_ACCESSIBLE] = {
		.prot_pte  = PROT_PTE_DEVICE | L_PTE_MT_DEV_SHARED |
				L_PTE_SHARED | L_PTE_USER | L_PTE_RDONLY,
		.prot_l1   = PMD_TYPE_TABLE,
		.prot_sect = PROT_SECT_DEVICE | PMD_SECT_S,
		.domain    = DOMAIN_IO,
	},
=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
};

const struct mem_type *get_mem_type(unsigned int type)
{
	return type < ARRAY_SIZE(mem_types) ? &mem_types[type] : NULL;
}
EXPORT_SYMBOL(get_mem_type);

<<<<<<< HEAD
#define PTE_SET_FN(_name, pteop) \
static int pte_set_##_name(pte_t *ptep, pgtable_t token, unsigned long addr, \
			void *data) \
{ \
	pte_t pte = pteop(*ptep); \
\
	set_pte_ext(ptep, pte, 0); \
	return 0; \
} \

#define SET_MEMORY_FN(_name, callback) \
int set_memory_##_name(unsigned long addr, int numpages) \
{ \
	unsigned long start = addr; \
	unsigned long size = PAGE_SIZE*numpages; \
	unsigned end = start + size; \
\
	if (!IS_ENABLED(CONFIG_FORCE_PAGES)) { \
		if (start < MODULES_VADDR || start >= MODULES_END) \
			return -EINVAL;\
\
		if (end < MODULES_VADDR || end >= MODULES_END) \
			return -EINVAL; \
	} \
\
	apply_to_page_range(&init_mm, start, size, callback, NULL); \
	flush_tlb_kernel_range(start, end); \
	return 0;\
}

PTE_SET_FN(ro, pte_wrprotect)
PTE_SET_FN(rw, pte_mkwrite)
PTE_SET_FN(x, pte_mkexec)
PTE_SET_FN(nx, pte_mknexec)

SET_MEMORY_FN(ro, pte_set_ro)
EXPORT_SYMBOL(set_memory_ro);
SET_MEMORY_FN(rw, pte_set_rw)
EXPORT_SYMBOL(set_memory_rw);
SET_MEMORY_FN(x, pte_set_x)
EXPORT_SYMBOL(set_memory_x);
SET_MEMORY_FN(nx, pte_set_nx)
EXPORT_SYMBOL(set_memory_nx);
=======
static pte_t *(*pte_offset_fixmap)(pmd_t *dir, unsigned long addr);

static pte_t bm_pte[PTRS_PER_PTE + PTE_HWTABLE_PTRS]
	__aligned(PTE_HWTABLE_OFF + PTE_HWTABLE_SIZE) __initdata;

static pte_t * __init pte_offset_early_fixmap(pmd_t *dir, unsigned long addr)
{
	return &bm_pte[pte_index(addr)];
}

static pte_t *pte_offset_late_fixmap(pmd_t *dir, unsigned long addr)
{
	return pte_offset_kernel(dir, addr);
}

static inline pmd_t * __init fixmap_pmd(unsigned long addr)
{
	return pmd_off_k(addr);
}

void __init early_fixmap_init(void)
{
	pmd_t *pmd;

	/*
	 * The early fixmap range spans multiple pmds, for which
	 * we are not prepared:
	 */
	BUILD_BUG_ON((__fix_to_virt(__end_of_early_ioremap_region) >> PMD_SHIFT)
		     != FIXADDR_TOP >> PMD_SHIFT);

	pmd = fixmap_pmd(FIXADDR_TOP);
	pmd_populate_kernel(&init_mm, pmd, bm_pte);

	pte_offset_fixmap = pte_offset_early_fixmap;
}

/*
 * To avoid TLB flush broadcasts, this uses local_flush_tlb_kernel_range().
 * As a result, this can only be called with preemption disabled, as under
 * stop_machine().
 */
void __set_fixmap(enum fixed_addresses idx, phys_addr_t phys, pgprot_t prot)
{
	unsigned long vaddr = __fix_to_virt(idx);
	pte_t *pte = pte_offset_fixmap(pmd_off_k(vaddr), vaddr);

	/* Make sure fixmap region does not exceed available allocation. */
	BUILD_BUG_ON(__fix_to_virt(__end_of_fixed_addresses) < FIXADDR_START);
	BUG_ON(idx >= __end_of_fixed_addresses);

	/* We support only device mappings before pgprot_kernel is set. */
	if (WARN_ON(pgprot_val(prot) != pgprot_val(FIXMAP_PAGE_IO) &&
		    pgprot_val(prot) && pgprot_val(pgprot_kernel) == 0))
		return;

	if (pgprot_val(prot))
		set_pte_at(NULL, vaddr, pte,
			pfn_pte(phys >> PAGE_SHIFT, prot));
	else
		pte_clear(NULL, vaddr, pte);
	local_flush_tlb_kernel_range(vaddr, vaddr + PAGE_SIZE);
}

static pgprot_t protection_map[16] __ro_after_init = {
	[VM_NONE]					= __PAGE_NONE,
	[VM_READ]					= __PAGE_READONLY,
	[VM_WRITE]					= __PAGE_COPY,
	[VM_WRITE | VM_READ]				= __PAGE_COPY,
	[VM_EXEC]					= __PAGE_READONLY_EXEC,
	[VM_EXEC | VM_READ]				= __PAGE_READONLY_EXEC,
	[VM_EXEC | VM_WRITE]				= __PAGE_COPY_EXEC,
	[VM_EXEC | VM_WRITE | VM_READ]			= __PAGE_COPY_EXEC,
	[VM_SHARED]					= __PAGE_NONE,
	[VM_SHARED | VM_READ]				= __PAGE_READONLY,
	[VM_SHARED | VM_WRITE]				= __PAGE_SHARED,
	[VM_SHARED | VM_WRITE | VM_READ]		= __PAGE_SHARED,
	[VM_SHARED | VM_EXEC]				= __PAGE_READONLY_EXEC,
	[VM_SHARED | VM_EXEC | VM_READ]			= __PAGE_READONLY_EXEC,
	[VM_SHARED | VM_EXEC | VM_WRITE]		= __PAGE_SHARED_EXEC,
	[VM_SHARED | VM_EXEC | VM_WRITE | VM_READ]	= __PAGE_SHARED_EXEC
};
DECLARE_VM_GET_PAGE_PROT
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

/*
 * Adjust the PMD section entries according to the CPU in use.
 */
static void __init build_mem_type_table(void)
{
	struct cachepolicy *cp;
	unsigned int cr = get_cr();
	pteval_t user_pgprot, kern_pgprot, vecs_pgprot;
	int cpu_arch = cpu_architecture();
	int i;

	if (cpu_arch < CPU_ARCH_ARMv6) {
#if defined(CONFIG_CPU_DCACHE_DISABLE)
		if (cachepolicy > CPOLICY_BUFFERED)
			cachepolicy = CPOLICY_BUFFERED;
#elif defined(CONFIG_CPU_DCACHE_WRITETHROUGH)
		if (cachepolicy > CPOLICY_WRITETHROUGH)
			cachepolicy = CPOLICY_WRITETHROUGH;
#endif
	}
	if (cpu_arch < CPU_ARCH_ARMv5) {
		if (cachepolicy >= CPOLICY_WRITEALLOC)
			cachepolicy = CPOLICY_WRITEBACK;
		ecc_mask = 0;
	}
<<<<<<< HEAD
	if (is_smp())
		cachepolicy = CPOLICY_WRITEALLOC;
=======

	if (is_smp()) {
		if (cachepolicy != CPOLICY_WRITEALLOC) {
			pr_warn("Forcing write-allocate cache policy for SMP\n");
			cachepolicy = CPOLICY_WRITEALLOC;
		}
		if (!(initial_pmd_value & PMD_SECT_S)) {
			pr_warn("Forcing shared mappings for SMP\n");
			initial_pmd_value |= PMD_SECT_S;
		}
	}
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	/*
	 * Strip out features not present on earlier architectures.
	 * Pre-ARMv5 CPUs don't have TEX bits.  Pre-ARMv6 CPUs or those
	 * without extended page tables don't have the 'Shared' bit.
	 */
	if (cpu_arch < CPU_ARCH_ARMv5)
		for (i = 0; i < ARRAY_SIZE(mem_types); i++)
			mem_types[i].prot_sect &= ~PMD_SECT_TEX(7);
	if ((cpu_arch < CPU_ARCH_ARMv6 || !(cr & CR_XP)) && !cpu_is_xsc3())
		for (i = 0; i < ARRAY_SIZE(mem_types); i++)
			mem_types[i].prot_sect &= ~PMD_SECT_S;

	/*
	 * ARMv5 and lower, bit 4 must be set for page tables (was: cache
	 * "update-able on write" bit on ARM610).  However, Xscale and
	 * Xscale3 require this bit to be cleared.
	 */
<<<<<<< HEAD
	if (cpu_is_xscale() || cpu_is_xsc3()) {
=======
	if (cpu_is_xscale_family()) {
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		for (i = 0; i < ARRAY_SIZE(mem_types); i++) {
			mem_types[i].prot_sect &= ~PMD_BIT4;
			mem_types[i].prot_l1 &= ~PMD_BIT4;
		}
	} else if (cpu_arch < CPU_ARCH_ARMv6) {
		for (i = 0; i < ARRAY_SIZE(mem_types); i++) {
			if (mem_types[i].prot_l1)
				mem_types[i].prot_l1 |= PMD_BIT4;
			if (mem_types[i].prot_sect)
				mem_types[i].prot_sect |= PMD_BIT4;
		}
	}

	/*
	 * Mark the device areas according to the CPU/architecture.
	 */
	if (cpu_is_xsc3() || (cpu_arch >= CPU_ARCH_ARMv6 && (cr & CR_XP))) {
		if (!cpu_is_xsc3()) {
			/*
			 * Mark device regions on ARMv6+ as execute-never
			 * to prevent speculative instruction fetches.
			 */
			mem_types[MT_DEVICE].prot_sect |= PMD_SECT_XN;
			mem_types[MT_DEVICE_NONSHARED].prot_sect |= PMD_SECT_XN;
			mem_types[MT_DEVICE_CACHED].prot_sect |= PMD_SECT_XN;
			mem_types[MT_DEVICE_WC].prot_sect |= PMD_SECT_XN;
<<<<<<< HEAD
=======

			/* Also setup NX memory mapping */
			mem_types[MT_MEMORY_RW].prot_sect |= PMD_SECT_XN;
			mem_types[MT_MEMORY_RO].prot_sect |= PMD_SECT_XN;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		}
		if (cpu_arch >= CPU_ARCH_ARMv7 && (cr & CR_TRE)) {
			/*
			 * For ARMv7 with TEX remapping,
			 * - shared device is SXCB=1100
			 * - nonshared device is SXCB=0100
			 * - write combine device mem is SXCB=0001
			 * (Uncached Normal memory)
			 */
			mem_types[MT_DEVICE].prot_sect |= PMD_SECT_TEX(1);
			mem_types[MT_DEVICE_NONSHARED].prot_sect |= PMD_SECT_TEX(1);
			mem_types[MT_DEVICE_WC].prot_sect |= PMD_SECT_BUFFERABLE;
		} else if (cpu_is_xsc3()) {
			/*
			 * For Xscale3,
			 * - shared device is TEXCB=00101
			 * - nonshared device is TEXCB=01000
			 * - write combine device mem is TEXCB=00100
			 * (Inner/Outer Uncacheable in xsc3 parlance)
			 */
			mem_types[MT_DEVICE].prot_sect |= PMD_SECT_TEX(1) | PMD_SECT_BUFFERED;
			mem_types[MT_DEVICE_NONSHARED].prot_sect |= PMD_SECT_TEX(2);
			mem_types[MT_DEVICE_WC].prot_sect |= PMD_SECT_TEX(1);
		} else {
			/*
			 * For ARMv6 and ARMv7 without TEX remapping,
			 * - shared device is TEXCB=00001
			 * - nonshared device is TEXCB=01000
			 * - write combine device mem is TEXCB=00100
			 * (Uncached Normal in ARMv6 parlance).
			 */
			mem_types[MT_DEVICE].prot_sect |= PMD_SECT_BUFFERED;
			mem_types[MT_DEVICE_NONSHARED].prot_sect |= PMD_SECT_TEX(2);
			mem_types[MT_DEVICE_WC].prot_sect |= PMD_SECT_TEX(1);
		}
	} else {
		/*
		 * On others, write combining is "Uncached/Buffered"
		 */
		mem_types[MT_DEVICE_WC].prot_sect |= PMD_SECT_BUFFERABLE;
	}

	/*
	 * Now deal with the memory-type mappings
	 */
	cp = &cache_policies[cachepolicy];
	vecs_pgprot = kern_pgprot = user_pgprot = cp->pte;

<<<<<<< HEAD
	/*
	 * Only use write-through for non-SMP systems
	 */
	if (!is_smp() && cpu_arch >= CPU_ARCH_ARMv5 && cachepolicy > CPOLICY_WRITETHROUGH)
		vecs_pgprot = cache_policies[CPOLICY_WRITETHROUGH].pte;

	/*
	 * Enable CPU-specific coherency if supported.
	 * (Only available on XSC3 at the moment.)
	 */
	if (arch_is_coherent() && cpu_is_xsc3()) {
		mem_types[MT_MEMORY].prot_sect |= PMD_SECT_S;
		mem_types[MT_MEMORY].prot_pte |= L_PTE_SHARED;
		mem_types[MT_MEMORY_DMA_READY].prot_pte |= L_PTE_SHARED;
		mem_types[MT_MEMORY_NONCACHED].prot_sect |= PMD_SECT_S;
		mem_types[MT_MEMORY_NONCACHED].prot_pte |= L_PTE_SHARED;
	}
=======
#ifndef CONFIG_ARM_LPAE
	/*
	 * We don't use domains on ARMv6 (since this causes problems with
	 * v6/v7 kernels), so we must use a separate memory type for user
	 * r/o, kernel r/w to map the vectors page.
	 */
	if (cpu_arch == CPU_ARCH_ARMv6)
		vecs_pgprot |= L_PTE_MT_VECTORS;

	/*
	 * Check is it with support for the PXN bit
	 * in the Short-descriptor translation table format descriptors.
	 */
	if (cpu_arch == CPU_ARCH_ARMv7 &&
		(read_cpuid_ext(CPUID_EXT_MMFR0) & 0xF) >= 4) {
		user_pmd_table |= PMD_PXNTABLE;
	}
#endif

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	/*
	 * ARMv6 and above have extended page tables.
	 */
	if (cpu_arch >= CPU_ARCH_ARMv6 && (cr & CR_XP)) {
#ifndef CONFIG_ARM_LPAE
		/*
		 * Mark cache clean areas and XIP ROM read only
		 * from SVC mode and no access from userspace.
		 */
		mem_types[MT_ROM].prot_sect |= PMD_SECT_APX|PMD_SECT_AP_WRITE;
<<<<<<< HEAD
		mem_types[MT_MEMORY_RX].prot_sect |= PMD_SECT_APX|PMD_SECT_AP_WRITE;
		mem_types[MT_MEMORY_R].prot_sect |= PMD_SECT_APX|PMD_SECT_AP_WRITE;
		mem_types[MT_MINICLEAN].prot_sect |= PMD_SECT_APX|PMD_SECT_AP_WRITE;
		mem_types[MT_CACHECLEAN].prot_sect |= PMD_SECT_APX|PMD_SECT_AP_WRITE;
#endif

		if (is_smp()) {
			/*
			 * Mark memory with the "shared" attribute
			 * for SMP systems
			 */
=======
		mem_types[MT_MINICLEAN].prot_sect |= PMD_SECT_APX|PMD_SECT_AP_WRITE;
		mem_types[MT_CACHECLEAN].prot_sect |= PMD_SECT_APX|PMD_SECT_AP_WRITE;
		mem_types[MT_MEMORY_RO].prot_sect |= PMD_SECT_APX|PMD_SECT_AP_WRITE;
#endif

		/*
		 * If the initial page tables were created with the S bit
		 * set, then we need to do the same here for the same
		 * reasons given in early_cachepolicy().
		 */
		if (initial_pmd_value & PMD_SECT_S) {
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
			user_pgprot |= L_PTE_SHARED;
			kern_pgprot |= L_PTE_SHARED;
			vecs_pgprot |= L_PTE_SHARED;
			mem_types[MT_DEVICE_WC].prot_sect |= PMD_SECT_S;
			mem_types[MT_DEVICE_WC].prot_pte |= L_PTE_SHARED;
			mem_types[MT_DEVICE_CACHED].prot_sect |= PMD_SECT_S;
			mem_types[MT_DEVICE_CACHED].prot_pte |= L_PTE_SHARED;
<<<<<<< HEAD
			mem_types[MT_MEMORY].prot_sect |= PMD_SECT_S;
			mem_types[MT_MEMORY].prot_pte |= L_PTE_SHARED;
			mem_types[MT_MEMORY_DMA_READY].prot_pte |= L_PTE_SHARED;
			mem_types[MT_MEMORY_NONCACHED].prot_sect |= PMD_SECT_S;
			mem_types[MT_MEMORY_R].prot_sect |= PMD_SECT_S;
			mem_types[MT_MEMORY_RW].prot_sect |= PMD_SECT_S;
			mem_types[MT_MEMORY_RX].prot_sect |= PMD_SECT_S;
			mem_types[MT_MEMORY_NONCACHED].prot_pte |= L_PTE_SHARED;
=======
			mem_types[MT_MEMORY_RWX].prot_sect |= PMD_SECT_S;
			mem_types[MT_MEMORY_RWX].prot_pte |= L_PTE_SHARED;
			mem_types[MT_MEMORY_RW].prot_sect |= PMD_SECT_S;
			mem_types[MT_MEMORY_RW].prot_pte |= L_PTE_SHARED;
			mem_types[MT_MEMORY_RO].prot_sect |= PMD_SECT_S;
			mem_types[MT_MEMORY_RO].prot_pte |= L_PTE_SHARED;
			mem_types[MT_MEMORY_DMA_READY].prot_pte |= L_PTE_SHARED;
			mem_types[MT_MEMORY_RWX_NONCACHED].prot_sect |= PMD_SECT_S;
			mem_types[MT_MEMORY_RWX_NONCACHED].prot_pte |= L_PTE_SHARED;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		}
	}

	/*
	 * Non-cacheable Normal - intended for memory areas that must
	 * not cause dirty cache line writebacks when used
	 */
	if (cpu_arch >= CPU_ARCH_ARMv6) {
		if (cpu_arch >= CPU_ARCH_ARMv7 && (cr & CR_TRE)) {
			/* Non-cacheable Normal is XCB = 001 */
<<<<<<< HEAD
			mem_types[MT_MEMORY_NONCACHED].prot_sect |=
				PMD_SECT_BUFFERED;
		} else {
			/* For both ARMv6 and non-TEX-remapping ARMv7 */
			mem_types[MT_MEMORY_NONCACHED].prot_sect |=
				PMD_SECT_TEX(1);
		}
	} else {
		mem_types[MT_MEMORY_NONCACHED].prot_sect |= PMD_SECT_BUFFERABLE;
=======
			mem_types[MT_MEMORY_RWX_NONCACHED].prot_sect |=
				PMD_SECT_BUFFERED;
		} else {
			/* For both ARMv6 and non-TEX-remapping ARMv7 */
			mem_types[MT_MEMORY_RWX_NONCACHED].prot_sect |=
				PMD_SECT_TEX(1);
		}
	} else {
		mem_types[MT_MEMORY_RWX_NONCACHED].prot_sect |= PMD_SECT_BUFFERABLE;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	}

#ifdef CONFIG_ARM_LPAE
	/*
	 * Do not generate access flag faults for the kernel mappings.
	 */
	for (i = 0; i < ARRAY_SIZE(mem_types); i++) {
		mem_types[i].prot_pte |= PTE_EXT_AF;
		if (mem_types[i].prot_sect)
			mem_types[i].prot_sect |= PMD_SECT_AF;
	}
	kern_pgprot |= PTE_EXT_AF;
	vecs_pgprot |= PTE_EXT_AF;
<<<<<<< HEAD
=======

	/*
	 * Set PXN for user mappings
	 */
	user_pgprot |= PTE_EXT_PXN;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#endif

	for (i = 0; i < 16; i++) {
		pteval_t v = pgprot_val(protection_map[i]);
		protection_map[i] = __pgprot(v | user_pgprot);
	}

	mem_types[MT_LOW_VECTORS].prot_pte |= vecs_pgprot;
	mem_types[MT_HIGH_VECTORS].prot_pte |= vecs_pgprot;

	pgprot_user   = __pgprot(L_PTE_PRESENT | L_PTE_YOUNG | user_pgprot);
	pgprot_kernel = __pgprot(L_PTE_PRESENT | L_PTE_YOUNG |
				 L_PTE_DIRTY | kern_pgprot);

	mem_types[MT_LOW_VECTORS].prot_l1 |= ecc_mask;
	mem_types[MT_HIGH_VECTORS].prot_l1 |= ecc_mask;
<<<<<<< HEAD
	mem_types[MT_MEMORY].prot_sect |= ecc_mask | cp->pmd;
	mem_types[MT_MEMORY].prot_pte |= kern_pgprot;
	mem_types[MT_MEMORY_DMA_READY].prot_pte |= kern_pgprot;
	mem_types[MT_MEMORY_NONCACHED].prot_sect |= ecc_mask;
	mem_types[MT_MEMORY_R].prot_sect |= ecc_mask | cp->pmd;
	mem_types[MT_MEMORY_RW].prot_sect |= ecc_mask | cp->pmd;
	mem_types[MT_MEMORY_RX].prot_sect |= ecc_mask | cp->pmd;
=======
	mem_types[MT_MEMORY_RWX].prot_sect |= ecc_mask | cp->pmd;
	mem_types[MT_MEMORY_RWX].prot_pte |= kern_pgprot;
	mem_types[MT_MEMORY_RW].prot_sect |= ecc_mask | cp->pmd;
	mem_types[MT_MEMORY_RW].prot_pte |= kern_pgprot;
	mem_types[MT_MEMORY_RO].prot_sect |= ecc_mask | cp->pmd;
	mem_types[MT_MEMORY_RO].prot_pte |= kern_pgprot;
	mem_types[MT_MEMORY_DMA_READY].prot_pte |= kern_pgprot;
	mem_types[MT_MEMORY_RWX_NONCACHED].prot_sect |= ecc_mask;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	mem_types[MT_ROM].prot_sect |= cp->pmd;

	switch (cp->pmd) {
	case PMD_SECT_WT:
		mem_types[MT_CACHECLEAN].prot_sect |= PMD_SECT_WT;
		break;
	case PMD_SECT_WB:
	case PMD_SECT_WBWA:
		mem_types[MT_CACHECLEAN].prot_sect |= PMD_SECT_WB;
		break;
	}
<<<<<<< HEAD
	printk("Memory policy: ECC %sabled, Data cache %s\n",
		ecc_mask ? "en" : "dis", cp->policy);
=======
	pr_info("Memory policy: %sData cache %s\n",
		ecc_mask ? "ECC enabled, " : "", cp->policy);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	for (i = 0; i < ARRAY_SIZE(mem_types); i++) {
		struct mem_type *t = &mem_types[i];
		if (t->prot_l1)
			t->prot_l1 |= PMD_DOMAIN(t->domain);
		if (t->prot_sect)
			t->prot_sect |= PMD_DOMAIN(t->domain);
	}
}

#ifdef CONFIG_ARM_DMA_MEM_BUFFERABLE
pgprot_t phys_mem_access_prot(struct file *file, unsigned long pfn,
			      unsigned long size, pgprot_t vma_prot)
{
	if (!pfn_valid(pfn))
		return pgprot_noncached(vma_prot);
	else if (file->f_flags & O_SYNC)
		return pgprot_writecombine(vma_prot);
	return vma_prot;
}
EXPORT_SYMBOL(phys_mem_access_prot);
#endif

#define vectors_base()	(vectors_high() ? 0xffff0000 : 0)

<<<<<<< HEAD
static void __init *early_alloc_aligned(unsigned long sz, unsigned long align)
{
	void *ptr = __va(memblock_alloc(sz, align));
	memset(ptr, 0, sz);
	return ptr;
}

static void __init *early_alloc(unsigned long sz)
{
	return early_alloc_aligned(sz, sz);
}

static pte_t * __init early_pte_alloc(pmd_t *pmd, unsigned long addr, unsigned long prot)
{
	if (pmd_none(*pmd)) {
		pte_t *pte = early_alloc(PTE_HWTABLE_OFF + PTE_HWTABLE_SIZE);
=======
static void __init *early_alloc(unsigned long sz)
{
	void *ptr = memblock_alloc(sz, sz);

	if (!ptr)
		panic("%s: Failed to allocate %lu bytes align=0x%lx\n",
		      __func__, sz, sz);

	return ptr;
}

static void *__init late_alloc(unsigned long sz)
{
	void *ptdesc = pagetable_alloc(GFP_PGTABLE_KERNEL & ~__GFP_HIGHMEM,
			get_order(sz));

	if (!ptdesc || !pagetable_pte_ctor(ptdesc))
		BUG();
	return ptdesc_to_virt(ptdesc);
}

static pte_t * __init arm_pte_alloc(pmd_t *pmd, unsigned long addr,
				unsigned long prot,
				void *(*alloc)(unsigned long sz))
{
	if (pmd_none(*pmd)) {
		pte_t *pte = alloc(PTE_HWTABLE_OFF + PTE_HWTABLE_SIZE);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		__pmd_populate(pmd, __pa(pte), prot);
	}
	BUG_ON(pmd_bad(*pmd));
	return pte_offset_kernel(pmd, addr);
}

<<<<<<< HEAD
static void __init alloc_init_pte(pmd_t *pmd, unsigned long addr,
				  unsigned long end, unsigned long pfn,
				  const struct mem_type *type)
{
	pte_t *pte = early_pte_alloc(pmd, addr, type->prot_l1);
	do {
		set_pte_ext(pte, pfn_pte(pfn, __pgprot(type->prot_pte)), 0);
=======
static pte_t * __init early_pte_alloc(pmd_t *pmd, unsigned long addr,
				      unsigned long prot)
{
	return arm_pte_alloc(pmd, addr, prot, early_alloc);
}

static void __init alloc_init_pte(pmd_t *pmd, unsigned long addr,
				  unsigned long end, unsigned long pfn,
				  const struct mem_type *type,
				  void *(*alloc)(unsigned long sz),
				  bool ng)
{
	pte_t *pte = arm_pte_alloc(pmd, addr, type->prot_l1, alloc);
	do {
		set_pte_ext(pte, pfn_pte(pfn, __pgprot(type->prot_pte)),
			    ng ? PTE_EXT_NG : 0);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		pfn++;
	} while (pte++, addr += PAGE_SIZE, addr != end);
}

<<<<<<< HEAD
static void __init map_init_section(pmd_t *pmd, unsigned long addr,
			unsigned long end, phys_addr_t phys,
			const struct mem_type *type)
{
=======
static void __init __map_init_section(pmd_t *pmd, unsigned long addr,
			unsigned long end, phys_addr_t phys,
			const struct mem_type *type, bool ng)
{
	pmd_t *p = pmd;

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#ifndef CONFIG_ARM_LPAE
	/*
	 * In classic MMU format, puds and pmds are folded in to
	 * the pgds. pmd_offset gives the PGD entry. PGDs refer to a
	 * group of L1 entries making up one logical pointer to
	 * an L2 table (2MB), where as PMDs refer to the individual
	 * L1 entries (1MB). Hence increment to get the correct
	 * offset for odd 1MB sections.
	 * (See arch/arm/include/asm/pgtable-2level.h)
	 */
	if (addr & SECTION_SIZE)
		pmd++;
#endif
	do {
<<<<<<< HEAD
		*pmd = __pmd(phys | type->prot_sect);
		phys += SECTION_SIZE;
	} while (pmd++, addr += SECTION_SIZE, addr != end);

	flush_pmd_entry(pmd);
=======
		*pmd = __pmd(phys | type->prot_sect | (ng ? PMD_SECT_nG : 0));
		phys += SECTION_SIZE;
	} while (pmd++, addr += SECTION_SIZE, addr != end);

	flush_pmd_entry(p);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

static void __init alloc_init_pmd(pud_t *pud, unsigned long addr,
				      unsigned long end, phys_addr_t phys,
<<<<<<< HEAD
				      const struct mem_type *type)
=======
				      const struct mem_type *type,
				      void *(*alloc)(unsigned long sz), bool ng)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
{
	pmd_t *pmd = pmd_offset(pud, addr);
	unsigned long next;

	do {
		/*
		 * With LPAE, we must loop over to map
		 * all the pmds for the given range.
		 */
		next = pmd_addr_end(addr, end);

		/*
		 * Try a section mapping - addr, next and phys must all be
		 * aligned to a section boundary.
		 */
		if (type->prot_sect &&
				((addr | next | phys) & ~SECTION_MASK) == 0) {
<<<<<<< HEAD
			map_init_section(pmd, addr, next, phys, type);
		} else {
			alloc_init_pte(pmd, addr, next,
						__phys_to_pfn(phys), type);
=======
			__map_init_section(pmd, addr, next, phys, type, ng);
		} else {
			alloc_init_pte(pmd, addr, next,
				       __phys_to_pfn(phys), type, alloc, ng);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		}

		phys += next - addr;

	} while (pmd++, addr = next, addr != end);
}

<<<<<<< HEAD
static void __init alloc_init_pud(pgd_t *pgd, unsigned long addr,
	unsigned long end, unsigned long phys, const struct mem_type *type)
{
	pud_t *pud = pud_offset(pgd, addr);
=======
static void __init alloc_init_pud(p4d_t *p4d, unsigned long addr,
				  unsigned long end, phys_addr_t phys,
				  const struct mem_type *type,
				  void *(*alloc)(unsigned long sz), bool ng)
{
	pud_t *pud = pud_offset(p4d, addr);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	unsigned long next;

	do {
		next = pud_addr_end(addr, end);
<<<<<<< HEAD
		alloc_init_pmd(pud, addr, next, phys, type);
=======
		alloc_init_pmd(pud, addr, next, phys, type, alloc, ng);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		phys += next - addr;
	} while (pud++, addr = next, addr != end);
}

<<<<<<< HEAD
#ifndef CONFIG_ARM_LPAE
static void __init create_36bit_mapping(struct map_desc *md,
					const struct mem_type *type)
=======
static void __init alloc_init_p4d(pgd_t *pgd, unsigned long addr,
				  unsigned long end, phys_addr_t phys,
				  const struct mem_type *type,
				  void *(*alloc)(unsigned long sz), bool ng)
{
	p4d_t *p4d = p4d_offset(pgd, addr);
	unsigned long next;

	do {
		next = p4d_addr_end(addr, end);
		alloc_init_pud(p4d, addr, next, phys, type, alloc, ng);
		phys += next - addr;
	} while (p4d++, addr = next, addr != end);
}

#ifndef CONFIG_ARM_LPAE
static void __init create_36bit_mapping(struct mm_struct *mm,
					struct map_desc *md,
					const struct mem_type *type,
					bool ng)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
{
	unsigned long addr, length, end;
	phys_addr_t phys;
	pgd_t *pgd;

	addr = md->virtual;
	phys = __pfn_to_phys(md->pfn);
	length = PAGE_ALIGN(md->length);

	if (!(cpu_architecture() >= CPU_ARCH_ARMv6 || cpu_is_xsc3())) {
<<<<<<< HEAD
		printk(KERN_ERR "MM: CPU does not support supersection "
		       "mapping for 0x%08llx at 0x%08lx\n",
=======
		pr_err("MM: CPU does not support supersection mapping for 0x%08llx at 0x%08lx\n",
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		       (long long)__pfn_to_phys((u64)md->pfn), addr);
		return;
	}

	/* N.B.	ARMv6 supersections are only defined to work with domain 0.
	 *	Since domain assignments can in fact be arbitrary, the
	 *	'domain == 0' check below is required to insure that ARMv6
	 *	supersections are only allocated for domain 0 regardless
	 *	of the actual domain assignments in use.
	 */
	if (type->domain) {
<<<<<<< HEAD
		printk(KERN_ERR "MM: invalid domain in supersection "
		       "mapping for 0x%08llx at 0x%08lx\n",
=======
		pr_err("MM: invalid domain in supersection mapping for 0x%08llx at 0x%08lx\n",
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		       (long long)__pfn_to_phys((u64)md->pfn), addr);
		return;
	}

	if ((addr | length | __pfn_to_phys(md->pfn)) & ~SUPERSECTION_MASK) {
<<<<<<< HEAD
		printk(KERN_ERR "MM: cannot create mapping for 0x%08llx"
		       " at 0x%08lx invalid alignment\n",
=======
		pr_err("MM: cannot create mapping for 0x%08llx at 0x%08lx invalid alignment\n",
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		       (long long)__pfn_to_phys((u64)md->pfn), addr);
		return;
	}

	/*
	 * Shift bits [35:32] of address into bits [23:20] of PMD
	 * (See ARMv6 spec).
	 */
	phys |= (((md->pfn >> (32 - PAGE_SHIFT)) & 0xF) << 20);

<<<<<<< HEAD
	pgd = pgd_offset_k(addr);
	end = addr + length;
	do {
		pud_t *pud = pud_offset(pgd, addr);
=======
	pgd = pgd_offset(mm, addr);
	end = addr + length;
	do {
		p4d_t *p4d = p4d_offset(pgd, addr);
		pud_t *pud = pud_offset(p4d, addr);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		pmd_t *pmd = pmd_offset(pud, addr);
		int i;

		for (i = 0; i < 16; i++)
<<<<<<< HEAD
			*pmd++ = __pmd(phys | type->prot_sect | PMD_SECT_SUPER);
=======
			*pmd++ = __pmd(phys | type->prot_sect | PMD_SECT_SUPER |
				       (ng ? PMD_SECT_nG : 0));
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

		addr += SUPERSECTION_SIZE;
		phys += SUPERSECTION_SIZE;
		pgd += SUPERSECTION_SIZE >> PGDIR_SHIFT;
	} while (addr != end);
}
#endif	/* !CONFIG_ARM_LPAE */

<<<<<<< HEAD
=======
static void __init __create_mapping(struct mm_struct *mm, struct map_desc *md,
				    void *(*alloc)(unsigned long sz),
				    bool ng)
{
	unsigned long addr, length, end;
	phys_addr_t phys;
	const struct mem_type *type;
	pgd_t *pgd;

	type = &mem_types[md->type];

#ifndef CONFIG_ARM_LPAE
	/*
	 * Catch 36-bit addresses
	 */
	if (md->pfn >= 0x100000) {
		create_36bit_mapping(mm, md, type, ng);
		return;
	}
#endif

	addr = md->virtual & PAGE_MASK;
	phys = __pfn_to_phys(md->pfn);
	length = PAGE_ALIGN(md->length + (md->virtual & ~PAGE_MASK));

	if (type->prot_l1 == 0 && ((addr | phys | length) & ~SECTION_MASK)) {
		pr_warn("BUG: map for 0x%08llx at 0x%08lx can not be mapped using pages, ignoring.\n",
			(long long)__pfn_to_phys(md->pfn), addr);
		return;
	}

	pgd = pgd_offset(mm, addr);
	end = addr + length;
	do {
		unsigned long next = pgd_addr_end(addr, end);

		alloc_init_p4d(pgd, addr, next, phys, type, alloc, ng);

		phys += next - addr;
		addr = next;
	} while (pgd++, addr != end);
}

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
/*
 * Create the page directory entries and any necessary
 * page tables for the mapping specified by `md'.  We
 * are able to cope here with varying sizes and address
 * offsets, and we take full advantage of sections and
 * supersections.
 */
static void __init create_mapping(struct map_desc *md)
{
<<<<<<< HEAD
	unsigned long addr, length, end;
	phys_addr_t phys;
	const struct mem_type *type;
	pgd_t *pgd;

	if ((md->virtual != vectors_base() &&
		md->virtual != get_user_accessible_timers_base()) &&
			md->virtual < TASK_SIZE) {
		printk(KERN_WARNING "BUG: not creating mapping for 0x%08llx"
		       " at 0x%08lx in user region\n",
		       (long long)__pfn_to_phys((u64)md->pfn), md->virtual);
		return;
	}

	if ((md->type == MT_DEVICE || md->type == MT_ROM) &&
	    md->virtual >= PAGE_OFFSET &&
	    (md->virtual < VMALLOC_START || md->virtual >= VMALLOC_END)) {
		printk(KERN_WARNING "BUG: mapping for 0x%08llx"
		       " at 0x%08lx out of vmalloc space\n",
		       (long long)__pfn_to_phys((u64)md->pfn), md->virtual);
	}

	type = &mem_types[md->type];

#ifndef CONFIG_ARM_LPAE
	/*
	 * Catch 36-bit addresses
	 */
	if (md->pfn >= 0x100000) {
		create_36bit_mapping(md, type);
		return;
	}
#endif

	addr = md->virtual & PAGE_MASK;
	phys = __pfn_to_phys(md->pfn);
	length = PAGE_ALIGN(md->length + (md->virtual & ~PAGE_MASK));

	if (type->prot_l1 == 0 && ((addr | phys | length) & ~SECTION_MASK)) {
		printk(KERN_WARNING "BUG: map for 0x%08llx at 0x%08lx can not "
		       "be mapped using pages, ignoring.\n",
		       (long long)__pfn_to_phys(md->pfn), addr);
		return;
	}

	pgd = pgd_offset_k(addr);
	end = addr + length;
	do {
		unsigned long next = pgd_addr_end(addr, end);

		alloc_init_pud(pgd, addr, next, phys, type);

		phys += next - addr;
		addr = next;
	} while (pgd++, addr != end);
=======
	if (md->virtual != vectors_base() && md->virtual < TASK_SIZE) {
		pr_warn("BUG: not creating mapping for 0x%08llx at 0x%08lx in user region\n",
			(long long)__pfn_to_phys((u64)md->pfn), md->virtual);
		return;
	}

	if (md->type == MT_DEVICE &&
	    md->virtual >= PAGE_OFFSET && md->virtual < FIXADDR_START &&
	    (md->virtual < VMALLOC_START || md->virtual >= VMALLOC_END)) {
		pr_warn("BUG: mapping for 0x%08llx at 0x%08lx out of vmalloc space\n",
			(long long)__pfn_to_phys((u64)md->pfn), md->virtual);
	}

	__create_mapping(&init_mm, md, early_alloc, false);
}

void __init create_mapping_late(struct mm_struct *mm, struct map_desc *md,
				bool ng)
{
#ifdef CONFIG_ARM_LPAE
	p4d_t *p4d;
	pud_t *pud;

	p4d = p4d_alloc(mm, pgd_offset(mm, md->virtual), md->virtual);
	if (WARN_ON(!p4d))
		return;
	pud = pud_alloc(mm, p4d, md->virtual);
	if (WARN_ON(!pud))
		return;
	pmd_alloc(mm, pud, 0);
#endif
	__create_mapping(mm, md, late_alloc, ng);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

/*
 * Create the architecture specific mappings
 */
void __init iotable_init(struct map_desc *io_desc, int nr)
{
	struct map_desc *md;
	struct vm_struct *vm;
<<<<<<< HEAD
	int rc = 0;
=======
	struct static_vm *svm;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	if (!nr)
		return;

<<<<<<< HEAD
	vm = early_alloc_aligned(sizeof(*vm) * nr, __alignof__(*vm));

	for (md = io_desc; nr; md++, nr--) {
		create_mapping(md);
=======
	svm = memblock_alloc(sizeof(*svm) * nr, __alignof__(*svm));
	if (!svm)
		panic("%s: Failed to allocate %zu bytes align=0x%zx\n",
		      __func__, sizeof(*svm) * nr, __alignof__(*svm));

	for (md = io_desc; nr; md++, nr--) {
		create_mapping(md);

		vm = &svm->vm;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		vm->addr = (void *)(md->virtual & PAGE_MASK);
		vm->size = PAGE_ALIGN(md->length + (md->virtual & ~PAGE_MASK));
		vm->phys_addr = __pfn_to_phys(md->pfn);
		vm->flags = VM_IOREMAP | VM_ARM_STATIC_MAPPING;
		vm->flags |= VM_ARM_MTYPE(md->type);
		vm->caller = iotable_init;
<<<<<<< HEAD
		rc = vm_area_check_early(vm);
		if (!rc)
			vm_area_add_early(vm++);
	}
}

=======
		add_static_vm_early(svm++);
	}
}

void __init vm_reserve_area_early(unsigned long addr, unsigned long size,
				  void *caller)
{
	struct vm_struct *vm;
	struct static_vm *svm;

	svm = memblock_alloc(sizeof(*svm), __alignof__(*svm));
	if (!svm)
		panic("%s: Failed to allocate %zu bytes align=0x%zx\n",
		      __func__, sizeof(*svm), __alignof__(*svm));

	vm = &svm->vm;
	vm->addr = (void *)addr;
	vm->size = size;
	vm->flags = VM_IOREMAP | VM_ARM_EMPTY_MAPPING;
	vm->caller = caller;
	add_static_vm_early(svm);
}

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#ifndef CONFIG_ARM_LPAE

/*
 * The Linux PMD is made of two consecutive section entries covering 2MB
 * (see definition in include/asm/pgtable-2level.h).  However a call to
 * create_mapping() may optimize static mappings by using individual
 * 1MB section mappings.  This leaves the actual PMD potentially half
 * initialized if the top or bottom section entry isn't used, leaving it
 * open to problems if a subsequent ioremap() or vmalloc() tries to use
 * the virtual space left free by that unused section entry.
 *
 * Let's avoid the issue by inserting dummy vm entries covering the unused
 * PMD halves once the static mappings are in place.
 */

static void __init pmd_empty_section_gap(unsigned long addr)
{
<<<<<<< HEAD
	struct vm_struct *vm;

	vm = early_alloc_aligned(sizeof(*vm), __alignof__(*vm));
	vm->addr = (void *)addr;
	vm->size = SECTION_SIZE;
	vm->flags = VM_IOREMAP | VM_ARM_EMPTY_MAPPING;
	vm->caller = pmd_empty_section_gap;
	vm_area_add_early(vm);
=======
	vm_reserve_area_early(addr, SECTION_SIZE, pmd_empty_section_gap);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

static void __init fill_pmd_gaps(void)
{
<<<<<<< HEAD
=======
	struct static_vm *svm;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	struct vm_struct *vm;
	unsigned long addr, next = 0;
	pmd_t *pmd;

<<<<<<< HEAD
	/* we're still single threaded hence no lock needed here */
	for (vm = vmlist; vm; vm = vm->next) {
		if (!(vm->flags & (VM_ARM_STATIC_MAPPING | VM_ARM_EMPTY_MAPPING)))
			continue;
=======
	list_for_each_entry(svm, &static_vmlist, list) {
		vm = &svm->vm;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		addr = (unsigned long)vm->addr;
		if (addr < next)
			continue;

		/*
		 * Check if this vm starts on an odd section boundary.
		 * If so and the first section entry for this PMD is free
		 * then we block the corresponding virtual address.
		 */
		if ((addr & ~PMD_MASK) == SECTION_SIZE) {
			pmd = pmd_off_k(addr);
			if (pmd_none(*pmd))
				pmd_empty_section_gap(addr & PMD_MASK);
		}

		/*
		 * Then check if this vm ends on an odd section boundary.
		 * If so and the second section entry for this PMD is empty
		 * then we block the corresponding virtual address.
		 */
		addr += vm->size;
		if ((addr & ~PMD_MASK) == SECTION_SIZE) {
			pmd = pmd_off_k(addr) + 1;
			if (pmd_none(*pmd))
				pmd_empty_section_gap(addr);
		}

		/* no need to look at any vm entry until we hit the next PMD */
		next = (addr + PMD_SIZE - 1) & PMD_MASK;
	}
}

#else
#define fill_pmd_gaps() do { } while (0)
#endif

<<<<<<< HEAD
static void * __initdata vmalloc_min =
	(void *)(VMALLOC_END - (240 << 20) - VMALLOC_OFFSET);
=======
#if defined(CONFIG_PCI) && !defined(CONFIG_NEED_MACH_IO_H)
static void __init pci_reserve_io(void)
{
	struct static_vm *svm;

	svm = find_static_vm_vaddr((void *)PCI_IO_VIRT_BASE);
	if (svm)
		return;

	vm_reserve_area_early(PCI_IO_VIRT_BASE, SZ_2M, pci_reserve_io);
}
#else
#define pci_reserve_io() do { } while (0)
#endif

#ifdef CONFIG_DEBUG_LL
void __init debug_ll_io_init(void)
{
	struct map_desc map;

	debug_ll_addr(&map.pfn, &map.virtual);
	if (!map.pfn || !map.virtual)
		return;
	map.pfn = __phys_to_pfn(map.pfn);
	map.virtual &= PAGE_MASK;
	map.length = PAGE_SIZE;
	map.type = MT_DEVICE;
	iotable_init(&map, 1);
}
#endif

static unsigned long __initdata vmalloc_size = 240 * SZ_1M;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

/*
 * vmalloc=size forces the vmalloc area to be exactly 'size'
 * bytes. This can be used to increase (or decrease) the vmalloc
<<<<<<< HEAD
 * area - the default is 240m.
=======
 * area - the default is 240MiB.
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 */
static int __init early_vmalloc(char *arg)
{
	unsigned long vmalloc_reserve = memparse(arg, NULL);
<<<<<<< HEAD

	if (vmalloc_reserve < SZ_16M) {
		vmalloc_reserve = SZ_16M;
		printk(KERN_WARNING
			"vmalloc area too small, limiting to %luMB\n",
			vmalloc_reserve >> 20);
	}

	if (vmalloc_reserve > VMALLOC_END - (PAGE_OFFSET + SZ_32M)) {
		vmalloc_reserve = VMALLOC_END - (PAGE_OFFSET + SZ_32M);
		printk(KERN_WARNING
			"vmalloc area is too big, limiting to %luMB\n",
			vmalloc_reserve >> 20);
	}

	vmalloc_min = (void *)(VMALLOC_END - vmalloc_reserve);
=======
	unsigned long vmalloc_max;

	if (vmalloc_reserve < SZ_16M) {
		vmalloc_reserve = SZ_16M;
		pr_warn("vmalloc area is too small, limiting to %luMiB\n",
			vmalloc_reserve >> 20);
	}

	vmalloc_max = VMALLOC_END - (PAGE_OFFSET + SZ_32M + VMALLOC_OFFSET);
	if (vmalloc_reserve > vmalloc_max) {
		vmalloc_reserve = vmalloc_max;
		pr_warn("vmalloc area is too big, limiting to %luMiB\n",
			vmalloc_reserve >> 20);
	}

	vmalloc_size = vmalloc_reserve;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	return 0;
}
early_param("vmalloc", early_vmalloc);

phys_addr_t arm_lowmem_limit __initdata = 0;

<<<<<<< HEAD
void __init sanity_check_meminfo(void)
{
	int i, j, highmem = 0;

#ifdef CONFIG_ENABLE_VMALLOC_SAVING
	unsigned long hole_start;
	for (i = 0; i < (meminfo.nr_banks - 1); i++) {
		hole_start = meminfo.bank[i].start + meminfo.bank[i].size;
		if (hole_start != meminfo.bank[i+1].start) {
			if (hole_start <= MAX_HOLE_ADDRESS) {
				vmalloc_min = (void *) (vmalloc_min +
				(meminfo.bank[i+1].start - hole_start));
			}
		}
	}
#endif
#ifdef CONFIG_DONT_MAP_HOLE_AFTER_MEMBANK0
	find_memory_hole();
#endif

	for (i = 0, j = 0; i < meminfo.nr_banks; i++) {
		struct membank *bank = &meminfo.bank[j];
		*bank = meminfo.bank[i];

		if (bank->start > ULONG_MAX)
			highmem = 1;

#ifdef CONFIG_HIGHMEM
		if (__va(bank->start) >= vmalloc_min ||
		    __va(bank->start) < (void *)PAGE_OFFSET)
			highmem = 1;

		bank->highmem = highmem;

		/*
		 * Split those memory banks which are partially overlapping
		 * the vmalloc area greatly simplifying things later.
		 */
		if (!highmem && __va(bank->start) < vmalloc_min &&
		    bank->size > vmalloc_min - __va(bank->start)) {
			if (meminfo.nr_banks >= NR_BANKS) {
				printk(KERN_CRIT "NR_BANKS too low, "
						 "ignoring high memory\n");
			} else {
				memmove(bank + 1, bank,
					(meminfo.nr_banks - i) * sizeof(*bank));
				meminfo.nr_banks++;
				i++;
				bank[1].size -= vmalloc_min - __va(bank->start);
				bank[1].start = __pa(vmalloc_min - 1) + 1;
				bank[1].highmem = highmem = 1;
				j++;
			}
			bank->size = vmalloc_min - __va(bank->start);
		}
#else
		bank->highmem = highmem;

		/*
		 * Highmem banks not allowed with !CONFIG_HIGHMEM.
		 */
		if (highmem) {
			printk(KERN_NOTICE "Ignoring RAM at %.8llx-%.8llx "
			       "(!CONFIG_HIGHMEM).\n",
			       (unsigned long long)bank->start,
			       (unsigned long long)bank->start + bank->size - 1);
			continue;
		}

		/*
		 * Check whether this memory bank would entirely overlap
		 * the vmalloc area.
		 */
		if (__va(bank->start) >= vmalloc_min ||
		    __va(bank->start) < (void *)PAGE_OFFSET) {
			printk(KERN_NOTICE "Ignoring RAM at %.8llx-%.8llx "
			       "(vmalloc region overlap).\n",
			       (unsigned long long)bank->start,
			       (unsigned long long)bank->start + bank->size - 1);
			continue;
		}

		/*
		 * Check whether this memory bank would partially overlap
		 * the vmalloc area.
		 */
		if (__va(bank->start + bank->size) > vmalloc_min ||
		    __va(bank->start + bank->size) < __va(bank->start)) {
			unsigned long newsize = vmalloc_min - __va(bank->start);
			printk(KERN_NOTICE "Truncating RAM at %.8llx-%.8llx "
			       "to -%.8llx (vmalloc region overlap).\n",
			       (unsigned long long)bank->start,
			       (unsigned long long)bank->start + bank->size - 1,
			       (unsigned long long)bank->start + newsize - 1);
			bank->size = newsize;
		}
#endif
		if (!bank->highmem && bank->start + bank->size > arm_lowmem_limit)
			arm_lowmem_limit = bank->start + bank->size;

		j++;
	}
#ifdef CONFIG_HIGHMEM
	if (highmem) {
		const char *reason = NULL;

		if (cache_is_vipt_aliasing()) {
			/*
			 * Interactions between kmap and other mappings
			 * make highmem support with aliasing VIPT caches
			 * rather difficult.
			 */
			reason = "with VIPT aliasing cache";
		}
		if (reason) {
			printk(KERN_CRIT "HIGHMEM is not supported %s, ignoring high memory\n",
				reason);
			while (j > 0 && meminfo.bank[j - 1].highmem)
				j--;
		}
	}
#endif
	meminfo.nr_banks = j;
	high_memory = __va(arm_lowmem_limit - 1) + 1;
	memblock_set_current_limit(arm_lowmem_limit);
}

static inline void prepare_page_table(void)
=======
void __init adjust_lowmem_bounds(void)
{
	phys_addr_t block_start, block_end, memblock_limit = 0;
	u64 vmalloc_limit, i;
	phys_addr_t lowmem_limit = 0;

	/*
	 * Let's use our own (unoptimized) equivalent of __pa() that is
	 * not affected by wrap-arounds when sizeof(phys_addr_t) == 4.
	 * The result is used as the upper bound on physical memory address
	 * and may itself be outside the valid range for which phys_addr_t
	 * and therefore __pa() is defined.
	 */
	vmalloc_limit = (u64)VMALLOC_END - vmalloc_size - VMALLOC_OFFSET -
			PAGE_OFFSET + PHYS_OFFSET;

	/*
	 * The first usable region must be PMD aligned. Mark its start
	 * as MEMBLOCK_NOMAP if it isn't
	 */
	for_each_mem_range(i, &block_start, &block_end) {
		if (!IS_ALIGNED(block_start, PMD_SIZE)) {
			phys_addr_t len;

			len = round_up(block_start, PMD_SIZE) - block_start;
			memblock_mark_nomap(block_start, len);
		}
		break;
	}

	for_each_mem_range(i, &block_start, &block_end) {
		if (block_start < vmalloc_limit) {
			if (block_end > lowmem_limit)
				/*
				 * Compare as u64 to ensure vmalloc_limit does
				 * not get truncated. block_end should always
				 * fit in phys_addr_t so there should be no
				 * issue with assignment.
				 */
				lowmem_limit = min_t(u64,
							 vmalloc_limit,
							 block_end);

			/*
			 * Find the first non-pmd-aligned page, and point
			 * memblock_limit at it. This relies on rounding the
			 * limit down to be pmd-aligned, which happens at the
			 * end of this function.
			 *
			 * With this algorithm, the start or end of almost any
			 * bank can be non-pmd-aligned. The only exception is
			 * that the start of the bank 0 must be section-
			 * aligned, since otherwise memory would need to be
			 * allocated when mapping the start of bank 0, which
			 * occurs before any free memory is mapped.
			 */
			if (!memblock_limit) {
				if (!IS_ALIGNED(block_start, PMD_SIZE))
					memblock_limit = block_start;
				else if (!IS_ALIGNED(block_end, PMD_SIZE))
					memblock_limit = lowmem_limit;
			}

		}
	}

	arm_lowmem_limit = lowmem_limit;

	high_memory = __va(arm_lowmem_limit - 1) + 1;

	if (!memblock_limit)
		memblock_limit = arm_lowmem_limit;

	/*
	 * Round the memblock limit down to a pmd size.  This
	 * helps to ensure that we will allocate memory from the
	 * last full pmd, which should be mapped.
	 */
	memblock_limit = round_down(memblock_limit, PMD_SIZE);

	if (!IS_ENABLED(CONFIG_HIGHMEM) || cache_is_vipt_aliasing()) {
		if (memblock_end_of_DRAM() > arm_lowmem_limit) {
			phys_addr_t end = memblock_end_of_DRAM();

			pr_notice("Ignoring RAM at %pa-%pa\n",
				  &memblock_limit, &end);
			pr_notice("Consider using a HIGHMEM enabled kernel.\n");

			memblock_remove(memblock_limit, end - memblock_limit);
		}
	}

	memblock_set_current_limit(memblock_limit);
}

static __init void prepare_page_table(void)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
{
	unsigned long addr;
	phys_addr_t end;

	/*
	 * Clear out all the mappings below the kernel image.
	 */
<<<<<<< HEAD
	for (addr = 0; addr < MODULES_VADDR; addr += PMD_SIZE)
		pmd_clear(pmd_off_k(addr));

#ifdef CONFIG_XIP_KERNEL
	/* The XIP kernel is mapped in the module area -- skip over it */
	addr = ((unsigned long)_etext + PMD_SIZE - 1) & PMD_MASK;
=======
#ifdef CONFIG_KASAN
	/*
	 * KASan's shadow memory inserts itself between the TASK_SIZE
	 * and MODULES_VADDR. Do not clear the KASan shadow memory mappings.
	 */
	for (addr = 0; addr < KASAN_SHADOW_START; addr += PMD_SIZE)
		pmd_clear(pmd_off_k(addr));
	/*
	 * Skip over the KASan shadow area. KASAN_SHADOW_END is sometimes
	 * equal to MODULES_VADDR and then we exit the pmd clearing. If we
	 * are using a thumb-compiled kernel, there there will be 8MB more
	 * to clear as KASan always offset to 16 MB below MODULES_VADDR.
	 */
	for (addr = KASAN_SHADOW_END; addr < MODULES_VADDR; addr += PMD_SIZE)
		pmd_clear(pmd_off_k(addr));
#else
	for (addr = 0; addr < MODULES_VADDR; addr += PMD_SIZE)
		pmd_clear(pmd_off_k(addr));
#endif

#ifdef CONFIG_XIP_KERNEL
	/* The XIP kernel is mapped in the module area -- skip over it */
	addr = ((unsigned long)_exiprom + PMD_SIZE - 1) & PMD_MASK;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#endif
	for ( ; addr < PAGE_OFFSET; addr += PMD_SIZE)
		pmd_clear(pmd_off_k(addr));

	/*
	 * Find the end of the first block of lowmem.
	 */
	end = memblock.memory.regions[0].base + memblock.memory.regions[0].size;
	if (end >= arm_lowmem_limit)
		end = arm_lowmem_limit;

	/*
	 * Clear out all the kernel space mappings, except for the first
	 * memory bank, up to the vmalloc region.
	 */
	for (addr = __phys_to_virt(end);
	     addr < VMALLOC_START; addr += PMD_SIZE)
		pmd_clear(pmd_off_k(addr));
}

#ifdef CONFIG_ARM_LPAE
/* the first page is reserved for pgd */
#define SWAPPER_PG_DIR_SIZE	(PAGE_SIZE + \
				 PTRS_PER_PGD * PTRS_PER_PMD * sizeof(pmd_t))
#else
#define SWAPPER_PG_DIR_SIZE	(PTRS_PER_PGD * sizeof(pgd_t))
#endif

/*
 * Reserve the special regions of memory
 */
void __init arm_mm_memblock_reserve(void)
{
	/*
	 * Reserve the page tables.  These are already in use,
	 * and can only be in node 0.
	 */
	memblock_reserve(__pa(swapper_pg_dir), SWAPPER_PG_DIR_SIZE);

#ifdef CONFIG_SA1111
	/*
	 * Because of the SA1111 DMA bug, we want to preserve our
	 * precious DMA-able memory...
	 */
	memblock_reserve(PHYS_OFFSET, __pa(swapper_pg_dir) - PHYS_OFFSET);
#endif
}

/*
 * Set up the device mappings.  Since we clear out the page tables for all
<<<<<<< HEAD
 * mappings above VMALLOC_START, we will remove any debug device mappings.
 * This means you have to be careful how you debug this function, or any
 * called function.  This means you can't use any function or debugging
 * method which may touch any device, otherwise the kernel _will_ crash.
 */
static void __init devicemaps_init(struct machine_desc *mdesc)
=======
 * mappings above VMALLOC_START, except early fixmap, we might remove debug
 * device mappings.  This means earlycon can be used to debug this function
 * Any other function or debugging method which may touch any device _will_
 * crash the kernel.
 */
static void __init devicemaps_init(const struct machine_desc *mdesc)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
{
	struct map_desc map;
	unsigned long addr;
	void *vectors;

	/*
	 * Allocate the vector page early.
	 */
	vectors = early_alloc(PAGE_SIZE * 2);

	early_trap_init(vectors);

<<<<<<< HEAD
	for (addr = VMALLOC_START; addr; addr += PMD_SIZE)
		pmd_clear(pmd_off_k(addr));

=======
	/*
	 * Clear page table except top pmd used by early fixmaps
	 */
	for (addr = VMALLOC_START; addr < (FIXADDR_TOP & PMD_MASK); addr += PMD_SIZE)
		pmd_clear(pmd_off_k(addr));

	if (__atags_pointer) {
		/* create a read-only mapping of the device tree */
		map.pfn = __phys_to_pfn(__atags_pointer & SECTION_MASK);
		map.virtual = FDT_FIXED_BASE;
		map.length = FDT_FIXED_SIZE;
		map.type = MT_MEMORY_RO;
		create_mapping(&map);
	}

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	/*
	 * Map the kernel if it is XIP.
	 * It is always first in the modulearea.
	 */
#ifdef CONFIG_XIP_KERNEL
	map.pfn = __phys_to_pfn(CONFIG_XIP_PHYS_ADDR & SECTION_MASK);
	map.virtual = MODULES_VADDR;
<<<<<<< HEAD
	map.length = ((unsigned long)_etext - map.virtual + ~SECTION_MASK) & SECTION_MASK;
=======
	map.length = ((unsigned long)_exiprom - map.virtual + ~SECTION_MASK) & SECTION_MASK;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	map.type = MT_ROM;
	create_mapping(&map);
#endif

	/*
	 * Map the cache flushing regions.
	 */
#ifdef FLUSH_BASE
	map.pfn = __phys_to_pfn(FLUSH_BASE_PHYS);
	map.virtual = FLUSH_BASE;
	map.length = SZ_1M;
	map.type = MT_CACHECLEAN;
	create_mapping(&map);
#endif
#ifdef FLUSH_BASE_MINICACHE
	map.pfn = __phys_to_pfn(FLUSH_BASE_PHYS + SZ_1M);
	map.virtual = FLUSH_BASE_MINICACHE;
	map.length = SZ_1M;
	map.type = MT_MINICLEAN;
	create_mapping(&map);
#endif

	/*
	 * Create a mapping for the machine vectors at the high-vectors
	 * location (0xffff0000).  If we aren't using high-vectors, also
	 * create a mapping at the low-vectors virtual address.
	 */
	map.pfn = __phys_to_pfn(virt_to_phys(vectors));
	map.virtual = 0xffff0000;
	map.length = PAGE_SIZE;
#ifdef CONFIG_KUSER_HELPERS
	map.type = MT_HIGH_VECTORS;
#else
	map.type = MT_LOW_VECTORS;
#endif
	create_mapping(&map);

	if (!vectors_high()) {
		map.virtual = 0;
		map.length = PAGE_SIZE * 2;
		map.type = MT_LOW_VECTORS;
		create_mapping(&map);
	}

	/* Now create a kernel read-only mapping */
	map.pfn += 1;
	map.virtual = 0xffff0000 + PAGE_SIZE;
	map.length = PAGE_SIZE;
	map.type = MT_LOW_VECTORS;
	create_mapping(&map);

	/*
	 * Ask the machine support to map in the statically mapped devices.
	 */
	if (mdesc->map_io)
		mdesc->map_io();
<<<<<<< HEAD
	fill_pmd_gaps();

	if (use_user_accessible_timers()) {
		/*
		 * Generate a mapping for the timer page.
		 */
		int page_addr = get_timer_page_address();
		if (page_addr != ARM_USER_ACCESSIBLE_TIMERS_INVALID_PAGE) {
			map.pfn = __phys_to_pfn(page_addr);
			map.virtual = CONFIG_ARM_USER_ACCESSIBLE_TIMER_BASE;
			map.length = PAGE_SIZE;
			map.type = MT_DEVICE_USER_ACCESSIBLE;
			create_mapping(&map);
		}
	}
=======
	else
		debug_ll_io_init();
	fill_pmd_gaps();

	/* Reserve fixed i/o space in VMALLOC region */
	pci_reserve_io();
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	/*
	 * Finally flush the caches and tlb to ensure that we're in a
	 * consistent state wrt the writebuffer.  This also ensures that
	 * any write-allocated cache lines in the vector page are written
	 * back.  After this point, we can start to touch devices again.
	 */
	local_flush_tlb_all();
	flush_cache_all();
<<<<<<< HEAD
=======

	/* Enable asynchronous aborts */
	early_abt_enable();
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

static void __init kmap_init(void)
{
#ifdef CONFIG_HIGHMEM
	pkmap_page_table = early_pte_alloc(pmd_off_k(PKMAP_BASE),
		PKMAP_BASE, _PAGE_KERNEL_TABLE);
#endif
<<<<<<< HEAD
}

#ifdef CONFIG_STRICT_MEMORY_RWX
static struct {
	pmd_t *pmd_to_flush;
	pmd_t *pmd;
	unsigned long addr;
	pmd_t saved_pmd;
	bool made_writeable;
} mem_unprotect;

static DEFINE_SPINLOCK(mem_text_writeable_lock);

void mem_text_writeable_spinlock(unsigned long *flags)
{
	spin_lock_irqsave(&mem_text_writeable_lock, *flags);
}

void mem_text_writeable_spinunlock(unsigned long *flags)
{
	spin_unlock_irqrestore(&mem_text_writeable_lock, *flags);
}

/*
 * mem_text_address_writeable() and mem_text_address_restore()
 * should be called as a pair. They are used to make the
 * specified address in the kernel text section temporarily writeable
 * when it has been marked read-only by STRICT_MEMORY_RWX.
 * Used by kprobes and other debugging tools to set breakpoints etc.
 * mem_text_address_writeable() is invoked before writing.
 * After the write, mem_text_address_restore() must be called
 * to restore the original state.
 * This is only effective when used on the kernel text section
 * marked as MEMORY_RX by map_lowmem()
 *
 * They must each be called with mem_text_writeable_lock locked
 * by the caller, with no unlocking between the calls.
 * The caller should release mem_text_writeable_lock immediately
 * after the call to mem_text_address_restore().
 * Only the write and associated cache operations should be performed
 * between the calls.
 */

/* this function must be called with mem_text_writeable_lock held */
void mem_text_address_writeable(unsigned long addr)
{
	struct task_struct *tsk = current;
	struct mm_struct *mm = tsk->active_mm;
	pgd_t *pgd = pgd_offset(mm, addr);
	pud_t *pud = pud_offset(pgd, addr);

	mem_unprotect.made_writeable = 0;

	if ((addr < (unsigned long)RX_AREA_START) ||
	    (addr >= (unsigned long)RX_AREA_END))
		return;

	mem_unprotect.pmd = pmd_offset(pud, addr);
	mem_unprotect.pmd_to_flush = mem_unprotect.pmd;
	mem_unprotect.addr = addr & PAGE_MASK;

	if (addr & SECTION_SIZE)
			mem_unprotect.pmd++;

	mem_unprotect.saved_pmd = *mem_unprotect.pmd;
	if ((mem_unprotect.saved_pmd & PMD_TYPE_MASK) != PMD_TYPE_SECT)
		return;

	*mem_unprotect.pmd &= ~PMD_SECT_APX;

	flush_pmd_entry(mem_unprotect.pmd_to_flush);
	flush_tlb_kernel_page(mem_unprotect.addr);
	mem_unprotect.made_writeable = 1;
}

/* this function must be called with mem_text_writeable_lock held */
void mem_text_address_restore(void)
{
	if (mem_unprotect.made_writeable) {
		*mem_unprotect.pmd = mem_unprotect.saved_pmd;
		flush_pmd_entry(mem_unprotect.pmd_to_flush);
		flush_tlb_kernel_page(mem_unprotect.addr);
	}
}
#endif

void mem_text_write_kernel_word(unsigned long *addr, unsigned long word)
{
	unsigned long flags;

	mem_text_writeable_spinlock(&flags);
	mem_text_address_writeable((unsigned long)addr);
	*addr = word;
	flush_icache_range((unsigned long)addr,
			   ((unsigned long)addr + sizeof(long)));
	mem_text_address_restore();
	mem_text_writeable_spinunlock(&flags);
}
EXPORT_SYMBOL(mem_text_write_kernel_word);

static void __init map_lowmem(void)
{
	struct memblock_region *reg;
	struct vm_struct *vm;
	phys_addr_t start;
	phys_addr_t end;
	unsigned long vaddr;
	unsigned long pfn;
	unsigned long length;
	unsigned int type;
	int nr = 0;

	/* Map all the lowmem memory banks. */
	for_each_memblock(memory, reg) {
		struct map_desc map;
		nr++;
		start = reg->base;
		end = start + reg->size;

=======

	early_pte_alloc(pmd_off_k(FIXADDR_START), FIXADDR_START,
			_PAGE_KERNEL_TABLE);
}

static void __init map_lowmem(void)
{
	phys_addr_t start, end;
	u64 i;

	/* Map all the lowmem memory banks. */
	for_each_mem_range(i, &start, &end) {
		struct map_desc map;

		pr_debug("map lowmem start: 0x%08llx, end: 0x%08llx\n",
			 (long long)start, (long long)end);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		if (end > arm_lowmem_limit)
			end = arm_lowmem_limit;
		if (start >= end)
			break;

<<<<<<< HEAD
		map.pfn = __phys_to_pfn(start);
		map.virtual = __phys_to_virt(start);
#ifdef CONFIG_STRICT_MEMORY_RWX
		if (start <= __pa(_text) && __pa(_text) < end) {
			map.length = SECTION_SIZE;
			map.type = MT_MEMORY_RW;

			create_mapping(&map);

			map.pfn = __phys_to_pfn(start + SECTION_SIZE);
			map.virtual = __phys_to_virt(start + SECTION_SIZE);
			map.length = (unsigned long)RX_AREA_END - map.virtual;
			map.type = MT_MEMORY_RX;

			create_mapping(&map);

			map.pfn = __phys_to_pfn(__pa(__start_rodata));
			map.virtual = (unsigned long)__start_rodata;
			map.length = __init_begin - __start_rodata;
			map.type = MT_MEMORY_R;

			create_mapping(&map);

			map.pfn = __phys_to_pfn(__pa(__init_begin));
			map.virtual = (unsigned long)__init_begin;
			map.length = (char *)__arch_info_begin - __init_begin;
			map.type = MT_MEMORY_RX;

			create_mapping(&map);

			map.pfn = __phys_to_pfn(__pa(__arch_info_begin));
			map.virtual = (unsigned long)__arch_info_begin;
			map.length = __phys_to_virt(end) -
				(unsigned long)__arch_info_begin;
			map.type = MT_MEMORY_RW;
		} else {
			map.length = end - start;
			map.type = MT_MEMORY_RW;
		}
#else
		map.length = end - start;
		map.type = MT_MEMORY;
#endif

		create_mapping(&map);
	}

	vm = early_alloc_aligned(sizeof(*vm) * nr, __alignof__(*vm));

	for_each_memblock(memory, reg) {

		start = reg->base;
		end = start + reg->size;

		if (end > arm_lowmem_limit)
			end = arm_lowmem_limit;
		if (start >= end)
			break;

		pfn = __phys_to_pfn(start);
		vaddr = __phys_to_virt(start);
		length = end - start;
		type = MT_MEMORY;

		vm->addr = (void *)(vaddr & PAGE_MASK);
		vm->size = PAGE_ALIGN(length + (vaddr & ~PAGE_MASK));
		vm->phys_addr = __pfn_to_phys(pfn);
		vm->flags = VM_LOWMEM | VM_ARM_STATIC_MAPPING;
		vm->flags |= VM_ARM_MTYPE(type);
		vm->caller = map_lowmem;
		vm_area_add_early(vm);
		mark_vmalloc_reserved_area(vm->addr, vm->size);
		vm++;
	}
}

#ifdef CONFIG_FORCE_PAGES
/*
 * remap a PMD into pages
 * We split a single pmd here none of this two pmd nonsense
 */
static noinline void split_pmd(pmd_t *pmd, unsigned long addr,
				unsigned long end, unsigned long pfn,
				const struct mem_type *type)
{
	pte_t *pte, *start_pte;
	pmd_t *base_pmd;

	base_pmd = pmd_offset(
			pud_offset(pgd_offset(&init_mm, addr), addr), addr);

	if (pmd_none(*base_pmd) || pmd_bad(*base_pmd)) {
		start_pte = early_alloc(PTE_HWTABLE_OFF + PTE_HWTABLE_SIZE);
#ifndef CONFIG_ARM_LPAE
		/*
		 * Following is needed when new pte is allocated for pmd[1]
		 * cases, which may happen when base (start) address falls
		 * under pmd[1].
		 */
		if (addr & SECTION_SIZE)
			start_pte += pte_index(addr);
#endif
	} else {
		start_pte = pte_offset_kernel(base_pmd, addr);
	}

	pte = start_pte;

	do {
		set_pte_ext(pte, pfn_pte(pfn, type->prot_pte), 0);
		pfn++;
	} while (pte++, addr += PAGE_SIZE, addr != end);

	*pmd = __pmd((__pa(start_pte) + PTE_HWTABLE_OFF) | type->prot_l1);
	mb();
	flush_pmd_entry(pmd);
	flush_tlb_all();
}

/*
 * It's significantly easier to remap as pages later after all memory is
 * mapped. Everything is sections so all we have to do is split
 */
static void __init remap_pages(void)
{
	struct memblock_region *reg;

	for_each_memblock(memory, reg) {
		phys_addr_t phys_start = reg->base;
		phys_addr_t phys_end = reg->base + reg->size;
		unsigned long addr = (unsigned long)__va(phys_start);
		unsigned long end = (unsigned long)__va(phys_end);
		pmd_t *pmd = NULL;
		unsigned long next;
		unsigned long pfn = __phys_to_pfn(phys_start);
		bool fixup = false;
		unsigned long saved_start = addr;

		if (phys_end > arm_lowmem_limit)
			end = (unsigned long)__va(arm_lowmem_limit);
		if (phys_start >= phys_end)
			break;

		pmd = pmd_offset(
			pud_offset(pgd_offset(&init_mm, addr), addr), addr);

#ifndef	CONFIG_ARM_LPAE
		if (addr & SECTION_SIZE) {
			fixup = true;
			pmd_empty_section_gap((addr - SECTION_SIZE) & PMD_MASK);
			pmd++;
		}

		if (end & SECTION_SIZE)
			pmd_empty_section_gap(end);
#endif

		do {
			next = addr + SECTION_SIZE;

			if (pmd_none(*pmd) || pmd_bad(*pmd))
				split_pmd(pmd, addr, next, pfn,
						&mem_types[MT_MEMORY]);
			pmd++;
			pfn += SECTION_SIZE >> PAGE_SHIFT;

		} while (addr = next, addr < end);

		if (fixup) {
			/*
			 * Put a faulting page table here to avoid detecting no
			 * pmd when accessing an odd section boundary. This
			 * needs to be faulting to help catch errors and avoid
			 * speculation
			 */
			pmd = pmd_off_k(saved_start);
			pmd[0] = pmd[1] & ~1;
		}
	}
}
#else
static void __init remap_pages(void)
{

}
#endif

=======
		/*
		 * If our kernel image is in the VMALLOC area we need to remove
		 * the kernel physical memory from lowmem since the kernel will
		 * be mapped separately.
		 *
		 * The kernel will typically be at the very start of lowmem,
		 * but any placement relative to memory ranges is possible.
		 *
		 * If the memblock contains the kernel, we have to chisel out
		 * the kernel memory from it and map each part separately. We
		 * get 6 different theoretical cases:
		 *
		 *                            +--------+ +--------+
		 *  +-- start --+  +--------+ | Kernel | | Kernel |
		 *  |           |  | Kernel | | case 2 | | case 5 |
		 *  |           |  | case 1 | +--------+ |        | +--------+
		 *  |  Memory   |  +--------+            |        | | Kernel |
		 *  |  range    |  +--------+            |        | | case 6 |
		 *  |           |  | Kernel | +--------+ |        | +--------+
		 *  |           |  | case 3 | | Kernel | |        |
		 *  +-- end ----+  +--------+ | case 4 | |        |
		 *                            +--------+ +--------+
		 */

		/* Case 5: kernel covers range, don't map anything, should be rare */
		if ((start > kernel_sec_start) && (end < kernel_sec_end))
			break;

		/* Cases where the kernel is starting inside the range */
		if ((kernel_sec_start >= start) && (kernel_sec_start <= end)) {
			/* Case 6: kernel is embedded in the range, we need two mappings */
			if ((start < kernel_sec_start) && (end > kernel_sec_end)) {
				/* Map memory below the kernel */
				map.pfn = __phys_to_pfn(start);
				map.virtual = __phys_to_virt(start);
				map.length = kernel_sec_start - start;
				map.type = MT_MEMORY_RW;
				create_mapping(&map);
				/* Map memory above the kernel */
				map.pfn = __phys_to_pfn(kernel_sec_end);
				map.virtual = __phys_to_virt(kernel_sec_end);
				map.length = end - kernel_sec_end;
				map.type = MT_MEMORY_RW;
				create_mapping(&map);
				break;
			}
			/* Case 1: kernel and range start at the same address, should be common */
			if (kernel_sec_start == start)
				start = kernel_sec_end;
			/* Case 3: kernel and range end at the same address, should be rare */
			if (kernel_sec_end == end)
				end = kernel_sec_start;
		} else if ((kernel_sec_start < start) && (kernel_sec_end > start) && (kernel_sec_end < end)) {
			/* Case 2: kernel ends inside range, starts below it */
			start = kernel_sec_end;
		} else if ((kernel_sec_start > start) && (kernel_sec_start < end) && (kernel_sec_end > end)) {
			/* Case 4: kernel starts inside range, ends above it */
			end = kernel_sec_start;
		}
		map.pfn = __phys_to_pfn(start);
		map.virtual = __phys_to_virt(start);
		map.length = end - start;
		map.type = MT_MEMORY_RW;
		create_mapping(&map);
	}
}

static void __init map_kernel(void)
{
	/*
	 * We use the well known kernel section start and end and split the area in the
	 * middle like this:
	 *  .                .
	 *  | RW memory      |
	 *  +----------------+ kernel_x_start
	 *  | Executable     |
	 *  | kernel memory  |
	 *  +----------------+ kernel_x_end / kernel_nx_start
	 *  | Non-executable |
	 *  | kernel memory  |
	 *  +----------------+ kernel_nx_end
	 *  | RW memory      |
	 *  .                .
	 *
	 * Notice that we are dealing with section sized mappings here so all of this
	 * will be bumped to the closest section boundary. This means that some of the
	 * non-executable part of the kernel memory is actually mapped as executable.
	 * This will only persist until we turn on proper memory management later on
	 * and we remap the whole kernel with page granularity.
	 */
	phys_addr_t kernel_x_start = kernel_sec_start;
	phys_addr_t kernel_x_end = round_up(__pa(__init_end), SECTION_SIZE);
	phys_addr_t kernel_nx_start = kernel_x_end;
	phys_addr_t kernel_nx_end = kernel_sec_end;
	struct map_desc map;

	map.pfn = __phys_to_pfn(kernel_x_start);
	map.virtual = __phys_to_virt(kernel_x_start);
	map.length = kernel_x_end - kernel_x_start;
	map.type = MT_MEMORY_RWX;
	create_mapping(&map);

	/* If the nx part is small it may end up covered by the tail of the RWX section */
	if (kernel_x_end == kernel_nx_end)
		return;

	map.pfn = __phys_to_pfn(kernel_nx_start);
	map.virtual = __phys_to_virt(kernel_nx_start);
	map.length = kernel_nx_end - kernel_nx_start;
	map.type = MT_MEMORY_RW;
	create_mapping(&map);
}

#ifdef CONFIG_ARM_PV_FIXUP
typedef void pgtables_remap(long long offset, unsigned long pgd);
pgtables_remap lpae_pgtables_remap_asm;

/*
 * early_paging_init() recreates boot time page table setup, allowing machines
 * to switch over to a high (>4G) address space on LPAE systems
 */
static void __init early_paging_init(const struct machine_desc *mdesc)
{
	pgtables_remap *lpae_pgtables_remap;
	unsigned long pa_pgd;
	unsigned int cr, ttbcr;
	long long offset;

	if (!mdesc->pv_fixup)
		return;

	offset = mdesc->pv_fixup();
	if (offset == 0)
		return;

	/*
	 * Offset the kernel section physical offsets so that the kernel
	 * mapping will work out later on.
	 */
	kernel_sec_start += offset;
	kernel_sec_end += offset;

	/*
	 * Get the address of the remap function in the 1:1 identity
	 * mapping setup by the early page table assembly code.  We
	 * must get this prior to the pv update.  The following barrier
	 * ensures that this is complete before we fixup any P:V offsets.
	 */
	lpae_pgtables_remap = (pgtables_remap *)(unsigned long)__pa(lpae_pgtables_remap_asm);
	pa_pgd = __pa(swapper_pg_dir);
	barrier();

	pr_info("Switching physical address space to 0x%08llx\n",
		(u64)PHYS_OFFSET + offset);

	/* Re-set the phys pfn offset, and the pv offset */
	__pv_offset += offset;
	__pv_phys_pfn_offset += PFN_DOWN(offset);

	/* Run the patch stub to update the constants */
	fixup_pv_table(&__pv_table_begin,
		(&__pv_table_end - &__pv_table_begin) << 2);

	/*
	 * We changing not only the virtual to physical mapping, but also
	 * the physical addresses used to access memory.  We need to flush
	 * all levels of cache in the system with caching disabled to
	 * ensure that all data is written back, and nothing is prefetched
	 * into the caches.  We also need to prevent the TLB walkers
	 * allocating into the caches too.  Note that this is ARMv7 LPAE
	 * specific.
	 */
	cr = get_cr();
	set_cr(cr & ~(CR_I | CR_C));
	asm("mrc p15, 0, %0, c2, c0, 2" : "=r" (ttbcr));
	asm volatile("mcr p15, 0, %0, c2, c0, 2"
		: : "r" (ttbcr & ~(3 << 8 | 3 << 10)));
	flush_cache_all();

	/*
	 * Fixup the page tables - this must be in the idmap region as
	 * we need to disable the MMU to do this safely, and hence it
	 * needs to be assembly.  It's fairly simple, as we're using the
	 * temporary tables setup by the initial assembly code.
	 */
	lpae_pgtables_remap(offset, pa_pgd);

	/* Re-enable the caches and cacheable TLB walks */
	asm volatile("mcr p15, 0, %0, c2, c0, 2" : : "r" (ttbcr));
	set_cr(cr);
}

#else

static void __init early_paging_init(const struct machine_desc *mdesc)
{
	long long offset;

	if (!mdesc->pv_fixup)
		return;

	offset = mdesc->pv_fixup();
	if (offset == 0)
		return;

	pr_crit("Physical address space modification is only to support Keystone2.\n");
	pr_crit("Please enable ARM_LPAE and ARM_PATCH_PHYS_VIRT support to use this\n");
	pr_crit("feature. Your kernel may crash now, have a good day.\n");
	add_taint(TAINT_CPU_OUT_OF_SPEC, LOCKDEP_STILL_OK);
}

#endif

static void __init early_fixmap_shutdown(void)
{
	int i;
	unsigned long va = fix_to_virt(__end_of_permanent_fixed_addresses - 1);

	pte_offset_fixmap = pte_offset_late_fixmap;
	pmd_clear(fixmap_pmd(va));
	local_flush_tlb_kernel_page(va);

	for (i = 0; i < __end_of_permanent_fixed_addresses; i++) {
		pte_t *pte;
		struct map_desc map;

		map.virtual = fix_to_virt(i);
		pte = pte_offset_early_fixmap(pmd_off_k(map.virtual), map.virtual);

		/* Only i/o device mappings are supported ATM */
		if (pte_none(*pte) ||
		    (pte_val(*pte) & L_PTE_MT_MASK) != L_PTE_MT_DEV_SHARED)
			continue;

		map.pfn = pte_pfn(*pte);
		map.type = MT_DEVICE;
		map.length = PAGE_SIZE;

		create_mapping(&map);
	}
}

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
/*
 * paging_init() sets up the page tables, initialises the zone memory
 * maps, and sets up the zero page, bad page and bad page tables.
 */
<<<<<<< HEAD
void __init paging_init(struct machine_desc *mdesc)
{
	void *zero_page;

	memblock_set_current_limit(arm_lowmem_limit);

	build_mem_type_table();
	prepare_page_table();
	map_lowmem();
	dma_contiguous_remap();
	remap_pages();
	devicemaps_init(mdesc);
	kmap_init();
=======
void __init paging_init(const struct machine_desc *mdesc)
{
	void *zero_page;

	pr_debug("physical kernel sections: 0x%08llx-0x%08llx\n",
		 kernel_sec_start, kernel_sec_end);

	prepare_page_table();
	map_lowmem();
	memblock_set_current_limit(arm_lowmem_limit);
	pr_debug("lowmem limit is %08llx\n", (long long)arm_lowmem_limit);
	/*
	 * After this point early_alloc(), i.e. the memblock allocator, can
	 * be used
	 */
	map_kernel();
	dma_contiguous_remap();
	early_fixmap_shutdown();
	devicemaps_init(mdesc);
	kmap_init();
	tcm_init();
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	top_pmd = pmd_off_k(0xffff0000);

	/* allocate the zero page. */
	zero_page = early_alloc(PAGE_SIZE);

	bootmem_init();

	empty_zero_page = virt_to_page(zero_page);
<<<<<<< HEAD
	__flush_dcache_page(NULL, empty_zero_page);
=======
	__flush_dcache_folio(NULL, page_folio(empty_zero_page));
}

void __init early_mm_init(const struct machine_desc *mdesc)
{
	build_mem_type_table();
	early_paging_init(mdesc);
}

void set_ptes(struct mm_struct *mm, unsigned long addr,
			      pte_t *ptep, pte_t pteval, unsigned int nr)
{
	unsigned long ext = 0;

	if (addr < TASK_SIZE && pte_valid_user(pteval)) {
		if (!pte_special(pteval))
			__sync_icache_dcache(pteval);
		ext |= PTE_EXT_NG;
	}

	for (;;) {
		set_pte_ext(ptep, pteval, ext);
		if (--nr == 0)
			break;
		ptep++;
		pteval = pte_next_pfn(pteval);
	}
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}
