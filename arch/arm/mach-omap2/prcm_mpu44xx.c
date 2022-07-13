<<<<<<< HEAD
=======
// SPDX-License-Identifier: GPL-2.0-only
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
/*
 * OMAP4 PRCM_MPU module functions
 *
 * Copyright (C) 2009 Nokia Corporation
 * Paul Walmsley
<<<<<<< HEAD
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 */

#include <linux/kernel.h>
#include <linux/types.h>
#include <linux/errno.h>
#include <linux/err.h>
#include <linux/io.h>

#include "iomap.h"
#include "common.h"
#include "prcm_mpu44xx.h"
#include "cm-regbits-44xx.h"

<<<<<<< HEAD
=======
/*
 * prcm_mpu_base: the virtual address of the start of the PRCM_MPU IP
 *   block registers
 */
struct omap_domain_base prcm_mpu_base;

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
/* PRCM_MPU low-level functions */

u32 omap4_prcm_mpu_read_inst_reg(s16 inst, u16 reg)
{
<<<<<<< HEAD
	return __raw_readl(OMAP44XX_PRCM_MPU_REGADDR(inst, reg));
=======
	return readl_relaxed(OMAP44XX_PRCM_MPU_REGADDR(inst, reg));
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

void omap4_prcm_mpu_write_inst_reg(u32 val, s16 inst, u16 reg)
{
<<<<<<< HEAD
	__raw_writel(val, OMAP44XX_PRCM_MPU_REGADDR(inst, reg));
}

u32 omap4_prcm_mpu_rmw_inst_reg_bits(u32 mask, u32 bits, s16 inst, s16 reg)
{
	u32 v;

	v = omap4_prcm_mpu_read_inst_reg(inst, reg);
	v &= ~mask;
	v |= bits;
	omap4_prcm_mpu_write_inst_reg(v, inst, reg);

	return v;
=======
	writel_relaxed(val, OMAP44XX_PRCM_MPU_REGADDR(inst, reg));
}

/**
 * omap2_set_globals_prcm_mpu - set the MPU PRCM base address (for early use)
 * @prcm_mpu: PRCM_MPU base virtual address
 *
 * XXX Will be replaced when the PRM/CM drivers are completed.
 */
void __init omap2_set_globals_prcm_mpu(void __iomem *prcm_mpu)
{
	prcm_mpu_base.va = prcm_mpu;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}
