<<<<<<< HEAD
#ifndef __ASM_TRAPS_H
#define __ASM_TRAPS_H

#ifdef __KERNEL__
=======
/* SPDX-License-Identifier: GPL-2.0 */
#ifndef __ASM_TRAPS_H
#define __ASM_TRAPS_H

#define PARISC_ITLB_TRAP	6 /* defined by architecture. Do not change. */

#if !defined(__ASSEMBLY__)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
struct pt_regs;

/* traps.c */
void parisc_terminate(char *msg, struct pt_regs *regs,
<<<<<<< HEAD
		int code, unsigned long offset);

/* mm/fault.c */
void do_page_fault(struct pt_regs *regs, unsigned long code,
		unsigned long address);
=======
		int code, unsigned long offset) __noreturn __cold;

void die_if_kernel(char *str, struct pt_regs *regs, long err);

/* mm/fault.c */
unsigned long parisc_acctyp(unsigned long code, unsigned int inst);
const char *trap_name(unsigned long code);
void do_page_fault(struct pt_regs *regs, unsigned long code,
		unsigned long address);
int handle_nadtlb_fault(struct pt_regs *regs);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#endif

#endif
