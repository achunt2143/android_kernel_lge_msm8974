<<<<<<< HEAD
/*
 *  Kernel Probes (KProbes)
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
 *
=======
// SPDX-License-Identifier: GPL-2.0-or-later
/*
 *  Kernel Probes (KProbes)
 *
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 * Copyright (C) IBM Corporation, 2002, 2004
 *
 * 2002-Oct	Created by Vamsi Krishna S <vamsi_krishna@in.ibm.com> Kernel
 *		Probes initial implementation ( includes contributions from
 *		Rusty Russell).
 * 2004-July	Suparna Bhattacharya <suparna@in.ibm.com> added jumper probes
 *		interface to access function arguments.
 * 2004-Nov	Ananth N Mavinakayanahalli <ananth@in.ibm.com> kprobes port
 *		for PPC64
 */

#include <linux/kprobes.h>
#include <linux/ptrace.h>
#include <linux/preempt.h>
<<<<<<< HEAD
#include <linux/module.h>
#include <linux/kdebug.h>
#include <linux/slab.h>
#include <asm/cacheflush.h>
#include <asm/sstep.h>
#include <asm/uaccess.h>

#ifdef CONFIG_PPC_ADV_DEBUG_REGS
#define MSR_SINGLESTEP	(MSR_DE)
#else
#define MSR_SINGLESTEP	(MSR_SE)
#endif
=======
#include <linux/extable.h>
#include <linux/kdebug.h>
#include <linux/slab.h>
#include <linux/moduleloader.h>
#include <linux/set_memory.h>
#include <asm/code-patching.h>
#include <asm/cacheflush.h>
#include <asm/sstep.h>
#include <asm/sections.h>
#include <asm/inst.h>
#include <linux/uaccess.h>
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

DEFINE_PER_CPU(struct kprobe *, current_kprobe) = NULL;
DEFINE_PER_CPU(struct kprobe_ctlblk, kprobe_ctlblk);

struct kretprobe_blackpoint kretprobe_blacklist[] = {{NULL, NULL}};

<<<<<<< HEAD
int __kprobes arch_prepare_kprobe(struct kprobe *p)
{
	int ret = 0;
	kprobe_opcode_t insn = *p->addr;
=======
bool arch_within_kprobe_blacklist(unsigned long addr)
{
	return  (addr >= (unsigned long)__kprobes_text_start &&
		 addr < (unsigned long)__kprobes_text_end) ||
		(addr >= (unsigned long)_stext &&
		 addr < (unsigned long)__head_end);
}

kprobe_opcode_t *kprobe_lookup_name(const char *name, unsigned int offset)
{
	kprobe_opcode_t *addr = NULL;

#ifdef CONFIG_PPC64_ELF_ABI_V2
	/* PPC64 ABIv2 needs local entry point */
	addr = (kprobe_opcode_t *)kallsyms_lookup_name(name);
	if (addr && !offset) {
#ifdef CONFIG_KPROBES_ON_FTRACE
		unsigned long faddr;
		/*
		 * Per livepatch.h, ftrace location is always within the first
		 * 16 bytes of a function on powerpc with -mprofile-kernel.
		 */
		faddr = ftrace_location_range((unsigned long)addr,
					      (unsigned long)addr + 16);
		if (faddr)
			addr = (kprobe_opcode_t *)faddr;
		else
#endif
			addr = (kprobe_opcode_t *)ppc_function_entry(addr);
	}
#elif defined(CONFIG_PPC64_ELF_ABI_V1)
	/*
	 * 64bit powerpc ABIv1 uses function descriptors:
	 * - Check for the dot variant of the symbol first.
	 * - If that fails, try looking up the symbol provided.
	 *
	 * This ensures we always get to the actual symbol and not
	 * the descriptor.
	 *
	 * Also handle <module:symbol> format.
	 */
	char dot_name[MODULE_NAME_LEN + 1 + KSYM_NAME_LEN];
	bool dot_appended = false;
	const char *c;
	ssize_t ret = 0;
	int len = 0;

	if ((c = strnchr(name, MODULE_NAME_LEN, ':')) != NULL) {
		c++;
		len = c - name;
		memcpy(dot_name, name, len);
	} else
		c = name;

	if (*c != '\0' && *c != '.') {
		dot_name[len++] = '.';
		dot_appended = true;
	}
	ret = strscpy(dot_name + len, c, KSYM_NAME_LEN);
	if (ret > 0)
		addr = (kprobe_opcode_t *)kallsyms_lookup_name(dot_name);

	/* Fallback to the original non-dot symbol lookup */
	if (!addr && dot_appended)
		addr = (kprobe_opcode_t *)kallsyms_lookup_name(name);
#else
	addr = (kprobe_opcode_t *)kallsyms_lookup_name(name);
#endif

	return addr;
}

static bool arch_kprobe_on_func_entry(unsigned long offset)
{
#ifdef CONFIG_PPC64_ELF_ABI_V2
#ifdef CONFIG_KPROBES_ON_FTRACE
	return offset <= 16;
#else
	return offset <= 8;
#endif
#else
	return !offset;
#endif
}

/* XXX try and fold the magic of kprobe_lookup_name() in this */
kprobe_opcode_t *arch_adjust_kprobe_addr(unsigned long addr, unsigned long offset,
					 bool *on_func_entry)
{
	*on_func_entry = arch_kprobe_on_func_entry(offset);
	return (kprobe_opcode_t *)(addr + offset);
}

void *alloc_insn_page(void)
{
	void *page;

	page = module_alloc(PAGE_SIZE);
	if (!page)
		return NULL;

	if (strict_module_rwx_enabled()) {
		int err = set_memory_rox((unsigned long)page, 1);

		if (err)
			goto error;
	}
	return page;
error:
	module_memfree(page);
	return NULL;
}

int arch_prepare_kprobe(struct kprobe *p)
{
	int ret = 0;
	struct kprobe *prev;
	ppc_inst_t insn = ppc_inst_read(p->addr);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	if ((unsigned long)p->addr & 0x03) {
		printk("Attempt to register kprobe at an unaligned address\n");
		ret = -EINVAL;
<<<<<<< HEAD
	} else if (IS_MTMSRD(insn) || IS_RFID(insn) || IS_RFI(insn)) {
		printk("Cannot register a kprobe on rfi/rfid or mtmsr[d]\n");
=======
	} else if (!can_single_step(ppc_inst_val(insn))) {
		printk("Cannot register a kprobe on instructions that can't be single stepped\n");
		ret = -EINVAL;
	} else if ((unsigned long)p->addr & ~PAGE_MASK &&
		   ppc_inst_prefixed(ppc_inst_read(p->addr - 1))) {
		printk("Cannot register a kprobe on the second word of prefixed instruction\n");
		ret = -EINVAL;
	}
	prev = get_kprobe(p->addr - 1);

	/*
	 * When prev is a ftrace-based kprobe, we don't have an insn, and it
	 * doesn't probe for prefixed instruction.
	 */
	if (prev && !kprobe_ftrace(prev) &&
	    ppc_inst_prefixed(ppc_inst_read(prev->ainsn.insn))) {
		printk("Cannot register a kprobe on the second word of prefixed instruction\n");
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		ret = -EINVAL;
	}

	/* insn must be on a special executable page on ppc64.  This is
	 * not explicitly required on ppc32 (right now), but it doesn't hurt */
	if (!ret) {
		p->ainsn.insn = get_insn_slot();
		if (!p->ainsn.insn)
			ret = -ENOMEM;
	}

	if (!ret) {
<<<<<<< HEAD
		memcpy(p->ainsn.insn, p->addr,
				MAX_INSN_SIZE * sizeof(kprobe_opcode_t));
		p->opcode = *p->addr;
		flush_icache_range((unsigned long)p->ainsn.insn,
			(unsigned long)p->ainsn.insn + sizeof(kprobe_opcode_t));
=======
		patch_instruction(p->ainsn.insn, insn);
		p->opcode = ppc_inst_val(insn);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	}

	p->ainsn.boostable = 0;
	return ret;
}
<<<<<<< HEAD

void __kprobes arch_arm_kprobe(struct kprobe *p)
{
	*p->addr = BREAKPOINT_INSTRUCTION;
	flush_icache_range((unsigned long) p->addr,
			   (unsigned long) p->addr + sizeof(kprobe_opcode_t));
}

void __kprobes arch_disarm_kprobe(struct kprobe *p)
{
	*p->addr = p->opcode;
	flush_icache_range((unsigned long) p->addr,
			   (unsigned long) p->addr + sizeof(kprobe_opcode_t));
}

void __kprobes arch_remove_kprobe(struct kprobe *p)
=======
NOKPROBE_SYMBOL(arch_prepare_kprobe);

void arch_arm_kprobe(struct kprobe *p)
{
	WARN_ON_ONCE(patch_instruction(p->addr, ppc_inst(BREAKPOINT_INSTRUCTION)));
}
NOKPROBE_SYMBOL(arch_arm_kprobe);

void arch_disarm_kprobe(struct kprobe *p)
{
	WARN_ON_ONCE(patch_instruction(p->addr, ppc_inst(p->opcode)));
}
NOKPROBE_SYMBOL(arch_disarm_kprobe);

void arch_remove_kprobe(struct kprobe *p)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
{
	if (p->ainsn.insn) {
		free_insn_slot(p->ainsn.insn, 0);
		p->ainsn.insn = NULL;
	}
}
<<<<<<< HEAD

static void __kprobes prepare_singlestep(struct kprobe *p, struct pt_regs *regs)
{
	/* We turn off async exceptions to ensure that the single step will
	 * be for the instruction we have the kprobe on, if we dont its
	 * possible we'd get the single step reported for an exception handler
	 * like Decrementer or External Interrupt */
	regs->msr &= ~MSR_EE;
	regs->msr |= MSR_SINGLESTEP;
#ifdef CONFIG_PPC_ADV_DEBUG_REGS
	regs->msr &= ~MSR_CE;
	mtspr(SPRN_DBCR0, mfspr(SPRN_DBCR0) | DBCR0_IC | DBCR0_IDM);
#ifdef CONFIG_PPC_47x
	isync();
#endif
#endif
=======
NOKPROBE_SYMBOL(arch_remove_kprobe);

static nokprobe_inline void prepare_singlestep(struct kprobe *p, struct pt_regs *regs)
{
	enable_single_step(regs);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	/*
	 * On powerpc we should single step on the original
	 * instruction even if the probed insn is a trap
	 * variant as values in regs could play a part in
	 * if the trap is taken or not
	 */
<<<<<<< HEAD
	regs->nip = (unsigned long)p->ainsn.insn;
}

static void __kprobes save_previous_kprobe(struct kprobe_ctlblk *kcb)
=======
	regs_set_return_ip(regs, (unsigned long)p->ainsn.insn);
}

static nokprobe_inline void save_previous_kprobe(struct kprobe_ctlblk *kcb)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
{
	kcb->prev_kprobe.kp = kprobe_running();
	kcb->prev_kprobe.status = kcb->kprobe_status;
	kcb->prev_kprobe.saved_msr = kcb->kprobe_saved_msr;
}

<<<<<<< HEAD
static void __kprobes restore_previous_kprobe(struct kprobe_ctlblk *kcb)
{
	__get_cpu_var(current_kprobe) = kcb->prev_kprobe.kp;
=======
static nokprobe_inline void restore_previous_kprobe(struct kprobe_ctlblk *kcb)
{
	__this_cpu_write(current_kprobe, kcb->prev_kprobe.kp);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	kcb->kprobe_status = kcb->prev_kprobe.status;
	kcb->kprobe_saved_msr = kcb->prev_kprobe.saved_msr;
}

<<<<<<< HEAD
static void __kprobes set_current_kprobe(struct kprobe *p, struct pt_regs *regs,
				struct kprobe_ctlblk *kcb)
{
	__get_cpu_var(current_kprobe) = p;
	kcb->kprobe_saved_msr = regs->msr;
}

void __kprobes arch_prepare_kretprobe(struct kretprobe_instance *ri,
				      struct pt_regs *regs)
{
	ri->ret_addr = (kprobe_opcode_t *)regs->link;

	/* Replace the return addr with trampoline addr */
	regs->link = (unsigned long)kretprobe_trampoline;
}

static int __kprobes kprobe_handler(struct pt_regs *regs)
=======
static nokprobe_inline void set_current_kprobe(struct kprobe *p, struct pt_regs *regs,
				struct kprobe_ctlblk *kcb)
{
	__this_cpu_write(current_kprobe, p);
	kcb->kprobe_saved_msr = regs->msr;
}

void arch_prepare_kretprobe(struct kretprobe_instance *ri, struct pt_regs *regs)
{
	ri->ret_addr = (kprobe_opcode_t *)regs->link;
	ri->fp = NULL;

	/* Replace the return addr with trampoline addr */
	regs->link = (unsigned long)__kretprobe_trampoline;
}
NOKPROBE_SYMBOL(arch_prepare_kretprobe);

static int try_to_emulate(struct kprobe *p, struct pt_regs *regs)
{
	int ret;
	ppc_inst_t insn = ppc_inst_read(p->ainsn.insn);

	/* regs->nip is also adjusted if emulate_step returns 1 */
	ret = emulate_step(regs, insn);
	if (ret > 0) {
		/*
		 * Once this instruction has been boosted
		 * successfully, set the boostable flag
		 */
		if (unlikely(p->ainsn.boostable == 0))
			p->ainsn.boostable = 1;
	} else if (ret < 0) {
		/*
		 * We don't allow kprobes on mtmsr(d)/rfi(d), etc.
		 * So, we should never get here... but, its still
		 * good to catch them, just in case...
		 */
		printk("Can't step on instruction %08lx\n", ppc_inst_as_ulong(insn));
		BUG();
	} else {
		/*
		 * If we haven't previously emulated this instruction, then it
		 * can't be boosted. Note it down so we don't try to do so again.
		 *
		 * If, however, we had emulated this instruction in the past,
		 * then this is just an error with the current run (for
		 * instance, exceptions due to a load/store). We return 0 so
		 * that this is now single-stepped, but continue to try
		 * emulating it in subsequent probe hits.
		 */
		if (unlikely(p->ainsn.boostable != 1))
			p->ainsn.boostable = -1;
	}

	return ret;
}
NOKPROBE_SYMBOL(try_to_emulate);

int kprobe_handler(struct pt_regs *regs)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
{
	struct kprobe *p;
	int ret = 0;
	unsigned int *addr = (unsigned int *)regs->nip;
	struct kprobe_ctlblk *kcb;

<<<<<<< HEAD
=======
	if (user_mode(regs))
		return 0;

	if (!IS_ENABLED(CONFIG_BOOKE) &&
	    (!(regs->msr & MSR_IR) || !(regs->msr & MSR_DR)))
		return 0;

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	/*
	 * We don't want to be preempted for the entire
	 * duration of kprobe processing
	 */
	preempt_disable();
	kcb = get_kprobe_ctlblk();

<<<<<<< HEAD
	/* Check we're not actually recursing */
	if (kprobe_running()) {
		p = get_kprobe(addr);
		if (p) {
			kprobe_opcode_t insn = *p->ainsn.insn;
			if (kcb->kprobe_status == KPROBE_HIT_SS &&
					is_trap(insn)) {
				/* Turn off 'trace' bits */
				regs->msr &= ~MSR_SINGLESTEP;
				regs->msr |= kcb->kprobe_saved_msr;
				goto no_kprobe;
			}
			/* We have reentered the kprobe_handler(), since
			 * another probe was hit while within the handler.
			 * We here save the original kprobes variables and
			 * just single step on the instruction of the new probe
			 * without calling any user handlers.
			 */
			save_previous_kprobe(kcb);
			set_current_kprobe(p, regs, kcb);
			kcb->kprobe_saved_msr = regs->msr;
			kprobes_inc_nmissed_count(p);
			prepare_singlestep(p, regs);
			kcb->kprobe_status = KPROBE_REENTER;
			return 1;
		} else {
			if (*addr != BREAKPOINT_INSTRUCTION) {
				/* If trap variant, then it belongs not to us */
				kprobe_opcode_t cur_insn = *addr;
				if (is_trap(cur_insn))
		       			goto no_kprobe;
				/* The breakpoint instruction was removed by
				 * another cpu right after we hit, no further
				 * handling of this interrupt is appropriate
				 */
				ret = 1;
				goto no_kprobe;
			}
			p = __get_cpu_var(current_kprobe);
			if (p->break_handler && p->break_handler(p, regs)) {
				goto ss_probe;
			}
		}
		goto no_kprobe;
	}

	p = get_kprobe(addr);
	if (!p) {
		if (*addr != BREAKPOINT_INSTRUCTION) {
=======
	p = get_kprobe(addr);
	if (!p) {
		unsigned int instr;

		if (get_kernel_nofault(instr, addr))
			goto no_kprobe;

		if (instr != BREAKPOINT_INSTRUCTION) {
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
			/*
			 * PowerPC has multiple variants of the "trap"
			 * instruction. If the current instruction is a
			 * trap variant, it could belong to someone else
			 */
<<<<<<< HEAD
			kprobe_opcode_t cur_insn = *addr;
			if (is_trap(cur_insn))
		       		goto no_kprobe;
=======
			if (is_trap(instr))
				goto no_kprobe;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
			/*
			 * The breakpoint instruction was removed right
			 * after we hit it.  Another cpu has removed
			 * either a probepoint or a debugger breakpoint
			 * at this address.  In either case, no further
			 * handling of this interrupt is appropriate.
			 */
			ret = 1;
		}
		/* Not one of ours: let kernel handle it */
		goto no_kprobe;
	}

<<<<<<< HEAD
	kcb->kprobe_status = KPROBE_HIT_ACTIVE;
	set_current_kprobe(p, regs, kcb);
	if (p->pre_handler && p->pre_handler(p, regs))
		/* handler has already set things up, so skip ss setup */
		return 1;

ss_probe:
	if (p->ainsn.boostable >= 0) {
		unsigned int insn = *p->ainsn.insn;

		/* regs->nip is also adjusted if emulate_step returns 1 */
		ret = emulate_step(regs, insn);
		if (ret > 0) {
			/*
			 * Once this instruction has been boosted
			 * successfully, set the boostable flag
			 */
			if (unlikely(p->ainsn.boostable == 0))
				p->ainsn.boostable = 1;

=======
	/* Check we're not actually recursing */
	if (kprobe_running()) {
		kprobe_opcode_t insn = *p->ainsn.insn;
		if (kcb->kprobe_status == KPROBE_HIT_SS && is_trap(insn)) {
			/* Turn off 'trace' bits */
			regs_set_return_msr(regs,
				(regs->msr & ~MSR_SINGLESTEP) |
				kcb->kprobe_saved_msr);
			goto no_kprobe;
		}

		/*
		 * We have reentered the kprobe_handler(), since another probe
		 * was hit while within the handler. We here save the original
		 * kprobes variables and just single step on the instruction of
		 * the new probe without calling any user handlers.
		 */
		save_previous_kprobe(kcb);
		set_current_kprobe(p, regs, kcb);
		kprobes_inc_nmissed_count(p);
		kcb->kprobe_status = KPROBE_REENTER;
		if (p->ainsn.boostable >= 0) {
			ret = try_to_emulate(p, regs);

			if (ret > 0) {
				restore_previous_kprobe(kcb);
				preempt_enable();
				return 1;
			}
		}
		prepare_singlestep(p, regs);
		return 1;
	}

	kcb->kprobe_status = KPROBE_HIT_ACTIVE;
	set_current_kprobe(p, regs, kcb);
	if (p->pre_handler && p->pre_handler(p, regs)) {
		/* handler changed execution path, so skip ss setup */
		reset_current_kprobe();
		preempt_enable();
		return 1;
	}

	if (p->ainsn.boostable >= 0) {
		ret = try_to_emulate(p, regs);

		if (ret > 0) {
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
			if (p->post_handler)
				p->post_handler(p, regs, 0);

			kcb->kprobe_status = KPROBE_HIT_SSDONE;
			reset_current_kprobe();
<<<<<<< HEAD
			preempt_enable_no_resched();
			return 1;
		} else if (ret < 0) {
			/*
			 * We don't allow kprobes on mtmsr(d)/rfi(d), etc.
			 * So, we should never get here... but, its still
			 * good to catch them, just in case...
			 */
			printk("Can't step on instruction %x\n", insn);
			BUG();
		} else if (ret == 0)
			/* This instruction can't be boosted */
			p->ainsn.boostable = -1;
=======
			preempt_enable();
			return 1;
		}
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	}
	prepare_singlestep(p, regs);
	kcb->kprobe_status = KPROBE_HIT_SS;
	return 1;

no_kprobe:
<<<<<<< HEAD
	preempt_enable_no_resched();
	return ret;
}
=======
	preempt_enable();
	return ret;
}
NOKPROBE_SYMBOL(kprobe_handler);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

/*
 * Function return probe trampoline:
 * 	- init_kprobes() establishes a probepoint here
 * 	- When the probed function returns, this probe
 * 		causes the handlers to fire
 */
<<<<<<< HEAD
static void __used kretprobe_trampoline_holder(void)
{
	asm volatile(".global kretprobe_trampoline\n"
			"kretprobe_trampoline:\n"
			"nop\n");
}
=======
asm(".global __kretprobe_trampoline\n"
	".type __kretprobe_trampoline, @function\n"
	"__kretprobe_trampoline:\n"
	"nop\n"
	"blr\n"
	".size __kretprobe_trampoline, .-__kretprobe_trampoline\n");
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

/*
 * Called when the probe at kretprobe trampoline is hit
 */
<<<<<<< HEAD
static int __kprobes trampoline_probe_handler(struct kprobe *p,
						struct pt_regs *regs)
{
	struct kretprobe_instance *ri = NULL;
	struct hlist_head *head, empty_rp;
	struct hlist_node *node, *tmp;
	unsigned long flags, orig_ret_address = 0;
	unsigned long trampoline_address =(unsigned long)&kretprobe_trampoline;

	INIT_HLIST_HEAD(&empty_rp);
	kretprobe_hash_lock(current, &head, &flags);

	/*
	 * It is possible to have multiple instances associated with a given
	 * task either because an multiple functions in the call path
	 * have a return probe installed on them, and/or more than one return
	 * return probe was registered for a target function.
	 *
	 * We can handle this because:
	 *     - instances are always inserted at the head of the list
	 *     - when multiple return probes are registered for the same
	 *       function, the first instance's ret_addr will point to the
	 *       real return address, and all the rest will point to
	 *       kretprobe_trampoline
	 */
	hlist_for_each_entry_safe(ri, node, tmp, head, hlist) {
		if (ri->task != current)
			/* another task is sharing our hash bucket */
			continue;

		if (ri->rp && ri->rp->handler)
			ri->rp->handler(ri, regs);

		orig_ret_address = (unsigned long)ri->ret_addr;
		recycle_rp_inst(ri, &empty_rp);

		if (orig_ret_address != trampoline_address)
			/*
			 * This is the real return address. Any other
			 * instances associated with this task are for
			 * other calls deeper on the call stack
			 */
			break;
	}

	kretprobe_assert(ri, orig_ret_address, trampoline_address);
	regs->nip = orig_ret_address;

	reset_current_kprobe();
	kretprobe_hash_unlock(current, &flags);
	preempt_enable_no_resched();

	hlist_for_each_entry_safe(ri, node, tmp, &empty_rp, hlist) {
		hlist_del(&ri->hlist);
		kfree(ri);
	}
	/*
	 * By returning a non-zero value, we are telling
	 * kprobe_handler() that we don't want the post_handler
	 * to run (and have re-enabled preemption)
	 */
	return 1;
}
=======
static int trampoline_probe_handler(struct kprobe *p, struct pt_regs *regs)
{
	unsigned long orig_ret_address;

	orig_ret_address = __kretprobe_trampoline_handler(regs, NULL);
	/*
	 * We get here through one of two paths:
	 * 1. by taking a trap -> kprobe_handler() -> here
	 * 2. by optprobe branch -> optimized_callback() -> opt_pre_handler() -> here
	 *
	 * When going back through (1), we need regs->nip to be setup properly
	 * as it is used to determine the return address from the trap.
	 * For (2), since nip is not honoured with optprobes, we instead setup
	 * the link register properly so that the subsequent 'blr' in
	 * __kretprobe_trampoline jumps back to the right instruction.
	 *
	 * For nip, we should set the address to the previous instruction since
	 * we end up emulating it in kprobe_handler(), which increments the nip
	 * again.
	 */
	regs_set_return_ip(regs, orig_ret_address - 4);
	regs->link = orig_ret_address;

	return 0;
}
NOKPROBE_SYMBOL(trampoline_probe_handler);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

/*
 * Called after single-stepping.  p->addr is the address of the
 * instruction whose first byte has been replaced by the "breakpoint"
 * instruction.  To avoid the SMP problems that can occur when we
 * temporarily put back the original opcode to single-step, we
 * single-stepped a copy of the instruction.  The address of this
 * copy is p->ainsn.insn.
 */
<<<<<<< HEAD
static int __kprobes post_kprobe_handler(struct pt_regs *regs)
{
	struct kprobe *cur = kprobe_running();
	struct kprobe_ctlblk *kcb = get_kprobe_ctlblk();

	if (!cur)
		return 0;

	/* make sure we got here for instruction we have a kprobe on */
	if (((unsigned long)cur->ainsn.insn + 4) != regs->nip)
=======
int kprobe_post_handler(struct pt_regs *regs)
{
	int len;
	struct kprobe *cur = kprobe_running();
	struct kprobe_ctlblk *kcb = get_kprobe_ctlblk();

	if (!cur || user_mode(regs))
		return 0;

	len = ppc_inst_len(ppc_inst_read(cur->ainsn.insn));
	/* make sure we got here for instruction we have a kprobe on */
	if (((unsigned long)cur->ainsn.insn + len) != regs->nip)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		return 0;

	if ((kcb->kprobe_status != KPROBE_REENTER) && cur->post_handler) {
		kcb->kprobe_status = KPROBE_HIT_SSDONE;
		cur->post_handler(cur, regs, 0);
	}

	/* Adjust nip to after the single-stepped instruction */
<<<<<<< HEAD
	regs->nip = (unsigned long)cur->addr + 4;
	regs->msr |= kcb->kprobe_saved_msr;
=======
	regs_set_return_ip(regs, (unsigned long)cur->addr + len);
	regs_set_return_msr(regs, regs->msr | kcb->kprobe_saved_msr);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	/*Restore back the original saved kprobes variables and continue. */
	if (kcb->kprobe_status == KPROBE_REENTER) {
		restore_previous_kprobe(kcb);
		goto out;
	}
	reset_current_kprobe();
out:
<<<<<<< HEAD
	preempt_enable_no_resched();
=======
	preempt_enable();
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	/*
	 * if somebody else is singlestepping across a probe point, msr
	 * will have DE/SE set, in which case, continue the remaining processing
	 * of do_debug, as if this is not a probe hit.
	 */
	if (regs->msr & MSR_SINGLESTEP)
		return 0;

	return 1;
}
<<<<<<< HEAD

int __kprobes kprobe_fault_handler(struct pt_regs *regs, int trapnr)
=======
NOKPROBE_SYMBOL(kprobe_post_handler);

int kprobe_fault_handler(struct pt_regs *regs, int trapnr)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
{
	struct kprobe *cur = kprobe_running();
	struct kprobe_ctlblk *kcb = get_kprobe_ctlblk();
	const struct exception_table_entry *entry;

	switch(kcb->kprobe_status) {
	case KPROBE_HIT_SS:
	case KPROBE_REENTER:
		/*
		 * We are here because the instruction being single
		 * stepped caused a page fault. We reset the current
		 * kprobe and the nip points back to the probe address
		 * and allow the page fault handler to continue as a
		 * normal page fault.
		 */
<<<<<<< HEAD
		regs->nip = (unsigned long)cur->addr;
		regs->msr &= ~MSR_SINGLESTEP; /* Turn off 'trace' bits */
		regs->msr |= kcb->kprobe_saved_msr;
=======
		regs_set_return_ip(regs, (unsigned long)cur->addr);
		/* Turn off 'trace' bits */
		regs_set_return_msr(regs,
			(regs->msr & ~MSR_SINGLESTEP) |
			kcb->kprobe_saved_msr);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		if (kcb->kprobe_status == KPROBE_REENTER)
			restore_previous_kprobe(kcb);
		else
			reset_current_kprobe();
<<<<<<< HEAD
		preempt_enable_no_resched();
=======
		preempt_enable();
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		break;
	case KPROBE_HIT_ACTIVE:
	case KPROBE_HIT_SSDONE:
		/*
<<<<<<< HEAD
		 * We increment the nmissed count for accounting,
		 * we can also use npre/npostfault count for accouting
		 * these specific fault cases.
		 */
		kprobes_inc_nmissed_count(cur);

		/*
		 * We come here because instructions in the pre/post
		 * handler caused the page_fault, this could happen
		 * if handler tries to access user space by
		 * copy_from_user(), get_user() etc. Let the
		 * user-specified handler try to fix it first.
		 */
		if (cur->fault_handler && cur->fault_handler(cur, regs, trapnr))
			return 1;

		/*
=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		 * In case the user-specified fault handler returned
		 * zero, try to fix up.
		 */
		if ((entry = search_exception_tables(regs->nip)) != NULL) {
<<<<<<< HEAD
			regs->nip = entry->fixup;
=======
			regs_set_return_ip(regs, extable_fixup(entry));
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
			return 1;
		}

		/*
		 * fixup_exception() could not handle it,
		 * Let do_page_fault() fix it.
		 */
		break;
	default:
		break;
	}
	return 0;
}
<<<<<<< HEAD

/*
 * Wrapper routine to for handling exceptions.
 */
int __kprobes kprobe_exceptions_notify(struct notifier_block *self,
				       unsigned long val, void *data)
{
	struct die_args *args = (struct die_args *)data;
	int ret = NOTIFY_DONE;

	if (args->regs && user_mode(args->regs))
		return ret;

	switch (val) {
	case DIE_BPT:
		if (kprobe_handler(args->regs))
			ret = NOTIFY_STOP;
		break;
	case DIE_SSTEP:
		if (post_kprobe_handler(args->regs))
			ret = NOTIFY_STOP;
		break;
	default:
		break;
	}
	return ret;
}

#ifdef CONFIG_PPC64
unsigned long arch_deref_entry_point(void *entry)
{
	return ((func_descr_t *)entry)->entry;
}
#endif

int __kprobes setjmp_pre_handler(struct kprobe *p, struct pt_regs *regs)
{
	struct jprobe *jp = container_of(p, struct jprobe, kp);
	struct kprobe_ctlblk *kcb = get_kprobe_ctlblk();

	memcpy(&kcb->jprobe_saved_regs, regs, sizeof(struct pt_regs));

	/* setup return addr to the jprobe handler routine */
	regs->nip = arch_deref_entry_point(jp->entry);
#ifdef CONFIG_PPC64
	regs->gpr[2] = (unsigned long)(((func_descr_t *)jp->entry)->toc);
#endif

	return 1;
}

void __used __kprobes jprobe_return(void)
{
	asm volatile("trap" ::: "memory");
}

static void __used __kprobes jprobe_return_end(void)
{
};

int __kprobes longjmp_break_handler(struct kprobe *p, struct pt_regs *regs)
{
	struct kprobe_ctlblk *kcb = get_kprobe_ctlblk();

	/*
	 * FIXME - we should ideally be validating that we got here 'cos
	 * of the "trap" in jprobe_return() above, before restoring the
	 * saved regs...
	 */
	memcpy(regs, &kcb->jprobe_saved_regs, sizeof(struct pt_regs));
	preempt_enable_no_resched();
	return 1;
}

static struct kprobe trampoline_p = {
	.addr = (kprobe_opcode_t *) &kretprobe_trampoline,
=======
NOKPROBE_SYMBOL(kprobe_fault_handler);

static struct kprobe trampoline_p = {
	.addr = (kprobe_opcode_t *) &__kretprobe_trampoline,
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	.pre_handler = trampoline_probe_handler
};

int __init arch_init_kprobes(void)
{
	return register_kprobe(&trampoline_p);
}

<<<<<<< HEAD
int __kprobes arch_trampoline_kprobe(struct kprobe *p)
{
	if (p->addr == (kprobe_opcode_t *)&kretprobe_trampoline)
=======
int arch_trampoline_kprobe(struct kprobe *p)
{
	if (p->addr == (kprobe_opcode_t *)&__kretprobe_trampoline)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		return 1;

	return 0;
}
<<<<<<< HEAD
=======
NOKPROBE_SYMBOL(arch_trampoline_kprobe);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
