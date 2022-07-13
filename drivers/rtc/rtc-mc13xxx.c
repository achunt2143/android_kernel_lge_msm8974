<<<<<<< HEAD
=======
// SPDX-License-Identifier: GPL-2.0
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
/*
 * Real Time Clock driver for Freescale MC13XXX PMIC
 *
 * (C) 2009 Sascha Hauer, Pengutronix
 * (C) 2009 Uwe Kleine-Koenig, Pengutronix
<<<<<<< HEAD
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 */

#include <linux/mfd/mc13xxx.h>
#include <linux/platform_device.h>
#include <linux/kernel.h>
#include <linux/module.h>
<<<<<<< HEAD
=======
#include <linux/mod_devicetable.h>
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#include <linux/slab.h>
#include <linux/rtc.h>

#define DRIVER_NAME "mc13xxx-rtc"

#define MC13XXX_RTCTOD	20
#define MC13XXX_RTCTODA	21
#define MC13XXX_RTCDAY	22
#define MC13XXX_RTCDAYA	23

<<<<<<< HEAD
=======
#define SEC_PER_DAY	(24 * 60 * 60)

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
struct mc13xxx_rtc {
	struct rtc_device *rtc;
	struct mc13xxx *mc13xxx;
	int valid;
};

static int mc13xxx_rtc_irq_enable_unlocked(struct device *dev,
		unsigned int enabled, int irq)
{
	struct mc13xxx_rtc *priv = dev_get_drvdata(dev);
	int (*func)(struct mc13xxx *mc13xxx, int irq);

	if (!priv->valid)
		return -ENODATA;

	func = enabled ? mc13xxx_irq_unmask : mc13xxx_irq_mask;
	return func(priv->mc13xxx, irq);
}

<<<<<<< HEAD
static int mc13xxx_rtc_irq_enable(struct device *dev,
		unsigned int enabled, int irq)
=======
static int mc13xxx_rtc_alarm_irq_enable(struct device *dev,
					unsigned int enabled)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
{
	struct mc13xxx_rtc *priv = dev_get_drvdata(dev);
	int ret;

	mc13xxx_lock(priv->mc13xxx);

<<<<<<< HEAD
	ret = mc13xxx_rtc_irq_enable_unlocked(dev, enabled, irq);
=======
	ret = mc13xxx_rtc_irq_enable_unlocked(dev, enabled, MC13XXX_IRQ_TODA);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	mc13xxx_unlock(priv->mc13xxx);

	return ret;
}

static int mc13xxx_rtc_read_time(struct device *dev, struct rtc_time *tm)
{
	struct mc13xxx_rtc *priv = dev_get_drvdata(dev);
	unsigned int seconds, days1, days2;
<<<<<<< HEAD
	unsigned long s1970;
	int ret;

	mc13xxx_lock(priv->mc13xxx);

	if (!priv->valid) {
		ret = -ENODATA;
		goto out;
	}

	ret = mc13xxx_reg_read(priv->mc13xxx, MC13XXX_RTCDAY, &days1);
	if (unlikely(ret))
		goto out;

	ret = mc13xxx_reg_read(priv->mc13xxx, MC13XXX_RTCTOD, &seconds);
	if (unlikely(ret))
		goto out;

	ret = mc13xxx_reg_read(priv->mc13xxx, MC13XXX_RTCDAY, &days2);
out:
	mc13xxx_unlock(priv->mc13xxx);

	if (ret)
		return ret;

	if (days2 == days1 + 1) {
		if (seconds >= 86400 / 2)
			days2 = days1;
		else
			days1 = days2;
	}

	if (days1 != days2)
		return -EIO;

	s1970 = days1 * 86400 + seconds;

	rtc_time_to_tm(s1970, tm);

	return rtc_valid_tm(tm);
}

static int mc13xxx_rtc_set_mmss(struct device *dev, unsigned long secs)
=======

	if (!priv->valid)
		return -ENODATA;

	do {
		int ret;

		ret = mc13xxx_reg_read(priv->mc13xxx, MC13XXX_RTCDAY, &days1);
		if (ret)
			return ret;

		ret = mc13xxx_reg_read(priv->mc13xxx, MC13XXX_RTCTOD, &seconds);
		if (ret)
			return ret;

		ret = mc13xxx_reg_read(priv->mc13xxx, MC13XXX_RTCDAY, &days2);
		if (ret)
			return ret;
	} while (days1 != days2);

	rtc_time64_to_tm((time64_t)days1 * SEC_PER_DAY + seconds, tm);

	return 0;
}

static int mc13xxx_rtc_set_time(struct device *dev, struct rtc_time *tm)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
{
	struct mc13xxx_rtc *priv = dev_get_drvdata(dev);
	unsigned int seconds, days;
	unsigned int alarmseconds;
	int ret;

<<<<<<< HEAD
	seconds = secs % 86400;
	days = secs / 86400;
=======
	days = div_s64_rem(rtc_tm_to_time64(tm), SEC_PER_DAY, &seconds);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	mc13xxx_lock(priv->mc13xxx);

	/*
	 * temporarily invalidate alarm to prevent triggering it when the day is
	 * already updated while the time isn't yet.
	 */
	ret = mc13xxx_reg_read(priv->mc13xxx, MC13XXX_RTCTODA, &alarmseconds);
	if (unlikely(ret))
		goto out;

<<<<<<< HEAD
	if (alarmseconds < 86400) {
=======
	if (alarmseconds < SEC_PER_DAY) {
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		ret = mc13xxx_reg_write(priv->mc13xxx,
				MC13XXX_RTCTODA, 0x1ffff);
		if (unlikely(ret))
			goto out;
	}

	/*
	 * write seconds=0 to prevent a day switch between writing days
	 * and seconds below
	 */
	ret = mc13xxx_reg_write(priv->mc13xxx, MC13XXX_RTCTOD, 0);
	if (unlikely(ret))
		goto out;

	ret = mc13xxx_reg_write(priv->mc13xxx, MC13XXX_RTCDAY, days);
	if (unlikely(ret))
		goto out;

	ret = mc13xxx_reg_write(priv->mc13xxx, MC13XXX_RTCTOD, seconds);
	if (unlikely(ret))
		goto out;

	/* restore alarm */
<<<<<<< HEAD
	if (alarmseconds < 86400) {
=======
	if (alarmseconds < SEC_PER_DAY) {
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		ret = mc13xxx_reg_write(priv->mc13xxx,
				MC13XXX_RTCTODA, alarmseconds);
		if (unlikely(ret))
			goto out;
	}

<<<<<<< HEAD
	ret = mc13xxx_irq_ack(priv->mc13xxx, MC13XXX_IRQ_RTCRST);
	if (unlikely(ret))
		goto out;

	ret = mc13xxx_irq_unmask(priv->mc13xxx, MC13XXX_IRQ_RTCRST);
=======
	if (!priv->valid) {
		ret = mc13xxx_irq_ack(priv->mc13xxx, MC13XXX_IRQ_RTCRST);
		if (unlikely(ret))
			goto out;

		ret = mc13xxx_irq_unmask(priv->mc13xxx, MC13XXX_IRQ_RTCRST);
	}

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
out:
	priv->valid = !ret;

	mc13xxx_unlock(priv->mc13xxx);

	return ret;
}

static int mc13xxx_rtc_read_alarm(struct device *dev, struct rtc_wkalrm *alarm)
{
	struct mc13xxx_rtc *priv = dev_get_drvdata(dev);
<<<<<<< HEAD
	unsigned seconds, days;
	unsigned long s1970;
=======
	unsigned int seconds, days;
	time64_t s1970;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	int enabled, pending;
	int ret;

	mc13xxx_lock(priv->mc13xxx);

	ret = mc13xxx_reg_read(priv->mc13xxx, MC13XXX_RTCTODA, &seconds);
	if (unlikely(ret))
		goto out;
<<<<<<< HEAD
	if (seconds >= 86400) {
=======
	if (seconds >= SEC_PER_DAY) {
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		ret = -ENODATA;
		goto out;
	}

	ret = mc13xxx_reg_read(priv->mc13xxx, MC13XXX_RTCDAY, &days);
	if (unlikely(ret))
		goto out;

	ret = mc13xxx_irq_status(priv->mc13xxx, MC13XXX_IRQ_TODA,
			&enabled, &pending);

out:
	mc13xxx_unlock(priv->mc13xxx);

	if (ret)
		return ret;

	alarm->enabled = enabled;
	alarm->pending = pending;

<<<<<<< HEAD
	s1970 = days * 86400 + seconds;

	rtc_time_to_tm(s1970, &alarm->time);
	dev_dbg(dev, "%s: %lu\n", __func__, s1970);
=======
	s1970 = (time64_t)days * SEC_PER_DAY + seconds;

	rtc_time64_to_tm(s1970, &alarm->time);
	dev_dbg(dev, "%s: %lld\n", __func__, (long long)s1970);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	return 0;
}

static int mc13xxx_rtc_set_alarm(struct device *dev, struct rtc_wkalrm *alarm)
{
	struct mc13xxx_rtc *priv = dev_get_drvdata(dev);
<<<<<<< HEAD
	unsigned long s1970;
	unsigned seconds, days;
=======
	time64_t s1970;
	u32 seconds, days;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	int ret;

	mc13xxx_lock(priv->mc13xxx);

	/* disable alarm to prevent false triggering */
	ret = mc13xxx_reg_write(priv->mc13xxx, MC13XXX_RTCTODA, 0x1ffff);
	if (unlikely(ret))
		goto out;

	ret = mc13xxx_irq_ack(priv->mc13xxx, MC13XXX_IRQ_TODA);
	if (unlikely(ret))
		goto out;

<<<<<<< HEAD
	ret = rtc_tm_to_time(&alarm->time, &s1970);
	if (unlikely(ret))
		goto out;

	dev_dbg(dev, "%s: o%2.s %lu\n", __func__, alarm->enabled ? "n" : "ff",
			s1970);
=======
	s1970 = rtc_tm_to_time64(&alarm->time);

	dev_dbg(dev, "%s: %s %lld\n", __func__, alarm->enabled ? "on" : "off",
			(long long)s1970);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	ret = mc13xxx_rtc_irq_enable_unlocked(dev, alarm->enabled,
			MC13XXX_IRQ_TODA);
	if (unlikely(ret))
		goto out;

<<<<<<< HEAD
	seconds = s1970 % 86400;
	days = s1970 / 86400;
=======
	days = div_s64_rem(s1970, SEC_PER_DAY, &seconds);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	ret = mc13xxx_reg_write(priv->mc13xxx, MC13XXX_RTCDAYA, days);
	if (unlikely(ret))
		goto out;

	ret = mc13xxx_reg_write(priv->mc13xxx, MC13XXX_RTCTODA, seconds);

out:
	mc13xxx_unlock(priv->mc13xxx);

	return ret;
}

static irqreturn_t mc13xxx_rtc_alarm_handler(int irq, void *dev)
{
	struct mc13xxx_rtc *priv = dev;
	struct mc13xxx *mc13xxx = priv->mc13xxx;

<<<<<<< HEAD
	dev_dbg(&priv->rtc->dev, "Alarm\n");

=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	rtc_update_irq(priv->rtc, 1, RTC_IRQF | RTC_AF);

	mc13xxx_irq_ack(mc13xxx, irq);

	return IRQ_HANDLED;
}

<<<<<<< HEAD
static irqreturn_t mc13xxx_rtc_update_handler(int irq, void *dev)
{
	struct mc13xxx_rtc *priv = dev;
	struct mc13xxx *mc13xxx = priv->mc13xxx;

	dev_dbg(&priv->rtc->dev, "1HZ\n");

	rtc_update_irq(priv->rtc, 1, RTC_IRQF | RTC_UF);

	mc13xxx_irq_ack(mc13xxx, irq);

	return IRQ_HANDLED;
}

static int mc13xxx_rtc_alarm_irq_enable(struct device *dev,
		unsigned int enabled)
{
	return mc13xxx_rtc_irq_enable(dev, enabled, MC13XXX_IRQ_TODA);
}

static const struct rtc_class_ops mc13xxx_rtc_ops = {
	.read_time = mc13xxx_rtc_read_time,
	.set_mmss = mc13xxx_rtc_set_mmss,
=======
static const struct rtc_class_ops mc13xxx_rtc_ops = {
	.read_time = mc13xxx_rtc_read_time,
	.set_time = mc13xxx_rtc_set_time,
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	.read_alarm = mc13xxx_rtc_read_alarm,
	.set_alarm = mc13xxx_rtc_set_alarm,
	.alarm_irq_enable = mc13xxx_rtc_alarm_irq_enable,
};

static irqreturn_t mc13xxx_rtc_reset_handler(int irq, void *dev)
{
	struct mc13xxx_rtc *priv = dev;
	struct mc13xxx *mc13xxx = priv->mc13xxx;

<<<<<<< HEAD
	dev_dbg(&priv->rtc->dev, "RTCRST\n");
=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	priv->valid = 0;

	mc13xxx_irq_mask(mc13xxx, irq);

	return IRQ_HANDLED;
}

static int __init mc13xxx_rtc_probe(struct platform_device *pdev)
{
	int ret;
	struct mc13xxx_rtc *priv;
	struct mc13xxx *mc13xxx;
<<<<<<< HEAD
	int rtcrst_pending;

	priv = kzalloc(sizeof(*priv), GFP_KERNEL);
=======

	priv = devm_kzalloc(&pdev->dev, sizeof(*priv), GFP_KERNEL);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	if (!priv)
		return -ENOMEM;

	mc13xxx = dev_get_drvdata(pdev->dev.parent);
	priv->mc13xxx = mc13xxx;
<<<<<<< HEAD

	platform_set_drvdata(pdev, priv);

	mc13xxx_lock(mc13xxx);

	ret = mc13xxx_irq_request(mc13xxx, MC13XXX_IRQ_RTCRST,
			mc13xxx_rtc_reset_handler, DRIVER_NAME, priv);
	if (ret)
		goto err_reset_irq_request;

	ret = mc13xxx_irq_status(mc13xxx, MC13XXX_IRQ_RTCRST,
			NULL, &rtcrst_pending);
	if (ret)
		goto err_reset_irq_status;

	priv->valid = !rtcrst_pending;

	ret = mc13xxx_irq_request_nounmask(mc13xxx, MC13XXX_IRQ_1HZ,
			mc13xxx_rtc_update_handler, DRIVER_NAME, priv);
	if (ret)
		goto err_update_irq_request;
=======
	priv->valid = 1;

	priv->rtc = devm_rtc_allocate_device(&pdev->dev);
	if (IS_ERR(priv->rtc))
		return PTR_ERR(priv->rtc);
	platform_set_drvdata(pdev, priv);

	priv->rtc->ops = &mc13xxx_rtc_ops;
	/* 15bit days + hours, minutes, seconds */
	priv->rtc->range_max = (timeu64_t)(1 << 15) * SEC_PER_DAY - 1;

	mc13xxx_lock(mc13xxx);

	mc13xxx_irq_ack(mc13xxx, MC13XXX_IRQ_RTCRST);

	ret = mc13xxx_irq_request(mc13xxx, MC13XXX_IRQ_RTCRST,
			mc13xxx_rtc_reset_handler, DRIVER_NAME, priv);
	if (ret)
		goto err_irq_request;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	ret = mc13xxx_irq_request_nounmask(mc13xxx, MC13XXX_IRQ_TODA,
			mc13xxx_rtc_alarm_handler, DRIVER_NAME, priv);
	if (ret)
<<<<<<< HEAD
		goto err_alarm_irq_request;

	mc13xxx_unlock(mc13xxx);

	priv->rtc = rtc_device_register(pdev->name,
			&pdev->dev, &mc13xxx_rtc_ops, THIS_MODULE);
	if (IS_ERR(priv->rtc)) {
		ret = PTR_ERR(priv->rtc);

		mc13xxx_lock(mc13xxx);

		mc13xxx_irq_free(mc13xxx, MC13XXX_IRQ_TODA, priv);
err_alarm_irq_request:

		mc13xxx_irq_free(mc13xxx, MC13XXX_IRQ_1HZ, priv);
err_update_irq_request:

err_reset_irq_status:

		mc13xxx_irq_free(mc13xxx, MC13XXX_IRQ_RTCRST, priv);
err_reset_irq_request:

		mc13xxx_unlock(mc13xxx);

		platform_set_drvdata(pdev, NULL);
		kfree(priv);
	}

	return ret;
}

static int __exit mc13xxx_rtc_remove(struct platform_device *pdev)
=======
		goto err_irq_request;

	mc13xxx_unlock(mc13xxx);

	ret = devm_rtc_register_device(priv->rtc);
	if (ret) {
		mc13xxx_lock(mc13xxx);
		goto err_irq_request;
	}

	return 0;

err_irq_request:
	mc13xxx_irq_free(mc13xxx, MC13XXX_IRQ_TODA, priv);
	mc13xxx_irq_free(mc13xxx, MC13XXX_IRQ_RTCRST, priv);

	mc13xxx_unlock(mc13xxx);

	return ret;
}

static void mc13xxx_rtc_remove(struct platform_device *pdev)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
{
	struct mc13xxx_rtc *priv = platform_get_drvdata(pdev);

	mc13xxx_lock(priv->mc13xxx);

<<<<<<< HEAD
	rtc_device_unregister(priv->rtc);

	mc13xxx_irq_free(priv->mc13xxx, MC13XXX_IRQ_TODA, priv);
	mc13xxx_irq_free(priv->mc13xxx, MC13XXX_IRQ_1HZ, priv);
	mc13xxx_irq_free(priv->mc13xxx, MC13XXX_IRQ_RTCRST, priv);

	mc13xxx_unlock(priv->mc13xxx);

	platform_set_drvdata(pdev, NULL);

	kfree(priv);

	return 0;
=======
	mc13xxx_irq_free(priv->mc13xxx, MC13XXX_IRQ_TODA, priv);
	mc13xxx_irq_free(priv->mc13xxx, MC13XXX_IRQ_RTCRST, priv);

	mc13xxx_unlock(priv->mc13xxx);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

static const struct platform_device_id mc13xxx_rtc_idtable[] = {
	{
		.name = "mc13783-rtc",
	}, {
		.name = "mc13892-rtc",
<<<<<<< HEAD
	},
	{ }
};

static struct platform_driver mc13xxx_rtc_driver = {
	.id_table = mc13xxx_rtc_idtable,
	.remove = __exit_p(mc13xxx_rtc_remove),
	.driver = {
		.name = DRIVER_NAME,
		.owner = THIS_MODULE,
	},
};

static int __init mc13xxx_rtc_init(void)
{
	return platform_driver_probe(&mc13xxx_rtc_driver, &mc13xxx_rtc_probe);
}
module_init(mc13xxx_rtc_init);

static void __exit mc13xxx_rtc_exit(void)
{
	platform_driver_unregister(&mc13xxx_rtc_driver);
}
module_exit(mc13xxx_rtc_exit);
=======
	}, {
		.name = "mc34708-rtc",
	},
	{ /* sentinel */ }
};
MODULE_DEVICE_TABLE(platform, mc13xxx_rtc_idtable);

static struct platform_driver mc13xxx_rtc_driver = {
	.id_table = mc13xxx_rtc_idtable,
	.remove_new = mc13xxx_rtc_remove,
	.driver = {
		.name = DRIVER_NAME,
	},
};

module_platform_driver_probe(mc13xxx_rtc_driver, &mc13xxx_rtc_probe);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

MODULE_AUTHOR("Sascha Hauer <s.hauer@pengutronix.de>");
MODULE_DESCRIPTION("RTC driver for Freescale MC13XXX PMIC");
MODULE_LICENSE("GPL v2");
<<<<<<< HEAD
MODULE_ALIAS("platform:" DRIVER_NAME);
=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
