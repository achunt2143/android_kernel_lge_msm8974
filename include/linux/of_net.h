<<<<<<< HEAD
/*
 * OF helpers for network devices.
 *
 * This file is released under the GPLv2
=======
/* SPDX-License-Identifier: GPL-2.0-only */
/*
 * OF helpers for network devices.
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
 */

#ifndef __LINUX_OF_NET_H
#define __LINUX_OF_NET_H

<<<<<<< HEAD
#ifdef CONFIG_OF_NET
#include <linux/of.h>
extern const int of_get_phy_mode(struct device_node *np);
extern const void *of_get_mac_address(struct device_node *np);
=======
#include <linux/phy.h>

#if defined(CONFIG_OF) && defined(CONFIG_NET)
#include <linux/of.h>

struct net_device;
extern int of_get_phy_mode(struct device_node *np, phy_interface_t *interface);
extern int of_get_mac_address(struct device_node *np, u8 *mac);
extern int of_get_mac_address_nvmem(struct device_node *np, u8 *mac);
int of_get_ethdev_address(struct device_node *np, struct net_device *dev);
extern struct net_device *of_find_net_device_by_node(struct device_node *np);
#else
static inline int of_get_phy_mode(struct device_node *np,
				  phy_interface_t *interface)
{
	return -ENODEV;
}

static inline int of_get_mac_address(struct device_node *np, u8 *mac)
{
	return -ENODEV;
}

static inline int of_get_mac_address_nvmem(struct device_node *np, u8 *mac)
{
	return -ENODEV;
}

static inline int of_get_ethdev_address(struct device_node *np, struct net_device *dev)
{
	return -ENODEV;
}

static inline struct net_device *of_find_net_device_by_node(struct device_node *np)
{
	return NULL;
}
>>>>>>> 26f1d324c6e (tools: use basename to identify file in gen-mach-types)
#endif

#endif /* __LINUX_OF_NET_H */
