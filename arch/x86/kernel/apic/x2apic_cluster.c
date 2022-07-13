<<<<<<< HEAD
#include <linux/threads.h>
#include <linux/cpumask.h>
#include <linux/string.h>
#include <linux/kernel.h>
#include <linux/ctype.h>
#include <linux/init.h>
#include <linux/dmar.h>
#include <linux/cpu.h>

#include <asm/smp.h>
#include <asm/x2apic.h>

static DEFINE_PER_CPU(u32, x86_cpu_to_logical_apicid);
static DEFINE_PER_CPU(cpumask_var_t, cpus_in_cluster);
static DEFINE_PER_CPU(cpumask_var_t, ipi_mask);
=======
// SPDX-License-Identifier: GPL-2.0

#include <linux/cpuhotplug.h>
#include <linux/cpumask.h>
#include <linux/slab.h>
#include <linux/mm.h>

#include <asm/apic.h>

#include "local.h"

#define apic_cluster(apicid) ((apicid) >> 4)

/*
 * __x2apic_send_IPI_mask() possibly needs to read
 * x86_cpu_to_logical_apicid for all online cpus in a sequential way.
 * Using per cpu variable would cost one cache line per cpu.
 */
static u32 *x86_cpu_to_logical_apicid __read_mostly;

static DEFINE_PER_CPU(cpumask_var_t, ipi_mask);
static DEFINE_PER_CPU_READ_MOSTLY(struct cpumask *, cluster_masks);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

static int x2apic_acpi_madt_oem_check(char *oem_id, char *oem_table_id)
{
	return x2apic_enabled();
}

<<<<<<< HEAD
static inline u32 x2apic_cluster(int cpu)
{
	return per_cpu(x86_cpu_to_logical_apicid, cpu) >> 16;
=======
static void x2apic_send_IPI(int cpu, int vector)
{
	u32 dest = x86_cpu_to_logical_apicid[cpu];

	/* x2apic MSRs are special and need a special fence: */
	weak_wrmsr_fence();
	__x2apic_send_IPI_dest(dest, vector, APIC_DEST_LOGICAL);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

static void
__x2apic_send_IPI_mask(const struct cpumask *mask, int vector, int apic_dest)
{
<<<<<<< HEAD
	struct cpumask *cpus_in_cluster_ptr;
	struct cpumask *ipi_mask_ptr;
	unsigned int cpu, this_cpu;
	unsigned long flags;
	u32 dest;

	x2apic_wrmsr_fence();

	local_irq_save(flags);

	this_cpu = smp_processor_id();

	/*
	 * We are to modify mask, so we need an own copy
	 * and be sure it's manipulated with irq off.
	 */
	ipi_mask_ptr = __raw_get_cpu_var(ipi_mask);
	cpumask_copy(ipi_mask_ptr, mask);

	/*
	 * The idea is to send one IPI per cluster.
	 */
	for_each_cpu(cpu, ipi_mask_ptr) {
		unsigned long i;

		cpus_in_cluster_ptr = per_cpu(cpus_in_cluster, cpu);
		dest = 0;

		/* Collect cpus in cluster. */
		for_each_cpu_and(i, ipi_mask_ptr, cpus_in_cluster_ptr) {
			if (apic_dest == APIC_DEST_ALLINC || i != this_cpu)
				dest |= per_cpu(x86_cpu_to_logical_apicid, i);
		}
=======
	unsigned int cpu, clustercpu;
	struct cpumask *tmpmsk;
	unsigned long flags;
	u32 dest;

	/* x2apic MSRs are special and need a special fence: */
	weak_wrmsr_fence();
	local_irq_save(flags);

	tmpmsk = this_cpu_cpumask_var_ptr(ipi_mask);
	cpumask_copy(tmpmsk, mask);
	/* If IPI should not be sent to self, clear current CPU */
	if (apic_dest != APIC_DEST_ALLINC)
		__cpumask_clear_cpu(smp_processor_id(), tmpmsk);

	/* Collapse cpus in a cluster so a single IPI per cluster is sent */
	for_each_cpu(cpu, tmpmsk) {
		struct cpumask *cmsk = per_cpu(cluster_masks, cpu);

		dest = 0;
		for_each_cpu_and(clustercpu, tmpmsk, cmsk)
			dest |= x86_cpu_to_logical_apicid[clustercpu];
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

		if (!dest)
			continue;

<<<<<<< HEAD
		__x2apic_send_IPI_dest(dest, vector, apic->dest_logical);
		/*
		 * Cluster sibling cpus should be discared now so
		 * we would not send IPI them second time.
		 */
		cpumask_andnot(ipi_mask_ptr, ipi_mask_ptr, cpus_in_cluster_ptr);
=======
		__x2apic_send_IPI_dest(dest, vector, APIC_DEST_LOGICAL);
		/* Remove cluster CPUs from tmpmask */
		cpumask_andnot(tmpmsk, tmpmsk, cmsk);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	}

	local_irq_restore(flags);
}

static void x2apic_send_IPI_mask(const struct cpumask *mask, int vector)
{
	__x2apic_send_IPI_mask(mask, vector, APIC_DEST_ALLINC);
}

static void
<<<<<<< HEAD
 x2apic_send_IPI_mask_allbutself(const struct cpumask *mask, int vector)
=======
x2apic_send_IPI_mask_allbutself(const struct cpumask *mask, int vector)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
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
	 * We're using fixed IRQ delivery, can only return one logical APIC ID.
	 * May as well be the first.
	 */
	int cpu = cpumask_first(cpumask);

	if ((unsigned)cpu < nr_cpu_ids)
		return per_cpu(x86_cpu_to_logical_apicid, cpu);
	else
		return BAD_APICID;
}

static unsigned int
x2apic_cpu_mask_to_apicid_and(const struct cpumask *cpumask,
			      const struct cpumask *andmask)
{
	int cpu;

	/*
	 * We're using fixed IRQ delivery, can only return one logical APIC ID.
	 * May as well be the first.
	 */
	for_each_cpu_and(cpu, cpumask, andmask) {
		if (cpumask_test_cpu(cpu, cpu_online_mask))
			break;
	}

	return per_cpu(x86_cpu_to_logical_apicid, cpu);
=======
static u32 x2apic_calc_apicid(unsigned int cpu)
{
	return x86_cpu_to_logical_apicid[cpu];
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

static void init_x2apic_ldr(void)
{
<<<<<<< HEAD
	unsigned int this_cpu = smp_processor_id();
	unsigned int cpu;

	per_cpu(x86_cpu_to_logical_apicid, this_cpu) = apic_read(APIC_LDR);

	__cpu_set(this_cpu, per_cpu(cpus_in_cluster, this_cpu));
	for_each_online_cpu(cpu) {
		if (x2apic_cluster(this_cpu) != x2apic_cluster(cpu))
			continue;
		__cpu_set(this_cpu, per_cpu(cpus_in_cluster, cpu));
		__cpu_set(cpu, per_cpu(cpus_in_cluster, this_cpu));
	}
}

 /*
  * At CPU state changes, update the x2apic cluster sibling info.
  */
static int __cpuinit
update_clusterinfo(struct notifier_block *nfb, unsigned long action, void *hcpu)
{
	unsigned int this_cpu = (unsigned long)hcpu;
	unsigned int cpu;
	int err = 0;

	switch (action) {
	case CPU_UP_PREPARE:
		if (!zalloc_cpumask_var(&per_cpu(cpus_in_cluster, this_cpu),
					GFP_KERNEL)) {
			err = -ENOMEM;
		} else if (!zalloc_cpumask_var(&per_cpu(ipi_mask, this_cpu),
					       GFP_KERNEL)) {
			free_cpumask_var(per_cpu(cpus_in_cluster, this_cpu));
			err = -ENOMEM;
		}
		break;
	case CPU_UP_CANCELED:
	case CPU_UP_CANCELED_FROZEN:
	case CPU_DEAD:
		for_each_online_cpu(cpu) {
			if (x2apic_cluster(this_cpu) != x2apic_cluster(cpu))
				continue;
			__cpu_clear(this_cpu, per_cpu(cpus_in_cluster, cpu));
			__cpu_clear(cpu, per_cpu(cpus_in_cluster, this_cpu));
		}
		free_cpumask_var(per_cpu(cpus_in_cluster, this_cpu));
		free_cpumask_var(per_cpu(ipi_mask, this_cpu));
		break;
	}

	return notifier_from_errno(err);
}

static struct notifier_block __refdata x2apic_cpu_notifier = {
	.notifier_call = update_clusterinfo,
};

static int x2apic_init_cpu_notifier(void)
{
	int cpu = smp_processor_id();

	zalloc_cpumask_var(&per_cpu(cpus_in_cluster, cpu), GFP_KERNEL);
	zalloc_cpumask_var(&per_cpu(ipi_mask, cpu), GFP_KERNEL);

	BUG_ON(!per_cpu(cpus_in_cluster, cpu) || !per_cpu(ipi_mask, cpu));

	__cpu_set(cpu, per_cpu(cpus_in_cluster, cpu));
	register_hotcpu_notifier(&x2apic_cpu_notifier);
	return 1;
=======
	struct cpumask *cmsk = this_cpu_read(cluster_masks);

	BUG_ON(!cmsk);

	cpumask_set_cpu(smp_processor_id(), cmsk);
}

/*
 * As an optimisation during boot, set the cluster_mask for all present
 * CPUs at once, to prevent each of them having to iterate over the others
 * to find the existing cluster_mask.
 */
static void prefill_clustermask(struct cpumask *cmsk, unsigned int cpu, u32 cluster)
{
	int cpu_i;

	for_each_present_cpu(cpu_i) {
		struct cpumask **cpu_cmsk = &per_cpu(cluster_masks, cpu_i);
		u32 apicid = apic->cpu_present_to_apicid(cpu_i);

		if (apicid == BAD_APICID || cpu_i == cpu || apic_cluster(apicid) != cluster)
			continue;

		if (WARN_ON_ONCE(*cpu_cmsk == cmsk))
			continue;

		BUG_ON(*cpu_cmsk);
		*cpu_cmsk = cmsk;
	}
}

static int alloc_clustermask(unsigned int cpu, u32 cluster, int node)
{
	struct cpumask *cmsk = NULL;
	unsigned int cpu_i;

	/*
	 * At boot time, the CPU present mask is stable. The cluster mask is
	 * allocated for the first CPU in the cluster and propagated to all
	 * present siblings in the cluster. If the cluster mask is already set
	 * on entry to this function for a given CPU, there is nothing to do.
	 */
	if (per_cpu(cluster_masks, cpu))
		return 0;

	if (system_state < SYSTEM_RUNNING)
		goto alloc;

	/*
	 * On post boot hotplug for a CPU which was not present at boot time,
	 * iterate over all possible CPUs (even those which are not present
	 * any more) to find any existing cluster mask.
	 */
	for_each_possible_cpu(cpu_i) {
		u32 apicid = apic->cpu_present_to_apicid(cpu_i);

		if (apicid != BAD_APICID && apic_cluster(apicid) == cluster) {
			cmsk = per_cpu(cluster_masks, cpu_i);
			/*
			 * If the cluster is already initialized, just store
			 * the mask and return. There's no need to propagate.
			 */
			if (cmsk) {
				per_cpu(cluster_masks, cpu) = cmsk;
				return 0;
			}
		}
	}
	/*
	 * No CPU in the cluster has ever been initialized, so fall through to
	 * the boot time code which will also populate the cluster mask for any
	 * other CPU in the cluster which is (now) present.
	 */
alloc:
	cmsk = kzalloc_node(sizeof(*cmsk), GFP_KERNEL, node);
	if (!cmsk)
		return -ENOMEM;
	per_cpu(cluster_masks, cpu) = cmsk;
	prefill_clustermask(cmsk, cpu, cluster);

	return 0;
}

static int x2apic_prepare_cpu(unsigned int cpu)
{
	u32 phys_apicid = apic->cpu_present_to_apicid(cpu);
	u32 cluster = apic_cluster(phys_apicid);
	u32 logical_apicid = (cluster << 16) | (1 << (phys_apicid & 0xf));

	x86_cpu_to_logical_apicid[cpu] = logical_apicid;

	if (alloc_clustermask(cpu, cluster, cpu_to_node(cpu)) < 0)
		return -ENOMEM;
	if (!zalloc_cpumask_var(&per_cpu(ipi_mask, cpu), GFP_KERNEL))
		return -ENOMEM;
	return 0;
}

static int x2apic_dead_cpu(unsigned int dead_cpu)
{
	struct cpumask *cmsk = per_cpu(cluster_masks, dead_cpu);

	if (cmsk)
		cpumask_clear_cpu(dead_cpu, cmsk);
	free_cpumask_var(per_cpu(ipi_mask, dead_cpu));
	return 0;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

static int x2apic_cluster_probe(void)
{
<<<<<<< HEAD
	if (x2apic_mode)
		return x2apic_init_cpu_notifier();
	else
		return 0;
}

static struct apic apic_x2apic_cluster = {
=======
	u32 slots;

	if (!x2apic_mode)
		return 0;

	slots = max_t(u32, L1_CACHE_BYTES/sizeof(u32), nr_cpu_ids);
	x86_cpu_to_logical_apicid = kcalloc(slots, sizeof(u32), GFP_KERNEL);
	if (!x86_cpu_to_logical_apicid)
		return 0;

	if (cpuhp_setup_state(CPUHP_X2APIC_PREPARE, "x86/x2apic:prepare",
			      x2apic_prepare_cpu, x2apic_dead_cpu) < 0) {
		pr_err("Failed to register X2APIC_PREPARE\n");
		kfree(x86_cpu_to_logical_apicid);
		x86_cpu_to_logical_apicid = NULL;
		return 0;
	}
	init_x2apic_ldr();
	return 1;
}

static struct apic apic_x2apic_cluster __ro_after_init = {
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	.name				= "cluster x2apic",
	.probe				= x2apic_cluster_probe,
	.acpi_madt_oem_check		= x2apic_acpi_madt_oem_check,
<<<<<<< HEAD
	.apic_id_valid			= x2apic_apic_id_valid,
	.apic_id_registered		= x2apic_apic_id_registered,

	.irq_delivery_mode		= dest_LowestPrio,
	.irq_dest_mode			= 1, /* logical */

	.target_cpus			= x2apic_target_cpus,
	.disable_esr			= 0,
	.dest_logical			= APIC_DEST_LOGICAL,
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

	.dest_mode_logical		= true,

	.disable_esr			= 0,

	.init_apic_ldr			= init_x2apic_ldr,
	.cpu_present_to_apicid		= default_cpu_present_to_apicid,

	.max_apic_id			= UINT_MAX,
	.x2apic_set_max_apicid		= true,
	.get_apic_id			= x2apic_get_apic_id,

	.calc_dest_apicid		= x2apic_calc_apicid,

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

apic_driver(apic_x2apic_cluster);
