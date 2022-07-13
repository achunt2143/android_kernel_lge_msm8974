/*
 * Copyright (C) 2003 - 2007 Jeff Dike (jdike@{addtoit,linux.intel}.com)
 * Copyright 2003 PathScale, Inc.
 *
 * Licensed under the GPL
 */

<<<<<<< HEAD
#include "linux/linkage.h"
#include "linux/personality.h"
#include "linux/utsname.h"
#include "asm/prctl.h" /* XXX This should get the constants from libc */
#include "asm/uaccess.h"
#include "os.h"

long arch_prctl(struct task_struct *task, int code, unsigned long __user *addr)
{
	unsigned long *ptr = addr, tmp;
	long ret;
	int pid = task->mm->context.id.u.pid;

	/*
	 * With ARCH_SET_FS (and ARCH_SET_GS is treated similarly to
	 * be safe), we need to call arch_prctl on the host because
	 * setting %fs may result in something else happening (like a
	 * GDT or thread.fs being set instead).  So, we let the host
	 * fiddle the registers and thread struct and restore the
	 * registers afterwards.
	 *
	 * So, the saved registers are stored to the process (this
	 * needed because a stub may have been the last thing to run),
	 * arch_prctl is run on the host, then the registers are read
	 * back.
	 */
	switch (code) {
	case ARCH_SET_FS:
	case ARCH_SET_GS:
		ret = restore_registers(pid, &current->thread.regs.regs);
		if (ret)
			return ret;
		break;
	case ARCH_GET_FS:
	case ARCH_GET_GS:
		/*
		 * With these two, we read to a local pointer and
		 * put_user it to the userspace pointer that we were
		 * given.  If addr isn't valid (because it hasn't been
		 * faulted in or is just bogus), we want put_user to
		 * fault it in (or return -EFAULT) instead of having
		 * the host return -EFAULT.
		 */
		ptr = &tmp;
	}

	ret = os_arch_prctl(pid, code, ptr);
	if (ret)
		return ret;

	switch (code) {
	case ARCH_SET_FS:
		current->thread.arch.fs = (unsigned long) ptr;
		ret = save_registers(pid, &current->thread.regs.regs);
		break;
	case ARCH_SET_GS:
		ret = save_registers(pid, &current->thread.regs.regs);
		break;
	case ARCH_GET_FS:
		ret = put_user(tmp, addr);
		break;
	case ARCH_GET_GS:
		ret = put_user(tmp, addr);
=======
#include <linux/sched.h>
#include <linux/sched/mm.h>
#include <linux/syscalls.h>
#include <linux/uaccess.h>
#include <asm/prctl.h> /* XXX This should get the constants from libc */
#include <registers.h>
#include <os.h>

long arch_prctl(struct task_struct *task, int option,
		unsigned long __user *arg2)
{
	long ret = -EINVAL;

	switch (option) {
	case ARCH_SET_FS:
		current->thread.regs.regs.gp[FS_BASE / sizeof(unsigned long)] =
			(unsigned long) arg2;
		ret = 0;
		break;
	case ARCH_SET_GS:
		current->thread.regs.regs.gp[GS_BASE / sizeof(unsigned long)] =
			(unsigned long) arg2;
		ret = 0;
		break;
	case ARCH_GET_FS:
		ret = put_user(current->thread.regs.regs.gp[FS_BASE / sizeof(unsigned long)], arg2);
		break;
	case ARCH_GET_GS:
		ret = put_user(current->thread.regs.regs.gp[GS_BASE / sizeof(unsigned long)], arg2);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		break;
	}

	return ret;
}

<<<<<<< HEAD
long sys_arch_prctl(int code, unsigned long addr)
{
	return arch_prctl(current, code, (unsigned long __user *) addr);
}

long sys_clone(unsigned long clone_flags, unsigned long newsp,
	       void __user *parent_tid, void __user *child_tid)
{
	long ret;

	if (!newsp)
		newsp = UPT_SP(&current->thread.regs.regs);
	current->thread.forking = 1;
	ret = do_fork(clone_flags, newsp, &current->thread.regs, 0, parent_tid,
		      child_tid);
	current->thread.forking = 0;
	return ret;
=======
SYSCALL_DEFINE2(arch_prctl, int, option, unsigned long, arg2)
{
	return arch_prctl(current, option, (unsigned long __user *) arg2);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

void arch_switch_to(struct task_struct *to)
{
<<<<<<< HEAD
	if ((to->thread.arch.fs == 0) || (to->mm == NULL))
		return;

	arch_prctl(to, ARCH_SET_FS, (void __user *) to->thread.arch.fs);
=======
	/*
	 * Nothing needs to be done on x86_64.
	 * The FS_BASE/GS_BASE registers are saved in the ptrace register set.
	 */
}

SYSCALL_DEFINE6(mmap, unsigned long, addr, unsigned long, len,
		unsigned long, prot, unsigned long, flags,
		unsigned long, fd, unsigned long, off)
{
	if (off & ~PAGE_MASK)
		return -EINVAL;

	return ksys_mmap_pgoff(addr, len, prot, flags, fd, off >> PAGE_SHIFT);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}
