<<<<<<< HEAD
=======
/* SPDX-License-Identifier: GPL-2.0 */
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
/*
 * Copyright (C) 1999 Cort Dougan <cort@cs.nmt.edu>
 */
#ifndef _ASM_POWERPC_DEBUG_H
#define _ASM_POWERPC_DEBUG_H

<<<<<<< HEAD
struct pt_regs;

extern struct dentry *powerpc_debugfs_root;

#if defined(CONFIG_DEBUGGER) || defined(CONFIG_KEXEC)
=======
#include <asm/hw_breakpoint.h>

struct pt_regs;

#if defined(CONFIG_DEBUGGER) || defined(CONFIG_KEXEC_CORE)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

extern int (*__debugger)(struct pt_regs *regs);
extern int (*__debugger_ipi)(struct pt_regs *regs);
extern int (*__debugger_bpt)(struct pt_regs *regs);
extern int (*__debugger_sstep)(struct pt_regs *regs);
extern int (*__debugger_iabr_match)(struct pt_regs *regs);
<<<<<<< HEAD
extern int (*__debugger_dabr_match)(struct pt_regs *regs);
=======
extern int (*__debugger_break_match)(struct pt_regs *regs);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
extern int (*__debugger_fault_handler)(struct pt_regs *regs);

#define DEBUGGER_BOILERPLATE(__NAME) \
static inline int __NAME(struct pt_regs *regs) \
{ \
	if (unlikely(__ ## __NAME)) \
		return __ ## __NAME(regs); \
	return 0; \
}

DEBUGGER_BOILERPLATE(debugger)
DEBUGGER_BOILERPLATE(debugger_ipi)
DEBUGGER_BOILERPLATE(debugger_bpt)
DEBUGGER_BOILERPLATE(debugger_sstep)
DEBUGGER_BOILERPLATE(debugger_iabr_match)
<<<<<<< HEAD
DEBUGGER_BOILERPLATE(debugger_dabr_match)
=======
DEBUGGER_BOILERPLATE(debugger_break_match)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
DEBUGGER_BOILERPLATE(debugger_fault_handler)

#else
static inline int debugger(struct pt_regs *regs) { return 0; }
static inline int debugger_ipi(struct pt_regs *regs) { return 0; }
static inline int debugger_bpt(struct pt_regs *regs) { return 0; }
static inline int debugger_sstep(struct pt_regs *regs) { return 0; }
static inline int debugger_iabr_match(struct pt_regs *regs) { return 0; }
<<<<<<< HEAD
static inline int debugger_dabr_match(struct pt_regs *regs) { return 0; }
static inline int debugger_fault_handler(struct pt_regs *regs) { return 0; }
#endif

extern int set_dabr(unsigned long dabr);
#ifdef CONFIG_PPC_ADV_DEBUG_REGS
extern void do_send_trap(struct pt_regs *regs, unsigned long address,
			 unsigned long error_code, int signal_code, int brkpt);
#else
extern void do_dabr(struct pt_regs *regs, unsigned long address,
		    unsigned long error_code);
=======
static inline int debugger_break_match(struct pt_regs *regs) { return 0; }
static inline int debugger_fault_handler(struct pt_regs *regs) { return 0; }
#endif

void __set_breakpoint(int nr, struct arch_hw_breakpoint *brk);
void suspend_breakpoints(void);
void restore_breakpoints(void);
bool ppc_breakpoint_available(void);
#ifdef CONFIG_PPC_ADV_DEBUG_REGS
extern void do_send_trap(struct pt_regs *regs, unsigned long address,
			 unsigned long error_code, int brkpt);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#endif

#endif /* _ASM_POWERPC_DEBUG_H */
