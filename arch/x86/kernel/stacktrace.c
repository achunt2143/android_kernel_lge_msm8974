/*
 * Stack trace management functions
 *
 *  Copyright (C) 2006-2009 Red Hat, Inc., Ingo Molnar <mingo@redhat.com>
 */
#include <linux/sched.h>
<<<<<<< HEAD
#include <linux/stacktrace.h>
#include <linux/module.h>
#include <linux/uaccess.h>
#include <asm/stacktrace.h>

static int save_stack_stack(void *data, char *name)
{
	return 0;
}

static void
__save_stack_address(void *data, unsigned long addr, bool reliable, bool nosched)
{
	struct stack_trace *trace = data;
#ifdef CONFIG_FRAME_POINTER
	if (!reliable)
		return;
#endif
	if (nosched && in_sched_functions(addr))
		return;
	if (trace->skip > 0) {
		trace->skip--;
		return;
	}
	if (trace->nr_entries < trace->max_entries)
		trace->entries[trace->nr_entries++] = addr;
}

static void save_stack_address(void *data, unsigned long addr, int reliable)
{
	return __save_stack_address(data, addr, reliable, false);
}

static void
save_stack_address_nosched(void *data, unsigned long addr, int reliable)
{
	return __save_stack_address(data, addr, reliable, true);
}

static const struct stacktrace_ops save_stack_ops = {
	.stack		= save_stack_stack,
	.address	= save_stack_address,
	.walk_stack	= print_context_stack,
};

static const struct stacktrace_ops save_stack_ops_nosched = {
	.stack		= save_stack_stack,
	.address	= save_stack_address_nosched,
	.walk_stack	= print_context_stack,
};

/*
 * Save stack-backtrace addresses into a stack_trace buffer.
 */
void save_stack_trace(struct stack_trace *trace)
{
	dump_trace(current, NULL, NULL, 0, &save_stack_ops, trace);
	if (trace->nr_entries < trace->max_entries)
		trace->entries[trace->nr_entries++] = ULONG_MAX;
}
EXPORT_SYMBOL_GPL(save_stack_trace);

void save_stack_trace_regs(struct pt_regs *regs, struct stack_trace *trace)
{
	dump_trace(current, regs, NULL, 0, &save_stack_ops, trace);
	if (trace->nr_entries < trace->max_entries)
		trace->entries[trace->nr_entries++] = ULONG_MAX;
}

void save_stack_trace_tsk(struct task_struct *tsk, struct stack_trace *trace)
{
	dump_trace(tsk, NULL, NULL, 0, &save_stack_ops_nosched, trace);
	if (trace->nr_entries < trace->max_entries)
		trace->entries[trace->nr_entries++] = ULONG_MAX;
}
EXPORT_SYMBOL_GPL(save_stack_trace_tsk);

=======
#include <linux/sched/debug.h>
#include <linux/sched/task_stack.h>
#include <linux/stacktrace.h>
#include <linux/export.h>
#include <linux/uaccess.h>
#include <asm/stacktrace.h>
#include <asm/unwind.h>

void arch_stack_walk(stack_trace_consume_fn consume_entry, void *cookie,
		     struct task_struct *task, struct pt_regs *regs)
{
	struct unwind_state state;
	unsigned long addr;

	if (regs && !consume_entry(cookie, regs->ip))
		return;

	for (unwind_start(&state, task, regs, NULL); !unwind_done(&state);
	     unwind_next_frame(&state)) {
		addr = unwind_get_return_address(&state);
		if (!addr || !consume_entry(cookie, addr))
			break;
	}
}

int arch_stack_walk_reliable(stack_trace_consume_fn consume_entry,
			     void *cookie, struct task_struct *task)
{
	struct unwind_state state;
	struct pt_regs *regs;
	unsigned long addr;

	for (unwind_start(&state, task, NULL, NULL);
	     !unwind_done(&state) && !unwind_error(&state);
	     unwind_next_frame(&state)) {

		regs = unwind_get_entry_regs(&state, NULL);
		if (regs) {
			/* Success path for user tasks */
			if (user_mode(regs))
				return 0;

			/*
			 * Kernel mode registers on the stack indicate an
			 * in-kernel interrupt or exception (e.g., preemption
			 * or a page fault), which can make frame pointers
			 * unreliable.
			 */
			if (IS_ENABLED(CONFIG_FRAME_POINTER))
				return -EINVAL;
		}

		addr = unwind_get_return_address(&state);

		/*
		 * A NULL or invalid return address probably means there's some
		 * generated code which __kernel_text_address() doesn't know
		 * about.
		 */
		if (!addr)
			return -EINVAL;

		if (!consume_entry(cookie, addr))
			return -EINVAL;
	}

	/* Check for stack corruption */
	if (unwind_error(&state))
		return -EINVAL;

	return 0;
}

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
/* Userspace stacktrace - based on kernel/trace/trace_sysprof.c */

struct stack_frame_user {
	const void __user	*next_fp;
	unsigned long		ret_addr;
};

static int
<<<<<<< HEAD
copy_stack_frame(const void __user *fp, struct stack_frame_user *frame)
{
	int ret;

	if (!access_ok(VERIFY_READ, fp, sizeof(*frame)))
=======
copy_stack_frame(const struct stack_frame_user __user *fp,
		 struct stack_frame_user *frame)
{
	int ret;

	if (!__access_ok(fp, sizeof(*frame)))
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		return 0;

	ret = 1;
	pagefault_disable();
<<<<<<< HEAD
	if (__copy_from_user_inatomic(frame, fp, sizeof(*frame)))
=======
	if (__get_user(frame->next_fp, &fp->next_fp) ||
	    __get_user(frame->ret_addr, &fp->ret_addr))
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		ret = 0;
	pagefault_enable();

	return ret;
}

<<<<<<< HEAD
static inline void __save_stack_trace_user(struct stack_trace *trace)
{
	const struct pt_regs *regs = task_pt_regs(current);
	const void __user *fp = (const void __user *)regs->bp;

	if (trace->nr_entries < trace->max_entries)
		trace->entries[trace->nr_entries++] = regs->ip;

	while (trace->nr_entries < trace->max_entries) {
=======
void arch_stack_walk_user(stack_trace_consume_fn consume_entry, void *cookie,
			  const struct pt_regs *regs)
{
	const void __user *fp = (const void __user *)regs->bp;

	if (!consume_entry(cookie, regs->ip))
		return;

	while (1) {
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		struct stack_frame_user frame;

		frame.next_fp = NULL;
		frame.ret_addr = 0;
		if (!copy_stack_frame(fp, &frame))
			break;
		if ((unsigned long)fp < regs->sp)
			break;
<<<<<<< HEAD
		if (frame.ret_addr) {
			trace->entries[trace->nr_entries++] =
				frame.ret_addr;
		}
		if (fp == frame.next_fp)
=======
		if (!frame.ret_addr)
			break;
		if (!consume_entry(cookie, frame.ret_addr))
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
			break;
		fp = frame.next_fp;
	}
}

<<<<<<< HEAD
void save_stack_trace_user(struct stack_trace *trace)
{
	/*
	 * Trace user stack if we are not a kernel thread
	 */
	if (current->mm) {
		__save_stack_trace_user(trace);
	}
	if (trace->nr_entries < trace->max_entries)
		trace->entries[trace->nr_entries++] = ULONG_MAX;
}

=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
