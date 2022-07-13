/*
 * include/asm-xtensa/thread_info.h
 *
 * This file is subject to the terms and conditions of the GNU General Public
 * License.  See the file "COPYING" in the main directory of this archive
 * for more details.
 *
 * Copyright (C) 2001 - 2005 Tensilica Inc.
 */

#ifndef _XTENSA_THREAD_INFO_H
#define _XTENSA_THREAD_INFO_H

<<<<<<< HEAD
#ifdef __KERNEL__
=======
#include <linux/stringify.h>
#include <asm/kmem_layout.h>

#define CURRENT_SHIFT KERNEL_STACK_SHIFT
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

#ifndef __ASSEMBLY__
# include <asm/processor.h>
#endif

/*
 * low level task data that entry.S needs immediate access to
 * - this struct should fit entirely inside of one cache line
 * - this struct shares the supervisor stack pages
 * - if the contents of this structure are changed, the assembly constants
 *   must also be changed
 */

#ifndef __ASSEMBLY__

#if XTENSA_HAVE_COPROCESSORS

typedef struct xtregs_coprocessor {
	xtregs_cp0_t cp0;
	xtregs_cp1_t cp1;
	xtregs_cp2_t cp2;
	xtregs_cp3_t cp3;
	xtregs_cp4_t cp4;
	xtregs_cp5_t cp5;
	xtregs_cp6_t cp6;
	xtregs_cp7_t cp7;
} xtregs_coprocessor_t;

#endif

struct thread_info {
	struct task_struct	*task;		/* main task structure */
<<<<<<< HEAD
	struct exec_domain	*exec_domain;	/* execution domain */
=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	unsigned long		flags;		/* low level flags */
	unsigned long		status;		/* thread-synchronous flags */
	__u32			cpu;		/* current CPU */
	__s32			preempt_count;	/* 0 => preemptable,< 0 => BUG*/

<<<<<<< HEAD
	mm_segment_t		addr_limit;	/* thread address space */
	struct restart_block    restart_block;

	unsigned long		cpenable;

=======
#if XCHAL_HAVE_EXCLUSIVE
	/* result of the most recent exclusive store */
	unsigned long		atomctl8;
#endif
#ifdef CONFIG_USER_ABI_CALL0_PROBE
	/* Address where PS.WOE was enabled by the ABI probing code */
	unsigned long		ps_woe_fix_addr;
#endif

	/*
	 * If i-th bit is set then coprocessor state is loaded into the
	 * coprocessor i on CPU cp_owner_cpu.
	 */
	unsigned long		cpenable;
	u32			cp_owner_cpu;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	/* Allocate storage for extra user states and coprocessor states. */
#if XTENSA_HAVE_COPROCESSORS
	xtregs_coprocessor_t	xtregs_cp;
#endif
	xtregs_user_t		xtregs_user;
};

<<<<<<< HEAD
#else /* !__ASSEMBLY__ */

/* offsets into the thread_info struct for assembly code access */
#define TI_TASK		 0x00000000
#define TI_EXEC_DOMAIN	 0x00000004
#define TI_FLAGS	 0x00000008
#define TI_STATUS	 0x0000000C
#define TI_CPU		 0x00000010
#define TI_PRE_COUNT	 0x00000014
#define TI_ADDR_LIMIT	 0x00000018
#define TI_RESTART_BLOCK 0x000001C

#endif

#define PREEMPT_ACTIVE		0x10000000

=======
#endif

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
/*
 * macros/functions for gaining access to the thread information structure
 */

#ifndef __ASSEMBLY__

#define INIT_THREAD_INFO(tsk)			\
{						\
	.task		= &tsk,			\
<<<<<<< HEAD
	.exec_domain	= &default_exec_domain,	\
	.flags		= 0,			\
	.cpu		= 0,			\
	.preempt_count	= INIT_PREEMPT_COUNT,	\
	.addr_limit	= KERNEL_DS,		\
	.restart_block = {			\
		.fn = do_no_restart_syscall,	\
	},					\
}

#define init_thread_info	(init_thread_union.thread_info)
#define init_stack		(init_thread_union.stack)

=======
	.flags		= 0,			\
	.cpu		= 0,			\
	.preempt_count	= INIT_PREEMPT_COUNT,	\
}

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
/* how to get the thread information struct from C */
static inline struct thread_info *current_thread_info(void)
{
	struct thread_info *ti;
<<<<<<< HEAD
	 __asm__("extui %0,a1,0,13\n\t"
=======
	 __asm__("extui %0, a1, 0, "__stringify(CURRENT_SHIFT)"\n\t"
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	         "xor %0, a1, %0" : "=&r" (ti) : );
	return ti;
}

#else /* !__ASSEMBLY__ */

/* how to get the thread information struct from ASM */
#define GET_THREAD_INFO(reg,sp) \
<<<<<<< HEAD
	extui reg, sp, 0, 13; \
=======
	extui reg, sp, 0, CURRENT_SHIFT; \
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	xor   reg, sp, reg
#endif


/*
 * thread information flags
 * - these are process state flags that various assembly files may need to access
<<<<<<< HEAD
 * - pending work-to-be-done flags are in LSW
 * - other flags in MSW
=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 */
#define TIF_SYSCALL_TRACE	0	/* syscall trace active */
#define TIF_SIGPENDING		1	/* signal pending */
#define TIF_NEED_RESCHED	2	/* rescheduling necessary */
#define TIF_SINGLESTEP		3	/* restore singlestep on return to user mode */
<<<<<<< HEAD
#define TIF_IRET		4	/* return with iret */
#define TIF_MEMDIE		5	/* is terminating due to OOM killer */
#define TIF_RESTORE_SIGMASK	6	/* restore signal mask in do_signal() */
#define TIF_POLLING_NRFLAG	16	/* true if poll_idle() is polling TIF_NEED_RESCHED */
=======
#define TIF_SYSCALL_TRACEPOINT	4	/* syscall tracepoint instrumentation */
#define TIF_NOTIFY_SIGNAL	5	/* signal notifications exist */
#define TIF_RESTORE_SIGMASK	6	/* restore signal mask in do_signal() */
#define TIF_NOTIFY_RESUME	7	/* callback before returning to user */
#define TIF_DB_DISABLED		8	/* debug trap disabled for syscall */
#define TIF_SYSCALL_AUDIT	9	/* syscall auditing active */
#define TIF_SECCOMP		10	/* secure computing */
#define TIF_MEMDIE		11	/* is terminating due to OOM killer */
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

#define _TIF_SYSCALL_TRACE	(1<<TIF_SYSCALL_TRACE)
#define _TIF_SIGPENDING		(1<<TIF_SIGPENDING)
#define _TIF_NEED_RESCHED	(1<<TIF_NEED_RESCHED)
#define _TIF_SINGLESTEP		(1<<TIF_SINGLESTEP)
<<<<<<< HEAD
#define _TIF_IRET		(1<<TIF_IRET)
#define _TIF_POLLING_NRFLAG	(1<<TIF_POLLING_NRFLAG)
#define _TIF_RESTORE_SIGMASK	(1<<TIF_RESTORE_SIGMASK)

#define _TIF_WORK_MASK		0x0000FFFE	/* work to do on interrupt/exception return */
#define _TIF_ALLWORK_MASK	0x0000FFFF	/* work to do on any return to u-space */

/*
 * Thread-synchronous status.
 *
 * This is different from the flags in that nobody else
 * ever touches our thread-synchronous status, so we don't
 * have to worry about atomic accesses.
 */
#define TS_USEDFPU		0x0001	/* FPU was used by this task this quantum (SMP) */

#define THREAD_SIZE 8192	//(2*PAGE_SIZE)
#define THREAD_SIZE_ORDER 1

#endif	/* __KERNEL__ */
=======
#define _TIF_SYSCALL_TRACEPOINT	(1<<TIF_SYSCALL_TRACEPOINT)
#define _TIF_NOTIFY_SIGNAL	(1<<TIF_NOTIFY_SIGNAL)
#define _TIF_NOTIFY_RESUME	(1<<TIF_NOTIFY_RESUME)
#define _TIF_SYSCALL_AUDIT	(1<<TIF_SYSCALL_AUDIT)
#define _TIF_SECCOMP		(1<<TIF_SECCOMP)

#define _TIF_WORK_MASK		(_TIF_SYSCALL_TRACE | _TIF_SINGLESTEP | \
				 _TIF_SYSCALL_TRACEPOINT | \
				 _TIF_SYSCALL_AUDIT | _TIF_SECCOMP)

#define THREAD_SIZE KERNEL_STACK_SIZE
#define THREAD_SIZE_ORDER (KERNEL_STACK_SHIFT - PAGE_SHIFT)

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#endif	/* _XTENSA_THREAD_INFO */
