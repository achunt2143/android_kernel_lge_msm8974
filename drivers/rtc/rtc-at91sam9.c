<<<<<<< HEAD
=======
// SPDX-License-Identifier: GPL-2.0+
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
/*
 * "RTT as Real Time Clock" driver for AT91SAM9 SoC family
 *
 * (C) 2007 Michel Benoit
 *
 * Based on rtc-at91rm9200.c by Rick Bronson
<<<<<<< HEAD
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version
 * 2 of the License, or (at your option) any later version.
 */

#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/platform_device.h>
#include <linux/time.h>
#include <linux/rtc.h>
#include <linux/interrupt.h>
#include <linux/ioctl.h>
#include <linux/slab.h>

#include <mach/board.h>
#include <mach/at91_rtt.h>
#include <mach/cpu.h>

=======
 */

#include <linux/clk.h>
#include <linux/interrupt.h>
#include <linux/ioctl.h>
#include <linux/io.h>
#include <linux/kernel.h>
#include <linux/mfd/syscon.h>
#include <linux/module.h>
#include <linux/of.h>
#include <linux/platform_device.h>
#include <linux/regmap.h>
#include <linux/rtc.h>
#include <linux/slab.h>
#include <linux/suspend.h>
#include <linux/time.h>
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

/*
 * This driver uses two configurable hardware resources that live in the
 * AT91SAM9 backup power domain (intended to be powered at all times)
 * to implement the Real Time Clock interfaces
 *
 *  - A "Real-time Timer" (RTT) counts up in seconds from a base time.
 *    We can't assign the counter value (CRTV) ... but we can reset it.
 *
 *  - One of the "General Purpose Backup Registers" (GPBRs) holds the
 *    base time, normally an offset from the beginning of the POSIX
 *    epoch (1970-Jan-1 00:00:00 UTC).  Some systems also include the
 *    local timezone's offset.
 *
 * The RTC's value is the RTT counter plus that offset.  The RTC's alarm
 * is likewise a base (ALMV) plus that offset.
 *
 * Not all RTTs will be used as RTCs; some systems have multiple RTTs to
 * choose from, or a "real" RTC module.  All systems have multiple GPBR
 * registers available, likewise usable for more than "RTC" support.
 */

<<<<<<< HEAD
=======
#define AT91_RTT_MR		0x00		/* Real-time Mode Register */
#define AT91_RTT_RTPRES		(0xffff << 0)	/* Timer Prescaler Value */
#define AT91_RTT_ALMIEN		BIT(16)		/* Alarm Interrupt Enable */
#define AT91_RTT_RTTINCIEN	BIT(17)		/* Increment Interrupt Enable */
#define AT91_RTT_RTTRST		BIT(18)		/* Timer Restart */

#define AT91_RTT_AR		0x04		/* Real-time Alarm Register */
#define AT91_RTT_ALMV		(0xffffffff)	/* Alarm Value */

#define AT91_RTT_VR		0x08		/* Real-time Value Register */
#define AT91_RTT_CRTV		(0xffffffff)	/* Current Real-time Value */

#define AT91_RTT_SR		0x0c		/* Real-time Status Register */
#define AT91_RTT_ALMS		BIT(0)		/* Alarm Status */
#define AT91_RTT_RTTINC		BIT(1)		/* Timer Increment */

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
/*
 * We store ALARM_DISABLED in ALMV to record that no alarm is set.
 * It's also the reset value for that field.
 */
#define ALARM_DISABLED	((u32)~0)

<<<<<<< HEAD

=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
struct sam9_rtc {
	void __iomem		*rtt;
	struct rtc_device	*rtcdev;
	u32			imr;
<<<<<<< HEAD
	void __iomem		*gpbr;
};

#define rtt_readl(rtc, field) \
	__raw_readl((rtc)->rtt + AT91_RTT_ ## field)
#define rtt_writel(rtc, field, val) \
	__raw_writel((val), (rtc)->rtt + AT91_RTT_ ## field)

#define gpbr_readl(rtc) \
	__raw_readl((rtc)->gpbr)
#define gpbr_writel(rtc, val) \
	__raw_writel((val), (rtc)->gpbr)
=======
	struct regmap		*gpbr;
	unsigned int		gpbr_offset;
	int			irq;
	struct clk		*sclk;
	bool			suspended;
	unsigned long		events;
	spinlock_t		lock;
};

#define rtt_readl(rtc, field) \
	readl((rtc)->rtt + AT91_RTT_ ## field)
#define rtt_writel(rtc, field, val) \
	writel((val), (rtc)->rtt + AT91_RTT_ ## field)

static inline unsigned int gpbr_readl(struct sam9_rtc *rtc)
{
	unsigned int val;

	regmap_read(rtc->gpbr, rtc->gpbr_offset, &val);

	return val;
}

static inline void gpbr_writel(struct sam9_rtc *rtc, unsigned int val)
{
	regmap_write(rtc->gpbr, rtc->gpbr_offset, val);
}
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

/*
 * Read current time and date in RTC
 */
static int at91_rtc_readtime(struct device *dev, struct rtc_time *tm)
{
	struct sam9_rtc *rtc = dev_get_drvdata(dev);
	u32 secs, secs2;
	u32 offset;

	/* read current time offset */
	offset = gpbr_readl(rtc);
	if (offset == 0)
		return -EILSEQ;

	/* reread the counter to help sync the two clock domains */
	secs = rtt_readl(rtc, VR);
	secs2 = rtt_readl(rtc, VR);
	if (secs != secs2)
		secs = rtt_readl(rtc, VR);

<<<<<<< HEAD
	rtc_time_to_tm(offset + secs, tm);

	dev_dbg(dev, "%s: %4d-%02d-%02d %02d:%02d:%02d\n", "readtime",
		1900 + tm->tm_year, tm->tm_mon, tm->tm_mday,
		tm->tm_hour, tm->tm_min, tm->tm_sec);
=======
	rtc_time64_to_tm(offset + secs, tm);

	dev_dbg(dev, "%s: %ptR\n", __func__, tm);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	return 0;
}

/*
 * Set current time and date in RTC
 */
static int at91_rtc_settime(struct device *dev, struct rtc_time *tm)
{
	struct sam9_rtc *rtc = dev_get_drvdata(dev);
<<<<<<< HEAD
	int err;
	u32 offset, alarm, mr;
	unsigned long secs;

	dev_dbg(dev, "%s: %4d-%02d-%02d %02d:%02d:%02d\n", "settime",
		1900 + tm->tm_year, tm->tm_mon, tm->tm_mday,
		tm->tm_hour, tm->tm_min, tm->tm_sec);

	err = rtc_tm_to_time(tm, &secs);
	if (err != 0)
		return err;
=======
	u32 offset, alarm, mr;
	unsigned long secs;

	dev_dbg(dev, "%s: %ptR\n", __func__, tm);

	secs = rtc_tm_to_time64(tm);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	mr = rtt_readl(rtc, MR);

	/* disable interrupts */
	rtt_writel(rtc, MR, mr & ~(AT91_RTT_ALMIEN | AT91_RTT_RTTINCIEN));

	/* read current time offset */
	offset = gpbr_readl(rtc);

	/* store the new base time in a battery backup register */
	secs += 1;
	gpbr_writel(rtc, secs);

	/* adjust the alarm time for the new base */
	alarm = rtt_readl(rtc, AR);
	if (alarm != ALARM_DISABLED) {
		if (offset > secs) {
			/* time jumped backwards, increase time until alarm */
			alarm += (offset - secs);
		} else if ((alarm + offset) > secs) {
			/* time jumped forwards, decrease time until alarm */
			alarm -= (secs - offset);
		} else {
			/* time jumped past the alarm, disable alarm */
			alarm = ALARM_DISABLED;
			mr &= ~AT91_RTT_ALMIEN;
		}
		rtt_writel(rtc, AR, alarm);
	}

	/* reset the timer, and re-enable interrupts */
	rtt_writel(rtc, MR, mr | AT91_RTT_RTTRST);

	return 0;
}

static int at91_rtc_readalarm(struct device *dev, struct rtc_wkalrm *alrm)
{
	struct sam9_rtc *rtc = dev_get_drvdata(dev);
	struct rtc_time *tm = &alrm->time;
	u32 alarm = rtt_readl(rtc, AR);
	u32 offset;

	offset = gpbr_readl(rtc);
	if (offset == 0)
		return -EILSEQ;

	memset(alrm, 0, sizeof(*alrm));
<<<<<<< HEAD
	if (alarm != ALARM_DISABLED && offset != 0) {
		rtc_time_to_tm(offset + alarm, tm);

		dev_dbg(dev, "%s: %4d-%02d-%02d %02d:%02d:%02d\n", "readalarm",
			1900 + tm->tm_year, tm->tm_mon, tm->tm_mday,
			tm->tm_hour, tm->tm_min, tm->tm_sec);
=======
	if (alarm != ALARM_DISABLED) {
		rtc_time64_to_tm(offset + alarm, tm);

		dev_dbg(dev, "%s: %ptR\n", __func__, tm);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

		if (rtt_readl(rtc, MR) & AT91_RTT_ALMIEN)
			alrm->enabled = 1;
	}

	return 0;
}

static int at91_rtc_setalarm(struct device *dev, struct rtc_wkalrm *alrm)
{
	struct sam9_rtc *rtc = dev_get_drvdata(dev);
	struct rtc_time *tm = &alrm->time;
	unsigned long secs;
	u32 offset;
	u32 mr;
<<<<<<< HEAD
	int err;

	err = rtc_tm_to_time(tm, &secs);
	if (err != 0)
		return err;
=======

	secs = rtc_tm_to_time64(tm);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	offset = gpbr_readl(rtc);
	if (offset == 0) {
		/* time is not set */
		return -EILSEQ;
	}
	mr = rtt_readl(rtc, MR);
	rtt_writel(rtc, MR, mr & ~AT91_RTT_ALMIEN);

	/* alarm in the past? finish and leave disabled */
	if (secs <= offset) {
		rtt_writel(rtc, AR, ALARM_DISABLED);
		return 0;
	}

	/* else set alarm and maybe enable it */
	rtt_writel(rtc, AR, secs - offset);
	if (alrm->enabled)
		rtt_writel(rtc, MR, mr | AT91_RTT_ALMIEN);

<<<<<<< HEAD
	dev_dbg(dev, "%s: %4d-%02d-%02d %02d:%02d:%02d\n", "setalarm",
		tm->tm_year, tm->tm_mon, tm->tm_mday, tm->tm_hour,
		tm->tm_min, tm->tm_sec);
=======
	dev_dbg(dev, "%s: %ptR\n", __func__, tm);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	return 0;
}

static int at91_rtc_alarm_irq_enable(struct device *dev, unsigned int enabled)
{
	struct sam9_rtc *rtc = dev_get_drvdata(dev);
	u32 mr = rtt_readl(rtc, MR);

	dev_dbg(dev, "alarm_irq_enable: enabled=%08x, mr %08x\n", enabled, mr);
	if (enabled)
		rtt_writel(rtc, MR, mr | AT91_RTT_ALMIEN);
	else
		rtt_writel(rtc, MR, mr & ~AT91_RTT_ALMIEN);
	return 0;
}

/*
 * Provide additional RTC information in /proc/driver/rtc
 */
static int at91_rtc_proc(struct device *dev, struct seq_file *seq)
{
	struct sam9_rtc *rtc = dev_get_drvdata(dev);
<<<<<<< HEAD
	u32 mr = mr = rtt_readl(rtc, MR);

	seq_printf(seq, "update_IRQ\t: %s\n",
			(mr & AT91_RTT_RTTINCIEN) ? "yes" : "no");
	return 0;
}

/*
 * IRQ handler for the RTC
 */
static irqreturn_t at91_rtc_interrupt(int irq, void *_rtc)
{
	struct sam9_rtc *rtc = _rtc;
	u32 sr, mr;
	unsigned long events = 0;
=======
	u32 mr = rtt_readl(rtc, MR);

	seq_printf(seq, "update_IRQ\t: %s\n",
		   (mr & AT91_RTT_RTTINCIEN) ? "yes" : "no");
	return 0;
}

static irqreturn_t at91_rtc_cache_events(struct sam9_rtc *rtc)
{
	u32 sr, mr;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	/* Shared interrupt may be for another device.  Note: reading
	 * SR clears it, so we must only read it in this irq handler!
	 */
	mr = rtt_readl(rtc, MR) & (AT91_RTT_ALMIEN | AT91_RTT_RTTINCIEN);
	sr = rtt_readl(rtc, SR) & (mr >> 16);
	if (!sr)
		return IRQ_NONE;

	/* alarm status */
	if (sr & AT91_RTT_ALMS)
<<<<<<< HEAD
		events |= (RTC_AF | RTC_IRQF);

	/* timer update/increment */
	if (sr & AT91_RTT_RTTINC)
		events |= (RTC_UF | RTC_IRQF);

	rtc_update_irq(rtc->rtcdev, 1, events);

	pr_debug("%s: num=%ld, events=0x%02lx\n", __func__,
		events >> 8, events & 0x000000FF);
=======
		rtc->events |= (RTC_AF | RTC_IRQF);

	/* timer update/increment */
	if (sr & AT91_RTT_RTTINC)
		rtc->events |= (RTC_UF | RTC_IRQF);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	return IRQ_HANDLED;
}

<<<<<<< HEAD
=======
static void at91_rtc_flush_events(struct sam9_rtc *rtc)
{
	if (!rtc->events)
		return;

	rtc_update_irq(rtc->rtcdev, 1, rtc->events);
	rtc->events = 0;

	pr_debug("%s: num=%ld, events=0x%02lx\n", __func__,
		 rtc->events >> 8, rtc->events & 0x000000FF);
}

/*
 * IRQ handler for the RTC
 */
static irqreturn_t at91_rtc_interrupt(int irq, void *_rtc)
{
	struct sam9_rtc *rtc = _rtc;
	int ret;

	spin_lock(&rtc->lock);

	ret = at91_rtc_cache_events(rtc);

	/* We're called in suspended state */
	if (rtc->suspended) {
		/* Mask irqs coming from this peripheral */
		rtt_writel(rtc, MR,
			   rtt_readl(rtc, MR) &
			   ~(AT91_RTT_ALMIEN | AT91_RTT_RTTINCIEN));
		/* Trigger a system wakeup */
		pm_system_wakeup();
	} else {
		at91_rtc_flush_events(rtc);
	}

	spin_unlock(&rtc->lock);

	return ret;
}

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
static const struct rtc_class_ops at91_rtc_ops = {
	.read_time	= at91_rtc_readtime,
	.set_time	= at91_rtc_settime,
	.read_alarm	= at91_rtc_readalarm,
	.set_alarm	= at91_rtc_setalarm,
	.proc		= at91_rtc_proc,
	.alarm_irq_enable = at91_rtc_alarm_irq_enable,
};

/*
 * Initialize and install RTC driver
 */
<<<<<<< HEAD
static int __devinit at91_rtc_probe(struct platform_device *pdev)
{
	struct resource	*r, *r_gpbr;
	struct sam9_rtc	*rtc;
	int		ret;
	u32		mr;

	r = platform_get_resource(pdev, IORESOURCE_MEM, 0);
	r_gpbr = platform_get_resource(pdev, IORESOURCE_MEM, 1);
	if (!r || !r_gpbr) {
		dev_err(&pdev->dev, "need 2 ressources\n");
		return -ENODEV;
	}

	rtc = kzalloc(sizeof *rtc, GFP_KERNEL);
	if (!rtc)
		return -ENOMEM;

=======
static int at91_rtc_probe(struct platform_device *pdev)
{
	struct sam9_rtc	*rtc;
	int		ret, irq;
	u32		mr;
	unsigned int	sclk_rate;
	struct of_phandle_args args;

	irq = platform_get_irq(pdev, 0);
	if (irq < 0)
		return irq;

	rtc = devm_kzalloc(&pdev->dev, sizeof(*rtc), GFP_KERNEL);
	if (!rtc)
		return -ENOMEM;

	spin_lock_init(&rtc->lock);
	rtc->irq = irq;

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	/* platform setup code should have handled this; sigh */
	if (!device_can_wakeup(&pdev->dev))
		device_init_wakeup(&pdev->dev, 1);

	platform_set_drvdata(pdev, rtc);
<<<<<<< HEAD
	rtc->rtt = ioremap(r->start, resource_size(r));
	if (!rtc->rtt) {
		dev_err(&pdev->dev, "failed to map registers, aborting.\n");
		ret = -ENOMEM;
		goto fail;
	}

	rtc->gpbr = ioremap(r_gpbr->start, resource_size(r_gpbr));
	if (!rtc->gpbr) {
		dev_err(&pdev->dev, "failed to map gpbr registers, aborting.\n");
		ret = -ENOMEM;
		goto fail_gpbr;
=======

	rtc->rtt = devm_platform_ioremap_resource(pdev, 0);
	if (IS_ERR(rtc->rtt))
		return PTR_ERR(rtc->rtt);

	ret = of_parse_phandle_with_fixed_args(pdev->dev.of_node,
					       "atmel,rtt-rtc-time-reg", 1, 0,
					       &args);
	if (ret)
		return ret;

	rtc->gpbr = syscon_node_to_regmap(args.np);
	rtc->gpbr_offset = args.args[0];
	if (IS_ERR(rtc->gpbr)) {
		dev_err(&pdev->dev, "failed to retrieve gpbr regmap, aborting.\n");
		return -ENOMEM;
	}

	rtc->sclk = devm_clk_get(&pdev->dev, NULL);
	if (IS_ERR(rtc->sclk))
		return PTR_ERR(rtc->sclk);

	ret = clk_prepare_enable(rtc->sclk);
	if (ret) {
		dev_err(&pdev->dev, "Could not enable slow clock\n");
		return ret;
	}

	sclk_rate = clk_get_rate(rtc->sclk);
	if (!sclk_rate || sclk_rate > AT91_RTT_RTPRES) {
		dev_err(&pdev->dev, "Invalid slow clock rate\n");
		ret = -EINVAL;
		goto err_clk;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	}

	mr = rtt_readl(rtc, MR);

	/* unless RTT is counting at 1 Hz, re-initialize it */
<<<<<<< HEAD
	if ((mr & AT91_RTT_RTPRES) != AT91_SLOW_CLOCK) {
		mr = AT91_RTT_RTTRST | (AT91_SLOW_CLOCK & AT91_RTT_RTPRES);
=======
	if ((mr & AT91_RTT_RTPRES) != sclk_rate) {
		mr = AT91_RTT_RTTRST | (sclk_rate & AT91_RTT_RTPRES);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		gpbr_writel(rtc, 0);
	}

	/* disable all interrupts (same as on shutdown path) */
	mr &= ~(AT91_RTT_ALMIEN | AT91_RTT_RTTINCIEN);
	rtt_writel(rtc, MR, mr);

<<<<<<< HEAD
	rtc->rtcdev = rtc_device_register(pdev->name, &pdev->dev,
				&at91_rtc_ops, THIS_MODULE);
	if (IS_ERR(rtc->rtcdev)) {
		ret = PTR_ERR(rtc->rtcdev);
		goto fail_register;
	}

	/* register irq handler after we know what name we'll use */
	ret = request_irq(AT91_ID_SYS, at91_rtc_interrupt,
				IRQF_SHARED,
				dev_name(&rtc->rtcdev->dev), rtc);
	if (ret) {
		dev_dbg(&pdev->dev, "can't share IRQ %d?\n", AT91_ID_SYS);
		rtc_device_unregister(rtc->rtcdev);
		goto fail_register;
=======
	rtc->rtcdev = devm_rtc_allocate_device(&pdev->dev);
	if (IS_ERR(rtc->rtcdev)) {
		ret = PTR_ERR(rtc->rtcdev);
		goto err_clk;
	}

	rtc->rtcdev->ops = &at91_rtc_ops;
	rtc->rtcdev->range_max = U32_MAX;

	/* register irq handler after we know what name we'll use */
	ret = devm_request_irq(&pdev->dev, rtc->irq, at91_rtc_interrupt,
			       IRQF_SHARED | IRQF_COND_SUSPEND,
			       dev_name(&rtc->rtcdev->dev), rtc);
	if (ret) {
		dev_dbg(&pdev->dev, "can't share IRQ %d?\n", rtc->irq);
		goto err_clk;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	}

	/* NOTE:  sam9260 rev A silicon has a ROM bug which resets the
	 * RTT on at least some reboots.  If you have that chip, you must
	 * initialize the time from some external source like a GPS, wall
	 * clock, discrete RTC, etc
	 */

	if (gpbr_readl(rtc) == 0)
		dev_warn(&pdev->dev, "%s: SET TIME!\n",
<<<<<<< HEAD
				dev_name(&rtc->rtcdev->dev));

	return 0;

fail_register:
	iounmap(rtc->gpbr);
fail_gpbr:
	iounmap(rtc->rtt);
fail:
	platform_set_drvdata(pdev, NULL);
	kfree(rtc);
=======
			 dev_name(&rtc->rtcdev->dev));

	return devm_rtc_register_device(rtc->rtcdev);

err_clk:
	clk_disable_unprepare(rtc->sclk);

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	return ret;
}

/*
 * Disable and remove the RTC driver
 */
<<<<<<< HEAD
static int __devexit at91_rtc_remove(struct platform_device *pdev)
=======
static void at91_rtc_remove(struct platform_device *pdev)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
{
	struct sam9_rtc	*rtc = platform_get_drvdata(pdev);
	u32		mr = rtt_readl(rtc, MR);

	/* disable all interrupts */
	rtt_writel(rtc, MR, mr & ~(AT91_RTT_ALMIEN | AT91_RTT_RTTINCIEN));
<<<<<<< HEAD
	free_irq(AT91_ID_SYS, rtc);

	rtc_device_unregister(rtc->rtcdev);

	iounmap(rtc->gpbr);
	iounmap(rtc->rtt);
	platform_set_drvdata(pdev, NULL);
	kfree(rtc);
	return 0;
=======

	clk_disable_unprepare(rtc->sclk);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

static void at91_rtc_shutdown(struct platform_device *pdev)
{
	struct sam9_rtc	*rtc = platform_get_drvdata(pdev);
	u32		mr = rtt_readl(rtc, MR);

	rtc->imr = mr & (AT91_RTT_ALMIEN | AT91_RTT_RTTINCIEN);
	rtt_writel(rtc, MR, mr & ~rtc->imr);
}

<<<<<<< HEAD
#ifdef CONFIG_PM

/* AT91SAM9 RTC Power management control */

static int at91_rtc_suspend(struct platform_device *pdev,
					pm_message_t state)
{
	struct sam9_rtc	*rtc = platform_get_drvdata(pdev);
=======
#ifdef CONFIG_PM_SLEEP

/* AT91SAM9 RTC Power management control */

static int at91_rtc_suspend(struct device *dev)
{
	struct sam9_rtc	*rtc = dev_get_drvdata(dev);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	u32		mr = rtt_readl(rtc, MR);

	/*
	 * This IRQ is shared with DBGU and other hardware which isn't
	 * necessarily a wakeup event source.
	 */
	rtc->imr = mr & (AT91_RTT_ALMIEN | AT91_RTT_RTTINCIEN);
	if (rtc->imr) {
<<<<<<< HEAD
		if (device_may_wakeup(&pdev->dev) && (mr & AT91_RTT_ALMIEN)) {
			enable_irq_wake(AT91_ID_SYS);
			/* don't let RTTINC cause wakeups */
			if (mr & AT91_RTT_RTTINCIEN)
				rtt_writel(rtc, MR, mr & ~AT91_RTT_RTTINCIEN);
		} else
			rtt_writel(rtc, MR, mr & ~rtc->imr);
=======
		if (device_may_wakeup(dev) && (mr & AT91_RTT_ALMIEN)) {
			unsigned long flags;

			enable_irq_wake(rtc->irq);
			spin_lock_irqsave(&rtc->lock, flags);
			rtc->suspended = true;
			spin_unlock_irqrestore(&rtc->lock, flags);
			/* don't let RTTINC cause wakeups */
			if (mr & AT91_RTT_RTTINCIEN)
				rtt_writel(rtc, MR, mr & ~AT91_RTT_RTTINCIEN);
		} else {
			rtt_writel(rtc, MR, mr & ~rtc->imr);
		}
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	}

	return 0;
}

<<<<<<< HEAD
static int at91_rtc_resume(struct platform_device *pdev)
{
	struct sam9_rtc	*rtc = platform_get_drvdata(pdev);
	u32		mr;

	if (rtc->imr) {
		if (device_may_wakeup(&pdev->dev))
			disable_irq_wake(AT91_ID_SYS);
		mr = rtt_readl(rtc, MR);
		rtt_writel(rtc, MR, mr | rtc->imr);
=======
static int at91_rtc_resume(struct device *dev)
{
	struct sam9_rtc	*rtc = dev_get_drvdata(dev);
	u32		mr;

	if (rtc->imr) {
		unsigned long flags;

		if (device_may_wakeup(dev))
			disable_irq_wake(rtc->irq);
		mr = rtt_readl(rtc, MR);
		rtt_writel(rtc, MR, mr | rtc->imr);

		spin_lock_irqsave(&rtc->lock, flags);
		rtc->suspended = false;
		at91_rtc_cache_events(rtc);
		at91_rtc_flush_events(rtc);
		spin_unlock_irqrestore(&rtc->lock, flags);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	}

	return 0;
}
<<<<<<< HEAD
#else
#define at91_rtc_suspend	NULL
#define at91_rtc_resume		NULL
#endif

static struct platform_driver at91_rtc_driver = {
	.probe		= at91_rtc_probe,
	.remove		= __devexit_p(at91_rtc_remove),
	.shutdown	= at91_rtc_shutdown,
	.suspend	= at91_rtc_suspend,
	.resume		= at91_rtc_resume,
	.driver		= {
		.name	= "rtc-at91sam9",
		.owner	= THIS_MODULE,
	},
};

static int __init at91_rtc_init(void)
{
	return platform_driver_register(&at91_rtc_driver);
}
module_init(at91_rtc_init);

static void __exit at91_rtc_exit(void)
{
	platform_driver_unregister(&at91_rtc_driver);
}
module_exit(at91_rtc_exit);

=======
#endif

static SIMPLE_DEV_PM_OPS(at91_rtc_pm_ops, at91_rtc_suspend, at91_rtc_resume);

static const struct of_device_id at91_rtc_dt_ids[] = {
	{ .compatible = "atmel,at91sam9260-rtt" },
	{ /* sentinel */ }
};
MODULE_DEVICE_TABLE(of, at91_rtc_dt_ids);

static struct platform_driver at91_rtc_driver = {
	.probe		= at91_rtc_probe,
	.remove_new	= at91_rtc_remove,
	.shutdown	= at91_rtc_shutdown,
	.driver		= {
		.name	= "rtc-at91sam9",
		.pm	= &at91_rtc_pm_ops,
		.of_match_table = at91_rtc_dt_ids,
	},
};

module_platform_driver(at91_rtc_driver);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

MODULE_AUTHOR("Michel Benoit");
MODULE_DESCRIPTION("RTC driver for Atmel AT91SAM9x");
MODULE_LICENSE("GPL");
