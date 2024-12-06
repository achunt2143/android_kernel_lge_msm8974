/*
 * Copyright 2003 PathScale, Inc.
 *
 * Licensed under the GPL
 */

#ifndef __SYSDEP_X86_64_SYSCALLS_H__
#define __SYSDEP_X86_64_SYSCALLS_H__

#include <linux/msg.h>
#include <linux/shm.h>

<<<<<<< HEAD
typedef long syscall_handler_t(void);
=======
typedef long syscall_handler_t(long, long, long, long, long, long);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

extern syscall_handler_t *sys_call_table[];

#define EXECUTE_SYSCALL(syscall, regs) \
<<<<<<< HEAD
	(((long (*)(long, long, long, long, long, long)) \
	  (*sys_call_table[syscall]))(UPT_SYSCALL_ARG1(&regs->regs), \
=======
	(((*sys_call_table[syscall]))(UPT_SYSCALL_ARG1(&regs->regs), \
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		 		      UPT_SYSCALL_ARG2(&regs->regs), \
				      UPT_SYSCALL_ARG3(&regs->regs), \
				      UPT_SYSCALL_ARG4(&regs->regs), \
				      UPT_SYSCALL_ARG5(&regs->regs), \
				      UPT_SYSCALL_ARG6(&regs->regs)))

<<<<<<< HEAD
extern long old_mmap(unsigned long addr, unsigned long len,
		     unsigned long prot, unsigned long flags,
		     unsigned long fd, unsigned long pgoff);
=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
extern syscall_handler_t sys_modify_ldt;
extern syscall_handler_t sys_arch_prctl;

#endif
