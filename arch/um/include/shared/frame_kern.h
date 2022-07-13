<<<<<<< HEAD
/* 
 * Copyright (C) 2002 Jeff Dike (jdike@karaya.com)
 * Licensed under the GPL
=======
/* SPDX-License-Identifier: GPL-2.0 */
/* 
 * Copyright (C) 2002 Jeff Dike (jdike@karaya.com)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 */

#ifndef __FRAME_KERN_H_
#define __FRAME_KERN_H_

<<<<<<< HEAD
#define _S(nr) (1<<((nr)-1))
#define _BLOCKABLE (~(_S(SIGKILL) | _S(SIGSTOP)))

extern int setup_signal_stack_sc(unsigned long stack_top, int sig, 
				 struct k_sigaction *ka,
				 struct pt_regs *regs, 
				 sigset_t *mask);
extern int setup_signal_stack_si(unsigned long stack_top, int sig, 
				 struct k_sigaction *ka,
				 struct pt_regs *regs, siginfo_t *info, 
				 sigset_t *mask);
=======
extern int setup_signal_stack_sc(unsigned long stack_top, struct ksignal *ksig,
				 struct pt_regs *regs, sigset_t *mask);
extern int setup_signal_stack_si(unsigned long stack_top, struct ksignal *ksig,
				 struct pt_regs *regs, sigset_t *mask);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

#endif

