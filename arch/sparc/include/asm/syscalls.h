<<<<<<< HEAD
=======
/* SPDX-License-Identifier: GPL-2.0 */
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#ifndef _SPARC64_SYSCALLS_H
#define _SPARC64_SYSCALLS_H

struct pt_regs;

<<<<<<< HEAD
extern asmlinkage long sparc_do_fork(unsigned long clone_flags,
				     unsigned long stack_start,
				     struct pt_regs *regs,
				     unsigned long stack_size);

extern asmlinkage int sparc_execve(struct pt_regs *regs);
=======
asmlinkage long sparc_fork(struct pt_regs *regs);
asmlinkage long sparc_vfork(struct pt_regs *regs);
asmlinkage long sparc_clone(struct pt_regs *regs);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

#endif /* _SPARC64_SYSCALLS_H */
