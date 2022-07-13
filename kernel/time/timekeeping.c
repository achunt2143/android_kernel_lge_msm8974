<<<<<<< HEAD
/*
 *  linux/kernel/time/timekeeping.c
 *
 *  Kernel timekeeping code and accessor functions
 *
 *  This code was moved from linux/kernel/timer.c.
 *  Please see that file for copyright and history logs.
 *
 */

=======
// SPDX-License-Identifier: GPL-2.0
/*
 *  Kernel timekeeping code and accessor functions. Based on code from
 *  timer.c, moved in commit 8524070b7982.
 */
#include <linux/timekeeper_internal.h>
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#include <linux/module.h>
#include <linux/interrupt.h>
#include <linux/percpu.h>
#include <linux/init.h>
#include <linux/mm.h>
<<<<<<< HEAD
#include <linux/sched.h>
=======
#include <linux/nmi.h>
#include <linux/sched.h>
#include <linux/sched/loadavg.h>
#include <linux/sched/clock.h>
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#include <linux/syscore_ops.h>
#include <linux/clocksource.h>
#include <linux/jiffies.h>
#include <linux/time.h>
<<<<<<< HEAD
#include <linux/tick.h>
#include <linux/stop_machine.h>

extern ktime_t ntp_get_next_leap(void);
extern int __do_adjtimex(struct timex *);

/* Structure holding internal timekeeping values. */
struct timekeeper {
	/* Current clocksource used for timekeeping. */
	struct clocksource *clock;
	/* NTP adjusted clock multiplier */
	u32	mult;
	/* The shift value of the current clocksource. */
	int	shift;

	/* CLOCK_MONOTONIC time value of a pending leap-second*/
	ktime_t	next_leap_ktime;
	/* Number of clock cycles in one NTP interval. */
	cycle_t cycle_interval;
	/* Number of clock shifted nano seconds in one NTP interval. */
	u64	xtime_interval;
	/* shifted nano seconds left over when rounding cycle_interval */
	s64	xtime_remainder;
	/* Raw nano seconds accumulated per NTP interval. */
	u32	raw_interval;

	/* Clock shifted nano seconds remainder not stored in xtime.tv_nsec. */
	u64	xtime_nsec;
	/* Difference between accumulated time and NTP time in ntp
	 * shifted nano seconds. */
	s64	ntp_error;
	/* Shift conversion between clock shifted nano seconds and
	 * ntp shifted nano seconds. */
	int	ntp_error_shift;

	/* The current time */
	struct timespec xtime;
	/*
	 * wall_to_monotonic is what we need to add to xtime (or xtime corrected
	 * for sub jiffie times) to get to monotonic time.  Monotonic is pegged
	 * at zero at system boot time, so wall_to_monotonic will be negative,
	 * however, we will ALWAYS keep the tv_nsec part positive so we can use
	 * the usual normalization.
	 *
	 * wall_to_monotonic is moved after resume from suspend for the
	 * monotonic time not to jump. We need to add total_sleep_time to
	 * wall_to_monotonic to get the real boot based time offset.
	 *
	 * - wall_to_monotonic is no longer the boot time, getboottime must be
	 * used instead.
	 */
	struct timespec wall_to_monotonic;
	/* time spent in suspend */
	struct timespec total_sleep_time;
	/* The raw monotonic time for the CLOCK_MONOTONIC_RAW posix clock. */
	struct timespec raw_time;

	/* Offset clock monotonic -> clock realtime */
	ktime_t offs_real;

	/* Offset clock monotonic -> clock boottime */
	ktime_t offs_boot;

	/* Seqlock for all timekeeper values */
	seqlock_t lock;
};

static struct timekeeper timekeeper;

/*
 * This read-write spinlock protects us from races in SMP while
 * playing with xtime.
 */
__cacheline_aligned_in_smp DEFINE_SEQLOCK(xtime_lock);

=======
#include <linux/timex.h>
#include <linux/tick.h>
#include <linux/stop_machine.h>
#include <linux/pvclock_gtod.h>
#include <linux/compiler.h>
#include <linux/audit.h>
#include <linux/random.h>

#include "tick-internal.h"
#include "ntp_internal.h"
#include "timekeeping_internal.h"

#define TK_CLEAR_NTP		(1 << 0)
#define TK_MIRROR		(1 << 1)
#define TK_CLOCK_WAS_SET	(1 << 2)

enum timekeeping_adv_mode {
	/* Update timekeeper when a tick has passed */
	TK_ADV_TICK,

	/* Update timekeeper on a direct frequency change */
	TK_ADV_FREQ
};

DEFINE_RAW_SPINLOCK(timekeeper_lock);

/*
 * The most important data for readout fits into a single 64 byte
 * cache line.
 */
static struct {
	seqcount_raw_spinlock_t	seq;
	struct timekeeper	timekeeper;
} tk_core ____cacheline_aligned = {
	.seq = SEQCNT_RAW_SPINLOCK_ZERO(tk_core.seq, &timekeeper_lock),
};

static struct timekeeper shadow_timekeeper;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

/* flag for if timekeeping is suspended */
int __read_mostly timekeeping_suspended;

<<<<<<< HEAD


/**
 * timekeeper_setup_internals - Set up internals to use clocksource clock.
 *
=======
/**
 * struct tk_fast - NMI safe timekeeper
 * @seq:	Sequence counter for protecting updates. The lowest bit
 *		is the index for the tk_read_base array
 * @base:	tk_read_base array. Access is indexed by the lowest bit of
 *		@seq.
 *
 * See @update_fast_timekeeper() below.
 */
struct tk_fast {
	seqcount_latch_t	seq;
	struct tk_read_base	base[2];
};

/* Suspend-time cycles value for halted fast timekeeper. */
static u64 cycles_at_suspend;

static u64 dummy_clock_read(struct clocksource *cs)
{
	if (timekeeping_suspended)
		return cycles_at_suspend;
	return local_clock();
}

static struct clocksource dummy_clock = {
	.read = dummy_clock_read,
};

/*
 * Boot time initialization which allows local_clock() to be utilized
 * during early boot when clocksources are not available. local_clock()
 * returns nanoseconds already so no conversion is required, hence mult=1
 * and shift=0. When the first proper clocksource is installed then
 * the fast time keepers are updated with the correct values.
 */
#define FAST_TK_INIT						\
	{							\
		.clock		= &dummy_clock,			\
		.mask		= CLOCKSOURCE_MASK(64),		\
		.mult		= 1,				\
		.shift		= 0,				\
	}

static struct tk_fast tk_fast_mono ____cacheline_aligned = {
	.seq     = SEQCNT_LATCH_ZERO(tk_fast_mono.seq),
	.base[0] = FAST_TK_INIT,
	.base[1] = FAST_TK_INIT,
};

static struct tk_fast tk_fast_raw  ____cacheline_aligned = {
	.seq     = SEQCNT_LATCH_ZERO(tk_fast_raw.seq),
	.base[0] = FAST_TK_INIT,
	.base[1] = FAST_TK_INIT,
};

static inline void tk_normalize_xtime(struct timekeeper *tk)
{
	while (tk->tkr_mono.xtime_nsec >= ((u64)NSEC_PER_SEC << tk->tkr_mono.shift)) {
		tk->tkr_mono.xtime_nsec -= (u64)NSEC_PER_SEC << tk->tkr_mono.shift;
		tk->xtime_sec++;
	}
	while (tk->tkr_raw.xtime_nsec >= ((u64)NSEC_PER_SEC << tk->tkr_raw.shift)) {
		tk->tkr_raw.xtime_nsec -= (u64)NSEC_PER_SEC << tk->tkr_raw.shift;
		tk->raw_sec++;
	}
}

static inline struct timespec64 tk_xtime(const struct timekeeper *tk)
{
	struct timespec64 ts;

	ts.tv_sec = tk->xtime_sec;
	ts.tv_nsec = (long)(tk->tkr_mono.xtime_nsec >> tk->tkr_mono.shift);
	return ts;
}

static void tk_set_xtime(struct timekeeper *tk, const struct timespec64 *ts)
{
	tk->xtime_sec = ts->tv_sec;
	tk->tkr_mono.xtime_nsec = (u64)ts->tv_nsec << tk->tkr_mono.shift;
}

static void tk_xtime_add(struct timekeeper *tk, const struct timespec64 *ts)
{
	tk->xtime_sec += ts->tv_sec;
	tk->tkr_mono.xtime_nsec += (u64)ts->tv_nsec << tk->tkr_mono.shift;
	tk_normalize_xtime(tk);
}

static void tk_set_wall_to_mono(struct timekeeper *tk, struct timespec64 wtm)
{
	struct timespec64 tmp;

	/*
	 * Verify consistency of: offset_real = -wall_to_monotonic
	 * before modifying anything
	 */
	set_normalized_timespec64(&tmp, -tk->wall_to_monotonic.tv_sec,
					-tk->wall_to_monotonic.tv_nsec);
	WARN_ON_ONCE(tk->offs_real != timespec64_to_ktime(tmp));
	tk->wall_to_monotonic = wtm;
	set_normalized_timespec64(&tmp, -wtm.tv_sec, -wtm.tv_nsec);
	tk->offs_real = timespec64_to_ktime(tmp);
	tk->offs_tai = ktime_add(tk->offs_real, ktime_set(tk->tai_offset, 0));
}

static inline void tk_update_sleep_time(struct timekeeper *tk, ktime_t delta)
{
	tk->offs_boot = ktime_add(tk->offs_boot, delta);
	/*
	 * Timespec representation for VDSO update to avoid 64bit division
	 * on every update.
	 */
	tk->monotonic_to_boot = ktime_to_timespec64(tk->offs_boot);
}

/*
 * tk_clock_read - atomic clocksource read() helper
 *
 * This helper is necessary to use in the read paths because, while the
 * seqcount ensures we don't return a bad value while structures are updated,
 * it doesn't protect from potential crashes. There is the possibility that
 * the tkr's clocksource may change between the read reference, and the
 * clock reference passed to the read function.  This can cause crashes if
 * the wrong clocksource is passed to the wrong read function.
 * This isn't necessary to use when holding the timekeeper_lock or doing
 * a read of the fast-timekeeper tkrs (which is protected by its own locking
 * and update logic).
 */
static inline u64 tk_clock_read(const struct tk_read_base *tkr)
{
	struct clocksource *clock = READ_ONCE(tkr->clock);

	return clock->read(clock);
}

#ifdef CONFIG_DEBUG_TIMEKEEPING
#define WARNING_FREQ (HZ*300) /* 5 minute rate-limiting */

static void timekeeping_check_update(struct timekeeper *tk, u64 offset)
{

	u64 max_cycles = tk->tkr_mono.clock->max_cycles;
	const char *name = tk->tkr_mono.clock->name;

	if (offset > max_cycles) {
		printk_deferred("WARNING: timekeeping: Cycle offset (%lld) is larger than allowed by the '%s' clock's max_cycles value (%lld): time overflow danger\n",
				offset, name, max_cycles);
		printk_deferred("         timekeeping: Your kernel is sick, but tries to cope by capping time updates\n");
	} else {
		if (offset > (max_cycles >> 1)) {
			printk_deferred("INFO: timekeeping: Cycle offset (%lld) is larger than the '%s' clock's 50%% safety margin (%lld)\n",
					offset, name, max_cycles >> 1);
			printk_deferred("      timekeeping: Your kernel is still fine, but is feeling a bit nervous\n");
		}
	}

	if (tk->underflow_seen) {
		if (jiffies - tk->last_warning > WARNING_FREQ) {
			printk_deferred("WARNING: Underflow in clocksource '%s' observed, time update ignored.\n", name);
			printk_deferred("         Please report this, consider using a different clocksource, if possible.\n");
			printk_deferred("         Your kernel is probably still fine.\n");
			tk->last_warning = jiffies;
		}
		tk->underflow_seen = 0;
	}

	if (tk->overflow_seen) {
		if (jiffies - tk->last_warning > WARNING_FREQ) {
			printk_deferred("WARNING: Overflow in clocksource '%s' observed, time update capped.\n", name);
			printk_deferred("         Please report this, consider using a different clocksource, if possible.\n");
			printk_deferred("         Your kernel is probably still fine.\n");
			tk->last_warning = jiffies;
		}
		tk->overflow_seen = 0;
	}
}

static inline u64 timekeeping_get_delta(const struct tk_read_base *tkr)
{
	struct timekeeper *tk = &tk_core.timekeeper;
	u64 now, last, mask, max, delta;
	unsigned int seq;

	/*
	 * Since we're called holding a seqcount, the data may shift
	 * under us while we're doing the calculation. This can cause
	 * false positives, since we'd note a problem but throw the
	 * results away. So nest another seqcount here to atomically
	 * grab the points we are checking with.
	 */
	do {
		seq = read_seqcount_begin(&tk_core.seq);
		now = tk_clock_read(tkr);
		last = tkr->cycle_last;
		mask = tkr->mask;
		max = tkr->clock->max_cycles;
	} while (read_seqcount_retry(&tk_core.seq, seq));

	delta = clocksource_delta(now, last, mask);

	/*
	 * Try to catch underflows by checking if we are seeing small
	 * mask-relative negative values.
	 */
	if (unlikely((~delta & mask) < (mask >> 3))) {
		tk->underflow_seen = 1;
		delta = 0;
	}

	/* Cap delta value to the max_cycles values to avoid mult overflows */
	if (unlikely(delta > max)) {
		tk->overflow_seen = 1;
		delta = tkr->clock->max_cycles;
	}

	return delta;
}
#else
static inline void timekeeping_check_update(struct timekeeper *tk, u64 offset)
{
}
static inline u64 timekeeping_get_delta(const struct tk_read_base *tkr)
{
	u64 cycle_now, delta;

	/* read clocksource */
	cycle_now = tk_clock_read(tkr);

	/* calculate the delta since the last update_wall_time */
	delta = clocksource_delta(cycle_now, tkr->cycle_last, tkr->mask);

	return delta;
}
#endif

/**
 * tk_setup_internals - Set up internals to use clocksource clock.
 *
 * @tk:		The target timekeeper to setup.
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 * @clock:		Pointer to clocksource.
 *
 * Calculates a fixed cycle/nsec interval for a given clocksource/adjustment
 * pair and interval request.
 *
 * Unless you're the timekeeping code, you should not be using this!
 */
<<<<<<< HEAD
static void timekeeper_setup_internals(struct clocksource *clock)
{
	cycle_t interval;
	u64 tmp, ntpinterval;

	timekeeper.clock = clock;
	clock->cycle_last = clock->read(clock);
=======
static void tk_setup_internals(struct timekeeper *tk, struct clocksource *clock)
{
	u64 interval;
	u64 tmp, ntpinterval;
	struct clocksource *old_clock;

	++tk->cs_was_changed_seq;
	old_clock = tk->tkr_mono.clock;
	tk->tkr_mono.clock = clock;
	tk->tkr_mono.mask = clock->mask;
	tk->tkr_mono.cycle_last = tk_clock_read(&tk->tkr_mono);

	tk->tkr_raw.clock = clock;
	tk->tkr_raw.mask = clock->mask;
	tk->tkr_raw.cycle_last = tk->tkr_mono.cycle_last;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	/* Do the ns -> cycle conversion first, using original mult */
	tmp = NTP_INTERVAL_LENGTH;
	tmp <<= clock->shift;
	ntpinterval = tmp;
	tmp += clock->mult/2;
	do_div(tmp, clock->mult);
	if (tmp == 0)
		tmp = 1;

<<<<<<< HEAD
	interval = (cycle_t) tmp;
	timekeeper.cycle_interval = interval;

	/* Go back from cycles -> shifted ns */
	timekeeper.xtime_interval = (u64) interval * clock->mult;
	timekeeper.xtime_remainder = ntpinterval - timekeeper.xtime_interval;
	timekeeper.raw_interval =
		((u64) interval * clock->mult) >> clock->shift;

	timekeeper.xtime_nsec = 0;
	timekeeper.shift = clock->shift;

	timekeeper.ntp_error = 0;
	timekeeper.ntp_error_shift = NTP_SCALE_SHIFT - clock->shift;
=======
	interval = (u64) tmp;
	tk->cycle_interval = interval;

	/* Go back from cycles -> shifted ns */
	tk->xtime_interval = interval * clock->mult;
	tk->xtime_remainder = ntpinterval - tk->xtime_interval;
	tk->raw_interval = interval * clock->mult;

	 /* if changing clocks, convert xtime_nsec shift units */
	if (old_clock) {
		int shift_change = clock->shift - old_clock->shift;
		if (shift_change < 0) {
			tk->tkr_mono.xtime_nsec >>= -shift_change;
			tk->tkr_raw.xtime_nsec >>= -shift_change;
		} else {
			tk->tkr_mono.xtime_nsec <<= shift_change;
			tk->tkr_raw.xtime_nsec <<= shift_change;
		}
	}

	tk->tkr_mono.shift = clock->shift;
	tk->tkr_raw.shift = clock->shift;

	tk->ntp_error = 0;
	tk->ntp_error_shift = NTP_SCALE_SHIFT - clock->shift;
	tk->ntp_tick = ntpinterval << tk->ntp_error_shift;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	/*
	 * The timekeeper keeps its own mult values for the currently
	 * active clocksource. These value will be adjusted via NTP
	 * to counteract clock drifting.
	 */
<<<<<<< HEAD
	timekeeper.mult = clock->mult;
}

/* Timekeeper helper functions. */
static inline s64 timekeeping_get_ns(void)
{
	cycle_t cycle_now, cycle_delta;
	struct clocksource *clock;

	/* read clocksource: */
	clock = timekeeper.clock;
	cycle_now = clock->read(clock);

	/* calculate the delta since the last update_wall_time: */
	cycle_delta = (cycle_now - clock->cycle_last) & clock->mask;

	/* return delta convert to nanoseconds using ntp adjusted mult. */
	return clocksource_cyc2ns(cycle_delta, timekeeper.mult,
				  timekeeper.shift);
}

static inline s64 timekeeping_get_ns_raw(void)
{
	cycle_t cycle_now, cycle_delta;
	struct clocksource *clock;

	/* read clocksource: */
	clock = timekeeper.clock;
	cycle_now = clock->read(clock);

	/* calculate the delta since the last update_wall_time: */
	cycle_delta = (cycle_now - clock->cycle_last) & clock->mask;

	/* return delta convert to nanoseconds. */
	return clocksource_cyc2ns(cycle_delta, clock->mult, clock->shift);
}

static void update_rt_offset(void)
{
	struct timespec tmp, *wtm = &timekeeper.wall_to_monotonic;

	set_normalized_timespec(&tmp, -wtm->tv_sec, -wtm->tv_nsec);
	timekeeper.offs_real = timespec_to_ktime(tmp);
}

/*
 *   tk_update_leap_state - helper to update the next_leap_ktime
=======
	tk->tkr_mono.mult = clock->mult;
	tk->tkr_raw.mult = clock->mult;
	tk->ntp_err_mult = 0;
	tk->skip_second_overflow = 0;
}

/* Timekeeper helper functions. */

static inline u64 timekeeping_delta_to_ns(const struct tk_read_base *tkr, u64 delta)
{
	u64 nsec;

	nsec = delta * tkr->mult + tkr->xtime_nsec;
	nsec >>= tkr->shift;

	return nsec;
}

static inline u64 timekeeping_get_ns(const struct tk_read_base *tkr)
{
	u64 delta;

	delta = timekeeping_get_delta(tkr);
	return timekeeping_delta_to_ns(tkr, delta);
}

static inline u64 timekeeping_cycles_to_ns(const struct tk_read_base *tkr, u64 cycles)
{
	u64 delta;

	/* calculate the delta since the last update_wall_time */
	delta = clocksource_delta(cycles, tkr->cycle_last, tkr->mask);
	return timekeeping_delta_to_ns(tkr, delta);
}

/**
 * update_fast_timekeeper - Update the fast and NMI safe monotonic timekeeper.
 * @tkr: Timekeeping readout base from which we take the update
 * @tkf: Pointer to NMI safe timekeeper
 *
 * We want to use this from any context including NMI and tracing /
 * instrumenting the timekeeping code itself.
 *
 * Employ the latch technique; see @raw_write_seqcount_latch.
 *
 * So if a NMI hits the update of base[0] then it will use base[1]
 * which is still consistent. In the worst case this can result is a
 * slightly wrong timestamp (a few nanoseconds). See
 * @ktime_get_mono_fast_ns.
 */
static void update_fast_timekeeper(const struct tk_read_base *tkr,
				   struct tk_fast *tkf)
{
	struct tk_read_base *base = tkf->base;

	/* Force readers off to base[1] */
	raw_write_seqcount_latch(&tkf->seq);

	/* Update base[0] */
	memcpy(base, tkr, sizeof(*base));

	/* Force readers back to base[0] */
	raw_write_seqcount_latch(&tkf->seq);

	/* Update base[1] */
	memcpy(base + 1, base, sizeof(*base));
}

static __always_inline u64 fast_tk_get_delta_ns(struct tk_read_base *tkr)
{
	u64 delta, cycles = tk_clock_read(tkr);

	delta = clocksource_delta(cycles, tkr->cycle_last, tkr->mask);
	return timekeeping_delta_to_ns(tkr, delta);
}

static __always_inline u64 __ktime_get_fast_ns(struct tk_fast *tkf)
{
	struct tk_read_base *tkr;
	unsigned int seq;
	u64 now;

	do {
		seq = raw_read_seqcount_latch(&tkf->seq);
		tkr = tkf->base + (seq & 0x01);
		now = ktime_to_ns(tkr->base);
		now += fast_tk_get_delta_ns(tkr);
	} while (raw_read_seqcount_latch_retry(&tkf->seq, seq));

	return now;
}

/**
 * ktime_get_mono_fast_ns - Fast NMI safe access to clock monotonic
 *
 * This timestamp is not guaranteed to be monotonic across an update.
 * The timestamp is calculated by:
 *
 *	now = base_mono + clock_delta * slope
 *
 * So if the update lowers the slope, readers who are forced to the
 * not yet updated second array are still using the old steeper slope.
 *
 * tmono
 * ^
 * |    o  n
 * |   o n
 * |  u
 * | o
 * |o
 * |12345678---> reader order
 *
 * o = old slope
 * u = update
 * n = new slope
 *
 * So reader 6 will observe time going backwards versus reader 5.
 *
 * While other CPUs are likely to be able to observe that, the only way
 * for a CPU local observation is when an NMI hits in the middle of
 * the update. Timestamps taken from that NMI context might be ahead
 * of the following timestamps. Callers need to be aware of that and
 * deal with it.
 */
u64 notrace ktime_get_mono_fast_ns(void)
{
	return __ktime_get_fast_ns(&tk_fast_mono);
}
EXPORT_SYMBOL_GPL(ktime_get_mono_fast_ns);

/**
 * ktime_get_raw_fast_ns - Fast NMI safe access to clock monotonic raw
 *
 * Contrary to ktime_get_mono_fast_ns() this is always correct because the
 * conversion factor is not affected by NTP/PTP correction.
 */
u64 notrace ktime_get_raw_fast_ns(void)
{
	return __ktime_get_fast_ns(&tk_fast_raw);
}
EXPORT_SYMBOL_GPL(ktime_get_raw_fast_ns);

/**
 * ktime_get_boot_fast_ns - NMI safe and fast access to boot clock.
 *
 * To keep it NMI safe since we're accessing from tracing, we're not using a
 * separate timekeeper with updates to monotonic clock and boot offset
 * protected with seqcounts. This has the following minor side effects:
 *
 * (1) Its possible that a timestamp be taken after the boot offset is updated
 * but before the timekeeper is updated. If this happens, the new boot offset
 * is added to the old timekeeping making the clock appear to update slightly
 * earlier:
 *    CPU 0                                        CPU 1
 *    timekeeping_inject_sleeptime64()
 *    __timekeeping_inject_sleeptime(tk, delta);
 *                                                 timestamp();
 *    timekeeping_update(tk, TK_CLEAR_NTP...);
 *
 * (2) On 32-bit systems, the 64-bit boot offset (tk->offs_boot) may be
 * partially updated.  Since the tk->offs_boot update is a rare event, this
 * should be a rare occurrence which postprocessing should be able to handle.
 *
 * The caveats vs. timestamp ordering as documented for ktime_get_mono_fast_ns()
 * apply as well.
 */
u64 notrace ktime_get_boot_fast_ns(void)
{
	struct timekeeper *tk = &tk_core.timekeeper;

	return (ktime_get_mono_fast_ns() + ktime_to_ns(data_race(tk->offs_boot)));
}
EXPORT_SYMBOL_GPL(ktime_get_boot_fast_ns);

/**
 * ktime_get_tai_fast_ns - NMI safe and fast access to tai clock.
 *
 * The same limitations as described for ktime_get_boot_fast_ns() apply. The
 * mono time and the TAI offset are not read atomically which may yield wrong
 * readouts. However, an update of the TAI offset is an rare event e.g., caused
 * by settime or adjtimex with an offset. The user of this function has to deal
 * with the possibility of wrong timestamps in post processing.
 */
u64 notrace ktime_get_tai_fast_ns(void)
{
	struct timekeeper *tk = &tk_core.timekeeper;

	return (ktime_get_mono_fast_ns() + ktime_to_ns(data_race(tk->offs_tai)));
}
EXPORT_SYMBOL_GPL(ktime_get_tai_fast_ns);

static __always_inline u64 __ktime_get_real_fast(struct tk_fast *tkf, u64 *mono)
{
	struct tk_read_base *tkr;
	u64 basem, baser, delta;
	unsigned int seq;

	do {
		seq = raw_read_seqcount_latch(&tkf->seq);
		tkr = tkf->base + (seq & 0x01);
		basem = ktime_to_ns(tkr->base);
		baser = ktime_to_ns(tkr->base_real);
		delta = fast_tk_get_delta_ns(tkr);
	} while (raw_read_seqcount_latch_retry(&tkf->seq, seq));

	if (mono)
		*mono = basem + delta;
	return baser + delta;
}

/**
 * ktime_get_real_fast_ns: - NMI safe and fast access to clock realtime.
 *
 * See ktime_get_mono_fast_ns() for documentation of the time stamp ordering.
 */
u64 ktime_get_real_fast_ns(void)
{
	return __ktime_get_real_fast(&tk_fast_mono, NULL);
}
EXPORT_SYMBOL_GPL(ktime_get_real_fast_ns);

/**
 * ktime_get_fast_timestamps: - NMI safe timestamps
 * @snapshot:	Pointer to timestamp storage
 *
 * Stores clock monotonic, boottime and realtime timestamps.
 *
 * Boot time is a racy access on 32bit systems if the sleep time injection
 * happens late during resume and not in timekeeping_resume(). That could
 * be avoided by expanding struct tk_read_base with boot offset for 32bit
 * and adding more overhead to the update. As this is a hard to observe
 * once per resume event which can be filtered with reasonable effort using
 * the accurate mono/real timestamps, it's probably not worth the trouble.
 *
 * Aside of that it might be possible on 32 and 64 bit to observe the
 * following when the sleep time injection happens late:
 *
 * CPU 0				CPU 1
 * timekeeping_resume()
 * ktime_get_fast_timestamps()
 *	mono, real = __ktime_get_real_fast()
 *					inject_sleep_time()
 *					   update boot offset
 *	boot = mono + bootoffset;
 *
 * That means that boot time already has the sleep time adjustment, but
 * real time does not. On the next readout both are in sync again.
 *
 * Preventing this for 64bit is not really feasible without destroying the
 * careful cache layout of the timekeeper because the sequence count and
 * struct tk_read_base would then need two cache lines instead of one.
 *
 * Access to the time keeper clock source is disabled across the innermost
 * steps of suspend/resume. The accessors still work, but the timestamps
 * are frozen until time keeping is resumed which happens very early.
 *
 * For regular suspend/resume there is no observable difference vs. sched
 * clock, but it might affect some of the nasty low level debug printks.
 *
 * OTOH, access to sched clock is not guaranteed across suspend/resume on
 * all systems either so it depends on the hardware in use.
 *
 * If that turns out to be a real problem then this could be mitigated by
 * using sched clock in a similar way as during early boot. But it's not as
 * trivial as on early boot because it needs some careful protection
 * against the clock monotonic timestamp jumping backwards on resume.
 */
void ktime_get_fast_timestamps(struct ktime_timestamps *snapshot)
{
	struct timekeeper *tk = &tk_core.timekeeper;

	snapshot->real = __ktime_get_real_fast(&tk_fast_mono, &snapshot->mono);
	snapshot->boot = snapshot->mono + ktime_to_ns(data_race(tk->offs_boot));
}

/**
 * halt_fast_timekeeper - Prevent fast timekeeper from accessing clocksource.
 * @tk: Timekeeper to snapshot.
 *
 * It generally is unsafe to access the clocksource after timekeeping has been
 * suspended, so take a snapshot of the readout base of @tk and use it as the
 * fast timekeeper's readout base while suspended.  It will return the same
 * number of cycles every time until timekeeping is resumed at which time the
 * proper readout base for the fast timekeeper will be restored automatically.
 */
static void halt_fast_timekeeper(const struct timekeeper *tk)
{
	static struct tk_read_base tkr_dummy;
	const struct tk_read_base *tkr = &tk->tkr_mono;

	memcpy(&tkr_dummy, tkr, sizeof(tkr_dummy));
	cycles_at_suspend = tk_clock_read(tkr);
	tkr_dummy.clock = &dummy_clock;
	tkr_dummy.base_real = tkr->base + tk->offs_real;
	update_fast_timekeeper(&tkr_dummy, &tk_fast_mono);

	tkr = &tk->tkr_raw;
	memcpy(&tkr_dummy, tkr, sizeof(tkr_dummy));
	tkr_dummy.clock = &dummy_clock;
	update_fast_timekeeper(&tkr_dummy, &tk_fast_raw);
}

static RAW_NOTIFIER_HEAD(pvclock_gtod_chain);

static void update_pvclock_gtod(struct timekeeper *tk, bool was_set)
{
	raw_notifier_call_chain(&pvclock_gtod_chain, was_set, tk);
}

/**
 * pvclock_gtod_register_notifier - register a pvclock timedata update listener
 * @nb: Pointer to the notifier block to register
 */
int pvclock_gtod_register_notifier(struct notifier_block *nb)
{
	struct timekeeper *tk = &tk_core.timekeeper;
	unsigned long flags;
	int ret;

	raw_spin_lock_irqsave(&timekeeper_lock, flags);
	ret = raw_notifier_chain_register(&pvclock_gtod_chain, nb);
	update_pvclock_gtod(tk, true);
	raw_spin_unlock_irqrestore(&timekeeper_lock, flags);

	return ret;
}
EXPORT_SYMBOL_GPL(pvclock_gtod_register_notifier);

/**
 * pvclock_gtod_unregister_notifier - unregister a pvclock
 * timedata update listener
 * @nb: Pointer to the notifier block to unregister
 */
int pvclock_gtod_unregister_notifier(struct notifier_block *nb)
{
	unsigned long flags;
	int ret;

	raw_spin_lock_irqsave(&timekeeper_lock, flags);
	ret = raw_notifier_chain_unregister(&pvclock_gtod_chain, nb);
	raw_spin_unlock_irqrestore(&timekeeper_lock, flags);

	return ret;
}
EXPORT_SYMBOL_GPL(pvclock_gtod_unregister_notifier);

/*
 * tk_update_leap_state - helper to update the next_leap_ktime
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 */
static inline void tk_update_leap_state(struct timekeeper *tk)
{
	tk->next_leap_ktime = ntp_get_next_leap();
<<<<<<< HEAD
	if (tk->next_leap_ktime.tv64 != KTIME_MAX)
=======
	if (tk->next_leap_ktime != KTIME_MAX)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		/* Convert to monotonic time */
		tk->next_leap_ktime = ktime_sub(tk->next_leap_ktime, tk->offs_real);
}

<<<<<<< HEAD
/* must hold write on timekeeper.lock */
static void timekeeping_update(bool clearntp)
{
	if (clearntp) {
		timekeeper.ntp_error = 0;
		ntp_clear();
	}
	tk_update_leap_state(&timekeeper);
	update_rt_offset();
	update_vsyscall(&timekeeper.xtime, &timekeeper.wall_to_monotonic,
			 timekeeper.clock, timekeeper.mult);
}


/**
 * timekeeping_forward_now - update clock to the current time
=======
/*
 * Update the ktime_t based scalar nsec members of the timekeeper
 */
static inline void tk_update_ktime_data(struct timekeeper *tk)
{
	u64 seconds;
	u32 nsec;

	/*
	 * The xtime based monotonic readout is:
	 *	nsec = (xtime_sec + wtm_sec) * 1e9 + wtm_nsec + now();
	 * The ktime based monotonic readout is:
	 *	nsec = base_mono + now();
	 * ==> base_mono = (xtime_sec + wtm_sec) * 1e9 + wtm_nsec
	 */
	seconds = (u64)(tk->xtime_sec + tk->wall_to_monotonic.tv_sec);
	nsec = (u32) tk->wall_to_monotonic.tv_nsec;
	tk->tkr_mono.base = ns_to_ktime(seconds * NSEC_PER_SEC + nsec);

	/*
	 * The sum of the nanoseconds portions of xtime and
	 * wall_to_monotonic can be greater/equal one second. Take
	 * this into account before updating tk->ktime_sec.
	 */
	nsec += (u32)(tk->tkr_mono.xtime_nsec >> tk->tkr_mono.shift);
	if (nsec >= NSEC_PER_SEC)
		seconds++;
	tk->ktime_sec = seconds;

	/* Update the monotonic raw base */
	tk->tkr_raw.base = ns_to_ktime(tk->raw_sec * NSEC_PER_SEC);
}

/* must hold timekeeper_lock */
static void timekeeping_update(struct timekeeper *tk, unsigned int action)
{
	if (action & TK_CLEAR_NTP) {
		tk->ntp_error = 0;
		ntp_clear();
	}

	tk_update_leap_state(tk);
	tk_update_ktime_data(tk);

	update_vsyscall(tk);
	update_pvclock_gtod(tk, action & TK_CLOCK_WAS_SET);

	tk->tkr_mono.base_real = tk->tkr_mono.base + tk->offs_real;
	update_fast_timekeeper(&tk->tkr_mono, &tk_fast_mono);
	update_fast_timekeeper(&tk->tkr_raw,  &tk_fast_raw);

	if (action & TK_CLOCK_WAS_SET)
		tk->clock_was_set_seq++;
	/*
	 * The mirroring of the data to the shadow-timekeeper needs
	 * to happen last here to ensure we don't over-write the
	 * timekeeper structure on the next update with stale data
	 */
	if (action & TK_MIRROR)
		memcpy(&shadow_timekeeper, &tk_core.timekeeper,
		       sizeof(tk_core.timekeeper));
}

/**
 * timekeeping_forward_now - update clock to the current time
 * @tk:		Pointer to the timekeeper to update
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 *
 * Forward the current clock to update its state since the last call to
 * update_wall_time(). This is useful before significant clock changes,
 * as it avoids having to deal with this time offset explicitly.
 */
<<<<<<< HEAD
static void timekeeping_forward_now(void)
{
	cycle_t cycle_now, cycle_delta;
	struct clocksource *clock;
	s64 nsec;

	clock = timekeeper.clock;
	cycle_now = clock->read(clock);
	cycle_delta = (cycle_now - clock->cycle_last) & clock->mask;
	clock->cycle_last = cycle_now;

	nsec = clocksource_cyc2ns(cycle_delta, timekeeper.mult,
				  timekeeper.shift);

	/* If arch requires, add in gettimeoffset() */
	nsec += arch_gettimeoffset();

	timespec_add_ns(&timekeeper.xtime, nsec);

	nsec = clocksource_cyc2ns(cycle_delta, clock->mult, clock->shift);
	timespec_add_ns(&timekeeper.raw_time, nsec);
}

/**
 * getnstimeofday - Returns the time of day in a timespec
 * @ts:		pointer to the timespec to be set
 *
 * Returns the time of day in a timespec.
 */
void getnstimeofday(struct timespec *ts)
{
	unsigned long seq;
	s64 nsecs;
=======
static void timekeeping_forward_now(struct timekeeper *tk)
{
	u64 cycle_now, delta;

	cycle_now = tk_clock_read(&tk->tkr_mono);
	delta = clocksource_delta(cycle_now, tk->tkr_mono.cycle_last, tk->tkr_mono.mask);
	tk->tkr_mono.cycle_last = cycle_now;
	tk->tkr_raw.cycle_last  = cycle_now;

	tk->tkr_mono.xtime_nsec += delta * tk->tkr_mono.mult;
	tk->tkr_raw.xtime_nsec += delta * tk->tkr_raw.mult;

	tk_normalize_xtime(tk);
}

/**
 * ktime_get_real_ts64 - Returns the time of day in a timespec64.
 * @ts:		pointer to the timespec to be set
 *
 * Returns the time of day in a timespec64 (WARN if suspended).
 */
void ktime_get_real_ts64(struct timespec64 *ts)
{
	struct timekeeper *tk = &tk_core.timekeeper;
	unsigned int seq;
	u64 nsecs;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	WARN_ON(timekeeping_suspended);

	do {
<<<<<<< HEAD
		seq = read_seqbegin(&timekeeper.lock);

		*ts = timekeeper.xtime;
		nsecs = timekeeping_get_ns();

		/* If arch requires, add in gettimeoffset() */
		nsecs += arch_gettimeoffset();

	} while (read_seqretry(&timekeeper.lock, seq));

	timespec_add_ns(ts, nsecs);
}

EXPORT_SYMBOL(getnstimeofday);

ktime_t ktime_get(void)
{
	unsigned int seq;
	s64 secs, nsecs;
=======
		seq = read_seqcount_begin(&tk_core.seq);

		ts->tv_sec = tk->xtime_sec;
		nsecs = timekeeping_get_ns(&tk->tkr_mono);

	} while (read_seqcount_retry(&tk_core.seq, seq));

	ts->tv_nsec = 0;
	timespec64_add_ns(ts, nsecs);
}
EXPORT_SYMBOL(ktime_get_real_ts64);

ktime_t ktime_get(void)
{
	struct timekeeper *tk = &tk_core.timekeeper;
	unsigned int seq;
	ktime_t base;
	u64 nsecs;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	WARN_ON(timekeeping_suspended);

	do {
<<<<<<< HEAD
		seq = read_seqbegin(&timekeeper.lock);
		secs = timekeeper.xtime.tv_sec +
				timekeeper.wall_to_monotonic.tv_sec;
		nsecs = timekeeper.xtime.tv_nsec +
				timekeeper.wall_to_monotonic.tv_nsec;
		nsecs += timekeeping_get_ns();
		/* If arch requires, add in gettimeoffset() */
		nsecs += arch_gettimeoffset();

	} while (read_seqretry(&timekeeper.lock, seq));
	/*
	 * Use ktime_set/ktime_add_ns to create a proper ktime on
	 * 32-bit architectures without CONFIG_KTIME_SCALAR.
	 */
	return ktime_add_ns(ktime_set(secs, 0), nsecs);
}
EXPORT_SYMBOL_GPL(ktime_get);

/**
 * ktime_get_ts - get the monotonic clock in timespec format
=======
		seq = read_seqcount_begin(&tk_core.seq);
		base = tk->tkr_mono.base;
		nsecs = timekeeping_get_ns(&tk->tkr_mono);

	} while (read_seqcount_retry(&tk_core.seq, seq));

	return ktime_add_ns(base, nsecs);
}
EXPORT_SYMBOL_GPL(ktime_get);

u32 ktime_get_resolution_ns(void)
{
	struct timekeeper *tk = &tk_core.timekeeper;
	unsigned int seq;
	u32 nsecs;

	WARN_ON(timekeeping_suspended);

	do {
		seq = read_seqcount_begin(&tk_core.seq);
		nsecs = tk->tkr_mono.mult >> tk->tkr_mono.shift;
	} while (read_seqcount_retry(&tk_core.seq, seq));

	return nsecs;
}
EXPORT_SYMBOL_GPL(ktime_get_resolution_ns);

static ktime_t *offsets[TK_OFFS_MAX] = {
	[TK_OFFS_REAL]	= &tk_core.timekeeper.offs_real,
	[TK_OFFS_BOOT]	= &tk_core.timekeeper.offs_boot,
	[TK_OFFS_TAI]	= &tk_core.timekeeper.offs_tai,
};

ktime_t ktime_get_with_offset(enum tk_offsets offs)
{
	struct timekeeper *tk = &tk_core.timekeeper;
	unsigned int seq;
	ktime_t base, *offset = offsets[offs];
	u64 nsecs;

	WARN_ON(timekeeping_suspended);

	do {
		seq = read_seqcount_begin(&tk_core.seq);
		base = ktime_add(tk->tkr_mono.base, *offset);
		nsecs = timekeeping_get_ns(&tk->tkr_mono);

	} while (read_seqcount_retry(&tk_core.seq, seq));

	return ktime_add_ns(base, nsecs);

}
EXPORT_SYMBOL_GPL(ktime_get_with_offset);

ktime_t ktime_get_coarse_with_offset(enum tk_offsets offs)
{
	struct timekeeper *tk = &tk_core.timekeeper;
	unsigned int seq;
	ktime_t base, *offset = offsets[offs];
	u64 nsecs;

	WARN_ON(timekeeping_suspended);

	do {
		seq = read_seqcount_begin(&tk_core.seq);
		base = ktime_add(tk->tkr_mono.base, *offset);
		nsecs = tk->tkr_mono.xtime_nsec >> tk->tkr_mono.shift;

	} while (read_seqcount_retry(&tk_core.seq, seq));

	return ktime_add_ns(base, nsecs);
}
EXPORT_SYMBOL_GPL(ktime_get_coarse_with_offset);

/**
 * ktime_mono_to_any() - convert monotonic time to any other time
 * @tmono:	time to convert.
 * @offs:	which offset to use
 */
ktime_t ktime_mono_to_any(ktime_t tmono, enum tk_offsets offs)
{
	ktime_t *offset = offsets[offs];
	unsigned int seq;
	ktime_t tconv;

	do {
		seq = read_seqcount_begin(&tk_core.seq);
		tconv = ktime_add(tmono, *offset);
	} while (read_seqcount_retry(&tk_core.seq, seq));

	return tconv;
}
EXPORT_SYMBOL_GPL(ktime_mono_to_any);

/**
 * ktime_get_raw - Returns the raw monotonic time in ktime_t format
 */
ktime_t ktime_get_raw(void)
{
	struct timekeeper *tk = &tk_core.timekeeper;
	unsigned int seq;
	ktime_t base;
	u64 nsecs;

	do {
		seq = read_seqcount_begin(&tk_core.seq);
		base = tk->tkr_raw.base;
		nsecs = timekeeping_get_ns(&tk->tkr_raw);

	} while (read_seqcount_retry(&tk_core.seq, seq));

	return ktime_add_ns(base, nsecs);
}
EXPORT_SYMBOL_GPL(ktime_get_raw);

/**
 * ktime_get_ts64 - get the monotonic clock in timespec64 format
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 * @ts:		pointer to timespec variable
 *
 * The function calculates the monotonic clock from the realtime
 * clock and the wall_to_monotonic offset and stores the result
<<<<<<< HEAD
 * in normalized timespec format in the variable pointed to by @ts.
 */
void ktime_get_ts(struct timespec *ts)
{
	struct timespec tomono;
	unsigned int seq;
	s64 nsecs;
=======
 * in normalized timespec64 format in the variable pointed to by @ts.
 */
void ktime_get_ts64(struct timespec64 *ts)
{
	struct timekeeper *tk = &tk_core.timekeeper;
	struct timespec64 tomono;
	unsigned int seq;
	u64 nsec;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	WARN_ON(timekeeping_suspended);

	do {
<<<<<<< HEAD
		seq = read_seqbegin(&timekeeper.lock);
		*ts = timekeeper.xtime;
		tomono = timekeeper.wall_to_monotonic;
		nsecs = timekeeping_get_ns();
		/* If arch requires, add in gettimeoffset() */
		nsecs += arch_gettimeoffset();

	} while (read_seqretry(&timekeeper.lock, seq));

	set_normalized_timespec(ts, ts->tv_sec + tomono.tv_sec,
				ts->tv_nsec + tomono.tv_nsec + nsecs);
}
EXPORT_SYMBOL_GPL(ktime_get_ts);

#ifdef CONFIG_NTP_PPS

/**
 * getnstime_raw_and_real - get day and raw monotonic time in timespec format
 * @ts_raw:	pointer to the timespec to be set to raw monotonic time
 * @ts_real:	pointer to the timespec to be set to the time of day
 *
 * This function reads both the time of day and raw monotonic time at the
 * same time atomically and stores the resulting timestamps in timespec
 * format.
 */
void getnstime_raw_and_real(struct timespec *ts_raw, struct timespec *ts_real)
{
	unsigned long seq;
	s64 nsecs_raw, nsecs_real;
=======
		seq = read_seqcount_begin(&tk_core.seq);
		ts->tv_sec = tk->xtime_sec;
		nsec = timekeeping_get_ns(&tk->tkr_mono);
		tomono = tk->wall_to_monotonic;

	} while (read_seqcount_retry(&tk_core.seq, seq));

	ts->tv_sec += tomono.tv_sec;
	ts->tv_nsec = 0;
	timespec64_add_ns(ts, nsec + tomono.tv_nsec);
}
EXPORT_SYMBOL_GPL(ktime_get_ts64);

/**
 * ktime_get_seconds - Get the seconds portion of CLOCK_MONOTONIC
 *
 * Returns the seconds portion of CLOCK_MONOTONIC with a single non
 * serialized read. tk->ktime_sec is of type 'unsigned long' so this
 * works on both 32 and 64 bit systems. On 32 bit systems the readout
 * covers ~136 years of uptime which should be enough to prevent
 * premature wrap arounds.
 */
time64_t ktime_get_seconds(void)
{
	struct timekeeper *tk = &tk_core.timekeeper;

	WARN_ON(timekeeping_suspended);
	return tk->ktime_sec;
}
EXPORT_SYMBOL_GPL(ktime_get_seconds);

/**
 * ktime_get_real_seconds - Get the seconds portion of CLOCK_REALTIME
 *
 * Returns the wall clock seconds since 1970.
 *
 * For 64bit systems the fast access to tk->xtime_sec is preserved. On
 * 32bit systems the access must be protected with the sequence
 * counter to provide "atomic" access to the 64bit tk->xtime_sec
 * value.
 */
time64_t ktime_get_real_seconds(void)
{
	struct timekeeper *tk = &tk_core.timekeeper;
	time64_t seconds;
	unsigned int seq;

	if (IS_ENABLED(CONFIG_64BIT))
		return tk->xtime_sec;

	do {
		seq = read_seqcount_begin(&tk_core.seq);
		seconds = tk->xtime_sec;

	} while (read_seqcount_retry(&tk_core.seq, seq));

	return seconds;
}
EXPORT_SYMBOL_GPL(ktime_get_real_seconds);

/**
 * __ktime_get_real_seconds - The same as ktime_get_real_seconds
 * but without the sequence counter protect. This internal function
 * is called just when timekeeping lock is already held.
 */
noinstr time64_t __ktime_get_real_seconds(void)
{
	struct timekeeper *tk = &tk_core.timekeeper;

	return tk->xtime_sec;
}

/**
 * ktime_get_snapshot - snapshots the realtime/monotonic raw clocks with counter
 * @systime_snapshot:	pointer to struct receiving the system time snapshot
 */
void ktime_get_snapshot(struct system_time_snapshot *systime_snapshot)
{
	struct timekeeper *tk = &tk_core.timekeeper;
	unsigned int seq;
	ktime_t base_raw;
	ktime_t base_real;
	u64 nsec_raw;
	u64 nsec_real;
	u64 now;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	WARN_ON_ONCE(timekeeping_suspended);

	do {
<<<<<<< HEAD
		u32 arch_offset;

		seq = read_seqbegin(&timekeeper.lock);

		*ts_raw = timekeeper.raw_time;
		*ts_real = timekeeper.xtime;

		nsecs_raw = timekeeping_get_ns_raw();
		nsecs_real = timekeeping_get_ns();

		/* If arch requires, add in gettimeoffset() */
		arch_offset = arch_gettimeoffset();
		nsecs_raw += arch_offset;
		nsecs_real += arch_offset;

	} while (read_seqretry(&timekeeper.lock, seq));

	timespec_add_ns(ts_raw, nsecs_raw);
	timespec_add_ns(ts_real, nsecs_real);
}
EXPORT_SYMBOL(getnstime_raw_and_real);

#endif /* CONFIG_NTP_PPS */

/**
 * do_gettimeofday - Returns the time of day in a timeval
 * @tv:		pointer to the timeval to be set
 *
 * NOTE: Users should be converted to using getnstimeofday()
 */
void do_gettimeofday(struct timeval *tv)
{
	struct timespec now;

	getnstimeofday(&now);
	tv->tv_sec = now.tv_sec;
	tv->tv_usec = now.tv_nsec/1000;
}

EXPORT_SYMBOL(do_gettimeofday);
/**
 * do_settimeofday - Sets the time of day
 * @tv:		pointer to the timespec variable containing the new time
 *
 * Sets the time of day to the new time and update NTP and notify hrtimers
 */
int do_settimeofday(const struct timespec *tv)
{
	struct timespec ts_delta;
	unsigned long flags;

	if (!timespec_valid_strict(tv))
		return -EINVAL;

	write_seqlock_irqsave(&timekeeper.lock, flags);

	timekeeping_forward_now();

	ts_delta.tv_sec = tv->tv_sec - timekeeper.xtime.tv_sec;
	ts_delta.tv_nsec = tv->tv_nsec - timekeeper.xtime.tv_nsec;
	timekeeper.wall_to_monotonic =
			timespec_sub(timekeeper.wall_to_monotonic, ts_delta);

	timekeeper.xtime = *tv;
	timekeeping_update(true);

	write_sequnlock_irqrestore(&timekeeper.lock, flags);

	/* signal hrtimers about time change */
	clock_was_set();
=======
		seq = read_seqcount_begin(&tk_core.seq);
		now = tk_clock_read(&tk->tkr_mono);
		systime_snapshot->cs_id = tk->tkr_mono.clock->id;
		systime_snapshot->cs_was_changed_seq = tk->cs_was_changed_seq;
		systime_snapshot->clock_was_set_seq = tk->clock_was_set_seq;
		base_real = ktime_add(tk->tkr_mono.base,
				      tk_core.timekeeper.offs_real);
		base_raw = tk->tkr_raw.base;
		nsec_real = timekeeping_cycles_to_ns(&tk->tkr_mono, now);
		nsec_raw  = timekeeping_cycles_to_ns(&tk->tkr_raw, now);
	} while (read_seqcount_retry(&tk_core.seq, seq));

	systime_snapshot->cycles = now;
	systime_snapshot->real = ktime_add_ns(base_real, nsec_real);
	systime_snapshot->raw = ktime_add_ns(base_raw, nsec_raw);
}
EXPORT_SYMBOL_GPL(ktime_get_snapshot);

/* Scale base by mult/div checking for overflow */
static int scale64_check_overflow(u64 mult, u64 div, u64 *base)
{
	u64 tmp, rem;

	tmp = div64_u64_rem(*base, div, &rem);

	if (((int)sizeof(u64)*8 - fls64(mult) < fls64(tmp)) ||
	    ((int)sizeof(u64)*8 - fls64(mult) < fls64(rem)))
		return -EOVERFLOW;
	tmp *= mult;

	rem = div64_u64(rem * mult, div);
	*base = tmp + rem;
	return 0;
}

/**
 * adjust_historical_crosststamp - adjust crosstimestamp previous to current interval
 * @history:			Snapshot representing start of history
 * @partial_history_cycles:	Cycle offset into history (fractional part)
 * @total_history_cycles:	Total history length in cycles
 * @discontinuity:		True indicates clock was set on history period
 * @ts:				Cross timestamp that should be adjusted using
 *	partial/total ratio
 *
 * Helper function used by get_device_system_crosststamp() to correct the
 * crosstimestamp corresponding to the start of the current interval to the
 * system counter value (timestamp point) provided by the driver. The
 * total_history_* quantities are the total history starting at the provided
 * reference point and ending at the start of the current interval. The cycle
 * count between the driver timestamp point and the start of the current
 * interval is partial_history_cycles.
 */
static int adjust_historical_crosststamp(struct system_time_snapshot *history,
					 u64 partial_history_cycles,
					 u64 total_history_cycles,
					 bool discontinuity,
					 struct system_device_crosststamp *ts)
{
	struct timekeeper *tk = &tk_core.timekeeper;
	u64 corr_raw, corr_real;
	bool interp_forward;
	int ret;

	if (total_history_cycles == 0 || partial_history_cycles == 0)
		return 0;

	/* Interpolate shortest distance from beginning or end of history */
	interp_forward = partial_history_cycles > total_history_cycles / 2;
	partial_history_cycles = interp_forward ?
		total_history_cycles - partial_history_cycles :
		partial_history_cycles;

	/*
	 * Scale the monotonic raw time delta by:
	 *	partial_history_cycles / total_history_cycles
	 */
	corr_raw = (u64)ktime_to_ns(
		ktime_sub(ts->sys_monoraw, history->raw));
	ret = scale64_check_overflow(partial_history_cycles,
				     total_history_cycles, &corr_raw);
	if (ret)
		return ret;

	/*
	 * If there is a discontinuity in the history, scale monotonic raw
	 *	correction by:
	 *	mult(real)/mult(raw) yielding the realtime correction
	 * Otherwise, calculate the realtime correction similar to monotonic
	 *	raw calculation
	 */
	if (discontinuity) {
		corr_real = mul_u64_u32_div
			(corr_raw, tk->tkr_mono.mult, tk->tkr_raw.mult);
	} else {
		corr_real = (u64)ktime_to_ns(
			ktime_sub(ts->sys_realtime, history->real));
		ret = scale64_check_overflow(partial_history_cycles,
					     total_history_cycles, &corr_real);
		if (ret)
			return ret;
	}

	/* Fixup monotonic raw and real time time values */
	if (interp_forward) {
		ts->sys_monoraw = ktime_add_ns(history->raw, corr_raw);
		ts->sys_realtime = ktime_add_ns(history->real, corr_real);
	} else {
		ts->sys_monoraw = ktime_sub_ns(ts->sys_monoraw, corr_raw);
		ts->sys_realtime = ktime_sub_ns(ts->sys_realtime, corr_real);
	}
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	return 0;
}

<<<<<<< HEAD
EXPORT_SYMBOL(do_settimeofday);


/**
 * timekeeping_inject_offset - Adds or subtracts from the current time.
 * @tv:		pointer to the timespec variable containing the offset
 *
 * Adds or subtracts an offset value from the current time.
 */
int timekeeping_inject_offset(struct timespec *ts)
{
	unsigned long flags;
	struct timespec tmp;
	int ret = 0;

	if ((unsigned long)ts->tv_nsec >= NSEC_PER_SEC)
		return -EINVAL;

	write_seqlock_irqsave(&timekeeper.lock, flags);

	timekeeping_forward_now();

	tmp = timespec_add(timekeeper.xtime,  *ts);
	if (!timespec_valid_strict(&tmp)) {
=======
/*
 * timestamp_in_interval - true if ts is chronologically in [start, end]
 *
 * True if ts occurs chronologically at or after start, and before or at end.
 */
static bool timestamp_in_interval(u64 start, u64 end, u64 ts)
{
	if (ts >= start && ts <= end)
		return true;
	if (start > end && (ts >= start || ts <= end))
		return true;
	return false;
}

/**
 * get_device_system_crosststamp - Synchronously capture system/device timestamp
 * @get_time_fn:	Callback to get simultaneous device time and
 *	system counter from the device driver
 * @ctx:		Context passed to get_time_fn()
 * @history_begin:	Historical reference point used to interpolate system
 *	time when counter provided by the driver is before the current interval
 * @xtstamp:		Receives simultaneously captured system and device time
 *
 * Reads a timestamp from a device and correlates it to system time
 */
int get_device_system_crosststamp(int (*get_time_fn)
				  (ktime_t *device_time,
				   struct system_counterval_t *sys_counterval,
				   void *ctx),
				  void *ctx,
				  struct system_time_snapshot *history_begin,
				  struct system_device_crosststamp *xtstamp)
{
	struct system_counterval_t system_counterval;
	struct timekeeper *tk = &tk_core.timekeeper;
	u64 cycles, now, interval_start;
	unsigned int clock_was_set_seq = 0;
	ktime_t base_real, base_raw;
	u64 nsec_real, nsec_raw;
	u8 cs_was_changed_seq;
	unsigned int seq;
	bool do_interp;
	int ret;

	do {
		seq = read_seqcount_begin(&tk_core.seq);
		/*
		 * Try to synchronously capture device time and a system
		 * counter value calling back into the device driver
		 */
		ret = get_time_fn(&xtstamp->device, &system_counterval, ctx);
		if (ret)
			return ret;

		/*
		 * Verify that the clocksource ID associated with the captured
		 * system counter value is the same as for the currently
		 * installed timekeeper clocksource
		 */
		if (system_counterval.cs_id == CSID_GENERIC ||
		    tk->tkr_mono.clock->id != system_counterval.cs_id)
			return -ENODEV;
		cycles = system_counterval.cycles;

		/*
		 * Check whether the system counter value provided by the
		 * device driver is on the current timekeeping interval.
		 */
		now = tk_clock_read(&tk->tkr_mono);
		interval_start = tk->tkr_mono.cycle_last;
		if (!timestamp_in_interval(interval_start, now, cycles)) {
			clock_was_set_seq = tk->clock_was_set_seq;
			cs_was_changed_seq = tk->cs_was_changed_seq;
			cycles = interval_start;
			do_interp = true;
		} else {
			do_interp = false;
		}

		base_real = ktime_add(tk->tkr_mono.base,
				      tk_core.timekeeper.offs_real);
		base_raw = tk->tkr_raw.base;

		nsec_real = timekeeping_cycles_to_ns(&tk->tkr_mono, cycles);
		nsec_raw = timekeeping_cycles_to_ns(&tk->tkr_raw, cycles);
	} while (read_seqcount_retry(&tk_core.seq, seq));

	xtstamp->sys_realtime = ktime_add_ns(base_real, nsec_real);
	xtstamp->sys_monoraw = ktime_add_ns(base_raw, nsec_raw);

	/*
	 * Interpolate if necessary, adjusting back from the start of the
	 * current interval
	 */
	if (do_interp) {
		u64 partial_history_cycles, total_history_cycles;
		bool discontinuity;

		/*
		 * Check that the counter value is not before the provided
		 * history reference and that the history doesn't cross a
		 * clocksource change
		 */
		if (!history_begin ||
		    !timestamp_in_interval(history_begin->cycles,
					   cycles, system_counterval.cycles) ||
		    history_begin->cs_was_changed_seq != cs_was_changed_seq)
			return -EINVAL;
		partial_history_cycles = cycles - system_counterval.cycles;
		total_history_cycles = cycles - history_begin->cycles;
		discontinuity =
			history_begin->clock_was_set_seq != clock_was_set_seq;

		ret = adjust_historical_crosststamp(history_begin,
						    partial_history_cycles,
						    total_history_cycles,
						    discontinuity, xtstamp);
		if (ret)
			return ret;
	}

	return 0;
}
EXPORT_SYMBOL_GPL(get_device_system_crosststamp);

/**
 * do_settimeofday64 - Sets the time of day.
 * @ts:     pointer to the timespec64 variable containing the new time
 *
 * Sets the time of day to the new time and update NTP and notify hrtimers
 */
int do_settimeofday64(const struct timespec64 *ts)
{
	struct timekeeper *tk = &tk_core.timekeeper;
	struct timespec64 ts_delta, xt;
	unsigned long flags;
	int ret = 0;

	if (!timespec64_valid_settod(ts))
		return -EINVAL;

	raw_spin_lock_irqsave(&timekeeper_lock, flags);
	write_seqcount_begin(&tk_core.seq);

	timekeeping_forward_now(tk);

	xt = tk_xtime(tk);
	ts_delta = timespec64_sub(*ts, xt);

	if (timespec64_compare(&tk->wall_to_monotonic, &ts_delta) > 0) {
		ret = -EINVAL;
		goto out;
	}

	tk_set_wall_to_mono(tk, timespec64_sub(tk->wall_to_monotonic, ts_delta));

	tk_set_xtime(tk, ts);
out:
	timekeeping_update(tk, TK_CLEAR_NTP | TK_MIRROR | TK_CLOCK_WAS_SET);

	write_seqcount_end(&tk_core.seq);
	raw_spin_unlock_irqrestore(&timekeeper_lock, flags);

	/* Signal hrtimers about time change */
	clock_was_set(CLOCK_SET_WALL);

	if (!ret) {
		audit_tk_injoffset(ts_delta);
		add_device_randomness(ts, sizeof(*ts));
	}

	return ret;
}
EXPORT_SYMBOL(do_settimeofday64);

/**
 * timekeeping_inject_offset - Adds or subtracts from the current time.
 * @ts:		Pointer to the timespec variable containing the offset
 *
 * Adds or subtracts an offset value from the current time.
 */
static int timekeeping_inject_offset(const struct timespec64 *ts)
{
	struct timekeeper *tk = &tk_core.timekeeper;
	unsigned long flags;
	struct timespec64 tmp;
	int ret = 0;

	if (ts->tv_nsec < 0 || ts->tv_nsec >= NSEC_PER_SEC)
		return -EINVAL;

	raw_spin_lock_irqsave(&timekeeper_lock, flags);
	write_seqcount_begin(&tk_core.seq);

	timekeeping_forward_now(tk);

	/* Make sure the proposed value is valid */
	tmp = timespec64_add(tk_xtime(tk), *ts);
	if (timespec64_compare(&tk->wall_to_monotonic, ts) > 0 ||
	    !timespec64_valid_settod(&tmp)) {
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		ret = -EINVAL;
		goto error;
	}

<<<<<<< HEAD
	timekeeper.xtime = timespec_add(timekeeper.xtime, *ts);
	timekeeper.wall_to_monotonic =
				timespec_sub(timekeeper.wall_to_monotonic, *ts);

error: /* even if we error out, we forwarded the time, so call update */
	timekeeping_update(true);

	write_sequnlock_irqrestore(&timekeeper.lock, flags);

	/* signal hrtimers about time change */
	clock_was_set();

	return ret;
}
EXPORT_SYMBOL(timekeeping_inject_offset);

/**
=======
	tk_xtime_add(tk, ts);
	tk_set_wall_to_mono(tk, timespec64_sub(tk->wall_to_monotonic, *ts));

error: /* even if we error out, we forwarded the time, so call update */
	timekeeping_update(tk, TK_CLEAR_NTP | TK_MIRROR | TK_CLOCK_WAS_SET);

	write_seqcount_end(&tk_core.seq);
	raw_spin_unlock_irqrestore(&timekeeper_lock, flags);

	/* Signal hrtimers about time change */
	clock_was_set(CLOCK_SET_WALL);

	return ret;
}

/*
 * Indicates if there is an offset between the system clock and the hardware
 * clock/persistent clock/rtc.
 */
int persistent_clock_is_local;

/*
 * Adjust the time obtained from the CMOS to be UTC time instead of
 * local time.
 *
 * This is ugly, but preferable to the alternatives.  Otherwise we
 * would either need to write a program to do it in /etc/rc (and risk
 * confusion if the program gets run more than once; it would also be
 * hard to make the program warp the clock precisely n hours)  or
 * compile in the timezone information into the kernel.  Bad, bad....
 *
 *						- TYT, 1992-01-01
 *
 * The best thing to do is to keep the CMOS clock in universal time (UTC)
 * as real UNIX machines always do it. This avoids all headaches about
 * daylight saving times and warping kernel clocks.
 */
void timekeeping_warp_clock(void)
{
	if (sys_tz.tz_minuteswest != 0) {
		struct timespec64 adjust;

		persistent_clock_is_local = 1;
		adjust.tv_sec = sys_tz.tz_minuteswest * 60;
		adjust.tv_nsec = 0;
		timekeeping_inject_offset(&adjust);
	}
}

/*
 * __timekeeping_set_tai_offset - Sets the TAI offset from UTC and monotonic
 */
static void __timekeeping_set_tai_offset(struct timekeeper *tk, s32 tai_offset)
{
	tk->tai_offset = tai_offset;
	tk->offs_tai = ktime_add(tk->offs_real, ktime_set(tai_offset, 0));
}

/*
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 * change_clocksource - Swaps clocksources if a new one is available
 *
 * Accumulates current time interval and initializes new clocksource
 */
static int change_clocksource(void *data)
{
<<<<<<< HEAD
	struct clocksource *new, *old;
	unsigned long flags;

	new = (struct clocksource *) data;

	write_seqlock_irqsave(&timekeeper.lock, flags);

	timekeeping_forward_now();
	if (!new->enable || new->enable(new) == 0) {
		old = timekeeper.clock;
		timekeeper_setup_internals(new);
		if (old->disable)
			old->disable(old);
	}
	timekeeping_update(true);

	write_sequnlock_irqrestore(&timekeeper.lock, flags);
=======
	struct timekeeper *tk = &tk_core.timekeeper;
	struct clocksource *new, *old = NULL;
	unsigned long flags;
	bool change = false;

	new = (struct clocksource *) data;

	/*
	 * If the cs is in module, get a module reference. Succeeds
	 * for built-in code (owner == NULL) as well.
	 */
	if (try_module_get(new->owner)) {
		if (!new->enable || new->enable(new) == 0)
			change = true;
		else
			module_put(new->owner);
	}

	raw_spin_lock_irqsave(&timekeeper_lock, flags);
	write_seqcount_begin(&tk_core.seq);

	timekeeping_forward_now(tk);

	if (change) {
		old = tk->tkr_mono.clock;
		tk_setup_internals(tk, new);
	}

	timekeeping_update(tk, TK_CLEAR_NTP | TK_MIRROR | TK_CLOCK_WAS_SET);

	write_seqcount_end(&tk_core.seq);
	raw_spin_unlock_irqrestore(&timekeeper_lock, flags);

	if (old) {
		if (old->disable)
			old->disable(old);

		module_put(old->owner);
	}
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	return 0;
}

/**
 * timekeeping_notify - Install a new clock source
 * @clock:		pointer to the clock source
 *
 * This function is called from clocksource.c after a new, better clock
 * source has been registered. The caller holds the clocksource_mutex.
 */
<<<<<<< HEAD
void timekeeping_notify(struct clocksource *clock)
{
	if (timekeeper.clock == clock)
		return;
	stop_machine(change_clocksource, clock, NULL);
	tick_clock_notify();
}

/**
 * ktime_get_real - get the real (wall-) time in ktime_t format
 *
 * returns the time in ktime_t format
 */
ktime_t ktime_get_real(void)
{
	struct timespec now;

	getnstimeofday(&now);

	return timespec_to_ktime(now);
}
EXPORT_SYMBOL_GPL(ktime_get_real);

/**
 * getrawmonotonic - Returns the raw monotonic time in a timespec
 * @ts:		pointer to the timespec to be set
 *
 * Returns the raw monotonic time (completely un-modified by ntp)
 */
void getrawmonotonic(struct timespec *ts)
{
	unsigned long seq;
	s64 nsecs;

	do {
		seq = read_seqbegin(&timekeeper.lock);
		nsecs = timekeeping_get_ns_raw();
		*ts = timekeeper.raw_time;

	} while (read_seqretry(&timekeeper.lock, seq));

	timespec_add_ns(ts, nsecs);
}
EXPORT_SYMBOL(getrawmonotonic);
=======
int timekeeping_notify(struct clocksource *clock)
{
	struct timekeeper *tk = &tk_core.timekeeper;

	if (tk->tkr_mono.clock == clock)
		return 0;
	stop_machine(change_clocksource, clock, NULL);
	tick_clock_notify();
	return tk->tkr_mono.clock == clock ? 0 : -1;
}

/**
 * ktime_get_raw_ts64 - Returns the raw monotonic time in a timespec
 * @ts:		pointer to the timespec64 to be set
 *
 * Returns the raw monotonic time (completely un-modified by ntp)
 */
void ktime_get_raw_ts64(struct timespec64 *ts)
{
	struct timekeeper *tk = &tk_core.timekeeper;
	unsigned int seq;
	u64 nsecs;

	do {
		seq = read_seqcount_begin(&tk_core.seq);
		ts->tv_sec = tk->raw_sec;
		nsecs = timekeeping_get_ns(&tk->tkr_raw);

	} while (read_seqcount_retry(&tk_core.seq, seq));

	ts->tv_nsec = 0;
	timespec64_add_ns(ts, nsecs);
}
EXPORT_SYMBOL(ktime_get_raw_ts64);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)


/**
 * timekeeping_valid_for_hres - Check if timekeeping is suitable for hres
 */
int timekeeping_valid_for_hres(void)
{
<<<<<<< HEAD
	unsigned long seq;
	int ret;

	do {
		seq = read_seqbegin(&timekeeper.lock);

		ret = timekeeper.clock->flags & CLOCK_SOURCE_VALID_FOR_HRES;

	} while (read_seqretry(&timekeeper.lock, seq));
=======
	struct timekeeper *tk = &tk_core.timekeeper;
	unsigned int seq;
	int ret;

	do {
		seq = read_seqcount_begin(&tk_core.seq);

		ret = tk->tkr_mono.clock->flags & CLOCK_SOURCE_VALID_FOR_HRES;

	} while (read_seqcount_retry(&tk_core.seq, seq));
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	return ret;
}

/**
 * timekeeping_max_deferment - Returns max time the clocksource can be deferred
 */
u64 timekeeping_max_deferment(void)
{
<<<<<<< HEAD
	unsigned long seq;
	u64 ret;
	do {
		seq = read_seqbegin(&timekeeper.lock);

		ret = timekeeper.clock->max_idle_ns;

	} while (read_seqretry(&timekeeper.lock, seq));
=======
	struct timekeeper *tk = &tk_core.timekeeper;
	unsigned int seq;
	u64 ret;

	do {
		seq = read_seqcount_begin(&tk_core.seq);

		ret = tk->tkr_mono.clock->max_idle_ns;

	} while (read_seqcount_retry(&tk_core.seq, seq));
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	return ret;
}

/**
<<<<<<< HEAD
 * read_persistent_clock -  Return time from the persistent clock.
=======
 * read_persistent_clock64 -  Return time from the persistent clock.
 * @ts: Pointer to the storage for the readout value
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 *
 * Weak dummy function for arches that do not yet support it.
 * Reads the time from the battery backed persistent clock.
 * Returns a timespec with tv_sec=0 and tv_nsec=0 if unsupported.
 *
 *  XXX - Do be sure to remove it once all arches implement it.
 */
<<<<<<< HEAD
void __attribute__((weak)) read_persistent_clock(struct timespec *ts)
=======
void __weak read_persistent_clock64(struct timespec64 *ts)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
{
	ts->tv_sec = 0;
	ts->tv_nsec = 0;
}

/**
<<<<<<< HEAD
 * read_boot_clock -  Return time of the system start.
 *
 * Weak dummy function for arches that do not yet support it.
 * Function to read the exact time the system has been started.
 * Returns a timespec with tv_sec=0 and tv_nsec=0 if unsupported.
 *
 *  XXX - Do be sure to remove it once all arches implement it.
 */
void __attribute__((weak)) read_boot_clock(struct timespec *ts)
{
	ts->tv_sec = 0;
	ts->tv_nsec = 0;
}

/*
=======
 * read_persistent_wall_and_boot_offset - Read persistent clock, and also offset
 *                                        from the boot.
 * @wall_time:	  current time as returned by persistent clock
 * @boot_offset:  offset that is defined as wall_time - boot_time
 *
 * Weak dummy function for arches that do not yet support it.
 *
 * The default function calculates offset based on the current value of
 * local_clock(). This way architectures that support sched_clock() but don't
 * support dedicated boot time clock will provide the best estimate of the
 * boot time.
 */
void __weak __init
read_persistent_wall_and_boot_offset(struct timespec64 *wall_time,
				     struct timespec64 *boot_offset)
{
	read_persistent_clock64(wall_time);
	*boot_offset = ns_to_timespec64(local_clock());
}

/*
 * Flag reflecting whether timekeeping_resume() has injected sleeptime.
 *
 * The flag starts of false and is only set when a suspend reaches
 * timekeeping_suspend(), timekeeping_resume() sets it to false when the
 * timekeeper clocksource is not stopping across suspend and has been
 * used to update sleep time. If the timekeeper clocksource has stopped
 * then the flag stays true and is used by the RTC resume code to decide
 * whether sleeptime must be injected and if so the flag gets false then.
 *
 * If a suspend fails before reaching timekeeping_resume() then the flag
 * stays false and prevents erroneous sleeptime injection.
 */
static bool suspend_timing_needed;

/* Flag for if there is a persistent clock on this platform */
static bool persistent_clock_exists;

/*
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 * timekeeping_init - Initializes the clocksource and common timekeeping values
 */
void __init timekeeping_init(void)
{
<<<<<<< HEAD
	struct clocksource *clock;
	unsigned long flags;
	struct timespec now, boot;

	read_persistent_clock(&now);
	if (!timespec_valid_strict(&now)) {
		pr_warn("WARNING: Persistent clock returned invalid value!\n"
			"         Check your CMOS/BIOS settings.\n");
		now.tv_sec = 0;
		now.tv_nsec = 0;
	}

	read_boot_clock(&boot);
	if (!timespec_valid_strict(&boot)) {
		pr_warn("WARNING: Boot clock returned invalid value!\n"
			"         Check your CMOS/BIOS settings.\n");
		boot.tv_sec = 0;
		boot.tv_nsec = 0;
	}

	seqlock_init(&timekeeper.lock);

	ntp_init();

	write_seqlock_irqsave(&timekeeper.lock, flags);
	clock = clocksource_default_clock();
	if (clock->enable)
		clock->enable(clock);
	timekeeper_setup_internals(clock);

	timekeeper.xtime.tv_sec = now.tv_sec;
	timekeeper.xtime.tv_nsec = now.tv_nsec;
	timekeeper.raw_time.tv_sec = 0;
	timekeeper.raw_time.tv_nsec = 0;
	if (boot.tv_sec == 0 && boot.tv_nsec == 0) {
		boot.tv_sec = timekeeper.xtime.tv_sec;
		boot.tv_nsec = timekeeper.xtime.tv_nsec;
	}
	set_normalized_timespec(&timekeeper.wall_to_monotonic,
				-boot.tv_sec, -boot.tv_nsec);
	update_rt_offset();
	timekeeper.total_sleep_time.tv_sec = 0;
	timekeeper.total_sleep_time.tv_nsec = 0;
	write_sequnlock_irqrestore(&timekeeper.lock, flags);
}

/* time in seconds when suspend began */
static struct timespec timekeeping_suspend_time;

static void update_sleep_time(struct timespec t)
{
	timekeeper.total_sleep_time = t;
	timekeeper.offs_boot = timespec_to_ktime(t);
}

/**
 * __timekeeping_inject_sleeptime - Internal function to add sleep interval
 * @delta: pointer to a timespec delta value
=======
	struct timespec64 wall_time, boot_offset, wall_to_mono;
	struct timekeeper *tk = &tk_core.timekeeper;
	struct clocksource *clock;
	unsigned long flags;

	read_persistent_wall_and_boot_offset(&wall_time, &boot_offset);
	if (timespec64_valid_settod(&wall_time) &&
	    timespec64_to_ns(&wall_time) > 0) {
		persistent_clock_exists = true;
	} else if (timespec64_to_ns(&wall_time) != 0) {
		pr_warn("Persistent clock returned invalid value");
		wall_time = (struct timespec64){0};
	}

	if (timespec64_compare(&wall_time, &boot_offset) < 0)
		boot_offset = (struct timespec64){0};

	/*
	 * We want set wall_to_mono, so the following is true:
	 * wall time + wall_to_mono = boot time
	 */
	wall_to_mono = timespec64_sub(boot_offset, wall_time);

	raw_spin_lock_irqsave(&timekeeper_lock, flags);
	write_seqcount_begin(&tk_core.seq);
	ntp_init();

	clock = clocksource_default_clock();
	if (clock->enable)
		clock->enable(clock);
	tk_setup_internals(tk, clock);

	tk_set_xtime(tk, &wall_time);
	tk->raw_sec = 0;

	tk_set_wall_to_mono(tk, wall_to_mono);

	timekeeping_update(tk, TK_MIRROR | TK_CLOCK_WAS_SET);

	write_seqcount_end(&tk_core.seq);
	raw_spin_unlock_irqrestore(&timekeeper_lock, flags);
}

/* time in seconds when suspend began for persistent clock */
static struct timespec64 timekeeping_suspend_time;

/**
 * __timekeeping_inject_sleeptime - Internal function to add sleep interval
 * @tk:		Pointer to the timekeeper to be updated
 * @delta:	Pointer to the delta value in timespec64 format
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 *
 * Takes a timespec offset measuring a suspend interval and properly
 * adds the sleep offset to the timekeeping variables.
 */
<<<<<<< HEAD
static void __timekeeping_inject_sleeptime(struct timespec *delta)
{
	if (!timespec_valid_strict(delta)) {
		printk(KERN_WARNING "__timekeeping_inject_sleeptime: Invalid "
					"sleep delta value!\n");
		return;
	}

	timekeeper.xtime = timespec_add(timekeeper.xtime, *delta);
	timekeeper.wall_to_monotonic =
			timespec_sub(timekeeper.wall_to_monotonic, *delta);
	update_sleep_time(timespec_add(timekeeper.total_sleep_time, *delta));
}


/**
 * timekeeping_inject_sleeptime - Adds suspend interval to timeekeeping values
 * @delta: pointer to a timespec delta value
 *
 * This hook is for architectures that cannot support read_persistent_clock
 * because their RTC/persistent clock is only accessible when irqs are enabled.
=======
static void __timekeeping_inject_sleeptime(struct timekeeper *tk,
					   const struct timespec64 *delta)
{
	if (!timespec64_valid_strict(delta)) {
		printk_deferred(KERN_WARNING
				"__timekeeping_inject_sleeptime: Invalid "
				"sleep delta value!\n");
		return;
	}
	tk_xtime_add(tk, delta);
	tk_set_wall_to_mono(tk, timespec64_sub(tk->wall_to_monotonic, *delta));
	tk_update_sleep_time(tk, timespec64_to_ktime(*delta));
	tk_debug_account_sleep_time(delta);
}

#if defined(CONFIG_PM_SLEEP) && defined(CONFIG_RTC_HCTOSYS_DEVICE)
/*
 * We have three kinds of time sources to use for sleep time
 * injection, the preference order is:
 * 1) non-stop clocksource
 * 2) persistent clock (ie: RTC accessible when irqs are off)
 * 3) RTC
 *
 * 1) and 2) are used by timekeeping, 3) by RTC subsystem.
 * If system has neither 1) nor 2), 3) will be used finally.
 *
 *
 * If timekeeping has injected sleeptime via either 1) or 2),
 * 3) becomes needless, so in this case we don't need to call
 * rtc_resume(), and this is what timekeeping_rtc_skipresume()
 * means.
 */
bool timekeeping_rtc_skipresume(void)
{
	return !suspend_timing_needed;
}

/*
 * 1) can be determined whether to use or not only when doing
 * timekeeping_resume() which is invoked after rtc_suspend(),
 * so we can't skip rtc_suspend() surely if system has 1).
 *
 * But if system has 2), 2) will definitely be used, so in this
 * case we don't need to call rtc_suspend(), and this is what
 * timekeeping_rtc_skipsuspend() means.
 */
bool timekeeping_rtc_skipsuspend(void)
{
	return persistent_clock_exists;
}

/**
 * timekeeping_inject_sleeptime64 - Adds suspend interval to timeekeeping values
 * @delta: pointer to a timespec64 delta value
 *
 * This hook is for architectures that cannot support read_persistent_clock64
 * because their RTC/persistent clock is only accessible when irqs are enabled.
 * and also don't have an effective nonstop clocksource.
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 *
 * This function should only be called by rtc_resume(), and allows
 * a suspend offset to be injected into the timekeeping values.
 */
<<<<<<< HEAD
void timekeeping_inject_sleeptime(struct timespec *delta)
{
	unsigned long flags;
	struct timespec ts;

	/* Make sure we don't set the clock twice */
	read_persistent_clock(&ts);
	if (!(ts.tv_sec == 0 && ts.tv_nsec == 0))
		return;

	write_seqlock_irqsave(&timekeeper.lock, flags);

	timekeeping_forward_now();

	__timekeeping_inject_sleeptime(delta);

	timekeeping_update(true);

	write_sequnlock_irqrestore(&timekeeper.lock, flags);

	/* signal hrtimers about time change */
	clock_was_set();
}


/**
 * timekeeping_resume - Resumes the generic timekeeping subsystem.
 *
 * This is for the generic clocksource timekeeping.
 * xtime/wall_to_monotonic/jiffies/etc are
 * still managed by arch specific suspend/resume code.
 */
static void timekeeping_resume(void)
{
	struct timekeeper *tk = &timekeeper;
	struct clocksource *clock = tk->clock;
	unsigned long flags;
	struct timespec ts_new, ts_delta;
	cycle_t cycle_now, cycle_delta;
	bool suspendtime_found = false;

	read_persistent_clock(&ts_new);

	clocksource_resume();

	write_seqlock_irqsave(&timekeeper.lock, flags);
=======
void timekeeping_inject_sleeptime64(const struct timespec64 *delta)
{
	struct timekeeper *tk = &tk_core.timekeeper;
	unsigned long flags;

	raw_spin_lock_irqsave(&timekeeper_lock, flags);
	write_seqcount_begin(&tk_core.seq);

	suspend_timing_needed = false;

	timekeeping_forward_now(tk);

	__timekeeping_inject_sleeptime(tk, delta);

	timekeeping_update(tk, TK_CLEAR_NTP | TK_MIRROR | TK_CLOCK_WAS_SET);

	write_seqcount_end(&tk_core.seq);
	raw_spin_unlock_irqrestore(&timekeeper_lock, flags);

	/* Signal hrtimers about time change */
	clock_was_set(CLOCK_SET_WALL | CLOCK_SET_BOOT);
}
#endif

/**
 * timekeeping_resume - Resumes the generic timekeeping subsystem.
 */
void timekeeping_resume(void)
{
	struct timekeeper *tk = &tk_core.timekeeper;
	struct clocksource *clock = tk->tkr_mono.clock;
	unsigned long flags;
	struct timespec64 ts_new, ts_delta;
	u64 cycle_now, nsec;
	bool inject_sleeptime = false;

	read_persistent_clock64(&ts_new);

	clockevents_resume();
	clocksource_resume();

	raw_spin_lock_irqsave(&timekeeper_lock, flags);
	write_seqcount_begin(&tk_core.seq);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	/*
	 * After system resumes, we need to calculate the suspended time and
	 * compensate it for the OS time. There are 3 sources that could be
	 * used: Nonstop clocksource during suspend, persistent clock and rtc
	 * device.
	 *
	 * One specific platform may have 1 or 2 or all of them, and the
	 * preference will be:
	 *	suspend-nonstop clocksource -> persistent clock -> rtc
	 * The less preferred source will only be tried if there is no better
	 * usable source. The rtc part is handled separately in rtc core code.
	 */
<<<<<<< HEAD
	cycle_now = clock->read(clock);
	if ((clock->flags & CLOCK_SOURCE_SUSPEND_NONSTOP) &&
		cycle_now > clock->cycle_last) {
		u64 num, max = ULLONG_MAX;
		u32 mult = clock->mult;
		u32 shift = clock->shift;
		s64 nsec = 0;

		cycle_delta = (cycle_now - clock->cycle_last) & clock->mask;

		/*
		 * "cycle_delta * mutl" may cause 64 bits overflow, if the
		 * suspended time is too long. In that case we need do the
		 * 64 bits math carefully
		 */
		do_div(max, mult);
		if (cycle_delta > max) {
			num = div64_u64(cycle_delta, max);
			nsec = (((u64) max * mult) >> shift) * num;
			cycle_delta -= num * max;
		}
		nsec += ((u64) cycle_delta * mult) >> shift;

		ts_delta = ns_to_timespec(nsec);
		suspendtime_found = true;
	} else if (timespec_compare(&ts_new, &timekeeping_suspend_time) > 0) {
		ts_delta = timespec_sub(ts_new, timekeeping_suspend_time);
		suspendtime_found = true;
	}

	if (suspendtime_found)
		__timekeeping_inject_sleeptime(&ts_delta);

	/* Re-base the last cycle value */
	clock->cycle_last = cycle_now;
	tk->ntp_error = 0;
	timekeeping_suspended = 0;
	timekeeping_update(false);
	write_sequnlock_irqrestore(&timekeeper.lock, flags);

	touch_softlockup_watchdog();

	clockevents_notify(CLOCK_EVT_NOTIFY_RESUME, NULL);

	/* Resume hrtimers */
	hrtimers_resume();
#ifdef CONFIG_MACH_LGE
	pr_debug("TIME suspended for %lu.%03lu seconds\n", ts_delta.tv_sec,
		ts_delta.tv_nsec / NSEC_PER_MSEC);
#endif
}

static int timekeeping_suspend(void)
{
	unsigned long flags;
	struct timespec		delta, delta_delta;
	static struct timespec	old_delta;

	read_persistent_clock(&timekeeping_suspend_time);

	write_seqlock_irqsave(&timekeeper.lock, flags);
	timekeeping_forward_now();
	timekeeping_suspended = 1;

	/*
	 * To avoid drift caused by repeated suspend/resumes,
	 * which each can add ~1 second drift error,
	 * try to compensate so the difference in system time
	 * and persistent_clock time stays close to constant.
	 */
	delta = timespec_sub(timekeeper.xtime, timekeeping_suspend_time);
	delta_delta = timespec_sub(delta, old_delta);
	if (abs(delta_delta.tv_sec)  >= 2) {
		/*
		 * if delta_delta is too large, assume time correction
		 * has occured and set old_delta to the current delta.
		 */
		old_delta = delta;
	} else {
		/* Otherwise try to adjust old_system to compensate */
		timekeeping_suspend_time =
			timespec_add(timekeeping_suspend_time, delta_delta);
	}
	write_sequnlock_irqrestore(&timekeeper.lock, flags);

	clockevents_notify(CLOCK_EVT_NOTIFY_SUSPEND, NULL);
	clocksource_suspend();
=======
	cycle_now = tk_clock_read(&tk->tkr_mono);
	nsec = clocksource_stop_suspend_timing(clock, cycle_now);
	if (nsec > 0) {
		ts_delta = ns_to_timespec64(nsec);
		inject_sleeptime = true;
	} else if (timespec64_compare(&ts_new, &timekeeping_suspend_time) > 0) {
		ts_delta = timespec64_sub(ts_new, timekeeping_suspend_time);
		inject_sleeptime = true;
	}

	if (inject_sleeptime) {
		suspend_timing_needed = false;
		__timekeeping_inject_sleeptime(tk, &ts_delta);
	}

	/* Re-base the last cycle value */
	tk->tkr_mono.cycle_last = cycle_now;
	tk->tkr_raw.cycle_last  = cycle_now;

	tk->ntp_error = 0;
	timekeeping_suspended = 0;
	timekeeping_update(tk, TK_MIRROR | TK_CLOCK_WAS_SET);
	write_seqcount_end(&tk_core.seq);
	raw_spin_unlock_irqrestore(&timekeeper_lock, flags);

	touch_softlockup_watchdog();

	/* Resume the clockevent device(s) and hrtimers */
	tick_resume();
	/* Notify timerfd as resume is equivalent to clock_was_set() */
	timerfd_resume();
}

int timekeeping_suspend(void)
{
	struct timekeeper *tk = &tk_core.timekeeper;
	unsigned long flags;
	struct timespec64		delta, delta_delta;
	static struct timespec64	old_delta;
	struct clocksource *curr_clock;
	u64 cycle_now;

	read_persistent_clock64(&timekeeping_suspend_time);

	/*
	 * On some systems the persistent_clock can not be detected at
	 * timekeeping_init by its return value, so if we see a valid
	 * value returned, update the persistent_clock_exists flag.
	 */
	if (timekeeping_suspend_time.tv_sec || timekeeping_suspend_time.tv_nsec)
		persistent_clock_exists = true;

	suspend_timing_needed = true;

	raw_spin_lock_irqsave(&timekeeper_lock, flags);
	write_seqcount_begin(&tk_core.seq);
	timekeeping_forward_now(tk);
	timekeeping_suspended = 1;

	/*
	 * Since we've called forward_now, cycle_last stores the value
	 * just read from the current clocksource. Save this to potentially
	 * use in suspend timing.
	 */
	curr_clock = tk->tkr_mono.clock;
	cycle_now = tk->tkr_mono.cycle_last;
	clocksource_start_suspend_timing(curr_clock, cycle_now);

	if (persistent_clock_exists) {
		/*
		 * To avoid drift caused by repeated suspend/resumes,
		 * which each can add ~1 second drift error,
		 * try to compensate so the difference in system time
		 * and persistent_clock time stays close to constant.
		 */
		delta = timespec64_sub(tk_xtime(tk), timekeeping_suspend_time);
		delta_delta = timespec64_sub(delta, old_delta);
		if (abs(delta_delta.tv_sec) >= 2) {
			/*
			 * if delta_delta is too large, assume time correction
			 * has occurred and set old_delta to the current delta.
			 */
			old_delta = delta;
		} else {
			/* Otherwise try to adjust old_system to compensate */
			timekeeping_suspend_time =
				timespec64_add(timekeeping_suspend_time, delta_delta);
		}
	}

	timekeeping_update(tk, TK_MIRROR);
	halt_fast_timekeeper(tk);
	write_seqcount_end(&tk_core.seq);
	raw_spin_unlock_irqrestore(&timekeeper_lock, flags);

	tick_suspend();
	clocksource_suspend();
	clockevents_suspend();
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	return 0;
}

/* sysfs resume/suspend bits for timekeeping */
static struct syscore_ops timekeeping_syscore_ops = {
	.resume		= timekeeping_resume,
	.suspend	= timekeeping_suspend,
};

static int __init timekeeping_init_ops(void)
{
	register_syscore_ops(&timekeeping_syscore_ops);
	return 0;
}
<<<<<<< HEAD

device_initcall(timekeeping_init_ops);

/*
 * If the error is already larger, we look ahead even further
 * to compensate for late or lost adjustments.
 */
static __always_inline int timekeeping_bigadjust(s64 error, s64 *interval,
						 s64 *offset)
{
	s64 tick_error, i;
	u32 look_ahead, adj;
	s32 error2, mult;

	/*
	 * Use the current error value to determine how much to look ahead.
	 * The larger the error the slower we adjust for it to avoid problems
	 * with losing too many ticks, otherwise we would overadjust and
	 * produce an even larger error.  The smaller the adjustment the
	 * faster we try to adjust for it, as lost ticks can do less harm
	 * here.  This is tuned so that an error of about 1 msec is adjusted
	 * within about 1 sec (or 2^20 nsec in 2^SHIFT_HZ ticks).
	 */
	error2 = timekeeper.ntp_error >> (NTP_SCALE_SHIFT + 22 - 2 * SHIFT_HZ);
	error2 = abs(error2);
	for (look_ahead = 0; error2 > 0; look_ahead++)
		error2 >>= 2;

	/*
	 * Now calculate the error in (1 << look_ahead) ticks, but first
	 * remove the single look ahead already included in the error.
	 */
	tick_error = ntp_tick_length() >> (timekeeper.ntp_error_shift + 1);
	tick_error -= timekeeper.xtime_interval >> 1;
	error = ((error - tick_error) >> look_ahead) + tick_error;

	/* Finally calculate the adjustment shift value.  */
	i = *interval;
	mult = 1;
	if (error < 0) {
		error = -error;
		*interval = -*interval;
		*offset = -*offset;
		mult = -1;
	}
	for (adj = 0; error > i; adj++)
		error >>= 1;

	*interval <<= adj;
	*offset <<= adj;
	return mult << adj;
}

/*
 * Adjust the multiplier to reduce the error value,
 * this is optimized for the most common adjustments of -1,0,1,
 * for other values we can do a bit more work.
 */
static void timekeeping_adjust(s64 offset)
{
	s64 error, interval = timekeeper.cycle_interval;
	int adj;

	/*
	 * The point of this is to check if the error is greater than half
	 * an interval.
	 *
	 * First we shift it down from NTP_SHIFT to clocksource->shifted nsecs.
	 *
	 * Note we subtract one in the shift, so that error is really error*2.
	 * This "saves" dividing(shifting) interval twice, but keeps the
	 * (error > interval) comparison as still measuring if error is
	 * larger than half an interval.
	 *
	 * Note: It does not "save" on aggravation when reading the code.
	 */
	error = timekeeper.ntp_error >> (timekeeper.ntp_error_shift - 1);
	if (error > interval) {
		/*
		 * We now divide error by 4(via shift), which checks if
		 * the error is greater than twice the interval.
		 * If it is greater, we need a bigadjust, if its smaller,
		 * we can adjust by 1.
		 */
		error >>= 2;
		/*
		 * XXX - In update_wall_time, we round up to the next
		 * nanosecond, and store the amount rounded up into
		 * the error. This causes the likely below to be unlikely.
		 *
		 * The proper fix is to avoid rounding up by using
		 * the high precision timekeeper.xtime_nsec instead of
		 * xtime.tv_nsec everywhere. Fixing this will take some
		 * time.
		 */
		if (likely(error <= interval))
			adj = 1;
		else
			adj = timekeeping_bigadjust(error, &interval, &offset);
	} else if (error < -interval) {
		/* See comment above, this is just switched for the negative */
		error >>= 2;
		if (likely(error >= -interval)) {
			adj = -1;
			interval = -interval;
			offset = -offset;
		} else
			adj = timekeeping_bigadjust(error, &interval, &offset);
	} else /* No adjustment needed */
		return;

	if (unlikely(timekeeper.clock->maxadj &&
			(timekeeper.mult + adj >
			timekeeper.clock->mult + timekeeper.clock->maxadj))) {
		printk_once(KERN_WARNING
			"Adjusting %s more than 11%% (%ld vs %ld)\n",
			timekeeper.clock->name, (long)timekeeper.mult + adj,
			(long)timekeeper.clock->mult +
				timekeeper.clock->maxadj);
	}
	/*
	 * So the following can be confusing.
	 *
	 * To keep things simple, lets assume adj == 1 for now.
	 *
	 * When adj != 1, remember that the interval and offset values
=======
device_initcall(timekeeping_init_ops);

/*
 * Apply a multiplier adjustment to the timekeeper
 */
static __always_inline void timekeeping_apply_adjustment(struct timekeeper *tk,
							 s64 offset,
							 s32 mult_adj)
{
	s64 interval = tk->cycle_interval;

	if (mult_adj == 0) {
		return;
	} else if (mult_adj == -1) {
		interval = -interval;
		offset = -offset;
	} else if (mult_adj != 1) {
		interval *= mult_adj;
		offset *= mult_adj;
	}

	/*
	 * So the following can be confusing.
	 *
	 * To keep things simple, lets assume mult_adj == 1 for now.
	 *
	 * When mult_adj != 1, remember that the interval and offset values
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	 * have been appropriately scaled so the math is the same.
	 *
	 * The basic idea here is that we're increasing the multiplier
	 * by one, this causes the xtime_interval to be incremented by
	 * one cycle_interval. This is because:
	 *	xtime_interval = cycle_interval * mult
	 * So if mult is being incremented by one:
	 *	xtime_interval = cycle_interval * (mult + 1)
	 * Its the same as:
	 *	xtime_interval = (cycle_interval * mult) + cycle_interval
	 * Which can be shortened to:
	 *	xtime_interval += cycle_interval
	 *
	 * So offset stores the non-accumulated cycles. Thus the current
	 * time (in shifted nanoseconds) is:
	 *	now = (offset * adj) + xtime_nsec
	 * Now, even though we're adjusting the clock frequency, we have
	 * to keep time consistent. In other words, we can't jump back
	 * in time, and we also want to avoid jumping forward in time.
	 *
	 * So given the same offset value, we need the time to be the same
	 * both before and after the freq adjustment.
	 *	now = (offset * adj_1) + xtime_nsec_1
	 *	now = (offset * adj_2) + xtime_nsec_2
	 * So:
	 *	(offset * adj_1) + xtime_nsec_1 =
	 *		(offset * adj_2) + xtime_nsec_2
	 * And we know:
	 *	adj_2 = adj_1 + 1
	 * So:
	 *	(offset * adj_1) + xtime_nsec_1 =
	 *		(offset * (adj_1+1)) + xtime_nsec_2
	 *	(offset * adj_1) + xtime_nsec_1 =
	 *		(offset * adj_1) + offset + xtime_nsec_2
	 * Canceling the sides:
	 *	xtime_nsec_1 = offset + xtime_nsec_2
	 * Which gives us:
	 *	xtime_nsec_2 = xtime_nsec_1 - offset
<<<<<<< HEAD
	 * Which simplfies to:
	 *	xtime_nsec -= offset
	 *
	 * XXX - TODO: Doc ntp_error calculation.
	 */
	timekeeper.mult += adj;
	timekeeper.xtime_interval += interval;
	timekeeper.xtime_nsec -= offset;
	timekeeper.ntp_error -= (interval - offset) <<
				timekeeper.ntp_error_shift;
}


/**
 * logarithmic_accumulation - shifted accumulation of cycles
 *
 * This functions accumulates a shifted interval of cycles into
 * into a shifted interval nanoseconds. Allows for O(log) accumulation
=======
	 * Which simplifies to:
	 *	xtime_nsec -= offset
	 */
	if ((mult_adj > 0) && (tk->tkr_mono.mult + mult_adj < mult_adj)) {
		/* NTP adjustment caused clocksource mult overflow */
		WARN_ON_ONCE(1);
		return;
	}

	tk->tkr_mono.mult += mult_adj;
	tk->xtime_interval += interval;
	tk->tkr_mono.xtime_nsec -= offset;
}

/*
 * Adjust the timekeeper's multiplier to the correct frequency
 * and also to reduce the accumulated error value.
 */
static void timekeeping_adjust(struct timekeeper *tk, s64 offset)
{
	u32 mult;

	/*
	 * Determine the multiplier from the current NTP tick length.
	 * Avoid expensive division when the tick length doesn't change.
	 */
	if (likely(tk->ntp_tick == ntp_tick_length())) {
		mult = tk->tkr_mono.mult - tk->ntp_err_mult;
	} else {
		tk->ntp_tick = ntp_tick_length();
		mult = div64_u64((tk->ntp_tick >> tk->ntp_error_shift) -
				 tk->xtime_remainder, tk->cycle_interval);
	}

	/*
	 * If the clock is behind the NTP time, increase the multiplier by 1
	 * to catch up with it. If it's ahead and there was a remainder in the
	 * tick division, the clock will slow down. Otherwise it will stay
	 * ahead until the tick length changes to a non-divisible value.
	 */
	tk->ntp_err_mult = tk->ntp_error > 0 ? 1 : 0;
	mult += tk->ntp_err_mult;

	timekeeping_apply_adjustment(tk, offset, mult - tk->tkr_mono.mult);

	if (unlikely(tk->tkr_mono.clock->maxadj &&
		(abs(tk->tkr_mono.mult - tk->tkr_mono.clock->mult)
			> tk->tkr_mono.clock->maxadj))) {
		printk_once(KERN_WARNING
			"Adjusting %s more than 11%% (%ld vs %ld)\n",
			tk->tkr_mono.clock->name, (long)tk->tkr_mono.mult,
			(long)tk->tkr_mono.clock->mult + tk->tkr_mono.clock->maxadj);
	}

	/*
	 * It may be possible that when we entered this function, xtime_nsec
	 * was very small.  Further, if we're slightly speeding the clocksource
	 * in the code above, its possible the required corrective factor to
	 * xtime_nsec could cause it to underflow.
	 *
	 * Now, since we have already accumulated the second and the NTP
	 * subsystem has been notified via second_overflow(), we need to skip
	 * the next update.
	 */
	if (unlikely((s64)tk->tkr_mono.xtime_nsec < 0)) {
		tk->tkr_mono.xtime_nsec += (u64)NSEC_PER_SEC <<
							tk->tkr_mono.shift;
		tk->xtime_sec--;
		tk->skip_second_overflow = 1;
	}
}

/*
 * accumulate_nsecs_to_secs - Accumulates nsecs into secs
 *
 * Helper function that accumulates the nsecs greater than a second
 * from the xtime_nsec field to the xtime_secs field.
 * It also calls into the NTP code to handle leapsecond processing.
 */
static inline unsigned int accumulate_nsecs_to_secs(struct timekeeper *tk)
{
	u64 nsecps = (u64)NSEC_PER_SEC << tk->tkr_mono.shift;
	unsigned int clock_set = 0;

	while (tk->tkr_mono.xtime_nsec >= nsecps) {
		int leap;

		tk->tkr_mono.xtime_nsec -= nsecps;
		tk->xtime_sec++;

		/*
		 * Skip NTP update if this second was accumulated before,
		 * i.e. xtime_nsec underflowed in timekeeping_adjust()
		 */
		if (unlikely(tk->skip_second_overflow)) {
			tk->skip_second_overflow = 0;
			continue;
		}

		/* Figure out if its a leap sec and apply if needed */
		leap = second_overflow(tk->xtime_sec);
		if (unlikely(leap)) {
			struct timespec64 ts;

			tk->xtime_sec += leap;

			ts.tv_sec = leap;
			ts.tv_nsec = 0;
			tk_set_wall_to_mono(tk,
				timespec64_sub(tk->wall_to_monotonic, ts));

			__timekeeping_set_tai_offset(tk, tk->tai_offset - leap);

			clock_set = TK_CLOCK_WAS_SET;
		}
	}
	return clock_set;
}

/*
 * logarithmic_accumulation - shifted accumulation of cycles
 *
 * This functions accumulates a shifted interval of cycles into
 * a shifted interval nanoseconds. Allows for O(log) accumulation
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 * loop.
 *
 * Returns the unconsumed cycles.
 */
<<<<<<< HEAD
static cycle_t logarithmic_accumulation(cycle_t offset, int shift,
							unsigned int *clock_set)
{
	u64 nsecps = (u64)NSEC_PER_SEC << timekeeper.shift;
	u64 raw_nsecs;

	/* If the offset is smaller than a shifted interval, do nothing */
	if (offset < timekeeper.cycle_interval<<shift)
		return offset;

	/* Accumulate one shifted interval */
	offset -= timekeeper.cycle_interval << shift;
	timekeeper.clock->cycle_last += timekeeper.cycle_interval << shift;

	timekeeper.xtime_nsec += timekeeper.xtime_interval << shift;
	while (timekeeper.xtime_nsec >= nsecps) {
		int leap;
		timekeeper.xtime_nsec -= nsecps;
		timekeeper.xtime.tv_sec++;
		leap = second_overflow(timekeeper.xtime.tv_sec);
		timekeeper.xtime.tv_sec += leap;
		timekeeper.wall_to_monotonic.tv_sec -= leap;
		if (leap)
			*clock_set = 1;
	}

	/* Accumulate raw time */
	raw_nsecs = (u64)timekeeper.raw_interval << shift;
	raw_nsecs += timekeeper.raw_time.tv_nsec;
	if (raw_nsecs >= NSEC_PER_SEC) {
		u64 raw_secs = raw_nsecs;
		raw_nsecs = do_div(raw_secs, NSEC_PER_SEC);
		timekeeper.raw_time.tv_sec += raw_secs;
	}
	timekeeper.raw_time.tv_nsec = raw_nsecs;

	/* Accumulate error between NTP and clock interval */
	timekeeper.ntp_error += ntp_tick_length() << shift;
	timekeeper.ntp_error -=
	    (timekeeper.xtime_interval + timekeeper.xtime_remainder) <<
				(timekeeper.ntp_error_shift + shift);
=======
static u64 logarithmic_accumulation(struct timekeeper *tk, u64 offset,
				    u32 shift, unsigned int *clock_set)
{
	u64 interval = tk->cycle_interval << shift;
	u64 snsec_per_sec;

	/* If the offset is smaller than a shifted interval, do nothing */
	if (offset < interval)
		return offset;

	/* Accumulate one shifted interval */
	offset -= interval;
	tk->tkr_mono.cycle_last += interval;
	tk->tkr_raw.cycle_last  += interval;

	tk->tkr_mono.xtime_nsec += tk->xtime_interval << shift;
	*clock_set |= accumulate_nsecs_to_secs(tk);

	/* Accumulate raw time */
	tk->tkr_raw.xtime_nsec += tk->raw_interval << shift;
	snsec_per_sec = (u64)NSEC_PER_SEC << tk->tkr_raw.shift;
	while (tk->tkr_raw.xtime_nsec >= snsec_per_sec) {
		tk->tkr_raw.xtime_nsec -= snsec_per_sec;
		tk->raw_sec++;
	}

	/* Accumulate error between NTP and clock interval */
	tk->ntp_error += tk->ntp_tick << shift;
	tk->ntp_error -= (tk->xtime_interval + tk->xtime_remainder) <<
						(tk->ntp_error_shift + shift);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	return offset;
}

<<<<<<< HEAD

/**
 * update_wall_time - Uses the current clocksource to increment the wall time
 *
 */
static void update_wall_time(void)
{
	struct clocksource *clock;
	cycle_t offset;
=======
/*
 * timekeeping_advance - Updates the timekeeper to the current time and
 * current NTP tick length
 */
static bool timekeeping_advance(enum timekeeping_adv_mode mode)
{
	struct timekeeper *real_tk = &tk_core.timekeeper;
	struct timekeeper *tk = &shadow_timekeeper;
	u64 offset;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	int shift = 0, maxshift;
	unsigned int clock_set = 0;
	unsigned long flags;

<<<<<<< HEAD
	write_seqlock_irqsave(&timekeeper.lock, flags);
=======
	raw_spin_lock_irqsave(&timekeeper_lock, flags);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	/* Make sure we're fully resumed: */
	if (unlikely(timekeeping_suspended))
		goto out;

<<<<<<< HEAD
	clock = timekeeper.clock;

#ifdef CONFIG_ARCH_USES_GETTIMEOFFSET
	offset = timekeeper.cycle_interval;
#else
	offset = (clock->read(clock) - clock->cycle_last) & clock->mask;
#endif
	/* Check if there's really nothing to do */
	if (offset < timekeeper.cycle_interval)
		goto out;

	timekeeper.xtime_nsec = (s64)timekeeper.xtime.tv_nsec <<
						timekeeper.shift;
=======
	offset = clocksource_delta(tk_clock_read(&tk->tkr_mono),
				   tk->tkr_mono.cycle_last, tk->tkr_mono.mask);

	/* Check if there's really nothing to do */
	if (offset < real_tk->cycle_interval && mode == TK_ADV_TICK)
		goto out;

	/* Do some additional sanity checking */
	timekeeping_check_update(tk, offset);

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	/*
	 * With NO_HZ we may have to accumulate many cycle_intervals
	 * (think "ticks") worth of time at once. To do this efficiently,
	 * we calculate the largest doubling multiple of cycle_intervals
	 * that is smaller than the offset.  We then accumulate that
	 * chunk in one go, and then try to consume the next smaller
	 * doubled multiple.
	 */
<<<<<<< HEAD
	shift = ilog2(offset) - ilog2(timekeeper.cycle_interval);
=======
	shift = ilog2(offset) - ilog2(tk->cycle_interval);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	shift = max(0, shift);
	/* Bound shift to one less than what overflows tick_length */
	maxshift = (64 - (ilog2(ntp_tick_length())+1)) - 1;
	shift = min(shift, maxshift);
<<<<<<< HEAD
	while (offset >= timekeeper.cycle_interval) {
		offset = logarithmic_accumulation(offset, shift, &clock_set);
		if(offset < timekeeper.cycle_interval<<shift)
			shift--;
	}

	/* correct the clock when NTP error is too big */
	timekeeping_adjust(offset);

	/*
	 * Since in the loop above, we accumulate any amount of time
	 * in xtime_nsec over a second into xtime.tv_sec, its possible for
	 * xtime_nsec to be fairly small after the loop. Further, if we're
	 * slightly speeding the clocksource up in timekeeping_adjust(),
	 * its possible the required corrective factor to xtime_nsec could
	 * cause it to underflow.
	 *
	 * Now, we cannot simply roll the accumulated second back, since
	 * the NTP subsystem has been notified via second_overflow. So
	 * instead we push xtime_nsec forward by the amount we underflowed,
	 * and add that amount into the error.
	 *
	 * We'll correct this error next time through this function, when
	 * xtime_nsec is not as small.
	 */
	if (unlikely((s64)timekeeper.xtime_nsec < 0)) {
		s64 neg = -(s64)timekeeper.xtime_nsec;
		timekeeper.xtime_nsec = 0;
		timekeeper.ntp_error += neg << timekeeper.ntp_error_shift;
	}


	/*
	 * Store full nanoseconds into xtime after rounding it up and
	 * add the remainder to the error difference.
	 */
	timekeeper.xtime.tv_nsec = ((s64)timekeeper.xtime_nsec >>
						timekeeper.shift) + 1;
	timekeeper.xtime_nsec -= (s64)timekeeper.xtime.tv_nsec <<
						timekeeper.shift;
	timekeeper.ntp_error +=	timekeeper.xtime_nsec <<
				timekeeper.ntp_error_shift;

	/*
	 * Finally, make sure that after the rounding
	 * xtime.tv_nsec isn't larger than NSEC_PER_SEC
	 */
	if (unlikely(timekeeper.xtime.tv_nsec >= NSEC_PER_SEC)) {
		int leap;
		timekeeper.xtime.tv_nsec -= NSEC_PER_SEC;
		timekeeper.xtime.tv_sec++;
		leap = second_overflow(timekeeper.xtime.tv_sec);
		timekeeper.xtime.tv_sec += leap;
		timekeeper.wall_to_monotonic.tv_sec -= leap;
		if (leap)
			clock_set = 1;
	}

	timekeeping_update(false);

out:
	write_sequnlock_irqrestore(&timekeeper.lock, flags);

	if (clock_set)
=======
	while (offset >= tk->cycle_interval) {
		offset = logarithmic_accumulation(tk, offset, shift,
							&clock_set);
		if (offset < tk->cycle_interval<<shift)
			shift--;
	}

	/* Adjust the multiplier to correct NTP error */
	timekeeping_adjust(tk, offset);

	/*
	 * Finally, make sure that after the rounding
	 * xtime_nsec isn't larger than NSEC_PER_SEC
	 */
	clock_set |= accumulate_nsecs_to_secs(tk);

	write_seqcount_begin(&tk_core.seq);
	/*
	 * Update the real timekeeper.
	 *
	 * We could avoid this memcpy by switching pointers, but that
	 * requires changes to all other timekeeper usage sites as
	 * well, i.e. move the timekeeper pointer getter into the
	 * spinlocked/seqcount protected sections. And we trade this
	 * memcpy under the tk_core.seq against one before we start
	 * updating.
	 */
	timekeeping_update(tk, clock_set);
	memcpy(real_tk, tk, sizeof(*tk));
	/* The memcpy must come last. Do not put anything here! */
	write_seqcount_end(&tk_core.seq);
out:
	raw_spin_unlock_irqrestore(&timekeeper_lock, flags);

	return !!clock_set;
}

/**
 * update_wall_time - Uses the current clocksource to increment the wall time
 *
 */
void update_wall_time(void)
{
	if (timekeeping_advance(TK_ADV_TICK))
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		clock_was_set_delayed();
}

/**
<<<<<<< HEAD
 * getboottime - Return the real time of system boot.
 * @ts:		pointer to the timespec to be set
 *
 * Returns the wall-time of boot in a timespec.
=======
 * getboottime64 - Return the real time of system boot.
 * @ts:		pointer to the timespec64 to be set
 *
 * Returns the wall-time of boot in a timespec64.
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 *
 * This is based on the wall_to_monotonic offset and the total suspend
 * time. Calls to settimeofday will affect the value returned (which
 * basically means that however wrong your real time clock is at boot time,
 * you get the right time here).
 */
<<<<<<< HEAD
void getboottime(struct timespec *ts)
{
	struct timespec boottime = {
		.tv_sec = timekeeper.wall_to_monotonic.tv_sec +
				timekeeper.total_sleep_time.tv_sec,
		.tv_nsec = timekeeper.wall_to_monotonic.tv_nsec +
				timekeeper.total_sleep_time.tv_nsec
	};

	set_normalized_timespec(ts, -boottime.tv_sec, -boottime.tv_nsec);
}
EXPORT_SYMBOL_GPL(getboottime);


/**
 * get_monotonic_boottime - Returns monotonic time since boot
 * @ts:		pointer to the timespec to be set
 *
 * Returns the monotonic time since boot in a timespec.
 *
 * This is similar to CLOCK_MONTONIC/ktime_get_ts, but also
 * includes the time spent in suspend.
 */
void get_monotonic_boottime(struct timespec *ts)
{
	struct timespec tomono, sleep;
	unsigned int seq;
	s64 nsecs;

	WARN_ON(timekeeping_suspended);

	do {
		seq = read_seqbegin(&timekeeper.lock);
		*ts = timekeeper.xtime;
		tomono = timekeeper.wall_to_monotonic;
		sleep = timekeeper.total_sleep_time;
		nsecs = timekeeping_get_ns();

	} while (read_seqretry(&timekeeper.lock, seq));

	set_normalized_timespec(ts, ts->tv_sec + tomono.tv_sec + sleep.tv_sec,
		(s64)ts->tv_nsec + tomono.tv_nsec + sleep.tv_nsec + nsecs);
}
EXPORT_SYMBOL_GPL(get_monotonic_boottime);

/**
 * ktime_get_boottime - Returns monotonic time since boot in a ktime
 *
 * Returns the monotonic time since boot in a ktime
 *
 * This is similar to CLOCK_MONTONIC/ktime_get, but also
 * includes the time spent in suspend.
 */
ktime_t ktime_get_boottime(void)
{
	struct timespec ts;

	get_monotonic_boottime(&ts);
	return timespec_to_ktime(ts);
}
EXPORT_SYMBOL_GPL(ktime_get_boottime);

/**
 * monotonic_to_bootbased - Convert the monotonic time to boot based.
 * @ts:		pointer to the timespec to be converted
 */
void monotonic_to_bootbased(struct timespec *ts)
{
	*ts = timespec_add(*ts, timekeeper.total_sleep_time);
}
EXPORT_SYMBOL_GPL(monotonic_to_bootbased);

unsigned long get_seconds(void)
{
	return timekeeper.xtime.tv_sec;
}
EXPORT_SYMBOL(get_seconds);

struct timespec __current_kernel_time(void)
{
	return timekeeper.xtime;
}

struct timespec current_kernel_time(void)
{
	struct timespec now;
	unsigned long seq;

	do {
		seq = read_seqbegin(&timekeeper.lock);

		now = timekeeper.xtime;
	} while (read_seqretry(&timekeeper.lock, seq));

	return now;
}
EXPORT_SYMBOL(current_kernel_time);

struct timespec get_monotonic_coarse(void)
{
	struct timespec now, mono;
	unsigned long seq;

	do {
		seq = read_seqbegin(&timekeeper.lock);

		now = timekeeper.xtime;
		mono = timekeeper.wall_to_monotonic;
	} while (read_seqretry(&timekeeper.lock, seq));

	set_normalized_timespec(&now, now.tv_sec + mono.tv_sec,
				now.tv_nsec + mono.tv_nsec);
	return now;
}

/*
 * The 64-bit jiffies value is not atomic - you MUST NOT read it
 * without sampling the sequence number in xtime_lock.
 * jiffies is defined in the linker script...
=======
void getboottime64(struct timespec64 *ts)
{
	struct timekeeper *tk = &tk_core.timekeeper;
	ktime_t t = ktime_sub(tk->offs_real, tk->offs_boot);

	*ts = ktime_to_timespec64(t);
}
EXPORT_SYMBOL_GPL(getboottime64);

void ktime_get_coarse_real_ts64(struct timespec64 *ts)
{
	struct timekeeper *tk = &tk_core.timekeeper;
	unsigned int seq;

	do {
		seq = read_seqcount_begin(&tk_core.seq);

		*ts = tk_xtime(tk);
	} while (read_seqcount_retry(&tk_core.seq, seq));
}
EXPORT_SYMBOL(ktime_get_coarse_real_ts64);

void ktime_get_coarse_ts64(struct timespec64 *ts)
{
	struct timekeeper *tk = &tk_core.timekeeper;
	struct timespec64 now, mono;
	unsigned int seq;

	do {
		seq = read_seqcount_begin(&tk_core.seq);

		now = tk_xtime(tk);
		mono = tk->wall_to_monotonic;
	} while (read_seqcount_retry(&tk_core.seq, seq));

	set_normalized_timespec64(ts, now.tv_sec + mono.tv_sec,
				now.tv_nsec + mono.tv_nsec);
}
EXPORT_SYMBOL(ktime_get_coarse_ts64);

/*
 * Must hold jiffies_lock
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 */
void do_timer(unsigned long ticks)
{
	jiffies_64 += ticks;
<<<<<<< HEAD
	update_wall_time();
	calc_global_load(ticks);
}

/**
 * get_xtime_and_monotonic_and_sleep_offset() - get xtime, wall_to_monotonic,
 *    and sleep offsets.
 * @xtim:	pointer to timespec to be set with xtime
 * @wtom:	pointer to timespec to be set with wall_to_monotonic
 * @sleep:	pointer to timespec to be set with time in suspend
 */
void get_xtime_and_monotonic_and_sleep_offset(struct timespec *xtim,
				struct timespec *wtom, struct timespec *sleep)
{
	unsigned long seq;

	do {
		seq = read_seqbegin(&timekeeper.lock);
		*xtim = timekeeper.xtime;
		*wtom = timekeeper.wall_to_monotonic;
		*sleep = timekeeper.total_sleep_time;
	} while (read_seqretry(&timekeeper.lock, seq));
}

#ifdef CONFIG_HIGH_RES_TIMERS
/**
 * ktime_get_update_offsets - hrtimer helper
 * @offs_real:	pointer to storage for monotonic -> realtime offset
 * @offs_boot:	pointer to storage for monotonic -> boottime offset
 *
 * Returns current monotonic time and updates the offsets
 * Called from hrtimer_interupt() or retrigger_next_event()
 */
ktime_t ktime_get_update_offsets(ktime_t *offs_real, ktime_t *offs_boot)
{
	ktime_t now;
	unsigned int seq;
	u64 secs, nsecs;

	do {
		seq = read_seqbegin(&timekeeper.lock);

		secs = timekeeper.xtime.tv_sec;
		nsecs = timekeeper.xtime.tv_nsec;
		nsecs += timekeeping_get_ns();
		/* If arch requires, add in gettimeoffset() */
		nsecs += arch_gettimeoffset();

		*offs_real = timekeeper.offs_real;
		*offs_boot = timekeeper.offs_boot;

		now = ktime_add_ns(ktime_set(secs, 0), nsecs);
		now = ktime_sub(now, *offs_real);

		/* Handle leapsecond insertion adjustments */
		if (unlikely(now.tv64 >= timekeeper.next_leap_ktime.tv64))
			*offs_real = ktime_sub(timekeeper.offs_real, ktime_set(1, 0));

	} while (read_seqretry(&timekeeper.lock, seq));

	return now;
}
#endif

/**
 * ktime_get_monotonic_offset() - get wall_to_monotonic in ktime_t format
 */
ktime_t ktime_get_monotonic_offset(void)
{
	unsigned long seq;
	struct timespec wtom;

	do {
		seq = read_seqbegin(&timekeeper.lock);
		wtom = timekeeper.wall_to_monotonic;
	} while (read_seqretry(&timekeeper.lock, seq));

	return timespec_to_ktime(wtom);
}
EXPORT_SYMBOL_GPL(ktime_get_monotonic_offset);

/*
 * do_adjtimex() - Accessor function to NTP __do_adjtimex function
 */
int do_adjtimex(struct timex *txc)
{
	int ret;
	ret = __do_adjtimex(txc);
	tk_update_leap_state(&timekeeper);
	return ret;
}

/**
 * xtime_update() - advances the timekeeping infrastructure
 * @ticks:	number of ticks, that have elapsed since the last call.
 *
 * Must be called with interrupts disabled.
 */
void xtime_update(unsigned long ticks)
{
	write_seqlock(&xtime_lock);
	do_timer(ticks);
	write_sequnlock(&xtime_lock);
}
=======
	calc_global_load();
}

/**
 * ktime_get_update_offsets_now - hrtimer helper
 * @cwsseq:	pointer to check and store the clock was set sequence number
 * @offs_real:	pointer to storage for monotonic -> realtime offset
 * @offs_boot:	pointer to storage for monotonic -> boottime offset
 * @offs_tai:	pointer to storage for monotonic -> clock tai offset
 *
 * Returns current monotonic time and updates the offsets if the
 * sequence number in @cwsseq and timekeeper.clock_was_set_seq are
 * different.
 *
 * Called from hrtimer_interrupt() or retrigger_next_event()
 */
ktime_t ktime_get_update_offsets_now(unsigned int *cwsseq, ktime_t *offs_real,
				     ktime_t *offs_boot, ktime_t *offs_tai)
{
	struct timekeeper *tk = &tk_core.timekeeper;
	unsigned int seq;
	ktime_t base;
	u64 nsecs;

	do {
		seq = read_seqcount_begin(&tk_core.seq);

		base = tk->tkr_mono.base;
		nsecs = timekeeping_get_ns(&tk->tkr_mono);
		base = ktime_add_ns(base, nsecs);

		if (*cwsseq != tk->clock_was_set_seq) {
			*cwsseq = tk->clock_was_set_seq;
			*offs_real = tk->offs_real;
			*offs_boot = tk->offs_boot;
			*offs_tai = tk->offs_tai;
		}

		/* Handle leapsecond insertion adjustments */
		if (unlikely(base >= tk->next_leap_ktime))
			*offs_real = ktime_sub(tk->offs_real, ktime_set(1, 0));

	} while (read_seqcount_retry(&tk_core.seq, seq));

	return base;
}

/*
 * timekeeping_validate_timex - Ensures the timex is ok for use in do_adjtimex
 */
static int timekeeping_validate_timex(const struct __kernel_timex *txc)
{
	if (txc->modes & ADJ_ADJTIME) {
		/* singleshot must not be used with any other mode bits */
		if (!(txc->modes & ADJ_OFFSET_SINGLESHOT))
			return -EINVAL;
		if (!(txc->modes & ADJ_OFFSET_READONLY) &&
		    !capable(CAP_SYS_TIME))
			return -EPERM;
	} else {
		/* In order to modify anything, you gotta be super-user! */
		if (txc->modes && !capable(CAP_SYS_TIME))
			return -EPERM;
		/*
		 * if the quartz is off by more than 10% then
		 * something is VERY wrong!
		 */
		if (txc->modes & ADJ_TICK &&
		    (txc->tick <  900000/USER_HZ ||
		     txc->tick > 1100000/USER_HZ))
			return -EINVAL;
	}

	if (txc->modes & ADJ_SETOFFSET) {
		/* In order to inject time, you gotta be super-user! */
		if (!capable(CAP_SYS_TIME))
			return -EPERM;

		/*
		 * Validate if a timespec/timeval used to inject a time
		 * offset is valid.  Offsets can be positive or negative, so
		 * we don't check tv_sec. The value of the timeval/timespec
		 * is the sum of its fields,but *NOTE*:
		 * The field tv_usec/tv_nsec must always be non-negative and
		 * we can't have more nanoseconds/microseconds than a second.
		 */
		if (txc->time.tv_usec < 0)
			return -EINVAL;

		if (txc->modes & ADJ_NANO) {
			if (txc->time.tv_usec >= NSEC_PER_SEC)
				return -EINVAL;
		} else {
			if (txc->time.tv_usec >= USEC_PER_SEC)
				return -EINVAL;
		}
	}

	/*
	 * Check for potential multiplication overflows that can
	 * only happen on 64-bit systems:
	 */
	if ((txc->modes & ADJ_FREQUENCY) && (BITS_PER_LONG == 64)) {
		if (LLONG_MIN / PPM_SCALE > txc->freq)
			return -EINVAL;
		if (LLONG_MAX / PPM_SCALE < txc->freq)
			return -EINVAL;
	}

	return 0;
}

/**
 * random_get_entropy_fallback - Returns the raw clock source value,
 * used by random.c for platforms with no valid random_get_entropy().
 */
unsigned long random_get_entropy_fallback(void)
{
	struct tk_read_base *tkr = &tk_core.timekeeper.tkr_mono;
	struct clocksource *clock = READ_ONCE(tkr->clock);

	if (unlikely(timekeeping_suspended || !clock))
		return 0;
	return clock->read(clock);
}
EXPORT_SYMBOL_GPL(random_get_entropy_fallback);

/**
 * do_adjtimex() - Accessor function to NTP __do_adjtimex function
 */
int do_adjtimex(struct __kernel_timex *txc)
{
	struct timekeeper *tk = &tk_core.timekeeper;
	struct audit_ntp_data ad;
	bool clock_set = false;
	struct timespec64 ts;
	unsigned long flags;
	s32 orig_tai, tai;
	int ret;

	/* Validate the data before disabling interrupts */
	ret = timekeeping_validate_timex(txc);
	if (ret)
		return ret;
	add_device_randomness(txc, sizeof(*txc));

	if (txc->modes & ADJ_SETOFFSET) {
		struct timespec64 delta;
		delta.tv_sec  = txc->time.tv_sec;
		delta.tv_nsec = txc->time.tv_usec;
		if (!(txc->modes & ADJ_NANO))
			delta.tv_nsec *= 1000;
		ret = timekeeping_inject_offset(&delta);
		if (ret)
			return ret;

		audit_tk_injoffset(delta);
	}

	audit_ntp_init(&ad);

	ktime_get_real_ts64(&ts);
	add_device_randomness(&ts, sizeof(ts));

	raw_spin_lock_irqsave(&timekeeper_lock, flags);
	write_seqcount_begin(&tk_core.seq);

	orig_tai = tai = tk->tai_offset;
	ret = __do_adjtimex(txc, &ts, &tai, &ad);

	if (tai != orig_tai) {
		__timekeeping_set_tai_offset(tk, tai);
		timekeeping_update(tk, TK_MIRROR | TK_CLOCK_WAS_SET);
		clock_set = true;
	}
	tk_update_leap_state(tk);

	write_seqcount_end(&tk_core.seq);
	raw_spin_unlock_irqrestore(&timekeeper_lock, flags);

	audit_ntp_log(&ad);

	/* Update the multiplier immediately if frequency was set directly */
	if (txc->modes & (ADJ_FREQUENCY | ADJ_TICK))
		clock_set |= timekeeping_advance(TK_ADV_FREQ);

	if (clock_set)
		clock_was_set(CLOCK_REALTIME);

	ntp_notify_cmos_timer();

	return ret;
}

#ifdef CONFIG_NTP_PPS
/**
 * hardpps() - Accessor function to NTP __hardpps function
 */
void hardpps(const struct timespec64 *phase_ts, const struct timespec64 *raw_ts)
{
	unsigned long flags;

	raw_spin_lock_irqsave(&timekeeper_lock, flags);
	write_seqcount_begin(&tk_core.seq);

	__hardpps(phase_ts, raw_ts);

	write_seqcount_end(&tk_core.seq);
	raw_spin_unlock_irqrestore(&timekeeper_lock, flags);
}
EXPORT_SYMBOL(hardpps);
#endif /* CONFIG_NTP_PPS */
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
