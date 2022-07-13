<<<<<<< HEAD
=======
/* SPDX-License-Identifier: GPL-2.0-only */
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
/*
 *  linux/arch/arm/kernel/debug.S
 *
 *  Copyright (C) 1994-1999 Russell King
 *
<<<<<<< HEAD
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 *
=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 *  32-bit debugging code
 */
#include <linux/linkage.h>
#include <asm/assembler.h>

		.text

/*
 * Some debugging routines (useful if you've got MM problems and
 * printk isn't working).  For DEBUGGING ONLY!!!  Do not leave
 * references to these in a production kernel!
 */

<<<<<<< HEAD
#if defined(CONFIG_DEBUG_ICEDCC)
		@@ debug using ARM EmbeddedICE DCC channel

		.macro	addruart, rp, rv, tmp
		.endm

#if defined(CONFIG_CPU_V6) || defined(CONFIG_CPU_V6K) || defined(CONFIG_CPU_V7)

		.macro	senduart, rd, rx
		mcr	p14, 0, \rd, c0, c5, 0
		.endm

		.macro	busyuart, rd, rx
1001:
		mrc	p14, 0, \rx, c0, c1, 0
		tst	\rx, #0x20000000
		beq	1001b
		.endm

		.macro	waituart, rd, rx
		mov	\rd, #0x2000000
1001:
		subs	\rd, \rd, #1
		bmi	1002f
		mrc	p14, 0, \rx, c0, c1, 0
		tst	\rx, #0x20000000
		bne	1001b
1002:
		.endm

#elif defined(CONFIG_CPU_XSCALE)

		.macro	senduart, rd, rx
		mcr	p14, 0, \rd, c8, c0, 0
		.endm

		.macro	busyuart, rd, rx
1001:
		mrc	p14, 0, \rx, c14, c0, 0
		tst	\rx, #0x10000000
		beq	1001b
		.endm

		.macro	waituart, rd, rx
		mov	\rd, #0x10000000
1001:
		subs	\rd, \rd, #1
		bmi	1002f
		mrc	p14, 0, \rx, c14, c0, 0
		tst	\rx, #0x10000000
		bne	1001b
1002:
		.endm

#else

		.macro	senduart, rd, rx
		mcr	p14, 0, \rd, c1, c0, 0
		.endm

		.macro	busyuart, rd, rx
1001:
		mrc	p14, 0, \rx, c0, c0, 0
		tst	\rx, #2
		beq	1001b

		.endm

		.macro	waituart, rd, rx
		mov	\rd, #0x2000000
1001:
		subs	\rd, \rd, #1
		bmi	1002f
		mrc	p14, 0, \rx, c0, c0, 0
		tst	\rx, #2
		bne	1001b
1002:
		.endm

#endif	/* CONFIG_CPU_V6 */

#elif !defined(CONFIG_DEBUG_SEMIHOSTING)
#include <mach/debug-macro.S>
#endif	/* CONFIG_DEBUG_ICEDCC */
=======
#if !defined(CONFIG_DEBUG_SEMIHOSTING)
#include CONFIG_DEBUG_LL_INCLUDE
#endif
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

#ifdef CONFIG_MMU
		.macro	addruart_current, rx, tmp1, tmp2
		addruart	\tmp1, \tmp2, \rx
		mrc		p15, 0, \rx, c1, c0
		tst		\rx, #1
		moveq		\rx, \tmp1
		movne		\rx, \tmp2
		.endm

#else /* !CONFIG_MMU */
		.macro	addruart_current, rx, tmp1, tmp2
<<<<<<< HEAD
		addruart	\rx, \tmp1
=======
		addruart	\rx, \tmp1, \tmp2
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		.endm

#endif /* CONFIG_MMU */

/*
 * Useful debugging routines
 */
ENTRY(printhex8)
		mov	r1, #8
		b	printhex
ENDPROC(printhex8)

ENTRY(printhex4)
		mov	r1, #4
		b	printhex
ENDPROC(printhex4)

ENTRY(printhex2)
		mov	r1, #2
<<<<<<< HEAD
printhex:	adr	r2, hexbuf
=======
printhex:	adr	r2, hexbuf_rel
		ldr	r3, [r2]
		add	r2, r2, r3
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		add	r3, r2, r1
		mov	r1, #0
		strb	r1, [r3]
1:		and	r1, r0, #15
		mov	r0, r0, lsr #4
		cmp	r1, #10
		addlt	r1, r1, #'0'
		addge	r1, r1, #'a' - 10
		strb	r1, [r3, #-1]!
		teq	r3, r2
		bne	1b
		mov	r0, r2
		b	printascii
ENDPROC(printhex2)

<<<<<<< HEAD
hexbuf:		.space 16
=======
		.pushsection .bss
hexbuf_addr:	.space 16
		.popsection
		.align
hexbuf_rel:	.long	hexbuf_addr - .
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

		.ltorg

#ifndef CONFIG_DEBUG_SEMIHOSTING

ENTRY(printascii)
		addruart_current r3, r1, r2
<<<<<<< HEAD
		b	2f
1:		waituart r2, r3
		senduart r1, r3
		busyuart r2, r3
		teq	r1, #'\n'
		moveq	r1, #'\r'
		beq	1b
2:		teq	r0, #0
		ldrneb	r1, [r0], #1
		teqne	r1, #0
		bne	1b
		mov	pc, lr
=======
1:		teq	r0, #0
		ldrbne	r1, [r0], #1
		teqne	r1, #0
		reteq	lr
2:		teq     r1, #'\n'
		bne	3f
		mov	r1, #'\r'
#ifdef CONFIG_DEBUG_UART_FLOW_CONTROL
		waituartcts r2, r3
#endif
		waituarttxrdy r2, r3
		senduart r1, r3
		busyuart r2, r3
		mov	r1, #'\n'
3:
#ifdef CONFIG_DEBUG_UART_FLOW_CONTROL
		waituartcts r2, r3
#endif
		waituarttxrdy r2, r3
		senduart r1, r3
		busyuart r2, r3
		b	1b
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
ENDPROC(printascii)

ENTRY(printch)
		addruart_current r3, r1, r2
		mov	r1, r0
		mov	r0, #0
<<<<<<< HEAD
		b	1b
ENDPROC(printch)

=======
		b	2b
ENDPROC(printch)

#ifdef CONFIG_MMU
ENTRY(debug_ll_addr)
		addruart r2, r3, ip
		str	r2, [r0]
		str	r3, [r1]
		ret	lr
ENDPROC(debug_ll_addr)
#endif

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#else

ENTRY(printascii)
		mov	r1, r0
		mov	r0, #0x04		@ SYS_WRITE0
	ARM(	svc	#0x123456	)
<<<<<<< HEAD
	THUMB(	svc	#0xab		)
		mov	pc, lr
ENDPROC(printascii)

ENTRY(printch)
		adr	r1, hexbuf
		strb	r0, [r1]
		mov	r0, #0x03		@ SYS_WRITEC
	ARM(	svc	#0x123456	)
	THUMB(	svc	#0xab		)
		mov	pc, lr
ENDPROC(printch)

=======
#ifdef CONFIG_CPU_V7M
	THUMB(	bkpt	#0xab		)
#else
	THUMB(	svc	#0xab		)
#endif
		ret	lr
ENDPROC(printascii)

ENTRY(printch)
		adr	r1, hexbuf_rel
		ldr	r2, [r1]
		add	r1, r1, r2
		strb	r0, [r1]
		mov	r0, #0x03		@ SYS_WRITEC
	ARM(	svc	#0x123456	)
#ifdef CONFIG_CPU_V7M
	THUMB(	bkpt	#0xab		)
#else
	THUMB(	svc	#0xab		)
#endif
		ret	lr
ENDPROC(printch)

ENTRY(debug_ll_addr)
		mov	r2, #0
		str	r2, [r0]
		str	r2, [r1]
		ret	lr
ENDPROC(debug_ll_addr)

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#endif
