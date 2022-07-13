<<<<<<< HEAD
/*
 *  arch/s390/kernel/entry.S
 *    S390 low-level entry points.
 *
 *    Copyright (C) IBM Corp. 1999,2012
 *    Author(s): Martin Schwidefsky (schwidefsky@de.ibm.com),
 *		 Hartmut Penner (hp@de.ibm.com),
 *		 Denis Joseph Barrow (djbarrow@de.ibm.com,barrow_dj@yahoo.com),
 *		 Heiko Carstens <heiko.carstens@de.ibm.com>
 */

#include <linux/init.h>
#include <linux/linkage.h>
#include <asm/cache.h>
=======
/* SPDX-License-Identifier: GPL-2.0 */
/*
 *    S390 low-level entry points.
 *
 *    Copyright IBM Corp. 1999, 2012
 *    Author(s): Martin Schwidefsky (schwidefsky@de.ibm.com),
 *		 Hartmut Penner (hp@de.ibm.com),
 *		 Denis Joseph Barrow (djbarrow@de.ibm.com,barrow_dj@yahoo.com),
 */

#include <linux/export.h>
#include <linux/init.h>
#include <linux/linkage.h>
#include <asm/asm-extable.h>
#include <asm/alternative-asm.h>
#include <asm/processor.h>
#include <asm/cache.h>
#include <asm/dwarf.h>
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#include <asm/errno.h>
#include <asm/ptrace.h>
#include <asm/thread_info.h>
#include <asm/asm-offsets.h>
#include <asm/unistd.h>
#include <asm/page.h>
<<<<<<< HEAD

__PT_R0      =	__PT_GPRS
__PT_R1      =	__PT_GPRS + 4
__PT_R2      =	__PT_GPRS + 8
__PT_R3      =	__PT_GPRS + 12
__PT_R4      =	__PT_GPRS + 16
__PT_R5      =	__PT_GPRS + 20
__PT_R6      =	__PT_GPRS + 24
__PT_R7      =	__PT_GPRS + 28
__PT_R8      =	__PT_GPRS + 32
__PT_R9      =	__PT_GPRS + 36
__PT_R10     =	__PT_GPRS + 40
__PT_R11     =	__PT_GPRS + 44
__PT_R12     =	__PT_GPRS + 48
__PT_R13     =	__PT_GPRS + 524
__PT_R14     =	__PT_GPRS + 56
__PT_R15     =	__PT_GPRS + 60

_TIF_WORK_SVC = (_TIF_SIGPENDING | _TIF_NOTIFY_RESUME | _TIF_NEED_RESCHED | \
		 _TIF_MCCK_PENDING | _TIF_PER_TRAP )
_TIF_WORK_INT = (_TIF_SIGPENDING | _TIF_NOTIFY_RESUME | _TIF_NEED_RESCHED | \
		 _TIF_MCCK_PENDING)
_TIF_TRACE    = (_TIF_SYSCALL_TRACE | _TIF_SYSCALL_AUDIT | _TIF_SECCOMP | \
		 _TIF_SYSCALL_TRACEPOINT)

STACK_SHIFT = PAGE_SHIFT + THREAD_ORDER
STACK_SIZE  = 1 << STACK_SHIFT

#define BASED(name) name-system_call(%r13)

	.macro	TRACE_IRQS_ON
#ifdef CONFIG_TRACE_IRQFLAGS
	basr	%r2,%r0
	l	%r1,BASED(.Lhardirqs_on)
	basr	%r14,%r1		# call trace_hardirqs_on_caller
#endif
	.endm

	.macro	TRACE_IRQS_OFF
#ifdef CONFIG_TRACE_IRQFLAGS
	basr	%r2,%r0
	l	%r1,BASED(.Lhardirqs_off)
	basr	%r14,%r1		# call trace_hardirqs_off_caller
#endif
	.endm

	.macro	LOCKDEP_SYS_EXIT
#ifdef CONFIG_LOCKDEP
	tm	__PT_PSW+1(%r11),0x01	# returning to user ?
	jz	.+10
	l	%r1,BASED(.Llockdep_sys_exit)
	basr	%r14,%r1		# call lockdep_sys_exit
#endif
	.endm

	.macro	CHECK_STACK stacksize,savearea
#ifdef CONFIG_CHECK_STACK
	tml	%r15,\stacksize - CONFIG_STACK_GUARD
	la	%r14,\savearea
=======
#include <asm/sigp.h>
#include <asm/irq.h>
#include <asm/fpu-insn.h>
#include <asm/setup.h>
#include <asm/nmi.h>
#include <asm/nospec-insn.h>

_LPP_OFFSET	= __LC_LPP

	.macro STBEAR address
	ALTERNATIVE "nop", ".insn s,0xb2010000,\address", 193
	.endm

	.macro LBEAR address
	ALTERNATIVE "nop", ".insn s,0xb2000000,\address", 193
	.endm

	.macro LPSWEY address,lpswe
	ALTERNATIVE "b \lpswe; nopr", ".insn siy,0xeb0000000071,\address,0", 193
	.endm

	.macro MBEAR reg
	ALTERNATIVE "brcl 0,0", __stringify(mvc __PT_LAST_BREAK(8,\reg),__LC_LAST_BREAK), 193
	.endm

	.macro	CHECK_STACK savearea
#ifdef CONFIG_CHECK_STACK
	tml	%r15,THREAD_SIZE - CONFIG_STACK_GUARD
	lghi	%r14,\savearea
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	jz	stack_overflow
#endif
	.endm

<<<<<<< HEAD
	.macro	SWITCH_ASYNC savearea,stack,shift
	tmh	%r8,0x0001		# interrupting from user ?
	jnz	1f
	lr	%r14,%r9
	sl	%r14,BASED(.Lcritical_start)
	cl	%r14,BASED(.Lcritical_length)
	jhe	0f
	la	%r11,\savearea		# inside critical section, do cleanup
	bras	%r14,cleanup_critical
	tmh	%r8,0x0001		# retest problem state after cleanup
	jnz	1f
0:	l	%r14,\stack		# are we already on the target stack?
	slr	%r14,%r15
	sra	%r14,\shift
	jnz	1f
	CHECK_STACK 1<<\shift,\savearea
	j	2f
1:	l	%r15,\stack		# load target stack
2:	ahi	%r15,-(STACK_FRAME_OVERHEAD + __PT_SIZE)
	la	%r11,STACK_FRAME_OVERHEAD(%r15)
	.endm

	.macro	ADD64 high,low,timer
	al	\high,\timer
	al	\low,4+\timer
	brc	12,.+8
	ahi	\high,1
	.endm

	.macro	SUB64 high,low,timer
	sl	\high,\timer
	sl	\low,4+\timer
	brc	3,.+8
	ahi	\high,-1
	.endm

	.macro	UPDATE_VTIME high,low,enter_timer
	lm	\high,\low,__LC_EXIT_TIMER
	SUB64	\high,\low,\enter_timer
	ADD64	\high,\low,__LC_USER_TIMER
	stm	\high,\low,__LC_USER_TIMER
	lm	\high,\low,__LC_LAST_UPDATE_TIMER
	SUB64	\high,\low,__LC_EXIT_TIMER
	ADD64	\high,\low,__LC_SYSTEM_TIMER
	stm	\high,\low,__LC_SYSTEM_TIMER
	mvc	__LC_LAST_UPDATE_TIMER(8),\enter_timer
	.endm

	.macro REENABLE_IRQS
	st	%r8,__LC_RETURN_PSW
	ni	__LC_RETURN_PSW,0xbf
	ssm	__LC_RETURN_PSW
	.endm

	.section .kprobes.text, "ax"

/*
 * Scheduler resume function, called by switch_to
 *  gpr2 = (task_struct *) prev
 *  gpr3 = (task_struct *) next
 * Returns:
 *  gpr2 = prev
 */
ENTRY(__switch_to)
	l	%r4,__THREAD_info(%r2)		# get thread_info of prev
	l	%r5,__THREAD_info(%r3)		# get thread_info of next
	tm	__TI_flags+3(%r4),_TIF_MCCK_PENDING # machine check pending?
	jz	0f
	ni	__TI_flags+3(%r4),255-_TIF_MCCK_PENDING	# clear flag in prev
	oi	__TI_flags+3(%r5),_TIF_MCCK_PENDING	# set it in next
0:	stm	%r6,%r15,__SF_GPRS(%r15)	# store gprs of prev task
	st	%r15,__THREAD_ksp(%r2)		# store kernel stack of prev
	l	%r15,__THREAD_ksp(%r3)		# load kernel stack of next
	lctl	%c4,%c4,__TASK_pid(%r3)		# load pid to control reg. 4
	lm	%r6,%r15,__SF_GPRS(%r15)	# load gprs of next task
	st	%r3,__LC_CURRENT		# store task struct of next
	mvc	__LC_CURRENT_PID(4,%r0),__TASK_pid(%r3)	# store pid of next
	st	%r5,__LC_THREAD_INFO		# store thread info of next
	ahi	%r5,STACK_SIZE			# end of kernel stack of next
	st	%r5,__LC_KERNEL_STACK		# store end of kernel stack
	br	%r14

__critical_start:
/*
 * SVC interrupt handler routine. System calls are synchronous events and
 * are executed with interrupts enabled.
 */

ENTRY(system_call)
	stpt	__LC_SYNC_ENTER_TIMER
sysc_stm:
	stm	%r8,%r15,__LC_SAVE_AREA_SYNC
	l	%r12,__LC_THREAD_INFO
	l	%r13,__LC_SVC_NEW_PSW+4
sysc_per:
	l	%r15,__LC_KERNEL_STACK
	ahi	%r15,-(STACK_FRAME_OVERHEAD + __PT_SIZE)
	la	%r11,STACK_FRAME_OVERHEAD(%r15)	# pointer to pt_regs
sysc_vtime:
	UPDATE_VTIME %r8,%r9,__LC_SYNC_ENTER_TIMER
	stm	%r0,%r7,__PT_R0(%r11)
	mvc	__PT_R8(32,%r11),__LC_SAVE_AREA_SYNC
	mvc	__PT_PSW(8,%r11),__LC_SVC_OLD_PSW
	mvc	__PT_INT_CODE(4,%r11),__LC_SVC_ILC
sysc_do_svc:
	oi	__TI_flags+3(%r12),_TIF_SYSCALL
	lh	%r8,__PT_INT_CODE+2(%r11)
	sla	%r8,2				# shift and test for svc0
	jnz	sysc_nr_ok
	# svc 0: system call number in %r1
	cl	%r1,BASED(.Lnr_syscalls)
	jnl	sysc_nr_ok
	sth	%r1,__PT_INT_CODE+2(%r11)
	lr	%r8,%r1
	sla	%r8,2
sysc_nr_ok:
	l	%r10,BASED(.Lsys_call_table)	# 31 bit system call table
	xc	__SF_BACKCHAIN(4,%r15),__SF_BACKCHAIN(%r15)
	st	%r2,__PT_ORIG_GPR2(%r11)
	st	%r7,STACK_FRAME_OVERHEAD(%r15)
	l	%r9,0(%r8,%r10)			# get system call addr.
	tm	__TI_flags+2(%r12),_TIF_TRACE >> 8
	jnz	sysc_tracesys
	basr	%r14,%r9			# call sys_xxxx
	st	%r2,__PT_R2(%r11)		# store return value

sysc_return:
	LOCKDEP_SYS_EXIT
sysc_tif:
	tm	__PT_PSW+1(%r11),0x01		# returning to user ?
	jno	sysc_restore
	tm	__TI_flags+3(%r12),_TIF_WORK_SVC
	jnz	sysc_work			# check for work
	ni	__TI_flags+3(%r12),255-_TIF_SYSCALL
sysc_restore:
	mvc	__LC_RETURN_PSW(8),__PT_PSW(%r11)
	stpt	__LC_EXIT_TIMER
	lm	%r0,%r15,__PT_R0(%r11)
	lpsw	__LC_RETURN_PSW
sysc_done:

#
# One of the work bits is on. Find out which one.
#
sysc_work:
	tm	__TI_flags+3(%r12),_TIF_MCCK_PENDING
	jo	sysc_mcck_pending
	tm	__TI_flags+3(%r12),_TIF_NEED_RESCHED
	jo	sysc_reschedule
	tm	__TI_flags+3(%r12),_TIF_SIGPENDING
	jo	sysc_sigpending
	tm	__TI_flags+3(%r12),_TIF_NOTIFY_RESUME
	jo	sysc_notify_resume
	tm	__TI_flags+3(%r12),_TIF_PER_TRAP
	jo	sysc_singlestep
	j	sysc_return		# beware of critical section cleanup

#
# _TIF_NEED_RESCHED is set, call schedule
#
sysc_reschedule:
	l	%r1,BASED(.Lschedule)
	la	%r14,BASED(sysc_return)
	br	%r1			# call schedule

#
# _TIF_MCCK_PENDING is set, call handler
#
sysc_mcck_pending:
	l	%r1,BASED(.Lhandle_mcck)
	la	%r14,BASED(sysc_return)
	br	%r1			# TIF bit will be cleared by handler

#
# _TIF_SIGPENDING is set, call do_signal
#
sysc_sigpending:
	ni	__TI_flags+3(%r12),255-_TIF_PER_TRAP # clear TIF_PER_TRAP
	lr	%r2,%r11		# pass pointer to pt_regs
	l	%r1,BASED(.Ldo_signal)
	basr	%r14,%r1		# call do_signal
	tm	__TI_flags+3(%r12),_TIF_SYSCALL
	jno	sysc_return
	lm	%r2,%r7,__PT_R2(%r11)	# load svc arguments
	xr	%r8,%r8			# svc 0 returns -ENOSYS
	clc	__PT_INT_CODE+2(2,%r11),BASED(.Lnr_syscalls+2)
	jnl	sysc_nr_ok		# invalid svc number -> do svc 0
	lh	%r8,__PT_INT_CODE+2(%r11)	# load new svc number
	sla	%r8,2
	j	sysc_nr_ok		# restart svc

#
# _TIF_NOTIFY_RESUME is set, call do_notify_resume
#
sysc_notify_resume:
	lr	%r2,%r11		# pass pointer to pt_regs
	l	%r1,BASED(.Ldo_notify_resume)
	la	%r14,BASED(sysc_return)
	br	%r1			# call do_notify_resume

#
# _TIF_PER_TRAP is set, call do_per_trap
#
sysc_singlestep:
	ni	__TI_flags+3(%r12),255-(_TIF_SYSCALL | _TIF_PER_TRAP)
	lr	%r2,%r11		# pass pointer to pt_regs
	l	%r1,BASED(.Ldo_per_trap)
	la	%r14,BASED(sysc_return)
	br	%r1			# call do_per_trap

#
# call tracehook_report_syscall_entry/tracehook_report_syscall_exit before
# and after the system call
#
sysc_tracesys:
	l	%r1,BASED(.Ltrace_enter)
	lr	%r2,%r11		# pass pointer to pt_regs
	la	%r3,0
	xr	%r0,%r0
	icm	%r0,3,__PT_INT_CODE+2(%r11)
	st	%r0,__PT_R2(%r11)
	basr	%r14,%r1		# call do_syscall_trace_enter
	cl	%r2,BASED(.Lnr_syscalls)
	jnl	sysc_tracenogo
	lr	%r8,%r2
	sll	%r8,2
	l	%r9,0(%r8,%r10)
sysc_tracego:
	lm	%r3,%r7,__PT_R3(%r11)
	st	%r7,STACK_FRAME_OVERHEAD(%r15)
	l	%r2,__PT_ORIG_GPR2(%r11)
	basr	%r14,%r9		# call sys_xxx
	st	%r2,__PT_R2(%r11)	# store return value
sysc_tracenogo:
	tm	__TI_flags+2(%r12),_TIF_TRACE >> 8
	jz	sysc_return
	l	%r1,BASED(.Ltrace_exit)
	lr	%r2,%r11		# pass pointer to pt_regs
	la	%r14,BASED(sysc_return)
	br	%r1			# call do_syscall_trace_exit
=======
	.macro	CHECK_VMAP_STACK savearea,oklabel
#ifdef CONFIG_VMAP_STACK
	lgr	%r14,%r15
	nill	%r14,0x10000 - THREAD_SIZE
	oill	%r14,STACK_INIT_OFFSET
	clg	%r14,__LC_KERNEL_STACK
	je	\oklabel
	clg	%r14,__LC_ASYNC_STACK
	je	\oklabel
	clg	%r14,__LC_MCCK_STACK
	je	\oklabel
	clg	%r14,__LC_NODAT_STACK
	je	\oklabel
	clg	%r14,__LC_RESTART_STACK
	je	\oklabel
	lghi	%r14,\savearea
	j	stack_overflow
#else
	j	\oklabel
#endif
	.endm

	/*
	 * The TSTMSK macro generates a test-under-mask instruction by
	 * calculating the memory offset for the specified mask value.
	 * Mask value can be any constant.  The macro shifts the mask
	 * value to calculate the memory offset for the test-under-mask
	 * instruction.
	 */
	.macro TSTMSK addr, mask, size=8, bytepos=0
		.if (\bytepos < \size) && (\mask >> 8)
			.if (\mask & 0xff)
				.error "Mask exceeds byte boundary"
			.endif
			TSTMSK \addr, "(\mask >> 8)", \size, "(\bytepos + 1)"
			.exitm
		.endif
		.ifeq \mask
			.error "Mask must not be zero"
		.endif
		off = \size - \bytepos - 1
		tm	off+\addr, \mask
	.endm

	.macro BPOFF
	ALTERNATIVE "nop", ".insn rrf,0xb2e80000,0,0,12,0", 82
	.endm

	.macro BPON
	ALTERNATIVE "nop", ".insn rrf,0xb2e80000,0,0,13,0", 82
	.endm

	.macro BPENTER tif_ptr,tif_mask
	ALTERNATIVE "TSTMSK \tif_ptr,\tif_mask; jz .+8; .insn rrf,0xb2e80000,0,0,13,0", \
		    "j .+12; nop; nop", 82
	.endm

	.macro BPEXIT tif_ptr,tif_mask
	TSTMSK	\tif_ptr,\tif_mask
	ALTERNATIVE "jz .+8;  .insn rrf,0xb2e80000,0,0,12,0", \
		    "jnz .+8; .insn rrf,0xb2e80000,0,0,13,0", 82
	.endm

#if IS_ENABLED(CONFIG_KVM)
	.macro SIEEXIT sie_control
	lg	%r9,\sie_control		# get control block pointer
	ni	__SIE_PROG0C+3(%r9),0xfe	# no longer in SIE
	lctlg	%c1,%c1,__LC_KERNEL_ASCE	# load primary asce
	ni	__LC_CPU_FLAGS+7,255-_CIF_SIE
	larl	%r9,sie_exit			# skip forward to sie_exit
	.endm
#endif

	.macro STACKLEAK_ERASE
#ifdef CONFIG_GCC_PLUGIN_STACKLEAK
	brasl	%r14,stackleak_erase_on_task_stack
#endif
	.endm

	GEN_BR_THUNK %r14

	.section .kprobes.text, "ax"
.Ldummy:
	/*
	 * The following nop exists only in order to avoid that the next
	 * symbol starts at the beginning of the kprobes text section.
	 * In that case there would be several symbols at the same address.
	 * E.g. objdump would take an arbitrary symbol when disassembling
	 * the code.
	 * With the added nop in between this cannot happen.
	 */
	nop	0

/*
 * Scheduler resume function, called by __switch_to
 *  gpr2 = (task_struct *)prev
 *  gpr3 = (task_struct *)next
 * Returns:
 *  gpr2 = prev
 */
SYM_FUNC_START(__switch_to_asm)
	stmg	%r6,%r15,__SF_GPRS(%r15)	# store gprs of prev task
	lghi	%r4,__TASK_stack
	lghi	%r1,__TASK_thread
	llill	%r5,STACK_INIT_OFFSET
	stg	%r15,__THREAD_ksp(%r1,%r2)	# store kernel stack of prev
	lg	%r15,0(%r4,%r3)			# start of kernel stack of next
	agr	%r15,%r5			# end of kernel stack of next
	stg	%r3,__LC_CURRENT		# store task struct of next
	stg	%r15,__LC_KERNEL_STACK		# store end of kernel stack
	lg	%r15,__THREAD_ksp(%r1,%r3)	# load kernel stack of next
	aghi	%r3,__TASK_pid
	mvc	__LC_CURRENT_PID(4,%r0),0(%r3)	# store pid of next
	lmg	%r6,%r15,__SF_GPRS(%r15)	# load gprs of next task
	ALTERNATIVE "nop", "lpp _LPP_OFFSET", 40
	BR_EX	%r14
SYM_FUNC_END(__switch_to_asm)

#if IS_ENABLED(CONFIG_KVM)
/*
 * __sie64a calling convention:
 * %r2 pointer to sie control block phys
 * %r3 pointer to sie control block virt
 * %r4 guest register save area
 */
SYM_FUNC_START(__sie64a)
	stmg	%r6,%r14,__SF_GPRS(%r15)	# save kernel registers
	lg	%r12,__LC_CURRENT
	stg	%r2,__SF_SIE_CONTROL_PHYS(%r15)	# save sie block physical..
	stg	%r3,__SF_SIE_CONTROL(%r15)	# ...and virtual addresses
	stg	%r4,__SF_SIE_SAVEAREA(%r15)	# save guest register save area
	xc	__SF_SIE_REASON(8,%r15),__SF_SIE_REASON(%r15) # reason code = 0
	mvc	__SF_SIE_FLAGS(8,%r15),__TI_flags(%r12) # copy thread flags
	lmg	%r0,%r13,0(%r4)			# load guest gprs 0-13
	lg	%r14,__LC_GMAP			# get gmap pointer
	ltgr	%r14,%r14
	jz	.Lsie_gmap
	oi	__LC_CPU_FLAGS+7,_CIF_SIE
	lctlg	%c1,%c1,__GMAP_ASCE(%r14)	# load primary asce
.Lsie_gmap:
	lg	%r14,__SF_SIE_CONTROL(%r15)	# get control block pointer
	oi	__SIE_PROG0C+3(%r14),1		# we are going into SIE now
	tm	__SIE_PROG20+3(%r14),3		# last exit...
	jnz	.Lsie_skip
	lg	%r14,__SF_SIE_CONTROL_PHYS(%r15)	# get sie block phys addr
	BPEXIT	__SF_SIE_FLAGS(%r15),_TIF_ISOLATE_BP_GUEST
.Lsie_entry:
	sie	0(%r14)
# Let the next instruction be NOP to avoid triggering a machine check
# and handling it in a guest as result of the instruction execution.
	nopr	7
.Lsie_leave:
	BPOFF
	BPENTER	__SF_SIE_FLAGS(%r15),_TIF_ISOLATE_BP_GUEST
.Lsie_skip:
	lg	%r14,__SF_SIE_CONTROL(%r15)	# get control block pointer
	ni	__SIE_PROG0C+3(%r14),0xfe	# no longer in SIE
	lctlg	%c1,%c1,__LC_KERNEL_ASCE	# load primary asce
	ni	__LC_CPU_FLAGS+7,255-_CIF_SIE
# some program checks are suppressing. C code (e.g. do_protection_exception)
# will rewind the PSW by the ILC, which is often 4 bytes in case of SIE. There
# are some corner cases (e.g. runtime instrumentation) where ILC is unpredictable.
# Other instructions between __sie64a and .Lsie_done should not cause program
# interrupts. So lets use 3 nops as a landing pad for all possible rewinds.
.Lrewind_pad6:
	nopr	7
.Lrewind_pad4:
	nopr	7
.Lrewind_pad2:
	nopr	7
SYM_INNER_LABEL(sie_exit, SYM_L_GLOBAL)
	lg	%r14,__SF_SIE_SAVEAREA(%r15)	# load guest register save area
	stmg	%r0,%r13,0(%r14)		# save guest gprs 0-13
	xgr	%r0,%r0				# clear guest registers to
	xgr	%r1,%r1				# prevent speculative use
	xgr	%r3,%r3
	xgr	%r4,%r4
	xgr	%r5,%r5
	lmg	%r6,%r14,__SF_GPRS(%r15)	# restore kernel registers
	lg	%r2,__SF_SIE_REASON(%r15)	# return exit reason code
	BR_EX	%r14
.Lsie_fault:
	lghi	%r14,-EFAULT
	stg	%r14,__SF_SIE_REASON(%r15)	# set exit reason code
	j	sie_exit

	EX_TABLE(.Lrewind_pad6,.Lsie_fault)
	EX_TABLE(.Lrewind_pad4,.Lsie_fault)
	EX_TABLE(.Lrewind_pad2,.Lsie_fault)
	EX_TABLE(sie_exit,.Lsie_fault)
SYM_FUNC_END(__sie64a)
EXPORT_SYMBOL(__sie64a)
EXPORT_SYMBOL(sie_exit)
#endif

/*
 * SVC interrupt handler routine. System calls are synchronous events and
 * are entered with interrupts disabled.
 */

SYM_CODE_START(system_call)
	stpt	__LC_SYS_ENTER_TIMER
	stmg	%r8,%r15,__LC_SAVE_AREA_SYNC
	BPOFF
	lghi	%r14,0
.Lsysc_per:
	STBEAR	__LC_LAST_BREAK
	lctlg	%c1,%c1,__LC_KERNEL_ASCE
	lg	%r15,__LC_KERNEL_STACK
	xc	__SF_BACKCHAIN(8,%r15),__SF_BACKCHAIN(%r15)
	stmg	%r0,%r7,STACK_FRAME_OVERHEAD+__PT_R0(%r15)
	# clear user controlled register to prevent speculative use
	xgr	%r0,%r0
	xgr	%r1,%r1
	xgr	%r4,%r4
	xgr	%r5,%r5
	xgr	%r6,%r6
	xgr	%r7,%r7
	xgr	%r8,%r8
	xgr	%r9,%r9
	xgr	%r10,%r10
	xgr	%r11,%r11
	la	%r2,STACK_FRAME_OVERHEAD(%r15)	# pointer to pt_regs
	mvc	__PT_R8(64,%r2),__LC_SAVE_AREA_SYNC
	MBEAR	%r2
	lgr	%r3,%r14
	brasl	%r14,__do_syscall
	STACKLEAK_ERASE
	lctlg	%c1,%c1,__LC_USER_ASCE
	mvc	__LC_RETURN_PSW(16),STACK_FRAME_OVERHEAD+__PT_PSW(%r15)
	BPON
	LBEAR	STACK_FRAME_OVERHEAD+__PT_LAST_BREAK(%r15)
	lmg	%r0,%r15,STACK_FRAME_OVERHEAD+__PT_R0(%r15)
	stpt	__LC_EXIT_TIMER
	LPSWEY	__LC_RETURN_PSW,__LC_RETURN_LPSWE
SYM_CODE_END(system_call)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

#
# a new process exits the kernel with ret_from_fork
#
<<<<<<< HEAD
ENTRY(ret_from_fork)
	la	%r11,STACK_FRAME_OVERHEAD(%r15)
	l	%r12,__LC_THREAD_INFO
	l	%r13,__LC_SVC_NEW_PSW+4
	tm	__PT_PSW+1(%r11),0x01	# forking a kernel thread ?
	jo	0f
	st	%r15,__PT_R15(%r11)	# store stack pointer for new kthread
0:	l	%r1,BASED(.Lschedule_tail)
	basr	%r14,%r1		# call schedule_tail
	TRACE_IRQS_ON
	ssm	__LC_SVC_NEW_PSW	# reenable interrupts
	j	sysc_tracenogo

#
# kernel_execve function needs to deal with pt_regs that is not
# at the usual place
#
ENTRY(kernel_execve)
	stm	%r12,%r15,48(%r15)
	lr	%r14,%r15
	l	%r13,__LC_SVC_NEW_PSW+4
	ahi	%r15,-(STACK_FRAME_OVERHEAD + __PT_SIZE)
	st	%r14,__SF_BACKCHAIN(%r15)
	la	%r12,STACK_FRAME_OVERHEAD(%r15)
	xc	0(__PT_SIZE,%r12),0(%r12)
	l	%r1,BASED(.Ldo_execve)
	lr	%r5,%r12
	basr	%r14,%r1		# call do_execve
	ltr	%r2,%r2
	je	0f
	ahi	%r15,(STACK_FRAME_OVERHEAD + __PT_SIZE)
	lm	%r12,%r15,48(%r15)
	br	%r14
	# execve succeeded.
0:	ssm	__LC_PGM_NEW_PSW	# disable I/O and ext. interrupts
	l	%r15,__LC_KERNEL_STACK	# load ksp
	ahi	%r15,-(STACK_FRAME_OVERHEAD + __PT_SIZE)
	la	%r11,STACK_FRAME_OVERHEAD(%r15)
	mvc	0(__PT_SIZE,%r11),0(%r12)	# copy pt_regs
	l	%r12,__LC_THREAD_INFO
	xc	__SF_BACKCHAIN(4,%r15),__SF_BACKCHAIN(%r15)
	ssm	__LC_SVC_NEW_PSW	# reenable interrupts
	l	%r1,BASED(.Lexecve_tail)
	basr	%r14,%r1		# call execve_tail
	j	sysc_return
=======
SYM_CODE_START(ret_from_fork)
	lgr	%r3,%r11
	brasl	%r14,__ret_from_fork
	STACKLEAK_ERASE
	lctlg	%c1,%c1,__LC_USER_ASCE
	mvc	__LC_RETURN_PSW(16),STACK_FRAME_OVERHEAD+__PT_PSW(%r15)
	BPON
	LBEAR	STACK_FRAME_OVERHEAD+__PT_LAST_BREAK(%r15)
	lmg	%r0,%r15,STACK_FRAME_OVERHEAD+__PT_R0(%r15)
	stpt	__LC_EXIT_TIMER
	LPSWEY	__LC_RETURN_PSW,__LC_RETURN_LPSWE
SYM_CODE_END(ret_from_fork)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

/*
 * Program check handler routine
 */

<<<<<<< HEAD
ENTRY(pgm_check_handler)
	stpt	__LC_SYNC_ENTER_TIMER
	stm	%r8,%r15,__LC_SAVE_AREA_SYNC
	l	%r12,__LC_THREAD_INFO
	l	%r13,__LC_SVC_NEW_PSW+4
	lm	%r8,%r9,__LC_PGM_OLD_PSW
	tmh	%r8,0x0001		# test problem state bit
	jnz	1f			# -> fault in user space
	tmh	%r8,0x4000		# PER bit set in old PSW ?
	jnz	0f			# -> enabled, can't be a double fault
	tm	__LC_PGM_ILC+3,0x80	# check for per exception
	jnz	pgm_svcper		# -> single stepped svc
0:	CHECK_STACK STACK_SIZE,__LC_SAVE_AREA_SYNC
	j	2f
1:	UPDATE_VTIME %r14,%r15,__LC_SYNC_ENTER_TIMER
	l	%r15,__LC_KERNEL_STACK
2:	ahi	%r15,-(STACK_FRAME_OVERHEAD + __PT_SIZE)
	la	%r11,STACK_FRAME_OVERHEAD(%r15)
	stm	%r0,%r7,__PT_R0(%r11)
	mvc	__PT_R8(32,%r11),__LC_SAVE_AREA_SYNC
	stm	%r8,%r9,__PT_PSW(%r11)
	mvc	__PT_INT_CODE(4,%r11),__LC_PGM_ILC
	mvc	__PT_INT_PARM_LONG(4,%r11),__LC_TRANS_EXC_CODE
	tm	__LC_PGM_ILC+3,0x80	# check for per exception
	jz	0f
	l	%r1,__TI_task(%r12)
	tmh	%r8,0x0001		# kernel per event ?
	jz	pgm_kprobe
	oi	__TI_flags+3(%r12),_TIF_PER_TRAP
	mvc	__THREAD_per_address(4,%r1),__LC_PER_ADDRESS
	mvc	__THREAD_per_cause(2,%r1),__LC_PER_CAUSE
	mvc	__THREAD_per_paid(1,%r1),__LC_PER_PAID
0:	REENABLE_IRQS
	xc	__SF_BACKCHAIN(4,%r15),__SF_BACKCHAIN(%r15)
	l	%r1,BASED(.Ljump_table)
	la	%r10,0x7f
	n	%r10,__PT_INT_CODE(%r11)
	je	sysc_return
	sll	%r10,2
	l	%r1,0(%r10,%r1)		# load address of handler routine
	lr	%r2,%r11		# pass pointer to pt_regs
	basr	%r14,%r1		# branch to interrupt-handler
	j	sysc_return

#
# PER event in supervisor state, must be kprobes
#
pgm_kprobe:
	REENABLE_IRQS
	xc	__SF_BACKCHAIN(4,%r15),__SF_BACKCHAIN(%r15)
	l	%r1,BASED(.Ldo_per_trap)
	lr	%r2,%r11		# pass pointer to pt_regs
	basr	%r14,%r1		# call do_per_trap
	j	sysc_return
=======
SYM_CODE_START(pgm_check_handler)
	stpt	__LC_SYS_ENTER_TIMER
	BPOFF
	stmg	%r8,%r15,__LC_SAVE_AREA_SYNC
	lgr	%r10,%r15
	lmg	%r8,%r9,__LC_PGM_OLD_PSW
	tmhh	%r8,0x0001		# coming from user space?
	jno	.Lpgm_skip_asce
	lctlg	%c1,%c1,__LC_KERNEL_ASCE
	j	3f			# -> fault in user space
.Lpgm_skip_asce:
1:	tmhh	%r8,0x4000		# PER bit set in old PSW ?
	jnz	2f			# -> enabled, can't be a double fault
	tm	__LC_PGM_ILC+3,0x80	# check for per exception
	jnz	.Lpgm_svcper		# -> single stepped svc
2:	CHECK_STACK __LC_SAVE_AREA_SYNC
	aghi	%r15,-(STACK_FRAME_OVERHEAD + __PT_SIZE)
	# CHECK_VMAP_STACK branches to stack_overflow or 4f
	CHECK_VMAP_STACK __LC_SAVE_AREA_SYNC,4f
3:	lg	%r15,__LC_KERNEL_STACK
4:	la	%r11,STACK_FRAME_OVERHEAD(%r15)
	xc	__PT_FLAGS(8,%r11),__PT_FLAGS(%r11)
	xc	__SF_BACKCHAIN(8,%r15),__SF_BACKCHAIN(%r15)
	stmg	%r0,%r7,__PT_R0(%r11)
	mvc	__PT_R8(64,%r11),__LC_SAVE_AREA_SYNC
	mvc	__PT_LAST_BREAK(8,%r11),__LC_PGM_LAST_BREAK
	stctg	%c1,%c1,__PT_CR1(%r11)
#if IS_ENABLED(CONFIG_KVM)
	ltg	%r12,__LC_GMAP
	jz	5f
	clc	__GMAP_ASCE(8,%r12), __PT_CR1(%r11)
	jne	5f
	BPENTER	__SF_SIE_FLAGS(%r10),_TIF_ISOLATE_BP_GUEST
	SIEEXIT __SF_SIE_CONTROL(%r10)
#endif
5:	stmg	%r8,%r9,__PT_PSW(%r11)
	# clear user controlled registers to prevent speculative use
	xgr	%r0,%r0
	xgr	%r1,%r1
	xgr	%r3,%r3
	xgr	%r4,%r4
	xgr	%r5,%r5
	xgr	%r6,%r6
	xgr	%r7,%r7
	lgr	%r2,%r11
	brasl	%r14,__do_pgm_check
	tmhh	%r8,0x0001		# returning to user space?
	jno	.Lpgm_exit_kernel
	STACKLEAK_ERASE
	lctlg	%c1,%c1,__LC_USER_ASCE
	BPON
	stpt	__LC_EXIT_TIMER
.Lpgm_exit_kernel:
	mvc	__LC_RETURN_PSW(16),STACK_FRAME_OVERHEAD+__PT_PSW(%r15)
	LBEAR	STACK_FRAME_OVERHEAD+__PT_LAST_BREAK(%r15)
	lmg	%r0,%r15,STACK_FRAME_OVERHEAD+__PT_R0(%r15)
	LPSWEY	__LC_RETURN_PSW,__LC_RETURN_LPSWE
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

#
# single stepped system call
#
<<<<<<< HEAD
pgm_svcper:
	oi	__TI_flags+3(%r12),_TIF_PER_TRAP
	mvc	__LC_RETURN_PSW(4),__LC_SVC_NEW_PSW
	mvc	__LC_RETURN_PSW+4(4),BASED(.Lsysc_per)
	lpsw	__LC_RETURN_PSW		# branch to sysc_per and enable irqs

/*
 * IO interrupt handler routine
 */

ENTRY(io_int_handler)
	stck	__LC_INT_CLOCK
	stpt	__LC_ASYNC_ENTER_TIMER
	stm	%r8,%r15,__LC_SAVE_AREA_ASYNC
	l	%r12,__LC_THREAD_INFO
	l	%r13,__LC_SVC_NEW_PSW+4
	lm	%r8,%r9,__LC_IO_OLD_PSW
	tmh	%r8,0x0001		# interrupting from user ?
	jz	io_skip
	UPDATE_VTIME %r14,%r15,__LC_ASYNC_ENTER_TIMER
io_skip:
	SWITCH_ASYNC __LC_SAVE_AREA_ASYNC,__LC_ASYNC_STACK,STACK_SHIFT
	stm	%r0,%r7,__PT_R0(%r11)
	mvc	__PT_R8(32,%r11),__LC_SAVE_AREA_ASYNC
	stm	%r8,%r9,__PT_PSW(%r11)
	TRACE_IRQS_OFF
	xc	__SF_BACKCHAIN(4,%r15),__SF_BACKCHAIN(%r15)
	l	%r1,BASED(.Ldo_IRQ)
	lr	%r2,%r11		# pass pointer to pt_regs
	basr	%r14,%r1		# call do_IRQ
io_return:
	LOCKDEP_SYS_EXIT
	TRACE_IRQS_ON
io_tif:
	tm	__TI_flags+3(%r12),_TIF_WORK_INT
	jnz	io_work			# there is work to do (signals etc.)
io_restore:
	mvc	__LC_RETURN_PSW(8),__PT_PSW(%r11)
	stpt	__LC_EXIT_TIMER
	lm	%r0,%r15,__PT_R0(%r11)
	lpsw	__LC_RETURN_PSW
io_done:

#
# There is work todo, find out in which context we have been interrupted:
# 1) if we return to user space we can do all _TIF_WORK_INT work
# 2) if we return to kernel code and preemptive scheduling is enabled check
#    the preemption counter and if it is zero call preempt_schedule_irq
# Before any work can be done, a switch to the kernel stack is required.
#
io_work:
	tm	__PT_PSW+1(%r11),0x01	# returning to user ?
	jo	io_work_user		# yes -> do resched & signal
#ifdef CONFIG_PREEMPT
	# check for preemptive scheduling
	icm	%r0,15,__TI_precount(%r12)
	jnz	io_restore		# preemption disabled
	tm	__TI_flags+3(%r12),_TIF_NEED_RESCHED
	jno	io_restore
	# switch to kernel stack
	l	%r1,__PT_R15(%r11)
	ahi	%r1,-(STACK_FRAME_OVERHEAD + __PT_SIZE)
	mvc	STACK_FRAME_OVERHEAD(__PT_SIZE,%r1),0(%r11)
	xc	__SF_BACKCHAIN(4,%r1),__SF_BACKCHAIN(%r1)
	la	%r11,STACK_FRAME_OVERHEAD(%r1)
	lr	%r15,%r1
	# TRACE_IRQS_ON already done at io_return, call
	# TRACE_IRQS_OFF to keep things symmetrical
	TRACE_IRQS_OFF
	l	%r1,BASED(.Lpreempt_irq)
	basr	%r14,%r1		# call preempt_schedule_irq
	j	io_return
#else
	j	io_restore
#endif

#
# Need to do work before returning to userspace, switch to kernel stack
#
io_work_user:
	l	%r1,__LC_KERNEL_STACK
	ahi	%r1,-(STACK_FRAME_OVERHEAD + __PT_SIZE)
	mvc	STACK_FRAME_OVERHEAD(__PT_SIZE,%r1),0(%r11)
	xc	__SF_BACKCHAIN(4,%r1),__SF_BACKCHAIN(%r1)
	la	%r11,STACK_FRAME_OVERHEAD(%r1)
	lr	%r15,%r1

#
# One of the work bits is on. Find out which one.
# Checked are: _TIF_SIGPENDING, _TIF_NOTIFY_RESUME, _TIF_NEED_RESCHED
#		and _TIF_MCCK_PENDING
#
io_work_tif:
	tm	__TI_flags+3(%r12),_TIF_MCCK_PENDING
	jo	io_mcck_pending
	tm	__TI_flags+3(%r12),_TIF_NEED_RESCHED
	jo	io_reschedule
	tm	__TI_flags+3(%r12),_TIF_SIGPENDING
	jo	io_sigpending
	tm	__TI_flags+3(%r12),_TIF_NOTIFY_RESUME
	jo	io_notify_resume
	j	io_return		# beware of critical section cleanup

#
# _TIF_MCCK_PENDING is set, call handler
#
io_mcck_pending:
	# TRACE_IRQS_ON already done at io_return
	l	%r1,BASED(.Lhandle_mcck)
	basr	%r14,%r1		# TIF bit will be cleared by handler
	TRACE_IRQS_OFF
	j	io_return

#
# _TIF_NEED_RESCHED is set, call schedule
#
io_reschedule:
	# TRACE_IRQS_ON already done at io_return
	l	%r1,BASED(.Lschedule)
	ssm	__LC_SVC_NEW_PSW	# reenable interrupts
	basr	%r14,%r1		# call scheduler
	ssm	__LC_PGM_NEW_PSW	# disable I/O and ext. interrupts
	TRACE_IRQS_OFF
	j	io_return

#
# _TIF_SIGPENDING is set, call do_signal
#
io_sigpending:
	# TRACE_IRQS_ON already done at io_return
	l	%r1,BASED(.Ldo_signal)
	ssm	__LC_SVC_NEW_PSW	# reenable interrupts
	lr	%r2,%r11		# pass pointer to pt_regs
	basr	%r14,%r1		# call do_signal
	ssm	__LC_PGM_NEW_PSW	# disable I/O and ext. interrupts
	TRACE_IRQS_OFF
	j	io_return

#
# _TIF_SIGPENDING is set, call do_signal
#
io_notify_resume:
	# TRACE_IRQS_ON already done at io_return
	l	%r1,BASED(.Ldo_notify_resume)
	ssm	__LC_SVC_NEW_PSW	# reenable interrupts
	lr	%r2,%r11		# pass pointer to pt_regs
	basr	%r14,%r1		# call do_notify_resume
	ssm	__LC_PGM_NEW_PSW	# disable I/O and ext. interrupts
	TRACE_IRQS_OFF
	j	io_return

/*
 * External interrupt handler routine
 */

ENTRY(ext_int_handler)
	stck	__LC_INT_CLOCK
	stpt	__LC_ASYNC_ENTER_TIMER
	stm	%r8,%r15,__LC_SAVE_AREA_ASYNC
	l	%r12,__LC_THREAD_INFO
	l	%r13,__LC_SVC_NEW_PSW+4
	lm	%r8,%r9,__LC_EXT_OLD_PSW
	tmh	%r8,0x0001		# interrupting from user ?
	jz	ext_skip
	UPDATE_VTIME %r14,%r15,__LC_ASYNC_ENTER_TIMER
ext_skip:
	SWITCH_ASYNC __LC_SAVE_AREA_ASYNC,__LC_ASYNC_STACK,STACK_SHIFT
	stm	%r0,%r7,__PT_R0(%r11)
	mvc	__PT_R8(32,%r11),__LC_SAVE_AREA_ASYNC
	stm	%r8,%r9,__PT_PSW(%r11)
	TRACE_IRQS_OFF
	lr	%r2,%r11		# pass pointer to pt_regs
	l	%r3,__LC_EXT_CPU_ADDR	# get cpu address + interruption code
	l	%r4,__LC_EXT_PARAMS	# get external parameters
	l	%r1,BASED(.Ldo_extint)
	basr	%r14,%r1		# call do_extint
	j	io_return

/*
 * Load idle PSW. The second "half" of this function is in cleanup_idle.
 */
ENTRY(psw_idle)
	st	%r4,__SF_EMPTY(%r15)
	basr	%r1,0
	la	%r1,psw_idle_lpsw+4-.(%r1)
	st	%r1,__SF_EMPTY+4(%r15)
	oi	__SF_EMPTY+4(%r15),0x80
	la	%r1,.Lvtimer_max-psw_idle_lpsw-4(%r1)
	stck	__IDLE_ENTER(%r2)
	ltr	%r5,%r5
	stpt	__VQ_IDLE_ENTER(%r3)
	jz	psw_idle_lpsw
	spt	0(%r1)
psw_idle_lpsw:
	lpsw	__SF_EMPTY(%r15)
	br	%r14
psw_idle_end:

__critical_end:
=======
.Lpgm_svcper:
	mvc	__LC_RETURN_PSW(8),__LC_SVC_NEW_PSW
	larl	%r14,.Lsysc_per
	stg	%r14,__LC_RETURN_PSW+8
	lghi	%r14,1
	LBEAR	__LC_PGM_LAST_BREAK
	LPSWEY	__LC_RETURN_PSW,__LC_RETURN_LPSWE # branch to .Lsysc_per
SYM_CODE_END(pgm_check_handler)

/*
 * Interrupt handler macro used for external and IO interrupts.
 */
.macro INT_HANDLER name,lc_old_psw,handler
SYM_CODE_START(\name)
	stckf	__LC_INT_CLOCK
	stpt	__LC_SYS_ENTER_TIMER
	STBEAR	__LC_LAST_BREAK
	BPOFF
	stmg	%r8,%r15,__LC_SAVE_AREA_ASYNC
	lmg	%r8,%r9,\lc_old_psw
	tmhh	%r8,0x0001			# interrupting from user ?
	jnz	1f
#if IS_ENABLED(CONFIG_KVM)
	TSTMSK	__LC_CPU_FLAGS,_CIF_SIE
	jz	0f
	BPENTER	__SF_SIE_FLAGS(%r15),_TIF_ISOLATE_BP_GUEST
	SIEEXIT __SF_SIE_CONTROL(%r15)
#endif
0:	CHECK_STACK __LC_SAVE_AREA_ASYNC
	aghi	%r15,-(STACK_FRAME_OVERHEAD + __PT_SIZE)
	j	2f
1:	lctlg	%c1,%c1,__LC_KERNEL_ASCE
	lg	%r15,__LC_KERNEL_STACK
2:	xc	__SF_BACKCHAIN(8,%r15),__SF_BACKCHAIN(%r15)
	la	%r11,STACK_FRAME_OVERHEAD(%r15)
	stmg	%r0,%r7,__PT_R0(%r11)
	# clear user controlled registers to prevent speculative use
	xgr	%r0,%r0
	xgr	%r1,%r1
	xgr	%r3,%r3
	xgr	%r4,%r4
	xgr	%r5,%r5
	xgr	%r6,%r6
	xgr	%r7,%r7
	xgr	%r10,%r10
	xc	__PT_FLAGS(8,%r11),__PT_FLAGS(%r11)
	mvc	__PT_R8(64,%r11),__LC_SAVE_AREA_ASYNC
	MBEAR	%r11
	stmg	%r8,%r9,__PT_PSW(%r11)
	lgr	%r2,%r11		# pass pointer to pt_regs
	brasl	%r14,\handler
	mvc	__LC_RETURN_PSW(16),__PT_PSW(%r11)
	tmhh	%r8,0x0001		# returning to user ?
	jno	2f
	STACKLEAK_ERASE
	lctlg	%c1,%c1,__LC_USER_ASCE
	BPON
	stpt	__LC_EXIT_TIMER
2:	LBEAR	__PT_LAST_BREAK(%r11)
	lmg	%r0,%r15,__PT_R0(%r11)
	LPSWEY	__LC_RETURN_PSW,__LC_RETURN_LPSWE
SYM_CODE_END(\name)
.endm

INT_HANDLER ext_int_handler,__LC_EXT_OLD_PSW,do_ext_irq
INT_HANDLER io_int_handler,__LC_IO_OLD_PSW,do_io_irq

/*
 * Load idle PSW.
 */
SYM_FUNC_START(psw_idle)
	stg	%r14,(__SF_GPRS+8*8)(%r15)
	stg	%r3,__SF_EMPTY(%r15)
	larl	%r1,psw_idle_exit
	stg	%r1,__SF_EMPTY+8(%r15)
	larl	%r1,smp_cpu_mtid
	llgf	%r1,0(%r1)
	ltgr	%r1,%r1
	jz	.Lpsw_idle_stcctm
	.insn	rsy,0xeb0000000017,%r1,5,__MT_CYCLES_ENTER(%r2)
.Lpsw_idle_stcctm:
	oi	__LC_CPU_FLAGS+7,_CIF_ENABLED_WAIT
	BPON
	stckf	__CLOCK_IDLE_ENTER(%r2)
	stpt	__TIMER_IDLE_ENTER(%r2)
	lpswe	__SF_EMPTY(%r15)
SYM_INNER_LABEL(psw_idle_exit, SYM_L_GLOBAL)
	BR_EX	%r14
SYM_FUNC_END(psw_idle)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

/*
 * Machine check handler routines
 */
<<<<<<< HEAD

ENTRY(mcck_int_handler)
	stck	__LC_MCCK_CLOCK
	spt	__LC_CPU_TIMER_SAVE_AREA	# revalidate cpu timer
	lm	%r0,%r15,__LC_GPREGS_SAVE_AREA	# revalidate gprs
	l	%r12,__LC_THREAD_INFO
	l	%r13,__LC_SVC_NEW_PSW+4
	lm	%r8,%r9,__LC_MCK_OLD_PSW
	tm	__LC_MCCK_CODE,0x80	# system damage?
	jo	mcck_panic		# yes -> rest of mcck code invalid
	la	%r14,__LC_CPU_TIMER_SAVE_AREA
	mvc	__LC_MCCK_ENTER_TIMER(8),0(%r14)
	tm	__LC_MCCK_CODE+5,0x02	# stored cpu timer value valid?
	jo	3f
	la	%r14,__LC_SYNC_ENTER_TIMER
	clc	0(8,%r14),__LC_ASYNC_ENTER_TIMER
	jl	0f
	la	%r14,__LC_ASYNC_ENTER_TIMER
0:	clc	0(8,%r14),__LC_EXIT_TIMER
=======
SYM_CODE_START(mcck_int_handler)
	BPOFF
	lmg	%r8,%r9,__LC_MCK_OLD_PSW
	TSTMSK	__LC_MCCK_CODE,MCCK_CODE_SYSTEM_DAMAGE
	jo	.Lmcck_panic		# yes -> rest of mcck code invalid
	TSTMSK	__LC_MCCK_CODE,MCCK_CODE_CR_VALID
	jno	.Lmcck_panic		# control registers invalid -> panic
	ptlb
	lghi	%r14,__LC_CPU_TIMER_SAVE_AREA
	mvc	__LC_MCCK_ENTER_TIMER(8),0(%r14)
	TSTMSK	__LC_MCCK_CODE,MCCK_CODE_CPU_TIMER_VALID
	jo	3f
	la	%r14,__LC_SYS_ENTER_TIMER
	clc	0(8,%r14),__LC_EXIT_TIMER
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	jl	1f
	la	%r14,__LC_EXIT_TIMER
1:	clc	0(8,%r14),__LC_LAST_UPDATE_TIMER
	jl	2f
	la	%r14,__LC_LAST_UPDATE_TIMER
2:	spt	0(%r14)
	mvc	__LC_MCCK_ENTER_TIMER(8),0(%r14)
<<<<<<< HEAD
3:	tm	__LC_MCCK_CODE+2,0x09	# mwp + ia of old psw valid?
	jno	mcck_panic		# no -> skip cleanup critical
	tm	%r8,0x0001		# interrupting from user ?
	jz	mcck_skip
	UPDATE_VTIME %r14,%r15,__LC_MCCK_ENTER_TIMER
mcck_skip:
	SWITCH_ASYNC __LC_GPREGS_SAVE_AREA+32,__LC_PANIC_STACK,PAGE_SHIFT
	stm	%r0,%r7,__PT_R0(%r11)
	mvc	__PT_R8(32,%r11),__LC_GPREGS_SAVE_AREA+32
	stm	%r8,%r9,__PT_PSW(%r11)
	xc	__SF_BACKCHAIN(4,%r15),__SF_BACKCHAIN(%r15)
	l	%r1,BASED(.Ldo_machine_check)
	lr	%r2,%r11		# pass pointer to pt_regs
	basr	%r14,%r1		# call s390_do_machine_check
	tm	__PT_PSW+1(%r11),0x01	# returning to user ?
	jno	mcck_return
	l	%r1,__LC_KERNEL_STACK	# switch to kernel stack
	ahi	%r1,-(STACK_FRAME_OVERHEAD + __PT_SIZE)
	mvc	STACK_FRAME_OVERHEAD(__PT_SIZE,%r1),0(%r11)
	xc	__SF_BACKCHAIN(4,%r1),__SF_BACKCHAIN(%r1)
	la	%r11,STACK_FRAME_OVERHEAD(%r15)
	lr	%r15,%r1
	ssm	__LC_PGM_NEW_PSW	# turn dat on, keep irqs off
	tm	__TI_flags+3(%r12),_TIF_MCCK_PENDING
	jno	mcck_return
	TRACE_IRQS_OFF
	l	%r1,BASED(.Lhandle_mcck)
	basr	%r14,%r1		# call s390_handle_mcck
	TRACE_IRQS_ON
mcck_return:
	mvc	__LC_RETURN_MCCK_PSW(8),__PT_PSW(%r11) # move return PSW
	tm	__LC_RETURN_MCCK_PSW+1,0x01 # returning to user ?
	jno	0f
	lm	%r0,%r15,__PT_R0(%r11)
	stpt	__LC_EXIT_TIMER
	lpsw	__LC_RETURN_MCCK_PSW
0:	lm	%r0,%r15,__PT_R0(%r11)
	lpsw	__LC_RETURN_MCCK_PSW

mcck_panic:
	l	%r14,__LC_PANIC_STACK
	slr	%r14,%r15
	sra	%r14,PAGE_SHIFT
	jz	0f
	l	%r15,__LC_PANIC_STACK
0:	ahi	%r15,-(STACK_FRAME_OVERHEAD + __PT_SIZE)
	j	mcck_skip

#
# PSW restart interrupt handler
#
ENTRY(restart_int_handler)
	st	%r15,__LC_SAVE_AREA_RESTART
	l	%r15,__LC_RESTART_STACK
	ahi	%r15,-__PT_SIZE			# create pt_regs on stack
	xc	0(__PT_SIZE,%r15),0(%r15)
	stm	%r0,%r14,__PT_R0(%r15)
	mvc	__PT_R15(4,%r15),__LC_SAVE_AREA_RESTART
	mvc	__PT_PSW(8,%r15),__LC_RST_OLD_PSW # store restart old psw
	ahi	%r15,-STACK_FRAME_OVERHEAD	# create stack frame on stack
	xc	0(STACK_FRAME_OVERHEAD,%r15),0(%r15)
	lm	%r1,%r3,__LC_RESTART_FN		# load fn, parm & source cpu
	ltr	%r3,%r3				# test source cpu address
	jm	1f				# negative -> skip source stop
0:	sigp	%r4,%r3,1			# sigp sense to source cpu
	brc	10,0b				# wait for status stored
1:	basr	%r14,%r1			# call function
	stap	__SF_EMPTY(%r15)		# store cpu address
	lh	%r3,__SF_EMPTY(%r15)
2:	sigp	%r4,%r3,5			# sigp stop to current cpu
	brc	2,2b
3:	j	3b

	.section .kprobes.text, "ax"

#ifdef CONFIG_CHECK_STACK
=======
3:	TSTMSK	__LC_MCCK_CODE,MCCK_CODE_PSW_MWP_VALID
	jno	.Lmcck_panic
	tmhh	%r8,0x0001		# interrupting from user ?
	jnz	.Lmcck_user
	TSTMSK	__LC_MCCK_CODE,MCCK_CODE_PSW_IA_VALID
	jno	.Lmcck_panic
#if IS_ENABLED(CONFIG_KVM)
	TSTMSK	__LC_CPU_FLAGS,_CIF_SIE
	jz	.Lmcck_user
	# Need to compare the address instead of a CIF_SIE* flag.
	# Otherwise there would be a race between setting the flag
	# and entering SIE (or leaving and clearing the flag). This
	# would cause machine checks targeted at the guest to be
	# handled by the host.
	larl	%r14,.Lsie_entry
	clgrjl	%r9,%r14, 4f
	larl	%r14,.Lsie_leave
	clgrjhe	%r9,%r14, 4f
	oi	__LC_CPU_FLAGS+7, _CIF_MCCK_GUEST
4:	BPENTER	__SF_SIE_FLAGS(%r15),_TIF_ISOLATE_BP_GUEST
	SIEEXIT __SF_SIE_CONTROL(%r15)
#endif
.Lmcck_user:
	lg	%r15,__LC_MCCK_STACK
	la	%r11,STACK_FRAME_OVERHEAD(%r15)
	stctg	%c1,%c1,__PT_CR1(%r11)
	lctlg	%c1,%c1,__LC_KERNEL_ASCE
	xc	__SF_BACKCHAIN(8,%r15),__SF_BACKCHAIN(%r15)
	lghi	%r14,__LC_GPREGS_SAVE_AREA+64
	stmg	%r0,%r7,__PT_R0(%r11)
	# clear user controlled registers to prevent speculative use
	xgr	%r0,%r0
	xgr	%r1,%r1
	xgr	%r3,%r3
	xgr	%r4,%r4
	xgr	%r5,%r5
	xgr	%r6,%r6
	xgr	%r7,%r7
	xgr	%r10,%r10
	mvc	__PT_R8(64,%r11),0(%r14)
	stmg	%r8,%r9,__PT_PSW(%r11)
	xc	__PT_FLAGS(8,%r11),__PT_FLAGS(%r11)
	xc	__SF_BACKCHAIN(8,%r15),__SF_BACKCHAIN(%r15)
	lgr	%r2,%r11		# pass pointer to pt_regs
	brasl	%r14,s390_do_machine_check
	lctlg	%c1,%c1,__PT_CR1(%r11)
	lmg	%r0,%r10,__PT_R0(%r11)
	mvc	__LC_RETURN_MCCK_PSW(16),__PT_PSW(%r11) # move return PSW
	tm	__LC_RETURN_MCCK_PSW+1,0x01 # returning to user ?
	jno	0f
	BPON
	stpt	__LC_EXIT_TIMER
0:	ALTERNATIVE "nop", __stringify(lghi %r12,__LC_LAST_BREAK_SAVE_AREA),193
	LBEAR	0(%r12)
	lmg	%r11,%r15,__PT_R11(%r11)
	LPSWEY	__LC_RETURN_MCCK_PSW,__LC_RETURN_MCCK_LPSWE

.Lmcck_panic:
	/*
	 * Iterate over all possible CPU addresses in the range 0..0xffff
	 * and stop each CPU using signal processor. Use compare and swap
	 * to allow just one CPU-stopper and prevent concurrent CPUs from
	 * stopping each other while leaving the others running.
	 */
	lhi	%r5,0
	lhi	%r6,1
	larl	%r7,stop_lock
	cs	%r5,%r6,0(%r7)		# single CPU-stopper only
	jnz	4f
	larl	%r7,this_cpu
	stap	0(%r7)			# this CPU address
	lh	%r4,0(%r7)
	nilh	%r4,0
	lhi	%r0,1
	sll	%r0,16			# CPU counter
	lhi	%r3,0			# next CPU address
0:	cr	%r3,%r4
	je	2f
1:	sigp	%r1,%r3,SIGP_STOP	# stop next CPU
	brc	SIGP_CC_BUSY,1b
2:	ahi	%r3,1
	brct	%r0,0b
3:	sigp	%r1,%r4,SIGP_STOP	# stop this CPU
	brc	SIGP_CC_BUSY,3b
4:	j	4b
SYM_CODE_END(mcck_int_handler)

SYM_CODE_START(restart_int_handler)
	ALTERNATIVE "nop", "lpp _LPP_OFFSET", 40
	stg	%r15,__LC_SAVE_AREA_RESTART
	TSTMSK	__LC_RESTART_FLAGS,RESTART_FLAG_CTLREGS,4
	jz	0f
	lctlg	%c0,%c15,__LC_CREGS_SAVE_AREA
0:	larl	%r15,daton_psw
	lpswe	0(%r15)				# turn dat on, keep irqs off
.Ldaton:
	lg	%r15,__LC_RESTART_STACK
	xc	STACK_FRAME_OVERHEAD(__PT_SIZE,%r15),STACK_FRAME_OVERHEAD(%r15)
	stmg	%r0,%r14,STACK_FRAME_OVERHEAD+__PT_R0(%r15)
	mvc	STACK_FRAME_OVERHEAD+__PT_R15(8,%r15),__LC_SAVE_AREA_RESTART
	mvc	STACK_FRAME_OVERHEAD+__PT_PSW(16,%r15),__LC_RST_OLD_PSW
	xc	0(STACK_FRAME_OVERHEAD,%r15),0(%r15)
	lg	%r1,__LC_RESTART_FN		# load fn, parm & source cpu
	lg	%r2,__LC_RESTART_DATA
	lgf	%r3,__LC_RESTART_SOURCE
	ltgr	%r3,%r3				# test source cpu address
	jm	1f				# negative -> skip source stop
0:	sigp	%r4,%r3,SIGP_SENSE		# sigp sense to source cpu
	brc	10,0b				# wait for status stored
1:	basr	%r14,%r1			# call function
	stap	__SF_EMPTY(%r15)		# store cpu address
	llgh	%r3,__SF_EMPTY(%r15)
2:	sigp	%r4,%r3,SIGP_STOP		# sigp stop to current cpu
	brc	2,2b
3:	j	3b
SYM_CODE_END(restart_int_handler)

	.section .kprobes.text, "ax"

#if defined(CONFIG_CHECK_STACK) || defined(CONFIG_VMAP_STACK)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
/*
 * The synchronous or the asynchronous stack overflowed. We are dead.
 * No need to properly save the registers, we are going to panic anyway.
 * Setup a pt_regs so that show_trace can provide a good call trace.
 */
<<<<<<< HEAD
stack_overflow:
	l	%r15,__LC_PANIC_STACK	# change to panic stack
	ahi	%r15,-__PT_SIZE		# create pt_regs
	stm	%r0,%r7,__PT_R0(%r15)
	stm	%r8,%r9,__PT_PSW(%r15)
	mvc	__PT_R8(32,%r11),0(%r14)
	lr	%r15,%r11
	ahi	%r15,-STACK_FRAME_OVERHEAD
	l	%r1,BASED(1f)
	xc	__SF_BACKCHAIN(4,%r15),__SF_BACKCHAIN(%r15)
	lr	%r2,%r11		# pass pointer to pt_regs
	br	%r1			# branch to kernel_stack_overflow
1:	.long	kernel_stack_overflow
#endif

cleanup_table:
	.long	system_call + 0x80000000
	.long	sysc_do_svc + 0x80000000
	.long	sysc_tif + 0x80000000
	.long	sysc_restore + 0x80000000
	.long	sysc_done + 0x80000000
	.long	io_tif + 0x80000000
	.long	io_restore + 0x80000000
	.long	io_done + 0x80000000
	.long	psw_idle + 0x80000000
	.long	psw_idle_end + 0x80000000

cleanup_critical:
	cl	%r9,BASED(cleanup_table)	# system_call
	jl	0f
	cl	%r9,BASED(cleanup_table+4)	# sysc_do_svc
	jl	cleanup_system_call
	cl	%r9,BASED(cleanup_table+8)	# sysc_tif
	jl	0f
	cl	%r9,BASED(cleanup_table+12)	# sysc_restore
	jl	cleanup_sysc_tif
	cl	%r9,BASED(cleanup_table+16)	# sysc_done
	jl	cleanup_sysc_restore
	cl	%r9,BASED(cleanup_table+20)	# io_tif
	jl	0f
	cl	%r9,BASED(cleanup_table+24)	# io_restore
	jl	cleanup_io_tif
	cl	%r9,BASED(cleanup_table+28)	# io_done
	jl	cleanup_io_restore
	cl	%r9,BASED(cleanup_table+32)	# psw_idle
	jl	0f
	cl	%r9,BASED(cleanup_table+36)	# psw_idle_end
	jl	cleanup_idle
0:	br	%r14

cleanup_system_call:
	# check if stpt has been executed
	cl	%r9,BASED(cleanup_system_call_insn)
	jh	0f
	mvc	__LC_SYNC_ENTER_TIMER(8),__LC_ASYNC_ENTER_TIMER
	chi	%r11,__LC_SAVE_AREA_ASYNC
	je	0f
	mvc	__LC_SYNC_ENTER_TIMER(8),__LC_MCCK_ENTER_TIMER
0:	# check if stm has been executed
	cl	%r9,BASED(cleanup_system_call_insn+4)
	jh	0f
	mvc	__LC_SAVE_AREA_SYNC(32),0(%r11)
0:	# set up saved registers r12, and r13
	st	%r12,16(%r11)		# r12 thread-info pointer
	st	%r13,20(%r11)		# r13 literal-pool pointer
	# check if the user time calculation has been done
	cl	%r9,BASED(cleanup_system_call_insn+8)
	jh	0f
	l	%r10,__LC_EXIT_TIMER
	l	%r15,__LC_EXIT_TIMER+4
	SUB64	%r10,%r15,__LC_SYNC_ENTER_TIMER
	ADD64	%r10,%r15,__LC_USER_TIMER
	st	%r10,__LC_USER_TIMER
	st	%r15,__LC_USER_TIMER+4
0:	# check if the system time calculation has been done
	cl	%r9,BASED(cleanup_system_call_insn+12)
	jh	0f
	l	%r10,__LC_LAST_UPDATE_TIMER
	l	%r15,__LC_LAST_UPDATE_TIMER+4
	SUB64	%r10,%r15,__LC_EXIT_TIMER
	ADD64	%r10,%r15,__LC_SYSTEM_TIMER
	st	%r10,__LC_SYSTEM_TIMER
	st	%r15,__LC_SYSTEM_TIMER+4
0:	# update accounting time stamp
	mvc	__LC_LAST_UPDATE_TIMER(8),__LC_SYNC_ENTER_TIMER
	# set up saved register 11
	l	%r15,__LC_KERNEL_STACK
	ahi	%r15,-__PT_SIZE
	st	%r15,12(%r11)		# r11 pt_regs pointer
	# fill pt_regs
	mvc	__PT_R8(32,%r15),__LC_SAVE_AREA_SYNC
	stm	%r0,%r7,__PT_R0(%r15)
	mvc	__PT_PSW(8,%r15),__LC_SVC_OLD_PSW
	mvc	__PT_INT_CODE(4,%r15),__LC_SVC_ILC
	# setup saved register 15
	ahi	%r15,-STACK_FRAME_OVERHEAD
	st	%r15,28(%r11)		# r15 stack pointer
	# set new psw address and exit
	l	%r9,BASED(cleanup_table+4)	# sysc_do_svc + 0x80000000
	br	%r14
cleanup_system_call_insn:
	.long	system_call + 0x80000000
	.long	sysc_stm + 0x80000000
	.long	sysc_vtime + 0x80000000 + 36
	.long	sysc_vtime + 0x80000000 + 76

cleanup_sysc_tif:
	l	%r9,BASED(cleanup_table+8)	# sysc_tif + 0x80000000
	br	%r14

cleanup_sysc_restore:
	cl	%r9,BASED(cleanup_sysc_restore_insn)
	jhe	0f
	l	%r9,12(%r11)		# get saved pointer to pt_regs
	mvc	__LC_RETURN_PSW(8),__PT_PSW(%r9)
	mvc	0(32,%r11),__PT_R8(%r9)
	lm	%r0,%r7,__PT_R0(%r9)
0:	lm	%r8,%r9,__LC_RETURN_PSW
	br	%r14
cleanup_sysc_restore_insn:
	.long	sysc_done - 4 + 0x80000000

cleanup_io_tif:
	l	%r9,BASED(cleanup_table+20)	# io_tif + 0x80000000
	br	%r14

cleanup_io_restore:
	cl	%r9,BASED(cleanup_io_restore_insn)
	jhe	0f
	l	%r9,12(%r11)		# get saved r11 pointer to pt_regs
	mvc	__LC_RETURN_PSW(8),__PT_PSW(%r9)
	mvc	0(32,%r11),__PT_R8(%r9)
	lm	%r0,%r7,__PT_R0(%r9)
0:	lm	%r8,%r9,__LC_RETURN_PSW
	br	%r14
cleanup_io_restore_insn:
	.long	io_done - 4 + 0x80000000

cleanup_idle:
	# copy interrupt clock & cpu timer
	mvc	__IDLE_EXIT(8,%r2),__LC_INT_CLOCK
	mvc	__VQ_IDLE_EXIT(8,%r3),__LC_ASYNC_ENTER_TIMER
	chi	%r11,__LC_SAVE_AREA_ASYNC
	je	0f
	mvc	__IDLE_EXIT(8,%r2),__LC_MCCK_CLOCK
	mvc	__VQ_IDLE_EXIT(8,%r3),__LC_MCCK_ENTER_TIMER
0:	# check if stck has been executed
	cl	%r9,BASED(cleanup_idle_insn)
	jhe	1f
	mvc	__IDLE_ENTER(8,%r2),__IDLE_EXIT(%r2)
	mvc	__VQ_IDLE_ENTER(8,%r3),__VQ_IDLE_EXIT(%r3)
	j	2f
1:	# check if the cpu timer has been reprogrammed
	ltr	%r5,%r5
	jz	2f
	spt	__VQ_IDLE_ENTER(%r3)
2:	# account system time going idle
	lm	%r9,%r10,__LC_STEAL_TIMER
	ADD64	%r9,%r10,__IDLE_ENTER(%r2)
	SUB64	%r9,%r10,__LC_LAST_UPDATE_CLOCK
	stm	%r9,%r10,__LC_STEAL_TIMER
	mvc	__LC_LAST_UPDATE_CLOCK(8),__IDLE_EXIT(%r2)
	lm	%r9,%r10,__LC_SYSTEM_TIMER
	ADD64	%r9,%r10,__LC_LAST_UPDATE_TIMER
	SUB64	%r9,%r10,__VQ_IDLE_ENTER(%r3)
	stm	%r9,%r10,__LC_SYSTEM_TIMER
	mvc	__LC_LAST_UPDATE_TIMER(8),__VQ_IDLE_EXIT(%r3)
	# prepare return psw
	n	%r8,BASED(cleanup_idle_wait)	# clear wait state bit
	l	%r9,24(%r11)			# return from psw_idle
	br	%r14
cleanup_idle_insn:
	.long	psw_idle_lpsw + 0x80000000
cleanup_idle_wait:
	.long	0xfffdffff

/*
 * Integer constants
 */
	.align	4
.Lnr_syscalls:
	.long	NR_syscalls
.Lvtimer_max:
	.quad	0x7fffffffffffffff

/*
 * Symbol constants
 */
.Ldo_machine_check:	.long	s390_do_machine_check
.Lhandle_mcck:		.long	s390_handle_mcck
.Ldo_IRQ:		.long	do_IRQ
.Ldo_extint:		.long	do_extint
.Ldo_signal:		.long	do_signal
.Ldo_notify_resume:	.long	do_notify_resume
.Ldo_per_trap:		.long	do_per_trap
.Ldo_execve:		.long	do_execve
.Lexecve_tail:		.long	execve_tail
.Ljump_table:		.long	pgm_check_table
.Lschedule:		.long	schedule
#ifdef CONFIG_PREEMPT
.Lpreempt_irq:		.long	preempt_schedule_irq
#endif
.Ltrace_enter:		.long	do_syscall_trace_enter
.Ltrace_exit:		.long	do_syscall_trace_exit
.Lschedule_tail:	.long	schedule_tail
.Lsys_call_table:	.long	sys_call_table
.Lsysc_per:		.long	sysc_per + 0x80000000
#ifdef CONFIG_TRACE_IRQFLAGS
.Lhardirqs_on:		.long	trace_hardirqs_on_caller
.Lhardirqs_off:		.long	trace_hardirqs_off_caller
#endif
#ifdef CONFIG_LOCKDEP
.Llockdep_sys_exit:	.long	lockdep_sys_exit
#endif
.Lcritical_start:	.long	__critical_start + 0x80000000
.Lcritical_length:	.long	__critical_end - __critical_start

		.section .rodata, "a"
#define SYSCALL(esa,esame,emu)	.long esa
	.globl	sys_call_table
sys_call_table:
#include "syscalls.S"
#undef SYSCALL
=======
SYM_CODE_START(stack_overflow)
	lg	%r15,__LC_NODAT_STACK	# change to panic stack
	la	%r11,STACK_FRAME_OVERHEAD(%r15)
	stmg	%r0,%r7,__PT_R0(%r11)
	stmg	%r8,%r9,__PT_PSW(%r11)
	mvc	__PT_R8(64,%r11),0(%r14)
	stg	%r10,__PT_ORIG_GPR2(%r11) # store last break to orig_gpr2
	xc	__SF_BACKCHAIN(8,%r15),__SF_BACKCHAIN(%r15)
	lgr	%r2,%r11		# pass pointer to pt_regs
	jg	kernel_stack_overflow
SYM_CODE_END(stack_overflow)
#endif

	.section .data, "aw"
	.balign	4
SYM_DATA_LOCAL(stop_lock,	.long 0)
SYM_DATA_LOCAL(this_cpu,	.short 0)
	.balign	8
SYM_DATA_START_LOCAL(daton_psw)
	.quad	PSW_KERNEL_BITS
	.quad	.Ldaton
SYM_DATA_END(daton_psw)

	.section .rodata, "a"
	.balign	8
#define SYSCALL(esame,emu)	.quad __s390x_ ## esame
SYM_DATA_START(sys_call_table)
#include "asm/syscall_table.h"
SYM_DATA_END(sys_call_table)
#undef SYSCALL

#ifdef CONFIG_COMPAT

#define SYSCALL(esame,emu)	.quad __s390_ ## emu
SYM_DATA_START(sys_call_table_emu)
#include "asm/syscall_table.h"
SYM_DATA_END(sys_call_table_emu)
#undef SYSCALL
#endif
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
