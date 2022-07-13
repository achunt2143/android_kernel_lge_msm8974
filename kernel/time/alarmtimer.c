<<<<<<< HEAD
/*
 * Alarmtimer interface
 *
 * This interface provides a timer which is similarto hrtimers,
=======
// SPDX-License-Identifier: GPL-2.0
/*
 * Alarmtimer interface
 *
 * This interface provides a timer which is similar to hrtimers,
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 * but triggers a RTC alarm if the box is suspend.
 *
 * This interface is influenced by the Android RTC Alarm timer
 * interface.
 *
<<<<<<< HEAD
 * Copyright (C) 2010 IBM Corperation
 *
 * Author: John Stultz <john.stultz@linaro.org>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
=======
 * Copyright (C) 2010 IBM Corporation
 *
 * Author: John Stultz <john.stultz@linaro.org>
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 */
#include <linux/time.h>
#include <linux/hrtimer.h>
#include <linux/timerqueue.h>
#include <linux/rtc.h>
<<<<<<< HEAD
=======
#include <linux/sched/signal.h>
#include <linux/sched/debug.h>
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#include <linux/alarmtimer.h>
#include <linux/mutex.h>
#include <linux/platform_device.h>
#include <linux/posix-timers.h>
#include <linux/workqueue.h>
#include <linux/freezer.h>
<<<<<<< HEAD

#define ALARM_DELTA 300
=======
#include <linux/compat.h>
#include <linux/module.h>
#include <linux/time_namespace.h>

#include "posix-timers.h"

#define CREATE_TRACE_POINTS
#include <trace/events/alarmtimer.h>
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

/**
 * struct alarm_base - Alarm timer bases
 * @lock:		Lock for syncrhonized access to the base
 * @timerqueue:		Timerqueue head managing the list of events
<<<<<<< HEAD
 * @timer: 		hrtimer used to schedule events while running
 * @gettime:		Function to read the time correlating to the base
=======
 * @get_ktime:		Function to read the time correlating to the base
 * @get_timespec:	Function to read the namespace time correlating to the base
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 * @base_clockid:	clockid for the base
 */
static struct alarm_base {
	spinlock_t		lock;
	struct timerqueue_head	timerqueue;
<<<<<<< HEAD
	ktime_t			(*gettime)(void);
	clockid_t		base_clockid;
} alarm_bases[ALARM_NUMTYPE];

/* freezer delta & lock used to handle clock_nanosleep triggered wakeups */
static ktime_t freezer_delta;
static DEFINE_SPINLOCK(freezer_delta_lock);

static struct wakeup_source *ws;
=======
	ktime_t			(*get_ktime)(void);
	void			(*get_timespec)(struct timespec64 *tp);
	clockid_t		base_clockid;
} alarm_bases[ALARM_NUMTYPE];

#if defined(CONFIG_POSIX_TIMERS) || defined(CONFIG_RTC_CLASS)
/* freezer information to handle clock_nanosleep triggered wakeups */
static enum alarmtimer_type freezer_alarmtype;
static ktime_t freezer_expires;
static ktime_t freezer_delta;
static DEFINE_SPINLOCK(freezer_delta_lock);
#endif
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

#ifdef CONFIG_RTC_CLASS
/* rtc timer and device for setting alarm wakeups at suspend */
static struct rtc_timer		rtctimer;
static struct rtc_device	*rtcdev;
static DEFINE_SPINLOCK(rtcdev_lock);
<<<<<<< HEAD
static unsigned long power_on_alarm;
static struct mutex power_on_alarm_lock;


void power_on_alarm_init(void)
{
	struct rtc_wkalrm rtc_alarm;
	struct rtc_time rt;
	unsigned long alarm_time;
	struct rtc_device *rtc;

	rtc = alarmtimer_get_rtcdev();

	/* If we have no rtcdev, just return */
	if (!rtc)
		return;

	rtc_read_alarm(rtc, &rtc_alarm);
	rt = rtc_alarm.time;

	rtc_tm_to_time(&rt, &alarm_time);

	if (alarm_time)
		power_on_alarm = alarm_time + ALARM_DELTA;
	else
		power_on_alarm = 0;
}

void set_power_on_alarm(long secs, bool enable)
{
	int rc;
	struct timespec wall_time;
	long rtc_secs, alarm_time, alarm_delta;
	struct rtc_time rtc_time;
	struct rtc_wkalrm alarm;

	rc = mutex_lock_interruptible(&power_on_alarm_lock);
	if (rc != 0)
		return;

	if (enable) {
			power_on_alarm = secs;
	} else {
		if (power_on_alarm == secs)
			power_on_alarm = 0;
		else
			goto exit;
	}

	if (!power_on_alarm)
		goto disable_alarm;

	rtc_read_time(rtcdev, &rtc_time);
	getnstimeofday(&wall_time);
	rtc_tm_to_time(&rtc_time, &rtc_secs);
	alarm_delta = wall_time.tv_sec - rtc_secs;
	alarm_time = power_on_alarm - alarm_delta;

	/*
	 *Substract ALARM_DELTA from actual alarm time
	 *to power up the device before actual alarm
	 *expiration
	 */
	if ((alarm_time - ALARM_DELTA) > rtc_secs)
		alarm_time -= ALARM_DELTA;
	else
		goto disable_alarm;

	rtc_time_to_tm(alarm_time, &alarm.time);
	alarm.enabled = 1;
	rc = rtc_set_alarm(rtcdev, &alarm);
	if (rc)
		goto disable_alarm;

	mutex_unlock(&power_on_alarm_lock);
	return;

disable_alarm:
	power_on_alarm = 0;
	rtc_alarm_irq_enable(rtcdev, 0);
exit:
	mutex_unlock(&power_on_alarm_lock);
}

static void alarmtimer_triggered_func(void *p)
{
	struct rtc_device *rtc = rtcdev;
	if (!(rtc->irq_data & RTC_AF))
		return;
	__pm_wakeup_event(ws, 2 * MSEC_PER_SEC);
}

static struct rtc_task alarmtimer_rtc_task = {
	.func = alarmtimer_triggered_func
};
=======

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
/**
 * alarmtimer_get_rtcdev - Return selected rtcdevice
 *
 * This function returns the rtc device to use for wakealarms.
<<<<<<< HEAD
 * If one has not already been chosen, it checks to see if a
 * functional rtc device is available.
=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 */
struct rtc_device *alarmtimer_get_rtcdev(void)
{
	unsigned long flags;
<<<<<<< HEAD
	struct rtc_device *ret = NULL;
=======
	struct rtc_device *ret;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	spin_lock_irqsave(&rtcdev_lock, flags);
	ret = rtcdev;
	spin_unlock_irqrestore(&rtcdev_lock, flags);

	return ret;
}
<<<<<<< HEAD

static int alarmtimer_rtc_add_device(struct device *dev,
				struct class_interface *class_intf)
{
	unsigned long flags;
	int err = 0;
	struct rtc_device *rtc = to_rtc_device(dev);
	if (rtcdev)
		return -EBUSY;
	if (!rtc->ops->set_alarm)
		return -1;

	spin_lock_irqsave(&rtcdev_lock, flags);
	if (!rtcdev) {
		err = rtc_irq_register(rtc, &alarmtimer_rtc_task);
		if (err)
			goto rtc_irq_reg_err;
		rtcdev = rtc;
		/* hold a reference so it doesn't go away */
		get_device(dev);
	}

rtc_irq_reg_err:
	spin_unlock_irqrestore(&rtcdev_lock, flags);
	return err;

}

static void alarmtimer_rtc_remove_device(struct device *dev,
				struct class_interface *class_intf)
{
	if (rtcdev && dev == &rtcdev->dev) {
		rtc_irq_unregister(rtcdev, &alarmtimer_rtc_task);
		rtcdev = NULL;
	}
=======
EXPORT_SYMBOL_GPL(alarmtimer_get_rtcdev);

static int alarmtimer_rtc_add_device(struct device *dev)
{
	unsigned long flags;
	struct rtc_device *rtc = to_rtc_device(dev);
	struct platform_device *pdev;
	int ret = 0;

	if (rtcdev)
		return -EBUSY;

	if (!test_bit(RTC_FEATURE_ALARM, rtc->features))
		return -1;
	if (!device_may_wakeup(rtc->dev.parent))
		return -1;

	pdev = platform_device_register_data(dev, "alarmtimer",
					     PLATFORM_DEVID_AUTO, NULL, 0);
	if (!IS_ERR(pdev))
		device_init_wakeup(&pdev->dev, true);

	spin_lock_irqsave(&rtcdev_lock, flags);
	if (!IS_ERR(pdev) && !rtcdev) {
		if (!try_module_get(rtc->owner)) {
			ret = -1;
			goto unlock;
		}

		rtcdev = rtc;
		/* hold a reference so it doesn't go away */
		get_device(dev);
		pdev = NULL;
	} else {
		ret = -1;
	}
unlock:
	spin_unlock_irqrestore(&rtcdev_lock, flags);

	platform_device_unregister(pdev);

	return ret;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

static inline void alarmtimer_rtc_timer_init(void)
{
<<<<<<< HEAD
	mutex_init(&power_on_alarm_lock);

=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	rtc_timer_init(&rtctimer, NULL, NULL);
}

static struct class_interface alarmtimer_rtc_interface = {
	.add_dev = &alarmtimer_rtc_add_device,
<<<<<<< HEAD
	.remove_dev = &alarmtimer_rtc_remove_device,
=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
};

static int alarmtimer_rtc_interface_setup(void)
{
<<<<<<< HEAD
	alarmtimer_rtc_interface.class = rtc_class;
=======
	alarmtimer_rtc_interface.class = &rtc_class;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	return class_interface_register(&alarmtimer_rtc_interface);
}
static void alarmtimer_rtc_interface_remove(void)
{
	class_interface_unregister(&alarmtimer_rtc_interface);
}
#else
<<<<<<< HEAD
struct rtc_device *alarmtimer_get_rtcdev(void)
{
	return NULL;
}
#define rtcdev (NULL)
static inline int alarmtimer_rtc_interface_setup(void) { return 0; }
static inline void alarmtimer_rtc_interface_remove(void) { }
static inline void alarmtimer_rtc_timer_init(void) { }
void set_power_on_alarm(long secs, bool enable) { }
=======
static inline int alarmtimer_rtc_interface_setup(void) { return 0; }
static inline void alarmtimer_rtc_interface_remove(void) { }
static inline void alarmtimer_rtc_timer_init(void) { }
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#endif

/**
 * alarmtimer_enqueue - Adds an alarm timer to an alarm_base timerqueue
 * @base: pointer to the base where the timer is being run
 * @alarm: pointer to alarm being enqueued.
 *
 * Adds alarm to a alarm_base timerqueue
 *
 * Must hold base->lock when calling.
 */
static void alarmtimer_enqueue(struct alarm_base *base, struct alarm *alarm)
{
	if (alarm->state & ALARMTIMER_STATE_ENQUEUED)
		timerqueue_del(&base->timerqueue, &alarm->node);

	timerqueue_add(&base->timerqueue, &alarm->node);
	alarm->state |= ALARMTIMER_STATE_ENQUEUED;
}

/**
<<<<<<< HEAD
 * alarmtimer_remove - Removes an alarm timer from an alarm_base timerqueue
=======
 * alarmtimer_dequeue - Removes an alarm timer from an alarm_base timerqueue
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 * @base: pointer to the base where the timer is running
 * @alarm: pointer to alarm being removed
 *
 * Removes alarm to a alarm_base timerqueue
 *
 * Must hold base->lock when calling.
 */
<<<<<<< HEAD
static void alarmtimer_remove(struct alarm_base *base, struct alarm *alarm)
=======
static void alarmtimer_dequeue(struct alarm_base *base, struct alarm *alarm)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
{
	if (!(alarm->state & ALARMTIMER_STATE_ENQUEUED))
		return;

	timerqueue_del(&base->timerqueue, &alarm->node);
	alarm->state &= ~ALARMTIMER_STATE_ENQUEUED;
}


/**
 * alarmtimer_fired - Handles alarm hrtimer being fired.
 * @timer: pointer to hrtimer being run
 *
 * When a alarm timer fires, this runs through the timerqueue to
 * see which alarms expired, and runs those. If there are more alarm
 * timers queued for the future, we set the hrtimer to fire when
<<<<<<< HEAD
 * when the next future alarm timer expires.
=======
 * the next future alarm timer expires.
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 */
static enum hrtimer_restart alarmtimer_fired(struct hrtimer *timer)
{
	struct alarm *alarm = container_of(timer, struct alarm, timer);
	struct alarm_base *base = &alarm_bases[alarm->type];
	unsigned long flags;
	int ret = HRTIMER_NORESTART;
	int restart = ALARMTIMER_NORESTART;

	spin_lock_irqsave(&base->lock, flags);
<<<<<<< HEAD
	alarmtimer_remove(base, alarm);
	spin_unlock_irqrestore(&base->lock, flags);

	if (alarm->function)
		restart = alarm->function(alarm, base->gettime());
=======
	alarmtimer_dequeue(base, alarm);
	spin_unlock_irqrestore(&base->lock, flags);

	if (alarm->function)
		restart = alarm->function(alarm, base->get_ktime());
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	spin_lock_irqsave(&base->lock, flags);
	if (restart != ALARMTIMER_NORESTART) {
		hrtimer_set_expires(&alarm->timer, alarm->node.expires);
		alarmtimer_enqueue(base, alarm);
		ret = HRTIMER_RESTART;
	}
	spin_unlock_irqrestore(&base->lock, flags);

<<<<<<< HEAD
=======
	trace_alarmtimer_fired(alarm, base->get_ktime());
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	return ret;

}

ktime_t alarm_expires_remaining(const struct alarm *alarm)
{
	struct alarm_base *base = &alarm_bases[alarm->type];
<<<<<<< HEAD
	return ktime_sub(alarm->node.expires, base->gettime());
}
=======
	return ktime_sub(alarm->node.expires, base->get_ktime());
}
EXPORT_SYMBOL_GPL(alarm_expires_remaining);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

#ifdef CONFIG_RTC_CLASS
/**
 * alarmtimer_suspend - Suspend time callback
 * @dev: unused
<<<<<<< HEAD
 * @state: unused
=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 *
 * When we are going into suspend, we look through the bases
 * to see which is the soonest timer to expire. We then
 * set an rtc timer to fire that far into the future, which
 * will wake us from suspend.
 */
<<<<<<< HEAD
#if defined(CONFIG_RTC_DRV_QPNP) && defined(CONFIG_MSM_PM)
extern void lpm_suspend_wake_time(uint64_t wakeup_time);
static int alarmtimer_suspend(struct device *dev)
{
	struct rtc_time tm;
	ktime_t min, now;
	unsigned long flags;
	struct rtc_device *rtc;
	int i;
	int ret = 0;

	spin_lock_irqsave(&freezer_delta_lock, flags);
	min = freezer_delta;
	freezer_delta = ktime_set(0, 0);
=======
static int alarmtimer_suspend(struct device *dev)
{
	ktime_t min, now, expires;
	int i, ret, type;
	struct rtc_device *rtc;
	unsigned long flags;
	struct rtc_time tm;

	spin_lock_irqsave(&freezer_delta_lock, flags);
	min = freezer_delta;
	expires = freezer_expires;
	type = freezer_alarmtype;
	freezer_delta = 0;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	spin_unlock_irqrestore(&freezer_delta_lock, flags);

	rtc = alarmtimer_get_rtcdev();
	/* If we have no rtcdev, just return */
	if (!rtc)
		return 0;

	/* Find the soonest timer to expire*/
	for (i = 0; i < ALARM_NUMTYPE; i++) {
		struct alarm_base *base = &alarm_bases[i];
		struct timerqueue_node *next;
		ktime_t delta;

		spin_lock_irqsave(&base->lock, flags);
		next = timerqueue_getnext(&base->timerqueue);
		spin_unlock_irqrestore(&base->lock, flags);
		if (!next)
			continue;
<<<<<<< HEAD
		delta = ktime_sub(next->expires, base->gettime());
		if (!min.tv64 || (delta.tv64 < min.tv64))
			min = delta;
	}
	if (min.tv64 == 0)
		return 0;

	if (ktime_to_ns(min) < 2 * NSEC_PER_SEC) {
		__pm_wakeup_event(ws, 2 * MSEC_PER_SEC);
		return -EBUSY;
	}

	/* Setup a timer to fire that far in the future */
	rtc_timer_cancel(rtc, &rtctimer);
	rtc_read_time(rtc, &tm);
	now = rtc_tm_to_ktime(tm);
	now = ktime_add(now, min);
	if (poweron_alarm) {
		struct rtc_time tm_val;
		unsigned long secs;
		tm_val = rtc_ktime_to_tm(min);
		rtc_tm_to_time(&tm_val, &secs);
		lpm_suspend_wake_time(secs);
	} else {
		/* Set alarm, if in the past reject suspend briefly to handle */
		ret = rtc_timer_start(rtc, &rtctimer, now, ktime_set(0, 0));
		if (ret < 0)
			__pm_wakeup_event(ws, MSEC_PER_SEC);
	}
	return ret;
}
#else
static int alarmtimer_suspend(struct device *dev)
{
	struct rtc_time tm;
	ktime_t min, now;
	unsigned long flags;
	struct rtc_device *rtc;
	int i;
	int ret;

	spin_lock_irqsave(&freezer_delta_lock, flags);
	min = freezer_delta;
	freezer_delta = ktime_set(0, 0);
	spin_unlock_irqrestore(&freezer_delta_lock, flags);

	rtc = alarmtimer_get_rtcdev();
	/* If we have no rtcdev, just return */
	if (!rtc)
		return 0;

	/* Find the soonest timer to expire*/
	for (i = 0; i < ALARM_NUMTYPE; i++) {
		struct alarm_base *base = &alarm_bases[i];
		struct timerqueue_node *next;
		ktime_t delta;

		spin_lock_irqsave(&base->lock, flags);
		next = timerqueue_getnext(&base->timerqueue);
		spin_unlock_irqrestore(&base->lock, flags);
		if (!next)
			continue;
		delta = ktime_sub(next->expires, base->gettime());
		if (!min.tv64 || (delta.tv64 < min.tv64))
			min = delta;
	}
	if (min.tv64 == 0)
		return 0;

	if (ktime_to_ns(min) < 2 * NSEC_PER_SEC) {
		__pm_wakeup_event(ws, 2 * MSEC_PER_SEC);
		return -EBUSY;
	}
=======
		delta = ktime_sub(next->expires, base->get_ktime());
		if (!min || (delta < min)) {
			expires = next->expires;
			min = delta;
			type = i;
		}
	}
	if (min == 0)
		return 0;

	if (ktime_to_ns(min) < 2 * NSEC_PER_SEC) {
		pm_wakeup_event(dev, 2 * MSEC_PER_SEC);
		return -EBUSY;
	}

	trace_alarmtimer_suspend(expires, type);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	/* Setup an rtc timer to fire that far in the future */
	rtc_timer_cancel(rtc, &rtctimer);
	rtc_read_time(rtc, &tm);
	now = rtc_tm_to_ktime(tm);
<<<<<<< HEAD
	now = ktime_add(now, min);

	/* Set alarm, if in the past reject suspend briefly to handle */
	ret = rtc_timer_start(rtc, &rtctimer, now, ktime_set(0, 0));
	if (ret < 0)
		__pm_wakeup_event(ws, 1 * MSEC_PER_SEC);
	return ret;
}
#endif
=======

	/*
	 * If the RTC alarm timer only supports a limited time offset, set the
	 * alarm time to the maximum supported value.
	 * The system may wake up earlier (possibly much earlier) than expected
	 * when the alarmtimer runs. This is the best the kernel can do if
	 * the alarmtimer exceeds the time that the rtc device can be programmed
	 * for.
	 */
	min = rtc_bound_alarmtime(rtc, min);

	now = ktime_add(now, min);

	/* Set alarm, if in the past reject suspend briefly to handle */
	ret = rtc_timer_start(rtc, &rtctimer, now, 0);
	if (ret < 0)
		pm_wakeup_event(dev, MSEC_PER_SEC);
	return ret;
}

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
static int alarmtimer_resume(struct device *dev)
{
	struct rtc_device *rtc;

	rtc = alarmtimer_get_rtcdev();
<<<<<<< HEAD
	/* If we have no rtcdev, just return */
	if (!rtc)
		return 0;
	rtc_timer_cancel(rtc, &rtctimer);

	set_power_on_alarm(power_on_alarm , 1);
	return 0;
}
=======
	if (rtc)
		rtc_timer_cancel(rtc, &rtctimer);
	return 0;
}

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#else
static int alarmtimer_suspend(struct device *dev)
{
	return 0;
}

static int alarmtimer_resume(struct device *dev)
{
	return 0;
}
#endif

<<<<<<< HEAD
static void alarmtimer_freezerset(ktime_t absexp, enum alarmtimer_type type)
{
	ktime_t delta;
	unsigned long flags;
	struct alarm_base *base = &alarm_bases[type];

	delta = ktime_sub(absexp, base->gettime());

	spin_lock_irqsave(&freezer_delta_lock, flags);
	if (!freezer_delta.tv64 || (delta.tv64 < freezer_delta.tv64))
		freezer_delta = delta;
	spin_unlock_irqrestore(&freezer_delta_lock, flags);
}


=======
static void
__alarm_init(struct alarm *alarm, enum alarmtimer_type type,
	     enum alarmtimer_restart (*function)(struct alarm *, ktime_t))
{
	timerqueue_init(&alarm->node);
	alarm->timer.function = alarmtimer_fired;
	alarm->function = function;
	alarm->type = type;
	alarm->state = ALARMTIMER_STATE_INACTIVE;
}

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
/**
 * alarm_init - Initialize an alarm structure
 * @alarm: ptr to alarm to be initialized
 * @type: the type of the alarm
 * @function: callback that is run when the alarm fires
 */
void alarm_init(struct alarm *alarm, enum alarmtimer_type type,
		enum alarmtimer_restart (*function)(struct alarm *, ktime_t))
{
<<<<<<< HEAD
	timerqueue_init(&alarm->node);
	hrtimer_init(&alarm->timer, alarm_bases[type].base_clockid,
			HRTIMER_MODE_ABS);
	alarm->timer.function = alarmtimer_fired;
	alarm->function = function;
	alarm->type = type;
	alarm->state = ALARMTIMER_STATE_INACTIVE;
}
=======
	hrtimer_init(&alarm->timer, alarm_bases[type].base_clockid,
		     HRTIMER_MODE_ABS);
	__alarm_init(alarm, type, function);
}
EXPORT_SYMBOL_GPL(alarm_init);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

/**
 * alarm_start - Sets an absolute alarm to fire
 * @alarm: ptr to alarm to set
 * @start: time to run the alarm
 */
<<<<<<< HEAD
int alarm_start(struct alarm *alarm, ktime_t start)
{
	struct alarm_base *base = &alarm_bases[alarm->type];
	unsigned long flags;
	int ret;
=======
void alarm_start(struct alarm *alarm, ktime_t start)
{
	struct alarm_base *base = &alarm_bases[alarm->type];
	unsigned long flags;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	spin_lock_irqsave(&base->lock, flags);
	alarm->node.expires = start;
	alarmtimer_enqueue(base, alarm);
<<<<<<< HEAD
	ret = hrtimer_start(&alarm->timer, alarm->node.expires,
				HRTIMER_MODE_ABS);
	spin_unlock_irqrestore(&base->lock, flags);
	return ret;
}
=======
	hrtimer_start(&alarm->timer, alarm->node.expires, HRTIMER_MODE_ABS);
	spin_unlock_irqrestore(&base->lock, flags);

	trace_alarmtimer_start(alarm, base->get_ktime());
}
EXPORT_SYMBOL_GPL(alarm_start);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

/**
 * alarm_start_relative - Sets a relative alarm to fire
 * @alarm: ptr to alarm to set
 * @start: time relative to now to run the alarm
 */
<<<<<<< HEAD
int alarm_start_relative(struct alarm *alarm, ktime_t start)
{
	struct alarm_base *base;

	if (alarm->type >= ALARM_NUMTYPE) {
		pr_err("Array out of index\n");
		return -EINVAL;
	}
	base = &alarm_bases[alarm->type];
	start = ktime_add(start, base->gettime());
	return alarm_start(alarm, start);
}
=======
void alarm_start_relative(struct alarm *alarm, ktime_t start)
{
	struct alarm_base *base = &alarm_bases[alarm->type];

	start = ktime_add_safe(start, base->get_ktime());
	alarm_start(alarm, start);
}
EXPORT_SYMBOL_GPL(alarm_start_relative);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

void alarm_restart(struct alarm *alarm)
{
	struct alarm_base *base = &alarm_bases[alarm->type];
	unsigned long flags;

	spin_lock_irqsave(&base->lock, flags);
	hrtimer_set_expires(&alarm->timer, alarm->node.expires);
	hrtimer_restart(&alarm->timer);
	alarmtimer_enqueue(base, alarm);
	spin_unlock_irqrestore(&base->lock, flags);
}
<<<<<<< HEAD
=======
EXPORT_SYMBOL_GPL(alarm_restart);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

/**
 * alarm_try_to_cancel - Tries to cancel an alarm timer
 * @alarm: ptr to alarm to be canceled
 *
 * Returns 1 if the timer was canceled, 0 if it was not running,
 * and -1 if the callback was running
 */
int alarm_try_to_cancel(struct alarm *alarm)
{
<<<<<<< HEAD
	struct alarm_base *base;
	unsigned long flags;
	int ret;

	if (alarm->type >= ALARM_NUMTYPE) {
		pr_err("Array out of index\n");
		return -EINVAL;
	}
	base = &alarm_bases[alarm->type];
	spin_lock_irqsave(&base->lock, flags);
	ret = hrtimer_try_to_cancel(&alarm->timer);
	if (ret >= 0)
		alarmtimer_remove(base, alarm);
	spin_unlock_irqrestore(&base->lock, flags);
	return ret;
}
=======
	struct alarm_base *base = &alarm_bases[alarm->type];
	unsigned long flags;
	int ret;

	spin_lock_irqsave(&base->lock, flags);
	ret = hrtimer_try_to_cancel(&alarm->timer);
	if (ret >= 0)
		alarmtimer_dequeue(base, alarm);
	spin_unlock_irqrestore(&base->lock, flags);

	trace_alarmtimer_cancel(alarm, base->get_ktime());
	return ret;
}
EXPORT_SYMBOL_GPL(alarm_try_to_cancel);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)


/**
 * alarm_cancel - Spins trying to cancel an alarm timer until it is done
 * @alarm: ptr to alarm to be canceled
 *
 * Returns 1 if the timer was canceled, 0 if it was not active.
 */
int alarm_cancel(struct alarm *alarm)
{
	for (;;) {
		int ret = alarm_try_to_cancel(alarm);
		if (ret >= 0)
			return ret;
<<<<<<< HEAD
		cpu_relax();
	}
}
=======
		hrtimer_cancel_wait_running(&alarm->timer);
	}
}
EXPORT_SYMBOL_GPL(alarm_cancel);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)


u64 alarm_forward(struct alarm *alarm, ktime_t now, ktime_t interval)
{
	u64 overrun = 1;
	ktime_t delta;

	delta = ktime_sub(now, alarm->node.expires);

<<<<<<< HEAD
	if (delta.tv64 < 0)
		return 0;

	if (unlikely(delta.tv64 >= interval.tv64)) {
=======
	if (delta < 0)
		return 0;

	if (unlikely(delta >= interval)) {
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		s64 incr = ktime_to_ns(interval);

		overrun = ktime_divns(delta, incr);

		alarm->node.expires = ktime_add_ns(alarm->node.expires,
							incr*overrun);

<<<<<<< HEAD
		if (alarm->node.expires.tv64 > now.tv64)
=======
		if (alarm->node.expires > now)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
			return overrun;
		/*
		 * This (and the ktime_add() below) is the
		 * correction for exact:
		 */
		overrun++;
	}

<<<<<<< HEAD
	alarm->node.expires = ktime_add(alarm->node.expires, interval);
	return overrun;
}

u64 alarm_forward_now(struct alarm *alarm, ktime_t interval)
{
	struct alarm_base *base = &alarm_bases[alarm->type];

	return alarm_forward(alarm, base->gettime(), interval);
}


=======
	alarm->node.expires = ktime_add_safe(alarm->node.expires, interval);
	return overrun;
}
EXPORT_SYMBOL_GPL(alarm_forward);

static u64 __alarm_forward_now(struct alarm *alarm, ktime_t interval, bool throttle)
{
	struct alarm_base *base = &alarm_bases[alarm->type];
	ktime_t now = base->get_ktime();

	if (IS_ENABLED(CONFIG_HIGH_RES_TIMERS) && throttle) {
		/*
		 * Same issue as with posix_timer_fn(). Timers which are
		 * periodic but the signal is ignored can starve the system
		 * with a very small interval. The real fix which was
		 * promised in the context of posix_timer_fn() never
		 * materialized, but someone should really work on it.
		 *
		 * To prevent DOS fake @now to be 1 jiffie out which keeps
		 * the overrun accounting correct but creates an
		 * inconsistency vs. timer_gettime(2).
		 */
		ktime_t kj = NSEC_PER_SEC / HZ;

		if (interval < kj)
			now = ktime_add(now, kj);
	}

	return alarm_forward(alarm, now, interval);
}

u64 alarm_forward_now(struct alarm *alarm, ktime_t interval)
{
	return __alarm_forward_now(alarm, interval, false);
}
EXPORT_SYMBOL_GPL(alarm_forward_now);

#ifdef CONFIG_POSIX_TIMERS

static void alarmtimer_freezerset(ktime_t absexp, enum alarmtimer_type type)
{
	struct alarm_base *base;
	unsigned long flags;
	ktime_t delta;

	switch(type) {
	case ALARM_REALTIME:
		base = &alarm_bases[ALARM_REALTIME];
		type = ALARM_REALTIME_FREEZER;
		break;
	case ALARM_BOOTTIME:
		base = &alarm_bases[ALARM_BOOTTIME];
		type = ALARM_BOOTTIME_FREEZER;
		break;
	default:
		WARN_ONCE(1, "Invalid alarm type: %d\n", type);
		return;
	}

	delta = ktime_sub(absexp, base->get_ktime());

	spin_lock_irqsave(&freezer_delta_lock, flags);
	if (!freezer_delta || (delta < freezer_delta)) {
		freezer_delta = delta;
		freezer_expires = absexp;
		freezer_alarmtype = type;
	}
	spin_unlock_irqrestore(&freezer_delta_lock, flags);
}
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

/**
 * clock2alarm - helper that converts from clockid to alarmtypes
 * @clockid: clockid.
 */
static enum alarmtimer_type clock2alarm(clockid_t clockid)
{
	if (clockid == CLOCK_REALTIME_ALARM)
		return ALARM_REALTIME;
	if (clockid == CLOCK_BOOTTIME_ALARM)
		return ALARM_BOOTTIME;
	return -1;
}

/**
 * alarm_handle_timer - Callback for posix timers
 * @alarm: alarm that fired
<<<<<<< HEAD
 *
 * Posix timer callback for expired alarm timers.
=======
 * @now: time at the timer expiration
 *
 * Posix timer callback for expired alarm timers.
 *
 * Return: whether the timer is to be restarted
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 */
static enum alarmtimer_restart alarm_handle_timer(struct alarm *alarm,
							ktime_t now)
{
<<<<<<< HEAD
	unsigned long flags;
	struct k_itimer *ptr = container_of(alarm, struct k_itimer,
						it.alarm.alarmtimer);
	enum alarmtimer_restart result = ALARMTIMER_NORESTART;

	spin_lock_irqsave(&ptr->it_lock, flags);
	if ((ptr->it_sigev_notify & ~SIGEV_THREAD_ID) != SIGEV_NONE) {
		if (posix_timer_event(ptr, 0) != 0)
			ptr->it_overrun++;
	}

	/* Re-add periodic timers */
	if (ptr->it.alarm.interval.tv64) {
		ptr->it_overrun += alarm_forward(alarm, now,
						ptr->it.alarm.interval);
=======
	struct k_itimer *ptr = container_of(alarm, struct k_itimer,
					    it.alarm.alarmtimer);
	enum alarmtimer_restart result = ALARMTIMER_NORESTART;
	unsigned long flags;
	int si_private = 0;

	spin_lock_irqsave(&ptr->it_lock, flags);

	ptr->it_active = 0;
	if (ptr->it_interval)
		si_private = ++ptr->it_requeue_pending;

	if (posix_timer_event(ptr, si_private) && ptr->it_interval) {
		/*
		 * Handle ignored signals and rearm the timer. This will go
		 * away once we handle ignored signals proper. Ensure that
		 * small intervals cannot starve the system.
		 */
		ptr->it_overrun += __alarm_forward_now(alarm, ptr->it_interval, true);
		++ptr->it_requeue_pending;
		ptr->it_active = 1;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		result = ALARMTIMER_RESTART;
	}
	spin_unlock_irqrestore(&ptr->it_lock, flags);

	return result;
}

/**
<<<<<<< HEAD
=======
 * alarm_timer_rearm - Posix timer callback for rearming timer
 * @timr:	Pointer to the posixtimer data struct
 */
static void alarm_timer_rearm(struct k_itimer *timr)
{
	struct alarm *alarm = &timr->it.alarm.alarmtimer;

	timr->it_overrun += alarm_forward_now(alarm, timr->it_interval);
	alarm_start(alarm, alarm->node.expires);
}

/**
 * alarm_timer_forward - Posix timer callback for forwarding timer
 * @timr:	Pointer to the posixtimer data struct
 * @now:	Current time to forward the timer against
 */
static s64 alarm_timer_forward(struct k_itimer *timr, ktime_t now)
{
	struct alarm *alarm = &timr->it.alarm.alarmtimer;

	return alarm_forward(alarm, timr->it_interval, now);
}

/**
 * alarm_timer_remaining - Posix timer callback to retrieve remaining time
 * @timr:	Pointer to the posixtimer data struct
 * @now:	Current time to calculate against
 */
static ktime_t alarm_timer_remaining(struct k_itimer *timr, ktime_t now)
{
	struct alarm *alarm = &timr->it.alarm.alarmtimer;

	return ktime_sub(alarm->node.expires, now);
}

/**
 * alarm_timer_try_to_cancel - Posix timer callback to cancel a timer
 * @timr:	Pointer to the posixtimer data struct
 */
static int alarm_timer_try_to_cancel(struct k_itimer *timr)
{
	return alarm_try_to_cancel(&timr->it.alarm.alarmtimer);
}

/**
 * alarm_timer_wait_running - Posix timer callback to wait for a timer
 * @timr:	Pointer to the posixtimer data struct
 *
 * Called from the core code when timer cancel detected that the callback
 * is running. @timr is unlocked and rcu read lock is held to prevent it
 * from being freed.
 */
static void alarm_timer_wait_running(struct k_itimer *timr)
{
	hrtimer_cancel_wait_running(&timr->it.alarm.alarmtimer.timer);
}

/**
 * alarm_timer_arm - Posix timer callback to arm a timer
 * @timr:	Pointer to the posixtimer data struct
 * @expires:	The new expiry time
 * @absolute:	Expiry value is absolute time
 * @sigev_none:	Posix timer does not deliver signals
 */
static void alarm_timer_arm(struct k_itimer *timr, ktime_t expires,
			    bool absolute, bool sigev_none)
{
	struct alarm *alarm = &timr->it.alarm.alarmtimer;
	struct alarm_base *base = &alarm_bases[alarm->type];

	if (!absolute)
		expires = ktime_add_safe(expires, base->get_ktime());
	if (sigev_none)
		alarm->node.expires = expires;
	else
		alarm_start(&timr->it.alarm.alarmtimer, expires);
}

/**
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 * alarm_clock_getres - posix getres interface
 * @which_clock: clockid
 * @tp: timespec to fill
 *
 * Returns the granularity of underlying alarm base clock
 */
<<<<<<< HEAD
static int alarm_clock_getres(const clockid_t which_clock, struct timespec *tp)
{
	clockid_t baseid = alarm_bases[clock2alarm(which_clock)].base_clockid;

	if (!alarmtimer_get_rtcdev())
		return -EINVAL;

	return hrtimer_get_res(baseid, tp);
}

/**
 * alarm_clock_get - posix clock_get interface
 * @which_clock: clockid
 * @tp: timespec to fill.
 *
 * Provides the underlying alarm base time.
 */
static int alarm_clock_get(clockid_t which_clock, struct timespec *tp)
=======
static int alarm_clock_getres(const clockid_t which_clock, struct timespec64 *tp)
{
	if (!alarmtimer_get_rtcdev())
		return -EINVAL;

	tp->tv_sec = 0;
	tp->tv_nsec = hrtimer_resolution;
	return 0;
}

/**
 * alarm_clock_get_timespec - posix clock_get_timespec interface
 * @which_clock: clockid
 * @tp: timespec to fill.
 *
 * Provides the underlying alarm base time in a tasks time namespace.
 */
static int alarm_clock_get_timespec(clockid_t which_clock, struct timespec64 *tp)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
{
	struct alarm_base *base = &alarm_bases[clock2alarm(which_clock)];

	if (!alarmtimer_get_rtcdev())
		return -EINVAL;

<<<<<<< HEAD
	*tp = ktime_to_timespec(base->gettime());
=======
	base->get_timespec(tp);

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	return 0;
}

/**
<<<<<<< HEAD
=======
 * alarm_clock_get_ktime - posix clock_get_ktime interface
 * @which_clock: clockid
 *
 * Provides the underlying alarm base time in the root namespace.
 */
static ktime_t alarm_clock_get_ktime(clockid_t which_clock)
{
	struct alarm_base *base = &alarm_bases[clock2alarm(which_clock)];

	if (!alarmtimer_get_rtcdev())
		return -EINVAL;

	return base->get_ktime();
}

/**
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 * alarm_timer_create - posix timer_create interface
 * @new_timer: k_itimer pointer to manage
 *
 * Initializes the k_itimer structure.
 */
static int alarm_timer_create(struct k_itimer *new_timer)
{
	enum  alarmtimer_type type;
<<<<<<< HEAD
	struct alarm_base *base;

	if (!alarmtimer_get_rtcdev())
		return -ENOTSUPP;
=======

	if (!alarmtimer_get_rtcdev())
		return -EOPNOTSUPP;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	if (!capable(CAP_WAKE_ALARM))
		return -EPERM;

	type = clock2alarm(new_timer->it_clock);
<<<<<<< HEAD
	base = &alarm_bases[type];
=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	alarm_init(&new_timer->it.alarm.alarmtimer, type, alarm_handle_timer);
	return 0;
}

/**
<<<<<<< HEAD
 * alarm_timer_get - posix timer_get interface
 * @new_timer: k_itimer pointer
 * @cur_setting: itimerspec data to fill
 *
 * Copies out the current itimerspec data
 */
static void alarm_timer_get(struct k_itimer *timr,
				struct itimerspec *cur_setting)
{
	ktime_t relative_expiry_time =
		alarm_expires_remaining(&(timr->it.alarm.alarmtimer));

	if (ktime_to_ns(relative_expiry_time) > 0) {
		cur_setting->it_value = ktime_to_timespec(relative_expiry_time);
	} else {
		cur_setting->it_value.tv_sec = 0;
		cur_setting->it_value.tv_nsec = 0;
	}

	cur_setting->it_interval = ktime_to_timespec(timr->it.alarm.interval);
}

/**
 * alarm_timer_del - posix timer_del interface
 * @timr: k_itimer pointer to be deleted
 *
 * Cancels any programmed alarms for the given timer.
 */
static int alarm_timer_del(struct k_itimer *timr)
{
	if (!rtcdev)
		return -ENOTSUPP;

	if (alarm_try_to_cancel(&timr->it.alarm.alarmtimer) < 0)
		return TIMER_RETRY;

	return 0;
}

/**
 * alarm_timer_set - posix timer_set interface
 * @timr: k_itimer pointer to be deleted
 * @flags: timer flags
 * @new_setting: itimerspec to be used
 * @old_setting: itimerspec being replaced
 *
 * Sets the timer to new_setting, and starts the timer.
 */
static int alarm_timer_set(struct k_itimer *timr, int flags,
				struct itimerspec *new_setting,
				struct itimerspec *old_setting)
{
	ktime_t exp;

	if (!rtcdev)
		return -ENOTSUPP;

	if (flags & ~TIMER_ABSTIME)
		return -EINVAL;

	if (old_setting)
		alarm_timer_get(timr, old_setting);

	/* If the timer was already set, cancel it */
	if (alarm_try_to_cancel(&timr->it.alarm.alarmtimer) < 0)
		return TIMER_RETRY;

	/* start the timer */
	timr->it.alarm.interval = timespec_to_ktime(new_setting->it_interval);
	exp = timespec_to_ktime(new_setting->it_value);
	/* Convert (if necessary) to absolute time */
	if (flags != TIMER_ABSTIME) {
		ktime_t now;

		now = alarm_bases[timr->it.alarm.alarmtimer.type].gettime();
		exp = ktime_add(now, exp);
	}

	alarm_start(&timr->it.alarm.alarmtimer, exp);
	return 0;
}

/**
 * alarmtimer_nsleep_wakeup - Wakeup function for alarm_timer_nsleep
 * @alarm: ptr to alarm that fired
 *
 * Wakes up the task that set the alarmtimer
=======
 * alarmtimer_nsleep_wakeup - Wakeup function for alarm_timer_nsleep
 * @alarm: ptr to alarm that fired
 * @now: time at the timer expiration
 *
 * Wakes up the task that set the alarmtimer
 *
 * Return: ALARMTIMER_NORESTART
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 */
static enum alarmtimer_restart alarmtimer_nsleep_wakeup(struct alarm *alarm,
								ktime_t now)
{
<<<<<<< HEAD
	struct task_struct *task = (struct task_struct *)alarm->data;
=======
	struct task_struct *task = alarm->data;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	alarm->data = NULL;
	if (task)
		wake_up_process(task);
	return ALARMTIMER_NORESTART;
}

/**
 * alarmtimer_do_nsleep - Internal alarmtimer nsleep implementation
 * @alarm: ptr to alarmtimer
 * @absexp: absolute expiration time
<<<<<<< HEAD
 *
 * Sets the alarm timer and sleeps until it is fired or interrupted.
 */
static int alarmtimer_do_nsleep(struct alarm *alarm, ktime_t absexp)
{
=======
 * @type: alarm type (BOOTTIME/REALTIME).
 *
 * Sets the alarm timer and sleeps until it is fired or interrupted.
 */
static int alarmtimer_do_nsleep(struct alarm *alarm, ktime_t absexp,
				enum alarmtimer_type type)
{
	struct restart_block *restart;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	alarm->data = (void *)current;
	do {
		set_current_state(TASK_INTERRUPTIBLE);
		alarm_start(alarm, absexp);
		if (likely(alarm->data))
			schedule();

		alarm_cancel(alarm);
	} while (alarm->data && !signal_pending(current));

	__set_current_state(TASK_RUNNING);

<<<<<<< HEAD
	return (alarm->data == NULL);
}


/**
 * update_rmtp - Update remaining timespec value
 * @exp: expiration time
 * @type: timer type
 * @rmtp: user pointer to remaining timepsec value
 *
 * Helper function that fills in rmtp value with time between
 * now and the exp value
 */
static int update_rmtp(ktime_t exp, enum  alarmtimer_type type,
			struct timespec __user *rmtp)
{
	struct timespec rmt;
	ktime_t rem;

	rem = ktime_sub(exp, alarm_bases[type].gettime());

	if (rem.tv64 <= 0)
		return 0;
	rmt = ktime_to_timespec(rem);

	if (copy_to_user(rmtp, &rmt, sizeof(*rmtp)))
		return -EFAULT;

	return 1;

=======
	destroy_hrtimer_on_stack(&alarm->timer);

	if (!alarm->data)
		return 0;

	if (freezing(current))
		alarmtimer_freezerset(absexp, type);
	restart = &current->restart_block;
	if (restart->nanosleep.type != TT_NONE) {
		struct timespec64 rmt;
		ktime_t rem;

		rem = ktime_sub(absexp, alarm_bases[type].get_ktime());

		if (rem <= 0)
			return 0;
		rmt = ktime_to_timespec64(rem);

		return nanosleep_copyout(restart, &rmt);
	}
	return -ERESTART_RESTARTBLOCK;
}

static void
alarm_init_on_stack(struct alarm *alarm, enum alarmtimer_type type,
		    enum alarmtimer_restart (*function)(struct alarm *, ktime_t))
{
	hrtimer_init_on_stack(&alarm->timer, alarm_bases[type].base_clockid,
			      HRTIMER_MODE_ABS);
	__alarm_init(alarm, type, function);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

/**
 * alarm_timer_nsleep_restart - restartblock alarmtimer nsleep
 * @restart: ptr to restart block
 *
 * Handles restarted clock_nanosleep calls
 */
static long __sched alarm_timer_nsleep_restart(struct restart_block *restart)
{
	enum  alarmtimer_type type = restart->nanosleep.clockid;
<<<<<<< HEAD
	ktime_t exp;
	struct timespec __user  *rmtp;
	struct alarm alarm;
	int ret = 0;

	exp.tv64 = restart->nanosleep.expires;
	alarm_init(&alarm, type, alarmtimer_nsleep_wakeup);

	if (alarmtimer_do_nsleep(&alarm, exp))
		goto out;

	if (freezing(current))
		alarmtimer_freezerset(exp, type);

	rmtp = restart->nanosleep.rmtp;
	if (rmtp) {
		ret = update_rmtp(exp, type, rmtp);
		if (ret <= 0)
			goto out;
	}


	/* The other values in restart are already filled in */
	ret = -ERESTART_RESTARTBLOCK;
out:
	return ret;
=======
	ktime_t exp = restart->nanosleep.expires;
	struct alarm alarm;

	alarm_init_on_stack(&alarm, type, alarmtimer_nsleep_wakeup);

	return alarmtimer_do_nsleep(&alarm, exp, type);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

/**
 * alarm_timer_nsleep - alarmtimer nanosleep
 * @which_clock: clockid
<<<<<<< HEAD
 * @flags: determins abstime or relative
 * @tsreq: requested sleep time (abs or rel)
 * @rmtp: remaining sleep time saved
=======
 * @flags: determines abstime or relative
 * @tsreq: requested sleep time (abs or rel)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 *
 * Handles clock_nanosleep calls against _ALARM clockids
 */
static int alarm_timer_nsleep(const clockid_t which_clock, int flags,
<<<<<<< HEAD
		     struct timespec *tsreq, struct timespec __user *rmtp)
{
	enum  alarmtimer_type type = clock2alarm(which_clock);
	struct alarm alarm;
	ktime_t exp;
	int ret = 0;
	struct restart_block *restart;

	if (!alarmtimer_get_rtcdev())
		return -ENOTSUPP;
=======
			      const struct timespec64 *tsreq)
{
	enum  alarmtimer_type type = clock2alarm(which_clock);
	struct restart_block *restart = &current->restart_block;
	struct alarm alarm;
	ktime_t exp;
	int ret;

	if (!alarmtimer_get_rtcdev())
		return -EOPNOTSUPP;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	if (flags & ~TIMER_ABSTIME)
		return -EINVAL;

	if (!capable(CAP_WAKE_ALARM))
		return -EPERM;

<<<<<<< HEAD
	alarm_init(&alarm, type, alarmtimer_nsleep_wakeup);

	exp = timespec_to_ktime(*tsreq);
	/* Convert (if necessary) to absolute time */
	if (flags != TIMER_ABSTIME) {
		ktime_t now = alarm_bases[type].gettime();
		exp = ktime_add(now, exp);
	}

	if (alarmtimer_do_nsleep(&alarm, exp))
		goto out;

	if (freezing(current))
		alarmtimer_freezerset(exp, type);

	/* abs timers don't set remaining time or restart */
	if (flags == TIMER_ABSTIME) {
		ret = -ERESTARTNOHAND;
		goto out;
	}

	if (rmtp) {
		ret = update_rmtp(exp, type, rmtp);
		if (ret <= 0)
			goto out;
	}

	restart = &current_thread_info()->restart_block;
	restart->fn = alarm_timer_nsleep_restart;
	restart->nanosleep.clockid = type;
	restart->nanosleep.expires = exp.tv64;
	restart->nanosleep.rmtp = rmtp;
	ret = -ERESTART_RESTARTBLOCK;

out:
	return ret;
}

=======
	alarm_init_on_stack(&alarm, type, alarmtimer_nsleep_wakeup);

	exp = timespec64_to_ktime(*tsreq);
	/* Convert (if necessary) to absolute time */
	if (flags != TIMER_ABSTIME) {
		ktime_t now = alarm_bases[type].get_ktime();

		exp = ktime_add_safe(now, exp);
	} else {
		exp = timens_ktime_to_host(which_clock, exp);
	}

	ret = alarmtimer_do_nsleep(&alarm, exp, type);
	if (ret != -ERESTART_RESTARTBLOCK)
		return ret;

	/* abs timers don't set remaining time or restart */
	if (flags == TIMER_ABSTIME)
		return -ERESTARTNOHAND;

	restart->nanosleep.clockid = type;
	restart->nanosleep.expires = exp;
	set_restart_fn(restart, alarm_timer_nsleep_restart);
	return ret;
}

const struct k_clock alarm_clock = {
	.clock_getres		= alarm_clock_getres,
	.clock_get_ktime	= alarm_clock_get_ktime,
	.clock_get_timespec	= alarm_clock_get_timespec,
	.timer_create		= alarm_timer_create,
	.timer_set		= common_timer_set,
	.timer_del		= common_timer_del,
	.timer_get		= common_timer_get,
	.timer_arm		= alarm_timer_arm,
	.timer_rearm		= alarm_timer_rearm,
	.timer_forward		= alarm_timer_forward,
	.timer_remaining	= alarm_timer_remaining,
	.timer_try_to_cancel	= alarm_timer_try_to_cancel,
	.timer_wait_running	= alarm_timer_wait_running,
	.nsleep			= alarm_timer_nsleep,
};
#endif /* CONFIG_POSIX_TIMERS */

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

/* Suspend hook structures */
static const struct dev_pm_ops alarmtimer_pm_ops = {
	.suspend = alarmtimer_suspend,
	.resume = alarmtimer_resume,
};

static struct platform_driver alarmtimer_driver = {
	.driver = {
		.name = "alarmtimer",
		.pm = &alarmtimer_pm_ops,
	}
};

<<<<<<< HEAD
=======
static void get_boottime_timespec(struct timespec64 *tp)
{
	ktime_get_boottime_ts64(tp);
	timens_add_boottime(tp);
}

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
/**
 * alarmtimer_init - Initialize alarm timer code
 *
 * This function initializes the alarm bases and registers
 * the posix clock ids.
 */
static int __init alarmtimer_init(void)
{
<<<<<<< HEAD
	struct platform_device *pdev;
	int error = 0;
	int i;
	struct k_clock alarm_clock = {
		.clock_getres	= alarm_clock_getres,
		.clock_get	= alarm_clock_get,
		.timer_create	= alarm_timer_create,
		.timer_set	= alarm_timer_set,
		.timer_del	= alarm_timer_del,
		.timer_get	= alarm_timer_get,
		.nsleep		= alarm_timer_nsleep,
	};

	alarmtimer_rtc_timer_init();

	posix_timers_register_clock(CLOCK_REALTIME_ALARM, &alarm_clock);
	posix_timers_register_clock(CLOCK_BOOTTIME_ALARM, &alarm_clock);

	/* Initialize alarm bases */
	alarm_bases[ALARM_REALTIME].base_clockid = CLOCK_REALTIME;
	alarm_bases[ALARM_REALTIME].gettime = &ktime_get_real;
	alarm_bases[ALARM_BOOTTIME].base_clockid = CLOCK_BOOTTIME;
	alarm_bases[ALARM_BOOTTIME].gettime = &ktime_get_boottime;
=======
	int error;
	int i;

	alarmtimer_rtc_timer_init();

	/* Initialize alarm bases */
	alarm_bases[ALARM_REALTIME].base_clockid = CLOCK_REALTIME;
	alarm_bases[ALARM_REALTIME].get_ktime = &ktime_get_real;
	alarm_bases[ALARM_REALTIME].get_timespec = ktime_get_real_ts64;
	alarm_bases[ALARM_BOOTTIME].base_clockid = CLOCK_BOOTTIME;
	alarm_bases[ALARM_BOOTTIME].get_ktime = &ktime_get_boottime;
	alarm_bases[ALARM_BOOTTIME].get_timespec = get_boottime_timespec;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	for (i = 0; i < ALARM_NUMTYPE; i++) {
		timerqueue_init_head(&alarm_bases[i].timerqueue);
		spin_lock_init(&alarm_bases[i].lock);
	}

	error = alarmtimer_rtc_interface_setup();
	if (error)
		return error;

	error = platform_driver_register(&alarmtimer_driver);
	if (error)
		goto out_if;

<<<<<<< HEAD
	pdev = platform_device_register_simple("alarmtimer", -1, NULL, 0);
	if (IS_ERR(pdev)) {
		error = PTR_ERR(pdev);
		goto out_drv;
	}
	ws = wakeup_source_register("alarmtimer");
	return 0;

out_drv:
	platform_driver_unregister(&alarmtimer_driver);
=======
	return 0;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
out_if:
	alarmtimer_rtc_interface_remove();
	return error;
}
device_initcall(alarmtimer_init);
