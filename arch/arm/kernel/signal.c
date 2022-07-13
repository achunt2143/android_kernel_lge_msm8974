<<<<<<< HEAD
=======
// SPDX-License-Identifier: GPL-2.0-only
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
/*
 *  linux/arch/arm/kernel/signal.c
 *
 *  Copyright (C) 1995-2009 Russell King
<<<<<<< HEAD
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 */
#include <linux/errno.h>
#include <linux/random.h>
#include <linux/signal.h>
#include <linux/personality.h>
<<<<<<< HEAD
#include <linux/freezer.h>
#include <linux/uaccess.h>
#include <linux/tracehook.h>
=======
#include <linux/uaccess.h>
#include <linux/resume_user_mode.h>
#include <linux/uprobes.h>
#include <linux/syscalls.h>
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

#include <asm/elf.h>
#include <asm/cacheflush.h>
#include <asm/traps.h>
<<<<<<< HEAD
#include <asm/ucontext.h>
#include <asm/unistd.h>
#include <asm/vfp.h>
#include "signal.h"

#define _BLOCKABLE (~(sigmask(SIGKILL) | sigmask(SIGSTOP)))

/*
 * For ARM syscalls, we encode the syscall number into the instruction.
 */
#define SWI_SYS_SIGRETURN	(0xef000000|(__NR_sigreturn)|(__NR_OABI_SYSCALL_BASE))
#define SWI_SYS_RT_SIGRETURN	(0xef000000|(__NR_rt_sigreturn)|(__NR_OABI_SYSCALL_BASE))
#define SWI_SYS_RESTART		(0xef000000|__NR_restart_syscall|__NR_OABI_SYSCALL_BASE)

/*
 * With EABI, the syscall number has to be loaded into r7.
 */
#define MOV_R7_NR_SIGRETURN	(0xe3a07000 | (__NR_sigreturn - __NR_SYSCALL_BASE))
#define MOV_R7_NR_RT_SIGRETURN	(0xe3a07000 | (__NR_rt_sigreturn - __NR_SYSCALL_BASE))

/*
 * For Thumb syscalls, we pass the syscall number via r7.  We therefore
 * need two 16-bit instructions.
 */
#define SWI_THUMB_SIGRETURN	(0xdf00 << 16 | 0x2700 | (__NR_sigreturn - __NR_SYSCALL_BASE))
#define SWI_THUMB_RT_SIGRETURN	(0xdf00 << 16 | 0x2700 | (__NR_rt_sigreturn - __NR_SYSCALL_BASE))

static const unsigned long sigreturn_codes[7] = {
	MOV_R7_NR_SIGRETURN,    SWI_SYS_SIGRETURN,    SWI_THUMB_SIGRETURN,
	MOV_R7_NR_RT_SIGRETURN, SWI_SYS_RT_SIGRETURN, SWI_THUMB_RT_SIGRETURN,
};

/*
 * Either we support OABI only, or we have EABI with the OABI
 * compat layer enabled.  In the later case we don't know if
 * user space is EABI or not, and if not we must not clobber r7.
 * Always using the OABI syscall solves that issue and works for
 * all those cases.
 */
const unsigned long syscall_restart_code[2] = {
	SWI_SYS_RESTART,	/* swi	__NR_restart_syscall */
	0xe49df004,		/* ldr	pc, [sp], #4 */
};

/*
 * atomically swap in the new signal mask, and wait for a signal.
 */
asmlinkage int sys_sigsuspend(int restart, unsigned long oldmask, old_sigset_t mask)
{
	sigset_t blocked;

	current->saved_sigmask = current->blocked;

	mask &= _BLOCKABLE;
	siginitset(&blocked, mask);
	set_current_blocked(&blocked);

	current->state = TASK_INTERRUPTIBLE;
	schedule();
	set_restore_sigmask();
	return -ERESTARTNOHAND;
}

asmlinkage int 
sys_sigaction(int sig, const struct old_sigaction __user *act,
	      struct old_sigaction __user *oact)
{
	struct k_sigaction new_ka, old_ka;
	int ret;

	if (act) {
		old_sigset_t mask;
		if (!access_ok(VERIFY_READ, act, sizeof(*act)) ||
		    __get_user(new_ka.sa.sa_handler, &act->sa_handler) ||
		    __get_user(new_ka.sa.sa_restorer, &act->sa_restorer))
			return -EFAULT;
		__get_user(new_ka.sa.sa_flags, &act->sa_flags);
		__get_user(mask, &act->sa_mask);
		siginitset(&new_ka.sa.sa_mask, mask);
	}

	ret = do_sigaction(sig, act ? &new_ka : NULL, oact ? &old_ka : NULL);

	if (!ret && oact) {
		if (!access_ok(VERIFY_WRITE, oact, sizeof(*oact)) ||
		    __put_user(old_ka.sa.sa_handler, &oact->sa_handler) ||
		    __put_user(old_ka.sa.sa_restorer, &oact->sa_restorer))
			return -EFAULT;
		__put_user(old_ka.sa.sa_flags, &oact->sa_flags);
		__put_user(old_ka.sa.sa_mask.sig[0], &oact->sa_mask);
	}

	return ret;
}

static unsigned long signal_return_offset;

#ifdef CONFIG_CRUNCH
static int preserve_crunch_context(struct crunch_sigframe __user *frame)
{
	char kbuf[sizeof(*frame) + 8];
	struct crunch_sigframe *kframe;

	/* the crunch context must be 64 bit aligned */
	kframe = (struct crunch_sigframe *)((unsigned long)(kbuf + 8) & ~7);
	kframe->magic = CRUNCH_MAGIC;
	kframe->size = CRUNCH_STORAGE_SIZE;
	crunch_task_copy(current_thread_info(), &kframe->storage);
	return __copy_to_user(frame, kframe, sizeof(*frame));
}

static int restore_crunch_context(struct crunch_sigframe __user *frame)
{
	char kbuf[sizeof(*frame) + 8];
	struct crunch_sigframe *kframe;

	/* the crunch context must be 64 bit aligned */
	kframe = (struct crunch_sigframe *)((unsigned long)(kbuf + 8) & ~7);
	if (__copy_from_user(kframe, frame, sizeof(*frame)))
		return -1;
	if (kframe->magic != CRUNCH_MAGIC ||
	    kframe->size != CRUNCH_STORAGE_SIZE)
		return -1;
	crunch_task_restore(current_thread_info(), &kframe->storage);
	return 0;
}
#endif

#ifdef CONFIG_IWMMXT

static int preserve_iwmmxt_context(struct iwmmxt_sigframe *frame)
{
	char kbuf[sizeof(*frame) + 8];
	struct iwmmxt_sigframe *kframe;

	/* the iWMMXt context must be 64 bit aligned */
	kframe = (struct iwmmxt_sigframe *)((unsigned long)(kbuf + 8) & ~7);
	kframe->magic = IWMMXT_MAGIC;
	kframe->size = IWMMXT_STORAGE_SIZE;
	iwmmxt_task_copy(current_thread_info(), &kframe->storage);
	return __copy_to_user(frame, kframe, sizeof(*frame));
}

static int restore_iwmmxt_context(struct iwmmxt_sigframe *frame)
{
=======
#include <asm/unistd.h>
#include <asm/vfp.h>
#include <asm/syscalls.h>

#include "signal.h"

extern const unsigned long sigreturn_codes[17];

static unsigned long signal_return_offset;

#ifdef CONFIG_IWMMXT

static int preserve_iwmmxt_context(struct iwmmxt_sigframe __user *frame)
{
	char kbuf[sizeof(*frame) + 8];
	struct iwmmxt_sigframe *kframe;
	int err = 0;

	/* the iWMMXt context must be 64 bit aligned */
	kframe = (struct iwmmxt_sigframe *)((unsigned long)(kbuf + 8) & ~7);

	if (test_thread_flag(TIF_USING_IWMMXT)) {
		kframe->magic = IWMMXT_MAGIC;
		kframe->size = IWMMXT_STORAGE_SIZE;
		iwmmxt_task_copy(current_thread_info(), &kframe->storage);
	} else {
		/*
		 * For bug-compatibility with older kernels, some space
		 * has to be reserved for iWMMXt even if it's not used.
		 * Set the magic and size appropriately so that properly
		 * written userspace can skip it reliably:
		 */
		*kframe = (struct iwmmxt_sigframe) {
			.magic = DUMMY_MAGIC,
			.size  = IWMMXT_STORAGE_SIZE,
		};
	}

	err = __copy_to_user(frame, kframe, sizeof(*kframe));

	return err;
}

static int restore_iwmmxt_context(char __user **auxp)
{
	struct iwmmxt_sigframe __user *frame =
		(struct iwmmxt_sigframe __user *)*auxp;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	char kbuf[sizeof(*frame) + 8];
	struct iwmmxt_sigframe *kframe;

	/* the iWMMXt context must be 64 bit aligned */
	kframe = (struct iwmmxt_sigframe *)((unsigned long)(kbuf + 8) & ~7);
	if (__copy_from_user(kframe, frame, sizeof(*frame)))
		return -1;
<<<<<<< HEAD
	if (kframe->magic != IWMMXT_MAGIC ||
	    kframe->size != IWMMXT_STORAGE_SIZE)
		return -1;
	iwmmxt_task_restore(current_thread_info(), &kframe->storage);
=======

	/*
	 * For non-iWMMXt threads: a single iwmmxt_sigframe-sized dummy
	 * block is discarded for compatibility with setup_sigframe() if
	 * present, but we don't mandate its presence.  If some other
	 * magic is here, it's not for us:
	 */
	if (!test_thread_flag(TIF_USING_IWMMXT) &&
	    kframe->magic != DUMMY_MAGIC)
		return 0;

	if (kframe->size != IWMMXT_STORAGE_SIZE)
		return -1;

	if (test_thread_flag(TIF_USING_IWMMXT)) {
		if (kframe->magic != IWMMXT_MAGIC)
			return -1;

		iwmmxt_task_restore(current_thread_info(), &kframe->storage);
	}

	*auxp += IWMMXT_STORAGE_SIZE;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	return 0;
}

#endif

#ifdef CONFIG_VFP

static int preserve_vfp_context(struct vfp_sigframe __user *frame)
{
<<<<<<< HEAD
	const unsigned long magic = VFP_MAGIC;
	const unsigned long size = VFP_STORAGE_SIZE;
	int err = 0;

	__put_user_error(magic, &frame->magic, err);
	__put_user_error(size, &frame->size, err);

	if (err)
		return -EFAULT;

	return vfp_preserve_user_clear_hwstate(&frame->ufp, &frame->ufp_exc);
}

static int restore_vfp_context(struct vfp_sigframe __user *frame)
{
	unsigned long magic;
	unsigned long size;
	int err = 0;

	__get_user_error(magic, &frame->magic, err);
	__get_user_error(size, &frame->size, err);

	if (err)
		return -EFAULT;
	if (magic != VFP_MAGIC || size != VFP_STORAGE_SIZE)
		return -EINVAL;

	return vfp_restore_user_hwstate(&frame->ufp, &frame->ufp_exc);
=======
	struct vfp_sigframe kframe;
	int err = 0;

	memset(&kframe, 0, sizeof(kframe));
	kframe.magic = VFP_MAGIC;
	kframe.size = VFP_STORAGE_SIZE;

	err = vfp_preserve_user_clear_hwstate(&kframe.ufp, &kframe.ufp_exc);
	if (err)
		return err;

	return __copy_to_user(frame, &kframe, sizeof(kframe));
}

static int restore_vfp_context(char __user **auxp)
{
	struct vfp_sigframe frame;
	int err;

	err = __copy_from_user(&frame, *auxp, sizeof(frame));
	if (err)
		return err;

	if (frame.magic != VFP_MAGIC || frame.size != VFP_STORAGE_SIZE)
		return -EINVAL;

	*auxp += sizeof(frame);
	return vfp_restore_user_hwstate(&frame.ufp, &frame.ufp_exc);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

#endif

/*
 * Do a signal return; undo the signal stack.  These are aligned to 64-bit.
 */
<<<<<<< HEAD
struct sigframe {
	struct ucontext uc;
	unsigned long retcode[2];
};

struct rt_sigframe {
	struct siginfo info;
	struct sigframe sig;
};

static int restore_sigframe(struct pt_regs *regs, struct sigframe __user *sf)
{
	struct aux_sigframe __user *aux;
=======

static int restore_sigframe(struct pt_regs *regs, struct sigframe __user *sf)
{
	struct sigcontext context;
	char __user *aux;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	sigset_t set;
	int err;

	err = __copy_from_user(&set, &sf->uc.uc_sigmask, sizeof(set));
<<<<<<< HEAD
	if (err == 0) {
		sigdelsetmask(&set, ~_BLOCKABLE);
		set_current_blocked(&set);
	}

	__get_user_error(regs->ARM_r0, &sf->uc.uc_mcontext.arm_r0, err);
	__get_user_error(regs->ARM_r1, &sf->uc.uc_mcontext.arm_r1, err);
	__get_user_error(regs->ARM_r2, &sf->uc.uc_mcontext.arm_r2, err);
	__get_user_error(regs->ARM_r3, &sf->uc.uc_mcontext.arm_r3, err);
	__get_user_error(regs->ARM_r4, &sf->uc.uc_mcontext.arm_r4, err);
	__get_user_error(regs->ARM_r5, &sf->uc.uc_mcontext.arm_r5, err);
	__get_user_error(regs->ARM_r6, &sf->uc.uc_mcontext.arm_r6, err);
	__get_user_error(regs->ARM_r7, &sf->uc.uc_mcontext.arm_r7, err);
	__get_user_error(regs->ARM_r8, &sf->uc.uc_mcontext.arm_r8, err);
	__get_user_error(regs->ARM_r9, &sf->uc.uc_mcontext.arm_r9, err);
	__get_user_error(regs->ARM_r10, &sf->uc.uc_mcontext.arm_r10, err);
	__get_user_error(regs->ARM_fp, &sf->uc.uc_mcontext.arm_fp, err);
	__get_user_error(regs->ARM_ip, &sf->uc.uc_mcontext.arm_ip, err);
	__get_user_error(regs->ARM_sp, &sf->uc.uc_mcontext.arm_sp, err);
	__get_user_error(regs->ARM_lr, &sf->uc.uc_mcontext.arm_lr, err);
	__get_user_error(regs->ARM_pc, &sf->uc.uc_mcontext.arm_pc, err);
	__get_user_error(regs->ARM_cpsr, &sf->uc.uc_mcontext.arm_cpsr, err);

	err |= !valid_user_regs(regs);

	aux = (struct aux_sigframe __user *) sf->uc.uc_regspace;
#ifdef CONFIG_CRUNCH
	if (err == 0)
		err |= restore_crunch_context(&aux->crunch);
#endif
#ifdef CONFIG_IWMMXT
	if (err == 0 && test_thread_flag(TIF_USING_IWMMXT))
		err |= restore_iwmmxt_context(&aux->iwmmxt);
#endif
#ifdef CONFIG_VFP
	if (err == 0)
		err |= restore_vfp_context(&aux->vfp);
=======
	if (err == 0)
		set_current_blocked(&set);

	err |= __copy_from_user(&context, &sf->uc.uc_mcontext, sizeof(context));
	if (err == 0) {
		regs->ARM_r0 = context.arm_r0;
		regs->ARM_r1 = context.arm_r1;
		regs->ARM_r2 = context.arm_r2;
		regs->ARM_r3 = context.arm_r3;
		regs->ARM_r4 = context.arm_r4;
		regs->ARM_r5 = context.arm_r5;
		regs->ARM_r6 = context.arm_r6;
		regs->ARM_r7 = context.arm_r7;
		regs->ARM_r8 = context.arm_r8;
		regs->ARM_r9 = context.arm_r9;
		regs->ARM_r10 = context.arm_r10;
		regs->ARM_fp = context.arm_fp;
		regs->ARM_ip = context.arm_ip;
		regs->ARM_sp = context.arm_sp;
		regs->ARM_lr = context.arm_lr;
		regs->ARM_pc = context.arm_pc;
		regs->ARM_cpsr = context.arm_cpsr;
	}

	err |= !valid_user_regs(regs);

	aux = (char __user *) sf->uc.uc_regspace;
#ifdef CONFIG_IWMMXT
	if (err == 0)
		err |= restore_iwmmxt_context(&aux);
#endif
#ifdef CONFIG_VFP
	if (err == 0)
		err |= restore_vfp_context(&aux);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#endif

	return err;
}

asmlinkage int sys_sigreturn(struct pt_regs *regs)
{
	struct sigframe __user *frame;

	/* Always make any pending restarted system calls return -EINTR */
<<<<<<< HEAD
	current_thread_info()->restart_block.fn = do_no_restart_syscall;
=======
	current->restart_block.fn = do_no_restart_syscall;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	/*
	 * Since we stacked the signal on a 64-bit boundary,
	 * then 'sp' should be word aligned here.  If it's
	 * not, then the user is trying to mess with us.
	 */
	if (regs->ARM_sp & 7)
		goto badframe;

	frame = (struct sigframe __user *)regs->ARM_sp;

<<<<<<< HEAD
	if (!access_ok(VERIFY_READ, frame, sizeof (*frame)))
=======
	if (!access_ok(frame, sizeof (*frame)))
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		goto badframe;

	if (restore_sigframe(regs, frame))
		goto badframe;

	return regs->ARM_r0;

badframe:
<<<<<<< HEAD
	force_sig(SIGSEGV, current);
=======
	force_sig(SIGSEGV);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	return 0;
}

asmlinkage int sys_rt_sigreturn(struct pt_regs *regs)
{
	struct rt_sigframe __user *frame;

	/* Always make any pending restarted system calls return -EINTR */
<<<<<<< HEAD
	current_thread_info()->restart_block.fn = do_no_restart_syscall;
=======
	current->restart_block.fn = do_no_restart_syscall;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	/*
	 * Since we stacked the signal on a 64-bit boundary,
	 * then 'sp' should be word aligned here.  If it's
	 * not, then the user is trying to mess with us.
	 */
	if (regs->ARM_sp & 7)
		goto badframe;

	frame = (struct rt_sigframe __user *)regs->ARM_sp;

<<<<<<< HEAD
	if (!access_ok(VERIFY_READ, frame, sizeof (*frame)))
=======
	if (!access_ok(frame, sizeof (*frame)))
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		goto badframe;

	if (restore_sigframe(regs, &frame->sig))
		goto badframe;

<<<<<<< HEAD
	if (do_sigaltstack(&frame->sig.uc.uc_stack, NULL, regs->ARM_sp) == -EFAULT)
=======
	if (restore_altstack(&frame->sig.uc.uc_stack))
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		goto badframe;

	return regs->ARM_r0;

badframe:
<<<<<<< HEAD
	force_sig(SIGSEGV, current);
=======
	force_sig(SIGSEGV);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	return 0;
}

static int
setup_sigframe(struct sigframe __user *sf, struct pt_regs *regs, sigset_t *set)
{
	struct aux_sigframe __user *aux;
<<<<<<< HEAD
	int err = 0;

	__put_user_error(regs->ARM_r0, &sf->uc.uc_mcontext.arm_r0, err);
	__put_user_error(regs->ARM_r1, &sf->uc.uc_mcontext.arm_r1, err);
	__put_user_error(regs->ARM_r2, &sf->uc.uc_mcontext.arm_r2, err);
	__put_user_error(regs->ARM_r3, &sf->uc.uc_mcontext.arm_r3, err);
	__put_user_error(regs->ARM_r4, &sf->uc.uc_mcontext.arm_r4, err);
	__put_user_error(regs->ARM_r5, &sf->uc.uc_mcontext.arm_r5, err);
	__put_user_error(regs->ARM_r6, &sf->uc.uc_mcontext.arm_r6, err);
	__put_user_error(regs->ARM_r7, &sf->uc.uc_mcontext.arm_r7, err);
	__put_user_error(regs->ARM_r8, &sf->uc.uc_mcontext.arm_r8, err);
	__put_user_error(regs->ARM_r9, &sf->uc.uc_mcontext.arm_r9, err);
	__put_user_error(regs->ARM_r10, &sf->uc.uc_mcontext.arm_r10, err);
	__put_user_error(regs->ARM_fp, &sf->uc.uc_mcontext.arm_fp, err);
	__put_user_error(regs->ARM_ip, &sf->uc.uc_mcontext.arm_ip, err);
	__put_user_error(regs->ARM_sp, &sf->uc.uc_mcontext.arm_sp, err);
	__put_user_error(regs->ARM_lr, &sf->uc.uc_mcontext.arm_lr, err);
	__put_user_error(regs->ARM_pc, &sf->uc.uc_mcontext.arm_pc, err);
	__put_user_error(regs->ARM_cpsr, &sf->uc.uc_mcontext.arm_cpsr, err);

	__put_user_error(current->thread.trap_no, &sf->uc.uc_mcontext.trap_no, err);
	__put_user_error(current->thread.error_code, &sf->uc.uc_mcontext.error_code, err);
	__put_user_error(current->thread.address, &sf->uc.uc_mcontext.fault_address, err);
	__put_user_error(set->sig[0], &sf->uc.uc_mcontext.oldmask, err);
=======
	struct sigcontext context;
	int err = 0;

	context = (struct sigcontext) {
		.arm_r0        = regs->ARM_r0,
		.arm_r1        = regs->ARM_r1,
		.arm_r2        = regs->ARM_r2,
		.arm_r3        = regs->ARM_r3,
		.arm_r4        = regs->ARM_r4,
		.arm_r5        = regs->ARM_r5,
		.arm_r6        = regs->ARM_r6,
		.arm_r7        = regs->ARM_r7,
		.arm_r8        = regs->ARM_r8,
		.arm_r9        = regs->ARM_r9,
		.arm_r10       = regs->ARM_r10,
		.arm_fp        = regs->ARM_fp,
		.arm_ip        = regs->ARM_ip,
		.arm_sp        = regs->ARM_sp,
		.arm_lr        = regs->ARM_lr,
		.arm_pc        = regs->ARM_pc,
		.arm_cpsr      = regs->ARM_cpsr,

		.trap_no       = current->thread.trap_no,
		.error_code    = current->thread.error_code,
		.fault_address = current->thread.address,
		.oldmask       = set->sig[0],
	};

	err |= __copy_to_user(&sf->uc.uc_mcontext, &context, sizeof(context));
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	err |= __copy_to_user(&sf->uc.uc_sigmask, set, sizeof(*set));

	aux = (struct aux_sigframe __user *) sf->uc.uc_regspace;
<<<<<<< HEAD
#ifdef CONFIG_CRUNCH
	if (err == 0)
		err |= preserve_crunch_context(&aux->crunch);
#endif
#ifdef CONFIG_IWMMXT
	if (err == 0 && test_thread_flag(TIF_USING_IWMMXT))
=======
#ifdef CONFIG_IWMMXT
	if (err == 0)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		err |= preserve_iwmmxt_context(&aux->iwmmxt);
#endif
#ifdef CONFIG_VFP
	if (err == 0)
		err |= preserve_vfp_context(&aux->vfp);
#endif
<<<<<<< HEAD
	__put_user_error(0, &aux->end_magic, err);
=======
	err |= __put_user(0, &aux->end_magic);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	return err;
}

static inline void __user *
<<<<<<< HEAD
get_sigframe(struct k_sigaction *ka, struct pt_regs *regs, int framesize)
{
	unsigned long sp = regs->ARM_sp;
	void __user *frame;

	/*
	 * This is the X/Open sanctioned signal stack switching.
	 */
	if ((ka->sa.sa_flags & SA_ONSTACK) && !sas_ss_flags(sp))
		sp = current->sas_ss_sp + current->sas_ss_size;

	/*
=======
get_sigframe(struct ksignal *ksig, struct pt_regs *regs, int framesize)
{
	unsigned long sp = sigsp(regs->ARM_sp, ksig);
	void __user *frame;

	/*
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	 * ATPCS B01 mandates 8-byte alignment
	 */
	frame = (void __user *)((sp - framesize) & ~7);

	/*
	 * Check that we can actually write to the signal frame.
	 */
<<<<<<< HEAD
	if (!access_ok(VERIFY_WRITE, frame, framesize))
=======
	if (!access_ok(frame, framesize))
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		frame = NULL;

	return frame;
}

static int
<<<<<<< HEAD
setup_return(struct pt_regs *regs, struct k_sigaction *ka,
	     unsigned long __user *rc, void __user *frame, int usig)
{
	unsigned long handler = (unsigned long)ka->sa.sa_handler;
	unsigned long retcode;
	int thumb = 0;
	unsigned long cpsr = regs->ARM_cpsr & ~(PSR_f | PSR_E_BIT);
=======
setup_return(struct pt_regs *regs, struct ksignal *ksig,
	     unsigned long __user *rc, void __user *frame)
{
	unsigned long handler = (unsigned long)ksig->ka.sa.sa_handler;
	unsigned long handler_fdpic_GOT = 0;
	unsigned long retcode;
	unsigned int idx, thumb = 0;
	unsigned long cpsr = regs->ARM_cpsr & ~(PSR_f | PSR_E_BIT);
	bool fdpic = IS_ENABLED(CONFIG_BINFMT_ELF_FDPIC) &&
		     (current->personality & FDPIC_FUNCPTRS);

	if (fdpic) {
		unsigned long __user *fdpic_func_desc =
					(unsigned long __user *)handler;
		if (__get_user(handler, &fdpic_func_desc[0]) ||
		    __get_user(handler_fdpic_GOT, &fdpic_func_desc[1]))
			return 1;
	}
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	cpsr |= PSR_ENDSTATE;

	/*
	 * Maybe we need to deliver a 32-bit signal to a 26-bit task.
	 */
<<<<<<< HEAD
	if (ka->sa.sa_flags & SA_THIRTYTWO)
=======
	if (ksig->ka.sa.sa_flags & SA_THIRTYTWO)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		cpsr = (cpsr & ~MODE_MASK) | USR_MODE;

#ifdef CONFIG_ARM_THUMB
	if (elf_hwcap & HWCAP_THUMB) {
		/*
		 * The LSB of the handler determines if we're going to
		 * be using THUMB or ARM mode for this signal handler.
		 */
		thumb = handler & 1;

<<<<<<< HEAD
#if __LINUX_ARM_ARCH__ >= 6
=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		/*
		 * Clear the If-Then Thumb-2 execution state.  ARM spec
		 * requires this to be all 000s in ARM mode.  Snapdragon
		 * S4/Krait misbehaves on a Thumb=>ARM signal transition
		 * without this.
		 *
		 * We must do this whenever we are running on a Thumb-2
		 * capable CPU, which includes ARMv6T2.  However, we elect
<<<<<<< HEAD
		 * to do this whenever we're on an ARMv6 or later CPU for
		 * simplicity.
		 */
		cpsr &= ~PSR_IT_MASK;
#endif
=======
		 * to always do this to simplify the code; this field is
		 * marked UNK/SBZP for older architectures.
		 */
		cpsr &= ~PSR_IT_MASK;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

		if (thumb) {
			cpsr |= PSR_T_BIT;
		} else
			cpsr &= ~PSR_T_BIT;
	}
#endif

<<<<<<< HEAD
	if (ka->sa.sa_flags & SA_RESTORER) {
		retcode = (unsigned long)ka->sa.sa_restorer;
	} else {
		unsigned int idx = thumb << 1;

		if (ka->sa.sa_flags & SA_SIGINFO)
			idx += 3;

=======
	if (ksig->ka.sa.sa_flags & SA_RESTORER) {
		retcode = (unsigned long)ksig->ka.sa.sa_restorer;
		if (fdpic) {
			/*
			 * We need code to load the function descriptor.
			 * That code follows the standard sigreturn code
			 * (6 words), and is made of 3 + 2 words for each
			 * variant. The 4th copied word is the actual FD
			 * address that the assembly code expects.
			 */
			idx = 6 + thumb * 3;
			if (ksig->ka.sa.sa_flags & SA_SIGINFO)
				idx += 5;
			if (__put_user(sigreturn_codes[idx],   rc  ) ||
			    __put_user(sigreturn_codes[idx+1], rc+1) ||
			    __put_user(sigreturn_codes[idx+2], rc+2) ||
			    __put_user(retcode,                rc+3))
				return 1;
			goto rc_finish;
		}
	} else {
		idx = thumb << 1;
		if (ksig->ka.sa.sa_flags & SA_SIGINFO)
			idx += 3;

		/*
		 * Put the sigreturn code on the stack no matter which return
		 * mechanism we use in order to remain ABI compliant
		 */
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		if (__put_user(sigreturn_codes[idx],   rc) ||
		    __put_user(sigreturn_codes[idx+1], rc+1))
			return 1;

<<<<<<< HEAD
=======
rc_finish:
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#ifdef CONFIG_MMU
		if (cpsr & MODE32_BIT) {
			struct mm_struct *mm = current->mm;

			/*
			 * 32-bit code can use the signal return page
			 * except when the MPU has protected the vectors
			 * page from PL0
			 */
			retcode = mm->context.sigpage + signal_return_offset +
				  (idx << 2) + thumb;
		} else
#endif
		{
			/*
			 * Ensure that the instruction cache sees
			 * the return code written onto the stack.
			 */
			flush_icache_range((unsigned long)rc,
<<<<<<< HEAD
					   (unsigned long)(rc + 2));
=======
					   (unsigned long)(rc + 3));
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

			retcode = ((unsigned long)rc) + thumb;
		}
	}

<<<<<<< HEAD
	regs->ARM_r0 = usig;
	regs->ARM_sp = (unsigned long)frame;
	regs->ARM_lr = retcode;
	regs->ARM_pc = handler;
=======
	regs->ARM_r0 = ksig->sig;
	regs->ARM_sp = (unsigned long)frame;
	regs->ARM_lr = retcode;
	regs->ARM_pc = handler;
	if (fdpic)
		regs->ARM_r9 = handler_fdpic_GOT;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	regs->ARM_cpsr = cpsr;

	return 0;
}

static int
<<<<<<< HEAD
setup_frame(int usig, struct k_sigaction *ka, sigset_t *set, struct pt_regs *regs)
{
	struct sigframe __user *frame = get_sigframe(ka, regs, sizeof(*frame));
=======
setup_frame(struct ksignal *ksig, sigset_t *set, struct pt_regs *regs)
{
	struct sigframe __user *frame = get_sigframe(ksig, regs, sizeof(*frame));
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	int err = 0;

	if (!frame)
		return 1;

	/*
	 * Set uc.uc_flags to a value which sc.trap_no would never have.
	 */
<<<<<<< HEAD
	__put_user_error(0x5ac3c35a, &frame->uc.uc_flags, err);

	err |= setup_sigframe(frame, regs, set);
	if (err == 0)
		err = setup_return(regs, ka, frame->retcode, frame, usig);
=======
	err = __put_user(0x5ac3c35a, &frame->uc.uc_flags);

	err |= setup_sigframe(frame, regs, set);
	if (err == 0)
		err = setup_return(regs, ksig, frame->retcode, frame);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	return err;
}

static int
<<<<<<< HEAD
setup_rt_frame(int usig, struct k_sigaction *ka, siginfo_t *info,
	       sigset_t *set, struct pt_regs *regs)
{
	struct rt_sigframe __user *frame = get_sigframe(ka, regs, sizeof(*frame));
	stack_t stack;
=======
setup_rt_frame(struct ksignal *ksig, sigset_t *set, struct pt_regs *regs)
{
	struct rt_sigframe __user *frame = get_sigframe(ksig, regs, sizeof(*frame));
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	int err = 0;

	if (!frame)
		return 1;

<<<<<<< HEAD
	err |= copy_siginfo_to_user(&frame->info, info);

	__put_user_error(0, &frame->sig.uc.uc_flags, err);
	__put_user_error(NULL, &frame->sig.uc.uc_link, err);

	memset(&stack, 0, sizeof(stack));
	stack.ss_sp = (void __user *)current->sas_ss_sp;
	stack.ss_flags = sas_ss_flags(regs->ARM_sp);
	stack.ss_size = current->sas_ss_size;
	err |= __copy_to_user(&frame->sig.uc.uc_stack, &stack, sizeof(stack));

	err |= setup_sigframe(&frame->sig, regs, set);
	if (err == 0)
		err = setup_return(regs, ka, frame->sig.retcode, frame, usig);
=======
	err |= copy_siginfo_to_user(&frame->info, &ksig->info);

	err |= __put_user(0, &frame->sig.uc.uc_flags);
	err |= __put_user(NULL, &frame->sig.uc.uc_link);

	err |= __save_altstack(&frame->sig.uc.uc_stack, regs->ARM_sp);
	err |= setup_sigframe(&frame->sig, regs, set);
	if (err == 0)
		err = setup_return(regs, ksig, frame->sig.retcode, frame);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	if (err == 0) {
		/*
		 * For realtime signals we must also set the second and third
		 * arguments for the signal handler.
		 *   -- Peter Maydell <pmaydell@chiark.greenend.org.uk> 2000-12-06
		 */
		regs->ARM_r1 = (unsigned long)&frame->info;
		regs->ARM_r2 = (unsigned long)&frame->sig.uc;
	}

	return err;
}

/*
 * OK, we're invoking a handler
 */	
<<<<<<< HEAD
static int
handle_signal(unsigned long sig, struct k_sigaction *ka,
	      siginfo_t *info, sigset_t *oldset,
	      struct pt_regs * regs)
{
	struct thread_info *thread = current_thread_info();
	struct task_struct *tsk = current;
	int usig = sig;
	int ret;

	/*
	 * translate the signal
	 */
	if (usig < 32 && thread->exec_domain && thread->exec_domain->signal_invmap)
		usig = thread->exec_domain->signal_invmap[usig];
=======
static void handle_signal(struct ksignal *ksig, struct pt_regs *regs)
{
	sigset_t *oldset = sigmask_to_save();
	int ret;

	/*
	 * Perform fixup for the pre-signal frame.
	 */
	rseq_signal_deliver(ksig, regs);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	/*
	 * Set up the stack frame
	 */
<<<<<<< HEAD
	if (ka->sa.sa_flags & SA_SIGINFO)
		ret = setup_rt_frame(usig, ka, info, oldset, regs);
	else
		ret = setup_frame(usig, ka, oldset, regs);
=======
	if (ksig->ka.sa.sa_flags & SA_SIGINFO)
		ret = setup_rt_frame(ksig, oldset, regs);
	else
		ret = setup_frame(ksig, oldset, regs);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	/*
	 * Check that the resulting registers are actually sane.
	 */
	ret |= !valid_user_regs(regs);

<<<<<<< HEAD
	if (ret != 0) {
		force_sigsegv(sig, tsk);
		return ret;
	}

	/*
	 * Block the signal if we were successful.
	 */
	block_sigmask(ka, sig);

	return 0;
=======
	signal_setup_done(ret, ksig, 0);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

/*
 * Note that 'init' is a special process: it doesn't get signals it doesn't
 * want to handle. Thus you cannot kill init even with a SIGKILL even by
 * mistake.
 *
 * Note that we go through the signals twice: once to check the signals that
 * the kernel can handle, and then we build all the user-level signal handling
 * stack-frames in one go after that.
 */
<<<<<<< HEAD
static void do_signal(struct pt_regs *regs, int syscall)
{
	unsigned int retval = 0, continue_addr = 0, restart_addr = 0;
	struct k_sigaction ka;
	siginfo_t info;
	int signr;

	/*
	 * We want the common case to go fast, which
	 * is why we may in certain cases get here from
	 * kernel mode. Just return without doing anything
	 * if so.
	 */
	if (!user_mode(regs))
		return;
=======
static int do_signal(struct pt_regs *regs, int syscall)
{
	unsigned int retval = 0, continue_addr = 0, restart_addr = 0;
	struct ksignal ksig;
	int restart = 0;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	/*
	 * If we were from a system call, check for system call restarting...
	 */
	if (syscall) {
		continue_addr = regs->ARM_pc;
		restart_addr = continue_addr - (thumb_mode(regs) ? 2 : 4);
		retval = regs->ARM_r0;

		/*
		 * Prepare for system call restart.  We do this here so that a
		 * debugger will see the already changed PSW.
		 */
		switch (retval) {
<<<<<<< HEAD
		case -ERESTARTNOHAND:
		case -ERESTARTSYS:
		case -ERESTARTNOINTR:
		case -ERESTART_RESTARTBLOCK:
=======
		case -ERESTART_RESTARTBLOCK:
			restart -= 2;
			fallthrough;
		case -ERESTARTNOHAND:
		case -ERESTARTSYS:
		case -ERESTARTNOINTR:
			restart++;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
			regs->ARM_r0 = regs->ARM_ORIG_r0;
			regs->ARM_pc = restart_addr;
			break;
		}
	}

<<<<<<< HEAD
	if (try_to_freeze())
		goto no_signal;

=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	/*
	 * Get the signal to deliver.  When running under ptrace, at this
	 * point the debugger may change all our registers ...
	 */
<<<<<<< HEAD
	signr = get_signal_to_deliver(&info, &ka, regs, NULL);
	if (signr > 0) {
		sigset_t *oldset;

		/*
		 * Depending on the signal settings we may need to revert the
		 * decision to restart the system call.  But skip this if a
		 * debugger has chosen to restart at a different PC.
		 */
		if (regs->ARM_pc == restart_addr) {
			if (retval == -ERESTARTNOHAND ||
			    retval == -ERESTART_RESTARTBLOCK
			    || (retval == -ERESTARTSYS
				&& !(ka.sa.sa_flags & SA_RESTART))) {
				regs->ARM_r0 = -EINTR;
				regs->ARM_pc = continue_addr;
			}
			clear_thread_flag(TIF_SYSCALL_RESTARTSYS);
		}

		if (test_thread_flag(TIF_RESTORE_SIGMASK))
			oldset = &current->saved_sigmask;
		else
			oldset = &current->blocked;
		if (handle_signal(signr, &ka, &info, oldset, regs) == 0) {
			/*
			 * A signal was successfully delivered; the saved
			 * sigmask will have been stored in the signal frame,
			 * and will be restored by sigreturn, so we can simply
			 * clear the TIF_RESTORE_SIGMASK flag.
			 */
			if (test_thread_flag(TIF_RESTORE_SIGMASK))
				clear_thread_flag(TIF_RESTORE_SIGMASK);
		}
		return;
	}

 no_signal:
	if (syscall) {
		/*
		 * Handle restarting a different system call.  As above,
		 * if a debugger has chosen to restart at a different PC,
		 * ignore the restart.
		 */
		if (retval == -ERESTART_RESTARTBLOCK
		    && regs->ARM_pc == restart_addr)
			set_thread_flag(TIF_SYSCALL_RESTARTSYS);
	}

	/* If there's no signal to deliver, we just put the saved sigmask
	 * back.
	 */
	if (test_and_clear_thread_flag(TIF_RESTORE_SIGMASK))
		set_current_blocked(&current->saved_sigmask);
}

asmlinkage void
do_notify_resume(struct pt_regs *regs, unsigned int thread_flags, int syscall)
{
	if (thread_flags & _TIF_SIGPENDING)
		do_signal(regs, syscall);

	if (thread_flags & _TIF_NOTIFY_RESUME) {
		clear_thread_flag(TIF_NOTIFY_RESUME);
		tracehook_notify_resume(regs);
		if (current->replacement_session_keyring)
			key_replace_session_keyring();
	}
=======
	/*
	 * Depending on the signal settings we may need to revert the
	 * decision to restart the system call.  But skip this if a
	 * debugger has chosen to restart at a different PC.
	 */
	if (get_signal(&ksig)) {
		/* handler */
		if (unlikely(restart) && regs->ARM_pc == restart_addr) {
			if (retval == -ERESTARTNOHAND ||
			    retval == -ERESTART_RESTARTBLOCK
			    || (retval == -ERESTARTSYS
				&& !(ksig.ka.sa.sa_flags & SA_RESTART))) {
				regs->ARM_r0 = -EINTR;
				regs->ARM_pc = continue_addr;
			}
		}
		handle_signal(&ksig, regs);
	} else {
		/* no handler */
		restore_saved_sigmask();
		if (unlikely(restart) && regs->ARM_pc == restart_addr) {
			regs->ARM_pc = continue_addr;
			return restart;
		}
	}
	return 0;
}

asmlinkage int
do_work_pending(struct pt_regs *regs, unsigned int thread_flags, int syscall)
{
	/*
	 * The assembly code enters us with IRQs off, but it hasn't
	 * informed the tracing code of that for efficiency reasons.
	 * Update the trace code with the current status.
	 */
	trace_hardirqs_off();
	do {
		if (likely(thread_flags & _TIF_NEED_RESCHED)) {
			schedule();
		} else {
			if (unlikely(!user_mode(regs)))
				return 0;
			local_irq_enable();
			if (thread_flags & (_TIF_SIGPENDING | _TIF_NOTIFY_SIGNAL)) {
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
			} else if (thread_flags & _TIF_UPROBE) {
				uprobe_notify_resume(regs);
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

struct page *get_signal_page(void)
{
	unsigned long ptr;
	unsigned offset;
	struct page *page;
	void *addr;

	page = alloc_pages(GFP_KERNEL, 0);

	if (!page)
		return NULL;

	addr = page_address(page);

<<<<<<< HEAD
	/* Give the signal return code some randomness */
	offset = 0x200 + (get_random_int() & 0x7fc);
	signal_return_offset = offset;

	/*
	 * Copy signal return handlers into the vector page, and
	 * set sigreturn to be a pointer to these.
	 */
	memcpy(addr + offset, sigreturn_codes, sizeof(sigreturn_codes));

	ptr = (unsigned long)addr + offset;
	flush_icache_range(ptr, ptr + sizeof(sigreturn_codes));

	return page;
}
=======
	/* Poison the entire page */
	memset32(addr, __opcode_to_mem_arm(0xe7fddef1),
		 PAGE_SIZE / sizeof(u32));

	/* Give the signal return code some randomness */
	offset = 0x200 + (get_random_u16() & 0x7fc);
	signal_return_offset = offset;

	/* Copy signal return handlers into the page */
	memcpy(addr + offset, sigreturn_codes, sizeof(sigreturn_codes));

	/* Flush out all instructions in this page */
	ptr = (unsigned long)addr;
	flush_icache_range(ptr, ptr + PAGE_SIZE);

	return page;
}

#ifdef CONFIG_DEBUG_RSEQ
asmlinkage void do_rseq_syscall(struct pt_regs *regs)
{
	rseq_syscall(regs);
}
#endif

/*
 * Compile-time assertions for siginfo_t offsets. Check NSIG* as well, as
 * changes likely come with new fields that should be added below.
 */
static_assert(NSIGILL	== 11);
static_assert(NSIGFPE	== 15);
static_assert(NSIGSEGV	== 10);
static_assert(NSIGBUS	== 5);
static_assert(NSIGTRAP	== 6);
static_assert(NSIGCHLD	== 6);
static_assert(NSIGSYS	== 2);
static_assert(sizeof(siginfo_t) == 128);
static_assert(__alignof__(siginfo_t) == 4);
static_assert(offsetof(siginfo_t, si_signo)	== 0x00);
static_assert(offsetof(siginfo_t, si_errno)	== 0x04);
static_assert(offsetof(siginfo_t, si_code)	== 0x08);
static_assert(offsetof(siginfo_t, si_pid)	== 0x0c);
static_assert(offsetof(siginfo_t, si_uid)	== 0x10);
static_assert(offsetof(siginfo_t, si_tid)	== 0x0c);
static_assert(offsetof(siginfo_t, si_overrun)	== 0x10);
static_assert(offsetof(siginfo_t, si_status)	== 0x14);
static_assert(offsetof(siginfo_t, si_utime)	== 0x18);
static_assert(offsetof(siginfo_t, si_stime)	== 0x1c);
static_assert(offsetof(siginfo_t, si_value)	== 0x14);
static_assert(offsetof(siginfo_t, si_int)	== 0x14);
static_assert(offsetof(siginfo_t, si_ptr)	== 0x14);
static_assert(offsetof(siginfo_t, si_addr)	== 0x0c);
static_assert(offsetof(siginfo_t, si_addr_lsb)	== 0x10);
static_assert(offsetof(siginfo_t, si_lower)	== 0x14);
static_assert(offsetof(siginfo_t, si_upper)	== 0x18);
static_assert(offsetof(siginfo_t, si_pkey)	== 0x14);
static_assert(offsetof(siginfo_t, si_perf_data)	== 0x10);
static_assert(offsetof(siginfo_t, si_perf_type)	== 0x14);
static_assert(offsetof(siginfo_t, si_perf_flags) == 0x18);
static_assert(offsetof(siginfo_t, si_band)	== 0x0c);
static_assert(offsetof(siginfo_t, si_fd)	== 0x10);
static_assert(offsetof(siginfo_t, si_call_addr)	== 0x0c);
static_assert(offsetof(siginfo_t, si_syscall)	== 0x10);
static_assert(offsetof(siginfo_t, si_arch)	== 0x14);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
