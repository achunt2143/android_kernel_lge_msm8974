<<<<<<< HEAD
=======
// SPDX-License-Identifier: GPL-2.0+
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
/*
 *      Driver for the MTX-1 Watchdog.
 *
 *      (C) Copyright 2005 4G Systems <info@4g-systems.biz>,
 *							All Rights Reserved.
 *                              http://www.4g-systems.biz
 *
 *	(C) Copyright 2007 OpenWrt.org, Florian Fainelli <florian@openwrt.org>
<<<<<<< HEAD
 *
 *      This program is free software; you can redistribute it and/or
 *      modify it under the terms of the GNU General Public License
 *      as published by the Free Software Foundation; either version
 *      2 of the License, or (at your option) any later version.
 *
 *      Neither Michael Stickel nor 4G Systems admit liability nor provide
 *      warranty for any of this software. This material is provided
 *      "AS-IS" and at no charge.
 *
=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 *      (c) Copyright 2005    4G Systems <info@4g-systems.biz>
 *
 *      Release 0.01.
 *      Author: Michael Stickel  michael.stickel@4g-systems.biz
 *
 *      Release 0.02.
 *	Author: Florian Fainelli florian@openwrt.org
 *		use the Linux watchdog/timer APIs
 *
 *      The Watchdog is configured to reset the MTX-1
 *      if it is not triggered for 100 seconds.
 *      It should not be triggered more often than 1.6 seconds.
 *
 *      A timer triggers the watchdog every 5 seconds, until
 *      it is opened for the first time. After the first open
 *      it MUST be triggered every 2..95 seconds.
 */

#include <linux/module.h>
#include <linux/moduleparam.h>
#include <linux/types.h>
#include <linux/errno.h>
#include <linux/miscdevice.h>
#include <linux/fs.h>
<<<<<<< HEAD
#include <linux/init.h>
=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#include <linux/ioport.h>
#include <linux/timer.h>
#include <linux/completion.h>
#include <linux/jiffies.h>
#include <linux/watchdog.h>
#include <linux/platform_device.h>
#include <linux/io.h>
#include <linux/uaccess.h>
<<<<<<< HEAD
#include <linux/gpio.h>

#include <asm/mach-au1x00/au1000.h>
=======
#include <linux/gpio/consumer.h>
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

#define MTX1_WDT_INTERVAL	(5 * HZ)

static int ticks = 100 * HZ;

static struct {
	struct completion stop;
	spinlock_t lock;
	int running;
	struct timer_list timer;
	int queue;
	int default_ticks;
	unsigned long inuse;
<<<<<<< HEAD
	unsigned gpio;
	unsigned int gstate;
} mtx1_wdt_device;

static void mtx1_wdt_trigger(unsigned long unused)
=======
	struct gpio_desc *gpiod;
	unsigned int gstate;
} mtx1_wdt_device;

static void mtx1_wdt_trigger(struct timer_list *unused)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
{
	spin_lock(&mtx1_wdt_device.lock);
	if (mtx1_wdt_device.running)
		ticks--;

	/* toggle wdt gpio */
	mtx1_wdt_device.gstate = !mtx1_wdt_device.gstate;
<<<<<<< HEAD
	gpio_set_value(mtx1_wdt_device.gpio, mtx1_wdt_device.gstate);
=======
	gpiod_set_value(mtx1_wdt_device.gpiod, mtx1_wdt_device.gstate);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	if (mtx1_wdt_device.queue && ticks)
		mod_timer(&mtx1_wdt_device.timer, jiffies + MTX1_WDT_INTERVAL);
	else
		complete(&mtx1_wdt_device.stop);
	spin_unlock(&mtx1_wdt_device.lock);
}

static void mtx1_wdt_reset(void)
{
	ticks = mtx1_wdt_device.default_ticks;
}


static void mtx1_wdt_start(void)
{
	unsigned long flags;

	spin_lock_irqsave(&mtx1_wdt_device.lock, flags);
	if (!mtx1_wdt_device.queue) {
		mtx1_wdt_device.queue = 1;
		mtx1_wdt_device.gstate = 1;
<<<<<<< HEAD
		gpio_set_value(mtx1_wdt_device.gpio, 1);
=======
		gpiod_set_value(mtx1_wdt_device.gpiod, 1);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		mod_timer(&mtx1_wdt_device.timer, jiffies + MTX1_WDT_INTERVAL);
	}
	mtx1_wdt_device.running++;
	spin_unlock_irqrestore(&mtx1_wdt_device.lock, flags);
}

static int mtx1_wdt_stop(void)
{
	unsigned long flags;

	spin_lock_irqsave(&mtx1_wdt_device.lock, flags);
	if (mtx1_wdt_device.queue) {
		mtx1_wdt_device.queue = 0;
		mtx1_wdt_device.gstate = 0;
<<<<<<< HEAD
		gpio_set_value(mtx1_wdt_device.gpio, 0);
=======
		gpiod_set_value(mtx1_wdt_device.gpiod, 0);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	}
	ticks = mtx1_wdt_device.default_ticks;
	spin_unlock_irqrestore(&mtx1_wdt_device.lock, flags);
	return 0;
}

/* Filesystem functions */

static int mtx1_wdt_open(struct inode *inode, struct file *file)
{
	if (test_and_set_bit(0, &mtx1_wdt_device.inuse))
		return -EBUSY;
<<<<<<< HEAD
	return nonseekable_open(inode, file);
=======
	return stream_open(inode, file);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}


static int mtx1_wdt_release(struct inode *inode, struct file *file)
{
	clear_bit(0, &mtx1_wdt_device.inuse);
	return 0;
}

static long mtx1_wdt_ioctl(struct file *file, unsigned int cmd,
							unsigned long arg)
{
	void __user *argp = (void __user *)arg;
	int __user *p = (int __user *)argp;
	unsigned int value;
	static const struct watchdog_info ident = {
		.options = WDIOF_CARDRESET,
		.identity = "MTX-1 WDT",
	};

	switch (cmd) {
	case WDIOC_GETSUPPORT:
		if (copy_to_user(argp, &ident, sizeof(ident)))
			return -EFAULT;
		break;
	case WDIOC_GETSTATUS:
	case WDIOC_GETBOOTSTATUS:
		put_user(0, p);
		break;
	case WDIOC_SETOPTIONS:
		if (get_user(value, p))
			return -EFAULT;
		if (value & WDIOS_ENABLECARD)
			mtx1_wdt_start();
		else if (value & WDIOS_DISABLECARD)
			mtx1_wdt_stop();
		else
			return -EINVAL;
		return 0;
	case WDIOC_KEEPALIVE:
		mtx1_wdt_reset();
		break;
	default:
		return -ENOTTY;
	}
	return 0;
}


static ssize_t mtx1_wdt_write(struct file *file, const char *buf,
						size_t count, loff_t *ppos)
{
	if (!count)
		return -EIO;
	mtx1_wdt_reset();
	return count;
}

static const struct file_operations mtx1_wdt_fops = {
	.owner		= THIS_MODULE,
	.llseek		= no_llseek,
	.unlocked_ioctl	= mtx1_wdt_ioctl,
<<<<<<< HEAD
=======
	.compat_ioctl	= compat_ptr_ioctl,
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	.open		= mtx1_wdt_open,
	.write		= mtx1_wdt_write,
	.release	= mtx1_wdt_release,
};


static struct miscdevice mtx1_wdt_misc = {
	.minor	= WATCHDOG_MINOR,
	.name	= "watchdog",
	.fops	= &mtx1_wdt_fops,
};


<<<<<<< HEAD
static int __devinit mtx1_wdt_probe(struct platform_device *pdev)
{
	int ret;

	mtx1_wdt_device.gpio = pdev->resource[0].start;
	ret = gpio_request_one(mtx1_wdt_device.gpio,
				GPIOF_OUT_INIT_HIGH, "mtx1-wdt");
	if (ret < 0) {
		dev_err(&pdev->dev, "failed to request gpio");
		return ret;
=======
static int mtx1_wdt_probe(struct platform_device *pdev)
{
	int ret;

	mtx1_wdt_device.gpiod = devm_gpiod_get(&pdev->dev,
					       NULL, GPIOD_OUT_HIGH);
	if (IS_ERR(mtx1_wdt_device.gpiod)) {
		dev_err(&pdev->dev, "failed to request gpio");
		return PTR_ERR(mtx1_wdt_device.gpiod);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	}

	spin_lock_init(&mtx1_wdt_device.lock);
	init_completion(&mtx1_wdt_device.stop);
	mtx1_wdt_device.queue = 0;
	clear_bit(0, &mtx1_wdt_device.inuse);
<<<<<<< HEAD
	setup_timer(&mtx1_wdt_device.timer, mtx1_wdt_trigger, 0L);
=======
	timer_setup(&mtx1_wdt_device.timer, mtx1_wdt_trigger, 0);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	mtx1_wdt_device.default_ticks = ticks;

	ret = misc_register(&mtx1_wdt_misc);
	if (ret < 0) {
		dev_err(&pdev->dev, "failed to register\n");
		return ret;
	}
	mtx1_wdt_start();
	dev_info(&pdev->dev, "MTX-1 Watchdog driver\n");
	return 0;
}

<<<<<<< HEAD
static int __devexit mtx1_wdt_remove(struct platform_device *pdev)
=======
static void mtx1_wdt_remove(struct platform_device *pdev)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
{
	/* FIXME: do we need to lock this test ? */
	if (mtx1_wdt_device.queue) {
		mtx1_wdt_device.queue = 0;
		wait_for_completion(&mtx1_wdt_device.stop);
	}

<<<<<<< HEAD
	gpio_free(mtx1_wdt_device.gpio);
	misc_deregister(&mtx1_wdt_misc);
	return 0;
=======
	misc_deregister(&mtx1_wdt_misc);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

static struct platform_driver mtx1_wdt_driver = {
	.probe = mtx1_wdt_probe,
<<<<<<< HEAD
	.remove = __devexit_p(mtx1_wdt_remove),
=======
	.remove_new = mtx1_wdt_remove,
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	.driver.name = "mtx1-wdt",
	.driver.owner = THIS_MODULE,
};

module_platform_driver(mtx1_wdt_driver);

MODULE_AUTHOR("Michael Stickel, Florian Fainelli");
MODULE_DESCRIPTION("Driver for the MTX-1 watchdog");
MODULE_LICENSE("GPL");
<<<<<<< HEAD
MODULE_ALIAS_MISCDEV(WATCHDOG_MINOR);
=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
MODULE_ALIAS("platform:mtx1-wdt");
