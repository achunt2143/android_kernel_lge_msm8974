<<<<<<< HEAD
/*
 *  linux/arch/parisc/kernel/signal.c: Architecture-specific signal
 *  handling support.
 *
 *  Copyright (C) 2000 David Huggins-Daines <dhd@debian.org>
 *  Copyright (C) 2000 Linuxcare, Inc.
 *
 *  Based on the ia64, i386, and alpha versions.
 *
 *  Like the IA-64, we are a recent enough port (we are *starting*
 *  with glibc2.2) that we do not need to support the old non-realtime
 *  Linux signals.  Therefore we don't.  HP/UX signals will go in
 *  arch/parisc/hpux/signal.c when we figure out how to do them.
 */

#include <linux/sched.h>
=======
// SPDX-License-Identifier: GPL-2.0
/*
 *  PA-RISC architecture-specific signal handling support.
 *
 *  Copyright (C) 2000 David Huggins-Daines <dhd@debian.org>
 *  Copyright (C) 2000 Linuxcare, Inc.
 *  Copyright (C) 2000-2022 Helge Deller <deller@gmx.de>
 *  Copyright (C) 2022 John David Anglin <dave.anglin@bell.net>
 *
 *  Based on the ia64, i386, and alpha versions.
 */

#include <linux/sched.h>
#include <linux/sched/debug.h>
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#include <linux/mm.h>
#include <linux/smp.h>
#include <linux/kernel.h>
#include <linux/signal.h>
#include <linux/errno.h>
#include <linux/wait.h>
#include <linux/ptrace.h>
<<<<<<< HEAD
#include <linux/tracehook.h>
=======
#include <linux/resume_user_mode.h>
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#include <linux/unistd.h>
#include <linux/stddef.h>
#include <linux/compat.h>
#include <linux/elf.h>
#include <asm/ucontext.h>
#include <asm/rt_sigframe.h>
<<<<<<< HEAD
#include <asm/uaccess.h>
#include <asm/pgalloc.h>
#include <asm/cacheflush.h>
#include <asm/asm-offsets.h>
=======
#include <linux/uaccess.h>
#include <asm/cacheflush.h>
#include <asm/asm-offsets.h>
#include <asm/vdso.h>
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

#ifdef CONFIG_COMPAT
#include "signal32.h"
#endif

#define DEBUG_SIG 0 
#define DEBUG_SIG_LEVEL 2

#if DEBUG_SIG
#define DBG(LEVEL, ...) \
        ((DEBUG_SIG_LEVEL >= LEVEL) \
	? printk(__VA_ARGS__) : (void) 0)
#else
#define DBG(LEVEL, ...)
#endif
	
<<<<<<< HEAD

#define _BLOCKABLE (~(sigmask(SIGKILL) | sigmask(SIGSTOP)))

=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
/* gcc will complain if a pointer is cast to an integer of different
 * size.  If you really need to do this (and we do for an ELF32 user
 * application in an ELF64 kernel) then you have to do a cast to an
 * integer of the same size first.  The A() macro accomplishes
 * this. */
#define A(__x)	((unsigned long)(__x))

/*
<<<<<<< HEAD
 * Atomically swap in the new signal mask, and wait for a signal.
 */
#ifdef CONFIG_64BIT
#include "sys32.h"
#endif

/*
 * Do a signal return - restore sigcontext.
 */

/* Trampoline for calling rt_sigreturn() */
#define INSN_LDI_R25_0	 0x34190000 /* ldi  0,%r25 (in_syscall=0) */
#define INSN_LDI_R25_1	 0x34190002 /* ldi  1,%r25 (in_syscall=1) */
#define INSN_LDI_R20	 0x3414015a /* ldi  __NR_rt_sigreturn,%r20 */
#define INSN_BLE_SR2_R0  0xe4008200 /* be,l 0x100(%sr2,%r0),%sr0,%r31 */
#define INSN_NOP	 0x08000240 /* nop */
/* For debugging */
#define INSN_DIE_HORRIBLY 0x68000ccc /* stw %r0,0x666(%sr0,%r0) */

=======
 * Do a signal return - restore sigcontext.
 */

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
static long
restore_sigcontext(struct sigcontext __user *sc, struct pt_regs *regs)
{
	long err = 0;

	err |= __copy_from_user(regs->gr, sc->sc_gr, sizeof(regs->gr));
	err |= __copy_from_user(regs->fr, sc->sc_fr, sizeof(regs->fr));
	err |= __copy_from_user(regs->iaoq, sc->sc_iaoq, sizeof(regs->iaoq));
	err |= __copy_from_user(regs->iasq, sc->sc_iasq, sizeof(regs->iasq));
	err |= __get_user(regs->sar, &sc->sc_sar);
<<<<<<< HEAD
	DBG(2,"restore_sigcontext: iaoq is 0x%#lx / 0x%#lx\n", 
			regs->iaoq[0],regs->iaoq[1]);
	DBG(2,"restore_sigcontext: r28 is %ld\n", regs->gr[28]);
	return err;
}

void
=======
	DBG(2, "%s: iaoq is %#lx / %#lx\n",
			__func__, regs->iaoq[0], regs->iaoq[1]);
	DBG(2, "%s: r28 is %ld\n", __func__, regs->gr[28]);
	return err;
}

asmlinkage void
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
sys_rt_sigreturn(struct pt_regs *regs, int in_syscall)
{
	struct rt_sigframe __user *frame;
	sigset_t set;
	unsigned long usp = (regs->gr[30] & ~(0x01UL));
	unsigned long sigframe_size = PARISC_RT_SIGFRAME_SIZE;
#ifdef CONFIG_64BIT
<<<<<<< HEAD
	compat_sigset_t compat_set;
=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	struct compat_rt_sigframe __user * compat_frame;
	
	if (is_compat_task())
		sigframe_size = PARISC_RT_SIGFRAME_SIZE32;
#endif

<<<<<<< HEAD
=======
	current->restart_block.fn = do_no_restart_syscall;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	/* Unwind the user stack to get the rt_sigframe structure. */
	frame = (struct rt_sigframe __user *)
		(usp - sigframe_size);
<<<<<<< HEAD
	DBG(2,"sys_rt_sigreturn: frame is %p\n", frame);
=======
	DBG(2, "%s: frame is %p pid %d\n", __func__, frame, task_pid_nr(current));

	regs->orig_r28 = 1; /* no restarts for sigreturn */
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

#ifdef CONFIG_64BIT
	compat_frame = (struct compat_rt_sigframe __user *)frame;
	
	if (is_compat_task()) {
<<<<<<< HEAD
		DBG(2,"sys_rt_sigreturn: ELF32 process.\n");
		if (__copy_from_user(&compat_set, &compat_frame->uc.uc_sigmask, sizeof(compat_set)))
			goto give_sigsegv;
		sigset_32to64(&set,&compat_set);
=======
		if (get_compat_sigset(&set, &compat_frame->uc.uc_sigmask))
			goto give_sigsegv;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	} else
#endif
	{
		if (__copy_from_user(&set, &frame->uc.uc_sigmask, sizeof(set)))
			goto give_sigsegv;
	}
		
<<<<<<< HEAD
	sigdelsetmask(&set, ~_BLOCKABLE);
	spin_lock_irq(&current->sighand->siglock);
	current->blocked = set;
	recalc_sigpending();
	spin_unlock_irq(&current->sighand->siglock);
=======
	set_current_blocked(&set);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	/* Good thing we saved the old gr[30], eh? */
#ifdef CONFIG_64BIT
	if (is_compat_task()) {
<<<<<<< HEAD
		DBG(1,"sys_rt_sigreturn: compat_frame->uc.uc_mcontext 0x%p\n",
				&compat_frame->uc.uc_mcontext);
=======
		DBG(1, "%s: compat_frame->uc.uc_mcontext 0x%p\n",
				__func__, &compat_frame->uc.uc_mcontext);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
// FIXME: Load upper half from register file
		if (restore_sigcontext32(&compat_frame->uc.uc_mcontext, 
					&compat_frame->regs, regs))
			goto give_sigsegv;
<<<<<<< HEAD
		DBG(1,"sys_rt_sigreturn: usp %#08lx stack 0x%p\n", 
				usp, &compat_frame->uc.uc_stack);
		if (do_sigaltstack32(&compat_frame->uc.uc_stack, NULL, usp) == -EFAULT)
=======
		DBG(1, "%s: usp %#08lx stack 0x%p\n",
				__func__, usp, &compat_frame->uc.uc_stack);
		if (compat_restore_altstack(&compat_frame->uc.uc_stack))
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
			goto give_sigsegv;
	} else
#endif
	{
<<<<<<< HEAD
		DBG(1,"sys_rt_sigreturn: frame->uc.uc_mcontext 0x%p\n",
				&frame->uc.uc_mcontext);
		if (restore_sigcontext(&frame->uc.uc_mcontext, regs))
			goto give_sigsegv;
		DBG(1,"sys_rt_sigreturn: usp %#08lx stack 0x%p\n", 
				usp, &frame->uc.uc_stack);
		if (do_sigaltstack(&frame->uc.uc_stack, NULL, usp) == -EFAULT)
=======
		DBG(1, "%s: frame->uc.uc_mcontext 0x%p\n",
				__func__, &frame->uc.uc_mcontext);
		if (restore_sigcontext(&frame->uc.uc_mcontext, regs))
			goto give_sigsegv;
		DBG(1, "%s: usp %#08lx stack 0x%p\n",
				__func__, usp, &frame->uc.uc_stack);
		if (restore_altstack(&frame->uc.uc_stack))
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
			goto give_sigsegv;
	}
		


	/* If we are on the syscall path IAOQ will not be restored, and
	 * if we are on the interrupt path we must not corrupt gr31.
	 */
	if (in_syscall)
		regs->gr[31] = regs->iaoq[0];
<<<<<<< HEAD
#if DEBUG_SIG
	DBG(1,"sys_rt_sigreturn: returning to %#lx, DUMPING REGS:\n", regs->iaoq[0]);
	show_regs(regs);
#endif
	return;

give_sigsegv:
	DBG(1,"sys_rt_sigreturn: Sending SIGSEGV\n");
	force_sig(SIGSEGV, current);
=======

	return;

give_sigsegv:
	DBG(1, "%s: Sending SIGSEGV\n", __func__);
	force_sig(SIGSEGV);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	return;
}

/*
 * Set up a signal frame.
 */

static inline void __user *
get_sigframe(struct k_sigaction *ka, unsigned long sp, size_t frame_size)
{
	/*FIXME: ELF32 vs. ELF64 has different frame_size, but since we
	  don't use the parameter it doesn't matter */

<<<<<<< HEAD
	DBG(1,"get_sigframe: ka = %#lx, sp = %#lx, frame_size = %#lx\n",
			(unsigned long)ka, sp, frame_size);
	
	if ((ka->sa.sa_flags & SA_ONSTACK) != 0 && ! sas_ss_flags(sp))
		sp = current->sas_ss_sp; /* Stacks grow up! */

	DBG(1,"get_sigframe: Returning sp = %#lx\n", (unsigned long)sp);
=======
	DBG(1, "%s: ka = %#lx, sp = %#lx, frame_size = %zu\n",
			__func__, (unsigned long)ka, sp, frame_size);
	
	/* Align alternate stack and reserve 64 bytes for the signal
	   handler's frame marker.  */
	if ((ka->sa.sa_flags & SA_ONSTACK) != 0 && ! sas_ss_flags(sp))
		sp = (current->sas_ss_sp + 0x7f) & ~0x3f; /* Stacks grow up! */

	DBG(1, "%s: Returning sp = %#lx\n", __func__, (unsigned long)sp);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	return (void __user *) sp; /* Stacks grow up.  Fun. */
}

static long
<<<<<<< HEAD
setup_sigcontext(struct sigcontext __user *sc, struct pt_regs *regs, int in_syscall)
=======
setup_sigcontext(struct sigcontext __user *sc, struct pt_regs *regs, long in_syscall)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		 
{
	unsigned long flags = 0;
	long err = 0;

	if (on_sig_stack((unsigned long) sc))
		flags |= PARISC_SC_FLAG_ONSTACK;
	if (in_syscall) {
		flags |= PARISC_SC_FLAG_IN_SYSCALL;
		/* regs->iaoq is undefined in the syscall return path */
		err |= __put_user(regs->gr[31], &sc->sc_iaoq[0]);
		err |= __put_user(regs->gr[31]+4, &sc->sc_iaoq[1]);
		err |= __put_user(regs->sr[3], &sc->sc_iasq[0]);
		err |= __put_user(regs->sr[3], &sc->sc_iasq[1]);
<<<<<<< HEAD
		DBG(1,"setup_sigcontext: iaoq %#lx / %#lx (in syscall)\n",
			regs->gr[31], regs->gr[31]+4);
	} else {
		err |= __copy_to_user(sc->sc_iaoq, regs->iaoq, sizeof(regs->iaoq));
		err |= __copy_to_user(sc->sc_iasq, regs->iasq, sizeof(regs->iasq));
		DBG(1,"setup_sigcontext: iaoq %#lx / %#lx (not in syscall)\n", 
			regs->iaoq[0], regs->iaoq[1]);
=======
		DBG(1, "%s: iaoq %#lx / %#lx (in syscall)\n",
			__func__, regs->gr[31], regs->gr[31]+4);
	} else {
		err |= __copy_to_user(sc->sc_iaoq, regs->iaoq, sizeof(regs->iaoq));
		err |= __copy_to_user(sc->sc_iasq, regs->iasq, sizeof(regs->iasq));
		DBG(1, "%s: iaoq %#lx / %#lx (not in syscall)\n",
			__func__, regs->iaoq[0], regs->iaoq[1]);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	}

	err |= __put_user(flags, &sc->sc_flags);
	err |= __copy_to_user(sc->sc_gr, regs->gr, sizeof(regs->gr));
	err |= __copy_to_user(sc->sc_fr, regs->fr, sizeof(regs->fr));
	err |= __put_user(regs->sar, &sc->sc_sar);
<<<<<<< HEAD
	DBG(1,"setup_sigcontext: r28 is %ld\n", regs->gr[28]);
=======
	DBG(1, "%s: r28 is %ld\n", __func__, regs->gr[28]);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	return err;
}

static long
<<<<<<< HEAD
setup_rt_frame(int sig, struct k_sigaction *ka, siginfo_t *info,
	       sigset_t *set, struct pt_regs *regs, int in_syscall)
=======
setup_rt_frame(struct ksignal *ksig, sigset_t *set, struct pt_regs *regs,
	       long in_syscall)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
{
	struct rt_sigframe __user *frame;
	unsigned long rp, usp;
	unsigned long haddr, sigframe_size;
<<<<<<< HEAD
	int err = 0;
#ifdef CONFIG_64BIT
	compat_int_t compat_val;
	struct compat_rt_sigframe __user * compat_frame;
	compat_sigset_t compat_set;
#endif
	
	usp = (regs->gr[30] & ~(0x01UL));
	/*FIXME: frame_size parameter is unused, remove it. */
	frame = get_sigframe(ka, usp, sizeof(*frame));

	DBG(1,"SETUP_RT_FRAME: START\n");
	DBG(1,"setup_rt_frame: frame %p info %p\n", frame, info);

=======
	unsigned long start;
	int err = 0;
#ifdef CONFIG_64BIT
	struct compat_rt_sigframe __user * compat_frame;
#endif
	
	usp = (regs->gr[30] & ~(0x01UL));
	sigframe_size = PARISC_RT_SIGFRAME_SIZE;
#ifdef CONFIG_64BIT
	if (is_compat_task()) {
		/* The gcc alloca implementation leaves garbage in the upper 32 bits of sp */
		usp = (compat_uint_t)usp;
		sigframe_size = PARISC_RT_SIGFRAME_SIZE32;
	}
#endif
	frame = get_sigframe(&ksig->ka, usp, sigframe_size);

	DBG(1, "%s: frame %p info %p\n", __func__, frame, &ksig->info);

	start = (unsigned long) frame;
	if (start >= TASK_SIZE_MAX - sigframe_size)
		return -EFAULT;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	
#ifdef CONFIG_64BIT

	compat_frame = (struct compat_rt_sigframe __user *)frame;
	
	if (is_compat_task()) {
<<<<<<< HEAD
		DBG(1,"setup_rt_frame: frame->info = 0x%p\n", &compat_frame->info);
		err |= copy_siginfo_to_user32(&compat_frame->info, info);
		DBG(1,"SETUP_RT_FRAME: 1\n");
		compat_val = (compat_int_t)current->sas_ss_sp;
		err |= __put_user(compat_val, &compat_frame->uc.uc_stack.ss_sp);
		DBG(1,"SETUP_RT_FRAME: 2\n");
		compat_val = (compat_int_t)current->sas_ss_size;
		err |= __put_user(compat_val, &compat_frame->uc.uc_stack.ss_size);
		DBG(1,"SETUP_RT_FRAME: 3\n");
		compat_val = sas_ss_flags(regs->gr[30]);		
		err |= __put_user(compat_val, &compat_frame->uc.uc_stack.ss_flags);		
		DBG(1,"setup_rt_frame: frame->uc = 0x%p\n", &compat_frame->uc);
		DBG(1,"setup_rt_frame: frame->uc.uc_mcontext = 0x%p\n", &compat_frame->uc.uc_mcontext);
		err |= setup_sigcontext32(&compat_frame->uc.uc_mcontext, 
					&compat_frame->regs, regs, in_syscall);
		sigset_64to32(&compat_set,set);
		err |= __copy_to_user(&compat_frame->uc.uc_sigmask, &compat_set, sizeof(compat_set));
	} else
#endif
	{	
		DBG(1,"setup_rt_frame: frame->info = 0x%p\n", &frame->info);
		err |= copy_siginfo_to_user(&frame->info, info);
		err |= __put_user(current->sas_ss_sp, &frame->uc.uc_stack.ss_sp);
		err |= __put_user(current->sas_ss_size, &frame->uc.uc_stack.ss_size);
		err |= __put_user(sas_ss_flags(regs->gr[30]),
				  &frame->uc.uc_stack.ss_flags);
		DBG(1,"setup_rt_frame: frame->uc = 0x%p\n", &frame->uc);
		DBG(1,"setup_rt_frame: frame->uc.uc_mcontext = 0x%p\n", &frame->uc.uc_mcontext);
=======
		DBG(1, "%s: frame->info = 0x%p\n", __func__, &compat_frame->info);
		err |= copy_siginfo_to_user32(&compat_frame->info, &ksig->info);
		err |= __compat_save_altstack( &compat_frame->uc.uc_stack, regs->gr[30]);
		DBG(1, "%s: frame->uc = 0x%p\n", __func__, &compat_frame->uc);
		DBG(1, "%s: frame->uc.uc_mcontext = 0x%p\n",
			__func__, &compat_frame->uc.uc_mcontext);
		err |= setup_sigcontext32(&compat_frame->uc.uc_mcontext, 
					&compat_frame->regs, regs, in_syscall);
		err |= put_compat_sigset(&compat_frame->uc.uc_sigmask, set,
					 sizeof(compat_sigset_t));
	} else
#endif
	{	
		DBG(1, "%s: frame->info = 0x%p\n", __func__, &frame->info);
		err |= copy_siginfo_to_user(&frame->info, &ksig->info);
		err |= __save_altstack(&frame->uc.uc_stack, regs->gr[30]);
		DBG(1, "%s: frame->uc = 0x%p\n", __func__, &frame->uc);
		DBG(1, "%s: frame->uc.uc_mcontext = 0x%p\n",
			__func__, &frame->uc.uc_mcontext);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		err |= setup_sigcontext(&frame->uc.uc_mcontext, regs, in_syscall);
		/* FIXME: Should probably be converted as well for the compat case */
		err |= __copy_to_user(&frame->uc.uc_sigmask, set, sizeof(*set));
	}
	
	if (err)
<<<<<<< HEAD
		goto give_sigsegv;

	/* Set up to return from userspace.  If provided, use a stub
	   already in userspace. The first words of tramp are used to
	   save the previous sigrestartblock trampoline that might be
	   on the stack. We start the sigreturn trampoline at 
	   SIGRESTARTBLOCK_TRAMP+X. */
	err |= __put_user(in_syscall ? INSN_LDI_R25_1 : INSN_LDI_R25_0,
			&frame->tramp[SIGRESTARTBLOCK_TRAMP+0]);
	err |= __put_user(INSN_LDI_R20, 
			&frame->tramp[SIGRESTARTBLOCK_TRAMP+1]);
	err |= __put_user(INSN_BLE_SR2_R0, 
			&frame->tramp[SIGRESTARTBLOCK_TRAMP+2]);
	err |= __put_user(INSN_NOP, &frame->tramp[SIGRESTARTBLOCK_TRAMP+3]);

#if DEBUG_SIG
	/* Assert that we're flushing in the correct space... */
	{
		int sid;
		asm ("mfsp %%sr3,%0" : "=r" (sid));
		DBG(1,"setup_rt_frame: Flushing 64 bytes at space %#x offset %p\n",
		       sid, frame->tramp);
	}
#endif

	flush_user_dcache_range((unsigned long) &frame->tramp[0],
			   (unsigned long) &frame->tramp[TRAMP_SIZE]);
	flush_user_icache_range((unsigned long) &frame->tramp[0],
			   (unsigned long) &frame->tramp[TRAMP_SIZE]);

	/* TRAMP Words 0-4, Length 5 = SIGRESTARTBLOCK_TRAMP
	 * TRAMP Words 5-9, Length 4 = SIGRETURN_TRAMP
	 * So the SIGRETURN_TRAMP is at the end of SIGRESTARTBLOCK_TRAMP
	 */
	rp = (unsigned long) &frame->tramp[SIGRESTARTBLOCK_TRAMP];

	if (err)
		goto give_sigsegv;

	haddr = A(ka->sa.sa_handler);
=======
		return -EFAULT;

#ifdef CONFIG_64BIT
	if (!is_compat_task())
		rp = VDSO64_SYMBOL(current, sigtramp_rt);
	else
#endif
		rp = VDSO32_SYMBOL(current, sigtramp_rt);

	if (in_syscall)
		rp += 4*4; /* skip 4 instructions and start at ldi 1,%r25 */

	haddr = A(ksig->ka.sa.sa_handler);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	/* The sa_handler may be a pointer to a function descriptor */
#ifdef CONFIG_64BIT
	if (is_compat_task()) {
#endif
		if (haddr & PA_PLABEL_FDESC) {
			Elf32_Fdesc fdesc;
			Elf32_Fdesc __user *ufdesc = (Elf32_Fdesc __user *)A(haddr & ~3);

			err = __copy_from_user(&fdesc, ufdesc, sizeof(fdesc));

			if (err)
<<<<<<< HEAD
				goto give_sigsegv;
=======
				return -EFAULT;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

			haddr = fdesc.addr;
			regs->gr[19] = fdesc.gp;
		}
#ifdef CONFIG_64BIT
	} else {
		Elf64_Fdesc fdesc;
		Elf64_Fdesc __user *ufdesc = (Elf64_Fdesc __user *)A(haddr & ~3);
		
		err = __copy_from_user(&fdesc, ufdesc, sizeof(fdesc));
		
		if (err)
<<<<<<< HEAD
			goto give_sigsegv;
		
		haddr = fdesc.addr;
		regs->gr[19] = fdesc.gp;
		DBG(1,"setup_rt_frame: 64 bit signal, exe=%#lx, r19=%#lx, in_syscall=%d\n",
		     haddr, regs->gr[19], in_syscall);
=======
			return -EFAULT;
		
		haddr = fdesc.addr;
		regs->gr[19] = fdesc.gp;
		DBG(1, "%s: 64 bit signal, exe=%#lx, r19=%#lx, in_syscall=%d\n",
		     __func__, haddr, regs->gr[19], in_syscall);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	}
#endif

	/* The syscall return path will create IAOQ values from r31.
	 */
<<<<<<< HEAD
	sigframe_size = PARISC_RT_SIGFRAME_SIZE;
#ifdef CONFIG_64BIT
	if (is_compat_task())
		sigframe_size = PARISC_RT_SIGFRAME_SIZE32;
#endif
=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	if (in_syscall) {
		regs->gr[31] = haddr;
#ifdef CONFIG_64BIT
		if (!test_thread_flag(TIF_32BIT))
<<<<<<< HEAD
			sigframe_size |= 1;
=======
			sigframe_size |= 1; /* XXX ???? */
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#endif
	} else {
		unsigned long psw = USER_PSW;
#ifdef CONFIG_64BIT
		if (!test_thread_flag(TIF_32BIT))
			psw |= PSW_W;
#endif

		/* If we are singlestepping, arrange a trap to be delivered
		   when we return to userspace. Note the semantics -- we
		   should trap before the first insn in the handler is
		   executed. Ref:
			http://sources.redhat.com/ml/gdb/2004-11/msg00245.html
		 */
		if (pa_psw(current)->r) {
			pa_psw(current)->r = 0;
			psw |= PSW_R;
			mtctl(-1, 0);
		}

		regs->gr[0] = psw;
<<<<<<< HEAD
		regs->iaoq[0] = haddr | 3;
		regs->iaoq[1] = regs->iaoq[0] + 4;
	}

	regs->gr[2]  = rp;                /* userland return pointer */
	regs->gr[26] = sig;               /* signal number */
=======
		regs->iaoq[0] = haddr | PRIV_USER;
		regs->iaoq[1] = regs->iaoq[0] + 4;
	}

	regs->gr[2]  = rp;			/* userland return pointer */
	regs->gr[26] = ksig->sig;               /* signal number */
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	
#ifdef CONFIG_64BIT
	if (is_compat_task()) {
		regs->gr[25] = A(&compat_frame->info); /* siginfo pointer */
		regs->gr[24] = A(&compat_frame->uc);   /* ucontext pointer */
	} else
#endif
	{		
		regs->gr[25] = A(&frame->info); /* siginfo pointer */
		regs->gr[24] = A(&frame->uc);   /* ucontext pointer */
	}
	
<<<<<<< HEAD
	DBG(1,"setup_rt_frame: making sigreturn frame: %#lx + %#lx = %#lx\n",
=======
	DBG(1, "%s: making sigreturn frame: %#lx + %#lx = %#lx\n", __func__,
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	       regs->gr[30], sigframe_size,
	       regs->gr[30] + sigframe_size);
	/* Raise the user stack pointer to make a proper call frame. */
	regs->gr[30] = (A(frame) + sigframe_size);


<<<<<<< HEAD
	DBG(1,"setup_rt_frame: sig deliver (%s,%d) frame=0x%p sp=%#lx iaoq=%#lx/%#lx rp=%#lx\n",
	       current->comm, current->pid, frame, regs->gr[30],
	       regs->iaoq[0], regs->iaoq[1], rp);

	return 1;

give_sigsegv:
	DBG(1,"setup_rt_frame: sending SIGSEGV\n");
	force_sigsegv(sig, current);
=======
	DBG(1, "%s: sig deliver (%s,%d) frame=0x%p sp=%#lx iaoq=%#lx/%#lx rp=%#lx\n",
	       __func__, current->comm, current->pid, frame, regs->gr[30],
	       regs->iaoq[0], regs->iaoq[1], rp);

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	return 0;
}

/*
 * OK, we're invoking a handler.
 */	

<<<<<<< HEAD
static long
handle_signal(unsigned long sig, siginfo_t *info, struct k_sigaction *ka,
		sigset_t *oldset, struct pt_regs *regs, int in_syscall)
{
	DBG(1,"handle_signal: sig=%ld, ka=%p, info=%p, oldset=%p, regs=%p\n",
	       sig, ka, info, oldset, regs);
	
	/* Set up the stack frame */
	if (!setup_rt_frame(sig, ka, info, oldset, regs, in_syscall))
		return 0;

	spin_lock_irq(&current->sighand->siglock);
	sigorsets(&current->blocked,&current->blocked,&ka->sa.sa_mask);
	if (!(ka->sa.sa_flags & SA_NODEFER))
		sigaddset(&current->blocked,sig);
	recalc_sigpending();
	spin_unlock_irq(&current->sighand->siglock);

	tracehook_signal_handler(sig, info, ka, regs, 
		test_thread_flag(TIF_SINGLESTEP) ||
		test_thread_flag(TIF_BLOCKSTEP));

	return 1;
=======
static void
handle_signal(struct ksignal *ksig, struct pt_regs *regs, long in_syscall)
{
	int ret;
	sigset_t *oldset = sigmask_to_save();

	DBG(1, "%s: sig=%d, ka=%p, info=%p, oldset=%p, regs=%p\n",
	       __func__, ksig->sig, &ksig->ka, &ksig->info, oldset, regs);
	
	/* Set up the stack frame */
	ret = setup_rt_frame(ksig, oldset, regs, in_syscall);

	signal_setup_done(ret, ksig, test_thread_flag(TIF_SINGLESTEP) ||
			  test_thread_flag(TIF_BLOCKSTEP));

	DBG(1, "%s: Exit (success), regs->gr[28] = %ld\n",
		__func__, regs->gr[28]);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

/*
 * Check how the syscall number gets loaded into %r20 within
 * the delay branch in userspace and adjust as needed.
 */

static void check_syscallno_in_delay_branch(struct pt_regs *regs)
{
	u32 opcode, source_reg;
	u32 __user *uaddr;
	int err;

	/* Usually we don't have to restore %r20 (the system call number)
	 * because it gets loaded in the delay slot of the branch external
	 * instruction via the ldi instruction.
	 * In some cases a register-to-register copy instruction might have
	 * been used instead, in which case we need to copy the syscall
	 * number into the source register before returning to userspace.
	 */

	/* A syscall is just a branch, so all we have to do is fiddle the
	 * return pointer so that the ble instruction gets executed again.
	 */
	regs->gr[31] -= 8; /* delayed branching */

	/* Get assembler opcode of code in delay branch */
<<<<<<< HEAD
	uaddr = (unsigned int *) ((regs->gr[31] & ~3) + 4);
=======
	uaddr = (u32 __user *) ((regs->gr[31] & ~3) + 4);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	err = get_user(opcode, uaddr);
	if (err)
		return;

	/* Check if delay branch uses "ldi int,%r20" */
	if ((opcode & 0xffff0000) == 0x34140000)
		return;	/* everything ok, just return */

	/* Check if delay branch uses "nop" */
	if (opcode == INSN_NOP)
		return;

	/* Check if delay branch uses "copy %rX,%r20" */
	if ((opcode & 0xffe0ffff) == 0x08000254) {
		source_reg = (opcode >> 16) & 31;
		regs->gr[source_reg] = regs->gr[20];
		return;
	}

	pr_warn("syscall restart: %s (pid %d): unexpected opcode 0x%08x\n",
		current->comm, task_pid_nr(current), opcode);
}

static inline void
syscall_restart(struct pt_regs *regs, struct k_sigaction *ka)
{
<<<<<<< HEAD
	/* Check the return code */
	switch (regs->gr[28]) {
	case -ERESTART_RESTARTBLOCK:
		current_thread_info()->restart_block.fn =
			do_no_restart_syscall;
	case -ERESTARTNOHAND:
		DBG(1,"ERESTARTNOHAND: returning -EINTR\n");
		regs->gr[28] = -EINTR;
		break;

	case -ERESTARTSYS:
		if (!(ka->sa.sa_flags & SA_RESTART)) {
			DBG(1,"ERESTARTSYS: putting -EINTR\n");
			regs->gr[28] = -EINTR;
			break;
		}
		/* fallthrough */
	case -ERESTARTNOINTR:
		check_syscallno_in_delay_branch(regs);
		/* Preserve original r28. */
		regs->gr[28] = regs->orig_r28;
=======
	if (regs->orig_r28)
		return;
	regs->orig_r28 = 1; /* no more restarts */

	DBG(1, "%s:  orig_r28 = %ld  pid %d  r20 %ld\n",
		__func__, regs->orig_r28, task_pid_nr(current), regs->gr[20]);

	/* Check the return code */
	switch (regs->gr[28]) {
	case -ERESTART_RESTARTBLOCK:
	case -ERESTARTNOHAND:
		DBG(1, "%s: ERESTARTNOHAND: returning -EINTR\n", __func__);
		regs->gr[28] = -EINTR;
		break;
	case -ERESTARTSYS:
		if (!(ka->sa.sa_flags & SA_RESTART)) {
			DBG(1, "%s: ERESTARTSYS: putting -EINTR pid %d\n",
				__func__, task_pid_nr(current));
			regs->gr[28] = -EINTR;
			break;
		}
		fallthrough;
	case -ERESTARTNOINTR:
		DBG(1, "%s: %ld\n", __func__, regs->gr[28]);
		check_syscallno_in_delay_branch(regs);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		break;
	}
}

static inline void
insert_restart_trampoline(struct pt_regs *regs)
{
<<<<<<< HEAD
	switch(regs->gr[28]) {
	case -ERESTART_RESTARTBLOCK: {
		/* Restart the system call - no handlers present */
		unsigned int *usp = (unsigned int *)regs->gr[30];

		/* Setup a trampoline to restart the syscall
		 * with __NR_restart_syscall
		 *
		 *  0: <return address (orig r31)>
		 *  4: <2nd half for 64-bit>
		 *  8: ldw 0(%sp), %r31
		 * 12: be 0x100(%sr2, %r0)
		 * 16: ldi __NR_restart_syscall, %r20
		 */
#ifdef CONFIG_64BIT
		put_user(regs->gr[31] >> 32, &usp[0]);
		put_user(regs->gr[31] & 0xffffffff, &usp[1]);
		put_user(0x0fc010df, &usp[2]);
#else
		put_user(regs->gr[31], &usp[0]);
		put_user(0x0fc0109f, &usp[2]);
#endif
		put_user(0xe0008200, &usp[3]);
		put_user(0x34140000, &usp[4]);

		/* Stack is 64-byte aligned, and we only need
		 * to flush 1 cache line.
		 * Flushing one cacheline is cheap.
		 * "sync" on bigger (> 4 way) boxes is not.
		 */
		flush_user_dcache_range(regs->gr[30], regs->gr[30] + 4);
		flush_user_icache_range(regs->gr[30], regs->gr[30] + 4);

		regs->gr[31] = regs->gr[30] + 8;
		/* Preserve original r28. */
		regs->gr[28] = regs->orig_r28;

		return;
	}
	case -ERESTARTNOHAND:
	case -ERESTARTSYS:
	case -ERESTARTNOINTR:
=======
	if (regs->orig_r28)
		return;
	regs->orig_r28 = 1; /* no more restarts */

	DBG(2, "%s: gr28 = %ld pid %d\n",
		__func__, regs->gr[28], task_pid_nr(current));

	switch (regs->gr[28]) {
	case -ERESTART_RESTARTBLOCK: {
		/* Restart the system call - no handlers present */
		unsigned int *usp = (unsigned int *)regs->gr[30];
		unsigned long rp;
		long err = 0;

		/* check that we don't exceed the stack */
		if (A(&usp[0]) >= TASK_SIZE_MAX - 5 * sizeof(int))
			return;

		/* Call trampoline in vdso to restart the syscall
		 * with __NR_restart_syscall.
		 * Original return addresses are on stack like this:
		 *
		 *  0: <return address (orig r31)>
		 *  4: <2nd half for 64-bit>
		 */
#ifdef CONFIG_64BIT
		if (!is_compat_task()) {
			err |= put_user(regs->gr[31] >> 32, &usp[0]);
			err |= put_user(regs->gr[31] & 0xffffffff, &usp[1]);
			rp = VDSO64_SYMBOL(current, restart_syscall);
		} else
#endif
		{
			err |= put_user(regs->gr[31], &usp[0]);
			rp = VDSO32_SYMBOL(current, restart_syscall);
		}
		WARN_ON(err);

		regs->gr[31] = rp;
		DBG(1, "%s: ERESTART_RESTARTBLOCK\n", __func__);
		return;
	}
	case -EINTR:
		/* ok, was handled before and should be returned. */
		break;
	case -ERESTARTNOHAND:
	case -ERESTARTSYS:
	case -ERESTARTNOINTR:
		DBG(1, "%s: Type %ld\n", __func__, regs->gr[28]);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		check_syscallno_in_delay_branch(regs);
		return;
	default:
		break;
	}
}

/*
<<<<<<< HEAD
 * Note that 'init' is a special process: it doesn't get signals it doesn't
 * want to handle. Thus you cannot kill init even with a SIGKILL even by
 * mistake.
 *
=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 * We need to be able to restore the syscall arguments (r21-r26) to
 * restart syscalls.  Thus, the syscall path should save them in the
 * pt_regs structure (it's okay to do so since they are caller-save
 * registers).  As noted below, the syscall number gets restored for
 * us due to the magic of delayed branching.
 */
<<<<<<< HEAD
asmlinkage void
do_signal(struct pt_regs *regs, long in_syscall)
{
	siginfo_t info;
	struct k_sigaction ka;
	int signr;
	sigset_t *oldset;

	DBG(1,"\ndo_signal: oldset=0x%p, regs=0x%p, sr7 %#lx, in_syscall=%d\n",
	       oldset, regs, regs->sr[7], in_syscall);

	/* Everyone else checks to see if they are in kernel mode at
	   this point and exits if that's the case.  I'm not sure why
	   we would be called in that case, but for some reason we
	   are. */

	if (test_thread_flag(TIF_RESTORE_SIGMASK))
		oldset = &current->saved_sigmask;
	else
		oldset = &current->blocked;

	DBG(1,"do_signal: oldset %08lx / %08lx\n", 
		oldset->sig[0], oldset->sig[1]);


	/* May need to force signal if handle_signal failed to deliver */
	while (1) {
	  
		signr = get_signal_to_deliver(&info, &ka, regs, NULL);
		DBG(3,"do_signal: signr = %d, regs->gr[28] = %ld\n", signr, regs->gr[28]); 
	
		if (signr <= 0)
		  break;
		
		/* Restart a system call if necessary. */
		if (in_syscall)
			syscall_restart(regs, &ka);

		/* Whee!  Actually deliver the signal.  If the
		   delivery failed, we need to continue to iterate in
		   this loop so we can deliver the SIGSEGV... */
		if (handle_signal(signr, &info, &ka, oldset,
				  regs, in_syscall)) {
			DBG(1,KERN_DEBUG "do_signal: Exit (success), regs->gr[28] = %ld\n",
				regs->gr[28]);
			if (test_thread_flag(TIF_RESTORE_SIGMASK))
				clear_thread_flag(TIF_RESTORE_SIGMASK);
			return;
		}
	}
	/* end of while(1) looping forever if we can't force a signal */

	/* Did we come from a system call? */
	if (in_syscall)
		insert_restart_trampoline(regs);
	
	DBG(1,"do_signal: Exit (not delivered), regs->gr[28] = %ld\n", 
		regs->gr[28]);

	if (test_thread_flag(TIF_RESTORE_SIGMASK)) {
		clear_thread_flag(TIF_RESTORE_SIGMASK);
		sigprocmask(SIG_SETMASK, &current->saved_sigmask, NULL);
	}

	return;
}

void do_notify_resume(struct pt_regs *regs, long in_syscall)
{
	if (test_thread_flag(TIF_SIGPENDING) ||
	    test_thread_flag(TIF_RESTORE_SIGMASK))
		do_signal(regs, in_syscall);

	if (test_thread_flag(TIF_NOTIFY_RESUME)) {
		clear_thread_flag(TIF_NOTIFY_RESUME);
		tracehook_notify_resume(regs);
		if (current->replacement_session_keyring)
			key_replace_session_keyring();
	}
=======
static void do_signal(struct pt_regs *regs, long in_syscall)
{
	struct ksignal ksig;
	int restart_syscall;
	bool has_handler;

	has_handler = get_signal(&ksig);

	restart_syscall = 0;
	if (in_syscall)
		restart_syscall = 1;

	if (has_handler) {
		/* Restart a system call if necessary. */
		if (restart_syscall)
			syscall_restart(regs, &ksig.ka);

		handle_signal(&ksig, regs, in_syscall);
		DBG(1, "%s: Handled signal pid %d\n",
			__func__, task_pid_nr(current));
		return;
	}

	/* Do we need to restart the system call? */
	if (restart_syscall)
		insert_restart_trampoline(regs);
	
	DBG(1, "%s: Exit (not delivered), regs->gr[28] = %ld  orig_r28 = %ld  pid %d\n",
		__func__, regs->gr[28], regs->orig_r28, task_pid_nr(current));

	restore_saved_sigmask();
}

asmlinkage void do_notify_resume(struct pt_regs *regs, long in_syscall)
{
	if (test_thread_flag(TIF_SIGPENDING) ||
	    test_thread_flag(TIF_NOTIFY_SIGNAL))
		do_signal(regs, in_syscall);

	if (test_thread_flag(TIF_NOTIFY_RESUME))
		resume_user_mode_work(regs);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}
