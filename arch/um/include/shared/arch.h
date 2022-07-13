<<<<<<< HEAD
/*
 * Copyright (C) 2007 Jeff Dike (jdike@{addtoit,linux.intel}.com)
 * Licensed under the GPL
=======
/* SPDX-License-Identifier: GPL-2.0 */
/*
 * Copyright (C) 2007 Jeff Dike (jdike@{addtoit,linux.intel}.com)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 */

#ifndef __ARCH_H__
#define __ARCH_H__

<<<<<<< HEAD
#include "sysdep/ptrace.h"
=======
#include <sysdep/ptrace.h>
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

extern void arch_check_bugs(void);
extern int arch_fixup(unsigned long address, struct uml_pt_regs *regs);
extern void arch_examine_signal(int sig, struct uml_pt_regs *regs);

#endif
