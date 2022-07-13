/*
 * cpuidle.c - core cpuidle infrastructure
 *
 * (C) 2006-2007 Venkatesh Pallipadi <venkatesh.pallipadi@intel.com>
 *               Shaohua Li <shaohua.li@intel.com>
 *               Adam Belay <abelay@novell.com>
 *
 * This code is licenced under the GPL.
 */

<<<<<<< HEAD
#include <linux/kernel.h>
#include <linux/mutex.h>
#include <linux/sched.h>
=======
#include "linux/percpu-defs.h"
#include <linux/clockchips.h>
#include <linux/kernel.h>
#include <linux/mutex.h>
#include <linux/sched.h>
#include <linux/sched/clock.h>
#include <linux/sched/idle.h>
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#include <linux/notifier.h>
#include <linux/pm_qos.h>
#include <linux/cpu.h>
#include <linux/cpuidle.h>
#include <linux/ktime.h>
#include <linux/hrtimer.h>
#include <linux/module.h>
<<<<<<< HEAD
=======
#include <linux/suspend.h>
#include <linux/tick.h>
#include <linux/mmu_context.h>
#include <linux/context_tracking.h>
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#include <trace/events/power.h>

#include "cpuidle.h"

DEFINE_PER_CPU(struct cpuidle_device *, cpuidle_devices);
DEFINE_PER_CPU(struct cpuidle_device, cpuidle_dev);

DEFINE_MUTEX(cpuidle_lock);
LIST_HEAD(cpuidle_detected_devices);

static int enabled_devices;
static int off __read_mostly;
static int initialized __read_mostly;

int cpuidle_disabled(void)
{
	return off;
}
void disable_cpuidle(void)
{
	off = 1;
}

<<<<<<< HEAD
#if defined(CONFIG_ARCH_HAS_CPU_IDLE_WAIT)
static void cpuidle_kick_cpus(void)
{
	cpu_idle_wait();
}
#elif defined(CONFIG_SMP)
# error "Arch needs cpu_idle_wait() equivalent here"
#else /* !CONFIG_ARCH_HAS_CPU_IDLE_WAIT && !CONFIG_SMP */
static void cpuidle_kick_cpus(void) {}
#endif

static int __cpuidle_register_device(struct cpuidle_device *dev);

static inline int cpuidle_enter(struct cpuidle_device *dev,
				struct cpuidle_driver *drv, int index)
{
	struct cpuidle_state *target_state = &drv->states[index];
	return target_state->enter(dev, drv, index);
}

static inline int cpuidle_enter_tk(struct cpuidle_device *dev,
			       struct cpuidle_driver *drv, int index)
{
	return cpuidle_wrap_enter(dev, drv, index, cpuidle_enter);
}

typedef int (*cpuidle_enter_t)(struct cpuidle_device *dev,
			       struct cpuidle_driver *drv, int index);

static cpuidle_enter_t cpuidle_enter_ops;
=======
bool cpuidle_not_available(struct cpuidle_driver *drv,
			   struct cpuidle_device *dev)
{
	return off || !initialized || !drv || !dev || !dev->enabled;
}
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

/**
 * cpuidle_play_dead - cpu off-lining
 *
 * Returns in case of an error or no driver
 */
int cpuidle_play_dead(void)
{
	struct cpuidle_device *dev = __this_cpu_read(cpuidle_devices);
<<<<<<< HEAD
	struct cpuidle_driver *drv = cpuidle_get_driver();
	int i, dead_state = -1;
	int power_usage = -1;
=======
	struct cpuidle_driver *drv = cpuidle_get_cpu_driver(dev);
	int i;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	if (!drv)
		return -ENODEV;

	/* Find lowest-power state that supports long-term idle */
<<<<<<< HEAD
	for (i = CPUIDLE_DRIVER_STATE_START; i < drv->state_count; i++) {
		struct cpuidle_state *s = &drv->states[i];

		if (s->power_usage < power_usage && s->enter_dead) {
			power_usage = s->power_usage;
			dead_state = i;
		}
	}

	if (dead_state != -1)
		return drv->states[dead_state].enter_dead(dev, dead_state);
=======
	for (i = drv->state_count - 1; i >= 0; i--)
		if (drv->states[i].enter_dead)
			return drv->states[i].enter_dead(dev, i);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	return -ENODEV;
}

<<<<<<< HEAD
=======
static int find_deepest_state(struct cpuidle_driver *drv,
			      struct cpuidle_device *dev,
			      u64 max_latency_ns,
			      unsigned int forbidden_flags,
			      bool s2idle)
{
	u64 latency_req = 0;
	int i, ret = 0;

	for (i = 1; i < drv->state_count; i++) {
		struct cpuidle_state *s = &drv->states[i];

		if (dev->states_usage[i].disable ||
		    s->exit_latency_ns <= latency_req ||
		    s->exit_latency_ns > max_latency_ns ||
		    (s->flags & forbidden_flags) ||
		    (s2idle && !s->enter_s2idle))
			continue;

		latency_req = s->exit_latency_ns;
		ret = i;
	}
	return ret;
}

/**
 * cpuidle_use_deepest_state - Set/unset governor override mode.
 * @latency_limit_ns: Idle state exit latency limit (or no override if 0).
 *
 * If @latency_limit_ns is nonzero, set the current CPU to use the deepest idle
 * state with exit latency within @latency_limit_ns (override governors going
 * forward), or do not override governors if it is zero.
 */
void cpuidle_use_deepest_state(u64 latency_limit_ns)
{
	struct cpuidle_device *dev;

	preempt_disable();
	dev = cpuidle_get_device();
	if (dev)
		dev->forced_idle_latency_limit_ns = latency_limit_ns;
	preempt_enable();
}

/**
 * cpuidle_find_deepest_state - Find the deepest available idle state.
 * @drv: cpuidle driver for the given CPU.
 * @dev: cpuidle device for the given CPU.
 * @latency_limit_ns: Idle state exit latency limit
 *
 * Return: the index of the deepest available idle state.
 */
int cpuidle_find_deepest_state(struct cpuidle_driver *drv,
			       struct cpuidle_device *dev,
			       u64 latency_limit_ns)
{
	return find_deepest_state(drv, dev, latency_limit_ns, 0, false);
}

#ifdef CONFIG_SUSPEND
static noinstr void enter_s2idle_proper(struct cpuidle_driver *drv,
					 struct cpuidle_device *dev, int index)
{
	struct cpuidle_state *target_state = &drv->states[index];
	ktime_t time_start, time_end;

	instrumentation_begin();

	time_start = ns_to_ktime(local_clock_noinstr());

	tick_freeze();
	/*
	 * The state used here cannot be a "coupled" one, because the "coupled"
	 * cpuidle mechanism enables interrupts and doing that with timekeeping
	 * suspended is generally unsafe.
	 */
	stop_critical_timings();
	if (!(target_state->flags & CPUIDLE_FLAG_RCU_IDLE)) {
		ct_cpuidle_enter();
		/* Annotate away the indirect call */
		instrumentation_begin();
	}
	target_state->enter_s2idle(dev, drv, index);
	if (WARN_ON_ONCE(!irqs_disabled()))
		raw_local_irq_disable();
	if (!(target_state->flags & CPUIDLE_FLAG_RCU_IDLE)) {
		instrumentation_end();
		ct_cpuidle_exit();
	}
	tick_unfreeze();
	start_critical_timings();

	time_end = ns_to_ktime(local_clock_noinstr());

	dev->states_usage[index].s2idle_time += ktime_us_delta(time_end, time_start);
	dev->states_usage[index].s2idle_usage++;
	instrumentation_end();
}

/**
 * cpuidle_enter_s2idle - Enter an idle state suitable for suspend-to-idle.
 * @drv: cpuidle driver for the given CPU.
 * @dev: cpuidle device for the given CPU.
 *
 * If there are states with the ->enter_s2idle callback, find the deepest of
 * them and enter it with frozen tick.
 */
int cpuidle_enter_s2idle(struct cpuidle_driver *drv, struct cpuidle_device *dev)
{
	int index;

	/*
	 * Find the deepest state with ->enter_s2idle present, which guarantees
	 * that interrupts won't be enabled when it exits and allows the tick to
	 * be frozen safely.
	 */
	index = find_deepest_state(drv, dev, U64_MAX, 0, true);
	if (index > 0) {
		enter_s2idle_proper(drv, dev, index);
		local_irq_enable();
	}
	return index;
}
#endif /* CONFIG_SUSPEND */

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
/**
 * cpuidle_enter_state - enter the state and update stats
 * @dev: cpuidle device for this cpu
 * @drv: cpuidle driver for this cpu
<<<<<<< HEAD
 * @next_state: index into drv->states of the state to enter
 */
int cpuidle_enter_state(struct cpuidle_device *dev, struct cpuidle_driver *drv,
		int next_state)
{
	int entered_state;

	entered_state = cpuidle_enter_ops(dev, drv, next_state);

	if (entered_state >= 0) {
		/* Update cpuidle counters */
		/* This can be moved to within driver enter routine
		 * but that results in multiple copies of same code.
		 */
		dev->states_usage[entered_state].time +=
				(unsigned long long)dev->last_residency;
		dev->states_usage[entered_state].usage++;
	} else {
		dev->last_residency = 0;
	}

=======
 * @index: index into the states table in @drv of the state to enter
 */
noinstr int cpuidle_enter_state(struct cpuidle_device *dev,
				 struct cpuidle_driver *drv,
				 int index)
{
	int entered_state;

	struct cpuidle_state *target_state = &drv->states[index];
	bool broadcast = !!(target_state->flags & CPUIDLE_FLAG_TIMER_STOP);
	ktime_t time_start, time_end;

	instrumentation_begin();

	/*
	 * Tell the time framework to switch to a broadcast timer because our
	 * local timer will be shut down.  If a local timer is used from another
	 * CPU as a broadcast timer, this call may fail if it is not available.
	 */
	if (broadcast && tick_broadcast_enter()) {
		index = find_deepest_state(drv, dev, target_state->exit_latency_ns,
					   CPUIDLE_FLAG_TIMER_STOP, false);
		if (index < 0) {
			default_idle_call();
			return -EBUSY;
		}
		target_state = &drv->states[index];
		broadcast = false;
	}

	if (target_state->flags & CPUIDLE_FLAG_TLB_FLUSHED)
		leave_mm();

	/* Take note of the planned idle state. */
	sched_idle_set_state(target_state);

	trace_cpu_idle(index, dev->cpu);
	time_start = ns_to_ktime(local_clock_noinstr());

	stop_critical_timings();
	if (!(target_state->flags & CPUIDLE_FLAG_RCU_IDLE)) {
		ct_cpuidle_enter();
		/* Annotate away the indirect call */
		instrumentation_begin();
	}

	/*
	 * NOTE!!
	 *
	 * For cpuidle_state::enter() methods that do *NOT* set
	 * CPUIDLE_FLAG_RCU_IDLE RCU will be disabled here and these functions
	 * must be marked either noinstr or __cpuidle.
	 *
	 * For cpuidle_state::enter() methods that *DO* set
	 * CPUIDLE_FLAG_RCU_IDLE this isn't required, but they must mark the
	 * function calling ct_cpuidle_enter() as noinstr/__cpuidle and all
	 * functions called within the RCU-idle region.
	 */
	entered_state = target_state->enter(dev, drv, index);

	if (WARN_ONCE(!irqs_disabled(), "%ps leaked IRQ state", target_state->enter))
		raw_local_irq_disable();

	if (!(target_state->flags & CPUIDLE_FLAG_RCU_IDLE)) {
		instrumentation_end();
		ct_cpuidle_exit();
	}
	start_critical_timings();

	sched_clock_idle_wakeup_event();
	time_end = ns_to_ktime(local_clock_noinstr());
	trace_cpu_idle(PWR_EVENT_EXIT, dev->cpu);

	/* The cpu is no longer idle or about to enter idle. */
	sched_idle_set_state(NULL);

	if (broadcast)
		tick_broadcast_exit();

	if (!cpuidle_state_is_coupled(drv, index))
		local_irq_enable();

	if (entered_state >= 0) {
		s64 diff, delay = drv->states[entered_state].exit_latency_ns;
		int i;

		/*
		 * Update cpuidle counters
		 * This can be moved to within driver enter routine,
		 * but that results in multiple copies of same code.
		 */
		diff = ktime_sub(time_end, time_start);

		dev->last_residency_ns = diff;
		dev->states_usage[entered_state].time_ns += diff;
		dev->states_usage[entered_state].usage++;

		if (diff < drv->states[entered_state].target_residency_ns) {
			for (i = entered_state - 1; i >= 0; i--) {
				if (dev->states_usage[i].disable)
					continue;

				/* Shallower states are enabled, so update. */
				dev->states_usage[entered_state].above++;
				trace_cpu_idle_miss(dev->cpu, entered_state, false);
				break;
			}
		} else if (diff > delay) {
			for (i = entered_state + 1; i < drv->state_count; i++) {
				if (dev->states_usage[i].disable)
					continue;

				/*
				 * Update if a deeper state would have been a
				 * better match for the observed idle duration.
				 */
				if (diff - delay >= drv->states[i].target_residency_ns) {
					dev->states_usage[entered_state].below++;
					trace_cpu_idle_miss(dev->cpu, entered_state, true);
				}

				break;
			}
		}
	} else {
		dev->last_residency_ns = 0;
		dev->states_usage[index].rejected++;
	}

	instrumentation_end();

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	return entered_state;
}

/**
<<<<<<< HEAD
 * cpuidle_idle_call - the main idle loop
 *
 * NOTE: no locks or semaphores should be used here
 * return non-zero on failure
 */
int cpuidle_idle_call(void)
{
	struct cpuidle_device *dev = __this_cpu_read(cpuidle_devices);
	struct cpuidle_driver *drv = cpuidle_get_driver();
	int next_state, entered_state;

	if (off)
		return -ENODEV;

	if (!initialized)
		return -ENODEV;

	/* check if the device is ready */
	if (!dev || !dev->enabled)
		return -EBUSY;

#if 0
	/* shows regressions, re-enable for 2.6.29 */
	/*
	 * run any timers that can be run now, at this point
	 * before calculating the idle duration etc.
	 */
	hrtimer_peek_ahead_timers();
#endif

	/* ask the governor for the next state */
	next_state = cpuidle_curr_governor->select(drv, dev);
	if (need_resched()) {
		local_irq_enable();
		return 0;
	}

	trace_power_start_rcuidle(POWER_CSTATE, next_state, dev->cpu);
	trace_cpu_idle_rcuidle(next_state, dev->cpu);

	if (cpuidle_state_is_coupled(dev, drv, next_state))
		entered_state = cpuidle_enter_state_coupled(dev, drv,
							    next_state);
	else
		entered_state = cpuidle_enter_state(dev, drv, next_state);

	trace_power_end_rcuidle(dev->cpu);
	trace_cpu_idle_rcuidle(PWR_EVENT_EXIT, dev->cpu);

	/* give the governor an opportunity to reflect on the outcome */
	if (cpuidle_curr_governor->reflect)
		cpuidle_curr_governor->reflect(dev, entered_state);

	return 0;
=======
 * cpuidle_select - ask the cpuidle framework to choose an idle state
 *
 * @drv: the cpuidle driver
 * @dev: the cpuidle device
 * @stop_tick: indication on whether or not to stop the tick
 *
 * Returns the index of the idle state.  The return value must not be negative.
 *
 * The memory location pointed to by @stop_tick is expected to be written the
 * 'false' boolean value if the scheduler tick should not be stopped before
 * entering the returned state.
 */
int cpuidle_select(struct cpuidle_driver *drv, struct cpuidle_device *dev,
		   bool *stop_tick)
{
	return cpuidle_curr_governor->select(drv, dev, stop_tick);
}

/**
 * cpuidle_enter - enter into the specified idle state
 *
 * @drv:   the cpuidle driver tied with the cpu
 * @dev:   the cpuidle device
 * @index: the index in the idle state table
 *
 * Returns the index in the idle state, < 0 in case of error.
 * The error code depends on the backend driver
 */
int cpuidle_enter(struct cpuidle_driver *drv, struct cpuidle_device *dev,
		  int index)
{
	int ret = 0;

	/*
	 * Store the next hrtimer, which becomes either next tick or the next
	 * timer event, whatever expires first. Additionally, to make this data
	 * useful for consumers outside cpuidle, we rely on that the governor's
	 * ->select() callback have decided, whether to stop the tick or not.
	 */
	WRITE_ONCE(dev->next_hrtimer, tick_nohz_get_next_hrtimer());

	if (cpuidle_state_is_coupled(drv, index))
		ret = cpuidle_enter_state_coupled(dev, drv, index);
	else
		ret = cpuidle_enter_state(dev, drv, index);

	WRITE_ONCE(dev->next_hrtimer, 0);
	return ret;
}

/**
 * cpuidle_reflect - tell the underlying governor what was the state
 * we were in
 *
 * @dev  : the cpuidle device
 * @index: the index in the idle state table
 *
 */
void cpuidle_reflect(struct cpuidle_device *dev, int index)
{
	if (cpuidle_curr_governor->reflect && index >= 0)
		cpuidle_curr_governor->reflect(dev, index);
}

/*
 * Min polling interval of 10usec is a guess. It is assuming that
 * for most users, the time for a single ping-pong workload like
 * perf bench pipe would generally complete within 10usec but
 * this is hardware dependant. Actual time can be estimated with
 *
 * perf bench sched pipe -l 10000
 *
 * Run multiple times to avoid cpufreq effects.
 */
#define CPUIDLE_POLL_MIN 10000
#define CPUIDLE_POLL_MAX (TICK_NSEC / 16)

/**
 * cpuidle_poll_time - return amount of time to poll for,
 * governors can override dev->poll_limit_ns if necessary
 *
 * @drv:   the cpuidle driver tied with the cpu
 * @dev:   the cpuidle device
 *
 */
__cpuidle u64 cpuidle_poll_time(struct cpuidle_driver *drv,
		      struct cpuidle_device *dev)
{
	int i;
	u64 limit_ns;

	BUILD_BUG_ON(CPUIDLE_POLL_MIN > CPUIDLE_POLL_MAX);

	if (dev->poll_limit_ns)
		return dev->poll_limit_ns;

	limit_ns = CPUIDLE_POLL_MAX;
	for (i = 1; i < drv->state_count; i++) {
		u64 state_limit;

		if (dev->states_usage[i].disable)
			continue;

		state_limit = drv->states[i].target_residency_ns;
		if (state_limit < CPUIDLE_POLL_MIN)
			continue;

		limit_ns = min_t(u64, state_limit, CPUIDLE_POLL_MAX);
		break;
	}

	dev->poll_limit_ns = limit_ns;

	return dev->poll_limit_ns;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

/**
 * cpuidle_install_idle_handler - installs the cpuidle idle loop handler
 */
void cpuidle_install_idle_handler(void)
{
	if (enabled_devices) {
		/* Make sure all changes finished before we switch to new idle */
		smp_wmb();
		initialized = 1;
	}
}

/**
 * cpuidle_uninstall_idle_handler - uninstalls the cpuidle idle loop handler
 */
void cpuidle_uninstall_idle_handler(void)
{
	if (enabled_devices) {
		initialized = 0;
<<<<<<< HEAD
		cpuidle_kick_cpus();
	}
=======
		wake_up_all_idle_cpus();
	}

	/*
	 * Make sure external observers (such as the scheduler)
	 * are done looking at pointed idle states.
	 */
	synchronize_rcu();
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

/**
 * cpuidle_pause_and_lock - temporarily disables CPUIDLE
 */
void cpuidle_pause_and_lock(void)
{
	mutex_lock(&cpuidle_lock);
	cpuidle_uninstall_idle_handler();
}

EXPORT_SYMBOL_GPL(cpuidle_pause_and_lock);

/**
 * cpuidle_resume_and_unlock - resumes CPUIDLE operation
 */
void cpuidle_resume_and_unlock(void)
{
	cpuidle_install_idle_handler();
	mutex_unlock(&cpuidle_lock);
}

EXPORT_SYMBOL_GPL(cpuidle_resume_and_unlock);

<<<<<<< HEAD
/**
 * cpuidle_wrap_enter - performs timekeeping and irqen around enter function
 * @dev: pointer to a valid cpuidle_device object
 * @drv: pointer to a valid cpuidle_driver object
 * @index: index of the target cpuidle state.
 */
int cpuidle_wrap_enter(struct cpuidle_device *dev,
				struct cpuidle_driver *drv, int index,
				int (*enter)(struct cpuidle_device *dev,
					struct cpuidle_driver *drv, int index))
{
	ktime_t time_start, time_end;
	s64 diff;

	time_start = ktime_get();

	index = enter(dev, drv, index);

	time_end = ktime_get();

	local_irq_enable();

	diff = ktime_to_us(ktime_sub(time_end, time_start));
	if (diff > INT_MAX)
		diff = INT_MAX;

	dev->last_residency = (int) diff;

	return index;
}

#ifdef CONFIG_ARCH_HAS_CPU_RELAX
static int poll_idle(struct cpuidle_device *dev,
		struct cpuidle_driver *drv, int index)
{
	ktime_t	t1, t2;
	s64 diff;

	t1 = ktime_get();
	local_irq_enable();
	while (!need_resched())
		cpu_relax();

	t2 = ktime_get();
	diff = ktime_to_us(ktime_sub(t2, t1));
	if (diff > INT_MAX)
		diff = INT_MAX;

	dev->last_residency = (int) diff;

	return index;
}

static void poll_idle_init(struct cpuidle_driver *drv)
{
	struct cpuidle_state *state = &drv->states[0];

	snprintf(state->name, CPUIDLE_NAME_LEN, "POLL");
	snprintf(state->desc, CPUIDLE_DESC_LEN, "CPUIDLE CORE POLL IDLE");
	state->exit_latency = 0;
	state->target_residency = 0;
	state->power_usage = -1;
	state->flags = 0;
	state->enter = poll_idle;
	state->disable = 0;
}
#else
static void poll_idle_init(struct cpuidle_driver *drv) {}
#endif /* CONFIG_ARCH_HAS_CPU_RELAX */

=======
/* Currently used in suspend/resume path to suspend cpuidle */
void cpuidle_pause(void)
{
	mutex_lock(&cpuidle_lock);
	cpuidle_uninstall_idle_handler();
	mutex_unlock(&cpuidle_lock);
}

/* Currently used in suspend/resume path to resume cpuidle */
void cpuidle_resume(void)
{
	mutex_lock(&cpuidle_lock);
	cpuidle_install_idle_handler();
	mutex_unlock(&cpuidle_lock);
}

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
/**
 * cpuidle_enable_device - enables idle PM for a CPU
 * @dev: the CPU
 *
 * This function must be called between cpuidle_pause_and_lock and
 * cpuidle_resume_and_unlock when used externally.
 */
int cpuidle_enable_device(struct cpuidle_device *dev)
{
<<<<<<< HEAD
	int ret, i;
	struct cpuidle_driver *drv = cpuidle_get_driver();

	if (dev->enabled)
		return 0;
	if (!drv || !cpuidle_curr_governor)
		return -EIO;
	if (!dev->state_count)
		dev->state_count = drv->state_count;

	if (dev->registered == 0) {
		ret = __cpuidle_register_device(dev);
		if (ret)
			return ret;
	}

	cpuidle_enter_ops = drv->en_core_tk_irqen ?
		cpuidle_enter_tk : cpuidle_enter;

	poll_idle_init(drv);

	if ((ret = cpuidle_add_state_sysfs(dev)))
		return ret;

	if (cpuidle_curr_governor->enable &&
	    (ret = cpuidle_curr_governor->enable(drv, dev)))
		goto fail_sysfs;

	for (i = 0; i < dev->state_count; i++) {
		dev->states_usage[i].usage = 0;
		dev->states_usage[i].time = 0;
	}
	dev->last_residency = 0;
=======
	int ret;
	struct cpuidle_driver *drv;

	if (!dev)
		return -EINVAL;

	if (dev->enabled)
		return 0;

	if (!cpuidle_curr_governor)
		return -EIO;

	drv = cpuidle_get_cpu_driver(dev);

	if (!drv)
		return -EIO;

	if (!dev->registered)
		return -EINVAL;

	ret = cpuidle_add_device_sysfs(dev);
	if (ret)
		return ret;

	if (cpuidle_curr_governor->enable) {
		ret = cpuidle_curr_governor->enable(drv, dev);
		if (ret)
			goto fail_sysfs;
	}
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	smp_wmb();

	dev->enabled = 1;

	enabled_devices++;
	return 0;

fail_sysfs:
<<<<<<< HEAD
	cpuidle_remove_state_sysfs(dev);
=======
	cpuidle_remove_device_sysfs(dev);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	return ret;
}

EXPORT_SYMBOL_GPL(cpuidle_enable_device);

/**
 * cpuidle_disable_device - disables idle PM for a CPU
 * @dev: the CPU
 *
 * This function must be called between cpuidle_pause_and_lock and
 * cpuidle_resume_and_unlock when used externally.
 */
void cpuidle_disable_device(struct cpuidle_device *dev)
{
<<<<<<< HEAD
	if (!dev->enabled)
		return;
	if (!cpuidle_get_driver() || !cpuidle_curr_governor)
=======
	struct cpuidle_driver *drv = cpuidle_get_cpu_driver(dev);

	if (!dev || !dev->enabled)
		return;

	if (!drv || !cpuidle_curr_governor)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		return;

	dev->enabled = 0;

	if (cpuidle_curr_governor->disable)
<<<<<<< HEAD
		cpuidle_curr_governor->disable(cpuidle_get_driver(), dev);

	cpuidle_remove_state_sysfs(dev);
=======
		cpuidle_curr_governor->disable(drv, dev);

	cpuidle_remove_device_sysfs(dev);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	enabled_devices--;
}

EXPORT_SYMBOL_GPL(cpuidle_disable_device);

<<<<<<< HEAD
=======
static void __cpuidle_unregister_device(struct cpuidle_device *dev)
{
	struct cpuidle_driver *drv = cpuidle_get_cpu_driver(dev);

	list_del(&dev->device_list);
	per_cpu(cpuidle_devices, dev->cpu) = NULL;
	module_put(drv->owner);

	dev->registered = 0;
}

static void __cpuidle_device_init(struct cpuidle_device *dev)
{
	memset(dev->states_usage, 0, sizeof(dev->states_usage));
	dev->last_residency_ns = 0;
	dev->next_hrtimer = 0;
}

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
/**
 * __cpuidle_register_device - internal register function called before register
 * and enable routines
 * @dev: the cpu
 *
 * cpuidle_lock mutex must be held before this is called
 */
static int __cpuidle_register_device(struct cpuidle_device *dev)
{
<<<<<<< HEAD
	int ret;
	struct device *cpu_dev = get_cpu_device((unsigned long)dev->cpu);
	struct cpuidle_driver *cpuidle_driver = cpuidle_get_driver();

	if (!dev)
		return -EINVAL;
	if (!try_module_get(cpuidle_driver->owner))
		return -EINVAL;

	init_completion(&dev->kobj_unregister);

	per_cpu(cpuidle_devices, dev->cpu) = dev;
	list_add(&dev->device_list, &cpuidle_detected_devices);
	ret = cpuidle_add_sysfs(cpu_dev);
	if (ret)
		goto err_sysfs;

	ret = cpuidle_coupled_register_device(dev);
	if (ret)
		goto err_coupled;

	dev->registered = 1;
	return 0;

err_coupled:
	cpuidle_remove_sysfs(cpu_dev);
	wait_for_completion(&dev->kobj_unregister);
err_sysfs:
	list_del(&dev->device_list);
	per_cpu(cpuidle_devices, dev->cpu) = NULL;
	module_put(cpuidle_driver->owner);
=======
	struct cpuidle_driver *drv = cpuidle_get_cpu_driver(dev);
	int i, ret;

	if (!try_module_get(drv->owner))
		return -EINVAL;

	for (i = 0; i < drv->state_count; i++) {
		if (drv->states[i].flags & CPUIDLE_FLAG_UNUSABLE)
			dev->states_usage[i].disable |= CPUIDLE_STATE_DISABLED_BY_DRIVER;

		if (drv->states[i].flags & CPUIDLE_FLAG_OFF)
			dev->states_usage[i].disable |= CPUIDLE_STATE_DISABLED_BY_USER;
	}

	per_cpu(cpuidle_devices, dev->cpu) = dev;
	list_add(&dev->device_list, &cpuidle_detected_devices);

	ret = cpuidle_coupled_register_device(dev);
	if (ret)
		__cpuidle_unregister_device(dev);
	else
		dev->registered = 1;

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	return ret;
}

/**
 * cpuidle_register_device - registers a CPU's idle PM feature
 * @dev: the cpu
 */
int cpuidle_register_device(struct cpuidle_device *dev)
{
<<<<<<< HEAD
	int ret;

	mutex_lock(&cpuidle_lock);

	if ((ret = __cpuidle_register_device(dev))) {
		mutex_unlock(&cpuidle_lock);
		return ret;
	}

	cpuidle_enable_device(dev);
	cpuidle_install_idle_handler();

	mutex_unlock(&cpuidle_lock);

	return 0;

=======
	int ret = -EBUSY;

	if (!dev)
		return -EINVAL;

	mutex_lock(&cpuidle_lock);

	if (dev->registered)
		goto out_unlock;

	__cpuidle_device_init(dev);

	ret = __cpuidle_register_device(dev);
	if (ret)
		goto out_unlock;

	ret = cpuidle_add_sysfs(dev);
	if (ret)
		goto out_unregister;

	ret = cpuidle_enable_device(dev);
	if (ret)
		goto out_sysfs;

	cpuidle_install_idle_handler();

out_unlock:
	mutex_unlock(&cpuidle_lock);

	return ret;

out_sysfs:
	cpuidle_remove_sysfs(dev);
out_unregister:
	__cpuidle_unregister_device(dev);
	goto out_unlock;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

EXPORT_SYMBOL_GPL(cpuidle_register_device);

/**
 * cpuidle_unregister_device - unregisters a CPU's idle PM feature
 * @dev: the cpu
 */
void cpuidle_unregister_device(struct cpuidle_device *dev)
{
<<<<<<< HEAD
	struct device *cpu_dev = get_cpu_device((unsigned long)dev->cpu);
	struct cpuidle_driver *cpuidle_driver = cpuidle_get_driver();

	if (dev->registered == 0)
=======
	if (!dev || dev->registered == 0)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		return;

	cpuidle_pause_and_lock();

	cpuidle_disable_device(dev);

<<<<<<< HEAD
	cpuidle_remove_sysfs(cpu_dev);
	list_del(&dev->device_list);
	wait_for_completion(&dev->kobj_unregister);
	per_cpu(cpuidle_devices, dev->cpu) = NULL;
=======
	cpuidle_remove_sysfs(dev);

	__cpuidle_unregister_device(dev);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	cpuidle_coupled_unregister_device(dev);

	cpuidle_resume_and_unlock();
<<<<<<< HEAD

	module_put(cpuidle_driver->owner);
=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

EXPORT_SYMBOL_GPL(cpuidle_unregister_device);

<<<<<<< HEAD
/*
=======
/**
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 * cpuidle_unregister: unregister a driver and the devices. This function
 * can be used only if the driver has been previously registered through
 * the cpuidle_register function.
 *
 * @drv: a valid pointer to a struct cpuidle_driver
 */
void cpuidle_unregister(struct cpuidle_driver *drv)
{
	int cpu;
	struct cpuidle_device *device;

<<<<<<< HEAD
	for_each_possible_cpu(cpu) {
=======
	for_each_cpu(cpu, drv->cpumask) {
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		device = &per_cpu(cpuidle_dev, cpu);
		cpuidle_unregister_device(device);
	}

	cpuidle_unregister_driver(drv);
}
EXPORT_SYMBOL_GPL(cpuidle_unregister);

/**
 * cpuidle_register: registers the driver and the cpu devices with the
 * coupled_cpus passed as parameter. This function is used for all common
 * initialization pattern there are in the arch specific drivers. The
 * devices is globally defined in this file.
 *
 * @drv         : a valid pointer to a struct cpuidle_driver
 * @coupled_cpus: a cpumask for the coupled states
 *
 * Returns 0 on success, < 0 otherwise
 */
int cpuidle_register(struct cpuidle_driver *drv,
		     const struct cpumask *const coupled_cpus)
{
	int ret, cpu;
	struct cpuidle_device *device;

	ret = cpuidle_register_driver(drv);
	if (ret) {
		pr_err("failed to register cpuidle driver\n");
		return ret;
	}

<<<<<<< HEAD
	for_each_possible_cpu(cpu) {
=======
	for_each_cpu(cpu, drv->cpumask) {
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		device = &per_cpu(cpuidle_dev, cpu);
		device->cpu = cpu;

#ifdef CONFIG_ARCH_NEEDS_CPU_IDLE_COUPLED
		/*
<<<<<<< HEAD
		 * On multiplatform for ARM, the coupled idle states could
=======
		 * On multiplatform for ARM, the coupled idle states could be
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		 * enabled in the kernel even if the cpuidle driver does not
		 * use it. Note, coupled_cpus is a struct copy.
		 */
		if (coupled_cpus)
			device->coupled_cpus = *coupled_cpus;
#endif
		ret = cpuidle_register_device(device);
		if (!ret)
			continue;

		pr_err("Failed to register cpuidle device for cpu%d\n", cpu);

		cpuidle_unregister(drv);
		break;
	}

	return ret;
}
EXPORT_SYMBOL_GPL(cpuidle_register);

<<<<<<< HEAD
#ifdef CONFIG_SMP

static void smp_callback(void *v)
{
	/* we already woke the CPU up, nothing more to do */
}

/*
 * This function gets called when a part of the kernel has a new latency
 * requirement.  This means we need to get all processors out of their C-state,
 * and then recalculate a new suitable C-state. Just do a cross-cpu IPI; that
 * wakes them all right up.
 */
static int cpuidle_latency_notify(struct notifier_block *b,
		unsigned long l, void *v)
{
	smp_call_function(smp_callback, NULL, 1);
	return NOTIFY_OK;
}

static struct notifier_block cpuidle_latency_notifier = {
	.notifier_call = cpuidle_latency_notify,
};

static inline void latency_notifier_init(struct notifier_block *n)
{
	pm_qos_add_notifier(PM_QOS_CPU_DMA_LATENCY, n);
}

#else /* CONFIG_SMP */

#define latency_notifier_init(x) do { } while (0)

#endif /* CONFIG_SMP */

=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
/**
 * cpuidle_init - core initializer
 */
static int __init cpuidle_init(void)
{
<<<<<<< HEAD
	int ret;

	if (cpuidle_disabled())
		return -ENODEV;

	ret = cpuidle_add_interface(cpu_subsys.dev_root);
	if (ret)
		return ret;

	latency_notifier_init(&cpuidle_latency_notifier);

	return 0;
}

module_param(off, int, 0444);
=======
	if (cpuidle_disabled())
		return -ENODEV;

	return cpuidle_add_interface();
}

module_param(off, int, 0444);
module_param_string(governor, param_governor, CPUIDLE_NAME_LEN, 0444);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
core_initcall(cpuidle_init);
