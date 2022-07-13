<<<<<<< HEAD
=======
// SPDX-License-Identifier: GPL-2.0-or-later
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
/*
 *	Xen Watchdog Driver
 *
 *	(c) Copyright 2010 Novell, Inc.
<<<<<<< HEAD
 *
 *	This program is free software; you can redistribute it and/or
 *	modify it under the terms of the GNU General Public License
 *	as published by the Free Software Foundation; either version
 *	2 of the License, or (at your option) any later version.
 */

#define pr_fmt(fmt) KBUILD_MODNAME ": " fmt

#define DRV_NAME	"wdt"
#define DRV_VERSION	"0.01"
=======
 */

#define DRV_NAME	"xen_wdt"
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

#include <linux/bug.h>
#include <linux/errno.h>
#include <linux/fs.h>
#include <linux/hrtimer.h>
#include <linux/kernel.h>
#include <linux/ktime.h>
#include <linux/init.h>
<<<<<<< HEAD
#include <linux/miscdevice.h>
#include <linux/module.h>
#include <linux/moduleparam.h>
#include <linux/platform_device.h>
#include <linux/spinlock.h>
#include <linux/uaccess.h>
=======
#include <linux/module.h>
#include <linux/moduleparam.h>
#include <linux/platform_device.h>
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#include <linux/watchdog.h>
#include <xen/xen.h>
#include <asm/xen/hypercall.h>
#include <xen/interface/sched.h>

static struct platform_device *platform_device;
<<<<<<< HEAD
static DEFINE_SPINLOCK(wdt_lock);
static struct sched_watchdog wdt;
static __kernel_time_t wdt_expires;
static bool is_active, expect_release;

#define WATCHDOG_TIMEOUT 60 /* in seconds */
static unsigned int timeout = WATCHDOG_TIMEOUT;
=======
static struct sched_watchdog wdt;
static time64_t wdt_expires;

#define WATCHDOG_TIMEOUT 60 /* in seconds */
static unsigned int timeout;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
module_param(timeout, uint, S_IRUGO);
MODULE_PARM_DESC(timeout, "Watchdog timeout in seconds "
	"(default=" __MODULE_STRING(WATCHDOG_TIMEOUT) ")");

static bool nowayout = WATCHDOG_NOWAYOUT;
module_param(nowayout, bool, S_IRUGO);
MODULE_PARM_DESC(nowayout, "Watchdog cannot be stopped once started "
	"(default=" __MODULE_STRING(WATCHDOG_NOWAYOUT) ")");

<<<<<<< HEAD
static inline __kernel_time_t set_timeout(void)
{
	wdt.timeout = timeout;
	return ktime_to_timespec(ktime_get()).tv_sec + timeout;
}

static int xen_wdt_start(void)
{
	__kernel_time_t expires;
	int err;

	spin_lock(&wdt_lock);

	expires = set_timeout();
=======
static inline time64_t set_timeout(struct watchdog_device *wdd)
{
	wdt.timeout = wdd->timeout;
	return ktime_get_seconds() + wdd->timeout;
}

static int xen_wdt_start(struct watchdog_device *wdd)
{
	time64_t expires;
	int err;

	expires = set_timeout(wdd);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	if (!wdt.id)
		err = HYPERVISOR_sched_op(SCHEDOP_watchdog, &wdt);
	else
		err = -EBUSY;
	if (err > 0) {
		wdt.id = err;
		wdt_expires = expires;
		err = 0;
	} else
		BUG_ON(!err);

<<<<<<< HEAD
	spin_unlock(&wdt_lock);

	return err;
}

static int xen_wdt_stop(void)
{
	int err = 0;

	spin_lock(&wdt_lock);

=======
	return err;
}

static int xen_wdt_stop(struct watchdog_device *wdd)
{
	int err = 0;

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	wdt.timeout = 0;
	if (wdt.id)
		err = HYPERVISOR_sched_op(SCHEDOP_watchdog, &wdt);
	if (!err)
		wdt.id = 0;

<<<<<<< HEAD
	spin_unlock(&wdt_lock);

	return err;
}

static int xen_wdt_kick(void)
{
	__kernel_time_t expires;
	int err;

	spin_lock(&wdt_lock);

	expires = set_timeout();
=======
	return err;
}

static int xen_wdt_kick(struct watchdog_device *wdd)
{
	time64_t expires;
	int err;

	expires = set_timeout(wdd);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	if (wdt.id)
		err = HYPERVISOR_sched_op(SCHEDOP_watchdog, &wdt);
	else
		err = -ENXIO;
	if (!err)
		wdt_expires = expires;

<<<<<<< HEAD
	spin_unlock(&wdt_lock);

	return err;
}

static int xen_wdt_open(struct inode *inode, struct file *file)
{
	int err;

	/* /dev/watchdog can only be opened once */
	if (xchg(&is_active, true))
		return -EBUSY;

	err = xen_wdt_start();
	if (err == -EBUSY)
		err = xen_wdt_kick();
	return err ?: nonseekable_open(inode, file);
}

static int xen_wdt_release(struct inode *inode, struct file *file)
{
	int err = 0;

	if (expect_release)
		err = xen_wdt_stop();
	else {
		pr_crit("unexpected close, not stopping watchdog!\n");
		xen_wdt_kick();
	}
	is_active = err;
	expect_release = false;
	return err;
}

static ssize_t xen_wdt_write(struct file *file, const char __user *data,
			     size_t len, loff_t *ppos)
{
	/* See if we got the magic character 'V' and reload the timer */
	if (len) {
		if (!nowayout) {
			size_t i;

			/* in case it was set long ago */
			expect_release = false;

			/* scan to see whether or not we got the magic
			   character */
			for (i = 0; i != len; i++) {
				char c;
				if (get_user(c, data + i))
					return -EFAULT;
				if (c == 'V')
					expect_release = true;
			}
		}

		/* someone wrote to us, we should reload the timer */
		xen_wdt_kick();
	}
	return len;
}

static long xen_wdt_ioctl(struct file *file, unsigned int cmd,
			  unsigned long arg)
{
	int new_options, retval = -EINVAL;
	int new_timeout;
	int __user *argp = (void __user *)arg;
	static const struct watchdog_info ident = {
		.options =		WDIOF_SETTIMEOUT | WDIOF_MAGICCLOSE,
		.firmware_version =	0,
		.identity =		DRV_NAME,
	};

	switch (cmd) {
	case WDIOC_GETSUPPORT:
		return copy_to_user(argp, &ident, sizeof(ident)) ? -EFAULT : 0;

	case WDIOC_GETSTATUS:
	case WDIOC_GETBOOTSTATUS:
		return put_user(0, argp);

	case WDIOC_SETOPTIONS:
		if (get_user(new_options, argp))
			return -EFAULT;

		if (new_options & WDIOS_DISABLECARD)
			retval = xen_wdt_stop();
		if (new_options & WDIOS_ENABLECARD) {
			retval = xen_wdt_start();
			if (retval == -EBUSY)
				retval = xen_wdt_kick();
		}
		return retval;

	case WDIOC_KEEPALIVE:
		xen_wdt_kick();
		return 0;

	case WDIOC_SETTIMEOUT:
		if (get_user(new_timeout, argp))
			return -EFAULT;
		if (!new_timeout)
			return -EINVAL;
		timeout = new_timeout;
		xen_wdt_kick();
		/* fall through */
	case WDIOC_GETTIMEOUT:
		return put_user(timeout, argp);

	case WDIOC_GETTIMELEFT:
		retval = wdt_expires - ktime_to_timespec(ktime_get()).tv_sec;
		return put_user(retval, argp);
	}

	return -ENOTTY;
}

static const struct file_operations xen_wdt_fops = {
	.owner =		THIS_MODULE,
	.llseek =		no_llseek,
	.write =		xen_wdt_write,
	.unlocked_ioctl =	xen_wdt_ioctl,
	.open =			xen_wdt_open,
	.release =		xen_wdt_release,
};

static struct miscdevice xen_wdt_miscdev = {
	.minor =	WATCHDOG_MINOR,
	.name =		"watchdog",
	.fops =		&xen_wdt_fops,
};

static int __devinit xen_wdt_probe(struct platform_device *dev)
{
	struct sched_watchdog wd = { .id = ~0 };
	int ret = HYPERVISOR_sched_op(SCHEDOP_watchdog, &wd);

	switch (ret) {
	case -EINVAL:
		if (!timeout) {
			timeout = WATCHDOG_TIMEOUT;
			pr_info("timeout value invalid, using %d\n", timeout);
		}

		ret = misc_register(&xen_wdt_miscdev);
		if (ret) {
			pr_err("cannot register miscdev on minor=%d (%d)\n",
			       WATCHDOG_MINOR, ret);
			break;
		}

		pr_info("initialized (timeout=%ds, nowayout=%d)\n",
			timeout, nowayout);
		break;

	case -ENOSYS:
		pr_info("not supported\n");
		ret = -ENODEV;
		break;

	default:
		pr_info("bogus return value %d\n", ret);
		break;
	}

	return ret;
}

static int __devexit xen_wdt_remove(struct platform_device *dev)
{
	/* Stop the timer before we leave */
	if (!nowayout)
		xen_wdt_stop();

	misc_deregister(&xen_wdt_miscdev);
=======
	return err;
}

static unsigned int xen_wdt_get_timeleft(struct watchdog_device *wdd)
{
	return wdt_expires - ktime_get_seconds();
}

static struct watchdog_info xen_wdt_info = {
	.identity = DRV_NAME,
	.options = WDIOF_SETTIMEOUT | WDIOF_KEEPALIVEPING | WDIOF_MAGICCLOSE,
};

static const struct watchdog_ops xen_wdt_ops = {
	.owner = THIS_MODULE,
	.start = xen_wdt_start,
	.stop = xen_wdt_stop,
	.ping = xen_wdt_kick,
	.get_timeleft = xen_wdt_get_timeleft,
};

static struct watchdog_device xen_wdt_dev = {
	.info = &xen_wdt_info,
	.ops = &xen_wdt_ops,
	.timeout = WATCHDOG_TIMEOUT,
};

static int xen_wdt_probe(struct platform_device *pdev)
{
	struct device *dev = &pdev->dev;
	struct sched_watchdog wd = { .id = ~0 };
	int ret = HYPERVISOR_sched_op(SCHEDOP_watchdog, &wd);

	if (ret == -ENOSYS) {
		dev_err(dev, "watchdog not supported by hypervisor\n");
		return -ENODEV;
	}

	if (ret != -EINVAL) {
		dev_err(dev, "unexpected hypervisor error (%d)\n", ret);
		return -ENODEV;
	}

	watchdog_init_timeout(&xen_wdt_dev, timeout, NULL);
	watchdog_set_nowayout(&xen_wdt_dev, nowayout);
	watchdog_stop_on_reboot(&xen_wdt_dev);
	watchdog_stop_on_unregister(&xen_wdt_dev);

	ret = devm_watchdog_register_device(dev, &xen_wdt_dev);
	if (ret)
		return ret;

	dev_info(dev, "initialized (timeout=%ds, nowayout=%d)\n",
		 xen_wdt_dev.timeout, nowayout);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	return 0;
}

<<<<<<< HEAD
static void xen_wdt_shutdown(struct platform_device *dev)
{
	xen_wdt_stop();
}

static int xen_wdt_suspend(struct platform_device *dev, pm_message_t state)
{
	typeof(wdt.id) id = wdt.id;
	int rc = xen_wdt_stop();
=======
static int xen_wdt_suspend(struct platform_device *dev, pm_message_t state)
{
	typeof(wdt.id) id = wdt.id;
	int rc = xen_wdt_stop(&xen_wdt_dev);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	wdt.id = id;
	return rc;
}

static int xen_wdt_resume(struct platform_device *dev)
{
	if (!wdt.id)
		return 0;
	wdt.id = 0;
<<<<<<< HEAD
	return xen_wdt_start();
=======
	return xen_wdt_start(&xen_wdt_dev);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

static struct platform_driver xen_wdt_driver = {
	.probe          = xen_wdt_probe,
<<<<<<< HEAD
	.remove         = __devexit_p(xen_wdt_remove),
	.shutdown       = xen_wdt_shutdown,
	.suspend        = xen_wdt_suspend,
	.resume         = xen_wdt_resume,
	.driver         = {
		.owner  = THIS_MODULE,
=======
	.suspend        = xen_wdt_suspend,
	.resume         = xen_wdt_resume,
	.driver         = {
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		.name   = DRV_NAME,
	},
};

static int __init xen_wdt_init_module(void)
{
	int err;

	if (!xen_domain())
		return -ENODEV;

<<<<<<< HEAD
	pr_info("Xen WatchDog Timer Driver v%s\n", DRV_VERSION);

=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	err = platform_driver_register(&xen_wdt_driver);
	if (err)
		return err;

	platform_device = platform_device_register_simple(DRV_NAME,
								  -1, NULL, 0);
	if (IS_ERR(platform_device)) {
		err = PTR_ERR(platform_device);
		platform_driver_unregister(&xen_wdt_driver);
	}

	return err;
}

static void __exit xen_wdt_cleanup_module(void)
{
	platform_device_unregister(platform_device);
	platform_driver_unregister(&xen_wdt_driver);
<<<<<<< HEAD
	pr_info("module unloaded\n");
=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

module_init(xen_wdt_init_module);
module_exit(xen_wdt_cleanup_module);

MODULE_AUTHOR("Jan Beulich <jbeulich@novell.com>");
MODULE_DESCRIPTION("Xen WatchDog Timer Driver");
<<<<<<< HEAD
MODULE_VERSION(DRV_VERSION);
MODULE_LICENSE("GPL");
MODULE_ALIAS_MISCDEV(WATCHDOG_MINOR);
=======
MODULE_LICENSE("GPL");
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
