/*
 * This file is subject to the terms and conditions of the GNU General Public
 * License.  See the file "COPYING" in the main directory of this archive
 * for more details.
 *
 * Copyright (C) 1995, 1996, 1997, 1999, 2001 by Ralf Baechle
 * Copyright (C) 1999 by Silicon Graphics, Inc.
 * Copyright (C) 2001 MIPS Technologies, Inc.
 * Copyright (C) 2002  Maciej W. Rozycki
 *
 * Some useful macros for MIPS assembler code
 *
 * Some of the routines below contain useless nops that will be optimized
 * away by gas in -O mode. These nops are however required to fill delay
 * slots in noreorder mode.
 */
#ifndef __ASM_ASM_H
#define __ASM_ASM_H

#include <asm/sgidefs.h>
<<<<<<< HEAD

#ifndef CAT
#ifdef __STDC__
#define __CAT(str1, str2) str1##str2
#else
#define __CAT(str1, str2) str1/**/str2
#endif
#define CAT(str1, str2) __CAT(str1, str2)
#endif

/*
 * PIC specific declarations
 * Not used for the kernel but here seems to be the right place.
 */
#ifdef __PIC__
#define CPRESTORE(register)                             \
		.cprestore register
#define CPADD(register)                                 \
		.cpadd	register
#define CPLOAD(register)                                \
		.cpload	register
#else
#define CPRESTORE(register)
#define CPADD(register)
#define CPLOAD(register)
=======
#include <asm/asm-eva.h>
#include <asm/isa-rev.h>

#ifndef __VDSO__
/*
 * Emit CFI data in .debug_frame sections, not .eh_frame sections.
 * We don't do DWARF unwinding at runtime, so only the offline DWARF
 * information is useful to anyone. Note we should change this if we
 * ever decide to enable DWARF unwinding at runtime.
 */
#define CFI_SECTIONS	.cfi_sections .debug_frame
#else
 /*
  * For the vDSO, emit both runtime unwind information and debug
  * symbols for the .dbg file.
  */
#define CFI_SECTIONS
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#endif

/*
 * LEAF - declare leaf routine
 */
<<<<<<< HEAD
#define	LEAF(symbol)                                    \
		.globl	symbol;                         \
		.align	2;                              \
		.type	symbol, @function;              \
		.ent	symbol, 0;                      \
symbol:		.frame	sp, 0, ra
=======
#define LEAF(symbol)					\
		CFI_SECTIONS;				\
		.globl	symbol;				\
		.align	2;				\
		.type	symbol, @function;		\
		.ent	symbol, 0;			\
symbol:		.frame	sp, 0, ra;			\
		.cfi_startproc;				\
		.insn
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

/*
 * NESTED - declare nested routine entry point
 */
<<<<<<< HEAD
#define	NESTED(symbol, framesize, rpc)                  \
		.globl	symbol;                         \
		.align	2;                              \
		.type	symbol, @function;              \
		.ent	symbol, 0;                       \
symbol:		.frame	sp, framesize, rpc
=======
#define NESTED(symbol, framesize, rpc)			\
		CFI_SECTIONS;				\
		.globl	symbol;				\
		.align	2;				\
		.type	symbol, @function;		\
		.ent	symbol, 0;			\
symbol:		.frame	sp, framesize, rpc;		\
		.cfi_startproc;				\
		.insn
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

/*
 * END - mark end of function
 */
<<<<<<< HEAD
#define	END(function)                                   \
		.end	function;		        \
=======
#define END(function)					\
		.cfi_endproc;				\
		.end	function;			\
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		.size	function, .-function

/*
 * EXPORT - export definition of symbol
 */
#define EXPORT(symbol)					\
<<<<<<< HEAD
		.globl	symbol;                         \
=======
		.globl	symbol;				\
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
symbol:

/*
 * FEXPORT - export definition of a function symbol
 */
#define FEXPORT(symbol)					\
		.globl	symbol;				\
		.type	symbol, @function;		\
<<<<<<< HEAD
symbol:
=======
symbol:		.insn
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

/*
 * ABS - export absolute symbol
 */
<<<<<<< HEAD
#define	ABS(symbol,value)                               \
		.globl	symbol;                         \
symbol		=	value

#define	PANIC(msg)                                      \
		.set	push;				\
		.set	reorder;                        \
		PTR_LA	a0, 8f;                          \
		jal	panic;                          \
9:		b	9b;                             \
=======
#define ABS(symbol,value)				\
		.globl	symbol;				\
symbol		=	value

#define TEXT(msg)					\
		.pushsection .data;			\
8:		.asciiz msg;				\
		.popsection;

#define ASM_PANIC(msg)					\
		.set	push;				\
		.set	reorder;			\
		PTR_LA	a0, 8f;				\
		jal	panic;				\
9:		b	9b;				\
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		.set	pop;				\
		TEXT(msg)

/*
 * Print formatted string
 */
#ifdef CONFIG_PRINTK
<<<<<<< HEAD
#define PRINT(string)                                   \
		.set	push;				\
		.set	reorder;                        \
		PTR_LA	a0, 8f;                          \
		jal	printk;                         \
		.set	pop;				\
		TEXT(string)
#else
#define PRINT(string)
#endif

#define	TEXT(msg)                                       \
		.pushsection .data;			\
8:		.asciiz	msg;                            \
		.popsection;

/*
 * Build text tables
 */
#define TTABLE(string)                                  \
		.pushsection .text;			\
		.word	1f;                             \
		.popsection				\
		.pushsection .data;			\
1:		.asciiz	string;                         \
		.popsection

/*
 * MIPS IV pref instruction.
 * Use with .set noreorder only!
 *
 * MIPS IV implementations are free to treat this as a nop.  The R5000
 * is one of them.  So we should have an option not to use this instruction.
 */
#ifdef CONFIG_CPU_HAS_PREFETCH

#define PREF(hint,addr)                                 \
		.set	push;				\
		.set	mips4;				\
		pref	hint, addr;			\
		.set	pop

#define PREFX(hint,addr)                                \
		.set	push;				\
		.set	mips4;				\
		prefx	hint, addr;			\
		.set	pop

#else /* !CONFIG_CPU_HAS_PREFETCH */

#define PREF(hint, addr)
#define PREFX(hint, addr)

#endif /* !CONFIG_CPU_HAS_PREFETCH */

/*
 * MIPS ISA IV/V movn/movz instructions and equivalents for older CPUs.
 */
#if (_MIPS_ISA == _MIPS_ISA_MIPS1)
#define MOVN(rd, rs, rt)                                \
		.set	push;				\
		.set	reorder;			\
		beqz	rt, 9f;                         \
		move	rd, rs;                         \
		.set	pop;				\
9:
#define MOVZ(rd, rs, rt)                                \
		.set	push;				\
		.set	reorder;			\
		bnez	rt, 9f;                         \
		move	rd, rs;                         \
		.set	pop;				\
9:
#endif /* _MIPS_ISA == _MIPS_ISA_MIPS1 */
#if (_MIPS_ISA == _MIPS_ISA_MIPS2) || (_MIPS_ISA == _MIPS_ISA_MIPS3)
#define MOVN(rd, rs, rt)                                \
		.set	push;				\
		.set	noreorder;			\
		bnezl	rt, 9f;                         \
		 move	rd, rs;                         \
		.set	pop;				\
9:
#define MOVZ(rd, rs, rt)                                \
		.set	push;				\
		.set	noreorder;			\
		beqzl	rt, 9f;                         \
		 move	rd, rs;                         \
		.set	pop;				\
9:
#endif /* (_MIPS_ISA == _MIPS_ISA_MIPS2) || (_MIPS_ISA == _MIPS_ISA_MIPS3) */
#if (_MIPS_ISA == _MIPS_ISA_MIPS4 ) || (_MIPS_ISA == _MIPS_ISA_MIPS5) || \
    (_MIPS_ISA == _MIPS_ISA_MIPS32) || (_MIPS_ISA == _MIPS_ISA_MIPS64)
#define MOVN(rd, rs, rt)                                \
		movn	rd, rs, rt
#define MOVZ(rd, rs, rt)                                \
		movz	rd, rs, rt
#endif /* MIPS IV, MIPS V, MIPS32 or MIPS64 */

=======
#define ASM_PRINT(string)				\
		.set	push;				\
		.set	reorder;			\
		PTR_LA	a0, 8f;				\
		jal	_printk;			\
		.set	pop;				\
		TEXT(string)
#else
#define ASM_PRINT(string)
#endif

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
/*
 * Stack alignment
 */
#if (_MIPS_SIM == _MIPS_SIM_ABI32)
#define ALSZ	7
#define ALMASK	~7
#endif
#if (_MIPS_SIM == _MIPS_SIM_NABI32) || (_MIPS_SIM == _MIPS_SIM_ABI64)
#define ALSZ	15
#define ALMASK	~15
#endif

/*
 * Macros to handle different pointer/register sizes for 32/64-bit code
 */

/*
 * Size of a register
 */
#ifdef __mips64
#define SZREG	8
#else
#define SZREG	4
#endif

/*
 * Use the following macros in assemblercode to load/store registers,
 * pointers etc.
 */
#if (_MIPS_SIM == _MIPS_SIM_ABI32)
#define REG_S		sw
#define REG_L		lw
#define REG_SUBU	subu
#define REG_ADDU	addu
#endif
#if (_MIPS_SIM == _MIPS_SIM_NABI32) || (_MIPS_SIM == _MIPS_SIM_ABI64)
#define REG_S		sd
#define REG_L		ld
#define REG_SUBU	dsubu
#define REG_ADDU	daddu
#endif

/*
 * How to add/sub/load/store/shift C int variables.
 */
#if (_MIPS_SZINT == 32)
#define INT_ADD		add
#define INT_ADDU	addu
#define INT_ADDI	addi
#define INT_ADDIU	addiu
#define INT_SUB		sub
#define INT_SUBU	subu
#define INT_L		lw
#define INT_S		sw
#define INT_SLL		sll
#define INT_SLLV	sllv
#define INT_SRL		srl
#define INT_SRLV	srlv
#define INT_SRA		sra
#define INT_SRAV	srav
#endif

#if (_MIPS_SZINT == 64)
#define INT_ADD		dadd
#define INT_ADDU	daddu
#define INT_ADDI	daddi
#define INT_ADDIU	daddiu
#define INT_SUB		dsub
#define INT_SUBU	dsubu
#define INT_L		ld
#define INT_S		sd
#define INT_SLL		dsll
#define INT_SLLV	dsllv
#define INT_SRL		dsrl
#define INT_SRLV	dsrlv
#define INT_SRA		dsra
#define INT_SRAV	dsrav
#endif

/*
 * How to add/sub/load/store/shift C long variables.
 */
#if (_MIPS_SZLONG == 32)
#define LONG_ADD	add
#define LONG_ADDU	addu
#define LONG_ADDI	addi
#define LONG_ADDIU	addiu
#define LONG_SUB	sub
#define LONG_SUBU	subu
#define LONG_L		lw
<<<<<<< HEAD
#define LONG_S		sw
=======
#define LONG_LL		ll
#define LONG_SC		sc
#define LONG_S		sw
#define LONG_SP		swp
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#define LONG_SLL	sll
#define LONG_SLLV	sllv
#define LONG_SRL	srl
#define LONG_SRLV	srlv
#define LONG_SRA	sra
#define LONG_SRAV	srav
<<<<<<< HEAD

#define LONG		.word
=======
#define LONG_INS	ins
#define LONG_EXT	ext

#ifdef __ASSEMBLY__
#define LONG		.word
#endif
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#define LONGSIZE	4
#define LONGMASK	3
#define LONGLOG		2
#endif

#if (_MIPS_SZLONG == 64)
#define LONG_ADD	dadd
#define LONG_ADDU	daddu
#define LONG_ADDI	daddi
#define LONG_ADDIU	daddiu
#define LONG_SUB	dsub
#define LONG_SUBU	dsubu
#define LONG_L		ld
<<<<<<< HEAD
#define LONG_S		sd
=======
#define LONG_LL		lld
#define LONG_SC		scd
#define LONG_S		sd
#define LONG_SP		sdp
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#define LONG_SLL	dsll
#define LONG_SLLV	dsllv
#define LONG_SRL	dsrl
#define LONG_SRLV	dsrlv
#define LONG_SRA	dsra
#define LONG_SRAV	dsrav
<<<<<<< HEAD

#define LONG		.dword
=======
#define LONG_INS	dins
#define LONG_EXT	dext

#ifdef __ASSEMBLY__
#define LONG		.dword
#endif
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#define LONGSIZE	8
#define LONGMASK	7
#define LONGLOG		3
#endif

/*
 * How to add/sub/load/store/shift pointers.
 */
#if (_MIPS_SZPTR == 32)
#define PTR_ADD		add
#define PTR_ADDU	addu
#define PTR_ADDI	addi
#define PTR_ADDIU	addiu
#define PTR_SUB		sub
#define PTR_SUBU	subu
#define PTR_L		lw
#define PTR_S		sw
#define PTR_LA		la
#define PTR_LI		li
#define PTR_SLL		sll
#define PTR_SLLV	sllv
#define PTR_SRL		srl
#define PTR_SRLV	srlv
#define PTR_SRA		sra
#define PTR_SRAV	srav

#define PTR_SCALESHIFT	2

<<<<<<< HEAD
#define PTR		.word
=======
#define PTR_WD		.word
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#define PTRSIZE		4
#define PTRLOG		2
#endif

#if (_MIPS_SZPTR == 64)
#define PTR_ADD		dadd
#define PTR_ADDU	daddu
#define PTR_ADDI	daddi
#define PTR_ADDIU	daddiu
#define PTR_SUB		dsub
#define PTR_SUBU	dsubu
#define PTR_L		ld
#define PTR_S		sd
#define PTR_LA		dla
#define PTR_LI		dli
#define PTR_SLL		dsll
#define PTR_SLLV	dsllv
#define PTR_SRL		dsrl
#define PTR_SRLV	dsrlv
#define PTR_SRA		dsra
#define PTR_SRAV	dsrav

#define PTR_SCALESHIFT	3

<<<<<<< HEAD
#define PTR		.dword
=======
#define PTR_WD		.dword
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#define PTRSIZE		8
#define PTRLOG		3
#endif

/*
 * Some cp0 registers were extended to 64bit for MIPS III.
 */
#if (_MIPS_SIM == _MIPS_SIM_ABI32)
#define MFC0		mfc0
#define MTC0		mtc0
#endif
#if (_MIPS_SIM == _MIPS_SIM_NABI32) || (_MIPS_SIM == _MIPS_SIM_ABI64)
#define MFC0		dmfc0
#define MTC0		dmtc0
#endif

#define SSNOP		sll zero, zero, 1

<<<<<<< HEAD
=======
/*
 * Using a branch-likely instruction to check the result of an sc instruction
 * works around a bug present in R10000 CPUs prior to revision 3.0 that could
 * cause ll-sc sequences to execute non-atomically.
 */
#ifdef CONFIG_WAR_R10000_LLSC
# define SC_BEQZ	beqzl
#elif !defined(CONFIG_CC_HAS_BROKEN_INLINE_COMPAT_BRANCH) && MIPS_ISA_REV >= 6
# define SC_BEQZ	beqzc
#else
# define SC_BEQZ	beqz
#endif

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#ifdef CONFIG_SGI_IP28
/* Inhibit speculative stores to volatile (e.g.DMA) or invalid addresses. */
#include <asm/cacheops.h>
#define R10KCBARRIER(addr)  cache   Cache_Barrier, addr;
#else
#define R10KCBARRIER(addr)
#endif

#endif /* __ASM_ASM_H */
