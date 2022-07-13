<<<<<<< HEAD
=======
// SPDX-License-Identifier: GPL-2.0-or-later
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
/*
 * Author: Kumar Gala <galak@kernel.crashing.org>
 *
 * Copyright 2009 Freescale Semiconductor Inc.
<<<<<<< HEAD
 *
 * This program is free software; you can redistribute  it and/or modify it
 * under  the terms of  the GNU General  Public License as published by the
 * Free Software Foundation;  either version 2 of the  License, or (at your
 * option) any later version.
=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 */

#include <linux/stddef.h>
#include <linux/kernel.h>
#include <linux/smp.h>
#include <linux/threads.h>
#include <linux/hardirq.h>

#include <asm/dbell.h>
<<<<<<< HEAD
#include <asm/irq_regs.h>

#ifdef CONFIG_SMP
void doorbell_setup_this_cpu(void)
{
	unsigned long tag = mfspr(SPRN_PIR) & 0x3fff;

	smp_muxed_ipi_set_data(smp_processor_id(), tag);
}

void doorbell_cause_ipi(int cpu, unsigned long data)
{
	/* Order previous accesses vs. msgsnd, which is treated as a store */
	mb();
	ppc_msgsnd(PPC_DBELL, 0, data);
}

void doorbell_exception(struct pt_regs *regs)
{
	struct pt_regs *old_regs = set_irq_regs(regs);

	irq_enter();

	may_hard_irq_enable();

	smp_ipi_demux();

	irq_exit();
	set_irq_regs(old_regs);
}
#else /* CONFIG_SMP */
void doorbell_exception(struct pt_regs *regs)
=======
#include <asm/interrupt.h>
#include <asm/irq_regs.h>
#include <asm/kvm_ppc.h>
#include <asm/trace.h>

#ifdef CONFIG_SMP

DEFINE_INTERRUPT_HANDLER_ASYNC(doorbell_exception)
{
	struct pt_regs *old_regs = set_irq_regs(regs);

	trace_doorbell_entry(regs);

	ppc_msgsync();

	if (should_hard_irq_enable(regs))
		do_hard_irq_enable();

	kvmppc_clear_host_ipi(smp_processor_id());
	__this_cpu_inc(irq_stat.doorbell_irqs);

	smp_ipi_demux_relaxed(); /* already performed the barrier */

	trace_doorbell_exit(regs);

	set_irq_regs(old_regs);
}
#else /* CONFIG_SMP */
DEFINE_INTERRUPT_HANDLER_ASYNC(doorbell_exception)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
{
	printk(KERN_WARNING "Received doorbell on non-smp system\n");
}
#endif /* CONFIG_SMP */
<<<<<<< HEAD

=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
