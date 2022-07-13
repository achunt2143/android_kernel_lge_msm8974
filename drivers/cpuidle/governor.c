/*
 * governor.c - governor support
 *
 * (C) 2006-2007 Venkatesh Pallipadi <venkatesh.pallipadi@intel.com>
 *               Shaohua Li <shaohua.li@intel.com>
 *               Adam Belay <abelay@novell.com>
 *
 * This code is licenced under the GPL.
 */

<<<<<<< HEAD
#include <linux/mutex.h>
#include <linux/module.h>
#include <linux/cpuidle.h>

#include "cpuidle.h"

LIST_HEAD(cpuidle_governors);
struct cpuidle_governor *cpuidle_curr_governor;

/**
 * __cpuidle_find_governor - finds a governor of the specified name
=======
#include <linux/cpu.h>
#include <linux/cpuidle.h>
#include <linux/mutex.h>
#include <linux/module.h>
#include <linux/pm_qos.h>

#include "cpuidle.h"

char param_governor[CPUIDLE_NAME_LEN];

LIST_HEAD(cpuidle_governors);
struct cpuidle_governor *cpuidle_curr_governor;
struct cpuidle_governor *cpuidle_prev_governor;

/**
 * cpuidle_find_governor - finds a governor of the specified name
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 * @str: the name
 *
 * Must be called with cpuidle_lock acquired.
 */
<<<<<<< HEAD
static struct cpuidle_governor * __cpuidle_find_governor(const char *str)
=======
struct cpuidle_governor *cpuidle_find_governor(const char *str)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
{
	struct cpuidle_governor *gov;

	list_for_each_entry(gov, &cpuidle_governors, governor_list)
<<<<<<< HEAD
		if (!strnicmp(str, gov->name, CPUIDLE_NAME_LEN))
=======
		if (!strncasecmp(str, gov->name, CPUIDLE_NAME_LEN))
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
			return gov;

	return NULL;
}

/**
 * cpuidle_switch_governor - changes the governor
 * @gov: the new target governor
<<<<<<< HEAD
 *
 * NOTE: "gov" can be NULL to specify disabled
=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 * Must be called with cpuidle_lock acquired.
 */
int cpuidle_switch_governor(struct cpuidle_governor *gov)
{
	struct cpuidle_device *dev;

<<<<<<< HEAD
=======
	if (!gov)
		return -EINVAL;

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	if (gov == cpuidle_curr_governor)
		return 0;

	cpuidle_uninstall_idle_handler();

	if (cpuidle_curr_governor) {
		list_for_each_entry(dev, &cpuidle_detected_devices, device_list)
			cpuidle_disable_device(dev);
<<<<<<< HEAD
		module_put(cpuidle_curr_governor->owner);
=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	}

	cpuidle_curr_governor = gov;

<<<<<<< HEAD
	if (gov) {
		if (!try_module_get(cpuidle_curr_governor->owner))
			return -EINVAL;
		list_for_each_entry(dev, &cpuidle_detected_devices, device_list)
			cpuidle_enable_device(dev);
		cpuidle_install_idle_handler();
		printk(KERN_INFO "cpuidle: using governor %s\n", gov->name);
	}
=======
	list_for_each_entry(dev, &cpuidle_detected_devices, device_list)
		cpuidle_enable_device(dev);

	cpuidle_install_idle_handler();
	pr_info("cpuidle: using governor %s\n", gov->name);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	return 0;
}

/**
 * cpuidle_register_governor - registers a governor
 * @gov: the governor
 */
int cpuidle_register_governor(struct cpuidle_governor *gov)
{
	int ret = -EEXIST;

	if (!gov || !gov->select)
		return -EINVAL;

	if (cpuidle_disabled())
		return -ENODEV;

	mutex_lock(&cpuidle_lock);
<<<<<<< HEAD
	if (__cpuidle_find_governor(gov->name) == NULL) {
		ret = 0;
		list_add_tail(&gov->governor_list, &cpuidle_governors);
		if (!cpuidle_curr_governor ||
		    cpuidle_curr_governor->rating < gov->rating)
=======
	if (cpuidle_find_governor(gov->name) == NULL) {
		ret = 0;
		list_add_tail(&gov->governor_list, &cpuidle_governors);
		if (!cpuidle_curr_governor ||
		    !strncasecmp(param_governor, gov->name, CPUIDLE_NAME_LEN) ||
		    (cpuidle_curr_governor->rating < gov->rating &&
		     strncasecmp(param_governor, cpuidle_curr_governor->name,
				 CPUIDLE_NAME_LEN)))
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
			cpuidle_switch_governor(gov);
	}
	mutex_unlock(&cpuidle_lock);

	return ret;
}

/**
<<<<<<< HEAD
 * cpuidle_replace_governor - find a replacement governor
 * @exclude_rating: the rating that will be skipped while looking for
 * new governor.
 */
static struct cpuidle_governor *cpuidle_replace_governor(int exclude_rating)
{
	struct cpuidle_governor *gov;
	struct cpuidle_governor *ret_gov = NULL;
	unsigned int max_rating = 0;

	list_for_each_entry(gov, &cpuidle_governors, governor_list) {
		if (gov->rating == exclude_rating)
			continue;
		if (gov->rating > max_rating) {
			max_rating = gov->rating;
			ret_gov = gov;
		}
	}

	return ret_gov;
}

/**
 * cpuidle_unregister_governor - unregisters a governor
 * @gov: the governor
 */
void cpuidle_unregister_governor(struct cpuidle_governor *gov)
{
	if (!gov)
		return;

	mutex_lock(&cpuidle_lock);
	if (gov == cpuidle_curr_governor) {
		struct cpuidle_governor *new_gov;
		new_gov = cpuidle_replace_governor(gov->rating);
		cpuidle_switch_governor(new_gov);
	}
	list_del(&gov->governor_list);
	mutex_unlock(&cpuidle_lock);
}

=======
 * cpuidle_governor_latency_req - Compute a latency constraint for CPU
 * @cpu: Target CPU
 */
s64 cpuidle_governor_latency_req(unsigned int cpu)
{
	struct device *device = get_cpu_device(cpu);
	int device_req = dev_pm_qos_raw_resume_latency(device);
	int global_req = cpu_latency_qos_limit();

	if (device_req > global_req)
		device_req = global_req;

	return (s64)device_req * NSEC_PER_USEC;
}
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
