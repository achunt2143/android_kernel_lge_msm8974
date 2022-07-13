<<<<<<< HEAD
=======
// SPDX-License-Identifier: GPL-2.0
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
/*
 * RTC subsystem, interface functions
 *
 * Copyright (C) 2005 Tower Technologies
 * Author: Alessandro Zummo <a.zummo@towertech.it>
 *
 * based on arch/arm/common/rtctime.c
<<<<<<< HEAD
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
*/
=======
 */
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

#include <linux/rtc.h>
#include <linux/sched.h>
#include <linux/module.h>
#include <linux/log2.h>
#include <linux/workqueue.h>

<<<<<<< HEAD
static int rtc_timer_enqueue(struct rtc_device *rtc, struct rtc_timer *timer);
static void rtc_timer_remove(struct rtc_device *rtc, struct rtc_timer *timer);

static int __rtc_read_time(struct rtc_device *rtc, struct rtc_time *tm)
{
	int err;
	if (!rtc->ops)
		err = -ENODEV;
	else if (!rtc->ops->read_time)
		err = -EINVAL;
	else {
		memset(tm, 0, sizeof(struct rtc_time));
		err = rtc->ops->read_time(rtc->dev.parent, tm);
=======
#define CREATE_TRACE_POINTS
#include <trace/events/rtc.h>

static int rtc_timer_enqueue(struct rtc_device *rtc, struct rtc_timer *timer);
static void rtc_timer_remove(struct rtc_device *rtc, struct rtc_timer *timer);

static void rtc_add_offset(struct rtc_device *rtc, struct rtc_time *tm)
{
	time64_t secs;

	if (!rtc->offset_secs)
		return;

	secs = rtc_tm_to_time64(tm);

	/*
	 * Since the reading time values from RTC device are always in the RTC
	 * original valid range, but we need to skip the overlapped region
	 * between expanded range and original range, which is no need to add
	 * the offset.
	 */
	if ((rtc->start_secs > rtc->range_min && secs >= rtc->start_secs) ||
	    (rtc->start_secs < rtc->range_min &&
	     secs <= (rtc->start_secs + rtc->range_max - rtc->range_min)))
		return;

	rtc_time64_to_tm(secs + rtc->offset_secs, tm);
}

static void rtc_subtract_offset(struct rtc_device *rtc, struct rtc_time *tm)
{
	time64_t secs;

	if (!rtc->offset_secs)
		return;

	secs = rtc_tm_to_time64(tm);

	/*
	 * If the setting time values are in the valid range of RTC hardware
	 * device, then no need to subtract the offset when setting time to RTC
	 * device. Otherwise we need to subtract the offset to make the time
	 * values are valid for RTC hardware device.
	 */
	if (secs >= rtc->range_min && secs <= rtc->range_max)
		return;

	rtc_time64_to_tm(secs - rtc->offset_secs, tm);
}

static int rtc_valid_range(struct rtc_device *rtc, struct rtc_time *tm)
{
	if (rtc->range_min != rtc->range_max) {
		time64_t time = rtc_tm_to_time64(tm);
		time64_t range_min = rtc->set_start_time ? rtc->start_secs :
			rtc->range_min;
		timeu64_t range_max = rtc->set_start_time ?
			(rtc->start_secs + rtc->range_max - rtc->range_min) :
			rtc->range_max;

		if (time < range_min || time > range_max)
			return -ERANGE;
	}

	return 0;
}

static int __rtc_read_time(struct rtc_device *rtc, struct rtc_time *tm)
{
	int err;

	if (!rtc->ops) {
		err = -ENODEV;
	} else if (!rtc->ops->read_time) {
		err = -EINVAL;
	} else {
		memset(tm, 0, sizeof(struct rtc_time));
		err = rtc->ops->read_time(rtc->dev.parent, tm);
		if (err < 0) {
			dev_dbg(&rtc->dev, "read_time: fail to read: %d\n",
				err);
			return err;
		}

		rtc_add_offset(rtc, tm);

		err = rtc_valid_tm(tm);
		if (err < 0)
			dev_dbg(&rtc->dev, "read_time: rtc_time isn't valid\n");
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	}
	return err;
}

int rtc_read_time(struct rtc_device *rtc, struct rtc_time *tm)
{
	int err;

	err = mutex_lock_interruptible(&rtc->ops_lock);
	if (err)
		return err;

	err = __rtc_read_time(rtc, tm);
	mutex_unlock(&rtc->ops_lock);
<<<<<<< HEAD
=======

	trace_rtc_read_time(rtc_tm_to_time64(tm), err);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	return err;
}
EXPORT_SYMBOL_GPL(rtc_read_time);

int rtc_set_time(struct rtc_device *rtc, struct rtc_time *tm)
{
<<<<<<< HEAD
	int err;
=======
	int err, uie;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	err = rtc_valid_tm(tm);
	if (err != 0)
		return err;

<<<<<<< HEAD
=======
	err = rtc_valid_range(rtc, tm);
	if (err)
		return err;

	rtc_subtract_offset(rtc, tm);

#ifdef CONFIG_RTC_INTF_DEV_UIE_EMUL
	uie = rtc->uie_rtctimer.enabled || rtc->uie_irq_active;
#else
	uie = rtc->uie_rtctimer.enabled;
#endif
	if (uie) {
		err = rtc_update_irq_enable(rtc, 0);
		if (err)
			return err;
	}

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	err = mutex_lock_interruptible(&rtc->ops_lock);
	if (err)
		return err;

	if (!rtc->ops)
		err = -ENODEV;
	else if (rtc->ops->set_time)
		err = rtc->ops->set_time(rtc->dev.parent, tm);
<<<<<<< HEAD
	else if (rtc->ops->set_mmss) {
		unsigned long secs;
		err = rtc_tm_to_time(tm, &secs);
		if (err == 0)
			err = rtc->ops->set_mmss(rtc->dev.parent, secs);
	} else
		err = -EINVAL;

	mutex_unlock(&rtc->ops_lock);
	/* A timer might have just expired */
	schedule_work(&rtc->irqwork);
=======
	else
		err = -EINVAL;

	pm_stay_awake(rtc->dev.parent);
	mutex_unlock(&rtc->ops_lock);
	/* A timer might have just expired */
	schedule_work(&rtc->irqwork);

	if (uie) {
		err = rtc_update_irq_enable(rtc, 1);
		if (err)
			return err;
	}

	trace_rtc_set_time(rtc_tm_to_time64(tm), err);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	return err;
}
EXPORT_SYMBOL_GPL(rtc_set_time);

<<<<<<< HEAD
int rtc_set_mmss(struct rtc_device *rtc, unsigned long secs)
=======
static int rtc_read_alarm_internal(struct rtc_device *rtc,
				   struct rtc_wkalrm *alarm)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
{
	int err;

	err = mutex_lock_interruptible(&rtc->ops_lock);
	if (err)
		return err;

<<<<<<< HEAD
	if (!rtc->ops)
		err = -ENODEV;
	else if (rtc->ops->set_mmss)
		err = rtc->ops->set_mmss(rtc->dev.parent, secs);
	else if (rtc->ops->read_time && rtc->ops->set_time) {
		struct rtc_time new, old;

		err = rtc->ops->read_time(rtc->dev.parent, &old);
		if (err == 0) {
			rtc_time_to_tm(secs, &new);

			/*
			 * avoid writing when we're going to change the day of
			 * the month. We will retry in the next minute. This
			 * basically means that if the RTC must not drift
			 * by more than 1 minute in 11 minutes.
			 */
			if (!((old.tm_hour == 23 && old.tm_min == 59) ||
				(new.tm_hour == 23 && new.tm_min == 59)))
				err = rtc->ops->set_time(rtc->dev.parent,
						&new);
		}
	}
	else
		err = -EINVAL;

	mutex_unlock(&rtc->ops_lock);
	/* A timer might have just expired */
	schedule_work(&rtc->irqwork);

	return err;
}
EXPORT_SYMBOL_GPL(rtc_set_mmss);

static int rtc_read_alarm_internal(struct rtc_device *rtc, struct rtc_wkalrm *alarm)
{
	int err;

	err = mutex_lock_interruptible(&rtc->ops_lock);
	if (err)
		return err;

	if (rtc->ops == NULL)
		err = -ENODEV;
	else if (!rtc->ops->read_alarm)
		err = -EINVAL;
	else {
		memset(alarm, 0, sizeof(struct rtc_wkalrm));
=======
	if (!rtc->ops) {
		err = -ENODEV;
	} else if (!test_bit(RTC_FEATURE_ALARM, rtc->features) || !rtc->ops->read_alarm) {
		err = -EINVAL;
	} else {
		alarm->enabled = 0;
		alarm->pending = 0;
		alarm->time.tm_sec = -1;
		alarm->time.tm_min = -1;
		alarm->time.tm_hour = -1;
		alarm->time.tm_mday = -1;
		alarm->time.tm_mon = -1;
		alarm->time.tm_year = -1;
		alarm->time.tm_wday = -1;
		alarm->time.tm_yday = -1;
		alarm->time.tm_isdst = -1;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		err = rtc->ops->read_alarm(rtc->dev.parent, alarm);
	}

	mutex_unlock(&rtc->ops_lock);
<<<<<<< HEAD
=======

	trace_rtc_read_alarm(rtc_tm_to_time64(&alarm->time), err);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	return err;
}

int __rtc_read_alarm(struct rtc_device *rtc, struct rtc_wkalrm *alarm)
{
	int err;
	struct rtc_time before, now;
	int first_time = 1;
<<<<<<< HEAD
	unsigned long t_now, t_alm;
	enum { none, day, month, year } missing = none;
	unsigned days;
=======
	time64_t t_now, t_alm;
	enum { none, day, month, year } missing = none;
	unsigned int days;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	/* The lower level RTC driver may return -1 in some fields,
	 * creating invalid alarm->time values, for reasons like:
	 *
	 *   - The hardware may not be capable of filling them in;
	 *     many alarms match only on time-of-day fields, not
	 *     day/month/year calendar data.
	 *
	 *   - Some hardware uses illegal values as "wildcard" match
	 *     values, which non-Linux firmware (like a BIOS) may try
	 *     to set up as e.g. "alarm 15 minutes after each hour".
	 *     Linux uses only oneshot alarms.
	 *
	 * When we see that here, we deal with it by using values from
	 * a current RTC timestamp for any missing (-1) values.  The
	 * RTC driver prevents "periodic alarm" modes.
	 *
	 * But this can be racey, because some fields of the RTC timestamp
	 * may have wrapped in the interval since we read the RTC alarm,
	 * which would lead to us inserting inconsistent values in place
	 * of the -1 fields.
	 *
	 * Reading the alarm and timestamp in the reverse sequence
	 * would have the same race condition, and not solve the issue.
	 *
	 * So, we must first read the RTC timestamp,
	 * then read the RTC alarm value,
	 * and then read a second RTC timestamp.
	 *
	 * If any fields of the second timestamp have changed
	 * when compared with the first timestamp, then we know
	 * our timestamp may be inconsistent with that used by
	 * the low-level rtc_read_alarm_internal() function.
	 *
	 * So, when the two timestamps disagree, we just loop and do
	 * the process again to get a fully consistent set of values.
	 *
	 * This could all instead be done in the lower level driver,
	 * but since more than one lower level RTC implementation needs it,
<<<<<<< HEAD
	 * then it's probably best best to do it here instead of there..
=======
	 * then it's probably best to do it here instead of there..
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	 */

	/* Get the "before" timestamp */
	err = rtc_read_time(rtc, &before);
	if (err < 0)
		return err;
	do {
		if (!first_time)
			memcpy(&before, &now, sizeof(struct rtc_time));
		first_time = 0;

		/* get the RTC alarm values, which may be incomplete */
		err = rtc_read_alarm_internal(rtc, alarm);
		if (err)
			return err;

		/* full-function RTCs won't have such missing fields */
<<<<<<< HEAD
		if (rtc_valid_tm(&alarm->time) == 0)
			return 0;
=======
		if (rtc_valid_tm(&alarm->time) == 0) {
			rtc_add_offset(rtc, &alarm->time);
			return 0;
		}
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

		/* get the "after" timestamp, to detect wrapped fields */
		err = rtc_read_time(rtc, &now);
		if (err < 0)
			return err;

		/* note that tm_sec is a "don't care" value here: */
<<<<<<< HEAD
	} while (   before.tm_min   != now.tm_min
		 || before.tm_hour  != now.tm_hour
		 || before.tm_mon   != now.tm_mon
		 || before.tm_year  != now.tm_year);
=======
	} while (before.tm_min  != now.tm_min ||
		 before.tm_hour != now.tm_hour ||
		 before.tm_mon  != now.tm_mon ||
		 before.tm_year != now.tm_year);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	/* Fill in the missing alarm fields using the timestamp; we
	 * know there's at least one since alarm->time is invalid.
	 */
	if (alarm->time.tm_sec == -1)
		alarm->time.tm_sec = now.tm_sec;
	if (alarm->time.tm_min == -1)
		alarm->time.tm_min = now.tm_min;
	if (alarm->time.tm_hour == -1)
		alarm->time.tm_hour = now.tm_hour;

	/* For simplicity, only support date rollover for now */
	if (alarm->time.tm_mday < 1 || alarm->time.tm_mday > 31) {
		alarm->time.tm_mday = now.tm_mday;
		missing = day;
	}
<<<<<<< HEAD
	if ((unsigned)alarm->time.tm_mon >= 12) {
=======
	if ((unsigned int)alarm->time.tm_mon >= 12) {
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		alarm->time.tm_mon = now.tm_mon;
		if (missing == none)
			missing = month;
	}
	if (alarm->time.tm_year == -1) {
		alarm->time.tm_year = now.tm_year;
		if (missing == none)
			missing = year;
	}

<<<<<<< HEAD
	/* with luck, no rollover is needed */
	rtc_tm_to_time(&now, &t_now);
	rtc_tm_to_time(&alarm->time, &t_alm);
=======
	/* Can't proceed if alarm is still invalid after replacing
	 * missing fields.
	 */
	err = rtc_valid_tm(&alarm->time);
	if (err)
		goto done;

	/* with luck, no rollover is needed */
	t_now = rtc_tm_to_time64(&now);
	t_alm = rtc_tm_to_time64(&alarm->time);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	if (t_now < t_alm)
		goto done;

	switch (missing) {
<<<<<<< HEAD

=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	/* 24 hour rollover ... if it's now 10am Monday, an alarm that
	 * that will trigger at 5am will do so at 5am Tuesday, which
	 * could also be in the next month or year.  This is a common
	 * case, especially for PCs.
	 */
	case day:
		dev_dbg(&rtc->dev, "alarm rollover: %s\n", "day");
		t_alm += 24 * 60 * 60;
<<<<<<< HEAD
		rtc_time_to_tm(t_alm, &alarm->time);
=======
		rtc_time64_to_tm(t_alm, &alarm->time);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		break;

	/* Month rollover ... if it's the 31th, an alarm on the 3rd will
	 * be next month.  An alarm matching on the 30th, 29th, or 28th
	 * may end up in the month after that!  Many newer PCs support
	 * this type of alarm.
	 */
	case month:
		dev_dbg(&rtc->dev, "alarm rollover: %s\n", "month");
		do {
<<<<<<< HEAD
			if (alarm->time.tm_mon < 11)
				alarm->time.tm_mon++;
			else {
=======
			if (alarm->time.tm_mon < 11) {
				alarm->time.tm_mon++;
			} else {
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
				alarm->time.tm_mon = 0;
				alarm->time.tm_year++;
			}
			days = rtc_month_days(alarm->time.tm_mon,
<<<<<<< HEAD
					alarm->time.tm_year);
=======
					      alarm->time.tm_year);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		} while (days < alarm->time.tm_mday);
		break;

	/* Year rollover ... easy except for leap years! */
	case year:
		dev_dbg(&rtc->dev, "alarm rollover: %s\n", "year");
		do {
			alarm->time.tm_year++;
<<<<<<< HEAD
		} while (rtc_valid_tm(&alarm->time) != 0);
=======
		} while (!is_leap_year(alarm->time.tm_year + 1900) &&
			 rtc_valid_tm(&alarm->time) != 0);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		break;

	default:
		dev_warn(&rtc->dev, "alarm rollover not handled\n");
	}

<<<<<<< HEAD
done:
	return 0;
=======
	err = rtc_valid_tm(&alarm->time);

done:
	if (err && alarm->enabled)
		dev_warn(&rtc->dev, "invalid alarm value: %ptR\n",
			 &alarm->time);

	return err;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

int rtc_read_alarm(struct rtc_device *rtc, struct rtc_wkalrm *alarm)
{
	int err;

	err = mutex_lock_interruptible(&rtc->ops_lock);
	if (err)
		return err;
<<<<<<< HEAD
	if (rtc->ops == NULL)
		err = -ENODEV;
	else if (!rtc->ops->read_alarm)
		err = -EINVAL;
	else {
=======
	if (!rtc->ops) {
		err = -ENODEV;
	} else if (!test_bit(RTC_FEATURE_ALARM, rtc->features)) {
		err = -EINVAL;
	} else {
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		memset(alarm, 0, sizeof(struct rtc_wkalrm));
		alarm->enabled = rtc->aie_timer.enabled;
		alarm->time = rtc_ktime_to_tm(rtc->aie_timer.node.expires);
	}
	mutex_unlock(&rtc->ops_lock);

<<<<<<< HEAD
=======
	trace_rtc_read_alarm(rtc_tm_to_time64(&alarm->time), err);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	return err;
}
EXPORT_SYMBOL_GPL(rtc_read_alarm);

static int __rtc_set_alarm(struct rtc_device *rtc, struct rtc_wkalrm *alarm)
{
	struct rtc_time tm;
<<<<<<< HEAD
	long now, scheduled;
=======
	time64_t now, scheduled;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	int err;

	err = rtc_valid_tm(&alarm->time);
	if (err)
		return err;
<<<<<<< HEAD
	rtc_tm_to_time(&alarm->time, &scheduled);

	/* Make sure we're not setting alarms in the past */
	err = __rtc_read_time(rtc, &tm);
	rtc_tm_to_time(&tm, &now);
=======

	scheduled = rtc_tm_to_time64(&alarm->time);

	/* Make sure we're not setting alarms in the past */
	err = __rtc_read_time(rtc, &tm);
	if (err)
		return err;
	now = rtc_tm_to_time64(&tm);

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	if (scheduled <= now)
		return -ETIME;
	/*
	 * XXX - We just checked to make sure the alarm time is not
	 * in the past, but there is still a race window where if
	 * the is alarm set for the next second and the second ticks
	 * over right here, before we set the alarm.
	 */

<<<<<<< HEAD
	if (!rtc->ops)
		err = -ENODEV;
	else if (!rtc->ops->set_alarm)
=======
	rtc_subtract_offset(rtc, &alarm->time);

	if (!rtc->ops)
		err = -ENODEV;
	else if (!test_bit(RTC_FEATURE_ALARM, rtc->features))
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		err = -EINVAL;
	else
		err = rtc->ops->set_alarm(rtc->dev.parent, alarm);

<<<<<<< HEAD
=======
	trace_rtc_set_alarm(rtc_tm_to_time64(&alarm->time), err);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	return err;
}

int rtc_set_alarm(struct rtc_device *rtc, struct rtc_wkalrm *alarm)
{
<<<<<<< HEAD
	int err;

=======
	ktime_t alarm_time;
	int err;

	if (!rtc->ops)
		return -ENODEV;
	else if (!test_bit(RTC_FEATURE_ALARM, rtc->features))
		return -EINVAL;

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	err = rtc_valid_tm(&alarm->time);
	if (err != 0)
		return err;

<<<<<<< HEAD
	err = mutex_lock_interruptible(&rtc->ops_lock);
	if (err)
		return err;
	if (rtc->aie_timer.enabled) {
		rtc_timer_remove(rtc, &rtc->aie_timer);
	}
	rtc->aie_timer.node.expires = rtc_tm_to_ktime(alarm->time);
	rtc->aie_timer.period = ktime_set(0, 0);
	if (alarm->enabled) {
		err = rtc_timer_enqueue(rtc, &rtc->aie_timer);
	}
	mutex_unlock(&rtc->ops_lock);
=======
	err = rtc_valid_range(rtc, &alarm->time);
	if (err)
		return err;

	err = mutex_lock_interruptible(&rtc->ops_lock);
	if (err)
		return err;
	if (rtc->aie_timer.enabled)
		rtc_timer_remove(rtc, &rtc->aie_timer);

	alarm_time = rtc_tm_to_ktime(alarm->time);
	/*
	 * Round down so we never miss a deadline, checking for past deadline is
	 * done in __rtc_set_alarm
	 */
	if (test_bit(RTC_FEATURE_ALARM_RES_MINUTE, rtc->features))
		alarm_time = ktime_sub_ns(alarm_time, (u64)alarm->time.tm_sec * NSEC_PER_SEC);

	rtc->aie_timer.node.expires = alarm_time;
	rtc->aie_timer.period = 0;
	if (alarm->enabled)
		err = rtc_timer_enqueue(rtc, &rtc->aie_timer);

	mutex_unlock(&rtc->ops_lock);

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	return err;
}
EXPORT_SYMBOL_GPL(rtc_set_alarm);

<<<<<<< HEAD
#ifdef CONFIG_LGE_PM_RTC_PWROFF_ALARM
int rtc_set_po_alarm(struct rtc_device *rtc, struct rtc_wkalrm *alarm)
{
	int err;

	err = rtc_valid_tm(&alarm->time);

	if (err != 0){
		return err;
	}

	if (!rtc->ops) {
		dev_err(&rtc->dev, "[%s %d] ops not exist\n", __func__, __LINE__);
		err = -ENODEV;
	} else if (!rtc->ops->set_po_alarm) {
		dev_err(&rtc->dev, "[%s %d] bootalarm func not exist\n", __func__, __LINE__);
		err = -EINVAL;
	} else
		err = rtc->ops->set_po_alarm(rtc->dev.parent, alarm);

	pr_info("[%s %d] Set Power Off Alarm\n", __func__, __LINE__);

	return err;
}
EXPORT_SYMBOL_GPL(rtc_set_po_alarm);
#endif

=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
/* Called once per device from rtc_device_register */
int rtc_initialize_alarm(struct rtc_device *rtc, struct rtc_wkalrm *alarm)
{
	int err;
	struct rtc_time now;

	err = rtc_valid_tm(&alarm->time);
	if (err != 0)
		return err;

	err = rtc_read_time(rtc, &now);
	if (err)
		return err;

	err = mutex_lock_interruptible(&rtc->ops_lock);
	if (err)
		return err;

	rtc->aie_timer.node.expires = rtc_tm_to_ktime(alarm->time);
<<<<<<< HEAD
	rtc->aie_timer.period = ktime_set(0, 0);

	/* Alarm has to be enabled & in the futrure for us to enqueue it */
	if (alarm->enabled && (rtc_tm_to_ktime(now).tv64 <
			 rtc->aie_timer.node.expires.tv64)) {

		rtc->aie_timer.enabled = 1;
		timerqueue_add(&rtc->timerqueue, &rtc->aie_timer.node);
=======
	rtc->aie_timer.period = 0;

	/* Alarm has to be enabled & in the future for us to enqueue it */
	if (alarm->enabled && (rtc_tm_to_ktime(now) <
			 rtc->aie_timer.node.expires)) {
		rtc->aie_timer.enabled = 1;
		timerqueue_add(&rtc->timerqueue, &rtc->aie_timer.node);
		trace_rtc_timer_enqueue(&rtc->aie_timer);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	}
	mutex_unlock(&rtc->ops_lock);
	return err;
}
EXPORT_SYMBOL_GPL(rtc_initialize_alarm);

<<<<<<< HEAD


int rtc_alarm_irq_enable(struct rtc_device *rtc, unsigned int enabled)
{
	int err = mutex_lock_interruptible(&rtc->ops_lock);
=======
int rtc_alarm_irq_enable(struct rtc_device *rtc, unsigned int enabled)
{
	int err;

	err = mutex_lock_interruptible(&rtc->ops_lock);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	if (err)
		return err;

	if (rtc->aie_timer.enabled != enabled) {
		if (enabled)
			err = rtc_timer_enqueue(rtc, &rtc->aie_timer);
		else
			rtc_timer_remove(rtc, &rtc->aie_timer);
	}

	if (err)
		/* nothing */;
	else if (!rtc->ops)
		err = -ENODEV;
<<<<<<< HEAD
	else if (!rtc->ops->alarm_irq_enable)
=======
	else if (!test_bit(RTC_FEATURE_ALARM, rtc->features) || !rtc->ops->alarm_irq_enable)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		err = -EINVAL;
	else
		err = rtc->ops->alarm_irq_enable(rtc->dev.parent, enabled);

	mutex_unlock(&rtc->ops_lock);
<<<<<<< HEAD
=======

	trace_rtc_alarm_irq_enable(enabled, err);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	return err;
}
EXPORT_SYMBOL_GPL(rtc_alarm_irq_enable);

int rtc_update_irq_enable(struct rtc_device *rtc, unsigned int enabled)
{
<<<<<<< HEAD
	int err = mutex_lock_interruptible(&rtc->ops_lock);
=======
	int err;

	err = mutex_lock_interruptible(&rtc->ops_lock);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	if (err)
		return err;

#ifdef CONFIG_RTC_INTF_DEV_UIE_EMUL
	if (enabled == 0 && rtc->uie_irq_active) {
		mutex_unlock(&rtc->ops_lock);
		return rtc_dev_update_irq_enable_emul(rtc, 0);
	}
#endif
	/* make sure we're changing state */
	if (rtc->uie_rtctimer.enabled == enabled)
		goto out;

<<<<<<< HEAD
	if (rtc->uie_unsupported) {
		err = -EINVAL;
		goto out;
=======
	if (!test_bit(RTC_FEATURE_UPDATE_INTERRUPT, rtc->features) ||
	    !test_bit(RTC_FEATURE_ALARM, rtc->features)) {
		mutex_unlock(&rtc->ops_lock);
#ifdef CONFIG_RTC_INTF_DEV_UIE_EMUL
		return rtc_dev_update_irq_enable_emul(rtc, enabled);
#else
		return -EINVAL;
#endif
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	}

	if (enabled) {
		struct rtc_time tm;
		ktime_t now, onesec;

<<<<<<< HEAD
		__rtc_read_time(rtc, &tm);
=======
		err = __rtc_read_time(rtc, &tm);
		if (err)
			goto out;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		onesec = ktime_set(1, 0);
		now = rtc_tm_to_ktime(tm);
		rtc->uie_rtctimer.node.expires = ktime_add(now, onesec);
		rtc->uie_rtctimer.period = ktime_set(1, 0);
		err = rtc_timer_enqueue(rtc, &rtc->uie_rtctimer);
<<<<<<< HEAD
	} else
		rtc_timer_remove(rtc, &rtc->uie_rtctimer);

out:
	mutex_unlock(&rtc->ops_lock);
#ifdef CONFIG_RTC_INTF_DEV_UIE_EMUL
	/*
	 * Enable emulation if the driver did not provide
	 * the update_irq_enable function pointer or if returned
	 * -EINVAL to signal that it has been configured without
	 * interrupts or that are not available at the moment.
	 */
	if (err == -EINVAL)
		err = rtc_dev_update_irq_enable_emul(rtc, enabled);
#endif
	return err;

}
EXPORT_SYMBOL_GPL(rtc_update_irq_enable);


/**
 * rtc_handle_legacy_irq - AIE, UIE and PIE event hook
 * @rtc: pointer to the rtc device
=======
	} else {
		rtc_timer_remove(rtc, &rtc->uie_rtctimer);
	}

out:
	mutex_unlock(&rtc->ops_lock);

	return err;
}
EXPORT_SYMBOL_GPL(rtc_update_irq_enable);

/**
 * rtc_handle_legacy_irq - AIE, UIE and PIE event hook
 * @rtc: pointer to the rtc device
 * @num: number of occurence of the event
 * @mode: type of the event, RTC_AF, RTC_UF of RTC_PF
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 *
 * This function is called when an AIE, UIE or PIE mode interrupt
 * has occurred (or been emulated).
 *
<<<<<<< HEAD
 * Triggers the registered irq_task function callback.
=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 */
void rtc_handle_legacy_irq(struct rtc_device *rtc, int num, int mode)
{
	unsigned long flags;

	/* mark one irq of the appropriate mode */
	spin_lock_irqsave(&rtc->irq_lock, flags);
<<<<<<< HEAD
	rtc->irq_data = (rtc->irq_data + (num << 8)) | (RTC_IRQF|mode);
	spin_unlock_irqrestore(&rtc->irq_lock, flags);

	/* call the task func */
	spin_lock_irqsave(&rtc->irq_task_lock, flags);
	if (rtc->irq_task)
		rtc->irq_task->func(rtc->irq_task->private_data);
	spin_unlock_irqrestore(&rtc->irq_task_lock, flags);

=======
	rtc->irq_data = (rtc->irq_data + (num << 8)) | (RTC_IRQF | mode);
	spin_unlock_irqrestore(&rtc->irq_lock, flags);

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	wake_up_interruptible(&rtc->irq_queue);
	kill_fasync(&rtc->async_queue, SIGIO, POLL_IN);
}

<<<<<<< HEAD

/**
 * rtc_aie_update_irq - AIE mode rtctimer hook
 * @private: pointer to the rtc_device
 *
 * This functions is called when the aie_timer expires.
 */
void rtc_aie_update_irq(void *private)
{
	struct rtc_device *rtc = (struct rtc_device *)private;
	rtc_handle_legacy_irq(rtc, 1, RTC_AF);
}


/**
 * rtc_uie_update_irq - UIE mode rtctimer hook
 * @private: pointer to the rtc_device
 *
 * This functions is called when the uie_timer expires.
 */
void rtc_uie_update_irq(void *private)
{
	struct rtc_device *rtc = (struct rtc_device *)private;
	rtc_handle_legacy_irq(rtc, 1,  RTC_UF);
}


=======
/**
 * rtc_aie_update_irq - AIE mode rtctimer hook
 * @rtc: pointer to the rtc_device
 *
 * This functions is called when the aie_timer expires.
 */
void rtc_aie_update_irq(struct rtc_device *rtc)
{
	rtc_handle_legacy_irq(rtc, 1, RTC_AF);
}

/**
 * rtc_uie_update_irq - UIE mode rtctimer hook
 * @rtc: pointer to the rtc_device
 *
 * This functions is called when the uie_timer expires.
 */
void rtc_uie_update_irq(struct rtc_device *rtc)
{
	rtc_handle_legacy_irq(rtc, 1,  RTC_UF);
}

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
/**
 * rtc_pie_update_irq - PIE mode hrtimer hook
 * @timer: pointer to the pie mode hrtimer
 *
 * This function is used to emulate PIE mode interrupts
 * using an hrtimer. This function is called when the periodic
 * hrtimer expires.
 */
enum hrtimer_restart rtc_pie_update_irq(struct hrtimer *timer)
{
	struct rtc_device *rtc;
	ktime_t period;
<<<<<<< HEAD
	int count;
	rtc = container_of(timer, struct rtc_device, pie_timer);

	period = ktime_set(0, NSEC_PER_SEC/rtc->irq_freq);
=======
	u64 count;

	rtc = container_of(timer, struct rtc_device, pie_timer);

	period = NSEC_PER_SEC / rtc->irq_freq;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	count = hrtimer_forward_now(timer, period);

	rtc_handle_legacy_irq(rtc, count, RTC_PF);

	return HRTIMER_RESTART;
}

/**
 * rtc_update_irq - Triggered when a RTC interrupt occurs.
 * @rtc: the rtc device
 * @num: how many irqs are being reported (usually one)
 * @events: mask of RTC_IRQF with one or more of RTC_PF, RTC_AF, RTC_UF
 * Context: any
 */
void rtc_update_irq(struct rtc_device *rtc,
<<<<<<< HEAD
		unsigned long num, unsigned long events)
{
=======
		    unsigned long num, unsigned long events)
{
	if (IS_ERR_OR_NULL(rtc))
		return;

	pm_stay_awake(rtc->dev.parent);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	schedule_work(&rtc->irqwork);
}
EXPORT_SYMBOL_GPL(rtc_update_irq);

<<<<<<< HEAD
static int __rtc_match(struct device *dev, void *data)
{
	char *name = (char *)data;

	if (strcmp(dev_name(dev), name) == 0)
		return 1;
	return 0;
}

struct rtc_device *rtc_class_open(char *name)
=======
struct rtc_device *rtc_class_open(const char *name)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
{
	struct device *dev;
	struct rtc_device *rtc = NULL;

<<<<<<< HEAD
	dev = class_find_device(rtc_class, NULL, name, __rtc_match);
=======
	dev = class_find_device_by_name(&rtc_class, name);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	if (dev)
		rtc = to_rtc_device(dev);

	if (rtc) {
		if (!try_module_get(rtc->owner)) {
			put_device(dev);
			rtc = NULL;
		}
	}

	return rtc;
}
EXPORT_SYMBOL_GPL(rtc_class_open);

void rtc_class_close(struct rtc_device *rtc)
{
	module_put(rtc->owner);
	put_device(&rtc->dev);
}
EXPORT_SYMBOL_GPL(rtc_class_close);

<<<<<<< HEAD
int rtc_irq_register(struct rtc_device *rtc, struct rtc_task *task)
{
	int retval = -EBUSY;

	if (task == NULL || task->func == NULL)
		return -EINVAL;

	/* Cannot register while the char dev is in use */
	if (test_and_set_bit_lock(RTC_DEV_BUSY, &rtc->flags))
		return -EBUSY;

	spin_lock_irq(&rtc->irq_task_lock);
	if (rtc->irq_task == NULL) {
		rtc->irq_task = task;
		retval = 0;
	}
	spin_unlock_irq(&rtc->irq_task_lock);

	clear_bit_unlock(RTC_DEV_BUSY, &rtc->flags);

	return retval;
}
EXPORT_SYMBOL_GPL(rtc_irq_register);

void rtc_irq_unregister(struct rtc_device *rtc, struct rtc_task *task)
{
	spin_lock_irq(&rtc->irq_task_lock);
	if (rtc->irq_task == task)
		rtc->irq_task = NULL;
	spin_unlock_irq(&rtc->irq_task_lock);
}
EXPORT_SYMBOL_GPL(rtc_irq_unregister);

=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
static int rtc_update_hrtimer(struct rtc_device *rtc, int enabled)
{
	/*
	 * We always cancel the timer here first, because otherwise
	 * we could run into BUG_ON(timer->state != HRTIMER_STATE_CALLBACK);
	 * when we manage to start the timer before the callback
	 * returns HRTIMER_RESTART.
	 *
	 * We cannot use hrtimer_cancel() here as a running callback
	 * could be blocked on rtc->irq_task_lock and hrtimer_cancel()
	 * would spin forever.
	 */
	if (hrtimer_try_to_cancel(&rtc->pie_timer) < 0)
		return -1;

	if (enabled) {
<<<<<<< HEAD
		ktime_t period = ktime_set(0, NSEC_PER_SEC / rtc->irq_freq);
=======
		ktime_t period = NSEC_PER_SEC / rtc->irq_freq;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

		hrtimer_start(&rtc->pie_timer, period, HRTIMER_MODE_REL);
	}
	return 0;
}

/**
 * rtc_irq_set_state - enable/disable 2^N Hz periodic IRQs
 * @rtc: the rtc device
<<<<<<< HEAD
 * @task: currently registered with rtc_irq_register()
=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 * @enabled: true to enable periodic IRQs
 * Context: any
 *
 * Note that rtc_irq_set_freq() should previously have been used to
<<<<<<< HEAD
 * specify the desired frequency of periodic IRQ task->func() callbacks.
 */
int rtc_irq_set_state(struct rtc_device *rtc, struct rtc_task *task, int enabled)
{
	int err = 0;
	unsigned long flags;

retry:
	spin_lock_irqsave(&rtc->irq_task_lock, flags);
	if (rtc->irq_task != NULL && task == NULL)
		err = -EBUSY;
	if (rtc->irq_task != task)
		err = -EACCES;
	if (!err) {
		if (rtc_update_hrtimer(rtc, enabled) < 0) {
			spin_unlock_irqrestore(&rtc->irq_task_lock, flags);
			cpu_relax();
			goto retry;
		}
		rtc->pie_enabled = enabled;
	}
	spin_unlock_irqrestore(&rtc->irq_task_lock, flags);
	return err;
}
EXPORT_SYMBOL_GPL(rtc_irq_set_state);
=======
 * specify the desired frequency of periodic IRQ.
 */
int rtc_irq_set_state(struct rtc_device *rtc, int enabled)
{
	int err = 0;

	while (rtc_update_hrtimer(rtc, enabled) < 0)
		cpu_relax();

	rtc->pie_enabled = enabled;

	trace_rtc_irq_set_state(enabled, err);
	return err;
}
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

/**
 * rtc_irq_set_freq - set 2^N Hz periodic IRQ frequency for IRQ
 * @rtc: the rtc device
<<<<<<< HEAD
 * @task: currently registered with rtc_irq_register()
 * @freq: positive frequency with which task->func() will be called
=======
 * @freq: positive frequency
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 * Context: any
 *
 * Note that rtc_irq_set_state() is used to enable or disable the
 * periodic IRQs.
 */
<<<<<<< HEAD
int rtc_irq_set_freq(struct rtc_device *rtc, struct rtc_task *task, int freq)
{
	int err = 0;
	unsigned long flags;

	if (freq <= 0 || freq > RTC_MAX_FREQ)
		return -EINVAL;
retry:
	spin_lock_irqsave(&rtc->irq_task_lock, flags);
	if (rtc->irq_task != NULL && task == NULL)
		err = -EBUSY;
	if (rtc->irq_task != task)
		err = -EACCES;
	if (!err) {
		rtc->irq_freq = freq;
		if (rtc->pie_enabled && rtc_update_hrtimer(rtc, 1) < 0) {
			spin_unlock_irqrestore(&rtc->irq_task_lock, flags);
			cpu_relax();
			goto retry;
		}
	}
	spin_unlock_irqrestore(&rtc->irq_task_lock, flags);
	return err;
}
EXPORT_SYMBOL_GPL(rtc_irq_set_freq);

/**
 * rtc_timer_enqueue - Adds a rtc_timer to the rtc_device timerqueue
 * @rtc rtc device
 * @timer timer being added.
=======
int rtc_irq_set_freq(struct rtc_device *rtc, int freq)
{
	int err = 0;

	if (freq <= 0 || freq > RTC_MAX_FREQ)
		return -EINVAL;

	rtc->irq_freq = freq;
	while (rtc->pie_enabled && rtc_update_hrtimer(rtc, 1) < 0)
		cpu_relax();

	trace_rtc_irq_set_freq(freq, err);
	return err;
}

/**
 * rtc_timer_enqueue - Adds a rtc_timer to the rtc_device timerqueue
 * @rtc: rtc device
 * @timer: timer being added.
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 *
 * Enqueues a timer onto the rtc devices timerqueue and sets
 * the next alarm event appropriately.
 *
 * Sets the enabled bit on the added timer.
 *
 * Must hold ops_lock for proper serialization of timerqueue
 */
static int rtc_timer_enqueue(struct rtc_device *rtc, struct rtc_timer *timer)
{
<<<<<<< HEAD
	timer->enabled = 1;
	timerqueue_add(&rtc->timerqueue, &timer->node);
	if (&timer->node == timerqueue_getnext(&rtc->timerqueue)) {
		struct rtc_wkalrm alarm;
		int err;
		alarm.time = rtc_ktime_to_tm(timer->node.expires);
		alarm.enabled = 1;
		err = __rtc_set_alarm(rtc, &alarm);
		if (err == -ETIME)
			schedule_work(&rtc->irqwork);
		else if (err) {
			timerqueue_del(&rtc->timerqueue, &timer->node);
=======
	struct timerqueue_node *next = timerqueue_getnext(&rtc->timerqueue);
	struct rtc_time tm;
	ktime_t now;
	int err;

	err = __rtc_read_time(rtc, &tm);
	if (err)
		return err;

	timer->enabled = 1;
	now = rtc_tm_to_ktime(tm);

	/* Skip over expired timers */
	while (next) {
		if (next->expires >= now)
			break;
		next = timerqueue_iterate_next(next);
	}

	timerqueue_add(&rtc->timerqueue, &timer->node);
	trace_rtc_timer_enqueue(timer);
	if (!next || ktime_before(timer->node.expires, next->expires)) {
		struct rtc_wkalrm alarm;

		alarm.time = rtc_ktime_to_tm(timer->node.expires);
		alarm.enabled = 1;
		err = __rtc_set_alarm(rtc, &alarm);
		if (err == -ETIME) {
			pm_stay_awake(rtc->dev.parent);
			schedule_work(&rtc->irqwork);
		} else if (err) {
			timerqueue_del(&rtc->timerqueue, &timer->node);
			trace_rtc_timer_dequeue(timer);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
			timer->enabled = 0;
			return err;
		}
	}
	return 0;
}

static void rtc_alarm_disable(struct rtc_device *rtc)
{
<<<<<<< HEAD
	if (!rtc->ops || !rtc->ops->alarm_irq_enable)
		return;

	rtc->ops->alarm_irq_enable(rtc->dev.parent, false);
=======
	if (!rtc->ops || !test_bit(RTC_FEATURE_ALARM, rtc->features) || !rtc->ops->alarm_irq_enable)
		return;

	rtc->ops->alarm_irq_enable(rtc->dev.parent, false);
	trace_rtc_alarm_irq_enable(0, 0);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

/**
 * rtc_timer_remove - Removes a rtc_timer from the rtc_device timerqueue
<<<<<<< HEAD
 * @rtc rtc device
 * @timer timer being removed.
=======
 * @rtc: rtc device
 * @timer: timer being removed.
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 *
 * Removes a timer onto the rtc devices timerqueue and sets
 * the next alarm event appropriately.
 *
 * Clears the enabled bit on the removed timer.
 *
 * Must hold ops_lock for proper serialization of timerqueue
 */
static void rtc_timer_remove(struct rtc_device *rtc, struct rtc_timer *timer)
{
	struct timerqueue_node *next = timerqueue_getnext(&rtc->timerqueue);
<<<<<<< HEAD
	timerqueue_del(&rtc->timerqueue, &timer->node);
=======

	timerqueue_del(&rtc->timerqueue, &timer->node);
	trace_rtc_timer_dequeue(timer);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	timer->enabled = 0;
	if (next == &timer->node) {
		struct rtc_wkalrm alarm;
		int err;
<<<<<<< HEAD
=======

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		next = timerqueue_getnext(&rtc->timerqueue);
		if (!next) {
			rtc_alarm_disable(rtc);
			return;
		}
		alarm.time = rtc_ktime_to_tm(next->expires);
		alarm.enabled = 1;
		err = __rtc_set_alarm(rtc, &alarm);
<<<<<<< HEAD
		if (err == -ETIME)
			schedule_work(&rtc->irqwork);
=======
		if (err == -ETIME) {
			pm_stay_awake(rtc->dev.parent);
			schedule_work(&rtc->irqwork);
		}
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	}
}

/**
 * rtc_timer_do_work - Expires rtc timers
<<<<<<< HEAD
 * @rtc rtc device
 * @timer timer being removed.
=======
 * @work: work item
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 *
 * Expires rtc timers. Reprograms next alarm event if needed.
 * Called via worktask.
 *
 * Serializes access to timerqueue via ops_lock mutex
 */
void rtc_timer_do_work(struct work_struct *work)
{
	struct rtc_timer *timer;
	struct timerqueue_node *next;
	ktime_t now;
	struct rtc_time tm;

	struct rtc_device *rtc =
		container_of(work, struct rtc_device, irqwork);

	mutex_lock(&rtc->ops_lock);
again:
	__rtc_read_time(rtc, &tm);
	now = rtc_tm_to_ktime(tm);
	while ((next = timerqueue_getnext(&rtc->timerqueue))) {
<<<<<<< HEAD
		if (next->expires.tv64 > now.tv64)
=======
		if (next->expires > now)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
			break;

		/* expire timer */
		timer = container_of(next, struct rtc_timer, node);
		timerqueue_del(&rtc->timerqueue, &timer->node);
<<<<<<< HEAD
		timer->enabled = 0;
		if (timer->task.func)
			timer->task.func(timer->task.private_data);

=======
		trace_rtc_timer_dequeue(timer);
		timer->enabled = 0;
		if (timer->func)
			timer->func(timer->rtc);

		trace_rtc_timer_fired(timer);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		/* Re-add/fwd periodic timers */
		if (ktime_to_ns(timer->period)) {
			timer->node.expires = ktime_add(timer->node.expires,
							timer->period);
			timer->enabled = 1;
			timerqueue_add(&rtc->timerqueue, &timer->node);
<<<<<<< HEAD
=======
			trace_rtc_timer_enqueue(timer);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		}
	}

	/* Set next alarm */
	if (next) {
		struct rtc_wkalrm alarm;
		int err;
<<<<<<< HEAD
		alarm.time = rtc_ktime_to_tm(next->expires);
		alarm.enabled = 1;
		err = __rtc_set_alarm(rtc, &alarm);
		if (err == -ETIME)
			goto again;
	} else
		rtc_alarm_disable(rtc);

	mutex_unlock(&rtc->ops_lock);
}


/* rtc_timer_init - Initializes an rtc_timer
 * @timer: timer to be intiialized
 * @f: function pointer to be called when timer fires
 * @data: private data passed to function pointer
 *
 * Kernel interface to initializing an rtc_timer.
 */
void rtc_timer_init(struct rtc_timer *timer, void (*f)(void* p), void* data)
{
	timerqueue_init(&timer->node);
	timer->enabled = 0;
	timer->task.func = f;
	timer->task.private_data = data;
=======
		int retry = 3;

		alarm.time = rtc_ktime_to_tm(next->expires);
		alarm.enabled = 1;
reprogram:
		err = __rtc_set_alarm(rtc, &alarm);
		if (err == -ETIME) {
			goto again;
		} else if (err) {
			if (retry-- > 0)
				goto reprogram;

			timer = container_of(next, struct rtc_timer, node);
			timerqueue_del(&rtc->timerqueue, &timer->node);
			trace_rtc_timer_dequeue(timer);
			timer->enabled = 0;
			dev_err(&rtc->dev, "__rtc_set_alarm: err=%d\n", err);
			goto again;
		}
	} else {
		rtc_alarm_disable(rtc);
	}

	pm_relax(rtc->dev.parent);
	mutex_unlock(&rtc->ops_lock);
}

/* rtc_timer_init - Initializes an rtc_timer
 * @timer: timer to be intiialized
 * @f: function pointer to be called when timer fires
 * @rtc: pointer to the rtc_device
 *
 * Kernel interface to initializing an rtc_timer.
 */
void rtc_timer_init(struct rtc_timer *timer, void (*f)(struct rtc_device *r),
		    struct rtc_device *rtc)
{
	timerqueue_init(&timer->node);
	timer->enabled = 0;
	timer->func = f;
	timer->rtc = rtc;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

/* rtc_timer_start - Sets an rtc_timer to fire in the future
 * @ rtc: rtc device to be used
 * @ timer: timer being set
 * @ expires: time at which to expire the timer
 * @ period: period that the timer will recur
 *
 * Kernel interface to set an rtc_timer
 */
<<<<<<< HEAD
int rtc_timer_start(struct rtc_device *rtc, struct rtc_timer* timer,
			ktime_t expires, ktime_t period)
{
	int ret = 0;
=======
int rtc_timer_start(struct rtc_device *rtc, struct rtc_timer *timer,
		    ktime_t expires, ktime_t period)
{
	int ret = 0;

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	mutex_lock(&rtc->ops_lock);
	if (timer->enabled)
		rtc_timer_remove(rtc, timer);

	timer->node.expires = expires;
	timer->period = period;

	ret = rtc_timer_enqueue(rtc, timer);

	mutex_unlock(&rtc->ops_lock);
	return ret;
}

/* rtc_timer_cancel - Stops an rtc_timer
 * @ rtc: rtc device to be used
 * @ timer: timer being set
 *
 * Kernel interface to cancel an rtc_timer
 */
<<<<<<< HEAD
int rtc_timer_cancel(struct rtc_device *rtc, struct rtc_timer* timer)
{
	int ret = 0;
=======
void rtc_timer_cancel(struct rtc_device *rtc, struct rtc_timer *timer)
{
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	mutex_lock(&rtc->ops_lock);
	if (timer->enabled)
		rtc_timer_remove(rtc, timer);
	mutex_unlock(&rtc->ops_lock);
<<<<<<< HEAD
	return ret;
}


=======
}

/**
 * rtc_read_offset - Read the amount of rtc offset in parts per billion
 * @rtc: rtc device to be used
 * @offset: the offset in parts per billion
 *
 * see below for details.
 *
 * Kernel interface to read rtc clock offset
 * Returns 0 on success, or a negative number on error.
 * If read_offset() is not implemented for the rtc, return -EINVAL
 */
int rtc_read_offset(struct rtc_device *rtc, long *offset)
{
	int ret;

	if (!rtc->ops)
		return -ENODEV;

	if (!rtc->ops->read_offset)
		return -EINVAL;

	mutex_lock(&rtc->ops_lock);
	ret = rtc->ops->read_offset(rtc->dev.parent, offset);
	mutex_unlock(&rtc->ops_lock);

	trace_rtc_read_offset(*offset, ret);
	return ret;
}

/**
 * rtc_set_offset - Adjusts the duration of the average second
 * @rtc: rtc device to be used
 * @offset: the offset in parts per billion
 *
 * Some rtc's allow an adjustment to the average duration of a second
 * to compensate for differences in the actual clock rate due to temperature,
 * the crystal, capacitor, etc.
 *
 * The adjustment applied is as follows:
 *   t = t0 * (1 + offset * 1e-9)
 * where t0 is the measured length of 1 RTC second with offset = 0
 *
 * Kernel interface to adjust an rtc clock offset.
 * Return 0 on success, or a negative number on error.
 * If the rtc offset is not setable (or not implemented), return -EINVAL
 */
int rtc_set_offset(struct rtc_device *rtc, long offset)
{
	int ret;

	if (!rtc->ops)
		return -ENODEV;

	if (!rtc->ops->set_offset)
		return -EINVAL;

	mutex_lock(&rtc->ops_lock);
	ret = rtc->ops->set_offset(rtc->dev.parent, offset);
	mutex_unlock(&rtc->ops_lock);

	trace_rtc_set_offset(offset, ret);
	return ret;
}
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
