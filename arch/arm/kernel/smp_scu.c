<<<<<<< HEAD
=======
// SPDX-License-Identifier: GPL-2.0-only
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
/*
 *  linux/arch/arm/kernel/smp_scu.c
 *
 *  Copyright (C) 2002 ARM Ltd.
 *  All Rights Reserved
<<<<<<< HEAD
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 */
#include <linux/init.h>
#include <linux/io.h>

<<<<<<< HEAD
=======
#include <asm/smp_plat.h>
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#include <asm/smp_scu.h>
#include <asm/cacheflush.h>
#include <asm/cputype.h>

#define SCU_CTRL		0x00
<<<<<<< HEAD
#define SCU_CONFIG		0x04
#define SCU_CPU_STATUS		0x08
=======
#define SCU_ENABLE		(1 << 0)
#define SCU_STANDBY_ENABLE	(1 << 5)
#define SCU_CONFIG		0x04
#define SCU_CPU_STATUS		0x08
#define SCU_CPU_STATUS_MASK	GENMASK(1, 0)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#define SCU_INVALIDATE		0x0c
#define SCU_FPGA_REVISION	0x10

#ifdef CONFIG_SMP
/*
 * Get the number of CPU cores from the SCU configuration
 */
unsigned int __init scu_get_core_count(void __iomem *scu_base)
{
<<<<<<< HEAD
	unsigned int ncores = __raw_readl(scu_base + SCU_CONFIG);
=======
	unsigned int ncores = readl_relaxed(scu_base + SCU_CONFIG);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	return (ncores & 0x03) + 1;
}

/*
 * Enable the SCU
 */
void scu_enable(void __iomem *scu_base)
{
	u32 scu_ctrl;

#ifdef CONFIG_ARM_ERRATA_764369
	/* Cortex-A9 only */
<<<<<<< HEAD
	if ((read_cpuid(CPUID_ID) & 0xff0ffff0) == 0x410fc090) {
		scu_ctrl = __raw_readl(scu_base + 0x30);
		if (!(scu_ctrl & 1))
			__raw_writel(scu_ctrl | 0x1, scu_base + 0x30);
	}
#endif

	scu_ctrl = __raw_readl(scu_base + SCU_CTRL);
	/* already enabled? */
	if (scu_ctrl & 1)
		return;

	scu_ctrl |= 1;
	__raw_writel(scu_ctrl, scu_base + SCU_CTRL);
=======
	if ((read_cpuid_id() & 0xff0ffff0) == 0x410fc090) {
		scu_ctrl = readl_relaxed(scu_base + 0x30);
		if (!(scu_ctrl & 1))
			writel_relaxed(scu_ctrl | 0x1, scu_base + 0x30);
	}
#endif

	scu_ctrl = readl_relaxed(scu_base + SCU_CTRL);
	/* already enabled? */
	if (scu_ctrl & SCU_ENABLE)
		return;

	scu_ctrl |= SCU_ENABLE;

	/* Cortex-A9 earlier than r2p0 has no standby bit in SCU */
	if ((read_cpuid_id() & 0xff0ffff0) == 0x410fc090 &&
	    (read_cpuid_id() & 0x00f0000f) >= 0x00200000)
		scu_ctrl |= SCU_STANDBY_ENABLE;

	writel_relaxed(scu_ctrl, scu_base + SCU_CTRL);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	/*
	 * Ensure that the data accessed by CPU0 before the SCU was
	 * initialised is visible to the other CPUs.
	 */
	flush_cache_all();
}
#endif

<<<<<<< HEAD
=======
static int scu_set_power_mode_internal(void __iomem *scu_base,
				       unsigned int logical_cpu,
				       unsigned int mode)
{
	unsigned int val;
	int cpu = MPIDR_AFFINITY_LEVEL(cpu_logical_map(logical_cpu), 0);

	if (mode > 3 || mode == 1 || cpu > 3)
		return -EINVAL;

	val = readb_relaxed(scu_base + SCU_CPU_STATUS + cpu);
	val &= ~SCU_CPU_STATUS_MASK;
	val |= mode;
	writeb_relaxed(val, scu_base + SCU_CPU_STATUS + cpu);

	return 0;
}

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
/*
 * Set the executing CPUs power mode as defined.  This will be in
 * preparation for it executing a WFI instruction.
 *
 * This function must be called with preemption disabled, and as it
 * has the side effect of disabling coherency, caches must have been
 * flushed.  Interrupts must also have been disabled.
 */
int scu_power_mode(void __iomem *scu_base, unsigned int mode)
{
<<<<<<< HEAD
	unsigned int val;
	int cpu = smp_processor_id();

	if (mode > 3 || mode == 1 || cpu > 3)
		return -EINVAL;

	val = __raw_readb(scu_base + SCU_CPU_STATUS + cpu) & ~0x03;
	val |= mode;
	__raw_writeb(val, scu_base + SCU_CPU_STATUS + cpu);

	return 0;
=======
	return scu_set_power_mode_internal(scu_base, smp_processor_id(), mode);
}

/*
 * Set the given (logical) CPU's power mode to SCU_PM_NORMAL.
 */
int scu_cpu_power_enable(void __iomem *scu_base, unsigned int cpu)
{
	return scu_set_power_mode_internal(scu_base, cpu, SCU_PM_NORMAL);
}

int scu_get_cpu_power_mode(void __iomem *scu_base, unsigned int logical_cpu)
{
	unsigned int val;
	int cpu = MPIDR_AFFINITY_LEVEL(cpu_logical_map(logical_cpu), 0);

	if (cpu > 3)
		return -EINVAL;

	val = readb_relaxed(scu_base + SCU_CPU_STATUS + cpu);
	val &= SCU_CPU_STATUS_MASK;

	return val;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}
