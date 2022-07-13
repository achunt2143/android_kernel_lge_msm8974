/*
 * This file is subject to the terms and conditions of the GNU General Public
 * License.  See the file "COPYING" in the main directory of this archive
 * for more details.
 *
 * Copyright (c) 1996, 1998, 1999, 2004 by Ralf Baechle
 * Copyright (c) 1999 Silicon Graphics, Inc.
 */
<<<<<<< HEAD
=======
#include <linux/export.h>
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#include <asm/asm.h>
#include <asm/asm-offsets.h>
#include <asm/regdef.h>

#define EX(insn,reg,addr,handler)			\
9:	insn	reg, addr;				\
	.section __ex_table,"a";			\
<<<<<<< HEAD
	PTR	9b, handler;				\
=======
	PTR_WD	9b, handler;				\
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	.previous

/*
 * Return the size of a string including the ending NUL character up to a
 * maximum of a1 or 0 in case of error.
 *
 * Note: for performance reasons we deliberately accept that a user may
<<<<<<< HEAD
 *       make strlen_user and strnlen_user access the first few KSEG0
 *       bytes.  There's nothing secret there.  On 64-bit accessing beyond
 *       the maximum is a tad hairier ...
 */
LEAF(__strnlen_user_asm)
	LONG_L		v0, TI_ADDR_LIMIT($28)	# pointer ok?
	and		v0, a0
	bnez		v0, .Lfault

FEXPORT(__strnlen_user_nocheck_asm)
	move		v0, a0
	PTR_ADDU	a1, a0			# stop pointer
1:	beq		v0, a1, 1f		# limit reached?
	EX(lb, t0, (v0), .Lfault)
	PTR_ADDU	v0, 1
	bnez		t0, 1b
1:	PTR_SUBU	v0, a0
=======
 *	 make strlen_user and strnlen_user access the first few KSEG0
 *	 bytes.	 There's nothing secret there.	On 64-bit accessing beyond
 *	 the maximum is a tad hairier ...
 */
LEAF(__strnlen_user_asm)
	move		v0, a0
	PTR_ADDU	a1, a0			# stop pointer
1:
#ifdef CONFIG_CPU_DADDI_WORKAROUNDS
	.set		noat
	li		AT, 1
#endif
	beq		v0, a1, 1f		# limit reached?
#ifdef CONFIG_EVA
	.set push
	.set eva
	EX(lbe, t0, (v0), .Lfault)
	.set pop
#else
	EX(lb, t0, (v0), .Lfault)
#endif
	.set		noreorder
	bnez		t0, 1b
1:
#ifndef CONFIG_CPU_DADDI_WORKAROUNDS
	 PTR_ADDIU	v0, 1
#else
	 PTR_ADDU	v0, AT
	.set		at
#endif
	.set		reorder
	PTR_SUBU	v0, a0
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	jr		ra
	END(__strnlen_user_asm)

.Lfault:
	move		v0, zero
	jr		ra
<<<<<<< HEAD
=======

	EXPORT_SYMBOL(__strnlen_user_asm)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
