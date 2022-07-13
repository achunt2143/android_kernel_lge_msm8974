/*
 * This file is subject to the terms and conditions of the GNU General Public
 * License.  See the file "COPYING" in the main directory of this archive
 * for more details.
 *
 * Quick'n'dirty IP checksum ...
 *
 * Copyright (C) 1998, 1999 Ralf Baechle
 * Copyright (C) 1999 Silicon Graphics, Inc.
 * Copyright (C) 2007  Maciej W. Rozycki
<<<<<<< HEAD
 */
#include <linux/errno.h>
=======
 * Copyright (C) 2014 Imagination Technologies Ltd.
 */
#include <linux/errno.h>
#include <linux/export.h>
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#include <asm/asm.h>
#include <asm/asm-offsets.h>
#include <asm/regdef.h>

#ifdef CONFIG_64BIT
/*
 * As we are sharing code base with the mips32 tree (which use the o32 ABI
 * register definitions). We need to redefine the register definitions from
 * the n64 ABI register naming to the o32 ABI register naming.
 */
#undef t0
#undef t1
#undef t2
#undef t3
#define t0	$8
#define t1	$9
#define t2	$10
#define t3	$11
#define t4	$12
#define t5	$13
#define t6	$14
#define t7	$15

#define USE_DOUBLE
#endif

#ifdef USE_DOUBLE

#define LOAD   ld
#define LOAD32 lwu
#define ADD    daddu
#define NBYTES 8

#else

#define LOAD   lw
#define LOAD32 lw
#define ADD    addu
#define NBYTES 4

#endif /* USE_DOUBLE */

#define UNIT(unit)  ((unit)*NBYTES)

#define ADDC(sum,reg)						\
<<<<<<< HEAD
	ADD	sum, reg;					\
	sltu	v1, sum, reg;					\
	ADD	sum, v1;					\

#define ADDC32(sum,reg)						\
	addu	sum, reg;					\
	sltu	v1, sum, reg;					\
	addu	sum, v1;					\
=======
	.set	push;						\
	.set	noat;						\
	ADD	sum, reg;					\
	sltu	v1, sum, reg;					\
	ADD	sum, v1;					\
	.set	pop

#define ADDC32(sum,reg)						\
	.set	push;						\
	.set	noat;						\
	addu	sum, reg;					\
	sltu	v1, sum, reg;					\
	addu	sum, v1;					\
	.set	pop
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

#define CSUM_BIGCHUNK1(src, offset, sum, _t0, _t1, _t2, _t3)	\
	LOAD	_t0, (offset + UNIT(0))(src);			\
	LOAD	_t1, (offset + UNIT(1))(src);			\
<<<<<<< HEAD
	LOAD	_t2, (offset + UNIT(2))(src); 			\
	LOAD	_t3, (offset + UNIT(3))(src); 			\
	ADDC(sum, _t0);						\
	ADDC(sum, _t1);						\
	ADDC(sum, _t2);						\
	ADDC(sum, _t3)
=======
	LOAD	_t2, (offset + UNIT(2))(src);			\
	LOAD	_t3, (offset + UNIT(3))(src);			\
	ADDC(_t0, _t1);						\
	ADDC(_t2, _t3);						\
	ADDC(sum, _t0);						\
	ADDC(sum, _t2)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

#ifdef USE_DOUBLE
#define CSUM_BIGCHUNK(src, offset, sum, _t0, _t1, _t2, _t3)	\
	CSUM_BIGCHUNK1(src, offset, sum, _t0, _t1, _t2, _t3)
#else
#define CSUM_BIGCHUNK(src, offset, sum, _t0, _t1, _t2, _t3)	\
	CSUM_BIGCHUNK1(src, offset, sum, _t0, _t1, _t2, _t3);	\
	CSUM_BIGCHUNK1(src, offset + 0x10, sum, _t0, _t1, _t2, _t3)
#endif

/*
 * a0: source address
 * a1: length of the area to checksum
 * a2: partial checksum
 */

#define src a0
#define sum v0

	.text
	.set	noreorder
	.align	5
LEAF(csum_partial)
<<<<<<< HEAD
=======
EXPORT_SYMBOL(csum_partial)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	move	sum, zero
	move	t7, zero

	sltiu	t8, a1, 0x8
	bnez	t8, .Lsmall_csumcpy		/* < 8 bytes to copy */
	 move	t2, a1

	andi	t7, src, 0x1			/* odd buffer? */

.Lhword_align:
	beqz	t7, .Lword_align
	 andi	t8, src, 0x2

	lbu	t0, (src)
	LONG_SUBU	a1, a1, 0x1
#ifdef __MIPSEL__
	sll	t0, t0, 8
#endif
	ADDC(sum, t0)
	PTR_ADDU	src, src, 0x1
	andi	t8, src, 0x2

.Lword_align:
	beqz	t8, .Ldword_align
	 sltiu	t8, a1, 56

	lhu	t0, (src)
	LONG_SUBU	a1, a1, 0x2
	ADDC(sum, t0)
	sltiu	t8, a1, 56
	PTR_ADDU	src, src, 0x2

.Ldword_align:
	bnez	t8, .Ldo_end_words
	 move	t8, a1

	andi	t8, src, 0x4
	beqz	t8, .Lqword_align
	 andi	t8, src, 0x8

	LOAD32	t0, 0x00(src)
	LONG_SUBU	a1, a1, 0x4
	ADDC(sum, t0)
	PTR_ADDU	src, src, 0x4
	andi	t8, src, 0x8

.Lqword_align:
	beqz	t8, .Loword_align
	 andi	t8, src, 0x10

#ifdef USE_DOUBLE
	ld	t0, 0x00(src)
	LONG_SUBU	a1, a1, 0x8
	ADDC(sum, t0)
#else
	lw	t0, 0x00(src)
	lw	t1, 0x04(src)
	LONG_SUBU	a1, a1, 0x8
	ADDC(sum, t0)
	ADDC(sum, t1)
#endif
	PTR_ADDU	src, src, 0x8
	andi	t8, src, 0x10

.Loword_align:
	beqz	t8, .Lbegin_movement
	 LONG_SRL	t8, a1, 0x7

#ifdef USE_DOUBLE
	ld	t0, 0x00(src)
	ld	t1, 0x08(src)
	ADDC(sum, t0)
	ADDC(sum, t1)
#else
	CSUM_BIGCHUNK1(src, 0x00, sum, t0, t1, t3, t4)
#endif
	LONG_SUBU	a1, a1, 0x10
	PTR_ADDU	src, src, 0x10
	LONG_SRL	t8, a1, 0x7

.Lbegin_movement:
	beqz	t8, 1f
	 andi	t2, a1, 0x40

.Lmove_128bytes:
	CSUM_BIGCHUNK(src, 0x00, sum, t0, t1, t3, t4)
	CSUM_BIGCHUNK(src, 0x20, sum, t0, t1, t3, t4)
	CSUM_BIGCHUNK(src, 0x40, sum, t0, t1, t3, t4)
	CSUM_BIGCHUNK(src, 0x60, sum, t0, t1, t3, t4)
	LONG_SUBU	t8, t8, 0x01
	.set	reorder				/* DADDI_WAR */
	PTR_ADDU	src, src, 0x80
	bnez	t8, .Lmove_128bytes
	.set	noreorder

1:
	beqz	t2, 1f
	 andi	t2, a1, 0x20

.Lmove_64bytes:
	CSUM_BIGCHUNK(src, 0x00, sum, t0, t1, t3, t4)
	CSUM_BIGCHUNK(src, 0x20, sum, t0, t1, t3, t4)
	PTR_ADDU	src, src, 0x40

1:
	beqz	t2, .Ldo_end_words
	 andi	t8, a1, 0x1c

.Lmove_32bytes:
	CSUM_BIGCHUNK(src, 0x00, sum, t0, t1, t3, t4)
	andi	t8, a1, 0x1c
	PTR_ADDU	src, src, 0x20

.Ldo_end_words:
	beqz	t8, .Lsmall_csumcpy
	 andi	t2, a1, 0x3
	LONG_SRL	t8, t8, 0x2

.Lend_words:
	LOAD32	t0, (src)
	LONG_SUBU	t8, t8, 0x1
	ADDC(sum, t0)
	.set	reorder				/* DADDI_WAR */
	PTR_ADDU	src, src, 0x4
	bnez	t8, .Lend_words
	.set	noreorder

/* unknown src alignment and < 8 bytes to go  */
.Lsmall_csumcpy:
	move	a1, t2

	andi	t0, a1, 4
	beqz	t0, 1f
	 andi	t0, a1, 2

	/* Still a full word to go  */
	ulw	t1, (src)
	PTR_ADDIU	src, 4
#ifdef USE_DOUBLE
	dsll	t1, t1, 32			/* clear lower 32bit */
#endif
	ADDC(sum, t1)

1:	move	t1, zero
	beqz	t0, 1f
	 andi	t0, a1, 1

	/* Still a halfword to go  */
	ulhu	t1, (src)
	PTR_ADDIU	src, 2

1:	beqz	t0, 1f
	 sll	t1, t1, 16

	lbu	t2, (src)
	 nop

#ifdef __MIPSEB__
	sll	t2, t2, 8
#endif
	or	t1, t2

1:	ADDC(sum, t1)

	/* fold checksum */
#ifdef USE_DOUBLE
	dsll32	v1, sum, 0
	daddu	sum, v1
	sltu	v1, sum, v1
	dsra32	sum, sum, 0
	addu	sum, v1
#endif

	/* odd buffer alignment? */
<<<<<<< HEAD
#ifdef CPU_MIPSR2
	wsbh	v1, sum
	movn	sum, v1, t7
=======
#if defined(CONFIG_CPU_MIPSR2) || defined(CONFIG_CPU_MIPSR5) || \
    defined(CONFIG_CPU_LOONGSON64)
	.set	push
	.set	arch=mips32r2
	wsbh	v1, sum
	movn	sum, v1, t7
	.set	pop
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#else
	beqz	t7, 1f			/* odd buffer alignment? */
	 lui	v1, 0x00ff
	addu	v1, 0x00ff
	and	t0, sum, v1
	sll	t0, t0, 8
	srl	sum, sum, 8
	and	sum, sum, v1
	or	sum, sum, t0
1:
#endif
	.set	reorder
<<<<<<< HEAD
	/* Add the passed partial csum.  */
=======
	/* Add the passed partial csum.	 */
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	ADDC32(sum, a2)
	jr	ra
	.set	noreorder
	END(csum_partial)


/*
 * checksum and copy routines based on memcpy.S
 *
<<<<<<< HEAD
 *	csum_partial_copy_nocheck(src, dst, len, sum)
 *	__csum_partial_copy_user(src, dst, len, sum, errp)
 *
 * See "Spec" in memcpy.S for details.  Unlike __copy_user, all
=======
 *	csum_partial_copy_nocheck(src, dst, len)
 *	__csum_partial_copy_kernel(src, dst, len)
 *
 * See "Spec" in memcpy.S for details.	Unlike __copy_user, all
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 * function in this file use the standard calling convention.
 */

#define src a0
#define dst a1
#define len a2
<<<<<<< HEAD
#define psum a3
#define sum v0
#define odd t8
#define errptr t9

/*
 * The exception handler for loads requires that:
 *  1- AT contain the address of the byte just past the end of the source
 *     of the copy,
 *  2- src_entry <= src < AT, and
 *  3- (dst - src) == (dst_entry - src_entry),
 * The _entry suffix denotes values when __copy_user was called.
 *
 * (1) is set up up by __csum_partial_copy_from_user and maintained by
 *	not writing AT in __csum_partial_copy
 * (2) is met by incrementing src by the number of bytes copied
 * (3) is met by not doing loads between a pair of increments of dst and src
 *
 * The exception handlers for stores stores -EFAULT to errptr and return.
 * These handlers do not need to overwrite any data.
 */

#define EXC(inst_reg,addr,handler)		\
9:	inst_reg, addr;				\
	.section __ex_table,"a";		\
	PTR	9b, handler;			\
	.previous

#ifdef USE_DOUBLE

#define LOAD   ld
#define LOADL  ldl
#define LOADR  ldr
#define STOREL sdl
#define STORER sdr
#define STORE  sd
=======
#define sum v0
#define odd t8

/*
 * All exception handlers simply return 0.
 */

/* Instruction type */
#define LD_INSN 1
#define ST_INSN 2
#define LEGACY_MODE 1
#define EVA_MODE    2
#define USEROP   1
#define KERNELOP 2

/*
 * Wrapper to add an entry in the exception table
 * in case the insn causes a memory exception.
 * Arguments:
 * insn    : Load/store instruction
 * type    : Instruction type
 * reg     : Register
 * addr    : Address
 * handler : Exception handler
 */
#define EXC(insn, type, reg, addr)		\
	.if \mode == LEGACY_MODE;		\
9:		insn reg, addr;			\
		.section __ex_table,"a";	\
		PTR_WD	9b, .L_exc;		\
		.previous;			\
	/* This is enabled in EVA mode */	\
	.else;					\
		/* If loading from user or storing to user */	\
		.if ((\from == USEROP) && (type == LD_INSN)) || \
		    ((\to == USEROP) && (type == ST_INSN));	\
9:			__BUILD_EVA_INSN(insn##e, reg, addr);	\
			.section __ex_table,"a";		\
			PTR_WD	9b, .L_exc;			\
			.previous;				\
		.else;						\
			/* EVA without exception */		\
			insn reg, addr;				\
		.endif;						\
	.endif

#undef LOAD

#ifdef USE_DOUBLE

#define LOADK	ld /* No exception */
#define LOAD(reg, addr)		EXC(ld, LD_INSN, reg, addr)
#define LOADBU(reg, addr)	EXC(lbu, LD_INSN, reg, addr)
#define LOADL(reg, addr)	EXC(ldl, LD_INSN, reg, addr)
#define LOADR(reg, addr)	EXC(ldr, LD_INSN, reg, addr)
#define STOREB(reg, addr)	EXC(sb, ST_INSN, reg, addr)
#define STOREL(reg, addr)	EXC(sdl, ST_INSN, reg, addr)
#define STORER(reg, addr)	EXC(sdr, ST_INSN, reg, addr)
#define STORE(reg, addr)	EXC(sd, ST_INSN, reg, addr)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#define ADD    daddu
#define SUB    dsubu
#define SRL    dsrl
#define SLL    dsll
#define SLLV   dsllv
#define SRLV   dsrlv
#define NBYTES 8
#define LOG_NBYTES 3

#else

<<<<<<< HEAD
#define LOAD   lw
#define LOADL  lwl
#define LOADR  lwr
#define STOREL swl
#define STORER swr
#define STORE  sw
=======
#define LOADK	lw /* No exception */
#define LOAD(reg, addr)		EXC(lw, LD_INSN, reg, addr)
#define LOADBU(reg, addr)	EXC(lbu, LD_INSN, reg, addr)
#define LOADL(reg, addr)	EXC(lwl, LD_INSN, reg, addr)
#define LOADR(reg, addr)	EXC(lwr, LD_INSN, reg, addr)
#define STOREB(reg, addr)	EXC(sb, ST_INSN, reg, addr)
#define STOREL(reg, addr)	EXC(swl, ST_INSN, reg, addr)
#define STORER(reg, addr)	EXC(swr, ST_INSN, reg, addr)
#define STORE(reg, addr)	EXC(sw, ST_INSN, reg, addr)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#define ADD    addu
#define SUB    subu
#define SRL    srl
#define SLL    sll
#define SLLV   sllv
#define SRLV   srlv
#define NBYTES 4
#define LOG_NBYTES 2

#endif /* USE_DOUBLE */

#ifdef CONFIG_CPU_LITTLE_ENDIAN
#define LDFIRST LOADR
<<<<<<< HEAD
#define LDREST  LOADL
#define STFIRST STORER
#define STREST  STOREL
=======
#define LDREST	LOADL
#define STFIRST STORER
#define STREST	STOREL
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#define SHIFT_DISCARD SLLV
#define SHIFT_DISCARD_REVERT SRLV
#else
#define LDFIRST LOADL
<<<<<<< HEAD
#define LDREST  LOADR
#define STFIRST STOREL
#define STREST  STORER
=======
#define LDREST	LOADR
#define STFIRST STOREL
#define STREST	STORER
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#define SHIFT_DISCARD SRLV
#define SHIFT_DISCARD_REVERT SLLV
#endif

#define FIRST(unit) ((unit)*NBYTES)
#define REST(unit)  (FIRST(unit)+NBYTES-1)

#define ADDRMASK (NBYTES-1)

#ifndef CONFIG_CPU_DADDI_WORKAROUNDS
	.set	noat
#else
	.set	at=v1
#endif

<<<<<<< HEAD
LEAF(__csum_partial_copy_user)
	PTR_ADDU	AT, src, len	/* See (1) above. */
#ifdef CONFIG_64BIT
	move	errptr, a4
#else
	lw	errptr, 16(sp)
#endif
FEXPORT(csum_partial_copy_nocheck)
	move	sum, zero
=======
	.macro __BUILD_CSUM_PARTIAL_COPY_USER mode, from, to

	li	sum, -1
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	move	odd, zero
	/*
	 * Note: dst & src may be unaligned, len may be 0
	 * Temps
	 */
	/*
	 * The "issue break"s below are very approximate.
	 * Issue delays for dcache fills will perturb the schedule, as will
	 * load queue full replay traps, etc.
	 *
	 * If len < NBYTES use byte operations.
	 */
	sltu	t2, len, NBYTES
	and	t1, dst, ADDRMASK
<<<<<<< HEAD
	bnez	t2, .Lcopy_bytes_checklen
	 and	t0, src, ADDRMASK
	andi	odd, dst, 0x1			/* odd buffer? */
	bnez	t1, .Ldst_unaligned
	 nop
	bnez	t0, .Lsrc_unaligned_dst_aligned
=======
	bnez	t2, .Lcopy_bytes_checklen\@
	 and	t0, src, ADDRMASK
	andi	odd, dst, 0x1			/* odd buffer? */
	bnez	t1, .Ldst_unaligned\@
	 nop
	bnez	t0, .Lsrc_unaligned_dst_aligned\@
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	/*
	 * use delay slot for fall-through
	 * src and dst are aligned; need to compute rem
	 */
<<<<<<< HEAD
.Lboth_aligned:
	 SRL	t0, len, LOG_NBYTES+3    # +3 for 8 units/iter
	beqz	t0, .Lcleanup_both_aligned # len < 8*NBYTES
=======
.Lboth_aligned\@:
	 SRL	t0, len, LOG_NBYTES+3	 # +3 for 8 units/iter
	beqz	t0, .Lcleanup_both_aligned\@ # len < 8*NBYTES
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	 nop
	SUB	len, 8*NBYTES		# subtract here for bgez loop
	.align	4
1:
<<<<<<< HEAD
EXC(	LOAD	t0, UNIT(0)(src),	.Ll_exc)
EXC(	LOAD	t1, UNIT(1)(src),	.Ll_exc_copy)
EXC(	LOAD	t2, UNIT(2)(src),	.Ll_exc_copy)
EXC(	LOAD	t3, UNIT(3)(src),	.Ll_exc_copy)
EXC(	LOAD	t4, UNIT(4)(src),	.Ll_exc_copy)
EXC(	LOAD	t5, UNIT(5)(src),	.Ll_exc_copy)
EXC(	LOAD	t6, UNIT(6)(src),	.Ll_exc_copy)
EXC(	LOAD	t7, UNIT(7)(src),	.Ll_exc_copy)
	SUB	len, len, 8*NBYTES
	ADD	src, src, 8*NBYTES
EXC(	STORE	t0, UNIT(0)(dst),	.Ls_exc)
	ADDC(sum, t0)
EXC(	STORE	t1, UNIT(1)(dst),	.Ls_exc)
	ADDC(sum, t1)
EXC(	STORE	t2, UNIT(2)(dst),	.Ls_exc)
	ADDC(sum, t2)
EXC(	STORE	t3, UNIT(3)(dst),	.Ls_exc)
	ADDC(sum, t3)
EXC(	STORE	t4, UNIT(4)(dst),	.Ls_exc)
	ADDC(sum, t4)
EXC(	STORE	t5, UNIT(5)(dst),	.Ls_exc)
	ADDC(sum, t5)
EXC(	STORE	t6, UNIT(6)(dst),	.Ls_exc)
	ADDC(sum, t6)
EXC(	STORE	t7, UNIT(7)(dst),	.Ls_exc)
	ADDC(sum, t7)
=======
	LOAD(t0, UNIT(0)(src))
	LOAD(t1, UNIT(1)(src))
	LOAD(t2, UNIT(2)(src))
	LOAD(t3, UNIT(3)(src))
	LOAD(t4, UNIT(4)(src))
	LOAD(t5, UNIT(5)(src))
	LOAD(t6, UNIT(6)(src))
	LOAD(t7, UNIT(7)(src))
	SUB	len, len, 8*NBYTES
	ADD	src, src, 8*NBYTES
	STORE(t0, UNIT(0)(dst))
	ADDC(t0, t1)
	STORE(t1, UNIT(1)(dst))
	ADDC(sum, t0)
	STORE(t2, UNIT(2)(dst))
	ADDC(t2, t3)
	STORE(t3, UNIT(3)(dst))
	ADDC(sum, t2)
	STORE(t4, UNIT(4)(dst))
	ADDC(t4, t5)
	STORE(t5, UNIT(5)(dst))
	ADDC(sum, t4)
	STORE(t6, UNIT(6)(dst))
	ADDC(t6, t7)
	STORE(t7, UNIT(7)(dst))
	ADDC(sum, t6)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	.set	reorder				/* DADDI_WAR */
	ADD	dst, dst, 8*NBYTES
	bgez	len, 1b
	.set	noreorder
	ADD	len, 8*NBYTES		# revert len (see above)

	/*
	 * len == the number of bytes left to copy < 8*NBYTES
	 */
<<<<<<< HEAD
.Lcleanup_both_aligned:
#define rem t7
	beqz	len, .Ldone
	 sltu	t0, len, 4*NBYTES
	bnez	t0, .Lless_than_4units
=======
.Lcleanup_both_aligned\@:
#define rem t7
	beqz	len, .Ldone\@
	 sltu	t0, len, 4*NBYTES
	bnez	t0, .Lless_than_4units\@
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	 and	rem, len, (NBYTES-1)	# rem = len % NBYTES
	/*
	 * len >= 4*NBYTES
	 */
<<<<<<< HEAD
EXC(	LOAD	t0, UNIT(0)(src),	.Ll_exc)
EXC(	LOAD	t1, UNIT(1)(src),	.Ll_exc_copy)
EXC(	LOAD	t2, UNIT(2)(src),	.Ll_exc_copy)
EXC(	LOAD	t3, UNIT(3)(src),	.Ll_exc_copy)
	SUB	len, len, 4*NBYTES
	ADD	src, src, 4*NBYTES
EXC(	STORE	t0, UNIT(0)(dst),	.Ls_exc)
	ADDC(sum, t0)
EXC(	STORE	t1, UNIT(1)(dst),	.Ls_exc)
	ADDC(sum, t1)
EXC(	STORE	t2, UNIT(2)(dst),	.Ls_exc)
	ADDC(sum, t2)
EXC(	STORE	t3, UNIT(3)(dst),	.Ls_exc)
	ADDC(sum, t3)
	.set	reorder				/* DADDI_WAR */
	ADD	dst, dst, 4*NBYTES
	beqz	len, .Ldone
	.set	noreorder
.Lless_than_4units:
	/*
	 * rem = len % NBYTES
	 */
	beq	rem, len, .Lcopy_bytes
	 nop
1:
EXC(	LOAD	t0, 0(src),		.Ll_exc)
	ADD	src, src, NBYTES
	SUB	len, len, NBYTES
EXC(	STORE	t0, 0(dst),		.Ls_exc)
=======
	LOAD(t0, UNIT(0)(src))
	LOAD(t1, UNIT(1)(src))
	LOAD(t2, UNIT(2)(src))
	LOAD(t3, UNIT(3)(src))
	SUB	len, len, 4*NBYTES
	ADD	src, src, 4*NBYTES
	STORE(t0, UNIT(0)(dst))
	ADDC(t0, t1)
	STORE(t1, UNIT(1)(dst))
	ADDC(sum, t0)
	STORE(t2, UNIT(2)(dst))
	ADDC(t2, t3)
	STORE(t3, UNIT(3)(dst))
	ADDC(sum, t2)
	.set	reorder				/* DADDI_WAR */
	ADD	dst, dst, 4*NBYTES
	beqz	len, .Ldone\@
	.set	noreorder
.Lless_than_4units\@:
	/*
	 * rem = len % NBYTES
	 */
	beq	rem, len, .Lcopy_bytes\@
	 nop
1:
	LOAD(t0, 0(src))
	ADD	src, src, NBYTES
	SUB	len, len, NBYTES
	STORE(t0, 0(dst))
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	ADDC(sum, t0)
	.set	reorder				/* DADDI_WAR */
	ADD	dst, dst, NBYTES
	bne	rem, len, 1b
	.set	noreorder

	/*
	 * src and dst are aligned, need to copy rem bytes (rem < NBYTES)
	 * A loop would do only a byte at a time with possible branch
<<<<<<< HEAD
	 * mispredicts.  Can't do an explicit LOAD dst,mask,or,STORE
=======
	 * mispredicts.	 Can't do an explicit LOAD dst,mask,or,STORE
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	 * because can't assume read-access to dst.  Instead, use
	 * STREST dst, which doesn't require read access to dst.
	 *
	 * This code should perform better than a simple loop on modern,
	 * wide-issue mips processors because the code has fewer branches and
	 * more instruction-level parallelism.
	 */
#define bits t2
<<<<<<< HEAD
	beqz	len, .Ldone
	 ADD	t1, dst, len	# t1 is just past last byte of dst
	li	bits, 8*NBYTES
	SLL	rem, len, 3	# rem = number of bits to keep
EXC(	LOAD	t0, 0(src),		.Ll_exc)
	SUB	bits, bits, rem	# bits = number of bits to discard
	SHIFT_DISCARD t0, t0, bits
EXC(	STREST	t0, -1(t1),		.Ls_exc)
	SHIFT_DISCARD_REVERT t0, t0, bits
	.set reorder
	ADDC(sum, t0)
	b	.Ldone
	.set noreorder
.Ldst_unaligned:
=======
	beqz	len, .Ldone\@
	 ADD	t1, dst, len	# t1 is just past last byte of dst
	li	bits, 8*NBYTES
	SLL	rem, len, 3	# rem = number of bits to keep
	LOAD(t0, 0(src))
	SUB	bits, bits, rem # bits = number of bits to discard
	SHIFT_DISCARD t0, t0, bits
	STREST(t0, -1(t1))
	SHIFT_DISCARD_REVERT t0, t0, bits
	.set reorder
	ADDC(sum, t0)
	b	.Ldone\@
	.set noreorder
.Ldst_unaligned\@:
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	/*
	 * dst is unaligned
	 * t0 = src & ADDRMASK
	 * t1 = dst & ADDRMASK; T1 > 0
	 * len >= NBYTES
	 *
	 * Copy enough bytes to align dst
	 * Set match = (src and dst have same alignment)
	 */
#define match rem
<<<<<<< HEAD
EXC(	LDFIRST	t3, FIRST(0)(src),	.Ll_exc)
	ADD	t2, zero, NBYTES
EXC(	LDREST	t3, REST(0)(src),	.Ll_exc_copy)
	SUB	t2, t2, t1	# t2 = number of bytes copied
	xor	match, t0, t1
EXC(	STFIRST t3, FIRST(0)(dst),	.Ls_exc)
=======
	LDFIRST(t3, FIRST(0)(src))
	ADD	t2, zero, NBYTES
	LDREST(t3, REST(0)(src))
	SUB	t2, t2, t1	# t2 = number of bytes copied
	xor	match, t0, t1
	STFIRST(t3, FIRST(0)(dst))
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	SLL	t4, t1, 3		# t4 = number of bits to discard
	SHIFT_DISCARD t3, t3, t4
	/* no SHIFT_DISCARD_REVERT to handle odd buffer properly */
	ADDC(sum, t3)
<<<<<<< HEAD
	beq	len, t2, .Ldone
	 SUB	len, len, t2
	ADD	dst, dst, t2
	beqz	match, .Lboth_aligned
	 ADD	src, src, t2

.Lsrc_unaligned_dst_aligned:
	SRL	t0, len, LOG_NBYTES+2    # +2 for 4 units/iter
	beqz	t0, .Lcleanup_src_unaligned
	 and	rem, len, (4*NBYTES-1)   # rem = len % 4*NBYTES
=======
	beq	len, t2, .Ldone\@
	 SUB	len, len, t2
	ADD	dst, dst, t2
	beqz	match, .Lboth_aligned\@
	 ADD	src, src, t2

.Lsrc_unaligned_dst_aligned\@:
	SRL	t0, len, LOG_NBYTES+2	 # +2 for 4 units/iter
	beqz	t0, .Lcleanup_src_unaligned\@
	 and	rem, len, (4*NBYTES-1)	 # rem = len % 4*NBYTES
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
1:
/*
 * Avoid consecutive LD*'s to the same register since some mips
 * implementations can't issue them in the same cycle.
 * It's OK to load FIRST(N+1) before REST(N) because the two addresses
 * are to the same unit (unless src is aligned, but it's not).
 */
<<<<<<< HEAD
EXC(	LDFIRST	t0, FIRST(0)(src),	.Ll_exc)
EXC(	LDFIRST	t1, FIRST(1)(src),	.Ll_exc_copy)
	SUB     len, len, 4*NBYTES
EXC(	LDREST	t0, REST(0)(src),	.Ll_exc_copy)
EXC(	LDREST	t1, REST(1)(src),	.Ll_exc_copy)
EXC(	LDFIRST	t2, FIRST(2)(src),	.Ll_exc_copy)
EXC(	LDFIRST	t3, FIRST(3)(src),	.Ll_exc_copy)
EXC(	LDREST	t2, REST(2)(src),	.Ll_exc_copy)
EXC(	LDREST	t3, REST(3)(src),	.Ll_exc_copy)
=======
	LDFIRST(t0, FIRST(0)(src))
	LDFIRST(t1, FIRST(1)(src))
	SUB	len, len, 4*NBYTES
	LDREST(t0, REST(0)(src))
	LDREST(t1, REST(1)(src))
	LDFIRST(t2, FIRST(2)(src))
	LDFIRST(t3, FIRST(3)(src))
	LDREST(t2, REST(2)(src))
	LDREST(t3, REST(3)(src))
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	ADD	src, src, 4*NBYTES
#ifdef CONFIG_CPU_SB1
	nop				# improves slotting
#endif
<<<<<<< HEAD
EXC(	STORE	t0, UNIT(0)(dst),	.Ls_exc)
	ADDC(sum, t0)
EXC(	STORE	t1, UNIT(1)(dst),	.Ls_exc)
	ADDC(sum, t1)
EXC(	STORE	t2, UNIT(2)(dst),	.Ls_exc)
	ADDC(sum, t2)
EXC(	STORE	t3, UNIT(3)(dst),	.Ls_exc)
	ADDC(sum, t3)
=======
	STORE(t0, UNIT(0)(dst))
	ADDC(t0, t1)
	STORE(t1, UNIT(1)(dst))
	ADDC(sum, t0)
	STORE(t2, UNIT(2)(dst))
	ADDC(t2, t3)
	STORE(t3, UNIT(3)(dst))
	ADDC(sum, t2)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	.set	reorder				/* DADDI_WAR */
	ADD	dst, dst, 4*NBYTES
	bne	len, rem, 1b
	.set	noreorder

<<<<<<< HEAD
.Lcleanup_src_unaligned:
	beqz	len, .Ldone
	 and	rem, len, NBYTES-1  # rem = len % NBYTES
	beq	rem, len, .Lcopy_bytes
	 nop
1:
EXC(	LDFIRST t0, FIRST(0)(src),	.Ll_exc)
EXC(	LDREST	t0, REST(0)(src),	.Ll_exc_copy)
	ADD	src, src, NBYTES
	SUB	len, len, NBYTES
EXC(	STORE	t0, 0(dst),		.Ls_exc)
=======
.Lcleanup_src_unaligned\@:
	beqz	len, .Ldone\@
	 and	rem, len, NBYTES-1  # rem = len % NBYTES
	beq	rem, len, .Lcopy_bytes\@
	 nop
1:
	LDFIRST(t0, FIRST(0)(src))
	LDREST(t0, REST(0)(src))
	ADD	src, src, NBYTES
	SUB	len, len, NBYTES
	STORE(t0, 0(dst))
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	ADDC(sum, t0)
	.set	reorder				/* DADDI_WAR */
	ADD	dst, dst, NBYTES
	bne	len, rem, 1b
	.set	noreorder

<<<<<<< HEAD
.Lcopy_bytes_checklen:
	beqz	len, .Ldone
	 nop
.Lcopy_bytes:
=======
.Lcopy_bytes_checklen\@:
	beqz	len, .Ldone\@
	 nop
.Lcopy_bytes\@:
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	/* 0 < len < NBYTES  */
#ifdef CONFIG_CPU_LITTLE_ENDIAN
#define SHIFT_START 0
#define SHIFT_INC 8
#else
#define SHIFT_START 8*(NBYTES-1)
#define SHIFT_INC -8
#endif
	move	t2, zero	# partial word
<<<<<<< HEAD
	li	t3, SHIFT_START	# shift
/* use .Ll_exc_copy here to return correct sum on fault */
#define COPY_BYTE(N)			\
EXC(	lbu	t0, N(src), .Ll_exc_copy);	\
	SUB	len, len, 1;		\
EXC(	sb	t0, N(dst), .Ls_exc);	\
	SLLV	t0, t0, t3;		\
	addu	t3, SHIFT_INC;		\
	beqz	len, .Lcopy_bytes_done;	\
=======
	li	t3, SHIFT_START # shift
#define COPY_BYTE(N)			\
	LOADBU(t0, N(src));		\
	SUB	len, len, 1;		\
	STOREB(t0, N(dst));		\
	SLLV	t0, t0, t3;		\
	addu	t3, SHIFT_INC;		\
	beqz	len, .Lcopy_bytes_done\@; \
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	 or	t2, t0

	COPY_BYTE(0)
	COPY_BYTE(1)
#ifdef USE_DOUBLE
	COPY_BYTE(2)
	COPY_BYTE(3)
	COPY_BYTE(4)
	COPY_BYTE(5)
#endif
<<<<<<< HEAD
EXC(	lbu	t0, NBYTES-2(src), .Ll_exc_copy)
	SUB	len, len, 1
EXC(	sb	t0, NBYTES-2(dst), .Ls_exc)
	SLLV	t0, t0, t3
	or	t2, t0
.Lcopy_bytes_done:
	ADDC(sum, t2)
.Ldone:
	/* fold checksum */
=======
	LOADBU(t0, NBYTES-2(src))
	SUB	len, len, 1
	STOREB(t0, NBYTES-2(dst))
	SLLV	t0, t0, t3
	or	t2, t0
.Lcopy_bytes_done\@:
	ADDC(sum, t2)
.Ldone\@:
	/* fold checksum */
	.set	push
	.set	noat
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#ifdef USE_DOUBLE
	dsll32	v1, sum, 0
	daddu	sum, v1
	sltu	v1, sum, v1
	dsra32	sum, sum, 0
	addu	sum, v1
#endif

<<<<<<< HEAD
#ifdef CPU_MIPSR2
	wsbh	v1, sum
	movn	sum, v1, odd
=======
#if defined(CONFIG_CPU_MIPSR2) || defined(CONFIG_CPU_MIPSR5) || \
    defined(CONFIG_CPU_LOONGSON64)
	.set	push
	.set	arch=mips32r2
	wsbh	v1, sum
	movn	sum, v1, odd
	.set	pop
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#else
	beqz	odd, 1f			/* odd buffer alignment? */
	 lui	v1, 0x00ff
	addu	v1, 0x00ff
	and	t0, sum, v1
	sll	t0, t0, 8
	srl	sum, sum, 8
	and	sum, sum, v1
	or	sum, sum, t0
1:
#endif
<<<<<<< HEAD
	.set reorder
	ADDC32(sum, psum)
	jr	ra
	.set noreorder

.Ll_exc_copy:
	/*
	 * Copy bytes from src until faulting load address (or until a
	 * lb faults)
	 *
	 * When reached by a faulting LDFIRST/LDREST, THREAD_BUADDR($28)
	 * may be more than a byte beyond the last address.
	 * Hence, the lb below may get an exception.
	 *
	 * Assumes src < THREAD_BUADDR($28)
	 */
	LOAD	t0, TI_TASK($28)
	 li	t2, SHIFT_START
	LOAD	t0, THREAD_BUADDR(t0)
1:
EXC(	lbu	t1, 0(src),	.Ll_exc)
	ADD	src, src, 1
	sb	t1, 0(dst)	# can't fault -- we're copy_from_user
	SLLV	t1, t1, t2
	addu	t2, SHIFT_INC
	ADDC(sum, t1)
	.set	reorder				/* DADDI_WAR */
	ADD	dst, dst, 1
	bne	src, t0, 1b
	.set	noreorder
.Ll_exc:
	LOAD	t0, TI_TASK($28)
	 nop
	LOAD	t0, THREAD_BUADDR(t0)	# t0 is just past last good address
	 nop
	SUB	len, AT, t0		# len number of uncopied bytes
	/*
	 * Here's where we rely on src and dst being incremented in tandem,
	 *   See (3) above.
	 * dst += (fault addr - src) to put dst at first byte to clear
	 */
	ADD	dst, t0			# compute start address in a1
	SUB	dst, src
	/*
	 * Clear len bytes starting at dst.  Can't call __bzero because it
	 * might modify len.  An inefficient loop for these rare times...
	 */
	.set	reorder				/* DADDI_WAR */
	SUB	src, len, 1
	beqz	len, .Ldone
	.set	noreorder
1:	sb	zero, 0(dst)
	ADD	dst, dst, 1
	.set	push
	.set	noat
#ifndef CONFIG_CPU_DADDI_WORKAROUNDS
	bnez	src, 1b
	 SUB	src, src, 1
#else
	li	v1, 1
	bnez	src, 1b
	 SUB	src, src, v1
#endif
	li	v1, -EFAULT
	b	.Ldone
	 sw	v1, (errptr)

.Ls_exc:
	li	v0, -1 /* invalid checksum */
	li	v1, -EFAULT
	jr	ra
	 sw	v1, (errptr)
	.set	pop
	END(__csum_partial_copy_user)
=======
	.set	pop
	.set reorder
	jr	ra
	.set noreorder
	.endm

	.set noreorder
.L_exc:
	jr	ra
	 li	v0, 0

FEXPORT(__csum_partial_copy_nocheck)
EXPORT_SYMBOL(__csum_partial_copy_nocheck)
#ifndef CONFIG_EVA
FEXPORT(__csum_partial_copy_to_user)
EXPORT_SYMBOL(__csum_partial_copy_to_user)
FEXPORT(__csum_partial_copy_from_user)
EXPORT_SYMBOL(__csum_partial_copy_from_user)
#endif
__BUILD_CSUM_PARTIAL_COPY_USER LEGACY_MODE USEROP USEROP

#ifdef CONFIG_EVA
LEAF(__csum_partial_copy_to_user)
__BUILD_CSUM_PARTIAL_COPY_USER EVA_MODE KERNELOP USEROP
END(__csum_partial_copy_to_user)

LEAF(__csum_partial_copy_from_user)
__BUILD_CSUM_PARTIAL_COPY_USER EVA_MODE USEROP KERNELOP
END(__csum_partial_copy_from_user)
#endif
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
