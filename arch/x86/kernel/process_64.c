<<<<<<< HEAD
=======
// SPDX-License-Identifier: GPL-2.0-only
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
/*
 *  Copyright (C) 1995  Linus Torvalds
 *
 *  Pentium III FXSR, SSE support
 *	Gareth Hughes <gareth@valinux.com>, May 2000
 *
 *  X86-64 port
 *	Andi Kleen.
 *
 *	CPU hotplug support - ashok.raj@intel.com
 */

/*
 * This file handles the architecture-dependent parts of process handling..
 */

#include <linux/cpu.h>
#include <linux/errno.h>
#include <linux/sched.h>
<<<<<<< HEAD
=======
#include <linux/sched/task.h>
#include <linux/sched/task_stack.h>
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#include <linux/fs.h>
#include <linux/kernel.h>
#include <linux/mm.h>
#include <linux/elfcore.h>
#include <linux/smp.h>
#include <linux/slab.h>
#include <linux/user.h>
#include <linux/interrupt.h>
#include <linux/delay.h>
<<<<<<< HEAD
#include <linux/module.h>
=======
#include <linux/export.h>
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#include <linux/ptrace.h>
#include <linux/notifier.h>
#include <linux/kprobes.h>
#include <linux/kdebug.h>
#include <linux/prctl.h>
#include <linux/uaccess.h>
#include <linux/io.h>
#include <linux/ftrace.h>
<<<<<<< HEAD

#include <asm/pgtable.h>
#include <asm/processor.h>
#include <asm/i387.h>
#include <asm/fpu-internal.h>
=======
#include <linux/syscalls.h>
#include <linux/iommu.h>

#include <asm/processor.h>
#include <asm/pkru.h>
#include <asm/fpu/sched.h>
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#include <asm/mmu_context.h>
#include <asm/prctl.h>
#include <asm/desc.h>
#include <asm/proto.h>
#include <asm/ia32.h>
<<<<<<< HEAD
#include <asm/idle.h>
#include <asm/syscalls.h>
#include <asm/debugreg.h>
#include <asm/switch_to.h>
#include <asm/xen/hypervisor.h>

asmlinkage extern void ret_from_fork(void);

DEFINE_PER_CPU(unsigned long, old_rsp);

/* Prints also some state that isn't saved in the pt_regs */
void __show_regs(struct pt_regs *regs, int all)
=======
#include <asm/debugreg.h>
#include <asm/switch_to.h>
#include <asm/xen/hypervisor.h>
#include <asm/vdso.h>
#include <asm/resctrl.h>
#include <asm/unistd.h>
#include <asm/fsgsbase.h>
#include <asm/fred.h>
#ifdef CONFIG_IA32_EMULATION
/* Not included via unistd.h */
#include <asm/unistd_32_ia32.h>
#endif

#include "process.h"

/* Prints also some state that isn't saved in the pt_regs */
void __show_regs(struct pt_regs *regs, enum show_regs_mode mode,
		 const char *log_lvl)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
{
	unsigned long cr0 = 0L, cr2 = 0L, cr3 = 0L, cr4 = 0L, fs, gs, shadowgs;
	unsigned long d0, d1, d2, d3, d6, d7;
	unsigned int fsindex, gsindex;
<<<<<<< HEAD
	unsigned int ds, cs, es;

	show_regs_common();
	printk(KERN_DEFAULT "RIP: %04lx:[<%016lx>] ", regs->cs & 0xffff, regs->ip);
	printk_address(regs->ip, 1);
	printk(KERN_DEFAULT "RSP: %04lx:%016lx  EFLAGS: %08lx\n", regs->ss,
			regs->sp, regs->flags);
	printk(KERN_DEFAULT "RAX: %016lx RBX: %016lx RCX: %016lx\n",
	       regs->ax, regs->bx, regs->cx);
	printk(KERN_DEFAULT "RDX: %016lx RSI: %016lx RDI: %016lx\n",
	       regs->dx, regs->si, regs->di);
	printk(KERN_DEFAULT "RBP: %016lx R08: %016lx R09: %016lx\n",
	       regs->bp, regs->r8, regs->r9);
	printk(KERN_DEFAULT "R10: %016lx R11: %016lx R12: %016lx\n",
	       regs->r10, regs->r11, regs->r12);
	printk(KERN_DEFAULT "R13: %016lx R14: %016lx R15: %016lx\n",
	       regs->r13, regs->r14, regs->r15);

	asm("movl %%ds,%0" : "=r" (ds));
	asm("movl %%cs,%0" : "=r" (cs));
=======
	unsigned int ds, es;

	show_iret_regs(regs, log_lvl);

	if (regs->orig_ax != -1)
		pr_cont(" ORIG_RAX: %016lx\n", regs->orig_ax);
	else
		pr_cont("\n");

	printk("%sRAX: %016lx RBX: %016lx RCX: %016lx\n",
	       log_lvl, regs->ax, regs->bx, regs->cx);
	printk("%sRDX: %016lx RSI: %016lx RDI: %016lx\n",
	       log_lvl, regs->dx, regs->si, regs->di);
	printk("%sRBP: %016lx R08: %016lx R09: %016lx\n",
	       log_lvl, regs->bp, regs->r8, regs->r9);
	printk("%sR10: %016lx R11: %016lx R12: %016lx\n",
	       log_lvl, regs->r10, regs->r11, regs->r12);
	printk("%sR13: %016lx R14: %016lx R15: %016lx\n",
	       log_lvl, regs->r13, regs->r14, regs->r15);

	if (mode == SHOW_REGS_SHORT)
		return;

	if (mode == SHOW_REGS_USER) {
		rdmsrl(MSR_FS_BASE, fs);
		rdmsrl(MSR_KERNEL_GS_BASE, shadowgs);
		printk("%sFS:  %016lx GS:  %016lx\n",
		       log_lvl, fs, shadowgs);
		return;
	}

	asm("movl %%ds,%0" : "=r" (ds));
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	asm("movl %%es,%0" : "=r" (es));
	asm("movl %%fs,%0" : "=r" (fsindex));
	asm("movl %%gs,%0" : "=r" (gsindex));

	rdmsrl(MSR_FS_BASE, fs);
	rdmsrl(MSR_GS_BASE, gs);
	rdmsrl(MSR_KERNEL_GS_BASE, shadowgs);

<<<<<<< HEAD
	if (!all)
		return;

	cr0 = read_cr0();
	cr2 = read_cr2();
	cr3 = read_cr3();
	cr4 = read_cr4();

	printk(KERN_DEFAULT "FS:  %016lx(%04x) GS:%016lx(%04x) knlGS:%016lx\n",
	       fs, fsindex, gs, gsindex, shadowgs);
	printk(KERN_DEFAULT "CS:  %04x DS: %04x ES: %04x CR0: %016lx\n", cs, ds,
			es, cr0);
	printk(KERN_DEFAULT "CR2: %016lx CR3: %016lx CR4: %016lx\n", cr2, cr3,
			cr4);
=======
	cr0 = read_cr0();
	cr2 = read_cr2();
	cr3 = __read_cr3();
	cr4 = __read_cr4();

	printk("%sFS:  %016lx(%04x) GS:%016lx(%04x) knlGS:%016lx\n",
	       log_lvl, fs, fsindex, gs, gsindex, shadowgs);
	printk("%sCS:  %04x DS: %04x ES: %04x CR0: %016lx\n",
		log_lvl, regs->cs, ds, es, cr0);
	printk("%sCR2: %016lx CR3: %016lx CR4: %016lx\n",
		log_lvl, cr2, cr3, cr4);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	get_debugreg(d0, 0);
	get_debugreg(d1, 1);
	get_debugreg(d2, 2);
<<<<<<< HEAD
	printk(KERN_DEFAULT "DR0: %016lx DR1: %016lx DR2: %016lx\n", d0, d1, d2);
	get_debugreg(d3, 3);
	get_debugreg(d6, 6);
	get_debugreg(d7, 7);
	printk(KERN_DEFAULT "DR3: %016lx DR6: %016lx DR7: %016lx\n", d3, d6, d7);
=======
	get_debugreg(d3, 3);
	get_debugreg(d6, 6);
	get_debugreg(d7, 7);

	/* Only print out debug registers if they are in their non-default state. */
	if (!((d0 == 0) && (d1 == 0) && (d2 == 0) && (d3 == 0) &&
	    (d6 == DR6_RESERVED) && (d7 == 0x400))) {
		printk("%sDR0: %016lx DR1: %016lx DR2: %016lx\n",
		       log_lvl, d0, d1, d2);
		printk("%sDR3: %016lx DR6: %016lx DR7: %016lx\n",
		       log_lvl, d3, d6, d7);
	}

	if (cr4 & X86_CR4_PKE)
		printk("%sPKRU: %08x\n", log_lvl, read_pkru());
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

void release_thread(struct task_struct *dead_task)
{
<<<<<<< HEAD
	if (dead_task->mm) {
		if (dead_task->mm->context.ldt) {
			printk("WARNING: dead process %8s still has LDT? <%p/%d>\n",
					dead_task->comm,
					dead_task->mm->context.ldt->entries,
					dead_task->mm->context.ldt->size);
			BUG();
		}
	}
}

static inline void set_32bit_tls(struct task_struct *t, int tls, u32 addr)
{
	struct user_desc ud = {
		.base_addr = addr,
		.limit = 0xfffff,
		.seg_32bit = 1,
		.limit_in_pages = 1,
		.useable = 1,
	};
	struct desc_struct *desc = t->thread.tls_array;
	desc += tls;
	fill_ldt(desc, &ud);
}

static inline u32 read_32bit_tls(struct task_struct *t, int tls)
{
	return get_desc_base(&t->thread.tls_array[tls]);
}

/*
 * This gets called before we allocate a new thread and copy
 * the current task into it.
 */
void prepare_to_copy(struct task_struct *tsk)
{
	unlazy_fpu(tsk);
}

int copy_thread(unsigned long clone_flags, unsigned long sp,
		unsigned long unused,
	struct task_struct *p, struct pt_regs *regs)
{
	int err;
	struct pt_regs *childregs;
	struct task_struct *me = current;

	childregs = ((struct pt_regs *)
			(THREAD_SIZE + task_stack_page(p))) - 1;
	*childregs = *regs;

	childregs->ax = 0;
	if (user_mode(regs))
		childregs->sp = sp;
	else
		childregs->sp = (unsigned long)childregs;

	p->thread.sp = (unsigned long) childregs;
	p->thread.sp0 = (unsigned long) (childregs+1);
	p->thread.usersp = me->thread.usersp;

	set_tsk_thread_flag(p, TIF_FORK);

	p->fpu_counter = 0;
	p->thread.io_bitmap_ptr = NULL;

	savesegment(gs, p->thread.gsindex);
	p->thread.gs = p->thread.gsindex ? 0 : me->thread.gs;
	savesegment(fs, p->thread.fsindex);
	p->thread.fs = p->thread.fsindex ? 0 : me->thread.fs;
	savesegment(es, p->thread.es);
	savesegment(ds, p->thread.ds);

	err = -ENOMEM;
	memset(p->thread.ptrace_bps, 0, sizeof(p->thread.ptrace_bps));

	if (unlikely(test_tsk_thread_flag(me, TIF_IO_BITMAP))) {
		p->thread.io_bitmap_ptr = kmemdup(me->thread.io_bitmap_ptr,
						  IO_BITMAP_BYTES, GFP_KERNEL);
		if (!p->thread.io_bitmap_ptr) {
			p->thread.io_bitmap_max = 0;
			return -ENOMEM;
		}
		set_tsk_thread_flag(p, TIF_IO_BITMAP);
	}

	/*
	 * Set a new TLS for the child thread?
	 */
	if (clone_flags & CLONE_SETTLS) {
#ifdef CONFIG_IA32_EMULATION
		if (test_thread_flag(TIF_IA32))
			err = do_set_thread_area(p, -1,
				(struct user_desc __user *)childregs->si, 0);
		else
#endif
			err = do_arch_prctl(p, ARCH_SET_FS, childregs->r8);
		if (err)
			goto out;
	}
	err = 0;
out:
	if (err && p->thread.io_bitmap_ptr) {
		kfree(p->thread.io_bitmap_ptr);
		p->thread.io_bitmap_max = 0;
	}

	return err;
=======
	WARN_ON(dead_task->mm);
}

enum which_selector {
	FS,
	GS
};

/*
 * Out of line to be protected from kprobes and tracing. If this would be
 * traced or probed than any access to a per CPU variable happens with
 * the wrong GS.
 *
 * It is not used on Xen paravirt. When paravirt support is needed, it
 * needs to be renamed with native_ prefix.
 */
static noinstr unsigned long __rdgsbase_inactive(void)
{
	unsigned long gsbase;

	lockdep_assert_irqs_disabled();

	/*
	 * SWAPGS is no longer needed thus NOT allowed with FRED because
	 * FRED transitions ensure that an operating system can _always_
	 * operate with its own GS base address:
	 * - For events that occur in ring 3, FRED event delivery swaps
	 *   the GS base address with the IA32_KERNEL_GS_BASE MSR.
	 * - ERETU (the FRED transition that returns to ring 3) also swaps
	 *   the GS base address with the IA32_KERNEL_GS_BASE MSR.
	 *
	 * And the operating system can still setup the GS segment for a
	 * user thread without the need of loading a user thread GS with:
	 * - Using LKGS, available with FRED, to modify other attributes
	 *   of the GS segment without compromising its ability always to
	 *   operate with its own GS base address.
	 * - Accessing the GS segment base address for a user thread as
	 *   before using RDMSR or WRMSR on the IA32_KERNEL_GS_BASE MSR.
	 *
	 * Note, LKGS loads the GS base address into the IA32_KERNEL_GS_BASE
	 * MSR instead of the GS segment’s descriptor cache. As such, the
	 * operating system never changes its runtime GS base address.
	 */
	if (!cpu_feature_enabled(X86_FEATURE_FRED) &&
	    !cpu_feature_enabled(X86_FEATURE_XENPV)) {
		native_swapgs();
		gsbase = rdgsbase();
		native_swapgs();
	} else {
		instrumentation_begin();
		rdmsrl(MSR_KERNEL_GS_BASE, gsbase);
		instrumentation_end();
	}

	return gsbase;
}

/*
 * Out of line to be protected from kprobes and tracing. If this would be
 * traced or probed than any access to a per CPU variable happens with
 * the wrong GS.
 *
 * It is not used on Xen paravirt. When paravirt support is needed, it
 * needs to be renamed with native_ prefix.
 */
static noinstr void __wrgsbase_inactive(unsigned long gsbase)
{
	lockdep_assert_irqs_disabled();

	if (!cpu_feature_enabled(X86_FEATURE_FRED) &&
	    !cpu_feature_enabled(X86_FEATURE_XENPV)) {
		native_swapgs();
		wrgsbase(gsbase);
		native_swapgs();
	} else {
		instrumentation_begin();
		wrmsrl(MSR_KERNEL_GS_BASE, gsbase);
		instrumentation_end();
	}
}

/*
 * Saves the FS or GS base for an outgoing thread if FSGSBASE extensions are
 * not available.  The goal is to be reasonably fast on non-FSGSBASE systems.
 * It's forcibly inlined because it'll generate better code and this function
 * is hot.
 */
static __always_inline void save_base_legacy(struct task_struct *prev_p,
					     unsigned short selector,
					     enum which_selector which)
{
	if (likely(selector == 0)) {
		/*
		 * On Intel (without X86_BUG_NULL_SEG), the segment base could
		 * be the pre-existing saved base or it could be zero.  On AMD
		 * (with X86_BUG_NULL_SEG), the segment base could be almost
		 * anything.
		 *
		 * This branch is very hot (it's hit twice on almost every
		 * context switch between 64-bit programs), and avoiding
		 * the RDMSR helps a lot, so we just assume that whatever
		 * value is already saved is correct.  This matches historical
		 * Linux behavior, so it won't break existing applications.
		 *
		 * To avoid leaking state, on non-X86_BUG_NULL_SEG CPUs, if we
		 * report that the base is zero, it needs to actually be zero:
		 * see the corresponding logic in load_seg_legacy.
		 */
	} else {
		/*
		 * If the selector is 1, 2, or 3, then the base is zero on
		 * !X86_BUG_NULL_SEG CPUs and could be anything on
		 * X86_BUG_NULL_SEG CPUs.  In the latter case, Linux
		 * has never attempted to preserve the base across context
		 * switches.
		 *
		 * If selector > 3, then it refers to a real segment, and
		 * saving the base isn't necessary.
		 */
		if (which == FS)
			prev_p->thread.fsbase = 0;
		else
			prev_p->thread.gsbase = 0;
	}
}

static __always_inline void save_fsgs(struct task_struct *task)
{
	savesegment(fs, task->thread.fsindex);
	savesegment(gs, task->thread.gsindex);
	if (static_cpu_has(X86_FEATURE_FSGSBASE)) {
		/*
		 * If FSGSBASE is enabled, we can't make any useful guesses
		 * about the base, and user code expects us to save the current
		 * value.  Fortunately, reading the base directly is efficient.
		 */
		task->thread.fsbase = rdfsbase();
		task->thread.gsbase = __rdgsbase_inactive();
	} else {
		save_base_legacy(task, task->thread.fsindex, FS);
		save_base_legacy(task, task->thread.gsindex, GS);
	}
}

/*
 * While a process is running,current->thread.fsbase and current->thread.gsbase
 * may not match the corresponding CPU registers (see save_base_legacy()).
 */
void current_save_fsgs(void)
{
	unsigned long flags;

	/* Interrupts need to be off for FSGSBASE */
	local_irq_save(flags);
	save_fsgs(current);
	local_irq_restore(flags);
}
#if IS_ENABLED(CONFIG_KVM)
EXPORT_SYMBOL_GPL(current_save_fsgs);
#endif

static __always_inline void loadseg(enum which_selector which,
				    unsigned short sel)
{
	if (which == FS)
		loadsegment(fs, sel);
	else
		load_gs_index(sel);
}

static __always_inline void load_seg_legacy(unsigned short prev_index,
					    unsigned long prev_base,
					    unsigned short next_index,
					    unsigned long next_base,
					    enum which_selector which)
{
	if (likely(next_index <= 3)) {
		/*
		 * The next task is using 64-bit TLS, is not using this
		 * segment at all, or is having fun with arcane CPU features.
		 */
		if (next_base == 0) {
			/*
			 * Nasty case: on AMD CPUs, we need to forcibly zero
			 * the base.
			 */
			if (static_cpu_has_bug(X86_BUG_NULL_SEG)) {
				loadseg(which, __USER_DS);
				loadseg(which, next_index);
			} else {
				/*
				 * We could try to exhaustively detect cases
				 * under which we can skip the segment load,
				 * but there's really only one case that matters
				 * for performance: if both the previous and
				 * next states are fully zeroed, we can skip
				 * the load.
				 *
				 * (This assumes that prev_base == 0 has no
				 * false positives.  This is the case on
				 * Intel-style CPUs.)
				 */
				if (likely(prev_index | next_index | prev_base))
					loadseg(which, next_index);
			}
		} else {
			if (prev_index != next_index)
				loadseg(which, next_index);
			wrmsrl(which == FS ? MSR_FS_BASE : MSR_KERNEL_GS_BASE,
			       next_base);
		}
	} else {
		/*
		 * The next task is using a real segment.  Loading the selector
		 * is sufficient.
		 */
		loadseg(which, next_index);
	}
}

/*
 * Store prev's PKRU value and load next's PKRU value if they differ. PKRU
 * is not XSTATE managed on context switch because that would require a
 * lookup in the task's FPU xsave buffer and require to keep that updated
 * in various places.
 */
static __always_inline void x86_pkru_load(struct thread_struct *prev,
					  struct thread_struct *next)
{
	if (!cpu_feature_enabled(X86_FEATURE_OSPKE))
		return;

	/* Stash the prev task's value: */
	prev->pkru = rdpkru();

	/*
	 * PKRU writes are slightly expensive.  Avoid them when not
	 * strictly necessary:
	 */
	if (prev->pkru != next->pkru)
		wrpkru(next->pkru);
}

static __always_inline void x86_fsgsbase_load(struct thread_struct *prev,
					      struct thread_struct *next)
{
	if (static_cpu_has(X86_FEATURE_FSGSBASE)) {
		/* Update the FS and GS selectors if they could have changed. */
		if (unlikely(prev->fsindex || next->fsindex))
			loadseg(FS, next->fsindex);
		if (unlikely(prev->gsindex || next->gsindex))
			loadseg(GS, next->gsindex);

		/* Update the bases. */
		wrfsbase(next->fsbase);
		__wrgsbase_inactive(next->gsbase);
	} else {
		load_seg_legacy(prev->fsindex, prev->fsbase,
				next->fsindex, next->fsbase, FS);
		load_seg_legacy(prev->gsindex, prev->gsbase,
				next->gsindex, next->gsbase, GS);
	}
}

unsigned long x86_fsgsbase_read_task(struct task_struct *task,
				     unsigned short selector)
{
	unsigned short idx = selector >> 3;
	unsigned long base;

	if (likely((selector & SEGMENT_TI_MASK) == 0)) {
		if (unlikely(idx >= GDT_ENTRIES))
			return 0;

		/*
		 * There are no user segments in the GDT with nonzero bases
		 * other than the TLS segments.
		 */
		if (idx < GDT_ENTRY_TLS_MIN || idx > GDT_ENTRY_TLS_MAX)
			return 0;

		idx -= GDT_ENTRY_TLS_MIN;
		base = get_desc_base(&task->thread.tls_array[idx]);
	} else {
#ifdef CONFIG_MODIFY_LDT_SYSCALL
		struct ldt_struct *ldt;

		/*
		 * If performance here mattered, we could protect the LDT
		 * with RCU.  This is a slow path, though, so we can just
		 * take the mutex.
		 */
		mutex_lock(&task->mm->context.lock);
		ldt = task->mm->context.ldt;
		if (unlikely(!ldt || idx >= ldt->nr_entries))
			base = 0;
		else
			base = get_desc_base(ldt->entries + idx);
		mutex_unlock(&task->mm->context.lock);
#else
		base = 0;
#endif
	}

	return base;
}

unsigned long x86_gsbase_read_cpu_inactive(void)
{
	unsigned long gsbase;

	if (boot_cpu_has(X86_FEATURE_FSGSBASE)) {
		unsigned long flags;

		local_irq_save(flags);
		gsbase = __rdgsbase_inactive();
		local_irq_restore(flags);
	} else {
		rdmsrl(MSR_KERNEL_GS_BASE, gsbase);
	}

	return gsbase;
}

void x86_gsbase_write_cpu_inactive(unsigned long gsbase)
{
	if (boot_cpu_has(X86_FEATURE_FSGSBASE)) {
		unsigned long flags;

		local_irq_save(flags);
		__wrgsbase_inactive(gsbase);
		local_irq_restore(flags);
	} else {
		wrmsrl(MSR_KERNEL_GS_BASE, gsbase);
	}
}

unsigned long x86_fsbase_read_task(struct task_struct *task)
{
	unsigned long fsbase;

	if (task == current)
		fsbase = x86_fsbase_read_cpu();
	else if (boot_cpu_has(X86_FEATURE_FSGSBASE) ||
		 (task->thread.fsindex == 0))
		fsbase = task->thread.fsbase;
	else
		fsbase = x86_fsgsbase_read_task(task, task->thread.fsindex);

	return fsbase;
}

unsigned long x86_gsbase_read_task(struct task_struct *task)
{
	unsigned long gsbase;

	if (task == current)
		gsbase = x86_gsbase_read_cpu_inactive();
	else if (boot_cpu_has(X86_FEATURE_FSGSBASE) ||
		 (task->thread.gsindex == 0))
		gsbase = task->thread.gsbase;
	else
		gsbase = x86_fsgsbase_read_task(task, task->thread.gsindex);

	return gsbase;
}

void x86_fsbase_write_task(struct task_struct *task, unsigned long fsbase)
{
	WARN_ON_ONCE(task == current);

	task->thread.fsbase = fsbase;
}

void x86_gsbase_write_task(struct task_struct *task, unsigned long gsbase)
{
	WARN_ON_ONCE(task == current);

	task->thread.gsbase = gsbase;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

static void
start_thread_common(struct pt_regs *regs, unsigned long new_ip,
		    unsigned long new_sp,
<<<<<<< HEAD
		    unsigned int _cs, unsigned int _ss, unsigned int _ds)
{
=======
		    u16 _cs, u16 _ss, u16 _ds)
{
	WARN_ON_ONCE(regs != current_pt_regs());

	if (static_cpu_has(X86_BUG_NULL_SEG)) {
		/* Loading zero below won't clear the base. */
		loadsegment(fs, __USER_DS);
		load_gs_index(__USER_DS);
	}

	reset_thread_features();

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	loadsegment(fs, 0);
	loadsegment(es, _ds);
	loadsegment(ds, _ds);
	load_gs_index(0);
<<<<<<< HEAD
	current->thread.usersp	= new_sp;
	regs->ip		= new_ip;
	regs->sp		= new_sp;
	percpu_write(old_rsp, new_sp);
	regs->cs		= _cs;
	regs->ss		= _ss;
	regs->flags		= X86_EFLAGS_IF;
	/*
	 * Free the old FP and other extended state
	 */
	free_thread_xstate(current);
=======

	regs->ip	= new_ip;
	regs->sp	= new_sp;
	regs->csx	= _cs;
	regs->ssx	= _ss;
	/*
	 * Allow single-step trap and NMI when starting a new task, thus
	 * once the new task enters user space, single-step trap and NMI
	 * are both enabled immediately.
	 *
	 * Entering a new task is logically speaking a return from a
	 * system call (exec, fork, clone, etc.). As such, if ptrace
	 * enables single stepping a single step exception should be
	 * allowed to trigger immediately upon entering user space.
	 * This is not optional.
	 *
	 * NMI should *never* be disabled in user space. As such, this
	 * is an optional, opportunistic way to catch errors.
	 *
	 * Paranoia: High-order 48 bits above the lowest 16 bit SS are
	 * discarded by the legacy IRET instruction on all Intel, AMD,
	 * and Cyrix/Centaur/VIA CPUs, thus can be set unconditionally,
	 * even when FRED is not enabled. But we choose the safer side
	 * to use these bits only when FRED is enabled.
	 */
	if (cpu_feature_enabled(X86_FEATURE_FRED)) {
		regs->fred_ss.swevent	= true;
		regs->fred_ss.nmi	= true;
	}

	regs->flags	= X86_EFLAGS_IF | X86_EFLAGS_FIXED;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

void
start_thread(struct pt_regs *regs, unsigned long new_ip, unsigned long new_sp)
{
	start_thread_common(regs, new_ip, new_sp,
			    __USER_CS, __USER_DS, 0);
}
<<<<<<< HEAD

#ifdef CONFIG_IA32_EMULATION
void start_thread_ia32(struct pt_regs *regs, u32 new_ip, u32 new_sp)
{
	start_thread_common(regs, new_ip, new_sp,
			    test_thread_flag(TIF_X32)
			    ? __USER_CS : __USER32_CS,
=======
EXPORT_SYMBOL_GPL(start_thread);

#ifdef CONFIG_COMPAT
void compat_start_thread(struct pt_regs *regs, u32 new_ip, u32 new_sp, bool x32)
{
	start_thread_common(regs, new_ip, new_sp,
			    x32 ? __USER_CS : __USER32_CS,
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
			    __USER_DS, __USER_DS);
}
#endif

/*
 *	switch_to(x,y) should switch tasks from x to y.
 *
 * This could still be optimized:
 * - fold all the options into a flag word and test it with a single test.
 * - could test fs/gs bitsliced
 *
 * Kprobes not supported here. Set the probe on schedule instead.
 * Function graph tracer not supported too.
 */
<<<<<<< HEAD
__notrace_funcgraph struct task_struct *
=======
__no_kmsan_checks
__visible __notrace_funcgraph struct task_struct *
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
__switch_to(struct task_struct *prev_p, struct task_struct *next_p)
{
	struct thread_struct *prev = &prev_p->thread;
	struct thread_struct *next = &next_p->thread;
	int cpu = smp_processor_id();
<<<<<<< HEAD
	struct tss_struct *tss = &per_cpu(init_tss, cpu);
	unsigned fsindex, gsindex;
	fpu_switch_t fpu;

	fpu = switch_fpu_prepare(prev_p, next_p, cpu);

	/* Reload esp0 and ss1. */
	load_sp0(tss, next);
=======

	WARN_ON_ONCE(IS_ENABLED(CONFIG_DEBUG_ENTRY) &&
		     this_cpu_read(pcpu_hot.hardirq_stack_inuse));

	if (!test_tsk_thread_flag(prev_p, TIF_NEED_FPU_LOAD))
		switch_fpu_prepare(prev_p, cpu);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	/* We must save %fs and %gs before load_TLS() because
	 * %fs and %gs may be cleared by load_TLS().
	 *
	 * (e.g. xen_load_tls())
	 */
<<<<<<< HEAD
	savesegment(fs, fsindex);
	savesegment(gs, gsindex);
=======
	save_fsgs(prev_p);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	/*
	 * Load TLS before restoring any segments so that segment loads
	 * reference the correct GDT entries.
	 */
	load_TLS(next, cpu);

	/*
	 * Leave lazy mode, flushing any hypercalls made here.  This
	 * must be done after loading TLS entries in the GDT but before
<<<<<<< HEAD
	 * loading segments that might reference them, and and it must
	 * be done before math_state_restore, so the TS bit is up to
	 * date.
=======
	 * loading segments that might reference them.
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	 */
	arch_end_context_switch(next_p);

	/* Switch DS and ES.
	 *
	 * Reading them only returns the selectors, but writing them (if
	 * nonzero) loads the full descriptor from the GDT or LDT.  The
	 * LDT for next is loaded in switch_mm, and the GDT is loaded
	 * above.
	 *
	 * We therefore need to write new values to the segment
	 * registers on every context switch unless both the new and old
	 * values are zero.
	 *
	 * Note that we don't need to do anything for CS and SS, as
	 * those are saved and restored as part of pt_regs.
	 */
	savesegment(es, prev->es);
	if (unlikely(next->es | prev->es))
		loadsegment(es, next->es);

	savesegment(ds, prev->ds);
	if (unlikely(next->ds | prev->ds))
		loadsegment(ds, next->ds);

<<<<<<< HEAD
	/*
	 * Switch FS and GS.
	 *
	 * These are even more complicated than FS and GS: they have
	 * 64-bit bases are that controlled by arch_prctl.  Those bases
	 * only differ from the values in the GDT or LDT if the selector
	 * is 0.
	 *
	 * Loading the segment register resets the hidden base part of
	 * the register to 0 or the value from the GDT / LDT.  If the
	 * next base address zero, writing 0 to the segment register is
	 * much faster than using wrmsr to explicitly zero the base.
	 *
	 * The thread_struct.fs and thread_struct.gs values are 0
	 * if the fs and gs bases respectively are not overridden
	 * from the values implied by fsindex and gsindex.  They
	 * are nonzero, and store the nonzero base addresses, if
	 * the bases are overridden.
	 *
	 * (fs != 0 && fsindex != 0) || (gs != 0 && gsindex != 0) should
	 * be impossible.
	 *
	 * Therefore we need to reload the segment registers if either
	 * the old or new selector is nonzero, and we need to override
	 * the base address if next thread expects it to be overridden.
	 *
	 * This code is unnecessarily slow in the case where the old and
	 * new indexes are zero and the new base is nonzero -- it will
	 * unnecessarily write 0 to the selector before writing the new
	 * base address.
	 *
	 * Note: This all depends on arch_prctl being the only way that
	 * user code can override the segment base.  Once wrfsbase and
	 * wrgsbase are enabled, most of this code will need to change.
	 */
	if (unlikely(fsindex | next->fsindex | prev->fs)) {
		loadsegment(fs, next->fsindex);

		/*
		 * If user code wrote a nonzero value to FS, then it also
		 * cleared the overridden base address.
		 *
		 * XXX: if user code wrote 0 to FS and cleared the base
		 * address itself, we won't notice and we'll incorrectly
		 * restore the prior base address next time we reschdule
		 * the process.
		 */
		if (fsindex)
			prev->fs = 0;
	}
	if (next->fs)
		wrmsrl(MSR_FS_BASE, next->fs);
	prev->fsindex = fsindex;

	if (unlikely(gsindex | next->gsindex | prev->gs)) {
		load_gs_index(next->gsindex);

		/* This works (and fails) the same way as fsindex above. */
		if (gsindex)
			prev->gs = 0;
	}
	if (next->gs)
		wrmsrl(MSR_KERNEL_GS_BASE, next->gs);
	prev->gsindex = gsindex;

	switch_fpu_finish(next_p, fpu);
=======
	x86_fsgsbase_load(prev, next);

	x86_pkru_load(prev, next);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	/*
	 * Switch the PDA and FPU contexts.
	 */
<<<<<<< HEAD
	prev->usersp = percpu_read(old_rsp);
	percpu_write(old_rsp, next->usersp);
	percpu_write(current_task, next_p);

	percpu_write(kernel_stack,
		  (unsigned long)task_stack_page(next_p) +
		  THREAD_SIZE - KERNEL_STACK_OFFSET);

	/*
	 * Now maybe reload the debug registers and handle I/O bitmaps
	 */
	if (unlikely(task_thread_info(next_p)->flags & _TIF_WORK_CTXSW_NEXT ||
		     task_thread_info(prev_p)->flags & _TIF_WORK_CTXSW_PREV))
		__switch_to_xtra(prev_p, next_p, tss);

#ifdef CONFIG_XEN
	/*
	 * On Xen PV, IOPL bits in pt_regs->flags have no effect, and
	 * current_pt_regs()->flags may not match the current task's
	 * intended IOPL.  We need to switch it manually.
	 */
	if (unlikely(xen_pv_domain() &&
		     prev->iopl != next->iopl))
		xen_set_iopl_mask(next->iopl);
#endif
=======
	raw_cpu_write(pcpu_hot.current_task, next_p);
	raw_cpu_write(pcpu_hot.top_of_stack, task_top_of_stack(next_p));

	switch_fpu_finish(next_p);

	/* Reload sp0. */
	update_task_stack(next_p);

	switch_to_extra(prev_p, next_p);

	if (static_cpu_has_bug(X86_BUG_SYSRET_SS_ATTRS)) {
		/*
		 * AMD CPUs have a misfeature: SYSRET sets the SS selector but
		 * does not update the cached descriptor.  As a result, if we
		 * do SYSRET while SS is NULL, we'll end up in user mode with
		 * SS apparently equal to __USER_DS but actually unusable.
		 *
		 * The straightforward workaround would be to fix it up just
		 * before SYSRET, but that would slow down the system call
		 * fast paths.  Instead, we ensure that SS is never NULL in
		 * system call context.  We do this by replacing NULL SS
		 * selectors at every context switch.  SYSCALL sets up a valid
		 * SS, so the only way to get NULL is to re-enter the kernel
		 * from CPL 3 through an interrupt.  Since that can't happen
		 * in the same task as a running syscall, we are guaranteed to
		 * context switch between every interrupt vector entry and a
		 * subsequent SYSRET.
		 *
		 * We read SS first because SS reads are much faster than
		 * writes.  Out of caution, we force SS to __KERNEL_DS even if
		 * it previously had a different non-NULL value.
		 */
		unsigned short ss_sel;
		savesegment(ss, ss_sel);
		if (ss_sel != __KERNEL_DS)
			loadsegment(ss, __KERNEL_DS);
	}

	/* Load the Intel cache allocation PQR MSR. */
	resctrl_sched_in(next_p);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	return prev_p;
}

void set_personality_64bit(void)
{
	/* inherit personality from parent */

	/* Make sure to be in 64bit mode */
<<<<<<< HEAD
	clear_thread_flag(TIF_IA32);
	clear_thread_flag(TIF_ADDR32);
	clear_thread_flag(TIF_X32);

	/* Ensure the corresponding mm is not marked. */
	if (current->mm)
		current->mm->context.ia32_compat = 0;
=======
	clear_thread_flag(TIF_ADDR32);
	/* Pretend that this comes from a 64bit execve */
	task_pt_regs(current)->orig_ax = __NR_execve;
	current_thread_info()->status &= ~TS_COMPAT;
	if (current->mm)
		__set_bit(MM_CONTEXT_HAS_VSYSCALL, &current->mm->context.flags);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	/* TBD: overwrites user setup. Should have two bits.
	   But 64bit processes have always behaved this way,
	   so it's not too bad. The main problem is just that
<<<<<<< HEAD
	   32bit childs are affected again. */
	current->personality &= ~READ_IMPLIES_EXEC;
}

void set_personality_ia32(bool x32)
{
	/* inherit personality from parent */

	/* Make sure to be in 32bit mode */
	set_thread_flag(TIF_ADDR32);

	/* Mark the associated mm as containing 32-bit tasks. */
	if (current->mm)
		current->mm->context.ia32_compat = 1;

	if (x32) {
		clear_thread_flag(TIF_IA32);
		set_thread_flag(TIF_X32);
		current->personality &= ~READ_IMPLIES_EXEC;
		/* is_compat_task() uses the presence of the x32
		   syscall bit flag to determine compat status */
		current_thread_info()->status &= ~TS_COMPAT;
	} else {
		set_thread_flag(TIF_IA32);
		clear_thread_flag(TIF_X32);
		current->personality |= force_personality32;
		/* Prepare the first "return" to user space */
		current_thread_info()->status |= TS_COMPAT;
	}
}
EXPORT_SYMBOL_GPL(set_personality_ia32);

/*
 * Called from fs/proc with a reference on @p to find the function
 * which called into schedule(). This needs to be done carefully
 * because the task might wake up and we might look at a stack
 * changing under us.
 */
unsigned long get_wchan(struct task_struct *p)
{
	unsigned long start, bottom, top, sp, fp, ip;
	int count = 0;

	if (!p || p == current || p->state == TASK_RUNNING)
		return 0;

	start = (unsigned long)task_stack_page(p);
	if (!start)
		return 0;

	/*
	 * Layout of the stack page:
	 *
	 * ----------- topmax = start + THREAD_SIZE - sizeof(unsigned long)
	 * PADDING
	 * ----------- top = topmax - TOP_OF_KERNEL_STACK_PADDING
	 * stack
	 * ----------- bottom = start + sizeof(thread_info)
	 * thread_info
	 * ----------- start
	 *
	 * The tasks stack pointer points at the location where the
	 * framepointer is stored. The data on the stack is:
	 * ... IP FP ... IP FP
	 *
	 * We need to read FP and IP, so we need to adjust the upper
	 * bound by another unsigned long.
	 */
	top = start + THREAD_SIZE;
	top -= 2 * sizeof(unsigned long);
	bottom = start + sizeof(struct thread_info);

	sp = ACCESS_ONCE(p->thread.sp);
	if (sp < bottom || sp > top)
		return 0;

	fp = ACCESS_ONCE(*(unsigned long *)sp);
	do {
		if (fp < bottom || fp > top)
			return 0;
		ip = ACCESS_ONCE(*(unsigned long *)(fp + sizeof(unsigned long)));
		if (!in_sched_functions(ip))
			return ip;
		fp = ACCESS_ONCE(*(unsigned long *)fp);
	} while (count++ < 16 && p->state != TASK_RUNNING);
	return 0;
}

long do_arch_prctl(struct task_struct *task, int code, unsigned long addr)
{
	int ret = 0;
	int doit = task == current;
	int cpu;

	switch (code) {
	case ARCH_SET_GS:
		if (addr >= TASK_SIZE_OF(task))
			return -EPERM;
		cpu = get_cpu();
		/* handle small bases via the GDT because that's faster to
		   switch. */
		if (addr <= 0xffffffff) {
			set_32bit_tls(task, GS_TLS, addr);
			if (doit) {
				load_TLS(&task->thread, cpu);
				load_gs_index(GS_TLS_SEL);
			}
			task->thread.gsindex = GS_TLS_SEL;
			task->thread.gs = 0;
		} else {
			task->thread.gsindex = 0;
			task->thread.gs = addr;
			if (doit) {
				load_gs_index(0);
				ret = checking_wrmsrl(MSR_KERNEL_GS_BASE, addr);
			}
		}
		put_cpu();
		break;
	case ARCH_SET_FS:
		/* Not strictly needed for fs, but do it for symmetry
		   with gs */
		if (addr >= TASK_SIZE_OF(task))
			return -EPERM;
		cpu = get_cpu();
		/* handle small bases via the GDT because that's faster to
		   switch. */
		if (addr <= 0xffffffff) {
			set_32bit_tls(task, FS_TLS, addr);
			if (doit) {
				load_TLS(&task->thread, cpu);
				loadsegment(fs, FS_TLS_SEL);
			}
			task->thread.fsindex = FS_TLS_SEL;
			task->thread.fs = 0;
		} else {
			task->thread.fsindex = 0;
			task->thread.fs = addr;
			if (doit) {
				/* set the selector to 0 to not confuse
				   __switch_to */
				loadsegment(fs, 0);
				ret = checking_wrmsrl(MSR_FS_BASE, addr);
			}
		}
		put_cpu();
		break;
	case ARCH_GET_FS: {
		unsigned long base;
		if (task->thread.fsindex == FS_TLS_SEL)
			base = read_32bit_tls(task, FS_TLS);
		else if (doit)
			rdmsrl(MSR_FS_BASE, base);
		else
			base = task->thread.fs;
		ret = put_user(base, (unsigned long __user *)addr);
		break;
	}
	case ARCH_GET_GS: {
		unsigned long base;
		unsigned gsindex;
		if (task->thread.gsindex == GS_TLS_SEL)
			base = read_32bit_tls(task, GS_TLS);
		else if (doit) {
			savesegment(gs, gsindex);
			if (gsindex)
				rdmsrl(MSR_KERNEL_GS_BASE, base);
			else
				base = task->thread.gs;
		} else
			base = task->thread.gs;
		ret = put_user(base, (unsigned long __user *)addr);
		break;
	}

=======
	   32bit children are affected again. */
	current->personality &= ~READ_IMPLIES_EXEC;
}

static void __set_personality_x32(void)
{
#ifdef CONFIG_X86_X32_ABI
	if (current->mm)
		current->mm->context.flags = 0;

	current->personality &= ~READ_IMPLIES_EXEC;
	/*
	 * in_32bit_syscall() uses the presence of the x32 syscall bit
	 * flag to determine compat status.  The x86 mmap() code relies on
	 * the syscall bitness so set x32 syscall bit right here to make
	 * in_32bit_syscall() work during exec().
	 *
	 * Pretend to come from a x32 execve.
	 */
	task_pt_regs(current)->orig_ax = __NR_x32_execve | __X32_SYSCALL_BIT;
	current_thread_info()->status &= ~TS_COMPAT;
#endif
}

static void __set_personality_ia32(void)
{
#ifdef CONFIG_IA32_EMULATION
	if (current->mm) {
		/*
		 * uprobes applied to this MM need to know this and
		 * cannot use user_64bit_mode() at that time.
		 */
		__set_bit(MM_CONTEXT_UPROBE_IA32, &current->mm->context.flags);
	}

	current->personality |= force_personality32;
	/* Prepare the first "return" to user space */
	task_pt_regs(current)->orig_ax = __NR_ia32_execve;
	current_thread_info()->status |= TS_COMPAT;
#endif
}

void set_personality_ia32(bool x32)
{
	/* Make sure to be in 32bit mode */
	set_thread_flag(TIF_ADDR32);

	if (x32)
		__set_personality_x32();
	else
		__set_personality_ia32();
}
EXPORT_SYMBOL_GPL(set_personality_ia32);

#ifdef CONFIG_CHECKPOINT_RESTORE
static long prctl_map_vdso(const struct vdso_image *image, unsigned long addr)
{
	int ret;

	ret = map_vdso_once(image, addr);
	if (ret)
		return ret;

	return (long)image->size;
}
#endif

#ifdef CONFIG_ADDRESS_MASKING

#define LAM_U57_BITS 6

static int prctl_enable_tagged_addr(struct mm_struct *mm, unsigned long nr_bits)
{
	if (!cpu_feature_enabled(X86_FEATURE_LAM))
		return -ENODEV;

	/* PTRACE_ARCH_PRCTL */
	if (current->mm != mm)
		return -EINVAL;

	if (mm_valid_pasid(mm) &&
	    !test_bit(MM_CONTEXT_FORCE_TAGGED_SVA, &mm->context.flags))
		return -EINVAL;

	if (mmap_write_lock_killable(mm))
		return -EINTR;

	if (test_bit(MM_CONTEXT_LOCK_LAM, &mm->context.flags)) {
		mmap_write_unlock(mm);
		return -EBUSY;
	}

	if (!nr_bits) {
		mmap_write_unlock(mm);
		return -EINVAL;
	} else if (nr_bits <= LAM_U57_BITS) {
		mm->context.lam_cr3_mask = X86_CR3_LAM_U57;
		mm->context.untag_mask =  ~GENMASK(62, 57);
	} else {
		mmap_write_unlock(mm);
		return -EINVAL;
	}

	write_cr3(__read_cr3() | mm->context.lam_cr3_mask);
	set_tlbstate_lam_mode(mm);
	set_bit(MM_CONTEXT_LOCK_LAM, &mm->context.flags);

	mmap_write_unlock(mm);

	return 0;
}
#endif

long do_arch_prctl_64(struct task_struct *task, int option, unsigned long arg2)
{
	int ret = 0;

	switch (option) {
	case ARCH_SET_GS: {
		if (unlikely(arg2 >= TASK_SIZE_MAX))
			return -EPERM;

		preempt_disable();
		/*
		 * ARCH_SET_GS has always overwritten the index
		 * and the base. Zero is the most sensible value
		 * to put in the index, and is the only value that
		 * makes any sense if FSGSBASE is unavailable.
		 */
		if (task == current) {
			loadseg(GS, 0);
			x86_gsbase_write_cpu_inactive(arg2);

			/*
			 * On non-FSGSBASE systems, save_base_legacy() expects
			 * that we also fill in thread.gsbase.
			 */
			task->thread.gsbase = arg2;

		} else {
			task->thread.gsindex = 0;
			x86_gsbase_write_task(task, arg2);
		}
		preempt_enable();
		break;
	}
	case ARCH_SET_FS: {
		/*
		 * Not strictly needed for %fs, but do it for symmetry
		 * with %gs
		 */
		if (unlikely(arg2 >= TASK_SIZE_MAX))
			return -EPERM;

		preempt_disable();
		/*
		 * Set the selector to 0 for the same reason
		 * as %gs above.
		 */
		if (task == current) {
			loadseg(FS, 0);
			x86_fsbase_write_cpu(arg2);

			/*
			 * On non-FSGSBASE systems, save_base_legacy() expects
			 * that we also fill in thread.fsbase.
			 */
			task->thread.fsbase = arg2;
		} else {
			task->thread.fsindex = 0;
			x86_fsbase_write_task(task, arg2);
		}
		preempt_enable();
		break;
	}
	case ARCH_GET_FS: {
		unsigned long base = x86_fsbase_read_task(task);

		ret = put_user(base, (unsigned long __user *)arg2);
		break;
	}
	case ARCH_GET_GS: {
		unsigned long base = x86_gsbase_read_task(task);

		ret = put_user(base, (unsigned long __user *)arg2);
		break;
	}

#ifdef CONFIG_CHECKPOINT_RESTORE
# ifdef CONFIG_X86_X32_ABI
	case ARCH_MAP_VDSO_X32:
		return prctl_map_vdso(&vdso_image_x32, arg2);
# endif
# if defined CONFIG_X86_32 || defined CONFIG_IA32_EMULATION
	case ARCH_MAP_VDSO_32:
		return prctl_map_vdso(&vdso_image_32, arg2);
# endif
	case ARCH_MAP_VDSO_64:
		return prctl_map_vdso(&vdso_image_64, arg2);
#endif
#ifdef CONFIG_ADDRESS_MASKING
	case ARCH_GET_UNTAG_MASK:
		return put_user(task->mm->context.untag_mask,
				(unsigned long __user *)arg2);
	case ARCH_ENABLE_TAGGED_ADDR:
		return prctl_enable_tagged_addr(task->mm, arg2);
	case ARCH_FORCE_TAGGED_SVA:
		if (current != task)
			return -EINVAL;
		set_bit(MM_CONTEXT_FORCE_TAGGED_SVA, &task->mm->context.flags);
		return 0;
	case ARCH_GET_MAX_TAG_BITS:
		if (!cpu_feature_enabled(X86_FEATURE_LAM))
			return put_user(0, (unsigned long __user *)arg2);
		else
			return put_user(LAM_U57_BITS, (unsigned long __user *)arg2);
#endif
	case ARCH_SHSTK_ENABLE:
	case ARCH_SHSTK_DISABLE:
	case ARCH_SHSTK_LOCK:
	case ARCH_SHSTK_UNLOCK:
	case ARCH_SHSTK_STATUS:
		return shstk_prctl(task, option, arg2);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	default:
		ret = -EINVAL;
		break;
	}

	return ret;
}

<<<<<<< HEAD
long sys_arch_prctl(int code, unsigned long addr)
{
	return do_arch_prctl(current, code, addr);
}

unsigned long KSTK_ESP(struct task_struct *task)
{
	return (test_tsk_thread_flag(task, TIF_IA32)) ?
			(task_pt_regs(task)->sp) : ((task)->thread.usersp);
=======
SYSCALL_DEFINE2(arch_prctl, int, option, unsigned long, arg2)
{
	long ret;

	ret = do_arch_prctl_64(current, option, arg2);
	if (ret == -EINVAL)
		ret = do_arch_prctl_common(option, arg2);

	return ret;
}

#ifdef CONFIG_IA32_EMULATION
COMPAT_SYSCALL_DEFINE2(arch_prctl, int, option, unsigned long, arg2)
{
	return do_arch_prctl_common(option, arg2);
}
#endif

unsigned long KSTK_ESP(struct task_struct *task)
{
	return task_pt_regs(task)->sp;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}
