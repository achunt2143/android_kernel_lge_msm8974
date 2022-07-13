<<<<<<< HEAD
/*
 * Read-Copy Update mechanism for mutual exclusion, the Bloatwatch edition.
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
 * Author: Paul E. McKenney <paulmck@linux.vnet.ibm.com>
=======
/* SPDX-License-Identifier: GPL-2.0+ */
/*
 * Read-Copy Update mechanism for mutual exclusion, the Bloatwatch edition.
 *
 * Copyright IBM Corporation, 2008
 *
 * Author: Paul E. McKenney <paulmck@linux.ibm.com>
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 *
 * For detailed explanation of Read-Copy Update mechanism see -
 *		Documentation/RCU
 */
#ifndef __LINUX_TINY_H
#define __LINUX_TINY_H

<<<<<<< HEAD
#include <linux/cache.h>

static inline void rcu_init(void)
{
}

static inline void rcu_barrier_bh(void)
{
	wait_rcu_gp(call_rcu_bh);
}

static inline void rcu_barrier_sched(void)
{
	wait_rcu_gp(call_rcu_sched);
}

#ifdef CONFIG_TINY_RCU

static inline void synchronize_rcu_expedited(void)
{
	synchronize_sched();	/* Only one CPU, so pretty fast anyway!!! */
}

static inline void rcu_barrier(void)
{
	rcu_barrier_sched();  /* Only one CPU, so only one list of callbacks! */
}

#else /* #ifdef CONFIG_TINY_RCU */

void synchronize_rcu_expedited(void);

static inline void rcu_barrier(void)
{
	wait_rcu_gp(call_rcu);
}

#endif /* #else #ifdef CONFIG_TINY_RCU */

static inline void synchronize_rcu_bh(void)
{
	synchronize_sched();
}

static inline void synchronize_rcu_bh_expedited(void)
{
	synchronize_sched();
}

static inline void synchronize_sched_expedited(void)
{
	synchronize_sched();
}

static inline void kfree_call_rcu(struct rcu_head *head,
				  void (*func)(struct rcu_head *rcu))
{
	call_rcu(head, func);
}

#ifdef CONFIG_TINY_RCU

static inline void rcu_preempt_note_context_switch(void)
{
}

static inline int rcu_needs_cpu(int cpu, unsigned long *delta_jiffies)
{
	*delta_jiffies = ULONG_MAX;
	return 0;
}

#else /* #ifdef CONFIG_TINY_RCU */

void rcu_preempt_note_context_switch(void);
int rcu_preempt_needs_cpu(void);

static inline int rcu_needs_cpu(int cpu, unsigned long *delta_jiffies)
{
	*delta_jiffies = ULONG_MAX;
	return rcu_preempt_needs_cpu();
}

#endif /* #else #ifdef CONFIG_TINY_RCU */

static inline void rcu_note_context_switch(int cpu)
{
	rcu_sched_qs(cpu);
	rcu_preempt_note_context_switch();
}
=======
#include <asm/param.h> /* for HZ */

struct rcu_gp_oldstate {
	unsigned long rgos_norm;
};

// Maximum number of rcu_gp_oldstate values corresponding to
// not-yet-completed RCU grace periods.
#define NUM_ACTIVE_RCU_POLL_FULL_OLDSTATE 2

/*
 * Are the two oldstate values the same?  See the Tree RCU version for
 * docbook header.
 */
static inline bool same_state_synchronize_rcu_full(struct rcu_gp_oldstate *rgosp1,
						   struct rcu_gp_oldstate *rgosp2)
{
	return rgosp1->rgos_norm == rgosp2->rgos_norm;
}

unsigned long get_state_synchronize_rcu(void);

static inline void get_state_synchronize_rcu_full(struct rcu_gp_oldstate *rgosp)
{
	rgosp->rgos_norm = get_state_synchronize_rcu();
}

unsigned long start_poll_synchronize_rcu(void);

static inline void start_poll_synchronize_rcu_full(struct rcu_gp_oldstate *rgosp)
{
	rgosp->rgos_norm = start_poll_synchronize_rcu();
}

bool poll_state_synchronize_rcu(unsigned long oldstate);

static inline bool poll_state_synchronize_rcu_full(struct rcu_gp_oldstate *rgosp)
{
	return poll_state_synchronize_rcu(rgosp->rgos_norm);
}

static inline void cond_synchronize_rcu(unsigned long oldstate)
{
	might_sleep();
}

static inline void cond_synchronize_rcu_full(struct rcu_gp_oldstate *rgosp)
{
	cond_synchronize_rcu(rgosp->rgos_norm);
}

static inline unsigned long start_poll_synchronize_rcu_expedited(void)
{
	return start_poll_synchronize_rcu();
}

static inline void start_poll_synchronize_rcu_expedited_full(struct rcu_gp_oldstate *rgosp)
{
	rgosp->rgos_norm = start_poll_synchronize_rcu_expedited();
}

static inline void cond_synchronize_rcu_expedited(unsigned long oldstate)
{
	cond_synchronize_rcu(oldstate);
}

static inline void cond_synchronize_rcu_expedited_full(struct rcu_gp_oldstate *rgosp)
{
	cond_synchronize_rcu_expedited(rgosp->rgos_norm);
}

extern void rcu_barrier(void);

static inline void synchronize_rcu_expedited(void)
{
	synchronize_rcu();
}

/*
 * Add one more declaration of kvfree() here. It is
 * not so straight forward to just include <linux/mm.h>
 * where it is defined due to getting many compile
 * errors caused by that include.
 */
extern void kvfree(const void *addr);

static inline void __kvfree_call_rcu(struct rcu_head *head, void *ptr)
{
	if (head) {
		call_rcu(head, (rcu_callback_t) ((void *) head - ptr));
		return;
	}

	// kvfree_rcu(one_arg) call.
	might_sleep();
	synchronize_rcu();
	kvfree(ptr);
}

#ifdef CONFIG_KASAN_GENERIC
void kvfree_call_rcu(struct rcu_head *head, void *ptr);
#else
static inline void kvfree_call_rcu(struct rcu_head *head, void *ptr)
{
	__kvfree_call_rcu(head, ptr);
}
#endif

void rcu_qs(void);

static inline void rcu_softirq_qs(void)
{
	rcu_qs();
}

#define rcu_note_context_switch(preempt) \
	do { \
		rcu_qs(); \
		rcu_tasks_qs(current, (preempt)); \
	} while (0)

static inline int rcu_needs_cpu(void)
{
	return 0;
}

static inline void rcu_request_urgent_qs_task(struct task_struct *t) { }
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

/*
 * Take advantage of the fact that there is only one CPU, which
 * allows us to ignore virtualization-based context switches.
 */
<<<<<<< HEAD
static inline void rcu_virt_note_context_switch(int cpu)
{
}

/*
 * Return the number of grace periods.
 */
static inline long rcu_batches_completed(void)
{
	return 0;
}

/*
 * Return the number of bottom-half grace periods.
 */
static inline long rcu_batches_completed_bh(void)
{
	return 0;
}

static inline void rcu_force_quiescent_state(void)
{
}

static inline void rcu_bh_force_quiescent_state(void)
{
}

static inline void rcu_sched_force_quiescent_state(void)
{
}

static inline void rcu_cpu_stall_reset(void)
{
}

#ifdef CONFIG_DEBUG_LOCK_ALLOC
extern int rcu_scheduler_active __read_mostly;
extern void rcu_scheduler_starting(void);
#else /* #ifdef CONFIG_DEBUG_LOCK_ALLOC */
static inline void rcu_scheduler_starting(void)
{
}
#endif /* #else #ifdef CONFIG_DEBUG_LOCK_ALLOC */
=======
static inline void rcu_virt_note_context_switch(void) { }
static inline void rcu_cpu_stall_reset(void) { }
static inline int rcu_jiffies_till_stall_check(void) { return 21 * HZ; }
static inline void rcu_irq_exit_check_preempt(void) { }
static inline void exit_rcu(void) { }
static inline bool rcu_preempt_need_deferred_qs(struct task_struct *t)
{
	return false;
}
static inline void rcu_preempt_deferred_qs(struct task_struct *t) { }
void rcu_scheduler_starting(void);
static inline void rcu_end_inkernel_boot(void) { }
static inline bool rcu_inkernel_boot_has_ended(void) { return true; }
static inline bool rcu_is_watching(void) { return true; }
static inline void rcu_momentary_dyntick_idle(void) { }
static inline void kfree_rcu_scheduler_running(void) { }
static inline bool rcu_gp_might_be_stalled(void) { return false; }

/* Avoid RCU read-side critical sections leaking across. */
static inline void rcu_all_qs(void) { barrier(); }

/* RCUtree hotplug events */
#define rcutree_prepare_cpu      NULL
#define rcutree_online_cpu       NULL
#define rcutree_offline_cpu      NULL
#define rcutree_dead_cpu         NULL
#define rcutree_dying_cpu        NULL
static inline void rcutree_report_cpu_starting(unsigned int cpu) { }
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

#endif /* __LINUX_RCUTINY_H */
