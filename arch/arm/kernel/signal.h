<<<<<<< HEAD
/*
 *  linux/arch/arm/kernel/signal.h
 *
 *  Copyright (C) 2005-2009 Russell King.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 */
#define KERN_SIGRETURN_CODE	(CONFIG_VECTORS_BASE + 0x00000500)
#define KERN_RESTART_CODE	(KERN_SIGRETURN_CODE + sizeof(sigreturn_codes))

extern const unsigned long syscall_restart_code[2];
=======
#include <asm/ucontext.h>

struct sigframe {
	struct ucontext uc;
	unsigned long retcode[4];
};

struct rt_sigframe {
	struct siginfo info;
	struct sigframe sig;
};

extern struct page *get_signal_page(void);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
