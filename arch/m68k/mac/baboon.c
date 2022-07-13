<<<<<<< HEAD
=======
// SPDX-License-Identifier: GPL-2.0
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
/*
 * Baboon Custom IC Management
 *
 * The Baboon custom IC controls the IDE, PCMCIA and media bay on the
 * PowerBook 190. It multiplexes multiple interrupt sources onto the
 * Nubus slot $C interrupt.
 */

#include <linux/types.h>
#include <linux/kernel.h>
#include <linux/irq.h>

#include <asm/macintosh.h>
#include <asm/macints.h>
#include <asm/mac_baboon.h>

<<<<<<< HEAD
/* #define DEBUG_IRQS */
=======
#include "mac.h"
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

int baboon_present;
static volatile struct baboon *baboon;

<<<<<<< HEAD
#if 0
extern int macide_ack_intr(struct ata_channel *);
#endif

=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
/*
 * Baboon initialization.
 */

void __init baboon_init(void)
{
	if (macintosh_config->ident != MAC_MODEL_PB190) {
		baboon = NULL;
		baboon_present = 0;
		return;
	}

	baboon = (struct baboon *) BABOON_BASE;
	baboon_present = 1;

<<<<<<< HEAD
	printk("Baboon detected at %p\n", baboon);
}

/*
 * Baboon interrupt handler. This works a lot like a VIA.
 */

static void baboon_irq(unsigned int irq, struct irq_desc *desc)
{
	int irq_bit, irq_num;
	unsigned char events;

#ifdef DEBUG_IRQS
	printk("baboon_irq: mb_control %02X mb_ifr %02X mb_status %02X\n",
		(uint) baboon->mb_control, (uint) baboon->mb_ifr,
		(uint) baboon->mb_status);
#endif

	events = baboon->mb_ifr & 0x07;
	if (!events)
		return;

	irq_num = IRQ_BABOON_0;
	irq_bit = 1;
	do {
	        if (events & irq_bit) {
			baboon->mb_ifr &= ~irq_bit;
			generic_handle_irq(irq_num);
		}
		irq_bit <<= 1;
		irq_num++;
	} while(events >= irq_bit);
#if 0
	if (baboon->mb_ifr & 0x02) macide_ack_intr(NULL);
	/* for now we need to smash all interrupts */
	baboon->mb_ifr &= ~events;
#endif
=======
	pr_debug("Baboon detected at %p\n", baboon);
}

/*
 * Baboon interrupt handler.
 * XXX how do you clear a pending IRQ? is it even necessary?
 */

static void baboon_irq(struct irq_desc *desc)
{
	short events, irq_bit;
	int irq_num;

	events = baboon->mb_ifr & 0x07;
	irq_num = IRQ_BABOON_0;
	irq_bit = 1;
	do {
		if (events & irq_bit) {
			events &= ~irq_bit;
			generic_handle_irq(irq_num);
		}
		++irq_num;
		irq_bit <<= 1;
	} while (events);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

/*
 * Register the Baboon interrupt dispatcher on nubus slot $C.
 */

void __init baboon_register_interrupts(void)
{
	irq_set_chained_handler(IRQ_NUBUS_C, baboon_irq);
}

/*
 * The means for masking individual Baboon interrupts remains a mystery.
 * However, since we only use the IDE IRQ, we can just enable/disable all
 * Baboon interrupts. If/when we handle more than one Baboon IRQ, we must
 * either figure out how to mask them individually or else implement the
 * same workaround that's used for NuBus slots (see nubus_disabled and
 * via_nubus_irq_shutdown).
 */

void baboon_irq_enable(int irq)
{
<<<<<<< HEAD
#ifdef DEBUG_IRQUSE
	printk("baboon_irq_enable(%d)\n", irq);
#endif

=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	mac_irq_enable(irq_get_irq_data(IRQ_NUBUS_C));
}

void baboon_irq_disable(int irq)
{
<<<<<<< HEAD
#ifdef DEBUG_IRQUSE
	printk("baboon_irq_disable(%d)\n", irq);
#endif

=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	mac_irq_disable(irq_get_irq_data(IRQ_NUBUS_C));
}
