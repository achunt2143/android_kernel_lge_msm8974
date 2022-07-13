<<<<<<< HEAD
/*
 *  include/asm-s390/thread_info.h
 *
 *  S390 version
 *    Copyright (C) IBM Corp. 2002,2006
=======
/* SPDX-License-Identifier: GPL-2.0 */
/*
 *  S390 version
 *    Copyright IBM Corp. 2002, 2006
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 *    Author(s): Martin Schwidefsky (schwidefsky@de.ibm.com)
 */

#ifndef _ASM_THREAD_INFO_H
#define _ASM_THREAD_INFO_H

<<<<<<< HEAD
#ifdef __KERNEL__

/*
 * Size of kernel stack for each process
 */
#ifndef __s390x__
#define THREAD_ORDER 1
#define ASYNC_ORDER  1
#else /* __s390x__ */
#ifndef __SMALL_STACK
#define THREAD_ORDER 2
#define ASYNC_ORDER  2
#else
#define THREAD_ORDER 1
#define ASYNC_ORDER  1
#endif
#endif /* __s390x__ */

#define THREAD_SIZE (PAGE_SIZE << THREAD_ORDER)
#define ASYNC_SIZE  (PAGE_SIZE << ASYNC_ORDER)
=======
#include <linux/bits.h>
#ifndef ASM_OFFSETS_C
#include <asm/asm-offsets.h>
#endif

/*
 * General size of kernel stacks
 */
#ifdef CONFIG_KASAN
#define THREAD_SIZE_ORDER 4
#else
#define THREAD_SIZE_ORDER 2
#endif
#define BOOT_STACK_SIZE (PAGE_SIZE << 2)
#define THREAD_SIZE (PAGE_SIZE << THREAD_SIZE_ORDER)

#define STACK_INIT_OFFSET (THREAD_SIZE - STACK_FRAME_OVERHEAD - __PT_SIZE)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

#ifndef __ASSEMBLY__
#include <asm/lowcore.h>
#include <asm/page.h>
<<<<<<< HEAD
#include <asm/processor.h>
=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

/*
 * low level task data that entry.S needs immediate access to
 * - this struct should fit entirely inside of one cache line
 * - this struct shares the supervisor stack pages
 * - if the contents of this structure are changed, the assembly constants must also be changed
 */
struct thread_info {
<<<<<<< HEAD
	struct task_struct	*task;		/* main task structure */
	struct exec_domain	*exec_domain;	/* execution domain */
	unsigned long		flags;		/* low level flags */
	unsigned int		cpu;		/* current CPU */
	int			preempt_count;	/* 0 => preemptable, <0 => BUG */
	struct restart_block	restart_block;
	unsigned int		system_call;
	__u64			user_timer;
	__u64			system_timer;
	unsigned long		last_break;	/* last breaking-event-address. */
=======
	unsigned long		flags;		/* low level flags */
	unsigned long		syscall_work;	/* SYSCALL_WORK_ flags */
	unsigned int		cpu;		/* current CPU */
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
};

/*
 * macros/functions for gaining access to the thread information structure
 */
#define INIT_THREAD_INFO(tsk)			\
{						\
<<<<<<< HEAD
	.task		= &tsk,			\
	.exec_domain	= &default_exec_domain,	\
	.flags		= 0,			\
	.cpu		= 0,			\
	.preempt_count	= INIT_PREEMPT_COUNT,	\
	.restart_block	= {			\
		.fn = do_no_restart_syscall,	\
	},					\
}

#define init_thread_info	(init_thread_union.thread_info)
#define init_stack		(init_thread_union.stack)

/* how to get the thread information struct from C */
static inline struct thread_info *current_thread_info(void)
{
	return (struct thread_info *) S390_lowcore.thread_info;
}

#define THREAD_SIZE_ORDER THREAD_ORDER
=======
	.flags		= 0,			\
}

struct task_struct;

void arch_setup_new_exec(void);
#define arch_setup_new_exec arch_setup_new_exec
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

#endif

/*
 * thread information flags bit numbers
 */
<<<<<<< HEAD
#define TIF_SYSCALL		0	/* inside a system call */
#define TIF_NOTIFY_RESUME	1	/* callback before returning to user */
#define TIF_SIGPENDING		2	/* signal pending */
#define TIF_NEED_RESCHED	3	/* rescheduling necessary */
#define TIF_PER_TRAP		6	/* deliver sigtrap on return to user */
#define TIF_MCCK_PENDING	7	/* machine check handling is pending */
#define TIF_SYSCALL_TRACE	8	/* syscall trace active */
#define TIF_SYSCALL_AUDIT	9	/* syscall auditing active */
#define TIF_SECCOMP		10	/* secure computing */
#define TIF_SYSCALL_TRACEPOINT	11	/* syscall tracepoint instrumentation */
#define TIF_SIE			12	/* guest execution active */
#define TIF_POLLING_NRFLAG	16	/* true if poll_idle() is polling
					   TIF_NEED_RESCHED */
#define TIF_31BIT		17	/* 32bit process */
#define TIF_MEMDIE		18	/* is terminating due to OOM killer */
#define TIF_RESTORE_SIGMASK	19	/* restore signal mask in do_signal() */
#define TIF_SINGLE_STEP		20	/* This task is single stepped */

#define _TIF_SYSCALL		(1<<TIF_SYSCALL)
#define _TIF_NOTIFY_RESUME	(1<<TIF_NOTIFY_RESUME)
#define _TIF_RESTORE_SIGMASK	(1<<TIF_RESTORE_SIGMASK)
#define _TIF_SIGPENDING		(1<<TIF_SIGPENDING)
#define _TIF_NEED_RESCHED	(1<<TIF_NEED_RESCHED)
#define _TIF_PER_TRAP		(1<<TIF_PER_TRAP)
#define _TIF_MCCK_PENDING	(1<<TIF_MCCK_PENDING)
#define _TIF_SYSCALL_TRACE	(1<<TIF_SYSCALL_TRACE)
#define _TIF_SYSCALL_AUDIT	(1<<TIF_SYSCALL_AUDIT)
#define _TIF_SECCOMP		(1<<TIF_SECCOMP)
#define _TIF_SYSCALL_TRACEPOINT	(1<<TIF_SYSCALL_TRACEPOINT)
#define _TIF_SIE		(1<<TIF_SIE)
#define _TIF_POLLING_NRFLAG	(1<<TIF_POLLING_NRFLAG)
#define _TIF_31BIT		(1<<TIF_31BIT)
#define _TIF_SINGLE_STEP	(1<<TIF_SINGLE_STEP)

#ifdef CONFIG_64BIT
#define is_32bit_task()		(test_thread_flag(TIF_31BIT))
#else
#define is_32bit_task()		(1)
#endif

#endif /* __KERNEL__ */

#define PREEMPT_ACTIVE		0x4000000
=======
/* _TIF_WORK bits */
#define TIF_NOTIFY_RESUME	0	/* callback before returning to user */
#define TIF_SIGPENDING		1	/* signal pending */
#define TIF_NEED_RESCHED	2	/* rescheduling necessary */
#define TIF_UPROBE		3	/* breakpointed or single-stepping */
#define TIF_GUARDED_STORAGE	4	/* load guarded storage control block */
#define TIF_PATCH_PENDING	5	/* pending live patching update */
#define TIF_PGSTE		6	/* New mm's will use 4K page tables */
#define TIF_NOTIFY_SIGNAL	7	/* signal notifications exist */
#define TIF_ISOLATE_BP_GUEST	9	/* Run KVM guests with isolated BP */
#define TIF_PER_TRAP		10	/* Need to handle PER trap on exit to usermode */

#define TIF_31BIT		16	/* 32bit process */
#define TIF_MEMDIE		17	/* is terminating due to OOM killer */
#define TIF_RESTORE_SIGMASK	18	/* restore signal mask in do_signal() */
#define TIF_SINGLE_STEP		19	/* This task is single stepped */
#define TIF_BLOCK_STEP		20	/* This task is block stepped */
#define TIF_UPROBE_SINGLESTEP	21	/* This task is uprobe single stepped */

/* _TIF_TRACE bits */
#define TIF_SYSCALL_TRACE	24	/* syscall trace active */
#define TIF_SYSCALL_AUDIT	25	/* syscall auditing active */
#define TIF_SECCOMP		26	/* secure computing */
#define TIF_SYSCALL_TRACEPOINT	27	/* syscall tracepoint instrumentation */

#define _TIF_NOTIFY_RESUME	BIT(TIF_NOTIFY_RESUME)
#define _TIF_NOTIFY_SIGNAL	BIT(TIF_NOTIFY_SIGNAL)
#define _TIF_SIGPENDING		BIT(TIF_SIGPENDING)
#define _TIF_NEED_RESCHED	BIT(TIF_NEED_RESCHED)
#define _TIF_UPROBE		BIT(TIF_UPROBE)
#define _TIF_GUARDED_STORAGE	BIT(TIF_GUARDED_STORAGE)
#define _TIF_PATCH_PENDING	BIT(TIF_PATCH_PENDING)
#define _TIF_ISOLATE_BP_GUEST	BIT(TIF_ISOLATE_BP_GUEST)
#define _TIF_PER_TRAP		BIT(TIF_PER_TRAP)

#define _TIF_31BIT		BIT(TIF_31BIT)
#define _TIF_SINGLE_STEP	BIT(TIF_SINGLE_STEP)

#define _TIF_SYSCALL_TRACE	BIT(TIF_SYSCALL_TRACE)
#define _TIF_SYSCALL_AUDIT	BIT(TIF_SYSCALL_AUDIT)
#define _TIF_SECCOMP		BIT(TIF_SECCOMP)
#define _TIF_SYSCALL_TRACEPOINT	BIT(TIF_SYSCALL_TRACEPOINT)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

#endif /* _ASM_THREAD_INFO_H */
