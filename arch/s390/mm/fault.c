<<<<<<< HEAD
/*
 *  arch/s390/mm/fault.c
 *
 *  S390 version
 *    Copyright (C) 1999 IBM Deutschland Entwicklung GmbH, IBM Corporation
 *    Author(s): Hartmut Penner (hp@de.ibm.com)
 *               Ulrich Weigand (uweigand@de.ibm.com)
=======
// SPDX-License-Identifier: GPL-2.0
/*
 *  S390 version
 *    Copyright IBM Corp. 1999
 *    Author(s): Hartmut Penner (hp@de.ibm.com)
 *		 Ulrich Weigand (uweigand@de.ibm.com)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 *
 *  Derived from "arch/i386/mm/fault.c"
 *    Copyright (C) 1995  Linus Torvalds
 */

#include <linux/kernel_stat.h>
<<<<<<< HEAD
#include <linux/perf_event.h>
#include <linux/signal.h>
#include <linux/sched.h>
=======
#include <linux/mmu_context.h>
#include <linux/perf_event.h>
#include <linux/signal.h>
#include <linux/sched.h>
#include <linux/sched/debug.h>
#include <linux/jump_label.h>
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#include <linux/kernel.h>
#include <linux/errno.h>
#include <linux/string.h>
#include <linux/types.h>
#include <linux/ptrace.h>
#include <linux/mman.h>
#include <linux/mm.h>
#include <linux/compat.h>
#include <linux/smp.h>
#include <linux/kdebug.h>
#include <linux/init.h>
#include <linux/console.h>
<<<<<<< HEAD
#include <linux/module.h>
=======
#include <linux/extable.h>
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#include <linux/hardirq.h>
#include <linux/kprobes.h>
#include <linux/uaccess.h>
#include <linux/hugetlb.h>
<<<<<<< HEAD
#include <asm/asm-offsets.h>
#include <asm/pgtable.h>
#include <asm/irq.h>
#include <asm/mmu_context.h>
#include <asm/facility.h>
#include "../kernel/entry.h"

#ifndef CONFIG_64BIT
#define __FAIL_ADDR_MASK 0x7ffff000
#define __SUBCODE_MASK 0x0200
#define __PF_RES_FIELD 0ULL
#else /* CONFIG_64BIT */
#define __FAIL_ADDR_MASK -4096L
#define __SUBCODE_MASK 0x0600
#define __PF_RES_FIELD 0x8000000000000000ULL
#endif /* CONFIG_64BIT */

#define VM_FAULT_BADCONTEXT	0x010000
#define VM_FAULT_BADMAP		0x020000
#define VM_FAULT_BADACCESS	0x040000

static unsigned long store_indication;

void fault_init(void)
{
	if (test_facility(2) && test_facility(75))
		store_indication = 0xc00;
}

static inline int notify_page_fault(struct pt_regs *regs)
{
	int ret = 0;

	/* kprobe_running() needs smp_processor_id() */
	if (kprobes_built_in() && !user_mode(regs)) {
		preempt_disable();
		if (kprobe_running() && kprobe_fault_handler(regs, 14))
			ret = 1;
		preempt_enable();
	}
	return ret;
}


/*
 * Unlock any spinlocks which will prevent us from getting the
 * message out.
 */
void bust_spinlocks(int yes)
{
	if (yes) {
		oops_in_progress = 1;
	} else {
		int loglevel_save = console_loglevel;
		console_unblank();
		oops_in_progress = 0;
		/*
		 * OK, the message is on the console.  Now we call printk()
		 * without oops_in_progress set so that printk will give klogd
		 * a poke.  Hold onto your hats...
		 */
		console_loglevel = 15;
		printk(" ");
		console_loglevel = loglevel_save;
	}
}

/*
 * Returns the address space associated with the fault.
 * Returns 0 for kernel space and 1 for user space.
 */
static inline int user_space_fault(unsigned long trans_exc_code)
{
	/*
	 * The lowest two bits of the translation exception
	 * identification indicate which paging table was used.
	 */
	trans_exc_code &= 3;
	if (trans_exc_code == 2)
		/* Access via secondary space, set_fs setting decides */
		return current->thread.mm_segment.ar4;
	if (user_mode == HOME_SPACE_MODE)
		/* User space if the access has been done via home space. */
		return trans_exc_code == 3;
	/*
	 * If the user space is not the home space the kernel runs in home
	 * space. Access via secondary space has already been covered,
	 * access via primary space or access register is from user space
	 * and access via home space is from the kernel.
	 */
	return trans_exc_code != 3;
}

static inline void report_user_fault(struct pt_regs *regs, long signr)
{
=======
#include <linux/kfence.h>
#include <asm/asm-extable.h>
#include <asm/asm-offsets.h>
#include <asm/ptrace.h>
#include <asm/fault.h>
#include <asm/diag.h>
#include <asm/gmap.h>
#include <asm/irq.h>
#include <asm/facility.h>
#include <asm/uv.h>
#include "../kernel/entry.h"

enum fault_type {
	KERNEL_FAULT,
	USER_FAULT,
	GMAP_FAULT,
};

static DEFINE_STATIC_KEY_FALSE(have_store_indication);

static int __init fault_init(void)
{
	if (test_facility(75))
		static_branch_enable(&have_store_indication);
	return 0;
}
early_initcall(fault_init);

/*
 * Find out which address space caused the exception.
 */
static enum fault_type get_fault_type(struct pt_regs *regs)
{
	union teid teid = { .val = regs->int_parm_long };
	struct gmap *gmap;

	if (likely(teid.as == PSW_BITS_AS_PRIMARY)) {
		if (user_mode(regs))
			return USER_FAULT;
		if (!IS_ENABLED(CONFIG_PGSTE))
			return KERNEL_FAULT;
		gmap = (struct gmap *)S390_lowcore.gmap;
		if (gmap && gmap->asce == regs->cr1)
			return GMAP_FAULT;
		return KERNEL_FAULT;
	}
	if (teid.as == PSW_BITS_AS_SECONDARY)
		return USER_FAULT;
	/* Access register mode, not used in the kernel */
	if (teid.as == PSW_BITS_AS_ACCREG)
		return USER_FAULT;
	/* Home space -> access via kernel ASCE */
	return KERNEL_FAULT;
}

static unsigned long get_fault_address(struct pt_regs *regs)
{
	union teid teid = { .val = regs->int_parm_long };

	return teid.addr * PAGE_SIZE;
}

static __always_inline bool fault_is_write(struct pt_regs *regs)
{
	union teid teid = { .val = regs->int_parm_long };

	if (static_branch_likely(&have_store_indication))
		return teid.fsi == TEID_FSI_STORE;
	return false;
}

static void dump_pagetable(unsigned long asce, unsigned long address)
{
	unsigned long entry, *table = __va(asce & _ASCE_ORIGIN);

	pr_alert("AS:%016lx ", asce);
	switch (asce & _ASCE_TYPE_MASK) {
	case _ASCE_TYPE_REGION1:
		table += (address & _REGION1_INDEX) >> _REGION1_SHIFT;
		if (get_kernel_nofault(entry, table))
			goto bad;
		pr_cont("R1:%016lx ", entry);
		if (entry & _REGION_ENTRY_INVALID)
			goto out;
		table = __va(entry & _REGION_ENTRY_ORIGIN);
		fallthrough;
	case _ASCE_TYPE_REGION2:
		table += (address & _REGION2_INDEX) >> _REGION2_SHIFT;
		if (get_kernel_nofault(entry, table))
			goto bad;
		pr_cont("R2:%016lx ", entry);
		if (entry & _REGION_ENTRY_INVALID)
			goto out;
		table = __va(entry & _REGION_ENTRY_ORIGIN);
		fallthrough;
	case _ASCE_TYPE_REGION3:
		table += (address & _REGION3_INDEX) >> _REGION3_SHIFT;
		if (get_kernel_nofault(entry, table))
			goto bad;
		pr_cont("R3:%016lx ", entry);
		if (entry & (_REGION_ENTRY_INVALID | _REGION3_ENTRY_LARGE))
			goto out;
		table = __va(entry & _REGION_ENTRY_ORIGIN);
		fallthrough;
	case _ASCE_TYPE_SEGMENT:
		table += (address & _SEGMENT_INDEX) >> _SEGMENT_SHIFT;
		if (get_kernel_nofault(entry, table))
			goto bad;
		pr_cont("S:%016lx ", entry);
		if (entry & (_SEGMENT_ENTRY_INVALID | _SEGMENT_ENTRY_LARGE))
			goto out;
		table = __va(entry & _SEGMENT_ENTRY_ORIGIN);
	}
	table += (address & _PAGE_INDEX) >> _PAGE_SHIFT;
	if (get_kernel_nofault(entry, table))
		goto bad;
	pr_cont("P:%016lx ", entry);
out:
	pr_cont("\n");
	return;
bad:
	pr_cont("BAD\n");
}

static void dump_fault_info(struct pt_regs *regs)
{
	union teid teid = { .val = regs->int_parm_long };
	unsigned long asce;

	pr_alert("Failing address: %016lx TEID: %016lx\n",
		 get_fault_address(regs), teid.val);
	pr_alert("Fault in ");
	switch (teid.as) {
	case PSW_BITS_AS_HOME:
		pr_cont("home space ");
		break;
	case PSW_BITS_AS_SECONDARY:
		pr_cont("secondary space ");
		break;
	case PSW_BITS_AS_ACCREG:
		pr_cont("access register ");
		break;
	case PSW_BITS_AS_PRIMARY:
		pr_cont("primary space ");
		break;
	}
	pr_cont("mode while using ");
	switch (get_fault_type(regs)) {
	case USER_FAULT:
		asce = S390_lowcore.user_asce.val;
		pr_cont("user ");
		break;
	case GMAP_FAULT:
		asce = ((struct gmap *)S390_lowcore.gmap)->asce;
		pr_cont("gmap ");
		break;
	case KERNEL_FAULT:
		asce = S390_lowcore.kernel_asce.val;
		pr_cont("kernel ");
		break;
	default:
		unreachable();
	}
	pr_cont("ASCE.\n");
	dump_pagetable(asce, get_fault_address(regs));
}

int show_unhandled_signals = 1;

void report_user_fault(struct pt_regs *regs, long signr, int is_mm_fault)
{
	static DEFINE_RATELIMIT_STATE(rs, DEFAULT_RATELIMIT_INTERVAL, DEFAULT_RATELIMIT_BURST);

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	if ((task_pid_nr(current) > 1) && !show_unhandled_signals)
		return;
	if (!unhandled_signal(current, signr))
		return;
<<<<<<< HEAD
	if (!printk_ratelimit())
		return;
	printk(KERN_ALERT "User process fault: interruption code 0x%X ",
	       regs->int_code);
	print_vma_addr(KERN_CONT "in ", regs->psw.addr & PSW_ADDR_INSN);
	printk(KERN_CONT "\n");
	printk(KERN_ALERT "failing address: %lX\n",
	       regs->int_parm_long & __FAIL_ADDR_MASK);
	show_regs(regs);
}

/*
 * Send SIGSEGV to task.  This is an external routine
 * to keep the stack usage of do_page_fault small.
 */
static noinline void do_sigsegv(struct pt_regs *regs, int si_code)
{
	struct siginfo si;

	report_user_fault(regs, SIGSEGV);
	si.si_signo = SIGSEGV;
	si.si_code = si_code;
	si.si_addr = (void __user *)(regs->int_parm_long & __FAIL_ADDR_MASK);
	force_sig_info(SIGSEGV, &si, current);
}

static noinline void do_no_context(struct pt_regs *regs)
{
	const struct exception_table_entry *fixup;
	unsigned long address;

	/* Are we prepared to handle this kernel fault?  */
	fixup = search_exception_tables(regs->psw.addr & PSW_ADDR_INSN);
	if (fixup) {
		regs->psw.addr = fixup->fixup | PSW_ADDR_AMODE;
		return;
	}

	/*
	 * Oops. The kernel tried to access some bad page. We'll have to
	 * terminate things with extreme prejudice.
	 */
	address = regs->int_parm_long & __FAIL_ADDR_MASK;
	if (!user_space_fault(regs->int_parm_long))
		printk(KERN_ALERT "Unable to handle kernel pointer dereference"
		       " at virtual kernel address %p\n", (void *)address);
	else
		printk(KERN_ALERT "Unable to handle kernel paging request"
		       " at virtual user address %p\n", (void *)address);

	die(regs, "Oops");
	do_exit(SIGKILL);
}

static noinline void do_low_address(struct pt_regs *regs)
{
	/* Low-address protection hit in kernel mode means
	   NULL pointer write access in kernel mode.  */
	if (regs->psw.mask & PSW_MASK_PSTATE) {
		/* Low-address protection hit in user mode 'cannot happen'. */
		die (regs, "Low-address protection");
		do_exit(SIGKILL);
	}

	do_no_context(regs);
}

static noinline void do_sigbus(struct pt_regs *regs)
{
	struct task_struct *tsk = current;
	struct siginfo si;

	/*
	 * Send a sigbus, regardless of whether we were in kernel
	 * or user mode.
	 */
	si.si_signo = SIGBUS;
	si.si_errno = 0;
	si.si_code = BUS_ADRERR;
	si.si_addr = (void __user *)(regs->int_parm_long & __FAIL_ADDR_MASK);
	force_sig_info(SIGBUS, &si, tsk);
}

static noinline void do_fault_error(struct pt_regs *regs, int fault)
{
	int si_code;

	switch (fault) {
	case VM_FAULT_BADACCESS:
	case VM_FAULT_BADMAP:
		/* Bad memory access. Check if it is kernel or user space. */
		if (regs->psw.mask & PSW_MASK_PSTATE) {
			/* User mode accesses just cause a SIGSEGV */
			si_code = (fault == VM_FAULT_BADMAP) ?
				SEGV_MAPERR : SEGV_ACCERR;
			do_sigsegv(regs, si_code);
			return;
		}
	case VM_FAULT_BADCONTEXT:
		do_no_context(regs);
		break;
	default: /* fault & VM_FAULT_ERROR */
		if (fault & VM_FAULT_OOM) {
			if (!(regs->psw.mask & PSW_MASK_PSTATE))
				do_no_context(regs);
			else
				pagefault_out_of_memory();
		} else if (fault & VM_FAULT_SIGSEGV) {
			/* Kernel mode? Handle exceptions or die */
			if (!user_mode(regs))
				do_no_context(regs);
			else
				do_sigsegv(regs, SEGV_MAPERR);
		} else if (fault & VM_FAULT_SIGBUS) {
			/* Kernel mode? Handle exceptions or die */
			if (!(regs->psw.mask & PSW_MASK_PSTATE))
				do_no_context(regs);
			else
				do_sigbus(regs);
		} else
			BUG();
		break;
	}
=======
	if (!__ratelimit(&rs))
		return;
	pr_alert("User process fault: interruption code %04x ilc:%d ",
		 regs->int_code & 0xffff, regs->int_code >> 17);
	print_vma_addr(KERN_CONT "in ", regs->psw.addr);
	pr_cont("\n");
	if (is_mm_fault)
		dump_fault_info(regs);
	show_regs(regs);
}

static void do_sigsegv(struct pt_regs *regs, int si_code)
{
	report_user_fault(regs, SIGSEGV, 1);
	force_sig_fault(SIGSEGV, si_code, (void __user *)get_fault_address(regs));
}

static void handle_fault_error_nolock(struct pt_regs *regs, int si_code)
{
	enum fault_type fault_type;
	unsigned long address;
	bool is_write;

	if (user_mode(regs)) {
		if (WARN_ON_ONCE(!si_code))
			si_code = SEGV_MAPERR;
		return do_sigsegv(regs, si_code);
	}
	if (fixup_exception(regs))
		return;
	fault_type = get_fault_type(regs);
	if (fault_type == KERNEL_FAULT) {
		address = get_fault_address(regs);
		is_write = fault_is_write(regs);
		if (kfence_handle_page_fault(address, is_write, regs))
			return;
	}
	if (fault_type == KERNEL_FAULT)
		pr_alert("Unable to handle kernel pointer dereference in virtual kernel address space\n");
	else
		pr_alert("Unable to handle kernel paging request in virtual user address space\n");
	dump_fault_info(regs);
	die(regs, "Oops");
}

static void handle_fault_error(struct pt_regs *regs, int si_code)
{
	struct mm_struct *mm = current->mm;

	mmap_read_unlock(mm);
	handle_fault_error_nolock(regs, si_code);
}

static void do_sigbus(struct pt_regs *regs)
{
	force_sig_fault(SIGBUS, BUS_ADRERR, (void __user *)get_fault_address(regs));
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

/*
 * This routine handles page faults.  It determines the address,
 * and the problem, and then passes it off to one of the appropriate
 * routines.
 *
 * interruption code (int_code):
<<<<<<< HEAD
 *   04       Protection           ->  Write-Protection  (suprression)
 *   10       Segment translation  ->  Not present       (nullification)
 *   11       Page translation     ->  Not present       (nullification)
 *   3b       Region third trans.  ->  Not present       (nullification)
 */
static inline int do_exception(struct pt_regs *regs, int access)
{
	struct task_struct *tsk;
	struct mm_struct *mm;
	struct vm_area_struct *vma;
	unsigned long trans_exc_code;
	unsigned long address;
	unsigned int flags;
	int fault;

	if (notify_page_fault(regs))
		return 0;

	tsk = current;
	mm = tsk->mm;
	trans_exc_code = regs->int_parm_long;

	/*
	 * Verify that the fault happened in user space, that
	 * we are not in an interrupt and that there is a 
	 * user context.
	 */
	fault = VM_FAULT_BADCONTEXT;
	if (unlikely(!user_space_fault(trans_exc_code) || in_atomic() || !mm))
		goto out;

	address = trans_exc_code & __FAIL_ADDR_MASK;
	perf_sw_event(PERF_COUNT_SW_PAGE_FAULTS, 1, regs, address);
	flags = FAULT_FLAG_ALLOW_RETRY;
	if (access == VM_WRITE || (trans_exc_code & store_indication) == 0x400)
		flags |= FAULT_FLAG_WRITE;
	down_read(&mm->mmap_sem);

#ifdef CONFIG_PGSTE
	if (test_tsk_thread_flag(current, TIF_SIE) && S390_lowcore.gmap) {
		address = __gmap_fault(address,
				     (struct gmap *) S390_lowcore.gmap);
		if (address == -EFAULT) {
			fault = VM_FAULT_BADMAP;
			goto out_up;
		}
		if (address == -ENOMEM) {
			fault = VM_FAULT_OOM;
			goto out_up;
		}
	}
#endif

retry:
	fault = VM_FAULT_BADMAP;
	vma = find_vma(mm, address);
	if (!vma)
		goto out_up;

	if (unlikely(vma->vm_start > address)) {
		if (!(vma->vm_flags & VM_GROWSDOWN))
			goto out_up;
		if (expand_stack(vma, address))
			goto out_up;
	}

	/*
	 * Ok, we have a good vm_area for this memory access, so
	 * we can handle it..
	 */
	fault = VM_FAULT_BADACCESS;
	if (unlikely(!(vma->vm_flags & access)))
		goto out_up;

	if (is_vm_hugetlb_page(vma))
		address &= HPAGE_MASK;
	/*
	 * If for any reason at all we couldn't handle the fault,
	 * make sure we exit gracefully rather than endlessly redo
	 * the fault.
	 */
	fault = handle_mm_fault(mm, vma, address, flags);
	if (unlikely(fault & VM_FAULT_ERROR))
		goto out_up;

	/*
	 * Major/minor page fault accounting is only done on the
	 * initial attempt. If we go through a retry, it is extremely
	 * likely that the page will be found in page cache at that point.
	 */
	if (flags & FAULT_FLAG_ALLOW_RETRY) {
		if (fault & VM_FAULT_MAJOR) {
			tsk->maj_flt++;
			perf_sw_event(PERF_COUNT_SW_PAGE_FAULTS_MAJ, 1,
				      regs, address);
		} else {
			tsk->min_flt++;
			perf_sw_event(PERF_COUNT_SW_PAGE_FAULTS_MIN, 1,
				      regs, address);
		}
		if (fault & VM_FAULT_RETRY) {
			/* Clear FAULT_FLAG_ALLOW_RETRY to avoid any risk
			 * of starvation. */
			flags &= ~FAULT_FLAG_ALLOW_RETRY;
			down_read(&mm->mmap_sem);
			goto retry;
		}
	}
	/*
	 * The instruction that caused the program check will
	 * be repeated. Don't signal single step via SIGTRAP.
	 */
	clear_tsk_thread_flag(tsk, TIF_PER_TRAP);
	fault = 0;
out_up:
	up_read(&mm->mmap_sem);
out:
	return fault;
}

void __kprobes do_protection_exception(struct pt_regs *regs)
{
	unsigned long trans_exc_code;
	int fault;

	trans_exc_code = regs->int_parm_long;
	/* Protection exception is suppressing, decrement psw address. */
	regs->psw.addr = __rewind_psw(regs->psw, regs->int_code >> 16);
=======
 *   04       Protection	   ->  Write-Protection  (suppression)
 *   10       Segment translation  ->  Not present	 (nullification)
 *   11       Page translation	   ->  Not present	 (nullification)
 *   3b       Region third trans.  ->  Not present	 (nullification)
 */
static void do_exception(struct pt_regs *regs, int access)
{
	struct vm_area_struct *vma;
	unsigned long address;
	struct mm_struct *mm;
	enum fault_type type;
	unsigned int flags;
	struct gmap *gmap;
	vm_fault_t fault;
	bool is_write;

	/*
	 * The instruction that caused the program check has
	 * been nullified. Don't signal single step via SIGTRAP.
	 */
	clear_thread_flag(TIF_PER_TRAP);
	if (kprobe_page_fault(regs, 14))
		return;
	mm = current->mm;
	address = get_fault_address(regs);
	is_write = fault_is_write(regs);
	type = get_fault_type(regs);
	switch (type) {
	case KERNEL_FAULT:
		return handle_fault_error_nolock(regs, 0);
	case USER_FAULT:
	case GMAP_FAULT:
		if (faulthandler_disabled() || !mm)
			return handle_fault_error_nolock(regs, 0);
		break;
	}
	perf_sw_event(PERF_COUNT_SW_PAGE_FAULTS, 1, regs, address);
	flags = FAULT_FLAG_DEFAULT;
	if (user_mode(regs))
		flags |= FAULT_FLAG_USER;
	if (is_write)
		access = VM_WRITE;
	if (access == VM_WRITE)
		flags |= FAULT_FLAG_WRITE;
	if (!(flags & FAULT_FLAG_USER))
		goto lock_mmap;
	vma = lock_vma_under_rcu(mm, address);
	if (!vma)
		goto lock_mmap;
	if (!(vma->vm_flags & access)) {
		vma_end_read(vma);
		goto lock_mmap;
	}
	fault = handle_mm_fault(vma, address, flags | FAULT_FLAG_VMA_LOCK, regs);
	if (!(fault & (VM_FAULT_RETRY | VM_FAULT_COMPLETED)))
		vma_end_read(vma);
	if (!(fault & VM_FAULT_RETRY)) {
		count_vm_vma_lock_event(VMA_LOCK_SUCCESS);
		if (unlikely(fault & VM_FAULT_ERROR))
			goto error;
		return;
	}
	count_vm_vma_lock_event(VMA_LOCK_RETRY);
	if (fault & VM_FAULT_MAJOR)
		flags |= FAULT_FLAG_TRIED;

	/* Quick path to respond to signals */
	if (fault_signal_pending(fault, regs)) {
		if (!user_mode(regs))
			handle_fault_error_nolock(regs, 0);
		return;
	}
lock_mmap:
	mmap_read_lock(mm);
	gmap = NULL;
	if (IS_ENABLED(CONFIG_PGSTE) && type == GMAP_FAULT) {
		gmap = (struct gmap *)S390_lowcore.gmap;
		current->thread.gmap_addr = address;
		current->thread.gmap_write_flag = !!(flags & FAULT_FLAG_WRITE);
		current->thread.gmap_int_code = regs->int_code & 0xffff;
		address = __gmap_translate(gmap, address);
		if (address == -EFAULT)
			return handle_fault_error(regs, SEGV_MAPERR);
		if (gmap->pfault_enabled)
			flags |= FAULT_FLAG_RETRY_NOWAIT;
	}
retry:
	vma = find_vma(mm, address);
	if (!vma)
		return handle_fault_error(regs, SEGV_MAPERR);
	if (unlikely(vma->vm_start > address)) {
		if (!(vma->vm_flags & VM_GROWSDOWN))
			return handle_fault_error(regs, SEGV_MAPERR);
		vma = expand_stack(mm, address);
		if (!vma)
			return handle_fault_error_nolock(regs, SEGV_MAPERR);
	}
	if (unlikely(!(vma->vm_flags & access)))
		return handle_fault_error(regs, SEGV_ACCERR);
	fault = handle_mm_fault(vma, address, flags, regs);
	if (fault_signal_pending(fault, regs)) {
		if (flags & FAULT_FLAG_RETRY_NOWAIT)
			mmap_read_unlock(mm);
		if (!user_mode(regs))
			handle_fault_error_nolock(regs, 0);
		return;
	}
	/* The fault is fully completed (including releasing mmap lock) */
	if (fault & VM_FAULT_COMPLETED) {
		if (gmap) {
			mmap_read_lock(mm);
			goto gmap;
		}
		return;
	}
	if (unlikely(fault & VM_FAULT_ERROR)) {
		mmap_read_unlock(mm);
		goto error;
	}
	if (fault & VM_FAULT_RETRY) {
		if (IS_ENABLED(CONFIG_PGSTE) && gmap &&	(flags & FAULT_FLAG_RETRY_NOWAIT)) {
			/*
			 * FAULT_FLAG_RETRY_NOWAIT has been set,
			 * mmap_lock has not been released
			 */
			current->thread.gmap_pfault = 1;
			return handle_fault_error(regs, 0);
		}
		flags &= ~FAULT_FLAG_RETRY_NOWAIT;
		flags |= FAULT_FLAG_TRIED;
		mmap_read_lock(mm);
		goto retry;
	}
gmap:
	if (IS_ENABLED(CONFIG_PGSTE) && gmap) {
		address =  __gmap_link(gmap, current->thread.gmap_addr,
				       address);
		if (address == -EFAULT)
			return handle_fault_error(regs, SEGV_MAPERR);
		if (address == -ENOMEM) {
			fault = VM_FAULT_OOM;
			mmap_read_unlock(mm);
			goto error;
		}
	}
	mmap_read_unlock(mm);
	return;
error:
	if (fault & VM_FAULT_OOM) {
		if (!user_mode(regs))
			handle_fault_error_nolock(regs, 0);
		else
			pagefault_out_of_memory();
	} else if (fault & VM_FAULT_SIGSEGV) {
		if (!user_mode(regs))
			handle_fault_error_nolock(regs, 0);
		else
			do_sigsegv(regs, SEGV_MAPERR);
	} else if (fault & VM_FAULT_SIGBUS) {
		if (!user_mode(regs))
			handle_fault_error_nolock(regs, 0);
		else
			do_sigbus(regs);
	} else {
		BUG();
	}
}

void do_protection_exception(struct pt_regs *regs)
{
	union teid teid = { .val = regs->int_parm_long };

	/*
	 * Protection exceptions are suppressing, decrement psw address.
	 * The exception to this rule are aborted transactions, for these
	 * the PSW already points to the correct location.
	 */
	if (!(regs->int_code & 0x200))
		regs->psw.addr = __rewind_psw(regs->psw, regs->int_code >> 16);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	/*
	 * Check for low-address protection.  This needs to be treated
	 * as a special case because the translation exception code
	 * field is not guaranteed to contain valid data in this case.
	 */
<<<<<<< HEAD
	if (unlikely(!(trans_exc_code & 4))) {
		do_low_address(regs);
		return;
	}
	fault = do_exception(regs, VM_WRITE);
	if (unlikely(fault))
		do_fault_error(regs, fault);
}

void __kprobes do_dat_exception(struct pt_regs *regs)
{
	int access, fault;

	access = VM_READ | VM_EXEC | VM_WRITE;
	fault = do_exception(regs, access);
	if (unlikely(fault))
		do_fault_error(regs, fault);
}

#ifdef CONFIG_64BIT
void __kprobes do_asce_exception(struct pt_regs *regs)
{
	struct mm_struct *mm = current->mm;
	struct vm_area_struct *vma;
	unsigned long trans_exc_code;

	trans_exc_code = regs->int_parm_long;
	if (unlikely(!user_space_fault(trans_exc_code) || in_atomic() || !mm))
		goto no_context;

	down_read(&mm->mmap_sem);
	vma = find_vma(mm, trans_exc_code & __FAIL_ADDR_MASK);
	up_read(&mm->mmap_sem);

	if (vma) {
		update_mm(mm, current);
		return;
	}

	/* User mode accesses just cause a SIGSEGV */
	if (regs->psw.mask & PSW_MASK_PSTATE) {
		do_sigsegv(regs, SEGV_MAPERR);
		return;
	}

no_context:
	do_no_context(regs);
}
#endif

int __handle_fault(unsigned long uaddr, unsigned long pgm_int_code, int write)
{
	struct pt_regs regs;
	int access, fault;

	/* Emulate a uaccess fault from kernel mode. */
	regs.psw.mask = psw_kernel_bits | PSW_MASK_DAT | PSW_MASK_MCHECK;
	if (!irqs_disabled())
		regs.psw.mask |= PSW_MASK_IO | PSW_MASK_EXT;
	regs.psw.addr = (unsigned long) __builtin_return_address(0);
	regs.psw.addr |= PSW_ADDR_AMODE;
	regs.int_code = pgm_int_code;
	regs.int_parm_long = (uaddr & PAGE_MASK) | 2;
	access = write ? VM_WRITE : VM_READ;
	fault = do_exception(&regs, access);
	/*
	 * Since the fault happened in kernel mode while performing a uaccess
	 * all we need to do now is emulating a fixup in case "fault" is not
	 * zero.
	 * For the calling uaccess functions this results always in -EFAULT.
	 */
	return fault ? -EFAULT : 0;
}

#ifdef CONFIG_PFAULT 
/*
 * 'pfault' pseudo page faults routines.
 */
static int pfault_disable;

static int __init nopfault(char *str)
{
	pfault_disable = 1;
	return 1;
}

__setup("nopfault", nopfault);

struct pfault_refbk {
	u16 refdiagc;
	u16 reffcode;
	u16 refdwlen;
	u16 refversn;
	u64 refgaddr;
	u64 refselmk;
	u64 refcmpmk;
	u64 reserved;
} __attribute__ ((packed, aligned(8)));

int pfault_init(void)
{
	struct pfault_refbk refbk = {
		.refdiagc = 0x258,
		.reffcode = 0,
		.refdwlen = 5,
		.refversn = 2,
		.refgaddr = __LC_CURRENT_PID,
		.refselmk = 1ULL << 48,
		.refcmpmk = 1ULL << 48,
		.reserved = __PF_RES_FIELD };
        int rc;

	if (pfault_disable)
		return -1;
	asm volatile(
		"	diag	%1,%0,0x258\n"
		"0:	j	2f\n"
		"1:	la	%0,8\n"
		"2:\n"
		EX_TABLE(0b,1b)
		: "=d" (rc) : "a" (&refbk), "m" (refbk) : "cc");
        return rc;
}

void pfault_fini(void)
{
	struct pfault_refbk refbk = {
		.refdiagc = 0x258,
		.reffcode = 1,
		.refdwlen = 5,
		.refversn = 2,
	};

	if (pfault_disable)
		return;
	asm volatile(
		"	diag	%0,0,0x258\n"
		"0:\n"
		EX_TABLE(0b,0b)
		: : "a" (&refbk), "m" (refbk) : "cc");
}

static DEFINE_SPINLOCK(pfault_lock);
static LIST_HEAD(pfault_list);

static void pfault_interrupt(struct ext_code ext_code,
			     unsigned int param32, unsigned long param64)
{
	struct task_struct *tsk;
	__u16 subcode;
	pid_t pid;

	/*
	 * Get the external interruption subcode & pfault
	 * initial/completion signal bit. VM stores this 
	 * in the 'cpu address' field associated with the
         * external interrupt. 
	 */
	subcode = ext_code.subcode;
	if ((subcode & 0xff00) != __SUBCODE_MASK)
		return;
	kstat_cpu(smp_processor_id()).irqs[EXTINT_PFL]++;
	if (subcode & 0x0080) {
		/* Get the token (= pid of the affected task). */
		pid = sizeof(void *) == 4 ? param32 : param64;
		rcu_read_lock();
		tsk = find_task_by_pid_ns(pid, &init_pid_ns);
		if (tsk)
			get_task_struct(tsk);
		rcu_read_unlock();
		if (!tsk)
			return;
	} else {
		tsk = current;
	}
	spin_lock(&pfault_lock);
	if (subcode & 0x0080) {
		/* signal bit is set -> a page has been swapped in by VM */
		if (tsk->thread.pfault_wait == 1) {
			/* Initial interrupt was faster than the completion
			 * interrupt. pfault_wait is valid. Set pfault_wait
			 * back to zero and wake up the process. This can
			 * safely be done because the task is still sleeping
			 * and can't produce new pfaults. */
			tsk->thread.pfault_wait = 0;
			list_del(&tsk->thread.list);
			wake_up_process(tsk);
			put_task_struct(tsk);
		} else {
			/* Completion interrupt was faster than initial
			 * interrupt. Set pfault_wait to -1 so the initial
			 * interrupt doesn't put the task to sleep.
			 * If the task is not running, ignore the completion
			 * interrupt since it must be a leftover of a PFAULT
			 * CANCEL operation which didn't remove all pending
			 * completion interrupts. */
			if (tsk->state == TASK_RUNNING)
				tsk->thread.pfault_wait = -1;
		}
		put_task_struct(tsk);
	} else {
		/* signal bit not set -> a real page is missing. */
		if (tsk->thread.pfault_wait == 1) {
			/* Already on the list with a reference: put to sleep */
			set_task_state(tsk, TASK_UNINTERRUPTIBLE);
			set_tsk_need_resched(tsk);
		} else if (tsk->thread.pfault_wait == -1) {
			/* Completion interrupt was faster than the initial
			 * interrupt (pfault_wait == -1). Set pfault_wait
			 * back to zero and exit. */
			tsk->thread.pfault_wait = 0;
		} else {
			/* Initial interrupt arrived before completion
			 * interrupt. Let the task sleep.
			 * An extra task reference is needed since a different
			 * cpu may set the task state to TASK_RUNNING again
			 * before the scheduler is reached. */
			get_task_struct(tsk);
			tsk->thread.pfault_wait = 1;
			list_add(&tsk->thread.list, &pfault_list);
			set_task_state(tsk, TASK_UNINTERRUPTIBLE);
			set_tsk_need_resched(tsk);
		}
	}
	spin_unlock(&pfault_lock);
}

static int __cpuinit pfault_cpu_notify(struct notifier_block *self,
				       unsigned long action, void *hcpu)
{
	struct thread_struct *thread, *next;
	struct task_struct *tsk;

	switch (action) {
	case CPU_DEAD:
	case CPU_DEAD_FROZEN:
		spin_lock_irq(&pfault_lock);
		list_for_each_entry_safe(thread, next, &pfault_list, list) {
			thread->pfault_wait = 0;
			list_del(&thread->list);
			tsk = container_of(thread, struct task_struct, thread);
			wake_up_process(tsk);
			put_task_struct(tsk);
		}
		spin_unlock_irq(&pfault_lock);
		break;
	default:
		break;
	}
	return NOTIFY_OK;
}

static int __init pfault_irq_init(void)
{
	int rc;

	rc = register_external_interrupt(0x2603, pfault_interrupt);
	if (rc)
		goto out_extint;
	rc = pfault_init() == 0 ? 0 : -EOPNOTSUPP;
	if (rc)
		goto out_pfault;
	service_subclass_irq_register();
	hotcpu_notifier(pfault_cpu_notify, 0);
	return 0;

out_pfault:
	unregister_external_interrupt(0x2603, pfault_interrupt);
out_extint:
	pfault_disable = 1;
	return rc;
}
early_initcall(pfault_irq_init);

#endif /* CONFIG_PFAULT */
=======
	if (unlikely(!teid.b61)) {
		if (user_mode(regs)) {
			/* Low-address protection in user mode: cannot happen */
			die(regs, "Low-address protection");
		}
		/*
		 * Low-address protection in kernel mode means
		 * NULL pointer write access in kernel mode.
		 */
		return handle_fault_error_nolock(regs, 0);
	}
	if (unlikely(MACHINE_HAS_NX && teid.b56)) {
		regs->int_parm_long = (teid.addr * PAGE_SIZE) | (regs->psw.addr & PAGE_MASK);
		return handle_fault_error_nolock(regs, SEGV_ACCERR);
	}
	do_exception(regs, VM_WRITE);
}
NOKPROBE_SYMBOL(do_protection_exception);

void do_dat_exception(struct pt_regs *regs)
{
	do_exception(regs, VM_ACCESS_FLAGS);
}
NOKPROBE_SYMBOL(do_dat_exception);

#if IS_ENABLED(CONFIG_PGSTE)

void do_secure_storage_access(struct pt_regs *regs)
{
	union teid teid = { .val = regs->int_parm_long };
	unsigned long addr = get_fault_address(regs);
	struct vm_area_struct *vma;
	struct mm_struct *mm;
	struct page *page;
	struct gmap *gmap;
	int rc;

	/*
	 * Bit 61 indicates if the address is valid, if it is not the
	 * kernel should be stopped or SIGSEGV should be sent to the
	 * process. Bit 61 is not reliable without the misc UV feature,
	 * therefore this needs to be checked too.
	 */
	if (uv_has_feature(BIT_UV_FEAT_MISC) && !teid.b61) {
		/*
		 * When this happens, userspace did something that it
		 * was not supposed to do, e.g. branching into secure
		 * memory. Trigger a segmentation fault.
		 */
		if (user_mode(regs)) {
			send_sig(SIGSEGV, current, 0);
			return;
		}
		/*
		 * The kernel should never run into this case and
		 * there is no way out of this situation.
		 */
		panic("Unexpected PGM 0x3d with TEID bit 61=0");
	}
	switch (get_fault_type(regs)) {
	case GMAP_FAULT:
		mm = current->mm;
		gmap = (struct gmap *)S390_lowcore.gmap;
		mmap_read_lock(mm);
		addr = __gmap_translate(gmap, addr);
		mmap_read_unlock(mm);
		if (IS_ERR_VALUE(addr))
			return handle_fault_error_nolock(regs, SEGV_MAPERR);
		fallthrough;
	case USER_FAULT:
		mm = current->mm;
		mmap_read_lock(mm);
		vma = find_vma(mm, addr);
		if (!vma)
			return handle_fault_error(regs, SEGV_MAPERR);
		page = follow_page(vma, addr, FOLL_WRITE | FOLL_GET);
		if (IS_ERR_OR_NULL(page)) {
			mmap_read_unlock(mm);
			break;
		}
		if (arch_make_page_accessible(page))
			send_sig(SIGSEGV, current, 0);
		put_page(page);
		mmap_read_unlock(mm);
		break;
	case KERNEL_FAULT:
		page = phys_to_page(addr);
		if (unlikely(!try_get_page(page)))
			break;
		rc = arch_make_page_accessible(page);
		put_page(page);
		if (rc)
			BUG();
		break;
	default:
		unreachable();
	}
}
NOKPROBE_SYMBOL(do_secure_storage_access);

void do_non_secure_storage_access(struct pt_regs *regs)
{
	struct gmap *gmap = (struct gmap *)S390_lowcore.gmap;
	unsigned long gaddr = get_fault_address(regs);

	if (WARN_ON_ONCE(get_fault_type(regs) != GMAP_FAULT))
		return handle_fault_error_nolock(regs, SEGV_MAPERR);
	if (gmap_convert_to_secure(gmap, gaddr) == -EINVAL)
		send_sig(SIGSEGV, current, 0);
}
NOKPROBE_SYMBOL(do_non_secure_storage_access);

void do_secure_storage_violation(struct pt_regs *regs)
{
	struct gmap *gmap = (struct gmap *)S390_lowcore.gmap;
	unsigned long gaddr = get_fault_address(regs);

	/*
	 * If the VM has been rebooted, its address space might still contain
	 * secure pages from the previous boot.
	 * Clear the page so it can be reused.
	 */
	if (!gmap_destroy_page(gmap, gaddr))
		return;
	/*
	 * Either KVM messed up the secure guest mapping or the same
	 * page is mapped into multiple secure guests.
	 *
	 * This exception is only triggered when a guest 2 is running
	 * and can therefore never occur in kernel context.
	 */
	pr_warn_ratelimited("Secure storage violation in task: %s, pid %d\n",
			    current->comm, current->pid);
	send_sig(SIGSEGV, current, 0);
}

#endif /* CONFIG_PGSTE */
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
