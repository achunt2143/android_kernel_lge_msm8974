<<<<<<< HEAD
=======
/* SPDX-License-Identifier: GPL-2.0 */
#include <asm/assembler.h>
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#include <asm/unwind.h>

#if __LINUX_ARM_ARCH__ >= 6
	.macro	bitop, name, instr
ENTRY(	\name		)
UNWIND(	.fnstart	)
	ands	ip, r1, #3
<<<<<<< HEAD
	strneb	r1, [ip]		@ assert word-aligned
=======
	strbne	r1, [ip]		@ assert word-aligned
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	mov	r2, #1
	and	r3, r0, #31		@ Get bit offset
	mov	r0, r0, lsr #5
	add	r1, r1, r0, lsl #2	@ Get word offset
<<<<<<< HEAD
=======
#if __LINUX_ARM_ARCH__ >= 7 && defined(CONFIG_SMP)
	.arch_extension	mp
	ALT_SMP(W(pldw)	[r1])
	ALT_UP(W(nop))
#endif
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	mov	r3, r2, lsl r3
1:	ldrex	r2, [r1]
	\instr	r2, r2, r3
	strex	r0, r2, [r1]
	cmp	r0, #0
	bne	1b
	bx	lr
UNWIND(	.fnend		)
ENDPROC(\name		)
	.endm

<<<<<<< HEAD
	.macro	testop, name, instr, store
ENTRY(	\name		)
UNWIND(	.fnstart	)
	ands	ip, r1, #3
	strneb	r1, [ip]		@ assert word-aligned
=======
	.macro	__testop, name, instr, store, barrier
ENTRY(	\name		)
UNWIND(	.fnstart	)
	ands	ip, r1, #3
	strbne	r1, [ip]		@ assert word-aligned
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	mov	r2, #1
	and	r3, r0, #31		@ Get bit offset
	mov	r0, r0, lsr #5
	add	r1, r1, r0, lsl #2	@ Get word offset
	mov	r3, r2, lsl r3		@ create mask
<<<<<<< HEAD
	smp_dmb
=======
	\barrier
#if __LINUX_ARM_ARCH__ >= 7 && defined(CONFIG_SMP)
	.arch_extension	mp
	ALT_SMP(W(pldw)	[r1])
	ALT_UP(W(nop))
#endif
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
1:	ldrex	r2, [r1]
	ands	r0, r2, r3		@ save old value of bit
	\instr	r2, r2, r3		@ toggle bit
	strex	ip, r2, [r1]
	cmp	ip, #0
	bne	1b
<<<<<<< HEAD
	smp_dmb
=======
	\barrier
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	cmp	r0, #0
	movne	r0, #1
2:	bx	lr
UNWIND(	.fnend		)
ENDPROC(\name		)
	.endm
<<<<<<< HEAD
=======

	.macro	testop, name, instr, store
	__testop \name, \instr, \store, smp_dmb
	.endm

	.macro	sync_testop, name, instr, store
	__testop \name, \instr, \store, __smp_dmb
	.endm
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#else
	.macro	bitop, name, instr
ENTRY(	\name		)
UNWIND(	.fnstart	)
	ands	ip, r1, #3
<<<<<<< HEAD
	strneb	r1, [ip]		@ assert word-aligned
=======
	strbne	r1, [ip]		@ assert word-aligned
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	and	r2, r0, #31
	mov	r0, r0, lsr #5
	mov	r3, #1
	mov	r3, r3, lsl r2
	save_and_disable_irqs ip
	ldr	r2, [r1, r0, lsl #2]
	\instr	r2, r2, r3
	str	r2, [r1, r0, lsl #2]
	restore_irqs ip
<<<<<<< HEAD
	mov	pc, lr
=======
	ret	lr
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
UNWIND(	.fnend		)
ENDPROC(\name		)
	.endm

/**
 * testop - implement a test_and_xxx_bit operation.
 * @instr: operational instruction
 * @store: store instruction
 *
 * Note: we can trivially conditionalise the store instruction
 * to avoid dirtying the data cache.
 */
	.macro	testop, name, instr, store
ENTRY(	\name		)
UNWIND(	.fnstart	)
	ands	ip, r1, #3
<<<<<<< HEAD
	strneb	r1, [ip]		@ assert word-aligned
=======
	strbne	r1, [ip]		@ assert word-aligned
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	and	r3, r0, #31
	mov	r0, r0, lsr #5
	save_and_disable_irqs ip
	ldr	r2, [r1, r0, lsl #2]!
	mov	r0, #1
	tst	r2, r0, lsl r3
	\instr	r2, r2, r0, lsl r3
	\store	r2, [r1]
	moveq	r0, #0
	restore_irqs ip
<<<<<<< HEAD
	mov	pc, lr
=======
	ret	lr
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
UNWIND(	.fnend		)
ENDPROC(\name		)
	.endm
#endif
