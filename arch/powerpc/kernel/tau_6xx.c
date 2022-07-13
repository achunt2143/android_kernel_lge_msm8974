<<<<<<< HEAD
=======
// SPDX-License-Identifier: GPL-2.0
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
/*
 * temp.c	Thermal management for cpu's with Thermal Assist Units
 *
 * Written by Troy Benjegerdes <hozer@drgw.net>
 *
 * TODO:
 * dynamic power management to limit peak CPU temp (using ICTC)
 * calibration???
 *
 * Silly, crazy ideas: use cpu load (from scheduler) and ICTC to extend battery
 * life in portables, and add a 'performance/watt' metric somewhere in /proc
 */

#include <linux/errno.h>
<<<<<<< HEAD
#include <linux/jiffies.h>
=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#include <linux/kernel.h>
#include <linux/param.h>
#include <linux/string.h>
#include <linux/mm.h>
#include <linux/interrupt.h>
#include <linux/init.h>
<<<<<<< HEAD

=======
#include <linux/delay.h>
#include <linux/workqueue.h>

#include <asm/interrupt.h>
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#include <asm/io.h>
#include <asm/reg.h>
#include <asm/nvram.h>
#include <asm/cache.h>
#include <asm/8xx_immap.h>
#include <asm/machdep.h>

<<<<<<< HEAD
=======
#include "setup.h"

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
static struct tau_temp
{
	int interrupts;
	unsigned char low;
	unsigned char high;
	unsigned char grew;
} tau[NR_CPUS];

<<<<<<< HEAD
struct timer_list tau_timer;

#undef DEBUG
=======
static bool tau_int_enable;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

/* TODO: put these in a /proc interface, with some sanity checks, and maybe
 * dynamic adjustment to minimize # of interrupts */
/* configurable values for step size and how much to expand the window when
 * we get an interrupt. These are based on the limit that was out of range */
#define step_size		2	/* step size when temp goes out of range */
#define window_expand		1	/* expand the window by this much */
/* configurable values for shrinking the window */
<<<<<<< HEAD
#define shrink_timer	2*HZ	/* period between shrinking the window */
#define min_window	2	/* minimum window size, degrees C */

void set_thresholds(unsigned long cpu)
{
#ifdef CONFIG_TAU_INT
	/*
	 * setup THRM1,
	 * threshold, valid bit, enable interrupts, interrupt when below threshold
	 */
	mtspr(SPRN_THRM1, THRM1_THRES(tau[cpu].low) | THRM1_V | THRM1_TIE | THRM1_TID);

	/* setup THRM2,
	 * threshold, valid bit, enable interrupts, interrupt when above threshold
	 */
	mtspr (SPRN_THRM2, THRM1_THRES(tau[cpu].high) | THRM1_V | THRM1_TIE);
#else
	/* same thing but don't enable interrupts */
	mtspr(SPRN_THRM1, THRM1_THRES(tau[cpu].low) | THRM1_V | THRM1_TID);
	mtspr(SPRN_THRM2, THRM1_THRES(tau[cpu].high) | THRM1_V);
#endif
}

void TAUupdate(int cpu)
{
	unsigned thrm;

#ifdef DEBUG
	printk("TAUupdate ");
#endif

	/* if both thresholds are crossed, the step_sizes cancel out
	 * and the window winds up getting expanded twice. */
	if((thrm = mfspr(SPRN_THRM1)) & THRM1_TIV){ /* is valid? */
		if(thrm & THRM1_TIN){ /* crossed low threshold */
			if (tau[cpu].low >= step_size){
				tau[cpu].low -= step_size;
				tau[cpu].high -= (step_size - window_expand);
			}
			tau[cpu].grew = 1;
#ifdef DEBUG
			printk("low threshold crossed ");
#endif
		}
	}
	if((thrm = mfspr(SPRN_THRM2)) & THRM1_TIV){ /* is valid? */
		if(thrm & THRM1_TIN){ /* crossed high threshold */
			if (tau[cpu].high <= 127-step_size){
				tau[cpu].low += (step_size - window_expand);
				tau[cpu].high += step_size;
			}
			tau[cpu].grew = 1;
#ifdef DEBUG
			printk("high threshold crossed ");
#endif
		}
	}

#ifdef DEBUG
	printk("grew = %d\n", tau[cpu].grew);
#endif

#ifndef CONFIG_TAU_INT /* tau_timeout will do this if not using interrupts */
	set_thresholds(cpu);
#endif

=======
#define shrink_timer	2000	/* period between shrinking the window */
#define min_window	2	/* minimum window size, degrees C */

static void set_thresholds(unsigned long cpu)
{
	u32 maybe_tie = tau_int_enable ? THRM1_TIE : 0;

	/* setup THRM1, threshold, valid bit, interrupt when below threshold */
	mtspr(SPRN_THRM1, THRM1_THRES(tau[cpu].low) | THRM1_V | maybe_tie | THRM1_TID);

	/* setup THRM2, threshold, valid bit, interrupt when above threshold */
	mtspr(SPRN_THRM2, THRM1_THRES(tau[cpu].high) | THRM1_V | maybe_tie);
}

static void TAUupdate(int cpu)
{
	u32 thrm;
	u32 bits = THRM1_TIV | THRM1_TIN | THRM1_V;

	/* if both thresholds are crossed, the step_sizes cancel out
	 * and the window winds up getting expanded twice. */
	thrm = mfspr(SPRN_THRM1);
	if ((thrm & bits) == bits) {
		mtspr(SPRN_THRM1, 0);

		if (tau[cpu].low >= step_size) {
			tau[cpu].low -= step_size;
			tau[cpu].high -= (step_size - window_expand);
		}
		tau[cpu].grew = 1;
		pr_debug("%s: low threshold crossed\n", __func__);
	}
	thrm = mfspr(SPRN_THRM2);
	if ((thrm & bits) == bits) {
		mtspr(SPRN_THRM2, 0);

		if (tau[cpu].high <= 127 - step_size) {
			tau[cpu].low += (step_size - window_expand);
			tau[cpu].high += step_size;
		}
		tau[cpu].grew = 1;
		pr_debug("%s: high threshold crossed\n", __func__);
	}
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

#ifdef CONFIG_TAU_INT
/*
 * TAU interrupts - called when we have a thermal assist unit interrupt
 * with interrupts disabled
 */

<<<<<<< HEAD
void TAUException(struct pt_regs * regs)
{
	int cpu = smp_processor_id();

	irq_enter();
	tau[cpu].interrupts++;

	TAUupdate(cpu);

	irq_exit();
=======
DEFINE_INTERRUPT_HANDLER_ASYNC(TAUException)
{
	int cpu = smp_processor_id();

	tau[cpu].interrupts++;

	TAUupdate(cpu);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}
#endif /* CONFIG_TAU_INT */

static void tau_timeout(void * info)
{
	int cpu;
<<<<<<< HEAD
	unsigned long flags;
	int size;
	int shrink;

	/* disabling interrupts *should* be okay */
	local_irq_save(flags);
	cpu = smp_processor_id();

#ifndef CONFIG_TAU_INT
	TAUupdate(cpu);
#endif
=======
	int size;
	int shrink;

	cpu = smp_processor_id();

	if (!tau_int_enable)
		TAUupdate(cpu);

	/* Stop thermal sensor comparisons and interrupts */
	mtspr(SPRN_THRM3, 0);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	size = tau[cpu].high - tau[cpu].low;
	if (size > min_window && ! tau[cpu].grew) {
		/* do an exponential shrink of half the amount currently over size */
		shrink = (2 + size - min_window) / 4;
		if (shrink) {
			tau[cpu].low += shrink;
			tau[cpu].high -= shrink;
		} else { /* size must have been min_window + 1 */
			tau[cpu].low += 1;
#if 1 /* debug */
			if ((tau[cpu].high - tau[cpu].low) != min_window){
				printk(KERN_ERR "temp.c: line %d, logic error\n", __LINE__);
			}
#endif
		}
	}

	tau[cpu].grew = 0;

	set_thresholds(cpu);

<<<<<<< HEAD
	/*
	 * Do the enable every time, since otherwise a bunch of (relatively)
	 * complex sleep code needs to be added. One mtspr every time
	 * tau_timeout is called is probably not a big deal.
	 *
	 * Enable thermal sensor and set up sample interval timer
	 * need 20 us to do the compare.. until a nice 'cpu_speed' function
	 * call is implemented, just assume a 500 mhz clock. It doesn't really
	 * matter if we take too long for a compare since it's all interrupt
	 * driven anyway.
	 *
	 * use a extra long time.. (60 us @ 500 mhz)
	 */
	mtspr(SPRN_THRM3, THRM3_SITV(500*60) | THRM3_E);

	local_irq_restore(flags);
}

static void tau_timeout_smp(unsigned long unused)
{

	/* schedule ourselves to be run again */
	mod_timer(&tau_timer, jiffies + shrink_timer) ;
	on_each_cpu(tau_timeout, NULL, 0);
}

=======
	/* Restart thermal sensor comparisons and interrupts.
	 * The "PowerPC 740 and PowerPC 750 Microprocessor Datasheet"
	 * recommends that "the maximum value be set in THRM3 under all
	 * conditions."
	 */
	mtspr(SPRN_THRM3, THRM3_SITV(0x1fff) | THRM3_E);
}

static struct workqueue_struct *tau_workq;

static void tau_work_func(struct work_struct *work)
{
	msleep(shrink_timer);
	on_each_cpu(tau_timeout, NULL, 0);
	/* schedule ourselves to be run again */
	queue_work(tau_workq, work);
}

static DECLARE_WORK(tau_work, tau_work_func);

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
/*
 * setup the TAU
 *
 * Set things up to use THRM1 as a temperature lower bound, and THRM2 as an upper bound.
 * Start off at zero
 */

int tau_initialized = 0;

<<<<<<< HEAD
void __init TAU_init_smp(void * info)
=======
static void __init TAU_init_smp(void *info)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
{
	unsigned long cpu = smp_processor_id();

	/* set these to a reasonable value and let the timer shrink the
	 * window */
	tau[cpu].low = 5;
	tau[cpu].high = 120;

	set_thresholds(cpu);
}

<<<<<<< HEAD
int __init TAU_init(void)
=======
static int __init TAU_init(void)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
{
	/* We assume in SMP that if one CPU has TAU support, they
	 * all have it --BenH
	 */
	if (!cpu_has_feature(CPU_FTR_TAU)) {
		printk("Thermal assist unit not available\n");
		tau_initialized = 0;
		return 1;
	}

<<<<<<< HEAD

	/* first, set up the window shrinking timer */
	init_timer(&tau_timer);
	tau_timer.function = tau_timeout_smp;
	tau_timer.expires = jiffies + shrink_timer;
	add_timer(&tau_timer);

	on_each_cpu(TAU_init_smp, NULL, 0);

	printk("Thermal assist unit ");
#ifdef CONFIG_TAU_INT
	printk("using interrupts, ");
#else
	printk("using timers, ");
#endif
	printk("shrink_timer: %d jiffies\n", shrink_timer);
=======
	tau_int_enable = IS_ENABLED(CONFIG_TAU_INT) &&
			 !strcmp(cur_cpu_spec->platform, "ppc750");

	tau_workq = alloc_ordered_workqueue("tau", 0);
	if (!tau_workq)
		return -ENOMEM;

	on_each_cpu(TAU_init_smp, NULL, 0);

	queue_work(tau_workq, &tau_work);

	pr_info("Thermal assist unit using %s, shrink_timer: %d ms\n",
		tau_int_enable ? "interrupts" : "workqueue", shrink_timer);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	tau_initialized = 1;

	return 0;
}

__initcall(TAU_init);

/*
 * return current temp
 */

u32 cpu_temp_both(unsigned long cpu)
{
	return ((tau[cpu].high << 16) | tau[cpu].low);
}

<<<<<<< HEAD
int cpu_temp(unsigned long cpu)
=======
u32 cpu_temp(unsigned long cpu)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
{
	return ((tau[cpu].high + tau[cpu].low) / 2);
}

<<<<<<< HEAD
int tau_interrupts(unsigned long cpu)
=======
u32 tau_interrupts(unsigned long cpu)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
{
	return (tau[cpu].interrupts);
}
