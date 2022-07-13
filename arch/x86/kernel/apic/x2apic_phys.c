<<<<<<< HEAD
#include <linux/threads.h>
#include <linux/cpumask.h>
#include <linux/string.h>
#include <linux/kernel.h>
#include <linux/ctype.h>
#include <linux/init.h>
#include <linux/dmar.h>

#include <asm/smp.h>
#include <asm/x2apic.h>
=======
// SPDX-License-Identifier: GPL-2.0

#include <linux/cpumask.h>
#include <linux/acpi.h>

#include "local.h"
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

int x2apic_phys;

static struct apic apic_x2apic_phys;
<<<<<<< HEAD

static int set_x2apic_phys_mode(char *arg)
=======
u32 x2apic_max_apicid __ro_after_init = UINT_MAX;

void __init x2apic_set_max_apicid(u32 apicid)
{
	x2apic_max_apicid = apicid;
	if (apic->x2apic_set_max_apicid)
		apic->max_apic_id = apicid;
}

static int __init set_x2apic_phys_mode(char *arg)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
{
	x2apic_phys = 1;
	return 0;
}
early_param("x2apic_phys", set_x2apic_phys_mode);

static bool x2apic_fadt_phys(void)
{
<<<<<<< HEAD
=======
#ifdef CONFIG_ACPI
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	if ((acpi_gbl_FADT.header.revision >= FADT2_REVISION_ID) &&
		(acpi_gbl_FADT.flags & ACPI_FADT_APIC_PHYSICAL)) {
		printk(KERN_DEBUG "System requires x2apic physical mode\n");
		return true;
	}
<<<<<<< HEAD
=======
#endif
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	return false;
}

static int x2apic_acpi_madt_oem_check(char *oem_id, char *oem_table_id)
{
	return x2apic_enabled() && (x2apic_phys || x2apic_fadt_phys());
}

<<<<<<< HEAD
=======
static void x2apic_send_IPI(int cpu, int vector)
{
	u32 dest = per_cpu(x86_cpu_to_apicid, cpu);

	/* x2apic MSRs are special and need a special fence: */
	weak_wrmsr_fence();
	__x2apic_send_IPI_dest(dest, vector, APIC_DEST_PHYSICAL);
}

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
static void
__x2apic_send_IPI_mask(const struct cpumask *mask, int vector, int apic_dest)
{
	unsigned long query_cpu;
	unsigned long this_cpu;
	unsigned long flags;

<<<<<<< HEAD
	x2apic_wrmsr_fence();
=======
	/* x2apic MSRs are special and need a special fence: */
	weak_wrmsr_fence();
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	local_irq_save(flags);

	this_cpu = smp_processor_id();
	for_each_cpu(query_cpu, mask) {
		if (apic_dest == APIC_DEST_ALLBUT && this_cpu == query_cpu)
			continue;
		__x2apic_send_IPI_dest(per_cpu(x86_cpu_to_apicid, query_cpu),
				       vector, APIC_DEST_PHYSICAL);
	}
	local_irq_restore(flags);
}

static void x2apic_send_IPI_mask(const struct cpumask *mask, int vector)
{
	__x2apic_send_IPI_mask(mask, vector, APIC_DEST_ALLINC);
}

static void
 x2apic_send_IPI_mask_allbutself(const struct cpumask *mask, int vector)
{
	__x2apic_send_IPI_mask(mask, vector, APIC_DEST_ALLBUT);
}

<<<<<<< HEAD
static void x2apic_send_IPI_allbutself(int vector)
{
	__x2apic_send_IPI_mask(cpu_online_mask, vector, APIC_DEST_ALLBUT);
}

static void x2apic_send_IPI_all(int vector)
{
	__x2apic_send_IPI_mask(cpu_online_mask, vector, APIC_DEST_ALLINC);
}

static unsigned int x2apic_cpu_mask_to_apicid(const struct cpumask *cpumask)
{
	/*
	 * We're using fixed IRQ delivery, can only return one phys APIC ID.
	 * May as well be the first.
	 */
	int cpu = cpumask_first(cpumask);

	if ((unsigned)cpu < nr_cpu_ids)
		return per_cpu(x86_cpu_to_apicid, cpu);
	else
		return BAD_APICID;
}

static unsigned int
x2apic_cpu_mask_to_apicid_and(const struct cpumask *cpumask,
			      const struct cpumask *andmask)
{
	int cpu;

	/*
	 * We're using fixed IRQ delivery, can only return one phys APIC ID.
	 * May as well be the first.
	 */
	for_each_cpu_and(cpu, cpumask, andmask) {
		if (cpumask_test_cpu(cpu, cpu_online_mask))
			break;
	}

	return per_cpu(x86_cpu_to_apicid, cpu);
}

static void init_x2apic_ldr(void)
{
=======
static void __x2apic_send_IPI_shorthand(int vector, u32 which)
{
	unsigned long cfg = __prepare_ICR(which, vector, 0);

	/* x2apic MSRs are special and need a special fence: */
	weak_wrmsr_fence();
	native_x2apic_icr_write(cfg, 0);
}

void x2apic_send_IPI_allbutself(int vector)
{
	__x2apic_send_IPI_shorthand(vector, APIC_DEST_ALLBUT);
}

void x2apic_send_IPI_all(int vector)
{
	__x2apic_send_IPI_shorthand(vector, APIC_DEST_ALLINC);
}

void x2apic_send_IPI_self(int vector)
{
	apic_write(APIC_SELF_IPI, vector);
}

void __x2apic_send_IPI_dest(unsigned int apicid, int vector, unsigned int dest)
{
	unsigned long cfg = __prepare_ICR(0, vector, dest);
	native_x2apic_icr_write(cfg, apicid);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

static int x2apic_phys_probe(void)
{
<<<<<<< HEAD
	if (x2apic_mode && (x2apic_phys || x2apic_fadt_phys()))
=======
	if (!x2apic_mode)
		return 0;

	if (x2apic_phys || x2apic_fadt_phys())
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		return 1;

	return apic == &apic_x2apic_phys;
}

<<<<<<< HEAD
static struct apic apic_x2apic_phys = {
=======
u32 x2apic_get_apic_id(u32 id)
{
	return id;
}

static struct apic apic_x2apic_phys __ro_after_init = {
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	.name				= "physical x2apic",
	.probe				= x2apic_phys_probe,
	.acpi_madt_oem_check		= x2apic_acpi_madt_oem_check,
<<<<<<< HEAD
	.apic_id_valid			= x2apic_apic_id_valid,
	.apic_id_registered		= x2apic_apic_id_registered,

	.irq_delivery_mode		= dest_Fixed,
	.irq_dest_mode			= 0, /* physical */

	.target_cpus			= x2apic_target_cpus,
	.disable_esr			= 0,
	.dest_logical			= 0,
	.check_apicid_used		= NULL,
	.check_apicid_present		= NULL,

	.vector_allocation_domain	= x2apic_vector_allocation_domain,
	.init_apic_ldr			= init_x2apic_ldr,

	.ioapic_phys_id_map		= NULL,
	.setup_apic_routing		= NULL,
	.multi_timer_check		= NULL,
	.cpu_present_to_apicid		= default_cpu_present_to_apicid,
	.apicid_to_cpu_present		= NULL,
	.setup_portio_remap		= NULL,
	.check_phys_apicid_present	= default_check_phys_apicid_present,
	.enable_apic_mode		= NULL,
	.phys_pkg_id			= x2apic_phys_pkg_id,
	.mps_oem_check			= NULL,

	.get_apic_id			= x2apic_get_apic_id,
	.set_apic_id			= x2apic_set_apic_id,
	.apic_id_mask			= 0xFFFFFFFFu,

	.cpu_mask_to_apicid		= x2apic_cpu_mask_to_apicid,
	.cpu_mask_to_apicid_and		= x2apic_cpu_mask_to_apicid_and,

=======

	.dest_mode_logical		= false,

	.disable_esr			= 0,

	.cpu_present_to_apicid		= default_cpu_present_to_apicid,

	.max_apic_id			= UINT_MAX,
	.x2apic_set_max_apicid		= true,
	.get_apic_id			= x2apic_get_apic_id,

	.calc_dest_apicid		= apic_default_calc_apicid,

	.send_IPI			= x2apic_send_IPI,
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	.send_IPI_mask			= x2apic_send_IPI_mask,
	.send_IPI_mask_allbutself	= x2apic_send_IPI_mask_allbutself,
	.send_IPI_allbutself		= x2apic_send_IPI_allbutself,
	.send_IPI_all			= x2apic_send_IPI_all,
	.send_IPI_self			= x2apic_send_IPI_self,
<<<<<<< HEAD

	.trampoline_phys_low		= DEFAULT_TRAMPOLINE_PHYS_LOW,
	.trampoline_phys_high		= DEFAULT_TRAMPOLINE_PHYS_HIGH,
	.wait_for_init_deassert		= NULL,
	.smp_callin_clear_local_apic	= NULL,
	.inquire_remote_apic		= NULL,

	.read				= native_apic_msr_read,
	.write				= native_apic_msr_write,
	.icr_read			= native_x2apic_icr_read,
	.icr_write			= native_x2apic_icr_write,
	.wait_icr_idle			= native_x2apic_wait_icr_idle,
	.safe_wait_icr_idle		= native_safe_x2apic_wait_icr_idle,
=======
	.nmi_to_offline_cpu		= true,

	.read				= native_apic_msr_read,
	.write				= native_apic_msr_write,
	.eoi				= native_apic_msr_eoi,
	.icr_read			= native_x2apic_icr_read,
	.icr_write			= native_x2apic_icr_write,
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
};

apic_driver(apic_x2apic_phys);
