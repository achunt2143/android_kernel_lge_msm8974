<<<<<<< HEAD
=======
// SPDX-License-Identifier: GPL-2.0-or-later
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
/*
 * OpenRISC signal.c
 *
 * Linux architectural port borrowing liberally from similar works of
 * others.  All original copyrights apply as per the original source
 * declaration.
 *
 * Modifications for the OpenRISC architecture:
 * Copyright (C) 2003 Matjaz Breskvar <phoenix@bsemi.com>
 * Copyright (C) 2010-2011 Jonas Bonn <jonas@southpole.se>
<<<<<<< HEAD
 *
 *      This program is free software; you can redistribute it and/or
 *      modify it under the terms of the GNU General Public License
 *      as published by the Free Software Foundation; either version
 *      2 of the License, or (at your option) any later version.
=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 */

#include <linux/sched.h>
#include <linux/mm.h>
#include <linux/smp.h>
#include <linux/kernel.h>
#include <linux/signal.h>
#include <linux/errno.h>
#include <linux/wait.h>
#include <linux/ptrace.h>
#include <linux/unistd.h>
#include <linux/stddef.h>
<<<<<<< HEAD
#include <linux/tracehook.h>

#include <asm/processor.h>
#include <asm/ucontext.h>
#include <asm/uaccess.h>

#define DEBUG_SIG 0

#define _BLOCKABLE (~(sigmask(SIGKILL) | sigmask(SIGSTOP)))

asmlinkage long
_sys_sigaltstack(const stack_t *uss, stack_t *uoss, struct pt_regs *regs)
{
	return do_sigaltstack(uss, uoss, regs->sp);
}

struct rt_sigframe {
	struct siginfo *pinfo;
	void *puc;
=======
#include <linux/resume_user_mode.h>

#include <asm/processor.h>
#include <asm/syscall.h>
#include <asm/ucontext.h>
#include <linux/uaccess.h>

struct rt_sigframe {
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	struct siginfo info;
	struct ucontext uc;
	unsigned char retcode[16];	/* trampoline code */
};

<<<<<<< HEAD
static int restore_sigcontext(struct pt_regs *regs, struct sigcontext *sc)
{
	unsigned int err = 0;

	/* Alwys make any pending restarted system call return -EINTR */
	current_thread_info()->restart_block.fn = do_no_restart_syscall;

	/*
	 * Restore the regs from &sc->regs.
	 * (sc is already checked for VERIFY_READ since the sigframe was
	 *  checked in sys_sigreturn previously)
	 */
	if (__copy_from_user(regs, sc->regs.gpr, 32 * sizeof(unsigned long)))
		goto badframe;
	if (__copy_from_user(&regs->pc, &sc->regs.pc, sizeof(unsigned long)))
		goto badframe;
	if (__copy_from_user(&regs->sr, &sc->regs.sr, sizeof(unsigned long)))
		goto badframe;
=======
asmlinkage long _sys_rt_sigreturn(struct pt_regs *regs);

asmlinkage int do_work_pending(struct pt_regs *regs, unsigned int thread_flags,
			       int syscall);

static int restore_sigcontext(struct pt_regs *regs,
			      struct sigcontext __user *sc)
{
	int err = 0;

	/* Always make any pending restarted system calls return -EINTR */
	current->restart_block.fn = do_no_restart_syscall;

	/*
	 * Restore the regs from &sc->regs.
	 * (sc is already checked since the sigframe was
	 *  checked in sys_sigreturn previously)
	 */
	err |= __copy_from_user(regs, sc->regs.gpr, 32 * sizeof(unsigned long));
	err |= __copy_from_user(&regs->pc, &sc->regs.pc, sizeof(unsigned long));
	err |= __copy_from_user(&regs->sr, &sc->regs.sr, sizeof(unsigned long));
	err |= __copy_from_user(&regs->fpcsr, &sc->fpcsr, sizeof(unsigned long));
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	/* make sure the SM-bit is cleared so user-mode cannot fool us */
	regs->sr &= ~SPR_SR_SM;

<<<<<<< HEAD
=======
	regs->orig_gpr11 = -1;	/* Avoid syscall restart checks */

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	/* TODO: the other ports use regs->orig_XX to disable syscall checks
	 * after this completes, but we don't use that mechanism. maybe we can
	 * use it now ?
	 */

	return err;
<<<<<<< HEAD

badframe:
	return 1;
=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

asmlinkage long _sys_rt_sigreturn(struct pt_regs *regs)
{
<<<<<<< HEAD
	struct rt_sigframe *frame = (struct rt_sigframe __user *)regs->sp;
	sigset_t set;
	stack_t st;
=======
	struct rt_sigframe __user *frame = (struct rt_sigframe __user *)regs->sp;
	sigset_t set;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	/*
	 * Since we stacked the signal on a dword boundary,
	 * then frame should be dword aligned here.  If it's
	 * not, then the user is trying to mess with us.
	 */
<<<<<<< HEAD
	if (((long)frame) & 3)
		goto badframe;

	if (!access_ok(VERIFY_READ, frame, sizeof(*frame)))
=======
	if (((unsigned long)frame) & 3)
		goto badframe;

	if (!access_ok(frame, sizeof(*frame)))
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		goto badframe;
	if (__copy_from_user(&set, &frame->uc.uc_sigmask, sizeof(set)))
		goto badframe;

<<<<<<< HEAD
	sigdelsetmask(&set, ~_BLOCKABLE);
=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	set_current_blocked(&set);

	if (restore_sigcontext(regs, &frame->uc.uc_mcontext))
		goto badframe;

<<<<<<< HEAD
	if (__copy_from_user(&st, &frame->uc.uc_stack, sizeof(st)))
		goto badframe;
	/* It is more difficult to avoid calling this function than to
	   call it and ignore errors.  */
	do_sigaltstack(&st, NULL, regs->sp);
=======
	if (restore_altstack(&frame->uc.uc_stack))
		goto badframe;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	return regs->gpr[11];

badframe:
<<<<<<< HEAD
	force_sig(SIGSEGV, current);
=======
	force_sig(SIGSEGV);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	return 0;
}

/*
 * Set up a signal frame.
 */

<<<<<<< HEAD
static int setup_sigcontext(struct sigcontext *sc, struct pt_regs *regs,
			    unsigned long mask)
=======
static int setup_sigcontext(struct pt_regs *regs, struct sigcontext __user *sc)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
{
	int err = 0;

	/* copy the regs */
<<<<<<< HEAD

	err |= __copy_to_user(sc->regs.gpr, regs, 32 * sizeof(unsigned long));
	err |= __copy_to_user(&sc->regs.pc, &regs->pc, sizeof(unsigned long));
	err |= __copy_to_user(&sc->regs.sr, &regs->sr, sizeof(unsigned long));

	/* then some other stuff */

	err |= __put_user(mask, &sc->oldmask);
=======
	/* There should be no need to save callee-saved registers here...
	 * ...but we save them anyway.  Revisit this
	 */
	err |= __copy_to_user(sc->regs.gpr, regs, 32 * sizeof(unsigned long));
	err |= __copy_to_user(&sc->regs.pc, &regs->pc, sizeof(unsigned long));
	err |= __copy_to_user(&sc->regs.sr, &regs->sr, sizeof(unsigned long));
	err |= __copy_to_user(&sc->fpcsr, &regs->fpcsr, sizeof(unsigned long));
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	return err;
}

static inline unsigned long align_sigframe(unsigned long sp)
{
	return sp & ~3UL;
}

/*
 * Work out where the signal frame should go.  It's either on the user stack
 * or the alternate stack.
 */

<<<<<<< HEAD
static inline void __user *get_sigframe(struct k_sigaction *ka,
					struct pt_regs *regs, size_t frame_size)
{
	unsigned long sp = regs->sp;
	int onsigstack = on_sig_stack(sp);

	/* redzone */
	sp -= STACK_FRAME_OVERHEAD;

	/* This is the X/Open sanctioned signal stack switching.  */
	if ((ka->sa.sa_flags & SA_ONSTACK) && !onsigstack) {
		if (current->sas_ss_size)
			sp = current->sas_ss_sp + current->sas_ss_size;
	}

	sp = align_sigframe(sp - frame_size);

	/*
	 * If we are on the alternate signal stack and would overflow it, don't.
	 * Return an always-bogus address instead so we will die with SIGSEGV.
	 */
	if (onsigstack && !likely(on_sig_stack(sp)))
		return (void __user *)-1L;

=======
static inline void __user *get_sigframe(struct ksignal *ksig,
					struct pt_regs *regs, size_t frame_size)
{
	unsigned long sp = regs->sp;

	/* redzone */
	sp -= STACK_FRAME_OVERHEAD;
	sp = sigsp(sp, ksig);
	sp = align_sigframe(sp - frame_size);

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	return (void __user *)sp;
}

/* grab and setup a signal frame.
 *
 * basically we stack a lot of state info, and arrange for the
 * user-mode program to return to the kernel using either a
 * trampoline which performs the syscall sigreturn, or a provided
 * user-mode trampoline.
 */
<<<<<<< HEAD
static int setup_rt_frame(int sig, struct k_sigaction *ka, siginfo_t *info,
			  sigset_t *set, struct pt_regs *regs)
{
	struct rt_sigframe *frame;
	unsigned long return_ip;
	int err = 0;

	frame = get_sigframe(ka, regs, sizeof(*frame));

	if (!access_ok(VERIFY_WRITE, frame, sizeof(*frame)))
		goto give_sigsegv;

	err |= __put_user(&frame->info, &frame->pinfo);
	err |= __put_user(&frame->uc, &frame->puc);

	if (ka->sa.sa_flags & SA_SIGINFO)
		err |= copy_siginfo_to_user(&frame->info, info);
	if (err)
		goto give_sigsegv;

	/* Clear all the bits of the ucontext we don't use.  */
	err |= __clear_user(&frame->uc, offsetof(struct ucontext, uc_mcontext));
	err |= __put_user(0, &frame->uc.uc_flags);
	err |= __put_user(NULL, &frame->uc.uc_link);
	err |= __put_user((void *)current->sas_ss_sp,
			  &frame->uc.uc_stack.ss_sp);
	err |= __put_user(sas_ss_flags(regs->sp), &frame->uc.uc_stack.ss_flags);
	err |= __put_user(current->sas_ss_size, &frame->uc.uc_stack.ss_size);
	err |= setup_sigcontext(&frame->uc.uc_mcontext, regs, set->sig[0]);
=======
static int setup_rt_frame(struct ksignal *ksig, sigset_t *set,
			  struct pt_regs *regs)
{
	struct rt_sigframe __user *frame;
	unsigned long return_ip;
	int err = 0;

	frame = get_sigframe(ksig, regs, sizeof(*frame));

	if (!access_ok(frame, sizeof(*frame)))
		return -EFAULT;

	/* Create siginfo.  */
	if (ksig->ka.sa.sa_flags & SA_SIGINFO)
		err |= copy_siginfo_to_user(&frame->info, &ksig->info);

	/* Create the ucontext.  */
	err |= __put_user(0, &frame->uc.uc_flags);
	err |= __put_user(NULL, &frame->uc.uc_link);
	err |= __save_altstack(&frame->uc.uc_stack, regs->sp);
	err |= setup_sigcontext(regs, &frame->uc.uc_mcontext);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	err |= __copy_to_user(&frame->uc.uc_sigmask, set, sizeof(*set));

	if (err)
<<<<<<< HEAD
		goto give_sigsegv;

	/* trampoline - the desired return ip is the retcode itself */
	return_ip = (unsigned long)&frame->retcode;
	/* This is l.ori r11,r0,__NR_sigreturn, l.sys 1 */
	err |= __put_user(0xa960, (short *)(frame->retcode + 0));
	err |= __put_user(__NR_rt_sigreturn, (short *)(frame->retcode + 2));
	err |= __put_user(0x20000001, (unsigned long *)(frame->retcode + 4));
	err |= __put_user(0x15000000, (unsigned long *)(frame->retcode + 8));

	if (err)
		goto give_sigsegv;

	/* TODO what is the current->exec_domain stuff and invmap ? */

	/* Set up registers for signal handler */
	regs->pc = (unsigned long)ka->sa.sa_handler; /* what we enter NOW */
	regs->gpr[9] = (unsigned long)return_ip;     /* what we enter LATER */
	regs->gpr[3] = (unsigned long)sig;           /* arg 1: signo */
=======
		return -EFAULT;

	/* trampoline - the desired return ip is the retcode itself */
	return_ip = (unsigned long)&frame->retcode;
	/* This is:
		l.ori r11,r0,__NR_sigreturn
		l.sys 1
	 */
	err |= __put_user(0xa960,             (short __user *)(frame->retcode + 0));
	err |= __put_user(__NR_rt_sigreturn,  (short __user *)(frame->retcode + 2));
	err |= __put_user(0x20000001, (unsigned long __user *)(frame->retcode + 4));
	err |= __put_user(0x15000000, (unsigned long __user *)(frame->retcode + 8));

	if (err)
		return -EFAULT;

	/* Set up registers for signal handler */
	regs->pc = (unsigned long)ksig->ka.sa.sa_handler; /* what we enter NOW */
	regs->gpr[9] = (unsigned long)return_ip;     /* what we enter LATER */
	regs->gpr[3] = (unsigned long)ksig->sig;           /* arg 1: signo */
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	regs->gpr[4] = (unsigned long)&frame->info;  /* arg 2: (siginfo_t*) */
	regs->gpr[5] = (unsigned long)&frame->uc;    /* arg 3: ucontext */

	/* actually move the usp to reflect the stacked frame */
	regs->sp = (unsigned long)frame;

	return 0;
<<<<<<< HEAD

give_sigsegv:
	force_sigsegv(sig, current);
	return -EFAULT;
}

static inline int
handle_signal(unsigned long sig,
	      siginfo_t *info, struct k_sigaction *ka,
	      sigset_t *oldset, struct pt_regs *regs)
{
	int ret;

	ret = setup_rt_frame(sig, ka, info, oldset, regs);
	if (ret)
		return ret;

	block_sigmask(ka, sig);

	return 0;
=======
}

static inline void
handle_signal(struct ksignal *ksig, struct pt_regs *regs)
{
	int ret;

	ret = setup_rt_frame(ksig, sigmask_to_save(), regs);

	signal_setup_done(ret, ksig, test_thread_flag(TIF_SINGLESTEP));
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

/*
 * Note that 'init' is a special process: it doesn't get signals it doesn't
 * want to handle. Thus you cannot kill init even with a SIGKILL even by
 * mistake.
 *
 * Also note that the regs structure given here as an argument, is the latest
 * pushed pt_regs. It may or may not be the same as the first pushed registers
 * when the initial usermode->kernelmode transition took place. Therefore
 * we can use user_mode(regs) to see if we came directly from kernel or user
 * mode below.
 */

<<<<<<< HEAD
void do_signal(struct pt_regs *regs)
{
	siginfo_t info;
	int signr;
	struct k_sigaction ka;

	/*
	 * We want the common case to go fast, which
	 * is why we may in certain cases get here from
	 * kernel mode. Just return without doing anything
	 * if so.
	 */
	if (!user_mode(regs))
		return;

	signr = get_signal_to_deliver(&info, &ka, regs, NULL);

	/* If we are coming out of a syscall then we need
	 * to check if the syscall was interrupted and wants to be
	 * restarted after handling the signal.  If so, the original
	 * syscall number is put back into r11 and the PC rewound to
	 * point at the l.sys instruction that resulted in the
	 * original syscall.  Syscall results other than the four
	 * below mean that the syscall executed to completion and no
	 * restart is necessary.
	 */
	if (regs->orig_gpr11) {
		int restart = 0;

		switch (regs->gpr[11]) {
		case -ERESTART_RESTARTBLOCK:
		case -ERESTARTNOHAND:
			/* Restart if there is no signal handler */
			restart = (signr <= 0);
			break;
		case -ERESTARTSYS:
			/* Restart if there no signal handler or
			 * SA_RESTART flag is set */
			restart = (signr <= 0 || (ka.sa.sa_flags & SA_RESTART));
			break;
		case -ERESTARTNOINTR:
			/* Always restart */
			restart = 1;
			break;
		}

		if (restart) {
			if (regs->gpr[11] == -ERESTART_RESTARTBLOCK)
				regs->gpr[11] = __NR_restart_syscall;
			else
				regs->gpr[11] = regs->orig_gpr11;
			regs->pc -= 4;
		} else {
			regs->gpr[11] = -EINTR;
		}
	}

	if (signr <= 0) {
		/* no signal to deliver so we just put the saved sigmask
		 * back */
		if (test_thread_flag(TIF_RESTORE_SIGMASK)) {
			clear_thread_flag(TIF_RESTORE_SIGMASK);
			sigprocmask(SIG_SETMASK, &current->saved_sigmask, NULL);
		}

	} else {		/* signr > 0 */
		sigset_t *oldset;

		if (current_thread_info()->flags & _TIF_RESTORE_SIGMASK)
			oldset = &current->saved_sigmask;
		else
			oldset = &current->blocked;

		/* Whee!  Actually deliver the signal.  */
		if (!handle_signal(signr, &info, &ka, oldset, regs)) {
			/* a signal was successfully delivered; the saved
			 * sigmask will have been stored in the signal frame,
			 * and will be restored by sigreturn, so we can simply
			 * clear the TIF_RESTORE_SIGMASK flag */
			clear_thread_flag(TIF_RESTORE_SIGMASK);
		}

		tracehook_signal_handler(signr, &info, &ka, regs,
					 test_thread_flag(TIF_SINGLESTEP));
	}

	return;
}

asmlinkage void do_notify_resume(struct pt_regs *regs)
{
	if (current_thread_info()->flags & _TIF_SIGPENDING)
		do_signal(regs);

	if (current_thread_info()->flags & _TIF_NOTIFY_RESUME) {
		clear_thread_flag(TIF_NOTIFY_RESUME);
		tracehook_notify_resume(regs);
		if (current->replacement_session_keyring)
			key_replace_session_keyring();
	}
=======
static int do_signal(struct pt_regs *regs, int syscall)
{
	struct ksignal ksig;
	unsigned long continue_addr = 0;
	unsigned long restart_addr = 0;
	unsigned long retval = 0;
	int restart = 0;

	if (syscall) {
		continue_addr = regs->pc;
		restart_addr = continue_addr - 4;
		retval = regs->gpr[11];

		/*
		 * Setup syscall restart here so that a debugger will
		 * see the already changed PC.
		 */
		switch (retval) {
		case -ERESTART_RESTARTBLOCK:
			restart = -2;
			fallthrough;
		case -ERESTARTNOHAND:
		case -ERESTARTSYS:
		case -ERESTARTNOINTR:
			restart++;
			regs->gpr[11] = regs->orig_gpr11;
			regs->pc = restart_addr;
			break;
		}
	}

	/*
	 * Get the signal to deliver.  During the call to get_signal the
	 * debugger may change all our registers so we may need to revert
	 * the decision to restart the syscall; specifically, if the PC is
	 * changed, don't restart the syscall.
	 */
	if (get_signal(&ksig)) {
		if (unlikely(restart) && regs->pc == restart_addr) {
			if (retval == -ERESTARTNOHAND ||
			    retval == -ERESTART_RESTARTBLOCK
			    || (retval == -ERESTARTSYS
			        && !(ksig.ka.sa.sa_flags & SA_RESTART))) {
				/* No automatic restart */
				regs->gpr[11] = -EINTR;
				regs->pc = continue_addr;
			}
		}
		handle_signal(&ksig, regs);
	} else {
		/* no handler */
		restore_saved_sigmask();
		/*
		 * Restore pt_regs PC as syscall restart will be handled by
		 * kernel without return to userspace
		 */
		if (unlikely(restart) && regs->pc == restart_addr) {
			regs->pc = continue_addr;
			return restart;
		}
	}

	return 0;
}

asmlinkage int
do_work_pending(struct pt_regs *regs, unsigned int thread_flags, int syscall)
{
	do {
		if (likely(thread_flags & _TIF_NEED_RESCHED)) {
			schedule();
		} else {
			if (unlikely(!user_mode(regs)))
				return 0;
			local_irq_enable();
			if (thread_flags & (_TIF_SIGPENDING|_TIF_NOTIFY_SIGNAL)) {
				int restart = do_signal(regs, syscall);
				if (unlikely(restart)) {
					/*
					 * Restart without handlers.
					 * Deal with it without leaving
					 * the kernel space.
					 */
					return restart;
				}
				syscall = 0;
			} else {
				resume_user_mode_work(regs);
			}
		}
		local_irq_disable();
		thread_flags = read_thread_flags();
	} while (thread_flags & _TIF_WORK_MASK);
	return 0;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}
