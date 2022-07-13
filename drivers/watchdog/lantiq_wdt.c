<<<<<<< HEAD
/*
 *  This program is free software; you can redistribute it and/or modify it
 *  under the terms of the GNU General Public License version 2 as published
 *  by the Free Software Foundation.
 *
 *  Copyright (C) 2010 John Crispin <blogic@openwrt.org>
 *  Based on EP93xx wdt driver
 */

#define pr_fmt(fmt) KBUILD_MODNAME ": " fmt

#include <linux/module.h>
#include <linux/fs.h>
#include <linux/miscdevice.h>
#include <linux/watchdog.h>
=======
// SPDX-License-Identifier: GPL-2.0-only
/*
 *
 *  Copyright (C) 2010 John Crispin <john@phrozen.org>
 *  Copyright (C) 2017 Hauke Mehrtens <hauke@hauke-m.de>
 *  Based on EP93xx wdt driver
 */

#include <linux/module.h>
#include <linux/bitops.h>
#include <linux/watchdog.h>
#include <linux/of.h>
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#include <linux/platform_device.h>
#include <linux/uaccess.h>
#include <linux/clk.h>
#include <linux/io.h>
<<<<<<< HEAD

#include <lantiq.h>

/* Section 3.4 of the datasheet
=======
#include <linux/regmap.h>
#include <linux/mfd/syscon.h>

#include <lantiq_soc.h>

#define LTQ_XRX_RCU_RST_STAT		0x0014
#define LTQ_XRX_RCU_RST_STAT_WDT	BIT(31)

/* CPU0 Reset Source Register */
#define LTQ_FALCON_SYS1_CPU0RS		0x0060
/* reset cause mask */
#define LTQ_FALCON_SYS1_CPU0RS_MASK	0x0007
#define LTQ_FALCON_SYS1_CPU0RS_WDT	0x02

/*
 * Section 3.4 of the datasheet
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 * The password sequence protects the WDT control register from unintended
 * write actions, which might cause malfunction of the WDT.
 *
 * essentially the following two magic passwords need to be written to allow
 * IO access to the WDT core
 */
<<<<<<< HEAD
#define LTQ_WDT_PW1		0x00BE0000
#define LTQ_WDT_PW2		0x00DC0000

#define LTQ_WDT_CR		0x0	/* watchdog control register */
#define LTQ_WDT_SR		0x8	/* watchdog status register */

#define LTQ_WDT_SR_EN		(0x1 << 31)	/* enable bit */
#define LTQ_WDT_SR_PWD		(0x3 << 26)	/* turn on power */
#define LTQ_WDT_SR_CLKDIV	(0x3 << 24)	/* turn on clock and set */
						/* divider to 0x40000 */
#define LTQ_WDT_DIVIDER		0x40000
#define LTQ_MAX_TIMEOUT		((1 << 16) - 1)	/* the reload field is 16 bit */

static bool nowayout = WATCHDOG_NOWAYOUT;

static void __iomem *ltq_wdt_membase;
static unsigned long ltq_io_region_clk_rate;

static unsigned long ltq_wdt_bootstatus;
static unsigned long ltq_wdt_in_use;
static int ltq_wdt_timeout = 30;
static int ltq_wdt_ok_to_close;

static void
ltq_wdt_enable(void)
{
	unsigned long int timeout = ltq_wdt_timeout *
			(ltq_io_region_clk_rate / LTQ_WDT_DIVIDER) + 0x1000;
	if (timeout > LTQ_MAX_TIMEOUT)
		timeout = LTQ_MAX_TIMEOUT;

	/* write the first password magic */
	ltq_w32(LTQ_WDT_PW1, ltq_wdt_membase + LTQ_WDT_CR);
	/* write the second magic plus the configuration and new timeout */
	ltq_w32(LTQ_WDT_SR_EN | LTQ_WDT_SR_PWD | LTQ_WDT_SR_CLKDIV |
		LTQ_WDT_PW2 | timeout, ltq_wdt_membase + LTQ_WDT_CR);
}

static void
ltq_wdt_disable(void)
{
	/* write the first password magic */
	ltq_w32(LTQ_WDT_PW1, ltq_wdt_membase + LTQ_WDT_CR);
	/* write the second password magic with no config
	 * this turns the watchdog off
	 */
	ltq_w32(LTQ_WDT_PW2, ltq_wdt_membase + LTQ_WDT_CR);
}

static ssize_t
ltq_wdt_write(struct file *file, const char __user *data,
		size_t len, loff_t *ppos)
{
	if (len) {
		if (!nowayout) {
			size_t i;

			ltq_wdt_ok_to_close = 0;
			for (i = 0; i != len; i++) {
				char c;

				if (get_user(c, data + i))
					return -EFAULT;
				if (c == 'V')
					ltq_wdt_ok_to_close = 1;
				else
					ltq_wdt_ok_to_close = 0;
			}
		}
		ltq_wdt_enable();
	}

	return len;
}

static struct watchdog_info ident = {
	.options = WDIOF_MAGICCLOSE | WDIOF_SETTIMEOUT | WDIOF_KEEPALIVEPING |
			WDIOF_CARDRESET,
	.identity = "ltq_wdt",
};

static long
ltq_wdt_ioctl(struct file *file,
		unsigned int cmd, unsigned long arg)
{
	int ret = -ENOTTY;

	switch (cmd) {
	case WDIOC_GETSUPPORT:
		ret = copy_to_user((struct watchdog_info __user *)arg, &ident,
				sizeof(ident)) ? -EFAULT : 0;
		break;

	case WDIOC_GETBOOTSTATUS:
		ret = put_user(ltq_wdt_bootstatus, (int __user *)arg);
		break;

	case WDIOC_GETSTATUS:
		ret = put_user(0, (int __user *)arg);
		break;

	case WDIOC_SETTIMEOUT:
		ret = get_user(ltq_wdt_timeout, (int __user *)arg);
		if (!ret)
			ltq_wdt_enable();
		/* intentional drop through */
	case WDIOC_GETTIMEOUT:
		ret = put_user(ltq_wdt_timeout, (int __user *)arg);
		break;

	case WDIOC_KEEPALIVE:
		ltq_wdt_enable();
		ret = 0;
		break;
	}
	return ret;
}

static int
ltq_wdt_open(struct inode *inode, struct file *file)
{
	if (test_and_set_bit(0, &ltq_wdt_in_use))
		return -EBUSY;
	ltq_wdt_in_use = 1;
	ltq_wdt_enable();

	return nonseekable_open(inode, file);
}

static int
ltq_wdt_release(struct inode *inode, struct file *file)
{
	if (ltq_wdt_ok_to_close)
		ltq_wdt_disable();
	else
		pr_err("watchdog closed without warning\n");
	ltq_wdt_ok_to_close = 0;
	clear_bit(0, &ltq_wdt_in_use);
=======
#define LTQ_WDT_CR_PW1		0x00BE0000
#define LTQ_WDT_CR_PW2		0x00DC0000

#define LTQ_WDT_CR		0x0		/* watchdog control register */
#define  LTQ_WDT_CR_GEN		BIT(31)		/* enable bit */
/* Pre-warning limit set to 1/16 of max WDT period */
#define  LTQ_WDT_CR_PWL		(0x3 << 26)
/* set clock divider to 0x40000 */
#define  LTQ_WDT_CR_CLKDIV	(0x3 << 24)
#define  LTQ_WDT_CR_PW_MASK	GENMASK(23, 16)	/* Password field */
#define  LTQ_WDT_CR_MAX_TIMEOUT	((1 << 16) - 1)	/* The reload field is 16 bit */
#define LTQ_WDT_SR		0x8		/* watchdog status register */
#define  LTQ_WDT_SR_EN		BIT(31)		/* Enable */
#define  LTQ_WDT_SR_VALUE_MASK	GENMASK(15, 0)	/* Timer value */

#define LTQ_WDT_DIVIDER		0x40000

static bool nowayout = WATCHDOG_NOWAYOUT;

struct ltq_wdt_hw {
	int (*bootstatus_get)(struct device *dev);
};

struct ltq_wdt_priv {
	struct watchdog_device wdt;
	void __iomem *membase;
	unsigned long clk_rate;
};

static u32 ltq_wdt_r32(struct ltq_wdt_priv *priv, u32 offset)
{
	return __raw_readl(priv->membase + offset);
}

static void ltq_wdt_w32(struct ltq_wdt_priv *priv, u32 val, u32 offset)
{
	__raw_writel(val, priv->membase + offset);
}

static void ltq_wdt_mask(struct ltq_wdt_priv *priv, u32 clear, u32 set,
			 u32 offset)
{
	u32 val = ltq_wdt_r32(priv, offset);

	val &= ~(clear);
	val |= set;
	ltq_wdt_w32(priv, val, offset);
}

static struct ltq_wdt_priv *ltq_wdt_get_priv(struct watchdog_device *wdt)
{
	return container_of(wdt, struct ltq_wdt_priv, wdt);
}

static struct watchdog_info ltq_wdt_info = {
	.options = WDIOF_MAGICCLOSE | WDIOF_SETTIMEOUT | WDIOF_KEEPALIVEPING |
		   WDIOF_CARDRESET,
	.identity = "ltq_wdt",
};

static int ltq_wdt_start(struct watchdog_device *wdt)
{
	struct ltq_wdt_priv *priv = ltq_wdt_get_priv(wdt);
	u32 timeout;

	timeout = wdt->timeout * priv->clk_rate;

	ltq_wdt_mask(priv, LTQ_WDT_CR_PW_MASK, LTQ_WDT_CR_PW1, LTQ_WDT_CR);
	/* write the second magic plus the configuration and new timeout */
	ltq_wdt_mask(priv, LTQ_WDT_CR_PW_MASK | LTQ_WDT_CR_MAX_TIMEOUT,
		     LTQ_WDT_CR_GEN | LTQ_WDT_CR_PWL | LTQ_WDT_CR_CLKDIV |
		     LTQ_WDT_CR_PW2 | timeout,
		     LTQ_WDT_CR);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	return 0;
}

<<<<<<< HEAD
static const struct file_operations ltq_wdt_fops = {
	.owner		= THIS_MODULE,
	.write		= ltq_wdt_write,
	.unlocked_ioctl	= ltq_wdt_ioctl,
	.open		= ltq_wdt_open,
	.release	= ltq_wdt_release,
	.llseek		= no_llseek,
};

static struct miscdevice ltq_wdt_miscdev = {
	.minor	= WATCHDOG_MINOR,
	.name	= "watchdog",
	.fops	= &ltq_wdt_fops,
};

static int __init
ltq_wdt_probe(struct platform_device *pdev)
{
	struct resource *res = platform_get_resource(pdev, IORESOURCE_MEM, 0);
	struct clk *clk;

	if (!res) {
		dev_err(&pdev->dev, "cannot obtain I/O memory region");
		return -ENOENT;
	}
	res = devm_request_mem_region(&pdev->dev, res->start,
		resource_size(res), dev_name(&pdev->dev));
	if (!res) {
		dev_err(&pdev->dev, "cannot request I/O memory region");
		return -EBUSY;
	}
	ltq_wdt_membase = devm_ioremap_nocache(&pdev->dev, res->start,
		resource_size(res));
	if (!ltq_wdt_membase) {
		dev_err(&pdev->dev, "cannot remap I/O memory region\n");
		return -ENOMEM;
	}

	/* we do not need to enable the clock as it is always running */
	clk = clk_get(&pdev->dev, "io");
	WARN_ON(!clk);
	ltq_io_region_clk_rate = clk_get_rate(clk);
	clk_put(clk);

	if (ltq_reset_cause() == LTQ_RST_CAUSE_WDTRST)
		ltq_wdt_bootstatus = WDIOF_CARDRESET;

	return misc_register(&ltq_wdt_miscdev);
}

static int __devexit
ltq_wdt_remove(struct platform_device *pdev)
{
	misc_deregister(&ltq_wdt_miscdev);

	return 0;
}


static struct platform_driver ltq_wdt_driver = {
	.remove = __devexit_p(ltq_wdt_remove),
	.driver = {
		.name = "ltq_wdt",
		.owner = THIS_MODULE,
	},
};

static int __init
init_ltq_wdt(void)
{
	return platform_driver_probe(&ltq_wdt_driver, ltq_wdt_probe);
}

static void __exit
exit_ltq_wdt(void)
{
	return platform_driver_unregister(&ltq_wdt_driver);
}

module_init(init_ltq_wdt);
module_exit(exit_ltq_wdt);

module_param(nowayout, bool, 0);
MODULE_PARM_DESC(nowayout, "Watchdog cannot be stopped once started");

MODULE_AUTHOR("John Crispin <blogic@openwrt.org>");
MODULE_DESCRIPTION("Lantiq SoC Watchdog");
MODULE_LICENSE("GPL");
MODULE_ALIAS_MISCDEV(WATCHDOG_MINOR);
=======
static int ltq_wdt_stop(struct watchdog_device *wdt)
{
	struct ltq_wdt_priv *priv = ltq_wdt_get_priv(wdt);

	ltq_wdt_mask(priv, LTQ_WDT_CR_PW_MASK, LTQ_WDT_CR_PW1, LTQ_WDT_CR);
	ltq_wdt_mask(priv, LTQ_WDT_CR_GEN | LTQ_WDT_CR_PW_MASK,
		     LTQ_WDT_CR_PW2, LTQ_WDT_CR);

	return 0;
}

static int ltq_wdt_ping(struct watchdog_device *wdt)
{
	struct ltq_wdt_priv *priv = ltq_wdt_get_priv(wdt);
	u32 timeout;

	timeout = wdt->timeout * priv->clk_rate;

	ltq_wdt_mask(priv, LTQ_WDT_CR_PW_MASK, LTQ_WDT_CR_PW1, LTQ_WDT_CR);
	/* write the second magic plus the configuration and new timeout */
	ltq_wdt_mask(priv, LTQ_WDT_CR_PW_MASK | LTQ_WDT_CR_MAX_TIMEOUT,
		     LTQ_WDT_CR_PW2 | timeout, LTQ_WDT_CR);

	return 0;
}

static unsigned int ltq_wdt_get_timeleft(struct watchdog_device *wdt)
{
	struct ltq_wdt_priv *priv = ltq_wdt_get_priv(wdt);
	u64 timeout;

	timeout = ltq_wdt_r32(priv, LTQ_WDT_SR) & LTQ_WDT_SR_VALUE_MASK;
	return do_div(timeout, priv->clk_rate);
}

static const struct watchdog_ops ltq_wdt_ops = {
	.owner		= THIS_MODULE,
	.start		= ltq_wdt_start,
	.stop		= ltq_wdt_stop,
	.ping		= ltq_wdt_ping,
	.get_timeleft	= ltq_wdt_get_timeleft,
};

static int ltq_wdt_xrx_bootstatus_get(struct device *dev)
{
	struct regmap *rcu_regmap;
	u32 val;
	int err;

	rcu_regmap = syscon_regmap_lookup_by_phandle(dev->of_node, "regmap");
	if (IS_ERR(rcu_regmap))
		return PTR_ERR(rcu_regmap);

	err = regmap_read(rcu_regmap, LTQ_XRX_RCU_RST_STAT, &val);
	if (err)
		return err;

	if (val & LTQ_XRX_RCU_RST_STAT_WDT)
		return WDIOF_CARDRESET;

	return 0;
}

static int ltq_wdt_falcon_bootstatus_get(struct device *dev)
{
	struct regmap *rcu_regmap;
	u32 val;
	int err;

	rcu_regmap = syscon_regmap_lookup_by_phandle(dev->of_node,
						     "lantiq,rcu");
	if (IS_ERR(rcu_regmap))
		return PTR_ERR(rcu_regmap);

	err = regmap_read(rcu_regmap, LTQ_FALCON_SYS1_CPU0RS, &val);
	if (err)
		return err;

	if ((val & LTQ_FALCON_SYS1_CPU0RS_MASK) == LTQ_FALCON_SYS1_CPU0RS_WDT)
		return WDIOF_CARDRESET;

	return 0;
}

static int ltq_wdt_probe(struct platform_device *pdev)
{
	struct device *dev = &pdev->dev;
	struct ltq_wdt_priv *priv;
	struct watchdog_device *wdt;
	struct clk *clk;
	const struct ltq_wdt_hw *ltq_wdt_hw;
	int ret;
	u32 status;

	priv = devm_kzalloc(dev, sizeof(*priv), GFP_KERNEL);
	if (!priv)
		return -ENOMEM;

	priv->membase = devm_platform_ioremap_resource(pdev, 0);
	if (IS_ERR(priv->membase))
		return PTR_ERR(priv->membase);

	/* we do not need to enable the clock as it is always running */
	clk = clk_get_io();
	priv->clk_rate = clk_get_rate(clk) / LTQ_WDT_DIVIDER;
	if (!priv->clk_rate) {
		dev_err(dev, "clock rate less than divider %i\n",
			LTQ_WDT_DIVIDER);
		return -EINVAL;
	}

	wdt = &priv->wdt;
	wdt->info		= &ltq_wdt_info;
	wdt->ops		= &ltq_wdt_ops;
	wdt->min_timeout	= 1;
	wdt->max_timeout	= LTQ_WDT_CR_MAX_TIMEOUT / priv->clk_rate;
	wdt->timeout		= wdt->max_timeout;
	wdt->parent		= dev;

	ltq_wdt_hw = of_device_get_match_data(dev);
	if (ltq_wdt_hw && ltq_wdt_hw->bootstatus_get) {
		ret = ltq_wdt_hw->bootstatus_get(dev);
		if (ret >= 0)
			wdt->bootstatus = ret;
	}

	watchdog_set_nowayout(wdt, nowayout);
	watchdog_init_timeout(wdt, 0, dev);

	status = ltq_wdt_r32(priv, LTQ_WDT_SR);
	if (status & LTQ_WDT_SR_EN) {
		/*
		 * If the watchdog is already running overwrite it with our
		 * new settings. Stop is not needed as the start call will
		 * replace all settings anyway.
		 */
		ltq_wdt_start(wdt);
		set_bit(WDOG_HW_RUNNING, &wdt->status);
	}

	return devm_watchdog_register_device(dev, wdt);
}

static const struct ltq_wdt_hw ltq_wdt_xrx100 = {
	.bootstatus_get = ltq_wdt_xrx_bootstatus_get,
};

static const struct ltq_wdt_hw ltq_wdt_falcon = {
	.bootstatus_get = ltq_wdt_falcon_bootstatus_get,
};

static const struct of_device_id ltq_wdt_match[] = {
	{ .compatible = "lantiq,wdt", .data = NULL },
	{ .compatible = "lantiq,xrx100-wdt", .data = &ltq_wdt_xrx100 },
	{ .compatible = "lantiq,falcon-wdt", .data = &ltq_wdt_falcon },
	{},
};
MODULE_DEVICE_TABLE(of, ltq_wdt_match);

static struct platform_driver ltq_wdt_driver = {
	.probe = ltq_wdt_probe,
	.driver = {
		.name = "wdt",
		.of_match_table = ltq_wdt_match,
	},
};

module_platform_driver(ltq_wdt_driver);

module_param(nowayout, bool, 0);
MODULE_PARM_DESC(nowayout, "Watchdog cannot be stopped once started");
MODULE_AUTHOR("John Crispin <john@phrozen.org>");
MODULE_DESCRIPTION("Lantiq SoC Watchdog");
MODULE_LICENSE("GPL");
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
