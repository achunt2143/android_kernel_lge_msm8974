<<<<<<< HEAD
/*
 * Copyright (C) 2002 - 2007 Jeff Dike (jdike@{addtoit,linux.intel}.com)
 * Licensed under the GPL
=======
/* SPDX-License-Identifier: GPL-2.0 */
/*
 * Copyright (C) 2002 - 2007 Jeff Dike (jdike@{addtoit,linux.intel}.com)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 */

#ifndef __UM_THREAD_INFO_H
#define __UM_THREAD_INFO_H

<<<<<<< HEAD
=======
#define THREAD_SIZE_ORDER CONFIG_KERNEL_STACK_ORDER
#define THREAD_SIZE ((1 << CONFIG_KERNEL_STACK_ORDER) * PAGE_SIZE)

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#ifndef __ASSEMBLY__

#include <asm/types.h>
#include <asm/page.h>
<<<<<<< HEAD
#include <asm/uaccess.h>

struct thread_info {
	struct task_struct	*task;		/* main task structure */
	struct exec_domain	*exec_domain;	/* execution domain */
=======
#include <asm/segment.h>
#include <sysdep/ptrace_user.h>

struct thread_info {
	struct task_struct	*task;		/* main task structure */
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	unsigned long		flags;		/* low level flags */
	__u32			cpu;		/* current CPU */
	int			preempt_count;  /* 0 => preemptable,
						   <0 => BUG */
<<<<<<< HEAD
	mm_segment_t		addr_limit;	/* thread address space:
					 	   0-0xBFFFFFFF for user
						   0-0xFFFFFFFF for kernel */
	struct restart_block    restart_block;
	struct thread_info	*real_thread;    /* Points to non-IRQ stack */
=======
	struct thread_info	*real_thread;    /* Points to non-IRQ stack */
	unsigned long aux_fp_regs[FP_SIZE];	/* auxiliary fp_regs to save/restore
						   them out-of-band */
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
};

#define INIT_THREAD_INFO(tsk)			\
{						\
	.task =		&tsk,			\
<<<<<<< HEAD
	.exec_domain =	&default_exec_domain,	\
	.flags =		0,		\
	.cpu =		0,			\
	.preempt_count = INIT_PREEMPT_COUNT,	\
	.addr_limit =	KERNEL_DS,		\
	.restart_block =  {			\
		.fn =  do_no_restart_syscall,	\
	},					\
	.real_thread = NULL,			\
}

#define init_thread_info	(init_thread_union.thread_info)
#define init_stack		(init_thread_union.stack)

#define THREAD_SIZE ((1 << CONFIG_KERNEL_STACK_ORDER) * PAGE_SIZE)
=======
	.flags =		0,		\
	.cpu =		0,			\
	.preempt_count = INIT_PREEMPT_COUNT,	\
	.real_thread = NULL,			\
}

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
/* how to get the thread information struct from C */
static inline struct thread_info *current_thread_info(void)
{
	struct thread_info *ti;
	unsigned long mask = THREAD_SIZE - 1;
	void *p;

	asm volatile ("" : "=r" (p) : "0" (&ti));
	ti = (struct thread_info *) (((unsigned long)p) & ~mask);
	return ti;
}

<<<<<<< HEAD
#define THREAD_SIZE_ORDER CONFIG_KERNEL_STACK_ORDER

#endif

#define PREEMPT_ACTIVE		0x10000000

#define TIF_SYSCALL_TRACE	0	/* syscall trace active */
#define TIF_SIGPENDING		1	/* signal pending */
#define TIF_NEED_RESCHED	2	/* rescheduling necessary */
#define TIF_POLLING_NRFLAG      3       /* true if poll_idle() is polling
					 * TIF_NEED_RESCHED */
=======
#endif

#define TIF_SYSCALL_TRACE	0	/* syscall trace active */
#define TIF_SIGPENDING		1	/* signal pending */
#define TIF_NEED_RESCHED	2	/* rescheduling necessary */
#define TIF_NOTIFY_SIGNAL	3	/* signal notifications exist */
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#define TIF_RESTART_BLOCK	4
#define TIF_MEMDIE		5	/* is terminating due to OOM killer */
#define TIF_SYSCALL_AUDIT	6
#define TIF_RESTORE_SIGMASK	7
<<<<<<< HEAD
=======
#define TIF_NOTIFY_RESUME	8
#define TIF_SECCOMP		9	/* secure computing */
#define TIF_SINGLESTEP		10	/* single stepping userspace */
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

#define _TIF_SYSCALL_TRACE	(1 << TIF_SYSCALL_TRACE)
#define _TIF_SIGPENDING		(1 << TIF_SIGPENDING)
#define _TIF_NEED_RESCHED	(1 << TIF_NEED_RESCHED)
<<<<<<< HEAD
#define _TIF_POLLING_NRFLAG     (1 << TIF_POLLING_NRFLAG)
#define _TIF_MEMDIE		(1 << TIF_MEMDIE)
#define _TIF_SYSCALL_AUDIT	(1 << TIF_SYSCALL_AUDIT)
#define _TIF_RESTORE_SIGMASK	(1 << TIF_RESTORE_SIGMASK)
=======
#define _TIF_NOTIFY_SIGNAL	(1 << TIF_NOTIFY_SIGNAL)
#define _TIF_MEMDIE		(1 << TIF_MEMDIE)
#define _TIF_SYSCALL_AUDIT	(1 << TIF_SYSCALL_AUDIT)
#define _TIF_SECCOMP		(1 << TIF_SECCOMP)
#define _TIF_SINGLESTEP		(1 << TIF_SINGLESTEP)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

#endif
