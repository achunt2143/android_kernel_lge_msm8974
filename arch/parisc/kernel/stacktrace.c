<<<<<<< HEAD
/*
 * Stack trace management functions
 *
 *  Copyright (C) 2009 Helge Deller <deller@gmx.de>
=======
// SPDX-License-Identifier: GPL-2.0-only
/*
 * Stack trace management functions
 *
 *  Copyright (C) 2009-2021 Helge Deller <deller@gmx.de>
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 *  based on arch/x86/kernel/stacktrace.c by Ingo Molnar <mingo@redhat.com>
 *  and parisc unwind functions by Randolph Chung <tausq@debian.org>
 *
 *  TODO: Userspace stacktrace (CONFIG_USER_STACKTRACE_SUPPORT)
 */
<<<<<<< HEAD
#include <linux/module.h>
=======
#include <linux/kernel.h>
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#include <linux/stacktrace.h>

#include <asm/unwind.h>

<<<<<<< HEAD
static void dump_trace(struct task_struct *task, struct stack_trace *trace)
{
	struct unwind_frame_info info;

	/* initialize unwind info */
	if (task == current) {
		unsigned long sp;
		struct pt_regs r;
HERE:
		asm volatile ("copy %%r30, %0" : "=r"(sp));
		memset(&r, 0, sizeof(struct pt_regs));
		r.iaoq[0] = (unsigned long)&&HERE;
		r.gr[2] = (unsigned long)__builtin_return_address(0);
		r.gr[30] = sp;
		unwind_frame_init(&info, task, &r);
	} else {
		unwind_frame_init_from_blocked_task(&info, task);
	}

	/* unwind stack and save entries in stack_trace struct */
	trace->nr_entries = 0;
	while (trace->nr_entries < trace->max_entries) {
=======
static void notrace walk_stackframe(struct task_struct *task,
	struct pt_regs *regs, bool (*fn)(void *, unsigned long), void *cookie)
{
	struct unwind_frame_info info;

	unwind_frame_init_task(&info, task, NULL);
	while (1) {
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		if (unwind_once(&info) < 0 || info.ip == 0)
			break;

		if (__kernel_text_address(info.ip))
<<<<<<< HEAD
			trace->entries[trace->nr_entries++] = info.ip;
	}
}


/*
 * Save stack-backtrace addresses into a stack_trace buffer.
 */
void save_stack_trace(struct stack_trace *trace)
{
	dump_trace(current, trace);
	if (trace->nr_entries < trace->max_entries)
		trace->entries[trace->nr_entries++] = ULONG_MAX;
}
EXPORT_SYMBOL_GPL(save_stack_trace);

void save_stack_trace_tsk(struct task_struct *tsk, struct stack_trace *trace)
{
	dump_trace(tsk, trace);
	if (trace->nr_entries < trace->max_entries)
		trace->entries[trace->nr_entries++] = ULONG_MAX;
}
EXPORT_SYMBOL_GPL(save_stack_trace_tsk);
=======
			if (!fn(cookie, info.ip))
				break;
	}
}

void arch_stack_walk(stack_trace_consume_fn consume_entry, void *cookie,
		     struct task_struct *task, struct pt_regs *regs)
{
	walk_stackframe(task, regs, consume_entry, cookie);
}

int arch_stack_walk_reliable(stack_trace_consume_fn consume_entry, void *cookie,
			     struct task_struct *task)
{
	walk_stackframe(task, NULL, consume_entry, cookie);
	return 1;
}
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
