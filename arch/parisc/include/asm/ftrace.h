<<<<<<< HEAD
=======
/* SPDX-License-Identifier: GPL-2.0 */
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#ifndef _ASM_PARISC_FTRACE_H
#define _ASM_PARISC_FTRACE_H

#ifndef __ASSEMBLY__
extern void mcount(void);

<<<<<<< HEAD
/*
 * Stack of return addresses for functions of a thread.
 * Used in struct thread_info
 */
struct ftrace_ret_stack {
	unsigned long ret;
	unsigned long func;
	unsigned long long calltime;
};

/*
 * Primary handler of a function return.
 * It relays on ftrace_return_to_handler.
 * Defined in entry.S
 */
extern void return_to_handler(void);


extern unsigned long return_address(unsigned int);

#define HAVE_ARCH_CALLER_ADDR

#define CALLER_ADDR0 ((unsigned long)__builtin_return_address(0))
#define CALLER_ADDR1 return_address(1)
#define CALLER_ADDR2 return_address(2)
#define CALLER_ADDR3 return_address(3)
#define CALLER_ADDR4 return_address(4)
#define CALLER_ADDR5 return_address(5)
#define CALLER_ADDR6 return_address(6)
=======
#define MCOUNT_ADDR		((unsigned long)mcount)
#define MCOUNT_INSN_SIZE	4
#define CC_USING_NOP_MCOUNT
#define ARCH_SUPPORTS_FTRACE_OPS 1
extern unsigned long sys_call_table[];

extern unsigned long return_address(unsigned int);
struct ftrace_regs;
extern void ftrace_function_trampoline(unsigned long parent,
		unsigned long self_addr, unsigned long org_sp_gr3,
		struct ftrace_regs *fregs);

#ifdef CONFIG_DYNAMIC_FTRACE
extern void ftrace_caller(void);

struct dyn_arch_ftrace {
};

unsigned long ftrace_call_adjust(unsigned long addr);

#endif

#define ftrace_return_address(n) return_address(n)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

#endif /* __ASSEMBLY__ */

#endif /* _ASM_PARISC_FTRACE_H */
