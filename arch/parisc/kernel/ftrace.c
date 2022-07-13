<<<<<<< HEAD
/*
 * Code for tracing calls in Linux kernel.
 * Copyright (C) 2009 Helge Deller <deller@gmx.de>
=======
// SPDX-License-Identifier: GPL-2.0
/*
 * Code for tracing calls in Linux kernel.
 * Copyright (C) 2009-2016 Helge Deller <deller@gmx.de>
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 *
 * based on code for x86 which is:
 * Copyright (C) 2007-2008 Steven Rostedt <srostedt@redhat.com>
 *
 * future possible enhancements:
<<<<<<< HEAD
 * 	- add CONFIG_DYNAMIC_FTRACE
=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 *	- add CONFIG_STACK_TRACER
 */

#include <linux/init.h>
#include <linux/ftrace.h>
<<<<<<< HEAD

#include <asm/sections.h>
#include <asm/ftrace.h>



#ifdef CONFIG_FUNCTION_GRAPH_TRACER

/* Add a function return address to the trace stack on thread info.*/
static int push_return_trace(unsigned long ret, unsigned long long time,
				unsigned long func, int *depth)
{
	int index;

	if (!current->ret_stack)
		return -EBUSY;

	/* The return trace stack is full */
	if (current->curr_ret_stack == FTRACE_RETFUNC_DEPTH - 1) {
		atomic_inc(&current->trace_overrun);
		return -EBUSY;
	}

	index = ++current->curr_ret_stack;
	barrier();
	current->ret_stack[index].ret = ret;
	current->ret_stack[index].func = func;
	current->ret_stack[index].calltime = time;
	*depth = index;

	return 0;
}

/* Retrieve a function return address to the trace stack on thread info.*/
static void pop_return_trace(struct ftrace_graph_ret *trace, unsigned long *ret)
{
	int index;

	index = current->curr_ret_stack;

	if (unlikely(index < 0)) {
		ftrace_graph_stop();
		WARN_ON(1);
		/* Might as well panic, otherwise we have no where to go */
		*ret = (unsigned long)
			dereference_function_descriptor(&panic);
		return;
	}

	*ret = current->ret_stack[index].ret;
	trace->func = current->ret_stack[index].func;
	trace->calltime = current->ret_stack[index].calltime;
	trace->overrun = atomic_read(&current->trace_overrun);
	trace->depth = index;
	barrier();
	current->curr_ret_stack--;

}

/*
 * Send the trace to the ring-buffer.
 * @return the original return address.
 */
unsigned long ftrace_return_to_handler(unsigned long retval0,
				       unsigned long retval1)
{
	struct ftrace_graph_ret trace;
	unsigned long ret;

	pop_return_trace(&trace, &ret);
	trace.rettime = local_clock();
	ftrace_graph_return(&trace);

	if (unlikely(!ret)) {
		ftrace_graph_stop();
		WARN_ON(1);
		/* Might as well panic. What else to do? */
		ret = (unsigned long)
			dereference_function_descriptor(&panic);
	}

	/* HACK: we hand over the old functions' return values
	   in %r23 and %r24. Assembly in entry.S will take care
	   and move those to their final registers %ret0 and %ret1 */
	asm( "copy %0, %%r23 \n\t"
	     "copy %1, %%r24 \n" : : "r" (retval0), "r" (retval1) );

	return ret;
}
=======
#include <linux/uaccess.h>
#include <linux/kprobes.h>
#include <linux/ptrace.h>
#include <linux/jump_label.h>

#include <asm/assembly.h>
#include <asm/sections.h>
#include <asm/ftrace.h>
#include <asm/patch.h>

#define __hot __section(".text.hot")

#ifdef CONFIG_FUNCTION_GRAPH_TRACER
static DEFINE_STATIC_KEY_FALSE(ftrace_graph_enable);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

/*
 * Hook the return address and push it in the stack of return addrs
 * in current thread info.
 */
<<<<<<< HEAD
void prepare_ftrace_return(unsigned long *parent, unsigned long self_addr)
{
	unsigned long old;
	unsigned long long calltime;
	struct ftrace_graph_ent trace;
=======
static void __hot prepare_ftrace_return(unsigned long *parent,
					unsigned long self_addr)
{
	unsigned long old;
	extern int parisc_return_to_handler;

	if (unlikely(ftrace_graph_is_dead()))
		return;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	if (unlikely(atomic_read(&current->tracing_graph_pause)))
		return;

	old = *parent;
<<<<<<< HEAD
	*parent = (unsigned long)
		  dereference_function_descriptor(&return_to_handler);

	if (unlikely(!__kernel_text_address(old))) {
		ftrace_graph_stop();
		*parent = old;
		WARN_ON(1);
		return;
	}

	calltime = local_clock();

	if (push_return_trace(old, calltime,
				self_addr, &trace.depth) == -EBUSY) {
		*parent = old;
		return;
	}

	trace.func = self_addr;

	/* Only trace if the calling function expects to */
	if (!ftrace_graph_entry(&trace)) {
		current->curr_ret_stack--;
		*parent = old;
	}
}

#endif /* CONFIG_FUNCTION_GRAPH_TRACER */


void ftrace_function_trampoline(unsigned long parent,
				unsigned long self_addr,
				unsigned long org_sp_gr3)
{
	extern ftrace_func_t ftrace_trace_function;

	if (function_trace_stop)
		return;

	if (ftrace_trace_function != ftrace_stub) {
		ftrace_trace_function(parent, self_addr);
		return;
	}
#ifdef CONFIG_FUNCTION_GRAPH_TRACER
	if (ftrace_graph_entry && ftrace_graph_return) {
		unsigned long sp;
		unsigned long *parent_rp;

                asm volatile ("copy %%r30, %0" : "=r"(sp));
		/* sanity check: is stack pointer which we got from
		   assembler function in entry.S in a reasonable
		   range compared to current stack pointer? */
		if ((sp - org_sp_gr3) > 0x400)
			return;

		/* calculate pointer to %rp in stack */
		parent_rp = (unsigned long *) org_sp_gr3 - 0x10;
		/* sanity check: parent_rp should hold parent */
		if (*parent_rp != parent)
			return;
		
=======

	if (!function_graph_enter(old, self_addr, 0, NULL))
		/* activate parisc_return_to_handler() as return point */
		*parent = (unsigned long) &parisc_return_to_handler;
}
#endif /* CONFIG_FUNCTION_GRAPH_TRACER */

static ftrace_func_t ftrace_func;

asmlinkage void notrace __hot ftrace_function_trampoline(unsigned long parent,
				unsigned long self_addr,
				unsigned long org_sp_gr3,
				struct ftrace_regs *fregs)
{
	extern struct ftrace_ops *function_trace_op;

	ftrace_func(self_addr, parent, function_trace_op, fregs);

#ifdef CONFIG_FUNCTION_GRAPH_TRACER
	if (static_branch_unlikely(&ftrace_graph_enable)) {
		unsigned long *parent_rp;

		/* calculate pointer to %rp in stack */
		parent_rp = (unsigned long *) (org_sp_gr3 - RP_OFFSET);
		/* sanity check: parent_rp should hold parent */
		if (*parent_rp != parent)
			return;

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		prepare_ftrace_return(parent_rp, self_addr);
		return;
	}
#endif
}

<<<<<<< HEAD
=======
#if defined(CONFIG_DYNAMIC_FTRACE) && defined(CONFIG_FUNCTION_GRAPH_TRACER)
int ftrace_enable_ftrace_graph_caller(void)
{
	static_key_enable(&ftrace_graph_enable.key);
	return 0;
}

int ftrace_disable_ftrace_graph_caller(void)
{
	static_key_enable(&ftrace_graph_enable.key);
	return 0;
}
#endif

#ifdef CONFIG_DYNAMIC_FTRACE
int ftrace_update_ftrace_func(ftrace_func_t func)
{
	ftrace_func = func;
	return 0;
}

int ftrace_modify_call(struct dyn_ftrace *rec, unsigned long old_addr,
			unsigned long addr)
{
	return 0;
}

unsigned long ftrace_call_adjust(unsigned long addr)
{
	return addr+(FTRACE_PATCHABLE_FUNCTION_SIZE-1)*4;
}

int ftrace_make_call(struct dyn_ftrace *rec, unsigned long addr)
{
	u32 insn[FTRACE_PATCHABLE_FUNCTION_SIZE];
	u32 *tramp;
	int size, ret, i;
	void *ip;

#ifdef CONFIG_64BIT
	unsigned long addr2 =
		(unsigned long)dereference_function_descriptor((void *)addr);

	u32 ftrace_trampoline[] = {
		0x73c10208, /* std,ma r1,100(sp) */
		0x0c2110c1, /* ldd -10(r1),r1 */
		0xe820d002, /* bve,n (r1) */
		addr2 >> 32,
		addr2 & 0xffffffff,
		0xe83f1fd7, /* b,l,n .-14,r1 */
	};

	u32 ftrace_trampoline_unaligned[] = {
		addr2 >> 32,
		addr2 & 0xffffffff,
		0x37de0200, /* ldo 100(sp),sp */
		0x73c13e01, /* std r1,-100(sp) */
		0x34213ff9, /* ldo -4(r1),r1 */
		0x50213fc1, /* ldd -20(r1),r1 */
		0xe820d002, /* bve,n (r1) */
		0xe83f1fcf, /* b,l,n .-20,r1 */
	};

	BUILD_BUG_ON(ARRAY_SIZE(ftrace_trampoline_unaligned) >
				FTRACE_PATCHABLE_FUNCTION_SIZE);
#else
	u32 ftrace_trampoline[] = {
		(u32)addr,
		0x6fc10080, /* stw,ma r1,40(sp) */
		0x48213fd1, /* ldw -18(r1),r1 */
		0xe820c002, /* bv,n r0(r1) */
		0xe83f1fdf, /* b,l,n .-c,r1 */
	};
#endif

	BUILD_BUG_ON(ARRAY_SIZE(ftrace_trampoline) >
				FTRACE_PATCHABLE_FUNCTION_SIZE);

	size = sizeof(ftrace_trampoline);
	tramp = ftrace_trampoline;

#ifdef CONFIG_64BIT
	if (rec->ip & 0x4) {
		size = sizeof(ftrace_trampoline_unaligned);
		tramp = ftrace_trampoline_unaligned;
	}
#endif

	ip = (void *)(rec->ip + 4 - size);

	ret = copy_from_kernel_nofault(insn, ip, size);
	if (ret)
		return ret;

	for (i = 0; i < size / 4; i++) {
		if (insn[i] != INSN_NOP)
			return -EINVAL;
	}

	__patch_text_multiple(ip, tramp, size);
	return 0;
}

int ftrace_make_nop(struct module *mod, struct dyn_ftrace *rec,
		    unsigned long addr)
{
	u32 insn[FTRACE_PATCHABLE_FUNCTION_SIZE];
	int i;

	for (i = 0; i < ARRAY_SIZE(insn); i++)
		insn[i] = INSN_NOP;

	__patch_text((void *)rec->ip, INSN_NOP);
	__patch_text_multiple((void *)rec->ip + 4 - sizeof(insn),
			      insn, sizeof(insn)-4);
	return 0;
}
#endif

#ifdef CONFIG_KPROBES_ON_FTRACE
void kprobe_ftrace_handler(unsigned long ip, unsigned long parent_ip,
			   struct ftrace_ops *ops, struct ftrace_regs *fregs)
{
	struct kprobe_ctlblk *kcb;
	struct pt_regs *regs;
	struct kprobe *p;
	int bit;

	bit = ftrace_test_recursion_trylock(ip, parent_ip);
	if (bit < 0)
		return;

	regs = ftrace_get_regs(fregs);
	p = get_kprobe((kprobe_opcode_t *)ip);
	if (unlikely(!p) || kprobe_disabled(p))
		goto out;

	if (kprobe_running()) {
		kprobes_inc_nmissed_count(p);
		goto out;
	}

	__this_cpu_write(current_kprobe, p);

	kcb = get_kprobe_ctlblk();
	kcb->kprobe_status = KPROBE_HIT_ACTIVE;

	regs->iaoq[0] = ip;
	regs->iaoq[1] = ip + 4;

	if (!p->pre_handler || !p->pre_handler(p, regs)) {
		regs->iaoq[0] = ip + 4;
		regs->iaoq[1] = ip + 8;

		if (unlikely(p->post_handler)) {
			kcb->kprobe_status = KPROBE_HIT_SSDONE;
			p->post_handler(p, regs, 0);
		}
	}
	__this_cpu_write(current_kprobe, NULL);
out:
	ftrace_test_recursion_unlock(bit);
}
NOKPROBE_SYMBOL(kprobe_ftrace_handler);

int arch_prepare_kprobe_ftrace(struct kprobe *p)
{
	p->ainsn.insn = NULL;
	return 0;
}
#endif
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
