<<<<<<< HEAD
/*
 * Copyright (C) 2002 - 2007 Jeff Dike (jdike@{addtoit,linux.intel}.com)
 * Licensed under the GPL
 */

#include "linux/kernel.h"
#include "linux/ptrace.h"
#include "kern_util.h"
#include "sysdep/ptrace.h"
#include "sysdep/syscalls.h"

extern int syscall_table_size;
#define NR_SYSCALLS (syscall_table_size / sizeof(void *))
=======
// SPDX-License-Identifier: GPL-2.0
/*
 * Copyright (C) 2002 - 2007 Jeff Dike (jdike@{addtoit,linux.intel}.com)
 */

#include <linux/kernel.h>
#include <linux/ptrace.h>
#include <linux/seccomp.h>
#include <kern_util.h>
#include <sysdep/ptrace.h>
#include <sysdep/ptrace_user.h>
#include <sysdep/syscalls.h>
#include <linux/time-internal.h>
#include <asm/unistd.h>
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

void handle_syscall(struct uml_pt_regs *r)
{
	struct pt_regs *regs = container_of(r, struct pt_regs, regs);
<<<<<<< HEAD
	long result;
	int syscall;

	syscall_trace(r, 0);

	/*
	 * This should go in the declaration of syscall, but when I do that,
	 * strace -f -c bash -c 'ls ; ls' breaks, sometimes not tracing
	 * children at all, sometimes hanging when bash doesn't see the first
	 * ls exit.
	 * The assembly looks functionally the same to me.  This is
	 *     gcc version 4.0.1 20050727 (Red Hat 4.0.1-5)
	 * in case it's a compiler bug.
	 */
	syscall = UPT_SYSCALL_NR(r);
	if ((syscall >= NR_SYSCALLS) || (syscall < 0))
		result = -ENOSYS;
	else result = EXECUTE_SYSCALL(syscall, regs);

	REGS_SET_SYSCALL_RETURN(r->gp, result);

	syscall_trace(r, 1);
=======
	int syscall;

	/*
	 * If we have infinite CPU resources, then make every syscall also a
	 * preemption point, since we don't have any other preemption in this
	 * case, and kernel threads would basically never run until userspace
	 * went to sleep, even if said userspace interacts with the kernel in
	 * various ways.
	 */
	if (time_travel_mode == TT_MODE_INFCPU ||
	    time_travel_mode == TT_MODE_EXTERNAL)
		schedule();

	/* Initialize the syscall number and default return value. */
	UPT_SYSCALL_NR(r) = PT_SYSCALL_NR(r->gp);
	PT_REGS_SET_SYSCALL_RETURN(regs, -ENOSYS);

	if (syscall_trace_enter(regs))
		goto out;

	/* Do the seccomp check after ptrace; failures should be fast. */
	if (secure_computing() == -1)
		goto out;

	syscall = UPT_SYSCALL_NR(r);
	if (syscall >= 0 && syscall < __NR_syscalls)
		PT_REGS_SET_SYSCALL_RETURN(regs,
				EXECUTE_SYSCALL(syscall, regs));

out:
	syscall_trace_leave(regs);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}
