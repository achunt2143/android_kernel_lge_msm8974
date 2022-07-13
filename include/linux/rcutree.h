<<<<<<< HEAD
/*
 * Read-Copy Update mechanism for mutual exclusion (tree-based version)
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
 * Copyright IBM Corporation, 2008
 *
 * Author: Dipankar Sarma <dipankar@in.ibm.com>
 *	   Paul E. McKenney <paulmck@linux.vnet.ibm.com> Hierarchical algorithm
 *
 * Based on the original work by Paul McKenney <paulmck@us.ibm.com>
=======
/* SPDX-License-Identifier: GPL-2.0+ */
/*
 * Read-Copy Update mechanism for mutual exclusion (tree-based version)
 *
 * Copyright IBM Corporation, 2008
 *
 * Author: Dipankar Sarma <dipankar@in.ibm.com>
 *	   Paul E. McKenney <paulmck@linux.ibm.com> Hierarchical algorithm
 *
 * Based on the original work by Paul McKenney <paulmck@linux.ibm.com>
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 * and inputs from Rusty Russell, Andrea Arcangeli and Andi Kleen.
 *
 * For detailed explanation of Read-Copy Update mechanism see -
 *	Documentation/RCU
 */

#ifndef __LINUX_RCUTREE_H
#define __LINUX_RCUTREE_H

<<<<<<< HEAD
extern void rcu_init(void);
extern void rcu_note_context_switch(int cpu);
extern int rcu_needs_cpu(int cpu, unsigned long *delta_jiffies);
extern void rcu_cpu_stall_reset(void);
=======
void rcu_softirq_qs(void);
void rcu_note_context_switch(bool preempt);
int rcu_needs_cpu(void);
void rcu_cpu_stall_reset(void);
void rcu_request_urgent_qs_task(struct task_struct *t);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

/*
 * Note a virtualization-based context switch.  This is simply a
 * wrapper around rcu_note_context_switch(), which allows TINY_RCU
<<<<<<< HEAD
 * to save a few bytes.
 */
static inline void rcu_virt_note_context_switch(int cpu)
{
	rcu_note_context_switch(cpu);
}

extern void synchronize_rcu_bh(void);
extern void synchronize_sched_expedited(void);
extern void synchronize_rcu_expedited(void);

void kfree_call_rcu(struct rcu_head *head, void (*func)(struct rcu_head *rcu));

/**
 * synchronize_rcu_bh_expedited - Brute-force RCU-bh grace period
 *
 * Wait for an RCU-bh grace period to elapse, but use a "big hammer"
 * approach to force the grace period to end quickly.  This consumes
 * significant time on all CPUs and is unfriendly to real-time workloads,
 * so is thus not recommended for any sort of common-case code.  In fact,
 * if you are using synchronize_rcu_bh_expedited() in a loop, please
 * restructure your code to batch your updates, and then use a single
 * synchronize_rcu_bh() instead.
 *
 * Note that it is illegal to call this function while holding any lock
 * that is acquired by a CPU-hotplug notifier.  And yes, it is also illegal
 * to call this function from a CPU-hotplug notifier.  Failing to observe
 * these restriction will result in deadlock.
 */
static inline void synchronize_rcu_bh_expedited(void)
{
	synchronize_sched_expedited();
}

extern void rcu_barrier(void);
extern void rcu_barrier_bh(void);
extern void rcu_barrier_sched(void);

extern unsigned long rcutorture_testseq;
extern unsigned long rcutorture_vernum;
extern long rcu_batches_completed(void);
extern long rcu_batches_completed_bh(void);
extern long rcu_batches_completed_sched(void);

extern void rcu_force_quiescent_state(void);
extern void rcu_bh_force_quiescent_state(void);
extern void rcu_sched_force_quiescent_state(void);

/* A context switch is a grace period for RCU-sched and RCU-bh. */
static inline int rcu_blocking_is_gp(void)
{
	might_sleep();  /* Check for RCU read-side critical section. */
	return num_online_cpus() == 1;
}

extern void rcu_scheduler_starting(void);
extern int rcu_scheduler_active __read_mostly;
=======
 * to save a few bytes. The caller must have disabled interrupts.
 */
static inline void rcu_virt_note_context_switch(void)
{
	rcu_note_context_switch(false);
}

void synchronize_rcu_expedited(void);
void kvfree_call_rcu(struct rcu_head *head, void *ptr);

void rcu_barrier(void);
void rcu_momentary_dyntick_idle(void);
void kfree_rcu_scheduler_running(void);
bool rcu_gp_might_be_stalled(void);

struct rcu_gp_oldstate {
	unsigned long rgos_norm;
	unsigned long rgos_exp;
};

// Maximum number of rcu_gp_oldstate values corresponding to
// not-yet-completed RCU grace periods.
#define NUM_ACTIVE_RCU_POLL_FULL_OLDSTATE 4

/**
 * same_state_synchronize_rcu_full - Are two old-state values identical?
 * @rgosp1: First old-state value.
 * @rgosp2: Second old-state value.
 *
 * The two old-state values must have been obtained from either
 * get_state_synchronize_rcu_full(), start_poll_synchronize_rcu_full(),
 * or get_completed_synchronize_rcu_full().  Returns @true if the two
 * values are identical and @false otherwise.  This allows structures
 * whose lifetimes are tracked by old-state values to push these values
 * to a list header, allowing those structures to be slightly smaller.
 *
 * Note that equality is judged on a bitwise basis, so that an
 * @rcu_gp_oldstate structure with an already-completed state in one field
 * will compare not-equal to a structure with an already-completed state
 * in the other field.  After all, the @rcu_gp_oldstate structure is opaque
 * so how did such a situation come to pass in the first place?
 */
static inline bool same_state_synchronize_rcu_full(struct rcu_gp_oldstate *rgosp1,
						   struct rcu_gp_oldstate *rgosp2)
{
	return rgosp1->rgos_norm == rgosp2->rgos_norm && rgosp1->rgos_exp == rgosp2->rgos_exp;
}

unsigned long start_poll_synchronize_rcu_expedited(void);
void start_poll_synchronize_rcu_expedited_full(struct rcu_gp_oldstate *rgosp);
void cond_synchronize_rcu_expedited(unsigned long oldstate);
void cond_synchronize_rcu_expedited_full(struct rcu_gp_oldstate *rgosp);
unsigned long get_state_synchronize_rcu(void);
void get_state_synchronize_rcu_full(struct rcu_gp_oldstate *rgosp);
unsigned long start_poll_synchronize_rcu(void);
void start_poll_synchronize_rcu_full(struct rcu_gp_oldstate *rgosp);
bool poll_state_synchronize_rcu(unsigned long oldstate);
bool poll_state_synchronize_rcu_full(struct rcu_gp_oldstate *rgosp);
void cond_synchronize_rcu(unsigned long oldstate);
void cond_synchronize_rcu_full(struct rcu_gp_oldstate *rgosp);

#ifdef CONFIG_PROVE_RCU
void rcu_irq_exit_check_preempt(void);
#else
static inline void rcu_irq_exit_check_preempt(void) { }
#endif

struct task_struct;
void rcu_preempt_deferred_qs(struct task_struct *t);

void exit_rcu(void);

void rcu_scheduler_starting(void);
extern int rcu_scheduler_active;
void rcu_end_inkernel_boot(void);
bool rcu_inkernel_boot_has_ended(void);
bool rcu_is_watching(void);
#ifndef CONFIG_PREEMPTION
void rcu_all_qs(void);
#endif

/* RCUtree hotplug events */
int rcutree_prepare_cpu(unsigned int cpu);
int rcutree_online_cpu(unsigned int cpu);
void rcutree_report_cpu_starting(unsigned int cpu);

#ifdef CONFIG_HOTPLUG_CPU
int rcutree_dead_cpu(unsigned int cpu);
int rcutree_dying_cpu(unsigned int cpu);
int rcutree_offline_cpu(unsigned int cpu);
#else
#define rcutree_dead_cpu NULL
#define rcutree_dying_cpu NULL
#define rcutree_offline_cpu NULL
#endif

void rcutree_migrate_callbacks(int cpu);

/* Called from hotplug and also arm64 early secondary boot failure */
void rcutree_report_cpu_dead(void);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

#endif /* __LINUX_RCUTREE_H */
