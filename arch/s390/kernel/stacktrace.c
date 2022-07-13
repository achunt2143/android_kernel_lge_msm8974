<<<<<<< HEAD
/*
 * arch/s390/kernel/stacktrace.c
 *
 * Stack trace management functions
 *
 *  Copyright (C) IBM Corp. 2006
 *  Author(s): Heiko Carstens <heiko.carstens@de.ibm.com>
 */

#include <linux/sched.h>
#include <linux/stacktrace.h>
#include <linux/kallsyms.h>
#include <linux/module.h>

static unsigned long save_context_stack(struct stack_trace *trace,
					unsigned long sp,
					unsigned long low,
					unsigned long high,
					int savesched)
{
	struct stack_frame *sf;
	struct pt_regs *regs;
	unsigned long addr;

	while(1) {
		sp &= PSW_ADDR_INSN;
		if (sp < low || sp > high)
			return sp;
		sf = (struct stack_frame *)sp;
		while(1) {
			addr = sf->gprs[8] & PSW_ADDR_INSN;
			if (!trace->skip)
				trace->entries[trace->nr_entries++] = addr;
			else
				trace->skip--;
			if (trace->nr_entries >= trace->max_entries)
				return sp;
			low = sp;
			sp = sf->back_chain & PSW_ADDR_INSN;
			if (!sp)
				break;
			if (sp <= low || sp > high - sizeof(*sf))
				return sp;
			sf = (struct stack_frame *)sp;
		}
		/* Zero backchain detected, check for interrupt frame. */
		sp = (unsigned long)(sf + 1);
		if (sp <= low || sp > high - sizeof(*regs))
			return sp;
		regs = (struct pt_regs *)sp;
		addr = regs->psw.addr & PSW_ADDR_INSN;
		if (savesched || !in_sched_functions(addr)) {
			if (!trace->skip)
				trace->entries[trace->nr_entries++] = addr;
			else
				trace->skip--;
		}
		if (trace->nr_entries >= trace->max_entries)
			return sp;
		low = sp;
		sp = regs->gprs[15];
	}
}

void save_stack_trace(struct stack_trace *trace)
{
	register unsigned long sp asm ("15");
	unsigned long orig_sp, new_sp;

	orig_sp = sp & PSW_ADDR_INSN;
	new_sp = save_context_stack(trace, orig_sp,
				    S390_lowcore.panic_stack - PAGE_SIZE,
				    S390_lowcore.panic_stack, 1);
	if (new_sp != orig_sp)
		return;
	new_sp = save_context_stack(trace, new_sp,
				    S390_lowcore.async_stack - ASYNC_SIZE,
				    S390_lowcore.async_stack, 1);
	if (new_sp != orig_sp)
		return;
	save_context_stack(trace, new_sp,
			   S390_lowcore.thread_info,
			   S390_lowcore.thread_info + THREAD_SIZE, 1);
}
EXPORT_SYMBOL_GPL(save_stack_trace);

void save_stack_trace_tsk(struct task_struct *tsk, struct stack_trace *trace)
{
	unsigned long sp, low, high;

	sp = tsk->thread.ksp & PSW_ADDR_INSN;
	low = (unsigned long) task_stack_page(tsk);
	high = (unsigned long) task_pt_regs(tsk);
	save_context_stack(trace, sp, low, high, 0);
	if (trace->nr_entries < trace->max_entries)
		trace->entries[trace->nr_entries++] = ULONG_MAX;
}
EXPORT_SYMBOL_GPL(save_stack_trace_tsk);
=======
// SPDX-License-Identifier: GPL-2.0
/*
 * Stack trace management functions
 *
 *  Copyright IBM Corp. 2006
 */

#include <linux/stacktrace.h>
#include <linux/uaccess.h>
#include <linux/compat.h>
#include <asm/stacktrace.h>
#include <asm/unwind.h>
#include <asm/kprobes.h>
#include <asm/ptrace.h>

void arch_stack_walk(stack_trace_consume_fn consume_entry, void *cookie,
		     struct task_struct *task, struct pt_regs *regs)
{
	struct unwind_state state;
	unsigned long addr;

	unwind_for_each_frame(&state, task, regs, 0) {
		addr = unwind_get_return_address(&state);
		if (!addr || !consume_entry(cookie, addr))
			break;
	}
}

int arch_stack_walk_reliable(stack_trace_consume_fn consume_entry,
			     void *cookie, struct task_struct *task)
{
	struct unwind_state state;
	unsigned long addr;

	unwind_for_each_frame(&state, task, NULL, 0) {
		if (state.stack_info.type != STACK_TYPE_TASK)
			return -EINVAL;

		if (state.regs)
			return -EINVAL;

		addr = unwind_get_return_address(&state);
		if (!addr)
			return -EINVAL;

#ifdef CONFIG_RETHOOK
		/*
		 * Mark stacktraces with krethook functions on them
		 * as unreliable.
		 */
		if (state.ip == (unsigned long)arch_rethook_trampoline)
			return -EINVAL;
#endif

		if (!consume_entry(cookie, addr))
			return -EINVAL;
	}

	/* Check for stack corruption */
	if (unwind_error(&state))
		return -EINVAL;
	return 0;
}

void arch_stack_walk_user(stack_trace_consume_fn consume_entry, void *cookie,
			  const struct pt_regs *regs)
{
	struct stack_frame_user __user *sf;
	unsigned long ip, sp;
	bool first = true;

	if (is_compat_task())
		return;
	if (!consume_entry(cookie, instruction_pointer(regs)))
		return;
	sf = (void __user *)user_stack_pointer(regs);
	pagefault_disable();
	while (1) {
		if (__get_user(sp, &sf->back_chain))
			break;
		if (__get_user(ip, &sf->gprs[8]))
			break;
		if (ip & 0x1) {
			/*
			 * If the instruction address is invalid, and this
			 * is the first stack frame, assume r14 has not
			 * been written to the stack yet. Otherwise exit.
			 */
			if (first && !(regs->gprs[14] & 0x1))
				ip = regs->gprs[14];
			else
				break;
		}
		if (!consume_entry(cookie, ip))
			break;
		/* Sanity check: ABI requires SP to be aligned 8 bytes. */
		if (!sp || sp & 0x7)
			break;
		sf = (void __user *)sp;
		first = false;
	}
	pagefault_enable();
}
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
