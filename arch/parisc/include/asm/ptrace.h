<<<<<<< HEAD
#ifndef _PARISC_PTRACE_H
#define _PARISC_PTRACE_H

/* written by Philipp Rumpf, Copyright (C) 1999 SuSE GmbH Nuernberg
** Copyright (C) 2000 Grant Grundler, Hewlett-Packard
*/

#include <linux/types.h>

/* This struct defines the way the registers are stored on the 
 * stack during a system call.
 *
 * N.B. gdb/strace care about the size and offsets within this
 * structure. If you change things, you may break object compatibility
 * for those applications.
 */

struct pt_regs {
	unsigned long gr[32];	/* PSW is in gr[0] */
	__u64 fr[32];
	unsigned long sr[ 8];
	unsigned long iasq[2];
	unsigned long iaoq[2];
	unsigned long cr27;
	unsigned long pad0;     /* available for other uses */
	unsigned long orig_r28;
	unsigned long ksp;
	unsigned long kpc;
	unsigned long sar;	/* CR11 */
	unsigned long iir;	/* CR19 */
	unsigned long isr;	/* CR20 */
	unsigned long ior;	/* CR21 */
	unsigned long ipsw;	/* CR22 */
};

/*
 * The numbers chosen here are somewhat arbitrary but absolutely MUST
 * not overlap with any of the number assigned in <linux/ptrace.h>.
 *
 * These ones are taken from IA-64 on the assumption that theirs are
 * the most correct (and we also want to support PTRACE_SINGLEBLOCK
 * since we have taken branch traps too)
 */
#define PTRACE_SINGLEBLOCK	12	/* resume execution until next branch */

#ifdef __KERNEL__
=======
/* SPDX-License-Identifier: GPL-2.0 */
/* written by Philipp Rumpf, Copyright (C) 1999 SuSE GmbH Nuernberg
** Copyright (C) 2000 Grant Grundler, Hewlett-Packard
*/
#ifndef _PARISC_PTRACE_H
#define _PARISC_PTRACE_H

#include <asm/assembly.h>
#include <uapi/asm/ptrace.h>
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

#define task_regs(task) ((struct pt_regs *) ((char *)(task) + TASK_REGS))

#define arch_has_single_step()	1
#define arch_has_block_step()	1

/* XXX should we use iaoq[1] or iaoq[0] ? */
<<<<<<< HEAD
#define user_mode(regs)			(((regs)->iaoq[0] & 3) ? 1 : 0)
#define user_space(regs)		(((regs)->iasq[1] != 0) ? 1 : 0)
=======
#define user_mode(regs)			(((regs)->iaoq[0] & 3) != PRIV_KERNEL)
#define user_space(regs)		((regs)->iasq[1] != PRIV_KERNEL)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#define instruction_pointer(regs)	((regs)->iaoq[0] & ~3)
#define user_stack_pointer(regs)	((regs)->gr[30])
unsigned long profile_pc(struct pt_regs *);

<<<<<<< HEAD

#endif /* __KERNEL__ */
=======
static inline unsigned long regs_return_value(struct pt_regs *regs)
{
	return regs->gr[28];
}

static inline void instruction_pointer_set(struct pt_regs *regs,
						unsigned long val)
{
	regs->iaoq[0] = val;
	regs->iaoq[1] = val + 4;
}

/* Query offset/name of register from its name/offset */
extern int regs_query_register_offset(const char *name);
extern const char *regs_query_register_name(unsigned int offset);
#define MAX_REG_OFFSET (offsetof(struct pt_regs, ipsw))

#define kernel_stack_pointer(regs) ((regs)->gr[30])

static inline unsigned long regs_get_register(struct pt_regs *regs,
					      unsigned int offset)
{
	if (unlikely(offset > MAX_REG_OFFSET))
		return 0;
	return *(unsigned long *)((unsigned long)regs + offset);
}

unsigned long regs_get_kernel_stack_nth(struct pt_regs *regs, unsigned int n);
int regs_within_kernel_stack(struct pt_regs *regs, unsigned long addr);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

#endif
