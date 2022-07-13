<<<<<<< HEAD
=======
// SPDX-License-Identifier: GPL-2.0-or-later
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
/*
 * mpc8xxx_wdt.c - MPC8xx/MPC83xx/MPC86xx watchdog userspace interface
 *
 * Authors: Dave Updegraff <dave@cray.org>
 *	    Kumar Gala <galak@kernel.crashing.org>
 *		Attribution: from 83xx_wst: Florian Schirmer <jolt@tuxbox.org>
 *				..and from sc520_wdt
 * Copyright (c) 2008  MontaVista Software, Inc.
 *                     Anton Vorontsov <avorontsov@ru.mvista.com>
 *
 * Note: it appears that you can only actually ENABLE or DISABLE the thing
 * once after POR. Once enabled, you cannot disable, and vice versa.
<<<<<<< HEAD
 *
 * This program is free software; you can redistribute  it and/or modify it
 * under  the terms of  the GNU General  Public License as published by the
 * Free Software Foundation;  either version 2 of the  License, or (at your
 * option) any later version.
 */

#define pr_fmt(fmt) KBUILD_MODNAME ": " fmt

#include <linux/fs.h>
#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/timer.h>
#include <linux/miscdevice.h>
#include <linux/of_platform.h>
=======
 */

#include <linux/fs.h>
#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/of.h>
#include <linux/platform_device.h>
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#include <linux/module.h>
#include <linux/watchdog.h>
#include <linux/io.h>
#include <linux/uaccess.h>
#include <sysdev/fsl_soc.h>

<<<<<<< HEAD
=======
#define WATCHDOG_TIMEOUT 10

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
struct mpc8xxx_wdt {
	__be32 res0;
	__be32 swcrr; /* System watchdog control register */
#define SWCRR_SWTC 0xFFFF0000 /* Software Watchdog Time Count. */
<<<<<<< HEAD
=======
#define SWCRR_SWF  0x00000008 /* Software Watchdog Freeze (mpc8xx). */
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#define SWCRR_SWEN 0x00000004 /* Watchdog Enable bit. */
#define SWCRR_SWRI 0x00000002 /* Software Watchdog Reset/Interrupt Select bit.*/
#define SWCRR_SWPR 0x00000001 /* Software Watchdog Counter Prescale bit. */
	__be32 swcnr; /* System watchdog count register */
	u8 res1[2];
	__be16 swsrr; /* System watchdog service register */
	u8 res2[0xF0];
};

struct mpc8xxx_wdt_type {
	int prescaler;
	bool hw_enabled;
<<<<<<< HEAD
};

static struct mpc8xxx_wdt __iomem *wd_base;
static int mpc8xxx_wdt_init_late(void);

static u16 timeout = 0xffff;
module_param(timeout, ushort, 0);
MODULE_PARM_DESC(timeout,
	"Watchdog timeout in ticks. (0<timeout<65536, default=65535)");
=======
	u32 rsr_mask;
};

struct mpc8xxx_wdt_ddata {
	struct mpc8xxx_wdt __iomem *base;
	struct watchdog_device wdd;
	spinlock_t lock;
	u16 swtc;
};

static u16 timeout;
module_param(timeout, ushort, 0);
MODULE_PARM_DESC(timeout,
	"Watchdog timeout in seconds. (1<timeout<65535, default="
	__MODULE_STRING(WATCHDOG_TIMEOUT) ")");
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

static bool reset = 1;
module_param(reset, bool, 0);
MODULE_PARM_DESC(reset,
	"Watchdog Interrupt/Reset Mode. 0 = interrupt, 1 = reset");

static bool nowayout = WATCHDOG_NOWAYOUT;
module_param(nowayout, bool, 0);
MODULE_PARM_DESC(nowayout, "Watchdog cannot be stopped once started "
		 "(default=" __MODULE_STRING(WATCHDOG_NOWAYOUT) ")");

<<<<<<< HEAD
/*
 * We always prescale, but if someone really doesn't want to they can set this
 * to 0
 */
static int prescale = 1;
static unsigned int timeout_sec;

static unsigned long wdt_is_open;
static DEFINE_SPINLOCK(wdt_spinlock);

static void mpc8xxx_wdt_keepalive(void)
{
	/* Ping the WDT */
	spin_lock(&wdt_spinlock);
	out_be16(&wd_base->swsrr, 0x556c);
	out_be16(&wd_base->swsrr, 0xaa39);
	spin_unlock(&wdt_spinlock);
}

static void mpc8xxx_wdt_timer_ping(unsigned long arg);
static DEFINE_TIMER(wdt_timer, mpc8xxx_wdt_timer_ping, 0, 0);

static void mpc8xxx_wdt_timer_ping(unsigned long arg)
{
	mpc8xxx_wdt_keepalive();
	/* We're pinging it twice faster than needed, just to be sure. */
	mod_timer(&wdt_timer, jiffies + HZ * timeout_sec / 2);
}

static void mpc8xxx_wdt_pr_warn(const char *msg)
{
	pr_crit("%s, expect the %s soon!\n", msg,
		reset ? "reset" : "machine check exception");
}

static ssize_t mpc8xxx_wdt_write(struct file *file, const char __user *buf,
				 size_t count, loff_t *ppos)
{
	if (count)
		mpc8xxx_wdt_keepalive();
	return count;
}

static int mpc8xxx_wdt_open(struct inode *inode, struct file *file)
{
	u32 tmp = SWCRR_SWEN;
	if (test_and_set_bit(0, &wdt_is_open))
		return -EBUSY;

	/* Once we start the watchdog we can't stop it */
	if (nowayout)
		__module_get(THIS_MODULE);

	/* Good, fire up the show */
	if (prescale)
		tmp |= SWCRR_SWPR;
	if (reset)
		tmp |= SWCRR_SWRI;

	tmp |= timeout << 16;

	out_be32(&wd_base->swcrr, tmp);

	del_timer_sync(&wdt_timer);

	return nonseekable_open(inode, file);
}

static int mpc8xxx_wdt_release(struct inode *inode, struct file *file)
{
	if (!nowayout)
		mpc8xxx_wdt_timer_ping(0);
	else
		mpc8xxx_wdt_pr_warn("watchdog closed");
	clear_bit(0, &wdt_is_open);
	return 0;
}

static long mpc8xxx_wdt_ioctl(struct file *file, unsigned int cmd,
							unsigned long arg)
{
	void __user *argp = (void __user *)arg;
	int __user *p = argp;
	static const struct watchdog_info ident = {
		.options = WDIOF_KEEPALIVEPING,
		.firmware_version = 1,
		.identity = "MPC8xxx",
	};

	switch (cmd) {
	case WDIOC_GETSUPPORT:
		return copy_to_user(argp, &ident, sizeof(ident)) ? -EFAULT : 0;
	case WDIOC_GETSTATUS:
	case WDIOC_GETBOOTSTATUS:
		return put_user(0, p);
	case WDIOC_KEEPALIVE:
		mpc8xxx_wdt_keepalive();
		return 0;
	case WDIOC_GETTIMEOUT:
		return put_user(timeout_sec, p);
	default:
		return -ENOTTY;
	}
}

static const struct file_operations mpc8xxx_wdt_fops = {
	.owner		= THIS_MODULE,
	.llseek		= no_llseek,
	.write		= mpc8xxx_wdt_write,
	.unlocked_ioctl	= mpc8xxx_wdt_ioctl,
	.open		= mpc8xxx_wdt_open,
	.release	= mpc8xxx_wdt_release,
};

static struct miscdevice mpc8xxx_wdt_miscdev = {
	.minor	= WATCHDOG_MINOR,
	.name	= "watchdog",
	.fops	= &mpc8xxx_wdt_fops,
};

static const struct of_device_id mpc8xxx_wdt_match[];
static int __devinit mpc8xxx_wdt_probe(struct platform_device *ofdev)
{
	int ret;
	const struct of_device_id *match;
	struct device_node *np = ofdev->dev.of_node;
	struct mpc8xxx_wdt_type *wdt_type;
	u32 freq = fsl_get_sys_freq();
	bool enabled;

	match = of_match_device(mpc8xxx_wdt_match, &ofdev->dev);
	if (!match)
		return -EINVAL;
	wdt_type = match->data;
=======
static void mpc8xxx_wdt_keepalive(struct mpc8xxx_wdt_ddata *ddata)
{
	/* Ping the WDT */
	spin_lock(&ddata->lock);
	out_be16(&ddata->base->swsrr, 0x556c);
	out_be16(&ddata->base->swsrr, 0xaa39);
	spin_unlock(&ddata->lock);
}

static int mpc8xxx_wdt_start(struct watchdog_device *w)
{
	struct mpc8xxx_wdt_ddata *ddata =
		container_of(w, struct mpc8xxx_wdt_ddata, wdd);
	u32 tmp = in_be32(&ddata->base->swcrr);

	/* Good, fire up the show */
	tmp &= ~(SWCRR_SWTC | SWCRR_SWF | SWCRR_SWEN | SWCRR_SWRI | SWCRR_SWPR);
	tmp |= SWCRR_SWEN | SWCRR_SWPR | (ddata->swtc << 16);

	if (reset)
		tmp |= SWCRR_SWRI;

	out_be32(&ddata->base->swcrr, tmp);

	tmp = in_be32(&ddata->base->swcrr);
	if (!(tmp & SWCRR_SWEN))
		return -EOPNOTSUPP;

	ddata->swtc = tmp >> 16;
	set_bit(WDOG_HW_RUNNING, &ddata->wdd.status);

	return 0;
}

static int mpc8xxx_wdt_ping(struct watchdog_device *w)
{
	struct mpc8xxx_wdt_ddata *ddata =
		container_of(w, struct mpc8xxx_wdt_ddata, wdd);

	mpc8xxx_wdt_keepalive(ddata);
	return 0;
}

static struct watchdog_info mpc8xxx_wdt_info = {
	.options = WDIOF_KEEPALIVEPING | WDIOF_MAGICCLOSE | WDIOF_SETTIMEOUT,
	.firmware_version = 1,
	.identity = "MPC8xxx",
};

static const struct watchdog_ops mpc8xxx_wdt_ops = {
	.owner = THIS_MODULE,
	.start = mpc8xxx_wdt_start,
	.ping = mpc8xxx_wdt_ping,
};

static int mpc8xxx_wdt_probe(struct platform_device *ofdev)
{
	int ret;
	struct resource *res;
	const struct mpc8xxx_wdt_type *wdt_type;
	struct mpc8xxx_wdt_ddata *ddata;
	u32 freq = fsl_get_sys_freq();
	bool enabled;
	struct device *dev = &ofdev->dev;

	wdt_type = of_device_get_match_data(dev);
	if (!wdt_type)
		return -EINVAL;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	if (!freq || freq == -1)
		return -EINVAL;

<<<<<<< HEAD
	wd_base = of_iomap(np, 0);
	if (!wd_base)
		return -ENOMEM;

	enabled = in_be32(&wd_base->swcrr) & SWCRR_SWEN;
	if (!enabled && wdt_type->hw_enabled) {
		pr_info("could not be enabled in software\n");
		ret = -ENOSYS;
		goto err_unmap;
	}

	/* Calculate the timeout in seconds */
	if (prescale)
		timeout_sec = (timeout * wdt_type->prescaler) / freq;
	else
		timeout_sec = timeout / freq;

#ifdef MODULE
	ret = mpc8xxx_wdt_init_late();
	if (ret)
		goto err_unmap;
#endif

	pr_info("WDT driver for MPC8xxx initialized. mode:%s timeout=%d (%d seconds)\n",
		reset ? "reset" : "interrupt", timeout, timeout_sec);
=======
	ddata = devm_kzalloc(dev, sizeof(*ddata), GFP_KERNEL);
	if (!ddata)
		return -ENOMEM;

	ddata->base = devm_platform_ioremap_resource(ofdev, 0);
	if (IS_ERR(ddata->base))
		return PTR_ERR(ddata->base);

	enabled = in_be32(&ddata->base->swcrr) & SWCRR_SWEN;
	if (!enabled && wdt_type->hw_enabled) {
		dev_info(dev, "could not be enabled in software\n");
		return -ENODEV;
	}

	res = platform_get_resource(ofdev, IORESOURCE_MEM, 1);
	if (res) {
		bool status;
		u32 __iomem *rsr = ioremap(res->start, resource_size(res));

		if (!rsr)
			return -ENOMEM;

		status = in_be32(rsr) & wdt_type->rsr_mask;
		ddata->wdd.bootstatus = status ? WDIOF_CARDRESET : 0;
		 /* clear reset status bits related to watchdog timer */
		out_be32(rsr, wdt_type->rsr_mask);
		iounmap(rsr);

		dev_info(dev, "Last boot was %scaused by watchdog\n",
			 status ? "" : "not ");
	}

	spin_lock_init(&ddata->lock);

	ddata->wdd.info = &mpc8xxx_wdt_info;
	ddata->wdd.ops = &mpc8xxx_wdt_ops;

	ddata->wdd.timeout = WATCHDOG_TIMEOUT;
	watchdog_init_timeout(&ddata->wdd, timeout, dev);

	watchdog_set_nowayout(&ddata->wdd, nowayout);

	ddata->swtc = min(ddata->wdd.timeout * freq / wdt_type->prescaler,
			  0xffffU);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	/*
	 * If the watchdog was previously enabled or we're running on
	 * MPC8xxx, we should ping the wdt from the kernel until the
	 * userspace handles it.
	 */
	if (enabled)
<<<<<<< HEAD
		mpc8xxx_wdt_timer_ping(0);
	return 0;
err_unmap:
	iounmap(wd_base);
	wd_base = NULL;
	return ret;
}

static int __devexit mpc8xxx_wdt_remove(struct platform_device *ofdev)
{
	mpc8xxx_wdt_pr_warn("watchdog removed");
	del_timer_sync(&wdt_timer);
	misc_deregister(&mpc8xxx_wdt_miscdev);
	iounmap(wd_base);

=======
		mpc8xxx_wdt_start(&ddata->wdd);

	ddata->wdd.max_hw_heartbeat_ms = (ddata->swtc * wdt_type->prescaler) /
					 (freq / 1000);
	ddata->wdd.min_timeout = ddata->wdd.max_hw_heartbeat_ms / 1000;
	if (ddata->wdd.timeout < ddata->wdd.min_timeout)
		ddata->wdd.timeout = ddata->wdd.min_timeout;

	ret = devm_watchdog_register_device(dev, &ddata->wdd);
	if (ret)
		return ret;

	dev_info(dev,
		 "WDT driver for MPC8xxx initialized. mode:%s timeout=%d sec\n",
		 reset ? "reset" : "interrupt", ddata->wdd.timeout);

	platform_set_drvdata(ofdev, ddata);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	return 0;
}

static const struct of_device_id mpc8xxx_wdt_match[] = {
	{
		.compatible = "mpc83xx_wdt",
		.data = &(struct mpc8xxx_wdt_type) {
			.prescaler = 0x10000,
<<<<<<< HEAD
=======
			.rsr_mask = BIT(3), /* RSR Bit SWRS */
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		},
	},
	{
		.compatible = "fsl,mpc8610-wdt",
		.data = &(struct mpc8xxx_wdt_type) {
			.prescaler = 0x10000,
			.hw_enabled = true,
<<<<<<< HEAD
=======
			.rsr_mask = BIT(20), /* RSTRSCR Bit WDT_RR */
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		},
	},
	{
		.compatible = "fsl,mpc823-wdt",
		.data = &(struct mpc8xxx_wdt_type) {
			.prescaler = 0x800,
<<<<<<< HEAD
=======
			.hw_enabled = true,
			.rsr_mask = BIT(28), /* RSR Bit SWRS */
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		},
	},
	{},
};
MODULE_DEVICE_TABLE(of, mpc8xxx_wdt_match);

static struct platform_driver mpc8xxx_wdt_driver = {
	.probe		= mpc8xxx_wdt_probe,
<<<<<<< HEAD
	.remove		= __devexit_p(mpc8xxx_wdt_remove),
	.driver = {
		.name = "mpc8xxx_wdt",
		.owner = THIS_MODULE,
=======
	.driver = {
		.name = "mpc8xxx_wdt",
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		.of_match_table = mpc8xxx_wdt_match,
	},
};

<<<<<<< HEAD
/*
 * We do wdt initialization in two steps: arch_initcall probes the wdt
 * very early to start pinging the watchdog (misc devices are not yet
 * available), and later module_init() just registers the misc device.
 */
static int mpc8xxx_wdt_init_late(void)
{
	int ret;

	if (!wd_base)
		return -ENODEV;

	ret = misc_register(&mpc8xxx_wdt_miscdev);
	if (ret) {
		pr_err("cannot register miscdev on minor=%d (err=%d)\n",
		       WATCHDOG_MINOR, ret);
		return ret;
	}
	return 0;
}
#ifndef MODULE
module_init(mpc8xxx_wdt_init_late);
#endif

=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
static int __init mpc8xxx_wdt_init(void)
{
	return platform_driver_register(&mpc8xxx_wdt_driver);
}
arch_initcall(mpc8xxx_wdt_init);

static void __exit mpc8xxx_wdt_exit(void)
{
	platform_driver_unregister(&mpc8xxx_wdt_driver);
}
module_exit(mpc8xxx_wdt_exit);

MODULE_AUTHOR("Dave Updegraff, Kumar Gala");
MODULE_DESCRIPTION("Driver for watchdog timer in MPC8xx/MPC83xx/MPC86xx "
		   "uProcessors");
MODULE_LICENSE("GPL");
<<<<<<< HEAD
MODULE_ALIAS_MISCDEV(WATCHDOG_MINOR);
=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
