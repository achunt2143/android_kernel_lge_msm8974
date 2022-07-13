<<<<<<< HEAD
#ifndef __ASM_LINKAGE_H
#define __ASM_LINKAGE_H

/* Nothing to see here... */
=======
/* SPDX-License-Identifier: GPL-2.0 */
#ifndef __ASM_LINKAGE_H
#define __ASM_LINKAGE_H

#define cond_syscall(x)  asm(".weak\t" #x "\n" #x " = sys_ni_syscall")
#define SYSCALL_ALIAS(alias, name)					\
	asm ( #alias " = " #name "\n\t.globl " #alias)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

#endif
