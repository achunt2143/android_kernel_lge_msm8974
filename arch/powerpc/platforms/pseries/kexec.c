<<<<<<< HEAD
/*
 *  Copyright 2006 Michael Ellerman, IBM Corporation
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version
 * 2 of the License, or (at your option) any later version.
=======
// SPDX-License-Identifier: GPL-2.0-or-later
/*
 *  Copyright 2006 Michael Ellerman, IBM Corporation
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 */

#include <linux/kernel.h>
#include <linux/interrupt.h>

<<<<<<< HEAD
#include <asm/machdep.h>
#include <asm/page.h>
#include <asm/firmware.h>
#include <asm/kexec.h>
#include <asm/mpic.h>
#include <asm/xics.h>
#include <asm/smp.h>

#include "pseries.h"
#include "plpar_wrappers.h"

static void pseries_kexec_cpu_down(int crash_shutdown, int secondary)
{
	/* Don't risk a hypervisor call if we're crashing */
=======
#include <asm/setup.h>
#include <asm/page.h>
#include <asm/firmware.h>
#include <asm/kexec.h>
#include <asm/xics.h>
#include <asm/xive.h>
#include <asm/smp.h>
#include <asm/plpar_wrappers.h>

#include "pseries.h"

void pseries_kexec_cpu_down(int crash_shutdown, int secondary)
{
	/*
	 * Don't risk a hypervisor call if we're crashing
	 * XXX: Why? The hypervisor is not crashing. It might be better
	 * to at least attempt unregister to avoid the hypervisor stepping
	 * on our memory.
	 */
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	if (firmware_has_feature(FW_FEATURE_SPLPAR) && !crash_shutdown) {
		int ret;
		int cpu = smp_processor_id();
		int hwcpu = hard_smp_processor_id();

		if (get_lppaca()->dtl_enable_mask) {
			ret = unregister_dtl(hwcpu);
			if (ret) {
				pr_err("WARNING: DTL deregistration for cpu "
				       "%d (hw %d) failed with %d\n",
				       cpu, hwcpu, ret);
			}
		}

		ret = unregister_slb_shadow(hwcpu);
		if (ret) {
			pr_err("WARNING: SLB shadow buffer deregistration "
			       "for cpu %d (hw %d) failed with %d\n",
			       cpu, hwcpu, ret);
		}

		ret = unregister_vpa(hwcpu);
		if (ret) {
			pr_err("WARNING: VPA deregistration for cpu %d "
			       "(hw %d) failed with %d\n", cpu, hwcpu, ret);
		}
	}
<<<<<<< HEAD
}

static void pseries_kexec_cpu_down_mpic(int crash_shutdown, int secondary)
{
	pseries_kexec_cpu_down(crash_shutdown, secondary);
	mpic_teardown_this_cpu(secondary);
}

void __init setup_kexec_cpu_down_mpic(void)
{
	ppc_md.kexec_cpu_down = pseries_kexec_cpu_down_mpic;
}

static void pseries_kexec_cpu_down_xics(int crash_shutdown, int secondary)
{
	pseries_kexec_cpu_down(crash_shutdown, secondary);
	xics_kexec_teardown_cpu(secondary);
}

void __init setup_kexec_cpu_down_xics(void)
{
	ppc_md.kexec_cpu_down = pseries_kexec_cpu_down_xics;
=======

	if (xive_enabled()) {
		xive_teardown_cpu();

		if (!secondary)
			xive_shutdown();
	} else
		xics_kexec_teardown_cpu(secondary);
}

void pseries_machine_kexec(struct kimage *image)
{
	if (firmware_has_feature(FW_FEATURE_SET_MODE))
		pseries_disable_reloc_on_exc();

	default_machine_kexec(image);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}
