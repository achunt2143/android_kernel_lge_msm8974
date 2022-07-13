<<<<<<< HEAD
=======
// SPDX-License-Identifier: GPL-2.0
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
/*
 *  Copyright (C) 1991, 1992  Linus Torvalds
 *  Copyright (C) 2000, 2001, 2002 Andi Kleen, SuSE Labs
 */
<<<<<<< HEAD
=======
#include <linux/sched/debug.h>
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#include <linux/kallsyms.h>
#include <linux/kprobes.h>
#include <linux/uaccess.h>
#include <linux/hardirq.h>
#include <linux/kdebug.h>
<<<<<<< HEAD
#include <linux/module.h>
=======
#include <linux/export.h>
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#include <linux/ptrace.h>
#include <linux/kexec.h>
#include <linux/sysfs.h>
#include <linux/bug.h>
#include <linux/nmi.h>

<<<<<<< HEAD
#include <asm/stacktrace.h>


#define N_EXCEPTION_STACKS_END \
		(N_EXCEPTION_STACKS + DEBUG_STKSZ/EXCEPTION_STKSZ - 2)

static char x86_stack_ids[][8] = {
		[ DEBUG_STACK-1			]	= "#DB",
		[ NMI_STACK-1			]	= "NMI",
		[ DOUBLEFAULT_STACK-1		]	= "#DF",
		[ MCE_STACK-1			]	= "#MC",
#if DEBUG_STKSZ > EXCEPTION_STKSZ
		[ N_EXCEPTION_STACKS ...
		  N_EXCEPTION_STACKS_END	]	= "#DB[?]"
#endif
};

static unsigned long *in_exception_stack(unsigned cpu, unsigned long stack,
					 unsigned *usedp, char **idp)
{
	unsigned k;

	/*
	 * Iterate over all exception stacks, and figure out whether
	 * 'stack' is in one of them:
	 */
	for (k = 0; k < N_EXCEPTION_STACKS; k++) {
		unsigned long end = per_cpu(orig_ist, cpu).ist[k];
		/*
		 * Is 'stack' above this exception frame's end?
		 * If yes then skip to the next frame.
		 */
		if (stack >= end)
			continue;
		/*
		 * Is 'stack' above this exception frame's start address?
		 * If yes then we found the right frame.
		 */
		if (stack >= end - EXCEPTION_STKSZ) {
			/*
			 * Make sure we only iterate through an exception
			 * stack once. If it comes up for the second time
			 * then there's something wrong going on - just
			 * break out and return NULL:
			 */
			if (*usedp & (1U << k))
				break;
			*usedp |= 1U << k;
			*idp = x86_stack_ids[k];
			return (unsigned long *)end;
		}
		/*
		 * If this is a debug stack, and if it has a larger size than
		 * the usual exception stacks, then 'stack' might still
		 * be within the lower portion of the debug stack:
		 */
#if DEBUG_STKSZ > EXCEPTION_STKSZ
		if (k == DEBUG_STACK - 1 && stack >= end - DEBUG_STKSZ) {
			unsigned j = N_EXCEPTION_STACKS - 1;

			/*
			 * Black magic. A large debug stack is composed of
			 * multiple exception stack entries, which we
			 * iterate through now. Dont look:
			 */
			do {
				++j;
				end -= EXCEPTION_STKSZ;
				x86_stack_ids[j][4] = '1' +
						(j - N_EXCEPTION_STACKS);
			} while (stack < end - EXCEPTION_STKSZ);
			if (*usedp & (1U << j))
				break;
			*usedp |= 1U << j;
			*idp = x86_stack_ids[j];
			return (unsigned long *)end;
		}
#endif
	}
	return NULL;
}

static inline int
in_irq_stack(unsigned long *stack, unsigned long *irq_stack,
	     unsigned long *irq_stack_end)
{
	return (stack >= irq_stack && stack < irq_stack_end);
}

/*
 * x86-64 can have up to three kernel stacks:
 * process stack
 * interrupt stack
 * severe exception (double fault, nmi, stack fault, debug, mce) hardware stack
 */

void dump_trace(struct task_struct *task, struct pt_regs *regs,
		unsigned long *stack, unsigned long bp,
		const struct stacktrace_ops *ops, void *data)
{
	const unsigned cpu = get_cpu();
	unsigned long *irq_stack_end =
		(unsigned long *)per_cpu(irq_stack_ptr, cpu);
	unsigned used = 0;
	struct thread_info *tinfo;
	int graph = 0;
	unsigned long dummy;

	if (!task)
		task = current;

	if (!stack) {
		if (regs)
			stack = (unsigned long *)regs->sp;
		else if (task != current)
			stack = (unsigned long *)task->thread.sp;
		else
			stack = &dummy;
	}

	if (!bp)
		bp = stack_frame(task, regs);
	/*
	 * Print function call entries in all stacks, starting at the
	 * current stack address. If the stacks consist of nested
	 * exceptions
	 */
	tinfo = task_thread_info(task);
	for (;;) {
		char *id;
		unsigned long *estack_end;
		estack_end = in_exception_stack(cpu, (unsigned long)stack,
						&used, &id);

		if (estack_end) {
			if (ops->stack(data, id) < 0)
				break;

			bp = ops->walk_stack(tinfo, stack, bp, ops,
					     data, estack_end, &graph);
			ops->stack(data, "<EOE>");
			/*
			 * We link to the next stack via the
			 * second-to-last pointer (index -2 to end) in the
			 * exception stack:
			 */
			stack = (unsigned long *) estack_end[-2];
			continue;
		}
		if (irq_stack_end) {
			unsigned long *irq_stack;
			irq_stack = irq_stack_end -
				(IRQ_STACK_SIZE - 64) / sizeof(*irq_stack);

			if (in_irq_stack(stack, irq_stack, irq_stack_end)) {
				if (ops->stack(data, "IRQ") < 0)
					break;
				bp = ops->walk_stack(tinfo, stack, bp,
					ops, data, irq_stack_end, &graph);
				/*
				 * We link to the next stack (which would be
				 * the process stack normally) the last
				 * pointer (index -1 to end) in the IRQ stack:
				 */
				stack = (unsigned long *) (irq_stack_end[-1]);
				irq_stack_end = NULL;
				ops->stack(data, "EOI");
				continue;
			}
		}
		break;
	}

	/*
	 * This handles the process stack:
	 */
	bp = ops->walk_stack(tinfo, stack, bp, ops, data, NULL, &graph);
	put_cpu();
}
EXPORT_SYMBOL(dump_trace);

void
show_stack_log_lvl(struct task_struct *task, struct pt_regs *regs,
		   unsigned long *sp, unsigned long bp, char *log_lvl)
{
	unsigned long *irq_stack_end;
	unsigned long *irq_stack;
	unsigned long *stack;
	int cpu;
	int i;

	preempt_disable();
	cpu = smp_processor_id();

	irq_stack_end	= (unsigned long *)(per_cpu(irq_stack_ptr, cpu));
	irq_stack	= (unsigned long *)(per_cpu(irq_stack_ptr, cpu) - IRQ_STACK_SIZE);

	/*
	 * Debugging aid: "show_stack(NULL, NULL);" prints the
	 * back trace for this cpu:
	 */
	if (sp == NULL) {
		if (task)
			sp = (unsigned long *)task->thread.sp;
		else
			sp = (unsigned long *)&sp;
	}

	stack = sp;
	for (i = 0; i < kstack_depth_to_print; i++) {
		if (stack >= irq_stack && stack <= irq_stack_end) {
			if (stack == irq_stack_end) {
				stack = (unsigned long *) (irq_stack_end[-1]);
				printk(KERN_CONT " <EOI> ");
			}
		} else {
		if (((long) stack & (THREAD_SIZE-1)) == 0)
			break;
		}
		if (i && ((i % STACKSLOTS_PER_LINE) == 0))
			printk(KERN_CONT "\n");
		printk(KERN_CONT " %016lx", *stack++);
		touch_nmi_watchdog();
	}
	preempt_enable();

	printk(KERN_CONT "\n");
	show_trace_log_lvl(task, regs, sp, bp, log_lvl);
}

void show_registers(struct pt_regs *regs)
{
	int i;
	unsigned long sp;
	const int cpu = smp_processor_id();
	struct task_struct *cur = current;

	sp = regs->sp;
	printk("CPU %d ", cpu);
	print_modules();
	__show_regs(regs, 1);
	printk("Process %s (pid: %d, threadinfo %p, task %p)\n",
		cur->comm, cur->pid, task_thread_info(cur), cur);

	/*
	 * When in-kernel, we also print out the stack and code at the
	 * time of the fault..
	 */
	if (!user_mode(regs)) {
		unsigned int code_prologue = code_bytes * 43 / 64;
		unsigned int code_len = code_bytes;
		unsigned char c;
		u8 *ip;

		printk(KERN_DEFAULT "Stack:\n");
		show_stack_log_lvl(NULL, regs, (unsigned long *)sp,
				   0, KERN_DEFAULT);

		printk(KERN_DEFAULT "Code: ");

		ip = (u8 *)regs->ip - code_prologue;
		if (ip < (u8 *)PAGE_OFFSET || probe_kernel_address(ip, c)) {
			/* try starting at IP */
			ip = (u8 *)regs->ip;
			code_len = code_len - code_prologue + 1;
		}
		for (i = 0; i < code_len; i++, ip++) {
			if (ip < (u8 *)PAGE_OFFSET ||
					probe_kernel_address(ip, c)) {
				printk(KERN_CONT " Bad RIP value.");
				break;
			}
			if (ip == (u8 *)regs->ip)
				printk(KERN_CONT "<%02x> ", c);
			else
				printk(KERN_CONT "%02x ", c);
		}
	}
	printk(KERN_CONT "\n");
}

int is_valid_bugaddr(unsigned long ip)
{
	unsigned short ud2;

	if (__copy_from_user(&ud2, (const void __user *) ip, sizeof(ud2)))
		return 0;

	return ud2 == 0x0b0f;
=======
#include <asm/cpu_entry_area.h>
#include <asm/stacktrace.h>

static const char * const exception_stack_names[] = {
		[ ESTACK_DF	]	= "#DF",
		[ ESTACK_NMI	]	= "NMI",
		[ ESTACK_DB	]	= "#DB",
		[ ESTACK_MCE	]	= "#MC",
		[ ESTACK_VC	]	= "#VC",
		[ ESTACK_VC2	]	= "#VC2",
};

const char *stack_type_name(enum stack_type type)
{
	BUILD_BUG_ON(N_EXCEPTION_STACKS != 6);

	if (type == STACK_TYPE_TASK)
		return "TASK";

	if (type == STACK_TYPE_IRQ)
		return "IRQ";

	if (type == STACK_TYPE_SOFTIRQ)
		return "SOFTIRQ";

	if (type == STACK_TYPE_ENTRY) {
		/*
		 * On 64-bit, we have a generic entry stack that we
		 * use for all the kernel entry points, including
		 * SYSENTER.
		 */
		return "ENTRY_TRAMPOLINE";
	}

	if (type >= STACK_TYPE_EXCEPTION && type <= STACK_TYPE_EXCEPTION_LAST)
		return exception_stack_names[type - STACK_TYPE_EXCEPTION];

	return NULL;
}

/**
 * struct estack_pages - Page descriptor for exception stacks
 * @offs:	Offset from the start of the exception stack area
 * @size:	Size of the exception stack
 * @type:	Type to store in the stack_info struct
 */
struct estack_pages {
	u32	offs;
	u16	size;
	u16	type;
};

#define EPAGERANGE(st)							\
	[PFN_DOWN(CEA_ESTACK_OFFS(st)) ...				\
	 PFN_DOWN(CEA_ESTACK_OFFS(st) + CEA_ESTACK_SIZE(st) - 1)] = {	\
		.offs	= CEA_ESTACK_OFFS(st),				\
		.size	= CEA_ESTACK_SIZE(st),				\
		.type	= STACK_TYPE_EXCEPTION + ESTACK_ ##st, }

/*
 * Array of exception stack page descriptors. If the stack is larger than
 * PAGE_SIZE, all pages covering a particular stack will have the same
 * info. The guard pages including the not mapped DB2 stack are zeroed
 * out.
 */
static const
struct estack_pages estack_pages[CEA_ESTACK_PAGES] ____cacheline_aligned = {
	EPAGERANGE(DF),
	EPAGERANGE(NMI),
	EPAGERANGE(DB),
	EPAGERANGE(MCE),
	EPAGERANGE(VC),
	EPAGERANGE(VC2),
};

static __always_inline bool in_exception_stack(unsigned long *stack, struct stack_info *info)
{
	unsigned long begin, end, stk = (unsigned long)stack;
	const struct estack_pages *ep;
	struct pt_regs *regs;
	unsigned int k;

	BUILD_BUG_ON(N_EXCEPTION_STACKS != 6);

	begin = (unsigned long)__this_cpu_read(cea_exception_stacks);
	/*
	 * Handle the case where stack trace is collected _before_
	 * cea_exception_stacks had been initialized.
	 */
	if (!begin)
		return false;

	end = begin + sizeof(struct cea_exception_stacks);
	/* Bail if @stack is outside the exception stack area. */
	if (stk < begin || stk >= end)
		return false;

	/* Calc page offset from start of exception stacks */
	k = (stk - begin) >> PAGE_SHIFT;
	/* Lookup the page descriptor */
	ep = &estack_pages[k];
	/* Guard page? */
	if (!ep->size)
		return false;

	begin += (unsigned long)ep->offs;
	end = begin + (unsigned long)ep->size;
	regs = (struct pt_regs *)end - 1;

	info->type	= ep->type;
	info->begin	= (unsigned long *)begin;
	info->end	= (unsigned long *)end;
	info->next_sp	= (unsigned long *)regs->sp;
	return true;
}

static __always_inline bool in_irq_stack(unsigned long *stack, struct stack_info *info)
{
	unsigned long *end = (unsigned long *)this_cpu_read(pcpu_hot.hardirq_stack_ptr);
	unsigned long *begin;

	/*
	 * @end points directly to the top most stack entry to avoid a -8
	 * adjustment in the stack switch hotpath. Adjust it back before
	 * calculating @begin.
	 */
	end++;
	begin = end - (IRQ_STACK_SIZE / sizeof(long));

	/*
	 * Due to the switching logic RSP can never be == @end because the
	 * final operation is 'popq %rsp' which means after that RSP points
	 * to the original stack and not to @end.
	 */
	if (stack < begin || stack >= end)
		return false;

	info->type	= STACK_TYPE_IRQ;
	info->begin	= begin;
	info->end	= end;

	/*
	 * The next stack pointer is stored at the top of the irq stack
	 * before switching to the irq stack. Actual stack entries are all
	 * below that.
	 */
	info->next_sp = (unsigned long *)*(end - 1);

	return true;
}

bool noinstr get_stack_info_noinstr(unsigned long *stack, struct task_struct *task,
				    struct stack_info *info)
{
	if (in_task_stack(stack, task, info))
		return true;

	if (task != current)
		return false;

	if (in_exception_stack(stack, info))
		return true;

	if (in_irq_stack(stack, info))
		return true;

	if (in_entry_stack(stack, info))
		return true;

	return false;
}

int get_stack_info(unsigned long *stack, struct task_struct *task,
		   struct stack_info *info, unsigned long *visit_mask)
{
	task = task ? : current;

	if (!stack)
		goto unknown;

	if (!get_stack_info_noinstr(stack, task, info))
		goto unknown;

	/*
	 * Make sure we don't iterate through any given stack more than once.
	 * If it comes up a second time then there's something wrong going on:
	 * just break out and report an unknown stack type.
	 */
	if (visit_mask) {
		if (*visit_mask & (1UL << info->type)) {
			if (task == current)
				printk_deferred_once(KERN_WARNING "WARNING: stack recursion on stack type %d\n", info->type);
			goto unknown;
		}
		*visit_mask |= 1UL << info->type;
	}

	return 0;

unknown:
	info->type = STACK_TYPE_UNKNOWN;
	return -EINVAL;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}
