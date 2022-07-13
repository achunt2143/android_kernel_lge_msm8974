/*
 * ladder.c - the residency ladder algorithm
 *
 *  Copyright (C) 2001, 2002 Andy Grover <andrew.grover@intel.com>
 *  Copyright (C) 2001, 2002 Paul Diefenbaugh <paul.s.diefenbaugh@intel.com>
 *  Copyright (C) 2004, 2005 Dominik Brodowski <linux@brodo.de>
 *
 * (C) 2006-2007 Venkatesh Pallipadi <venkatesh.pallipadi@intel.com>
 *               Shaohua Li <shaohua.li@intel.com>
 *               Adam Belay <abelay@novell.com>
 *
 * This code is licenced under the GPL.
 */

#include <linux/kernel.h>
#include <linux/cpuidle.h>
<<<<<<< HEAD
#include <linux/pm_qos.h>
#include <linux/module.h>
#include <linux/jiffies.h>

#include <asm/io.h>
#include <asm/uaccess.h>
=======
#include <linux/jiffies.h>
#include <linux/tick.h>

#include <asm/io.h>
#include <linux/uaccess.h>
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

#define PROMOTION_COUNT 4
#define DEMOTION_COUNT 1

struct ladder_device_state {
	struct {
		u32 promotion_count;
		u32 demotion_count;
<<<<<<< HEAD
		u32 promotion_time;
		u32 demotion_time;
=======
		u64 promotion_time_ns;
		u64 demotion_time_ns;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	} threshold;
	struct {
		int promotion_count;
		int demotion_count;
	} stats;
};

struct ladder_device {
	struct ladder_device_state states[CPUIDLE_STATE_MAX];
<<<<<<< HEAD
	int last_state_idx;
=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
};

static DEFINE_PER_CPU(struct ladder_device, ladder_devices);

/**
 * ladder_do_selection - prepares private data for a state change
 * @ldev: the ladder device
 * @old_idx: the current state index
 * @new_idx: the new target state index
 */
<<<<<<< HEAD
static inline void ladder_do_selection(struct ladder_device *ldev,
=======
static inline void ladder_do_selection(struct cpuidle_device *dev,
				       struct ladder_device *ldev,
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
				       int old_idx, int new_idx)
{
	ldev->states[old_idx].stats.promotion_count = 0;
	ldev->states[old_idx].stats.demotion_count = 0;
<<<<<<< HEAD
	ldev->last_state_idx = new_idx;
=======
	dev->last_state_idx = new_idx;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

/**
 * ladder_select_state - selects the next state to enter
 * @drv: cpuidle driver
 * @dev: the CPU
<<<<<<< HEAD
 */
static int ladder_select_state(struct cpuidle_driver *drv,
				struct cpuidle_device *dev)
{
	struct ladder_device *ldev = &__get_cpu_var(ladder_devices);
	struct ladder_device_state *last_state;
	int last_residency, last_idx = ldev->last_state_idx;
	int latency_req = pm_qos_request(PM_QOS_CPU_DMA_LATENCY);

	/* Special case when user has set very strict latency requirement */
	if (unlikely(latency_req == 0)) {
		ladder_do_selection(ldev, last_idx, 0);
=======
 * @dummy: not used
 */
static int ladder_select_state(struct cpuidle_driver *drv,
			       struct cpuidle_device *dev, bool *dummy)
{
	struct ladder_device *ldev = this_cpu_ptr(&ladder_devices);
	struct ladder_device_state *last_state;
	int last_idx = dev->last_state_idx;
	int first_idx = drv->states[0].flags & CPUIDLE_FLAG_POLLING ? 1 : 0;
	s64 latency_req = cpuidle_governor_latency_req(dev->cpu);
	s64 last_residency;

	/* Special case when user has set very strict latency requirement */
	if (unlikely(latency_req == 0)) {
		ladder_do_selection(dev, ldev, last_idx, 0);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		return 0;
	}

	last_state = &ldev->states[last_idx];

<<<<<<< HEAD
	if (drv->states[last_idx].flags & CPUIDLE_FLAG_TIME_VALID) {
		last_residency = cpuidle_get_last_residency(dev) - \
					 drv->states[last_idx].exit_latency;
	}
	else
		last_residency = last_state->threshold.promotion_time + 1;

	/* consider promotion */
	if (last_idx < drv->state_count - 1 &&
	    last_residency > last_state->threshold.promotion_time &&
	    drv->states[last_idx + 1].exit_latency <= latency_req) {
		last_state->stats.promotion_count++;
		last_state->stats.demotion_count = 0;
		if (last_state->stats.promotion_count >= last_state->threshold.promotion_count) {
			ladder_do_selection(ldev, last_idx, last_idx + 1);
=======
	last_residency = dev->last_residency_ns - drv->states[last_idx].exit_latency_ns;

	/* consider promotion */
	if (last_idx < drv->state_count - 1 &&
	    !dev->states_usage[last_idx + 1].disable &&
	    last_residency > last_state->threshold.promotion_time_ns &&
	    drv->states[last_idx + 1].exit_latency_ns <= latency_req) {
		last_state->stats.promotion_count++;
		last_state->stats.demotion_count = 0;
		if (last_state->stats.promotion_count >= last_state->threshold.promotion_count) {
			ladder_do_selection(dev, ldev, last_idx, last_idx + 1);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
			return last_idx + 1;
		}
	}

	/* consider demotion */
<<<<<<< HEAD
	if (last_idx > CPUIDLE_DRIVER_STATE_START &&
	    drv->states[last_idx].exit_latency > latency_req) {
		int i;

		for (i = last_idx - 1; i > CPUIDLE_DRIVER_STATE_START; i--) {
			if (drv->states[i].exit_latency <= latency_req)
				break;
		}
		ladder_do_selection(ldev, last_idx, i);
		return i;
	}

	if (last_idx > CPUIDLE_DRIVER_STATE_START &&
	    last_residency < last_state->threshold.demotion_time) {
		last_state->stats.demotion_count++;
		last_state->stats.promotion_count = 0;
		if (last_state->stats.demotion_count >= last_state->threshold.demotion_count) {
			ladder_do_selection(ldev, last_idx, last_idx - 1);
=======
	if (last_idx > first_idx &&
	    (dev->states_usage[last_idx].disable ||
	    drv->states[last_idx].exit_latency_ns > latency_req)) {
		int i;

		for (i = last_idx - 1; i > first_idx; i--) {
			if (drv->states[i].exit_latency_ns <= latency_req)
				break;
		}
		ladder_do_selection(dev, ldev, last_idx, i);
		return i;
	}

	if (last_idx > first_idx &&
	    last_residency < last_state->threshold.demotion_time_ns) {
		last_state->stats.demotion_count++;
		last_state->stats.promotion_count = 0;
		if (last_state->stats.demotion_count >= last_state->threshold.demotion_count) {
			ladder_do_selection(dev, ldev, last_idx, last_idx - 1);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
			return last_idx - 1;
		}
	}

	/* otherwise remain at the current state */
	return last_idx;
}

/**
 * ladder_enable_device - setup for the governor
 * @drv: cpuidle driver
 * @dev: the CPU
 */
static int ladder_enable_device(struct cpuidle_driver *drv,
				struct cpuidle_device *dev)
{
	int i;
<<<<<<< HEAD
=======
	int first_idx = drv->states[0].flags & CPUIDLE_FLAG_POLLING ? 1 : 0;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	struct ladder_device *ldev = &per_cpu(ladder_devices, dev->cpu);
	struct ladder_device_state *lstate;
	struct cpuidle_state *state;

<<<<<<< HEAD
	ldev->last_state_idx = CPUIDLE_DRIVER_STATE_START;

	for (i = 0; i < drv->state_count; i++) {
=======
	dev->last_state_idx = first_idx;

	for (i = first_idx; i < drv->state_count; i++) {
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		state = &drv->states[i];
		lstate = &ldev->states[i];

		lstate->stats.promotion_count = 0;
		lstate->stats.demotion_count = 0;

		lstate->threshold.promotion_count = PROMOTION_COUNT;
		lstate->threshold.demotion_count = DEMOTION_COUNT;

		if (i < drv->state_count - 1)
<<<<<<< HEAD
			lstate->threshold.promotion_time = state->exit_latency;
		if (i > 0)
			lstate->threshold.demotion_time = state->exit_latency;
=======
			lstate->threshold.promotion_time_ns = state->exit_latency_ns;
		if (i > first_idx)
			lstate->threshold.demotion_time_ns = state->exit_latency_ns;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	}

	return 0;
}

/**
 * ladder_reflect - update the correct last_state_idx
 * @dev: the CPU
 * @index: the index of actual state entered
 */
static void ladder_reflect(struct cpuidle_device *dev, int index)
{
<<<<<<< HEAD
	struct ladder_device *ldev = &__get_cpu_var(ladder_devices);
	if (index > 0)
		ldev->last_state_idx = index;
=======
	if (index > 0)
		dev->last_state_idx = index;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

static struct cpuidle_governor ladder_governor = {
	.name =		"ladder",
	.rating =	10,
	.enable =	ladder_enable_device,
	.select =	ladder_select_state,
	.reflect =	ladder_reflect,
<<<<<<< HEAD
	.owner =	THIS_MODULE,
=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
};

/**
 * init_ladder - initializes the governor
 */
static int __init init_ladder(void)
{
<<<<<<< HEAD
	return cpuidle_register_governor(&ladder_governor);
}

/**
 * exit_ladder - exits the governor
 */
static void __exit exit_ladder(void)
{
	cpuidle_unregister_governor(&ladder_governor);
}

MODULE_LICENSE("GPL");
module_init(init_ladder);
module_exit(exit_ladder);
=======
	/*
	 * When NO_HZ is disabled, or when booting with nohz=off, the ladder
	 * governor is better so give it a higher rating than the menu
	 * governor.
	 */
	if (!tick_nohz_enabled)
		ladder_governor.rating = 25;

	return cpuidle_register_governor(&ladder_governor);
}

postcore_initcall(init_ladder);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
