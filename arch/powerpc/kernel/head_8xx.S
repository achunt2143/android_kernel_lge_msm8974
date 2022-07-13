<<<<<<< HEAD
=======
/* SPDX-License-Identifier: GPL-2.0-or-later */
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
/*
 *  PowerPC version
 *    Copyright (C) 1995-1996 Gary Thomas (gdt@linuxppc.org)
 *  Rewritten by Cort Dougan (cort@cs.nmt.edu) for PReP
 *    Copyright (C) 1996 Cort Dougan <cort@cs.nmt.edu>
 *  Low-level exception handlers and MMU support
 *  rewritten by Paul Mackerras.
 *    Copyright (C) 1996 Paul Mackerras.
 *  MPC8xx modifications by Dan Malek
 *    Copyright (C) 1997 Dan Malek (dmalek@jlc.net).
 *
 *  This file contains low-level support and setup for PowerPC 8xx
 *  embedded processors, including trap and interrupt dispatch.
<<<<<<< HEAD
 *
 *  This program is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU General Public License
 *  as published by the Free Software Foundation; either version
 *  2 of the License, or (at your option) any later version.
 *
 */

#include <linux/init.h>
=======
 */

#include <linux/init.h>
#include <linux/magic.h>
#include <linux/pgtable.h>
#include <linux/sizes.h>
#include <linux/linkage.h>

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#include <asm/processor.h>
#include <asm/page.h>
#include <asm/mmu.h>
#include <asm/cache.h>
<<<<<<< HEAD
#include <asm/pgtable.h>
=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#include <asm/cputable.h>
#include <asm/thread_info.h>
#include <asm/ppc_asm.h>
#include <asm/asm-offsets.h>
#include <asm/ptrace.h>
<<<<<<< HEAD

/* Macro to make the code more readable. */
#ifdef CONFIG_8xx_CPU6
#define DO_8xx_CPU6(val, reg)	\
	li	reg, val;	\
	stw	reg, 12(r0);	\
	lwz	reg, 12(r0);
#else
#define DO_8xx_CPU6(val, reg)
#endif
	__HEAD
_ENTRY(_stext);
_ENTRY(_start);
=======
#include <asm/code-patching-asm.h>
#include <asm/interrupt.h>

/*
 * Value for the bits that have fixed value in RPN entries.
 * Also used for tagging DAR for DTLBerror.
 */
#define RPN_PATTERN	0x00f0

#include "head_32.h"

.macro compare_to_kernel_boundary scratch, addr
#if CONFIG_TASK_SIZE <= 0x80000000 && CONFIG_PAGE_OFFSET >= 0x80000000
/* By simply checking Address >= 0x80000000, we know if its a kernel address */
	not.	\scratch, \addr
#else
	rlwinm	\scratch, \addr, 16, 0xfff8
	cmpli	cr0, \scratch, PAGE_OFFSET@h
#endif
.endm

#define PAGE_SHIFT_512K		19
#define PAGE_SHIFT_8M		23

	__HEAD
_GLOBAL(_stext);
_GLOBAL(_start);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

/* MPC8xx
 * This port was done on an MBX board with an 860.  Right now I only
 * support an ELF compressed (zImage) boot from EPPC-Bug because the
 * code there loads up some registers before calling us:
 *   r3: ptr to board info data
 *   r4: initrd_start or if no initrd then 0
 *   r5: initrd_end - unused if r4 is 0
 *   r6: Start of command line string
 *   r7: End of command line string
 *
 * I decided to use conditional compilation instead of checking PVR and
 * adding more processor specific branches around code I don't need.
 * Since this is an embedded processor, I also appreciate any memory
 * savings I can get.
 *
 * The MPC8xx does not have any BATs, but it supports large page sizes.
 * We first initialize the MMU to support 8M byte pages, then load one
 * entry into each of the instruction and data TLBs to map the first
 * 8M 1:1.  I also mapped an additional I/O space 1:1 so we can get to
 * the "internal" processor registers before MMU_init is called.
 *
<<<<<<< HEAD
 * The TLB code currently contains a major hack.  Since I use the condition
 * code register, I have to save and restore it.  I am out of registers, so
 * I just store it in memory location 0 (the TLB handlers are not reentrant).
 * To avoid making any decisions, I need to use the "segment" valid bit
 * in the first level table, but that would require many changes to the
 * Linux page directory/table functions that I don't want to do right now.
 *
=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 *	-- Dan
 */
	.globl	__start
__start:
	mr	r31,r3			/* save device tree ptr */

	/* We have to turn on the MMU right away so we get cache modes
	 * set correctly.
	 */
	bl	initial_mmu

/* We now have the lower 8 Meg mapped into TLB entries, and the caches
 * ready to work.
 */

turn_on_mmu:
	mfmsr	r0
	ori	r0,r0,MSR_DR|MSR_IR
	mtspr	SPRN_SRR1,r0
	lis	r0,start_here@h
	ori	r0,r0,start_here@l
	mtspr	SPRN_SRR0,r0
<<<<<<< HEAD
	SYNC
	rfi				/* enables MMU */

/*
 * Exception entry code.  This code runs with address translation
 * turned off, i.e. using physical addresses.
 * We assume sprg3 has the physical address of the current
 * task's thread_struct.
 */
#define EXCEPTION_PROLOG	\
	mtspr	SPRN_SPRG_SCRATCH0,r10;	\
	mtspr	SPRN_SPRG_SCRATCH1,r11;	\
	mfcr	r10;		\
	EXCEPTION_PROLOG_1;	\
	EXCEPTION_PROLOG_2

#define EXCEPTION_PROLOG_1	\
	mfspr	r11,SPRN_SRR1;		/* check whether user or kernel */ \
	andi.	r11,r11,MSR_PR;	\
	tophys(r11,r1);			/* use tophys(r1) if kernel */ \
	beq	1f;		\
	mfspr	r11,SPRN_SPRG_THREAD;	\
	lwz	r11,THREAD_INFO-THREAD(r11);	\
	addi	r11,r11,THREAD_SIZE;	\
	tophys(r11,r11);	\
1:	subi	r11,r11,INT_FRAME_SIZE	/* alloc exc. frame */


#define EXCEPTION_PROLOG_2	\
	CLR_TOP32(r11);		\
	stw	r10,_CCR(r11);		/* save registers */ \
	stw	r12,GPR12(r11);	\
	stw	r9,GPR9(r11);	\
	mfspr	r10,SPRN_SPRG_SCRATCH0;	\
	stw	r10,GPR10(r11);	\
	mfspr	r12,SPRN_SPRG_SCRATCH1;	\
	stw	r12,GPR11(r11);	\
	mflr	r10;		\
	stw	r10,_LINK(r11);	\
	mfspr	r12,SPRN_SRR0;	\
	mfspr	r9,SPRN_SRR1;	\
	stw	r1,GPR1(r11);	\
	stw	r1,0(r11);	\
	tovirt(r1,r11);			/* set new kernel sp */	\
	li	r10,MSR_KERNEL & ~(MSR_IR|MSR_DR); /* can take exceptions */ \
	MTMSRD(r10);			/* (except for mach check in rtas) */ \
	stw	r0,GPR0(r11);	\
	SAVE_4GPRS(3, r11);	\
	SAVE_2GPRS(7, r11)

/*
 * Note: code which follows this uses cr0.eq (set if from kernel),
 * r11, r12 (SRR0), and r9 (SRR1).
 *
 * Note2: once we have set r1 we are in a position to take exceptions
 * again, and we could thus set MSR:RI at that point.
 */

/*
 * Exception vectors.
 */
#define EXCEPTION(n, label, hdlr, xfer)		\
	. = n;					\
label:						\
	EXCEPTION_PROLOG;			\
	addi	r3,r1,STACK_FRAME_OVERHEAD;	\
	xfer(n, hdlr)

#define EXC_XFER_TEMPLATE(n, hdlr, trap, copyee, tfer, ret)	\
	li	r10,trap;					\
	stw	r10,_TRAP(r11);					\
	li	r10,MSR_KERNEL;					\
	copyee(r10, r9);					\
	bl	tfer;						\
i##n:								\
	.long	hdlr;						\
	.long	ret

#define COPY_EE(d, s)		rlwimi d,s,0,16,16
#define NOCOPY(d, s)

#define EXC_XFER_STD(n, hdlr)		\
	EXC_XFER_TEMPLATE(n, hdlr, n, NOCOPY, transfer_to_handler_full,	\
			  ret_from_except_full)

#define EXC_XFER_LITE(n, hdlr)		\
	EXC_XFER_TEMPLATE(n, hdlr, n+1, NOCOPY, transfer_to_handler, \
			  ret_from_except)

#define EXC_XFER_EE(n, hdlr)		\
	EXC_XFER_TEMPLATE(n, hdlr, n, COPY_EE, transfer_to_handler_full, \
			  ret_from_except_full)

#define EXC_XFER_EE_LITE(n, hdlr)	\
	EXC_XFER_TEMPLATE(n, hdlr, n+1, COPY_EE, transfer_to_handler, \
			  ret_from_except)

/* System reset */
	EXCEPTION(0x100, Reset, unknown_exception, EXC_XFER_STD)

/* Machine check */
	. = 0x200
MachineCheck:
	EXCEPTION_PROLOG
	mfspr r4,SPRN_DAR
	stw r4,_DAR(r11)
	li r5,0x00f0
	mtspr SPRN_DAR,r5	/* Tag DAR, to be used in DTLB Error */
	mfspr r5,SPRN_DSISR
	stw r5,_DSISR(r11)
	addi r3,r1,STACK_FRAME_OVERHEAD
	EXC_XFER_STD(0x200, machine_check_exception)

/* Data access exception.
 * This is "never generated" by the MPC8xx.  We jump to it for other
 * translation errors.
 */
	. = 0x300
DataAccess:
	EXCEPTION_PROLOG
	mfspr	r10,SPRN_DSISR
	stw	r10,_DSISR(r11)
	mr	r5,r10
	mfspr	r4,SPRN_DAR
	li	r10,0x00f0
	mtspr	SPRN_DAR,r10	/* Tag DAR, to be used in DTLB Error */
	EXC_XFER_LITE(0x300, handle_page_fault)

/* Instruction access exception.
 * This is "never generated" by the MPC8xx.  We jump to it for other
 * translation errors.
 */
	. = 0x400
InstructionAccess:
	EXCEPTION_PROLOG
	mr	r4,r12
	mr	r5,r9
	EXC_XFER_LITE(0x400, handle_page_fault)

/* External interrupt */
	EXCEPTION(0x500, HardwareInterrupt, do_IRQ, EXC_XFER_LITE)

/* Alignment exception */
	. = 0x600
Alignment:
	EXCEPTION_PROLOG
	mfspr	r4,SPRN_DAR
	stw	r4,_DAR(r11)
	li	r5,0x00f0
	mtspr	SPRN_DAR,r5	/* Tag DAR, to be used in DTLB Error */
	mfspr	r5,SPRN_DSISR
	stw	r5,_DSISR(r11)
	addi	r3,r1,STACK_FRAME_OVERHEAD
	EXC_XFER_EE(0x600, alignment_exception)

/* Program check exception */
	EXCEPTION(0x700, ProgramCheck, program_check_exception, EXC_XFER_STD)

/* No FPU on MPC8xx.  This exception is not supposed to happen.
*/
	EXCEPTION(0x800, FPUnavailable, unknown_exception, EXC_XFER_STD)

/* Decrementer */
	EXCEPTION(0x900, Decrementer, timer_interrupt, EXC_XFER_LITE)

	EXCEPTION(0xa00, Trap_0a, unknown_exception, EXC_XFER_EE)
	EXCEPTION(0xb00, Trap_0b, unknown_exception, EXC_XFER_EE)

/* System call */
	. = 0xc00
SystemCall:
	EXCEPTION_PROLOG
	EXC_XFER_EE_LITE(0xc00, DoSyscall)

/* Single step - not used on 601 */
	EXCEPTION(0xd00, SingleStep, single_step_exception, EXC_XFER_STD)
	EXCEPTION(0xe00, Trap_0e, unknown_exception, EXC_XFER_EE)
	EXCEPTION(0xf00, Trap_0f, unknown_exception, EXC_XFER_EE)
=======
	rfi				/* enables MMU */


#ifdef CONFIG_PERF_EVENTS
	.align	4

	.globl	itlb_miss_counter
itlb_miss_counter:
	.space	4

	.globl	dtlb_miss_counter
dtlb_miss_counter:
	.space	4

	.globl	instruction_counter
instruction_counter:
	.space	4
#endif

/* System reset */
	EXCEPTION(INTERRUPT_SYSTEM_RESET, Reset, system_reset_exception)

/* Machine check */
	START_EXCEPTION(INTERRUPT_MACHINE_CHECK, MachineCheck)
	EXCEPTION_PROLOG INTERRUPT_MACHINE_CHECK MachineCheck handle_dar_dsisr=1
	prepare_transfer_to_handler
	bl	machine_check_exception
	b	interrupt_return

/* External interrupt */
	EXCEPTION(INTERRUPT_EXTERNAL, HardwareInterrupt, do_IRQ)

/* Alignment exception */
	START_EXCEPTION(INTERRUPT_ALIGNMENT, Alignment)
	EXCEPTION_PROLOG INTERRUPT_ALIGNMENT Alignment handle_dar_dsisr=1
	prepare_transfer_to_handler
	bl	alignment_exception
	REST_NVGPRS(r1)
	b	interrupt_return

/* Program check exception */
	START_EXCEPTION(INTERRUPT_PROGRAM, ProgramCheck)
	EXCEPTION_PROLOG INTERRUPT_PROGRAM ProgramCheck
	prepare_transfer_to_handler
	bl	program_check_exception
	REST_NVGPRS(r1)
	b	interrupt_return

/* Decrementer */
	EXCEPTION(INTERRUPT_DECREMENTER, Decrementer, timer_interrupt)

/* System call */
	START_EXCEPTION(INTERRUPT_SYSCALL, SystemCall)
	SYSCALL_ENTRY	INTERRUPT_SYSCALL

/* Single step - not used on 601 */
	EXCEPTION(INTERRUPT_TRACE, SingleStep, single_step_exception)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

/* On the MPC8xx, this is a software emulation interrupt.  It occurs
 * for all unimplemented and illegal instructions.
 */
<<<<<<< HEAD
	EXCEPTION(0x1000, SoftEmu, SoftwareEmulation, EXC_XFER_STD)

	. = 0x1100
/*
 * For the MPC8xx, this is a software tablewalk to load the instruction
 * TLB.  It is modelled after the example in the Motorola manual.  The task
 * switch loads the M_TWB register with the pointer to the first level table.
=======
	START_EXCEPTION(INTERRUPT_SOFT_EMU_8xx, SoftEmu)
	EXCEPTION_PROLOG INTERRUPT_SOFT_EMU_8xx SoftEmu
	prepare_transfer_to_handler
	bl	emulation_assist_interrupt
	REST_NVGPRS(r1)
	b	interrupt_return

/*
 * For the MPC8xx, this is a software tablewalk to load the instruction
 * TLB.  The task switch loads the M_TWB register with the pointer to the first
 * level table.
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 * If we discover there is no second level table (value is zero) or if there
 * is an invalid pte, we load that into the TLB, which causes another fault
 * into the TLB Error interrupt where we can handle such problems.
 * We have to use the MD_xxx registers for the tablewalk because the
 * equivalent MI_xxx registers only perform the attribute functions.
 */
<<<<<<< HEAD
InstructionTLBMiss:
#ifdef CONFIG_8xx_CPU6
	stw	r3, 8(r0)
#endif
	DO_8xx_CPU6(0x3f80, r3)
	mtspr	SPRN_M_TW, r10	/* Save a couple of working registers */
	mfcr	r10
#ifdef CONFIG_8xx_CPU6
	stw	r10, 0(r0)
	stw	r11, 4(r0)
#else
	mtspr	SPRN_DAR, r10
	mtspr	SPRN_SPRG2, r11
#endif
	mfspr	r10, SPRN_SRR0	/* Get effective address of fault */
#ifdef CONFIG_8xx_CPU15
	addi	r11, r10, 0x1000
	tlbie	r11
	addi	r11, r10, -0x1000
	tlbie	r11
#endif
	DO_8xx_CPU6(0x3780, r3)
	mtspr	SPRN_MD_EPN, r10	/* Have to use MD_EPN for walk, MI_EPN can't */
	mfspr	r10, SPRN_M_TWB	/* Get level 1 table entry address */
=======

#ifdef CONFIG_8xx_CPU15
#define INVALIDATE_ADJACENT_PAGES_CPU15(addr, tmp)	\
	addi	tmp, addr, PAGE_SIZE;	\
	tlbie	tmp;			\
	addi	tmp, addr, -PAGE_SIZE;	\
	tlbie	tmp
#else
#define INVALIDATE_ADJACENT_PAGES_CPU15(addr, tmp)
#endif

	START_EXCEPTION(INTERRUPT_INST_TLB_MISS_8xx, InstructionTLBMiss)
	mtspr	SPRN_SPRG_SCRATCH2, r10
	mtspr	SPRN_M_TW, r11
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	/* If we are faulting a kernel address, we have to use the
	 * kernel page tables.
	 */
<<<<<<< HEAD
#ifdef CONFIG_MODULES
	/* Only modules will cause ITLB Misses as we always
	 * pin the first 8MB of kernel memory */
	andi.	r11, r10, 0x0800	/* Address >= 0x80000000 */
	beq	3f
	lis	r11, swapper_pg_dir@h
	ori	r11, r11, swapper_pg_dir@l
	rlwimi	r10, r11, 0, 2, 19
3:
#endif
	lwz	r11, 0(r10)	/* Get the level 1 entry */
	rlwinm.	r10, r11,0,0,19	/* Extract page descriptor page address */
	beq	2f		/* If zero, don't try to find a pte */

	/* We have a pte table, so load the MI_TWC with the attributes
	 * for this "segment."
	 */
	ori	r11,r11,1		/* Set valid bit */
	DO_8xx_CPU6(0x2b80, r3)
	mtspr	SPRN_MI_TWC, r11	/* Set segment attributes */
	DO_8xx_CPU6(0x3b80, r3)
	mtspr	SPRN_MD_TWC, r11	/* Load pte table base address */
	mfspr	r11, SPRN_MD_TWC	/* ....and get the pte address */
	lwz	r10, 0(r11)	/* Get the pte */

#ifdef CONFIG_SWAP
	andi.	r11, r10, _PAGE_ACCESSED | _PAGE_PRESENT
	cmpwi	cr0, r11, _PAGE_ACCESSED | _PAGE_PRESENT
	bne-	cr0, 2f
#endif
	/* The Linux PTE won't go exactly into the MMU TLB.
	 * Software indicator bits 21 and 28 must be clear.
	 * Software indicator bits 24, 25, 26, and 27 must be
	 * set.  All other Linux PTE bits control the behavior
	 * of the MMU.
	 */
	li	r11, 0x00f0
	rlwimi	r10, r11, 0, 0x07f8	/* Set 24-27, clear 21-23,28 */
	DO_8xx_CPU6(0x2d80, r3)
	mtspr	SPRN_MI_RPN, r10	/* Update TLB entry */

	/* Restore registers */
#ifndef CONFIG_8xx_CPU6
	mfspr	r10, SPRN_DAR
	mtcr	r10
	mtspr	SPRN_DAR, r11	/* Tag DAR */
	mfspr	r11, SPRN_SPRG2
#else
	lwz	r11, 0(r0)
	mtcr	r11
	lwz	r11, 4(r0)
	lwz	r3, 8(r0)
#endif
	mfspr	r10, SPRN_M_TW
	rfi
2:
	mfspr	r11, SPRN_SRR1
	/* clear all error bits as TLB Miss
	 * sets a few unconditionally
	*/
	rlwinm	r11, r11, 0, 0xffff
	mtspr	SPRN_SRR1, r11

	/* Restore registers */
#ifndef CONFIG_8xx_CPU6
	mfspr	r10, SPRN_DAR
	mtcr	r10
	li	r11, 0x00f0
	mtspr	SPRN_DAR, r11	/* Tag DAR */
	mfspr	r11, SPRN_SPRG2
#else
	lwz	r11, 0(r0)
	mtcr	r11
	lwz	r11, 4(r0)
	lwz	r3, 8(r0)
#endif
	mfspr	r10, SPRN_M_TW
	b	InstructionAccess

	. = 0x1200
DataStoreTLBMiss:
#ifdef CONFIG_8xx_CPU6
	stw	r3, 8(r0)
#endif
	DO_8xx_CPU6(0x3f80, r3)
	mtspr	SPRN_M_TW, r10	/* Save a couple of working registers */
	mfcr	r10
#ifdef CONFIG_8xx_CPU6
	stw	r10, 0(r0)
	stw	r11, 4(r0)
#else
	mtspr	SPRN_DAR, r10
	mtspr	SPRN_SPRG2, r11
#endif
	mfspr	r10, SPRN_M_TWB	/* Get level 1 table entry address */
=======
	mfspr	r10, SPRN_SRR0	/* Get effective address of fault */
	INVALIDATE_ADJACENT_PAGES_CPU15(r10, r11)
	mtspr	SPRN_MD_EPN, r10
#ifdef CONFIG_MODULES
	mfcr	r11
	compare_to_kernel_boundary r10, r10
#endif
	mfspr	r10, SPRN_M_TWB	/* Get level 1 table */
#ifdef CONFIG_MODULES
	blt+	3f
	rlwinm	r10, r10, 0, 20, 31
	oris	r10, r10, (swapper_pg_dir - PAGE_OFFSET)@ha
3:
	mtcr	r11
#endif
	lwz	r11, (swapper_pg_dir-PAGE_OFFSET)@l(r10)	/* Get level 1 entry */
	mtspr	SPRN_MD_TWC, r11
	mfspr	r10, SPRN_MD_TWC
	lwz	r10, 0(r10)	/* Get the pte */
	rlwimi	r11, r10, 0, _PAGE_GUARDED | _PAGE_ACCESSED
	rlwimi	r11, r10, 32 - 9, _PMD_PAGE_512K
	mtspr	SPRN_MI_TWC, r11
	/* The Linux PTE won't go exactly into the MMU TLB.
	 * Software indicator bits 20 and 23 must be clear.
	 * Software indicator bits 22, 24, 25, 26, and 27 must be
	 * set.  All other Linux PTE bits control the behavior
	 * of the MMU.
	 */
	rlwinm	r10, r10, 0, ~0x0f00	/* Clear bits 20-23 */
	rlwimi	r10, r10, 4, 0x0400	/* Copy _PAGE_EXEC into bit 21 */
	ori	r10, r10, RPN_PATTERN | 0x200 /* Set 22 and 24-27 */
	mtspr	SPRN_MI_RPN, r10	/* Update TLB entry */

	/* Restore registers */
0:	mfspr	r10, SPRN_SPRG_SCRATCH2
	mfspr	r11, SPRN_M_TW
	rfi
	patch_site	0b, patch__itlbmiss_exit_1

#ifdef CONFIG_PERF_EVENTS
	patch_site	0f, patch__itlbmiss_perf
0:	lwz	r10, (itlb_miss_counter - PAGE_OFFSET)@l(0)
	addi	r10, r10, 1
	stw	r10, (itlb_miss_counter - PAGE_OFFSET)@l(0)
	mfspr	r10, SPRN_SPRG_SCRATCH2
	mfspr	r11, SPRN_M_TW
	rfi
#endif

	START_EXCEPTION(INTERRUPT_DATA_TLB_MISS_8xx, DataStoreTLBMiss)
	mtspr	SPRN_SPRG_SCRATCH2, r10
	mtspr	SPRN_M_TW, r11
	mfcr	r11
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	/* If we are faulting a kernel address, we have to use the
	 * kernel page tables.
	 */
<<<<<<< HEAD
	andi.	r11, r10, 0x0800
	beq	3f
	lis	r11, swapper_pg_dir@h
	ori	r11, r11, swapper_pg_dir@l
	rlwimi	r10, r11, 0, 2, 19
3:
	lwz	r11, 0(r10)	/* Get the level 1 entry */
	rlwinm.	r10, r11,0,0,19	/* Extract page descriptor page address */
	beq	2f		/* If zero, don't try to find a pte */

	/* We have a pte table, so load fetch the pte from the table.
	 */
	ori	r11, r11, 1	/* Set valid bit in physical L2 page */
	DO_8xx_CPU6(0x3b80, r3)
	mtspr	SPRN_MD_TWC, r11	/* Load pte table base address */
	mfspr	r10, SPRN_MD_TWC	/* ....and get the pte address */
	lwz	r10, 0(r10)	/* Get the pte */

	/* Insert the Guarded flag into the TWC from the Linux PTE.
=======
	mfspr	r10, SPRN_MD_EPN
	compare_to_kernel_boundary r10, r10
	mfspr	r10, SPRN_M_TWB	/* Get level 1 table */
	blt+	3f
	rlwinm	r10, r10, 0, 20, 31
	oris	r10, r10, (swapper_pg_dir - PAGE_OFFSET)@ha
3:
	mtcr	r11
	lwz	r11, (swapper_pg_dir-PAGE_OFFSET)@l(r10)	/* Get level 1 entry */

	mtspr	SPRN_MD_TWC, r11
	mfspr	r10, SPRN_MD_TWC
	lwz	r10, 0(r10)	/* Get the pte */

	/* Insert Guarded and Accessed flags into the TWC from the Linux PTE.
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	 * It is bit 27 of both the Linux PTE and the TWC (at least
	 * I got that right :-).  It will be better when we can put
	 * this into the Linux pgd/pmd and load it in the operation
	 * above.
	 */
<<<<<<< HEAD
	rlwimi	r11, r10, 0, 27, 27
	/* Insert the WriteThru flag into the TWC from the Linux PTE.
	 * It is bit 25 in the Linux PTE and bit 30 in the TWC
	 */
	rlwimi	r11, r10, 32-5, 30, 30
	DO_8xx_CPU6(0x3b80, r3)
	mtspr	SPRN_MD_TWC, r11

	/* Both _PAGE_ACCESSED and _PAGE_PRESENT has to be set.
	 * We also need to know if the insn is a load/store, so:
	 * Clear _PAGE_PRESENT and load that which will
	 * trap into DTLB Error with store bit set accordinly.
	 */
	/* PRESENT=0x1, ACCESSED=0x20
	 * r11 = ((r10 & PRESENT) & ((r10 & ACCESSED) >> 5));
	 * r10 = (r10 & ~PRESENT) | r11;
	 */
#ifdef CONFIG_SWAP
	rlwinm	r11, r10, 32-5, _PAGE_PRESENT
	and	r11, r11, r10
	rlwimi	r10, r11, 0, _PAGE_PRESENT
#endif
	/* Honour kernel RO, User NA */
	/* 0x200 == Extended encoding, bit 22 */
	rlwimi	r10, r10, 32-2, 0x200 /* Copy USER to bit 22, 0x200 */
	/* r11 =  (r10 & _PAGE_RW) >> 1 */
	rlwinm	r11, r10, 32-1, 0x200
	or	r10, r11, r10
	/* invert RW and 0x200 bits */
	xori	r10, r10, _PAGE_RW | 0x200

	/* The Linux PTE won't go exactly into the MMU TLB.
	 * Software indicator bits 22 and 28 must be clear.
=======
	rlwimi	r11, r10, 0, _PAGE_GUARDED | _PAGE_ACCESSED
	rlwimi	r11, r10, 32 - 9, _PMD_PAGE_512K
	mtspr	SPRN_MD_TWC, r11

	/* The Linux PTE won't go exactly into the MMU TLB.
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	 * Software indicator bits 24, 25, 26, and 27 must be
	 * set.  All other Linux PTE bits control the behavior
	 * of the MMU.
	 */
<<<<<<< HEAD
2:	li	r11, 0x00f0
	rlwimi	r10, r11, 0, 24, 28	/* Set 24-27, clear 28 */
	DO_8xx_CPU6(0x3d80, r3)
	mtspr	SPRN_MD_RPN, r10	/* Update TLB entry */

	/* Restore registers */
#ifndef CONFIG_8xx_CPU6
	mfspr	r10, SPRN_DAR
	mtcr	r10
	mtspr	SPRN_DAR, r11	/* Tag DAR */
	mfspr	r11, SPRN_SPRG2
#else
	mtspr	SPRN_DAR, r11	/* Tag DAR */
	lwz	r11, 0(r0)
	mtcr	r11
	lwz	r11, 4(r0)
	lwz	r3, 8(r0)
#endif
	mfspr	r10, SPRN_M_TW
	rfi
=======
	li	r11, RPN_PATTERN
	rlwimi	r10, r11, 0, 24, 27	/* Set 24-27 */
	mtspr	SPRN_MD_RPN, r10	/* Update TLB entry */
	mtspr	SPRN_DAR, r11		/* Tag DAR */

	/* Restore registers */

0:	mfspr	r10, SPRN_SPRG_SCRATCH2
	mfspr	r11, SPRN_M_TW
	rfi
	patch_site	0b, patch__dtlbmiss_exit_1

#ifdef CONFIG_PERF_EVENTS
	patch_site	0f, patch__dtlbmiss_perf
0:	lwz	r10, (dtlb_miss_counter - PAGE_OFFSET)@l(0)
	addi	r10, r10, 1
	stw	r10, (dtlb_miss_counter - PAGE_OFFSET)@l(0)
	mfspr	r10, SPRN_SPRG_SCRATCH2
	mfspr	r11, SPRN_M_TW
	rfi
#endif
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

/* This is an instruction TLB error on the MPC8xx.  This could be due
 * to many reasons, such as executing guarded memory or illegal instruction
 * addresses.  There is nothing to do but handle a big time error fault.
 */
<<<<<<< HEAD
	. = 0x1300
InstructionTLBError:
	b	InstructionAccess

/* This is the data TLB error on the MPC8xx.  This could be due to
 * many reasons, including a dirty update to a pte.  We can catch that
 * one here, but anything else is an error.  First, we track down the
 * Linux pte.  If it is valid, write access is allowed, but the
 * page dirty bit is not set, we will set it and reload the TLB.  For
 * any other case, we bail out to a higher level function that can
 * handle it.
 */
	. = 0x1400
DataTLBError:
#ifdef CONFIG_8xx_CPU6
	stw	r3, 8(r0)
#endif
	DO_8xx_CPU6(0x3f80, r3)
	mtspr	SPRN_M_TW, r10	/* Save a couple of working registers */
	mfcr	r10
	stw	r10, 0(r0)
	stw	r11, 4(r0)

	mfspr	r10, SPRN_DAR
	cmpwi	cr0, r10, 0x00f0
	beq-	FixupDAR	/* must be a buggy dcbX, icbi insn. */
DARFixed:/* Return from dcbx instruction bug workaround, r10 holds value of DAR */
	mfspr	r10, SPRN_M_TW	/* Restore registers */
	lwz	r11, 0(r0)
	mtcr	r11
	lwz	r11, 4(r0)
#ifdef CONFIG_8xx_CPU6
	lwz	r3, 8(r0)
#endif
	b	DataAccess

	EXCEPTION(0x1500, Trap_15, unknown_exception, EXC_XFER_EE)
	EXCEPTION(0x1600, Trap_16, unknown_exception, EXC_XFER_EE)
	EXCEPTION(0x1700, Trap_17, unknown_exception, EXC_XFER_EE)
	EXCEPTION(0x1800, Trap_18, unknown_exception, EXC_XFER_EE)
	EXCEPTION(0x1900, Trap_19, unknown_exception, EXC_XFER_EE)
	EXCEPTION(0x1a00, Trap_1a, unknown_exception, EXC_XFER_EE)
	EXCEPTION(0x1b00, Trap_1b, unknown_exception, EXC_XFER_EE)
=======
	START_EXCEPTION(INTERRUPT_INST_TLB_ERROR_8xx, InstructionTLBError)
	/* 0x400 is InstructionAccess exception, needed by bad_page_fault() */
	EXCEPTION_PROLOG INTERRUPT_INST_STORAGE InstructionTLBError
	andis.	r5,r9,DSISR_SRR1_MATCH_32S@h /* Filter relevant SRR1 bits */
	andis.	r10,r9,SRR1_ISI_NOPT@h
	beq+	.Litlbie
	tlbie	r12
.Litlbie:
	stw	r12, _DAR(r11)
	stw	r5, _DSISR(r11)
	prepare_transfer_to_handler
	bl	do_page_fault
	b	interrupt_return

/* This is the data TLB error on the MPC8xx.  This could be due to
 * many reasons, including a dirty update to a pte.  We bail out to
 * a higher level function that can handle it.
 */
	START_EXCEPTION(INTERRUPT_DATA_TLB_ERROR_8xx, DataTLBError)
	EXCEPTION_PROLOG_0 handle_dar_dsisr=1
	mfspr	r11, SPRN_DAR
	cmpwi	cr1, r11, RPN_PATTERN
	beq-	cr1, FixupDAR	/* must be a buggy dcbX, icbi insn. */
DARFixed:/* Return from dcbx instruction bug workaround */
	EXCEPTION_PROLOG_1
	/* 0x300 is DataAccess exception, needed by bad_page_fault() */
	EXCEPTION_PROLOG_2 INTERRUPT_DATA_STORAGE DataTLBError handle_dar_dsisr=1
	lwz	r4, _DAR(r11)
	lwz	r5, _DSISR(r11)
	andis.	r10,r5,DSISR_NOHPTE@h
	beq+	.Ldtlbie
	tlbie	r4
.Ldtlbie:
	prepare_transfer_to_handler
	bl	do_page_fault
	b	interrupt_return

#ifdef CONFIG_VMAP_STACK
	vmap_stack_overflow_exception
#endif
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

/* On the MPC8xx, these next four traps are used for development
 * support of breakpoints and such.  Someday I will get around to
 * using them.
 */
<<<<<<< HEAD
	EXCEPTION(0x1c00, Trap_1c, unknown_exception, EXC_XFER_EE)
	EXCEPTION(0x1d00, Trap_1d, unknown_exception, EXC_XFER_EE)
	EXCEPTION(0x1e00, Trap_1e, unknown_exception, EXC_XFER_EE)
	EXCEPTION(0x1f00, Trap_1f, unknown_exception, EXC_XFER_EE)

=======
	START_EXCEPTION(INTERRUPT_DATA_BREAKPOINT_8xx, DataBreakpoint)
	EXCEPTION_PROLOG_0 handle_dar_dsisr=1
	mfspr	r11, SPRN_SRR0
	cmplwi	cr1, r11, (.Ldtlbie - PAGE_OFFSET)@l
	cmplwi	cr7, r11, (.Litlbie - PAGE_OFFSET)@l
	cror	4*cr1+eq, 4*cr1+eq, 4*cr7+eq
	bne	cr1, 1f
	mtcr	r10
	mfspr	r10, SPRN_SPRG_SCRATCH0
	mfspr	r11, SPRN_SPRG_SCRATCH1
	rfi

1:	EXCEPTION_PROLOG_1
	EXCEPTION_PROLOG_2 INTERRUPT_DATA_BREAKPOINT_8xx DataBreakpoint handle_dar_dsisr=1
	mfspr	r4,SPRN_BAR
	stw	r4,_DAR(r11)
	prepare_transfer_to_handler
	bl	do_break
	REST_NVGPRS(r1)
	b	interrupt_return

#ifdef CONFIG_PERF_EVENTS
	START_EXCEPTION(INTERRUPT_INST_BREAKPOINT_8xx, InstructionBreakpoint)
	mtspr	SPRN_SPRG_SCRATCH0, r10
	lwz	r10, (instruction_counter - PAGE_OFFSET)@l(0)
	addi	r10, r10, -1
	stw	r10, (instruction_counter - PAGE_OFFSET)@l(0)
	lis	r10, 0xffff
	ori	r10, r10, 0x01
	mtspr	SPRN_COUNTA, r10
	mfspr	r10, SPRN_SPRG_SCRATCH0
	rfi
#else
	EXCEPTION(INTERRUPT_INST_BREAKPOINT_8xx, Trap_1d, unknown_exception)
#endif
	EXCEPTION(0x1e00, Trap_1e, unknown_exception)
	EXCEPTION(0x1f00, Trap_1f, unknown_exception)

	__HEAD
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	. = 0x2000

/* This is the procedure to calculate the data EA for buggy dcbx,dcbi instructions
 * by decoding the registers used by the dcbx instruction and adding them.
<<<<<<< HEAD
 * DAR is set to the calculated address and r10 also holds the EA on exit.
 */
 /* define if you don't want to use self modifying code */
#define NO_SELF_MODIFYING_CODE
FixupDAR:/* Entry point for dcbx workaround. */
	/* fetch instruction from memory. */
	mfspr	r10, SPRN_SRR0
	andis.	r11, r10, 0x8000	/* Address >= 0x80000000 */
	DO_8xx_CPU6(0x3780, r3)
	mtspr	SPRN_MD_EPN, r10
	mfspr	r11, SPRN_M_TWB	/* Get level 1 table entry address */
	beq-	3f		/* Branch if user space */
	lis	r11, (swapper_pg_dir-PAGE_OFFSET)@h
	ori	r11, r11, (swapper_pg_dir-PAGE_OFFSET)@l
	rlwimi	r11, r10, 32-20, 0xffc /* r11 = r11&~0xffc|(r10>>20)&0xffc */
3:	lwz	r11, 0(r11)	/* Get the level 1 entry */
	DO_8xx_CPU6(0x3b80, r3)
	mtspr	SPRN_MD_TWC, r11	/* Load pte table base address */
	mfspr	r11, SPRN_MD_TWC	/* ....and get the pte address */
	lwz	r11, 0(r11)	/* Get the pte */
	/* concat physical page address(r11) and page offset(r10) */
	rlwimi	r11, r10, 0, 20, 31
	lwz	r11,0(r11)
/* Check if it really is a dcbx instruction. */
/* dcbt and dcbtst does not generate DTLB Misses/Errors,
 * no need to include them here */
	srwi	r10, r11, 26	/* check if major OP code is 31 */
	cmpwi	cr0, r10, 31
	bne-	141f
	rlwinm	r10, r11, 0, 21, 30
	cmpwi	cr0, r10, 2028	/* Is dcbz? */
	beq+	142f
	cmpwi	cr0, r10, 940	/* Is dcbi? */
	beq+	142f
	cmpwi	cr0, r10, 108	/* Is dcbst? */
	beq+	144f		/* Fix up store bit! */
	cmpwi	cr0, r10, 172	/* Is dcbf? */
	beq+	142f
	cmpwi	cr0, r10, 1964	/* Is icbi? */
	beq+	142f
141:	mfspr	r10, SPRN_DAR	/* r10 must hold DAR at exit */
	b	DARFixed	/* Nope, go back to normal TLB processing */

=======
 * DAR is set to the calculated address.
 */
FixupDAR:/* Entry point for dcbx workaround. */
	mtspr	SPRN_M_TW, r10
	/* fetch instruction from memory. */
	mfspr	r10, SPRN_SRR0
	mtspr	SPRN_MD_EPN, r10
	rlwinm	r11, r10, 16, 0xfff8
	cmpli	cr1, r11, PAGE_OFFSET@h
	mfspr	r11, SPRN_M_TWB	/* Get level 1 table */
	blt+	cr1, 3f

	/* create physical page address from effective address */
	tophys(r11, r10)
	mfspr	r11, SPRN_M_TWB	/* Get level 1 table */
	rlwinm	r11, r11, 0, 20, 31
	oris	r11, r11, (swapper_pg_dir - PAGE_OFFSET)@ha
3:
	lwz	r11, (swapper_pg_dir-PAGE_OFFSET)@l(r11)	/* Get the level 1 entry */
	mtspr	SPRN_MD_TWC, r11
	mtcrf	0x01, r11
	mfspr	r11, SPRN_MD_TWC
	lwz	r11, 0(r11)	/* Get the pte */
	bt	28,200f		/* bit 28 = Large page (8M) */
	/* concat physical page address(r11) and page offset(r10) */
	rlwimi	r11, r10, 0, 32 - PAGE_SHIFT, 31
201:	lwz	r11,0(r11)
/* Check if it really is a dcbx instruction. */
/* dcbt and dcbtst does not generate DTLB Misses/Errors,
 * no need to include them here */
	xoris	r10, r11, 0x7c00	/* check if major OP code is 31 */
	rlwinm	r10, r10, 0, 21, 5
	cmpwi	cr1, r10, 2028	/* Is dcbz? */
	beq+	cr1, 142f
	cmpwi	cr1, r10, 940	/* Is dcbi? */
	beq+	cr1, 142f
	cmpwi	cr1, r10, 108	/* Is dcbst? */
	beq+	cr1, 144f		/* Fix up store bit! */
	cmpwi	cr1, r10, 172	/* Is dcbf? */
	beq+	cr1, 142f
	cmpwi	cr1, r10, 1964	/* Is icbi? */
	beq+	cr1, 142f
141:	mfspr	r10,SPRN_M_TW
	b	DARFixed	/* Nope, go back to normal TLB processing */

200:
	/* concat physical page address(r11) and page offset(r10) */
	rlwimi	r11, r10, 0, 32 - PAGE_SHIFT_8M, 31
	b	201b

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
144:	mfspr	r10, SPRN_DSISR
	rlwinm	r10, r10,0,7,5	/* Clear store bit for buggy dcbst insn */
	mtspr	SPRN_DSISR, r10
142:	/* continue, it was a dcbx, dcbi instruction. */
<<<<<<< HEAD
#ifdef CONFIG_8xx_CPU6
	lwz	r3, 8(r0)	/* restore r3 from memory */
#endif
#ifndef NO_SELF_MODIFYING_CODE
	andis.	r10,r11,0x1f	/* test if reg RA is r0 */
	li	r10,modified_instr@l
	dcbtst	r0,r10		/* touch for store */
	rlwinm	r11,r11,0,0,20	/* Zero lower 10 bits */
	oris	r11,r11,640	/* Transform instr. to a "add r10,RA,RB" */
	ori	r11,r11,532
	stw	r11,0(r10)	/* store add/and instruction */
	dcbf	0,r10		/* flush new instr. to memory. */
	icbi	0,r10		/* invalidate instr. cache line */
	lwz	r11, 4(r0)	/* restore r11 from memory */
	mfspr	r10, SPRN_M_TW	/* restore r10 from M_TW */
	isync			/* Wait until new instr is loaded from memory */
modified_instr:
	.space	4		/* this is where the add instr. is stored */
	bne+	143f
	subf	r10,r0,r10	/* r10=r10-r0, only if reg RA is r0 */
143:	mtdar	r10		/* store faulting EA in DAR */
	b	DARFixed	/* Go back to normal TLB handling */
#else
=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	mfctr	r10
	mtdar	r10			/* save ctr reg in DAR */
	rlwinm	r10, r11, 24, 24, 28	/* offset into jump table for reg RB */
	addi	r10, r10, 150f@l	/* add start of table */
	mtctr	r10			/* load ctr with jump address */
	xor	r10, r10, r10		/* sum starts at zero */
	bctr				/* jump into table */
150:
	add	r10, r10, r0	;b	151f
	add	r10, r10, r1	;b	151f
	add	r10, r10, r2	;b	151f
	add	r10, r10, r3	;b	151f
	add	r10, r10, r4	;b	151f
	add	r10, r10, r5	;b	151f
	add	r10, r10, r6	;b	151f
	add	r10, r10, r7	;b	151f
	add	r10, r10, r8	;b	151f
	add	r10, r10, r9	;b	151f
	mtctr	r11	;b	154f	/* r10 needs special handling */
	mtctr	r11	;b	153f	/* r11 needs special handling */
	add	r10, r10, r12	;b	151f
	add	r10, r10, r13	;b	151f
	add	r10, r10, r14	;b	151f
	add	r10, r10, r15	;b	151f
	add	r10, r10, r16	;b	151f
	add	r10, r10, r17	;b	151f
	add	r10, r10, r18	;b	151f
	add	r10, r10, r19	;b	151f
	add	r10, r10, r20	;b	151f
	add	r10, r10, r21	;b	151f
	add	r10, r10, r22	;b	151f
	add	r10, r10, r23	;b	151f
	add	r10, r10, r24	;b	151f
	add	r10, r10, r25	;b	151f
	add	r10, r10, r26	;b	151f
	add	r10, r10, r27	;b	151f
	add	r10, r10, r28	;b	151f
	add	r10, r10, r29	;b	151f
	add	r10, r10, r30	;b	151f
	add	r10, r10, r31
151:
<<<<<<< HEAD
	rlwinm. r11,r11,19,24,28	/* offset into jump table for reg RA */
	beq	152f			/* if reg RA is zero, don't add it */
=======
	rlwinm	r11,r11,19,24,28	/* offset into jump table for reg RA */
	cmpwi	cr1, r11, 0
	beq	cr1, 152f		/* if reg RA is zero, don't add it */
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	addi	r11, r11, 150b@l	/* add start of table */
	mtctr	r11			/* load ctr with jump address */
	rlwinm	r11,r11,0,16,10		/* make sure we don't execute this more than once */
	bctr				/* jump into table */
152:
	mfdar	r11
	mtctr	r11			/* restore ctr reg from DAR */
<<<<<<< HEAD
	mtdar	r10			/* save fault EA to DAR */
	b	DARFixed		/* Go back to normal TLB handling */

	/* special handling for r10,r11 since these are modified already */
153:	lwz	r11, 4(r0)	/* load r11 from memory */
	b	155f
154:	mfspr	r11, SPRN_M_TW	/* load r10 from M_TW */
155:	add	r10, r10, r11	/* add it */
	mfctr	r11		/* restore r11 */
	b	151b
#endif

	.globl	giveup_fpu
giveup_fpu:
	blr
=======
	mfspr	r11, SPRN_SPRG_THREAD
	stw	r10, DAR(r11)
	mfspr	r10, SPRN_DSISR
	stw	r10, DSISR(r11)
	mfspr	r10,SPRN_M_TW
	b	DARFixed		/* Go back to normal TLB handling */

	/* special handling for r10,r11 since these are modified already */
153:	mfspr	r11, SPRN_SPRG_SCRATCH1	/* load r11 from SPRN_SPRG_SCRATCH1 */
	add	r10, r10, r11	/* add it */
	mfctr	r11		/* restore r11 */
	b	151b
154:	mfspr	r11, SPRN_SPRG_SCRATCH0	/* load r10 from SPRN_SPRG_SCRATCH0 */
	add	r10, r10, r11	/* add it */
	mfctr	r11		/* restore r11 */
	b	151b
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

/*
 * This is where the main kernel code starts.
 */
start_here:
	/* ptr to current */
	lis	r2,init_task@h
	ori	r2,r2,init_task@l

	/* ptr to phys current thread */
	tophys(r4,r2)
	addi	r4,r4,THREAD	/* init task's THREAD */
	mtspr	SPRN_SPRG_THREAD,r4

	/* stack */
	lis	r1,init_thread_union@ha
	addi	r1,r1,init_thread_union@l
<<<<<<< HEAD
	li	r0,0
	stwu	r0,THREAD_SIZE-STACK_FRAME_OVERHEAD(r1)
=======
	lis	r0, STACK_END_MAGIC@h
	ori	r0, r0, STACK_END_MAGIC@l
	stw	r0, 0(r1)
	li	r0,0
	stwu	r0,THREAD_SIZE-STACK_FRAME_MIN_SIZE(r1)

	lis	r6, swapper_pg_dir@ha
	tophys(r6,r6)
	mtspr	SPRN_M_TWB, r6
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	bl	early_init	/* We have to do this with MMU on */

/*
 * Decide what sort of machine this is and initialize the MMU.
 */
<<<<<<< HEAD
=======
#ifdef CONFIG_KASAN
	bl	kasan_early_init
#endif
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	li	r3,0
	mr	r4,r31
	bl	machine_init
	bl	MMU_init

/*
 * Go back to running unmapped so we can load up new values
 * and change to using our exception vectors.
 * On the 8xx, all we have to do is invalidate the TLB to clear
 * the old 8M byte TLB mappings and load the page table base register.
 */
	/* The right way to do this would be to track it down through
	 * init's THREAD like the context switch code does, but this is
	 * easier......until someone changes init's static structures.
	 */
<<<<<<< HEAD
	lis	r6, swapper_pg_dir@h
	ori	r6, r6, swapper_pg_dir@l
	tophys(r6,r6)
#ifdef CONFIG_8xx_CPU6
	lis	r4, cpu6_errata_word@h
	ori	r4, r4, cpu6_errata_word@l
	li	r3, 0x3980
	stw	r3, 12(r4)
	lwz	r3, 12(r4)
#endif
	mtspr	SPRN_M_TWB, r6
=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	lis	r4,2f@h
	ori	r4,r4,2f@l
	tophys(r4,r4)
	li	r3,MSR_KERNEL & ~(MSR_IR|MSR_DR)
	mtspr	SPRN_SRR0,r4
	mtspr	SPRN_SRR1,r3
	rfi
/* Load up the kernel context */
2:
<<<<<<< HEAD
	SYNC			/* Force all PTE updates to finish */
	tlbia			/* Clear all TLB entries */
	sync			/* wait for tlbia/tlbie to finish */
	TLBSYNC			/* ... on all CPUs */

	/* set up the PTE pointers for the Abatron bdiGDB.
	*/
	tovirt(r6,r6)
	lis	r5, abatron_pteptrs@h
	ori	r5, r5, abatron_pteptrs@l
	stw	r5, 0xf0(r0)	/* Must match your Abatron config file */
	tophys(r5,r5)
=======
#ifdef CONFIG_PIN_TLB_IMMR
	lis	r0, MD_TWAM@h
	oris	r0, r0, 0x1f00
	mtspr	SPRN_MD_CTR, r0
	LOAD_REG_IMMEDIATE(r0, VIRT_IMMR_BASE | MD_EVALID)
	tlbie	r0
	mtspr	SPRN_MD_EPN, r0
	LOAD_REG_IMMEDIATE(r0, MD_SVALID | MD_PS512K | MD_GUARDED)
	mtspr	SPRN_MD_TWC, r0
	mfspr   r0, SPRN_IMMR
	rlwinm	r0, r0, 0, 0xfff80000
	ori	r0, r0, 0xf0 | _PAGE_DIRTY | _PAGE_SPS | _PAGE_SH | \
			_PAGE_NO_CACHE | _PAGE_PRESENT
	mtspr	SPRN_MD_RPN, r0
	lis	r0, (MD_TWAM | MD_RSV4I)@h
	mtspr	SPRN_MD_CTR, r0
#endif
#if !defined(CONFIG_PIN_TLB_DATA) && !defined(CONFIG_PIN_TLB_IMMR)
	lis	r0, MD_TWAM@h
	mtspr	SPRN_MD_CTR, r0
#endif
	tlbia			/* Clear all TLB entries */
	sync			/* wait for tlbia/tlbie to finish */

	/* set up the PTE pointers for the Abatron bdiGDB.
	*/
	lis	r5, abatron_pteptrs@h
	ori	r5, r5, abatron_pteptrs@l
	stw	r5, 0xf0(0)	/* Must match your Abatron config file */
	tophys(r5,r5)
	lis	r6, swapper_pg_dir@h
	ori	r6, r6, swapper_pg_dir@l
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	stw	r6, 0(r5)

/* Now turn on the MMU for real! */
	li	r4,MSR_KERNEL
	lis	r3,start_kernel@h
	ori	r3,r3,start_kernel@l
	mtspr	SPRN_SRR0,r3
	mtspr	SPRN_SRR1,r4
	rfi			/* enable MMU and jump to start_kernel */

/* Set up the initial MMU state so we can do the first level of
 * kernel initialization.  This maps the first 8 MBytes of memory 1:1
 * virtual to physical.  Also, set the cache mode since that is defined
 * by TLB entries and perform any additional mapping (like of the IMMR).
 * If configured to pin some TLBs, we pin the first 8 Mbytes of kernel,
<<<<<<< HEAD
 * 24 Mbytes of data, and the 8M IMMR space.  Anything not covered by
 * these mappings is mapped by page tables.
 */
initial_mmu:
	tlbia			/* Invalidate all TLB entries */
/* Always pin the first 8 MB ITLB to prevent ITLB
   misses while mucking around with SRR0/SRR1 in asm
*/
	lis	r8, MI_RSV4I@h
	ori	r8, r8, 0x1c00

	mtspr	SPRN_MI_CTR, r8	/* Set instruction MMU control */

#ifdef CONFIG_PIN_TLB
	lis	r10, (MD_RSV4I | MD_RESETVAL)@h
	ori	r10, r10, 0x1c00
	mr	r8, r10
#else
	lis	r10, MD_RESETVAL@h
#endif
#ifndef CONFIG_8xx_COPYBACK
	oris	r10, r10, MD_WTDEF@h
#endif
	mtspr	SPRN_MD_CTR, r10	/* Set data TLB control */

	/* Now map the lower 8 Meg into the TLBs.  For this quick hack,
	 * we can load the instruction and data TLB registers with the
	 * same values.
	 */
	lis	r8, KERNELBASE@h	/* Create vaddr for TLB */
	ori	r8, r8, MI_EVALID	/* Mark it valid */
	mtspr	SPRN_MI_EPN, r8
	mtspr	SPRN_MD_EPN, r8
	li	r8, MI_PS8MEG		/* Set 8M byte page */
	ori	r8, r8, MI_SVALID	/* Make it valid */
	mtspr	SPRN_MI_TWC, r8
	mtspr	SPRN_MD_TWC, r8
	li	r8, MI_BOOTINIT		/* Create RPN for address 0 */
	mtspr	SPRN_MI_RPN, r8		/* Store TLB entry */
	mtspr	SPRN_MD_RPN, r8
	lis	r8, MI_Kp@h		/* Set the protection mode */
	mtspr	SPRN_MI_AP, r8
	mtspr	SPRN_MD_AP, r8

	/* Map another 8 MByte at the IMMR to get the processor
	 * internal registers (among other things).
	 */
#ifdef CONFIG_PIN_TLB
	addi	r10, r10, 0x0100
	mtspr	SPRN_MD_CTR, r10
#endif
	mfspr	r9, 638			/* Get current IMMR */
	andis.	r9, r9, 0xff80		/* Get 8Mbyte boundary */

	mr	r8, r9			/* Create vaddr for TLB */
	ori	r8, r8, MD_EVALID	/* Mark it valid */
	mtspr	SPRN_MD_EPN, r8
	li	r8, MD_PS8MEG		/* Set 8M byte page */
	ori	r8, r8, MD_SVALID	/* Make it valid */
	mtspr	SPRN_MD_TWC, r8
	mr	r8, r9			/* Create paddr for TLB */
	ori	r8, r8, MI_BOOTINIT|0x2 /* Inhibit cache -- Cort */
	mtspr	SPRN_MD_RPN, r8

#ifdef CONFIG_PIN_TLB
	/* Map two more 8M kernel data pages.
	*/
	addi	r10, r10, 0x0100
	mtspr	SPRN_MD_CTR, r10

	lis	r8, KERNELBASE@h	/* Create vaddr for TLB */
	addis	r8, r8, 0x0080		/* Add 8M */
	ori	r8, r8, MI_EVALID	/* Mark it valid */
	mtspr	SPRN_MD_EPN, r8
	li	r9, MI_PS8MEG		/* Set 8M byte page */
	ori	r9, r9, MI_SVALID	/* Make it valid */
	mtspr	SPRN_MD_TWC, r9
	li	r11, MI_BOOTINIT	/* Create RPN for address 0 */
	addis	r11, r11, 0x0080	/* Add 8M */
	mtspr	SPRN_MD_RPN, r11

	addis	r8, r8, 0x0080		/* Add 8M */
	mtspr	SPRN_MD_EPN, r8
	mtspr	SPRN_MD_TWC, r9
	addis	r11, r11, 0x0080	/* Add 8M */
	mtspr	SPRN_MD_RPN, r11
#endif
=======
 * 24 Mbytes of data, and the 512k IMMR space.  Anything not covered by
 * these mappings is mapped by page tables.
 */
SYM_FUNC_START_LOCAL(initial_mmu)
	li	r8, 0
	mtspr	SPRN_MI_CTR, r8		/* remove PINNED ITLB entries */
	lis	r10, MD_TWAM@h
	mtspr	SPRN_MD_CTR, r10	/* remove PINNED DTLB entries */

	tlbia			/* Invalidate all TLB entries */

	lis	r8, MI_APG_INIT@h	/* Set protection modes */
	ori	r8, r8, MI_APG_INIT@l
	mtspr	SPRN_MI_AP, r8
	lis	r8, MD_APG_INIT@h
	ori	r8, r8, MD_APG_INIT@l
	mtspr	SPRN_MD_AP, r8

	/* Map the lower RAM (up to 32 Mbytes) into the ITLB and DTLB */
	lis	r8, MI_RSV4I@h
	ori	r8, r8, 0x1c00
	oris	r12, r10, MD_RSV4I@h
	ori	r12, r12, 0x1c00
	li	r9, 4				/* up to 4 pages of 8M */
	mtctr	r9
	lis	r9, KERNELBASE@h		/* Create vaddr for TLB */
	li	r10, MI_PS8MEG | _PMD_ACCESSED | MI_SVALID
	li	r11, MI_BOOTINIT		/* Create RPN for address 0 */
1:
	mtspr	SPRN_MI_CTR, r8	/* Set instruction MMU control */
	addi	r8, r8, 0x100
	ori	r0, r9, MI_EVALID		/* Mark it valid */
	mtspr	SPRN_MI_EPN, r0
	mtspr	SPRN_MI_TWC, r10
	mtspr	SPRN_MI_RPN, r11		/* Store TLB entry */
	mtspr	SPRN_MD_CTR, r12
	addi	r12, r12, 0x100
	mtspr	SPRN_MD_EPN, r0
	mtspr	SPRN_MD_TWC, r10
	mtspr	SPRN_MD_RPN, r11
	addis	r9, r9, 0x80
	addis	r11, r11, 0x80

	bdnz	1b
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	/* Since the cache is enabled according to the information we
	 * just loaded into the TLB, invalidate and enable the caches here.
	 * We should probably check/set other modes....later.
	 */
	lis	r8, IDC_INVALL@h
	mtspr	SPRN_IC_CST, r8
	mtspr	SPRN_DC_CST, r8
	lis	r8, IDC_ENABLE@h
	mtspr	SPRN_IC_CST, r8
<<<<<<< HEAD
#ifdef CONFIG_8xx_COPYBACK
	mtspr	SPRN_DC_CST, r8
#else
	/* For a debug option, I left this here to easily enable
	 * the write through cache mode
	 */
	lis	r8, DC_SFWT@h
	mtspr	SPRN_DC_CST, r8
	lis	r8, IDC_ENABLE@h
	mtspr	SPRN_DC_CST, r8
#endif
	blr


/*
 * Set up to use a given MMU context.
 * r3 is context number, r4 is PGD pointer.
 *
 * We place the physical address of the new task page directory loaded
 * into the MMU base register, and set the ASID compare register with
 * the new "context."
 */
_GLOBAL(set_context)

#ifdef CONFIG_BDI_SWITCH
	/* Context switch the PTE pointer for the Abatron BDI2000.
	 * The PGDIR is passed as second argument.
	 */
	lis	r5, KERNELBASE@h
	lwz	r5, 0xf0(r5)
	stw	r4, 0x4(r5)
#endif

#ifdef CONFIG_8xx_CPU6
	lis	r6, cpu6_errata_word@h
	ori	r6, r6, cpu6_errata_word@l
	tophys	(r4, r4)
	li	r7, 0x3980
	stw	r7, 12(r6)
	lwz	r7, 12(r6)
        mtspr   SPRN_M_TWB, r4               /* Update MMU base address */
	li	r7, 0x3380
	stw	r7, 12(r6)
	lwz	r7, 12(r6)
        mtspr   SPRN_M_CASID, r3             /* Update context */
#else
        mtspr   SPRN_M_CASID,r3		/* Update context */
	tophys	(r4, r4)
	mtspr	SPRN_M_TWB, r4		/* and pgd */
#endif
	SYNC
	blr

#ifdef CONFIG_8xx_CPU6
/* It's here because it is unique to the 8xx.
 * It is important we get called with interrupts disabled.  I used to
 * do that, but it appears that all code that calls this already had
 * interrupt disabled.
 */
	.globl	set_dec_cpu6
set_dec_cpu6:
	lis	r7, cpu6_errata_word@h
	ori	r7, r7, cpu6_errata_word@l
	li	r4, 0x2c00
	stw	r4, 8(r7)
	lwz	r4, 8(r7)
        mtspr   22, r3		/* Update Decrementer */
	SYNC
	blr
#endif

/*
 * We put a few things here that have to be page-aligned.
 * This stuff goes at the beginning of the data segment,
 * which is page-aligned.
 */
	.data
	.globl	sdata
sdata:
	.globl	empty_zero_page
empty_zero_page:
	.space	4096

	.globl	swapper_pg_dir
swapper_pg_dir:
	.space	4096

/* Room for two PTE table poiners, usually the kernel and current user
 * pointer to their respective root page table (pgdir).
 */
abatron_pteptrs:
	.space	8

#ifdef CONFIG_8xx_CPU6
	.globl	cpu6_errata_word
cpu6_errata_word:
	.space	16
#endif

=======
	mtspr	SPRN_DC_CST, r8
	/* Disable debug mode entry on breakpoints */
	mfspr	r8, SPRN_DER
#ifdef CONFIG_PERF_EVENTS
	rlwinm	r8, r8, 0, ~0xc
#else
	rlwinm	r8, r8, 0, ~0x8
#endif
	mtspr	SPRN_DER, r8
	blr
SYM_FUNC_END(initial_mmu)

_GLOBAL(mmu_pin_tlb)
	lis	r9, (1f - PAGE_OFFSET)@h
	ori	r9, r9, (1f - PAGE_OFFSET)@l
	mfmsr	r10
	mflr	r11
	li	r12, MSR_KERNEL & ~(MSR_IR | MSR_DR | MSR_RI)
	rlwinm	r0, r10, 0, ~MSR_RI
	rlwinm	r0, r0, 0, ~MSR_EE
	mtmsr	r0
	isync
	.align	4
	mtspr	SPRN_SRR0, r9
	mtspr	SPRN_SRR1, r12
	rfi
1:
	li	r5, 0
	lis	r6, MD_TWAM@h
	mtspr	SPRN_MI_CTR, r5
	mtspr	SPRN_MD_CTR, r6
	tlbia

	LOAD_REG_IMMEDIATE(r5, 28 << 8)
	LOAD_REG_IMMEDIATE(r6, PAGE_OFFSET)
	LOAD_REG_IMMEDIATE(r7, MI_SVALID | MI_PS8MEG | _PMD_ACCESSED)
	LOAD_REG_IMMEDIATE(r8, 0xf0 | _PAGE_RO | _PAGE_SPS | _PAGE_SH | _PAGE_PRESENT)
	LOAD_REG_ADDR(r9, _sinittext)
	li	r0, 4
	mtctr	r0

2:	ori	r0, r6, MI_EVALID
	mtspr	SPRN_MI_CTR, r5
	mtspr	SPRN_MI_EPN, r0
	mtspr	SPRN_MI_TWC, r7
	mtspr	SPRN_MI_RPN, r8
	addi	r5, r5, 0x100
	addis	r6, r6, SZ_8M@h
	addis	r8, r8, SZ_8M@h
	cmplw	r6, r9
	bdnzt	lt, 2b
	lis	r0, MI_RSV4I@h
	mtspr	SPRN_MI_CTR, r0

	LOAD_REG_IMMEDIATE(r5, 28 << 8 | MD_TWAM)
#ifdef CONFIG_PIN_TLB_DATA
	LOAD_REG_IMMEDIATE(r6, PAGE_OFFSET)
	LOAD_REG_IMMEDIATE(r7, MI_SVALID | MI_PS8MEG | _PMD_ACCESSED)
	li	r8, 0
#ifdef CONFIG_PIN_TLB_IMMR
	li	r0, 3
#else
	li	r0, 4
#endif
	mtctr	r0
	cmpwi	r4, 0
	beq	4f
	LOAD_REG_ADDR(r9, _sinittext)

2:	ori	r0, r6, MD_EVALID
	ori	r12, r8, 0xf0 | _PAGE_RO | _PAGE_SPS | _PAGE_SH | _PAGE_PRESENT
	mtspr	SPRN_MD_CTR, r5
	mtspr	SPRN_MD_EPN, r0
	mtspr	SPRN_MD_TWC, r7
	mtspr	SPRN_MD_RPN, r12
	addi	r5, r5, 0x100
	addis	r6, r6, SZ_8M@h
	addis	r8, r8, SZ_8M@h
	cmplw	r6, r9
	bdnzt	lt, 2b
4:
2:	ori	r0, r6, MD_EVALID
	ori	r12, r8, 0xf0 | _PAGE_DIRTY | _PAGE_SPS | _PAGE_SH | _PAGE_PRESENT
	mtspr	SPRN_MD_CTR, r5
	mtspr	SPRN_MD_EPN, r0
	mtspr	SPRN_MD_TWC, r7
	mtspr	SPRN_MD_RPN, r12
	addi	r5, r5, 0x100
	addis	r6, r6, SZ_8M@h
	addis	r8, r8, SZ_8M@h
	cmplw	r6, r3
	bdnzt	lt, 2b
#endif
#ifdef CONFIG_PIN_TLB_IMMR
	LOAD_REG_IMMEDIATE(r0, VIRT_IMMR_BASE | MD_EVALID)
	LOAD_REG_IMMEDIATE(r7, MD_SVALID | MD_PS512K | MD_GUARDED | _PMD_ACCESSED)
	mfspr   r8, SPRN_IMMR
	rlwinm	r8, r8, 0, 0xfff80000
	ori	r8, r8, 0xf0 | _PAGE_DIRTY | _PAGE_SPS | _PAGE_SH | \
			_PAGE_NO_CACHE | _PAGE_PRESENT
	mtspr	SPRN_MD_CTR, r5
	mtspr	SPRN_MD_EPN, r0
	mtspr	SPRN_MD_TWC, r7
	mtspr	SPRN_MD_RPN, r8
#endif
#if defined(CONFIG_PIN_TLB_IMMR) || defined(CONFIG_PIN_TLB_DATA)
	lis	r0, (MD_RSV4I | MD_TWAM)@h
	mtspr	SPRN_MD_CTR, r0
#endif
	mtspr	SPRN_SRR1, r10
	mtspr	SPRN_SRR0, r11
	rfi
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
