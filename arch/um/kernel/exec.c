<<<<<<< HEAD
/*
 * Copyright (C) 2000 - 2007 Jeff Dike (jdike@{addtoit,linux.intel}.com)
 * Licensed under the GPL
=======
// SPDX-License-Identifier: GPL-2.0
/*
 * Copyright (C) 2000 - 2007 Jeff Dike (jdike@{addtoit,linux.intel}.com)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 */

#include <linux/stddef.h>
#include <linux/module.h>
#include <linux/fs.h>
#include <linux/ptrace.h>
<<<<<<< HEAD
#include <linux/sched.h>
#include <linux/slab.h>
#include <asm/current.h>
#include <asm/processor.h>
#include <asm/uaccess.h>
#include "as-layout.h"
#include "mem_user.h"
#include "skas.h"
#include "os.h"
#include "internal.h"
=======
#include <linux/sched/mm.h>
#include <linux/sched/task.h>
#include <linux/sched/task_stack.h>
#include <linux/slab.h>
#include <asm/current.h>
#include <asm/processor.h>
#include <linux/uaccess.h>
#include <as-layout.h>
#include <mem_user.h>
#include <registers.h>
#include <skas.h>
#include <os.h>
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

void flush_thread(void)
{
	void *data = NULL;
	int ret;

	arch_flush_thread(&current->thread.arch);

<<<<<<< HEAD
	ret = unmap(&current->mm->context.id, 0, STUB_START, 0, &data);
	ret = ret || unmap(&current->mm->context.id, STUB_END,
			   host_task_size - STUB_END, 1, &data);
	if (ret) {
		printk(KERN_ERR "flush_thread - clearing address space failed, "
		       "err = %d\n", ret);
		force_sig(SIGKILL, current);
	}
=======
	ret = unmap(&current->mm->context.id, 0, TASK_SIZE, 1, &data);
	if (ret) {
		printk(KERN_ERR "%s - clearing address space failed, err = %d\n",
		       __func__, ret);
		force_sig(SIGKILL);
	}
	get_safe_registers(current_pt_regs()->regs.gp,
			   current_pt_regs()->regs.fp);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	__switch_mm(&current->mm->context.id);
}

void start_thread(struct pt_regs *regs, unsigned long eip, unsigned long esp)
{
	PT_REGS_IP(regs) = eip;
	PT_REGS_SP(regs) = esp;
<<<<<<< HEAD
}
EXPORT_SYMBOL(start_thread);

static long execve1(const char *file,
		    const char __user *const __user *argv,
		    const char __user *const __user *env)
{
	long error;

	error = do_execve(file, argv, env, &current->thread.regs);
	if (error == 0) {
		task_lock(current);
		current->ptrace &= ~PT_DTRACE;
#ifdef SUBARCH_EXECVE1
		SUBARCH_EXECVE1(&current->thread.regs.regs);
#endif
		task_unlock(current);
	}
	return error;
}

long um_execve(const char *file, const char __user *const __user *argv, const char __user *const __user *env)
{
	long err;

	err = execve1(file, argv, env);
	if (!err)
		UML_LONGJMP(current->thread.exec_buf, 1);
	return err;
}

long sys_execve(const char __user *file, const char __user *const __user *argv,
		const char __user *const __user *env)
{
	long error;
	char *filename;

	filename = getname(file);
	error = PTR_ERR(filename);
	if (IS_ERR(filename)) goto out;
	error = execve1(filename, argv, env);
	putname(filename);
 out:
	return error;
}
=======
	clear_thread_flag(TIF_SINGLESTEP);
#ifdef SUBARCH_EXECVE1
	SUBARCH_EXECVE1(regs->regs);
#endif
}
EXPORT_SYMBOL(start_thread);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
