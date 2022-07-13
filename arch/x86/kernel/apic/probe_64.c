<<<<<<< HEAD
/*
 * Copyright 2004 James Cleverdon, IBM.
 * Subject to the GNU Public License, v.2
=======
// SPDX-License-Identifier: GPL-2.0-only
/*
 * Copyright 2004 James Cleverdon, IBM.
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 *
 * Generic APIC sub-arch probe layer.
 *
 * Hacked for x86-64 by James Cleverdon from i386 architecture code by
 * Martin Bligh, Andi Kleen, James Bottomley, John Stultz, and
 * James Cleverdon.
 */
<<<<<<< HEAD
#include <linux/threads.h>
#include <linux/cpumask.h>
#include <linux/string.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/ctype.h>
#include <linux/init.h>
#include <linux/hardirq.h>
#include <linux/dmar.h>

#include <asm/smp.h>
#include <asm/apic.h>
#include <asm/ipi.h>
#include <asm/setup.h>

static int apicid_phys_pkg_id(int initial_apic_id, int index_msb)
{
	return hard_smp_processor_id() >> index_msb;
}

/*
 * Check the APIC IDs in bios_cpu_apicid and choose the APIC mode.
 */
void __init default_setup_apic_routing(void)
=======
#include <linux/thread_info.h>
#include <asm/apic.h>

#include "local.h"

/* Select the appropriate APIC driver */
void __init x86_64_probe_apic(void)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
{
	struct apic **drv;

	enable_IR_x2apic();

	for (drv = __apicdrivers; drv < __apicdrivers_end; drv++) {
		if ((*drv)->probe && (*drv)->probe()) {
<<<<<<< HEAD
			if (apic != *drv) {
				apic = *drv;
				pr_info("Switched APIC routing to %s.\n",
					apic->name);
			}
			break;
		}
	}

	if (is_vsmp_box()) {
		/* need to update phys_pkg_id */
		apic->phys_pkg_id = apicid_phys_pkg_id;
	}
}

/* Same for both flat and physical. */

void apic_send_IPI_self(int vector)
{
	__default_send_IPI_shortcut(APIC_DEST_SELF, vector, APIC_DEST_PHYSICAL);
=======
			apic_install_driver(*drv);
			break;
		}
	}
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

int __init default_acpi_madt_oem_check(char *oem_id, char *oem_table_id)
{
	struct apic **drv;

	for (drv = __apicdrivers; drv < __apicdrivers_end; drv++) {
		if ((*drv)->acpi_madt_oem_check(oem_id, oem_table_id)) {
<<<<<<< HEAD
			if (apic != *drv) {
				apic = *drv;
				pr_info("Setting APIC routing to %s.\n",
					apic->name);
			}
=======
			apic_install_driver(*drv);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
			return 1;
		}
	}
	return 0;
}
