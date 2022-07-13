<<<<<<< HEAD
#ifndef _ASM_X86_IRQ_REMAPPING_H
#define _ASM_X86_IRQ_REMAPPING_H

#define IRTE_DEST(dest) ((x2apic_mode) ? dest : dest << 8)

#ifdef CONFIG_IRQ_REMAP
static void irq_remap_modify_chip_defaults(struct irq_chip *chip);
static inline void prepare_irte(struct irte *irte, int vector,
			        unsigned int dest)
{
	memset(irte, 0, sizeof(*irte));

	irte->present = 1;
	irte->dst_mode = apic->irq_dest_mode;
	/*
	 * Trigger mode in the IRTE will always be edge, and for IO-APIC, the
	 * actual level or edge trigger will be setup in the IO-APIC
	 * RTE. This will help simplify level triggered irq migration.
	 * For more details, see the comments (in io_apic.c) explainig IO-APIC
	 * irq migration in the presence of interrupt-remapping.
	*/
	irte->trigger_mode = 0;
	irte->dlvry_mode = apic->irq_delivery_mode;
	irte->vector = vector;
	irte->dest_id = IRTE_DEST(dest);
	irte->redir_hint = 1;
}
static inline bool irq_remapped(struct irq_cfg *cfg)
{
	return cfg->irq_2_iommu.iommu != NULL;
}
#else
static void prepare_irte(struct irte *irte, int vector, unsigned int dest)
{
}
static inline bool irq_remapped(struct irq_cfg *cfg)
{
	return false;
}
static inline void irq_remap_modify_chip_defaults(struct irq_chip *chip)
{
}
#endif

#endif	/* _ASM_X86_IRQ_REMAPPING_H */
=======
/* SPDX-License-Identifier: GPL-2.0-only */
/*
 * Copyright (C) 2012 Advanced Micro Devices, Inc.
 * Author: Joerg Roedel <joerg.roedel@amd.com>
 *
 * This header file contains the interface of the interrupt remapping code to
 * the x86 interrupt management code.
 */

#ifndef __X86_IRQ_REMAPPING_H
#define __X86_IRQ_REMAPPING_H

#include <asm/irqdomain.h>
#include <asm/hw_irq.h>
#include <asm/io_apic.h>

struct msi_msg;
struct irq_alloc_info;

enum irq_remap_cap {
	IRQ_POSTING_CAP = 0,
};

enum {
	IRQ_REMAP_XAPIC_MODE,
	IRQ_REMAP_X2APIC_MODE,
};

struct vcpu_data {
	u64 pi_desc_addr;	/* Physical address of PI Descriptor */
	u32 vector;		/* Guest vector of the interrupt */
};

#ifdef CONFIG_IRQ_REMAP

extern raw_spinlock_t irq_2_ir_lock;

extern bool irq_remapping_cap(enum irq_remap_cap cap);
extern void set_irq_remapping_broken(void);
extern int irq_remapping_prepare(void);
extern int irq_remapping_enable(void);
extern void irq_remapping_disable(void);
extern int irq_remapping_reenable(int);
extern int irq_remap_enable_fault_handling(void);
extern void panic_if_irq_remap(const char *msg);

/* Get parent irqdomain for interrupt remapping irqdomain */
static inline struct irq_domain *arch_get_ir_parent_domain(void)
{
	return x86_vector_domain;
}

#else  /* CONFIG_IRQ_REMAP */

static inline bool irq_remapping_cap(enum irq_remap_cap cap) { return 0; }
static inline void set_irq_remapping_broken(void) { }
static inline int irq_remapping_prepare(void) { return -ENODEV; }
static inline int irq_remapping_enable(void) { return -ENODEV; }
static inline void irq_remapping_disable(void) { }
static inline int irq_remapping_reenable(int eim) { return -ENODEV; }
static inline int irq_remap_enable_fault_handling(void) { return -ENODEV; }

static inline void panic_if_irq_remap(const char *msg)
{
}

#endif /* CONFIG_IRQ_REMAP */
#endif /* __X86_IRQ_REMAPPING_H */
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
