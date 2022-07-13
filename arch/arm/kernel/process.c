<<<<<<< HEAD
=======
// SPDX-License-Identifier: GPL-2.0-only
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
/*
 *  linux/arch/arm/kernel/process.c
 *
 *  Copyright (C) 1996-2000 Russell King - Converted to ARM.
 *  Original Copyright (C) 1995  Linus Torvalds
<<<<<<< HEAD
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 */
#include <stdarg.h>

#include <linux/export.h>
#include <linux/sched.h>
#include <linux/kernel.h>
#include <linux/mm.h>
#include <linux/vmalloc.h>
#include <linux/stddef.h>
#include <linux/unistd.h>
#include <linux/user.h>
#include <linux/delay.h>
#include <linux/reboot.h>
#include <linux/interrupt.h>
#include <linux/kallsyms.h>
#include <linux/init.h>
#include <linux/cpu.h>
=======
 */
#include <linux/export.h>
#include <linux/sched.h>
#include <linux/sched/debug.h>
#include <linux/sched/task.h>
#include <linux/sched/task_stack.h>
#include <linux/kernel.h>
#include <linux/mm.h>
#include <linux/stddef.h>
#include <linux/unistd.h>
#include <linux/user.h>
#include <linux/interrupt.h>
#include <linux/init.h>
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#include <linux/elfcore.h>
#include <linux/pm.h>
#include <linux/tick.h>
#include <linux/utsname.h>
#include <linux/uaccess.h>
#include <linux/random.h>
#include <linux/hw_breakpoint.h>
<<<<<<< HEAD
#include <linux/cpuidle.h>
#include <linux/console.h>

#include <asm/cacheflush.h>
#include <asm/processor.h>
#include <asm/thread_notify.h>
#include <asm/stacktrace.h>
#include <asm/mach/time.h>
#include <asm/tls.h>

#ifdef CONFIG_CC_STACKPROTECTOR
=======
#include <linux/leds.h>

#include <asm/processor.h>
#include <asm/thread_notify.h>
#include <asm/stacktrace.h>
#include <asm/system_misc.h>
#include <asm/mach/time.h>
#include <asm/tls.h>
#include <asm/vdso.h>

#include "signal.h"

#if defined(CONFIG_CURRENT_POINTER_IN_TPIDRURO) || defined(CONFIG_SMP)
DEFINE_PER_CPU(struct task_struct *, __entry_task);
#endif

#if defined(CONFIG_STACKPROTECTOR) && !defined(CONFIG_STACKPROTECTOR_PER_TASK)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#include <linux/stackprotector.h>
unsigned long __stack_chk_guard __read_mostly;
EXPORT_SYMBOL(__stack_chk_guard);
#endif

<<<<<<< HEAD
static const char *processor_modes[] = {
  "USER_26", "FIQ_26" , "IRQ_26" , "SVC_26" , "UK4_26" , "UK5_26" , "UK6_26" , "UK7_26" ,
  "UK8_26" , "UK9_26" , "UK10_26", "UK11_26", "UK12_26", "UK13_26", "UK14_26", "UK15_26",
  "USER_32", "FIQ_32" , "IRQ_32" , "SVC_32" , "UK4_32" , "UK5_32" , "UK6_32" , "ABT_32" ,
  "UK8_32" , "UK9_32" , "UK10_32", "UND_32" , "UK12_32", "UK13_32", "UK14_32", "SYS_32"
};

static const char *isa_modes[] = {
  "ARM" , "Thumb" , "Jazelle", "ThumbEE"
};

extern void setup_mm_for_reboot(void);

static volatile int hlt_counter;

#ifdef CONFIG_SMP
void arch_trigger_all_cpu_backtrace(void)
{
	smp_send_all_cpu_backtrace();
}
#else
void arch_trigger_all_cpu_backtrace(void)
{
	dump_stack();
}
#endif

void disable_hlt(void)
{
	hlt_counter++;
}

EXPORT_SYMBOL(disable_hlt);

void enable_hlt(void)
{
	hlt_counter--;
}

EXPORT_SYMBOL(enable_hlt);

int get_hlt(void)
{
	return hlt_counter;
}
EXPORT_SYMBOL(get_hlt);

static int __init nohlt_setup(char *__unused)
{
	hlt_counter = 1;
	return 1;
}

static int __init hlt_setup(char *__unused)
{
	hlt_counter = 0;
	return 1;
}

__setup("nohlt", nohlt_setup);
__setup("hlt", hlt_setup);

extern void call_with_stack(void (*fn)(void *), void *arg, void *sp);
typedef void (*phys_reset_t)(unsigned long);

#ifdef CONFIG_ARM_FLUSH_CONSOLE_ON_RESTART
void arm_machine_flush_console(void)
{
	printk("\n");
	pr_emerg("Restarting %s\n", linux_banner);
	if (console_trylock()) {
		console_unlock();
		return;
	}

	mdelay(50);

	local_irq_disable();
	if (!console_trylock())
		pr_emerg("arm_restart: Console was locked! Busting\n");
	else
		pr_emerg("arm_restart: Console was locked!\n");
	console_unlock();
}
#else
void arm_machine_flush_console(void)
{
}
#endif

/*
 * A temporary stack to use for CPU reset. This is static so that we
 * don't clobber it with the identity mapping. When running with this
 * stack, any references to the current task *will not work* so you
 * should really do as little as possible before jumping to your reset
 * code.
 */
static u64 soft_restart_stack[16];

static void __soft_restart(void *addr)
{
	phys_reset_t phys_reset;

	/* Take out a flat memory mapping. */
	setup_mm_for_reboot();

	/* Clean and invalidate caches */
	flush_cache_all();

	/* Turn off caching */
	cpu_proc_fin();

	/* Push out any further dirty data, and ensure cache is empty */
	flush_cache_all();

	/* Push out the dirty data from external caches */
	outer_disable();

	/* Switch to the identity mapping. */
	phys_reset = (phys_reset_t)(unsigned long)virt_to_phys(cpu_reset);
	phys_reset((unsigned long)addr);

	/* Should never get here. */
	BUG();
}

void soft_restart(unsigned long addr)
{
	u64 *stack = soft_restart_stack + ARRAY_SIZE(soft_restart_stack);

	/* Disable interrupts first */
	local_irq_disable();
	local_fiq_disable();

	/* Disable the L2 if we're the last man standing. */
	if (num_online_cpus() == 1)
		outer_disable();

	/* Change to the new stack and continue with the reset. */
	call_with_stack(__soft_restart, (void *)addr, (void *)stack);

	/* Should never get here. */
	BUG();
}

static void null_restart(char mode, const char *cmd)
{
}

/*
 * Function pointers to optional machine specific functions
 */
void (*pm_power_off)(void);
EXPORT_SYMBOL(pm_power_off);

void (*arm_pm_restart)(char str, const char *cmd) = null_restart;
EXPORT_SYMBOL_GPL(arm_pm_restart);

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

=======
#ifndef CONFIG_CURRENT_POINTER_IN_TPIDRURO
asmlinkage struct task_struct *__current;
EXPORT_SYMBOL(__current);
#endif

static const char *processor_modes[] __maybe_unused = {
  "USER_26", "FIQ_26" , "IRQ_26" , "SVC_26" , "UK4_26" , "UK5_26" , "UK6_26" , "UK7_26" ,
  "UK8_26" , "UK9_26" , "UK10_26", "UK11_26", "UK12_26", "UK13_26", "UK14_26", "UK15_26",
  "USER_32", "FIQ_32" , "IRQ_32" , "SVC_32" , "UK4_32" , "UK5_32" , "MON_32" , "ABT_32" ,
  "UK8_32" , "UK9_32" , "HYP_32", "UND_32" , "UK12_32", "UK13_32", "UK14_32", "SYS_32"
};

static const char *isa_modes[] __maybe_unused = {
  "ARM" , "Thumb" , "Jazelle", "ThumbEE"
};

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
/*
 * This is our default idle handler.
 */

<<<<<<< HEAD
extern void arch_idle(void);
void (*arm_pm_idle)(void) = arch_idle;

static void default_idle(void)
=======
void (*arm_pm_idle)(void);

/*
 * Called from the core idle loop.
 */

void arch_cpu_idle(void)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
{
	if (arm_pm_idle)
		arm_pm_idle();
	else
		cpu_do_idle();
<<<<<<< HEAD
	local_irq_enable();
}

void (*pm_idle)(void) = default_idle;
EXPORT_SYMBOL(pm_idle);

/*
 * The idle thread, has rather strange semantics for calling pm_idle,
 * but this is what x86 does and we need to do the same, so that
 * things like cpuidle get called in the same way.  The only difference
 * is that we always respect 'hlt_counter' to prevent low power idle.
 */
void cpu_idle(void)
{
	local_fiq_enable();

	/* endless idle loop with no priority at all */
	while (1) {
		idle_notifier_call_chain(IDLE_START);
		tick_nohz_idle_enter();
		rcu_idle_enter();
		while (!need_resched()) {
			/*
			 * We need to disable interrupts here
			 * to ensure we don't miss a wakeup call.
			 */
			local_irq_disable();
#ifdef CONFIG_PL310_ERRATA_769419
			wmb();
#endif
			if (hlt_counter) {
				local_irq_enable();
				cpu_relax();
			} else if (!need_resched()) {
				stop_critical_timings();
				if (cpuidle_idle_call())
					pm_idle();
				start_critical_timings();
				/*
				 * pm_idle functions must always
				 * return with IRQs enabled.
				 */
				WARN_ON(irqs_disabled());
			} else
				local_irq_enable();
		}
		rcu_idle_exit();
		tick_nohz_idle_exit();
		idle_notifier_call_chain(IDLE_END);
		schedule_preempt_disabled();
#ifdef CONFIG_HOTPLUG_CPU
		if (cpu_is_offline(smp_processor_id()))
			cpu_die();
#endif
	}
}

static char reboot_mode = 'h';

int __init reboot_setup(char *str)
{
	reboot_mode = str[0];
	return 1;
}

__setup("reboot=", reboot_setup);

void machine_shutdown(void)
{
	preempt_disable();
#ifdef CONFIG_SMP
	smp_send_stop();
#endif
}

void machine_halt(void)
{
	machine_shutdown();
	local_irq_disable();
	while (1);
}

void machine_power_off(void)
{
	machine_shutdown();
	if (pm_power_off)
		pm_power_off();
}

void machine_restart(char *cmd)
{
	machine_shutdown();

	/* Flush the console to make sure all the relevant messages make it
	 * out to the console drivers */
	arm_machine_flush_console();

	arm_pm_restart(reboot_mode, cmd);

	/* Give a grace period for failure to restart of 1s */
	mdelay(1000);

	/* Whoops - the platform was unable to reboot. Tell the user! */
	printk("Reboot failed -- System halted\n");
	local_irq_disable();
	while (1);
}

/*
 * dump a block of kernel memory from around the given address
 */
static void show_data(unsigned long addr, int nbytes, const char *name)
{
	int	i, j;
	int	nlines;
	u32	*p;

	/*
	 * don't attempt to dump non-kernel addresses or
	 * values that are probably just small negative numbers
	 */
	if (addr < PAGE_OFFSET || addr > -256UL)
		return;

	printk("\n%s: %#lx:\n", name, addr);

	/*
	 * round address down to a 32 bit boundary
	 * and always dump a multiple of 32 bytes
	 */
	p = (u32 *)(addr & ~(sizeof(u32) - 1));
	nbytes += (addr & (sizeof(u32) - 1));
	nlines = (nbytes + 31) / 32;


	for (i = 0; i < nlines; i++) {
		/*
		 * just display low 16 bits of address to keep
		 * each line of the dump < 80 characters
		 */
		printk("%04lx ", (unsigned long)p & 0xffff);
		for (j = 0; j < 8; j++) {
			u32	data;
			/*
			 * vmalloc addresses may point to
			 * memory-mapped peripherals
			 */
			if (is_vmalloc_addr(p) ||
			    probe_kernel_address(p, data)) {
				printk(" ********");
			} else {
				printk(" %08x", data);
			}
			++p;
		}
		printk("\n");
	}
}

static void show_extra_register_data(struct pt_regs *regs, int nbytes)
{
	mm_segment_t fs;

	fs = get_fs();
	set_fs(KERNEL_DS);
	show_data(regs->ARM_pc - nbytes, nbytes * 2, "PC");
	show_data(regs->ARM_lr - nbytes, nbytes * 2, "LR");
	show_data(regs->ARM_sp - nbytes, nbytes * 2, "SP");
	show_data(regs->ARM_ip - nbytes, nbytes * 2, "IP");
	show_data(regs->ARM_fp - nbytes, nbytes * 2, "FP");
	show_data(regs->ARM_r0 - nbytes, nbytes * 2, "R0");
	show_data(regs->ARM_r1 - nbytes, nbytes * 2, "R1");
	show_data(regs->ARM_r2 - nbytes, nbytes * 2, "R2");
	show_data(regs->ARM_r3 - nbytes, nbytes * 2, "R3");
	show_data(regs->ARM_r4 - nbytes, nbytes * 2, "R4");
	show_data(regs->ARM_r5 - nbytes, nbytes * 2, "R5");
	show_data(regs->ARM_r6 - nbytes, nbytes * 2, "R6");
	show_data(regs->ARM_r7 - nbytes, nbytes * 2, "R7");
	show_data(regs->ARM_r8 - nbytes, nbytes * 2, "R8");
	show_data(regs->ARM_r9 - nbytes, nbytes * 2, "R9");
	show_data(regs->ARM_r10 - nbytes, nbytes * 2, "R10");
	set_fs(fs);
}

=======
}

void arch_cpu_idle_prepare(void)
{
	local_fiq_enable();
}

void arch_cpu_idle_enter(void)
{
	ledtrig_cpu(CPU_LED_IDLE_START);
#ifdef CONFIG_PL310_ERRATA_769419
	wmb();
#endif
}

void arch_cpu_idle_exit(void)
{
	ledtrig_cpu(CPU_LED_IDLE_END);
}

void __show_regs_alloc_free(struct pt_regs *regs)
{
	int i;

	/* check for r0 - r12 only */
	for (i = 0; i < 13; i++) {
		pr_alert("Register r%d information:", i);
		mem_dump_obj((void *)regs->uregs[i]);
	}
}

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
void __show_regs(struct pt_regs *regs)
{
	unsigned long flags;
	char buf[64];
<<<<<<< HEAD

	printk("CPU: %d    %s  (%s %.*s)\n",
		raw_smp_processor_id(), print_tainted(),
		init_utsname()->release,
		(int)strcspn(init_utsname()->version, " "),
		init_utsname()->version);
	print_symbol("PC is at %s\n", instruction_pointer(regs));
	print_symbol("LR is at %s\n", regs->ARM_lr);
	printk("pc : [<%08lx>]    lr : [<%08lx>]    psr: %08lx\n"
	       "sp : %08lx  ip : %08lx  fp : %08lx\n",
		regs->ARM_pc, regs->ARM_lr, regs->ARM_cpsr,
		regs->ARM_sp, regs->ARM_ip, regs->ARM_fp);
=======
#ifndef CONFIG_CPU_V7M
	unsigned int domain;
#ifdef CONFIG_CPU_SW_DOMAIN_PAN
	/*
	 * Get the domain register for the parent context. In user
	 * mode, we don't save the DACR, so lets use what it should
	 * be. For other modes, we place it after the pt_regs struct.
	 */
	if (user_mode(regs)) {
		domain = DACR_UACCESS_ENABLE;
	} else {
		domain = to_svc_pt_regs(regs)->dacr;
	}
#else
	domain = get_domain();
#endif
#endif

	show_regs_print_info(KERN_DEFAULT);

	printk("PC is at %pS\n", (void *)instruction_pointer(regs));
	printk("LR is at %pS\n", (void *)regs->ARM_lr);
	printk("pc : [<%08lx>]    lr : [<%08lx>]    psr: %08lx\n",
	       regs->ARM_pc, regs->ARM_lr, regs->ARM_cpsr);
	printk("sp : %08lx  ip : %08lx  fp : %08lx\n",
	       regs->ARM_sp, regs->ARM_ip, regs->ARM_fp);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	printk("r10: %08lx  r9 : %08lx  r8 : %08lx\n",
		regs->ARM_r10, regs->ARM_r9,
		regs->ARM_r8);
	printk("r7 : %08lx  r6 : %08lx  r5 : %08lx  r4 : %08lx\n",
		regs->ARM_r7, regs->ARM_r6,
		regs->ARM_r5, regs->ARM_r4);
	printk("r3 : %08lx  r2 : %08lx  r1 : %08lx  r0 : %08lx\n",
		regs->ARM_r3, regs->ARM_r2,
		regs->ARM_r1, regs->ARM_r0);

	flags = regs->ARM_cpsr;
	buf[0] = flags & PSR_N_BIT ? 'N' : 'n';
	buf[1] = flags & PSR_Z_BIT ? 'Z' : 'z';
	buf[2] = flags & PSR_C_BIT ? 'C' : 'c';
	buf[3] = flags & PSR_V_BIT ? 'V' : 'v';
	buf[4] = '\0';

<<<<<<< HEAD
	printk("Flags: %s  IRQs o%s  FIQs o%s  Mode %s  ISA %s  Segment %s\n",
		buf, interrupts_enabled(regs) ? "n" : "ff",
		fast_interrupts_enabled(regs) ? "n" : "ff",
		processor_modes[processor_mode(regs)],
		isa_modes[isa_mode(regs)],
		get_fs() == get_ds() ? "kernel" : "user");
=======
#ifndef CONFIG_CPU_V7M
	{
		const char *segment;

		if ((domain & domain_mask(DOMAIN_USER)) ==
		    domain_val(DOMAIN_USER, DOMAIN_NOACCESS))
			segment = "none";
		else
			segment = "user";

		printk("Flags: %s  IRQs o%s  FIQs o%s  Mode %s  ISA %s  Segment %s\n",
			buf, interrupts_enabled(regs) ? "n" : "ff",
			fast_interrupts_enabled(regs) ? "n" : "ff",
			processor_modes[processor_mode(regs)],
			isa_modes[isa_mode(regs)], segment);
	}
#else
	printk("xPSR: %08lx\n", regs->ARM_cpsr);
#endif

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#ifdef CONFIG_CPU_CP15
	{
		unsigned int ctrl;

		buf[0] = '\0';
#ifdef CONFIG_CPU_CP15_MMU
		{
<<<<<<< HEAD
			unsigned int transbase, dac;
			asm("mrc p15, 0, %0, c2, c0\n\t"
			    "mrc p15, 0, %1, c3, c0\n"
			    : "=r" (transbase), "=r" (dac));
			snprintf(buf, sizeof(buf), "  Table: %08x  DAC: %08x",
			  	transbase, dac);
=======
			unsigned int transbase;
			asm("mrc p15, 0, %0, c2, c0\n\t"
			    : "=r" (transbase));
			snprintf(buf, sizeof(buf), "  Table: %08x  DAC: %08x",
				transbase, domain);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		}
#endif
		asm("mrc p15, 0, %0, c1, c0\n" : "=r" (ctrl));

		printk("Control: %08x%s\n", ctrl, buf);
	}
#endif
<<<<<<< HEAD

	show_extra_register_data(regs, 128);
=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

void show_regs(struct pt_regs * regs)
{
<<<<<<< HEAD
	printk("\n");
	printk("Pid: %d, comm: %20s\n", task_pid_nr(current), current->comm);
	__show_regs(regs);
	dump_stack();
=======
	__show_regs(regs);
	dump_backtrace(regs, NULL, KERN_DEFAULT);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

ATOMIC_NOTIFIER_HEAD(thread_notify_head);

EXPORT_SYMBOL_GPL(thread_notify_head);

/*
 * Free current thread data structures etc..
 */
<<<<<<< HEAD
void exit_thread(void)
{
	thread_notify(THREAD_NOTIFY_EXIT, current_thread_info());
=======
void exit_thread(struct task_struct *tsk)
{
	thread_notify(THREAD_NOTIFY_EXIT, task_thread_info(tsk));
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

void flush_thread(void)
{
	struct thread_info *thread = current_thread_info();
	struct task_struct *tsk = current;

	flush_ptrace_hw_breakpoint(tsk);

<<<<<<< HEAD
	memset(thread->used_cp, 0, sizeof(thread->used_cp));
	memset(&tsk->thread.debug, 0, sizeof(struct debug_info));
	memset(&thread->fpstate, 0, sizeof(union fp_state));

	thread_notify(THREAD_NOTIFY_FLUSH, thread);
}

void release_thread(struct task_struct *dead_task)
{
}

asmlinkage void ret_from_fork(void) __asm__("ret_from_fork");

int
copy_thread(unsigned long clone_flags, unsigned long stack_start,
	    unsigned long stk_sz, struct task_struct *p, struct pt_regs *regs)
{
	struct thread_info *thread = task_thread_info(p);
	struct pt_regs *childregs = task_pt_regs(p);

	*childregs = *regs;
	childregs->ARM_r0 = 0;
	childregs->ARM_sp = stack_start;

	memset(&thread->cpu_context, 0, sizeof(struct cpu_context_save));
	thread->cpu_context.sp = (unsigned long)childregs;
	thread->cpu_context.pc = (unsigned long)ret_from_fork;
=======
	memset(&tsk->thread.debug, 0, sizeof(struct debug_info));
	memset(&thread->fpstate, 0, sizeof(union fp_state));

	flush_tls();

	thread_notify(THREAD_NOTIFY_FLUSH, thread);
}

asmlinkage void ret_from_fork(void) __asm__("ret_from_fork");

int copy_thread(struct task_struct *p, const struct kernel_clone_args *args)
{
	unsigned long clone_flags = args->flags;
	unsigned long stack_start = args->stack;
	unsigned long tls = args->tls;
	struct thread_info *thread = task_thread_info(p);
	struct pt_regs *childregs = task_pt_regs(p);

	memset(&thread->cpu_context, 0, sizeof(struct cpu_context_save));

#ifdef CONFIG_CPU_USE_DOMAINS
	/*
	 * Copy the initial value of the domain access control register
	 * from the current thread: thread->addr_limit will have been
	 * copied from the current thread via setup_thread_stack() in
	 * kernel/fork.c
	 */
	thread->cpu_domain = get_domain();
#endif

	if (likely(!args->fn)) {
		*childregs = *current_pt_regs();
		childregs->ARM_r0 = 0;
		if (stack_start)
			childregs->ARM_sp = stack_start;
	} else {
		memset(childregs, 0, sizeof(struct pt_regs));
		thread->cpu_context.r4 = (unsigned long)args->fn_arg;
		thread->cpu_context.r5 = (unsigned long)args->fn;
		childregs->ARM_cpsr = SVC_MODE;
	}
	thread->cpu_context.pc = (unsigned long)ret_from_fork;
	thread->cpu_context.sp = (unsigned long)childregs;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	clear_ptrace_hw_breakpoint(p);

	if (clone_flags & CLONE_SETTLS)
<<<<<<< HEAD
		thread->tp_value[0] = childregs->ARM_r3;
=======
		thread->tp_value[0] = tls;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	thread->tp_value[1] = get_tpuser();

	thread_notify(THREAD_NOTIFY_COPY, thread);

	return 0;
}

<<<<<<< HEAD
/*
 * Fill in the task's elfregs structure for a core dump.
 */
int dump_task_regs(struct task_struct *t, elf_gregset_t *elfregs)
{
	elf_core_copy_regs(elfregs, task_pt_regs(t));
	return 1;
}

/*
 * fill in the fpe structure for a core dump...
 */
int dump_fpu (struct pt_regs *regs, struct user_fp *fp)
{
	struct thread_info *thread = current_thread_info();
	int used_math = thread->used_cp[1] | thread->used_cp[2];

	if (used_math)
		memcpy(fp, &thread->fpstate.soft, sizeof (*fp));

	return used_math != 0;
}
EXPORT_SYMBOL(dump_fpu);

/*
 * Shuffle the argument into the correct register before calling the
 * thread function.  r4 is the thread argument, r5 is the pointer to
 * the thread function, and r6 points to the exit function.
 */
extern void kernel_thread_helper(void);
asm(	".pushsection .text\n"
"	.align\n"
"	.type	kernel_thread_helper, #function\n"
"kernel_thread_helper:\n"
#ifdef CONFIG_TRACE_IRQFLAGS
"	bl	trace_hardirqs_on\n"
#endif
"	msr	cpsr_c, r7\n"
"	mov	r0, r4\n"
"	mov	lr, r6\n"
"	mov	pc, r5\n"
"	.size	kernel_thread_helper, . - kernel_thread_helper\n"
"	.popsection");

#ifdef CONFIG_ARM_UNWIND
extern void kernel_thread_exit(long code);
asm(	".pushsection .text\n"
"	.align\n"
"	.type	kernel_thread_exit, #function\n"
"kernel_thread_exit:\n"
"	.fnstart\n"
"	.cantunwind\n"
"	bl	do_exit\n"
"	nop\n"
"	.fnend\n"
"	.size	kernel_thread_exit, . - kernel_thread_exit\n"
"	.popsection");
#else
#define kernel_thread_exit	do_exit
#endif

/*
 * Create a kernel thread.
 */
pid_t kernel_thread(int (*fn)(void *), void *arg, unsigned long flags)
{
	struct pt_regs regs;

	memset(&regs, 0, sizeof(regs));

	regs.ARM_r4 = (unsigned long)arg;
	regs.ARM_r5 = (unsigned long)fn;
	regs.ARM_r6 = (unsigned long)kernel_thread_exit;
	regs.ARM_r7 = SVC_MODE | PSR_ENDSTATE | PSR_ISETSTATE;
	regs.ARM_pc = (unsigned long)kernel_thread_helper;
	regs.ARM_cpsr = regs.ARM_r7 | PSR_I_BIT;

	return do_fork(flags|CLONE_VM|CLONE_UNTRACED, 0, &regs, 0, NULL, NULL);
}
EXPORT_SYMBOL(kernel_thread);

unsigned long get_wchan(struct task_struct *p)
=======
unsigned long __get_wchan(struct task_struct *p)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
{
	struct stackframe frame;
	unsigned long stack_page;
	int count = 0;
<<<<<<< HEAD
	if (!p || p == current || p->state == TASK_RUNNING)
		return 0;
=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	frame.fp = thread_saved_fp(p);
	frame.sp = thread_saved_sp(p);
	frame.lr = 0;			/* recovered from the stack */
	frame.pc = thread_saved_pc(p);
	stack_page = (unsigned long)task_stack_page(p);
	do {
		if (frame.sp < stack_page ||
		    frame.sp >= stack_page + THREAD_SIZE ||
		    unwind_frame(&frame) < 0)
			return 0;
		if (!in_sched_functions(frame.pc))
			return frame.pc;
	} while (count ++ < 16);
	return 0;
}

<<<<<<< HEAD
unsigned long arch_randomize_brk(struct mm_struct *mm)
{
	unsigned long range_end = mm->brk + 0x02000000;
	return randomize_range(mm->brk, range_end, 0) ? : mm->brk;
}

=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#ifdef CONFIG_MMU
#ifdef CONFIG_KUSER_HELPERS
/*
 * The vectors page is always readable from user space for the
 * atomic helpers. Insert it into the gate_vma so that it is visible
 * through ptrace and /proc/<pid>/mem.
 */
static struct vm_area_struct gate_vma;

static int __init gate_vma_init(void)
{
<<<<<<< HEAD
	gate_vma.vm_start	= 0xffff0000;
	gate_vma.vm_end		= 0xffff0000 + PAGE_SIZE;
	gate_vma.vm_page_prot	= PAGE_READONLY_EXEC;
	gate_vma.vm_flags	= VM_READ | VM_EXEC |
				  VM_MAYREAD | VM_MAYEXEC;
=======
	vma_init(&gate_vma, NULL);
	gate_vma.vm_page_prot = PAGE_READONLY_EXEC;
	gate_vma.vm_start = 0xffff0000;
	gate_vma.vm_end	= 0xffff0000 + PAGE_SIZE;
	vm_flags_init(&gate_vma, VM_READ | VM_EXEC | VM_MAYREAD | VM_MAYEXEC);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	return 0;
}
arch_initcall(gate_vma_init);

struct vm_area_struct *get_gate_vma(struct mm_struct *mm)
{
	return &gate_vma;
}

int in_gate_area(struct mm_struct *mm, unsigned long addr)
{
	return (addr >= gate_vma.vm_start) && (addr < gate_vma.vm_end);
}

int in_gate_area_no_mm(unsigned long addr)
{
	return in_gate_area(NULL, addr);
}
#define is_gate_vma(vma)	((vma) == &gate_vma)
#else
#define is_gate_vma(vma)	0
#endif

const char *arch_vma_name(struct vm_area_struct *vma)
{
<<<<<<< HEAD
	if (is_gate_vma(vma))
		return "[vectors]";
	else if (vma->vm_mm && vma->vm_start == vma->vm_mm->context.sigpage)
		return "[sigpage]";
	else if (vma == get_user_timers_vma(NULL))
		return "[timers]";
	else
		return NULL;
=======
	return is_gate_vma(vma) ? "[vectors]" : NULL;
}

/* If possible, provide a placement hint at a random offset from the
 * stack for the sigpage and vdso pages.
 */
static unsigned long sigpage_addr(const struct mm_struct *mm,
				  unsigned int npages)
{
	unsigned long offset;
	unsigned long first;
	unsigned long last;
	unsigned long addr;
	unsigned int slots;

	first = PAGE_ALIGN(mm->start_stack);

	last = TASK_SIZE - (npages << PAGE_SHIFT);

	/* No room after stack? */
	if (first > last)
		return 0;

	/* Just enough room? */
	if (first == last)
		return first;

	slots = ((last - first) >> PAGE_SHIFT) + 1;

	offset = get_random_u32_below(slots);

	addr = first + (offset << PAGE_SHIFT);

	return addr;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

static struct page *signal_page;
extern struct page *get_signal_page(void);

<<<<<<< HEAD
int arch_setup_additional_pages(struct linux_binprm *bprm, int uses_interp)
{
	struct mm_struct *mm = current->mm;
	unsigned long addr;
	int ret;
=======
static int sigpage_mremap(const struct vm_special_mapping *sm,
		struct vm_area_struct *new_vma)
{
	current->mm->context.sigpage = new_vma->vm_start;
	return 0;
}

static const struct vm_special_mapping sigpage_mapping = {
	.name = "[sigpage]",
	.pages = &signal_page,
	.mremap = sigpage_mremap,
};

int arch_setup_additional_pages(struct linux_binprm *bprm, int uses_interp)
{
	struct mm_struct *mm = current->mm;
	struct vm_area_struct *vma;
	unsigned long npages;
	unsigned long addr;
	unsigned long hint;
	int ret = 0;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	if (!signal_page)
		signal_page = get_signal_page();
	if (!signal_page)
		return -ENOMEM;

<<<<<<< HEAD
	down_write(&mm->mmap_sem);
	addr = get_unmapped_area(NULL, 0, PAGE_SIZE, 0, 0);
=======
	npages = 1; /* for sigpage */
	npages += vdso_total_pages;

	if (mmap_write_lock_killable(mm))
		return -EINTR;
	hint = sigpage_addr(mm, npages);
	addr = get_unmapped_area(NULL, hint, npages << PAGE_SHIFT, 0, 0);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	if (IS_ERR_VALUE(addr)) {
		ret = addr;
		goto up_fail;
	}

<<<<<<< HEAD
	ret = install_special_mapping(mm, addr, PAGE_SIZE,
		VM_READ | VM_EXEC | VM_MAYREAD | VM_MAYWRITE | VM_MAYEXEC,
		&signal_page);

	if (ret == 0)
		mm->context.sigpage = addr;

 up_fail:
	up_write(&mm->mmap_sem);
=======
	vma = _install_special_mapping(mm, addr, PAGE_SIZE,
		VM_READ | VM_EXEC | VM_MAYREAD | VM_MAYWRITE | VM_MAYEXEC,
		&sigpage_mapping);

	if (IS_ERR(vma)) {
		ret = PTR_ERR(vma);
		goto up_fail;
	}

	mm->context.sigpage = addr;

	/* Unlike the sigpage, failure to install the vdso is unlikely
	 * to be fatal to the process, so no error check needed
	 * here.
	 */
	arm_install_vdso(mm, addr + PAGE_SIZE);

 up_fail:
	mmap_write_unlock(mm);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	return ret;
}
#endif
