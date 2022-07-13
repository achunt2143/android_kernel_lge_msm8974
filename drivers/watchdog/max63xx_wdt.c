<<<<<<< HEAD
=======
// SPDX-License-Identifier: GPL-2.0-only
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
/*
 * drivers/char/watchdog/max63xx_wdt.c
 *
 * Driver for max63{69,70,71,72,73,74} watchdog timers
 *
 * Copyright (C) 2009 Marc Zyngier <maz@misterjones.org>
 *
<<<<<<< HEAD
 * This file is licensed under the terms of the GNU General Public
 * License version 2. This program is licensed "as is" without any
 * warranty of any kind, whether express or implied.
 *
=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 * This driver assumes the watchdog pins are memory mapped (as it is
 * the case for the Arcom Zeus). Should it be connected over GPIOs or
 * another interface, some abstraction will have to be introduced.
 */

<<<<<<< HEAD
#include <linux/module.h>
#include <linux/moduleparam.h>
#include <linux/types.h>
#include <linux/kernel.h>
#include <linux/miscdevice.h>
#include <linux/watchdog.h>
#include <linux/init.h>
=======
#include <linux/err.h>
#include <linux/module.h>
#include <linux/moduleparam.h>
#include <linux/mod_devicetable.h>
#include <linux/types.h>
#include <linux/kernel.h>
#include <linux/watchdog.h>
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#include <linux/bitops.h>
#include <linux/platform_device.h>
#include <linux/spinlock.h>
#include <linux/io.h>
#include <linux/slab.h>
<<<<<<< HEAD
=======
#include <linux/property.h>
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

#define DEFAULT_HEARTBEAT 60
#define MAX_HEARTBEAT     60

static unsigned int heartbeat = DEFAULT_HEARTBEAT;
static bool nowayout  = WATCHDOG_NOWAYOUT;

/*
 * Memory mapping: a single byte, 3 first lower bits to select bit 3
 * to ping the watchdog.
 */
#define MAX6369_WDSET	(7 << 0)
#define MAX6369_WDI	(1 << 3)

<<<<<<< HEAD
static DEFINE_SPINLOCK(io_lock);

static int nodelay;
static void __iomem	*wdt_base;
=======
#define MAX6369_WDSET_DISABLED	3

static int nodelay;

struct max63xx_wdt {
	struct watchdog_device wdd;
	const struct max63xx_timeout *timeout;

	/* memory mapping */
	void __iomem *base;
	spinlock_t lock;

	/* WDI and WSET bits write access routines */
	void (*ping)(struct max63xx_wdt *wdt);
	void (*set)(struct max63xx_wdt *wdt, u8 set);
};
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

/*
 * The timeout values used are actually the absolute minimum the chip
 * offers. Typical values on my board are slightly over twice as long
 * (10s setting ends up with a 25s timeout), and can be up to 3 times
 * the nominal setting (according to the datasheet). So please take
 * these values with a grain of salt. Same goes for the initial delay
 * "feature". Only max6373/74 have a few settings without this initial
 * delay (selected with the "nodelay" parameter).
 *
 * I also decided to remove from the tables any timeout smaller than a
 * second, as it looked completly overkill...
 */

/* Timeouts in second */
struct max63xx_timeout {
<<<<<<< HEAD
	u8 wdset;
	u8 tdelay;
	u8 twd;
};

static struct max63xx_timeout max6369_table[] = {
=======
	const u8 wdset;
	const u8 tdelay;
	const u8 twd;
};

static const struct max63xx_timeout max6369_table[] = {
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	{ 5,  1,  1 },
	{ 6, 10, 10 },
	{ 7, 60, 60 },
	{ },
};

<<<<<<< HEAD
static struct max63xx_timeout max6371_table[] = {
=======
static const struct max63xx_timeout max6371_table[] = {
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	{ 6, 60,  3 },
	{ 7, 60, 60 },
	{ },
};

<<<<<<< HEAD
static struct max63xx_timeout max6373_table[] = {
=======
static const struct max63xx_timeout max6373_table[] = {
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	{ 2, 60,  1 },
	{ 5,  0,  1 },
	{ 1,  3,  3 },
	{ 7, 60, 10 },
	{ 6,  0, 10 },
	{ },
};

<<<<<<< HEAD
static struct max63xx_timeout *current_timeout;

static struct max63xx_timeout *
max63xx_select_timeout(struct max63xx_timeout *table, int value)
=======
static const struct max63xx_timeout *
max63xx_select_timeout(const struct max63xx_timeout *table, int value)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
{
	while (table->twd) {
		if (value <= table->twd) {
			if (nodelay && table->tdelay == 0)
				return table;

			if (!nodelay)
				return table;
		}

		table++;
	}

	return NULL;
}

static int max63xx_wdt_ping(struct watchdog_device *wdd)
{
<<<<<<< HEAD
	u8 val;

	spin_lock(&io_lock);

	val = __raw_readb(wdt_base);

	__raw_writeb(val | MAX6369_WDI, wdt_base);
	__raw_writeb(val & ~MAX6369_WDI, wdt_base);

	spin_unlock(&io_lock);
=======
	struct max63xx_wdt *wdt = watchdog_get_drvdata(wdd);

	wdt->ping(wdt);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	return 0;
}

static int max63xx_wdt_start(struct watchdog_device *wdd)
{
<<<<<<< HEAD
	struct max63xx_timeout *entry = watchdog_get_drvdata(wdd);
	u8 val;

	spin_lock(&io_lock);

	val = __raw_readb(wdt_base);
	val &= ~MAX6369_WDSET;
	val |= entry->wdset;
	__raw_writeb(val, wdt_base);

	spin_unlock(&io_lock);

	/* check for a edge triggered startup */
	if (entry->tdelay == 0)
		max63xx_wdt_ping(wdd);
=======
	struct max63xx_wdt *wdt = watchdog_get_drvdata(wdd);

	wdt->set(wdt, wdt->timeout->wdset);

	/* check for a edge triggered startup */
	if (wdt->timeout->tdelay == 0)
		wdt->ping(wdt);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	return 0;
}

static int max63xx_wdt_stop(struct watchdog_device *wdd)
{
<<<<<<< HEAD
	u8 val;

	spin_lock(&io_lock);

	val = __raw_readb(wdt_base);
	val &= ~MAX6369_WDSET;
	val |= 3;
	__raw_writeb(val, wdt_base);

	spin_unlock(&io_lock);
	return 0;
}

static const struct watchdog_info max63xx_wdt_info = {
	.options = WDIOF_KEEPALIVEPING | WDIOF_MAGICCLOSE,
	.identity = "max63xx Watchdog",
};

=======
	struct max63xx_wdt *wdt = watchdog_get_drvdata(wdd);

	wdt->set(wdt, MAX6369_WDSET_DISABLED);
	return 0;
}

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
static const struct watchdog_ops max63xx_wdt_ops = {
	.owner = THIS_MODULE,
	.start = max63xx_wdt_start,
	.stop = max63xx_wdt_stop,
	.ping = max63xx_wdt_ping,
};

<<<<<<< HEAD
static struct watchdog_device max63xx_wdt_dev = {
	.info = &max63xx_wdt_info,
	.ops = &max63xx_wdt_ops,
};

static int __devinit max63xx_wdt_probe(struct platform_device *pdev)
{
	struct resource	*wdt_mem;
	struct max63xx_timeout *table;

	table = (struct max63xx_timeout *)pdev->id_entry->driver_data;
=======
static const struct watchdog_info max63xx_wdt_info = {
	.options = WDIOF_KEEPALIVEPING | WDIOF_MAGICCLOSE,
	.identity = "max63xx Watchdog",
};

static void max63xx_mmap_ping(struct max63xx_wdt *wdt)
{
	u8 val;

	spin_lock(&wdt->lock);

	val = __raw_readb(wdt->base);

	__raw_writeb(val | MAX6369_WDI, wdt->base);
	__raw_writeb(val & ~MAX6369_WDI, wdt->base);

	spin_unlock(&wdt->lock);
}

static void max63xx_mmap_set(struct max63xx_wdt *wdt, u8 set)
{
	u8 val;

	spin_lock(&wdt->lock);

	val = __raw_readb(wdt->base);
	val &= ~MAX6369_WDSET;
	val |= set & MAX6369_WDSET;
	__raw_writeb(val, wdt->base);

	spin_unlock(&wdt->lock);
}

static int max63xx_mmap_init(struct platform_device *p, struct max63xx_wdt *wdt)
{
	wdt->base = devm_platform_ioremap_resource(p, 0);
	if (IS_ERR(wdt->base))
		return PTR_ERR(wdt->base);

	spin_lock_init(&wdt->lock);

	wdt->ping = max63xx_mmap_ping;
	wdt->set = max63xx_mmap_set;
	return 0;
}

static int max63xx_wdt_probe(struct platform_device *pdev)
{
	struct device *dev = &pdev->dev;
	struct max63xx_wdt *wdt;
	const struct max63xx_timeout *table;
	int err;

	wdt = devm_kzalloc(dev, sizeof(*wdt), GFP_KERNEL);
	if (!wdt)
		return -ENOMEM;

	/* Attempt to use fwnode first */
	table = device_get_match_data(dev);
	if (!table)
		table = (struct max63xx_timeout *)pdev->id_entry->driver_data;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	if (heartbeat < 1 || heartbeat > MAX_HEARTBEAT)
		heartbeat = DEFAULT_HEARTBEAT;

<<<<<<< HEAD
	dev_info(&pdev->dev, "requesting %ds heartbeat\n", heartbeat);
	current_timeout = max63xx_select_timeout(table, heartbeat);

	if (!current_timeout) {
		dev_err(&pdev->dev, "unable to satisfy heartbeat request\n");
		return -EINVAL;
	}

	dev_info(&pdev->dev, "using %ds heartbeat with %ds initial delay\n",
		 current_timeout->twd, current_timeout->tdelay);

	heartbeat = current_timeout->twd;

	wdt_mem = platform_get_resource(pdev, IORESOURCE_MEM, 0);
	wdt_base = devm_request_and_ioremap(&pdev->dev, wdt_mem);
	if (!wdt_base)
		return -ENOMEM;

	max63xx_wdt_dev.timeout = heartbeat;
	watchdog_set_nowayout(&max63xx_wdt_dev, nowayout);
	watchdog_set_drvdata(&max63xx_wdt_dev, current_timeout);

	return watchdog_register_device(&max63xx_wdt_dev);
}

static int __devexit max63xx_wdt_remove(struct platform_device *pdev)
{
	watchdog_unregister_device(&max63xx_wdt_dev);
	return 0;
}

static struct platform_device_id max63xx_id_table[] = {
=======
	wdt->timeout = max63xx_select_timeout(table, heartbeat);
	if (!wdt->timeout) {
		dev_err(dev, "unable to satisfy %ds heartbeat request\n",
			heartbeat);
		return -EINVAL;
	}

	err = max63xx_mmap_init(pdev, wdt);
	if (err)
		return err;

	platform_set_drvdata(pdev, &wdt->wdd);
	watchdog_set_drvdata(&wdt->wdd, wdt);

	wdt->wdd.parent = dev;
	wdt->wdd.timeout = wdt->timeout->twd;
	wdt->wdd.info = &max63xx_wdt_info;
	wdt->wdd.ops = &max63xx_wdt_ops;

	watchdog_set_nowayout(&wdt->wdd, nowayout);

	err = devm_watchdog_register_device(dev, &wdt->wdd);
	if (err)
		return err;

	dev_info(dev, "using %ds heartbeat with %ds initial delay\n",
		 wdt->timeout->twd, wdt->timeout->tdelay);
	return 0;
}

static const struct platform_device_id max63xx_id_table[] = {
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	{ "max6369_wdt", (kernel_ulong_t)max6369_table, },
	{ "max6370_wdt", (kernel_ulong_t)max6369_table, },
	{ "max6371_wdt", (kernel_ulong_t)max6371_table, },
	{ "max6372_wdt", (kernel_ulong_t)max6371_table, },
	{ "max6373_wdt", (kernel_ulong_t)max6373_table, },
	{ "max6374_wdt", (kernel_ulong_t)max6373_table, },
	{ },
};
MODULE_DEVICE_TABLE(platform, max63xx_id_table);

<<<<<<< HEAD
static struct platform_driver max63xx_wdt_driver = {
	.probe		= max63xx_wdt_probe,
	.remove		= __devexit_p(max63xx_wdt_remove),
	.id_table	= max63xx_id_table,
	.driver		= {
		.name	= "max63xx_wdt",
		.owner	= THIS_MODULE,
=======
static const struct of_device_id max63xx_dt_id_table[] = {
	{ .compatible = "maxim,max6369", .data = max6369_table, },
	{ .compatible = "maxim,max6370", .data = max6369_table, },
	{ .compatible = "maxim,max6371", .data = max6371_table, },
	{ .compatible = "maxim,max6372", .data = max6371_table, },
	{ .compatible = "maxim,max6373", .data = max6373_table, },
	{ .compatible = "maxim,max6374", .data = max6373_table, },
	{ }
};
MODULE_DEVICE_TABLE(of, max63xx_dt_id_table);

static struct platform_driver max63xx_wdt_driver = {
	.probe		= max63xx_wdt_probe,
	.id_table	= max63xx_id_table,
	.driver		= {
		.name	= "max63xx_wdt",
		.of_match_table = max63xx_dt_id_table,
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	},
};

module_platform_driver(max63xx_wdt_driver);

MODULE_AUTHOR("Marc Zyngier <maz@misterjones.org>");
MODULE_DESCRIPTION("max63xx Watchdog Driver");

module_param(heartbeat, int, 0);
MODULE_PARM_DESC(heartbeat,
		 "Watchdog heartbeat period in seconds from 1 to "
		 __MODULE_STRING(MAX_HEARTBEAT) ", default "
		 __MODULE_STRING(DEFAULT_HEARTBEAT));

module_param(nowayout, bool, 0);
MODULE_PARM_DESC(nowayout, "Watchdog cannot be stopped once started (default="
		 __MODULE_STRING(WATCHDOG_NOWAYOUT) ")");

module_param(nodelay, int, 0);
MODULE_PARM_DESC(nodelay,
		 "Force selection of a timeout setting without initial delay "
		 "(max6373/74 only, default=0)");

<<<<<<< HEAD
MODULE_LICENSE("GPL");
MODULE_ALIAS_MISCDEV(WATCHDOG_MINOR);
=======
MODULE_LICENSE("GPL v2");
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
