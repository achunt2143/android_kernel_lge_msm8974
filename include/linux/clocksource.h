<<<<<<< HEAD
=======
/* SPDX-License-Identifier: GPL-2.0 */
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
/*  linux/include/linux/clocksource.h
 *
 *  This file contains the structure definitions for clocksources.
 *
 *  If you are not a clocksource, or timekeeping code, you should
 *  not be including this file!
 */
#ifndef _LINUX_CLOCKSOURCE_H
#define _LINUX_CLOCKSOURCE_H

#include <linux/types.h>
#include <linux/timex.h>
#include <linux/time.h>
#include <linux/list.h>
#include <linux/cache.h>
#include <linux/timer.h>
#include <linux/init.h>
<<<<<<< HEAD
#include <asm/div64.h>
#include <asm/io.h>

/* clocksource cycle base type */
typedef u64 cycle_t;
struct clocksource;

#ifdef CONFIG_ARCH_CLOCKSOURCE_DATA
#include <asm/clocksource.h>
#endif

/**
 * struct cyclecounter - hardware abstraction for a free running counter
 *	Provides completely state-free accessors to the underlying hardware.
 *	Depending on which hardware it reads, the cycle counter may wrap
 *	around quickly. Locking rules (if necessary) have to be defined
 *	by the implementor and user of specific instances of this API.
 *
 * @read:		returns the current cycle value
 * @mask:		bitmask for two's complement
 *			subtraction of non 64 bit counters,
 *			see CLOCKSOURCE_MASK() helper macro
 * @mult:		cycle to nanosecond multiplier
 * @shift:		cycle to nanosecond divisor (power of two)
 */
struct cyclecounter {
	cycle_t (*read)(const struct cyclecounter *cc);
	cycle_t mask;
	u32 mult;
	u32 shift;
};

/**
 * struct timecounter - layer above a %struct cyclecounter which counts nanoseconds
 *	Contains the state needed by timecounter_read() to detect
 *	cycle counter wrap around. Initialize with
 *	timecounter_init(). Also used to convert cycle counts into the
 *	corresponding nanosecond counts with timecounter_cyc2time(). Users
 *	of this code are responsible for initializing the underlying
 *	cycle counter hardware, locking issues and reading the time
 *	more often than the cycle counter wraps around. The nanosecond
 *	counter will only wrap around after ~585 years.
 *
 * @cc:			the cycle counter used by this instance
 * @cycle_last:		most recent cycle counter value seen by
 *			timecounter_read()
 * @nsec:		continuously increasing count
 */
struct timecounter {
	const struct cyclecounter *cc;
	cycle_t cycle_last;
	u64 nsec;
};

/**
 * cyclecounter_cyc2ns - converts cycle counter cycles to nanoseconds
 * @cc:		Pointer to cycle counter.
 * @cycles:	Cycles
 *
 * XXX - This could use some mult_lxl_ll() asm optimization. Same code
 * as in cyc2ns, but with unsigned result.
 */
static inline u64 cyclecounter_cyc2ns(const struct cyclecounter *cc,
				      cycle_t cycles)
{
	u64 ret = (u64)cycles;
	ret = (ret * cc->mult) >> cc->shift;
	return ret;
}

/**
 * timecounter_init - initialize a time counter
 * @tc:			Pointer to time counter which is to be initialized/reset
 * @cc:			A cycle counter, ready to be used.
 * @start_tstamp:	Arbitrary initial time stamp.
 *
 * After this call the current cycle register (roughly) corresponds to
 * the initial time stamp. Every call to timecounter_read() increments
 * the time stamp counter by the number of elapsed nanoseconds.
 */
extern void timecounter_init(struct timecounter *tc,
			     const struct cyclecounter *cc,
			     u64 start_tstamp);

/**
 * timecounter_read - return nanoseconds elapsed since timecounter_init()
 *                    plus the initial time stamp
 * @tc:          Pointer to time counter.
 *
 * In other words, keeps track of time since the same epoch as
 * the function which generated the initial time stamp.
 */
extern u64 timecounter_read(struct timecounter *tc);

/**
 * timecounter_cyc2time - convert a cycle counter to same
 *                        time base as values returned by
 *                        timecounter_read()
 * @tc:		Pointer to time counter.
 * @cycle_tstamp:	a value returned by tc->cc->read()
 *
 * Cycle counts that are converted correctly as long as they
 * fall into the interval [-1/2 max cycle count, +1/2 max cycle count],
 * with "max cycle count" == cs->mask+1.
 *
 * This allows conversion of cycle counter values which were generated
 * in the past.
 */
extern u64 timecounter_cyc2time(struct timecounter *tc,
				cycle_t cycle_tstamp);
=======
#include <linux/of.h>
#include <linux/clocksource_ids.h>
#include <asm/div64.h>
#include <asm/io.h>

struct clocksource;
struct module;

#if defined(CONFIG_ARCH_CLOCKSOURCE_DATA) || \
    defined(CONFIG_GENERIC_GETTIMEOFDAY)
#include <asm/clocksource.h>
#endif

#include <vdso/clocksource.h>
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

/**
 * struct clocksource - hardware abstraction for a free running counter
 *	Provides mostly state-free accessors to the underlying hardware.
 *	This is the structure used for system time.
 *
<<<<<<< HEAD
 * @name:		ptr to clocksource name
 * @list:		list head for registration
 * @rating:		rating value for selection (higher is better)
=======
 * @read:		Returns a cycle value, passes clocksource as argument
 * @mask:		Bitmask for two's complement
 *			subtraction of non 64 bit counters
 * @mult:		Cycle to nanosecond multiplier
 * @shift:		Cycle to nanosecond divisor (power of two)
 * @max_idle_ns:	Maximum idle time permitted by the clocksource (nsecs)
 * @maxadj:		Maximum adjustment value to mult (~11%)
 * @uncertainty_margin:	Maximum uncertainty in nanoseconds per half second.
 *			Zero says to use default WATCHDOG_THRESHOLD.
 * @archdata:		Optional arch-specific data
 * @max_cycles:		Maximum safe cycle value which won't overflow on
 *			multiplication
 * @name:		Pointer to clocksource name
 * @list:		List head for registration (internal)
 * @rating:		Rating value for selection (higher is better)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 *			To avoid rating inflation the following
 *			list should give you a guide as to how
 *			to assign your clocksource a rating
 *			1-99: Unfit for real use
 *				Only available for bootup and testing purposes.
 *			100-199: Base level usability.
 *				Functional for real use, but not desired.
 *			200-299: Good.
 *				A correct and usable clocksource.
 *			300-399: Desired.
 *				A reasonably fast and accurate clocksource.
 *			400-499: Perfect
 *				The ideal clocksource. A must-use where
 *				available.
<<<<<<< HEAD
 * @read:		returns a cycle value, passes clocksource as argument
 * @enable:		optional function to enable the clocksource
 * @disable:		optional function to disable the clocksource
 * @mask:		bitmask for two's complement
 *			subtraction of non 64 bit counters
 * @mult:		cycle to nanosecond multiplier
 * @shift:		cycle to nanosecond divisor (power of two)
 * @max_idle_ns:	max idle time permitted by the clocksource (nsecs)
 * @maxadj:		maximum adjustment value to mult (~11%)
 * @flags:		flags describing special properties
 * @archdata:		arch-specific data
 * @suspend:		suspend function for the clocksource, if necessary
 * @resume:		resume function for the clocksource, if necessary
 * @cycle_last:		most recent cycle counter value seen by ::read()
 */
struct clocksource {
	/*
	 * Hotpath data, fits in a single cache line when the
	 * clocksource itself is cacheline aligned.
	 */
	cycle_t (*read)(struct clocksource *cs);
	cycle_t cycle_last;
	cycle_t mask;
	u32 mult;
	u32 shift;
	u64 max_idle_ns;
	u32 maxadj;
#ifdef CONFIG_ARCH_CLOCKSOURCE_DATA
	struct arch_clocksource_data archdata;
#endif

	const char *name;
	struct list_head list;
	int rating;
	int (*enable)(struct clocksource *cs);
	void (*disable)(struct clocksource *cs);
	unsigned long flags;
	void (*suspend)(struct clocksource *cs);
	void (*resume)(struct clocksource *cs);
=======
 * @id:			Defaults to CSID_GENERIC. The id value is captured
 *			in certain snapshot functions to allow callers to
 *			validate the clocksource from which the snapshot was
 *			taken.
 * @flags:		Flags describing special properties
 * @enable:		Optional function to enable the clocksource
 * @disable:		Optional function to disable the clocksource
 * @suspend:		Optional suspend function for the clocksource
 * @resume:		Optional resume function for the clocksource
 * @mark_unstable:	Optional function to inform the clocksource driver that
 *			the watchdog marked the clocksource unstable
 * @tick_stable:        Optional function called periodically from the watchdog
 *			code to provide stable synchronization points
 * @wd_list:		List head to enqueue into the watchdog list (internal)
 * @cs_last:		Last clocksource value for clocksource watchdog
 * @wd_last:		Last watchdog value corresponding to @cs_last
 * @owner:		Module reference, must be set by clocksource in modules
 *
 * Note: This struct is not used in hotpathes of the timekeeping code
 * because the timekeeper caches the hot path fields in its own data
 * structure, so no cache line alignment is required,
 *
 * The pointer to the clocksource itself is handed to the read
 * callback. If you need extra information there you can wrap struct
 * clocksource into your own struct. Depending on the amount of
 * information you need you should consider to cache line align that
 * structure.
 */
struct clocksource {
	u64			(*read)(struct clocksource *cs);
	u64			mask;
	u32			mult;
	u32			shift;
	u64			max_idle_ns;
	u32			maxadj;
	u32			uncertainty_margin;
#ifdef CONFIG_ARCH_CLOCKSOURCE_DATA
	struct arch_clocksource_data archdata;
#endif
	u64			max_cycles;
	const char		*name;
	struct list_head	list;
	int			rating;
	enum clocksource_ids	id;
	enum vdso_clock_mode	vdso_clock_mode;
	unsigned long		flags;

	int			(*enable)(struct clocksource *cs);
	void			(*disable)(struct clocksource *cs);
	void			(*suspend)(struct clocksource *cs);
	void			(*resume)(struct clocksource *cs);
	void			(*mark_unstable)(struct clocksource *cs);
	void			(*tick_stable)(struct clocksource *cs);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	/* private: */
#ifdef CONFIG_CLOCKSOURCE_WATCHDOG
	/* Watchdog related data, used by the framework */
<<<<<<< HEAD
	struct list_head wd_list;
	cycle_t cs_last;
	cycle_t wd_last;
#endif
} ____cacheline_aligned;
=======
	struct list_head	wd_list;
	u64			cs_last;
	u64			wd_last;
#endif
	struct module		*owner;
};
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

/*
 * Clock source flags bits::
 */
#define CLOCK_SOURCE_IS_CONTINUOUS		0x01
#define CLOCK_SOURCE_MUST_VERIFY		0x02

#define CLOCK_SOURCE_WATCHDOG			0x10
#define CLOCK_SOURCE_VALID_FOR_HRES		0x20
#define CLOCK_SOURCE_UNSTABLE			0x40
#define CLOCK_SOURCE_SUSPEND_NONSTOP		0x80
<<<<<<< HEAD

/* simplify initialization of mask field */
#define CLOCKSOURCE_MASK(bits) (cycle_t)((bits) < 64 ? ((1ULL<<(bits))-1) : -1)
=======
#define CLOCK_SOURCE_RESELECT			0x100
#define CLOCK_SOURCE_VERIFY_PERCPU		0x200
/* simplify initialization of mask field */
#define CLOCKSOURCE_MASK(bits) GENMASK_ULL((bits) - 1, 0)

static inline u32 clocksource_freq2mult(u32 freq, u32 shift_constant, u64 from)
{
	/*  freq = cyc/from
	 *  mult/2^shift  = ns/cyc
	 *  mult = ns/cyc * 2^shift
	 *  mult = from/freq * 2^shift
	 *  mult = from * 2^shift / freq
	 *  mult = (from<<shift) / freq
	 */
	u64 tmp = ((u64)from) << shift_constant;

	tmp += freq/2; /* round for do_div */
	do_div(tmp, freq);

	return (u32)tmp;
}
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

/**
 * clocksource_khz2mult - calculates mult from khz and shift
 * @khz:		Clocksource frequency in KHz
 * @shift_constant:	Clocksource shift factor
 *
 * Helper functions that converts a khz counter frequency to a timsource
 * multiplier, given the clocksource shift value
 */
static inline u32 clocksource_khz2mult(u32 khz, u32 shift_constant)
{
<<<<<<< HEAD
	/*  khz = cyc/(Million ns)
	 *  mult/2^shift  = ns/cyc
	 *  mult = ns/cyc * 2^shift
	 *  mult = 1Million/khz * 2^shift
	 *  mult = 1000000 * 2^shift / khz
	 *  mult = (1000000<<shift) / khz
	 */
	u64 tmp = ((u64)1000000) << shift_constant;

	tmp += khz/2; /* round for do_div */
	do_div(tmp, khz);

	return (u32)tmp;
=======
	return clocksource_freq2mult(khz, shift_constant, NSEC_PER_MSEC);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

/**
 * clocksource_hz2mult - calculates mult from hz and shift
 * @hz:			Clocksource frequency in Hz
 * @shift_constant:	Clocksource shift factor
 *
 * Helper functions that converts a hz counter
 * frequency to a timsource multiplier, given the
 * clocksource shift value
 */
static inline u32 clocksource_hz2mult(u32 hz, u32 shift_constant)
{
<<<<<<< HEAD
	/*  hz = cyc/(Billion ns)
	 *  mult/2^shift  = ns/cyc
	 *  mult = ns/cyc * 2^shift
	 *  mult = 1Billion/hz * 2^shift
	 *  mult = 1000000000 * 2^shift / hz
	 *  mult = (1000000000<<shift) / hz
	 */
	u64 tmp = ((u64)1000000000) << shift_constant;

	tmp += hz/2; /* round for do_div */
	do_div(tmp, hz);

	return (u32)tmp;
=======
	return clocksource_freq2mult(hz, shift_constant, NSEC_PER_SEC);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

/**
 * clocksource_cyc2ns - converts clocksource cycles to nanoseconds
 * @cycles:	cycles
 * @mult:	cycle to nanosecond multiplier
 * @shift:	cycle to nanosecond divisor (power of two)
 *
<<<<<<< HEAD
 * Converts cycles to nanoseconds, using the given mult and shift.
 *
 * XXX - This could use some mult_lxl_ll() asm optimization
 */
static inline s64 clocksource_cyc2ns(cycle_t cycles, u32 mult, u32 shift)
=======
 * Converts clocksource cycles to nanoseconds, using the given @mult and @shift.
 * The code is optimized for performance and is not intended to work
 * with absolute clocksource cycles (as those will easily overflow),
 * but is only intended to be used with relative (delta) clocksource cycles.
 *
 * XXX - This could use some mult_lxl_ll() asm optimization
 */
static inline s64 clocksource_cyc2ns(u64 cycles, u32 mult, u32 shift)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
{
	return ((u64) cycles * mult) >> shift;
}


<<<<<<< HEAD
extern int clocksource_register(struct clocksource*);
extern void clocksource_unregister(struct clocksource*);
extern void clocksource_touch_watchdog(void);
extern struct clocksource* clocksource_get_next(void);
extern void clocksource_change_rating(struct clocksource *cs, int rating);
extern void clocksource_suspend(void);
extern void clocksource_resume(void);
extern struct clocksource * __init __weak clocksource_default_clock(void);
extern void clocksource_mark_unstable(struct clocksource *cs);

extern u64
clocks_calc_max_nsecs(u32 mult, u32 shift, u32 maxadj, u64 mask);
=======
extern int clocksource_unregister(struct clocksource*);
extern void clocksource_touch_watchdog(void);
extern void clocksource_change_rating(struct clocksource *cs, int rating);
extern void clocksource_suspend(void);
extern void clocksource_resume(void);
extern struct clocksource * __init clocksource_default_clock(void);
extern void clocksource_mark_unstable(struct clocksource *cs);
extern void
clocksource_start_suspend_timing(struct clocksource *cs, u64 start_cycles);
extern u64 clocksource_stop_suspend_timing(struct clocksource *cs, u64 now);

extern u64
clocks_calc_max_nsecs(u32 mult, u32 shift, u32 maxadj, u64 mask, u64 *max_cycles);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
extern void
clocks_calc_mult_shift(u32 *mult, u32 *shift, u32 from, u32 to, u32 minsec);

/*
 * Don't call __clocksource_register_scale directly, use
 * clocksource_register_hz/khz
 */
extern int
__clocksource_register_scale(struct clocksource *cs, u32 scale, u32 freq);
extern void
<<<<<<< HEAD
__clocksource_updatefreq_scale(struct clocksource *cs, u32 scale, u32 freq);
=======
__clocksource_update_freq_scale(struct clocksource *cs, u32 scale, u32 freq);

/*
 * Don't call this unless you are a default clocksource
 * (AKA: jiffies) and absolutely have to.
 */
static inline int __clocksource_register(struct clocksource *cs)
{
	return __clocksource_register_scale(cs, 1, 0);
}
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

static inline int clocksource_register_hz(struct clocksource *cs, u32 hz)
{
	return __clocksource_register_scale(cs, 1, hz);
}

static inline int clocksource_register_khz(struct clocksource *cs, u32 khz)
{
	return __clocksource_register_scale(cs, 1000, khz);
}

<<<<<<< HEAD
static inline void __clocksource_updatefreq_hz(struct clocksource *cs, u32 hz)
{
	__clocksource_updatefreq_scale(cs, 1, hz);
}

static inline void __clocksource_updatefreq_khz(struct clocksource *cs, u32 khz)
{
	__clocksource_updatefreq_scale(cs, 1000, khz);
}

#ifdef CONFIG_GENERIC_TIME_VSYSCALL
extern void
update_vsyscall(struct timespec *ts, struct timespec *wtm,
			struct clocksource *c, u32 mult);
extern void update_vsyscall_tz(void);
#else
static inline void
update_vsyscall(struct timespec *ts, struct timespec *wtm,
			struct clocksource *c, u32 mult)
{
}

static inline void update_vsyscall_tz(void)
{
}
#endif

extern void timekeeping_notify(struct clocksource *clock);

extern cycle_t clocksource_mmio_readl_up(struct clocksource *);
extern cycle_t clocksource_mmio_readl_down(struct clocksource *);
extern cycle_t clocksource_mmio_readw_up(struct clocksource *);
extern cycle_t clocksource_mmio_readw_down(struct clocksource *);

extern int clocksource_mmio_init(void __iomem *, const char *,
	unsigned long, int, unsigned, cycle_t (*)(struct clocksource *));

extern int clocksource_i8253_init(void);

=======
static inline void __clocksource_update_freq_hz(struct clocksource *cs, u32 hz)
{
	__clocksource_update_freq_scale(cs, 1, hz);
}

static inline void __clocksource_update_freq_khz(struct clocksource *cs, u32 khz)
{
	__clocksource_update_freq_scale(cs, 1000, khz);
}

#ifdef CONFIG_ARCH_CLOCKSOURCE_INIT
extern void clocksource_arch_init(struct clocksource *cs);
#else
static inline void clocksource_arch_init(struct clocksource *cs) { }
#endif

extern int timekeeping_notify(struct clocksource *clock);

extern u64 clocksource_mmio_readl_up(struct clocksource *);
extern u64 clocksource_mmio_readl_down(struct clocksource *);
extern u64 clocksource_mmio_readw_up(struct clocksource *);
extern u64 clocksource_mmio_readw_down(struct clocksource *);

extern int clocksource_mmio_init(void __iomem *, const char *,
	unsigned long, int, unsigned, u64 (*)(struct clocksource *));

extern int clocksource_i8253_init(void);

#define TIMER_OF_DECLARE(name, compat, fn) \
	OF_DECLARE_1_RET(timer, name, compat, fn)

#ifdef CONFIG_TIMER_PROBE
extern void timer_probe(void);
#else
static inline void timer_probe(void) {}
#endif

#define TIMER_ACPI_DECLARE(name, table_id, fn)		\
	ACPI_DECLARE_PROBE_ENTRY(timer, name, table_id, 0, NULL, 0, fn)

static inline unsigned int clocksource_get_max_watchdog_retry(void)
{
	/*
	 * When system is in the boot phase or under heavy workload, there
	 * can be random big latencies during the clocksource/watchdog
	 * read, so allow retries to filter the noise latency. As the
	 * latency's frequency and maximum value goes up with the number of
	 * CPUs, scale the number of retries with the number of online
	 * CPUs.
	 */
	return (ilog2(num_online_cpus()) / 2) + 1;
}

void clocksource_verify_percpu(struct clocksource *cs);

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#endif /* _LINUX_CLOCKSOURCE_H */
