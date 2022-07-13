<<<<<<< HEAD
=======
// SPDX-License-Identifier: GPL-2.0
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
/*
 * SMP support for R-Mobile / SH-Mobile
 *
 * Copyright (C) 2010  Magnus Damm
 * Copyright (C) 2011  Paul Mundt
 *
 * Based on vexpress, Copyright (C) 2002 ARM Ltd, All Rights Reserved
<<<<<<< HEAD
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 */
#include <linux/init.h>
#include <linux/errno.h>
#include <linux/delay.h>
#include <linux/device.h>
#include <linux/smp.h>
#include <linux/io.h>
#include <asm/hardware/gic.h>
#include <asm/mach-types.h>
#include <mach/common.h>

#define is_sh73a0() (machine_is_ag5evm() || machine_is_kota2())
#define is_r8a7779() machine_is_marzen()

static unsigned int __init shmobile_smp_get_core_count(void)
{
	if (is_sh73a0())
		return sh73a0_get_core_count();

	if (is_r8a7779())
		return r8a7779_get_core_count();

	return 1;
}

static void __init shmobile_smp_prepare_cpus(void)
{
	if (is_sh73a0())
		sh73a0_smp_prepare_cpus();

	if (is_r8a7779())
		r8a7779_smp_prepare_cpus();
}

int shmobile_platform_cpu_kill(unsigned int cpu)
{
	if (is_r8a7779())
		return r8a7779_platform_cpu_kill(cpu);

	return 1;
}

void __cpuinit platform_secondary_init(unsigned int cpu)
{
	trace_hardirqs_off();

	if (is_sh73a0())
		sh73a0_secondary_init(cpu);

	if (is_r8a7779())
		r8a7779_secondary_init(cpu);
}

int __cpuinit boot_secondary(unsigned int cpu, struct task_struct *idle)
{
	if (is_sh73a0())
		return sh73a0_boot_secondary(cpu);

	if (is_r8a7779())
		return r8a7779_boot_secondary(cpu);

	return -ENOSYS;
}

void __init smp_init_cpus(void)
{
	unsigned int ncores = shmobile_smp_get_core_count();
	unsigned int i;

	if (ncores > nr_cpu_ids) {
		pr_warn("SMP: %u cores greater than maximum (%u), clipping\n",
			ncores, nr_cpu_ids);
		ncores = nr_cpu_ids;
	}

	for (i = 0; i < ncores; i++)
		set_cpu_possible(i, true);

	set_smp_cross_call(gic_raise_softirq);
}

void __init platform_smp_prepare_cpus(unsigned int max_cpus)
{
	shmobile_smp_prepare_cpus();
}
=======
 */
#include <linux/init.h>
#include <asm/cacheflush.h>
#include <asm/smp_plat.h>
#include "common.h"

extern unsigned long shmobile_smp_fn[];
extern unsigned long shmobile_smp_arg[];
extern unsigned long shmobile_smp_mpidr[];

void shmobile_smp_hook(unsigned int cpu, unsigned long fn, unsigned long arg)
{
	shmobile_smp_fn[cpu] = 0;
	flush_cache_all();

	shmobile_smp_mpidr[cpu] = cpu_logical_map(cpu);
	shmobile_smp_fn[cpu] = fn;
	shmobile_smp_arg[cpu] = arg;
	flush_cache_all();
}

#ifdef CONFIG_HOTPLUG_CPU
bool shmobile_smp_cpu_can_disable(unsigned int cpu)
{
	return true; /* Hotplug of any CPU is supported */
}
#endif
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
