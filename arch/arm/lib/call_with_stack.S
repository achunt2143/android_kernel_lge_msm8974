<<<<<<< HEAD
=======
/* SPDX-License-Identifier: GPL-2.0-only */
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
/*
 * arch/arm/lib/call_with_stack.S
 *
 * Copyright (C) 2011 ARM Ltd.
 * Written by Will Deacon <will.deacon@arm.com>
<<<<<<< HEAD
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307 USA
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
 * void call_with_stack(void (*fn)(void *), void *arg, void *sp)
 *
 * Change the stack to that pointed at by sp, then invoke fn(arg) with
 * the new stack.
<<<<<<< HEAD
 */
ENTRY(call_with_stack)
	str	sp, [r2, #-4]!
	str	lr, [r2, #-4]!

=======
 *
 * The sequence below follows the APCS frame convention for frame pointer
 * unwinding, and implements the unwinder annotations needed by the EABI
 * unwinder.
 */

ENTRY(call_with_stack)
#if defined(CONFIG_UNWINDER_FRAME_POINTER) && defined(CONFIG_CC_IS_GCC)
	mov	ip, sp
	push	{fp, ip, lr, pc}
	sub	fp, ip, #4
#else
UNWIND( .fnstart		)
UNWIND( .save	{fpreg, lr}	)
	push	{fpreg, lr}
UNWIND( .setfp	fpreg, sp	)
	mov	fpreg, sp
#endif
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	mov	sp, r2
	mov	r2, r0
	mov	r0, r1

<<<<<<< HEAD
	adr	lr, BSYM(1f)
	mov	pc, r2

1:	ldr	lr, [sp]
	ldr	sp, [sp, #4]
	mov	pc, lr
=======
	bl_r	r2

#if defined(CONFIG_UNWINDER_FRAME_POINTER) && defined(CONFIG_CC_IS_GCC)
	ldmdb	fp, {fp, sp, pc}
#else
	mov	sp, fpreg
	pop	{fpreg, pc}
UNWIND( .fnend			)
#endif
	.globl call_with_stack_end
call_with_stack_end:
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
ENDPROC(call_with_stack)
