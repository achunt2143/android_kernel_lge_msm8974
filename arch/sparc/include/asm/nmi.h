<<<<<<< HEAD
#ifndef __NMI_H
#define __NMI_H

extern int __init nmi_init(void);
extern void perfctr_irq(int irq, struct pt_regs *regs);
extern void nmi_adjust_hz(unsigned int new_hz);

extern atomic_t nmi_active;

extern void start_nmi_watchdog(void *unused);
extern void stop_nmi_watchdog(void *unused);
=======
/* SPDX-License-Identifier: GPL-2.0 */
#ifndef __NMI_H
#define __NMI_H

int __init nmi_init(void);
void perfctr_irq(int irq, struct pt_regs *regs);
void nmi_adjust_hz(unsigned int new_hz);

extern atomic_t nmi_active;

void start_nmi_watchdog(void *unused);
void stop_nmi_watchdog(void *unused);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

#endif /* __NMI_H */
