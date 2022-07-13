<<<<<<< HEAD
#ifndef _ASM_IRQ_H
#define _ASM_IRQ_H

#include <linux/hardirq.h>
#include <linux/types.h>

enum interruption_class {
	EXTERNAL_INTERRUPT,
	IO_INTERRUPT,
	EXTINT_CLK,
	EXTINT_EXC,
	EXTINT_EMS,
	EXTINT_TMR,
	EXTINT_TLA,
	EXTINT_PFL,
	EXTINT_DSD,
	EXTINT_VRT,
	EXTINT_SCP,
	EXTINT_IUC,
	EXTINT_CPM,
	IOINT_CIO,
	IOINT_QAI,
	IOINT_DAS,
	IOINT_C15,
	IOINT_C70,
	IOINT_TAP,
	IOINT_VMR,
	IOINT_LCS,
	IOINT_CLW,
	IOINT_CTC,
	IOINT_APB,
	IOINT_CSC,
	NMI_NMI,
	NR_IRQS,
};

struct ext_code {
	unsigned short subcode;
	unsigned short code;
=======
/* SPDX-License-Identifier: GPL-2.0 */
#ifndef _ASM_IRQ_H
#define _ASM_IRQ_H

#define EXT_INTERRUPT	0
#define IO_INTERRUPT	1
#define THIN_INTERRUPT	2

#define NR_IRQS_BASE	3

#define NR_IRQS	NR_IRQS_BASE
#define NR_IRQS_LEGACY NR_IRQS_BASE

/* External interruption codes */
#define EXT_IRQ_INTERRUPT_KEY	0x0040
#define EXT_IRQ_CLK_COMP	0x1004
#define EXT_IRQ_CPU_TIMER	0x1005
#define EXT_IRQ_WARNING_TRACK	0x1007
#define EXT_IRQ_MALFUNC_ALERT	0x1200
#define EXT_IRQ_EMERGENCY_SIG	0x1201
#define EXT_IRQ_EXTERNAL_CALL	0x1202
#define EXT_IRQ_TIMING_ALERT	0x1406
#define EXT_IRQ_MEASURE_ALERT	0x1407
#define EXT_IRQ_SERVICE_SIG	0x2401
#define EXT_IRQ_CP_SERVICE	0x2603
#define EXT_IRQ_IUCV		0x4000

#ifndef __ASSEMBLY__

#include <linux/hardirq.h>
#include <linux/percpu.h>
#include <linux/cache.h>
#include <linux/types.h>
#include <asm/ctlreg.h>

enum interruption_class {
	IRQEXT_CLK,
	IRQEXT_EXC,
	IRQEXT_EMS,
	IRQEXT_TMR,
	IRQEXT_TLA,
	IRQEXT_PFL,
	IRQEXT_DSD,
	IRQEXT_VRT,
	IRQEXT_SCP,
	IRQEXT_IUC,
	IRQEXT_CMS,
	IRQEXT_CMC,
	IRQEXT_FTP,
	IRQIO_CIO,
	IRQIO_DAS,
	IRQIO_C15,
	IRQIO_C70,
	IRQIO_TAP,
	IRQIO_VMR,
	IRQIO_LCS,
	IRQIO_CTC,
	IRQIO_ADM,
	IRQIO_CSC,
	IRQIO_VIR,
	IRQIO_QAI,
	IRQIO_APB,
	IRQIO_PCF,
	IRQIO_PCD,
	IRQIO_MSI,
	IRQIO_VAI,
	IRQIO_GAL,
	NMI_NMI,
	CPU_RST,
	NR_ARCH_IRQS
};

struct irq_stat {
	unsigned int irqs[NR_ARCH_IRQS];
};

DECLARE_PER_CPU_SHARED_ALIGNED(struct irq_stat, irq_stat);

static __always_inline void inc_irq_stat(enum interruption_class irq)
{
	__this_cpu_inc(irq_stat.irqs[irq]);
}

struct ext_code {
	union {
		struct {
			unsigned short subcode;
			unsigned short code;
		};
		unsigned int int_code;
	};
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
};

typedef void (*ext_int_handler_t)(struct ext_code, unsigned int, unsigned long);

<<<<<<< HEAD
int register_external_interrupt(u16 code, ext_int_handler_t handler);
int unregister_external_interrupt(u16 code, ext_int_handler_t handler);
void service_subclass_irq_register(void);
void service_subclass_irq_unregister(void);
void measurement_alert_subclass_register(void);
void measurement_alert_subclass_unregister(void);
=======
int register_external_irq(u16 code, ext_int_handler_t handler);
int unregister_external_irq(u16 code, ext_int_handler_t handler);

enum irq_subclass {
	IRQ_SUBCLASS_MEASUREMENT_ALERT = 5,
	IRQ_SUBCLASS_SERVICE_SIGNAL = 9,
};

#define CR0_IRQ_SUBCLASS_MASK					  \
	(CR0_WARNING_TRACK					| \
	 CR0_MALFUNCTION_ALERT_SUBMASK				| \
	 CR0_EMERGENCY_SIGNAL_SUBMASK				| \
	 CR0_EXTERNAL_CALL_SUBMASK				| \
	 CR0_CLOCK_COMPARATOR_SUBMASK				| \
	 CR0_CPU_TIMER_SUBMASK					| \
	 CR0_SERVICE_SIGNAL_SUBMASK				| \
	 CR0_INTERRUPT_KEY_SUBMASK				| \
	 CR0_MEASUREMENT_ALERT_SUBMASK				| \
	 CR0_ETR_SUBMASK					| \
	 CR0_IUCV)

void irq_subclass_register(enum irq_subclass subclass);
void irq_subclass_unregister(enum irq_subclass subclass);

#define irq_canonicalize(irq)  (irq)

#endif /* __ASSEMBLY__ */
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

#endif /* _ASM_IRQ_H */
