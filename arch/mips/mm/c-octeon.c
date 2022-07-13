/*
 * This file is subject to the terms and conditions of the GNU General Public
 * License.  See the file "COPYING" in the main directory of this archive
 * for more details.
 *
 * Copyright (C) 2005-2007 Cavium Networks
 */
<<<<<<< HEAD
#include <linux/init.h>
=======
#include <linux/export.h>
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#include <linux/kernel.h>
#include <linux/sched.h>
#include <linux/smp.h>
#include <linux/mm.h>
#include <linux/bitops.h>
#include <linux/cpu.h>
#include <linux/io.h>

#include <asm/bcache.h>
#include <asm/bootinfo.h>
#include <asm/cacheops.h>
#include <asm/cpu-features.h>
<<<<<<< HEAD
#include <asm/page.h>
#include <asm/pgtable.h>
#include <asm/r4kcache.h>
#include <asm/mmu_context.h>
#include <asm/war.h>
=======
#include <asm/cpu-type.h>
#include <asm/page.h>
#include <asm/r4kcache.h>
#include <asm/traps.h>
#include <asm/mmu_context.h>
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

#include <asm/octeon/octeon.h>

unsigned long long cache_err_dcache[NR_CPUS];
<<<<<<< HEAD

/**
=======
EXPORT_SYMBOL_GPL(cache_err_dcache);

/*
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 * Octeon automatically flushes the dcache on tlb changes, so
 * from Linux's viewpoint it acts much like a physically
 * tagged cache. No flushing is needed
 *
 */
static void octeon_flush_data_cache_page(unsigned long addr)
{
    /* Nothing to do */
}

static inline void octeon_local_flush_icache(void)
{
	asm volatile ("synci 0($0)");
}

/*
 * Flush local I-cache for the specified range.
 */
static void local_octeon_flush_icache_range(unsigned long start,
					    unsigned long end)
{
	octeon_local_flush_icache();
}

/**
<<<<<<< HEAD
 * Flush caches as necessary for all cores affected by a
 * vma. If no vma is supplied, all cores are flushed.
=======
 * octeon_flush_icache_all_cores -  Flush caches as necessary for all cores
 * affected by a vma. If no vma is supplied, all cores are flushed.
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 *
 * @vma:    VMA to flush or NULL to flush all icaches.
 */
static void octeon_flush_icache_all_cores(struct vm_area_struct *vma)
{
	extern void octeon_send_ipi_single(int cpu, unsigned int action);
#ifdef CONFIG_SMP
	int cpu;
	cpumask_t mask;
#endif

	mb();
	octeon_local_flush_icache();
#ifdef CONFIG_SMP
	preempt_disable();
	cpu = smp_processor_id();

	/*
	 * If we have a vma structure, we only need to worry about
	 * cores it has been used on
	 */
	if (vma)
		mask = *mm_cpumask(vma->vm_mm);
	else
		mask = *cpu_online_mask;
	cpumask_clear_cpu(cpu, &mask);
<<<<<<< HEAD
	for_each_cpu(cpu, &mask)
		octeon_send_ipi_single(cpu, SMP_ICACHE_FLUSH);
=======
#ifdef CONFIG_CAVIUM_OCTEON_SOC
	for_each_cpu(cpu, &mask)
		octeon_send_ipi_single(cpu, SMP_ICACHE_FLUSH);
#else
	smp_call_function_many(&mask, (smp_call_func_t)octeon_local_flush_icache,
			       NULL, 1);
#endif
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	preempt_enable();
#endif
}


<<<<<<< HEAD
/**
=======
/*
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 * Called to flush the icache on all cores
 */
static void octeon_flush_icache_all(void)
{
	octeon_flush_icache_all_cores(NULL);
}


/**
<<<<<<< HEAD
 * Called to flush all memory associated with a memory
 * context.
 *
 * @mm:     Memory context to flush
=======
 * octeon_flush_cache_mm - flush all memory associated with a memory context.
 *
 * @mm:	    Memory context to flush
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 */
static void octeon_flush_cache_mm(struct mm_struct *mm)
{
	/*
	 * According to the R4K version of this file, CPUs without
	 * dcache aliases don't need to do anything here
	 */
}


<<<<<<< HEAD
/**
=======
/*
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 * Flush a range of kernel addresses out of the icache
 *
 */
static void octeon_flush_icache_range(unsigned long start, unsigned long end)
{
	octeon_flush_icache_all_cores(NULL);
}


/**
<<<<<<< HEAD
 * Flush the icache for a trampoline. These are used for interrupt
 * and exception hooking.
 *
 * @addr:   Address to flush
 */
static void octeon_flush_cache_sigtramp(unsigned long addr)
{
	struct vm_area_struct *vma;

	vma = find_vma(current->mm, addr);
	octeon_flush_icache_all_cores(vma);
}


/**
 * Flush a range out of a vma
 *
 * @vma:    VMA to flush
 * @start:
 * @end:
=======
 * octeon_flush_cache_range - Flush a range out of a vma
 *
 * @vma:    VMA to flush
 * @start:  beginning address for flush
 * @end:    ending address for flush
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 */
static void octeon_flush_cache_range(struct vm_area_struct *vma,
				     unsigned long start, unsigned long end)
{
	if (vma->vm_flags & VM_EXEC)
		octeon_flush_icache_all_cores(vma);
}


/**
<<<<<<< HEAD
 * Flush a specific page of a vma
 *
 * @vma:    VMA to flush page for
 * @page:   Page to flush
 * @pfn:
=======
 * octeon_flush_cache_page - Flush a specific page of a vma
 *
 * @vma:    VMA to flush page for
 * @page:   Page to flush
 * @pfn:    Page frame number
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 */
static void octeon_flush_cache_page(struct vm_area_struct *vma,
				    unsigned long page, unsigned long pfn)
{
	if (vma->vm_flags & VM_EXEC)
		octeon_flush_icache_all_cores(vma);
}

static void octeon_flush_kernel_vmap_range(unsigned long vaddr, int size)
{
	BUG();
}

<<<<<<< HEAD
/**
 * Probe Octeon's caches
 *
 */
static void __cpuinit probe_octeon(void)
=======
/*
 * Probe Octeon's caches
 *
 */
static void probe_octeon(void)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
{
	unsigned long icache_size;
	unsigned long dcache_size;
	unsigned int config1;
	struct cpuinfo_mips *c = &current_cpu_data;
<<<<<<< HEAD

	config1 = read_c0_config1();
	switch (c->cputype) {
=======
	int cputype = current_cpu_type();

	config1 = read_c0_config1();
	switch (cputype) {
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	case CPU_CAVIUM_OCTEON:
	case CPU_CAVIUM_OCTEON_PLUS:
		c->icache.linesz = 2 << ((config1 >> 19) & 7);
		c->icache.sets = 64 << ((config1 >> 22) & 7);
		c->icache.ways = 1 + ((config1 >> 16) & 7);
		c->icache.flags |= MIPS_CACHE_VTAG;
		icache_size =
			c->icache.sets * c->icache.ways * c->icache.linesz;
		c->icache.waybit = ffs(icache_size / c->icache.ways) - 1;
		c->dcache.linesz = 128;
<<<<<<< HEAD
		if (c->cputype == CPU_CAVIUM_OCTEON_PLUS)
=======
		if (cputype == CPU_CAVIUM_OCTEON_PLUS)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
			c->dcache.sets = 2; /* CN5XXX has two Dcache sets */
		else
			c->dcache.sets = 1; /* CN3XXX has one Dcache set */
		c->dcache.ways = 64;
		dcache_size =
			c->dcache.sets * c->dcache.ways * c->dcache.linesz;
		c->dcache.waybit = ffs(dcache_size / c->dcache.ways) - 1;
		c->options |= MIPS_CPU_PREFETCH;
		break;

	case CPU_CAVIUM_OCTEON2:
		c->icache.linesz = 2 << ((config1 >> 19) & 7);
		c->icache.sets = 8;
		c->icache.ways = 37;
		c->icache.flags |= MIPS_CACHE_VTAG;
		icache_size = c->icache.sets * c->icache.ways * c->icache.linesz;

		c->dcache.linesz = 128;
		c->dcache.ways = 32;
		c->dcache.sets = 8;
		dcache_size = c->dcache.sets * c->dcache.ways * c->dcache.linesz;
		c->options |= MIPS_CPU_PREFETCH;
		break;

<<<<<<< HEAD
=======
	case CPU_CAVIUM_OCTEON3:
		c->icache.linesz = 128;
		c->icache.sets = 16;
		c->icache.ways = 39;
		c->icache.flags |= MIPS_CACHE_VTAG;
		icache_size = c->icache.sets * c->icache.ways * c->icache.linesz;

		c->dcache.linesz = 128;
		c->dcache.ways = 32;
		c->dcache.sets = 8;
		dcache_size = c->dcache.sets * c->dcache.ways * c->dcache.linesz;
		c->options |= MIPS_CPU_PREFETCH;
		break;

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	default:
		panic("Unsupported Cavium Networks CPU type");
		break;
	}

	/* compute a couple of other cache variables */
	c->icache.waysize = icache_size / c->icache.ways;
	c->dcache.waysize = dcache_size / c->dcache.ways;

	c->icache.sets = icache_size / (c->icache.linesz * c->icache.ways);
	c->dcache.sets = dcache_size / (c->dcache.linesz * c->dcache.ways);

	if (smp_processor_id() == 0) {
<<<<<<< HEAD
		pr_notice("Primary instruction cache %ldkB, %s, %d way, "
			  "%d sets, linesize %d bytes.\n",
			  icache_size >> 10,
			  cpu_has_vtag_icache ?
				"virtually tagged" : "physically tagged",
			  c->icache.ways, c->icache.sets, c->icache.linesz);

		pr_notice("Primary data cache %ldkB, %d-way, %d sets, "
			  "linesize %d bytes.\n",
			  dcache_size >> 10, c->dcache.ways,
			  c->dcache.sets, c->dcache.linesz);
	}
}


/**
 * Setup the Octeon cache flush routines
 *
 */
void __cpuinit octeon_cache_init(void)
{
	extern unsigned long ebase;
	extern char except_vec2_octeon;

	memcpy((void *)(ebase + 0x100), &except_vec2_octeon, 0x80);
	octeon_flush_cache_sigtramp(ebase + 0x100);

=======
		pr_info("Primary instruction cache %ldkB, %s, %d way, "
			"%d sets, linesize %d bytes.\n",
			icache_size >> 10,
			cpu_has_vtag_icache ?
				"virtually tagged" : "physically tagged",
			c->icache.ways, c->icache.sets, c->icache.linesz);

		pr_info("Primary data cache %ldkB, %d-way, %d sets, "
			"linesize %d bytes.\n",
			dcache_size >> 10, c->dcache.ways,
			c->dcache.sets, c->dcache.linesz);
	}
}

static void  octeon_cache_error_setup(void)
{
	extern char except_vec2_octeon;
	set_handler(0x100, &except_vec2_octeon, 0x80);
}

/*
 * Setup the Octeon cache flush routines
 *
 */
void octeon_cache_init(void)
{
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	probe_octeon();

	shm_align_mask = PAGE_SIZE - 1;

	flush_cache_all			= octeon_flush_icache_all;
	__flush_cache_all		= octeon_flush_icache_all;
	flush_cache_mm			= octeon_flush_cache_mm;
	flush_cache_page		= octeon_flush_cache_page;
	flush_cache_range		= octeon_flush_cache_range;
<<<<<<< HEAD
	flush_cache_sigtramp		= octeon_flush_cache_sigtramp;
=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	flush_icache_all		= octeon_flush_icache_all;
	flush_data_cache_page		= octeon_flush_data_cache_page;
	flush_icache_range		= octeon_flush_icache_range;
	local_flush_icache_range	= local_octeon_flush_icache_range;
<<<<<<< HEAD
=======
	__flush_icache_user_range	= octeon_flush_icache_range;
	__local_flush_icache_user_range	= local_octeon_flush_icache_range;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	__flush_kernel_vmap_range	= octeon_flush_kernel_vmap_range;

	build_clear_page();
	build_copy_page();
<<<<<<< HEAD
}

/**
 * Handle a cache error exception
 */

static void  cache_parity_error_octeon(int non_recoverable)
{
	unsigned long coreid = cvmx_get_core_num();
	uint64_t icache_err = read_octeon_c0_icacheerr();

	pr_err("Cache error exception:\n");
	pr_err("cp0_errorepc == %lx\n", read_c0_errorepc());
	if (icache_err & 1) {
		pr_err("CacheErr (Icache) == %llx\n",
		       (unsigned long long)icache_err);
		write_octeon_c0_icacheerr(0);
	}
	if (cache_err_dcache[coreid] & 1) {
		pr_err("CacheErr (Dcache) == %llx\n",
		       (unsigned long long)cache_err_dcache[coreid]);
		cache_err_dcache[coreid] = 0;
	}

	if (non_recoverable)
		panic("Can't handle cache error: nested exception");
}

/**
 * Called when the the exception is recoverable
=======

	board_cache_error_setup = octeon_cache_error_setup;
}

/*
 * Handle a cache error exception
 */
static RAW_NOTIFIER_HEAD(co_cache_error_chain);

int register_co_cache_error_notifier(struct notifier_block *nb)
{
	return raw_notifier_chain_register(&co_cache_error_chain, nb);
}
EXPORT_SYMBOL_GPL(register_co_cache_error_notifier);

int unregister_co_cache_error_notifier(struct notifier_block *nb)
{
	return raw_notifier_chain_unregister(&co_cache_error_chain, nb);
}
EXPORT_SYMBOL_GPL(unregister_co_cache_error_notifier);

static void co_cache_error_call_notifiers(unsigned long val)
{
	int rv = raw_notifier_call_chain(&co_cache_error_chain, val, NULL);
	if ((rv & ~NOTIFY_STOP_MASK) != NOTIFY_OK) {
		u64 dcache_err;
		unsigned long coreid = cvmx_get_core_num();
		u64 icache_err = read_octeon_c0_icacheerr();

		if (val) {
			dcache_err = cache_err_dcache[coreid];
			cache_err_dcache[coreid] = 0;
		} else {
			dcache_err = read_octeon_c0_dcacheerr();
		}

		pr_err("Core%lu: Cache error exception:\n", coreid);
		pr_err("cp0_errorepc == %lx\n", read_c0_errorepc());
		if (icache_err & 1) {
			pr_err("CacheErr (Icache) == %llx\n",
			       (unsigned long long)icache_err);
			write_octeon_c0_icacheerr(0);
		}
		if (dcache_err & 1) {
			pr_err("CacheErr (Dcache) == %llx\n",
			       (unsigned long long)dcache_err);
		}
	}
}

/*
 * Called when the exception is recoverable
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 */

asmlinkage void cache_parity_error_octeon_recoverable(void)
{
<<<<<<< HEAD
	cache_parity_error_octeon(0);
}

/**
 * Called when the the exception is not recoverable
=======
	co_cache_error_call_notifiers(0);
}

/*
 * Called when the exception is not recoverable
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 */

asmlinkage void cache_parity_error_octeon_non_recoverable(void)
{
<<<<<<< HEAD
	cache_parity_error_octeon(1);
=======
	co_cache_error_call_notifiers(1);
	panic("Can't handle cache error: nested exception");
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}
