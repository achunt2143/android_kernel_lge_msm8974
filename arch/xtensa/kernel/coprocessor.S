/*
 * arch/xtensa/kernel/coprocessor.S
 *
 * Xtensa processor configuration-specific table of coprocessor and
 * other custom register layout information.
 *
 * This file is subject to the terms and conditions of the GNU General Public
 * License.  See the file "COPYING" in the main directory of this archive
 * for more details.
 *
 * Copyright (C) 2003 - 2007 Tensilica Inc.
 */


#include <linux/linkage.h>
#include <asm/asm-offsets.h>
<<<<<<< HEAD
#include <asm/processor.h>
#include <asm/coprocessor.h>
#include <asm/thread_info.h>
#include <asm/uaccess.h>
#include <asm/unistd.h>
#include <asm/ptrace.h>
#include <asm/current.h>
#include <asm/pgtable.h>
#include <asm/page.h>
#include <asm/signal.h>
#include <asm/tlbflush.h>

/*
 * Entry condition:
 *
 *   a0:	trashed, original value saved on stack (PT_AREG0)
 *   a1:	a1
 *   a2:	new stack pointer, original in DEPC
 *   a3:	dispatch table
 *   depc:	a2, original value saved on stack (PT_DEPC)
 *   excsave_1:	a3
 *
 *   PT_DEPC >= VALID_DOUBLE_EXCEPTION_ADDRESS: double exception, DEPC
 *	     <  VALID_DOUBLE_EXCEPTION_ADDRESS: regular exception
 */

/* IO protection is currently unsupported. */

ENTRY(fast_io_protect)
	wsr	a0, EXCSAVE_1
	movi	a0, unrecoverable_exception
	callx0	a0

=======
#include <asm/asmmacro.h>
#include <asm/coprocessor.h>
#include <asm/current.h>
#include <asm/regs.h>

/*
 * Rules for coprocessor state manipulation on SMP:
 *
 * - a task may have live coprocessors only on one CPU.
 *
 * - whether coprocessor context of task T is live on some CPU is
 *   denoted by T's thread_info->cpenable.
 *
 * - non-zero thread_info->cpenable means that thread_info->cp_owner_cpu
 *   is valid in the T's thread_info. Zero thread_info->cpenable means that
 *   coprocessor context is valid in the T's thread_info.
 *
 * - if a coprocessor context of task T is live on CPU X, only CPU X changes
 *   T's thread_info->cpenable, cp_owner_cpu and coprocessor save area.
 *   This is done by making sure that for the task T with live coprocessor
 *   on CPU X cpenable SR is 0 when T runs on any other CPU Y.
 *   When fast_coprocessor exception is taken on CPU Y it goes to the
 *   C-level do_coprocessor that uses IPI to make CPU X flush T's coprocessors.
 */

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#if XTENSA_HAVE_COPROCESSORS

/*
 * Macros for lazy context switch. 
 */

#define SAVE_CP_REGS(x)							\
<<<<<<< HEAD
	.align 4;							\
	.Lsave_cp_regs_cp##x:						\
	.if XTENSA_HAVE_COPROCESSOR(x);					\
		xchal_cp##x##_store a2 a4 a5 a6 a7;			\
	.endif;								\
	jx	a0

#define SAVE_CP_REGS_TAB(x)						\
	.if XTENSA_HAVE_COPROCESSOR(x);					\
		.long .Lsave_cp_regs_cp##x - .Lsave_cp_regs_jump_table;	\
	.else;								\
		.long 0;						\
	.endif;								\
	.long THREAD_XTREGS_CP##x


#define LOAD_CP_REGS(x)							\
	.align 4;							\
	.Lload_cp_regs_cp##x:						\
	.if XTENSA_HAVE_COPROCESSOR(x);					\
		xchal_cp##x##_load a2 a4 a5 a6 a7;			\
	.endif;								\
	jx	a0

#define LOAD_CP_REGS_TAB(x)						\
	.if XTENSA_HAVE_COPROCESSOR(x);					\
		.long .Lload_cp_regs_cp##x - .Lload_cp_regs_jump_table; \
	.else;								\
		.long 0;						\
	.endif;								\
	.long THREAD_XTREGS_CP##x

=======
	.if XTENSA_HAVE_COPROCESSOR(x);					\
		.align 4;						\
	.Lsave_cp_regs_cp##x:						\
		xchal_cp##x##_store a2 a3 a4 a5 a6;			\
		ret;							\
	.endif

#define LOAD_CP_REGS(x)							\
	.if XTENSA_HAVE_COPROCESSOR(x);					\
		.align 4;						\
	.Lload_cp_regs_cp##x:						\
		xchal_cp##x##_load a2 a3 a4 a5 a6;			\
		ret;							\
	.endif

#define CP_REGS_TAB(x)							\
	.if XTENSA_HAVE_COPROCESSOR(x);					\
		.long .Lsave_cp_regs_cp##x;				\
		.long .Lload_cp_regs_cp##x;				\
	.else;								\
		.long 0, 0;						\
	.endif;								\
	.long THREAD_XTREGS_CP##x

#define CP_REGS_TAB_SAVE 0
#define CP_REGS_TAB_LOAD 4
#define CP_REGS_TAB_OFFSET 8

	__XTENSA_HANDLER

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	SAVE_CP_REGS(0)
	SAVE_CP_REGS(1)
	SAVE_CP_REGS(2)
	SAVE_CP_REGS(3)
	SAVE_CP_REGS(4)
	SAVE_CP_REGS(5)
	SAVE_CP_REGS(6)
	SAVE_CP_REGS(7)

	LOAD_CP_REGS(0)
	LOAD_CP_REGS(1)
	LOAD_CP_REGS(2)
	LOAD_CP_REGS(3)
	LOAD_CP_REGS(4)
	LOAD_CP_REGS(5)
	LOAD_CP_REGS(6)
	LOAD_CP_REGS(7)

	.align 4
<<<<<<< HEAD
.Lsave_cp_regs_jump_table:
	SAVE_CP_REGS_TAB(0)
	SAVE_CP_REGS_TAB(1)
	SAVE_CP_REGS_TAB(2)
	SAVE_CP_REGS_TAB(3)
	SAVE_CP_REGS_TAB(4)
	SAVE_CP_REGS_TAB(5)
	SAVE_CP_REGS_TAB(6)
	SAVE_CP_REGS_TAB(7)

.Lload_cp_regs_jump_table:
	LOAD_CP_REGS_TAB(0)
	LOAD_CP_REGS_TAB(1)
	LOAD_CP_REGS_TAB(2)
	LOAD_CP_REGS_TAB(3)
	LOAD_CP_REGS_TAB(4)
	LOAD_CP_REGS_TAB(5)
	LOAD_CP_REGS_TAB(6)
	LOAD_CP_REGS_TAB(7)

/*
 * coprocessor_save(buffer, index) 
 *                    a2      a3
 * coprocessor_load(buffer, index)
 *                    a2      a3
 *
 * Save or load coprocessor registers for coprocessor 'index'. 
 * The register values are saved to or loaded from them 'buffer' address.
 *
 * Note that these functions don't update the coprocessor_owner information!
 *
 */

ENTRY(coprocessor_save)
	entry	a1, 32
	s32i	a0, a1, 0
	movi	a0, .Lsave_cp_regs_jump_table
	addx8	a3, a3, a0
	l32i	a3, a3, 0
	beqz	a3, 1f
	add	a0, a0, a3
	callx0	a0
1:	l32i	a0, a1, 0
	retw

ENTRY(coprocessor_load)
	entry	a1, 32
	s32i	a0, a1, 0
	movi	a0, .Lload_cp_regs_jump_table
	addx4	a3, a3, a0
	l32i	a3, a3, 0
	beqz	a3, 1f
	add	a0, a0, a3
	callx0	a0
1:	l32i	a0, a1, 0
	retw

/*
 * coprocessor_flush(struct task_info*, index) 
 *                             a2        a3
 * coprocessor_restore(struct task_info*, index)
 *                              a2         a3
 *
 * Save or load coprocessor registers for coprocessor 'index'. 
 * The register values are saved to or loaded from the coprocessor area 
 * inside the task_info structure.
 *
 * Note that these functions don't update the coprocessor_owner information!
 *
 */


ENTRY(coprocessor_flush)
	entry	a1, 32
	s32i	a0, a1, 0
	movi	a0, .Lsave_cp_regs_jump_table
	addx8	a3, a3, a0
	l32i	a4, a3, 4
	l32i	a3, a3, 0
	add	a2, a2, a4
	beqz	a3, 1f
	add	a0, a0, a3
	callx0	a0
1:	l32i	a0, a1, 0
	retw

ENTRY(coprocessor_restore)
	entry	a1, 32
	s32i	a0, a1, 0
	movi	a0, .Lload_cp_regs_jump_table
	addx4	a3, a3, a0
	l32i	a4, a3, 4
	l32i	a3, a3, 0
	add	a2, a2, a4
	beqz	a3, 1f
	add	a0, a0, a3
	callx0	a0
1:	l32i	a0, a1, 0
	retw
=======
.Lcp_regs_jump_table:
	CP_REGS_TAB(0)
	CP_REGS_TAB(1)
	CP_REGS_TAB(2)
	CP_REGS_TAB(3)
	CP_REGS_TAB(4)
	CP_REGS_TAB(5)
	CP_REGS_TAB(6)
	CP_REGS_TAB(7)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

/*
 * Entry condition:
 *
 *   a0:	trashed, original value saved on stack (PT_AREG0)
 *   a1:	a1
 *   a2:	new stack pointer, original in DEPC
<<<<<<< HEAD
 *   a3:	dispatch table
 *   depc:	a2, original value saved on stack (PT_DEPC)
 *   excsave_1:	a3
=======
 *   a3:	a3
 *   depc:	a2, original value saved on stack (PT_DEPC)
 *   excsave_1:	dispatch table
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 *
 *   PT_DEPC >= VALID_DOUBLE_EXCEPTION_ADDRESS: double exception, DEPC
 *	     <  VALID_DOUBLE_EXCEPTION_ADDRESS: regular exception
 */

<<<<<<< HEAD
ENTRY(fast_coprocessor_double)
	wsr	a0, EXCSAVE_1
	movi	a0, unrecoverable_exception
	callx0	a0


ENTRY(fast_coprocessor)

	/* Save remaining registers a1-a3 and SAR */

	xsr	a3, EXCSAVE_1
	s32i	a3, a2, PT_AREG3
	rsr	a3, SAR
	s32i	a1, a2, PT_AREG1
	s32i	a3, a2, PT_SAR
	mov	a1, a2
	rsr	a2, DEPC
	s32i	a2, a1, PT_AREG2

	/*
	 * The hal macros require up to 4 temporary registers. We use a3..a6.
	 */
=======
ENTRY(fast_coprocessor)

	s32i	a3, a2, PT_AREG3

#ifdef CONFIG_SMP
	/*
	 * Check if any coprocessor context is live on another CPU
	 * and if so go through the C-level coprocessor exception handler
	 * to flush it to memory.
	 */
	GET_THREAD_INFO (a0, a2)
	l32i	a3, a0, THREAD_CPENABLE
	beqz	a3, .Lload_local

	/*
	 * Pairs with smp_wmb in local_coprocessor_release_all
	 * and with both memws below.
	 */
	memw
	l32i	a3, a0, THREAD_CPU
	l32i	a0, a0, THREAD_CP_OWNER_CPU
	beq	a0, a3, .Lload_local

	rsr	a0, ps
	l32i	a3, a2, PT_AREG3
	bbci.l	a0, PS_UM_BIT, 1f
	call0	user_exception
1:	call0	kernel_exception
#endif

	/* Save remaining registers a1-a3 and SAR */

.Lload_local:
	rsr	a3, sar
	s32i	a1, a2, PT_AREG1
	s32i	a3, a2, PT_SAR
	mov	a1, a2
	rsr	a2, depc
	s32i	a2, a1, PT_AREG2

	/* The hal macros require up to 4 temporary registers. We use a3..a6. */
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	s32i	a4, a1, PT_AREG4
	s32i	a5, a1, PT_AREG5
	s32i	a6, a1, PT_AREG6
<<<<<<< HEAD

	/* Find coprocessor number. Subtract first CP EXCCAUSE from EXCCAUSE */

	rsr	a3, EXCCAUSE
=======
	s32i	a7, a1, PT_AREG7
	s32i	a8, a1, PT_AREG8
	s32i	a9, a1, PT_AREG9
	s32i	a10, a1, PT_AREG10

	/* Find coprocessor number. Subtract first CP EXCCAUSE from EXCCAUSE */

	rsr	a3, exccause
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	addi	a3, a3, -EXCCAUSE_COPROCESSOR0_DISABLED

	/* Set corresponding CPENABLE bit -> (sar:cp-index, a3: 1<<cp-index)*/

	ssl	a3			# SAR: 32 - coprocessor_number
	movi	a2, 1
<<<<<<< HEAD
	rsr	a0, CPENABLE
	sll	a2, a2
	or	a0, a0, a2
	wsr	a0, CPENABLE
	rsync

	/* Retrieve previous owner. (a3 still holds CP number) */

	movi	a0, coprocessor_owner	# list of owners
	addx4	a0, a3, a0		# entry for CP
	l32i	a4, a0, 0

	beqz	a4, 1f			# skip 'save' if no previous owner

	/* Disable coprocessor for previous owner. (a2 = 1 << CP number) */

	l32i	a5, a4, THREAD_CPENABLE
	xor	a5, a5, a2		# (1 << cp-id) still in a2
	s32i	a5, a4, THREAD_CPENABLE

	/*
	 * Get context save area and 'call' save routine. 
	 * (a4 still holds previous owner (thread_info), a3 CP number)
	 */

	movi	a5, .Lsave_cp_regs_jump_table
	movi	a0, 2f			# a0: 'return' address
	addx8	a3, a3, a5		# a3: coprocessor number
	l32i	a2, a3, 4		# a2: xtregs offset
	l32i	a3, a3, 0		# a3: jump offset
	add	a2, a2, a4
	add	a4, a3, a5		# a4: address of save routine
	jx	a4

	/* Note that only a0 and a1 were preserved. */

2:	rsr	a3, EXCCAUSE
	addi	a3, a3, -EXCCAUSE_COPROCESSOR0_DISABLED
	movi	a0, coprocessor_owner
	addx4	a0, a3, a0

	/* Set new 'owner' (a0 points to the CP owner, a3 contains the CP nr) */

1:	GET_THREAD_INFO (a4, a1)
	s32i	a4, a0, 0

	/* Get context save area and 'call' load routine. */

	movi	a5, .Lload_cp_regs_jump_table
	movi	a0, 1f
	addx8	a3, a3, a5
	l32i	a2, a3, 4		# a2: xtregs offset
	l32i	a3, a3, 0		# a3: jump offset
	add	a2, a2, a4
	add	a4, a3, a5
	jx	a4

	/* Restore all registers and return from exception handler. */

1:	l32i	a6, a1, PT_AREG6
=======
	rsr	a0, cpenable
	sll	a2, a2
	or	a0, a0, a2
	wsr	a0, cpenable
	rsync

	/* Get coprocessor save/load table entry (a7). */

	movi	a7, .Lcp_regs_jump_table
	addx8	a7, a3, a7
	addx4	a7, a3, a7

	/* Retrieve previous owner (a8). */

	rsr	a0, excsave1		# exc_table
	addx4	a0, a3, a0		# entry for CP
	l32i	a8, a0, EXC_TABLE_COPROCESSOR_OWNER

	/* Set new owner (a9). */

	GET_THREAD_INFO (a9, a1)
	l32i	a4, a9, THREAD_CPU
	s32i	a9, a0, EXC_TABLE_COPROCESSOR_OWNER
	s32i	a4, a9, THREAD_CP_OWNER_CPU

	/*
	 * Enable coprocessor for the new owner. (a2 = 1 << CP number)
	 * This can be done before loading context into the coprocessor.
	 */
	l32i	a4, a9, THREAD_CPENABLE
	or	a4, a4, a2

	/*
	 * Make sure THREAD_CP_OWNER_CPU is in memory before updating
	 * THREAD_CPENABLE
	 */
	memw				# (2)
	s32i	a4, a9, THREAD_CPENABLE

	beqz	a8, 1f			# skip 'save' if no previous owner

	/* Disable coprocessor for previous owner. (a2 = 1 << CP number) */

	l32i	a10, a8, THREAD_CPENABLE
	xor	a10, a10, a2

	/* Get context save area and call save routine. */

	l32i	a2, a7, CP_REGS_TAB_OFFSET
	l32i	a3, a7, CP_REGS_TAB_SAVE
	add	a2, a2, a8
	callx0	a3

	/*
	 * Make sure coprocessor context and THREAD_CP_OWNER_CPU are in memory
	 * before updating THREAD_CPENABLE
	 */
	memw				# (3)
	s32i	a10, a8, THREAD_CPENABLE
1:
	/* Get context save area and call load routine. */

	l32i	a2, a7, CP_REGS_TAB_OFFSET
	l32i	a3, a7, CP_REGS_TAB_LOAD
	add	a2, a2, a9
	callx0	a3

	/* Restore all registers and return from exception handler. */

	l32i	a10, a1, PT_AREG10
	l32i	a9, a1, PT_AREG9
	l32i	a8, a1, PT_AREG8
	l32i	a7, a1, PT_AREG7
	l32i	a6, a1, PT_AREG6
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	l32i	a5, a1, PT_AREG5
	l32i	a4, a1, PT_AREG4

	l32i	a0, a1, PT_SAR
	l32i	a3, a1, PT_AREG3
	l32i	a2, a1, PT_AREG2
<<<<<<< HEAD
	wsr	a0, SAR
=======
	wsr	a0, sar
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	l32i	a0, a1, PT_AREG0
	l32i	a1, a1, PT_AREG1

	rfe

<<<<<<< HEAD
	.data
ENTRY(coprocessor_owner)
	.fill XCHAL_CP_MAX, 4, 0

#endif /* XTENSA_HAVE_COPROCESSORS */

=======
ENDPROC(fast_coprocessor)

	.text

/*
 * coprocessor_flush(struct thread_info*, index)
 *                             a2        a3
 *
 * Save coprocessor registers for coprocessor 'index'.
 * The register values are saved to or loaded from the coprocessor area
 * inside the task_info structure.
 *
 * Note that this function doesn't update the coprocessor_owner information!
 *
 */

ENTRY(coprocessor_flush)

	abi_entry_default

	movi	a4, .Lcp_regs_jump_table
	addx8	a4, a3, a4
	addx4	a3, a3, a4
	l32i	a4, a3, CP_REGS_TAB_SAVE
	beqz	a4, 1f
	l32i	a3, a3, CP_REGS_TAB_OFFSET
	add	a2, a2, a3
	mov	a7, a0
	callx0	a4
	mov	a0, a7
1:
	abi_ret_default

ENDPROC(coprocessor_flush)

#endif /* XTENSA_HAVE_COPROCESSORS */
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
