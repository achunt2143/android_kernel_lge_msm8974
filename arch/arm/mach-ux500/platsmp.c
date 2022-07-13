<<<<<<< HEAD
=======
// SPDX-License-Identifier: GPL-2.0-only
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
/*
 * Copyright (C) 2002 ARM Ltd.
 * Copyright (C) 2008 STMicroelctronics.
 * Copyright (C) 2009 ST-Ericsson.
 * Author: Srinidhi Kasagar <srinidhi.kasagar@stericsson.com>
 *
 * This file is based on arm realview platform
<<<<<<< HEAD
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 */
#include <linux/init.h>
#include <linux/errno.h>
#include <linux/delay.h>
#include <linux/device.h>
#include <linux/smp.h>
#include <linux/io.h>
<<<<<<< HEAD

#include <asm/cacheflush.h>
#include <asm/hardware/gic.h>
#include <asm/smp_plat.h>
#include <asm/smp_scu.h>
#include <mach/hardware.h>
#include <mach/setup.h>

/* This is called from headsmp.S to wakeup the secondary core */
extern void u8500_secondary_startup(void);

/*
 * control for which core is the next to come out of the secondary
 * boot "holding pen"
 */
volatile int pen_release = -1;

/*
 * Write pen_release in a way that is guaranteed to be visible to all
 * observers, irrespective of whether they're taking part in coherency
 * or not.  This is necessary for the hotplug code to work reliably.
 */
static void write_pen_release(int val)
{
	pen_release = val;
	smp_wmb();
	__cpuc_flush_dcache_area((void *)&pen_release, sizeof(pen_release));
	outer_clean_range(__pa(&pen_release), __pa(&pen_release + 1));
}

static void __iomem *scu_base_addr(void)
{
	if (cpu_is_u5500())
		return __io_address(U5500_SCU_BASE);
	else if (cpu_is_u8500())
		return __io_address(U8500_SCU_BASE);
	else
		ux500_unknown_soc();

	return NULL;
}

static DEFINE_SPINLOCK(boot_lock);

void __cpuinit platform_secondary_init(unsigned int cpu)
{
	/*
	 * if any interrupts are already enabled for the primary
	 * core (e.g. timer irq), then they will not have been enabled
	 * for us: do so
	 */
	gic_secondary_init(0);

	/*
	 * let the primary processor know we're out of the
	 * pen, then head off into the C entry point
	 */
	write_pen_release(-1);

	/*
	 * Synchronise with the boot thread.
	 */
	spin_lock(&boot_lock);
	spin_unlock(&boot_lock);
}

int __cpuinit boot_secondary(unsigned int cpu, struct task_struct *idle)
{
	unsigned long timeout;

	/*
	 * set synchronisation state between this boot processor
	 * and the secondary one
	 */
	spin_lock(&boot_lock);

	/*
	 * The secondary processor is waiting to be released from
	 * the holding pen - release it, then wait for it to flag
	 * that it has been released by resetting pen_release.
	 */
	write_pen_release(cpu_logical_map(cpu));

	smp_send_reschedule(cpu);

	timeout = jiffies + (1 * HZ);
	while (time_before(jiffies, timeout)) {
		if (pen_release == -1)
			break;
	}

	/*
	 * now the secondary core is starting up let it run its
	 * calibrations, then wait for it to finish
	 */
	spin_unlock(&boot_lock);

	return pen_release != -1 ? -ENOSYS : 0;
}

static void __init wakeup_secondary(void)
{
	void __iomem *backupram;

	if (cpu_is_u5500())
		backupram = __io_address(U5500_BACKUPRAM0_BASE);
	else if (cpu_is_u8500())
		backupram = __io_address(U8500_BACKUPRAM0_BASE);
	else
		ux500_unknown_soc();

=======
#include <linux/of.h>
#include <linux/of_address.h>

#include <asm/cacheflush.h>
#include <asm/smp_plat.h>
#include <asm/smp_scu.h>

/* Magic triggers in backup RAM */
#define UX500_CPU1_JUMPADDR_OFFSET 0x1FF4
#define UX500_CPU1_WAKEMAGIC_OFFSET 0x1FF0

static void __iomem *backupram;

static void __init ux500_smp_prepare_cpus(unsigned int max_cpus)
{
	struct device_node *np;
	static void __iomem *scu_base;
	unsigned int ncores;
	int i;

	np = of_find_compatible_node(NULL, NULL, "ste,dbx500-backupram");
	if (!np) {
		pr_err("No backupram base address\n");
		return;
	}
	backupram = of_iomap(np, 0);
	of_node_put(np);
	if (!backupram) {
		pr_err("No backupram remap\n");
		return;
	}

	np = of_find_compatible_node(NULL, NULL, "arm,cortex-a9-scu");
	if (!np) {
		pr_err("No SCU base address\n");
		return;
	}
	scu_base = of_iomap(np, 0);
	of_node_put(np);
	if (!scu_base) {
		pr_err("No SCU remap\n");
		return;
	}

	scu_enable(scu_base);
	ncores = scu_get_core_count(scu_base);
	for (i = 0; i < ncores; i++)
		set_cpu_possible(i, true);
	iounmap(scu_base);
}

static int ux500_boot_secondary(unsigned int cpu, struct task_struct *idle)
{
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	/*
	 * write the address of secondary startup into the backup ram register
	 * at offset 0x1FF4, then write the magic number 0xA1FEED01 to the
	 * backup ram register at offset 0x1FF0, which is what boot rom code
<<<<<<< HEAD
	 * is waiting for. This would wake up the secondary core from WFE
	 */
#define UX500_CPU1_JUMPADDR_OFFSET 0x1FF4
	__raw_writel(virt_to_phys(u8500_secondary_startup),
		     backupram + UX500_CPU1_JUMPADDR_OFFSET);

#define UX500_CPU1_WAKEMAGIC_OFFSET 0x1FF0
	__raw_writel(0xA1FEED01,
		     backupram + UX500_CPU1_WAKEMAGIC_OFFSET);

	/* make sure write buffer is drained */
	mb();
}

/*
 * Initialise the CPU possible map early - this describes the CPUs
 * which may be present or become present in the system.
 */
void __init smp_init_cpus(void)
{
	void __iomem *scu_base = scu_base_addr();
	unsigned int i, ncores;

	ncores = scu_base ? scu_get_core_count(scu_base) : 1;

	/* sanity check */
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

	scu_enable(scu_base_addr());
	wakeup_secondary();
}
=======
	 * is waiting for. This will wake up the secondary core from WFE.
	 */
	writel(__pa_symbol(secondary_startup),
	       backupram + UX500_CPU1_JUMPADDR_OFFSET);
	writel(0xA1FEED01,
	       backupram + UX500_CPU1_WAKEMAGIC_OFFSET);

	/* make sure write buffer is drained */
	mb();
	arch_send_wakeup_ipi_mask(cpumask_of(cpu));
	return 0;
}

#ifdef CONFIG_HOTPLUG_CPU
static void ux500_cpu_die(unsigned int cpu)
{
	wfi();
}
#endif

static const struct smp_operations ux500_smp_ops __initconst = {
	.smp_prepare_cpus	= ux500_smp_prepare_cpus,
	.smp_boot_secondary	= ux500_boot_secondary,
#ifdef CONFIG_HOTPLUG_CPU
	.cpu_die		= ux500_cpu_die,
#endif
};
CPU_METHOD_OF_DECLARE(ux500_smp, "ste,dbx500-smp", &ux500_smp_ops);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
