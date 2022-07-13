<<<<<<< HEAD
/*
 * Copyright 2004-2008 Freescale Semiconductor, Inc. All Rights Reserved.
 *
 * The code contained herein is licensed under the GNU General Public
 * License. You may obtain a copy of the GNU General Public License
 * Version 2 or later at the following locations:
 *
 * http://www.opensource.org/licenses/gpl-license.html
 * http://www.gnu.org/copyleft/gpl.html
 */
=======
// SPDX-License-Identifier: GPL-2.0+
//
// Copyright 2004-2008 Freescale Semiconductor, Inc. All Rights Reserved.
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

#include <linux/io.h>
#include <linux/rtc.h>
#include <linux/module.h>
#include <linux/slab.h>
#include <linux/interrupt.h>
#include <linux/platform_device.h>
<<<<<<< HEAD
#include <linux/clk.h>

#include <mach/hardware.h>
=======
#include <linux/pm_wakeirq.h>
#include <linux/clk.h>
#include <linux/of.h>
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

#define RTC_INPUT_CLK_32768HZ	(0x00 << 5)
#define RTC_INPUT_CLK_32000HZ	(0x01 << 5)
#define RTC_INPUT_CLK_38400HZ	(0x02 << 5)

#define RTC_SW_BIT      (1 << 0)
#define RTC_ALM_BIT     (1 << 2)
#define RTC_1HZ_BIT     (1 << 4)
#define RTC_2HZ_BIT     (1 << 7)
#define RTC_SAM0_BIT    (1 << 8)
#define RTC_SAM1_BIT    (1 << 9)
#define RTC_SAM2_BIT    (1 << 10)
#define RTC_SAM3_BIT    (1 << 11)
#define RTC_SAM4_BIT    (1 << 12)
#define RTC_SAM5_BIT    (1 << 13)
#define RTC_SAM6_BIT    (1 << 14)
#define RTC_SAM7_BIT    (1 << 15)
#define PIT_ALL_ON      (RTC_2HZ_BIT | RTC_SAM0_BIT | RTC_SAM1_BIT | \
			 RTC_SAM2_BIT | RTC_SAM3_BIT | RTC_SAM4_BIT | \
			 RTC_SAM5_BIT | RTC_SAM6_BIT | RTC_SAM7_BIT)

#define RTC_ENABLE_BIT  (1 << 7)

#define MAX_PIE_NUM     9
#define MAX_PIE_FREQ    512
<<<<<<< HEAD
static const u32 PIE_BIT_DEF[MAX_PIE_NUM][2] = {
	{ 2,		RTC_2HZ_BIT },
	{ 4,		RTC_SAM0_BIT },
	{ 8,		RTC_SAM1_BIT },
	{ 16,		RTC_SAM2_BIT },
	{ 32,		RTC_SAM3_BIT },
	{ 64,		RTC_SAM4_BIT },
	{ 128,		RTC_SAM5_BIT },
	{ 256,		RTC_SAM6_BIT },
	{ MAX_PIE_FREQ,	RTC_SAM7_BIT },
};
=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

#define MXC_RTC_TIME	0
#define MXC_RTC_ALARM	1

#define RTC_HOURMIN	0x00	/*  32bit rtc hour/min counter reg */
#define RTC_SECOND	0x04	/*  32bit rtc seconds counter reg */
#define RTC_ALRM_HM	0x08	/*  32bit rtc alarm hour/min reg */
#define RTC_ALRM_SEC	0x0C	/*  32bit rtc alarm seconds reg */
#define RTC_RTCCTL	0x10	/*  32bit rtc control reg */
#define RTC_RTCISR	0x14	/*  32bit rtc interrupt status reg */
#define RTC_RTCIENR	0x18	/*  32bit rtc interrupt enable reg */
#define RTC_STPWCH	0x1C	/*  32bit rtc stopwatch min reg */
#define RTC_DAYR	0x20	/*  32bit rtc days counter reg */
#define RTC_DAYALARM	0x24	/*  32bit rtc day alarm reg */
#define RTC_TEST1	0x28	/*  32bit rtc test reg 1 */
#define RTC_TEST2	0x2C	/*  32bit rtc test reg 2 */
#define RTC_TEST3	0x30	/*  32bit rtc test reg 3 */

<<<<<<< HEAD
=======
enum imx_rtc_type {
	IMX1_RTC,
	IMX21_RTC,
};

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
struct rtc_plat_data {
	struct rtc_device *rtc;
	void __iomem *ioaddr;
	int irq;
<<<<<<< HEAD
	struct clk *clk;
	struct rtc_time g_rtc_alarm;
};

=======
	struct clk *clk_ref;
	struct clk *clk_ipg;
	struct rtc_time g_rtc_alarm;
	enum imx_rtc_type devtype;
};

static const struct of_device_id imx_rtc_dt_ids[] = {
	{ .compatible = "fsl,imx1-rtc", .data = (const void *)IMX1_RTC },
	{ .compatible = "fsl,imx21-rtc", .data = (const void *)IMX21_RTC },
	{}
};
MODULE_DEVICE_TABLE(of, imx_rtc_dt_ids);

static inline int is_imx1_rtc(struct rtc_plat_data *data)
{
	return data->devtype == IMX1_RTC;
}

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
/*
 * This function is used to obtain the RTC time or the alarm value in
 * second.
 */
<<<<<<< HEAD
static u32 get_alarm_or_time(struct device *dev, int time_alarm)
{
	struct platform_device *pdev = to_platform_device(dev);
	struct rtc_plat_data *pdata = platform_get_drvdata(pdev);
=======
static time64_t get_alarm_or_time(struct device *dev, int time_alarm)
{
	struct rtc_plat_data *pdata = dev_get_drvdata(dev);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	void __iomem *ioaddr = pdata->ioaddr;
	u32 day = 0, hr = 0, min = 0, sec = 0, hr_min = 0;

	switch (time_alarm) {
	case MXC_RTC_TIME:
		day = readw(ioaddr + RTC_DAYR);
		hr_min = readw(ioaddr + RTC_HOURMIN);
		sec = readw(ioaddr + RTC_SECOND);
		break;
	case MXC_RTC_ALARM:
		day = readw(ioaddr + RTC_DAYALARM);
		hr_min = readw(ioaddr + RTC_ALRM_HM) & 0xffff;
		sec = readw(ioaddr + RTC_ALRM_SEC);
		break;
	}

	hr = hr_min >> 8;
	min = hr_min & 0xff;

<<<<<<< HEAD
	return (((day * 24 + hr) * 60) + min) * 60 + sec;
=======
	return ((((time64_t)day * 24 + hr) * 60) + min) * 60 + sec;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

/*
 * This function sets the RTC alarm value or the time value.
 */
<<<<<<< HEAD
static void set_alarm_or_time(struct device *dev, int time_alarm, u32 time)
{
	u32 day, hr, min, sec, temp;
	struct platform_device *pdev = to_platform_device(dev);
	struct rtc_plat_data *pdata = platform_get_drvdata(pdev);
	void __iomem *ioaddr = pdata->ioaddr;

	day = time / 86400;
	time -= day * 86400;

	/* time is within a day now */
	hr = time / 3600;
	time -= hr * 3600;

	/* time is within an hour now */
	min = time / 60;
	sec = time - min * 60;
=======
static void set_alarm_or_time(struct device *dev, int time_alarm, time64_t time)
{
	u32 tod, day, hr, min, sec, temp;
	struct rtc_plat_data *pdata = dev_get_drvdata(dev);
	void __iomem *ioaddr = pdata->ioaddr;

	day = div_s64_rem(time, 86400, &tod);

	/* time is within a day now */
	hr = tod / 3600;
	tod -= hr * 3600;

	/* time is within an hour now */
	min = tod / 60;
	sec = tod - min * 60;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	temp = (hr << 8) + min;

	switch (time_alarm) {
	case MXC_RTC_TIME:
		writew(day, ioaddr + RTC_DAYR);
		writew(sec, ioaddr + RTC_SECOND);
		writew(temp, ioaddr + RTC_HOURMIN);
		break;
	case MXC_RTC_ALARM:
		writew(day, ioaddr + RTC_DAYALARM);
		writew(sec, ioaddr + RTC_ALRM_SEC);
		writew(temp, ioaddr + RTC_ALRM_HM);
		break;
	}
}

/*
 * This function updates the RTC alarm registers and then clears all the
 * interrupt status bits.
 */
<<<<<<< HEAD
static int rtc_update_alarm(struct device *dev, struct rtc_time *alrm)
{
	struct rtc_time alarm_tm, now_tm;
	unsigned long now, time;
	struct platform_device *pdev = to_platform_device(dev);
	struct rtc_plat_data *pdata = platform_get_drvdata(pdev);
	void __iomem *ioaddr = pdata->ioaddr;

	now = get_alarm_or_time(dev, MXC_RTC_TIME);
	rtc_time_to_tm(now, &now_tm);
	alarm_tm.tm_year = now_tm.tm_year;
	alarm_tm.tm_mon = now_tm.tm_mon;
	alarm_tm.tm_mday = now_tm.tm_mday;
	alarm_tm.tm_hour = alrm->tm_hour;
	alarm_tm.tm_min = alrm->tm_min;
	alarm_tm.tm_sec = alrm->tm_sec;
	rtc_tm_to_time(&alarm_tm, &time);
=======
static void rtc_update_alarm(struct device *dev, struct rtc_time *alrm)
{
	time64_t time;
	struct rtc_plat_data *pdata = dev_get_drvdata(dev);
	void __iomem *ioaddr = pdata->ioaddr;

	time = rtc_tm_to_time64(alrm);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	/* clear all the interrupt status bits */
	writew(readw(ioaddr + RTC_RTCISR), ioaddr + RTC_RTCISR);
	set_alarm_or_time(dev, MXC_RTC_ALARM, time);
<<<<<<< HEAD

	return 0;
=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

static void mxc_rtc_irq_enable(struct device *dev, unsigned int bit,
				unsigned int enabled)
{
<<<<<<< HEAD
	struct platform_device *pdev = to_platform_device(dev);
	struct rtc_plat_data *pdata = platform_get_drvdata(pdev);
	void __iomem *ioaddr = pdata->ioaddr;
	u32 reg;

	spin_lock_irq(&pdata->rtc->irq_lock);
=======
	struct rtc_plat_data *pdata = dev_get_drvdata(dev);
	void __iomem *ioaddr = pdata->ioaddr;
	u32 reg;
	unsigned long flags;

	spin_lock_irqsave(&pdata->rtc->irq_lock, flags);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	reg = readw(ioaddr + RTC_RTCIENR);

	if (enabled)
		reg |= bit;
	else
		reg &= ~bit;

	writew(reg, ioaddr + RTC_RTCIENR);
<<<<<<< HEAD
	spin_unlock_irq(&pdata->rtc->irq_lock);
=======
	spin_unlock_irqrestore(&pdata->rtc->irq_lock, flags);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

/* This function is the RTC interrupt service routine. */
static irqreturn_t mxc_rtc_interrupt(int irq, void *dev_id)
{
	struct platform_device *pdev = dev_id;
	struct rtc_plat_data *pdata = platform_get_drvdata(pdev);
	void __iomem *ioaddr = pdata->ioaddr;
<<<<<<< HEAD
	unsigned long flags;
	u32 status;
	u32 events = 0;

	spin_lock_irqsave(&pdata->rtc->irq_lock, flags);
=======
	u32 status;
	u32 events = 0;

	spin_lock(&pdata->rtc->irq_lock);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	status = readw(ioaddr + RTC_RTCISR) & readw(ioaddr + RTC_RTCIENR);
	/* clear interrupt sources */
	writew(status, ioaddr + RTC_RTCISR);

	/* update irq data & counter */
	if (status & RTC_ALM_BIT) {
		events |= (RTC_AF | RTC_IRQF);
		/* RTC alarm should be one-shot */
		mxc_rtc_irq_enable(&pdev->dev, RTC_ALM_BIT, 0);
	}

<<<<<<< HEAD
	if (status & RTC_1HZ_BIT)
		events |= (RTC_UF | RTC_IRQF);

=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	if (status & PIT_ALL_ON)
		events |= (RTC_PF | RTC_IRQF);

	rtc_update_irq(pdata->rtc, 1, events);
<<<<<<< HEAD
	spin_unlock_irqrestore(&pdata->rtc->irq_lock, flags);
=======
	spin_unlock(&pdata->rtc->irq_lock);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	return IRQ_HANDLED;
}

<<<<<<< HEAD
/*
 * Clear all interrupts and release the IRQ
 */
static void mxc_rtc_release(struct device *dev)
{
	struct platform_device *pdev = to_platform_device(dev);
	struct rtc_plat_data *pdata = platform_get_drvdata(pdev);
	void __iomem *ioaddr = pdata->ioaddr;

	spin_lock_irq(&pdata->rtc->irq_lock);

	/* Disable all rtc interrupts */
	writew(0, ioaddr + RTC_RTCIENR);

	/* Clear all interrupt status */
	writew(0xffffffff, ioaddr + RTC_RTCISR);

	spin_unlock_irq(&pdata->rtc->irq_lock);
}

=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
static int mxc_rtc_alarm_irq_enable(struct device *dev, unsigned int enabled)
{
	mxc_rtc_irq_enable(dev, RTC_ALM_BIT, enabled);
	return 0;
}

/*
 * This function reads the current RTC time into tm in Gregorian date.
 */
static int mxc_rtc_read_time(struct device *dev, struct rtc_time *tm)
{
<<<<<<< HEAD
	u32 val;
=======
	time64_t val;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	/* Avoid roll-over from reading the different registers */
	do {
		val = get_alarm_or_time(dev, MXC_RTC_TIME);
	} while (val != get_alarm_or_time(dev, MXC_RTC_TIME));

<<<<<<< HEAD
	rtc_time_to_tm(val, tm);
=======
	rtc_time64_to_tm(val, tm);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	return 0;
}

/*
 * This function sets the internal RTC time based on tm in Gregorian date.
 */
<<<<<<< HEAD
static int mxc_rtc_set_mmss(struct device *dev, unsigned long time)
{
	/*
	 * TTC_DAYR register is 9-bit in MX1 SoC, save time and day of year only
	 */
	if (cpu_is_mx1()) {
		struct rtc_time tm;

		rtc_time_to_tm(time, &tm);
		tm.tm_year = 70;
		rtc_tm_to_time(&tm, &time);
	}
=======
static int mxc_rtc_set_time(struct device *dev, struct rtc_time *tm)
{
	time64_t time = rtc_tm_to_time64(tm);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	/* Avoid roll-over from reading the different registers */
	do {
		set_alarm_or_time(dev, MXC_RTC_TIME, time);
	} while (time != get_alarm_or_time(dev, MXC_RTC_TIME));

	return 0;
}

/*
 * This function reads the current alarm value into the passed in 'alrm'
 * argument. It updates the alrm's pending field value based on the whether
 * an alarm interrupt occurs or not.
 */
static int mxc_rtc_read_alarm(struct device *dev, struct rtc_wkalrm *alrm)
{
<<<<<<< HEAD
	struct platform_device *pdev = to_platform_device(dev);
	struct rtc_plat_data *pdata = platform_get_drvdata(pdev);
	void __iomem *ioaddr = pdata->ioaddr;

	rtc_time_to_tm(get_alarm_or_time(dev, MXC_RTC_ALARM), &alrm->time);
=======
	struct rtc_plat_data *pdata = dev_get_drvdata(dev);
	void __iomem *ioaddr = pdata->ioaddr;

	rtc_time64_to_tm(get_alarm_or_time(dev, MXC_RTC_ALARM), &alrm->time);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	alrm->pending = ((readw(ioaddr + RTC_RTCISR) & RTC_ALM_BIT)) ? 1 : 0;

	return 0;
}

/*
 * This function sets the RTC alarm based on passed in alrm.
 */
static int mxc_rtc_set_alarm(struct device *dev, struct rtc_wkalrm *alrm)
{
<<<<<<< HEAD
	struct platform_device *pdev = to_platform_device(dev);
	struct rtc_plat_data *pdata = platform_get_drvdata(pdev);
	int ret;

	ret = rtc_update_alarm(dev, &alrm->time);
	if (ret)
		return ret;
=======
	struct rtc_plat_data *pdata = dev_get_drvdata(dev);

	rtc_update_alarm(dev, &alrm->time);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	memcpy(&pdata->g_rtc_alarm, &alrm->time, sizeof(struct rtc_time));
	mxc_rtc_irq_enable(dev, RTC_ALM_BIT, alrm->enabled);

	return 0;
}

/* RTC layer */
<<<<<<< HEAD
static struct rtc_class_ops mxc_rtc_ops = {
	.release		= mxc_rtc_release,
	.read_time		= mxc_rtc_read_time,
	.set_mmss		= mxc_rtc_set_mmss,
=======
static const struct rtc_class_ops mxc_rtc_ops = {
	.read_time		= mxc_rtc_read_time,
	.set_time		= mxc_rtc_set_time,
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	.read_alarm		= mxc_rtc_read_alarm,
	.set_alarm		= mxc_rtc_set_alarm,
	.alarm_irq_enable	= mxc_rtc_alarm_irq_enable,
};

<<<<<<< HEAD
static int __init mxc_rtc_probe(struct platform_device *pdev)
{
	struct resource *res;
=======
static int mxc_rtc_probe(struct platform_device *pdev)
{
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	struct rtc_device *rtc;
	struct rtc_plat_data *pdata = NULL;
	u32 reg;
	unsigned long rate;
	int ret;

<<<<<<< HEAD
	res = platform_get_resource(pdev, IORESOURCE_MEM, 0);
	if (!res)
		return -ENODEV;

=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	pdata = devm_kzalloc(&pdev->dev, sizeof(*pdata), GFP_KERNEL);
	if (!pdata)
		return -ENOMEM;

<<<<<<< HEAD
	if (!devm_request_mem_region(&pdev->dev, res->start,
				     resource_size(res), pdev->name))
		return -EBUSY;

	pdata->ioaddr = devm_ioremap(&pdev->dev, res->start,
				     resource_size(res));

	pdata->clk = clk_get(&pdev->dev, "rtc");
	if (IS_ERR(pdata->clk)) {
		dev_err(&pdev->dev, "unable to get clock!\n");
		ret = PTR_ERR(pdata->clk);
		goto exit_free_pdata;
	}

	clk_enable(pdata->clk);
	rate = clk_get_rate(pdata->clk);
=======
	pdata->devtype = (uintptr_t)of_device_get_match_data(&pdev->dev);

	pdata->ioaddr = devm_platform_ioremap_resource(pdev, 0);
	if (IS_ERR(pdata->ioaddr))
		return PTR_ERR(pdata->ioaddr);

	rtc = devm_rtc_allocate_device(&pdev->dev);
	if (IS_ERR(rtc))
		return PTR_ERR(rtc);

	pdata->rtc = rtc;
	rtc->ops = &mxc_rtc_ops;
	if (is_imx1_rtc(pdata)) {
		struct rtc_time tm;

		/* 9bit days + hours minutes seconds */
		rtc->range_max = (1 << 9) * 86400 - 1;

		/*
		 * Set the start date as beginning of the current year. This can
		 * be overridden using device tree.
		 */
		rtc_time64_to_tm(ktime_get_real_seconds(), &tm);
		rtc->start_secs =  mktime64(tm.tm_year, 1, 1, 0, 0, 0);
		rtc->set_start_time = true;
	} else {
		/* 16bit days + hours minutes seconds */
		rtc->range_max = (1 << 16) * 86400ULL - 1;
	}

	pdata->clk_ipg = devm_clk_get_enabled(&pdev->dev, "ipg");
	if (IS_ERR(pdata->clk_ipg)) {
		dev_err(&pdev->dev, "unable to get ipg clock!\n");
		return PTR_ERR(pdata->clk_ipg);
	}

	pdata->clk_ref = devm_clk_get_enabled(&pdev->dev, "ref");
	if (IS_ERR(pdata->clk_ref)) {
		dev_err(&pdev->dev, "unable to get ref clock!\n");
		return PTR_ERR(pdata->clk_ref);
	}

	rate = clk_get_rate(pdata->clk_ref);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	if (rate == 32768)
		reg = RTC_INPUT_CLK_32768HZ;
	else if (rate == 32000)
		reg = RTC_INPUT_CLK_32000HZ;
	else if (rate == 38400)
		reg = RTC_INPUT_CLK_38400HZ;
	else {
		dev_err(&pdev->dev, "rtc clock is not valid (%lu)\n", rate);
<<<<<<< HEAD
		ret = -EINVAL;
		goto exit_put_clk;
=======
		return -EINVAL;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	}

	reg |= RTC_ENABLE_BIT;
	writew(reg, (pdata->ioaddr + RTC_RTCCTL));
	if (((readw(pdata->ioaddr + RTC_RTCCTL)) & RTC_ENABLE_BIT) == 0) {
		dev_err(&pdev->dev, "hardware module can't be enabled!\n");
<<<<<<< HEAD
		ret = -EIO;
		goto exit_put_clk;
=======
		return -EIO;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	}

	platform_set_drvdata(pdev, pdata);

	/* Configure and enable the RTC */
	pdata->irq = platform_get_irq(pdev, 0);

	if (pdata->irq >= 0 &&
	    devm_request_irq(&pdev->dev, pdata->irq, mxc_rtc_interrupt,
			     IRQF_SHARED, pdev->name, pdev) < 0) {
		dev_warn(&pdev->dev, "interrupt not available.\n");
		pdata->irq = -1;
	}

<<<<<<< HEAD
	if (pdata->irq >=0)
		device_init_wakeup(&pdev->dev, 1);

	rtc = rtc_device_register(pdev->name, &pdev->dev, &mxc_rtc_ops,
				  THIS_MODULE);
	if (IS_ERR(rtc)) {
		ret = PTR_ERR(rtc);
		goto exit_clr_drvdata;
	}

	pdata->rtc = rtc;

	return 0;

exit_clr_drvdata:
	platform_set_drvdata(pdev, NULL);
exit_put_clk:
	clk_disable(pdata->clk);
	clk_put(pdata->clk);

exit_free_pdata:
=======
	if (pdata->irq >= 0) {
		device_init_wakeup(&pdev->dev, 1);
		ret = dev_pm_set_wake_irq(&pdev->dev, pdata->irq);
		if (ret)
			dev_err(&pdev->dev, "failed to enable irq wake\n");
	}

	ret = devm_rtc_register_device(rtc);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	return ret;
}

<<<<<<< HEAD
static int __exit mxc_rtc_remove(struct platform_device *pdev)
{
	struct rtc_plat_data *pdata = platform_get_drvdata(pdev);

	rtc_device_unregister(pdata->rtc);

	clk_disable(pdata->clk);
	clk_put(pdata->clk);
	platform_set_drvdata(pdev, NULL);

	return 0;
}

#ifdef CONFIG_PM
static int mxc_rtc_suspend(struct device *dev)
{
	struct rtc_plat_data *pdata = dev_get_drvdata(dev);

	if (device_may_wakeup(dev))
		enable_irq_wake(pdata->irq);

	return 0;
}

static int mxc_rtc_resume(struct device *dev)
{
	struct rtc_plat_data *pdata = dev_get_drvdata(dev);

	if (device_may_wakeup(dev))
		disable_irq_wake(pdata->irq);

	return 0;
}

static struct dev_pm_ops mxc_rtc_pm_ops = {
	.suspend	= mxc_rtc_suspend,
	.resume		= mxc_rtc_resume,
};
#endif

static struct platform_driver mxc_rtc_driver = {
	.driver = {
		   .name	= "mxc_rtc",
#ifdef CONFIG_PM
		   .pm		= &mxc_rtc_pm_ops,
#endif
		   .owner	= THIS_MODULE,
	},
	.remove		= __exit_p(mxc_rtc_remove),
};

static int __init mxc_rtc_init(void)
{
	return platform_driver_probe(&mxc_rtc_driver, mxc_rtc_probe);
}

static void __exit mxc_rtc_exit(void)
{
	platform_driver_unregister(&mxc_rtc_driver);
}

module_init(mxc_rtc_init);
module_exit(mxc_rtc_exit);
=======
static struct platform_driver mxc_rtc_driver = {
	.driver = {
		   .name	= "mxc_rtc",
		   .of_match_table = imx_rtc_dt_ids,
	},
	.probe = mxc_rtc_probe,
};

module_platform_driver(mxc_rtc_driver)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

MODULE_AUTHOR("Daniel Mack <daniel@caiaq.de>");
MODULE_DESCRIPTION("RTC driver for Freescale MXC");
MODULE_LICENSE("GPL");

