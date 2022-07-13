<<<<<<< HEAD
=======
/* SPDX-License-Identifier: GPL-2.0 */
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
/*
 * Copyright (c) 2011 Samsung Electronics Co., Ltd.
 *		http://www.samsung.com
 *
 * Common Header for S5PV210 machines
<<<<<<< HEAD
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 */

#ifndef __ARCH_ARM_MACH_S5PV210_COMMON_H
#define __ARCH_ARM_MACH_S5PV210_COMMON_H

<<<<<<< HEAD
void s5pv210_init_io(struct map_desc *mach_desc, int size);
void s5pv210_init_irq(void);

void s5pv210_register_clocks(void);
void s5pv210_setup_clocks(void);

void s5pv210_restart(char mode, const char *cmd);

#ifdef CONFIG_CPU_S5PV210

extern  int s5pv210_init(void);
extern void s5pv210_map_io(void);
extern void s5pv210_init_clocks(int xtal);
extern void s5pv210_init_uarts(struct s3c2410_uartcfg *cfg, int no);

#else
#define s5pv210_init_clocks NULL
#define s5pv210_init_uarts NULL
#define s5pv210_map_io NULL
#define s5pv210_init NULL
=======
#ifdef CONFIG_PM_SLEEP
void s5pv210_cpu_resume(void);
void s5pv210_pm_init(void);
#else
static inline void s5pv210_pm_init(void) {}
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#endif

#endif /* __ARCH_ARM_MACH_S5PV210_COMMON_H */
