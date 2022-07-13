<<<<<<< HEAD
=======
/* SPDX-License-Identifier: GPL-2.0-or-later */
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#ifdef __KERNEL__
#ifndef _ASM_POWERPC_IRQ_H
#define _ASM_POWERPC_IRQ_H

/*
<<<<<<< HEAD
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version
 * 2 of the License, or (at your option) any later version.
 */

#include <linux/irqdomain.h>
=======
 */

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#include <linux/threads.h>
#include <linux/list.h>
#include <linux/radix-tree.h>

#include <asm/types.h>
#include <linux/atomic.h>


extern atomic_t ppc_n_lost_interrupts;

<<<<<<< HEAD
/* This number is used when no interrupt has been assigned */
#define NO_IRQ			(0)

/* Total number of virq in the platform */
#define NR_IRQS		CONFIG_NR_IRQS

/* Same thing, used by the generic IRQ code */
#define NR_IRQS_LEGACY		NUM_ISA_INTERRUPTS

extern irq_hw_number_t virq_to_hw(unsigned int virq);

/**
 * irq_early_init - Init irq remapping subsystem
 */
extern void irq_early_init(void);

=======
/* Total number of virq in the platform */
#define NR_IRQS		CONFIG_NR_IRQS

/* Number of irqs reserved for a legacy isa controller */
#define NR_IRQS_LEGACY		16

extern irq_hw_number_t virq_to_hw(unsigned int virq);

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
static __inline__ int irq_canonicalize(int irq)
{
	return irq;
}

extern int distribute_irqs;

<<<<<<< HEAD
struct irqaction;
struct pt_regs;

#define __ARCH_HAS_DO_SOFTIRQ

#if defined(CONFIG_BOOKE) || defined(CONFIG_40x)
=======
struct pt_regs;

#ifdef CONFIG_BOOKE_OR_40x
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
/*
 * Per-cpu stacks for handling critical, debug and machine check
 * level interrupts.
 */
<<<<<<< HEAD
extern struct thread_info *critirq_ctx[NR_CPUS];
extern struct thread_info *dbgirq_ctx[NR_CPUS];
extern struct thread_info *mcheckirq_ctx[NR_CPUS];
extern void exc_lvl_ctx_init(void);
#else
#define exc_lvl_ctx_init()
=======
extern void *critirq_ctx[NR_CPUS];
extern void *dbgirq_ctx[NR_CPUS];
extern void *mcheckirq_ctx[NR_CPUS];
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#endif

/*
 * Per-cpu stacks for handling hard and soft interrupts.
 */
<<<<<<< HEAD
extern struct thread_info *hardirq_ctx[NR_CPUS];
extern struct thread_info *softirq_ctx[NR_CPUS];

extern void irq_ctx_init(void);
extern void call_do_softirq(struct thread_info *tp);
extern int call_handle_irq(int irq, void *p1,
			   struct thread_info *tp, void *func);
extern void do_IRQ(struct pt_regs *regs);

int irq_choose_cpu(const struct cpumask *mask);

=======
extern void *hardirq_ctx[NR_CPUS];
extern void *softirq_ctx[NR_CPUS];

void __do_IRQ(struct pt_regs *regs);

int irq_choose_cpu(const struct cpumask *mask);

#if defined(CONFIG_PPC_BOOK3S_64) && defined(CONFIG_NMI_IPI)
extern void arch_trigger_cpumask_backtrace(const cpumask_t *mask,
					   int exclude_cpu);
#define arch_trigger_cpumask_backtrace arch_trigger_cpumask_backtrace
#endif

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#endif /* _ASM_IRQ_H */
#endif /* __KERNEL__ */
