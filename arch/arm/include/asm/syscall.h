<<<<<<< HEAD
/*
 * Access to user system call parameters and results
 *
 * Copyright (C) 2012 The Chromium OS Authors <chromium-os-dev@chromium.org>
 *
 * This copyrighted material is made available to anyone wishing to use,
 * modify, copy, or redistribute it subject to the terms and conditions
 * of the GNU General Public License v.2.
 *
 * See asm-generic/syscall.h for descriptions of what we must do here.
 */
#ifndef _ASM_ARM_SYSCALL_H
#define _ASM_ARM_SYSCALL_H

#include <linux/audit.h> /* for AUDIT_ARCH_* */
#include <linux/elf.h> /* for ELF_EM */
#include <linux/sched.h>
#include <linux/thread_info.h> /* for task_thread_info */
#include <linux/err.h>

static inline int syscall_get_nr(struct task_struct *task, struct pt_regs *regs)
{
	return task_thread_info(task)->syscall;
=======
/* SPDX-License-Identifier: GPL-2.0 */
/*
 * Access to user system call parameters and results
 *
 * See asm-generic/syscall.h for descriptions of what we must do here.
 */

#ifndef _ASM_ARM_SYSCALL_H
#define _ASM_ARM_SYSCALL_H

#include <uapi/linux/audit.h> /* for AUDIT_ARCH_* */
#include <linux/elf.h> /* for ELF_EM */
#include <linux/err.h>
#include <linux/sched.h>

#include <asm/unistd.h>

#define NR_syscalls (__NR_syscalls)

extern const unsigned long sys_call_table[];

static inline int syscall_get_nr(struct task_struct *task,
				 struct pt_regs *regs)
{
	if (IS_ENABLED(CONFIG_AEABI) && !IS_ENABLED(CONFIG_OABI_COMPAT))
		return task_thread_info(task)->abi_syscall;

	if (task_thread_info(task)->abi_syscall == -1)
		return -1;

	return task_thread_info(task)->abi_syscall & __NR_SYSCALL_MASK;
}

static inline bool __in_oabi_syscall(struct task_struct *task)
{
	return IS_ENABLED(CONFIG_OABI_COMPAT) &&
		(task_thread_info(task)->abi_syscall & __NR_OABI_SYSCALL_BASE);
}

static inline bool in_oabi_syscall(void)
{
	return __in_oabi_syscall(current);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

static inline void syscall_rollback(struct task_struct *task,
				    struct pt_regs *regs)
{
	regs->ARM_r0 = regs->ARM_ORIG_r0;
}

static inline long syscall_get_error(struct task_struct *task,
				     struct pt_regs *regs)
{
	unsigned long error = regs->ARM_r0;
	return IS_ERR_VALUE(error) ? error : 0;
}

static inline long syscall_get_return_value(struct task_struct *task,
					    struct pt_regs *regs)
{
	return regs->ARM_r0;
}

static inline void syscall_set_return_value(struct task_struct *task,
					    struct pt_regs *regs,
					    int error, long val)
{
<<<<<<< HEAD
	regs->ARM_r0 = (long) error ?: val;
}

static inline void syscall_get_arguments(struct task_struct *task,
					 struct pt_regs *regs,
					 unsigned int i, unsigned int n,
					 unsigned long *args)
{
	BUG_ON(i + n > 6);
	memcpy(args, &regs->ARM_r0 + i, n * sizeof(args[0]));
}

static inline void syscall_set_arguments(struct task_struct *task,
					 struct pt_regs *regs,
					 unsigned int i, unsigned int n,
					 const unsigned long *args)
{
	BUG_ON(i + n > 6);
	memcpy(&regs->ARM_r0 + i, args, n * sizeof(args[0]));
}

static inline int syscall_get_arch(struct task_struct *task,
				    struct pt_regs *regs)
{
	/* ARM tasks don't change audit architectures on the fly. */
#ifdef __ARMEB__
	return AUDIT_ARCH_ARMEB;
#else
	return AUDIT_ARCH_ARM;
#endif
}
#endif	/* _ASM_ARM_SYSCALL_H */
=======
	regs->ARM_r0 = (long) error ? error : val;
}

#define SYSCALL_MAX_ARGS 7

static inline void syscall_get_arguments(struct task_struct *task,
					 struct pt_regs *regs,
					 unsigned long *args)
{
	args[0] = regs->ARM_ORIG_r0;
	args++;

	memcpy(args, &regs->ARM_r0 + 1, 5 * sizeof(args[0]));
}

static inline int syscall_get_arch(struct task_struct *task)
{
	/* ARM tasks don't change audit architectures on the fly. */
	return AUDIT_ARCH_ARM;
}

#endif /* _ASM_ARM_SYSCALL_H */
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
