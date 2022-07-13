<<<<<<< HEAD
=======
/* SPDX-License-Identifier: GPL-2.0-only */
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
/*
 *  linux/arch/arm/lib/memset.S
 *
 *  Copyright (C) 1995-2000 Russell King
 *
<<<<<<< HEAD
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 *
=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 *  ASM optimised string functions
 */
#include <linux/linkage.h>
#include <asm/assembler.h>
<<<<<<< HEAD
=======
#include <asm/unwind.h>
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	.text
	.align	5

<<<<<<< HEAD
ENTRY(memset)
=======
ENTRY(__memset)
ENTRY(mmioset)
WEAK(memset)
UNWIND( .fnstart         )
	and	r1, r1, #255		@ cast to unsigned char
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	ands	r3, r0, #3		@ 1 unaligned?
	mov	ip, r0			@ preserve r0 as return value
	bne	6f			@ 1
/*
 * we know that the pointer in ip is aligned to a word boundary.
 */
1:	orr	r1, r1, r1, lsl #8
	orr	r1, r1, r1, lsl #16
	mov	r3, r1
<<<<<<< HEAD
	cmp	r2, #16
	blt	4f
=======
7:	cmp	r2, #16
	blt	4f
UNWIND( .fnend              )
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

#if ! CALGN(1)+0

/*
 * We need 2 extra registers for this loop - use r8 and the LR
 */
<<<<<<< HEAD
	stmfd	sp!, {r8, lr}
	mov	r8, r1
	mov	lr, r1

2:	subs	r2, r2, #64
	stmgeia	ip!, {r1, r3, r8, lr}	@ 64 bytes at a time.
	stmgeia	ip!, {r1, r3, r8, lr}
	stmgeia	ip!, {r1, r3, r8, lr}
	stmgeia	ip!, {r1, r3, r8, lr}
	bgt	2b
	ldmeqfd	sp!, {r8, pc}		@ Now <64 bytes to go.
=======
UNWIND( .fnstart            )
UNWIND( .save {r8, lr}      )
	stmfd	sp!, {r8, lr}
	mov	r8, r1
	mov	lr, r3

2:	subs	r2, r2, #64
	stmiage	ip!, {r1, r3, r8, lr}	@ 64 bytes at a time.
	stmiage	ip!, {r1, r3, r8, lr}
	stmiage	ip!, {r1, r3, r8, lr}
	stmiage	ip!, {r1, r3, r8, lr}
	bgt	2b
	ldmfdeq	sp!, {r8, pc}		@ Now <64 bytes to go.
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
/*
 * No need to correct the count; we're only testing bits from now on
 */
	tst	r2, #32
<<<<<<< HEAD
	stmneia	ip!, {r1, r3, r8, lr}
	stmneia	ip!, {r1, r3, r8, lr}
	tst	r2, #16
	stmneia	ip!, {r1, r3, r8, lr}
	ldmfd	sp!, {r8, lr}
=======
	stmiane	ip!, {r1, r3, r8, lr}
	stmiane	ip!, {r1, r3, r8, lr}
	tst	r2, #16
	stmiane	ip!, {r1, r3, r8, lr}
	ldmfd	sp!, {r8, lr}
UNWIND( .fnend              )
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

#else

/*
 * This version aligns the destination pointer in order to write
 * whole cache lines at once.
 */

<<<<<<< HEAD
	stmfd	sp!, {r4-r8, lr}
	mov	r4, r1
	mov	r5, r1
	mov	r6, r1
	mov	r7, r1
	mov	r8, r1
	mov	lr, r1
=======
UNWIND( .fnstart               )
UNWIND( .save {r4-r8, lr}      )
	stmfd	sp!, {r4-r8, lr}
	mov	r4, r1
	mov	r5, r3
	mov	r6, r1
	mov	r7, r3
	mov	r8, r1
	mov	lr, r3
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	cmp	r2, #96
	tstgt	ip, #31
	ble	3f

	and	r8, ip, #31
	rsb	r8, r8, #32
	sub	r2, r2, r8
	movs	r8, r8, lsl #(32 - 4)
<<<<<<< HEAD
	stmcsia	ip!, {r4, r5, r6, r7}
	stmmiia	ip!, {r4, r5}
=======
	stmiacs	ip!, {r4, r5, r6, r7}
	stmiami	ip!, {r4, r5}
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	tst	r8, #(1 << 30)
	mov	r8, r1
	strne	r1, [ip], #4

3:	subs	r2, r2, #64
<<<<<<< HEAD
	stmgeia	ip!, {r1, r3-r8, lr}
	stmgeia	ip!, {r1, r3-r8, lr}
	bgt	3b
	ldmeqfd	sp!, {r4-r8, pc}

	tst	r2, #32
	stmneia	ip!, {r1, r3-r8, lr}
	tst	r2, #16
	stmneia	ip!, {r4-r7}
	ldmfd	sp!, {r4-r8, lr}

#endif

4:	tst	r2, #8
	stmneia	ip!, {r1, r3}
	tst	r2, #4
	strne	r1, [ip], #4
/*
 * When we get here, we've got less than 4 bytes to zero.  We
 * may have an unaligned pointer as well.
 */
5:	tst	r2, #2
	strneb	r1, [ip], #1
	strneb	r1, [ip], #1
	tst	r2, #1
	strneb	r1, [ip], #1
	mov	pc, lr
=======
	stmiage	ip!, {r1, r3-r8, lr}
	stmiage	ip!, {r1, r3-r8, lr}
	bgt	3b
	ldmfdeq	sp!, {r4-r8, pc}

	tst	r2, #32
	stmiane	ip!, {r1, r3-r8, lr}
	tst	r2, #16
	stmiane	ip!, {r4-r7}
	ldmfd	sp!, {r4-r8, lr}
UNWIND( .fnend                 )

#endif

UNWIND( .fnstart            )
4:	tst	r2, #8
	stmiane	ip!, {r1, r3}
	tst	r2, #4
	strne	r1, [ip], #4
/*
 * When we get here, we've got less than 4 bytes to set.  We
 * may have an unaligned pointer as well.
 */
5:	tst	r2, #2
	strbne	r1, [ip], #1
	strbne	r1, [ip], #1
	tst	r2, #1
	strbne	r1, [ip], #1
	ret	lr
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

6:	subs	r2, r2, #4		@ 1 do we have enough
	blt	5b			@ 1 bytes to align with?
	cmp	r3, #2			@ 1
<<<<<<< HEAD
	strltb	r1, [ip], #1		@ 1
	strleb	r1, [ip], #1		@ 1
	strb	r1, [ip], #1		@ 1
	add	r2, r2, r3		@ 1 (r2 = r2 - (4 - r3))
	b	1b
ENDPROC(memset)
=======
	strblt	r1, [ip], #1		@ 1
	strble	r1, [ip], #1		@ 1
	strb	r1, [ip], #1		@ 1
	add	r2, r2, r3		@ 1 (r2 = r2 - (4 - r3))
	b	1b
UNWIND( .fnend   )
ENDPROC(memset)
ENDPROC(mmioset)
ENDPROC(__memset)

ENTRY(__memset32)
UNWIND( .fnstart         )
	mov	r3, r1			@ copy r1 to r3 and fall into memset64
UNWIND( .fnend   )
ENDPROC(__memset32)
ENTRY(__memset64)
UNWIND( .fnstart         )
	mov	ip, r0			@ preserve r0 as return value
	b	7b			@ jump into the middle of memset
UNWIND( .fnend   )
ENDPROC(__memset64)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
