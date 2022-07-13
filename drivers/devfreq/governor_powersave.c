<<<<<<< HEAD
=======
// SPDX-License-Identifier: GPL-2.0-only
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
/*
 *  linux/drivers/devfreq/governor_powersave.c
 *
 *  Copyright (C) 2011 Samsung Electronics
 *	MyungJoo Ham <myungjoo.ham@samsung.com>
<<<<<<< HEAD
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 */

#include <linux/devfreq.h>
#include <linux/module.h>
#include "governor.h"

static int devfreq_powersave_func(struct devfreq *df,
<<<<<<< HEAD
				  unsigned long *freq,
				u32 *flag)
=======
				  unsigned long *freq)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
{
	/*
	 * target callback should be able to get ceiling value as
	 * said in devfreq.h
	 */
<<<<<<< HEAD
	*freq = df->min_freq;
=======
	*freq = DEVFREQ_MIN_FREQ;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	return 0;
}

static int devfreq_powersave_handler(struct devfreq *devfreq,
				unsigned int event, void *data)
{
	int ret = 0;

<<<<<<< HEAD
	if (event == DEVFREQ_GOV_START || event == DEVFREQ_GOV_RESUME) {
=======
	if (event == DEVFREQ_GOV_START) {
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		mutex_lock(&devfreq->lock);
		ret = update_devfreq(devfreq);
		mutex_unlock(&devfreq->lock);
	}

	return ret;
}

static struct devfreq_governor devfreq_powersave = {
<<<<<<< HEAD
	.name = "powersave",
=======
	.name = DEVFREQ_GOV_POWERSAVE,
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	.get_target_freq = devfreq_powersave_func,
	.event_handler = devfreq_powersave_handler,
};

static int __init devfreq_powersave_init(void)
{
	return devfreq_add_governor(&devfreq_powersave);
}
subsys_initcall(devfreq_powersave_init);

static void __exit devfreq_powersave_exit(void)
{
	int ret;

	ret = devfreq_remove_governor(&devfreq_powersave);
	if (ret)
		pr_err("%s: failed remove governor %d\n", __func__, ret);

	return;
}
module_exit(devfreq_powersave_exit);
MODULE_LICENSE("GPL");
