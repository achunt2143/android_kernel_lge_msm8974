<<<<<<< HEAD
=======
/* SPDX-License-Identifier: GPL-2.0 */
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#ifndef __ASM_SH_KDEBUG_H
#define __ASM_SH_KDEBUG_H

/* Grossly misnamed. */
enum die_val {
	DIE_TRAP,
	DIE_NMI,
	DIE_OOPS,
	DIE_BREAKPOINT,
	DIE_SSTEP,
};

<<<<<<< HEAD
=======
/* arch/sh/kernel/dumpstack.c */
extern void printk_address(unsigned long address, int reliable);
extern void dump_mem(const char *str, const char *loglvl,
		     unsigned long bottom, unsigned long top);

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#endif /* __ASM_SH_KDEBUG_H */
