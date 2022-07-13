<<<<<<< HEAD
=======
/* SPDX-License-Identifier: GPL-2.0 */
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#ifndef _ASM_X86_MATH_EMU_H
#define _ASM_X86_MATH_EMU_H

#include <asm/ptrace.h>
<<<<<<< HEAD
#include <asm/vm86.h>
=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

/* This structure matches the layout of the data saved to the stack
   following a device-not-present interrupt, part of it saved
   automatically by the 80386/80486.
   */
struct math_emu_info {
	long ___orig_eip;
<<<<<<< HEAD
	union {
		struct pt_regs *regs;
		struct kernel_vm86_regs *vm86;
	};
=======
	struct pt_regs *regs;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
};
#endif /* _ASM_X86_MATH_EMU_H */
