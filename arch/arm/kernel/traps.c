<<<<<<< HEAD
=======
// SPDX-License-Identifier: GPL-2.0-only
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
/*
 *  linux/arch/arm/kernel/traps.c
 *
 *  Copyright (C) 1995-2009 Russell King
 *  Fragments that appear the same as linux/arch/i386/kernel/traps.c (C) Linus Torvalds
 *
<<<<<<< HEAD
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 *
=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 *  'traps.c' handles hardware exceptions after we have saved some state in
 *  'linux/arch/arm/lib/traps.S'.  Mostly a debugging aid, but will probably
 *  kill the offending process.
 */
#include <linux/signal.h>
#include <linux/personality.h>
#include <linux/kallsyms.h>
#include <linux/spinlock.h>
#include <linux/uaccess.h>
#include <linux/hardirq.h>
#include <linux/kdebug.h>
<<<<<<< HEAD
=======
#include <linux/kprobes.h>
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#include <linux/module.h>
#include <linux/kexec.h>
#include <linux/bug.h>
#include <linux/delay.h>
#include <linux/init.h>
<<<<<<< HEAD
#include <linux/sched.h>
#include <linux/bug.h>
=======
#include <linux/sched/signal.h>
#include <linux/sched/debug.h>
#include <linux/sched/task_stack.h>
#include <linux/irq.h>
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

#include <linux/atomic.h>
#include <asm/cacheflush.h>
#include <asm/exception.h>
<<<<<<< HEAD
#include <asm/unistd.h>
#include <asm/traps.h>
#include <asm/unwind.h>
#include <asm/tls.h>
#include <asm/system_misc.h>

#include <trace/events/exception.h>
=======
#include <asm/spectre.h>
#include <asm/unistd.h>
#include <asm/traps.h>
#include <asm/ptrace.h>
#include <asm/unwind.h>
#include <asm/tls.h>
#include <asm/stacktrace.h>
#include <asm/system_misc.h>
#include <asm/opcodes.h>

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

static const char *handler[]= {
	"prefetch abort",
	"data abort",
	"address exception",
	"interrupt",
	"undefined instruction",
};

void *vectors_page;

#ifdef CONFIG_DEBUG_USER
unsigned int user_debug;

static int __init user_debug_setup(char *str)
{
	get_option(&str, &user_debug);
	return 1;
}
__setup("user_debug=", user_debug_setup);
#endif

<<<<<<< HEAD
static void dump_mem(const char *, const char *, unsigned long, unsigned long);

void dump_backtrace_entry(unsigned long where, unsigned long from, unsigned long frame)
{
#ifdef CONFIG_KALLSYMS
	printk("[<%08lx>] (%pS) from [<%08lx>] (%pS)\n", where, (void *)where, from, (void *)from);
#else
	printk("Function entered at [<%08lx>] from [<%08lx>]\n", where, from);
#endif

	if (in_exception_text(where))
		dump_mem("", "Exception stack", frame + 4, frame + 4 + sizeof(struct pt_regs));
=======
void dump_backtrace_entry(unsigned long where, unsigned long from,
			  unsigned long frame, const char *loglvl)
{
	unsigned long end = frame + 4 + sizeof(struct pt_regs);

	if (IS_ENABLED(CONFIG_UNWINDER_FRAME_POINTER) &&
	    IS_ENABLED(CONFIG_CC_IS_GCC) &&
	    end > ALIGN(frame, THREAD_SIZE)) {
		/*
		 * If we are walking past the end of the stack, it may be due
		 * to the fact that we are on an IRQ or overflow stack. In this
		 * case, we can load the address of the other stack from the
		 * frame record.
		 */
		frame = ((unsigned long *)frame)[-2] - 4;
		end = frame + 4 + sizeof(struct pt_regs);
	}

#ifndef CONFIG_KALLSYMS
	printk("%sFunction entered at [<%08lx>] from [<%08lx>]\n",
		loglvl, where, from);
#elif defined CONFIG_BACKTRACE_VERBOSE
	printk("%s[<%08lx>] (%ps) from [<%08lx>] (%pS)\n",
		loglvl, where, (void *)where, from, (void *)from);
#else
	printk("%s %ps from %pS\n", loglvl, (void *)where, (void *)from);
#endif

	if (in_entry_text(from) && end <= ALIGN(frame, THREAD_SIZE))
		dump_mem(loglvl, "Exception stack", frame + 4, end);
}

void dump_backtrace_stm(u32 *stack, u32 instruction, const char *loglvl)
{
	char str[80], *p;
	unsigned int x;
	int reg;

	for (reg = 10, x = 0, p = str; reg >= 0; reg--) {
		if (instruction & BIT(reg)) {
			p += sprintf(p, " r%d:%08x", reg, *stack--);
			if (++x == 6) {
				x = 0;
				p = str;
				printk("%s%s\n", loglvl, str);
			}
		}
	}
	if (p != str)
		printk("%s%s\n", loglvl, str);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

#ifndef CONFIG_ARM_UNWIND
/*
 * Stack pointers should always be within the kernels view of
 * physical memory.  If it is not there, then we can't dump
 * out any information relating to the stack.
 */
static int verify_stack(unsigned long sp)
{
	if (sp < PAGE_OFFSET ||
<<<<<<< HEAD
	    (sp > (unsigned long)high_memory && high_memory != NULL))
=======
	    (!IS_ENABLED(CONFIG_VMAP_STACK) &&
	     sp > (unsigned long)high_memory && high_memory != NULL))
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		return -EFAULT;

	return 0;
}
#endif

/*
 * Dump out the contents of some memory nicely...
 */
<<<<<<< HEAD
static void dump_mem(const char *lvl, const char *str, unsigned long bottom,
		     unsigned long top)
{
	unsigned long first;
	mm_segment_t fs;
	int i;

	/*
	 * We need to switch to kernel mode so that we can use __get_user
	 * to safely read from kernel space.  Note that we now dump the
	 * code first, just in case the backtrace kills us.
	 */
	fs = get_fs();
	set_fs(KERNEL_DS);

=======
void dump_mem(const char *lvl, const char *str, unsigned long bottom,
	      unsigned long top)
{
	unsigned long first;
	int i;

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	printk("%s%s(0x%08lx to 0x%08lx)\n", lvl, str, bottom, top);

	for (first = bottom & ~31; first < top; first += 32) {
		unsigned long p;
		char str[sizeof(" 12345678") * 8 + 1];

		memset(str, ' ', sizeof(str));
		str[sizeof(str) - 1] = '\0';

		for (p = first, i = 0; i < 8 && p < top; i++, p += 4) {
			if (p >= bottom && p < top) {
				unsigned long val;
<<<<<<< HEAD
				if (__get_user(val, (unsigned long *)p) == 0)
=======
				if (!get_kernel_nofault(val, (unsigned long *)p))
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
					sprintf(str + i * 9, " %08lx", val);
				else
					sprintf(str + i * 9, " ????????");
			}
		}
		printk("%s%04lx:%s\n", lvl, first & 0xffff, str);
	}
<<<<<<< HEAD

	set_fs(fs);
=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

static void dump_instr(const char *lvl, struct pt_regs *regs)
{
	unsigned long addr = instruction_pointer(regs);
	const int thumb = thumb_mode(regs);
	const int width = thumb ? 4 : 8;
<<<<<<< HEAD
	mm_segment_t fs;
=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	char str[sizeof("00000000 ") * 5 + 2 + 1], *p = str;
	int i;

	/*
<<<<<<< HEAD
	 * We need to switch to kernel mode so that we can use __get_user
	 * to safely read from kernel space.  Note that we now dump the
	 * code first, just in case the backtrace kills us.
	 */
	fs = get_fs();
	set_fs(KERNEL_DS);
=======
	 * Note that we now dump the code first, just in case the backtrace
	 * kills us.
	 */
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	for (i = -4; i < 1 + !!thumb; i++) {
		unsigned int val, bad;

<<<<<<< HEAD
		if (thumb)
			bad = __get_user(val, &((u16 *)addr)[i]);
		else
			bad = __get_user(val, &((u32 *)addr)[i]);
=======
		if (thumb) {
			u16 tmp;

			if (user_mode(regs))
				bad = get_user(tmp, &((u16 __user *)addr)[i]);
			else
				bad = get_kernel_nofault(tmp, &((u16 *)addr)[i]);

			val = __mem_to_opcode_thumb16(tmp);
		} else {
			if (user_mode(regs))
				bad = get_user(val, &((u32 __user *)addr)[i]);
			else
				bad = get_kernel_nofault(val, &((u32 *)addr)[i]);

			val = __mem_to_opcode_arm(val);
		}
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

		if (!bad)
			p += sprintf(p, i == 0 ? "(%0*x) " : "%0*x ",
					width, val);
		else {
			p += sprintf(p, "bad PC value");
			break;
		}
	}
	printk("%sCode: %s\n", lvl, str);
<<<<<<< HEAD

	set_fs(fs);
}

#ifdef CONFIG_ARM_UNWIND
static inline void dump_backtrace(struct pt_regs *regs, struct task_struct *tsk)
{
	unwind_backtrace(regs, tsk);
}
#else
static void dump_backtrace(struct pt_regs *regs, struct task_struct *tsk)
=======
}

#ifdef CONFIG_ARM_UNWIND
void dump_backtrace(struct pt_regs *regs, struct task_struct *tsk,
		    const char *loglvl)
{
	unwind_backtrace(regs, tsk, loglvl);
}
#else
void dump_backtrace(struct pt_regs *regs, struct task_struct *tsk,
		    const char *loglvl)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
{
	unsigned int fp, mode;
	int ok = 1;

<<<<<<< HEAD
	printk("Backtrace: ");
=======
	printk("%sCall trace: ", loglvl);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	if (!tsk)
		tsk = current;

	if (regs) {
<<<<<<< HEAD
		fp = regs->ARM_fp;
=======
		fp = frame_pointer(regs);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		mode = processor_mode(regs);
	} else if (tsk != current) {
		fp = thread_saved_fp(tsk);
		mode = 0x10;
	} else {
		asm("mov %0, fp" : "=r" (fp) : : "cc");
		mode = 0x10;
	}

	if (!fp) {
<<<<<<< HEAD
		printk("no frame pointer");
		ok = 0;
	} else if (verify_stack(fp)) {
		printk("invalid frame pointer 0x%08x", fp);
		ok = 0;
	} else if (fp < (unsigned long)end_of_stack(tsk))
		printk("frame pointer underflow");
	printk("\n");

	if (ok)
		c_backtrace(fp, mode);
}
#endif

void dump_stack(void)
{
	dump_backtrace(NULL, NULL);
}

EXPORT_SYMBOL(dump_stack);

void show_stack(struct task_struct *tsk, unsigned long *sp)
{
	dump_backtrace(NULL, tsk);
=======
		pr_cont("no frame pointer");
		ok = 0;
	} else if (verify_stack(fp)) {
		pr_cont("invalid frame pointer 0x%08x", fp);
		ok = 0;
	} else if (fp < (unsigned long)end_of_stack(tsk))
		pr_cont("frame pointer underflow");
	pr_cont("\n");

	if (ok)
		c_backtrace(fp, mode, loglvl);
}
#endif

void show_stack(struct task_struct *tsk, unsigned long *sp, const char *loglvl)
{
	dump_backtrace(NULL, tsk, loglvl);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	barrier();
}

#ifdef CONFIG_PREEMPT
#define S_PREEMPT " PREEMPT"
<<<<<<< HEAD
=======
#elif defined(CONFIG_PREEMPT_RT)
#define S_PREEMPT " PREEMPT_RT"
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#else
#define S_PREEMPT ""
#endif
#ifdef CONFIG_SMP
#define S_SMP " SMP"
#else
#define S_SMP ""
#endif
#ifdef CONFIG_THUMB2_KERNEL
#define S_ISA " THUMB2"
#else
#define S_ISA " ARM"
#endif

<<<<<<< HEAD
static int __die(const char *str, int err, struct thread_info *thread, struct pt_regs *regs)
{
	struct task_struct *tsk = thread->task;
	static int die_counter;
	int ret;

	printk(KERN_EMERG "Internal error: %s: %x [#%d]" S_PREEMPT S_SMP
	       S_ISA "\n", str, err, ++die_counter);
=======
static int __die(const char *str, int err, struct pt_regs *regs)
{
	struct task_struct *tsk = current;
	static int die_counter;
	int ret;

	pr_emerg("Internal error: %s: %x [#%d]" S_PREEMPT S_SMP S_ISA "\n",
	         str, err, ++die_counter);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	/* trap and error numbers are mostly meaningless on ARM */
	ret = notify_die(DIE_OOPS, str, regs, err, tsk->thread.trap_no, SIGSEGV);
	if (ret == NOTIFY_STOP)
<<<<<<< HEAD
		return ret;

	print_modules();
	__show_regs(regs);
	printk(KERN_EMERG "Process %.*s (pid: %d, stack limit = 0x%p)\n",
		TASK_COMM_LEN, tsk->comm, task_pid_nr(tsk), thread + 1);

	if (!user_mode(regs) || in_interrupt()) {
		dump_mem(KERN_EMERG, "Stack: ", regs->ARM_sp,
			 THREAD_SIZE + (unsigned long)task_stack_page(tsk));
		dump_backtrace(regs, tsk);
		dump_instr(KERN_EMERG, regs);
	}

	return ret;
}

static DEFINE_RAW_SPINLOCK(die_lock);

/*
 * This function is protected against re-entrancy.
 */
void die(const char *str, struct pt_regs *regs, int err)
{
	struct thread_info *thread = current_thread_info();
	int ret;
	enum bug_trap_type bug_type = BUG_TRAP_TYPE_NONE;

	oops_enter();

	raw_spin_lock_irq(&die_lock);
	console_verbose();
	bust_spinlocks(1);
	if (!user_mode(regs))
		bug_type = report_bug(regs->ARM_pc, regs);
	if (bug_type != BUG_TRAP_TYPE_NONE)
		str = "Oops - BUG";
	ret = __die(str, err, thread, regs);

	if (regs && kexec_should_crash(thread->task))
		crash_kexec(regs);

	bust_spinlocks(0);
	add_taint(TAINT_DIE);
	raw_spin_unlock_irq(&die_lock);
=======
		return 1;

	print_modules();
	__show_regs(regs);
	__show_regs_alloc_free(regs);
	pr_emerg("Process %.*s (pid: %d, stack limit = 0x%p)\n",
		 TASK_COMM_LEN, tsk->comm, task_pid_nr(tsk), end_of_stack(tsk));

	if (!user_mode(regs) || in_interrupt()) {
		dump_mem(KERN_EMERG, "Stack: ", regs->ARM_sp,
			 ALIGN(regs->ARM_sp - THREAD_SIZE, THREAD_ALIGN)
			 + THREAD_SIZE);
		dump_backtrace(regs, tsk, KERN_EMERG);
		dump_instr(KERN_EMERG, regs);
	}

	return 0;
}

static arch_spinlock_t die_lock = __ARCH_SPIN_LOCK_UNLOCKED;
static int die_owner = -1;
static unsigned int die_nest_count;

static unsigned long oops_begin(void)
{
	int cpu;
	unsigned long flags;

	oops_enter();

	/* racy, but better than risking deadlock. */
	raw_local_irq_save(flags);
	cpu = smp_processor_id();
	if (!arch_spin_trylock(&die_lock)) {
		if (cpu == die_owner)
			/* nested oops. should stop eventually */;
		else
			arch_spin_lock(&die_lock);
	}
	die_nest_count++;
	die_owner = cpu;
	console_verbose();
	bust_spinlocks(1);
	return flags;
}

static void oops_end(unsigned long flags, struct pt_regs *regs, int signr)
{
	if (regs && kexec_should_crash(current))
		crash_kexec(regs);

	bust_spinlocks(0);
	die_owner = -1;
	add_taint(TAINT_DIE, LOCKDEP_NOW_UNRELIABLE);
	die_nest_count--;
	if (!die_nest_count)
		/* Nest count reaches zero, release the lock. */
		arch_spin_unlock(&die_lock);
	raw_local_irq_restore(flags);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	oops_exit();

	if (in_interrupt())
		panic("Fatal exception in interrupt");
	if (panic_on_oops)
		panic("Fatal exception");
<<<<<<< HEAD
	if (ret != NOTIFY_STOP)
		do_exit(SIGSEGV);
}

void arm_notify_die(const char *str, struct pt_regs *regs,
		struct siginfo *info, unsigned long err, unsigned long trap)
=======
	if (signr)
		make_task_dead(signr);
}

/*
 * This function is protected against re-entrancy.
 */
void die(const char *str, struct pt_regs *regs, int err)
{
	enum bug_trap_type bug_type = BUG_TRAP_TYPE_NONE;
	unsigned long flags = oops_begin();
	int sig = SIGSEGV;

	if (!user_mode(regs))
		bug_type = report_bug(regs->ARM_pc, regs);
	if (bug_type != BUG_TRAP_TYPE_NONE)
		str = "Oops - BUG";

	if (__die(str, err, regs))
		sig = 0;

	oops_end(flags, regs, sig);
}

void arm_notify_die(const char *str, struct pt_regs *regs,
		int signo, int si_code, void __user *addr,
		unsigned long err, unsigned long trap)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
{
	if (user_mode(regs)) {
		current->thread.error_code = err;
		current->thread.trap_no = trap;

<<<<<<< HEAD
		force_sig_info(info->si_signo, info, current);
=======
		force_sig_fault(signo, si_code, addr);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	} else {
		die(str, regs, err);
	}
}

#ifdef CONFIG_GENERIC_BUG

int is_valid_bugaddr(unsigned long pc)
{
#ifdef CONFIG_THUMB2_KERNEL
<<<<<<< HEAD
	unsigned short bkpt;
#else
	unsigned long bkpt;
#endif

	if (probe_kernel_address((unsigned *)pc, bkpt))
		return 0;

	return bkpt == BUG_INSTR_VALUE;
=======
	u16 bkpt;
	u16 insn = __opcode_to_mem_thumb16(BUG_INSTR_VALUE);
#else
	u32 bkpt;
	u32 insn = __opcode_to_mem_arm(BUG_INSTR_VALUE);
#endif

	if (get_kernel_nofault(bkpt, (void *)pc))
		return 0;

	return bkpt == insn;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

#endif

static LIST_HEAD(undef_hook);
static DEFINE_RAW_SPINLOCK(undef_lock);

void register_undef_hook(struct undef_hook *hook)
{
	unsigned long flags;

	raw_spin_lock_irqsave(&undef_lock, flags);
	list_add(&hook->node, &undef_hook);
	raw_spin_unlock_irqrestore(&undef_lock, flags);
}

void unregister_undef_hook(struct undef_hook *hook)
{
	unsigned long flags;

	raw_spin_lock_irqsave(&undef_lock, flags);
	list_del(&hook->node);
	raw_spin_unlock_irqrestore(&undef_lock, flags);
}

<<<<<<< HEAD
static int call_undef_hook(struct pt_regs *regs, unsigned int instr)
=======
static nokprobe_inline
int call_undef_hook(struct pt_regs *regs, unsigned int instr)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
{
	struct undef_hook *hook;
	unsigned long flags;
	int (*fn)(struct pt_regs *regs, unsigned int instr) = NULL;

	raw_spin_lock_irqsave(&undef_lock, flags);
	list_for_each_entry(hook, &undef_hook, node)
		if ((instr & hook->instr_mask) == hook->instr_val &&
		    (regs->ARM_cpsr & hook->cpsr_mask) == hook->cpsr_val)
			fn = hook->fn;
	raw_spin_unlock_irqrestore(&undef_lock, flags);

	return fn ? fn(regs, instr) : 1;
}

<<<<<<< HEAD
asmlinkage void __exception do_undefinstr(struct pt_regs *regs)
{
	unsigned int instr;
	siginfo_t info;
=======
asmlinkage void do_undefinstr(struct pt_regs *regs)
{
	unsigned int instr;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	void __user *pc;

	pc = (void __user *)instruction_pointer(regs);

	if (processor_mode(regs) == SVC_MODE) {
#ifdef CONFIG_THUMB2_KERNEL
		if (thumb_mode(regs)) {
<<<<<<< HEAD
			instr = ((u16 *)pc)[0];
			if (is_wide_instruction(instr)) {
				instr <<= 16;
				instr |= ((u16 *)pc)[1];
			}
		} else
#endif
			instr = *(u32 *) pc;
	} else if (thumb_mode(regs)) {
		if (get_user(instr, (u16 __user *)pc))
			goto die_sig;
=======
			instr = __mem_to_opcode_thumb16(((u16 *)pc)[0]);
			if (is_wide_instruction(instr)) {
				u16 inst2;
				inst2 = __mem_to_opcode_thumb16(((u16 *)pc)[1]);
				instr = __opcode_thumb32_compose(instr, inst2);
			}
		} else
#endif
			instr = __mem_to_opcode_arm(*(u32 *) pc);
	} else if (thumb_mode(regs)) {
		if (get_user(instr, (u16 __user *)pc))
			goto die_sig;
		instr = __mem_to_opcode_thumb16(instr);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		if (is_wide_instruction(instr)) {
			unsigned int instr2;
			if (get_user(instr2, (u16 __user *)pc+1))
				goto die_sig;
<<<<<<< HEAD
			instr <<= 16;
			instr |= instr2;
		}
	} else if (get_user(instr, (u32 __user *)pc)) {
		goto die_sig;
=======
			instr2 = __mem_to_opcode_thumb16(instr2);
			instr = __opcode_thumb32_compose(instr, instr2);
		}
	} else {
		if (get_user(instr, (u32 __user *)pc))
			goto die_sig;
		instr = __mem_to_opcode_arm(instr);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	}

	if (call_undef_hook(regs, instr) == 0)
		return;

<<<<<<< HEAD
	trace_undef_instr(regs, (void *)pc);

die_sig:
#ifdef CONFIG_DEBUG_USER
	if (user_debug & UDBG_UNDEFINED) {
		printk(KERN_INFO "%s (%d): undefined instruction: pc=%p\n",
			current->comm, task_pid_nr(current), pc);
		dump_instr(KERN_INFO, regs);
	}
#endif

	info.si_signo = SIGILL;
	info.si_errno = 0;
	info.si_code  = ILL_ILLOPC;
	info.si_addr  = pc;

	arm_notify_die("Oops - undefined instruction", regs, &info, 0, 6);
}

asmlinkage void do_unexp_fiq (struct pt_regs *regs)
{
	printk("Hmm.  Unexpected FIQ received, but trying to continue\n");
	printk("You may have a hardware problem...\n");
=======
die_sig:
#ifdef CONFIG_DEBUG_USER
	if (user_debug & UDBG_UNDEFINED) {
		pr_info("%s (%d): undefined instruction: pc=%px\n",
			current->comm, task_pid_nr(current), pc);
		__show_regs(regs);
		dump_instr(KERN_INFO, regs);
	}
#endif
	arm_notify_die("Oops - undefined instruction", regs,
		       SIGILL, ILL_ILLOPC, pc, 0, 6);
}
NOKPROBE_SYMBOL(do_undefinstr)

/*
 * Handle FIQ similarly to NMI on x86 systems.
 *
 * The runtime environment for NMIs is extremely restrictive
 * (NMIs can pre-empt critical sections meaning almost all locking is
 * forbidden) meaning this default FIQ handling must only be used in
 * circumstances where non-maskability improves robustness, such as
 * watchdog or debug logic.
 *
 * This handler is not appropriate for general purpose use in drivers
 * platform code and can be overrideen using set_fiq_handler.
 */
asmlinkage void __exception_irq_entry handle_fiq_as_nmi(struct pt_regs *regs)
{
	struct pt_regs *old_regs = set_irq_regs(regs);

	nmi_enter();

	/* nop. FIQ handlers for special arch/arm features can be added here. */

	nmi_exit();

	set_irq_regs(old_regs);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

/*
 * bad_mode handles the impossible case in the vectors.  If you see one of
 * these, then it's extremely serious, and could mean you have buggy hardware.
 * It never returns, and never tries to sync.  We hope that we can at least
 * dump out some state information...
 */
asmlinkage void bad_mode(struct pt_regs *regs, int reason)
{
	console_verbose();

<<<<<<< HEAD
	printk(KERN_CRIT "Bad mode in %s handler detected\n", handler[reason]);
=======
	pr_crit("Bad mode in %s handler detected\n", handler[reason]);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	die("Oops - bad mode", regs, 0);
	local_irq_disable();
	panic("bad mode");
}

static int bad_syscall(int n, struct pt_regs *regs)
{
<<<<<<< HEAD
	struct thread_info *thread = current_thread_info();
	siginfo_t info;

	if ((current->personality & PER_MASK) != PER_LINUX &&
	    thread->exec_domain->handler) {
		thread->exec_domain->handler(n, regs);
=======
	if ((current->personality & PER_MASK) != PER_LINUX) {
		send_sig(SIGSEGV, current, 1);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		return regs->ARM_r0;
	}

#ifdef CONFIG_DEBUG_USER
	if (user_debug & UDBG_SYSCALL) {
<<<<<<< HEAD
		printk(KERN_ERR "[%d] %s: obsolete system call %08x.\n",
=======
		pr_err("[%d] %s: obsolete system call %08x.\n",
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
			task_pid_nr(current), current->comm, n);
		dump_instr(KERN_ERR, regs);
	}
#endif

<<<<<<< HEAD
	info.si_signo = SIGILL;
	info.si_errno = 0;
	info.si_code  = ILL_ILLTRP;
	info.si_addr  = (void __user *)instruction_pointer(regs) -
			 (thumb_mode(regs) ? 2 : 4);

	arm_notify_die("Oops - bad syscall", regs, &info, n, 0);
=======
	arm_notify_die("Oops - bad syscall", regs, SIGILL, ILL_ILLTRP,
		       (void __user *)instruction_pointer(regs) -
			 (thumb_mode(regs) ? 2 : 4),
		       n, 0);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	return regs->ARM_r0;
}

<<<<<<< HEAD
static inline void
do_cache_op(unsigned long start, unsigned long end, int flags)
{
	struct mm_struct *mm = current->active_mm;
	struct vm_area_struct *vma;

	if (end < start || flags)
		return;

	down_read(&mm->mmap_sem);
	vma = find_vma(mm, start);
	if (vma && vma->vm_start < end) {
		if (start < vma->vm_start)
			start = vma->vm_start;
		if (end > vma->vm_end)
			end = vma->vm_end;

		up_read(&mm->mmap_sem);
		flush_cache_user_range(start, end);
		return;
	}
	up_read(&mm->mmap_sem);
=======
static inline int
__do_cache_op(unsigned long start, unsigned long end)
{
	int ret;

	do {
		unsigned long chunk = min(PAGE_SIZE, end - start);

		if (fatal_signal_pending(current))
			return 0;

		ret = flush_icache_user_range(start, start + chunk);
		if (ret)
			return ret;

		cond_resched();
		start += chunk;
	} while (start < end);

	return 0;
}

static inline int
do_cache_op(unsigned long start, unsigned long end, int flags)
{
	if (end < start || flags)
		return -EINVAL;

	if (!access_ok((void __user *)start, end - start))
		return -EFAULT;

	return __do_cache_op(start, end);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

/*
 * Handle all unrecognised system calls.
 *  0x9f0000 - 0x9fffff are some more esoteric system calls
 */
#define NR(x) ((__ARM_NR_##x) - __ARM_NR_BASE)
asmlinkage int arm_syscall(int no, struct pt_regs *regs)
{
<<<<<<< HEAD
	struct thread_info *thread = current_thread_info();
	siginfo_t info;

	/* Emulate/fallthrough. */
	if (no == -1)
		return regs->ARM_r0;

=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	if ((no >> 16) != (__ARM_NR_BASE>> 16))
		return bad_syscall(no, regs);

	switch (no & 0xffff) {
	case 0: /* branch through 0 */
<<<<<<< HEAD
		info.si_signo = SIGSEGV;
		info.si_errno = 0;
		info.si_code  = SEGV_MAPERR;
		info.si_addr  = NULL;

		arm_notify_die("branch through zero", regs, &info, 0, 0);
=======
		arm_notify_die("branch through zero", regs,
			       SIGSEGV, SEGV_MAPERR, NULL, 0, 0);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		return 0;

	case NR(breakpoint): /* SWI BREAK_POINT */
		regs->ARM_pc -= thumb_mode(regs) ? 2 : 4;
<<<<<<< HEAD
		ptrace_break(current, regs);
=======
		ptrace_break(regs);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		return regs->ARM_r0;

	/*
	 * Flush a region from virtual address 'r0' to virtual address 'r1'
	 * _exclusive_.  There is no alignment requirement on either address;
	 * user space does not need to know the hardware cache layout.
	 *
	 * r2 contains flags.  It should ALWAYS be passed as ZERO until it
	 * is defined to be something else.  For now we ignore it, but may
	 * the fires of hell burn in your belly if you break this rule. ;)
	 *
	 * (at a later date, we may want to allow this call to not flush
	 * various aspects of the cache.  Passing '0' will guarantee that
	 * everything necessary gets flushed to maintain consistency in
	 * the specified region).
	 */
	case NR(cacheflush):
<<<<<<< HEAD
		do_cache_op(regs->ARM_r0, regs->ARM_r1, regs->ARM_r2);
		return 0;
=======
		return do_cache_op(regs->ARM_r0, regs->ARM_r1, regs->ARM_r2);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	case NR(usr26):
		if (!(elf_hwcap & HWCAP_26BIT))
			break;
		regs->ARM_cpsr &= ~MODE32_BIT;
		return regs->ARM_r0;

	case NR(usr32):
		if (!(elf_hwcap & HWCAP_26BIT))
			break;
		regs->ARM_cpsr |= MODE32_BIT;
		return regs->ARM_r0;

	case NR(set_tls):
<<<<<<< HEAD
		thread->tp_value[0] = regs->ARM_r0;
		if (tls_emu)
			return 0;
		if (has_tls_reg) {
			asm ("mcr p15, 0, %0, c13, c0, 3"
				: : "r" (regs->ARM_r0));
		} else {
			/*
			 * User space must never try to access this directly.
			 * Expect your app to break eventually if you do so.
			 * The user helper at 0xffff0fe0 must be used instead.
			 * (see entry-armv.S for details)
			 */
			*((unsigned int *)0xffff0ff0) = regs->ARM_r0;
		}
		return 0;

#ifdef CONFIG_NEEDS_SYSCALL_FOR_CMPXCHG
	/*
	 * Atomically store r1 in *r2 if *r2 is equal to r0 for user space.
	 * Return zero in r0 if *MEM was changed or non-zero if no exchange
	 * happened.  Also set the user C flag accordingly.
	 * If access permissions have to be fixed up then non-zero is
	 * returned and the operation has to be re-attempted.
	 *
	 * *NOTE*: This is a ghost syscall private to the kernel.  Only the
	 * __kuser_cmpxchg code in entry-armv.S should be aware of its
	 * existence.  Don't ever use this from user code.
	 */
	case NR(cmpxchg):
	for (;;) {
		extern void do_DataAbort(unsigned long addr, unsigned int fsr,
					 struct pt_regs *regs);
		unsigned long val;
		unsigned long addr = regs->ARM_r2;
		struct mm_struct *mm = current->mm;
		pgd_t *pgd; pmd_t *pmd; pte_t *pte;
		spinlock_t *ptl;

		regs->ARM_cpsr &= ~PSR_C_BIT;
		down_read(&mm->mmap_sem);
		pgd = pgd_offset(mm, addr);
		if (!pgd_present(*pgd))
			goto bad_access;
		pmd = pmd_offset(pgd, addr);
		if (!pmd_present(*pmd))
			goto bad_access;
		pte = pte_offset_map_lock(mm, pmd, addr, &ptl);
		if (!pte_present(*pte) || !pte_write(*pte) || !pte_dirty(*pte)) {
			pte_unmap_unlock(pte, ptl);
			goto bad_access;
		}
		val = *(unsigned long *)addr;
		val -= regs->ARM_r0;
		if (val == 0) {
			*(unsigned long *)addr = regs->ARM_r1;
			regs->ARM_cpsr |= PSR_C_BIT;
		}
		pte_unmap_unlock(pte, ptl);
		up_read(&mm->mmap_sem);
		return val;

		bad_access:
		up_read(&mm->mmap_sem);
		/* simulate a write access fault */
		do_DataAbort(addr, 15 + (1 << 11), regs);
	}
#endif
=======
		set_tls(regs->ARM_r0);
		return 0;

	case NR(get_tls):
		return current_thread_info()->tp_value[0];
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	default:
		/* Calls 9f00xx..9f07ff are defined to return -ENOSYS
		   if not implemented, rather than raising SIGILL.  This
		   way the calling program can gracefully determine whether
		   a feature is supported.  */
		if ((no & 0xffff) <= 0x7ff)
			return -ENOSYS;
		break;
	}
#ifdef CONFIG_DEBUG_USER
	/*
	 * experience shows that these seem to indicate that
	 * something catastrophic has happened
	 */
	if (user_debug & UDBG_SYSCALL) {
<<<<<<< HEAD
		printk("[%d] %s: arm syscall %d\n",
		       task_pid_nr(current), current->comm, no);
		dump_instr("", regs);
		if (user_mode(regs)) {
			__show_regs(regs);
			c_backtrace(regs->ARM_fp, processor_mode(regs));
		}
	}
#endif
	info.si_signo = SIGILL;
	info.si_errno = 0;
	info.si_code  = ILL_ILLTRP;
	info.si_addr  = (void __user *)instruction_pointer(regs) -
			 (thumb_mode(regs) ? 2 : 4);

	arm_notify_die("Oops - bad syscall(2)", regs, &info, no, 0);
=======
		pr_err("[%d] %s: arm syscall %d\n",
		       task_pid_nr(current), current->comm, no);
		dump_instr(KERN_ERR, regs);
		if (user_mode(regs)) {
			__show_regs(regs);
			c_backtrace(frame_pointer(regs), processor_mode(regs), KERN_ERR);
		}
	}
#endif
	arm_notify_die("Oops - bad syscall(2)", regs, SIGILL, ILL_ILLTRP,
		       (void __user *)instruction_pointer(regs) -
			 (thumb_mode(regs) ? 2 : 4),
		       no, 0);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	return 0;
}

#ifdef CONFIG_TLS_REG_EMUL

/*
 * We might be running on an ARMv6+ processor which should have the TLS
 * register but for some reason we can't use it, or maybe an SMP system
 * using a pre-ARMv6 processor (there are apparently a few prototypes like
 * that in existence) and therefore access to that register must be
 * emulated.
 */

static int get_tp_trap(struct pt_regs *regs, unsigned int instr)
{
	int reg = (instr >> 12) & 15;
	if (reg == 15)
		return 1;
	regs->uregs[reg] = current_thread_info()->tp_value[0];
	regs->ARM_pc += 4;
	return 0;
}

static struct undef_hook arm_mrc_hook = {
	.instr_mask	= 0x0fff0fff,
	.instr_val	= 0x0e1d0f70,
	.cpsr_mask	= PSR_T_BIT,
	.cpsr_val	= 0,
	.fn		= get_tp_trap,
};

static int __init arm_mrc_hook_init(void)
{
	register_undef_hook(&arm_mrc_hook);
	return 0;
}

late_initcall(arm_mrc_hook_init);

#endif

<<<<<<< HEAD
void __bad_xchg(volatile void *ptr, int size)
{
	printk("xchg: bad data size: pc 0x%p, ptr 0x%p, size %d\n",
		__builtin_return_address(0), ptr, size);
	BUG();
}
EXPORT_SYMBOL(__bad_xchg);

=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
/*
 * A data abort trap was taken, but we did not handle the instruction.
 * Try to abort the user program, or panic if it was the kernel.
 */
asmlinkage void
baddataabort(int code, unsigned long instr, struct pt_regs *regs)
{
	unsigned long addr = instruction_pointer(regs);
<<<<<<< HEAD
	siginfo_t info;

#ifdef CONFIG_DEBUG_USER
	if (user_debug & UDBG_BADABORT) {
		printk(KERN_ERR "[%d] %s: bad data abort: code %d instr 0x%08lx\n",
			task_pid_nr(current), current->comm, code, instr);
		dump_instr(KERN_ERR, regs);
		show_pte(current->mm, addr);
	}
#endif

	info.si_signo = SIGILL;
	info.si_errno = 0;
	info.si_code  = ILL_ILLOPC;
	info.si_addr  = (void __user *)addr;

	arm_notify_die("unknown data abort code", regs, &info, instr, 0);
=======

#ifdef CONFIG_DEBUG_USER
	if (user_debug & UDBG_BADABORT) {
		pr_err("8<--- cut here ---\n");
		pr_err("[%d] %s: bad data abort: code %d instr 0x%08lx\n",
		       task_pid_nr(current), current->comm, code, instr);
		dump_instr(KERN_ERR, regs);
		show_pte(KERN_ERR, current->mm, addr);
	}
#endif

	arm_notify_die("unknown data abort code", regs,
		       SIGILL, ILL_ILLOPC, (void __user *)addr, instr, 0);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

void __readwrite_bug(const char *fn)
{
<<<<<<< HEAD
	printk("%s called, but not implemented\n", fn);
=======
	pr_err("%s called, but not implemented\n", fn);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	BUG();
}
EXPORT_SYMBOL(__readwrite_bug);

<<<<<<< HEAD
void __pte_error(const char *file, int line, pte_t pte)
{
	printk("%s:%d: bad pte %08llx.\n", file, line, (long long)pte_val(pte));
=======
#ifdef CONFIG_MMU
void __pte_error(const char *file, int line, pte_t pte)
{
	pr_err("%s:%d: bad pte %08llx.\n", file, line, (long long)pte_val(pte));
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

void __pmd_error(const char *file, int line, pmd_t pmd)
{
<<<<<<< HEAD
	printk("%s:%d: bad pmd %08llx.\n", file, line, (long long)pmd_val(pmd));
=======
	pr_err("%s:%d: bad pmd %08llx.\n", file, line, (long long)pmd_val(pmd));
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

void __pgd_error(const char *file, int line, pgd_t pgd)
{
<<<<<<< HEAD
	printk("%s:%d: bad pgd %08llx.\n", file, line, (long long)pgd_val(pgd));
}

asmlinkage void __div0(void)
{
	printk("Division by zero in kernel.\n");
	BUG_ON(PANIC_CORRUPTION);
=======
	pr_err("%s:%d: bad pgd %08llx.\n", file, line, (long long)pgd_val(pgd));
}
#endif

asmlinkage void __div0(void)
{
	pr_err("Division by zero in kernel.\n");
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	dump_stack();
}
EXPORT_SYMBOL(__div0);

void abort(void)
{
	BUG();

	/* if that doesn't kill us, halt */
	panic("Oops failed to kill thread");
}
<<<<<<< HEAD
EXPORT_SYMBOL(abort);

void __init trap_init(void)
{
	return;
}
=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

#ifdef CONFIG_KUSER_HELPERS
static void __init kuser_init(void *vectors)
{
	extern char __kuser_helper_start[], __kuser_helper_end[];
	int kuser_sz = __kuser_helper_end - __kuser_helper_start;

	memcpy(vectors + 0x1000 - kuser_sz, __kuser_helper_start, kuser_sz);

	/*
	 * vectors + 0xfe0 = __kuser_get_tls
	 * vectors + 0xfe8 = hardware TLS instruction at 0xffff0fe8
	 */
	if (tls_emu || has_tls_reg)
		memcpy(vectors + 0xfe0, vectors + 0xfe8, 4);
}
#else
<<<<<<< HEAD
static void __init kuser_init(void *vectors)
=======
static inline void __init kuser_init(void *vectors)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
{
}
#endif

<<<<<<< HEAD
void __init early_trap_init(void *vectors_base)
{
	unsigned long vectors = (unsigned long)vectors_base;
=======
#ifndef CONFIG_CPU_V7M
static void copy_from_lma(void *vma, void *lma_start, void *lma_end)
{
	memcpy(vma, lma_start, lma_end - lma_start);
}

static void flush_vectors(void *vma, size_t offset, size_t size)
{
	unsigned long start = (unsigned long)vma + offset;
	unsigned long end = start + size;

	flush_icache_range(start, end);
}

#ifdef CONFIG_HARDEN_BRANCH_HISTORY
int spectre_bhb_update_vectors(unsigned int method)
{
	extern char __vectors_bhb_bpiall_start[], __vectors_bhb_bpiall_end[];
	extern char __vectors_bhb_loop8_start[], __vectors_bhb_loop8_end[];
	void *vec_start, *vec_end;

	if (system_state >= SYSTEM_FREEING_INITMEM) {
		pr_err("CPU%u: Spectre BHB workaround too late - system vulnerable\n",
		       smp_processor_id());
		return SPECTRE_VULNERABLE;
	}

	switch (method) {
	case SPECTRE_V2_METHOD_LOOP8:
		vec_start = __vectors_bhb_loop8_start;
		vec_end = __vectors_bhb_loop8_end;
		break;

	case SPECTRE_V2_METHOD_BPIALL:
		vec_start = __vectors_bhb_bpiall_start;
		vec_end = __vectors_bhb_bpiall_end;
		break;

	default:
		pr_err("CPU%u: unknown Spectre BHB state %d\n",
		       smp_processor_id(), method);
		return SPECTRE_VULNERABLE;
	}

	copy_from_lma(vectors_page, vec_start, vec_end);
	flush_vectors(vectors_page, 0, vec_end - vec_start);

	return SPECTRE_MITIGATED;
}
#endif

void __init early_trap_init(void *vectors_base)
{
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	extern char __stubs_start[], __stubs_end[];
	extern char __vectors_start[], __vectors_end[];
	unsigned i;

	vectors_page = vectors_base;

	/*
	 * Poison the vectors page with an undefined instruction.  This
	 * instruction is chosen to be undefined for both ARM and Thumb
	 * ISAs.  The Thumb version is an undefined instruction with a
	 * branch back to the undefined instruction.
	 */
	for (i = 0; i < PAGE_SIZE / sizeof(u32); i++)
		((u32 *)vectors_base)[i] = 0xe7fddef1;

	/*
	 * Copy the vectors, stubs and kuser helpers (in entry-armv.S)
	 * into the vector page, mapped at 0xffff0000, and ensure these
	 * are visible to the instruction stream.
	 */
<<<<<<< HEAD
	memcpy((void *)vectors, __vectors_start, __vectors_end - __vectors_start);
	memcpy((void *)vectors + 0x1000, __stubs_start, __stubs_end - __stubs_start);

	kuser_init(vectors_base);

	flush_icache_range(vectors, vectors + PAGE_SIZE * 2);
	modify_domain(DOMAIN_USER, DOMAIN_CLIENT);
}
=======
	copy_from_lma(vectors_base, __vectors_start, __vectors_end);
	copy_from_lma(vectors_base + 0x1000, __stubs_start, __stubs_end);

	kuser_init(vectors_base);

	flush_vectors(vectors_base, 0, PAGE_SIZE * 2);
}
#else /* ifndef CONFIG_CPU_V7M */
void __init early_trap_init(void *vectors_base)
{
	/*
	 * on V7-M there is no need to copy the vector table to a dedicated
	 * memory area. The address is configurable and so a table in the kernel
	 * image can be used.
	 */
}
#endif

#ifdef CONFIG_VMAP_STACK

DECLARE_PER_CPU(u8 *, irq_stack_ptr);

asmlinkage DEFINE_PER_CPU(u8 *, overflow_stack_ptr);

static int __init allocate_overflow_stacks(void)
{
	u8 *stack;
	int cpu;

	for_each_possible_cpu(cpu) {
		stack = (u8 *)__get_free_page(GFP_KERNEL);
		if (WARN_ON(!stack))
			return -ENOMEM;
		per_cpu(overflow_stack_ptr, cpu) = &stack[OVERFLOW_STACK_SIZE];
	}
	return 0;
}
early_initcall(allocate_overflow_stacks);

asmlinkage void handle_bad_stack(struct pt_regs *regs)
{
	unsigned long tsk_stk = (unsigned long)current->stack;
#ifdef CONFIG_IRQSTACKS
	unsigned long irq_stk = (unsigned long)raw_cpu_read(irq_stack_ptr);
#endif
	unsigned long ovf_stk = (unsigned long)raw_cpu_read(overflow_stack_ptr);

	console_verbose();
	pr_emerg("Insufficient stack space to handle exception!");

	pr_emerg("Task stack:     [0x%08lx..0x%08lx]\n",
		 tsk_stk, tsk_stk + THREAD_SIZE);
#ifdef CONFIG_IRQSTACKS
	pr_emerg("IRQ stack:      [0x%08lx..0x%08lx]\n",
		 irq_stk - THREAD_SIZE, irq_stk);
#endif
	pr_emerg("Overflow stack: [0x%08lx..0x%08lx]\n",
		 ovf_stk - OVERFLOW_STACK_SIZE, ovf_stk);

	die("kernel stack overflow", regs, 0);
}

#ifndef CONFIG_ARM_LPAE
/*
 * Normally, we rely on the logic in do_translation_fault() to update stale PMD
 * entries covering the vmalloc space in a task's page tables when it first
 * accesses the region in question. Unfortunately, this is not sufficient when
 * the task stack resides in the vmalloc region, as do_translation_fault() is a
 * C function that needs a stack to run.
 *
 * So we need to ensure that these PMD entries are up to date *before* the MM
 * switch. As we already have some logic in the MM switch path that takes care
 * of this, let's trigger it by bumping the counter every time the core vmalloc
 * code modifies a PMD entry in the vmalloc region. Use release semantics on
 * the store so that other CPUs observing the counter's new value are
 * guaranteed to see the updated page table entries as well.
 */
void arch_sync_kernel_mappings(unsigned long start, unsigned long end)
{
	if (start < VMALLOC_END && end > VMALLOC_START)
		atomic_inc_return_release(&init_mm.context.vmalloc_seq);
}
#endif
#endif
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
