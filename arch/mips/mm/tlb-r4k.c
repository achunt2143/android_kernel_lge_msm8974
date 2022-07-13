/*
 * This file is subject to the terms and conditions of the GNU General Public
 * License.  See the file "COPYING" in the main directory of this archive
 * for more details.
 *
 * Copyright (C) 1996 David S. Miller (davem@davemloft.net)
 * Copyright (C) 1997, 1998, 1999, 2000 Ralf Baechle ralf@gnu.org
 * Carsten Langgaard, carstenl@mips.com
 * Copyright (C) 2002 MIPS Technologies, Inc.  All rights reserved.
 */
<<<<<<< HEAD
=======
#include <linux/cpu_pm.h>
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#include <linux/init.h>
#include <linux/sched.h>
#include <linux/smp.h>
#include <linux/mm.h>
#include <linux/hugetlb.h>
<<<<<<< HEAD

#include <asm/cpu.h>
#include <asm/bootinfo.h>
#include <asm/mmu_context.h>
#include <asm/pgtable.h>
#include <asm/tlbmisc.h>

extern void build_tlb_refill_handler(void);

/*
 * Make sure all entries differ.  If they're not different
 * MIPS32 will take revenge ...
 */
#define UNIQUE_ENTRYHI(idx) (CKSEG0 + ((idx) << (PAGE_SHIFT + 1)))

/* Atomicity and interruptability */
#ifdef CONFIG_MIPS_MT_SMTC

#include <asm/smtc.h>
#include <asm/mipsmtregs.h>

#define ENTER_CRITICAL(flags) \
	{ \
	unsigned int mvpflags; \
	local_irq_save(flags);\
	mvpflags = dvpe()
#define EXIT_CRITICAL(flags) \
	evpe(mvpflags); \
	local_irq_restore(flags); \
	}
#else

#define ENTER_CRITICAL(flags) local_irq_save(flags)
#define EXIT_CRITICAL(flags) local_irq_restore(flags)

#endif /* CONFIG_MIPS_MT_SMTC */

#if defined(CONFIG_CPU_LOONGSON2)
/*
 * LOONGSON2 has a 4 entry itlb which is a subset of dtlb,
 * unfortrunately, itlb is not totally transparent to software.
 */
#define FLUSH_ITLB write_c0_diag(4);

#define FLUSH_ITLB_VM(vma) { if ((vma)->vm_flags & VM_EXEC)  write_c0_diag(4); }

#else

#define FLUSH_ITLB
#define FLUSH_ITLB_VM(vma)

#endif
=======
#include <linux/export.h>

#include <asm/cpu.h>
#include <asm/cpu-type.h>
#include <asm/bootinfo.h>
#include <asm/hazards.h>
#include <asm/mmu_context.h>
#include <asm/tlb.h>
#include <asm/tlbex.h>
#include <asm/tlbmisc.h>
#include <asm/setup.h>

/*
 * LOONGSON-2 has a 4 entry itlb which is a subset of jtlb, LOONGSON-3 has
 * a 4 entry itlb and a 4 entry dtlb which are subsets of jtlb. Unfortunately,
 * itlb/dtlb are not totally transparent to software.
 */
static inline void flush_micro_tlb(void)
{
	switch (current_cpu_type()) {
	case CPU_LOONGSON2EF:
		write_c0_diag(LOONGSON_DIAG_ITLB);
		break;
	case CPU_LOONGSON64:
		write_c0_diag(LOONGSON_DIAG_ITLB | LOONGSON_DIAG_DTLB);
		break;
	default:
		break;
	}
}

static inline void flush_micro_tlb_vm(struct vm_area_struct *vma)
{
	if (vma->vm_flags & VM_EXEC)
		flush_micro_tlb();
}
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

void local_flush_tlb_all(void)
{
	unsigned long flags;
	unsigned long old_ctx;
<<<<<<< HEAD
	int entry;

	ENTER_CRITICAL(flags);
	/* Save old context and create impossible VPN2 value */
	old_ctx = read_c0_entryhi();
	write_c0_entrylo0(0);
	write_c0_entrylo1(0);

	entry = read_c0_wired();

	/* Blast 'em all away. */
	while (entry < current_cpu_data.tlbsize) {
		/* Make sure all entries differ. */
		write_c0_entryhi(UNIQUE_ENTRYHI(entry));
		write_c0_index(entry);
		mtc0_tlbw_hazard();
		tlb_write_indexed();
		entry++;
	}
	tlbw_use_hazard();
	write_c0_entryhi(old_ctx);
	FLUSH_ITLB;
	EXIT_CRITICAL(flags);
}

/* All entries common to a mm share an asid.  To effectively flush
   these entries, we just bump the asid. */
void local_flush_tlb_mm(struct mm_struct *mm)
{
	int cpu;

	preempt_disable();

	cpu = smp_processor_id();

	if (cpu_context(cpu, mm) != 0) {
		drop_mmu_context(mm, cpu);
	}

	preempt_enable();
}
=======
	int entry, ftlbhighset;

	local_irq_save(flags);
	/* Save old context and create impossible VPN2 value */
	old_ctx = read_c0_entryhi();
	htw_stop();
	write_c0_entrylo0(0);
	write_c0_entrylo1(0);

	entry = num_wired_entries();

	/*
	 * Blast 'em all away.
	 * If there are any wired entries, fall back to iterating
	 */
	if (cpu_has_tlbinv && !entry) {
		if (current_cpu_data.tlbsizevtlb) {
			write_c0_index(0);
			mtc0_tlbw_hazard();
			tlbinvf();  /* invalidate VTLB */
		}
		ftlbhighset = current_cpu_data.tlbsizevtlb +
			current_cpu_data.tlbsizeftlbsets;
		for (entry = current_cpu_data.tlbsizevtlb;
		     entry < ftlbhighset;
		     entry++) {
			write_c0_index(entry);
			mtc0_tlbw_hazard();
			tlbinvf();  /* invalidate one FTLB set */
		}
	} else {
		while (entry < current_cpu_data.tlbsize) {
			/* Make sure all entries differ. */
			write_c0_entryhi(UNIQUE_ENTRYHI(entry));
			write_c0_index(entry);
			mtc0_tlbw_hazard();
			tlb_write_indexed();
			entry++;
		}
	}
	tlbw_use_hazard();
	write_c0_entryhi(old_ctx);
	htw_start();
	flush_micro_tlb();
	local_irq_restore(flags);
}
EXPORT_SYMBOL(local_flush_tlb_all);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

void local_flush_tlb_range(struct vm_area_struct *vma, unsigned long start,
	unsigned long end)
{
	struct mm_struct *mm = vma->vm_mm;
	int cpu = smp_processor_id();

	if (cpu_context(cpu, mm) != 0) {
		unsigned long size, flags;
<<<<<<< HEAD
		int huge = is_vm_hugetlb_page(vma);

		ENTER_CRITICAL(flags);
		if (huge) {
			start = round_down(start, HPAGE_SIZE);
			end = round_up(end, HPAGE_SIZE);
			size = (end - start) >> HPAGE_SHIFT;
		} else {
			start = round_down(start, PAGE_SIZE << 1);
			end = round_up(end, PAGE_SIZE << 1);
			size = (end - start) >> (PAGE_SHIFT + 1);
		}
		if (size <= current_cpu_data.tlbsize/2) {
			int oldpid = read_c0_entryhi();
			int newpid = cpu_asid(cpu, mm);

			while (start < end) {
				int idx;

				write_c0_entryhi(start | newpid);
				if (huge)
					start += HPAGE_SIZE;
				else
					start += (PAGE_SIZE << 1);
=======

		local_irq_save(flags);
		start = round_down(start, PAGE_SIZE << 1);
		end = round_up(end, PAGE_SIZE << 1);
		size = (end - start) >> (PAGE_SHIFT + 1);
		if (size <= (current_cpu_data.tlbsizeftlbsets ?
			     current_cpu_data.tlbsize / 8 :
			     current_cpu_data.tlbsize / 2)) {
			unsigned long old_entryhi, old_mmid;
			int newpid = cpu_asid(cpu, mm);

			old_entryhi = read_c0_entryhi();
			if (cpu_has_mmid) {
				old_mmid = read_c0_memorymapid();
				write_c0_memorymapid(newpid);
			}

			htw_stop();
			while (start < end) {
				int idx;

				if (cpu_has_mmid)
					write_c0_entryhi(start);
				else
					write_c0_entryhi(start | newpid);
				start += (PAGE_SIZE << 1);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
				mtc0_tlbw_hazard();
				tlb_probe();
				tlb_probe_hazard();
				idx = read_c0_index();
				write_c0_entrylo0(0);
				write_c0_entrylo1(0);
				if (idx < 0)
					continue;
				/* Make sure all entries differ. */
				write_c0_entryhi(UNIQUE_ENTRYHI(idx));
				mtc0_tlbw_hazard();
				tlb_write_indexed();
			}
			tlbw_use_hazard();
<<<<<<< HEAD
			write_c0_entryhi(oldpid);
		} else {
			drop_mmu_context(mm, cpu);
		}
		FLUSH_ITLB;
		EXIT_CRITICAL(flags);
=======
			write_c0_entryhi(old_entryhi);
			if (cpu_has_mmid)
				write_c0_memorymapid(old_mmid);
			htw_start();
		} else {
			drop_mmu_context(mm);
		}
		flush_micro_tlb();
		local_irq_restore(flags);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	}
}

void local_flush_tlb_kernel_range(unsigned long start, unsigned long end)
{
	unsigned long size, flags;

<<<<<<< HEAD
	ENTER_CRITICAL(flags);
	size = (end - start + (PAGE_SIZE - 1)) >> PAGE_SHIFT;
	size = (size + 1) >> 1;
	if (size <= current_cpu_data.tlbsize / 2) {
=======
	local_irq_save(flags);
	size = (end - start + (PAGE_SIZE - 1)) >> PAGE_SHIFT;
	size = (size + 1) >> 1;
	if (size <= (current_cpu_data.tlbsizeftlbsets ?
		     current_cpu_data.tlbsize / 8 :
		     current_cpu_data.tlbsize / 2)) {
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		int pid = read_c0_entryhi();

		start &= (PAGE_MASK << 1);
		end += ((PAGE_SIZE << 1) - 1);
		end &= (PAGE_MASK << 1);
<<<<<<< HEAD
=======
		htw_stop();
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

		while (start < end) {
			int idx;

			write_c0_entryhi(start);
			start += (PAGE_SIZE << 1);
			mtc0_tlbw_hazard();
			tlb_probe();
			tlb_probe_hazard();
			idx = read_c0_index();
			write_c0_entrylo0(0);
			write_c0_entrylo1(0);
			if (idx < 0)
				continue;
			/* Make sure all entries differ. */
			write_c0_entryhi(UNIQUE_ENTRYHI(idx));
			mtc0_tlbw_hazard();
			tlb_write_indexed();
		}
		tlbw_use_hazard();
		write_c0_entryhi(pid);
<<<<<<< HEAD
	} else {
		local_flush_tlb_all();
	}
	FLUSH_ITLB;
	EXIT_CRITICAL(flags);
=======
		htw_start();
	} else {
		local_flush_tlb_all();
	}
	flush_micro_tlb();
	local_irq_restore(flags);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

void local_flush_tlb_page(struct vm_area_struct *vma, unsigned long page)
{
	int cpu = smp_processor_id();

	if (cpu_context(cpu, vma->vm_mm) != 0) {
<<<<<<< HEAD
		unsigned long flags;
		int oldpid, newpid, idx;

		newpid = cpu_asid(cpu, vma->vm_mm);
		page &= (PAGE_MASK << 1);
		ENTER_CRITICAL(flags);
		oldpid = read_c0_entryhi();
		write_c0_entryhi(page | newpid);
=======
		unsigned long old_mmid;
		unsigned long flags, old_entryhi;
		int idx;

		page &= (PAGE_MASK << 1);
		local_irq_save(flags);
		old_entryhi = read_c0_entryhi();
		htw_stop();
		if (cpu_has_mmid) {
			old_mmid = read_c0_memorymapid();
			write_c0_entryhi(page);
			write_c0_memorymapid(cpu_asid(cpu, vma->vm_mm));
		} else {
			write_c0_entryhi(page | cpu_asid(cpu, vma->vm_mm));
		}
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		mtc0_tlbw_hazard();
		tlb_probe();
		tlb_probe_hazard();
		idx = read_c0_index();
		write_c0_entrylo0(0);
		write_c0_entrylo1(0);
		if (idx < 0)
			goto finish;
		/* Make sure all entries differ. */
		write_c0_entryhi(UNIQUE_ENTRYHI(idx));
		mtc0_tlbw_hazard();
		tlb_write_indexed();
		tlbw_use_hazard();

	finish:
<<<<<<< HEAD
		write_c0_entryhi(oldpid);
		FLUSH_ITLB_VM(vma);
		EXIT_CRITICAL(flags);
=======
		write_c0_entryhi(old_entryhi);
		if (cpu_has_mmid)
			write_c0_memorymapid(old_mmid);
		htw_start();
		flush_micro_tlb_vm(vma);
		local_irq_restore(flags);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	}
}

/*
 * This one is only used for pages with the global bit set so we don't care
 * much about the ASID.
 */
void local_flush_tlb_one(unsigned long page)
{
	unsigned long flags;
	int oldpid, idx;

<<<<<<< HEAD
	ENTER_CRITICAL(flags);
	oldpid = read_c0_entryhi();
=======
	local_irq_save(flags);
	oldpid = read_c0_entryhi();
	htw_stop();
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	page &= (PAGE_MASK << 1);
	write_c0_entryhi(page);
	mtc0_tlbw_hazard();
	tlb_probe();
	tlb_probe_hazard();
	idx = read_c0_index();
	write_c0_entrylo0(0);
	write_c0_entrylo1(0);
	if (idx >= 0) {
		/* Make sure all entries differ. */
		write_c0_entryhi(UNIQUE_ENTRYHI(idx));
		mtc0_tlbw_hazard();
		tlb_write_indexed();
		tlbw_use_hazard();
	}
	write_c0_entryhi(oldpid);
<<<<<<< HEAD
	FLUSH_ITLB;
	EXIT_CRITICAL(flags);
=======
	htw_start();
	flush_micro_tlb();
	local_irq_restore(flags);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

/*
 * We will need multiple versions of update_mmu_cache(), one that just
 * updates the TLB with the new pte(s), and another which also checks
 * for the R4k "end of page" hardware bug and does the needy.
 */
void __update_tlb(struct vm_area_struct * vma, unsigned long address, pte_t pte)
{
	unsigned long flags;
	pgd_t *pgdp;
<<<<<<< HEAD
	pud_t *pudp;
	pmd_t *pmdp;
	pte_t *ptep;
	int idx, pid;

	/*
	 * Handle debugger faulting in for debugee.
=======
	p4d_t *p4dp;
	pud_t *pudp;
	pmd_t *pmdp;
	pte_t *ptep, *ptemap = NULL;
	int idx, pid;

	/*
	 * Handle debugger faulting in for debuggee.
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	 */
	if (current->active_mm != vma->vm_mm)
		return;

<<<<<<< HEAD
	ENTER_CRITICAL(flags);

	pid = read_c0_entryhi() & ASID_MASK;
	address &= (PAGE_MASK << 1);
	write_c0_entryhi(address | pid);
=======
	local_irq_save(flags);

	htw_stop();
	address &= (PAGE_MASK << 1);
	if (cpu_has_mmid) {
		write_c0_entryhi(address);
	} else {
		pid = read_c0_entryhi() & cpu_asid_mask(&current_cpu_data);
		write_c0_entryhi(address | pid);
	}
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	pgdp = pgd_offset(vma->vm_mm, address);
	mtc0_tlbw_hazard();
	tlb_probe();
	tlb_probe_hazard();
<<<<<<< HEAD
	pudp = pud_offset(pgdp, address);
	pmdp = pmd_offset(pudp, address);
	idx = read_c0_index();
#ifdef CONFIG_HUGETLB_PAGE
=======
	p4dp = p4d_offset(pgdp, address);
	pudp = pud_offset(p4dp, address);
	pmdp = pmd_offset(pudp, address);
	idx = read_c0_index();
#ifdef CONFIG_MIPS_HUGE_TLB_SUPPORT
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	/* this could be a huge page  */
	if (pmd_huge(*pmdp)) {
		unsigned long lo;
		write_c0_pagemask(PM_HUGE_MASK);
		ptep = (pte_t *)pmdp;
		lo = pte_to_entrylo(pte_val(*ptep));
		write_c0_entrylo0(lo);
		write_c0_entrylo1(lo + (HPAGE_SIZE >> 7));

		mtc0_tlbw_hazard();
		if (idx < 0)
			tlb_write_random();
		else
			tlb_write_indexed();
<<<<<<< HEAD
=======
		tlbw_use_hazard();
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		write_c0_pagemask(PM_DEFAULT_MASK);
	} else
#endif
	{
<<<<<<< HEAD
		ptep = pte_offset_map(pmdp, address);

#if defined(CONFIG_64BIT_PHYS_ADDR) && defined(CONFIG_CPU_MIPS32)
		write_c0_entrylo0(ptep->pte_high);
		ptep++;
		write_c0_entrylo1(ptep->pte_high);
=======
		ptemap = ptep = pte_offset_map(pmdp, address);
		/*
		 * update_mmu_cache() is called between pte_offset_map_lock()
		 * and pte_unmap_unlock(), so we can assume that ptep is not
		 * NULL here: and what should be done below if it were NULL?
		 */

#if defined(CONFIG_PHYS_ADDR_T_64BIT) && defined(CONFIG_CPU_MIPS32)
#ifdef CONFIG_XPA
		write_c0_entrylo0(pte_to_entrylo(ptep->pte_high));
		if (cpu_has_xpa)
			writex_c0_entrylo0(ptep->pte_low & _PFNX_MASK);
		ptep++;
		write_c0_entrylo1(pte_to_entrylo(ptep->pte_high));
		if (cpu_has_xpa)
			writex_c0_entrylo1(ptep->pte_low & _PFNX_MASK);
#else
		write_c0_entrylo0(ptep->pte_high);
		ptep++;
		write_c0_entrylo1(ptep->pte_high);
#endif
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#else
		write_c0_entrylo0(pte_to_entrylo(pte_val(*ptep++)));
		write_c0_entrylo1(pte_to_entrylo(pte_val(*ptep)));
#endif
		mtc0_tlbw_hazard();
		if (idx < 0)
			tlb_write_random();
		else
			tlb_write_indexed();
	}
	tlbw_use_hazard();
<<<<<<< HEAD
	FLUSH_ITLB_VM(vma);
	EXIT_CRITICAL(flags);
=======
	htw_start();
	flush_micro_tlb_vm(vma);

	if (ptemap)
		pte_unmap(ptemap);
	local_irq_restore(flags);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

void add_wired_entry(unsigned long entrylo0, unsigned long entrylo1,
		     unsigned long entryhi, unsigned long pagemask)
{
<<<<<<< HEAD
=======
#ifdef CONFIG_XPA
	panic("Broken for XPA kernels");
#else
	unsigned int old_mmid;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	unsigned long flags;
	unsigned long wired;
	unsigned long old_pagemask;
	unsigned long old_ctx;

<<<<<<< HEAD
	ENTER_CRITICAL(flags);
	/* Save old context and create impossible VPN2 value */
	old_ctx = read_c0_entryhi();
	old_pagemask = read_c0_pagemask();
	wired = read_c0_wired();
=======
	local_irq_save(flags);
	if (cpu_has_mmid) {
		old_mmid = read_c0_memorymapid();
		write_c0_memorymapid(MMID_KERNEL_WIRED);
	}
	/* Save old context and create impossible VPN2 value */
	old_ctx = read_c0_entryhi();
	htw_stop();
	old_pagemask = read_c0_pagemask();
	wired = num_wired_entries();
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	write_c0_wired(wired + 1);
	write_c0_index(wired);
	tlbw_use_hazard();	/* What is the hazard here? */
	write_c0_pagemask(pagemask);
	write_c0_entryhi(entryhi);
	write_c0_entrylo0(entrylo0);
	write_c0_entrylo1(entrylo1);
	mtc0_tlbw_hazard();
	tlb_write_indexed();
	tlbw_use_hazard();

	write_c0_entryhi(old_ctx);
<<<<<<< HEAD
	tlbw_use_hazard();	/* What is the hazard here? */
	write_c0_pagemask(old_pagemask);
	local_flush_tlb_all();
	EXIT_CRITICAL(flags);
}

static int __cpuinitdata ntlb;
=======
	if (cpu_has_mmid)
		write_c0_memorymapid(old_mmid);
	tlbw_use_hazard();	/* What is the hazard here? */
	htw_start();
	write_c0_pagemask(old_pagemask);
	local_flush_tlb_all();
	local_irq_restore(flags);
#endif
}

#ifdef CONFIG_TRANSPARENT_HUGEPAGE

int has_transparent_hugepage(void)
{
	static unsigned int mask = -1;

	if (mask == -1) {	/* first call comes during __init */
		unsigned long flags;

		local_irq_save(flags);
		write_c0_pagemask(PM_HUGE_MASK);
		back_to_back_c0_hazard();
		mask = read_c0_pagemask();
		write_c0_pagemask(PM_DEFAULT_MASK);
		local_irq_restore(flags);
	}
	return mask == PM_HUGE_MASK;
}
EXPORT_SYMBOL(has_transparent_hugepage);

#endif /* CONFIG_TRANSPARENT_HUGEPAGE  */

/*
 * Used for loading TLB entries before trap_init() has started, when we
 * don't actually want to add a wired entry which remains throughout the
 * lifetime of the system
 */

int temp_tlb_entry;

#ifndef CONFIG_64BIT
__init int add_temporary_entry(unsigned long entrylo0, unsigned long entrylo1,
			       unsigned long entryhi, unsigned long pagemask)
{
	int ret = 0;
	unsigned long flags;
	unsigned long wired;
	unsigned long old_pagemask;
	unsigned long old_ctx;

	local_irq_save(flags);
	/* Save old context and create impossible VPN2 value */
	htw_stop();
	old_ctx = read_c0_entryhi();
	old_pagemask = read_c0_pagemask();
	wired = num_wired_entries();
	if (--temp_tlb_entry < wired) {
		printk(KERN_WARNING
		       "No TLB space left for add_temporary_entry\n");
		ret = -ENOSPC;
		goto out;
	}

	write_c0_index(temp_tlb_entry);
	write_c0_pagemask(pagemask);
	write_c0_entryhi(entryhi);
	write_c0_entrylo0(entrylo0);
	write_c0_entrylo1(entrylo1);
	mtc0_tlbw_hazard();
	tlb_write_indexed();
	tlbw_use_hazard();

	write_c0_entryhi(old_ctx);
	write_c0_pagemask(old_pagemask);
	htw_start();
out:
	local_irq_restore(flags);
	return ret;
}
#endif

static int ntlb;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
static int __init set_ntlb(char *str)
{
	get_option(&str, &ntlb);
	return 1;
}

__setup("ntlb=", set_ntlb);

<<<<<<< HEAD
void __cpuinit tlb_init(void)
=======
/*
 * Configure TLB (for init or after a CPU has been powered off).
 */
static void r4k_tlb_configure(void)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
{
	/*
	 * You should never change this register:
	 *   - On R4600 1.7 the tlbp never hits for pages smaller than
	 *     the value in the c0_pagemask register.
	 *   - The entire mm handling assumes the c0_pagemask register to
	 *     be set to fixed-size pages.
	 */
	write_c0_pagemask(PM_DEFAULT_MASK);
<<<<<<< HEAD
	write_c0_wired(0);
	if (current_cpu_type() == CPU_R10000 ||
	    current_cpu_type() == CPU_R12000 ||
	    current_cpu_type() == CPU_R14000)
		write_c0_framemask(0);

	if (kernel_uses_smartmips_rixi) {
		/*
		 * Enable the no read, no exec bits, and enable large virtual
		 * address.
		 */
		u32 pg = PG_RIE | PG_XIE;
#ifdef CONFIG_64BIT
		pg |= PG_ELPA;
#endif
		write_c0_pagegrain(pg);
	}

        /* From this point on the ARC firmware is dead.  */
	local_flush_tlb_all();

	/* Did I tell you that ARC SUCKS?  */
=======
	back_to_back_c0_hazard();
	if (read_c0_pagemask() != PM_DEFAULT_MASK)
		panic("MMU doesn't support PAGE_SIZE=0x%lx", PAGE_SIZE);

	write_c0_wired(0);
	if (current_cpu_type() == CPU_R10000 ||
	    current_cpu_type() == CPU_R12000 ||
	    current_cpu_type() == CPU_R14000 ||
	    current_cpu_type() == CPU_R16000)
		write_c0_framemask(0);

	if (cpu_has_rixi) {
		/*
		 * Enable the no read, no exec bits, and enable large physical
		 * address.
		 */
#ifdef CONFIG_64BIT
		set_c0_pagegrain(PG_RIE | PG_XIE | PG_ELPA);
#else
		set_c0_pagegrain(PG_RIE | PG_XIE);
#endif
	}

	temp_tlb_entry = current_cpu_data.tlbsize - 1;

	/* From this point on the ARC firmware is dead.	 */
	local_flush_tlb_all();

	/* Did I tell you that ARC SUCKS?  */
}

void tlb_init(void)
{
	r4k_tlb_configure();
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	if (ntlb) {
		if (ntlb > 1 && ntlb <= current_cpu_data.tlbsize) {
			int wired = current_cpu_data.tlbsize - ntlb;
			write_c0_wired(wired);
			write_c0_index(wired-1);
			printk("Restricting TLB to %d entries\n", ntlb);
		} else
			printk("Ignoring invalid argument ntlb=%d\n", ntlb);
	}

	build_tlb_refill_handler();
}
<<<<<<< HEAD
=======

static int r4k_tlb_pm_notifier(struct notifier_block *self, unsigned long cmd,
			       void *v)
{
	switch (cmd) {
	case CPU_PM_ENTER_FAILED:
	case CPU_PM_EXIT:
		r4k_tlb_configure();
		break;
	}

	return NOTIFY_OK;
}

static struct notifier_block r4k_tlb_pm_notifier_block = {
	.notifier_call = r4k_tlb_pm_notifier,
};

static int __init r4k_tlb_init_pm(void)
{
	return cpu_pm_register_notifier(&r4k_tlb_pm_notifier_block);
}
arch_initcall(r4k_tlb_init_pm);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
