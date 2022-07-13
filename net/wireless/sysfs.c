<<<<<<< HEAD
=======
// SPDX-License-Identifier: GPL-2.0-only
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
/*
 * This file provides /sys/class/ieee80211/<wiphy name>/
 * and some default attributes.
 *
 * Copyright 2005-2006	Jiri Benc <jbenc@suse.cz>
 * Copyright 2006	Johannes Berg <johannes@sipsolutions.net>
<<<<<<< HEAD
 *
 * This file is GPLv2 as found in COPYING.
=======
 * Copyright (C) 2020-2021, 2023 Intel Corporation
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 */

#include <linux/device.h>
#include <linux/module.h>
#include <linux/netdevice.h>
#include <linux/nl80211.h>
#include <linux/rtnetlink.h>
#include <net/cfg80211.h>
#include "sysfs.h"
#include "core.h"
<<<<<<< HEAD
=======
#include "rdev-ops.h"
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

static inline struct cfg80211_registered_device *dev_to_rdev(
	struct device *dev)
{
	return container_of(dev, struct cfg80211_registered_device, wiphy.dev);
}

#define SHOW_FMT(name, fmt, member)					\
static ssize_t name ## _show(struct device *dev,			\
			      struct device_attribute *attr,		\
			      char *buf)				\
{									\
	return sprintf(buf, fmt "\n", dev_to_rdev(dev)->member);	\
<<<<<<< HEAD
}
=======
}									\
static DEVICE_ATTR_RO(name)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

SHOW_FMT(index, "%d", wiphy_idx);
SHOW_FMT(macaddress, "%pM", wiphy.perm_addr);
SHOW_FMT(address_mask, "%pM", wiphy.addr_mask);

static ssize_t name_show(struct device *dev,
			 struct device_attribute *attr,
<<<<<<< HEAD
			 char *buf) {
	struct wiphy *wiphy = &dev_to_rdev(dev)->wiphy;
	return sprintf(buf, "%s\n", dev_name(&wiphy->dev));
}

=======
			 char *buf)
{
	struct wiphy *wiphy = &dev_to_rdev(dev)->wiphy;

	return sprintf(buf, "%s\n", wiphy_name(wiphy));
}
static DEVICE_ATTR_RO(name);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

static ssize_t addresses_show(struct device *dev,
			      struct device_attribute *attr,
			      char *buf)
{
	struct wiphy *wiphy = &dev_to_rdev(dev)->wiphy;
	char *start = buf;
	int i;

	if (!wiphy->addresses)
		return sprintf(buf, "%pM\n", wiphy->perm_addr);

	for (i = 0; i < wiphy->n_addresses; i++)
<<<<<<< HEAD
		buf += sprintf(buf, "%pM\n", &wiphy->addresses[i].addr);

	return buf - start;
}

static struct device_attribute ieee80211_dev_attrs[] = {
	__ATTR_RO(index),
	__ATTR_RO(macaddress),
	__ATTR_RO(address_mask),
	__ATTR_RO(addresses),
	__ATTR_RO(name),
	{}
};
=======
		buf += sprintf(buf, "%pM\n", wiphy->addresses[i].addr);

	return buf - start;
}
static DEVICE_ATTR_RO(addresses);

static struct attribute *ieee80211_attrs[] = {
	&dev_attr_index.attr,
	&dev_attr_macaddress.attr,
	&dev_attr_address_mask.attr,
	&dev_attr_addresses.attr,
	&dev_attr_name.attr,
	NULL,
};
ATTRIBUTE_GROUPS(ieee80211);
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

static void wiphy_dev_release(struct device *dev)
{
	struct cfg80211_registered_device *rdev = dev_to_rdev(dev);

	cfg80211_dev_free(rdev);
}

<<<<<<< HEAD
#ifdef CONFIG_HOTPLUG
static int wiphy_uevent(struct device *dev, struct kobj_uevent_env *env)
{
	/* TODO, we probably need stuff here */
	return 0;
}
#endif

static int wiphy_suspend(struct device *dev, pm_message_t state)
=======
#ifdef CONFIG_PM_SLEEP
static void cfg80211_leave_all(struct cfg80211_registered_device *rdev)
{
	struct wireless_dev *wdev;

	list_for_each_entry(wdev, &rdev->wiphy.wdev_list, list)
		cfg80211_leave(rdev, wdev);
}

static int wiphy_suspend(struct device *dev)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
{
	struct cfg80211_registered_device *rdev = dev_to_rdev(dev);
	int ret = 0;

<<<<<<< HEAD
	rdev->suspend_at = get_seconds();

	if (rdev->ops->suspend) {
		rtnl_lock();
		if (rdev->wiphy.registered)
			ret = rdev->ops->suspend(&rdev->wiphy, rdev->wowlan);
		rtnl_unlock();
	}
=======
	rdev->suspend_at = ktime_get_boottime_seconds();

	rtnl_lock();
	wiphy_lock(&rdev->wiphy);
	if (rdev->wiphy.registered) {
		if (!rdev->wiphy.wowlan_config) {
			cfg80211_leave_all(rdev);
			cfg80211_process_rdev_events(rdev);
		}
		cfg80211_process_wiphy_works(rdev, NULL);
		if (rdev->ops->suspend)
			ret = rdev_suspend(rdev, rdev->wiphy.wowlan_config);
		if (ret == 1) {
			/* Driver refuse to configure wowlan */
			cfg80211_leave_all(rdev);
			cfg80211_process_rdev_events(rdev);
			cfg80211_process_wiphy_works(rdev, NULL);
			ret = rdev_suspend(rdev, NULL);
		}
		if (ret == 0)
			rdev->suspended = true;
	}
	wiphy_unlock(&rdev->wiphy);
	rtnl_unlock();
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	return ret;
}

static int wiphy_resume(struct device *dev)
{
	struct cfg80211_registered_device *rdev = dev_to_rdev(dev);
	int ret = 0;

	/* Age scan results with time spent in suspend */
<<<<<<< HEAD
	spin_lock_bh(&rdev->bss_lock);
	cfg80211_bss_age(rdev, get_seconds() - rdev->suspend_at);
	spin_unlock_bh(&rdev->bss_lock);

	if (rdev->ops->resume) {
		rtnl_lock();
		if (rdev->wiphy.registered)
			ret = rdev->ops->resume(&rdev->wiphy);
		rtnl_unlock();
	}
=======
	cfg80211_bss_age(rdev, ktime_get_boottime_seconds() - rdev->suspend_at);

	rtnl_lock();
	wiphy_lock(&rdev->wiphy);
	if (rdev->wiphy.registered && rdev->ops->resume)
		ret = rdev_resume(rdev);
	rdev->suspended = false;
	schedule_work(&rdev->wiphy_work);
	wiphy_unlock(&rdev->wiphy);

	if (ret)
		cfg80211_shutdown_all_interfaces(&rdev->wiphy);

	rtnl_unlock();
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)

	return ret;
}

<<<<<<< HEAD
static const void *wiphy_namespace(struct device *d)
=======
static SIMPLE_DEV_PM_OPS(wiphy_pm_ops, wiphy_suspend, wiphy_resume);
#define WIPHY_PM_OPS (&wiphy_pm_ops)
#else
#define WIPHY_PM_OPS NULL
#endif

static const void *wiphy_namespace(const struct device *d)
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
{
	struct wiphy *wiphy = container_of(d, struct wiphy, dev);

	return wiphy_net(wiphy);
}

struct class ieee80211_class = {
	.name = "ieee80211",
<<<<<<< HEAD
	.owner = THIS_MODULE,
	.dev_release = wiphy_dev_release,
	.dev_attrs = ieee80211_dev_attrs,
#ifdef CONFIG_HOTPLUG
	.dev_uevent = wiphy_uevent,
#endif
	.suspend = wiphy_suspend,
	.resume = wiphy_resume,
=======
	.dev_release = wiphy_dev_release,
	.dev_groups = ieee80211_groups,
	.pm = WIPHY_PM_OPS,
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
	.ns_type = &net_ns_type_operations,
	.namespace = wiphy_namespace,
};

int wiphy_sysfs_init(void)
{
	return class_register(&ieee80211_class);
}

void wiphy_sysfs_exit(void)
{
	class_unregister(&ieee80211_class);
}
