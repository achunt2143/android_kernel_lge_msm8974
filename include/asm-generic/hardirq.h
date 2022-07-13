<<<<<<< HEAD
=======
/* SPDX-License-Identifier: GPL-2.0 */
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#ifndef __ASM_GENERIC_HARDIRQ_H
#define __ASM_GENERIC_HARDIRQ_H

#include <linux/cache.h>
#include <linux/threads.h>

typedef struct {
	unsigned int __softirq_pending;
<<<<<<< HEAD
} ____cacheline_aligned irq_cpustat_t;

#include <linux/irq_cpustat.h>	/* Standard mappings for irq_cpustat_t above */
=======
#ifdef ARCH_WANTS_NMI_IRQSTAT
	unsigned int __nmi_count;
#endif
} ____cacheline_aligned irq_cpustat_t;

DECLARE_PER_CPU_ALIGNED(irq_cpustat_t, irq_stat);

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#include <linux/irq.h>

#ifndef ack_bad_irq
static inline void ack_bad_irq(unsigned int irq)
{
	printk(KERN_CRIT "unexpected IRQ trap at vector %02x\n", irq);
}
#endif

#endif /* __ASM_GENERIC_HARDIRQ_H */
