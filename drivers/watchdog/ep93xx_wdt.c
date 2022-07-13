<<<<<<< HEAD
=======
// SPDX-License-Identifier: GPL-2.0-only
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
/*
 * Watchdog driver for Cirrus Logic EP93xx family of devices.
 *
 * Copyright (c) 2004 Ray Lehtiniemi
 * Copyright (c) 2006 Tower Technologies
 * Based on ep93xx driver, bits from alim7101_wdt.c
 *
 * Authors: Ray Lehtiniemi <rayl@mail.com>,
 *	Alessandro Zummo <a.zummo@towertech.it>
 *
 * Copyright (c) 2012 H Hartley Sweeten <hsweeten@visionengravers.com>
 *	Convert to a platform device and use the watchdog framework API
 *
<<<<<<< HEAD
 * This file is licensed under the terms of the GNU General Public
 * License version 2. This program is licensed "as is" without any
 * warranty of any kind, whether express or implied.
 *
=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 * This watchdog fires after 250msec, which is a too short interval
 * for us to rely on the user space daemon alone. So we ping the
 * wdt each ~200msec and eventually stop doing it if the user space
 * daemon dies.
<<<<<<< HEAD
 *
 * TODO:
 *
 *	- Test last reset from watchdog status
 *	- Add a few missing ioctls
=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 */

#include <linux/platform_device.h>
#include <linux/module.h>
<<<<<<< HEAD
#include <linux/miscdevice.h>
#include <linux/watchdog.h>
#include <linux/timer.h>
#include <linux/io.h>

#define WDT_VERSION	"0.4"

=======
#include <linux/watchdog.h>
#include <linux/io.h>

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
/* default timeout (secs) */
#define WDT_TIMEOUT 30

static bool nowayout = WATCHDOG_NOWAYOUT;
module_param(nowayout, bool, 0);
MODULE_PARM_DESC(nowayout, "Watchdog cannot be stopped once started");

<<<<<<< HEAD
static unsigned int timeout = WDT_TIMEOUT;
module_param(timeout, uint, 0);
MODULE_PARM_DESC(timeout,
	"Watchdog timeout in seconds. (1<=timeout<=3600, default="
				__MODULE_STRING(WDT_TIMEOUT) ")");

static void __iomem *mmio_base;
static struct timer_list timer;
static unsigned long next_heartbeat;
=======
static unsigned int timeout;
module_param(timeout, uint, 0);
MODULE_PARM_DESC(timeout, "Watchdog timeout in seconds.");
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

#define EP93XX_WATCHDOG		0x00
#define EP93XX_WDSTATUS		0x04

<<<<<<< HEAD
/* reset the wdt every ~200ms - the heartbeat of the device is 0.250 seconds*/
#define WDT_INTERVAL (HZ/5)

static void ep93xx_wdt_timer_ping(unsigned long data)
{
	if (time_before(jiffies, next_heartbeat))
		writel(0x5555, mmio_base + EP93XX_WATCHDOG);

	/* Re-set the timer interval */
	mod_timer(&timer, jiffies + WDT_INTERVAL);
}

static int ep93xx_wdt_start(struct watchdog_device *wdd)
{
	next_heartbeat = jiffies + (timeout * HZ);

	writel(0xaaaa, mmio_base + EP93XX_WATCHDOG);
	mod_timer(&timer, jiffies + WDT_INTERVAL);
=======
struct ep93xx_wdt_priv {
	void __iomem *mmio;
	struct watchdog_device wdd;
};

static int ep93xx_wdt_start(struct watchdog_device *wdd)
{
	struct ep93xx_wdt_priv *priv = watchdog_get_drvdata(wdd);

	writel(0xaaaa, priv->mmio + EP93XX_WATCHDOG);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	return 0;
}

static int ep93xx_wdt_stop(struct watchdog_device *wdd)
{
<<<<<<< HEAD
	del_timer_sync(&timer);
	writel(0xaa55, mmio_base + EP93XX_WATCHDOG);
=======
	struct ep93xx_wdt_priv *priv = watchdog_get_drvdata(wdd);

	writel(0xaa55, priv->mmio + EP93XX_WATCHDOG);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	return 0;
}

<<<<<<< HEAD
static int ep93xx_wdt_keepalive(struct watchdog_device *wdd)
{
	/* user land ping */
	next_heartbeat = jiffies + (timeout * HZ);
=======
static int ep93xx_wdt_ping(struct watchdog_device *wdd)
{
	struct ep93xx_wdt_priv *priv = watchdog_get_drvdata(wdd);

	writel(0x5555, priv->mmio + EP93XX_WATCHDOG);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	return 0;
}

static const struct watchdog_info ep93xx_wdt_ident = {
	.options	= WDIOF_CARDRESET |
<<<<<<< HEAD
=======
			  WDIOF_SETTIMEOUT |
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
			  WDIOF_MAGICCLOSE |
			  WDIOF_KEEPALIVEPING,
	.identity	= "EP93xx Watchdog",
};

<<<<<<< HEAD
static struct watchdog_ops ep93xx_wdt_ops = {
	.owner		= THIS_MODULE,
	.start		= ep93xx_wdt_start,
	.stop		= ep93xx_wdt_stop,
	.ping		= ep93xx_wdt_keepalive,
};

static struct watchdog_device ep93xx_wdt_wdd = {
	.info		= &ep93xx_wdt_ident,
	.ops		= &ep93xx_wdt_ops,
};

static int __devinit ep93xx_wdt_probe(struct platform_device *pdev)
{
	struct resource *res;
	unsigned long val;
	int err;

	res = platform_get_resource(pdev, IORESOURCE_MEM, 0);
	if (!res)
		return -ENXIO;

	if (!devm_request_mem_region(&pdev->dev, res->start,
				     resource_size(res), pdev->name))
		return -EBUSY;

	mmio_base = devm_ioremap(&pdev->dev, res->start, resource_size(res));
	if (!mmio_base)
		return -ENXIO;

	if (timeout < 1 || timeout > 3600) {
		timeout = WDT_TIMEOUT;
		dev_warn(&pdev->dev,
			"timeout value must be 1<=x<=3600, using %d\n",
			timeout);
	}

	val = readl(mmio_base + EP93XX_WATCHDOG);
	ep93xx_wdt_wdd.bootstatus = (val & 0x01) ? WDIOF_CARDRESET : 0;
	ep93xx_wdt_wdd.timeout = timeout;

	watchdog_set_nowayout(&ep93xx_wdt_wdd, nowayout);

	setup_timer(&timer, ep93xx_wdt_timer_ping, 1);

	err = watchdog_register_device(&ep93xx_wdt_wdd);
	if (err)
		return err;

	dev_info(&pdev->dev,
		"EP93XX watchdog, driver version " WDT_VERSION "%s\n",
		(val & 0x08) ? " (nCS1 disable detected)" : "");
=======
static const struct watchdog_ops ep93xx_wdt_ops = {
	.owner		= THIS_MODULE,
	.start		= ep93xx_wdt_start,
	.stop		= ep93xx_wdt_stop,
	.ping		= ep93xx_wdt_ping,
};

static int ep93xx_wdt_probe(struct platform_device *pdev)
{
	struct device *dev = &pdev->dev;
	struct ep93xx_wdt_priv *priv;
	struct watchdog_device *wdd;
	unsigned long val;
	int ret;

	priv = devm_kzalloc(dev, sizeof(*priv), GFP_KERNEL);
	if (!priv)
		return -ENOMEM;

	priv->mmio = devm_platform_ioremap_resource(pdev, 0);
	if (IS_ERR(priv->mmio))
		return PTR_ERR(priv->mmio);

	val = readl(priv->mmio + EP93XX_WATCHDOG);

	wdd = &priv->wdd;
	wdd->bootstatus = (val & 0x01) ? WDIOF_CARDRESET : 0;
	wdd->info = &ep93xx_wdt_ident;
	wdd->ops = &ep93xx_wdt_ops;
	wdd->min_timeout = 1;
	wdd->max_hw_heartbeat_ms = 200;
	wdd->parent = dev;

	watchdog_set_nowayout(wdd, nowayout);

	wdd->timeout = WDT_TIMEOUT;
	watchdog_init_timeout(wdd, timeout, dev);

	watchdog_set_drvdata(wdd, priv);

	ret = devm_watchdog_register_device(dev, wdd);
	if (ret)
		return ret;

	dev_info(dev, "EP93XX watchdog driver %s\n",
		 (val & 0x08) ? " (nCS1 disable detected)" : "");
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	return 0;
}

<<<<<<< HEAD
static int __devexit ep93xx_wdt_remove(struct platform_device *pdev)
{
	watchdog_unregister_device(&ep93xx_wdt_wdd);
	return 0;
}

static struct platform_driver ep93xx_wdt_driver = {
	.driver		= {
		.owner	= THIS_MODULE,
		.name	= "ep93xx-wdt",
	},
	.probe		= ep93xx_wdt_probe,
	.remove		= __devexit_p(ep93xx_wdt_remove),
=======
static struct platform_driver ep93xx_wdt_driver = {
	.driver		= {
		.name	= "ep93xx-wdt",
	},
	.probe		= ep93xx_wdt_probe,
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
};

module_platform_driver(ep93xx_wdt_driver);

<<<<<<< HEAD
MODULE_AUTHOR("Ray Lehtiniemi <rayl@mail.com>,"
		"Alessandro Zummo <a.zummo@towertech.it>,"
		"H Hartley Sweeten <hsweeten@visionengravers.com>");
MODULE_DESCRIPTION("EP93xx Watchdog");
MODULE_LICENSE("GPL");
MODULE_VERSION(WDT_VERSION);
MODULE_ALIAS_MISCDEV(WATCHDOG_MINOR);
=======
MODULE_AUTHOR("Ray Lehtiniemi <rayl@mail.com>");
MODULE_AUTHOR("Alessandro Zummo <a.zummo@towertech.it>");
MODULE_AUTHOR("H Hartley Sweeten <hsweeten@visionengravers.com>");
MODULE_DESCRIPTION("EP93xx Watchdog");
MODULE_LICENSE("GPL");
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
