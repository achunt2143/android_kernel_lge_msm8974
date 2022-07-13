<<<<<<< HEAD
=======
// SPDX-License-Identifier: GPL-2.0-only
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
/*
 * Utility to set the DAVINCI MUX register from a table in mux.h
 *
 * Author: Vladimir Barinov, MontaVista Software, Inc. <source@mvista.com>
 *
 * Based on linux/arch/arm/plat-omap/mux.c:
 * Copyright (C) 2003 - 2005 Nokia Corporation
 *
 * Written by Tony Lindgren
 *
<<<<<<< HEAD
 * 2007 (c) MontaVista Software, Inc. This file is licensed under
 * the terms of the GNU General Public License version 2. This program
 * is licensed "as is" without any warranty of any kind, whether express
 * or implied.
 *
 * Copyright (C) 2008 Texas Instruments.
 */
=======
 * 2007 (c) MontaVista Software, Inc.
 *
 * Copyright (C) 2008 Texas Instruments.
 */

#define pr_fmt(fmt) KBUILD_MODNAME ": " fmt

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#include <linux/io.h>
#include <linux/module.h>
#include <linux/spinlock.h>

<<<<<<< HEAD
#include <mach/mux.h>
#include <mach/common.h>
=======
#include "mux.h"
#include "common.h"
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

static void __iomem *pinmux_base;

/*
 * Sets the DAVINCI MUX register based on the table
 */
<<<<<<< HEAD
int __init_or_module davinci_cfg_reg(const unsigned long index)
=======
int davinci_cfg_reg(const unsigned long index)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
{
	static DEFINE_SPINLOCK(mux_spin_lock);
	struct davinci_soc_info *soc_info = &davinci_soc_info;
	unsigned long flags;
	const struct mux_config *cfg;
	unsigned int reg_orig = 0, reg = 0;
	unsigned int mask, warn = 0;

	if (WARN_ON(!soc_info->pinmux_pins))
		return -ENODEV;

	if (!pinmux_base) {
		pinmux_base = ioremap(soc_info->pinmux_base, SZ_4K);
		if (WARN_ON(!pinmux_base))
			return -ENOMEM;
	}

	if (index >= soc_info->pinmux_pins_num) {
<<<<<<< HEAD
		printk(KERN_ERR "Invalid pin mux index: %lu (%lu)\n",
=======
		pr_err("Invalid pin mux index: %lu (%lu)\n",
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		       index, soc_info->pinmux_pins_num);
		dump_stack();
		return -ENODEV;
	}

	cfg = &soc_info->pinmux_pins[index];

	if (cfg->name == NULL) {
<<<<<<< HEAD
		printk(KERN_ERR "No entry for the specified index\n");
=======
		pr_err("No entry for the specified index\n");
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		return -ENODEV;
	}

	/* Update the mux register in question */
	if (cfg->mask) {
		unsigned	tmp1, tmp2;

		spin_lock_irqsave(&mux_spin_lock, flags);
		reg_orig = __raw_readl(pinmux_base + cfg->mux_reg);

		mask = (cfg->mask << cfg->mask_offset);
		tmp1 = reg_orig & mask;
		reg = reg_orig & ~mask;

		tmp2 = (cfg->mode << cfg->mask_offset);
		reg |= tmp2;

		if (tmp1 != tmp2)
			warn = 1;

		__raw_writel(reg, pinmux_base + cfg->mux_reg);
		spin_unlock_irqrestore(&mux_spin_lock, flags);
	}

	if (warn) {
#ifdef CONFIG_DAVINCI_MUX_WARNINGS
<<<<<<< HEAD
		printk(KERN_WARNING "MUX: initialized %s\n", cfg->name);
=======
		pr_warn("initialized %s\n", cfg->name);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#endif
	}

#ifdef CONFIG_DAVINCI_MUX_DEBUG
	if (cfg->debug || warn) {
<<<<<<< HEAD
		printk(KERN_WARNING "MUX: Setting register %s\n", cfg->name);
		printk(KERN_WARNING "	   %s (0x%08x) = 0x%08x -> 0x%08x\n",
		       cfg->mux_reg_name, cfg->mux_reg, reg_orig, reg);
=======
		pr_warn("Setting register %s\n", cfg->name);
		pr_warn("   %s (0x%08x) = 0x%08x -> 0x%08x\n",
			cfg->mux_reg_name, cfg->mux_reg, reg_orig, reg);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	}
#endif

	return 0;
}
<<<<<<< HEAD
EXPORT_SYMBOL(davinci_cfg_reg);

int __init_or_module davinci_cfg_reg_list(const short pins[])
{
	int i, error = -EINVAL;

	if (pins)
		for (i = 0; pins[i] >= 0; i++) {
			error = davinci_cfg_reg(pins[i]);
			if (error)
				break;
		}

	return error;
}
=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
