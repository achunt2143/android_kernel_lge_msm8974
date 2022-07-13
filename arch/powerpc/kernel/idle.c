<<<<<<< HEAD
=======
// SPDX-License-Identifier: GPL-2.0-or-later
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
/*
 * Idle daemon for PowerPC.  Idle daemon will handle any action
 * that needs to be taken when the system becomes idle.
 *
 * Originally written by Cort Dougan (cort@cs.nmt.edu).
 * Subsequent 32-bit hacking by Tom Rini, Armin Kuster,
 * Paul Mackerras and others.
 *
 * iSeries supported added by Mike Corrigan <mikejc@us.ibm.com>
 *
 * Additional shared processor, SMT, and firmware support
 *    Copyright (c) 2003 Dave Engebretsen <engebret@us.ibm.com>
 *
 * 32-bit and 64-bit versions merged by Paul Mackerras <paulus@samba.org>
<<<<<<< HEAD
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version
 * 2 of the License, or (at your option) any later version.
=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 */

#include <linux/sched.h>
#include <linux/kernel.h>
#include <linux/smp.h>
#include <linux/cpu.h>
#include <linux/sysctl.h>
#include <linux/tick.h>

#include <asm/processor.h>
#include <asm/cputable.h>
#include <asm/time.h>
#include <asm/machdep.h>
#include <asm/runlatch.h>
#include <asm/smp.h>

<<<<<<< HEAD
#ifdef CONFIG_HOTPLUG_CPU
#define cpu_should_die()	cpu_is_offline(smp_processor_id())
#else
#define cpu_should_die()	0
#endif
=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

unsigned long cpuidle_disable = IDLE_NO_OVERRIDE;
EXPORT_SYMBOL(cpuidle_disable);

static int __init powersave_off(char *arg)
{
	ppc_md.power_save = NULL;
	cpuidle_disable = IDLE_POWERSAVE_OFF;
<<<<<<< HEAD
	return 0;
}
__setup("powersave=off", powersave_off);

/*
 * The body of the idle task.
 */
void cpu_idle(void)
{
	if (ppc_md.idle_loop)
		ppc_md.idle_loop();	/* doesn't return */

	set_thread_flag(TIF_POLLING_NRFLAG);
	while (1) {
		tick_nohz_idle_enter();
		rcu_idle_enter();

		while (!need_resched() && !cpu_should_die()) {
			ppc64_runlatch_off();

			if (ppc_md.power_save) {
				clear_thread_flag(TIF_POLLING_NRFLAG);
				/*
				 * smp_mb is so clearing of TIF_POLLING_NRFLAG
				 * is ordered w.r.t. need_resched() test.
				 */
				smp_mb();
				local_irq_disable();

				/* Don't trace irqs off for idle */
				stop_critical_timings();

				/* check again after disabling irqs */
				if (!need_resched() && !cpu_should_die())
					ppc_md.power_save();

				start_critical_timings();

				/* Some power_save functions return with
				 * interrupts enabled, some don't.
				 */
				if (irqs_disabled())
					local_irq_enable();
				set_thread_flag(TIF_POLLING_NRFLAG);

			} else {
				/*
				 * Go into low thread priority and possibly
				 * low power mode.
				 */
				HMT_low();
				HMT_very_low();
			}
		}

		HMT_medium();
		ppc64_runlatch_on();
		rcu_idle_exit();
		tick_nohz_idle_exit();
		if (cpu_should_die()) {
			sched_preempt_enable_no_resched();
			cpu_die();
		}
		schedule_preempt_disabled();
	}
}

static void do_nothing(void *unused)
{
}

/*
 * cpu_idle_wait - Used to ensure that all the CPUs come out of the old
 * idle loop and start using the new idle loop.
 * Required while changing idle handler on SMP systems.
 * Caller must have changed idle handler to the new value before the call.
 * This window may be larger on shared systems.
 */
void cpu_idle_wait(void)
{
	smp_mb();
	/* kick all the CPUs so that they exit out of pm_idle */
	smp_call_function(do_nothing, NULL, 1);
}
EXPORT_SYMBOL_GPL(cpu_idle_wait);

int powersave_nap;

=======
	return 1;
}
__setup("powersave=off", powersave_off);

void arch_cpu_idle(void)
{
	ppc64_runlatch_off();

	if (ppc_md.power_save) {
		ppc_md.power_save();
		/*
		 * Some power_save functions return with
		 * interrupts enabled, some don't.
		 */
		if (!irqs_disabled())
			raw_local_irq_disable();
	} else {
		/*
		 * Go into low thread priority and possibly
		 * low power mode.
		 */
		HMT_low();
		HMT_very_low();
	}

	HMT_medium();
	ppc64_runlatch_on();
}

int powersave_nap;

#ifdef CONFIG_PPC_970_NAP
void power4_idle(void)
{
	if (!cpu_has_feature(CPU_FTR_CAN_NAP))
		return;

	if (!powersave_nap)
		return;

	if (!prep_irq_for_idle())
		return;

	if (cpu_has_feature(CPU_FTR_ALTIVEC))
		asm volatile(PPC_DSSALL " ; sync" ::: "memory");

	power4_idle_nap();

	/*
	 * power4_idle_nap returns with interrupts enabled (soft and hard).
	 * to our caller with interrupts enabled (soft and hard). Our caller
	 * can cope with either interrupts disabled or enabled upon return.
	 */
}
#endif

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#ifdef CONFIG_SYSCTL
/*
 * Register the sysctl to set/clear powersave_nap.
 */
<<<<<<< HEAD
static ctl_table powersave_nap_ctl_table[]={
=======
static struct ctl_table powersave_nap_ctl_table[] = {
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	{
		.procname	= "powersave-nap",
		.data		= &powersave_nap,
		.maxlen		= sizeof(int),
		.mode		= 0644,
		.proc_handler	= proc_dointvec,
	},
<<<<<<< HEAD
	{}
};
static ctl_table powersave_nap_sysctl_root[] = {
	{
		.procname	= "kernel",
		.mode		= 0555,
		.child		= powersave_nap_ctl_table,
	},
	{}
=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
};

static int __init
register_powersave_nap_sysctl(void)
{
<<<<<<< HEAD
	register_sysctl_table(powersave_nap_sysctl_root);
=======
	register_sysctl("kernel", powersave_nap_ctl_table);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	return 0;
}
__initcall(register_powersave_nap_sysctl);
#endif
