<<<<<<< HEAD
=======
/* SPDX-License-Identifier: GPL-2.0-only */
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
/*
 * arch/arm/mach-tegra/reset.h
 *
 * CPU reset dispatcher.
 *
 * Copyright (c) 2011, NVIDIA Corporation.
<<<<<<< HEAD
 *
 * This software is licensed under the terms of the GNU General Public
 * License version 2, as published by the Free Software Foundation, and
 * may be copied, distributed, and modified under those terms.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 */

#ifndef __MACH_TEGRA_RESET_H
#define __MACH_TEGRA_RESET_H

#define TEGRA_RESET_MASK_PRESENT	0
#define TEGRA_RESET_MASK_LP1		1
#define TEGRA_RESET_MASK_LP2		2
#define TEGRA_RESET_STARTUP_SECONDARY	3
#define TEGRA_RESET_STARTUP_LP2		4
#define TEGRA_RESET_STARTUP_LP1		5
<<<<<<< HEAD
#define TEGRA_RESET_DATA_SIZE		6

#ifndef __ASSEMBLY__

=======
#define TEGRA_RESET_TF_PRESENT		6
#define TEGRA_RESET_DATA_SIZE		7

#define RESET_DATA(x)	((TEGRA_RESET_##x)*4)

#ifndef __ASSEMBLY__

#include "irammap.h"

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
extern unsigned long __tegra_cpu_reset_handler_data[TEGRA_RESET_DATA_SIZE];

void __tegra_cpu_reset_handler_start(void);
void __tegra_cpu_reset_handler(void);
<<<<<<< HEAD
void __tegra_cpu_reset_handler_end(void);
void tegra_secondary_startup(void);
=======
void __tegra20_cpu1_resettable_status_offset(void);
void __tegra_cpu_reset_handler_end(void);

#ifdef CONFIG_PM_SLEEP
#define tegra_cpu_lp1_mask \
	(IO_ADDRESS(TEGRA_IRAM_BASE + TEGRA_IRAM_RESET_HANDLER_OFFSET + \
	((u32)&__tegra_cpu_reset_handler_data[TEGRA_RESET_MASK_LP1] - \
	 (u32)__tegra_cpu_reset_handler_start)))
#define tegra_cpu_lp2_mask \
	(IO_ADDRESS(TEGRA_IRAM_BASE + TEGRA_IRAM_RESET_HANDLER_OFFSET + \
	((u32)&__tegra_cpu_reset_handler_data[TEGRA_RESET_MASK_LP2] - \
	 (u32)__tegra_cpu_reset_handler_start)))
#endif
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

#define tegra_cpu_reset_handler_offset \
		((u32)__tegra_cpu_reset_handler - \
		 (u32)__tegra_cpu_reset_handler_start)

#define tegra_cpu_reset_handler_size \
		(__tegra_cpu_reset_handler_end - \
		 __tegra_cpu_reset_handler_start)

void __init tegra_cpu_reset_handler_init(void);

#endif
#endif
