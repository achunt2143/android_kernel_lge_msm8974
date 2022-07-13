<<<<<<< HEAD
=======
/* SPDX-License-Identifier: GPL-2.0-only */
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
/*
 * Access to user system call parameters and results
 *
 * Copyright (C) 2008 Red Hat, Inc.  All rights reserved.
 *
<<<<<<< HEAD
 * This copyrighted material is made available to anyone wishing to use,
 * modify, copy, or redistribute it subject to the terms and conditions
 * of the GNU General Public License v.2.
 *
=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 * See asm-generic/syscall.h for descriptions of what we must do here.
 */

#ifndef _ASM_SYSCALL_H
#define _ASM_SYSCALL_H	1

<<<<<<< HEAD
#include <linux/sched.h>

/* ftrace syscalls requires exporting the sys_call_table */
#ifdef CONFIG_FTRACE_SYSCALLS
extern const unsigned long *sys_call_table;
#endif /* CONFIG_FTRACE_SYSCALLS */

static inline long syscall_get_nr(struct task_struct *task,
				  struct pt_regs *regs)
{
	return TRAP(regs) == 0xc00 ? regs->gpr[0] : -1L;
=======
#include <uapi/linux/audit.h>
#include <linux/sched.h>
#include <linux/thread_info.h>

#ifdef CONFIG_ARCH_HAS_SYSCALL_WRAPPER
typedef long (*syscall_fn)(const struct pt_regs *);
#else
typedef long (*syscall_fn)(unsigned long, unsigned long, unsigned long,
			   unsigned long, unsigned long, unsigned long);
#endif

/* ftrace syscalls requires exporting the sys_call_table */
extern const syscall_fn sys_call_table[];
extern const syscall_fn compat_sys_call_table[];

static inline int syscall_get_nr(struct task_struct *task, struct pt_regs *regs)
{
	/*
	 * Note that we are returning an int here. That means 0xffffffff, ie.
	 * 32-bit negative 1, will be interpreted as -1 on a 64-bit kernel.
	 * This is important for seccomp so that compat tasks can set r0 = -1
	 * to reject the syscall.
	 */
	if (trap_is_syscall(regs))
		return regs->gpr[0];
	else
		return -1;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

static inline void syscall_rollback(struct task_struct *task,
				    struct pt_regs *regs)
{
	regs->gpr[3] = regs->orig_gpr3;
}

static inline long syscall_get_error(struct task_struct *task,
				     struct pt_regs *regs)
{
<<<<<<< HEAD
	return (regs->ccr & 0x10000000) ? -regs->gpr[3] : 0;
=======
	if (trap_is_scv(regs)) {
		unsigned long error = regs->gpr[3];

		return IS_ERR_VALUE(error) ? error : 0;
	} else {
		/*
		 * If the system call failed,
		 * regs->gpr[3] contains a positive ERRORCODE.
		 */
		return (regs->ccr & 0x10000000UL) ? -regs->gpr[3] : 0;
	}
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

static inline long syscall_get_return_value(struct task_struct *task,
					    struct pt_regs *regs)
{
	return regs->gpr[3];
}

static inline void syscall_set_return_value(struct task_struct *task,
					    struct pt_regs *regs,
					    int error, long val)
{
<<<<<<< HEAD
	if (error) {
		regs->ccr |= 0x10000000L;
		regs->gpr[3] = -error;
	} else {
		regs->ccr &= ~0x10000000L;
		regs->gpr[3] = val;
=======
	if (trap_is_scv(regs)) {
		regs->gpr[3] = (long) error ?: val;
	} else {
		/*
		 * In the general case it's not obvious that we must deal with
		 * CCR here, as the syscall exit path will also do that for us.
		 * However there are some places, eg. the signal code, which
		 * check ccr to decide if the value in r3 is actually an error.
		 */
		if (error) {
			regs->ccr |= 0x10000000L;
			regs->gpr[3] = error;
		} else {
			regs->ccr &= ~0x10000000L;
			regs->gpr[3] = val;
		}
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	}
}

static inline void syscall_get_arguments(struct task_struct *task,
					 struct pt_regs *regs,
<<<<<<< HEAD
					 unsigned int i, unsigned int n,
					 unsigned long *args)
{
	BUG_ON(i + n > 6);
#ifdef CONFIG_PPC64
	if (test_tsk_thread_flag(task, TIF_32BIT)) {
		/*
		 * Zero-extend 32-bit argument values.  The high bits are
		 * garbage ignored by the actual syscall dispatch.
		 */
		while (n-- > 0)
			args[n] = (u32) regs->gpr[3 + i + n];
		return;
	}
#endif
	memcpy(args, &regs->gpr[3 + i], n * sizeof(args[0]));
}

static inline void syscall_set_arguments(struct task_struct *task,
					 struct pt_regs *regs,
					 unsigned int i, unsigned int n,
					 const unsigned long *args)
{
	BUG_ON(i + n > 6);
	memcpy(&regs->gpr[3 + i], args, n * sizeof(args[0]));
}

=======
					 unsigned long *args)
{
	unsigned long val, mask = -1UL;
	unsigned int n = 6;

	if (is_tsk_32bit_task(task))
		mask = 0xffffffff;

	while (n--) {
		if (n == 0)
			val = regs->orig_gpr3;
		else
			val = regs->gpr[3 + n];

		args[n] = val & mask;
	}
}

static inline int syscall_get_arch(struct task_struct *task)
{
	if (is_tsk_32bit_task(task))
		return AUDIT_ARCH_PPC;
	else if (IS_ENABLED(CONFIG_CPU_LITTLE_ENDIAN))
		return AUDIT_ARCH_PPC64LE;
	else
		return AUDIT_ARCH_PPC64;
}
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#endif	/* _ASM_SYSCALL_H */
