<<<<<<< HEAD
/*
 * Copyright (C) 2010 Red Hat, Inc., Peter Zijlstra <pzijlstr@redhat.com>
=======
// SPDX-License-Identifier: GPL-2.0-only
/*
 * Copyright (C) 2010 Red Hat, Inc., Peter Zijlstra
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 *
 * Provides a framework for enqueueing and running callbacks from hardirq
 * context. The enqueueing is NMI-safe.
 */

#include <linux/bug.h>
#include <linux/kernel.h>
#include <linux/export.h>
#include <linux/irq_work.h>
#include <linux/percpu.h>
#include <linux/hardirq.h>
#include <linux/irqflags.h>
<<<<<<< HEAD
#include <asm/processor.h>

/*
 * An entry can be in one of four states:
 *
 * free	     NULL, 0 -> {claimed}       : free to be used
 * claimed   NULL, 3 -> {pending}       : claimed to be enqueued
 * pending   next, 3 -> {busy}          : queued, pending callback
 * busy      NULL, 2 -> {free, claimed} : callback in progress, can be claimed
 */

#define IRQ_WORK_PENDING	1UL
#define IRQ_WORK_BUSY		2UL
#define IRQ_WORK_FLAGS		3UL

static DEFINE_PER_CPU(struct llist_head, irq_work_list);
=======
#include <linux/sched.h>
#include <linux/tick.h>
#include <linux/cpu.h>
#include <linux/notifier.h>
#include <linux/smp.h>
#include <linux/smpboot.h>
#include <asm/processor.h>
#include <linux/kasan.h>

#include <trace/events/ipi.h>

static DEFINE_PER_CPU(struct llist_head, raised_list);
static DEFINE_PER_CPU(struct llist_head, lazy_list);
static DEFINE_PER_CPU(struct task_struct *, irq_workd);

static void wake_irq_workd(void)
{
	struct task_struct *tsk = __this_cpu_read(irq_workd);

	if (!llist_empty(this_cpu_ptr(&lazy_list)) && tsk)
		wake_up_process(tsk);
}

#ifdef CONFIG_SMP
static void irq_work_wake(struct irq_work *entry)
{
	wake_irq_workd();
}

static DEFINE_PER_CPU(struct irq_work, irq_work_wakeup) =
	IRQ_WORK_INIT_HARD(irq_work_wake);
#endif

static int irq_workd_should_run(unsigned int cpu)
{
	return !llist_empty(this_cpu_ptr(&lazy_list));
}
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

/*
 * Claim the entry so that no one else will poke at it.
 */
static bool irq_work_claim(struct irq_work *work)
{
<<<<<<< HEAD
	unsigned long flags, nflags;

	for (;;) {
		flags = work->flags;
		if (flags & IRQ_WORK_PENDING)
			return false;
		nflags = flags | IRQ_WORK_FLAGS;
		if (cmpxchg(&work->flags, flags, nflags) == flags)
			break;
		cpu_relax();
	}

=======
	int oflags;

	oflags = atomic_fetch_or(IRQ_WORK_CLAIMED | CSD_TYPE_IRQ_WORK, &work->node.a_flags);
	/*
	 * If the work is already pending, no need to raise the IPI.
	 * The pairing smp_mb() in irq_work_single() makes sure
	 * everything we did before is visible.
	 */
	if (oflags & IRQ_WORK_PENDING)
		return false;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	return true;
}

void __weak arch_irq_work_raise(void)
{
	/*
	 * Lame architectures will get the timer tick callback
	 */
}

<<<<<<< HEAD
/*
 * Queue the entry and raise the IPI if needed.
 */
static void __irq_work_queue(struct irq_work *work)
{
	bool empty;

	preempt_disable();

	empty = llist_add(&work->llnode, &__get_cpu_var(irq_work_list));
	/* The list was empty, raise self-interrupt to start processing. */
	if (empty)
		arch_irq_work_raise();

	preempt_enable();
}

/*
 * Enqueue the irq_work @entry, returns true on success, failure when the
 * @entry was already enqueued by someone else.
 *
 * Can be re-enqueued while the callback is still in progress.
 */
bool irq_work_queue(struct irq_work *work)
{
	if (!irq_work_claim(work)) {
		/*
		 * Already enqueued, can't do!
		 */
		return false;
	}

	__irq_work_queue(work);
=======
static __always_inline void irq_work_raise(struct irq_work *work)
{
	if (trace_ipi_send_cpu_enabled() && arch_irq_work_has_interrupt())
		trace_ipi_send_cpu(smp_processor_id(), _RET_IP_, work->func);

	arch_irq_work_raise();
}

/* Enqueue on current CPU, work must already be claimed and preempt disabled */
static void __irq_work_queue_local(struct irq_work *work)
{
	struct llist_head *list;
	bool rt_lazy_work = false;
	bool lazy_work = false;
	int work_flags;

	work_flags = atomic_read(&work->node.a_flags);
	if (work_flags & IRQ_WORK_LAZY)
		lazy_work = true;
	else if (IS_ENABLED(CONFIG_PREEMPT_RT) &&
		 !(work_flags & IRQ_WORK_HARD_IRQ))
		rt_lazy_work = true;

	if (lazy_work || rt_lazy_work)
		list = this_cpu_ptr(&lazy_list);
	else
		list = this_cpu_ptr(&raised_list);

	if (!llist_add(&work->node.llist, list))
		return;

	/* If the work is "lazy", handle it from next tick if any */
	if (!lazy_work || tick_nohz_tick_stopped())
		irq_work_raise(work);
}

/* Enqueue the irq work @work on the current CPU */
bool irq_work_queue(struct irq_work *work)
{
	/* Only queue if not already pending */
	if (!irq_work_claim(work))
		return false;

	/* Queue the entry and raise the IPI if needed. */
	preempt_disable();
	__irq_work_queue_local(work);
	preempt_enable();

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	return true;
}
EXPORT_SYMBOL_GPL(irq_work_queue);

/*
<<<<<<< HEAD
 * Run the irq_work entries on this cpu. Requires to be ran from hardirq
 * context with local IRQs disabled.
 */
void irq_work_run(void)
{
	struct irq_work *work;
	struct llist_head *this_list;
	struct llist_node *llnode;

	this_list = &__get_cpu_var(irq_work_list);
	if (llist_empty(this_list))
		return;

	BUG_ON(!in_irq());
	BUG_ON(!irqs_disabled());

	llnode = llist_del_all(this_list);
	while (llnode != NULL) {
		work = llist_entry(llnode, struct irq_work, llnode);

		llnode = llist_next(llnode);

		/*
		 * Clear the PENDING bit, after this point the @work
		 * can be re-used.
		 */
		work->flags = IRQ_WORK_BUSY;
		work->func(work);
		/*
		 * Clear the BUSY bit and return to the free state if
		 * no-one else claimed it meanwhile.
		 */
		(void)cmpxchg(&work->flags, IRQ_WORK_BUSY, 0);
	}
}
EXPORT_SYMBOL_GPL(irq_work_run);

=======
 * Enqueue the irq_work @work on @cpu unless it's already pending
 * somewhere.
 *
 * Can be re-enqueued while the callback is still in progress.
 */
bool irq_work_queue_on(struct irq_work *work, int cpu)
{
#ifndef CONFIG_SMP
	return irq_work_queue(work);

#else /* CONFIG_SMP: */
	/* All work should have been flushed before going offline */
	WARN_ON_ONCE(cpu_is_offline(cpu));

	/* Only queue if not already pending */
	if (!irq_work_claim(work))
		return false;

	kasan_record_aux_stack_noalloc(work);

	preempt_disable();
	if (cpu != smp_processor_id()) {
		/* Arch remote IPI send/receive backend aren't NMI safe */
		WARN_ON_ONCE(in_nmi());

		/*
		 * On PREEMPT_RT the items which are not marked as
		 * IRQ_WORK_HARD_IRQ are added to the lazy list and a HARD work
		 * item is used on the remote CPU to wake the thread.
		 */
		if (IS_ENABLED(CONFIG_PREEMPT_RT) &&
		    !(atomic_read(&work->node.a_flags) & IRQ_WORK_HARD_IRQ)) {

			if (!llist_add(&work->node.llist, &per_cpu(lazy_list, cpu)))
				goto out;

			work = &per_cpu(irq_work_wakeup, cpu);
			if (!irq_work_claim(work))
				goto out;
		}

		__smp_call_single_queue(cpu, &work->node.llist);
	} else {
		__irq_work_queue_local(work);
	}
out:
	preempt_enable();

	return true;
#endif /* CONFIG_SMP */
}

bool irq_work_needs_cpu(void)
{
	struct llist_head *raised, *lazy;

	raised = this_cpu_ptr(&raised_list);
	lazy = this_cpu_ptr(&lazy_list);

	if (llist_empty(raised) || arch_irq_work_has_interrupt())
		if (llist_empty(lazy))
			return false;

	/* All work should have been flushed before going offline */
	WARN_ON_ONCE(cpu_is_offline(smp_processor_id()));

	return true;
}

void irq_work_single(void *arg)
{
	struct irq_work *work = arg;
	int flags;

	/*
	 * Clear the PENDING bit, after this point the @work can be re-used.
	 * The PENDING bit acts as a lock, and we own it, so we can clear it
	 * without atomic ops.
	 */
	flags = atomic_read(&work->node.a_flags);
	flags &= ~IRQ_WORK_PENDING;
	atomic_set(&work->node.a_flags, flags);

	/*
	 * See irq_work_claim().
	 */
	smp_mb();

	lockdep_irq_work_enter(flags);
	work->func(work);
	lockdep_irq_work_exit(flags);

	/*
	 * Clear the BUSY bit, if set, and return to the free state if no-one
	 * else claimed it meanwhile.
	 */
	(void)atomic_cmpxchg(&work->node.a_flags, flags, flags & ~IRQ_WORK_BUSY);

	if ((IS_ENABLED(CONFIG_PREEMPT_RT) && !irq_work_is_hard(work)) ||
	    !arch_irq_work_has_interrupt())
		rcuwait_wake_up(&work->irqwait);
}

static void irq_work_run_list(struct llist_head *list)
{
	struct irq_work *work, *tmp;
	struct llist_node *llnode;

	/*
	 * On PREEMPT_RT IRQ-work which is not marked as HARD will be processed
	 * in a per-CPU thread in preemptible context. Only the items which are
	 * marked as IRQ_WORK_HARD_IRQ will be processed in hardirq context.
	 */
	BUG_ON(!irqs_disabled() && !IS_ENABLED(CONFIG_PREEMPT_RT));

	if (llist_empty(list))
		return;

	llnode = llist_del_all(list);
	llist_for_each_entry_safe(work, tmp, llnode, node.llist)
		irq_work_single(work);
}

/*
 * hotplug calls this through:
 *  hotplug_cfd() -> flush_smp_call_function_queue()
 */
void irq_work_run(void)
{
	irq_work_run_list(this_cpu_ptr(&raised_list));
	if (!IS_ENABLED(CONFIG_PREEMPT_RT))
		irq_work_run_list(this_cpu_ptr(&lazy_list));
	else
		wake_irq_workd();
}
EXPORT_SYMBOL_GPL(irq_work_run);

void irq_work_tick(void)
{
	struct llist_head *raised = this_cpu_ptr(&raised_list);

	if (!llist_empty(raised) && !arch_irq_work_has_interrupt())
		irq_work_run_list(raised);

	if (!IS_ENABLED(CONFIG_PREEMPT_RT))
		irq_work_run_list(this_cpu_ptr(&lazy_list));
	else
		wake_irq_workd();
}

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
/*
 * Synchronize against the irq_work @entry, ensures the entry is not
 * currently in use.
 */
void irq_work_sync(struct irq_work *work)
{
<<<<<<< HEAD
	WARN_ON_ONCE(irqs_disabled());

	while (work->flags & IRQ_WORK_BUSY)
		cpu_relax();
}
EXPORT_SYMBOL_GPL(irq_work_sync);
=======
	lockdep_assert_irqs_enabled();
	might_sleep();

	if ((IS_ENABLED(CONFIG_PREEMPT_RT) && !irq_work_is_hard(work)) ||
	    !arch_irq_work_has_interrupt()) {
		rcuwait_wait_event(&work->irqwait, !irq_work_is_busy(work),
				   TASK_UNINTERRUPTIBLE);
		return;
	}

	while (irq_work_is_busy(work))
		cpu_relax();
}
EXPORT_SYMBOL_GPL(irq_work_sync);

static void run_irq_workd(unsigned int cpu)
{
	irq_work_run_list(this_cpu_ptr(&lazy_list));
}

static void irq_workd_setup(unsigned int cpu)
{
	sched_set_fifo_low(current);
}

static struct smp_hotplug_thread irqwork_threads = {
	.store                  = &irq_workd,
	.setup			= irq_workd_setup,
	.thread_should_run      = irq_workd_should_run,
	.thread_fn              = run_irq_workd,
	.thread_comm            = "irq_work/%u",
};

static __init int irq_work_init_threads(void)
{
	if (IS_ENABLED(CONFIG_PREEMPT_RT))
		BUG_ON(smpboot_register_percpu_thread(&irqwork_threads));
	return 0;
}
early_initcall(irq_work_init_threads);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
