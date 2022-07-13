<<<<<<< HEAD
=======
/* SPDX-License-Identifier: GPL-2.0 */
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
/*
 * cpuidle.h - The internal header file
 */

#ifndef __DRIVER_CPUIDLE_H
#define __DRIVER_CPUIDLE_H

<<<<<<< HEAD
#include <linux/device.h>

/* For internal use only */
extern struct cpuidle_governor *cpuidle_curr_governor;
=======
/* For internal use only */
extern char param_governor[];
extern struct cpuidle_governor *cpuidle_curr_governor;
extern struct cpuidle_governor *cpuidle_prev_governor;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
extern struct list_head cpuidle_governors;
extern struct list_head cpuidle_detected_devices;
extern struct mutex cpuidle_lock;
extern spinlock_t cpuidle_driver_lock;
extern int cpuidle_disabled(void);
extern int cpuidle_enter_state(struct cpuidle_device *dev,
		struct cpuidle_driver *drv, int next_state);

/* idle loop */
extern void cpuidle_install_idle_handler(void);
extern void cpuidle_uninstall_idle_handler(void);

/* governors */
<<<<<<< HEAD
extern int cpuidle_switch_governor(struct cpuidle_governor *gov);

/* sysfs */
extern int cpuidle_add_interface(struct device *dev);
extern void cpuidle_remove_interface(struct device *dev);
extern int cpuidle_add_state_sysfs(struct cpuidle_device *device);
extern void cpuidle_remove_state_sysfs(struct cpuidle_device *device);
extern int cpuidle_add_sysfs(struct device *dev);
extern void cpuidle_remove_sysfs(struct device *dev);

#ifdef CONFIG_ARCH_NEEDS_CPU_IDLE_COUPLED
bool cpuidle_state_is_coupled(struct cpuidle_device *dev,
		struct cpuidle_driver *drv, int state);
=======
extern struct cpuidle_governor *cpuidle_find_governor(const char *str);
extern int cpuidle_switch_governor(struct cpuidle_governor *gov);

/* sysfs */

struct device;

extern int cpuidle_add_interface(void);
extern void cpuidle_remove_interface(struct device *dev);
extern int cpuidle_add_device_sysfs(struct cpuidle_device *device);
extern void cpuidle_remove_device_sysfs(struct cpuidle_device *device);
extern int cpuidle_add_sysfs(struct cpuidle_device *dev);
extern void cpuidle_remove_sysfs(struct cpuidle_device *dev);

#ifdef CONFIG_ARCH_NEEDS_CPU_IDLE_COUPLED
bool cpuidle_state_is_coupled(struct cpuidle_driver *drv, int state);
int cpuidle_coupled_state_verify(struct cpuidle_driver *drv);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
int cpuidle_enter_state_coupled(struct cpuidle_device *dev,
		struct cpuidle_driver *drv, int next_state);
int cpuidle_coupled_register_device(struct cpuidle_device *dev);
void cpuidle_coupled_unregister_device(struct cpuidle_device *dev);
#else
<<<<<<< HEAD
static inline bool cpuidle_state_is_coupled(struct cpuidle_device *dev,
		struct cpuidle_driver *drv, int state)
=======
static inline
bool cpuidle_state_is_coupled(struct cpuidle_driver *drv, int state)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
{
	return false;
}

<<<<<<< HEAD
=======
static inline int cpuidle_coupled_state_verify(struct cpuidle_driver *drv)
{
	return 0;
}

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
static inline int cpuidle_enter_state_coupled(struct cpuidle_device *dev,
		struct cpuidle_driver *drv, int next_state)
{
	return -1;
}

static inline int cpuidle_coupled_register_device(struct cpuidle_device *dev)
{
	return 0;
}

static inline void cpuidle_coupled_unregister_device(struct cpuidle_device *dev)
{
}
#endif

#endif /* __DRIVER_CPUIDLE_H */
