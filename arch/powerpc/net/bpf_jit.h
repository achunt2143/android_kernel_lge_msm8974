<<<<<<< HEAD
/* bpf_jit.h: BPF JIT compiler for PPC64
 *
 * Copyright 2011 Matt Evans <matt@ozlabs.org>, IBM Corporation
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; version 2
 * of the License.
=======
/* SPDX-License-Identifier: GPL-2.0-only */
/*
 * bpf_jit.h: BPF JIT compiler for PPC
 *
 * Copyright 2011 Matt Evans <matt@ozlabs.org>, IBM Corporation
 * 	     2016 Naveen N. Rao <naveen.n.rao@linux.vnet.ibm.com>
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 */
#ifndef _BPF_JIT_H
#define _BPF_JIT_H

<<<<<<< HEAD
#define BPF_PPC_STACK_LOCALS	32
#define BPF_PPC_STACK_BASIC	(48+64)
#define BPF_PPC_STACK_SAVE	(18*8)
#define BPF_PPC_STACKFRAME	(BPF_PPC_STACK_BASIC+BPF_PPC_STACK_LOCALS+ \
				 BPF_PPC_STACK_SAVE)
#define BPF_PPC_SLOWPATH_FRAME	(48+64)

/*
 * Generated code register usage:
 *
 * As normal PPC C ABI (e.g. r1=sp, r2=TOC), with:
 *
 * skb		r3	(Entry parameter)
 * A register	r4
 * X register	r5
 * addr param	r6
 * r7-r10	scratch
 * skb->data	r14
 * skb headlen	r15	(skb->len - skb->data_len)
 * m[0]		r16
 * m[...]	...
 * m[15]	r31
 */
#define r_skb		3
#define r_ret		3
#define r_A		4
#define r_X		5
#define r_addr		6
#define r_scratch1	7
#define r_D		14
#define r_HL		15
#define r_M		16

#ifndef __ASSEMBLY__

/*
 * Assembly helpers from arch/powerpc/net/bpf_jit.S:
 */
#define DECLARE_LOAD_FUNC(func)	\
	extern u8 func[], func##_negative_offset[], func##_positive_offset[]

DECLARE_LOAD_FUNC(sk_load_word);
DECLARE_LOAD_FUNC(sk_load_half);
DECLARE_LOAD_FUNC(sk_load_byte);
DECLARE_LOAD_FUNC(sk_load_byte_msh);

#define FUNCTION_DESCR_SIZE	24

/*
 * 16-bit immediate helper macros: HA() is for use with sign-extending instrs
 * (e.g. LD, ADDI).  If the bottom 16 bits is "-ve", add another bit into the
 * top half to negate the effect (i.e. 0xffff + 1 = 0x(1)0000).
 */
#define IMM_H(i)		((uintptr_t)(i)>>16)
#define IMM_HA(i)		(((uintptr_t)(i)>>16) +			      \
				 (((uintptr_t)(i) & 0x8000) >> 15))
#define IMM_L(i)		((uintptr_t)(i) & 0xffff)
=======
#ifndef __ASSEMBLY__

#include <asm/types.h>
#include <asm/ppc-opcode.h>

#ifdef CONFIG_PPC64_ELF_ABI_V1
#define FUNCTION_DESCR_SIZE	24
#else
#define FUNCTION_DESCR_SIZE	0
#endif

#define CTX_NIA(ctx) ((unsigned long)ctx->idx * 4)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

#define PLANT_INSTR(d, idx, instr)					      \
	do { if (d) { (d)[idx] = instr; } idx++; } while (0)
#define EMIT(instr)		PLANT_INSTR(image, ctx->idx, instr)

<<<<<<< HEAD
#define PPC_NOP()		EMIT(PPC_INST_NOP)
#define PPC_BLR()		EMIT(PPC_INST_BLR)
#define PPC_BLRL()		EMIT(PPC_INST_BLRL)
#define PPC_MTLR(r)		EMIT(PPC_INST_MTLR | __PPC_RT(r))
#define PPC_ADDI(d, a, i)	EMIT(PPC_INST_ADDI | __PPC_RT(d) |	      \
				     __PPC_RA(a) | IMM_L(i))
#define PPC_MR(d, a)		PPC_OR(d, a, a)
#define PPC_LI(r, i)		PPC_ADDI(r, 0, i)
#define PPC_ADDIS(d, a, i)	EMIT(PPC_INST_ADDIS |			      \
				     __PPC_RS(d) | __PPC_RA(a) | IMM_L(i))
#define PPC_LIS(r, i)		PPC_ADDIS(r, 0, i)
#define PPC_STD(r, base, i)	EMIT(PPC_INST_STD | __PPC_RS(r) |	      \
				     __PPC_RA(base) | ((i) & 0xfffc))

#define PPC_LD(r, base, i)	EMIT(PPC_INST_LD | __PPC_RT(r) |	      \
				     __PPC_RA(base) | IMM_L(i))
#define PPC_LWZ(r, base, i)	EMIT(PPC_INST_LWZ | __PPC_RT(r) |	      \
				     __PPC_RA(base) | IMM_L(i))
#define PPC_LHZ(r, base, i)	EMIT(PPC_INST_LHZ | __PPC_RT(r) |	      \
				     __PPC_RA(base) | IMM_L(i))
/* Convenience helpers for the above with 'far' offsets: */
#define PPC_LD_OFFS(r, base, i) do { if ((i) < 32768) PPC_LD(r, base, i);     \
		else {	PPC_ADDIS(r, base, IMM_HA(i));			      \
			PPC_LD(r, r, IMM_L(i)); } } while(0)

#define PPC_LWZ_OFFS(r, base, i) do { if ((i) < 32768) PPC_LWZ(r, base, i);   \
		else {	PPC_ADDIS(r, base, IMM_HA(i));			      \
			PPC_LWZ(r, r, IMM_L(i)); } } while(0)

#define PPC_LHZ_OFFS(r, base, i) do { if ((i) < 32768) PPC_LHZ(r, base, i);   \
		else {	PPC_ADDIS(r, base, IMM_HA(i));			      \
			PPC_LHZ(r, r, IMM_L(i)); } } while(0)

#define PPC_CMPWI(a, i)		EMIT(PPC_INST_CMPWI | __PPC_RA(a) | IMM_L(i))
#define PPC_CMPDI(a, i)		EMIT(PPC_INST_CMPDI | __PPC_RA(a) | IMM_L(i))
#define PPC_CMPLWI(a, i)	EMIT(PPC_INST_CMPLWI | __PPC_RA(a) | IMM_L(i))
#define PPC_CMPLW(a, b)		EMIT(PPC_INST_CMPLW | __PPC_RA(a) | __PPC_RB(b))

#define PPC_SUB(d, a, b)	EMIT(PPC_INST_SUB | __PPC_RT(d) |	      \
				     __PPC_RB(a) | __PPC_RA(b))
#define PPC_ADD(d, a, b)	EMIT(PPC_INST_ADD | __PPC_RT(d) |	      \
				     __PPC_RA(a) | __PPC_RB(b))
#define PPC_MUL(d, a, b)	EMIT(PPC_INST_MULLW | __PPC_RT(d) |	      \
				     __PPC_RA(a) | __PPC_RB(b))
#define PPC_MULHWU(d, a, b)	EMIT(PPC_INST_MULHWU | __PPC_RT(d) |	      \
				     __PPC_RA(a) | __PPC_RB(b))
#define PPC_MULI(d, a, i)	EMIT(PPC_INST_MULLI | __PPC_RT(d) |	      \
				     __PPC_RA(a) | IMM_L(i))
#define PPC_DIVWU(d, a, b)	EMIT(PPC_INST_DIVWU | __PPC_RT(d) |	      \
				     __PPC_RA(a) | __PPC_RB(b))
#define PPC_AND(d, a, b)	EMIT(PPC_INST_AND | __PPC_RA(d) |	      \
				     __PPC_RS(a) | __PPC_RB(b))
#define PPC_ANDI(d, a, i)	EMIT(PPC_INST_ANDI | __PPC_RA(d) |	      \
				     __PPC_RS(a) | IMM_L(i))
#define PPC_AND_DOT(d, a, b)	EMIT(PPC_INST_ANDDOT | __PPC_RA(d) |	      \
				     __PPC_RS(a) | __PPC_RB(b))
#define PPC_OR(d, a, b)		EMIT(PPC_INST_OR | __PPC_RA(d) |	      \
				     __PPC_RS(a) | __PPC_RB(b))
#define PPC_ORI(d, a, i)	EMIT(PPC_INST_ORI | __PPC_RA(d) |	      \
				     __PPC_RS(a) | IMM_L(i))
#define PPC_ORIS(d, a, i)	EMIT(PPC_INST_ORIS | __PPC_RA(d) |	      \
				     __PPC_RS(a) | IMM_L(i))
#define PPC_SLW(d, a, s)	EMIT(PPC_INST_SLW | __PPC_RA(d) |	      \
				     __PPC_RS(a) | __PPC_RB(s))
#define PPC_SRW(d, a, s)	EMIT(PPC_INST_SRW | __PPC_RA(d) |	      \
				     __PPC_RS(a) | __PPC_RB(s))
/* slwi = rlwinm Rx, Ry, n, 0, 31-n */
#define PPC_SLWI(d, a, i)	EMIT(PPC_INST_RLWINM | __PPC_RA(d) |	      \
				     __PPC_RS(a) | __PPC_SH(i) |	      \
				     __PPC_MB(0) | __PPC_ME(31-(i)))
/* srwi = rlwinm Rx, Ry, 32-n, n, 31 */
#define PPC_SRWI(d, a, i)	EMIT(PPC_INST_RLWINM | __PPC_RA(d) |	      \
				     __PPC_RS(a) | __PPC_SH(32-(i)) |	      \
				     __PPC_MB(i) | __PPC_ME(31))
/* sldi = rldicr Rx, Ry, n, 63-n */
#define PPC_SLDI(d, a, i)	EMIT(PPC_INST_RLDICR | __PPC_RA(d) |	      \
				     __PPC_RS(a) | __PPC_SH(i) |	      \
				     __PPC_MB(63-(i)) | (((i) & 0x20) >> 4))
#define PPC_NEG(d, a)		EMIT(PPC_INST_NEG | __PPC_RT(d) | __PPC_RA(a))

/* Long jump; (unconditional 'branch') */
#define PPC_JMP(dest)		EMIT(PPC_INST_BRANCH |			      \
				     (((dest) - (ctx->idx * 4)) & 0x03fffffc))
/* "cond" here covers BO:BI fields. */
#define PPC_BCC_SHORT(cond, dest)	EMIT(PPC_INST_BRANCH_COND |	      \
					     (((cond) & 0x3ff) << 16) |	      \
					     (((dest) - (ctx->idx * 4)) &     \
					      0xfffc))
#define PPC_LI32(d, i)		do { PPC_LI(d, IMM_L(i));		      \
		if ((u32)(uintptr_t)(i) >= 32768) {			      \
			PPC_ADDIS(d, d, IMM_HA(i));			      \
		} } while(0)
#define PPC_LI64(d, i)		do {					      \
		if (!((uintptr_t)(i) & 0xffffffff00000000ULL))		      \
			PPC_LI32(d, i);					      \
		else {							      \
			PPC_LIS(d, ((uintptr_t)(i) >> 48));		      \
			if ((uintptr_t)(i) & 0x0000ffff00000000ULL)	      \
				PPC_ORI(d, d,				      \
					((uintptr_t)(i) >> 32) & 0xffff);     \
			PPC_SLDI(d, d, 32);				      \
			if ((uintptr_t)(i) & 0x00000000ffff0000ULL)	      \
				PPC_ORIS(d, d,				      \
					 ((uintptr_t)(i) >> 16) & 0xffff);    \
			if ((uintptr_t)(i) & 0x000000000000ffffULL)	      \
				PPC_ORI(d, d, (uintptr_t)(i) & 0xffff);	      \
		} } while (0);

static inline bool is_nearbranch(int offset)
{
	return (offset < 32768) && (offset >= -32768);
}
=======
/* Long jump; (unconditional 'branch') */
#define PPC_JMP(dest)							      \
	do {								      \
		long offset = (long)(dest) - CTX_NIA(ctx);		      \
		if ((dest) != 0 && !is_offset_in_branch_range(offset)) {		      \
			pr_err_ratelimited("Branch offset 0x%lx (@%u) out of range\n", offset, ctx->idx);			\
			return -ERANGE;					      \
		}							      \
		EMIT(PPC_RAW_BRANCH(offset));				      \
	} while (0)

/* "cond" here covers BO:BI fields. */
#define PPC_BCC_SHORT(cond, dest)					      \
	do {								      \
		long offset = (long)(dest) - CTX_NIA(ctx);		      \
		if ((dest) != 0 && !is_offset_in_cond_branch_range(offset)) {		      \
			pr_err_ratelimited("Conditional branch offset 0x%lx (@%u) out of range\n", offset, ctx->idx);		\
			return -ERANGE;					      \
		}							      \
		EMIT(PPC_INST_BRANCH_COND | (((cond) & 0x3ff) << 16) | (offset & 0xfffc));					\
	} while (0)

/* Sign-extended 32-bit immediate load */
#define PPC_LI32(d, i)		do {					      \
		if ((int)(uintptr_t)(i) >= -32768 &&			      \
				(int)(uintptr_t)(i) < 32768)		      \
			EMIT(PPC_RAW_LI(d, i));				      \
		else {							      \
			EMIT(PPC_RAW_LIS(d, IMM_H(i)));			      \
			if (IMM_L(i))					      \
				EMIT(PPC_RAW_ORI(d, d, IMM_L(i)));	      \
		} } while(0)

#ifdef CONFIG_PPC64
#define PPC_LI64(d, i)		do {					      \
		if ((long)(i) >= -2147483648 &&				      \
				(long)(i) < 2147483648)			      \
			PPC_LI32(d, i);					      \
		else {							      \
			if (!((uintptr_t)(i) & 0xffff800000000000ULL))	      \
				EMIT(PPC_RAW_LI(d, ((uintptr_t)(i) >> 32) &   \
						0xffff));		      \
			else {						      \
				EMIT(PPC_RAW_LIS(d, ((uintptr_t)(i) >> 48))); \
				if ((uintptr_t)(i) & 0x0000ffff00000000ULL)   \
					EMIT(PPC_RAW_ORI(d, d,		      \
					  ((uintptr_t)(i) >> 32) & 0xffff));  \
			}						      \
			EMIT(PPC_RAW_SLDI(d, d, 32));			      \
			if ((uintptr_t)(i) & 0x00000000ffff0000ULL)	      \
				EMIT(PPC_RAW_ORIS(d, d,			      \
					 ((uintptr_t)(i) >> 16) & 0xffff));   \
			if ((uintptr_t)(i) & 0x000000000000ffffULL)	      \
				EMIT(PPC_RAW_ORI(d, d, (uintptr_t)(i) &       \
							0xffff));             \
		} } while (0)
#endif
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

/*
 * The fly in the ointment of code size changing from pass to pass is
 * avoided by padding the short branch case with a NOP.	 If code size differs
 * with different branch reaches we will have the issue of code moving from
 * one pass to the next and will need a few passes to converge on a stable
 * state.
 */
#define PPC_BCC(cond, dest)	do {					      \
<<<<<<< HEAD
		if (is_nearbranch((dest) - (ctx->idx * 4))) {		      \
			PPC_BCC_SHORT(cond, dest);			      \
			PPC_NOP();					      \
		} else {						      \
			/* Flip the 'T or F' bit to invert comparison */      \
			PPC_BCC_SHORT(cond ^ COND_CMP_TRUE, (ctx->idx+2)*4);  \
=======
		if (is_offset_in_cond_branch_range((long)(dest) - CTX_NIA(ctx))) {	\
			PPC_BCC_SHORT(cond, dest);			      \
			EMIT(PPC_RAW_NOP());				      \
		} else {						      \
			/* Flip the 'T or F' bit to invert comparison */      \
			PPC_BCC_SHORT(cond ^ COND_CMP_TRUE, CTX_NIA(ctx) + 2*4);  \
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
			PPC_JMP(dest);					      \
		} } while(0)

/* To create a branch condition, select a bit of cr0... */
#define CR0_LT		0
#define CR0_GT		1
#define CR0_EQ		2
/* ...and modify BO[3] */
#define COND_CMP_TRUE	0x100
#define COND_CMP_FALSE	0x000
/* Together, they make all required comparisons: */
#define COND_GT		(CR0_GT | COND_CMP_TRUE)
#define COND_GE		(CR0_LT | COND_CMP_FALSE)
#define COND_EQ		(CR0_EQ | COND_CMP_TRUE)
#define COND_NE		(CR0_EQ | COND_CMP_FALSE)
#define COND_LT		(CR0_LT | COND_CMP_TRUE)
<<<<<<< HEAD

#define SEEN_DATAREF 0x10000 /* might call external helpers */
#define SEEN_XREG    0x20000 /* X reg is used */
#define SEEN_MEM     0x40000 /* SEEN_MEM+(1<<n) = use mem[n] for temporary
			      * storage */
#define SEEN_MEM_MSK 0x0ffff

struct codegen_context {
	unsigned int seen;
	unsigned int idx;
	int pc_ret0; /* bpf index of first RET #0 instruction (if any) */
};

=======
#define COND_LE		(CR0_GT | COND_CMP_FALSE)

#define SEEN_FUNC	0x20000000 /* might call external helpers */
#define SEEN_TAILCALL	0x40000000 /* uses tail calls */

struct codegen_context {
	/*
	 * This is used to track register usage as well
	 * as calls to external helpers.
	 * - register usage is tracked with corresponding
	 *   bits (r3-r31)
	 * - rest of the bits can be used to track other
	 *   things -- for now, we use bits 0 to 2
	 *   encoded in SEEN_* macros above
	 */
	unsigned int seen;
	unsigned int idx;
	unsigned int stack_size;
	int b2p[MAX_BPF_JIT_REG + 2];
	unsigned int exentry_idx;
	unsigned int alt_exit_addr;
};

#define bpf_to_ppc(r)	(ctx->b2p[r])

#ifdef CONFIG_PPC32
#define BPF_FIXUP_LEN	3 /* Three instructions => 12 bytes */
#else
#define BPF_FIXUP_LEN	2 /* Two instructions => 8 bytes */
#endif

static inline bool bpf_is_seen_register(struct codegen_context *ctx, int i)
{
	return ctx->seen & (1 << (31 - i));
}

static inline void bpf_set_seen_register(struct codegen_context *ctx, int i)
{
	ctx->seen |= 1 << (31 - i);
}

static inline void bpf_clear_seen_register(struct codegen_context *ctx, int i)
{
	ctx->seen &= ~(1 << (31 - i));
}

void bpf_jit_init_reg_mapping(struct codegen_context *ctx);
int bpf_jit_emit_func_call_rel(u32 *image, u32 *fimage, struct codegen_context *ctx, u64 func);
int bpf_jit_build_body(struct bpf_prog *fp, u32 *image, u32 *fimage, struct codegen_context *ctx,
		       u32 *addrs, int pass, bool extra_pass);
void bpf_jit_build_prologue(u32 *image, struct codegen_context *ctx);
void bpf_jit_build_epilogue(u32 *image, struct codegen_context *ctx);
void bpf_jit_realloc_regs(struct codegen_context *ctx);
int bpf_jit_emit_exit_insn(u32 *image, struct codegen_context *ctx, int tmp_reg, long exit_addr);

int bpf_add_extable_entry(struct bpf_prog *fp, u32 *image, u32 *fimage, int pass,
			  struct codegen_context *ctx, int insn_idx,
			  int jmp_off, int dst_reg);

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#endif

#endif
