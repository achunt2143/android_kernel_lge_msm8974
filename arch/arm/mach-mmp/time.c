<<<<<<< HEAD
=======
// SPDX-License-Identifier: GPL-2.0-only
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
/*
 * linux/arch/arm/mach-mmp/time.c
 *
 *   Support for clocksource and clockevents
 *
 * Copyright (C) 2008 Marvell International Ltd.
 * All rights reserved.
 *
 *   2008-04-11: Jason Chagas <Jason.chagas@marvell.com>
 *   2008-10-08: Bin Yang <bin.yang@marvell.com>
 *
 * The timers module actually includes three timers, each timer with up to
 * three match comparators. Timer #0 is used here in free-running mode as
 * the clock source, and match comparator #1 used as clock event device.
<<<<<<< HEAD
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 */

#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/interrupt.h>
#include <linux/clockchips.h>
<<<<<<< HEAD

#include <linux/io.h>
#include <linux/irq.h>
#include <linux/sched_clock.h>

#include <mach/addr-map.h>
#include <mach/regs-timers.h>
#include <mach/regs-apbc.h>
#include <mach/irqs.h>
#include <mach/cputype.h>
#include <asm/mach/time.h>

#include "clock.h"

#define TIMERS_VIRT_BASE	TIMERS1_VIRT_BASE
=======
#include <linux/clk.h>

#include <linux/io.h>
#include <linux/irq.h>
#include <linux/of.h>
#include <linux/of_address.h>
#include <linux/of_irq.h>
#include <linux/sched_clock.h>
#include <asm/mach/time.h>

#include "regs-timers.h"
#include <linux/soc/mmp/cputype.h>
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

#define MAX_DELTA		(0xfffffffe)
#define MIN_DELTA		(16)

<<<<<<< HEAD
/*
 * FIXME: the timer needs some delay to stablize the counter capture
 */
static inline uint32_t timer_read(void)
{
	int delay = 100;

	__raw_writel(1, TIMERS_VIRT_BASE + TMR_CVWR(1));

	while (delay--)
		cpu_relax();

	return __raw_readl(TIMERS_VIRT_BASE + TMR_CVWR(1));
}

static u32 notrace mmp_read_sched_clock(void)
=======
static void __iomem *mmp_timer_base;

/*
 * Read the timer through the CVWR register. Delay is required after requesting
 * a read. The CR register cannot be directly read due to metastability issues
 * documented in the PXA168 software manual.
 */
static inline uint32_t timer_read(void)
{
	uint32_t val;
	int delay = 3;

	__raw_writel(1, mmp_timer_base + TMR_CVWR(1));

	while (delay--)
		val = __raw_readl(mmp_timer_base + TMR_CVWR(1));

	return val;
}

static u64 notrace mmp_read_sched_clock(void)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
{
	return timer_read();
}

static irqreturn_t timer_interrupt(int irq, void *dev_id)
{
	struct clock_event_device *c = dev_id;

	/*
	 * Clear pending interrupt status.
	 */
<<<<<<< HEAD
	__raw_writel(0x01, TIMERS_VIRT_BASE + TMR_ICR(0));
=======
	__raw_writel(0x01, mmp_timer_base + TMR_ICR(0));
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	/*
	 * Disable timer 0.
	 */
<<<<<<< HEAD
	__raw_writel(0x02, TIMERS_VIRT_BASE + TMR_CER);
=======
	__raw_writel(0x02, mmp_timer_base + TMR_CER);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	c->event_handler(c);

	return IRQ_HANDLED;
}

static int timer_set_next_event(unsigned long delta,
				struct clock_event_device *dev)
{
	unsigned long flags;

	local_irq_save(flags);

	/*
	 * Disable timer 0.
	 */
<<<<<<< HEAD
	__raw_writel(0x02, TIMERS_VIRT_BASE + TMR_CER);
=======
	__raw_writel(0x02, mmp_timer_base + TMR_CER);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	/*
	 * Clear and enable timer match 0 interrupt.
	 */
<<<<<<< HEAD
	__raw_writel(0x01, TIMERS_VIRT_BASE + TMR_ICR(0));
	__raw_writel(0x01, TIMERS_VIRT_BASE + TMR_IER(0));
=======
	__raw_writel(0x01, mmp_timer_base + TMR_ICR(0));
	__raw_writel(0x01, mmp_timer_base + TMR_IER(0));
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	/*
	 * Setup new clockevent timer value.
	 */
<<<<<<< HEAD
	__raw_writel(delta - 1, TIMERS_VIRT_BASE + TMR_TN_MM(0, 0));
=======
	__raw_writel(delta - 1, mmp_timer_base + TMR_TN_MM(0, 0));
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	/*
	 * Enable timer 0.
	 */
<<<<<<< HEAD
	__raw_writel(0x03, TIMERS_VIRT_BASE + TMR_CER);
=======
	__raw_writel(0x03, mmp_timer_base + TMR_CER);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	local_irq_restore(flags);

	return 0;
}

<<<<<<< HEAD
static void timer_set_mode(enum clock_event_mode mode,
			   struct clock_event_device *dev)
=======
static int timer_set_shutdown(struct clock_event_device *evt)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
{
	unsigned long flags;

	local_irq_save(flags);
<<<<<<< HEAD
	switch (mode) {
	case CLOCK_EVT_MODE_ONESHOT:
	case CLOCK_EVT_MODE_UNUSED:
	case CLOCK_EVT_MODE_SHUTDOWN:
		/* disable the matching interrupt */
		__raw_writel(0x00, TIMERS_VIRT_BASE + TMR_IER(0));
		break;
	case CLOCK_EVT_MODE_RESUME:
	case CLOCK_EVT_MODE_PERIODIC:
		break;
	}
	local_irq_restore(flags);
}

static struct clock_event_device ckevt = {
	.name		= "clockevent",
	.features	= CLOCK_EVT_FEAT_ONESHOT,
	.shift		= 32,
	.rating		= 200,
	.set_next_event	= timer_set_next_event,
	.set_mode	= timer_set_mode,
};

static cycle_t clksrc_read(struct clocksource *cs)
=======
	/* disable the matching interrupt */
	__raw_writel(0x00, mmp_timer_base + TMR_IER(0));
	local_irq_restore(flags);

	return 0;
}

static struct clock_event_device ckevt = {
	.name			= "clockevent",
	.features		= CLOCK_EVT_FEAT_ONESHOT,
	.rating			= 200,
	.set_next_event		= timer_set_next_event,
	.set_state_shutdown	= timer_set_shutdown,
	.set_state_oneshot	= timer_set_shutdown,
};

static u64 clksrc_read(struct clocksource *cs)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
{
	return timer_read();
}

static struct clocksource cksrc = {
	.name		= "clocksource",
	.rating		= 200,
	.read		= clksrc_read,
	.mask		= CLOCKSOURCE_MASK(32),
	.flags		= CLOCK_SOURCE_IS_CONTINUOUS,
};

static void __init timer_config(void)
{
<<<<<<< HEAD
	uint32_t ccr = __raw_readl(TIMERS_VIRT_BASE + TMR_CCR);

	__raw_writel(0x0, TIMERS_VIRT_BASE + TMR_CER); /* disable */

	ccr &= (cpu_is_mmp2()) ? (TMR_CCR_CS_0(0) | TMR_CCR_CS_1(0)) :
		(TMR_CCR_CS_0(3) | TMR_CCR_CS_1(3));
	__raw_writel(ccr, TIMERS_VIRT_BASE + TMR_CCR);

	/* set timer 0 to periodic mode, and timer 1 to free-running mode */
	__raw_writel(0x2, TIMERS_VIRT_BASE + TMR_CMR);

	__raw_writel(0x1, TIMERS_VIRT_BASE + TMR_PLCR(0)); /* periodic */
	__raw_writel(0x7, TIMERS_VIRT_BASE + TMR_ICR(0));  /* clear status */
	__raw_writel(0x0, TIMERS_VIRT_BASE + TMR_IER(0));

	__raw_writel(0x0, TIMERS_VIRT_BASE + TMR_PLCR(1)); /* free-running */
	__raw_writel(0x7, TIMERS_VIRT_BASE + TMR_ICR(1));  /* clear status */
	__raw_writel(0x0, TIMERS_VIRT_BASE + TMR_IER(1));

	/* enable timer 1 counter */
	__raw_writel(0x2, TIMERS_VIRT_BASE + TMR_CER);
}

static struct irqaction timer_irq = {
	.name		= "timer",
	.flags		= IRQF_DISABLED | IRQF_TIMER | IRQF_IRQPOLL,
	.handler	= timer_interrupt,
	.dev_id		= &ckevt,
};

void __init timer_init(int irq)
{
	timer_config();

	setup_sched_clock(mmp_read_sched_clock, 32, CLOCK_TICK_RATE);

	ckevt.mult = div_sc(CLOCK_TICK_RATE, NSEC_PER_SEC, ckevt.shift);
	ckevt.max_delta_ns = clockevent_delta2ns(MAX_DELTA, &ckevt);
	ckevt.min_delta_ns = clockevent_delta2ns(MIN_DELTA, &ckevt);
	ckevt.cpumask = cpumask_of(0);

	setup_irq(irq, &timer_irq);

	clocksource_register_hz(&cksrc, CLOCK_TICK_RATE);
	clockevents_register_device(&ckevt);
}
=======
	uint32_t ccr = __raw_readl(mmp_timer_base + TMR_CCR);

	__raw_writel(0x0, mmp_timer_base + TMR_CER); /* disable */

	ccr &= (cpu_is_mmp2() || cpu_is_mmp3()) ?
		(TMR_CCR_CS_0(0) | TMR_CCR_CS_1(0)) :
		(TMR_CCR_CS_0(3) | TMR_CCR_CS_1(3));
	__raw_writel(ccr, mmp_timer_base + TMR_CCR);

	/* set timer 0 to periodic mode, and timer 1 to free-running mode */
	__raw_writel(0x2, mmp_timer_base + TMR_CMR);

	__raw_writel(0x1, mmp_timer_base + TMR_PLCR(0)); /* periodic */
	__raw_writel(0x7, mmp_timer_base + TMR_ICR(0));  /* clear status */
	__raw_writel(0x0, mmp_timer_base + TMR_IER(0));

	__raw_writel(0x0, mmp_timer_base + TMR_PLCR(1)); /* free-running */
	__raw_writel(0x7, mmp_timer_base + TMR_ICR(1));  /* clear status */
	__raw_writel(0x0, mmp_timer_base + TMR_IER(1));

	/* enable timer 1 counter */
	__raw_writel(0x2, mmp_timer_base + TMR_CER);
}

static void __init mmp_timer_init(int irq, unsigned long rate)
{
	timer_config();

	sched_clock_register(mmp_read_sched_clock, 32, rate);

	ckevt.cpumask = cpumask_of(0);

	if (request_irq(irq, timer_interrupt, IRQF_TIMER | IRQF_IRQPOLL,
			"timer", &ckevt))
		pr_err("Failed to request irq %d (timer)\n", irq);

	clocksource_register_hz(&cksrc, rate);
	clockevents_config_and_register(&ckevt, rate, MIN_DELTA, MAX_DELTA);
}

static int __init mmp_dt_init_timer(struct device_node *np)
{
	struct clk *clk;
	int irq, ret;
	unsigned long rate;

	clk = of_clk_get(np, 0);
	if (!IS_ERR(clk)) {
		ret = clk_prepare_enable(clk);
		if (ret)
			return ret;
		rate = clk_get_rate(clk);
	} else if (cpu_is_pj4()) {
		rate = 6500000;
	} else {
		rate = 3250000;
	}

	irq = irq_of_parse_and_map(np, 0);
	if (!irq)
		return -EINVAL;

	mmp_timer_base = of_iomap(np, 0);
	if (!mmp_timer_base)
		return -ENOMEM;

	mmp_timer_init(irq, rate);
	return 0;
}

TIMER_OF_DECLARE(mmp_timer, "mrvl,mmp-timer", mmp_dt_init_timer);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
