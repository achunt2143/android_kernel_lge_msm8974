/*
 * This file is subject to the terms and conditions of the GNU General Public
 * License.  See the file "COPYING" in the main directory of this archive
 * for more details.
 *
 * Copyright (C) 1991, 1992  Linus Torvalds
 * Copyright (C) 1994 - 2000  Ralf Baechle
 * Copyright (C) 1999, 2000 Silicon Graphics, Inc.
 */

#ifndef __SIGNAL_COMMON_H
#define __SIGNAL_COMMON_H

/* #define DEBUG_SIG */

#ifdef DEBUG_SIG
#  define DEBUGP(fmt, args...) printk("%s: " fmt, __func__, ##args)
#else
#  define DEBUGP(fmt, args...)
#endif

<<<<<<< HEAD
#define _BLOCKABLE (~(sigmask(SIGKILL) | sigmask(SIGSTOP)))

/*
 * Determine which stack to use..
 */
extern void __user *get_sigframe(struct k_sigaction *ka, struct pt_regs *regs,
=======
/*
 * Determine which stack to use..
 */
extern void __user *get_sigframe(struct ksignal *ksig, struct pt_regs *regs,
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
				 size_t frame_size);
/* Check and clear pending FPU exceptions in saved CSR */
extern int fpcsr_pending(unsigned int __user *fpcsr);

/* Make sure we will not lose FPU ownership */
<<<<<<< HEAD
#ifdef CONFIG_PREEMPT
#define lock_fpu_owner()	preempt_disable()
#define unlock_fpu_owner()	preempt_enable()
#else
#define lock_fpu_owner()	pagefault_disable()
#define unlock_fpu_owner()	pagefault_enable()
#endif
=======
#define lock_fpu_owner()	({ preempt_disable(); pagefault_disable(); })
#define unlock_fpu_owner()	({ pagefault_enable(); preempt_enable(); })

/* Assembly functions to move context to/from the FPU */
extern asmlinkage int
_save_fp_context(void __user *fpregs, void __user *csr);
extern asmlinkage int
_restore_fp_context(void __user *fpregs, void __user *csr);

extern asmlinkage int _save_msa_all_upper(void __user *buf);
extern asmlinkage int _restore_msa_all_upper(void __user *buf);

extern int setup_sigcontext(struct pt_regs *, struct sigcontext __user *);
extern int restore_sigcontext(struct pt_regs *, struct sigcontext __user *);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

#endif	/* __SIGNAL_COMMON_H */
