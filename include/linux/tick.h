<<<<<<< HEAD
/*  linux/include/linux/tick.h
 *
 *  This file contains the structure definitions for tick related functions
 *
=======
/* SPDX-License-Identifier: GPL-2.0 */
/*
 * Tick related global functions
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 */
#ifndef _LINUX_TICK_H
#define _LINUX_TICK_H

#include <linux/clockchips.h>
#include <linux/irqflags.h>
<<<<<<< HEAD
#include <linux/hrtimer.h>

#ifdef CONFIG_GENERIC_CLOCKEVENTS

enum tick_device_mode {
	TICKDEV_MODE_PERIODIC,
	TICKDEV_MODE_ONESHOT,
};

struct tick_device {
	struct clock_event_device *evtdev;
	enum tick_device_mode mode;
};

enum tick_nohz_mode {
	NOHZ_MODE_INACTIVE,
	NOHZ_MODE_LOWRES,
	NOHZ_MODE_HIGHRES,
};

/**
 * struct tick_sched - sched tick emulation and no idle tick control/stats
 * @sched_timer:	hrtimer to schedule the periodic tick in high
 *			resolution mode
 * @idle_tick:		Store the last idle tick expiry time when the tick
 *			timer is modified for idle sleeps. This is necessary
 *			to resume the tick timer operation in the timeline
 *			when the CPU returns from idle
 * @tick_stopped:	Indicator that the idle tick has been stopped
 * @idle_jiffies:	jiffies at the entry to idle for idle time accounting
 * @idle_calls:		Total number of idle calls
 * @idle_sleeps:	Number of idle calls, where the sched tick was stopped
 * @idle_entrytime:	Time when the idle call was entered
 * @idle_waketime:	Time when the idle was interrupted
 * @idle_exittime:	Time when the idle state was left
 * @idle_sleeptime:	Sum of the time slept in idle with sched tick stopped
 * @iowait_sleeptime:	Sum of the time slept in idle with sched tick stopped, with IO outstanding
 * @do_timer_lst:	CPU was the last one doing do_timer before going idle
 */
struct tick_sched {
	struct hrtimer			sched_timer;
	unsigned long			check_clocks;
	enum tick_nohz_mode		nohz_mode;
	ktime_t				idle_tick;
	int				inidle;
	int				tick_stopped;
	unsigned long			idle_jiffies;
	unsigned long			idle_calls;
	unsigned long			idle_sleeps;
	int				idle_active;
	ktime_t				idle_entrytime;
	ktime_t				idle_waketime;
	ktime_t				idle_exittime;
	ktime_t				idle_sleeptime;
	ktime_t				iowait_sleeptime;
	unsigned long			last_jiffies;
	unsigned long			next_jiffies;
	ktime_t				idle_expires;
	int				do_timer_last;
};

extern void __init tick_init(void);
extern int tick_is_oneshot_available(void);
extern struct tick_device *tick_get_device(int cpu);

# ifdef CONFIG_HIGH_RES_TIMERS
extern int tick_init_highres(void);
extern int tick_program_event(ktime_t expires, int force);
extern void tick_setup_sched_timer(void);
# endif

# if defined CONFIG_NO_HZ || defined CONFIG_HIGH_RES_TIMERS
extern void tick_cancel_sched_timer(int cpu);
# else
static inline void tick_cancel_sched_timer(int cpu) { }
# endif

# ifdef CONFIG_GENERIC_CLOCKEVENTS_BROADCAST
extern struct tick_device *tick_get_broadcast_device(void);
extern struct cpumask *tick_get_broadcast_mask(void);

#  ifdef CONFIG_TICK_ONESHOT
extern struct cpumask *tick_get_broadcast_oneshot_mask(void);
#  endif
#else
static inline struct tick_device *tick_get_broadcast_device(void)
{
	return NULL;
}

static inline struct cpumask *tick_get_broadcast_mask(void)
{
	return NULL;
}
# endif /* BROADCAST */

# ifdef CONFIG_TICK_ONESHOT
extern void tick_clock_notify(void);
extern int tick_check_oneshot_change(int allow_nohz);
extern struct tick_sched *tick_get_tick_sched(int cpu);
extern void tick_check_idle(int cpu);
extern int tick_oneshot_mode_active(void);
#  ifndef arch_needs_cpu
#   define arch_needs_cpu(cpu) (0)
#  endif
# else
static inline void tick_clock_notify(void) { }
static inline int tick_check_oneshot_change(int allow_nohz) { return 0; }
static inline void tick_check_idle(int cpu) { }
static inline int tick_oneshot_mode_active(void) { return 0; }
static inline struct cpumask *tick_get_broadcast_oneshot_mask(void)
{
	return NULL;
}
# endif

#else /* CONFIG_GENERIC_CLOCKEVENTS */
static inline void tick_init(void) { }
static inline void tick_cancel_sched_timer(int cpu) { }
static inline void tick_clock_notify(void) { }
static inline int tick_check_oneshot_change(int allow_nohz) { return 0; }
static inline void tick_check_idle(int cpu) { }
static inline int tick_oneshot_mode_active(void) { return 0; }
#endif /* !CONFIG_GENERIC_CLOCKEVENTS */

# ifdef CONFIG_NO_HZ
extern void tick_nohz_idle_enter(void);
extern void tick_nohz_idle_exit(void);
extern void tick_nohz_irq_exit(void);
extern ktime_t tick_nohz_get_sleep_length(void);
extern u64 get_cpu_idle_time_us(int cpu, u64 *last_update_time);
extern u64 get_cpu_iowait_time_us(int cpu, u64 *last_update_time);
# else
static inline void tick_nohz_idle_enter(void) { }
static inline void tick_nohz_idle_exit(void) { }

static inline ktime_t tick_nohz_get_sleep_length(void)
{
	ktime_t len = { .tv64 = NSEC_PER_SEC/HZ };

	return len;
}
static inline u64 get_cpu_idle_time_us(int cpu, u64 *unused) { return -1; }
static inline u64 get_cpu_iowait_time_us(int cpu, u64 *unused) { return -1; }
# endif /* !NO_HZ */
=======
#include <linux/percpu.h>
#include <linux/context_tracking_state.h>
#include <linux/cpumask.h>
#include <linux/sched.h>
#include <linux/rcupdate.h>
#include <linux/static_key.h>

#ifdef CONFIG_GENERIC_CLOCKEVENTS
extern void __init tick_init(void);
/* Should be core only, but ARM BL switcher requires it */
extern void tick_suspend_local(void);
/* Should be core only, but XEN resume magic and ARM BL switcher require it */
extern void tick_resume_local(void);
extern void tick_cleanup_dead_cpu(int cpu);
#else /* CONFIG_GENERIC_CLOCKEVENTS */
static inline void tick_init(void) { }
static inline void tick_suspend_local(void) { }
static inline void tick_resume_local(void) { }
static inline void tick_cleanup_dead_cpu(int cpu) { }
#endif /* !CONFIG_GENERIC_CLOCKEVENTS */

#if defined(CONFIG_GENERIC_CLOCKEVENTS) && defined(CONFIG_HOTPLUG_CPU)
extern int tick_cpu_dying(unsigned int cpu);
extern void tick_assert_timekeeping_handover(void);
#else
#define tick_cpu_dying	NULL
static inline void tick_assert_timekeeping_handover(void) { }
#endif

#if defined(CONFIG_GENERIC_CLOCKEVENTS) && defined(CONFIG_SUSPEND)
extern void tick_freeze(void);
extern void tick_unfreeze(void);
#else
static inline void tick_freeze(void) { }
static inline void tick_unfreeze(void) { }
#endif

#ifdef CONFIG_TICK_ONESHOT
extern void tick_irq_enter(void);
#  ifndef arch_needs_cpu
#   define arch_needs_cpu() (0)
#  endif
# else
static inline void tick_irq_enter(void) { }
#endif

#if defined(CONFIG_GENERIC_CLOCKEVENTS_BROADCAST) && defined(CONFIG_TICK_ONESHOT)
extern void hotplug_cpu__broadcast_tick_pull(int dead_cpu);
#else
static inline void hotplug_cpu__broadcast_tick_pull(int dead_cpu) { }
#endif

enum tick_broadcast_mode {
	TICK_BROADCAST_OFF,
	TICK_BROADCAST_ON,
	TICK_BROADCAST_FORCE,
};

enum tick_broadcast_state {
	TICK_BROADCAST_EXIT,
	TICK_BROADCAST_ENTER,
};

extern struct static_key_false arch_needs_tick_broadcast;

#ifdef CONFIG_GENERIC_CLOCKEVENTS_BROADCAST
extern void tick_broadcast_control(enum tick_broadcast_mode mode);
#else
static inline void tick_broadcast_control(enum tick_broadcast_mode mode) { }
#endif /* BROADCAST */

#ifdef CONFIG_GENERIC_CLOCKEVENTS
extern int tick_broadcast_oneshot_control(enum tick_broadcast_state state);
#else
static inline int tick_broadcast_oneshot_control(enum tick_broadcast_state state)
{
	return 0;
}
#endif

static inline void tick_broadcast_enable(void)
{
	tick_broadcast_control(TICK_BROADCAST_ON);
}
static inline void tick_broadcast_disable(void)
{
	tick_broadcast_control(TICK_BROADCAST_OFF);
}
static inline void tick_broadcast_force(void)
{
	tick_broadcast_control(TICK_BROADCAST_FORCE);
}
static inline int tick_broadcast_enter(void)
{
	return tick_broadcast_oneshot_control(TICK_BROADCAST_ENTER);
}
static inline void tick_broadcast_exit(void)
{
	tick_broadcast_oneshot_control(TICK_BROADCAST_EXIT);
}

enum tick_dep_bits {
	TICK_DEP_BIT_POSIX_TIMER	= 0,
	TICK_DEP_BIT_PERF_EVENTS	= 1,
	TICK_DEP_BIT_SCHED		= 2,
	TICK_DEP_BIT_CLOCK_UNSTABLE	= 3,
	TICK_DEP_BIT_RCU		= 4,
	TICK_DEP_BIT_RCU_EXP		= 5
};
#define TICK_DEP_BIT_MAX TICK_DEP_BIT_RCU_EXP

#define TICK_DEP_MASK_NONE		0
#define TICK_DEP_MASK_POSIX_TIMER	(1 << TICK_DEP_BIT_POSIX_TIMER)
#define TICK_DEP_MASK_PERF_EVENTS	(1 << TICK_DEP_BIT_PERF_EVENTS)
#define TICK_DEP_MASK_SCHED		(1 << TICK_DEP_BIT_SCHED)
#define TICK_DEP_MASK_CLOCK_UNSTABLE	(1 << TICK_DEP_BIT_CLOCK_UNSTABLE)
#define TICK_DEP_MASK_RCU		(1 << TICK_DEP_BIT_RCU)
#define TICK_DEP_MASK_RCU_EXP		(1 << TICK_DEP_BIT_RCU_EXP)

#ifdef CONFIG_NO_HZ_COMMON
extern bool tick_nohz_enabled;
extern bool tick_nohz_tick_stopped(void);
extern bool tick_nohz_tick_stopped_cpu(int cpu);
extern void tick_nohz_idle_stop_tick(void);
extern void tick_nohz_idle_retain_tick(void);
extern void tick_nohz_idle_restart_tick(void);
extern void tick_nohz_idle_enter(void);
extern void tick_nohz_idle_exit(void);
extern void tick_nohz_irq_exit(void);
extern bool tick_nohz_idle_got_tick(void);
extern ktime_t tick_nohz_get_next_hrtimer(void);
extern ktime_t tick_nohz_get_sleep_length(ktime_t *delta_next);
extern unsigned long tick_nohz_get_idle_calls(void);
extern unsigned long tick_nohz_get_idle_calls_cpu(int cpu);
extern u64 get_cpu_idle_time_us(int cpu, u64 *last_update_time);
extern u64 get_cpu_iowait_time_us(int cpu, u64 *last_update_time);
#else /* !CONFIG_NO_HZ_COMMON */
#define tick_nohz_enabled (0)
static inline int tick_nohz_tick_stopped(void) { return 0; }
static inline int tick_nohz_tick_stopped_cpu(int cpu) { return 0; }
static inline void tick_nohz_idle_stop_tick(void) { }
static inline void tick_nohz_idle_retain_tick(void) { }
static inline void tick_nohz_idle_restart_tick(void) { }
static inline void tick_nohz_idle_enter(void) { }
static inline void tick_nohz_idle_exit(void) { }
static inline bool tick_nohz_idle_got_tick(void) { return false; }
static inline ktime_t tick_nohz_get_next_hrtimer(void)
{
	/* Next wake up is the tick period, assume it starts now */
	return ktime_add(ktime_get(), TICK_NSEC);
}
static inline ktime_t tick_nohz_get_sleep_length(ktime_t *delta_next)
{
	*delta_next = TICK_NSEC;
	return *delta_next;
}
static inline u64 get_cpu_idle_time_us(int cpu, u64 *unused) { return -1; }
static inline u64 get_cpu_iowait_time_us(int cpu, u64 *unused) { return -1; }
#endif /* !CONFIG_NO_HZ_COMMON */

/*
 * Mask of CPUs that are nohz_full.
 *
 * Users should be guarded by CONFIG_NO_HZ_FULL or a tick_nohz_full_cpu()
 * check.
 */
extern cpumask_var_t tick_nohz_full_mask;

#ifdef CONFIG_NO_HZ_FULL
extern bool tick_nohz_full_running;

static inline bool tick_nohz_full_enabled(void)
{
	if (!context_tracking_enabled())
		return false;

	return tick_nohz_full_running;
}

/*
 * Check if a CPU is part of the nohz_full subset. Arrange for evaluating
 * the cpu expression (typically smp_processor_id()) _after_ the static
 * key.
 */
#define tick_nohz_full_cpu(_cpu) ({					\
	bool __ret = false;						\
	if (tick_nohz_full_enabled())					\
		__ret = cpumask_test_cpu((_cpu), tick_nohz_full_mask);	\
	__ret;								\
})

static inline void tick_nohz_full_add_cpus_to(struct cpumask *mask)
{
	if (tick_nohz_full_enabled())
		cpumask_or(mask, mask, tick_nohz_full_mask);
}

extern void tick_nohz_dep_set(enum tick_dep_bits bit);
extern void tick_nohz_dep_clear(enum tick_dep_bits bit);
extern void tick_nohz_dep_set_cpu(int cpu, enum tick_dep_bits bit);
extern void tick_nohz_dep_clear_cpu(int cpu, enum tick_dep_bits bit);
extern void tick_nohz_dep_set_task(struct task_struct *tsk,
				   enum tick_dep_bits bit);
extern void tick_nohz_dep_clear_task(struct task_struct *tsk,
				     enum tick_dep_bits bit);
extern void tick_nohz_dep_set_signal(struct task_struct *tsk,
				     enum tick_dep_bits bit);
extern void tick_nohz_dep_clear_signal(struct signal_struct *signal,
				       enum tick_dep_bits bit);
extern bool tick_nohz_cpu_hotpluggable(unsigned int cpu);

/*
 * The below are tick_nohz_[set,clear]_dep() wrappers that optimize off-cases
 * on top of static keys.
 */
static inline void tick_dep_set(enum tick_dep_bits bit)
{
	if (tick_nohz_full_enabled())
		tick_nohz_dep_set(bit);
}

static inline void tick_dep_clear(enum tick_dep_bits bit)
{
	if (tick_nohz_full_enabled())
		tick_nohz_dep_clear(bit);
}

static inline void tick_dep_set_cpu(int cpu, enum tick_dep_bits bit)
{
	if (tick_nohz_full_cpu(cpu))
		tick_nohz_dep_set_cpu(cpu, bit);
}

static inline void tick_dep_clear_cpu(int cpu, enum tick_dep_bits bit)
{
	if (tick_nohz_full_cpu(cpu))
		tick_nohz_dep_clear_cpu(cpu, bit);
}

static inline void tick_dep_set_task(struct task_struct *tsk,
				     enum tick_dep_bits bit)
{
	if (tick_nohz_full_enabled())
		tick_nohz_dep_set_task(tsk, bit);
}
static inline void tick_dep_clear_task(struct task_struct *tsk,
				       enum tick_dep_bits bit)
{
	if (tick_nohz_full_enabled())
		tick_nohz_dep_clear_task(tsk, bit);
}
static inline void tick_dep_set_signal(struct task_struct *tsk,
				       enum tick_dep_bits bit)
{
	if (tick_nohz_full_enabled())
		tick_nohz_dep_set_signal(tsk, bit);
}
static inline void tick_dep_clear_signal(struct signal_struct *signal,
					 enum tick_dep_bits bit)
{
	if (tick_nohz_full_enabled())
		tick_nohz_dep_clear_signal(signal, bit);
}

extern void tick_nohz_full_kick_cpu(int cpu);
extern void __tick_nohz_task_switch(void);
extern void __init tick_nohz_full_setup(cpumask_var_t cpumask);
#else
static inline bool tick_nohz_full_enabled(void) { return false; }
static inline bool tick_nohz_full_cpu(int cpu) { return false; }
static inline void tick_nohz_full_add_cpus_to(struct cpumask *mask) { }

static inline void tick_nohz_dep_set_cpu(int cpu, enum tick_dep_bits bit) { }
static inline void tick_nohz_dep_clear_cpu(int cpu, enum tick_dep_bits bit) { }
static inline bool tick_nohz_cpu_hotpluggable(unsigned int cpu) { return true; }

static inline void tick_dep_set(enum tick_dep_bits bit) { }
static inline void tick_dep_clear(enum tick_dep_bits bit) { }
static inline void tick_dep_set_cpu(int cpu, enum tick_dep_bits bit) { }
static inline void tick_dep_clear_cpu(int cpu, enum tick_dep_bits bit) { }
static inline void tick_dep_set_task(struct task_struct *tsk,
				     enum tick_dep_bits bit) { }
static inline void tick_dep_clear_task(struct task_struct *tsk,
				       enum tick_dep_bits bit) { }
static inline void tick_dep_set_signal(struct task_struct *tsk,
				       enum tick_dep_bits bit) { }
static inline void tick_dep_clear_signal(struct signal_struct *signal,
					 enum tick_dep_bits bit) { }

static inline void tick_nohz_full_kick_cpu(int cpu) { }
static inline void __tick_nohz_task_switch(void) { }
static inline void tick_nohz_full_setup(cpumask_var_t cpumask) { }
#endif

static inline void tick_nohz_task_switch(void)
{
	if (tick_nohz_full_enabled())
		__tick_nohz_task_switch();
}

static inline void tick_nohz_user_enter_prepare(void)
{
	if (tick_nohz_full_cpu(smp_processor_id()))
		rcu_nocb_flush_deferred_wakeup();
}
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

#endif
