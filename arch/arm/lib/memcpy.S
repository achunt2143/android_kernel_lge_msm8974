<<<<<<< HEAD
=======
/* SPDX-License-Identifier: GPL-2.0-only */
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
/*
 *  linux/arch/arm/lib/memcpy.S
 *
 *  Author:	Nicolas Pitre
 *  Created:	Sep 28, 2005
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

#define LDR1W_SHIFT	0
#define STR1W_SHIFT	0

	.macro ldr1w ptr reg abort
	W(ldr) \reg, [\ptr], #4
	.endm

	.macro ldr4w ptr reg1 reg2 reg3 reg4 abort
	ldmia \ptr!, {\reg1, \reg2, \reg3, \reg4}
	.endm

	.macro ldr8w ptr reg1 reg2 reg3 reg4 reg5 reg6 reg7 reg8 abort
	ldmia \ptr!, {\reg1, \reg2, \reg3, \reg4, \reg5, \reg6, \reg7, \reg8}
	.endm

	.macro ldr1b ptr reg cond=al abort
<<<<<<< HEAD
	ldr\cond\()b \reg, [\ptr], #1
=======
	ldrb\cond \reg, [\ptr], #1
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	.endm

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
	stmdb sp!, {r0, \reg1, \reg2}
	.endm

	.macro exit reg1 reg2
	ldmfd sp!, {r0, \reg1, \reg2}
=======
	strb\cond \reg, [\ptr], #1
	.endm

	.macro enter regs:vararg
UNWIND( .save	{r0, \regs}		)
	stmdb sp!, {r0, \regs}
	.endm

	.macro exit regs:vararg
	ldmfd sp!, {r0, \regs}
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	.endm

	.text

/* Prototype: void *memcpy(void *dest, const void *src, size_t n); */

<<<<<<< HEAD
ENTRY(memcpy)
=======
ENTRY(__memcpy)
ENTRY(mmiocpy)
WEAK(memcpy)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

#include "copy_template.S"

ENDPROC(memcpy)
<<<<<<< HEAD
=======
ENDPROC(mmiocpy)
ENDPROC(__memcpy)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
