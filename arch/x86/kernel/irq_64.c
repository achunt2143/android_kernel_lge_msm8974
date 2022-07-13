<<<<<<< HEAD
=======
// SPDX-License-Identifier: GPL-2.0
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
/*
 *	Copyright (C) 1992, 1998 Linus Torvalds, Ingo Molnar
 *
 * This file contains the lowest level x86_64-specific interrupt
 * entry and irq statistics code. All the remaining irq logic is
 * done by the generic kernel/irq/ code and in the
 * x86_64-specific irq controller code. (e.g. i8259.c and
 * io_apic.c.)
 */

#include <linux/kernel_stat.h>
#include <linux/interrupt.h>
<<<<<<< HEAD
#include <linux/seq_file.h>
#include <linux/module.h>
=======
#include <linux/irq.h>
#include <linux/seq_file.h>
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#include <linux/delay.h>
#include <linux/ftrace.h>
#include <linux/uaccess.h>
#include <linux/smp.h>
<<<<<<< HEAD
#include <asm/io_apic.h>
#include <asm/idle.h>
#include <asm/apic.h>

DEFINE_PER_CPU_SHARED_ALIGNED(irq_cpustat_t, irq_stat);
EXPORT_PER_CPU_SYMBOL(irq_stat);

DEFINE_PER_CPU(struct pt_regs *, irq_regs);
EXPORT_PER_CPU_SYMBOL(irq_regs);

int sysctl_panic_on_stackoverflow;

/*
 * Probabilistic stack overflow check:
 *
 * Only check the stack in process context, because everything else
 * runs on the big interrupt stacks. Checking reliably is too expensive,
 * so we just check from interrupts.
 */
static inline void stack_overflow_check(struct pt_regs *regs)
{
#ifdef CONFIG_DEBUG_STACKOVERFLOW
#define STACK_TOP_MARGIN	128
	struct orig_ist *oist;
	u64 irq_stack_top, irq_stack_bottom;
	u64 estack_top, estack_bottom;
	u64 curbase = (u64)task_stack_page(current);

	if (user_mode_vm(regs))
		return;

	if (regs->sp >= curbase + sizeof(struct thread_info) +
				  sizeof(struct pt_regs) + STACK_TOP_MARGIN &&
	    regs->sp <= curbase + THREAD_SIZE)
		return;

	irq_stack_top = (u64)__get_cpu_var(irq_stack_union.irq_stack) +
			STACK_TOP_MARGIN;
	irq_stack_bottom = (u64)__get_cpu_var(irq_stack_ptr);
	if (regs->sp >= irq_stack_top && regs->sp <= irq_stack_bottom)
		return;

	oist = &__get_cpu_var(orig_ist);
	estack_top = (u64)oist->ist[0] - EXCEPTION_STKSZ + STACK_TOP_MARGIN;
	estack_bottom = (u64)oist->ist[N_EXCEPTION_STACKS - 1];
	if (regs->sp >= estack_top && regs->sp <= estack_bottom)
		return;

	WARN_ONCE(1, "do_IRQ(): %s has overflown the kernel stack (cur:%Lx,sp:%lx,irq stk top-bottom:%Lx-%Lx,exception stk top-bottom:%Lx-%Lx)\n",
		current->comm, curbase, regs->sp,
		irq_stack_top, irq_stack_bottom,
		estack_top, estack_bottom);

	if (sysctl_panic_on_stackoverflow)
		panic("low stack detected by irq handler - check messages\n");
#endif
}

bool handle_irq(unsigned irq, struct pt_regs *regs)
{
	struct irq_desc *desc;

	stack_overflow_check(regs);

	desc = irq_to_desc(irq);
	if (unlikely(!desc))
		return false;

	generic_handle_irq_desc(irq, desc);
	return true;
}


extern void call_softirq(void);

asmlinkage void do_softirq(void)
{
	__u32 pending;
	unsigned long flags;

	if (in_interrupt())
		return;

	local_irq_save(flags);
	pending = local_softirq_pending();
	/* Switch to interrupt stack */
	if (pending) {
		call_softirq();
		WARN_ON_ONCE(softirq_count());
	}
	local_irq_restore(flags);
=======
#include <linux/sched/task_stack.h>

#include <asm/cpu_entry_area.h>
#include <asm/softirq_stack.h>
#include <asm/irq_stack.h>
#include <asm/io_apic.h>
#include <asm/apic.h>

DEFINE_PER_CPU_PAGE_ALIGNED(struct irq_stack, irq_stack_backing_store) __visible;
DECLARE_INIT_PER_CPU(irq_stack_backing_store);

#ifdef CONFIG_VMAP_STACK
/*
 * VMAP the backing store with guard pages
 */
static int map_irq_stack(unsigned int cpu)
{
	char *stack = (char *)per_cpu_ptr(&irq_stack_backing_store, cpu);
	struct page *pages[IRQ_STACK_SIZE / PAGE_SIZE];
	void *va;
	int i;

	for (i = 0; i < IRQ_STACK_SIZE / PAGE_SIZE; i++) {
		phys_addr_t pa = per_cpu_ptr_to_phys(stack + (i << PAGE_SHIFT));

		pages[i] = pfn_to_page(pa >> PAGE_SHIFT);
	}

	va = vmap(pages, IRQ_STACK_SIZE / PAGE_SIZE, VM_MAP, PAGE_KERNEL);
	if (!va)
		return -ENOMEM;

	/* Store actual TOS to avoid adjustment in the hotpath */
	per_cpu(pcpu_hot.hardirq_stack_ptr, cpu) = va + IRQ_STACK_SIZE - 8;
	return 0;
}
#else
/*
 * If VMAP stacks are disabled due to KASAN, just use the per cpu
 * backing store without guard pages.
 */
static int map_irq_stack(unsigned int cpu)
{
	void *va = per_cpu_ptr(&irq_stack_backing_store, cpu);

	/* Store actual TOS to avoid adjustment in the hotpath */
	per_cpu(pcpu_hot.hardirq_stack_ptr, cpu) = va + IRQ_STACK_SIZE - 8;
	return 0;
}
#endif

int irq_init_percpu_irqstack(unsigned int cpu)
{
	if (per_cpu(pcpu_hot.hardirq_stack_ptr, cpu))
		return 0;
	return map_irq_stack(cpu);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}
