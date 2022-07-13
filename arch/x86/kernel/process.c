<<<<<<< HEAD
=======
// SPDX-License-Identifier: GPL-2.0
#define pr_fmt(fmt) KBUILD_MODNAME ": " fmt

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#include <linux/errno.h>
#include <linux/kernel.h>
#include <linux/mm.h>
#include <linux/smp.h>
<<<<<<< HEAD
#include <linux/prctl.h>
#include <linux/slab.h>
#include <linux/sched.h>
#include <linux/module.h>
#include <linux/pm.h>
#include <linux/clockchips.h>
=======
#include <linux/cpu.h>
#include <linux/prctl.h>
#include <linux/slab.h>
#include <linux/sched.h>
#include <linux/sched/idle.h>
#include <linux/sched/debug.h>
#include <linux/sched/task.h>
#include <linux/sched/task_stack.h>
#include <linux/init.h>
#include <linux/export.h>
#include <linux/pm.h>
#include <linux/tick.h>
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#include <linux/random.h>
#include <linux/user-return-notifier.h>
#include <linux/dmi.h>
#include <linux/utsname.h>
#include <linux/stackprotector.h>
<<<<<<< HEAD
#include <linux/tick.h>
#include <linux/cpuidle.h>
#include <trace/events/power.h>
#include <linux/hw_breakpoint.h>
#include <asm/cpu.h>
#include <asm/apic.h>
#include <asm/syscalls.h>
#include <asm/idle.h>
#include <asm/uaccess.h>
#include <asm/i387.h>
#include <asm/fpu-internal.h>
#include <asm/debugreg.h>
#include <asm/nmi.h>

#ifdef CONFIG_X86_64
static DEFINE_PER_CPU(unsigned char, is_idle);
#endif

struct kmem_cache *task_xstate_cachep;
EXPORT_SYMBOL_GPL(task_xstate_cachep);

int arch_dup_task_struct(struct task_struct *dst, struct task_struct *src)
{
	int ret;

	*dst = *src;
	if (fpu_allocated(&src->thread.fpu)) {
		memset(&dst->thread.fpu, 0, sizeof(dst->thread.fpu));
		ret = fpu_alloc(&dst->thread.fpu);
		if (ret)
			return ret;
		fpu_copy(&dst->thread.fpu, &src->thread.fpu);
	}
	return 0;
}

void free_thread_xstate(struct task_struct *tsk)
{
	fpu_free(&tsk->thread.fpu);
}

void free_thread_info(struct thread_info *ti)
{
	free_thread_xstate(ti->task);
	free_pages((unsigned long)ti, THREAD_ORDER);
}

void arch_task_cache_init(void)
{
        task_xstate_cachep =
        	kmem_cache_create("task_xstate", xstate_size,
				  __alignof__(union thread_xstate),
				  SLAB_PANIC | SLAB_NOTRACK, NULL);
}

/*
 * Free current thread data structures etc..
 */
void exit_thread(void)
{
	struct task_struct *me = current;
	struct thread_struct *t = &me->thread;
	unsigned long *bp = t->io_bitmap_ptr;

	if (bp) {
		struct tss_struct *tss = &per_cpu(init_tss, get_cpu());

		t->io_bitmap_ptr = NULL;
		clear_thread_flag(TIF_IO_BITMAP);
		/*
		 * Careful, clear this in the TSS too:
		 */
		memset(tss->io_bitmap, 0xff, t->io_bitmap_max);
		t->io_bitmap_max = 0;
		put_cpu();
		kfree(bp);
	}
}

void show_regs(struct pt_regs *regs)
{
	show_registers(regs);
	show_trace(NULL, regs, (unsigned long *)kernel_stack_pointer(regs), 0);
}

void show_regs_common(void)
{
	const char *vendor, *product, *board;

	vendor = dmi_get_system_info(DMI_SYS_VENDOR);
	if (!vendor)
		vendor = "";
	product = dmi_get_system_info(DMI_PRODUCT_NAME);
	if (!product)
		product = "";

	/* Board Name is optional */
	board = dmi_get_system_info(DMI_BOARD_NAME);

	printk(KERN_CONT "\n");
	printk(KERN_DEFAULT "Pid: %d, comm: %.20s %s %s %.*s",
		current->pid, current->comm, print_tainted(),
		init_utsname()->release,
		(int)strcspn(init_utsname()->version, " "),
		init_utsname()->version);
	printk(KERN_CONT " %s %s", vendor, product);
	if (board)
		printk(KERN_CONT "/%s", board);
	printk(KERN_CONT "\n");
=======
#include <linux/cpuidle.h>
#include <linux/acpi.h>
#include <linux/elf-randomize.h>
#include <linux/static_call.h>
#include <trace/events/power.h>
#include <linux/hw_breakpoint.h>
#include <linux/entry-common.h>
#include <asm/cpu.h>
#include <asm/apic.h>
#include <linux/uaccess.h>
#include <asm/mwait.h>
#include <asm/fpu/api.h>
#include <asm/fpu/sched.h>
#include <asm/fpu/xstate.h>
#include <asm/debugreg.h>
#include <asm/nmi.h>
#include <asm/tlbflush.h>
#include <asm/mce.h>
#include <asm/vm86.h>
#include <asm/switch_to.h>
#include <asm/desc.h>
#include <asm/prctl.h>
#include <asm/spec-ctrl.h>
#include <asm/io_bitmap.h>
#include <asm/proto.h>
#include <asm/frame.h>
#include <asm/unwind.h>
#include <asm/tdx.h>
#include <asm/mmu_context.h>
#include <asm/shstk.h>

#include "process.h"

/*
 * per-CPU TSS segments. Threads are completely 'soft' on Linux,
 * no more per-task TSS's. The TSS size is kept cacheline-aligned
 * so they are allowed to end up in the .data..cacheline_aligned
 * section. Since TSS's are completely CPU-local, we want them
 * on exact cacheline boundaries, to eliminate cacheline ping-pong.
 */
__visible DEFINE_PER_CPU_PAGE_ALIGNED(struct tss_struct, cpu_tss_rw) = {
	.x86_tss = {
		/*
		 * .sp0 is only used when entering ring 0 from a lower
		 * privilege level.  Since the init task never runs anything
		 * but ring 0 code, there is no need for a valid value here.
		 * Poison it.
		 */
		.sp0 = (1UL << (BITS_PER_LONG-1)) + 1,

#ifdef CONFIG_X86_32
		.sp1 = TOP_OF_INIT_STACK,

		.ss0 = __KERNEL_DS,
		.ss1 = __KERNEL_CS,
#endif
		.io_bitmap_base	= IO_BITMAP_OFFSET_INVALID,
	 },
};
EXPORT_PER_CPU_SYMBOL(cpu_tss_rw);

DEFINE_PER_CPU(bool, __tss_limit_invalid);
EXPORT_PER_CPU_SYMBOL_GPL(__tss_limit_invalid);

/*
 * this gets called so that we can store lazy state into memory and copy the
 * current task into the new thread.
 */
int arch_dup_task_struct(struct task_struct *dst, struct task_struct *src)
{
	memcpy(dst, src, arch_task_struct_size);
#ifdef CONFIG_VM86
	dst->thread.vm86 = NULL;
#endif
	/* Drop the copied pointer to current's fpstate */
	dst->thread.fpu.fpstate = NULL;

	return 0;
}

#ifdef CONFIG_X86_64
void arch_release_task_struct(struct task_struct *tsk)
{
	if (fpu_state_size_dynamic())
		fpstate_free(&tsk->thread.fpu);
}
#endif

/*
 * Free thread data structures etc..
 */
void exit_thread(struct task_struct *tsk)
{
	struct thread_struct *t = &tsk->thread;
	struct fpu *fpu = &t->fpu;

	if (test_thread_flag(TIF_IO_BITMAP))
		io_bitmap_exit(tsk);

	free_vm86(t);

	shstk_free(tsk);
	fpu__drop(fpu);
}

static int set_new_tls(struct task_struct *p, unsigned long tls)
{
	struct user_desc __user *utls = (struct user_desc __user *)tls;

	if (in_ia32_syscall())
		return do_set_thread_area(p, -1, utls, 0);
	else
		return do_set_thread_area_64(p, ARCH_SET_FS, tls);
}

__visible void ret_from_fork(struct task_struct *prev, struct pt_regs *regs,
				     int (*fn)(void *), void *fn_arg)
{
	schedule_tail(prev);

	/* Is this a kernel thread? */
	if (unlikely(fn)) {
		fn(fn_arg);
		/*
		 * A kernel thread is allowed to return here after successfully
		 * calling kernel_execve().  Exit to userspace to complete the
		 * execve() syscall.
		 */
		regs->ax = 0;
	}

	syscall_exit_to_user_mode(regs);
}

int copy_thread(struct task_struct *p, const struct kernel_clone_args *args)
{
	unsigned long clone_flags = args->flags;
	unsigned long sp = args->stack;
	unsigned long tls = args->tls;
	struct inactive_task_frame *frame;
	struct fork_frame *fork_frame;
	struct pt_regs *childregs;
	unsigned long new_ssp;
	int ret = 0;

	childregs = task_pt_regs(p);
	fork_frame = container_of(childregs, struct fork_frame, regs);
	frame = &fork_frame->frame;

	frame->bp = encode_frame_pointer(childregs);
	frame->ret_addr = (unsigned long) ret_from_fork_asm;
	p->thread.sp = (unsigned long) fork_frame;
	p->thread.io_bitmap = NULL;
	p->thread.iopl_warn = 0;
	memset(p->thread.ptrace_bps, 0, sizeof(p->thread.ptrace_bps));

#ifdef CONFIG_X86_64
	current_save_fsgs();
	p->thread.fsindex = current->thread.fsindex;
	p->thread.fsbase = current->thread.fsbase;
	p->thread.gsindex = current->thread.gsindex;
	p->thread.gsbase = current->thread.gsbase;

	savesegment(es, p->thread.es);
	savesegment(ds, p->thread.ds);

	if (p->mm && (clone_flags & (CLONE_VM | CLONE_VFORK)) == CLONE_VM)
		set_bit(MM_CONTEXT_LOCK_LAM, &p->mm->context.flags);
#else
	p->thread.sp0 = (unsigned long) (childregs + 1);
	savesegment(gs, p->thread.gs);
	/*
	 * Clear all status flags including IF and set fixed bit. 64bit
	 * does not have this initialization as the frame does not contain
	 * flags. The flags consistency (especially vs. AC) is there
	 * ensured via objtool, which lacks 32bit support.
	 */
	frame->flags = X86_EFLAGS_FIXED;
#endif

	/*
	 * Allocate a new shadow stack for thread if needed. If shadow stack,
	 * is disabled, new_ssp will remain 0, and fpu_clone() will know not to
	 * update it.
	 */
	new_ssp = shstk_alloc_thread_stack(p, clone_flags, args->stack_size);
	if (IS_ERR_VALUE(new_ssp))
		return PTR_ERR((void *)new_ssp);

	fpu_clone(p, clone_flags, args->fn, new_ssp);

	/* Kernel thread ? */
	if (unlikely(p->flags & PF_KTHREAD)) {
		p->thread.pkru = pkru_get_init_value();
		memset(childregs, 0, sizeof(struct pt_regs));
		kthread_frame_init(frame, args->fn, args->fn_arg);
		return 0;
	}

	/*
	 * Clone current's PKRU value from hardware. tsk->thread.pkru
	 * is only valid when scheduled out.
	 */
	p->thread.pkru = read_pkru();

	frame->bx = 0;
	*childregs = *current_pt_regs();
	childregs->ax = 0;
	if (sp)
		childregs->sp = sp;

	if (unlikely(args->fn)) {
		/*
		 * A user space thread, but it doesn't return to
		 * ret_after_fork().
		 *
		 * In order to indicate that to tools like gdb,
		 * we reset the stack and instruction pointers.
		 *
		 * It does the same kernel frame setup to return to a kernel
		 * function that a kernel thread does.
		 */
		childregs->sp = 0;
		childregs->ip = 0;
		kthread_frame_init(frame, args->fn, args->fn_arg);
		return 0;
	}

	/* Set a new TLS for the child thread? */
	if (clone_flags & CLONE_SETTLS)
		ret = set_new_tls(p, tls);

	if (!ret && unlikely(test_tsk_thread_flag(current, TIF_IO_BITMAP)))
		io_bitmap_share(p);

	return ret;
}

static void pkru_flush_thread(void)
{
	/*
	 * If PKRU is enabled the default PKRU value has to be loaded into
	 * the hardware right here (similar to context switch).
	 */
	pkru_write_default();
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

void flush_thread(void)
{
	struct task_struct *tsk = current;

	flush_ptrace_hw_breakpoint(tsk);
	memset(tsk->thread.tls_array, 0, sizeof(tsk->thread.tls_array));
<<<<<<< HEAD
	/*
	 * Forget coprocessor state..
	 */
	tsk->fpu_counter = 0;
	clear_fpu(tsk);
	clear_used_math();
}

static void hard_disable_TSC(void)
{
	write_cr4(read_cr4() | X86_CR4_TSD);
=======

	fpu_flush_thread();
	pkru_flush_thread();
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

void disable_TSC(void)
{
	preempt_disable();
	if (!test_and_set_thread_flag(TIF_NOTSC))
		/*
		 * Must flip the CPU state synchronously with
		 * TIF_NOTSC in the current running context.
		 */
<<<<<<< HEAD
		hard_disable_TSC();
	preempt_enable();
}

static void hard_enable_TSC(void)
{
	write_cr4(read_cr4() & ~X86_CR4_TSD);
}

=======
		cr4_set_bits(X86_CR4_TSD);
	preempt_enable();
}

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
static void enable_TSC(void)
{
	preempt_disable();
	if (test_and_clear_thread_flag(TIF_NOTSC))
		/*
		 * Must flip the CPU state synchronously with
		 * TIF_NOTSC in the current running context.
		 */
<<<<<<< HEAD
		hard_enable_TSC();
=======
		cr4_clear_bits(X86_CR4_TSD);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	preempt_enable();
}

int get_tsc_mode(unsigned long adr)
{
	unsigned int val;

	if (test_thread_flag(TIF_NOTSC))
		val = PR_TSC_SIGSEGV;
	else
		val = PR_TSC_ENABLE;

	return put_user(val, (unsigned int __user *)adr);
}

int set_tsc_mode(unsigned int val)
{
	if (val == PR_TSC_SIGSEGV)
		disable_TSC();
	else if (val == PR_TSC_ENABLE)
		enable_TSC();
	else
		return -EINVAL;

	return 0;
}

<<<<<<< HEAD
void __switch_to_xtra(struct task_struct *prev_p, struct task_struct *next_p,
		      struct tss_struct *tss)
{
	struct thread_struct *prev, *next;

	prev = &prev_p->thread;
	next = &next_p->thread;

	if (test_tsk_thread_flag(prev_p, TIF_BLOCKSTEP) ^
	    test_tsk_thread_flag(next_p, TIF_BLOCKSTEP)) {
		unsigned long debugctl = get_debugctlmsr();

		debugctl &= ~DEBUGCTLMSR_BTF;
		if (test_tsk_thread_flag(next_p, TIF_BLOCKSTEP))
			debugctl |= DEBUGCTLMSR_BTF;

		update_debugctlmsr(debugctl);
	}

	if (test_tsk_thread_flag(prev_p, TIF_NOTSC) ^
	    test_tsk_thread_flag(next_p, TIF_NOTSC)) {
		/* prev and next are different */
		if (test_tsk_thread_flag(next_p, TIF_NOTSC))
			hard_disable_TSC();
		else
			hard_enable_TSC();
	}

	if (test_tsk_thread_flag(next_p, TIF_IO_BITMAP)) {
		/*
		 * Copy the relevant range of the IO bitmap.
		 * Normally this is 128 bytes or less:
		 */
		memcpy(tss->io_bitmap, next->io_bitmap_ptr,
		       max(prev->io_bitmap_max, next->io_bitmap_max));
	} else if (test_tsk_thread_flag(prev_p, TIF_IO_BITMAP)) {
		/*
		 * Clear any possible leftover bits:
		 */
		memset(tss->io_bitmap, 0xff, prev->io_bitmap_max);
	}
	propagate_user_return_notify(prev_p, next_p);
}

int sys_fork(struct pt_regs *regs)
{
	return do_fork(SIGCHLD, regs->sp, regs, 0, NULL, NULL);
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
int sys_vfork(struct pt_regs *regs)
{
	return do_fork(CLONE_VFORK | CLONE_VM | SIGCHLD, regs->sp, regs, 0,
		       NULL, NULL);
}

long
sys_clone(unsigned long clone_flags, unsigned long newsp,
	  void __user *parent_tid, void __user *child_tid, struct pt_regs *regs)
{
	if (!newsp)
		newsp = regs->sp;
	return do_fork(clone_flags, newsp, regs, 0, parent_tid, child_tid);
}

/*
 * This gets run with %si containing the
 * function to call, and %di containing
 * the "args".
 */
extern void kernel_thread_helper(void);

/*
 * Create a kernel thread
 */
int kernel_thread(int (*fn)(void *), void *arg, unsigned long flags)
{
	struct pt_regs regs;

	memset(&regs, 0, sizeof(regs));

	regs.si = (unsigned long) fn;
	regs.di = (unsigned long) arg;

#ifdef CONFIG_X86_32
	regs.ds = __USER_DS;
	regs.es = __USER_DS;
	regs.fs = __KERNEL_PERCPU;
	regs.gs = __KERNEL_STACK_CANARY;
#else
	regs.ss = __KERNEL_DS;
#endif

	regs.orig_ax = -1;
	regs.ip = (unsigned long) kernel_thread_helper;
	regs.cs = __KERNEL_CS | get_kernel_rpl();
	regs.flags = X86_EFLAGS_IF | X86_EFLAGS_BIT1;

	/* Ok, create the new process.. */
	return do_fork(flags | CLONE_VM | CLONE_UNTRACED, 0, &regs, 0, NULL, NULL);
}
EXPORT_SYMBOL(kernel_thread);

/*
 * sys_execve() executes a new program.
 */
long sys_execve(const char __user *name,
		const char __user *const __user *argv,
		const char __user *const __user *envp, struct pt_regs *regs)
{
	long error;
	char *filename;

	filename = getname(name);
	error = PTR_ERR(filename);
	if (IS_ERR(filename))
		return error;
	error = do_execve(filename, argv, envp, regs);

#ifdef CONFIG_X86_32
	if (error == 0) {
		/* Make sure we don't return using sysenter.. */
                set_thread_flag(TIF_IRET);
        }
#endif

	putname(filename);
	return error;
=======
DEFINE_PER_CPU(u64, msr_misc_features_shadow);

static void set_cpuid_faulting(bool on)
{
	u64 msrval;

	msrval = this_cpu_read(msr_misc_features_shadow);
	msrval &= ~MSR_MISC_FEATURES_ENABLES_CPUID_FAULT;
	msrval |= (on << MSR_MISC_FEATURES_ENABLES_CPUID_FAULT_BIT);
	this_cpu_write(msr_misc_features_shadow, msrval);
	wrmsrl(MSR_MISC_FEATURES_ENABLES, msrval);
}

static void disable_cpuid(void)
{
	preempt_disable();
	if (!test_and_set_thread_flag(TIF_NOCPUID)) {
		/*
		 * Must flip the CPU state synchronously with
		 * TIF_NOCPUID in the current running context.
		 */
		set_cpuid_faulting(true);
	}
	preempt_enable();
}

static void enable_cpuid(void)
{
	preempt_disable();
	if (test_and_clear_thread_flag(TIF_NOCPUID)) {
		/*
		 * Must flip the CPU state synchronously with
		 * TIF_NOCPUID in the current running context.
		 */
		set_cpuid_faulting(false);
	}
	preempt_enable();
}

static int get_cpuid_mode(void)
{
	return !test_thread_flag(TIF_NOCPUID);
}

static int set_cpuid_mode(unsigned long cpuid_enabled)
{
	if (!boot_cpu_has(X86_FEATURE_CPUID_FAULT))
		return -ENODEV;

	if (cpuid_enabled)
		enable_cpuid();
	else
		disable_cpuid();

	return 0;
}

/*
 * Called immediately after a successful exec.
 */
void arch_setup_new_exec(void)
{
	/* If cpuid was previously disabled for this task, re-enable it. */
	if (test_thread_flag(TIF_NOCPUID))
		enable_cpuid();

	/*
	 * Don't inherit TIF_SSBD across exec boundary when
	 * PR_SPEC_DISABLE_NOEXEC is used.
	 */
	if (test_thread_flag(TIF_SSBD) &&
	    task_spec_ssb_noexec(current)) {
		clear_thread_flag(TIF_SSBD);
		task_clear_spec_ssb_disable(current);
		task_clear_spec_ssb_noexec(current);
		speculation_ctrl_update(read_thread_flags());
	}

	mm_reset_untag_mask(current->mm);
}

#ifdef CONFIG_X86_IOPL_IOPERM
static inline void switch_to_bitmap(unsigned long tifp)
{
	/*
	 * Invalidate I/O bitmap if the previous task used it. This prevents
	 * any possible leakage of an active I/O bitmap.
	 *
	 * If the next task has an I/O bitmap it will handle it on exit to
	 * user mode.
	 */
	if (tifp & _TIF_IO_BITMAP)
		tss_invalidate_io_bitmap();
}

static void tss_copy_io_bitmap(struct tss_struct *tss, struct io_bitmap *iobm)
{
	/*
	 * Copy at least the byte range of the incoming tasks bitmap which
	 * covers the permitted I/O ports.
	 *
	 * If the previous task which used an I/O bitmap had more bits
	 * permitted, then the copy needs to cover those as well so they
	 * get turned off.
	 */
	memcpy(tss->io_bitmap.bitmap, iobm->bitmap,
	       max(tss->io_bitmap.prev_max, iobm->max));

	/*
	 * Store the new max and the sequence number of this bitmap
	 * and a pointer to the bitmap itself.
	 */
	tss->io_bitmap.prev_max = iobm->max;
	tss->io_bitmap.prev_sequence = iobm->sequence;
}

/**
 * native_tss_update_io_bitmap - Update I/O bitmap before exiting to user mode
 */
void native_tss_update_io_bitmap(void)
{
	struct tss_struct *tss = this_cpu_ptr(&cpu_tss_rw);
	struct thread_struct *t = &current->thread;
	u16 *base = &tss->x86_tss.io_bitmap_base;

	if (!test_thread_flag(TIF_IO_BITMAP)) {
		native_tss_invalidate_io_bitmap();
		return;
	}

	if (IS_ENABLED(CONFIG_X86_IOPL_IOPERM) && t->iopl_emul == 3) {
		*base = IO_BITMAP_OFFSET_VALID_ALL;
	} else {
		struct io_bitmap *iobm = t->io_bitmap;

		/*
		 * Only copy bitmap data when the sequence number differs. The
		 * update time is accounted to the incoming task.
		 */
		if (tss->io_bitmap.prev_sequence != iobm->sequence)
			tss_copy_io_bitmap(tss, iobm);

		/* Enable the bitmap */
		*base = IO_BITMAP_OFFSET_VALID_MAP;
	}

	/*
	 * Make sure that the TSS limit is covering the IO bitmap. It might have
	 * been cut down by a VMEXIT to 0x67 which would cause a subsequent I/O
	 * access from user space to trigger a #GP because the bitmap is outside
	 * the TSS limit.
	 */
	refresh_tss_limit();
}
#else /* CONFIG_X86_IOPL_IOPERM */
static inline void switch_to_bitmap(unsigned long tifp) { }
#endif

#ifdef CONFIG_SMP

struct ssb_state {
	struct ssb_state	*shared_state;
	raw_spinlock_t		lock;
	unsigned int		disable_state;
	unsigned long		local_state;
};

#define LSTATE_SSB	0

static DEFINE_PER_CPU(struct ssb_state, ssb_state);

void speculative_store_bypass_ht_init(void)
{
	struct ssb_state *st = this_cpu_ptr(&ssb_state);
	unsigned int this_cpu = smp_processor_id();
	unsigned int cpu;

	st->local_state = 0;

	/*
	 * Shared state setup happens once on the first bringup
	 * of the CPU. It's not destroyed on CPU hotunplug.
	 */
	if (st->shared_state)
		return;

	raw_spin_lock_init(&st->lock);

	/*
	 * Go over HT siblings and check whether one of them has set up the
	 * shared state pointer already.
	 */
	for_each_cpu(cpu, topology_sibling_cpumask(this_cpu)) {
		if (cpu == this_cpu)
			continue;

		if (!per_cpu(ssb_state, cpu).shared_state)
			continue;

		/* Link it to the state of the sibling: */
		st->shared_state = per_cpu(ssb_state, cpu).shared_state;
		return;
	}

	/*
	 * First HT sibling to come up on the core.  Link shared state of
	 * the first HT sibling to itself. The siblings on the same core
	 * which come up later will see the shared state pointer and link
	 * themselves to the state of this CPU.
	 */
	st->shared_state = st;
}

/*
 * Logic is: First HT sibling enables SSBD for both siblings in the core
 * and last sibling to disable it, disables it for the whole core. This how
 * MSR_SPEC_CTRL works in "hardware":
 *
 *  CORE_SPEC_CTRL = THREAD0_SPEC_CTRL | THREAD1_SPEC_CTRL
 */
static __always_inline void amd_set_core_ssb_state(unsigned long tifn)
{
	struct ssb_state *st = this_cpu_ptr(&ssb_state);
	u64 msr = x86_amd_ls_cfg_base;

	if (!static_cpu_has(X86_FEATURE_ZEN)) {
		msr |= ssbd_tif_to_amd_ls_cfg(tifn);
		wrmsrl(MSR_AMD64_LS_CFG, msr);
		return;
	}

	if (tifn & _TIF_SSBD) {
		/*
		 * Since this can race with prctl(), block reentry on the
		 * same CPU.
		 */
		if (__test_and_set_bit(LSTATE_SSB, &st->local_state))
			return;

		msr |= x86_amd_ls_cfg_ssbd_mask;

		raw_spin_lock(&st->shared_state->lock);
		/* First sibling enables SSBD: */
		if (!st->shared_state->disable_state)
			wrmsrl(MSR_AMD64_LS_CFG, msr);
		st->shared_state->disable_state++;
		raw_spin_unlock(&st->shared_state->lock);
	} else {
		if (!__test_and_clear_bit(LSTATE_SSB, &st->local_state))
			return;

		raw_spin_lock(&st->shared_state->lock);
		st->shared_state->disable_state--;
		if (!st->shared_state->disable_state)
			wrmsrl(MSR_AMD64_LS_CFG, msr);
		raw_spin_unlock(&st->shared_state->lock);
	}
}
#else
static __always_inline void amd_set_core_ssb_state(unsigned long tifn)
{
	u64 msr = x86_amd_ls_cfg_base | ssbd_tif_to_amd_ls_cfg(tifn);

	wrmsrl(MSR_AMD64_LS_CFG, msr);
}
#endif

static __always_inline void amd_set_ssb_virt_state(unsigned long tifn)
{
	/*
	 * SSBD has the same definition in SPEC_CTRL and VIRT_SPEC_CTRL,
	 * so ssbd_tif_to_spec_ctrl() just works.
	 */
	wrmsrl(MSR_AMD64_VIRT_SPEC_CTRL, ssbd_tif_to_spec_ctrl(tifn));
}

/*
 * Update the MSRs managing speculation control, during context switch.
 *
 * tifp: Previous task's thread flags
 * tifn: Next task's thread flags
 */
static __always_inline void __speculation_ctrl_update(unsigned long tifp,
						      unsigned long tifn)
{
	unsigned long tif_diff = tifp ^ tifn;
	u64 msr = x86_spec_ctrl_base;
	bool updmsr = false;

	lockdep_assert_irqs_disabled();

	/* Handle change of TIF_SSBD depending on the mitigation method. */
	if (static_cpu_has(X86_FEATURE_VIRT_SSBD)) {
		if (tif_diff & _TIF_SSBD)
			amd_set_ssb_virt_state(tifn);
	} else if (static_cpu_has(X86_FEATURE_LS_CFG_SSBD)) {
		if (tif_diff & _TIF_SSBD)
			amd_set_core_ssb_state(tifn);
	} else if (static_cpu_has(X86_FEATURE_SPEC_CTRL_SSBD) ||
		   static_cpu_has(X86_FEATURE_AMD_SSBD)) {
		updmsr |= !!(tif_diff & _TIF_SSBD);
		msr |= ssbd_tif_to_spec_ctrl(tifn);
	}

	/* Only evaluate TIF_SPEC_IB if conditional STIBP is enabled. */
	if (IS_ENABLED(CONFIG_SMP) &&
	    static_branch_unlikely(&switch_to_cond_stibp)) {
		updmsr |= !!(tif_diff & _TIF_SPEC_IB);
		msr |= stibp_tif_to_spec_ctrl(tifn);
	}

	if (updmsr)
		update_spec_ctrl_cond(msr);
}

static unsigned long speculation_ctrl_update_tif(struct task_struct *tsk)
{
	if (test_and_clear_tsk_thread_flag(tsk, TIF_SPEC_FORCE_UPDATE)) {
		if (task_spec_ssb_disable(tsk))
			set_tsk_thread_flag(tsk, TIF_SSBD);
		else
			clear_tsk_thread_flag(tsk, TIF_SSBD);

		if (task_spec_ib_disable(tsk))
			set_tsk_thread_flag(tsk, TIF_SPEC_IB);
		else
			clear_tsk_thread_flag(tsk, TIF_SPEC_IB);
	}
	/* Return the updated threadinfo flags*/
	return read_task_thread_flags(tsk);
}

void speculation_ctrl_update(unsigned long tif)
{
	unsigned long flags;

	/* Forced update. Make sure all relevant TIF flags are different */
	local_irq_save(flags);
	__speculation_ctrl_update(~tif, tif);
	local_irq_restore(flags);
}

/* Called from seccomp/prctl update */
void speculation_ctrl_update_current(void)
{
	preempt_disable();
	speculation_ctrl_update(speculation_ctrl_update_tif(current));
	preempt_enable();
}

static inline void cr4_toggle_bits_irqsoff(unsigned long mask)
{
	unsigned long newval, cr4 = this_cpu_read(cpu_tlbstate.cr4);

	newval = cr4 ^ mask;
	if (newval != cr4) {
		this_cpu_write(cpu_tlbstate.cr4, newval);
		__write_cr4(newval);
	}
}

void __switch_to_xtra(struct task_struct *prev_p, struct task_struct *next_p)
{
	unsigned long tifp, tifn;

	tifn = read_task_thread_flags(next_p);
	tifp = read_task_thread_flags(prev_p);

	switch_to_bitmap(tifp);

	propagate_user_return_notify(prev_p, next_p);

	if ((tifp & _TIF_BLOCKSTEP || tifn & _TIF_BLOCKSTEP) &&
	    arch_has_block_step()) {
		unsigned long debugctl, msk;

		rdmsrl(MSR_IA32_DEBUGCTLMSR, debugctl);
		debugctl &= ~DEBUGCTLMSR_BTF;
		msk = tifn & _TIF_BLOCKSTEP;
		debugctl |= (msk >> TIF_BLOCKSTEP) << DEBUGCTLMSR_BTF_SHIFT;
		wrmsrl(MSR_IA32_DEBUGCTLMSR, debugctl);
	}

	if ((tifp ^ tifn) & _TIF_NOTSC)
		cr4_toggle_bits_irqsoff(X86_CR4_TSD);

	if ((tifp ^ tifn) & _TIF_NOCPUID)
		set_cpuid_faulting(!!(tifn & _TIF_NOCPUID));

	if (likely(!((tifp | tifn) & _TIF_SPEC_FORCE_UPDATE))) {
		__speculation_ctrl_update(tifp, tifn);
	} else {
		speculation_ctrl_update_tif(prev_p);
		tifn = speculation_ctrl_update_tif(next_p);

		/* Enforce MSR update to ensure consistent state */
		__speculation_ctrl_update(~tifn, tifn);
	}
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

/*
 * Idle related variables and functions
 */
unsigned long boot_option_idle_override = IDLE_NO_OVERRIDE;
EXPORT_SYMBOL(boot_option_idle_override);

/*
<<<<<<< HEAD
 * Powermanagement idle function, if any..
 */
void (*pm_idle)(void);
#ifdef CONFIG_APM_MODULE
EXPORT_SYMBOL(pm_idle);
#endif

static inline int hlt_use_halt(void)
{
	return 1;
}

#ifndef CONFIG_SMP
static inline void play_dead(void)
=======
 * We use this if we don't have any better idle routine..
 */
void __cpuidle default_idle(void)
{
	raw_safe_halt();
	raw_local_irq_disable();
}
#if defined(CONFIG_APM_MODULE) || defined(CONFIG_HALTPOLL_CPUIDLE_MODULE)
EXPORT_SYMBOL(default_idle);
#endif

DEFINE_STATIC_CALL_NULL(x86_idle, default_idle);

static bool x86_idle_set(void)
{
	return !!static_call_query(x86_idle);
}

#ifndef CONFIG_SMP
static inline void __noreturn play_dead(void)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
{
	BUG();
}
#endif

<<<<<<< HEAD
#ifdef CONFIG_X86_64
void enter_idle(void)
{
	percpu_write(is_idle, 1);
	idle_notifier_call_chain(IDLE_START);
}

static void __exit_idle(void)
{
	if (x86_test_and_clear_bit_percpu(0, is_idle) == 0)
		return;
	idle_notifier_call_chain(IDLE_END);
}

/* Called from interrupts to signify idle end */
void exit_idle(void)
{
	/* idle loop has pid 0 */
	if (current->pid)
		return;
	__exit_idle();
}
#endif

/*
 * The idle thread. There's no useful work to be
 * done, so just try to conserve power and have a
 * low exit latency (ie sit in a loop waiting for
 * somebody to say that they'd like to reschedule)
 */
void cpu_idle(void)
{
	/*
	 * If we're the non-boot CPU, nothing set the stack canary up
	 * for us.  CPU0 already has it initialized but no harm in
	 * doing it again.  This is a good place for updating it, as
	 * we wont ever return from this function (so the invalid
	 * canaries already on the stack wont ever trigger).
	 */
	boot_init_stack_canary();
	current_thread_info()->status |= TS_POLLING;

	while (1) {
		tick_nohz_idle_enter();

		while (!need_resched()) {
			rmb();

			if (cpu_is_offline(smp_processor_id()))
				play_dead();

			/*
			 * Idle routines should keep interrupts disabled
			 * from here on, until they go to idle.
			 * Otherwise, idle callbacks can misfire.
			 */
			local_touch_nmi();
			local_irq_disable();

			enter_idle();

			/* Don't trace irqs off for idle */
			stop_critical_timings();

			/* enter_idle() needs rcu for notifiers */
			rcu_idle_enter();

			if (cpuidle_idle_call())
				pm_idle();

			rcu_idle_exit();
			start_critical_timings();

			/* In many cases the interrupt that ended idle
			   has already called exit_idle. But some idle
			   loops can be woken up without interrupt. */
			__exit_idle();
		}

		tick_nohz_idle_exit();
		preempt_enable_no_resched();
		schedule();
		preempt_disable();
	}
}

/*
 * We use this if we don't have any better
 * idle routine..
 */
void default_idle(void)
{
	if (hlt_use_halt()) {
		trace_power_start_rcuidle(POWER_CSTATE, 1, smp_processor_id());
		trace_cpu_idle_rcuidle(1, smp_processor_id());
		current_thread_info()->status &= ~TS_POLLING;
		/*
		 * TS_POLLING-cleared state must be visible before we
		 * test NEED_RESCHED:
		 */
		smp_mb();

		if (!need_resched())
			safe_halt();	/* enables interrupts racelessly */
		else
			local_irq_enable();
		current_thread_info()->status |= TS_POLLING;
		trace_power_end_rcuidle(smp_processor_id());
		trace_cpu_idle_rcuidle(PWR_EVENT_EXIT, smp_processor_id());
	} else {
		local_irq_enable();
		/* loop is done by the caller */
		cpu_relax();
	}
}
#ifdef CONFIG_APM_MODULE
EXPORT_SYMBOL(default_idle);
#endif

bool set_pm_idle_to_default(void)
{
	bool ret = !!pm_idle;

	pm_idle = default_idle;

	return ret;
}
void stop_this_cpu(void *dummy)
{
	local_irq_disable();
	/*
	 * Remove this CPU:
	 */
	set_cpu_online(smp_processor_id(), false);
	disable_local_APIC();

	for (;;) {
		if (hlt_works(smp_processor_id()))
			halt();
	}
}

static void do_nothing(void *unused)
{
}

/*
 * cpu_idle_wait - Used to ensure that all the CPUs discard old value of
 * pm_idle and update to new pm_idle value. Required while changing pm_idle
 * handler on SMP systems.
 *
 * Caller must have changed pm_idle to the new value before the call. Old
 * pm_idle value will not be used by any CPU after the return of this function.
 */
void cpu_idle_wait(void)
{
	smp_mb();
	/* kick all the CPUs so that they exit out of pm_idle */
	smp_call_function(do_nothing, NULL, 1);
}
EXPORT_SYMBOL_GPL(cpu_idle_wait);

/* Default MONITOR/MWAIT with no hints, used for default C1 state */
static void mwait_idle(void)
{
	if (!need_resched()) {
		trace_power_start_rcuidle(POWER_CSTATE, 1, smp_processor_id());
		trace_cpu_idle_rcuidle(1, smp_processor_id());
		if (this_cpu_has(X86_FEATURE_CLFLUSH_MONITOR))
			clflush((void *)&current_thread_info()->flags);

		__monitor((void *)&current_thread_info()->flags, 0, 0);
		smp_mb();
		if (!need_resched())
			__sti_mwait(0, 0);
		else
			local_irq_enable();
		trace_power_end_rcuidle(smp_processor_id());
		trace_cpu_idle_rcuidle(PWR_EVENT_EXIT, smp_processor_id());
	} else
		local_irq_enable();
}

/*
 * On SMP it's slightly faster (but much more power-consuming!)
 * to poll the ->work.need_resched flag instead of waiting for the
 * cross-CPU IPI to arrive. Use this option with caution.
 */
static void poll_idle(void)
{
	trace_power_start_rcuidle(POWER_CSTATE, 0, smp_processor_id());
	trace_cpu_idle_rcuidle(0, smp_processor_id());
	local_irq_enable();
	while (!need_resched())
		cpu_relax();
	trace_power_end_rcuidle(smp_processor_id());
	trace_cpu_idle_rcuidle(PWR_EVENT_EXIT, smp_processor_id());
}

/*
 * mwait selection logic:
 *
 * It depends on the CPU. For AMD CPUs that support MWAIT this is
 * wrong. Family 0x10 and 0x11 CPUs will enter C1 on HLT. Powersavings
 * then depend on a clock divisor and current Pstate of the core. If
 * all cores of a processor are in halt state (C1) the processor can
 * enter the C1E (C1 enhanced) state. If mwait is used this will never
 * happen.
 *
 * idle=mwait overrides this decision and forces the usage of mwait.
 */

#define MWAIT_INFO			0x05
#define MWAIT_ECX_EXTENDED_INFO		0x01
#define MWAIT_EDX_C1			0xf0

int mwait_usable(const struct cpuinfo_x86 *c)
{
	u32 eax, ebx, ecx, edx;

	if (boot_option_idle_override == IDLE_FORCE_MWAIT)
		return 1;

	if (c->cpuid_level < MWAIT_INFO)
		return 0;

	cpuid(MWAIT_INFO, &eax, &ebx, &ecx, &edx);
	/* Check, whether EDX has extended info about MWAIT */
	if (!(ecx & MWAIT_ECX_EXTENDED_INFO))
		return 1;

	/*
	 * edx enumeratios MONITOR/MWAIT extensions. Check, whether
	 * C1  supports MWAIT
	 */
	return (edx & MWAIT_EDX_C1);
}

bool amd_e400_c1e_detected;
EXPORT_SYMBOL(amd_e400_c1e_detected);

static cpumask_var_t amd_e400_c1e_mask;

void amd_e400_remove_cpu(int cpu)
{
	if (amd_e400_c1e_mask != NULL)
		cpumask_clear_cpu(cpu, amd_e400_c1e_mask);
}

/*
 * AMD Erratum 400 aware idle routine. We check for C1E active in the interrupt
 * pending message MSR. If we detect C1E, then we handle it the same
 * way as C3 power states (local apic timer and TSC stop)
 */
static void amd_e400_idle(void)
{
	if (need_resched())
		return;

	if (!amd_e400_c1e_detected) {
		u32 lo, hi;

		rdmsr(MSR_K8_INT_PENDING_MSG, lo, hi);

		if (lo & K8_INTP_C1E_ACTIVE_MASK) {
			amd_e400_c1e_detected = true;
			if (!boot_cpu_has(X86_FEATURE_NONSTOP_TSC))
				mark_tsc_unstable("TSC halt in AMD C1E");
			printk(KERN_INFO "System has AMD C1E enabled\n");
		}
	}

	if (amd_e400_c1e_detected) {
		int cpu = smp_processor_id();

		if (!cpumask_test_cpu(cpu, amd_e400_c1e_mask)) {
			cpumask_set_cpu(cpu, amd_e400_c1e_mask);
			/*
			 * Force broadcast so ACPI can not interfere.
			 */
			clockevents_notify(CLOCK_EVT_NOTIFY_BROADCAST_FORCE,
					   &cpu);
			printk(KERN_INFO "Switch to broadcast mode on CPU%d\n",
			       cpu);
		}
		clockevents_notify(CLOCK_EVT_NOTIFY_BROADCAST_ENTER, &cpu);

		default_idle();

		/*
		 * The switch back from broadcast mode needs to be
		 * called with interrupts disabled.
		 */
		 local_irq_disable();
		 clockevents_notify(CLOCK_EVT_NOTIFY_BROADCAST_EXIT, &cpu);
		 local_irq_enable();
	} else
		default_idle();
}

void __cpuinit select_idle_routine(const struct cpuinfo_x86 *c)
{
#ifdef CONFIG_SMP
	if (pm_idle == poll_idle && smp_num_siblings > 1) {
		printk_once(KERN_WARNING "WARNING: polling idle and HT enabled,"
			" performance may degrade.\n");
	}
#endif
	if (pm_idle)
		return;

	if (cpu_has(c, X86_FEATURE_MWAIT) && mwait_usable(c)) {
		/*
		 * One CPU supports mwait => All CPUs supports mwait
		 */
		printk(KERN_INFO "using mwait in idle threads.\n");
		pm_idle = mwait_idle;
	} else if (cpu_has_amd_erratum(amd_erratum_400)) {
		/* E400: APIC timer interrupt does not wake up CPU from C1e */
		printk(KERN_INFO "using AMD E400 aware idle routine\n");
		pm_idle = amd_e400_idle;
	} else
		pm_idle = default_idle;
}

void __init init_amd_e400_c1e_mask(void)
{
	/* If we're using amd_e400_idle, we need to allocate amd_e400_c1e_mask. */
	if (pm_idle == amd_e400_idle)
		zalloc_cpumask_var(&amd_e400_c1e_mask, GFP_KERNEL);
=======
void arch_cpu_idle_enter(void)
{
	tsc_verify_tsc_adjust(false);
	local_touch_nmi();
}

void __noreturn arch_cpu_idle_dead(void)
{
	play_dead();
}

/*
 * Called from the generic idle code.
 */
void __cpuidle arch_cpu_idle(void)
{
	static_call(x86_idle)();
}
EXPORT_SYMBOL_GPL(arch_cpu_idle);

#ifdef CONFIG_XEN
bool xen_set_default_idle(void)
{
	bool ret = x86_idle_set();

	static_call_update(x86_idle, default_idle);

	return ret;
}
#endif

struct cpumask cpus_stop_mask;

void __noreturn stop_this_cpu(void *dummy)
{
	struct cpuinfo_x86 *c = this_cpu_ptr(&cpu_info);
	unsigned int cpu = smp_processor_id();

	local_irq_disable();

	/*
	 * Remove this CPU from the online mask and disable it
	 * unconditionally. This might be redundant in case that the reboot
	 * vector was handled late and stop_other_cpus() sent an NMI.
	 *
	 * According to SDM and APM NMIs can be accepted even after soft
	 * disabling the local APIC.
	 */
	set_cpu_online(cpu, false);
	disable_local_APIC();
	mcheck_cpu_clear(c);

	/*
	 * Use wbinvd on processors that support SME. This provides support
	 * for performing a successful kexec when going from SME inactive
	 * to SME active (or vice-versa). The cache must be cleared so that
	 * if there are entries with the same physical address, both with and
	 * without the encryption bit, they don't race each other when flushed
	 * and potentially end up with the wrong entry being committed to
	 * memory.
	 *
	 * Test the CPUID bit directly because the machine might've cleared
	 * X86_FEATURE_SME due to cmdline options.
	 */
	if (c->extended_cpuid_level >= 0x8000001f && (cpuid_eax(0x8000001f) & BIT(0)))
		native_wbinvd();

	/*
	 * This brings a cache line back and dirties it, but
	 * native_stop_other_cpus() will overwrite cpus_stop_mask after it
	 * observed that all CPUs reported stop. This write will invalidate
	 * the related cache line on this CPU.
	 */
	cpumask_clear_cpu(cpu, &cpus_stop_mask);

	for (;;) {
		/*
		 * Use native_halt() so that memory contents don't change
		 * (stack usage and variables) after possibly issuing the
		 * native_wbinvd() above.
		 */
		native_halt();
	}
}

/*
 * Prefer MWAIT over HALT if MWAIT is supported, MWAIT_CPUID leaf
 * exists and whenever MONITOR/MWAIT extensions are present there is at
 * least one C1 substate.
 *
 * Do not prefer MWAIT if MONITOR instruction has a bug or idle=nomwait
 * is passed to kernel commandline parameter.
 */
static __init bool prefer_mwait_c1_over_halt(void)
{
	const struct cpuinfo_x86 *c = &boot_cpu_data;
	u32 eax, ebx, ecx, edx;

	/* If override is enforced on the command line, fall back to HALT. */
	if (boot_option_idle_override != IDLE_NO_OVERRIDE)
		return false;

	/* MWAIT is not supported on this platform. Fallback to HALT */
	if (!cpu_has(c, X86_FEATURE_MWAIT))
		return false;

	/* Monitor has a bug or APIC stops in C1E. Fallback to HALT */
	if (boot_cpu_has_bug(X86_BUG_MONITOR) || boot_cpu_has_bug(X86_BUG_AMD_APIC_C1E))
		return false;

	cpuid(CPUID_MWAIT_LEAF, &eax, &ebx, &ecx, &edx);

	/*
	 * If MWAIT extensions are not available, it is safe to use MWAIT
	 * with EAX=0, ECX=0.
	 */
	if (!(ecx & CPUID5_ECX_EXTENSIONS_SUPPORTED))
		return true;

	/*
	 * If MWAIT extensions are available, there should be at least one
	 * MWAIT C1 substate present.
	 */
	return !!(edx & MWAIT_C1_SUBSTATE_MASK);
}

/*
 * MONITOR/MWAIT with no hints, used for default C1 state. This invokes MWAIT
 * with interrupts enabled and no flags, which is backwards compatible with the
 * original MWAIT implementation.
 */
static __cpuidle void mwait_idle(void)
{
	if (!current_set_polling_and_test()) {
		if (this_cpu_has(X86_BUG_CLFLUSH_MONITOR)) {
			mb(); /* quirk */
			clflush((void *)&current_thread_info()->flags);
			mb(); /* quirk */
		}

		__monitor((void *)&current_thread_info()->flags, 0, 0);
		if (!need_resched()) {
			__sti_mwait(0, 0);
			raw_local_irq_disable();
		}
	}
	__current_clr_polling();
}

void __init select_idle_routine(void)
{
	if (boot_option_idle_override == IDLE_POLL) {
		if (IS_ENABLED(CONFIG_SMP) && __max_threads_per_core > 1)
			pr_warn_once("WARNING: polling idle and HT enabled, performance may degrade\n");
		return;
	}

	/* Required to guard against xen_set_default_idle() */
	if (x86_idle_set())
		return;

	if (prefer_mwait_c1_over_halt()) {
		pr_info("using mwait in idle threads\n");
		static_call_update(x86_idle, mwait_idle);
	} else if (cpu_feature_enabled(X86_FEATURE_TDX_GUEST)) {
		pr_info("using TDX aware idle routine\n");
		static_call_update(x86_idle, tdx_safe_halt);
	} else {
		static_call_update(x86_idle, default_idle);
	}
}

void amd_e400_c1e_apic_setup(void)
{
	if (boot_cpu_has_bug(X86_BUG_AMD_APIC_C1E)) {
		pr_info("Switch to broadcast mode on CPU%d\n", smp_processor_id());
		local_irq_disable();
		tick_broadcast_force();
		local_irq_enable();
	}
}

void __init arch_post_acpi_subsys_init(void)
{
	u32 lo, hi;

	if (!boot_cpu_has_bug(X86_BUG_AMD_E400))
		return;

	/*
	 * AMD E400 detection needs to happen after ACPI has been enabled. If
	 * the machine is affected K8_INTP_C1E_ACTIVE_MASK bits are set in
	 * MSR_K8_INT_PENDING_MSG.
	 */
	rdmsr(MSR_K8_INT_PENDING_MSG, lo, hi);
	if (!(lo & K8_INTP_C1E_ACTIVE_MASK))
		return;

	boot_cpu_set_bug(X86_BUG_AMD_APIC_C1E);

	if (!boot_cpu_has(X86_FEATURE_NONSTOP_TSC))
		mark_tsc_unstable("TSC halt in AMD C1E");

	if (IS_ENABLED(CONFIG_GENERIC_CLOCKEVENTS_BROADCAST_IDLE))
		static_branch_enable(&arch_needs_tick_broadcast);
	pr_info("System has AMD C1E erratum E400. Workaround enabled.\n");
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

static int __init idle_setup(char *str)
{
	if (!str)
		return -EINVAL;

	if (!strcmp(str, "poll")) {
<<<<<<< HEAD
		printk("using polling idle threads.\n");
		pm_idle = poll_idle;
		boot_option_idle_override = IDLE_POLL;
	} else if (!strcmp(str, "mwait")) {
		boot_option_idle_override = IDLE_FORCE_MWAIT;
		WARN_ONCE(1, "\"idle=mwait\" will be removed in 2012\n");
	} else if (!strcmp(str, "halt")) {
		/*
		 * When the boot option of idle=halt is added, halt is
		 * forced to be used for CPU idle. In such case CPU C2/C3
		 * won't be used again.
		 * To continue to load the CPU idle driver, don't touch
		 * the boot_option_idle_override.
		 */
		pm_idle = default_idle;
		boot_option_idle_override = IDLE_HALT;
	} else if (!strcmp(str, "nomwait")) {
		/*
		 * If the boot option of "idle=nomwait" is added,
		 * it means that mwait will be disabled for CPU C2/C3
		 * states. In such case it won't touch the variable
		 * of boot_option_idle_override.
		 */
		boot_option_idle_override = IDLE_NOMWAIT;
	} else
		return -1;
=======
		pr_info("using polling idle threads\n");
		boot_option_idle_override = IDLE_POLL;
		cpu_idle_poll_ctrl(true);
	} else if (!strcmp(str, "halt")) {
		/* 'idle=halt' HALT for idle. C-states are disabled. */
		boot_option_idle_override = IDLE_HALT;
	} else if (!strcmp(str, "nomwait")) {
		/* 'idle=nomwait' disables MWAIT for idle */
		boot_option_idle_override = IDLE_NOMWAIT;
	} else {
		return -EINVAL;
	}
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	return 0;
}
early_param("idle", idle_setup);

unsigned long arch_align_stack(unsigned long sp)
{
	if (!(current->personality & ADDR_NO_RANDOMIZE) && randomize_va_space)
<<<<<<< HEAD
		sp -= get_random_int() % 8192;
=======
		sp -= get_random_u32_below(8192);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	return sp & ~0xf;
}

unsigned long arch_randomize_brk(struct mm_struct *mm)
{
<<<<<<< HEAD
	unsigned long range_end = mm->brk + 0x02000000;
	return randomize_range(mm->brk, range_end, 0) ? : mm->brk;
}

=======
	if (mmap_is_ia32())
		return randomize_page(mm->brk, SZ_32M);

	return randomize_page(mm->brk, SZ_1G);
}

/*
 * Called from fs/proc with a reference on @p to find the function
 * which called into schedule(). This needs to be done carefully
 * because the task might wake up and we might look at a stack
 * changing under us.
 */
unsigned long __get_wchan(struct task_struct *p)
{
	struct unwind_state state;
	unsigned long addr = 0;

	if (!try_get_task_stack(p))
		return 0;

	for (unwind_start(&state, p, NULL, NULL); !unwind_done(&state);
	     unwind_next_frame(&state)) {
		addr = unwind_get_return_address(&state);
		if (!addr)
			break;
		if (in_sched_functions(addr))
			continue;
		break;
	}

	put_task_stack(p);

	return addr;
}

long do_arch_prctl_common(int option, unsigned long arg2)
{
	switch (option) {
	case ARCH_GET_CPUID:
		return get_cpuid_mode();
	case ARCH_SET_CPUID:
		return set_cpuid_mode(arg2);
	case ARCH_GET_XCOMP_SUPP:
	case ARCH_GET_XCOMP_PERM:
	case ARCH_REQ_XCOMP_PERM:
	case ARCH_GET_XCOMP_GUEST_PERM:
	case ARCH_REQ_XCOMP_GUEST_PERM:
		return fpu_xstate_prctl(option, arg2);
	}

	return -EINVAL;
}
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
