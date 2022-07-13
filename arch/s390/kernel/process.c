<<<<<<< HEAD
/*
 * This file handles the architecture dependent parts of process handling.
 *
 *    Copyright IBM Corp. 1999,2009
=======
// SPDX-License-Identifier: GPL-2.0
/*
 * This file handles the architecture dependent parts of process handling.
 *
 *    Copyright IBM Corp. 1999, 2009
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 *    Author(s): Martin Schwidefsky <schwidefsky@de.ibm.com>,
 *		 Hartmut Penner <hp@de.ibm.com>,
 *		 Denis Joseph Barrow,
 */

<<<<<<< HEAD
#include <linux/compiler.h>
#include <linux/cpu.h>
#include <linux/sched.h>
=======
#include <linux/elf-randomize.h>
#include <linux/compiler.h>
#include <linux/cpu.h>
#include <linux/sched.h>
#include <linux/sched/debug.h>
#include <linux/sched/task.h>
#include <linux/sched/task_stack.h>
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#include <linux/kernel.h>
#include <linux/mm.h>
#include <linux/elfcore.h>
#include <linux/smp.h>
#include <linux/slab.h>
#include <linux/interrupt.h>
#include <linux/tick.h>
#include <linux/personality.h>
#include <linux/syscalls.h>
#include <linux/compat.h>
#include <linux/kprobes.h>
#include <linux/random.h>
<<<<<<< HEAD
#include <linux/module.h>
#include <asm/io.h>
#include <asm/processor.h>
#include <asm/irq.h>
#include <asm/timer.h>
#include <asm/nmi.h>
#include <asm/smp.h>
#include <asm/switch_to.h>
#include "entry.h"

asmlinkage void ret_from_fork(void) asm ("ret_from_fork");

/*
 * Return saved PC of a blocked thread. used in kernel/sched.
 * resume in entry.S does not create a new stack frame, it
 * just stores the registers %r6-%r15 to the frame given by
 * schedule. We want to return the address of the caller of
 * schedule, so we have to walk the backchain one time to
 * find the frame schedule() store its return address.
 */
unsigned long thread_saved_pc(struct task_struct *tsk)
{
	struct stack_frame *sf, *low, *high;

	if (!tsk || !task_stack_page(tsk))
		return 0;
	low = task_stack_page(tsk);
	high = (struct stack_frame *) task_pt_regs(tsk);
	sf = (struct stack_frame *) (tsk->thread.ksp & PSW_ADDR_INSN);
	if (sf <= low || sf > high)
		return 0;
	sf = (struct stack_frame *) (sf->back_chain & PSW_ADDR_INSN);
	if (sf <= low || sf > high)
		return 0;
	return sf->gprs[8];
}

/*
 * The idle loop on a S390...
 */
static void default_idle(void)
{
	if (cpu_is_offline(smp_processor_id()))
		cpu_die();
	local_irq_disable();
	if (need_resched()) {
		local_irq_enable();
		return;
	}
	local_mcck_disable();
	if (test_thread_flag(TIF_MCCK_PENDING)) {
		local_mcck_enable();
		local_irq_enable();
		return;
	}
	/* Halt the cpu and keep track of cpu time accounting. */
	vtime_stop_cpu();
}

void cpu_idle(void)
{
	for (;;) {
		tick_nohz_idle_enter();
		rcu_idle_enter();
		while (!need_resched() && !test_thread_flag(TIF_MCCK_PENDING))
			default_idle();
		rcu_idle_exit();
		tick_nohz_idle_exit();
		if (test_thread_flag(TIF_MCCK_PENDING))
			s390_handle_mcck();
		schedule_preempt_disabled();
	}
}

extern void __kprobes kernel_thread_starter(void);

asm(
	".section .kprobes.text, \"ax\"\n"
	".global kernel_thread_starter\n"
	"kernel_thread_starter:\n"
	"    la    2,0(10)\n"
	"    basr  14,9\n"
	"    la    2,0\n"
	"    br    11\n"
	".previous\n");

int kernel_thread(int (*fn)(void *), void * arg, unsigned long flags)
{
	struct pt_regs regs;

	memset(&regs, 0, sizeof(regs));
	regs.psw.mask = psw_kernel_bits |
		PSW_MASK_DAT | PSW_MASK_IO | PSW_MASK_EXT | PSW_MASK_MCHECK;
	regs.psw.addr = (unsigned long) kernel_thread_starter | PSW_ADDR_AMODE;
	regs.gprs[9] = (unsigned long) fn;
	regs.gprs[10] = (unsigned long) arg;
	regs.gprs[11] = (unsigned long) do_exit;
	regs.orig_gpr2 = -1;

	/* Ok, create the new process.. */
	return do_fork(flags | CLONE_VM | CLONE_UNTRACED,
		       0, &regs, 0, NULL, NULL);
}
EXPORT_SYMBOL(kernel_thread);

/*
 * Free current thread data structures etc..
 */
void exit_thread(void)
{
=======
#include <linux/export.h>
#include <linux/init_task.h>
#include <linux/entry-common.h>
#include <linux/io.h>
#include <asm/guarded_storage.h>
#include <asm/access-regs.h>
#include <asm/switch_to.h>
#include <asm/cpu_mf.h>
#include <asm/processor.h>
#include <asm/ptrace.h>
#include <asm/vtimer.h>
#include <asm/exec.h>
#include <asm/fpu.h>
#include <asm/irq.h>
#include <asm/nmi.h>
#include <asm/smp.h>
#include <asm/stacktrace.h>
#include <asm/runtime_instr.h>
#include <asm/unwind.h>
#include "entry.h"

void ret_from_fork(void) asm("ret_from_fork");

void __ret_from_fork(struct task_struct *prev, struct pt_regs *regs)
{
	void (*func)(void *arg);

	schedule_tail(prev);

	if (!user_mode(regs)) {
		/* Kernel thread */
		func = (void *)regs->gprs[9];
		func((void *)regs->gprs[10]);
	}
	clear_pt_regs_flag(regs, PIF_SYSCALL);
	syscall_exit_to_user_mode(regs);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

void flush_thread(void)
{
}

<<<<<<< HEAD
void release_thread(struct task_struct *dead_task)
{
}

int copy_thread(unsigned long clone_flags, unsigned long new_stackp,
		unsigned long unused,
		struct task_struct *p, struct pt_regs *regs)
{
	struct thread_info *ti;
=======
void arch_setup_new_exec(void)
{
	if (S390_lowcore.current_pid != current->pid) {
		S390_lowcore.current_pid = current->pid;
		if (test_facility(40))
			lpp(&S390_lowcore.lpp);
	}
}

void arch_release_task_struct(struct task_struct *tsk)
{
	runtime_instr_release(tsk);
	guarded_storage_release(tsk);
}

int arch_dup_task_struct(struct task_struct *dst, struct task_struct *src)
{
	/*
	 * Save the floating-point or vector register state of the current
	 * task and set the TIF_FPU flag to lazy restore the FPU register
	 * state when returning to user space.
	 */
	save_user_fpu_regs();

	*dst = *src;
	dst->thread.kfpu_flags = 0;

	/*
	 * Don't transfer over the runtime instrumentation or the guarded
	 * storage control block pointers. These fields are cleared here instead
	 * of in copy_thread() to avoid premature freeing of associated memory
	 * on fork() failure. Wait to clear the RI flag because ->stack still
	 * refers to the source thread.
	 */
	dst->thread.ri_cb = NULL;
	dst->thread.gs_cb = NULL;
	dst->thread.gs_bc_cb = NULL;

	return 0;
}

int copy_thread(struct task_struct *p, const struct kernel_clone_args *args)
{
	unsigned long clone_flags = args->flags;
	unsigned long new_stackp = args->stack;
	unsigned long tls = args->tls;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	struct fake_frame
	{
		struct stack_frame sf;
		struct pt_regs childregs;
	} *frame;

	frame = container_of(task_pt_regs(p), struct fake_frame, childregs);
	p->thread.ksp = (unsigned long) frame;
<<<<<<< HEAD
	/* Store access registers to kernel stack of new process. */
	frame->childregs = *regs;
	frame->childregs.gprs[2] = 0;	/* child returns 0 on fork. */
	frame->childregs.gprs[15] = new_stackp;
	frame->sf.back_chain = 0;

	/* new return point is ret_from_fork */
	frame->sf.gprs[8] = (unsigned long) ret_from_fork;

	/* fake return stack for resume(), don't go back to schedule */
	frame->sf.gprs[9] = (unsigned long) frame;

	/* Save access registers to new thread structure. */
	save_access_regs(&p->thread.acrs[0]);

#ifndef CONFIG_64BIT
	/*
	 * save fprs to current->thread.fp_regs to merge them with
	 * the emulated registers and then copy the result to the child.
	 */
	save_fp_regs(&current->thread.fp_regs);
	memcpy(&p->thread.fp_regs, &current->thread.fp_regs,
	       sizeof(s390_fp_regs));
	/* Set a new TLS ?  */
	if (clone_flags & CLONE_SETTLS)
		p->thread.acrs[0] = regs->gprs[6];
#else /* CONFIG_64BIT */
	/* Save the fpu registers to new thread structure. */
	save_fp_regs(&p->thread.fp_regs);
	/* Set a new TLS ?  */
	if (clone_flags & CLONE_SETTLS) {
		if (is_compat_task()) {
			p->thread.acrs[0] = (unsigned int) regs->gprs[6];
		} else {
			p->thread.acrs[0] = (unsigned int)(regs->gprs[6] >> 32);
			p->thread.acrs[1] = (unsigned int) regs->gprs[6];
		}
	}
#endif /* CONFIG_64BIT */
	/* start new process with ar4 pointing to the correct address space */
	p->thread.mm_segment = get_fs();
=======
	/* Save access registers to new thread structure. */
	save_access_regs(&p->thread.acrs[0]);
	/* start new process with ar4 pointing to the correct address space */
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	/* Don't copy debug registers */
	memset(&p->thread.per_user, 0, sizeof(p->thread.per_user));
	memset(&p->thread.per_event, 0, sizeof(p->thread.per_event));
	clear_tsk_thread_flag(p, TIF_SINGLE_STEP);
<<<<<<< HEAD
	clear_tsk_thread_flag(p, TIF_PER_TRAP);
	/* Initialize per thread user and system timer values */
	ti = task_thread_info(p);
	ti->user_timer = 0;
	ti->system_timer = 0;
	return 0;
}

SYSCALL_DEFINE0(fork)
{
	struct pt_regs *regs = task_pt_regs(current);
	return do_fork(SIGCHLD, regs->gprs[15], regs, 0, NULL, NULL);
}

SYSCALL_DEFINE4(clone, unsigned long, newsp, unsigned long, clone_flags,
		int __user *, parent_tidptr, int __user *, child_tidptr)
{
	struct pt_regs *regs = task_pt_regs(current);

	if (!newsp)
		newsp = regs->gprs[15];
	return do_fork(clone_flags, newsp, regs, 0,
		       parent_tidptr, child_tidptr);
}

/*
 * This is trivial, and on the face of it looks like it
 * could equally well be done in user mode.
 *
 * Not so, for quite unobvious reasons - register pressure.
 * In user mode vfork() cannot have a stack frame, and if
 * done by calling the "clone()" system call directly, you
 * do not have enough call-clobbered registers to hold all
 * the information you need.
 */
SYSCALL_DEFINE0(vfork)
{
	struct pt_regs *regs = task_pt_regs(current);
	return do_fork(CLONE_VFORK | CLONE_VM | SIGCHLD,
		       regs->gprs[15], regs, 0, NULL, NULL);
}

asmlinkage void execve_tail(void)
{
	current->thread.fp_regs.fpc = 0;
	if (MACHINE_HAS_IEEE)
		asm volatile("sfpc %0" : : "d" (0));
}

/*
 * sys_execve() executes a new program.
 */
SYSCALL_DEFINE3(execve, const char __user *, name,
		const char __user *const __user *, argv,
		const char __user *const __user *, envp)
{
	struct pt_regs *regs = task_pt_regs(current);
	char *filename;
	long rc;

	filename = getname(name);
	rc = PTR_ERR(filename);
	if (IS_ERR(filename))
		return rc;
	rc = do_execve(filename, argv, envp, regs);
	if (rc)
		goto out;
	execve_tail();
	rc = regs->gprs[2];
out:
	putname(filename);
	return rc;
}

/*
 * fill in the FPU structure for a core dump.
 */
int dump_fpu (struct pt_regs * regs, s390_fp_regs *fpregs)
{
#ifndef CONFIG_64BIT
	/*
	 * save fprs to current->thread.fp_regs to merge them with
	 * the emulated registers and then copy the result to the dump.
	 */
	save_fp_regs(&current->thread.fp_regs);
	memcpy(fpregs, &current->thread.fp_regs, sizeof(s390_fp_regs));
#else /* CONFIG_64BIT */
	save_fp_regs(fpregs);
#endif /* CONFIG_64BIT */
	return 1;
}
EXPORT_SYMBOL(dump_fpu);

unsigned long get_wchan(struct task_struct *p)
{
	struct stack_frame *sf, *low, *high;
	unsigned long return_address;
	int count;

	if (!p || p == current || p->state == TASK_RUNNING || !task_stack_page(p))
		return 0;
	low = task_stack_page(p);
	high = (struct stack_frame *) task_pt_regs(p);
	sf = (struct stack_frame *) (p->thread.ksp & PSW_ADDR_INSN);
	if (sf <= low || sf > high)
		return 0;
	for (count = 0; count < 16; count++) {
		sf = (struct stack_frame *) (sf->back_chain & PSW_ADDR_INSN);
		if (sf <= low || sf > high)
			return 0;
		return_address = sf->gprs[8] & PSW_ADDR_INSN;
		if (!in_sched_functions(return_address))
			return return_address;
	}
	return 0;
}

unsigned long arch_align_stack(unsigned long sp)
{
	if (!(current->personality & ADDR_NO_RANDOMIZE) && randomize_va_space)
		sp -= get_random_int() & ~PAGE_MASK;
=======
	p->thread.per_flags = 0;
	/* Initialize per thread user and system timer values */
	p->thread.user_timer = 0;
	p->thread.guest_timer = 0;
	p->thread.system_timer = 0;
	p->thread.hardirq_timer = 0;
	p->thread.softirq_timer = 0;
	p->thread.last_break = 1;

	frame->sf.back_chain = 0;
	frame->sf.gprs[11 - 6] = (unsigned long)&frame->childregs;
	frame->sf.gprs[12 - 6] = (unsigned long)p;
	/* new return point is ret_from_fork */
	frame->sf.gprs[14 - 6] = (unsigned long)ret_from_fork;
	/* fake return stack for resume(), don't go back to schedule */
	frame->sf.gprs[15 - 6] = (unsigned long)frame;

	/* Store access registers to kernel stack of new process. */
	if (unlikely(args->fn)) {
		/* kernel thread */
		memset(&frame->childregs, 0, sizeof(struct pt_regs));
		frame->childregs.psw.mask = PSW_KERNEL_BITS | PSW_MASK_IO |
					    PSW_MASK_EXT | PSW_MASK_MCHECK;
		frame->childregs.gprs[9] = (unsigned long)args->fn;
		frame->childregs.gprs[10] = (unsigned long)args->fn_arg;
		frame->childregs.orig_gpr2 = -1;
		frame->childregs.last_break = 1;
		return 0;
	}
	frame->childregs = *current_pt_regs();
	frame->childregs.gprs[2] = 0;	/* child returns 0 on fork. */
	frame->childregs.flags = 0;
	if (new_stackp)
		frame->childregs.gprs[15] = new_stackp;
	/*
	 * Clear the runtime instrumentation flag after the above childregs
	 * copy. The CB pointer was already cleared in arch_dup_task_struct().
	 */
	frame->childregs.psw.mask &= ~PSW_MASK_RI;

	/* Set a new TLS ?  */
	if (clone_flags & CLONE_SETTLS) {
		if (is_compat_task()) {
			p->thread.acrs[0] = (unsigned int)tls;
		} else {
			p->thread.acrs[0] = (unsigned int)(tls >> 32);
			p->thread.acrs[1] = (unsigned int)tls;
		}
	}
	/*
	 * s390 stores the svc return address in arch_data when calling
	 * sigreturn()/restart_syscall() via vdso. 1 means no valid address
	 * stored.
	 */
	p->restart_block.arch_data = 1;
	return 0;
}

void execve_tail(void)
{
	current->thread.ufpu.fpc = 0;
	fpu_sfpc(0);
}

struct task_struct *__switch_to(struct task_struct *prev, struct task_struct *next)
{
	save_user_fpu_regs();
	save_kernel_fpu_regs(&prev->thread);
	save_access_regs(&prev->thread.acrs[0]);
	save_ri_cb(prev->thread.ri_cb);
	save_gs_cb(prev->thread.gs_cb);
	update_cr_regs(next);
	restore_kernel_fpu_regs(&next->thread);
	restore_access_regs(&next->thread.acrs[0]);
	restore_ri_cb(next->thread.ri_cb, prev->thread.ri_cb);
	restore_gs_cb(next->thread.gs_cb);
	return __switch_to_asm(prev, next);
}

unsigned long __get_wchan(struct task_struct *p)
{
	struct unwind_state state;
	unsigned long ip = 0;

	if (!task_stack_page(p))
		return 0;

	if (!try_get_task_stack(p))
		return 0;

	unwind_for_each_frame(&state, p, NULL, 0) {
		if (state.stack_info.type != STACK_TYPE_TASK) {
			ip = 0;
			break;
		}

		ip = unwind_get_return_address(&state);
		if (!ip)
			break;

		if (!in_sched_functions(ip))
			break;
	}

	put_task_stack(p);
	return ip;
}

unsigned long arch_align_stack(unsigned long sp)
{
	if (!(current->personality & ADDR_NO_RANDOMIZE) && randomize_va_space)
		sp -= get_random_u32_below(PAGE_SIZE);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	return sp & ~0xf;
}

static inline unsigned long brk_rnd(void)
{
<<<<<<< HEAD
	/* 8MB for 32bit, 1GB for 64bit */
	if (is_32bit_task())
		return (get_random_int() & 0x7ffUL) << PAGE_SHIFT;
	else
		return (get_random_int() & 0x3ffffUL) << PAGE_SHIFT;
=======
	return (get_random_u16() & BRK_RND_MASK) << PAGE_SHIFT;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

unsigned long arch_randomize_brk(struct mm_struct *mm)
{
<<<<<<< HEAD
	unsigned long ret = PAGE_ALIGN(mm->brk + brk_rnd());

	if (ret < mm->brk)
		return mm->brk;
	return ret;
}

unsigned long randomize_et_dyn(unsigned long base)
{
	unsigned long ret = PAGE_ALIGN(base + brk_rnd());

	if (!(current->flags & PF_RANDOMIZE))
		return base;
	if (ret < base)
		return base;
	return ret;
=======
	unsigned long ret;

	ret = PAGE_ALIGN(mm->brk + brk_rnd());
	return (ret > mm->brk) ? ret : mm->brk;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}
