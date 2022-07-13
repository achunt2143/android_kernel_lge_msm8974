<<<<<<< HEAD
/*
 * poweroff.c - sysrq handler to gracefully power down machine.
 *
 * This file is released under the GPL v2
=======
// SPDX-License-Identifier: GPL-2.0-only
/*
 * poweroff.c - sysrq handler to gracefully power down machine.
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 */

#include <linux/kernel.h>
#include <linux/sysrq.h>
#include <linux/init.h>
#include <linux/pm.h>
#include <linux/workqueue.h>
#include <linux/reboot.h>
#include <linux/cpumask.h>

/*
 * When the user hits Sys-Rq o to power down the machine this is the
 * callback we use.
 */

static void do_poweroff(struct work_struct *dummy)
{
	kernel_power_off();
}

static DECLARE_WORK(poweroff_work, do_poweroff);

<<<<<<< HEAD
static void handle_poweroff(int key)
=======
static void handle_poweroff(u8 key)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
{
	/* run sysrq poweroff on boot cpu */
	schedule_work_on(cpumask_first(cpu_online_mask), &poweroff_work);
}

<<<<<<< HEAD
static struct sysrq_key_op	sysrq_poweroff_op = {
	.handler        = handle_poweroff,
	.help_msg       = "powerOff",
=======
static const struct sysrq_key_op	sysrq_poweroff_op = {
	.handler        = handle_poweroff,
	.help_msg       = "poweroff(o)",
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	.action_msg     = "Power Off",
	.enable_mask	= SYSRQ_ENABLE_BOOT,
};

<<<<<<< HEAD
static int pm_sysrq_init(void)
=======
static int __init pm_sysrq_init(void)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
{
	register_sysrq_key('o', &sysrq_poweroff_op);
	return 0;
}

subsys_initcall(pm_sysrq_init);
