<<<<<<< HEAD
=======
// SPDX-License-Identifier: GPL-2.0-only
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
/*
 * arch/arm/mach-tegra/reset.c
 *
 * Copyright (C) 2011,2012 NVIDIA Corporation.
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
 */

#include <linux/init.h>
#include <linux/io.h>
#include <linux/cpumask.h>
#include <linux/bitops.h>

#include <asm/cacheflush.h>
#include <asm/hardware/cache-l2x0.h>

#include <mach/iomap.h>
#include <mach/irammap.h>

#include "reset.h"
#include "fuse.h"
=======
 */

#include <linux/bitops.h>
#include <linux/cpumask.h>
#include <linux/init.h>
#include <linux/io.h>

#include <linux/firmware/trusted_foundations.h>

#include <soc/tegra/fuse.h>

#include <asm/cacheflush.h>
#include <asm/firmware.h>
#include <asm/hardware/cache-l2x0.h>

#include "iomap.h"
#include "irammap.h"
#include "reset.h"
#include "sleep.h"
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

#define TEGRA_IRAM_RESET_BASE (TEGRA_IRAM_BASE + \
				TEGRA_IRAM_RESET_HANDLER_OFFSET)

static bool is_enabled;

<<<<<<< HEAD
static void __init tegra_cpu_reset_handler_enable(void)
{
	void __iomem *iram_base = IO_ADDRESS(TEGRA_IRAM_RESET_BASE);
=======
static void __init tegra_cpu_reset_handler_set(const u32 reset_address)
{
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	void __iomem *evp_cpu_reset =
		IO_ADDRESS(TEGRA_EXCEPTION_VECTORS_BASE + 0x100);
	void __iomem *sb_ctrl = IO_ADDRESS(TEGRA_SB_BASE);
	u32 reg;

<<<<<<< HEAD
	BUG_ON(is_enabled);
	BUG_ON(tegra_cpu_reset_handler_size > TEGRA_IRAM_RESET_HANDLER_SIZE);

	memcpy(iram_base, (void *)__tegra_cpu_reset_handler_start,
			tegra_cpu_reset_handler_size);

=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	/*
	 * NOTE: This must be the one and only write to the EVP CPU reset
	 *       vector in the entire system.
	 */
<<<<<<< HEAD
	writel(TEGRA_IRAM_RESET_BASE + tegra_cpu_reset_handler_offset,
			evp_cpu_reset);
=======
	writel(reset_address, evp_cpu_reset);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	wmb();
	reg = readl(evp_cpu_reset);

	/*
	 * Prevent further modifications to the physical reset vector.
	 *  NOTE: Has no effect on chips prior to Tegra30.
	 */
<<<<<<< HEAD
	if (tegra_chip_id != TEGRA20) {
		reg = readl(sb_ctrl);
		reg |= 2;
		writel(reg, sb_ctrl);
		wmb();
	}

	is_enabled = true;
=======
	reg = readl(sb_ctrl);
	reg |= 2;
	writel(reg, sb_ctrl);
	wmb();
}

static void __init tegra_cpu_reset_handler_enable(void)
{
	void __iomem *iram_base = IO_ADDRESS(TEGRA_IRAM_RESET_BASE);
	const u32 reset_address = TEGRA_IRAM_RESET_BASE +
						tegra_cpu_reset_handler_offset;
	int err;

	BUG_ON(is_enabled);
	BUG_ON(tegra_cpu_reset_handler_size > TEGRA_IRAM_RESET_HANDLER_SIZE);

	memcpy(iram_base, (void *)__tegra_cpu_reset_handler_start,
			tegra_cpu_reset_handler_size);

	err = call_firmware_op(set_cpu_boot_addr, 0, reset_address);
	switch (err) {
	case -ENOSYS:
		tegra_cpu_reset_handler_set(reset_address);
		fallthrough;
	case 0:
		is_enabled = true;
		break;
	default:
		pr_crit("Cannot set CPU reset handler: %d\n", err);
		BUG();
	}
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

void __init tegra_cpu_reset_handler_init(void)
{
<<<<<<< HEAD

#ifdef CONFIG_SMP
	__tegra_cpu_reset_handler_data[TEGRA_RESET_MASK_PRESENT] =
		*((u32 *)cpu_present_mask);
	__tegra_cpu_reset_handler_data[TEGRA_RESET_STARTUP_SECONDARY] =
		virt_to_phys((void *)tegra_secondary_startup);
=======
	__tegra_cpu_reset_handler_data[TEGRA_RESET_TF_PRESENT] =
		trusted_foundations_registered();

#ifdef CONFIG_SMP
	__tegra_cpu_reset_handler_data[TEGRA_RESET_MASK_PRESENT] =
		*((u32 *)cpu_possible_mask);
	__tegra_cpu_reset_handler_data[TEGRA_RESET_STARTUP_SECONDARY] =
		__pa_symbol((void *)secondary_startup);
#endif

#ifdef CONFIG_PM_SLEEP
	__tegra_cpu_reset_handler_data[TEGRA_RESET_STARTUP_LP1] =
		TEGRA_IRAM_LPx_RESUME_AREA;
	__tegra_cpu_reset_handler_data[TEGRA_RESET_STARTUP_LP2] =
		__pa_symbol((void *)tegra_resume);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#endif

	tegra_cpu_reset_handler_enable();
}
