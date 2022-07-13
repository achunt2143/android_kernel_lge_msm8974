<<<<<<< HEAD
=======
// SPDX-License-Identifier: GPL-2.0
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
/*
 * 8253/PIT functions
 *
 */
#include <linux/clockchips.h>
<<<<<<< HEAD
#include <linux/module.h>
#include <linux/timex.h>
#include <linux/i8253.h>

=======
#include <linux/init.h>
#include <linux/timex.h>
#include <linux/i8253.h>

#include <asm/apic.h>
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#include <asm/hpet.h>
#include <asm/time.h>
#include <asm/smp.h>

/*
 * HPET replaces the PIT, when enabled. So we need to know, which of
 * the two timers is used
 */
struct clock_event_device *global_clock_event;

<<<<<<< HEAD
void __init setup_pit_timer(void)
{
	clockevent_i8253_init(true);
	global_clock_event = &i8253_clockevent;
=======
/*
 * Modern chipsets can disable the PIT clock which makes it unusable. It
 * would be possible to enable the clock but the registers are chipset
 * specific and not discoverable. Avoid the whack a mole game.
 *
 * These platforms have discoverable TSC/CPU frequencies but this also
 * requires to know the local APIC timer frequency as it normally is
 * calibrated against the PIT interrupt.
 */
static bool __init use_pit(void)
{
	if (!IS_ENABLED(CONFIG_X86_TSC) || !boot_cpu_has(X86_FEATURE_TSC))
		return true;

	/* This also returns true when APIC is disabled */
	return apic_needs_pit();
}

bool __init pit_timer_init(void)
{
	if (!use_pit())
		return false;

	clockevent_i8253_init(true);
	global_clock_event = &i8253_clockevent;
	return true;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

#ifndef CONFIG_X86_64
static int __init init_pit_clocksource(void)
{
	 /*
	  * Several reasons not to register PIT as a clocksource:
	  *
	  * - On SMP PIT does not scale due to i8253_lock
	  * - when HPET is enabled
	  * - when local APIC timer is active (PIT is switched off)
	  */
	if (num_possible_cpus() > 1 || is_hpet_enabled() ||
<<<<<<< HEAD
	    i8253_clockevent.mode != CLOCK_EVT_MODE_PERIODIC)
=======
	    !clockevent_state_periodic(&i8253_clockevent))
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		return 0;

	return clocksource_i8253_init();
}
arch_initcall(init_pit_clocksource);
#endif /* !CONFIG_X86_64 */
