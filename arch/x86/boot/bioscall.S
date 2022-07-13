<<<<<<< HEAD
/* -----------------------------------------------------------------------
 *
 *   Copyright 2009 Intel Corporation; author H. Peter Anvin
 *
 *   This file is part of the Linux kernel, and is made available under
 *   the terms of the GNU General Public License version 2 or (at your
 *   option) any later version; incorporated herein by reference.
=======
/* SPDX-License-Identifier: GPL-2.0-or-later */
/* -----------------------------------------------------------------------
 *
 *   Copyright 2009-2014 Intel Corporation; author H. Peter Anvin
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 *
 * ----------------------------------------------------------------------- */

/*
 * "Glove box" for BIOS calls.  Avoids the constant problems with BIOSes
 * touching registers they shouldn't be.
 */

<<<<<<< HEAD
	.code16gcc
	.text
=======
	.code16
	.section ".inittext","ax"
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	.globl	intcall
	.type	intcall, @function
intcall:
	/* Self-modify the INT instruction.  Ugly, but works. */
	cmpb	%al, 3f
	je	1f
	movb	%al, 3f
	jmp	1f		/* Synchronize pipeline */
1:
	/* Save state */
	pushfl
	pushw	%fs
	pushw	%gs
	pushal

	/* Copy input state to stack frame */
	subw	$44, %sp
	movw	%dx, %si
	movw	%sp, %di
	movw	$11, %cx
<<<<<<< HEAD
	rep; movsd
=======
	rep; movsl
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	/* Pop full state from the stack */
	popal
	popw	%gs
	popw	%fs
	popw	%es
	popw	%ds
	popfl

	/* Actual INT */
	.byte	0xcd		/* INT opcode */
3:	.byte	0

	/* Push full state to the stack */
	pushfl
	pushw	%ds
	pushw	%es
	pushw	%fs
	pushw	%gs
	pushal

	/* Re-establish C environment invariants */
	cld
	movzwl	%sp, %esp
	movw	%cs, %ax
	movw	%ax, %ds
	movw	%ax, %es

	/* Copy output state from stack frame */
	movw	68(%esp), %di	/* Original %cx == 3rd argument */
	andw	%di, %di
	jz	4f
	movw	%sp, %si
	movw	$11, %cx
<<<<<<< HEAD
	rep; movsd
=======
	rep; movsl
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
4:	addw	$44, %sp

	/* Restore state and return */
	popal
	popw	%gs
	popw	%fs
	popfl
	retl
	.size	intcall, .-intcall
