<<<<<<< HEAD
/*
 * Common signal handling code for both 32 and 64 bits
 *
 *    Copyright (c) 2007 Benjamin Herrenschmidt, IBM Coproration
 *    Extracted from signal_32.c and signal_64.c
 *
 * This file is subject to the terms and conditions of the GNU General
 * Public License.  See the file README.legal in the main directory of
 * this archive for more details.
 */

#include <linux/tracehook.h>
#include <linux/signal.h>
#include <linux/key.h>
#include <asm/hw_breakpoint.h>
#include <asm/uaccess.h>
#include <asm/unistd.h>
#include <asm/debug.h>

#include "signal.h"

=======
// SPDX-License-Identifier: GPL-2.0-or-later
/*
 * Common signal handling code for both 32 and 64 bits
 *
 *    Copyright (c) 2007 Benjamin Herrenschmidt, IBM Corporation
 *    Extracted from signal_32.c and signal_64.c
 */

#include <linux/resume_user_mode.h>
#include <linux/signal.h>
#include <linux/uprobes.h>
#include <linux/key.h>
#include <linux/context_tracking.h>
#include <linux/livepatch.h>
#include <linux/syscalls.h>
#include <asm/hw_breakpoint.h>
#include <linux/uaccess.h>
#include <asm/switch_to.h>
#include <asm/unistd.h>
#include <asm/debug.h>
#include <asm/tm.h>

#include "signal.h"

#ifdef CONFIG_VSX
unsigned long copy_fpr_to_user(void __user *to,
			       struct task_struct *task)
{
	u64 buf[ELF_NFPREG];
	int i;

	/* save FPR copy to local buffer then write to the thread_struct */
	for (i = 0; i < (ELF_NFPREG - 1) ; i++)
		buf[i] = task->thread.TS_FPR(i);
	buf[i] = task->thread.fp_state.fpscr;
	return __copy_to_user(to, buf, ELF_NFPREG * sizeof(double));
}

unsigned long copy_fpr_from_user(struct task_struct *task,
				 void __user *from)
{
	u64 buf[ELF_NFPREG];
	int i;

	if (__copy_from_user(buf, from, ELF_NFPREG * sizeof(double)))
		return 1;
	for (i = 0; i < (ELF_NFPREG - 1) ; i++)
		task->thread.TS_FPR(i) = buf[i];
	task->thread.fp_state.fpscr = buf[i];

	return 0;
}

unsigned long copy_vsx_to_user(void __user *to,
			       struct task_struct *task)
{
	u64 buf[ELF_NVSRHALFREG];
	int i;

	/* save FPR copy to local buffer then write to the thread_struct */
	for (i = 0; i < ELF_NVSRHALFREG; i++)
		buf[i] = task->thread.fp_state.fpr[i][TS_VSRLOWOFFSET];
	return __copy_to_user(to, buf, ELF_NVSRHALFREG * sizeof(double));
}

unsigned long copy_vsx_from_user(struct task_struct *task,
				 void __user *from)
{
	u64 buf[ELF_NVSRHALFREG];
	int i;

	if (__copy_from_user(buf, from, ELF_NVSRHALFREG * sizeof(double)))
		return 1;
	for (i = 0; i < ELF_NVSRHALFREG ; i++)
		task->thread.fp_state.fpr[i][TS_VSRLOWOFFSET] = buf[i];
	return 0;
}

#ifdef CONFIG_PPC_TRANSACTIONAL_MEM
unsigned long copy_ckfpr_to_user(void __user *to,
				  struct task_struct *task)
{
	u64 buf[ELF_NFPREG];
	int i;

	/* save FPR copy to local buffer then write to the thread_struct */
	for (i = 0; i < (ELF_NFPREG - 1) ; i++)
		buf[i] = task->thread.TS_CKFPR(i);
	buf[i] = task->thread.ckfp_state.fpscr;
	return __copy_to_user(to, buf, ELF_NFPREG * sizeof(double));
}

unsigned long copy_ckfpr_from_user(struct task_struct *task,
					  void __user *from)
{
	u64 buf[ELF_NFPREG];
	int i;

	if (__copy_from_user(buf, from, ELF_NFPREG * sizeof(double)))
		return 1;
	for (i = 0; i < (ELF_NFPREG - 1) ; i++)
		task->thread.TS_CKFPR(i) = buf[i];
	task->thread.ckfp_state.fpscr = buf[i];

	return 0;
}

unsigned long copy_ckvsx_to_user(void __user *to,
				  struct task_struct *task)
{
	u64 buf[ELF_NVSRHALFREG];
	int i;

	/* save FPR copy to local buffer then write to the thread_struct */
	for (i = 0; i < ELF_NVSRHALFREG; i++)
		buf[i] = task->thread.ckfp_state.fpr[i][TS_VSRLOWOFFSET];
	return __copy_to_user(to, buf, ELF_NVSRHALFREG * sizeof(double));
}

unsigned long copy_ckvsx_from_user(struct task_struct *task,
					  void __user *from)
{
	u64 buf[ELF_NVSRHALFREG];
	int i;

	if (__copy_from_user(buf, from, ELF_NVSRHALFREG * sizeof(double)))
		return 1;
	for (i = 0; i < ELF_NVSRHALFREG ; i++)
		task->thread.ckfp_state.fpr[i][TS_VSRLOWOFFSET] = buf[i];
	return 0;
}
#endif /* CONFIG_PPC_TRANSACTIONAL_MEM */
#endif

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
/* Log an error when sending an unhandled signal to a process. Controlled
 * through debug.exception-trace sysctl.
 */

<<<<<<< HEAD
int show_unhandled_signals = 0;
=======
int show_unhandled_signals = 1;

unsigned long get_min_sigframe_size(void)
{
	if (IS_ENABLED(CONFIG_PPC64))
		return get_min_sigframe_size_64();
	else
		return get_min_sigframe_size_32();
}

#ifdef CONFIG_COMPAT
unsigned long get_min_sigframe_size_compat(void)
{
	return get_min_sigframe_size_32();
}
#endif
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

/*
 * Allocate space for the signal frame
 */
<<<<<<< HEAD
void __user * get_sigframe(struct k_sigaction *ka, struct pt_regs *regs,
			   size_t frame_size, int is_32)
{
        unsigned long oldsp, newsp;

        /* Default to using normal stack */
        oldsp = get_clean_sp(regs, is_32);

	/* Check for alt stack */
	if ((ka->sa.sa_flags & SA_ONSTACK) &&
	    current->sas_ss_size && !on_sig_stack(oldsp))
		oldsp = (current->sas_ss_sp + current->sas_ss_size);

	/* Get aligned frame */
	newsp = (oldsp - frame_size) & ~0xFUL;

	/* Check access */
	if (!access_ok(VERIFY_WRITE, (void __user *)newsp, oldsp - newsp))
		return NULL;

        return (void __user *)newsp;
}


/*
 * Restore the user process's signal mask
 */
void restore_sigmask(sigset_t *set)
{
	sigdelsetmask(set, ~_BLOCKABLE);
	set_current_blocked(set);
}

=======
static unsigned long get_tm_stackpointer(struct task_struct *tsk);

void __user *get_sigframe(struct ksignal *ksig, struct task_struct *tsk,
			  size_t frame_size, int is_32)
{
        unsigned long oldsp, newsp;
	unsigned long sp = get_tm_stackpointer(tsk);

        /* Default to using normal stack */
	if (is_32)
		oldsp = sp & 0x0ffffffffUL;
	else
		oldsp = sp;
	oldsp = sigsp(oldsp, ksig);
	newsp = (oldsp - frame_size) & ~0xFUL;

        return (void __user *)newsp;
}

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
static void check_syscall_restart(struct pt_regs *regs, struct k_sigaction *ka,
				  int has_handler)
{
	unsigned long ret = regs->gpr[3];
	int restart = 1;

	/* syscall ? */
<<<<<<< HEAD
	if (TRAP(regs) != 0x0C00)
		return;

	/* error signalled ? */
	if (!(regs->ccr & 0x10000000))
		return;
=======
	if (!trap_is_syscall(regs))
		return;

	if (trap_norestart(regs))
		return;

	/* error signalled ? */
	if (trap_is_scv(regs)) {
		/* 32-bit compat mode sign extend? */
		if (!IS_ERR_VALUE(ret))
			return;
		ret = -ret;
	} else if (!(regs->ccr & 0x10000000)) {
		return;
	}
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	switch (ret) {
	case ERESTART_RESTARTBLOCK:
	case ERESTARTNOHAND:
		/* ERESTARTNOHAND means that the syscall should only be
		 * restarted if there was no handler for the signal, and since
		 * we only get here if there is a handler, we dont restart.
		 */
		restart = !has_handler;
		break;
	case ERESTARTSYS:
		/* ERESTARTSYS means to restart the syscall if there is no
		 * handler or the handler was registered with SA_RESTART
		 */
		restart = !has_handler || (ka->sa.sa_flags & SA_RESTART) != 0;
		break;
	case ERESTARTNOINTR:
		/* ERESTARTNOINTR means that the syscall should be
		 * called again after the signal handler returns.
		 */
		break;
	default:
		return;
	}
	if (restart) {
		if (ret == ERESTART_RESTARTBLOCK)
			regs->gpr[0] = __NR_restart_syscall;
		else
			regs->gpr[3] = regs->orig_gpr3;
<<<<<<< HEAD
		regs->nip -= 4;
		regs->result = 0;
	} else {
		regs->result = -EINTR;
		regs->gpr[3] = EINTR;
		regs->ccr |= 0x10000000;
	}
}

static int do_signal(struct pt_regs *regs)
{
	sigset_t *oldset;
	siginfo_t info;
	int signr;
	struct k_sigaction ka;
	int ret;
	int is32 = is_32bit_task();

	if (current_thread_info()->local_flags & _TLF_RESTORE_SIGMASK)
		oldset = &current->saved_sigmask;
	else
		oldset = &current->blocked;

	signr = get_signal_to_deliver(&info, &ka, regs, NULL);

	/* Is there any syscall restart business here ? */
	check_syscall_restart(regs, &ka, signr > 0);

	if (signr <= 0) {
		struct thread_info *ti = current_thread_info();
		/* No signal to deliver -- put the saved sigmask back */
		if (ti->local_flags & _TLF_RESTORE_SIGMASK) {
			ti->local_flags &= ~_TLF_RESTORE_SIGMASK;
			sigprocmask(SIG_SETMASK, &current->saved_sigmask, NULL);
		}
		regs->trap = 0;
		return 0;               /* no signals delivered */
	}

#ifndef CONFIG_PPC_ADV_DEBUG_REGS
=======
		regs_add_return_ip(regs, -4);
		regs->result = 0;
	} else {
		if (trap_is_scv(regs)) {
			regs->result = -EINTR;
			regs->gpr[3] = -EINTR;
		} else {
			regs->result = -EINTR;
			regs->gpr[3] = EINTR;
			regs->ccr |= 0x10000000;
		}
	}
}

static void do_signal(struct task_struct *tsk)
{
	sigset_t *oldset = sigmask_to_save();
	struct ksignal ksig = { .sig = 0 };
	int ret;

	BUG_ON(tsk != current);

	get_signal(&ksig);

	/* Is there any syscall restart business here ? */
	check_syscall_restart(tsk->thread.regs, &ksig.ka, ksig.sig > 0);

	if (ksig.sig <= 0) {
		/* No signal to deliver -- put the saved sigmask back */
		restore_saved_sigmask();
		set_trap_norestart(tsk->thread.regs);
		return;               /* no signals delivered */
	}

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
        /*
	 * Reenable the DABR before delivering the signal to
	 * user space. The DABR will have been cleared if it
	 * triggered inside the kernel.
	 */
<<<<<<< HEAD
	if (current->thread.dabr)
		set_dabr(current->thread.dabr);
#endif
	/* Re-enable the breakpoints for the signal stack */
	thread_change_pc(current, regs);

	if (is32) {
        	if (ka.sa.sa_flags & SA_SIGINFO)
			ret = handle_rt_signal32(signr, &ka, &info, oldset,
					regs);
		else
			ret = handle_signal32(signr, &ka, &info, oldset,
					regs);
	} else {
		ret = handle_rt_signal64(signr, &ka, &info, oldset, regs);
	}

	regs->trap = 0;
	if (ret) {
		block_sigmask(&ka, signr);

		/*
		 * A signal was successfully delivered; the saved sigmask is in
		 * its frame, and we can clear the TLF_RESTORE_SIGMASK flag.
		 */
		current_thread_info()->local_flags &= ~_TLF_RESTORE_SIGMASK;

		/*
		 * Let tracing know that we've done the handler setup.
		 */
		tracehook_signal_handler(signr, &info, &ka, regs,
					 test_thread_flag(TIF_SINGLESTEP));
	}

	return ret;
=======
	if (!IS_ENABLED(CONFIG_PPC_ADV_DEBUG_REGS)) {
		int i;

		for (i = 0; i < nr_wp_slots(); i++) {
			if (tsk->thread.hw_brk[i].address && tsk->thread.hw_brk[i].type)
				__set_breakpoint(i, &tsk->thread.hw_brk[i]);
		}
	}

	/* Re-enable the breakpoints for the signal stack */
	thread_change_pc(tsk, tsk->thread.regs);

	rseq_signal_deliver(&ksig, tsk->thread.regs);

	if (is_32bit_task()) {
        	if (ksig.ka.sa.sa_flags & SA_SIGINFO)
			ret = handle_rt_signal32(&ksig, oldset, tsk);
		else
			ret = handle_signal32(&ksig, oldset, tsk);
	} else {
		ret = handle_rt_signal64(&ksig, oldset, tsk);
	}

	set_trap_norestart(tsk->thread.regs);
	signal_setup_done(ret, &ksig, test_thread_flag(TIF_SINGLESTEP));
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

void do_notify_resume(struct pt_regs *regs, unsigned long thread_info_flags)
{
<<<<<<< HEAD
	if (thread_info_flags & _TIF_SIGPENDING)
		do_signal(regs);

	if (thread_info_flags & _TIF_NOTIFY_RESUME) {
		clear_thread_flag(TIF_NOTIFY_RESUME);
		tracehook_notify_resume(regs);
		if (current->replacement_session_keyring)
			key_replace_session_keyring();
	}
}

long sys_sigaltstack(const stack_t __user *uss, stack_t __user *uoss,
		unsigned long r5, unsigned long r6, unsigned long r7,
		unsigned long r8, struct pt_regs *regs)
{
	return do_sigaltstack(uss, uoss, regs->gpr[1]);
=======
	if (thread_info_flags & _TIF_UPROBE)
		uprobe_notify_resume(regs);

	if (thread_info_flags & _TIF_PATCH_PENDING)
		klp_update_patch_state(current);

	if (thread_info_flags & (_TIF_SIGPENDING | _TIF_NOTIFY_SIGNAL)) {
		BUG_ON(regs != current->thread.regs);
		do_signal(current);
	}

	if (thread_info_flags & _TIF_NOTIFY_RESUME)
		resume_user_mode_work(regs);
}

static unsigned long get_tm_stackpointer(struct task_struct *tsk)
{
	/* When in an active transaction that takes a signal, we need to be
	 * careful with the stack.  It's possible that the stack has moved back
	 * up after the tbegin.  The obvious case here is when the tbegin is
	 * called inside a function that returns before a tend.  In this case,
	 * the stack is part of the checkpointed transactional memory state.
	 * If we write over this non transactionally or in suspend, we are in
	 * trouble because if we get a tm abort, the program counter and stack
	 * pointer will be back at the tbegin but our in memory stack won't be
	 * valid anymore.
	 *
	 * To avoid this, when taking a signal in an active transaction, we
	 * need to use the stack pointer from the checkpointed state, rather
	 * than the speculated state.  This ensures that the signal context
	 * (written tm suspended) will be written below the stack required for
	 * the rollback.  The transaction is aborted because of the treclaim,
	 * so any memory written between the tbegin and the signal will be
	 * rolled back anyway.
	 *
	 * For signals taken in non-TM or suspended mode, we use the
	 * normal/non-checkpointed stack pointer.
	 */
	struct pt_regs *regs = tsk->thread.regs;
	unsigned long ret = regs->gpr[1];

#ifdef CONFIG_PPC_TRANSACTIONAL_MEM
	BUG_ON(tsk != current);

	if (MSR_TM_ACTIVE(regs->msr)) {
		preempt_disable();
		tm_reclaim_current(TM_CAUSE_SIGNAL);
		if (MSR_TM_TRANSACTIONAL(regs->msr))
			ret = tsk->thread.ckpt_regs.gpr[1];

		/*
		 * If we treclaim, we must clear the current thread's TM bits
		 * before re-enabling preemption. Otherwise we might be
		 * preempted and have the live MSR[TS] changed behind our back
		 * (tm_recheckpoint_new_task() would recheckpoint). Besides, we
		 * enter the signal handler in non-transactional state.
		 */
		regs_set_return_msr(regs, regs->msr & ~MSR_TS_MASK);
		preempt_enable();
	}
#endif
	return ret;
}

static const char fm32[] = KERN_INFO "%s[%d]: bad frame in %s: %p nip %08lx lr %08lx\n";
static const char fm64[] = KERN_INFO "%s[%d]: bad frame in %s: %p nip %016lx lr %016lx\n";

void signal_fault(struct task_struct *tsk, struct pt_regs *regs,
		  const char *where, void __user *ptr)
{
	if (show_unhandled_signals)
		printk_ratelimited(regs->msr & MSR_64BIT ? fm64 : fm32, tsk->comm,
				   task_pid_nr(tsk), where, ptr, regs->nip, regs->link);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}
