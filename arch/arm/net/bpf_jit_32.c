<<<<<<< HEAD
/*
 * Just-In-Time compiler for BPF filters on 32bit ARM
 *
 * Copyright (c) 2011 Mircea Gherzan <mgherzan@gmail.com>
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the
 * Free Software Foundation; version 2 of the License.
 */

=======
// SPDX-License-Identifier: GPL-2.0-only
/*
 * Just-In-Time compiler for eBPF filters on 32bit ARM
 *
 * Copyright (c) 2023 Puranjay Mohan <puranjay12@gmail.com>
 * Copyright (c) 2017 Shubham Bansal <illusionist.neo@gmail.com>
 * Copyright (c) 2011 Mircea Gherzan <mgherzan@gmail.com>
 */

#include <linux/bpf.h>
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#include <linux/bitops.h>
#include <linux/compiler.h>
#include <linux/errno.h>
#include <linux/filter.h>
<<<<<<< HEAD
#include <linux/moduleloader.h>
#include <linux/netdevice.h>
#include <linux/string.h>
#include <linux/slab.h>
#include <asm/cacheflush.h>
#include <asm/hwcap.h>
=======
#include <linux/netdevice.h>
#include <linux/string.h>
#include <linux/slab.h>
#include <linux/if_vlan.h>
#include <linux/math64.h>

#include <asm/cacheflush.h>
#include <asm/hwcap.h>
#include <asm/opcodes.h>
#include <asm/system_info.h>
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

#include "bpf_jit_32.h"

/*
<<<<<<< HEAD
 * ABI:
 *
 * r0	scratch register
 * r4	BPF register A
 * r5	BPF register X
 * r6	pointer to the skb
 * r7	skb->data
 * r8	skb_headlen(skb)
 */

#define r_scratch	ARM_R0
/* r1-r3 are (also) used for the unaligned loads on the non-ARMv7 slowpath */
#define r_off		ARM_R1
#define r_A		ARM_R4
#define r_X		ARM_R5
#define r_skb		ARM_R6
#define r_skb_data	ARM_R7
#define r_skb_hl	ARM_R8

#define SCRATCH_SP_OFFSET	0
#define SCRATCH_OFF(k)		(SCRATCH_SP_OFFSET + (k))

#define SEEN_MEM		((1 << BPF_MEMWORDS) - 1)
#define SEEN_MEM_WORD(k)	(1 << (k))
#define SEEN_X			(1 << BPF_MEMWORDS)
#define SEEN_CALL		(1 << (BPF_MEMWORDS + 1))
#define SEEN_SKB		(1 << (BPF_MEMWORDS + 2))
#define SEEN_DATA		(1 << (BPF_MEMWORDS + 3))

#define FLAG_NEED_X_RESET	(1 << 0)
#define FLAG_IMM_OVERFLOW	(1 << 1)

struct jit_ctx {
	const struct sk_filter *skf;
	unsigned idx;
	unsigned prologue_bytes;
	int ret0_fp_idx;
	u32 seen;
	u32 flags;
	u32 *offsets;
	u32 *target;
=======
 * eBPF prog stack layout:
 *
 *                         high
 * original ARM_SP =>     +-----+
 *                        |     | callee saved registers
 *                        +-----+ <= (BPF_FP + SCRATCH_SIZE)
 *                        | ... | eBPF JIT scratch space
 * eBPF fp register =>    +-----+
 *   (BPF_FP)             | ... | eBPF prog stack
 *                        +-----+
 *                        |RSVD | JIT scratchpad
 * current ARM_SP =>      +-----+ <= (BPF_FP - STACK_SIZE + SCRATCH_SIZE)
 *                        | ... | caller-saved registers
 *                        +-----+
 *                        | ... | arguments passed on stack
 * ARM_SP during call =>  +-----|
 *                        |     |
 *                        | ... | Function call stack
 *                        |     |
 *                        +-----+
 *                          low
 *
 * The callee saved registers depends on whether frame pointers are enabled.
 * With frame pointers (to be compliant with the ABI):
 *
 *                              high
 * original ARM_SP =>     +--------------+ \
 *                        |      pc      | |
 * current ARM_FP =>      +--------------+ } callee saved registers
 *                        |r4-r9,fp,ip,lr| |
 *                        +--------------+ /
 *                              low
 *
 * Without frame pointers:
 *
 *                              high
 * original ARM_SP =>     +--------------+
 *                        |  r4-r9,fp,lr | callee saved registers
 * current ARM_FP =>      +--------------+
 *                              low
 *
 * When popping registers off the stack at the end of a BPF function, we
 * reference them via the current ARM_FP register.
 *
 * Some eBPF operations are implemented via a call to a helper function.
 * Such calls are "invisible" in the eBPF code, so it is up to the calling
 * program to preserve any caller-saved ARM registers during the call. The
 * JIT emits code to push and pop those registers onto the stack, immediately
 * above the callee stack frame.
 */
#define CALLEE_MASK	(1 << ARM_R4 | 1 << ARM_R5 | 1 << ARM_R6 | \
			 1 << ARM_R7 | 1 << ARM_R8 | 1 << ARM_R9 | \
			 1 << ARM_FP)
#define CALLEE_PUSH_MASK (CALLEE_MASK | 1 << ARM_LR)
#define CALLEE_POP_MASK  (CALLEE_MASK | 1 << ARM_PC)

#define CALLER_MASK	(1 << ARM_R0 | 1 << ARM_R1 | 1 << ARM_R2 | 1 << ARM_R3)

enum {
	/* Stack layout - these are offsets from (top of stack - 4) */
	BPF_R2_HI,
	BPF_R2_LO,
	BPF_R3_HI,
	BPF_R3_LO,
	BPF_R4_HI,
	BPF_R4_LO,
	BPF_R5_HI,
	BPF_R5_LO,
	BPF_R7_HI,
	BPF_R7_LO,
	BPF_R8_HI,
	BPF_R8_LO,
	BPF_R9_HI,
	BPF_R9_LO,
	BPF_FP_HI,
	BPF_FP_LO,
	BPF_TC_HI,
	BPF_TC_LO,
	BPF_AX_HI,
	BPF_AX_LO,
	/* Stack space for BPF_REG_2, BPF_REG_3, BPF_REG_4,
	 * BPF_REG_5, BPF_REG_7, BPF_REG_8, BPF_REG_9,
	 * BPF_REG_FP and Tail call counts.
	 */
	BPF_JIT_SCRATCH_REGS,
};

/*
 * Negative "register" values indicate the register is stored on the stack
 * and are the offset from the top of the eBPF JIT scratch space.
 */
#define STACK_OFFSET(k)	(-4 - (k) * 4)
#define SCRATCH_SIZE	(BPF_JIT_SCRATCH_REGS * 4)

#ifdef CONFIG_FRAME_POINTER
#define EBPF_SCRATCH_TO_ARM_FP(x) ((x) - 4 * hweight16(CALLEE_PUSH_MASK) - 4)
#else
#define EBPF_SCRATCH_TO_ARM_FP(x) (x)
#endif

#define TMP_REG_1	(MAX_BPF_JIT_REG + 0)	/* TEMP Register 1 */
#define TMP_REG_2	(MAX_BPF_JIT_REG + 1)	/* TEMP Register 2 */
#define TCALL_CNT	(MAX_BPF_JIT_REG + 2)	/* Tail Call Count */

#define FLAG_IMM_OVERFLOW	(1 << 0)

/*
 * Map eBPF registers to ARM 32bit registers or stack scratch space.
 *
 * 1. First argument is passed using the arm 32bit registers and rest of the
 * arguments are passed on stack scratch space.
 * 2. First callee-saved argument is mapped to arm 32 bit registers and rest
 * arguments are mapped to scratch space on stack.
 * 3. We need two 64 bit temp registers to do complex operations on eBPF
 * registers.
 *
 * As the eBPF registers are all 64 bit registers and arm has only 32 bit
 * registers, we have to map each eBPF registers with two arm 32 bit regs or
 * scratch memory space and we have to build eBPF 64 bit register from those.
 *
 */
static const s8 bpf2a32[][2] = {
	/* return value from in-kernel function, and exit value from eBPF */
	[BPF_REG_0] = {ARM_R1, ARM_R0},
	/* arguments from eBPF program to in-kernel function */
	[BPF_REG_1] = {ARM_R3, ARM_R2},
	/* Stored on stack scratch space */
	[BPF_REG_2] = {STACK_OFFSET(BPF_R2_HI), STACK_OFFSET(BPF_R2_LO)},
	[BPF_REG_3] = {STACK_OFFSET(BPF_R3_HI), STACK_OFFSET(BPF_R3_LO)},
	[BPF_REG_4] = {STACK_OFFSET(BPF_R4_HI), STACK_OFFSET(BPF_R4_LO)},
	[BPF_REG_5] = {STACK_OFFSET(BPF_R5_HI), STACK_OFFSET(BPF_R5_LO)},
	/* callee saved registers that in-kernel function will preserve */
	[BPF_REG_6] = {ARM_R5, ARM_R4},
	/* Stored on stack scratch space */
	[BPF_REG_7] = {STACK_OFFSET(BPF_R7_HI), STACK_OFFSET(BPF_R7_LO)},
	[BPF_REG_8] = {STACK_OFFSET(BPF_R8_HI), STACK_OFFSET(BPF_R8_LO)},
	[BPF_REG_9] = {STACK_OFFSET(BPF_R9_HI), STACK_OFFSET(BPF_R9_LO)},
	/* Read only Frame Pointer to access Stack */
	[BPF_REG_FP] = {STACK_OFFSET(BPF_FP_HI), STACK_OFFSET(BPF_FP_LO)},
	/* Temporary Register for BPF JIT, can be used
	 * for constant blindings and others.
	 */
	[TMP_REG_1] = {ARM_R7, ARM_R6},
	[TMP_REG_2] = {ARM_R9, ARM_R8},
	/* Tail call count. Stored on stack scratch space. */
	[TCALL_CNT] = {STACK_OFFSET(BPF_TC_HI), STACK_OFFSET(BPF_TC_LO)},
	/* temporary register for blinding constants.
	 * Stored on stack scratch space.
	 */
	[BPF_REG_AX] = {STACK_OFFSET(BPF_AX_HI), STACK_OFFSET(BPF_AX_LO)},
};

#define	dst_lo	dst[1]
#define dst_hi	dst[0]
#define src_lo	src[1]
#define src_hi	src[0]

/*
 * JIT Context:
 *
 * prog			:	bpf_prog
 * idx			:	index of current last JITed instruction.
 * prologue_bytes	:	bytes used in prologue.
 * epilogue_offset	:	offset of epilogue starting.
 * offsets		:	array of eBPF instruction offsets in
 *				JITed code.
 * target		:	final JITed code.
 * epilogue_bytes	:	no of bytes used in epilogue.
 * imm_count		:	no of immediate counts used for global
 *				variables.
 * imms			:	array of global variable addresses.
 */

struct jit_ctx {
	const struct bpf_prog *prog;
	unsigned int idx;
	unsigned int prologue_bytes;
	unsigned int epilogue_offset;
	unsigned int cpu_architecture;
	u32 flags;
	u32 *offsets;
	u32 *target;
	u32 stack_size;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#if __LINUX_ARM_ARCH__ < 7
	u16 epilogue_bytes;
	u16 imm_count;
	u32 *imms;
#endif
};

<<<<<<< HEAD
int bpf_jit_enable __read_mostly;

static u64 jit_get_skb_b(struct sk_buff *skb, unsigned offset)
{
	u8 ret;
	int err;

	err = skb_copy_bits(skb, offset, &ret, 1);

	return (u64)err << 32 | ret;
}

static u64 jit_get_skb_h(struct sk_buff *skb, unsigned offset)
{
	u16 ret;
	int err;

	err = skb_copy_bits(skb, offset, &ret, 2);

	return (u64)err << 32 | ntohs(ret);
}

static u64 jit_get_skb_w(struct sk_buff *skb, unsigned offset)
{
	u32 ret;
	int err;

	err = skb_copy_bits(skb, offset, &ret, 4);

	return (u64)err << 32 | ntohl(ret);
}

/*
 * Wrapper that handles both OABI and EABI and assures Thumb2 interworking
 * (where the assembly routines like __aeabi_uidiv could cause problems).
 */
static u32 jit_udiv(u32 dividend, u32 divisor)
=======
/*
 * Wrappers which handle both OABI and EABI and assures Thumb2 interworking
 * (where the assembly routines like __aeabi_uidiv could cause problems).
 */
static u32 jit_udiv32(u32 dividend, u32 divisor)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
{
	return dividend / divisor;
}

<<<<<<< HEAD
static inline void _emit(int cond, u32 inst, struct jit_ctx *ctx)
{
	if (ctx->target != NULL)
		ctx->target[ctx->idx] = inst | (cond << 28);
=======
static u32 jit_mod32(u32 dividend, u32 divisor)
{
	return dividend % divisor;
}

static s32 jit_sdiv32(s32 dividend, s32 divisor)
{
	return dividend / divisor;
}

static s32 jit_smod32(s32 dividend, s32 divisor)
{
	return dividend % divisor;
}

/* Wrappers for 64-bit div/mod */
static u64 jit_udiv64(u64 dividend, u64 divisor)
{
	return div64_u64(dividend, divisor);
}

static u64 jit_mod64(u64 dividend, u64 divisor)
{
	u64 rem;

	div64_u64_rem(dividend, divisor, &rem);
	return rem;
}

static s64 jit_sdiv64(s64 dividend, s64 divisor)
{
	return div64_s64(dividend, divisor);
}

static s64 jit_smod64(s64 dividend, s64 divisor)
{
	u64 q;

	q = div64_s64(dividend, divisor);

	return dividend - q * divisor;
}

static inline void _emit(int cond, u32 inst, struct jit_ctx *ctx)
{
	inst |= (cond << 28);
	inst = __opcode_to_mem_arm(inst);

	if (ctx->target != NULL)
		ctx->target[ctx->idx] = inst;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	ctx->idx++;
}

/*
 * Emit an instruction that will be executed unconditionally.
 */
static inline void emit(u32 inst, struct jit_ctx *ctx)
{
	_emit(ARM_COND_AL, inst, ctx);
}

<<<<<<< HEAD
static u16 saved_regs(struct jit_ctx *ctx)
{
	u16 ret = 0;

	if ((ctx->skf->len > 1) ||
	    (ctx->skf->insns[0].code == BPF_S_RET_A))
		ret |= 1 << r_A;

#ifdef CONFIG_FRAME_POINTER
	ret |= (1 << ARM_FP) | (1 << ARM_IP) | (1 << ARM_LR) | (1 << ARM_PC);
#else
	if (ctx->seen & SEEN_CALL)
		ret |= 1 << ARM_LR;
#endif
	if (ctx->seen & (SEEN_DATA | SEEN_SKB))
		ret |= 1 << r_skb;
	if (ctx->seen & SEEN_DATA)
		ret |= (1 << r_skb_data) | (1 << r_skb_hl);
	if (ctx->seen & SEEN_X)
		ret |= 1 << r_X;

	return ret;
}

static inline int mem_words_used(struct jit_ctx *ctx)
{
	/* yes, we do waste some stack space IF there are "holes" in the set" */
	return fls(ctx->seen & SEEN_MEM);
}

static inline bool is_load_to_a(u16 inst)
{
	switch (inst) {
	case BPF_S_LD_W_LEN:
	case BPF_S_LD_W_ABS:
	case BPF_S_LD_H_ABS:
	case BPF_S_LD_B_ABS:
	case BPF_S_ANC_CPU:
	case BPF_S_ANC_IFINDEX:
	case BPF_S_ANC_MARK:
	case BPF_S_ANC_PROTOCOL:
	case BPF_S_ANC_RXHASH:
	case BPF_S_ANC_QUEUE:
		return true;
	default:
		return false;
	}
}

static void build_prologue(struct jit_ctx *ctx)
{
	u16 reg_set = saved_regs(ctx);
	u16 first_inst = ctx->skf->insns[0].code;
	u16 off;

#ifdef CONFIG_FRAME_POINTER
	emit(ARM_MOV_R(ARM_IP, ARM_SP), ctx);
	emit(ARM_PUSH(reg_set), ctx);
	emit(ARM_SUB_I(ARM_FP, ARM_IP, 4), ctx);
#else
	if (reg_set)
		emit(ARM_PUSH(reg_set), ctx);
#endif

	if (ctx->seen & (SEEN_DATA | SEEN_SKB))
		emit(ARM_MOV_R(r_skb, ARM_R0), ctx);

	if (ctx->seen & SEEN_DATA) {
		off = offsetof(struct sk_buff, data);
		emit(ARM_LDR_I(r_skb_data, r_skb, off), ctx);
		/* headlen = len - data_len */
		off = offsetof(struct sk_buff, len);
		emit(ARM_LDR_I(r_skb_hl, r_skb, off), ctx);
		off = offsetof(struct sk_buff, data_len);
		emit(ARM_LDR_I(r_scratch, r_skb, off), ctx);
		emit(ARM_SUB_R(r_skb_hl, r_skb_hl, r_scratch), ctx);
	}

	if (ctx->flags & FLAG_NEED_X_RESET)
		emit(ARM_MOV_I(r_X, 0), ctx);

	/* do not leak kernel data to userspace */
	if ((first_inst != BPF_S_RET_K) && !(is_load_to_a(first_inst)))
		emit(ARM_MOV_I(r_A, 0), ctx);

	/* stack space for the BPF_MEM words */
	if (ctx->seen & SEEN_MEM)
		emit(ARM_SUB_I(ARM_SP, ARM_SP, mem_words_used(ctx) * 4), ctx);
}

static void build_epilogue(struct jit_ctx *ctx)
{
	u16 reg_set = saved_regs(ctx);

	if (ctx->seen & SEEN_MEM)
		emit(ARM_ADD_I(ARM_SP, ARM_SP, mem_words_used(ctx) * 4), ctx);

	reg_set &= ~(1 << ARM_LR);

#ifdef CONFIG_FRAME_POINTER
	/* the first instruction of the prologue was: mov ip, sp */
	reg_set &= ~(1 << ARM_IP);
	reg_set |= (1 << ARM_SP);
	emit(ARM_LDM(ARM_SP, reg_set), ctx);
#else
	if (reg_set) {
		if (ctx->seen & SEEN_CALL)
			reg_set |= 1 << ARM_PC;
		emit(ARM_POP(reg_set), ctx);
	}

	if (!(ctx->seen & SEEN_CALL))
		emit(ARM_BX(ARM_LR), ctx);
#endif
}

static int16_t imm8m(u32 x)
=======
/*
 * This is rather horrid, but necessary to convert an integer constant
 * to an immediate operand for the opcodes, and be able to detect at
 * build time whether the constant can't be converted (iow, usable in
 * BUILD_BUG_ON()).
 */
#define imm12val(v, s) (rol32(v, (s)) | (s) << 7)
#define const_imm8m(x)					\
	({ int r;					\
	   u32 v = (x);					\
	   if (!(v & ~0x000000ff))			\
		r = imm12val(v, 0);			\
	   else if (!(v & ~0xc000003f))			\
		r = imm12val(v, 2);			\
	   else if (!(v & ~0xf000000f))			\
		r = imm12val(v, 4);			\
	   else if (!(v & ~0xfc000003))			\
		r = imm12val(v, 6);			\
	   else if (!(v & ~0xff000000))			\
		r = imm12val(v, 8);			\
	   else if (!(v & ~0x3fc00000))			\
		r = imm12val(v, 10);			\
	   else if (!(v & ~0x0ff00000))			\
		r = imm12val(v, 12);			\
	   else if (!(v & ~0x03fc0000))			\
		r = imm12val(v, 14);			\
	   else if (!(v & ~0x00ff0000))			\
		r = imm12val(v, 16);			\
	   else if (!(v & ~0x003fc000))			\
		r = imm12val(v, 18);			\
	   else if (!(v & ~0x000ff000))			\
		r = imm12val(v, 20);			\
	   else if (!(v & ~0x0003fc00))			\
		r = imm12val(v, 22);			\
	   else if (!(v & ~0x0000ff00))			\
		r = imm12val(v, 24);			\
	   else if (!(v & ~0x00003fc0))			\
		r = imm12val(v, 26);			\
	   else if (!(v & ~0x00000ff0))			\
		r = imm12val(v, 28);			\
	   else if (!(v & ~0x000003fc))			\
		r = imm12val(v, 30);			\
	   else						\
		r = -1;					\
	   r; })

/*
 * Checks if immediate value can be converted to imm12(12 bits) value.
 */
static int imm8m(u32 x)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
{
	u32 rot;

	for (rot = 0; rot < 16; rot++)
		if ((x & ~ror32(0xff, 2 * rot)) == 0)
			return rol32(x, 2 * rot) | (rot << 8);
<<<<<<< HEAD

	return -1;
}

=======
	return -1;
}

#define imm8m(x) (__builtin_constant_p(x) ? const_imm8m(x) : imm8m(x))

static u32 arm_bpf_ldst_imm12(u32 op, u8 rt, u8 rn, s16 imm12)
{
	op |= rt << 12 | rn << 16;
	if (imm12 >= 0)
		op |= ARM_INST_LDST__U;
	else
		imm12 = -imm12;
	return op | (imm12 & ARM_INST_LDST__IMM12);
}

static u32 arm_bpf_ldst_imm8(u32 op, u8 rt, u8 rn, s16 imm8)
{
	op |= rt << 12 | rn << 16;
	if (imm8 >= 0)
		op |= ARM_INST_LDST__U;
	else
		imm8 = -imm8;
	return op | (imm8 & 0xf0) << 4 | (imm8 & 0x0f);
}

#define ARM_LDR_I(rt, rn, off)	arm_bpf_ldst_imm12(ARM_INST_LDR_I, rt, rn, off)
#define ARM_LDRB_I(rt, rn, off)	arm_bpf_ldst_imm12(ARM_INST_LDRB_I, rt, rn, off)
#define ARM_LDRD_I(rt, rn, off)	arm_bpf_ldst_imm8(ARM_INST_LDRD_I, rt, rn, off)
#define ARM_LDRH_I(rt, rn, off)	arm_bpf_ldst_imm8(ARM_INST_LDRH_I, rt, rn, off)

#define ARM_LDRSH_I(rt, rn, off) arm_bpf_ldst_imm8(ARM_INST_LDRSH_I, rt, rn, off)
#define ARM_LDRSB_I(rt, rn, off) arm_bpf_ldst_imm8(ARM_INST_LDRSB_I, rt, rn, off)

#define ARM_STR_I(rt, rn, off)	arm_bpf_ldst_imm12(ARM_INST_STR_I, rt, rn, off)
#define ARM_STRB_I(rt, rn, off)	arm_bpf_ldst_imm12(ARM_INST_STRB_I, rt, rn, off)
#define ARM_STRD_I(rt, rn, off)	arm_bpf_ldst_imm8(ARM_INST_STRD_I, rt, rn, off)
#define ARM_STRH_I(rt, rn, off)	arm_bpf_ldst_imm8(ARM_INST_STRH_I, rt, rn, off)

/*
 * Initializes the JIT space with undefined instructions.
 */
static void jit_fill_hole(void *area, unsigned int size)
{
	u32 *ptr;
	/* We are guaranteed to have aligned memory. */
	for (ptr = area; size >= sizeof(u32); size -= sizeof(u32))
		*ptr++ = __opcode_to_mem_arm(ARM_INST_UDF);
}

#if defined(CONFIG_AEABI) && (__LINUX_ARM_ARCH__ >= 5)
/* EABI requires the stack to be aligned to 64-bit boundaries */
#define STACK_ALIGNMENT	8
#else
/* Stack must be aligned to 32-bit boundaries */
#define STACK_ALIGNMENT	4
#endif

/* total stack size used in JITed code */
#define _STACK_SIZE	(ctx->prog->aux->stack_depth + SCRATCH_SIZE)
#define STACK_SIZE	ALIGN(_STACK_SIZE, STACK_ALIGNMENT)

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#if __LINUX_ARM_ARCH__ < 7

static u16 imm_offset(u32 k, struct jit_ctx *ctx)
{
<<<<<<< HEAD
	unsigned i = 0, offset;
=======
	unsigned int i = 0, offset;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	u16 imm;

	/* on the "fake" run we just count them (duplicates included) */
	if (ctx->target == NULL) {
		ctx->imm_count++;
		return 0;
	}

	while ((i < ctx->imm_count) && ctx->imms[i]) {
		if (ctx->imms[i] == k)
			break;
		i++;
	}

	if (ctx->imms[i] == 0)
		ctx->imms[i] = k;

	/* constants go just after the epilogue */
<<<<<<< HEAD
	offset =  ctx->offsets[ctx->skf->len];
=======
	offset =  ctx->offsets[ctx->prog->len - 1] * 4;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	offset += ctx->prologue_bytes;
	offset += ctx->epilogue_bytes;
	offset += i * 4;

	ctx->target[offset / 4] = k;

	/* PC in ARM mode == address of the instruction + 8 */
	imm = offset - (8 + ctx->idx * 4);

	if (imm & ~0xfff) {
		/*
		 * literal pool is too far, signal it into flags. we
		 * can only detect it on the second pass unfortunately.
		 */
		ctx->flags |= FLAG_IMM_OVERFLOW;
		return 0;
	}

	return imm;
}

#endif /* __LINUX_ARM_ARCH__ */

<<<<<<< HEAD
/*
 * Move an immediate that's not an imm8m to a core register.
 */
static inline void emit_mov_i_no8m(int rd, u32 val, struct jit_ctx *ctx)
=======
static inline int bpf2a32_offset(int bpf_to, int bpf_from,
				 const struct jit_ctx *ctx) {
	int to, from;

	if (ctx->target == NULL)
		return 0;
	to = ctx->offsets[bpf_to];
	from = ctx->offsets[bpf_from];

	return to - from - 1;
}

/*
 * Move an immediate that's not an imm8m to a core register.
 */
static inline void emit_mov_i_no8m(const u8 rd, u32 val, struct jit_ctx *ctx)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
{
#if __LINUX_ARM_ARCH__ < 7
	emit(ARM_LDR_I(rd, ARM_PC, imm_offset(val, ctx)), ctx);
#else
	emit(ARM_MOVW(rd, val & 0xffff), ctx);
	if (val > 0xffff)
		emit(ARM_MOVT(rd, val >> 16), ctx);
#endif
}

<<<<<<< HEAD
static inline void emit_mov_i(int rd, u32 val, struct jit_ctx *ctx)
=======
static inline void emit_mov_i(const u8 rd, u32 val, struct jit_ctx *ctx)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
{
	int imm12 = imm8m(val);

	if (imm12 >= 0)
		emit(ARM_MOV_I(rd, imm12), ctx);
	else
		emit_mov_i_no8m(rd, val, ctx);
}

<<<<<<< HEAD
#if __LINUX_ARM_ARCH__ < 6

static void emit_load_be32(u8 cond, u8 r_res, u8 r_addr, struct jit_ctx *ctx)
{
	_emit(cond, ARM_LDRB_I(ARM_R3, r_addr, 1), ctx);
	_emit(cond, ARM_LDRB_I(ARM_R1, r_addr, 0), ctx);
	_emit(cond, ARM_LDRB_I(ARM_R2, r_addr, 3), ctx);
	_emit(cond, ARM_LSL_I(ARM_R3, ARM_R3, 16), ctx);
	_emit(cond, ARM_LDRB_I(ARM_R0, r_addr, 2), ctx);
	_emit(cond, ARM_ORR_S(ARM_R3, ARM_R3, ARM_R1, SRTYPE_LSL, 24), ctx);
	_emit(cond, ARM_ORR_R(ARM_R3, ARM_R3, ARM_R2), ctx);
	_emit(cond, ARM_ORR_S(r_res, ARM_R3, ARM_R0, SRTYPE_LSL, 8), ctx);
}

static void emit_load_be16(u8 cond, u8 r_res, u8 r_addr, struct jit_ctx *ctx)
{
	_emit(cond, ARM_LDRB_I(ARM_R1, r_addr, 0), ctx);
	_emit(cond, ARM_LDRB_I(ARM_R2, r_addr, 1), ctx);
	_emit(cond, ARM_ORR_S(r_res, ARM_R2, ARM_R1, SRTYPE_LSL, 8), ctx);
}

static inline void emit_swap16(u8 r_dst, u8 r_src, struct jit_ctx *ctx)
{
	emit(ARM_LSL_R(ARM_R1, r_src, 8), ctx);
	emit(ARM_ORR_S(r_dst, ARM_R1, r_src, SRTYPE_LSL, 8), ctx);
	emit(ARM_LSL_I(r_dst, r_dst, 8), ctx);
	emit(ARM_LSL_R(r_dst, r_dst, 8), ctx);
}

#else  /* ARMv6+ */

static void emit_load_be32(u8 cond, u8 r_res, u8 r_addr, struct jit_ctx *ctx)
{
	_emit(cond, ARM_LDR_I(r_res, r_addr, 0), ctx);
#ifdef __LITTLE_ENDIAN
	_emit(cond, ARM_REV(r_res, r_res), ctx);
#endif
}

static void emit_load_be16(u8 cond, u8 r_res, u8 r_addr, struct jit_ctx *ctx)
{
	_emit(cond, ARM_LDRH_I(r_res, r_addr, 0), ctx);
#ifdef __LITTLE_ENDIAN
	_emit(cond, ARM_REV16(r_res, r_res), ctx);
#endif
}

static inline void emit_swap16(u8 r_dst __maybe_unused,
			       u8 r_src __maybe_unused,
			       struct jit_ctx *ctx __maybe_unused)
{
#ifdef __LITTLE_ENDIAN
	emit(ARM_REV16(r_dst, r_src), ctx);
#endif
}

#endif /* __LINUX_ARM_ARCH__ < 6 */


/* Compute the immediate value for a PC-relative branch. */
static inline u32 b_imm(unsigned tgt, struct jit_ctx *ctx)
{
	u32 imm;

	if (ctx->target == NULL)
		return 0;
	/*
	 * BPF allows only forward jumps and the offset of the target is
	 * still the one computed during the first pass.
	 */
	imm  = ctx->offsets[tgt] + ctx->prologue_bytes - (ctx->idx * 4 + 8);

	return imm >> 2;
}

#define OP_IMM3(op, r1, r2, imm_val, ctx)				\
	do {								\
		imm12 = imm8m(imm_val);					\
		if (imm12 < 0) {					\
			emit_mov_i_no8m(r_scratch, imm_val, ctx);	\
			emit(op ## _R((r1), (r2), r_scratch), ctx);	\
		} else {						\
			emit(op ## _I((r1), (r2), imm12), ctx);		\
		}							\
	} while (0)

static inline void emit_err_ret(u8 cond, struct jit_ctx *ctx)
{
	if (ctx->ret0_fp_idx >= 0) {
		_emit(cond, ARM_B(b_imm(ctx->ret0_fp_idx, ctx)), ctx);
		/* NOP to keep the size constant between passes */
		emit(ARM_MOV_R(ARM_R0, ARM_R0), ctx);
	} else {
		_emit(cond, ARM_MOV_I(ARM_R0, 0), ctx);
		_emit(cond, ARM_B(b_imm(ctx->skf->len, ctx)), ctx);
	}
=======
static void emit_bx_r(u8 tgt_reg, struct jit_ctx *ctx)
{
	if (elf_hwcap & HWCAP_THUMB)
		emit(ARM_BX(tgt_reg), ctx);
	else
		emit(ARM_MOV_R(ARM_PC, tgt_reg), ctx);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

static inline void emit_blx_r(u8 tgt_reg, struct jit_ctx *ctx)
{
#if __LINUX_ARM_ARCH__ < 5
	emit(ARM_MOV_R(ARM_LR, ARM_PC), ctx);
<<<<<<< HEAD

	if (elf_hwcap & HWCAP_THUMB)
		emit(ARM_BX(tgt_reg), ctx);
	else
		emit(ARM_MOV_R(ARM_PC, tgt_reg), ctx);
=======
	emit_bx_r(tgt_reg, ctx);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#else
	emit(ARM_BLX_R(tgt_reg), ctx);
#endif
}

<<<<<<< HEAD
static inline void emit_udiv(u8 rd, u8 rm, u8 rn, struct jit_ctx *ctx)
{
#if __LINUX_ARM_ARCH__ == 7
	if (elf_hwcap & HWCAP_IDIVA) {
		emit(ARM_UDIV(rd, rm, rn), ctx);
		return;
	}
#endif
	if (rm != ARM_R0)
		emit(ARM_MOV_R(ARM_R0, rm), ctx);
	if (rn != ARM_R1)
		emit(ARM_MOV_R(ARM_R1, rn), ctx);

	ctx->seen |= SEEN_CALL;
	emit_mov_i(ARM_R3, (u32)jit_udiv, ctx);
	emit_blx_r(ARM_R3, ctx);

	if (rd != ARM_R0)
		emit(ARM_MOV_R(rd, ARM_R0), ctx);
}

static inline void update_on_xread(struct jit_ctx *ctx)
{
	if (!(ctx->seen & SEEN_X))
		ctx->flags |= FLAG_NEED_X_RESET;

	ctx->seen |= SEEN_X;
=======
static inline int epilogue_offset(const struct jit_ctx *ctx)
{
	int to, from;
	/* No need for 1st dummy run */
	if (ctx->target == NULL)
		return 0;
	to = ctx->epilogue_offset;
	from = ctx->idx;

	return to - from - 2;
}

static inline void emit_udivmod(u8 rd, u8 rm, u8 rn, struct jit_ctx *ctx, u8 op, u8 sign)
{
	const int exclude_mask = BIT(ARM_R0) | BIT(ARM_R1);
	const s8 *tmp = bpf2a32[TMP_REG_1];
	u32 dst;

#if __LINUX_ARM_ARCH__ == 7
	if (elf_hwcap & HWCAP_IDIVA) {
		if (op == BPF_DIV) {
			emit(sign ? ARM_SDIV(rd, rm, rn) : ARM_UDIV(rd, rm, rn), ctx);
		} else {
			emit(sign ? ARM_SDIV(ARM_IP, rm, rn) : ARM_UDIV(ARM_IP, rm, rn), ctx);
			emit(ARM_MLS(rd, rn, ARM_IP, rm), ctx);
		}
		return;
	}
#endif

	/*
	 * For BPF_ALU | BPF_DIV | BPF_K instructions
	 * As ARM_R1 and ARM_R0 contains 1st argument of bpf
	 * function, we need to save it on caller side to save
	 * it from getting destroyed within callee.
	 * After the return from the callee, we restore ARM_R0
	 * ARM_R1.
	 */
	if (rn != ARM_R1) {
		emit(ARM_MOV_R(tmp[0], ARM_R1), ctx);
		emit(ARM_MOV_R(ARM_R1, rn), ctx);
	}
	if (rm != ARM_R0) {
		emit(ARM_MOV_R(tmp[1], ARM_R0), ctx);
		emit(ARM_MOV_R(ARM_R0, rm), ctx);
	}

	/* Push caller-saved registers on stack */
	emit(ARM_PUSH(CALLER_MASK & ~exclude_mask), ctx);

	/* Call appropriate function */
	if (sign) {
		if (op == BPF_DIV)
			dst = (u32)jit_sdiv32;
		else
			dst = (u32)jit_smod32;
	} else {
		if (op == BPF_DIV)
			dst = (u32)jit_udiv32;
		else
			dst = (u32)jit_mod32;
	}

	emit_mov_i(ARM_IP, dst, ctx);
	emit_blx_r(ARM_IP, ctx);

	/* Restore caller-saved registers from stack */
	emit(ARM_POP(CALLER_MASK & ~exclude_mask), ctx);

	/* Save return value */
	if (rd != ARM_R0)
		emit(ARM_MOV_R(rd, ARM_R0), ctx);

	/* Restore ARM_R0 and ARM_R1 */
	if (rn != ARM_R1)
		emit(ARM_MOV_R(ARM_R1, tmp[0]), ctx);
	if (rm != ARM_R0)
		emit(ARM_MOV_R(ARM_R0, tmp[1]), ctx);
}

static inline void emit_udivmod64(const s8 *rd, const s8 *rm, const s8 *rn, struct jit_ctx *ctx,
				  u8 op, u8 sign)
{
	u32 dst;

	/* Push caller-saved registers on stack */
	emit(ARM_PUSH(CALLER_MASK), ctx);

	/*
	 * As we are implementing 64-bit div/mod as function calls, We need to put the dividend in
	 * R0-R1 and the divisor in R2-R3. As we have already pushed these registers on the stack,
	 * we can recover them later after returning from the function call.
	 */
	if (rm[1] != ARM_R0 || rn[1] != ARM_R2) {
		/*
		 * Move Rm to {R1, R0} if it is not already there.
		 */
		if (rm[1] != ARM_R0) {
			if (rn[1] == ARM_R0)
				emit(ARM_PUSH(BIT(ARM_R0) | BIT(ARM_R1)), ctx);
			emit(ARM_MOV_R(ARM_R1, rm[0]), ctx);
			emit(ARM_MOV_R(ARM_R0, rm[1]), ctx);
			if (rn[1] == ARM_R0) {
				emit(ARM_POP(BIT(ARM_R2) | BIT(ARM_R3)), ctx);
				goto cont;
			}
		}
		/*
		 * Move Rn to {R3, R2} if it is not already there.
		 */
		if (rn[1] != ARM_R2) {
			emit(ARM_MOV_R(ARM_R3, rn[0]), ctx);
			emit(ARM_MOV_R(ARM_R2, rn[1]), ctx);
		}
	}

cont:

	/* Call appropriate function */
	if (sign) {
		if (op == BPF_DIV)
			dst = (u32)jit_sdiv64;
		else
			dst = (u32)jit_smod64;
	} else {
		if (op == BPF_DIV)
			dst = (u32)jit_udiv64;
		else
			dst = (u32)jit_mod64;
	}

	emit_mov_i(ARM_IP, dst, ctx);
	emit_blx_r(ARM_IP, ctx);

	/* Save return value */
	if (rd[1] != ARM_R0) {
		emit(ARM_MOV_R(rd[0], ARM_R1), ctx);
		emit(ARM_MOV_R(rd[1], ARM_R0), ctx);
	}

	/* Recover {R3, R2} and {R1, R0} from stack if they are not Rd */
	if (rd[1] != ARM_R0 && rd[1] != ARM_R2) {
		emit(ARM_POP(CALLER_MASK), ctx);
	} else if (rd[1] != ARM_R0) {
		emit(ARM_POP(BIT(ARM_R0) | BIT(ARM_R1)), ctx);
		emit(ARM_ADD_I(ARM_SP, ARM_SP, 8), ctx);
	} else {
		emit(ARM_ADD_I(ARM_SP, ARM_SP, 8), ctx);
		emit(ARM_POP(BIT(ARM_R2) | BIT(ARM_R3)), ctx);
	}
}

/* Is the translated BPF register on stack? */
static bool is_stacked(s8 reg)
{
	return reg < 0;
}

/* If a BPF register is on the stack (stk is true), load it to the
 * supplied temporary register and return the temporary register
 * for subsequent operations, otherwise just use the CPU register.
 */
static s8 arm_bpf_get_reg32(s8 reg, s8 tmp, struct jit_ctx *ctx)
{
	if (is_stacked(reg)) {
		emit(ARM_LDR_I(tmp, ARM_FP, EBPF_SCRATCH_TO_ARM_FP(reg)), ctx);
		reg = tmp;
	}
	return reg;
}

static const s8 *arm_bpf_get_reg64(const s8 *reg, const s8 *tmp,
				   struct jit_ctx *ctx)
{
	if (is_stacked(reg[1])) {
		if (__LINUX_ARM_ARCH__ >= 6 ||
		    ctx->cpu_architecture >= CPU_ARCH_ARMv5TE) {
			emit(ARM_LDRD_I(tmp[1], ARM_FP,
					EBPF_SCRATCH_TO_ARM_FP(reg[1])), ctx);
		} else {
			emit(ARM_LDR_I(tmp[1], ARM_FP,
				       EBPF_SCRATCH_TO_ARM_FP(reg[1])), ctx);
			emit(ARM_LDR_I(tmp[0], ARM_FP,
				       EBPF_SCRATCH_TO_ARM_FP(reg[0])), ctx);
		}
		reg = tmp;
	}
	return reg;
}

/* If a BPF register is on the stack (stk is true), save the register
 * back to the stack.  If the source register is not the same, then
 * move it into the correct register.
 */
static void arm_bpf_put_reg32(s8 reg, s8 src, struct jit_ctx *ctx)
{
	if (is_stacked(reg))
		emit(ARM_STR_I(src, ARM_FP, EBPF_SCRATCH_TO_ARM_FP(reg)), ctx);
	else if (reg != src)
		emit(ARM_MOV_R(reg, src), ctx);
}

static void arm_bpf_put_reg64(const s8 *reg, const s8 *src,
			      struct jit_ctx *ctx)
{
	if (is_stacked(reg[1])) {
		if (__LINUX_ARM_ARCH__ >= 6 ||
		    ctx->cpu_architecture >= CPU_ARCH_ARMv5TE) {
			emit(ARM_STRD_I(src[1], ARM_FP,
				       EBPF_SCRATCH_TO_ARM_FP(reg[1])), ctx);
		} else {
			emit(ARM_STR_I(src[1], ARM_FP,
				       EBPF_SCRATCH_TO_ARM_FP(reg[1])), ctx);
			emit(ARM_STR_I(src[0], ARM_FP,
				       EBPF_SCRATCH_TO_ARM_FP(reg[0])), ctx);
		}
	} else {
		if (reg[1] != src[1])
			emit(ARM_MOV_R(reg[1], src[1]), ctx);
		if (reg[0] != src[0])
			emit(ARM_MOV_R(reg[0], src[0]), ctx);
	}
}

static inline void emit_a32_mov_i(const s8 dst, const u32 val,
				  struct jit_ctx *ctx)
{
	const s8 *tmp = bpf2a32[TMP_REG_1];

	if (is_stacked(dst)) {
		emit_mov_i(tmp[1], val, ctx);
		arm_bpf_put_reg32(dst, tmp[1], ctx);
	} else {
		emit_mov_i(dst, val, ctx);
	}
}

static void emit_a32_mov_i64(const s8 dst[], u64 val, struct jit_ctx *ctx)
{
	const s8 *tmp = bpf2a32[TMP_REG_1];
	const s8 *rd = is_stacked(dst_lo) ? tmp : dst;

	emit_mov_i(rd[1], (u32)val, ctx);
	emit_mov_i(rd[0], val >> 32, ctx);

	arm_bpf_put_reg64(dst, rd, ctx);
}

/* Sign extended move */
static inline void emit_a32_mov_se_i64(const bool is64, const s8 dst[],
				       const u32 val, struct jit_ctx *ctx) {
	u64 val64 = val;

	if (is64 && (val & (1<<31)))
		val64 |= 0xffffffff00000000ULL;
	emit_a32_mov_i64(dst, val64, ctx);
}

static inline void emit_a32_add_r(const u8 dst, const u8 src,
			      const bool is64, const bool hi,
			      struct jit_ctx *ctx) {
	/* 64 bit :
	 *	adds dst_lo, dst_lo, src_lo
	 *	adc dst_hi, dst_hi, src_hi
	 * 32 bit :
	 *	add dst_lo, dst_lo, src_lo
	 */
	if (!hi && is64)
		emit(ARM_ADDS_R(dst, dst, src), ctx);
	else if (hi && is64)
		emit(ARM_ADC_R(dst, dst, src), ctx);
	else
		emit(ARM_ADD_R(dst, dst, src), ctx);
}

static inline void emit_a32_sub_r(const u8 dst, const u8 src,
				  const bool is64, const bool hi,
				  struct jit_ctx *ctx) {
	/* 64 bit :
	 *	subs dst_lo, dst_lo, src_lo
	 *	sbc dst_hi, dst_hi, src_hi
	 * 32 bit :
	 *	sub dst_lo, dst_lo, src_lo
	 */
	if (!hi && is64)
		emit(ARM_SUBS_R(dst, dst, src), ctx);
	else if (hi && is64)
		emit(ARM_SBC_R(dst, dst, src), ctx);
	else
		emit(ARM_SUB_R(dst, dst, src), ctx);
}

static inline void emit_alu_r(const u8 dst, const u8 src, const bool is64,
			      const bool hi, const u8 op, struct jit_ctx *ctx){
	switch (BPF_OP(op)) {
	/* dst = dst + src */
	case BPF_ADD:
		emit_a32_add_r(dst, src, is64, hi, ctx);
		break;
	/* dst = dst - src */
	case BPF_SUB:
		emit_a32_sub_r(dst, src, is64, hi, ctx);
		break;
	/* dst = dst | src */
	case BPF_OR:
		emit(ARM_ORR_R(dst, dst, src), ctx);
		break;
	/* dst = dst & src */
	case BPF_AND:
		emit(ARM_AND_R(dst, dst, src), ctx);
		break;
	/* dst = dst ^ src */
	case BPF_XOR:
		emit(ARM_EOR_R(dst, dst, src), ctx);
		break;
	/* dst = dst * src */
	case BPF_MUL:
		emit(ARM_MUL(dst, dst, src), ctx);
		break;
	/* dst = dst << src */
	case BPF_LSH:
		emit(ARM_LSL_R(dst, dst, src), ctx);
		break;
	/* dst = dst >> src */
	case BPF_RSH:
		emit(ARM_LSR_R(dst, dst, src), ctx);
		break;
	/* dst = dst >> src (signed)*/
	case BPF_ARSH:
		emit(ARM_MOV_SR(dst, dst, SRTYPE_ASR, src), ctx);
		break;
	}
}

/* ALU operation (64 bit) */
static inline void emit_a32_alu_r64(const bool is64, const s8 dst[],
				  const s8 src[], struct jit_ctx *ctx,
				  const u8 op) {
	const s8 *tmp = bpf2a32[TMP_REG_1];
	const s8 *tmp2 = bpf2a32[TMP_REG_2];
	const s8 *rd;

	rd = arm_bpf_get_reg64(dst, tmp, ctx);
	if (is64) {
		const s8 *rs;

		rs = arm_bpf_get_reg64(src, tmp2, ctx);

		/* ALU operation */
		emit_alu_r(rd[1], rs[1], true, false, op, ctx);
		emit_alu_r(rd[0], rs[0], true, true, op, ctx);
	} else {
		s8 rs;

		rs = arm_bpf_get_reg32(src_lo, tmp2[1], ctx);

		/* ALU operation */
		emit_alu_r(rd[1], rs, true, false, op, ctx);
		if (!ctx->prog->aux->verifier_zext)
			emit_a32_mov_i(rd[0], 0, ctx);
	}

	arm_bpf_put_reg64(dst, rd, ctx);
}

/* dst = src (4 bytes)*/
static inline void emit_a32_mov_r(const s8 dst, const s8 src, struct jit_ctx *ctx) {
	const s8 *tmp = bpf2a32[TMP_REG_1];
	s8 rt;

	rt = arm_bpf_get_reg32(src, tmp[0], ctx);
	arm_bpf_put_reg32(dst, rt, ctx);
}

/* dst = src */
static inline void emit_a32_mov_r64(const bool is64, const s8 dst[],
				  const s8 src[],
				  struct jit_ctx *ctx) {
	if (!is64) {
		emit_a32_mov_r(dst_lo, src_lo, ctx);
		if (!ctx->prog->aux->verifier_zext)
			/* Zero out high 4 bytes */
			emit_a32_mov_i(dst_hi, 0, ctx);
	} else if (__LINUX_ARM_ARCH__ < 6 &&
		   ctx->cpu_architecture < CPU_ARCH_ARMv5TE) {
		/* complete 8 byte move */
		emit_a32_mov_r(dst_lo, src_lo, ctx);
		emit_a32_mov_r(dst_hi, src_hi, ctx);
	} else if (is_stacked(src_lo) && is_stacked(dst_lo)) {
		const u8 *tmp = bpf2a32[TMP_REG_1];

		emit(ARM_LDRD_I(tmp[1], ARM_FP, EBPF_SCRATCH_TO_ARM_FP(src_lo)), ctx);
		emit(ARM_STRD_I(tmp[1], ARM_FP, EBPF_SCRATCH_TO_ARM_FP(dst_lo)), ctx);
	} else if (is_stacked(src_lo)) {
		emit(ARM_LDRD_I(dst[1], ARM_FP, EBPF_SCRATCH_TO_ARM_FP(src_lo)), ctx);
	} else if (is_stacked(dst_lo)) {
		emit(ARM_STRD_I(src[1], ARM_FP, EBPF_SCRATCH_TO_ARM_FP(dst_lo)), ctx);
	} else {
		emit(ARM_MOV_R(dst[0], src[0]), ctx);
		emit(ARM_MOV_R(dst[1], src[1]), ctx);
	}
}

/* dst = (signed)src */
static inline void emit_a32_movsx_r64(const bool is64, const u8 off, const s8 dst[], const s8 src[],
				      struct jit_ctx *ctx) {
	const s8 *tmp = bpf2a32[TMP_REG_1];
	s8 rs;
	s8 rd;

	if (is_stacked(dst_lo))
		rd = tmp[1];
	else
		rd = dst_lo;
	rs = arm_bpf_get_reg32(src_lo, rd, ctx);
	/* rs may be one of src[1], dst[1], or tmp[1] */

	/* Sign extend rs if needed. If off == 32, lower 32-bits of src are moved to dst and sign
	 * extension only happens in the upper 64 bits.
	 */
	if (off != 32) {
		/* Sign extend rs into rd */
		emit(ARM_LSL_I(rd, rs, 32 - off), ctx);
		emit(ARM_ASR_I(rd, rd, 32 - off), ctx);
	} else {
		rd = rs;
	}

	/* Write rd to dst_lo
	 *
	 * Optimization:
	 * Assume:
	 * 1. dst == src and stacked.
	 * 2. off == 32
	 *
	 * In this case src_lo was loaded into rd(tmp[1]) but rd was not sign extended as off==32.
	 * So, we don't need to write rd back to dst_lo as they have the same value.
	 * This saves us one str instruction.
	 */
	if (dst_lo != src_lo || off != 32)
		arm_bpf_put_reg32(dst_lo, rd, ctx);

	if (!is64) {
		if (!ctx->prog->aux->verifier_zext)
			/* Zero out high 4 bytes */
			emit_a32_mov_i(dst_hi, 0, ctx);
	} else {
		if (is_stacked(dst_hi)) {
			emit(ARM_ASR_I(tmp[0], rd, 31), ctx);
			arm_bpf_put_reg32(dst_hi, tmp[0], ctx);
		} else {
			emit(ARM_ASR_I(dst_hi, rd, 31), ctx);
		}
	}
}

/* Shift operations */
static inline void emit_a32_alu_i(const s8 dst, const u32 val,
				struct jit_ctx *ctx, const u8 op) {
	const s8 *tmp = bpf2a32[TMP_REG_1];
	s8 rd;

	rd = arm_bpf_get_reg32(dst, tmp[0], ctx);

	/* Do shift operation */
	switch (op) {
	case BPF_LSH:
		emit(ARM_LSL_I(rd, rd, val), ctx);
		break;
	case BPF_RSH:
		emit(ARM_LSR_I(rd, rd, val), ctx);
		break;
	case BPF_ARSH:
		emit(ARM_ASR_I(rd, rd, val), ctx);
		break;
	case BPF_NEG:
		emit(ARM_RSB_I(rd, rd, val), ctx);
		break;
	}

	arm_bpf_put_reg32(dst, rd, ctx);
}

/* dst = ~dst (64 bit) */
static inline void emit_a32_neg64(const s8 dst[],
				struct jit_ctx *ctx){
	const s8 *tmp = bpf2a32[TMP_REG_1];
	const s8 *rd;

	/* Setup Operand */
	rd = arm_bpf_get_reg64(dst, tmp, ctx);

	/* Do Negate Operation */
	emit(ARM_RSBS_I(rd[1], rd[1], 0), ctx);
	emit(ARM_RSC_I(rd[0], rd[0], 0), ctx);

	arm_bpf_put_reg64(dst, rd, ctx);
}

/* dst = dst << src */
static inline void emit_a32_lsh_r64(const s8 dst[], const s8 src[],
				    struct jit_ctx *ctx) {
	const s8 *tmp = bpf2a32[TMP_REG_1];
	const s8 *tmp2 = bpf2a32[TMP_REG_2];
	const s8 *rd;
	s8 rt;

	/* Setup Operands */
	rt = arm_bpf_get_reg32(src_lo, tmp2[1], ctx);
	rd = arm_bpf_get_reg64(dst, tmp, ctx);

	/* Do LSH operation */
	emit(ARM_SUB_I(ARM_IP, rt, 32), ctx);
	emit(ARM_RSB_I(tmp2[0], rt, 32), ctx);
	emit(ARM_MOV_SR(ARM_LR, rd[0], SRTYPE_ASL, rt), ctx);
	emit(ARM_ORR_SR(ARM_LR, ARM_LR, rd[1], SRTYPE_ASL, ARM_IP), ctx);
	emit(ARM_ORR_SR(ARM_IP, ARM_LR, rd[1], SRTYPE_LSR, tmp2[0]), ctx);
	emit(ARM_MOV_SR(ARM_LR, rd[1], SRTYPE_ASL, rt), ctx);

	arm_bpf_put_reg32(dst_lo, ARM_LR, ctx);
	arm_bpf_put_reg32(dst_hi, ARM_IP, ctx);
}

/* dst = dst >> src (signed)*/
static inline void emit_a32_arsh_r64(const s8 dst[], const s8 src[],
				     struct jit_ctx *ctx) {
	const s8 *tmp = bpf2a32[TMP_REG_1];
	const s8 *tmp2 = bpf2a32[TMP_REG_2];
	const s8 *rd;
	s8 rt;

	/* Setup Operands */
	rt = arm_bpf_get_reg32(src_lo, tmp2[1], ctx);
	rd = arm_bpf_get_reg64(dst, tmp, ctx);

	/* Do the ARSH operation */
	emit(ARM_RSB_I(ARM_IP, rt, 32), ctx);
	emit(ARM_SUBS_I(tmp2[0], rt, 32), ctx);
	emit(ARM_MOV_SR(ARM_LR, rd[1], SRTYPE_LSR, rt), ctx);
	emit(ARM_ORR_SR(ARM_LR, ARM_LR, rd[0], SRTYPE_ASL, ARM_IP), ctx);
	_emit(ARM_COND_PL,
	      ARM_ORR_SR(ARM_LR, ARM_LR, rd[0], SRTYPE_ASR, tmp2[0]), ctx);
	emit(ARM_MOV_SR(ARM_IP, rd[0], SRTYPE_ASR, rt), ctx);

	arm_bpf_put_reg32(dst_lo, ARM_LR, ctx);
	arm_bpf_put_reg32(dst_hi, ARM_IP, ctx);
}

/* dst = dst >> src */
static inline void emit_a32_rsh_r64(const s8 dst[], const s8 src[],
				    struct jit_ctx *ctx) {
	const s8 *tmp = bpf2a32[TMP_REG_1];
	const s8 *tmp2 = bpf2a32[TMP_REG_2];
	const s8 *rd;
	s8 rt;

	/* Setup Operands */
	rt = arm_bpf_get_reg32(src_lo, tmp2[1], ctx);
	rd = arm_bpf_get_reg64(dst, tmp, ctx);

	/* Do RSH operation */
	emit(ARM_RSB_I(ARM_IP, rt, 32), ctx);
	emit(ARM_SUBS_I(tmp2[0], rt, 32), ctx);
	emit(ARM_MOV_SR(ARM_LR, rd[1], SRTYPE_LSR, rt), ctx);
	emit(ARM_ORR_SR(ARM_LR, ARM_LR, rd[0], SRTYPE_ASL, ARM_IP), ctx);
	emit(ARM_ORR_SR(ARM_LR, ARM_LR, rd[0], SRTYPE_LSR, tmp2[0]), ctx);
	emit(ARM_MOV_SR(ARM_IP, rd[0], SRTYPE_LSR, rt), ctx);

	arm_bpf_put_reg32(dst_lo, ARM_LR, ctx);
	arm_bpf_put_reg32(dst_hi, ARM_IP, ctx);
}

/* dst = dst << val */
static inline void emit_a32_lsh_i64(const s8 dst[],
				    const u32 val, struct jit_ctx *ctx){
	const s8 *tmp = bpf2a32[TMP_REG_1];
	const s8 *tmp2 = bpf2a32[TMP_REG_2];
	const s8 *rd;

	/* Setup operands */
	rd = arm_bpf_get_reg64(dst, tmp, ctx);

	/* Do LSH operation */
	if (val < 32) {
		emit(ARM_MOV_SI(tmp2[0], rd[0], SRTYPE_ASL, val), ctx);
		emit(ARM_ORR_SI(rd[0], tmp2[0], rd[1], SRTYPE_LSR, 32 - val), ctx);
		emit(ARM_MOV_SI(rd[1], rd[1], SRTYPE_ASL, val), ctx);
	} else {
		if (val == 32)
			emit(ARM_MOV_R(rd[0], rd[1]), ctx);
		else
			emit(ARM_MOV_SI(rd[0], rd[1], SRTYPE_ASL, val - 32), ctx);
		emit(ARM_EOR_R(rd[1], rd[1], rd[1]), ctx);
	}

	arm_bpf_put_reg64(dst, rd, ctx);
}

/* dst = dst >> val */
static inline void emit_a32_rsh_i64(const s8 dst[],
				    const u32 val, struct jit_ctx *ctx) {
	const s8 *tmp = bpf2a32[TMP_REG_1];
	const s8 *tmp2 = bpf2a32[TMP_REG_2];
	const s8 *rd;

	/* Setup operands */
	rd = arm_bpf_get_reg64(dst, tmp, ctx);

	/* Do LSR operation */
	if (val == 0) {
		/* An immediate value of 0 encodes a shift amount of 32
		 * for LSR. To shift by 0, don't do anything.
		 */
	} else if (val < 32) {
		emit(ARM_MOV_SI(tmp2[1], rd[1], SRTYPE_LSR, val), ctx);
		emit(ARM_ORR_SI(rd[1], tmp2[1], rd[0], SRTYPE_ASL, 32 - val), ctx);
		emit(ARM_MOV_SI(rd[0], rd[0], SRTYPE_LSR, val), ctx);
	} else if (val == 32) {
		emit(ARM_MOV_R(rd[1], rd[0]), ctx);
		emit(ARM_MOV_I(rd[0], 0), ctx);
	} else {
		emit(ARM_MOV_SI(rd[1], rd[0], SRTYPE_LSR, val - 32), ctx);
		emit(ARM_MOV_I(rd[0], 0), ctx);
	}

	arm_bpf_put_reg64(dst, rd, ctx);
}

/* dst = dst >> val (signed) */
static inline void emit_a32_arsh_i64(const s8 dst[],
				     const u32 val, struct jit_ctx *ctx){
	const s8 *tmp = bpf2a32[TMP_REG_1];
	const s8 *tmp2 = bpf2a32[TMP_REG_2];
	const s8 *rd;

	/* Setup operands */
	rd = arm_bpf_get_reg64(dst, tmp, ctx);

	/* Do ARSH operation */
	if (val == 0) {
		/* An immediate value of 0 encodes a shift amount of 32
		 * for ASR. To shift by 0, don't do anything.
		 */
	} else if (val < 32) {
		emit(ARM_MOV_SI(tmp2[1], rd[1], SRTYPE_LSR, val), ctx);
		emit(ARM_ORR_SI(rd[1], tmp2[1], rd[0], SRTYPE_ASL, 32 - val), ctx);
		emit(ARM_MOV_SI(rd[0], rd[0], SRTYPE_ASR, val), ctx);
	} else if (val == 32) {
		emit(ARM_MOV_R(rd[1], rd[0]), ctx);
		emit(ARM_MOV_SI(rd[0], rd[0], SRTYPE_ASR, 31), ctx);
	} else {
		emit(ARM_MOV_SI(rd[1], rd[0], SRTYPE_ASR, val - 32), ctx);
		emit(ARM_MOV_SI(rd[0], rd[0], SRTYPE_ASR, 31), ctx);
	}

	arm_bpf_put_reg64(dst, rd, ctx);
}

static inline void emit_a32_mul_r64(const s8 dst[], const s8 src[],
				    struct jit_ctx *ctx) {
	const s8 *tmp = bpf2a32[TMP_REG_1];
	const s8 *tmp2 = bpf2a32[TMP_REG_2];
	const s8 *rd, *rt;

	/* Setup operands for multiplication */
	rd = arm_bpf_get_reg64(dst, tmp, ctx);
	rt = arm_bpf_get_reg64(src, tmp2, ctx);

	/* Do Multiplication */
	emit(ARM_MUL(ARM_IP, rd[1], rt[0]), ctx);
	emit(ARM_MUL(ARM_LR, rd[0], rt[1]), ctx);
	emit(ARM_ADD_R(ARM_LR, ARM_IP, ARM_LR), ctx);

	emit(ARM_UMULL(ARM_IP, rd[0], rd[1], rt[1]), ctx);
	emit(ARM_ADD_R(rd[0], ARM_LR, rd[0]), ctx);

	arm_bpf_put_reg32(dst_lo, ARM_IP, ctx);
	arm_bpf_put_reg32(dst_hi, rd[0], ctx);
}

static bool is_ldst_imm(s16 off, const u8 size)
{
	s16 off_max = 0;

	switch (size) {
	case BPF_B:
	case BPF_W:
		off_max = 0xfff;
		break;
	case BPF_H:
		off_max = 0xff;
		break;
	case BPF_DW:
		/* Need to make sure off+4 does not overflow. */
		off_max = 0xfff - 4;
		break;
	}
	return -off_max <= off && off <= off_max;
}

static bool is_ldst_imm8(s16 off, const u8 size)
{
	s16 off_max = 0;

	switch (size) {
	case BPF_B:
		off_max = 0xff;
		break;
	case BPF_W:
		off_max = 0xfff;
		break;
	case BPF_H:
		off_max = 0xff;
		break;
	}
	return -off_max <= off && off <= off_max;
}

/* *(size *)(dst + off) = src */
static inline void emit_str_r(const s8 dst, const s8 src[],
			      s16 off, struct jit_ctx *ctx, const u8 sz){
	const s8 *tmp = bpf2a32[TMP_REG_1];
	s8 rd;

	rd = arm_bpf_get_reg32(dst, tmp[1], ctx);

	if (!is_ldst_imm(off, sz)) {
		emit_a32_mov_i(tmp[0], off, ctx);
		emit(ARM_ADD_R(tmp[0], tmp[0], rd), ctx);
		rd = tmp[0];
		off = 0;
	}
	switch (sz) {
	case BPF_B:
		/* Store a Byte */
		emit(ARM_STRB_I(src_lo, rd, off), ctx);
		break;
	case BPF_H:
		/* Store a HalfWord */
		emit(ARM_STRH_I(src_lo, rd, off), ctx);
		break;
	case BPF_W:
		/* Store a Word */
		emit(ARM_STR_I(src_lo, rd, off), ctx);
		break;
	case BPF_DW:
		/* Store a Double Word */
		emit(ARM_STR_I(src_lo, rd, off), ctx);
		emit(ARM_STR_I(src_hi, rd, off + 4), ctx);
		break;
	}
}

/* dst = *(size*)(src + off) */
static inline void emit_ldx_r(const s8 dst[], const s8 src,
			      s16 off, struct jit_ctx *ctx, const u8 sz){
	const s8 *tmp = bpf2a32[TMP_REG_1];
	const s8 *rd = is_stacked(dst_lo) ? tmp : dst;
	s8 rm = src;

	if (!is_ldst_imm(off, sz)) {
		emit_a32_mov_i(tmp[0], off, ctx);
		emit(ARM_ADD_R(tmp[0], tmp[0], src), ctx);
		rm = tmp[0];
		off = 0;
	} else if (rd[1] == rm) {
		emit(ARM_MOV_R(tmp[0], rm), ctx);
		rm = tmp[0];
	}
	switch (sz) {
	case BPF_B:
		/* Load a Byte */
		emit(ARM_LDRB_I(rd[1], rm, off), ctx);
		if (!ctx->prog->aux->verifier_zext)
			emit_a32_mov_i(rd[0], 0, ctx);
		break;
	case BPF_H:
		/* Load a HalfWord */
		emit(ARM_LDRH_I(rd[1], rm, off), ctx);
		if (!ctx->prog->aux->verifier_zext)
			emit_a32_mov_i(rd[0], 0, ctx);
		break;
	case BPF_W:
		/* Load a Word */
		emit(ARM_LDR_I(rd[1], rm, off), ctx);
		if (!ctx->prog->aux->verifier_zext)
			emit_a32_mov_i(rd[0], 0, ctx);
		break;
	case BPF_DW:
		/* Load a Double Word */
		emit(ARM_LDR_I(rd[1], rm, off), ctx);
		emit(ARM_LDR_I(rd[0], rm, off + 4), ctx);
		break;
	}
	arm_bpf_put_reg64(dst, rd, ctx);
}

/* dst = *(signed size*)(src + off) */
static inline void emit_ldsx_r(const s8 dst[], const s8 src,
			       s16 off, struct jit_ctx *ctx, const u8 sz){
	const s8 *tmp = bpf2a32[TMP_REG_1];
	const s8 *rd = is_stacked(dst_lo) ? tmp : dst;
	s8 rm = src;
	int add_off;

	if (!is_ldst_imm8(off, sz)) {
		/*
		 * offset does not fit in the load/store immediate,
		 * construct an ADD instruction to apply the offset.
		 */
		add_off = imm8m(off);
		if (add_off > 0) {
			emit(ARM_ADD_I(tmp[0], src, add_off), ctx);
			rm = tmp[0];
		} else {
			emit_a32_mov_i(tmp[0], off, ctx);
			emit(ARM_ADD_R(tmp[0], tmp[0], src), ctx);
			rm = tmp[0];
		}
		off = 0;
	}

	switch (sz) {
	case BPF_B:
		/* Load a Byte with sign extension*/
		emit(ARM_LDRSB_I(rd[1], rm, off), ctx);
		break;
	case BPF_H:
		/* Load a HalfWord with sign extension*/
		emit(ARM_LDRSH_I(rd[1], rm, off), ctx);
		break;
	case BPF_W:
		/* Load a Word*/
		emit(ARM_LDR_I(rd[1], rm, off), ctx);
		break;
	}
	/* Carry the sign extension to upper 32 bits */
	emit(ARM_ASR_I(rd[0], rd[1], 31), ctx);
	arm_bpf_put_reg64(dst, rd, ctx);
}

/* Arithmatic Operation */
static inline void emit_ar_r(const u8 rd, const u8 rt, const u8 rm,
			     const u8 rn, struct jit_ctx *ctx, u8 op,
			     bool is_jmp64) {
	switch (op) {
	case BPF_JSET:
		if (is_jmp64) {
			emit(ARM_AND_R(ARM_IP, rt, rn), ctx);
			emit(ARM_AND_R(ARM_LR, rd, rm), ctx);
			emit(ARM_ORRS_R(ARM_IP, ARM_LR, ARM_IP), ctx);
		} else {
			emit(ARM_ANDS_R(ARM_IP, rt, rn), ctx);
		}
		break;
	case BPF_JEQ:
	case BPF_JNE:
	case BPF_JGT:
	case BPF_JGE:
	case BPF_JLE:
	case BPF_JLT:
		if (is_jmp64) {
			emit(ARM_CMP_R(rd, rm), ctx);
			/* Only compare low halve if high halve are equal. */
			_emit(ARM_COND_EQ, ARM_CMP_R(rt, rn), ctx);
		} else {
			emit(ARM_CMP_R(rt, rn), ctx);
		}
		break;
	case BPF_JSLE:
	case BPF_JSGT:
		emit(ARM_CMP_R(rn, rt), ctx);
		if (is_jmp64)
			emit(ARM_SBCS_R(ARM_IP, rm, rd), ctx);
		break;
	case BPF_JSLT:
	case BPF_JSGE:
		emit(ARM_CMP_R(rt, rn), ctx);
		if (is_jmp64)
			emit(ARM_SBCS_R(ARM_IP, rd, rm), ctx);
		break;
	}
}

static int out_offset = -1; /* initialized on the first pass of build_body() */
static int emit_bpf_tail_call(struct jit_ctx *ctx)
{

	/* bpf_tail_call(void *prog_ctx, struct bpf_array *array, u64 index) */
	const s8 *r2 = bpf2a32[BPF_REG_2];
	const s8 *r3 = bpf2a32[BPF_REG_3];
	const s8 *tmp = bpf2a32[TMP_REG_1];
	const s8 *tmp2 = bpf2a32[TMP_REG_2];
	const s8 *tcc = bpf2a32[TCALL_CNT];
	const s8 *tc;
	const int idx0 = ctx->idx;
#define cur_offset (ctx->idx - idx0)
#define jmp_offset (out_offset - (cur_offset) - 2)
	u32 lo, hi;
	s8 r_array, r_index;
	int off;

	/* if (index >= array->map.max_entries)
	 *	goto out;
	 */
	BUILD_BUG_ON(offsetof(struct bpf_array, map.max_entries) >
		     ARM_INST_LDST__IMM12);
	off = offsetof(struct bpf_array, map.max_entries);
	r_array = arm_bpf_get_reg32(r2[1], tmp2[0], ctx);
	/* index is 32-bit for arrays */
	r_index = arm_bpf_get_reg32(r3[1], tmp2[1], ctx);
	/* array->map.max_entries */
	emit(ARM_LDR_I(tmp[1], r_array, off), ctx);
	/* index >= array->map.max_entries */
	emit(ARM_CMP_R(r_index, tmp[1]), ctx);
	_emit(ARM_COND_CS, ARM_B(jmp_offset), ctx);

	/* tmp2[0] = array, tmp2[1] = index */

	/*
	 * if (tail_call_cnt >= MAX_TAIL_CALL_CNT)
	 *	goto out;
	 * tail_call_cnt++;
	 */
	lo = (u32)MAX_TAIL_CALL_CNT;
	hi = (u32)((u64)MAX_TAIL_CALL_CNT >> 32);
	tc = arm_bpf_get_reg64(tcc, tmp, ctx);
	emit(ARM_CMP_I(tc[0], hi), ctx);
	_emit(ARM_COND_EQ, ARM_CMP_I(tc[1], lo), ctx);
	_emit(ARM_COND_CS, ARM_B(jmp_offset), ctx);
	emit(ARM_ADDS_I(tc[1], tc[1], 1), ctx);
	emit(ARM_ADC_I(tc[0], tc[0], 0), ctx);
	arm_bpf_put_reg64(tcc, tmp, ctx);

	/* prog = array->ptrs[index]
	 * if (prog == NULL)
	 *	goto out;
	 */
	BUILD_BUG_ON(imm8m(offsetof(struct bpf_array, ptrs)) < 0);
	off = imm8m(offsetof(struct bpf_array, ptrs));
	emit(ARM_ADD_I(tmp[1], r_array, off), ctx);
	emit(ARM_LDR_R_SI(tmp[1], tmp[1], r_index, SRTYPE_ASL, 2), ctx);
	emit(ARM_CMP_I(tmp[1], 0), ctx);
	_emit(ARM_COND_EQ, ARM_B(jmp_offset), ctx);

	/* goto *(prog->bpf_func + prologue_size); */
	BUILD_BUG_ON(offsetof(struct bpf_prog, bpf_func) >
		     ARM_INST_LDST__IMM12);
	off = offsetof(struct bpf_prog, bpf_func);
	emit(ARM_LDR_I(tmp[1], tmp[1], off), ctx);
	emit(ARM_ADD_I(tmp[1], tmp[1], ctx->prologue_bytes), ctx);
	emit_bx_r(tmp[1], ctx);

	/* out: */
	if (out_offset == -1)
		out_offset = cur_offset;
	if (cur_offset != out_offset) {
		pr_err_once("tail_call out_offset = %d, expected %d!\n",
			    cur_offset, out_offset);
		return -1;
	}
	return 0;
#undef cur_offset
#undef jmp_offset
}

/* 0xabcd => 0xcdab */
static inline void emit_rev16(const u8 rd, const u8 rn, struct jit_ctx *ctx)
{
#if __LINUX_ARM_ARCH__ < 6
	const s8 *tmp2 = bpf2a32[TMP_REG_2];

	emit(ARM_AND_I(tmp2[1], rn, 0xff), ctx);
	emit(ARM_MOV_SI(tmp2[0], rn, SRTYPE_LSR, 8), ctx);
	emit(ARM_AND_I(tmp2[0], tmp2[0], 0xff), ctx);
	emit(ARM_ORR_SI(rd, tmp2[0], tmp2[1], SRTYPE_LSL, 8), ctx);
#else /* ARMv6+ */
	emit(ARM_REV16(rd, rn), ctx);
#endif
}

/* 0xabcdefgh => 0xghefcdab */
static inline void emit_rev32(const u8 rd, const u8 rn, struct jit_ctx *ctx)
{
#if __LINUX_ARM_ARCH__ < 6
	const s8 *tmp2 = bpf2a32[TMP_REG_2];

	emit(ARM_AND_I(tmp2[1], rn, 0xff), ctx);
	emit(ARM_MOV_SI(tmp2[0], rn, SRTYPE_LSR, 24), ctx);
	emit(ARM_ORR_SI(ARM_IP, tmp2[0], tmp2[1], SRTYPE_LSL, 24), ctx);

	emit(ARM_MOV_SI(tmp2[1], rn, SRTYPE_LSR, 8), ctx);
	emit(ARM_AND_I(tmp2[1], tmp2[1], 0xff), ctx);
	emit(ARM_MOV_SI(tmp2[0], rn, SRTYPE_LSR, 16), ctx);
	emit(ARM_AND_I(tmp2[0], tmp2[0], 0xff), ctx);
	emit(ARM_MOV_SI(tmp2[0], tmp2[0], SRTYPE_LSL, 8), ctx);
	emit(ARM_ORR_SI(tmp2[0], tmp2[0], tmp2[1], SRTYPE_LSL, 16), ctx);
	emit(ARM_ORR_R(rd, ARM_IP, tmp2[0]), ctx);

#else /* ARMv6+ */
	emit(ARM_REV(rd, rn), ctx);
#endif
}

// push the scratch stack register on top of the stack
static inline void emit_push_r64(const s8 src[], struct jit_ctx *ctx)
{
	const s8 *tmp2 = bpf2a32[TMP_REG_2];
	const s8 *rt;
	u16 reg_set = 0;

	rt = arm_bpf_get_reg64(src, tmp2, ctx);

	reg_set = (1 << rt[1]) | (1 << rt[0]);
	emit(ARM_PUSH(reg_set), ctx);
}

static void build_prologue(struct jit_ctx *ctx)
{
	const s8 arm_r0 = bpf2a32[BPF_REG_0][1];
	const s8 *bpf_r1 = bpf2a32[BPF_REG_1];
	const s8 *bpf_fp = bpf2a32[BPF_REG_FP];
	const s8 *tcc = bpf2a32[TCALL_CNT];

	/* Save callee saved registers. */
#ifdef CONFIG_FRAME_POINTER
	u16 reg_set = CALLEE_PUSH_MASK | 1 << ARM_IP | 1 << ARM_PC;
	emit(ARM_MOV_R(ARM_IP, ARM_SP), ctx);
	emit(ARM_PUSH(reg_set), ctx);
	emit(ARM_SUB_I(ARM_FP, ARM_IP, 4), ctx);
#else
	emit(ARM_PUSH(CALLEE_PUSH_MASK), ctx);
	emit(ARM_MOV_R(ARM_FP, ARM_SP), ctx);
#endif
	/* mov r3, #0 */
	/* sub r2, sp, #SCRATCH_SIZE */
	emit(ARM_MOV_I(bpf_r1[0], 0), ctx);
	emit(ARM_SUB_I(bpf_r1[1], ARM_SP, SCRATCH_SIZE), ctx);

	ctx->stack_size = imm8m(STACK_SIZE);

	/* Set up function call stack */
	emit(ARM_SUB_I(ARM_SP, ARM_SP, ctx->stack_size), ctx);

	/* Set up BPF prog stack base register */
	emit_a32_mov_r64(true, bpf_fp, bpf_r1, ctx);

	/* Initialize Tail Count */
	emit(ARM_MOV_I(bpf_r1[1], 0), ctx);
	emit_a32_mov_r64(true, tcc, bpf_r1, ctx);

	/* Move BPF_CTX to BPF_R1 */
	emit(ARM_MOV_R(bpf_r1[1], arm_r0), ctx);

	/* end of prologue */
}

/* restore callee saved registers. */
static void build_epilogue(struct jit_ctx *ctx)
{
#ifdef CONFIG_FRAME_POINTER
	/* When using frame pointers, some additional registers need to
	 * be loaded. */
	u16 reg_set = CALLEE_POP_MASK | 1 << ARM_SP;
	emit(ARM_SUB_I(ARM_SP, ARM_FP, hweight16(reg_set) * 4), ctx);
	emit(ARM_LDM(ARM_SP, reg_set), ctx);
#else
	/* Restore callee saved registers. */
	emit(ARM_MOV_R(ARM_SP, ARM_FP), ctx);
	emit(ARM_POP(CALLEE_POP_MASK), ctx);
#endif
}

/*
 * Convert an eBPF instruction to native instruction, i.e
 * JITs an eBPF instruction.
 * Returns :
 *	0  - Successfully JITed an 8-byte eBPF instruction
 *	>0 - Successfully JITed a 16-byte eBPF instruction
 *	<0 - Failed to JIT.
 */
static int build_insn(const struct bpf_insn *insn, struct jit_ctx *ctx)
{
	const u8 code = insn->code;
	const s8 *dst = bpf2a32[insn->dst_reg];
	const s8 *src = bpf2a32[insn->src_reg];
	const s8 *tmp = bpf2a32[TMP_REG_1];
	const s8 *tmp2 = bpf2a32[TMP_REG_2];
	const s16 off = insn->off;
	const s32 imm = insn->imm;
	const int i = insn - ctx->prog->insnsi;
	const bool is64 = BPF_CLASS(code) == BPF_ALU64;
	const s8 *rd, *rs;
	s8 rd_lo, rt, rm, rn;
	s32 jmp_offset;

#define check_imm(bits, imm) do {				\
	if ((imm) >= (1 << ((bits) - 1)) ||			\
	    (imm) < -(1 << ((bits) - 1))) {			\
		pr_info("[%2d] imm=%d(0x%x) out of range\n",	\
			i, imm, imm);				\
		return -EINVAL;					\
	}							\
} while (0)
#define check_imm24(imm) check_imm(24, imm)

	switch (code) {
	/* ALU operations */

	/* dst = src */
	case BPF_ALU | BPF_MOV | BPF_K:
	case BPF_ALU | BPF_MOV | BPF_X:
	case BPF_ALU64 | BPF_MOV | BPF_K:
	case BPF_ALU64 | BPF_MOV | BPF_X:
		switch (BPF_SRC(code)) {
		case BPF_X:
			if (imm == 1) {
				/* Special mov32 for zext */
				emit_a32_mov_i(dst_hi, 0, ctx);
				break;
			}
			if (insn->off)
				emit_a32_movsx_r64(is64, insn->off, dst, src, ctx);
			else
				emit_a32_mov_r64(is64, dst, src, ctx);
			break;
		case BPF_K:
			/* Sign-extend immediate value to destination reg */
			emit_a32_mov_se_i64(is64, dst, imm, ctx);
			break;
		}
		break;
	/* dst = dst + src/imm */
	/* dst = dst - src/imm */
	/* dst = dst | src/imm */
	/* dst = dst & src/imm */
	/* dst = dst ^ src/imm */
	/* dst = dst * src/imm */
	/* dst = dst << src */
	/* dst = dst >> src */
	case BPF_ALU | BPF_ADD | BPF_K:
	case BPF_ALU | BPF_ADD | BPF_X:
	case BPF_ALU | BPF_SUB | BPF_K:
	case BPF_ALU | BPF_SUB | BPF_X:
	case BPF_ALU | BPF_OR | BPF_K:
	case BPF_ALU | BPF_OR | BPF_X:
	case BPF_ALU | BPF_AND | BPF_K:
	case BPF_ALU | BPF_AND | BPF_X:
	case BPF_ALU | BPF_XOR | BPF_K:
	case BPF_ALU | BPF_XOR | BPF_X:
	case BPF_ALU | BPF_MUL | BPF_K:
	case BPF_ALU | BPF_MUL | BPF_X:
	case BPF_ALU | BPF_LSH | BPF_X:
	case BPF_ALU | BPF_RSH | BPF_X:
	case BPF_ALU | BPF_ARSH | BPF_X:
	case BPF_ALU64 | BPF_ADD | BPF_K:
	case BPF_ALU64 | BPF_ADD | BPF_X:
	case BPF_ALU64 | BPF_SUB | BPF_K:
	case BPF_ALU64 | BPF_SUB | BPF_X:
	case BPF_ALU64 | BPF_OR | BPF_K:
	case BPF_ALU64 | BPF_OR | BPF_X:
	case BPF_ALU64 | BPF_AND | BPF_K:
	case BPF_ALU64 | BPF_AND | BPF_X:
	case BPF_ALU64 | BPF_XOR | BPF_K:
	case BPF_ALU64 | BPF_XOR | BPF_X:
		switch (BPF_SRC(code)) {
		case BPF_X:
			emit_a32_alu_r64(is64, dst, src, ctx, BPF_OP(code));
			break;
		case BPF_K:
			/* Move immediate value to the temporary register
			 * and then do the ALU operation on the temporary
			 * register as this will sign-extend the immediate
			 * value into temporary reg and then it would be
			 * safe to do the operation on it.
			 */
			emit_a32_mov_se_i64(is64, tmp2, imm, ctx);
			emit_a32_alu_r64(is64, dst, tmp2, ctx, BPF_OP(code));
			break;
		}
		break;
	/* dst = dst / src(imm) */
	/* dst = dst % src(imm) */
	case BPF_ALU | BPF_DIV | BPF_K:
	case BPF_ALU | BPF_DIV | BPF_X:
	case BPF_ALU | BPF_MOD | BPF_K:
	case BPF_ALU | BPF_MOD | BPF_X:
		rd_lo = arm_bpf_get_reg32(dst_lo, tmp2[1], ctx);
		switch (BPF_SRC(code)) {
		case BPF_X:
			rt = arm_bpf_get_reg32(src_lo, tmp2[0], ctx);
			break;
		case BPF_K:
			rt = tmp2[0];
			emit_a32_mov_i(rt, imm, ctx);
			break;
		default:
			rt = src_lo;
			break;
		}
		emit_udivmod(rd_lo, rd_lo, rt, ctx, BPF_OP(code), off);
		arm_bpf_put_reg32(dst_lo, rd_lo, ctx);
		if (!ctx->prog->aux->verifier_zext)
			emit_a32_mov_i(dst_hi, 0, ctx);
		break;
	case BPF_ALU64 | BPF_DIV | BPF_K:
	case BPF_ALU64 | BPF_DIV | BPF_X:
	case BPF_ALU64 | BPF_MOD | BPF_K:
	case BPF_ALU64 | BPF_MOD | BPF_X:
		rd = arm_bpf_get_reg64(dst, tmp2, ctx);
		switch (BPF_SRC(code)) {
		case BPF_X:
			rs = arm_bpf_get_reg64(src, tmp, ctx);
			break;
		case BPF_K:
			rs = tmp;
			emit_a32_mov_se_i64(is64, rs, imm, ctx);
			break;
		}
		emit_udivmod64(rd, rd, rs, ctx, BPF_OP(code), off);
		arm_bpf_put_reg64(dst, rd, ctx);
		break;
	/* dst = dst << imm */
	/* dst = dst >> imm */
	/* dst = dst >> imm (signed) */
	case BPF_ALU | BPF_LSH | BPF_K:
	case BPF_ALU | BPF_RSH | BPF_K:
	case BPF_ALU | BPF_ARSH | BPF_K:
		if (unlikely(imm > 31))
			return -EINVAL;
		if (imm)
			emit_a32_alu_i(dst_lo, imm, ctx, BPF_OP(code));
		if (!ctx->prog->aux->verifier_zext)
			emit_a32_mov_i(dst_hi, 0, ctx);
		break;
	/* dst = dst << imm */
	case BPF_ALU64 | BPF_LSH | BPF_K:
		if (unlikely(imm > 63))
			return -EINVAL;
		emit_a32_lsh_i64(dst, imm, ctx);
		break;
	/* dst = dst >> imm */
	case BPF_ALU64 | BPF_RSH | BPF_K:
		if (unlikely(imm > 63))
			return -EINVAL;
		emit_a32_rsh_i64(dst, imm, ctx);
		break;
	/* dst = dst << src */
	case BPF_ALU64 | BPF_LSH | BPF_X:
		emit_a32_lsh_r64(dst, src, ctx);
		break;
	/* dst = dst >> src */
	case BPF_ALU64 | BPF_RSH | BPF_X:
		emit_a32_rsh_r64(dst, src, ctx);
		break;
	/* dst = dst >> src (signed) */
	case BPF_ALU64 | BPF_ARSH | BPF_X:
		emit_a32_arsh_r64(dst, src, ctx);
		break;
	/* dst = dst >> imm (signed) */
	case BPF_ALU64 | BPF_ARSH | BPF_K:
		if (unlikely(imm > 63))
			return -EINVAL;
		emit_a32_arsh_i64(dst, imm, ctx);
		break;
	/* dst = ~dst */
	case BPF_ALU | BPF_NEG:
		emit_a32_alu_i(dst_lo, 0, ctx, BPF_OP(code));
		if (!ctx->prog->aux->verifier_zext)
			emit_a32_mov_i(dst_hi, 0, ctx);
		break;
	/* dst = ~dst (64 bit) */
	case BPF_ALU64 | BPF_NEG:
		emit_a32_neg64(dst, ctx);
		break;
	/* dst = dst * src/imm */
	case BPF_ALU64 | BPF_MUL | BPF_X:
	case BPF_ALU64 | BPF_MUL | BPF_K:
		switch (BPF_SRC(code)) {
		case BPF_X:
			emit_a32_mul_r64(dst, src, ctx);
			break;
		case BPF_K:
			/* Move immediate value to the temporary register
			 * and then do the multiplication on it as this
			 * will sign-extend the immediate value into temp
			 * reg then it would be safe to do the operation
			 * on it.
			 */
			emit_a32_mov_se_i64(is64, tmp2, imm, ctx);
			emit_a32_mul_r64(dst, tmp2, ctx);
			break;
		}
		break;
	/* dst = htole(dst) */
	/* dst = htobe(dst) */
	case BPF_ALU | BPF_END | BPF_FROM_LE: /* also BPF_TO_LE */
	case BPF_ALU | BPF_END | BPF_FROM_BE: /* also BPF_TO_BE */
	/* dst = bswap(dst) */
	case BPF_ALU64 | BPF_END | BPF_FROM_LE: /* also BPF_TO_LE */
		rd = arm_bpf_get_reg64(dst, tmp, ctx);
		if (BPF_SRC(code) == BPF_FROM_LE && BPF_CLASS(code) != BPF_ALU64)
			goto emit_bswap_uxt;
		switch (imm) {
		case 16:
			emit_rev16(rd[1], rd[1], ctx);
			goto emit_bswap_uxt;
		case 32:
			emit_rev32(rd[1], rd[1], ctx);
			goto emit_bswap_uxt;
		case 64:
			emit_rev32(ARM_LR, rd[1], ctx);
			emit_rev32(rd[1], rd[0], ctx);
			emit(ARM_MOV_R(rd[0], ARM_LR), ctx);
			break;
		}
		goto exit;
emit_bswap_uxt:
		switch (imm) {
		case 16:
			/* zero-extend 16 bits into 64 bits */
#if __LINUX_ARM_ARCH__ < 6
			emit_a32_mov_i(tmp2[1], 0xffff, ctx);
			emit(ARM_AND_R(rd[1], rd[1], tmp2[1]), ctx);
#else /* ARMv6+ */
			emit(ARM_UXTH(rd[1], rd[1]), ctx);
#endif
			if (!ctx->prog->aux->verifier_zext)
				emit(ARM_EOR_R(rd[0], rd[0], rd[0]), ctx);
			break;
		case 32:
			/* zero-extend 32 bits into 64 bits */
			if (!ctx->prog->aux->verifier_zext)
				emit(ARM_EOR_R(rd[0], rd[0], rd[0]), ctx);
			break;
		case 64:
			/* nop */
			break;
		}
exit:
		arm_bpf_put_reg64(dst, rd, ctx);
		break;
	/* dst = imm64 */
	case BPF_LD | BPF_IMM | BPF_DW:
	{
		u64 val = (u32)imm | (u64)insn[1].imm << 32;

		emit_a32_mov_i64(dst, val, ctx);

		return 1;
	}
	/* LDX: dst = *(size *)(src + off) */
	case BPF_LDX | BPF_MEM | BPF_W:
	case BPF_LDX | BPF_MEM | BPF_H:
	case BPF_LDX | BPF_MEM | BPF_B:
	case BPF_LDX | BPF_MEM | BPF_DW:
	/* LDSX: dst = *(signed size *)(src + off) */
	case BPF_LDX | BPF_MEMSX | BPF_B:
	case BPF_LDX | BPF_MEMSX | BPF_H:
	case BPF_LDX | BPF_MEMSX | BPF_W:
		rn = arm_bpf_get_reg32(src_lo, tmp2[1], ctx);
		if (BPF_MODE(insn->code) == BPF_MEMSX)
			emit_ldsx_r(dst, rn, off, ctx, BPF_SIZE(code));
		else
			emit_ldx_r(dst, rn, off, ctx, BPF_SIZE(code));
		break;
	/* speculation barrier */
	case BPF_ST | BPF_NOSPEC:
		break;
	/* ST: *(size *)(dst + off) = imm */
	case BPF_ST | BPF_MEM | BPF_W:
	case BPF_ST | BPF_MEM | BPF_H:
	case BPF_ST | BPF_MEM | BPF_B:
	case BPF_ST | BPF_MEM | BPF_DW:
		switch (BPF_SIZE(code)) {
		case BPF_DW:
			/* Sign-extend immediate value into temp reg */
			emit_a32_mov_se_i64(true, tmp2, imm, ctx);
			break;
		case BPF_W:
		case BPF_H:
		case BPF_B:
			emit_a32_mov_i(tmp2[1], imm, ctx);
			break;
		}
		emit_str_r(dst_lo, tmp2, off, ctx, BPF_SIZE(code));
		break;
	/* Atomic ops */
	case BPF_STX | BPF_ATOMIC | BPF_W:
	case BPF_STX | BPF_ATOMIC | BPF_DW:
		goto notyet;
	/* STX: *(size *)(dst + off) = src */
	case BPF_STX | BPF_MEM | BPF_W:
	case BPF_STX | BPF_MEM | BPF_H:
	case BPF_STX | BPF_MEM | BPF_B:
	case BPF_STX | BPF_MEM | BPF_DW:
		rs = arm_bpf_get_reg64(src, tmp2, ctx);
		emit_str_r(dst_lo, rs, off, ctx, BPF_SIZE(code));
		break;
	/* PC += off if dst == src */
	/* PC += off if dst > src */
	/* PC += off if dst >= src */
	/* PC += off if dst < src */
	/* PC += off if dst <= src */
	/* PC += off if dst != src */
	/* PC += off if dst > src (signed) */
	/* PC += off if dst >= src (signed) */
	/* PC += off if dst < src (signed) */
	/* PC += off if dst <= src (signed) */
	/* PC += off if dst & src */
	case BPF_JMP | BPF_JEQ | BPF_X:
	case BPF_JMP | BPF_JGT | BPF_X:
	case BPF_JMP | BPF_JGE | BPF_X:
	case BPF_JMP | BPF_JNE | BPF_X:
	case BPF_JMP | BPF_JSGT | BPF_X:
	case BPF_JMP | BPF_JSGE | BPF_X:
	case BPF_JMP | BPF_JSET | BPF_X:
	case BPF_JMP | BPF_JLE | BPF_X:
	case BPF_JMP | BPF_JLT | BPF_X:
	case BPF_JMP | BPF_JSLT | BPF_X:
	case BPF_JMP | BPF_JSLE | BPF_X:
	case BPF_JMP32 | BPF_JEQ | BPF_X:
	case BPF_JMP32 | BPF_JGT | BPF_X:
	case BPF_JMP32 | BPF_JGE | BPF_X:
	case BPF_JMP32 | BPF_JNE | BPF_X:
	case BPF_JMP32 | BPF_JSGT | BPF_X:
	case BPF_JMP32 | BPF_JSGE | BPF_X:
	case BPF_JMP32 | BPF_JSET | BPF_X:
	case BPF_JMP32 | BPF_JLE | BPF_X:
	case BPF_JMP32 | BPF_JLT | BPF_X:
	case BPF_JMP32 | BPF_JSLT | BPF_X:
	case BPF_JMP32 | BPF_JSLE | BPF_X:
		/* Setup source registers */
		rm = arm_bpf_get_reg32(src_hi, tmp2[0], ctx);
		rn = arm_bpf_get_reg32(src_lo, tmp2[1], ctx);
		goto go_jmp;
	/* PC += off if dst == imm */
	/* PC += off if dst > imm */
	/* PC += off if dst >= imm */
	/* PC += off if dst < imm */
	/* PC += off if dst <= imm */
	/* PC += off if dst != imm */
	/* PC += off if dst > imm (signed) */
	/* PC += off if dst >= imm (signed) */
	/* PC += off if dst < imm (signed) */
	/* PC += off if dst <= imm (signed) */
	/* PC += off if dst & imm */
	case BPF_JMP | BPF_JEQ | BPF_K:
	case BPF_JMP | BPF_JGT | BPF_K:
	case BPF_JMP | BPF_JGE | BPF_K:
	case BPF_JMP | BPF_JNE | BPF_K:
	case BPF_JMP | BPF_JSGT | BPF_K:
	case BPF_JMP | BPF_JSGE | BPF_K:
	case BPF_JMP | BPF_JSET | BPF_K:
	case BPF_JMP | BPF_JLT | BPF_K:
	case BPF_JMP | BPF_JLE | BPF_K:
	case BPF_JMP | BPF_JSLT | BPF_K:
	case BPF_JMP | BPF_JSLE | BPF_K:
	case BPF_JMP32 | BPF_JEQ | BPF_K:
	case BPF_JMP32 | BPF_JGT | BPF_K:
	case BPF_JMP32 | BPF_JGE | BPF_K:
	case BPF_JMP32 | BPF_JNE | BPF_K:
	case BPF_JMP32 | BPF_JSGT | BPF_K:
	case BPF_JMP32 | BPF_JSGE | BPF_K:
	case BPF_JMP32 | BPF_JSET | BPF_K:
	case BPF_JMP32 | BPF_JLT | BPF_K:
	case BPF_JMP32 | BPF_JLE | BPF_K:
	case BPF_JMP32 | BPF_JSLT | BPF_K:
	case BPF_JMP32 | BPF_JSLE | BPF_K:
		if (off == 0)
			break;
		rm = tmp2[0];
		rn = tmp2[1];
		/* Sign-extend immediate value */
		emit_a32_mov_se_i64(true, tmp2, imm, ctx);
go_jmp:
		/* Setup destination register */
		rd = arm_bpf_get_reg64(dst, tmp, ctx);

		/* Check for the condition */
		emit_ar_r(rd[0], rd[1], rm, rn, ctx, BPF_OP(code),
			  BPF_CLASS(code) == BPF_JMP);

		/* Setup JUMP instruction */
		jmp_offset = bpf2a32_offset(i+off, i, ctx);
		switch (BPF_OP(code)) {
		case BPF_JNE:
		case BPF_JSET:
			_emit(ARM_COND_NE, ARM_B(jmp_offset), ctx);
			break;
		case BPF_JEQ:
			_emit(ARM_COND_EQ, ARM_B(jmp_offset), ctx);
			break;
		case BPF_JGT:
			_emit(ARM_COND_HI, ARM_B(jmp_offset), ctx);
			break;
		case BPF_JGE:
			_emit(ARM_COND_CS, ARM_B(jmp_offset), ctx);
			break;
		case BPF_JSGT:
			_emit(ARM_COND_LT, ARM_B(jmp_offset), ctx);
			break;
		case BPF_JSGE:
			_emit(ARM_COND_GE, ARM_B(jmp_offset), ctx);
			break;
		case BPF_JLE:
			_emit(ARM_COND_LS, ARM_B(jmp_offset), ctx);
			break;
		case BPF_JLT:
			_emit(ARM_COND_CC, ARM_B(jmp_offset), ctx);
			break;
		case BPF_JSLT:
			_emit(ARM_COND_LT, ARM_B(jmp_offset), ctx);
			break;
		case BPF_JSLE:
			_emit(ARM_COND_GE, ARM_B(jmp_offset), ctx);
			break;
		}
		break;
	/* JMP OFF */
	case BPF_JMP | BPF_JA:
	case BPF_JMP32 | BPF_JA:
	{
		if (BPF_CLASS(code) == BPF_JMP32 && imm != 0)
			jmp_offset = bpf2a32_offset(i + imm, i, ctx);
		else if (BPF_CLASS(code) == BPF_JMP && off != 0)
			jmp_offset = bpf2a32_offset(i + off, i, ctx);
		else
			break;

		check_imm24(jmp_offset);
		emit(ARM_B(jmp_offset), ctx);
		break;
	}
	/* tail call */
	case BPF_JMP | BPF_TAIL_CALL:
		if (emit_bpf_tail_call(ctx))
			return -EFAULT;
		break;
	/* function call */
	case BPF_JMP | BPF_CALL:
	{
		const s8 *r0 = bpf2a32[BPF_REG_0];
		const s8 *r1 = bpf2a32[BPF_REG_1];
		const s8 *r2 = bpf2a32[BPF_REG_2];
		const s8 *r3 = bpf2a32[BPF_REG_3];
		const s8 *r4 = bpf2a32[BPF_REG_4];
		const s8 *r5 = bpf2a32[BPF_REG_5];
		const u32 func = (u32)__bpf_call_base + (u32)imm;

		emit_a32_mov_r64(true, r0, r1, ctx);
		emit_a32_mov_r64(true, r1, r2, ctx);
		emit_push_r64(r5, ctx);
		emit_push_r64(r4, ctx);
		emit_push_r64(r3, ctx);

		emit_a32_mov_i(tmp[1], func, ctx);
		emit_blx_r(tmp[1], ctx);

		emit(ARM_ADD_I(ARM_SP, ARM_SP, imm8m(24)), ctx); // callee clean
		break;
	}
	/* function return */
	case BPF_JMP | BPF_EXIT:
		/* Optimization: when last instruction is EXIT
		 * simply fallthrough to epilogue.
		 */
		if (i == ctx->prog->len - 1)
			break;
		jmp_offset = epilogue_offset(ctx);
		check_imm24(jmp_offset);
		emit(ARM_B(jmp_offset), ctx);
		break;
notyet:
		pr_info_once("*** NOT YET: opcode %02x ***\n", code);
		return -EFAULT;
	default:
		pr_err_once("unknown opcode %02x\n", code);
		return -EINVAL;
	}

	if (ctx->flags & FLAG_IMM_OVERFLOW)
		/*
		 * this instruction generated an overflow when
		 * trying to access the literal pool, so
		 * delegate this filter to the kernel interpreter.
		 */
		return -1;
	return 0;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

static int build_body(struct jit_ctx *ctx)
{
<<<<<<< HEAD
	void *load_func[] = {jit_get_skb_b, jit_get_skb_h, jit_get_skb_w};
	const struct sk_filter *prog = ctx->skf;
	const struct sock_filter *inst;
	unsigned i, load_order, off, condt;
	int imm12;
	u32 k;

	for (i = 0; i < prog->len; i++) {
		inst = &(prog->insns[i]);
		/* K as an immediate value operand */
		k = inst->k;

		/* compute offsets only in the fake pass */
		if (ctx->target == NULL)
			ctx->offsets[i] = ctx->idx * 4;

		switch (inst->code) {
		case BPF_S_LD_IMM:
			emit_mov_i(r_A, k, ctx);
			break;
		case BPF_S_LD_W_LEN:
			ctx->seen |= SEEN_SKB;
			BUILD_BUG_ON(FIELD_SIZEOF(struct sk_buff, len) != 4);
			emit(ARM_LDR_I(r_A, r_skb,
				       offsetof(struct sk_buff, len)), ctx);
			break;
		case BPF_S_LD_MEM:
			/* A = scratch[k] */
			ctx->seen |= SEEN_MEM_WORD(k);
			emit(ARM_LDR_I(r_A, ARM_SP, SCRATCH_OFF(k)), ctx);
			break;
		case BPF_S_LD_W_ABS:
			load_order = 2;
			goto load;
		case BPF_S_LD_H_ABS:
			load_order = 1;
			goto load;
		case BPF_S_LD_B_ABS:
			load_order = 0;
load:
			/* the interpreter will deal with the negative K */
			if ((int)k < 0)
				return -ENOTSUPP;
			emit_mov_i(r_off, k, ctx);
load_common:
			ctx->seen |= SEEN_DATA | SEEN_CALL;

			if (load_order > 0) {
				emit(ARM_SUB_I(r_scratch, r_skb_hl,
					       1 << load_order), ctx);
				emit(ARM_CMP_R(r_scratch, r_off), ctx);
				condt = ARM_COND_HS;
			} else {
				emit(ARM_CMP_R(r_skb_hl, r_off), ctx);
				condt = ARM_COND_HI;
			}

			_emit(condt, ARM_ADD_R(r_scratch, r_off, r_skb_data),
			      ctx);

			if (load_order == 0)
				_emit(condt, ARM_LDRB_I(r_A, r_scratch, 0),
				      ctx);
			else if (load_order == 1)
				emit_load_be16(condt, r_A, r_scratch, ctx);
			else if (load_order == 2)
				emit_load_be32(condt, r_A, r_scratch, ctx);

			_emit(condt, ARM_B(b_imm(i + 1, ctx)), ctx);

			/* the slowpath */
			emit_mov_i(ARM_R3, (u32)load_func[load_order], ctx);
			emit(ARM_MOV_R(ARM_R0, r_skb), ctx);
			/* the offset is already in R1 */
			emit_blx_r(ARM_R3, ctx);
			/* check the result of skb_copy_bits */
			emit(ARM_CMP_I(ARM_R1, 0), ctx);
			emit_err_ret(ARM_COND_NE, ctx);
			emit(ARM_MOV_R(r_A, ARM_R0), ctx);
			break;
		case BPF_S_LD_W_IND:
			load_order = 2;
			goto load_ind;
		case BPF_S_LD_H_IND:
			load_order = 1;
			goto load_ind;
		case BPF_S_LD_B_IND:
			load_order = 0;
load_ind:
			OP_IMM3(ARM_ADD, r_off, r_X, k, ctx);
			goto load_common;
		case BPF_S_LDX_IMM:
			ctx->seen |= SEEN_X;
			emit_mov_i(r_X, k, ctx);
			break;
		case BPF_S_LDX_W_LEN:
			ctx->seen |= SEEN_X | SEEN_SKB;
			emit(ARM_LDR_I(r_X, r_skb,
				       offsetof(struct sk_buff, len)), ctx);
			break;
		case BPF_S_LDX_MEM:
			ctx->seen |= SEEN_X | SEEN_MEM_WORD(k);
			emit(ARM_LDR_I(r_X, ARM_SP, SCRATCH_OFF(k)), ctx);
			break;
		case BPF_S_LDX_B_MSH:
			/* x = ((*(frame + k)) & 0xf) << 2; */
			ctx->seen |= SEEN_X | SEEN_DATA | SEEN_CALL;
			/* the interpreter should deal with the negative K */
			if (k < 0)
				return -1;
			/* offset in r1: we might have to take the slow path */
			emit_mov_i(r_off, k, ctx);
			emit(ARM_CMP_R(r_skb_hl, r_off), ctx);

			/* load in r0: common with the slowpath */
			_emit(ARM_COND_HI, ARM_LDRB_R(ARM_R0, r_skb_data,
						      ARM_R1), ctx);
			/*
			 * emit_mov_i() might generate one or two instructions,
			 * the same holds for emit_blx_r()
			 */
			_emit(ARM_COND_HI, ARM_B(b_imm(i + 1, ctx) - 2), ctx);

			emit(ARM_MOV_R(ARM_R0, r_skb), ctx);
			/* r_off is r1 */
			emit_mov_i(ARM_R3, (u32)jit_get_skb_b, ctx);
			emit_blx_r(ARM_R3, ctx);
			/* check the return value of skb_copy_bits */
			emit(ARM_CMP_I(ARM_R1, 0), ctx);
			emit_err_ret(ARM_COND_NE, ctx);

			emit(ARM_AND_I(r_X, ARM_R0, 0x00f), ctx);
			emit(ARM_LSL_I(r_X, r_X, 2), ctx);
			break;
		case BPF_S_ST:
			ctx->seen |= SEEN_MEM_WORD(k);
			emit(ARM_STR_I(r_A, ARM_SP, SCRATCH_OFF(k)), ctx);
			break;
		case BPF_S_STX:
			update_on_xread(ctx);
			ctx->seen |= SEEN_MEM_WORD(k);
			emit(ARM_STR_I(r_X, ARM_SP, SCRATCH_OFF(k)), ctx);
			break;
		case BPF_S_ALU_ADD_K:
			/* A += K */
			OP_IMM3(ARM_ADD, r_A, r_A, k, ctx);
			break;
		case BPF_S_ALU_ADD_X:
			update_on_xread(ctx);
			emit(ARM_ADD_R(r_A, r_A, r_X), ctx);
			break;
		case BPF_S_ALU_SUB_K:
			/* A -= K */
			OP_IMM3(ARM_SUB, r_A, r_A, k, ctx);
			break;
		case BPF_S_ALU_SUB_X:
			update_on_xread(ctx);
			emit(ARM_SUB_R(r_A, r_A, r_X), ctx);
			break;
		case BPF_S_ALU_MUL_K:
			/* A *= K */
			emit_mov_i(r_scratch, k, ctx);
			emit(ARM_MUL(r_A, r_A, r_scratch), ctx);
			break;
		case BPF_S_ALU_MUL_X:
			update_on_xread(ctx);
			emit(ARM_MUL(r_A, r_A, r_X), ctx);
			break;
		case BPF_S_ALU_DIV_K:
			/* current k == reciprocal_value(userspace k) */
			emit_mov_i(r_scratch, k, ctx);
			/* A = top 32 bits of the product */
			emit(ARM_UMULL(r_scratch, r_A, r_A, r_scratch), ctx);
			break;
		case BPF_S_ALU_DIV_X:
			update_on_xread(ctx);
			emit(ARM_CMP_I(r_X, 0), ctx);
			emit_err_ret(ARM_COND_EQ, ctx);
			emit_udiv(r_A, r_A, r_X, ctx);
			break;
		case BPF_S_ALU_OR_K:
			/* A |= K */
			OP_IMM3(ARM_ORR, r_A, r_A, k, ctx);
			break;
		case BPF_S_ALU_OR_X:
			update_on_xread(ctx);
			emit(ARM_ORR_R(r_A, r_A, r_X), ctx);
			break;
		case BPF_S_ALU_AND_K:
			/* A &= K */
			OP_IMM3(ARM_AND, r_A, r_A, k, ctx);
			break;
		case BPF_S_ALU_AND_X:
			update_on_xread(ctx);
			emit(ARM_AND_R(r_A, r_A, r_X), ctx);
			break;
		case BPF_S_ALU_LSH_K:
			if (unlikely(k > 31))
				return -1;
			emit(ARM_LSL_I(r_A, r_A, k), ctx);
			break;
		case BPF_S_ALU_LSH_X:
			update_on_xread(ctx);
			emit(ARM_LSL_R(r_A, r_A, r_X), ctx);
			break;
		case BPF_S_ALU_RSH_K:
			if (unlikely(k > 31))
				return -1;
			emit(ARM_LSR_I(r_A, r_A, k), ctx);
			break;
		case BPF_S_ALU_RSH_X:
			update_on_xread(ctx);
			emit(ARM_LSR_R(r_A, r_A, r_X), ctx);
			break;
		case BPF_S_ALU_NEG:
			/* A = -A */
			emit(ARM_RSB_I(r_A, r_A, 0), ctx);
			break;
		case BPF_S_JMP_JA:
			/* pc += K */
			emit(ARM_B(b_imm(i + k + 1, ctx)), ctx);
			break;
		case BPF_S_JMP_JEQ_K:
			/* pc += (A == K) ? pc->jt : pc->jf */
			condt  = ARM_COND_EQ;
			goto cmp_imm;
		case BPF_S_JMP_JGT_K:
			/* pc += (A > K) ? pc->jt : pc->jf */
			condt  = ARM_COND_HI;
			goto cmp_imm;
		case BPF_S_JMP_JGE_K:
			/* pc += (A >= K) ? pc->jt : pc->jf */
			condt  = ARM_COND_HS;
cmp_imm:
			imm12 = imm8m(k);
			if (imm12 < 0) {
				emit_mov_i_no8m(r_scratch, k, ctx);
				emit(ARM_CMP_R(r_A, r_scratch), ctx);
			} else {
				emit(ARM_CMP_I(r_A, imm12), ctx);
			}
cond_jump:
			if (inst->jt)
				_emit(condt, ARM_B(b_imm(i + inst->jt + 1,
						   ctx)), ctx);
			if (inst->jf)
				_emit(condt ^ 1, ARM_B(b_imm(i + inst->jf + 1,
							     ctx)), ctx);
			break;
		case BPF_S_JMP_JEQ_X:
			/* pc += (A == X) ? pc->jt : pc->jf */
			condt   = ARM_COND_EQ;
			goto cmp_x;
		case BPF_S_JMP_JGT_X:
			/* pc += (A > X) ? pc->jt : pc->jf */
			condt   = ARM_COND_HI;
			goto cmp_x;
		case BPF_S_JMP_JGE_X:
			/* pc += (A >= X) ? pc->jt : pc->jf */
			condt   = ARM_COND_CS;
cmp_x:
			update_on_xread(ctx);
			emit(ARM_CMP_R(r_A, r_X), ctx);
			goto cond_jump;
		case BPF_S_JMP_JSET_K:
			/* pc += (A & K) ? pc->jt : pc->jf */
			condt  = ARM_COND_NE;
			/* not set iff all zeroes iff Z==1 iff EQ */

			imm12 = imm8m(k);
			if (imm12 < 0) {
				emit_mov_i_no8m(r_scratch, k, ctx);
				emit(ARM_TST_R(r_A, r_scratch), ctx);
			} else {
				emit(ARM_TST_I(r_A, imm12), ctx);
			}
			goto cond_jump;
		case BPF_S_JMP_JSET_X:
			/* pc += (A & X) ? pc->jt : pc->jf */
			update_on_xread(ctx);
			condt  = ARM_COND_NE;
			emit(ARM_TST_R(r_A, r_X), ctx);
			goto cond_jump;
		case BPF_S_RET_A:
			emit(ARM_MOV_R(ARM_R0, r_A), ctx);
			goto b_epilogue;
		case BPF_S_RET_K:
			if ((k == 0) && (ctx->ret0_fp_idx < 0))
				ctx->ret0_fp_idx = i;
			emit_mov_i(ARM_R0, k, ctx);
b_epilogue:
			if (i != ctx->skf->len - 1)
				emit(ARM_B(b_imm(prog->len, ctx)), ctx);
			break;
		case BPF_S_MISC_TAX:
			/* X = A */
			ctx->seen |= SEEN_X;
			emit(ARM_MOV_R(r_X, r_A), ctx);
			break;
		case BPF_S_MISC_TXA:
			/* A = X */
			update_on_xread(ctx);
			emit(ARM_MOV_R(r_A, r_X), ctx);
			break;
		case BPF_S_ANC_PROTOCOL:
			/* A = ntohs(skb->protocol) */
			ctx->seen |= SEEN_SKB;
			BUILD_BUG_ON(FIELD_SIZEOF(struct sk_buff,
						  protocol) != 2);
			off = offsetof(struct sk_buff, protocol);
			emit(ARM_LDRH_I(r_scratch, r_skb, off), ctx);
			emit_swap16(r_A, r_scratch, ctx);
			break;
		case BPF_S_ANC_CPU:
			/* r_scratch = current_thread_info() */
			OP_IMM3(ARM_BIC, r_scratch, ARM_SP, THREAD_SIZE - 1, ctx);
			/* A = current_thread_info()->cpu */
			BUILD_BUG_ON(FIELD_SIZEOF(struct thread_info, cpu) != 4);
			off = offsetof(struct thread_info, cpu);
			emit(ARM_LDR_I(r_A, r_scratch, off), ctx);
			break;
		case BPF_S_ANC_IFINDEX:
			/* A = skb->dev->ifindex */
			ctx->seen |= SEEN_SKB;
			off = offsetof(struct sk_buff, dev);
			emit(ARM_LDR_I(r_scratch, r_skb, off), ctx);

			emit(ARM_CMP_I(r_scratch, 0), ctx);
			emit_err_ret(ARM_COND_EQ, ctx);

			BUILD_BUG_ON(FIELD_SIZEOF(struct net_device,
						  ifindex) != 4);
			off = offsetof(struct net_device, ifindex);
			emit(ARM_LDR_I(r_A, r_scratch, off), ctx);
			break;
		case BPF_S_ANC_MARK:
			ctx->seen |= SEEN_SKB;
			BUILD_BUG_ON(FIELD_SIZEOF(struct sk_buff, mark) != 4);
			off = offsetof(struct sk_buff, mark);
			emit(ARM_LDR_I(r_A, r_skb, off), ctx);
			break;
		case BPF_S_ANC_RXHASH:
			ctx->seen |= SEEN_SKB;
			BUILD_BUG_ON(FIELD_SIZEOF(struct sk_buff, rxhash) != 4);
			off = offsetof(struct sk_buff, rxhash);
			emit(ARM_LDR_I(r_A, r_skb, off), ctx);
			break;
		case BPF_S_ANC_QUEUE:
			ctx->seen |= SEEN_SKB;
			BUILD_BUG_ON(FIELD_SIZEOF(struct sk_buff,
						  queue_mapping) != 2);
			BUILD_BUG_ON(offsetof(struct sk_buff,
					      queue_mapping) > 0xff);
			off = offsetof(struct sk_buff, queue_mapping);
			emit(ARM_LDRH_I(r_A, r_skb, off), ctx);
			break;
		default:
			return -1;
		}

		if (ctx->flags & FLAG_IMM_OVERFLOW)
			/*
			 * this instruction generated an overflow when
			 * trying to access the literal pool, so
			 * delegate this filter to the kernel interpreter.
			 */
			return -1;
	}

	/* compute offsets only during the first pass */
	if (ctx->target == NULL)
		ctx->offsets[i] = ctx->idx * 4;

	return 0;
}


void bpf_jit_compile(struct sk_filter *fp)
{
	struct jit_ctx ctx;
	unsigned tmp_idx;
	unsigned alloc_size;

	if (!bpf_jit_enable)
		return;

	memset(&ctx, 0, sizeof(ctx));
	ctx.skf		= fp;
	ctx.ret0_fp_idx = -1;

	ctx.offsets = kzalloc(GFP_KERNEL, 4 * (ctx.skf->len + 1));
	if (ctx.offsets == NULL)
		return;

	/* fake pass to fill in the ctx->seen */
	if (unlikely(build_body(&ctx)))
		goto out;
=======
	const struct bpf_prog *prog = ctx->prog;
	unsigned int i;

	for (i = 0; i < prog->len; i++) {
		const struct bpf_insn *insn = &(prog->insnsi[i]);
		int ret;

		ret = build_insn(insn, ctx);

		/* It's used with loading the 64 bit immediate value. */
		if (ret > 0) {
			i++;
			if (ctx->target == NULL)
				ctx->offsets[i] = ctx->idx;
			continue;
		}

		if (ctx->target == NULL)
			ctx->offsets[i] = ctx->idx;

		/* If unsuccesful, return with error code */
		if (ret)
			return ret;
	}
	return 0;
}

static int validate_code(struct jit_ctx *ctx)
{
	int i;

	for (i = 0; i < ctx->idx; i++) {
		if (ctx->target[i] == __opcode_to_mem_arm(ARM_INST_UDF))
			return -1;
	}

	return 0;
}

bool bpf_jit_needs_zext(void)
{
	return true;
}

struct bpf_prog *bpf_int_jit_compile(struct bpf_prog *prog)
{
	struct bpf_prog *tmp, *orig_prog = prog;
	struct bpf_binary_header *header;
	bool tmp_blinded = false;
	struct jit_ctx ctx;
	unsigned int tmp_idx;
	unsigned int image_size;
	u8 *image_ptr;

	/* If BPF JIT was not enabled then we must fall back to
	 * the interpreter.
	 */
	if (!prog->jit_requested)
		return orig_prog;

	/* If constant blinding was enabled and we failed during blinding
	 * then we must fall back to the interpreter. Otherwise, we save
	 * the new JITed code.
	 */
	tmp = bpf_jit_blind_constants(prog);

	if (IS_ERR(tmp))
		return orig_prog;
	if (tmp != prog) {
		tmp_blinded = true;
		prog = tmp;
	}

	memset(&ctx, 0, sizeof(ctx));
	ctx.prog = prog;
	ctx.cpu_architecture = cpu_architecture();

	/* Not able to allocate memory for offsets[] , then
	 * we must fall back to the interpreter
	 */
	ctx.offsets = kcalloc(prog->len, sizeof(int), GFP_KERNEL);
	if (ctx.offsets == NULL) {
		prog = orig_prog;
		goto out;
	}

	/* 1) fake pass to find in the length of the JITed code,
	 * to compute ctx->offsets and other context variables
	 * needed to compute final JITed code.
	 * Also, calculate random starting pointer/start of JITed code
	 * which is prefixed by random number of fault instructions.
	 *
	 * If the first pass fails then there is no chance of it
	 * being successful in the second pass, so just fall back
	 * to the interpreter.
	 */
	if (build_body(&ctx)) {
		prog = orig_prog;
		goto out_off;
	}
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	tmp_idx = ctx.idx;
	build_prologue(&ctx);
	ctx.prologue_bytes = (ctx.idx - tmp_idx) * 4;

<<<<<<< HEAD
=======
	ctx.epilogue_offset = ctx.idx;

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#if __LINUX_ARM_ARCH__ < 7
	tmp_idx = ctx.idx;
	build_epilogue(&ctx);
	ctx.epilogue_bytes = (ctx.idx - tmp_idx) * 4;

	ctx.idx += ctx.imm_count;
	if (ctx.imm_count) {
<<<<<<< HEAD
		ctx.imms = kzalloc(GFP_KERNEL, 4 * ctx.imm_count);
		if (ctx.imms == NULL)
			goto out;
	}
#else
	/* there's nothing after the epilogue on ARMv7 */
	build_epilogue(&ctx);
#endif

	alloc_size = 4 * ctx.idx;
	ctx.target = module_alloc(max(sizeof(struct work_struct),
				      alloc_size));
	if (unlikely(ctx.target == NULL))
		goto out;

	ctx.idx = 0;
	build_prologue(&ctx);
	if (build_body(&ctx) < 0) {
#if __LINUX_ARM_ARCH__ < 7
		if (ctx.imm_count)
			kfree(ctx.imms);
#endif
		module_free(NULL, ctx.target);
		goto out;
	}
	build_epilogue(&ctx);

	flush_icache_range((u32)ctx.target, (u32)(ctx.target + ctx.idx));

=======
		ctx.imms = kcalloc(ctx.imm_count, sizeof(u32), GFP_KERNEL);
		if (ctx.imms == NULL) {
			prog = orig_prog;
			goto out_off;
		}
	}
#else
	/* there's nothing about the epilogue on ARMv7 */
	build_epilogue(&ctx);
#endif
	/* Now we can get the actual image size of the JITed arm code.
	 * Currently, we are not considering the THUMB-2 instructions
	 * for jit, although it can decrease the size of the image.
	 *
	 * As each arm instruction is of length 32bit, we are translating
	 * number of JITed instructions into the size required to store these
	 * JITed code.
	 */
	image_size = sizeof(u32) * ctx.idx;

	/* Now we know the size of the structure to make */
	header = bpf_jit_binary_alloc(image_size, &image_ptr,
				      sizeof(u32), jit_fill_hole);
	/* Not able to allocate memory for the structure then
	 * we must fall back to the interpretation
	 */
	if (header == NULL) {
		prog = orig_prog;
		goto out_imms;
	}

	/* 2.) Actual pass to generate final JIT code */
	ctx.target = (u32 *) image_ptr;
	ctx.idx = 0;

	build_prologue(&ctx);

	/* If building the body of the JITed code fails somehow,
	 * we fall back to the interpretation.
	 */
	if (build_body(&ctx) < 0) {
		image_ptr = NULL;
		bpf_jit_binary_free(header);
		prog = orig_prog;
		goto out_imms;
	}
	build_epilogue(&ctx);

	/* 3.) Extra pass to validate JITed Code */
	if (validate_code(&ctx)) {
		image_ptr = NULL;
		bpf_jit_binary_free(header);
		prog = orig_prog;
		goto out_imms;
	}
	flush_icache_range((u32)header, (u32)(ctx.target + ctx.idx));

	if (bpf_jit_enable > 1)
		/* there are 2 passes here */
		bpf_jit_dump(prog->len, image_size, 2, ctx.target);

	bpf_jit_binary_lock_ro(header);
	prog->bpf_func = (void *)ctx.target;
	prog->jited = 1;
	prog->jited_len = image_size;

out_imms:
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#if __LINUX_ARM_ARCH__ < 7
	if (ctx.imm_count)
		kfree(ctx.imms);
#endif
<<<<<<< HEAD

	if (bpf_jit_enable > 1)
		print_hex_dump(KERN_INFO, "BPF JIT code: ",
			       DUMP_PREFIX_ADDRESS, 16, 4, ctx.target,
			       alloc_size, false);

	fp->bpf_func = (void *)ctx.target;
out:
	kfree(ctx.offsets);
	return;
}

static void bpf_jit_free_worker(struct work_struct *work)
{
	module_free(NULL, work);
}

void bpf_jit_free(struct sk_filter *fp)
{
	struct work_struct *work;

	if (fp->bpf_func != sk_run_filter) {
		work = (struct work_struct *)fp->bpf_func;

		INIT_WORK(work, bpf_jit_free_worker);
		schedule_work(work);
	}
}
=======
out_off:
	kfree(ctx.offsets);
out:
	if (tmp_blinded)
		bpf_jit_prog_release_other(prog, prog == orig_prog ?
					   tmp : orig_prog);
	return prog;
}

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
