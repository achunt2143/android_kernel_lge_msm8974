<<<<<<< HEAD
/*
 * arch/arm/mach-tegra/board.h
 *
=======
/* SPDX-License-Identifier: GPL-2.0-only */
/*
 * arch/arm/mach-tegra/board.h
 *
 * Copyright (c) 2013 NVIDIA Corporation. All rights reserved.
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 * Copyright (C) 2010 Google, Inc.
 *
 * Author:
 *	Colin Cross <ccross@google.com>
 *	Erik Gilling <konkers@google.com>
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

#ifndef __MACH_TEGRA_BOARD_H
#define __MACH_TEGRA_BOARD_H

#include <linux/types.h>
<<<<<<< HEAD

void tegra_assert_system_reset(char mode, const char *cmd);

void __init tegra20_init_early(void);
void __init tegra30_init_early(void);
void __init tegra_map_common_io(void);
void __init tegra_init_irq(void);
void __init tegra_dt_init_irq(void);
int __init tegra_pcie_init(bool init_port0, bool init_port1);

extern struct sys_timer tegra_timer;
=======
#include <linux/reboot.h>

void __init tegra_map_common_io(void);
void __init tegra_init_irq(void);

void __init tegra_paz00_wifikill_init(void);

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#endif
