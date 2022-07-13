/* 
 * Copyright (C) 2000 - 2008 Jeff Dike (jdike@{addtoit,linux.intel}.com)
 * Licensed under the GPL
 */

<<<<<<< HEAD
#include "asm/unistd.h"
#include "sysdep/ptrace.h"

typedef long syscall_handler_t(struct pt_regs);

/* Not declared on x86, incompatible declarations on x86_64, so these have
 * to go here rather than in sys_call_table.c
 */
extern syscall_handler_t sys_rt_sigaction;
=======
#include <asm/unistd.h>
#include <sysdep/ptrace.h>

typedef long syscall_handler_t(struct syscall_args);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

extern syscall_handler_t *sys_call_table[];

#define EXECUTE_SYSCALL(syscall, regs) \
<<<<<<< HEAD
	((long (*)(struct syscall_args)) \
	 (*sys_call_table[syscall]))(SYSCALL_ARGS(&regs->regs))
=======
	((*sys_call_table[syscall]))(SYSCALL_ARGS(&regs->regs))
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
