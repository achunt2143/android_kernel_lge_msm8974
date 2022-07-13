<<<<<<< HEAD
=======
/* SPDX-License-Identifier: GPL-2.0 */
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
/*
 * x86 TSC related functions
 */
#ifndef _ASM_X86_TSC_H
#define _ASM_X86_TSC_H

<<<<<<< HEAD
#include <asm/processor.h>

#define NS_SCALE	10 /* 2^10, carefully chosen */
#define US_SCALE	32 /* 2^32, arbitralrily chosen */
=======
#include <asm/cpufeature.h>
#include <asm/processor.h>
#include <asm/msr.h>
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

/*
 * Standard way to access the cycle counter.
 */
typedef unsigned long long cycles_t;

extern unsigned int cpu_khz;
extern unsigned int tsc_khz;

extern void disable_TSC(void);

static inline cycles_t get_cycles(void)
{
<<<<<<< HEAD
	unsigned long long ret = 0;

#ifndef CONFIG_X86_TSC
	if (!cpu_has_tsc)
		return 0;
#endif
	rdtscll(ret);

	return ret;
}

static __always_inline cycles_t vget_cycles(void)
{
	/*
	 * We only do VDSOs on TSC capable CPUs, so this shouldn't
	 * access boot_cpu_data (which is not VDSO-safe):
	 */
#ifndef CONFIG_X86_TSC
	if (!cpu_has_tsc)
		return 0;
#endif
	return (cycles_t)__native_read_tsc();
}

=======
	if (!IS_ENABLED(CONFIG_X86_TSC) &&
	    !cpu_feature_enabled(X86_FEATURE_TSC))
		return 0;
	return rdtsc();
}
#define get_cycles get_cycles

extern struct system_counterval_t convert_art_to_tsc(u64 art);
extern struct system_counterval_t convert_art_ns_to_tsc(u64 art_ns);

extern void tsc_early_init(void);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
extern void tsc_init(void);
extern void mark_tsc_unstable(char *reason);
extern int unsynchronized_tsc(void);
extern int check_tsc_unstable(void);
<<<<<<< HEAD
extern unsigned long native_calibrate_tsc(void);

extern int tsc_clocksource_reliable;
=======
extern void mark_tsc_async_resets(char *reason);
extern unsigned long native_calibrate_cpu_early(void);
extern unsigned long native_calibrate_tsc(void);
extern unsigned long long native_sched_clock_from_tsc(u64 tsc);

extern int tsc_clocksource_reliable;
#ifdef CONFIG_X86_TSC
extern bool tsc_async_resets;
#else
# define tsc_async_resets	false
#endif
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

/*
 * Boot-time check whether the TSCs are synchronized across
 * all CPUs/cores:
 */
<<<<<<< HEAD
extern void check_tsc_sync_source(int cpu);
extern void check_tsc_sync_target(void);
=======
#ifdef CONFIG_X86_TSC
extern bool tsc_store_and_check_tsc_adjust(bool bootcpu);
extern void tsc_verify_tsc_adjust(bool resume);
extern void check_tsc_sync_target(void);
#else
static inline bool tsc_store_and_check_tsc_adjust(bool bootcpu) { return false; }
static inline void tsc_verify_tsc_adjust(bool resume) { }
static inline void check_tsc_sync_target(void) { }
#endif
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

extern int notsc_setup(char *);
extern void tsc_save_sched_clock_state(void);
extern void tsc_restore_sched_clock_state(void);

<<<<<<< HEAD
=======
unsigned long cpu_khz_from_msr(void);

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#endif /* _ASM_X86_TSC_H */
