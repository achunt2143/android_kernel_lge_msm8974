<<<<<<< HEAD
=======
// SPDX-License-Identifier: GPL-2.0
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
/*
 * arch/sh/kernel/cpu/shmobile/cpuidle.c
 *
 * Cpuidle support code for SuperH Mobile
 *
 *  Copyright (C) 2009 Magnus Damm
<<<<<<< HEAD
 *
 * This file is subject to the terms and conditions of the GNU General Public
 * License.  See the file "COPYING" in the main directory of this archive
 * for more details.
=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 */
#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/io.h>
#include <linux/suspend.h>
#include <linux/cpuidle.h>
#include <linux/export.h>
#include <asm/suspend.h>
<<<<<<< HEAD
#include <asm/uaccess.h>
=======
#include <linux/uaccess.h>
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

static unsigned long cpuidle_mode[] = {
	SUSP_SH_SLEEP, /* regular sleep mode */
	SUSP_SH_SLEEP | SUSP_SH_SF, /* sleep mode + self refresh */
	SUSP_SH_STANDBY | SUSP_SH_SF, /* software standby mode + self refresh */
};

static int cpuidle_sleep_enter(struct cpuidle_device *dev,
				struct cpuidle_driver *drv,
				int index)
{
	unsigned long allowed_mode = SUSP_SH_SLEEP;
	int requested_state = index;
	int allowed_state;
	int k;

	/* convert allowed mode to allowed state */
	for (k = ARRAY_SIZE(cpuidle_mode) - 1; k > 0; k--)
		if (cpuidle_mode[k] == allowed_mode)
			break;

	allowed_state = k;

	/* take the following into account for sleep mode selection:
	 * - allowed_state: best mode allowed by hardware (clock deps)
	 * - requested_state: best mode allowed by software (latencies)
	 */
	k = min_t(int, allowed_state, requested_state);

	sh_mobile_call_standby(cpuidle_mode[k]);

	return k;
}

<<<<<<< HEAD
static struct cpuidle_device cpuidle_dev;
static struct cpuidle_driver cpuidle_driver = {
	.name			= "sh_idle",
	.owner			= THIS_MODULE,
	.en_core_tk_irqen	= 1,
};

void sh_mobile_setup_cpuidle(void)
{
	struct cpuidle_device *dev = &cpuidle_dev;
	struct cpuidle_driver *drv = &cpuidle_driver;
	struct cpuidle_state *state;
	int i;


	for (i = 0; i < CPUIDLE_STATE_MAX; i++) {
		drv->states[i].name[0] = '\0';
		drv->states[i].desc[0] = '\0';
	}

	i = CPUIDLE_DRIVER_STATE_START;

	state = &drv->states[i++];
	snprintf(state->name, CPUIDLE_NAME_LEN, "C1");
	strncpy(state->desc, "SuperH Sleep Mode", CPUIDLE_DESC_LEN);
	state->exit_latency = 1;
	state->target_residency = 1 * 2;
	state->power_usage = 3;
	state->flags = 0;
	state->flags |= CPUIDLE_FLAG_TIME_VALID;
	state->enter = cpuidle_sleep_enter;

	drv->safe_state_index = i-1;

	if (sh_mobile_sleep_supported & SUSP_SH_SF) {
		state = &drv->states[i++];
		snprintf(state->name, CPUIDLE_NAME_LEN, "C2");
		strncpy(state->desc, "SuperH Sleep Mode [SF]",
			CPUIDLE_DESC_LEN);
		state->exit_latency = 100;
		state->target_residency = 1 * 2;
		state->power_usage = 1;
		state->flags = 0;
		state->flags |= CPUIDLE_FLAG_TIME_VALID;
		state->enter = cpuidle_sleep_enter;
	}

	if (sh_mobile_sleep_supported & SUSP_SH_STANDBY) {
		state = &drv->states[i++];
		snprintf(state->name, CPUIDLE_NAME_LEN, "C3");
		strncpy(state->desc, "SuperH Mobile Standby Mode [SF]",
			CPUIDLE_DESC_LEN);
		state->exit_latency = 2300;
		state->target_residency = 1 * 2;
		state->power_usage = 1;
		state->flags = 0;
		state->flags |= CPUIDLE_FLAG_TIME_VALID;
		state->enter = cpuidle_sleep_enter;
	}

	drv->state_count = i;
	dev->state_count = i;

	cpuidle_register_driver(&cpuidle_driver);

	cpuidle_register_device(dev);
=======
static struct cpuidle_driver cpuidle_driver = {
	.name   = "sh_idle",
	.owner  = THIS_MODULE,
	.states = {
		{
			.exit_latency = 1,
			.target_residency = 1 * 2,
			.power_usage = 3,
			.enter = cpuidle_sleep_enter,
			.name = "C1",
			.desc = "SuperH Sleep Mode",
		},
		{
			.exit_latency = 100,
			.target_residency = 1 * 2,
			.power_usage = 1,
			.enter = cpuidle_sleep_enter,
			.name = "C2",
			.desc = "SuperH Sleep Mode [SF]",
			.flags = CPUIDLE_FLAG_UNUSABLE,
		},
		{
			.exit_latency = 2300,
			.target_residency = 1 * 2,
			.power_usage = 1,
			.enter = cpuidle_sleep_enter,
			.name = "C3",
			.desc = "SuperH Mobile Standby Mode [SF]",
			.flags = CPUIDLE_FLAG_UNUSABLE,
		},
	},
	.safe_state_index = 0,
	.state_count = 3,
};

int __init sh_mobile_setup_cpuidle(void)
{
	if (sh_mobile_sleep_supported & SUSP_SH_SF)
		cpuidle_driver.states[1].flags = CPUIDLE_FLAG_NONE;

	if (sh_mobile_sleep_supported & SUSP_SH_STANDBY)
		cpuidle_driver.states[2].flags = CPUIDLE_FLAG_NONE;

	return cpuidle_register(&cpuidle_driver, NULL);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}
