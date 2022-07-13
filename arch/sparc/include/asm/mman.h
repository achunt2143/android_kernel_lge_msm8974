<<<<<<< HEAD
#ifndef __SPARC_MMAN_H__
#define __SPARC_MMAN_H__

#include <asm-generic/mman-common.h>

/* SunOS'ified... */

#define MAP_RENAME      MAP_ANONYMOUS   /* In SunOS terminology */
#define MAP_NORESERVE   0x40            /* don't reserve swap pages */
#define MAP_INHERIT     0x80            /* SunOS doesn't do this, but... */
#define MAP_LOCKED      0x100           /* lock the mapping */
#define _MAP_NEW        0x80000000      /* Binary compatibility is fun... */

#define MAP_GROWSDOWN	0x0200		/* stack-like segment */
#define MAP_DENYWRITE	0x0800		/* ETXTBSY */
#define MAP_EXECUTABLE	0x1000		/* mark it as an executable */

#define MCL_CURRENT     0x2000          /* lock all currently mapped pages */
#define MCL_FUTURE      0x4000          /* lock all additions to address space */

#define MAP_POPULATE	0x8000		/* populate (prefault) pagetables */
#define MAP_NONBLOCK	0x10000		/* do not block on IO */
#define MAP_STACK	0x20000		/* give out an address that is best suited for process/thread stacks */
#define MAP_HUGETLB	0x40000		/* create a huge page mapping */

#ifdef __KERNEL__
#ifndef __ASSEMBLY__
#define arch_mmap_check(addr,len,flags)	sparc_mmap_check(addr,len)
int sparc_mmap_check(unsigned long addr, unsigned long len);
#endif
#endif

=======
/* SPDX-License-Identifier: GPL-2.0 */
#ifndef __SPARC_MMAN_H__
#define __SPARC_MMAN_H__

#include <uapi/asm/mman.h>

#ifndef __ASSEMBLY__
#define arch_mmap_check(addr,len,flags)	sparc_mmap_check(addr,len)
int sparc_mmap_check(unsigned long addr, unsigned long len);

#ifdef CONFIG_SPARC64
#include <asm/adi_64.h>

static inline void ipi_set_tstate_mcde(void *arg)
{
	struct mm_struct *mm = arg;

	/* Set TSTATE_MCDE for the task using address map that ADI has been
	 * enabled on if the task is running. If not, it will be set
	 * automatically at the next context switch
	 */
	if (current->mm == mm) {
		struct pt_regs *regs;

		regs = task_pt_regs(current);
		regs->tstate |= TSTATE_MCDE;
	}
}

#define arch_calc_vm_prot_bits(prot, pkey) sparc_calc_vm_prot_bits(prot)
static inline unsigned long sparc_calc_vm_prot_bits(unsigned long prot)
{
	if (adi_capable() && (prot & PROT_ADI)) {
		struct pt_regs *regs;

		if (!current->mm->context.adi) {
			regs = task_pt_regs(current);
			regs->tstate |= TSTATE_MCDE;
			current->mm->context.adi = true;
			on_each_cpu_mask(mm_cpumask(current->mm),
					 ipi_set_tstate_mcde, current->mm, 0);
		}
		return VM_SPARC_ADI;
	} else {
		return 0;
	}
}

#define arch_validate_prot(prot, addr) sparc_validate_prot(prot, addr)
static inline int sparc_validate_prot(unsigned long prot, unsigned long addr)
{
	if (prot & ~(PROT_READ | PROT_WRITE | PROT_EXEC | PROT_SEM | PROT_ADI))
		return 0;
	return 1;
}

#define arch_validate_flags(vm_flags) arch_validate_flags(vm_flags)
/* arch_validate_flags() - Ensure combination of flags is valid for a
 *	VMA.
 */
static inline bool arch_validate_flags(unsigned long vm_flags)
{
	/* If ADI is being enabled on this VMA, check for ADI
	 * capability on the platform and ensure VMA is suitable
	 * for ADI
	 */
	if (vm_flags & VM_SPARC_ADI) {
		if (!adi_capable())
			return false;

		/* ADI can not be enabled on PFN mapped pages */
		if (vm_flags & (VM_PFNMAP | VM_MIXEDMAP))
			return false;

		/* Mergeable pages can become unmergeable
		 * if ADI is enabled on them even if they
		 * have identical data on them. This can be
		 * because ADI enabled pages with identical
		 * data may still not have identical ADI
		 * tags on them. Disallow ADI on mergeable
		 * pages.
		 */
		if (vm_flags & VM_MERGEABLE)
			return false;
	}
	return true;
}
#endif /* CONFIG_SPARC64 */

#endif /* __ASSEMBLY__ */
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#endif /* __SPARC_MMAN_H__ */
