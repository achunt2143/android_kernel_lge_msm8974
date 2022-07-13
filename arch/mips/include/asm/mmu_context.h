/*
 * Switch a MMU context.
 *
 * This file is subject to the terms and conditions of the GNU General Public
 * License.  See the file "COPYING" in the main directory of this archive
 * for more details.
 *
 * Copyright (C) 1996, 1997, 1998, 1999 by Ralf Baechle
 * Copyright (C) 1999 Silicon Graphics, Inc.
 */
#ifndef _ASM_MMU_CONTEXT_H
#define _ASM_MMU_CONTEXT_H

#include <linux/errno.h>
#include <linux/sched.h>
<<<<<<< HEAD
#include <linux/smp.h>
#include <linux/slab.h>
#include <asm/cacheflush.h>
#include <asm/hazards.h>
#include <asm/tlbflush.h>
#ifdef CONFIG_MIPS_MT_SMTC
#include <asm/mipsmtregs.h>
#include <asm/smtc.h>
#endif /* SMTC */
#include <asm-generic/mm_hooks.h>

#ifdef CONFIG_MIPS_PGD_C0_CONTEXT

#define TLBMISS_HANDLER_SETUP_PGD(pgd)				\
	tlbmiss_handler_setup_pgd((unsigned long)(pgd))

extern void tlbmiss_handler_setup_pgd(unsigned long pgd);
=======
#include <linux/mm_types.h>
#include <linux/smp.h>
#include <linux/slab.h>

#include <asm/barrier.h>
#include <asm/cacheflush.h>
#include <asm/dsemul.h>
#include <asm/ginvt.h>
#include <asm/hazards.h>
#include <asm/tlbflush.h>
#include <asm-generic/mm_hooks.h>

#define htw_set_pwbase(pgd)						\
do {									\
	if (cpu_has_htw) {						\
		write_c0_pwbase(pgd);					\
		back_to_back_c0_hazard();				\
	}								\
} while (0)

extern void tlbmiss_handler_setup_pgd(unsigned long);
extern char tlbmiss_handler_setup_pgd_end[];

/* Note: This is also implemented with uasm in arch/mips/kvm/entry.c */
#define TLBMISS_HANDLER_SETUP_PGD(pgd)					\
do {									\
	tlbmiss_handler_setup_pgd((unsigned long)(pgd));		\
	htw_set_pwbase((unsigned long)pgd);				\
} while (0)

#ifdef CONFIG_MIPS_PGD_C0_CONTEXT

#define TLBMISS_HANDLER_RESTORE()					\
	write_c0_xcontext((unsigned long) smp_processor_id() <<		\
			  SMP_CPUID_REGSHIFT)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

#define TLBMISS_HANDLER_SETUP()						\
	do {								\
		TLBMISS_HANDLER_SETUP_PGD(swapper_pg_dir);		\
<<<<<<< HEAD
		write_c0_xcontext((unsigned long) smp_processor_id() << 51); \
	} while (0)

#else /* CONFIG_MIPS_PGD_C0_CONTEXT: using  pgd_current*/
=======
		TLBMISS_HANDLER_RESTORE();				\
	} while (0)

#else /* !CONFIG_MIPS_PGD_C0_CONTEXT: using  pgd_current*/
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

/*
 * For the fast tlb miss handlers, we keep a per cpu array of pointers
 * to the current pgd for each processor. Also, the proc. id is stuffed
 * into the context register.
 */
extern unsigned long pgd_current[];

<<<<<<< HEAD
#define TLBMISS_HANDLER_SETUP_PGD(pgd) \
	pgd_current[smp_processor_id()] = (unsigned long)(pgd)

#ifdef CONFIG_32BIT
#define TLBMISS_HANDLER_SETUP()						\
	write_c0_context((unsigned long) smp_processor_id() << 25);	\
	back_to_back_c0_hazard();					\
	TLBMISS_HANDLER_SETUP_PGD(swapper_pg_dir)
#endif
#ifdef CONFIG_64BIT
#define TLBMISS_HANDLER_SETUP()						\
	write_c0_context((unsigned long) smp_processor_id() << 26);	\
	back_to_back_c0_hazard();					\
	TLBMISS_HANDLER_SETUP_PGD(swapper_pg_dir)
#endif
#endif /* CONFIG_MIPS_PGD_C0_CONTEXT*/
#if defined(CONFIG_CPU_R3000) || defined(CONFIG_CPU_TX39XX)

#define ASID_INC	0x40
#define ASID_MASK	0xfc0

#elif defined(CONFIG_CPU_R8000)

#define ASID_INC	0x10
#define ASID_MASK	0xff0

#elif defined(CONFIG_CPU_RM9000)

#define ASID_INC	0x1
#define ASID_MASK	0xfff

/* SMTC/34K debug hack - but maybe we'll keep it */
#elif defined(CONFIG_MIPS_MT_SMTC)

#define ASID_INC	0x1
extern unsigned long smtc_asid_mask;
#define ASID_MASK	(smtc_asid_mask)
#define	HW_ASID_MASK	0xff
/* End SMTC/34K debug hack */
#else /* FIXME: not correct for R6000 */

#define ASID_INC	0x1
#define ASID_MASK	0xff

#endif

#define cpu_context(cpu, mm)	((mm)->context.asid[cpu])
#define cpu_asid(cpu, mm)	(cpu_context((cpu), (mm)) & ASID_MASK)
#define asid_cache(cpu)		(cpu_data[cpu].asid_cache)

static inline void enter_lazy_tlb(struct mm_struct *mm, struct task_struct *tsk)
{
}
=======
#define TLBMISS_HANDLER_RESTORE()					\
	write_c0_context((unsigned long) smp_processor_id() <<		\
			 SMP_CPUID_REGSHIFT)

#define TLBMISS_HANDLER_SETUP()						\
	TLBMISS_HANDLER_RESTORE();					\
	back_to_back_c0_hazard();					\
	TLBMISS_HANDLER_SETUP_PGD(swapper_pg_dir)
#endif /* CONFIG_MIPS_PGD_C0_CONTEXT*/

/*
 * The ginvt instruction will invalidate wired entries when its type field
 * targets anything other than the entire TLB. That means that if we were to
 * allow the kernel to create wired entries with the MMID of current->active_mm
 * then those wired entries could be invalidated when we later use ginvt to
 * invalidate TLB entries with that MMID.
 *
 * In order to prevent ginvt from trashing wired entries, we reserve one MMID
 * for use by the kernel when creating wired entries. This MMID will never be
 * assigned to a struct mm, and we'll never target it with a ginvt instruction.
 */
#define MMID_KERNEL_WIRED	0
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

/*
 *  All unused by hardware upper bits will be considered
 *  as a software asid extension.
 */
<<<<<<< HEAD
#define ASID_VERSION_MASK  ((unsigned long)~(ASID_MASK|(ASID_MASK-1)))
#define ASID_FIRST_VERSION ((unsigned long)(~ASID_VERSION_MASK) + 1)

#ifndef CONFIG_MIPS_MT_SMTC
/* Normal, classic MIPS get_new_mmu_context */
static inline void
get_new_mmu_context(struct mm_struct *mm, unsigned long cpu)
{
	unsigned long asid = asid_cache(cpu);

	if (! ((asid += ASID_INC) & ASID_MASK) ) {
		if (cpu_has_vtag_icache)
			flush_icache_all();
		local_flush_tlb_all();	/* start new asid cycle */
		if (!asid)		/* fix version if needed */
			asid = ASID_FIRST_VERSION;
	}
	cpu_context(cpu, mm) = asid_cache(cpu) = asid;
}

#else /* CONFIG_MIPS_MT_SMTC */

#define get_new_mmu_context(mm, cpu) smtc_get_new_mmu_context((mm), (cpu))

#endif /* CONFIG_MIPS_MT_SMTC */
=======
static inline u64 asid_version_mask(unsigned int cpu)
{
	unsigned long asid_mask = cpu_asid_mask(&cpu_data[cpu]);

	return ~(u64)(asid_mask | (asid_mask - 1));
}

static inline u64 asid_first_version(unsigned int cpu)
{
	return ~asid_version_mask(cpu) + 1;
}

static inline u64 cpu_context(unsigned int cpu, const struct mm_struct *mm)
{
	if (cpu_has_mmid)
		return atomic64_read(&mm->context.mmid);

	return mm->context.asid[cpu];
}

static inline void set_cpu_context(unsigned int cpu,
				   struct mm_struct *mm, u64 ctx)
{
	if (cpu_has_mmid)
		atomic64_set(&mm->context.mmid, ctx);
	else
		mm->context.asid[cpu] = ctx;
}

#define asid_cache(cpu)		(cpu_data[cpu].asid_cache)
#define cpu_asid(cpu, mm) \
	(cpu_context((cpu), (mm)) & cpu_asid_mask(&cpu_data[cpu]))

extern void get_new_mmu_context(struct mm_struct *mm);
extern void check_mmu_context(struct mm_struct *mm);
extern void check_switch_mmu_context(struct mm_struct *mm);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

/*
 * Initialize the context related info for a new mm_struct
 * instance.
 */
<<<<<<< HEAD
=======
#define init_new_context init_new_context
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
static inline int
init_new_context(struct task_struct *tsk, struct mm_struct *mm)
{
	int i;

<<<<<<< HEAD
	for_each_online_cpu(i)
		cpu_context(i, mm) = 0;
=======
	if (cpu_has_mmid) {
		set_cpu_context(0, mm, 0);
	} else {
		for_each_possible_cpu(i)
			set_cpu_context(i, mm, 0);
	}

	mm->context.bd_emupage_allocmap = NULL;
	spin_lock_init(&mm->context.bd_emupage_lock);
	init_waitqueue_head(&mm->context.bd_emupage_queue);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	return 0;
}

static inline void switch_mm(struct mm_struct *prev, struct mm_struct *next,
<<<<<<< HEAD
                             struct task_struct *tsk)
{
	unsigned int cpu = smp_processor_id();
	unsigned long flags;
#ifdef CONFIG_MIPS_MT_SMTC
	unsigned long oldasid;
	unsigned long mtflags;
	int mytlb = (smtc_status & SMTC_TLB_SHARED) ? 0 : cpu_data[cpu].vpe_id;
	local_irq_save(flags);
	mtflags = dvpe();
#else /* Not SMTC */
	local_irq_save(flags);
#endif /* CONFIG_MIPS_MT_SMTC */

	/* Check if our ASID is of an older version and thus invalid */
	if ((cpu_context(cpu, next) ^ asid_cache(cpu)) & ASID_VERSION_MASK)
		get_new_mmu_context(next, cpu);
#ifdef CONFIG_MIPS_MT_SMTC
	/*
	 * If the EntryHi ASID being replaced happens to be
	 * the value flagged at ASID recycling time as having
	 * an extended life, clear the bit showing it being
	 * in use by this "CPU", and if that's the last bit,
	 * free up the ASID value for use and flush any old
	 * instances of it from the TLB.
	 */
	oldasid = (read_c0_entryhi() & ASID_MASK);
	if(smtc_live_asid[mytlb][oldasid]) {
		smtc_live_asid[mytlb][oldasid] &= ~(0x1 << cpu);
		if(smtc_live_asid[mytlb][oldasid] == 0)
			smtc_flush_tlb_asid(oldasid);
	}
	/*
	 * Tread softly on EntryHi, and so long as we support
	 * having ASID_MASK smaller than the hardware maximum,
	 * make sure no "soft" bits become "hard"...
	 */
	write_c0_entryhi((read_c0_entryhi() & ~HW_ASID_MASK) |
			 cpu_asid(cpu, next));
	ehb(); /* Make sure it propagates to TCStatus */
	evpe(mtflags);
#else
	write_c0_entryhi(cpu_asid(cpu, next));
#endif /* CONFIG_MIPS_MT_SMTC */
	TLBMISS_HANDLER_SETUP_PGD(next->pgd);
=======
			     struct task_struct *tsk)
{
	unsigned int cpu = smp_processor_id();
	unsigned long flags;
	local_irq_save(flags);

	htw_stop();
	check_switch_mmu_context(next);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	/*
	 * Mark current->active_mm as not "active" anymore.
	 * We don't want to mislead possible IPI tlb flush routines.
	 */
	cpumask_clear_cpu(cpu, mm_cpumask(prev));
	cpumask_set_cpu(cpu, mm_cpumask(next));
<<<<<<< HEAD
=======
	htw_start();
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	local_irq_restore(flags);
}

/*
 * Destroy context related info for an mm_struct that is about
 * to be put to rest.
 */
<<<<<<< HEAD
static inline void destroy_context(struct mm_struct *mm)
{
}

#define deactivate_mm(tsk, mm)	do { } while (0)

/*
 * After we have set current->mm to a new value, this activates
 * the context for the new mm so we see the new mappings.
 */
static inline void
activate_mm(struct mm_struct *prev, struct mm_struct *next)
{
	unsigned long flags;
	unsigned int cpu = smp_processor_id();

#ifdef CONFIG_MIPS_MT_SMTC
	unsigned long oldasid;
	unsigned long mtflags;
	int mytlb = (smtc_status & SMTC_TLB_SHARED) ? 0 : cpu_data[cpu].vpe_id;
#endif /* CONFIG_MIPS_MT_SMTC */

	local_irq_save(flags);

	/* Unconditionally get a new ASID.  */
	get_new_mmu_context(next, cpu);

#ifdef CONFIG_MIPS_MT_SMTC
	/* See comments for similar code above */
	mtflags = dvpe();
	oldasid = read_c0_entryhi() & ASID_MASK;
	if(smtc_live_asid[mytlb][oldasid]) {
		smtc_live_asid[mytlb][oldasid] &= ~(0x1 << cpu);
		if(smtc_live_asid[mytlb][oldasid] == 0)
			 smtc_flush_tlb_asid(oldasid);
	}
	/* See comments for similar code above */
	write_c0_entryhi((read_c0_entryhi() & ~HW_ASID_MASK) |
	                 cpu_asid(cpu, next));
	ehb(); /* Make sure it propagates to TCStatus */
	evpe(mtflags);
#else
	write_c0_entryhi(cpu_asid(cpu, next));
#endif /* CONFIG_MIPS_MT_SMTC */
	TLBMISS_HANDLER_SETUP_PGD(next->pgd);

	/* mark mmu ownership change */
	cpumask_clear_cpu(cpu, mm_cpumask(prev));
	cpumask_set_cpu(cpu, mm_cpumask(next));

	local_irq_restore(flags);
}

/*
 * If mm is currently active_mm, we can't really drop it.  Instead,
 * we will get a new one for it.
 */
static inline void
drop_mmu_context(struct mm_struct *mm, unsigned cpu)
{
	unsigned long flags;
#ifdef CONFIG_MIPS_MT_SMTC
	unsigned long oldasid;
	/* Can't use spinlock because called from TLB flush within DVPE */
	unsigned int prevvpe;
	int mytlb = (smtc_status & SMTC_TLB_SHARED) ? 0 : cpu_data[cpu].vpe_id;
#endif /* CONFIG_MIPS_MT_SMTC */

	local_irq_save(flags);

	if (cpumask_test_cpu(cpu, mm_cpumask(mm)))  {
		get_new_mmu_context(mm, cpu);
#ifdef CONFIG_MIPS_MT_SMTC
		/* See comments for similar code above */
		prevvpe = dvpe();
		oldasid = (read_c0_entryhi() & ASID_MASK);
		if (smtc_live_asid[mytlb][oldasid]) {
			smtc_live_asid[mytlb][oldasid] &= ~(0x1 << cpu);
			if(smtc_live_asid[mytlb][oldasid] == 0)
				smtc_flush_tlb_asid(oldasid);
		}
		/* See comments for similar code above */
		write_c0_entryhi((read_c0_entryhi() & ~HW_ASID_MASK)
				| cpu_asid(cpu, mm));
		ehb(); /* Make sure it propagates to TCStatus */
		evpe(prevvpe);
#else /* not CONFIG_MIPS_MT_SMTC */
		write_c0_entryhi(cpu_asid(cpu, mm));
#endif /* CONFIG_MIPS_MT_SMTC */
	} else {
		/* will get a new context next time */
#ifndef CONFIG_MIPS_MT_SMTC
		cpu_context(cpu, mm) = 0;
#else /* SMTC */
		int i;

		/* SMTC shares the TLB (and ASIDs) across VPEs */
		for_each_online_cpu(i) {
		    if((smtc_status & SMTC_TLB_SHARED)
		    || (cpu_data[i].vpe_id == cpu_data[cpu].vpe_id))
			cpu_context(i, mm) = 0;
		}
#endif /* CONFIG_MIPS_MT_SMTC */
	}
	local_irq_restore(flags);
}

=======
#define destroy_context destroy_context
static inline void destroy_context(struct mm_struct *mm)
{
	dsemul_mm_cleanup(mm);
}

static inline void
drop_mmu_context(struct mm_struct *mm)
{
	unsigned long flags;
	unsigned int cpu;
	u32 old_mmid;
	u64 ctx;

	local_irq_save(flags);

	cpu = smp_processor_id();
	ctx = cpu_context(cpu, mm);

	if (!ctx) {
		/* no-op */
	} else if (cpu_has_mmid) {
		/*
		 * Globally invalidating TLB entries associated with the MMID
		 * is pretty cheap using the GINVT instruction, so we'll do
		 * that rather than incur the overhead of allocating a new
		 * MMID. The latter would be especially difficult since MMIDs
		 * are global & other CPUs may be actively using ctx.
		 */
		htw_stop();
		old_mmid = read_c0_memorymapid();
		write_c0_memorymapid(ctx & cpu_asid_mask(&cpu_data[cpu]));
		mtc0_tlbw_hazard();
		ginvt_mmid();
		sync_ginv();
		write_c0_memorymapid(old_mmid);
		instruction_hazard();
		htw_start();
	} else if (cpumask_test_cpu(cpu, mm_cpumask(mm))) {
		/*
		 * mm is currently active, so we can't really drop it.
		 * Instead we bump the ASID.
		 */
		htw_stop();
		get_new_mmu_context(mm);
		write_c0_entryhi(cpu_asid(cpu, mm));
		htw_start();
	} else {
		/* will get a new context next time */
		set_cpu_context(cpu, mm, 0);
	}

	local_irq_restore(flags);
}

#include <asm-generic/mmu_context.h>

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#endif /* _ASM_MMU_CONTEXT_H */
