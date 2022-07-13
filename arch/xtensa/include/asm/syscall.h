/*
<<<<<<< HEAD
 * include/asm-xtensa/syscall.h
 *
=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 * This file is subject to the terms and conditions of the GNU General Public
 * License.  See the file "COPYING" in the main directory of this archive
 * for more details.
 *
 * Copyright (C) 2001 - 2007 Tensilica Inc.
<<<<<<< HEAD
 */

struct pt_regs;
struct sigaction;
asmlinkage long xtensa_execve(char*, char**, char**, struct pt_regs*);
asmlinkage long xtensa_clone(unsigned long, unsigned long, struct pt_regs*);
asmlinkage long xtensa_ptrace(long, long, long, long);
asmlinkage long xtensa_sigreturn(struct pt_regs*);
asmlinkage long xtensa_rt_sigreturn(struct pt_regs*);
asmlinkage long xtensa_sigsuspend(struct pt_regs*);
asmlinkage long xtensa_rt_sigsuspend(struct pt_regs*);
asmlinkage long xtensa_sigaction(int, const struct old_sigaction*,
				 struct old_sigaction*);
asmlinkage long xtensa_sigaltstack(struct pt_regs *regs);
asmlinkage long sys_rt_sigaction(int,
				 const struct sigaction __user *,
				 struct sigaction __user *,
				 size_t);
=======
 * Copyright (C) 2018 Cadence Design Systems Inc.
 */

#ifndef _ASM_SYSCALL_H
#define _ASM_SYSCALL_H

#include <linux/err.h>
#include <asm/ptrace.h>
#include <uapi/linux/audit.h>

static inline int syscall_get_arch(struct task_struct *task)
{
	return AUDIT_ARCH_XTENSA;
}

typedef void (*syscall_t)(void);
extern syscall_t sys_call_table[];

static inline long syscall_get_nr(struct task_struct *task,
				  struct pt_regs *regs)
{
	return regs->syscall;
}

static inline void syscall_rollback(struct task_struct *task,
				    struct pt_regs *regs)
{
	/* Do nothing. */
}

static inline long syscall_get_error(struct task_struct *task,
				     struct pt_regs *regs)
{
	/* 0 if syscall succeeded, otherwise -Errorcode */
	return IS_ERR_VALUE(regs->areg[2]) ? regs->areg[2] : 0;
}

static inline long syscall_get_return_value(struct task_struct *task,
					    struct pt_regs *regs)
{
	return regs->areg[2];
}

static inline void syscall_set_return_value(struct task_struct *task,
					    struct pt_regs *regs,
					    int error, long val)
{
	regs->areg[2] = (long) error ? error : val;
}

#define SYSCALL_MAX_ARGS 6
#define XTENSA_SYSCALL_ARGUMENT_REGS {6, 3, 4, 5, 8, 9}

static inline void syscall_get_arguments(struct task_struct *task,
					 struct pt_regs *regs,
					 unsigned long *args)
{
	static const unsigned int reg[] = XTENSA_SYSCALL_ARGUMENT_REGS;
	unsigned int i;

	for (i = 0; i < 6; ++i)
		args[i] = regs->areg[reg[i]];
}

asmlinkage long xtensa_rt_sigreturn(void);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
asmlinkage long xtensa_shmat(int, char __user *, int);
asmlinkage long xtensa_fadvise64_64(int, int,
				    unsigned long long, unsigned long long);

<<<<<<< HEAD
/* Should probably move to linux/syscalls.h */
struct pollfd;
asmlinkage long sys_pselect6(int n, fd_set __user *inp, fd_set __user *outp,
	fd_set __user *exp, struct timespec __user *tsp, void __user *sig);
asmlinkage long sys_ppoll(struct pollfd __user *ufds, unsigned int nfds,
	struct timespec __user *tsp, const sigset_t __user *sigmask,
	size_t sigsetsize);


=======
#endif
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
