<<<<<<< HEAD
=======
// SPDX-License-Identifier: GPL-2.0
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
/*
 * SMP support for R-Mobile / SH-Mobile - r8a7779 portion
 *
 * Copyright (C) 2011  Renesas Solutions Corp.
 * Copyright (C) 2011  Magnus Damm
<<<<<<< HEAD
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; version 2 of the License.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 */
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/smp.h>
#include <linux/spinlock.h>
#include <linux/io.h>
#include <linux/delay.h>
<<<<<<< HEAD
#include <mach/common.h>
#include <mach/r8a7779.h>
#include <asm/smp_plat.h>
#include <asm/smp_scu.h>
#include <asm/smp_twd.h>
#include <asm/hardware/gic.h>

#define AVECR IOMEM(0xfe700040)

static struct r8a7779_pm_ch r8a7779_ch_cpu1 = {
	.chan_offs = 0x40, /* PWRSR0 .. PWRER0 */
	.chan_bit = 1, /* ARM1 */
	.isr_bit = 1, /* ARM1 */
};

static struct r8a7779_pm_ch r8a7779_ch_cpu2 = {
	.chan_offs = 0x40, /* PWRSR0 .. PWRER0 */
	.chan_bit = 2, /* ARM2 */
	.isr_bit = 2, /* ARM2 */
};

static struct r8a7779_pm_ch r8a7779_ch_cpu3 = {
	.chan_offs = 0x40, /* PWRSR0 .. PWRER0 */
	.chan_bit = 3, /* ARM3 */
	.isr_bit = 3, /* ARM3 */
};

static struct r8a7779_pm_ch *r8a7779_ch_cpu[4] = {
	[1] = &r8a7779_ch_cpu1,
	[2] = &r8a7779_ch_cpu2,
	[3] = &r8a7779_ch_cpu3,
};

static void __iomem *scu_base_addr(void)
{
	return (void __iomem *)0xf0000000;
}

static DEFINE_SPINLOCK(scu_lock);
static unsigned long tmp;

#ifdef CONFIG_HAVE_ARM_TWD
static DEFINE_TWD_LOCAL_TIMER(twd_local_timer, 0xf0000600, 29);

void __init r8a7779_register_twd(void)
{
	twd_local_timer_register(&twd_local_timer);
}
#endif

static void modify_scu_cpu_psr(unsigned long set, unsigned long clr)
{
	void __iomem *scu_base = scu_base_addr();

	spin_lock(&scu_lock);
	tmp = __raw_readl(scu_base + 8);
	tmp &= ~clr;
	tmp |= set;
	spin_unlock(&scu_lock);

	/* disable cache coherency after releasing the lock */
	__raw_writel(tmp, scu_base + 8);
}

unsigned int __init r8a7779_get_core_count(void)
{
	void __iomem *scu_base = scu_base_addr();

	return scu_get_core_count(scu_base);
}

int r8a7779_platform_cpu_kill(unsigned int cpu)
{
	struct r8a7779_pm_ch *ch = NULL;
	int ret = -EIO;

	cpu = cpu_logical_map(cpu);

	/* disable cache coherency */
	modify_scu_cpu_psr(3 << (cpu * 8), 0);

	if (cpu < ARRAY_SIZE(r8a7779_ch_cpu))
		ch = r8a7779_ch_cpu[cpu];

	if (ch)
		ret = r8a7779_sysc_power_down(ch);

	return ret ? ret : 1;
}

void __cpuinit r8a7779_secondary_init(unsigned int cpu)
{
	gic_secondary_init(0);
}

int __cpuinit r8a7779_boot_secondary(unsigned int cpu)
{
	struct r8a7779_pm_ch *ch = NULL;
	int ret = -EIO;

	cpu = cpu_logical_map(cpu);

	/* enable cache coherency */
	modify_scu_cpu_psr(0, 3 << (cpu * 8));

	if (cpu < ARRAY_SIZE(r8a7779_ch_cpu))
		ch = r8a7779_ch_cpu[cpu];

	if (ch)
		ret = r8a7779_sysc_power_up(ch);
=======
#include <linux/soc/renesas/rcar-sysc.h>

#include <asm/cacheflush.h>
#include <asm/smp_plat.h>
#include <asm/smp_scu.h>

#include "common.h"
#include "r8a7779.h"

#define HPBREG_BASE		0xfe700000
#define AVECR			0x0040	/* ARM Reset Vector Address Register */

#define R8A7779_SCU_BASE	0xf0000000

static int r8a7779_boot_secondary(unsigned int cpu, struct task_struct *idle)
{
	int ret = -EIO;

	cpu = cpu_logical_map(cpu);
	if (cpu)
		ret = rcar_sysc_power_up_cpu(cpu);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	return ret;
}

<<<<<<< HEAD
void __init r8a7779_smp_prepare_cpus(void)
{
	int cpu = cpu_logical_map(0);

	scu_enable(scu_base_addr());

	/* Map the reset vector (in headsmp.S) */
	__raw_writel(__pa(shmobile_secondary_vector), AVECR);

	/* enable cache coherency on CPU0 */
	modify_scu_cpu_psr(0, 3 << (cpu * 8));

	r8a7779_pm_init();

	/* power off secondary CPUs */
	r8a7779_platform_cpu_kill(1);
	r8a7779_platform_cpu_kill(2);
	r8a7779_platform_cpu_kill(3);
}
=======
static void __init r8a7779_smp_prepare_cpus(unsigned int max_cpus)
{
	void __iomem *base;

	if (!request_mem_region(0, SZ_4K, "Boot Area")) {
		pr_err("Failed to request boot area\n");
		return;
	}

	base = ioremap(HPBREG_BASE, 0x1000);

	/* Map the reset vector (in headsmp-scu.S, headsmp.S) */
	writel(__pa(shmobile_boot_vector), base + AVECR);

	/* setup r8a7779 specific SCU bits */
	shmobile_smp_scu_prepare_cpus(R8A7779_SCU_BASE, max_cpus);

	iounmap(base);
}

#ifdef CONFIG_HOTPLUG_CPU
static int r8a7779_platform_cpu_kill(unsigned int cpu)
{
	int ret = -EIO;

	cpu = cpu_logical_map(cpu);
	if (cpu)
		ret = rcar_sysc_power_down_cpu(cpu);

	return ret ? ret : 1;
}

static int r8a7779_cpu_kill(unsigned int cpu)
{
	if (shmobile_smp_scu_cpu_kill(cpu))
		return r8a7779_platform_cpu_kill(cpu);

	return 0;
}
#endif /* CONFIG_HOTPLUG_CPU */

const struct smp_operations r8a7779_smp_ops  __initconst = {
	.smp_prepare_cpus	= r8a7779_smp_prepare_cpus,
	.smp_boot_secondary	= r8a7779_boot_secondary,
#ifdef CONFIG_HOTPLUG_CPU
	.cpu_die		= shmobile_smp_scu_cpu_die,
	.cpu_kill		= r8a7779_cpu_kill,
#endif
};
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
