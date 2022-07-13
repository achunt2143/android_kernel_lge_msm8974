<<<<<<< HEAD
=======
// SPDX-License-Identifier: GPL-2.0
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
/*
 * SMP support for R-Mobile / SH-Mobile - sh73a0 portion
 *
 * Copyright (C) 2010  Magnus Damm
 * Copyright (C) 2010  Takashi Yoshii
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
<<<<<<< HEAD
#include <linux/spinlock.h>
#include <linux/io.h>
#include <mach/common.h>
#include <asm/smp_plat.h>
#include <asm/smp_scu.h>
#include <asm/smp_twd.h>
#include <asm/hardware/gic.h>

#define WUPCR		IOMEM(0xe6151010)
#define SRESCR		IOMEM(0xe6151018)
#define PSTR		IOMEM(0xe6151040)
#define SBAR		IOMEM(0xe6180020)
#define APARMBAREA	IOMEM(0xe6f10020)

static void __iomem *scu_base_addr(void)
{
	return (void __iomem *)0xf0000000;
}

static DEFINE_SPINLOCK(scu_lock);
static unsigned long tmp;

#ifdef CONFIG_HAVE_ARM_TWD
static DEFINE_TWD_LOCAL_TIMER(twd_local_timer, 0xf0000600, 29);
void __init sh73a0_register_twd(void)
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

unsigned int __init sh73a0_get_core_count(void)
{
	void __iomem *scu_base = scu_base_addr();

	return scu_get_core_count(scu_base);
}

void __cpuinit sh73a0_secondary_init(unsigned int cpu)
{
	gic_secondary_init(0);
}

int __cpuinit sh73a0_boot_secondary(unsigned int cpu)
{
	cpu = cpu_logical_map(cpu);

	/* enable cache coherency */
	modify_scu_cpu_psr(0, 3 << (cpu * 8));

	if (((__raw_readl(PSTR) >> (4 * cpu)) & 3) == 3)
		__raw_writel(1 << cpu, WUPCR);	/* wake up */
	else
		__raw_writel(1 << cpu, SRESCR);	/* reset */

	return 0;
}

void __init sh73a0_smp_prepare_cpus(void)
{
	int cpu = cpu_logical_map(0);

	scu_enable(scu_base_addr());

	/* Map the reset vector (in headsmp.S) */
	__raw_writel(0, APARMBAREA);      /* 4k */
	__raw_writel(__pa(shmobile_secondary_vector), SBAR);

	/* enable cache coherency on CPU0 */
	modify_scu_cpu_psr(0, 3 << (cpu * 8));
}
=======
#include <linux/io.h>
#include <linux/delay.h>

#include <asm/smp_plat.h>

#include "common.h"
#include "sh73a0.h"

#define CPG_BASE2	0xe6151000
#define WUPCR		0x10	/* System-CPU Wake Up Control Register */
#define SRESCR		0x18	/* System-CPU Software Reset Control Register */
#define PSTR		0x40	/* System-CPU Power Status Register */

#define SYSC_BASE	0xe6180000
#define SBAR		0x20	/* SYS Boot Address Register */

#define AP_BASE		0xe6f10000
#define APARMBAREA	0x20	/* Address Translation Area Register */

#define SH73A0_SCU_BASE 0xf0000000

static int sh73a0_boot_secondary(unsigned int cpu, struct task_struct *idle)
{
	unsigned int lcpu = cpu_logical_map(cpu);
	void __iomem *cpg2 = ioremap(CPG_BASE2, PAGE_SIZE);

	if (((readl(cpg2 + PSTR) >> (4 * lcpu)) & 3) == 3)
		writel(1 << lcpu, cpg2 + WUPCR);	/* wake up */
	else
		writel(1 << lcpu, cpg2 + SRESCR);	/* reset */
	iounmap(cpg2);
	return 0;
}

static void __init sh73a0_smp_prepare_cpus(unsigned int max_cpus)
{
	void __iomem *ap, *sysc;

	if (!request_mem_region(0, SZ_4K, "Boot Area")) {
		pr_err("Failed to request boot area\n");
		return;
	}

	/* Map the reset vector (in headsmp.S) */
	ap = ioremap(AP_BASE, PAGE_SIZE);
	sysc = ioremap(SYSC_BASE, PAGE_SIZE);
	writel(0, ap + APARMBAREA);      /* 4k */
	writel(__pa(shmobile_boot_vector), sysc + SBAR);
	iounmap(sysc);
	iounmap(ap);

	/* setup sh73a0 specific SCU bits */
	shmobile_smp_scu_prepare_cpus(SH73A0_SCU_BASE, max_cpus);
}

const struct smp_operations sh73a0_smp_ops __initconst = {
	.smp_prepare_cpus	= sh73a0_smp_prepare_cpus,
	.smp_boot_secondary	= sh73a0_boot_secondary,
#ifdef CONFIG_HOTPLUG_CPU
	.cpu_can_disable	= shmobile_smp_cpu_can_disable,
	.cpu_die		= shmobile_smp_scu_cpu_die,
	.cpu_kill		= shmobile_smp_scu_cpu_kill,
#endif
};
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
