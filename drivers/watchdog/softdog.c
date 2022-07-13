<<<<<<< HEAD
=======
// SPDX-License-Identifier: GPL-2.0+
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
/*
 *	SoftDog:	A Software Watchdog Device
 *
 *	(c) Copyright 1996 Alan Cox <alan@lxorguk.ukuu.org.uk>,
 *							All Rights Reserved.
 *
<<<<<<< HEAD
 *	This program is free software; you can redistribute it and/or
 *	modify it under the terms of the GNU General Public License
 *	as published by the Free Software Foundation; either version
 *	2 of the License, or (at your option) any later version.
 *
=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 *	Neither Alan Cox nor CymruNet Ltd. admit liability nor provide
 *	warranty for any of this software. This material is provided
 *	"AS-IS" and at no charge.
 *
 *	(c) Copyright 1995    Alan Cox <alan@lxorguk.ukuu.org.uk>
 *
 *	Software only watchdog driver. Unlike its big brother the WDT501P
 *	driver this won't always recover a failed machine.
<<<<<<< HEAD
 *
 *  03/96: Angelo Haritsis <ah@doc.ic.ac.uk> :
 *	Modularised.
 *	Added soft_margin; use upon insmod to change the timer delay.
 *	NB: uses same minor as wdt (WATCHDOG_MINOR); we could use separate
 *	    minors.
 *
 *  19980911 Alan Cox
 *	Made SMP safe for 2.3.x
 *
 *  20011127 Joel Becker (jlbec@evilplan.org>
 *	Added soft_noboot; Allows testing the softdog trigger without
 *	requiring a recompile.
 *	Added WDIOC_GETTIMEOUT and WDIOC_SETTIMOUT.
 *
 *  20020530 Joel Becker <joel.becker@oracle.com>
 *	Added Matt Domsch's nowayout module option.
=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 */

#define pr_fmt(fmt) KBUILD_MODNAME ": " fmt

<<<<<<< HEAD
#include <linux/module.h>
#include <linux/moduleparam.h>
#include <linux/types.h>
#include <linux/timer.h>
#include <linux/miscdevice.h>
#include <linux/watchdog.h>
#include <linux/notifier.h>
#include <linux/reboot.h>
#include <linux/init.h>
#include <linux/jiffies.h>
#include <linux/kernel.h>
=======
#include <linux/hrtimer.h>
#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/kthread.h>
#include <linux/module.h>
#include <linux/moduleparam.h>
#include <linux/reboot.h>
#include <linux/types.h>
#include <linux/watchdog.h>
#include <linux/workqueue.h>
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

#define TIMER_MARGIN	60		/* Default is 60 seconds */
static unsigned int soft_margin = TIMER_MARGIN;	/* in seconds */
module_param(soft_margin, uint, 0);
MODULE_PARM_DESC(soft_margin,
	"Watchdog soft_margin in seconds. (0 < soft_margin < 65536, default="
					__MODULE_STRING(TIMER_MARGIN) ")");

static bool nowayout = WATCHDOG_NOWAYOUT;
module_param(nowayout, bool, 0);
MODULE_PARM_DESC(nowayout,
		"Watchdog cannot be stopped once started (default="
				__MODULE_STRING(WATCHDOG_NOWAYOUT) ")");

<<<<<<< HEAD
static int soft_noboot = 0;
=======
static int soft_noboot;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
module_param(soft_noboot, int, 0);
MODULE_PARM_DESC(soft_noboot,
	"Softdog action, set to 1 to ignore reboots, 0 to reboot (default=0)");

static int soft_panic;
module_param(soft_panic, int, 0);
MODULE_PARM_DESC(soft_panic,
	"Softdog action, set to 1 to panic, 0 to reboot (default=0)");

<<<<<<< HEAD
/*
 *	Our timer
 */

static void watchdog_fire(unsigned long);

static struct timer_list watchdog_ticktock =
		TIMER_INITIALIZER(watchdog_fire, 0, 0);

/*
 *	If the timer expires..
 */

static void watchdog_fire(unsigned long data)
{
	if (soft_noboot)
		pr_crit("Triggered - Reboot ignored\n");
	else if (soft_panic) {
=======
static char *soft_reboot_cmd;
module_param(soft_reboot_cmd, charp, 0000);
MODULE_PARM_DESC(soft_reboot_cmd,
	"Set reboot command. Emergency reboot takes place if unset");

static bool soft_active_on_boot;
module_param(soft_active_on_boot, bool, 0000);
MODULE_PARM_DESC(soft_active_on_boot,
	"Set to true to active Softdog on boot (default=false)");

static struct hrtimer softdog_ticktock;
static struct hrtimer softdog_preticktock;

static int reboot_kthread_fn(void *data)
{
	kernel_restart(soft_reboot_cmd);
	return -EPERM; /* Should not reach here */
}

static void reboot_work_fn(struct work_struct *unused)
{
	kthread_run(reboot_kthread_fn, NULL, "softdog_reboot");
}

static enum hrtimer_restart softdog_fire(struct hrtimer *timer)
{
	static bool soft_reboot_fired;

	module_put(THIS_MODULE);
	if (soft_noboot) {
		pr_crit("Triggered - Reboot ignored\n");
	} else if (soft_panic) {
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		pr_crit("Initiating panic\n");
		panic("Software Watchdog Timer expired");
	} else {
		pr_crit("Initiating system reboot\n");
<<<<<<< HEAD
		emergency_restart();
		pr_crit("Reboot didn't ?????\n");
	}
}

/*
 *	Softdog operations
 */

static int softdog_ping(struct watchdog_device *w)
{
	mod_timer(&watchdog_ticktock, jiffies+(w->timeout*HZ));
=======
		if (!soft_reboot_fired && soft_reboot_cmd != NULL) {
			static DECLARE_WORK(reboot_work, reboot_work_fn);
			/*
			 * The 'kernel_restart' is a 'might-sleep' operation.
			 * Also, executing it in system-wide workqueues blocks
			 * any driver from using the same workqueue in its
			 * shutdown callback function. Thus, we should execute
			 * the 'kernel_restart' in a standalone kernel thread.
			 * But since starting a kernel thread is also a
			 * 'might-sleep' operation, so the 'reboot_work' is
			 * required as a launcher of the kernel thread.
			 *
			 * After request the reboot, restart the timer to
			 * schedule an 'emergency_restart' reboot after
			 * 'TIMER_MARGIN' seconds. It's because if the softdog
			 * hangs, it might be because of scheduling issues. And
			 * if that is the case, both 'schedule_work' and
			 * 'kernel_restart' may possibly be malfunctional at the
			 * same time.
			 */
			soft_reboot_fired = true;
			schedule_work(&reboot_work);
			hrtimer_add_expires_ns(timer,
					(u64)TIMER_MARGIN * NSEC_PER_SEC);

			return HRTIMER_RESTART;
		}
		emergency_restart();
		pr_crit("Reboot didn't ?????\n");
	}

	return HRTIMER_NORESTART;
}

static struct watchdog_device softdog_dev;

static enum hrtimer_restart softdog_pretimeout(struct hrtimer *timer)
{
	watchdog_notify_pretimeout(&softdog_dev);

	return HRTIMER_NORESTART;
}

static int softdog_ping(struct watchdog_device *w)
{
	if (!hrtimer_active(&softdog_ticktock))
		__module_get(THIS_MODULE);
	hrtimer_start(&softdog_ticktock, ktime_set(w->timeout, 0),
		      HRTIMER_MODE_REL);

	if (IS_ENABLED(CONFIG_SOFT_WATCHDOG_PRETIMEOUT)) {
		if (w->pretimeout)
			hrtimer_start(&softdog_preticktock,
				      ktime_set(w->timeout - w->pretimeout, 0),
				      HRTIMER_MODE_REL);
		else
			hrtimer_cancel(&softdog_preticktock);
	}

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	return 0;
}

static int softdog_stop(struct watchdog_device *w)
{
<<<<<<< HEAD
	del_timer(&watchdog_ticktock);
	return 0;
}

static int softdog_set_timeout(struct watchdog_device *w, unsigned int t)
{
	w->timeout = t;
	return 0;
}

/*
 *	Notifier for system down
 */

static int softdog_notify_sys(struct notifier_block *this, unsigned long code,
	void *unused)
{
	if (code == SYS_DOWN || code == SYS_HALT)
		/* Turn the WDT off */
		softdog_stop(NULL);
	return NOTIFY_DONE;
}

/*
 *	Kernel Interfaces
 */

static struct notifier_block softdog_notifier = {
	.notifier_call	= softdog_notify_sys,
};

=======
	if (hrtimer_cancel(&softdog_ticktock))
		module_put(THIS_MODULE);

	if (IS_ENABLED(CONFIG_SOFT_WATCHDOG_PRETIMEOUT))
		hrtimer_cancel(&softdog_preticktock);

	return 0;
}

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
static struct watchdog_info softdog_info = {
	.identity = "Software Watchdog",
	.options = WDIOF_SETTIMEOUT | WDIOF_KEEPALIVEPING | WDIOF_MAGICCLOSE,
};

<<<<<<< HEAD
static struct watchdog_ops softdog_ops = {
	.owner = THIS_MODULE,
	.start = softdog_ping,
	.stop = softdog_stop,
	.ping = softdog_ping,
	.set_timeout = softdog_set_timeout,
=======
static const struct watchdog_ops softdog_ops = {
	.owner = THIS_MODULE,
	.start = softdog_ping,
	.stop = softdog_stop,
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
};

static struct watchdog_device softdog_dev = {
	.info = &softdog_info,
	.ops = &softdog_ops,
	.min_timeout = 1,
<<<<<<< HEAD
	.max_timeout = 0xFFFF
};

static int __init watchdog_init(void)
{
	int ret;

	/* Check that the soft_margin value is within it's range;
	   if not reset to the default */
	if (soft_margin < 1 || soft_margin > 65535) {
		pr_info("soft_margin must be 0 < soft_margin < 65536, using %d\n",
			TIMER_MARGIN);
		return -EINVAL;
	}
	softdog_dev.timeout = soft_margin;

	watchdog_set_nowayout(&softdog_dev, nowayout);

	ret = register_reboot_notifier(&softdog_notifier);
	if (ret) {
		pr_err("cannot register reboot notifier (err=%d)\n", ret);
		return ret;
	}

	ret = watchdog_register_device(&softdog_dev);
	if (ret) {
		unregister_reboot_notifier(&softdog_notifier);
		return ret;
	}

	pr_info("Software Watchdog Timer: 0.08 initialized. soft_noboot=%d soft_margin=%d sec soft_panic=%d (nowayout=%d)\n",
		soft_noboot, soft_margin, soft_panic, nowayout);

	return 0;
}

static void __exit watchdog_exit(void)
{
	watchdog_unregister_device(&softdog_dev);
	unregister_reboot_notifier(&softdog_notifier);
}

module_init(watchdog_init);
module_exit(watchdog_exit);
=======
	.max_timeout = 65535,
	.timeout = TIMER_MARGIN,
};

static int __init softdog_init(void)
{
	int ret;

	watchdog_init_timeout(&softdog_dev, soft_margin, NULL);
	watchdog_set_nowayout(&softdog_dev, nowayout);
	watchdog_stop_on_reboot(&softdog_dev);

	hrtimer_init(&softdog_ticktock, CLOCK_MONOTONIC, HRTIMER_MODE_REL);
	softdog_ticktock.function = softdog_fire;

	if (IS_ENABLED(CONFIG_SOFT_WATCHDOG_PRETIMEOUT)) {
		softdog_info.options |= WDIOF_PRETIMEOUT;
		hrtimer_init(&softdog_preticktock, CLOCK_MONOTONIC,
			     HRTIMER_MODE_REL);
		softdog_preticktock.function = softdog_pretimeout;
	}

	if (soft_active_on_boot)
		softdog_ping(&softdog_dev);

	ret = watchdog_register_device(&softdog_dev);
	if (ret)
		return ret;

	pr_info("initialized. soft_noboot=%d soft_margin=%d sec soft_panic=%d (nowayout=%d)\n",
		soft_noboot, softdog_dev.timeout, soft_panic, nowayout);
	pr_info("             soft_reboot_cmd=%s soft_active_on_boot=%d\n",
		soft_reboot_cmd ?: "<not set>", soft_active_on_boot);

	return 0;
}
module_init(softdog_init);

static void __exit softdog_exit(void)
{
	watchdog_unregister_device(&softdog_dev);
}
module_exit(softdog_exit);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

MODULE_AUTHOR("Alan Cox");
MODULE_DESCRIPTION("Software Watchdog Device Driver");
MODULE_LICENSE("GPL");
<<<<<<< HEAD
MODULE_ALIAS_MISCDEV(WATCHDOG_MINOR);
=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
