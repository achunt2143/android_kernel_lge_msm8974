<<<<<<< HEAD
/*
 * OMAP4 CPU idle Routines
 *
 * Copyright (C) 2011 Texas Instruments, Inc.
 * Santosh Shilimkar <santosh.shilimkar@ti.com>
 * Rajendra Nayak <rnayak@ti.com>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
=======
// SPDX-License-Identifier: GPL-2.0-only
/*
 * OMAP4+ CPU idle Routines
 *
 * Copyright (C) 2011-2013 Texas Instruments, Inc.
 * Santosh Shilimkar <santosh.shilimkar@ti.com>
 * Rajendra Nayak <rnayak@ti.com>
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 */

#include <linux/sched.h>
#include <linux/cpuidle.h>
#include <linux/cpu_pm.h>
#include <linux/export.h>
<<<<<<< HEAD
#include <linux/clockchips.h>

#include <asm/proc-fns.h>
=======
#include <linux/tick.h>

#include <asm/cpuidle.h>
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

#include "common.h"
#include "pm.h"
#include "prm.h"
<<<<<<< HEAD

#ifdef CONFIG_CPU_IDLE

/* Machine specific information to be recorded in the C-state driver_data */
struct omap4_idle_statedata {
	u32 cpu_state;
	u32 mpu_logic_state;
	u32 mpu_state;
	u8 valid;
};

static struct cpuidle_params cpuidle_params_table[] = {
	/* C1 - CPU0 ON + CPU1 ON + MPU ON */
	{.exit_latency = 2 + 2 , .target_residency = 5, .valid = 1},
	/* C2- CPU0 OFF + CPU1 OFF + MPU CSWR */
	{.exit_latency = 328 + 440 , .target_residency = 960, .valid = 1},
	/* C3 - CPU0 OFF + CPU1 OFF + MPU OSWR */
	{.exit_latency = 460 + 518 , .target_residency = 1100, .valid = 1},
};

#define OMAP4_NUM_STATES ARRAY_SIZE(cpuidle_params_table)

struct omap4_idle_statedata omap4_idle_data[OMAP4_NUM_STATES];
static struct powerdomain *mpu_pd, *cpu0_pd, *cpu1_pd;

/**
 * omap4_enter_idle - Programs OMAP4 to enter the specified state
=======
#include "soc.h"
#include "clockdomain.h"

#define MAX_CPUS	2

/* Machine specific information */
struct idle_statedata {
	u32 cpu_state;
	u32 mpu_logic_state;
	u32 mpu_state;
	u32 mpu_state_vote;
};

static struct idle_statedata omap4_idle_data[] = {
	{
		.cpu_state = PWRDM_POWER_ON,
		.mpu_state = PWRDM_POWER_ON,
		.mpu_logic_state = PWRDM_POWER_RET,
	},
	{
		.cpu_state = PWRDM_POWER_OFF,
		.mpu_state = PWRDM_POWER_RET,
		.mpu_logic_state = PWRDM_POWER_RET,
	},
	{
		.cpu_state = PWRDM_POWER_OFF,
		.mpu_state = PWRDM_POWER_RET,
		.mpu_logic_state = PWRDM_POWER_OFF,
	},
};

static struct idle_statedata omap5_idle_data[] = {
	{
		.cpu_state = PWRDM_POWER_ON,
		.mpu_state = PWRDM_POWER_ON,
		.mpu_logic_state = PWRDM_POWER_ON,
	},
	{
		.cpu_state = PWRDM_POWER_RET,
		.mpu_state = PWRDM_POWER_RET,
		.mpu_logic_state = PWRDM_POWER_RET,
	},
};

static struct powerdomain *mpu_pd, *cpu_pd[MAX_CPUS];
static struct clockdomain *cpu_clkdm[MAX_CPUS];

static atomic_t abort_barrier;
static bool cpu_done[MAX_CPUS];
static struct idle_statedata *state_ptr = &omap4_idle_data[0];
static DEFINE_RAW_SPINLOCK(mpu_lock);

/* Private functions */

/**
 * omap_enter_idle_[simple/coupled] - OMAP4PLUS cpuidle entry functions
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 * @dev: cpuidle device
 * @drv: cpuidle driver
 * @index: the index of state to be entered
 *
 * Called from the CPUidle framework to program the device to the
 * specified low power state selected by the governor.
 * Returns the amount of time spent in the low power state.
 */
<<<<<<< HEAD
static int omap4_enter_idle(struct cpuidle_device *dev,
			struct cpuidle_driver *drv,
			int index)
{
	struct omap4_idle_statedata *cx =
			cpuidle_get_statedata(&dev->states_usage[index]);
	u32 cpu1_state;
	int cpu_id = smp_processor_id();

	local_fiq_disable();

	/*
	 * CPU0 has to stay ON (i.e in C1) until CPU1 is OFF state.
	 * This is necessary to honour hardware recommondation
	 * of triggeing all the possible low power modes once CPU1 is
	 * out of coherency and in OFF mode.
	 * Update dev->last_state so that governor stats reflects right
	 * data.
	 */
	cpu1_state = pwrdm_read_pwrst(cpu1_pd);
	if (cpu1_state != PWRDM_POWER_OFF) {
		index = drv->safe_state_index;
		cx = cpuidle_get_statedata(&dev->states_usage[index]);
	}

	if (index > 0)
		clockevents_notify(CLOCK_EVT_NOTIFY_BROADCAST_ENTER, &cpu_id);
=======
static int omap_enter_idle_simple(struct cpuidle_device *dev,
			struct cpuidle_driver *drv,
			int index)
{
	omap_do_wfi();
	return index;
}

static int omap_enter_idle_smp(struct cpuidle_device *dev,
			       struct cpuidle_driver *drv,
			       int index)
{
	struct idle_statedata *cx = state_ptr + index;
	unsigned long flag;

	raw_spin_lock_irqsave(&mpu_lock, flag);
	cx->mpu_state_vote++;
	if (cx->mpu_state_vote == num_online_cpus()) {
		pwrdm_set_logic_retst(mpu_pd, cx->mpu_logic_state);
		omap_set_pwrdm_state(mpu_pd, cx->mpu_state);
	}
	raw_spin_unlock_irqrestore(&mpu_lock, flag);

	omap4_enter_lowpower(dev->cpu, cx->cpu_state, true);

	raw_spin_lock_irqsave(&mpu_lock, flag);
	if (cx->mpu_state_vote == num_online_cpus())
		omap_set_pwrdm_state(mpu_pd, PWRDM_POWER_ON);
	cx->mpu_state_vote--;
	raw_spin_unlock_irqrestore(&mpu_lock, flag);

	return index;
}

static int omap_enter_idle_coupled(struct cpuidle_device *dev,
			struct cpuidle_driver *drv,
			int index)
{
	struct idle_statedata *cx = state_ptr + index;
	u32 mpuss_can_lose_context = 0;
	int error;

	/*
	 * CPU0 has to wait and stay ON until CPU1 is OFF state.
	 * This is necessary to honour hardware recommondation
	 * of triggeing all the possible low power modes once CPU1 is
	 * out of coherency and in OFF mode.
	 */
	if (dev->cpu == 0 && cpumask_test_cpu(1, cpu_online_mask)) {
		while (pwrdm_read_pwrst(cpu_pd[1]) != PWRDM_POWER_OFF) {
			cpu_relax();

			/*
			 * CPU1 could have already entered & exited idle
			 * without hitting off because of a wakeup
			 * or a failed attempt to hit off mode.  Check for
			 * that here, otherwise we could spin forever
			 * waiting for CPU1 off.
			 */
			if (cpu_done[1])
			    goto fail;

		}
	}

	mpuss_can_lose_context = (cx->mpu_state == PWRDM_POWER_RET) &&
				 (cx->mpu_logic_state == PWRDM_POWER_OFF);

	/* Enter broadcast mode for periodic timers */
	tick_broadcast_enable();

	/* Enter broadcast mode for one-shot timers */
	tick_broadcast_enter();
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	/*
	 * Call idle CPU PM enter notifier chain so that
	 * VFP and per CPU interrupt context is saved.
	 */
<<<<<<< HEAD
	if (cx->cpu_state == PWRDM_POWER_OFF)
		cpu_pm_enter();

	pwrdm_set_logic_retst(mpu_pd, cx->mpu_logic_state);
	omap_set_pwrdm_state(mpu_pd, cx->mpu_state);

	/*
	 * Call idle CPU cluster PM enter notifier chain
	 * to save GIC and wakeupgen context.
	 */
	if ((cx->mpu_state == PWRDM_POWER_RET) &&
		(cx->mpu_logic_state == PWRDM_POWER_OFF))
			cpu_cluster_pm_enter();

	omap4_enter_lowpower(dev->cpu, cx->cpu_state);

	/*
	 * Call idle CPU PM exit notifier chain to restore
	 * VFP and per CPU IRQ context. Only CPU0 state is
	 * considered since CPU1 is managed by CPU hotplug.
	 */
	if (pwrdm_read_prev_pwrst(cpu0_pd) == PWRDM_POWER_OFF)
		cpu_pm_exit();
=======
	error = cpu_pm_enter();
	if (error)
		goto cpu_pm_out;

	if (dev->cpu == 0) {
		pwrdm_set_logic_retst(mpu_pd, cx->mpu_logic_state);
		omap_set_pwrdm_state(mpu_pd, cx->mpu_state);

		/*
		 * Call idle CPU cluster PM enter notifier chain
		 * to save GIC and wakeupgen context.
		 */
		if (mpuss_can_lose_context) {
			error = cpu_cluster_pm_enter();
			if (error) {
				index = 0;
				cx = state_ptr + index;
				pwrdm_set_logic_retst(mpu_pd, cx->mpu_logic_state);
				omap_set_pwrdm_state(mpu_pd, cx->mpu_state);
				mpuss_can_lose_context = 0;
			}
		}
	}

	omap4_enter_lowpower(dev->cpu, cx->cpu_state, true);
	cpu_done[dev->cpu] = true;

	/* Wakeup CPU1 only if it is not offlined */
	if (dev->cpu == 0 && cpumask_test_cpu(1, cpu_online_mask)) {

		if (IS_PM44XX_ERRATUM(PM_OMAP4_ROM_SMP_BOOT_ERRATUM_GICD) &&
		    mpuss_can_lose_context)
			gic_dist_disable();

		clkdm_deny_idle(cpu_clkdm[1]);
		omap_set_pwrdm_state(cpu_pd[1], PWRDM_POWER_ON);
		clkdm_allow_idle(cpu_clkdm[1]);

		if (IS_PM44XX_ERRATUM(PM_OMAP4_ROM_SMP_BOOT_ERRATUM_GICD) &&
		    mpuss_can_lose_context) {
			while (gic_dist_disabled()) {
				udelay(1);
				cpu_relax();
			}
			gic_timer_retrigger();
		}
	}
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	/*
	 * Call idle CPU cluster PM exit notifier chain
	 * to restore GIC and wakeupgen context.
	 */
<<<<<<< HEAD
	if (omap4_mpuss_read_prev_context_state())
		cpu_cluster_pm_exit();

	if (index > 0)
		clockevents_notify(CLOCK_EVT_NOTIFY_BROADCAST_EXIT, &cpu_id);

	local_fiq_enable();
=======
	if (dev->cpu == 0 && mpuss_can_lose_context)
		cpu_cluster_pm_exit();

	/*
	 * Call idle CPU PM exit notifier chain to restore
	 * VFP and per CPU IRQ context.
	 */
	cpu_pm_exit();

cpu_pm_out:
	tick_broadcast_exit();

fail:
	cpuidle_coupled_parallel_barrier(dev, &abort_barrier);
	cpu_done[dev->cpu] = false;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	return index;
}

<<<<<<< HEAD
DEFINE_PER_CPU(struct cpuidle_device, omap4_idle_dev);

struct cpuidle_driver omap4_idle_driver = {
	.name				= "omap4_idle",
	.owner				= THIS_MODULE,
	.en_core_tk_irqen		= 1,
};

static inline void _fill_cstate(struct cpuidle_driver *drv,
					int idx, const char *descr)
{
	struct cpuidle_state *state = &drv->states[idx];

	state->exit_latency	= cpuidle_params_table[idx].exit_latency;
	state->target_residency	= cpuidle_params_table[idx].target_residency;
	state->flags		= CPUIDLE_FLAG_TIME_VALID;
	state->enter		= omap4_enter_idle;
	sprintf(state->name, "C%d", idx + 1);
	strncpy(state->desc, descr, CPUIDLE_DESC_LEN);
}

static inline struct omap4_idle_statedata *_fill_cstate_usage(
					struct cpuidle_device *dev,
					int idx)
{
	struct omap4_idle_statedata *cx = &omap4_idle_data[idx];
	struct cpuidle_state_usage *state_usage = &dev->states_usage[idx];

	cx->valid		= cpuidle_params_table[idx].valid;
	cpuidle_set_statedata(state_usage, cx);

	return cx;
}



/**
 * omap4_idle_init - Init routine for OMAP4 idle
 *
 * Registers the OMAP4 specific cpuidle driver to the cpuidle
=======
static struct cpuidle_driver omap4_idle_driver = {
	.name				= "omap4_idle",
	.owner				= THIS_MODULE,
	.states = {
		{
			/* C1 - CPU0 ON + CPU1 ON + MPU ON */
			.exit_latency = 2 + 2,
			.target_residency = 5,
			.enter = omap_enter_idle_simple,
			.name = "C1",
			.desc = "CPUx ON, MPUSS ON"
		},
		{
			/* C2 - CPU0 OFF + CPU1 OFF + MPU CSWR */
			.exit_latency = 328 + 440,
			.target_residency = 960,
			.flags = CPUIDLE_FLAG_COUPLED |
				 CPUIDLE_FLAG_RCU_IDLE,
			.enter = omap_enter_idle_coupled,
			.name = "C2",
			.desc = "CPUx OFF, MPUSS CSWR",
		},
		{
			/* C3 - CPU0 OFF + CPU1 OFF + MPU OSWR */
			.exit_latency = 460 + 518,
			.target_residency = 1100,
			.flags = CPUIDLE_FLAG_COUPLED |
				 CPUIDLE_FLAG_RCU_IDLE,
			.enter = omap_enter_idle_coupled,
			.name = "C3",
			.desc = "CPUx OFF, MPUSS OSWR",
		},
	},
	.state_count = ARRAY_SIZE(omap4_idle_data),
	.safe_state_index = 0,
};

static struct cpuidle_driver omap5_idle_driver = {
	.name				= "omap5_idle",
	.owner				= THIS_MODULE,
	.states = {
		{
			/* C1 - CPU0 ON + CPU1 ON + MPU ON */
			.exit_latency = 2 + 2,
			.target_residency = 5,
			.enter = omap_enter_idle_simple,
			.name = "C1",
			.desc = "CPUx WFI, MPUSS ON"
		},
		{
			/* C2 - CPU0 RET + CPU1 RET + MPU CSWR */
			.exit_latency = 48 + 60,
			.target_residency = 100,
			.flags = CPUIDLE_FLAG_TIMER_STOP |
				 CPUIDLE_FLAG_RCU_IDLE,
			.enter = omap_enter_idle_smp,
			.name = "C2",
			.desc = "CPUx CSWR, MPUSS CSWR",
		},
	},
	.state_count = ARRAY_SIZE(omap5_idle_data),
	.safe_state_index = 0,
};

/* Public functions */

/**
 * omap4_idle_init - Init routine for OMAP4+ idle
 *
 * Registers the OMAP4+ specific cpuidle driver to the cpuidle
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 * framework with the valid set of states.
 */
int __init omap4_idle_init(void)
{
<<<<<<< HEAD
	struct omap4_idle_statedata *cx;
	struct cpuidle_device *dev;
	struct cpuidle_driver *drv = &omap4_idle_driver;
	unsigned int cpu_id = 0;

	mpu_pd = pwrdm_lookup("mpu_pwrdm");
	cpu0_pd = pwrdm_lookup("cpu0_pwrdm");
	cpu1_pd = pwrdm_lookup("cpu1_pwrdm");
	if ((!mpu_pd) || (!cpu0_pd) || (!cpu1_pd))
		return -ENODEV;


	drv->safe_state_index = -1;
	dev = &per_cpu(omap4_idle_dev, cpu_id);
	dev->cpu = cpu_id;

	/* C1 - CPU0 ON + CPU1 ON + MPU ON */
	_fill_cstate(drv, 0, "MPUSS ON");
	drv->safe_state_index = 0;
	cx = _fill_cstate_usage(dev, 0);
	cx->valid = 1;	/* C1 is always valid */
	cx->cpu_state = PWRDM_POWER_ON;
	cx->mpu_state = PWRDM_POWER_ON;
	cx->mpu_logic_state = PWRDM_POWER_RET;

	/* C2 - CPU0 OFF + CPU1 OFF + MPU CSWR */
	_fill_cstate(drv, 1, "MPUSS CSWR");
	cx = _fill_cstate_usage(dev, 1);
	cx->cpu_state = PWRDM_POWER_OFF;
	cx->mpu_state = PWRDM_POWER_RET;
	cx->mpu_logic_state = PWRDM_POWER_RET;

	/* C3 - CPU0 OFF + CPU1 OFF + MPU OSWR */
	_fill_cstate(drv, 2, "MPUSS OSWR");
	cx = _fill_cstate_usage(dev, 2);
	cx->cpu_state = PWRDM_POWER_OFF;
	cx->mpu_state = PWRDM_POWER_RET;
	cx->mpu_logic_state = PWRDM_POWER_OFF;

	drv->state_count = OMAP4_NUM_STATES;
	cpuidle_register_driver(&omap4_idle_driver);

	dev->state_count = OMAP4_NUM_STATES;
	if (cpuidle_register_device(dev)) {
		pr_err("%s: CPUidle register device failed\n", __func__);
			return -EIO;
		}

	return 0;
}
#else
int __init omap4_idle_init(void)
{
	return 0;
}
#endif /* CONFIG_CPU_IDLE */
=======
	struct cpuidle_driver *idle_driver;

	if (soc_is_omap54xx()) {
		state_ptr = &omap5_idle_data[0];
		idle_driver = &omap5_idle_driver;
	} else {
		state_ptr = &omap4_idle_data[0];
		idle_driver = &omap4_idle_driver;
	}

	mpu_pd = pwrdm_lookup("mpu_pwrdm");
	cpu_pd[0] = pwrdm_lookup("cpu0_pwrdm");
	cpu_pd[1] = pwrdm_lookup("cpu1_pwrdm");
	if ((!mpu_pd) || (!cpu_pd[0]) || (!cpu_pd[1]))
		return -ENODEV;

	cpu_clkdm[0] = clkdm_lookup("mpu0_clkdm");
	cpu_clkdm[1] = clkdm_lookup("mpu1_clkdm");
	if (!cpu_clkdm[0] || !cpu_clkdm[1])
		return -ENODEV;

	return cpuidle_register(idle_driver, cpu_online_mask);
}
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
