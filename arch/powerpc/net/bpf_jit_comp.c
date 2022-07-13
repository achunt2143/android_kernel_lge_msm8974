<<<<<<< HEAD
/* bpf_jit_comp.c: BPF JIT compiler for PPC64
 *
 * Copyright 2011 Matt Evans <matt@ozlabs.org>, IBM Corporation
 *
 * Based on the x86 BPF compiler, by Eric Dumazet (eric.dumazet@gmail.com)
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; version 2
 * of the License.
 */
#include <linux/moduleloader.h>
#include <asm/cacheflush.h>
#include <linux/netdevice.h>
#include <linux/filter.h>
#include "bpf_jit.h"

#ifndef __BIG_ENDIAN
/* There are endianness assumptions herein. */
#error "Little-endian PPC not supported in BPF compiler"
#endif

int bpf_jit_enable __read_mostly;


static inline void bpf_flush_icache(void *start, void *end)
{
	smp_wmb();
	flush_icache_range((unsigned long)start, (unsigned long)end);
}

static void bpf_jit_build_prologue(struct sk_filter *fp, u32 *image,
				   struct codegen_context *ctx)
{
	int i;
	const struct sock_filter *filter = fp->insns;

	if (ctx->seen & (SEEN_MEM | SEEN_DATAREF)) {
		/* Make stackframe */
		if (ctx->seen & SEEN_DATAREF) {
			/* If we call any helpers (for loads), save LR */
			EMIT(PPC_INST_MFLR | __PPC_RT(0));
			PPC_STD(0, 1, 16);

			/* Back up non-volatile regs. */
			PPC_STD(r_D, 1, -(8*(32-r_D)));
			PPC_STD(r_HL, 1, -(8*(32-r_HL)));
		}
		if (ctx->seen & SEEN_MEM) {
			/*
			 * Conditionally save regs r15-r31 as some will be used
			 * for M[] data.
			 */
			for (i = r_M; i < (r_M+16); i++) {
				if (ctx->seen & (1 << (i-r_M)))
					PPC_STD(i, 1, -(8*(32-i)));
			}
		}
		EMIT(PPC_INST_STDU | __PPC_RS(1) | __PPC_RA(1) |
		     (-BPF_PPC_STACKFRAME & 0xfffc));
	}

	if (ctx->seen & SEEN_DATAREF) {
		/*
		 * If this filter needs to access skb data,
		 * prepare r_D and r_HL:
		 *  r_HL = skb->len - skb->data_len
		 *  r_D	 = skb->data
		 */
		PPC_LWZ_OFFS(r_scratch1, r_skb, offsetof(struct sk_buff,
							 data_len));
		PPC_LWZ_OFFS(r_HL, r_skb, offsetof(struct sk_buff, len));
		PPC_SUB(r_HL, r_HL, r_scratch1);
		PPC_LD_OFFS(r_D, r_skb, offsetof(struct sk_buff, data));
	}

	if (ctx->seen & SEEN_XREG) {
		/*
		 * TODO: Could also detect whether first instr. sets X and
		 * avoid this (as below, with A).
		 */
		PPC_LI(r_X, 0);
	}

	switch (filter[0].code) {
	case BPF_S_RET_K:
	case BPF_S_LD_W_LEN:
	case BPF_S_ANC_PROTOCOL:
	case BPF_S_ANC_IFINDEX:
	case BPF_S_ANC_MARK:
	case BPF_S_ANC_RXHASH:
	case BPF_S_ANC_CPU:
	case BPF_S_ANC_QUEUE:
	case BPF_S_LD_W_ABS:
	case BPF_S_LD_H_ABS:
	case BPF_S_LD_B_ABS:
		/* first instruction sets A register (or is RET 'constant') */
		break;
	default:
		/* make sure we dont leak kernel information to user */
		PPC_LI(r_A, 0);
	}
}

static void bpf_jit_build_epilogue(u32 *image, struct codegen_context *ctx)
{
	int i;

	if (ctx->seen & (SEEN_MEM | SEEN_DATAREF)) {
		PPC_ADDI(1, 1, BPF_PPC_STACKFRAME);
		if (ctx->seen & SEEN_DATAREF) {
			PPC_LD(0, 1, 16);
			PPC_MTLR(0);
			PPC_LD(r_D, 1, -(8*(32-r_D)));
			PPC_LD(r_HL, 1, -(8*(32-r_HL)));
		}
		if (ctx->seen & SEEN_MEM) {
			/* Restore any saved non-vol registers */
			for (i = r_M; i < (r_M+16); i++) {
				if (ctx->seen & (1 << (i-r_M)))
					PPC_LD(i, 1, -(8*(32-i)));
			}
		}
	}
	/* The RETs have left a return value in R3. */

	PPC_BLR();
}

#define CHOOSE_LOAD_FUNC(K, func) \
	((int)K < 0 ? ((int)K >= SKF_LL_OFF ? func##_negative_offset : func) : func##_positive_offset)

/* Assemble the body code between the prologue & epilogue. */
static int bpf_jit_build_body(struct sk_filter *fp, u32 *image,
			      struct codegen_context *ctx,
			      unsigned int *addrs)
{
	const struct sock_filter *filter = fp->insns;
	int flen = fp->len;
	u8 *func;
	unsigned int true_cond;
	int i;

	/* Start of epilogue code */
	unsigned int exit_addr = addrs[flen];

	for (i = 0; i < flen; i++) {
		unsigned int K = filter[i].k;

		/*
		 * addrs[] maps a BPF bytecode address into a real offset from
		 * the start of the body code.
		 */
		addrs[i] = ctx->idx * 4;

		switch (filter[i].code) {
			/*** ALU ops ***/
		case BPF_S_ALU_ADD_X: /* A += X; */
			ctx->seen |= SEEN_XREG;
			PPC_ADD(r_A, r_A, r_X);
			break;
		case BPF_S_ALU_ADD_K: /* A += K; */
			if (!K)
				break;
			PPC_ADDI(r_A, r_A, IMM_L(K));
			if (K >= 32768)
				PPC_ADDIS(r_A, r_A, IMM_HA(K));
			break;
		case BPF_S_ALU_SUB_X: /* A -= X; */
			ctx->seen |= SEEN_XREG;
			PPC_SUB(r_A, r_A, r_X);
			break;
		case BPF_S_ALU_SUB_K: /* A -= K */
			if (!K)
				break;
			PPC_ADDI(r_A, r_A, IMM_L(-K));
			if (K >= 32768)
				PPC_ADDIS(r_A, r_A, IMM_HA(-K));
			break;
		case BPF_S_ALU_MUL_X: /* A *= X; */
			ctx->seen |= SEEN_XREG;
			PPC_MUL(r_A, r_A, r_X);
			break;
		case BPF_S_ALU_MUL_K: /* A *= K */
			if (K < 32768)
				PPC_MULI(r_A, r_A, K);
			else {
				PPC_LI32(r_scratch1, K);
				PPC_MUL(r_A, r_A, r_scratch1);
			}
			break;
		case BPF_S_ALU_DIV_X: /* A /= X; */
			ctx->seen |= SEEN_XREG;
			PPC_CMPWI(r_X, 0);
			if (ctx->pc_ret0 != -1) {
				PPC_BCC(COND_EQ, addrs[ctx->pc_ret0]);
			} else {
				/*
				 * Exit, returning 0; first pass hits here
				 * (longer worst-case code size).
				 */
				PPC_BCC_SHORT(COND_NE, (ctx->idx*4)+12);
				PPC_LI(r_ret, 0);
				PPC_JMP(exit_addr);
			}
			PPC_DIVWU(r_A, r_A, r_X);
			break;
		case BPF_S_ALU_DIV_K: /* A = reciprocal_divide(A, K); */
			PPC_LI32(r_scratch1, K);
			/* Top 32 bits of 64bit result -> A */
			PPC_MULHWU(r_A, r_A, r_scratch1);
			break;
		case BPF_S_ALU_AND_X:
			ctx->seen |= SEEN_XREG;
			PPC_AND(r_A, r_A, r_X);
			break;
		case BPF_S_ALU_AND_K:
			if (!IMM_H(K))
				PPC_ANDI(r_A, r_A, K);
			else {
				PPC_LI32(r_scratch1, K);
				PPC_AND(r_A, r_A, r_scratch1);
			}
			break;
		case BPF_S_ALU_OR_X:
			ctx->seen |= SEEN_XREG;
			PPC_OR(r_A, r_A, r_X);
			break;
		case BPF_S_ALU_OR_K:
			if (IMM_L(K))
				PPC_ORI(r_A, r_A, IMM_L(K));
			if (K >= 65536)
				PPC_ORIS(r_A, r_A, IMM_H(K));
			break;
		case BPF_S_ALU_LSH_X: /* A <<= X; */
			ctx->seen |= SEEN_XREG;
			PPC_SLW(r_A, r_A, r_X);
			break;
		case BPF_S_ALU_LSH_K:
			if (K == 0)
				break;
			else
				PPC_SLWI(r_A, r_A, K);
			break;
		case BPF_S_ALU_RSH_X: /* A >>= X; */
			ctx->seen |= SEEN_XREG;
			PPC_SRW(r_A, r_A, r_X);
			break;
		case BPF_S_ALU_RSH_K: /* A >>= K; */
			if (K == 0)
				break;
			else
				PPC_SRWI(r_A, r_A, K);
			break;
		case BPF_S_ALU_NEG:
			PPC_NEG(r_A, r_A);
			break;
		case BPF_S_RET_K:
			PPC_LI32(r_ret, K);
			if (!K) {
				if (ctx->pc_ret0 == -1)
					ctx->pc_ret0 = i;
			}
			/*
			 * If this isn't the very last instruction, branch to
			 * the epilogue if we've stuff to clean up.  Otherwise,
			 * if there's nothing to tidy, just return.  If we /are/
			 * the last instruction, we're about to fall through to
			 * the epilogue to return.
			 */
			if (i != flen - 1) {
				/*
				 * Note: 'seen' is properly valid only on pass
				 * #2.	Both parts of this conditional are the
				 * same instruction size though, meaning the
				 * first pass will still correctly determine the
				 * code size/addresses.
				 */
				if (ctx->seen)
					PPC_JMP(exit_addr);
				else
					PPC_BLR();
			}
			break;
		case BPF_S_RET_A:
			PPC_MR(r_ret, r_A);
			if (i != flen - 1) {
				if (ctx->seen)
					PPC_JMP(exit_addr);
				else
					PPC_BLR();
			}
			break;
		case BPF_S_MISC_TAX: /* X = A */
			PPC_MR(r_X, r_A);
			break;
		case BPF_S_MISC_TXA: /* A = X */
			ctx->seen |= SEEN_XREG;
			PPC_MR(r_A, r_X);
			break;

			/*** Constant loads/M[] access ***/
		case BPF_S_LD_IMM: /* A = K */
			PPC_LI32(r_A, K);
			break;
		case BPF_S_LDX_IMM: /* X = K */
			PPC_LI32(r_X, K);
			break;
		case BPF_S_LD_MEM: /* A = mem[K] */
			PPC_MR(r_A, r_M + (K & 0xf));
			ctx->seen |= SEEN_MEM | (1<<(K & 0xf));
			break;
		case BPF_S_LDX_MEM: /* X = mem[K] */
			PPC_MR(r_X, r_M + (K & 0xf));
			ctx->seen |= SEEN_MEM | (1<<(K & 0xf));
			break;
		case BPF_S_ST: /* mem[K] = A */
			PPC_MR(r_M + (K & 0xf), r_A);
			ctx->seen |= SEEN_MEM | (1<<(K & 0xf));
			break;
		case BPF_S_STX: /* mem[K] = X */
			PPC_MR(r_M + (K & 0xf), r_X);
			ctx->seen |= SEEN_XREG | SEEN_MEM | (1<<(K & 0xf));
			break;
		case BPF_S_LD_W_LEN: /*	A = skb->len; */
			BUILD_BUG_ON(FIELD_SIZEOF(struct sk_buff, len) != 4);
			PPC_LWZ_OFFS(r_A, r_skb, offsetof(struct sk_buff, len));
			break;
		case BPF_S_LDX_W_LEN: /* X = skb->len; */
			PPC_LWZ_OFFS(r_X, r_skb, offsetof(struct sk_buff, len));
			break;

			/*** Ancillary info loads ***/

			/* None of the BPF_S_ANC* codes appear to be passed by
			 * sk_chk_filter().  The interpreter and the x86 BPF
			 * compiler implement them so we do too -- they may be
			 * planted in future.
			 */
		case BPF_S_ANC_PROTOCOL: /* A = ntohs(skb->protocol); */
			BUILD_BUG_ON(FIELD_SIZEOF(struct sk_buff,
						  protocol) != 2);
			PPC_LHZ_OFFS(r_A, r_skb, offsetof(struct sk_buff,
							  protocol));
			/* ntohs is a NOP with BE loads. */
			break;
		case BPF_S_ANC_IFINDEX:
			PPC_LD_OFFS(r_scratch1, r_skb, offsetof(struct sk_buff,
								dev));
			PPC_CMPDI(r_scratch1, 0);
			if (ctx->pc_ret0 != -1) {
				PPC_BCC(COND_EQ, addrs[ctx->pc_ret0]);
			} else {
				/* Exit, returning 0; first pass hits here. */
				PPC_BCC_SHORT(COND_NE, (ctx->idx*4)+12);
				PPC_LI(r_ret, 0);
				PPC_JMP(exit_addr);
			}
			BUILD_BUG_ON(FIELD_SIZEOF(struct net_device,
						  ifindex) != 4);
			PPC_LWZ_OFFS(r_A, r_scratch1,
				     offsetof(struct net_device, ifindex));
			break;
		case BPF_S_ANC_MARK:
			BUILD_BUG_ON(FIELD_SIZEOF(struct sk_buff, mark) != 4);
			PPC_LWZ_OFFS(r_A, r_skb, offsetof(struct sk_buff,
							  mark));
			break;
		case BPF_S_ANC_RXHASH:
			BUILD_BUG_ON(FIELD_SIZEOF(struct sk_buff, rxhash) != 4);
			PPC_LWZ_OFFS(r_A, r_skb, offsetof(struct sk_buff,
							  rxhash));
			break;
		case BPF_S_ANC_QUEUE:
			BUILD_BUG_ON(FIELD_SIZEOF(struct sk_buff,
						  queue_mapping) != 2);
			PPC_LHZ_OFFS(r_A, r_skb, offsetof(struct sk_buff,
							  queue_mapping));
			break;
		case BPF_S_ANC_CPU:
#ifdef CONFIG_SMP
			/*
			 * PACA ptr is r13:
			 * raw_smp_processor_id() = local_paca->paca_index
			 */
			BUILD_BUG_ON(FIELD_SIZEOF(struct paca_struct,
						  paca_index) != 2);
			PPC_LHZ_OFFS(r_A, 13,
				     offsetof(struct paca_struct, paca_index));
#else
			PPC_LI(r_A, 0);
#endif
			break;

			/*** Absolute loads from packet header/data ***/
		case BPF_S_LD_W_ABS:
			func = CHOOSE_LOAD_FUNC(K, sk_load_word);
			goto common_load;
		case BPF_S_LD_H_ABS:
			func = CHOOSE_LOAD_FUNC(K, sk_load_half);
			goto common_load;
		case BPF_S_LD_B_ABS:
			func = CHOOSE_LOAD_FUNC(K, sk_load_byte);
		common_load:
			/* Load from [K]. */
			ctx->seen |= SEEN_DATAREF;
			PPC_LI64(r_scratch1, func);
			PPC_MTLR(r_scratch1);
			PPC_LI32(r_addr, K);
			PPC_BLRL();
			/*
			 * Helper returns 'lt' condition on error, and an
			 * appropriate return value in r3
			 */
			PPC_BCC(COND_LT, exit_addr);
			break;

			/*** Indirect loads from packet header/data ***/
		case BPF_S_LD_W_IND:
			func = sk_load_word;
			goto common_load_ind;
		case BPF_S_LD_H_IND:
			func = sk_load_half;
			goto common_load_ind;
		case BPF_S_LD_B_IND:
			func = sk_load_byte;
		common_load_ind:
			/*
			 * Load from [X + K].  Negative offsets are tested for
			 * in the helper functions.
			 */
			ctx->seen |= SEEN_DATAREF | SEEN_XREG;
			PPC_LI64(r_scratch1, func);
			PPC_MTLR(r_scratch1);
			PPC_ADDI(r_addr, r_X, IMM_L(K));
			if (K >= 32768)
				PPC_ADDIS(r_addr, r_addr, IMM_HA(K));
			PPC_BLRL();
			/* If error, cr0.LT set */
			PPC_BCC(COND_LT, exit_addr);
			break;

		case BPF_S_LDX_B_MSH:
			func = CHOOSE_LOAD_FUNC(K, sk_load_byte_msh);
			goto common_load;
			break;

			/*** Jump and branches ***/
		case BPF_S_JMP_JA:
			if (K != 0)
				PPC_JMP(addrs[i + 1 + K]);
			break;

		case BPF_S_JMP_JGT_K:
		case BPF_S_JMP_JGT_X:
			true_cond = COND_GT;
			goto cond_branch;
		case BPF_S_JMP_JGE_K:
		case BPF_S_JMP_JGE_X:
			true_cond = COND_GE;
			goto cond_branch;
		case BPF_S_JMP_JEQ_K:
		case BPF_S_JMP_JEQ_X:
			true_cond = COND_EQ;
			goto cond_branch;
		case BPF_S_JMP_JSET_K:
		case BPF_S_JMP_JSET_X:
			true_cond = COND_NE;
			/* Fall through */
		cond_branch:
			/* same targets, can avoid doing the test :) */
			if (filter[i].jt == filter[i].jf) {
				if (filter[i].jt > 0)
					PPC_JMP(addrs[i + 1 + filter[i].jt]);
				break;
			}

			switch (filter[i].code) {
			case BPF_S_JMP_JGT_X:
			case BPF_S_JMP_JGE_X:
			case BPF_S_JMP_JEQ_X:
				ctx->seen |= SEEN_XREG;
				PPC_CMPLW(r_A, r_X);
				break;
			case BPF_S_JMP_JSET_X:
				ctx->seen |= SEEN_XREG;
				PPC_AND_DOT(r_scratch1, r_A, r_X);
				break;
			case BPF_S_JMP_JEQ_K:
			case BPF_S_JMP_JGT_K:
			case BPF_S_JMP_JGE_K:
				if (K < 32768)
					PPC_CMPLWI(r_A, K);
				else {
					PPC_LI32(r_scratch1, K);
					PPC_CMPLW(r_A, r_scratch1);
				}
				break;
			case BPF_S_JMP_JSET_K:
				if (K < 32768)
					/* PPC_ANDI is /only/ dot-form */
					PPC_ANDI(r_scratch1, r_A, K);
				else {
					PPC_LI32(r_scratch1, K);
					PPC_AND_DOT(r_scratch1, r_A,
						    r_scratch1);
				}
				break;
			}
			/* Sometimes branches are constructed "backward", with
			 * the false path being the branch and true path being
			 * a fallthrough to the next instruction.
			 */
			if (filter[i].jt == 0)
				/* Swap the sense of the branch */
				PPC_BCC(true_cond ^ COND_CMP_TRUE,
					addrs[i + 1 + filter[i].jf]);
			else {
				PPC_BCC(true_cond, addrs[i + 1 + filter[i].jt]);
				if (filter[i].jf != 0)
					PPC_JMP(addrs[i + 1 + filter[i].jf]);
			}
			break;
		default:
			/* The filter contains something cruel & unusual.
			 * We don't handle it, but also there shouldn't be
			 * anything missing from our list.
			 */
			if (printk_ratelimit())
				pr_err("BPF filter opcode %04x (@%d) unsupported\n",
				       filter[i].code, i);
			return -ENOTSUPP;
		}

	}
	/* Set end-of-body-code address for exit. */
	addrs[i] = ctx->idx * 4;

	return 0;
}

void bpf_jit_compile(struct sk_filter *fp)
{
	unsigned int proglen;
	unsigned int alloclen;
	u32 *image = NULL;
	u32 *code_base;
	unsigned int *addrs;
	struct codegen_context cgctx;
	int pass;
	int flen = fp->len;

	if (!bpf_jit_enable)
		return;

	addrs = kzalloc((flen+1) * sizeof(*addrs), GFP_KERNEL);
	if (addrs == NULL)
		return;

	/*
	 * There are multiple assembly passes as the generated code will change
	 * size as it settles down, figuring out the max branch offsets/exit
	 * paths required.
	 *
	 * The range of standard conditional branches is +/- 32Kbytes.	Since
	 * BPF_MAXINSNS = 4096, we can only jump from (worst case) start to
	 * finish with 8 bytes/instruction.  Not feasible, so long jumps are
	 * used, distinct from short branches.
	 *
	 * Current:
	 *
	 * For now, both branch types assemble to 2 words (short branches padded
	 * with a NOP); this is less efficient, but assembly will always complete
	 * after exactly 3 passes:
	 *
	 * First pass: No code buffer; Program is "faux-generated" -- no code
	 * emitted but maximum size of output determined (and addrs[] filled
	 * in).	 Also, we note whether we use M[], whether we use skb data, etc.
	 * All generation choices assumed to be 'worst-case', e.g. branches all
	 * far (2 instructions), return path code reduction not available, etc.
	 *
	 * Second pass: Code buffer allocated with size determined previously.
	 * Prologue generated to support features we have seen used.  Exit paths
	 * determined and addrs[] is filled in again, as code may be slightly
	 * smaller as a result.
	 *
	 * Third pass: Code generated 'for real', and branch destinations
	 * determined from now-accurate addrs[] map.
	 *
	 * Ideal:
	 *
	 * If we optimise this, near branches will be shorter.	On the
	 * first assembly pass, we should err on the side of caution and
	 * generate the biggest code.  On subsequent passes, branches will be
	 * generated short or long and code size will reduce.  With smaller
	 * code, more branches may fall into the short category, and code will
	 * reduce more.
	 *
	 * Finally, if we see one pass generate code the same size as the
	 * previous pass we have converged and should now generate code for
	 * real.  Allocating at the end will also save the memory that would
	 * otherwise be wasted by the (small) current code shrinkage.
	 * Preferably, we should do a small number of passes (e.g. 5) and if we
	 * haven't converged by then, get impatient and force code to generate
	 * as-is, even if the odd branch would be left long.  The chances of a
	 * long jump are tiny with all but the most enormous of BPF filter
	 * inputs, so we should usually converge on the third pass.
	 */

	cgctx.idx = 0;
	cgctx.seen = 0;
	cgctx.pc_ret0 = -1;
	/* Scouting faux-generate pass 0 */
	if (bpf_jit_build_body(fp, 0, &cgctx, addrs))
		/* We hit something illegal or unsupported. */
		goto out;

=======
// SPDX-License-Identifier: GPL-2.0-only
/*
 * eBPF JIT compiler
 *
 * Copyright 2016 Naveen N. Rao <naveen.n.rao@linux.vnet.ibm.com>
 *		  IBM Corporation
 *
 * Based on the powerpc classic BPF JIT compiler by Matt Evans
 */
#include <linux/moduleloader.h>
#include <asm/cacheflush.h>
#include <asm/asm-compat.h>
#include <linux/netdevice.h>
#include <linux/filter.h>
#include <linux/if_vlan.h>
#include <linux/kernel.h>
#include <linux/memory.h>
#include <linux/bpf.h>

#include <asm/kprobes.h>
#include <asm/code-patching.h>

#include "bpf_jit.h"

static void bpf_jit_fill_ill_insns(void *area, unsigned int size)
{
	memset32(area, BREAKPOINT_INSTRUCTION, size / 4);
}

int bpf_jit_emit_exit_insn(u32 *image, struct codegen_context *ctx, int tmp_reg, long exit_addr)
{
	if (!exit_addr || is_offset_in_branch_range(exit_addr - (ctx->idx * 4))) {
		PPC_JMP(exit_addr);
	} else if (ctx->alt_exit_addr) {
		if (WARN_ON(!is_offset_in_branch_range((long)ctx->alt_exit_addr - (ctx->idx * 4))))
			return -1;
		PPC_JMP(ctx->alt_exit_addr);
	} else {
		ctx->alt_exit_addr = ctx->idx * 4;
		bpf_jit_build_epilogue(image, ctx);
	}

	return 0;
}

struct powerpc_jit_data {
	/* address of rw header */
	struct bpf_binary_header *hdr;
	/* address of ro final header */
	struct bpf_binary_header *fhdr;
	u32 *addrs;
	u8 *fimage;
	u32 proglen;
	struct codegen_context ctx;
};

bool bpf_jit_needs_zext(void)
{
	return true;
}

struct bpf_prog *bpf_int_jit_compile(struct bpf_prog *fp)
{
	u32 proglen;
	u32 alloclen;
	u8 *image = NULL;
	u32 *code_base;
	u32 *addrs;
	struct powerpc_jit_data *jit_data;
	struct codegen_context cgctx;
	int pass;
	int flen;
	struct bpf_binary_header *fhdr = NULL;
	struct bpf_binary_header *hdr = NULL;
	struct bpf_prog *org_fp = fp;
	struct bpf_prog *tmp_fp;
	bool bpf_blinded = false;
	bool extra_pass = false;
	u8 *fimage = NULL;
	u32 *fcode_base;
	u32 extable_len;
	u32 fixup_len;

	if (!fp->jit_requested)
		return org_fp;

	tmp_fp = bpf_jit_blind_constants(org_fp);
	if (IS_ERR(tmp_fp))
		return org_fp;

	if (tmp_fp != org_fp) {
		bpf_blinded = true;
		fp = tmp_fp;
	}

	jit_data = fp->aux->jit_data;
	if (!jit_data) {
		jit_data = kzalloc(sizeof(*jit_data), GFP_KERNEL);
		if (!jit_data) {
			fp = org_fp;
			goto out;
		}
		fp->aux->jit_data = jit_data;
	}

	flen = fp->len;
	addrs = jit_data->addrs;
	if (addrs) {
		cgctx = jit_data->ctx;
		/*
		 * JIT compiled to a writable location (image/code_base) first.
		 * It is then moved to the readonly final location (fimage/fcode_base)
		 * using instruction patching.
		 */
		fimage = jit_data->fimage;
		fhdr = jit_data->fhdr;
		proglen = jit_data->proglen;
		hdr = jit_data->hdr;
		image = (void *)hdr + ((void *)fimage - (void *)fhdr);
		extra_pass = true;
		/* During extra pass, ensure index is reset before repopulating extable entries */
		cgctx.exentry_idx = 0;
		goto skip_init_ctx;
	}

	addrs = kcalloc(flen + 1, sizeof(*addrs), GFP_KERNEL);
	if (addrs == NULL) {
		fp = org_fp;
		goto out_addrs;
	}

	memset(&cgctx, 0, sizeof(struct codegen_context));
	bpf_jit_init_reg_mapping(&cgctx);

	/* Make sure that the stack is quadword aligned. */
	cgctx.stack_size = round_up(fp->aux->stack_depth, 16);

	/* Scouting faux-generate pass 0 */
	if (bpf_jit_build_body(fp, NULL, NULL, &cgctx, addrs, 0, false)) {
		/* We hit something illegal or unsupported. */
		fp = org_fp;
		goto out_addrs;
	}

	/*
	 * If we have seen a tail call, we need a second pass.
	 * This is because bpf_jit_emit_common_epilogue() is called
	 * from bpf_jit_emit_tail_call() with a not yet stable ctx->seen.
	 * We also need a second pass if we ended up with too large
	 * a program so as to ensure BPF_EXIT branches are in range.
	 */
	if (cgctx.seen & SEEN_TAILCALL || !is_offset_in_branch_range((long)cgctx.idx * 4)) {
		cgctx.idx = 0;
		if (bpf_jit_build_body(fp, NULL, NULL, &cgctx, addrs, 0, false)) {
			fp = org_fp;
			goto out_addrs;
		}
	}

	bpf_jit_realloc_regs(&cgctx);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	/*
	 * Pretend to build prologue, given the features we've seen.  This will
	 * update ctgtx.idx as it pretends to output instructions, then we can
	 * calculate total size from idx.
	 */
<<<<<<< HEAD
	bpf_jit_build_prologue(fp, 0, &cgctx);
	bpf_jit_build_epilogue(0, &cgctx);

	proglen = cgctx.idx * 4;
	alloclen = proglen + FUNCTION_DESCR_SIZE;
	image = module_alloc(max_t(unsigned int, alloclen,
				   sizeof(struct work_struct)));
	if (!image)
		goto out;

	code_base = image + (FUNCTION_DESCR_SIZE/4);
=======
	bpf_jit_build_prologue(NULL, &cgctx);
	addrs[fp->len] = cgctx.idx * 4;
	bpf_jit_build_epilogue(NULL, &cgctx);

	fixup_len = fp->aux->num_exentries * BPF_FIXUP_LEN * 4;
	extable_len = fp->aux->num_exentries * sizeof(struct exception_table_entry);

	proglen = cgctx.idx * 4;
	alloclen = proglen + FUNCTION_DESCR_SIZE + fixup_len + extable_len;

	fhdr = bpf_jit_binary_pack_alloc(alloclen, &fimage, 4, &hdr, &image,
					      bpf_jit_fill_ill_insns);
	if (!fhdr) {
		fp = org_fp;
		goto out_addrs;
	}

	if (extable_len)
		fp->aux->extable = (void *)fimage + FUNCTION_DESCR_SIZE + proglen + fixup_len;

skip_init_ctx:
	code_base = (u32 *)(image + FUNCTION_DESCR_SIZE);
	fcode_base = (u32 *)(fimage + FUNCTION_DESCR_SIZE);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	/* Code generation passes 1-2 */
	for (pass = 1; pass < 3; pass++) {
		/* Now build the prologue, body code & epilogue for real. */
		cgctx.idx = 0;
<<<<<<< HEAD
		bpf_jit_build_prologue(fp, code_base, &cgctx);
		bpf_jit_build_body(fp, code_base, &cgctx, addrs);
=======
		cgctx.alt_exit_addr = 0;
		bpf_jit_build_prologue(code_base, &cgctx);
		if (bpf_jit_build_body(fp, code_base, fcode_base, &cgctx, addrs, pass,
				       extra_pass)) {
			bpf_arch_text_copy(&fhdr->size, &hdr->size, sizeof(hdr->size));
			bpf_jit_binary_pack_free(fhdr, hdr);
			fp = org_fp;
			goto out_addrs;
		}
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		bpf_jit_build_epilogue(code_base, &cgctx);

		if (bpf_jit_enable > 1)
			pr_info("Pass %d: shrink = %d, seen = 0x%x\n", pass,
				proglen - (cgctx.idx * 4), cgctx.seen);
	}

	if (bpf_jit_enable > 1)
<<<<<<< HEAD
		pr_info("flen=%d proglen=%u pass=%d image=%p\n",
		       flen, proglen, pass, image);

	if (image) {
		if (bpf_jit_enable > 1)
			print_hex_dump(KERN_ERR, "JIT code: ",
				       DUMP_PREFIX_ADDRESS,
				       16, 1, code_base,
				       proglen, false);

		bpf_flush_icache(code_base, code_base + (proglen/4));
		/* Function descriptor nastiness: Address + TOC */
		((u64 *)image)[0] = (u64)code_base;
		((u64 *)image)[1] = local_paca->kernel_toc;
		fp->bpf_func = (void *)image;
	}
out:
	kfree(addrs);
	return;
}

static void jit_free_defer(struct work_struct *arg)
{
	module_free(NULL, arg);
}

/* run from softirq, we must use a work_struct to call
 * module_free() from process context
 */
void bpf_jit_free(struct sk_filter *fp)
{
	if (fp->bpf_func != sk_run_filter) {
		struct work_struct *work = (struct work_struct *)fp->bpf_func;

		INIT_WORK(work, jit_free_defer);
		schedule_work(work);
	}
=======
		/*
		 * Note that we output the base address of the code_base
		 * rather than image, since opcodes are in code_base.
		 */
		bpf_jit_dump(flen, proglen, pass, code_base);

#ifdef CONFIG_PPC64_ELF_ABI_V1
	/* Function descriptor nastiness: Address + TOC */
	((u64 *)image)[0] = (u64)fcode_base;
	((u64 *)image)[1] = local_paca->kernel_toc;
#endif

	fp->bpf_func = (void *)fimage;
	fp->jited = 1;
	fp->jited_len = proglen + FUNCTION_DESCR_SIZE;

	if (!fp->is_func || extra_pass) {
		if (bpf_jit_binary_pack_finalize(fp, fhdr, hdr)) {
			fp = org_fp;
			goto out_addrs;
		}
		bpf_prog_fill_jited_linfo(fp, addrs);
out_addrs:
		kfree(addrs);
		kfree(jit_data);
		fp->aux->jit_data = NULL;
	} else {
		jit_data->addrs = addrs;
		jit_data->ctx = cgctx;
		jit_data->proglen = proglen;
		jit_data->fimage = fimage;
		jit_data->fhdr = fhdr;
		jit_data->hdr = hdr;
	}

out:
	if (bpf_blinded)
		bpf_jit_prog_release_other(fp, fp == org_fp ? tmp_fp : org_fp);

	return fp;
}

/*
 * The caller should check for (BPF_MODE(code) == BPF_PROBE_MEM) before calling
 * this function, as this only applies to BPF_PROBE_MEM, for now.
 */
int bpf_add_extable_entry(struct bpf_prog *fp, u32 *image, u32 *fimage, int pass,
			  struct codegen_context *ctx, int insn_idx, int jmp_off,
			  int dst_reg)
{
	off_t offset;
	unsigned long pc;
	struct exception_table_entry *ex, *ex_entry;
	u32 *fixup;

	/* Populate extable entries only in the last pass */
	if (pass != 2)
		return 0;

	if (!fp->aux->extable ||
	    WARN_ON_ONCE(ctx->exentry_idx >= fp->aux->num_exentries))
		return -EINVAL;

	/*
	 * Program is first written to image before copying to the
	 * final location (fimage). Accordingly, update in the image first.
	 * As all offsets used are relative, copying as is to the
	 * final location should be alright.
	 */
	pc = (unsigned long)&image[insn_idx];
	ex = (void *)fp->aux->extable - (void *)fimage + (void *)image;

	fixup = (void *)ex -
		(fp->aux->num_exentries * BPF_FIXUP_LEN * 4) +
		(ctx->exentry_idx * BPF_FIXUP_LEN * 4);

	fixup[0] = PPC_RAW_LI(dst_reg, 0);
	if (IS_ENABLED(CONFIG_PPC32))
		fixup[1] = PPC_RAW_LI(dst_reg - 1, 0); /* clear higher 32-bit register too */

	fixup[BPF_FIXUP_LEN - 1] =
		PPC_RAW_BRANCH((long)(pc + jmp_off) - (long)&fixup[BPF_FIXUP_LEN - 1]);

	ex_entry = &ex[ctx->exentry_idx];

	offset = pc - (long)&ex_entry->insn;
	if (WARN_ON_ONCE(offset >= 0 || offset < INT_MIN))
		return -ERANGE;
	ex_entry->insn = offset;

	offset = (long)fixup - (long)&ex_entry->fixup;
	if (WARN_ON_ONCE(offset >= 0 || offset < INT_MIN))
		return -ERANGE;
	ex_entry->fixup = offset;

	ctx->exentry_idx++;
	return 0;
}

void *bpf_arch_text_copy(void *dst, void *src, size_t len)
{
	int err;

	if (WARN_ON_ONCE(core_kernel_text((unsigned long)dst)))
		return ERR_PTR(-EINVAL);

	mutex_lock(&text_mutex);
	err = patch_instructions(dst, src, len, false);
	mutex_unlock(&text_mutex);

	return err ? ERR_PTR(err) : dst;
}

int bpf_arch_text_invalidate(void *dst, size_t len)
{
	u32 insn = BREAKPOINT_INSTRUCTION;
	int ret;

	if (WARN_ON_ONCE(core_kernel_text((unsigned long)dst)))
		return -EINVAL;

	mutex_lock(&text_mutex);
	ret = patch_instructions(dst, &insn, len, true);
	mutex_unlock(&text_mutex);

	return ret;
}

void bpf_jit_free(struct bpf_prog *fp)
{
	if (fp->jited) {
		struct powerpc_jit_data *jit_data = fp->aux->jit_data;
		struct bpf_binary_header *hdr;

		/*
		 * If we fail the final pass of JIT (from jit_subprogs),
		 * the program may not be finalized yet. Call finalize here
		 * before freeing it.
		 */
		if (jit_data) {
			bpf_jit_binary_pack_finalize(fp, jit_data->fhdr, jit_data->hdr);
			kvfree(jit_data->addrs);
			kfree(jit_data);
		}
		hdr = bpf_jit_binary_pack_hdr(fp);
		bpf_jit_binary_pack_free(hdr, NULL);
		WARN_ON_ONCE(!bpf_prog_kallsyms_verify_off(fp));
	}

	bpf_prog_unlock_free(fp);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}
