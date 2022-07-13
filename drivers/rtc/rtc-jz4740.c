<<<<<<< HEAD
=======
// SPDX-License-Identifier: GPL-2.0+
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
/*
 *  Copyright (C) 2009-2010, Lars-Peter Clausen <lars@metafoo.de>
 *  Copyright (C) 2010, Paul Cercueil <paul@crapouillou.net>
 *	 JZ4740 SoC RTC driver
<<<<<<< HEAD
 *
 *  This program is free software; you can redistribute it and/or modify it
 *  under  the terms of  the GNU General Public License as published by the
 *  Free Software Foundation;  either version 2 of the License, or (at your
 *  option) any later version.
 *
 *  You should have received a copy of the GNU General Public License along
 *  with this program; if not, write to the Free Software Foundation, Inc.,
 *  675 Mass Ave, Cambridge, MA 02139, USA.
 *
 */

#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/platform_device.h>
=======
 */

#include <linux/clk.h>
#include <linux/clk-provider.h>
#include <linux/io.h>
#include <linux/iopoll.h>
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/of.h>
#include <linux/platform_device.h>
#include <linux/pm_wakeirq.h>
#include <linux/property.h>
#include <linux/reboot.h>
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#include <linux/rtc.h>
#include <linux/slab.h>
#include <linux/spinlock.h>

#define JZ_REG_RTC_CTRL		0x00
#define JZ_REG_RTC_SEC		0x04
#define JZ_REG_RTC_SEC_ALARM	0x08
#define JZ_REG_RTC_REGULATOR	0x0C
#define JZ_REG_RTC_HIBERNATE	0x20
<<<<<<< HEAD
#define JZ_REG_RTC_SCRATCHPAD	0x34
=======
#define JZ_REG_RTC_WAKEUP_FILTER	0x24
#define JZ_REG_RTC_RESET_COUNTER	0x28
#define JZ_REG_RTC_SCRATCHPAD	0x34
#define JZ_REG_RTC_CKPCR	0x40

/* The following are present on the jz4780 */
#define JZ_REG_RTC_WENR	0x3C
#define JZ_RTC_WENR_WEN	BIT(31)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

#define JZ_RTC_CTRL_WRDY	BIT(7)
#define JZ_RTC_CTRL_1HZ		BIT(6)
#define JZ_RTC_CTRL_1HZ_IRQ	BIT(5)
#define JZ_RTC_CTRL_AF		BIT(4)
#define JZ_RTC_CTRL_AF_IRQ	BIT(3)
#define JZ_RTC_CTRL_AE		BIT(2)
#define JZ_RTC_CTRL_ENABLE	BIT(0)

<<<<<<< HEAD
struct jz4740_rtc {
	struct resource *mem;
	void __iomem *base;

	struct rtc_device *rtc;

	unsigned int irq;
=======
/* Magic value to enable writes on jz4780 */
#define JZ_RTC_WENR_MAGIC	0xA55A

#define JZ_RTC_WAKEUP_FILTER_MASK	0x0000FFE0
#define JZ_RTC_RESET_COUNTER_MASK	0x00000FE0

#define JZ_RTC_CKPCR_CK32PULL_DIS	BIT(4)
#define JZ_RTC_CKPCR_CK32CTL_EN		(BIT(2) | BIT(1))

enum jz4740_rtc_type {
	ID_JZ4740,
	ID_JZ4760,
	ID_JZ4780,
};

struct jz4740_rtc {
	void __iomem *base;
	enum jz4740_rtc_type type;

	struct rtc_device *rtc;

	struct clk_hw clk32k;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	spinlock_t lock;
};

<<<<<<< HEAD
=======
static struct device *dev_for_power_off;

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
static inline uint32_t jz4740_rtc_reg_read(struct jz4740_rtc *rtc, size_t reg)
{
	return readl(rtc->base + reg);
}

static int jz4740_rtc_wait_write_ready(struct jz4740_rtc *rtc)
{
	uint32_t ctrl;
<<<<<<< HEAD
	int timeout = 1000;

	do {
		ctrl = jz4740_rtc_reg_read(rtc, JZ_REG_RTC_CTRL);
	} while (!(ctrl & JZ_RTC_CTRL_WRDY) && --timeout);

	return timeout ? 0 : -EIO;
=======

	return readl_poll_timeout(rtc->base + JZ_REG_RTC_CTRL, ctrl,
				  ctrl & JZ_RTC_CTRL_WRDY, 0, 1000);
}

static inline int jz4780_rtc_enable_write(struct jz4740_rtc *rtc)
{
	uint32_t ctrl;
	int ret;

	ret = jz4740_rtc_wait_write_ready(rtc);
	if (ret != 0)
		return ret;

	writel(JZ_RTC_WENR_MAGIC, rtc->base + JZ_REG_RTC_WENR);

	return readl_poll_timeout(rtc->base + JZ_REG_RTC_WENR, ctrl,
				  ctrl & JZ_RTC_WENR_WEN, 0, 1000);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

static inline int jz4740_rtc_reg_write(struct jz4740_rtc *rtc, size_t reg,
	uint32_t val)
{
<<<<<<< HEAD
	int ret;
	ret = jz4740_rtc_wait_write_ready(rtc);
=======
	int ret = 0;

	if (rtc->type >= ID_JZ4760)
		ret = jz4780_rtc_enable_write(rtc);
	if (ret == 0)
		ret = jz4740_rtc_wait_write_ready(rtc);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	if (ret == 0)
		writel(val, rtc->base + reg);

	return ret;
}

static int jz4740_rtc_ctrl_set_bits(struct jz4740_rtc *rtc, uint32_t mask,
	bool set)
{
	int ret;
	unsigned long flags;
	uint32_t ctrl;

	spin_lock_irqsave(&rtc->lock, flags);

	ctrl = jz4740_rtc_reg_read(rtc, JZ_REG_RTC_CTRL);

	/* Don't clear interrupt flags by accident */
	ctrl |= JZ_RTC_CTRL_1HZ | JZ_RTC_CTRL_AF;

	if (set)
		ctrl |= mask;
	else
		ctrl &= ~mask;

	ret = jz4740_rtc_reg_write(rtc, JZ_REG_RTC_CTRL, ctrl);

	spin_unlock_irqrestore(&rtc->lock, flags);

	return ret;
}

static int jz4740_rtc_read_time(struct device *dev, struct rtc_time *time)
{
	struct jz4740_rtc *rtc = dev_get_drvdata(dev);
	uint32_t secs, secs2;
	int timeout = 5;

<<<<<<< HEAD
=======
	if (jz4740_rtc_reg_read(rtc, JZ_REG_RTC_SCRATCHPAD) != 0x12345678)
		return -EINVAL;

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	/* If the seconds register is read while it is updated, it can contain a
	 * bogus value. This can be avoided by making sure that two consecutive
	 * reads have the same value.
	 */
	secs = jz4740_rtc_reg_read(rtc, JZ_REG_RTC_SEC);
	secs2 = jz4740_rtc_reg_read(rtc, JZ_REG_RTC_SEC);

	while (secs != secs2 && --timeout) {
		secs = secs2;
		secs2 = jz4740_rtc_reg_read(rtc, JZ_REG_RTC_SEC);
	}

	if (timeout == 0)
		return -EIO;

<<<<<<< HEAD
	rtc_time_to_tm(secs, time);

	return rtc_valid_tm(time);
}

static int jz4740_rtc_set_mmss(struct device *dev, unsigned long secs)
{
	struct jz4740_rtc *rtc = dev_get_drvdata(dev);

	return jz4740_rtc_reg_write(rtc, JZ_REG_RTC_SEC, secs);
=======
	rtc_time64_to_tm(secs, time);

	return 0;
}

static int jz4740_rtc_set_time(struct device *dev, struct rtc_time *time)
{
	struct jz4740_rtc *rtc = dev_get_drvdata(dev);
	int ret;

	ret = jz4740_rtc_reg_write(rtc, JZ_REG_RTC_SEC, rtc_tm_to_time64(time));
	if (ret)
		return ret;

	return jz4740_rtc_reg_write(rtc, JZ_REG_RTC_SCRATCHPAD, 0x12345678);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

static int jz4740_rtc_read_alarm(struct device *dev, struct rtc_wkalrm *alrm)
{
	struct jz4740_rtc *rtc = dev_get_drvdata(dev);
	uint32_t secs;
	uint32_t ctrl;

	secs = jz4740_rtc_reg_read(rtc, JZ_REG_RTC_SEC_ALARM);

	ctrl = jz4740_rtc_reg_read(rtc, JZ_REG_RTC_CTRL);

	alrm->enabled = !!(ctrl & JZ_RTC_CTRL_AE);
	alrm->pending = !!(ctrl & JZ_RTC_CTRL_AF);

<<<<<<< HEAD
	rtc_time_to_tm(secs, &alrm->time);

	return rtc_valid_tm(&alrm->time);
=======
	rtc_time64_to_tm(secs, &alrm->time);

	return 0;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

static int jz4740_rtc_set_alarm(struct device *dev, struct rtc_wkalrm *alrm)
{
	int ret;
	struct jz4740_rtc *rtc = dev_get_drvdata(dev);
<<<<<<< HEAD
	unsigned long secs;

	rtc_tm_to_time(&alrm->time, &secs);
=======
	uint32_t secs = lower_32_bits(rtc_tm_to_time64(&alrm->time));
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	ret = jz4740_rtc_reg_write(rtc, JZ_REG_RTC_SEC_ALARM, secs);
	if (!ret)
		ret = jz4740_rtc_ctrl_set_bits(rtc,
			JZ_RTC_CTRL_AE | JZ_RTC_CTRL_AF_IRQ, alrm->enabled);

	return ret;
}

static int jz4740_rtc_alarm_irq_enable(struct device *dev, unsigned int enable)
{
	struct jz4740_rtc *rtc = dev_get_drvdata(dev);
	return jz4740_rtc_ctrl_set_bits(rtc, JZ_RTC_CTRL_AF_IRQ, enable);
}

<<<<<<< HEAD
static struct rtc_class_ops jz4740_rtc_ops = {
	.read_time	= jz4740_rtc_read_time,
	.set_mmss	= jz4740_rtc_set_mmss,
=======
static const struct rtc_class_ops jz4740_rtc_ops = {
	.read_time	= jz4740_rtc_read_time,
	.set_time	= jz4740_rtc_set_time,
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	.read_alarm	= jz4740_rtc_read_alarm,
	.set_alarm	= jz4740_rtc_set_alarm,
	.alarm_irq_enable = jz4740_rtc_alarm_irq_enable,
};

static irqreturn_t jz4740_rtc_irq(int irq, void *data)
{
	struct jz4740_rtc *rtc = data;
	uint32_t ctrl;
	unsigned long events = 0;

	ctrl = jz4740_rtc_reg_read(rtc, JZ_REG_RTC_CTRL);

	if (ctrl & JZ_RTC_CTRL_1HZ)
		events |= (RTC_UF | RTC_IRQF);

	if (ctrl & JZ_RTC_CTRL_AF)
		events |= (RTC_AF | RTC_IRQF);

	rtc_update_irq(rtc->rtc, 1, events);

	jz4740_rtc_ctrl_set_bits(rtc, JZ_RTC_CTRL_1HZ | JZ_RTC_CTRL_AF, false);

	return IRQ_HANDLED;
}

<<<<<<< HEAD
void jz4740_rtc_poweroff(struct device *dev)
=======
static void jz4740_rtc_poweroff(struct device *dev)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
{
	struct jz4740_rtc *rtc = dev_get_drvdata(dev);
	jz4740_rtc_reg_write(rtc, JZ_REG_RTC_HIBERNATE, 1);
}
<<<<<<< HEAD
EXPORT_SYMBOL_GPL(jz4740_rtc_poweroff);

static int __devinit jz4740_rtc_probe(struct platform_device *pdev)
{
	int ret;
	struct jz4740_rtc *rtc;
	uint32_t scratchpad;

	rtc = kzalloc(sizeof(*rtc), GFP_KERNEL);
	if (!rtc)
		return -ENOMEM;

	rtc->irq = platform_get_irq(pdev, 0);
	if (rtc->irq < 0) {
		ret = -ENOENT;
		dev_err(&pdev->dev, "Failed to get platform irq\n");
		goto err_free;
	}

	rtc->mem = platform_get_resource(pdev, IORESOURCE_MEM, 0);
	if (!rtc->mem) {
		ret = -ENOENT;
		dev_err(&pdev->dev, "Failed to get platform mmio memory\n");
		goto err_free;
	}

	rtc->mem = request_mem_region(rtc->mem->start, resource_size(rtc->mem),
					pdev->name);
	if (!rtc->mem) {
		ret = -EBUSY;
		dev_err(&pdev->dev, "Failed to request mmio memory region\n");
		goto err_free;
	}

	rtc->base = ioremap_nocache(rtc->mem->start, resource_size(rtc->mem));
	if (!rtc->base) {
		ret = -EBUSY;
		dev_err(&pdev->dev, "Failed to ioremap mmio memory\n");
		goto err_release_mem_region;
	}
=======

static void jz4740_rtc_power_off(void)
{
	jz4740_rtc_poweroff(dev_for_power_off);
	kernel_halt();
}

static const struct of_device_id jz4740_rtc_of_match[] = {
	{ .compatible = "ingenic,jz4740-rtc", .data = (void *)ID_JZ4740 },
	{ .compatible = "ingenic,jz4760-rtc", .data = (void *)ID_JZ4760 },
	{ .compatible = "ingenic,jz4770-rtc", .data = (void *)ID_JZ4780 },
	{ .compatible = "ingenic,jz4780-rtc", .data = (void *)ID_JZ4780 },
	{},
};
MODULE_DEVICE_TABLE(of, jz4740_rtc_of_match);

static void jz4740_rtc_set_wakeup_params(struct jz4740_rtc *rtc,
					 struct device_node *np,
					 unsigned long rate)
{
	unsigned long wakeup_ticks, reset_ticks;
	unsigned int min_wakeup_pin_assert_time = 60; /* Default: 60ms */
	unsigned int reset_pin_assert_time = 100; /* Default: 100ms */

	of_property_read_u32(np, "ingenic,reset-pin-assert-time-ms",
			     &reset_pin_assert_time);
	of_property_read_u32(np, "ingenic,min-wakeup-pin-assert-time-ms",
			     &min_wakeup_pin_assert_time);

	/*
	 * Set minimum wakeup pin assertion time: 100 ms.
	 * Range is 0 to 2 sec if RTC is clocked at 32 kHz.
	 */
	wakeup_ticks = (min_wakeup_pin_assert_time * rate) / 1000;
	if (wakeup_ticks < JZ_RTC_WAKEUP_FILTER_MASK)
		wakeup_ticks &= JZ_RTC_WAKEUP_FILTER_MASK;
	else
		wakeup_ticks = JZ_RTC_WAKEUP_FILTER_MASK;
	jz4740_rtc_reg_write(rtc, JZ_REG_RTC_WAKEUP_FILTER, wakeup_ticks);

	/*
	 * Set reset pin low-level assertion time after wakeup: 60 ms.
	 * Range is 0 to 125 ms if RTC is clocked at 32 kHz.
	 */
	reset_ticks = (reset_pin_assert_time * rate) / 1000;
	if (reset_ticks < JZ_RTC_RESET_COUNTER_MASK)
		reset_ticks &= JZ_RTC_RESET_COUNTER_MASK;
	else
		reset_ticks = JZ_RTC_RESET_COUNTER_MASK;
	jz4740_rtc_reg_write(rtc, JZ_REG_RTC_RESET_COUNTER, reset_ticks);
}

static int jz4740_rtc_clk32k_enable(struct clk_hw *hw)
{
	struct jz4740_rtc *rtc = container_of(hw, struct jz4740_rtc, clk32k);

	return jz4740_rtc_reg_write(rtc, JZ_REG_RTC_CKPCR,
				    JZ_RTC_CKPCR_CK32PULL_DIS |
				    JZ_RTC_CKPCR_CK32CTL_EN);
}

static void jz4740_rtc_clk32k_disable(struct clk_hw *hw)
{
	struct jz4740_rtc *rtc = container_of(hw, struct jz4740_rtc, clk32k);

	jz4740_rtc_reg_write(rtc, JZ_REG_RTC_CKPCR, 0);
}

static int jz4740_rtc_clk32k_is_enabled(struct clk_hw *hw)
{
	struct jz4740_rtc *rtc = container_of(hw, struct jz4740_rtc, clk32k);
	u32 ckpcr;

	ckpcr = jz4740_rtc_reg_read(rtc, JZ_REG_RTC_CKPCR);

	return !!(ckpcr & JZ_RTC_CKPCR_CK32CTL_EN);
}

static const struct clk_ops jz4740_rtc_clk32k_ops = {
	.enable = jz4740_rtc_clk32k_enable,
	.disable = jz4740_rtc_clk32k_disable,
	.is_enabled = jz4740_rtc_clk32k_is_enabled,
};

static int jz4740_rtc_probe(struct platform_device *pdev)
{
	struct device *dev = &pdev->dev;
	struct device_node *np = dev->of_node;
	struct jz4740_rtc *rtc;
	unsigned long rate;
	struct clk *clk;
	int ret, irq;

	rtc = devm_kzalloc(dev, sizeof(*rtc), GFP_KERNEL);
	if (!rtc)
		return -ENOMEM;

	rtc->type = (uintptr_t)device_get_match_data(dev);

	irq = platform_get_irq(pdev, 0);
	if (irq < 0)
		return irq;

	rtc->base = devm_platform_ioremap_resource(pdev, 0);
	if (IS_ERR(rtc->base))
		return PTR_ERR(rtc->base);

	clk = devm_clk_get_enabled(dev, "rtc");
	if (IS_ERR(clk))
		return dev_err_probe(dev, PTR_ERR(clk), "Failed to get RTC clock\n");
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	spin_lock_init(&rtc->lock);

	platform_set_drvdata(pdev, rtc);

<<<<<<< HEAD
	device_init_wakeup(&pdev->dev, 1);

	rtc->rtc = rtc_device_register(pdev->name, &pdev->dev, &jz4740_rtc_ops,
					THIS_MODULE);
	if (IS_ERR(rtc->rtc)) {
		ret = PTR_ERR(rtc->rtc);
		dev_err(&pdev->dev, "Failed to register rtc device: %d\n", ret);
		goto err_iounmap;
	}

	ret = request_irq(rtc->irq, jz4740_rtc_irq, 0,
				pdev->name, rtc);
	if (ret) {
		dev_err(&pdev->dev, "Failed to request rtc irq: %d\n", ret);
		goto err_unregister_rtc;
	}

	scratchpad = jz4740_rtc_reg_read(rtc, JZ_REG_RTC_SCRATCHPAD);
	if (scratchpad != 0x12345678) {
		ret = jz4740_rtc_reg_write(rtc, JZ_REG_RTC_SCRATCHPAD, 0x12345678);
		ret = jz4740_rtc_reg_write(rtc, JZ_REG_RTC_SEC, 0);
		if (ret) {
			dev_err(&pdev->dev, "Could not write write to RTC registers\n");
			goto err_free_irq;
		}
	}

	return 0;

err_free_irq:
	free_irq(rtc->irq, rtc);
err_unregister_rtc:
	rtc_device_unregister(rtc->rtc);
err_iounmap:
	platform_set_drvdata(pdev, NULL);
	iounmap(rtc->base);
err_release_mem_region:
	release_mem_region(rtc->mem->start, resource_size(rtc->mem));
err_free:
	kfree(rtc);

	return ret;
}

static int __devexit jz4740_rtc_remove(struct platform_device *pdev)
{
	struct jz4740_rtc *rtc = platform_get_drvdata(pdev);

	free_irq(rtc->irq, rtc);

	rtc_device_unregister(rtc->rtc);

	iounmap(rtc->base);
	release_mem_region(rtc->mem->start, resource_size(rtc->mem));

	kfree(rtc);

	platform_set_drvdata(pdev, NULL);

	return 0;
}


#ifdef CONFIG_PM
static int jz4740_rtc_suspend(struct device *dev)
{
	struct jz4740_rtc *rtc = dev_get_drvdata(dev);

	if (device_may_wakeup(dev))
		enable_irq_wake(rtc->irq);
	return 0;
}

static int jz4740_rtc_resume(struct device *dev)
{
	struct jz4740_rtc *rtc = dev_get_drvdata(dev);

	if (device_may_wakeup(dev))
		disable_irq_wake(rtc->irq);
	return 0;
}

static const struct dev_pm_ops jz4740_pm_ops = {
	.suspend = jz4740_rtc_suspend,
	.resume  = jz4740_rtc_resume,
};
#define JZ4740_RTC_PM_OPS (&jz4740_pm_ops)

#else
#define JZ4740_RTC_PM_OPS NULL
#endif  /* CONFIG_PM */

static struct platform_driver jz4740_rtc_driver = {
	.probe	 = jz4740_rtc_probe,
	.remove	 = __devexit_p(jz4740_rtc_remove),
	.driver	 = {
		.name  = "jz4740-rtc",
		.owner = THIS_MODULE,
		.pm    = JZ4740_RTC_PM_OPS,
=======
	device_init_wakeup(dev, 1);

	ret = dev_pm_set_wake_irq(dev, irq);
	if (ret)
		return dev_err_probe(dev, ret, "Failed to set wake irq\n");

	rtc->rtc = devm_rtc_allocate_device(dev);
	if (IS_ERR(rtc->rtc))
		return dev_err_probe(dev, PTR_ERR(rtc->rtc),
				     "Failed to allocate rtc device\n");

	rtc->rtc->ops = &jz4740_rtc_ops;
	rtc->rtc->range_max = U32_MAX;

	rate = clk_get_rate(clk);
	jz4740_rtc_set_wakeup_params(rtc, np, rate);

	/* Each 1 Hz pulse should happen after (rate) ticks */
	jz4740_rtc_reg_write(rtc, JZ_REG_RTC_REGULATOR, rate - 1);

	ret = devm_rtc_register_device(rtc->rtc);
	if (ret)
		return ret;

	ret = devm_request_irq(dev, irq, jz4740_rtc_irq, 0,
			       pdev->name, rtc);
	if (ret)
		return dev_err_probe(dev, ret, "Failed to request rtc irq\n");

	if (of_device_is_system_power_controller(np)) {
		dev_for_power_off = dev;

		if (!pm_power_off)
			pm_power_off = jz4740_rtc_power_off;
		else
			dev_warn(dev, "Poweroff handler already present!\n");
	}

	if (device_property_present(dev, "#clock-cells")) {
		rtc->clk32k.init = CLK_HW_INIT_HW("clk32k", __clk_get_hw(clk),
						  &jz4740_rtc_clk32k_ops, 0);

		ret = devm_clk_hw_register(dev, &rtc->clk32k);
		if (ret)
			return dev_err_probe(dev, ret,
					     "Unable to register clk32k clock\n");

		ret = devm_of_clk_add_hw_provider(dev, of_clk_hw_simple_get,
						  &rtc->clk32k);
		if (ret)
			return dev_err_probe(dev, ret,
					     "Unable to register clk32k clock provider\n");
	}

	return 0;
}

static struct platform_driver jz4740_rtc_driver = {
	.probe	 = jz4740_rtc_probe,
	.driver	 = {
		.name  = "jz4740-rtc",
		.of_match_table = jz4740_rtc_of_match,
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	},
};

module_platform_driver(jz4740_rtc_driver);

MODULE_AUTHOR("Lars-Peter Clausen <lars@metafoo.de>");
MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("RTC driver for the JZ4740 SoC\n");
MODULE_ALIAS("platform:jz4740-rtc");
