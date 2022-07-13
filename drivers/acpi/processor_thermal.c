<<<<<<< HEAD
=======
// SPDX-License-Identifier: GPL-2.0-or-later
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
/*
 * processor_thermal.c - Passive cooling submodule of the ACPI processor driver
 *
 *  Copyright (C) 2001, 2002 Andy Grover <andrew.grover@intel.com>
 *  Copyright (C) 2001, 2002 Paul Diefenbaugh <paul.s.diefenbaugh@intel.com>
 *  Copyright (C) 2004       Dominik Brodowski <linux@brodo.de>
 *  Copyright (C) 2004  Anil S Keshavamurthy <anil.s.keshavamurthy@intel.com>
 *  			- Added processor hotplug support
<<<<<<< HEAD
 *
 * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or (at
 *  your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful, but
 *  WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 *  General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License along
 *  with this program; if not, write to the Free Software Foundation, Inc.,
 *  59 Temple Place, Suite 330, Boston, MA 02111-1307 USA.
 *
 * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 */

#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/init.h>
#include <linux/cpufreq.h>
<<<<<<< HEAD

#include <asm/uaccess.h>

#include <acpi/acpi_bus.h>
#include <acpi/processor.h>
#include <acpi/acpi_drivers.h>

#define PREFIX "ACPI: "

#define ACPI_PROCESSOR_CLASS            "processor"
#define _COMPONENT              ACPI_PROCESSOR_COMPONENT
ACPI_MODULE_NAME("processor_thermal");
=======
#include <linux/acpi.h>
#include <acpi/processor.h>
#include <linux/uaccess.h>

#include "internal.h"
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

#ifdef CONFIG_CPU_FREQ

/* If a passive cooling situation is detected, primarily CPUfreq is used, as it
 * offers (in most cases) voltage scaling in addition to frequency scaling, and
 * thus a cubic (instead of linear) reduction of energy. Also, we allow for
 * _any_ cpufreq driver and not only the acpi-cpufreq driver.
 */

#define CPUFREQ_THERMAL_MIN_STEP 0
<<<<<<< HEAD
#define CPUFREQ_THERMAL_MAX_STEP 3

static DEFINE_PER_CPU(unsigned int, cpufreq_thermal_reduction_pctg);
static unsigned int acpi_thermal_cpufreq_is_init = 0;

#define reduction_pctg(cpu) \
	per_cpu(cpufreq_thermal_reduction_pctg, phys_package_first_cpu(cpu))
=======

static int cpufreq_thermal_max_step __read_mostly = 3;

/*
 * Minimum throttle percentage for processor_thermal cooling device.
 * The processor_thermal driver uses it to calculate the percentage amount by
 * which cpu frequency must be reduced for each cooling state. This is also used
 * to calculate the maximum number of throttling steps or cooling states.
 */
static int cpufreq_thermal_reduction_pctg __read_mostly = 20;

static DEFINE_PER_CPU(unsigned int, cpufreq_thermal_reduction_step);

#define reduction_step(cpu) \
	per_cpu(cpufreq_thermal_reduction_step, phys_package_first_cpu(cpu))
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

/*
 * Emulate "per package data" using per cpu data (which should really be
 * provided elsewhere)
 *
 * Note we can lose a CPU on cpu hotunplug, in this case we forget the state
 * temporarily. Fortunately that's not a big issue here (I hope)
 */
static int phys_package_first_cpu(int cpu)
{
	int i;
	int id = topology_physical_package_id(cpu);

	for_each_online_cpu(i)
		if (topology_physical_package_id(i) == id)
			return i;
	return 0;
}

static int cpu_has_cpufreq(unsigned int cpu)
{
<<<<<<< HEAD
	struct cpufreq_policy policy;
	if (!acpi_thermal_cpufreq_is_init || cpufreq_get_policy(&policy, cpu))
		return 0;
	return 1;
}

static int acpi_thermal_cpufreq_notifier(struct notifier_block *nb,
					 unsigned long event, void *data)
{
	struct cpufreq_policy *policy = data;
	unsigned long max_freq = 0;

	if (event != CPUFREQ_ADJUST)
		goto out;

	max_freq = (
	    policy->cpuinfo.max_freq *
	    (100 - reduction_pctg(policy->cpu) * 20)
	) / 100;

	cpufreq_verify_within_limits(policy, 0, max_freq);

      out:
	return 0;
}

static struct notifier_block acpi_thermal_cpufreq_notifier_block = {
	.notifier_call = acpi_thermal_cpufreq_notifier,
};

=======
	struct cpufreq_policy *policy;

	if (!acpi_processor_cpufreq_init)
		return 0;

	policy = cpufreq_cpu_get(cpu);
	if (policy) {
		cpufreq_cpu_put(policy);
		return 1;
	}
	return 0;
}

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
static int cpufreq_get_max_state(unsigned int cpu)
{
	if (!cpu_has_cpufreq(cpu))
		return 0;

<<<<<<< HEAD
	return CPUFREQ_THERMAL_MAX_STEP;
=======
	return cpufreq_thermal_max_step;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

static int cpufreq_get_cur_state(unsigned int cpu)
{
	if (!cpu_has_cpufreq(cpu))
		return 0;

<<<<<<< HEAD
	return reduction_pctg(cpu);
=======
	return reduction_step(cpu);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
}

static int cpufreq_set_cur_state(unsigned int cpu, int state)
{
<<<<<<< HEAD
	int i;
=======
	struct cpufreq_policy *policy;
	struct acpi_processor *pr;
	unsigned long max_freq;
	int i, ret;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	if (!cpu_has_cpufreq(cpu))
		return 0;

<<<<<<< HEAD
	reduction_pctg(cpu) = state;
=======
	reduction_step(cpu) = state;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	/*
	 * Update all the CPUs in the same package because they all
	 * contribute to the temperature and often share the same
	 * frequency.
	 */
	for_each_online_cpu(i) {
<<<<<<< HEAD
		if (topology_physical_package_id(i) ==
		    topology_physical_package_id(cpu))
			cpufreq_update_policy(i);
=======
		if (topology_physical_package_id(i) !=
		    topology_physical_package_id(cpu))
			continue;

		pr = per_cpu(processors, i);

		if (unlikely(!freq_qos_request_active(&pr->thermal_req)))
			continue;

		policy = cpufreq_cpu_get(i);
		if (!policy)
			return -EINVAL;

		max_freq = (policy->cpuinfo.max_freq *
			    (100 - reduction_step(i) * cpufreq_thermal_reduction_pctg)) / 100;

		cpufreq_cpu_put(policy);

		ret = freq_qos_update_request(&pr->thermal_req, max_freq);
		if (ret < 0) {
			pr_warn("Failed to update thermal freq constraint: CPU%d (%d)\n",
				pr->id, ret);
		}
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	}
	return 0;
}

<<<<<<< HEAD
void acpi_thermal_cpufreq_init(void)
{
	int i;

	i = cpufreq_register_notifier(&acpi_thermal_cpufreq_notifier_block,
				      CPUFREQ_POLICY_NOTIFIER);
	if (!i)
		acpi_thermal_cpufreq_is_init = 1;
}

void acpi_thermal_cpufreq_exit(void)
{
	if (acpi_thermal_cpufreq_is_init)
		cpufreq_unregister_notifier
		    (&acpi_thermal_cpufreq_notifier_block,
		     CPUFREQ_POLICY_NOTIFIER);

	acpi_thermal_cpufreq_is_init = 0;
}

=======
static void acpi_thermal_cpufreq_config(void)
{
	int cpufreq_pctg = acpi_arch_thermal_cpufreq_pctg();

	if (!cpufreq_pctg)
		return;

	cpufreq_thermal_reduction_pctg = cpufreq_pctg;

	/*
	 * Derive the MAX_STEP from minimum throttle percentage so that the reduction
	 * percentage doesn't end up becoming negative. Also, cap the MAX_STEP so that
	 * the CPU performance doesn't become 0.
	 */
	cpufreq_thermal_max_step = (100 / cpufreq_pctg) - 2;
}

void acpi_thermal_cpufreq_init(struct cpufreq_policy *policy)
{
	unsigned int cpu;

	acpi_thermal_cpufreq_config();

	for_each_cpu(cpu, policy->related_cpus) {
		struct acpi_processor *pr = per_cpu(processors, cpu);
		int ret;

		if (!pr)
			continue;

		ret = freq_qos_add_request(&policy->constraints,
					   &pr->thermal_req,
					   FREQ_QOS_MAX, INT_MAX);
		if (ret < 0) {
			pr_err("Failed to add freq constraint for CPU%d (%d)\n",
			       cpu, ret);
			continue;
		}

		thermal_cooling_device_update(pr->cdev);
	}
}

void acpi_thermal_cpufreq_exit(struct cpufreq_policy *policy)
{
	unsigned int cpu;

	for_each_cpu(cpu, policy->related_cpus) {
		struct acpi_processor *pr = per_cpu(processors, cpu);

		if (!pr)
			continue;

		freq_qos_remove_request(&pr->thermal_req);

		thermal_cooling_device_update(pr->cdev);
	}
}
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#else				/* ! CONFIG_CPU_FREQ */
static int cpufreq_get_max_state(unsigned int cpu)
{
	return 0;
}

static int cpufreq_get_cur_state(unsigned int cpu)
{
	return 0;
}

static int cpufreq_set_cur_state(unsigned int cpu, int state)
{
	return 0;
}

#endif

<<<<<<< HEAD
int acpi_processor_get_limit_info(struct acpi_processor *pr)
{

	if (!pr)
		return -EINVAL;

	if (pr->flags.throttling)
		pr->flags.limit = 1;

	return 0;
}

/* thermal coolign device callbacks */
=======
/* thermal cooling device callbacks */
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
static int acpi_processor_max_state(struct acpi_processor *pr)
{
	int max_state = 0;

	/*
	 * There exists four states according to
<<<<<<< HEAD
	 * cpufreq_thermal_reduction_ptg. 0, 1, 2, 3
=======
	 * cpufreq_thermal_reduction_step. 0, 1, 2, 3
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	 */
	max_state += cpufreq_get_max_state(pr->id);
	if (pr->flags.throttling)
		max_state += (pr->throttling.state_count -1);

	return max_state;
}
static int
processor_get_max_state(struct thermal_cooling_device *cdev,
			unsigned long *state)
{
	struct acpi_device *device = cdev->devdata;
<<<<<<< HEAD
	struct acpi_processor *pr = acpi_driver_data(device);

	if (!device || !pr)
=======
	struct acpi_processor *pr;

	if (!device)
		return -EINVAL;

	pr = acpi_driver_data(device);
	if (!pr)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		return -EINVAL;

	*state = acpi_processor_max_state(pr);
	return 0;
}

static int
processor_get_cur_state(struct thermal_cooling_device *cdev,
			unsigned long *cur_state)
{
	struct acpi_device *device = cdev->devdata;
<<<<<<< HEAD
	struct acpi_processor *pr = acpi_driver_data(device);

	if (!device || !pr)
=======
	struct acpi_processor *pr;

	if (!device)
		return -EINVAL;

	pr = acpi_driver_data(device);
	if (!pr)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		return -EINVAL;

	*cur_state = cpufreq_get_cur_state(pr->id);
	if (pr->flags.throttling)
		*cur_state += pr->throttling.state;
	return 0;
}

static int
processor_set_cur_state(struct thermal_cooling_device *cdev,
			unsigned long state)
{
	struct acpi_device *device = cdev->devdata;
<<<<<<< HEAD
	struct acpi_processor *pr = acpi_driver_data(device);
	int result = 0;
	int max_pstate;

	if (!device || !pr)
=======
	struct acpi_processor *pr;
	int result = 0;
	int max_pstate;

	if (!device)
		return -EINVAL;

	pr = acpi_driver_data(device);
	if (!pr)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
		return -EINVAL;

	max_pstate = cpufreq_get_max_state(pr->id);

	if (state > acpi_processor_max_state(pr))
		return -EINVAL;

	if (state <= max_pstate) {
		if (pr->flags.throttling && pr->throttling.state)
			result = acpi_processor_set_throttling(pr, 0, false);
		cpufreq_set_cur_state(pr->id, state);
	} else {
		cpufreq_set_cur_state(pr->id, max_pstate);
		result = acpi_processor_set_throttling(pr,
				state - max_pstate, false);
	}
	return result;
}

const struct thermal_cooling_device_ops processor_cooling_ops = {
	.get_max_state = processor_get_max_state,
	.get_cur_state = processor_get_cur_state,
	.set_cur_state = processor_set_cur_state,
};
<<<<<<< HEAD
=======

int acpi_processor_thermal_init(struct acpi_processor *pr,
				struct acpi_device *device)
{
	int result = 0;

	pr->cdev = thermal_cooling_device_register("Processor", device,
						   &processor_cooling_ops);
	if (IS_ERR(pr->cdev)) {
		result = PTR_ERR(pr->cdev);
		return result;
	}

	dev_dbg(&device->dev, "registered as cooling_device%d\n",
		pr->cdev->id);

	result = sysfs_create_link(&device->dev.kobj,
				   &pr->cdev->device.kobj,
				   "thermal_cooling");
	if (result) {
		dev_err(&device->dev,
			"Failed to create sysfs link 'thermal_cooling'\n");
		goto err_thermal_unregister;
	}

	result = sysfs_create_link(&pr->cdev->device.kobj,
				   &device->dev.kobj,
				   "device");
	if (result) {
		dev_err(&pr->cdev->device,
			"Failed to create sysfs link 'device'\n");
		goto err_remove_sysfs_thermal;
	}

	return 0;

err_remove_sysfs_thermal:
	sysfs_remove_link(&device->dev.kobj, "thermal_cooling");
err_thermal_unregister:
	thermal_cooling_device_unregister(pr->cdev);

	return result;
}

void acpi_processor_thermal_exit(struct acpi_processor *pr,
				 struct acpi_device *device)
{
	if (pr->cdev) {
		sysfs_remove_link(&device->dev.kobj, "thermal_cooling");
		sysfs_remove_link(&pr->cdev->device.kobj, "device");
		thermal_cooling_device_unregister(pr->cdev);
		pr->cdev = NULL;
	}
}
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
