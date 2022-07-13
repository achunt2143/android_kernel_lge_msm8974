<<<<<<< HEAD
#ifndef _ASM_ARM_FTRACE
#define _ASM_ARM_FTRACE

=======
/* SPDX-License-Identifier: GPL-2.0 */
#ifndef _ASM_ARM_FTRACE
#define _ASM_ARM_FTRACE

#define HAVE_FUNCTION_GRAPH_FP_TEST

#ifdef CONFIG_DYNAMIC_FTRACE_WITH_REGS
#define ARCH_SUPPORTS_FTRACE_OPS 1
#endif

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#ifdef CONFIG_FUNCTION_TRACER
#define MCOUNT_ADDR		((unsigned long)(__gnu_mcount_nc))
#define MCOUNT_INSN_SIZE	4 /* sizeof mcount call */

#ifndef __ASSEMBLY__
<<<<<<< HEAD
extern void mcount(void);
=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
extern void __gnu_mcount_nc(void);

#ifdef CONFIG_DYNAMIC_FTRACE
struct dyn_arch_ftrace {
<<<<<<< HEAD
#ifdef CONFIG_OLD_MCOUNT
	bool	old_mcount;
=======
#ifdef CONFIG_ARM_MODULE_PLTS
	struct module *mod;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#endif
};

static inline unsigned long ftrace_call_adjust(unsigned long addr)
{
	/* With Thumb-2, the recorded addresses have the lsb set */
	return addr & ~1;
}
<<<<<<< HEAD

extern void ftrace_caller_old(void);
extern void ftrace_call_old(void);
=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#endif

#endif

#endif

#ifndef __ASSEMBLY__

#if defined(CONFIG_FRAME_POINTER) && !defined(CONFIG_ARM_UNWIND)
/*
 * return_address uses walk_stackframe to do it's work.  If both
 * CONFIG_FRAME_POINTER=y and CONFIG_ARM_UNWIND=y walk_stackframe uses unwind
 * information.  For this to work in the function tracer many functions would
 * have to be marked with __notrace.  So for now just depend on
 * !CONFIG_ARM_UNWIND.
 */

void *return_address(unsigned int);

#else

<<<<<<< HEAD
extern inline void *return_address(unsigned int level)
{
	return NULL;
=======
static inline void *return_address(unsigned int level)
{
       return NULL;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

#endif

<<<<<<< HEAD
#define HAVE_ARCH_CALLER_ADDR

#define CALLER_ADDR0 ((unsigned long)__builtin_return_address(0))
#define CALLER_ADDR1 ((unsigned long)return_address(1))
#define CALLER_ADDR2 ((unsigned long)return_address(2))
#define CALLER_ADDR3 ((unsigned long)return_address(3))
#define CALLER_ADDR4 ((unsigned long)return_address(4))
#define CALLER_ADDR5 ((unsigned long)return_address(5))
#define CALLER_ADDR6 ((unsigned long)return_address(6))
=======
#define ftrace_return_address(n) return_address(n)

#define ARCH_HAS_SYSCALL_MATCH_SYM_NAME

static inline bool arch_syscall_match_sym_name(const char *sym,
					       const char *name)
{
	if (!strcmp(sym, "sys_mmap2"))
		sym = "sys_mmap_pgoff";
	else if (!strcmp(sym, "sys_statfs64_wrapper"))
		sym = "sys_statfs64";
	else if (!strcmp(sym, "sys_fstatfs64_wrapper"))
		sym = "sys_fstatfs64";
	else if (!strcmp(sym, "sys_arm_fadvise64_64"))
		sym = "sys_fadvise64_64";

	/* Ignore case since sym may start with "SyS" instead of "sys" */
	return !strcasecmp(sym, name);
}

void prepare_ftrace_return(unsigned long *parent, unsigned long self,
			   unsigned long frame_pointer,
			   unsigned long stack_pointer);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

#endif /* ifndef __ASSEMBLY__ */

#endif /* _ASM_ARM_FTRACE */
