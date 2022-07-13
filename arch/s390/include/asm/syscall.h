<<<<<<< HEAD
=======
/* SPDX-License-Identifier: GPL-2.0 */
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
/*
 * Access to user system call parameters and results
 *
 *  Copyright IBM Corp. 2008
 *  Author(s): Martin Schwidefsky (schwidefsky@de.ibm.com)
<<<<<<< HEAD
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License (version 2 only)
 * as published by the Free Software Foundation.
=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 */

#ifndef _ASM_SYSCALL_H
#define _ASM_SYSCALL_H	1

<<<<<<< HEAD
=======
#include <uapi/linux/audit.h>
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#include <linux/sched.h>
#include <linux/err.h>
#include <asm/ptrace.h>

<<<<<<< HEAD
/*
 * The syscall table always contains 32 bit pointers since we know that the
 * address of the function to be called is (way) below 4GB.  So the "int"
 * type here is what we want [need] for both 32 bit and 64 bit systems.
 */
extern const unsigned int sys_call_table[];
=======
extern const sys_call_ptr_t sys_call_table[];
extern const sys_call_ptr_t sys_call_table_emu[];
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

static inline long syscall_get_nr(struct task_struct *task,
				  struct pt_regs *regs)
{
<<<<<<< HEAD
	return test_tsk_thread_flag(task, TIF_SYSCALL) ?
=======
	return test_pt_regs_flag(regs, PIF_SYSCALL) ?
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		(regs->int_code & 0xffff) : -1;
}

static inline void syscall_rollback(struct task_struct *task,
				    struct pt_regs *regs)
{
	regs->gprs[2] = regs->orig_gpr2;
}

static inline long syscall_get_error(struct task_struct *task,
				     struct pt_regs *regs)
{
<<<<<<< HEAD
	return IS_ERR_VALUE(regs->gprs[2]) ? regs->gprs[2] : 0;
=======
	unsigned long error = regs->gprs[2];
#ifdef CONFIG_COMPAT
	if (test_tsk_thread_flag(task, TIF_31BIT)) {
		/*
		 * Sign-extend the value so (int)-EFOO becomes (long)-EFOO
		 * and will match correctly in comparisons.
		 */
		error = (long)(int)error;
	}
#endif
	return IS_ERR_VALUE(error) ? error : 0;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

static inline long syscall_get_return_value(struct task_struct *task,
					    struct pt_regs *regs)
{
	return regs->gprs[2];
}

static inline void syscall_set_return_value(struct task_struct *task,
					    struct pt_regs *regs,
					    int error, long val)
{
<<<<<<< HEAD
	regs->gprs[2] = error ? -error : val;
=======
	set_pt_regs_flag(regs, PIF_SYSCALL_RET_SET);
	regs->gprs[2] = error ? error : val;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

static inline void syscall_get_arguments(struct task_struct *task,
					 struct pt_regs *regs,
<<<<<<< HEAD
					 unsigned int i, unsigned int n,
					 unsigned long *args)
{
	unsigned long mask = -1UL;

	BUG_ON(i + n > 6);
=======
					 unsigned long *args)
{
	unsigned long mask = -1UL;
	unsigned int n = 6;

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#ifdef CONFIG_COMPAT
	if (test_tsk_thread_flag(task, TIF_31BIT))
		mask = 0xffffffff;
#endif
	while (n-- > 0)
<<<<<<< HEAD
		if (i + n > 0)
			args[n] = regs->gprs[2 + i + n] & mask;
	if (i == 0)
		args[0] = regs->orig_gpr2 & mask;
}

static inline void syscall_set_arguments(struct task_struct *task,
					 struct pt_regs *regs,
					 unsigned int i, unsigned int n,
					 const unsigned long *args)
{
	BUG_ON(i + n > 6);
	while (n-- > 0)
		if (i + n > 0)
			regs->gprs[2 + i + n] = args[n];
	if (i == 0)
		regs->orig_gpr2 = args[0];
}

=======
		if (n > 0)
			args[n] = regs->gprs[2 + n] & mask;

	args[0] = regs->orig_gpr2 & mask;
}

static inline int syscall_get_arch(struct task_struct *task)
{
#ifdef CONFIG_COMPAT
	if (test_tsk_thread_flag(task, TIF_31BIT))
		return AUDIT_ARCH_S390;
#endif
	return AUDIT_ARCH_S390X;
}

static inline bool arch_syscall_is_vdso_sigreturn(struct pt_regs *regs)
{
	return false;
}

#define SYSCALL_FMT_0
#define SYSCALL_FMT_1 , "0" (r2)
#define SYSCALL_FMT_2 , "d" (r3) SYSCALL_FMT_1
#define SYSCALL_FMT_3 , "d" (r4) SYSCALL_FMT_2
#define SYSCALL_FMT_4 , "d" (r5) SYSCALL_FMT_3
#define SYSCALL_FMT_5 , "d" (r6) SYSCALL_FMT_4
#define SYSCALL_FMT_6 , "d" (r7) SYSCALL_FMT_5

#define SYSCALL_PARM_0
#define SYSCALL_PARM_1 , long arg1
#define SYSCALL_PARM_2 SYSCALL_PARM_1, long arg2
#define SYSCALL_PARM_3 SYSCALL_PARM_2, long arg3
#define SYSCALL_PARM_4 SYSCALL_PARM_3, long arg4
#define SYSCALL_PARM_5 SYSCALL_PARM_4, long arg5
#define SYSCALL_PARM_6 SYSCALL_PARM_5, long arg6

#define SYSCALL_REGS_0
#define SYSCALL_REGS_1							\
	register long r2 asm("2") = arg1
#define SYSCALL_REGS_2							\
	SYSCALL_REGS_1;							\
	register long r3 asm("3") = arg2
#define SYSCALL_REGS_3							\
	SYSCALL_REGS_2;							\
	register long r4 asm("4") = arg3
#define SYSCALL_REGS_4							\
	SYSCALL_REGS_3;							\
	register long r5 asm("5") = arg4
#define SYSCALL_REGS_5							\
	SYSCALL_REGS_4;							\
	register long r6 asm("6") = arg5
#define SYSCALL_REGS_6							\
	SYSCALL_REGS_5;							\
	register long r7 asm("7") = arg6

#define GENERATE_SYSCALL_FUNC(nr)					\
static __always_inline							\
long syscall##nr(unsigned long syscall SYSCALL_PARM_##nr)		\
{									\
	register unsigned long r1 asm ("1") = syscall;			\
	register long rc asm ("2");					\
	SYSCALL_REGS_##nr;						\
									\
	asm volatile (							\
		"	svc	0\n"					\
		: "=d" (rc)						\
		: "d" (r1) SYSCALL_FMT_##nr				\
		: "memory");						\
	return rc;							\
}

GENERATE_SYSCALL_FUNC(0)
GENERATE_SYSCALL_FUNC(1)
GENERATE_SYSCALL_FUNC(2)
GENERATE_SYSCALL_FUNC(3)
GENERATE_SYSCALL_FUNC(4)
GENERATE_SYSCALL_FUNC(5)
GENERATE_SYSCALL_FUNC(6)

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#endif	/* _ASM_SYSCALL_H */
