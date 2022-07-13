<<<<<<< HEAD
=======
/* SPDX-License-Identifier: GPL-2.0-only */
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
/*
 *  linux/arch/arm/lib/copy_from_user.S
 *
 *  Author:	Nicolas Pitre
 *  Created:	Sep 29, 2005
 *  Copyright:	MontaVista Software, Inc.
<<<<<<< HEAD
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License version 2 as
 *  published by the Free Software Foundation.
=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 */

#include <linux/linkage.h>
#include <asm/assembler.h>
<<<<<<< HEAD
=======
#include <asm/unwind.h>
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

/*
 * Prototype:
 *
<<<<<<< HEAD
 *	size_t __copy_from_user(void *to, const void *from, size_t n)
=======
 *	size_t arm_copy_from_user(void *to, const void *from, size_t n)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 *
 * Purpose:
 *
 *	copy a block to kernel memory from user memory
 *
 * Params:
 *
 *	to = kernel memory
 *	from = user memory
 *	n = number of bytes to copy
 *
 * Return value:
 *
 *	Number of bytes NOT copied.
 */

<<<<<<< HEAD
=======
#ifdef CONFIG_CPU_USE_DOMAINS

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#ifndef CONFIG_THUMB2_KERNEL
#define LDR1W_SHIFT	0
#else
#define LDR1W_SHIFT	1
#endif
<<<<<<< HEAD
#define STR1W_SHIFT	0
=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	.macro ldr1w ptr reg abort
	ldrusr	\reg, \ptr, 4, abort=\abort
	.endm

	.macro ldr4w ptr reg1 reg2 reg3 reg4 abort
	ldr1w \ptr, \reg1, \abort
	ldr1w \ptr, \reg2, \abort
	ldr1w \ptr, \reg3, \abort
	ldr1w \ptr, \reg4, \abort
	.endm

	.macro ldr8w ptr reg1 reg2 reg3 reg4 reg5 reg6 reg7 reg8 abort
	ldr4w \ptr, \reg1, \reg2, \reg3, \reg4, \abort
	ldr4w \ptr, \reg5, \reg6, \reg7, \reg8, \abort
	.endm

<<<<<<< HEAD
=======
#else

#define LDR1W_SHIFT	0

	.macro ldr1w ptr reg abort
	USERL(\abort, W(ldr) \reg, [\ptr], #4)
	.endm

	.macro ldr4w ptr reg1 reg2 reg3 reg4 abort
	USERL(\abort, ldmia \ptr!, {\reg1, \reg2, \reg3, \reg4})
	.endm

	.macro ldr8w ptr reg1 reg2 reg3 reg4 reg5 reg6 reg7 reg8 abort
	USERL(\abort, ldmia \ptr!, {\reg1, \reg2, \reg3, \reg4, \reg5, \reg6, \reg7, \reg8})
	.endm

#endif /* CONFIG_CPU_USE_DOMAINS */

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	.macro ldr1b ptr reg cond=al abort
	ldrusr	\reg, \ptr, 1, \cond, abort=\abort
	.endm

<<<<<<< HEAD
=======
#define STR1W_SHIFT	0

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	.macro str1w ptr reg abort
	W(str) \reg, [\ptr], #4
	.endm

	.macro str8w ptr reg1 reg2 reg3 reg4 reg5 reg6 reg7 reg8 abort
	stmia \ptr!, {\reg1, \reg2, \reg3, \reg4, \reg5, \reg6, \reg7, \reg8}
	.endm

	.macro str1b ptr reg cond=al abort
<<<<<<< HEAD
	str\cond\()b \reg, [\ptr], #1
	.endm

	.macro enter reg1 reg2
	mov	r3, #0
	stmdb	sp!, {r0, r2, r3, \reg1, \reg2}
	.endm

	.macro exit reg1 reg2
	add	sp, sp, #8
	ldmfd	sp!, {r0, \reg1, \reg2}
=======
	strb\cond \reg, [\ptr], #1
	.endm

	.macro enter regs:vararg
	mov	r3, #0
UNWIND( .save	{r0, r2, r3, \regs}		)
	stmdb	sp!, {r0, r2, r3, \regs}
	.endm

	.macro exit regs:vararg
	add	sp, sp, #8
	ldmfd	sp!, {r0, \regs}
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	.endm

	.text

<<<<<<< HEAD
ENTRY(__copy_from_user)

#include "copy_template.S"

ENDPROC(__copy_from_user)

	.pushsection .fixup,"ax"
	.align 0
	copy_abort_preamble
	ldmfd	sp!, {r1, r2}
	sub	r3, r0, r1
	rsb	r1, r3, r2
	str	r1, [sp]
	bl	__memzero
	ldr	r0, [sp], #4
=======
ENTRY(arm_copy_from_user)
#ifdef CONFIG_CPU_SPECTRE
	ldr	r3, =TASK_SIZE
	uaccess_mask_range_ptr r1, r2, r3, ip
#endif

#include "copy_template.S"

ENDPROC(arm_copy_from_user)

	.pushsection .text.fixup,"ax"
	.align 0
	copy_abort_preamble
	ldmfd	sp!, {r1, r2, r3}
	sub	r0, r0, r1
	rsb	r0, r0, r2
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	copy_abort_end
	.popsection

