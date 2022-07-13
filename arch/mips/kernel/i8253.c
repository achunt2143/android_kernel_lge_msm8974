<<<<<<< HEAD
=======
// SPDX-License-Identifier: GPL-2.0
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
/*
 * i8253.c  8253/PIT functions
 *
 */
#include <linux/clockchips.h>
#include <linux/i8253.h>
#include <linux/export.h>
#include <linux/smp.h>
#include <linux/irq.h>

#include <asm/time.h>

static irqreturn_t timer_interrupt(int irq, void *dev_id)
{
	i8253_clockevent.event_handler(&i8253_clockevent);

	return IRQ_HANDLED;
}

<<<<<<< HEAD
static struct irqaction irq0  = {
	.handler = timer_interrupt,
	.flags = IRQF_NOBALANCING | IRQF_TIMER,
	.name = "timer"
};

void __init setup_pit_timer(void)
{
	clockevent_i8253_init(true);
	setup_irq(0, &irq0);
=======
void __init setup_pit_timer(void)
{
	unsigned long flags = IRQF_NOBALANCING | IRQF_TIMER;

	clockevent_i8253_init(true);
	if (request_irq(0, timer_interrupt, flags, "timer", NULL))
		pr_err("Failed to request irq 0 (timer)\n");
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

static int __init init_pit_clocksource(void)
{
<<<<<<< HEAD
	if (num_possible_cpus() > 1) /* PIT does not scale! */
=======
	if (num_possible_cpus() > 1 || /* PIT does not scale! */
	    !clockevent_state_periodic(&i8253_clockevent))
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		return 0;

	return clocksource_i8253_init();
}
arch_initcall(init_pit_clocksource);
