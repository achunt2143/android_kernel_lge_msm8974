<<<<<<< HEAD
=======
// SPDX-License-Identifier: GPL-2.0-only
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
/******************************************************************************
 * emulate.c
 *
 * Generic x86 (32-bit and 64-bit) instruction decoder and emulator.
 *
 * Copyright (c) 2005 Keir Fraser
 *
 * Linux coding style, mod r/m decoder, segment base fixes, real-mode
 * privileged instructions:
 *
 * Copyright (C) 2006 Qumranet
 * Copyright 2010 Red Hat, Inc. and/or its affiliates.
 *
 *   Avi Kivity <avi@qumranet.com>
 *   Yaniv Kamay <yaniv@qumranet.com>
 *
<<<<<<< HEAD
 * This work is licensed under the terms of the GNU GPL, version 2.  See
 * the COPYING file in the top-level directory.
 *
 * From: xen-unstable 10676:af9809f51f81a3c43f276f00c81a52ef558afda4
 */

#include <linux/kvm_host.h>
#include "kvm_cache_regs.h"
#include <linux/module.h>
#include <asm/kvm_emulate.h>

#include "x86.h"
#include "tss.h"
=======
 * From: xen-unstable 10676:af9809f51f81a3c43f276f00c81a52ef558afda4
 */
#define pr_fmt(fmt) KBUILD_MODNAME ": " fmt

#include <linux/kvm_host.h>
#include "kvm_cache_regs.h"
#include "kvm_emulate.h"
#include <linux/stringify.h>
#include <asm/debugreg.h>
#include <asm/nospec-branch.h>
#include <asm/ibt.h>

#include "x86.h"
#include "tss.h"
#include "mmu.h"
#include "pmu.h"
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

/*
 * Operand types
 */
#define OpNone             0ull
#define OpImplicit         1ull  /* No generic decode */
#define OpReg              2ull  /* Register */
#define OpMem              3ull  /* Memory */
#define OpAcc              4ull  /* Accumulator: AL/AX/EAX/RAX */
#define OpDI               5ull  /* ES:DI/EDI/RDI */
#define OpMem64            6ull  /* Memory, 64-bit */
#define OpImmUByte         7ull  /* Zero-extended 8-bit immediate */
#define OpDX               8ull  /* DX register */
#define OpCL               9ull  /* CL register (for shifts) */
#define OpImmByte         10ull  /* 8-bit sign extended immediate */
#define OpOne             11ull  /* Implied 1 */
<<<<<<< HEAD
#define OpImm             12ull  /* Sign extended immediate */
=======
#define OpImm             12ull  /* Sign extended up to 32-bit immediate */
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#define OpMem16           13ull  /* Memory operand (16-bit). */
#define OpMem32           14ull  /* Memory operand (32-bit). */
#define OpImmU            15ull  /* Immediate operand, zero extended */
#define OpSI              16ull  /* SI/ESI/RSI */
#define OpImmFAddr        17ull  /* Immediate far address */
#define OpMemFAddr        18ull  /* Far address in memory */
#define OpImmU16          19ull  /* Immediate operand, 16 bits, zero extended */
#define OpES              20ull  /* ES */
#define OpCS              21ull  /* CS */
#define OpSS              22ull  /* SS */
#define OpDS              23ull  /* DS */
#define OpFS              24ull  /* FS */
#define OpGS              25ull  /* GS */
#define OpMem8            26ull  /* 8-bit zero extended memory operand */
<<<<<<< HEAD
=======
#define OpImm64           27ull  /* Sign extended 16/32/64-bit immediate */
#define OpXLat            28ull  /* memory at BX/EBX/RBX + zero-extended AL */
#define OpAccLo           29ull  /* Low part of extended acc (AX/AX/EAX/RAX) */
#define OpAccHi           30ull  /* High part of extended acc (-/DX/EDX/RDX) */
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

#define OpBits             5  /* Width of operand field */
#define OpMask             ((1ull << OpBits) - 1)

/*
 * Opcode effective-address decode tables.
 * Note that we only emulate instructions that have at least one memory
 * operand (excluding implicit stack references). We assume that stack
 * references and instruction fetches will never occur in special memory
 * areas that require emulation. So, for example, 'mov <imm>,<reg>' need
 * not be handled.
 */

/* Operand sizes: 8-bit operands or specified/overridden size. */
#define ByteOp      (1<<0)	/* 8-bit operands. */
/* Destination operand type. */
#define DstShift    1
#define ImplicitOps (OpImplicit << DstShift)
#define DstReg      (OpReg << DstShift)
#define DstMem      (OpMem << DstShift)
#define DstAcc      (OpAcc << DstShift)
#define DstDI       (OpDI << DstShift)
#define DstMem64    (OpMem64 << DstShift)
<<<<<<< HEAD
#define DstImmUByte (OpImmUByte << DstShift)
#define DstDX       (OpDX << DstShift)
=======
#define DstMem16    (OpMem16 << DstShift)
#define DstImmUByte (OpImmUByte << DstShift)
#define DstDX       (OpDX << DstShift)
#define DstAccLo    (OpAccLo << DstShift)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#define DstMask     (OpMask << DstShift)
/* Source operand type. */
#define SrcShift    6
#define SrcNone     (OpNone << SrcShift)
#define SrcReg      (OpReg << SrcShift)
#define SrcMem      (OpMem << SrcShift)
#define SrcMem16    (OpMem16 << SrcShift)
#define SrcMem32    (OpMem32 << SrcShift)
#define SrcImm      (OpImm << SrcShift)
#define SrcImmByte  (OpImmByte << SrcShift)
#define SrcOne      (OpOne << SrcShift)
#define SrcImmUByte (OpImmUByte << SrcShift)
#define SrcImmU     (OpImmU << SrcShift)
#define SrcSI       (OpSI << SrcShift)
<<<<<<< HEAD
=======
#define SrcXLat     (OpXLat << SrcShift)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#define SrcImmFAddr (OpImmFAddr << SrcShift)
#define SrcMemFAddr (OpMemFAddr << SrcShift)
#define SrcAcc      (OpAcc << SrcShift)
#define SrcImmU16   (OpImmU16 << SrcShift)
<<<<<<< HEAD
#define SrcDX       (OpDX << SrcShift)
#define SrcMem8     (OpMem8 << SrcShift)
=======
#define SrcImm64    (OpImm64 << SrcShift)
#define SrcDX       (OpDX << SrcShift)
#define SrcMem8     (OpMem8 << SrcShift)
#define SrcAccHi    (OpAccHi << SrcShift)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#define SrcMask     (OpMask << SrcShift)
#define BitOp       (1<<11)
#define MemAbs      (1<<12)      /* Memory operand is absolute displacement */
#define String      (1<<13)     /* String instruction (rep capable) */
#define Stack       (1<<14)     /* Stack instruction (push/pop) */
#define GroupMask   (7<<15)     /* Opcode uses one of the group mechanisms */
#define Group       (1<<15)     /* Bits 3:5 of modrm byte extend opcode */
#define GroupDual   (2<<15)     /* Alternate decoding of mod == 3 */
#define Prefix      (3<<15)     /* Instruction varies with 66/f2/f3 prefix */
#define RMExt       (4<<15)     /* Opcode extension in ModRM r/m if mod == 3 */
<<<<<<< HEAD
=======
#define Escape      (5<<15)     /* Escape to coprocessor instruction */
#define InstrDual   (6<<15)     /* Alternate instruction decoding of mod == 3 */
#define ModeDual    (7<<15)     /* Different instruction for 32/64 bit */
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#define Sse         (1<<18)     /* SSE Vector instruction */
/* Generic ModRM decode. */
#define ModRM       (1<<19)
/* Destination is only written; never read. */
#define Mov         (1<<20)
/* Misc flags */
#define Prot        (1<<21) /* instruction generates #UD if not in prot-mode */
<<<<<<< HEAD
#define VendorSpecific (1<<22) /* Vendor specific instruction */
=======
#define EmulateOnUD (1<<22) /* Emulate if unsupported by the host */
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#define NoAccess    (1<<23) /* Don't access memory (lea/invlpg/verr etc) */
#define Op3264      (1<<24) /* Operand is 64b in long mode, 32b otherwise */
#define Undefined   (1<<25) /* No Such Instruction */
#define Lock        (1<<26) /* lock prefix is allowed for the instruction */
#define Priv        (1<<27) /* instruction generates #GP if current CPL != 0 */
#define No64	    (1<<28)
#define PageTable   (1 << 29)   /* instruction used to write page table */
<<<<<<< HEAD
/* Source 2 operand type */
#define Src2Shift   (30)
#define Src2None    (OpNone << Src2Shift)
=======
#define NotImpl     (1 << 30)   /* instruction is not implemented */
/* Source 2 operand type */
#define Src2Shift   (31)
#define Src2None    (OpNone << Src2Shift)
#define Src2Mem     (OpMem << Src2Shift)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#define Src2CL      (OpCL << Src2Shift)
#define Src2ImmByte (OpImmByte << Src2Shift)
#define Src2One     (OpOne << Src2Shift)
#define Src2Imm     (OpImm << Src2Shift)
#define Src2ES      (OpES << Src2Shift)
#define Src2CS      (OpCS << Src2Shift)
#define Src2SS      (OpSS << Src2Shift)
#define Src2DS      (OpDS << Src2Shift)
#define Src2FS      (OpFS << Src2Shift)
#define Src2GS      (OpGS << Src2Shift)
#define Src2Mask    (OpMask << Src2Shift)
<<<<<<< HEAD
=======
#define Mmx         ((u64)1 << 40)  /* MMX Vector instruction */
#define AlignMask   ((u64)7 << 41)
#define Aligned     ((u64)1 << 41)  /* Explicitly aligned (e.g. MOVDQA) */
#define Unaligned   ((u64)2 << 41)  /* Explicitly unaligned (e.g. MOVDQU) */
#define Avx         ((u64)3 << 41)  /* Advanced Vector Extensions */
#define Aligned16   ((u64)4 << 41)  /* Aligned to 16 byte boundary (e.g. FXSAVE) */
#define Fastop      ((u64)1 << 44)  /* Use opcode::u.fastop */
#define NoWrite     ((u64)1 << 45)  /* No writeback */
#define SrcWrite    ((u64)1 << 46)  /* Write back src operand */
#define NoMod	    ((u64)1 << 47)  /* Mod field is ignored */
#define Intercept   ((u64)1 << 48)  /* Has valid intercept field */
#define CheckPerm   ((u64)1 << 49)  /* Has valid check_perm field */
#define PrivUD      ((u64)1 << 51)  /* #UD instead of #GP on CPL > 0 */
#define NearBranch  ((u64)1 << 52)  /* Near branches */
#define No16	    ((u64)1 << 53)  /* No 16 bit operand */
#define IncSP       ((u64)1 << 54)  /* SP is incremented before ModRM calc */
#define TwoMemOp    ((u64)1 << 55)  /* Instruction has two memory operand */
#define IsBranch    ((u64)1 << 56)  /* Instruction is considered a branch. */

#define DstXacc     (DstAccLo | SrcAccHi | SrcWrite)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

#define X2(x...) x, x
#define X3(x...) X2(x), x
#define X4(x...) X2(x), X2(x)
#define X5(x...) X4(x), x
#define X6(x...) X4(x), X2(x)
#define X7(x...) X4(x), X3(x)
#define X8(x...) X4(x), X4(x)
#define X16(x...) X8(x), X8(x)

struct opcode {
<<<<<<< HEAD
	u64 flags : 56;
	u64 intercept : 8;
	union {
		int (*execute)(struct x86_emulate_ctxt *ctxt);
		struct opcode *group;
		struct group_dual *gdual;
		struct gprefix *gprefix;
=======
	u64 flags;
	u8 intercept;
	u8 pad[7];
	union {
		int (*execute)(struct x86_emulate_ctxt *ctxt);
		const struct opcode *group;
		const struct group_dual *gdual;
		const struct gprefix *gprefix;
		const struct escape *esc;
		const struct instr_dual *idual;
		const struct mode_dual *mdual;
		void (*fastop)(struct fastop *fake);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	} u;
	int (*check_perm)(struct x86_emulate_ctxt *ctxt);
};

struct group_dual {
	struct opcode mod012[8];
	struct opcode mod3[8];
};

struct gprefix {
	struct opcode pfx_no;
	struct opcode pfx_66;
	struct opcode pfx_f2;
	struct opcode pfx_f3;
};

<<<<<<< HEAD
/* EFLAGS bit definitions. */
#define EFLG_ID (1<<21)
#define EFLG_VIP (1<<20)
#define EFLG_VIF (1<<19)
#define EFLG_AC (1<<18)
#define EFLG_VM (1<<17)
#define EFLG_RF (1<<16)
#define EFLG_IOPL (3<<12)
#define EFLG_NT (1<<14)
#define EFLG_OF (1<<11)
#define EFLG_DF (1<<10)
#define EFLG_IF (1<<9)
#define EFLG_TF (1<<8)
#define EFLG_SF (1<<7)
#define EFLG_ZF (1<<6)
#define EFLG_AF (1<<4)
#define EFLG_PF (1<<2)
#define EFLG_CF (1<<0)

#define EFLG_RESERVED_ZEROS_MASK 0xffc0802a
#define EFLG_RESERVED_ONE_MASK 2

/*
 * Instruction emulation:
 * Most instructions are emulated directly via a fragment of inline assembly
 * code. This allows us to save/restore EFLAGS and thus very easily pick up
 * any modified flags.
 */

#if defined(CONFIG_X86_64)
#define _LO32 "k"		/* force 32-bit operand */
#define _STK  "%%rsp"		/* stack pointer */
#elif defined(__i386__)
#define _LO32 ""		/* force 32-bit operand */
#define _STK  "%%esp"		/* stack pointer */
#endif
=======
struct escape {
	struct opcode op[8];
	struct opcode high[64];
};

struct instr_dual {
	struct opcode mod012;
	struct opcode mod3;
};

struct mode_dual {
	struct opcode mode32;
	struct opcode mode64;
};

#define EFLG_RESERVED_ZEROS_MASK 0xffc0802a

enum x86_transfer_type {
	X86_TRANSFER_NONE,
	X86_TRANSFER_CALL_JMP,
	X86_TRANSFER_RET,
	X86_TRANSFER_TASK_SWITCH,
};

static void writeback_registers(struct x86_emulate_ctxt *ctxt)
{
	unsigned long dirty = ctxt->regs_dirty;
	unsigned reg;

	for_each_set_bit(reg, &dirty, NR_EMULATOR_GPRS)
		ctxt->ops->write_gpr(ctxt, reg, ctxt->_regs[reg]);
}

static void invalidate_registers(struct x86_emulate_ctxt *ctxt)
{
	ctxt->regs_dirty = 0;
	ctxt->regs_valid = 0;
}
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

/*
 * These EFLAGS bits are restored from saved value during emulation, and
 * any changes are written back to the saved value after emulation.
 */
<<<<<<< HEAD
#define EFLAGS_MASK (EFLG_OF|EFLG_SF|EFLG_ZF|EFLG_AF|EFLG_PF|EFLG_CF)

/* Before executing instruction: restore necessary bits in EFLAGS. */
#define _PRE_EFLAGS(_sav, _msk, _tmp)					\
	/* EFLAGS = (_sav & _msk) | (EFLAGS & ~_msk); _sav &= ~_msk; */ \
	"movl %"_sav",%"_LO32 _tmp"; "                                  \
	"push %"_tmp"; "                                                \
	"push %"_tmp"; "                                                \
	"movl %"_msk",%"_LO32 _tmp"; "                                  \
	"andl %"_LO32 _tmp",("_STK"); "                                 \
	"pushf; "                                                       \
	"notl %"_LO32 _tmp"; "                                          \
	"andl %"_LO32 _tmp",("_STK"); "                                 \
	"andl %"_LO32 _tmp","__stringify(BITS_PER_LONG/4)"("_STK"); "	\
	"pop  %"_tmp"; "                                                \
	"orl  %"_LO32 _tmp",("_STK"); "                                 \
	"popf; "                                                        \
	"pop  %"_sav"; "

/* After executing instruction: write-back necessary bits in EFLAGS. */
#define _POST_EFLAGS(_sav, _msk, _tmp) \
	/* _sav |= EFLAGS & _msk; */		\
	"pushf; "				\
	"pop  %"_tmp"; "			\
	"andl %"_msk",%"_LO32 _tmp"; "		\
	"orl  %"_LO32 _tmp",%"_sav"; "
=======
#define EFLAGS_MASK (X86_EFLAGS_OF|X86_EFLAGS_SF|X86_EFLAGS_ZF|X86_EFLAGS_AF|\
		     X86_EFLAGS_PF|X86_EFLAGS_CF)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

#ifdef CONFIG_X86_64
#define ON64(x) x
#else
#define ON64(x)
#endif

<<<<<<< HEAD
#define ____emulate_2op(ctxt, _op, _x, _y, _suffix, _dsttype)	\
	do {								\
		__asm__ __volatile__ (					\
			_PRE_EFLAGS("0", "4", "2")			\
			_op _suffix " %"_x"3,%1; "			\
			_POST_EFLAGS("0", "4", "2")			\
			: "=m" ((ctxt)->eflags),			\
			  "+q" (*(_dsttype*)&(ctxt)->dst.val),		\
			  "=&r" (_tmp)					\
			: _y ((ctxt)->src.val), "i" (EFLAGS_MASK));	\
	} while (0)


/* Raw emulation: instruction has two explicit operands. */
#define __emulate_2op_nobyte(ctxt,_op,_wx,_wy,_lx,_ly,_qx,_qy)		\
	do {								\
		unsigned long _tmp;					\
									\
		switch ((ctxt)->dst.bytes) {				\
		case 2:							\
			____emulate_2op(ctxt,_op,_wx,_wy,"w",u16);	\
			break;						\
		case 4:							\
			____emulate_2op(ctxt,_op,_lx,_ly,"l",u32);	\
			break;						\
		case 8:							\
			ON64(____emulate_2op(ctxt,_op,_qx,_qy,"q",u64)); \
			break;						\
		}							\
	} while (0)

#define __emulate_2op(ctxt,_op,_bx,_by,_wx,_wy,_lx,_ly,_qx,_qy)		     \
	do {								     \
		unsigned long _tmp;					     \
		switch ((ctxt)->dst.bytes) {				     \
		case 1:							     \
			____emulate_2op(ctxt,_op,_bx,_by,"b",u8);	     \
			break;						     \
		default:						     \
			__emulate_2op_nobyte(ctxt, _op,			     \
					     _wx, _wy, _lx, _ly, _qx, _qy);  \
			break;						     \
		}							     \
	} while (0)

/* Source operand is byte-sized and may be restricted to just %cl. */
#define emulate_2op_SrcB(ctxt, _op)					\
	__emulate_2op(ctxt, _op, "b", "c", "b", "c", "b", "c", "b", "c")

/* Source operand is byte, word, long or quad sized. */
#define emulate_2op_SrcV(ctxt, _op)					\
	__emulate_2op(ctxt, _op, "b", "q", "w", "r", _LO32, "r", "", "r")

/* Source operand is word, long or quad sized. */
#define emulate_2op_SrcV_nobyte(ctxt, _op)				\
	__emulate_2op_nobyte(ctxt, _op, "w", "r", _LO32, "r", "", "r")

/* Instruction has three operands and one operand is stored in ECX register */
#define __emulate_2op_cl(ctxt, _op, _suffix, _type)		\
	do {								\
		unsigned long _tmp;					\
		_type _clv  = (ctxt)->src2.val;				\
		_type _srcv = (ctxt)->src.val;				\
		_type _dstv = (ctxt)->dst.val;				\
									\
		__asm__ __volatile__ (					\
			_PRE_EFLAGS("0", "5", "2")			\
			_op _suffix " %4,%1 \n"				\
			_POST_EFLAGS("0", "5", "2")			\
			: "=m" ((ctxt)->eflags), "+r" (_dstv), "=&r" (_tmp) \
			: "c" (_clv) , "r" (_srcv), "i" (EFLAGS_MASK)	\
			);						\
									\
		(ctxt)->src2.val  = (unsigned long) _clv;		\
		(ctxt)->src2.val = (unsigned long) _srcv;		\
		(ctxt)->dst.val = (unsigned long) _dstv;		\
	} while (0)

#define emulate_2op_cl(ctxt, _op)					\
	do {								\
		switch ((ctxt)->dst.bytes) {				\
		case 2:							\
			__emulate_2op_cl(ctxt, _op, "w", u16);		\
			break;						\
		case 4:							\
			__emulate_2op_cl(ctxt, _op, "l", u32);		\
			break;						\
		case 8:							\
			ON64(__emulate_2op_cl(ctxt, _op, "q", ulong));	\
			break;						\
		}							\
	} while (0)

#define __emulate_1op(ctxt, _op, _suffix)				\
	do {								\
		unsigned long _tmp;					\
									\
		__asm__ __volatile__ (					\
			_PRE_EFLAGS("0", "3", "2")			\
			_op _suffix " %1; "				\
			_POST_EFLAGS("0", "3", "2")			\
			: "=m" ((ctxt)->eflags), "+m" ((ctxt)->dst.val), \
			  "=&r" (_tmp)					\
			: "i" (EFLAGS_MASK));				\
	} while (0)

/* Instruction has only one explicit operand (no source operand). */
#define emulate_1op(ctxt, _op)						\
	do {								\
		switch ((ctxt)->dst.bytes) {				\
		case 1:	__emulate_1op(ctxt, _op, "b"); break;		\
		case 2:	__emulate_1op(ctxt, _op, "w"); break;		\
		case 4:	__emulate_1op(ctxt, _op, "l"); break;		\
		case 8:	ON64(__emulate_1op(ctxt, _op, "q")); break;	\
		}							\
	} while (0)

#define __emulate_1op_rax_rdx(ctxt, _op, _suffix, _ex)			\
	do {								\
		unsigned long _tmp;					\
		ulong *rax = &(ctxt)->regs[VCPU_REGS_RAX];		\
		ulong *rdx = &(ctxt)->regs[VCPU_REGS_RDX];		\
									\
		__asm__ __volatile__ (					\
			_PRE_EFLAGS("0", "5", "1")			\
			"1: \n\t"					\
			_op _suffix " %6; "				\
			"2: \n\t"					\
			_POST_EFLAGS("0", "5", "1")			\
			".pushsection .fixup,\"ax\" \n\t"		\
			"3: movb $1, %4 \n\t"				\
			"jmp 2b \n\t"					\
			".popsection \n\t"				\
			_ASM_EXTABLE(1b, 3b)				\
			: "=m" ((ctxt)->eflags), "=&r" (_tmp),		\
			  "+a" (*rax), "+d" (*rdx), "+qm"(_ex)		\
			: "i" (EFLAGS_MASK), "m" ((ctxt)->src.val),	\
			  "a" (*rax), "d" (*rdx));			\
	} while (0)

/* instruction has only one source operand, destination is implicit (e.g. mul, div, imul, idiv) */
#define emulate_1op_rax_rdx(ctxt, _op, _ex)	\
	do {								\
		switch((ctxt)->src.bytes) {				\
		case 1:							\
			__emulate_1op_rax_rdx(ctxt, _op, "b", _ex);	\
			break;						\
		case 2:							\
			__emulate_1op_rax_rdx(ctxt, _op, "w", _ex);	\
			break;						\
		case 4:							\
			__emulate_1op_rax_rdx(ctxt, _op, "l", _ex);	\
			break;						\
		case 8: ON64(						\
			__emulate_1op_rax_rdx(ctxt, _op, "q", _ex));	\
			break;						\
		}							\
	} while (0)
=======
/*
 * fastop functions have a special calling convention:
 *
 * dst:    rax        (in/out)
 * src:    rdx        (in/out)
 * src2:   rcx        (in)
 * flags:  rflags     (in/out)
 * ex:     rsi        (in:fastop pointer, out:zero if exception)
 *
 * Moreover, they are all exactly FASTOP_SIZE bytes long, so functions for
 * different operand sizes can be reached by calculation, rather than a jump
 * table (which would be bigger than the code).
 *
 * The 16 byte alignment, considering 5 bytes for the RET thunk, 3 for ENDBR
 * and 1 for the straight line speculation INT3, leaves 7 bytes for the
 * body of the function.  Currently none is larger than 4.
 */
static int fastop(struct x86_emulate_ctxt *ctxt, fastop_t fop);

#define FASTOP_SIZE	16

#define __FOP_FUNC(name) \
	".align " __stringify(FASTOP_SIZE) " \n\t" \
	".type " name ", @function \n\t" \
	name ":\n\t" \
	ASM_ENDBR \
	IBT_NOSEAL(name)

#define FOP_FUNC(name) \
	__FOP_FUNC(#name)

#define __FOP_RET(name) \
	"11: " ASM_RET \
	".size " name ", .-" name "\n\t"

#define FOP_RET(name) \
	__FOP_RET(#name)

#define __FOP_START(op, align) \
	extern void em_##op(struct fastop *fake); \
	asm(".pushsection .text, \"ax\" \n\t" \
	    ".global em_" #op " \n\t" \
	    ".align " __stringify(align) " \n\t" \
	    "em_" #op ":\n\t"

#define FOP_START(op) __FOP_START(op, FASTOP_SIZE)

#define FOP_END \
	    ".popsection")

#define __FOPNOP(name) \
	__FOP_FUNC(name) \
	__FOP_RET(name)

#define FOPNOP() \
	__FOPNOP(__stringify(__UNIQUE_ID(nop)))

#define FOP1E(op,  dst) \
	__FOP_FUNC(#op "_" #dst) \
	"10: " #op " %" #dst " \n\t" \
	__FOP_RET(#op "_" #dst)

#define FOP1EEX(op,  dst) \
	FOP1E(op, dst) _ASM_EXTABLE_TYPE_REG(10b, 11b, EX_TYPE_ZERO_REG, %%esi)

#define FASTOP1(op) \
	FOP_START(op) \
	FOP1E(op##b, al) \
	FOP1E(op##w, ax) \
	FOP1E(op##l, eax) \
	ON64(FOP1E(op##q, rax))	\
	FOP_END

/* 1-operand, using src2 (for MUL/DIV r/m) */
#define FASTOP1SRC2(op, name) \
	FOP_START(name) \
	FOP1E(op, cl) \
	FOP1E(op, cx) \
	FOP1E(op, ecx) \
	ON64(FOP1E(op, rcx)) \
	FOP_END

/* 1-operand, using src2 (for MUL/DIV r/m), with exceptions */
#define FASTOP1SRC2EX(op, name) \
	FOP_START(name) \
	FOP1EEX(op, cl) \
	FOP1EEX(op, cx) \
	FOP1EEX(op, ecx) \
	ON64(FOP1EEX(op, rcx)) \
	FOP_END

#define FOP2E(op,  dst, src)	   \
	__FOP_FUNC(#op "_" #dst "_" #src) \
	#op " %" #src ", %" #dst " \n\t" \
	__FOP_RET(#op "_" #dst "_" #src)

#define FASTOP2(op) \
	FOP_START(op) \
	FOP2E(op##b, al, dl) \
	FOP2E(op##w, ax, dx) \
	FOP2E(op##l, eax, edx) \
	ON64(FOP2E(op##q, rax, rdx)) \
	FOP_END

/* 2 operand, word only */
#define FASTOP2W(op) \
	FOP_START(op) \
	FOPNOP() \
	FOP2E(op##w, ax, dx) \
	FOP2E(op##l, eax, edx) \
	ON64(FOP2E(op##q, rax, rdx)) \
	FOP_END

/* 2 operand, src is CL */
#define FASTOP2CL(op) \
	FOP_START(op) \
	FOP2E(op##b, al, cl) \
	FOP2E(op##w, ax, cl) \
	FOP2E(op##l, eax, cl) \
	ON64(FOP2E(op##q, rax, cl)) \
	FOP_END

/* 2 operand, src and dest are reversed */
#define FASTOP2R(op, name) \
	FOP_START(name) \
	FOP2E(op##b, dl, al) \
	FOP2E(op##w, dx, ax) \
	FOP2E(op##l, edx, eax) \
	ON64(FOP2E(op##q, rdx, rax)) \
	FOP_END

#define FOP3E(op,  dst, src, src2) \
	__FOP_FUNC(#op "_" #dst "_" #src "_" #src2) \
	#op " %" #src2 ", %" #src ", %" #dst " \n\t"\
	__FOP_RET(#op "_" #dst "_" #src "_" #src2)

/* 3-operand, word-only, src2=cl */
#define FASTOP3WCL(op) \
	FOP_START(op) \
	FOPNOP() \
	FOP3E(op##w, ax, dx, cl) \
	FOP3E(op##l, eax, edx, cl) \
	ON64(FOP3E(op##q, rax, rdx, cl)) \
	FOP_END

/* Special case for SETcc - 1 instruction per cc */
#define FOP_SETCC(op) \
	FOP_FUNC(op) \
	#op " %al \n\t" \
	FOP_RET(op)

FOP_START(setcc)
FOP_SETCC(seto)
FOP_SETCC(setno)
FOP_SETCC(setc)
FOP_SETCC(setnc)
FOP_SETCC(setz)
FOP_SETCC(setnz)
FOP_SETCC(setbe)
FOP_SETCC(setnbe)
FOP_SETCC(sets)
FOP_SETCC(setns)
FOP_SETCC(setp)
FOP_SETCC(setnp)
FOP_SETCC(setl)
FOP_SETCC(setnl)
FOP_SETCC(setle)
FOP_SETCC(setnle)
FOP_END;

FOP_START(salc)
FOP_FUNC(salc)
"pushf; sbb %al, %al; popf \n\t"
FOP_RET(salc)
FOP_END;

/*
 * XXX: inoutclob user must know where the argument is being expanded.
 *      Using asm goto would allow us to remove _fault.
 */
#define asm_safe(insn, inoutclob...) \
({ \
	int _fault = 0; \
 \
	asm volatile("1:" insn "\n" \
	             "2:\n" \
		     _ASM_EXTABLE_TYPE_REG(1b, 2b, EX_TYPE_ONE_REG, %[_fault]) \
	             : [_fault] "+r"(_fault) inoutclob ); \
 \
	_fault ? X86EMUL_UNHANDLEABLE : X86EMUL_CONTINUE; \
})
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

static int emulator_check_intercept(struct x86_emulate_ctxt *ctxt,
				    enum x86_intercept intercept,
				    enum x86_intercept_stage stage)
{
	struct x86_instruction_info info = {
		.intercept  = intercept,
		.rep_prefix = ctxt->rep_prefix,
		.modrm_mod  = ctxt->modrm_mod,
		.modrm_reg  = ctxt->modrm_reg,
		.modrm_rm   = ctxt->modrm_rm,
		.src_val    = ctxt->src.val64,
<<<<<<< HEAD
=======
		.dst_val    = ctxt->dst.val64,
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		.src_bytes  = ctxt->src.bytes,
		.dst_bytes  = ctxt->dst.bytes,
		.ad_bytes   = ctxt->ad_bytes,
		.next_rip   = ctxt->eip,
	};

	return ctxt->ops->intercept(ctxt, &info, stage);
}

<<<<<<< HEAD
=======
static void assign_masked(ulong *dest, ulong src, ulong mask)
{
	*dest = (*dest & ~mask) | (src & mask);
}

static void assign_register(unsigned long *reg, u64 val, int bytes)
{
	/* The 4-byte case *is* correct: in 64-bit mode we zero-extend. */
	switch (bytes) {
	case 1:
		*(u8 *)reg = (u8)val;
		break;
	case 2:
		*(u16 *)reg = (u16)val;
		break;
	case 4:
		*reg = (u32)val;
		break;	/* 64b: zero-extend */
	case 8:
		*reg = val;
		break;
	}
}

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
static inline unsigned long ad_mask(struct x86_emulate_ctxt *ctxt)
{
	return (1UL << (ctxt->ad_bytes << 3)) - 1;
}

<<<<<<< HEAD
=======
static ulong stack_mask(struct x86_emulate_ctxt *ctxt)
{
	u16 sel;
	struct desc_struct ss;

	if (ctxt->mode == X86EMUL_MODE_PROT64)
		return ~0UL;
	ctxt->ops->get_segment(ctxt, &sel, &ss, NULL, VCPU_SREG_SS);
	return ~0U >> ((ss.d ^ 1) * 16);  /* d=0: 0xffff; d=1: 0xffffffff */
}

static int stack_size(struct x86_emulate_ctxt *ctxt)
{
	return (__fls(stack_mask(ctxt)) + 1) >> 3;
}

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
/* Access/update address held in a register, based on addressing mode. */
static inline unsigned long
address_mask(struct x86_emulate_ctxt *ctxt, unsigned long reg)
{
	if (ctxt->ad_bytes == sizeof(unsigned long))
		return reg;
	else
		return reg & ad_mask(ctxt);
}

static inline unsigned long
<<<<<<< HEAD
register_address(struct x86_emulate_ctxt *ctxt, unsigned long reg)
{
	return address_mask(ctxt, reg);
}

static inline void
register_address_increment(struct x86_emulate_ctxt *ctxt, unsigned long *reg, int inc)
{
	if (ctxt->ad_bytes == sizeof(unsigned long))
		*reg += inc;
	else
		*reg = (*reg & ~ad_mask(ctxt)) | ((*reg + inc) & ad_mask(ctxt));
=======
register_address(struct x86_emulate_ctxt *ctxt, int reg)
{
	return address_mask(ctxt, reg_read(ctxt, reg));
}

static void masked_increment(ulong *reg, ulong mask, int inc)
{
	assign_masked(reg, *reg + inc, mask);
}

static inline void
register_address_increment(struct x86_emulate_ctxt *ctxt, int reg, int inc)
{
	ulong *preg = reg_rmw(ctxt, reg);

	assign_register(preg, *preg + inc, ctxt->ad_bytes);
}

static void rsp_increment(struct x86_emulate_ctxt *ctxt, int inc)
{
	masked_increment(reg_rmw(ctxt, VCPU_REGS_RSP), stack_mask(ctxt), inc);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

static u32 desc_limit_scaled(struct desc_struct *desc)
{
	u32 limit = get_desc_limit(desc);

	return desc->g ? (limit << 12) | 0xfff : limit;
}

<<<<<<< HEAD
static void set_seg_override(struct x86_emulate_ctxt *ctxt, int seg)
{
	ctxt->has_seg_override = true;
	ctxt->seg_override = seg;
}

=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
static unsigned long seg_base(struct x86_emulate_ctxt *ctxt, int seg)
{
	if (ctxt->mode == X86EMUL_MODE_PROT64 && seg < VCPU_SREG_FS)
		return 0;

	return ctxt->ops->get_cached_segment_base(ctxt, seg);
}

<<<<<<< HEAD
static unsigned seg_override(struct x86_emulate_ctxt *ctxt)
{
	if (!ctxt->has_seg_override)
		return 0;

	return ctxt->seg_override;
}

static int emulate_exception(struct x86_emulate_ctxt *ctxt, int vec,
			     u32 error, bool valid)
{
=======
static int emulate_exception(struct x86_emulate_ctxt *ctxt, int vec,
			     u32 error, bool valid)
{
	if (KVM_EMULATOR_BUG_ON(vec > 0x1f, ctxt))
		return X86EMUL_UNHANDLEABLE;

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	ctxt->exception.vector = vec;
	ctxt->exception.error_code = error;
	ctxt->exception.error_code_valid = valid;
	return X86EMUL_PROPAGATE_FAULT;
}

static int emulate_db(struct x86_emulate_ctxt *ctxt)
{
	return emulate_exception(ctxt, DB_VECTOR, 0, false);
}

static int emulate_gp(struct x86_emulate_ctxt *ctxt, int err)
{
	return emulate_exception(ctxt, GP_VECTOR, err, true);
}

static int emulate_ss(struct x86_emulate_ctxt *ctxt, int err)
{
	return emulate_exception(ctxt, SS_VECTOR, err, true);
}

static int emulate_ud(struct x86_emulate_ctxt *ctxt)
{
	return emulate_exception(ctxt, UD_VECTOR, 0, false);
}

static int emulate_ts(struct x86_emulate_ctxt *ctxt, int err)
{
	return emulate_exception(ctxt, TS_VECTOR, err, true);
}

static int emulate_de(struct x86_emulate_ctxt *ctxt)
{
	return emulate_exception(ctxt, DE_VECTOR, 0, false);
}

static int emulate_nm(struct x86_emulate_ctxt *ctxt)
{
	return emulate_exception(ctxt, NM_VECTOR, 0, false);
}

<<<<<<< HEAD
static inline int assign_eip_far(struct x86_emulate_ctxt *ctxt, ulong dst,
			       int cs_l)
{
	switch (ctxt->op_bytes) {
	case 2:
		ctxt->_eip = (u16)dst;
		break;
	case 4:
		ctxt->_eip = (u32)dst;
		break;
#ifdef CONFIG_X86_64
	case 8:
		if ((cs_l && is_noncanonical_address(dst)) ||
		    (!cs_l && (dst >> 32) != 0))
			return emulate_gp(ctxt, 0);
		ctxt->_eip = dst;
		break;
#endif
	default:
		WARN(1, "unsupported eip assignment size\n");
	}
	return X86EMUL_CONTINUE;
}

static inline int assign_eip_near(struct x86_emulate_ctxt *ctxt, ulong dst)
{
	return assign_eip_far(ctxt, dst, ctxt->mode == X86EMUL_MODE_PROT64);
}

static inline int jmp_rel(struct x86_emulate_ctxt *ctxt, int rel)
{
	return assign_eip_near(ctxt, ctxt->_eip + rel);
}

=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
static u16 get_segment_selector(struct x86_emulate_ctxt *ctxt, unsigned seg)
{
	u16 selector;
	struct desc_struct desc;

	ctxt->ops->get_segment(ctxt, &selector, &desc, NULL, seg);
	return selector;
}

static void set_segment_selector(struct x86_emulate_ctxt *ctxt, u16 selector,
				 unsigned seg)
{
	u16 dummy;
	u32 base3;
	struct desc_struct desc;

	ctxt->ops->get_segment(ctxt, &dummy, &desc, &base3, seg);
	ctxt->ops->set_segment(ctxt, selector, &desc, base3, seg);
}

<<<<<<< HEAD
static int __linearize(struct x86_emulate_ctxt *ctxt,
		     struct segmented_address addr,
		     unsigned size, bool write, bool fetch,
		     ulong *linear)
=======
static inline u8 ctxt_virt_addr_bits(struct x86_emulate_ctxt *ctxt)
{
	return (ctxt->ops->get_cr(ctxt, 4) & X86_CR4_LA57) ? 57 : 48;
}

static inline bool emul_is_noncanonical_address(u64 la,
						struct x86_emulate_ctxt *ctxt)
{
	return !__is_canonical_address(la, ctxt_virt_addr_bits(ctxt));
}

/*
 * x86 defines three classes of vector instructions: explicitly
 * aligned, explicitly unaligned, and the rest, which change behaviour
 * depending on whether they're AVX encoded or not.
 *
 * Also included is CMPXCHG16B which is not a vector instruction, yet it is
 * subject to the same check.  FXSAVE and FXRSTOR are checked here too as their
 * 512 bytes of data must be aligned to a 16 byte boundary.
 */
static unsigned insn_alignment(struct x86_emulate_ctxt *ctxt, unsigned size)
{
	u64 alignment = ctxt->d & AlignMask;

	if (likely(size < 16))
		return 1;

	switch (alignment) {
	case Unaligned:
	case Avx:
		return 1;
	case Aligned16:
		return 16;
	case Aligned:
	default:
		return size;
	}
}

static __always_inline int __linearize(struct x86_emulate_ctxt *ctxt,
				       struct segmented_address addr,
				       unsigned *max_size, unsigned size,
				       enum x86emul_mode mode, ulong *linear,
				       unsigned int flags)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
{
	struct desc_struct desc;
	bool usable;
	ulong la;
	u32 lim;
	u16 sel;
<<<<<<< HEAD
	unsigned cpl, rpl;

	la = seg_base(ctxt, addr.seg) + addr.ea;
	switch (ctxt->mode) {
	case X86EMUL_MODE_REAL:
		break;
	case X86EMUL_MODE_PROT64:
		if (((signed long)la << 16) >> 16 != la)
			return emulate_gp(ctxt, 0);
		break;
	default:
=======
	u8  va_bits;

	la = seg_base(ctxt, addr.seg) + addr.ea;
	*max_size = 0;
	switch (mode) {
	case X86EMUL_MODE_PROT64:
		*linear = la = ctxt->ops->get_untagged_addr(ctxt, la, flags);
		va_bits = ctxt_virt_addr_bits(ctxt);
		if (!__is_canonical_address(la, va_bits))
			goto bad;

		*max_size = min_t(u64, ~0u, (1ull << va_bits) - la);
		if (size > *max_size)
			goto bad;
		break;
	default:
		*linear = la = (u32)la;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		usable = ctxt->ops->get_segment(ctxt, &sel, &desc, NULL,
						addr.seg);
		if (!usable)
			goto bad;
<<<<<<< HEAD
		/* code segment or read-only data segment */
		if (((desc.type & 8) || !(desc.type & 2)) && write)
			goto bad;
		/* unreadable code segment */
		if (!fetch && (desc.type & 8) && !(desc.type & 2))
			goto bad;
		lim = desc_limit_scaled(&desc);
		if ((desc.type & 8) || !(desc.type & 4)) {
			/* expand-up segment */
			if (addr.ea > lim || (u32)(addr.ea + size - 1) > lim)
				goto bad;
		} else {
			/* exapand-down segment */
			if (addr.ea <= lim || (u32)(addr.ea + size - 1) <= lim)
				goto bad;
			lim = desc.d ? 0xffffffff : 0xffff;
			if (addr.ea > lim || (u32)(addr.ea + size - 1) > lim)
				goto bad;
		}
		cpl = ctxt->ops->cpl(ctxt);
		rpl = sel & 3;
		cpl = max(cpl, rpl);
		if (!(desc.type & 8)) {
			/* data segment */
			if (cpl > desc.dpl)
				goto bad;
		} else if ((desc.type & 8) && !(desc.type & 4)) {
			/* nonconforming code segment */
			if (cpl != desc.dpl)
				goto bad;
		} else if ((desc.type & 8) && (desc.type & 4)) {
			/* conforming code segment */
			if (cpl < desc.dpl)
=======
		/* code segment in protected mode or read-only data segment */
		if ((((ctxt->mode != X86EMUL_MODE_REAL) && (desc.type & 8)) || !(desc.type & 2)) &&
		    (flags & X86EMUL_F_WRITE))
			goto bad;
		/* unreadable code segment */
		if (!(flags & X86EMUL_F_FETCH) && (desc.type & 8) && !(desc.type & 2))
			goto bad;
		lim = desc_limit_scaled(&desc);
		if (!(desc.type & 8) && (desc.type & 4)) {
			/* expand-down segment */
			if (addr.ea <= lim)
				goto bad;
			lim = desc.d ? 0xffffffff : 0xffff;
		}
		if (addr.ea > lim)
			goto bad;
		if (lim == 0xffffffff)
			*max_size = ~0u;
		else {
			*max_size = (u64)lim + 1 - addr.ea;
			if (size > *max_size)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
				goto bad;
		}
		break;
	}
<<<<<<< HEAD
	if (fetch ? ctxt->mode != X86EMUL_MODE_PROT64 : ctxt->ad_bytes != 8)
		la &= (u32)-1;
	*linear = la;
	return X86EMUL_CONTINUE;
bad:
	if (addr.seg == VCPU_SREG_SS)
		return emulate_ss(ctxt, addr.seg);
	else
		return emulate_gp(ctxt, addr.seg);
=======
	if (la & (insn_alignment(ctxt, size) - 1))
		return emulate_gp(ctxt, 0);
	return X86EMUL_CONTINUE;
bad:
	if (addr.seg == VCPU_SREG_SS)
		return emulate_ss(ctxt, 0);
	else
		return emulate_gp(ctxt, 0);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

static int linearize(struct x86_emulate_ctxt *ctxt,
		     struct segmented_address addr,
		     unsigned size, bool write,
		     ulong *linear)
{
<<<<<<< HEAD
	return __linearize(ctxt, addr, size, write, false, linear);
}

=======
	unsigned max_size;
	return __linearize(ctxt, addr, &max_size, size, ctxt->mode, linear,
			   write ? X86EMUL_F_WRITE : 0);
}

static inline int assign_eip(struct x86_emulate_ctxt *ctxt, ulong dst)
{
	ulong linear;
	int rc;
	unsigned max_size;
	struct segmented_address addr = { .seg = VCPU_SREG_CS,
					   .ea = dst };

	if (ctxt->op_bytes != sizeof(unsigned long))
		addr.ea = dst & ((1UL << (ctxt->op_bytes << 3)) - 1);
	rc = __linearize(ctxt, addr, &max_size, 1, ctxt->mode, &linear,
			 X86EMUL_F_FETCH);
	if (rc == X86EMUL_CONTINUE)
		ctxt->_eip = addr.ea;
	return rc;
}

static inline int emulator_recalc_and_set_mode(struct x86_emulate_ctxt *ctxt)
{
	u64 efer;
	struct desc_struct cs;
	u16 selector;
	u32 base3;

	ctxt->ops->get_msr(ctxt, MSR_EFER, &efer);

	if (!(ctxt->ops->get_cr(ctxt, 0) & X86_CR0_PE)) {
		/* Real mode. cpu must not have long mode active */
		if (efer & EFER_LMA)
			return X86EMUL_UNHANDLEABLE;
		ctxt->mode = X86EMUL_MODE_REAL;
		return X86EMUL_CONTINUE;
	}

	if (ctxt->eflags & X86_EFLAGS_VM) {
		/* Protected/VM86 mode. cpu must not have long mode active */
		if (efer & EFER_LMA)
			return X86EMUL_UNHANDLEABLE;
		ctxt->mode = X86EMUL_MODE_VM86;
		return X86EMUL_CONTINUE;
	}

	if (!ctxt->ops->get_segment(ctxt, &selector, &cs, &base3, VCPU_SREG_CS))
		return X86EMUL_UNHANDLEABLE;

	if (efer & EFER_LMA) {
		if (cs.l) {
			/* Proper long mode */
			ctxt->mode = X86EMUL_MODE_PROT64;
		} else if (cs.d) {
			/* 32 bit compatibility mode*/
			ctxt->mode = X86EMUL_MODE_PROT32;
		} else {
			ctxt->mode = X86EMUL_MODE_PROT16;
		}
	} else {
		/* Legacy 32 bit / 16 bit mode */
		ctxt->mode = cs.d ? X86EMUL_MODE_PROT32 : X86EMUL_MODE_PROT16;
	}

	return X86EMUL_CONTINUE;
}

static inline int assign_eip_near(struct x86_emulate_ctxt *ctxt, ulong dst)
{
	return assign_eip(ctxt, dst);
}

static int assign_eip_far(struct x86_emulate_ctxt *ctxt, ulong dst)
{
	int rc = emulator_recalc_and_set_mode(ctxt);

	if (rc != X86EMUL_CONTINUE)
		return rc;

	return assign_eip(ctxt, dst);
}

static inline int jmp_rel(struct x86_emulate_ctxt *ctxt, int rel)
{
	return assign_eip_near(ctxt, ctxt->_eip + rel);
}

static int linear_read_system(struct x86_emulate_ctxt *ctxt, ulong linear,
			      void *data, unsigned size)
{
	return ctxt->ops->read_std(ctxt, linear, data, size, &ctxt->exception, true);
}

static int linear_write_system(struct x86_emulate_ctxt *ctxt,
			       ulong linear, void *data,
			       unsigned int size)
{
	return ctxt->ops->write_std(ctxt, linear, data, size, &ctxt->exception, true);
}
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

static int segmented_read_std(struct x86_emulate_ctxt *ctxt,
			      struct segmented_address addr,
			      void *data,
			      unsigned size)
{
	int rc;
	ulong linear;

	rc = linearize(ctxt, addr, size, false, &linear);
	if (rc != X86EMUL_CONTINUE)
		return rc;
<<<<<<< HEAD
	return ctxt->ops->read_std(ctxt, linear, data, size, &ctxt->exception);
}

/*
 * Fetch the next byte of the instruction being emulated which is pointed to
 * by ctxt->_eip, then increment ctxt->_eip.
 *
 * Also prefetch the remaining bytes of the instruction without crossing page
 * boundary if they are not in fetch_cache yet.
 */
static int do_insn_fetch_byte(struct x86_emulate_ctxt *ctxt, u8 *dest)
{
	struct fetch_cache *fc = &ctxt->fetch;
	int rc;
	int size, cur_size;

	if (ctxt->_eip == fc->end) {
		unsigned long linear;
		struct segmented_address addr = { .seg = VCPU_SREG_CS,
						  .ea  = ctxt->_eip };
		cur_size = fc->end - fc->start;
		size = min(15UL - cur_size,
			   PAGE_SIZE - offset_in_page(ctxt->_eip));
		rc = __linearize(ctxt, addr, size, false, true, &linear);
		if (unlikely(rc != X86EMUL_CONTINUE))
			return rc;
		rc = ctxt->ops->fetch(ctxt, linear, fc->data + cur_size,
				      size, &ctxt->exception);
		if (unlikely(rc != X86EMUL_CONTINUE))
			return rc;
		fc->end += size;
	}
	*dest = fc->data[ctxt->_eip - fc->start];
	ctxt->_eip++;
	return X86EMUL_CONTINUE;
}

static int do_insn_fetch(struct x86_emulate_ctxt *ctxt,
			 void *dest, unsigned size)
{
	int rc;

	/* x86 instructions are limited to 15 bytes. */
	if (unlikely(ctxt->_eip + size - ctxt->eip > 15))
		return X86EMUL_UNHANDLEABLE;
	while (size--) {
		rc = do_insn_fetch_byte(ctxt, dest++);
		if (rc != X86EMUL_CONTINUE)
			return rc;
	}
	return X86EMUL_CONTINUE;
=======
	return ctxt->ops->read_std(ctxt, linear, data, size, &ctxt->exception, false);
}

static int segmented_write_std(struct x86_emulate_ctxt *ctxt,
			       struct segmented_address addr,
			       void *data,
			       unsigned int size)
{
	int rc;
	ulong linear;

	rc = linearize(ctxt, addr, size, true, &linear);
	if (rc != X86EMUL_CONTINUE)
		return rc;
	return ctxt->ops->write_std(ctxt, linear, data, size, &ctxt->exception, false);
}

/*
 * Prefetch the remaining bytes of the instruction without crossing page
 * boundary if they are not in fetch_cache yet.
 */
static int __do_insn_fetch_bytes(struct x86_emulate_ctxt *ctxt, int op_size)
{
	int rc;
	unsigned size, max_size;
	unsigned long linear;
	int cur_size = ctxt->fetch.end - ctxt->fetch.data;
	struct segmented_address addr = { .seg = VCPU_SREG_CS,
					   .ea = ctxt->eip + cur_size };

	/*
	 * We do not know exactly how many bytes will be needed, and
	 * __linearize is expensive, so fetch as much as possible.  We
	 * just have to avoid going beyond the 15 byte limit, the end
	 * of the segment, or the end of the page.
	 *
	 * __linearize is called with size 0 so that it does not do any
	 * boundary check itself.  Instead, we use max_size to check
	 * against op_size.
	 */
	rc = __linearize(ctxt, addr, &max_size, 0, ctxt->mode, &linear,
			 X86EMUL_F_FETCH);
	if (unlikely(rc != X86EMUL_CONTINUE))
		return rc;

	size = min_t(unsigned, 15UL ^ cur_size, max_size);
	size = min_t(unsigned, size, PAGE_SIZE - offset_in_page(linear));

	/*
	 * One instruction can only straddle two pages,
	 * and one has been loaded at the beginning of
	 * x86_decode_insn.  So, if not enough bytes
	 * still, we must have hit the 15-byte boundary.
	 */
	if (unlikely(size < op_size))
		return emulate_gp(ctxt, 0);

	rc = ctxt->ops->fetch(ctxt, linear, ctxt->fetch.end,
			      size, &ctxt->exception);
	if (unlikely(rc != X86EMUL_CONTINUE))
		return rc;
	ctxt->fetch.end += size;
	return X86EMUL_CONTINUE;
}

static __always_inline int do_insn_fetch_bytes(struct x86_emulate_ctxt *ctxt,
					       unsigned size)
{
	unsigned done_size = ctxt->fetch.end - ctxt->fetch.ptr;

	if (unlikely(done_size < size))
		return __do_insn_fetch_bytes(ctxt, size - done_size);
	else
		return X86EMUL_CONTINUE;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

/* Fetch next part of the instruction being emulated. */
#define insn_fetch(_type, _ctxt)					\
<<<<<<< HEAD
({	unsigned long _x;						\
	rc = do_insn_fetch(_ctxt, &_x, sizeof(_type));			\
	if (rc != X86EMUL_CONTINUE)					\
		goto done;						\
	(_type)_x;							\
})

#define insn_fetch_arr(_arr, _size, _ctxt)				\
({	rc = do_insn_fetch(_ctxt, _arr, (_size));			\
	if (rc != X86EMUL_CONTINUE)					\
		goto done;						\
=======
({	_type _x;							\
									\
	rc = do_insn_fetch_bytes(_ctxt, sizeof(_type));			\
	if (rc != X86EMUL_CONTINUE)					\
		goto done;						\
	ctxt->_eip += sizeof(_type);					\
	memcpy(&_x, ctxt->fetch.ptr, sizeof(_type));			\
	ctxt->fetch.ptr += sizeof(_type);				\
	_x;								\
})

#define insn_fetch_arr(_arr, _size, _ctxt)				\
({									\
	rc = do_insn_fetch_bytes(_ctxt, _size);				\
	if (rc != X86EMUL_CONTINUE)					\
		goto done;						\
	ctxt->_eip += (_size);						\
	memcpy(_arr, ctxt->fetch.ptr, _size);				\
	ctxt->fetch.ptr += (_size);					\
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
})

/*
 * Given the 'reg' portion of a ModRM byte, and a register block, return a
 * pointer into the block that addresses the relevant register.
 * @highbyte_regs specifies whether to decode AH,CH,DH,BH.
 */
<<<<<<< HEAD
static void *decode_register(u8 modrm_reg, unsigned long *regs,
			     int highbyte_regs)
{
	void *p;

	p = &regs[modrm_reg];
	if (highbyte_regs && modrm_reg >= 4 && modrm_reg < 8)
		p = (unsigned char *)&regs[modrm_reg & 3] + 1;
=======
static void *decode_register(struct x86_emulate_ctxt *ctxt, u8 modrm_reg,
			     int byteop)
{
	void *p;
	int highbyte_regs = (ctxt->rex_prefix == 0) && byteop;

	if (highbyte_regs && modrm_reg >= 4 && modrm_reg < 8)
		p = (unsigned char *)reg_rmw(ctxt, modrm_reg & 3) + 1;
	else
		p = reg_rmw(ctxt, modrm_reg);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	return p;
}

static int read_descriptor(struct x86_emulate_ctxt *ctxt,
			   struct segmented_address addr,
			   u16 *size, unsigned long *address, int op_bytes)
{
	int rc;

	if (op_bytes == 2)
		op_bytes = 3;
	*address = 0;
	rc = segmented_read_std(ctxt, addr, size, 2);
	if (rc != X86EMUL_CONTINUE)
		return rc;
	addr.ea += 2;
	rc = segmented_read_std(ctxt, addr, address, op_bytes);
	return rc;
}

<<<<<<< HEAD
static int test_cc(unsigned int condition, unsigned int flags)
{
	int rc = 0;

	switch ((condition & 15) >> 1) {
	case 0: /* o */
		rc |= (flags & EFLG_OF);
		break;
	case 1: /* b/c/nae */
		rc |= (flags & EFLG_CF);
		break;
	case 2: /* z/e */
		rc |= (flags & EFLG_ZF);
		break;
	case 3: /* be/na */
		rc |= (flags & (EFLG_CF|EFLG_ZF));
		break;
	case 4: /* s */
		rc |= (flags & EFLG_SF);
		break;
	case 5: /* p/pe */
		rc |= (flags & EFLG_PF);
		break;
	case 7: /* le/ng */
		rc |= (flags & EFLG_ZF);
		/* fall through */
	case 6: /* l/nge */
		rc |= (!(flags & EFLG_SF) != !(flags & EFLG_OF));
		break;
	}

	/* Odd condition identifiers (lsb == 1) have inverted sense. */
	return (!!rc ^ (condition & 1));
=======
FASTOP2(add);
FASTOP2(or);
FASTOP2(adc);
FASTOP2(sbb);
FASTOP2(and);
FASTOP2(sub);
FASTOP2(xor);
FASTOP2(cmp);
FASTOP2(test);

FASTOP1SRC2(mul, mul_ex);
FASTOP1SRC2(imul, imul_ex);
FASTOP1SRC2EX(div, div_ex);
FASTOP1SRC2EX(idiv, idiv_ex);

FASTOP3WCL(shld);
FASTOP3WCL(shrd);

FASTOP2W(imul);

FASTOP1(not);
FASTOP1(neg);
FASTOP1(inc);
FASTOP1(dec);

FASTOP2CL(rol);
FASTOP2CL(ror);
FASTOP2CL(rcl);
FASTOP2CL(rcr);
FASTOP2CL(shl);
FASTOP2CL(shr);
FASTOP2CL(sar);

FASTOP2W(bsf);
FASTOP2W(bsr);
FASTOP2W(bt);
FASTOP2W(bts);
FASTOP2W(btr);
FASTOP2W(btc);

FASTOP2(xadd);

FASTOP2R(cmp, cmp_r);

static int em_bsf_c(struct x86_emulate_ctxt *ctxt)
{
	/* If src is zero, do not writeback, but update flags */
	if (ctxt->src.val == 0)
		ctxt->dst.type = OP_NONE;
	return fastop(ctxt, em_bsf);
}

static int em_bsr_c(struct x86_emulate_ctxt *ctxt)
{
	/* If src is zero, do not writeback, but update flags */
	if (ctxt->src.val == 0)
		ctxt->dst.type = OP_NONE;
	return fastop(ctxt, em_bsr);
}

static __always_inline u8 test_cc(unsigned int condition, unsigned long flags)
{
	u8 rc;
	void (*fop)(void) = (void *)em_setcc + FASTOP_SIZE * (condition & 0xf);

	flags = (flags & EFLAGS_MASK) | X86_EFLAGS_IF;
	asm("push %[flags]; popf; " CALL_NOSPEC
	    : "=a"(rc) : [thunk_target]"r"(fop), [flags]"r"(flags));
	return rc;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

static void fetch_register_operand(struct operand *op)
{
	switch (op->bytes) {
	case 1:
		op->val = *(u8 *)op->addr.reg;
		break;
	case 2:
		op->val = *(u16 *)op->addr.reg;
		break;
	case 4:
		op->val = *(u32 *)op->addr.reg;
		break;
	case 8:
		op->val = *(u64 *)op->addr.reg;
		break;
	}
}

<<<<<<< HEAD
static void read_sse_reg(struct x86_emulate_ctxt *ctxt, sse128_t *data, int reg)
{
	ctxt->ops->get_fpu(ctxt);
	switch (reg) {
	case 0: asm("movdqu %%xmm0, %0" : "=m"(*data)); break;
	case 1: asm("movdqu %%xmm1, %0" : "=m"(*data)); break;
	case 2: asm("movdqu %%xmm2, %0" : "=m"(*data)); break;
	case 3: asm("movdqu %%xmm3, %0" : "=m"(*data)); break;
	case 4: asm("movdqu %%xmm4, %0" : "=m"(*data)); break;
	case 5: asm("movdqu %%xmm5, %0" : "=m"(*data)); break;
	case 6: asm("movdqu %%xmm6, %0" : "=m"(*data)); break;
	case 7: asm("movdqu %%xmm7, %0" : "=m"(*data)); break;
#ifdef CONFIG_X86_64
	case 8: asm("movdqu %%xmm8, %0" : "=m"(*data)); break;
	case 9: asm("movdqu %%xmm9, %0" : "=m"(*data)); break;
	case 10: asm("movdqu %%xmm10, %0" : "=m"(*data)); break;
	case 11: asm("movdqu %%xmm11, %0" : "=m"(*data)); break;
	case 12: asm("movdqu %%xmm12, %0" : "=m"(*data)); break;
	case 13: asm("movdqu %%xmm13, %0" : "=m"(*data)); break;
	case 14: asm("movdqu %%xmm14, %0" : "=m"(*data)); break;
	case 15: asm("movdqu %%xmm15, %0" : "=m"(*data)); break;
#endif
	default: BUG();
	}
	ctxt->ops->put_fpu(ctxt);
}

static void write_sse_reg(struct x86_emulate_ctxt *ctxt, sse128_t *data,
			  int reg)
{
	ctxt->ops->get_fpu(ctxt);
	switch (reg) {
	case 0: asm("movdqu %0, %%xmm0" : : "m"(*data)); break;
	case 1: asm("movdqu %0, %%xmm1" : : "m"(*data)); break;
	case 2: asm("movdqu %0, %%xmm2" : : "m"(*data)); break;
	case 3: asm("movdqu %0, %%xmm3" : : "m"(*data)); break;
	case 4: asm("movdqu %0, %%xmm4" : : "m"(*data)); break;
	case 5: asm("movdqu %0, %%xmm5" : : "m"(*data)); break;
	case 6: asm("movdqu %0, %%xmm6" : : "m"(*data)); break;
	case 7: asm("movdqu %0, %%xmm7" : : "m"(*data)); break;
#ifdef CONFIG_X86_64
	case 8: asm("movdqu %0, %%xmm8" : : "m"(*data)); break;
	case 9: asm("movdqu %0, %%xmm9" : : "m"(*data)); break;
	case 10: asm("movdqu %0, %%xmm10" : : "m"(*data)); break;
	case 11: asm("movdqu %0, %%xmm11" : : "m"(*data)); break;
	case 12: asm("movdqu %0, %%xmm12" : : "m"(*data)); break;
	case 13: asm("movdqu %0, %%xmm13" : : "m"(*data)); break;
	case 14: asm("movdqu %0, %%xmm14" : : "m"(*data)); break;
	case 15: asm("movdqu %0, %%xmm15" : : "m"(*data)); break;
#endif
	default: BUG();
	}
	ctxt->ops->put_fpu(ctxt);
=======
static int em_fninit(struct x86_emulate_ctxt *ctxt)
{
	if (ctxt->ops->get_cr(ctxt, 0) & (X86_CR0_TS | X86_CR0_EM))
		return emulate_nm(ctxt);

	kvm_fpu_get();
	asm volatile("fninit");
	kvm_fpu_put();
	return X86EMUL_CONTINUE;
}

static int em_fnstcw(struct x86_emulate_ctxt *ctxt)
{
	u16 fcw;

	if (ctxt->ops->get_cr(ctxt, 0) & (X86_CR0_TS | X86_CR0_EM))
		return emulate_nm(ctxt);

	kvm_fpu_get();
	asm volatile("fnstcw %0": "+m"(fcw));
	kvm_fpu_put();

	ctxt->dst.val = fcw;

	return X86EMUL_CONTINUE;
}

static int em_fnstsw(struct x86_emulate_ctxt *ctxt)
{
	u16 fsw;

	if (ctxt->ops->get_cr(ctxt, 0) & (X86_CR0_TS | X86_CR0_EM))
		return emulate_nm(ctxt);

	kvm_fpu_get();
	asm volatile("fnstsw %0": "+m"(fsw));
	kvm_fpu_put();

	ctxt->dst.val = fsw;

	return X86EMUL_CONTINUE;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

static void decode_register_operand(struct x86_emulate_ctxt *ctxt,
				    struct operand *op)
{
<<<<<<< HEAD
	unsigned reg = ctxt->modrm_reg;
	int highbyte_regs = ctxt->rex_prefix == 0;

	if (!(ctxt->d & ModRM))
=======
	unsigned int reg;

	if (ctxt->d & ModRM)
		reg = ctxt->modrm_reg;
	else
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		reg = (ctxt->b & 7) | ((ctxt->rex_prefix & 1) << 3);

	if (ctxt->d & Sse) {
		op->type = OP_XMM;
		op->bytes = 16;
		op->addr.xmm = reg;
<<<<<<< HEAD
		read_sse_reg(ctxt, &op->vec_val, reg);
=======
		kvm_read_sse_reg(reg, &op->vec_val);
		return;
	}
	if (ctxt->d & Mmx) {
		reg &= 7;
		op->type = OP_MM;
		op->bytes = 8;
		op->addr.mm = reg;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		return;
	}

	op->type = OP_REG;
<<<<<<< HEAD
	if (ctxt->d & ByteOp) {
		op->addr.reg = decode_register(reg, ctxt->regs, highbyte_regs);
		op->bytes = 1;
	} else {
		op->addr.reg = decode_register(reg, ctxt->regs, 0);
		op->bytes = ctxt->op_bytes;
	}
=======
	op->bytes = (ctxt->d & ByteOp) ? 1 : ctxt->op_bytes;
	op->addr.reg = decode_register(ctxt, reg, ctxt->d & ByteOp);

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	fetch_register_operand(op);
	op->orig_val = op->val;
}

<<<<<<< HEAD
=======
static void adjust_modrm_seg(struct x86_emulate_ctxt *ctxt, int base_reg)
{
	if (base_reg == VCPU_REGS_RSP || base_reg == VCPU_REGS_RBP)
		ctxt->modrm_seg = VCPU_SREG_SS;
}

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
static int decode_modrm(struct x86_emulate_ctxt *ctxt,
			struct operand *op)
{
	u8 sib;
<<<<<<< HEAD
	int index_reg = 0, base_reg = 0, scale;
	int rc = X86EMUL_CONTINUE;
	ulong modrm_ea = 0;

	if (ctxt->rex_prefix) {
		ctxt->modrm_reg = (ctxt->rex_prefix & 4) << 1;	/* REX.R */
		index_reg = (ctxt->rex_prefix & 2) << 2; /* REX.X */
		ctxt->modrm_rm = base_reg = (ctxt->rex_prefix & 1) << 3; /* REG.B */
	}

	ctxt->modrm = insn_fetch(u8, ctxt);
	ctxt->modrm_mod |= (ctxt->modrm & 0xc0) >> 6;
	ctxt->modrm_reg |= (ctxt->modrm & 0x38) >> 3;
	ctxt->modrm_rm |= (ctxt->modrm & 0x07);
	ctxt->modrm_seg = VCPU_SREG_DS;

	if (ctxt->modrm_mod == 3) {
		op->type = OP_REG;
		op->bytes = (ctxt->d & ByteOp) ? 1 : ctxt->op_bytes;
		op->addr.reg = decode_register(ctxt->modrm_rm,
					       ctxt->regs, ctxt->d & ByteOp);
=======
	int index_reg, base_reg, scale;
	int rc = X86EMUL_CONTINUE;
	ulong modrm_ea = 0;

	ctxt->modrm_reg = ((ctxt->rex_prefix << 1) & 8); /* REX.R */
	index_reg = (ctxt->rex_prefix << 2) & 8; /* REX.X */
	base_reg = (ctxt->rex_prefix << 3) & 8; /* REX.B */

	ctxt->modrm_mod = (ctxt->modrm & 0xc0) >> 6;
	ctxt->modrm_reg |= (ctxt->modrm & 0x38) >> 3;
	ctxt->modrm_rm = base_reg | (ctxt->modrm & 0x07);
	ctxt->modrm_seg = VCPU_SREG_DS;

	if (ctxt->modrm_mod == 3 || (ctxt->d & NoMod)) {
		op->type = OP_REG;
		op->bytes = (ctxt->d & ByteOp) ? 1 : ctxt->op_bytes;
		op->addr.reg = decode_register(ctxt, ctxt->modrm_rm,
				ctxt->d & ByteOp);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		if (ctxt->d & Sse) {
			op->type = OP_XMM;
			op->bytes = 16;
			op->addr.xmm = ctxt->modrm_rm;
<<<<<<< HEAD
			read_sse_reg(ctxt, &op->vec_val, ctxt->modrm_rm);
=======
			kvm_read_sse_reg(ctxt->modrm_rm, &op->vec_val);
			return rc;
		}
		if (ctxt->d & Mmx) {
			op->type = OP_MM;
			op->bytes = 8;
			op->addr.mm = ctxt->modrm_rm & 7;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
			return rc;
		}
		fetch_register_operand(op);
		return rc;
	}

	op->type = OP_MEM;

	if (ctxt->ad_bytes == 2) {
<<<<<<< HEAD
		unsigned bx = ctxt->regs[VCPU_REGS_RBX];
		unsigned bp = ctxt->regs[VCPU_REGS_RBP];
		unsigned si = ctxt->regs[VCPU_REGS_RSI];
		unsigned di = ctxt->regs[VCPU_REGS_RDI];
=======
		unsigned bx = reg_read(ctxt, VCPU_REGS_RBX);
		unsigned bp = reg_read(ctxt, VCPU_REGS_RBP);
		unsigned si = reg_read(ctxt, VCPU_REGS_RSI);
		unsigned di = reg_read(ctxt, VCPU_REGS_RDI);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

		/* 16-bit ModR/M decode. */
		switch (ctxt->modrm_mod) {
		case 0:
			if (ctxt->modrm_rm == 6)
				modrm_ea += insn_fetch(u16, ctxt);
			break;
		case 1:
			modrm_ea += insn_fetch(s8, ctxt);
			break;
		case 2:
			modrm_ea += insn_fetch(u16, ctxt);
			break;
		}
		switch (ctxt->modrm_rm) {
		case 0:
			modrm_ea += bx + si;
			break;
		case 1:
			modrm_ea += bx + di;
			break;
		case 2:
			modrm_ea += bp + si;
			break;
		case 3:
			modrm_ea += bp + di;
			break;
		case 4:
			modrm_ea += si;
			break;
		case 5:
			modrm_ea += di;
			break;
		case 6:
			if (ctxt->modrm_mod != 0)
				modrm_ea += bp;
			break;
		case 7:
			modrm_ea += bx;
			break;
		}
		if (ctxt->modrm_rm == 2 || ctxt->modrm_rm == 3 ||
		    (ctxt->modrm_rm == 6 && ctxt->modrm_mod != 0))
			ctxt->modrm_seg = VCPU_SREG_SS;
		modrm_ea = (u16)modrm_ea;
	} else {
		/* 32/64-bit ModR/M decode. */
		if ((ctxt->modrm_rm & 7) == 4) {
			sib = insn_fetch(u8, ctxt);
			index_reg |= (sib >> 3) & 7;
			base_reg |= sib & 7;
			scale = sib >> 6;

			if ((base_reg & 7) == 5 && ctxt->modrm_mod == 0)
				modrm_ea += insn_fetch(s32, ctxt);
<<<<<<< HEAD
			else
				modrm_ea += ctxt->regs[base_reg];
			if (index_reg != 4)
				modrm_ea += ctxt->regs[index_reg] << scale;
		} else if ((ctxt->modrm_rm & 7) == 5 && ctxt->modrm_mod == 0) {
			if (ctxt->mode == X86EMUL_MODE_PROT64)
				ctxt->rip_relative = 1;
		} else
			modrm_ea += ctxt->regs[ctxt->modrm_rm];
		switch (ctxt->modrm_mod) {
		case 0:
			if (ctxt->modrm_rm == 5)
				modrm_ea += insn_fetch(s32, ctxt);
			break;
=======
			else {
				modrm_ea += reg_read(ctxt, base_reg);
				adjust_modrm_seg(ctxt, base_reg);
				/* Increment ESP on POP [ESP] */
				if ((ctxt->d & IncSP) &&
				    base_reg == VCPU_REGS_RSP)
					modrm_ea += ctxt->op_bytes;
			}
			if (index_reg != 4)
				modrm_ea += reg_read(ctxt, index_reg) << scale;
		} else if ((ctxt->modrm_rm & 7) == 5 && ctxt->modrm_mod == 0) {
			modrm_ea += insn_fetch(s32, ctxt);
			if (ctxt->mode == X86EMUL_MODE_PROT64)
				ctxt->rip_relative = 1;
		} else {
			base_reg = ctxt->modrm_rm;
			modrm_ea += reg_read(ctxt, base_reg);
			adjust_modrm_seg(ctxt, base_reg);
		}
		switch (ctxt->modrm_mod) {
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		case 1:
			modrm_ea += insn_fetch(s8, ctxt);
			break;
		case 2:
			modrm_ea += insn_fetch(s32, ctxt);
			break;
		}
	}
	op->addr.mem.ea = modrm_ea;
<<<<<<< HEAD
=======
	if (ctxt->ad_bytes != 8)
		ctxt->memop.addr.mem.ea = (u32)ctxt->memop.addr.mem.ea;

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
done:
	return rc;
}

static int decode_abs(struct x86_emulate_ctxt *ctxt,
		      struct operand *op)
{
	int rc = X86EMUL_CONTINUE;

	op->type = OP_MEM;
	switch (ctxt->ad_bytes) {
	case 2:
		op->addr.mem.ea = insn_fetch(u16, ctxt);
		break;
	case 4:
		op->addr.mem.ea = insn_fetch(u32, ctxt);
		break;
	case 8:
		op->addr.mem.ea = insn_fetch(u64, ctxt);
		break;
	}
done:
	return rc;
}

static void fetch_bit_operand(struct x86_emulate_ctxt *ctxt)
{
	long sv = 0, mask;

	if (ctxt->dst.type == OP_MEM && ctxt->src.type == OP_REG) {
<<<<<<< HEAD
		mask = ~(ctxt->dst.bytes * 8 - 1);
=======
		mask = ~((long)ctxt->dst.bytes * 8 - 1);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

		if (ctxt->src.bytes == 2)
			sv = (s16)ctxt->src.val & (s16)mask;
		else if (ctxt->src.bytes == 4)
			sv = (s32)ctxt->src.val & (s32)mask;
<<<<<<< HEAD

		ctxt->dst.addr.mem.ea += (sv >> 3);
=======
		else
			sv = (s64)ctxt->src.val & (s64)mask;

		ctxt->dst.addr.mem.ea = address_mask(ctxt,
					   ctxt->dst.addr.mem.ea + (sv >> 3));
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	}

	/* only subword offset */
	ctxt->src.val &= (ctxt->dst.bytes << 3) - 1;
}

static int read_emulated(struct x86_emulate_ctxt *ctxt,
			 unsigned long addr, void *dest, unsigned size)
{
	int rc;
	struct read_cache *mc = &ctxt->mem_read;

<<<<<<< HEAD
	while (size) {
		int n = min(size, 8u);
		size -= n;
		if (mc->pos < mc->end)
			goto read_cached;

		rc = ctxt->ops->read_emulated(ctxt, addr, mc->data + mc->end, n,
					      &ctxt->exception);
		if (rc != X86EMUL_CONTINUE)
			return rc;
		mc->end += n;

	read_cached:
		memcpy(dest, mc->data + mc->pos, n);
		mc->pos += n;
		dest += n;
		addr += n;
	}
=======
	if (mc->pos < mc->end)
		goto read_cached;

	if (KVM_EMULATOR_BUG_ON((mc->end + size) >= sizeof(mc->data), ctxt))
		return X86EMUL_UNHANDLEABLE;

	rc = ctxt->ops->read_emulated(ctxt, addr, mc->data + mc->end, size,
				      &ctxt->exception);
	if (rc != X86EMUL_CONTINUE)
		return rc;

	mc->end += size;

read_cached:
	memcpy(dest, mc->data + mc->pos, size);
	mc->pos += size;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	return X86EMUL_CONTINUE;
}

static int segmented_read(struct x86_emulate_ctxt *ctxt,
			  struct segmented_address addr,
			  void *data,
			  unsigned size)
{
	int rc;
	ulong linear;

	rc = linearize(ctxt, addr, size, false, &linear);
	if (rc != X86EMUL_CONTINUE)
		return rc;
	return read_emulated(ctxt, linear, data, size);
}

static int segmented_write(struct x86_emulate_ctxt *ctxt,
			   struct segmented_address addr,
			   const void *data,
			   unsigned size)
{
	int rc;
	ulong linear;

	rc = linearize(ctxt, addr, size, true, &linear);
	if (rc != X86EMUL_CONTINUE)
		return rc;
	return ctxt->ops->write_emulated(ctxt, linear, data, size,
					 &ctxt->exception);
}

static int segmented_cmpxchg(struct x86_emulate_ctxt *ctxt,
			     struct segmented_address addr,
			     const void *orig_data, const void *data,
			     unsigned size)
{
	int rc;
	ulong linear;

	rc = linearize(ctxt, addr, size, true, &linear);
	if (rc != X86EMUL_CONTINUE)
		return rc;
	return ctxt->ops->cmpxchg_emulated(ctxt, linear, orig_data, data,
					   size, &ctxt->exception);
}

static int pio_in_emulated(struct x86_emulate_ctxt *ctxt,
			   unsigned int size, unsigned short port,
			   void *dest)
{
	struct read_cache *rc = &ctxt->io_read;

	if (rc->pos == rc->end) { /* refill pio read ahead */
		unsigned int in_page, n;
		unsigned int count = ctxt->rep_prefix ?
<<<<<<< HEAD
			address_mask(ctxt, ctxt->regs[VCPU_REGS_RCX]) : 1;
		in_page = (ctxt->eflags & EFLG_DF) ?
			offset_in_page(ctxt->regs[VCPU_REGS_RDI]) :
			PAGE_SIZE - offset_in_page(ctxt->regs[VCPU_REGS_RDI]);
		n = min(min(in_page, (unsigned int)sizeof(rc->data)) / size,
			count);
=======
			address_mask(ctxt, reg_read(ctxt, VCPU_REGS_RCX)) : 1;
		in_page = (ctxt->eflags & X86_EFLAGS_DF) ?
			offset_in_page(reg_read(ctxt, VCPU_REGS_RDI)) :
			PAGE_SIZE - offset_in_page(reg_read(ctxt, VCPU_REGS_RDI));
		n = min3(in_page, (unsigned int)sizeof(rc->data) / size, count);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		if (n == 0)
			n = 1;
		rc->pos = rc->end = 0;
		if (!ctxt->ops->pio_in_emulated(ctxt, size, port, rc->data, n))
			return 0;
		rc->end = n * size;
	}

<<<<<<< HEAD
	memcpy(dest, rc->data + rc->pos, size);
	rc->pos += size;
=======
	if (ctxt->rep_prefix && (ctxt->d & String) &&
	    !(ctxt->eflags & X86_EFLAGS_DF)) {
		ctxt->dst.data = rc->data + rc->pos;
		ctxt->dst.type = OP_MEM_STR;
		ctxt->dst.count = (rc->end - rc->pos) / size;
		rc->pos = rc->end;
	} else {
		memcpy(dest, rc->data + rc->pos, size);
		rc->pos += size;
	}
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	return 1;
}

static int read_interrupt_descriptor(struct x86_emulate_ctxt *ctxt,
				     u16 index, struct desc_struct *desc)
{
	struct desc_ptr dt;
	ulong addr;

	ctxt->ops->get_idt(ctxt, &dt);

	if (dt.size < index * 8 + 7)
		return emulate_gp(ctxt, index << 3 | 0x2);

	addr = dt.address + index * 8;
<<<<<<< HEAD
	return ctxt->ops->read_std(ctxt, addr, desc, sizeof *desc,
				   &ctxt->exception);
=======
	return linear_read_system(ctxt, addr, desc, sizeof(*desc));
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

static void get_descriptor_table_ptr(struct x86_emulate_ctxt *ctxt,
				     u16 selector, struct desc_ptr *dt)
{
<<<<<<< HEAD
	struct x86_emulate_ops *ops = ctxt->ops;
=======
	const struct x86_emulate_ops *ops = ctxt->ops;
	u32 base3 = 0;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	if (selector & 1 << 2) {
		struct desc_struct desc;
		u16 sel;

<<<<<<< HEAD
		memset (dt, 0, sizeof *dt);
		if (!ops->get_segment(ctxt, &sel, &desc, NULL, VCPU_SREG_LDTR))
			return;

		dt->size = desc_limit_scaled(&desc); /* what if limit > 65535? */
		dt->address = get_desc_base(&desc);
=======
		memset(dt, 0, sizeof(*dt));
		if (!ops->get_segment(ctxt, &sel, &desc, &base3,
				      VCPU_SREG_LDTR))
			return;

		dt->size = desc_limit_scaled(&desc); /* what if limit > 65535? */
		dt->address = get_desc_base(&desc) | ((u64)base3 << 32);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	} else
		ops->get_gdt(ctxt, dt);
}

<<<<<<< HEAD
/* allowed just for 8 bytes segments */
static int read_segment_descriptor(struct x86_emulate_ctxt *ctxt,
				   u16 selector, struct desc_struct *desc)
=======
static int get_descriptor_ptr(struct x86_emulate_ctxt *ctxt,
			      u16 selector, ulong *desc_addr_p)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
{
	struct desc_ptr dt;
	u16 index = selector >> 3;
	ulong addr;

	get_descriptor_table_ptr(ctxt, selector, &dt);

	if (dt.size < index * 8 + 7)
		return emulate_gp(ctxt, selector & 0xfffc);

	addr = dt.address + index * 8;
<<<<<<< HEAD
	return ctxt->ops->read_std(ctxt, addr, desc, sizeof *desc,
				   &ctxt->exception);
=======

#ifdef CONFIG_X86_64
	if (addr >> 32 != 0) {
		u64 efer = 0;

		ctxt->ops->get_msr(ctxt, MSR_EFER, &efer);
		if (!(efer & EFER_LMA))
			addr &= (u32)-1;
	}
#endif

	*desc_addr_p = addr;
	return X86EMUL_CONTINUE;
}

/* allowed just for 8 bytes segments */
static int read_segment_descriptor(struct x86_emulate_ctxt *ctxt,
				   u16 selector, struct desc_struct *desc,
				   ulong *desc_addr_p)
{
	int rc;

	rc = get_descriptor_ptr(ctxt, selector, desc_addr_p);
	if (rc != X86EMUL_CONTINUE)
		return rc;

	return linear_read_system(ctxt, *desc_addr_p, desc, sizeof(*desc));
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

/* allowed just for 8 bytes segments */
static int write_segment_descriptor(struct x86_emulate_ctxt *ctxt,
				    u16 selector, struct desc_struct *desc)
{
<<<<<<< HEAD
	struct desc_ptr dt;
	u16 index = selector >> 3;
	ulong addr;

	get_descriptor_table_ptr(ctxt, selector, &dt);

	if (dt.size < index * 8 + 7)
		return emulate_gp(ctxt, selector & 0xfffc);

	addr = dt.address + index * 8;
	return ctxt->ops->write_std(ctxt, addr, desc, sizeof *desc,
				    &ctxt->exception);
}

/* Does not support long mode */
static int __load_segment_descriptor(struct x86_emulate_ctxt *ctxt,
				     u16 selector, int seg, u8 cpl,
				     bool in_task_switch,
				     struct desc_struct *desc)
{
	struct desc_struct seg_desc;
=======
	int rc;
	ulong addr;

	rc = get_descriptor_ptr(ctxt, selector, &addr);
	if (rc != X86EMUL_CONTINUE)
		return rc;

	return linear_write_system(ctxt, addr, desc, sizeof(*desc));
}

static int __load_segment_descriptor(struct x86_emulate_ctxt *ctxt,
				     u16 selector, int seg, u8 cpl,
				     enum x86_transfer_type transfer,
				     struct desc_struct *desc)
{
	struct desc_struct seg_desc, old_desc;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	u8 dpl, rpl;
	unsigned err_vec = GP_VECTOR;
	u32 err_code = 0;
	bool null_selector = !(selector & ~0x3); /* 0000-0003 are null */
<<<<<<< HEAD
	int ret;

	memset(&seg_desc, 0, sizeof seg_desc);

	if ((seg <= VCPU_SREG_GS && ctxt->mode == X86EMUL_MODE_VM86)
	    || ctxt->mode == X86EMUL_MODE_REAL) {
		/* set real mode segment descriptor */
=======
	ulong desc_addr;
	int ret;
	u16 dummy;
	u32 base3 = 0;

	memset(&seg_desc, 0, sizeof(seg_desc));

	if (ctxt->mode == X86EMUL_MODE_REAL) {
		/* set real mode segment descriptor (keep limit etc. for
		 * unreal mode) */
		ctxt->ops->get_segment(ctxt, &dummy, &seg_desc, NULL, seg);
		set_desc_base(&seg_desc, selector << 4);
		goto load;
	} else if (seg <= VCPU_SREG_GS && ctxt->mode == X86EMUL_MODE_VM86) {
		/* VM86 needs a clean new segment descriptor */
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		set_desc_base(&seg_desc, selector << 4);
		set_desc_limit(&seg_desc, 0xffff);
		seg_desc.type = 3;
		seg_desc.p = 1;
		seg_desc.s = 1;
<<<<<<< HEAD
		if (ctxt->mode == X86EMUL_MODE_VM86)
			seg_desc.dpl = 3;
		goto load;
	}

	/* NULL selector is not valid for TR, CS and SS */
	if ((seg == VCPU_SREG_CS || seg == VCPU_SREG_SS || seg == VCPU_SREG_TR)
	    && null_selector)
		goto exception;
=======
		seg_desc.dpl = 3;
		goto load;
	}

	rpl = selector & 3;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	/* TR should be in GDT only */
	if (seg == VCPU_SREG_TR && (selector & (1 << 2)))
		goto exception;

<<<<<<< HEAD
	if (null_selector) /* for NULL selector skip all following checks */
		goto load;

	ret = read_segment_descriptor(ctxt, selector, &seg_desc);
=======
	/* NULL selector is not valid for TR, CS and (except for long mode) SS */
	if (null_selector) {
		if (seg == VCPU_SREG_CS || seg == VCPU_SREG_TR)
			goto exception;

		if (seg == VCPU_SREG_SS) {
			if (ctxt->mode != X86EMUL_MODE_PROT64 || rpl != cpl)
				goto exception;

			/*
			 * ctxt->ops->set_segment expects the CPL to be in
			 * SS.DPL, so fake an expand-up 32-bit data segment.
			 */
			seg_desc.type = 3;
			seg_desc.p = 1;
			seg_desc.s = 1;
			seg_desc.dpl = cpl;
			seg_desc.d = 1;
			seg_desc.g = 1;
		}

		/* Skip all following checks */
		goto load;
	}

	ret = read_segment_descriptor(ctxt, selector, &seg_desc, &desc_addr);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	if (ret != X86EMUL_CONTINUE)
		return ret;

	err_code = selector & 0xfffc;
<<<<<<< HEAD
	err_vec = GP_VECTOR;

	/* can't load system descriptor into segment selecor */
	if (seg <= VCPU_SREG_GS && !seg_desc.s)
		goto exception;

	if (!seg_desc.p) {
		err_vec = (seg == VCPU_SREG_SS) ? SS_VECTOR : NP_VECTOR;
		goto exception;
	}

	rpl = selector & 3;
=======
	err_vec = (transfer == X86_TRANSFER_TASK_SWITCH) ? TS_VECTOR :
							   GP_VECTOR;

	/* can't load system descriptor into segment selector */
	if (seg <= VCPU_SREG_GS && !seg_desc.s) {
		if (transfer == X86_TRANSFER_CALL_JMP)
			return X86EMUL_UNHANDLEABLE;
		goto exception;
	}

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	dpl = seg_desc.dpl;

	switch (seg) {
	case VCPU_SREG_SS:
		/*
		 * segment is not a writable data segment or segment
<<<<<<< HEAD
		 * selector's RPL != CPL or segment selector's RPL != CPL
=======
		 * selector's RPL != CPL or DPL != CPL
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		 */
		if (rpl != cpl || (seg_desc.type & 0xa) != 0x2 || dpl != cpl)
			goto exception;
		break;
	case VCPU_SREG_CS:
<<<<<<< HEAD
		if (!(seg_desc.type & 8))
			goto exception;

		if (seg_desc.type & 4) {
			/* conforming */
			if (dpl > cpl)
				goto exception;
		} else {
			/* nonconforming */
			if (rpl > cpl || dpl != cpl)
				goto exception;
		}
=======
		/*
		 * KVM uses "none" when loading CS as part of emulating Real
		 * Mode exceptions and IRET (handled above).  In all other
		 * cases, loading CS without a control transfer is a KVM bug.
		 */
		if (WARN_ON_ONCE(transfer == X86_TRANSFER_NONE))
			goto exception;

		if (!(seg_desc.type & 8))
			goto exception;

		if (transfer == X86_TRANSFER_RET) {
			/* RET can never return to an inner privilege level. */
			if (rpl < cpl)
				goto exception;
			/* Outer-privilege level return is not implemented */
			if (rpl > cpl)
				return X86EMUL_UNHANDLEABLE;
		}
		if (transfer == X86_TRANSFER_RET || transfer == X86_TRANSFER_TASK_SWITCH) {
			if (seg_desc.type & 4) {
				/* conforming */
				if (dpl > rpl)
					goto exception;
			} else {
				/* nonconforming */
				if (dpl != rpl)
					goto exception;
			}
		} else { /* X86_TRANSFER_CALL_JMP */
			if (seg_desc.type & 4) {
				/* conforming */
				if (dpl > cpl)
					goto exception;
			} else {
				/* nonconforming */
				if (rpl > cpl || dpl != cpl)
					goto exception;
			}
		}
		/* in long-mode d/b must be clear if l is set */
		if (seg_desc.d && seg_desc.l) {
			u64 efer = 0;

			ctxt->ops->get_msr(ctxt, MSR_EFER, &efer);
			if (efer & EFER_LMA)
				goto exception;
		}

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		/* CS(RPL) <- CPL */
		selector = (selector & 0xfffc) | cpl;
		break;
	case VCPU_SREG_TR:
		if (seg_desc.s || (seg_desc.type != 1 && seg_desc.type != 9))
			goto exception;
		break;
	case VCPU_SREG_LDTR:
		if (seg_desc.s || seg_desc.type != 2)
			goto exception;
		break;
	default: /*  DS, ES, FS, or GS */
		/*
		 * segment is not a data or readable code segment or
		 * ((segment is a data or nonconforming code segment)
<<<<<<< HEAD
		 * and (both RPL and CPL > DPL))
		 */
		if ((seg_desc.type & 0xa) == 0x8 ||
		    (((seg_desc.type & 0xc) != 0xc) &&
		     (rpl > dpl && cpl > dpl)))
=======
		 * and ((RPL > DPL) or (CPL > DPL)))
		 */
		if ((seg_desc.type & 0xa) == 0x8 ||
		    (((seg_desc.type & 0xc) != 0xc) &&
		     (rpl > dpl || cpl > dpl)))
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
			goto exception;
		break;
	}

<<<<<<< HEAD
	if (seg_desc.s) {
		/* mark segment as accessed */
		seg_desc.type |= 1;
		ret = write_segment_descriptor(ctxt, selector, &seg_desc);
=======
	if (!seg_desc.p) {
		err_vec = (seg == VCPU_SREG_SS) ? SS_VECTOR : NP_VECTOR;
		goto exception;
	}

	if (seg_desc.s) {
		/* mark segment as accessed */
		if (!(seg_desc.type & 1)) {
			seg_desc.type |= 1;
			ret = write_segment_descriptor(ctxt, selector,
						       &seg_desc);
			if (ret != X86EMUL_CONTINUE)
				return ret;
		}
	} else if (ctxt->mode == X86EMUL_MODE_PROT64) {
		ret = linear_read_system(ctxt, desc_addr+8, &base3, sizeof(base3));
		if (ret != X86EMUL_CONTINUE)
			return ret;
		if (emul_is_noncanonical_address(get_desc_base(&seg_desc) |
						 ((u64)base3 << 32), ctxt))
			return emulate_gp(ctxt, err_code);
	}

	if (seg == VCPU_SREG_TR) {
		old_desc = seg_desc;
		seg_desc.type |= 2; /* busy */
		ret = ctxt->ops->cmpxchg_emulated(ctxt, desc_addr, &old_desc, &seg_desc,
						  sizeof(seg_desc), &ctxt->exception);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		if (ret != X86EMUL_CONTINUE)
			return ret;
	}
load:
<<<<<<< HEAD
	ctxt->ops->set_segment(ctxt, selector, &seg_desc, 0, seg);
=======
	ctxt->ops->set_segment(ctxt, selector, &seg_desc, base3, seg);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	if (desc)
		*desc = seg_desc;
	return X86EMUL_CONTINUE;
exception:
<<<<<<< HEAD
	emulate_exception(ctxt, err_vec, err_code, true);
	return X86EMUL_PROPAGATE_FAULT;
=======
	return emulate_exception(ctxt, err_vec, err_code, true);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

static int load_segment_descriptor(struct x86_emulate_ctxt *ctxt,
				   u16 selector, int seg)
{
	u8 cpl = ctxt->ops->cpl(ctxt);
<<<<<<< HEAD
	return __load_segment_descriptor(ctxt, selector, seg, cpl, false, NULL);
=======

	/*
	 * None of MOV, POP and LSS can load a NULL selector in CPL=3, but
	 * they can load it at CPL<3 (Intel's manual says only LSS can,
	 * but it's wrong).
	 *
	 * However, the Intel manual says that putting IST=1/DPL=3 in
	 * an interrupt gate will result in SS=3 (the AMD manual instead
	 * says it doesn't), so allow SS=3 in __load_segment_descriptor
	 * and only forbid it here.
	 */
	if (seg == VCPU_SREG_SS && selector == 3 &&
	    ctxt->mode == X86EMUL_MODE_PROT64)
		return emulate_exception(ctxt, GP_VECTOR, 0, true);

	return __load_segment_descriptor(ctxt, selector, seg, cpl,
					 X86_TRANSFER_NONE, NULL);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

static void write_register_operand(struct operand *op)
{
<<<<<<< HEAD
	/* The 4-byte case *is* correct: in 64-bit mode we zero-extend. */
	switch (op->bytes) {
	case 1:
		*(u8 *)op->addr.reg = (u8)op->val;
		break;
	case 2:
		*(u16 *)op->addr.reg = (u16)op->val;
		break;
	case 4:
		*op->addr.reg = (u32)op->val;
		break;	/* 64b: zero-extend */
	case 8:
		*op->addr.reg = op->val;
		break;
	}
}

static int writeback(struct x86_emulate_ctxt *ctxt)
{
	int rc;

	switch (ctxt->dst.type) {
	case OP_REG:
		write_register_operand(&ctxt->dst);
		break;
	case OP_MEM:
		if (ctxt->lock_prefix)
			rc = segmented_cmpxchg(ctxt,
					       ctxt->dst.addr.mem,
					       &ctxt->dst.orig_val,
					       &ctxt->dst.val,
					       ctxt->dst.bytes);
		else
			rc = segmented_write(ctxt,
					     ctxt->dst.addr.mem,
					     &ctxt->dst.val,
					     ctxt->dst.bytes);
		if (rc != X86EMUL_CONTINUE)
			return rc;
		break;
	case OP_XMM:
		write_sse_reg(ctxt, &ctxt->dst.vec_val, ctxt->dst.addr.xmm);
=======
	return assign_register(op->addr.reg, op->val, op->bytes);
}

static int writeback(struct x86_emulate_ctxt *ctxt, struct operand *op)
{
	switch (op->type) {
	case OP_REG:
		write_register_operand(op);
		break;
	case OP_MEM:
		if (ctxt->lock_prefix)
			return segmented_cmpxchg(ctxt,
						 op->addr.mem,
						 &op->orig_val,
						 &op->val,
						 op->bytes);
		else
			return segmented_write(ctxt,
					       op->addr.mem,
					       &op->val,
					       op->bytes);
	case OP_MEM_STR:
		return segmented_write(ctxt,
				       op->addr.mem,
				       op->data,
				       op->bytes * op->count);
	case OP_XMM:
		kvm_write_sse_reg(op->addr.xmm, &op->vec_val);
		break;
	case OP_MM:
		kvm_write_mmx_reg(op->addr.mm, &op->mm_val);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		break;
	case OP_NONE:
		/* no writeback */
		break;
	default:
		break;
	}
	return X86EMUL_CONTINUE;
}

<<<<<<< HEAD
static int em_push(struct x86_emulate_ctxt *ctxt)
{
	struct segmented_address addr;

	register_address_increment(ctxt, &ctxt->regs[VCPU_REGS_RSP], -ctxt->op_bytes);
	addr.ea = register_address(ctxt, ctxt->regs[VCPU_REGS_RSP]);
	addr.seg = VCPU_SREG_SS;

	/* Disable writeback. */
	ctxt->dst.type = OP_NONE;
	return segmented_write(ctxt, addr, &ctxt->src.val, ctxt->op_bytes);
=======
static int emulate_push(struct x86_emulate_ctxt *ctxt, const void *data, int len)
{
	struct segmented_address addr;

	rsp_increment(ctxt, -len);
	addr.ea = reg_read(ctxt, VCPU_REGS_RSP) & stack_mask(ctxt);
	addr.seg = VCPU_SREG_SS;

	return segmented_write(ctxt, addr, data, len);
}

static int em_push(struct x86_emulate_ctxt *ctxt)
{
	/* Disable writeback. */
	ctxt->dst.type = OP_NONE;
	return emulate_push(ctxt, &ctxt->src.val, ctxt->op_bytes);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

static int emulate_pop(struct x86_emulate_ctxt *ctxt,
		       void *dest, int len)
{
	int rc;
	struct segmented_address addr;

<<<<<<< HEAD
	addr.ea = register_address(ctxt, ctxt->regs[VCPU_REGS_RSP]);
=======
	addr.ea = reg_read(ctxt, VCPU_REGS_RSP) & stack_mask(ctxt);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	addr.seg = VCPU_SREG_SS;
	rc = segmented_read(ctxt, addr, dest, len);
	if (rc != X86EMUL_CONTINUE)
		return rc;

<<<<<<< HEAD
	register_address_increment(ctxt, &ctxt->regs[VCPU_REGS_RSP], len);
=======
	rsp_increment(ctxt, len);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	return rc;
}

static int em_pop(struct x86_emulate_ctxt *ctxt)
{
	return emulate_pop(ctxt, &ctxt->dst.val, ctxt->op_bytes);
}

static int emulate_popf(struct x86_emulate_ctxt *ctxt,
			void *dest, int len)
{
	int rc;
<<<<<<< HEAD
	unsigned long val, change_mask;
	int iopl = (ctxt->eflags & X86_EFLAGS_IOPL) >> IOPL_SHIFT;
=======
	unsigned long val = 0;
	unsigned long change_mask;
	int iopl = (ctxt->eflags & X86_EFLAGS_IOPL) >> X86_EFLAGS_IOPL_BIT;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	int cpl = ctxt->ops->cpl(ctxt);

	rc = emulate_pop(ctxt, &val, len);
	if (rc != X86EMUL_CONTINUE)
		return rc;

<<<<<<< HEAD
	change_mask = EFLG_CF | EFLG_PF | EFLG_AF | EFLG_ZF | EFLG_SF | EFLG_OF
		| EFLG_TF | EFLG_DF | EFLG_NT | EFLG_RF | EFLG_AC | EFLG_ID;
=======
	change_mask = X86_EFLAGS_CF | X86_EFLAGS_PF | X86_EFLAGS_AF |
		      X86_EFLAGS_ZF | X86_EFLAGS_SF | X86_EFLAGS_OF |
		      X86_EFLAGS_TF | X86_EFLAGS_DF | X86_EFLAGS_NT |
		      X86_EFLAGS_AC | X86_EFLAGS_ID;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	switch(ctxt->mode) {
	case X86EMUL_MODE_PROT64:
	case X86EMUL_MODE_PROT32:
	case X86EMUL_MODE_PROT16:
		if (cpl == 0)
<<<<<<< HEAD
			change_mask |= EFLG_IOPL;
		if (cpl <= iopl)
			change_mask |= EFLG_IF;
=======
			change_mask |= X86_EFLAGS_IOPL;
		if (cpl <= iopl)
			change_mask |= X86_EFLAGS_IF;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		break;
	case X86EMUL_MODE_VM86:
		if (iopl < 3)
			return emulate_gp(ctxt, 0);
<<<<<<< HEAD
		change_mask |= EFLG_IF;
		break;
	default: /* real mode */
		change_mask |= (EFLG_IOPL | EFLG_IF);
=======
		change_mask |= X86_EFLAGS_IF;
		break;
	default: /* real mode */
		change_mask |= (X86_EFLAGS_IOPL | X86_EFLAGS_IF);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		break;
	}

	*(unsigned long *)dest =
		(ctxt->eflags & ~change_mask) | (val & change_mask);

	return rc;
}

static int em_popf(struct x86_emulate_ctxt *ctxt)
{
	ctxt->dst.type = OP_REG;
	ctxt->dst.addr.reg = &ctxt->eflags;
	ctxt->dst.bytes = ctxt->op_bytes;
	return emulate_popf(ctxt, &ctxt->dst.val, ctxt->op_bytes);
}

<<<<<<< HEAD
=======
static int em_enter(struct x86_emulate_ctxt *ctxt)
{
	int rc;
	unsigned frame_size = ctxt->src.val;
	unsigned nesting_level = ctxt->src2.val & 31;
	ulong rbp;

	if (nesting_level)
		return X86EMUL_UNHANDLEABLE;

	rbp = reg_read(ctxt, VCPU_REGS_RBP);
	rc = emulate_push(ctxt, &rbp, stack_size(ctxt));
	if (rc != X86EMUL_CONTINUE)
		return rc;
	assign_masked(reg_rmw(ctxt, VCPU_REGS_RBP), reg_read(ctxt, VCPU_REGS_RSP),
		      stack_mask(ctxt));
	assign_masked(reg_rmw(ctxt, VCPU_REGS_RSP),
		      reg_read(ctxt, VCPU_REGS_RSP) - frame_size,
		      stack_mask(ctxt));
	return X86EMUL_CONTINUE;
}

static int em_leave(struct x86_emulate_ctxt *ctxt)
{
	assign_masked(reg_rmw(ctxt, VCPU_REGS_RSP), reg_read(ctxt, VCPU_REGS_RBP),
		      stack_mask(ctxt));
	return emulate_pop(ctxt, reg_rmw(ctxt, VCPU_REGS_RBP), ctxt->op_bytes);
}

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
static int em_push_sreg(struct x86_emulate_ctxt *ctxt)
{
	int seg = ctxt->src2.val;

	ctxt->src.val = get_segment_selector(ctxt, seg);
<<<<<<< HEAD
=======
	if (ctxt->op_bytes == 4) {
		rsp_increment(ctxt, -2);
		ctxt->op_bytes = 2;
	}
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	return em_push(ctxt);
}

static int em_pop_sreg(struct x86_emulate_ctxt *ctxt)
{
	int seg = ctxt->src2.val;
<<<<<<< HEAD
	unsigned long selector;
	int rc;

	rc = emulate_pop(ctxt, &selector, ctxt->op_bytes);
	if (rc != X86EMUL_CONTINUE)
		return rc;

=======
	unsigned long selector = 0;
	int rc;

	rc = emulate_pop(ctxt, &selector, 2);
	if (rc != X86EMUL_CONTINUE)
		return rc;

	if (seg == VCPU_SREG_SS)
		ctxt->interruptibility = KVM_X86_SHADOW_INT_MOV_SS;
	if (ctxt->op_bytes > 2)
		rsp_increment(ctxt, ctxt->op_bytes - 2);

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	rc = load_segment_descriptor(ctxt, (u16)selector, seg);
	return rc;
}

static int em_pusha(struct x86_emulate_ctxt *ctxt)
{
<<<<<<< HEAD
	unsigned long old_esp = ctxt->regs[VCPU_REGS_RSP];
=======
	unsigned long old_esp = reg_read(ctxt, VCPU_REGS_RSP);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	int rc = X86EMUL_CONTINUE;
	int reg = VCPU_REGS_RAX;

	while (reg <= VCPU_REGS_RDI) {
		(reg == VCPU_REGS_RSP) ?
<<<<<<< HEAD
		(ctxt->src.val = old_esp) : (ctxt->src.val = ctxt->regs[reg]);
=======
		(ctxt->src.val = old_esp) : (ctxt->src.val = reg_read(ctxt, reg));
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

		rc = em_push(ctxt);
		if (rc != X86EMUL_CONTINUE)
			return rc;

		++reg;
	}

	return rc;
}

static int em_pushf(struct x86_emulate_ctxt *ctxt)
{
<<<<<<< HEAD
	ctxt->src.val =  (unsigned long)ctxt->eflags;
=======
	ctxt->src.val = (unsigned long)ctxt->eflags & ~X86_EFLAGS_VM;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	return em_push(ctxt);
}

static int em_popa(struct x86_emulate_ctxt *ctxt)
{
	int rc = X86EMUL_CONTINUE;
	int reg = VCPU_REGS_RDI;
<<<<<<< HEAD

	while (reg >= VCPU_REGS_RAX) {
		if (reg == VCPU_REGS_RSP) {
			register_address_increment(ctxt, &ctxt->regs[VCPU_REGS_RSP],
							ctxt->op_bytes);
			--reg;
		}

		rc = emulate_pop(ctxt, &ctxt->regs[reg], ctxt->op_bytes);
		if (rc != X86EMUL_CONTINUE)
			break;
=======
	u32 val = 0;

	while (reg >= VCPU_REGS_RAX) {
		if (reg == VCPU_REGS_RSP) {
			rsp_increment(ctxt, ctxt->op_bytes);
			--reg;
		}

		rc = emulate_pop(ctxt, &val, ctxt->op_bytes);
		if (rc != X86EMUL_CONTINUE)
			break;
		assign_register(reg_rmw(ctxt, reg), val, ctxt->op_bytes);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		--reg;
	}
	return rc;
}

<<<<<<< HEAD
int emulate_int_real(struct x86_emulate_ctxt *ctxt, int irq)
{
	struct x86_emulate_ops *ops = ctxt->ops;
=======
static int __emulate_int_real(struct x86_emulate_ctxt *ctxt, int irq)
{
	const struct x86_emulate_ops *ops = ctxt->ops;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	int rc;
	struct desc_ptr dt;
	gva_t cs_addr;
	gva_t eip_addr;
	u16 cs, eip;

	/* TODO: Add limit checks */
	ctxt->src.val = ctxt->eflags;
	rc = em_push(ctxt);
	if (rc != X86EMUL_CONTINUE)
		return rc;

<<<<<<< HEAD
	ctxt->eflags &= ~(EFLG_IF | EFLG_TF | EFLG_AC);
=======
	ctxt->eflags &= ~(X86_EFLAGS_IF | X86_EFLAGS_TF | X86_EFLAGS_AC);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	ctxt->src.val = get_segment_selector(ctxt, VCPU_SREG_CS);
	rc = em_push(ctxt);
	if (rc != X86EMUL_CONTINUE)
		return rc;

	ctxt->src.val = ctxt->_eip;
	rc = em_push(ctxt);
	if (rc != X86EMUL_CONTINUE)
		return rc;

	ops->get_idt(ctxt, &dt);

	eip_addr = dt.address + (irq << 2);
	cs_addr = dt.address + (irq << 2) + 2;

<<<<<<< HEAD
	rc = ops->read_std(ctxt, cs_addr, &cs, 2, &ctxt->exception);
	if (rc != X86EMUL_CONTINUE)
		return rc;

	rc = ops->read_std(ctxt, eip_addr, &eip, 2, &ctxt->exception);
=======
	rc = linear_read_system(ctxt, cs_addr, &cs, 2);
	if (rc != X86EMUL_CONTINUE)
		return rc;

	rc = linear_read_system(ctxt, eip_addr, &eip, 2);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	if (rc != X86EMUL_CONTINUE)
		return rc;

	rc = load_segment_descriptor(ctxt, cs, VCPU_SREG_CS);
	if (rc != X86EMUL_CONTINUE)
		return rc;

	ctxt->_eip = eip;

	return rc;
}

<<<<<<< HEAD
=======
int emulate_int_real(struct x86_emulate_ctxt *ctxt, int irq)
{
	int rc;

	invalidate_registers(ctxt);
	rc = __emulate_int_real(ctxt, irq);
	if (rc == X86EMUL_CONTINUE)
		writeback_registers(ctxt);
	return rc;
}

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
static int emulate_int(struct x86_emulate_ctxt *ctxt, int irq)
{
	switch(ctxt->mode) {
	case X86EMUL_MODE_REAL:
<<<<<<< HEAD
		return emulate_int_real(ctxt, irq);
=======
		return __emulate_int_real(ctxt, irq);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	case X86EMUL_MODE_VM86:
	case X86EMUL_MODE_PROT16:
	case X86EMUL_MODE_PROT32:
	case X86EMUL_MODE_PROT64:
	default:
		/* Protected mode interrupts unimplemented yet */
		return X86EMUL_UNHANDLEABLE;
	}
}

static int emulate_iret_real(struct x86_emulate_ctxt *ctxt)
{
	int rc = X86EMUL_CONTINUE;
	unsigned long temp_eip = 0;
	unsigned long temp_eflags = 0;
	unsigned long cs = 0;
<<<<<<< HEAD
	unsigned long mask = EFLG_CF | EFLG_PF | EFLG_AF | EFLG_ZF | EFLG_SF | EFLG_TF |
			     EFLG_IF | EFLG_DF | EFLG_OF | EFLG_IOPL | EFLG_NT | EFLG_RF |
			     EFLG_AC | EFLG_ID | (1 << 1); /* Last one is the reserved bit */
	unsigned long vm86_mask = EFLG_VM | EFLG_VIF | EFLG_VIP;
=======
	unsigned long mask = X86_EFLAGS_CF | X86_EFLAGS_PF | X86_EFLAGS_AF |
			     X86_EFLAGS_ZF | X86_EFLAGS_SF | X86_EFLAGS_TF |
			     X86_EFLAGS_IF | X86_EFLAGS_DF | X86_EFLAGS_OF |
			     X86_EFLAGS_IOPL | X86_EFLAGS_NT | X86_EFLAGS_RF |
			     X86_EFLAGS_AC | X86_EFLAGS_ID |
			     X86_EFLAGS_FIXED;
	unsigned long vm86_mask = X86_EFLAGS_VM | X86_EFLAGS_VIF |
				  X86_EFLAGS_VIP;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	/* TODO: Add stack limit check */

	rc = emulate_pop(ctxt, &temp_eip, ctxt->op_bytes);

	if (rc != X86EMUL_CONTINUE)
		return rc;

	if (temp_eip & ~0xffff)
		return emulate_gp(ctxt, 0);

	rc = emulate_pop(ctxt, &cs, ctxt->op_bytes);

	if (rc != X86EMUL_CONTINUE)
		return rc;

	rc = emulate_pop(ctxt, &temp_eflags, ctxt->op_bytes);

	if (rc != X86EMUL_CONTINUE)
		return rc;

	rc = load_segment_descriptor(ctxt, (u16)cs, VCPU_SREG_CS);

	if (rc != X86EMUL_CONTINUE)
		return rc;

	ctxt->_eip = temp_eip;

<<<<<<< HEAD

=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	if (ctxt->op_bytes == 4)
		ctxt->eflags = ((temp_eflags & mask) | (ctxt->eflags & vm86_mask));
	else if (ctxt->op_bytes == 2) {
		ctxt->eflags &= ~0xffff;
		ctxt->eflags |= temp_eflags;
	}

	ctxt->eflags &= ~EFLG_RESERVED_ZEROS_MASK; /* Clear reserved zeros */
<<<<<<< HEAD
	ctxt->eflags |= EFLG_RESERVED_ONE_MASK;
=======
	ctxt->eflags |= X86_EFLAGS_FIXED;
	ctxt->ops->set_nmi_mask(ctxt, false);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	return rc;
}

static int em_iret(struct x86_emulate_ctxt *ctxt)
{
	switch(ctxt->mode) {
	case X86EMUL_MODE_REAL:
		return emulate_iret_real(ctxt);
	case X86EMUL_MODE_VM86:
	case X86EMUL_MODE_PROT16:
	case X86EMUL_MODE_PROT32:
	case X86EMUL_MODE_PROT64:
	default:
		/* iret from protected mode unimplemented yet */
		return X86EMUL_UNHANDLEABLE;
	}
}

static int em_jmp_far(struct x86_emulate_ctxt *ctxt)
{
	int rc;
<<<<<<< HEAD
	unsigned short sel, old_sel;
	struct desc_struct old_desc, new_desc;
	const struct x86_emulate_ops *ops = ctxt->ops;
	u8 cpl = ctxt->ops->cpl(ctxt);

	/* Assignment of RIP may only fail in 64-bit mode */
	if (ctxt->mode == X86EMUL_MODE_PROT64)
		ops->get_segment(ctxt, &old_sel, &old_desc, NULL,
				 VCPU_SREG_CS);

	memcpy(&sel, ctxt->src.valptr + ctxt->op_bytes, 2);

	rc = __load_segment_descriptor(ctxt, sel, VCPU_SREG_CS, cpl, false,
=======
	unsigned short sel;
	struct desc_struct new_desc;
	u8 cpl = ctxt->ops->cpl(ctxt);

	memcpy(&sel, ctxt->src.valptr + ctxt->op_bytes, 2);

	rc = __load_segment_descriptor(ctxt, sel, VCPU_SREG_CS, cpl,
				       X86_TRANSFER_CALL_JMP,
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
				       &new_desc);
	if (rc != X86EMUL_CONTINUE)
		return rc;

<<<<<<< HEAD
	rc = assign_eip_far(ctxt, ctxt->src.val, new_desc.l);
	if (rc != X86EMUL_CONTINUE) {
		WARN_ON(ctxt->mode != X86EMUL_MODE_PROT64);
		/* assigning eip failed; restore the old cs */
		ops->set_segment(ctxt, old_sel, &old_desc, 0, VCPU_SREG_CS);
		return rc;
	}
	return rc;
}

static int em_grp2(struct x86_emulate_ctxt *ctxt)
{
	switch (ctxt->modrm_reg) {
	case 0:	/* rol */
		emulate_2op_SrcB(ctxt, "rol");
		break;
	case 1:	/* ror */
		emulate_2op_SrcB(ctxt, "ror");
		break;
	case 2:	/* rcl */
		emulate_2op_SrcB(ctxt, "rcl");
		break;
	case 3:	/* rcr */
		emulate_2op_SrcB(ctxt, "rcr");
		break;
	case 4:	/* sal/shl */
	case 6:	/* sal/shl */
		emulate_2op_SrcB(ctxt, "sal");
		break;
	case 5:	/* shr */
		emulate_2op_SrcB(ctxt, "shr");
		break;
	case 7:	/* sar */
		emulate_2op_SrcB(ctxt, "sar");
		break;
	}
	return X86EMUL_CONTINUE;
}

static int em_not(struct x86_emulate_ctxt *ctxt)
{
	ctxt->dst.val = ~ctxt->dst.val;
	return X86EMUL_CONTINUE;
}

static int em_neg(struct x86_emulate_ctxt *ctxt)
{
	emulate_1op(ctxt, "neg");
	return X86EMUL_CONTINUE;
}

static int em_mul_ex(struct x86_emulate_ctxt *ctxt)
{
	u8 ex = 0;

	emulate_1op_rax_rdx(ctxt, "mul", ex);
	return X86EMUL_CONTINUE;
}

static int em_imul_ex(struct x86_emulate_ctxt *ctxt)
{
	u8 ex = 0;

	emulate_1op_rax_rdx(ctxt, "imul", ex);
	return X86EMUL_CONTINUE;
}

static int em_div_ex(struct x86_emulate_ctxt *ctxt)
{
	u8 de = 0;

	emulate_1op_rax_rdx(ctxt, "div", de);
	if (de)
		return emulate_de(ctxt);
	return X86EMUL_CONTINUE;
}

static int em_idiv_ex(struct x86_emulate_ctxt *ctxt)
{
	u8 de = 0;

	emulate_1op_rax_rdx(ctxt, "idiv", de);
	if (de)
		return emulate_de(ctxt);
	return X86EMUL_CONTINUE;
}

static int em_grp45(struct x86_emulate_ctxt *ctxt)
{
	int rc = X86EMUL_CONTINUE;

	switch (ctxt->modrm_reg) {
	case 0:	/* inc */
		emulate_1op(ctxt, "inc");
		break;
	case 1:	/* dec */
		emulate_1op(ctxt, "dec");
		break;
	case 2: /* call near abs */ {
		long int old_eip;
		old_eip = ctxt->_eip;
		rc = assign_eip_near(ctxt, ctxt->src.val);
		if (rc != X86EMUL_CONTINUE)
			break;
		ctxt->src.val = old_eip;
		rc = em_push(ctxt);
		break;
	}
	case 4: /* jmp abs */
		rc = assign_eip_near(ctxt, ctxt->src.val);
		break;
	case 5: /* jmp far */
		rc = em_jmp_far(ctxt);
		break;
	case 6:	/* push */
		rc = em_push(ctxt);
		break;
	}
=======
	rc = assign_eip_far(ctxt, ctxt->src.val);
	/* Error handling is not implemented. */
	if (rc != X86EMUL_CONTINUE)
		return X86EMUL_UNHANDLEABLE;

	return rc;
}

static int em_jmp_abs(struct x86_emulate_ctxt *ctxt)
{
	return assign_eip_near(ctxt, ctxt->src.val);
}

static int em_call_near_abs(struct x86_emulate_ctxt *ctxt)
{
	int rc;
	long int old_eip;

	old_eip = ctxt->_eip;
	rc = assign_eip_near(ctxt, ctxt->src.val);
	if (rc != X86EMUL_CONTINUE)
		return rc;
	ctxt->src.val = old_eip;
	rc = em_push(ctxt);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	return rc;
}

static int em_cmpxchg8b(struct x86_emulate_ctxt *ctxt)
{
	u64 old = ctxt->dst.orig_val64;

<<<<<<< HEAD
	if (((u32) (old >> 0) != (u32) ctxt->regs[VCPU_REGS_RAX]) ||
	    ((u32) (old >> 32) != (u32) ctxt->regs[VCPU_REGS_RDX])) {
		ctxt->regs[VCPU_REGS_RAX] = (u32) (old >> 0);
		ctxt->regs[VCPU_REGS_RDX] = (u32) (old >> 32);
		ctxt->eflags &= ~EFLG_ZF;
	} else {
		ctxt->dst.val64 = ((u64)ctxt->regs[VCPU_REGS_RCX] << 32) |
			(u32) ctxt->regs[VCPU_REGS_RBX];

		ctxt->eflags |= EFLG_ZF;
=======
	if (ctxt->dst.bytes == 16)
		return X86EMUL_UNHANDLEABLE;

	if (((u32) (old >> 0) != (u32) reg_read(ctxt, VCPU_REGS_RAX)) ||
	    ((u32) (old >> 32) != (u32) reg_read(ctxt, VCPU_REGS_RDX))) {
		*reg_write(ctxt, VCPU_REGS_RAX) = (u32) (old >> 0);
		*reg_write(ctxt, VCPU_REGS_RDX) = (u32) (old >> 32);
		ctxt->eflags &= ~X86_EFLAGS_ZF;
	} else {
		ctxt->dst.val64 = ((u64)reg_read(ctxt, VCPU_REGS_RCX) << 32) |
			(u32) reg_read(ctxt, VCPU_REGS_RBX);

		ctxt->eflags |= X86_EFLAGS_ZF;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	}
	return X86EMUL_CONTINUE;
}

static int em_ret(struct x86_emulate_ctxt *ctxt)
{
	int rc;
<<<<<<< HEAD
	unsigned long eip;
=======
	unsigned long eip = 0;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	rc = emulate_pop(ctxt, &eip, ctxt->op_bytes);
	if (rc != X86EMUL_CONTINUE)
		return rc;

	return assign_eip_near(ctxt, eip);
}

static int em_ret_far(struct x86_emulate_ctxt *ctxt)
{
	int rc;
<<<<<<< HEAD
	unsigned long eip, cs;
	u16 old_cs;
	struct desc_struct old_desc, new_desc;
	const struct x86_emulate_ops *ops = ctxt->ops;

	if (ctxt->mode == X86EMUL_MODE_PROT64)
		ops->get_segment(ctxt, &old_cs, &old_desc, NULL,
				 VCPU_SREG_CS);
=======
	unsigned long eip = 0;
	unsigned long cs = 0;
	int cpl = ctxt->ops->cpl(ctxt);
	struct desc_struct new_desc;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	rc = emulate_pop(ctxt, &eip, ctxt->op_bytes);
	if (rc != X86EMUL_CONTINUE)
		return rc;
	rc = emulate_pop(ctxt, &cs, ctxt->op_bytes);
	if (rc != X86EMUL_CONTINUE)
		return rc;
<<<<<<< HEAD
	rc = __load_segment_descriptor(ctxt, (u16)cs, VCPU_SREG_CS, 0, false,
				       &new_desc);
	if (rc != X86EMUL_CONTINUE)
		return rc;
	rc = assign_eip_far(ctxt, eip, new_desc.l);
	if (rc != X86EMUL_CONTINUE) {
		WARN_ON(ctxt->mode != X86EMUL_MODE_PROT64);
		ops->set_segment(ctxt, old_cs, &old_desc, 0, VCPU_SREG_CS);
	}
	return rc;
}

static int em_cmpxchg(struct x86_emulate_ctxt *ctxt)
{
	/* Save real source value, then compare EAX against destination. */
	ctxt->src.orig_val = ctxt->src.val;
	ctxt->src.val = ctxt->regs[VCPU_REGS_RAX];
	emulate_2op_SrcV(ctxt, "cmp");

	if (ctxt->eflags & EFLG_ZF) {
		/* Success: write back to memory. */
		ctxt->dst.val = ctxt->src.orig_val;
	} else {
		/* Failure: write the value we saw to EAX. */
		ctxt->dst.type = OP_REG;
		ctxt->dst.addr.reg = (unsigned long *)&ctxt->regs[VCPU_REGS_RAX];
=======
	rc = __load_segment_descriptor(ctxt, (u16)cs, VCPU_SREG_CS, cpl,
				       X86_TRANSFER_RET,
				       &new_desc);
	if (rc != X86EMUL_CONTINUE)
		return rc;
	rc = assign_eip_far(ctxt, eip);
	/* Error handling is not implemented. */
	if (rc != X86EMUL_CONTINUE)
		return X86EMUL_UNHANDLEABLE;

	return rc;
}

static int em_ret_far_imm(struct x86_emulate_ctxt *ctxt)
{
        int rc;

        rc = em_ret_far(ctxt);
        if (rc != X86EMUL_CONTINUE)
                return rc;
        rsp_increment(ctxt, ctxt->src.val);
        return X86EMUL_CONTINUE;
}

static int em_cmpxchg(struct x86_emulate_ctxt *ctxt)
{
	/* Save real source value, then compare EAX against destination. */
	ctxt->dst.orig_val = ctxt->dst.val;
	ctxt->dst.val = reg_read(ctxt, VCPU_REGS_RAX);
	ctxt->src.orig_val = ctxt->src.val;
	ctxt->src.val = ctxt->dst.orig_val;
	fastop(ctxt, em_cmp);

	if (ctxt->eflags & X86_EFLAGS_ZF) {
		/* Success: write back to memory; no update of EAX */
		ctxt->src.type = OP_NONE;
		ctxt->dst.val = ctxt->src.orig_val;
	} else {
		/* Failure: write the value we saw to EAX. */
		ctxt->src.type = OP_REG;
		ctxt->src.addr.reg = reg_rmw(ctxt, VCPU_REGS_RAX);
		ctxt->src.val = ctxt->dst.orig_val;
		/* Create write-cycle to dest by writing the same value */
		ctxt->dst.val = ctxt->dst.orig_val;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	}
	return X86EMUL_CONTINUE;
}

static int em_lseg(struct x86_emulate_ctxt *ctxt)
{
	int seg = ctxt->src2.val;
	unsigned short sel;
	int rc;

	memcpy(&sel, ctxt->src.valptr + ctxt->op_bytes, 2);

	rc = load_segment_descriptor(ctxt, sel, seg);
	if (rc != X86EMUL_CONTINUE)
		return rc;

	ctxt->dst.val = ctxt->src.val;
	return rc;
}

<<<<<<< HEAD
static void
setup_syscalls_segments(struct x86_emulate_ctxt *ctxt,
			struct desc_struct *cs, struct desc_struct *ss)
{
	u16 selector;

	memset(cs, 0, sizeof(struct desc_struct));
	ctxt->ops->get_segment(ctxt, &selector, cs, NULL, VCPU_SREG_CS);
	memset(ss, 0, sizeof(struct desc_struct));

=======
static int em_rsm(struct x86_emulate_ctxt *ctxt)
{
	if (!ctxt->ops->is_smm(ctxt))
		return emulate_ud(ctxt);

	if (ctxt->ops->leave_smm(ctxt))
		ctxt->ops->triple_fault(ctxt);

	return emulator_recalc_and_set_mode(ctxt);
}

static void
setup_syscalls_segments(struct desc_struct *cs, struct desc_struct *ss)
{
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	cs->l = 0;		/* will be adjusted later */
	set_desc_base(cs, 0);	/* flat segment */
	cs->g = 1;		/* 4kb granularity */
	set_desc_limit(cs, 0xfffff);	/* 4GB limit */
	cs->type = 0x0b;	/* Read, Execute, Accessed */
	cs->s = 1;
	cs->dpl = 0;		/* will be adjusted later */
	cs->p = 1;
	cs->d = 1;
<<<<<<< HEAD
=======
	cs->avl = 0;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	set_desc_base(ss, 0);	/* flat segment */
	set_desc_limit(ss, 0xfffff);	/* 4GB limit */
	ss->g = 1;		/* 4kb granularity */
	ss->s = 1;
	ss->type = 0x03;	/* Read/Write, Accessed */
	ss->d = 1;		/* 32bit stack segment */
	ss->dpl = 0;
	ss->p = 1;
<<<<<<< HEAD
=======
	ss->l = 0;
	ss->avl = 0;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

static bool vendor_intel(struct x86_emulate_ctxt *ctxt)
{
	u32 eax, ebx, ecx, edx;

	eax = ecx = 0;
<<<<<<< HEAD
	return ctxt->ops->get_cpuid(ctxt, &eax, &ebx, &ecx, &edx)
		&& ebx == X86EMUL_CPUID_VENDOR_GenuineIntel_ebx
		&& ecx == X86EMUL_CPUID_VENDOR_GenuineIntel_ecx
		&& edx == X86EMUL_CPUID_VENDOR_GenuineIntel_edx;
=======
	ctxt->ops->get_cpuid(ctxt, &eax, &ebx, &ecx, &edx, true);
	return is_guest_vendor_intel(ebx, ecx, edx);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

static bool em_syscall_is_enabled(struct x86_emulate_ctxt *ctxt)
{
<<<<<<< HEAD
	struct x86_emulate_ops *ops = ctxt->ops;
=======
	const struct x86_emulate_ops *ops = ctxt->ops;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	u32 eax, ebx, ecx, edx;

	/*
	 * syscall should always be enabled in longmode - so only become
	 * vendor specific (cpuid) if other modes are active...
	 */
	if (ctxt->mode == X86EMUL_MODE_PROT64)
		return true;

	eax = 0x00000000;
	ecx = 0x00000000;
<<<<<<< HEAD
	if (ops->get_cpuid(ctxt, &eax, &ebx, &ecx, &edx)) {
		/*
		 * Intel ("GenuineIntel")
		 * remark: Intel CPUs only support "syscall" in 64bit
		 * longmode. Also an 64bit guest with a
		 * 32bit compat-app running will #UD !! While this
		 * behaviour can be fixed (by emulating) into AMD
		 * response - CPUs of AMD can't behave like Intel.
		 */
		if (ebx == X86EMUL_CPUID_VENDOR_GenuineIntel_ebx &&
		    ecx == X86EMUL_CPUID_VENDOR_GenuineIntel_ecx &&
		    edx == X86EMUL_CPUID_VENDOR_GenuineIntel_edx)
			return false;

		/* AMD ("AuthenticAMD") */
		if (ebx == X86EMUL_CPUID_VENDOR_AuthenticAMD_ebx &&
		    ecx == X86EMUL_CPUID_VENDOR_AuthenticAMD_ecx &&
		    edx == X86EMUL_CPUID_VENDOR_AuthenticAMD_edx)
			return true;

		/* AMD ("AMDisbetter!") */
		if (ebx == X86EMUL_CPUID_VENDOR_AMDisbetterI_ebx &&
		    ecx == X86EMUL_CPUID_VENDOR_AMDisbetterI_ecx &&
		    edx == X86EMUL_CPUID_VENDOR_AMDisbetterI_edx)
			return true;
	}

	/* default: (not Intel, not AMD), apply Intel's stricter rules... */
=======
	ops->get_cpuid(ctxt, &eax, &ebx, &ecx, &edx, true);
	/*
	 * remark: Intel CPUs only support "syscall" in 64bit longmode. Also a
	 * 64bit guest with a 32bit compat-app running will #UD !! While this
	 * behaviour can be fixed (by emulating) into AMD response - CPUs of
	 * AMD can't behave like Intel.
	 */
	if (is_guest_vendor_intel(ebx, ecx, edx))
		return false;

	if (is_guest_vendor_amd(ebx, ecx, edx) ||
	    is_guest_vendor_hygon(ebx, ecx, edx))
		return true;

	/*
	 * default: (not Intel, not AMD, not Hygon), apply Intel's
	 * stricter rules...
	 */
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	return false;
}

static int em_syscall(struct x86_emulate_ctxt *ctxt)
{
<<<<<<< HEAD
	struct x86_emulate_ops *ops = ctxt->ops;
=======
	const struct x86_emulate_ops *ops = ctxt->ops;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	struct desc_struct cs, ss;
	u64 msr_data;
	u16 cs_sel, ss_sel;
	u64 efer = 0;

	/* syscall is not available in real mode */
	if (ctxt->mode == X86EMUL_MODE_REAL ||
	    ctxt->mode == X86EMUL_MODE_VM86)
		return emulate_ud(ctxt);

	if (!(em_syscall_is_enabled(ctxt)))
		return emulate_ud(ctxt);

	ops->get_msr(ctxt, MSR_EFER, &efer);
<<<<<<< HEAD
	setup_syscalls_segments(ctxt, &cs, &ss);

	if (!(efer & EFER_SCE))
		return emulate_ud(ctxt);

=======
	if (!(efer & EFER_SCE))
		return emulate_ud(ctxt);

	setup_syscalls_segments(&cs, &ss);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	ops->get_msr(ctxt, MSR_STAR, &msr_data);
	msr_data >>= 32;
	cs_sel = (u16)(msr_data & 0xfffc);
	ss_sel = (u16)(msr_data + 8);

	if (efer & EFER_LMA) {
		cs.d = 0;
		cs.l = 1;
	}
	ops->set_segment(ctxt, cs_sel, &cs, 0, VCPU_SREG_CS);
	ops->set_segment(ctxt, ss_sel, &ss, 0, VCPU_SREG_SS);

<<<<<<< HEAD
	ctxt->regs[VCPU_REGS_RCX] = ctxt->_eip;
	if (efer & EFER_LMA) {
#ifdef CONFIG_X86_64
		ctxt->regs[VCPU_REGS_R11] = ctxt->eflags & ~EFLG_RF;
=======
	*reg_write(ctxt, VCPU_REGS_RCX) = ctxt->_eip;
	if (efer & EFER_LMA) {
#ifdef CONFIG_X86_64
		*reg_write(ctxt, VCPU_REGS_R11) = ctxt->eflags;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

		ops->get_msr(ctxt,
			     ctxt->mode == X86EMUL_MODE_PROT64 ?
			     MSR_LSTAR : MSR_CSTAR, &msr_data);
		ctxt->_eip = msr_data;

		ops->get_msr(ctxt, MSR_SYSCALL_MASK, &msr_data);
<<<<<<< HEAD
		ctxt->eflags &= ~(msr_data | EFLG_RF);
=======
		ctxt->eflags &= ~msr_data;
		ctxt->eflags |= X86_EFLAGS_FIXED;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#endif
	} else {
		/* legacy mode */
		ops->get_msr(ctxt, MSR_STAR, &msr_data);
		ctxt->_eip = (u32)msr_data;

<<<<<<< HEAD
		ctxt->eflags &= ~(EFLG_VM | EFLG_IF | EFLG_RF);
	}

=======
		ctxt->eflags &= ~(X86_EFLAGS_VM | X86_EFLAGS_IF);
	}

	ctxt->tf = (ctxt->eflags & X86_EFLAGS_TF) != 0;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	return X86EMUL_CONTINUE;
}

static int em_sysenter(struct x86_emulate_ctxt *ctxt)
{
<<<<<<< HEAD
	struct x86_emulate_ops *ops = ctxt->ops;
=======
	const struct x86_emulate_ops *ops = ctxt->ops;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	struct desc_struct cs, ss;
	u64 msr_data;
	u16 cs_sel, ss_sel;
	u64 efer = 0;

	ops->get_msr(ctxt, MSR_EFER, &efer);
	/* inject #GP if in real mode */
	if (ctxt->mode == X86EMUL_MODE_REAL)
		return emulate_gp(ctxt, 0);

	/*
	 * Not recognized on AMD in compat mode (but is recognized in legacy
	 * mode).
	 */
<<<<<<< HEAD
	if ((ctxt->mode == X86EMUL_MODE_PROT32) && (efer & EFER_LMA)
	    && !vendor_intel(ctxt))
		return emulate_ud(ctxt);

	/* XXX sysenter/sysexit have not been tested in 64bit mode.
	* Therefore, we inject an #UD.
	*/
	if (ctxt->mode == X86EMUL_MODE_PROT64)
		return emulate_ud(ctxt);

	setup_syscalls_segments(ctxt, &cs, &ss);

	ops->get_msr(ctxt, MSR_IA32_SYSENTER_CS, &msr_data);
	switch (ctxt->mode) {
	case X86EMUL_MODE_PROT32:
		if ((msr_data & 0xfffc) == 0x0)
			return emulate_gp(ctxt, 0);
		break;
	case X86EMUL_MODE_PROT64:
		if (msr_data == 0x0)
			return emulate_gp(ctxt, 0);
		break;
	}

	ctxt->eflags &= ~(EFLG_VM | EFLG_IF | EFLG_RF);
	cs_sel = (u16)msr_data;
	cs_sel &= ~SELECTOR_RPL_MASK;
	ss_sel = cs_sel + 8;
	ss_sel &= ~SELECTOR_RPL_MASK;
	if (ctxt->mode == X86EMUL_MODE_PROT64 || (efer & EFER_LMA)) {
=======
	if ((ctxt->mode != X86EMUL_MODE_PROT64) && (efer & EFER_LMA)
	    && !vendor_intel(ctxt))
		return emulate_ud(ctxt);

	/* sysenter/sysexit have not been tested in 64bit mode. */
	if (ctxt->mode == X86EMUL_MODE_PROT64)
		return X86EMUL_UNHANDLEABLE;

	ops->get_msr(ctxt, MSR_IA32_SYSENTER_CS, &msr_data);
	if ((msr_data & 0xfffc) == 0x0)
		return emulate_gp(ctxt, 0);

	setup_syscalls_segments(&cs, &ss);
	ctxt->eflags &= ~(X86_EFLAGS_VM | X86_EFLAGS_IF);
	cs_sel = (u16)msr_data & ~SEGMENT_RPL_MASK;
	ss_sel = cs_sel + 8;
	if (efer & EFER_LMA) {
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		cs.d = 0;
		cs.l = 1;
	}

	ops->set_segment(ctxt, cs_sel, &cs, 0, VCPU_SREG_CS);
	ops->set_segment(ctxt, ss_sel, &ss, 0, VCPU_SREG_SS);

	ops->get_msr(ctxt, MSR_IA32_SYSENTER_EIP, &msr_data);
<<<<<<< HEAD
	ctxt->_eip = msr_data;

	ops->get_msr(ctxt, MSR_IA32_SYSENTER_ESP, &msr_data);
	ctxt->regs[VCPU_REGS_RSP] = msr_data;
=======
	ctxt->_eip = (efer & EFER_LMA) ? msr_data : (u32)msr_data;

	ops->get_msr(ctxt, MSR_IA32_SYSENTER_ESP, &msr_data);
	*reg_write(ctxt, VCPU_REGS_RSP) = (efer & EFER_LMA) ? msr_data :
							      (u32)msr_data;
	if (efer & EFER_LMA)
		ctxt->mode = X86EMUL_MODE_PROT64;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	return X86EMUL_CONTINUE;
}

static int em_sysexit(struct x86_emulate_ctxt *ctxt)
{
<<<<<<< HEAD
	struct x86_emulate_ops *ops = ctxt->ops;
=======
	const struct x86_emulate_ops *ops = ctxt->ops;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	struct desc_struct cs, ss;
	u64 msr_data, rcx, rdx;
	int usermode;
	u16 cs_sel = 0, ss_sel = 0;

	/* inject #GP if in real mode or Virtual 8086 mode */
	if (ctxt->mode == X86EMUL_MODE_REAL ||
	    ctxt->mode == X86EMUL_MODE_VM86)
		return emulate_gp(ctxt, 0);

<<<<<<< HEAD
	setup_syscalls_segments(ctxt, &cs, &ss);
=======
	setup_syscalls_segments(&cs, &ss);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	if ((ctxt->rex_prefix & 0x8) != 0x0)
		usermode = X86EMUL_MODE_PROT64;
	else
		usermode = X86EMUL_MODE_PROT32;

<<<<<<< HEAD
	rcx = ctxt->regs[VCPU_REGS_RCX];
	rdx = ctxt->regs[VCPU_REGS_RDX];
=======
	rcx = reg_read(ctxt, VCPU_REGS_RCX);
	rdx = reg_read(ctxt, VCPU_REGS_RDX);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	cs.dpl = 3;
	ss.dpl = 3;
	ops->get_msr(ctxt, MSR_IA32_SYSENTER_CS, &msr_data);
	switch (usermode) {
	case X86EMUL_MODE_PROT32:
		cs_sel = (u16)(msr_data + 16);
		if ((msr_data & 0xfffc) == 0x0)
			return emulate_gp(ctxt, 0);
		ss_sel = (u16)(msr_data + 24);
<<<<<<< HEAD
=======
		rcx = (u32)rcx;
		rdx = (u32)rdx;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		break;
	case X86EMUL_MODE_PROT64:
		cs_sel = (u16)(msr_data + 32);
		if (msr_data == 0x0)
			return emulate_gp(ctxt, 0);
		ss_sel = cs_sel + 8;
		cs.d = 0;
		cs.l = 1;
<<<<<<< HEAD
		if (is_noncanonical_address(rcx) ||
		    is_noncanonical_address(rdx))
			return emulate_gp(ctxt, 0);
		break;
	}
	cs_sel |= SELECTOR_RPL_MASK;
	ss_sel |= SELECTOR_RPL_MASK;
=======
		if (emul_is_noncanonical_address(rcx, ctxt) ||
		    emul_is_noncanonical_address(rdx, ctxt))
			return emulate_gp(ctxt, 0);
		break;
	}
	cs_sel |= SEGMENT_RPL_MASK;
	ss_sel |= SEGMENT_RPL_MASK;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	ops->set_segment(ctxt, cs_sel, &cs, 0, VCPU_SREG_CS);
	ops->set_segment(ctxt, ss_sel, &ss, 0, VCPU_SREG_SS);

	ctxt->_eip = rdx;
<<<<<<< HEAD
	ctxt->regs[VCPU_REGS_RSP] = rcx;
=======
	ctxt->mode = usermode;
	*reg_write(ctxt, VCPU_REGS_RSP) = rcx;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	return X86EMUL_CONTINUE;
}

static bool emulator_bad_iopl(struct x86_emulate_ctxt *ctxt)
{
	int iopl;
	if (ctxt->mode == X86EMUL_MODE_REAL)
		return false;
	if (ctxt->mode == X86EMUL_MODE_VM86)
		return true;
<<<<<<< HEAD
	iopl = (ctxt->eflags & X86_EFLAGS_IOPL) >> IOPL_SHIFT;
	return ctxt->ops->cpl(ctxt) > iopl;
}

static bool emulator_io_port_access_allowed(struct x86_emulate_ctxt *ctxt,
					    u16 port, u16 len)
{
	struct x86_emulate_ops *ops = ctxt->ops;
=======
	iopl = (ctxt->eflags & X86_EFLAGS_IOPL) >> X86_EFLAGS_IOPL_BIT;
	return ctxt->ops->cpl(ctxt) > iopl;
}

#define VMWARE_PORT_VMPORT	(0x5658)
#define VMWARE_PORT_VMRPC	(0x5659)

static bool emulator_io_port_access_allowed(struct x86_emulate_ctxt *ctxt,
					    u16 port, u16 len)
{
	const struct x86_emulate_ops *ops = ctxt->ops;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	struct desc_struct tr_seg;
	u32 base3;
	int r;
	u16 tr, io_bitmap_ptr, perm, bit_idx = port & 0x7;
	unsigned mask = (1 << len) - 1;
	unsigned long base;

<<<<<<< HEAD
=======
	/*
	 * VMware allows access to these ports even if denied
	 * by TSS I/O permission bitmap. Mimic behavior.
	 */
	if (enable_vmware_backdoor &&
	    ((port == VMWARE_PORT_VMPORT) || (port == VMWARE_PORT_VMRPC)))
		return true;

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	ops->get_segment(ctxt, &tr, &tr_seg, &base3, VCPU_SREG_TR);
	if (!tr_seg.p)
		return false;
	if (desc_limit_scaled(&tr_seg) < 103)
		return false;
	base = get_desc_base(&tr_seg);
#ifdef CONFIG_X86_64
	base |= ((u64)base3) << 32;
#endif
<<<<<<< HEAD
	r = ops->read_std(ctxt, base + 102, &io_bitmap_ptr, 2, NULL);
=======
	r = ops->read_std(ctxt, base + 102, &io_bitmap_ptr, 2, NULL, true);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	if (r != X86EMUL_CONTINUE)
		return false;
	if (io_bitmap_ptr + port/8 > desc_limit_scaled(&tr_seg))
		return false;
<<<<<<< HEAD
	r = ops->read_std(ctxt, base + io_bitmap_ptr + port/8, &perm, 2, NULL);
=======
	r = ops->read_std(ctxt, base + io_bitmap_ptr + port/8, &perm, 2, NULL, true);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	if (r != X86EMUL_CONTINUE)
		return false;
	if ((perm >> bit_idx) & mask)
		return false;
	return true;
}

<<<<<<< HEAD
static bool emulator_io_permited(struct x86_emulate_ctxt *ctxt,
				 u16 port, u16 len)
=======
static bool emulator_io_permitted(struct x86_emulate_ctxt *ctxt,
				  u16 port, u16 len)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
{
	if (ctxt->perm_ok)
		return true;

	if (emulator_bad_iopl(ctxt))
		if (!emulator_io_port_access_allowed(ctxt, port, len))
			return false;

	ctxt->perm_ok = true;

	return true;
}

<<<<<<< HEAD
=======
static void string_registers_quirk(struct x86_emulate_ctxt *ctxt)
{
	/*
	 * Intel CPUs mask the counter and pointers in quite strange
	 * manner when ECX is zero due to REP-string optimizations.
	 */
#ifdef CONFIG_X86_64
	if (ctxt->ad_bytes != 4 || !vendor_intel(ctxt))
		return;

	*reg_write(ctxt, VCPU_REGS_RCX) = 0;

	switch (ctxt->b) {
	case 0xa4:	/* movsb */
	case 0xa5:	/* movsd/w */
		*reg_rmw(ctxt, VCPU_REGS_RSI) &= (u32)-1;
		fallthrough;
	case 0xaa:	/* stosb */
	case 0xab:	/* stosd/w */
		*reg_rmw(ctxt, VCPU_REGS_RDI) &= (u32)-1;
	}
#endif
}

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
static void save_state_to_tss16(struct x86_emulate_ctxt *ctxt,
				struct tss_segment_16 *tss)
{
	tss->ip = ctxt->_eip;
	tss->flag = ctxt->eflags;
<<<<<<< HEAD
	tss->ax = ctxt->regs[VCPU_REGS_RAX];
	tss->cx = ctxt->regs[VCPU_REGS_RCX];
	tss->dx = ctxt->regs[VCPU_REGS_RDX];
	tss->bx = ctxt->regs[VCPU_REGS_RBX];
	tss->sp = ctxt->regs[VCPU_REGS_RSP];
	tss->bp = ctxt->regs[VCPU_REGS_RBP];
	tss->si = ctxt->regs[VCPU_REGS_RSI];
	tss->di = ctxt->regs[VCPU_REGS_RDI];
=======
	tss->ax = reg_read(ctxt, VCPU_REGS_RAX);
	tss->cx = reg_read(ctxt, VCPU_REGS_RCX);
	tss->dx = reg_read(ctxt, VCPU_REGS_RDX);
	tss->bx = reg_read(ctxt, VCPU_REGS_RBX);
	tss->sp = reg_read(ctxt, VCPU_REGS_RSP);
	tss->bp = reg_read(ctxt, VCPU_REGS_RBP);
	tss->si = reg_read(ctxt, VCPU_REGS_RSI);
	tss->di = reg_read(ctxt, VCPU_REGS_RDI);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	tss->es = get_segment_selector(ctxt, VCPU_SREG_ES);
	tss->cs = get_segment_selector(ctxt, VCPU_SREG_CS);
	tss->ss = get_segment_selector(ctxt, VCPU_SREG_SS);
	tss->ds = get_segment_selector(ctxt, VCPU_SREG_DS);
	tss->ldt = get_segment_selector(ctxt, VCPU_SREG_LDTR);
}

static int load_state_from_tss16(struct x86_emulate_ctxt *ctxt,
				 struct tss_segment_16 *tss)
{
	int ret;
	u8 cpl;

	ctxt->_eip = tss->ip;
	ctxt->eflags = tss->flag | 2;
<<<<<<< HEAD
	ctxt->regs[VCPU_REGS_RAX] = tss->ax;
	ctxt->regs[VCPU_REGS_RCX] = tss->cx;
	ctxt->regs[VCPU_REGS_RDX] = tss->dx;
	ctxt->regs[VCPU_REGS_RBX] = tss->bx;
	ctxt->regs[VCPU_REGS_RSP] = tss->sp;
	ctxt->regs[VCPU_REGS_RBP] = tss->bp;
	ctxt->regs[VCPU_REGS_RSI] = tss->si;
	ctxt->regs[VCPU_REGS_RDI] = tss->di;
=======
	*reg_write(ctxt, VCPU_REGS_RAX) = tss->ax;
	*reg_write(ctxt, VCPU_REGS_RCX) = tss->cx;
	*reg_write(ctxt, VCPU_REGS_RDX) = tss->dx;
	*reg_write(ctxt, VCPU_REGS_RBX) = tss->bx;
	*reg_write(ctxt, VCPU_REGS_RSP) = tss->sp;
	*reg_write(ctxt, VCPU_REGS_RBP) = tss->bp;
	*reg_write(ctxt, VCPU_REGS_RSI) = tss->si;
	*reg_write(ctxt, VCPU_REGS_RDI) = tss->di;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	/*
	 * SDM says that segment selectors are loaded before segment
	 * descriptors
	 */
	set_segment_selector(ctxt, tss->ldt, VCPU_SREG_LDTR);
	set_segment_selector(ctxt, tss->es, VCPU_SREG_ES);
	set_segment_selector(ctxt, tss->cs, VCPU_SREG_CS);
	set_segment_selector(ctxt, tss->ss, VCPU_SREG_SS);
	set_segment_selector(ctxt, tss->ds, VCPU_SREG_DS);

	cpl = tss->cs & 3;

	/*
<<<<<<< HEAD
	 * Now load segment descriptors. If fault happenes at this stage
	 * it is handled in a context of new task
	 */
	ret = __load_segment_descriptor(ctxt, tss->ldt, VCPU_SREG_LDTR, cpl,
					true, NULL);
	if (ret != X86EMUL_CONTINUE)
		return ret;
	ret = __load_segment_descriptor(ctxt, tss->es, VCPU_SREG_ES, cpl,
					true, NULL);
	if (ret != X86EMUL_CONTINUE)
		return ret;
	ret = __load_segment_descriptor(ctxt, tss->cs, VCPU_SREG_CS, cpl,
					true, NULL);
	if (ret != X86EMUL_CONTINUE)
		return ret;
	ret = __load_segment_descriptor(ctxt, tss->ss, VCPU_SREG_SS, cpl,
					true, NULL);
	if (ret != X86EMUL_CONTINUE)
		return ret;
	ret = __load_segment_descriptor(ctxt, tss->ds, VCPU_SREG_DS, cpl,
					true, NULL);
=======
	 * Now load segment descriptors. If fault happens at this stage
	 * it is handled in a context of new task
	 */
	ret = __load_segment_descriptor(ctxt, tss->ldt, VCPU_SREG_LDTR, cpl,
					X86_TRANSFER_TASK_SWITCH, NULL);
	if (ret != X86EMUL_CONTINUE)
		return ret;
	ret = __load_segment_descriptor(ctxt, tss->es, VCPU_SREG_ES, cpl,
					X86_TRANSFER_TASK_SWITCH, NULL);
	if (ret != X86EMUL_CONTINUE)
		return ret;
	ret = __load_segment_descriptor(ctxt, tss->cs, VCPU_SREG_CS, cpl,
					X86_TRANSFER_TASK_SWITCH, NULL);
	if (ret != X86EMUL_CONTINUE)
		return ret;
	ret = __load_segment_descriptor(ctxt, tss->ss, VCPU_SREG_SS, cpl,
					X86_TRANSFER_TASK_SWITCH, NULL);
	if (ret != X86EMUL_CONTINUE)
		return ret;
	ret = __load_segment_descriptor(ctxt, tss->ds, VCPU_SREG_DS, cpl,
					X86_TRANSFER_TASK_SWITCH, NULL);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	if (ret != X86EMUL_CONTINUE)
		return ret;

	return X86EMUL_CONTINUE;
}

<<<<<<< HEAD
static int task_switch_16(struct x86_emulate_ctxt *ctxt,
			  u16 tss_selector, u16 old_tss_sel,
			  ulong old_tss_base, struct desc_struct *new_desc)
{
	struct x86_emulate_ops *ops = ctxt->ops;
=======
static int task_switch_16(struct x86_emulate_ctxt *ctxt, u16 old_tss_sel,
			  ulong old_tss_base, struct desc_struct *new_desc)
{
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	struct tss_segment_16 tss_seg;
	int ret;
	u32 new_tss_base = get_desc_base(new_desc);

<<<<<<< HEAD
	ret = ops->read_std(ctxt, old_tss_base, &tss_seg, sizeof tss_seg,
			    &ctxt->exception);
	if (ret != X86EMUL_CONTINUE)
		/* FIXME: need to provide precise fault address */
=======
	ret = linear_read_system(ctxt, old_tss_base, &tss_seg, sizeof(tss_seg));
	if (ret != X86EMUL_CONTINUE)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		return ret;

	save_state_to_tss16(ctxt, &tss_seg);

<<<<<<< HEAD
	ret = ops->write_std(ctxt, old_tss_base, &tss_seg, sizeof tss_seg,
			     &ctxt->exception);
	if (ret != X86EMUL_CONTINUE)
		/* FIXME: need to provide precise fault address */
		return ret;

	ret = ops->read_std(ctxt, new_tss_base, &tss_seg, sizeof tss_seg,
			    &ctxt->exception);
	if (ret != X86EMUL_CONTINUE)
		/* FIXME: need to provide precise fault address */
=======
	ret = linear_write_system(ctxt, old_tss_base, &tss_seg, sizeof(tss_seg));
	if (ret != X86EMUL_CONTINUE)
		return ret;

	ret = linear_read_system(ctxt, new_tss_base, &tss_seg, sizeof(tss_seg));
	if (ret != X86EMUL_CONTINUE)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		return ret;

	if (old_tss_sel != 0xffff) {
		tss_seg.prev_task_link = old_tss_sel;

<<<<<<< HEAD
		ret = ops->write_std(ctxt, new_tss_base,
				     &tss_seg.prev_task_link,
				     sizeof tss_seg.prev_task_link,
				     &ctxt->exception);
		if (ret != X86EMUL_CONTINUE)
			/* FIXME: need to provide precise fault address */
=======
		ret = linear_write_system(ctxt, new_tss_base,
					  &tss_seg.prev_task_link,
					  sizeof(tss_seg.prev_task_link));
		if (ret != X86EMUL_CONTINUE)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
			return ret;
	}

	return load_state_from_tss16(ctxt, &tss_seg);
}

static void save_state_to_tss32(struct x86_emulate_ctxt *ctxt,
				struct tss_segment_32 *tss)
{
<<<<<<< HEAD
	tss->cr3 = ctxt->ops->get_cr(ctxt, 3);
	tss->eip = ctxt->_eip;
	tss->eflags = ctxt->eflags;
	tss->eax = ctxt->regs[VCPU_REGS_RAX];
	tss->ecx = ctxt->regs[VCPU_REGS_RCX];
	tss->edx = ctxt->regs[VCPU_REGS_RDX];
	tss->ebx = ctxt->regs[VCPU_REGS_RBX];
	tss->esp = ctxt->regs[VCPU_REGS_RSP];
	tss->ebp = ctxt->regs[VCPU_REGS_RBP];
	tss->esi = ctxt->regs[VCPU_REGS_RSI];
	tss->edi = ctxt->regs[VCPU_REGS_RDI];
=======
	/* CR3 and ldt selector are not saved intentionally */
	tss->eip = ctxt->_eip;
	tss->eflags = ctxt->eflags;
	tss->eax = reg_read(ctxt, VCPU_REGS_RAX);
	tss->ecx = reg_read(ctxt, VCPU_REGS_RCX);
	tss->edx = reg_read(ctxt, VCPU_REGS_RDX);
	tss->ebx = reg_read(ctxt, VCPU_REGS_RBX);
	tss->esp = reg_read(ctxt, VCPU_REGS_RSP);
	tss->ebp = reg_read(ctxt, VCPU_REGS_RBP);
	tss->esi = reg_read(ctxt, VCPU_REGS_RSI);
	tss->edi = reg_read(ctxt, VCPU_REGS_RDI);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	tss->es = get_segment_selector(ctxt, VCPU_SREG_ES);
	tss->cs = get_segment_selector(ctxt, VCPU_SREG_CS);
	tss->ss = get_segment_selector(ctxt, VCPU_SREG_SS);
	tss->ds = get_segment_selector(ctxt, VCPU_SREG_DS);
	tss->fs = get_segment_selector(ctxt, VCPU_SREG_FS);
	tss->gs = get_segment_selector(ctxt, VCPU_SREG_GS);
<<<<<<< HEAD
	tss->ldt_selector = get_segment_selector(ctxt, VCPU_SREG_LDTR);
=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

static int load_state_from_tss32(struct x86_emulate_ctxt *ctxt,
				 struct tss_segment_32 *tss)
{
	int ret;
	u8 cpl;

	if (ctxt->ops->set_cr(ctxt, 3, tss->cr3))
		return emulate_gp(ctxt, 0);
	ctxt->_eip = tss->eip;
	ctxt->eflags = tss->eflags | 2;

	/* General purpose registers */
<<<<<<< HEAD
	ctxt->regs[VCPU_REGS_RAX] = tss->eax;
	ctxt->regs[VCPU_REGS_RCX] = tss->ecx;
	ctxt->regs[VCPU_REGS_RDX] = tss->edx;
	ctxt->regs[VCPU_REGS_RBX] = tss->ebx;
	ctxt->regs[VCPU_REGS_RSP] = tss->esp;
	ctxt->regs[VCPU_REGS_RBP] = tss->ebp;
	ctxt->regs[VCPU_REGS_RSI] = tss->esi;
	ctxt->regs[VCPU_REGS_RDI] = tss->edi;
=======
	*reg_write(ctxt, VCPU_REGS_RAX) = tss->eax;
	*reg_write(ctxt, VCPU_REGS_RCX) = tss->ecx;
	*reg_write(ctxt, VCPU_REGS_RDX) = tss->edx;
	*reg_write(ctxt, VCPU_REGS_RBX) = tss->ebx;
	*reg_write(ctxt, VCPU_REGS_RSP) = tss->esp;
	*reg_write(ctxt, VCPU_REGS_RBP) = tss->ebp;
	*reg_write(ctxt, VCPU_REGS_RSI) = tss->esi;
	*reg_write(ctxt, VCPU_REGS_RDI) = tss->edi;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	/*
	 * SDM says that segment selectors are loaded before segment
	 * descriptors.  This is important because CPL checks will
	 * use CS.RPL.
	 */
	set_segment_selector(ctxt, tss->ldt_selector, VCPU_SREG_LDTR);
	set_segment_selector(ctxt, tss->es, VCPU_SREG_ES);
	set_segment_selector(ctxt, tss->cs, VCPU_SREG_CS);
	set_segment_selector(ctxt, tss->ss, VCPU_SREG_SS);
	set_segment_selector(ctxt, tss->ds, VCPU_SREG_DS);
	set_segment_selector(ctxt, tss->fs, VCPU_SREG_FS);
	set_segment_selector(ctxt, tss->gs, VCPU_SREG_GS);

	/*
	 * If we're switching between Protected Mode and VM86, we need to make
	 * sure to update the mode before loading the segment descriptors so
	 * that the selectors are interpreted correctly.
	 */
	if (ctxt->eflags & X86_EFLAGS_VM) {
		ctxt->mode = X86EMUL_MODE_VM86;
		cpl = 3;
	} else {
		ctxt->mode = X86EMUL_MODE_PROT32;
		cpl = tss->cs & 3;
	}

	/*
<<<<<<< HEAD
	 * Now load segment descriptors. If fault happenes at this stage
	 * it is handled in a context of new task
	 */
	ret = __load_segment_descriptor(ctxt, tss->ldt_selector, VCPU_SREG_LDTR,
					cpl, true, NULL);
	if (ret != X86EMUL_CONTINUE)
		return ret;
	ret = __load_segment_descriptor(ctxt, tss->es, VCPU_SREG_ES, cpl,
					true, NULL);
	if (ret != X86EMUL_CONTINUE)
		return ret;
	ret = __load_segment_descriptor(ctxt, tss->cs, VCPU_SREG_CS, cpl,
					true, NULL);
	if (ret != X86EMUL_CONTINUE)
		return ret;
	ret = __load_segment_descriptor(ctxt, tss->ss, VCPU_SREG_SS, cpl,
					true, NULL);
	if (ret != X86EMUL_CONTINUE)
		return ret;
	ret = __load_segment_descriptor(ctxt, tss->ds, VCPU_SREG_DS, cpl,
					true, NULL);
	if (ret != X86EMUL_CONTINUE)
		return ret;
	ret = __load_segment_descriptor(ctxt, tss->fs, VCPU_SREG_FS, cpl,
					true, NULL);
	if (ret != X86EMUL_CONTINUE)
		return ret;
	ret = __load_segment_descriptor(ctxt, tss->gs, VCPU_SREG_GS, cpl,
					true, NULL);
	if (ret != X86EMUL_CONTINUE)
		return ret;

	return X86EMUL_CONTINUE;
}

static int task_switch_32(struct x86_emulate_ctxt *ctxt,
			  u16 tss_selector, u16 old_tss_sel,
			  ulong old_tss_base, struct desc_struct *new_desc)
{
	struct x86_emulate_ops *ops = ctxt->ops;
	struct tss_segment_32 tss_seg;
	int ret;
	u32 new_tss_base = get_desc_base(new_desc);

	ret = ops->read_std(ctxt, old_tss_base, &tss_seg, sizeof tss_seg,
			    &ctxt->exception);
	if (ret != X86EMUL_CONTINUE)
		/* FIXME: need to provide precise fault address */
=======
	 * Now load segment descriptors. If fault happens at this stage
	 * it is handled in a context of new task
	 */
	ret = __load_segment_descriptor(ctxt, tss->ldt_selector, VCPU_SREG_LDTR,
					cpl, X86_TRANSFER_TASK_SWITCH, NULL);
	if (ret != X86EMUL_CONTINUE)
		return ret;
	ret = __load_segment_descriptor(ctxt, tss->es, VCPU_SREG_ES, cpl,
					X86_TRANSFER_TASK_SWITCH, NULL);
	if (ret != X86EMUL_CONTINUE)
		return ret;
	ret = __load_segment_descriptor(ctxt, tss->cs, VCPU_SREG_CS, cpl,
					X86_TRANSFER_TASK_SWITCH, NULL);
	if (ret != X86EMUL_CONTINUE)
		return ret;
	ret = __load_segment_descriptor(ctxt, tss->ss, VCPU_SREG_SS, cpl,
					X86_TRANSFER_TASK_SWITCH, NULL);
	if (ret != X86EMUL_CONTINUE)
		return ret;
	ret = __load_segment_descriptor(ctxt, tss->ds, VCPU_SREG_DS, cpl,
					X86_TRANSFER_TASK_SWITCH, NULL);
	if (ret != X86EMUL_CONTINUE)
		return ret;
	ret = __load_segment_descriptor(ctxt, tss->fs, VCPU_SREG_FS, cpl,
					X86_TRANSFER_TASK_SWITCH, NULL);
	if (ret != X86EMUL_CONTINUE)
		return ret;
	ret = __load_segment_descriptor(ctxt, tss->gs, VCPU_SREG_GS, cpl,
					X86_TRANSFER_TASK_SWITCH, NULL);

	return ret;
}

static int task_switch_32(struct x86_emulate_ctxt *ctxt, u16 old_tss_sel,
			  ulong old_tss_base, struct desc_struct *new_desc)
{
	struct tss_segment_32 tss_seg;
	int ret;
	u32 new_tss_base = get_desc_base(new_desc);
	u32 eip_offset = offsetof(struct tss_segment_32, eip);
	u32 ldt_sel_offset = offsetof(struct tss_segment_32, ldt_selector);

	ret = linear_read_system(ctxt, old_tss_base, &tss_seg, sizeof(tss_seg));
	if (ret != X86EMUL_CONTINUE)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		return ret;

	save_state_to_tss32(ctxt, &tss_seg);

<<<<<<< HEAD
	ret = ops->write_std(ctxt, old_tss_base, &tss_seg, sizeof tss_seg,
			     &ctxt->exception);
	if (ret != X86EMUL_CONTINUE)
		/* FIXME: need to provide precise fault address */
		return ret;

	ret = ops->read_std(ctxt, new_tss_base, &tss_seg, sizeof tss_seg,
			    &ctxt->exception);
	if (ret != X86EMUL_CONTINUE)
		/* FIXME: need to provide precise fault address */
=======
	/* Only GP registers and segment selectors are saved */
	ret = linear_write_system(ctxt, old_tss_base + eip_offset, &tss_seg.eip,
				  ldt_sel_offset - eip_offset);
	if (ret != X86EMUL_CONTINUE)
		return ret;

	ret = linear_read_system(ctxt, new_tss_base, &tss_seg, sizeof(tss_seg));
	if (ret != X86EMUL_CONTINUE)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		return ret;

	if (old_tss_sel != 0xffff) {
		tss_seg.prev_task_link = old_tss_sel;

<<<<<<< HEAD
		ret = ops->write_std(ctxt, new_tss_base,
				     &tss_seg.prev_task_link,
				     sizeof tss_seg.prev_task_link,
				     &ctxt->exception);
		if (ret != X86EMUL_CONTINUE)
			/* FIXME: need to provide precise fault address */
=======
		ret = linear_write_system(ctxt, new_tss_base,
					  &tss_seg.prev_task_link,
					  sizeof(tss_seg.prev_task_link));
		if (ret != X86EMUL_CONTINUE)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
			return ret;
	}

	return load_state_from_tss32(ctxt, &tss_seg);
}

static int emulator_do_task_switch(struct x86_emulate_ctxt *ctxt,
				   u16 tss_selector, int idt_index, int reason,
				   bool has_error_code, u32 error_code)
{
<<<<<<< HEAD
	struct x86_emulate_ops *ops = ctxt->ops;
=======
	const struct x86_emulate_ops *ops = ctxt->ops;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	struct desc_struct curr_tss_desc, next_tss_desc;
	int ret;
	u16 old_tss_sel = get_segment_selector(ctxt, VCPU_SREG_TR);
	ulong old_tss_base =
		ops->get_cached_segment_base(ctxt, VCPU_SREG_TR);
	u32 desc_limit;
<<<<<<< HEAD

	/* FIXME: old_tss_base == ~0 ? */

	ret = read_segment_descriptor(ctxt, tss_selector, &next_tss_desc);
	if (ret != X86EMUL_CONTINUE)
		return ret;
	ret = read_segment_descriptor(ctxt, old_tss_sel, &curr_tss_desc);
=======
	ulong desc_addr, dr7;

	/* FIXME: old_tss_base == ~0 ? */

	ret = read_segment_descriptor(ctxt, tss_selector, &next_tss_desc, &desc_addr);
	if (ret != X86EMUL_CONTINUE)
		return ret;
	ret = read_segment_descriptor(ctxt, old_tss_sel, &curr_tss_desc, &desc_addr);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	if (ret != X86EMUL_CONTINUE)
		return ret;

	/* FIXME: check that next_tss_desc is tss */

	/*
	 * Check privileges. The three cases are task switch caused by...
	 *
	 * 1. jmp/call/int to task gate: Check against DPL of the task gate
	 * 2. Exception/IRQ/iret: No check is performed
<<<<<<< HEAD
	 * 3. jmp/call to TSS: Check agains DPL of the TSS
=======
	 * 3. jmp/call to TSS/task-gate: No check is performed since the
	 *    hardware checks it before exiting.
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	 */
	if (reason == TASK_SWITCH_GATE) {
		if (idt_index != -1) {
			/* Software interrupts */
			struct desc_struct task_gate_desc;
			int dpl;

			ret = read_interrupt_descriptor(ctxt, idt_index,
							&task_gate_desc);
			if (ret != X86EMUL_CONTINUE)
				return ret;

			dpl = task_gate_desc.dpl;
			if ((tss_selector & 3) > dpl || ops->cpl(ctxt) > dpl)
				return emulate_gp(ctxt, (idt_index << 3) | 0x2);
		}
<<<<<<< HEAD
	} else if (reason != TASK_SWITCH_IRET) {
		int dpl = next_tss_desc.dpl;
		if ((tss_selector & 3) > dpl || ops->cpl(ctxt) > dpl)
			return emulate_gp(ctxt, tss_selector);
	}


=======
	}

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	desc_limit = desc_limit_scaled(&next_tss_desc);
	if (!next_tss_desc.p ||
	    ((desc_limit < 0x67 && (next_tss_desc.type & 8)) ||
	     desc_limit < 0x2b)) {
<<<<<<< HEAD
		emulate_ts(ctxt, tss_selector & 0xfffc);
		return X86EMUL_PROPAGATE_FAULT;
=======
		return emulate_ts(ctxt, tss_selector & 0xfffc);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	}

	if (reason == TASK_SWITCH_IRET || reason == TASK_SWITCH_JMP) {
		curr_tss_desc.type &= ~(1 << 1); /* clear busy flag */
		write_segment_descriptor(ctxt, old_tss_sel, &curr_tss_desc);
	}

	if (reason == TASK_SWITCH_IRET)
		ctxt->eflags = ctxt->eflags & ~X86_EFLAGS_NT;

	/* set back link to prev task only if NT bit is set in eflags
<<<<<<< HEAD
	   note that old_tss_sel is not used afetr this point */
=======
	   note that old_tss_sel is not used after this point */
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	if (reason != TASK_SWITCH_CALL && reason != TASK_SWITCH_GATE)
		old_tss_sel = 0xffff;

	if (next_tss_desc.type & 8)
<<<<<<< HEAD
		ret = task_switch_32(ctxt, tss_selector, old_tss_sel,
				     old_tss_base, &next_tss_desc);
	else
		ret = task_switch_16(ctxt, tss_selector, old_tss_sel,
=======
		ret = task_switch_32(ctxt, old_tss_sel, old_tss_base, &next_tss_desc);
	else
		ret = task_switch_16(ctxt, old_tss_sel,
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
				     old_tss_base, &next_tss_desc);
	if (ret != X86EMUL_CONTINUE)
		return ret;

	if (reason == TASK_SWITCH_CALL || reason == TASK_SWITCH_GATE)
		ctxt->eflags = ctxt->eflags | X86_EFLAGS_NT;

	if (reason != TASK_SWITCH_IRET) {
		next_tss_desc.type |= (1 << 1); /* set busy flag */
		write_segment_descriptor(ctxt, tss_selector, &next_tss_desc);
	}

	ops->set_cr(ctxt, 0,  ops->get_cr(ctxt, 0) | X86_CR0_TS);
	ops->set_segment(ctxt, tss_selector, &next_tss_desc, 0, VCPU_SREG_TR);

	if (has_error_code) {
		ctxt->op_bytes = ctxt->ad_bytes = (next_tss_desc.type & 8) ? 4 : 2;
		ctxt->lock_prefix = 0;
		ctxt->src.val = (unsigned long) error_code;
		ret = em_push(ctxt);
	}

<<<<<<< HEAD
=======
	dr7 = ops->get_dr(ctxt, 7);
	ops->set_dr(ctxt, 7, dr7 & ~(DR_LOCAL_ENABLE_MASK | DR_LOCAL_SLOWDOWN));

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	return ret;
}

int emulator_task_switch(struct x86_emulate_ctxt *ctxt,
			 u16 tss_selector, int idt_index, int reason,
			 bool has_error_code, u32 error_code)
{
	int rc;

<<<<<<< HEAD
=======
	invalidate_registers(ctxt);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	ctxt->_eip = ctxt->eip;
	ctxt->dst.type = OP_NONE;

	rc = emulator_do_task_switch(ctxt, tss_selector, idt_index, reason,
				     has_error_code, error_code);

<<<<<<< HEAD
	if (rc == X86EMUL_CONTINUE)
		ctxt->eip = ctxt->_eip;
=======
	if (rc == X86EMUL_CONTINUE) {
		ctxt->eip = ctxt->_eip;
		writeback_registers(ctxt);
	}
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	return (rc == X86EMUL_UNHANDLEABLE) ? EMULATION_FAILED : EMULATION_OK;
}

<<<<<<< HEAD
static void string_addr_inc(struct x86_emulate_ctxt *ctxt, unsigned seg,
			    int reg, struct operand *op)
{
	int df = (ctxt->eflags & EFLG_DF) ? -1 : 1;

	register_address_increment(ctxt, &ctxt->regs[reg], df * op->bytes);
	op->addr.mem.ea = register_address(ctxt, ctxt->regs[reg]);
	op->addr.mem.seg = seg;
=======
static void string_addr_inc(struct x86_emulate_ctxt *ctxt, int reg,
		struct operand *op)
{
	int df = (ctxt->eflags & X86_EFLAGS_DF) ? -op->count : op->count;

	register_address_increment(ctxt, reg, df * op->bytes);
	op->addr.mem.ea = register_address(ctxt, reg);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

static int em_das(struct x86_emulate_ctxt *ctxt)
{
	u8 al, old_al;
	bool af, cf, old_cf;

	cf = ctxt->eflags & X86_EFLAGS_CF;
	al = ctxt->dst.val;

	old_al = al;
	old_cf = cf;
	cf = false;
	af = ctxt->eflags & X86_EFLAGS_AF;
	if ((al & 0x0f) > 9 || af) {
		al -= 6;
		cf = old_cf | (al >= 250);
		af = true;
	} else {
		af = false;
	}
	if (old_al > 0x99 || old_cf) {
		al -= 0x60;
		cf = true;
	}

	ctxt->dst.val = al;
	/* Set PF, ZF, SF */
	ctxt->src.type = OP_IMM;
	ctxt->src.val = 0;
	ctxt->src.bytes = 1;
<<<<<<< HEAD
	emulate_2op_SrcV(ctxt, "or");
=======
	fastop(ctxt, em_or);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	ctxt->eflags &= ~(X86_EFLAGS_AF | X86_EFLAGS_CF);
	if (cf)
		ctxt->eflags |= X86_EFLAGS_CF;
	if (af)
		ctxt->eflags |= X86_EFLAGS_AF;
	return X86EMUL_CONTINUE;
}

<<<<<<< HEAD
=======
static int em_aam(struct x86_emulate_ctxt *ctxt)
{
	u8 al, ah;

	if (ctxt->src.val == 0)
		return emulate_de(ctxt);

	al = ctxt->dst.val & 0xff;
	ah = al / ctxt->src.val;
	al %= ctxt->src.val;

	ctxt->dst.val = (ctxt->dst.val & 0xffff0000) | al | (ah << 8);

	/* Set PF, ZF, SF */
	ctxt->src.type = OP_IMM;
	ctxt->src.val = 0;
	ctxt->src.bytes = 1;
	fastop(ctxt, em_or);

	return X86EMUL_CONTINUE;
}

static int em_aad(struct x86_emulate_ctxt *ctxt)
{
	u8 al = ctxt->dst.val & 0xff;
	u8 ah = (ctxt->dst.val >> 8) & 0xff;

	al = (al + (ah * ctxt->src.val)) & 0xff;

	ctxt->dst.val = (ctxt->dst.val & 0xffff0000) | al;

	/* Set PF, ZF, SF */
	ctxt->src.type = OP_IMM;
	ctxt->src.val = 0;
	ctxt->src.bytes = 1;
	fastop(ctxt, em_or);

	return X86EMUL_CONTINUE;
}

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
static int em_call(struct x86_emulate_ctxt *ctxt)
{
	int rc;
	long rel = ctxt->src.val;

	ctxt->src.val = (unsigned long)ctxt->_eip;
	rc = jmp_rel(ctxt, rel);
	if (rc != X86EMUL_CONTINUE)
		return rc;
	return em_push(ctxt);
}

static int em_call_far(struct x86_emulate_ctxt *ctxt)
{
	u16 sel, old_cs;
	ulong old_eip;
	int rc;
	struct desc_struct old_desc, new_desc;
	const struct x86_emulate_ops *ops = ctxt->ops;
	int cpl = ctxt->ops->cpl(ctxt);
<<<<<<< HEAD
=======
	enum x86emul_mode prev_mode = ctxt->mode;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	old_eip = ctxt->_eip;
	ops->get_segment(ctxt, &old_cs, &old_desc, NULL, VCPU_SREG_CS);

	memcpy(&sel, ctxt->src.valptr + ctxt->op_bytes, 2);
<<<<<<< HEAD
	rc = __load_segment_descriptor(ctxt, sel, VCPU_SREG_CS, cpl, false,
				       &new_desc);
	if (rc != X86EMUL_CONTINUE)
		return X86EMUL_CONTINUE;

	rc = assign_eip_far(ctxt, ctxt->src.val, new_desc.l);
=======
	rc = __load_segment_descriptor(ctxt, sel, VCPU_SREG_CS, cpl,
				       X86_TRANSFER_CALL_JMP, &new_desc);
	if (rc != X86EMUL_CONTINUE)
		return rc;

	rc = assign_eip_far(ctxt, ctxt->src.val);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	if (rc != X86EMUL_CONTINUE)
		goto fail;

	ctxt->src.val = old_cs;
	rc = em_push(ctxt);
	if (rc != X86EMUL_CONTINUE)
		goto fail;

	ctxt->src.val = old_eip;
	rc = em_push(ctxt);
	/* If we failed, we tainted the memory, but the very least we should
	   restore cs */
<<<<<<< HEAD
	if (rc != X86EMUL_CONTINUE)
		goto fail;
	return rc;
fail:
	ops->set_segment(ctxt, old_cs, &old_desc, 0, VCPU_SREG_CS);
=======
	if (rc != X86EMUL_CONTINUE) {
		pr_warn_once("faulting far call emulation tainted memory\n");
		goto fail;
	}
	return rc;
fail:
	ops->set_segment(ctxt, old_cs, &old_desc, 0, VCPU_SREG_CS);
	ctxt->mode = prev_mode;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	return rc;

}

static int em_ret_near_imm(struct x86_emulate_ctxt *ctxt)
{
	int rc;
<<<<<<< HEAD
	unsigned long eip;
=======
	unsigned long eip = 0;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	rc = emulate_pop(ctxt, &eip, ctxt->op_bytes);
	if (rc != X86EMUL_CONTINUE)
		return rc;
	rc = assign_eip_near(ctxt, eip);
	if (rc != X86EMUL_CONTINUE)
		return rc;
<<<<<<< HEAD
	register_address_increment(ctxt, &ctxt->regs[VCPU_REGS_RSP], ctxt->src.val);
	return X86EMUL_CONTINUE;
}

static int em_add(struct x86_emulate_ctxt *ctxt)
{
	emulate_2op_SrcV(ctxt, "add");
	return X86EMUL_CONTINUE;
}

static int em_or(struct x86_emulate_ctxt *ctxt)
{
	emulate_2op_SrcV(ctxt, "or");
	return X86EMUL_CONTINUE;
}

static int em_adc(struct x86_emulate_ctxt *ctxt)
{
	emulate_2op_SrcV(ctxt, "adc");
	return X86EMUL_CONTINUE;
}

static int em_sbb(struct x86_emulate_ctxt *ctxt)
{
	emulate_2op_SrcV(ctxt, "sbb");
	return X86EMUL_CONTINUE;
}

static int em_and(struct x86_emulate_ctxt *ctxt)
{
	emulate_2op_SrcV(ctxt, "and");
	return X86EMUL_CONTINUE;
}

static int em_sub(struct x86_emulate_ctxt *ctxt)
{
	emulate_2op_SrcV(ctxt, "sub");
	return X86EMUL_CONTINUE;
}

static int em_xor(struct x86_emulate_ctxt *ctxt)
{
	emulate_2op_SrcV(ctxt, "xor");
	return X86EMUL_CONTINUE;
}

static int em_cmp(struct x86_emulate_ctxt *ctxt)
{
	emulate_2op_SrcV(ctxt, "cmp");
	/* Disable writeback. */
	ctxt->dst.type = OP_NONE;
	return X86EMUL_CONTINUE;
}

static int em_test(struct x86_emulate_ctxt *ctxt)
{
	emulate_2op_SrcV(ctxt, "test");
	/* Disable writeback. */
	ctxt->dst.type = OP_NONE;
=======
	rsp_increment(ctxt, ctxt->src.val);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	return X86EMUL_CONTINUE;
}

static int em_xchg(struct x86_emulate_ctxt *ctxt)
{
	/* Write back the register source. */
	ctxt->src.val = ctxt->dst.val;
	write_register_operand(&ctxt->src);

	/* Write back the memory destination with implicit LOCK prefix. */
	ctxt->dst.val = ctxt->src.orig_val;
	ctxt->lock_prefix = 1;
	return X86EMUL_CONTINUE;
}

<<<<<<< HEAD
static int em_imul(struct x86_emulate_ctxt *ctxt)
{
	emulate_2op_SrcV_nobyte(ctxt, "imul");
	return X86EMUL_CONTINUE;
}

static int em_imul_3op(struct x86_emulate_ctxt *ctxt)
{
	ctxt->dst.val = ctxt->src2.val;
	return em_imul(ctxt);
=======
static int em_imul_3op(struct x86_emulate_ctxt *ctxt)
{
	ctxt->dst.val = ctxt->src2.val;
	return fastop(ctxt, em_imul);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

static int em_cwd(struct x86_emulate_ctxt *ctxt)
{
	ctxt->dst.type = OP_REG;
	ctxt->dst.bytes = ctxt->src.bytes;
<<<<<<< HEAD
	ctxt->dst.addr.reg = &ctxt->regs[VCPU_REGS_RDX];
=======
	ctxt->dst.addr.reg = reg_rmw(ctxt, VCPU_REGS_RDX);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	ctxt->dst.val = ~((ctxt->src.val >> (ctxt->src.bytes * 8 - 1)) - 1);

	return X86EMUL_CONTINUE;
}

<<<<<<< HEAD
=======
static int em_rdpid(struct x86_emulate_ctxt *ctxt)
{
	u64 tsc_aux = 0;

	if (!ctxt->ops->guest_has_rdpid(ctxt))
		return emulate_ud(ctxt);

	ctxt->ops->get_msr(ctxt, MSR_TSC_AUX, &tsc_aux);
	ctxt->dst.val = tsc_aux;
	return X86EMUL_CONTINUE;
}

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
static int em_rdtsc(struct x86_emulate_ctxt *ctxt)
{
	u64 tsc = 0;

	ctxt->ops->get_msr(ctxt, MSR_IA32_TSC, &tsc);
<<<<<<< HEAD
	ctxt->regs[VCPU_REGS_RAX] = (u32)tsc;
	ctxt->regs[VCPU_REGS_RDX] = tsc >> 32;
=======
	*reg_write(ctxt, VCPU_REGS_RAX) = (u32)tsc;
	*reg_write(ctxt, VCPU_REGS_RDX) = tsc >> 32;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	return X86EMUL_CONTINUE;
}

static int em_rdpmc(struct x86_emulate_ctxt *ctxt)
{
	u64 pmc;

<<<<<<< HEAD
	if (ctxt->ops->read_pmc(ctxt, ctxt->regs[VCPU_REGS_RCX], &pmc))
		return emulate_gp(ctxt, 0);
	ctxt->regs[VCPU_REGS_RAX] = (u32)pmc;
	ctxt->regs[VCPU_REGS_RDX] = pmc >> 32;
=======
	if (ctxt->ops->read_pmc(ctxt, reg_read(ctxt, VCPU_REGS_RCX), &pmc))
		return emulate_gp(ctxt, 0);
	*reg_write(ctxt, VCPU_REGS_RAX) = (u32)pmc;
	*reg_write(ctxt, VCPU_REGS_RDX) = pmc >> 32;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	return X86EMUL_CONTINUE;
}

static int em_mov(struct x86_emulate_ctxt *ctxt)
{
<<<<<<< HEAD
	ctxt->dst.val = ctxt->src.val;
=======
	memcpy(ctxt->dst.valptr, ctxt->src.valptr, sizeof(ctxt->src.valptr));
	return X86EMUL_CONTINUE;
}

static int em_movbe(struct x86_emulate_ctxt *ctxt)
{
	u16 tmp;

	if (!ctxt->ops->guest_has_movbe(ctxt))
		return emulate_ud(ctxt);

	switch (ctxt->op_bytes) {
	case 2:
		/*
		 * From MOVBE definition: "...When the operand size is 16 bits,
		 * the upper word of the destination register remains unchanged
		 * ..."
		 *
		 * Both casting ->valptr and ->val to u16 breaks strict aliasing
		 * rules so we have to do the operation almost per hand.
		 */
		tmp = (u16)ctxt->src.val;
		ctxt->dst.val &= ~0xffffUL;
		ctxt->dst.val |= (unsigned long)swab16(tmp);
		break;
	case 4:
		ctxt->dst.val = swab32((u32)ctxt->src.val);
		break;
	case 8:
		ctxt->dst.val = swab64(ctxt->src.val);
		break;
	default:
		BUG();
	}
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	return X86EMUL_CONTINUE;
}

static int em_cr_write(struct x86_emulate_ctxt *ctxt)
{
<<<<<<< HEAD
	if (ctxt->ops->set_cr(ctxt, ctxt->modrm_reg, ctxt->src.val))
=======
	int cr_num = ctxt->modrm_reg;
	int r;

	if (ctxt->ops->set_cr(ctxt, cr_num, ctxt->src.val))
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		return emulate_gp(ctxt, 0);

	/* Disable writeback. */
	ctxt->dst.type = OP_NONE;
<<<<<<< HEAD
=======

	if (cr_num == 0) {
		/*
		 * CR0 write might have updated CR0.PE and/or CR0.PG
		 * which can affect the cpu's execution mode.
		 */
		r = emulator_recalc_and_set_mode(ctxt);
		if (r != X86EMUL_CONTINUE)
			return r;
	}

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	return X86EMUL_CONTINUE;
}

static int em_dr_write(struct x86_emulate_ctxt *ctxt)
{
	unsigned long val;

	if (ctxt->mode == X86EMUL_MODE_PROT64)
		val = ctxt->src.val & ~0ULL;
	else
		val = ctxt->src.val & ~0U;

	/* #UD condition is already handled. */
	if (ctxt->ops->set_dr(ctxt, ctxt->modrm_reg, val) < 0)
		return emulate_gp(ctxt, 0);

	/* Disable writeback. */
	ctxt->dst.type = OP_NONE;
	return X86EMUL_CONTINUE;
}

static int em_wrmsr(struct x86_emulate_ctxt *ctxt)
{
<<<<<<< HEAD
	u64 msr_data;

	msr_data = (u32)ctxt->regs[VCPU_REGS_RAX]
		| ((u64)ctxt->regs[VCPU_REGS_RDX] << 32);
	if (ctxt->ops->set_msr(ctxt, ctxt->regs[VCPU_REGS_RCX], msr_data))
		return emulate_gp(ctxt, 0);

	return X86EMUL_CONTINUE;
=======
	u64 msr_index = reg_read(ctxt, VCPU_REGS_RCX);
	u64 msr_data;
	int r;

	msr_data = (u32)reg_read(ctxt, VCPU_REGS_RAX)
		| ((u64)reg_read(ctxt, VCPU_REGS_RDX) << 32);
	r = ctxt->ops->set_msr_with_filter(ctxt, msr_index, msr_data);

	if (r == X86EMUL_PROPAGATE_FAULT)
		return emulate_gp(ctxt, 0);

	return r;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

static int em_rdmsr(struct x86_emulate_ctxt *ctxt)
{
<<<<<<< HEAD
	u64 msr_data;

	if (ctxt->ops->get_msr(ctxt, ctxt->regs[VCPU_REGS_RCX], &msr_data))
		return emulate_gp(ctxt, 0);

	ctxt->regs[VCPU_REGS_RAX] = (u32)msr_data;
	ctxt->regs[VCPU_REGS_RDX] = msr_data >> 32;
=======
	u64 msr_index = reg_read(ctxt, VCPU_REGS_RCX);
	u64 msr_data;
	int r;

	r = ctxt->ops->get_msr_with_filter(ctxt, msr_index, &msr_data);

	if (r == X86EMUL_PROPAGATE_FAULT)
		return emulate_gp(ctxt, 0);

	if (r == X86EMUL_CONTINUE) {
		*reg_write(ctxt, VCPU_REGS_RAX) = (u32)msr_data;
		*reg_write(ctxt, VCPU_REGS_RDX) = msr_data >> 32;
	}
	return r;
}

static int em_store_sreg(struct x86_emulate_ctxt *ctxt, int segment)
{
	if (segment > VCPU_SREG_GS &&
	    (ctxt->ops->get_cr(ctxt, 4) & X86_CR4_UMIP) &&
	    ctxt->ops->cpl(ctxt) > 0)
		return emulate_gp(ctxt, 0);

	ctxt->dst.val = get_segment_selector(ctxt, segment);
	if (ctxt->dst.bytes == 4 && ctxt->dst.type == OP_MEM)
		ctxt->dst.bytes = 2;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	return X86EMUL_CONTINUE;
}

static int em_mov_rm_sreg(struct x86_emulate_ctxt *ctxt)
{
	if (ctxt->modrm_reg > VCPU_SREG_GS)
		return emulate_ud(ctxt);

<<<<<<< HEAD
	ctxt->dst.val = get_segment_selector(ctxt, ctxt->modrm_reg);
	return X86EMUL_CONTINUE;
=======
	return em_store_sreg(ctxt, ctxt->modrm_reg);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

static int em_mov_sreg_rm(struct x86_emulate_ctxt *ctxt)
{
	u16 sel = ctxt->src.val;

	if (ctxt->modrm_reg == VCPU_SREG_CS || ctxt->modrm_reg > VCPU_SREG_GS)
		return emulate_ud(ctxt);

	if (ctxt->modrm_reg == VCPU_SREG_SS)
		ctxt->interruptibility = KVM_X86_SHADOW_INT_MOV_SS;

	/* Disable writeback. */
	ctxt->dst.type = OP_NONE;
	return load_segment_descriptor(ctxt, sel, ctxt->modrm_reg);
}

<<<<<<< HEAD
static int em_movdqu(struct x86_emulate_ctxt *ctxt)
{
	memcpy(&ctxt->dst.vec_val, &ctxt->src.vec_val, ctxt->op_bytes);
	return X86EMUL_CONTINUE;
=======
static int em_sldt(struct x86_emulate_ctxt *ctxt)
{
	return em_store_sreg(ctxt, VCPU_SREG_LDTR);
}

static int em_lldt(struct x86_emulate_ctxt *ctxt)
{
	u16 sel = ctxt->src.val;

	/* Disable writeback. */
	ctxt->dst.type = OP_NONE;
	return load_segment_descriptor(ctxt, sel, VCPU_SREG_LDTR);
}

static int em_str(struct x86_emulate_ctxt *ctxt)
{
	return em_store_sreg(ctxt, VCPU_SREG_TR);
}

static int em_ltr(struct x86_emulate_ctxt *ctxt)
{
	u16 sel = ctxt->src.val;

	/* Disable writeback. */
	ctxt->dst.type = OP_NONE;
	return load_segment_descriptor(ctxt, sel, VCPU_SREG_TR);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

static int em_invlpg(struct x86_emulate_ctxt *ctxt)
{
	int rc;
	ulong linear;
<<<<<<< HEAD

	rc = linearize(ctxt, ctxt->src.addr.mem, 1, false, &linear);
=======
	unsigned int max_size;

	rc = __linearize(ctxt, ctxt->src.addr.mem, &max_size, 1, ctxt->mode,
			 &linear, X86EMUL_F_INVLPG);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	if (rc == X86EMUL_CONTINUE)
		ctxt->ops->invlpg(ctxt, linear);
	/* Disable writeback. */
	ctxt->dst.type = OP_NONE;
	return X86EMUL_CONTINUE;
}

static int em_clts(struct x86_emulate_ctxt *ctxt)
{
	ulong cr0;

	cr0 = ctxt->ops->get_cr(ctxt, 0);
	cr0 &= ~X86_CR0_TS;
	ctxt->ops->set_cr(ctxt, 0, cr0);
	return X86EMUL_CONTINUE;
}

<<<<<<< HEAD
static int em_vmcall(struct x86_emulate_ctxt *ctxt)
{
	int rc;

	if (ctxt->modrm_mod != 3 || ctxt->modrm_rm != 1)
		return X86EMUL_UNHANDLEABLE;

	rc = ctxt->ops->fix_hypercall(ctxt);
=======
static int em_hypercall(struct x86_emulate_ctxt *ctxt)
{
	int rc = ctxt->ops->fix_hypercall(ctxt);

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	if (rc != X86EMUL_CONTINUE)
		return rc;

	/* Let the processor re-execute the fixed hypercall */
	ctxt->_eip = ctxt->eip;
	/* Disable writeback. */
	ctxt->dst.type = OP_NONE;
	return X86EMUL_CONTINUE;
}

<<<<<<< HEAD
static int em_lgdt(struct x86_emulate_ctxt *ctxt)
=======
static int emulate_store_desc_ptr(struct x86_emulate_ctxt *ctxt,
				  void (*get)(struct x86_emulate_ctxt *ctxt,
					      struct desc_ptr *ptr))
{
	struct desc_ptr desc_ptr;

	if ((ctxt->ops->get_cr(ctxt, 4) & X86_CR4_UMIP) &&
	    ctxt->ops->cpl(ctxt) > 0)
		return emulate_gp(ctxt, 0);

	if (ctxt->mode == X86EMUL_MODE_PROT64)
		ctxt->op_bytes = 8;
	get(ctxt, &desc_ptr);
	if (ctxt->op_bytes == 2) {
		ctxt->op_bytes = 4;
		desc_ptr.address &= 0x00ffffff;
	}
	/* Disable writeback. */
	ctxt->dst.type = OP_NONE;
	return segmented_write_std(ctxt, ctxt->dst.addr.mem,
				   &desc_ptr, 2 + ctxt->op_bytes);
}

static int em_sgdt(struct x86_emulate_ctxt *ctxt)
{
	return emulate_store_desc_ptr(ctxt, ctxt->ops->get_gdt);
}

static int em_sidt(struct x86_emulate_ctxt *ctxt)
{
	return emulate_store_desc_ptr(ctxt, ctxt->ops->get_idt);
}

static int em_lgdt_lidt(struct x86_emulate_ctxt *ctxt, bool lgdt)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
{
	struct desc_ptr desc_ptr;
	int rc;

<<<<<<< HEAD
=======
	if (ctxt->mode == X86EMUL_MODE_PROT64)
		ctxt->op_bytes = 8;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	rc = read_descriptor(ctxt, ctxt->src.addr.mem,
			     &desc_ptr.size, &desc_ptr.address,
			     ctxt->op_bytes);
	if (rc != X86EMUL_CONTINUE)
		return rc;
<<<<<<< HEAD
	ctxt->ops->set_gdt(ctxt, &desc_ptr);
=======
	if (ctxt->mode == X86EMUL_MODE_PROT64 &&
	    emul_is_noncanonical_address(desc_ptr.address, ctxt))
		return emulate_gp(ctxt, 0);
	if (lgdt)
		ctxt->ops->set_gdt(ctxt, &desc_ptr);
	else
		ctxt->ops->set_idt(ctxt, &desc_ptr);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	/* Disable writeback. */
	ctxt->dst.type = OP_NONE;
	return X86EMUL_CONTINUE;
}

<<<<<<< HEAD
static int em_vmmcall(struct x86_emulate_ctxt *ctxt)
{
	int rc;

	rc = ctxt->ops->fix_hypercall(ctxt);

	/* Disable writeback. */
	ctxt->dst.type = OP_NONE;
	return rc;
=======
static int em_lgdt(struct x86_emulate_ctxt *ctxt)
{
	return em_lgdt_lidt(ctxt, true);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

static int em_lidt(struct x86_emulate_ctxt *ctxt)
{
<<<<<<< HEAD
	struct desc_ptr desc_ptr;
	int rc;

	rc = read_descriptor(ctxt, ctxt->src.addr.mem,
			     &desc_ptr.size, &desc_ptr.address,
			     ctxt->op_bytes);
	if (rc != X86EMUL_CONTINUE)
		return rc;
	ctxt->ops->set_idt(ctxt, &desc_ptr);
	/* Disable writeback. */
	ctxt->dst.type = OP_NONE;
	return X86EMUL_CONTINUE;
=======
	return em_lgdt_lidt(ctxt, false);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

static int em_smsw(struct x86_emulate_ctxt *ctxt)
{
<<<<<<< HEAD
	ctxt->dst.bytes = 2;
=======
	if ((ctxt->ops->get_cr(ctxt, 4) & X86_CR4_UMIP) &&
	    ctxt->ops->cpl(ctxt) > 0)
		return emulate_gp(ctxt, 0);

	if (ctxt->dst.type == OP_MEM)
		ctxt->dst.bytes = 2;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	ctxt->dst.val = ctxt->ops->get_cr(ctxt, 0);
	return X86EMUL_CONTINUE;
}

static int em_lmsw(struct x86_emulate_ctxt *ctxt)
{
	ctxt->ops->set_cr(ctxt, 0, (ctxt->ops->get_cr(ctxt, 0) & ~0x0eul)
			  | (ctxt->src.val & 0x0f));
	ctxt->dst.type = OP_NONE;
	return X86EMUL_CONTINUE;
}

static int em_loop(struct x86_emulate_ctxt *ctxt)
{
	int rc = X86EMUL_CONTINUE;

<<<<<<< HEAD
	register_address_increment(ctxt, &ctxt->regs[VCPU_REGS_RCX], -1);
	if ((address_mask(ctxt, ctxt->regs[VCPU_REGS_RCX]) != 0) &&
=======
	register_address_increment(ctxt, VCPU_REGS_RCX, -1);
	if ((address_mask(ctxt, reg_read(ctxt, VCPU_REGS_RCX)) != 0) &&
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	    (ctxt->b == 0xe2 || test_cc(ctxt->b ^ 0x5, ctxt->eflags)))
		rc = jmp_rel(ctxt, ctxt->src.val);

	return rc;
}

static int em_jcxz(struct x86_emulate_ctxt *ctxt)
{
	int rc = X86EMUL_CONTINUE;

<<<<<<< HEAD
	if (address_mask(ctxt, ctxt->regs[VCPU_REGS_RCX]) == 0)
=======
	if (address_mask(ctxt, reg_read(ctxt, VCPU_REGS_RCX)) == 0)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		rc = jmp_rel(ctxt, ctxt->src.val);

	return rc;
}

static int em_in(struct x86_emulate_ctxt *ctxt)
{
	if (!pio_in_emulated(ctxt, ctxt->dst.bytes, ctxt->src.val,
			     &ctxt->dst.val))
		return X86EMUL_IO_NEEDED;

	return X86EMUL_CONTINUE;
}

static int em_out(struct x86_emulate_ctxt *ctxt)
{
	ctxt->ops->pio_out_emulated(ctxt, ctxt->src.bytes, ctxt->dst.val,
				    &ctxt->src.val, 1);
	/* Disable writeback. */
	ctxt->dst.type = OP_NONE;
	return X86EMUL_CONTINUE;
}

static int em_cli(struct x86_emulate_ctxt *ctxt)
{
	if (emulator_bad_iopl(ctxt))
		return emulate_gp(ctxt, 0);

	ctxt->eflags &= ~X86_EFLAGS_IF;
	return X86EMUL_CONTINUE;
}

static int em_sti(struct x86_emulate_ctxt *ctxt)
{
	if (emulator_bad_iopl(ctxt))
		return emulate_gp(ctxt, 0);

	ctxt->interruptibility = KVM_X86_SHADOW_INT_STI;
	ctxt->eflags |= X86_EFLAGS_IF;
	return X86EMUL_CONTINUE;
}

<<<<<<< HEAD
static int em_bt(struct x86_emulate_ctxt *ctxt)
{
	/* Disable writeback. */
	ctxt->dst.type = OP_NONE;
	/* only subword offset */
	ctxt->src.val &= (ctxt->dst.bytes << 3) - 1;

	emulate_2op_SrcV_nobyte(ctxt, "bt");
	return X86EMUL_CONTINUE;
}

static int em_bts(struct x86_emulate_ctxt *ctxt)
{
	emulate_2op_SrcV_nobyte(ctxt, "bts");
	return X86EMUL_CONTINUE;
}

static int em_btr(struct x86_emulate_ctxt *ctxt)
{
	emulate_2op_SrcV_nobyte(ctxt, "btr");
	return X86EMUL_CONTINUE;
}

static int em_btc(struct x86_emulate_ctxt *ctxt)
{
	emulate_2op_SrcV_nobyte(ctxt, "btc");
	return X86EMUL_CONTINUE;
}

static int em_bsf(struct x86_emulate_ctxt *ctxt)
{
	u8 zf;

	__asm__ ("bsf %2, %0; setz %1"
		 : "=r"(ctxt->dst.val), "=q"(zf)
		 : "r"(ctxt->src.val));

	ctxt->eflags &= ~X86_EFLAGS_ZF;
	if (zf) {
		ctxt->eflags |= X86_EFLAGS_ZF;
		/* Disable writeback. */
		ctxt->dst.type = OP_NONE;
=======
static int em_cpuid(struct x86_emulate_ctxt *ctxt)
{
	u32 eax, ebx, ecx, edx;
	u64 msr = 0;

	ctxt->ops->get_msr(ctxt, MSR_MISC_FEATURES_ENABLES, &msr);
	if (msr & MSR_MISC_FEATURES_ENABLES_CPUID_FAULT &&
	    ctxt->ops->cpl(ctxt)) {
		return emulate_gp(ctxt, 0);
	}

	eax = reg_read(ctxt, VCPU_REGS_RAX);
	ecx = reg_read(ctxt, VCPU_REGS_RCX);
	ctxt->ops->get_cpuid(ctxt, &eax, &ebx, &ecx, &edx, false);
	*reg_write(ctxt, VCPU_REGS_RAX) = eax;
	*reg_write(ctxt, VCPU_REGS_RBX) = ebx;
	*reg_write(ctxt, VCPU_REGS_RCX) = ecx;
	*reg_write(ctxt, VCPU_REGS_RDX) = edx;
	return X86EMUL_CONTINUE;
}

static int em_sahf(struct x86_emulate_ctxt *ctxt)
{
	u32 flags;

	flags = X86_EFLAGS_CF | X86_EFLAGS_PF | X86_EFLAGS_AF | X86_EFLAGS_ZF |
		X86_EFLAGS_SF;
	flags &= *reg_rmw(ctxt, VCPU_REGS_RAX) >> 8;

	ctxt->eflags &= ~0xffUL;
	ctxt->eflags |= flags | X86_EFLAGS_FIXED;
	return X86EMUL_CONTINUE;
}

static int em_lahf(struct x86_emulate_ctxt *ctxt)
{
	*reg_rmw(ctxt, VCPU_REGS_RAX) &= ~0xff00UL;
	*reg_rmw(ctxt, VCPU_REGS_RAX) |= (ctxt->eflags & 0xff) << 8;
	return X86EMUL_CONTINUE;
}

static int em_bswap(struct x86_emulate_ctxt *ctxt)
{
	switch (ctxt->op_bytes) {
#ifdef CONFIG_X86_64
	case 8:
		asm("bswap %0" : "+r"(ctxt->dst.val));
		break;
#endif
	default:
		asm("bswap %0" : "+r"(*(u32 *)&ctxt->dst.val));
		break;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	}
	return X86EMUL_CONTINUE;
}

<<<<<<< HEAD
static int em_bsr(struct x86_emulate_ctxt *ctxt)
{
	u8 zf;

	__asm__ ("bsr %2, %0; setz %1"
		 : "=r"(ctxt->dst.val), "=q"(zf)
		 : "r"(ctxt->src.val));

	ctxt->eflags &= ~X86_EFLAGS_ZF;
	if (zf) {
		ctxt->eflags |= X86_EFLAGS_ZF;
		/* Disable writeback. */
		ctxt->dst.type = OP_NONE;
	}
=======
static int em_clflush(struct x86_emulate_ctxt *ctxt)
{
	/* emulating clflush regardless of cpuid */
	return X86EMUL_CONTINUE;
}

static int em_clflushopt(struct x86_emulate_ctxt *ctxt)
{
	/* emulating clflushopt regardless of cpuid */
	return X86EMUL_CONTINUE;
}

static int em_movsxd(struct x86_emulate_ctxt *ctxt)
{
	ctxt->dst.val = (s32) ctxt->src.val;
	return X86EMUL_CONTINUE;
}

static int check_fxsr(struct x86_emulate_ctxt *ctxt)
{
	if (!ctxt->ops->guest_has_fxsr(ctxt))
		return emulate_ud(ctxt);

	if (ctxt->ops->get_cr(ctxt, 0) & (X86_CR0_TS | X86_CR0_EM))
		return emulate_nm(ctxt);

	/*
	 * Don't emulate a case that should never be hit, instead of working
	 * around a lack of fxsave64/fxrstor64 on old compilers.
	 */
	if (ctxt->mode >= X86EMUL_MODE_PROT64)
		return X86EMUL_UNHANDLEABLE;

	return X86EMUL_CONTINUE;
}

/*
 * Hardware doesn't save and restore XMM 0-7 without CR4.OSFXSR, but does save
 * and restore MXCSR.
 */
static size_t __fxstate_size(int nregs)
{
	return offsetof(struct fxregs_state, xmm_space[0]) + nregs * 16;
}

static inline size_t fxstate_size(struct x86_emulate_ctxt *ctxt)
{
	bool cr4_osfxsr;
	if (ctxt->mode == X86EMUL_MODE_PROT64)
		return __fxstate_size(16);

	cr4_osfxsr = ctxt->ops->get_cr(ctxt, 4) & X86_CR4_OSFXSR;
	return __fxstate_size(cr4_osfxsr ? 8 : 0);
}

/*
 * FXSAVE and FXRSTOR have 4 different formats depending on execution mode,
 *  1) 16 bit mode
 *  2) 32 bit mode
 *     - like (1), but FIP and FDP (foo) are only 16 bit.  At least Intel CPUs
 *       preserve whole 32 bit values, though, so (1) and (2) are the same wrt.
 *       save and restore
 *  3) 64-bit mode with REX.W prefix
 *     - like (2), but XMM 8-15 are being saved and restored
 *  4) 64-bit mode without REX.W prefix
 *     - like (3), but FIP and FDP are 64 bit
 *
 * Emulation uses (3) for (1) and (2) and preserves XMM 8-15 to reach the
 * desired result.  (4) is not emulated.
 *
 * Note: Guest and host CPUID.(EAX=07H,ECX=0H):EBX[bit 13] (deprecate FPU CS
 * and FPU DS) should match.
 */
static int em_fxsave(struct x86_emulate_ctxt *ctxt)
{
	struct fxregs_state fx_state;
	int rc;

	rc = check_fxsr(ctxt);
	if (rc != X86EMUL_CONTINUE)
		return rc;

	kvm_fpu_get();

	rc = asm_safe("fxsave %[fx]", , [fx] "+m"(fx_state));

	kvm_fpu_put();

	if (rc != X86EMUL_CONTINUE)
		return rc;

	return segmented_write_std(ctxt, ctxt->memop.addr.mem, &fx_state,
		                   fxstate_size(ctxt));
}

/*
 * FXRSTOR might restore XMM registers not provided by the guest. Fill
 * in the host registers (via FXSAVE) instead, so they won't be modified.
 * (preemption has to stay disabled until FXRSTOR).
 *
 * Use noinline to keep the stack for other functions called by callers small.
 */
static noinline int fxregs_fixup(struct fxregs_state *fx_state,
				 const size_t used_size)
{
	struct fxregs_state fx_tmp;
	int rc;

	rc = asm_safe("fxsave %[fx]", , [fx] "+m"(fx_tmp));
	memcpy((void *)fx_state + used_size, (void *)&fx_tmp + used_size,
	       __fxstate_size(16) - used_size);

	return rc;
}

static int em_fxrstor(struct x86_emulate_ctxt *ctxt)
{
	struct fxregs_state fx_state;
	int rc;
	size_t size;

	rc = check_fxsr(ctxt);
	if (rc != X86EMUL_CONTINUE)
		return rc;

	size = fxstate_size(ctxt);
	rc = segmented_read_std(ctxt, ctxt->memop.addr.mem, &fx_state, size);
	if (rc != X86EMUL_CONTINUE)
		return rc;

	kvm_fpu_get();

	if (size < __fxstate_size(16)) {
		rc = fxregs_fixup(&fx_state, size);
		if (rc != X86EMUL_CONTINUE)
			goto out;
	}

	if (fx_state.mxcsr >> 16) {
		rc = emulate_gp(ctxt, 0);
		goto out;
	}

	if (rc == X86EMUL_CONTINUE)
		rc = asm_safe("fxrstor %[fx]", : [fx] "m"(fx_state));

out:
	kvm_fpu_put();

	return rc;
}

static int em_xsetbv(struct x86_emulate_ctxt *ctxt)
{
	u32 eax, ecx, edx;

	if (!(ctxt->ops->get_cr(ctxt, 4) & X86_CR4_OSXSAVE))
		return emulate_ud(ctxt);

	eax = reg_read(ctxt, VCPU_REGS_RAX);
	edx = reg_read(ctxt, VCPU_REGS_RDX);
	ecx = reg_read(ctxt, VCPU_REGS_RCX);

	if (ctxt->ops->set_xcr(ctxt, ecx, ((u64)edx << 32) | eax))
		return emulate_gp(ctxt, 0);

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	return X86EMUL_CONTINUE;
}

static bool valid_cr(int nr)
{
	switch (nr) {
	case 0:
	case 2 ... 4:
	case 8:
		return true;
	default:
		return false;
	}
}

<<<<<<< HEAD
static int check_cr_read(struct x86_emulate_ctxt *ctxt)
=======
static int check_cr_access(struct x86_emulate_ctxt *ctxt)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
{
	if (!valid_cr(ctxt->modrm_reg))
		return emulate_ud(ctxt);

	return X86EMUL_CONTINUE;
}

<<<<<<< HEAD
static int check_cr_write(struct x86_emulate_ctxt *ctxt)
{
	u64 new_val = ctxt->src.val64;
	int cr = ctxt->modrm_reg;
	u64 efer = 0;

	static u64 cr_reserved_bits[] = {
		0xffffffff00000000ULL,
		0, 0, 0, /* CR3 checked later */
		CR4_RESERVED_BITS,
		0, 0, 0,
		CR8_RESERVED_BITS,
	};

	if (!valid_cr(cr))
		return emulate_ud(ctxt);

	if (new_val & cr_reserved_bits[cr])
		return emulate_gp(ctxt, 0);

	switch (cr) {
	case 0: {
		u64 cr4;
		if (((new_val & X86_CR0_PG) && !(new_val & X86_CR0_PE)) ||
		    ((new_val & X86_CR0_NW) && !(new_val & X86_CR0_CD)))
			return emulate_gp(ctxt, 0);

		cr4 = ctxt->ops->get_cr(ctxt, 4);
		ctxt->ops->get_msr(ctxt, MSR_EFER, &efer);

		if ((new_val & X86_CR0_PG) && (efer & EFER_LME) &&
		    !(cr4 & X86_CR4_PAE))
			return emulate_gp(ctxt, 0);

		break;
		}
	case 3: {
		u64 rsvd = 0;

		ctxt->ops->get_msr(ctxt, MSR_EFER, &efer);
		if (efer & EFER_LMA)
			rsvd = CR3_L_MODE_RESERVED_BITS;
		else if (ctxt->ops->get_cr(ctxt, 4) & X86_CR4_PAE)
			rsvd = CR3_PAE_RESERVED_BITS;
		else if (ctxt->ops->get_cr(ctxt, 0) & X86_CR0_PG)
			rsvd = CR3_NONPAE_RESERVED_BITS;

		if (new_val & rsvd)
			return emulate_gp(ctxt, 0);

		break;
		}
	case 4: {
		ctxt->ops->get_msr(ctxt, MSR_EFER, &efer);

		if ((efer & EFER_LMA) && !(new_val & X86_CR4_PAE))
			return emulate_gp(ctxt, 0);

		break;
		}
	}

	return X86EMUL_CONTINUE;
}

static int check_dr7_gd(struct x86_emulate_ctxt *ctxt)
{
	unsigned long dr7;

	ctxt->ops->get_dr(ctxt, 7, &dr7);

	/* Check if DR7.Global_Enable is set */
	return dr7 & (1 << 13);
}

=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
static int check_dr_read(struct x86_emulate_ctxt *ctxt)
{
	int dr = ctxt->modrm_reg;
	u64 cr4;

	if (dr > 7)
		return emulate_ud(ctxt);

	cr4 = ctxt->ops->get_cr(ctxt, 4);
	if ((cr4 & X86_CR4_DE) && (dr == 4 || dr == 5))
		return emulate_ud(ctxt);

<<<<<<< HEAD
	if (check_dr7_gd(ctxt))
		return emulate_db(ctxt);
=======
	if (ctxt->ops->get_dr(ctxt, 7) & DR7_GD) {
		ulong dr6;

		dr6 = ctxt->ops->get_dr(ctxt, 6);
		dr6 &= ~DR_TRAP_BITS;
		dr6 |= DR6_BD | DR6_ACTIVE_LOW;
		ctxt->ops->set_dr(ctxt, 6, dr6);
		return emulate_db(ctxt);
	}
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	return X86EMUL_CONTINUE;
}

static int check_dr_write(struct x86_emulate_ctxt *ctxt)
{
	u64 new_val = ctxt->src.val64;
	int dr = ctxt->modrm_reg;

	if ((dr == 6 || dr == 7) && (new_val & 0xffffffff00000000ULL))
		return emulate_gp(ctxt, 0);

	return check_dr_read(ctxt);
}

static int check_svme(struct x86_emulate_ctxt *ctxt)
{
<<<<<<< HEAD
	u64 efer;
=======
	u64 efer = 0;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	ctxt->ops->get_msr(ctxt, MSR_EFER, &efer);

	if (!(efer & EFER_SVME))
		return emulate_ud(ctxt);

	return X86EMUL_CONTINUE;
}

static int check_svme_pa(struct x86_emulate_ctxt *ctxt)
{
<<<<<<< HEAD
	u64 rax = ctxt->regs[VCPU_REGS_RAX];
=======
	u64 rax = reg_read(ctxt, VCPU_REGS_RAX);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	/* Valid physical address? */
	if (rax & 0xffff000000000000ULL)
		return emulate_gp(ctxt, 0);

	return check_svme(ctxt);
}

static int check_rdtsc(struct x86_emulate_ctxt *ctxt)
{
	u64 cr4 = ctxt->ops->get_cr(ctxt, 4);

	if (cr4 & X86_CR4_TSD && ctxt->ops->cpl(ctxt))
<<<<<<< HEAD
		return emulate_ud(ctxt);
=======
		return emulate_gp(ctxt, 0);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	return X86EMUL_CONTINUE;
}

static int check_rdpmc(struct x86_emulate_ctxt *ctxt)
{
	u64 cr4 = ctxt->ops->get_cr(ctxt, 4);
<<<<<<< HEAD
	u64 rcx = ctxt->regs[VCPU_REGS_RCX];

	if ((!(cr4 & X86_CR4_PCE) && ctxt->ops->cpl(ctxt)) ||
	    (rcx > 3))
=======
	u64 rcx = reg_read(ctxt, VCPU_REGS_RCX);

	/*
	 * VMware allows access to these Pseduo-PMCs even when read via RDPMC
	 * in Ring3 when CR4.PCE=0.
	 */
	if (enable_vmware_backdoor && is_vmware_backdoor_pmc(rcx))
		return X86EMUL_CONTINUE;

	/*
	 * If CR4.PCE is set, the SDM requires CPL=0 or CR0.PE=0.  The CR0.PE
	 * check however is unnecessary because CPL is always 0 outside
	 * protected mode.
	 */
	if ((!(cr4 & X86_CR4_PCE) && ctxt->ops->cpl(ctxt)) ||
	    ctxt->ops->check_rdpmc_early(ctxt, rcx))
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		return emulate_gp(ctxt, 0);

	return X86EMUL_CONTINUE;
}

static int check_perm_in(struct x86_emulate_ctxt *ctxt)
{
	ctxt->dst.bytes = min(ctxt->dst.bytes, 4u);
<<<<<<< HEAD
	if (!emulator_io_permited(ctxt, ctxt->src.val, ctxt->dst.bytes))
=======
	if (!emulator_io_permitted(ctxt, ctxt->src.val, ctxt->dst.bytes))
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		return emulate_gp(ctxt, 0);

	return X86EMUL_CONTINUE;
}

static int check_perm_out(struct x86_emulate_ctxt *ctxt)
{
	ctxt->src.bytes = min(ctxt->src.bytes, 4u);
<<<<<<< HEAD
	if (!emulator_io_permited(ctxt, ctxt->dst.val, ctxt->src.bytes))
=======
	if (!emulator_io_permitted(ctxt, ctxt->dst.val, ctxt->src.bytes))
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		return emulate_gp(ctxt, 0);

	return X86EMUL_CONTINUE;
}

#define D(_y) { .flags = (_y) }
<<<<<<< HEAD
#define DI(_y, _i) { .flags = (_y), .intercept = x86_intercept_##_i }
#define DIP(_y, _i, _p) { .flags = (_y), .intercept = x86_intercept_##_i, \
		      .check_perm = (_p) }
#define N    D(0)
#define EXT(_f, _e) { .flags = ((_f) | RMExt), .u.group = (_e) }
#define G(_f, _g) { .flags = ((_f) | Group), .u.group = (_g) }
#define GD(_f, _g) { .flags = ((_f) | GroupDual), .u.gdual = (_g) }
#define I(_f, _e) { .flags = (_f), .u.execute = (_e) }
#define II(_f, _e, _i) \
	{ .flags = (_f), .u.execute = (_e), .intercept = x86_intercept_##_i }
#define IIP(_f, _e, _i, _p) \
	{ .flags = (_f), .u.execute = (_e), .intercept = x86_intercept_##_i, \
	  .check_perm = (_p) }
=======
#define DI(_y, _i) { .flags = (_y)|Intercept, .intercept = x86_intercept_##_i }
#define DIP(_y, _i, _p) { .flags = (_y)|Intercept|CheckPerm, \
		      .intercept = x86_intercept_##_i, .check_perm = (_p) }
#define N    D(NotImpl)
#define EXT(_f, _e) { .flags = ((_f) | RMExt), .u.group = (_e) }
#define G(_f, _g) { .flags = ((_f) | Group | ModRM), .u.group = (_g) }
#define GD(_f, _g) { .flags = ((_f) | GroupDual | ModRM), .u.gdual = (_g) }
#define ID(_f, _i) { .flags = ((_f) | InstrDual | ModRM), .u.idual = (_i) }
#define MD(_f, _m) { .flags = ((_f) | ModeDual), .u.mdual = (_m) }
#define E(_f, _e) { .flags = ((_f) | Escape | ModRM), .u.esc = (_e) }
#define I(_f, _e) { .flags = (_f), .u.execute = (_e) }
#define F(_f, _e) { .flags = (_f) | Fastop, .u.fastop = (_e) }
#define II(_f, _e, _i) \
	{ .flags = (_f)|Intercept, .u.execute = (_e), .intercept = x86_intercept_##_i }
#define IIP(_f, _e, _i, _p) \
	{ .flags = (_f)|Intercept|CheckPerm, .u.execute = (_e), \
	  .intercept = x86_intercept_##_i, .check_perm = (_p) }
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#define GP(_f, _g) { .flags = ((_f) | Prefix), .u.gprefix = (_g) }

#define D2bv(_f)      D((_f) | ByteOp), D(_f)
#define D2bvIP(_f, _i, _p) DIP((_f) | ByteOp, _i, _p), DIP(_f, _i, _p)
#define I2bv(_f, _e)  I((_f) | ByteOp, _e), I(_f, _e)
<<<<<<< HEAD
#define I2bvIP(_f, _e, _i, _p) \
	IIP((_f) | ByteOp, _e, _i, _p), IIP(_f, _e, _i, _p)

#define I6ALU(_f, _e) I2bv((_f) | DstMem | SrcReg | ModRM, _e),		\
		I2bv(((_f) | DstReg | SrcMem | ModRM) & ~Lock, _e),	\
		I2bv(((_f) & ~Lock) | DstAcc | SrcImm, _e)

static struct opcode group7_rm1[] = {
	DI(SrcNone | ModRM | Priv, monitor),
	DI(SrcNone | ModRM | Priv, mwait),
	N, N, N, N, N, N,
};

static struct opcode group7_rm3[] = {
	DIP(SrcNone | ModRM | Prot | Priv, vmrun,   check_svme_pa),
	II(SrcNone | ModRM | Prot | VendorSpecific, em_vmmcall, vmmcall),
	DIP(SrcNone | ModRM | Prot | Priv, vmload,  check_svme_pa),
	DIP(SrcNone | ModRM | Prot | Priv, vmsave,  check_svme_pa),
	DIP(SrcNone | ModRM | Prot | Priv, stgi,    check_svme),
	DIP(SrcNone | ModRM | Prot | Priv, clgi,    check_svme),
	DIP(SrcNone | ModRM | Prot | Priv, skinit,  check_svme),
	DIP(SrcNone | ModRM | Prot | Priv, invlpga, check_svme),
};

static struct opcode group7_rm7[] = {
	N,
	DIP(SrcNone | ModRM, rdtscp, check_rdtsc),
	N, N, N, N, N, N,
};

static struct opcode group1[] = {
	I(Lock, em_add),
	I(Lock | PageTable, em_or),
	I(Lock, em_adc),
	I(Lock, em_sbb),
	I(Lock | PageTable, em_and),
	I(Lock, em_sub),
	I(Lock, em_xor),
	I(0, em_cmp),
};

static struct opcode group1A[] = {
	I(DstMem | SrcNone | ModRM | Mov | Stack, em_pop), N, N, N, N, N, N, N,
};

static struct opcode group3[] = {
	I(DstMem | SrcImm | ModRM, em_test),
	I(DstMem | SrcImm | ModRM, em_test),
	I(DstMem | SrcNone | ModRM | Lock, em_not),
	I(DstMem | SrcNone | ModRM | Lock, em_neg),
	I(SrcMem | ModRM, em_mul_ex),
	I(SrcMem | ModRM, em_imul_ex),
	I(SrcMem | ModRM, em_div_ex),
	I(SrcMem | ModRM, em_idiv_ex),
};

static struct opcode group4[] = {
	I(ByteOp | DstMem | SrcNone | ModRM | Lock, em_grp45),
	I(ByteOp | DstMem | SrcNone | ModRM | Lock, em_grp45),
	N, N, N, N, N, N,
};

static struct opcode group5[] = {
	I(DstMem | SrcNone | ModRM | Lock, em_grp45),
	I(DstMem | SrcNone | ModRM | Lock, em_grp45),
	I(SrcMem | ModRM | Stack, em_grp45),
	I(SrcMemFAddr | ModRM | ImplicitOps | Stack, em_call_far),
	I(SrcMem | ModRM | Stack, em_grp45),
	I(SrcMemFAddr | ModRM | ImplicitOps, em_grp45),
	I(SrcMem | ModRM | Stack, em_grp45), N,
};

static struct opcode group6[] = {
	DI(ModRM | Prot,        sldt),
	DI(ModRM | Prot,        str),
	DI(ModRM | Prot | Priv, lldt),
	DI(ModRM | Prot | Priv, ltr),
	N, N, N, N,
};

static struct group_dual group7 = { {
	DI(ModRM | Mov | DstMem | Priv, sgdt),
	DI(ModRM | Mov | DstMem | Priv, sidt),
	II(ModRM | SrcMem | Priv, em_lgdt, lgdt),
	II(ModRM | SrcMem | Priv, em_lidt, lidt),
	II(SrcNone | ModRM | DstMem | Mov, em_smsw, smsw), N,
	II(SrcMem16 | ModRM | Mov | Priv, em_lmsw, lmsw),
	II(SrcMem | ModRM | ByteOp | Priv | NoAccess, em_invlpg, invlpg),
}, {
	I(SrcNone | ModRM | Priv | VendorSpecific, em_vmcall),
	EXT(0, group7_rm1),
	N, EXT(0, group7_rm3),
	II(SrcNone | ModRM | DstMem | Mov, em_smsw, smsw), N,
	II(SrcMem16 | ModRM | Mov | Priv, em_lmsw, lmsw), EXT(0, group7_rm7),
} };

static struct opcode group8[] = {
	N, N, N, N,
	I(DstMem | SrcImmByte | ModRM, em_bt),
	I(DstMem | SrcImmByte | ModRM | Lock | PageTable, em_bts),
	I(DstMem | SrcImmByte | ModRM | Lock, em_btr),
	I(DstMem | SrcImmByte | ModRM | Lock | PageTable, em_btc),
};

static struct group_dual group9 = { {
	N, I(DstMem64 | ModRM | Lock | PageTable, em_cmpxchg8b), N, N, N, N, N, N,
=======
#define F2bv(_f, _e)  F((_f) | ByteOp, _e), F(_f, _e)
#define I2bvIP(_f, _e, _i, _p) \
	IIP((_f) | ByteOp, _e, _i, _p), IIP(_f, _e, _i, _p)

#define F6ALU(_f, _e) F2bv((_f) | DstMem | SrcReg | ModRM, _e),		\
		F2bv(((_f) | DstReg | SrcMem | ModRM) & ~Lock, _e),	\
		F2bv(((_f) & ~Lock) | DstAcc | SrcImm, _e)

static const struct opcode group7_rm0[] = {
	N,
	I(SrcNone | Priv | EmulateOnUD,	em_hypercall),
	N, N, N, N, N, N,
};

static const struct opcode group7_rm1[] = {
	DI(SrcNone | Priv, monitor),
	DI(SrcNone | Priv, mwait),
	N, N, N, N, N, N,
};

static const struct opcode group7_rm2[] = {
	N,
	II(ImplicitOps | Priv,			em_xsetbv,	xsetbv),
	N, N, N, N, N, N,
};

static const struct opcode group7_rm3[] = {
	DIP(SrcNone | Prot | Priv,		vmrun,		check_svme_pa),
	II(SrcNone  | Prot | EmulateOnUD,	em_hypercall,	vmmcall),
	DIP(SrcNone | Prot | Priv,		vmload,		check_svme_pa),
	DIP(SrcNone | Prot | Priv,		vmsave,		check_svme_pa),
	DIP(SrcNone | Prot | Priv,		stgi,		check_svme),
	DIP(SrcNone | Prot | Priv,		clgi,		check_svme),
	DIP(SrcNone | Prot | Priv,		skinit,		check_svme),
	DIP(SrcNone | Prot | Priv,		invlpga,	check_svme),
};

static const struct opcode group7_rm7[] = {
	N,
	DIP(SrcNone, rdtscp, check_rdtsc),
	N, N, N, N, N, N,
};

static const struct opcode group1[] = {
	F(Lock, em_add),
	F(Lock | PageTable, em_or),
	F(Lock, em_adc),
	F(Lock, em_sbb),
	F(Lock | PageTable, em_and),
	F(Lock, em_sub),
	F(Lock, em_xor),
	F(NoWrite, em_cmp),
};

static const struct opcode group1A[] = {
	I(DstMem | SrcNone | Mov | Stack | IncSP | TwoMemOp, em_pop), N, N, N, N, N, N, N,
};

static const struct opcode group2[] = {
	F(DstMem | ModRM, em_rol),
	F(DstMem | ModRM, em_ror),
	F(DstMem | ModRM, em_rcl),
	F(DstMem | ModRM, em_rcr),
	F(DstMem | ModRM, em_shl),
	F(DstMem | ModRM, em_shr),
	F(DstMem | ModRM, em_shl),
	F(DstMem | ModRM, em_sar),
};

static const struct opcode group3[] = {
	F(DstMem | SrcImm | NoWrite, em_test),
	F(DstMem | SrcImm | NoWrite, em_test),
	F(DstMem | SrcNone | Lock, em_not),
	F(DstMem | SrcNone | Lock, em_neg),
	F(DstXacc | Src2Mem, em_mul_ex),
	F(DstXacc | Src2Mem, em_imul_ex),
	F(DstXacc | Src2Mem, em_div_ex),
	F(DstXacc | Src2Mem, em_idiv_ex),
};

static const struct opcode group4[] = {
	F(ByteOp | DstMem | SrcNone | Lock, em_inc),
	F(ByteOp | DstMem | SrcNone | Lock, em_dec),
	N, N, N, N, N, N,
};

static const struct opcode group5[] = {
	F(DstMem | SrcNone | Lock,		em_inc),
	F(DstMem | SrcNone | Lock,		em_dec),
	I(SrcMem | NearBranch | IsBranch,       em_call_near_abs),
	I(SrcMemFAddr | ImplicitOps | IsBranch, em_call_far),
	I(SrcMem | NearBranch | IsBranch,       em_jmp_abs),
	I(SrcMemFAddr | ImplicitOps | IsBranch, em_jmp_far),
	I(SrcMem | Stack | TwoMemOp,		em_push), D(Undefined),
};

static const struct opcode group6[] = {
	II(Prot | DstMem,	   em_sldt, sldt),
	II(Prot | DstMem,	   em_str, str),
	II(Prot | Priv | SrcMem16, em_lldt, lldt),
	II(Prot | Priv | SrcMem16, em_ltr, ltr),
	N, N, N, N,
};

static const struct group_dual group7 = { {
	II(Mov | DstMem,			em_sgdt, sgdt),
	II(Mov | DstMem,			em_sidt, sidt),
	II(SrcMem | Priv,			em_lgdt, lgdt),
	II(SrcMem | Priv,			em_lidt, lidt),
	II(SrcNone | DstMem | Mov,		em_smsw, smsw), N,
	II(SrcMem16 | Mov | Priv,		em_lmsw, lmsw),
	II(SrcMem | ByteOp | Priv | NoAccess,	em_invlpg, invlpg),
}, {
	EXT(0, group7_rm0),
	EXT(0, group7_rm1),
	EXT(0, group7_rm2),
	EXT(0, group7_rm3),
	II(SrcNone | DstMem | Mov,		em_smsw, smsw), N,
	II(SrcMem16 | Mov | Priv,		em_lmsw, lmsw),
	EXT(0, group7_rm7),
} };

static const struct opcode group8[] = {
	N, N, N, N,
	F(DstMem | SrcImmByte | NoWrite,		em_bt),
	F(DstMem | SrcImmByte | Lock | PageTable,	em_bts),
	F(DstMem | SrcImmByte | Lock,			em_btr),
	F(DstMem | SrcImmByte | Lock | PageTable,	em_btc),
};

/*
 * The "memory" destination is actually always a register, since we come
 * from the register case of group9.
 */
static const struct gprefix pfx_0f_c7_7 = {
	N, N, N, II(DstMem | ModRM | Op3264 | EmulateOnUD, em_rdpid, rdpid),
};


static const struct group_dual group9 = { {
	N, I(DstMem64 | Lock | PageTable, em_cmpxchg8b), N, N, N, N, N, N,
}, {
	N, N, N, N, N, N, N,
	GP(0, &pfx_0f_c7_7),
} };

static const struct opcode group11[] = {
	I(DstMem | SrcImm | Mov | PageTable, em_mov),
	X7(D(Undefined)),
};

static const struct gprefix pfx_0f_ae_7 = {
	I(SrcMem | ByteOp, em_clflush), I(SrcMem | ByteOp, em_clflushopt), N, N,
};

static const struct group_dual group15 = { {
	I(ModRM | Aligned16, em_fxsave),
	I(ModRM | Aligned16, em_fxrstor),
	N, N, N, N, N, GP(0, &pfx_0f_ae_7),
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}, {
	N, N, N, N, N, N, N, N,
} };

<<<<<<< HEAD
static struct opcode group11[] = {
	I(DstMem | SrcImm | ModRM | Mov | PageTable, em_mov),
	X7(D(Undefined)),
};

static struct gprefix pfx_0f_6f_0f_7f = {
	N, N, N, I(Sse, em_movdqu),
};

static struct opcode opcode_table[256] = {
	/* 0x00 - 0x07 */
	I6ALU(Lock, em_add),
	I(ImplicitOps | Stack | No64 | Src2ES, em_push_sreg),
	I(ImplicitOps | Stack | No64 | Src2ES, em_pop_sreg),
	/* 0x08 - 0x0F */
	I6ALU(Lock | PageTable, em_or),
	I(ImplicitOps | Stack | No64 | Src2CS, em_push_sreg),
	N,
	/* 0x10 - 0x17 */
	I6ALU(Lock, em_adc),
	I(ImplicitOps | Stack | No64 | Src2SS, em_push_sreg),
	I(ImplicitOps | Stack | No64 | Src2SS, em_pop_sreg),
	/* 0x18 - 0x1F */
	I6ALU(Lock, em_sbb),
	I(ImplicitOps | Stack | No64 | Src2DS, em_push_sreg),
	I(ImplicitOps | Stack | No64 | Src2DS, em_pop_sreg),
	/* 0x20 - 0x27 */
	I6ALU(Lock | PageTable, em_and), N, N,
	/* 0x28 - 0x2F */
	I6ALU(Lock, em_sub), N, I(ByteOp | DstAcc | No64, em_das),
	/* 0x30 - 0x37 */
	I6ALU(Lock, em_xor), N, N,
	/* 0x38 - 0x3F */
	I6ALU(0, em_cmp), N, N,
	/* 0x40 - 0x4F */
	X16(D(DstReg)),
=======
static const struct gprefix pfx_0f_6f_0f_7f = {
	I(Mmx, em_mov), I(Sse | Aligned, em_mov), N, I(Sse | Unaligned, em_mov),
};

static const struct instr_dual instr_dual_0f_2b = {
	I(0, em_mov), N
};

static const struct gprefix pfx_0f_2b = {
	ID(0, &instr_dual_0f_2b), ID(0, &instr_dual_0f_2b), N, N,
};

static const struct gprefix pfx_0f_10_0f_11 = {
	I(Unaligned, em_mov), I(Unaligned, em_mov), N, N,
};

static const struct gprefix pfx_0f_28_0f_29 = {
	I(Aligned, em_mov), I(Aligned, em_mov), N, N,
};

static const struct gprefix pfx_0f_e7 = {
	N, I(Sse, em_mov), N, N,
};

static const struct escape escape_d9 = { {
	N, N, N, N, N, N, N, I(DstMem16 | Mov, em_fnstcw),
}, {
	/* 0xC0 - 0xC7 */
	N, N, N, N, N, N, N, N,
	/* 0xC8 - 0xCF */
	N, N, N, N, N, N, N, N,
	/* 0xD0 - 0xC7 */
	N, N, N, N, N, N, N, N,
	/* 0xD8 - 0xDF */
	N, N, N, N, N, N, N, N,
	/* 0xE0 - 0xE7 */
	N, N, N, N, N, N, N, N,
	/* 0xE8 - 0xEF */
	N, N, N, N, N, N, N, N,
	/* 0xF0 - 0xF7 */
	N, N, N, N, N, N, N, N,
	/* 0xF8 - 0xFF */
	N, N, N, N, N, N, N, N,
} };

static const struct escape escape_db = { {
	N, N, N, N, N, N, N, N,
}, {
	/* 0xC0 - 0xC7 */
	N, N, N, N, N, N, N, N,
	/* 0xC8 - 0xCF */
	N, N, N, N, N, N, N, N,
	/* 0xD0 - 0xC7 */
	N, N, N, N, N, N, N, N,
	/* 0xD8 - 0xDF */
	N, N, N, N, N, N, N, N,
	/* 0xE0 - 0xE7 */
	N, N, N, I(ImplicitOps, em_fninit), N, N, N, N,
	/* 0xE8 - 0xEF */
	N, N, N, N, N, N, N, N,
	/* 0xF0 - 0xF7 */
	N, N, N, N, N, N, N, N,
	/* 0xF8 - 0xFF */
	N, N, N, N, N, N, N, N,
} };

static const struct escape escape_dd = { {
	N, N, N, N, N, N, N, I(DstMem16 | Mov, em_fnstsw),
}, {
	/* 0xC0 - 0xC7 */
	N, N, N, N, N, N, N, N,
	/* 0xC8 - 0xCF */
	N, N, N, N, N, N, N, N,
	/* 0xD0 - 0xC7 */
	N, N, N, N, N, N, N, N,
	/* 0xD8 - 0xDF */
	N, N, N, N, N, N, N, N,
	/* 0xE0 - 0xE7 */
	N, N, N, N, N, N, N, N,
	/* 0xE8 - 0xEF */
	N, N, N, N, N, N, N, N,
	/* 0xF0 - 0xF7 */
	N, N, N, N, N, N, N, N,
	/* 0xF8 - 0xFF */
	N, N, N, N, N, N, N, N,
} };

static const struct instr_dual instr_dual_0f_c3 = {
	I(DstMem | SrcReg | ModRM | No16 | Mov, em_mov), N
};

static const struct mode_dual mode_dual_63 = {
	N, I(DstReg | SrcMem32 | ModRM | Mov, em_movsxd)
};

static const struct instr_dual instr_dual_8d = {
	D(DstReg | SrcMem | ModRM | NoAccess), N
};

static const struct opcode opcode_table[256] = {
	/* 0x00 - 0x07 */
	F6ALU(Lock, em_add),
	I(ImplicitOps | Stack | No64 | Src2ES, em_push_sreg),
	I(ImplicitOps | Stack | No64 | Src2ES, em_pop_sreg),
	/* 0x08 - 0x0F */
	F6ALU(Lock | PageTable, em_or),
	I(ImplicitOps | Stack | No64 | Src2CS, em_push_sreg),
	N,
	/* 0x10 - 0x17 */
	F6ALU(Lock, em_adc),
	I(ImplicitOps | Stack | No64 | Src2SS, em_push_sreg),
	I(ImplicitOps | Stack | No64 | Src2SS, em_pop_sreg),
	/* 0x18 - 0x1F */
	F6ALU(Lock, em_sbb),
	I(ImplicitOps | Stack | No64 | Src2DS, em_push_sreg),
	I(ImplicitOps | Stack | No64 | Src2DS, em_pop_sreg),
	/* 0x20 - 0x27 */
	F6ALU(Lock | PageTable, em_and), N, N,
	/* 0x28 - 0x2F */
	F6ALU(Lock, em_sub), N, I(ByteOp | DstAcc | No64, em_das),
	/* 0x30 - 0x37 */
	F6ALU(Lock, em_xor), N, N,
	/* 0x38 - 0x3F */
	F6ALU(NoWrite, em_cmp), N, N,
	/* 0x40 - 0x4F */
	X8(F(DstReg, em_inc)), X8(F(DstReg, em_dec)),
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	/* 0x50 - 0x57 */
	X8(I(SrcReg | Stack, em_push)),
	/* 0x58 - 0x5F */
	X8(I(DstReg | Stack, em_pop)),
	/* 0x60 - 0x67 */
	I(ImplicitOps | Stack | No64, em_pusha),
	I(ImplicitOps | Stack | No64, em_popa),
<<<<<<< HEAD
	N, D(DstReg | SrcMem32 | ModRM | Mov) /* movsxd (x86/64) */ ,
=======
	N, MD(ModRM, &mode_dual_63),
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	N, N, N, N,
	/* 0x68 - 0x6F */
	I(SrcImm | Mov | Stack, em_push),
	I(DstReg | SrcMem | ModRM | Src2Imm, em_imul_3op),
	I(SrcImmByte | Mov | Stack, em_push),
	I(DstReg | SrcMem | ModRM | Src2ImmByte, em_imul_3op),
<<<<<<< HEAD
	I2bvIP(DstDI | SrcDX | Mov | String, em_in, ins, check_perm_in), /* insb, insw/insd */
	I2bvIP(SrcSI | DstDX | String, em_out, outs, check_perm_out), /* outsb, outsw/outsd */
	/* 0x70 - 0x7F */
	X16(D(SrcImmByte)),
	/* 0x80 - 0x87 */
	G(ByteOp | DstMem | SrcImm | ModRM | Group, group1),
	G(DstMem | SrcImm | ModRM | Group, group1),
	G(ByteOp | DstMem | SrcImm | ModRM | No64 | Group, group1),
	G(DstMem | SrcImmByte | ModRM | Group, group1),
	I2bv(DstMem | SrcReg | ModRM, em_test),
=======
	I2bvIP(DstDI | SrcDX | Mov | String | Unaligned, em_in, ins, check_perm_in), /* insb, insw/insd */
	I2bvIP(SrcSI | DstDX | String, em_out, outs, check_perm_out), /* outsb, outsw/outsd */
	/* 0x70 - 0x7F */
	X16(D(SrcImmByte | NearBranch | IsBranch)),
	/* 0x80 - 0x87 */
	G(ByteOp | DstMem | SrcImm, group1),
	G(DstMem | SrcImm, group1),
	G(ByteOp | DstMem | SrcImm | No64, group1),
	G(DstMem | SrcImmByte, group1),
	F2bv(DstMem | SrcReg | ModRM | NoWrite, em_test),
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	I2bv(DstMem | SrcReg | ModRM | Lock | PageTable, em_xchg),
	/* 0x88 - 0x8F */
	I2bv(DstMem | SrcReg | ModRM | Mov | PageTable, em_mov),
	I2bv(DstReg | SrcMem | ModRM | Mov, em_mov),
	I(DstMem | SrcNone | ModRM | Mov | PageTable, em_mov_rm_sreg),
<<<<<<< HEAD
	D(ModRM | SrcMem | NoAccess | DstReg),
=======
	ID(0, &instr_dual_8d),
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	I(ImplicitOps | SrcMem16 | ModRM, em_mov_sreg_rm),
	G(0, group1A),
	/* 0x90 - 0x97 */
	DI(SrcAcc | DstReg, pause), X7(D(SrcAcc | DstReg)),
	/* 0x98 - 0x9F */
	D(DstAcc | SrcNone), I(ImplicitOps | SrcAcc, em_cwd),
<<<<<<< HEAD
	I(SrcImmFAddr | No64, em_call_far), N,
	II(ImplicitOps | Stack, em_pushf, pushf),
	II(ImplicitOps | Stack, em_popf, popf), N, N,
	/* 0xA0 - 0xA7 */
	I2bv(DstAcc | SrcMem | Mov | MemAbs, em_mov),
	I2bv(DstMem | SrcAcc | Mov | MemAbs | PageTable, em_mov),
	I2bv(SrcSI | DstDI | Mov | String, em_mov),
	I2bv(SrcSI | DstDI | String, em_cmp),
	/* 0xA8 - 0xAF */
	I2bv(DstAcc | SrcImm, em_test),
	I2bv(SrcAcc | DstDI | Mov | String, em_mov),
	I2bv(SrcSI | DstAcc | Mov | String, em_mov),
	I2bv(SrcAcc | DstDI | String, em_cmp),
	/* 0xB0 - 0xB7 */
	X8(I(ByteOp | DstReg | SrcImm | Mov, em_mov)),
	/* 0xB8 - 0xBF */
	X8(I(DstReg | SrcImm | Mov, em_mov)),
	/* 0xC0 - 0xC7 */
	D2bv(DstMem | SrcImmByte | ModRM),
	I(ImplicitOps | Stack | SrcImmU16, em_ret_near_imm),
	I(ImplicitOps | Stack, em_ret),
=======
	I(SrcImmFAddr | No64 | IsBranch, em_call_far), N,
	II(ImplicitOps | Stack, em_pushf, pushf),
	II(ImplicitOps | Stack, em_popf, popf),
	I(ImplicitOps, em_sahf), I(ImplicitOps, em_lahf),
	/* 0xA0 - 0xA7 */
	I2bv(DstAcc | SrcMem | Mov | MemAbs, em_mov),
	I2bv(DstMem | SrcAcc | Mov | MemAbs | PageTable, em_mov),
	I2bv(SrcSI | DstDI | Mov | String | TwoMemOp, em_mov),
	F2bv(SrcSI | DstDI | String | NoWrite | TwoMemOp, em_cmp_r),
	/* 0xA8 - 0xAF */
	F2bv(DstAcc | SrcImm | NoWrite, em_test),
	I2bv(SrcAcc | DstDI | Mov | String, em_mov),
	I2bv(SrcSI | DstAcc | Mov | String, em_mov),
	F2bv(SrcAcc | DstDI | String | NoWrite, em_cmp_r),
	/* 0xB0 - 0xB7 */
	X8(I(ByteOp | DstReg | SrcImm | Mov, em_mov)),
	/* 0xB8 - 0xBF */
	X8(I(DstReg | SrcImm64 | Mov, em_mov)),
	/* 0xC0 - 0xC7 */
	G(ByteOp | Src2ImmByte, group2), G(Src2ImmByte, group2),
	I(ImplicitOps | NearBranch | SrcImmU16 | IsBranch, em_ret_near_imm),
	I(ImplicitOps | NearBranch | IsBranch, em_ret),
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	I(DstReg | SrcMemFAddr | ModRM | No64 | Src2ES, em_lseg),
	I(DstReg | SrcMemFAddr | ModRM | No64 | Src2DS, em_lseg),
	G(ByteOp, group11), G(0, group11),
	/* 0xC8 - 0xCF */
<<<<<<< HEAD
	N, N, N, I(ImplicitOps | Stack, em_ret_far),
	D(ImplicitOps), DI(SrcImmByte, intn),
	D(ImplicitOps | No64), II(ImplicitOps, em_iret, iret),
	/* 0xD0 - 0xD7 */
	D2bv(DstMem | SrcOne | ModRM), D2bv(DstMem | ModRM),
	N, N, N, N,
	/* 0xD8 - 0xDF */
	N, N, N, N, N, N, N, N,
	/* 0xE0 - 0xE7 */
	X3(I(SrcImmByte, em_loop)),
	I(SrcImmByte, em_jcxz),
	I2bvIP(SrcImmUByte | DstAcc, em_in,  in,  check_perm_in),
	I2bvIP(SrcAcc | DstImmUByte, em_out, out, check_perm_out),
	/* 0xE8 - 0xEF */
	I(SrcImm | Stack, em_call), D(SrcImm | ImplicitOps),
	I(SrcImmFAddr | No64, em_jmp_far), D(SrcImmByte | ImplicitOps),
=======
	I(Stack | SrcImmU16 | Src2ImmByte | IsBranch, em_enter),
	I(Stack | IsBranch, em_leave),
	I(ImplicitOps | SrcImmU16 | IsBranch, em_ret_far_imm),
	I(ImplicitOps | IsBranch, em_ret_far),
	D(ImplicitOps | IsBranch), DI(SrcImmByte | IsBranch, intn),
	D(ImplicitOps | No64 | IsBranch),
	II(ImplicitOps | IsBranch, em_iret, iret),
	/* 0xD0 - 0xD7 */
	G(Src2One | ByteOp, group2), G(Src2One, group2),
	G(Src2CL | ByteOp, group2), G(Src2CL, group2),
	I(DstAcc | SrcImmUByte | No64, em_aam),
	I(DstAcc | SrcImmUByte | No64, em_aad),
	F(DstAcc | ByteOp | No64, em_salc),
	I(DstAcc | SrcXLat | ByteOp, em_mov),
	/* 0xD8 - 0xDF */
	N, E(0, &escape_d9), N, E(0, &escape_db), N, E(0, &escape_dd), N, N,
	/* 0xE0 - 0xE7 */
	X3(I(SrcImmByte | NearBranch | IsBranch, em_loop)),
	I(SrcImmByte | NearBranch | IsBranch, em_jcxz),
	I2bvIP(SrcImmUByte | DstAcc, em_in,  in,  check_perm_in),
	I2bvIP(SrcAcc | DstImmUByte, em_out, out, check_perm_out),
	/* 0xE8 - 0xEF */
	I(SrcImm | NearBranch | IsBranch, em_call),
	D(SrcImm | ImplicitOps | NearBranch | IsBranch),
	I(SrcImmFAddr | No64 | IsBranch, em_jmp_far),
	D(SrcImmByte | ImplicitOps | NearBranch | IsBranch),
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	I2bvIP(SrcDX | DstAcc, em_in,  in,  check_perm_in),
	I2bvIP(SrcAcc | DstDX, em_out, out, check_perm_out),
	/* 0xF0 - 0xF7 */
	N, DI(ImplicitOps, icebp), N, N,
	DI(ImplicitOps | Priv, hlt), D(ImplicitOps),
	G(ByteOp, group3), G(0, group3),
	/* 0xF8 - 0xFF */
	D(ImplicitOps), D(ImplicitOps),
	I(ImplicitOps, em_cli), I(ImplicitOps, em_sti),
	D(ImplicitOps), D(ImplicitOps), G(0, group4), G(0, group5),
};

<<<<<<< HEAD
static struct opcode twobyte_table[256] = {
	/* 0x00 - 0x0F */
	G(0, group6), GD(0, &group7), N, N,
	N, I(ImplicitOps | VendorSpecific, em_syscall),
	II(ImplicitOps | Priv, em_clts, clts), N,
	DI(ImplicitOps | Priv, invd), DI(ImplicitOps | Priv, wbinvd), N, N,
	N, D(ImplicitOps | ModRM), N, N,
	/* 0x10 - 0x1F */
	N, N, N, N, N, N, N, N, D(ImplicitOps | ModRM), N, N, N, N, N, N, N,
	/* 0x20 - 0x2F */
	DIP(ModRM | DstMem | Priv | Op3264, cr_read, check_cr_read),
	DIP(ModRM | DstMem | Priv | Op3264, dr_read, check_dr_read),
	IIP(ModRM | SrcMem | Priv | Op3264, em_cr_write, cr_write, check_cr_write),
	IIP(ModRM | SrcMem | Priv | Op3264, em_dr_write, dr_write, check_dr_write),
	N, N, N, N,
	N, N, N, N, N, N, N, N,
=======
static const struct opcode twobyte_table[256] = {
	/* 0x00 - 0x0F */
	G(0, group6), GD(0, &group7), N, N,
	N, I(ImplicitOps | EmulateOnUD | IsBranch, em_syscall),
	II(ImplicitOps | Priv, em_clts, clts), N,
	DI(ImplicitOps | Priv, invd), DI(ImplicitOps | Priv, wbinvd), N, N,
	N, D(ImplicitOps | ModRM | SrcMem | NoAccess), N, N,
	/* 0x10 - 0x1F */
	GP(ModRM | DstReg | SrcMem | Mov | Sse, &pfx_0f_10_0f_11),
	GP(ModRM | DstMem | SrcReg | Mov | Sse, &pfx_0f_10_0f_11),
	N, N, N, N, N, N,
	D(ImplicitOps | ModRM | SrcMem | NoAccess), /* 4 * prefetch + 4 * reserved NOP */
	D(ImplicitOps | ModRM | SrcMem | NoAccess), N, N,
	D(ImplicitOps | ModRM | SrcMem | NoAccess), /* 8 * reserved NOP */
	D(ImplicitOps | ModRM | SrcMem | NoAccess), /* 8 * reserved NOP */
	D(ImplicitOps | ModRM | SrcMem | NoAccess), /* 8 * reserved NOP */
	D(ImplicitOps | ModRM | SrcMem | NoAccess), /* NOP + 7 * reserved NOP */
	/* 0x20 - 0x2F */
	DIP(ModRM | DstMem | Priv | Op3264 | NoMod, cr_read, check_cr_access),
	DIP(ModRM | DstMem | Priv | Op3264 | NoMod, dr_read, check_dr_read),
	IIP(ModRM | SrcMem | Priv | Op3264 | NoMod, em_cr_write, cr_write,
						check_cr_access),
	IIP(ModRM | SrcMem | Priv | Op3264 | NoMod, em_dr_write, dr_write,
						check_dr_write),
	N, N, N, N,
	GP(ModRM | DstReg | SrcMem | Mov | Sse, &pfx_0f_28_0f_29),
	GP(ModRM | DstMem | SrcReg | Mov | Sse, &pfx_0f_28_0f_29),
	N, GP(ModRM | DstMem | SrcReg | Mov | Sse, &pfx_0f_2b),
	N, N, N, N,
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	/* 0x30 - 0x3F */
	II(ImplicitOps | Priv, em_wrmsr, wrmsr),
	IIP(ImplicitOps, em_rdtsc, rdtsc, check_rdtsc),
	II(ImplicitOps | Priv, em_rdmsr, rdmsr),
	IIP(ImplicitOps, em_rdpmc, rdpmc, check_rdpmc),
<<<<<<< HEAD
	I(ImplicitOps | VendorSpecific, em_sysenter),
	I(ImplicitOps | Priv | VendorSpecific, em_sysexit),
	N, N,
	N, N, N, N, N, N, N, N,
	/* 0x40 - 0x4F */
	X16(D(DstReg | SrcMem | ModRM | Mov)),
=======
	I(ImplicitOps | EmulateOnUD | IsBranch, em_sysenter),
	I(ImplicitOps | Priv | EmulateOnUD | IsBranch, em_sysexit),
	N, N,
	N, N, N, N, N, N, N, N,
	/* 0x40 - 0x4F */
	X16(D(DstReg | SrcMem | ModRM)),
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	/* 0x50 - 0x5F */
	N, N, N, N, N, N, N, N, N, N, N, N, N, N, N, N,
	/* 0x60 - 0x6F */
	N, N, N, N,
	N, N, N, N,
	N, N, N, N,
	N, N, N, GP(SrcMem | DstReg | ModRM | Mov, &pfx_0f_6f_0f_7f),
	/* 0x70 - 0x7F */
	N, N, N, N,
	N, N, N, N,
	N, N, N, N,
	N, N, N, GP(SrcReg | DstMem | ModRM | Mov, &pfx_0f_6f_0f_7f),
	/* 0x80 - 0x8F */
<<<<<<< HEAD
	X16(D(SrcImm)),
=======
	X16(D(SrcImm | NearBranch | IsBranch)),
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	/* 0x90 - 0x9F */
	X16(D(ByteOp | DstMem | SrcNone | ModRM| Mov)),
	/* 0xA0 - 0xA7 */
	I(Stack | Src2FS, em_push_sreg), I(Stack | Src2FS, em_pop_sreg),
<<<<<<< HEAD
	DI(ImplicitOps, cpuid), I(DstMem | SrcReg | ModRM | BitOp, em_bt),
	D(DstMem | SrcReg | Src2ImmByte | ModRM),
	D(DstMem | SrcReg | Src2CL | ModRM), N, N,
	/* 0xA8 - 0xAF */
	I(Stack | Src2GS, em_push_sreg), I(Stack | Src2GS, em_pop_sreg),
	DI(ImplicitOps, rsm),
	I(DstMem | SrcReg | ModRM | BitOp | Lock | PageTable, em_bts),
	D(DstMem | SrcReg | Src2ImmByte | ModRM),
	D(DstMem | SrcReg | Src2CL | ModRM),
	D(ModRM), I(DstReg | SrcMem | ModRM, em_imul),
	/* 0xB0 - 0xB7 */
	I2bv(DstMem | SrcReg | ModRM | Lock | PageTable, em_cmpxchg),
	I(DstReg | SrcMemFAddr | ModRM | Src2SS, em_lseg),
	I(DstMem | SrcReg | ModRM | BitOp | Lock, em_btr),
=======
	II(ImplicitOps, em_cpuid, cpuid),
	F(DstMem | SrcReg | ModRM | BitOp | NoWrite, em_bt),
	F(DstMem | SrcReg | Src2ImmByte | ModRM, em_shld),
	F(DstMem | SrcReg | Src2CL | ModRM, em_shld), N, N,
	/* 0xA8 - 0xAF */
	I(Stack | Src2GS, em_push_sreg), I(Stack | Src2GS, em_pop_sreg),
	II(EmulateOnUD | ImplicitOps, em_rsm, rsm),
	F(DstMem | SrcReg | ModRM | BitOp | Lock | PageTable, em_bts),
	F(DstMem | SrcReg | Src2ImmByte | ModRM, em_shrd),
	F(DstMem | SrcReg | Src2CL | ModRM, em_shrd),
	GD(0, &group15), F(DstReg | SrcMem | ModRM, em_imul),
	/* 0xB0 - 0xB7 */
	I2bv(DstMem | SrcReg | ModRM | Lock | PageTable | SrcWrite, em_cmpxchg),
	I(DstReg | SrcMemFAddr | ModRM | Src2SS, em_lseg),
	F(DstMem | SrcReg | ModRM | BitOp | Lock, em_btr),
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	I(DstReg | SrcMemFAddr | ModRM | Src2FS, em_lseg),
	I(DstReg | SrcMemFAddr | ModRM | Src2GS, em_lseg),
	D(DstReg | SrcMem8 | ModRM | Mov), D(DstReg | SrcMem16 | ModRM | Mov),
	/* 0xB8 - 0xBF */
	N, N,
	G(BitOp, group8),
<<<<<<< HEAD
	I(DstMem | SrcReg | ModRM | BitOp | Lock | PageTable, em_btc),
	I(DstReg | SrcMem | ModRM, em_bsf), I(DstReg | SrcMem | ModRM, em_bsr),
	D(DstReg | SrcMem8 | ModRM | Mov), D(DstReg | SrcMem16 | ModRM | Mov),
	/* 0xC0 - 0xCF */
	D2bv(DstMem | SrcReg | ModRM | Lock),
	N, D(DstMem | SrcReg | ModRM | Mov),
	N, N, N, GD(0, &group9),
	N, N, N, N, N, N, N, N,
	/* 0xD0 - 0xDF */
	N, N, N, N, N, N, N, N, N, N, N, N, N, N, N, N,
	/* 0xE0 - 0xEF */
	N, N, N, N, N, N, N, N, N, N, N, N, N, N, N, N,
=======
	F(DstMem | SrcReg | ModRM | BitOp | Lock | PageTable, em_btc),
	I(DstReg | SrcMem | ModRM, em_bsf_c),
	I(DstReg | SrcMem | ModRM, em_bsr_c),
	D(DstReg | SrcMem8 | ModRM | Mov), D(DstReg | SrcMem16 | ModRM | Mov),
	/* 0xC0 - 0xC7 */
	F2bv(DstMem | SrcReg | ModRM | SrcWrite | Lock, em_xadd),
	N, ID(0, &instr_dual_0f_c3),
	N, N, N, GD(0, &group9),
	/* 0xC8 - 0xCF */
	X8(I(DstReg, em_bswap)),
	/* 0xD0 - 0xDF */
	N, N, N, N, N, N, N, N, N, N, N, N, N, N, N, N,
	/* 0xE0 - 0xEF */
	N, N, N, N, N, N, N, GP(SrcReg | DstMem | ModRM | Mov, &pfx_0f_e7),
	N, N, N, N, N, N, N, N,
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	/* 0xF0 - 0xFF */
	N, N, N, N, N, N, N, N, N, N, N, N, N, N, N, N
};

<<<<<<< HEAD
=======
static const struct instr_dual instr_dual_0f_38_f0 = {
	I(DstReg | SrcMem | Mov, em_movbe), N
};

static const struct instr_dual instr_dual_0f_38_f1 = {
	I(DstMem | SrcReg | Mov, em_movbe), N
};

static const struct gprefix three_byte_0f_38_f0 = {
	ID(0, &instr_dual_0f_38_f0), ID(0, &instr_dual_0f_38_f0), N, N
};

static const struct gprefix three_byte_0f_38_f1 = {
	ID(0, &instr_dual_0f_38_f1), ID(0, &instr_dual_0f_38_f1), N, N
};

/*
 * Insns below are selected by the prefix which indexed by the third opcode
 * byte.
 */
static const struct opcode opcode_map_0f_38[256] = {
	/* 0x00 - 0x7f */
	X16(N), X16(N), X16(N), X16(N), X16(N), X16(N), X16(N), X16(N),
	/* 0x80 - 0xef */
	X16(N), X16(N), X16(N), X16(N), X16(N), X16(N), X16(N),
	/* 0xf0 - 0xf1 */
	GP(EmulateOnUD | ModRM, &three_byte_0f_38_f0),
	GP(EmulateOnUD | ModRM, &three_byte_0f_38_f1),
	/* 0xf2 - 0xff */
	N, N, X4(N), X8(N)
};

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#undef D
#undef N
#undef G
#undef GD
#undef I
#undef GP
#undef EXT
<<<<<<< HEAD
=======
#undef MD
#undef ID
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

#undef D2bv
#undef D2bvIP
#undef I2bv
#undef I2bvIP
#undef I6ALU

static unsigned imm_size(struct x86_emulate_ctxt *ctxt)
{
	unsigned size;

	size = (ctxt->d & ByteOp) ? 1 : ctxt->op_bytes;
	if (size == 8)
		size = 4;
	return size;
}

static int decode_imm(struct x86_emulate_ctxt *ctxt, struct operand *op,
		      unsigned size, bool sign_extension)
{
	int rc = X86EMUL_CONTINUE;

	op->type = OP_IMM;
	op->bytes = size;
	op->addr.mem.ea = ctxt->_eip;
	/* NB. Immediates are sign-extended as necessary. */
	switch (op->bytes) {
	case 1:
		op->val = insn_fetch(s8, ctxt);
		break;
	case 2:
		op->val = insn_fetch(s16, ctxt);
		break;
	case 4:
		op->val = insn_fetch(s32, ctxt);
		break;
<<<<<<< HEAD
=======
	case 8:
		op->val = insn_fetch(s64, ctxt);
		break;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	}
	if (!sign_extension) {
		switch (op->bytes) {
		case 1:
			op->val &= 0xff;
			break;
		case 2:
			op->val &= 0xffff;
			break;
		case 4:
			op->val &= 0xffffffff;
			break;
		}
	}
done:
	return rc;
}

static int decode_operand(struct x86_emulate_ctxt *ctxt, struct operand *op,
			  unsigned d)
{
	int rc = X86EMUL_CONTINUE;

	switch (d) {
	case OpReg:
		decode_register_operand(ctxt, op);
		break;
	case OpImmUByte:
		rc = decode_imm(ctxt, op, 1, false);
		break;
	case OpMem:
		ctxt->memop.bytes = (ctxt->d & ByteOp) ? 1 : ctxt->op_bytes;
	mem_common:
		*op = ctxt->memop;
		ctxt->memopp = op;
<<<<<<< HEAD
		if ((ctxt->d & BitOp) && op == &ctxt->dst)
=======
		if (ctxt->d & BitOp)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
			fetch_bit_operand(ctxt);
		op->orig_val = op->val;
		break;
	case OpMem64:
<<<<<<< HEAD
		ctxt->memop.bytes = 8;
=======
		ctxt->memop.bytes = (ctxt->op_bytes == 8) ? 16 : 8;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		goto mem_common;
	case OpAcc:
		op->type = OP_REG;
		op->bytes = (ctxt->d & ByteOp) ? 1 : ctxt->op_bytes;
<<<<<<< HEAD
		op->addr.reg = &ctxt->regs[VCPU_REGS_RAX];
=======
		op->addr.reg = reg_rmw(ctxt, VCPU_REGS_RAX);
		fetch_register_operand(op);
		op->orig_val = op->val;
		break;
	case OpAccLo:
		op->type = OP_REG;
		op->bytes = (ctxt->d & ByteOp) ? 2 : ctxt->op_bytes;
		op->addr.reg = reg_rmw(ctxt, VCPU_REGS_RAX);
		fetch_register_operand(op);
		op->orig_val = op->val;
		break;
	case OpAccHi:
		if (ctxt->d & ByteOp) {
			op->type = OP_NONE;
			break;
		}
		op->type = OP_REG;
		op->bytes = ctxt->op_bytes;
		op->addr.reg = reg_rmw(ctxt, VCPU_REGS_RDX);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		fetch_register_operand(op);
		op->orig_val = op->val;
		break;
	case OpDI:
		op->type = OP_MEM;
		op->bytes = (ctxt->d & ByteOp) ? 1 : ctxt->op_bytes;
		op->addr.mem.ea =
<<<<<<< HEAD
			register_address(ctxt, ctxt->regs[VCPU_REGS_RDI]);
		op->addr.mem.seg = VCPU_SREG_ES;
		op->val = 0;
=======
			register_address(ctxt, VCPU_REGS_RDI);
		op->addr.mem.seg = VCPU_SREG_ES;
		op->val = 0;
		op->count = 1;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		break;
	case OpDX:
		op->type = OP_REG;
		op->bytes = 2;
<<<<<<< HEAD
		op->addr.reg = &ctxt->regs[VCPU_REGS_RDX];
		fetch_register_operand(op);
		break;
	case OpCL:
		op->bytes = 1;
		op->val = ctxt->regs[VCPU_REGS_RCX] & 0xff;
=======
		op->addr.reg = reg_rmw(ctxt, VCPU_REGS_RDX);
		fetch_register_operand(op);
		break;
	case OpCL:
		op->type = OP_IMM;
		op->bytes = 1;
		op->val = reg_read(ctxt, VCPU_REGS_RCX) & 0xff;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		break;
	case OpImmByte:
		rc = decode_imm(ctxt, op, 1, true);
		break;
	case OpOne:
<<<<<<< HEAD
=======
		op->type = OP_IMM;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		op->bytes = 1;
		op->val = 1;
		break;
	case OpImm:
		rc = decode_imm(ctxt, op, imm_size(ctxt), true);
		break;
<<<<<<< HEAD
	case OpMem8:
		ctxt->memop.bytes = 1;
=======
	case OpImm64:
		rc = decode_imm(ctxt, op, ctxt->op_bytes, true);
		break;
	case OpMem8:
		ctxt->memop.bytes = 1;
		if (ctxt->memop.type == OP_REG) {
			ctxt->memop.addr.reg = decode_register(ctxt,
					ctxt->modrm_rm, true);
			fetch_register_operand(&ctxt->memop);
		}
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		goto mem_common;
	case OpMem16:
		ctxt->memop.bytes = 2;
		goto mem_common;
	case OpMem32:
		ctxt->memop.bytes = 4;
		goto mem_common;
	case OpImmU16:
		rc = decode_imm(ctxt, op, 2, false);
		break;
	case OpImmU:
		rc = decode_imm(ctxt, op, imm_size(ctxt), false);
		break;
	case OpSI:
		op->type = OP_MEM;
		op->bytes = (ctxt->d & ByteOp) ? 1 : ctxt->op_bytes;
		op->addr.mem.ea =
<<<<<<< HEAD
			register_address(ctxt, ctxt->regs[VCPU_REGS_RSI]);
		op->addr.mem.seg = seg_override(ctxt);
=======
			register_address(ctxt, VCPU_REGS_RSI);
		op->addr.mem.seg = ctxt->seg_override;
		op->val = 0;
		op->count = 1;
		break;
	case OpXLat:
		op->type = OP_MEM;
		op->bytes = (ctxt->d & ByteOp) ? 1 : ctxt->op_bytes;
		op->addr.mem.ea =
			address_mask(ctxt,
				reg_read(ctxt, VCPU_REGS_RBX) +
				(reg_read(ctxt, VCPU_REGS_RAX) & 0xff));
		op->addr.mem.seg = ctxt->seg_override;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		op->val = 0;
		break;
	case OpImmFAddr:
		op->type = OP_IMM;
		op->addr.mem.ea = ctxt->_eip;
		op->bytes = ctxt->op_bytes + 2;
		insn_fetch_arr(op->valptr, op->bytes, ctxt);
		break;
	case OpMemFAddr:
		ctxt->memop.bytes = ctxt->op_bytes + 2;
		goto mem_common;
	case OpES:
<<<<<<< HEAD
		op->val = VCPU_SREG_ES;
		break;
	case OpCS:
		op->val = VCPU_SREG_CS;
		break;
	case OpSS:
		op->val = VCPU_SREG_SS;
		break;
	case OpDS:
		op->val = VCPU_SREG_DS;
		break;
	case OpFS:
		op->val = VCPU_SREG_FS;
		break;
	case OpGS:
=======
		op->type = OP_IMM;
		op->val = VCPU_SREG_ES;
		break;
	case OpCS:
		op->type = OP_IMM;
		op->val = VCPU_SREG_CS;
		break;
	case OpSS:
		op->type = OP_IMM;
		op->val = VCPU_SREG_SS;
		break;
	case OpDS:
		op->type = OP_IMM;
		op->val = VCPU_SREG_DS;
		break;
	case OpFS:
		op->type = OP_IMM;
		op->val = VCPU_SREG_FS;
		break;
	case OpGS:
		op->type = OP_IMM;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		op->val = VCPU_SREG_GS;
		break;
	case OpImplicit:
		/* Special instructions do their own operand decoding. */
	default:
		op->type = OP_NONE; /* Disable writeback. */
		break;
	}

done:
	return rc;
}

<<<<<<< HEAD
int x86_decode_insn(struct x86_emulate_ctxt *ctxt, void *insn, int insn_len)
=======
int x86_decode_insn(struct x86_emulate_ctxt *ctxt, void *insn, int insn_len, int emulation_type)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
{
	int rc = X86EMUL_CONTINUE;
	int mode = ctxt->mode;
	int def_op_bytes, def_ad_bytes, goffset, simd_prefix;
	bool op_prefix = false;
<<<<<<< HEAD
	struct opcode opcode;
=======
	bool has_seg_override = false;
	struct opcode opcode;
	u16 dummy;
	struct desc_struct desc;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	ctxt->memop.type = OP_NONE;
	ctxt->memopp = NULL;
	ctxt->_eip = ctxt->eip;
<<<<<<< HEAD
	ctxt->fetch.start = ctxt->_eip;
	ctxt->fetch.end = ctxt->fetch.start + insn_len;
	if (insn_len > 0)
		memcpy(ctxt->fetch.data, insn, insn_len);
=======
	ctxt->fetch.ptr = ctxt->fetch.data;
	ctxt->fetch.end = ctxt->fetch.data + insn_len;
	ctxt->opcode_len = 1;
	ctxt->intercept = x86_intercept_none;
	if (insn_len > 0)
		memcpy(ctxt->fetch.data, insn, insn_len);
	else {
		rc = __do_insn_fetch_bytes(ctxt, 1);
		if (rc != X86EMUL_CONTINUE)
			goto done;
	}
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	switch (mode) {
	case X86EMUL_MODE_REAL:
	case X86EMUL_MODE_VM86:
<<<<<<< HEAD
=======
		def_op_bytes = def_ad_bytes = 2;
		ctxt->ops->get_segment(ctxt, &dummy, &desc, NULL, VCPU_SREG_CS);
		if (desc.d)
			def_op_bytes = def_ad_bytes = 4;
		break;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	case X86EMUL_MODE_PROT16:
		def_op_bytes = def_ad_bytes = 2;
		break;
	case X86EMUL_MODE_PROT32:
		def_op_bytes = def_ad_bytes = 4;
		break;
#ifdef CONFIG_X86_64
	case X86EMUL_MODE_PROT64:
		def_op_bytes = 4;
		def_ad_bytes = 8;
		break;
#endif
	default:
		return EMULATION_FAILED;
	}

	ctxt->op_bytes = def_op_bytes;
	ctxt->ad_bytes = def_ad_bytes;

	/* Legacy prefixes. */
	for (;;) {
		switch (ctxt->b = insn_fetch(u8, ctxt)) {
		case 0x66:	/* operand-size override */
			op_prefix = true;
			/* switch between 2/4 bytes */
			ctxt->op_bytes = def_op_bytes ^ 6;
			break;
		case 0x67:	/* address-size override */
			if (mode == X86EMUL_MODE_PROT64)
				/* switch between 4/8 bytes */
				ctxt->ad_bytes = def_ad_bytes ^ 12;
			else
				/* switch between 2/4 bytes */
				ctxt->ad_bytes = def_ad_bytes ^ 6;
			break;
		case 0x26:	/* ES override */
<<<<<<< HEAD
		case 0x2e:	/* CS override */
		case 0x36:	/* SS override */
		case 0x3e:	/* DS override */
			set_seg_override(ctxt, (ctxt->b >> 3) & 3);
			break;
		case 0x64:	/* FS override */
		case 0x65:	/* GS override */
			set_seg_override(ctxt, ctxt->b & 7);
=======
			has_seg_override = true;
			ctxt->seg_override = VCPU_SREG_ES;
			break;
		case 0x2e:	/* CS override */
			has_seg_override = true;
			ctxt->seg_override = VCPU_SREG_CS;
			break;
		case 0x36:	/* SS override */
			has_seg_override = true;
			ctxt->seg_override = VCPU_SREG_SS;
			break;
		case 0x3e:	/* DS override */
			has_seg_override = true;
			ctxt->seg_override = VCPU_SREG_DS;
			break;
		case 0x64:	/* FS override */
			has_seg_override = true;
			ctxt->seg_override = VCPU_SREG_FS;
			break;
		case 0x65:	/* GS override */
			has_seg_override = true;
			ctxt->seg_override = VCPU_SREG_GS;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
			break;
		case 0x40 ... 0x4f: /* REX */
			if (mode != X86EMUL_MODE_PROT64)
				goto done_prefixes;
			ctxt->rex_prefix = ctxt->b;
			continue;
		case 0xf0:	/* LOCK */
			ctxt->lock_prefix = 1;
			break;
		case 0xf2:	/* REPNE/REPNZ */
		case 0xf3:	/* REP/REPE/REPZ */
			ctxt->rep_prefix = ctxt->b;
			break;
		default:
			goto done_prefixes;
		}

		/* Any legacy prefix after a REX prefix nullifies its effect. */

		ctxt->rex_prefix = 0;
	}

done_prefixes:

	/* REX prefix. */
	if (ctxt->rex_prefix & 8)
		ctxt->op_bytes = 8;	/* REX.W */

	/* Opcode byte(s). */
	opcode = opcode_table[ctxt->b];
	/* Two-byte opcode? */
	if (ctxt->b == 0x0f) {
<<<<<<< HEAD
		ctxt->twobyte = 1;
		ctxt->b = insn_fetch(u8, ctxt);
		opcode = twobyte_table[ctxt->b];
	}
	ctxt->d = opcode.flags;

	while (ctxt->d & GroupMask) {
		switch (ctxt->d & GroupMask) {
		case Group:
			ctxt->modrm = insn_fetch(u8, ctxt);
			--ctxt->_eip;
=======
		ctxt->opcode_len = 2;
		ctxt->b = insn_fetch(u8, ctxt);
		opcode = twobyte_table[ctxt->b];

		/* 0F_38 opcode map */
		if (ctxt->b == 0x38) {
			ctxt->opcode_len = 3;
			ctxt->b = insn_fetch(u8, ctxt);
			opcode = opcode_map_0f_38[ctxt->b];
		}
	}
	ctxt->d = opcode.flags;

	if (ctxt->d & ModRM)
		ctxt->modrm = insn_fetch(u8, ctxt);

	/* vex-prefix instructions are not implemented */
	if (ctxt->opcode_len == 1 && (ctxt->b == 0xc5 || ctxt->b == 0xc4) &&
	    (mode == X86EMUL_MODE_PROT64 || (ctxt->modrm & 0xc0) == 0xc0)) {
		ctxt->d = NotImpl;
	}

	while (ctxt->d & GroupMask) {
		switch (ctxt->d & GroupMask) {
		case Group:
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
			goffset = (ctxt->modrm >> 3) & 7;
			opcode = opcode.u.group[goffset];
			break;
		case GroupDual:
<<<<<<< HEAD
			ctxt->modrm = insn_fetch(u8, ctxt);
			--ctxt->_eip;
=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
			goffset = (ctxt->modrm >> 3) & 7;
			if ((ctxt->modrm >> 6) == 3)
				opcode = opcode.u.gdual->mod3[goffset];
			else
				opcode = opcode.u.gdual->mod012[goffset];
			break;
		case RMExt:
			goffset = ctxt->modrm & 7;
			opcode = opcode.u.group[goffset];
			break;
		case Prefix:
			if (ctxt->rep_prefix && op_prefix)
				return EMULATION_FAILED;
			simd_prefix = op_prefix ? 0x66 : ctxt->rep_prefix;
			switch (simd_prefix) {
			case 0x00: opcode = opcode.u.gprefix->pfx_no; break;
			case 0x66: opcode = opcode.u.gprefix->pfx_66; break;
			case 0xf2: opcode = opcode.u.gprefix->pfx_f2; break;
			case 0xf3: opcode = opcode.u.gprefix->pfx_f3; break;
			}
			break;
<<<<<<< HEAD
=======
		case Escape:
			if (ctxt->modrm > 0xbf) {
				size_t size = ARRAY_SIZE(opcode.u.esc->high);
				u32 index = array_index_nospec(
					ctxt->modrm - 0xc0, size);

				opcode = opcode.u.esc->high[index];
			} else {
				opcode = opcode.u.esc->op[(ctxt->modrm >> 3) & 7];
			}
			break;
		case InstrDual:
			if ((ctxt->modrm >> 6) == 3)
				opcode = opcode.u.idual->mod3;
			else
				opcode = opcode.u.idual->mod012;
			break;
		case ModeDual:
			if (ctxt->mode == X86EMUL_MODE_PROT64)
				opcode = opcode.u.mdual->mode64;
			else
				opcode = opcode.u.mdual->mode32;
			break;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		default:
			return EMULATION_FAILED;
		}

		ctxt->d &= ~(u64)GroupMask;
		ctxt->d |= opcode.flags;
	}

<<<<<<< HEAD
	ctxt->execute = opcode.u.execute;
	ctxt->check_perm = opcode.check_perm;
	ctxt->intercept = opcode.intercept;

	/* Unrecognised? */
	if (ctxt->d == 0 || (ctxt->d & Undefined))
		return EMULATION_FAILED;

	if (!(ctxt->d & VendorSpecific) && ctxt->only_vendor_specific_insn)
		return EMULATION_FAILED;

	if (mode == X86EMUL_MODE_PROT64 && (ctxt->d & Stack))
		ctxt->op_bytes = 8;

	if (ctxt->d & Op3264) {
		if (mode == X86EMUL_MODE_PROT64)
			ctxt->op_bytes = 8;
		else
			ctxt->op_bytes = 4;
	}

	if (ctxt->d & Sse)
		ctxt->op_bytes = 16;
=======
	ctxt->is_branch = opcode.flags & IsBranch;

	/* Unrecognised? */
	if (ctxt->d == 0)
		return EMULATION_FAILED;

	ctxt->execute = opcode.u.execute;

	if (unlikely(emulation_type & EMULTYPE_TRAP_UD) &&
	    likely(!(ctxt->d & EmulateOnUD)))
		return EMULATION_FAILED;

	if (unlikely(ctxt->d &
	    (NotImpl|Stack|Op3264|Sse|Mmx|Intercept|CheckPerm|NearBranch|
	     No16))) {
		/*
		 * These are copied unconditionally here, and checked unconditionally
		 * in x86_emulate_insn.
		 */
		ctxt->check_perm = opcode.check_perm;
		ctxt->intercept = opcode.intercept;

		if (ctxt->d & NotImpl)
			return EMULATION_FAILED;

		if (mode == X86EMUL_MODE_PROT64) {
			if (ctxt->op_bytes == 4 && (ctxt->d & Stack))
				ctxt->op_bytes = 8;
			else if (ctxt->d & NearBranch)
				ctxt->op_bytes = 8;
		}

		if (ctxt->d & Op3264) {
			if (mode == X86EMUL_MODE_PROT64)
				ctxt->op_bytes = 8;
			else
				ctxt->op_bytes = 4;
		}

		if ((ctxt->d & No16) && ctxt->op_bytes == 2)
			ctxt->op_bytes = 4;

		if (ctxt->d & Sse)
			ctxt->op_bytes = 16;
		else if (ctxt->d & Mmx)
			ctxt->op_bytes = 8;
	}
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	/* ModRM and SIB bytes. */
	if (ctxt->d & ModRM) {
		rc = decode_modrm(ctxt, &ctxt->memop);
<<<<<<< HEAD
		if (!ctxt->has_seg_override)
			set_seg_override(ctxt, ctxt->modrm_seg);
=======
		if (!has_seg_override) {
			has_seg_override = true;
			ctxt->seg_override = ctxt->modrm_seg;
		}
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	} else if (ctxt->d & MemAbs)
		rc = decode_abs(ctxt, &ctxt->memop);
	if (rc != X86EMUL_CONTINUE)
		goto done;

<<<<<<< HEAD
	if (!ctxt->has_seg_override)
		set_seg_override(ctxt, VCPU_SREG_DS);

	ctxt->memop.addr.mem.seg = seg_override(ctxt);

	if (ctxt->memop.type == OP_MEM && ctxt->ad_bytes != 8)
		ctxt->memop.addr.mem.ea = (u32)ctxt->memop.addr.mem.ea;
=======
	if (!has_seg_override)
		ctxt->seg_override = VCPU_SREG_DS;

	ctxt->memop.addr.mem.seg = ctxt->seg_override;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	/*
	 * Decode and fetch the source operand: register, memory
	 * or immediate.
	 */
	rc = decode_operand(ctxt, &ctxt->src, (ctxt->d >> SrcShift) & OpMask);
	if (rc != X86EMUL_CONTINUE)
		goto done;

	/*
	 * Decode and fetch the second source operand: register, memory
	 * or immediate.
	 */
	rc = decode_operand(ctxt, &ctxt->src2, (ctxt->d >> Src2Shift) & OpMask);
	if (rc != X86EMUL_CONTINUE)
		goto done;

	/* Decode and fetch the destination operand: register or memory. */
	rc = decode_operand(ctxt, &ctxt->dst, (ctxt->d >> DstShift) & OpMask);

<<<<<<< HEAD
done:
	if (ctxt->memopp && ctxt->memopp->type == OP_MEM && ctxt->rip_relative)
		ctxt->memopp->addr.mem.ea += ctxt->_eip;

=======
	if (ctxt->rip_relative && likely(ctxt->memopp))
		ctxt->memopp->addr.mem.ea = address_mask(ctxt,
					ctxt->memopp->addr.mem.ea + ctxt->_eip);

done:
	if (rc == X86EMUL_PROPAGATE_FAULT)
		ctxt->have_exception = true;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	return (rc != X86EMUL_CONTINUE) ? EMULATION_FAILED : EMULATION_OK;
}

bool x86_page_table_writing_insn(struct x86_emulate_ctxt *ctxt)
{
	return ctxt->d & PageTable;
}

static bool string_insn_completed(struct x86_emulate_ctxt *ctxt)
{
	/* The second termination condition only applies for REPE
	 * and REPNE. Test if the repeat string operation prefix is
	 * REPE/REPZ or REPNE/REPNZ and if it's the case it tests the
	 * corresponding termination condition according to:
	 * 	- if REPE/REPZ and ZF = 0 then done
	 * 	- if REPNE/REPNZ and ZF = 1 then done
	 */
	if (((ctxt->b == 0xa6) || (ctxt->b == 0xa7) ||
	     (ctxt->b == 0xae) || (ctxt->b == 0xaf))
	    && (((ctxt->rep_prefix == REPE_PREFIX) &&
<<<<<<< HEAD
		 ((ctxt->eflags & EFLG_ZF) == 0))
		|| ((ctxt->rep_prefix == REPNE_PREFIX) &&
		    ((ctxt->eflags & EFLG_ZF) == EFLG_ZF))))
=======
		 ((ctxt->eflags & X86_EFLAGS_ZF) == 0))
		|| ((ctxt->rep_prefix == REPNE_PREFIX) &&
		    ((ctxt->eflags & X86_EFLAGS_ZF) == X86_EFLAGS_ZF))))
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		return true;

	return false;
}

<<<<<<< HEAD
int x86_emulate_insn(struct x86_emulate_ctxt *ctxt)
{
	struct x86_emulate_ops *ops = ctxt->ops;
	int rc = X86EMUL_CONTINUE;
	int saved_dst_type = ctxt->dst.type;

	ctxt->mem_read.pos = 0;

	if (ctxt->mode == X86EMUL_MODE_PROT64 && (ctxt->d & No64)) {
		rc = emulate_ud(ctxt);
		goto done;
	}

=======
static int flush_pending_x87_faults(struct x86_emulate_ctxt *ctxt)
{
	int rc;

	kvm_fpu_get();
	rc = asm_safe("fwait");
	kvm_fpu_put();

	if (unlikely(rc != X86EMUL_CONTINUE))
		return emulate_exception(ctxt, MF_VECTOR, 0, false);

	return X86EMUL_CONTINUE;
}

static void fetch_possible_mmx_operand(struct operand *op)
{
	if (op->type == OP_MM)
		kvm_read_mmx_reg(op->addr.mm, &op->mm_val);
}

static int fastop(struct x86_emulate_ctxt *ctxt, fastop_t fop)
{
	ulong flags = (ctxt->eflags & EFLAGS_MASK) | X86_EFLAGS_IF;

	if (!(ctxt->d & ByteOp))
		fop += __ffs(ctxt->dst.bytes) * FASTOP_SIZE;

	asm("push %[flags]; popf; " CALL_NOSPEC " ; pushf; pop %[flags]\n"
	    : "+a"(ctxt->dst.val), "+d"(ctxt->src.val), [flags]"+D"(flags),
	      [thunk_target]"+S"(fop), ASM_CALL_CONSTRAINT
	    : "c"(ctxt->src2.val));

	ctxt->eflags = (ctxt->eflags & ~EFLAGS_MASK) | (flags & EFLAGS_MASK);
	if (!fop) /* exception is returned in fop variable */
		return emulate_de(ctxt);
	return X86EMUL_CONTINUE;
}

void init_decode_cache(struct x86_emulate_ctxt *ctxt)
{
	/* Clear fields that are set conditionally but read without a guard. */
	ctxt->rip_relative = false;
	ctxt->rex_prefix = 0;
	ctxt->lock_prefix = 0;
	ctxt->rep_prefix = 0;
	ctxt->regs_valid = 0;
	ctxt->regs_dirty = 0;

	ctxt->io_read.pos = 0;
	ctxt->io_read.end = 0;
	ctxt->mem_read.end = 0;
}

int x86_emulate_insn(struct x86_emulate_ctxt *ctxt)
{
	const struct x86_emulate_ops *ops = ctxt->ops;
	int rc = X86EMUL_CONTINUE;
	int saved_dst_type = ctxt->dst.type;
	bool is_guest_mode = ctxt->ops->is_guest_mode(ctxt);

	ctxt->mem_read.pos = 0;

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	/* LOCK prefix is allowed only with some instructions */
	if (ctxt->lock_prefix && (!(ctxt->d & Lock) || ctxt->dst.type != OP_MEM)) {
		rc = emulate_ud(ctxt);
		goto done;
	}

	if ((ctxt->d & SrcMask) == SrcMemFAddr && ctxt->src.type != OP_MEM) {
		rc = emulate_ud(ctxt);
		goto done;
	}

<<<<<<< HEAD
	if ((ctxt->d & Sse)
	    && ((ops->get_cr(ctxt, 0) & X86_CR0_EM)
		|| !(ops->get_cr(ctxt, 4) & X86_CR4_OSFXSR))) {
		rc = emulate_ud(ctxt);
		goto done;
	}

	if ((ctxt->d & Sse) && (ops->get_cr(ctxt, 0) & X86_CR0_TS)) {
		rc = emulate_nm(ctxt);
		goto done;
	}

	if (unlikely(ctxt->guest_mode) && ctxt->intercept) {
		rc = emulator_check_intercept(ctxt, ctxt->intercept,
					      X86_ICPT_PRE_EXCEPT);
		if (rc != X86EMUL_CONTINUE)
			goto done;
	}

	/* Privileged instruction can be executed only in CPL=0 */
	if ((ctxt->d & Priv) && ops->cpl(ctxt)) {
		rc = emulate_gp(ctxt, 0);
		goto done;
	}

	/* Instruction can only be executed in protected mode */
	if ((ctxt->d & Prot) && !(ctxt->mode & X86EMUL_MODE_PROT)) {
		rc = emulate_ud(ctxt);
		goto done;
	}

	/* Do instruction specific permission checks */
	if (ctxt->check_perm) {
		rc = ctxt->check_perm(ctxt);
		if (rc != X86EMUL_CONTINUE)
			goto done;
	}

	if (unlikely(ctxt->guest_mode) && ctxt->intercept) {
		rc = emulator_check_intercept(ctxt, ctxt->intercept,
					      X86_ICPT_POST_EXCEPT);
		if (rc != X86EMUL_CONTINUE)
			goto done;
	}

	if (ctxt->rep_prefix && (ctxt->d & String)) {
		/* All REP prefixes have the same first termination condition */
		if (address_mask(ctxt, ctxt->regs[VCPU_REGS_RCX]) == 0) {
			ctxt->eip = ctxt->_eip;
			goto done;
		}
=======
	if (unlikely(ctxt->d &
		     (No64|Undefined|Sse|Mmx|Intercept|CheckPerm|Priv|Prot|String))) {
		if ((ctxt->mode == X86EMUL_MODE_PROT64 && (ctxt->d & No64)) ||
				(ctxt->d & Undefined)) {
			rc = emulate_ud(ctxt);
			goto done;
		}

		if (((ctxt->d & (Sse|Mmx)) && ((ops->get_cr(ctxt, 0) & X86_CR0_EM)))
		    || ((ctxt->d & Sse) && !(ops->get_cr(ctxt, 4) & X86_CR4_OSFXSR))) {
			rc = emulate_ud(ctxt);
			goto done;
		}

		if ((ctxt->d & (Sse|Mmx)) && (ops->get_cr(ctxt, 0) & X86_CR0_TS)) {
			rc = emulate_nm(ctxt);
			goto done;
		}

		if (ctxt->d & Mmx) {
			rc = flush_pending_x87_faults(ctxt);
			if (rc != X86EMUL_CONTINUE)
				goto done;
			/*
			 * Now that we know the fpu is exception safe, we can fetch
			 * operands from it.
			 */
			fetch_possible_mmx_operand(&ctxt->src);
			fetch_possible_mmx_operand(&ctxt->src2);
			if (!(ctxt->d & Mov))
				fetch_possible_mmx_operand(&ctxt->dst);
		}

		if (unlikely(is_guest_mode) && ctxt->intercept) {
			rc = emulator_check_intercept(ctxt, ctxt->intercept,
						      X86_ICPT_PRE_EXCEPT);
			if (rc != X86EMUL_CONTINUE)
				goto done;
		}

		/* Instruction can only be executed in protected mode */
		if ((ctxt->d & Prot) && ctxt->mode < X86EMUL_MODE_PROT16) {
			rc = emulate_ud(ctxt);
			goto done;
		}

		/* Privileged instruction can be executed only in CPL=0 */
		if ((ctxt->d & Priv) && ops->cpl(ctxt)) {
			if (ctxt->d & PrivUD)
				rc = emulate_ud(ctxt);
			else
				rc = emulate_gp(ctxt, 0);
			goto done;
		}

		/* Do instruction specific permission checks */
		if (ctxt->d & CheckPerm) {
			rc = ctxt->check_perm(ctxt);
			if (rc != X86EMUL_CONTINUE)
				goto done;
		}

		if (unlikely(is_guest_mode) && (ctxt->d & Intercept)) {
			rc = emulator_check_intercept(ctxt, ctxt->intercept,
						      X86_ICPT_POST_EXCEPT);
			if (rc != X86EMUL_CONTINUE)
				goto done;
		}

		if (ctxt->rep_prefix && (ctxt->d & String)) {
			/* All REP prefixes have the same first termination condition */
			if (address_mask(ctxt, reg_read(ctxt, VCPU_REGS_RCX)) == 0) {
				string_registers_quirk(ctxt);
				ctxt->eip = ctxt->_eip;
				ctxt->eflags &= ~X86_EFLAGS_RF;
				goto done;
			}
		}
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	}

	if ((ctxt->src.type == OP_MEM) && !(ctxt->d & NoAccess)) {
		rc = segmented_read(ctxt, ctxt->src.addr.mem,
				    ctxt->src.valptr, ctxt->src.bytes);
		if (rc != X86EMUL_CONTINUE)
			goto done;
		ctxt->src.orig_val64 = ctxt->src.val64;
	}

	if (ctxt->src2.type == OP_MEM) {
		rc = segmented_read(ctxt, ctxt->src2.addr.mem,
				    &ctxt->src2.val, ctxt->src2.bytes);
		if (rc != X86EMUL_CONTINUE)
			goto done;
	}

	if ((ctxt->d & DstMask) == ImplicitOps)
		goto special_insn;


	if ((ctxt->dst.type == OP_MEM) && !(ctxt->d & Mov)) {
		/* optimisation - avoid slow emulated read if Mov */
		rc = segmented_read(ctxt, ctxt->dst.addr.mem,
				   &ctxt->dst.val, ctxt->dst.bytes);
<<<<<<< HEAD
		if (rc != X86EMUL_CONTINUE)
			goto done;
=======
		if (rc != X86EMUL_CONTINUE) {
			if (!(ctxt->d & NoWrite) &&
			    rc == X86EMUL_PROPAGATE_FAULT &&
			    ctxt->exception.vector == PF_VECTOR)
				ctxt->exception.error_code |= PFERR_WRITE_MASK;
			goto done;
		}
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	}
	/* Copy full 64-bit value for CMPXCHG8B.  */
	ctxt->dst.orig_val64 = ctxt->dst.val64;

special_insn:

<<<<<<< HEAD
	if (unlikely(ctxt->guest_mode) && ctxt->intercept) {
=======
	if (unlikely(is_guest_mode) && (ctxt->d & Intercept)) {
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		rc = emulator_check_intercept(ctxt, ctxt->intercept,
					      X86_ICPT_POST_MEMACCESS);
		if (rc != X86EMUL_CONTINUE)
			goto done;
	}

<<<<<<< HEAD
	if (ctxt->execute) {
		rc = ctxt->execute(ctxt);
=======
	if (ctxt->rep_prefix && (ctxt->d & String))
		ctxt->eflags |= X86_EFLAGS_RF;
	else
		ctxt->eflags &= ~X86_EFLAGS_RF;

	if (ctxt->execute) {
		if (ctxt->d & Fastop)
			rc = fastop(ctxt, ctxt->fop);
		else
			rc = ctxt->execute(ctxt);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		if (rc != X86EMUL_CONTINUE)
			goto done;
		goto writeback;
	}

<<<<<<< HEAD
	if (ctxt->twobyte)
		goto twobyte_insn;

	switch (ctxt->b) {
	case 0x40 ... 0x47: /* inc r16/r32 */
		emulate_1op(ctxt, "inc");
		break;
	case 0x48 ... 0x4f: /* dec r16/r32 */
		emulate_1op(ctxt, "dec");
		break;
	case 0x63:		/* movsxd */
		if (ctxt->mode != X86EMUL_MODE_PROT64)
			goto cannot_emulate;
		ctxt->dst.val = (s32) ctxt->src.val;
		break;
=======
	if (ctxt->opcode_len == 2)
		goto twobyte_insn;
	else if (ctxt->opcode_len == 3)
		goto threebyte_insn;

	switch (ctxt->b) {
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	case 0x70 ... 0x7f: /* jcc (short) */
		if (test_cc(ctxt->b, ctxt->eflags))
			rc = jmp_rel(ctxt, ctxt->src.val);
		break;
	case 0x8d: /* lea r16/r32, m */
		ctxt->dst.val = ctxt->src.addr.mem.ea;
		break;
	case 0x90 ... 0x97: /* nop / xchg reg, rax */
<<<<<<< HEAD
		if (ctxt->dst.addr.reg == &ctxt->regs[VCPU_REGS_RAX])
			break;
		rc = em_xchg(ctxt);
=======
		if (ctxt->dst.addr.reg == reg_rmw(ctxt, VCPU_REGS_RAX))
			ctxt->dst.type = OP_NONE;
		else
			rc = em_xchg(ctxt);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		break;
	case 0x98: /* cbw/cwde/cdqe */
		switch (ctxt->op_bytes) {
		case 2: ctxt->dst.val = (s8)ctxt->dst.val; break;
		case 4: ctxt->dst.val = (s16)ctxt->dst.val; break;
		case 8: ctxt->dst.val = (s32)ctxt->dst.val; break;
		}
		break;
<<<<<<< HEAD
	case 0xc0 ... 0xc1:
		rc = em_grp2(ctxt);
		break;
=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	case 0xcc:		/* int3 */
		rc = emulate_int(ctxt, 3);
		break;
	case 0xcd:		/* int n */
		rc = emulate_int(ctxt, ctxt->src.val);
		break;
	case 0xce:		/* into */
<<<<<<< HEAD
		if (ctxt->eflags & EFLG_OF)
			rc = emulate_int(ctxt, 4);
		break;
	case 0xd0 ... 0xd1:	/* Grp2 */
		rc = em_grp2(ctxt);
		break;
	case 0xd2 ... 0xd3:	/* Grp2 */
		ctxt->src.val = ctxt->regs[VCPU_REGS_RCX];
		rc = em_grp2(ctxt);
		break;
=======
		if (ctxt->eflags & X86_EFLAGS_OF)
			rc = emulate_int(ctxt, 4);
		break;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	case 0xe9: /* jmp rel */
	case 0xeb: /* jmp rel short */
		rc = jmp_rel(ctxt, ctxt->src.val);
		ctxt->dst.type = OP_NONE; /* Disable writeback. */
		break;
	case 0xf4:              /* hlt */
		ctxt->ops->halt(ctxt);
		break;
	case 0xf5:	/* cmc */
		/* complement carry flag from eflags reg */
<<<<<<< HEAD
		ctxt->eflags ^= EFLG_CF;
		break;
	case 0xf8: /* clc */
		ctxt->eflags &= ~EFLG_CF;
		break;
	case 0xf9: /* stc */
		ctxt->eflags |= EFLG_CF;
		break;
	case 0xfc: /* cld */
		ctxt->eflags &= ~EFLG_DF;
		break;
	case 0xfd: /* std */
		ctxt->eflags |= EFLG_DF;
=======
		ctxt->eflags ^= X86_EFLAGS_CF;
		break;
	case 0xf8: /* clc */
		ctxt->eflags &= ~X86_EFLAGS_CF;
		break;
	case 0xf9: /* stc */
		ctxt->eflags |= X86_EFLAGS_CF;
		break;
	case 0xfc: /* cld */
		ctxt->eflags &= ~X86_EFLAGS_DF;
		break;
	case 0xfd: /* std */
		ctxt->eflags |= X86_EFLAGS_DF;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		break;
	default:
		goto cannot_emulate;
	}

	if (rc != X86EMUL_CONTINUE)
		goto done;

writeback:
<<<<<<< HEAD
	rc = writeback(ctxt);
	if (rc != X86EMUL_CONTINUE)
		goto done;
=======
	if (ctxt->d & SrcWrite) {
		BUG_ON(ctxt->src.type == OP_MEM || ctxt->src.type == OP_MEM_STR);
		rc = writeback(ctxt, &ctxt->src);
		if (rc != X86EMUL_CONTINUE)
			goto done;
	}
	if (!(ctxt->d & NoWrite)) {
		rc = writeback(ctxt, &ctxt->dst);
		if (rc != X86EMUL_CONTINUE)
			goto done;
	}
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	/*
	 * restore dst type in case the decoding will be reused
	 * (happens for string instruction )
	 */
	ctxt->dst.type = saved_dst_type;

	if ((ctxt->d & SrcMask) == SrcSI)
<<<<<<< HEAD
		string_addr_inc(ctxt, seg_override(ctxt),
				VCPU_REGS_RSI, &ctxt->src);

	if ((ctxt->d & DstMask) == DstDI)
		string_addr_inc(ctxt, VCPU_SREG_ES, VCPU_REGS_RDI,
				&ctxt->dst);

	if (ctxt->rep_prefix && (ctxt->d & String)) {
		struct read_cache *r = &ctxt->io_read;
		register_address_increment(ctxt, &ctxt->regs[VCPU_REGS_RCX], -1);
=======
		string_addr_inc(ctxt, VCPU_REGS_RSI, &ctxt->src);

	if ((ctxt->d & DstMask) == DstDI)
		string_addr_inc(ctxt, VCPU_REGS_RDI, &ctxt->dst);

	if (ctxt->rep_prefix && (ctxt->d & String)) {
		unsigned int count;
		struct read_cache *r = &ctxt->io_read;
		if ((ctxt->d & SrcMask) == SrcSI)
			count = ctxt->src.count;
		else
			count = ctxt->dst.count;
		register_address_increment(ctxt, VCPU_REGS_RCX, -count);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

		if (!string_insn_completed(ctxt)) {
			/*
			 * Re-enter guest when pio read ahead buffer is empty
			 * or, if it is not used, after each 1024 iteration.
			 */
<<<<<<< HEAD
			if ((r->end != 0 || ctxt->regs[VCPU_REGS_RCX] & 0x3ff) &&
=======
			if ((r->end != 0 || reg_read(ctxt, VCPU_REGS_RCX) & 0x3ff) &&
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
			    (r->end == 0 || r->end != r->pos)) {
				/*
				 * Reset read cache. Usually happens before
				 * decode, but since instruction is restarted
				 * we have to do it here.
				 */
				ctxt->mem_read.end = 0;
<<<<<<< HEAD
=======
				writeback_registers(ctxt);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
				return EMULATION_RESTART;
			}
			goto done; /* skip rip writeback */
		}
<<<<<<< HEAD
	}

	ctxt->eip = ctxt->_eip;

done:
	if (rc == X86EMUL_PROPAGATE_FAULT)
		ctxt->have_exception = true;
	if (rc == X86EMUL_INTERCEPTED)
		return EMULATION_INTERCEPTED;

=======
		ctxt->eflags &= ~X86_EFLAGS_RF;
	}

	ctxt->eip = ctxt->_eip;
	if (ctxt->mode != X86EMUL_MODE_PROT64)
		ctxt->eip = (u32)ctxt->_eip;

done:
	if (rc == X86EMUL_PROPAGATE_FAULT) {
		if (KVM_EMULATOR_BUG_ON(ctxt->exception.vector > 0x1f, ctxt))
			return EMULATION_FAILED;
		ctxt->have_exception = true;
	}
	if (rc == X86EMUL_INTERCEPTED)
		return EMULATION_INTERCEPTED;

	if (rc == X86EMUL_CONTINUE)
		writeback_registers(ctxt);

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	return (rc == X86EMUL_UNHANDLEABLE) ? EMULATION_FAILED : EMULATION_OK;

twobyte_insn:
	switch (ctxt->b) {
	case 0x09:		/* wbinvd */
		(ctxt->ops->wbinvd)(ctxt);
		break;
	case 0x08:		/* invd */
	case 0x0d:		/* GrpP (prefetch) */
	case 0x18:		/* Grp16 (prefetch/nop) */
<<<<<<< HEAD
=======
	case 0x1f:		/* nop */
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		break;
	case 0x20: /* mov cr, reg */
		ctxt->dst.val = ops->get_cr(ctxt, ctxt->modrm_reg);
		break;
	case 0x21: /* mov from dr to reg */
<<<<<<< HEAD
		ops->get_dr(ctxt, ctxt->modrm_reg, &ctxt->dst.val);
		break;
	case 0x40 ... 0x4f:	/* cmov */
		ctxt->dst.val = ctxt->dst.orig_val = ctxt->src.val;
		if (!test_cc(ctxt->b, ctxt->eflags))
=======
		ctxt->dst.val = ops->get_dr(ctxt, ctxt->modrm_reg);
		break;
	case 0x40 ... 0x4f:	/* cmov */
		if (test_cc(ctxt->b, ctxt->eflags))
			ctxt->dst.val = ctxt->src.val;
		else if (ctxt->op_bytes != 4)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
			ctxt->dst.type = OP_NONE; /* no writeback */
		break;
	case 0x80 ... 0x8f: /* jnz rel, etc*/
		if (test_cc(ctxt->b, ctxt->eflags))
			rc = jmp_rel(ctxt, ctxt->src.val);
		break;
	case 0x90 ... 0x9f:     /* setcc r/m8 */
		ctxt->dst.val = test_cc(ctxt->b, ctxt->eflags);
		break;
<<<<<<< HEAD
	case 0xa4: /* shld imm8, r, r/m */
	case 0xa5: /* shld cl, r, r/m */
		emulate_2op_cl(ctxt, "shld");
		break;
	case 0xac: /* shrd imm8, r, r/m */
	case 0xad: /* shrd cl, r, r/m */
		emulate_2op_cl(ctxt, "shrd");
		break;
	case 0xae:              /* clflush */
		break;
	case 0xb6 ... 0xb7:	/* movzx */
		ctxt->dst.bytes = ctxt->op_bytes;
		ctxt->dst.val = (ctxt->d & ByteOp) ? (u8) ctxt->src.val
=======
	case 0xb6 ... 0xb7:	/* movzx */
		ctxt->dst.bytes = ctxt->op_bytes;
		ctxt->dst.val = (ctxt->src.bytes == 1) ? (u8) ctxt->src.val
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
						       : (u16) ctxt->src.val;
		break;
	case 0xbe ... 0xbf:	/* movsx */
		ctxt->dst.bytes = ctxt->op_bytes;
<<<<<<< HEAD
		ctxt->dst.val = (ctxt->d & ByteOp) ? (s8) ctxt->src.val :
							(s16) ctxt->src.val;
		break;
	case 0xc0 ... 0xc1:	/* xadd */
		emulate_2op_SrcV(ctxt, "add");
		/* Write back the register source. */
		ctxt->src.val = ctxt->dst.orig_val;
		write_register_operand(&ctxt->src);
		break;
	case 0xc3:		/* movnti */
		ctxt->dst.bytes = ctxt->op_bytes;
		ctxt->dst.val = (ctxt->op_bytes == 4) ? (u32) ctxt->src.val :
							(u64) ctxt->src.val;
		break;
=======
		ctxt->dst.val = (ctxt->src.bytes == 1) ? (s8) ctxt->src.val :
							(s16) ctxt->src.val;
		break;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	default:
		goto cannot_emulate;
	}

<<<<<<< HEAD
=======
threebyte_insn:

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	if (rc != X86EMUL_CONTINUE)
		goto done;

	goto writeback;

cannot_emulate:
	return EMULATION_FAILED;
}
<<<<<<< HEAD
=======

void emulator_invalidate_register_cache(struct x86_emulate_ctxt *ctxt)
{
	invalidate_registers(ctxt);
}

void emulator_writeback_register_cache(struct x86_emulate_ctxt *ctxt)
{
	writeback_registers(ctxt);
}

bool emulator_can_use_gpa(struct x86_emulate_ctxt *ctxt)
{
	if (ctxt->rep_prefix && (ctxt->d & String))
		return false;

	if (ctxt->d & TwoMemOp)
		return false;

	return true;
}
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
