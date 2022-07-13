/*
 * Copyright (C) 2002 - 2007 Jeff Dike (jdike@{addtoit,linux.intel}.com)
 * Licensed under the GPL
 */

#include <errno.h>
<<<<<<< HEAD
#include "ptrace_user.h"
=======
#include <ptrace_user.h>
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

int ptrace_getregs(long pid, unsigned long *regs_out)
{
	if (ptrace(PTRACE_GETREGS, pid, 0, regs_out) < 0)
		return -errno;
	return 0;
}

int ptrace_setregs(long pid, unsigned long *regs)
{
	if (ptrace(PTRACE_SETREGS, pid, 0, regs) < 0)
		return -errno;
	return 0;
}
