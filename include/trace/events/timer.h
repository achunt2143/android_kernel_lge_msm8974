<<<<<<< HEAD
=======
/* SPDX-License-Identifier: GPL-2.0 */
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#undef TRACE_SYSTEM
#define TRACE_SYSTEM timer

#if !defined(_TRACE_TIMER_H) || defined(TRACE_HEADER_MULTI_READ)
#define _TRACE_TIMER_H

#include <linux/tracepoint.h>
#include <linux/hrtimer.h>
#include <linux/timer.h>

DECLARE_EVENT_CLASS(timer_class,

	TP_PROTO(struct timer_list *timer),

	TP_ARGS(timer),

	TP_STRUCT__entry(
		__field( void *,	timer	)
	),

	TP_fast_assign(
		__entry->timer	= timer;
	),

	TP_printk("timer=%p", __entry->timer)
);

/**
 * timer_init - called when the timer is initialized
 * @timer:	pointer to struct timer_list
 */
DEFINE_EVENT(timer_class, timer_init,

	TP_PROTO(struct timer_list *timer),

	TP_ARGS(timer)
);

<<<<<<< HEAD
/**
 * timer_start - called when the timer is started
 * @timer:	pointer to struct timer_list
 * @expires:	the timers expiry time
=======
#define decode_timer_flags(flags)			\
	__print_flags(flags, "|",			\
		{  TIMER_MIGRATING,	"M" },		\
		{  TIMER_DEFERRABLE,	"D" },		\
		{  TIMER_PINNED,	"P" },		\
		{  TIMER_IRQSAFE,	"I" })

/**
 * timer_start - called when the timer is started
 * @timer:		pointer to struct timer_list
 * @bucket_expiry:	the bucket expiry time
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 */
TRACE_EVENT(timer_start,

	TP_PROTO(struct timer_list *timer,
<<<<<<< HEAD
		 unsigned long expires, char deferrable),

	TP_ARGS(timer, expires, deferrable),
=======
		unsigned long bucket_expiry),

	TP_ARGS(timer, bucket_expiry),
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	TP_STRUCT__entry(
		__field( void *,	timer		)
		__field( void *,	function	)
		__field( unsigned long,	expires		)
<<<<<<< HEAD
		__field( unsigned long,	now		)
		__field(char,	deferrable)
=======
		__field( unsigned long,	bucket_expiry	)
		__field( unsigned long,	now		)
		__field( unsigned int,	flags		)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	),

	TP_fast_assign(
		__entry->timer		= timer;
		__entry->function	= timer->function;
<<<<<<< HEAD
		__entry->expires	= expires;
		__entry->now		= jiffies;
		__entry->deferrable     = deferrable;
	),

	TP_printk("timer=%p function=%pf expires=%lu [timeout=%ld] defer=%c",
		  __entry->timer, __entry->function, __entry->expires,
		  (long)__entry->expires - __entry->now, __entry->deferrable)
=======
		__entry->expires	= timer->expires;
		__entry->bucket_expiry	= bucket_expiry;
		__entry->now		= jiffies;
		__entry->flags		= timer->flags;
	),

	TP_printk("timer=%p function=%ps expires=%lu [timeout=%ld] bucket_expiry=%lu cpu=%u idx=%u flags=%s",
		  __entry->timer, __entry->function, __entry->expires,
		  (long)__entry->expires - __entry->now,
		  __entry->bucket_expiry, __entry->flags & TIMER_CPUMASK,
		  __entry->flags >> TIMER_ARRAYSHIFT,
		  decode_timer_flags(__entry->flags & TIMER_TRACE_FLAGMASK))
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
);

/**
 * timer_expire_entry - called immediately before the timer callback
 * @timer:	pointer to struct timer_list
<<<<<<< HEAD
=======
 * @baseclk:	value of timer_base::clk when timer expires
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 *
 * Allows to determine the timer latency.
 */
TRACE_EVENT(timer_expire_entry,

<<<<<<< HEAD
	TP_PROTO(struct timer_list *timer),

	TP_ARGS(timer),
=======
	TP_PROTO(struct timer_list *timer, unsigned long baseclk),

	TP_ARGS(timer, baseclk),
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	TP_STRUCT__entry(
		__field( void *,	timer	)
		__field( unsigned long,	now	)
		__field( void *,	function)
<<<<<<< HEAD
=======
		__field( unsigned long,	baseclk	)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	),

	TP_fast_assign(
		__entry->timer		= timer;
		__entry->now		= jiffies;
		__entry->function	= timer->function;
<<<<<<< HEAD
	),

	TP_printk("timer=%p function=%pf now=%lu", __entry->timer, __entry->function,__entry->now)
=======
		__entry->baseclk	= baseclk;
	),

	TP_printk("timer=%p function=%ps now=%lu baseclk=%lu",
		  __entry->timer, __entry->function, __entry->now,
		  __entry->baseclk)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
);

/**
 * timer_expire_exit - called immediately after the timer callback returns
 * @timer:	pointer to struct timer_list
 *
 * When used in combination with the timer_expire_entry tracepoint we can
 * determine the runtime of the timer callback function.
 *
<<<<<<< HEAD
 * NOTE: Do NOT derefernce timer in TP_fast_assign. The pointer might
=======
 * NOTE: Do NOT dereference timer in TP_fast_assign. The pointer might
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 * be invalid. We solely track the pointer.
 */
DEFINE_EVENT(timer_class, timer_expire_exit,

	TP_PROTO(struct timer_list *timer),

	TP_ARGS(timer)
);

/**
 * timer_cancel - called when the timer is canceled
 * @timer:	pointer to struct timer_list
 */
DEFINE_EVENT(timer_class, timer_cancel,

	TP_PROTO(struct timer_list *timer),

	TP_ARGS(timer)
);

<<<<<<< HEAD
/**
 * hrtimer_init - called when the hrtimer is initialized
 * @timer:	pointer to struct hrtimer
=======
TRACE_EVENT(timer_base_idle,

	TP_PROTO(bool is_idle, unsigned int cpu),

	TP_ARGS(is_idle, cpu),

	TP_STRUCT__entry(
		__field( bool,		is_idle	)
		__field( unsigned int,	cpu	)
	),

	TP_fast_assign(
		__entry->is_idle	= is_idle;
		__entry->cpu		= cpu;
	),

	TP_printk("is_idle=%d cpu=%d",
		  __entry->is_idle, __entry->cpu)
);

#define decode_clockid(type)						\
	__print_symbolic(type,						\
		{ CLOCK_REALTIME,	"CLOCK_REALTIME"	},	\
		{ CLOCK_MONOTONIC,	"CLOCK_MONOTONIC"	},	\
		{ CLOCK_BOOTTIME,	"CLOCK_BOOTTIME"	},	\
		{ CLOCK_TAI,		"CLOCK_TAI"		})

#define decode_hrtimer_mode(mode)					\
	__print_symbolic(mode,						\
		{ HRTIMER_MODE_ABS,		"ABS"		},	\
		{ HRTIMER_MODE_REL,		"REL"		},	\
		{ HRTIMER_MODE_ABS_PINNED,	"ABS|PINNED"	},	\
		{ HRTIMER_MODE_REL_PINNED,	"REL|PINNED"	},	\
		{ HRTIMER_MODE_ABS_SOFT,	"ABS|SOFT"	},	\
		{ HRTIMER_MODE_REL_SOFT,	"REL|SOFT"	},	\
		{ HRTIMER_MODE_ABS_PINNED_SOFT,	"ABS|PINNED|SOFT" },	\
		{ HRTIMER_MODE_REL_PINNED_SOFT,	"REL|PINNED|SOFT" },	\
		{ HRTIMER_MODE_ABS_HARD,	"ABS|HARD" },		\
		{ HRTIMER_MODE_REL_HARD,	"REL|HARD" },		\
		{ HRTIMER_MODE_ABS_PINNED_HARD, "ABS|PINNED|HARD" },	\
		{ HRTIMER_MODE_REL_PINNED_HARD,	"REL|PINNED|HARD" })

/**
 * hrtimer_init - called when the hrtimer is initialized
 * @hrtimer:	pointer to struct hrtimer
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 * @clockid:	the hrtimers clock
 * @mode:	the hrtimers mode
 */
TRACE_EVENT(hrtimer_init,

	TP_PROTO(struct hrtimer *hrtimer, clockid_t clockid,
		 enum hrtimer_mode mode),

	TP_ARGS(hrtimer, clockid, mode),

	TP_STRUCT__entry(
		__field( void *,		hrtimer		)
		__field( clockid_t,		clockid		)
		__field( enum hrtimer_mode,	mode		)
	),

	TP_fast_assign(
		__entry->hrtimer	= hrtimer;
		__entry->clockid	= clockid;
		__entry->mode		= mode;
	),

	TP_printk("hrtimer=%p clockid=%s mode=%s", __entry->hrtimer,
<<<<<<< HEAD
		  __entry->clockid == CLOCK_REALTIME ?
			"CLOCK_REALTIME" : "CLOCK_MONOTONIC",
		  __entry->mode == HRTIMER_MODE_ABS ?
			"HRTIMER_MODE_ABS" : "HRTIMER_MODE_REL")
=======
		  decode_clockid(__entry->clockid),
		  decode_hrtimer_mode(__entry->mode))
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
);

/**
 * hrtimer_start - called when the hrtimer is started
<<<<<<< HEAD
 * @timer: pointer to struct hrtimer
 */
TRACE_EVENT(hrtimer_start,

	TP_PROTO(struct hrtimer *hrtimer),

	TP_ARGS(hrtimer),
=======
 * @hrtimer:	pointer to struct hrtimer
 * @mode:	the hrtimers mode
 */
TRACE_EVENT(hrtimer_start,

	TP_PROTO(struct hrtimer *hrtimer, enum hrtimer_mode mode),

	TP_ARGS(hrtimer, mode),
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	TP_STRUCT__entry(
		__field( void *,	hrtimer		)
		__field( void *,	function	)
		__field( s64,		expires		)
		__field( s64,		softexpires	)
<<<<<<< HEAD
=======
		__field( enum hrtimer_mode,	mode	)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	),

	TP_fast_assign(
		__entry->hrtimer	= hrtimer;
		__entry->function	= hrtimer->function;
<<<<<<< HEAD
		__entry->expires	= hrtimer_get_expires(hrtimer).tv64;
		__entry->softexpires	= hrtimer_get_softexpires(hrtimer).tv64;
	),

	TP_printk("hrtimer=%p function=%pf expires=%llu softexpires=%llu",
		  __entry->hrtimer, __entry->function,
		  (unsigned long long)ktime_to_ns((ktime_t) {
				  .tv64 = __entry->expires }),
		  (unsigned long long)ktime_to_ns((ktime_t) {
				  .tv64 = __entry->softexpires }))
);

/**
 * htimmer_expire_entry - called immediately before the hrtimer callback
 * @timer:	pointer to struct hrtimer
=======
		__entry->expires	= hrtimer_get_expires(hrtimer);
		__entry->softexpires	= hrtimer_get_softexpires(hrtimer);
		__entry->mode		= mode;
	),

	TP_printk("hrtimer=%p function=%ps expires=%llu softexpires=%llu "
		  "mode=%s", __entry->hrtimer, __entry->function,
		  (unsigned long long) __entry->expires,
		  (unsigned long long) __entry->softexpires,
		  decode_hrtimer_mode(__entry->mode))
);

/**
 * hrtimer_expire_entry - called immediately before the hrtimer callback
 * @hrtimer:	pointer to struct hrtimer
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 * @now:	pointer to variable which contains current time of the
 *		timers base.
 *
 * Allows to determine the timer latency.
 */
TRACE_EVENT(hrtimer_expire_entry,

	TP_PROTO(struct hrtimer *hrtimer, ktime_t *now),

	TP_ARGS(hrtimer, now),

	TP_STRUCT__entry(
		__field( void *,	hrtimer	)
		__field( s64,		now	)
		__field( void *,	function)
	),

	TP_fast_assign(
		__entry->hrtimer	= hrtimer;
<<<<<<< HEAD
		__entry->now		= now->tv64;
		__entry->function	= hrtimer->function;
	),

	TP_printk("hrtimer=%p function=%pf now=%llu", __entry->hrtimer, __entry->function,
		  (unsigned long long)ktime_to_ns((ktime_t) { .tv64 = __entry->now }))
 );
=======
		__entry->now		= *now;
		__entry->function	= hrtimer->function;
	),

	TP_printk("hrtimer=%p function=%ps now=%llu",
		  __entry->hrtimer, __entry->function,
		  (unsigned long long) __entry->now)
);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

DECLARE_EVENT_CLASS(hrtimer_class,

	TP_PROTO(struct hrtimer *hrtimer),

	TP_ARGS(hrtimer),

	TP_STRUCT__entry(
		__field( void *,	hrtimer	)
	),

	TP_fast_assign(
		__entry->hrtimer	= hrtimer;
	),

	TP_printk("hrtimer=%p", __entry->hrtimer)
);

/**
 * hrtimer_expire_exit - called immediately after the hrtimer callback returns
<<<<<<< HEAD
 * @timer:	pointer to struct hrtimer
=======
 * @hrtimer:	pointer to struct hrtimer
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 *
 * When used in combination with the hrtimer_expire_entry tracepoint we can
 * determine the runtime of the callback function.
 */
DEFINE_EVENT(hrtimer_class, hrtimer_expire_exit,

	TP_PROTO(struct hrtimer *hrtimer),

	TP_ARGS(hrtimer)
);

/**
 * hrtimer_cancel - called when the hrtimer is canceled
 * @hrtimer:	pointer to struct hrtimer
 */
DEFINE_EVENT(hrtimer_class, hrtimer_cancel,

	TP_PROTO(struct hrtimer *hrtimer),

	TP_ARGS(hrtimer)
);

/**
 * itimer_state - called when itimer is started or canceled
 * @which:	name of the interval timer
 * @value:	the itimers value, itimer is canceled if value->it_value is
 *		zero, otherwise it is started
 * @expires:	the itimers expiry time
 */
TRACE_EVENT(itimer_state,

<<<<<<< HEAD
	TP_PROTO(int which, const struct itimerval *const value,
		 cputime_t expires),
=======
	TP_PROTO(int which, const struct itimerspec64 *const value,
		 unsigned long long expires),
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	TP_ARGS(which, value, expires),

	TP_STRUCT__entry(
<<<<<<< HEAD
		__field(	int,		which		)
		__field(	cputime_t,	expires		)
		__field(	long,		value_sec	)
		__field(	long,		value_usec	)
		__field(	long,		interval_sec	)
		__field(	long,		interval_usec	)
=======
		__field(	int,			which		)
		__field(	unsigned long long,	expires		)
		__field(	long,			value_sec	)
		__field(	long,			value_nsec	)
		__field(	long,			interval_sec	)
		__field(	long,			interval_nsec	)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	),

	TP_fast_assign(
		__entry->which		= which;
		__entry->expires	= expires;
		__entry->value_sec	= value->it_value.tv_sec;
<<<<<<< HEAD
		__entry->value_usec	= value->it_value.tv_usec;
		__entry->interval_sec	= value->it_interval.tv_sec;
		__entry->interval_usec	= value->it_interval.tv_usec;
	),

	TP_printk("which=%d expires=%llu it_value=%ld.%ld it_interval=%ld.%ld",
		  __entry->which, (unsigned long long)__entry->expires,
		  __entry->value_sec, __entry->value_usec,
		  __entry->interval_sec, __entry->interval_usec)
=======
		__entry->value_nsec	= value->it_value.tv_nsec;
		__entry->interval_sec	= value->it_interval.tv_sec;
		__entry->interval_nsec	= value->it_interval.tv_nsec;
	),

	TP_printk("which=%d expires=%llu it_value=%ld.%06ld it_interval=%ld.%06ld",
		  __entry->which, __entry->expires,
		  __entry->value_sec, __entry->value_nsec / NSEC_PER_USEC,
		  __entry->interval_sec, __entry->interval_nsec / NSEC_PER_USEC)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
);

/**
 * itimer_expire - called when itimer expires
 * @which:	type of the interval timer
 * @pid:	pid of the process which owns the timer
 * @now:	current time, used to calculate the latency of itimer
 */
TRACE_EVENT(itimer_expire,

<<<<<<< HEAD
	TP_PROTO(int which, struct pid *pid, cputime_t now),
=======
	TP_PROTO(int which, struct pid *pid, unsigned long long now),
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	TP_ARGS(which, pid, now),

	TP_STRUCT__entry(
<<<<<<< HEAD
		__field( int ,		which	)
		__field( pid_t,		pid	)
		__field( cputime_t,	now	)
=======
		__field( int ,			which	)
		__field( pid_t,			pid	)
		__field( unsigned long long,	now	)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	),

	TP_fast_assign(
		__entry->which	= which;
		__entry->now	= now;
		__entry->pid	= pid_nr(pid);
	),

	TP_printk("which=%d pid=%d now=%llu", __entry->which,
<<<<<<< HEAD
		  (int) __entry->pid, (unsigned long long)__entry->now)
);

=======
		  (int) __entry->pid, __entry->now)
);

#ifdef CONFIG_NO_HZ_COMMON

#define TICK_DEP_NAMES					\
		tick_dep_mask_name(NONE)		\
		tick_dep_name(POSIX_TIMER)		\
		tick_dep_name(PERF_EVENTS)		\
		tick_dep_name(SCHED)			\
		tick_dep_name(CLOCK_UNSTABLE)		\
		tick_dep_name(RCU)			\
		tick_dep_name_end(RCU_EXP)

#undef tick_dep_name
#undef tick_dep_mask_name
#undef tick_dep_name_end

/* The MASK will convert to their bits and they need to be processed too */
#define tick_dep_name(sdep) TRACE_DEFINE_ENUM(TICK_DEP_BIT_##sdep); \
	TRACE_DEFINE_ENUM(TICK_DEP_MASK_##sdep);
#define tick_dep_name_end(sdep)  TRACE_DEFINE_ENUM(TICK_DEP_BIT_##sdep); \
	TRACE_DEFINE_ENUM(TICK_DEP_MASK_##sdep);
/* NONE only has a mask defined for it */
#define tick_dep_mask_name(sdep) TRACE_DEFINE_ENUM(TICK_DEP_MASK_##sdep);

TICK_DEP_NAMES

#undef tick_dep_name
#undef tick_dep_mask_name
#undef tick_dep_name_end

#define tick_dep_name(sdep) { TICK_DEP_MASK_##sdep, #sdep },
#define tick_dep_mask_name(sdep) { TICK_DEP_MASK_##sdep, #sdep },
#define tick_dep_name_end(sdep) { TICK_DEP_MASK_##sdep, #sdep }

#define show_tick_dep_name(val)				\
	__print_symbolic(val, TICK_DEP_NAMES)

TRACE_EVENT(tick_stop,

	TP_PROTO(int success, int dependency),

	TP_ARGS(success, dependency),

	TP_STRUCT__entry(
		__field( int ,		success	)
		__field( int ,		dependency )
	),

	TP_fast_assign(
		__entry->success	= success;
		__entry->dependency	= dependency;
	),

	TP_printk("success=%d dependency=%s",  __entry->success, \
			show_tick_dep_name(__entry->dependency))
);
#endif

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#endif /*  _TRACE_TIMER_H */

/* This part must be outside protection */
#include <trace/define_trace.h>
