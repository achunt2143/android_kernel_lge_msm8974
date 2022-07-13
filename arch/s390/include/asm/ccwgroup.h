<<<<<<< HEAD
=======
/* SPDX-License-Identifier: GPL-2.0 */
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#ifndef S390_CCWGROUP_H
#define S390_CCWGROUP_H

struct ccw_device;
struct ccw_driver;

/**
 * struct ccwgroup_device - ccw group device
<<<<<<< HEAD
 * @creator_id: unique number of the driver
=======
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 * @state: online/offline state
 * @count: number of attached slave devices
 * @dev: embedded device structure
 * @cdev: variable number of slave devices, allocated as needed
<<<<<<< HEAD
 */
struct ccwgroup_device {
	unsigned long creator_id;
=======
 * @ungroup_work: used to ungroup the ccwgroup device
 */
struct ccwgroup_device {
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	enum {
		CCWGROUP_OFFLINE,
		CCWGROUP_ONLINE,
	} state;
/* private: */
	atomic_t onoff;
	struct mutex reg_mutex;
/* public: */
	unsigned int count;
	struct device	dev;
<<<<<<< HEAD
	struct ccw_device *cdev[0];
=======
	struct work_struct ungroup_work;
	struct ccw_device *cdev[];
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
};

/**
 * struct ccwgroup_driver - driver for ccw group devices
<<<<<<< HEAD
 * @max_slaves: maximum number of slave devices
 * @driver_id: unique id
 * @probe: function called on probe
=======
 * @setup: function called during device creation to setup the device
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 * @remove: function called on remove
 * @set_online: function called when device is set online
 * @set_offline: function called when device is set offline
 * @shutdown: function called when device is shut down
<<<<<<< HEAD
 * @prepare: prepare for pm state transition
 * @complete: undo work done in @prepare
 * @freeze: callback for freezing during hibernation snapshotting
 * @thaw: undo work done in @freeze
 * @restore: callback for restoring after hibernation
 * @driver: embedded driver structure
 */
struct ccwgroup_driver {
	int max_slaves;
	unsigned long driver_id;

	int (*probe) (struct ccwgroup_device *);
=======
 * @driver: embedded driver structure
 * @ccw_driver: supported ccw_driver (optional)
 */
struct ccwgroup_driver {
	int (*setup) (struct ccwgroup_device *);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	void (*remove) (struct ccwgroup_device *);
	int (*set_online) (struct ccwgroup_device *);
	int (*set_offline) (struct ccwgroup_device *);
	void (*shutdown)(struct ccwgroup_device *);
<<<<<<< HEAD
	int (*prepare) (struct ccwgroup_device *);
	void (*complete) (struct ccwgroup_device *);
	int (*freeze)(struct ccwgroup_device *);
	int (*thaw) (struct ccwgroup_device *);
	int (*restore)(struct ccwgroup_device *);

	struct device_driver driver;
=======

	struct device_driver driver;
	struct ccw_driver *ccw_driver;
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
};

extern int  ccwgroup_driver_register   (struct ccwgroup_driver *cdriver);
extern void ccwgroup_driver_unregister (struct ccwgroup_driver *cdriver);
<<<<<<< HEAD
int ccwgroup_create_from_string(struct device *root, unsigned int creator_id,
				struct ccw_driver *cdrv, int num_devices,
				const char *buf);
=======
int ccwgroup_create_dev(struct device *root, struct ccwgroup_driver *gdrv,
			int num_devices, const char *buf);

extern int ccwgroup_set_online(struct ccwgroup_device *gdev);
int ccwgroup_set_offline(struct ccwgroup_device *gdev, bool call_gdrv);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

extern int ccwgroup_probe_ccwdev(struct ccw_device *cdev);
extern void ccwgroup_remove_ccwdev(struct ccw_device *cdev);

#define to_ccwgroupdev(x) container_of((x), struct ccwgroup_device, dev)
#define to_ccwgroupdrv(x) container_of((x), struct ccwgroup_driver, driver)
<<<<<<< HEAD
=======

#if IS_ENABLED(CONFIG_CCWGROUP)
bool dev_is_ccwgroup(struct device *dev);
#else /* CONFIG_CCWGROUP */
static inline bool dev_is_ccwgroup(struct device *dev)
{
	return false;
}
#endif /* CONFIG_CCWGROUP */

>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#endif
