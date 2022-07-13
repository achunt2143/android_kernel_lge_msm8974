<<<<<<< HEAD
=======
// SPDX-License-Identifier: GPL-2.0-or-later
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
/*
 * SMP support for pSeries machines.
 *
 * Dave Engebretsen, Peter Bergner, and
 * Mike Corrigan {engebret|bergner|mikec}@us.ibm.com
 *
 * Plus various changes from other IBM teams...
<<<<<<< HEAD
 *
 *      This program is free software; you can redistribute it and/or
 *      modify it under the terms of the GNU General Public License
 *      as published by the Free Software Foundation; either version
 *      2 of the License, or (at your option) any later version.
=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 */


#include <linux/kernel.h>
#include <linux/sched.h>
#include <linux/smp.h>
#include <linux/interrupt.h>
#include <linux/delay.h>
#include <linux/init.h>
#include <linux/spinlock.h>
#include <linux/cache.h>
#include <linux/err.h>
#include <linux/device.h>
#include <linux/cpu.h>
<<<<<<< HEAD
=======
#include <linux/pgtable.h>
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

#include <asm/ptrace.h>
#include <linux/atomic.h>
#include <asm/irq.h>
#include <asm/page.h>
<<<<<<< HEAD
#include <asm/pgtable.h>
#include <asm/io.h>
#include <asm/prom.h>
=======
#include <asm/io.h>
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#include <asm/smp.h>
#include <asm/paca.h>
#include <asm/machdep.h>
#include <asm/cputable.h>
#include <asm/firmware.h>
#include <asm/rtas.h>
<<<<<<< HEAD
#include <asm/pSeries_reconfig.h>
#include <asm/mpic.h>
#include <asm/vdso_datapage.h>
#include <asm/cputhreads.h>
#include <asm/xics.h>

#include "plpar_wrappers.h"
#include "pseries.h"
#include "offline_states.h"

=======
#include <asm/vdso_datapage.h>
#include <asm/cputhreads.h>
#include <asm/xics.h>
#include <asm/xive.h>
#include <asm/dbell.h>
#include <asm/plpar_wrappers.h>
#include <asm/code-patching.h>
#include <asm/svm.h>
#include <asm/kvm_guest.h>

#include "pseries.h"
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

/*
 * The Primary thread of each non-boot processor was started from the OF client
 * interface by prom_hold_cpus and is spinning on secondary_hold_spinloop.
 */
static cpumask_var_t of_spin_mask;

/* Query where a cpu is now.  Return codes #defined in plpar_wrappers.h */
int smp_query_cpu_stopped(unsigned int pcpu)
{
	int cpu_status, status;
<<<<<<< HEAD
	int qcss_tok = rtas_token("query-cpu-stopped-state");
=======
	int qcss_tok = rtas_function_token(RTAS_FN_QUERY_CPU_STOPPED_STATE);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	if (qcss_tok == RTAS_UNKNOWN_SERVICE) {
		printk_once(KERN_INFO
			"Firmware doesn't support query-cpu-stopped-state\n");
		return QCSS_HARDWARE_ERROR;
	}

	status = rtas_call(qcss_tok, 1, 2, &cpu_status, pcpu);
	if (status != 0) {
		printk(KERN_ERR
		       "RTAS query-cpu-stopped-state failed: %i\n", status);
		return status;
	}

	return cpu_status;
}

/**
 * smp_startup_cpu() - start the given cpu
 *
 * At boot time, there is nothing to do for primary threads which were
 * started from Open Firmware.  For anything else, call RTAS with the
 * appropriate start location.
 *
 * Returns:
 *	0	- failure
 *	1	- success
 */
<<<<<<< HEAD
static inline int __devinit smp_startup_cpu(unsigned int lcpu)
{
	int status;
	unsigned long start_here = __pa((u32)*((unsigned long *)
					       generic_secondary_smp_init));
=======
static inline int smp_startup_cpu(unsigned int lcpu)
{
	int status;
	unsigned long start_here =
			__pa(ppc_function_entry(generic_secondary_smp_init));
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	unsigned int pcpu;
	int start_cpu;

	if (cpumask_test_cpu(lcpu, of_spin_mask))
		/* Already started by OF and sitting in spin loop */
		return 1;

	pcpu = get_hard_smp_processor_id(lcpu);

	/* Check to see if the CPU out of FW already for kexec */
	if (smp_query_cpu_stopped(pcpu) == QCSS_NOT_STOPPED){
		cpumask_set_cpu(lcpu, of_spin_mask);
		return 1;
	}

<<<<<<< HEAD
	/* Fixup atomic count: it exited inside IRQ handler. */
	task_thread_info(paca[lcpu].__current)->preempt_count	= 0;
#ifdef CONFIG_HOTPLUG_CPU
	if (get_cpu_current_state(lcpu) == CPU_STATE_INACTIVE)
		goto out;
#endif
=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	/* 
	 * If the RTAS start-cpu token does not exist then presume the
	 * cpu is already spinning.
	 */
<<<<<<< HEAD
	start_cpu = rtas_token("start-cpu");
=======
	start_cpu = rtas_function_token(RTAS_FN_START_CPU);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	if (start_cpu == RTAS_UNKNOWN_SERVICE)
		return 1;

	status = rtas_call(start_cpu, 3, 1, NULL, pcpu, start_here, pcpu);
	if (status != 0) {
		printk(KERN_ERR "start-cpu failed: %i\n", status);
		return 0;
	}

<<<<<<< HEAD
#ifdef CONFIG_HOTPLUG_CPU
out:
#endif
	return 1;
}

static void __devinit smp_xics_setup_cpu(int cpu)
{
	if (cpu != boot_cpuid)
=======
	return 1;
}

static void smp_setup_cpu(int cpu)
{
	if (xive_enabled())
		xive_smp_setup_cpu();
	else if (cpu != boot_cpuid)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		xics_setup_cpu();

	if (firmware_has_feature(FW_FEATURE_SPLPAR))
		vpa_init(cpu);

	cpumask_clear_cpu(cpu, of_spin_mask);
<<<<<<< HEAD
#ifdef CONFIG_HOTPLUG_CPU
	set_cpu_current_state(cpu, CPU_STATE_ONLINE);
	set_default_offline_state(cpu);
#endif
	pseries_notify_cpuidle_add_cpu(cpu);
}

static int __devinit smp_pSeries_kick_cpu(int nr)
{
	BUG_ON(nr < 0 || nr >= NR_CPUS);
=======
}

static int smp_pSeries_kick_cpu(int nr)
{
	if (nr < 0 || nr >= nr_cpu_ids)
		return -EINVAL;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	if (!smp_startup_cpu(nr))
		return -ENOENT;

	/*
	 * The processor is currently spinning, waiting for the
	 * cpu_start field to become non-zero After we set cpu_start,
	 * the processor will continue on to secondary_start
	 */
<<<<<<< HEAD
	paca[nr].cpu_start = 1;
#ifdef CONFIG_HOTPLUG_CPU
	set_preferred_offline_state(nr, CPU_STATE_ONLINE);

	if (get_cpu_current_state(nr) == CPU_STATE_INACTIVE) {
		long rc;
		unsigned long hcpuid;

		hcpuid = get_hard_smp_processor_id(nr);
		rc = plpar_hcall_norets(H_PROD, hcpuid);
		if (rc != H_SUCCESS)
			printk(KERN_ERR "Error: Prod to wake up processor %d "
						"Ret= %ld\n", nr, rc);
	}
#endif
=======
	paca_ptrs[nr]->cpu_start = 1;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	return 0;
}

<<<<<<< HEAD
static int smp_pSeries_cpu_bootable(unsigned int nr)
{
	/* Special case - we inhibit secondary thread startup
	 * during boot if the user requests it.
	 */
	if (system_state < SYSTEM_RUNNING && cpu_has_feature(CPU_FTR_SMT)) {
		if (!smt_enabled_at_boot && cpu_thread_in_core(nr) != 0)
			return 0;
		if (smt_enabled_at_boot
		    && cpu_thread_in_core(nr) >= smt_enabled_at_boot)
			return 0;
	}

	return 1;
}

static struct smp_ops_t pSeries_mpic_smp_ops = {
	.message_pass	= smp_mpic_message_pass,
	.probe		= smp_mpic_probe,
	.kick_cpu	= smp_pSeries_kick_cpu,
	.setup_cpu	= smp_mpic_setup_cpu,
};

static struct smp_ops_t pSeries_xics_smp_ops = {
	.message_pass	= NULL,	/* Use smp_muxed_ipi_message_pass */
	.cause_ipi	= NULL,	/* Filled at runtime by xics_smp_probe() */
	.probe		= xics_smp_probe,
	.kick_cpu	= smp_pSeries_kick_cpu,
	.setup_cpu	= smp_xics_setup_cpu,
	.cpu_bootable	= smp_pSeries_cpu_bootable,
};

/* This is called very early */
static void __init smp_init_pseries(void)
=======
static int pseries_smp_prepare_cpu(int cpu)
{
	if (xive_enabled())
		return xive_smp_prepare_cpu(cpu);
	return 0;
}

/* Cause IPI as setup by the interrupt controller (xics or xive) */
static void (*ic_cause_ipi)(int cpu) __ro_after_init;

/* Use msgsndp doorbells target is a sibling, else use interrupt controller */
static void dbell_or_ic_cause_ipi(int cpu)
{
	if (doorbell_try_core_ipi(cpu))
		return;

	ic_cause_ipi(cpu);
}

static int pseries_cause_nmi_ipi(int cpu)
{
	int hwcpu;

	if (cpu == NMI_IPI_ALL_OTHERS) {
		hwcpu = H_SIGNAL_SYS_RESET_ALL_OTHERS;
	} else {
		if (cpu < 0) {
			WARN_ONCE(true, "incorrect cpu parameter %d", cpu);
			return 0;
		}

		hwcpu = get_hard_smp_processor_id(cpu);
	}

	if (plpar_signal_sys_reset(hwcpu) == H_SUCCESS)
		return 1;

	return 0;
}

static __init void pSeries_smp_probe(void)
{
	if (xive_enabled())
		xive_smp_probe();
	else
		xics_smp_probe();

	/* No doorbell facility, must use the interrupt controller for IPIs */
	if (!cpu_has_feature(CPU_FTR_DBELL))
		return;

	/* Doorbells can only be used for IPIs between SMT siblings */
	if (!cpu_has_feature(CPU_FTR_SMT))
		return;

	check_kvm_guest();

	if (is_kvm_guest()) {
		/*
		 * KVM emulates doorbells by disabling FSCR[MSGP] so msgsndp
		 * faults to the hypervisor which then reads the instruction
		 * from guest memory, which tends to be slower than using XIVE.
		 */
		if (xive_enabled())
			return;

		/*
		 * XICS hcalls aren't as fast, so we can use msgsndp (which
		 * also helps exercise KVM emulation), however KVM can't
		 * emulate secure guests because it can't read the instruction
		 * out of their memory.
		 */
		if (is_secure_guest())
			return;
	}

	/*
	 * Under PowerVM, FSCR[MSGP] is enabled as guest vCPU siblings are
	 * gang scheduled on the same physical core, so doorbells are always
	 * faster than the interrupt controller, and they can be used by
	 * secure guests.
	 */

	ic_cause_ipi = smp_ops->cause_ipi;
	smp_ops->cause_ipi = dbell_or_ic_cause_ipi;
}

static struct smp_ops_t pseries_smp_ops = {
	.message_pass	= NULL,	/* Use smp_muxed_ipi_message_pass */
	.cause_ipi	= NULL,	/* Filled at runtime by pSeries_smp_probe() */
	.cause_nmi_ipi	= pseries_cause_nmi_ipi,
	.probe		= pSeries_smp_probe,
	.prepare_cpu	= pseries_smp_prepare_cpu,
	.kick_cpu	= smp_pSeries_kick_cpu,
	.setup_cpu	= smp_setup_cpu,
	.cpu_bootable	= smp_generic_cpu_bootable,
};

/* This is called very early */
void __init smp_init_pseries(void)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
{
	int i;

	pr_debug(" -> smp_init_pSeries()\n");
<<<<<<< HEAD

	alloc_bootmem_cpumask_var(&of_spin_mask);

	/* Mark threads which are still spinning in hold loops. */
	if (cpu_has_feature(CPU_FTR_SMT)) {
		for_each_present_cpu(i) { 
			if (cpu_thread_in_core(i) == 0)
				cpumask_set_cpu(i, of_spin_mask);
		}
	} else {
		cpumask_copy(of_spin_mask, cpu_present_mask);
	}

	cpumask_clear_cpu(boot_cpuid, of_spin_mask);

	/* Non-lpar has additional take/give timebase */
	if (rtas_token("freeze-time-base") != RTAS_UNKNOWN_SERVICE) {
		smp_ops->give_timebase = rtas_give_timebase;
		smp_ops->take_timebase = rtas_take_timebase;
=======
	smp_ops = &pseries_smp_ops;

	alloc_bootmem_cpumask_var(&of_spin_mask);

	/*
	 * Mark threads which are still spinning in hold loops
	 *
	 * We know prom_init will not have started them if RTAS supports
	 * query-cpu-stopped-state.
	 */
	if (rtas_function_token(RTAS_FN_QUERY_CPU_STOPPED_STATE) == RTAS_UNKNOWN_SERVICE) {
		if (cpu_has_feature(CPU_FTR_SMT)) {
			for_each_present_cpu(i) {
				if (cpu_thread_in_core(i) == 0)
					cpumask_set_cpu(i, of_spin_mask);
			}
		} else
			cpumask_copy(of_spin_mask, cpu_present_mask);

		cpumask_clear_cpu(boot_cpuid, of_spin_mask);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	}

	pr_debug(" <- smp_init_pSeries()\n");
}
<<<<<<< HEAD

void __init smp_init_pseries_mpic(void)
{
	smp_ops = &pSeries_mpic_smp_ops;

	smp_init_pseries();
}

void __init smp_init_pseries_xics(void)
{
	smp_ops = &pSeries_xics_smp_ops;

	smp_init_pseries();
}
=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
