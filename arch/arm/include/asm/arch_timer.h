<<<<<<< HEAD
#ifndef __ASMARM_ARCH_TIMER_H
#define __ASMARM_ARCH_TIMER_H

#include <linux/ioport.h>
#include <linux/clocksource.h>
#include <asm/errno.h>

#define ARCH_TIMER_USR_PCT_ACCESS_EN	(1 << 0) /* physical counter */
#define ARCH_TIMER_USR_VCT_ACCESS_EN	(1 << 1) /* virtual counter */
#define ARCH_TIMER_VIRT_EVT_EN		(1 << 2)
#define ARCH_TIMER_EVT_TRIGGER_SHIFT	(4)
#define ARCH_TIMER_EVT_TRIGGER_MASK	(0xF << ARCH_TIMER_EVT_TRIGGER_SHIFT)
#define ARCH_TIMER_USR_VT_ACCESS_EN	(1 << 8) /* virtual timer registers */
#define ARCH_TIMER_USR_PT_ACCESS_EN	(1 << 9) /* physical timer registers */

struct arch_timer {
	struct resource	res[3];
};

#ifdef CONFIG_ARM_ARCH_TIMER
int arch_timer_register(struct arch_timer *);
int arch_timer_of_register(void);
cycle_t arch_counter_get_cntpct(void);
=======
/* SPDX-License-Identifier: GPL-2.0 */
#ifndef __ASMARM_ARCH_TIMER_H
#define __ASMARM_ARCH_TIMER_H

#include <asm/barrier.h>
#include <asm/errno.h>
#include <asm/hwcap.h>
#include <linux/clocksource.h>
#include <linux/init.h>
#include <linux/io-64-nonatomic-lo-hi.h>
#include <linux/types.h>

#include <clocksource/arm_arch_timer.h>

#ifdef CONFIG_ARM_ARCH_TIMER
/* 32bit ARM doesn't know anything about timer errata... */
#define has_erratum_handler(h)		(false)
#define erratum_handler(h)		(arch_timer_##h)

int arch_timer_arch_init(void);

/*
 * These register accessors are marked inline so the compiler can
 * nicely work out which register we want, and chuck away the rest of
 * the code. At least it does so with a recent GCC (4.6.3).
 */
static __always_inline
void arch_timer_reg_write_cp15(int access, enum arch_timer_reg reg, u64 val)
{
	if (access == ARCH_TIMER_PHYS_ACCESS) {
		switch (reg) {
		case ARCH_TIMER_REG_CTRL:
			asm volatile("mcr p15, 0, %0, c14, c2, 1" : : "r" ((u32)val));
			isb();
			break;
		case ARCH_TIMER_REG_CVAL:
			asm volatile("mcrr p15, 2, %Q0, %R0, c14" : : "r" (val));
			break;
		default:
			BUILD_BUG();
		}
	} else if (access == ARCH_TIMER_VIRT_ACCESS) {
		switch (reg) {
		case ARCH_TIMER_REG_CTRL:
			asm volatile("mcr p15, 0, %0, c14, c3, 1" : : "r" ((u32)val));
			isb();
			break;
		case ARCH_TIMER_REG_CVAL:
			asm volatile("mcrr p15, 3, %Q0, %R0, c14" : : "r" (val));
			break;
		default:
			BUILD_BUG();
		}
	} else {
		BUILD_BUG();
	}
}

static __always_inline
u32 arch_timer_reg_read_cp15(int access, enum arch_timer_reg reg)
{
	u32 val = 0;

	if (access == ARCH_TIMER_PHYS_ACCESS) {
		switch (reg) {
		case ARCH_TIMER_REG_CTRL:
			asm volatile("mrc p15, 0, %0, c14, c2, 1" : "=r" (val));
			break;
		default:
			BUILD_BUG();
		}
	} else if (access == ARCH_TIMER_VIRT_ACCESS) {
		switch (reg) {
		case ARCH_TIMER_REG_CTRL:
			asm volatile("mrc p15, 0, %0, c14, c3, 1" : "=r" (val));
			break;
		default:
			BUILD_BUG();
		}
	} else {
		BUILD_BUG();
	}

	return val;
}

static inline u32 arch_timer_get_cntfrq(void)
{
	u32 val;
	asm volatile("mrc p15, 0, %0, c14, c0, 0" : "=r" (val));
	return val;
}

static inline u64 __arch_counter_get_cntpct(void)
{
	u64 cval;

	isb();
	asm volatile("mrrc p15, 0, %Q0, %R0, c14" : "=r" (cval));
	return cval;
}

static inline u64 __arch_counter_get_cntpct_stable(void)
{
	return __arch_counter_get_cntpct();
}

static inline u64 __arch_counter_get_cntvct(void)
{
	u64 cval;

	isb();
	asm volatile("mrrc p15, 1, %Q0, %R0, c14" : "=r" (cval));
	return cval;
}

static inline u64 __arch_counter_get_cntvct_stable(void)
{
	return __arch_counter_get_cntvct();
}
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

static inline u32 arch_timer_get_cntkctl(void)
{
	u32 cntkctl;
	asm volatile("mrc p15, 0, %0, c14, c1, 0" : "=r" (cntkctl));
	return cntkctl;
}

static inline void arch_timer_set_cntkctl(u32 cntkctl)
{
	asm volatile("mcr p15, 0, %0, c14, c1, 0" : : "r" (cntkctl));
<<<<<<< HEAD
}

static inline void arch_counter_set_user_access(void)
{
	u32 cntkctl = arch_timer_get_cntkctl();

	/* Disable user access to the timers and the physical counter */
	/* Also disable virtual event stream */
	cntkctl &= ~(ARCH_TIMER_USR_PT_ACCESS_EN
			| ARCH_TIMER_USR_VT_ACCESS_EN
			| ARCH_TIMER_VIRT_EVT_EN
			| ARCH_TIMER_USR_PCT_ACCESS_EN);

	/* Enable user access to the virtual counter */
	cntkctl |= ARCH_TIMER_USR_VCT_ACCESS_EN;

	arch_timer_set_cntkctl(cntkctl);
}

static inline void arch_timer_evtstrm_enable(int divider)
{
	u32 cntkctl = arch_timer_get_cntkctl();
	cntkctl &= ~ARCH_TIMER_EVT_TRIGGER_MASK;
	/* Set the divider and enable virtual event stream */
	cntkctl |= (divider << ARCH_TIMER_EVT_TRIGGER_SHIFT)
			| ARCH_TIMER_VIRT_EVT_EN;
	arch_timer_set_cntkctl(cntkctl);
	elf_hwcap |= HWCAP_EVTSTRM;
}
#else
static inline int arch_timer_register(struct arch_timer *at)
{
	return -ENXIO;
}

static inline int arch_timer_of_register(void)
{
	return -ENXIO;
}

static inline cycle_t arch_counter_get_cntpct(void)
{
	return 0;
=======
	isb();
}

static inline void arch_timer_set_evtstrm_feature(void)
{
	elf_hwcap |= HWCAP_EVTSTRM;
}

static inline bool arch_timer_have_evtstrm_feature(void)
{
	return elf_hwcap & HWCAP_EVTSTRM;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}
#endif

#endif
