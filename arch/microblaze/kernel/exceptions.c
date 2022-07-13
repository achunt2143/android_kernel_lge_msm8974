/*
 * HW exception handling
 *
 * Copyright (C) 2008-2009 Michal Simek <monstr@monstr.eu>
 * Copyright (C) 2008 PetaLogix
 *
 * This file is subject to the terms and conditions of the GNU General
 * Public License.  See the file COPYING in the main directory of this
 * archive for more details.
 */

/*
 * This file handles the architecture-dependent parts of hardware exceptions
 */

<<<<<<< HEAD
#include <linux/kernel.h>
#include <linux/signal.h>
#include <linux/sched.h>
#include <linux/kallsyms.h>
#include <linux/module.h>
=======
#include <linux/export.h>
#include <linux/kernel.h>
#include <linux/signal.h>
#include <linux/sched.h>
#include <linux/sched/debug.h>
#include <linux/kallsyms.h>
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

#include <asm/exceptions.h>
#include <asm/entry.h>		/* For KM CPU var */
#include <linux/uaccess.h>
#include <linux/errno.h>
#include <linux/ptrace.h>
#include <asm/current.h>
#include <asm/cacheflush.h>

#define MICROBLAZE_ILL_OPCODE_EXCEPTION	0x02
#define MICROBLAZE_IBUS_EXCEPTION	0x03
#define MICROBLAZE_DBUS_EXCEPTION	0x04
#define MICROBLAZE_DIV_ZERO_EXCEPTION	0x05
#define MICROBLAZE_FPU_EXCEPTION	0x06
#define MICROBLAZE_PRIVILEGED_EXCEPTION	0x07

static DEFINE_SPINLOCK(die_lock);

void die(const char *str, struct pt_regs *fp, long err)
{
	console_verbose();
	spin_lock_irq(&die_lock);
<<<<<<< HEAD
	printk(KERN_WARNING "Oops: %s, sig: %ld\n", str, err);
	show_regs(fp);
	spin_unlock_irq(&die_lock);
	/* do_exit() should take care of panic'ing from an interrupt
	 * context so we don't handle it here
	 */
	do_exit(err);
=======
	pr_warn("Oops: %s, sig: %ld\n", str, err);
	show_regs(fp);
	spin_unlock_irq(&die_lock);
	/* make_task_dead() should take care of panic'ing from an interrupt
	 * context so we don't handle it here
	 */
	make_task_dead(err);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

/* for user application debugging */
asmlinkage void sw_exception(struct pt_regs *regs)
{
	_exception(SIGTRAP, regs, TRAP_BRKPT, regs->r16);
	flush_dcache_range(regs->r16, regs->r16 + 0x4);
	flush_icache_range(regs->r16, regs->r16 + 0x4);
}

void _exception(int signr, struct pt_regs *regs, int code, unsigned long addr)
{
<<<<<<< HEAD
	siginfo_t info;

	if (kernel_mode(regs)) {
		die("Exception in kernel mode", regs, signr);
	}
	info.si_signo = signr;
	info.si_errno = 0;
	info.si_code = code;
	info.si_addr = (void __user *) addr;
	force_sig_info(signr, &info, current);
=======
	if (kernel_mode(regs))
		die("Exception in kernel mode", regs, signr);

	force_sig_fault(signr, code, (void __user *)addr);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

asmlinkage void full_exception(struct pt_regs *regs, unsigned int type,
							int fsr, int addr)
{
<<<<<<< HEAD
#ifdef CONFIG_MMU
	addr = regs->pc;
#endif

#if 0
	printk(KERN_WARNING "Exception %02x in %s mode, FSR=%08x PC=%08x " \
							"ESR=%08x\n",
=======
	addr = regs->pc;

#if 0
	pr_warn("Exception %02x in %s mode, FSR=%08x PC=%08x ESR=%08x\n",
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
			type, user_mode(regs) ? "user" : "kernel", fsr,
			(unsigned int) regs->pc, (unsigned int) regs->esr);
#endif

	switch (type & 0x1F) {
	case MICROBLAZE_ILL_OPCODE_EXCEPTION:
		if (user_mode(regs)) {
			pr_debug("Illegal opcode exception in user mode\n");
			_exception(SIGILL, regs, ILL_ILLOPC, addr);
			return;
		}
<<<<<<< HEAD
		printk(KERN_WARNING "Illegal opcode exception " \
							"in kernel mode.\n");
=======
		pr_warn("Illegal opcode exception in kernel mode.\n");
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		die("opcode exception", regs, SIGBUS);
		break;
	case MICROBLAZE_IBUS_EXCEPTION:
		if (user_mode(regs)) {
			pr_debug("Instruction bus error exception in user mode\n");
			_exception(SIGBUS, regs, BUS_ADRERR, addr);
			return;
		}
<<<<<<< HEAD
		printk(KERN_WARNING "Instruction bus error exception " \
							"in kernel mode.\n");
=======
		pr_warn("Instruction bus error exception in kernel mode.\n");
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		die("bus exception", regs, SIGBUS);
		break;
	case MICROBLAZE_DBUS_EXCEPTION:
		if (user_mode(regs)) {
			pr_debug("Data bus error exception in user mode\n");
			_exception(SIGBUS, regs, BUS_ADRERR, addr);
			return;
		}
<<<<<<< HEAD
		printk(KERN_WARNING "Data bus error exception " \
							"in kernel mode.\n");
=======
		pr_warn("Data bus error exception in kernel mode.\n");
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		die("bus exception", regs, SIGBUS);
		break;
	case MICROBLAZE_DIV_ZERO_EXCEPTION:
		if (user_mode(regs)) {
			pr_debug("Divide by zero exception in user mode\n");
			_exception(SIGFPE, regs, FPE_INTDIV, addr);
			return;
		}
<<<<<<< HEAD
		printk(KERN_WARNING "Divide by zero exception " \
							"in kernel mode.\n");
=======
		pr_warn("Divide by zero exception in kernel mode.\n");
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		die("Divide by zero exception", regs, SIGBUS);
		break;
	case MICROBLAZE_FPU_EXCEPTION:
		pr_debug("FPU exception\n");
		/* IEEE FP exception */
		/* I removed fsr variable and use code var for storing fsr */
		if (fsr & FSR_IO)
			fsr = FPE_FLTINV;
		else if (fsr & FSR_OF)
			fsr = FPE_FLTOVF;
		else if (fsr & FSR_UF)
			fsr = FPE_FLTUND;
		else if (fsr & FSR_DZ)
			fsr = FPE_FLTDIV;
		else if (fsr & FSR_DO)
			fsr = FPE_FLTRES;
		_exception(SIGFPE, regs, fsr, addr);
		break;
<<<<<<< HEAD

#ifdef CONFIG_MMU
=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	case MICROBLAZE_PRIVILEGED_EXCEPTION:
		pr_debug("Privileged exception\n");
		_exception(SIGILL, regs, ILL_PRVOPC, addr);
		break;
<<<<<<< HEAD
#endif
	default:
	/* FIXME what to do in unexpected exception */
		printk(KERN_WARNING "Unexpected exception %02x "
			"PC=%08x in %s mode\n", type, (unsigned int) addr,
=======
	default:
	/* FIXME what to do in unexpected exception */
		pr_warn("Unexpected exception %02x PC=%08x in %s mode\n",
			type, (unsigned int) addr,
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
			kernel_mode(regs) ? "kernel" : "user");
	}
	return;
}
