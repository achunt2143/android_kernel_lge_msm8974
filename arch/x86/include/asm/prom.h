<<<<<<< HEAD
=======
/* SPDX-License-Identifier: GPL-2.0-or-later */
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
/*
 * Definitions for Device tree / OpenFirmware handling on X86
 *
 * based on arch/powerpc/include/asm/prom.h which is
 *         Copyright (C) 1996-2005 Paul Mackerras.
<<<<<<< HEAD
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version
 * 2 of the License, or (at your option) any later version.
=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 */

#ifndef _ASM_X86_PROM_H
#define _ASM_X86_PROM_H
#ifndef __ASSEMBLY__

#include <linux/of.h>
#include <linux/types.h>
#include <linux/pci.h>

#include <asm/irq.h>
#include <linux/atomic.h>
#include <asm/setup.h>

#ifdef CONFIG_OF
extern int of_ioapic;
extern u64 initial_dtb;
extern void add_dtb(u64 data);
<<<<<<< HEAD
extern void x86_add_irq_domains(void);
void __cpuinit x86_of_pci_init(void);
void x86_dtb_init(void);
#else
static inline void add_dtb(u64 data) { }
static inline void x86_add_irq_domains(void) { }
static inline void x86_of_pci_init(void) { }
static inline void x86_dtb_init(void) { }
#define of_ioapic 0
#endif

extern char cmd_line[COMMAND_LINE_SIZE];

#define pci_address_to_pio pci_address_to_pio
unsigned long pci_address_to_pio(phys_addr_t addr);

#define HAVE_ARCH_DEVTREE_FIXUPS

=======
void x86_of_pci_init(void);
void x86_dtb_parse_smp_config(void);
#else
static inline void add_dtb(u64 data) { }
static inline void x86_of_pci_init(void) { }
static inline void x86_dtb_parse_smp_config(void) { }
#define of_ioapic 0
#endif

#ifdef CONFIG_OF_EARLY_FLATTREE
void x86_flattree_get_config(void);
#else
static inline void x86_flattree_get_config(void) { }
#endif
extern char cmd_line[COMMAND_LINE_SIZE];

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#endif /* __ASSEMBLY__ */
#endif
