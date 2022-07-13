<<<<<<< HEAD
#ifndef _ENTRY_H
#define _ENTRY_H

#include <linux/types.h>
#include <linux/signal.h>
#include <asm/ptrace.h>
#include <asm/cputime.h>
#include <asm/timer.h>

extern void (*pgm_check_table[128])(struct pt_regs *);
=======
/* SPDX-License-Identifier: GPL-2.0 */
#ifndef _ENTRY_H
#define _ENTRY_H

#include <linux/percpu.h>
#include <linux/types.h>
#include <linux/signal.h>
#include <asm/extable.h>
#include <asm/ptrace.h>
#include <asm/idle.h>

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
extern void *restart_stack;

void system_call(void);
void pgm_check_handler(void);
void ext_int_handler(void);
void io_int_handler(void);
void mcck_int_handler(void);
void restart_int_handler(void);
<<<<<<< HEAD
void restart_call_handler(void);
void psw_idle(struct s390_idle_data *, struct vtimer_queue *,
	      unsigned long, int);

asmlinkage long do_syscall_trace_enter(struct pt_regs *regs);
asmlinkage void do_syscall_trace_exit(struct pt_regs *regs);

void do_protection_exception(struct pt_regs *regs);
void do_dat_exception(struct pt_regs *regs);
void do_asce_exception(struct pt_regs *regs);

void do_per_trap(struct pt_regs *regs);
void syscall_trace(struct pt_regs *regs, int entryexit);
void kernel_stack_overflow(struct pt_regs * regs);
void do_signal(struct pt_regs *regs);
int handle_signal32(unsigned long sig, struct k_sigaction *ka,
		    siginfo_t *info, sigset_t *oldset, struct pt_regs *regs);
void do_notify_resume(struct pt_regs *regs);

struct ext_code;
void do_extint(struct pt_regs *regs, struct ext_code, unsigned int, unsigned long);
void do_restart(void);
void __init startup_init(void);
void die(struct pt_regs *regs, const char *str);

void __init time_init(void);
=======
void early_pgm_check_handler(void);

struct task_struct *__switch_to_asm(struct task_struct *prev, struct task_struct *next);
void __ret_from_fork(struct task_struct *prev, struct pt_regs *regs);
void __do_pgm_check(struct pt_regs *regs);
void __do_syscall(struct pt_regs *regs, int per_trap);
void __do_early_pgm_check(struct pt_regs *regs);

void do_protection_exception(struct pt_regs *regs);
void do_dat_exception(struct pt_regs *regs);
void do_secure_storage_access(struct pt_regs *regs);
void do_non_secure_storage_access(struct pt_regs *regs);
void do_secure_storage_violation(struct pt_regs *regs);
void do_report_trap(struct pt_regs *regs, int si_signo, int si_code, char *str);
void kernel_stack_overflow(struct pt_regs * regs);
void handle_signal32(struct ksignal *ksig, sigset_t *oldset,
		     struct pt_regs *regs);

void do_io_irq(struct pt_regs *regs);
void do_ext_irq(struct pt_regs *regs);
void do_restart(void *arg);
void __init startup_init(void);
void die(struct pt_regs *regs, const char *str);
int setup_profiling_timer(unsigned int multiplier);
unsigned long prepare_ftrace_return(unsigned long parent, unsigned long sp, unsigned long ip);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

struct s390_mmap_arg_struct;
struct fadvise64_64_args;
struct old_sigaction;

<<<<<<< HEAD
long sys_mmap2(struct s390_mmap_arg_struct __user  *arg);
long sys_s390_ipc(uint call, int first, unsigned long second,
	     unsigned long third, void __user *ptr);
long sys_s390_personality(unsigned int personality);
long sys_s390_fadvise64(int fd, u32 offset_high, u32 offset_low,
		    size_t len, int advice);
long sys_s390_fadvise64_64(struct fadvise64_64_args __user *args);
long sys_s390_fallocate(int fd, int mode, loff_t offset, u32 len_high,
			u32 len_low);
long sys_fork(void);
long sys_clone(unsigned long newsp, unsigned long clone_flags,
	       int __user *parent_tidptr, int __user *child_tidptr);
long sys_vfork(void);
void execve_tail(void);
long sys_execve(const char __user *name, const char __user *const __user *argv,
		const char __user *const __user *envp);
long sys_sigsuspend(int history0, int history1, old_sigset_t mask);
long sys_sigaction(int sig, const struct old_sigaction __user *act,
		   struct old_sigaction __user *oact);
long sys_sigaltstack(const stack_t __user *uss, stack_t __user *uoss);
long sys_sigreturn(void);
long sys_rt_sigreturn(void);
long sys32_sigreturn(void);
long sys32_rt_sigreturn(void);
=======
long sys_rt_sigreturn(void);
long sys_sigreturn(void);

long sys_s390_personality(unsigned int personality);
long sys_s390_runtime_instr(int command, int signum);
long sys_s390_guarded_storage(int command, struct gs_cb __user *);
long sys_s390_pci_mmio_write(unsigned long, const void __user *, size_t);
long sys_s390_pci_mmio_read(unsigned long, void __user *, size_t);
long sys_s390_sthyi(unsigned long function_code, void __user *buffer, u64 __user *return_code, unsigned long flags);

DECLARE_PER_CPU(u64, mt_cycles[8]);

unsigned long stack_alloc(void);
void stack_free(unsigned long stack);

extern char kprobes_insn_page[];

extern char _samode31[], _eamode31[];
extern char _stext_amode31[], _etext_amode31[];
extern struct exception_table_entry _start_amode31_ex_table[];
extern struct exception_table_entry _stop_amode31_ex_table[];

#define __amode31_data __section(".amode31.data")
#define __amode31_ref __section(".amode31.refs")
extern long _start_amode31_refs[], _end_amode31_refs[];
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

#endif /* _ENTRY_H */
