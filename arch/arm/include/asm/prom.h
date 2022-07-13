<<<<<<< HEAD
=======
/* SPDX-License-Identifier: GPL-2.0-only */
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
/*
 *  arch/arm/include/asm/prom.h
 *
 *  Copyright (C) 2009 Canonical Ltd. <jeremy.kerr@canonical.com>
<<<<<<< HEAD
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 *
=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 */
#ifndef __ASMARM_PROM_H
#define __ASMARM_PROM_H

#ifdef CONFIG_OF

<<<<<<< HEAD
extern struct machine_desc *setup_machine_fdt(unsigned int dt_phys);
extern void arm_dt_memblock_reserve(void);

#else /* CONFIG_OF */

static inline struct machine_desc *setup_machine_fdt(unsigned int dt_phys)
=======
extern const struct machine_desc *setup_machine_fdt(void *dt_virt);
extern void __init arm_dt_init_cpu_maps(void);

#else /* CONFIG_OF */

static inline const struct machine_desc *setup_machine_fdt(void *dt_virt)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
{
	return NULL;
}

<<<<<<< HEAD
static inline void arm_dt_memblock_reserve(void) { }
=======
static inline void arm_dt_init_cpu_maps(void) { }
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

#endif /* CONFIG_OF */
#endif /* ASMARM_PROM_H */
