<<<<<<< HEAD
=======
/* SPDX-License-Identifier: GPL-2.0 */
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#ifndef _ASM_MICROBLAZE_FTRACE
#define _ASM_MICROBLAZE_FTRACE

#ifdef CONFIG_FUNCTION_TRACER

<<<<<<< HEAD
#define MCOUNT_ADDR		((long)(_mcount))
=======
#define MCOUNT_ADDR		((unsigned long)(_mcount))
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#define MCOUNT_INSN_SIZE	8 /* sizeof mcount call */

#ifndef __ASSEMBLY__
extern void _mcount(void);
extern void ftrace_call_graph(void);
<<<<<<< HEAD
#endif

#ifdef CONFIG_DYNAMIC_FTRACE
/* reloction of mcount call site is the same as the address */
=======
void prepare_ftrace_return(unsigned long *parent, unsigned long self_addr);
#endif

#ifdef CONFIG_DYNAMIC_FTRACE
/* relocation of mcount call site is the same as the address */
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
static inline unsigned long ftrace_call_adjust(unsigned long addr)
{
	return addr;
}

struct dyn_arch_ftrace {
};
#endif /* CONFIG_DYNAMIC_FTRACE */

#endif /* CONFIG_FUNCTION_TRACER */
#endif /* _ASM_MICROBLAZE_FTRACE */
