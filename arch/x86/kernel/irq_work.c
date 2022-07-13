<<<<<<< HEAD
/*
 * x86 specific code for irq_work
 *
 * Copyright (C) 2010 Red Hat, Inc., Peter Zijlstra <pzijlstr@redhat.com>
=======
// SPDX-License-Identifier: GPL-2.0
/*
 * x86 specific code for irq_work
 *
 * Copyright (C) 2010 Red Hat, Inc., Peter Zijlstra
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 */

#include <linux/kernel.h>
#include <linux/irq_work.h>
#include <linux/hardirq.h>
#include <asm/apic.h>
<<<<<<< HEAD

void smp_irq_work_interrupt(struct pt_regs *regs)
{
	irq_enter();
	ack_APIC_irq();
	inc_irq_stat(apic_irq_work_irqs);
	irq_work_run();
	irq_exit();
=======
#include <asm/idtentry.h>
#include <asm/trace/irq_vectors.h>
#include <linux/interrupt.h>

#ifdef CONFIG_X86_LOCAL_APIC
DEFINE_IDTENTRY_SYSVEC(sysvec_irq_work)
{
	apic_eoi();
	trace_irq_work_entry(IRQ_WORK_VECTOR);
	inc_irq_stat(apic_irq_work_irqs);
	irq_work_run();
	trace_irq_work_exit(IRQ_WORK_VECTOR);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

void arch_irq_work_raise(void)
{
<<<<<<< HEAD
#ifdef CONFIG_X86_LOCAL_APIC
	if (!cpu_has_apic)
		return;

	apic->send_IPI_self(IRQ_WORK_VECTOR);
	apic_wait_icr_idle();
#endif
}
=======
	if (!arch_irq_work_has_interrupt())
		return;

	__apic_send_IPI_self(IRQ_WORK_VECTOR);
	apic_wait_icr_idle();
}
#endif
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
