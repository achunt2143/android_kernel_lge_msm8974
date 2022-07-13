<<<<<<< HEAD
/*
 * Copyright (C) 2002 - 2007 Jeff Dike (jdike@{addtoit,linux.intel}.com)
 * Licensed under the GPL
=======
/* SPDX-License-Identifier: GPL-2.0 */
/*
 * Copyright (C) 2002 - 2007 Jeff Dike (jdike@{addtoit,linux.intel}.com)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 */

#ifndef __SKAS_H
#define __SKAS_H

<<<<<<< HEAD
#include "sysdep/ptrace.h"

extern int userspace_pid[];
extern int proc_mm, ptrace_faultinfo, ptrace_ldt;
extern int skas_needs_stub;
=======
#include <sysdep/ptrace.h>

extern int userspace_pid[];
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

extern int user_thread(unsigned long stack, int flags);
extern void new_thread_handler(void);
extern void handle_syscall(struct uml_pt_regs *regs);
<<<<<<< HEAD
extern int new_mm(unsigned long stack);
=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
extern long execute_syscall_skas(void *r);
extern unsigned long current_stub_stack(void);

#endif
