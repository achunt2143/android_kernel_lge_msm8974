<<<<<<< HEAD
=======
/* SPDX-License-Identifier: GPL-2.0 */
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#ifndef _ASM_POWERPC_HARDIRQ_H
#define _ASM_POWERPC_HARDIRQ_H

#include <linux/threads.h>
#include <linux/irq.h>

typedef struct {
	unsigned int __softirq_pending;
<<<<<<< HEAD
	unsigned int timer_irqs;
	unsigned int pmu_irqs;
	unsigned int mce_exceptions;
	unsigned int spurious_irqs;
=======
	unsigned int timer_irqs_event;
	unsigned int broadcast_irqs_event;
	unsigned int timer_irqs_others;
	unsigned int pmu_irqs;
	unsigned int mce_exceptions;
	unsigned int spurious_irqs;
	unsigned int sreset_irqs;
#ifdef CONFIG_PPC_WATCHDOG
	unsigned int soft_nmi_irqs;
#endif
#ifdef CONFIG_PPC_DOORBELL
	unsigned int doorbell_irqs;
#endif
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
} ____cacheline_aligned irq_cpustat_t;

DECLARE_PER_CPU_SHARED_ALIGNED(irq_cpustat_t, irq_stat);

#define __ARCH_IRQ_STAT
<<<<<<< HEAD

#define local_softirq_pending()	__get_cpu_var(irq_stat).__softirq_pending
=======
#define __ARCH_IRQ_EXIT_IRQS_DISABLED
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

static inline void ack_bad_irq(unsigned int irq)
{
	printk(KERN_CRIT "unexpected IRQ trap at vector %02x\n", irq);
}

extern u64 arch_irq_stat_cpu(unsigned int cpu);
#define arch_irq_stat_cpu	arch_irq_stat_cpu

#endif /* _ASM_POWERPC_HARDIRQ_H */
