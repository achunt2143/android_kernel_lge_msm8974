<<<<<<< HEAD
=======
/* SPDX-License-Identifier: GPL-2.0-only */
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
/*
 * Syscall support for the Hexagon architecture
 *
 * Copyright (c) 2010-2011, The Linux Foundation. All rights reserved.
<<<<<<< HEAD
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
=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 */

#ifndef _ASM_HEXAGON_SYSCALL_H
#define _ASM_HEXAGON_SYSCALL_H

<<<<<<< HEAD
=======
#include <uapi/linux/audit.h>
#include <linux/err.h>
#include <asm/ptrace.h>

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
typedef long (*syscall_fn)(unsigned long, unsigned long,
	unsigned long, unsigned long,
	unsigned long, unsigned long);

<<<<<<< HEAD
asmlinkage int sys_execve(char __user *ufilename, char __user * __user *argv,
			  char __user * __user *envp);
asmlinkage int sys_clone(unsigned long clone_flags, unsigned long newsp,
			 unsigned long parent_tidp, unsigned long child_tidp);

#define sys_execve	sys_execve
#define sys_clone	sys_clone

=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#include <asm-generic/syscalls.h>

extern void *sys_call_table[];

static inline long syscall_get_nr(struct task_struct *task,
				  struct pt_regs *regs)
{
	return regs->r06;
}

static inline void syscall_get_arguments(struct task_struct *task,
					 struct pt_regs *regs,
<<<<<<< HEAD
					 unsigned int i, unsigned int n,
					 unsigned long *args)
{
	BUG_ON(i + n > 6);
	memcpy(args, &(&regs->r00)[i], n * sizeof(args[0]));
}
=======
					 unsigned long *args)
{
	memcpy(args, &(&regs->r00)[0], 6 * sizeof(args[0]));
}

static inline long syscall_get_error(struct task_struct *task,
				     struct pt_regs *regs)
{
	return IS_ERR_VALUE(regs->r00) ? regs->r00 : 0;
}

static inline long syscall_get_return_value(struct task_struct *task,
					    struct pt_regs *regs)
{
	return regs->r00;
}

static inline int syscall_get_arch(struct task_struct *task)
{
	return AUDIT_ARCH_HEXAGON;
}

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#endif
