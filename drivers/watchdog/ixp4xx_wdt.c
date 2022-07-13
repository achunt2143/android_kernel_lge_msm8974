<<<<<<< HEAD
=======
// SPDX-License-Identifier: GPL-2.0-only
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
/*
 * drivers/char/watchdog/ixp4xx_wdt.c
 *
 * Watchdog driver for Intel IXP4xx network processors
 *
 * Author: Deepak Saxena <dsaxena@plexity.net>
<<<<<<< HEAD
 *
 * Copyright 2004 (c) MontaVista, Software, Inc.
 * Based on sa1100 driver, Copyright (C) 2000 Oleg Drokin <green@crimea.edu>
 *
 * This file is licensed under  the terms of the GNU General Public
 * License version 2. This program is licensed "as is" without any
 * warranty of any kind, whether express or implied.
 */

#define pr_fmt(fmt) KBUILD_MODNAME ": " fmt

#include <linux/module.h>
#include <linux/moduleparam.h>
#include <linux/types.h>
#include <linux/kernel.h>
#include <linux/fs.h>
#include <linux/miscdevice.h>
#include <linux/watchdog.h>
#include <linux/init.h>
#include <linux/bitops.h>
#include <linux/uaccess.h>
#include <mach/hardware.h>

static bool nowayout = WATCHDOG_NOWAYOUT;
static int heartbeat = 60;	/* (secs) Default is 1 minute */
static unsigned long wdt_status;
static unsigned long boot_status;
static DEFINE_SPINLOCK(wdt_lock);

#define WDT_TICK_RATE (IXP4XX_PERIPHERAL_BUS_CLOCK * 1000000UL)

#define	WDT_IN_USE		0
#define	WDT_OK_TO_CLOSE		1

static void wdt_enable(void)
{
	spin_lock(&wdt_lock);
	*IXP4XX_OSWK = IXP4XX_WDT_KEY;
	*IXP4XX_OSWE = 0;
	*IXP4XX_OSWT = WDT_TICK_RATE * heartbeat;
	*IXP4XX_OSWE = IXP4XX_WDT_COUNT_ENABLE | IXP4XX_WDT_RESET_ENABLE;
	*IXP4XX_OSWK = 0;
	spin_unlock(&wdt_lock);
}

static void wdt_disable(void)
{
	spin_lock(&wdt_lock);
	*IXP4XX_OSWK = IXP4XX_WDT_KEY;
	*IXP4XX_OSWE = 0;
	*IXP4XX_OSWK = 0;
	spin_unlock(&wdt_lock);
}

static int ixp4xx_wdt_open(struct inode *inode, struct file *file)
{
	if (test_and_set_bit(WDT_IN_USE, &wdt_status))
		return -EBUSY;

	clear_bit(WDT_OK_TO_CLOSE, &wdt_status);
	wdt_enable();
	return nonseekable_open(inode, file);
}

static ssize_t
ixp4xx_wdt_write(struct file *file, const char *data, size_t len, loff_t *ppos)
{
	if (len) {
		if (!nowayout) {
			size_t i;

			clear_bit(WDT_OK_TO_CLOSE, &wdt_status);

			for (i = 0; i != len; i++) {
				char c;

				if (get_user(c, data + i))
					return -EFAULT;
				if (c == 'V')
					set_bit(WDT_OK_TO_CLOSE, &wdt_status);
			}
		}
		wdt_enable();
	}
	return len;
}

static const struct watchdog_info ident = {
	.options	= WDIOF_CARDRESET | WDIOF_MAGICCLOSE |
			  WDIOF_SETTIMEOUT | WDIOF_KEEPALIVEPING,
	.identity	= "IXP4xx Watchdog",
};


static long ixp4xx_wdt_ioctl(struct file *file, unsigned int cmd,
							unsigned long arg)
{
	int ret = -ENOTTY;
	int time;

	switch (cmd) {
	case WDIOC_GETSUPPORT:
		ret = copy_to_user((struct watchdog_info *)arg, &ident,
				   sizeof(ident)) ? -EFAULT : 0;
		break;

	case WDIOC_GETSTATUS:
		ret = put_user(0, (int *)arg);
		break;

	case WDIOC_GETBOOTSTATUS:
		ret = put_user(boot_status, (int *)arg);
		break;

	case WDIOC_KEEPALIVE:
		wdt_enable();
		ret = 0;
		break;

	case WDIOC_SETTIMEOUT:
		ret = get_user(time, (int *)arg);
		if (ret)
			break;

		if (time <= 0 || time > 60) {
			ret = -EINVAL;
			break;
		}

		heartbeat = time;
		wdt_enable();
		/* Fall through */

	case WDIOC_GETTIMEOUT:
		ret = put_user(heartbeat, (int *)arg);
		break;
	}
	return ret;
}

static int ixp4xx_wdt_release(struct inode *inode, struct file *file)
{
	if (test_bit(WDT_OK_TO_CLOSE, &wdt_status))
		wdt_disable();
	else
		pr_crit("Device closed unexpectedly - timer will not stop\n");
	clear_bit(WDT_IN_USE, &wdt_status);
	clear_bit(WDT_OK_TO_CLOSE, &wdt_status);
=======
 * Author: Linus Walleij <linus.walleij@linaro.org>
 *
 * Copyright 2004 (c) MontaVista, Software, Inc.
 * Based on sa1100 driver, Copyright (C) 2000 Oleg Drokin <green@crimea.edu>
 */

#include <linux/module.h>
#include <linux/types.h>
#include <linux/kernel.h>
#include <linux/watchdog.h>
#include <linux/bits.h>
#include <linux/platform_device.h>
#include <linux/clk.h>
#include <linux/soc/ixp4xx/cpu.h>

struct ixp4xx_wdt {
	struct watchdog_device wdd;
	void __iomem *base;
	unsigned long rate;
};

/* Fallback if we do not have a clock for this */
#define IXP4XX_TIMER_FREQ	66666000

/* Registers after the timer registers */
#define IXP4XX_OSWT_OFFSET	0x14  /* Watchdog Timer */
#define IXP4XX_OSWE_OFFSET	0x18  /* Watchdog Enable */
#define IXP4XX_OSWK_OFFSET	0x1C  /* Watchdog Key */
#define IXP4XX_OSST_OFFSET	0x20  /* Timer Status */

#define IXP4XX_OSST_TIMER_WDOG_PEND	0x00000008
#define IXP4XX_OSST_TIMER_WARM_RESET	0x00000010
#define IXP4XX_WDT_KEY			0x0000482E
#define IXP4XX_WDT_RESET_ENABLE		0x00000001
#define IXP4XX_WDT_IRQ_ENABLE		0x00000002
#define IXP4XX_WDT_COUNT_ENABLE		0x00000004

static inline
struct ixp4xx_wdt *to_ixp4xx_wdt(struct watchdog_device *wdd)
{
	return container_of(wdd, struct ixp4xx_wdt, wdd);
}

static int ixp4xx_wdt_start(struct watchdog_device *wdd)
{
	struct ixp4xx_wdt *iwdt = to_ixp4xx_wdt(wdd);

	__raw_writel(IXP4XX_WDT_KEY, iwdt->base + IXP4XX_OSWK_OFFSET);
	__raw_writel(0, iwdt->base + IXP4XX_OSWE_OFFSET);
	__raw_writel(wdd->timeout * iwdt->rate,
		     iwdt->base + IXP4XX_OSWT_OFFSET);
	__raw_writel(IXP4XX_WDT_COUNT_ENABLE | IXP4XX_WDT_RESET_ENABLE,
		     iwdt->base + IXP4XX_OSWE_OFFSET);
	__raw_writel(0, iwdt->base + IXP4XX_OSWK_OFFSET);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	return 0;
}

<<<<<<< HEAD

static const struct file_operations ixp4xx_wdt_fops = {
	.owner		= THIS_MODULE,
	.llseek		= no_llseek,
	.write		= ixp4xx_wdt_write,
	.unlocked_ioctl	= ixp4xx_wdt_ioctl,
	.open		= ixp4xx_wdt_open,
	.release	= ixp4xx_wdt_release,
};

static struct miscdevice ixp4xx_wdt_miscdev = {
	.minor		= WATCHDOG_MINOR,
	.name		= "watchdog",
	.fops		= &ixp4xx_wdt_fops,
};

static int __init ixp4xx_wdt_init(void)
{
	int ret;

	if (!(read_cpuid_id() & 0xf) && !cpu_is_ixp46x()) {
		pr_err("Rev. A0 IXP42x CPU detected - watchdog disabled\n");

		return -ENODEV;
	}
	boot_status = (*IXP4XX_OSST & IXP4XX_OSST_TIMER_WARM_RESET) ?
			WDIOF_CARDRESET : 0;
	ret = misc_register(&ixp4xx_wdt_miscdev);
	if (ret == 0)
		pr_info("timer heartbeat %d sec\n", heartbeat);
	return ret;
}

static void __exit ixp4xx_wdt_exit(void)
{
	misc_deregister(&ixp4xx_wdt_miscdev);
}


module_init(ixp4xx_wdt_init);
module_exit(ixp4xx_wdt_exit);

MODULE_AUTHOR("Deepak Saxena <dsaxena@plexity.net>");
MODULE_DESCRIPTION("IXP4xx Network Processor Watchdog");

module_param(heartbeat, int, 0);
MODULE_PARM_DESC(heartbeat, "Watchdog heartbeat in seconds (default 60s)");

module_param(nowayout, bool, 0);
MODULE_PARM_DESC(nowayout, "Watchdog cannot be stopped once started");

MODULE_LICENSE("GPL");
MODULE_ALIAS_MISCDEV(WATCHDOG_MINOR);

=======
static int ixp4xx_wdt_stop(struct watchdog_device *wdd)
{
	struct ixp4xx_wdt *iwdt = to_ixp4xx_wdt(wdd);

	__raw_writel(IXP4XX_WDT_KEY, iwdt->base + IXP4XX_OSWK_OFFSET);
	__raw_writel(0, iwdt->base + IXP4XX_OSWE_OFFSET);
	__raw_writel(0, iwdt->base + IXP4XX_OSWK_OFFSET);

	return 0;
}

static int ixp4xx_wdt_set_timeout(struct watchdog_device *wdd,
				  unsigned int timeout)
{
	wdd->timeout = timeout;
	if (watchdog_active(wdd))
		ixp4xx_wdt_start(wdd);

	return 0;
}

static int ixp4xx_wdt_restart(struct watchdog_device *wdd,
                              unsigned long action, void *data)
{
	struct ixp4xx_wdt *iwdt = to_ixp4xx_wdt(wdd);

	__raw_writel(IXP4XX_WDT_KEY, iwdt->base + IXP4XX_OSWK_OFFSET);
	__raw_writel(0, iwdt->base + IXP4XX_OSWT_OFFSET);
	__raw_writel(IXP4XX_WDT_COUNT_ENABLE | IXP4XX_WDT_RESET_ENABLE,
		     iwdt->base + IXP4XX_OSWE_OFFSET);

	return 0;
}

static const struct watchdog_ops ixp4xx_wdt_ops = {
	.start = ixp4xx_wdt_start,
	.stop = ixp4xx_wdt_stop,
	.set_timeout = ixp4xx_wdt_set_timeout,
	.restart = ixp4xx_wdt_restart,
	.owner = THIS_MODULE,
};

/*
 * The A0 version of the IXP422 had a bug in the watchdog making
 * is useless, but we still need to use it to restart the system
 * as it is the only way, so in this special case we register a
 * "dummy" watchdog that doesn't really work, but will support
 * the restart operation.
 */
static int ixp4xx_wdt_dummy(struct watchdog_device *wdd)
{
	return 0;
}

static const struct watchdog_ops ixp4xx_wdt_restart_only_ops = {
	.start = ixp4xx_wdt_dummy,
	.stop = ixp4xx_wdt_dummy,
	.restart = ixp4xx_wdt_restart,
	.owner = THIS_MODULE,
};

static const struct watchdog_info ixp4xx_wdt_info = {
	.options = WDIOF_KEEPALIVEPING
		| WDIOF_MAGICCLOSE
		| WDIOF_SETTIMEOUT,
	.identity = KBUILD_MODNAME,
};

static int ixp4xx_wdt_probe(struct platform_device *pdev)
{
	static const struct watchdog_ops *iwdt_ops;
	struct device *dev = &pdev->dev;
	struct ixp4xx_wdt *iwdt;
	struct clk *clk;
	int ret;

	if (!(read_cpuid_id() & 0xf) && !cpu_is_ixp46x()) {
		dev_info(dev, "Rev. A0 IXP42x CPU detected - only restart supported\n");
		iwdt_ops = &ixp4xx_wdt_restart_only_ops;
	} else {
		iwdt_ops = &ixp4xx_wdt_ops;
	}

	iwdt = devm_kzalloc(dev, sizeof(*iwdt), GFP_KERNEL);
	if (!iwdt)
		return -ENOMEM;
	iwdt->base = (void __iomem *)dev->platform_data;

	/*
	 * Retrieve rate from a fixed clock from the device tree if
	 * the parent has that, else use the default clock rate.
	 */
	clk = devm_clk_get_enabled(dev->parent, NULL);
	if (!IS_ERR(clk))
		iwdt->rate = clk_get_rate(clk);

	if (!iwdt->rate)
		iwdt->rate = IXP4XX_TIMER_FREQ;

	iwdt->wdd.info = &ixp4xx_wdt_info;
	iwdt->wdd.ops = iwdt_ops;
	iwdt->wdd.min_timeout = 1;
	iwdt->wdd.max_timeout = U32_MAX / iwdt->rate;
	iwdt->wdd.parent = dev;
	/* Default to 60 seconds */
	iwdt->wdd.timeout = 60U;
	watchdog_init_timeout(&iwdt->wdd, 0, dev);

	if (__raw_readl(iwdt->base + IXP4XX_OSST_OFFSET) &
	    IXP4XX_OSST_TIMER_WARM_RESET)
		iwdt->wdd.bootstatus = WDIOF_CARDRESET;

	ret = devm_watchdog_register_device(dev, &iwdt->wdd);
	if (ret)
		return ret;

	dev_info(dev, "IXP4xx watchdog available\n");

	return 0;
}

static struct platform_driver ixp4xx_wdt_driver = {
	.probe = ixp4xx_wdt_probe,
	.driver = {
		.name   = "ixp4xx-watchdog",
	},
};
module_platform_driver(ixp4xx_wdt_driver);

MODULE_AUTHOR("Deepak Saxena <dsaxena@plexity.net>");
MODULE_DESCRIPTION("IXP4xx Network Processor Watchdog");
MODULE_LICENSE("GPL");
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
