<<<<<<< HEAD
/*
 * Copyright 2008-2010 Freescale Semiconductor, Inc. All Rights Reserved.
 *
 * The code contained herein is licensed under the GNU General Public
 * License. You may obtain a copy of the GNU General Public License
 * Version 2 or later at the following locations:
 *
 * http://www.opensource.org/licenses/gpl-license.html
 * http://www.gnu.org/copyleft/gpl.html
 *
=======
// SPDX-License-Identifier: GPL-2.0-or-later
/*
 * Copyright 2008-2010 Freescale Semiconductor, Inc. All Rights Reserved.
 *
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 * This file contains the CPU initialization code.
 */

#include <linux/types.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/module.h>
<<<<<<< HEAD
#include <mach/hardware.h>
#include <linux/io.h>
=======
#include <linux/io.h>
#include <linux/of.h>
#include <linux/of_address.h>

#include "hardware.h"
#include "common.h"
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

static int mx5_cpu_rev = -1;

#define IIM_SREV 0x24
<<<<<<< HEAD
#define MX50_HW_ADADIG_DIGPROG	0xB0

static int get_mx51_srev(void)
{
	void __iomem *iim_base = MX51_IO_ADDRESS(MX51_IIM_BASE_ADDR);
	u32 rev = readl(iim_base + IIM_SREV) & 0xff;
=======

static u32 imx5_read_srev_reg(const char *compat)
{
	void __iomem *iim_base;
	struct device_node *np;
	u32 srev;

	np = of_find_compatible_node(NULL, NULL, compat);
	iim_base = of_iomap(np, 0);
	of_node_put(np);
	WARN_ON(!iim_base);

	srev = readl(iim_base + IIM_SREV) & 0xff;

	iounmap(iim_base);

	return srev;
}

static int get_mx51_srev(void)
{
	u32 rev = imx5_read_srev_reg("fsl,imx51-iim");
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	switch (rev) {
	case 0x0:
		return IMX_CHIP_REVISION_2_0;
	case 0x10:
		return IMX_CHIP_REVISION_3_0;
	default:
		return IMX_CHIP_REVISION_UNKNOWN;
	}
}

/*
 * Returns:
 *	the silicon revision of the cpu
<<<<<<< HEAD
 *	-EINVAL - not a mx51
 */
int mx51_revision(void)
{
	if (!cpu_is_mx51())
		return -EINVAL;

=======
 */
int mx51_revision(void)
{
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	if (mx5_cpu_rev == -1)
		mx5_cpu_rev = get_mx51_srev();

	return mx5_cpu_rev;
}
EXPORT_SYMBOL(mx51_revision);

#ifdef CONFIG_NEON

/*
 * All versions of the silicon before Rev. 3 have broken NEON implementations.
 * Dependent on link order - so the assumption is that vfp_init is called
 * before us.
 */
<<<<<<< HEAD
static int __init mx51_neon_fixup(void)
{
	if (!cpu_is_mx51())
		return 0;

=======
int __init mx51_neon_fixup(void)
{
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	if (mx51_revision() < IMX_CHIP_REVISION_3_0 &&
			(elf_hwcap & HWCAP_NEON)) {
		elf_hwcap &= ~HWCAP_NEON;
		pr_info("Turning off NEON support, detected broken NEON implementation\n");
	}
	return 0;
}

<<<<<<< HEAD
late_initcall(mx51_neon_fixup);
=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#endif

static int get_mx53_srev(void)
{
<<<<<<< HEAD
	void __iomem *iim_base = MX51_IO_ADDRESS(MX53_IIM_BASE_ADDR);
	u32 rev = readl(iim_base + IIM_SREV) & 0xff;
=======
	u32 rev = imx5_read_srev_reg("fsl,imx53-iim");
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	switch (rev) {
	case 0x0:
		return IMX_CHIP_REVISION_1_0;
	case 0x2:
		return IMX_CHIP_REVISION_2_0;
	case 0x3:
		return IMX_CHIP_REVISION_2_1;
	default:
		return IMX_CHIP_REVISION_UNKNOWN;
	}
}

/*
 * Returns:
 *	the silicon revision of the cpu
<<<<<<< HEAD
 *	-EINVAL - not a mx53
 */
int mx53_revision(void)
{
	if (!cpu_is_mx53())
		return -EINVAL;

=======
 */
int mx53_revision(void)
{
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	if (mx5_cpu_rev == -1)
		mx5_cpu_rev = get_mx53_srev();

	return mx5_cpu_rev;
}
EXPORT_SYMBOL(mx53_revision);

<<<<<<< HEAD
static int get_mx50_srev(void)
{
	void __iomem *anatop = ioremap(MX50_ANATOP_BASE_ADDR, SZ_8K);
	u32 rev;

	if (!anatop) {
		mx5_cpu_rev = -EINVAL;
		return 0;
	}

	rev = readl(anatop + MX50_HW_ADADIG_DIGPROG);
	rev &= 0xff;

	iounmap(anatop);
	if (rev == 0x0)
		return IMX_CHIP_REVISION_1_0;
	else if (rev == 0x1)
		return IMX_CHIP_REVISION_1_1;
	return 0;
}

/*
 * Returns:
 *	the silicon revision of the cpu
 *	-EINVAL - not a mx50
 */
int mx50_revision(void)
{
	if (!cpu_is_mx50())
		return -EINVAL;

	if (mx5_cpu_rev == -1)
		mx5_cpu_rev = get_mx50_srev();

	return mx5_cpu_rev;
}
EXPORT_SYMBOL(mx50_revision);
=======
#define ARM_GPC		0x4
#define DBGEN		BIT(16)

/*
 * This enables the DBGEN bit in ARM_GPC register, which is
 * required for accessing some performance counter features.
 * Technically it is only required while perf is used, but to
 * keep the source code simple we just enable it all the time
 * when the kernel configuration allows using the feature.
 */
void __init imx5_pmu_init(void)
{
	void __iomem *tigerp_base;
	struct device_node *np;
	u32 gpc;

	if (!IS_ENABLED(CONFIG_ARM_PMU))
		return;

	np = of_find_compatible_node(NULL, NULL, "arm,cortex-a8-pmu");
	if (!np)
		return;

	if (!of_property_read_bool(np, "secure-reg-access"))
		goto exit;

	of_node_put(np);

	np = of_find_compatible_node(NULL, NULL, "fsl,imx51-tigerp");
	if (!np)
		return;

	tigerp_base = of_iomap(np, 0);
	if (!tigerp_base)
		goto exit;

	gpc = readl_relaxed(tigerp_base + ARM_GPC);
	gpc |= DBGEN;
	writel_relaxed(gpc, tigerp_base + ARM_GPC);
	iounmap(tigerp_base);
exit:
	of_node_put(np);

}
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
