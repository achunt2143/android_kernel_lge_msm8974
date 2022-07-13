<<<<<<< HEAD
/*
 *  arch/s390/kernel/reipl.S
 *
 *  S390 version
 *    Copyright (C) 2000 IBM Deutschland Entwicklung GmbH, IBM Corporation
 *    Author(s): Holger Smolinski (Holger.Smolinski@de.ibm.com)
=======
/* SPDX-License-Identifier: GPL-2.0 */
/*
 *    Copyright IBM Corp 2000, 2011
 *    Author(s): Holger Smolinski <Holger.Smolinski@de.ibm.com>,
 *		 Denis Joseph Barrow,
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 */

#include <linux/linkage.h>
#include <asm/asm-offsets.h>
<<<<<<< HEAD

#
# store_status: Empty implementation until kdump is supported on 31 bit
#
ENTRY(store_status)
		br	%r14

#
# do_reipl_asm
# Parameter: r2 = schid of reipl device
#
ENTRY(do_reipl_asm)
		basr	%r13,0
.Lpg0:		lpsw	.Lnewpsw-.Lpg0(%r13)
.Lpg1:		# do store status of all registers

		stm	%r0,%r15,__LC_GPREGS_SAVE_AREA
		stctl	%c0,%c15,__LC_CREGS_SAVE_AREA
		stam	%a0,%a15,__LC_AREGS_SAVE_AREA
		l	%r10,.Ldump_pfx-.Lpg0(%r13)
		mvc	__LC_PREFIX_SAVE_AREA(4),0(%r10)
		stckc	.Lclkcmp-.Lpg0(%r13)
		mvc	__LC_CLOCK_COMP_SAVE_AREA(8),.Lclkcmp-.Lpg0(%r13)
		stpt	__LC_CPU_TIMER_SAVE_AREA
		st	%r13, __LC_PSW_SAVE_AREA+4
		lctl	%c6,%c6,.Lall-.Lpg0(%r13)
		lr	%r1,%r2
		mvc	__LC_PGM_NEW_PSW(8),.Lpcnew-.Lpg0(%r13)
		stsch	.Lschib-.Lpg0(%r13)
		oi	.Lschib+5-.Lpg0(%r13),0x84
.Lecs:  	xi	.Lschib+27-.Lpg0(%r13),0x01
		msch	.Lschib-.Lpg0(%r13)
		lhi	%r0,5
.Lssch:		ssch	.Liplorb-.Lpg0(%r13)
		jz	.L001
		brct	%r0,.Lssch
		bas	%r14,.Ldisab-.Lpg0(%r13)
.L001:		mvc	__LC_IO_NEW_PSW(8),.Lionew-.Lpg0(%r13)
.Ltpi:		lpsw	.Lwaitpsw-.Lpg0(%r13)
.Lcont:		c	%r1,__LC_SUBCHANNEL_ID
		jnz	.Ltpi
		clc	__LC_IO_INT_PARM(4),.Liplorb-.Lpg0(%r13)
		jnz	.Ltpi
		tsch	.Liplirb-.Lpg0(%r13)
		tm	.Liplirb+9-.Lpg0(%r13),0xbf
		jz	.L002
		bas	%r14,.Ldisab-.Lpg0(%r13)
.L002:		tm	.Liplirb+8-.Lpg0(%r13),0xf3
		jz	.L003
		bas	%r14,.Ldisab-.Lpg0(%r13)
.L003:		st	%r1,__LC_SUBCHANNEL_ID
		lpsw	0
		sigp	0,0,0(6)
.Ldisab:	st	%r14,.Ldispsw+4-.Lpg0(%r13)
		lpsw	.Ldispsw-.Lpg0(%r13)
		.align	8
.Lclkcmp:	.quad	0x0000000000000000
.Lall:		.long	0xff000000
.Ldump_pfx:	.long	dump_prefix_page
		.align	8
.Lnewpsw:	.long	0x00080000,0x80000000+.Lpg1
.Lpcnew:	.long	0x00080000,0x80000000+.Lecs
.Lionew:	.long	0x00080000,0x80000000+.Lcont
.Lwaitpsw:	.long	0x020a0000,0x00000000+.Ltpi
.Ldispsw:	.long	0x000a0000,0x00000000
.Liplccws:	.long	0x02000000,0x60000018
		.long	0x08000008,0x20000001
.Liplorb:	.long	0x0049504c,0x0040ff80
		.long	0x00000000+.Liplccws
.Lschib:	.long	0x00000000,0x00000000
		.long	0x00000000,0x00000000
		.long	0x00000000,0x00000000
		.long	0x00000000,0x00000000
		.long	0x00000000,0x00000000
		.long	0x00000000,0x00000000
.Liplirb:	.long	0x00000000,0x00000000
		.long	0x00000000,0x00000000
		.long	0x00000000,0x00000000
		.long	0x00000000,0x00000000
		.long	0x00000000,0x00000000
		.long	0x00000000,0x00000000
		.long	0x00000000,0x00000000
		.long	0x00000000,0x00000000
=======
#include <asm/nospec-insn.h>
#include <asm/sigp.h>

	GEN_BR_THUNK %r9

#
# Issue "store status" for the current CPU to its prefix page
# and call passed function afterwards
#
# r2 = Function to be called after store status
# r3 = Parameter for function
#
SYM_CODE_START(store_status)
	/* Save register one and load save area base */
	stg	%r1,__LC_SAVE_AREA_RESTART
	/* General purpose registers */
	lghi	%r1,__LC_GPREGS_SAVE_AREA
	stmg	%r0,%r15,0(%r1)
	mvc	8(8,%r1),__LC_SAVE_AREA_RESTART
	/* Control registers */
	lghi	%r1,__LC_CREGS_SAVE_AREA
	stctg	%c0,%c15,0(%r1)
	/* Access registers */
	lghi	%r1,__LC_AREGS_SAVE_AREA
	stam	%a0,%a15,0(%r1)
	/* Floating point registers */
	lghi	%r1,__LC_FPREGS_SAVE_AREA
	std	%f0, 0x00(%r1)
	std	%f1, 0x08(%r1)
	std	%f2, 0x10(%r1)
	std	%f3, 0x18(%r1)
	std	%f4, 0x20(%r1)
	std	%f5, 0x28(%r1)
	std	%f6, 0x30(%r1)
	std	%f7, 0x38(%r1)
	std	%f8, 0x40(%r1)
	std	%f9, 0x48(%r1)
	std	%f10,0x50(%r1)
	std	%f11,0x58(%r1)
	std	%f12,0x60(%r1)
	std	%f13,0x68(%r1)
	std	%f14,0x70(%r1)
	std	%f15,0x78(%r1)
	/* Floating point control register */
	lghi	%r1,__LC_FP_CREG_SAVE_AREA
	stfpc	0(%r1)
	/* CPU timer */
	lghi	%r1,__LC_CPU_TIMER_SAVE_AREA
	stpt	0(%r1)
	/* Store prefix register */
	lghi	%r1,__LC_PREFIX_SAVE_AREA
	stpx	0(%r1)
	/* Clock comparator - seven bytes */
	lghi	%r1,__LC_CLOCK_COMP_SAVE_AREA
	larl	%r4,clkcmp
	stckc	0(%r4)
	mvc	1(7,%r1),1(%r4)
	/* Program status word */
	lghi	%r1,__LC_PSW_SAVE_AREA
	epsw	%r4,%r5
	st	%r4,0(%r1)
	st	%r5,4(%r1)
	stg	%r2,8(%r1)
	lgr	%r9,%r2
	lgr	%r2,%r3
	BR_EX	%r9
SYM_CODE_END(store_status)

	.section .bss
	.balign	8
SYM_DATA_LOCAL(clkcmp,	.quad 0x0000000000000000)
	.previous
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
