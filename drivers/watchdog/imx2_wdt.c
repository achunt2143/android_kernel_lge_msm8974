<<<<<<< HEAD
/*
 * Watchdog driver for IMX2 and later processors
 *
 *  Copyright (C) 2010 Wolfram Sang, Pengutronix e.K. <w.sang@pengutronix.de>
=======
// SPDX-License-Identifier: GPL-2.0
/*
 * Watchdog driver for IMX2 and later processors
 *
 *  Copyright (C) 2010 Wolfram Sang, Pengutronix e.K. <kernel@pengutronix.de>
 *  Copyright (C) 2014 Freescale Semiconductor, Inc.
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 *
 * some parts adapted by similar drivers from Darius Augulis and Vladimir
 * Zapolskiy, additional improvements by Wim Van Sebroeck.
 *
<<<<<<< HEAD
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License version 2 as published by
 * the Free Software Foundation.
 *
=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 * NOTE: MX1 has a slightly different Watchdog than MX2 and later:
 *
 *			MX1:		MX2+:
 *			----		-----
 * Registers:		32-bit		16-bit
 * Stopable timer:	Yes		No
 * Need to enable clk:	No		Yes
 * Halt on suspend:	Manual		Can be automatic
 */

<<<<<<< HEAD
#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/miscdevice.h>
#include <linux/module.h>
#include <linux/moduleparam.h>
#include <linux/platform_device.h>
#include <linux/watchdog.h>
#include <linux/clk.h>
#include <linux/fs.h>
#include <linux/io.h>
#include <linux/uaccess.h>
#include <linux/timer.h>
#include <linux/jiffies.h>
#include <mach/hardware.h>
=======
#include <linux/clk.h>
#include <linux/delay.h>
#include <linux/init.h>
#include <linux/interrupt.h>
#include <linux/io.h>
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/moduleparam.h>
#include <linux/of.h>
#include <linux/platform_device.h>
#include <linux/regmap.h>
#include <linux/watchdog.h>
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

#define DRIVER_NAME "imx2-wdt"

#define IMX2_WDT_WCR		0x00		/* Control Register */
#define IMX2_WDT_WCR_WT		(0xFF << 8)	/* -> Watchdog Timeout Field */
<<<<<<< HEAD
#define IMX2_WDT_WCR_WRE	(1 << 3)	/* -> WDOG Reset Enable */
#define IMX2_WDT_WCR_WDE	(1 << 2)	/* -> Watchdog Enable */
=======
#define IMX2_WDT_WCR_WDW	BIT(7)		/* -> Watchdog disable for WAIT */
#define IMX2_WDT_WCR_WDA	BIT(5)		/* -> External Reset WDOG_B */
#define IMX2_WDT_WCR_SRS	BIT(4)		/* -> Software Reset Signal */
#define IMX2_WDT_WCR_WRE	BIT(3)		/* -> WDOG Reset Enable */
#define IMX2_WDT_WCR_WDE	BIT(2)		/* -> Watchdog Enable */
#define IMX2_WDT_WCR_WDZST	BIT(0)		/* -> Watchdog timer Suspend */
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

#define IMX2_WDT_WSR		0x02		/* Service Register */
#define IMX2_WDT_SEQ1		0x5555		/* -> service sequence 1 */
#define IMX2_WDT_SEQ2		0xAAAA		/* -> service sequence 2 */

#define IMX2_WDT_WRSR		0x04		/* Reset Status Register */
<<<<<<< HEAD
#define IMX2_WDT_WRSR_TOUT	(1 << 1)	/* -> Reset due to Timeout */

#define IMX2_WDT_MAX_TIME	128
=======
#define IMX2_WDT_WRSR_TOUT	BIT(1)		/* -> Reset due to Timeout */

#define IMX2_WDT_WICR		0x06		/* Interrupt Control Register */
#define IMX2_WDT_WICR_WIE	BIT(15)		/* -> Interrupt Enable */
#define IMX2_WDT_WICR_WTIS	BIT(14)		/* -> Interrupt Status */
#define IMX2_WDT_WICR_WICT	0xFF		/* -> Interrupt Count Timeout */

#define IMX2_WDT_WMCR		0x08		/* Misc Register */

#define IMX2_WDT_MAX_TIME	128U
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#define IMX2_WDT_DEFAULT_TIME	60		/* in seconds */

#define WDOG_SEC_TO_COUNT(s)	((s * 2 - 1) << 8)

<<<<<<< HEAD
#define IMX2_WDT_STATUS_OPEN	0
#define IMX2_WDT_STATUS_STARTED	1
#define IMX2_WDT_EXPECT_CLOSE	2

static struct {
	struct clk *clk;
	void __iomem *base;
	unsigned timeout;
	unsigned long status;
	struct timer_list timer;	/* Pings the watchdog when closed */
} imx2_wdt;

static struct miscdevice imx2_wdt_miscdev;
=======
struct imx2_wdt_data {
	bool wdw_supported;
};

struct imx2_wdt_device {
	struct clk *clk;
	struct regmap *regmap;
	struct watchdog_device wdog;
	const struct imx2_wdt_data *data;
	bool ext_reset;
	bool clk_is_on;
	bool no_ping;
	bool sleep_wait;
};
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

static bool nowayout = WATCHDOG_NOWAYOUT;
module_param(nowayout, bool, 0);
MODULE_PARM_DESC(nowayout, "Watchdog cannot be stopped once started (default="
				__MODULE_STRING(WATCHDOG_NOWAYOUT) ")");

<<<<<<< HEAD

static unsigned timeout = IMX2_WDT_DEFAULT_TIME;
=======
static unsigned timeout;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
module_param(timeout, uint, 0);
MODULE_PARM_DESC(timeout, "Watchdog timeout in seconds (default="
				__MODULE_STRING(IMX2_WDT_DEFAULT_TIME) ")");

static const struct watchdog_info imx2_wdt_info = {
	.identity = "imx2+ watchdog",
	.options = WDIOF_KEEPALIVEPING | WDIOF_SETTIMEOUT | WDIOF_MAGICCLOSE,
};

<<<<<<< HEAD
static inline void imx2_wdt_setup(void)
{
	u16 val = __raw_readw(imx2_wdt.base + IMX2_WDT_WCR);

	/* Strip the old watchdog Time-Out value */
	val &= ~IMX2_WDT_WCR_WT;
	/* Generate reset if WDOG times out */
	val &= ~IMX2_WDT_WCR_WRE;
	/* Keep Watchdog Disabled */
	val &= ~IMX2_WDT_WCR_WDE;
	/* Set the watchdog's Time-Out value */
	val |= WDOG_SEC_TO_COUNT(imx2_wdt.timeout);

	__raw_writew(val, imx2_wdt.base + IMX2_WDT_WCR);

	/* enable the watchdog */
	val |= IMX2_WDT_WCR_WDE;
	__raw_writew(val, imx2_wdt.base + IMX2_WDT_WCR);
}

static inline void imx2_wdt_ping(void)
{
	__raw_writew(IMX2_WDT_SEQ1, imx2_wdt.base + IMX2_WDT_WSR);
	__raw_writew(IMX2_WDT_SEQ2, imx2_wdt.base + IMX2_WDT_WSR);
}

static void imx2_wdt_timer_ping(unsigned long arg)
{
	/* ping it every imx2_wdt.timeout / 2 seconds to prevent reboot */
	imx2_wdt_ping();
	mod_timer(&imx2_wdt.timer, jiffies + imx2_wdt.timeout * HZ / 2);
}

static void imx2_wdt_start(void)
{
	if (!test_and_set_bit(IMX2_WDT_STATUS_STARTED, &imx2_wdt.status)) {
		/* at our first start we enable clock and do initialisations */
		clk_enable(imx2_wdt.clk);

		imx2_wdt_setup();
	} else	/* delete the timer that pings the watchdog after close */
		del_timer_sync(&imx2_wdt.timer);

	/* Watchdog is enabled - time to reload the timeout value */
	imx2_wdt_ping();
}

static void imx2_wdt_stop(void)
{
	/* we don't need a clk_disable, it cannot be disabled once started.
	 * We use a timer to ping the watchdog while /dev/watchdog is closed */
	imx2_wdt_timer_ping(0);
}

static void imx2_wdt_set_timeout(int new_timeout)
{
	u16 val = __raw_readw(imx2_wdt.base + IMX2_WDT_WCR);

	/* set the new timeout value in the WSR */
	val &= ~IMX2_WDT_WCR_WT;
	val |= WDOG_SEC_TO_COUNT(new_timeout);
	__raw_writew(val, imx2_wdt.base + IMX2_WDT_WCR);
}

static int imx2_wdt_open(struct inode *inode, struct file *file)
{
	if (test_and_set_bit(IMX2_WDT_STATUS_OPEN, &imx2_wdt.status))
		return -EBUSY;

	imx2_wdt_start();
	return nonseekable_open(inode, file);
}

static int imx2_wdt_close(struct inode *inode, struct file *file)
{
	if (test_bit(IMX2_WDT_EXPECT_CLOSE, &imx2_wdt.status) && !nowayout)
		imx2_wdt_stop();
	else {
		dev_crit(imx2_wdt_miscdev.parent,
			"Unexpected close: Expect reboot!\n");
		imx2_wdt_ping();
	}

	clear_bit(IMX2_WDT_EXPECT_CLOSE, &imx2_wdt.status);
	clear_bit(IMX2_WDT_STATUS_OPEN, &imx2_wdt.status);
	return 0;
}

static long imx2_wdt_ioctl(struct file *file, unsigned int cmd,
							unsigned long arg)
{
	void __user *argp = (void __user *)arg;
	int __user *p = argp;
	int new_value;
	u16 val;

	switch (cmd) {
	case WDIOC_GETSUPPORT:
		return copy_to_user(argp, &imx2_wdt_info,
			sizeof(struct watchdog_info)) ? -EFAULT : 0;

	case WDIOC_GETSTATUS:
		return put_user(0, p);

	case WDIOC_GETBOOTSTATUS:
		val = __raw_readw(imx2_wdt.base + IMX2_WDT_WRSR);
		new_value = val & IMX2_WDT_WRSR_TOUT ? WDIOF_CARDRESET : 0;
		return put_user(new_value, p);

	case WDIOC_KEEPALIVE:
		imx2_wdt_ping();
		return 0;

	case WDIOC_SETTIMEOUT:
		if (get_user(new_value, p))
			return -EFAULT;
		if ((new_value < 1) || (new_value > IMX2_WDT_MAX_TIME))
			return -EINVAL;
		imx2_wdt_set_timeout(new_value);
		imx2_wdt.timeout = new_value;
		imx2_wdt_ping();

		/* Fallthrough to return current value */
	case WDIOC_GETTIMEOUT:
		return put_user(imx2_wdt.timeout, p);

	default:
		return -ENOTTY;
	}
}

static ssize_t imx2_wdt_write(struct file *file, const char __user *data,
						size_t len, loff_t *ppos)
{
	size_t i;
	char c;

	if (len == 0)	/* Can we see this even ? */
		return 0;

	clear_bit(IMX2_WDT_EXPECT_CLOSE, &imx2_wdt.status);
	/* scan to see whether or not we got the magic character */
	for (i = 0; i != len; i++) {
		if (get_user(c, data + i))
			return -EFAULT;
		if (c == 'V')
			set_bit(IMX2_WDT_EXPECT_CLOSE, &imx2_wdt.status);
	}

	imx2_wdt_ping();
	return len;
}

static const struct file_operations imx2_wdt_fops = {
	.owner = THIS_MODULE,
	.llseek = no_llseek,
	.unlocked_ioctl = imx2_wdt_ioctl,
	.open = imx2_wdt_open,
	.release = imx2_wdt_close,
	.write = imx2_wdt_write,
};

static struct miscdevice imx2_wdt_miscdev = {
	.minor = WATCHDOG_MINOR,
	.name = "watchdog",
	.fops = &imx2_wdt_fops,
};

static int __init imx2_wdt_probe(struct platform_device *pdev)
{
	int ret;
	struct resource *res;

	res = platform_get_resource(pdev, IORESOURCE_MEM, 0);
	if (!res) {
		dev_err(&pdev->dev, "can't get device resources\n");
		return -ENODEV;
	}

	imx2_wdt.base = devm_request_and_ioremap(&pdev->dev, res);
	if (!imx2_wdt.base) {
		dev_err(&pdev->dev, "ioremap failed\n");
		return -ENOMEM;
	}

	imx2_wdt.clk = clk_get(&pdev->dev, NULL);
	if (IS_ERR(imx2_wdt.clk)) {
		dev_err(&pdev->dev, "can't get Watchdog clock\n");
		return PTR_ERR(imx2_wdt.clk);
	}

	imx2_wdt.timeout = clamp_t(unsigned, timeout, 1, IMX2_WDT_MAX_TIME);
	if (imx2_wdt.timeout != timeout)
		dev_warn(&pdev->dev, "Initial timeout out of range! "
			"Clamped from %u to %u\n", timeout, imx2_wdt.timeout);

	setup_timer(&imx2_wdt.timer, imx2_wdt_timer_ping, 0);

	imx2_wdt_miscdev.parent = &pdev->dev;
	ret = misc_register(&imx2_wdt_miscdev);
	if (ret)
		goto fail;

	dev_info(&pdev->dev,
		"IMX2+ Watchdog Timer enabled. timeout=%ds (nowayout=%d)\n",
						imx2_wdt.timeout, nowayout);
	return 0;

fail:
	imx2_wdt_miscdev.parent = NULL;
	clk_put(imx2_wdt.clk);
	return ret;
}

static int __exit imx2_wdt_remove(struct platform_device *pdev)
{
	misc_deregister(&imx2_wdt_miscdev);

	if (test_bit(IMX2_WDT_STATUS_STARTED, &imx2_wdt.status)) {
		del_timer_sync(&imx2_wdt.timer);

		dev_crit(imx2_wdt_miscdev.parent,
			"Device removed: Expect reboot!\n");
	} else
		clk_put(imx2_wdt.clk);

	imx2_wdt_miscdev.parent = NULL;
	return 0;
=======
static const struct watchdog_info imx2_wdt_pretimeout_info = {
	.identity = "imx2+ watchdog",
	.options = WDIOF_KEEPALIVEPING | WDIOF_SETTIMEOUT | WDIOF_MAGICCLOSE |
		   WDIOF_PRETIMEOUT,
};

static int imx2_wdt_restart(struct watchdog_device *wdog, unsigned long action,
			    void *data)
{
	struct imx2_wdt_device *wdev = watchdog_get_drvdata(wdog);
	unsigned int wcr_enable = IMX2_WDT_WCR_WDE;

	/* Use internal reset or external - not both */
	if (wdev->ext_reset)
		wcr_enable |= IMX2_WDT_WCR_SRS; /* do not assert int reset */
	else
		wcr_enable |= IMX2_WDT_WCR_WDA; /* do not assert ext-reset */

	/* Assert SRS signal */
	regmap_write(wdev->regmap, IMX2_WDT_WCR, wcr_enable);
	/*
	 * Due to imx6q errata ERR004346 (WDOG: WDOG SRS bit requires to be
	 * written twice), we add another two writes to ensure there must be at
	 * least two writes happen in the same one 32kHz clock period.  We save
	 * the target check here, since the writes shouldn't be a huge burden
	 * for other platforms.
	 */
	regmap_write(wdev->regmap, IMX2_WDT_WCR, wcr_enable);
	regmap_write(wdev->regmap, IMX2_WDT_WCR, wcr_enable);

	/* wait for reset to assert... */
	mdelay(500);

	return 0;
}

static inline void imx2_wdt_setup(struct watchdog_device *wdog)
{
	struct imx2_wdt_device *wdev = watchdog_get_drvdata(wdog);
	u32 val;

	regmap_read(wdev->regmap, IMX2_WDT_WCR, &val);

	/* Suspend timer in low power mode, write once-only */
	val |= IMX2_WDT_WCR_WDZST;
	/* Suspend timer in low power WAIT mode, write once-only */
	if (wdev->sleep_wait)
		val |= IMX2_WDT_WCR_WDW;
	/* Strip the old watchdog Time-Out value */
	val &= ~IMX2_WDT_WCR_WT;
	/* Generate internal chip-level reset if WDOG times out */
	if (!wdev->ext_reset)
		val &= ~IMX2_WDT_WCR_WRE;
	/* Or if external-reset assert WDOG_B reset only on time-out */
	else
		val |= IMX2_WDT_WCR_WRE;
	/* Keep Watchdog Disabled */
	val &= ~IMX2_WDT_WCR_WDE;
	/* Set the watchdog's Time-Out value */
	val |= WDOG_SEC_TO_COUNT(wdog->timeout);

	regmap_write(wdev->regmap, IMX2_WDT_WCR, val);

	/* enable the watchdog */
	val |= IMX2_WDT_WCR_WDE;
	regmap_write(wdev->regmap, IMX2_WDT_WCR, val);
}

static inline bool imx2_wdt_is_running(struct imx2_wdt_device *wdev)
{
	u32 val;

	regmap_read(wdev->regmap, IMX2_WDT_WCR, &val);

	return val & IMX2_WDT_WCR_WDE;
}

static int imx2_wdt_ping(struct watchdog_device *wdog)
{
	struct imx2_wdt_device *wdev = watchdog_get_drvdata(wdog);

	if (!wdev->clk_is_on)
		return 0;

	regmap_write(wdev->regmap, IMX2_WDT_WSR, IMX2_WDT_SEQ1);
	regmap_write(wdev->regmap, IMX2_WDT_WSR, IMX2_WDT_SEQ2);
	return 0;
}

static void __imx2_wdt_set_timeout(struct watchdog_device *wdog,
				   unsigned int new_timeout)
{
	struct imx2_wdt_device *wdev = watchdog_get_drvdata(wdog);

	regmap_update_bits(wdev->regmap, IMX2_WDT_WCR, IMX2_WDT_WCR_WT,
			   WDOG_SEC_TO_COUNT(new_timeout));
}

static int imx2_wdt_set_timeout(struct watchdog_device *wdog,
				unsigned int new_timeout)
{
	unsigned int actual;

	actual = min(new_timeout, IMX2_WDT_MAX_TIME);
	__imx2_wdt_set_timeout(wdog, actual);
	wdog->timeout = new_timeout;
	return 0;
}

static int imx2_wdt_set_pretimeout(struct watchdog_device *wdog,
				   unsigned int new_pretimeout)
{
	struct imx2_wdt_device *wdev = watchdog_get_drvdata(wdog);

	if (new_pretimeout >= IMX2_WDT_MAX_TIME)
		return -EINVAL;

	wdog->pretimeout = new_pretimeout;

	regmap_update_bits(wdev->regmap, IMX2_WDT_WICR,
			   IMX2_WDT_WICR_WIE | IMX2_WDT_WICR_WICT,
			   IMX2_WDT_WICR_WIE | (new_pretimeout << 1));
	return 0;
}

static irqreturn_t imx2_wdt_isr(int irq, void *wdog_arg)
{
	struct watchdog_device *wdog = wdog_arg;
	struct imx2_wdt_device *wdev = watchdog_get_drvdata(wdog);

	regmap_write_bits(wdev->regmap, IMX2_WDT_WICR,
			  IMX2_WDT_WICR_WTIS, IMX2_WDT_WICR_WTIS);

	watchdog_notify_pretimeout(wdog);

	return IRQ_HANDLED;
}

static int imx2_wdt_start(struct watchdog_device *wdog)
{
	struct imx2_wdt_device *wdev = watchdog_get_drvdata(wdog);

	if (imx2_wdt_is_running(wdev))
		imx2_wdt_set_timeout(wdog, wdog->timeout);
	else
		imx2_wdt_setup(wdog);

	set_bit(WDOG_HW_RUNNING, &wdog->status);

	return imx2_wdt_ping(wdog);
}

static const struct watchdog_ops imx2_wdt_ops = {
	.owner = THIS_MODULE,
	.start = imx2_wdt_start,
	.ping = imx2_wdt_ping,
	.set_timeout = imx2_wdt_set_timeout,
	.set_pretimeout = imx2_wdt_set_pretimeout,
	.restart = imx2_wdt_restart,
};

static const struct regmap_config imx2_wdt_regmap_config = {
	.reg_bits = 16,
	.reg_stride = 2,
	.val_bits = 16,
	.max_register = 0x8,
};

static void imx2_wdt_action(void *data)
{
	clk_disable_unprepare(data);
}

static int __init imx2_wdt_probe(struct platform_device *pdev)
{
	struct device *dev = &pdev->dev;
	struct imx2_wdt_device *wdev;
	struct watchdog_device *wdog;
	void __iomem *base;
	int ret;
	u32 val;

	wdev = devm_kzalloc(dev, sizeof(*wdev), GFP_KERNEL);
	if (!wdev)
		return -ENOMEM;

	base = devm_platform_ioremap_resource(pdev, 0);
	if (IS_ERR(base))
		return PTR_ERR(base);

	wdev->regmap = devm_regmap_init_mmio_clk(dev, NULL, base,
						 &imx2_wdt_regmap_config);
	if (IS_ERR(wdev->regmap)) {
		dev_err(dev, "regmap init failed\n");
		return PTR_ERR(wdev->regmap);
	}

	wdev->clk = devm_clk_get(dev, NULL);
	if (IS_ERR(wdev->clk)) {
		dev_err(dev, "can't get Watchdog clock\n");
		return PTR_ERR(wdev->clk);
	}

	wdog			= &wdev->wdog;
	wdog->info		= &imx2_wdt_info;
	wdog->ops		= &imx2_wdt_ops;
	wdog->min_timeout	= 1;
	wdog->timeout		= IMX2_WDT_DEFAULT_TIME;
	wdog->max_hw_heartbeat_ms = IMX2_WDT_MAX_TIME * 1000;
	wdog->parent		= dev;

	wdev->data = of_device_get_match_data(dev);

	ret = platform_get_irq(pdev, 0);
	if (ret > 0)
		if (!devm_request_irq(dev, ret, imx2_wdt_isr, 0,
				      dev_name(dev), wdog))
			wdog->info = &imx2_wdt_pretimeout_info;

	ret = clk_prepare_enable(wdev->clk);
	if (ret)
		return ret;

	ret = devm_add_action_or_reset(dev, imx2_wdt_action, wdev->clk);
	if (ret)
		return ret;

	wdev->clk_is_on = true;

	regmap_read(wdev->regmap, IMX2_WDT_WRSR, &val);
	wdog->bootstatus = val & IMX2_WDT_WRSR_TOUT ? WDIOF_CARDRESET : 0;

	wdev->ext_reset = of_property_read_bool(dev->of_node,
						"fsl,ext-reset-output");

	if (of_property_read_bool(dev->of_node, "fsl,suspend-in-wait")) {
		if (!wdev->data->wdw_supported) {
			dev_err(dev, "suspend-in-wait not supported\n");
			return -EINVAL;
		}
		wdev->sleep_wait = true;
	}

	/*
	 * The i.MX7D doesn't support low power mode, so we need to ping the watchdog
	 * during suspend. Interaction with "fsl,suspend-in-wait" is unknown!
	 */
	wdev->no_ping = !of_device_is_compatible(dev->of_node, "fsl,imx7d-wdt");
	platform_set_drvdata(pdev, wdog);
	watchdog_set_drvdata(wdog, wdev);
	watchdog_set_nowayout(wdog, nowayout);
	watchdog_set_restart_priority(wdog, 128);
	watchdog_init_timeout(wdog, timeout, dev);
	if (wdev->no_ping)
		watchdog_stop_ping_on_suspend(wdog);

	if (imx2_wdt_is_running(wdev)) {
		imx2_wdt_set_timeout(wdog, wdog->timeout);
		set_bit(WDOG_HW_RUNNING, &wdog->status);
	}

	/*
	 * Disable the watchdog power down counter at boot. Otherwise the power
	 * down counter will pull down the #WDOG interrupt line for one clock
	 * cycle.
	 */
	regmap_write(wdev->regmap, IMX2_WDT_WMCR, 0);

	return devm_watchdog_register_device(dev, wdog);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

static void imx2_wdt_shutdown(struct platform_device *pdev)
{
<<<<<<< HEAD
	if (test_bit(IMX2_WDT_STATUS_STARTED, &imx2_wdt.status)) {
		/* we are running, we need to delete the timer but will give
		 * max timeout before reboot will take place */
		del_timer_sync(&imx2_wdt.timer);
		imx2_wdt_set_timeout(IMX2_WDT_MAX_TIME);
		imx2_wdt_ping();

		dev_crit(imx2_wdt_miscdev.parent,
			"Device shutdown: Expect reboot!\n");
	}
}

static const struct of_device_id imx2_wdt_dt_ids[] = {
	{ .compatible = "fsl,imx21-wdt", },
	{ /* sentinel */ }
};

static struct platform_driver imx2_wdt_driver = {
	.remove		= __exit_p(imx2_wdt_remove),
	.shutdown	= imx2_wdt_shutdown,
	.driver		= {
		.name	= DRIVER_NAME,
		.owner	= THIS_MODULE,
=======
	struct watchdog_device *wdog = platform_get_drvdata(pdev);
	struct imx2_wdt_device *wdev = watchdog_get_drvdata(wdog);

	if (imx2_wdt_is_running(wdev)) {
		/*
		 * We are running, configure max timeout before reboot
		 * will take place.
		 */
		imx2_wdt_set_timeout(wdog, IMX2_WDT_MAX_TIME);
		imx2_wdt_ping(wdog);
		dev_crit(&pdev->dev, "Device shutdown.\n");
	}
}

/* Disable watchdog if it is active or non-active but still running */
static int __maybe_unused imx2_wdt_suspend(struct device *dev)
{
	struct watchdog_device *wdog = dev_get_drvdata(dev);
	struct imx2_wdt_device *wdev = watchdog_get_drvdata(wdog);

	/* The watchdog IP block is running */
	if (imx2_wdt_is_running(wdev)) {
		/*
		 * Don't update wdog->timeout, we'll restore the current value
		 * during resume.
		 */
		__imx2_wdt_set_timeout(wdog, IMX2_WDT_MAX_TIME);
		imx2_wdt_ping(wdog);
	}

	if (wdev->no_ping) {
		clk_disable_unprepare(wdev->clk);

		wdev->clk_is_on = false;
	}

	return 0;
}

/* Enable watchdog and configure it if necessary */
static int __maybe_unused imx2_wdt_resume(struct device *dev)
{
	struct watchdog_device *wdog = dev_get_drvdata(dev);
	struct imx2_wdt_device *wdev = watchdog_get_drvdata(wdog);
	int ret;

	if (wdev->no_ping) {
		ret = clk_prepare_enable(wdev->clk);

		if (ret)
			return ret;

		wdev->clk_is_on = true;
	}

	if (watchdog_active(wdog) && !imx2_wdt_is_running(wdev)) {
		/*
		 * If the watchdog is still active and resumes
		 * from deep sleep state, need to restart the
		 * watchdog again.
		 */
		imx2_wdt_setup(wdog);
	}
	if (imx2_wdt_is_running(wdev)) {
		imx2_wdt_set_timeout(wdog, wdog->timeout);
		imx2_wdt_ping(wdog);
	}

	return 0;
}

static SIMPLE_DEV_PM_OPS(imx2_wdt_pm_ops, imx2_wdt_suspend,
			 imx2_wdt_resume);

static struct imx2_wdt_data imx_wdt = {
	.wdw_supported = true,
};

static struct imx2_wdt_data imx_wdt_legacy = {
	.wdw_supported = false,
};

static const struct of_device_id imx2_wdt_dt_ids[] = {
	{ .compatible = "fsl,imx21-wdt", .data = &imx_wdt_legacy },
	{ .compatible = "fsl,imx25-wdt", .data = &imx_wdt },
	{ .compatible = "fsl,imx27-wdt", .data = &imx_wdt_legacy },
	{ .compatible = "fsl,imx31-wdt", .data = &imx_wdt_legacy },
	{ .compatible = "fsl,imx35-wdt", .data = &imx_wdt },
	{ .compatible = "fsl,imx50-wdt", .data = &imx_wdt },
	{ .compatible = "fsl,imx51-wdt", .data = &imx_wdt },
	{ .compatible = "fsl,imx53-wdt", .data = &imx_wdt },
	{ .compatible = "fsl,imx6q-wdt", .data = &imx_wdt },
	{ .compatible = "fsl,imx6sl-wdt", .data = &imx_wdt },
	{ .compatible = "fsl,imx6sll-wdt", .data = &imx_wdt },
	{ .compatible = "fsl,imx6sx-wdt", .data = &imx_wdt },
	{ .compatible = "fsl,imx6ul-wdt", .data = &imx_wdt },
	{ .compatible = "fsl,imx7d-wdt", .data = &imx_wdt },
	{ .compatible = "fsl,imx8mm-wdt", .data = &imx_wdt },
	{ .compatible = "fsl,imx8mn-wdt", .data = &imx_wdt },
	{ .compatible = "fsl,imx8mp-wdt", .data = &imx_wdt },
	{ .compatible = "fsl,imx8mq-wdt", .data = &imx_wdt },
	{ .compatible = "fsl,ls1012a-wdt", .data = &imx_wdt_legacy },
	{ .compatible = "fsl,ls1043a-wdt", .data = &imx_wdt_legacy },
	{ .compatible = "fsl,vf610-wdt", .data = &imx_wdt },
	{ /* sentinel */ }
};
MODULE_DEVICE_TABLE(of, imx2_wdt_dt_ids);

static struct platform_driver imx2_wdt_driver = {
	.shutdown	= imx2_wdt_shutdown,
	.driver		= {
		.name	= DRIVER_NAME,
		.pm     = &imx2_wdt_pm_ops,
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		.of_match_table = imx2_wdt_dt_ids,
	},
};

<<<<<<< HEAD
static int __init imx2_wdt_init(void)
{
	return platform_driver_probe(&imx2_wdt_driver, imx2_wdt_probe);
}
module_init(imx2_wdt_init);

static void __exit imx2_wdt_exit(void)
{
	platform_driver_unregister(&imx2_wdt_driver);
}
module_exit(imx2_wdt_exit);
=======
module_platform_driver_probe(imx2_wdt_driver, imx2_wdt_probe);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

MODULE_AUTHOR("Wolfram Sang");
MODULE_DESCRIPTION("Watchdog driver for IMX2 and later");
MODULE_LICENSE("GPL v2");
<<<<<<< HEAD
MODULE_ALIAS_MISCDEV(WATCHDOG_MINOR);
=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
MODULE_ALIAS("platform:" DRIVER_NAME);
