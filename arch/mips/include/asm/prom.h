<<<<<<< HEAD
=======
/* SPDX-License-Identifier: GPL-2.0-only */
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
/*
 *  arch/mips/include/asm/prom.h
 *
 *  Copyright (C) 2010 Cisco Systems Inc. <dediao@cisco.com>
<<<<<<< HEAD
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 *
=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 */
#ifndef __ASM_PROM_H
#define __ASM_PROM_H

<<<<<<< HEAD
#ifdef CONFIG_OF
#include <asm/bootinfo.h>

extern int early_init_dt_scan_memory_arch(unsigned long node,
	const char *uname, int depth, void *data);

extern int reserve_mem_mach(unsigned long addr, unsigned long size);
extern void free_mem_mach(unsigned long addr, unsigned long size);

extern void device_tree_init(void);
#else /* CONFIG_OF */
static inline void device_tree_init(void) { }
#endif /* CONFIG_OF */
=======
#ifdef CONFIG_USE_OF
#include <linux/bug.h>
#include <linux/io.h>
#include <linux/types.h>
#include <asm/bootinfo.h>

extern void device_tree_init(void);

struct boot_param_header;

extern void __dt_setup_arch(void *bph);
extern int __dt_register_buses(const char *bus0, const char *bus1);

#else /* !CONFIG_USE_OF */
static inline void device_tree_init(void) { }
#endif /* !CONFIG_USE_OF */

extern char *mips_get_machine_name(void);
extern void mips_set_machine_name(const char *name);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

#endif /* __ASM_PROM_H */
