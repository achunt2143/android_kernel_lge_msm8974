/*
 * Copyright (C) 2007-2009 Michal Simek <monstr@monstr.eu>
 * Copyright (C) 2007-2009 PetaLogix
 * Copyright (C) 2006 Atmark Techno, Inc.
 *
 * This file is subject to the terms and conditions of the GNU General Public
 * License. See the file "COPYING" in the main directory of this archive
 * for more details.
 */

#include <linux/init.h>
#include <linux/ftrace.h>
#include <linux/kernel.h>
#include <linux/hardirq.h>
#include <linux/interrupt.h>
#include <linux/irqflags.h>
#include <linux/seq_file.h>
#include <linux/kernel_stat.h>
#include <linux/irq.h>
<<<<<<< HEAD
#include <linux/of_irq.h>
#include <linux/export.h>

#include <asm/prom.h>

static u32 concurrent_irq;

void __irq_entry do_IRQ(struct pt_regs *regs)
{
	unsigned int irq;
=======
#include <linux/irqchip.h>
#include <linux/of_irq.h>

void __irq_entry do_IRQ(struct pt_regs *regs)
{
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	struct pt_regs *old_regs = set_irq_regs(regs);
	trace_hardirqs_off();

	irq_enter();
<<<<<<< HEAD
	irq = get_irq();
next_irq:
	BUG_ON(!irq);
	generic_handle_irq(irq);

	irq = get_irq();
	if (irq != -1U) {
		pr_debug("next irq: %d\n", irq);
		++concurrent_irq;
		goto next_irq;
	}

=======
	handle_arch_irq(regs);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	irq_exit();
	set_irq_regs(old_regs);
	trace_hardirqs_on();
}
<<<<<<< HEAD
=======

void __init init_IRQ(void)
{
	/* process the entire interrupt tree in one go */
	irqchip_init();
}
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
