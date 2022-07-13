<<<<<<< HEAD
=======
// SPDX-License-Identifier: GPL-2.0-only
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
/*
 * OMAP4 PRM instance functions
 *
 * Copyright (C) 2009 Nokia Corporation
 * Copyright (C) 2011 Texas Instruments, Inc.
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
<<<<<<< HEAD
#include "prm44xx.h"
#include "prminst44xx.h"
#include "prm-regbits-44xx.h"
#include "prcm44xx.h"
#include "prcm_mpu44xx.h"

static u32 _prm_bases[OMAP4_MAX_PRCM_PARTITIONS] = {
	[OMAP4430_INVALID_PRCM_PARTITION]	= 0,
	[OMAP4430_PRM_PARTITION]		= OMAP4430_PRM_BASE,
	[OMAP4430_CM1_PARTITION]		= 0,
	[OMAP4430_CM2_PARTITION]		= 0,
	[OMAP4430_SCRM_PARTITION]		= 0,
	[OMAP4430_PRCM_MPU_PARTITION]		= OMAP4430_PRCM_MPU_BASE,
};
=======
#include "prcm-common.h"
#include "prm44xx.h"
#include "prm54xx.h"
#include "prm7xx.h"
#include "prminst44xx.h"
#include "prm-regbits-44xx.h"
#include "prcm44xx.h"
#include "prcm43xx.h"
#include "prcm_mpu44xx.h"
#include "soc.h"

static struct omap_domain_base _prm_bases[OMAP4_MAX_PRCM_PARTITIONS];

static s32 prm_dev_inst = PRM_INSTANCE_UNKNOWN;

/**
 * omap_prm_base_init - Populates the prm partitions
 *
 * Populates the base addresses of the _prm_bases
 * array used for read/write of prm module registers.
 */
void omap_prm_base_init(void)
{
	memcpy(&_prm_bases[OMAP4430_PRM_PARTITION], &prm_base,
	       sizeof(prm_base));
	memcpy(&_prm_bases[OMAP4430_PRCM_MPU_PARTITION], &prcm_mpu_base,
	       sizeof(prcm_mpu_base));
}

s32 omap4_prmst_get_prm_dev_inst(void)
{
	return prm_dev_inst;
}

void omap4_prminst_set_prm_dev_inst(s32 dev_inst)
{
	prm_dev_inst = dev_inst;
}
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

/* Read a register in a PRM instance */
u32 omap4_prminst_read_inst_reg(u8 part, s16 inst, u16 idx)
{
	BUG_ON(part >= OMAP4_MAX_PRCM_PARTITIONS ||
	       part == OMAP4430_INVALID_PRCM_PARTITION ||
<<<<<<< HEAD
	       !_prm_bases[part]);
	return __raw_readl(OMAP2_L4_IO_ADDRESS(_prm_bases[part] + inst +
					       idx));
=======
	       !_prm_bases[part].va);
	return readl_relaxed(_prm_bases[part].va + inst + idx);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

/* Write into a register in a PRM instance */
void omap4_prminst_write_inst_reg(u32 val, u8 part, s16 inst, u16 idx)
{
	BUG_ON(part >= OMAP4_MAX_PRCM_PARTITIONS ||
	       part == OMAP4430_INVALID_PRCM_PARTITION ||
<<<<<<< HEAD
	       !_prm_bases[part]);
	__raw_writel(val, OMAP2_L4_IO_ADDRESS(_prm_bases[part] + inst + idx));
=======
	       !_prm_bases[part].va);
	writel_relaxed(val, _prm_bases[part].va + inst + idx);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

/* Read-modify-write a register in PRM. Caller must lock */
u32 omap4_prminst_rmw_inst_reg_bits(u32 mask, u32 bits, u8 part, s16 inst,
				    u16 idx)
{
	u32 v;

	v = omap4_prminst_read_inst_reg(part, inst, idx);
	v &= ~mask;
	v |= bits;
	omap4_prminst_write_inst_reg(v, part, inst, idx);

	return v;
}

<<<<<<< HEAD
/*
 * Address offset (in bytes) between the reset control and the reset
 * status registers: 4 bytes on OMAP4
 */
#define OMAP4_RST_CTRL_ST_OFFSET		4

=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
/**
 * omap4_prminst_is_hardreset_asserted - read the HW reset line state of
 * submodules contained in the hwmod module
 * @rstctrl_reg: RM_RSTCTRL register address for this module
 * @shift: register bit shift corresponding to the reset line to check
 *
 * Returns 1 if the (sub)module hardreset line is currently asserted,
 * 0 if the (sub)module hardreset line is not currently asserted, or
 * -EINVAL upon parameter error.
 */
int omap4_prminst_is_hardreset_asserted(u8 shift, u8 part, s16 inst,
					u16 rstctrl_offs)
{
	u32 v;

	v = omap4_prminst_read_inst_reg(part, inst, rstctrl_offs);
	v &= 1 << shift;
	v >>= shift;

	return v;
}

/**
 * omap4_prminst_assert_hardreset - assert the HW reset line of a submodule
 * @rstctrl_reg: RM_RSTCTRL register address for this module
 * @shift: register bit shift corresponding to the reset line to assert
 *
 * Some IPs like dsp, ipu or iva contain processors that require an HW
 * reset line to be asserted / deasserted in order to fully enable the
 * IP.  These modules may have multiple hard-reset lines that reset
 * different 'submodules' inside the IP block.  This function will
 * place the submodule into reset.  Returns 0 upon success or -EINVAL
 * upon an argument error.
 */
int omap4_prminst_assert_hardreset(u8 shift, u8 part, s16 inst,
				   u16 rstctrl_offs)
{
	u32 mask = 1 << shift;

	omap4_prminst_rmw_inst_reg_bits(mask, mask, part, inst, rstctrl_offs);

	return 0;
}

/**
 * omap4_prminst_deassert_hardreset - deassert a submodule hardreset line and
 * wait
<<<<<<< HEAD
 * @rstctrl_reg: RM_RSTCTRL register address for this module
 * @shift: register bit shift corresponding to the reset line to deassert
=======
 * @shift: register bit shift corresponding to the reset line to deassert
 * @st_shift: status bit offset corresponding to the reset line
 * @part: PRM partition
 * @inst: PRM instance offset
 * @rstctrl_offs: reset register offset
 * @rstst_offs: reset status register offset
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 *
 * Some IPs like dsp, ipu or iva contain processors that require an HW
 * reset line to be asserted / deasserted in order to fully enable the
 * IP.  These modules may have multiple hard-reset lines that reset
 * different 'submodules' inside the IP block.  This function will
 * take the submodule out of reset and wait until the PRCM indicates
 * that the reset has completed before returning.  Returns 0 upon success or
 * -EINVAL upon an argument error, -EEXIST if the submodule was already out
 * of reset, or -EBUSY if the submodule did not exit reset promptly.
 */
<<<<<<< HEAD
int omap4_prminst_deassert_hardreset(u8 shift, u8 part, s16 inst,
				     u16 rstctrl_offs)
{
	int c;
	u32 mask = 1 << shift;
	u16 rstst_offs = rstctrl_offs + OMAP4_RST_CTRL_ST_OFFSET;
=======
int omap4_prminst_deassert_hardreset(u8 shift, u8 st_shift, u8 part, s16 inst,
				     u16 rstctrl_offs, u16 rstst_offs)
{
	int c;
	u32 mask = 1 << shift;
	u32 st_mask = 1 << st_shift;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	/* Check the current status to avoid de-asserting the line twice */
	if (omap4_prminst_is_hardreset_asserted(shift, part, inst,
						rstctrl_offs) == 0)
		return -EEXIST;

	/* Clear the reset status by writing 1 to the status bit */
<<<<<<< HEAD
	omap4_prminst_rmw_inst_reg_bits(0xffffffff, mask, part, inst,
=======
	omap4_prminst_rmw_inst_reg_bits(0xffffffff, st_mask, part, inst,
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
					rstst_offs);
	/* de-assert the reset control line */
	omap4_prminst_rmw_inst_reg_bits(mask, 0, part, inst, rstctrl_offs);
	/* wait the status to be set */
<<<<<<< HEAD
	omap_test_timeout(omap4_prminst_is_hardreset_asserted(shift, part, inst,
							      rstst_offs),
=======
	omap_test_timeout(omap4_prminst_is_hardreset_asserted(st_shift, part,
							      inst, rstst_offs),
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
			  MAX_MODULE_HARDRESET_WAIT, c);

	return (c == MAX_MODULE_HARDRESET_WAIT) ? -EBUSY : 0;
}


void omap4_prminst_global_warm_sw_reset(void)
{
	u32 v;
<<<<<<< HEAD

	v = omap4_prminst_read_inst_reg(OMAP4430_PRM_PARTITION,
				    OMAP4430_PRM_DEVICE_INST,
				    OMAP4_PRM_RSTCTRL_OFFSET);
	v |= OMAP4430_RST_GLOBAL_WARM_SW_MASK;
	omap4_prminst_write_inst_reg(v, OMAP4430_PRM_PARTITION,
				 OMAP4430_PRM_DEVICE_INST,
				 OMAP4_PRM_RSTCTRL_OFFSET);

	/* OCP barrier */
	v = omap4_prminst_read_inst_reg(OMAP4430_PRM_PARTITION,
				    OMAP4430_PRM_DEVICE_INST,
				    OMAP4_PRM_RSTCTRL_OFFSET);
=======
	s32 inst = omap4_prmst_get_prm_dev_inst();

	if (inst == PRM_INSTANCE_UNKNOWN)
		return;

	v = omap4_prminst_read_inst_reg(OMAP4430_PRM_PARTITION, inst,
					OMAP4_PRM_RSTCTRL_OFFSET);
	v |= OMAP4430_RST_GLOBAL_WARM_SW_MASK;
	omap4_prminst_write_inst_reg(v, OMAP4430_PRM_PARTITION,
				 inst, OMAP4_PRM_RSTCTRL_OFFSET);

	/* OCP barrier */
	v = omap4_prminst_read_inst_reg(OMAP4430_PRM_PARTITION,
				    inst, OMAP4_PRM_RSTCTRL_OFFSET);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}
