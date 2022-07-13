<<<<<<< HEAD
=======
/* SPDX-License-Identifier: GPL-2.0-only */
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
/*
 * devfreq: Generic Dynamic Voltage and Frequency Scaling (DVFS) Framework
 *	    for Non-CPU Devices.
 *
 * Copyright (C) 2011 Samsung Electronics
 *	MyungJoo Ham <myungjoo.ham@samsung.com>
<<<<<<< HEAD
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 */

#ifndef __LINUX_DEVFREQ_H__
#define __LINUX_DEVFREQ_H__

#include <linux/device.h>
#include <linux/notifier.h>
<<<<<<< HEAD
#include <linux/opp.h>

#define DEVFREQ_NAME_LEN 16

struct devfreq;
=======
#include <linux/pm_opp.h>
#include <linux/pm_qos.h>

/* DEVFREQ governor name */
#define DEVFREQ_GOV_SIMPLE_ONDEMAND	"simple_ondemand"
#define DEVFREQ_GOV_PERFORMANCE		"performance"
#define DEVFREQ_GOV_POWERSAVE		"powersave"
#define DEVFREQ_GOV_USERSPACE		"userspace"
#define DEVFREQ_GOV_PASSIVE		"passive"

/* DEVFREQ notifier interface */
#define DEVFREQ_TRANSITION_NOTIFIER	(0)

/* Transition notifiers of DEVFREQ_TRANSITION_NOTIFIER */
#define	DEVFREQ_PRECHANGE		(0)
#define DEVFREQ_POSTCHANGE		(1)

/* DEVFREQ work timers */
enum devfreq_timer {
	DEVFREQ_TIMER_DEFERRABLE = 0,
	DEVFREQ_TIMER_DELAYED,
	DEVFREQ_TIMER_NUM,
};

struct devfreq;
struct devfreq_governor;
struct devfreq_cpu_data;
struct thermal_cooling_device;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

/**
 * struct devfreq_dev_status - Data given from devfreq user device to
 *			     governors. Represents the performance
 *			     statistics.
 * @total_time:		The total time represented by this instance of
 *			devfreq_dev_status
 * @busy_time:		The time that the device was working among the
 *			total_time.
 * @current_frequency:	The operating frequency.
 * @private_data:	An entry not specified by the devfreq framework.
 *			A device and a specific governor may have their
 *			own protocol with private_data. However, because
 *			this is governor-specific, a governor using this
 *			will be only compatible with devices aware of it.
 */
struct devfreq_dev_status {
	/* both since the last measure */
	unsigned long total_time;
	unsigned long busy_time;
	unsigned long current_frequency;
	void *private_data;
};

/*
 * The resulting frequency should be at most this. (this bound is the
 * least upper bound; thus, the resulting freq should be lower or same)
 * If the flag is not set, the resulting frequency should be at most the
 * bound (greatest lower bound)
 */
#define DEVFREQ_FLAG_LEAST_UPPER_BOUND		0x1

<<<<<<< HEAD
#define DEVFREQ_FLAG_FAST_HINT			0x2
#define DEVFREQ_FLAG_SLOW_HINT			0x4
#define DEVFREQ_FLAG_WAKEUP_MAXFREQ		0x8

/**
 * struct devfreq_governor_data - mapping to per device governor data
 * @name:		The name of the governor.
 * @data:		Private data for the governor.
 *
 * Devices may pass in an array of this structure to allow governors
 * to get the correct data pointer when they are enabled after
 * the devfreq_add_device() call.
 */
struct devfreq_governor_data {
	const char *name;
	void *data;
};

=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
/**
 * struct devfreq_dev_profile - Devfreq's user device profile
 * @initial_freq:	The operating frequency when devfreq_add_device() is
 *			called.
 * @polling_ms:		The polling interval in ms. 0 disables polling.
<<<<<<< HEAD
=======
 * @timer:		Timer type is either deferrable or delayed timer.
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 * @target:		The device should set its operating frequency at
 *			freq or lowest-upper-than-freq value. If freq is
 *			higher than any operable frequency, set maximum.
 *			Before returning, target function should set
 *			freq at the current frequency.
 *			The "flags" parameter's possible values are
 *			explained above with "DEVFREQ_FLAG_*" macros.
 * @get_dev_status:	The device should provide the current performance
<<<<<<< HEAD
 *			status to devfreq, which is used by governors.
=======
 *			status to devfreq. Governors are recommended not to
 *			use this directly. Instead, governors are recommended
 *			to use devfreq_update_stats() along with
 *			devfreq.last_status.
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 * @get_cur_freq:	The device should provide the current frequency
 *			at which it is operating.
 * @exit:		An optional callback that is called when devfreq
 *			is removing the devfreq object due to error or
 *			from devfreq_remove_device() call. If the user
 *			has registered devfreq->nb at a notifier-head,
 *			this is the time to unregister it.
<<<<<<< HEAD
 * @freq_table:	Optional list of frequencies to support statistics.
 * @max_state:	The size of freq_table.
 * @governor_data:	Optional array of private data for governors.
 *			This is used to set devfreq->data correctly
 *			when a governor is enabled via sysfs or other
 *			mechanisms after the devfreq_add_device() call.
 * @num_governor_data:  Number of elements in governor_data.
=======
 * @freq_table:		Optional list of frequencies to support statistics
 *			and freq_table must be generated in ascending order.
 * @max_state:		The size of freq_table.
 *
 * @is_cooling_device: A self-explanatory boolean giving the device a
 *                     cooling effect property.
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 */
struct devfreq_dev_profile {
	unsigned long initial_freq;
	unsigned int polling_ms;
<<<<<<< HEAD
=======
	enum devfreq_timer timer;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	int (*target)(struct device *dev, unsigned long *freq, u32 flags);
	int (*get_dev_status)(struct device *dev,
			      struct devfreq_dev_status *stat);
	int (*get_cur_freq)(struct device *dev, unsigned long *freq);
	void (*exit)(struct device *dev);

<<<<<<< HEAD
	unsigned int *freq_table;
	unsigned int max_state;
	const struct devfreq_governor_data *governor_data;
	unsigned int num_governor_data;
};

/**
 * struct devfreq_governor - Devfreq policy governor
 * @node:		list node - contains registered devfreq governors
 * @name:		Governor's name
 * @get_target_freq:	Returns desired operating frequency for the device.
 *			Basically, get_target_freq will run
 *			devfreq_dev_profile.get_dev_status() to get the
 *			status of the device (load = busy_time / total_time).
 *			If no_central_polling is set, this callback is called
 *			only with update_devfreq() notified by OPP.
 * @event_handler:      Callback for devfreq core framework to notify events
 *                      to governors. Events include per device governor
 *                      init and exit, opp changes out of devfreq, suspend
 *                      and resume of per device devfreq during device idle.
 *
 * Note that the callbacks are called with devfreq->lock locked by devfreq.
 */
struct devfreq_governor {
	struct list_head node;

	const char name[DEVFREQ_NAME_LEN];
	int (*get_target_freq)(struct devfreq *this, unsigned long *freq,
				u32 *flag);
	int (*event_handler)(struct devfreq *devfreq,
				unsigned int event, void *data);
#ifdef CONFIG_LGE_DEVFREQ_DFPS
	int (*init)(struct devfreq *this);
	void (*exit)(struct devfreq *this);
#endif
=======
	unsigned long *freq_table;
	unsigned int max_state;

	bool is_cooling_device;
};

/**
 * struct devfreq_stats - Statistics of devfreq device behavior
 * @total_trans:	Number of devfreq transitions.
 * @trans_table:	Statistics of devfreq transitions.
 * @time_in_state:	Statistics of devfreq states.
 * @last_update:	The last time stats were updated.
 */
struct devfreq_stats {
	unsigned int total_trans;
	unsigned int *trans_table;
	u64 *time_in_state;
	u64 last_update;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
};

/**
 * struct devfreq - Device devfreq structure
 * @node:	list node - contains the devices with devfreq that have been
 *		registered.
 * @lock:	a mutex to protect accessing devfreq.
 * @dev:	device registered by devfreq class. dev.parent is the device
 *		using devfreq.
 * @profile:	device-specific devfreq profile
 * @governor:	method how to choose frequency based on the usage.
<<<<<<< HEAD
 * @governor_name:	devfreq governor name for use with this devfreq
=======
 * @opp_table:	Reference to OPP table of dev.parent, if one exists.
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 * @nb:		notifier block used to notify devfreq object that it should
 *		reevaluate operable frequencies. Devfreq users may use
 *		devfreq.nb to the corresponding register notifier call chain.
 * @work:	delayed work for load monitoring.
<<<<<<< HEAD
 * @previous_freq:	previously configured frequency value.
 * @data:	Private data of the governor. The devfreq framework does not
 *		touch this.
 * @min_freq:	Limit minimum frequency requested by user (0: none)
 * @max_freq:	Limit maximum frequency requested by user (0: none)
 * @stop_polling:	 devfreq polling status of a device.
 * @total_trans:	Number of devfreq transitions
 * @trans_table:	Statistics of devfreq transitions
 * @time_in_state:	Statistics of devfreq states
 * @last_stat_updated:	The last time stat updated
 *
 * This structure stores the devfreq information for a give device.
=======
 * @freq_table:		current frequency table used by the devfreq driver.
 * @max_state:		count of entry present in the frequency table.
 * @previous_freq:	previously configured frequency value.
 * @last_status:	devfreq user device info, performance statistics
 * @data:	devfreq driver pass to governors, governor should not change it.
 * @governor_data:	private data for governors, devfreq core doesn't touch it.
 * @user_min_freq_req:	PM QoS minimum frequency request from user (via sysfs)
 * @user_max_freq_req:	PM QoS maximum frequency request from user (via sysfs)
 * @scaling_min_freq:	Limit minimum frequency requested by OPP interface
 * @scaling_max_freq:	Limit maximum frequency requested by OPP interface
 * @stop_polling:	 devfreq polling status of a device.
 * @suspend_freq:	 frequency of a device set during suspend phase.
 * @resume_freq:	 frequency of a device set in resume phase.
 * @suspend_count:	 suspend requests counter for a device.
 * @stats:	Statistics of devfreq device behavior
 * @transition_notifier_list: list head of DEVFREQ_TRANSITION_NOTIFIER notifier
 * @cdev:	Cooling device pointer if the devfreq has cooling property
 * @nb_min:		Notifier block for DEV_PM_QOS_MIN_FREQUENCY
 * @nb_max:		Notifier block for DEV_PM_QOS_MAX_FREQUENCY
 *
 * This structure stores the devfreq information for a given device.
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 *
 * Note that when a governor accesses entries in struct devfreq in its
 * functions except for the context of callbacks defined in struct
 * devfreq_governor, the governor should protect its access with the
 * struct mutex lock in struct devfreq. A governor may use this mutex
<<<<<<< HEAD
 * to protect its own private data in void *data as well.
=======
 * to protect its own private data in ``void *data`` as well.
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 */
struct devfreq {
	struct list_head node;

	struct mutex lock;
	struct device dev;
	struct devfreq_dev_profile *profile;
	const struct devfreq_governor *governor;
<<<<<<< HEAD
	char governor_name[DEVFREQ_NAME_LEN];
	struct notifier_block nb;
	struct delayed_work work;

	unsigned long previous_freq;

	void *data; /* private data for governors */

	unsigned long min_freq;
	unsigned long max_freq;
	bool stop_polling;

	/* information for device freqeuncy transition */
	unsigned int total_trans;
	unsigned int *trans_table;
	unsigned long *time_in_state;
	unsigned long last_stat_updated;
};

#if defined(CONFIG_PM_DEVFREQ)
extern struct devfreq *devfreq_add_device(struct device *dev,
				  struct devfreq_dev_profile *profile,
				  const char *governor_name,
				  void *data);
extern int devfreq_remove_device(struct devfreq *devfreq);
extern int devfreq_suspend_device(struct devfreq *devfreq);
extern int devfreq_resume_device(struct devfreq *devfreq);

/* Helper functions for devfreq user device driver with OPP. */
extern struct opp *devfreq_recommended_opp(struct device *dev,
					   unsigned long *freq, u32 flags);
extern int devfreq_register_opp_notifier(struct device *dev,
					 struct devfreq *devfreq);
extern int devfreq_unregister_opp_notifier(struct device *dev,
					   struct devfreq *devfreq);

#ifdef CONFIG_DEVFREQ_GOV_SIMPLE_ONDEMAND
/**
 * struct devfreq_simple_ondemand_data - void *data fed to struct devfreq
=======
	struct opp_table *opp_table;
	struct notifier_block nb;
	struct delayed_work work;

	unsigned long *freq_table;
	unsigned int max_state;

	unsigned long previous_freq;
	struct devfreq_dev_status last_status;

	void *data;
	void *governor_data;

	struct dev_pm_qos_request user_min_freq_req;
	struct dev_pm_qos_request user_max_freq_req;
	unsigned long scaling_min_freq;
	unsigned long scaling_max_freq;
	bool stop_polling;

	unsigned long suspend_freq;
	unsigned long resume_freq;
	atomic_t suspend_count;

	/* information for device frequency transitions */
	struct devfreq_stats stats;

	struct srcu_notifier_head transition_notifier_list;

	/* Pointer to the cooling device if used for thermal mitigation */
	struct thermal_cooling_device *cdev;

	struct notifier_block nb_min;
	struct notifier_block nb_max;
};

struct devfreq_freqs {
	unsigned long old;
	unsigned long new;
};

#if defined(CONFIG_PM_DEVFREQ)
struct devfreq *devfreq_add_device(struct device *dev,
				struct devfreq_dev_profile *profile,
				const char *governor_name,
				void *data);
int devfreq_remove_device(struct devfreq *devfreq);
struct devfreq *devm_devfreq_add_device(struct device *dev,
				struct devfreq_dev_profile *profile,
				const char *governor_name,
				void *data);
void devm_devfreq_remove_device(struct device *dev, struct devfreq *devfreq);

/* Supposed to be called by PM callbacks */
int devfreq_suspend_device(struct devfreq *devfreq);
int devfreq_resume_device(struct devfreq *devfreq);

void devfreq_suspend(void);
void devfreq_resume(void);

/* update_devfreq() - Reevaluate the device and configure frequency */
int update_devfreq(struct devfreq *devfreq);

/* Helper functions for devfreq user device driver with OPP. */
struct dev_pm_opp *devfreq_recommended_opp(struct device *dev,
				unsigned long *freq, u32 flags);
int devfreq_register_opp_notifier(struct device *dev,
				struct devfreq *devfreq);
int devfreq_unregister_opp_notifier(struct device *dev,
				struct devfreq *devfreq);
int devm_devfreq_register_opp_notifier(struct device *dev,
				struct devfreq *devfreq);
void devm_devfreq_unregister_opp_notifier(struct device *dev,
				struct devfreq *devfreq);
int devfreq_register_notifier(struct devfreq *devfreq,
				struct notifier_block *nb,
				unsigned int list);
int devfreq_unregister_notifier(struct devfreq *devfreq,
				struct notifier_block *nb,
				unsigned int list);
int devm_devfreq_register_notifier(struct device *dev,
				struct devfreq *devfreq,
				struct notifier_block *nb,
				unsigned int list);
void devm_devfreq_unregister_notifier(struct device *dev,
				struct devfreq *devfreq,
				struct notifier_block *nb,
				unsigned int list);
struct devfreq *devfreq_get_devfreq_by_node(struct device_node *node);
struct devfreq *devfreq_get_devfreq_by_phandle(struct device *dev,
				const char *phandle_name, int index);
#endif /* CONFIG_PM_DEVFREQ */

/**
 * struct devfreq_simple_ondemand_data - ``void *data`` fed to struct devfreq
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 *	and devfreq_add_device
 * @upthreshold:	If the load is over this value, the frequency jumps.
 *			Specify 0 to use the default. Valid value = 0 to 100.
 * @downdifferential:	If the load is under upthreshold - downdifferential,
 *			the governor may consider slowing the frequency down.
 *			Specify 0 to use the default. Valid value = 0 to 100.
 *			downdifferential < upthreshold must hold.
<<<<<<< HEAD
 * @simple_scaling:	Setting this flag will scale the clocks up only if the
 *			load is above @upthreshold and will scale the clocks
 *			down only if the load is below @downdifferential.
=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 *
 * If the fed devfreq_simple_ondemand_data pointer is NULL to the governor,
 * the governor uses the default values.
 */
struct devfreq_simple_ondemand_data {
	unsigned int upthreshold;
	unsigned int downdifferential;
<<<<<<< HEAD
	unsigned int simple_scaling;
};
#endif

#else /* !CONFIG_PM_DEVFREQ */
static struct devfreq *devfreq_add_device(struct device *dev,
					  struct devfreq_dev_profile *profile,
					  const char *governor_name,
					  void *data)
{
	return NULL;
}

static int devfreq_remove_device(struct devfreq *devfreq)
=======
};

enum devfreq_parent_dev_type {
	DEVFREQ_PARENT_DEV,
	CPUFREQ_PARENT_DEV,
};

/**
 * struct devfreq_passive_data - ``void *data`` fed to struct devfreq
 *	and devfreq_add_device
 * @parent:	the devfreq instance of parent device.
 * @get_target_freq:	Optional callback, Returns desired operating frequency
 *			for the device using passive governor. That is called
 *			when passive governor should decide the next frequency
 *			by using the new frequency of parent devfreq device
 *			using governors except for passive governor.
 *			If the devfreq device has the specific method to decide
 *			the next frequency, should use this callback.
 * @parent_type:	the parent type of the device.
 * @this:		the devfreq instance of own device.
 * @nb:			the notifier block for DEVFREQ_TRANSITION_NOTIFIER or
 *			CPUFREQ_TRANSITION_NOTIFIER list.
 * @cpu_data_list:	the list of cpu frequency data for all cpufreq_policy.
 *
 * The devfreq_passive_data have to set the devfreq instance of parent
 * device with governors except for the passive governor. But, don't need to
 * initialize the 'this' and 'nb' field because the devfreq core will handle
 * them.
 */
struct devfreq_passive_data {
	/* Should set the devfreq instance of parent device */
	struct devfreq *parent;

	/* Optional callback to decide the next frequency of passvice device */
	int (*get_target_freq)(struct devfreq *this, unsigned long *freq);

	/* Should set the type of parent device */
	enum devfreq_parent_dev_type parent_type;

	/* For passive governor's internal use. Don't need to set them */
	struct devfreq *this;
	struct notifier_block nb;
	struct list_head cpu_data_list;
};

#if !defined(CONFIG_PM_DEVFREQ)
static inline struct devfreq *devfreq_add_device(struct device *dev,
					struct devfreq_dev_profile *profile,
					const char *governor_name,
					void *data)
{
	return ERR_PTR(-ENOSYS);
}

static inline int devfreq_remove_device(struct devfreq *devfreq)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
{
	return 0;
}

<<<<<<< HEAD
static int devfreq_suspend_device(struct devfreq *devfreq)
=======
static inline struct devfreq *devm_devfreq_add_device(struct device *dev,
					struct devfreq_dev_profile *profile,
					const char *governor_name,
					void *data)
{
	return ERR_PTR(-ENOSYS);
}

static inline void devm_devfreq_remove_device(struct device *dev,
					struct devfreq *devfreq)
{
}

static inline int devfreq_suspend_device(struct devfreq *devfreq)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
{
	return 0;
}

<<<<<<< HEAD
static int devfreq_resume_device(struct devfreq *devfreq)
=======
static inline int devfreq_resume_device(struct devfreq *devfreq)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
{
	return 0;
}

<<<<<<< HEAD
static struct opp *devfreq_recommended_opp(struct device *dev,
					   unsigned long *freq, u32 flags)
=======
static inline void devfreq_suspend(void) {}
static inline void devfreq_resume(void) {}

static inline struct dev_pm_opp *devfreq_recommended_opp(struct device *dev,
					unsigned long *freq, u32 flags)
{
	return ERR_PTR(-EINVAL);
}

static inline int devfreq_register_opp_notifier(struct device *dev,
					struct devfreq *devfreq)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
{
	return -EINVAL;
}

<<<<<<< HEAD
static int devfreq_register_opp_notifier(struct device *dev,
					 struct devfreq *devfreq)
=======
static inline int devfreq_unregister_opp_notifier(struct device *dev,
					struct devfreq *devfreq)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
{
	return -EINVAL;
}

<<<<<<< HEAD
static int devfreq_unregister_opp_notifier(struct device *dev,
					   struct devfreq *devfreq)
=======
static inline int devm_devfreq_register_opp_notifier(struct device *dev,
					struct devfreq *devfreq)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
{
	return -EINVAL;
}

<<<<<<< HEAD
=======
static inline void devm_devfreq_unregister_opp_notifier(struct device *dev,
					struct devfreq *devfreq)
{
}

static inline int devfreq_register_notifier(struct devfreq *devfreq,
					struct notifier_block *nb,
					unsigned int list)
{
	return 0;
}

static inline int devfreq_unregister_notifier(struct devfreq *devfreq,
					struct notifier_block *nb,
					unsigned int list)
{
	return 0;
}

static inline int devm_devfreq_register_notifier(struct device *dev,
					struct devfreq *devfreq,
					struct notifier_block *nb,
					unsigned int list)
{
	return 0;
}

static inline void devm_devfreq_unregister_notifier(struct device *dev,
					struct devfreq *devfreq,
					struct notifier_block *nb,
					unsigned int list)
{
}

static inline struct devfreq *devfreq_get_devfreq_by_node(struct device_node *node)
{
	return ERR_PTR(-ENODEV);
}

static inline struct devfreq *devfreq_get_devfreq_by_phandle(struct device *dev,
					const char *phandle_name, int index)
{
	return ERR_PTR(-ENODEV);
}

static inline int devfreq_update_stats(struct devfreq *df)
{
	return -EINVAL;
}
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#endif /* CONFIG_PM_DEVFREQ */

#endif /* __LINUX_DEVFREQ_H__ */
