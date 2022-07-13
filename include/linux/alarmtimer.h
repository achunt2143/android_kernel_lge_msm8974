<<<<<<< HEAD
=======
/* SPDX-License-Identifier: GPL-2.0 */
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#ifndef _LINUX_ALARMTIMER_H
#define _LINUX_ALARMTIMER_H

#include <linux/time.h>
#include <linux/hrtimer.h>
#include <linux/timerqueue.h>
<<<<<<< HEAD
#include <linux/rtc.h>
=======

struct rtc_device;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

enum alarmtimer_type {
	ALARM_REALTIME,
	ALARM_BOOTTIME,

<<<<<<< HEAD
	ALARM_NUMTYPE,
=======
	/* Supported types end here */
	ALARM_NUMTYPE,

	/* Used for tracing information. No usable types. */
	ALARM_REALTIME_FREEZER,
	ALARM_BOOTTIME_FREEZER,
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
};

enum alarmtimer_restart {
	ALARMTIMER_NORESTART,
	ALARMTIMER_RESTART,
};


#define ALARMTIMER_STATE_INACTIVE	0x00
#define ALARMTIMER_STATE_ENQUEUED	0x01
<<<<<<< HEAD
#define ALARMTIMER_STATE_CALLBACK	0x02
=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

/**
 * struct alarm - Alarm timer structure
 * @node:	timerqueue node for adding to the event list this value
 *		also includes the expiration time.
<<<<<<< HEAD
 * @period:	Period for recuring alarms
 * @function:	Function pointer to be executed when the timer fires.
 * @type:	Alarm type (BOOTTIME/REALTIME)
 * @enabled:	Flag that represents if the alarm is set to fire or not
=======
 * @timer:	hrtimer used to schedule events while running
 * @function:	Function pointer to be executed when the timer fires.
 * @type:	Alarm type (BOOTTIME/REALTIME).
 * @state:	Flag that represents if the alarm is set to fire or not.
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 * @data:	Internal data value.
 */
struct alarm {
	struct timerqueue_node	node;
	struct hrtimer		timer;
	enum alarmtimer_restart	(*function)(struct alarm *, ktime_t now);
	enum alarmtimer_type	type;
	int			state;
	void			*data;
};

void alarm_init(struct alarm *alarm, enum alarmtimer_type type,
		enum alarmtimer_restart (*function)(struct alarm *, ktime_t));
<<<<<<< HEAD
int alarm_start(struct alarm *alarm, ktime_t start);
int alarm_start_relative(struct alarm *alarm, ktime_t start);
void alarm_restart(struct alarm *alarm);
int alarm_try_to_cancel(struct alarm *alarm);
int alarm_cancel(struct alarm *alarm);
void set_power_on_alarm(long secs, bool enable);
void power_on_alarm_init(void);
=======
void alarm_start(struct alarm *alarm, ktime_t start);
void alarm_start_relative(struct alarm *alarm, ktime_t start);
void alarm_restart(struct alarm *alarm);
int alarm_try_to_cancel(struct alarm *alarm);
int alarm_cancel(struct alarm *alarm);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

u64 alarm_forward(struct alarm *alarm, ktime_t now, ktime_t interval);
u64 alarm_forward_now(struct alarm *alarm, ktime_t interval);
ktime_t alarm_expires_remaining(const struct alarm *alarm);

<<<<<<< HEAD
/*
 * A alarmtimer is active, when it is enqueued into timerqueue or the
 * callback function is running.
 */
static inline int alarmtimer_active(const struct alarm *timer)
{
	return timer->state != ALARMTIMER_STATE_INACTIVE;
}

/*
 * Helper function to check, whether the timer is on one of the queues
 */
static inline int alarmtimer_is_queued(struct alarm *timer)
{
	return timer->state & ALARMTIMER_STATE_ENQUEUED;
}

/*
 * Helper function to check, whether the timer is running the callback
 * function
 */
static inline int alarmtimer_callback_running(struct alarm *timer)
{
	return timer->state & ALARMTIMER_STATE_CALLBACK;
}


/* Provide way to access the rtc device being used by alarmtimers */
struct rtc_device *alarmtimer_get_rtcdev(void);
#ifdef CONFIG_RTC_DRV_QPNP
extern bool poweron_alarm;
=======
#ifdef CONFIG_RTC_CLASS
/* Provide way to access the rtc device being used by alarmtimers */
struct rtc_device *alarmtimer_get_rtcdev(void);
#else
static inline struct rtc_device *alarmtimer_get_rtcdev(void) { return NULL; }
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#endif

#endif
