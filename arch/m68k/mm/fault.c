<<<<<<< HEAD
=======
// SPDX-License-Identifier: GPL-2.0
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
/*
 *  linux/arch/m68k/mm/fault.c
 *
 *  Copyright (C) 1995  Hamish Macdonald
 */

#include <linux/mman.h>
#include <linux/mm.h>
#include <linux/kernel.h>
#include <linux/ptrace.h>
#include <linux/interrupt.h>
#include <linux/module.h>
<<<<<<< HEAD

#include <asm/setup.h>
#include <asm/traps.h>
#include <asm/uaccess.h>
#include <asm/pgalloc.h>
=======
#include <linux/uaccess.h>
#include <linux/perf_event.h>

#include <asm/setup.h>
#include <asm/traps.h>

#include "fault.h"
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

extern void die_if_kernel(char *, struct pt_regs *, long);

int send_fault_sig(struct pt_regs *regs)
{
<<<<<<< HEAD
	siginfo_t siginfo = { 0, 0, 0, };

	siginfo.si_signo = current->thread.signo;
	siginfo.si_code = current->thread.code;
	siginfo.si_addr = (void *)current->thread.faddr;
#ifdef DEBUG
	printk("send_fault_sig: %p,%d,%d\n", siginfo.si_addr, siginfo.si_signo, siginfo.si_code);
#endif

	if (user_mode(regs)) {
		force_sig_info(siginfo.si_signo,
			       &siginfo, current);
	} else {
		if (handle_kernel_fault(regs))
			return -1;

		//if (siginfo.si_signo == SIGBUS)
		//	force_sig_info(siginfo.si_signo,
		//		       &siginfo, current);
=======
	int signo, si_code;
	void __user *addr;

	signo = current->thread.signo;
	si_code = current->thread.code;
	addr = (void __user *)current->thread.faddr;
	pr_debug("send_fault_sig: %p,%d,%d\n", addr, signo, si_code);

	if (user_mode(regs)) {
		force_sig_fault(signo, si_code, addr);
	} else {
		if (fixup_exception(regs))
			return -1;

		//if (signo == SIGBUS)
		//	force_sig_fault(si_signo, si_code, addr);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

		/*
		 * Oops. The kernel tried to access some bad page. We'll have to
		 * terminate things with extreme prejudice.
		 */
<<<<<<< HEAD
		if ((unsigned long)siginfo.si_addr < PAGE_SIZE)
			printk(KERN_ALERT "Unable to handle kernel NULL pointer dereference");
		else
			printk(KERN_ALERT "Unable to handle kernel access");
		printk(" at virtual address %p\n", siginfo.si_addr);
		die_if_kernel("Oops", regs, 0 /*error_code*/);
		do_exit(SIGKILL);
=======
		if ((unsigned long)addr < PAGE_SIZE)
			pr_alert("Unable to handle kernel NULL pointer dereference");
		else
			pr_alert("Unable to handle kernel access");
		pr_cont(" at virtual address %p\n", addr);
		die_if_kernel("Oops", regs, 0 /*error_code*/);
		make_task_dead(SIGKILL);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	}

	return 1;
}

/*
 * This routine handles page faults.  It determines the problem, and
 * then passes it off to one of the appropriate routines.
 *
 * error_code:
 *	bit 0 == 0 means no page found, 1 means protection fault
 *	bit 1 == 0 means read, 1 means write
 *
 * If this routine detects a bad access, it returns 1, otherwise it
 * returns 0.
 */
int do_page_fault(struct pt_regs *regs, unsigned long address,
			      unsigned long error_code)
{
	struct mm_struct *mm = current->mm;
	struct vm_area_struct * vma;
<<<<<<< HEAD
	int write, fault;

#ifdef DEBUG
	printk ("do page fault:\nregs->sr=%#x, regs->pc=%#lx, address=%#lx, %ld, %p\n",
		regs->sr, regs->pc, address, error_code,
		current->mm->pgd);
#endif
=======
	vm_fault_t fault;
	unsigned int flags = FAULT_FLAG_DEFAULT;

	pr_debug("do page fault:\nregs->sr=%#x, regs->pc=%#lx, address=%#lx, %ld, %p\n",
		regs->sr, regs->pc, address, error_code, mm ? mm->pgd : NULL);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	/*
	 * If we're in an interrupt or have no user
	 * context, we must not take the fault..
	 */
<<<<<<< HEAD
	if (in_atomic() || !mm)
		goto no_context;

	down_read(&mm->mmap_sem);
=======
	if (faulthandler_disabled() || !mm)
		goto no_context;

	if (user_mode(regs))
		flags |= FAULT_FLAG_USER;

	perf_sw_event(PERF_COUNT_SW_PAGE_FAULTS, 1, regs, address);
retry:
	mmap_read_lock(mm);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	vma = find_vma(mm, address);
	if (!vma)
		goto map_err;
<<<<<<< HEAD
	if (vma->vm_flags & VM_IO)
		goto acc_err;
=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	if (vma->vm_start <= address)
		goto good_area;
	if (!(vma->vm_flags & VM_GROWSDOWN))
		goto map_err;
	if (user_mode(regs)) {
		/* Accessing the stack below usp is always a bug.  The
		   "+ 256" is there due to some instructions doing
		   pre-decrement on the stack and that doesn't show up
		   until later.  */
		if (address + 256 < rdusp())
			goto map_err;
	}
<<<<<<< HEAD
	if (expand_stack(vma, address))
		goto map_err;
=======
	vma = expand_stack(mm, address);
	if (!vma)
		goto map_err_nosemaphore;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

/*
 * Ok, we have a good vm_area for this memory access, so
 * we can handle it..
 */
good_area:
<<<<<<< HEAD
#ifdef DEBUG
	printk("do_page_fault: good_area\n");
#endif
	write = 0;
	switch (error_code & 3) {
		default:	/* 3: write, present */
			/* fall through */
		case 2:		/* write, not present */
			if (!(vma->vm_flags & VM_WRITE))
				goto acc_err;
			write++;
=======
	pr_debug("do_page_fault: good_area\n");
	switch (error_code & 3) {
		default:	/* 3: write, present */
			fallthrough;
		case 2:		/* write, not present */
			if (!(vma->vm_flags & VM_WRITE))
				goto acc_err;
			flags |= FAULT_FLAG_WRITE;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
			break;
		case 1:		/* read, present */
			goto acc_err;
		case 0:		/* read, not present */
<<<<<<< HEAD
			if (!(vma->vm_flags & (VM_READ | VM_EXEC | VM_WRITE)))
=======
			if (unlikely(!vma_is_accessible(vma)))
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
				goto acc_err;
	}

	/*
	 * If for any reason at all we couldn't handle the fault,
	 * make sure we exit gracefully rather than endlessly redo
	 * the fault.
	 */

<<<<<<< HEAD
	fault = handle_mm_fault(mm, vma, address, write ? FAULT_FLAG_WRITE : 0);
#ifdef DEBUG
	printk("handle_mm_fault returns %d\n",fault);
#endif
=======
	fault = handle_mm_fault(vma, address, flags, regs);
	pr_debug("handle_mm_fault returns %x\n", fault);

	if (fault_signal_pending(fault, regs)) {
		if (!user_mode(regs))
			goto no_context;
		return 0;
	}

	/* The fault is fully completed (including releasing mmap lock) */
	if (fault & VM_FAULT_COMPLETED)
		return 0;

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	if (unlikely(fault & VM_FAULT_ERROR)) {
		if (fault & VM_FAULT_OOM)
			goto out_of_memory;
		else if (fault & VM_FAULT_SIGSEGV)
			goto map_err;
		else if (fault & VM_FAULT_SIGBUS)
			goto bus_err;
		BUG();
	}
<<<<<<< HEAD
	if (fault & VM_FAULT_MAJOR)
		current->maj_flt++;
	else
		current->min_flt++;

	up_read(&mm->mmap_sem);
=======

	if (fault & VM_FAULT_RETRY) {
		flags |= FAULT_FLAG_TRIED;

		/*
		 * No need to mmap_read_unlock(mm) as we would
		 * have already released it in __lock_page_or_retry
		 * in mm/filemap.c.
		 */

		goto retry;
	}

	mmap_read_unlock(mm);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	return 0;

/*
 * We ran out of memory, or some other thing happened to us that made
 * us unable to handle the page fault gracefully.
 */
out_of_memory:
<<<<<<< HEAD
	up_read(&mm->mmap_sem);
=======
	mmap_read_unlock(mm);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	if (!user_mode(regs))
		goto no_context;
	pagefault_out_of_memory();
	return 0;

no_context:
	current->thread.signo = SIGBUS;
	current->thread.faddr = address;
	return send_fault_sig(regs);

bus_err:
	current->thread.signo = SIGBUS;
	current->thread.code = BUS_ADRERR;
	current->thread.faddr = address;
	goto send_sig;

map_err:
<<<<<<< HEAD
	current->thread.signo = SIGSEGV;
	current->thread.code = SEGV_MAPERR;
	current->thread.faddr = address;
	goto send_sig;
=======
	mmap_read_unlock(mm);
map_err_nosemaphore:
	current->thread.signo = SIGSEGV;
	current->thread.code = SEGV_MAPERR;
	current->thread.faddr = address;
	return send_fault_sig(regs);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

acc_err:
	current->thread.signo = SIGSEGV;
	current->thread.code = SEGV_ACCERR;
	current->thread.faddr = address;

send_sig:
<<<<<<< HEAD
	up_read(&mm->mmap_sem);
=======
	mmap_read_unlock(mm);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	return send_fault_sig(regs);
}
