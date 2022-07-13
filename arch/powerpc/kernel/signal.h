<<<<<<< HEAD
/*
 *    Copyright (c) 2007 Benjamin Herrenschmidt, IBM Coproration
 *    Extracted from signal_32.c and signal_64.c
 *
 * This file is subject to the terms and conditions of the GNU General
 * Public License.  See the file README.legal in the main directory of
 * this archive for more details.
=======
/* SPDX-License-Identifier: GPL-2.0-or-later
 *
 *    Copyright (c) 2007 Benjamin Herrenschmidt, IBM Corporation
 *    Extracted from signal_32.c and signal_64.c
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 */

#ifndef _POWERPC_ARCH_SIGNAL_H
#define _POWERPC_ARCH_SIGNAL_H

<<<<<<< HEAD
#define _BLOCKABLE (~(sigmask(SIGKILL) | sigmask(SIGSTOP)))

extern void do_notify_resume(struct pt_regs *regs, unsigned long thread_info_flags);

extern void __user * get_sigframe(struct k_sigaction *ka, struct pt_regs *regs,
				  size_t frame_size, int is_32);
extern void restore_sigmask(sigset_t *set);

extern int handle_signal32(unsigned long sig, struct k_sigaction *ka,
			   siginfo_t *info, sigset_t *oldset,
			   struct pt_regs *regs);

extern int handle_rt_signal32(unsigned long sig, struct k_sigaction *ka,
			      siginfo_t *info, sigset_t *oldset,
			      struct pt_regs *regs);

extern unsigned long copy_fpr_to_user(void __user *to,
				      struct task_struct *task);
extern unsigned long copy_fpr_from_user(struct task_struct *task,
					void __user *from);
#ifdef CONFIG_VSX
extern unsigned long copy_vsx_to_user(void __user *to,
				      struct task_struct *task);
extern unsigned long copy_vsx_from_user(struct task_struct *task,
					void __user *from);
=======
void __user *get_sigframe(struct ksignal *ksig, struct task_struct *tsk,
			  size_t frame_size, int is_32);

extern int handle_signal32(struct ksignal *ksig, sigset_t *oldset,
			   struct task_struct *tsk);

extern int handle_rt_signal32(struct ksignal *ksig, sigset_t *oldset,
			      struct task_struct *tsk);

static inline int __get_user_sigset(sigset_t *dst, const sigset_t __user *src)
{
	BUILD_BUG_ON(sizeof(sigset_t) != sizeof(u64));

	return __get_user(dst->sig[0], (u64 __user *)&src->sig[0]);
}
#define unsafe_get_user_sigset(dst, src, label) do {			\
	sigset_t *__dst = dst;						\
	const sigset_t __user *__src = src;				\
	int i;								\
									\
	for (i = 0; i < _NSIG_WORDS; i++)				\
		unsafe_get_user(__dst->sig[i], &__src->sig[i], label);	\
} while (0)

#ifdef CONFIG_VSX
extern unsigned long copy_vsx_to_user(void __user *to,
				      struct task_struct *task);
extern unsigned long copy_ckvsx_to_user(void __user *to,
					       struct task_struct *task);
extern unsigned long copy_vsx_from_user(struct task_struct *task,
					void __user *from);
extern unsigned long copy_ckvsx_from_user(struct task_struct *task,
						 void __user *from);
unsigned long copy_fpr_to_user(void __user *to, struct task_struct *task);
unsigned long copy_ckfpr_to_user(void __user *to, struct task_struct *task);
unsigned long copy_fpr_from_user(struct task_struct *task, void __user *from);
unsigned long copy_ckfpr_from_user(struct task_struct *task, void __user *from);

#define unsafe_copy_fpr_to_user(to, task, label)	do {		\
	struct task_struct *__t = task;					\
	u64 __user *buf = (u64 __user *)to;				\
	int i;								\
									\
	for (i = 0; i < ELF_NFPREG - 1 ; i++)				\
		unsafe_put_user(__t->thread.TS_FPR(i), &buf[i], label); \
	unsafe_put_user(__t->thread.fp_state.fpscr, &buf[i], label);	\
} while (0)

#define unsafe_copy_vsx_to_user(to, task, label)	do {		\
	struct task_struct *__t = task;					\
	u64 __user *buf = (u64 __user *)to;				\
	int i;								\
									\
	for (i = 0; i < ELF_NVSRHALFREG ; i++)				\
		unsafe_put_user(__t->thread.fp_state.fpr[i][TS_VSRLOWOFFSET], \
				&buf[i], label);\
} while (0)

#define unsafe_copy_fpr_from_user(task, from, label)	do {		\
	struct task_struct *__t = task;					\
	u64 __user *buf = (u64 __user *)from;				\
	int i;								\
									\
	for (i = 0; i < ELF_NFPREG - 1; i++)				\
		unsafe_get_user(__t->thread.TS_FPR(i), &buf[i], label); \
	unsafe_get_user(__t->thread.fp_state.fpscr, &buf[i], label);	\
} while (0)

#define unsafe_copy_vsx_from_user(task, from, label)	do {		\
	struct task_struct *__t = task;					\
	u64 __user *buf = (u64 __user *)from;				\
	int i;								\
									\
	for (i = 0; i < ELF_NVSRHALFREG ; i++)				\
		unsafe_get_user(__t->thread.fp_state.fpr[i][TS_VSRLOWOFFSET], \
				&buf[i], label);			\
} while (0)

#ifdef CONFIG_PPC_TRANSACTIONAL_MEM
#define unsafe_copy_ckfpr_to_user(to, task, label)	do {		\
	struct task_struct *__t = task;					\
	u64 __user *buf = (u64 __user *)to;				\
	int i;								\
									\
	for (i = 0; i < ELF_NFPREG - 1 ; i++)				\
		unsafe_put_user(__t->thread.TS_CKFPR(i), &buf[i], label);\
	unsafe_put_user(__t->thread.ckfp_state.fpscr, &buf[i], label);	\
} while (0)

#define unsafe_copy_ckvsx_to_user(to, task, label)	do {		\
	struct task_struct *__t = task;					\
	u64 __user *buf = (u64 __user *)to;				\
	int i;								\
									\
	for (i = 0; i < ELF_NVSRHALFREG ; i++)				\
		unsafe_put_user(__t->thread.ckfp_state.fpr[i][TS_VSRLOWOFFSET], \
				&buf[i], label);\
} while (0)

#define unsafe_copy_ckfpr_from_user(task, from, label)	do {		\
	struct task_struct *__t = task;					\
	u64 __user *buf = (u64 __user *)from;				\
	int i;								\
									\
	for (i = 0; i < ELF_NFPREG - 1 ; i++)				\
		unsafe_get_user(__t->thread.TS_CKFPR(i), &buf[i], label);\
	unsafe_get_user(__t->thread.ckfp_state.fpscr, &buf[i], failed);	\
} while (0)

#define unsafe_copy_ckvsx_from_user(task, from, label)	do {		\
	struct task_struct *__t = task;					\
	u64 __user *buf = (u64 __user *)from;				\
	int i;								\
									\
	for (i = 0; i < ELF_NVSRHALFREG ; i++)				\
		unsafe_get_user(__t->thread.ckfp_state.fpr[i][TS_VSRLOWOFFSET], \
				&buf[i], label);			\
} while (0)
#endif
#elif defined(CONFIG_PPC_FPU_REGS)

#define unsafe_copy_fpr_to_user(to, task, label)		\
	unsafe_copy_to_user(to, (task)->thread.fp_state.fpr,	\
			    ELF_NFPREG * sizeof(double), label)

#define unsafe_copy_fpr_from_user(task, from, label)			\
	unsafe_copy_from_user((task)->thread.fp_state.fpr, from,	\
			    ELF_NFPREG * sizeof(double), label)

static inline unsigned long
copy_fpr_to_user(void __user *to, struct task_struct *task)
{
	return __copy_to_user(to, task->thread.fp_state.fpr,
			      ELF_NFPREG * sizeof(double));
}

static inline unsigned long
copy_fpr_from_user(struct task_struct *task, void __user *from)
{
	return __copy_from_user(task->thread.fp_state.fpr, from,
			      ELF_NFPREG * sizeof(double));
}

#ifdef CONFIG_PPC_TRANSACTIONAL_MEM
#define unsafe_copy_ckfpr_to_user(to, task, label)		\
	unsafe_copy_to_user(to, (task)->thread.ckfp_state.fpr,	\
			    ELF_NFPREG * sizeof(double), label)

inline unsigned long copy_ckfpr_to_user(void __user *to, struct task_struct *task)
{
	return __copy_to_user(to, task->thread.ckfp_state.fpr,
			      ELF_NFPREG * sizeof(double));
}

static inline unsigned long
copy_ckfpr_from_user(struct task_struct *task, void __user *from)
{
	return __copy_from_user(task->thread.ckfp_state.fpr, from,
				ELF_NFPREG * sizeof(double));
}
#endif /* CONFIG_PPC_TRANSACTIONAL_MEM */
#else
#define unsafe_copy_fpr_to_user(to, task, label) do { if (0) goto label;} while (0)

#define unsafe_copy_fpr_from_user(task, from, label) do { if (0) goto label;} while (0)

static inline unsigned long
copy_fpr_to_user(void __user *to, struct task_struct *task)
{
	return 0;
}

static inline unsigned long
copy_fpr_from_user(struct task_struct *task, void __user *from)
{
	return 0;
}
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#endif

#ifdef CONFIG_PPC64

<<<<<<< HEAD
extern int handle_rt_signal64(int signr, struct k_sigaction *ka,
			      siginfo_t *info, sigset_t *set,
			      struct pt_regs *regs);

#else /* CONFIG_PPC64 */

static inline int handle_rt_signal64(int signr, struct k_sigaction *ka,
				     siginfo_t *info, sigset_t *set,
				     struct pt_regs *regs)
=======
extern int handle_rt_signal64(struct ksignal *ksig, sigset_t *set,
			      struct task_struct *tsk);

#else /* CONFIG_PPC64 */

static inline int handle_rt_signal64(struct ksignal *ksig, sigset_t *set,
				     struct task_struct *tsk)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
{
	return -EFAULT;
}

#endif /* !defined(CONFIG_PPC64) */

<<<<<<< HEAD
=======
void signal_fault(struct task_struct *tsk, struct pt_regs *regs,
		  const char *where, void __user *ptr);

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#endif  /* _POWERPC_ARCH_SIGNAL_H */
