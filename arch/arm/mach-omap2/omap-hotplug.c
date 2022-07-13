<<<<<<< HEAD
=======
// SPDX-License-Identifier: GPL-2.0-only
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
/*
 * OMAP4 SMP cpu-hotplug support
 *
 * Copyright (C) 2010 Texas Instruments, Inc.
 * Author:
 *      Santosh Shilimkar <santosh.shilimkar@ti.com>
 *
 * Platform file needed for the OMAP4 SMP. This file is based on arm
 * realview smp platform.
 * Copyright (c) 2002 ARM Limited.
<<<<<<< HEAD
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 */

#include <linux/kernel.h>
#include <linux/errno.h>
#include <linux/smp.h>
<<<<<<< HEAD

#include <asm/cacheflush.h>

#include "common.h"

#include "powerdomain.h"

int platform_cpu_kill(unsigned int cpu)
{
	return 1;
}

=======
#include <linux/io.h>

#include "omap-wakeupgen.h"
#include "common.h"
#include "powerdomain.h"

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
/*
 * platform-specific code to shutdown a CPU
 * Called with IRQs disabled
 */
<<<<<<< HEAD
void __ref platform_cpu_die(unsigned int cpu)
{
	unsigned int this_cpu;

	flush_cache_all();
	dsb();
=======
void omap4_cpu_die(unsigned int cpu)
{
	unsigned int boot_cpu = 0;
	void __iomem *base = omap_get_wakeupgen_base();
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	/*
	 * we're ready for shutdown now, so do it
	 */
<<<<<<< HEAD
	if (omap_modify_auxcoreboot0(0x0, 0x200) != 0x0)
		pr_err("Secure clear status failed\n");
=======
	if (omap_secure_apis_support()) {
		if (omap_modify_auxcoreboot0(0x0, 0x200) != 0x0)
			pr_err("Secure clear status failed\n");
	} else {
		writel_relaxed(0, base + OMAP_AUX_CORE_BOOT_0);
	}

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	for (;;) {
		/*
		 * Enter into low power state
		 */
		omap4_hotplug_cpu(cpu, PWRDM_POWER_OFF);
<<<<<<< HEAD
		this_cpu = smp_processor_id();
		if (omap_read_auxcoreboot0() == this_cpu) {
=======

		if (omap_secure_apis_support())
			boot_cpu = omap_read_auxcoreboot0() >> 9;
		else
			boot_cpu =
				readl_relaxed(base + OMAP_AUX_CORE_BOOT_0) >> 5;

		if (boot_cpu == smp_processor_id()) {
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
			/*
			 * OK, proper wakeup, we're done
			 */
			break;
		}
		pr_debug("CPU%u: spurious wakeup call\n", cpu);
	}
}

<<<<<<< HEAD
int platform_cpu_disable(unsigned int cpu)
{
	/*
	 * we don't allow CPU 0 to be shutdown (it is still too special
	 * e.g. clock tick interrupts)
	 */
	return cpu == 0 ? -EPERM : 0;
=======
/* Needed by kexec and platform_can_cpu_hotplug() */
int omap4_cpu_kill(unsigned int cpu)
{
	return 1;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}
