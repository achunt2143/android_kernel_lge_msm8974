<<<<<<< HEAD
=======
/* SPDX-License-Identifier: GPL-2.0-only */
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
/*
 *  linux/arch/arm/kernel/entry-common.S
 *
 *  Copyright (C) 2000 Russell King
<<<<<<< HEAD
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 */

#include <asm/unistd.h>
#include <asm/ftrace.h>
#include <asm/unwind.h>

#ifdef CONFIG_NEED_RET_TO_USER
#include <mach/entry-macro.S>
#else
	.macro  arch_ret_to_user, tmp1, tmp2
	.endm
#endif

#include "entry-header.S"


	.align	5
/*
 * This is the fast syscall return path.  We do as little as
 * possible here, and this includes saving r0 back into the SVC
 * stack.
 */
ret_fast_syscall:
 UNWIND(.fnstart	)
 UNWIND(.cantunwind	)
	disable_irq				@ disable interrupts
	ldr	r1, [tsk, #TI_FLAGS]		@ re-check for syscall tracing
	tst	r1, #_TIF_SYSCALL_WORK
	bne	__sys_trace_return
	tst	r1, #_TIF_WORK_MASK
	bne	fast_work_pending
#if defined(CONFIG_IRQSOFF_TRACER)
	asm_trace_hardirqs_on
#endif

	/* perform architecture specific actions before user return */
	arch_ret_to_user r1, lr

	restore_user_regs fast = 1, offset = S_OFF
 UNWIND(.fnend		)

/*
 * Ok, we need to do extra processing, enter the slow path.
 */
fast_work_pending:
	str	r0, [sp, #S_R0+S_OFF]!		@ returned r0
work_pending:
	tst	r1, #_TIF_NEED_RESCHED
	bne	work_resched
	tst	r1, #_TIF_SIGPENDING|_TIF_NOTIFY_RESUME
	beq	no_work_pending
	mov	r0, sp				@ 'regs'
	mov	r2, why				@ 'syscall'
	tst	r1, #_TIF_SIGPENDING		@ delivering a signal?
	movne	why, #0				@ prevent further restarts
	bl	do_notify_resume
	b	ret_slow_syscall		@ Check work again

work_resched:
	bl	schedule
/*
 * "slow" syscall return path.  "why" tells us if this was a real syscall.
 */
ENTRY(ret_to_user)
ret_slow_syscall:
	disable_irq				@ disable interrupts
ENTRY(ret_to_user_from_irq)
	ldr	r1, [tsk, #TI_FLAGS]
	tst	r1, #_TIF_WORK_MASK
	bne	work_pending
no_work_pending:
#if defined(CONFIG_IRQSOFF_TRACER)
	asm_trace_hardirqs_on
#endif
	/* perform architecture specific actions before user return */
	arch_ret_to_user r1, lr
=======
 */

#include <asm/assembler.h>
#include <asm/unistd.h>
#include <asm/ftrace.h>
#include <asm/unwind.h>
#include <asm/page.h>
#ifdef CONFIG_AEABI
#include <asm/unistd-oabi.h>
#endif

	.equ	NR_syscalls, __NR_syscalls

#include "entry-header.S"

saved_psr	.req	r8
#if defined(CONFIG_TRACE_IRQFLAGS) || defined(CONFIG_CONTEXT_TRACKING_USER)
saved_pc	.req	r9
#define TRACE(x...) x
#else
saved_pc	.req	lr
#define TRACE(x...)
#endif

	.section .entry.text,"ax",%progbits
	.align	5
#if !(IS_ENABLED(CONFIG_TRACE_IRQFLAGS) || IS_ENABLED(CONFIG_CONTEXT_TRACKING_USER) || \
	IS_ENABLED(CONFIG_DEBUG_RSEQ))
/*
 * This is the fast syscall return path.  We do as little as possible here,
 * such as avoiding writing r0 to the stack.  We only use this path if we
 * have tracing, context tracking and rseq debug disabled - the overheads
 * from those features make this path too inefficient.
 */
ret_fast_syscall:
__ret_fast_syscall:
 UNWIND(.fnstart	)
 UNWIND(.cantunwind	)
	disable_irq_notrace			@ disable interrupts
	ldr	r1, [tsk, #TI_FLAGS]		@ re-check for syscall tracing
	movs	r1, r1, lsl #16
	bne	fast_work_pending

	restore_user_regs fast = 1, offset = S_OFF
 UNWIND(.fnend		)
ENDPROC(ret_fast_syscall)

	/* Ok, we need to do extra processing, enter the slow path. */
fast_work_pending:
	str	r0, [sp, #S_R0+S_OFF]!		@ returned r0
	/* fall through to work_pending */
#else
/*
 * The "replacement" ret_fast_syscall for when tracing, context tracking,
 * or rseq debug is enabled.  As we will need to call out to some C functions,
 * we save r0 first to avoid needing to save registers around each C function
 * call.
 */
ret_fast_syscall:
__ret_fast_syscall:
 UNWIND(.fnstart	)
 UNWIND(.cantunwind	)
	str	r0, [sp, #S_R0 + S_OFF]!	@ save returned r0
#if IS_ENABLED(CONFIG_DEBUG_RSEQ)
	/* do_rseq_syscall needs interrupts enabled. */
	mov	r0, sp				@ 'regs'
	bl	do_rseq_syscall
#endif
	disable_irq_notrace			@ disable interrupts
	ldr	r1, [tsk, #TI_FLAGS]		@ re-check for syscall tracing
	movs	r1, r1, lsl #16
	beq	no_work_pending
 UNWIND(.fnend		)
ENDPROC(ret_fast_syscall)

	/* Slower path - fall through to work_pending */
#endif

	tst	r1, #_TIF_SYSCALL_WORK
	bne	__sys_trace_return_nosave
slow_work_pending:
	mov	r0, sp				@ 'regs'
	mov	r2, why				@ 'syscall'
	bl	do_work_pending
	cmp	r0, #0
	beq	no_work_pending
	movlt	scno, #(__NR_restart_syscall - __NR_SYSCALL_BASE)
	str	scno, [tsk, #TI_ABI_SYSCALL]	@ make sure tracers see update
	ldmia	sp, {r0 - r6}			@ have to reload r0 - r6
	b	local_restart			@ ... and off we go
ENDPROC(ret_fast_syscall)

/*
 * "slow" syscall return path.  "why" tells us if this was a real syscall.
 * IRQs may be enabled here, so always disable them.  Note that we use the
 * "notrace" version to avoid calling into the tracing code unnecessarily.
 * do_work_pending() will update this state if necessary.
 */
ENTRY(ret_to_user)
ret_slow_syscall:
#if IS_ENABLED(CONFIG_DEBUG_RSEQ)
	/* do_rseq_syscall needs interrupts enabled. */
	enable_irq_notrace			@ enable interrupts
	mov	r0, sp				@ 'regs'
	bl	do_rseq_syscall
#endif
	disable_irq_notrace			@ disable interrupts
ENTRY(ret_to_user_from_irq)
	ldr	r1, [tsk, #TI_FLAGS]
	movs	r1, r1, lsl #16
	bne	slow_work_pending
no_work_pending:
	asm_trace_hardirqs_on save = 0

	ct_user_enter save = 0
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	restore_user_regs fast = 0, offset = 0
ENDPROC(ret_to_user_from_irq)
ENDPROC(ret_to_user)

/*
 * This is how we return from a fork.
 */
ENTRY(ret_from_fork)
	bl	schedule_tail
<<<<<<< HEAD
	get_thread_info tsk
	ldr	r1, [tsk, #TI_FLAGS]		@ check for syscall tracing
	mov	why, #1
	tst	r1, #_TIF_SYSCALL_WORK		@ are we tracing syscalls?
	beq	ret_slow_syscall
	mov	r1, sp
	mov	r0, #1				@ trace exit [IP = 1]
	bl	syscall_trace
	b	ret_slow_syscall
ENDPROC(ret_from_fork)

	.equ NR_syscalls,0
#define CALL(x) .equ NR_syscalls,NR_syscalls+1
#include "calls.S"
#undef CALL
#define CALL(x) .long x

#ifdef CONFIG_FUNCTION_TRACER
/*
 * When compiling with -pg, gcc inserts a call to the mcount routine at the
 * start of every function.  In mcount, apart from the function's address (in
 * lr), we need to get hold of the function's caller's address.
 *
 * Older GCCs (pre-4.4) inserted a call to a routine called mcount like this:
 *
 *	bl	mcount
 *
 * These versions have the limitation that in order for the mcount routine to
 * be able to determine the function's caller's address, an APCS-style frame
 * pointer (which is set up with something like the code below) is required.
 *
 *	mov     ip, sp
 *	push    {fp, ip, lr, pc}
 *	sub     fp, ip, #4
 *
 * With EABI, these frame pointers are not available unless -mapcs-frame is
 * specified, and if building as Thumb-2, not even then.
 *
 * Newer GCCs (4.4+) solve this problem by introducing a new version of mcount,
 * with call sites like:
 *
 *	push	{lr}
 *	bl	__gnu_mcount_nc
 *
 * With these compilers, frame pointers are not necessary.
 *
 * mcount can be thought of as a function called in the middle of a subroutine
 * call.  As such, it needs to be transparent for both the caller and the
 * callee: the original lr needs to be restored when leaving mcount, and no
 * registers should be clobbered.  (In the __gnu_mcount_nc implementation, we
 * clobber the ip register.  This is OK because the ARM calling convention
 * allows it to be clobbered in subroutines and doesn't use it to hold
 * parameters.)
 *
 * When using dynamic ftrace, we patch out the mcount call by a "mov r0, r0"
 * for the mcount case, and a "pop {lr}" for the __gnu_mcount_nc case (see
 * arch/arm/kernel/ftrace.c).
 */

#ifndef CONFIG_OLD_MCOUNT
#if (__GNUC__ < 4 || (__GNUC__ == 4 && __GNUC_MINOR__ < 4))
#error Ftrace requires CONFIG_FRAME_POINTER=y with GCC older than 4.4.0.
#endif
#endif

.macro mcount_adjust_addr rd, rn
	bic	\rd, \rn, #1		@ clear the Thumb bit if present
	sub	\rd, \rd, #MCOUNT_INSN_SIZE
.endm

.macro __mcount suffix
	mcount_enter
	ldr	r0, =ftrace_trace_function
	ldr	r2, [r0]
	adr	r0, .Lftrace_stub
	cmp	r0, r2
	bne	1f

#ifdef CONFIG_FUNCTION_GRAPH_TRACER
	ldr     r1, =ftrace_graph_return
	ldr     r2, [r1]
	cmp     r0, r2
	bne     ftrace_graph_caller\suffix

	ldr     r1, =ftrace_graph_entry
	ldr     r2, [r1]
	ldr     r0, =ftrace_graph_entry_stub
	cmp     r0, r2
	bne     ftrace_graph_caller\suffix
#endif

	mcount_exit

1: 	mcount_get_lr	r1			@ lr of instrumented func
	mcount_adjust_addr	r0, lr		@ instrumented function
	adr	lr, BSYM(2f)
	mov	pc, r2
2:	mcount_exit
.endm

.macro __ftrace_caller suffix
	mcount_enter

	mcount_get_lr	r1			@ lr of instrumented func
	mcount_adjust_addr	r0, lr		@ instrumented function

	.globl ftrace_call\suffix
ftrace_call\suffix:
	bl	ftrace_stub

#ifdef CONFIG_FUNCTION_GRAPH_TRACER
	.globl ftrace_graph_call\suffix
ftrace_graph_call\suffix:
	mov	r0, r0
#endif

	mcount_exit
.endm

.macro __ftrace_graph_caller
	sub	r0, fp, #4		@ &lr of instrumented routine (&parent)
#ifdef CONFIG_DYNAMIC_FTRACE
	@ called from __ftrace_caller, saved in mcount_enter
	ldr	r1, [sp, #16]		@ instrumented routine (func)
	mcount_adjust_addr	r1, r1
#else
	@ called from __mcount, untouched in lr
	mcount_adjust_addr	r1, lr	@ instrumented routine (func)
#endif
	mov	r2, fp			@ frame pointer
	bl	prepare_ftrace_return
	mcount_exit
.endm

#ifdef CONFIG_OLD_MCOUNT
/*
 * mcount
 */

.macro mcount_enter
	stmdb	sp!, {r0-r3, lr}
.endm

.macro mcount_get_lr reg
	ldr	\reg, [fp, #-4]
.endm

.macro mcount_exit
	ldr	lr, [fp, #-4]
	ldmia	sp!, {r0-r3, pc}
.endm

ENTRY(mcount)
#ifdef CONFIG_DYNAMIC_FTRACE
	stmdb	sp!, {lr}
	ldr	lr, [fp, #-4]
	ldmia	sp!, {pc}
#else
	__mcount _old
#endif
ENDPROC(mcount)

#ifdef CONFIG_DYNAMIC_FTRACE
ENTRY(ftrace_caller_old)
	__ftrace_caller _old
ENDPROC(ftrace_caller_old)
#endif

#ifdef CONFIG_FUNCTION_GRAPH_TRACER
ENTRY(ftrace_graph_caller_old)
	__ftrace_graph_caller
ENDPROC(ftrace_graph_caller_old)
#endif

.purgem mcount_enter
.purgem mcount_get_lr
.purgem mcount_exit
#endif

/*
 * __gnu_mcount_nc
 */

.macro mcount_enter
	stmdb	sp!, {r0-r3, lr}
.endm

.macro mcount_get_lr reg
	ldr	\reg, [sp, #20]
.endm

.macro mcount_exit
	ldmia	sp!, {r0-r3, ip, lr}
	mov	pc, ip
.endm

ENTRY(__gnu_mcount_nc)
#ifdef CONFIG_DYNAMIC_FTRACE
	mov	ip, lr
	ldmia	sp!, {lr}
	mov	pc, ip
#else
	__mcount
#endif
ENDPROC(__gnu_mcount_nc)

#ifdef CONFIG_DYNAMIC_FTRACE
ENTRY(ftrace_caller)
	__ftrace_caller
ENDPROC(ftrace_caller)
#endif

#ifdef CONFIG_FUNCTION_GRAPH_TRACER
ENTRY(ftrace_graph_caller)
	__ftrace_graph_caller
ENDPROC(ftrace_graph_caller)
#endif

.purgem mcount_enter
.purgem mcount_get_lr
.purgem mcount_exit

#ifdef CONFIG_FUNCTION_GRAPH_TRACER
	.globl return_to_handler
return_to_handler:
	stmdb	sp!, {r0-r3}
	mov	r0, fp			@ frame pointer
	bl	ftrace_return_to_handler
	mov	lr, r0			@ r0 has real ret addr
	ldmia	sp!, {r0-r3}
	mov	pc, lr
#endif

ENTRY(ftrace_stub)
.Lftrace_stub:
	mov	pc, lr
ENDPROC(ftrace_stub)

#endif /* CONFIG_FUNCTION_TRACER */

=======
	cmp	r5, #0
	movne	r0, r4
	badrne	lr, 1f
	retne	r5
1:	get_thread_info tsk
	b	ret_slow_syscall
ENDPROC(ret_from_fork)

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
/*=============================================================================
 * SWI handler
 *-----------------------------------------------------------------------------
 */

<<<<<<< HEAD
	/* If we're optimising for StrongARM the resulting code won't 
	   run on an ARM7 and we can save a couple of instructions.  
								--pb */
#ifdef CONFIG_CPU_ARM710
#define A710(code...) code
.Larm710bug:
	ldmia	sp, {r0 - lr}^			@ Get calling r0 - lr
	mov	r0, r0
	add	sp, sp, #S_FRAME_SIZE
	subs	pc, lr, #4
#else
#define A710(code...)
#endif

	.align	5
ENTRY(vector_swi)
	sub	sp, sp, #S_FRAME_SIZE
	stmia	sp, {r0 - r12}			@ Calling r0 - r12
=======
	.align	5
#ifdef CONFIG_HARDEN_BRANCH_HISTORY
ENTRY(vector_bhb_loop8_swi)
	sub	sp, sp, #PT_REGS_SIZE
	stmia	sp, {r0 - r12}
	mov	r8, #8
1:	b	2f
2:	subs	r8, r8, #1
	bne	1b
	dsb	nsh
	isb
	b	3f
ENDPROC(vector_bhb_loop8_swi)

	.align	5
ENTRY(vector_bhb_bpiall_swi)
	sub	sp, sp, #PT_REGS_SIZE
	stmia	sp, {r0 - r12}
	mcr	p15, 0, r8, c7, c5, 6	@ BPIALL
	isb
	b	3f
ENDPROC(vector_bhb_bpiall_swi)
#endif
	.align	5
ENTRY(vector_swi)
#ifdef CONFIG_CPU_V7M
	v7m_exception_entry
#else
	sub	sp, sp, #PT_REGS_SIZE
	stmia	sp, {r0 - r12}			@ Calling r0 - r12
3:
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 ARM(	add	r8, sp, #S_PC		)
 ARM(	stmdb	r8, {sp, lr}^		)	@ Calling sp, lr
 THUMB(	mov	r8, sp			)
 THUMB(	store_user_sp_lr r8, r10, S_SP	)	@ calling sp, lr
<<<<<<< HEAD
	mrs	r8, spsr			@ called from non-FIQ mode, so ok.
	str	lr, [sp, #S_PC]			@ Save calling PC
	str	r8, [sp, #S_PSR]		@ Save CPSR
	str	r0, [sp, #S_OLD_R0]		@ Save OLD_R0
	zero_fp
=======
	mrs	saved_psr, spsr			@ called from non-FIQ mode, so ok.
 TRACE(	mov	saved_pc, lr		)
	str	saved_pc, [sp, #S_PC]		@ Save calling PC
	str	saved_psr, [sp, #S_PSR]		@ Save CPSR
	str	r0, [sp, #S_OLD_R0]		@ Save OLD_R0
#endif
	reload_current r10, ip
	zero_fp
	alignment_trap r10, ip, cr_alignment
	asm_trace_hardirqs_on save=0
	enable_irq_notrace
	ct_user_exit save=0
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	/*
	 * Get the system call number.
	 */

#if defined(CONFIG_OABI_COMPAT)

	/*
	 * If we have CONFIG_OABI_COMPAT then we need to look at the swi
	 * value to determine if it is an EABI or an old ABI call.
	 */
#ifdef CONFIG_ARM_THUMB
<<<<<<< HEAD
	tst	r8, #PSR_T_BIT
	movne	r10, #0				@ no thumb OABI emulation
	ldreq	r10, [lr, #-4]			@ get SWI instruction
#else
	ldr	r10, [lr, #-4]			@ get SWI instruction
  A710(	and	ip, r10, #0x0f000000		@ check for SWI		)
  A710(	teq	ip, #0x0f000000						)
  A710(	bne	.Larm710bug						)
#endif
#ifdef CONFIG_CPU_ENDIAN_BE8
	rev	r10, r10			@ little endian instruction
#endif
=======
	tst	saved_psr, #PSR_T_BIT
	movne	r10, #0				@ no thumb OABI emulation
 USER(	ldreq	r10, [saved_pc, #-4]	)	@ get SWI instruction
#else
 USER(	ldr	r10, [saved_pc, #-4]	)	@ get SWI instruction
#endif
 ARM_BE8(rev	r10, r10)			@ little endian instruction
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

#elif defined(CONFIG_AEABI)

	/*
	 * Pure EABI user space always put syscall number into scno (r7).
	 */
<<<<<<< HEAD
  A710(	ldr	ip, [lr, #-4]			@ get SWI instruction	)
  A710(	and	ip, ip, #0x0f000000		@ check for SWI		)
  A710(	teq	ip, #0x0f000000						)
  A710(	bne	.Larm710bug						)

#elif defined(CONFIG_ARM_THUMB)

	/* Legacy ABI only, possibly thumb mode. */
	tst	r8, #PSR_T_BIT			@ this is SPSR from save_user_regs
	addne	scno, r7, #__NR_SYSCALL_BASE	@ put OS number in
	ldreq	scno, [lr, #-4]

#else

	/* Legacy ABI only. */
	ldr	scno, [lr, #-4]			@ get SWI instruction
  A710(	and	ip, scno, #0x0f000000		@ check for SWI		)
  A710(	teq	ip, #0x0f000000						)
  A710(	bne	.Larm710bug						)

#endif

#ifdef CONFIG_ALIGNMENT_TRAP
	ldr	ip, __cr_alignment
	ldr	ip, [ip]
	mcr	p15, 0, ip, c1, c0		@ update control register
#endif
	enable_irq

	get_thread_info tsk
=======
#elif defined(CONFIG_ARM_THUMB)
	/* Legacy ABI only, possibly thumb mode. */
	tst	saved_psr, #PSR_T_BIT		@ this is SPSR from save_user_regs
	addne	scno, r7, #__NR_SYSCALL_BASE	@ put OS number in
 USER(	ldreq	scno, [saved_pc, #-4]	)

#else
	/* Legacy ABI only. */
 USER(	ldr	scno, [saved_pc, #-4]	)	@ get SWI instruction
#endif

	/* saved_psr and saved_pc are now dead */

	uaccess_disable tbl
	get_thread_info tsk

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	adr	tbl, sys_call_table		@ load syscall table pointer

#if defined(CONFIG_OABI_COMPAT)
	/*
	 * If the swi argument is zero, this is an EABI call and we do nothing.
	 *
	 * If this is an old ABI call, get the syscall number into scno and
	 * get the old ABI syscall table address.
	 */
	bics	r10, r10, #0xff000000
<<<<<<< HEAD
=======
	strne	r10, [tsk, #TI_ABI_SYSCALL]
	streq	scno, [tsk, #TI_ABI_SYSCALL]
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	eorne	scno, r10, #__NR_OABI_SYSCALL_BASE
	ldrne	tbl, =sys_oabi_call_table
#elif !defined(CONFIG_AEABI)
	bic	scno, scno, #0xff000000		@ mask off SWI op-code
<<<<<<< HEAD
	eor	scno, scno, #__NR_SYSCALL_BASE	@ check OS number
#endif

	ldr	r10, [tsk, #TI_FLAGS]		@ check for syscall tracing
	stmdb	sp!, {r4, r5}			@ push fifth and sixth args

#ifdef CONFIG_SECCOMP
	tst	r10, #_TIF_SECCOMP
	bne	__sys_trace
#endif

	tst	r10, #_TIF_SYSCALL_WORK		@ are we tracing syscalls?
	bne	__sys_trace

	cmp	scno, #NR_syscalls		@ check upper syscall limit
	adr	lr, BSYM(ret_fast_syscall)	@ return address
	ldrcc	pc, [tbl, scno, lsl #2]		@ call sys_* routine

	add	r1, sp, #S_OFF
2:	mov	why, #0				@ no longer a real syscall
	cmp	scno, #(__ARM_NR_BASE - __NR_SYSCALL_BASE)
	eor	r0, scno, #__NR_SYSCALL_BASE	@ put OS number back
	bcs	arm_syscall	
	b	sys_ni_syscall			@ not private func
ENDPROC(vector_swi)
=======
	str	scno, [tsk, #TI_ABI_SYSCALL]
	eor	scno, scno, #__NR_SYSCALL_BASE	@ check OS number
#else
	str	scno, [tsk, #TI_ABI_SYSCALL]
#endif
	/*
	 * Reload the registers that may have been corrupted on entry to
	 * the syscall assembly (by tracing or context tracking.)
	 */
 TRACE(	ldmia	sp, {r0 - r3}		)

local_restart:
	ldr	r10, [tsk, #TI_FLAGS]		@ check for syscall tracing
	stmdb	sp!, {r4, r5}			@ push fifth and sixth args

	tst	r10, #_TIF_SYSCALL_WORK		@ are we tracing syscalls?
	bne	__sys_trace

	invoke_syscall tbl, scno, r10, __ret_fast_syscall

	add	r1, sp, #S_OFF
2:	cmp	scno, #(__ARM_NR_BASE - __NR_SYSCALL_BASE)
	eor	r0, scno, #__NR_SYSCALL_BASE	@ put OS number back
	bcs	arm_syscall
	mov	why, #0				@ no longer a real syscall
	b	sys_ni_syscall			@ not private func

#if defined(CONFIG_OABI_COMPAT) || !defined(CONFIG_AEABI)
	/*
	 * We failed to handle a fault trying to access the page
	 * containing the swi instruction, but we're not really in a
	 * position to return -EFAULT. Instead, return back to the
	 * instruction and re-enter the user fault handling path trying
	 * to page it in. This will likely result in sending SEGV to the
	 * current task.
	 */
9001:
	sub	lr, saved_pc, #4
	str	lr, [sp, #S_PC]
	get_thread_info tsk
	b	ret_fast_syscall
#endif
ENDPROC(vector_swi)
	.ltorg
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	/*
	 * This is the really slow path.  We're going to be doing
	 * context switches, and waiting for our parent to respond.
	 */
__sys_trace:
<<<<<<< HEAD
	mov	r2, scno
	add	r1, sp, #S_OFF
	mov	r0, #0				@ trace entry [IP = 0]
	bl	syscall_trace

	adr	lr, BSYM(__sys_trace_return)	@ return address
	mov	scno, r0			@ syscall number (possibly new)
	add	r1, sp, #S_R0 + S_OFF		@ pointer to regs
	cmp	scno, #NR_syscalls		@ check upper syscall limit
	ldmccia	r1, {r0 - r3}			@ have to reload r0 - r3
	ldrcc	pc, [tbl, scno, lsl #2]		@ call sys_* routine
	b	2b

__sys_trace_return:
	str	r0, [sp, #S_R0 + S_OFF]!	@ save returned r0
	mov	r2, scno
	mov	r1, sp
	mov	r0, #1				@ trace exit [IP = 1]
	bl	syscall_trace
	b	ret_slow_syscall

	.align	5
#ifdef CONFIG_ALIGNMENT_TRAP
	.type	__cr_alignment, #object
__cr_alignment:
	.word	cr_alignment
#endif
	.ltorg
=======
	add	r0, sp, #S_OFF
	bl	syscall_trace_enter
	mov	scno, r0
	invoke_syscall tbl, scno, r10, __sys_trace_return, reload=1
	cmp	scno, #-1			@ skip the syscall?
	bne	2b
	add	sp, sp, #S_OFF			@ restore stack

__sys_trace_return_nosave:
	enable_irq_notrace
	mov	r0, sp
	bl	syscall_trace_exit
	b	ret_slow_syscall

__sys_trace_return:
	str	r0, [sp, #S_R0 + S_OFF]!	@ save returned r0
	mov	r0, sp
	bl	syscall_trace_exit
	b	ret_slow_syscall

	.macro	syscall_table_start, sym
	.equ	__sys_nr, 0
	.type	\sym, #object
ENTRY(\sym)
	.endm

	.macro	syscall, nr, func
	.ifgt	__sys_nr - \nr
	.error	"Duplicated/unorded system call entry"
	.endif
	.rept	\nr - __sys_nr
	.long	sys_ni_syscall
	.endr
	.long	\func
	.equ	__sys_nr, \nr + 1
	.endm

	.macro	syscall_table_end, sym
	.ifgt	__sys_nr - __NR_syscalls
	.error	"System call table too big"
	.endif
	.rept	__NR_syscalls - __sys_nr
	.long	sys_ni_syscall
	.endr
	.size	\sym, . - \sym
	.endm

#define __SYSCALL_WITH_COMPAT(nr, native, compat)	__SYSCALL(nr, native)
#define __SYSCALL(nr, func) syscall nr, func
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

/*
 * This is the syscall table declaration for native ABI syscalls.
 * With EABI a couple syscalls are obsolete and defined as sys_ni_syscall.
 */
<<<<<<< HEAD
#define ABI(native, compat) native
#ifdef CONFIG_AEABI
#define OBSOLETE(syscall) sys_ni_syscall
#else
#define OBSOLETE(syscall) syscall
#endif

	.type	sys_call_table, #object
ENTRY(sys_call_table)
#include "calls.S"
#undef ABI
#undef OBSOLETE
=======
	syscall_table_start sys_call_table
#ifdef CONFIG_AEABI
#include <calls-eabi.S>
#else
#include <calls-oabi.S>
#endif
	syscall_table_end sys_call_table
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

/*============================================================================
 * Special system call wrappers
 */
@ r0 = syscall number
@ r8 = syscall table
sys_syscall:
		bic	scno, r0, #__NR_OABI_SYSCALL_BASE
		cmp	scno, #__NR_syscall - __NR_SYSCALL_BASE
		cmpne	scno, #NR_syscalls	@ check range
<<<<<<< HEAD
		stmloia	sp, {r5, r6}		@ shuffle args
=======
#ifdef CONFIG_CPU_SPECTRE
		movhs	scno, #0
		csdb
#endif
		stmialo	sp, {r5, r6}		@ shuffle args
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		movlo	r0, r1
		movlo	r1, r2
		movlo	r2, r3
		movlo	r3, r4
		ldrlo	pc, [tbl, scno, lsl #2]
		b	sys_ni_syscall
ENDPROC(sys_syscall)

<<<<<<< HEAD
sys_fork_wrapper:
		add	r0, sp, #S_OFF
		b	sys_fork
ENDPROC(sys_fork_wrapper)

sys_vfork_wrapper:
		add	r0, sp, #S_OFF
		b	sys_vfork
ENDPROC(sys_vfork_wrapper)

sys_execve_wrapper:
		add	r3, sp, #S_OFF
		b	sys_execve
ENDPROC(sys_execve_wrapper)

sys_clone_wrapper:
		add	ip, sp, #S_OFF
		str	ip, [sp, #4]
		b	sys_clone
ENDPROC(sys_clone_wrapper)

=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
sys_sigreturn_wrapper:
		add	r0, sp, #S_OFF
		mov	why, #0		@ prevent syscall restart handling
		b	sys_sigreturn
ENDPROC(sys_sigreturn_wrapper)

sys_rt_sigreturn_wrapper:
		add	r0, sp, #S_OFF
		mov	why, #0		@ prevent syscall restart handling
		b	sys_rt_sigreturn
ENDPROC(sys_rt_sigreturn_wrapper)

<<<<<<< HEAD
sys_sigaltstack_wrapper:
		ldr	r2, [sp, #S_OFF + S_SP]
		b	do_sigaltstack
ENDPROC(sys_sigaltstack_wrapper)

=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
sys_statfs64_wrapper:
		teq	r1, #88
		moveq	r1, #84
		b	sys_statfs64
ENDPROC(sys_statfs64_wrapper)

sys_fstatfs64_wrapper:
		teq	r1, #88
		moveq	r1, #84
		b	sys_fstatfs64
ENDPROC(sys_fstatfs64_wrapper)

/*
 * Note: off_4k (r5) is always units of 4K.  If we can't do the requested
 * offset, we return EINVAL.
 */
sys_mmap2:
<<<<<<< HEAD
#if PAGE_SHIFT > 12
		tst	r5, #PGOFF_MASK
		moveq	r5, r5, lsr #PAGE_SHIFT - 12
		streq	r5, [sp, #4]
		beq	sys_mmap_pgoff
		mov	r0, #-EINVAL
		mov	pc, lr
#else
		str	r5, [sp, #4]
		b	sys_mmap_pgoff
#endif
=======
		str	r5, [sp, #4]
		b	sys_mmap_pgoff
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
ENDPROC(sys_mmap2)

#ifdef CONFIG_OABI_COMPAT

/*
 * These are syscalls with argument register differences
 */

sys_oabi_pread64:
		stmia	sp, {r3, r4}
		b	sys_pread64
ENDPROC(sys_oabi_pread64)

sys_oabi_pwrite64:
		stmia	sp, {r3, r4}
		b	sys_pwrite64
ENDPROC(sys_oabi_pwrite64)

sys_oabi_truncate64:
		mov	r3, r2
		mov	r2, r1
		b	sys_truncate64
ENDPROC(sys_oabi_truncate64)

sys_oabi_ftruncate64:
		mov	r3, r2
		mov	r2, r1
		b	sys_ftruncate64
ENDPROC(sys_oabi_ftruncate64)

sys_oabi_readahead:
		str	r3, [sp]
		mov	r3, r2
		mov	r2, r1
		b	sys_readahead
ENDPROC(sys_oabi_readahead)

/*
 * Let's declare a second syscall table for old ABI binaries
 * using the compatibility syscall entries.
 */
<<<<<<< HEAD
#define ABI(native, compat) compat
#define OBSOLETE(syscall) syscall

	.type	sys_oabi_call_table, #object
ENTRY(sys_oabi_call_table)
#include "calls.S"
#undef ABI
#undef OBSOLETE
=======
	syscall_table_start sys_oabi_call_table
#undef __SYSCALL_WITH_COMPAT
#define __SYSCALL_WITH_COMPAT(nr, native, compat)	__SYSCALL(nr, compat)
#include <calls-oabi.S>
	syscall_table_end sys_oabi_call_table
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

#endif

