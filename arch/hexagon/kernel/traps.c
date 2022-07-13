<<<<<<< HEAD
/*
 * Kernel traps/events for Hexagon processor
 *
 * Copyright (c) 2010-2011, The Linux Foundation. All rights reserved.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 and
 * only version 2 as published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA
 * 02110-1301, USA.
 */

#include <linux/init.h>
#include <linux/sched.h>
=======
// SPDX-License-Identifier: GPL-2.0-only
/*
 * Kernel traps/events for Hexagon processor
 *
 * Copyright (c) 2010-2014, The Linux Foundation. All rights reserved.
 */

#include <linux/init.h>
#include <linux/sched/signal.h>
#include <linux/sched/debug.h>
#include <linux/sched/task_stack.h>
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#include <linux/module.h>
#include <linux/kallsyms.h>
#include <linux/kdebug.h>
#include <linux/syscalls.h>
#include <linux/signal.h>
<<<<<<< HEAD
#include <linux/tracehook.h>
=======
#include <linux/ptrace.h>
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#include <asm/traps.h>
#include <asm/vm_fault.h>
#include <asm/syscall.h>
#include <asm/registers.h>
#include <asm/unistd.h>
#include <asm/sections.h>
#ifdef CONFIG_KGDB
# include <linux/kgdb.h>
#endif

#define TRAP_SYSCALL	1
#define TRAP_DEBUG	0xdb

<<<<<<< HEAD
void __init trap_init(void)
{
}

=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#ifdef CONFIG_GENERIC_BUG
/* Maybe should resemble arch/sh/kernel/traps.c ?? */
int is_valid_bugaddr(unsigned long addr)
{
	return 1;
}
#endif /* CONFIG_GENERIC_BUG */

static const char *ex_name(int ex)
{
	switch (ex) {
	case HVM_GE_C_XPROT:
	case HVM_GE_C_XUSER:
		return "Execute protection fault";
	case HVM_GE_C_RPROT:
	case HVM_GE_C_RUSER:
		return "Read protection fault";
	case HVM_GE_C_WPROT:
	case HVM_GE_C_WUSER:
		return "Write protection fault";
	case HVM_GE_C_XMAL:
		return "Misaligned instruction";
<<<<<<< HEAD
=======
	case HVM_GE_C_WREG:
		return "Multiple writes to same register in packet";
	case HVM_GE_C_PCAL:
		return "Program counter values that are not properly aligned";
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	case HVM_GE_C_RMAL:
		return "Misaligned data load";
	case HVM_GE_C_WMAL:
		return "Misaligned data store";
	case HVM_GE_C_INVI:
	case HVM_GE_C_PRIVI:
		return "Illegal instruction";
	case HVM_GE_C_BUS:
		return "Precise bus error";
	case HVM_GE_C_CACHE:
		return "Cache error";

	case 0xdb:
		return "Debugger trap";

	default:
		return "Unrecognized exception";
	}
}

static void do_show_stack(struct task_struct *task, unsigned long *fp,
<<<<<<< HEAD
			  unsigned long ip)
=======
			  unsigned long ip, const char *loglvl)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
{
	int kstack_depth_to_print = 24;
	unsigned long offset, size;
	const char *name = NULL;
	unsigned long *newfp;
	unsigned long low, high;
	char tmpstr[128];
	char *modname;
	int i;

	if (task == NULL)
		task = current;

<<<<<<< HEAD
	printk(KERN_INFO "CPU#%d, %s/%d, Call Trace:\n",
	       raw_smp_processor_id(), task->comm,
	       task_pid_nr(task));
=======
	printk("%sCPU#%d, %s/%d, Call Trace:\n", loglvl, raw_smp_processor_id(),
		task->comm, task_pid_nr(task));
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	if (fp == NULL) {
		if (task == current) {
			asm("%0 = r30" : "=r" (fp));
		} else {
			fp = (unsigned long *)
			     ((struct hexagon_switch_stack *)
			     task->thread.switch_sp)->fp;
		}
	}

	if ((((unsigned long) fp) & 0x3) || ((unsigned long) fp < 0x1000)) {
<<<<<<< HEAD
		printk(KERN_INFO "-- Corrupt frame pointer %p\n", fp);
=======
		printk("%s-- Corrupt frame pointer %p\n", loglvl, fp);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		return;
	}

	/* Saved link reg is one word above FP */
	if (!ip)
		ip = *(fp+1);

	/* Expect kernel stack to be in-bounds */
	low = (unsigned long)task_stack_page(task);
	high = low + THREAD_SIZE - 8;
	low += sizeof(struct thread_info);

	for (i = 0; i < kstack_depth_to_print; i++) {

		name = kallsyms_lookup(ip, &size, &offset, &modname, tmpstr);

<<<<<<< HEAD
		printk(KERN_INFO "[%p] 0x%lx: %s + 0x%lx", fp, ip, name,
			offset);
=======
		printk("%s[%p] 0x%lx: %s + 0x%lx", loglvl, fp, ip, name, offset);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		if (((unsigned long) fp < low) || (high < (unsigned long) fp))
			printk(KERN_CONT " (FP out of bounds!)");
		if (modname)
			printk(KERN_CONT " [%s] ", modname);
		printk(KERN_CONT "\n");

		newfp = (unsigned long *) *fp;

		if (((unsigned long) newfp) & 0x3) {
<<<<<<< HEAD
			printk(KERN_INFO "-- Corrupt frame pointer %p\n",
				newfp);
=======
			printk("%s-- Corrupt frame pointer %p\n", loglvl, newfp);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
			break;
		}

		/* Attempt to continue past exception. */
		if (0 == newfp) {
			struct pt_regs *regs = (struct pt_regs *) (((void *)fp)
						+ 8);

			if (regs->syscall_nr != -1) {
<<<<<<< HEAD
				printk(KERN_INFO "-- trap0 -- syscall_nr: %ld",
=======
				printk("%s-- trap0 -- syscall_nr: %ld", loglvl,
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
					regs->syscall_nr);
				printk(KERN_CONT "  psp: %lx  elr: %lx\n",
					 pt_psp(regs), pt_elr(regs));
				break;
			} else {
				/* really want to see more ... */
				kstack_depth_to_print += 6;
<<<<<<< HEAD
				printk(KERN_INFO "-- %s (0x%lx)  badva: %lx\n",
=======
				printk("%s-- %s (0x%lx)  badva: %lx\n", loglvl,
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
					ex_name(pt_cause(regs)), pt_cause(regs),
					pt_badva(regs));
			}

			newfp = (unsigned long *) regs->r30;
			ip = pt_elr(regs);
		} else {
			ip = *(newfp + 1);
		}

		/* If link reg is null, we are done. */
		if (ip == 0x0)
			break;

		/* If newfp isn't larger, we're tracing garbage. */
		if (newfp > fp)
			fp = newfp;
		else
			break;
	}
}

<<<<<<< HEAD
void show_stack(struct task_struct *task, unsigned long *fp)
{
	/* Saved link reg is one word above FP */
	do_show_stack(task, fp, 0);
}

void dump_stack(void)
{
	unsigned long *fp;
	asm("%0 = r30" : "=r" (fp));
	show_stack(current, fp);
}
EXPORT_SYMBOL(dump_stack);

=======
void show_stack(struct task_struct *task, unsigned long *fp, const char *loglvl)
{
	/* Saved link reg is one word above FP */
	do_show_stack(task, fp, 0, loglvl);
}

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
int die(const char *str, struct pt_regs *regs, long err)
{
	static struct {
		spinlock_t lock;
		int counter;
	} die = {
		.lock = __SPIN_LOCK_UNLOCKED(die.lock),
		.counter = 0
	};

	console_verbose();
	oops_enter();

	spin_lock_irq(&die.lock);
	bust_spinlocks(1);
	printk(KERN_EMERG "Oops: %s[#%d]:\n", str, ++die.counter);

	if (notify_die(DIE_OOPS, str, regs, err, pt_cause(regs), SIGSEGV) ==
	    NOTIFY_STOP)
		return 1;

	print_modules();
	show_regs(regs);
<<<<<<< HEAD
	do_show_stack(current, &regs->r30, pt_elr(regs));

	bust_spinlocks(0);
	add_taint(TAINT_DIE);
=======
	do_show_stack(current, &regs->r30, pt_elr(regs), KERN_EMERG);

	bust_spinlocks(0);
	add_taint(TAINT_DIE, LOCKDEP_NOW_UNRELIABLE);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	spin_unlock_irq(&die.lock);

	if (in_interrupt())
		panic("Fatal exception in interrupt");

	if (panic_on_oops)
		panic("Fatal exception");

	oops_exit();
<<<<<<< HEAD
	do_exit(err);
=======
	make_task_dead(err);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	return 0;
}

int die_if_kernel(char *str, struct pt_regs *regs, long err)
{
	if (!user_mode(regs))
		return die(str, regs, err);
	else
		return 0;
}

/*
 * It's not clear that misaligned fetches are ever recoverable.
 */
static void misaligned_instruction(struct pt_regs *regs)
{
	die_if_kernel("Misaligned Instruction", regs, 0);
<<<<<<< HEAD
	force_sig(SIGBUS, current);
=======
	force_sig(SIGBUS);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

/*
 * Misaligned loads and stores, on the other hand, can be
 * emulated, and probably should be, some day.  But for now
 * they will be considered fatal.
 */
static void misaligned_data_load(struct pt_regs *regs)
{
	die_if_kernel("Misaligned Data Load", regs, 0);
<<<<<<< HEAD
	force_sig(SIGBUS, current);
=======
	force_sig(SIGBUS);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

static void misaligned_data_store(struct pt_regs *regs)
{
	die_if_kernel("Misaligned Data Store", regs, 0);
<<<<<<< HEAD
	force_sig(SIGBUS, current);
=======
	force_sig(SIGBUS);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

static void illegal_instruction(struct pt_regs *regs)
{
	die_if_kernel("Illegal Instruction", regs, 0);
<<<<<<< HEAD
	force_sig(SIGILL, current);
=======
	force_sig(SIGILL);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

/*
 * Precise bus errors may be recoverable with a a retry,
 * but for now, treat them as irrecoverable.
 */
static void precise_bus_error(struct pt_regs *regs)
{
	die_if_kernel("Precise Bus Error", regs, 0);
<<<<<<< HEAD
	force_sig(SIGBUS, current);
=======
	force_sig(SIGBUS);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

/*
 * If anything is to be done here other than panic,
 * it will probably be complex and migrate to another
 * source module.  For now, just die.
 */
static void cache_error(struct pt_regs *regs)
{
	die("Cache Error", regs, 0);
}

/*
 * General exception handler
 */
<<<<<<< HEAD
=======
void do_genex(struct pt_regs *regs);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
void do_genex(struct pt_regs *regs)
{
	/*
	 * Decode Cause and Dispatch
	 */
	switch (pt_cause(regs)) {
	case HVM_GE_C_XPROT:
	case HVM_GE_C_XUSER:
		execute_protection_fault(regs);
		break;
	case HVM_GE_C_RPROT:
	case HVM_GE_C_RUSER:
		read_protection_fault(regs);
		break;
	case HVM_GE_C_WPROT:
	case HVM_GE_C_WUSER:
		write_protection_fault(regs);
		break;
	case HVM_GE_C_XMAL:
		misaligned_instruction(regs);
		break;
<<<<<<< HEAD
=======
	case HVM_GE_C_WREG:
		illegal_instruction(regs);
		break;
	case HVM_GE_C_PCAL:
		misaligned_instruction(regs);
		break;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	case HVM_GE_C_RMAL:
		misaligned_data_load(regs);
		break;
	case HVM_GE_C_WMAL:
		misaligned_data_store(regs);
		break;
	case HVM_GE_C_INVI:
	case HVM_GE_C_PRIVI:
		illegal_instruction(regs);
		break;
	case HVM_GE_C_BUS:
		precise_bus_error(regs);
		break;
	case HVM_GE_C_CACHE:
		cache_error(regs);
		break;
	default:
		/* Halt and catch fire */
		panic("Unrecognized exception 0x%lx\n", pt_cause(regs));
		break;
	}
}

<<<<<<< HEAD
/* Indirect system call dispatch */
long sys_syscall(void)
{
	printk(KERN_ERR "sys_syscall invoked!\n");
	return -ENOSYS;
}

void do_trap0(struct pt_regs *regs)
{
	unsigned long syscallret = 0;
=======
void do_trap0(struct pt_regs *regs);
void do_trap0(struct pt_regs *regs)
{
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	syscall_fn syscall;

	switch (pt_cause(regs)) {
	case TRAP_SYSCALL:
		/* System call is trap0 #1 */

		/* allow strace to catch syscall args  */
		if (unlikely(test_thread_flag(TIF_SYSCALL_TRACE) &&
<<<<<<< HEAD
			tracehook_report_syscall_entry(regs)))
=======
			ptrace_report_syscall_entry(regs)))
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
			return;  /*  return -ENOSYS somewhere?  */

		/* Interrupts should be re-enabled for syscall processing */
		__vmsetie(VM_INT_ENABLE);

		/*
		 * System call number is in r6, arguments in r0..r5.
		 * Fortunately, no Linux syscall has more than 6 arguments,
		 * and Hexagon ABI passes first 6 arguments in registers.
		 * 64-bit arguments are passed in odd/even register pairs.
		 * Fortunately, we have no system calls that take more
		 * than three arguments with more than one 64-bit value.
		 * Should that change, we'd need to redesign to copy
		 * between user and kernel stacks.
		 */
		regs->syscall_nr = regs->r06;

		/*
		 * GPR R0 carries the first parameter, and is also used
		 * to report the return value.  We need a backup of
		 * the user's value in case we need to do a late restart
		 * of the system call.
		 */
		regs->restart_r0 = regs->r00;

		if ((unsigned long) regs->syscall_nr >= __NR_syscalls) {
			regs->r00 = -1;
		} else {
			syscall = (syscall_fn)
				  (sys_call_table[regs->syscall_nr]);
<<<<<<< HEAD
			syscallret = syscall(regs->r00, regs->r01,
=======
			regs->r00 = syscall(regs->r00, regs->r01,
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
				   regs->r02, regs->r03,
				   regs->r04, regs->r05);
		}

<<<<<<< HEAD
		/*
		 * If it was a sigreturn system call, don't overwrite
		 * r0 value in stack frame with return value.
		 *
		 * __NR_sigreturn doesn't seem to exist in new unistd.h
		 */

		if (regs->syscall_nr != __NR_rt_sigreturn)
			regs->r00 = syscallret;

		/* allow strace to get the syscall return state  */
		if (unlikely(test_thread_flag(TIF_SYSCALL_TRACE)))
			tracehook_report_syscall_exit(regs, 0);
=======
		/* allow strace to get the syscall return state  */
		if (unlikely(test_thread_flag(TIF_SYSCALL_TRACE)))
			ptrace_report_syscall_exit(regs, 0);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

		break;
	case TRAP_DEBUG:
		/* Trap0 0xdb is debug breakpoint */
		if (user_mode(regs)) {
<<<<<<< HEAD
			struct siginfo info;

			info.si_signo = SIGTRAP;
			info.si_errno = 0;
=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
			/*
			 * Some architecures add some per-thread state
			 * to distinguish between breakpoint traps and
			 * trace traps.  We may want to do that, and
			 * set the si_code value appropriately, or we
			 * may want to use a different trap0 flavor.
			 */
<<<<<<< HEAD
			info.si_code = TRAP_BRKPT;
			info.si_addr = (void __user *) pt_elr(regs);
			send_sig_info(SIGTRAP, &info, current);
=======
			force_sig_fault(SIGTRAP, TRAP_BRKPT,
					(void __user *) pt_elr(regs));
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		} else {
#ifdef CONFIG_KGDB
			kgdb_handle_exception(pt_cause(regs), SIGTRAP,
					      TRAP_BRKPT, regs);
#endif
		}
		break;
	}
	/* Ignore other trap0 codes for now, especially 0 (Angel calls) */
}

/*
 * Machine check exception handler
 */
<<<<<<< HEAD
=======
void do_machcheck(struct pt_regs *regs);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
void do_machcheck(struct pt_regs *regs)
{
	/* Halt and catch fire */
	__vmstop();
}
<<<<<<< HEAD
=======

/*
 * Treat this like the old 0xdb trap.
 */

void do_debug_exception(struct pt_regs *regs);
void do_debug_exception(struct pt_regs *regs)
{
	regs->hvmer.vmest &= ~HVM_VMEST_CAUSE_MSK;
	regs->hvmer.vmest |= (TRAP_DEBUG << HVM_VMEST_CAUSE_SFT);
	do_trap0(regs);
}
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
