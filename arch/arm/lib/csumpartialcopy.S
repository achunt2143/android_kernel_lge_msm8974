<<<<<<< HEAD
=======
/* SPDX-License-Identifier: GPL-2.0-only */
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
/*
 *  linux/arch/arm/lib/csumpartialcopy.S
 *
 *  Copyright (C) 1995-1998 Russell King
<<<<<<< HEAD
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 */
#include <linux/linkage.h>
#include <asm/assembler.h>

		.text

<<<<<<< HEAD
/* Function: __u32 csum_partial_copy_nocheck(const char *src, char *dst, int len, __u32 sum)
 * Params  : r0 = src, r1 = dst, r2 = len, r3 = checksum
=======
/* Function: __u32 csum_partial_copy_nocheck(const char *src, char *dst, int len)
 * Params  : r0 = src, r1 = dst, r2 = len
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 * Returns : r0 = new checksum
 */

		.macro	save_regs
		stmfd	sp!, {r1, r4 - r8, lr}
		.endm

		.macro	load_regs
		ldmfd	sp!, {r1, r4 - r8, pc}
		.endm

		.macro	load1b, reg1
		ldrb	\reg1, [r0], #1
		.endm

		.macro	load2b, reg1, reg2
		ldrb	\reg1, [r0], #1
		ldrb	\reg2, [r0], #1
		.endm

		.macro	load1l, reg1
		ldr	\reg1, [r0], #4
		.endm

		.macro	load2l, reg1, reg2
		ldr	\reg1, [r0], #4
		ldr	\reg2, [r0], #4
		.endm

		.macro	load4l, reg1, reg2, reg3, reg4
		ldmia	r0!, {\reg1, \reg2, \reg3, \reg4}
		.endm

#define FN_ENTRY	ENTRY(csum_partial_copy_nocheck)
#define FN_EXIT		ENDPROC(csum_partial_copy_nocheck)

#include "csumpartialcopygeneric.S"
