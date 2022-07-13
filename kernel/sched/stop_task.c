<<<<<<< HEAD
#include "sched.h"

=======
// SPDX-License-Identifier: GPL-2.0
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
/*
 * stop-task scheduling class.
 *
 * The stop task is the highest priority task in the system, it preempts
 * everything and will be preempted by nothing.
 *
 * See kernel/stop_machine.c
 */

#ifdef CONFIG_SMP
static int
<<<<<<< HEAD
select_task_rq_stop(struct task_struct *p, int sd_flag, int flags)
{
	return task_cpu(p); /* stop tasks as never migrate */
}
#endif /* CONFIG_SMP */

static void
check_preempt_curr_stop(struct rq *rq, struct task_struct *p, int flags)
=======
select_task_rq_stop(struct task_struct *p, int cpu, int flags)
{
	return task_cpu(p); /* stop tasks as never migrate */
}

static int
balance_stop(struct rq *rq, struct task_struct *prev, struct rq_flags *rf)
{
	return sched_stop_runnable(rq);
}
#endif /* CONFIG_SMP */

static void
wakeup_preempt_stop(struct rq *rq, struct task_struct *p, int flags)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
{
	/* we're never preempted */
}

<<<<<<< HEAD
static struct task_struct *pick_next_task_stop(struct rq *rq)
{
	struct task_struct *stop = rq->stop;

	if (stop && stop->on_rq) {
		stop->se.exec_start = rq->clock_task;
		return stop;
	}

	return NULL;
=======
static void set_next_task_stop(struct rq *rq, struct task_struct *stop, bool first)
{
	stop->se.exec_start = rq_clock_task(rq);
}

static struct task_struct *pick_task_stop(struct rq *rq)
{
	if (!sched_stop_runnable(rq))
		return NULL;

	return rq->stop;
}

static struct task_struct *pick_next_task_stop(struct rq *rq)
{
	struct task_struct *p = pick_task_stop(rq);

	if (p)
		set_next_task_stop(rq, p, true);

	return p;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

static void
enqueue_task_stop(struct rq *rq, struct task_struct *p, int flags)
{
<<<<<<< HEAD
	inc_nr_running(rq);
=======
	add_nr_running(rq, 1);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

static void
dequeue_task_stop(struct rq *rq, struct task_struct *p, int flags)
{
<<<<<<< HEAD
	dec_nr_running(rq);
=======
	sub_nr_running(rq, 1);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

static void yield_task_stop(struct rq *rq)
{
	BUG(); /* the stop task should never yield, its pointless. */
}

static void put_prev_task_stop(struct rq *rq, struct task_struct *prev)
{
<<<<<<< HEAD
	struct task_struct *curr = rq->curr;
	u64 delta_exec;

	delta_exec = rq->clock_task - curr->se.exec_start;
	if (unlikely((s64)delta_exec < 0))
		delta_exec = 0;

	schedstat_set(curr->se.statistics.exec_max,
			max(curr->se.statistics.exec_max, delta_exec));

	curr->se.sum_exec_runtime += delta_exec;
	account_group_exec_runtime(curr, delta_exec);

	curr->se.exec_start = rq->clock_task;
	cpuacct_charge(curr, delta_exec);
}

=======
	update_curr_common(rq);
}

/*
 * scheduler tick hitting a task of our scheduling class.
 *
 * NOTE: This function can be called remotely by the tick offload that
 * goes along full dynticks. Therefore no local assumption can be made
 * and everything must be accessed through the @rq and @curr passed in
 * parameters.
 */
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
static void task_tick_stop(struct rq *rq, struct task_struct *curr, int queued)
{
}

<<<<<<< HEAD
static void set_curr_task_stop(struct rq *rq)
{
	struct task_struct *stop = rq->stop;

	stop->se.exec_start = rq->clock_task;
}

=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
static void switched_to_stop(struct rq *rq, struct task_struct *p)
{
	BUG(); /* its impossible to change to this class */
}

static void
prio_changed_stop(struct rq *rq, struct task_struct *p, int oldprio)
{
	BUG(); /* how!?, what priority? */
}

<<<<<<< HEAD
static unsigned int
get_rr_interval_stop(struct rq *rq, struct task_struct *task)
{
	return 0;
=======
static void update_curr_stop(struct rq *rq)
{
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

/*
 * Simple, special scheduling class for the per-CPU stop tasks:
 */
<<<<<<< HEAD
const struct sched_class stop_sched_class = {
	.next			= &rt_sched_class,
=======
DEFINE_SCHED_CLASS(stop) = {
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	.enqueue_task		= enqueue_task_stop,
	.dequeue_task		= dequeue_task_stop,
	.yield_task		= yield_task_stop,

<<<<<<< HEAD
	.check_preempt_curr	= check_preempt_curr_stop,

	.pick_next_task		= pick_next_task_stop,
	.put_prev_task		= put_prev_task_stop,

#ifdef CONFIG_SMP
	.select_task_rq		= select_task_rq_stop,
#endif

	.set_curr_task          = set_curr_task_stop,
	.task_tick		= task_tick_stop,

	.get_rr_interval	= get_rr_interval_stop,

	.prio_changed		= prio_changed_stop,
	.switched_to		= switched_to_stop,
=======
	.wakeup_preempt		= wakeup_preempt_stop,

	.pick_next_task		= pick_next_task_stop,
	.put_prev_task		= put_prev_task_stop,
	.set_next_task          = set_next_task_stop,

#ifdef CONFIG_SMP
	.balance		= balance_stop,
	.pick_task		= pick_task_stop,
	.select_task_rq		= select_task_rq_stop,
	.set_cpus_allowed	= set_cpus_allowed_common,
#endif

	.task_tick		= task_tick_stop,

	.prio_changed		= prio_changed_stop,
	.switched_to		= switched_to_stop,
	.update_curr		= update_curr_stop,
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
};
